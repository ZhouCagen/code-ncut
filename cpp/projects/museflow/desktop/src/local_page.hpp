#pragma once
#include <QWebEnginePage>
#include <QWebEngineUrlRequestInterceptor>
class LocalRequests final : public QWebEngineUrlRequestInterceptor {
public:
    using QWebEngineUrlRequestInterceptor::QWebEngineUrlRequestInterceptor;
    void interceptRequest(QWebEngineUrlRequestInfo& info) override;
};
class LocalPage final : public QWebEnginePage {
public:
    using QWebEnginePage::QWebEnginePage;
protected:
    bool acceptNavigationRequest(const QUrl& url, NavigationType, bool) override;
    void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel, const QString&, int, const QString&) override {}
};
