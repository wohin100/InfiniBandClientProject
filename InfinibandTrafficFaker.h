#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;

class InfinibandTrafficFaker {
public:
    InfinibandTrafficFaker();

public:
    nlohmann::basic_json<> *getTraffic(string configuration, int intervallTime);
};