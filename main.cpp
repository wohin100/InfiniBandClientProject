#include <iostream>
#include <detector/BuildConfig.h>
#include <detector/IbFabric.h>
#include <detector/exception/IbFileException.h>

int main() {
    Detector::BuildConfig::printBanner();
    bool network = true;
    bool compatibility = true;
    Detector::IbFabric *fabric = nullptr;

    try {
        fabric = new Detector::IbFabric(network, compatibility);
    } catch(const Detector::IbPerfException &exception) {
        printf("An exception occurred: %s", exception.what());
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
