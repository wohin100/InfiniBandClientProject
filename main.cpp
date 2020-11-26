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
    Detector::IbFabric fabric(local, compatibility);

    for(Detector::IbNode *node : fabric.GetNodes()) {
        for(Detector::IbPort *port : node->GetPorts()) {
            printf("%s (port %u): %lu", node->GetDescription().c_str(), port->GetNum(), port->GetXmitDataBytes());
        }
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
