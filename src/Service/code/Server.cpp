#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include "Server.hpp"
#include "Response.hpp"
#include "httpParser.hpp"


namespace Service
{
HttpServer::HttpServer(const std::uint16_t port,const std::string address):listenPort(port), listenAddress(address)
{
    ePoll.reset(new lib::event::EPoll());
    threadPool.reset(new lib::ThreadPool());
    return;
}

bool HttpServer::listenFdInit()
{
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket == INVAILD_SOCKET)
    {
        std::cout << "Create socket failure!" << std::endl;
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
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

    struct epoll_event ev = {};
    std::unique_ptr<lib::event::sockitem> si(new lib::event::sockitem());
    ev.events = EPOLLIN;
    ev.data.fd = listenSocket;

    ePoll->epollAdd(listenSocket, ev);
    return true;
}

bool HttpServer::start()
{
    ePoll->start(MAX_EPOLLSIZE);
    //threadPool->start(MAX_THREAD);
    if(not listenFdInit())
    {
        return false;
    }

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

	ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = clientfd;

	ePoll->epollAdd(clientfd, ev);

	return clientfd;
}

void HttpServer::clientCallBack(int fd, epoll_event& event)
{
    if(event.events & EPOLLIN )
    {
        recvCallBack(fd, event);
    }
    else if(event.events & EPOLLOUT)
    {
        std::cout << "sendCallBack" << std::endl;
        sendCallBack(fd, event);
    }
    return;
}

int HttpServer::sendCallBack(int fd, epoll_event& event)
{

    //根据要转发的对象(client 2)找到对应的FD
    std::string peerName = chatMap[fd].receiveMessage.messageDataItem.to;

    int peerFd = 0;
    for(auto &it : chatMap)
    {
        if(it.second.localName == peerName)
        {
            peerFd = it.first;
            break;
        }
    }
    //send text
    if(peerFd != 0)
    {
        //转发
        std::string responseToOtherClient = chatMap[fd].MessageForForwardingOtherClient();
        send(peerFd, responseToOtherClient.c_str(), responseToOtherClient.size(), 0);
        //返回处理结果 成功
        std::string responseSuccess = chatMap[fd].responseWithProcessResult(0,"success");
        send(fd, responseSuccess.c_str(), responseSuccess.size(), 0);
    }
    else
    {
        //返回处理结果 失败
        std::string responsefail = chatMap[fd].responseWithProcessResult(1,"fail");
        send(fd, responsefail.c_str(), responsefail.size(), 0);
    }

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = fd;

	ePoll->epollMod(fd, ev);

    return 0;
}

int HttpServer::recvCallBack(int fd, epoll_event& event)
{
    struct epoll_event ev;

    char buffer[1024] = {0};
    int ret = recv(fd, buffer, 1024, 0);
    if (ret < 0)
    {
		if (errno == EAGAIN || errno == EWOULDBLOCK) { 
			return -1;
		}

		ev.events = EPOLLIN;
        ePoll->epollDel(fd, ev);

        auto chat = chatMap.find(fd);
        if(chat != chatMap.end())
        {
            chatMap.erase(chat);
            std::cout << chatMap.size() << std::endl;
        }
		close(fd);
	}
    else if (ret == 0)
    {
        std::cout << "disconnect: " << fd << std::endl; 

		ev.events = EPOLLIN;
		ePoll->epollDel(fd, ev);

        auto chat = chatMap.find(fd);
        if(chat != chatMap.end())
        {
            chatMap.erase(chat);
            std::cout << chatMap.size() << std::endl;
        }
		close(fd);
    }
    else
    {
        // printf("Recv: %s, %d Bytes\n", buffer, ret);
        // http::HttpParser http_package(buffer);
        // http_package.show();

        auto chatClient = chatMap.find(fd);
        if(chatMap.find(fd) == chatMap.end())
        {
            chatMap.insert(pair<int, Chat>(fd, Chat()));
            chatMap[fd].processRequest((unsigned char*)buffer, ret);
        }
        else
        {
            chatClient->second.processRequest((unsigned char*)buffer, ret);
        }

		struct epoll_event ev;
		ev.events = EPOLLOUT | EPOLLET;
        ev.data.fd = fd;

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
            auto triggerFd = events.at(i).data.fd;
            if(triggerFd == listenSocket)
            {
                std::cout << "listenSocket: " << listenSocket << std::endl;
                acceptCallBack(listenSocket, events.at(i).events);
            }
            else
            {
                std::cout << "triggerFd: " << triggerFd << std::endl;
                clientCallBack(triggerFd, events.at(i));
            }
        }
    }
}

HttpServer::~HttpServer()
{
    return;
}

}
