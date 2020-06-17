#ifndef _DEBUG__H_
#define _DEBUG__H_
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#define TEM_NO_INFO 0
#define TEM_DEBUG	1
#define TEM_INFO	2
#define TEM_WARNING	3
#define TEM_ERROR	4
#define DEBUG_UART UART0

#if defined (__cplusplus)
extern "C" {
#endif

void debug(int level, const char *file,const char *func, int line, const char *format, ...);
void dump_binary(unsigned char *da,int len);
#define PFUNC(level, msg...) debug(level,__FILE__,__func__, __LINE__, msg)
//#define ASSERT(expr) (expr) ? (void)0 : assert(expr)
#ifdef DEBUG
#define ASSERT(expr) {                                      \
                         if(!(expr))                        \
                         {                                  \
                             __error__(__FILE__, __LINE__); \
                         }                                  \
                     }
#else
#define ASSERT(expr)
#endif
#ifdef __cplusplus
}
#endif
#endif

