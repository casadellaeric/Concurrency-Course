#pragma once
#include <iostream>
#include <thread>

namespace
{
    void test()
    {
        std::cout << "Hello from test\n";
    }

    void functionA()
    {
        std::cout << "Hello from functionA\n";

        std::thread threadC(test);
        threadC.join();
    }

    void functionB()
    {
        std::cout << "Hello from functionB\n";
    }
}

void run()
{
    std::thread threadA(functionA);
    std::thread threadB(functionB);

    threadA.join();
    threadB.join();
}
