#include "FakeInfinibandReader.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
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

int periodMin = 3000;
int periodMax = 9000;

int nodeNumber = 0;
int checkPoint = 0;
int intervalTime = 0;
int periodIsSilenced = 1;
int periodFactor = rand() % 10 + 1;
int periodRunningCounter = 0;
// how long will period run (between 100000 and 300000)
int periodEnd = periodMin;

int test = 1;
int meanLastTen = 0;
int meanValue = 0;

json FakeInfinibandReader::collectNodeInfos() {
    json jsonToTransfer;


    string text = "TestNode";
    text += std::to_string(nodeNumber);
    jsonToTransfer["nodeDescription"] = text;
    jsonToTransfer["nodeNumPorts"] = 1;
    text = "00000";
    text += std::to_string(nodeNumber);
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
    int value = generateRandomNumber(0,2);
    int currentFactor = generateRandomNumber(1,20);

    if (periodRunningCounter < periodEnd ){
        periodRunningCounter++;
    }
    // reset periodFactor, period and counter
    else{
        periodEnd = periodMin + generateRandomNumber(periodMin,periodMax);
        periodFactor = generateRandomNumber(1,100);
        periodRunningCounter = 0;
        periodIsSilenced = generateRandomNumber(1,2) % 2;
        test = test+1;
        meanValue = 0;
        std::cout << periodIsSilenced << std::endl;

    }
    int increaseValue = periodFactor * value + currentFactor;

    meanLastTen = meanLastTen + increaseValue;

    // calculate mean value of last 10 seconds
    if(periodRunningCounter % checkPoint == 0){
        meanValue = (meanLastTen / checkPoint) * (1000 / intervalTime);
        meanLastTen = 0;

        if (periodIsSilenced != 0){
            printf("Client%d -> %d\n", nodeNumber, meanValue);
        }
        else{
            printf("Client%d -> %d\n", nodeNumber, 0);
        }
    }

    if ((periodEnd-periodRunningCounter) % 100 == 0) {
        //std::cout << (periodEnd - periodRunningCounter) << std::endl;
        //std::cout << increaseValue << std::endl;
    }
    if(periodIsSilenced == 0){
        return *valueToIncrease;
        //return test;
    }
    else{
        *valueToIncrease = *valueToIncrease + increaseValue;
        return *valueToIncrease;
        //return test;
    }
}

int FakeInfinibandReader::generateRandomNumber(int min, int max) const {
    srand((unsigned) time(0)*nodeNumber);
    int randomNumber;
    randomNumber = (rand() % max) + min;
    return randomNumber;
}

FakeInfinibandReader::FakeInfinibandReader(int nodeNr, int interval) {
    intervalTime = interval;
    nodeNumber = nodeNr;
    int numberOfFakeFunctionCalls = 19;
    checkPoint = (10000 / intervalTime) * numberOfFakeFunctionCalls;
}
