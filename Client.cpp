#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "InfinibandReader.h"
#include "Client.h"
#include <cstring>

using namespace std;

int clientSocket;
char* address;
int port;

Client::Client(string serverAddress, int serverPort) {
    parseServerAddress(serverAddress);

    port = serverPort;
}

void Client::parseServerAddress(const string &serverAddress) const {
    int length = serverAddress.length();

    char char_array[length + 1];
    strcpy(address, serverAddress.c_str());

    for (int charNr = 0; charNr < length; charNr++){
        cout << char_array[charNr];
    }
}

void Client::sendDataToServer(string dataToSend)
{
    // socket creation
    std::cout << "Send infiniband data to server" << std::endl;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Socket can't be created" << endl;
        exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }

    // send data
    char buffer[4096];

    int sendResult = send(clientSocket, dataToSend.c_str(), dataToSend.size() + 1, 0);

    if (dataToSend.size() + 1 - sendResult == 0) {
        std::cout << "Data transmission completed" << std::endl;
    }

    close(clientSocket);
}

