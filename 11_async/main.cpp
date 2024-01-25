#include <iostream>
#include <future>

int test_func(int param, bool b)
{
    return b ? param : 1;
}

// Not asynchronous
void packaged_task_test()
{
    std::packaged_task<int(int, bool)> task(test_func);
    // auto taskFuture{ task.get_future() };
    task(50, false);
    std::cout << "Testing packaged task: " << task.get_future().get() << "\n";

    std::packaged_task<int()> boundTask(std::bind(test_func, 60, true));
    boundTask();
    std::cout << "Testing bound packaged task: " << boundTask.get_future().get() << std::endl;
}

void packaged_task_test_thread()
{
    std::packaged_task<int(int, bool)> task(test_func);
    auto future{ task.get_future() };                   // Need to do this before std::move(task)
    auto t{ std::thread(std::move(task), 140, true) };  // packaged_task is not copyable
    t.detach();                                         // To make it asynchronous
    std::cout << "Testing thread packaged task: " << future.get() << std::endl;
}

int main()
{
    // Can also use std::launch::deferred (or both) to wait for the .get() call
    std::future<int> testFuncRes{ std::async(std::launch::async, test_func, 100, true) };

    // Lambdas...
    // std::future<int> testFuncRes{ std::async(std::launch::async, [] { return 1000; }) };

    packaged_task_test();
    packaged_task_test_thread();

    std::cout << "Test function result is: " << testFuncRes.get() << std::endl;
}
