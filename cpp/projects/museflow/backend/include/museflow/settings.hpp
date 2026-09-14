#pragma once
#include "museflow/http_client.hpp"
#include <filesystem>
#include <mutex>
namespace museflow {
struct TextSettings {
    std::string provider = "mock", baseUrl = "https://api.deepseek.com", model = "deepseek-v4-flash";
    std::string apiKey;
    int connectTimeoutSeconds = 10, requestTimeoutSeconds = 90;
    unsigned long revision = 0;
};
struct MusicSettings {
    std::string provider = "mock", model = "lyria-3-clip-preview", outputFormat = "wav", apiKey;
    unsigned long revision = 0;
};
class SettingsService {
public:
    explicit SettingsService(std::filesystem::path configDirectory);
    json publicSettings();
    json update(const json& patch);
    TextSettings textSnapshot();
    MusicSettings musicSnapshot();
    void recordMusicTest(unsigned long revision, const TestResult& result);
    void recordTextTest(unsigned long revision, const TestResult& result);
private:
    json publicLocked() const;
    std::filesystem::path directory_;
    std::mutex mutex_;
    TextSettings text_;
    json music_ = {{"provider", "mock"}, {"model", "lyria-3-clip-preview"}, {"outputFormat", "wav"}, {"previewFirst", true}, {"songapiModel", "v5"}};
    std::string textEnvironmentKey_, musicEnvironmentKey_, musicSessionKey_, songapiEnvironmentKey_, songapiSessionKey_, elevenlabsEnvironmentKey_, elevenlabsSessionKey_;
    bool suppressTextEnvironment_ = false, suppressMusicEnvironment_ = false, suppressSongapiEnvironment_ = false, suppressElevenlabsEnvironment_ = false;
    unsigned long musicRevision_ = 0;
    std::string musicStatus_ = "untested", musicMessage_;
    std::string status_ = "untested", message_;
};
}
