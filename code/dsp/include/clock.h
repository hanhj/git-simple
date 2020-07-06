/*
 * clock.h
 *
 *  Created on: 2013-10-18
 *      Author: Administrator
 */

#ifndef CLOCK_H_
#define CLOCK_H_
#include "datatype.h"
#define RUNING -1
#define REACHED 1
#define SLEEP -2

class Clock {
    private:
         // data
         int id;
         uint32 oldticks;
         uint32 newticks;
         uint64 ticks;
         uint64 sysms;
         int microsecond;
         uint64 millisecond;
         int second;
         int minute;
         int hour;
         int day;
         int month;
         int year;
        // int century;
        // int millenium;
         //Diags_Mask clockLog;

    public:
        // methods
        CP56Time2a Time;
        Clock(int newId);  // Constructor
        ~Clock();                           // Destructor
        void tick();
        int16 DTrigger(int16 InPut,uint32 Td1,uint32 Td2,setmstime_t *RTUTimer1,setmstime_t *RTUTimer2);
        int16 STrigger(int16 InPut,uint32 Td,setmstime_t *RTUTimer);
        int CheckMsClock(setmstime_t *RTUTime);
        int StopMsClock(setmstime_t *RTUTimer);
        int StartMsClock(setmstime_t *RTUTimer,uint64 TdMs);
        int CheckClock(settime_t *RTUTimer);
        int StopClock(settime_t *RTUTimer);
        int StartClock(settime_t *RTUTimer,uint8 year,uint8 month,uint8 day,uint8 hour,uint8 minute,uint16 millisecond);
        CP56Time2a AndClock(CP56Time2a T1,CP56Time2a T2);
        long getTicks();
        int getId();
        uint64 GetSysMs();
        int getMicrosecond();
        int getMillisecond();
        int getSecond();
        int getMinute();
        int getHour();
        int getDay();
        int getMonth();
        int getYear();
        //int getCentury();
        //int getMillenium();
        CP56Time2a GetCP56Time2a();
        void setMicrosecond();
        void setMillisecond();
        void setSecond();
        void setMinute();
        void setHour();
        void setDay();
        void setMonth();
        void setYear();

       // void setCentury();
       // void setMillenium();
};

/*String months[12] = { "January", "February", "March",
                      "April",   "May",      "June",
                      "July",    "August",   "September",
                      "October", "November", "December" };*/

extern Clock clk0;
#endif /* CLOCK_H_ */
