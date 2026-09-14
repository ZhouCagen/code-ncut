#include "museflow/http_client.hpp"
#include <curl/curl.h>
#include <cstdlib>
#include <memory>
#include <algorithm>

namespace museflow {
namespace {
struct CurlRuntime {
    CurlRuntime() { if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) throw std::runtime_error("无法初始化网络库"); }
    ~CurlRuntime() { curl_global_cleanup(); }
};
void initialize() { static CurlRuntime runtime; }
std::string part(CURLU* url, CURLUPart field) {
    char* value = nullptr;
    if (curl_url_get(url, field, &value, 0) != CURLUE_OK) return "";
    std::string result(value); curl_free(value); return result;
}
struct Buffer { std::string body; bool overflow = false; size_t limit = 2 * 1024 * 1024; };
size_t receive(char* bytes, size_t size, size_t count, void* pointer) {
    auto& buffer = *static_cast<Buffer*>(pointer);
    const size_t limit = buffer.limit;
    if (size && count > (limit - buffer.body.size()) / size) { buffer.overflow = true; return 0; }
    try { buffer.body.append(bytes, size * count); return size * count; } catch (...) { return 0; }
}
int progress(void* pointer, curl_off_t, curl_off_t, curl_off_t, curl_off_t) {
    return pointer && static_cast<const std::atomic_bool*>(pointer)->load() ? 1 : 0;
}
[[noreturn]] void httpError(long status) {
    switch (status) {
        case 400: case 422: throw ProviderError("bad_request", "模型服务拒绝了请求参数，请检查模型名称及请求参数兼容性");
        case 401: throw ProviderError("unauthorized", "API Key 无效或已过期，请在模型设置中重新输入");
        case 402: throw ProviderError("insufficient_balance", "模型账户余额不足，请检查服务商账户");
        case 403: throw ProviderError("forbidden", "没有访问该模型的权限，请检查账户授权或地区限制");
        case 404: throw ProviderError("not_found", "模型或 API 地址不存在，请检查地址和模型名称");
        case 408: case 504: throw ProviderError("timeout", "模型服务响应超时，请稍后重试或调整响应超时", 504);
        case 429: throw ProviderError("rate_limited", "模型请求过于频繁或额度已用尽，请稍后重试；本次未自动重试", 429);
        default:
            if (status >= 300 && status < 400) throw ProviderError("redirect", "模型地址返回重定向；为保护密钥，请直接配置最终 API 地址");
            throw ProviderError("upstream_error", "模型服务暂时不可用，请稍后重试");
    }
}
}
std::string normalizeTextEndpoint(const std::string& address) {
    initialize();
    if (address.empty() || address.size() > 2048 || std::any_of(address.begin(), address.end(), [](unsigned char c) { return c <= 32 || c == 127; }))
        throw ValidationError("API 地址不能为空、过长或包含空白字符");
    std::unique_ptr<CURLU, decltype(&curl_url_cleanup)> url(curl_url(), curl_url_cleanup);
    if (!url || curl_url_set(url.get(), CURLUPART_URL, address.c_str(), 0) != CURLUE_OK) throw ValidationError("API 地址不是有效的 HTTP(S) URL");
    auto scheme = part(url.get(), CURLUPART_SCHEME), host = part(url.get(), CURLUPART_HOST);
    if (scheme != "https" && scheme != "http") throw ValidationError("API 地址仅支持 HTTP(S)");
    if (scheme == "http" && host != "127.0.0.1" && host != "localhost" && host != "[::1]") throw ValidationError("远程 API 必须使用 HTTPS；HTTP 仅用于本机兼容服务");
    if (!part(url.get(), CURLUPART_USER).empty() || !part(url.get(), CURLUPART_PASSWORD).empty() || !part(url.get(), CURLUPART_QUERY).empty() || !part(url.get(), CURLUPART_FRAGMENT).empty())
        throw ValidationError("API 地址不能包含账号、密码、查询参数或片段；Key 请填入独立字段");
    auto normalized = part(url.get(), CURLUPART_URL);
    while (normalized.ends_with('/')) normalized.pop_back();
    if (!normalized.ends_with("/chat/completions")) normalized += "/chat/completions";
    return normalized;
}
json parseProviderJson(const std::string& input) {
    return json::parse(input, [](int depth, json::parse_event_t, json&) {
        if (depth > 64) throw ProviderError("invalid_response", "模型 JSON 嵌套过深，已拒绝处理");
        return true;
    });
}
bool containsSecret(const json& value, const std::string& secret) {
    if (secret.empty()) return false;
    if (value.is_string()) return value.get_ref<const std::string&>().find(secret) != std::string::npos;
    if (value.is_object() || value.is_array()) {
        for (auto it = value.begin(); it != value.end(); ++it) {
            if (value.is_object() && it.key().find(secret) != std::string::npos) return true;
            if (containsSecret(it.value(), secret)) return true;
        }
    }
    return false;
}
static std::string requestBody(const std::string& url, const std::string& key, const json* body, const HttpOptions& options, bool google, bool audio = false, bool eleven = false) {
    initialize();
    if (!audio && key.empty()) throw ProviderError("missing_key", "请先在模型设置输入 API Key，或设置服务商环境变量", 400);
    if (key.size() > 8192 || std::any_of(key.begin(), key.end(), [](unsigned char c) { return c < 33 || c > 126; })) throw ValidationError("API Key 格式无效");
    if (options.cancelled && options.cancelled->load()) throw ProviderError("cancelled", "服务正在关闭，模型请求已取消", 503);
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> handle(curl_easy_init(), curl_easy_cleanup);
    if (!handle) throw ProviderError("network_init", "无法初始化模型网络请求");
    curl_slist* rawHeaders = nullptr;
    std::vector<std::string> requestHeaders;
    if (!audio) requestHeaders = {"Content-Type: application/json", (eleven ? "Accept: audio/mpeg, application/json" : "Accept: application/json"), (eleven ? "xi-api-key: " : google ? "x-goog-api-key: " : "Authorization: Bearer ") + key};
    for (const auto& header : requestHeaders) {
        auto* next = curl_slist_append(rawHeaders, header.c_str());
        if (!next) { curl_slist_free_all(rawHeaders); throw std::bad_alloc(); }
        rawHeaders = next;
    }
    std::unique_ptr<curl_slist, decltype(&curl_slist_free_all)> headers(rawHeaders, curl_slist_free_all);
    const auto payload = body ? body->dump() : ""; Buffer buffer;
    if (audio || eleven) buffer.limit = 128 * 1024 * 1024;
    if (google) buffer.limit = 192 * 1024 * 1024;
    curl_easy_setopt(handle.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle.get(), CURLOPT_HTTPHEADER, headers.get());
    if (body) {
    curl_easy_setopt(handle.get(), CURLOPT_POST, 1L);
    curl_easy_setopt(handle.get(), CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(handle.get(), CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(payload.size()));
    }
    curl_easy_setopt(handle.get(), CURLOPT_CONNECTTIMEOUT, static_cast<long>(options.connectTimeoutSeconds));
    curl_easy_setopt(handle.get(), CURLOPT_TIMEOUT, static_cast<long>(options.requestTimeoutSeconds));
    curl_easy_setopt(handle.get(), CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(handle.get(), CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(handle.get(), CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(handle.get(), CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, receive);
    curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(handle.get(), CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle.get(), CURLOPT_XFERINFOFUNCTION, progress);
    curl_easy_setopt(handle.get(), CURLOPT_XFERINFODATA, options.cancelled);
    // libcurl reads HTTPS_PROXY and NO_PROXY itself. Standalone Linux app also accepts HTTP_PROXY.
    if (url.starts_with("http:") && !std::getenv("http_proxy")) {
        if (const auto* proxy = std::getenv("HTTP_PROXY")) curl_easy_setopt(handle.get(), CURLOPT_PROXY, proxy);
    }
    auto result = curl_easy_perform(handle.get());
    if (result != CURLE_OK) {
        if (buffer.overflow) throw ProviderError("response_too_large", "模型响应超过本地大小限制，已停止接收");
        if (result == CURLE_OPERATION_TIMEDOUT) throw ProviderError("timeout", "连接或模型响应超时，请检查网络、代理或调整响应超时", 504);
        if (result == CURLE_ABORTED_BY_CALLBACK) throw ProviderError("cancelled", "服务正在关闭，模型请求已取消", 503);
        if (result == CURLE_COULDNT_RESOLVE_HOST || result == CURLE_COULDNT_RESOLVE_PROXY) throw ProviderError("dns_error", "无法解析模型或代理地址，请检查网络及代理设置");
        if (result == CURLE_PEER_FAILED_VERIFICATION || result == CURLE_SSL_CONNECT_ERROR) throw ProviderError("tls_error", "模型 HTTPS 证书或安全连接验证失败，请检查系统时间和证书");
        throw ProviderError("network_error", "无法连接模型服务，请检查网络、地址及代理设置");
    }
    long status = 0; curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &status);
    if (eleven && (status < 200 || status >= 300)) {
        // Inspect only known codes. Never expose raw upstream messages or credentials.
        std::string code, detailStatus;
        try {
            const auto error = parseProviderJson(buffer.body).value("detail", json::object());
            if (error.is_object()) { code = error.value("code", ""); detailStatus = error.value("status", ""); }
        } catch (...) {}
        if (code == "paid_plan_required") throw ProviderError("paid_plan_required", "ElevenLabs 音乐 API 需要付费套餐，请检查订阅是否已生效");
        if (detailStatus == "missing_permissions" || code == "missing_permissions") throw ProviderError("missing_permissions", "ElevenLabs Key 缺少音乐接口权限，请在控制台启用 Music 权限");
        if (status == 402 || code == "quota_exceeded" || detailStatus == "quota_exceeded") throw ProviderError("insufficient_balance", "ElevenLabs 音乐额度不足，请检查账户及 Key 的额度限制");
    }
    if (status < 200 || status >= 300) httpError(status);
    if (eleven) {
        if (buffer.body.find(key) != std::string::npos) throw ProviderError("unsafe_response", "模型响应包含敏感凭据，已丢弃响应");
        char* contentType = nullptr;
        curl_easy_getinfo(handle.get(), CURLINFO_CONTENT_TYPE, &contentType);
        const std::string type = contentType ? contentType : "";
        const bool plan = url.ends_with("/music/plan");
        if (plan ? !type.starts_with("application/json") : !(type.starts_with("audio/mpeg") || type.starts_with("audio/mp3") || type.starts_with("application/octet-stream")))
            throw ProviderError("invalid_response", "ElevenLabs 返回了不支持的响应类型");
    }
    return buffer.body;
}
static json requestJson(const std::string& url, const std::string& key, const json* body, const HttpOptions& options, bool google) {
    const auto content = requestBody(url, key, body, options, google);
    // Never return raw upstream errors: they may echo Authorization headers or submitted keys.
    if (content.find(key) != std::string::npos) throw ProviderError("unsafe_response", "模型响应包含敏感凭据，已丢弃响应");
    try {
        auto value = parseProviderJson(content);
        if (containsSecret(value, key)) throw ProviderError("unsafe_response", "模型响应包含敏感凭据，已丢弃响应");
        return value;
    }
    catch (const json::exception&) { throw ProviderError("invalid_response", "模型服务返回的内容不是有效 JSON，请检查 API 兼容性"); }
}
json postModelJson(const std::string& endpoint, const std::string& key, const json& body, const HttpOptions& options) {
    return requestJson(normalizeTextEndpoint(endpoint), key, &body, options, false);
}
json requestGoogleJson(const std::string& path, const std::string& key, const json* body, const HttpOptions& options) {
    std::string base = "https://generativelanguage.googleapis.com/v1beta";
    // Test injection is restricted to loopback; production always uses Google's official endpoint.
    if (const char* fixture = std::getenv("MUSEFLOW_GOOGLE_TEST_BASE")) {
        std::string value = fixture;
        auto normalized = normalizeTextEndpoint(value);
        if (!normalized.starts_with("http://127.0.0.1:")) throw ValidationError("Google 测试地址仅允许本机 IPv4 回环地址");
        base = normalized.substr(0, normalized.size() - std::string("/chat/completions").size());
    }
    return requestJson(base + path, key, body, options, true);
}
namespace {
std::string songApiBase() {
    if (const auto* fixture = std::getenv("MUSEFLOW_SONGAPI_TEST_BASE")) {
        const auto normalized = normalizeTextEndpoint(fixture);
        if (!normalized.starts_with("http://127.0.0.1:")) throw ValidationError("SongAPI 测试地址仅允许本机 IPv4 回环地址");
        return normalized.substr(0, normalized.size() - std::string("/chat/completions").size());
    }
    return "https://api.songapi.dev";
}
}
std::string requestElevenLabs(const std::string& path, const std::string& key, const json& body, const HttpOptions& options) {
    std::string base = "https://api.elevenlabs.io";
    if (const auto* fixture = std::getenv("MUSEFLOW_ELEVENLABS_TEST_BASE")) {
        const auto normalized = normalizeTextEndpoint(fixture);
        if (!normalized.starts_with("http://127.0.0.1:")) throw ValidationError("ElevenLabs 测试地址仅允许本机 IPv4 回环地址");
        base = normalized.substr(0, normalized.size() - std::string("/chat/completions").size());
    }
    return requestBody(base + path, key, &body, options, false, false, true);
}
json requestSongApiJson(const std::string& path, const std::string& key, const json* body, const HttpOptions& options) {
    return requestJson(songApiBase() + path, key, body, options, false);
}
std::string downloadSongApiAudio(const std::string& address, const HttpOptions& options) {
    initialize();
    // Audio is fetched without credentials; only known audio hosts (or the explicit fixture) are accepted.
    if (address.empty() || address.size() > 8192 || std::any_of(address.begin(), address.end(), [](unsigned char c) { return c <= 32 || c == 127; }))
        throw ProviderError("invalid_audio_url", "SongAPI 返回了无效音频地址");
    std::unique_ptr<CURLU, decltype(&curl_url_cleanup)> url(curl_url(), curl_url_cleanup);
    if (!url || curl_url_set(url.get(), CURLUPART_URL, address.c_str(), 0) != CURLUE_OK)
        throw ProviderError("invalid_audio_url", "SongAPI 返回了无效音频地址");
    const auto host = part(url.get(), CURLUPART_HOST);
    const auto base = songApiBase();
    const bool fixture = base.starts_with("http://127.0.0.1:") && address.starts_with(base + "/");
    const auto port = part(url.get(), CURLUPART_PORT);
    const bool allowed = part(url.get(), CURLUPART_SCHEME) == "https" && (port.empty() || port == "443") &&
        (host == "cdn.songapi.dev" || host == "cdn.suno.ai" || host == "cdn1.suno.ai" || host == "cdn2.suno.ai" || host == "cdn.suno.com");
    if ((!fixture && !allowed) || !part(url.get(), CURLUPART_USER).empty() || !part(url.get(), CURLUPART_PASSWORD).empty() || !part(url.get(), CURLUPART_FRAGMENT).empty())
        throw ProviderError("invalid_audio_url", "SongAPI 返回了未支持的音频下载地址，请在 SongAPI 控制台查看结果");
    return requestBody(address, "", nullptr, options, false, true);
}

}
