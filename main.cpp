#include <QApplication>
#include <QQmlApplicationEngine>

int32_t main(int32_t argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
