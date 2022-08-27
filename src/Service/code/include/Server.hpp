#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define BUFSIZE 50

const int PORT = 990;//监听的端口号
const char *IPAddress = "172.27.15.149";//监听的服务器本地的IP地址