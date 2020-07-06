#ifndef LITTLE_WIRELESS_UART_H
#define LITTLE_WIRELESS_UART_H


class LITTLE_WIRELESS_UART:public UART{
public:
	int 	init(int port,void *parameter);
	//初始化串口，注意要提供收发缓冲区地址
	//复位通道函数
	int 	reset(int port);
};

#endif