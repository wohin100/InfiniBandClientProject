#include <nlohmann/json.hpp>

class FakeInfinibandReader {


    int fakeValue(int valueToIncrease);

    int fakeValue(int *valueToIncrease);

public:
    nlohmann::json collectNodeInfos(int nodeNr);
};