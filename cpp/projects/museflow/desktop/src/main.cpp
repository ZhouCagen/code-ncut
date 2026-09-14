#include "window.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QCryptographicHash>
#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QLockFile>
#include <QMessageBox>
#include <QSocketNotifier>
#include <QStandardPaths>
#include <QFileInfo>
#include <fcntl.h>
#include <unistd.h>
#include <csignal>
#include <sys/stat.h>
namespace {
int signalWrite = -1;
void signalHandler(int) { const char byte = 1; if (signalWrite >= 0) { auto unused = write(signalWrite, &byte, 1); (void)unused; } }
}
int main(int argc, char** argv) {
    umask(0077);
    // Never give environment API keys to the GUI renderer. Transfer only to the backend process.
    auto textKey = qgetenv("DEEPSEEK_API_KEY"), musicKey = qgetenv("GEMINI_API_KEY"), songapiKey = qgetenv("SONGAPI_KEY"), elevenlabsKey = qgetenv("ELEVENLABS_API_KEY");
    qunsetenv("DEEPSEEK_API_KEY"); qunsetenv("GEMINI_API_KEY"); qunsetenv("SONGAPI_KEY"); qunsetenv("ELEVENLABS_API_KEY");
    QApplication app(argc, argv); app.setApplicationName("MuseFlow"); app.setOrganizationName("MuseFlow"); app.setApplicationVersion("0.4.0");
    app.setDesktopFileName("museflow"); app.setQuitOnLastWindowClosed(false);
    app.setProperty("DEEPSEEK_API_KEY", QString::fromUtf8(textKey)); app.setProperty("GEMINI_API_KEY", QString::fromUtf8(musicKey));
    app.setProperty("SONGAPI_KEY", QString::fromUtf8(songapiKey)); songapiKey.fill(0); songapiKey.clear();
    app.setProperty("ELEVENLABS_API_KEY", QString::fromUtf8(elevenlabsKey)); elevenlabsKey.fill(0); elevenlabsKey.clear();
    textKey.fill(0); textKey.clear(); musicKey.fill(0); musicKey.clear();
    QCommandLineParser parser; parser.setApplicationDescription("MuseFlow 本地音乐创作桌面应用"); parser.addHelpOption(); parser.addVersionOption();
    parser.addOptions({{"backend", "Backend executable", "file", MUSEFLOW_BACKEND_PATH}, {"assets", "Audio assets", "dir", MUSEFLOW_PROJECT_DIR "/assets"}, {"web", "Built interface", "dir", MUSEFLOW_PROJECT_DIR "/frontend/dist"}}); parser.process(app);
    const auto runtime = QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation);
    QDir().mkpath(runtime);
    const QString socketName = runtime + "/museflow-desktop";
    QLockFile lock(socketName + ".lock"); lock.setStaleLockTime(0);
    if (!lock.tryLock(0)) {
        QLocalSocket existing; existing.connectToServer(socketName);
        if (existing.waitForConnected(1500)) { existing.write("activate"); existing.waitForBytesWritten(500); return 0; }
        QMessageBox::warning(nullptr, "MuseFlow", "MuseFlow 正在启动或退出，请稍后重试。"); return 1;
    }
    QLocalServer::removeServer(socketName); QLocalServer server; server.setSocketOptions(QLocalServer::UserAccessOption);
    if (!server.listen(socketName)) { QMessageBox::critical(nullptr, "MuseFlow", "无法创建桌面应用的单实例通信通道。"); return 1; }
    if (!QFileInfo::exists(parser.value("web") + "/index.html") || !QFileInfo(parser.value("backend")).isExecutable()) {
        QMessageBox::critical(nullptr, "MuseFlow", "缺少已构建的程序或界面，请先运行 scripts/build.sh。"); return 1;
    }
    Window window(parser.value("backend"), parser.value("assets"), parser.value("web"));
    QObject::connect(&server, &QLocalServer::newConnection, &window, [&] {
        while (auto* connection = server.nextPendingConnection()) { connection->disconnectFromServer(); connection->deleteLater(); }
        window.showNormal(); window.raise(); window.activateWindow();
    });
    int pipes[2];
    if (pipe2(pipes, O_NONBLOCK | O_CLOEXEC)) return 1;
    signalWrite = pipes[1]; QSocketNotifier notifier(pipes[0], QSocketNotifier::Read);
    QObject::connect(&notifier, &QSocketNotifier::activated, &window, [&] { char buffer[16]; while (read(pipes[0], buffer, sizeof(buffer)) > 0) {} window.shutdown(); });
    struct sigaction action{}; action.sa_handler = signalHandler; sigemptyset(&action.sa_mask); sigaction(SIGTERM, &action, nullptr); sigaction(SIGINT, &action, nullptr);
    window.show(); int result = app.exec(); signalWrite = -1; close(pipes[0]); close(pipes[1]); return result;
}
