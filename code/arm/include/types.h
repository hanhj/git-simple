
/****************************************************************
 * $ID: types.h äž, 19  7æ?2010 09:01:50 +0800  grant $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  (grant)                                         *
 *                                                              *
 * Copyright (C)  2010  zx-jy                                   *
 *   www.zx-jy.com                                              *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it            *
 *   under the terms of the GNU General Public Licence (GPL).   *
 *                                                              *
 * Last modified: äº? 18  1æ?2011 13:52:39 +0800    by grant #
 ****************************************************************/
#ifndef TYPES_H
#define TYPES_H

#define TRUE 1
#define FALSE 0

typedef unsigned int BOOL;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long   uint64;                  /* defined for unsigned 64-bits integer variable 	无符号64位整型变量 */

typedef signed char int8;
typedef signed short int16;
typedef signed long int32;
typedef signed   long long   int64;                   /* defined for signed 64-bits integer variable 		有符号64位整型变量 */

typedef uint8  BYTE;
typedef uint32 DWORD;
typedef uint16 WORD;

#endif
