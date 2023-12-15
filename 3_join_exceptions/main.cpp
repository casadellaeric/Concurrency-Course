#include <iostream>
#include <thread>
#include <string>

#include "common.hpp"

void test()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Hello from test\n";
}

void throwing_function()
{
    std::cout << "...\n";
    throw std::runtime_error("Test runtime error.");
}

int main()
{
    std::thread t1(test);

    // Destructor will be called before t1 is destroyed, so it will join even in the case of exceptions
    ThreadGuard tg_t1(t1);

    try {
        throwing_function();
    }
    catch (...) {
    }

}
