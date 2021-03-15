#include <csignal>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>
#include <unistd.h>
#include <thread>
#include "Client.h"
#include "InfinibandReader.h"
#include "ConfigFileReader.h"
#include "FakeInfinibandReader.h"
#include "InfinibandTrafficFaker.h"

using nlohmann::json;

using namespace std;
ConfigFileReader *configFileReader;
FakeInfinibandReader *fakeInfinibandReader;

bool isRunning = true;

//#define DEBUG "dummy"

void singeTestModeOperation();

void testModeOperation();

static void SignalHandler(int signal) {
    if (signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[]) {
    // check params
    if (argc < 3) {
        printf("Usage: ./InfiniBandClient <configFilePath> <Node Number>\n");
        exit(EXIT_FAILURE);
    } else {
        configFileReader = new ConfigFileReader();
        configFileReader->read(argv[1]);
        configFileReader->setClientNr(stoi(argv[2]));
    }

    if (configFileReader->isTestMode()) {
        if (configFileReader->isSingleTestMode()) {
            while (isRunning) {
                singeTestModeOperation();
                // wait some time till next data collection
                usleep(configFileReader->getInterval());
            }
        }
        else {
            this_thread::sleep_for(chrono::milliseconds(configFileReader->getClientNr() * 1000));
            fakeInfinibandReader = new FakeInfinibandReader(configFileReader->getClientNr(),
                                                            configFileReader->getInterval());
            while (isRunning) {
                testModeOperation();

                // wait some time till next data collection
                usleep(configFileReader->getInterval());
            }
        }
    } else {
        string dataToSend = "";
        while (isRunning) {
            json infos = InfinibandReader::collectNodeInfos(configFileReader->getClientNr());
            dataToSend = infos.dump();

            // send it to server
            auto *client = new Client(configFileReader->getServerAddress(), configFileReader->getServerPort());

            client->sendDataToServer(dataToSend);

            // wait some time till next data collection
            usleep(configFileReader->getInterval());
        }
    }
    return 0;
}

void testModeOperation() {
    json infos = fakeInfinibandReader->collectNodeInfos();
    string dataToSend = infos.dump();

    // send it to server
    auto *client = new Client(configFileReader->getServerAddress(), configFileReader->getServerPort());

    client->sendDataToServer(dataToSend);
}

void singeTestModeOperation() {
    InfinibandTrafficFaker *infinibandTrafficFaker = new InfinibandTrafficFaker();
    nlohmann::basic_json<> *datasToSend = infinibandTrafficFaker->getTraffic(
            configFileReader->getConfiguration(), configFileReader->getInterval());
    for (int dataSetNr = 0; dataSetNr < datasToSend->size(); ++dataSetNr) {
        if (datasToSend[dataSetNr] != nullptr) {
            string dataToSend = datasToSend[dataSetNr].dump();
            // send it to server
            auto *client = new Client(configFileReader->getServerAddress(),
                                      configFileReader->getServerPort());
            client->sendDataToServer(dataToSend);
            // delay between each request 0,1 s
            usleep(100000);
        }
    }
}
