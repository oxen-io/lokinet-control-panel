#include "StatFetcher.hpp"

#include <QObject>

constexpr auto LOKI_DAEMON_URL = "http://localhost:1190/";

// StatFetcher Constructor
StatFetcher::StatFetcher() {
    m_timer = new QTimer();
    m_timer->setInterval(DEFAULT_POLLING_INTERVAL_MS);
    connect(m_timer, &QTimer::timeout, this, &StatFetcher::pollDaemon);
}

// StatFetcher Destructor
StatFetcher::~StatFetcher() {
    delete m_timer;
    m_timer = nullptr;
}

// StatFetcher::setIntervalMs
void StatFetcher::setIntervalMs(int intervalMs) {
    m_timer->setInterval(intervalMs);
}

// StatFetcher::startPolling
void StatFetcher::startPolling() {
    m_timer->start();
}

// StatFetcher::stopPolling
void StatFetcher::stopPolling() {
    m_timer->stop();
}

// StatFetcher::pollImmediately
void StatFetcher::pollImmediately() {
    QTimer::singleShot(0, this, &StatFetcher::pollDaemon);
}

// StatFetcher::pollDaemon
void StatFetcher::pollDaemon() {
    const std::string jsonRpcPayload = R"JSON({
            "jsonrpc": "2.0",
            "method": "llarp.admin.dumpstate",
            "params": {},
            "id": "empty"
        })JSON";
    m_httpClient.postJson(LOKI_DAEMON_URL, jsonRpcPayload, [=](QNetworkReply* reply) {
        if (reply->error()) {
            qDebug() << "JSON-RPC error: " << reply->error();
        }
        emit statusAvailable(reply->readAll(), reply->error());
    });
}
