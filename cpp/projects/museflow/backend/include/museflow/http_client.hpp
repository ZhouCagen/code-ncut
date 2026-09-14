#pragma once
#include "museflow/models.hpp"
#include <atomic>
namespace museflow {
struct ProviderError : std::runtime_error {
    std::string code;
    int httpStatus;
    ProviderError(std::string code, std::string message, int status = 502)
        : std::runtime_error(std::move(message)), code(std::move(code)), httpStatus(status) {}
};
struct HttpOptions {
    int connectTimeoutSeconds = 10;
    int requestTimeoutSeconds = 90;
    const std::atomic_bool* cancelled = nullptr;
};
std::string requestElevenLabs(const std::string& path, const std::string& key, const json& body, const HttpOptions& options);
json requestSongApiJson(const std::string& path, const std::string& key, const json* body, const HttpOptions& options);
std::string downloadSongApiAudio(const std::string& url, const HttpOptions& options);
json requestGoogleJson(const std::string& path, const std::string& key, const json* body, const HttpOptions& options);
std::string normalizeTextEndpoint(const std::string& address);
json parseProviderJson(const std::string& input);
bool containsSecret(const json& value, const std::string& secret);
json postModelJson(const std::string& endpoint, const std::string& key, const json& body, const HttpOptions& options);
}
