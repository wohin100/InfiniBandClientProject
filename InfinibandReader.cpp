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
        std::cout << "Collect infiniband data" << std::endl;

        bool network = false;
        bool compatibility = true;
        IbFabric fabric(network, compatibility);

        fabric.RefreshCounters();
        vector<Detector::IbNode *> nodeVector = fabric.GetNodes();
        for (Detector::IbNode *node : nodeVector) {
            jsonToTransfer["nodeDescription"] = node->GetDescription();
            jsonToTransfer["nodeGuid"] = node->GetGuid();
            jsonToTransfer["nodeNumPorts"] = node->GetNumPorts();
            jsonToTransfer["nodeExcessiveBufferOverrunErrors"] = node->GetExcessiveBufferOverrunErrors();
            jsonToTransfer["nodeLinkDownedCounter"] = node->GetLinkDownedCounter();
            jsonToTransfer["nodeLocalLinkIntegrityErrors"] = node->GetLocalLinkIntegrityErrors();
            jsonToTransfer["nodeLinkRecoveryCounter"] = node->GetLinkRecoveryCounter();
            jsonToTransfer["nodeLinkDownedCounter"] = node->GetLinkDownedCounter();
            jsonToTransfer["nodeExcessiveBufferOverrunErrors"] = node->GetExcessiveBufferOverrunErrors();
            jsonToTransfer["nodeMulticastRcvPkts"] = node->GetMulticastRcvPkts();
            jsonToTransfer["nodeMulticastXmitPkts"] = node->GetMulticastXmitPkts();
            jsonToTransfer["nodeXmitWait"] = node->GetXmitWait();
            jsonToTransfer["nodeXmitPkts"] = node->GetXmitPkts();
            jsonToTransfer["nodeXmitDiscards"] = node->GetXmitDiscards();
            jsonToTransfer["nodeXmitDataBytes"] = node->GetXmitDataBytes();
            jsonToTransfer["nodeXmitConstraintErrors"] = node->GetXmitConstraintErrors();
            jsonToTransfer["nodeVL15Dropped"] = node->GetVL15Dropped();
            jsonToTransfer["nodeRcvPkts"] = node->GetRcvPkts();
            jsonToTransfer["nodeRcvErrors"] = node->GetRcvErrors();
            jsonToTransfer["nodeRcvConstraintErrors"] = node->GetRcvConstraintErrors();
            jsonToTransfer["nodeRcvDataBytes"] = node->GetRcvDataBytes();
            jsonToTransfer["nodeRcvRemotePhysicalErrors"] = node->GetRcvRemotePhysicalErrors();
            jsonToTransfer["nodeRcvSwitchRelayErrors"] = node->GetRcvSwitchRelayErrors();
            jsonToTransfer["nodeUnicastRcvPkts"] = node->GetUnicastRcvPkts();
            jsonToTransfer["nodeUnicastXmitPkts"] = node->GetUnicastXmitPkts();
            vector<Detector::IbPort *> portVector = node->GetPorts();
            int portNr = 0;
            for (Detector::IbPort *port : portVector) {
                jsonToTransfer[&"port"[portNr]] = port->GetNum();
                jsonToTransfer[&"transmitted"[portNr]] = port->GetXmitDataBytes();
                jsonToTransfer[&"received"[portNr]] = port->GetRcvDataBytes();
                jsonToTransfer[&"MulticastRcvPkts"[portNr]] = port->GetMulticastRcvPkts();
            }
        }
        string debugOutput = jsonToTransfer.dump();
        cout << debugOutput << std::endl;

    } catch (const std::exception &e) {
        jsonToTransfer["node"] = "failed";
        jsonToTransfer["port"] = 99;
        jsonToTransfer["transmitted"] = 1;
        jsonToTransfer["received"] = 1;
        jsonToTransfer["MulticastRcvPkts"] = 1;
    }
    return jsonToTransfer;
}
