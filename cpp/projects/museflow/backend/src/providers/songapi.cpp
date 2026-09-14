#include "museflow/providers.hpp"
#include "museflow/runtime.hpp"
#include <algorithm>
#include <fstream>

namespace museflow {
namespace {
bool safeId(const std::string& id) {
    return !id.empty() && id.size() <= 256 && std::all_of(id.begin(), id.end(), [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
    });
}
// Count UTF-16 units, matching the API's string limits; never truncate user lyrics silently.
size_t characters(const std::string& text) {
    size_t count = 0;
    for (unsigned char c : text) if ((c & 0xc0) != 0x80) count += c >= 0xf0 ? 2 : 1;
    return count;
}
}
SongApiProvider::SongApiProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled)
    : settings_(std::move(settings)), directory_(std::move(directory)), cancelled_(cancelled) {}
TestResult SongApiProvider::testConnection() {
    auto response = requestSongApiJson("/v1/generations?limit=1", settings_.apiKey, nullptr, {10, 30, cancelled_});
    if (!response.is_object() || response.value("object", "") != "list" || !response.contains("data") || !response["data"].is_array())
        throw ProviderError("invalid_response", "SongAPI 未返回有效记录列表，请检查服务状态");
    return {true, settings_.apiKey.starts_with("sk_test_")
        ? "SongAPI 测试 Key 验证成功；生成只返回静音示例，不扣积分。此检查未生成音乐。"
        : "SongAPI 连接与 Key 验证成功；此检查未生成音乐、不扣生成积分，余额和模型权限以实际生成为准。"};
}
GenerationTask SongApiProvider::generate(const MusicGenerateRequest& request) {
    const auto& b = request.blueprint;
    if (characters(b.finalLyriaPrompt) > 3000) throw ProviderError("prompt_too_long", "SongAPI 音乐提示词最多 3000 字符，请缩短场景或结构描述", 400);
    if (characters(b.negativePrompt) > 500) throw ProviderError("prompt_too_long", "SongAPI 避免元素最多 500 字符，请缩短内容", 400);
    if (b.vocalMode != "instrumental" && characters(b.lyrics) > 5000) throw ProviderError("lyrics_too_long", "SongAPI 歌词最多 5000 字符，请缩短歌词", 400);
    json body{{"prompt", b.finalLyriaPrompt}, {"title", b.title}, {"model", settings_.model},
        {"instrumental", b.vocalMode == "instrumental"}, {"negative_tags", b.negativePrompt}};
    if (b.vocalMode != "instrumental" && !b.lyrics.empty()) body["lyrics"] = b.lyrics;
    auto response = requestSongApiJson("/v1/generate", settings_.apiKey, &body, {10, 60, cancelled_});
    try {
        id_ = response.at("id").get<std::string>();
        if (!safeId(id_)) throw ProviderError("invalid_response", "SongAPI 未返回有效任务 ID；请在控制台确认任务后再决定是否重试");
        // Always poll the accepted id, even when submission already reports completion.
        return {id_};
    } catch (const json::exception&) { throw ProviderError("invalid_response", "SongAPI 返回的任务结构无效；请在控制台确认任务后再决定是否重试"); }
}
GenerationStatus SongApiProvider::queryTask(const std::string& id) {
    if (id != id_) throw ProviderError("invalid_task", "SongAPI 任务 ID 不匹配");
    if (status_.state == "completed") return status_;
    const auto response = requestSongApiJson("/v1/generations/" + id, settings_.apiKey, nullptr, {10, 60, cancelled_});
    try {
        if (response.at("id") != id) throw ProviderError("invalid_response", "SongAPI 返回的任务 ID 不匹配");
        const auto state = response.at("status").get<std::string>();
        if (state == "queued" || state == "processing") return {"generating", state == "queued" ? 25 : 50, "", 0};
        if (state == "failed") throw ProviderError("generation_failed", "SongAPI 音乐任务失败，请在控制台查看原因及积分退还情况；未自动重新生成");
        if (state != "complete") throw ProviderError("invalid_response", "SongAPI 返回了未知任务状态");
        std::string address;
        if (response.contains("audio_url") && response["audio_url"].is_string()) address = response["audio_url"];
        const json* clip = nullptr;
        if (response.contains("clips") && response["clips"].is_array() && !response["clips"].empty()) {
            clip = &response["clips"][0];
            if (address.empty() && clip->contains("audio_url") && (*clip)["audio_url"].is_string()) address = (*clip)["audio_url"];
        }
        if (address.empty()) throw ProviderError("missing_audio", "SongAPI 任务已完成，但未返回音频地址");
        const auto bytes = downloadSongApiAudio(address, {10, 120, cancelled_});
        if (bytes.empty()) throw ProviderError("invalid_audio", "SongAPI 返回了空音频");
        GenerationStatus result{"completed", 80, "", 0};
        result.format = bytes.starts_with("RIFF") ? "wav" : "mp3";
        if (clip && clip->contains("lyrics") && (*clip)["lyrics"].is_string()) result.songText.push_back((*clip)["lyrics"]);
        std::filesystem::create_directories(directory_);
        const auto path = directory_ / (uuid() + "." + result.format);
        std::ofstream output(path, std::ios::binary); output.write(bytes.data(), bytes.size()); output.close();
        if (!output) { std::error_code error; std::filesystem::remove(path, error); throw ProviderError("audio_write", "无法保存 SongAPI 音频，请检查磁盘空间及目录权限"); }
        result.audioPath = path.string();
        status_ = result;
        return status_;
    } catch (const json::exception&) { throw ProviderError("invalid_response", "SongAPI 任务结果格式无效"); }
}
}
