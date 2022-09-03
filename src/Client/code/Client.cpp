//client 是在windows 上运行的 所以client code
//在windows 上开发(运行在windows上)
//所以目前linux 上没有 client 的代码
//To do linux client code here ...

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("172.27.15.149");  //具体的IP地址
    serv_addr.sin_port = htons(992);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   
    //读取服务器传回的数据
    for(;;)
    {
        //回声客户端
        char bufSend[BUF_SIZE];
        printf("输入字符串: ");
        scanf("%s",bufSend);
        write(sock, bufSend,sizeof(bufSend));

        //读取服务器传回的数据
        char buffer[40];
        read(sock, buffer, sizeof(buffer)-1);

        printf("服务器返回: %s\n", buffer);
    }

    
   
    //关闭套接字
    close(sock);
    return 0;
}