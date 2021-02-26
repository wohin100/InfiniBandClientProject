#include "ConfigFileReader.h"
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

void trimString(string &token);

void applyInputValueToProperty(const string &key, const string &value);

using namespace std;

int serverPort = 80;
string serverAddress;
int clientNr = 1;
int testMode = 0;
int interval = 100000;

ConfigFileReader::ConfigFileReader() = default;

int ConfigFileReader::getServerPort(){
    return serverPort;
}

string ConfigFileReader::getServerAddress(){
    return serverAddress;
}

int ConfigFileReader::getClientNr(){
    return clientNr;
}

int ConfigFileReader::getInterval(){
    return interval;
}

void ConfigFileReader::setClientNr(int nr){
    clientNr = nr;
}

bool ConfigFileReader::isTestMode(){
    if (testMode == 0){
        return false;
    } else{
        return true;
    }
}

void splitLine(string line){
    string delimiter = "=";

    size_t position = 0;
    string key;
    string value;
    int tokens = 0;
    while ((position = line.find(delimiter)) != string::npos) {
        key = line.substr(0, position);
        value = line.substr(position + 1, line.length());

        trimString(key);
        trimString(value);

        applyInputValueToProperty(key, value);

        line.erase(0, position + delimiter.length());
        tokens++;
    }
    if(tokens > 1){
        cerr << "Config file is invalid" << endl;
        exit(EXIT_FAILURE);
    }
}

void applyInputValueToProperty(const string &key, const string &value) {
    if (key == "serverAddress"){
        serverAddress = value;
    }
    if (key == "serverPort"){
        serverPort = stoi(value);
    }
    if (key == "clientNr"){
        clientNr = stoi(value);
    }
    if (key == "testMode"){
        testMode = stoi(value);
    }
    if (key == "interval"){
        interval = stoi(value);
    }
}

void trimString(string &token) {
    while(!token.empty() && isspace(token.front()) || isblank(token.front()) ){
        token.erase(token.begin());
    }
    while(!token.empty() && isspace(token.back()) || isblank(token.back()) ){
        token.pop_back();
    }
}

void ConfigFileReader::read(string path)
{
    ifstream file(path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            splitLine(line);
        }
        file.close();
    }
    else{
        cerr << "Could not open config file" << endl;
        cerr << path << endl;
        exit(EXIT_FAILURE);
    }
}