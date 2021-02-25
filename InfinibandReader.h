#include <nlohmann/json.hpp>

class InfinibandReader {
    public: static nlohmann::basic_json<> collectNodeInfos();

    static nlohmann::json collectNodeInfos(int clientNr);
};
