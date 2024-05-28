#include <thread>
#include <iostream>

void test_print_loop(std::stop_token token)
{
    size_t i{ 0 };
    while (!token.stop_requested()) {
        std::cout << "Test print\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::jthread t(test_print_loop);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    t.request_stop();
}