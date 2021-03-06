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
string a;

//#define DEBUG "dummy"


Client::Client(string serverAddress, int serverPort) {
    //parseServerAddress(serverAddress);
    int length = serverAddress.length();

    char charArray[length + 1];
    strcpy(charArray, serverAddress.c_str());

    port = serverPort;

    address = new char[a.length() + 1];
    strcpy(address, a.c_str());
    delete [] address;
}

void Client::sendDataToServer(string dataToSend)
{
    // socket creation

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
    int connectionSuccess = connect(
            clientSocket,
            (sockaddr *) &serverSocketAddressInformation,
            sizeof(serverSocketAddressInformation)
            );

    // error
    if (connectionSuccess == -1) {
        cerr << "Can't connect to server" << endl;
        cerr << errno << endl;
        exit(EXIT_FAILURE);
    }

    // send data
    char buffer[4096];

    int sendResult = send(clientSocket, dataToSend.c_str(), dataToSend.size() + 1, 0);

    close(clientSocket);
}

