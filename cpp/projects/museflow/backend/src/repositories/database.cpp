#include "museflow/database.hpp"
#include "museflow/runtime.hpp"

namespace museflow {
namespace {
void execute(sqlite3* db, const char* sql) {
    if (sqlite3_exec(db, sql, nullptr, nullptr, nullptr) != SQLITE_OK) throw std::runtime_error("数据库操作失败");
}
class Statement {
    sqlite3_stmt* value_ = nullptr;
public:
    Statement(sqlite3* db, const char* sql) {
        if (sqlite3_prepare_v2(db, sql, -1, &value_, nullptr) != SQLITE_OK) throw std::runtime_error("数据库查询准备失败");
    }
    ~Statement() { sqlite3_finalize(value_); }
    void bind(int index, const std::string& value) {
        if (sqlite3_bind_text(value_, index, value.c_str(), static_cast<int>(value.size()), SQLITE_TRANSIENT) != SQLITE_OK) throw std::runtime_error("数据库参数绑定失败");
    }
    bool next() {
        int result = sqlite3_step(value_);
        if (result != SQLITE_ROW && result != SQLITE_DONE) throw std::runtime_error("数据库读写失败");
        return result == SQLITE_ROW;
    }
    std::string text(int index) {
        const auto* result = sqlite3_column_text(value_, index);
        return result ? reinterpret_cast<const char*>(result) : "";
    }
};
json record(sqlite3* db, const char* sql, const std::string& id) {
    Statement query(db, sql); query.bind(1, id);
    if (!query.next()) throw NotFound("记录不存在");
    return json::parse(query.text(0));
}
void writeTask(sqlite3* db, const json& task) {
    Statement query(db, "UPDATE tasks SET status=?, payload=? WHERE id=?");
    query.bind(1, task.at("status")); query.bind(2, task.dump()); query.bind(3, task.at("id")); query.next();
}
}
Database::Database(const std::filesystem::path& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        sqlite3_close(db_); db_ = nullptr; throw std::runtime_error("无法打开数据库");
    }
    try {
        sqlite3_busy_timeout(db_, 5000);
        execute(db_, "PRAGMA foreign_keys=ON; PRAGMA journal_mode=WAL; PRAGMA user_version=1;"
            "CREATE TABLE IF NOT EXISTS projects (id TEXT PRIMARY KEY, payload TEXT NOT NULL);"
            "CREATE TABLE IF NOT EXISTS tasks (id TEXT PRIMARY KEY, project_id TEXT NOT NULL REFERENCES projects(id) ON DELETE CASCADE, status TEXT NOT NULL, payload TEXT NOT NULL);"
            "CREATE TABLE IF NOT EXISTS versions (id TEXT PRIMARY KEY, project_id TEXT NOT NULL REFERENCES projects(id) ON DELETE CASCADE, task_id TEXT NOT NULL UNIQUE REFERENCES tasks(id) ON DELETE CASCADE, payload TEXT NOT NULL);"
            "CREATE INDEX IF NOT EXISTS tasks_project ON tasks(project_id);"
            "CREATE INDEX IF NOT EXISTS versions_project ON versions(project_id);");
        Statement pending(db_, "SELECT payload FROM tasks WHERE status NOT IN ('completed','failed')");
        std::vector<json> interrupted;
        while (pending.next()) interrupted.push_back(json::parse(pending.text(0)));
        for (auto& task : interrupted) {
            task["status"] = "failed"; task["error"] = "程序已重启，上次生成中断，请重新生成"; task["updatedAt"] = timestamp(); writeTask(db_, task);
        }
    } catch (...) { sqlite3_close(db_); db_ = nullptr; throw; }
}
Database::~Database() { sqlite3_close(db_); }
json Database::projects() {
    std::lock_guard lock(mutex_);
    Statement query(db_, "SELECT payload FROM projects ORDER BY json_extract(payload,'$.updatedAt') DESC, rowid DESC");
    json result = json::array(); while (query.next()) result.push_back(json::parse(query.text(0))); return result;
}
json Database::project(const std::string& id) {
    std::lock_guard lock(mutex_);
    auto result = record(db_, "SELECT payload FROM projects WHERE id=?", id);
    Statement query(db_, "SELECT payload FROM tasks WHERE project_id=? ORDER BY rowid DESC"); query.bind(1, id);
    result["tasks"] = json::array(); while (query.next()) result["tasks"].push_back(json::parse(query.text(0))); return result;
}
json Database::saveProject(const json& payload, const std::string& id) {
    auto blueprint = parseBlueprint(payload.at("blueprint"));
    auto name = payload.value("name", blueprint.title);
    if (name.empty() || name.size() > 300) throw ValidationError("项目名称不能为空，且不能超过 300 字节");
    std::lock_guard lock(mutex_);
    json result;
    if (id.empty()) result = {{"id", uuid()}, {"createdAt", timestamp()}};
    else result = record(db_, "SELECT payload FROM projects WHERE id=?", id);
    // Whitelist persisted fields: never persist arbitrary request properties or secrets.
    result["name"] = name; result["blueprint"] = blueprint; result["sceneDescription"] = blueprint.sceneDescription; result["updatedAt"] = timestamp();
    if (payload.contains("draftParentVersionId")) result["draftParentVersionId"] = payload["draftParentVersionId"];
    if (result.contains("draftParentVersionId") && !result["draftParentVersionId"].is_null()) {
        const auto parentId = result["draftParentVersionId"].get<std::string>();
        if (!isUuid(parentId)) throw ValidationError("父版本 ID 无效");
        auto parent = record(db_, "SELECT payload FROM versions WHERE id=?", parentId);
        if (parent["projectId"] != result["id"]) throw ValidationError("父版本必须属于同一项目");
    }
    Statement query(db_, "INSERT INTO projects(id,payload) VALUES(?,?) ON CONFLICT(id) DO UPDATE SET payload=excluded.payload");
    query.bind(1, result["id"]); query.bind(2, result.dump()); query.next(); return result;
}
void Database::deleteProject(const std::string& id) {
    std::lock_guard lock(mutex_);
    record(db_, "SELECT payload FROM projects WHERE id=?", id);
    Statement active(db_, "SELECT id FROM tasks WHERE project_id=? AND status NOT IN ('completed','failed')"); active.bind(1, id);
    if (active.next()) throw ValidationError("项目仍有生成任务，请等待完成后删除");
    Statement query(db_, "DELETE FROM projects WHERE id=?"); query.bind(1, id); query.next();
}
void Database::createTask(const json& task) {
    std::lock_guard lock(mutex_);
    record(db_, "SELECT payload FROM projects WHERE id=?", task.at("projectId"));
    if (task.contains("parentVersionId") && !task["parentVersionId"].is_null()) {
        auto parent = record(db_, "SELECT payload FROM versions WHERE id=?", task["parentVersionId"]);
        if (parent["projectId"] != task["projectId"]) throw ValidationError("不能从其他项目的版本创建分支");
    }
    Statement query(db_, "INSERT INTO tasks(id,project_id,status,payload) VALUES(?,?,?,?)");
    query.bind(1, task.at("id")); query.bind(2, task.at("projectId")); query.bind(3, task.at("status")); query.bind(4, task.dump()); query.next();
}
json Database::task(const std::string& id) {
    std::lock_guard lock(mutex_); return record(db_, "SELECT payload FROM tasks WHERE id=?", id);
}
void Database::updateTask(const std::string& id, const std::string& status, int progress, const std::string& error) {
    std::lock_guard lock(mutex_);
    auto task = record(db_, "SELECT payload FROM tasks WHERE id=?", id);
    task["status"] = status; task["progress"] = progress; task["error"] = error; task["updatedAt"] = timestamp(); writeTask(db_, task);
}
void Database::completeTask(const std::string& id, const json& version) {
    std::lock_guard lock(mutex_);
    auto task = record(db_, "SELECT payload FROM tasks WHERE id=?", id);
    task["status"] = "completed"; task["progress"] = 100; task["versionId"] = version.at("id"); task["actualDurationSeconds"] = version.at("actualDurationSeconds"); task["updatedAt"] = timestamp();
    for (const auto* field : {"format", "sourceDurationSeconds", "strictDurationApplied", "songText", "songStructure"})
        if (version.contains(field)) task[field] = version[field];
    execute(db_, "BEGIN IMMEDIATE");
    try {
        Statement query(db_, "INSERT INTO versions(id,project_id,task_id,payload) VALUES(?,?,?,?)");
        query.bind(1, version.at("id")); query.bind(2, task.at("projectId")); query.bind(3, id); query.bind(4, version.dump()); query.next();
        writeTask(db_, task); execute(db_, "COMMIT");
    } catch (...) { execute(db_, "ROLLBACK"); throw; }
}
json Database::versions(const std::string& projectId) {
    std::lock_guard lock(mutex_);
    Statement query(db_, "SELECT payload FROM versions WHERE (?='' OR project_id=?) ORDER BY rowid DESC");
    query.bind(1, projectId); query.bind(2, projectId);
    json result = json::array(); while (query.next()) result.push_back(json::parse(query.text(0))); return result;
}

json Database::version(const std::string& id) {
    std::lock_guard lock(mutex_); return record(db_, "SELECT payload FROM versions WHERE id=?", id);
}
int Database::activeTaskCount() {
    std::lock_guard lock(mutex_);
    Statement query(db_, "SELECT COUNT(*) FROM tasks WHERE status NOT IN ('completed','failed')"); query.next(); return std::stoi(query.text(0));
}
json Database::branchVersion(const std::string& id) {
    std::lock_guard lock(mutex_);
    auto parent = record(db_, "SELECT payload FROM versions WHERE id=?", id);
    const std::string projectId = parent["projectId"];
    auto project = record(db_, "SELECT payload FROM projects WHERE id=?", projectId);
    Statement active(db_, "SELECT id FROM tasks WHERE project_id=? AND status NOT IN ('completed','failed')"); active.bind(1, projectId);
    if (active.next()) throw ValidationError("该项目仍有生成任务，请完成后再创建分支草稿");
    project["blueprint"] = parent["blueprint"]; project["blueprint"]["id"] = uuid();
    project["sceneDescription"] = parent["blueprint"]["sceneDescription"];
    project["draftParentVersionId"] = id; project["updatedAt"] = timestamp();
    Statement update(db_, "UPDATE projects SET payload=? WHERE id=?"); update.bind(1, project.dump()); update.bind(2, projectId); update.next();
    return project;
}
json Database::updateVersionMetadata(const std::string& id, const json& patch) {
    if (!patch.is_object()) throw ValidationError("版本评价必须是对象");
    for (auto it = patch.begin(); it != patch.end(); ++it)
        if (it.key() != "rating" && it.key() != "notes") throw ValidationError("只能修改评分和备注，版本参数不可覆盖");
    if (patch.contains("rating") && (!patch["rating"].is_number_integer() || patch["rating"].get<double>() < 0 || patch["rating"].get<double>() > 5)) throw ValidationError("评分必须是 0～5 的整数，0 表示未评分");
    if (patch.contains("notes") && (!patch["notes"].is_string() || patch["notes"].get_ref<const std::string&>().size() > 6000)) throw ValidationError("备注不能超过 6000 字节");
    std::lock_guard lock(mutex_); auto version = record(db_, "SELECT payload FROM versions WHERE id=?", id);
    version.update(patch); version["metadataUpdatedAt"] = timestamp();
    Statement query(db_, "UPDATE versions SET payload=? WHERE id=?"); query.bind(1, version.dump()); query.bind(2, id); query.next(); return version;
}
json Database::deleteVersion(const std::string& id) {
    std::lock_guard lock(mutex_); auto version = record(db_, "SELECT payload FROM versions WHERE id=?", id);
    Statement children(db_, "SELECT id FROM versions WHERE json_extract(payload,'$.parentVersionId')=?"); children.bind(1, id);
    if (children.next()) throw ValidationError("该版本仍有分支作品，请先删除末端分支，或删除整个项目");
    Statement active(db_, "SELECT id FROM tasks WHERE json_extract(payload,'$.parentVersionId')=? AND status NOT IN ('completed','failed')"); active.bind(1, id);
    if (active.next()) throw ValidationError("该版本仍有正在生成的分支，请等待完成后再删除");
    const std::string taskId = version["generationId"], projectId = version["projectId"];
    auto task = record(db_, "SELECT payload FROM tasks WHERE id=?", taskId);
    auto project = record(db_, "SELECT payload FROM projects WHERE id=?", projectId);
    task["audioDeleted"] = true; task["updatedAt"] = timestamp();
    if (project.value("draftParentVersionId", json(nullptr)) == id) project["draftParentVersionId"] = nullptr;
    execute(db_, "BEGIN IMMEDIATE");
    try {
        Statement remove(db_, "DELETE FROM versions WHERE id=?"); remove.bind(1, id); remove.next();
        writeTask(db_, task);
        Statement update(db_, "UPDATE projects SET payload=? WHERE id=?"); update.bind(1, project.dump()); update.bind(2, projectId); update.next();
        execute(db_, "COMMIT");
    } catch (...) { execute(db_, "ROLLBACK"); throw; }
    return version;
}
json Database::exportProject(const std::string& id) {
    std::lock_guard lock(mutex_);
    auto project = record(db_, "SELECT payload FROM projects WHERE id=?", id);
    json tasks = json::array(), versions = json::array();
    Statement taskQuery(db_, "SELECT payload FROM tasks WHERE project_id=? ORDER BY rowid"); taskQuery.bind(1, id);
    while (taskQuery.next()) tasks.push_back(json::parse(taskQuery.text(0)));
    Statement versionQuery(db_, "SELECT payload FROM versions WHERE project_id=? ORDER BY rowid"); versionQuery.bind(1, id);
    while (versionQuery.next()) versions.push_back(json::parse(versionQuery.text(0)));
    return {{"format", "museflow-project"}, {"schemaVersion", 1}, {"exportedAt", timestamp()}, {"project", project}, {"tasks", tasks}, {"versions", versions}, {"includesAudio", false}};
}
}
