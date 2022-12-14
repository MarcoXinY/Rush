#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include "eventPoll.hpp"
#include "threadPool.hpp"
#include "Chat.hpp"

namespace Service
{
constexpr std::uint16_t PORT = 80;//监听的端口号
const std::string IPAddress = "127.0.0.1";//监听的服务器本地的IP地址
constexpr std::uint32_t BUFSIZE = 1024;
constexpr std::int32_t INVAILD_SOCKET = -1;
constexpr std::int32_t MAX_EPOLLSIZE = 100000;//socket 数量
constexpr std::int32_t MAX_THREAD = 10;

class HttpServer
{
public:
    HttpServer(const std::uint16_t port, const std::string address);
    ~HttpServer();
    bool start();
    void stop();
    void process();
private:
    bool listenFdInit();
    int acceptCallBack(int fd, int events);
    void clientCallBack(int fd, epoll_event& event);
    int recvCallBack(int fd, epoll_event& event);
    int sendCallBack(int fd, epoll_event& event);
    //std::string fileGet();
    //void fileSave(std::string);
    //std::string GetpostResponse();

    std::uint16_t listenPort{0};
    std::int32_t listenSocket{INVAILD_SOCKET};
    std::string listenAddress;
    struct sockaddr_in serverAddr{0};
    std::unique_ptr<lib::event::EPoll> ePoll;
    std::unique_ptr<lib::ThreadPool> threadPool;
    std::map<int, lib::event::sockitem> userContext;
    std::map<int, Chat> chatMap;
};
}//Server

