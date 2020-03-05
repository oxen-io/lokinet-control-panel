#include "HttpClient.hpp"

#include <QObject>

#include <memory>

// QNetworkAccessManager isn't threadsafe, and won't allow access in different threads
// (simultaneously or otherwise). So we use thread_local and lazily create a QNAM for
// each thread.
//
// TODO: this will probably have issues with ownership/lifetime. in particular,
// when an HttpClient is destroyed, there will be lingering QNAMs which have remaining
// signals connected (that is, they will try to use 'this' inproperly)
thread_local std::unique_ptr<QNetworkAccessManager> t_networkManager;
QNetworkAccessManager* HttpClient::networkManager() {
    if (not t_networkManager) {
        t_networkManager = std::make_unique<QNetworkAccessManager>();
        QObject::connect(
                t_networkManager.get(), &QNetworkAccessManager::finished,
                this, &HttpClient::handleResponse);
    }
    return t_networkManager.get();
}

uint32_t HttpClient::m_lastCallbackId = 0;
std::mutex HttpClient::m_callbackMutex;
std::unordered_map<uint32_t, HttpClient::ReplyCallback> HttpClient::m_callbackMap;

void HttpClient::handleResponse(QNetworkReply* reply) {
    uint32_t callbackId = reply->property("callbackId").toUInt();
    // qDebug() << "reply for callbackId " << callbackId << " is in";

    std::unique_lock<std::mutex> lock(m_callbackMutex);

    auto itr = m_callbackMap.find(callbackId);
    if (itr == m_callbackMap.end()) {
        qDebug() << "Response is about to bomb: " << reply->readAll();
    }
    Q_ASSERT(itr != m_callbackMap.end());

    // pull our callback out of the map
    auto callback = itr->second;
    // qDebug() << "erasing callbackId " << itr->first;
    m_callbackMap.erase(itr);

    lock.unlock();

    callback(reply);

    reply->deleteLater();
}

// HttpClient Constructor
HttpClient::HttpClient() = default;

// HttpClient Destructor
HttpClient::~HttpClient() = default;

// HttpClient::request
void HttpClient::postJson(const std::string& url, const std::string& payload, ReplyCallback callback) {

    std::lock_guard<std::mutex> guard(m_callbackMutex);

    m_lastCallbackId++;
    // qDebug() << "post() established callbackId " << m_lastCallbackId;
    m_callbackMap[m_lastCallbackId] = callback;

    QNetworkRequest request;
    request.setUrl(QUrl(url.c_str()));

    // qDebug() << "POSTing to " << url.c_str();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Host", "localhost");

    QNetworkReply* reply = networkManager()->post(request, QByteArray(payload.c_str()));

    // set our callbackId on this reply so that we can correlate the response with the callback
    reply->setProperty("callbackId", QVariant(m_lastCallbackId));

}

// HttpClient::request
void HttpClient::get(const std::string& url, ReplyCallback callback) {

    std::lock_guard<std::mutex> guard(m_callbackMutex);

    // TODO: DRY
    m_lastCallbackId++;
    // qDebug() << "get() established callbackId " << m_lastCallbackId;
    m_callbackMap[m_lastCallbackId] = callback;

    QNetworkRequest request;
    request.setUrl(QUrl(url.c_str()));

    request.setRawHeader("Host", "localhost");

    QNetworkReply* reply = networkManager()->get(request);

    // set our callbackId on this reply so that we can correlate the response with the callback
    reply->setProperty("callbackId", QVariant(m_lastCallbackId));

}
 
