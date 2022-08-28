#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include "Server.hpp"

namespace Service
{
HttpServer::HttpServer(const std::uint16_t port,const std::string address):listenPort(port), listenAddress(address)
{
    ePoll.reset(new lib::event::EPoll());
    return;
}

bool HttpServer::start()
{
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket == INVAILD_SOCKET)
    {
        std::cout << "Create socket failure!" << std::endl;
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(listenAddress.c_str());
    serverAddr.sin_port = htons(listenPort);

    if(bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Bind socket failure!" << std::endl;
        return false;
    }

    if(listen(listenSocket, 5) < 0)//5->半连接数
    {
        std::cout << "Listen socket failure!" << std::endl;
        return false;
    }

    ePoll->start(MAX_EPOLLSIZE);

    struct epoll_event ev = {};
    std::unique_ptr<lib::event::sockitem> si(new lib::event::sockitem());
    si->sockfd = listenSocket;
	ev.data.ptr = si.get();
    ev.events = EPOLLIN;
    ev.data.fd = listenSocket;

    ePoll->epollAdd(listenSocket, ev);

    std::cout << "HttpServer start successful!" << std::endl;
    return true;
}

void HttpServer::stop()
{
    return;
}

int HttpServer::accept_cb(int fd, int events)
{
    struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int clientfd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (clientfd <= 0) return -1;

	char str[INET_ADDRSTRLEN] = {0};
	printf("recv from %s at port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
		ntohs(client_addr.sin_port));

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = clientfd;

	std::unique_ptr<lib::event::sockitem> si(new lib::event::sockitem());
	si->sockfd = clientfd;
	ev.data.ptr = si.get();;

	ePoll->epollAdd(clientfd, ev);

	return clientfd;
}

void HttpServer::process()
{
    for(;;)
    {
        std::int32_t nready = ePoll->epollWait();
		if (nready < -1) {
			break;
		}
        for(int i = 0; i < nready; i++)
        {
            auto events = ePoll->getEpollEvents();
            if(events.at(i).data.fd == listenSocket)
            {
                accept_cb(listenSocket, events.at(i).events);
            }
        }
    }
}

HttpServer::~HttpServer()
{
    return;
}

}