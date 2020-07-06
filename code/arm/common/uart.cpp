#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "datatype.h"
#include "task.h"
#include "debug.h"
#include "device.h"
#include "uart.h"
#include "com.h"
#include "utils.h"//for myopen
static int speed_arr[] = {  B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B600,B300};
static int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400, 1200, 600,300};
int uart_fd[MAX_UART];
int uart_quit_thread[MAX_UART];
pthread_t uart_tid[MAX_UART];
THREAD_DATA uart_thread_data[MAX_UART];

void UART::set_speed ( int fd, int speed )
{
	uint16   i;
	int   status;

	struct termios   Opt;
	tcgetattr ( fd, &Opt );

	for (i = 0;  i < sizeof ( speed_arr ) / sizeof ( int );  i++) {
		if (speed == name_arr[i]) {
			tcflush ( fd, TCIOFLUSH );
			cfsetispeed ( &Opt, speed_arr[i] );
			cfsetospeed ( &Opt, speed_arr[i] );
			status = tcsetattr ( fd, TCSANOW, &Opt );

			if (status != 0)
				PFUNC(TEM_ERROR ,DEBUG_COM,  "tcsetattr fd1\n" );

			return;
		}

		tcflush ( fd, TCIOFLUSH );
	}
}

int UART::set_Parity ( int fd, int databits, int stopbits, int parity )
{

	struct termios options;

	if (tcgetattr ( fd, &options )  !=  0) {
		PFUNC( TEM_ERROR ,DEBUG_COM, "set_Parity Error 1\n");
		return( FALSE );
	}

	options.c_cflag &= ~CSIZE;

	switch (databits) {

	case 7:
		options.c_cflag |= CS7;
		break;

	case 8:
		options.c_cflag |= CS8;
		break;

	default:
		PFUNC ( TEM_WARNING ,DEBUG_COM, "Unsupported data size:%d\n",databits );
		return( FALSE );
	}

	switch (parity) {

	case 'n':

	case 'N':
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;

	case 'o':

	case 'O':
		options.c_cflag |= ( PARODD | PARENB );
		options.c_iflag |= INPCK;
		break;

	case 'e':

	case 'E':
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;

	case 'S':

	case 's':
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;

	default:
		PFUNC ( TEM_WARNING ,DEBUG_COM, "Unsupported parity\n" );
		return( FALSE );
	}

	switch (stopbits) {

	case 1:
		options.c_cflag &= ~CSTOPB;
		break;

	case 2:
		options.c_cflag |= CSTOPB;
		break;

	default:
		PFUNC ( TEM_WARNING ,DEBUG_COM, "Unsupported stop bits\n" );
		return( FALSE );
	}

	options.c_cflag &= ~CRTSCTS;

	/* Set input parity option */

	if (parity != 'n')
		options.c_iflag |= INPCK;

	//add by qianwei
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	options.c_lflag &= ~(ICANON |ISIG | ECHO | ECHOE);
	options.c_iflag &= ~(IXON | IXOFF|IXANY);
	options.c_oflag &= ~ONLCR;
	options.c_iflag &= ~ICRNL;

	tcflush ( fd, TCIFLUSH );

	/* Update the options and do it NOW */
	if (tcsetattr ( fd, TCSANOW, &options ) != 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "Setup Serial error 2\n");
		return( FALSE );
	}

	return( TRUE );
}

int UART::open_dev ( char *dev )
{
	int fd = myopen ( dev, O_RDWR );

	if (-1 == fd) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "Can't Open %s failed\n", dev );
		return -1;
	} else {
		return fd;
	}
}


static void *receive_thread ( void* parameter )
{
	int nread, i;
	THREAD_DATA *pdata;
	BUF_DATA* p_buf;
	char t_buf[100];
	int channel;
	fd_set rfds;
	struct timeval to;

	pdata = ( THREAD_DATA *) parameter;
	p_buf = &(pdata->p_buf->recv);
	channel = pdata->fd_index;
	uart_quit_thread[channel]=0;
PFUNC(TEM_INFO,DEBUG_COM, "start uart thread:%d\n",UART_THD+channel);
	while (1) {
		memset(t_buf, 0, sizeof(t_buf) );
		FD_ZERO(&rfds);
		FD_SET(uart_fd[channel], &rfds);
		to.tv_sec = 0;
		to.tv_usec = 10000;
		if(uart_quit_thread[channel]==1)
			break;
		g_task->Heart(UART_THD+channel);	
		if (select(uart_fd[channel] + 1,  &rfds, 0, 0, &to) > 0) {
			if (uart_fd[channel] < 0)
				break;
			if (!FD_ISSET(uart_fd[channel], &rfds))
				continue;
			if (( nread = read ( uart_fd[channel], t_buf, 100 ) ) > 0) {
				for (i = 0;i < nread;i++) {
					p_buf->buf[p_buf->in_cnt] = t_buf[i];
					if (++p_buf->in_cnt >= BUFFER_SIZE)
						p_buf->in_cnt = 0;

				}
			PFUNC(TEM_INFO,DEBUG_COM, "recv uart channel:%d Len:%d\n",channel,nread);	
			//dump_binary((uint8*)t_buf,nread);
			}
		}
		if (uart_fd[channel] < 0)
			break;
	}
	g_task->UnRegist(UART_THD+channel);
	PFUNC(TEM_ERROR ,DEBUG_COM, "exit uart thread:%d \n",UART_THD+channel);		
	return NULL;
}

int UART::update_serial(int channel, uint32 speed, int databits, int stopbits, int parity)
{
	int ctrlbits;

	set_speed ( uart_fd[channel], speed );

	if (set_Parity ( uart_fd[channel], databits, stopbits, parity ) == FALSE) {
		PFUNC ( TEM_ERROR ,DEBUG_COM, "set parity of channel %d fail\n" ,channel);
		return -1;
	}

	ioctl ( uart_fd[channel], TIOCMGET, &ctrlbits );
	ctrlbits &= ~TIOCM_RTS;
	ioctl ( uart_fd[channel], TIOCMSET, &ctrlbits );
	return 0;
}
int UART::revert(int channel, int revert)
{
	int index = -1;
	uint16  *pconsume;
	index= channel;
	pconsume = &uart_thread_data[index].p_buf->recv.out_cnt;
		
	if (*pconsume >= revert) {
		*pconsume -= revert;
	} else {
		*pconsume += (BUFFER_SIZE - revert);
	}
	return 0;
}
int UART::init_serial(int channel, uint32 speed, int databits, int stopbits, int parity)
{
	char dev[100];
	int index = -1;
	int ret=0;

	if (channel < 0 || channel >= MAX_UART) {
		return -1;
	}

	index = channel;
	switch(index){
		case 0:
			sprintf(dev,"%s",UART1_DEV);	
			break;
		case 1:
			sprintf(dev,"%s",UART2_DEV);	
			break;
		case 2:
			sprintf(dev,"%s",UART3_DEV);	
			break;
		case 3:
			sprintf(dev,"%s",UART4_DEV);	
			break;
		case 4:
			sprintf(dev,"%s",UART5_DEV);	
			break;
		case 5:
			sprintf(dev,"%s",UART6_DEV);	
			break;
		case 6:
			sprintf(dev,"%s",UART7_DEV);	
			break;
		case 7:
			sprintf(dev,"%s",UART8_DEV);	
			break;
		case 8:
			sprintf(dev,"%s",UART9_DEV);	
			break;
		case 9:
			sprintf(dev,"%s",UART10_DEV);	
			break;
		default:
			break;
			
	}

	if (uart_fd[channel] > 0) {
		PFUNC(TEM_INFO,DEBUG_COM, "Channel already opened, update serial\n");
		return update_serial(channel, speed, databits, stopbits, parity);
	}

	uart_fd[channel] = open_dev(dev);

	if (uart_fd[channel] == -1) {
		return -1;
	}
	update_serial(channel, speed, databits, stopbits, parity);
	PFUNC(TEM_INFO,DEBUG_COM, "Init Channel:%d dev:%s ok\n",channel,dev);
	return ret;
}
int UART::get_channel(int port)
{
	int channel;
	if(port>=UART_PORT1&&port<=UART_PORT12)
		channel=port-UART_PORT1;
	else
		channel=-1;
return channel;
}	

/*****************************************************************************
																	public fun
******************************************************************************/
int UART::init(int port,void *parameter)
{
	int channel;
	UartCfg*para;
	para=(UartCfg*)parameter;
	channel=get_channel(port);

	if(channel!=-1)
		{
			uart_thread_data[channel].fd_index = channel;
			uart_thread_data[channel].p_buf = para->Buff;
			uart_thread_data[channel].para = parameter;

			uart_thread_data[channel].p_buf->recv.in_cnt = 0;

			uart_thread_data[channel].p_buf->recv.out_cnt = 0;
			uart_thread_data[channel].p_buf->send.in_cnt = 0;
			uart_thread_data[channel].p_buf->send.out_cnt = 0;
			memset(uart_thread_data[channel].p_buf->recv.buf, 0, BUFFER_SIZE*sizeof(uint8));
			memset(uart_thread_data[channel].p_buf->send.buf, 0, BUFFER_SIZE*sizeof(uint8));		
			uart_fd[channel]=-1;
			init_serial(channel,para->BaudRate,para->DataBit,para->StopBit,para->CheckBit);
			g_task->Regist(UART_THD+channel);
			return pthread_create ( &uart_tid[channel], NULL, receive_thread, ( void* ) &uart_thread_data[channel] );
		}
	return -1;
}	
//复位通道函数
int UART::reset(int port)
{
	
	return 1;
}	

int UART::sendbuf(int port, uint8 *buf, int len)
{
	int channel;
	channel=get_channel(port);
	if(channel!=-1)
	return write ( uart_fd[channel], buf, len );
	return -1;
}

int UART::getbyte( int port, uint8 *c )
{
	int index = -1;
	int channel;
	uint16 *pproduce, *pconsume;
	channel=get_channel(port);

	if (channel < 0 || channel > MAX_UART) {
		return -1;
	}

	index= channel;

	pproduce = &uart_thread_data[index].p_buf->recv.in_cnt;
	pconsume = &uart_thread_data[index].p_buf->recv.out_cnt;


	if (*pconsume != *pproduce) {
		*c = uart_thread_data[index].p_buf->recv.buf[*pconsume];

		PFUNC(TEM_INFO,DEBUG_COM, "1. index = %d, produce = %d, consume = %d, receive char=%02x\n", index, *pproduce, *pconsume, *c);
		if (++(*pconsume) >= BUFFER_SIZE)
			*pconsume = 0;
		return 0;
	} else
		return -1;
}
int UART::checkstate ( int port )
{
	return 1;
}

int UART::close_port(int port)
{
	int channel;
	channel=get_channel(port);
	uart_quit_thread[channel] = 1;
	
	if (uart_fd[channel] > 0) {
		close(uart_fd[channel]);
		uart_fd[channel] = -1;
	}
	if (uart_tid[channel] != 0) {
		pthread_join(uart_tid[channel], NULL);
		uart_tid[channel] = 0;
		return 1;
	}
	return 0;
}
	
int UART::close_serial(int channel)
{
	if (uart_fd[channel] > 0) {
	close(uart_fd[channel]);
	uart_fd[channel] = -1;
	}
	return 0;
}	
int UART::sendbyte(int channel,uint8 c){
	if(uart_fd[channel]>0){
		write(uart_fd[channel],&c,1);
		return 0;
	}
return -1;		
}
