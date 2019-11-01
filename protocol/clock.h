/*
 * Clock.h
 *
 *  Created on: 2014-12-4
 *      Author: Administrator
 */

#ifndef CLOCK_H_
#define CLOCK_H_
#include "datatype.h"
#define GET8025 1
#define GETSOFTTIME 0

class Clock {
    private:
         int id;
         uint16 millisecond;
         int second;
         int minute;
         int hour;
         int day;
         uint8 week;
         int month;
         int year;
         uint8 SynLock;
    public:
        CP56Time2a Time;
         uint64 ticks;//�������ϵͳ��
         uint64 sysms;//�������ϵͳ���룻
        Clock(int newId);
        ~Clock();
	void reset(void );
        int ClockSynchronization(CP56Time2a syntime);
        void tick(void);
		void milli_tick(void);
        uint64 getTicks();
        int getId();
        uint64 GetSysMs();
        int getSecond();
        int getMinute();
        int getHour();
        int getDay();
        int getMonth();
        int getYear();
        CP56Time2a GetCP56Time2a(Uint16 GetType);
        void setSecond();
        void setMinute();
        void setHour();
        void setDay();
        void setMonth();
        void setYear();
};

#define RUNING -1
#define REACHED 1
#define SLEEP -2
class Counter{
public:
	int16 StartCounter(uint32 TdMs,uint64 SysMs,setmstime_t *RTUTimer);
	int16 CheckCounter(uint64 SysMs,setmstime_t *RTUTimer);
	int16 StopCounter(setmstime_t *RTUTimer);
	bool Plus01_T1_1_10_T2_0(bool InBool,int32 TdMs1,int32 TdMs2,uint64 SysMs,BASEPROSTATU_T *statu);
	bool TCell(bool InBool,uint32 TdMs,uint64 SysMs,BASEPROSTATU_T *statu);//������0��1 ����T�����1������Ϊ0ʱ�����Ϊ0��
};


/*String months[12] = { "January", "February", "March",
                      "April",   "May",      "June",
                      "July",    "August",   "September",
                      "October", "November", "December" };*/

extern Clock clk0;
extern Uint64 Sys500us;
extern Uint64 SystemMs;
extern int 	SecondChageFlag;
extern CP56Time2a TerTime;
#ifdef __cplusplus
extern "C" {
#endif
extern CP56Time2a get_time(Uint16 Type);
extern void  update_time(CP56Time2a);
extern void TimeAdd(CP56Time2a * T);
extern void TimeMinus(CP56Time2a * T);
extern CP56Time2a CP56Time2aSub(CP56Time2a InTime,uint32 DiffTms);
extern Counter T_tool;

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
