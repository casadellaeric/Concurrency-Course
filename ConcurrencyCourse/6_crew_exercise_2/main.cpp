#include <iostream>
#include <thread>
#include <string>
#include <queue>

void clean_ship() {
    std::cout << "The cleaning crew has started cleaning..." << std::endl;
}

void engine_full_speed()
{
    std::cout << "The engine crew is accelerating the engine..." << std::endl;
}

void engine_stop()
{
    std::cout << "The engine crew is stopping the engine..." << std::endl;
}

void cleaning_crew_work(std::queue<int>& workQueue, const bool& done) {
    while (!done) {
        if (!workQueue.empty()) {
            int job{ workQueue.front() };
            workQueue.pop();
            switch (job) {
            case 1:
                clean_ship();
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::cout << "The cleaning crew has no work to do." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

void engine_crew_work(std::queue<int>& workQueue, const bool& done) {
    while (!done) {
        if (!workQueue.empty()) {
            int job{ workQueue.front() };
            workQueue.pop();
            switch (job) {
            case 2:
                engine_full_speed();
                break;
            case 3:
                engine_stop();
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::cout << "The engine crew has no work to do." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
}

int main()
{
    std::cout << "Please enter a command for the captain:\n"
        << "\t1: Order the cleaning crew to clean.\n"
        << "\t2: Order the engine crew to go full speed.\n"
        << "\t3: Order the engine crew to stop speed.\n"
        << "\t100: Exit the program.\n";
    std::cout.flush();

    // Commands
    std::queue<int> cleaningQueue{};
    std::queue<int> engineQueue{};
    bool done{};

    std::thread cleaningCrew(cleaning_crew_work, std::ref(cleaningQueue), std::ref(done));
    std::thread engineCrew(engine_crew_work, std::ref(engineQueue), std::ref(done));

    int input;
    while (!done && std::cin >> input) {
        switch (input) {
        case 1: // cleaning
            cleaningQueue.push(1);
            break;
        case 2: // full speed
            engineQueue.push(2);
            break;
        case 3: // stop
            engineQueue.push(3);
            break;
        case 100: // exit
            done = true;
            break;
        default:
            std::cout << "Invalid command!\n";
        }
        std::cout << "Please send another command:" << std::endl;
    }

    cleaningCrew.join();
    engineCrew.join();
}
