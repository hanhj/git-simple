/*
 * clock.cpp
 *
 *  Created on: 2013-10-18
 *      Author: Administrator
 */

#include "os.h"
#include "clock.h"
#include "omapl138timer.h"
#include "types.h"
#include "datatype.h"
Clock clk0(0);

/*
 * Clock methods
 */
Clock::Clock(int newId)
{
    id = newId;
    oldticks = 0;
    newticks = 0;
    ticks = 0;
    microsecond = 0;
    millisecond = 0;
    second = 0;
    minute = 0;
    hour = 0;
    day = 19;
    month = 8;
    year = 10;
  //  century = 20;
  //  millenium = 0;
}

Clock::~Clock()
{
}

void Clock::tick()
{
    ticks++;
    millisecond+=1000;
    setSecond();
    return;
}
//双边触发器
int16 Clock::DTrigger(int16 InPut,uint32 Td1,uint32 Td2,setmstime_t *RTUTimer1,setmstime_t *RTUTimer2)
{
	int16 ret =-1;
	if(InPut==1){
		StopMsClock(RTUTimer2);
		if(CheckMsClock(RTUTimer1)==REACHED){
			ret = 1;
		}else{
			StartMsClock(RTUTimer1,Td1);
		}
	}else if (InPut==0){
		StopMsClock(RTUTimer1);
		if(CheckMsClock(RTUTimer2)==REACHED){
			ret = 0;
		}else{
			StartMsClock(RTUTimer2,Td2);
		}
	}
	return ret;
}
//单边触发器 输入由”0“到1，触发器立即触发返回1；
//输入一直为1时，第N次读取后 Td 内读取范围值为0，Td后读取返回值为1；
int16 Clock::STrigger(int16 InPut,uint32 Td,setmstime_t *RTUTimer)
{
	int16 clkstatu;
	int16 ret = 0;
	if(InPut==1){
		clkstatu = CheckMsClock(RTUTimer);
		if(clkstatu==REACHED){
			ret = 1;
			StopMsClock(RTUTimer);
			StartMsClock(RTUTimer,Td);
		}else if (clkstatu==SLEEP){
			ret = 1;
			StopMsClock(RTUTimer);
			StartMsClock(RTUTimer,Td);
		}
	}else if (InPut==0){
		StopMsClock(RTUTimer);
		StartMsClock(RTUTimer,0);
	}
	return ret;
}
uint64 Clock::GetSysMs()
{   uint32 ms;
	newticks = Clock_getTicks();
	if (newticks>=oldticks){
		ms = (newticks - oldticks)%1000;
	}else{
		ms = (oldticks - newticks)%1000;
	}
	oldticks = newticks;


	sysms  = millisecond + ms ;//((TMR3->TIM12)/150000);
	return sysms;
}
int Clock::StartMsClock(setmstime_t *RTUTimer,uint64 TdMs)
{
	uint64 ntime;
	if ((*RTUTimer).SetFlag!=1){
		RTUTimer->SetFlag  = 1;
		ntime = GetSysMs();
		(*RTUTimer).T = ntime +  TdMs;
		return 1;
	}
	return -1;
}
int Clock::StopMsClock(setmstime_t *RTUTimer)
{
	RTUTimer->SetFlag = 0;
	RTUTimer->T = 0;
	return 1;
}

int Clock::CheckMsClock(setmstime_t *RTUTimer)
{
    if (RTUTimer->SetFlag!=1){//未开始计时
    	return SLEEP;
    }
	if(GetSysMs()>=RTUTimer->T){
		RTUTimer->SetFlag = 0;
		RTUTimer->T = 0;
		return REACHED;
	}else {
		return RUNING;
	}

}
int Clock::CheckClock(settime_t *RTUTimer)
{
	CP56Time2a NewTime;
    if (RTUTimer->SetFlag!=1){//未开始计时
    	return -2;
    }
	NewTime = GetCP56Time2a();
	if(NewTime.year>RTUTimer->T.year){
		RTUTimer->SetFlag = 0;
		return 1;
	}else if (NewTime.month>RTUTimer->T.month){
		RTUTimer->SetFlag = 0;
		return 1;
	}else if (NewTime.day_week>RTUTimer->T.day_week){
		RTUTimer->SetFlag = 0;
		return 1;
	}else if (NewTime.hour_su>RTUTimer->T.hour_su){
		RTUTimer->SetFlag = 0;
		return 1;
	}else if (NewTime.minute_iv>RTUTimer->T.minute_iv){
		RTUTimer->SetFlag = 0;
		return 1;
	}else if (NewTime.millisecond>=RTUTimer->T.millisecond){
		RTUTimer->SetFlag = 0;
		return 1;
	}else {
		return 0;
	}
	//return 0;
}
//Tn 当前时间，Ta 定时时间，返回定时满时间
CP56Time2a Clock::AndClock(CP56Time2a Tn,CP56Time2a Ta)
{	uint8 i;
    bool thirtydays = false;
    bool feb = false;
    bool leap = false;
	uint32 Tmillisecond = 0;
	uint16 Tminute = 0;
	uint16 Thour =0;
	uint16 Tmonth = 0;
	uint16 Tyear = 0;
	uint16 Tday = 0;
	CP56Time2a T1,T2,T3;
	T1 = Tn;
	T2 = Ta;
	Tmillisecond = T1.millisecond + T2.millisecond;
	if(Tmillisecond>=60000){
		Tmillisecond-= 60000;
		Tminute++;
	}
	Tminute +=T1.minute_iv + T2.minute_iv;
	if(Tminute>=60){
		Tminute-= 60;
		Thour++;
	}
	Thour+=T1.hour_su + T2.hour_su;
	if (Thour>=24){
		Thour-=24;
		Tday++;
	}
	for (i = 0 ;i < T2.day_week; i++){
		if (T1.month == 4 || T1.month == 6 || T1.month == 9 || T1.month == 11) {
			thirtydays = true;
		}
		if (T1.month == 2) {
			feb = true;
		}
		if ((T1.year%4 == 0 && T1.year%100 != 0) ||
				(T1.year%4 == 0 && T1.year%100 == 0 && T1.year%400 == 0)) {
			leap = true;
		}
		if ((T1.day_week == 28) && (feb) && (!leap)) {
			T1.month++;
			if (T1.month>12){
				T1.month = 1;
				T1.year++;
			}
			T1.day_week = 1;
		}else if ((T1.day_week == 29) && (feb) && (leap)) {
			T1.month++;
			if (T1.month>12){
				T1.month = 1;
				T1.year++;
			}
			T1.day_week = 1;
		}else if ((T1.day_week == 30) && (thirtydays == true)) {
			T1.month++;
			if (T1.month>12){
				T1.month = 1;
				T1.year++;
			}
			T1.day_week = 1;;
		}else if ((T1.day_week == 31) && (thirtydays == false)) {

			T1.month++;
			if (T1.month>12){
				T1.month = 1;
				T1.year++;
			}
			T1.day_week = 1;
		}else {
			T1.day_week++;
		}
	}
	Tday = T1.day_week;
	Tmonth=T1.month + T2.month;
	if(Tmonth>12)
	{
		Tmonth-=12;
		Tyear++;
	}
	Tyear+=T1.year+T2.year;
	T3.day_week = Tday;
	T3.month = Tmonth;
	T3.year = Tyear;
	T3.millisecond = Tmillisecond;
	T3.hour_su  = Thour;
	T3.minute_iv = Tminute;
	return T3;
}
/*
CP56Time2a Clock::DiffClock(CP56Time2a MinuendT,CP56Time2a SubtrahendT)
{

}
*/
int Clock::StartClock(settime_t *RTUTimer,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute,uint16 millisecond)
{
	CP56Time2a ntime;
	if ((*RTUTimer).SetFlag!=1){
		RTUTimer->SetFlag = 1;
		(*RTUTimer).T.day_week = day;
		(*RTUTimer).T.month = month;
		(*RTUTimer).T.year = year;
		(*RTUTimer).T.millisecond = millisecond;
		(*RTUTimer).T.hour_su  = hour;
		(*RTUTimer).T.minute_iv = minute;
		ntime = GetCP56Time2a();
		(*RTUTimer).T = AndClock(ntime,(*RTUTimer).T);
		return 1;
	}
	return -1;
}
int Clock::StopClock(settime_t *RTUTimer)
{
	RTUTimer->SetFlag = 0;
	RTUTimer->T.year = 0;
	RTUTimer->T.month = 0;
	RTUTimer->T.day_week = 0;
	RTUTimer->T.hour_su = 0;
	RTUTimer->T.minute_iv = 0;
	RTUTimer->T.millisecond = 0;
	return 1;
}
void Clock::setMicrosecond()
{
    if (microsecond >= 999) {
        setMillisecond();
        microsecond = 0;
    }
    else {
        microsecond++;
    }

    return;
}

void Clock::setMillisecond()
{
    if (millisecond >= 999) {
        setSecond();
        millisecond = 0;
    }
    else {
        millisecond++;
    }

    return;
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
/*
void Clock::setCentury()
{
    century++;
    if ((century%10) == 0) {
        setMillenium();
    }

    return;
}

void Clock::setMillenium()
{
    millenium++;

    return;
}
*/
long Clock::getTicks()
{
    return ticks;
}

int Clock::getId()
{
    return id;
}

int Clock::getMicrosecond()
{
    return microsecond;
}

int Clock::getMillisecond()
{
    return millisecond;
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
CP56Time2a Clock::GetCP56Time2a()
{
	uint32 ms;

	newticks = Clock_getTicks();
	if (newticks>=oldticks){
		ms = (newticks - oldticks)%1000;
	}else{
		ms = (oldticks - newticks)%1000;
	}
	oldticks = newticks;
	Time.year = year;
	Time.day_week = day;
	Time.month = month;
	Time.hour_su = hour;
	Time.minute_iv = minute;
	Time.millisecond = second*1000 + ms;//(TMR3->TIM12)/150000;

	return Time;
}
//int Clock::getCentury()
//{
//    return century;
//}

//int Clock::getMillenium()
//{
//    return millenium;
//}
