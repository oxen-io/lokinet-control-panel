#include "ApiPoller.hpp"

#include <string>

#include <QObject>
#include <QDebug>

constexpr auto LOKI_DAEMON_URL = "http://localhost:1190/";

// ApiPoller Constructor
ApiPoller::ApiPoller() {
    m_lmq.start();
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
    m_rpcMethod = endpoint.toStdString();
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
    if (m_rpcMethod.empty()) {
      qDebug() << "Warning: No endpoint; call ApiPoller::setApiEndpoint() before polling";
      return;
    }
    if(not m_Conn.has_value())
    {
      m_Conn = m_lmq.connect_remote(LOKI_DAEMON_URL, [](auto &&){},
                                    [=](auto,auto msg) {
                                      qInfo() << std::string{msg}.c_str();
                                      m_Conn = std::nullopt;
                                    });
    }
    m_lmq.request(*m_Conn, m_rpcMethod,
        [=](bool success, std::vector<std::string> data)
        {
            if(success and not data.empty())
            {
              emit statusAvailable(QString::fromStdString(data[0]));
            }
        });
        
}
