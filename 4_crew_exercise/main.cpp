#include <iostream>
#include <thread>
#include <string>

void clean_ship() {
    std::cout << "The cleaning crew has started cleaning..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    std::cout << "The cleaning crew has finished cleaning." << std::endl;
}

void engine_full_speed()
{
    std::cout << "The engine crew is accelerating the engine..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "The engine crew has accelerated the engine." << std::endl;
}

void engine_stop()
{
    std::cout << "The engine crew is stopping the engine..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "The engine crew has stopped the engine." << std::endl;
}

int main()
{
    std::cout << "Please enter a command for the captain:\n"
        << "\t1: Order the cleaning crew to clean.\n"
        << "\t2: Order the engine crew to go full speed.\n"
        << "\t3: Order the engine crew to stop speed.\n"
        << "\t100: Exit the program.\n";
    std::cout.flush();

    int input;
    while (std::cin >> input) {
        switch (input) {
        case 1: // cleaning
            std::thread(clean_ship).detach();
            break;
        case 2: // full speed
            std::thread(engine_full_speed).join();
            break;
        case 3: // stop
            std::thread(engine_stop).join();
            break;
        case 100: // exit
            return 0;
        default:
            std::cout << "Invalid command!\n";
        }
        std::cout << "Please send another command:" << std::endl;
    }

}
