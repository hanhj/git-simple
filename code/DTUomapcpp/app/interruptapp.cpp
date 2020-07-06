/*
 * interrupapp.cpp
 *
 *  Created on: 2013-8-7
 *      Author: Administrator
 */

#include "product.h"
#include "message.h"
#include "scada.h"
#include "types.h"
#include "datatype.h"
#include "interruptapp.h"
#include "fa_fft_cal.h"
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "fpga.h"
#include "gpio.h"
#include "evmomapl138.h"
#include "clock.h"
#include "fa.h"
#include "evmomapl138_timer.h"
#include <ti/sysbios/timers/timer64/Timer.h>
#define TIME1_12_COUNT TIMER64P0DSP

Timer_Handle HwiTime1Handle;
Timer_Handle HwiTime2Handle;
Hwi_Handle HwiAdFinHandle;
Swi_Handle SwiProtectHandle;
Swi_Handle SwiGetYxHandle;
Swi_Handle SwiGetAdHandle;
Swi_Handle SwiMsHandle;
Swi_Handle SwiFaHandle;

uint64 time1 = 0;
uint64 time2= 0;
uint64 time3= 0;
Uint64 _500usCn = 0;
Uint16 _625usCn = 0;
Uint16 startsoftT = 1;
uint16 yxdata = 0;

#pragma DATA_SECTION("DATAINDDR2");
uint8 YxCurrent[MAX_SG_DB_YXPORT_NUM];// 96 个回线遥信，4个公共遥信，28个备用遥信，16个双点遥信

typedef struct bordyx{
	uint64 ch0:8;
	uint64 ch1:8;
	uint64 ch2:8;
	uint64 ch3:8;
	uint64 ch4:8;
	uint64 ch5:8;
	uint64 ch6:8;
	uint64 ch7:8;
}bordyx;
typedef union bordyxunion{
	uint64 all;
	bordyx bit;
}bordyxu;
#pragma DATA_SECTION("DATAINIRAM");
bordyxu yxtmp1[3];
#pragma DATA_SECTION("DATAINIRAM");
bordyxu  yxtmp2[3];
#pragma DATA_SECTION("YXDATA");
bordyxu YXREG1[3];
uint16 YxCsNum = 0;
Uint16 yxreadflag = 1;
#pragma DATA_SECTION("DATAINDDR2")
YX_BUFF YxBuff;
union SKR_UNION  krtmp = { 0,0,0};
union SKR_UNION  lastkrtmp = {0,0,0};

#pragma CODE_SECTION("CODEINL1PSRAM")
Void HwiMsTimeFxn(UArg arg)
{
	asm ( " DINT" );
	_500usCn++;
	_625usCn++;
	if(_500usCn==4){//500us;
		Swi_post(SwiGetYxHandle);
	}
	if(_500usCn==8){
		_500usCn=0;
		Swi_post(SwiMsHandle);
	}
	if(_625usCn>=5){//625us;
		CONVST1A_0;
		_625usCn = 0;
		CONVST1A_1;
	}

}
#pragma CODE_SECTION("CODEINIRAM")
Void HwiTimer(UArg arg)
{
}
/*
 * 由单点遥信虚拟出双点遥信
 */
#pragma CODE_SECTION("CODEINIRAM")
void SBitMoveToDBit(union SKR_UNION  *Data ,Uint64 DBitMask, Uint64 SBitMask,Uint16 SyxPos)
{
	if(SyxPos<64){

		if(CHKBIT(Data->all.S1,SBitMask)){
			SETBIT(Data->all.D1,DBitMask);
		}else{
			CLRBIT(Data->all.D1,DBitMask);
		}
	}else if (SyxPos<96){
		if(CHKBIT(Data->all.S2,SBitMask)){
			SETBIT(Data->all.D1,DBitMask);
		}else{
			CLRBIT(Data->all.D1,DBitMask);
		}
	}
}

/*
 * 286-227us(max)
 *
 */
/*
 *  遥信1板-遥信4板：
 *  起始地址:0x60000100   结束地址：0x60000107
 *  长度：8个字,奇地址的高字节无效，估计96BIT;
 *  第一个BIT 为端口1的遥信，以此类推 96个遥信端口，96个BIT。
 *  起始地址:0x60000108   结束地址：0x60000108
*   bit0:公共遥信1；bit1:公共遥信2；
*   bit2:公共遥信3；bit3:公共遥信4；
*   bit4:远方/就地；bit5:电池欠压；
*   bit6:终端直流供电；bit7:电源异常；
*   bit8:活化状态；bit9:主交流供电状态；
*   bit10:备交流供电状态；
 *
 */
#pragma CODE_SECTION("CODEINL1PSRAM")
Void SwiGetYxFxn(UArg arg1, UArg arg2)
{
	Sys500us++;
	if(YxCsNum>=9){
		return ;
	}
	if(yxreadflag==1){
		yxtmp1[0].all = YXREG1[0].all;
		yxtmp1[1].all = YXREG1[1].all;
		yxtmp1[2].all = YXREG1[2].all;
	}
	//由于开入开出板是挂在总线上的，如果开出开出板没有安装，此时读取遥信状态的时候是不确定的，造成假的遥信变位；
	////////////////开入开出板1
	if(INOUT_SLOT1_STATU==IN_STATU){
		yxtmp2[0].bit.ch0 = yxtmp1[0].bit.ch0;
		yxtmp2[0].bit.ch1 = yxtmp1[0].bit.ch1;
		yxtmp2[0].bit.ch2 = yxtmp1[0].bit.ch2;//低字节保留，高字节舍弃；
	}else{
		yxtmp2[0].bit.ch0=0xff;
		yxtmp2[0].bit.ch1=0xff;
		yxtmp2[0].bit.ch2=0xff;
	}

///////////////开入开出板2
	if(INOUT_SLOT2_STATU==IN_STATU){
		yxtmp2[0].bit.ch3 = yxtmp1[0].bit.ch4;
		yxtmp2[0].bit.ch4 = yxtmp1[0].bit.ch5;
		yxtmp2[0].bit.ch5 = yxtmp1[0].bit.ch6;
	}else{
		yxtmp2[0].bit.ch3=0xff;
		yxtmp2[0].bit.ch4=0xff;
		yxtmp2[0].bit.ch5=0xff;
	}
///////////////开入开出板3
	if(INOUT_SLOT3_STATU==IN_STATU){
		yxtmp2[0].bit.ch6 = yxtmp1[1].bit.ch0;
		yxtmp2[0].bit.ch7 = yxtmp1[1].bit.ch1;
		yxtmp2[1].bit.ch0 = yxtmp1[1].bit.ch2;
	}else{
		yxtmp2[0].bit.ch6=0xff;
		yxtmp2[0].bit.ch7=0xff;
		yxtmp2[1].bit.ch0=0xff;
	}
///////////////开入开出板4
	if(INOUT_SLOT4_STATU==IN_STATU){
		yxtmp2[1].bit.ch1 = yxtmp1[1].bit.ch4;
		yxtmp2[1].bit.ch2 = yxtmp1[1].bit.ch5;
		yxtmp2[1].bit.ch3 = yxtmp1[1].bit.ch6;
	}else{
		yxtmp2[1].bit.ch1=0xff;
		yxtmp2[1].bit.ch2=0xff;
		yxtmp2[1].bit.ch3=0xff;
	}
	///////////////电源板
	//9
	yxtmp2[1].bit.ch4 = yxtmp1[2].bit.ch0;
	yxtmp2[1].bit.ch5 = yxtmp1[2].bit.ch1&0x01;

	//yxtmp2[1].bit.ch6 = yxtmp1[2].bit.ch2;
	//yxtmp2[1].bit.ch7 = yxtmp1[2].bit.ch4;
	krtmp.all.S1 =yxtmp2[0].all;
	krtmp.all.S2 =yxtmp2[1].all;
	krtmp.all.D1 = 0;
	if((lastkrtmp.all.S1!=krtmp.all.S1)||(lastkrtmp.all.S2!=krtmp.all.S2)){
		if(YxBuff.CurPrepare>=MAX_SOE_NUM){
			YxBuff.CurPrepare = 0;
		}
		Uint16 i;
		Uint16 SyxPos;
		for ( i = 0; i < MAX_LINE_NUM; i++){
				SyxPos = scada.yx_config.config_data[DBYX(i)].ClosePort;
			if(SyxPos<MAX_YX_PORT_NUM){
				SBitMoveToDBit(&krtmp,HeYxMask[i],MASKTAB[SyxPos],SyxPos);
			}
				SyxPos = scada.yx_config.config_data[DBYX(i)].OpenPort;
			if(SyxPos<MAX_YX_PORT_NUM){
				SBitMoveToDBit(&krtmp,FenYxMask[i],MASKTAB[SyxPos],SyxPos);
			}
		}
		YxBuff.databuff[YxBuff.CurPrepare].TimeCnt = Sys500us;
		YxBuff.databuff[YxBuff.CurPrepare].time = clk0.GetCP56Time2a(GETSOFTTIME);
		YxBuff.databuff[YxBuff.CurPrepare].ChangeFlag.all.S1 =(lastkrtmp.all.S1^krtmp.all.S1);//变化的的位置 置1；
		YxBuff.databuff[YxBuff.CurPrepare].ChangeFlag.all.S2 =(lastkrtmp.all.S2^krtmp.all.S2);//变化的的位置 置1；
		YxBuff.databuff[YxBuff.CurPrepare].ChangeFlag.all.D1 =(lastkrtmp.all.D1^krtmp.all.D1);//变化的的位置 置1；

		YxBuff.databuff[YxBuff.CurPrepare].Yx.all.S1 =krtmp.all.S1;//实时的遥信状态
		YxBuff.databuff[YxBuff.CurPrepare].Yx.all.S2 =krtmp.all.S2;//实时的遥信状态
		YxBuff.databuff[YxBuff.CurPrepare].Yx.all.D1 =krtmp.all.D1;//实时的遥信状态
		YxBuff.databuff[YxBuff.CurPrepare].OldYx.all.S1 =lastkrtmp.all.S1 ;//之前的遥信状态
		YxBuff.databuff[YxBuff.CurPrepare].OldYx.all.S2 =lastkrtmp.all.S2 ;//之前的遥信状态
		YxBuff.databuff[YxBuff.CurPrepare].OldYx.all.D1 =lastkrtmp.all.D1 ;//之前的遥信状态
		YxBuff.CurPrepare++;
		lastkrtmp.all = krtmp.all;
		if(YxBuff.CurPrepare>=MAX_SOE_NUM){
			YxBuff.CurPrepare = 0;
		}
	}
}
void GetFirstYxData(void)
{
	Uint16 i;
	Uint16 SyxPos;
	Sys500us++;
	yxtmp1[0].all = YXREG1[0].all;
	yxtmp1[1].all = YXREG1[1].all;
	yxtmp1[2].all = YXREG1[2].all;

//由于开入开出板是挂在总线上的，如果开出开出板没有安装，此时读取遥信状态的时候是不确定的，造成假的遥信变位；
////////////////开入开出板1

	if(INOUT_SLOT1_STATU==IN_STATU){
		yxtmp2[0].bit.ch0 = yxtmp1[0].bit.ch0;
		yxtmp2[0].bit.ch1 = yxtmp1[0].bit.ch1;
		yxtmp2[0].bit.ch2 = yxtmp1[0].bit.ch2;//低字节保留，高字节舍弃；
	}else{
		yxtmp2[0].bit.ch0=0xff;
		yxtmp2[0].bit.ch1=0xff;
		yxtmp2[0].bit.ch2=0xff;
	}

///////////////开入开出板2
	if(INOUT_SLOT2_STATU==IN_STATU){
		yxtmp2[0].bit.ch3 = yxtmp1[0].bit.ch4;
		yxtmp2[0].bit.ch4 = yxtmp1[0].bit.ch5;
		yxtmp2[0].bit.ch5 = yxtmp1[0].bit.ch6;
	}else{
		yxtmp2[0].bit.ch3=0xff;
		yxtmp2[0].bit.ch4=0xff;
		yxtmp2[0].bit.ch5=0xff;
	}
///////////////开入开出板3
	if(INOUT_SLOT3_STATU==IN_STATU){
		yxtmp2[0].bit.ch6 = yxtmp1[1].bit.ch0;
		yxtmp2[0].bit.ch7 = yxtmp1[1].bit.ch1;
		yxtmp2[1].bit.ch0 = yxtmp1[1].bit.ch2;
	}else{
		yxtmp2[0].bit.ch6=0xff;
		yxtmp2[0].bit.ch7=0xff;
		yxtmp2[1].bit.ch0=0xff;
	}
///////////////开入开出板4
	if(INOUT_SLOT4_STATU==IN_STATU){
		yxtmp2[1].bit.ch1 = yxtmp1[1].bit.ch4;
		yxtmp2[1].bit.ch2 = yxtmp1[1].bit.ch5;
		yxtmp2[1].bit.ch3 = yxtmp1[1].bit.ch6;
	}else{
		yxtmp2[1].bit.ch1=0xff;
		yxtmp2[1].bit.ch2=0xff;
		yxtmp2[1].bit.ch3=0xff;
	}
///////////////电源板
	//9
	yxtmp2[1].bit.ch4 = yxtmp1[2].bit.ch0;
	yxtmp2[1].bit.ch5 = yxtmp1[2].bit.ch1&0x01;

	krtmp.all.S1 =yxtmp2[0].all;
	krtmp.all.S2 =yxtmp2[1].all;
	krtmp.all.D1 = 0;
	for ( i = 0; i < MAX_LINE_NUM; i++){
		SyxPos = scada.yx_config.config_data[DBYX(i)].ClosePort;
		if(SyxPos<MAX_YX_PORT_NUM){
			SBitMoveToDBit(&krtmp,HeYxMask[i],MASKTAB[SyxPos],SyxPos);
		}
		SyxPos = scada.yx_config.config_data[DBYX(i)].OpenPort;
		if(SyxPos<MAX_YX_PORT_NUM){
			SBitMoveToDBit(&krtmp,FenYxMask[i],MASKTAB[SyxPos],SyxPos);
		}
	}
	lastkrtmp.all = krtmp.all;
	for( i = 0 ; i < MAX_SG_DB_YXPORT_NUM;i++){
		YxCurrent[i] = scada.YxBitFun((union KR_UNION*)&krtmp,MASKTAB[i],i,GET_BIT);//(krtmp.all.S1>>i)&0x0001;
	}
}
#pragma CODE_SECTION("CODEINL1PSRAM")
Void SwiMsFxn(UArg arg1, UArg arg2)
{
	clk0.milli_tick();
	scada.SoftMsTime();
}

#pragma CODE_SECTION("CODEINL1PSRAM")
void interrupt_init(void)
{
	Timer_Params HwiTime1Para;
	Timer_Params HwiTime2Para;
	Swi_Params SwiGetYxPara;
	Swi_Params SwiGetAdPara;
	Hwi_Params HwiAdFinPara;
	Swi_Params SwiMsPara;
	Swi_Params SwiFaPara;

	Timer_Params_init(&HwiTime2Para);
	Timer_Params_init(&HwiTime1Para);
	Hwi_Params_init(&HwiAdFinPara);

	SwiGetYxPara.priority = 1;
	SwiGetYxHandle= Swi_create(SwiGetYxFxn, &SwiGetYxPara, NULL);

	SwiGetAdPara.priority = 2;
	SwiGetAdHandle= Swi_create(SwiGetAdFxn, &SwiGetAdPara, NULL);

	SwiMsPara.priority = 3;
	SwiMsHandle= Swi_create(SwiMsFxn, &SwiMsPara, NULL);

	SwiFaPara.priority = 4;
	SwiFaHandle= Swi_create(SwiFaFxn, &SwiFaPara, NULL);

	HwiTime2Para.period = 1000000;
	HwiTime2Para.periodType = Timer_PeriodType_MICROSECS;
	//HwiTime2Para.half =  Timer_Half_UPPER;
	HwiTime2Para.startMode =Timer_StartMode_USER;
	HwiTime2Para.intNum = 15;
	HwiTime2Handle = Timer_create(2, HwiTimer, &HwiTime2Para, NULL);
	//SETBIT(TMR2->TCR,0x08000000);
	//TMR2->PRD12 =0xffffffff;
	//TMR2->PRD34 =150000000;//1秒

///定时中断服务
	HwiTime1Para.period =125;//0.125ms     //950;
	HwiTime1Para.periodType = Timer_PeriodType_MICROSECS;
	HwiTime1Para.half =  Timer_Half_LOWER;
	HwiTime1Para.startMode =Timer_StartMode_USER;
	HwiTime1Para.intNum = 4;
	HwiTime1Handle = Timer_create(1, HwiMsTimeFxn, &HwiTime1Para, NULL);

	HwiAdFinPara.enableInt = FALSE;
	HwiAdFinPara.eventId = 72;//GPIO Bank 7 Interrupt
	HwiAdFinPara.maskSetting = Hwi_MaskingOption_SELF;
	HwiAdFinHandle=Hwi_create(6, HwiAdFinFxn, &HwiAdFinPara, NULL);//如果用5的话语ARM测有冲突
	memset(&AD_BUF_scada_cal,0,sizeof(AD_BUF_scada_cal));
	tw_gen (ptr_w32, SAMP_NUM);
	AdReset();
#if  ENABLE_FA
	memset(&AD_BUF_fa_cal,0,sizeof(AD_BUF_fa_cal));
	tw_gen (ptr_w16, FA_SAMP_NUM);
#endif

	Timer_start(HwiTime2Handle);
	SETBIT(TMR2->TCR,0x00800000);//SET ENAMODE34 = 2h;// 组合成一个64位的定时器；
}

void StartIr(void){
	Timer_start(HwiTime1Handle);
	Hwi_enableInterrupt(6);
	GPIO_BANK67_BIT->SET_FAL_TRIG.bit.Hbit8 = 1;
}
