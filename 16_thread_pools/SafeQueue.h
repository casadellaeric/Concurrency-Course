#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <iostream>

template<typename T>
class SafeQueue
{
public:

    SafeQueue(){};
    SafeQueue(const SafeQueue& other) = default;

    void push(T val)
    {
        {
            std::lock_guard<std::mutex> lg(m);
            m_q.push(std::make_shared<T>(val));
        }
        // Probably many redundant calls, but might be more efficient than an if block
        cv.notify_one();
    };

    bool pop(T& ref)
    {
        std::lock_guard<std::mutex> lg(m);
        if (m_q.empty()) {
            return false;
        }
        ref = *m_q.front();
        m_q.pop();
        return true;
    };

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lg(m);
        if (m_q.empty()) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> ret(m_q.front());
        m_q.pop();
        return ret;
    };

    // If the queue is empty, waits for an element to be pushed
    void wait_pop(T& ref)
    {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [] { return !m_q.empty(); });
        ref = *m_q.front();
        m_q.pop();
    };

    std::shared_ptr<T> wait_pop()
    {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [] { return !m_q.empty(); });
        std::shared_ptr<T> ret(m_q.front());
        m_q.pop();
        return ret;
    };

    std::shared_ptr<T> front()
    {
        std::lock_guard<std::mutex> lg(m);
        return m_q.front();
    };

    bool empty()
    {
        std::lock_guard<std::mutex> lg(m);
        return m_q.empty();
    };

    size_t size()
    {
        std::lock_guard<std::mutex> lg(m);
        return m_q.size();
    };

private:

    std::queue<std::shared_ptr<T>> m_q{};
    std::condition_variable cv;
    std::mutex m;
};