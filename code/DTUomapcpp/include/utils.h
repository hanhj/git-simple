/*
 * File Name: utils.h
 * Author: hanhj
 * 
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of
 * any kind, whether express or implied.
 * 
 * Created Time: 2020-06-17 23:40:55 week:3
*/
#ifndef _utils_h
#define _utils_h
#include "types.h"
#ifdef __cplusplus
extern "C"{
#endif 

/**
 * 延时second秒
 *
 * @param second
 *
 * @return
 */
int delay ( uint32 second );
/**
 * 延时usecond微妙
 *
 * @param usecond
 *
 * @return
 */
int udelay ( uint32 usecond );
/**
 * 延时msecond毫秒
 *
 * @param msecond
 *
 * @return
 */
extern int mdelay ( uint32 msecond );

int uniq_int_array(uint16 *in,uint16 in_len,uint16 ignore,uint16 *out);
extern void SortFloat(float *p,int16 p_size); //泡排序(大到小)
extern void SortUint16(uint16_t *p,int16_t p_size); //冒泡排序(小到大)
extern void SortUint8(uint8_t *p,uint8_t p_size);
extern float GetAverage(int16 *p ,int16 p_size);
void DelayUs(Uint32 us);
uint8 bcd2bin(uint8);
uint8 bin2bcd(uint8);
int calcrc ( uint8 *ptr, int count );

#ifdef __cplusplus
}
#endif 
#endif //_utils_h
// vim:tw=72 
