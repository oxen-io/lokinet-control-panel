#ifndef __LOKI_STAT_FETCHER_H__
#define __LOKI_STAT_FETCHER_H__

#include <unordered_map>
#include <functional>
#include <mutex>
 
#include <QObject>
#include <QTimer>

#include "HttpClient.hpp"
 
/**
 * The StatFetcher periodically requests stats from the Loki daemon.
 */
class StatFetcher : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(StatFetcher);

    static constexpr int DEFAULT_POLLING_INTERVAL_MS = 3000;

public:

    StatFetcher();
    ~StatFetcher();

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

    QTimer* m_timer;
    HttpClient m_httpClient;
};
 
#endif // __LOKI_STAT_FETCHER_H__
