#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>//for file open flag
#include <sys/ioctl.h>//for ioctl
#include "os.h"// for mutex
#include "timer.h"
#include "statis.h"
#include "task.h"
#include "message.h"
#include "gps.h"
#include "hisdata.h"
#include "custom.h"
#include "utils.h"
#include "gpio.h"
#include "device.h"
#include "omapl138_misc.h"
Statis statis;
/** 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2 ( MessageData<BaseDataType>message )
{
    statis.GetMsg ( message );
    return 1;
}
void* statis_main ( void *arg )
{
    statis.Run ( arg );
    return NULL;
}
/////////////////////公有函数/////////////////////////////////////////////////
Statis::Statis()
{
    strcpy ( name, "Statis" );
    Id = TASK_STATIS;
}
Statis::~Statis()
{
    PFUNC ( TEM_WARNING,DEBUG_STATIS, "Destroy Statis\r\n" );
}
int16 Statis::Init(){
	msg.RegistMsg(Id,GetMsg2);///<向message类注册线程号以及消息回调函数	
	if(first_run==0){
	thread_wait_cond(&user_data.cond);//等待参数装载完毕		
	first_run=1;
	}
	//设置版本号
	Data<BaseDataType> para;
	SwVersionPara d2;
	para.Data=&d2;
	memcpy((char *)d2.Data.version,(char *)VERSION,sizeof(d2.Data.version));
	user_data.SetData(PARA_5201,&para);		
    return 1;
}
int16 Statis::Run ( void*arg )
{
    SYSTIME _t; 
    CP56Time2a stat_time;
    uint16 AlarmMin, AlarmDay;
	Data<BaseDataType> para;
	TerminalTime d2;
	para.Data=&d2;
	int tmp2=0;
    Init();
    ReadCfg();
    _t.min = sta_cfg.HisSaveSpan.Data;//遥测历史数据存储间隔
    _t.sec = 0;
    timer.SetAlarm ( &AlarmMin, Min, _t, 0 );
    timer.SetAlarm ( &AlarmDay, Day, _t, 0 );
    //timer.SynRtc ();
	SynDspTime();
    while ( 1 ) {
        delay ( 1 );
		if(tmp2==0){
		   gpio.Write(RUN_PIN,1);
//		   gpio.Write(FEED_DOG_PIN,1);
//		   gpio.Write(ENABLE_DOG_PIN,1);
//		   gpio.Write(BEEP,1);
//		   gpio.Write(GPS_WAKE,1);
//		   gpio.Write(GPS_RESET,1);
//		   gpio.Write(WIRELESS_POWER,1);
//		   gpio.Write(WIRELESS_ON_OFF,1);
//		   gpio.Write(WIRELESS_RST,1);
//		   gpio.Write(WIRELESS_INDICATE,1);
		   tmp2=1;
		   }
		else{
		  gpio.Write(RUN_PIN,0);
//		  gpio.Write(FEED_DOG_PIN,0);
//		  gpio.Write(ENABLE_DOG_PIN,0);
//		  gpio.Write(BEEP,0);
//		  gpio.Write(GPS_WAKE,0);
//		  gpio.Write(GPS_RESET,0);
//		  gpio.Write(WIRELESS_POWER,0);
//		  gpio.Write(WIRELESS_ON_OFF,0);
//		  gpio.Write(WIRELESS_RST,0);
//		  gpio.Write(WIRELESS_INDICATE,0);
		  tmp2=0;
		}        
        /*读取当前时间*/
        timer.GetCP56Time ( &stat_time );
        //PFUNC(TEM_INFO,DEBUG_STATIS,"milisecond:%d min:%d hour:%d day:%d week:%d mon:%d year:%d \n",stat_time.millisecond,stat_time.minute_iv,stat_time.hour_su,stat_time.day_week&0x1f,stat_time.day_week	>>5&7,stat_time.month,stat_time.year);
		memcpy(&d2.Data.time,&stat_time,sizeof(CP56Time2a));
		user_data.SetData(PARA_5215,&para);	        
        if ( timer.IsTimesUp ( AlarmMin ) ) {
			SynDspTime();
            HisDataSta();
            ExtremumDataSta();
            ElecQualSta();
        }
        if ( timer.IsTimesUp ( AlarmDay ) ) {
            ExtremumDataSta();
            ElecQualSta();
        }
        if ( FaultWaveRecordFlag ) {
            FaultWaveRecord();
            FaultWaveRecordFlag = 0;
        }
        Heart();
        CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }
		msg.UnRegistMsg ( Id );
    PFUNC ( TEM_WARNING,DEBUG_STATIS, "Exit Sta Run\r\n" );
    return 1;
}
int16 Statis::WriteCfg ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "write stat config\n" );
    return 0;
}
int16 Statis::ReadCfg ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "read stat config\n" );
    return 0;
}
int16 Statis::GetMsg ( MessageData<BaseDataType>message )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "%s  deal Message\r\n", name );
    PFUNC ( TEM_INFO,DEBUG_STATIS, "stat get msgID=%d,msgDataType=%d,msgOwner=%d,dataid=%x,data=%d\n", message.MessageId, message.Data->DataType, message.Owner, message.Id, ( ( DataType1* ) ( message.Data ) )->Data );
    if ( message.MessageId == MSG_YX ) {//保存YX记录
      SaveSoe ( message, SOFT_YX_EVENT );
    }
    if ( message.MessageId == MSG_YK ) {//处理远方动作指令
      SaveSoe ( message, ACT_EVENT );
    }
    if ( message.MessageId == MSG_PARA ) {
    }
    return 1;
}
/////////////////////私有函数/////////////////////////////////////////////////
/*历史数据统计*/
void Statis::HisDataSta()
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "HisDataSta\n" );
}
/*极值统计*/
void Statis::ExtremumDataSta ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "\n" );
}
/*掉电记录*/
void Statis::LoseElecSta ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "\n" );
}
/*故障录播*/
void Statis::FaultWaveRecord ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "\n" );
}
/*电能质量统计*/
void Statis::ElecQualSta ( void )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "\n" );
}
void Statis::SaveSoe ( MessageData<BaseDataType>message, EVENT_TYPE type )
{
    SOE_MSG soe;
    soe.Id = message.Id;
    soe.Status = ( ( DataType1* ) message.Data )->Data;
    strncpy ( ( char * ) &soe.Time, ( char * ) &message.Time, sizeof ( soe.Time ) );
    PFUNC ( TEM_INFO,DEBUG_STATIS, "save soe:%x %d %d\n", soe.Id, soe.Status, sizeof ( soe ) );
    his_data.SaveHisRecord ( &soe, sizeof ( SOE_MSG ), type );
}
int16 Statis::ReadHisData ( void*p, int32 offset, int16 len, int32 Id )
{
    PFUNC ( TEM_INFO,DEBUG_STATIS, "\n" );
    return his_data.ReadHisRecord ( p, offset, len, Id );
}
int32 Statis::GetHisDataLen ( int32 Id )
{
    FILE_INDEX_TABLE *db;
    db = his_data.FindHisDbById ( Id );
    if(db==NULL)return 0;
    return db->Len;
}
int Statis::SynDspTime(void){
	int fd;
	int ret;
	TerminalTime2 tertime;
	fd=myopen(DEV_OMAPL138_MISC,O_RDWR);
	if(fd<0){
		PFUNC(TEM_ERROR,DEBUG_STATIS,"Can't Open OmapL138Timer2\n");
		goto err1;
	}
    timer.GetCP56Time ( &tertime.Data.time );
	ret=ioctl(fd,GET_TIMER2_COUNTER,(unsigned long)&tertime.Data.counter);
	if(ret<0)
		goto err2;
	msg.SendMsg(TASK_VDSP,MSG_PARA,PARA_5217,&tertime,&tertime.Data.time);
	close(fd);
	return 0;
err2:
	close(fd);
err1:
	return -1;
}
