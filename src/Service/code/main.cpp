#include <iostream>
#include "Server.hpp"
#include <memory>
#include <stdexcept>


int main(){

    // Parse config file

    std::unique_ptr<Service::HttpServer> httpServer(new Service::HttpServer(Service::PORT, Service::IPAddress));
    if(not httpServer->start())
    {
        return -1;
    }

    httpServer->process();
    return 0;
}
