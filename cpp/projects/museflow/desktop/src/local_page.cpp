#include "local_page.hpp"
#include <QWebEngineUrlRequestInfo>
namespace {
bool local(const QUrl& url) { return url.scheme() == "http" && url.host() == "127.0.0.1" && url.port() == 18080 && url.userInfo().isEmpty(); }
}
void LocalRequests::interceptRequest(QWebEngineUrlRequestInfo& info) {
    const auto url = info.requestUrl();
    if (!local(url) && url.scheme() != "data" && url.scheme() != "blob" && url != QUrl("about:blank")) info.block(true);
}
bool LocalPage::acceptNavigationRequest(const QUrl& url, NavigationType, bool) { return local(url) || url == QUrl("about:blank"); }
