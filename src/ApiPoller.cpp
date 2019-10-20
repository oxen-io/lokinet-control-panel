#include "ApiPoller.hpp"

#include <string>

#include <QObject>

constexpr auto LOKI_DAEMON_URL = "http://localhost:1190/";

// ApiPoller Constructor
ApiPoller::ApiPoller() {
    m_timer = new QTimer();
    m_timer->setInterval(DEFAULT_POLLING_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &ApiPoller::pollDaemon);
}

// ApiPoller Destructor
ApiPoller::~ApiPoller() {
    delete m_timer;
    m_timer = nullptr;
}

// ApiPoller::setApiEndpoint
void ApiPoller::setApiEndpoint(const QString& endpoint) {
    // we make the same API call every time, so build our payload once
    char buffer[256];
    snprintf(buffer, sizeof(buffer), R"JSON({
            "jsonrpc": "2.0",
            "method": "%s",
            "params": {},
            "id": "empty"
        })JSON",
        endpoint.data());
    m_rpcPayload = buffer;
}

// ApiPoller::setIntervalMs
void ApiPoller::setIntervalMs(int intervalMs) {
    m_timer->setInterval(intervalMs);
}

// ApiPoller::startPolling
void ApiPoller::startPolling() {
    m_timer->start();
}

// ApiPoller::stopPolling
void ApiPoller::stopPolling() {
    m_timer->stop();
}

// ApiPoller::pollImmediately
void ApiPoller::pollImmediately() {
    QTimer::singleShot(0, this, &ApiPoller::pollDaemon);
}

// ApiPoller::pollDaemon
void ApiPoller::pollDaemon() {
    if (m_rpcPayload.empty()) {
        qDebug() << "Warning: No endpoint; call ApiPoller::setApiEndpoint() before polling";
        return;
    }
    m_httpClient.postJson(LOKI_DAEMON_URL, m_rpcPayload, [=](QNetworkReply* reply) {
        if (reply->error()) {
            qDebug() << "JSON-RPC error: " << reply->error();
        }
        emit statusAvailable(reply->readAll(), reply->error());
    });
}
