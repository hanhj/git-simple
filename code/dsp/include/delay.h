#ifndef DELAY_H
#define DELAY_H
#include "types.h"
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
int mdelay ( uint32 msecond );
void sleep(int32 time);
#endif
