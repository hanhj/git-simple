#include <time.h>
#include <string.h>

#include "utils.h"
#include "debug.h"
#include "localtime.h"
//#include "product.h"

int day_tbl[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
			{0,31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

uint8 get_month(void)
{
	uint8 time[6];
	get_time(time, TIME_SECOND, 6);
	return time[4];
}

uint8 get_day(void)
{
	uint8 time[6];
	get_time(time, TIME_SECOND, 6);
	return time[3];
}

uint8 get_hour(void)
{
	uint8 time[6];
	get_time(time, TIME_SECOND, 6);
	return time[2];
}

uint8 get_minute(void)
{
	uint8 time[6];
	get_time(time, TIME_SECOND, 6);
	return time[1];
}

uint8 get_second(void)
{
	uint8 time[6];
	get_time(time, TIME_SECOND, 6);
	return time[0];
}

//返回设置时间改变（0位：日；4位：月）
uint8 get_time_change(uint8* pbuf)
{
	uint8 rtn = 0;
	uint8 time[6];	//秒分时日月年
	get_time(time, TIME_SECOND, 6);
	if(time[3] != pbuf[3]) {		//日改变
		rtn |= (0x01);
	}
	if(time[4] != pbuf[5]) {		//月改变，日也一定改变
		rtn |= (0x11);
	}
	return rtn;
}

//秒分时日月年
int set_time(uint8* pbuf)
{
    struct rtc_time_stu rtcTime;

    rtcTime.tm_sec = pbuf[0]; 
    rtcTime.tm_min = pbuf[1];
    rtcTime.tm_hour = pbuf[2];
    rtcTime.tm_mday = pbuf[3];
    rtcTime.tm_mon = pbuf[5];
    rtcTime.tm_year = pbuf[6];
 
	return set_rtc(&rtcTime);
}

int set_alarm_time(struct rtc_time_stu *wmtime)
{
	return set_alm_rtc(wmtime);
}

//0 : 秒分时日月年(time1)		--->秒分时日周月年(time2)
//1 : 秒分时日周月年(time2)		--->秒分时日月年 (time1)
void conv_time(uint8 *time1, uint8 *time2, int dir)
{
	if (0 == dir) {
		memcpy(time2, time1, 4);
		time2[4] = 0;
		memcpy(&time2[5], &time1[4], 2);
	} else {
		memcpy(time1, time2, 4);
		memcpy(&time1[4], &time2[5],2);
	}
}


/**
 * 判断是否超时
 *
 * @param cycle  超时周期
 * @param relative_time
 *               当前时间
 *
 * @return 0: 未超时
 *         1: 超时
 */
int sec_timer(int32 cycle, time_t* relative_time)
{
	time_t cur_second;
	int ret;

	time(&cur_second);

	if ((cur_second <= *relative_time) || (0 == *relative_time)) {
		*relative_time = cur_second;
		return 0;
	}

	if (cur_second < (*relative_time) + cycle && cur_second >= (*relative_time)) {
		ret = 0;
	} else if (cur_second < (*relative_time) && (cur_second+(0xffffffff-(unsigned long)(*relative_time)) < cycle)) {
		ret = 0;
	} else {
		*relative_time = cur_second;
		ret = 1;
	}
	return ret;
}

int min_timer(uint32 cycle, time_t* relative_time)
{
	return sec_timer(cycle * 60, relative_time);
}


time_t get_ticket(time_t* relative_time)
{
	return time(relative_time);
}

void get_time(uint8 *systime, int start, int len)
{
	time_t curTime;
	struct tm *now;
	int i = 0, l = 0;

	curTime = time(NULL);
	now = localtime( &curTime );
	switch (start) {
	case TIME_SECOND:
		goto second;
	case TIME_MINUTE:
		goto minute;
	case TIME_HOUR:
		goto hour;
	case TIME_DAY:
		goto day;
	case TIME_MONTH:
		goto mon;
	case TIME_YEAR:
		goto year;
	default:
		break;
	}
second:
	systime[i++] = now->tm_sec;
	if (++l >= len)
		goto out;
minute:
	systime[i++] = now->tm_min;
	if (++l >= len)
		goto out;
hour:
	systime[i++] = now->tm_hour;
	if (++l >= len)
		goto out;
day:
	systime[i++] = now->tm_mday;
	if (++l >= len)
		goto out;
mon:
	systime[i++] = now->tm_mon+1;
	if (++l >= len)
		goto out;
year:
	systime[i++] = now->tm_year%100;
	if (++l >= len)
		goto out;
out:
	return;
}

int add_time_month(int8 *ptime, int16 span)
{
	int month;

	month = ptime[4] + span;
	ptime[4] = (48+month) % 12;
	if(0 == ptime[4])
		ptime[4] = 12;
	ptime[5] += (((48+month) / 12)-4);
	if(0 == month)
		ptime[5]--;
/*
	if (month <= 0 && span < 0)
		ptime[4] = 12;
	else {
		if (month != 12)
			ptime[4] = month % 12;
		else
			ptime[4] = month;
	}
*/
	return 0;
}

int add_time_day(int8 *ptime, int16 span)
{
	int day, year, is_leap;

	get_time((uint8 *)&year, TIME_YEAR, 1);
	year += 2000;
	is_leap = LEAP_YEAR(year);
	if (ptime[4] <1 || ptime[4] > 12) {
		get_time((uint8 *)&ptime[4], TIME_MONTH, 1);
	}
	if ((ptime[3] + span) > 0 ) {
//		PFUNC(TEM_INFO, DEBUG_NORMAL,"++++++++++++++ day_tbl[is_leap][ptime[4]] = %d, ptime[4] = %d\n", day_tbl[is_leap][ptime[4]], ptime[4]);
		day = ptime[3] + span;
		if (day != day_tbl[is_leap][ptime[4]])
			ptime[3] = day % day_tbl[is_leap][ptime[4]];
		else
			ptime[3] = day_tbl[is_leap][ptime[4]];
		//PFUNC(TEM_INFO, DEBUG_NORMAL,"year ========= %d - %d\n", ptime[5],(day / (day_tbl[is_leap][ptime[4]]+1)));
		if(0 == (day / (day_tbl[is_leap][ptime[4]]+1)))
			return 0;
		else
			return add_time_month(ptime, (day / (day_tbl[is_leap][ptime[4]]+1)) );
	} else {

//		PFUNC(TEM_INFO,DEBUG_NORMAL, "MON ========= %d\n", ptime[4]);
		if (ptime[4] == 1)
			day = ptime[3] + span + day_tbl[is_leap][12];
		else
			day = ptime[3] + span + day_tbl[is_leap][ptime[4]-1];
		if (ptime[4] == 1) {
			ptime[3] = day;
		} else {
			if (day != day_tbl[is_leap][ptime[4]-1]){
				ptime[3] = day % day_tbl[is_leap][ptime[4]-1];
			} else
				ptime[3] = day_tbl[is_leap][ptime[4]-1];
		}
		return add_time_month(ptime, -1 );
	}
}

int add_time_hour(int8 *ptime, int16 span)
{
	int hour;
	int borrow = 0;

	if ((ptime[2]+span) >= 0) {
		hour = ptime[2] + span;
		ptime[2] = hour % 24;
		if(0 == ( hour / 24))
			return 0;
		else
			return add_time_day(ptime, hour / 24);
	} else {
		borrow = (0>((span%60)+ptime[2]))?1:0; //借位
		hour = ptime[2] +span + (24*10);
		ptime[2] = hour % 24;
		if((0 == (span / 24)) && (0 == borrow))
			return 0;
		else
			return add_time_day(ptime, (span / 24)-borrow);
	}
}

int add_time_min(int8 *ptime, int16 span)
{
	int min;
	int borrow = 0;

	if ((ptime[1] + span) >= 0 ) {
		min = ptime[1] + span;
		ptime[1] = min % 60;
		if(0 == (min / 60))
			return 0;
		else
			return add_time_hour(ptime, min / 60);
	} else {
		borrow = (0>((span%60)+ptime[1]))?1:0; //借位
		min = ptime[1] + (60*60) + span;
		ptime[1] = min % 60;
		if((0 == (span / 60)) && (0 == borrow))
			return 0;
		else
			return add_time_hour(ptime, (span / 60)-borrow);
	}
}

int add_time_sec(int8 *ptime, int16 span)
{
	int sec;
	int borrow = 0;

	if ((ptime[0] + span) >= 0) {
		sec = ptime[0] + span;
		ptime[0] = sec % 60;
		if(0 == (sec / 60))
			return 0;
		else
			return add_time_min(ptime, sec / 60);
	} else {
		borrow = (0>((span%60)+ptime[0]))?1:0; //借位
		sec = ptime[0] + (60*60) +span;
		ptime[0] = sec % 60;
		if((0 == (span / 60)) && (0 == borrow))
			return 0;
		else
			return add_time_min(ptime, (span / 60)-borrow);
	}
}

/**
 * 时间增长跨度
 * @param ptime 秒分时日月年
 * @param span	时间增长的跨度
 * @param unit  增长单位(0:秒，分，时，日，月，年)
 * @return -1: 失败
 *	    0：成功
 */
int add_time_span(int8 *ptime, int16 span, int8 unit)
{
	switch(unit) {
	case TIME_SECOND:
		add_time_sec(ptime, span);
		break;

	case TIME_MINUTE:
		add_time_min(ptime, span);
		break;

	case TIME_HOUR:
		add_time_hour(ptime, span);
		break;
	case TIME_DAY:
		add_time_day(ptime, span);
		break;
	case TIME_MONTH:
		add_time_month(ptime, span);
		break;
	case TIME_YEAR:
	default:
		return -1;
	}
	return 0;
}

int set_rtc(struct rtc_time_stu *time)
{
	return 1;
}
int set_alm_rtc(struct rtc_time_stu *time)
{
	return 1;
}