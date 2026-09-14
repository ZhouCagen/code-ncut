#include "museflow/providers.hpp"
#include "museflow/runtime.hpp"
#include <fstream>
#include <algorithm>

namespace museflow {
namespace {
std::vector<unsigned char> decode(const std::string& input) {
    auto fail = [] { throw ProviderError("invalid_audio", "Lyria 音频 Base64 无效或超过 128 MiB 限制"); };
    if (input.empty() || input.size() % 4 || input.size() / 4 * 3 > 128 * 1024 * 1024) fail();
    const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<unsigned char> result; result.reserve(input.size() / 4 * 3);
    for (size_t i = 0; i < input.size(); i += 4) {
        unsigned values[4]{}; int padding = 0;
        for (int k = 0; k < 4; ++k) {
            char c = input[i + k];
            if (c == '=') { if (k < 2 || i + 4 != input.size()) fail(); ++padding; }
            else { if (padding) fail(); auto p = alphabet.find(c); if (p == std::string::npos) fail(); values[k] = p; }
        }
        if ((padding == 2 && (values[1] & 15)) || (padding == 1 && (values[2] & 3))) fail();
        result.push_back((values[0] << 2) | (values[1] >> 4));
        if (padding < 2) result.push_back((values[1] << 4) | (values[2] >> 2));
        if (!padding) result.push_back((values[2] << 6) | values[3]);
    }
    return result;
}
bool safeId(const std::string& id) {
    return !id.empty() && id.size() <= 512 && std::all_of(id.begin(), id.end(), [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-';
    });
}
}
LyriaProvider::LyriaProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled)
    : settings_(std::move(settings)), directory_(std::move(directory)), cancelled_(cancelled) {}
TestResult LyriaProvider::testConnection() {
    auto value = requestGoogleJson("/models/" + settings_.model, settings_.apiKey, nullptr, {10, 30, cancelled_});
    if (!value.is_object() || value.value("name", "") != "models/" + settings_.model)
        throw ProviderError("invalid_response", "Google 未返回所选模型信息，请检查模型权限");
    return {true, "Google 密钥与模型查询成功；此检查未生成音乐，生成权限仍以实际任务为准"};
}
GenerationTask LyriaProvider::generate(const MusicGenerateRequest& request) {
    json body{{"model", request.mode == "preview" ? "lyria-3-clip-preview" : "lyria-3.5"}, {"input", request.blueprint.finalLyriaPrompt}};
    // Official Lyria guide: only full Lyria 3.5 accepts this WAV response configuration.
    if (request.mode == "full" && settings_.outputFormat == "wav") body["response_format"] = {{"type", "audio"}};
    auto response = requestGoogleJson("/interactions", settings_.apiKey, &body, {10, 600, cancelled_});
    try {
        id_ = response.value("id", "");
        status_ = parse(response);
        if (status_.state != "completed" && !safeId(id_)) throw ProviderError("invalid_response", "Lyria 未返回可查询的任务 ID");
        if (id_.empty()) id_ = uuid();
        return {id_};
    } catch (const json::exception&) { throw ProviderError("invalid_response", "Lyria 响应结构无效"); }
}
GenerationStatus LyriaProvider::queryTask(const std::string& id) {
    if (id != id_) throw ProviderError("invalid_task", "Lyria 任务 ID 不匹配");
    if (status_.state == "completed") return status_;
    try { status_ = parse(requestGoogleJson("/interactions/" + id, settings_.apiKey, nullptr, {10, 60, cancelled_})); }
    catch (const json::exception&) { throw ProviderError("invalid_response", "Lyria 任务响应结构无效"); }
    return status_;
}
GenerationStatus LyriaProvider::parse(const json& response) {
    const auto state = response.value("status", "");
    if (state == "in_progress" || state == "queued") return {"generating", 40, "", 0};
    if (state != "completed") throw ProviderError("model_refusal", "Lyria 未完成生成或拒绝了请求，请调整描述并检查账户；本次未自动重试");
    GenerationStatus result{"completed", 80, "", 0};
    const json* audio = nullptr;
    if (!response.contains("steps") || !response["steps"].is_array()) throw ProviderError("invalid_response", "Lyria 响应缺少 steps");
    size_t textSize = 0;
    for (const auto& step : response["steps"]) {
        if (!step.is_object() || step.value("type", "") != "model_output") continue;
        if (!step.contains("content") || !step["content"].is_array()) continue;
        for (const auto& content : step["content"]) {
            const auto type = content.value("type", "");
            if (type == "audio") audio = &content;
            if (type == "text") {
                auto text = content.at("text").get<std::string>(); textSize += text.size();
                if (textSize > 256 * 1024) throw ProviderError("invalid_response", "Lyria 歌词及结构文本超过大小限制");
                result.songText.push_back(text);
                try {
                    auto structure = parseProviderJson(text);
                    if (containsSecret(structure, settings_.apiKey)) throw ProviderError("unsafe_response", "模型响应包含敏感凭据，已丢弃响应");
                    if (structure.is_object() || structure.is_array()) result.songStructure.push_back(structure);
                } catch (const json::exception&) { /* Lyrics may be ordinary text. Preserve it verbatim. */ }
            }
        }
    }
    if (!audio) throw ProviderError("missing_audio", "Lyria 已结束但未返回音频，可能触发内容限制；请调整描述");
    const auto mime = audio->value("mime_type", "");
    if (mime == "audio/mpeg" || mime == "audio/mp3") result.format = "mp3";
    else if (mime == "audio/wav" || mime == "audio/x-wav") result.format = "wav";
    else throw ProviderError("unsupported_audio", "Lyria 返回了不支持的音频 MIME 类型");
    auto bytes = decode(audio->at("data").get_ref<const std::string&>());
    std::filesystem::create_directories(directory_);
    auto path = directory_ / (uuid() + "." + result.format);
    std::ofstream output(path, std::ios::binary); output.write(reinterpret_cast<const char*>(bytes.data()), bytes.size()); output.close();
    if (!output) { std::error_code error; std::filesystem::remove(path, error); throw ProviderError("audio_write", "无法保存 Lyria 音频，请检查磁盘空间及目录权限"); }
    result.audioPath = path.string();
    return result;
}
}
