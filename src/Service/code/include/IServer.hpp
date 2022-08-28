#program once

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

class IServer
{
public:
    IServer(std::uint16_t port, std::string address);
    virtual ~IServer();
    virtual bool start() = 0;
    virtual void stop() = 0;
private:
    int listenPort;
    int listenSocket;
    struct sockaddr_in serverAddr;
}