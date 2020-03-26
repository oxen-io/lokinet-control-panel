#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#include "QmlClipboardAdapter.hpp"
#include "LokinetApiClient.hpp"
#include "ApiPoller.hpp"
#include "PlatformDetails.hpp"
#include "BandwidthChartData.hpp"

#if defined(SYSTEMD)
constexpr bool isSystemd = true;
#else
constexpr bool isSystemd = false;
#endif

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    qDebug() << "------ Lokinet Control Panel ------";
    qDebug() << "Compile-time Qt Version:" <<  QT_VERSION_STR;
    qDebug() << "Run-time Qt Version:" <<  qVersion();

    // crude CLI option parsing
    bool nohide = false;
    bool notray = false;
    for (int i=0; argv[i] != nullptr; ++i) {
        std::string arg = argv[i];
        if (arg == "--nohide" || arg == "--no-hide") nohide = true;
        if (arg == "--notray" || arg == "--no-tray") notray = true;
    }

    // notray implies nohide
    if (notray)
        nohide = true;

    qDebug() << "nohide: " << (nohide ? "T":"F");
    qDebug() << "notray: " << (notray ? "T":"F");

    qmlRegisterType<LokinetApiClient>("LokinetApiClient", 1, 0, "LokinetApiClient");
    qmlRegisterType<QmlClipboardAdapter>("QClipboard", 1, 0, "QClipboard");
    qmlRegisterType<ApiPoller>("ApiPoller", 1, 0, "ApiPoller");
    qmlRegisterType<PlatformDetails>("PlatformDetails", 1, 0, "PlatformDetails");
    qmlRegisterType<BandwidthChartData>("BandwidthChartData", 1, 0, "BandwidthChartData");

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/res/images/icon.svg"));
    app.setQuitOnLastWindowClosed(false);
    QCoreApplication::setApplicationName("Lokinet Control Panel");

    QQmlApplicationEngine engine;
    engine.globalObject().setProperty("nohide", nohide);
    engine.globalObject().setProperty("notray", notray);
    engine.globalObject().setProperty("isSystemd", isSystemd);
    engine.load(QUrl(QStringLiteral("qrc:/res/qml/main.qml")));


    return app.exec();
}
