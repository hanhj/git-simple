#ifndef _DEBUG__H_
#define _DEBUG__H_
#include <assert.h>
#include <stdio.h>
#include "datatype.h"

#define TEM_INFO	1
#define TEM_WARNING 2
#define TEM_ERROR	3
//control print msg level,only >=level can print
#define DEBUG_LEVEL TEM_INFO
void debug ( int level, int task, const char *file, const char *func, int line, const char *format, ... );

#define PFUNC(level,task, msg...) debug(level, task, __FILE__,__func__, __LINE__, msg)

#define ASSERT(expr) (expr) ? (void)0 : assert(expr)

void dump_binary ( uint8 *da, int16 len );

//control if print msg of task
#define DEBUG_NORMAL	0
#define DEBUG_STATIS	0
#define DEBUG_PRO		0
#define DEBUG_PRO101 	1
#define DEBUG_PRO104 	0
#define DEBUG_COM		0
#define DEBUG_VDSP		0
#define DEBUG_BATT		0
#define DEBUG_MSG		0
#define DEBUG_TIMER		0
#define DEBUG_PROUPDATE 0
#define DEBUG_ESAM		0
#define DEBUG_GPS		0
#endif

