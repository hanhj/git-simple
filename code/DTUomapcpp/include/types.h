/*
 * types.h
 *
 *  Created on: 2013-8-6
 *      Author: Administrator
 */
#ifndef TYPES_H_
#define TYPES_H_

#define TRUE 1
#define FALSE 0
#define false 0
#define true 1

typedef char 				bool_e;
typedef unsigned int 		BOOL;

typedef unsigned char 		uint8;
typedef unsigned short 		uint16;
typedef unsigned long 		uint32;
typedef unsigned long long  uint64;
typedef signed char 		int8;
typedef signed short 		int16;
typedef signed long 		int32;
typedef signed   long long  int64;

#ifndef xdc_std__include
typedef unsigned int 		Uint32;
#endif
typedef unsigned long long 	Uint64;
typedef float               float32;
typedef long                long32;
typedef unsigned long       ulong32;
typedef double              Double64;
typedef unsigned long   	ulong_t;

#ifndef _STDINT_H_
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int      int32_t;
#endif

typedef uint8  BYTE;
typedef uint32 DWORD;
typedef uint16 WORD;
#endif /* TYPES_H_ */
