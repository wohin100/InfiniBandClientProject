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

using nlohmann::json;

using namespace std;
ConfigFileReader *configFileReader;
FakeInfinibandReader *fakeInfinibandReader;

bool isRunning = true;

#define DEBUG "dummy"

static void SignalHandler(int signal) {
    if (signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[]) {
    // check params
    if (argc < 2) {
        printf("Usage: ./InfiniBandClient <configFilePath>\n");
        exit(EXIT_FAILURE);
    } else {
        configFileReader = new ConfigFileReader();
        configFileReader->read(argv[1]);
    }

    #ifdef DEBUG
    std::cout << configFileReader->getInterval() << std::endl;
    #endif


    if(configFileReader->isTestMode()){
        this_thread::sleep_for(chrono::milliseconds(configFileReader->getClientNr() * 1000));
        fakeInfinibandReader = new FakeInfinibandReader(configFileReader->getClientNr(), configFileReader->getInterval());
        while (isRunning){
            json infos = fakeInfinibandReader->collectNodeInfos();
            string dataToSend = infos.dump();
            #ifdef DEBUG
            cout << dataToSend << endl;
            #endif

            // send it to server
            auto *client = new Client(configFileReader->getServerAddress(), configFileReader->getServerPort());

            client->sendDataToServer(dataToSend);

            // wait some time till next data collection
            this_thread::sleep_for(chrono::milliseconds(configFileReader->getInterval()));
        }
    }
    else{
        string dataToSend =  "";
        while (isRunning){
            json infos = InfinibandReader::collectNodeInfos(configFileReader->getClientNr());
            dataToSend = infos.dump();

            // send it to server
            auto *client = new Client(configFileReader->getServerAddress(), configFileReader->getServerPort());

            client->sendDataToServer(dataToSend);

            // wait some time till next data collection
            usleep(configFileReader->getInterval());
        }
    }

    /*
    int ground = 10;
    string dataToSend =  "{\"MulticastRcvPkts\":8,\"node\":\"mlx4_0\",\"port\":1,\"received\":"+std::to_string(ground)+",\"transmitted\":33984}";

    while (isRunning) {
        int randomNumber = rand() % 10;
        dataToSend =  "";
        if (configFileReader->isTestMode()){
            dataToSend =  "{\"MulticastRcvPkts\":8,\"node\":\""+
                    std::to_string(configFileReader->getClientNr())+
                    "\",\"port\":1,\"received\":"+
                    std::to_string(randomNumber)+
                    ",\"transmitted\":33984}";
        }
        else{
            // collect infiniband infos
            json infos = InfinibandReader::collectNodeInfos();
            dataToSend = infos.dump();
            cout << dataToSend << endl;
        }

        // send it to server
        auto *client = new Client(configFileReader->getServerAddress(), configFileReader->getServerPort());

        client->sendDataToServer(dataToSend);

        // wait some time till next data collection
        usleep(configFileReader->getInterval());
    }
    */
    return 0;
}
