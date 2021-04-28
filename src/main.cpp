#include <QApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QDebug>
#include <QtGlobal>

#include "QmlClipboardAdapter.hpp"
#include "LokinetApiClient.hpp"
#include "ApiPoller.hpp"
#include "PlatformDetails.hpp"
#include "BandwidthChartData.hpp"
#include "lmq_settings.hpp"

#if defined(SYSTEMD)
constexpr bool isSystemd = true;
#else
constexpr bool isSystemd = false;
#endif

oxenmq::OxenMQ lmq{};
std::optional<oxenmq::ConnectionID> lmq_conn;
std::string RPCURL{LOKINET_RPC_URL};

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    qDebug() << "------ Lokinet Control Panel ------";
    qDebug() << "Compile-time Qt Version:" <<  QT_VERSION_STR;
    qDebug() << "Run-time Qt Version:" <<  qVersion();

    // crude CLI option parsing
    bool nohide = true;
    bool notray = true;
    for (int i=0; argv[i] != nullptr; ++i) {
        std::string arg = argv[i];
        if (arg == "--hide") nohide = false;
        if (arg == "--tray") notray = false;
        if (arg == "--rpc" and argv[i+1] != nullptr)
        {
          RPCURL = argv[i+1];
        }
    }

    qDebug() << "nohide: " << (nohide ? "T":"F");
    qDebug() << "notray: " << (notray ? "T":"F");
    qRegisterMetaType<QJSValueList>("QJSValueList");
    qmlRegisterType<LokinetApiClient>("LokinetApiClient", 1, 0, "LokinetApiClient");
    qmlRegisterType<QmlClipboardAdapter>("QClipboard", 1, 0, "QClipboard");
    qmlRegisterType<ApiPoller>("ApiPoller", 1, 0, "ApiPoller");
    qmlRegisterType<PlatformDetails>("PlatformDetails", 1, 0, "PlatformDetails");
    qmlRegisterType<BandwidthChartData>("BandwidthChartData", 1, 0, "BandwidthChartData");

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    
    lmq.start();
    lmq.add_timer([]() {
      if(lmq_conn.has_value())
        return;
      lmq.connect_remote(
        RPCURL,
        [](oxenmq::ConnectionID conn)
        {
          lmq_conn = conn;
        },
        [](auto, auto)
        {
          lmq_conn = std::nullopt;
        });
    }, std::chrono::milliseconds{500});
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
