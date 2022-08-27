#include "Server.hpp"

int main(){
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IPAddress);
    serv_addr.sin_port = htons(990);
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
 
    listen(serv_sock, 20);
 
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = -1;
    do {
         clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
         if (clnt_sock < 0) {
             printf("Connected Error, re-try to get connected with client ... \n");
         }
    } while(clnt_sock < 0);

    std::cout << "here get connected client" << std::endl;

    while(1) {
        char strBuffer[BUFSIZE];
        int dataLength = read(clnt_sock, strBuffer, sizeof(strBuffer)-1);
        if (dataLength < 0) {
            printf("Read Error ... \n");
            continue;
        }
        printf("The server has already got the data: %s\n", strBuffer);
 
 
        int writeFlag = write(clnt_sock, strBuffer, dataLength);
        if (writeFlag < 0) {
            printf("Write error ... \n");
        }
        printf("Has already sent the data to the client");
 
    }
    close(clnt_sock);
    close(serv_sock);
 
    return 0;
}
