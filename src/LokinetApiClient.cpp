#include "LokinetApiClient.hpp"

#include <stdexcept>
#include <cstdio>

bool LokinetApiClient::invoke(const std::string& endpoint, ReplyCallback callback) {
  if(not m_lmqConnection.has_value())
  {
    m_lmqClient.start();
    m_lmqConnection =
      m_lmqClient.connect_remote(
        "tcp://127.0.0.1:1190",
        [](auto){},
        [&](auto, std::string_view reason) {
          // qDebug() << "failed to connect to lokinet: "<< reason;
          m_lmqConnection = std::nullopt;
        });
  }
  m_lmqClient.request(
    *m_lmqConnection,
    std::string_view{endpoint},
    [cb = std::move(callback)](bool success, std::vector<std::string> data)
    {
      if(success and not data.empty())
      {
        cb(std::move(data[0]));
      }
      else
      {
        cb(std::nullopt);
      }
    });
    return true;
}

Q_INVOKABLE bool LokinetApiClient::invoke(const std::string& endpoint, QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
      // qDebug() << "callback should be a function (if present)";
        return false;
    }

    return invoke(endpoint, [=](std::optional<std::string> reply) mutable {
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
        callback.call(args);
    });
}

