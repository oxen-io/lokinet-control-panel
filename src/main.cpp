#include <QApplication>
#include <QQmlApplicationEngine>

#include "QmlClipboardAdapter.hpp"
#include "LokinetApiClient.hpp"
#include "ApiPoller.hpp"
#include "PlatformDetails.hpp"
#include "BandwidthChartData.hpp"

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    qDebug() << "------ Lokinet Control Panel ------";
    qDebug() << "Compile-time Qt Version:" <<  QT_VERSION_STR;
    qDebug() << "Run-time Qt Version:" <<  qVersion();

    // crude CLI option parsing
    bool nohide = false;
    for (int i=0; argv[i] != nullptr; ++i) {
        if (strcmp(argv[i], "--nohide") == 0) {
            qDebug() << "nohide: true";
            nohide = true;
        }
    }

    qmlRegisterType<LokinetApiClient>("LokinetApiClient", 1, 0, "LokinetApiClient");
    qmlRegisterType<QmlClipboardAdapter>("QClipboard", 1, 0, "QClipboard");
    qmlRegisterType<ApiPoller>("ApiPoller", 1, 0, "ApiPoller");
    qmlRegisterType<PlatformDetails>("PlatformDetails", 1, 0, "PlatformDetails");
    qmlRegisterType<BandwidthChartData>("BandwidthChartData", 1, 0, "BandwidthChartData");

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    QQmlApplicationEngine engine;
    engine.globalObject().setProperty("nohide", nohide);
    engine.load(QUrl(QStringLiteral("qrc:/res/qml/main.qml")));

    return app.exec();
}
