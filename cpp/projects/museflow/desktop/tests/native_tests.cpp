#include "window.hpp"
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QTcpServer>
#include <QDir>
#include <QElapsedTimer>
#include <memory>
#include <sys/stat.h>

QVariant javascript(QWebEngineView* view, const QString& code) {
    struct State { bool done = false; QVariant value; };
    auto state = std::make_shared<State>();
    view->page()->runJavaScript(code, [state](const QVariant& value) { state->value = value; state->done = true; });
    QElapsedTimer timer; timer.start();
    while (!state->done && timer.elapsed() < 3000) QTest::qWait(10);
    return state->value;
}
class NativeTests : public QObject {
    Q_OBJECT
private slots:
    void windowDownloadsAndClose() {
        Window window(MUSEFLOW_BACKEND_PATH, MUSEFLOW_PROJECT_DIR "/assets", MUSEFLOW_PROJECT_DIR "/frontend/dist"); window.show();
        QTRY_VERIFY_WITH_TIMEOUT(window.findChild<QWebEngineView*>() != nullptr, 12000);
        auto* view = window.findChild<QWebEngineView*>();
        QTRY_VERIFY_WITH_TIMEOUT(javascript(view, "document.querySelector('h1')?.textContent || ''").toString().contains("从一个场景"), 15000);
        QVERIFY(!javascript(view, "window.__museflowUnsaved").toBool());
        // A second desktop window must never attach to the first window's backend instance.
        {
            Window foreign(MUSEFLOW_BACKEND_PATH, MUSEFLOW_PROJECT_DIR "/assets", MUSEFLOW_PROJECT_DIR "/frontend/dist");
            foreign.show(); QTest::qWait(700); QVERIFY(foreign.findChild<QWebEngineView*>() == nullptr);
        }
        javascript(view, R"JS((async () => {
            const api = async (path, body) => (await fetch('/api' + path, {method: 'POST', headers: {'Content-Type':'application/json'}, body: JSON.stringify(body)})).json();
            const {blueprint} = await api('/blueprints/generate', {userInput: '桌面原生交互测试', purpose: 'game'});
            const project = await api('/projects', {name: '原生窗口项目', blueprint});
            const task = await api('/generations', {projectId: project.id, blueprint, mode: 'preview'});
            window.__nativeTask = task.id; window.__nativeProject = project.id;
        })())JS");
        QTRY_VERIFY_WITH_TIMEOUT(!javascript(view, "window.__nativeTask || ''").toString().isEmpty(), 3000);
        bool closePrompt = false;
        QTimer dismiss; dismiss.setInterval(20);
        connect(&dismiss, &QTimer::timeout, [&] {
            for (auto* widget : QApplication::topLevelWidgets()) if (auto* message = qobject_cast<QMessageBox*>(widget); message && message->isVisible()) {
                closePrompt = true; message->button(QMessageBox::No)->click(); dismiss.stop();
            }
        });
        dismiss.start(); window.close();
        QTRY_VERIFY_WITH_TIMEOUT(closePrompt, 3000); QVERIFY(window.isVisible());
        javascript(view, R"JS((async () => { for(let i=0;i<100;i++) { const task=await (await fetch('/api/generations/'+window.__nativeTask)).json(); if(task.status==='completed'){ window.__nativeDone=true; return } await new Promise(resolve=>setTimeout(resolve,100)); } })())JS");
        QTRY_VERIFY_WITH_TIMEOUT(javascript(view, "Boolean(window.__nativeDone)").toBool(), 15000);
        QTemporaryDir downloads; QVERIFY(downloads.isValid());
        const auto save = [&](const QString& name, const QString& pathExpression) {
            bool shown = false;
            QTimer choose; choose.setInterval(20);
            connect(&choose, &QTimer::timeout, [&] {
                for (auto* widget : QApplication::topLevelWidgets()) if (auto* dialog = qobject_cast<QFileDialog*>(widget); dialog && dialog->isVisible()) {
                    shown = true; dialog->selectFile(downloads.filePath(name));
                    QMetaObject::invokeMethod(dialog, "accept", Qt::DirectConnection); choose.stop();
                }
            });
            choose.start(); javascript(view, "{ const link=document.createElement('a'); link.href=" + pathExpression + "; link.download=''; document.body.append(link); link.click(); link.remove(); }");
            QElapsedTimer timer; timer.start();
            while ((!shown || !QFileInfo::exists(downloads.filePath(name)) || QFileInfo(downloads.filePath(name)).size() == 0) && timer.elapsed() < 10000) QTest::qWait(20);
            return shown && QFileInfo(downloads.filePath(name)).size() > 0;
        };
        QVERIFY(save("native-audio.wav", "'/api/generations/'+window.__nativeTask+'/audio?download=1'"));
        QFile audio(downloads.filePath("native-audio.wav")); QVERIFY(audio.open(QIODevice::ReadOnly)); QCOMPARE(audio.read(4), QByteArray("RIFF"));
        QVERIFY(save("native-project.json", "'/api/projects/'+window.__nativeProject+'/export'"));
        QFile project(downloads.filePath("native-project.json")); QVERIFY(project.open(QIODevice::ReadOnly)); QVERIFY(project.readAll().contains("museflow-project"));
        // Unsubmitted editor content must also trigger the native close confirmation.
        javascript(view, "window.__museflowUnsaved=true"); closePrompt = false; dismiss.start(); window.close();
        QTRY_VERIFY_WITH_TIMEOUT(closePrompt, 3000); QVERIFY(window.isVisible());
        javascript(view, "window.__museflowUnsaved=false");
        QDir().mkpath(MUSEFLOW_PROJECT_DIR "/frontend/test-results");
        QVERIFY(window.grab().save(MUSEFLOW_PROJECT_DIR "/frontend/test-results/desktop-native-window.png"));
        window.close(); QTRY_VERIFY_WITH_TIMEOUT(!window.isVisible(), 3000);
        QTcpServer probe;
        QTRY_VERIFY_WITH_TIMEOUT(probe.isListening() || probe.listen(QHostAddress::LocalHost, 18080), 10000);
    }
};
int main(int argc, char** argv) {
    umask(0077); QTemporaryDir directory;
    if (!directory.isValid()) return 1;
    for (const auto& part : {"data", "config", "cache", "runtime"}) QDir().mkpath(directory.filePath(part));
    qputenv("XDG_DATA_HOME", directory.filePath("data").toUtf8()); qputenv("XDG_CONFIG_HOME", directory.filePath("config").toUtf8()); qputenv("XDG_CACHE_HOME", directory.filePath("cache").toUtf8()); qputenv("XDG_RUNTIME_DIR", directory.filePath("runtime").toUtf8());
    qunsetenv("DEEPSEEK_API_KEY"); qunsetenv("GEMINI_API_KEY"); qunsetenv("MUSEFLOW_GOOGLE_TEST_BASE"); qunsetenv("QTWEBENGINE_REMOTE_DEBUGGING");
    qputenv("QT_QPA_PLATFORM", "offscreen"); qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu");
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication app(argc, argv); app.setQuitOnLastWindowClosed(false);
    NativeTests tests; return QTest::qExec(&tests, argc, argv);
}
#include "native_tests.moc"
