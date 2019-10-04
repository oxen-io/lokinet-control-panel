#include <QApplication>
#include <QQmlApplicationEngine>

int32_t main(int32_t argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/res/qml/main.qml")));

    return app.exec();
}
