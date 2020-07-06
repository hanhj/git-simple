/*
 * clock.cpp
 *
 *  Created on: 2013-10-18
 *      Author: Administrator
 */

#include "os.h"
#include "clock.h"
#include "evmomapl138_timer.h"
#include "types.h"
#include "datatype.h"
#include "interruptapp.h"

Uint64 Sys500us = 0;
Uint64 SystemMs = 0;
int SecondChageFlag;
CP56Time2a TerTime = {
0,
0,
0,
5,
12,
 14
};
Clock clk0(0);
Counter T_tool;

Clock::Clock(int newId)
{
    id = newId;
    ticks = 0;
	sysms=0;
    millisecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    day = 19;
    month = 8;
    year = 10;
    SynLock = 0;
    week = 0;
}

void Clock::reset(){
    ticks = 0;
    sysms = 0;
    millisecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    day = 19;
    month = 8;
    year = 10;
    SynLock = 0;
    week = 0;
}
Clock::~Clock()
{
}
int Clock::ClockSynchronization(CP56Time2a *syntime)
{
	SynLock = 1;
	millisecond = syntime->millisecond%1000;
	second= syntime->millisecond/1000;
	minute= syntime->minute;
	hour= syntime->hour;
	day= syntime->day&0x1f;
	week= syntime->day>>5;//syntime.cWeek;
	month= syntime->month;
	year= syntime->year;
	SynLock = 0;
	return 1;
}
void Clock::tick()
{
   ticks++;
   setSecond();
   return;
}

void Clock::milli_tick()
{
	if(SynLock ==1){
		return ;
	}
	sysms++;
	millisecond ++;
	SystemMs=sysms;
	if(millisecond>= 999){
		millisecond = 0;
		tick();
		SecondChageFlag = 1;
	}
}
uint64 Clock::GetSysMs()
{
	return  sysms;
}

void Clock::setSecond()
{
    if (second == 59) {
        setMinute();
        second = 0;
    }
    else {
        second++;
    }

    return;
}

void Clock::setMinute()
{
    if (minute == 59) {
        setHour();
        minute = 0;
    }
    else {
        minute++;
    }

    return;
}

void Clock::setHour()
{
    if (hour == 23) {
        setDay();
        hour = 0;
        week++;
        if(week>=8){
        	week = 1;
        }
    }
    else {
        hour++;
    }

    return;
}

void Clock::setDay()
{
    bool thirtydays = false;
    bool feb = false;
    bool leap = false;

    if (month == 4 || month == 6 || month == 9 || month == 11) {
        // April, June, September, November.
        thirtydays = true;
    }

    if (month == 2) {  // test for February
        feb = true;
    }

    /*
     * A year is a leap year if it is divisible by 4, but not by 100.
     *
     * If a year is divisible by 4 and by 100, it is a leap year only
     * if it is also divisible by 400.
     */
    if ((year%4 == 0 && year%100 != 0) ||
            (year%4 == 0 && year%100 == 0 && year%400 == 0)) {
        leap = true;
    }

    if ((day == 28) && (feb) && (!leap)) {
        setMonth();
        day = 1;
    }
    else if ((day == 29) && (feb) && (leap)) {
        setMonth();
        day = 1;
    }
    else if ((day == 30) && (thirtydays == true)) {
        setMonth();
        day = 1;
    }
    else if ((day == 31) && (thirtydays == false)) {
        setMonth();
        day = 1;
    }
    else {
        day++;
    }

    return;
}

void Clock::setMonth()
{
    if (month >= 12) {
        setYear();
        month = 1;
    }
    else {
        month++;
    }

    return;
}

void Clock::setYear()
{
    year++;
    if ((year%100) == 0) {
       // setCentury();
    }

    return;
}
uint64 Clock::getTicks()
{
    return ticks;
}

int Clock::getId()
{
    return id;
}
int Clock::getSecond()
{
    return second;
}

int Clock::getMinute()
{
    return minute;
}

int Clock::getHour()
{
    return hour;
}

int Clock::getDay()
{
    return day;
}

int Clock::getMonth()
{
    return month;
}

int Clock::getYear()
{
    return year;
}
/* cp56time2a 格式 */
/*
低地址
	|	millisecond		2字节
	|	minute			1字节 	bit0-5
    |	hour			1字节	bit0-4
    |	day and week	1字节	bit0-4 day of month bit 5-7 day of week (1-7)
	|	month			1字节	bit0-3
	\/	year			1字节	bit0-6
高地址

*/
CP56Time2a Clock::GetCP56Time2a(Uint16 GetType)
{
	CP56Time2a Time;
	if(GetType == GET8025){
		strTIME _8025time;
		rx8025_get_time(&_8025time);
		SynLock = 1;
		year =_8025time.sYear;
		day =_8025time.cDay;
		month = _8025time.cMonth;
		hour = _8025time.cHour;
		minute = _8025time.cMinute;
		second = _8025time.cSecond;
		millisecond = millisecond;
		SynLock = 0;
		
		Time.year =_8025time.sYear;
		Time.day =_8025time.cDay;
		Time.month = _8025time.cMonth;
		Time.hour = _8025time.cHour;
		Time.minute = _8025time.cMinute;
		Time.millisecond = _8025time.cSecond*1000+millisecond;
	}else{
		Time.year = year;
		Time.day = day;
		Time.month = month;
		Time.hour = hour;
		Time.minute = minute;
		Time.millisecond = second*1000+millisecond;
	}

	return Time;
}

//按照Per最大值 递加
void TimeAdd(CP56Time2a * T)
{
	T->millisecond++;
		if ( T->millisecond >= 60000 ) {
			T->millisecond = 0;
			T->minute++;
			if ( T->minute >= 60 ) {
				T->minute = 0;
				T->hour++;

				if ( T->hour >= 24 ) {
					T->hour = 0;
					T->day++;

					if ( ( T->month== 1 ) || ( T->month == 3 ) || ( T->month == 7 ) || ( T->month == 8 ) || ( T->month == 10 ) || ( T->month == 12 ) ) {
						if ( T->day>= 32 ) {

							T->day = 1;
							T->month++;

							if ( T->month >= 12 ) {
								T->month = 1;
								T->year++;
							}
						}
					} else if ( (  T->month == 4 ) || (  T->month == 6 ) || (  T->month == 9 ) || (  T->month == 11 ) ) {
						if ( T->day  >= 31 ) {
							T->day  = 1;
						}
					} else if (  T->month == 2 ) {
						if ( ( ( ( T->year % 4 ) == 0 ) && ( ( T->year % 100 ) != 0 ) ) || ( ( T->year % 400 ) == 0 ) ) {
							if ( T->day  >= 30 ) {
								T->day  = 1;
								T->month++;
							}
						} else {
							if ( T->day >= 29 ) {
								T->day = 1;
								T->month++;
							}
						}
					}
				}
			}
		}
}

//按照Per最大值 递加
void TimeMinus(CP56Time2a * T)
{
	if ( T->millisecond ==0 ) {
		T->millisecond = 999;
		if ( T->minute == 0 ) {
			T->minute = 59;
			if ( T->hour ==0 ) {
				T->hour =23;
				if ( ( T->month== 1 ) || ( T->month == 5) || ( T->month == 7 ) || ( T->month == 8 ) || ( T->month == 10 ) || ( T->month == 12 ) ) {
					if ( T->day==1 ) {
						T->day = 30;
						if ( T->month ==1 ) {
							T->month = 12;
							T->year--;
						}else{
							T->month--;
						}
					}else{
						T->day--;
					}
				} else if ((  T->month == 2 )|| (  T->month == 4 ) || (  T->month == 6 ) || (  T->month == 9 ) || (  T->month == 11 ) ) {
					if ( T->day==1 ) {
						T->day = 31;
						T->month--;
					}else{
						T->day--;
					}
				} else if (  T->month == 3 ) {
					if ( ( ( ( T->year % 4 ) == 0 ) && ( ( T->year % 100 ) != 0 ) ) || ( ( T->year % 400 ) == 0 ) ) {
						if ( T->day ==1 ) {
							T->day  = 29;
							T->month--;
						}else{
							T->day--;
						}
					} else {
						if ( T->day ==1 ) {
							T->day  = 28;
							T->month--;
						}else{
							T->day--;
						}
					}
				}
			}else{
				T->hour--;
			}
		}else{
			T->minute--;
		}
	}else{
		T->millisecond--;
	}
}

//已知 InTime，和毫秒偏移量DiffTms ,返回新的时间
CP56Time2a CP56Time2aSub(CP56Time2a InTime,uint32 DiffTms)
{
	int32 i;
	CP56Time2a retT =  InTime;
	for(i = 0; i <DiffTms; i++){
		TimeMinus(&retT);
	}
	return retT;
}
	
void Counter::set_clk(Clock*p){
	clk=p;
}
int16 Counter::StartCount(set_ms_time *RTUTimer,uint32 TdMs)
{
	if (RTUTimer->SetFlag !=1){//避免重复启动的情况。
		RTUTimer->SetFlag  = 1;
		RTUTimer->T = clk->GetSysMs() +  TdMs;
		return 1;
	}
	return -1;
};

int16 Counter::StopCount(set_ms_time *RTUTimer)
	{
		RTUTimer->SetFlag = 0;
		RTUTimer->T = 0;
		return 1;
	};

int16 Counter:: CheckCount(set_ms_time *RTUTimer)
{
	if (RTUTimer->SetFlag == 0){//未开始计时
		return SLEEP;
	}
	if(clk->GetSysMs() >=RTUTimer->T){//计时到达 访问后，计时器即使停止；
		RTUTimer->SetFlag = 0;
		RTUTimer->T = 0;
		return REACHED;
	}else {
		return RUNING;
	}
};

bool Counter::Plus01_T1_1_10_T2_0(bool InBool,int32 TdMs1,int32 TdMs2,BASEPROSTATU_T *statu)
{

	if(statu==NULL){
		while(1);
	}
	if (InBool){
		statu->CurrentMs2 = clk->GetSysMs() ;//+ TdMs2 ;
		if(clk->GetSysMs()  >= (statu->CurrentMs1+ TdMs1)){
			statu->CurrentBool = InBool;
		}
	}else{
		statu->CurrentMs1 = clk->GetSysMs()  ;//+ TdMs1 ;
		if(clk->GetSysMs() >= (statu->CurrentMs2+ TdMs2)){
			statu->CurrentBool = InBool;
		}
	}
	return statu->CurrentBool;
}

bool Counter::TCell(bool InBool,uint32 TdMs,BASEPROSTATU_T *statu)//输入由0变1 经过T后输出1；输入为0时，输出为0；
{
	if(statu==NULL){
		while(1);
	}
	if (InBool){
		if(clk->GetSysMs() >= (statu->CurrentMs1 + TdMs)){
			statu->CurrentBool = InBool;
		}
	}else{
		statu->CurrentMs1 = clk->GetSysMs() ;//+ TdMs;
		statu->CurrentBool = InBool;
	}
	return statu->CurrentBool ;
}

