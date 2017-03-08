#ifndef TCPDATACLIENT_H
#define TCPDATACLIENT_H

#include "header.h"


class TcpDataClient : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit TcpDataClient(QQuickItem *parent = 0);
    void paint(QPainter *painter);

    /***
     * interface to qml
     ***/
    Q_PROPERTY(bool connectionStatus READ connectionStatus WRITE setConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(int imageStatus READ imageStatus WRITE setImageStatus NOTIFY imageStatusChanged)

    Q_INVOKABLE void tryConnectDataServer();
    Q_INVOKABLE void mannuallyDisconnectFromServer();
    /***
     * end of interface
     ***/
    bool connectionStatus();
    void setConnectionStatus(bool connectionStatus);

    int imageStatus();
    void setImageStatus(int imageStatus);
signals:
    void connectionStatusChanged();
    void imageStatusChanged();
public slots:
    void onConnected();
    void onDisconnected();
    void onStateChanged(QAbstractSocket::SocketState);
    void readyRead();

    void onReconnectDataServer();
    void onCheckConnectionStatus();
    void dispatch_packet();  //分发数据包
private:
    ///private methods
    uint rgbFromWaveLength(double wave);
    void filter_repeat_packet(QByteArray array);
    void initDispatchTimer();

    void combineImage(QByteArray array);

    ///private variables
    QTcpSocket *m_tcpsocket;
    bool m_connectionStatus;
    int m_imageStatus;
    QTimer *m_checkConnectionStatusTimer;
    QList<QByteArray> m_packet_list;  //无重复的包列表
    quint8 m_burst_read_num;
    quint32 m_lost_cnt;   //丢包计数器
    quint32 m_recv_cnt;   //无重复包计数器
    QTimer *m_dispatch_timer;  //包分发定时器

    quint16 m_bscan_h;
    quint16 m_bscan_w;
    enum { ColormapSize = 256 };
    uint colormap[ColormapSize];
    QImage m_oldImage;
    QImage m_bscanImage;

};



#endif // TCPDATACLIENT_H
