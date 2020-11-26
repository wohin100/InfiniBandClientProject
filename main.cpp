#include <iostream>
#include <detector/BuildConfig.h>
#include <detector/IbFabric.h>
#include <detector/IbNode.h>
#include <detector/exception/IbFileException.h>
#include <csignal>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

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
    std::vector<Detector::IbNode *> nodes = fabric.GetNodes();
    uint8_t ports = nodes.front()->GetNumPorts();

    std::cout << ports << std::endl;
    return 0;
}
