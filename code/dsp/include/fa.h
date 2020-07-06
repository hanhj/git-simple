#ifndef FA_H
#define FA_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到BaseApp类.
#include "interruptapp.h"
#include "clock.h"
#include "dspdatatype.h"
#include "fft.h"
#include <stdio.h>
#include <stdlib.h>
#include "fpga.h"
#include "scada.h"
#define ONESHOT 0
#define NODELAY 0

#define CHECKHE 0
#define CHECKFEN 1

#define STARTCHZ 1
#define CHECKCHZ 0

#define LOCK 1
#define UNLOCK 0
typedef enum PROTECTSTATUE{
	PROTECTSTOP = 0,
	PROTECTIN,
	PROTECTOUT
}PROTECTSTATU_EM;
typedef enum BRKOUTSTATU{
	OUTSTOP = 0,
	OUTIN,
	OUTCK,
	OUTFAIL,
	OUTCKOK,
	OUTCKFAIL
}BRKOUTSTATU_E;
typedef enum CHZSTATU{
	CHZSTOP = 0,
	CHZIN,
	CHZOUT,
	CHZOUTFAIL,
	CHZCK,
	CHZCKOK,
	CHZCKFAIL,
	HJSTIME
}CHZSTATU_E;
typedef struct CURRENTPRO
{
	ProtectSet Id1;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
	ProtectSet Id2;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
	ProtectSet Id3;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
	ProtectSet I0d1;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
	ProtectSet I0d2;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
	ProtectSet I0d3;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
	ProtectSet U0d;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
	ProtectDirSet Pdir;///< 电流方向保护投入定值
	ProtectDirSet P0dir;///< 零序电流方向保护投入定值
}CurrentProSets_t;
typedef struct _FaPara
{
	CurrentProSets_t CurrentProSets[2];
	DataType1  d_5049;///<线路1保护定值区号设置
	DataType1  d_504A;///<线路2保护定值区号设置
	RecloseSet d_504B;///<重合闸设置
	FaDevotionSet d_5055;///<保护功能投退设置
	//////////自定义/////////////////
	BASEPROSTATU_T OneShotStatu;

}FaPara;



/*当输入为“0”时，输出为“0”；当输入由“0”变“1”时，
 *经过时间T1延时后，输出为“1”，此前输出一直为“0
 */
class DelayStartTime{
public:
	bool TimeCell(bool InBool,int32 TdMs,uint64 SysMs,BASEPROSTATU_T *statu)
	{
		if(statu==NULL){
			while(1);
		}
		if (InBool){
			if(SysMs >= statu->CurrentMs){
				statu->CurrentBool = InBool;
			}
		}else{
			statu->CurrentMs = SysMs + TdMs;
			statu->CurrentBool = InBool;
		}
		return statu->CurrentBool ;
	}
};
/*
当输入由“0”变“1”时，经过Td时间 输出触发一次 返回1，其他时刻全为0；
 */
class DelayStartTrigger:public DelayStartTime{
public:
	bool Trigger(uint8 InPut,uint32 Td,uint64 T,BASEPROSTATU_T *statu)
	{	bool ret = FALSE;
		if(TimeCell(InPut,Td,T,statu)){
			if(statu->Trigger==FALSE){
				statu->Trigger = TRUE;
				ret = TRUE;
			}
		}else{
			if(statu->Trigger){
				statu->Trigger = FALSE;
			}
		}
		return ret;
	}
};

/*
当输入为“0”时，输出为“0”；输入由“0”变“1”时，
输出为“1”，经过时间T2延时后，输出为“0”。
 */

class PulseTime{
	public:
		bool TimeCell(bool InBool,int32 TdMs,uint64 SysMs,BASEPROSTATU_T *statu)
		{
			if(statu==NULL){
				while(1);
			}
			if (InBool){
				if(SysMs >= statu->CurrentMs){
					statu->CurrentBool = 0;
				}else{
					statu->CurrentBool = InBool;
				}
			}else{
				statu->CurrentMs = SysMs + TdMs;
				statu->CurrentBool = InBool;
			}
			return statu->CurrentBool;
		}
	private:
	//bool CurrentBool ;//当前状态
};
/*
当输入由“0“变”1“时，输出触发一次 返回1，
当输入一直为”1“时，每隔Td触发一次，返回1；
其他返回0；
 */
class PulseTrigger:public PulseTime{
public:
	bool Trigger(uint8 InPut,uint32 Td,uint64 T,BASEPROSTATU_T *statu)
	{	bool ret = FALSE;
		if(TimeCell(InPut,Td,T,statu)){
			if(statu->Trigger==FALSE){
				statu->Trigger = TRUE;
				ret = TRUE;
			}
		}else{
			if(statu->Trigger){
				statu->Trigger = FALSE;
			}
		}
		return ret;
	}
};

/*
当输入为“0”时，输出为“0”；输入由“0”变“1”时，经过时间T1延时后，输出为“1”，此前输出一直为“0”；
当输入由“1”变“0”时，经过时间T2延时后，输出为“0”，此前输出一直为“1”。
*/
class DelayStartDelayTime{
	public:
		bool TimeCell(bool InBool,int32 TdMs1,int32 TdMs2,uint64 SysMs,BASEPROSTATU2_T *statu)
		{

			if(statu==NULL){
				while(1);
			}
			if (InBool){
				statu->CurrentMs2 = SysMs + TdMs2 ;
				if(SysMs >= statu->CurrentMs1){
					statu->CurrentBool = InBool;
				}
			}else{
				statu->CurrentMs1 = SysMs + TdMs1 ;
				if(SysMs >= statu->CurrentMs2){
					statu->CurrentBool = InBool;
				}
			}
			return statu->CurrentBool;
		}
	private:
};
/*
当输入由“0”变“1”时，经过T1 输出触发一次 返回1，当输入由“1”变“0”时，经过T2输出触发一次 返回0；其他情况返回-1；
 */
class DelayStartDelayTrigger:public DelayStartDelayTime{
public:
	int16 Trigger(uint8 InPut,uint32 Td01,uint32 Td10,uint64 T,BASEPROSTATU2_T *statu)
	{	int16 ret = -1;
		if(TimeCell(InPut,Td01,Td10,T,statu)){
			if(statu->Trigger==FALSE){
				statu->Trigger = TRUE;
				ret = 1;
			}
		}else{
			if(statu->Trigger){
				statu->Trigger = FALSE;
				ret = 0;
			}
		}
		return ret;
	}
};



//////////////////////////////////保护的一些基本单元类//////////

//1功率方向计算
//公式：0.866(Ur*Ir+Ui*Ii) -0.5(Ui*Ir -Ur*Ii)
class DirecBaseCalc{
public:
       int16 Fun(SP_COMPLEX U,SP_COMPLEX I,float cosvalue,float sinvalue){
    	   int16 ret = 0;
    	   ret = cosvalue*(U.rel*I.rel + U.img*I.img) -  sinvalue*(U.img*I.rel + U.rel*I.img);
    	   if (ret>=0){
    		   ret = UNLOCK;
    	   }else{
    		   ret  = LOCK;
    	   }
    	   return ret ;
       };
};
class PowerDirect :private DirecBaseCalc{
	public:
	SP_COMPLEX *Ubc;
	SP_COMPLEX *Ia;
	SP_COMPLEX *Uca;
	SP_COMPLEX *Ib;
	SP_COMPLEX *Uab;
	SP_COMPLEX *Ic;
	SP_COMPLEX *U0 ;
	SP_COMPLEX *I0;
	float cosa[2];//相间与零序功率方向角余弦值
	float sina[2];//相间与零序功率方向角正弦值
	int16 LockStatu[2];//相间与零序功率方向闭锁状态；
	int16 Phase[4];//分项功率方向闭锁状态；
	int16 Enable[4];
	void PdFun(void){
		if(Enable[0]==1){
			Phase[0] = Fun(*Ubc,*Ia,cosa[0],sina[0]);
		}else{
			Phase[0] = UNLOCK;
		}
		if(Enable[1]==1){
			Phase[1] = Fun(*Uca,*Ib,cosa[0],sina[0]);
		}else{
			Phase[1] = UNLOCK;
		}
		if(Enable[2]==1){
		Phase[2] = Fun(*Uab,*Ic,cosa[0],sina[0]);
		}else{
			Phase[2] = UNLOCK;
		}
		if(Enable[3]==1){
		Phase[3] = Fun(*U0,*I0,cosa[1],sina[1]);
		}else{
			Phase[3] = UNLOCK;
		}
		LockStatu[0] = Phase[0]+Phase[1]+Phase[2];
		LockStatu[1] = Phase[3];
	}
};
//2电流告警类
/*
当输入为“0”时，输出为“0”；输入由“0”变“1”时，经过时间T1延时后，输出为“1”，此前输出一直为“0”；
当输入由“1”变“0”时，经过时间T2延时后，输出为“0”，此前输出一直为“1”。
*/
class DelayStartDelayPlus{
	public:
		bool Plus01_T1_1_10_T2_0(bool InBool,int32 TdMs1,int32 TdMs2,uint64 SysMs,BASEPROSTATU2_T *statu)
		{

			if(statu==NULL){
				while(1);
			}
			if (InBool){
				statu->CurrentMs2 = SysMs + TdMs2 ;
				if(SysMs >= statu->CurrentMs1){
					statu->CurrentBool = InBool;
				}
			}else{
				statu->CurrentMs1 = SysMs + TdMs1 ;
				if(SysMs >= statu->CurrentMs2){
					statu->CurrentBool = InBool;
				}
			}
			return statu->CurrentBool;
		}
	private:
};
class DelayStartAndDelayStartDelayTime:public DelayStartDelayPlus{
public:
	bool TCel1(bool InBool,uint32 TdMs,uint64 SysMs,BASEPROSTATU_T *statu)
	{
		if(statu==NULL){
			while(1);
		}
		if (InBool){
			if(SysMs >= statu->CurrentMs){
				statu->CurrentBool = InBool;
			}
		}else{
			statu->CurrentMs = SysMs + TdMs;
			statu->CurrentBool = InBool;
		}
		return statu->CurrentBool ;
	}
};


class HighCurrent:public DelayStartAndDelayStartDelayTime{
public:
	bool HighFun(float InData, float Dz)
	{
		if (InData>Dz){
			return 1;
		}else
			return 0;
	};
};


class CurrentCheck :public HighCurrent{
	public:
	float *I;//输入电流
	float Id;//电流定值
	uint32 Td[2];//定时限/后加速时限
	uint16 Enable;//使能
//	uint16 StartChz;//是否启动重合闸
	int16 *GlfxFlag;
	BASEPROSTATU_T Statu;// 时间状态
	int8 state;// 保护所处状态
	uint16 SoeId[2];//soe数据Id  定时限/后加速时限 SOEid
	uint16 SoeData;//soe防抖 后的状态；
	uint8 GjYx;//告警SOE瞬时值，没有防抖前的值；
	uint32 SoeT01;//告警状态0变成1 时间定值
	uint32 SoeT10;//告警状态1变成0 时间定值
	BASEPROSTATU2_T SoeStatu;//SOE 时间状态
	uint8 SoeFlag;
	bool SoeIRF;//告警产生标志位，注意读取后清零；
	CurrentCheck(){
		 Statu.CurrentBool = 0;
		 Statu.CurrentMs = 0;
		 Statu.CurrentMs2 = 0;
		 Statu.Trigger = 0;
		 Statu.BASEPROSTATU::CurrentBool = 0;
		 Statu.BASEPROSTATU::Trigger = 0;
		state= 0;
		// SoeId[2];
		 SoeData= 0;
		 GjYx= 0;
		// SoeT01;
		//SoeT10;
		 SoeStatu.CurrentBool  =0;//SOE 时间状态
		 SoeStatu.CurrentMs1 = 0;
		 SoeStatu.CurrentMs2 = 0;
		 SoeStatu.Trigger = 0;
		 SoeStatu.BASEPROSTATU2::CurrentBool = 0;
		 SoeStatu.BASEPROSTATU2::CurrentMs1 =0;
		 SoeStatu.BASEPROSTATU2::CurrentMs2 =0;
		 SoeStatu.BASEPROSTATU2::Trigger = 0;
		 SoeFlag= 0;
	     SoeIRF= 0;
	}
	int16 fun(uint64 SysMs,uint16 HjsFlag)
	{
		state = PROTECTSTOP;
		if(Enable==0){
			return state;
		}
		if((HighFun(*I,Id))&&((*GlfxFlag)!=1)){
			if (TCel1(1,Td[HjsFlag],SysMs,&Statu)){
				state = PROTECTOUT;                    //出段
				TCel1(0,Td[HjsFlag],SysMs,&Statu);//状态清零
				GjYx = 1;
			}else{
				state = PROTECTIN;                    //入段
			}
		}else{
			GjYx = 0;
			state = TCel1(0,Td[HjsFlag],SysMs,&Statu); //段外
		}
		if(Plus01_T1_1_10_T2_0(GjYx,SoeT01,SoeT10,SysMs,&SoeStatu)){
			if (SoeFlag==0){
				SoeFlag = 1;
				SoeIRF = TRUE;
			}
			SoeData = 1;
		}else{
			if (SoeFlag==1){
				SoeFlag = 0;
				SoeIRF = TRUE;
			}
			SoeData = 0;
		}
		return state;
	};
	bool ReadSoeIRF(void){
		if (SoeIRF){
			SoeIRF = FALSE;
			return TRUE;
		}else{
			return FALSE;
		}
	};
};

//3保护出口类
#define RUNING -1
#define REACHED 1
#define SLEEP -2
class Counter{
public:
	int16 StartConter(uint32 TdMs,uint64 SysMs,setmstime_t *RTUTimer)
	{
		if ((*RTUTimer).SetFlag!=1){
			RTUTimer->SetFlag  = 1;
			(*RTUTimer).T = SysMs +  TdMs;
			return 1;
		}
		return -1;
	};
	int16 StopConter(setmstime_t *RTUTimer)
	{
		RTUTimer->SetFlag = 0;
		RTUTimer->T = 0;
		return 1;
	};
	int16 CheckConter(uint64 SysMs,setmstime_t *RTUTimer)
	{
	    if (RTUTimer->SetFlag!=1){//未开始计时
	    	return SLEEP;
	    }
		if(SysMs >=RTUTimer->T){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			return REACHED;
		}else {
			return RUNING;
		}
	};

};

//输入有“0”变“1”是，输出为1，经过时间T时，输出变为0
class ConterDelayPlus:public DelayStartDelayPlus{
public:
	int16 StartConter(uint32 TdMs,uint64 SysMs,setmstime_t *RTUTimer)
	{
		if ((*RTUTimer).SetFlag!=1){
			RTUTimer->SetFlag  = 1;
			(*RTUTimer).T = SysMs +  TdMs;
			return 1;
		}
		return -1;
	};
	int16 StopConter(setmstime_t *RTUTimer)
	{
		RTUTimer->SetFlag = 0;
		RTUTimer->T = 0;
		return 1;
	};
	int16 CheckConter(uint64 SysMs,setmstime_t *RTUTimer)
	{
	    if (RTUTimer->SetFlag!=1){//未开始计时
	    	return SLEEP;
	    }
		if(SysMs >=RTUTimer->T){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			return REACHED;
		}else {
			return RUNING;
		}
	};
};
class ActionOut :public ConterDelayPlus{
public:

	uint32 Td;//跳闸返校时间；
	setmstime_t Statu;
	uint16 IRF;
	uint16 BreakNum;
	uint16 ActionType;
	uint16 SoeNum;
	int8 state;
/*
	uint16 *ActEnable;
	uint16 *SoeId;//soe数据Id
	uint16 *SoeData;//soe防抖 后的状态；
	uint8 *BhYx;//告警SOE瞬时值，没有防抖前的值；
	uint32 *SoeT01;//告警状态0变成1 时间定值
	uint32 *SoeT10;//告警状态1变成0 时间定值
	BASEPROSTATU2_T *SoeStatu;//SOE 时间状态
	uint8 *SoeFlag;
	uint8 *SoeIRF;//告警产生标志位，注意读取后清零；
*/
	uint16 ActEnable[6];
	uint16 SoeId[6];//soe数据Id
	uint16 SoeData[6];//soe防抖 后的状态；
	uint8 BhYx[6];//告警SOE瞬时值，没有防抖前的值；
	uint32 SoeT01[6];//告警状态0变成1 时间定值
	uint32 SoeT10[6];//告警状态1变成0 时间定值
	BASEPROSTATU2_T SoeStatu[6];//SOE 时间状态
	uint8 SoeFlag[6];
	uint8 SoeIRF[6];//告警产生标志位，注意读取后清零；

	int16 (*BreakerAction)(uint16 num,uint8 type);
	int16 (*BreakerStatus)(uint16 num,uint8 type);
	ActionOut(){
		uint8 i;
		printf("ActionOut Creat \n");
		Td = 0;//跳闸返校时间；
		Statu.Psystime = NULL;
		Statu.SetFlag = 0;
		Statu.T = 0;
		IRF =0 ;
		BreakNum =0 ;
		ActionType =0 ;
		SoeNum =0 ;
		state =0 ;
		for ( i = 0; i < 6 ; i++){
		 ActEnable[i] = 0;
		 SoeId[i] = 0;
		 SoeData[i] = 0;
		 BhYx[i] = 0;
		 SoeT01[i] = 10;
		 SoeT10[i] = 10;
			SoeStatu[i].CurrentBool  =0;
			SoeStatu[i].CurrentMs1 = 0;
			SoeStatu[i].CurrentMs2 = 0;
			SoeStatu[i].Trigger = 0;
			SoeStatu[i].BASEPROSTATU2::CurrentBool = 0;
			SoeStatu[i].BASEPROSTATU2::CurrentMs1 = 0;
			SoeStatu[i].BASEPROSTATU2::Trigger = 0;
		 SoeFlag[i] = 0;
		 SoeIRF[i] = 0;//告警产生标志位，注意读取后清零；
		}
	/*	SoeNum = Bhnum;
		ActEnable = (uint16 *)malloc(SoeNum * sizeof(uint16));
		SoeId = (uint16 *)malloc(SoeNum * sizeof(uint16));
		SoeData = (uint16 *)malloc(SoeNum * sizeof(uint16));
		BhYx = (uint8 *)malloc(SoeNum * sizeof(uint8));
		SoeT01 = (uint32 *)malloc(SoeNum * sizeof(uint32));
		SoeT10 = (uint32 *)malloc(SoeNum * sizeof(uint32));
		SoeStatu = (BASEPROSTATU2_T *)malloc(SoeNum * sizeof(BASEPROSTATU2_T));
		SoeFlag = (uint8 *)malloc(SoeNum * sizeof(SoeFlag));
		SoeIRF = (uint8 *)malloc(SoeNum * sizeof(SoeFlag));*/
	};
	int16 fun(uint64 SysMs ,uint16 BhNum,uint16 GjYxInData)
	{
		int16 ConterStatu;
		if (ActEnable[BhNum] == 1){
			if(GjYxInData==1){//有故障信号
				if(StartConter(Td,SysMs,&Statu)==1){//启动返校时间，
					BhYx[BhNum] = 1;//记录第一个启动定时器成功的保护号，
				}
				if (BreakerAction(BreakNum,ActionType)==1){
					state = OUTIN;
				}else {//跳闸失败
					state = OUTFAIL;
				}
			}else{
				BhYx[BhNum] = 0;
				state = OUTSTOP;
			}
			ConterStatu = CheckConter(SysMs,&Statu);

			if (ConterStatu==RUNING){
				if((BreakerStatus(BreakNum,CHECKFEN)==1)&&(state != OUTFAIL)){
					state = OUTCKOK;
					StopConter(&Statu);
				}
			}else if(ConterStatu==REACHED){
					state = OUTCKFAIL;
					StopConter(&Statu);
			}
			if(Plus01_T1_1_10_T2_0(BhYx[BhNum],SoeT01[BhNum],SoeT10[BhNum],SysMs,&SoeStatu[BhNum])){
				if (SoeFlag[BhNum]==0){
					SoeFlag[BhNum] = 1;
					SoeIRF[BhNum] = TRUE;
				}
				SoeData[BhNum] = 1;
			}else{
				if (SoeFlag[BhNum]==1){
					SoeFlag[BhNum] = 0;
					SoeIRF[BhNum] = TRUE;
				}
				SoeData[BhNum] = 0;
			}
		}
		return state;
	};
	bool ReadSoeIRF(uint16 BhNum){
		if (SoeIRF[BhNum]==1){
			SoeIRF[BhNum] = FALSE;
			return TRUE;
		}else{
			return FALSE;
		}
	};
/*	int16 fun(uint64 SysMs ,uint16 BhNum,uint16 GjYxInData) //0；未出口，1,出口，2，返校成功，3返校失败。
	{
		int16 ret = 0;
		if (ActEnable[BhNum] == 1){
			if(GjYxInData==1){//有故障信号
				BhYx[BhNum] = 1;
				if (BreakerAction(BreakNum,ActionType)==1){
					state = OUTIN;
				}else {//跳闸失败
					state = OUTCKFAIL;
				}
				if(IRF==0){
					IRF = 1;
					TimeCell(0,Td,SysMs,&Statu);
				}
			}else{//无故障
				BhYx[BhNum] = 0;
			}
			if(IRF==1){
				if(TimeCell(1,Td,SysMs,&Statu)){//在Td 时间内检测到断路器分闸，则成功.
					if((BreakerStatus(BreakNum,CHECKFEN)==1)){
						state = OUTCKOK;
						IRF= 0;
					}else{
						state = OUTCK;//  返校中
					}
				}else{//时间到
					IRF= 0;
					state = OUTCKFAIL;
				}
			}
		}else{
			BhYx[BhNum] = 0;
			state = OUTSTOP;
		}

		if(Plus01_T1_1_10_T2_0(BhYx[BhNum],SoeT01[BhNum],SoeT10[BhNum],SysMs,&SoeStatu[BhNum])){
			if (SoeFlag[BhNum]==0){
				SoeFlag[BhNum] = 1;
				SoeIRF[BhNum] = TRUE;
			}
			SoeData[BhNum] = 1;
		}else{
			if (SoeFlag[BhNum]==1){
				SoeFlag[BhNum] = 0;
				SoeIRF[BhNum] = TRUE;
			}
			SoeData[BhNum] = 0;
		}
		return ret;
	};

	bool ReadSoeIRF(uint16 BhNum){
		if (SoeIRF[BhNum]==1){
			SoeIRF[BhNum] = FALSE;
			return TRUE;
		}else{
			return FALSE;
		}
	};
*/
};

//4重合闸类
class ChzOut :public Counter{
public:
	int8  Times;///<重合闸次数
	int32 Time[3];///<三次重合闸时间
	int8  HjsEnable;///<后加速允许
	int32 LockTime;///<闭锁2次重合延时
	uint16 breakernum;
	uint16 CHZtype;
	uint32 OutCheckT;
	int8 state;
	uint16 MaskEnable[3][2];
	uint16 ChzEnable;
	uint8 StartFlag;
	setmstime_t chztime;
	setmstime_t chzcktime;
	setmstime_t hjsetime;
	uint8 ChzCn;
	uint16 HjsLock;
	int16(*BreakerAction)(uint16 num,uint8 type);
	int16(*BreakerStatus)(uint16 num,uint8 type);
	int16(*BreakCloseCheck)(uint8 ChzType,uint8 Enable);
	ChzOut(){
		 state = 0;
		// ChzEnable= 0;
		 StartFlag= 0;
		 ChzCn= 0;
		 HjsLock= 0;
		 chztime.Psystime = NULL;
		 chztime.SetFlag = 0;
		 chztime.T = 0;
		 chzcktime.Psystime = NULL;
		 chzcktime.SetFlag = 0;
		 chzcktime.T = 0;
		 hjsetime.Psystime = NULL;
		 hjsetime.SetFlag = 0;
		 hjsetime.T = 0;
	}
	int16 Fun(uint64 SysMs,uint16 GzStatu,uint8 secnum,uint8 protype)
	{
		int16 conterstatu;
		state = CHZSTOP;
		if(HjsLock==LOCK){
			conterstatu = CheckConter(SysMs,&hjsetime);
			if (conterstatu!=RUNING){
				HjsLock = UNLOCK;
			}
			return CHZSTOP;
		}
		if((MaskEnable[secnum][protype] == 0)||(ChzEnable == 0)){
			state = CHZSTOP;
			return CHZSTOP;
		}
		if(GzStatu == OUTCKOK){
			StartConter(Time[ChzCn],SysMs,&chztime);
		}
		conterstatu = CheckConter(SysMs,&chztime);
		if (conterstatu==REACHED){
			if (BreakCloseCheck(CHZtype,1)==1){
				if (BreakerAction(breakernum,CLOSEBREAKER)==1){
					state = CHZOUT;
					StartConter(OutCheckT,SysMs,&chzcktime);
				}else{
					state = CHZOUTFAIL;
				}
			}else{
				state = CHZOUTFAIL;
			}
		}else if (conterstatu==RUNING){
			state = CHZIN;
		}

		conterstatu = CheckConter(SysMs,&chzcktime);
		if (conterstatu==RUNING){
			state = CHZCK;
			if(BreakerStatus(breakernum,CHECKHE)==1){
					//合闸成功
				state = CHZCKOK;
				ChzCn++;
				if(ChzCn>=Times){
					ChzCn = 0;
					ChzEnable = 0;
				}
				StartConter(LockTime,SysMs,&hjsetime);
			}
		}else if (conterstatu==REACHED){
			state = CHZCKFAIL;
		}else if(state == CHZOUTFAIL){
			state = CHZCKFAIL;
		}else{//SLEEP
		}

		conterstatu = CheckConter(SysMs,&hjsetime);
		if (conterstatu==RUNING){
			state = HJSTIME;
			HjsLock = LOCK;
		}else{
			HjsLock = UNLOCK;
		}
		return state;
	}

};
#define RATIO(line,Phase) YcRatio[line].PortRatio[Phase]
class FA :public BaseApp{
private: 
	uint16 timer_id;///<内部定时器id
	//ChzBreakReg Brkstatu;
	//PowerDirect Glfx[MAX_LINE_NUM];
	//PowerDirect *GlfxHandle;
	PowerDirect GlfxHandle[MAX_LINE_NUM];
	//CurrentCheck *CurrenHandle;
	CurrentCheck CurrenHandle[MAX_LINE_NUM][3][2];
	//ActionOut *ActionHandle[4];
	ActionOut ActionHandle[MAX_LINE_NUM];
	//ChzOut *ChzHandle;
	ChzOut ChzHandle[MAX_LINE_NUM];

public:
	FaPara	fa_cfg;///配置参数
	FA();
	~FA(){};
	int16 FixTimeProtect(void);
	float Imax[MAX_LINE_NUM];
	float I0[MAX_LINE_NUM];
	float GetMax(float ia,float ib,float ic,uint8 line)
	{
		float ret = 0;
		if(ia>ib){
			if(ia>ic){
				ret = ia*RATIO(line,IA12)*4.0;
			}else{
				ret = ic*RATIO(line,IC12)*4.0;
			}
		}else{
			if(ib>ic){
				ret = ib*RATIO(line,IB12)*4.0;
			}else{
				ret = ic*RATIO(line,IC12)*4.0;
			}
		}
		return ret;
	};
/*	float GetI(uint8 Line,uint8 Itype)
	{
		if (Itype ==0 ){
			return GetMax(JB_RMS[IA2(Line)],JB_RMS[IB2(Line)],JB_RMS[IC2(Line)]);
		}else if(Itype ==1){
			return JB_RMS[I01(Line)];
		}
		return 0;
	};*/
	int16 GetParaId(int8 SecNum,int8 LineNum);
	int16 ReadCfg(uint32 Id);
	//int16 WriteCfg();
	int16 Init();
	void ParaInit(void);
	void ParaChange(void);
	void FunHandleInit(void);
	int16 Run();	
	//BASELOGIC LogicTool;
	//DelayStartTime Logic01_T_1;
	//PulseTime Logic01_1_T_0;
	//DelayStartDelayTime Logic01_T1_1_10_T2_0;
	//DelayStartTrigger Trigger01_T_10;
	//PulseTrigger TriggerX_T_1_10;
	//DelayStartDelayTrigger Trigger01_T1_1X_10_T2_0X;
};

extern FA fa;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* fa_main(void*);
#endif
