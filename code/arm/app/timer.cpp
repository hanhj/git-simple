#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include<linux/rtc.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "device.h"// for rtc device
#include "os.h"// for mutex

#include "timer.h"
#include "task.h"
#include "debug.h"
#include "message.h"

Timer timer;



void *timer_main ( void *arg )
{
    timer.Run ( arg );
    return NULL;
}

/** /brief 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2 ( MessageData<BaseDataType>message )
{
    timer.GetMsg ( message );
    return 1;
}

Timer::Timer ( void )
{
    strcpy ( name, "Timer" );
    Id = TASK_TIMER;
}

Timer::~Timer ( void )
{
    PFUNC ( TEM_WARNING,DEBUG_TIMER, "Destroy Timer\r\n" );
}

int16 Timer::Init ( void )
{

    time_t  now;
    int i;

    struct tm   *timenow;         //实例化tm结构指针
    msg.RegistMsg ( Id, GetMsg2 );///<向message类注册线程号以及消息回调函数
       
    time ( &now );
    timenow = localtime ( &now );
    memcpy ( ( char* ) &sys_time, ( char* ) timenow, sizeof ( struct tm ) );
    sys_time.year += 1900;
#ifdef CONFIG_LINUX
    gettimeofday ( &tm_2, 0 );
#else

#endif
    time_counter = tm_2.tv_sec;
    memset ( time_table, 0, sizeof ( time_table ) );

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        time_table[i].start = 0;
        time_table[i].flag = 0;
        time_table[i].counter = 0;
        time_table[i].id = 100 + i;
    }

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        alarm_table[i].start = 0;
        alarm_table[i].flag = 0;
        alarm_table[i].id = 200 + i;
        alarm_table[i].now_point = 0;
    }

    return 1;
}

int16 Timer::Run ( void*arg )
{
    Init();

    while ( 1 ) {
        delay ( 1 );
        //PFUNC(TEM_INFO ,DEBUG_TIMER,"%%%%%%%%%%%%this is test timer \n");
        CheckTimer();
        CheckAlarm();
        OnTimer();
        Heart();
        CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }
		msg.UnRegistMsg ( Id );
    PFUNC ( TEM_WARNING,DEBUG_TIMER, "Exit Timer Run\r\n" );

    return 1;
}

uint8 Timer::SetAlarm ( uint16* id, uint8 interval, SYSTIME time , void ( *pfunc ) ( void ) )
{
    *id = 0;
    uint8 i;

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        if ( alarm_table[i].start == 0 ) {
            *id = alarm_table[i].id;
            alarm_table[i].time = time;
            alarm_table[i].point = interval;
            alarm_table[i].pfunc = pfunc;
            alarm_table[i].flag = 0;
            alarm_table[i].start = 1;
            return 1;
        }
    }
PFUNC(TEM_ERROR,DEBUG_TIMER,"exceed max alarm:%d\r\n",MAX_ALARM_NUM);
    return 0;
}

uint8 Timer::SetTimer ( uint16 *id, uint32 s , void ( *pfunc ) ( void ) )
{

    *id = 0;
    uint8 i;

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        if ( time_table[i].start == 0 ) {
            *id = time_table[i].id;
            time_table[i].cycle = s;
            time_table[i].pfunc = pfunc;
            time_table[i].flag = 0;
            time_table[i].counter = 0;
            time_table[i].start = 1;
            return 1;
        }
    }
PFUNC(TEM_ERROR,DEBUG_TIMER,"exceed max timer:%d\r\n",MAX_TIMER_NUM);

    return 0;

}

void Timer::KillTimer ( uint16 id )
{
    uint8 i;

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        if ( time_table[i].id == id ) {
            time_table[i].cycle = 0;
            time_table[i].pfunc = 0;
            time_table[i].counter = 0;
            time_table[i].start = 0;
        }
    }
}

void Timer::KillAlarm ( uint16 id )
{
    uint8 i;

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        if ( alarm_table[i].id == id ) {
            memset ( ( char* ) & alarm_table[i].time, 0, sizeof ( SYSTIME ) );
            alarm_table[i].point = 0;
            alarm_table[i].pfunc = 0;
            alarm_table[i].start = 0;
            return ;
        }
    }
}

void Timer::OnTimer()
{
    uint8 i;

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        if ( alarm_table[i].start == 1 && alarm_table[i].flag == 1 ) {
            if ( alarm_table[i].pfunc != 0 ) {
                alarm_table[i].flag = 0;
                ( * alarm_table[i].pfunc ) ();
            }
        }
    }

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        if ( time_table[i].start == 1 && time_table[i].flag == 1 ) {
            if ( time_table[i].pfunc != 0 ) {
                time_table[i].flag = 0;
                ( * time_table[i].pfunc ) ();
            }
        }
    }
}


void Timer::GetRtcTime ( struct tm *time )
{
	int fd=-1;
  fd = open(RTC_DEV, O_RDWR);
  if (fd <0){
	PFUNC(TEM_ERROR,DEBUG_TIMER,"Open %s error\n", RTC_DEV);
  }	
	ioctl(fd, RTC_RD_TIME, time); 
	close(fd);
}
void Timer::SetRtcTime ( struct tm *time )
{
	int fd;

	fd = open(RTC_DEV, O_RDWR);
	if (fd <0) {
		PFUNC(TEM_ERROR, DEBUG_TIMER,"open rtc device error\n");
		return ;
	}

	if (ioctl(fd, RTC_SET_TIME,time)<0) {
		PFUNC(TEM_ERROR, DEBUG_TIMER,"set time error\n");
		close(fd);
	}
	close(fd);
}

void Timer::GetCP56Time ( CP56Time2a *ptime )
{

    struct  timeval start;

    struct tm *p;
    time_t now;
    
#ifdef CONFIG_LINUX
p = localtime ( &now );
    gettimeofday ( &start, 0 );//获得微秒
    time ( &now );//获得自1970年的秒
    p = localtime ( &now );
#else
#endif
    ptime->millisecond = p->tm_sec * 1000 + start.tv_usec / 1000;
    ptime->minute_iv = p->tm_min;
    ptime->hour_su = p->tm_hour;

    if ( p->tm_wday == 0 )
        p->tm_wday = 7;

    ptime->day_week = p->tm_mday | ( ( p->tm_wday ) << 5 );

    ptime->month = p->tm_mon + 1;

    ptime->year = p->tm_year - 100;//tm_year是1900年以来的年，而ptime->year从2000开始计算所以要减100


}

void Timer::GetLocalTime ( SYSTIME *ret_time )
{
	
    time_t   now;         //实例化time_t结构
    struct tm   *p;         //实例化tm结构指针
    time ( &now );
    p = localtime ( &now );
    
    p->tm_year = p->tm_year-100;	
    p->tm_mon=p->tm_mon+1;
    if ( p->tm_wday == 0 )
        p->tm_wday = 7;   
    
	ret_time->sec=p->tm_sec;

	ret_time->min= p->tm_min;

	ret_time->hour= p->tm_hour;

	ret_time->mday= p->tm_mday;

	ret_time->mon= p->tm_mon;

	ret_time->year= p->tm_year;

	ret_time->wday= p->tm_wday;

	ret_time->yday= p->tm_yday;    
    
}

void Timer::GetLocalTime ( uint8 *ret_time )
{
	SYSTIME time;
	GetLocalTime(&time);
	ret_time[0]=time.sec;
	ret_time[1]=time.min;
	ret_time[2]=time.hour;
	ret_time[3]=time.mday;
	ret_time[4]=time.mon;
	ret_time[5]=time.year;
}
//获得UTC时间
void Timer::GetUtcTime ( SYSTEMTIME* lpSystemTime )
{

    struct  timeval tv;
    struct tm *p;
    
    gettimeofday ( &tv, NULL );//获得微秒,not pay attention timezone

    p=gmtime(&tv.tv_sec);

	lpSystemTime->wYear=p->tm_year+1900;
	lpSystemTime->wMonth=p->tm_mon+1;
	lpSystemTime->wDayOfWeek=p->tm_wday;
	lpSystemTime->wDay=p->tm_mday;
	lpSystemTime->wHour=p->tm_hour;
	lpSystemTime->wMinute=p->tm_min;
	lpSystemTime->wSecond=p->tm_sec;
    lpSystemTime->wMilliseconds= p->tm_sec * 1000 + tv.tv_usec / 1000;

	PFUNC(TEM_INFO,DEBUG_TIMER,"UTC time year:%d mon:%d week:%d day:%d hour:%d min:%d sec:%d millisec:%d\n",
						lpSystemTime->wYear,
						lpSystemTime->wMonth,
						lpSystemTime->wDayOfWeek,
						lpSystemTime->wDay,
						lpSystemTime->wHour,
						lpSystemTime->wMinute,
						lpSystemTime->wSecond,
					    lpSystemTime->wMilliseconds);    
}

/**
set system time by uint8 time array,time is localtime.
@time:local time data str:[0]:sec,[1]:min,[2]:,[3]:day,[4]:mon,[5]:year 
*/
void Timer::SetLocalTime ( uint8 *time ){
    struct tm tmTime;
	struct tm wTime;
	struct timeval tv = { 0, 0};

    tmTime.tm_sec = time[0];
    tmTime.tm_min = time[1];
    tmTime.tm_hour = time[2];
    tmTime.tm_mday = time[3];
    tmTime.tm_mon = time[4];
    tmTime.tm_year = time[5]+2000-1900;//because tm_year is from 1900
	tmTime.tm_wday = 0;
	if (tmTime.tm_mon >= 1) {//because tm_mon is from 0
        tmTime.tm_mon--;
	}
    memcpy(&wTime, &tmTime, sizeof(tmTime));
	tv.tv_sec = mktime(&wTime);
	settimeofday ( &tv, NULL );//not pay attention timezone
	SetRtcTime(&wTime);
}

/**
set time by systemtime stru,time is localtime.
@time:systemtime pointer 
*/
void Timer::SetLocalTime ( SYSTEMTIME* lpSystemTime ){
    struct tm tmTime;
	struct tm wTime;
	struct timeval tv = { 0, 0};

	PFUNC(TEM_INFO,DEBUG_TIMER,"Local time year:%d mon:%d week:%d day:%d hour:%d min:%d sec:%d millisec:%d\n",
						lpSystemTime->wYear,
						lpSystemTime->wMonth,
						lpSystemTime->wDayOfWeek,
						lpSystemTime->wDay,
						lpSystemTime->wHour,
						lpSystemTime->wMinute,
						lpSystemTime->wSecond,
					    lpSystemTime->wMilliseconds);   	

	tmTime.tm_year=lpSystemTime->wYear-1900,
	tmTime.tm_mon=lpSystemTime->wMonth-1,
	tmTime.tm_wday=lpSystemTime->wDayOfWeek,
	tmTime.tm_mday=lpSystemTime->wDay;
	tmTime.tm_hour=lpSystemTime->wHour;
	tmTime.tm_min=lpSystemTime->wMinute;
	tmTime.tm_sec=lpSystemTime->wSecond;

  
    memcpy(&wTime, &tmTime, sizeof(tmTime));
	tv.tv_sec = mktime(&wTime);
	settimeofday ( &tv, NULL );
	SetRtcTime(&wTime);
}

void Timer::SynRtc(void){
	struct tm rtcTime;
	struct timeval tv = { 0, 0};

	GetRtcTime(&rtcTime);
	tv.tv_sec = mktime(&rtcTime);
	settimeofday ( &tv, NULL);

}
uint8 Timer::CheckTimer ( void )
{
    uint8 i;
    time_t d_sec = 0;
#ifdef CONFIG_LINUX
    gettimeofday ( &tm_2, 0 );
#else

#endif

    if ( tm_2.tv_sec != time_counter ) {
        d_sec = tm_2.tv_sec - time_counter;
        //PFUNC(TEM_INFO ,DEBUG_TIMER,"tm_2.tv_sec=%lu,tm_1.tv_sec=%lu,d_sec=%lu\n",tm_2.tv_sec,time_counter,d_sec);
        time_counter = tm_2.tv_sec;
    }

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        if ( time_table[i].start == 1 ) {
            time_table[i].counter += d_sec;

            if ( time_table[i].counter >= time_table[i].cycle ) {
                time_table[i].counter = 0;
                time_table[i].flag = 1;
            }
           
        }
    }

    return 1;
}

uint8 Timer::CheckAlarm ( void )
{
    uint8 i;
    time_t   now;         //实例化time_t结构

    struct tm   *timenow;         //实例化tm结构指针
    time ( &now );
    timenow = localtime ( &now );
    memcpy ( ( char* ) &sys_time, ( char* ) timenow, sizeof ( struct tm ) );
    sys_time.year += 1900;

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        if ( alarm_table[i].start == 1 ) {
            switch ( alarm_table[i].point ) {

            case Min:

                if ( alarm_table[i].now_point != sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.min;
                    alarm_table[i].flag = 1;
                } 

                break;

            case Hour:
                if ( alarm_table[i].now_point != sys_time.hour
                        && alarm_table[i].time.min == sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.hour;
                    alarm_table[i].flag = 1;
                }

                break;

            case Day:

                if ( alarm_table[i].now_point != sys_time.mday
                        && alarm_table[i].time.hour == sys_time.hour
                        && alarm_table[i].time.min == sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.mday;
                    alarm_table[i].flag = 1;
                }

                break;

            case Week:

                if ( alarm_table[i].now_point != sys_time.wday
                        && alarm_table[i].time.wday == sys_time.wday
                        && alarm_table[i].time.hour == sys_time.hour
                        && alarm_table[i].time.min == sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.wday;
                    alarm_table[i].flag = 1;
                }

                break;

            case Month:

                if ( alarm_table[i].now_point != sys_time.mon
                        && alarm_table[i].time.mday == sys_time.mday
                        && alarm_table[i].time.hour == sys_time.hour
                        && alarm_table[i].time.min == sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.mon;
                    alarm_table[i].flag = 1;
                }

                break;

            case Year:

                if ( alarm_table[i].now_point != sys_time.year
                        && alarm_table[i].time.mon == sys_time.mon
                        && alarm_table[i].time.mday == sys_time.mday
                        && alarm_table[i].time.hour == sys_time.hour
                        && alarm_table[i].time.min == sys_time.min
                        && alarm_table[i].time.sec == sys_time.sec ) {
                    alarm_table[i].now_point = sys_time.year;
                    alarm_table[i].flag = 1;
                }

                break;

            default:
                break;

            }

            return 1;
        }
    }

    return 0;
}

uint8 Timer::IsTimesUp ( uint16 id )
{
    uint8 i;

    for ( i = 0;i < MAX_ALARM_NUM;i++ ) {
        if ( alarm_table[i].id == id ) {
            if ( alarm_table[i].flag == 1 )
            	{
            		alarm_table[i].flag=0;
                return 1;
              }
            else
                return 0;
        }
    }

    for ( i = 0;i < MAX_TIMER_NUM;i++ ) {
        if ( time_table[i].id == id ) {
            if ( time_table[i].flag == 1 )
            	{
            		time_table[i].flag = 0;
                return 1;
              }
            else
                return 0;
        }
    }

    return 0;
}

int16 Timer::GetMsg ( MessageData<BaseDataType>message )
{
    PFUNC ( TEM_INFO ,DEBUG_TIMER, "%s deal  Message\r\n", name );

    if ( message.MessageId == MSG_YX ) {
        PFUNC ( TEM_INFO ,DEBUG_TIMER, "%s rcv Yx Message\r\n", name );

    }

    if ( message.MessageId == MSG_YC ) {
        PFUNC ( TEM_INFO ,DEBUG_TIMER, "%s rcv YC Message\r\n", name );
    }

    if ( message.MessageId == MSG_YK ) {
        PFUNC ( TEM_INFO ,DEBUG_TIMER, "%s rcv YK Message\r\n", name );
    }

    if ( message.MessageId == MSG_PARA ) {
        PFUNC ( TEM_INFO ,DEBUG_TIMER, "%s rcv PARA Message\r\n", name );
    }

    return 1;
}

#ifndef CONFIG_LINUX
void sleep ( int32 time )
{
    Task_sleep ( time );
}

#endif
