#include <string.h>///<memcpy
//#include <unistd.h>///<for sleep
#include "scada.h"
#include "message.h"///
//#include "timer.h"//
#include "task.h"//
#include "fa.h"///<
#include "debug.h"
#include "mathlist.h"
//#include "fft.h"
#include "fpga.h"
#include "yx.h"
#include "yk.h"
#define YKTIME  d_5092.Data
#define SaveProSoe(SoeId,DATA) PushSoe(PROTECTSOE,SoeId,DATA)
#pragma DATA_SECTION("faDATA")
FA fa;
void* fa_main(void*){
	fa.Run();
	return NULL;
}

FA::FA(){
	strcpy(name,"Fa");
	Id=TASK_FA;
	//Init();
	//GlfxHandle = NULL;
	memset(&GlfxHandle,0,sizeof(GlfxHandle));
	//CurrenHandle = NULL;
	//ActionHandle[0] = NULL;
	//ActionHandle[1] = NULL;
	//ActionHandle[2] = NULL;
	//ActionHandle[3] = NULL;
	//ChzHandle= NULL;
}
int16 BreakerStatu(uint16 breakernum,uint8 type)
{

	if (type==CHECKHE){
		if((YxNature[YxConfigure.ad_compass[HE(breakernum)]]==HE_HE)
		||(YxNature[YxConfigure.ad_compass[FEN(breakernum)]]==FEN_HE))
		{
			return 1;
		}else{
			return -1;
		}
	}
	if (type==CHECKFEN){
		if((YxNature[YxConfigure.ad_compass[HE(breakernum)]]==HE_FEN)
		||(YxNature[YxConfigure.ad_compass[FEN(breakernum)]]==FEN_FEN))
		{
			return 1;
		}else{
			return -1;
		}
	}
	return -1;
}

int16 ProtectOut(uint16 breakernum, uint8 type)
{
	uint8 YkPort;
	uint8 FpgaNum;
	int16 erro = 1;
	uint16 YkTd;
	YkTd = d_5092.Data;
	YkPort = d_508A.Data.Define[breakernum].Port;
	if((FpgaCfg.YxCardSize<YkPort)||(FpgaCfg.YxCardSize==0)){
		return -1;
	}
	FpgaNum = FpgaCfg.YxCard[YkPort].num;

	if(type == OPENBREAKER){
		if(BreakerStatu(breakernum,CHECKHE)==1){
			erro = FpgaOut(FpgaNum,YkPort,OPENBREAKER,YkTd);
		}else {
			erro = -1;
		}
	}else {
		if(BreakerStatu(breakernum,CHECKFEN)==1){
			erro = FpgaOut(FpgaNum,YkPort,CLOSEBREAKER,YkTd);
		}else {
			erro = -1;
		}
	}
	//scada.BreakRecord(breakernum,BRKRCDSTART);
	return erro;
}
int16 CloseBreakerCheck(uint8 CkTp,uint8 Enable){
	return BreakCloseCheck(CkTp,Enable);
}
#pragma CODE_SECTION("FaInDDR2_CODE")
void FA::ParaInit(void){
	fa_cfg.CurrentProSets[0].Pdir.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].P0dir.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].Pdir.Data.Anagle = 0.78539815;//45
	fa_cfg.CurrentProSets[0].P0dir.Data.Anagle =1.2217304 ;//70

	fa_cfg.CurrentProSets[0].Id1.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].Id1.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].Id1.Data.LimitTime = 0;
	fa_cfg.CurrentProSets[0].Id1.Data.EnableOut = 1;
	fa_cfg.CurrentProSets[0].Id2.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].Id2.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].Id2.Data.LimitTime = 100;
	fa_cfg.CurrentProSets[0].Id2.Data.EnableOut = 1;
	fa_cfg.CurrentProSets[0].Id3.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].Id3.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].Id3.Data.LimitTime = 500;
	fa_cfg.CurrentProSets[0].Id3.Data.EnableOut = 1;
	fa_cfg.CurrentProSets[0].I0d1.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].I0d1.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].I0d1.Data.LimitTime = 0;
	fa_cfg.CurrentProSets[0].I0d1.Data.EnableOut = 1;
	fa_cfg.CurrentProSets[0].I0d2.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].I0d2.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].I0d2.Data.LimitTime = 100;
	fa_cfg.CurrentProSets[0].I0d2.Data.EnableOut = 1;
	fa_cfg.CurrentProSets[0].I0d3.Data.Enable = 1;
	fa_cfg.CurrentProSets[0].I0d3.Data.LimitValue = 10.0;
	fa_cfg.CurrentProSets[0].I0d3.Data.LimitTime = 500;
	fa_cfg.CurrentProSets[0].I0d3.Data.EnableOut = 1;
	memcpy(&fa_cfg.CurrentProSets[1],&fa_cfg.CurrentProSets[0],sizeof(CurrentProSets_t));
	fa_cfg.d_504B.Data.AcceTimes[0]=0;
	fa_cfg.d_504B.Data.AcceTimes[1]=0;
	fa_cfg.d_504B.Data.AcceTimes[2]=0;
	fa_cfg.d_504B.Data.AcceTimes[3]=0;
	fa_cfg.d_504B.Data.Type = 0;
	fa_cfg.d_504B.Data.Enable =1;
	fa_cfg.d_504B.Data.LockTime = 3000;
	fa_cfg.d_504B.Data.Time[0] = 1000;
	fa_cfg.d_504B.Data.Time[1] = 2000;
	fa_cfg.d_504B.Data.Time[2] = 5000;
	fa_cfg.d_504B.Data.Times = 3;
}
#pragma CODE_SECTION("FaInDDR2_CODE")
void FA::FunHandleInit(void){
	uint16 i,j,k;

		//GlfxHandle = new PowerDirect[MAX_LINE_NUM];
		GlfxHandle[0].Uab = &LineVol[Uab1];
		GlfxHandle[0].Ubc = &LineVol[Ubc1];
		GlfxHandle[0].Uca = &LineVol[Uca1];
		GlfxHandle[0].Ia = &FFT_OUT[IA12][BASEHARMONIC];
		GlfxHandle[0].Ib = &FFT_OUT[IB12][BASEHARMONIC];
		GlfxHandle[0].Ic = &FFT_OUT[IC12][BASEHARMONIC];
		GlfxHandle[0].I0 = &FFT_OUT[I011][BASEHARMONIC];
		GlfxHandle[0].Enable[0] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;
		GlfxHandle[0].Enable[1] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;
		GlfxHandle[0].Enable[2] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;
		GlfxHandle[0].Enable[3] = fa_cfg.CurrentProSets[0].P0dir.Data.Enable;
		GlfxHandle[0].cosa[0] = cossp(fa_cfg.CurrentProSets[0].Pdir.Data.Anagle);
		GlfxHandle[0].sina[0] = sinsp(fa_cfg.CurrentProSets[0].Pdir.Data.Anagle);
		GlfxHandle[0].cosa[1] = cossp(fa_cfg.CurrentProSets[0].P0dir.Data.Anagle);
		GlfxHandle[0].sina[1] = sinsp(fa_cfg.CurrentProSets[0].P0dir.Data.Anagle);

		GlfxHandle[1].Uab = &LineVol[Uab2];
		GlfxHandle[1].Ubc = &LineVol[Ubc2];
		GlfxHandle[1].Uca = &LineVol[Uca2];
		GlfxHandle[1].Ia = &FFT_OUT[IA22][BASEHARMONIC];
		GlfxHandle[1].Ib = &FFT_OUT[IB22][BASEHARMONIC];
		GlfxHandle[1].Ic = &FFT_OUT[IC22][BASEHARMONIC];
		GlfxHandle[1].I0 = &FFT_OUT[I021][BASEHARMONIC];
		GlfxHandle[1].Enable[0] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
		GlfxHandle[1].Enable[1] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
		GlfxHandle[1].Enable[2] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
		GlfxHandle[1].Enable[3] = fa_cfg.CurrentProSets[1].P0dir.Data.Enable;
		GlfxHandle[1].cosa[0] = cossp(fa_cfg.CurrentProSets[1].Pdir.Data.Anagle);
		GlfxHandle[1].sina[0] = sinsp(fa_cfg.CurrentProSets[1].Pdir.Data.Anagle);
		GlfxHandle[1].cosa[1] = cossp(fa_cfg.CurrentProSets[1].P0dir.Data.Anagle);
		GlfxHandle[1].sina[1] = sinsp(fa_cfg.CurrentProSets[1].P0dir.Data.Anagle);

		//CurrenHandle = new CurrentCheck[12];
		for (i = 0; i < 2; i++){
			for ( j = 0; j < 3; j++){
				for ( k = 0; k < 2; k++){
					if (k==0){//
					  if (j==0){
						  CurrenHandle[i][j][k].I = &Imax[i];
						  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].Id1.Data.Enable;
						  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].Id1.Data.LimitValue;
						  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].Id1.Data.LimitTime;
						  CurrenHandle[i][j][k].Td[1] = fa_cfg.CurrentProSets[i].Id1.Data.LimitTime;
						  CurrenHandle[i][j][k].GjYx = 0;
						  CurrenHandle[i][j][k].SoeId[0] = 0x0007 + i*0x001a;
						  CurrenHandle[i][j][k].SoeId[1] = 0x0007 + i*0x001a;//����ٸ澯SOE
						  CurrenHandle[i][j][k].SoeFlag = 0;
						  CurrenHandle[i][j][k].SoeIRF = 0;
						  CurrenHandle[i][j][k].SoeT01 = 100;
						  CurrenHandle[i][j][k].SoeT10 = 100;
						  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[0];
					  }else if(j==1){
						  CurrenHandle[i][j][k].I = &Imax[i];
						  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].Id2.Data.Enable;
						  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].Id2.Data.LimitValue;
						  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].Id2.Data.LimitTime;
						  CurrenHandle[i][j][k].Td[1] = fa_cfg.d_504B.Data.AcceTimes[0];
						  CurrenHandle[i][j][k].GjYx = 0;
						  CurrenHandle[i][j][k].SoeId[0] = 0x0008 + i*0x001a;
						  CurrenHandle[i][j][k].SoeId[1] = 0x0008 + i*0x001a;//����ٸ澯SOE
						  CurrenHandle[i][j][k].SoeFlag = 0;
						  CurrenHandle[i][j][k].SoeIRF = 0;
						  CurrenHandle[i][j][k].SoeT01 = 100;
						  CurrenHandle[i][j][k].SoeT10 = 100;
						  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[0];
					  }else if(j==2){
						  CurrenHandle[i][j][k].I = &Imax[i];
						  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].Id3.Data.Enable;
						  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].Id3.Data.LimitValue;
						  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].Id3.Data.LimitTime;
						  CurrenHandle[i][j][k].Td[1] = fa_cfg.d_504B.Data.AcceTimes[1];
						  CurrenHandle[i][j][k].GjYx = 0;
						  CurrenHandle[i][j][k].SoeId[0] = 0x0009 + i*0x001a;
						  CurrenHandle[i][j][k].SoeId[1] = 0x0009 + i*0x001a;//����ٸ澯SOE
						  CurrenHandle[i][j][k].SoeFlag = 0;
						  CurrenHandle[i][j][k].SoeIRF = 0;
						  CurrenHandle[i][j][k].SoeT01 = 100;
						  CurrenHandle[i][j][k].SoeT10 = 100;
						  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[0];
					  }
					}else {
						 if (j==0){
						  CurrenHandle[i][j][k].I = &I0[i];
						  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].I0d1.Data.Enable;
						  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].I0d1.Data.LimitValue;
						  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].I0d1.Data.LimitTime;
						  CurrenHandle[i][j][k].Td[1] = fa_cfg.CurrentProSets[i].I0d1.Data.LimitTime;
						  CurrenHandle[i][j][k].GjYx = 0;
						  CurrenHandle[i][j][k].SoeId[0] = 0x000A + i*0x001a;
						  CurrenHandle[i][j][k].SoeId[1] = 0x000A + i*0x001a;//����ٸ澯SOE
						  CurrenHandle[i][j][k].SoeFlag = 0;
						  CurrenHandle[i][j][k].SoeIRF = 0;
						  CurrenHandle[i][j][k].SoeT01 = 100;
						  CurrenHandle[i][j][k].SoeT10 = 100;
						  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[1];
					  }else if(j==1){
						  CurrenHandle[i][j][k].I = &I0[i];
						  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].I0d2.Data.Enable;
						  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].I0d2.Data.LimitValue;
						  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].I0d2.Data.LimitTime;
						  CurrenHandle[i][j][k].Td[1] = fa_cfg.d_504B.Data.AcceTimes[2];
						  CurrenHandle[i][j][k].GjYx = 0;
						  CurrenHandle[i][j][k].SoeId[0] = 0x000A + i*0x001a;
						  CurrenHandle[i][j][k].SoeId[1] = 0x000A + i*0x001a;//����ٸ澯SOE
						  CurrenHandle[i][j][k].SoeFlag = 0;
						  CurrenHandle[i][j][k].SoeIRF = 0;
						  CurrenHandle[i][j][k].SoeT01 = 100;
						  CurrenHandle[i][j][k].SoeT10 = 100;
						  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[1];
					  }else if(j==2){
					  CurrenHandle[i][j][k].I = &I0[i];
					  CurrenHandle[i][j][k].Enable = fa_cfg.CurrentProSets[i].I0d3.Data.Enable;
					  CurrenHandle[i][j][k].Id = fa_cfg.CurrentProSets[i].I0d3.Data.LimitValue;
					  CurrenHandle[i][j][k].Td[0] = fa_cfg.CurrentProSets[i].I0d3.Data.LimitTime;
					  CurrenHandle[i][j][k].Td[1] = fa_cfg.d_504B.Data.AcceTimes[3];
					  CurrenHandle[i][j][k].GjYx = 0;
					  CurrenHandle[i][j][k].SoeId[0] = 0x000A + i*0x001a;
					  CurrenHandle[i][j][k].SoeId[1] = 0x000A + i*0x001a;//����ٸ澯SOE
					  CurrenHandle[i][j][k].SoeFlag = 0;
					  CurrenHandle[i][j][k].SoeIRF = 0;
					  CurrenHandle[i][j][k].SoeT01 = 100;
					  CurrenHandle[i][j][k].SoeT10 = 100;
					  CurrenHandle[i][j][k].GlfxFlag = &GlfxHandle[i].LockStatu[1];
					}
					}
				}
			}
		}

	//	for (i = 0;i < MAX_LINE_NUM; i++){
	//		ActionHandle[i] = new ActionOut(6);
	//	}
		for (i = 0; i < 2; i++){
			for ( j = 0; j < 3; j++){
				for ( k = 0; k < 2; k++){
					if(k==0){
						if (j==0){
							ActionHandle[i].ActEnable[j+k]=fa_cfg.CurrentProSets[i].Id1.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+k] = 0;
							ActionHandle[i].SoeT01[j+k] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+k] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+k] = 0x000c + i*0x001a;
							ActionHandle[i].SoeIRF[i+k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==1){
							ActionHandle[i].ActEnable[j+k]=fa_cfg.CurrentProSets[i].Id2.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+k] = 0;
							ActionHandle[i].SoeT01[j+k] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+k] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+k] = 0x000D + i*0x001a;
							ActionHandle[i].SoeIRF[i+k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==2){
							ActionHandle[i].ActEnable[j+k]=fa_cfg.CurrentProSets[i].Id3.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+k] = 0;
							ActionHandle[i].SoeT01[j+k] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+k] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+k] = 0x000E + i*0x001a;
							ActionHandle[i].SoeIRF[i+k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}

					}else{
						if (j==0){
							ActionHandle[i].ActEnable[j+3]=fa_cfg.CurrentProSets[i].I0d1.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+3] = 0;
							ActionHandle[i].SoeT01[j+3] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+3] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+3] = 0x000F + i*0x001a;
							ActionHandle[i].SoeIRF[i+3] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==1){
							ActionHandle[i].ActEnable[j+3]=fa_cfg.CurrentProSets[i].I0d2.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+3] = 0;
							ActionHandle[i].SoeT01[j+3] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+3] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+3] = 0x000F + i*0x001a;
							ActionHandle[i].SoeIRF[i+3] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==2){
							ActionHandle[i].ActEnable[j+3]=fa_cfg.CurrentProSets[i].I0d3.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = i;
							ActionHandle[i].BhYx[j+3] = 0;
							ActionHandle[i].SoeT01[j+3] =100 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j+3] =100 ;//告警状态1变成0 时间定值
							ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[i+3] = 0x000F + i*0x001a;
							ActionHandle[i].SoeIRF[i+3] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}
					}
				}
			}
		}
		//ChzHandle  = new ChzOut[MAX_LINE_NUM];
		for ( i = 0; i < 2; i++){
			ChzHandle[i].BreakerAction = ProtectOut;
			ChzHandle[i].BreakerStatus = BreakerStatu;
			ChzHandle[i].BreakCloseCheck = CloseBreakerCheck;
			ChzHandle[i].CHZtype = fa_cfg.d_504B.Data.Type;
			ChzHandle[i].ChzCn = 0;
			ChzHandle[i].MaskEnable[0][0] =0;
			ChzHandle[i].MaskEnable[0][1] =0;
			ChzHandle[i].MaskEnable[1][0] =1;
			ChzHandle[i].MaskEnable[1][1] =1;
			ChzHandle[i].MaskEnable[2][0] =1;
			ChzHandle[i].MaskEnable[2][1] =1;
			ChzHandle[i].ChzEnable = 1;
			ChzHandle[i].HjsEnable = fa_cfg.d_504B.Data.Enable;
			ChzHandle[i].HjsLock = 0;
			ChzHandle[i].LockTime = fa_cfg.d_504B.Data.LockTime;
			ChzHandle[i].OutCheckT = YKTIME;
			ChzHandle[i].StartFlag = 0;
			ChzHandle[i].Time[0] = fa_cfg.d_504B.Data.Time[0];
			ChzHandle[i].Time[1] = fa_cfg.d_504B.Data.Time[1];
			ChzHandle[i].Time[2] = fa_cfg.d_504B.Data.Time[2];
			ChzHandle[i].Times = fa_cfg.d_504B.Data.Times;
			ChzHandle[i].breakernum = i;
		}
}
#pragma CODE_SECTION("FaInDDR2_CODE")
int16 FA::Init(){
	ParaInit();
	FunHandleInit();
	return 1 ;
}
#pragma CODE_SECTION("FaInDDR2_CODE")
int16 FA::GetParaId(int8 SecNum,int8 LineNum)
{
	Data<BaseDataType> data;
	uint32 Id;
	if(SecNum>=0&&SecNum<=7){
		Id = 0x5001+SecNum*9;
		//ProtectSet d_5001;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].Id1,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5002;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].Id2,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5003;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].Id3,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5004;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].I0d1,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5005;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].I0d2,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5006;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].I0d3,data.Data,sizeof(ProtectSet));
		//ProtectSet d_5007;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].U0d,data.Data,sizeof(ProtectSet));
		//ProtectDirSet d_5008;///< 电流方向保护投入定值
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].Pdir,data.Data,sizeof(ProtectDirSet));
		//ProtectDirSet d_5009;///< 零序电流方向保护投入定值
		user_data.GetData(Id++,&data);
		memcpy(&fa_cfg.CurrentProSets[LineNum].P0dir,data.Data,sizeof(ProtectDirSet));
		return 1;
	}
	return -1;
}
#pragma CODE_SECTION("FaInDDR2_CODE")
int16 FA::ReadCfg(uint32 Id)
{
	Data<BaseDataType> data;
	user_data.GetData(Id,&data);
	switch(Id)
	{
		case 0x5049:
		//DataType1  d_5049;///<线路1保护定值区号设置
		memcpy(&fa_cfg.d_5049,data.Data,sizeof(fa_cfg.d_5049));
		GetParaId(fa_cfg.d_5049.Data,0);
		FunHandleInit();
		break;
		case 0x504A:
		//DataType1  d_504A;///<线路2保护定值区号设置
		memcpy(&fa_cfg.d_504A,data.Data,sizeof(fa_cfg.d_504A));
		GetParaId(fa_cfg.d_504A.Data,1);
		FunHandleInit();
		break;
		case 0x504b:
		//RecloseSet d_504B;///<重合闸设置
		memcpy(&fa_cfg.d_504B,data.Data,sizeof(fa_cfg.d_504B));
		GetParaId(fa_cfg.d_5049.Data,1);
		break;
		case 0x5055:
		//FaDevotionSet d_5055;///<保护功能投退设置
		memcpy(&fa_cfg.d_5055,data.Data,sizeof(fa_cfg.d_5055));
		GetParaId(fa_cfg.d_5049.Data,1);
		break;
		default:
		break;
	}
	return 1;
}
#pragma CODE_SECTION("FaInDDR2_CODE")
int16 FA::Run()
{
	Init();
	ReadCfg(0);
	FixTimeProtect();
	return 1;
}
uint8 linenum = 2;
uint8 secnum = 3;
uint8 protype = 2;
int16 FA::FixTimeProtect(void)
{
	uint8 i,j,k;
	uint64 SysMs;
	int16 gzstatu;
	int16 chzstatu;
	uint16 HjsFlag = 0;
	SysMs = clk0.GetSysMs();
	for (i = 0;i<linenum; i++){//
		//Imax[i] = GetMax(JB_RMS[IA2(i)],JB_RMS[IB2(i)],JB_RMS[IC2(i)],i);
		//I0[i]= JB_RMS[I01(i)]*RATIO(i,I011)*4.0;
		GlfxHandle[i].PdFun();
		for (j = 0; j < secnum; j++){//
			for (k = 0; k < protype; k++){//
				CurrenHandle[i][j][k].fun(SysMs,HjsFlag);
				if(CurrenHandle[i][j][k].ReadSoeIRF()){
					SaveProSoe(CurrenHandle[i][j][k].SoeId[HjsFlag],CurrenHandle[i][j][k].SoeData);  //产生软遥信
				}
				ActionHandle[i].fun(SysMs,j+k*3,CurrenHandle[i][j][k].GjYx);
				if(ActionHandle[i].ReadSoeIRF(j+k*3)){
					if(ActionHandle[i].SoeData[j+k*3]==1){
						SaveProSoe(ActionHandle[i].SoeId[j+k*3],ActionHandle[i].SoeData[j+k*3]);//产生保护遥信；
					}
				}
				gzstatu = ActionHandle[i].state;
				if(gzstatu==OUTCKOK){
					SaveProSoe(ActionHandle[i].SoeId[j+k*3],ActionHandle[i].SoeData[j+k*3]);//跳闸成功SOE
				}else if(gzstatu==OUTCKFAIL){
					//SaveProSoe(ActionHandle[i]->SoeId[j+k],ActionHandle[i]->SoeData[j+k]);	//跳闸失败SOE
				}
				chzstatu = ChzHandle[i].Fun(SysMs,gzstatu,j,k);
				if(chzstatu ==HJSTIME){
					HjsFlag = 1;
				}else if (chzstatu ==CHZCKOK){
					SaveProSoe(0x001a,1);//重合闸成功
				}else if (chzstatu ==CHZCKFAIL){
					SaveProSoe(0x001a,0);//重合闸失败
				}else if (chzstatu ==CHZOUT){
					SaveProSoe(0x0019,1);//启动重合闸
				}
			}
		}
	}
	return 1;
}

//int16 FA::WriteCfg(void)
//{/
//	return 1;
//}
