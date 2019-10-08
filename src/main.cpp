#include <QApplication>
#include <QQmlApplicationEngine>

#include "QmlClipboardAdapter.hpp"
#include "StatFetcher.hpp"

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    qmlRegisterType<QmlClipboardAdapter>("QClipboard", 1, 0, "QClipboard");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/res/qml/main.qml")));

    StatFetcher statFetcher;

    return app.exec();
}
