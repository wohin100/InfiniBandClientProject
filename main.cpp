#include <iostream>
#include <detector/BuildConfig.h>
#include <detector/IbFabric.h>
#include <detector/IbNode.h>
#include <detector/exception/IbFileException.h>
#include <csignal>
#include <chrono>
#include <thread>

bool isRunning = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main() {
    Detector::BuildConfig::printBanner();
    bool network = true;
    bool compatibility = true;
    Detector::IbFabric fabric(network, compatibility);

    while(isRunning) {
        try {
            fabric.RefreshCounters();
            std::cout << fabric << std::endl << std::endl;
        } catch(const Detector::IbPerfException &exception) {
            printf("An exception occurred: %s", exception.what());
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
