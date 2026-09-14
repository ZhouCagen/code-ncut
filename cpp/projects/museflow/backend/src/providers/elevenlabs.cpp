#include "museflow/providers.hpp"
#include "museflow/runtime.hpp"
#include <cmath>
#include <fstream>

namespace museflow {
ElevenLabsProvider::ElevenLabsProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled)
    : settings_(std::move(settings)), directory_(std::move(directory)), cancelled_(cancelled) {}
TestResult ElevenLabsProvider::testConnection() {
    const json body{{"prompt", "A gentle solo piano instrumental."}, {"music_length_ms", 3000}, {"model_id", "music_v1"}};
    const auto content = requestElevenLabs("/v1/music/plan", settings_.apiKey, body, {10, 90, cancelled_});
    try {
        const auto plan = parseProviderJson(content);
        if (containsSecret(plan, settings_.apiKey)) throw ProviderError("unsafe_response", "模型响应包含敏感凭据，已丢弃响应");
        if (!plan.is_object() || !plan.contains("sections") || !plan["sections"].is_array() || plan["sections"].empty())
            throw ProviderError("invalid_response", "ElevenLabs 未返回有效的音乐编排，请检查服务状态");
    } catch (const json::exception&) { throw ProviderError("invalid_response", "ElevenLabs 音乐编排响应无效"); }
    return {true, "ElevenLabs Key 与音乐编排接口连接成功；此测试不生成音频、不消耗音乐积分，实际生成权限及额度以生成结果为准。"};
}
GenerationTask ElevenLabsProvider::generate(const MusicGenerateRequest& request) {
    const auto& prompt = request.blueprint.finalLyriaPrompt;
    size_t characters = 0;
    for (unsigned char c : prompt) if ((c & 0xc0) != 0x80) ++characters;
    if (prompt.empty() || characters > 4100) throw ProviderError("prompt_too_long", "ElevenLabs 音乐提示词（含歌词与时间轴）应为 1～4100 字符，请缩短内容", 400);
    const double seconds = request.mode == "preview" ? 30.0 : request.blueprint.targetDurationSeconds;
    if (!std::isfinite(seconds) || seconds < 3 || seconds > 600) throw ProviderError("invalid_duration", "ElevenLabs 音乐时长须为 3～600 秒", 400);
    const json body{{"prompt", prompt}, {"music_length_ms", std::llround(seconds * 1000)}, {"model_id", "music_v1"},
        {"force_instrumental", request.blueprint.vocalMode == "instrumental"}};
    const auto bytes = requestElevenLabs("/v1/music?output_format=mp3_44100_128", settings_.apiKey, body, {10, 600, cancelled_});
    const bool mp3 = bytes.starts_with("ID3") || (bytes.size() > 1 && static_cast<unsigned char>(bytes[0]) == 0xff && (static_cast<unsigned char>(bytes[1]) & 0xe0) == 0xe0);
    if (!mp3) throw ProviderError("invalid_audio", "ElevenLabs 未返回有效的 MP3 音频");
    std::filesystem::create_directories(directory_);
    id_ = uuid();
    const auto path = directory_ / (id_ + ".mp3");
    std::ofstream output(path, std::ios::binary); output.write(bytes.data(), bytes.size()); output.close();
    if (!output) { std::error_code error; std::filesystem::remove(path, error); throw ProviderError("audio_write", "无法保存 ElevenLabs 音频，请检查磁盘空间及目录权限"); }
    status_ = {"completed", 80, path.string(), 0}; status_.format = "mp3";
    return {id_};
}
GenerationStatus ElevenLabsProvider::queryTask(const std::string& id) {
    if (id.empty() || id != id_) throw ProviderError("invalid_task", "ElevenLabs 任务 ID 不匹配");
    return status_;
}
}
