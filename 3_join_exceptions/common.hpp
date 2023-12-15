#pragma once

#include <vector>
#include <thread>

class ThreadGuard
{

public:

    explicit ThreadGuard(std::thread& t) : m_t(t) {}

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard operator=(const ThreadGuard&) = delete;

    ~ThreadGuard() {
        if (m_t.joinable()) m_t.join();
    }

private:

    std::thread& m_t;
};
