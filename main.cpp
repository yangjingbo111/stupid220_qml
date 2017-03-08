#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "tcpdataclient.h"
#include "tcpcmdclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    ///register c++ type to qml world
    qmlRegisterType<TcpDataClient>("TcpDataClient",1,0,"TcpDataClient");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
