#include "evmomapl138.h"
#include "evmomapl138_uart.h"

#include <stdarg.h>
#include <stdio.h>
#include "debug.h"
#ifdef CONFIG_LINUX
#include <syslog.h>
#endif
#include <string.h>

#define DEBUG_BUFFER_MAX_LENGTH 1024
#define DEBUG_HEAD_LENGTH 100


void debug(int level, const char *file, const char *func, int line, const char *format, ...)
{
	char buffer[DEBUG_HEAD_LENGTH + DEBUG_BUFFER_MAX_LENGTH + 1];
	const char *color = "";
	int index;

	if (level == TEM_NO_INFO)
		return;

	switch (level) {
		case TEM_DEBUG:
			color = "";
			break;
		case TEM_INFO:
			color = "\033[;32m";
			break;
		case TEM_WARNING:
			color = "\033[;33m";
			break;
		case TEM_ERROR:
			color = "\033[;41m";
			break;
		default:
			break;
	}
	sprintf(buffer, "%s%s[%s:%d]\033[0m", color,file, func, line);
	index=strlen(buffer);
	va_list arg;
	va_start(arg, format);
	vsnprintf(&buffer[index], DEBUG_BUFFER_MAX_LENGTH, format, arg);
	va_end(arg);

#ifdef CONFIG_LINUX
	if (level >= TEM_ERROR) {
		if (log == 0) {
			log = 1;
			openlog("ftu212", LOG_CONS | LOG_PID, LOG_USER);
		}
		syslog(LOG_ERR, "[%s:%d]%s", func, line, buffer);

	}
#endif
	UART_txString(DEBUG_UART,buffer);
}

void dump_binary(uint8 *da,int16 len)
{
	int16 i;
	i=0;
	fprintf(stderr,"\r\ndump:%d:",len);
	while (len--)
	{
		if(i%16==0)
		{
			fprintf(stderr,"\r\n ");	
			fprintf(stderr,"%x:",i);
		}
		fprintf(stderr,"%x ",*da++);	
		i++;
	}
}

/*
void PFUNC(int level,const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	debug(level, 0, __FILE__,__FILE__,(int)(__FILE__),format,arg);
}


void fun(int a,...)
{
	va_list pp;
	int n=1;//使用 n 计量参数个数
	va_start(pp,a);
	do
	{
		printf("第 %d 个参数 =%d/n",n++,a);
		a=va_arg(pp,int);//使 pp 指向下一个参数，将下一个参数的值赋给变量 a
	}
	while (a!=0);//直到参数为 0 时停止循环
}

void FUNC (int msg,...)
{
	va_list arg;
	va_start(arg, msg);
	fun(msg,arg);
}

*/
