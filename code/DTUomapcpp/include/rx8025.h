/*
 * File Name: include/rx8025.h
 * Author: hanhj
 * 
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of
 * any kind, whether express or implied.
 * 
 * Created Time: 2020-06-29 20:32:14 week:1
*/
#ifndef _rx8025_h
#define _rx8025_h
 
/* r8025N寄存器定义 */
#define RX8025_REG_SEC          0x00
#define RX8025_REG_MIN          0x01
#define RX8025_REG_HOUR         0x02
#define RX8025_REG_WDAY         0x03
#define RX8025_REG_MDAY         0x04
#define RX8025_REG_MONTH        0x05
#define RX8025_REG_YEAR         0x06
#define RX8025_REG_RAM          0x07//#define RX8025_REG_DIGOFF       0x07  8025
#define RX8025_REG_ALWMIN       0x08
#define RX8025_REG_ALWHOUR      0x09
#define RX8025_REG_ALWWDAY      0x0a
#define RX8025_REG_TMCNT0       0x0b//#define RX8025_REG_ALDMIN       0x0b
#define RX8025_REG_TMCNT1       0x0c//#define RX8025_REG_ALDHOUR      0x0c
/* 0x0d is reserved */
#define RX8025_REG_FLAG         0x0e//#define RX8025_REG_CTRL1        0x0e
#define RX8025_REG_CTRL         0x0f//#define RX8025_REG_CTRL2         0x0f
#define RX8025_BIT_FLAG_VDET     (1<<0)//用于检测温度补偿的状态
#define RX8025_BIT_FLAG_VLF      (1<<1)//用于检测电压低的状态

typedef struct strTIME_t{
	uint8	cSecond;
	uint8	cMinute;
	uint8	cHour;
	uint8	cMonth;
	uint8	cWeek;
	uint8	cDay;
	uint16  sYear;
}strTIME;
#ifdef __cplusplus
extern "C"{
#endif 
extern int rx8025_get_time(strTIME *get_time);
extern int rx8025_Adjust(strTIME *dt);
#ifdef __cplusplus
}
#endif 
#endif //_rx8025_h
// vim:tw=72 
