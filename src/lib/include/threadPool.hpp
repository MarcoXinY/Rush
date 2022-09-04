#pragma once
#include <vector>
#include <thread>
#include <memory>

namespace lib
{
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    bool start(const std::int32_t&);
    void stop();
private:
    std::unique_ptr<std::vector<std::thread>> pool;
};
}//lib