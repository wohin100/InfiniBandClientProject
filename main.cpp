#include <csignal>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>
#include <unistd.h>
#include "Client.h"
#include "InfinibandReader.h"

using nlohmann::json;

using namespace std;
int serverPort = 80;
char *serverAddress;
int clientNr = 1;


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
        clientNr = stoi(argv[3]);
    }

    int ground = 10;
    string dataToSend =  "{\"MulticastRcvPkts\":8,\"node\":\"mlx4_0\",\"port\":1,\"received\":"+std::to_string(ground)+",\"transmitted\":33984}";

    while (true) {
    // collect infiniband infos
    json infos = InfinibandReader::collectNodeInfos();
    string debugOutput = infos.dump();
    auto *client = new Client(serverAddress, serverPort);

        int randomNumber = rand() % 10;
        dataToSend =  "{\"MulticastRcvPkts\":8,\"node\":\""+std::to_string(clientNr)+"\",\"port\":1,\"received\":"+std::to_string(randomNumber)+",\"transmitted\":33984}";
        client->sendDataToServer(debugOutput);
        usleep(100000);
    }

    return 0;

}
