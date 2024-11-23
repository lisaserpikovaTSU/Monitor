#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::condition_variable cond1;
std::mutex mtx;
int ready = 0;

void producer() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(mtx);
        if (ready == 1) {
            continue;
        }
        ready = 1;
        std::cout << "provided" << std::endl;
        cond1.notify_one();
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        while (ready == 0) {
            cond1.wait(lock);
            std::cout << "awoke" << std::endl;
        }
        ready = 0;
        std::cout << "consumed" << std::endl;
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
