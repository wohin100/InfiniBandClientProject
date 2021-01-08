#include <csignal>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Client.h"

using nlohmann::json;

using namespace std;
int serverPort = 80;
char *serverAddress;

bool isRunning = true;

//#define debug = true;

static void SignalHandler(int signal) {
    if (signal == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char *argv[]) {
    // check params
    if (argc < 3) {
        printf("Usage: ./InfiniBandClient <serverAddress> <port>\n");
        exit(EXIT_FAILURE);
    } else {
        serverAddress = argv[1];
        serverPort = stoi(argv[2]);
    }


    string dataToSend = "{ \"happy\": true, \"pi\": 3.141 }";
    auto j3 = json::parse(dataToSend);

    std::cout << j3["happy"] << "\n";

    // collect infiniband infos
    //InfinibandReader::collectNodeInfos();
    auto *client = new Client(serverAddress, serverPort);

    for (int i = 0; i < 10; ++i) {
        client->sendDataToServer(dataToSend);
    }

    return 0;

}
