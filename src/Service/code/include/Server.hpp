#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <memory>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "eventPoll.hpp"

namespace Service
{
constexpr std::uint16_t PORT = 992;//监听的端口号
const std::string IPAddress = "172.27.15.149";//监听的服务器本地的IP地址
constexpr std::uint32_t BUFSIZE = 1024;
constexpr std::int32_t INVAILD_SOCKET = -1;
constexpr std::int32_t MAX_EPOLLSIZE = 100000;//socket 数量

class HttpServer
{
public:
    HttpServer(const std::uint16_t port, const std::string address);
    ~HttpServer();
    bool start();
    void stop();
    void process();
private:
    int acceptCallBack(int fd, int events);
    void clientCallBakc(int fd, epoll_event& event);
    int recvCallBack(int fd, epoll_event& event);
    int sendCallBack(int fd, epoll_event& event);

    std::uint16_t listenPort{0};
    std::int32_t listenSocket{INVAILD_SOCKET};
    std::string listenAddress;
    struct sockaddr_in serverAddr{0};
    std::unique_ptr<lib::event::EPoll> ePoll;
};
}//Server

#endif
