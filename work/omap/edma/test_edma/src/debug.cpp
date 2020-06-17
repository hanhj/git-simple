#include "evmomapl138.h"
#include "evmomapl138_uart.h"
#include "types.h"
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

void dump_binary(unsigned char*da,int len)
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

