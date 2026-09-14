#include "museflow/generation_queue.hpp"
#include "museflow/runtime.hpp"
#include "museflow/audio_processing.hpp"

namespace museflow {
GenerationQueue::GenerationQueue(Database& db, std::filesystem::path data, std::unique_ptr<IMusicModelProvider> provider, SettingsService* settings)
    : db_(db), data_(std::move(data)), provider_(std::move(provider)), settings_(settings), worker_([this] { run(); }) {}
GenerationQueue::~GenerationQueue() {
    { std::lock_guard lock(mutex_); stopping_ = true; }
    condition_.notify_all(); worker_.join();
}
json GenerationQueue::enqueue(const std::string& projectId, const MusicBlueprint& blueprint, const std::string& mode, const std::string& parentVersionId) {
    validate(blueprint);
    if (!isUuid(projectId)) throw ValidationError("项目 ID 无效");
    if (mode != "preview" && mode != "full") throw ValidationError("生成模式必须是 preview 或 full");
    if (!parentVersionId.empty() && !isUuid(parentVersionId)) throw ValidationError("父版本 ID 无效");
    auto snapshot = settings_ ? settings_->musicSnapshot() : MusicSettings{};
    if (snapshot.provider == "lyria" && snapshot.apiKey.empty()) throw ProviderError("missing_key", "请先在模型设置输入 Google Gemini API Key，或设置 GEMINI_API_KEY", 400);
    if (snapshot.provider == "songapi" && snapshot.apiKey.empty()) throw ProviderError("missing_key", "请先在模型设置输入 SongAPI Key，或设置 SONGAPI_KEY", 400);
    if (snapshot.provider == "elevenlabs" && snapshot.apiKey.empty()) throw ProviderError("missing_key", "请先在模型设置输入 ElevenLabs API Key，或设置 ELEVENLABS_API_KEY", 400);
    auto rendered = blueprint;
    if (mode == "preview") {
        const double scale = 30.0 / blueprint.targetDurationSeconds;
        rendered.targetDurationSeconds = 30;
        for (auto& section : rendered.structure) { section.startSeconds *= scale; section.endSeconds *= scale; }
        rendered.finalLyriaPrompt = composePrompt(rendered);
    }
    std::lock_guard lock(mutex_);
    if (stopping_ || pending_.size() >= 32) throw ValidationError("生成队列已满或正在关闭，请稍后重试");
    json task = {{"id", uuid()}, {"projectId", projectId}, {"blueprint", blueprint}, {"mode", mode}, {"provider", snapshot.provider},
        {"model", snapshot.provider == "mock" ? "MockMusicProvider" : ((snapshot.provider == "songapi" || snapshot.provider == "elevenlabs") ? snapshot.model : (mode == "preview" ? "lyria-3-clip-preview" : "lyria-3.5"))},
        {"generationPrompt", rendered.finalLyriaPrompt}, {"requestedFormat", snapshot.outputFormat}, {"format", blueprint.strictDuration ? "wav" : snapshot.outputFormat},
        {"parentVersionId", parentVersionId.empty() ? json(nullptr) : json(parentVersionId)}, {"status", "queued"}, {"progress", 0}, {"error", ""}, {"createdAt", timestamp()}, {"updatedAt", timestamp()},
        {"modelTargetDurationSeconds", mode == "preview" ? 30.0 : blueprint.targetDurationSeconds}};
    db_.createTask(task); pending_.push_back({task.at("id"), std::move(snapshot)}); condition_.notify_one(); return task;
}
bool GenerationQueue::pause(std::chrono::milliseconds duration) {
    std::unique_lock lock(mutex_); return condition_.wait_for(lock, duration, [this] { return stopping_.load(); });
}
void GenerationQueue::run() {
    for (;;) {
        Job job;
        { std::unique_lock lock(mutex_); condition_.wait(lock, [this] { return stopping_ || !pending_.empty(); });
          if (stopping_) return;
          job = std::move(pending_.front()); pending_.pop_front(); }
        const auto& id = job.id;
        std::filesystem::path destination, original, staging;
        auto fail = [&](const std::string& message) {
            std::error_code error;
            // Preserve a paid model result when local conversion fails; only discard the partial final file.
            if (!destination.empty()) std::filesystem::remove(destination, error);
            try { db_.updateTask(id, "failed", 0, message); } catch (...) {}
        };
        try {
            const auto task = db_.task(id);
            db_.updateTask(id, "preparing", 10);
            if (pause(std::chrono::milliseconds(200))) throw ProviderError("cancelled", "程序关闭，任务已停止；未自动重新生成");
            staging = data_ / "staging" / id;
            std::unique_ptr<IMusicModelProvider> live;
            IMusicModelProvider* provider = provider_.get();
            if (job.settings.provider == "lyria") { live = std::make_unique<LyriaProvider>(job.settings, staging, &stopping_); provider = live.get(); }
            if (job.settings.provider == "songapi") { live = std::make_unique<SongApiProvider>(job.settings, staging, &stopping_); provider = live.get(); }
            if (job.settings.provider == "elevenlabs") { live = std::make_unique<ElevenLabsProvider>(job.settings, staging, &stopping_); provider = live.get(); }
            auto blueprint = task.at("blueprint").get<MusicBlueprint>(); blueprint.finalLyriaPrompt = task.at("generationPrompt");
            db_.updateTask(id, "generating", 20);
            auto remote = provider->generate({blueprint, task.at("mode")});
            GenerationStatus status;
            const auto deadline = std::chrono::steady_clock::now() + std::chrono::minutes(15);
            do {
                status = provider->queryTask(remote.id);
                if (status.state == "failed") throw ProviderError("generation_failed", "音乐生成失败，请检查模型设置");
                if (status.state == "completed") break;
                db_.updateTask(id, "generating", status.progress);
                if (pause(std::chrono::milliseconds(live ? 3000 : 180))) throw ProviderError("cancelled", "程序关闭，任务已停止；未自动重新生成");
                if (std::chrono::steady_clock::now() > deadline) throw ProviderError("timeout", "音乐任务超过等待上限；请在服务商账户确认结果后再决定是否重新生成");
            } while (true);
            db_.updateTask(id, "post_processing", 90);
            if (pause(std::chrono::milliseconds(200))) throw ProviderError("cancelled", "程序关闭，音频处理已停止");
            const std::string projectId = task.at("projectId"), format = task.at("format");
            if (!isUuid(projectId) || !isUuid(id)) throw ValidationError("存储标识符无效");
            auto directory = data_ / "projects" / projectId;
            std::filesystem::create_directories(directory);
            destination = directory / (id + "." + format);
            original = directory / (id + ".source." + status.format);
            std::filesystem::copy_file(status.audioPath, original, std::filesystem::copy_options::none);
            auto processed = processAudio(original, destination, status.format, format, blueprint.strictDuration, task.at("modelTargetDurationSeconds"), &stopping_);
            json version = {{"id", uuid()}, {"projectId", projectId}, {"generationId", id}, {"parentVersionId", task.value("parentVersionId", json(nullptr))},
                {"title", task["blueprint"]["title"]}, {"blueprint", task["blueprint"]}, {"mode", task["mode"]},
                {"provider", task["provider"]}, {"model", task["model"]}, {"generationPrompt", task["generationPrompt"]},
                {"format", format}, {"sourceFormat", status.format}, {"requestedFormat", task["requestedFormat"]},
                {"actualDurationSeconds", processed.actualDurationSeconds}, {"sourceDurationSeconds", processed.sourceDurationSeconds},
                {"modelTargetDurationSeconds", task["modelTargetDurationSeconds"]}, {"strictDurationApplied", blueprint.strictDuration},
                {"songText", status.songText}, {"songStructure", status.songStructure},
                {"rating", 0}, {"notes", ""}, {"createdAt", timestamp()}};
            db_.completeTask(id, version);
        } catch (const ProviderError& error) { fail(error.what()); }
        catch (...) { fail("音乐生成或保存失败，请检查音频资源、数据目录权限及磁盘空间"); }
        if (!staging.empty()) { std::error_code error; std::filesystem::remove_all(staging, error); }
    }
}
}
