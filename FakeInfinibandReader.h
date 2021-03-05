#include <nlohmann/json.hpp>

class FakeInfinibandReader {

public:
    FakeInfinibandReader(int nodeNr, int interval);

public:

    nlohmann::json collectNodeInfos();
    int generateRandomNumber(int min, int max) const;

    int fakeValue(int *valueToIncrease, int isError);

private:
    int nodeNumber;
};