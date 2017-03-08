#include "tcpcmdclient.h"

TcpCmdClient::TcpCmdClient(QObject *parent)
    :QObject(parent)
{
    setConnectionStatus(false);
}

bool TcpCmdClient::connectionStatus()
{
    return m_connectionStatus;
}

void TcpCmdClient::setConnectionStatus(bool connectionStatus)
{
    m_connectionStatus = connectionStatus;
}

void TcpCmdClient::onConnected()
{
    qDebug()<<__func__<<"connected to cmd server";
    //initDispatchTimer();
    setConnectionStatus(true);
    emit connectionStatusChanged();
}

void TcpCmdClient::onDisconnected()
{
    qDebug()<<__func__<<"disconnected from cmd server";
    setConnectionStatus(false);
    emit connectionStatusChanged();
    //tryConnectServer();
}

void TcpCmdClient::onStateChanged(QAbstractSocket::SocketState sta)
{
    qDebug()<<__func__<<"connection status changed"<<sta;
}

void TcpCmdClient::readyRead()
{
    qDebug()<<__func__<<"data is ready to ready from cmd server";
}

void TcpCmdClient::onReconnectCmdServer()
{

}

void TcpCmdClient::onCheckConnectionStatus()
{

}
