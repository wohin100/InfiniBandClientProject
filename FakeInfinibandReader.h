#include <nlohmann/json.hpp>

class FakeInfinibandReader {

public:
    FakeInfinibandReader();

    FakeInfinibandReader(int nodeNr, int interval);

private:

    int fakeValue(int *valueToIncrease);

public:

    nlohmann::json collectNodeInfos(int nodeNr, int interval);

    nlohmann::json collectNodeInfos();
    int generateRandomNumber(int min, int max) const;

    int fakeValue(int *valueToIncrease, int isError);
};