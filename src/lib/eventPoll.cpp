#include "eventPoll.hpp"

namespace lib
{
namespace event
{

bool EPoll::start(const std::int32_t& maxEpollSize)
{
    ePollFd = epoll_create(maxEpollSize);
    return true;
}

void EPoll::epollAdd(std::int32_t& listenSocket, epoll_event& ev)
{
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, listenSocket, &ev);
    return;
}

}
}