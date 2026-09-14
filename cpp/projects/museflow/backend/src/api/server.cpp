#include "museflow/server.hpp"
#include "museflow/schema.hpp"
#include <httplib.h>
#include <set>
#include <cstdlib>

namespace museflow {
namespace {
void respond(httplib::Response& response, const json& data, int status = 200) {
    response.status = status; response.set_content(data.dump(), "application/json; charset=utf-8");
}
json body(const httplib::Request& request) {
    if (request.get_header_value("Content-Type").find("application/json") != 0) throw ValidationError("请求必须使用 application/json");
    auto result = json::parse(request.body);
    if (!result.is_object()) throw ValidationError("请求正文必须是 JSON 对象");
    return result;
}
std::string idOf(const httplib::Request& request) {
    std::string id = request.matches[1];
    if (!isUuid(id)) throw ValidationError("记录 ID 格式无效");
    return id;
}
}
void configureServer(httplib::Server& server, Database& db, GenerationQueue& queue, SettingsService& settings, TextJobs& textJobs, const RuntimePaths& paths, const std::filesystem::path& web) {
    server.new_task_queue = [] { return new httplib::ThreadPool(8); };
    // Avoid cpp-httplib's Linux SO_REUSEPORT default: separate instances must never share requests.
    server.set_socket_options([](auto socket) {
        const int reuse = 1;
        if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) throw std::runtime_error("无法设置本地监听选项");
    });
    server.set_payload_max_length(256 * 1024);
    server.set_read_timeout(10, 0); server.set_write_timeout(10, 0);
    server.set_default_headers({{"X-Content-Type-Options", "nosniff"}, {"Referrer-Policy", "no-referrer"}, {"X-Frame-Options", "DENY"},
        {"Content-Security-Policy", "default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data:; media-src 'self' blob:; connect-src 'self'; frame-ancestors 'none'"}});
    server.set_pre_routing_handler([](const auto& request, auto& response) {
        auto host = request.get_header_value("Host");
        auto origin = request.get_header_value("Origin");
        const std::set<std::string> hosts{"127.0.0.1:18080", "localhost:18080"};
        const std::set<std::string> origins{"http://127.0.0.1:5173", "http://localhost:5173", "http://127.0.0.1:18080", "http://localhost:18080"};
        if (!hosts.contains(host) || (!origin.empty() && !origins.contains(origin)) || request.get_header_value("Sec-Fetch-Site") == "cross-site") {
            respond(response, {{"error", "仅允许本地 MuseFlow 页面访问"}}, 403); return httplib::Server::HandlerResponse::Handled;
        }
        if (request.path.starts_with("/api/")) response.set_header("Cache-Control", "no-store");
        return httplib::Server::HandlerResponse::Unhandled;
    });
    server.set_exception_handler([](const auto&, auto& response, std::exception_ptr error) {
        try { if (error) std::rethrow_exception(error); }
        catch (const ProviderError& e) { respond(response, {{"error", e.what()}, {"code", e.code}}, e.httpStatus); }
        catch (const ValidationError& e) { respond(response, {{"error", e.what()}}, 400); }
        catch (const NotFound& e) { respond(response, {{"error", e.what()}}, 404); }
        catch (const json::exception&) { respond(response, {{"error", "请求 JSON 无效或缺少必要字段"}}, 400); }
        catch (...) { respond(response, {{"error", "本地服务处理失败，请检查数据目录权限及磁盘空间"}}, 500); }
    });
    server.set_error_handler([](const auto&, auto& response) {
        if (response.body.empty()) respond(response, {{"error", response.status == 413 ? "请求内容过大" : "请求的资源不存在或请求无效"}}, response.status);
    });
    const auto* session = std::getenv("MUSEFLOW_PARENT_SESSION");
    const std::string desktopSession = session ? session : "";
    server.Get("/api/activity", [&db, &textJobs](const auto&, auto& response) { respond(response, {{"musicTasks", db.activeTaskCount()}, {"textTasks", textJobs.activeCount()}}); });
    server.Get("/api/health", [&settings, desktopSession](const auto&, auto& response) {
        respond(response, {{"status", "ok"}, {"app", "MuseFlow"}, {"version", "0.4.0"}, {"mode", settings.publicSettings()["mode"]}, {"phase", 4}, {"desktopSession", desktopSession}});
    });
    server.Get("/api/settings", [&settings](const auto&, auto& response) { respond(response, settings.publicSettings()); });
    server.Put("/api/settings", [&settings](const auto& request, auto& response) { respond(response, settings.update(body(request))); });
    server.Get(R"(/api/text-jobs/([^/]+))", [&textJobs](const auto& request, auto& response) { respond(response, textJobs.get(idOf(request))); });
    server.Get("/api/blueprints/schema", [](const auto&, auto& response) { respond(response, blueprintSchema()); });
    server.Post("/api/providers/text/test", [&settings, &textJobs](const auto&, auto& response) {
        auto snapshot = settings.textSnapshot();
        if (snapshot.provider == "mock") {
            auto result = MockTextProvider{}.testConnection(); settings.recordTextTest(snapshot.revision, result);
            respond(response, {{"ok", result.ok}, {"message", result.message}}); return;
        }
        auto job = textJobs.submit([snapshot, &settings](const std::atomic_bool& cancelled) {
            try {
                auto result = DeepSeekProvider(snapshot, &cancelled).testConnection();
                settings.recordTextTest(snapshot.revision, result);
                return json{{"ok", result.ok}, {"message", result.message}};
            } catch (const ProviderError& error) { settings.recordTextTest(snapshot.revision, {false, error.what()}); throw; }
            catch (const ValidationError& error) { settings.recordTextTest(snapshot.revision, {false, error.what()}); throw; }
        });
        respond(response, {{"job", job}}, 202);
    });
    server.Post("/api/providers/music/test", [&settings, &textJobs, paths](const auto&, auto& response) {
        auto snapshot = settings.musicSnapshot();
        if (snapshot.provider == "mock") {
            TestResult result{true, "模拟音乐服务已启用；请生成测试音乐以验证完整流程"};
            settings.recordMusicTest(snapshot.revision, result);
            respond(response, {{"ok", true}, {"message", result.message}}); return;
        }
        auto job = textJobs.submit([snapshot, &settings, paths](const std::atomic_bool& cancelled) {
            try {
                auto result = snapshot.provider == "elevenlabs"
                    ? ElevenLabsProvider(snapshot, paths.cache, &cancelled).testConnection()
                    : snapshot.provider == "songapi"
                    ? SongApiProvider(snapshot, paths.cache, &cancelled).testConnection()
                    : LyriaProvider(snapshot, paths.cache, &cancelled).testConnection();
                settings.recordMusicTest(snapshot.revision, result);
                return json{{"ok", result.ok}, {"message", result.message}};
            } catch (const ProviderError& error) { settings.recordMusicTest(snapshot.revision, {false, error.what()}); throw; }
        });
        respond(response, {{"job", job}}, 202);
    });
    server.Post("/api/blueprints/generate", [&settings, &textJobs](const auto& request, auto& response) {
        auto data = body(request);
        const auto input = data.at("userInput").template get<std::string>();
        const auto purpose = data.value("purpose", "other");
        if (input.empty() || input.size() > 12000) throw ValidationError("请输入场景描述（最多 12000 字节）");
        if (!std::set<std::string>{"game", "video", "animation", "story", "song", "other"}.contains(purpose)) throw ValidationError("用途无效");
        auto snapshot = settings.textSnapshot();
        if (snapshot.provider == "mock") {
            auto blueprint = MockTextProvider{}.createBlueprint(input); blueprint.purpose = purpose;
            blueprint.finalLyriaPrompt = composePrompt(blueprint); stampPrompt(blueprint, "local");
            respond(response, {{"blueprint", blueprint}, {"provider", "mock"}}); return;
        }
        auto job = textJobs.submit([snapshot, input, purpose](const std::atomic_bool& cancelled) {
            auto blueprint = DeepSeekProvider(snapshot, &cancelled).createBlueprint(json{{"sceneDescription", input}, {"purpose", purpose}}.dump());
            if (blueprint.purpose != purpose) throw ProviderError("invalid_blueprint", "模型未保留指定用途，请重新生成");
            return json{{"blueprint", blueprint}, {"provider", "deepseek"}};
        });
        respond(response, {{"job", job}}, 202);
    });
    server.Post("/api/blueprints/validate", [](const auto& request, auto& response) {
        auto blueprint = parseBlueprint(body(request).at("blueprint")); respond(response, {{"blueprint", blueprint}});
    });
    server.Post("/api/blueprints/revise", [&settings, &textJobs](const auto& request, auto& response) {
        auto data = body(request); auto current = parseBlueprint(data.at("blueprint"));
        const auto instruction = data.at("instruction").template get<std::string>();
        if (instruction.empty() || instruction.size() > 6000) throw ValidationError("修改要求应为 1～6000 字节");
        auto snapshot = settings.textSnapshot();
        if (snapshot.provider == "mock") {
            auto blueprint = MockTextProvider{}.reviseBlueprint(current, instruction); stampPrompt(blueprint, "local");
            respond(response, {{"blueprint", blueprint}, {"provider", "mock"}, {"message", "模拟模式仅记录修改要求，不执行智能改写"}}); return;
        }
        auto job = textJobs.submit([snapshot, current, instruction](const std::atomic_bool& cancelled) {
            return json{{"blueprint", DeepSeekProvider(snapshot, &cancelled).reviseBlueprint(current, instruction)}, {"provider", "deepseek"}};
        });
        respond(response, {{"job", job}}, 202);
    });
    server.Get("/api/projects", [&db](const auto&, auto& response) { respond(response, db.projects()); });
    server.Post("/api/projects", [&db](const auto& request, auto& response) { respond(response, db.saveProject(body(request)), 201); });
    server.Get(R"(/api/projects/([^/]+))", [&db](const auto& request, auto& response) { respond(response, db.project(idOf(request))); });
    server.Put(R"(/api/projects/([^/]+))", [&db](const auto& request, auto& response) { respond(response, db.saveProject(body(request), idOf(request))); });
    server.Delete(R"(/api/projects/([^/]+))", [&db, paths](const auto& request, auto& response) {
        auto id = idOf(request); db.deleteProject(id);
        std::error_code error; std::filesystem::remove_all(paths.data / "projects" / id, error);
        respond(response, {{"ok", true}, {"audioCleanupPending", static_cast<bool>(error)}});
    });
    server.Post("/api/generations", [&queue](const auto& request, auto& response) {
        auto data = body(request);
        respond(response, queue.enqueue(data.at("projectId"), parseBlueprint(data.at("blueprint")), data.value("mode", "preview"), data.value("parentVersionId", "")), 202);
    });
    server.Get(R"(/api/generations/([^/]+))", [&db](const auto& request, auto& response) { respond(response, db.task(idOf(request))); });
    server.Get(R"(/api/generations/([^/]+)/audio)", [&db, paths](const auto& request, auto& response) {
        auto id = idOf(request); auto task = db.task(id);
        if (task.value("audioDeleted", false)) throw NotFound("该作品音频已删除");
        if (task.at("status") != "completed") { respond(response, {{"error", "音频尚未生成完成"}}, 409); return; }
        const std::string projectId = task.at("projectId");
        if (!isUuid(projectId)) throw NotFound("音频不存在");
        const auto format = task.value("format", "wav");
        if (format != "wav" && format != "mp3") throw NotFound("音频格式无效");
        auto file = paths.data / "projects" / projectId / (id + "." + format);
        if (!std::filesystem::is_regular_file(file)) throw NotFound("音频文件不存在");
        if (request.has_param("download")) response.set_header("Content-Disposition", "attachment; filename=\"museflow-" + id + "." + format + "\"");
        response.set_file_content(file.string(), format == "wav" ? "audio/wav" : "audio/mpeg");
    });
    server.Get(R"(/api/projects/([^/]+)/versions)", [&db](const auto& request, auto& response) {
        auto id = idOf(request); db.project(id); respond(response, db.versions(id));
    });
    server.Get(R"(/api/projects/([^/]+)/export)", [&db](const auto& request, auto& response) {
        auto id = idOf(request); response.set_header("Content-Disposition", "attachment; filename=\"museflow-project-" + id + ".json\"");
        respond(response, db.exportProject(id));
    });
    server.Get(R"(/api/versions/([^/]+))", [&db](const auto& request, auto& response) { respond(response, db.version(idOf(request))); });
    server.Post(R"(/api/versions/([^/]+)/branch)", [&db](const auto& request, auto& response) { respond(response, {{"project", db.branchVersion(idOf(request))}}, 201); });
    server.Put(R"(/api/versions/([^/]+)/metadata)", [&db](const auto& request, auto& response) { respond(response, db.updateVersionMetadata(idOf(request), body(request))); });
    server.Delete(R"(/api/versions/([^/]+))", [&db, paths](const auto& request, auto& response) {
        auto version = db.deleteVersion(idOf(request));
        const std::string projectId = version["projectId"], taskId = version["generationId"];
        if (!isUuid(projectId) || !isUuid(taskId)) throw ValidationError("音频存储标识符无效");
        bool pending = false;
        for (const auto* suffix : {".wav", ".mp3", ".source.wav", ".source.mp3"}) {
            std::error_code error; std::filesystem::remove(paths.data / "projects" / projectId / (taskId + suffix), error); pending |= static_cast<bool>(error);
        }
        respond(response, {{"ok", true}, {"audioCleanupPending", pending}});
    });
    server.Get("/api/versions", [&db](const auto&, auto& response) { respond(response, db.versions()); });
    if (std::filesystem::is_directory(web)) server.set_mount_point("/", web.string());
    server.Get(R"(/.*)", [web](const auto& request, auto& response) {
        const std::set<std::string> routes{"/", "/studio", "/projects", "/library", "/compare", "/inspiration", "/settings", "/about"};
        if (!routes.contains(request.path)) { respond(response, {{"error", "资源不存在"}}, 404); return; }
        if (std::filesystem::is_regular_file(web / "index.html")) response.set_file_content((web / "index.html").string(), "text/html; charset=utf-8");
        else response.set_content("MuseFlow 后端已启动。开发界面：http://127.0.0.1:5173；生产界面请先运行 scripts/build.sh。", "text/plain; charset=utf-8");
    });
}
}
