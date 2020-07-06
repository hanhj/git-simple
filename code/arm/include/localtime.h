#ifndef TIME_H
#define TIME_H

#include "types.h"
#include "time.h"
#include "os.h"
#include <linux/rtc.h>

/**
 * 对时
 *
 * @param pbuf		秒分时天周月年
 * @return: 0:		成功
 *	   -1：		失败
 */
int set_time(uint8* pbuf);

/**
 * 设定闹钟时间
 *
 * @param pbuf		秒分时天周月年
 * @return: 0:		成功
 *	   -1：		失败
 */
int set_alarm_time(struct rtc_time_stu *wmtime);

/**
 * 时间格式转换
 * @param time1		秒分时日月年 if dir = 0	;秒分时日月年 if dir = 1
 * @param time2		秒分时日周月年 if dir = 0 ; 秒分时日月年 if dir = 1
 * @param dir		转换方向
 */
void conv_time(uint8 *time1, uint8 *time2, int dir);

/**
 * 判断超时
 *
 * @param cycle  超时周期（单位：秒）
 * @param relative_time 老的时间
 *
 * @return 0: 未超时
 *         1: 超时
 */
int sec_timer(int32 cycle, time_t* relative_time);

/**
 * 判断超时
 *
 * @param cycle  超时周期(单位：分）
 * @param relative_time 老的时间
 *
 * @return 0: 未超时
 *         1: 超时
 */
int min_timer(uint32 cycle, time_t* relative_time);

enum {
	TIME_SECOND =0,
	TIME_MINUTE,
	TIME_HOUR,
	TIME_DAY,
	TIME_MONTH,
	TIME_YEAR
};

extern int day_tbl[2][13];
uint8 get_month(void);
uint8 get_day(void);
uint8 get_hour(void);
uint8 get_minute(void);
uint8 get_second(void);
uint8 get_time_change(uint8* pbuf);

/*
 * 返回秒、分、时、天、月、年
 */
void get_time(uint8 *time, int start, int len);

/**
 * 获取当前时间
 */
time_t get_ticket(time_t* relative_time);

/**
 * 时间增长跨度
 * @param ptime 秒分时日月年
 * @param span  时间增长的跨度
 * @param unit  增长单位(0:秒，分，时，日，月，年)
 * @return -1: 失败
 *	    0：成功
 */
int add_time_span(int8 *ptime, int16 span, int8 unit);
#endif
