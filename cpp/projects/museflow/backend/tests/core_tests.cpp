#include "museflow/generation_queue.hpp"
#include "museflow/runtime.hpp"
#include "museflow/schema.hpp"
#include "museflow/settings.hpp"
#include <fstream>
#include <iostream>
#include <set>

using namespace museflow;
static int checks = 0;
void check(bool condition, const std::string& name) {
    if (!condition) throw std::runtime_error("FAIL: " + name);
    ++checks;
}
template<class F> void invalid(F function, const std::string& name) {
    try { function(); } catch (const ValidationError&) { ++checks; return; }
    throw std::runtime_error("FAIL: accepted " + name);
}
int main() {
    const auto root = std::filesystem::temp_directory_path() / ("museflow-test-" + uuid());
    std::filesystem::create_directories(root);
    try {
        auto b = MockTextProvider{}.createBlueprint("旅人走进森林，情绪逐渐明亮");
        check(parseBlueprint(json(b)).title == b.title, "blueprint roundtrip");
        check(isUuid(b.id) && !isUuid("../../secret"), "UUID path guard");
        check(b.finalLyriaPrompt.find("0s–15s") != std::string::npos, "timed prompt");
        auto bad = b; bad.structure[1].startSeconds = 14;
        invalid([&] { validate(bad); }, "overlapping sections");
        bad = b; bad.structure[0].endSeconds = 0;
        invalid([&] { validate(bad); }, "empty section");
        bad = b; bad.structure.back().endSeconds = 61;
        invalid([&] { validate(bad); }, "section past target");
        bad = b; bad.structure[0].intensity = 11;
        invalid([&] { validate(bad); }, "intensity");
        bad = b; bad.bpm = 0;
        invalid([&] { validate(bad); }, "bpm");
        bad = b; bad.targetDurationSeconds = 601;
        invalid([&] { validate(bad); }, "duration");
        bad = b; bad.structure[1].id = bad.structure[0].id;
        invalid([&] { validate(bad); }, "duplicate section id");
        invalid([&] { parseBlueprint(json{{"title", "incomplete"}}); }, "missing required fields");
        json broken = b; broken["bpm"] = "fast";
        invalid([&] { parseBlueprint(broken); }, "wrong JSON type");
        broken = b; broken["structure"][0]["intensity"] = 1.5;
        invalid([&] { parseBlueprint(broken); }, "fractional intensity");
        broken = b; broken["bpm"] = 4294967384ULL;
        invalid([&] { parseBlueprint(broken); }, "integer overflow before bpm conversion");
        broken = b; broken["structure"][0]["intensity"] = 4294967299ULL;
        invalid([&] { parseBlueprint(broken); }, "integer overflow before intensity conversion");
        check(containsSecret(json{{"nested", json::array({"escaped-secret"})}}, "escaped-secret"), "decoded secret inspection");
        json extra = b; extra["apiKey"] = "test-not-a-key";
        check(!json(parseBlueprint(extra)).contains("apiKey"), "unknown secrets excluded");
        json modelOutput = b; modelOutput.erase("promptSource"); modelOutput.erase("promptFingerprint");
        modelOutput["finalLyriaPrompt"] = "AI-authored cinematic direction";
        auto generated = parseModelBlueprint(modelOutput);
        check(generated.promptSource == "deepseek" && promptIsCurrent(generated), "model prompt freshness");
        check(parseBlueprint(json(generated)).finalLyriaPrompt == "AI-authored cinematic direction", "preserve unedited AI prompt");
        generated.bpm = 105;
        check(parseBlueprint(json(generated)).finalLyriaPrompt.find("105 BPM") != std::string::npos, "manual changes regenerate stale prompt");
        auto invalidOutput = modelOutput; invalidOutput["extra-field"] = "ignored?";
        invalid([&] { parseModelBlueprint(invalidOutput); }, "schema additional property");
        invalidOutput = modelOutput; invalidOutput.erase("finalLyriaPrompt");
        invalid([&] { parseModelBlueprint(invalidOutput); }, "schema missing final prompt");
        invalidOutput = modelOutput; invalidOutput["strictDuration"] = "false";
        invalid([&] { parseModelBlueprint(invalidOutput); }, "schema bool type");
        invalidOutput = modelOutput; invalidOutput["lyrics"] = nullptr;
        invalid([&] { parseModelBlueprint(invalidOutput); }, "schema null string");
        check(normalizeTextEndpoint("https://api.example.com/v1/") == "https://api.example.com/v1/chat/completions", "base endpoint normalization");
        check(normalizeTextEndpoint("http://127.0.0.1:19876/v1/chat/completions") == "http://127.0.0.1:19876/v1/chat/completions", "full local endpoint");
        invalid([&] { normalizeTextEndpoint("http://remote.example/v1"); }, "remote plaintext endpoint");
        invalid([&] { normalizeTextEndpoint("https://name:password@example.com/v1"); }, "URL credentials");
        invalid([&] { normalizeTextEndpoint("https://example.com/v1?api_key=example"); }, "URL query keys");
        invalid([&] { normalizeTextEndpoint("file:///etc/passwd"); }, "local file protocol");
        setenv("DEEPSEEK_API_KEY", "fixture-environment-text", 1); setenv("GEMINI_API_KEY", "fixture-environment-music", 1);
        auto config = root / "config"; std::filesystem::create_directory(config);
        {
            SettingsService settings(config);
            check(settings.publicSettings()["text"]["keySource"] == "environment", "environment text key source");
            check(settings.publicSettings()["music"]["keySource"] == "environment", "environment music key source");
            auto saved = settings.update({{"text", {{"apiKey", "fixture-session-text"}, {"model", "custom-model"}}}, {"music", {{"apiKey", "fixture-session-music"}, {"outputFormat", "mp3"}}}});
            check(settings.textSnapshot().apiKey == "fixture-session-text", "session overrides environment");
            check(saved.dump().find("fixture-session") == std::string::npos, "public settings redact session keys");
            check(settings.musicSnapshot().apiKey == "fixture-session-music", "music session snapshot");
            auto musicSnapshot = settings.musicSnapshot();
            settings.update({{"music", {{"provider", "lyria"}}}});
            settings.recordMusicTest(musicSnapshot.revision, {true, "stale"});
            check(settings.publicSettings()["music"]["status"] == "untested", "stale music test ignored");
            check(settings.musicSnapshot().provider == "lyria" && settings.publicSettings()["mode"] == "mixed", "independent music provider");
            settings.update({{"music", {{"clearApiKey", true}}}});
            check(settings.musicSnapshot().apiKey.empty(), "music clear suppresses environment");
            settings.update({{"music", {{"useEnvironmentKey", true}}}});
            check(settings.musicSnapshot().apiKey == "fixture-environment-music", "restore music environment");
            invalid([&] { settings.update({{"music", {{"provider", "chat"}}}}); }, "reject generic music chat provider");
            auto snapshot = settings.textSnapshot();
            settings.update({{"text", {{"model", "new-model"}}}});
            settings.recordTextTest(snapshot.revision, {true, "stale"});
            check(settings.publicSettings()["text"]["status"] == "untested", "stale connection test ignored");
            settings.update({{"text", {{"clearApiKey", true}}}});
            check(settings.textSnapshot().apiKey.empty(), "clear also suppresses environment");
            settings.update({{"text", {{"useEnvironmentKey", true}}}});
            check(settings.textSnapshot().apiKey == "fixture-environment-text", "restore environment");
            invalid([&] { settings.update({{"text", {{"apiKey", "bad\nkey"}}}}); }, "header injection in key");
            invalid([&] { settings.update({{"text", {{"requestTimeoutSeconds", 0}}}}); }, "invalid timeout");
            std::ifstream stream(config / "settings.json"); std::string savedFile((std::istreambuf_iterator<char>(stream)), {});
            check(savedFile.find("fixture-") == std::string::npos && savedFile.find("apiKey") == std::string::npos, "no key persisted to config");
        }
        unsetenv("DEEPSEEK_API_KEY"); unsetenv("GEMINI_API_KEY");
        {
            SettingsService restarted(config);
            check(!restarted.publicSettings()["text"]["hasApiKey"].get<bool>() && !restarted.publicSettings()["music"]["hasApiKey"].get<bool>(), "session keys vanish after restart");
            check(restarted.publicSettings()["text"]["model"] == "new-model", "nonsecret preferences survive restart");
        }
        std::string projectId, firstTaskId;
        {
            Database db(root / "test.db");
            auto project = db.saveProject({{"name", "quote ' ; --"}, {"blueprint", b}, {"apiKey", "test-not-a-key"}});
            projectId = project["id"];
            check(db.projects().size() == 1 && db.project(projectId)["name"] == "quote ' ; --", "bound SQL parameters");
            check(!db.project(projectId).contains("apiKey"), "project property whitelist");
            {
                GenerationQueue queue(db, root, std::make_unique<MockMusicProvider>(std::filesystem::path(MUSEFLOW_ASSET_DIR) / "mock-demo.wav"));
                auto first = queue.enqueue(projectId, b, "preview"); firstTaskId = first["id"];
                check(first["status"] == "queued", "initial queued state");
                invalid([&] { db.deleteProject(projectId); }, "delete busy project");

                invalid([&] { queue.enqueue(projectId, b, "invented"); }, "invalid generation mode");
                b.bpm = 100;
                auto second = queue.enqueue(projectId, b, "full");
                std::set<std::string> states;
                auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(12);
                while (std::chrono::steady_clock::now() < deadline) {
                    states.insert(db.task(firstTaskId)["status"]);
                    if (db.task(second["id"])["status"] == "completed") break;
                    std::this_thread::sleep_for(std::chrono::milliseconds(30));
                }
                check(db.versions(projectId).size() == 2, "two immutable versions");
                for (const auto* state : {"preparing", "generating", "post_processing", "completed"}) check(states.contains(state), std::string("observed state ") + state);
                auto versions = db.versions(projectId);
                check(versions[0]["blueprint"]["bpm"] == 100 && versions[1]["blueprint"]["bpm"] == 88, "version snapshots preserved");
                check(db.task(firstTaskId)["modelTargetDurationSeconds"] == 30 && db.task(firstTaskId)["actualDurationSeconds"] == 8, "target versus actual duration");
                for (const auto& version : versions) check(std::filesystem::file_size(root / "projects" / projectId / (version["generationId"].get<std::string>() + ".wav")) > 44, "audio written");
            }
            auto parent = db.versions(projectId).back();
            auto branch = db.branchVersion(parent["id"]);
            check(branch["draftParentVersionId"] == parent["id"] && branch["blueprint"]["id"] != parent["blueprint"]["id"], "branch creates a persistent draft with fresh blueprint id");
            check(db.versions(projectId).size() == 2, "branch does not fabricate a generated version");
            auto metadata = db.updateVersionMetadata(parent["id"], {{"rating", 4}, {"notes", "更适合片尾"}});
            check(metadata["rating"] == 4 && metadata["blueprint"] == parent["blueprint"], "metadata preserves blueprint snapshot");
            invalid([&] { db.updateVersionMetadata(parent["id"], {{"rating", 4294967300LL}}); }, "rating integer overflow rejected");
            invalid([&] { db.updateVersionMetadata(parent["id"], {{"blueprint", b}}); }, "immutable version fields rejected");
            check(db.activeTaskCount() == 0, "completed tasks excluded from activity");
            check(db.exportProject(projectId)["versions"].size() == 2 && !db.exportProject(projectId)["includesAudio"].get<bool>(), "structured project export includes version records");
            db.createTask({{"id", "interrupted-test"}, {"projectId", projectId}, {"status", "generating"}});
        }
        {
            Database db(root / "test.db");
            check(db.projects().size() == 1 && db.versions(projectId).size() == 2, "restart persistence");
            check(db.task("interrupted-test")["status"] == "failed", "interrupted task recovery");
            check(db.task(firstTaskId)["status"] == "completed", "completed task survives restart");
            {
                GenerationQueue queue(db, root, std::make_unique<MockMusicProvider>(root / "missing.wav"));
                auto failed = queue.enqueue(projectId, b, "preview");
                for (int i = 0; i < 100 && db.task(failed["id"])["status"] != "failed"; ++i) std::this_thread::sleep_for(std::chrono::milliseconds(30));
                check(db.task(failed["id"])["status"] == "failed", "missing audio failure");
            }
            db.deleteProject(projectId);
            check(db.projects().empty() && db.versions().empty(), "cascade deletion");
        }
        std::filesystem::remove_all(root);
        std::cout << "Passed " << checks << " core checks\n"; return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n'; std::filesystem::remove_all(root); return 1;
    }
}
