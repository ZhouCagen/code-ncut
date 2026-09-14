#include "museflow/runtime.hpp"
#include <array>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <sys/random.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

namespace museflow {
DataDirectoryLock::DataDirectoryLock(const std::filesystem::path& directory) {
    descriptor_ = open((directory / ".instance.lock").c_str(), O_CREAT | O_RDWR | O_CLOEXEC | O_NOFOLLOW, 0600);
    if (descriptor_ < 0) throw std::runtime_error("无法锁定数据目录，请检查目录权限");
    if (flock(descriptor_, LOCK_EX | LOCK_NB) != 0) {
        close(descriptor_); descriptor_ = -1;
        throw std::runtime_error("此数据目录已有 MuseFlow 实例运行，请勿重复启动");
    }
}
DataDirectoryLock::~DataDirectoryLock() { if (descriptor_ >= 0) close(descriptor_); }
std::string uuid() {
    std::array<unsigned char, 16> bytes{};
    std::size_t offset = 0;
    while (offset < bytes.size()) {
        auto count = getrandom(bytes.data() + offset, bytes.size() - offset, 0);
        if (count < 0 && errno == EINTR) continue;
        if (count <= 0) throw std::runtime_error("无法生成安全标识符");
        offset += static_cast<std::size_t>(count);
    }
    bytes[6] = (bytes[6] & 0x0f) | 0x40;
    bytes[8] = (bytes[8] & 0x3f) | 0x80;
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (i == 4 || i == 6 || i == 8 || i == 10) out << '-';
        out << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return out.str();
}
bool isUuid(const std::string& value) {
    if (value.size() != 36) return false;
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) { if (value[i] != '-') return false; }
        else if (!((value[i] >= '0' && value[i] <= '9') || (value[i] >= 'a' && value[i] <= 'f'))) return false;
    }
    return true;
}
std::string timestamp() {
    auto now = std::time(nullptr);
    std::tm value{}; gmtime_r(&now, &value);
    std::ostringstream out; out << std::put_time(&value, "%Y-%m-%dT%H:%M:%SZ");
    return out.str();
}
RuntimePaths RuntimePaths::create() {
    const auto* home = std::getenv("HOME");
    if (!home) throw std::runtime_error("未设置 HOME");
    auto path = [home](const char* variable, const char* fallback) {
        const auto* value = std::getenv(variable);
        auto base = value && std::filesystem::path(value).is_absolute() ? std::filesystem::path(value) : std::filesystem::path(home) / fallback;
        return base / "museflow";
    };
    RuntimePaths result{path("XDG_CONFIG_HOME", ".config"), path("XDG_DATA_HOME", ".local/share"), path("XDG_CACHE_HOME", ".cache")};
    for (const auto& directory : {result.config, result.data, result.cache, result.data / "projects"}) {
        std::filesystem::create_directories(directory);
        std::filesystem::permissions(directory, std::filesystem::perms::owner_all, std::filesystem::perm_options::replace);
    }
    return result;
}
}
