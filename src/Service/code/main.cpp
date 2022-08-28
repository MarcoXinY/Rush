#include "Server.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>


int main(){

    // Parse config file

    std::unique_ptr<Service::HttpServer> httpServer;
    if(not httpServer->start())
    {
        return -1;
    }

    httpServer->process();
    return 0;
}
