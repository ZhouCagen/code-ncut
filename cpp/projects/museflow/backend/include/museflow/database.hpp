#pragma once
#include "museflow/models.hpp"
#include <filesystem>
#include <mutex>
#include <sqlite3.h>

namespace museflow {
struct NotFound : std::runtime_error { using std::runtime_error::runtime_error; };
class Database {
public:
    explicit Database(const std::filesystem::path& path);
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    json projects();
    json project(const std::string& id);
    json saveProject(const json& payload, const std::string& id = "");
    void deleteProject(const std::string& id);
    void createTask(const json& task);
    json task(const std::string& id);
    void updateTask(const std::string& id, const std::string& status, int progress, const std::string& error = "");
    void completeTask(const std::string& id, const json& version);
    json version(const std::string& id);
    json branchVersion(const std::string& id);
    json updateVersionMetadata(const std::string& id, const json& patch);
    json deleteVersion(const std::string& id);
    json exportProject(const std::string& id);
    int activeTaskCount();
    json versions(const std::string& projectId = "");
private:
    sqlite3* db_ = nullptr;
    std::mutex mutex_;
};
}
