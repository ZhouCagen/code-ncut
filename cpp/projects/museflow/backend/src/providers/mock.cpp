#include "museflow/providers.hpp"
#include "museflow/runtime.hpp"

namespace museflow {
TestResult MockTextProvider::testConnection() { return {true, "模拟创作助手可用，不会调用外部模型"}; }
MusicBlueprint MockTextProvider::createBlueprint(const std::string& input) {
    if (input.empty() || input.size() > 12000) throw ValidationError("请输入场景描述（最多 12000 字节）");
    MusicBlueprint b;
    b.id = uuid(); b.title = "微光 · 场景配乐"; b.sceneDescription = input;
    b.genre = "氛围电影配乐"; b.subGenre = "Ambient / Cinematic";
    b.mood = {"静谧", "好奇", "希望"}; b.bpm = 88; b.key = "D minor";
    b.instruments = {"钢琴", "弦乐", "合成器铺底"}; b.language = "中文";
    b.structure = {
        {uuid(), "序章 · 微光", 0, 15, "静谧", 3, {"钢琴"}, "稀疏的钢琴音符进入，留出叙事空间"},
        {uuid(), "展开 · 远行", 15, 40, "好奇", 6, {"钢琴", "弦乐"}, "弦乐渐入，旋律逐渐展开"},
        {uuid(), "尾声 · 回响", 40, 60, "希望", 4, {"钢琴", "合成器铺底"}, "弦乐退出，留下温暖的尾音"}
    };
    b.negativePrompt = "刺耳高频、突然中断、过强鼓点";
    validate(b); b.finalLyriaPrompt = composePrompt(b); return b;
}
MusicBlueprint MockTextProvider::reviseBlueprint(const MusicBlueprint& current, const std::string& instruction) {
    if (instruction.empty() || instruction.size() > 2000) throw ValidationError("修改要求应为 1～2000 字节");
    auto result = current; result.id = uuid();
    result.sceneDescription += "\n补充要求（模拟记录）：" + instruction;
    validate(result); result.finalLyriaPrompt = composePrompt(result); return result;
}
MockMusicProvider::MockMusicProvider(std::filesystem::path audio) : audio_(std::move(audio)) {}
TestResult MockMusicProvider::testConnection() {
    return {std::filesystem::is_regular_file(audio_), "模拟音乐服务使用内置 8 秒 WAV 测试音频"};
}
GenerationTask MockMusicProvider::generate(const MusicGenerateRequest& request) {
    validate(request.blueprint);
    if (!testConnection().ok) throw std::runtime_error("内置测试音频不存在");
    auto id = uuid(); tasks_[id] = std::chrono::steady_clock::now(); return {id};
}
GenerationStatus MockMusicProvider::queryTask(const std::string& id) {
    auto it = tasks_.find(id);
    if (it == tasks_.end()) throw ValidationError("模拟任务不存在");
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - it->second).count();
    if (elapsed < 2400) return {"generating", 20 + static_cast<int>(elapsed * 55 / 2400), "", 0};
    tasks_.erase(it);
    return {"completed", 85, audio_.string(), 8.0};
}
}
