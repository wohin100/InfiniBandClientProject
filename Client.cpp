//
// Created by master on 08.01.21.
//

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <nlohmann/json.hpp>
#include "InfinibandReader.h"
#include "Client.h"

using namespace std;

int clientSocket;
char* address;
int port;

Client::Client(char* serverAddress, int serverPort) {
    address = serverAddress;
    port = serverPort;
}

void Client::sendDataToServer(string dataToSend)
{
    // socket creation
    std::cout << "Send infiniband data to server" << std::endl;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Socket can't be created" << endl;
        throw "Socket can't be created";
    }

    // set address informations to client info struct
    sockaddr_in serverSocketAddressInformation;
    serverSocketAddressInformation.sin_family = AF_INET;
    serverSocketAddressInformation.sin_port = htons(port);
    // make it binary
    inet_pton(AF_INET, address, &serverSocketAddressInformation.sin_addr);

    //	Connect to server
    int connectionSuccess = connect(clientSocket, (sockaddr *) &serverSocketAddressInformation,
                                    sizeof(serverSocketAddressInformation));

    // error
    if (connectionSuccess == -1) {
        cerr << "Can't connect to server" << endl;
        cerr << errno << endl;
        throw "Can't connect to server";
    }

    // send data
    char buffer[4096];

    int sendResult = send(clientSocket, dataToSend.c_str(), dataToSend.size() + 1, 0);

    if (dataToSend.size() + 1 - sendResult == 0) {
        std::cout << "Data transmission completed" << std::endl;
    }

    close(clientSocket);
}
