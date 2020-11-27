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
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace std;
int port = 80;
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
        port = stoi(argv[2]);
    }


    string dataToSend = "{}";




    // collect infiniband infos
    try {

        //std::cout << "Collect infiniband data" << std::endl;

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
        while (true) {
            /*
            json jsonToTransfer;
            int xmitData = 0;
            int recData = 0;
            fabric.RefreshCounters();
            for (Detector::IbNode *node : fabric.GetNodes()) {
                for (Detector::IbPort *port : node->GetPorts()) {
                    //printf("{node:%s, port:%u, transmitted:%lu, received:%lu}", node->GetDescription().c_str(),
                    //       port->GetNum(), port->GetXmitDataBytes(), port->GetRcvDataBytes());
                    //std::cout << std::endl;
                    //jsonToTransfer["node"] = node->GetDescription();
                    //jsonToTransfer["port"] = port->GetNum();
                    //jsonToTransfer["transmitted"] = port->GetXmitDataBytes();
                    //jsonToTransfer["received"] = port->GetRcvDataBytes();
                    //xmitData = port->GetXmitDataBytes();
                    //recData = port->GetRcvDataBytes();
                }
            }
            if (xmitData > 0 || recData > 0) {
                std::string debugOutput = jsonToTransfer.dump();
                std::cout << debugOutput << std::endl;
            }
             */

            fabric.RefreshCounters();
            std::vector<Detector::IbNode *> nodeVector = fabric.GetNodes();
            for (Detector::IbNode *node : nodeVector){
                int rec = node->GetRcvDataBytes();
                //std::cout << rec << std::endl;
                std::vector<Detector::IbPort *> portVector = node->GetPorts();
                for (Detector::IbPort *port : portVector){
                    int rec = port->GetRcvDataBytes();
                    std::cout << rec << std::endl;
                }
            }
            //std::cout << nodeVector << std::endl << std::endl;
        }
    } catch (const std::exception &e) {


    }
    // socket creation
    std::cout << "Send infiniband data to server" << std::endl;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
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
    int connectionSuccess = connect(clientSocket, (sockaddr *) &serverSocketAddressInformation,
                                    sizeof(serverSocketAddressInformation));

    // error
    if (connectionSuccess == -1) {
        cerr << "Can't connect to server" << endl;
        cerr << errno << endl;
        return -1;
    }

    // send data
    char buffer[4096];

    int sendResult = send(clientSocket, dataToSend.c_str(), dataToSend.size() + 1, 0);

    if (dataToSend.size() + 1 - sendResult == 0) {
        std::cout << "Data transmission completed" << std::endl;
    }

    close(clientSocket);
    return 0;
}
