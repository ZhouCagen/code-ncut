#include "museflow/audio_processing.hpp"
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <chrono>
#include <thread>
#include <cmath>
#include <cerrno>
extern char** environ;
namespace museflow {
namespace {
// No shell, bounded stdout, child receives no API keys, cancellation/timeout always reaps the child.
std::string run(std::vector<std::string> args, int timeout, const std::atomic_bool* cancelled) {
    int pipes[2];
    if (pipe2(pipes, O_CLOEXEC | O_NONBLOCK)) throw ProviderError("audio_process", "无法创建音频处理管道");
    posix_spawn_file_actions_t actions; posix_spawn_file_actions_init(&actions);
    posix_spawn_file_actions_adddup2(&actions, pipes[1], STDOUT_FILENO);
    posix_spawn_file_actions_addopen(&actions, STDERR_FILENO, "/dev/null", O_WRONLY, 0);
    posix_spawn_file_actions_addopen(&actions, STDIN_FILENO, "/dev/null", O_RDONLY, 0);
    posix_spawnattr_t attributes; posix_spawnattr_init(&attributes);
    sigset_t empty; sigemptyset(&empty); posix_spawnattr_setsigmask(&attributes, &empty);
    posix_spawnattr_setflags(&attributes, POSIX_SPAWN_SETSIGMASK);
    std::vector<char*> argv; for (auto& arg : args) argv.push_back(arg.data()); argv.push_back(nullptr);
    std::vector<char*> environment;
    for (char** e = environ; *e; ++e) {
        std::string_view entry(*e);
        if (!entry.starts_with("DEEPSEEK_API_KEY=") && !entry.starts_with("GEMINI_API_KEY=") && !entry.starts_with("SONGAPI_KEY=")) environment.push_back(*e);
    }
    environment.push_back(nullptr);
    pid_t child; int launched = posix_spawnp(&child, argv[0], &actions, &attributes, argv.data(), environment.data());
    posix_spawn_file_actions_destroy(&actions); posix_spawnattr_destroy(&attributes); close(pipes[1]);
    if (launched) { close(pipes[0]); throw ProviderError("missing_ffmpeg", "无法启动 FFmpeg/ffprobe，请安装 ffmpeg 并检查 PATH"); }
    std::string output; const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(timeout);
    int status = 0;
    for (;;) {
        char buffer[4096]; ssize_t count;
        while ((count = read(pipes[0], buffer, sizeof(buffer))) > 0) output.append(buffer, count);
        const auto ended = waitpid(child, &status, WNOHANG);
        if (ended == child) break;
        if ((ended < 0 && errno != EINTR) || output.size() > 65536 || (cancelled && cancelled->load()) || std::chrono::steady_clock::now() > deadline) {
            kill(child, SIGKILL); while (waitpid(child, &status, 0) < 0 && errno == EINTR) {}
            close(pipes[0]); throw ProviderError("audio_timeout", "音频处理已取消或超时，请检查音频文件及系统资源");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    close(pipes[0]);
    if (!WIFEXITED(status) || WEXITSTATUS(status)) throw ProviderError("invalid_audio", "FFmpeg 无法解码或处理音频，请检查文件格式及磁盘空间");
    return output;
}
double probe(const std::filesystem::path& file, const std::string& format, const std::atomic_bool* cancelled) {
    auto result = run({"ffprobe", "-v", "error", "-protocol_whitelist", "file,pipe", "-f", format, "-i", file.string(),
        "-show_entries", "format=duration", "-of", "json"}, 30, cancelled);
    try {
        auto value = json::parse(result); auto duration = std::stod(value.at("format").at("duration").get<std::string>());
        if (!std::isfinite(duration) || duration <= 0 || duration > 1200) throw std::runtime_error("range");
        return duration;
    } catch (...) { throw ProviderError("invalid_duration", "音频时长无效或超过 20 分钟处理上限"); }
}
}
ProcessedAudio processAudio(const std::filesystem::path& source, const std::filesystem::path& destination,
    const std::string& sourceFormat, const std::string& outputFormat, bool strict, double target, const std::atomic_bool* cancelled) {
    if ((sourceFormat != "wav" && sourceFormat != "mp3") || (outputFormat != "wav" && outputFormat != "mp3")) throw ValidationError("音频格式无效");
    if (strict && (outputFormat != "wav" || !std::isfinite(target) || target < 5 || target > 600)) throw ValidationError("严格时长使用 WAV，目标范围为 5～600 秒");
    const auto original = probe(source, sourceFormat, cancelled);
    std::vector<std::string> args{"ffmpeg", "-nostdin", "-hide_banner", "-loglevel", "error", "-n", "-protocol_whitelist", "file,pipe", "-f", sourceFormat,
        "-i", source.string(), "-map", "0:a:0", "-vn", "-map_metadata", "-1", "-ar", "44100", "-ac", "2"};
    if (strict) {
        // Fade before padding as well, so a short source never ends abruptly before added silence.
        const double fadeEnd = std::min(original, target), fade = std::min(2.0, fadeEnd / 2);
        args.insert(args.end(), {"-af", "afade=t=out:st=" + std::to_string(fadeEnd - fade) + ":d=" + std::to_string(fade) +
            ",apad,atrim=duration=" + std::to_string(target), "-t", std::to_string(target)});
    }
    if (outputFormat == "wav") args.insert(args.end(), {"-c:a", "pcm_s16le", "-f", "wav"});
    else args.insert(args.end(), {"-c:a", "libmp3lame", "-b:a", "192k", "-f", "mp3"});
    args.push_back(destination.string());
    run(std::move(args), 120, cancelled);
    auto actual = probe(destination, outputFormat, cancelled);
    if (strict && std::abs(actual - target) > 1.0 / 44100 + 0.000001) throw ProviderError("duration_mismatch", "处理后音频未满足严格时长，作品未保存");
    return {original, actual};
}
}
