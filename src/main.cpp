#include <QApplication>
#include <QQmlApplicationEngine>

#include "QmlClipboardAdapter.hpp"
#include "LokinetApiClient.hpp"
#include "StatFetcher.hpp"

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    qDebug() << "------ Lokinet Control Panel ------";
    qDebug() << "Compile-time Qt Version:" <<  QT_VERSION_STR;
    qDebug() << "Run-time Qt Version:" <<  qVersion();

    qmlRegisterType<LokinetApiClient>("LokinetApiClient", 1, 0, "LokinetApiClient");
    qmlRegisterType<QmlClipboardAdapter>("QClipboard", 1, 0, "QClipboard");
    qmlRegisterType<StatFetcher>("StatFetcher", 1, 0, "StatFetcher");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/res/qml/main.qml")));

    return app.exec();
}
