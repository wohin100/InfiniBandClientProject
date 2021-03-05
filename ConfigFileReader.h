#include <string>
using namespace std;
class ConfigFileReader {

public:
    ConfigFileReader();

    void read(string path);

    int getServerPort();

    string getServerAddress();

    int getClientNr();

    bool isTestMode();

    int getInterval();

    void setClientNr(int nr);

    bool isSingleTestMode();

    string getConfiguration();
};
