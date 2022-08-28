#include "eventPoll.hpp"
#include <iostream>
namespace lib
{
namespace event
{

bool EPoll::start(const std::int32_t& maxEpollSize)
{
    ePollFd = epoll_create(maxEpollSize);
    return true;
}

void EPoll::epollAdd(std::int32_t& socket, epoll_event& ev)
{
    epoll_ctl(ePollFd, EPOLL_CTL_ADD, socket, &ev);
    return;
}

std::int32_t EPoll::epollWait()
{
    return epoll_wait(ePollFd, events.data(), 512, -1);
}

std::array<epoll_event, 512> EPoll::getEpollEvents()
{
    return events;
}

}
}