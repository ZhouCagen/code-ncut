#pragma once
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace museflow {
using json = nlohmann::json;
struct ValidationError : std::runtime_error { using std::runtime_error::runtime_error; };
struct MusicSection {
    std::string id, name;
    double startSeconds = 0, endSeconds = 30;
    std::string emotion;
    int intensity = 5;
    std::vector<std::string> instruments;
    std::string description;
};
struct MusicBlueprint {
    std::string id, title, sceneDescription, purpose = "other";
    std::string genre, subGenre;
    std::vector<std::string> mood;
    int bpm = 90;
    std::string key;
    std::vector<std::string> instruments;
    std::string vocalMode = "instrumental", vocalGender, language, lyrics;
    double targetDurationSeconds = 60;
    bool strictDuration = false;
    std::vector<MusicSection> structure;
    std::string negativePrompt, finalLyriaPrompt;
    std::string promptSource = "local", promptFingerprint;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MusicSection, id, name, startSeconds, endSeconds, emotion, intensity, instruments, description)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(MusicBlueprint, id, title, sceneDescription, purpose, genre, subGenre, mood, bpm, key, instruments, vocalMode, vocalGender, language, lyrics, targetDurationSeconds, strictDuration, structure, negativePrompt, finalLyriaPrompt, promptSource, promptFingerprint)
void validate(const MusicBlueprint& blueprint);
MusicBlueprint parseBlueprint(const json& value);
std::string composePrompt(const MusicBlueprint& blueprint);
struct TestResult { bool ok; std::string message; };
struct MusicGenerateRequest { MusicBlueprint blueprint; std::string mode; };
struct GenerationTask { std::string id; };
struct GenerationStatus {
    std::string state;
    int progress = 0;
    std::string audioPath;
    double actualDurationSeconds = 0;
    std::string format = "wav";
    json songText = json::array();
    json songStructure = json::array();
};
}
