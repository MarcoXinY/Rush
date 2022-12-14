#include <sys/epoll.h>
#include <sys/poll.h>
#include <array>
#include <functional>

namespace lib
{
namespace event
{
constexpr int BUFFER_LENGTH = 1024;

struct sockitem {
	int sockfd;
    std::array<char, BUFFER_LENGTH> recvbuffer;
    std::array<char, BUFFER_LENGTH> sendbuffer;

	int rlength;
	int slength;
};

class EPoll
{
public:
    EPoll(){};
    ~EPoll(){};
    bool start(const std::int32_t&);
    void stop();
    void epollAdd(std::int32_t&, epoll_event&);
    void epollDel(std::int32_t&, epoll_event&);
    void epollMod(std::int32_t&, epoll_event&);
    std::int32_t epollWait();
    std::array<epoll_event, 512> getEpollEvents();
private:
    std::int32_t ePollFd{0};
    std::array<epoll_event, 512> events;
};
}//event
}//lib