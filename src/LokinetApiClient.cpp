#include "LokinetApiClient.hpp"

#include <stdexcept>

// LokinetApiClient::llarpAdminStart
bool LokinetApiClient::llarpAdminStart(QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
        qDebug() << "callback should be a function (if present)";
        return false;
    }

    const std::string jsonRpcPayload = R"JSON({
            "jsonrpc": "2.0",
            "method": "llarp.admin.start",
            "params": {},
            "id": "empty"
        })JSON";
    m_httpClient.postJson("http://localhost:1190", jsonRpcPayload, [=](QNetworkReply* reply) mutable {
        QJSValueList args;
        args << QJSValue(reply->readAll().data());
        args << QJSValue(reply->error());
        callback.call(args);
    });

    return true;
}
