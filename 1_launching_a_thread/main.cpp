#include <iostream>
#include <thread>
#include <string>

#include "exercise.hpp"

class CallableClass
{

public:
    void operator()()
    {
        std::cout << "Hello from CallableClass " << std::this_thread::get_id() << "\n";
    }
};

void foo()
{
    std::cout << "Hello from foo " << std::this_thread::get_id() << "\n";
}

int main()
{
    // From a function
    std::thread t1(foo);

    // From a class with an overloaded operator()
    CallableClass callable{};
    std::thread t2(callable);

    // From a lambda
    std::thread t3([] { std::cout << "Hello from lambda " << std::this_thread::get_id() << "\n"; });

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Hello from main " << std::this_thread::get_id() << "\n";

    run();
}
