#pragma once
#include "museflow/generation_queue.hpp"
#include "museflow/runtime.hpp"
#include "museflow/settings.hpp"
#include "museflow/text_jobs.hpp"
#include <filesystem>
namespace httplib { class Server; }
namespace museflow {
void configureServer(httplib::Server& server, Database& db, GenerationQueue& queue, SettingsService& settings, TextJobs& textJobs, const RuntimePaths& paths, const std::filesystem::path& web);
}
