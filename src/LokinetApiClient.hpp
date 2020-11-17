#ifndef __LOKI_LOKINET_API_CLIENT_HPP__
#define __LOKI_LOKINET_API_CLIENT_HPP__

#include <QObject>
#include <QJSValue>
#include <QJsonObject>

#include <optional>

/**
 * A client that implements convenience wrappers around making specific
 * JSON-RPC requests to a lokinet daemon.
 *
 * These are meant to be invoked from QML; as such, they each take a `QJSValue`
 * object which should be a Javascript callback function. This function should
 * take two parameters: the message payload and an error, e.g.:
 *
 * ```javascript
 * apiClient.llarpAdminStart(function(response, err) {
 *     if (err) {
 *         console.log("Received error when trying to start lokinet daemon: ", err);
 *     }
 * });
 * ```
 *
 * TODO: abstract this and just take a string as the endpoint. let QML/Javascript
 *       do the JSON-y things.
 */
class LokinetApiClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LokinetApiClient);

public:

  using ReplyCallback = std::function<void(std::optional<std::string>)>;

  LokinetApiClient();

  /**
   * Invoke an endpoint.
   *
   * TODO: provide argument to be used for JSON-RPC params (if/when needed)
   *
   * @param endpoint should be the full API endpoint (e.g. "llarp.admin.status")
   *        to invoke
   * @param callback is a callback that will receive the reply or error
   * @return true on success, false otherwise
   */
  bool invoke(const std::string& endpoint, QJsonObject args, ReplyCallback callback);
  Q_INVOKABLE bool invoke(const std::string& endpoint, QJsonObject args, QJSValue callback);

  /**
   * The following functions are conveniences for invoking particular API
   * endpoints. Internally, they delegate to the above 'invoke()' functions.
   *
   * @param callback is an optional JS function to invoke on success
   * @return true if the asynchronous request could be made, false otherwise
   */
/*
  Q_INVOKABLE bool llarpAdminWakeup(QJSValue callback) {
  return invoke("llarp.admin.wakeup", callback);
  }
*/

  Q_INVOKABLE bool llarpVersion(QJSValue callback) {
    return invoke("llarp.version", QJsonObject{}, callback);
  }

  Q_INVOKABLE bool llarpAddExit(QString exitAddress, QString exitToken, QJSValue callback)
  {
    if(exitToken.isEmpty())
    {
      return invoke("llarp.exit", QJsonObject{{"exit", exitAddress}}, callback);
    }
    else
    {
      return invoke("llarp.exit", QJsonObject{{"exit", exitAddress}, {"token", exitToken}}, callback);
    }
  }

  Q_INVOKABLE bool llarpConfigSet(QString section, QString key, QString value, QJSValue callback)
  {
    QJsonObject obj, config;
    config[key] = value;
    obj[section] = config;
    return invoke("llarp.config", QJsonObject{{"override", obj}, {"reload", true}}, callback);
  }

  Q_INVOKABLE bool llarpDelExit(QJSValue callback)
  {
    return invoke("llarp.exit", QJsonObject{{"unmap", true}}, callback);
  }

  bool llarpAdminDie(ReplyCallback callback) {
    return invoke("llarp.halt", QJsonObject{}, callback);
  }
signals:
    void
    CallCallback(QJSValue callback, QJSValueList args);
};

#endif // __LOKI_LOKINET_API_CLIENT_HPP__
