//
// Created by master on 08.01.21.
//

#ifndef INFINIBANDCLIENTPROJECT_CLIENT_H
#define INFINIBANDCLIENTPROJECT_CLIENT_H


class Client {
public:
    Client(char *serverAddress);

    Client(char *serverAddress, int port);

    void sendDataToServer(std::string dataToSend);
};


#endif //INFINIBANDCLIENTPROJECT_CLIENT_H
