#include "LokinetApiClient.hpp"

#include <stdexcept>

// LokinetApiClient::llarpAdminWakeup
bool LokinetApiClient::llarpAdminWakeup(QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
        qDebug() << "callback should be a function (if present)";
        return false;
    }

    const std::string jsonRpcPayload = R"JSON({
            "jsonrpc": "2.0",
            "method": "llarp.admin.wakeup",
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

// LokinetApiClient::llarpVersion
bool LokinetApiClient::llarpVersion(QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
        qDebug() << "callback should be a function (if present)";
        return false;
    }

    const std::string jsonRpcPayload = R"JSON({
            "jsonrpc": "2.0",
            "method": "llarp.version",
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

// LokinetApiClient::llarpAdminStatus
bool LokinetApiClient::llarpAdminStatus(QJSValue callback) {

    if (! callback.isUndefined() && ! callback.isCallable()) {
        qDebug() << "callback should be a function (if present)";
        return false;
    }

    const std::string jsonRpcPayload = R"JSON({
            "jsonrpc": "2.0",
            "method": "llarp.admin.status",
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
