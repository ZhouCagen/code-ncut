#pragma once
#include "museflow/http_client.hpp"
#include <condition_variable>
#include <deque>
#include <functional>
#include <map>
#include <mutex>
#include <thread>
namespace museflow {
class TextJobs {
public:
    using Work = std::function<json(const std::atomic_bool&)>;
    TextJobs();
    ~TextJobs();
    json submit(Work work);
    int activeCount();
    json get(const std::string& id);
private:
    void run();
    std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<std::pair<std::string, Work>> pending_;
    std::deque<std::string> finished_;
    std::map<std::string, json> jobs_;
    std::atomic_bool stopping_{false};
    std::thread worker_;
};
}
