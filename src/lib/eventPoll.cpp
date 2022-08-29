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

void EPoll::epollDel(std::int32_t& socket, epoll_event& ev)
{
    epoll_ctl(ePollFd, EPOLL_CTL_DEL, socket, &ev);
    return;
}

void EPoll::epollMod(std::int32_t& socket, epoll_event& ev)
{
    epoll_ctl(ePollFd, EPOLL_CTL_MOD, socket, &ev);
    return;
}

std::int32_t EPoll::epollWait()
{
    return epoll_wait(ePollFd, events.data(), 1024, -1);
}

std::array<epoll_event, 512> EPoll::getEpollEvents()
{
    return events;
}

}
}