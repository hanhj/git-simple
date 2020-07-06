#ifndef _DEBUG__H_
#define _DEBUG__H_
#include <assert.h>
#include <stdio.h>
#include "datatype.h"

#define TEM_NO_INFO 0
#define TEM_DEBUG 1
#define TEM_INFO 2
#define TEM_WARNING 3
#define TEM_ERROR 4
void debug ( int level, int task, const char *file, const char *func, int line, const char *format, ... );

#define PFUNC(level, msg...) debug(level, 0, __FILE__,__func__, __LINE__, msg)

#define ASSERT(expr) (expr) ? (void)0 : assert(expr)

void dump_binary ( uint8 *da, int16 len );

#endif

