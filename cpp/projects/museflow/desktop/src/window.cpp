#include "window.hpp"
#include "local_page.hpp"
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QProcessEnvironment>
#include <QStandardPaths>
#include <QStatusBar>
#include <QWebEngineDownloadRequest>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QUuid>

Window::Window(QString backend, QString assets, QString web) {
    setWindowTitle("MuseFlow"); setWindowIcon(QIcon(":/museflow.svg")); resize(1380, 920); setMinimumSize(850, 620);
    network_.setProxy(QNetworkProxy::NoProxy);
    auto* placeholder = new QLabel("正在启动 MuseFlow 本地工作台…", this); placeholder->setAlignment(Qt::AlignCenter); setCentralWidget(placeholder);
    auto* appMenu = menuBar()->addMenu("MuseFlow");
    auto* quit = appMenu->addAction("退出"); quit->setShortcut(QKeySequence::Quit); connect(quit, &QAction::triggered, this, &Window::close);
    auto* about = appMenu->addAction("关于"); connect(about, &QAction::triggered, this, [this] { QMessageBox::about(this, "关于 MuseFlow", "MuseFlow 0.4.0\n本地优先的 AI 场景配乐桌面工作台\nC++20 · Qt 6 · Vue 3\n项目与音频保存在本机。"); });
    backend_.setProgram(backend); backend_.setArguments({"--assets", assets, "--web", web});
    // Backend inherits model keys; the WebEngine subprocess environment was scrubbed in main.
    backendSession_ = QUuid::createUuid().toString(QUuid::WithoutBraces);
    auto environment = QProcessEnvironment::systemEnvironment();
    for (const auto& name : {QString("DEEPSEEK_API_KEY"), QString("GEMINI_API_KEY"), QString("SONGAPI_KEY"), QString("ELEVENLABS_API_KEY")}) {
        auto value = qApp->property(name.toUtf8().constData()).toString();
        if (!value.isEmpty()) environment.insert(name, value);
        qApp->setProperty(name.toUtf8().constData(), QVariant());
    }
    environment.insert("MUSEFLOW_PARENT_SESSION", backendSession_);
    backend_.setProcessEnvironment(environment);
    backend_.setStandardOutputFile(QProcess::nullDevice()); backend_.setStandardErrorFile(QProcess::nullDevice());
    connect(&backend_, &QProcess::errorOccurred, this, [this](QProcess::ProcessError) { if (!closing_) fail("无法启动本地服务，请检查 MuseFlow 安装文件及执行权限。"); });
    connect(&backend_, &QProcess::finished, this, [this](int, QProcess::ExitStatus) {
        if (closing_) { QTimer::singleShot(0, qApp, &QApplication::quit); return; }
        fail("本地服务已停止。可能已有开发服务占用 18080 端口，或配置、数据目录不可用。请关闭占用端口的服务后重新启动。");
    });
    connect(&readyTimer_, &QTimer::timeout, this, &Window::checkReady);
    connect(&backend_, &QProcess::started, this, [this] { backend_.setProcessEnvironment(QProcessEnvironment()); readyTimer_.start(200); });
    backend_.start();
}
Window::~Window() {
    closing_ = true;
    if (backend_.state() != QProcess::NotRunning) { backend_.terminate(); if (!backend_.waitForFinished(8000)) { backend_.kill(); backend_.waitForFinished(2000); } }
    // Profile must outlive its page.
    delete takeCentralWidget(); delete profile_;
}
void Window::fail(const QString& text) {
    readyTimer_.stop(); ready_ = false;
    if (view_) view_->setEnabled(false);
    auto* label = new QLabel(text, this); label->setWordWrap(true); label->setAlignment(Qt::AlignCenter);
    if (!view_) { delete takeCentralWidget(); setCentralWidget(label); }
    else { delete label; statusBar()->showMessage(text); }
}
void Window::checkReady() {
    if (++attempts_ > 100) { fail("本地服务启动超时，请检查 18080 端口和数据目录。"); return; }
    QNetworkRequest request(QUrl("http://127.0.0.1:18080/api/health")); request.setTransferTimeout(1000);
    auto* reply = network_.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        const auto value = QJsonDocument::fromJson(reply->readAll()).object(); const bool ok = reply->error() == QNetworkReply::NoError; reply->deleteLater();
        if (ready_ || closing_ || !ok || value["app"] != "MuseFlow" || value["desktopSession"] != backendSession_ || backend_.state() != QProcess::Running) return;
        // The launched backend checks port ownership before this readiness probe is accepted.
        ready_ = true; readyTimer_.stop();
        profile_ = new QWebEngineProfile(this); // No storage name: memory-only cookies, cache and local storage.
        profile_->setHttpCacheType(QWebEngineProfile::MemoryHttpCache);
        profile_->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
        profile_->setUrlRequestInterceptor(new LocalRequests(profile_));
        view_ = new QWebEngineView(this); auto* page = new LocalPage(profile_, view_); view_->setPage(page);
        page->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, false);
        page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, false);
        page->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false);
        connect(page, &QWebEnginePage::renderProcessTerminated, this, [this](auto, int) { fail("界面渲染进程已停止，请重新启动 MuseFlow；已保存的作品仍在本机。"); });
        connect(profile_, &QWebEngineProfile::downloadRequested, this, [this](QWebEngineDownloadRequest* download) {
            auto file = QFileDialog::getSaveFileName(this, "保存 MuseFlow 文件", QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).filePath(download->suggestedFileName()));
            if (file.isEmpty()) { download->cancel(); return; }
            download->setDownloadDirectory(QFileInfo(file).absolutePath()); download->setDownloadFileName(QFileInfo(file).fileName());
            ++downloads_; statusBar()->showMessage("正在保存文件…");
            connect(download, &QWebEngineDownloadRequest::isFinishedChanged, this, [this, download] {
                if (!download->isFinished()) return;
                --downloads_; statusBar()->showMessage(download->state() == QWebEngineDownloadRequest::DownloadCompleted ? "文件已保存" : "文件保存失败或已取消", 6000);
            });
            download->accept();
        });
        delete takeCentralWidget(); setCentralWidget(view_); view_->setUrl(QUrl("http://127.0.0.1:18080/studio"));
    });
}
void Window::closeEvent(QCloseEvent* event) {
    if (closing_ || !ready_ || !view_) { event->ignore(); shutdown(); return; }
    event->ignore(); if (checkingClose_) return; checkingClose_ = true;
    view_->page()->runJavaScript("Boolean(window.__museflowUnsaved)", [this](const QVariant& dirty) {
        if (closing_) return;
        QNetworkRequest request(QUrl("http://127.0.0.1:18080/api/activity")); request.setTransferTimeout(1500);
        auto* reply = network_.get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, dirty] {
            const auto value = QJsonDocument::fromJson(reply->readAll()).object(); const bool unknown = reply->error() != QNetworkReply::NoError; reply->deleteLater(); checkingClose_ = false;
            if (closing_) return;
            if (dirty.toBool() || downloads_ || unknown || value["musicTasks"].toInt() || value["textTasks"].toInt()) {
                const auto answer = QMessageBox::question(this, "退出 MuseFlow", "有未保存修改、下载或后台任务（或无法确认任务状态）。退出将丢弃未保存修改并停止本机任务；远端音乐请求可能仍在执行和计费。确定退出？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (answer != QMessageBox::Yes) return;
            }
            shutdown();
        });
    });
}
void Window::shutdown() {
    if (closing_) return;
    closing_ = true; readyTimer_.stop(); hide();
    if (backend_.state() == QProcess::NotRunning) { QTimer::singleShot(0, qApp, &QApplication::quit); return; }
    backend_.terminate();
    QTimer::singleShot(8000, this, [this] { if (backend_.state() != QProcess::NotRunning) backend_.kill(); });
}
