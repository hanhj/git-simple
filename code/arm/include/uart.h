#ifndef UART_H
#define UART_H


class UART{
public:
	int 	init(int port,void *parameter);
	//初始化串口，注意要提供收发缓冲区地址
	//复位通道函数
	int 	reset(int port);
	int 	sendbuf(int port, uint8 *buf, int len);
	int 	getbyte( int port, uint8 *c );
	int 	checkstate( int port );
	int 	close_port(int port);
	
	int 	init_serial(int channel, uint32 speed, int databits, int stopbits, int parity);
	int 	close_serial(int channel);
	int  sendbyte(int channel,uint8 c);
protected:

	void 	set_speed ( int fd, int speed );
	int  	set_Parity ( int fd, int databits, int stopbits, int parity );
	int 	open_dev ( char *dev );
	int 	update_serial(int channel, uint32 speed, int databits, int stopbits, int parity);
	int 	revert(int channel, int revert);
	int   get_channel(int port);
};
//extern UART uart;

extern int uart_fd[MAX_UART];
extern int uart_quit_thread[MAX_UART];
extern pthread_t uart_tid[MAX_UART];
extern THREAD_DATA uart_thread_data[MAX_UART];
#endif