#include "tcpdataclient.h"


TcpDataClient::TcpDataClient(QQuickItem *parent)
    :QQuickPaintedItem(parent)
{
    //m_imageStatus = 0;
    setConnectionStatus(false);
    m_burst_read_num = BURST_READ_NUM;
    m_bscan_h = 730;//m_application_para->win_bscan.h;
    m_bscan_w = 620;//m_application_para->win_bscan.w;
    setImageStatus(0);

    for (int i = 0; i < ColormapSize; ++i)
        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
    m_oldImage = QImage(m_bscan_w,m_bscan_h,QImage::Format_RGB32);
    m_oldImage.fill(qRgb(0,0,80));
    m_bscanImage = QImage(m_bscan_w,m_bscan_h,QImage::Format_RGB32);
    m_bscanImage.fill(qRgb(0,0,80));

    m_tcpsocket = new QTcpSocket(this);
    connect(m_tcpsocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_tcpsocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(m_tcpsocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
    connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    m_checkConnectionStatusTimer = new QTimer(this);
    m_checkConnectionStatusTimer->setInterval(3000);
    m_checkConnectionStatusTimer->setSingleShot(false);
    connect(m_checkConnectionStatusTimer, SIGNAL(timeout()), this, SLOT(onCheckConnectionStatus()));
    //m_checkConnectionStatusTimer->start();
}

void TcpDataClient::paint(QPainter *painter)
{
    //qDebug()<<__func__<<"is called.";
    painter->drawImage(0,0,m_bscanImage);
}

void TcpDataClient::tryConnectDataServer()
{
    static int tryConnectCnt = 0;
    if(m_tcpsocket){
        qDebug()<<__func__<<tryConnectCnt++;
        //m_tcpsocket->abort();
        m_tcpsocket->connectToHost(DATA_SERVER_IP, DATA_SERVER_PORT);
        //m_tcpsocket->waitForConnected(3000);
    }
}

void TcpDataClient::mannuallyDisconnectFromServer()
{
    if(m_tcpsocket){
        qDebug()<<__func__<<"manually disconnect from server";
        m_tcpsocket->disconnectFromHost();
        //m_tcpsocket->waitForConnected(3000);
    }
}

bool TcpDataClient::connectionStatus()
{
    return m_connectionStatus;
}

void TcpDataClient::setConnectionStatus(bool connectionStatus)
{
    m_connectionStatus = connectionStatus;
}

int TcpDataClient::imageStatus()
{
    return m_imageStatus;
}

void TcpDataClient::setImageStatus(int imageStatus)
{
    m_imageStatus = imageStatus;
}

void TcpDataClient::onConnected()
{
    qDebug()<<__func__<<"connected to data server";
    initDispatchTimer();
    setConnectionStatus(true);
    emit connectionStatusChanged();
}

void TcpDataClient::onDisconnected()
{
    qDebug()<<__func__<<"disconnected from server";
    setConnectionStatus(false);
    emit connectionStatusChanged();
    //tryConnectServer();
}

/**
 * @brief TcpDataClient::onStateChanged
 * @param sta
 *          - callback parameter represent the connection status which is provided by qt framework
 * @details connect to server until the server is online
 * @author yangjingbo
 * @date 2017-3-8
 */
void TcpDataClient::onStateChanged(QAbstractSocket::SocketState sta)
{
    qDebug()<<__func__<<"connection status changed"<<sta;
    if(sta == QAbstractSocket::UnconnectedState){
        qDebug()<<__func__<<"try to reconnect to server";
        /**
         * @brief reconnectTimer
         */
        QTimer *reconnectTimer = new QTimer;
        reconnectTimer->setInterval(3000);
        reconnectTimer->setSingleShot(true);
        connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(onReconnectDataServer()));
        connect(reconnectTimer, SIGNAL(timeout()), reconnectTimer, SLOT(deleteLater()));
        reconnectTimer->start();

    }
}

void TcpDataClient::readyRead()
{
    //qDebug()<<__func__<<"data is ready to ready";
    QByteArray array;
    if(m_tcpsocket->bytesAvailable() < PACKET_SIZE * m_burst_read_num){
        return;
    }
    array = m_tcpsocket->read(PACKET_SIZE * m_burst_read_num);  //每次读取 m_burst_read_num 个数据包
    //qDebug()<<__func__<<array;
    filter_repeat_packet(array);
}

void TcpDataClient::onReconnectDataServer()
{
    tryConnectDataServer();
}

void TcpDataClient::onCheckConnectionStatus()
{
    qDebug()<<__func__<<m_tcpsocket->state();  //pay attention this function could return the right status after you plug out the netwire
}

uint TcpDataClient::rgbFromWaveLength(double wave)
{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (wave >= 380.0 && wave <= 490.0) {

        g = (wave - 380.0) / (490.0 - 380.0);
        b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    else if (wave <  420.0)
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

    r = pow(r * s, 0.8);
    g = pow(g * s, 0.8);
    b = pow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}

void TcpDataClient::filter_repeat_packet(QByteArray array)
{
    //本函数任务
    //1 将不重复的帧放入list列表
    //2 统计接收帧数
    //3 统计丢失帧数
    if(!connectionStatus()){
        return;
    }
    static qint8 cur_frame_index = array.at(FRAME_CNT);
    static qint8 last_frame_index = array.at(FRAME_CNT);
    qint8 index_diff;
    //qDebug()<<"array size "<<array.size();
    for(int i = 0; i < m_burst_read_num; i++){
        cur_frame_index = array.at((i * PACKET_SIZE) + FRAME_CNT);
        index_diff = cur_frame_index - last_frame_index;

        if(index_diff > 1){
            m_lost_cnt += index_diff-1;
        }
        else{
            m_recv_cnt++;
            QByteArray tmp;
            tmp = array.mid(i * PACKET_SIZE, PACKET_SIZE);
            m_packet_list<<tmp;
            /*
            QFile file("/Users/yangjb/Desktop/tmp/file.dat");
            file.open(QIODevice::Append);
            QDataStream out(&file);   // we will serialize the data into the file
            //out << QString("the answer is");   // serialize a string
            out << tmp;        // serialize an integer
            //qDebug()<<"recv cnt "<<m_recv_cnt<< "  lost cnt "<<m_lost_cnt;
            */
        }

        //qDebug()<<"list size "<< m_packet_list.size();

        last_frame_index = cur_frame_index;
    }
}

void TcpDataClient::initDispatchTimer()
{
    m_dispatch_timer = new QTimer(this);
    m_dispatch_timer->setInterval(DISPATCH_PERIOD);
    connect(m_dispatch_timer, SIGNAL(timeout()), this, SLOT(dispatch_packet()));
    m_dispatch_timer->start();
}

void TcpDataClient::dispatch_packet()
{
    //qDebug()<<"[NET_MODULE::dispatch_packet()] period:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<m_prob_time->elapsed();
    //m_prob_time->start();
    //static int cnt;
    //qDebug()<<"dispatch packet "<<cnt++ <<"  list size" <<m_packet_list.size();

    //动态调整分发数据包的周期，防止缓存过多，导致实时性差（实测如果采用固定的DISPATCH_PERIOD，在小声程下会有35个包的缓存，切换量程时会有3s左右的延时）
    if(!connectionStatus()){
        return;
    }
    if(m_packet_list.size() > BURST_READ_NUM){
        m_dispatch_timer->setInterval(DISPATCH_PERIOD/2);
    }
    else{
        m_dispatch_timer->setInterval(DISPATCH_PERIOD);
    }
    if(!m_packet_list.isEmpty()){
        //m_imageStatus++;
        //emit imageStatusChanged();
        QByteArray array;
        array = m_packet_list.takeFirst();

        //add by quhongnian
        /*if(m_average_cnt<m_average_num)   //当小于平均个数的时候一直往里面
        {
            m_prepare_average_list.insert(0,array);
            m_average_cnt++;

        }
        else
        {
            m_prepare_average_list.removeLast();
            m_prepare_average_list.insert(0,array);
            array = calc_average_array(m_prepare_average_list,m_average_num,1000);

        }
        //原先直接在SLOT函数中改变m_average_num，但是若此时list还未更新到要求的元素数目，
        //比如原来是5个，现在改为15个，此时若引用第10个元素就会导致“index out of data”错误
        if(m_flag_change_interference)
        {
            m_flag_change_interference = false;
            qDebug()<<"[void NET_MODULE::dispatch_packet()]i get m_flag_change_interference changed "<<m_flag_change_interference;
            switch(m_application_para->interference)
            {
                case 0: m_average_num = 1;   //off
                        break;
                case 1: m_average_num = 5;  //level1
                        break;
                case 2: m_average_num = 10; //level2
                        break;
                case 3: m_average_num = 15; //level3
                default:
                        break;
            }
         //   m_average_cnt = 0;
         //   m_prepare_average_list.clear();      在重新设置抗干扰等级的时候不需要将list清除，也不需要清除m_average_cnt

        }*/
        //qDebug()<<__func__<<array; //test

        //calc_depth(array);
        //emit dataReady(array);  //向ascan分发数据
        combineImage(array);

        //save to bmp on sdcard
        //save_to_bmp(0,0,array);
    }
}

void TcpDataClient::combineImage(QByteArray array)
{
    if(!connectionStatus()){
        return;
    }
    quint32 gate1_peak_pos_val_original; //fpga返回的门1的峰值位置 原始值为实际采样点
    quint32 gate1_front_edge_pos_val_original; //fpga返回的门1的前沿位置 原始值为实际采样点
    quint32 gate2_peak_pos_val_original; //fpga返回的门2的峰值位置 原始值为实际采样点
    quint32 gate2_front_edge_pos_val_original; //fpga返回的门2的前沿位置 原始值为实际采样点


    static quint32 m_recv_frame_cnt; //接收帧计数器
    static quint32 m_lost_frame_cnt; //丢帧计数器
 //   static quint32 m_all_frame; //理论总帧数
    //获取fpga返回的原始数据 实际采样点为单位

    quint16 cur_tmp = 0 ;
    cur_tmp = (quint8)(array.at(FRAME_CNT+1));
    cur_tmp <<= 8;
    cur_tmp |= (quint8)array.at(FRAME_CNT);

    static quint16 current_frame_index = cur_tmp; //同步当前和上一帧数据，避免固定的误差
    static quint16 last_frame_index = cur_tmp;
    static quint32 last_frame_index_old = 0;
    static quint16 last_frame_index_CNT = 0 ;
    qint16 diff;

    m_recv_frame_cnt++; // 实际接收帧数
    current_frame_index = cur_tmp;

    if( last_frame_index_old > current_frame_index )
        last_frame_index_CNT = last_frame_index_CNT + 1;
    //qDebug()<<"current_frame_index:-------------  :"<<current_frame_index;
    diff = current_frame_index - last_frame_index;

    if(diff != 0)
    {
        //save_to_file(array);
        if(diff > 1){
            m_lost_frame_cnt += diff-1;  //丢帧累加
        }
        last_frame_index = current_frame_index;
        gate1_peak_pos_val_original = (unsigned char)array.at(OFFSET_GATE1_PEAK_POS+3); //higheast byte
        gate1_peak_pos_val_original <<= 8;
        gate1_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_PEAK_POS+2);
        gate1_peak_pos_val_original <<= 8;
        gate1_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_PEAK_POS+1);
        gate1_peak_pos_val_original <<= 8;
        gate1_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_PEAK_POS);

        gate1_front_edge_pos_val_original = (unsigned char)array.at(OFFSET_GATE1_FRONT_EDGE_POS+3); //higheast byte
        gate1_front_edge_pos_val_original <<= 8;
        gate1_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_FRONT_EDGE_POS+2);
        gate1_front_edge_pos_val_original <<= 8;
        gate1_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_FRONT_EDGE_POS+1);
        gate1_front_edge_pos_val_original <<= 8;
        gate1_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE1_FRONT_EDGE_POS);

        gate2_peak_pos_val_original = (unsigned char)array.at(OFFSET_GATE2_PEAK_POS+3); //higheast byte
        gate2_peak_pos_val_original <<= 8;
        gate2_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_PEAK_POS+2);
        gate2_peak_pos_val_original <<= 8;
        gate2_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_PEAK_POS+1);
        gate2_peak_pos_val_original <<= 8;
        gate2_peak_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_PEAK_POS);

        gate2_front_edge_pos_val_original = (unsigned char)array.at(OFFSET_GATE2_FRONT_EDGE_POS+3); //higheast byte
        gate2_front_edge_pos_val_original <<= 8;
        gate2_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_FRONT_EDGE_POS+2);
        gate2_front_edge_pos_val_original <<= 8;
        gate2_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_FRONT_EDGE_POS+1);
        gate2_front_edge_pos_val_original <<= 8;
        gate2_front_edge_pos_val_original += (unsigned char)array.at(OFFSET_GATE2_FRONT_EDGE_POS);


        //static QImage m_oldImage = QImage(m_bscan_w,m_bscan_h,QImage::Format_RGB32);
        QImage tmp = QImage(m_bscan_w,m_bscan_h,QImage::Format_RGB32);
        tmp = m_oldImage.copy(diff,0,m_bscan_w,m_bscan_h);  //每次前进2帧
        //*p_global_image = m_oldImage.copy(1,0,BscanItemWidth,m_bscan_h);
        //static quint16 y0;

        for (int y = 0; y < m_bscan_h; y++)
       {

            /*
          if((y>(m_bscan_h-5)) & (((last_frame_index_CNT * 65536)+current_frame_index*80)%6000 == 0 ) )
          {
               tmp.setPixel(m_bscan_w-1,y,qRgb(255,255,255));

          }
          else if((y>(m_bscan_h-10)) & (((last_frame_index_CNT * 65536)+current_frame_index*80)%60000 == 0 ) )
          {
               tmp.setPixel(m_bscan_w-1,y,qRgb(255,255,255));
          }
          else
          {
              tmp.setPixel(m_bscan_w-1,y,colormap[(unsigned char)array.at(y)]);
          }*/
            if(y>(m_bscan_h-10))
            {
                if(((y<(m_bscan_h-7)))&&(((last_frame_index_CNT * 65536)+current_frame_index*80)%6000 == 0))
                    tmp.setPixel(m_bscan_w-1,y,qRgb(200,200,200));
                else if(((last_frame_index_CNT * 65536)+current_frame_index*80)%60000 == 0 )
                    tmp.setPixel(m_bscan_w-1,y,qRgb(255,255,255));
                else
                    tmp.setPixel(m_bscan_w-1,y,qRgb(0,0,0));
            }
            else
            {
                tmp.setPixel(m_bscan_w-1,y,colormap[(unsigned char)array.at(y)]);
            }

       }
        m_oldImage = tmp;
        m_bscanImage = tmp;
        //m_bscanImage.fill(qRgb(0,255,0));
        m_imageStatus++;
        //qDebug()<<__func__<<"new image "<<m_imageStatus;
        //emit imageReady(tmp);
        emit imageStatusChanged();
    }
}
