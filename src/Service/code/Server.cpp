#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <cstring>
#include "Server.hpp"
#include "Response.hpp"

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
    //ev.data.fd = listenSocket;

    ePoll->epollAdd(listenSocket, ev);

    std::cout << "HttpServer start successful!" << std::endl;
    return true;
}

void HttpServer::stop()
{
    return;
}

int HttpServer::acceptCallBack(int fd, int events)
{
    struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	
	int clientfd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
	if (clientfd <= 0) return -1;

	char str[INET_ADDRSTRLEN] = {0};
	printf("recv from %s at port %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
		ntohs(client_addr.sin_port));

	struct epoll_event ev;
    lib::event::sockitem* si = new lib::event::sockitem();
	si->sockfd = clientfd;
	ev.events = EPOLLIN | EPOLLET;

	ev.data.ptr = si;

	ePoll->epollAdd(clientfd, ev);

	return clientfd;
}

void HttpServer::clientCallBakc(int fd, epoll_event& event)
{
    if(event.events & EPOLLIN | EPOLLET)
    {
        recvCallBack(fd, event);
    }
    else if(event.events & EPOLLOUT | EPOLLET)
    {
        std::cout << "sendCallBack" << std::endl;
        sendCallBack(fd, event);
    }
    return;
}

int HttpServer::sendCallBack(int fd, epoll_event& event)
{
    lib::event::sockitem* si = static_cast<lib::event::sockitem*>(event.data.ptr);

	send(fd, si->sendbuffer.data(), si->slength, 0);

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = fd;
	si->sockfd = fd;
	//si->callback = recv_cb;
	ev.data.ptr = si;

	ePoll->epollMod(fd, ev);
}

int HttpServer::recvCallBack(int fd, epoll_event& event)
{
    lib::event::sockitem* si = static_cast<lib::event::sockitem*>(event.data.ptr);
    struct epoll_event ev;

    int ret = recv(fd, si->recvbuffer.data(), lib::event::BUFFER_LENGTH, 0);
    std::cout << "yhx: " << ret << std::endl; 
    if (ret < 0)
    {
		if (errno == EAGAIN || errno == EWOULDBLOCK) { 
			return -1;
		}

		ev.events = EPOLLIN;
        ePoll->epollDel(fd, ev);

		close(fd);
        if(si != nullptr) delete si;
	}
    else if (ret == 0)
    {
        std::cout << "disconnect: " << fd << std::endl; 

		ev.events = EPOLLIN;
		ePoll->epollDel(fd, ev);

		close(fd);
        if(si != nullptr) delete si;
    }
    else
    {
        printf("Recv: %s, %d Bytes\n", si->recvbuffer.data(), ret);

		si->rlength = ret;
		memcpy(si->sendbuffer.data(), si->recvbuffer.data(), si->rlength);
        //si->sendbuffer.swap(si->recvbuffer);
		si->slength = si->rlength;

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
		si->sockfd = fd;
		//si->callback = send_cb;
		ev.data.ptr = si;

        ePoll->epollMod(fd, ev);
    }

    return 0;
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
            lib::event::sockitem si = *static_cast<lib::event::sockitem*>(events.at(i).data.ptr);
            if(si.sockfd == listenSocket)
            {
                std::cout << "listenSocket: " << listenSocket << std::endl;
                acceptCallBack(listenSocket, events.at(i).events);
            }
            else
            {
                std::cout << "si->sockfd: " << si.sockfd << std::endl;
                clientCallBakc(si.sockfd, events.at(i));
            }
        }
    }
}

HttpServer::~HttpServer()
{
    return;
}

}