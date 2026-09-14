#pragma once
#include "museflow/http_client.hpp"
#include <filesystem>
namespace museflow {
struct ProcessedAudio { double sourceDurationSeconds, actualDurationSeconds; };
ProcessedAudio processAudio(const std::filesystem::path& source, const std::filesystem::path& destination,
    const std::string& sourceFormat, const std::string& outputFormat, bool strict, double target,
    const std::atomic_bool* cancelled = nullptr);
}
