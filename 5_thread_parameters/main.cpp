#include <iostream>
#include <thread>
#include <string>

void print_sum(int x, int y) {
    std::cout << x << " + " << y << " = " << x + y << std::endl;
}

void print_while(int& x) {
    while (true) {
        std::cout << "The value is " << x << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    int a{ 5 }, b{ 10 };
    std::thread(print_sum, a, b).join();

    int x{ 1 };
    std::thread t(print_while, std::ref(x));
    std::this_thread::sleep_for(std::chrono::seconds(3));
    x++;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    t.join();
}
