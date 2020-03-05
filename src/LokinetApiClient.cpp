#include "LokinetApiClient.hpp"

#include <stdexcept>
#include <cstdio>

bool LokinetApiClient::invoke(const std::string& endpoint, HttpClient::ReplyCallback callback) {

    char buffer[1024];
    int result = snprintf(
            buffer,
            sizeof(buffer),
            R"JSON({
                "jsonrpc": "2.0",
                "method": "%s",
                "params": {},
                "id": "empty"
            })JSON",
            endpoint.c_str());

    if (result < 0) {
        qDebug() << "snprintf failed: " << result;
        return false;
    }

    qDebug() << "invoking json rpc payload: " << buffer;

    m_httpClient.postJson("http://localhost:1190", buffer, std::move(callback));
    return true;
}

Q_INVOKABLE bool LokinetApiClient::invoke(const std::string& endpoint, QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
        qDebug() << "callback should be a function (if present)";
        return false;
    }

    return invoke(std::move(endpoint), [=](QNetworkReply* reply) mutable {
        QJSValueList args;
        args << QJSValue(reply->readAll().data());
        args << QJSValue(reply->error());
        callback.call(args);
    });
}

