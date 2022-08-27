/*
 * config.h 包含该tcp/ip套接字编程所需要的基本头文件
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

const int MAX_LINE = 2048;
const int PORT = 990;
const int BACKLOG = 10;
const int LISTENQ = 6666;
const int MAX_CONNECT = 20;