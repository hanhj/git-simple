#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "os.h"// for mutex
#include "timer.h"
#include "debug.h"
#include "bat.h"
#include "task.h"
#include "message.h"
#include "custom.h"
#include "gpio.h"//for gpio
#include "localtime.h"//for sec min_timer
#include "utils.h"//for save read_file
#include "ad.h"//for ad
#define BATTRUNDATA RTDATA_ROOT"battrun.dat"
Batt batt;
/** /brief 需要定义一个全局函数Task类处理该类的入口
*/
void * batt_main ( void *arg )
{
    batt.Run ( ( TASK* ) arg );
    return NULL;
}
/** /brief 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2 ( MessageData<BaseDataType>message )
{
    batt.GetMsg ( message );
    return 1;
}
Batt::Batt ( void )   /**< 初始化函数  */
{
    strcpy ( name, "Batt" );
    Id = TASK_BATT;
}
Batt::~Batt ( void )
{
    msg.Msg.Flush();
    PFUNC ( TEM_WARNING,DEBUG_BATT, "Destroy Batt\r\n" );
}
int16 Batt::Init()
{
    memset ( ( char* ) &bat_data, 0, sizeof ( bat_data ) );
		msg.RegistMsg(Id,GetMsg2);///<向message类注册线程号以及消息回调函数	 
		if(first_run==0){
		thread_wait_cond(&user_data.cond);//等待参数装载完毕		
		first_run=1;
		}
		Data<BaseDataType> para;
		user_data.GetData(PARA_5207,&para);	
		bat_para.CellLessLevel=(DataType4*)para.Data;
		user_data.GetData(PARA_5208,&para);	
		bat_para.CellLowLevel=(DataType4*)para.Data;
		user_data.GetData(PARA_5209,&para);	
		bat_para.CellActTime=(DataType2*)para.Data;
		ReadCfg();
		user_data.GetData(YC_4121,&para);	
		bat_data.CellVol=(DataType4*)para.Data;
		bat_data.LosePower=0;//gpio.Read(BAT_PIN_ALARM_LOSEPOWER)	;//电池GPIO 默认为0
		bat_data.PowerError=1;//gpio.Read(BAT_PIN_ALARM_POWER_ERROR);
		bat_data.AlarmLowerU2=1;//gpio.Read(BAT_PIN_ALARM_LOWU)	;
		bat_data.AlarmLowerU1=0;
		bat_data.CutBatt=0;
		bat_data.BattActState=0;
		//初始化电池控制脚
		gpio.Init(BAT_PIN_CUT_BATT,GPIO_DIR_OUT,1);
		gpio.Init(BAT_PIN_STOP_ACT,GPIO_DIR_OUT,1);
		gpio.Init(BAT_PIN_START_ACT,GPIO_DIR_OUT,1);
		//gpio.Init(RUN_PIN,GPIO_DIR_OUT,1);
		gpio.Init(PIN_STATE_JD,GPIO_DIR_IN,0);
		//gpio.Init(BAT_PIN_ALARM_LOSEPOWER,GPIO_DIR_IN,0);
		gpio.Init(BAT_PIN_ALARM_POWER_ERROR,GPIO_DIR_IN,0);
		gpio.Init(BAT_PIN_STATE_ACT,GPIO_DIR_IN,0);
		gpio.Init(BAT_PIN_ALARM_LOWU,GPIO_DIR_IN,0);		
		gpio.Write(BAT_PIN_CUT_BATT,1);
		gpio.Write(BAT_PIN_STOP_ACT,1);
		gpio.Write(BAT_PIN_START_ACT,1);
	return 1;
}
int16 Batt::Run ( void*arg )
{
    CP56Time2a time;
    DataType1 d1;
    DataType4 d2;
	Data<BaseDataType> yc;
    int16 bat_run_time=0;
    int16  delay_cut=0;
    static time_t freeze_second = 0;
    int8 yf_state=0;//远方就地状态
	int tmp2=0;
	int tmp=0;
    Init();
	mdelay(1000);//等待一下,防止规约线程未初始化好
    while ( 1 ) {
        mdelay(100);
		if (sec_timer ( 1, &freeze_second ) == 1)  //检查电池活化时间间隔
		{
			if(tmp2==0){
		    	//gpio.Write(RUN_PIN,1);
		    	tmp2=1;
		    	}
		    else{
		    	//gpio.Write(RUN_PIN,0);
		    	tmp2=0;
		    }
			bat_run_time++;
			if(bat_run_time>60)
			{
				bat_data.Run++;			
				if(bat_data.Run>bat_para.CellActTime->Data&&bat_para.CellActTime->Data!=0)
				{
       			if(!bat_data.LosePower){//电池在交流供电情况下才能活化
				bat_data.Run=0;
				gpio.Write(BAT_PIN_START_ACT,0);//启动活化
				sleep(1);
				gpio.Write(BAT_PIN_START_ACT,1);
				/*
				电源手册上说:
				电池的活化，当电池长时间处于浮充电状态，应对电池进行活化以免电池极板钝化，
				活化可以由用户CPU控制的继电器把电源的活化端子HK与VG短接一次（不小于0.5秒，
				但端子不应长时间短接，否则电源将失去自动功能）电源进入活化状态，电源关闭输出，电池放电，
				当电池放电至活化完成点时，电源自动启动工作向负载供电并为电池充电；当中途需要提前退出活化时，
				可手动按电源面板上的活化OFF键退出活化，或由用户CPU控制的继电器触点把电源的HG与VG端子短接一次
				（不小于0.5秒）则可提前退出活化。				
				*/
				}
				WriteCfg();
				}
				bat_run_time=0;
			}
			if(gpio.Read(PIN_STATE_JD)!=yf_state)//远方就地状态变化
			{
				timer.GetCP56Time(&time);
				yf_state=gpio.Read(PIN_STATE_JD);
				d1.Data=yf_state;
				yc.Data=&d1;
				user_data.SetData(YX_0035,&yc);
				msg.SendMsg(TASK_STATIS|TASK_PRO|TASK_VDSP,MSG_YX,YX_0035,&d1,&time);        		
				PFUNC(TEM_INFO,DEBUG_BATT,"YF:%d\n ",yf_state);
			}        
			tmp=gpio.Read(BAT_PIN_STATE_ACT);
			if(tmp!=bat_data.BattActState)//活化状态
			{
				bat_data.BattActState=tmp;
				timer.GetCP56Time(&time);
				d1.Data=bat_data.BattActState;
				yc.Data=&d1;
				user_data.SetData(YX_0179,&yc);
				msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0179,&d1,&time);  
				PFUNC(TEM_INFO,DEBUG_BATT,"BattActState:%d\n",bat_data.BattActState);      		
			}
			//tmp=gpio.Read(BAT_PIN_ALARM_LOSEPOWER);
			if(tmp!=bat_data.LosePower)//交流失电得电告警及恢复
			{
				bat_data.LosePower=tmp;
				timer.GetCP56Time(&time);
				d1.Data=bat_data.LosePower;
				yc.Data=&d1;
				user_data.SetData(YX_0177,&yc);
				msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0177,&d1,&time);  
				PFUNC(TEM_INFO,DEBUG_BATT,"bat_data.LosePower:%d\n",bat_data.LosePower);      		
			}
			tmp=gpio.Read(BAT_PIN_ALARM_POWER_ERROR);
			if(tmp!=bat_data.PowerError)//电源故障告警及恢复
			{
				bat_data.PowerError=tmp;
				timer.GetCP56Time(&time);
				d1.Data=!bat_data.PowerError;
				yc.Data=&d1;
				user_data.SetData(YX_0178,&yc);
				msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0178,&d1,&time);        		
				PFUNC(TEM_INFO,DEBUG_BATT,"bat_data.PowerError:%d\n",bat_data.PowerError);      		
			}	
			tmp=gpio.Read(BAT_PIN_ALARM_LOWU);	
			if(tmp!=bat_data.AlarmLowerU2)//电源欠压告警及恢复
			{
				bat_data.AlarmLowerU2=tmp;
				timer.GetCP56Time(&time);
				d1.Data=!bat_data.AlarmLowerU2;
				yc.Data=&d1;
				user_data.SetData(YX_0176,&yc);
				msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0176,&d1,&time);        		
				PFUNC(TEM_INFO,DEBUG_BATT,"bat_data.AlarmLowerU2:%d\n",bat_data.AlarmLowerU2);      		
			}
//d2.Data=50;
//msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YC,YC_401B,&d2,&time);
			//获得并设置第一路直流量-电池电压
			float tmp3;
			yc.Data=&d2;
			tmp3=ad.Read(AD_CHANEL1);
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 0 origin vol: %f\n ",tmp3);
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 0 slope: %f base :%f\n ",ad.ad_coefficent[AD_CHANEL1].slope,ad.ad_coefficent[AD_CHANEL1].base);
			if(ad.ad_coefficent[AD_CHANEL1].type==AD_TYPE_U)
				tmp3=(tmp3-ad.ad_coefficent[AD_CHANEL1].base)*ad.ad_coefficent[AD_CHANEL1].slope+MIN_U_VALUE;
			else
				tmp3=(tmp3-ad.ad_coefficent[AD_CHANEL1].base)*ad.ad_coefficent[AD_CHANEL1].slope+MIN_I_VALUE;		
			d2.Data=tmp3;
			user_data.SetData(YC_4125,&yc);
			user_data.SetData(YC_4121,&yc);
			bat_data.CellVol->Data=tmp3;
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 0: %f\n ",tmp3);
			//获得第二路直流量
			tmp3=ad.Read(AD_CHANEL2);
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 origin vol: %f\n ",tmp3);
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 slope: %f base:%f\n ",ad.ad_coefficent[AD_CHANEL2].slope,ad.ad_coefficent[AD_CHANEL2].base);
			if(ad.ad_coefficent[AD_CHANEL2].type==AD_TYPE_U){
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",tmp3-ad.ad_coefficent[AD_CHANEL2].base);	
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope);	
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope+MIN_U_VALUE);	
			tmp3=(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope+MIN_U_VALUE;
			}
			else{
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",tmp3-ad.ad_coefficent[AD_CHANEL2].base);	
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope);	
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1 %f",(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope+MIN_U_VALUE);	
			tmp3=(tmp3-ad.ad_coefficent[AD_CHANEL2].base)*ad.ad_coefficent[AD_CHANEL2].slope+MIN_I_VALUE;	
			}
			d2.Data=tmp3;
			user_data.SetData(YC_4126,&yc);
//			PFUNC(TEM_INFO,DEBUG_BATT,"channel 1: %f\n ",tmp3);
			CalibrateAd(cal_cmd);
	        if(bat_data.CellVol->Data<bat_para.CellLowLevel->Data)//电池低压告警
	        {
		       	if(	bat_data.AlarmLowerU1==0)
		    	{
		    		bat_data.AlarmLowerU1=1;	
					timer.GetCP56Time(&time);
					d1.Data=1;
					yc.Data=&d1;
					user_data.SetData(YX_0176,&yc);
					msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0176,&d1,&time);        		
				}
	        }
	        if(bat_data.CellVol->Data>bat_para.CellLowLevel->Data)//电池低压告警恢复
	        {
	        	if(	bat_data.AlarmLowerU1==1)
	        	{
	        		bat_data.AlarmLowerU1=0;	
					timer.GetCP56Time(&time);
					d1.Data=0;
					yc.Data=&d1;
					user_data.SetData(YX_0176,&yc);
					msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,YX_0176,&d1,&time);        		
				}
	        }
	        if(bat_data.CellVol->Data<bat_para.CellLessLevel->Data)//电池欠压切除
	        {//按照电源手册上讲电池切除时间必须保持5秒以上
		       	if(	bat_data.CutBatt==0)
	        		{
	        		bat_data.CutBatt=1;	
					gpio.Write(BAT_PIN_CUT_BATT,0);
					delay_cut=0;
					}
				else if(bat_data.CutBatt==1)	
					{
					delay_cut++;
					if(delay_cut>10){
						gpio.Write(BAT_PIN_CUT_BATT,1);
						}
					}
	        }
		}	
        Heart ();
        CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }
	msg.UnRegistMsg ( Id );
    PFUNC ( TEM_WARNING,DEBUG_BATT, "Exit Batt Run\r\n" );
    return 1;
}
int16 Batt::GetMsg ( MessageData<BaseDataType>message )
{
	CP56Time2a time;
    PFUNC ( TEM_INFO,DEBUG_BATT, "%s deal  Message\r\n", name );
    if ( message.MessageId == MSG_YX ) {
        PFUNC ( TEM_INFO,DEBUG_BATT, "%s rcv YX Message\r\n", name );
    }
    if ( message.MessageId == MSG_YC ) {
        PFUNC ( TEM_INFO,DEBUG_BATT, "%s rcv YC Message\r\n", name );
    }
    if ( message.MessageId == MSG_YK ) {
        PFUNC ( TEM_INFO,DEBUG_BATT, "%s rcv YK Message\r\n", name );
        DataType1 *data;
        uint8 cmd;
        data=(DataType1 *)message.Data;        
        cmd=data->Data;
        PFUNC ( TEM_INFO,DEBUG_BATT, "msgId:%x,data:%x\r\n", message.Id,cmd);
        if(message.Id==YK_6064)//电池活化
        	{
        		if(!is_set(cmd,7)&&is_set(cmd,0))//判断是否是选择命令
        		{
        			if(bat_data.LosePower)//如果处于电池供电状态,不活化
        				return 0;
        			gpio.Write(BAT_PIN_START_ACT,0);//电池活化启动
        			sleep(1);
        			gpio.Write(BAT_PIN_START_ACT,1);
        		}
        		else if(!is_set(cmd,7)&&!is_set(cmd,0))
        		{
        			gpio.Write(BAT_PIN_STOP_ACT,0);//电池活化退出
        			sleep(1);
        			gpio.Write(BAT_PIN_STOP_ACT,1);
        		}
				timer.GetCP56Time(&time);
				msg.SendMsg(TASK_PRO,MSG_YK,YK_6064,data,&time);
        	}
        if(message.Id==YK_6064)//电池退出
        	{
        		if(!is_set(cmd,7)&&!is_set(cmd,0))//判断是否是选择命令
        		{
        			gpio.Write(BAT_PIN_CUT_BATT,0);//电池退出
        		}
        		else if(!is_set(cmd,7)&&is_set(cmd,0))
        		{
        			gpio.Write(BAT_PIN_CUT_BATT,1);//电池投入
        		}
				timer.GetCP56Time(&time);
				msg.SendMsg(TASK_PRO,MSG_YK,YK_6064,data,&time);
        	}
    }
    if ( message.MessageId == MSG_PARA ) {
        PFUNC ( TEM_INFO,DEBUG_BATT, "%s rcv PARA Message\r\n", name );
        if(message.Id==PARA_5097)//标定
        	{
        		int8 cal_ac_dc;
        		DataType2 *data;
        		data=(DataType2 *)message.Data;
        		cal_cmd=(data->Data);
				PFUNC ( TEM_INFO,DEBUG_BATT, "rcvdata:%x\r\n", cal_cmd );
        		cal_ac_dc=(cal_cmd)>>12&0x000f;//交直流标定标志
						if(cal_ac_dc==CAL_DC){
								start_cal=1;
						}
        	}
        if(message.Id==PARA_5206)//恢复出厂值
        	{
        		copy(DEFAULT_PARA_CFG,PARA_CFG);
        		send_cmd ( process_fp[1], RESTART_APP );
        	}
    }
    return 1;
}
void Batt::SendMsg(char *buf)
{
		DataType1 d1;
		d1.Data=10;
		CP56Time2a time;
		timer.GetCP56Time(&time);
		msg.SendMsg(TASK_STATIS|TASK_PRO,MSG_YX,4,&d1,&time);
		PFUNC(TEM_WARNING,DEBUG_BATT,"batt send data=%d dataid=%d\n",d1.Data,4);
		ProtectSet *d2;
		d2=(ProtectSet *)user_data.GetDataPtr(0x5001)->Data;
		msg.SendMsg(TASK_VDSP,MSG_PARA,0x5001,d2,&time);
}
void Batt::GetMsg ( )   /**< 调用msg.h中的接口接收消息  */
{
    PFUNC ( TEM_INFO,DEBUG_BATT, "%s\n", name );
    return;
}
int16 Batt::WriteCfg ()
{
		save_file(BATTRUNDATA,&bat_data,sizeof(bat_data),"wb",0);
    return 1;
}
int16 Batt::ReadCfg (  )
{
	int16 ret;
		read_file(BATTRUNDATA,&bat_data,sizeof(bat_data),"rb",0);
		ret=read_file(DC_CALIBRATE_FILE,&ad.ad_coefficent,sizeof(ad.ad_coefficent),"rb",0);
		if(ret<0)
		{
			copy(DEFAULT_DC_CALIBRATE_FILE,DC_CALIBRATE_FILE);
			read_file(DC_CALIBRATE_FILE,&ad.ad_coefficent,sizeof(ad.ad_coefficent),"rb",0);	
		}
    return 1;
}
#define SUC 14
#define FAIL 15
#define CAL_COUNT 10
#define UNUSE_COUNT 5
int8 Batt::CalibrateAd(uint16 cmd)
{
        		int8 i,j;
        		static int8 count,count2;
        		static int16 tmp[MAX_AD_CHANNEL][CAL_COUNT];
        		static int32 tmp2;
				int8 cal_type,channel_flag,step;  
				DataType2 d2;
				CP56Time2a time;
				if(start_cal==0)return 0;
	        	cal_type=(cmd)>>8&0x000f;//直流标定类型标志：电压还是电流
        		channel_flag=(cmd)>>4&0x000f;//标定通道标志，每一位代表一个通道						
        		step=(cmd)&0x000f;
        				if(step==1)
        					{
				        		for(i=0;i<MAX_AD_CHANNEL;i++)
				        		{
				        			if(channel_flag&(1<<i))
			        				{
					        			count++;
					        			if(count>=UNUSE_COUNT)
					        			{
					        				tmp[i][count2]=	ad.Read(i);
					        				PFUNC ( TEM_INFO,DEBUG_BATT, "in1:channel:%d %d\r\n",i,tmp[i][count2]);
					        				count2++;
					        			}
										if(count2>=CAL_COUNT)
					        			{
					        				for(j=0;j<CAL_COUNT;j++)
					        				tmp2+=tmp[i][j];
	        								ad.ad_coefficent[i].base=tmp2/CAL_COUNT;
	        								start_cal=0;
	        								tmp2=0;
	        								count=0;
	        								count2=0;
											timer.GetCP56Time(&time);
											cmd&=0xfff0;
											cmd|=SUC;
											d2.Data=cmd;
											msg.SendMsg(TASK_PRO,MSG_PARA,PARA_5097,&d2,&time);
	        							}
	        							ad.ad_coefficent[i].type=cal_type;
	        						}
        						}
        					}
        				if(step==2)
        					{
				        		for(i=0;i<MAX_AD_CHANNEL;i++)
				        		{ 
				        			if(channel_flag&(1<<i))
			        				{
					        			count++;
					        			if(count>=UNUSE_COUNT)
					        			{
					        				tmp[i][count2]=	ad.Read(i);
					        				PFUNC ( TEM_INFO,DEBUG_BATT, "in2:channel:%d %d \r\n",i,tmp[i][count2]);
					        				count2++;
					        			}
										if(count2>=CAL_COUNT)
					        			{
					        				for(j=0;j<CAL_COUNT;j++)
					        				tmp2+=tmp[i][j];
	        								tmp2=tmp2/CAL_COUNT;
	        								start_cal=0;
	        								count=0;
	        								count2=0;
			        						if(ad.ad_coefficent[i].type==AD_TYPE_U)
			        						{
			        							ad.ad_coefficent[i].slope=(MAX_U_VALUE-MIN_U_VALUE)*1.0/(tmp2-ad.ad_coefficent[i].base);
												PFUNC ( TEM_ERROR,DEBUG_BATT, "channel:%d slope:%f %d %d \r\n",i, ad.ad_coefficent[i].slope,tmp2,ad.ad_coefficent[i].base);
			        						}
			        						else
			        						{
			        							ad.ad_coefficent[i].slope=(MAX_I_VALUE-MIN_I_VALUE)*1.0/(tmp2-ad.ad_coefficent[i].base);	
			        							PFUNC ( TEM_ERROR,DEBUG_BATT, "channel:%d slope:%f %d %d \r\n", i,ad.ad_coefficent[i].slope,tmp2,ad.ad_coefficent[i].base);
			        						}
											timer.GetCP56Time(&time);
											cmd&=0xfff0;
											cmd|=SUC;
											d2.Data=cmd;
											msg.SendMsg(TASK_PRO,MSG_PARA,PARA_5097,&d2,&time);
		        						}
	        						}
        						}
        					}
        				if(step==3)	{
        					save_file(DC_CALIBRATE_FILE,&ad.ad_coefficent,sizeof(ad.ad_coefficent),"wb",0);
        					start_cal=0;
									timer.GetCP56Time(&time);
									cmd&=0xfff0;
									cmd|=SUC;
									d2.Data=cmd;
									msg.SendMsg(TASK_PRO,MSG_PARA,PARA_5097,&d2,&time);
									PFUNC ( TEM_INFO,DEBUG_BATT, "in3 %x\r\n" ,d2.Data);        					
        				}
return 1;
}	
