#include <string.h>///<memcpy
//#include <unistd.h>///<for sleep
#include "scada.h"
#include "message.h"//
#include "task.h"//
#include "fa.h"///<
#include "debug.h"
#include "mathlist.h"
#include "fpga.h"
#include "evmomapl138_timer.h"
#include "gpio.h"
#include "inout.h"
#define YKTIME  scada.scada_cfg.d_5092.Data
CP56Time2a PROTECTTIME;
#define GetYxNature(YXID) (scada.GetYxNature(YXID))


#pragma DATA_SECTION("DATAINDDR2")
FA fa;
FA::FA(){
	strcpy(name,"Fa");
	Id=TASK_FA;
#ifdef GLFX
	memset(&GlfxHandle,0,sizeof(GlfxHandle));
#endif
}

void* fa_main(void*){
	fa.Run();
	return NULL;
}
int16 BreakerStatu(uint16 break_no,uint8 type)
{
/*	uint16 breakpos  = 0;
	if(break_no==BREAKER1){
		breakpos = 0;
	}else if(break_no==BREAKER2){
		breakpos = 1;
	}
    if(BreakLockState[breakpos]==LOCK){
    	return -1;
    }
	if(scada.yx_config.config_data[YFJD].Data==JIUDI){//接地位置直接退出
		return -1;
	}
	if (type==CHECKHE){
		if((scada.yx_config.config_data[HE(breakpos)].Data==1)
		||(scada.yx_config.config_data[FEN(breakpos)].Data==0))
		{
			return 1;
		}else{
			return -1;
		}
	}
	if (type==CHECKFEN){
		if((scada.yx_config.config_data[HE(breakpos)].Data==0)
		||(scada.yx_config.config_data[FEN(breakpos)].Data==1))
		{
			return 1;
		}else{
			return -1;
		}
	}*/
	return -1;
}

int16 ProtectOut(uint16 break_no, uint8 type)
{
/*	uint8 YkPort;
	uint8 SlotNum;
	int16 erro = 1;
	uint16 YkTd;
	uint16 i;
	uint16 breakpos  = 0;
	if(break_no==BREAKER1){
		breakpos = 0;
	}else if(break_no==BREAKER2){
		breakpos = 1;
	}
    if(BreakLockState[breakpos]==LOCK){//闭锁
    	return -1;
    }
	YkTd = scada.scada_cfg.d_5093.Data;
	YkPort = scada.scada_cfg.d_508A.Data.Define[break_no].ClosePort;
	if((FpgaCfg.YxCardSize<YkPort)||(FpgaCfg.YxCardSize==0)){
		return -1;
	}
	//FpgaNum = FpgaCfg.YxCard[YkPort].num;//
	for (i = 0 ; i < FpgaCfg.YxCardSize; i++){
		if(FpgaCfg.YxCard[i].SlotNum>=SLAVE_BOARD_NUM){
			return -1;
		}
		if(FpgaCfg.YxCard[i].IdNum==YkPort){
			SlotNum =FpgaCfg.YxCard[i].SlotNum;
			break;
		}
	}

	if(type == OPENBREAKER){
		if(BreakerStatu(break_no,CHECKHE)==1){
			erro = FpgaOut(SlotNum,YkPort,OPENBREAKER,YkTd);
		}else {
			erro = -1;
		}
	}else {
		if(BreakerStatu(break_no,CHECKFEN)==1){
			erro = FpgaOut(SlotNum,YkPort,CLOSEBREAKER,YkTd);
		}else {
			erro = -1;
		}
	}
	//scada.BreakRecord(break_no,BRKRCDSTART);
	return erro;*/
	return 1;
}
int16 BreakCloseCheck(uint8 CkTp,uint8 Enable){
	return scada.BreakReCloceCheck(CkTp,Enable);
}
#pragma CODE_SECTION("CODEINDDR2")
void FA::ParaInit(void){
	uint8 i;
	fa_cfg.CurrentProSets[0].Pdir.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].P0dir.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].Pdir.Data.Anagle = 0.78539815;//45
	fa_cfg.CurrentProSets[0].P0dir.Data.Anagle =1.2217304 ;//70
	fa_cfg.CurrentProSets[0].Id1.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].Id1.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].Id1.Data.LimitTime = 0;
	fa_cfg.CurrentProSets[0].Id1.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].Id2.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].Id2.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].Id2.Data.LimitTime = 100;
	fa_cfg.CurrentProSets[0].Id2.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].Id3.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].Id3.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].Id3.Data.LimitTime = 500;
	fa_cfg.CurrentProSets[0].Id3.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].I0d1.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].I0d1.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].I0d1.Data.LimitTime = 0;
	fa_cfg.CurrentProSets[0].I0d1.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].I0d2.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].I0d2.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].I0d2.Data.LimitTime = 100;
	fa_cfg.CurrentProSets[0].I0d2.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].I0d3.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].I0d3.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].I0d3.Data.LimitTime = 500;
	fa_cfg.CurrentProSets[0].I0d3.Data.EnableOut = 0;
	fa_cfg.CurrentProSets[0].U0d.Data.Enable = 0;
	fa_cfg.CurrentProSets[0].U0d.Data.LimitValue = 100.0;
	fa_cfg.CurrentProSets[0].U0d.Data.LimitTime = 500;
	fa_cfg.CurrentProSets[0].U0d.Data.EnableOut = 0;
	for ( i = 1; i<MAX_LINE_NUM;i++){
		memcpy(&fa_cfg.CurrentProSets[i],&fa_cfg.CurrentProSets[0],sizeof(CurrentProSets_t));
	}
	fa_cfg.d_504B.Data.AcceTimes[0]=0;
	fa_cfg.d_504B.Data.AcceTimes[1]=0;
	fa_cfg.d_504B.Data.AcceTimes[2]=0;
	fa_cfg.d_504B.Data.AcceTimes[3]=0;
	fa_cfg.d_504B.Data.Type = 0;
	fa_cfg.d_504B.Data.Enable =0;
	fa_cfg.d_504B.Data.LockTime = 3000;
	fa_cfg.d_504B.Data.Time[0] = 1000;
	fa_cfg.d_504B.Data.Time[1] = 2000;
	fa_cfg.d_504B.Data.Time[2] = 5000;
	fa_cfg.d_504B.Data.Times = 3;
	fa_cfg.NoShutOffCurr[0]=100;
	fa_cfg.NoShutOffCurr[1]=100;
}
typedef struct SECNUM{
	uint32 d1:4;
	uint32 d2:4;
	uint32 d3:4;
	uint32 d4:4;
	uint32 d5:4;
	uint32 d6:4;
	uint32 d7:4;
	uint32 d8:4;
}SECNUM;
typedef union SECUNION{
   struct SECNUM  bit;
   uint32 all;
}SECUNION;
#define PERLINESOFTYXNUM YX_0017
void FA::HandleInit(void){

	uint16 i,j;
#ifdef GLFX
	//GlfxHandle = new PowerDirect[MAX_LINE_NUM];
	GlfxHandle[0].Uab = &LineVol[Uab1];
	GlfxHandle[0].Ubc = &LineVol[Ubc1];
	GlfxHandle[0].Uca = &LineVol[Uca1];
	GlfxHandle[0].U0 = &fft_out_fa[U01][BASEHARMONIC];
	GlfxHandle[0].Ia = &fft_out_fa[IA12][BASEHARMONIC];
	GlfxHandle[0].Ib = &fft_out_fa[IB12][BASEHARMONIC];
	GlfxHandle[0].Ic = &fft_out_fa[IC12][BASEHARMONIC];
	GlfxHandle[0].I0 = &fft_out_fa[I011][BASEHARMONIC];
	GlfxHandle[0].Ua = &JB_YC[UA1];
	GlfxHandle[0].Ub = &JB_YC[UB1];
	GlfxHandle[0].Uc = &JB_YC[UC1];
	GlfxHandle[0].Uzero = &JB_YC[U01];
	GlfxHandle[0].ia = &JB_YC[IA12];
	GlfxHandle[0].ib = &JB_YC[IB12];
	GlfxHandle[0].ic = &JB_YC[IC12];
	GlfxHandle[0].i0 = &JB_YC[I011];
	GlfxHandle[0].Ulimit = NOMINAL_U *0.3;
	GlfxHandle[0].Ilimit = NOMINAL_I *0.002;
	GlfxHandle[0].Enable[0] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;// phase a
	GlfxHandle[0].Enable[1] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;// phase b
	GlfxHandle[0].Enable[2] = fa_cfg.CurrentProSets[0].Pdir.Data.Enable;// phase c
	GlfxHandle[0].dir =  fa_cfg.CurrentProSets[0].Pdir.Data.Dir;
	GlfxHandle[0].Enable[3] = fa_cfg.CurrentProSets[0].P0dir.Data.Enable;
	GlfxHandle[0].dir0 = fa_cfg.CurrentProSets[0].P0dir.Data.Dir;

	GlfxHandle[0].cosa[0] = cossp(((float)fa_cfg.CurrentProSets[0].Pdir.Data.Anagle*PI)/180.0);
	GlfxHandle[0].sina[0] = sinsp(((float)fa_cfg.CurrentProSets[0].Pdir.Data.Anagle*PI)/180.0);
	GlfxHandle[0].cosa[1] = cossp(((float)fa_cfg.CurrentProSets[0].P0dir.Data.Anagle*PI)/180.0);
	GlfxHandle[0].sina[1] = sinsp(((float)fa_cfg.CurrentProSets[0].P0dir.Data.Anagle*PI)/180.0);

	GlfxHandle[1].Uab = &LineVol[Uab2];
	GlfxHandle[1].Ubc = &LineVol[Ubc2];
	GlfxHandle[1].Uca = &LineVol[Uca2];
	GlfxHandle[1].U0 = &fft_out_fa[U02][BASEHARMONIC];
	GlfxHandle[1].Ia = &fft_out_fa[IA22][BASEHARMONIC];
	GlfxHandle[1].Ib = &fft_out_fa[IB22][BASEHARMONIC];
	GlfxHandle[1].Ic = &fft_out_fa[IC22][BASEHARMONIC];
	GlfxHandle[1].I0 = &fft_out_fa[I021][BASEHARMONIC];
	GlfxHandle[1].Ua = &JB_YC[UA2];
	GlfxHandle[1].Ub = &JB_YC[UB2];
	GlfxHandle[1].Uc = &JB_YC[UC2];
	GlfxHandle[1].Uzero = &JB_YC[U02];
	GlfxHandle[1].ia = &JB_YC[IA22];
	GlfxHandle[1].ib = &JB_YC[IB22];
	GlfxHandle[1].ic = &JB_YC[IC22];
	GlfxHandle[1].i0 = &JB_YC[I021];
	GlfxHandle[1].Ulimit = NOMINAL_U *0.3;
	GlfxHandle[1].Ilimit = NOMINAL_I *0.002;
	GlfxHandle[1].Enable[0] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
	GlfxHandle[1].Enable[1] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
	GlfxHandle[1].Enable[2] = fa_cfg.CurrentProSets[1].Pdir.Data.Enable;
	GlfxHandle[1].dir =  fa_cfg.CurrentProSets[1].Pdir.Data.Dir;
	GlfxHandle[1].Enable[3] = fa_cfg.CurrentProSets[1].P0dir.Data.Enable;
	GlfxHandle[1].dir0 = fa_cfg.CurrentProSets[1].P0dir.Data.Dir;
	GlfxHandle[1].cosa[0] = cossp(((float)fa_cfg.CurrentProSets[1].Pdir.Data.Anagle*PI)/180.0);
	GlfxHandle[1].sina[0] = sinsp(((float)fa_cfg.CurrentProSets[1].Pdir.Data.Anagle*PI)/180.0);
	GlfxHandle[1].cosa[1] = cossp(((float)fa_cfg.CurrentProSets[1].P0dir.Data.Anagle*PI)/180.0);
	GlfxHandle[1].sina[1] = sinsp(((float)fa_cfg.CurrentProSets[1].P0dir.Data.Anagle*PI)/180.0);
#endif
		for (i = 0; i < MAX_LINE_NUM; i++){//	相间短路  参数有三段，实际上相间短路只用了一段。

			GrdCurrenHandle[i].I = &I0[i];
			GrdCurrenHandle[i].Enable = fa_cfg.CurrentProSets[i].I0d1.Data.Enable;
			GrdCurrenHandle[i].Id = fa_cfg.CurrentProSets[i].I0d1.Data.LimitValue;
			GrdCurrenHandle[i].Td[0] = fa_cfg.CurrentProSets[i].I0d1.Data.LimitTime;
			GrdCurrenHandle[i].Td[1] = fa_cfg.CurrentProSets[i].I0d1.Data.LimitTime;
	//	    GrdCurrenHandle[i][j][k].GjYx = _SYSFEN_;
			GrdCurrenHandle[i].SoeId[0] = 0x000B + i*PERLINESOFTYXNUM;
			GrdCurrenHandle[i].SoeId[1] = 0x000B + i*PERLINESOFTYXNUM;
	//	    GrdCurrenHandle[i][j][k].SoeFlag = 0;
	//	    CurrenHandle[i][j][k].SoeIRF = 0;
			GrdCurrenHandle[i].SoeT01 = 1;
			GrdCurrenHandle[i].SoeT10 = 1;
//////////////////////////////////////////////////////电压接地短路

			GrdVoltHansdle[i].I = &U0[i];
			GrdVoltHansdle[i].Enable = fa_cfg.CurrentProSets[i].U0d.Data.Enable;
			GrdVoltHansdle[i].Id = fa_cfg.CurrentProSets[i].U0d.Data.LimitValue;
			GrdVoltHansdle[i].Td[0] = fa_cfg.CurrentProSets[i].U0d.Data.LimitTime;
			GrdVoltHansdle[i].Td[1] = fa_cfg.CurrentProSets[i].U0d.Data.LimitTime;
	//	    GrdVoltHansdle[i][j][k].GjYx = _SYSFEN_;
			GrdVoltHansdle[i].SoeId[0] = NULL;//0x000B + i*PERLINESOFTYXNUM;
			GrdVoltHansdle[i].SoeId[1] = NULL;//0x000B + i*PERLINESOFTYXNUM;
	//	    GrdVoltHansdle[i][j][k].SoeFlag = 0;
	//	    GrdVoltHansdle[i][j][k].SoeIRF = 0;
			GrdVoltHansdle[i].SoeT01 = 1;
			GrdVoltHansdle[i].SoeT10 = 1;


#ifdef GLFX
			GrdCurrenHandle[i].GlfxFlag = &GlfxHandle[i].LockStatu[1];
#endif

		}
		//CurrenHandle = new CurrentCheck[12];
		for (i = 0; i < MAX_LINE_NUM; i++){//	相间短路
			for ( j = 0; j < 3; j++){
					  if (j==0){
						  CurrenHandle[i][j].I = &Imax[i];
						  CurrenHandle[i][j].Enable = fa_cfg.CurrentProSets[i].Id1.Data.Enable;
						  CurrenHandle[i][j].Id =     fa_cfg.CurrentProSets[i].Id1.Data.LimitValue;
						  CurrenHandle[i][j].Td[0] =  fa_cfg.CurrentProSets[i].Id1.Data.LimitTime;
						  CurrenHandle[i][j].Td[1] =  fa_cfg.CurrentProSets[i].Id1.Data.LimitTime;
						 // CurrenHandle[i][j].GjYx = _SYSFEN_;
						  CurrenHandle[i][j].SoeId[0] = 0x0007 + i*PERLINESOFTYXNUM;
						  CurrenHandle[i][j].SoeId[1] = 0x0007 + i*PERLINESOFTYXNUM;
						 // CurrenHandle[i][j].SoeFlag = 0;
						 // CurrenHandle[i][j].SoeIRF = 0;
						  CurrenHandle[i][j].SoeT01 = 1;
						  CurrenHandle[i][j].SoeT10 = 1;
#ifdef GLFX
						  CurrenHandle[i][j].GlfxFlag = &GlfxHandle[i].LockStatu[0];
#endif
					  }else if(j==1){
						  CurrenHandle[i][j].I = &Imax[i];
						  CurrenHandle[i][j].Enable = fa_cfg.CurrentProSets[i].Id2.Data.Enable;
						  CurrenHandle[i][j].Id = fa_cfg.CurrentProSets[i].Id2.Data.LimitValue;
						  CurrenHandle[i][j].Td[0] = fa_cfg.CurrentProSets[i].Id2.Data.LimitTime;
						  CurrenHandle[i][j].Td[1] = fa_cfg.d_504B.Data.AcceTimes[0];
						//CurrenHandle[i][j].GjYx = _SYSFEN_;
						  CurrenHandle[i][j].SoeId[0] = 0x0008 + i*PERLINESOFTYXNUM;
						  CurrenHandle[i][j].SoeId[1] = 0x0008 + i*PERLINESOFTYXNUM;
						//CurrenHandle[i][j].SoeFlag = 0;
						//CurrenHandle[i][j].SoeIRF = 0;
						  CurrenHandle[i][j].SoeT01 = 1;
						  CurrenHandle[i][j].SoeT10 = 1;
#ifdef GLFX
						  CurrenHandle[i][j].GlfxFlag = &GlfxHandle[i].LockStatu[0];
#endif
					  }else if(j==2){
						  CurrenHandle[i][j].I = &Imax[i];
						  CurrenHandle[i][j].Enable = fa_cfg.CurrentProSets[i].Id3.Data.Enable;
						  CurrenHandle[i][j].Id = fa_cfg.CurrentProSets[i].Id3.Data.LimitValue;
						  CurrenHandle[i][j].Td[0] = fa_cfg.CurrentProSets[i].Id3.Data.LimitTime;
						  CurrenHandle[i][j].Td[1] = fa_cfg.d_504B.Data.AcceTimes[1];
						//  CurrenHandle[i][j].GjYx = _SYSFEN_;
						  CurrenHandle[i][j].SoeId[0] = 0x0009 + i*PERLINESOFTYXNUM;
						  CurrenHandle[i][j].SoeId[1] = 0x0009 + i*PERLINESOFTYXNUM;
						//  CurrenHandle[i][j].SoeFlag = 0;
						//  CurrenHandle[i][j].SoeIRF = 0;
						  CurrenHandle[i][j].SoeT01 = 1;
						  CurrenHandle[i][j].SoeT10 = 1;
#ifdef GLFX
						  CurrenHandle[i][j].GlfxFlag = &GlfxHandle[i].LockStatu[0];
#endif
					  }
			}
		}

			for (i = 0; i < MAX_LINE_NUM; i++){//瞬时永久故障判别
				GzCheckHandle[i].I=&Imax[i];//输入电流
				GzCheckHandle[i].Ino =fa_cfg.d_5054.Data.StartI;//无流定值
				GzCheckHandle[i].DefaultType=fa_cfg.d_5054.Data.ResetFaType;//故障类型 1瞬时故障，2:永久故障 ;3 其他；
				GzCheckHandle[i].Enable = ENABLE;//使能
				//GzCheckHandle[i]. GzYx;//告警SOE瞬时值，没有防抖前的值；此处0为故障1为恢复；
				GzCheckHandle[i].Tdefault=fa_cfg.d_5054.Data.ResetFaTime*1000;//延时复归时间
				GzCheckHandle[i].Tinstantaneour=fa_cfg.d_5054.Data.InstantFaultChkTime*1000;////瞬时故障判断时间
				GzCheckHandle[i].GzSoeId[0]=  0x000a + i*PERLINESOFTYXNUM;   //线路i相间短路故障
				GzCheckHandle[i].GzSoeId[1]=  0x000b + i*PERLINESOFTYXNUM;   //线路i单相接地故障信号
				GzCheckHandle[i].GzSoeId[2]=  0x000c + i*PERLINESOFTYXNUM;   //线路i瞬时故障信号
				GzCheckHandle[i].GzSoeId[3]=  0x000d + i*PERLINESOFTYXNUM;   //线路i永久故障信号
				GzCheckHandle[i].GzSoeId[4]=  0x000e + i*PERLINESOFTYXNUM;   //线路i零序电压故障信号
				GzCheckHandle[i].GzSoeId[5]=  0x000f + i*PERLINESOFTYXNUM;   //线路i零序电流故障信号

			}
#ifdef BHCK
		for (i = 0; i < 2; i++){
			for ( j = 0; j < 3; j++){
				for ( k = 0; k < 2; k++){
					if(k==0){
						if (j==0){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].Id1.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum =BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k]=1 ;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000c + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==1){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].Id2.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k] =1;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000D + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==2){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].Id3.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1 ;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k] =1 ;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000E + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}

					}else{
						if (j==0){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].I0d1.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k] =1;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000F + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==1){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].I0d2.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k] =1;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000F + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
							ActionHandle[i].Td = YKTIME;
							ActionHandle[i].BreakerAction = ProtectOut;
							ActionHandle[i].BreakerStatus = BreakerStatu;
						}else if (j==2){
							ActionHandle[i].ActEnable[j][k]=fa_cfg.CurrentProSets[i].I0d3.Data.EnableOut;
							ActionHandle[i].ActionType = OPENBREAKER;
							ActionHandle[i].BreakNum = BREAKER1+ i*2;
					//		ActionHandle[i].BhYx[j][k] = _SYSFEN_;
							ActionHandle[i].SoeT01[j][k] =1;//告警状态0变成1 时间定值
							ActionHandle[i].SoeT10[j][k] =1;//告警状态1变成0 时间定值
					//		ActionHandle[i].IRF = 0;
							ActionHandle[i].SoeId[j][k] = 0x000F + i*0x001a;
					//		ActionHandle[i].SoeIRF[j][k] = 0;
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
			ChzHandle[i].BreakCloseCheck = BreakCloseCheck;//重合闸检定
			ChzHandle[i].CHZtype = fa_cfg.d_504B.Data.Type;
			ChzHandle[i].ChzCn = NULL;
			ActionHandle[i].MaskEnable[0][0] = DISENABLE;//一段故障不重合
			ActionHandle[i].MaskEnable[0][1] = DISENABLE;//一段故障不重合
			ActionHandle[i].MaskEnable[1][0] = ENABLE;
			ActionHandle[i].MaskEnable[1][1] = ENABLE;
			ActionHandle[i].MaskEnable[2][0] = ENABLE;
			ActionHandle[i].MaskEnable[2][1] = ENABLE;
			ChzHandle[i].ChzEnable = fa_cfg.d_5055.Data.EnableReclose;
			ChzHandle[i].HjsEnable = fa_cfg.d_504B.Data.Enable;
			ChzHandle[i].HjsLock = ULOCK;
			ChzHandle[i].LockTime = fa_cfg.d_504B.Data.LockTime;
			ChzHandle[i].OutCheckT = YKTIME;
			ChzHandle[i].StartFlag = NULL;
			ChzHandle[i].Time[0] = fa_cfg.d_504B.Data.Time[0];
			ChzHandle[i].Time[1] = fa_cfg.d_504B.Data.Time[1];
			ChzHandle[i].Time[2] = fa_cfg.d_504B.Data.Time[2];
			ChzHandle[i].Times = fa_cfg.d_504B.Data.Times;
			ChzHandle[i].breakernum = BREAKER1+ i*2;
			ChzHandle[i].he = &scada.yx_config.config_data[HE(i)].Data;
			ChzHandle[i].fen = &scada.yx_config.config_data[FEN(i)].Data;
			ChzHandle[i].manyhstatu = 0;
		}
#endif
}
#pragma CODE_SECTION("CODEINDDR2")
void FA::FunHandleInit(void){

	union SECUNION LineNum;
	LineNum.all = fa_cfg.d_5049.Data;
	GetParaId(LineNum.bit.d1,0);
	GetParaId(LineNum.bit.d2,1);
	GetParaId(LineNum.bit.d3,2);
	GetParaId(LineNum.bit.d4,3);
	GetParaId(LineNum.bit.d5,4);
	GetParaId(LineNum.bit.d6,5);
	GetParaId(LineNum.bit.d7,6);
	GetParaId(LineNum.bit.d8,7);
	LineNum.all = fa_cfg.d_504A.Data;
	GetParaId(LineNum.bit.d1,8);
	GetParaId(LineNum.bit.d2,9);
	GetParaId(LineNum.bit.d3,10);
	GetParaId(LineNum.bit.d4,11);
	GetParaId(LineNum.bit.d5,12);
	GetParaId(LineNum.bit.d6,13);
	GetParaId(LineNum.bit.d7,14);
	GetParaId(LineNum.bit.d8,15);
	HandleInit();
}
#pragma CODE_SECTION("CODEINDDR2")
int16 FA::Init(){
	ParaInit();
	//FunHandleInit();
	HandleInit();
	return 1 ;
}
#pragma CODE_SECTION("CODEINDDR2")
int16 FA::GetParaId(int8 SecNum,int8 LineNum)
{
	Data<BaseDataType> data;
	uint32 Id;
	//uint8 SecTmp ;
	int8 Section;
	//uint16 i;
	//SecTmp = SecNum;
	Section = SecNum;
/*	if(SecTmp==0){
		ParaInit();
	}
	for ( i = 0 ; i < 8 ; i++){
		if(SecTmp==1){
			Section = i;
			break;
		}
		SecTmp/=2;
	}*/
	if(Section>=0&&Section<=7){
		Id = 0x5001+Section*9;
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
	}else{
		ParaInit();
	}
	return -1;
}

#pragma CODE_SECTION("CODEINDDR2")
int16 FA::ReadCfg(int32 DataId)
{
	Data<BaseDataType> data;
	user_data.GetData(DataId,&data);
	uint16 i;
	uint16 KeyData;
	DataType3 d_5057;   //故障信号关联出口（1~4回）
	DataType3 d_5058;   //故障信号关联出口（5~8回）
	DataType3 d_5059;   //故障信号关联出口（9~12回）
	DataType3 d_505A;   //故障信号关联出口（13~16回）
	DataType3 d_505B;   //故障信号关联出口（17~20回）
	switch(DataId)
	{
		case 0x5049:
		//DataType1  d_5049;///<线路1-8保护定值区号设置
		memcpy(&fa_cfg.d_5049,data.Data,sizeof(fa_cfg.d_5049));

		FunHandleInit();
		break;
		case 0x504A:
		//DataType1  d_504A;///<线路9-16保护定值区号设置
		memcpy(&fa_cfg.d_504A,data.Data,sizeof(fa_cfg.d_504A));

		FunHandleInit();
		break;
		case 0x504b:
		//RecloseSet d_504B;///<重合闸设置
		memcpy(&fa_cfg.d_504B,data.Data,sizeof(fa_cfg.d_504B));

		FunHandleInit();
		break;
		case 0x5096://CT变比
		memcpy(&fa_cfg.d_5096,data.Data,sizeof(fa_cfg.d_5096));
		if(fa_cfg.d_5052.Data==1){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[0]=100;
		}
		if(fa_cfg.d_5053.Data==1){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[1]=100;
		}
		break;
		case 0x504c://遮断电流 第一套
		memcpy(&fa_cfg.d_504C,data.Data,sizeof(fa_cfg.d_504C));
		if(fa_cfg.d_5052.Data==1){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[0]=100;
		}
		if(fa_cfg.d_5053.Data==1){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[1]=100;
		}
		break;
		case 0x504f://遮断电流 第二套
		memcpy(&fa_cfg.d_504F,data.Data,sizeof(fa_cfg.d_504F));
		if(fa_cfg.d_5052.Data==1){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[0]=100;
		}
		if(fa_cfg.d_5053.Data==1){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[1]=100;
		}
		break;
		case 0x5052:
		//DataType1  		d_5052;///<线路1开关定值区号设置
		memcpy(&fa_cfg.d_5052,data.Data,sizeof(fa_cfg.d_5052));
		if(fa_cfg.d_5052.Data==1){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[0]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[0]=100;
		}
		break;
		case 0x5053:
		//DataType1  		d_5053;///<线路2开关定值区号设置
		memcpy(&fa_cfg.d_5053,data.Data,sizeof(fa_cfg.d_5053));
		if(fa_cfg.d_5053.Data==1){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504C.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else if(fa_cfg.d_5052.Data==2){
			fa_cfg.NoShutOffCurr[1]= (float)(fa_cfg.d_504F.Data.NoShutOffCurr/(float)fa_cfg.d_5096.Data);
		}else{
			fa_cfg.NoShutOffCurr[1]=100;
		}
		break;


		case 0x5055:
		//FaDevotionSet d_5055;///<保护功能投退设置
		memcpy(&fa_cfg.d_5055,data.Data,sizeof(fa_cfg.d_5055));
		FunHandleInit();
		//case
		//FaCommSet	 		d_5054;///<FA功能公用参数设置
		break;
		case 0x5054:
		//FaCommSet	 		//启动电流，失压阀值，故障复归时间；
		memcpy(&fa_cfg.d_5054,data.Data,sizeof(fa_cfg.d_5054));
		FunHandleInit();
		break;
		case 0x5056:
			//uint16 i;
			//uint16 KeyData;

			memcpy(&fa_cfg.d_5056,data.Data,sizeof(fa_cfg.d_5056));
			KeyData = fa_cfg.d_5056.Data&0xffff;
			for ( i = 0; i < MAX_LINE_NUM;i++){
				GzCheckHandle[i].CheckKey((KeyData>>i)&0x0001);
			}
		break;

		case 0x5057:
			//DataType3 d_5057;   //故障信号关联出口（1~4回）
			memcpy(&d_5057,data.Data,sizeof(d_5057));
			fa_cfg.OutPort[0] = (d_5057.Data&0xff)-1;
			fa_cfg.OutPort[1] = ((d_5057.Data>>8)&0xff)-1;
			fa_cfg.OutPort[2] = ((d_5057.Data>>16)&0xff)-1;
			fa_cfg.OutPort[3] = ((d_5057.Data>>24)&0xff)-1;

		break;

		case 0x5058:
		//	DataType3 d_5058;   //故障信号关联出口（5~8回）
			memcpy(&d_5058,data.Data,sizeof(d_5058));
			fa_cfg.OutPort[4] = (d_5058.Data&0xff)-1;
			fa_cfg.OutPort[5] = ((d_5058.Data>>8)&0xff)-1;
			fa_cfg.OutPort[6] = ((d_5058.Data>>16)&0xff)-1;
			fa_cfg.OutPort[7] = ((d_5058.Data>>24)&0xff)-1;

		break;

		case 0x5059:
		//	DataType3 d_5059;   //故障信号关联出口（9~12回）
			memcpy(&d_5059,data.Data,sizeof(d_5059));
			fa_cfg.OutPort[8] = (d_5059.Data&0xff)-1;
			fa_cfg.OutPort[9] = ((d_5059.Data>>8)&0xff)-1;
			fa_cfg.OutPort[10] = ((d_5059.Data>>16)&0xff)-1;
			fa_cfg.OutPort[11] = ((d_5059.Data>>24)&0xff)-1;

		break;

		case 0x505A:
			//DataType3 d_505A;   //故障信号关联出口（13~16回）
			memcpy(&d_505A,data.Data,sizeof(d_505A));
			fa_cfg.OutPort[12] = (d_505A.Data&0xff)-1;
			fa_cfg.OutPort[13] = ((d_505A.Data>>8)&0xff)-1;
			fa_cfg.OutPort[14] = ((d_505A.Data>>16)&0xff)-1;
			fa_cfg.OutPort[15] = ((d_505A.Data>>24)&0xff)-1;

		break;
		case 0x505B:
			//DataType3 d_505B;   //故障信号关联出口（17~20回）
			memcpy(&d_505B,data.Data,sizeof(d_505B));
			fa_cfg.OutPort[16] = (d_505B.Data&0xff)-1;
			fa_cfg.OutPort[17] = ((d_505B.Data>>8)&0xff)-1;
			fa_cfg.OutPort[18] = ((d_505B.Data>>16)&0xff)-1;
			fa_cfg.OutPort[19] = ((d_505B.Data>>24)&0xff)-1;
		break;
		//DataType2 d_505C;   //故障信号关联出口功能允许
		case 0x505C:
			memcpy(&fa_cfg.d_505C,data.Data,sizeof(fa_cfg.d_505C));
		break;
		case 0x5216:
			memcpy(&fa_cfg.d_5216,data.Data,sizeof(fa_cfg.d_5216));
		break;
		default:
		break;
	}
	return 1;
}
#pragma CODE_SECTION("CODEINDDR2")
int16 FA::Run()
{
	//Init();
	//FixTimeProtect();
	return 1;
}
uint8 linenum = 2;
uint8 secnum = 3;
uint8 protype = 2;
//#pragma CODE_SECTION("CODEINL1PSRAM")
/*
*/
int16 FA::FixTimeProtect(void)
{
	uint8 i,j;
	SysMs = clk0.GetSysMs();
	PROTECTTIME = clk0.GetCP56Time2a(GETSOFTTIME);
	DataType4 *Pyc=&scada.scada_cfg.YcData.Ua1_4001;
/*	for ( i = 0; i<linenum; i++){
		Imax[i] = GetMax(JB_RMS[IA2(i)],JB_RMS[IB2(i)],JB_RMS[IC2(i)],i);
		if(Imax[i]>fa_cfg.NoShutOffCurr[i]){//电流大于遮断电流禁止闭锁断路器
			if(BreakLockState[i]==UNLOCK){
				SaveProSoe(YX_0013+i*0x1a,_SYSHE_);
			 }
			BreakLockState[i] = LOCK;
		}else	if(lp.BaseTabl[i][MDL_GETPWT].C!=TRUE){//双侧带电禁止合环
			if(BreakLockState[i]==LOCK){
				SaveProSoe(YX_0013+i*0x1a,_SYSFEN_);
			}
			BreakLockState[i] = UNLOCK;
		}
	}
*/
	if(fa_cfg.d_5055.Data.EnableCurrentProtect == DISENABLE){
		for ( i = 0; i<MAX_LINE_NUM; i++){
			for ( j = 0; j < secnum; j++){

				if(CurrenHandle[i][j].ReadSoeIRF()){
					SaveProSoe(CurrenHandle[i][j].SoeId[0],CurrenHandle[i][j].report_data);//产生软遥信
				}else if(CurrenHandle[i][j].report_data == _SYSHE_ ){
					if (CurrenHandle[i][j].SoeFlag==1){
						CurrenHandle[i][j].SoeFlag = 0;
						CurrenHandle[i][j].SoeIRF = FALSE;                                      //变位标志
					}
					CurrenHandle[i][j].report_data =_SYSFEN_;
					SaveProSoe(CurrenHandle[i][j].SoeId[0],CurrenHandle[i][j].report_data);//产生软遥信
				}

			}
			if(GrdCurrenHandle[i].ReadSoeIRF()){
				SaveProSoe(GrdCurrenHandle[i].SoeId[0],GrdCurrenHandle[i].report_data);//产生软遥信
			}else if(GrdCurrenHandle[i].report_data == _SYSHE_ ){
				if (GrdCurrenHandle[i].SoeFlag==1){
					GrdCurrenHandle[i].SoeFlag = 0;
					GrdCurrenHandle[i].SoeIRF = FALSE;                                      //变位标志
				}
				GrdCurrenHandle[i].report_data =_SYSFEN_;
				SaveProSoe(GrdCurrenHandle[i].SoeId[0],GrdCurrenHandle[i].report_data);//产生软遥信
			}
			if(GrdVoltHansdle[i].ReadSoeIRF()){

			}else if(GrdVoltHansdle[i].report_data == _SYSHE_ ){
				if (GrdVoltHansdle[i].SoeFlag==1){
					GrdVoltHansdle[i].SoeFlag = 0;
					GrdVoltHansdle[i].SoeIRF = FALSE;                                      //变位标志
				}
				GrdVoltHansdle[i].report_data =_SYSFEN_;
			}
		}
		return 1;
	}

	for (i =0;i<MAX_LINE_NUM; i++){//
		Imax[i] = Fmax(Pyc[IA1(i)].Data,Pyc[IB1(i)].Data,Pyc[IC1(i)].Data);
		//Imax[i] = GetMax(JB_RMS[IA2(i)],JB_RMS[IB2(i)],JB_RMS[IC2(i)],i);
		//if(Imax[i]<0.5){//电流小的时候用遥测互感器值；
		//	Imax[i] = GeYcMax(JB_RMS[IA1(i)],JB_RMS[IB1(i)],JB_RMS[IC1(i)],i);
		//}
		I0[i]= Pyc[I0(i)].Data; 
		U0[i]= Pyc[U0(i)].Data;
#ifdef GLFX
		//功率方向
		GlfxHandle[i].PdFun();
#endif
		GzCheckHandle[i].CheckKey(DEFAULTKEY);//故障复归按键测试。

		for (j = 0; j < secnum; j++){//

				//if(KeyStatu[i]==0){                                                   // 复归按钮
				//	CurrenHandle[i][j][k].DefaultType = DEFAULTRESET;
				//}
//电流定时限  依赖于 功率方向
				CurrenHandle[i][j].fun(SysMs,0);                                         //
				if(CurrenHandle[i][j].ReadSoeIRF()){
					SaveProSoe(CurrenHandle[i][j].SoeId[0],CurrenHandle[i][j].report_data);//产生软遥信
				}
				//故障告警功能  依赖于 电流定时限
				//瞬时/永久故障复归功能
				//GzCheckHandle[i].fun(SysMs,CurrenHandle[i][j].GjYx,PHA);
#ifdef BHCK
//保护跳闸出口功能，依赖于电流定时限
				ActionHandle[i].fun(SysMs,j,k,CurrenHandle[i][j][k].GjYx);
				if(ActionHandle[i].ReadSoeIRF(j,k)){
					if(ActionHandle[i].report_data[j][k]==_SYSHE_){
						SaveProSoe(ActionHandle[i].SoeId[j][k],ActionHandle[i].report_data[j][k]);//产生保护遥信；
					}
				}
				gzstatu[i] = ActionHandle[i].state;
				if(gzstatu[i]==OUTCKOK){
					//SaveProSoe(ActionHandle[i].SoeId[j][k],ActionHandle[i].report_data[j][k]);        //跳闸成功SOE
				}else if(gzstatu[i]==OUTCKFAIL){
					//SaveProSoe(ActionHandle[i]->SoeId[j+k],ActionHandle[i]->report_data[j+k]);	    //跳闸失败SOE
				}

//重新合闸功能，依赖于保护跳闸出口
				chzstatu[i] = ChzHandle[i].Fun(SysMs,gzstatu[i],ActionHandle[i].ChzQd);
				HjsFlag = 0;
				if(chzstatu[i] ==HJSTIME){
					HjsFlag = 1;
				}else if (chzstatu[i] ==CHZCKOK){
					SaveProSoe(0x001a+i*0x1a,_SYSHE_); //重合闸成功
					HjsFlag = 1;
				}else if (chzstatu[i] ==CHZCKFAIL){
					SaveProSoe(0x001a+i*0x1a,_SYSFEN_);//重合闸失败
				}else if (chzstatu[i] ==CHZOUT){
					SaveProSoe(0x0019+i*0x1a,_SYSHE_); //启动重合闸
				}
#endif
		}
		if(fa_cfg.d_5054.Data.GroundLinkType==BIGCURRENT){
			GrdCurrenHandle[i].fun(SysMs,0);                                         //
			if(GrdCurrenHandle[i].ReadSoeIRF()){
				SaveProSoe(GrdCurrenHandle[i].SoeId[0],GrdCurrenHandle[i].report_data);//产生软遥信
			}
			GrdVoltHansdle[i].GjYx = _SYSFEN_;
		}else{
			GrdVoltHansdle[i].fun(SysMs,0);
			GrdCurrenHandle[i].GjYx = _SYSFEN_;
		}

		//故障告警功能  依赖于 电流定时限
		//瞬时/永久故障复归功能
		GzCheckHandle[i].fun(SysMs,CurrenHandle[i][0].GjYx,
				CurrenHandle[i][1].GjYx,
				CurrenHandle[i][2].GjYx,
				GrdCurrenHandle[i].GjYx,
				GrdVoltHansdle[i].GjYx);

	}
	//线路故障灯
	uint16 gzflag = 0;
	for(i = 0 ;i < MAX_LINE_NUM; i++){
		if(GzCheckHandle[i].GzStatu !=DEFAULTRESET){//有故障
			gzflag++;
			if( strcmp((char*)&fa_cfg.d_5216.Data.pass,"D30")!=0){//不是三摇型，出口继电器点灯
				if(CHKBIT(fa_cfg.d_505C.Data,(0x00000001<<i))){
					RelayOut(fa_cfg.OutPort[i],SET);
				}
			}

		}else{
			if(strcmp((char*)&fa_cfg.d_5216.Data.pass,"D30")!=0){
				if(CHKBIT(fa_cfg.d_505C.Data,(0x00000001<<i))){
					RelayOut(fa_cfg.OutPort[i],CLR);
				}
			}
		}
		if(gzflag!=0){           //线路有故障，点灯
			LINEALARM_1;
		}else{                   //线路无关灯，关灯
			LINEALARM_0;
		}
	}
	// strcmp((char*)&fa_cfg.d_5216.Data.pass,"D30");
	return 1;
}

