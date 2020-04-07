#ifndef __LOKI_STAT_FETCHER_H__
#define __LOKI_STAT_FETCHER_H__

#include <unordered_map>
#include <functional>
#include <mutex>
#include <string>

#include <QObject>
#include <QTimer>

#include "HttpClient.hpp"

/**
 * The ApiPoller periodically requests a JSON-RPC endpoint from the Loki daemon.
 */
class ApiPoller : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ApiPoller);

    static constexpr int DEFAULT_POLLING_INTERVAL_MS = 3000;

public:

    ApiPoller();
    ~ApiPoller();

    /**
     * Set the API endpoint.
     *
     * NOTE: must be called before any polling occurs. The QML engine prevents
     * us from being able to pass this in the constructor.
     */
    Q_INVOKABLE void setApiEndpoint(const QString& endpoint);

    /**
     * Set the polling interval
     *
     * @param intervalMs is the new interval in milliseconds
     */
    Q_INVOKABLE void setIntervalMs(int intervalMs);

    /**
     * Start the periodic polling
     */
    Q_INVOKABLE void startPolling();

    /**
     * Stop the periodic polling
     */
    Q_INVOKABLE void stopPolling();

    /**
     * Poll immediately. This polls as soon as the underlying QTimer is able
     * to fire its *timeout* signal. This does not affect periodic polling.
     */
    Q_INVOKABLE void pollImmediately();
    Q_INVOKABLE void pollOnce();

signals:

    /**
     * Emitted when a JSON-RPC seponse containing new status information is
     * received from the server
     *
     * @param payload is the payload received from the server
     * @param error is a QNetworkReply::NetworkError enum representing the
     *              error, if any
     */
    void statusAvailable(const QByteArray& payload, QNetworkReply::NetworkError error);

private:

    /**
     * Poll the Loki daemon.
     */
    void pollDaemon();
    void watchDog();

    QTimer* m_timer;
    bool m_responded;
    QTimer* m_timeout_timer;
    HttpClient m_httpClient;
    std::string m_rpcPayload;
};

#endif // __LOKI_STAT_FETCHER_H__
