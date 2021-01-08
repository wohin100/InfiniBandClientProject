#include "InfinibandReader.h"
#include <iostream>
#include <detector/IbFabric.h>
#include <detector/IbNode.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using nlohmann::json;
using Detector::IbFabric;

using namespace std;

json InfinibandReader::collectNodeInfos() {
    json jsonToTransfer;
    try {
        //std::cout << "Collect infiniband data" << std::endl;

        bool network = false;
        bool compatibility = true;
        IbFabric fabric(network, compatibility);

        fabric.RefreshCounters();
        vector<Detector::IbNode *> nodeVector = fabric.GetNodes();
        for (Detector::IbNode *node : nodeVector) {
            vector<Detector::IbPort *> portVector = node->GetPorts();
            for (Detector::IbPort *port : portVector) {
                jsonToTransfer["node"] = node->GetDescription();
                jsonToTransfer["port"] = port->GetNum();
                jsonToTransfer["transmitted"] = port->GetXmitDataBytes();
                jsonToTransfer["received"] = port->GetRcvDataBytes();
                jsonToTransfer["MulticastRcvPkts"] = port->GetMulticastRcvPkts();
            }
        }
        string debugOutput = jsonToTransfer.dump();
        cout << debugOutput << std::endl;

    } catch (const std::exception &e) {
    }
    return jsonToTransfer;
}
