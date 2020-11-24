#include "LokinetApiClient.hpp"
#include "lmq_settings.hpp"
#include <QJsonDocument>
#include <stdexcept>
#include <cstdio>
#include <QDebug>

bool LokinetApiClient::invoke(const std::string& endpoint, QJsonObject args, ReplyCallback callback) {
  std::cout << "call " << endpoint;
  QJsonDocument doc(args);
  const auto req = doc.toJson();
  lmq.request(
    lmq_conn,
    std::string_view{endpoint},
    [cb = callback](bool success, std::vector<std::string> data)
    {
      if(success and not data.empty())
      {
        cb(std::move(data[0]));
      }
      else
      {
        cb(std::nullopt);
      }
    }, req.toStdString());
    return true;
}

Q_INVOKABLE bool LokinetApiClient::invoke(const std::string& endpoint,QJsonObject callargs, QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
      qWarning() << "callback should be a function (if present)";
        return false;
    }
    return invoke(endpoint, callargs,[=](std::optional<std::string> reply) mutable {
        QJSValueList args;
        if(reply.has_value())
        {
          args << QJSValue(reply->c_str());
          args << QJSValue(false);
        }
        else
        {
          args << QJSValue(false);
          args << QJSValue("no response given from lokinet");
        }
        emit CallCallback(callback, args);
    });
}


LokinetApiClient::LokinetApiClient() : QObject(nullptr)
{
  connect(this, &LokinetApiClient::CallCallback, this, [](auto callback, auto args){ callback.call(args); });
}
