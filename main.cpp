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

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;
int port = 8088;
char* serverAddress = "93.245.196.10";

bool isRunning = true;

#define debug = true;

static void SignalHandler(int signal) {
    if(signal == SIGINT) {
        isRunning = false;
    }
}

int main() {
    // collect infiniband infos
    std::cout << "Collect infiniband" << std::endl;
    string dataToSend = "{a:x}";

    bool network = false;
    bool compatibility = true;
    Detector::IbFabric fabric(network, compatibility);

    uint32_t nodeNr = fabric.GetNumNodes();
    std::vector<Detector::IbNode *> nodes = fabric.GetNodes();
    uint8_t ports = nodes.front()->GetNumPorts();

    #ifdef debug
    std::cout << nodeNr << std::endl;
    std::cout << ports << std::endl;
    #endif

    for(Detector::IbNode *node : fabric.GetNodes()) {
        for(Detector::IbPort *port : node->GetPorts()) {
            dataToSend = printf("{node:%s, port:%u, transmitted:%lu", node->GetDescription().c_str(), port->GetNum(), port->GetXmitDataBytes());
        }
    }
    std::cout << dataToSend << std::endl;

    // socket creation
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cerr << "Socket can't be created" << endl;
        return -1;
    }

    // set address informations to client info struct
    sockaddr_in serverSocketAddressInformation;
    serverSocketAddressInformation.sin_family = AF_INET;
    serverSocketAddressInformation.sin_port = htons(port);
    // make it binary
    inet_pton(AF_INET, serverAddress, &serverSocketAddressInformation.sin_addr);

    //	Connect to server
    int connectionSuccess = connect(clientSocket, (sockaddr*)&serverSocketAddressInformation, sizeof(serverSocketAddressInformation));

    // error
    if (connectionSuccess == -1)
    {
        cerr << "Can't connect to server" << endl;
        cerr << errno << endl;
        return -1;
    }

    // send data
    char buffer[4096];

    int sendResult = send(clientSocket, dataToSend.c_str(), dataToSend.size() + 1, 0);

    if(dataToSend.size() + 1 - sendResult == 0){
        std::cout << "Data transmission completed" << std::endl;
    }

    close(clientSocket);
    return 0;
}
