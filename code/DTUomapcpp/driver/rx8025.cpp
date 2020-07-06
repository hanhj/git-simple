/*
 * File Name: driver/rx8025.cpp
 * Author: hanhj
 * 
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of
 * any kind, whether express or implied.
 * 
 * Created Time: 2020-06-29 20:28:56 week:1
*/
#include "utils.h"
#include "rx8025.h"
#include "i2c.h"
static const uint8 SlaveAdd[1]={
	0x64
};

/***********************************************************************
**  Time:        liwenjin 2012.10
**  Function:从8025N获取时间
**  Input:
**  Output:        None
**	return on success 1;fail 0
**  Attention:
***********************************************************************/

int rx8025_get_time(strTIME *get_time)
{
    int ret;
	I2CSLAVEREG_U addr;
    uint8 date[7]={0,0,0,0,0,0,0};
	addr.all = SlaveAdd[0];
	ret=ReadI2cDevBypes(&addr,RX8025_REG_SEC,7,&date[0],1);
    if (ret==0)
		return ret; 
    get_time->cSecond =bcd2bin(date[RX8025_REG_SEC]&0x7f);
    get_time->cMinute =bcd2bin(date[RX8025_REG_MIN]&0x7f);
    get_time->cHour = bcd2bin(date[RX8025_REG_HOUR] & 0x3f);
    get_time->cDay = bcd2bin(date[RX8025_REG_MDAY] & 0x3f);
    get_time->cMonth= bcd2bin(date[RX8025_REG_MONTH] & 0x1f);
    get_time->sYear= bcd2bin(date[RX8025_REG_YEAR]);
    return 1;
}
/***********************************************************************
**  Time:        liwenjin 2012.10
**  Function:校正8025N的时间
**  Input:
**  Output:        None
**	return on success 1;fail 0
**  Attention:
***********************************************************************/
int rx8025_Adjust(strTIME *dt)
{
    int ret;
	I2CSLAVEREG_U addr;
    uint8 date[7];
	addr.all = SlaveAdd[0];
    //该8025假定能被4整除是闰年，但2100年不是闰年（BUG）
    date[RX8025_REG_SEC] = bin2bcd(dt->cSecond);
    date[RX8025_REG_MIN] = bin2bcd(dt->cMinute);
    date[RX8025_REG_HOUR] = bin2bcd(dt->cHour);
//  date[RX8025_REG_WDAY] = bin2bcd(dt->cWeek);
    date[RX8025_REG_MDAY] = bin2bcd(dt->cDay);
    date[RX8025_REG_MONTH] = bin2bcd(dt->cMonth);
    date[RX8025_REG_YEAR] = bin2bcd(dt->sYear% 100);
	ret=WriteI2cDevBypes(&addr,RX8025_REG_SEC,7,&date[0],1);
    return ret;
}

// vim:tw=72 
