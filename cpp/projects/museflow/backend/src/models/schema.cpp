#include "museflow/schema.hpp"
#include "museflow/runtime.hpp"
#include "embedded_schema.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <sstream>
#include <set>

namespace museflow {
namespace {
// Implements exactly the keywords used by our bundled, closed schema. Unknown keywords fail at startup.
void checkSchema(const json& schema) {
    static const std::set<std::string> supported{"$schema", "title", "type", "properties", "required", "additionalProperties", "items", "minItems", "maxItems", "minLength", "maxLength", "minimum", "maximum", "enum"};
    for (auto it = schema.begin(); it != schema.end(); ++it) if (!supported.contains(it.key())) throw std::logic_error("音乐 Schema 包含未支持的关键字");
    if (schema.contains("properties")) for (const auto& child : schema["properties"]) checkSchema(child);
    if (schema.contains("items")) checkSchema(schema["items"]);
}
void requireSchema(bool condition, const std::string& path, const char* reason) {
    if (!condition) throw ValidationError("音乐设计稿 Schema 校验失败：" + path + " " + reason);
}
void validateNode(const json& value, const json& schema, const std::string& path) {
    const std::string type = schema.at("type");
    bool correct = type == "object" ? value.is_object() : type == "array" ? value.is_array() : type == "string" ? value.is_string()
        : type == "integer" ? value.is_number_integer() : type == "number" ? value.is_number() : type == "boolean" && value.is_boolean();
    requireSchema(correct, path, "类型错误");
    if (schema.contains("enum")) requireSchema(std::find(schema["enum"].begin(), schema["enum"].end(), value) != schema["enum"].end(), path, "不在允许值中");
    if (type == "object") {
        for (const auto& required : schema.at("required")) requireSchema(value.contains(required.get<std::string>()), path + "." + required.get<std::string>(), "缺少必填字段");
        const auto& properties = schema.at("properties");
        for (auto it = value.begin(); it != value.end(); ++it) {
            // Never echo unrecognized field names: model responses may contain credentials there.
            requireSchema(properties.contains(it.key()), path, "包含未允许的字段");
            validateNode(it.value(), properties.at(it.key()), path + "." + it.key());
        }
    } else if (type == "array") {
        requireSchema(value.size() >= schema.value("minItems", size_t(0)) && value.size() <= schema.value("maxItems", size_t(32)), path, "数组长度超出范围");
        for (size_t i = 0; i < value.size(); ++i) validateNode(value[i], schema.at("items"), path + "[" + std::to_string(i) + "]");
    } else if (type == "string") {
        const auto& text = value.get_ref<const std::string&>();
        const auto length = static_cast<size_t>(std::count_if(text.begin(), text.end(), [](unsigned char c) { return (c & 0xc0) != 0x80; }));
        requireSchema(length >= schema.value("minLength", size_t(0)) && length <= schema.value("maxLength", size_t(32000)), path, "文本长度超出范围");
    } else if (value.is_number()) {
        const auto number = value.get<double>();
        requireSchema(std::isfinite(number) && number >= schema.at("minimum").get<double>() && number <= schema.at("maximum").get<double>(), path, "数值超出范围");
    }
}
std::string fingerprint(const MusicBlueprint& blueprint) {
    json basis = blueprint;
    for (const auto* field : {"id", "finalLyriaPrompt", "promptSource", "promptFingerprint"}) basis.erase(field);
    for (auto& section : basis["structure"]) section.erase("id");
    // Stable FNV-1a is a change detector only, not an authentication or security mechanism.
    uint64_t hash = 14695981039346656037ULL;
    for (unsigned char byte : basis.dump()) { hash ^= byte; hash *= 1099511628211ULL; }
    std::ostringstream output; output << std::hex << hash; return output.str();
}
}
const json& blueprintSchema() {
    static const json schema = [] { auto value = json::parse(blueprintSchemaText); checkSchema(value); return value; }();
    return schema;
}
void stampPrompt(MusicBlueprint& blueprint, const std::string& source) { blueprint.promptSource = source; blueprint.promptFingerprint = fingerprint(blueprint); }
bool promptIsCurrent(const MusicBlueprint& blueprint) {
    return !blueprint.finalLyriaPrompt.empty() && blueprint.finalLyriaPrompt.size() <= 64000 &&
        (blueprint.promptSource == "local" || blueprint.promptSource == "deepseek") && blueprint.promptFingerprint == fingerprint(blueprint);
}
MusicBlueprint parseModelBlueprint(const json& value) {
    validateNode(value, blueprintSchema(), "$blueprint");
    auto blueprint = value.get<MusicBlueprint>();
    validate(blueprint);
    blueprint.id = uuid(); for (auto& section : blueprint.structure) section.id = uuid();
    stampPrompt(blueprint, "deepseek");
    return blueprint;
}
}
