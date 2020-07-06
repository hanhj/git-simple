/*
 * clock.h
 *
 *  Created on: 2013-10-18
 *      Author: Administrator
 */

#ifndef CLOCK_H_
#define CLOCK_H_
#include "datatype.h"
#include "rx8025.h"
#define GET8025 1
#define GETSOFTTIME 0

#define RUNING -1
#define REACHED 1
#define SLEEP -2

class Clock {
    private:
         int id;
         uint64 millisecond;
         int second;
         int minute;
         int hour;
         int day;
         uint8 week;
         int month;
         int year;
         uint8 SynLock;
    public:
        Clock(int newId);
        ~Clock();

        uint64 ticks;//开机后的系统秒
        uint64 sysms;//开机后的系统毫秒；
		void reset(void );
        int ClockSynchronization(CP56Time2a *syntime);
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
class Counter{
	Clock *clk;
public:
	void set_clk(Clock*);
	int16 StartCount(set_ms_time *RTUTimer,uint32 TdMs);
	int16 CheckCount(set_ms_time *RTUTimer);
	int16 StopCount(set_ms_time *RTUTimer);
	bool Plus01_T1_1_10_T2_0(bool InBool,int32 TdMs1,int32 TdMs2,BASEPROSTATU_T *statu);
	bool TCell(bool InBool,uint32 TdMs,BASEPROSTATU_T *statu);
};

extern Clock clk0;
extern Counter T_tool;

#ifdef __cplusplus
extern "C" {
#endif
CP56Time2a CP56Time2aSub(CP56Time2a InTime,uint32 DiffTms);
extern void TimeAdd(CP56Time2a * T);
extern void TimeMinus(CP56Time2a * T);

#ifdef __cplusplus
}
#endif
#endif /* CLOCK_H_ */
