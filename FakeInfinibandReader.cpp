#include "FakeInfinibandReader.h"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace std;

int nodeMulticastRcvPkts = 0;
int nodeMulticastXmitPkts = 0;

int nodeUnicastRcvPkts = 0;
int nodeUnicastXmitPkts = 0;

int nodeRcvPkts = 0;
int nodeRcvErrors = 0;
int nodeRcvConstraintErrors = 0;
int nodeRcvDataBytes = 0;
int nodeRcvRemotePhysicalErrors = 0;
int nodeRcvSwitchRelayErrors = 0;

int nodeXmitWait = 0;
int nodeXmitPkts = 0;
int nodeXmitDiscards = 0;
int nodeXmitDataBytes = 0;
int nodeXmitConstraintErrors = 0;

int nodeExcessiveBufferOverrunErrors = 0;
int nodeLinkDownedCounter = 0;
int nodeLocalLinkIntegrityErrors = 0;
int nodeLinkRecoveryCounter = 0;

int periodFactor = rand() % 10 + 1;
int periodRunningCounter = 0;
// how long will period run (between 100000 and 300000)
int periodEnd = 100000 + (rand() % 100000 + 200000);

json FakeInfinibandReader::collectNodeInfos(int nodeNr) {
    json jsonToTransfer;

    string text = "TestNode";
    text += std::to_string(nodeNr);
    jsonToTransfer["nodeDescription"] = text;
    jsonToTransfer["nodeNumPorts"] = 1;
    text = "00000";
    text += std::to_string(nodeNr);
    jsonToTransfer["nodeGuid"] = text;

    jsonToTransfer["nodeUnicastRcvPkts"] = fakeValue(&nodeUnicastRcvPkts);
    jsonToTransfer["nodeUnicastXmitPkts"] = fakeValue(&nodeUnicastXmitPkts);

    jsonToTransfer["nodeMulticastRcvPkts"] = fakeValue(&nodeMulticastRcvPkts);
    jsonToTransfer["nodeMulticastXmitPkts"] = fakeValue(&nodeMulticastXmitPkts);

    jsonToTransfer["nodeRcvPkts"] = fakeValue(&nodeRcvPkts);
    jsonToTransfer["nodeRcvErrors"] = fakeValue(&nodeRcvErrors);
    jsonToTransfer["nodeRcvConstraintErrors"] = fakeValue(&nodeRcvConstraintErrors);
    jsonToTransfer["nodeRcvDataBytes"] = fakeValue(&nodeRcvDataBytes);
    jsonToTransfer["nodeRcvRemotePhysicalErrors"] = fakeValue(&nodeRcvRemotePhysicalErrors);
    jsonToTransfer["nodeRcvSwitchRelayErrors"] = fakeValue(&nodeRcvSwitchRelayErrors);

    jsonToTransfer["nodeXmitWait"] = fakeValue(&nodeXmitWait);
    jsonToTransfer["nodeXmitPkts"] = fakeValue(&nodeXmitPkts);
    jsonToTransfer["nodeXmitDiscards"] = fakeValue(&nodeXmitDiscards);
    jsonToTransfer["nodeXmitDataBytes"] = fakeValue(&nodeXmitDataBytes);
    jsonToTransfer["nodeXmitConstraintErrors"] = fakeValue(&nodeXmitConstraintErrors);

    jsonToTransfer["nodeExcessiveBufferOverrunErrors"] = fakeValue(&nodeExcessiveBufferOverrunErrors);
    jsonToTransfer["nodeLinkDownedCounter"] = fakeValue(&nodeLinkDownedCounter);
    jsonToTransfer["nodeLocalLinkIntegrityErrors"] = fakeValue(&nodeLocalLinkIntegrityErrors);
    jsonToTransfer["nodeLinkRecoveryCounter"] = fakeValue(&nodeLinkRecoveryCounter);

    jsonToTransfer["nodeVL15Dropped"] = 0;

    return jsonToTransfer;
}


int FakeInfinibandReader::fakeValue(int* valueToIncrease) {
    int value = rand() % 2;
    int currentFactor = rand() % 100 + 1;

    if (periodRunningCounter < periodEnd ){
        periodRunningCounter++;
    }
    // reset periodFactor, period and counter
    else{
        periodEnd = 100000 + (rand() % 100000 + 200000);
        periodFactor = rand() % 100 + 1;
        periodRunningCounter = 0;
    }
    int increaseValue = periodFactor * value + currentFactor;
    *valueToIncrease = *valueToIncrease + increaseValue;
    return *valueToIncrease;
}
