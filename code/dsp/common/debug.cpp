#include <stdarg.h>
#include <stdio.h>
#include "debug.h"
#ifdef CONFIG_LINUX
#include <syslog.h>
#endif

#define DEBUG_BUFFER_MAX_LENGTH 1024

static int log = 0;

void debug ( int level, int task, const char *file, const char *func, int line, const char *format, ... )
{
    char buffer[DEBUG_BUFFER_MAX_LENGTH + 1];
    const char *color = "";

    if ( level == TEM_NO_INFO )
        return;

    switch ( level ) {

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

    va_list arg;

    va_start ( arg, format );
#ifdef CONFIG_LINUX
    vsnprintf ( buffer, DEBUG_BUFFER_MAX_LENGTH, format, arg );
#else
    //sprintf ( buffer, format,arg );
#endif
    va_end ( arg );

#ifdef CONFIG_LINUX

    if ( level >= TEM_ERROR ) {
        if ( log == 0 ) {
            log = 1;
            openlog ( "ftu212", LOG_CONS | LOG_PID, LOG_USER );
        }

        syslog ( LOG_ERR, "[%s:%d]%s", func, line, buffer );

    }
	fprintf(stderr, "%s%s[%s:%d]%s\033[0m", color,file, func, line, buffer);
#else
	//printf(format, arg);

#endif

}

void dump_binary ( uint8 *da, int16 len )
{
    int16 i;
    i = 0;
    fprintf ( stderr, "\r\ndump:%d:", len );

    while ( len-- ) {
        if ( i % 16 == 0 ) {
            fprintf ( stderr, "\r\n " );
            fprintf ( stderr, "%02x:", i );
        }

        fprintf ( stderr, "%02x ", *da++ );

        i++;
    }
}
