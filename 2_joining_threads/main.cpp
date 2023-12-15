#include <iostream>
#include <thread>
#include <string>

void test()
{
    std::cout << "Hello from test\n";
}

int main()
{
    std::thread t1(test);

    std::cout << t1.joinable() << "\n";

    t1.join();

    std::cout << t1.joinable() << "\n";

    std::thread t2;

    std::cout << t2.joinable() << "\n";
}
