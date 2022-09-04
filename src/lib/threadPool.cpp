#include "threadPool.hpp"
#include <iostream>

namespace lib
{

ThreadPool::ThreadPool()
{
    pool.reset(new std::vector<std::thread>());
    return;
}

ThreadPool::~ThreadPool()
{
    return;
}

void thread_function()
{
    std::cout << "hello world" << std::endl;
}

bool ThreadPool::start(const std::int32_t& threadNum)
{
    for(std::int32_t i = 0; i < threadNum; i++)
    {
        (*pool).emplace_back(std::thread(&thread_function));
    }
    return true;
}

void ThreadPool::stop()
{
    return;
}

}