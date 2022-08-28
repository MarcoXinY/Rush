#include "eventPoll.hpp"

namespace lib
{
namespace event
{

bool EPoll::start(const std::int32_t& maxEpollSize)
{
    std::cout << "14143111" << std::endl;
    ePollFd = epoll_create(maxEpollSize);
    std::cout << "141431" << std::endl;
    return true;
}

void EPoll::epollAdd(std::int32_t& listenSocket, epoll_event& ev)
{
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, listenSocket, &ev);
    return;
}

}
}