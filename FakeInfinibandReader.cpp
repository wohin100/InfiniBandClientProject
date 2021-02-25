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

    jsonToTransfer["nodeUnicastRcvPkts"] = fakeValue(&nodeUnicastRcvPkts, 0);
    jsonToTransfer["nodeUnicastXmitPkts"] = fakeValue(&nodeUnicastXmitPkts, 0);

    jsonToTransfer["nodeMulticastRcvPkts"] = fakeValue(&nodeMulticastRcvPkts, 0);
    jsonToTransfer["nodeMulticastXmitPkts"] = fakeValue(&nodeMulticastXmitPkts, 0);

    jsonToTransfer["nodeRcvPkts"] = fakeValue(&nodeRcvPkts, 0);
    jsonToTransfer["nodeRcvErrors"] = fakeValue(&nodeRcvErrors, 1);
    jsonToTransfer["nodeRcvConstraintErrors"] = fakeValue(&nodeRcvConstraintErrors, 2);
    jsonToTransfer["nodeRcvDataBytes"] = fakeValue(&nodeRcvDataBytes, 0);
    jsonToTransfer["nodeRcvRemotePhysicalErrors"] = fakeValue(&nodeRcvRemotePhysicalErrors, 3);
    jsonToTransfer["nodeRcvSwitchRelayErrors"] = fakeValue(&nodeRcvSwitchRelayErrors, 4);

    jsonToTransfer["nodeXmitWait"] = fakeValue(&nodeXmitWait, 0);
    jsonToTransfer["nodeXmitPkts"] = fakeValue(&nodeXmitPkts, 0);
    jsonToTransfer["nodeXmitDiscards"] = fakeValue(&nodeXmitDiscards, 5);
    jsonToTransfer["nodeXmitDataBytes"] = fakeValue(&nodeXmitDataBytes, 0);
    jsonToTransfer["nodeXmitConstraintErrors"] = fakeValue(&nodeXmitConstraintErrors, 6);

    jsonToTransfer["nodeExcessiveBufferOverrunErrors"] = fakeValue(&nodeExcessiveBufferOverrunErrors, 7);
    jsonToTransfer["nodeLinkDownedCounter"] = fakeValue(&nodeLinkDownedCounter, 0);
    jsonToTransfer["nodeLocalLinkIntegrityErrors"] = fakeValue(&nodeLocalLinkIntegrityErrors, 8);
    jsonToTransfer["nodeLinkRecoveryCounter"] = fakeValue(&nodeLinkRecoveryCounter, 0);

    jsonToTransfer["nodeVL15Dropped"] = 0;

    jsonToTransfer["nodeName"] = text;

    return jsonToTransfer;
}

int FakeInfinibandReader::fakeValue(int* valueToIncrease, int isError) {
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

    // error counter should increase very slow
    if(isError > 0){
        int setError = generateRandomNumber(1, 1000) % 1;
        if (setError == 0){
            return isError;
        } else{
            return 0;
        }
    }

    if(periodIsSilenced == 0){
        return *valueToIncrease;
    }
    else{
        *valueToIncrease = *valueToIncrease + increaseValue;
        return *valueToIncrease;
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
