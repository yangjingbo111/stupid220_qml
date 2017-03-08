#ifndef MICRO_H
#define MICRO_H

#define DATA_SERVER_IP                      "192.168.2.100"     //服务器默认ip(本机ip)
#define DATA_SERVER_PORT                    8088                //数据传输端口
//#define DATA_SERVER_IP                      "192.168.175.129"   //服务器默认ip(本机ip)
//#define DATA_SERVER_PORT                    8088                //数据传输端口

#define PACKET_SIZE                         1024    //每帧数据包的字节数
#define BURST_READ_NUM                      1//5       //突发读取数据包个数
#define DISPATCH_PERIOD                     80      //80ms分发一次数据

#define FRAME_CNT                           0X3E8   //重复频率帧计数 2BYTES LOW BYTE
#define OFFSET_PRODUCT_SERIAL               0X3EA   //产品标识寄存器 2BYTES LOW BYTE
#define OFFSET_GATE1_PEAK_AMP               0X3EC   //波门1峰值幅度  1BYTE
#define OFFSET_GATE2_PEAK_AMP               0X3ED   //波门2峰值幅度  1BYTE
#define OFFSET_GATE1_FRONT_EDGE_AMP         0X3EE   //波门1前沿幅度  1BYTE
#define OFFSET_GATE2_FRONT_EDGE_AMP         0X3EF   //波门2前沿幅度  1BYTE
#define OFFSET_GATE1_PEAK_POS               0X3F0   //波门1峰值位置  4BYTE LOW BYTE
#define OFFSET_GATE2_PEAK_POS               0X3F4   //波门2峰值位置  4BYTE LOW BYTE
#define OFFSET_GATE1_FRONT_EDGE_POS         0X3F8   //波门1前沿位置  4BYTE LOW BYTE
#define OFFSET_GATE2_FRONT_EDGE_POS         0X3FC   //波门2前沿位置  4BYTE LOW BYTE

#define TX_OFFSET 300  //采样开始后第13个点开始激励  120 is the actual


#endif // MICRO_H
