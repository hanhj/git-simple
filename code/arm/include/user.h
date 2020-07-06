#ifndef USER_H
#define USER_H

#define CONFIG_LINUX		//this define is used for if this app is run on linux or dsp
#define CONFIG_SYSLINK		//this define is used for if this app have syslink modal
//#define CONFIG_FTU			//this define is used for if this app is run as ftu or dtu
#define CONFIG_DTU			//this define is used for if this app is run as ftu or dtu
#define WITH_MANAGE			//this define is used for if this app is run with manage monitor

#include "product.h"

#define MAXTASK 32
//TASK_ID
//Arm
#define TASK_STATIS 1<<0
#define TASK_PRO 1<<1
#define TASK_COM 1<<2
#define TASK_VDSP 1<<3
#define TASK_BATT 1<<4
#define TASK_MSG 1<<5
#define TASK_TIMER 1<<6
#define TASK_ALL TASK_STATIS|TASK_PRO|TASK_COM|TASK_TIMER

#define DEFAULT_TIMEOUT 60 
#define DEFAULT_RESTART_TIMES 2

//DSP
#define TASK_SCADA 1<<0
#define TASK_FA 1<<1
#define TASK_VARM 1<<2


#define 	SOCKET_THD (1<<7)
#define 	UART_THD (1<<8)

#define NO_DEAL_MSG_TIME 4 //消息队列中未处理消息最长存活时间 单位秒


typedef struct buf_frame {
    uint16 in_cnt;/**<   写缓存位置指针  */
    uint16 out_cnt;/**<  读缓存位置指针  */
    char buf[FRAME_SIZE];/**< 512bytes 缓存空间  */

}__attribute__ ((packed))BUF_FRAME;


typedef struct buf_data {
    uint16 in_cnt;/**<  写缓存位置指针  */
    uint16 out_cnt;/**<  读缓存位置指针  */
    char buf[BUFFER_SIZE];/**<  4k bytes 缓存空间  */
}__attribute__ ((packed))BUF_DATA;

typedef struct buf_mux_ {
    //struct mutex com_mutex;/**<  线程同步锁  */
    BUF_DATA recv;/**<  接收缓存  */
    BUF_DATA send;/**<  发送缓存  */
    BUF_FRAME frame;/**<  一帧缓存  */
    BUF_FRAME send_frame_copy;
    uint16 port_id;
    uint8 pro_id;
    uint16 forward_port_id;
    //通道初始化函数
    int (*fun_init)(int port,void *);
    //通道复位函数
    int (*fun_reset)(int port);
    //通道发送数据函数
	int (*fun_sendbuf)(int port, uint8 *buf, int len);
	//通道接受数据函数
	int (*fun_getbyte)( int port, uint8 *c );   
	//通道检查函数
	int (*fun_checkstate)( int port);    
	//关闭通道函数
	int (*fun_close)( int port);  		
}__attribute__ ((packed))BUF_MUX;

typedef struct tag_thread_data {
	int fd_index;
	void *para;
	BUF_MUX *p_buf;
}THREAD_DATA;

typedef struct time_ {//it same as struct tm
    int sec; /*秒，正常范围0-59， 但允许至61*/

    int min; /*分钟，0-59*/

    int hour; /*小时，0-23*/

    int mday; /*日，即一个月中的第几天，1-31*/

    int mon; /*月， 从一月算起，0-11*/

    int year; /*年， 从1900至今已经多少年*/

    int wday; /*星期，一周中的第几天， 从星期日算起，0-6*/

    int yday; /*从今年1月1日到目前的天数，范围0-365*/

    int isdst; /*日光节约时间的旗标*/
} SYSTIME;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
/*
IV--是否有效 0有效 1无效 占字节最高位
SU--是否夏令时 0标准时间，1夏令时 占字节最高位
day_week:week占字节高3位，day占字节低4位
*/

typedef struct time_data {
    uint16 millisecond;//0-999毫秒
    uint8 minute_iv;//IV--是否有效 0有效 1无效 占字节最高位 第七位备用 低6位为数据（0-59分--在GB_T 18657.4-2002 远动设备及系统 第五部分 传输规约 第四篇 应用信息元素的编码和定义中例图有误写成0-99）
    uint8 hour_su;//SU--是否夏令时 0标准时间，1夏令时 占字节最高位，第六、七位备用，低5位为数据（0-23小时）
    uint8 day_week;//day_week:week占字节高3位(1-7星期的天)，day占字节低5位 （1-31月的天）
    uint8 month;//高四位备用 month占低4位（1-12月）
    uint8 year;//最高位备用 year占低7位（0-99年）
}__attribute__ ((packed))  CP56Time2a;

//CP56Time2a;

#ifdef CONFIG_LINUX
#include <time.h>
#include <sys/time.h>
#else
struct timeval
{
uint32 tv_sec;        //Seconds.
uint32 tv_usec;    // Microseconds.
};
#endif
enum {
    /*Upgrade command*/
    UPGRADE = 0x1001,
    UPDATING,
    SUCCESS,
    FAILED_NO_AUTH,
    FAILED_OLD_VERSION,
    FAILED_TAR,
    /*ALIVE*/
    ALIVE = 0x3001,
    ALIVE_ACK,
    /*QUIT*/
    QUIT = 0x4001,
    NO_FEEDING_DOG,
	RESTART_APP,
};

typedef struct {
    int32 Id;
    int16 Len;
}__attribute__ ((packed)) Item ;

typedef struct _dataHead {
    //uint32 Fun;//功能号
    int16 Num;//数量
}DataHead;

//历史事件类型定义
enum EVENT_TYPE {
    HARD_YX_EVENT,
    SOFT_YX_EVENT,
    POWER_OFF_EVENT,
    ACT_EVENT,
    TERMINAL_FAULT_EVENT,
    TOTAL_EVENT,
};

#endif


