#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include "Server.hpp"

namespace Service
{
HttpServer::HttpServer(std::uint16_t port, std::string address):listenPort(port), listenSocket(INVAILD_SOCKET), listenAddress(address)
{
    return;
}

bool HttpServer::start()
{
std::cout << "1" << std::endl;
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
std::cout << "1414" << std::endl;
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
    std::unique_ptr<lib::event::sockitem> si;
    si->sockfd = listenSocket;
	ev.data.ptr = si.get();
    ev.events = EPOLLIN;

    ePoll->epollAdd(listenSocket, ev);

    std::cout << "HttpServer start successful!" << std::endl;
    return true;
}

void HttpServer::stop()
{
    return;
}

int HttpServer::accept_cb(int fd, int events, void *arg)
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

	std::unique_ptr<lib::event::sockitem> si;
	si->sockfd = clientfd;
	ev.data.ptr = si.get();;
	
	ePoll->epollAdd(clientfd, ev);

	return clientfd;
}

void HttpServer::process()
{
    for(;;)
    {
        //int nready = epoll_wait(eventloop->epfd, eventloop->events, 512, -1);
		// if (nready < -1) {
		// 	break;
		// }
        std::cout << "HttpServer wait" << std::endl;
        sleep(1000);
    }
}

HttpServer::~HttpServer()
{
    return;
}

}