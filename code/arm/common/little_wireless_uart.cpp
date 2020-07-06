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
#include "gpio.h"
#include "little_wireless_uart.h"
/*****************************************************************************
																	public fun
******************************************************************************/
int LITTLE_WIRELESS_UART::init(int port,void *parameter)
{
	gpio.Init(LITTLE_WIRELESS_SLEEP,GPIO_DIR_OUT,0);	
	gpio.Init(LITTLE_WIRELESS_RST,GPIO_DIR_OUT,1);	
	reset(port);
	return UART::init(port,parameter);
}	
//复位通道函数
int LITTLE_WIRELESS_UART::reset(int port)
{
	gpio.Write(LITTLE_WIRELESS_SLEEP,1);
	mdelay(15);
	gpio.Write(LITTLE_WIRELESS_RST,0);
	mdelay(1);
	gpio.Write(LITTLE_WIRELESS_RST,1);
	return 1;
}	

