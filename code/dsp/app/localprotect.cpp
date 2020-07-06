/*
 * localprotect.cpp
 *
 *  Created on: 2013-11-27
 *      Author: Administrator
 */
#include "localprotect.h"
#include <string.h>
#include "scada.h"
typedef enum MODNUM{
	MDL_BREAKFEN=0,//断路器分状态监测模块
	MDL_BREAKHE,   //断路器合状态监测模块
	MDL_VLEFTLOSE, //左侧失压监测模块
	MDL_VRIGHTLOSE,//右侧失压监测模块
	MDL_VLEFTGET,  //左侧得电监测模块
	MDL_VRIGHTGET, //右侧得电监测模块
	MDL_Vlose,//失电状态
	MDL_Vget,//得电状态
	MDL_VLEFTRES,  //左侧残压监测模块
	MDL_VRIGHTRES, //右侧残压监测模块
	MDL_IGZ,    //10   //故障电流监测模块
	MDL_VLOSE,     //失压监测模块
	MDL_ILOSE,     //无流监控模块
	MDL_U0,         //U0监测模块
	MDL_LSPWENB,//  EnableOpenSwitch;///<双侧失电延时分闸功能压板   1
    MDL_BIGTPWENB,//  EnableCloseSwitch;///<单侧得电延时合闸功能压板  2
    MDL_LCCLSENB, // EnableLockCloseSwitch;///<闭锁合闸功能压板      3
    MDL_CKIENB,//  EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压) 4
    MDL_LKOPENB,//  EnableLockOpenSwitch;///<无故障闭锁分闸功能压板 5
    MDL_VRESCKENB, // EnableChkRemainVol;///<残压检测功能允许 6
    MDL_U0CKENB,////20  EnableU0;///<零序电压保护投入定值  7
    MDL_BILSENB, //EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许 8
    MDL_GETPWENB,//  EnableLockCloseSwitch;///<双侧得电闭锁开关合闸功能允许 9
	MDL_TZLOCKSTATU, //openlock;//闭锁跳闸状态；
	MDL_HZLOCKSTATU,// closlock;//闭锁合闸状态；
	MDL_LSPWBSENB,// LsPwEnb;//失电跳闸闭锁检测；
	MDL_BGTPWBSENB,// BiGtPwEnb;//单侧得电合闸闭锁监测；
	MDL_U0BSENB,// U0Enb;//U0保护跳闸闭锁监测；
	MDL_IUBSENB,// IUEnb;// 和故障线路跳闸闭锁监测；
	MDL_BLSPWBSENB,// BiLsPwEnb;//失电合闸闭锁监测；//27
	MDL_U0BREAKERCLOSE,//30  //U0电压保护功能  断路器合闸出口监测+时限；<零序电压保护时间定值 LimitU0Time
	MDL_BSHZBREAKERCLOSE,//闭锁合闸功能       断路器合闸出口监测+时限；闭锁合闸时间LockCloseSwitchTime（Y时限）或者 无故障闭锁分闸时限LockOpenSwitchTime
	MDL_BSFZBREAKERCLOSE, //闭锁分闸功能    断路器合闸出口监测+闭锁复归时限；LockOpenSwitchResetTime
    MDL_LSPWT,// LosePowerTime;///<双侧失电延时时间（T时限）
    MDL_BIGTPWT,// int32 GetPowerTime;///<单侧得电延时时间（X时限）
    MDL_VRESCKTEND, // <残压检测功能  没有定值，采用默认值；
    MDL_BILST, //CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
    MDL_GETPWT,//37// LockCloseSwitchTime;///<双侧得电闭锁开关合闸时间
    LSPW_MDL1,
    LSPW_MDL2,
    LSPW_MDL3,//40
    LSPW_MDL4,
    LSPW_MDL5,
    LSPW_MDLEND,
    BIGTPW_MDL1,
    BIGTPW_MDL2,
    BIGTPW_MDL3,
    BIGTPW_MDL4,
    BIGTPW_MDL5,
    BIGTPW_MDL6,
    BIGTPW_MDLEND,//50
    U0_MDL1,
    U0_MDL2,
    U0_MDL3,
    U0_MDL4,
    U0_MDL5,
    U0_MDLEND,
    Ures_MDL1,
    Ures_MDL2,
    Ures_MDL3,
    LCHZ_MDL1,//60
    LCHZ_MDL2,
    LCHZ_MDL3,
    LCHZ_MDL4,
    LCHZ_MDL5,
    LCHZ_MDL6,
    LCHZ_MDL7,
    LCHZ_MDLEND,
    LCFZ_MDL1,
    LCFZ_MDL2,
    LCFZ_MDLEND,//70
    BILSPW_MDL1,
    BILSPW_MDL2,
    BILSPW_MDL3,
    BILSPW_MDL4,
    BILSPW_MDL5,
    BILSPW_MDL6,
    BILSPW_MDLEND,
    POWERON_MDL1,
    POWERON_MDL2,
    POWERON_MDL3,//80
    POWERON_MDLEND
}MODNUM_E;
ConterSleepTAB ConterSleepTab ;
ConterRuningTAB ConterRuningTab ;
ConterReachedTAB ConterReachedTab ;
StopConterTAB StopConterTab ;
StartConterTAB StartConterTab ;
DelayStartDelayTAB DelayStartDelayTab ;
PulseTAB PulseTab ;
DelayStartTAB DelayStartTab ;
ANALOGSTEADYTAB AnalogsteadyTab ;
COMPARETAB CompareTab ;
LOWANALOGTAB LowanalogTab ;
HIGHANALOGTAB HighanalogTab ;
STEADYTAB SteadyTab ;
XNORTAB XnorTab ;
XORTAB XorTab ;
ORTAB OrTab ;
NANDTAB NandTab ;
ANDTAB AndTab ;
NOTTAB NotTab ;
EQUALTAB EqualTab ;

#pragma DATA_SECTION("localprotectData")
LocalProtect lp;
uint16 lpnum = BILSPW_MDLEND;
uint16 lpst = 0;
uint16 checkmd = 0;
int8 he = 0;
int8 fen = 0;
float UL = 0;
float UR = 0;
float IMAX = 0;
float UMAX = 0;
float Uzero = 0;
void LocalProtect::fun(void){
	uint8 i;
	uint8 LINE;
	HeStatu[0] = he;// scada.scada_cfg.YxNature[HE1].Data;
	HeStatu[1] = he;// scada.scada_cfg.YxNature[HE2].Data;
	FenStatu[0] = fen;// scada.scada_cfg.YxNature[FEN1].Data;
	FenStatu[1] = fen;// scada.scada_cfg.YxNature[FEN2].Data;
	//heSta[0]= HeStatu[0] ;
	//heSta[1]= HeStatu[1] ;
	//fenSta[0] = FenStatu[0];
	//fenSta[1] = FenStatu[1];
	SystermMs = clk0.GetSysMs();
	Uleft[0] = UL;//scada.scada_cfg.YcData.ua1_4001.Data;
	Uright[0] = UR;//scada.scada_cfg.YcData.uc1_4003.Data;
	Imax[0] = IMAX;//GetMax(scada.scada_cfg.YcData.ia11_4007.Data,scada.scada_cfg.YcData.ib11_4008.Data,scada.scada_cfg.YcData.ic11_4009.Data);
	Umax[0] = UMAX;//GetMax(scada.scada_cfg.YcData.ua1_4001.Data,scada.scada_cfg.YcData.ub1_4002.Data,scada.scada_cfg.YcData.uc1_4003.Data);
	U0[0] = Uzero;//scada.scada_cfg.YcData.u011_4006.Data;
	for (LINE = 0 ;LINE<MAX_LINE_NUM;LINE++){
		for(i = 0; i <=POWERON_MDLEND; i++){//公共模块运行
			if(BaseTabl[LINE][i].HostHandle!=NULL){
				BaseTabl[LINE][i].fun();
				//printf("LINE:%d\n",LINE);
			}
		}

		if(BaseTabl[LINE][LSPW_MDLEND].C==1){//失电跳闸功能
			ProtectOut(LINE,OPENBREAKER);
		}
		if(BaseTabl[LINE][BIGTPW_MDLEND].C==1){//单侧得电合闸功能
			ProtectOut(LINE,CLOSEBREAKER);
		}
		if(BaseTabl[LINE][U0_MDLEND].C==1){//零序电压跳闸功能
			ProtectOut(LINE,OPENBREAKER);
		}
		if(BaseTabl[LINE][MDL_VRESCKTEND].C==1){//残压监测功能  闭锁合闸
			LockStatu[LINE].closlock = LOCK;
		}else{

		}
		if(BaseTabl[LINE][LCHZ_MDLEND].C==1){//合闸后监测到故障（电流或者失压）跳闸并 闭锁合闸功能
			ProtectOut(LINE,OPENBREAKER);
			LockStatu[LINE].closlock = LOCK;
		}
		if(BaseTabl[LINE][LCFZ_MDLEND].C==1){//闭锁分闸/复归功能
			LockStatu[LINE].openlock = LOCK;
		}else{
			LockStatu[LINE].openlock = LOCK;
		}
		if(BaseTabl[LINE][BILSPW_MDLEND].C==1){//单侧失电合闸功能
			ProtectOut(LINE,CLOSEBREAKER);
		}
		if(BaseTabl[LINE][POWERON_MDLEND].C==1){//双侧带电禁止合环
			LockStatu[LINE].closlock = LOCK;
		}else{
			LockStatu[LINE].closlock = UNLOCK;
		}
	}
}
#pragma CODE_SECTION("localprotectInDDR2")
void LocalProtect::ParaInit(void)
{

	//
	lp_cfg.dz[0].d_504C.Data.NoShutOffCurr = 600;//非遮断电流
	//
	lp_cfg.dz[0].d_504D.Data.LosePowerTime = 3000;///<双侧失电延时时间（T时限）
	lp_cfg.dz[0].d_504D.Data.EnableOpenSwitch = 1;///<双侧失电延时分闸功能压板   1
	lp_cfg.dz[0].d_504D.Data.GetPowerTime = 3000;///<单侧得电延时时间（X时限）
	lp_cfg.dz[0].d_504D.Data.EnableCloseSwitch = 1;///<单侧得电延时合闸功能压板  2
	lp_cfg.dz[0].d_504D.Data.LockCloseSwitchTime = 3000;///<闭锁合闸时间（Y时限）
	lp_cfg.dz[0].d_504D.Data.EnableLockCloseSwitch =1;///<闭锁合闸功能压板      3
	lp_cfg.dz[0].d_504D.Data.FaultCurrent = 6.0;///<闭锁合闸故障电流定值
	lp_cfg.dz[0].d_504D.Data.EnableChkFaultCurrent = 0;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压) 4
	lp_cfg.dz[0].d_504D.Data.LockOpenSwitchTime = 3000;///<无故障闭锁分闸时限
	lp_cfg.dz[0].d_504D.Data.LockOpenSwitchResetTime = 5;///<无故障闭锁分闸复归时限
	lp_cfg.dz[0].d_504D.Data.EnableLockOpenSwitch =1;///<无故障闭锁分闸功能压板 5
	lp_cfg.dz[0].d_504D.Data.RemainVol = 30;///<残压定值
	lp_cfg.dz[0].d_504D.Data.EnableChkRemainVol = 1;///<残压检测功能允许 6
	lp_cfg.dz[0].d_504D.Data.LimitU0 = 50;///<零序电压保护定值U0
	lp_cfg.dz[0].d_504D.Data.LimitU0Time = 3000;///<零序电压保护时间定值
	lp_cfg.dz[0].d_504D.Data.EnableU0 = 1;///<零序电压保护投入定值  7
	/////
	lp_cfg.dz[0].d_504E.Data.CloseSwitchOneSideTime = 3000;///<单侧失压延时后合闸设定时间（XL时限）
	lp_cfg.dz[0].d_504E.Data.EnableCloseSwitchOneSide = 1;///<单侧失压延时后合闸功能允许
	lp_cfg.dz[0].d_504E.Data.LockCloseSwitchTime = 3000;///<双侧得电闭锁开关合闸时间
    lp_cfg.dz[0].d_504E.Data.EnableLockCloseSwitch = 1;///<双侧得电闭锁开关合闸功能允许

	lp_cfg.d_5052.Data = 0;///<线路1开关定值区号设置
	lp_cfg.d_5053.Data = 0;///<线路2开关定值区号设置
	///<FA功能公用参数设置
	lp_cfg.d_5054.Data.StartI = 0.5;//
	lp_cfg.d_5054.Data.StartU = 10.0;
//	lp_cfg.d_5054.Data.ResetFaTime;
}
void LocalProtect::Paracpy(void)
{
	//
	usr_cfg.BrkPara[0].NoShutOffCurr = lp_cfg.dz[0].d_504C.Data.NoShutOffCurr;
	//
	usr_cfg.BrkPara[0].LosePowerTime = lp_cfg.dz[0].d_504D.Data.LosePowerTime;///<双侧失电延时时间（T时限）
	usr_cfg.BrkPara[0].EnableOpenSwitch = lp_cfg.dz[0].d_504D.Data.EnableOpenSwitch;///<双侧失电延时分闸功能压板   1
	usr_cfg.BrkPara[0].GetPowerTime = lp_cfg.dz[0].d_504D.Data.GetPowerTime ;///<单侧得电延时时间（X时限）
	usr_cfg.BrkPara[0].EnableCloseSwitch =lp_cfg.dz[0].d_504D.Data.EnableCloseSwitch;///<单侧得电延时合闸功能压板  2
	usr_cfg.BrkPara[0].LockCloseSwitchTime1 = lp_cfg.dz[0].d_504D.Data.LockCloseSwitchTime;///<闭锁合闸时间（Y时限）
	usr_cfg.BrkPara[0].EnableLockCloseSwitch1 =lp_cfg.dz[0].d_504D.Data.EnableLockCloseSwitch;///<闭锁合闸功能压板      3
	usr_cfg.BrkPara[0].FaultCurrent = lp_cfg.dz[0].d_504D.Data.FaultCurrent;///<闭锁合闸故障电流定值
	usr_cfg.BrkPara[0].EnableChkFaultCurrent =lp_cfg.dz[0].d_504D.Data.EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压) 4
	usr_cfg.BrkPara[0].LockOpenSwitchTime = lp_cfg.dz[0].d_504D.Data.LockOpenSwitchTime;///<无故障闭锁分闸时限
	usr_cfg.BrkPara[0].LockOpenSwitchResetTime = lp_cfg.dz[0].d_504D.Data.LockOpenSwitchResetTime*1000;///<无故障闭锁分闸复归时限
	usr_cfg.BrkPara[0].EnableLockOpenSwitch =lp_cfg.dz[0].d_504D.Data.EnableLockOpenSwitch;///<无故障闭锁分闸功能压板 5
	usr_cfg.BrkPara[0].RemainVol = lp_cfg.dz[0].d_504D.Data.RemainVol ;///<残压定值
	usr_cfg.BrkPara[0].EnableChkRemainVol = lp_cfg.dz[0].d_504D.Data.EnableChkRemainVol;///<残压检测功能允许 6
	usr_cfg.BrkPara[0].LimitU0 = lp_cfg.dz[0].d_504D.Data.LimitU0;///<零序电压保护定值U0
	usr_cfg.BrkPara[0].LimitU0Time = lp_cfg.dz[0].d_504D.Data.LimitU0Time;///<零序电压保护时间定值
	usr_cfg.BrkPara[0].EnableU0 = lp_cfg.dz[0].d_504D.Data.EnableU0 ;///<零序电压保护投入定值  7
	/////
	usr_cfg.BrkPara[0].CloseSwitchOneSideTime = lp_cfg.dz[0].d_504E.Data.CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
	usr_cfg.BrkPara[0].EnableCloseSwitchOneSide = lp_cfg.dz[0].d_504E.Data.EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许
	usr_cfg.BrkPara[0].LockCloseSwitchTime2 = lp_cfg.dz[0].d_504E.Data.LockCloseSwitchTime ;///<双侧得电闭锁开关合闸时间
	usr_cfg.BrkPara[0].EnableLockCloseSwitch2 = lp_cfg.dz[0].d_504E.Data.EnableLockCloseSwitch ;///<双侧得电闭锁开关合闸功能允许

	usr_cfg.secnum1 =lp_cfg.d_5052.Data;///<线路1开关定值区号设置
	usr_cfg.secnum2 = lp_cfg.d_5053.Data;///<线路2开关定值区号设置
	///<FA功能公用参数设置
	usr_cfg.StartI = lp_cfg.d_5054.Data.StartI;//
	usr_cfg.StartU = lp_cfg.d_5054.Data.StartU;
	usr_cfg.ResetFaTime =lp_cfg.d_5054.Data.ResetFaTime;
}
#pragma CODE_SECTION("localprotectInDDR2")
void LocalProtect::BaseMdInit(void)
{
	uint8 i;
	uint8 LINE;
	memset(&BaseTabl,0,sizeof(BaseTabl));
	for(LINE = 0;LINE<MAX_LINE_NUM;LINE++){
			memset(&BaseMd,0,sizeof(BaseMd));
			//MDL_BREAKFEN=0,//断路器分状态监测模块
			BaseMd[MDL_BREAKFEN].Local.Num = MDL_BREAKFEN;
			BaseMd[MDL_BREAKFEN].Local.Name = D_EQUAL;
			BaseMd[MDL_BREAKFEN].Local.InputPara.Int8_Reg.pA = &FenStatu[LINE];
			BaseMd[MDL_BREAKFEN].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_BREAKHE,   //断路器合状态监测模块
			BaseMd[MDL_BREAKHE].Local.Num = MDL_BREAKHE;
			BaseMd[MDL_BREAKHE].Local.Name = D_EQUAL;
			BaseMd[MDL_BREAKHE].Local.InputPara.Int8_Reg.pA = &HeStatu[LINE];
			BaseMd[MDL_BREAKHE].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_VLEFTLOSE, //左侧失压监测模块
			BaseMd[MDL_VLEFTLOSE].Local.Num = MDL_VLEFTLOSE;
			BaseMd[MDL_VLEFTLOSE].Local.Name = D_LOWANALOG;
			BaseMd[MDL_VLEFTLOSE].Local.InputPara.HighLow_Reg.pA = &Uleft[LINE];
			BaseMd[MDL_VLEFTLOSE].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartU;
			//MDL_VRIGHTLOSE,//右侧失压监测模块
			BaseMd[MDL_VRIGHTLOSE].Local.Num = MDL_VRIGHTLOSE;
			BaseMd[MDL_VRIGHTLOSE].Local.Name = D_LOWANALOG;
			BaseMd[MDL_VRIGHTLOSE].Local.InputPara.HighLow_Reg.pA = &Uright[LINE];
			BaseMd[MDL_VRIGHTLOSE].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartU;
			//MDL_VLEFTGET,  //左侧得电监测模块
			BaseMd[MDL_VLEFTGET].Local.Num = MDL_VLEFTGET;
			BaseMd[MDL_VLEFTGET].Local.Name = D_HIGHANALOG;
			BaseMd[MDL_VLEFTGET].Local.InputPara.HighLow_Reg.pA = &Uleft[LINE];
			BaseMd[MDL_VLEFTGET].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartU;
			//MDL_VRIGHTGET, //右侧得电监测模块
			BaseMd[MDL_VRIGHTGET].Local.Num = MDL_VRIGHTGET;
			BaseMd[MDL_VRIGHTGET].Local.Name = D_HIGHANALOG;
			BaseMd[MDL_VRIGHTGET].Local.InputPara.HighLow_Reg.pA = &Uright[LINE];
			BaseMd[MDL_VRIGHTGET].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartU;
			//MDL_Vlose,//失电状态
			BaseMd[MDL_Vlose].Local.Num = MDL_Vlose;
			BaseMd[MDL_Vlose].Local.Name = D_ANALOGSTEADY;
			BaseMd[MDL_Vlose].Local.InputPara.Region_Reg.pA = &Uleft[LINE];
			BaseMd[MDL_Vlose].Local.InputPara.Region_Reg.pB = &Uright[LINE];
			BaseMd[MDL_Vlose].Local.InputPara.Region_Reg.Dz= usr_cfg.StartU;
			BaseMd[MDL_Vlose].Local.InputPara.Region_Reg.dir= FALSE;
			//MDL_Vget,//得电状态
			BaseMd[MDL_Vget].Local.Num = MDL_Vget;
			BaseMd[MDL_Vget].Local.Name = D_ANALOGSTEADY;
			BaseMd[MDL_Vget].Local.InputPara.Region_Reg.pA = &Uleft[LINE];
			BaseMd[MDL_Vget].Local.InputPara.Region_Reg.pB = &Uright[LINE];
			BaseMd[MDL_Vget].Local.InputPara.Region_Reg.Dz= usr_cfg.StartU;
			BaseMd[MDL_Vget].Local.InputPara.Region_Reg.dir= TRUE;
			//MDL_VLEFTRES,  //左侧残压监测模块
			BaseMd[MDL_VLEFTRES].Local.Num = MDL_VLEFTRES;
			BaseMd[MDL_VLEFTRES].Local.Name = D_COMPARE;
			BaseMd[MDL_VLEFTRES].Local.InputPara.Region_Reg.pA = &Uleft[LINE];
			BaseMd[MDL_VLEFTRES].Local.InputPara.Region_Reg.Dz= usr_cfg.BrkPara[LINE].RemainVol;
			BaseMd[MDL_VLEFTRES].Local.InputPara.Region_Reg.Region= 5.0;
			//MDL_VRIGHTRES, //右侧残压监测模块
			BaseMd[MDL_VRIGHTRES].Local.Num = MDL_VRIGHTRES;
			BaseMd[MDL_VRIGHTRES].Local.Name = D_COMPARE;
			BaseMd[MDL_VRIGHTRES].Local.InputPara.Region_Reg.pA = &Uright[LINE];
			BaseMd[MDL_VRIGHTRES].Local.InputPara.Region_Reg.Dz= usr_cfg.BrkPara[LINE].RemainVol;
			BaseMd[MDL_VRIGHTRES].Local.InputPara.Region_Reg.Region= 5.0;
			//MDL_IGZ,       //故障电流监测模块
			BaseMd[MDL_IGZ].Local.Num = MDL_IGZ;
			BaseMd[MDL_IGZ].Local.Name = D_HIGHANALOG;
			BaseMd[MDL_IGZ].Local.InputPara.HighLow_Reg.pA = &Imax[LINE];
			BaseMd[MDL_IGZ].Local.InputPara.HighLow_Reg.pB = &usr_cfg.BrkPara[LINE].FaultCurrent;//故障电流
			//MDL_VLOSE,     //失压监测模块
			BaseMd[MDL_VLOSE].Local.Num = MDL_VLOSE;
			BaseMd[MDL_VLOSE].Local.Name = D_LOWANALOG;
			BaseMd[MDL_VLOSE].Local.InputPara.HighLow_Reg.pA = &Umax[LINE];
			BaseMd[MDL_VLOSE].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartU;
			//MDL_ILOSE,     //无流监控模块
			BaseMd[MDL_ILOSE].Local.Num = MDL_ILOSE;
			BaseMd[MDL_ILOSE].Local.Name = D_LOWANALOG;
			BaseMd[MDL_ILOSE].Local.InputPara.HighLow_Reg.pA = &Imax[LINE];//无流
			BaseMd[MDL_ILOSE].Local.InputPara.HighLow_Reg.pB = &usr_cfg.StartI;
			//MDL_U0         //U0监测模块
			BaseMd[MDL_U0].Local.Num = MDL_U0;
			BaseMd[MDL_U0].Local.Name = D_HIGHANALOG;
			BaseMd[MDL_U0].Local.InputPara.HighLow_Reg.pA = &U0[LINE];
			BaseMd[MDL_U0].Local.InputPara.HighLow_Reg.pB = &usr_cfg.BrkPara[LINE].LimitU0;

			//MDL_LSPWENB,//  EnableOpenSwitch;///<双侧失电延时分闸功能压板   1
			BaseMd[MDL_LSPWENB].Local.Num = MDL_LSPWENB;
			BaseMd[MDL_LSPWENB].Local.Name = D_EQUAL;
			BaseMd[MDL_LSPWENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableOpenSwitch;
			BaseMd[MDL_LSPWENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_BIGTPWENB,//  EnableCloseSwitch;///<单侧得电延时合闸功能压板  2
			BaseMd[MDL_BIGTPWENB].Local.Num = MDL_BIGTPWENB;
			BaseMd[MDL_BIGTPWENB].Local.Name = D_EQUAL;
			BaseMd[MDL_BIGTPWENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableCloseSwitch;
			BaseMd[MDL_BIGTPWENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_LCCLSENB, // EnableLockCloseSwitch;///<闭锁合闸功能压板      3
			BaseMd[MDL_LCCLSENB].Local.Num = MDL_LCCLSENB;
			BaseMd[MDL_LCCLSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_LCCLSENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableLockCloseSwitch1;
			BaseMd[MDL_LCCLSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_CKIENB,//  EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压) 4
			BaseMd[MDL_CKIENB].Local.Num = MDL_CKIENB;
			BaseMd[MDL_CKIENB].Local.Name = D_EQUAL;
			BaseMd[MDL_CKIENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableChkFaultCurrent;
			BaseMd[MDL_CKIENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_LKOPENB,//  EnableLockOpenSwitch;///<无故障闭锁分闸功能压板 5
			BaseMd[MDL_LKOPENB].Local.Num = MDL_LKOPENB;
			BaseMd[MDL_LKOPENB].Local.Name = D_EQUAL;
			BaseMd[MDL_LKOPENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableLockOpenSwitch;
			BaseMd[MDL_LKOPENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_VRESCKENB, // EnableChkRemainVol;///<残压检测功能允许 6
			BaseMd[MDL_VRESCKENB].Local.Num = MDL_VRESCKENB;
			BaseMd[MDL_VRESCKENB].Local.Name = D_EQUAL;
			BaseMd[MDL_VRESCKENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableChkRemainVol;
			BaseMd[MDL_VRESCKENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_U0CKENB,//  EnableU0;///<零序电压保护投入定值  7
			BaseMd[MDL_U0CKENB].Local.Num = MDL_U0CKENB;
			BaseMd[MDL_U0CKENB].Local.Name = D_EQUAL;
			BaseMd[MDL_U0CKENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableU0;
			BaseMd[MDL_U0CKENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_BILSENB, //EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许 8
			BaseMd[MDL_BILSENB].Local.Num = MDL_BILSENB;
			BaseMd[MDL_BILSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_BILSENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableCloseSwitchOneSide;
			BaseMd[MDL_BILSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_GETPWENB//  EnableLockCloseSwitch;///<双侧得电闭锁开关合闸功能允许 9
			BaseMd[MDL_GETPWENB].Local.Num = MDL_GETPWENB;
			BaseMd[MDL_GETPWENB].Local.Name = D_EQUAL;
			BaseMd[MDL_GETPWENB].Local.InputPara.Int8_Reg.pA = &usr_cfg.BrkPara[LINE].EnableLockCloseSwitch2;
			BaseMd[MDL_GETPWENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_TZLOCKSTATU, //openlock;//闭锁跳闸状态；
			BaseMd[MDL_TZLOCKSTATU].Local.Num = MDL_TZLOCKSTATU;
			BaseMd[MDL_TZLOCKSTATU].Local.Name = D_NOT;
			BaseMd[MDL_TZLOCKSTATU].Local.InputPara.Base_Reg.pA = (bool*)&LockStatu[LINE].openlock;
			BaseMd[MDL_TZLOCKSTATU].Local.InputPara.Base_Reg.pB = NULL;
			//MDL_HZLOCKSTATU,// closlock;//闭锁合闸状态；
			BaseMd[MDL_HZLOCKSTATU].Local.Num = MDL_HZLOCKSTATU;
			BaseMd[MDL_HZLOCKSTATU].Local.Name = D_NOT;
			BaseMd[MDL_HZLOCKSTATU].Local.InputPara.Base_Reg.pA = (bool*)&LockStatu[LINE].closlock;
			BaseMd[MDL_HZLOCKSTATU].Local.InputPara.Base_Reg.pB = NULL;
			//MDL_LSPWBSENB,// LsPwEnb;//失电跳闸闭锁检测；
			BaseMd[MDL_LSPWBSENB].Local.Num = MDL_LSPWBSENB;
			BaseMd[MDL_LSPWBSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_LSPWBSENB].Local.InputPara.Int8_Reg.pA =  &LockStatu[LINE].LsPwEnb;
			BaseMd[MDL_LSPWBSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_BGTPWBSENB,// BiGtPwEnb;//单侧得电合闸闭锁监测；
			BaseMd[MDL_BGTPWBSENB].Local.Num = MDL_BGTPWBSENB;
			BaseMd[MDL_BGTPWBSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_BGTPWBSENB].Local.InputPara.Int8_Reg.pA =  &LockStatu[LINE].BiGtPwEnb;
			BaseMd[MDL_BGTPWBSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_U0BSENB,// U0Enb;//U0保护跳闸闭锁监测；
			BaseMd[MDL_U0BSENB].Local.Num = MDL_U0BSENB;
			BaseMd[MDL_U0BSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_U0BSENB].Local.InputPara.Int8_Reg.pA =  &LockStatu[LINE].U0Enb;
			BaseMd[MDL_U0BSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_IUBSENB,// IUEnb;// 和故障线路跳闸闭锁监测；
			BaseMd[MDL_IUBSENB].Local.Num = MDL_IUBSENB;
			BaseMd[MDL_IUBSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_IUBSENB].Local.InputPara.Int8_Reg.pA =  &LockStatu[LINE].IUEnb;
			BaseMd[MDL_IUBSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_BLSPWBSENB// BiLsPwEnb;//失电合闸闭锁监测；
			BaseMd[MDL_BLSPWBSENB].Local.Num = MDL_BLSPWBSENB;
			BaseMd[MDL_BLSPWBSENB].Local.Name = D_EQUAL;
			BaseMd[MDL_BLSPWBSENB].Local.InputPara.Int8_Reg.pA =  &LockStatu[LINE].BiLsPwEnb;
			BaseMd[MDL_BLSPWBSENB].Local.InputPara.Int8_Reg.pB = NULL;
			//MDL_U0BREAKERCLOSE,  //U0电压保护断路器合闸出口监测；
			BaseMd[MDL_U0BREAKERCLOSE].Local.Num = MDL_U0BREAKERCLOSE;
			BaseMd[MDL_U0BREAKERCLOSE].Local.Name = D_Pulse;
			BaseMd[MDL_U0BREAKERCLOSE].Local.InputPara.Trigger_Reg.PA =  (bool*)&HeStatu[LINE];
			BaseMd[MDL_U0BREAKERCLOSE].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_U0BREAKERCLOSE].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LimitU0Time;
			//MDL_BSHZBREAKERCLOSE,//闭锁合闸 断路器合闸出口监测；
			BaseMd[MDL_BSHZBREAKERCLOSE].Local.Num = MDL_BSHZBREAKERCLOSE;
			BaseMd[MDL_BSHZBREAKERCLOSE].Local.Name = D_Pulse;
			BaseMd[MDL_BSHZBREAKERCLOSE].Local.InputPara.Trigger_Reg.PA =  (bool*)&HeStatu[LINE];
			BaseMd[MDL_BSHZBREAKERCLOSE].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_BSHZBREAKERCLOSE].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LockCloseSwitchTime1;
			//MDL_BSFZBREAKERCLOSE //闭锁分闸 断路器合闸出口监测；
			BaseMd[MDL_BSFZBREAKERCLOSE].Local.Num = MDL_BSFZBREAKERCLOSE;
			BaseMd[MDL_BSFZBREAKERCLOSE].Local.Name = D_Pulse;
			BaseMd[MDL_BSFZBREAKERCLOSE].Local.InputPara.Trigger_Reg.PA =  (bool*)&HeStatu[LINE];
			BaseMd[MDL_BSFZBREAKERCLOSE].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_BSFZBREAKERCLOSE].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LockOpenSwitchResetTime;

			//MDL_LSPWT,// LosePowerTime;///<双侧失电延时时间（T时限）
			BaseMd[MDL_LSPWT].Local.Num = MDL_LSPWT;
			BaseMd[MDL_LSPWT].Local.Name = D_DelayStart;
			BaseMd[MDL_LSPWT].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[MDL_LSPWT].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_LSPWT].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LosePowerTime;
			BaseMd[MDL_LSPWT].NodeNumber = NULL;// 此处还无法确认本定时器的位置，注意在下面要确定；
		   // MDL_BIGTPWT,// int32 GetPowerTime;///<单侧得电延时时间（X时限）
			BaseMd[MDL_BIGTPWT].Local.Num = MDL_BIGTPWT;
			BaseMd[MDL_BIGTPWT].Local.Name = D_DelayStart;
			BaseMd[MDL_BIGTPWT].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[MDL_BIGTPWT].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_BIGTPWT].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].GetPowerTime;
			BaseMd[MDL_BIGTPWT].NodeNumber = NULL;// 此处还无法确认本定时器的位置，注意在下面要确定；
		   // MDL_VRESCKT, // <残压检测功能  没有定值，采用默认值；
			BaseMd[MDL_VRESCKTEND].Local.Num = MDL_VRESCKTEND;
			BaseMd[MDL_VRESCKTEND].Local.Name = D_DelayStart;
			BaseMd[MDL_VRESCKTEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[MDL_VRESCKTEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_VRESCKTEND].Local.InputPara.Trigger_Reg.Td1 = 0;//默认值
			BaseMd[MDL_VRESCKTEND].NodeNumber = NULL;// 此处还无法确认本定时器的位置，注意在下面要确定；
		   // MDL_BILST, //d_504E.Data.CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
			BaseMd[MDL_BILST].Local.Num = MDL_BILST;
			BaseMd[MDL_BILST].Local.Name = D_Pulse;
			BaseMd[MDL_BILST].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[MDL_BILST].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_BILST].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].CloseSwitchOneSideTime;//默认值
			BaseMd[MDL_BILST].NodeNumber = NULL;// 此处还无法确认本定时器的位置，注意在下面要确定；
		   // MDL_GETPWT,//d_504E.Data.LockCloseSwitchTime;///<双侧得电闭锁开关合闸时间
			BaseMd[MDL_GETPWT].Local.Num = MDL_GETPWT;
			BaseMd[MDL_GETPWT].Local.Name = D_DelayStart;
			BaseMd[MDL_GETPWT].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[MDL_GETPWT].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[MDL_GETPWT].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LockCloseSwitchTime2;//默认值
			BaseMd[MDL_GETPWT].NodeNumber = NULL;// 此处还无法确认本定时器的位置，注意在下面要确定；

			BaseMd[LSPW_MDL1].Local.Num = LSPW_MDL1;
			BaseMd[LSPW_MDL1].Local.Name = D_OR;
			BaseMd[LSPW_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LSPW_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LSPW_MDL1].NodeNumber = 2;
			BaseMd[LSPW_MDL1].Node[0].Num = MDL_TZLOCKSTATU;//闭锁跳闸状态；
			BaseMd[LSPW_MDL1].Node[1].Num = MDL_LSPWBSENB; //失电跳闸闭锁检测压板；

			BaseMd[LSPW_MDL2].Local.Num = LSPW_MDL2;
			BaseMd[LSPW_MDL2].Local.Name = D_AND;
			BaseMd[LSPW_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LSPW_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LSPW_MDL2].NodeNumber = 2;
			BaseMd[LSPW_MDL2].Node[0].Num = MDL_BREAKHE;//断路器合状态监测模块
			BaseMd[LSPW_MDL2].Node[1].Num = MDL_ILOSE;  //无流监控模块

			BaseMd[LSPW_MDL3].Local.Num = LSPW_MDL3;
			BaseMd[LSPW_MDL3].Local.Name = D_AND;
			BaseMd[LSPW_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LSPW_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LSPW_MDL3].NodeNumber = 2;
			BaseMd[LSPW_MDL3].Node[0].Num = LSPW_MDL1;
			BaseMd[LSPW_MDL3].Node[1].Num = LSPW_MDL2;

			BaseMd[LSPW_MDL4].Local.Num = LSPW_MDL4;
			BaseMd[LSPW_MDL4].Local.Name = D_AND;
			BaseMd[LSPW_MDL4].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LSPW_MDL4].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LSPW_MDL4].NodeNumber = 2;
			BaseMd[LSPW_MDL4].Node[0].Num = LSPW_MDL3;
			BaseMd[LSPW_MDL4].Node[1].Num =MDL_Vlose;

			BaseMd[LSPW_MDL5].Local.Num = LSPW_MDL5;
			BaseMd[LSPW_MDL5].Local.Name = D_AND;
			BaseMd[LSPW_MDL5].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LSPW_MDL5].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LSPW_MDL5].NodeNumber = 2;
			BaseMd[LSPW_MDL5].Node[0].Num = MDL_LSPWENB;//双侧失电分闸功能压板；
			BaseMd[LSPW_MDL5].Node[1].Num = LSPW_MDL4;

			BaseMd[MDL_LSPWT].NodeNumber = 1;
			BaseMd[MDL_LSPWT].Node[0].Num = LSPW_MDL5;

			BaseMd[LSPW_MDLEND].Local.Num = LSPW_MDLEND;
			BaseMd[LSPW_MDLEND].Local.Name = D_Pulse;
			BaseMd[LSPW_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[LSPW_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[LSPW_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[LSPW_MDLEND].NodeNumber = 1;
			BaseMd[LSPW_MDLEND].Node[0].Num = MDL_LSPWT;


			BaseMd[BIGTPW_MDL1].Local.Num = BIGTPW_MDL1;
			BaseMd[BIGTPW_MDL1].Local.Name = D_OR;
			BaseMd[BIGTPW_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL1].NodeNumber = 2;
			BaseMd[BIGTPW_MDL1].Node[0].Num = MDL_BGTPWBSENB;//单侧的电跳合闸闭锁检测；
			BaseMd[BIGTPW_MDL1].Node[1].Num = MDL_HZLOCKSTATU;//闭锁合闸状态；

			BaseMd[BIGTPW_MDL2].Local.Num = BIGTPW_MDL2;
			BaseMd[BIGTPW_MDL2].Local.Name = D_AND;
			BaseMd[BIGTPW_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL2].NodeNumber = 2;
			BaseMd[BIGTPW_MDL2].Node[0].Num = MDL_BREAKFEN;//断路器分状态监测模块
			BaseMd[BIGTPW_MDL2].Node[1].Num = MDL_BIGTPWENB;  //单侧得电延时合闸功能压板  2

			BaseMd[BIGTPW_MDL3].Local.Num = BIGTPW_MDL3;
			BaseMd[BIGTPW_MDL3].Local.Name = D_XOR; //异或门
			BaseMd[BIGTPW_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL3].NodeNumber = 2;
			BaseMd[BIGTPW_MDL3].Node[0].Num = MDL_VLEFTGET;//左侧得电监测模块
			BaseMd[BIGTPW_MDL3].Node[1].Num = MDL_VRIGHTGET;//右侧得电监测模块

			BaseMd[BIGTPW_MDL4].Local.Num = BIGTPW_MDL4;
			BaseMd[BIGTPW_MDL4].Local.Name = D_AND;
			BaseMd[BIGTPW_MDL4].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL4].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL4].NodeNumber = 2;
			BaseMd[BIGTPW_MDL4].Node[0].Num = BIGTPW_MDL1;
			BaseMd[BIGTPW_MDL4].Node[1].Num = BIGTPW_MDL2;


			BaseMd[BIGTPW_MDL5].Local.Num = BIGTPW_MDL5;//从无电到有电逻辑实现
			BaseMd[BIGTPW_MDL5].Local.Name = D_AND;
			BaseMd[BIGTPW_MDL5].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL5].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL5].NodeNumber = 2;
			BaseMd[BIGTPW_MDL5].Node[0].Num = BIGTPW_MDL3;
			BaseMd[BIGTPW_MDL5].Node[1].Num = MDL_Vlose;

			BaseMd[BIGTPW_MDL6].Local.Num = BIGTPW_MDL6;
			BaseMd[BIGTPW_MDL6].Local.Name = D_AND;
			BaseMd[BIGTPW_MDL6].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BIGTPW_MDL6].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BIGTPW_MDL6].NodeNumber = 2;
			BaseMd[BIGTPW_MDL6].Node[0].Num = BIGTPW_MDL4;
			BaseMd[BIGTPW_MDL6].Node[1].Num = BIGTPW_MDL5;


			BaseMd[MDL_BIGTPWT].NodeNumber = 1;
			BaseMd[MDL_BIGTPWT].Node[0].Num = BIGTPW_MDL6;

			BaseMd[BIGTPW_MDLEND].Local.Num = BIGTPW_MDLEND;
			BaseMd[BIGTPW_MDLEND].Local.Name = D_Pulse;
			BaseMd[BIGTPW_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[BIGTPW_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[BIGTPW_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[BIGTPW_MDLEND].NodeNumber = 1;
			BaseMd[BIGTPW_MDLEND].Node[0].Num = MDL_BIGTPWT;

			BaseMd[U0_MDL1].Local.Num = U0_MDL1;
			BaseMd[U0_MDL1].Local.Name = D_OR;
			BaseMd[U0_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[U0_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[U0_MDL1].NodeNumber = 2;
			BaseMd[U0_MDL1].Node[0].Num = MDL_U0BSENB;//U0故障跳闸闭锁检测；
			BaseMd[U0_MDL1].Node[1].Num = MDL_TZLOCKSTATU;//闭锁跳闸状态；

			BaseMd[U0_MDL2].Local.Num = U0_MDL2;
			BaseMd[U0_MDL2].Local.Name = D_AND;
			BaseMd[U0_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[U0_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[U0_MDL2].NodeNumber = 2;
			BaseMd[U0_MDL2].Node[0].Num = MDL_U0;      //U0监测模块
			BaseMd[U0_MDL2].Node[1].Num = MDL_U0CKENB;  //U0保护功能压板

			BaseMd[U0_MDL3].Local.Num = U0_MDL3;
			BaseMd[U0_MDL3].Local.Name = D_AND;
			BaseMd[U0_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[U0_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[U0_MDL3].NodeNumber = 2;
			BaseMd[U0_MDL3].Node[0].Num = U0_MDL1;
			BaseMd[U0_MDL3].Node[1].Num = U0_MDL2;

			BaseMd[U0_MDL4].Local.Num = U0_MDL4;
			BaseMd[U0_MDL4].Local.Name = D_AND;
			BaseMd[U0_MDL4].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[U0_MDL4].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[U0_MDL4].NodeNumber = 2;
			BaseMd[U0_MDL4].Node[0].Num = U0_MDL3;
			BaseMd[U0_MDL4].Node[1].Num = MDL_U0BREAKERCLOSE;//在Tu0时间内

			BaseMd[U0_MDL5].Local.Num = U0_MDL5;
			BaseMd[U0_MDL5].Local.Name = D_DelayStart;
			BaseMd[U0_MDL5].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[U0_MDL5].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[U0_MDL5].Local.InputPara.Trigger_Reg.Td1 = NODELAY;
			BaseMd[U0_MDL5].NodeNumber = 1;//
			BaseMd[U0_MDL5].Node[0].Num = U0_MDL4;

			BaseMd[U0_MDLEND].Local.Num = U0_MDLEND;
			BaseMd[U0_MDLEND].Local.Name = D_Pulse;
			BaseMd[U0_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[U0_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[U0_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[U0_MDLEND].NodeNumber = 1;
			BaseMd[U0_MDLEND].Node[0].Num = U0_MDL5;

			BaseMd[Ures_MDL1].Local.Num = Ures_MDL1;
			BaseMd[Ures_MDL1].Local.Name = D_AND;
			BaseMd[Ures_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[Ures_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[Ures_MDL1].NodeNumber = 2;
			BaseMd[Ures_MDL1].Node[0].Num = MDL_BREAKFEN;  //断路器分状态
			BaseMd[Ures_MDL1].Node[1].Num = MDL_VRESCKENB; //残压功能压板

			BaseMd[Ures_MDL2].Local.Num = Ures_MDL2;
			BaseMd[Ures_MDL2].Local.Name = D_OR;
			BaseMd[Ures_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[Ures_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[Ures_MDL2].NodeNumber = 2;
			BaseMd[Ures_MDL2].Node[0].Num = MDL_VLEFTRES;  //左边残压检定
			BaseMd[Ures_MDL2].Node[1].Num = MDL_VRIGHTRES; //右边残压检定

			BaseMd[Ures_MDL3].Local.Num = Ures_MDL3;
			BaseMd[Ures_MDL3].Local.Name = D_AND;
			BaseMd[Ures_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[Ures_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[Ures_MDL3].NodeNumber = 2;
			BaseMd[Ures_MDL3].Node[0].Num = Ures_MDL1;
			BaseMd[Ures_MDL3].Node[1].Num = Ures_MDL2;

			BaseMd[MDL_VRESCKTEND].NodeNumber = 1;
			BaseMd[MDL_VRESCKTEND].Node[0].Num = Ures_MDL3;//不停的监测；

			BaseMd[LCHZ_MDL1].Local.Num = LCHZ_MDL1;
			BaseMd[LCHZ_MDL1].Local.Name = D_AND;
			BaseMd[LCHZ_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL1].NodeNumber = 2;
			BaseMd[LCHZ_MDL1].Node[0].Num =MDL_CKIENB ;//故障电流压板；
			BaseMd[LCHZ_MDL1].Node[1].Num = MDL_IGZ;//故障电流；

			BaseMd[LCHZ_MDL2].Local.Num = LCHZ_MDL2;
			BaseMd[LCHZ_MDL2].Local.Name = D_OR;
			BaseMd[LCHZ_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL2].NodeNumber = 2;
			BaseMd[LCHZ_MDL2].Node[0].Num = MDL_TZLOCKSTATU; //闭锁跳闸状态
			BaseMd[LCHZ_MDL2].Node[1].Num = MDL_IUBSENB;     //ui闭锁跳闸状态压板

			BaseMd[LCHZ_MDL3].Local.Num = LCHZ_MDL3;//出现故障；
			BaseMd[LCHZ_MDL3].Local.Name = D_OR;
			BaseMd[LCHZ_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL3].NodeNumber = 2;
			BaseMd[LCHZ_MDL3].Node[0].Num = LCHZ_MDL1;  //故障电流监测     二者是或的关系；
			BaseMd[LCHZ_MDL3].Node[1].Num = MDL_VLOSE;  //失压监测

			BaseMd[LCHZ_MDL4].Local.Num = LCHZ_MDL4;
			BaseMd[LCHZ_MDL4].Local.Name = D_AND;
			BaseMd[LCHZ_MDL4].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL4].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL4].NodeNumber = 2;
			BaseMd[LCHZ_MDL4].Node[0].Num = LCHZ_MDL3;
			BaseMd[LCHZ_MDL4].Node[1].Num = MDL_LCCLSENB;//闭锁分闸功能压板

			BaseMd[LCHZ_MDL5].Local.Num = LCHZ_MDL5;  //未出现故障
			BaseMd[LCHZ_MDL5].Local.Name = D_NOT;
			BaseMd[LCHZ_MDL5].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL5].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL5].NodeNumber = 1;
			BaseMd[LCHZ_MDL5].Node[0].Num = LCHZ_MDL4;

			BaseMd[LCHZ_MDL6].Local.Num = LCHZ_MDL6;
			BaseMd[LCHZ_MDL6].Local.Name = D_AND;
			BaseMd[LCHZ_MDL6].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCHZ_MDL6].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCHZ_MDL6].NodeNumber = 2;
			BaseMd[LCHZ_MDL6].Node[0].Num = LCHZ_MDL4;
			BaseMd[LCHZ_MDL6].Node[1].Num = MDL_BSHZBREAKERCLOSE;//闭锁合闸功能       断路器合闸出口监测+时限；闭锁合闸时间LockCloseSwitchTime（Y时限）或者 无故障闭锁分闸时限LockOpenSwitchTime


			BaseMd[LCHZ_MDL7].Local.Num = LCHZ_MDL7;
			BaseMd[LCHZ_MDL7].Local.Name = D_DelayStart;
			BaseMd[LCHZ_MDL7].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[LCHZ_MDL7].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[LCHZ_MDL7].Local.InputPara.Trigger_Reg.Td1 = NODELAY;
			BaseMd[LCHZ_MDL7].NodeNumber = 1;//
			BaseMd[LCHZ_MDL7].Node[0].Num = LCHZ_MDL6;

			BaseMd[LCHZ_MDLEND].Local.Num = LCHZ_MDLEND;////闭锁合闸 出口 （跳闸，并置闭锁合闸标志位）
			BaseMd[LCHZ_MDLEND].Local.Name = D_Pulse;
			BaseMd[LCHZ_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[LCHZ_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[LCHZ_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[LCHZ_MDLEND].NodeNumber = 1;
			BaseMd[LCHZ_MDLEND].Node[0].Num = LCHZ_MDL7;

			BaseMd[LCFZ_MDL1].Local.Num = LCFZ_MDL1;  //对闭锁合闸 时限取反
			BaseMd[LCFZ_MDL1].Local.Name = D_NOT;
			BaseMd[LCFZ_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCFZ_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCFZ_MDL1].NodeNumber = 1;
			BaseMd[LCFZ_MDL1].Node[0].Num = MDL_BSHZBREAKERCLOSE;

			BaseMd[LCFZ_MDL2].Local.Num = LCFZ_MDL2;
			BaseMd[LCFZ_MDL2].Local.Name = D_AND;
			BaseMd[LCFZ_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[LCFZ_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[LCFZ_MDL2].NodeNumber = 2;
			BaseMd[LCFZ_MDL2].Node[0].Num = LCHZ_MDL5;//故障未出现
			BaseMd[LCFZ_MDL2].Node[1].Num = LCFZ_MDL1;//闭锁合闸功能       断路器合闸出口监测+时限；闭锁合闸时间LockCloseSwitchTime（Y时限）或者 无故障闭锁分闸时限LockOpenSwitchTime

			BaseMd[LCFZ_MDLEND].Local.Num = LCFZ_MDLEND;////闭锁分闸出口 TRUE 闭锁，FALSE 闭锁复归；
			BaseMd[LCFZ_MDLEND].Local.Name = D_Pulse;
			BaseMd[LCFZ_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[LCFZ_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[LCFZ_MDLEND].Local.InputPara.Trigger_Reg.Td1 = usr_cfg.BrkPara[LINE].LockOpenSwitchResetTime;
			BaseMd[LCFZ_MDLEND].NodeNumber = 1;
			BaseMd[LCFZ_MDLEND].Node[0].Num = LCFZ_MDL2;

			BaseMd[BILSPW_MDL1].Local.Num = BILSPW_MDL1;
			BaseMd[BILSPW_MDL1].Local.Name = D_OR;
			BaseMd[BILSPW_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL1].NodeNumber = 2;
			BaseMd[BILSPW_MDL1].Node[0].Num = MDL_BLSPWBSENB;//单侧失电跳合闸闭锁检测；
			BaseMd[BILSPW_MDL1].Node[1].Num = MDL_HZLOCKSTATU;//闭锁合闸状态；

			BaseMd[BILSPW_MDL2].Local.Num = BILSPW_MDL2;
			BaseMd[BILSPW_MDL2].Local.Name = D_AND;
			BaseMd[BILSPW_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL2].NodeNumber = 2;
			BaseMd[BILSPW_MDL2].Node[0].Num = MDL_BREAKFEN;//断路器分状态监测模块
			BaseMd[BILSPW_MDL2].Node[1].Num = MDL_BILSENB;  //单侧失电延时合闸功能压板  2

			BaseMd[BILSPW_MDL3].Local.Num = BILSPW_MDL3;
			BaseMd[BILSPW_MDL3].Local.Name = D_XOR; //异或门
			BaseMd[BILSPW_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL3].NodeNumber = 2;
			BaseMd[BILSPW_MDL3].Node[0].Num = MDL_VLEFTLOSE;//左侧失压监测模块
			BaseMd[BILSPW_MDL3].Node[1].Num = MDL_VRIGHTLOSE;//右侧失压监测模块

			BaseMd[BILSPW_MDL4].Local.Num = BILSPW_MDL4;
			BaseMd[BILSPW_MDL4].Local.Name = D_AND;
			BaseMd[BILSPW_MDL4].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL4].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL4].NodeNumber = 2;
			BaseMd[BILSPW_MDL4].Node[0].Num = BILSPW_MDL1;
			BaseMd[BILSPW_MDL4].Node[1].Num = BILSPW_MDL2;

			BaseMd[BILSPW_MDL5].Local.Num = BILSPW_MDL5;//完成从有电到无电状态的逻辑实现
			BaseMd[BILSPW_MDL5].Local.Name = D_AND;//
			BaseMd[BILSPW_MDL5].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL5].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL5].NodeNumber = 2;
			BaseMd[BILSPW_MDL5].Node[0].Num = BILSPW_MDL3;
			BaseMd[BILSPW_MDL5].Node[1].Num = MDL_Vget;

			BaseMd[BILSPW_MDL6].Local.Num = BILSPW_MDL6;
			BaseMd[BILSPW_MDL6].Local.Name = D_AND;
			BaseMd[BILSPW_MDL6].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[BILSPW_MDL6].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[BILSPW_MDL6].NodeNumber = 2;
			BaseMd[BILSPW_MDL6].Node[0].Num = BILSPW_MDL4;
			BaseMd[BILSPW_MDL6].Node[1].Num = BILSPW_MDL5;

			BaseMd[MDL_BILST].NodeNumber = 1;
			BaseMd[MDL_BILST].Node[0].Num = BILSPW_MDL6;

			BaseMd[BILSPW_MDLEND].Local.Num = BILSPW_MDLEND;
			BaseMd[BILSPW_MDLEND].Local.Name = D_Pulse;
			BaseMd[BILSPW_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[BILSPW_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[BILSPW_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[BILSPW_MDLEND].NodeNumber = 1;
			BaseMd[BILSPW_MDLEND].Node[0].Num = MDL_BILST;


			BaseMd[POWERON_MDL1].Local.Num = POWERON_MDL1;
			BaseMd[POWERON_MDL1].Local.Name = D_AND;
			BaseMd[POWERON_MDL1].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[POWERON_MDL1].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[POWERON_MDL1].NodeNumber = 2;
			BaseMd[POWERON_MDL1].Node[0].Num = MDL_BREAKFEN;//断路器分状态监测模块
			BaseMd[POWERON_MDL1].Node[1].Num = MDL_GETPWENB;  //双侧得电闭锁合闸 功能压板  2

			BaseMd[POWERON_MDL2].Local.Num = POWERON_MDL2;
			BaseMd[POWERON_MDL2].Local.Name = D_AND;
			BaseMd[POWERON_MDL2].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[POWERON_MDL2].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[POWERON_MDL2].NodeNumber = 2;
			BaseMd[POWERON_MDL2].Node[0].Num = MDL_VLEFTGET;//左侧得电监测模块
			BaseMd[POWERON_MDL2].Node[1].Num = MDL_VRIGHTGET;//右侧得电监测模块

			BaseMd[POWERON_MDL3].Local.Num = POWERON_MDL3;
			BaseMd[POWERON_MDL3].Local.Name = D_AND;
			BaseMd[POWERON_MDL3].Local.InputPara.Base_Reg.pA = NULL;
			BaseMd[POWERON_MDL3].Local.InputPara.Base_Reg.pB = NULL;
			BaseMd[POWERON_MDL3].NodeNumber = 2;
			BaseMd[POWERON_MDL3].Node[0].Num = POWERON_MDL1;
			BaseMd[POWERON_MDL3].Node[1].Num = POWERON_MDL2;

			BaseMd[MDL_GETPWT].NodeNumber = 1;
			BaseMd[MDL_GETPWT].Node[0].Num = POWERON_MDL3;

			BaseMd[POWERON_MDLEND].Local.Num = POWERON_MDLEND;
			BaseMd[POWERON_MDLEND].Local.Name = D_Pulse;
			BaseMd[POWERON_MDLEND].Local.InputPara.Trigger_Reg.PA =  NULL;
			BaseMd[POWERON_MDLEND].Local.InputPara.Trigger_Reg.sysms = &SystermMs;
			BaseMd[POWERON_MDLEND].Local.InputPara.Trigger_Reg.Td1 = ONESHOT;
			BaseMd[POWERON_MDLEND].NodeNumber = 1;
			BaseMd[POWERON_MDLEND].Node[0].Num = MDL_GETPWT;


			BaseMdNum = POWERON_MDLEND+1;

			//BaseMdHandle = new NewTool<BaseTool>[BaseMdNum];
			//delete []BaseMdHandle;

			for (i =0;i<BaseMdNum;i++){//第一步  模块功能配置；
					//printf("%d\n",i);
					BaseMdHandle= &BaseTabl[LINE][i];
					if(i==60){
						i = 60;
					}
					switch(BaseMd[i].Local.Name)
					{

					case D_BaseTool:
						break;
					case D_EQUAL :
						 BaseMdHandle->HostHandle = EqualTab.GetLab();//new EQUAL(BaseMdHandle[i]->CfgReg.Local.InputPara.Int8_Reg);
						 break;
					case D_NOT :
						BaseMdHandle->HostHandle =NotTab.GetLab(); //new NOT(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg);
						 break;
					case D_AND :
						BaseMdHandle->HostHandle = AndTab.GetLab();//new AND(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg);
						break;
					case D_NAND:
						BaseMdHandle->HostHandle = NandTab.GetLab();//new NAND(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg);
						break;
					case D_OR :
						BaseMdHandle->HostHandle = OrTab.GetLab();//new OR(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg);
						break;
					case D_XOR :
						BaseMdHandle->HostHandle = XorTab.GetLab();//new XOR(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg);
						break;
					case D_XNOR :
						BaseMdHandle->HostHandle = XnorTab.GetLab();//new XNOR(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg) ;
						break;
					case D_STEADY:
						BaseMdHandle->HostHandle = SteadyTab.GetLab();//new STEADY(BaseMdHandle[i]->CfgReg.Local.InputPara.Base_Reg) ;
						break;
					case D_HIGHANALOG :
						BaseMdHandle->HostHandle = HighanalogTab.GetLab();//new HIGHANALOG(BaseMdHandle[i]->CfgReg.Local.InputPara.HighLow_Reg) ;
						break;
					case D_LOWANALOG :
						BaseMdHandle->HostHandle = LowanalogTab.GetLab();//new LOWANALOG(BaseMdHandle[i]->CfgReg.Local.InputPara.HighLow_Reg) ;
						break;
					case D_COMPARE :
						BaseMdHandle->HostHandle =CompareTab.GetLab() ;//new COMPARE(BaseMdHandle[i]->CfgReg.Local.InputPara.Region_Reg) ;
						break;
					case D_ANALOGSTEADY:
						BaseMdHandle->HostHandle = AnalogsteadyTab.GetLab();//new ANALOGSTEADY(BaseMdHandle[i]->CfgReg.Local.InputPara.Region_Reg) ;
						break;
					case D_DelayStart :
						BaseMdHandle->HostHandle = DelayStartTab.GetLab();//new DelayStart(BaseMdHandle[i]->CfgReg.Local.InputPara.Trigger_Reg) ;
						break;
					case D_Pulse:
						BaseMdHandle->HostHandle = PulseTab.GetLab();//new Pulse(BaseMdHandle[i]->CfgReg.Local.InputPara.Trigger_Reg);
						break;
					case D_DelayStartDelay:
						BaseMdHandle->HostHandle = DelayStartDelayTab.GetLab();//new DelayStartDelay(BaseMdHandle[i]->CfgReg.Local.InputPara.Trigger_Reg) ;
						break;
					case D_StartConter:
						BaseMdHandle->HostHandle = StartConterTab.GetLab();//new StartConter(BaseMdHandle[i]->CfgReg.Local.InputPara.Conter_Reg);
						break;
					case D_StopConter:
						BaseMdHandle->HostHandle = StopConterTab.GetLab();//new StopConter(BaseMdHandle[i]->CfgReg.Local.InputPara.Conter_Reg);
						break;
					case D_ConterReached:
						BaseMdHandle->HostHandle = ConterReachedTab.GetLab();//new ConterReached(BaseMdHandle[i]->CfgReg.Local.InputPara.Conter_Reg);
						break;
					case D_ConterRuning:
						BaseMdHandle->HostHandle = ConterRuningTab.GetLab();//new ConterRuning(BaseMdHandle[i]->CfgReg.Local.InputPara.Conter_Reg);
						break;
					case D_ConterSleep:
						BaseMdHandle->HostHandle = ConterSleepTab.GetLab();//new ConterSleep(BaseMdHandle[i]->CfgReg.Local.InputPara.Conter_Reg);
						break;
					default:
						break;
					}
				if(BaseMdHandle->HostHandle ==NULL){
					while(1){
						printf("BaseMdHandle[%d][%d]==NULL \n",LINE,i);
					}
				}
				if (i==41){
					i  = 41;
				}
				BaseMdHandle->Config(BaseMd[i]);
			}
			//第二步 模块 功能链接

			for (i =0;i<BaseMdNum;i++){
				BaseMdHandle= &BaseTabl[LINE][i];
				if (i==41){
					i  = 41;
				}
				if (BaseMdHandle->CfgReg.NodeNumber>=1){
					//printf("need_node:%d\n",i);
					BaseMdHandle->HostHandle->A= &BaseTabl[LINE][BaseMdHandle->CfgReg.Node[0]].C;
				}
				if (BaseMdHandle->CfgReg.NodeNumber>=2){
					BaseMdHandle->HostHandle->B = &BaseTabl[LINE][BaseMdHandle->CfgReg.Node[1]].C;
				}
			}
	}

}
#pragma CODE_SECTION("localprotectInDDR2")
void LocalProtect::Init(){
	ParaInit();
	Paracpy();
	BaseMdInit();
}

#pragma CODE_SECTION("localprotectInDDR2")
int16 LocalProtect::GetParaId(int8 SecNum,int8 LineNum)
{
	Data<BaseDataType> data;
	uint32 Id;
	if(SecNum>=0&&SecNum<=1){
		Id = 0x504C+SecNum*3;
		//FaFunSet   			d_504C;///<FA功能设置参数
		user_data.GetData(Id++,&data);
		memcpy(&lp_cfg.dz[LineNum].d_504C,data.Data,sizeof(FaFunSet));
		//FaIsolationSet  d_504D;///<FA功能中分段开关参数
		user_data.GetData(Id++,&data);
		memcpy(&lp_cfg.dz[LineNum].d_504D,data.Data,sizeof(FaIsolationSet));
		//FaConnectionSet d_504E;///<FA功能中联络开关参数
		user_data.GetData(Id++,&data);
		memcpy(&lp_cfg.dz[LineNum].d_504E,data.Data,sizeof(FaConnectionSet));
		return 1;
	}
	return -1;
}
#pragma CODE_SECTION("localprotectInDDR2")
int16 LocalProtect::ReadCfg(uint32 Id)
{
	Data<BaseDataType> data;
	user_data.GetData(Id,&data);
	switch(Id)
	{

		case 0x5052:
		//DataType1  		d_5052;///<线路1开关定值区号设置
		memcpy(&lp_cfg.d_5052,data.Data,sizeof(lp_cfg.d_5052));
		GetParaId(lp_cfg.d_5052.Data,0);
		Paracpy();
		BaseMdInit();
		break;
		case 0x5053:
		//DataType1  		d_5053;///<线路2开关定值区号设置
		memcpy(&lp_cfg.d_5053,data.Data,sizeof(lp_cfg.d_5053));
		GetParaId(lp_cfg.d_5053.Data,1);
		Paracpy();
		BaseMdInit();
		break;
		case 0x5054:
		//FaCommSet	 		d_5054;///<FA功能公用参数设置
		memcpy(&lp_cfg.d_5054,data.Data,sizeof(lp_cfg.d_5054));
		Paracpy();
		BaseMdInit();
		break;

		default:
		break;
	}
	return 1;
}

