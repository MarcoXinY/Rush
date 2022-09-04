#include <iostream>
#include "Server.hpp"
#include <memory>
#include <stdexcept>


int main(){

    // Parse config file
    std::cout << "Main" << std::endl;
    std::unique_ptr<Service::HttpServer> httpServer(new Service::HttpServer(Service::PORT, Service::IPAddress));
    //std::unique_ptr<Service::HttpServer> httpServer = std::make_unique<Service::HttpServer>(Service::PORT, Service::IPAddress);
    if(not httpServer->start())
    {
        return -1;
    }

    httpServer->process();
    return 0;
}
