#include "InfinibandTrafficFaker.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>
#include "Client.h"
#include "InfinibandReader.h"
#include "ConfigFileReader.h"
#include "FakeInfinibandReader.h"

using nlohmann::json;
using namespace std;

InfinibandTrafficFaker::InfinibandTrafficFaker() {}

nlohmann::basic_json<> * InfinibandTrafficFaker::getTraffic(string configuration, int intervallTime) {
    // get config a json
    json receivedJson;
    try {
        receivedJson = json::parse(configuration);
    } catch (const nlohmann::detail::parse_error& e) {
        cerr << e.what() << endl;
    }

    json nodes = receivedJson["nodes"].get<json>();
    FakeInfinibandReader *fakers[nodes.size()];


    // create a faker for each node
    for (int nodeNr = 0; nodeNr < nodes.size(); ++nodeNr) {
        int nodeDescriptionNr = nodes[std::to_string(nodeNr)].get<int>();
        fakers[nodeNr] = new FakeInfinibandReader(nodeDescriptionNr, intervallTime);
    }

    // iterate over relations
    json relations = receivedJson["relations"].get<json>();
    static json results[40];

    int* alreadyReceivedPackages = new int[40];
    int* alreadySendPackages = new int[40];
    for (int relationNr = 0; relationNr < relations.size(); ++relationNr) {
        json relation = relations[std::to_string(relationNr)].get<json>();
        int nodeNumberFrom = relation["from"].get<int>();
        int nodeNumberTo = relation["to"].get<int>();
        json from = fakers[nodeNumberFrom - 1]->collectNodeInfos();
        json to = fakers[nodeNumberTo - 1]->collectNodeInfos();

        // change values
        if(alreadySendPackages[nodeNumberFrom - 1] != 1){
            //no packages send and no packages received [][]
            if(alreadyReceivedPackages[nodeNumberTo - 1] != 1){
                // [xmit:getValueFromReceiver | rcv:1] => [xmit:1 | rcv:stay]
                int toRcv = to["nodeRcvPkts"];
                from["nodeXmitPkts"] = toRcv;

                from["nodeRcvPkts"] = 1;
                to["nodeXmitPkts"] = 1;
            }
            //no packages send but already packages received [][X]
            else{
                // [xmit:stay | rcv:stay] => [xmit:1 | rcv:stay+getValueFromSender]
                int fromXmit = from["nodeXmitPkts"];
                int toRcv = from["nodeRcvPkts"];
                to["nodeRcvPkts"] = toRcv+fromXmit;
                to["nodeXmitPkts"] = 1;
            }


        }
        else{
            //already packages send but no packages received [X][]
            if(alreadyReceivedPackages[nodeNumberTo - 1] != 1){
                std::cout << nodeNumberFrom << std::endl;
                // [xmit:stay+getValueFromReceiver | rcv:1] => [xmit:1 | rcv:stay]
                int toRcv = to["nodeRcvPkts"];
                int fromXmit = from["nodeXmitPkts"];
                from["nodeXmitPkts"] = fromXmit+toRcv;

                from["nodeRcvPkts"] = 1;
                to["nodeXmitPkts"] = 1;
            }
            //already packages send and already packages received [X][X]
            else{
                // [xmit:stay+getValueFromReceiver | rcv:stay] => [xmit:stay | rcv:stay+]
                int toRcv = to["nodeRcvPkts"];
                int fromXmit = from["nodeXmitPkts"];
                from["nodeXmitPkts"] = fromXmit+toRcv;
            }
        }

        alreadySendPackages[nodeNumberFrom - 1] = 1;
        alreadyReceivedPackages[nodeNumberTo - 1] = 1;

        results[nodeNumberFrom - 1] = from;
        results[nodeNumberTo - 1] = to;
    }


/*
    int nodeNumberFrom = relations["from"].get<int>();
    int nodeNumberTo = relations["to"].get<int>();


    FakeInfinibandReader *fakeInfinibandReaderFrom = new FakeInfinibandReader(nodeNumberFrom, intervallTime);
    FakeInfinibandReader *fakeInfinibandReaderTo = new FakeInfinibandReader(nodeNumberTo, intervallTime);
    json infos = fakeInfinibandReaderFrom->collectNodeInfos();
    json infos2 = fakeInfinibandReaderTo->collectNodeInfos();

    json infos3 = fakers[0]->collectNodeInfos();
    json infos4 = fakers[1]->collectNodeInfos();
    results[0] = infos3;
    results[1] = infos4;*/
    return results;
}