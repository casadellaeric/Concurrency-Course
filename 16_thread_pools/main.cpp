#include "SafeQueue.h"
#include <functional>
#include <iostream>
#include <future>

class ThreadPool
{
public:

    ThreadPool() :
      done(false)
    {
        try {
            const unsigned int threadCount{ std::thread::hardware_concurrency() };
            for (int i = 0; i < threadCount; ++i) {
                threads.push_back(std::jthread(&ThreadPool::worker_thread, this));
            }
        }
        catch (...) {
            done = true;
            throw;
        }
    }

    ~ThreadPool()
    {
        done = true;
        join_threads();
    }

    template<typename functionType>
    void submit(functionType function)
    {
        workQueue.push(std::function<void()>(function));
    }

    void join_threads()
    {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:

    std::atomic<bool> done;
    SafeQueue<std::function<void()>> workQueue;
    std::vector<std::jthread> threads;

    void worker_thread()
    {
        while (!done) {
            std::function<void()> task;
            if (workQueue.pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
};

int main()
{
    ThreadPool pool{};

    for (int i = 0; i < 100; ++i) {
        pool.submit(
            [=]()
            {
                std::cout << "Printing it " << i << " from thread " << std::this_thread::get_id()
                          << std::endl;
            });
    }

    system("pause");
}