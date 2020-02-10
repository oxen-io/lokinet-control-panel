#ifndef __LOKI_HTTP_CLIENT_H__
#define __LOKI_HTTP_CLIENT_H__

#include <unordered_map>
#include <functional>
#include <mutex>
 
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
 
/**
 * An HTTP Client that uses QNetworkAccessManager behind the scenes but allows
 * for responses and requests to be correlated (something that
 * QNetworkAccessManager doesn't explicitly do). Callers need only provide a
 * callback with a given request.
 */
class HttpClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(HttpClient);

    using ReplyCallback = std::function<void(QNetworkReply*)>;

public:

    HttpClient();
    ~HttpClient();

    /**
     * Make an HTTP POST request to the given URL. The 'content-type' header
     * will be set to 'application/json' with the given payload set as the POST
     * body.
     *
     * @param url is the URL to POST to
     * @param payload will be the POST body
     * @param callback is a function that will be called when a response or
     *                 error is received
     *
     */
    void postJson(const std::string& url, const std::string& payload, ReplyCallback callback);

    /**
     * Make a HTTP GET request to the given URL.
     *
     * @param url is the URL to GET
     * @param callback is a function that will be called when a response or
     *                 error is received
     */
    void get(const std::string& url, ReplyCallback callback);
 
private:
    QNetworkAccessManager* m_networkManager = nullptr;

    std::mutex m_callbackMutex;
    uint32_t m_lastCallbackId = 0;
    std::unordered_map<uint32_t, ReplyCallback> m_callbackMap;

};
 
#endif // __LOKI_HTTP_CLIENT_H__
