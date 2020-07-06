
/**
*********************************************************************************************************************
*   @file timer.h
*   @brief 定时器头文件
*   @warning 最多支持32个定时器同时工作，定时周期以s计算
*   @param
*   @author  liht
*   @date  2013.07.24
*   @version  v1.0.0.0
*   @note   由处理函数交给定时器处理的工作耗时必须小于500ms
*
**********************************************************************************************************************
*/

#ifndef TIMER_H
#define TIMER_H
#include "baseapp.h"
#include "datatype.h"



#define MAX_ALARM_NUM 16
#define MAX_TIMER_NUM 16

typedef struct timer_tab_ {
    uint16 id;/**<  定时器ID  */
    uint8 start;/**<  定时器启动标志 1启动 0关闭 */
    uint32 cycle;/**< 定时周期  */
    uint32 counter;
    uint8 flag;
    void ( *pfunc ) ( void );/**<  对应的处理函数指针  */
} TIME_TABLE;

typedef struct alarm_tab_ {
    uint16 id;/**<  闹钟ID  */
    uint8 start;/**<  闹钟启动标志 1启动 0关闭 */
    uint16  point;/**<  闹钟间隔  */
    uint16  now_point;
    uint8 flag;
    SYSTIME time;/**<  闹钟时间点  */
    void ( *pfunc ) ( void );/**<  对应的处理函数指针  */
} ALARM_TABLE;

enum WEEK_DAY { Sunday = 1, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};
enum POINT {Min = 0, Hour = 1, Day, Week, Month, Year};
/**
************************************************************************************************************
*  @class  TIMER
*  @brief  提供定时器服务，以及系统时钟读取服务
*  @author  liht
*  @note   通过set_timer 接口启动对应的定时器服务，并将需要处理的函数指针传递给服务程序。
*  detailed
************************************************************************************************************
*/

class Timer: public BaseApp
{

public:
    Timer ( void );
    ~Timer ( void );
    int16 Init();
    /**
    *************************************************************************************
    *  @brief  运行模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Run ( void*arg );

    void OnTimer();
    /**
    *************************************************************************************
    *  @brief  启动定时器服务接口
    *  @author  liht
    *  @param[in]  uint16 id 定时器ID
    *  @param[in]  uint32 s 定时器周期，单位秒
    *  @param[in]  void (*pfunc)(void) 其他模块的函数指针
    *  @return  1 成功 0 失败
    *  @see  CTIMER
    **************************************************************************************
    */

    uint8 SetTimer ( uint16* id, uint32 s , void ( *pfunc ) ( void ) );
    uint8 SetAlarm ( uint16* id, uint8 interval, SYSTIME time , void ( *pfunc ) ( void ) );
    uint8 IsTimesUp ( uint16 id );
    void KillTimer ( uint16 id );
    void KillAlarm ( uint16 id );

    void GetCP56Time ( CP56Time2a *time );
    void GetLocalTime(uint8*time);
    void GetLocalTime ( SYSTIME *tm );       
	void GetUtcTime ( SYSTEMTIME* lpSystemTime );//UTC时间
    void SetLocalTime(uint8*time);
	void SetLocalTime ( SYSTEMTIME* lpSystemTime );
    void SynRtc(void);
    
    /**
    *************************************************************************************
    *  @brief  处理本模块获得的消息.
    *  @author  liht
    *  @param [in] MessageData*message，消息类型定义参见datatype.h.
    *  @param [out]  .
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 GetMsg ( MessageData<BaseDataType>message );

private:
    TIME_TABLE time_table[MAX_TIMER_NUM];/**<  Detailed  description  after  the  member  */
    ALARM_TABLE alarm_table[MAX_ALARM_NUM];/**<  Detailed  description  after  the  member  */
    SYSTIME sys_time;/**<  Detailed  description  after  the  member  */
    void SetRtcTime ( struct tm *time );
    void GetRtcTime ( struct tm *time );
    
	struct  timeval tm_2;
	time_t  time_counter ;    
    uint8 CheckTimer ( void );
    uint8 CheckAlarm ( void );    
};

#ifndef CONFIG_LINUX
void sleep ( int32 );
#else
#include <unistd.h>///<for sleep
#endif

extern Timer timer;
extern void *timer_main ( void *arg );
#endif

