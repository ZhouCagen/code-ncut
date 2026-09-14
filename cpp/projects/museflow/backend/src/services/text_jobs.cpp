#include "museflow/text_jobs.hpp"
#include "museflow/runtime.hpp"
#include "museflow/database.hpp"
namespace museflow {
TextJobs::TextJobs() : worker_([this] { run(); }) {}
TextJobs::~TextJobs() { stopping_ = true; condition_.notify_all(); worker_.join(); }
json TextJobs::submit(Work work) {
    std::lock_guard lock(mutex_);
    if (stopping_ || pending_.size() >= 8) throw ProviderError("queue_full", "文本任务队列已满，请稍后重试", 429);
    const auto id = uuid();
    json job = {{"id", id}, {"status", "queued"}, {"createdAt", timestamp()}};
    jobs_[id] = job; pending_.emplace_back(id, std::move(work)); condition_.notify_one(); return job;
}
int TextJobs::activeCount() {
    std::lock_guard lock(mutex_); int count = 0;
    for (const auto& [id, job] : jobs_) if (job["status"] == "queued" || job["status"] == "running") ++count;
    return count;
}
json TextJobs::get(const std::string& id) {
    std::lock_guard lock(mutex_); auto it = jobs_.find(id);
    if (it == jobs_.end()) throw NotFound("文本任务不存在、已过期或程序已重启，请重新发起");
    return it->second;
}
void TextJobs::run() {
    for (;;) {
        std::pair<std::string, Work> item;
        { std::unique_lock lock(mutex_); condition_.wait(lock, [this] { return stopping_ || !pending_.empty(); });
          if (stopping_) return;
          item = std::move(pending_.front()); pending_.pop_front(); jobs_[item.first]["status"] = "running"; }
        json outcome;
        try { outcome = {{"status", "completed"}, {"result", item.second(stopping_)}}; }
        catch (const ProviderError& error) { outcome = {{"status", "failed"}, {"error", error.what()}, {"code", error.code}}; }
        catch (const ValidationError& error) { outcome = {{"status", "failed"}, {"error", error.what()}, {"code", "validation"}}; }
        catch (...) { outcome = {{"status", "failed"}, {"error", "文本任务处理失败，请检查模型配置后重试"}, {"code", "internal"}}; }
        item.second = {}; // Release request settings/keys immediately; only sanitized public results remain.
        { std::lock_guard lock(mutex_);
          auto& job = jobs_[item.first]; job.update(outcome); job["updatedAt"] = timestamp(); finished_.push_back(item.first);
          while (finished_.size() > 64) { jobs_.erase(finished_.front()); finished_.pop_front(); } }
    }
}
}
