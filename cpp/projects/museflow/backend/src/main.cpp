#include "museflow/server.hpp"
#include "museflow/schema.hpp"
#include <httplib.h>
#include <iostream>
#include <csignal>
#include <pthread.h>
#include <spawn.h>
#include <sys/stat.h>
#include <sys/wait.h>
extern char** environ;

int main(int argc, char** argv) {
    using namespace museflow;
    bool openBrowser = false;
    std::filesystem::path assets = MUSEFLOW_ASSET_DIR, web = MUSEFLOW_WEB_DIR;
    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "--open") openBrowser = true;
        else if (argument == "--assets" && i + 1 < argc) assets = argv[++i];
        else if (argument == "--web" && i + 1 < argc) web = argv[++i];
        else { std::cout << "Usage: museflow [--open] [--assets DIR] [--web DIR]\n"; return argument == "--help" ? 0 : 1; }
    }
    umask(0077);
    sigset_t signals; sigemptyset(&signals); sigaddset(&signals, SIGINT); sigaddset(&signals, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &signals, nullptr);
    try {
        auto paths = RuntimePaths::create();
        DataDirectoryLock instance(paths.data);
        SettingsService settings(paths.config);
        blueprintSchema();
        TextJobs textJobs;
        Database db(paths.data / "museflow.db");
        auto provider = std::make_unique<MockMusicProvider>(assets / "mock-demo.wav");
        if (!provider->testConnection().ok) throw std::runtime_error("缺少 assets/mock-demo.wav，请通过 --assets 指定资源目录");
        GenerationQueue queue(db, paths.data, std::move(provider), &settings);
        httplib::Server server;
        configureServer(server, db, queue, settings, textJobs, paths, web);
        if (!server.bind_to_port("127.0.0.1", 18080)) throw std::runtime_error("无法监听 127.0.0.1:18080，请检查端口占用");
        std::thread stopper([&] { int signal = 0; sigwait(&signals, &signal); server.stop(); });
        std::cout << "MuseFlow 0.4.0 | 本地场景配乐工作台 | http://127.0.0.1:18080\n" << std::flush;
        if (openBrowser) {
            pid_t child;
            char command[] = "xdg-open", url[] = "http://127.0.0.1:18080";
            char* args[]{command, url, nullptr};
            if (posix_spawnp(&child, command, nullptr, nullptr, args, environ) == 0) {
                std::thread([child] { int status; waitpid(child, &status, 0); }).detach();
            } else std::cerr << "无法打开浏览器，请手动访问本地地址。\n";
        }
        const bool ok = server.listen_after_bind();
        pthread_kill(stopper.native_handle(), SIGTERM); stopper.join();
        return ok ? 0 : 1;
    } catch (const std::exception& error) { std::cerr << "启动失败：" << error.what() << '\n'; return 1; }
}
