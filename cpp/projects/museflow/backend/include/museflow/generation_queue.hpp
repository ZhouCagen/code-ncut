#pragma once
#include "museflow/database.hpp"
#include "museflow/providers.hpp"
#include <condition_variable>
#include <deque>
#include <thread>

namespace museflow {
class GenerationQueue {
public:
    GenerationQueue(Database& db, std::filesystem::path data, std::unique_ptr<IMusicModelProvider> provider, SettingsService* settings = nullptr);
    ~GenerationQueue();
    json enqueue(const std::string& projectId, const MusicBlueprint& blueprint, const std::string& mode, const std::string& parentVersionId = "");
private:
    void run();
    bool pause(std::chrono::milliseconds duration);
    Database& db_;
    std::filesystem::path data_;
    std::unique_ptr<IMusicModelProvider> provider_;
    std::mutex mutex_;
    std::condition_variable condition_;
    struct Job { std::string id; MusicSettings settings; };
    std::deque<Job> pending_;
    SettingsService* settings_;
    std::atomic_bool stopping_ = false;
    std::thread worker_;
};
}
