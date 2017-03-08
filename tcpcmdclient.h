#ifndef TCPCMDCLIENT_H
#define TCPCMDCLIENT_H

#include "header.h"

class TcpCmdClient : public QObject
{
    Q_OBJECT
public:
    TcpCmdClient(QObject *parent = 0);

    /***
     * interface to qml
     ***/
    Q_PROPERTY(bool connectionStatus READ connectionStatus WRITE setConnectionStatus NOTIFY connectionStatusChanged)




    bool connectionStatus();
    void setConnectionStatus(bool connectionStatus);

signals:
    void connectionStatusChanged();
public slots:

private:
    ///private methods
    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState);
    void readyRead();
    void onReconnectCmdServer();
    void onCheckConnectionStatus();
    ///private variables
    bool m_connectionStatus;
    QTcpSocket *m_tcpsocket;
};

#endif // TCPCMDCLIENT_H
