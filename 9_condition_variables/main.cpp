#include <iostream>
#include <thread>
#include <condition_variable>

std::condition_variable cv;
std::mutex m;
bool destinationReached = false;

void driver_thread()
{
    int currentDistance = 0;
    int targetDistance  = 5;
    while (currentDistance < targetDistance) {
        currentDistance += 1;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    {
        std::lock_guard<std::mutex> lg(m);
        destinationReached = true;
    }

    cv.notify_one();
}

void passenger_thread()
{
    std::unique_lock<std::mutex> ul(m);
    cv.wait(ul, [] { return destinationReached; });
    std::cout << "We have arrived!\n";
}

int main()
{
    std::thread driver(driver_thread);
    std::thread passenger(passenger_thread);

    driver.join();
    passenger.join();
}
