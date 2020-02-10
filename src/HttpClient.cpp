#include "HttpClient.hpp"

#include <QObject>

// HttpClient Constructor
HttpClient::HttpClient() {
    m_networkManager = new QNetworkAccessManager();
    QObject::connect(m_networkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {

        uint32_t callbackId = reply->property("callbackId").toUInt();

        std::unique_lock<std::mutex> lock(m_callbackMutex);

        auto itr = m_callbackMap.find(callbackId);
        Q_ASSERT(itr != m_callbackMap.end());

        // pull our callback out of the map
        auto callback = itr->second;
        m_callbackMap.erase(itr);

        lock.unlock();

        callback(reply);

        reply->deleteLater();
    });
}

// HttpClient Destructor
HttpClient::~HttpClient() {
    delete m_networkManager;
    m_networkManager = nullptr;
}

// HttpClient::request
void HttpClient::postJson(const std::string& url, const std::string& payload, ReplyCallback callback) {

    std::lock_guard<std::mutex> guard(m_callbackMutex);

    m_lastCallbackId++;
    m_callbackMap[m_lastCallbackId] = callback;

    QNetworkRequest request;
    request.setUrl(QUrl(url.c_str()));

    // qDebug() << "POSTing to " << url.c_str();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = m_networkManager->post(request, QByteArray(payload.c_str()));

    // set our callbackId on this reply so that we can correlate the response with the callback
    reply->setProperty("callbackId", QVariant(m_lastCallbackId));

}

// HttpClient::request
void HttpClient::get(const std::string& url, ReplyCallback callback) {

    std::lock_guard<std::mutex> guard(m_callbackMutex);

    // TODO: DRY
    m_lastCallbackId++;
    m_callbackMap[m_lastCallbackId] = callback;

    QNetworkRequest request;
    request.setUrl(QUrl(url.c_str()));

    QNetworkReply* reply = m_networkManager->get(request);

    // set our callbackId on this reply so that we can correlate the response with the callback
    reply->setProperty("callbackId", QVariant(m_lastCallbackId));

}
 
