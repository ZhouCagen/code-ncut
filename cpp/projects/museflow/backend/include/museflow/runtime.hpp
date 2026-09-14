#pragma once
#include <filesystem>
#include <string>
namespace museflow {
std::string uuid();
bool isUuid(const std::string& value);
std::string timestamp();
class DataDirectoryLock {
public:
    explicit DataDirectoryLock(const std::filesystem::path& directory);
    ~DataDirectoryLock();
    DataDirectoryLock(const DataDirectoryLock&) = delete;
    DataDirectoryLock& operator=(const DataDirectoryLock&) = delete;
private:
    int descriptor_ = -1;
};
struct RuntimePaths {
    std::filesystem::path config, data, cache;
    static RuntimePaths create();
};
}
