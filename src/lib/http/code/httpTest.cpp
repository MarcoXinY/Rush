#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
//  #include "../include/httpParser.hpp"

#define port 80 //监听端口，可以在范围内自由设定
//#define message "HTTP server is ready!"//返回给客户机的信息
const std::string IPAddress = "120.27.251.123";//监听的服务器本地的IP地址
using namespace std;
//using namespace http;

int main()
{
//1.创建一个socket套接字
int local_fd = socket(AF_INET, SOCK_STREAM, 0);
if (local_fd == -1)
{
cout << "socket error!" << endl;
exit(-1);
}
cout << "socket ready!" << endl;
//2.sockaddr_in结构体：可以存储一套网络地址（包括IP与端口）,此处存储本机IP地址与本地的一个端口
struct sockaddr_in local_addr;
local_addr.sin_family = AF_INET;
local_addr.sin_port = htons(port);  //绑定端口
local_addr.sin_addr.s_addr = INADDR_ANY;//绑定本机IP地址

//3.bind()： 将一个网络地址与一个套接字绑定，此处将本地地址绑定到一个套接字上
int res = bind(local_fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
if (res == -1)
{
cout << "bind error!" << endl;
exit(-1);
}
cout << "bind ready!" << endl;
//4.listen()函数：监听试图连接本机的客户端
//参数二：监听的进程数
listen(local_fd, 10);
cout << "等待来自客户端的连接...." << endl;
while (true)//循环接收客户端的请求
{
//5.创建一个sockaddr_in结构体，用来存储客户机的地址
struct sockaddr_in client_addr;
socklen_t len = sizeof(client_addr);
//6.accept()函数：阻塞运行，直到收到某一客户机的连接请求，并返回客户机的描述符
int client_fd = accept(local_fd, (struct sockaddr *)&client_addr, &len);
if (client_fd == -1)
{
cout << "accept错误\n"
<< endl;
exit(-1);
}
//7.输出客户机的信息
char *ip = inet_ntoa(client_addr.sin_addr);
cout << "客户机： " << ip << " 连接到本服务器成功!" << endl;

//8.输出客户机请求的信息
char buff[1024] = {0};
int size = read(client_fd, buff, sizeof(buff));

// HttpParser http_package(buff);    //将buf构造成一个HTTP_Parser类
// cout << "-----" << endl;
// http_package.show();
// cout << "-----" << endl;
// cout << "Request information:\n"
// << buff << endl;
// cout << size << " bytes" << endl;

//直接发送数据
// string first_line = "HTTP/1.0 200 OK\r\n";
// string body = "<html><div><h1>hello world</h1></div></html>\r\n";
// string header = "Content-Type: text/html\ncharset: gbk\nContent-Length:"+to_string(body.size())+"\r\n\r\n";
// std::string message = first_line+header+body;
//使用类
// Response response{"Hello !!!"};
// write(client_fd, response.c_str(), response.size());//返回message

//9.使用第6步accept()返回socket描述符，即客户机的描述符，进行通信。
// write(client_fd, message.c_str(), message.size()+1);//返回message
sleep(3);
//10.关闭sockfd
close(client_fd);
}
close(local_fd);
return 0;
}