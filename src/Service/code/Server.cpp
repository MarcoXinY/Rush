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
#include "json11.hpp"

using namespace json11;

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
    threadPool->start(MAX_THREAD);
    if(listenFdInit())
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
    //lib::event::sockitem* si = static_cast<lib::event::sockitem*>(event.data.ptr);
    // http::Response response{"are you happy now"};

	// send(fd, response.ToString().c_str(), response.ToString().size()+1, 0);

    if(httpRequestType == 0) //GET
    {
        std::string hsitory = fileGet();
        http::Response response{hsitory};
        send(fd, response.ToString().c_str(), response.ToString().size()+1, 0);
    }

    if(httpRequestType == 1) //POST
    {
        std::string postEmptyResponse = GetpostResponse();
        http::Response response{postEmptyResponse};
	    send(fd, response.ToString().c_str(), response.ToString().size()+1, 0);
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

		close(fd);
	}
    else if (ret == 0)
    {
        std::cout << "disconnect: " << fd << std::endl; 

		ev.events = EPOLLIN;
		ePoll->epollDel(fd, ev);

		close(fd);
    }
    else
    {
        printf("Recv: %s, %d Bytes\n", buffer, ret);
        http::HttpParser http_package(buffer);
        http_package.show();

        if(http_package["method"] == "GET")
        {
            httpRequestType = 0;//0 表示get  1 表示 post
        }

        if(http_package["method"] == "POST")
        {
            httpRequestType = 1;//0 表示get  1 表示 post
            fileSave(http_package["body"]);
        }

		//si->rlength = ret;
		//memcpy(si->sendbuffer.data(), si->recvbuffer.data(), si->rlength);
        //si->sendbuffer.swap(si->recvbuffer);
		//si->slength = si->rlength;

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


std::string HttpServer::fileGet()
{
    std::ifstream fin("/home/wujinchao/Rush/history.json", std::ios::in);
    std::stringstream in;
    in << fin.rdbuf();
    std::cout << "sent content :" << in.str() << std::endl;
    return in.str();
}

class MessageJsonFormat {
public:
    int code=0;
    std::string message="";
    std::vector<std::string> data;

    MessageJsonFormat (){}
    void addData(std::string content){data.push_back(content);}
    Json to_json() const { return Json::object ({
        {"code", 0},
        {"message", ""},
        {"data", Json(data)}
    });}
};

std::string HttpServer::GetpostResponse()
{
    MessageJsonFormat postResponse;
    std::string points_json = Json(postResponse).dump();
    std::cout << "points_json: " << points_json << "\n";
    return points_json;
}

void HttpServer::fileSave(std::string data)
{
    //读取文本信息
    std::cout << "save content :" << data << std::endl;

    std::ifstream fin("/home/wujinchao/Rush/history.json", std::ios::in);
    std::stringstream in;
    in << fin.rdbuf();

    std::cout << "json content before:" << in.str() << std::endl;
    std::string err;
    const auto json = Json::parse(in.str(), err);
    MessageJsonFormat messageTobeSave;

    fin.close();

    for (auto &k : json["data"].array_items()) {
        std::cout << "    - " << k.string_value() << "\n";
        messageTobeSave.data.push_back(k.string_value());//读取文件里的数据
    }
    //添加新的数据
    // 先解析POST的格式
    const auto postJson = Json::parse(data, err);
    std::cout <<"****1: " <<postJson["data"][0].string_value() << "\n";

    messageTobeSave.data.push_back(postJson["data"][0].string_value());
    //存到本地
    std::ofstream fsave("/home/wujinchao/Rush/history.json", std::ios::out);// 清空写入
    std::string messageTobeSave_json = Json(messageTobeSave).dump();
    fsave << messageTobeSave_json;
    fsave.close();
}

}