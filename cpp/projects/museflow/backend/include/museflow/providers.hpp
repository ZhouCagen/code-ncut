#pragma once
#include "museflow/models.hpp"
#include "museflow/settings.hpp"
#include <chrono>
#include <filesystem>
#include <map>

namespace museflow {
class ITextModelProvider {
public:
    virtual ~ITextModelProvider() = default;
    virtual TestResult testConnection() = 0;
    virtual MusicBlueprint createBlueprint(const std::string& userInput) = 0;
    virtual MusicBlueprint reviseBlueprint(const MusicBlueprint& current, const std::string& instruction) = 0;
};
class IMusicModelProvider {
public:
    virtual ~IMusicModelProvider() = default;
    virtual TestResult testConnection() = 0;
    virtual GenerationTask generate(const MusicGenerateRequest& request) = 0;
    virtual GenerationStatus queryTask(const std::string& taskId) = 0;
};
class MockTextProvider final : public ITextModelProvider {
public:
    TestResult testConnection() override;
    MusicBlueprint createBlueprint(const std::string& userInput) override;
    MusicBlueprint reviseBlueprint(const MusicBlueprint& current, const std::string& instruction) override;
};
class DeepSeekProvider final : public ITextModelProvider {
public:
    explicit DeepSeekProvider(TextSettings settings, const std::atomic_bool* cancelled = nullptr);
    TestResult testConnection() override;
    MusicBlueprint createBlueprint(const std::string& userInput) override;
    MusicBlueprint reviseBlueprint(const MusicBlueprint& current, const std::string& instruction) override;
private:
    std::string completion(const json& messages, int maxTokens);
    MusicBlueprint design(const std::string& request);
    TextSettings settings_;
    const std::atomic_bool* cancelled_;
};
class LyriaProvider final : public IMusicModelProvider {
public:
    LyriaProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled = nullptr);
    TestResult testConnection() override;
    GenerationTask generate(const MusicGenerateRequest& request) override;
    GenerationStatus queryTask(const std::string& taskId) override;
private:
    GenerationStatus parse(const json& response);
    MusicSettings settings_;
    std::filesystem::path directory_;
    const std::atomic_bool* cancelled_;
    std::string id_;
    GenerationStatus status_;
};
class ElevenLabsProvider final : public IMusicModelProvider {
public:
    ElevenLabsProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled = nullptr);
    TestResult testConnection() override;
    GenerationTask generate(const MusicGenerateRequest& request) override;
    GenerationStatus queryTask(const std::string& taskId) override;
private:
    MusicSettings settings_;
    std::filesystem::path directory_;
    const std::atomic_bool* cancelled_;
    std::string id_;
    GenerationStatus status_;
};
class SongApiProvider final : public IMusicModelProvider {
public:
    SongApiProvider(MusicSettings settings, std::filesystem::path directory, const std::atomic_bool* cancelled = nullptr);
    TestResult testConnection() override;
    GenerationTask generate(const MusicGenerateRequest& request) override;
    GenerationStatus queryTask(const std::string& taskId) override;
private:
    MusicSettings settings_;
    std::filesystem::path directory_;
    const std::atomic_bool* cancelled_;
    std::string id_;
    GenerationStatus status_;
};
// Owned and called exclusively by the generation worker.
class MockMusicProvider final : public IMusicModelProvider {
public:
    explicit MockMusicProvider(std::filesystem::path audio);
    TestResult testConnection() override;
    GenerationTask generate(const MusicGenerateRequest& request) override;
    GenerationStatus queryTask(const std::string& taskId) override;
private:
    std::filesystem::path audio_;
    std::map<std::string, std::chrono::steady_clock::time_point> tasks_;
};
}
