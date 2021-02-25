#include <nlohmann/json.hpp>

class InfinibandReader {
    public: static nlohmann::basic_json<> collectNodeInfos();

    nlohmann::json collectNodeInfos(int clientNr);
};
