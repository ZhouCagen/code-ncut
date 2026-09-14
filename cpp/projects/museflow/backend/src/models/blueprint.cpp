#include "museflow/models.hpp"
#include "museflow/schema.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>

namespace museflow {
static void require(bool condition, const std::string& message) {
    if (!condition) throw ValidationError(message);
}
static void strings(const std::vector<std::string>& values, const std::string& field) {
    require(values.size() <= 32, field + "最多包含 32 项");
    for (const auto& value : values) require(!value.empty() && value.size() <= 200, field + "每项应为 1～200 字节");
}
void validate(const MusicBlueprint& b) {
    require(!b.title.empty() && b.title.size() <= 300, "标题不能为空，且不能超过 300 字节");
    require(!b.sceneDescription.empty() && b.sceneDescription.size() <= 12000, "场景描述不能为空，且不能超过 12000 字节");
    require(std::set<std::string>{"game", "video", "animation", "story", "song", "other"}.contains(b.purpose), "用途无效");
    require(!b.genre.empty() && b.genre.size() <= 200, "请填写有效曲风");
    require(b.bpm >= 40 && b.bpm <= 240, "BPM 必须在 40～240 之间");
    require(std::isfinite(b.targetDurationSeconds) && b.targetDurationSeconds >= 5 && b.targetDurationSeconds <= 600, "目标时长必须在 5～600 秒之间");
    require(b.vocalMode == "instrumental" || b.vocalMode == "vocal", "人声模式无效");
    strings(b.mood, "情绪"); strings(b.instruments, "乐器");
    require(b.lyrics.size() <= 20000 && b.negativePrompt.size() <= 4000, "歌词或负面提示词过长");
    require(b.key.size() <= 100 && b.subGenre.size() <= 200 && b.vocalGender.size() <= 100 && b.language.size() <= 100, "音乐参数文本过长");
    require(!b.structure.empty() && b.structure.size() <= 32, "时间轴必须包含 1～32 个段落");
    for (const auto& s : b.structure) require(std::isfinite(s.startSeconds) && std::isfinite(s.endSeconds), "时间轴起止时间必须是有限数字");
    auto sections = b.structure;
    std::sort(sections.begin(), sections.end(), [](const auto& a, const auto& c) { return a.startSeconds < c.startSeconds; });
    double previousEnd = 0;
    std::set<std::string> ids;
    for (const auto& s : sections) {
        require(!s.id.empty() && s.id.size() <= 100 && ids.insert(s.id).second, "时间轴段落 ID 必须唯一且不能为空");
        require(!s.name.empty() && s.name.size() <= 200, "段落名称不能为空，且不能过长");
        require(s.emotion.size() <= 200 && s.description.size() <= 2000, "段落描述过长");
        require(std::isfinite(s.startSeconds) && std::isfinite(s.endSeconds) && s.startSeconds >= 0 && s.startSeconds < s.endSeconds, "段落开始时间必须小于结束时间，且不能为负数");
        require(s.startSeconds >= previousEnd, "时间轴段落不能重叠");
        require(s.endSeconds <= b.targetDurationSeconds, "段落结束时间不能超过目标时长");
        require(s.intensity >= 1 && s.intensity <= 10, "情绪强度必须在 1～10 之间");
        strings(s.instruments, "段落乐器");
        previousEnd = s.endSeconds;
    }
}
MusicBlueprint parseBlueprint(const json& value) {
    try {
        require(value.is_object(), "音乐设计稿必须是 JSON 对象");
        for (const auto* field : {"title", "sceneDescription", "purpose", "genre", "mood", "instruments", "vocalMode", "targetDurationSeconds", "strictDuration", "structure"})
            require(value.contains(field), std::string("音乐设计稿缺少字段：") + field);
        if (value.contains("bpm")) {
            require(value["bpm"].is_number_integer(), "BPM 必须是整数");
            require(value["bpm"].get<double>() >= 40 && value["bpm"].get<double>() <= 240, "BPM 必须在 40～240 之间");
        }
        require(value["structure"].is_array(), "时间轴必须是数组");
        for (const auto& section : value["structure"]) {
            for (const auto* field : {"id", "name", "startSeconds", "endSeconds", "emotion", "intensity", "instruments", "description"})
                require(section.is_object() && section.contains(field), std::string("时间轴缺少字段：") + field);
            require(section["intensity"].is_number_integer(), "情绪强度必须是整数");
            require(section["intensity"].get<double>() >= 1 && section["intensity"].get<double>() <= 10, "情绪强度必须在 1～10 之间");
        }
        auto blueprint = value.get<MusicBlueprint>();
        validate(blueprint);
        if (!promptIsCurrent(blueprint)) { blueprint.finalLyriaPrompt = composePrompt(blueprint); stampPrompt(blueprint, "local"); }
        return blueprint;
    } catch (const json::exception&) { throw ValidationError("音乐设计稿字段类型错误，请检查数字、文本和数组"); }
}
std::string composePrompt(const MusicBlueprint& b) {
    std::ostringstream out;
    out << "Title: " << b.title << "\nScene: " << b.sceneDescription << "\nPurpose: " << b.purpose
        << "\nGenre: " << b.genre << " / " << b.subGenre << "\nTempo: " << b.bpm << " BPM; key: " << b.key
        << "\nTarget duration: approximately " << b.targetDurationSeconds << " seconds.\nMood: ";
    for (const auto& mood : b.mood) out << mood << "; ";
    out << "\nInstruments: "; for (const auto& instrument : b.instruments) out << instrument << "; ";
    out << "\nVocals: " << b.vocalMode;
    if (b.vocalMode == "vocal") out << ", " << b.vocalGender << ", " << b.language << "\nLyrics:\n" << b.lyrics;
    auto sections = b.structure;
    std::sort(sections.begin(), sections.end(), [](const auto& a, const auto& c) { return a.startSeconds < c.startSeconds; });
    for (const auto& s : sections) {
        out << "\n[" << s.startSeconds << "s–" << s.endSeconds << "s] " << s.name << ": " << s.emotion << ", intensity " << s.intensity << "/10. " << s.description << " Instruments: ";
        for (const auto& instrument : s.instruments) out << instrument << "; ";
    }
    out << "\nAvoid: " << b.negativePrompt;
    return out.str();
}
}
