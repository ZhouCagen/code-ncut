#include "museflow/settings.hpp"
#include "museflow/runtime.hpp"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <set>

namespace museflow {
namespace {
void validKey(const std::string& key) {
    if (key.size() > 8192 || std::any_of(key.begin(), key.end(), [](unsigned char c) { return c < 33 || c > 126; })) throw ValidationError("API Key 格式无效，请勿包含空格或换行");
}
json persisted(const TextSettings& text, const json& music) {
    return {{"version", 1}, {"text", {{"provider", text.provider}, {"baseUrl", text.baseUrl}, {"model", text.model},
        {"connectTimeoutSeconds", text.connectTimeoutSeconds}, {"requestTimeoutSeconds", text.requestTimeoutSeconds}}}, {"music", music}};
}
void applyPreferences(TextSettings& text, json& music, const json& patch) {
    if (!patch.is_object()) throw ValidationError("模型设置必须是 JSON 对象");
    if (patch.contains("text")) {
        const auto& value = patch.at("text");
        if (!value.is_object()) throw ValidationError("创作助手设置必须是对象");
        text.provider = value.value("provider", text.provider); text.baseUrl = value.value("baseUrl", text.baseUrl); text.model = value.value("model", text.model);
        for (const auto* field : {"connectTimeoutSeconds", "requestTimeoutSeconds"})
            if (value.contains(field) && !value[field].is_number_integer()) throw ValidationError("超时秒数必须是整数");
        text.connectTimeoutSeconds = value.value("connectTimeoutSeconds", text.connectTimeoutSeconds);
        text.requestTimeoutSeconds = value.value("requestTimeoutSeconds", text.requestTimeoutSeconds);
    }
    if (text.provider != "mock" && text.provider != "deepseek") throw ValidationError("创作助手服务商无效");
    normalizeTextEndpoint(text.baseUrl);
    if (text.model.empty() || text.model.size() > 160 || std::any_of(text.model.begin(), text.model.end(), [](unsigned char c) { return c <= 32 || c == 127; })) throw ValidationError("模型名称不能为空、过长或包含空白字符");
    if (text.connectTimeoutSeconds < 1 || text.connectTimeoutSeconds > 30 || text.requestTimeoutSeconds < 2 || text.requestTimeoutSeconds > 180 || text.connectTimeoutSeconds > text.requestTimeoutSeconds) throw ValidationError("连接超时应为 1～30 秒，响应超时应为 2～180 秒，且不能小于连接超时");
    if (patch.contains("music")) {
        const auto& value = patch.at("music");
        if (!value.is_object()) throw ValidationError("音乐模型设置必须是对象");
        for (const auto* field : {"provider", "model", "outputFormat", "previewFirst", "songapiModel"}) if (value.contains(field)) music[field] = value[field];
    }
    if (music["provider"] != "mock" && music["provider"] != "lyria" && music["provider"] != "songapi" && music["provider"] != "elevenlabs") throw ValidationError("音乐服务商选项无效");
    if (music["model"] != "lyria-3-clip-preview" && music["model"] != "lyria-3.5") throw ValidationError("音乐模型选项无效");
    if (!music["songapiModel"].is_string() || !std::set<std::string>{"v3.5", "v4", "v4.5", "v4.5-plus", "v4.5-all", "v5", "v5.5"}.contains(music["songapiModel"].get<std::string>())) throw ValidationError("SongAPI 模型选项无效");
    if (music["outputFormat"] != "wav" && music["outputFormat"] != "mp3") throw ValidationError("输出格式必须是 WAV 或 MP3");
    if (!music["previewFirst"].is_boolean()) throw ValidationError("试听优先必须是布尔值");
}
void atomicWrite(const std::filesystem::path& directory, const json& value) {
    auto temporary = directory / (uuid() + ".tmp");
    const auto payload = value.dump(2);
    int descriptor = open(temporary.c_str(), O_CREAT | O_EXCL | O_WRONLY | O_CLOEXEC | O_NOFOLLOW, 0600);
    if (descriptor < 0) throw std::runtime_error("无法保存模型设置");
    try {
        size_t offset = 0;
        while (offset < payload.size()) {
            auto count = write(descriptor, payload.data() + offset, payload.size() - offset);
            if (count < 0 && errno == EINTR) continue;
            if (count <= 0) throw std::runtime_error("无法保存模型设置");
            offset += static_cast<size_t>(count);
        }
        if (fsync(descriptor) != 0) throw std::runtime_error("无法保存模型设置");
        close(descriptor); descriptor = -1;
        std::filesystem::rename(temporary, directory / "settings.json");
    } catch (...) {
        if (descriptor >= 0) close(descriptor);
        std::error_code error; std::filesystem::remove(temporary, error); throw;
    }
}
}
SettingsService::SettingsService(std::filesystem::path directory) : directory_(std::move(directory)) {
    if (const auto* value = std::getenv("DEEPSEEK_API_KEY")) textEnvironmentKey_ = value;
    if (const auto* value = std::getenv("GEMINI_API_KEY")) musicEnvironmentKey_ = value;
    if (const auto* value = std::getenv("SONGAPI_KEY")) songapiEnvironmentKey_ = value;
    if (const auto* value = std::getenv("ELEVENLABS_API_KEY")) elevenlabsEnvironmentKey_ = value;
    const auto file = directory_ / "settings.json";
    if (std::filesystem::exists(file)) {
        try {
            if (std::filesystem::file_size(file) > 16384) throw ValidationError("设置文件过大");
            std::ifstream input(file); auto value = json::parse(input);
            applyPreferences(text_, music_, value); // API Key properties on disk are intentionally ignored.
        } catch (...) { throw std::runtime_error("模型设置文件无效，请检查 XDG 配置目录中的 settings.json"); }
    }
}
json SettingsService::publicLocked() const {
    auto source = [](const std::string& session, const std::string& environment, bool suppressed) { return !session.empty() ? "session" : (!suppressed && !environment.empty() ? "environment" : "none"); };
    const std::string textSource = source(text_.apiKey, textEnvironmentKey_, suppressTextEnvironment_);
    const bool songapi = music_["provider"] == "songapi";
    const std::string musicSource = music_["provider"] == "elevenlabs" ? source(elevenlabsSessionKey_, elevenlabsEnvironmentKey_, suppressElevenlabsEnvironment_) : songapi ? source(songapiSessionKey_, songapiEnvironmentKey_, suppressSongapiEnvironment_) : source(musicSessionKey_, musicEnvironmentKey_, suppressMusicEnvironment_);
    auto result = persisted(text_, music_);
    result["mode"] = text_.provider == "mock" && music_["provider"] == "mock" ? "mock" : (text_.provider != "mock" && music_["provider"] != "mock" ? "live" : "mixed");
    result["text"]["endpoint"] = normalizeTextEndpoint(text_.baseUrl);
    result["text"]["hasApiKey"] = textSource != "none"; result["text"]["keySource"] = textSource;
    result["text"]["status"] = status_; result["text"]["message"] = message_;
    result["music"]["activeModel"] = music_["provider"] == "mock" ? json("MockMusicProvider") : (music_["provider"] == "elevenlabs" ? json("music_v1") : songapi ? music_["songapiModel"] : music_["model"]);
    result["music"]["activeOutputFormat"] = music_["outputFormat"]; result["music"]["status"] = musicStatus_;
    result["music"]["message"] = musicMessage_;
    result["music"]["hasApiKey"] = musicSource != "none"; result["music"]["keySource"] = musicSource;
    const auto songapiKey = !songapiSessionKey_.empty() ? songapiSessionKey_ : (suppressSongapiEnvironment_ ? "" : songapiEnvironmentKey_);
    result["music"]["songapiSandbox"] = songapiKey.starts_with("sk_test_");
    result["music"]["songapiKeySource"] = source(songapiSessionKey_, songapiEnvironmentKey_, suppressSongapiEnvironment_);
    result["music"]["elevenlabsKeySource"] = source(elevenlabsSessionKey_, elevenlabsEnvironmentKey_, suppressElevenlabsEnvironment_);
    result["music"]["lyriaKeySource"] = source(musicSessionKey_, musicEnvironmentKey_, suppressMusicEnvironment_);
    result["capabilities"] = {{"realTextProvider", true}, {"realMusicProvider", true}, {"strictDuration", true}, {"branching", true}};
    return result;
}
json SettingsService::publicSettings() { std::lock_guard lock(mutex_); return publicLocked(); }
TextSettings SettingsService::textSnapshot() {
    std::lock_guard lock(mutex_); auto result = text_;
    if (result.apiKey.empty() && !suppressTextEnvironment_) result.apiKey = textEnvironmentKey_;
    return result;
}
MusicSettings SettingsService::musicSnapshot() {
    std::lock_guard lock(mutex_);
    if (music_["provider"] == "elevenlabs") return {"elevenlabs", "music_v1", music_["outputFormat"],
        !elevenlabsSessionKey_.empty() ? elevenlabsSessionKey_ : (suppressElevenlabsEnvironment_ ? "" : elevenlabsEnvironmentKey_), musicRevision_};
    if (music_["provider"] == "songapi") return {"songapi", music_["songapiModel"], music_["outputFormat"],
        !songapiSessionKey_.empty() ? songapiSessionKey_ : (suppressSongapiEnvironment_ ? "" : songapiEnvironmentKey_), musicRevision_};
    return {music_["provider"], music_["model"], music_["outputFormat"],
        !musicSessionKey_.empty() ? musicSessionKey_ : (suppressMusicEnvironment_ ? "" : musicEnvironmentKey_), musicRevision_};
}
void SettingsService::recordMusicTest(unsigned long revision, const TestResult& result) {
    std::lock_guard lock(mutex_);
    if (revision != musicRevision_) return;
    musicStatus_ = result.ok ? "connected" : "error"; musicMessage_ = result.message;
}
void SettingsService::recordTextTest(unsigned long revision, const TestResult& result) {
    std::lock_guard lock(mutex_);
    if (revision != text_.revision) return;
    status_ = result.ok ? "connected" : "error"; message_ = result.message;
}
json SettingsService::update(const json& patch) {
    std::lock_guard lock(mutex_);
    auto text = text_; auto music = music_; auto musicKey = musicSessionKey_; auto songapiKey = songapiSessionKey_;
    auto elevenlabsKey = elevenlabsSessionKey_;
    bool elevenlabsSuppressed = suppressElevenlabsEnvironment_;
    bool songapiSuppressed = suppressSongapiEnvironment_;
    bool textSuppressed = suppressTextEnvironment_, musicSuppressed = suppressMusicEnvironment_;
    applyPreferences(text, music, patch);
    auto setKey = [](const json& value, std::string& key, bool& suppressed) {
        if (value.value("clearApiKey", false)) { key.clear(); suppressed = true; }
        if (value.value("useEnvironmentKey", false)) { key.clear(); suppressed = false; }
        if (value.contains("apiKey")) { key = value.at("apiKey").get<std::string>(); validKey(key); suppressed = key.empty(); }
    };
    if (patch.contains("text")) setKey(patch["text"], text.apiKey, textSuppressed);
    if (patch.contains("music")) {
        if (music["provider"] == "elevenlabs") setKey(patch["music"], elevenlabsKey, elevenlabsSuppressed);
        else if (music["provider"] == "songapi") setKey(patch["music"], songapiKey, songapiSuppressed);
        else setKey(patch["music"], musicKey, musicSuppressed);
    }
    // Persist only non-secret preferences; apply memory keys only after the atomic write succeeds.
    atomicWrite(directory_, persisted(text, music));
    if (patch.contains("text")) { ++text.revision; status_ = "untested"; message_.clear(); }
    if (patch.contains("music")) { ++musicRevision_; musicStatus_ = "untested"; musicMessage_.clear(); }
    text_ = std::move(text); music_ = std::move(music); musicSessionKey_ = std::move(musicKey);
    elevenlabsSessionKey_ = std::move(elevenlabsKey); suppressElevenlabsEnvironment_ = elevenlabsSuppressed;
    songapiSessionKey_ = std::move(songapiKey); suppressSongapiEnvironment_ = songapiSuppressed;
    suppressTextEnvironment_ = textSuppressed; suppressMusicEnvironment_ = musicSuppressed;
    return publicLocked();
}
}
