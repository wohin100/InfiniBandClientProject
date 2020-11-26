#include <iostream>
#include <detector/BuildConfig.h>
#include <detector/IbFabric.h>
#include <detector/IbNode.h>
#include <detector/exception/IbFileException.h>
#include <csignal>
#include <chrono>
#include <thread>
#include <string>

bool isRunning = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main() {
    Detector::BuildConfig::printBanner();
    bool network = false;
    bool compatibility = true;
    Detector::IbFabric fabric(network, compatibility);

    uint32_t nodeNr = fabric.GetNumNodes();

    std::cout << nodeNr << std::endl;
    return 0;
}
