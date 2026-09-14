#pragma once
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QTimer>
class QWebEngineView;
class QWebEngineProfile;
class Window final : public QMainWindow {
public:
    Window(QString backend, QString assets, QString web);
    ~Window() override;
    void shutdown();
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void checkReady();
    void fail(const QString& text);
    QWebEngineView* view_ = nullptr;
    QWebEngineProfile* profile_ = nullptr;
    QString backendSession_;
    QProcess backend_;
    QNetworkAccessManager network_;
    QTimer readyTimer_;
    int attempts_ = 0, downloads_ = 0;
    bool ready_ = false, closing_ = false, checkingClose_ = false;
};
