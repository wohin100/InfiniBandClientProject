#include <string>
using namespace std;

class Client {
public:
    Client(string);

    Client(basic_string<char> serverAddress, int port);

    void sendDataToServer(std::string dataToSend);

    void parseServerAddress(const string &serverAddress) const;
};
