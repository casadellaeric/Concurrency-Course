#include <iostream>
#include <future>

void consume_and_receive_exception(std::future<bool>& future)
{
    try {
        std::cout << "Trying to get: " << future.get() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Exception thrown from future: " << e.what() << std::endl;
    }
}

void consume_promise(std::future<bool>& future)
{
    std::cout << "The value that has been set to the promise is: " << future.get() << std::endl;
}

int main()
{
    std::promise<bool> promise;
    auto future{ promise.get_future() };

    auto t{ std::thread(consume_promise, std::ref(future)) };
    // t.detach(); // Could detach here...

    std::cout << "Waiting to set the promise..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    promise.set_value(true);
    t.join();

    std::cout << "Testing promised exception..." << std::endl;
    promise = std::promise<bool>{};
    future  = promise.get_future();
    t       = std::thread(consume_and_receive_exception, std::ref(future));
    try {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        throw std::runtime_error("RUNTIME ERROR!");
    }
    catch (std::exception&) {
        promise.set_exception(std::current_exception());
    }
    t.join();
    std::cout << "Waiting before exiting..." << std::endl;
}
