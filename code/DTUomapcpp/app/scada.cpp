#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_uart.h"
#include <time.h>
#include <string.h>///<memcpy
#include <stdlib.h>
#include "scada.h"
#include "message.h"///<用到message类中的消息
#include "fa.h"
#include "task.h"///<用到Task类中的g_task全局变量中Heart函数
#include "debug.h"
#include "mathlist.h"
#include "datatype.h"
#include "gpio.h"
#include "adsamp.h"
#include "DSPF_sp_fftSPxSP.h"
#include "fa_fft_cal.h"
#include "DSPF_util.h"
#include <math.h>
#include "mathlist.h"
#include "fpga.h"
#include "clock.h"
#include "interruptapp.h"
#include "dspdatatype.h"
#include "inout.h"
#include "at24c02.h"
#include "i2c.h"
#include "utils.h"
#include "inout.h"
#include "algorithm"
#include "edma3_interface.h"
extern Semaphore_Handle varm_sem;
#pragma DATA_SECTION("DATAINDDR2")
Scada scada;

#pragma DATA_SECTION("DATAINDDR2")
CHANGE_YC change_yc;
#pragma DATA_SECTION("DATAINDDR2")
SP_COMPLEX fft_out_scada[CHANNEL_NUM][SAMP_NUM];
#pragma DATA_SECTION("DATAINDDR2")
float channel_modules[CHANNEL_NUM][SAMP_NUM/2];//直流量，基波，... N次谐波    按照遥测数据项排列的
#pragma DATA_SECTION("DATAINDDR2")
SP_COMPLEX CurrentComplex[CHANNEL_NUM][SAMP_NUM];
#pragma DATA_SECTION("DATAINDDR2")
float CurrentModulus[CHANNEL_NUM][SAMP_NUM/2];//直流量，基波，... N次谐波    按照遥测数据项排列的
#pragma DATA_SECTION("DATAINDDR2");
YcRatio_S YcPortRatio[YC_BOARD_NUM];
#pragma DATA_SECTION("DATAINDDR2")
MANVALUE man_set;
#pragma DATA_SECTION("DATAINDDR2");
ratio_t  YcRatio[MAX_LINE_NUM];
float w32[M];
float *const ptr_w32 = w32;

#pragma DATA_SECTION("DATAINDDR2");
DcRatio_S DcRation[2][2];//两个通道，两种类型（电流，电压）
#pragma DATA_SECTION("DATAINDDR2");
DcRatio_S DcRationTmp[2][2];//两个通道，两种类型（电流，电压）

#pragma DATA_SECTION("DATAINDDR2")
YX_SYN_REG YxSynFlag;
#pragma DATA_SECTION("DATAINDDR2")
YX_REG YxTimeFlag;
#pragma DATA_SECTION("DATAINDDR2")
Uint64  FenYxMask[DB_YX_NUM]= {
	0x0000000000000001,
	0x0000000000000004,
	0x0000000000000010,
	0x0000000000000040,
	0x0000000000000100,
	0x0000000000000400,
	0x0000000000001000,
	0x0000000000004000,
	0x0000000000010000,
	0x0000000000040000,
	0x0000000000100000,
	0x0000000000400000,
	0x0000000001000000,
	0x0000000004000000,
	0x0000000010000000,
	0x0000000040000000,
};
#pragma DATA_SECTION("DATAINDDR2")
Uint64 HeYxMask[DB_YX_NUM]= {
	0x0000000000000002,
	0x0000000000000008,
	0x0000000000000020,
	0x0000000000000080,
	0x0000000000000200,
	0x0000000000000800,
	0x0000000000002000,
	0x0000000000008000,
	0x0000000000020000,
	0x0000000000080000,
	0x0000000000200000,
	0x0000000000800000,
	0x0000000002000000,
	0x0000000008000000,
	0x0000000020000000,
	0x0000000080000000,
};
#pragma DATA_SECTION("DATAINDDR2")
Uint64 MASKTAB[144]=
{
//bord1
0x0000000000000080,
0x0000000000000040,
0x0000000000000020,
0x0000000000000010,
0x0000000000000008,
0x0000000000000004,
0x0000000000000002,
0x0000000000000001,
0x0000000000008000,
0x0000000000004000,
0x0000000000002000,
0x0000000000001000,
0x0000000000000800,
0x0000000000000400,
0x0000000000000200,
0x0000000000000100,
0x0000000000010000,
0x0000000000020000,
0x0000000000040000,
0x0000000000080000,
0x0000000000100000,
0x0000000000200000,
0x0000000000400000,
0x0000000000800000,
//bord2
0x0000000080000000,
0x0000000040000000,
0x0000000020000000,
0x0000000010000000,
0x0000000008000000,
0x0000000004000000,
0x0000000002000000,
0x0000000001000000,
0x0000008000000000,
0x0000004000000000,
0x0000002000000000,
0x0000001000000000,
0x0000000800000000,
0x0000000400000000,
0x0000000200000000,
0x0000000100000000,
0x0000010000000000,
0x0000020000000000,
0x0000040000000000,
0x0000080000000000,
0x0000100000000000,
0x0000200000000000,
0x0000400000000000,
0x0000800000000000,

//bord3
0x0080000000000000,
0x0040000000000000,
0x0020000000000000,
0x0010000000000000,
0x0008000000000000,
0x0004000000000000,
0x0002000000000000,
0x0001000000000000,
0x8000000000000000,
0x4000000000000000,
0x2000000000000000,
0x1000000000000000,
0x0800000000000000,
0x0400000000000000,
0x0200000000000000,
0x0100000000000000,
0x0000000000000001,
0x0000000000000002,
0x0000000000000004,
0x0000000000000008,
0x0000000000000010,
0x0000000000000020,
0x0000000000000040,
0x0000000000000080,

//bord4
0x0000000000008000,
0x0000000000004000,
0x0000000000002000,
0x0000000000001000,
0x0000000000000800,
0x0000000000000400,
0x0000000000000200,
0x0000000000000100,
0x0000000000800000,
0x0000000000400000,
0x0000000000200000,
0x0000000000100000,
0x0000000000080000,
0x0000000000040000,
0x0000000000020000,
0x0000000000010000,
0x0000000001000000,
0x0000000002000000,
0x0000000004000000,
0x0000000008000000,
0x0000000010000000,
0x0000000020000000,
0x0000000040000000,
0x0000000080000000,//0-95
////powerbord
0x0000008000000000,//porwer 板 右边 遥信1 96
0x0000004000000000,//porwer 板 右边 遥信2 97
0x0000002000000000,//porwer 板 右边 遥信3 98
0x0000001000000000,//porwer 板 右边 遥信4 99
0x0000000100000000,//porwer 板 左边 遥信1 100
0x0000000200000000,//porwer 板 左边 遥信2
0x0000000400000000,//porwer 板 左边 遥信3
0x0000000800000000,//porwer 板 左边 遥信4
0x0000010000000000,////porwer 板 右边 遥信5 104
//////////////////end
0x0000020000000000,
0x0000040000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,
0x0000000000000000,//127
0x0000000000000003,//128
0x000000000000000C,//
0x0000000000000030,//
0x00000000000000C0,//
0x0000000000000300,//
0x0000000000000C00,//
0x0000000000003000,//
0x000000000000C000,
0x0000000000030000,
0x00000000000C0000,
0x0000000000300000,
0x0000000000C00000,
0x0000000003000000,
0x000000000C000000,
0x0000000030000000,
0x00000000C0000000
};                                        
                                           
                                           
/**
 * /brief 需要定义一个全局函数Task类处理该类的入口
*/                                         
void* scada_main(void*){                   
scada.Run();                               
return NULL;                               
}                                          
///////////////公有函数//////////////////////////////////////////////////
Scada::Scada(){
	strcpy(name,"Scada");
	Id=TASK_SCADA;
	xiebo_len=1;
}
/**
*************************************************************************************
*  @brief  初始化本模块。
*  @param[in]
*  @param[out]
*  @return  成功，返回1，否则返回0。
*  @note 初始化过程，保证在arm测没有参数传输过来的时刻依然可以默认参数运行；
**************************************************************************************
*/
int16 Scada::Init(){
	uint8 i;
	g_gpio.set(POWER_UP_RELAY_PIN1,1);
	g_gpio.set(POWER_UP_RELAY_PIN2,1);//cut power relay .
	TerFaultStatu.all = 0;
	memset(&YKREG,0,sizeof(YKREG));
	memset(&YKMEMREG,0,sizeof(YKMEMREG));
	memset(&RelayStatu,0,sizeof(RelayStatu));
	memset(&YxBuff,0,sizeof(YxBuff));
	memset(&YxTimeFlag,0,sizeof(YxTimeFlag));
	memset(&krtmp,0,sizeof(krtmp));
	memset(&lastkrtmp,0,sizeof(lastkrtmp));
	memset(&scada_cfg.soe_buffer,0,sizeof(scada_cfg.soe_buffer));
	memset(&scada_cfg.fault_buffer,0,sizeof(scada_cfg.fault_buffer));
	memset(&change_yc,0,sizeof(change_yc));
	memset(&yx_config,0xff,sizeof(yx_config));
	scada_cfg.d_5093.Data = 1000;//遥控脉冲时间
	scada_cfg.d_508D.Data = 5;
	scada_cfg.d_508E.Data = 220;
	scada_cfg.d_5094.Data = 5;//秒为单位
	scada_cfg.d_5092.Data = 1000;
	scada_cfg.ycavrnum = 0;
	scada_cfg.d_508B.Data = Pt_3;//三表法
	scada_cfg.d_508B.Data = Pt_3;//三表法
	scada_cfg.StartClbrt = 0;
	scada_cfg.ClbrtTime.SetFlag = 0;
	scada_cfg.Clbrtnum = 0;
	//人工值
	scada_cfg.d_5213.Data = 0;//人工值；
	scada_cfg.d_5214.Data.Ua.BaseValue = 100;
	scada_cfg.d_5214.Data.Ua.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Ua.HarmContent = 0;
	scada_cfg.d_5214.Data.Ub.BaseValue = 100;
	scada_cfg.d_5214.Data.Ub.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Ub.HarmContent = 0;
	scada_cfg.d_5214.Data.Uc.BaseValue = 100;
	scada_cfg.d_5214.Data.Uc.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Uc.HarmContent = 0;
	scada_cfg.d_5214.Data.Ia.BaseValue = 5;
	scada_cfg.d_5214.Data.Ia.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Ia.HarmContent = 0;
	scada_cfg.d_5214.Data.Ib.BaseValue = 5;
	scada_cfg.d_5214.Data.Ib.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Ib.HarmContent = 0;
	scada_cfg.d_5214.Data.Ic.BaseValue = 5;
	scada_cfg.d_5214.Data.Ic.HarmoDgree = 0;
	scada_cfg.d_5214.Data.Ic.HarmContent = 0;
	scada_cfg.d_5214.Data.IaAngle = 60;
	scada_cfg.d_5214.Data.IbAngle = 60;
	scada_cfg.d_5214.Data.IcAngle = 60;

	scada_cfg.d_508F.Data.DeadTime=500;//遥测死区时间设置 ms
	scada_cfg.d_508F.Data.DeadAngle = 0.5;//死区值
	scada_cfg.d_508F.Data.DeadFreq = 0.005;
	scada_cfg.d_508F.Data.DeadHarmoicCount = 0;
	scada_cfg.d_508F.Data.DeadHarmoicValue = 0;
	scada_cfg.d_508F.Data.DeadHumidity = 0;
	scada_cfg.d_508F.Data.DeadI = 0.002;
	scada_cfg.d_508F.Data.DeadP = 1.0;
	scada_cfg.d_508F.Data.DeadTemp = 0;
	scada_cfg.d_508F.Data.DeadU = 0.1;

	scada_cfg.DiffVol = 10;
	scada_cfg.DiffPhase = 10;
	scada_cfg.HzCkType = 0;
	scada_cfg.DiffVolP[0] = &scada_cfg.YcData.Ua1_4001;
	scada_cfg.DiffVolP[1] = &scada_cfg.YcData.Ub1_4002;
	scada_cfg.DiffVolName[0] = 0;
	scada_cfg.DiffVolName[1] = 1;

	for ( i = 0  ; i < MAX_LINE_NUM; i++ ){
		FreqReg[i].samp_t = SAMP_T;
		FreqReg[i].samp_Freq = 50;
		FreqReg[i].samp_adjcnt = 0;
		FreqReg[i].Freq = 50;
	}
	scada_cfg.d_5090.Data = 20;//遥信防抖时间
	UserParaMask.all = 0x000003FF;
	YxDataInit(SW_YX);	//对所有遥信的值进行初始化(软遥信)
	YxDataInit(HW_YX);	//对所有遥信的值进行初始化(硬遥信)
	init_ad_edma();
	return 1;
}
void Scada::doConfig(){

	YxPortConfig();			//根据遥信端口配置参数对scada_cfg.yx_config配置
	YueXianYxConfig();	//根据越限参数,初始化本类中的越限参数
	if (scada_cfg.d_507F.Data == TOTALRMS){
		xiebo_len = XIEBOLEN;
	}
	YcPortConfig();		//根据遥测端口配置参数对本类中的遥测配置参数进行设置.
	DeadConfig();
	YkPortConfig();
}
int16 Scada:: DeadConfig(void)
{
	uint16 i;
	if(scada_cfg.d_508F.Data.DeadU<0.001)   ///<电压死区设置
	{
		scada_cfg.d_508F.Data.DeadU = 0.5;
	}
	if(scada_cfg.d_508F.Data.DeadI<0.001)      ///<电流死区设置
	{
		   scada_cfg.d_508F.Data.DeadI = 0.5;
	}
	if(scada_cfg.d_508F.Data.DeadP<0.001)      ///<功率死区设置
	{
		   scada_cfg.d_508F.Data.DeadP = 1.0;
	}
	if(scada_cfg.d_508F.Data.DeadFreq<0.001)   ///<频率死区设置
	{
		   scada_cfg.d_508F.Data.DeadFreq = 0.5;
	}
	if(scada_cfg.d_508F.Data.DeadAngle<0.001)///<角度死区设置
	{
		   scada_cfg.d_508F.Data.DeadAngle = 0.5;//死区值
	}
	if(scada_cfg.d_508F.Data.DeadHarmoicValue<0.001)///<谐波有效值死区设置
	{
		   scada_cfg.d_508F.Data.DeadHarmoicValue = 0.5;
	}
	if(scada_cfg.d_508F.Data.DeadHarmoicCount<0.5)///<   谐波含有率死区设置
	{
		   scada_cfg.d_508F.Data.DeadHarmoicCount = 1.0;
	}
	if(scada_cfg.d_508F.Data.DeadTemp<0.001)   ///<温度死区设置
	{
		   scada_cfg.d_508F.Data.DeadTemp = 0.5;
	}
	if(scada_cfg.d_508F.Data.DeadHumidity<0.001)///<湿度死区设置
	{
		   scada_cfg.d_508F.Data.DeadHumidity = 0.5;
	}

	scada_cfg.Dead.Data.DeadU = scada_cfg.d_508F.Data.DeadU *scada_cfg.d_508E.Data/100;
	scada_cfg.Dead.Data.DeadI = scada_cfg.d_508F.Data.DeadI *scada_cfg.d_508D.Data/100;
	scada_cfg.Dead.Data.DeadP = scada_cfg.d_508E.Data * scada_cfg.d_508D.Data *scada_cfg.d_508F.Data.DeadP/100;
	scada_cfg.Dead.Data.cos = scada_cfg.d_508F.Data.DeadHarmoicCount;//scada_cfg.d_508F.Data.DeadP/100.0;
	scada_cfg.Dead.Data.DeadHarmoicValueU = scada_cfg.d_508F.Data.DeadHarmoicValue *scada_cfg.d_508E.Data/100;
	scada_cfg.Dead.Data.DeadHarmoicValueI = scada_cfg.d_508F.Data.DeadHarmoicValue *scada_cfg.d_508D.Data/100;
	scada_cfg.Dead.Data.DeadHarmoicCount  = scada_cfg.d_508F.Data.DeadHarmoicCount;
	scada_cfg.Dead.Data.DeadFreq = scada_cfg.d_508F.Data.DeadFreq * 50 /100;;
	scada_cfg.Dead.Data.DeadAngle = scada_cfg.d_508F.Data.DeadAngle * 360/100;;
	scada_cfg.Dead.Data.DeadHumidity =scada_cfg.d_508F.Data.DeadHumidity;
	scada_cfg.Dead.Data.DeadTemp = scada_cfg.d_508F.Data.DeadTemp * 100/100;;
	scada_cfg.Dead.Data.DeadTime = scada_cfg.d_508F.Data.DeadTime;//5
	CHANGE_YC_REG	*p_change_yc = &change_yc.Ua1_4001;
	for ( i = 0 ;i < MAX_LINE_NUM; i++){
		p_change_yc[UA(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[UA(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[UB(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[UC(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[UAB(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[UCB(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[U0(i)].dead = &scada_cfg.Dead.Data.DeadU;
		p_change_yc[IA1(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[IB1(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[IC1(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[I0(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[IA2(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[IB2(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[IC2(i)].dead = &scada_cfg.Dead.Data.DeadI;
		p_change_yc[P(i)].dead = &scada_cfg.Dead.Data.DeadP;
		p_change_yc[Q(i)].dead = &scada_cfg.Dead.Data.DeadP;
		p_change_yc[S(i)].dead = &scada_cfg.Dead.Data.DeadP;
		p_change_yc[Factor(i)].dead = &scada_cfg.Dead.Data.cos;
		p_change_yc[Freq(i)].dead = &scada_cfg.Dead.Data.DeadFreq;
	}
	change_yc.Dcv_4121.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.EnvTemp_4122.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.TerTemp_4123.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.LineTemp_4124.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.DC1_4125.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.DC2_4126.dead=&scada_cfg.Dead.Data.DeadTemp;
	change_yc.d_4A01.dead=&scada_cfg.Dead.Data.DeadAngle;
	change_yc.d_4A02.dead=&scada_cfg.Dead.Data.DeadAngle;
	return 1;
}



/**
*************************************************************************************  
*  @brief  读取配置文件.
*  @param[in] 
*  @param[out]  
*  @return  成功，返回1，否则返回0。
**************************************************************************************
*/ 	
int16 Scada:: ReadCfg(int32 Id)
{
	uint16 i,j;
	Data<BaseDataType> data;
	RecloseSet d_504B;
	DCOWORD Dco;
	DataType4 *Pyc1;
	SingleDigitSet *YcPortP = NULL;
	int16 Port = 0;
	user_data.GetData(Id,&data);
	switch(Id)
	{
	case 0x5054://保护公共定值:启动电流，失压阀值，故障复归时间；
		memcpy(&scada_cfg.d_5054,data.Data,sizeof(scada_cfg.d_5054));
		YueXianYxConfig();
		break;
	case 0x505D://DataType3 d_505D;   //出口继电器软压板
		memcpy(&scada_cfg.d_505D,data.Data,sizeof(scada_cfg.d_505D));
		break;
	case 0x505E://分回线远方就地开关设置（1~4回）
	case 0x505F://分回线远方就地开关设置（5~8回）
	case 0x5060://分回线远方就地开关设置（9~12回）
	case 0x5061://分回线远方就地开关设置（13~16回）
	case 0x5062://分回线远方就地开关设置（17~20回）
		i=Id-0x505E;
		memcpy(&scada_cfg.d_505E[i],data.Data,sizeof(scada_cfg.d_505E[i]));
		break;
	case 0x5063://DataType3 d_5063;   //分回线远方就地开关设置允许
		memcpy(&scada_cfg.d_5063,data.Data,sizeof(scada_cfg.d_5063));
		break;
	case 0x5064://出口继电器1~16类型
	case 0x5065://出口继电器17~32类型
	case 0x5066://出口继电器33~48类型
		i=Id-0x5064;
		memcpy(&scada_cfg.d_5064[i],data.Data,sizeof(scada_cfg.d_5064[i]));
		YkPortConfig();
		break;
	case 0x507F://DataType1	//<电压电流有效值计算方法  0全波 1 基波
		memcpy(&scada_cfg.d_507F,data.Data,sizeof(scada_cfg.d_507F));
		break;
	case 0x5080://模拟量端口配置
	case 0x5081:
	case 0x5082:
	case 0x5083:
	case 0x5084:
		i=Id-0x5080;
		memcpy(&scada_cfg.d_5080[i],data.Data,sizeof(scada_cfg.d_5080[i]));
		if(Id==0x5084){
			YcPortP = &scada_cfg.d_5080[0];
			Port = 0;
			/*
			 *  上位机下发的参数：总共5块遥测板，每块13个通道，共计65个通道
			 *  实际上每块板两块AD4606,具备16个通道的能力，所以将参数装换成实际的AD通道编号；
			 *  上位机 配置的通道            AD的通道
			 *   1--13                1--13
			 *   14-26                17-29
			 *   27-39                33-45
			 *   40-52                49-61
			 *   53-65                65-77
			 *   --                   78-80
			 */
			for ( i =0; i < YC_PORT_PARA_NUM; i++){
				for ( j = 0; j<SINGLE_DIGIT_SET_LEN; j++){
					Port = YcPortP[i].Data.Define[j].Port;
					if((Port!=NO_YC)&&(Port<=65)){
						YcPortP[i].Data.Define[j].Port = ((Port-1)/13)*3 +Port;
					}
				}
			}
			YcPortConfig();
		}
		break;
	case 0x5085://开入量端口配置
	case 0x5086:
	case 0x5087:
	case 0x5088:
		i=Id-0x5085;
		memcpy(&scada_cfg.d_5085[i],data.Data,sizeof(scada_cfg.d_5085[i]));
		if(Id==0x5088){
			YxPortConfig();
			YxDataInit(HW_YX);
		}
		break;
	case 0x5089://<双点遥信开关量输入配置
		memcpy(&scada_cfg.d_5089,data.Data,sizeof(scada_cfg.d_5089));
		YxPortConfig();
		YxDataInit(HW_YX);
		break;
	case 0x508a://UserPortSet d_508A;///<开出量端口配置
		memcpy(&scada_cfg.d_508A,data.Data,sizeof(scada_cfg.d_508A));
		YkPortConfig();
		break;
	case 0x508b://DataType1		d_508B;///<功率计算方法配置
		memcpy(&scada_cfg.d_508B,data.Data,sizeof(scada_cfg.d_508B));
		YueXianYxConfig();
		break;
	case 0x508c://DataType1		d_508C;///<直流输入量配置
		memcpy(&scada_cfg.d_508C,data.Data,sizeof(scada_cfg.d_508C));
		/*
		 * 	int8 DcType; 低四位第一通道，高四位第二通道，0：代表电压；1：代表电流
			int16 NominalValue; 低八位第电压 标称值，高八位电流标称值 ；
	
		 * 	int8 DcType; 低四位第二通道，高四位第一通道，0：代表电压；1：代表电流
			int16 NominalValue; 低八位第二通道标称值，高八位第一通道标称值 ；
		 *
		 */
	
		//第一通道参数配置
		scada_cfg.DcPara[0].DcType= (scada_cfg.d_508C.Data.DcType>>4)&0x0f;
		scada_cfg.DcPara[0].MaxStand[0]= (scada_cfg.d_508C.Data.NominalValue>>8)&0x00ff;
		scada_cfg.DcPara[0].MinStand[0]= 10;//scada_cfg.DcPara[0].MaxStand[0]/2;
		//第二通道参数配置
		scada_cfg.DcPara[1].DcType= scada_cfg.d_508C.Data.DcType&0x0f;
		scada_cfg.DcPara[1].MaxStand[0]= scada_cfg.d_508C.Data.NominalValue&0x00ff;
		scada_cfg.DcPara[1].MinStand[0]= 10;//scada_cfg.DcPara[1].MaxStand[0]/2;
	
		break;
	case 0x508d://DataType1		d_508D;///<电流标称值
		memcpy(&scada_cfg.d_508D,data.Data,sizeof(scada_cfg.d_508D));
		if((scada_cfg.d_508D.Data<10)||(scada_cfg.d_508D.Data>255)){
			scada_cfg.d_508D.Data = 5;
		}
		//	YcStandInit();
		DeadConfig();
		YueXianYxConfig();
		break;
	case 0x508e:
		//DataType1		d_508E;///<电压标称值
		memcpy(&scada_cfg.d_508E,data.Data,sizeof(scada_cfg.d_508E));
		if((scada_cfg.d_508E.Data<10)||(scada_cfg.d_508E.Data>255)){
			scada_cfg.d_508E.Data = 100;
		}
		//	YcStandInit();
		DeadConfig();
		YueXianYxConfig();
		break;
	case 0x508f://DeadAreaSet	d_508F;///<遥测死区值设置
		memcpy(&scada_cfg.d_508F,data.Data,sizeof(scada_cfg.d_508F));
		DeadConfig();
		break;
	case 0x5090://DataType3		d_5090;///<遥信防抖时间
		memcpy(&scada_cfg.d_5090,data.Data,sizeof(scada_cfg.d_5090));
		if(scada_cfg.d_5090.Data<2){
			scada_cfg.d_5090.Data = 1000;
		}
		//YxTimeSave(scada_cfg.d_5090.Data);
		//BreakOutCheck[0].CheckTd = scada_cfg.d_5093.Data + scada_cfg.d_5090.Data + 200;
		//BreakOutCheck[1].CheckTd = BreakOutCheck[0].CheckTd;
		break;
	case 0x5091://DataType3		d_5091;///<遥控返校失败取消遥控时间
		memcpy(&scada_cfg.d_5091,data.Data,sizeof(scada_cfg.d_5091));
		break;
	case 0x5092://DataType3		d_5092;///<遥控保持时间
		memcpy(&scada_cfg.d_5092,data.Data,sizeof(scada_cfg.d_5092));
		break;
	case 0x5093://DataType3		d_5093;///<遥控脉冲宽
		memcpy(&scada_cfg.d_5093,data.Data,sizeof(scada_cfg.d_5093));
		//BreakOutCheck[0].CheckTd = scada_cfg.d_5093.Data + scada_cfg.d_5090.Data + 200;
		//BreakOutCheck[1].CheckTd = BreakOutCheck[0].CheckTd;
		YkPortConfig();
		break;
	case 0x5094://DataType3		d_5094;///<延时断开操作电源时间
		memcpy(&scada_cfg.d_5094,data.Data,sizeof(scada_cfg.d_5094));
		break;
	case 0x5095://DataType2		d_5095;///<PT
		memcpy(&scada_cfg.d_5095,data.Data,sizeof(scada_cfg.d_5095));
		break;
	case 0x5096://DataType2		d_5096;///<CT
		memcpy(&scada_cfg.d_5096,data.Data,sizeof(scada_cfg.d_5096));
		break;
	case 0x5097://DataType2		d_5097;///<标定
		/*
		 * 第一字节：高四位代表类型1-直流标定，2-交流标定。
		 *           低四位代表数据类型：1-代表直流电压（0~48V），2-代表直流电流（4~20mA）。
		 * 第二字节：高四位代表通道号，每一位代表一个通道。
		 *          低四位代表标定步奏，对于直流标定分为1-代表输入最小值，
		 *                            2-输入最大值，3-结束标定。
		 * 每一步骤返回14表示标定成功，返回15表示标定失败。
		 */
		memcpy(&scada_cfg.d_5097,data.Data,sizeof(scada_cfg.d_5097));
		if (scada_cfg.d_5097.Data == BDSTEP_1){
			 scada_cfg.StartClbrt = BDSTART;
		}else{
			scada_cfg.StartDcClbrt = scada_cfg.d_5097.Data;
			DcCalibration();
		}
		break;
		//为遥测信息体配置
	case 0x5098:
	case 0x5099:
	case 0x509a:
	case 0x509b:
	case 0x509c:
	case 0x509d:
	case 0x509e:
	case 0x509f:
	case 0x50a1:
	case 0x50a2:
	case 0x50a3:
	case 0x50a4:
	case 0x50a5:
	case 0x50a6:
	case 0x50a7:
	case 0x50a8:
	case 0x50a9:
	case 0x50aa:
	case 0x50ab:
	case 0x50ac:
		i=Id-0x5098;
		memcpy(&scada_cfg.d_5098[i],data.Data,sizeof(scada_cfg.d_5098[i]));
		if (Id==0x50ac)//最后一帧才计算,放置多次重复计算
			YcPortConfig();
		break;
#if 0	
	//0x50AD-0x50CA为遥信信息体配置,在dsp侧可以不用理会
	case 0x50AD:
	case 0x50AE:
	case 0x50AF:
	case 0x50B0:
	case 0x50B1:
	case 0x50B2:
	case 0x50B3:
	case 0x50B4:
	case 0x50B5:
	case 0x50B6:
	case 0x50B7:
	case 0x50B8:
	case 0x50B9:
	case 0x50BA:
	case 0x50BB:
	case 0x50BC:
	case 0x50BD:
	case 0x50BE:
	case 0x50BF:
	case 0x50C0:
		//下面没有用
	case 0x50C1:
	case 0x50C2:
	case 0x50C3:
	case 0x50C4:
	case 0x50C5:
	case 0x50C6:
	case 0x50C7:
	case 0x50C8:
	case 0x50C9:
	case 0x50CA:
		i=Id-0x50AD;
		memcpy(&scada_cfg.d_50AD[i],data.Data,sizeof(scada_cfg.d_50AD[i]));
		if(Id==0x50C0){
			YxNatureInit();
			YxDataInit(HW_YX);
		}
		break;
#endif 
#if 0
	case 0x50CB://YKMsgSet	  d_50B8;///<遥控信息体属性配置
		memcpy(&scada_cfg.d_50CB,data.Data,sizeof(scada_cfg.d_50CB));
		break;
	case 0x5202://DataType4   d_5202;///<温度定值
		memcpy(&scada_cfg.d_5202,data.Data,sizeof(scada_cfg.d_5202));
		scada_cfg.TemLimit.Data.Limit = scada_cfg.d_5202.Data;
		scada_cfg.TemLimit.Data.Recover = scada_cfg.d_5202.Data - 0.5;
		scada_cfg.TemLimit.Data.ContinueTime = 1000;//1秒；
		break;
	case 0x5203://DataType4   d_5203;///<湿度定值
		memcpy(&scada_cfg.d_5203,data.Data,sizeof(scada_cfg.d_5203));
		scada_cfg.HumLimit.Data.Limit = scada_cfg.d_5203.Data;
		scada_cfg.HumLimit.Data.Recover = scada_cfg.d_5203.Data - 1.0;
		scada_cfg.HumLimit.Data.ContinueTime = 1000;//1秒；
		//DataType4   d_5204;///<经度定值
		//DataType4   d_5205;///<纬度定值
		//DataType1   d_5206;///<恢复出厂设置
#endif
	case 0x5207://DataType4   d_5207;//**蓄电池欠压切除阈值*/
		memcpy(&scada_cfg.d_5207,data.Data,sizeof(scada_cfg.d_5207));
		break;
	case 0x5208://DataType4   d_5208;///<蓄电池低压告警阈值
		memcpy(&scada_cfg.d_5208,data.Data,sizeof(scada_cfg.d_5208));
		scada_cfg.BatLowData.Data.Limit = scada_cfg.d_5208.Data;
		scada_cfg.BatLowData.Data.Recover = scada_cfg.d_5208.Data - 0.5;
		scada_cfg.BatLowData.Data.ContinueTime = 1000;//1秒；
		break;
	case 0x5209://DataType2   d_5209;///<蓄电池活化充放电间隔时间
		memcpy(&scada_cfg.d_5209,data.Data,sizeof(scada_cfg.d_5209));
		break;
	case 0x520A://WarnPara    d_520A;///<电压上限告警阈值，恢复阈值，持续时间
		memcpy(&scada_cfg.d_520A,data.Data,sizeof(scada_cfg.d_520A));
		YueXianYxConfig();
		break;
	case 0x520B://WarnPara    d_520B;///<电压下限告警阈值，恢复阈值，持续时间
		memcpy(&scada_cfg.d_520B,data.Data,sizeof(scada_cfg.d_520B));
		YueXianYxConfig();
		break;
	case 0x520C://WarnPara    d_520C;///<电流上限阈值，恢复阈值，持续时间
		memcpy(&scada_cfg.d_520C,data.Data,sizeof(scada_cfg.d_520C));
		YueXianYxConfig();
		break;
	case 0x520D://WarnPara    d_520D;///负荷越上限阈值，恢复阈值，持续时间
		memcpy(&scada_cfg.d_520D,data.Data,sizeof(scada_cfg.d_520D));
		YueXianYxConfig();
		break;
	case 0x520e:
		memcpy(&scada_cfg.d_520E,data.Data,sizeof(scada_cfg.d_520E));
		YueXianYxConfig();
		break;
	case 0x520F:
		memcpy(&scada_cfg.d_520F,data.Data,sizeof(scada_cfg.d_520F));
		YueXianYxConfig();
		break;
	case 0x5213://DataType1   d_5211;///<人工置数使能
		memcpy(&scada_cfg.d_5213,data.Data,sizeof(scada_cfg.d_5213));
		break;
	case 0x5214://UserRealDataDefPara d_5212;///<人工置数参数
		memcpy(&scada_cfg.d_5214,data.Data,sizeof(scada_cfg.d_5214));
		break;
	case 0x6001://DataType1 d_6001;///<断路器1（开/合）
		memcpy(&scada_cfg.d_6001,data.Data,sizeof(scada_cfg.d_6001));
		Dco.data = scada_cfg.d_6001.Data;
		ControlYk(BREAKER1,Dco);
		break;
	case 0x6002://
		memcpy(&scada_cfg.d_6002,data.Data,sizeof(scada_cfg.d_6002));
		Dco.data = scada_cfg.d_6002.Data;
		ControlYk(BREAKER2,Dco);
		break;
	case 0x6003://
		memcpy(&scada_cfg.d_6003,data.Data,sizeof(scada_cfg.d_6003));
		Dco.data = scada_cfg.d_6003.Data;
		ControlYk(BREAKER3,Dco);
		break;
	case 0x6004://
		memcpy(&scada_cfg.d_6004,data.Data,sizeof(scada_cfg.d_6004));
		Dco.data = scada_cfg.d_6004.Data;
		ControlYk(BREAKER4,Dco);
		break;
	case 0x6005:
		memcpy(&scada_cfg.d_6005,data.Data,sizeof(scada_cfg.d_6005));
		Dco.data = scada_cfg.d_6005.Data;
		ControlYk(BREAKER5,Dco);
		break;
	case 0x6006://
		memcpy(&scada_cfg.d_6006,data.Data,sizeof(scada_cfg.d_6006));
		Dco.data = scada_cfg.d_6006.Data;
		ControlYk(BREAKER6,Dco);
		break;
	case 0x6007://
		memcpy(&scada_cfg.d_6007,data.Data,sizeof(scada_cfg.d_6007));
		Dco.data = scada_cfg.d_6007.Data;
		ControlYk(BREAKER7,Dco);
		break;
	case 0x6008://
		memcpy(&scada_cfg.d_6008,data.Data,sizeof(scada_cfg.d_6008));
		Dco.data = scada_cfg.d_6008.Data;
		ControlYk(BREAKER8,Dco);
		break;
	case 0x6009://
		memcpy(&scada_cfg.d_6009,data.Data,sizeof(scada_cfg.d_6009));
		Dco.data = scada_cfg.d_6009.Data;
		ControlYk(BREAKER9,Dco);
		break;
	case 0x600A://
		memcpy(&scada_cfg.d_600A,data.Data,sizeof(scada_cfg.d_600A));
		Dco.data = scada_cfg.d_600A.Data;
		ControlYk(BREAKER10,Dco);
		break;
	case 0x600B:
		memcpy(&scada_cfg.d_600B,data.Data,sizeof(scada_cfg.d_600B));
		Dco.data = scada_cfg.d_600B.Data;
		ControlYk(BREAKER11,Dco);
		break;
	case 0x600C://
		memcpy(&scada_cfg.d_600C,data.Data,sizeof(scada_cfg.d_600C));
		Dco.data = scada_cfg.d_600C.Data;
		ControlYk(BREAKER12,Dco);
		break;
	case 0x600D://
		memcpy(&scada_cfg.d_600D,data.Data,sizeof(scada_cfg.d_600D));
		Dco.data = scada_cfg.d_600D.Data;
		ControlYk(BREAKER13,Dco);
		break;
	case 0x600E://
		memcpy(&scada_cfg.d_600E,data.Data,sizeof(scada_cfg.d_600E));
		Dco.data = scada_cfg.d_600E.Data;
		ControlYk(BREAKER14,Dco);
		break;
	case 0x600F:
		memcpy(&scada_cfg.d_600F,data.Data,sizeof(scada_cfg.d_600F));
		Dco.data = scada_cfg.d_600F.Data;
		ControlYk(BREAKER15,Dco);
		break;
	case 0x6010://
		memcpy(&scada_cfg.d_6010,data.Data,sizeof(scada_cfg.d_6010));
		Dco.data = scada_cfg.d_6010.Data;
		ControlYk(BREAKER16,Dco);
		break;
	case 0x6064://DataType1 d_6066;///活化
		memcpy(&scada_cfg.d_6064,data.Data,sizeof(scada_cfg.d_6064));
		Dco.data = scada_cfg.d_6064.Data;
		ControlYk(BATTARYACTION,Dco);
		break;
	case 0x6065://DataType1 d_6066;///公共遥控1
		memcpy(&scada_cfg.d_6065,data.Data,sizeof(scada_cfg.d_6065));
		Dco.data = scada_cfg.d_6065.Data;
		ControlYk(WINGMOTOR,Dco);
		break;
	case 0x6066://DataType1 d_6066;///公共遥控2
		memcpy(&scada_cfg.d_6066,data.Data,sizeof(scada_cfg.d_6066));
		Dco.data = scada_cfg.d_6066.Data;
		ControlYk(WATERMOTOR,Dco);
		break;
	/*case 0x6067:
		memcpy(&scada_cfg.d_6067,data.Data,sizeof(scada_cfg.d_6067));
		//DataType1 d_6066;///<电动机操作机构电源投入/退出
		Dco.data = scada_cfg.d_6067.Data;
		ControlYk(BATTARYOFF,Dco);
		break;*/
/*	case 0x6601:
		memcpy(&scada_cfg.d_6601,data.Data,sizeof(scada_cfg.d_6601));
		break;
	case 0x6602:
		memcpy(&scada_cfg.d_6602,data.Data,sizeof(scada_cfg.d_6602));
		break;
	case 0x6603:
		memcpy(&scada_cfg.d_6603,data.Data,sizeof(scada_cfg.d_6603));
		break;
	case 0x6604:
		memcpy(&scada_cfg.d_6604,data.Data,sizeof(scada_cfg.d_6604));
		break;
	case 0x6605:
		memcpy(&scada_cfg.d_6605,data.Data,sizeof(scada_cfg.d_6605));
		break;
	case 0x6606:
		memcpy(&scada_cfg.d_6606,data.Data,sizeof(scada_cfg.d_6606));
		break;
	case 0x6607:
		memcpy(&scada_cfg.d_6607,data.Data,sizeof(scada_cfg.d_6607));
		break;
*/
	case 0x504b://重合闸设置
		Pyc1=&scada_cfg.YcData.Ua1_4001;
		memcpy(&d_504B,data.Data,sizeof(d_504B));
		scada_cfg.DiffVol = d_504B.Data.DiffVol;
		scada_cfg.DiffPhase = d_504B.Data.DiffPhase;
		scada_cfg.HzCkType = d_504B.Data.Type;
		for ( i = 0; i < 2; i++){
			if(d_504B.Data.DiffVolDefine[i]<=0x4402){
				scada_cfg.DiffVolP[i] = &Pyc1[d_504B.Data.DiffVolDefine[i] - 0x4001];
			}
		}
		for ( i = 0; i < 2; i++){
			if(d_504B.Data.DiffVolDefine[i]<=0x400d){
				scada_cfg.DiffVolName[i] = d_504B.Data.DiffVolDefine[i] - 0x4001;
			}else if ((d_504B.Data.DiffVolDefine[i]<=0x4106)&&(d_504B.Data.DiffVolDefine[i]>=0x40fa)){
				scada_cfg.DiffVolName[i] = d_504B.Data.DiffVolDefine[i] - 0x40fa+13;
			}
		}
		break;
	case 0x5215:
		memcpy(&scada_cfg.d_5215,data.Data,sizeof(scada_cfg.d_5215));
		clk0.ClockSynchronization(&scada_cfg.d_5215.Data.time);
	   break;
	case 0x5055://FaDevotionSet d_5055;///<保护功能投退设置
		memcpy(&scada_cfg.d_5055,data.Data,sizeof(scada_cfg.d_5055));
	break;
	case 0x514a:
		memcpy(&scada_cfg.d_514A,data.Data,sizeof(scada_cfg.d_514A));
	default:
		break;
	}
	return -1;
}
//遥信属性本地参数初始化
#if 0
int16 Scada::YxMsgAddrCfgParaInit(void)
{
	uint16 i,j,Id;
	DataType1 data;
	YXMsgSet *P = &scada_cfg.d_50AD[0];
	Id = YX_0001;
	for (i = 0 ; i< YxNatureFrames;i++){
		for ( j = 0;j<YxNaturePerNum;j++){
			P[i].Data.Define[j].DataId = Id;
			P[i].Data.Define[j].Type = _0_KAI_1_HE;
			Id++;
			if(Id==YX_01A1+1){  //公共遥控4本地动作
				Id = YX_0201;   //线路1 A相电压越上限告警
			}
			if(Id==YX_0301+1){ //故障指示器
				Id = YX_0501;  //备自投
			}
			if(Id==YX_0501+1){ //备自投
				Id = YX_0701;  //状态监测
			}
			if(Id==YX_0701+1){  //备自投
				Id = YX_0801;   //配电房水位告警
			}
		}
	}
	return 1;
}
#endif 
#if 0
//遥信端口配置本地默认参数初始化,在没有从Arm侧获得参数前的默认参数.
void Scada::YxPortCfgParaInit(void)
{
	uint8 i,j;
	uint16 k;
	uint16 line;
///////////////////////单点
	SingleDigitSet *YxPortP = &scada_cfg.d_5085[0];
	for ( i = 0 ;i < YX_PORT_PARA_NUM;i++){
		for (j = 0;j < SINGLE_DIGIT_SET_LEN;j++){
			YxPortP[i].Data.Define[j].Port = NO_YC;
		}
	}
	k = 1;
	i = 0;
	j = 0;
	for ( line = 0 ; line < MAX_LINE_NUM; line++){
		YxPortP[i].Data.Define[j].Port = k++;
		YxPortP[i].Data.Define[j].Id = YX_0001 + line*YX_0017;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port =  k++;
		YxPortP[i].Data.Define[j].Id = YX_0002 + line*YX_0017;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port =  k++;
		YxPortP[i].Data.Define[j].Id = YX_0003 + line*YX_0017;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port = k++;
		YxPortP[i].Data.Define[j].Id = YX_0004 + line*YX_0017;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}

		YxPortP[i].Data.Define[j].Port =  NO_YX;
		YxPortP[i].Data.Define[j].Id = YX_0005 + line*YX_0017;//
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port =  NO_YX;
		YxPortP[i].Data.Define[j].Id = YX_0006 + line*YX_0017;//
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port =  k++;
		YxPortP[i].Data.Define[j].Id = YX_0016 + line*YX_0017;//本地合闸
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YxPortP[i].Data.Define[j].Port =  k++;
		YxPortP[i].Data.Define[j].Id = YX_0017 + line*YX_0017;//本地分闸
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
	}
	/*
	#define YX_0191  0x191   //公共遥信1
	#define YX_0192  0x192   //公共遥信2
	#define YX_0193  0x193   //公共遥信3
	#define YX_0194  0x194   //公共遥信4
	#define YX_0195  0x195   //远方/就地位置
	#define YX_0196  0x196   //电池低压告警/恢复
	#define YX_0197  0x197   //终端直流供电
	#define YX_0198  0x198   //装置工作电源异常告警/恢复
	#define YX_0199  0x199   //电池活化状态
	#define YX_019A  0x19A   //终端主交流供电
	#define YX_019B  0x19B   //终端备交流供电
	*/
	for ( line = 0 ; line < 11; line++){
		YxPortP[i].Data.Define[j].Port = NO_YX;
		YxPortP[i].Data.Define[j].Id = YX_0191 + line;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
	}
	//////////////////////双点
	for ( i = 0; i < DB_DIGIT_SET_LEN; i++){
		scada_cfg.d_5089.Data.Define[i].ClosePort = NO_YX;
		scada_cfg.d_5089.Data.Define[i].OpenPort = NO_YX;
	}
	for ( i = 0;i < MAX_LINE_NUM; i++){
		scada_cfg.d_5089.Data.Define[i].ClosePort = YX_0001+i*YX_0017;//1+i*6;//合位置
		scada_cfg.d_5089.Data.Define[i].OpenPort =  YX_0002+i*YX_0017;//2+i*6;//分位置
		scada_cfg.d_5089.Data.Define[i].Id = 0x171+i;
	}
	scada_cfg.d_5089.Data.Num = MAX_LINE_NUM;
}

#endif 
#if 0
//遥信属性配置表更新调用的函数
int16 Scada::YxNatureUpdata(YXMsgSet *Old, YXMsgSet  *New)
{
	uint16 i;
	Data<BaseDataType> yxtmp;
	DataType1 data;
	for ( i = 0; i <YxNaturePerNum ; i++ ){
		if (Old->Data.Define[i].Type!=New->Data.Define[i].Type)
		{
			user_data.GetData(Old->Data.Define[i].DataId,&yxtmp);
			memcpy(&data,yxtmp.Data,sizeof(data));
			//不管原先属性如何,只要属性变化了,就将遥信值翻转
			if (data.Data==0){
				data.Data = 1;
			}else if (data.Data==1){
				data.Data =0;
			}
			yxtmp.Data = &data;
			user_data.SetData(Old->Data.Define[i].DataId,&yxtmp);
		}
	}
	return 1;
}
#endif 
uint8 NullYx = _SYSFEN_;
void Scada::YxPortConfig(void)
{
	uint16 i,j;
	uint16 index = 0;
	SingleDigitSet *YxPortP;

	for ( i = 0;i <MaxUserYxNum; i++ ){
		yx_config.config_data[i].YxPortP  = &NullYx;
	}
	/*
	 *  step 1 单点遥信端口号配置
	 */
	YxPortP = &scada_cfg.d_5085[0];
	for ( i = 0; i < YX_PORT_PARA_NUM; i++){
		for ( j = 0; j<SINGLE_DIGIT_SET_LEN; j++){
			index = i*SINGLE_DIGIT_SET_LEN+j;
			if(index>=MAX_USER_SG_YX_NUM){//单点遥信配置的量
				break;
			}
			if(index>=MaxUserYxNum){
				continue;
			}
			if ((YxPortP[i].Data.Define[j].Port<=0)||(YxPortP[i].Data.Define[j].Port>(MAX_YX_PORT_NUM))){//PORT号异常处理
				yx_config.config_data[index].Port = NO_YX;
			}else{
				yx_config.config_data[index].Port = YxPortP[i].Data.Define[j].Port-1;
				yx_config.config_data[index].YxPortP = (uint8 *)&YxCurrent[yx_config.config_data[index].Port];
			}
			yx_config.config_data[index].Id = YxPortP[i].Data.Define[j].Id;
		}
	}

	/*
	 *  step 2 对配置了端口号的遥信项进行提取，放入yx_config.name中，并且统计出数目，统计需支持的板载信息
	 */
	yx_config.yx_num = MAX_USER_SG_YX_NUM;
	index = 0;
	for (i = 0; i < MAX_USER_SG_YX_NUM; i++){
		if (yx_config.config_data[i].Port == NO_YX){
			yx_config.config_data[i].YxPortP = &NullYx;;
			yx_config.yx_num--;
		}else{
			yx_config.configed_yx[index++] = i;//下标号就是遥信名字；
		}
	}
	/*
	 *  step 3 双点遥信 端口配置
	 */
	for (i = 0; i < DB_YX_NUM; i++){
		if ((scada_cfg.d_5089.Data.Define[i].ClosePort<=0)||(scada_cfg.d_5089.Data.Define[i].ClosePort==(0xffff))){//PORT号异常处理
			yx_config.config_data[DBYX(i)].ClosePort= NO_YX;
		}else{
			yx_config.config_data[DBYX(i)].ClosePort = NO_YX;
			for ( j = 0;j <MaxUserYxNum; j++ ){
				if(scada_cfg.d_5089.Data.Define[i].ClosePort==yx_config.config_data[j].Id){
					yx_config.config_data[DBYX(i)].ClosePort = yx_config.config_data[j].Port;
					break;
				}
			}
		}
		if ((scada_cfg.d_5089.Data.Define[i].OpenPort<=0)||(scada_cfg.d_5089.Data.Define[i].OpenPort==(0xffff))){//PORT号异常处理
			yx_config.config_data[DBYX(i)].OpenPort = NO_YX;
		}else{
			yx_config.config_data[DBYX(i)].OpenPort = NO_YX;
			for ( j = 0;j <MaxUserYxNum; j++ ){
				if(scada_cfg.d_5089.Data.Define[i].OpenPort==yx_config.config_data[j].Id){
					yx_config.config_data[DBYX(i)].OpenPort = yx_config.config_data[j].Port;
					break;
				}
			}
		}
		yx_config.config_data[DBYX(i)].Port = MAX_YX_PORT_NUM+i; //双点遥信的虚拟端口是固定的，分，和对应的硬端口是可配置的
		yx_config.config_data[DBYX(i)].YxPortP = (uint8*)&YxCurrent[MAX_YX_PORT_NUM+i];
		yx_config.config_data[DBYX(i)].Id = scada_cfg.d_5089.Data.Define[i].Id;
	}
	index=yx_config.yx_num;
	yx_config.yx_num+=DB_YX_NUM;
	for (i = 0; i < DB_YX_NUM; i++){
		if (yx_config.config_data[DBYX(i)].Port == NO_YX){
			yx_config.config_data[DBYX(i)].YxPortP = &NullYx;
			yx_config.yx_num--;
		}else{
			yx_config.configed_yx[index++] = DBYX(i);
		}
	}
}

#define PERLINESOFTYX 0x10
#define PERPTCTSOFTYX 0x17
int16 Scada::YueXianYxCfgParaInit(void)
{
	uint16 i;
	uint16 j;
	DataType4	*YcDataP = NULL	;
	scada_cfg.d_5054.Data.StartI = 0.1;
	scada_cfg.d_5054.Data.StartU = 30;
	scada_cfg.d_5054.Data.StopU = 25;
	scada_cfg.d_520A.Data.ContinueTime=1000;//1秒
	scada_cfg.d_520A.Data.Limit = 110.0;
	scada_cfg.d_520A.Data.Recover= 105.0;

	scada_cfg.d_520B.Data.ContinueTime=1000;//1秒
	scada_cfg.d_520B.Data.Limit =90.0;
	scada_cfg.d_520B.Data.Recover=95.0;

	scada_cfg.d_520C.Data.ContinueTime=1000;//1秒
	scada_cfg.d_520C.Data.Limit = 110.0;
	scada_cfg.d_520C.Data.Recover= 105.0;

	scada_cfg.d_520D.Data.ContinueTime=1000;//1秒
	scada_cfg.d_520D.Data.Limit  = 110.0;
	scada_cfg.d_520D.Data.Recover = 105.0;
	scada_cfg.TemLimit.Data.ContinueTime = 1000;
	scada_cfg.TemLimit.Data.Limit  = 45.0;
	scada_cfg.TemLimit.Data.Recover = 44.0;
	scada_cfg.HumLimit.Data.ContinueTime = 1000;
	scada_cfg.HumLimit.Data.Limit  = 95.0;
	scada_cfg.HumLimit.Data.Recover = 94.0;
	scada_cfg.BatLowData.Data.ContinueTime = 1000;
	scada_cfg.BatLowData.Data.Limit  = 23.0;
	scada_cfg.BatLowData.Data.Recover = 23.5;
	for ( i = 0; i<YUEXIANYXNUM+MAX_LINE_NUM;i++ ){
		scada_cfg.YueXianYx[i].ChangeFlag = 0;
	}
	j = 0;
	YcDataP = (DataType4 *)&scada_cfg.YcData.Ua1_4001;
	for ( i = 0; i<MAX_LINE_NUM;i++ )
	{
		//DataType1 d_0201;///<线路1 A相电压越上限告警
		scada_cfg.YueXianYx[j].DataId.Id = YX_0201+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP =&YcDataP[UA(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1;
		// DataType1 d_0202;///<线路1 B相电压越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0202+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[UB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		// DataType1 d_0203;///<线路1 C相电压越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0203+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[UC(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		//线路1 AB线电压越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0204+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[UAB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		//线路1 CB线电压越上限告警
	    j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0205+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[UCB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		j++;
		//线路1 零序电压越上限告警
		scada_cfg.YueXianYx[j].DataId.Id = YX_0206+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.U0upData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[U0(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		// DataType1 d_0204;///<线路1 A相电压越下限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0207+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP =&YcDataP[UA(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		// DataType1 d_0205;///<线路1 B相电压越下限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0208+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP =  &YcDataP[UB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		// DataType1 d_0206;///<线路1 C相电压越下限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0209+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP =  &YcDataP[UC(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		//线路1 AB线电压越下限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020A+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[UAB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		//线路1 CB线电压越下限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020B+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP =  &YcDataP[UCB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		// DataType1 d_0207;///<线路1 A相电流越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020C+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[IA1(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		// DataType1 d_0208;///<线路1 B相电流越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020D+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[IB1(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		// DataType1 d_0209;///<线路1 C相电流越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020E+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[IC1(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;

		//线路1 零序电流越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_020F+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.I0upData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[I0(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		// DataType1 d_020A;///<线路1 负荷越上限告警
		j++;
		scada_cfg.YueXianYx[j].DataId.Id = YX_0210+i*PERLINESOFTYX;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.SupData;
		scada_cfg.YueXianYx[j].LimitType = SHANG_XIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[S(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		j++;
	}
	//失压告警
	for( i = YUEXIANYXNUM,j=0 ;i<YUEXIANYXNUM+MAX_LINE_NUM; i++,j++) {
		scada_cfg.YueXianYx[i].DataId.Id = YX_0181+j;
		scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.LoseVol;
		scada_cfg.YueXianYx[i].LimitType = XIA_XIAN;
		scada_cfg.YueXianYx[i].SrcYcP = &MaxVol[j] ;
		scada_cfg.YueXianYx[i].Enable = 1 ;
	}
	//#define YX_0196  0x196   电池低压告警/恢复(1)（同时这个遥信也可能是硬遥信对应到电池欠压告警遥信上）
	scada_cfg.YueXianYx[i].DataId.Id = YX_0196;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.BatLowData;
	scada_cfg.YueXianYx[i].LimitType = XIA_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.Dcv_4121;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;
	/*
	DataType4	EnvTemp_4122	;//环境温度
	DataType4	TerTemp_4123	;//终端温度
	DataType4	LineTemp_4124	;//电缆温度
	DataType4	DC1_4125	;
	DataType4	DC2_4126	;
	 */
//#define YX_019D  0x19D   //终端温度告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_019D;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.TemLimit;
	scada_cfg.YueXianYx[i].LimitType = SHANG_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.TerTemp_4123;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;
//#define YX_0801  0x801   //配电房水位告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0801;
	scada_cfg.YueXianYx[i].LimitParaP = NULL;
	scada_cfg.YueXianYx[i].LimitType = XIA_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP =NULL;
	scada_cfg.YueXianYx[i].Enable = 0 ;
	i++;
//#define YX_0802  0x802   //配电房火灾告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0802;
	scada_cfg.YueXianYx[i].LimitParaP = NULL;
	scada_cfg.YueXianYx[i].LimitType = XIA_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = NULL;
	scada_cfg.YueXianYx[i].Enable = 0 ;
	i++;
//#define YX_0803  0x803   //配电房环境温度告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0803;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.TemLimit;
	scada_cfg.YueXianYx[i].LimitType = SHANG_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.EnvTemp_4122;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;
//#define YX_0804  0x804   //电缆温度告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0804;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.TemLimit;
	scada_cfg.YueXianYx[i].LimitType = SHANG_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.LineTemp_4124;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;

//#define YX_0805  0x805   //直流母线电压越上限告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0805;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.UupData;
	scada_cfg.YueXianYx[i].LimitType = SHANG_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.DC2_4126;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;
//#define YX_0806  0x806   //直流母线电压越下限告警(1)
	scada_cfg.YueXianYx[i].DataId.Id = YX_0806;
	scada_cfg.YueXianYx[i].LimitParaP = &scada_cfg.UdownData;
	scada_cfg.YueXianYx[i].LimitType = XIA_XIAN;
	scada_cfg.YueXianYx[i].SrcYcP = &scada_cfg.YcData.DC2_4126;
	scada_cfg.YueXianYx[i].Enable = 1 ;
	i++;
	YcDataP = (DataType4 *)&scada_cfg.YcData.Ua1_4001;
	for ( i = 0; i<MAX_LINE_NUM;i++ ){
		PtCtReg[i].SrcIa[0].SrcP = &YcDataP[IA1(i)] ;
		PtCtReg[i].SrcIa[1].SrcP = &YcDataP[IB1(i)] ;
		PtCtReg[i].SrcIa[2].SrcP = &YcDataP[IC1(i)] ;
		PtCtReg[i].StartI = NOMINAL_I * 0.002;
		PtCtReg[i].CriticalU = NOMINAL_U *0.3;
		PtCtReg[i].limitT =10000;//10秒；
		for (j = 0;j < 3;j++){
			PtCtReg[i].PtSoeId.Id = YX_0014+i*PERPTCTSOFTYX;
			PtCtReg[i].CtSoeId.Id = YX_0015+i*PERPTCTSOFTYX;
			//PtCtReg[i].PtFlag= 0;
			PtCtReg[i].PtStatu[j]= _SYSFEN_;
			//PtCtReg[i].CtFlag = 0;
			PtCtReg[i].CtStatu[j]= _SYSFEN_;
			//PtCtReg[i].PhFlag = 0;
		}
	}
	return 1;
}
/*
 * 功率计算方法变化；各类越线参数变化需要调用此函数
 */
int16 Scada::YueXianYxConfig(void)
{
	uint16 i;
	DataType4 *YcDataP;
	YcDataP = (DataType4	*)&scada_cfg.YcData.Ua1_4001;
	for ( i = 0; i<MAX_LINE_NUM;i++ ){
			PtCtReg[i].SrcUa[0].SrcP = &YcDataP[UAB(i)];
		if(scada_cfg.d_508B.Data>= Pt_2){//两表法
			PtCtReg[i].SrcUa[1].SrcP = NULL;
			PtCtReg[i].SrcUa[2].SrcP = &YcDataP[UCB(i)];
		}else{
			PtCtReg[i].SrcUa[0].SrcP = &YcDataP[UA(i)];
			PtCtReg[i].SrcUa[1].SrcP = &YcDataP[UB(i)];
			PtCtReg[i].SrcUa[2].SrcP = &YcDataP[UC(i)];
		}

	}
    scada_cfg.UupData.Data.ContinueTime = scada_cfg.d_520A.Data.ContinueTime;
    scada_cfg.UupData.Data.Limit = scada_cfg.d_520A.Data.Limit * NOMINAL_U /100.0;
    scada_cfg.UupData.Data.Recover  =  scada_cfg.d_520A.Data.Recover* NOMINAL_U /100.0;

    scada_cfg.U0upData.Data.ContinueTime = scada_cfg.d_520E.Data.ContinueTime;
    scada_cfg.U0upData.Data.Limit = scada_cfg.d_520E.Data.Limit * NOMINAL_U /100.0;
    scada_cfg.U0upData.Data.Recover  =  scada_cfg.d_520E.Data.Recover* NOMINAL_U /100.0;

    scada_cfg.UdownData.Data.ContinueTime = scada_cfg.d_520B.Data.ContinueTime;
    scada_cfg.UdownData.Data.Limit = scada_cfg.d_520B.Data.Limit * NOMINAL_U /100.0;
    scada_cfg.UdownData.Data.Recover  =  scada_cfg.d_520B.Data.Recover* NOMINAL_U /100.0;

    scada_cfg.IupData.Data.ContinueTime = scada_cfg.d_520C.Data.ContinueTime;
    scada_cfg.IupData.Data.Limit = scada_cfg.d_520C.Data.Limit * NOMINAL_I /100.0;
    scada_cfg.IupData.Data.Recover  =  scada_cfg.d_520C.Data.Recover* NOMINAL_I /100.0;

    scada_cfg.I0upData.Data.ContinueTime = scada_cfg.d_520F.Data.ContinueTime;
    scada_cfg.I0upData.Data.Limit = scada_cfg.d_520F.Data.Limit * NOMINAL_I /100.0;
    scada_cfg.I0upData.Data.Recover  =  scada_cfg.d_520F.Data.Recover* NOMINAL_I /100.0;

    scada_cfg.SupData.Data.ContinueTime = scada_cfg.d_520D.Data.ContinueTime;
    scada_cfg.SupData.Data.Limit = scada_cfg.d_520D.Data.Limit * NOMINAL_TOTALS /100.0;
    scada_cfg.SupData.Data.Recover  =  scada_cfg.d_520D.Data.Recover* NOMINAL_TOTALS /100.0;

    scada_cfg.LoseVol.Data.ContinueTime =1000;
    scada_cfg.LoseVol.Data.Limit = scada_cfg.d_5054.Data.StopU * NOMINAL_U /100.0;
    scada_cfg.LoseVol.Data.Recover  =  scada_cfg.d_5054.Data.StartU* NOMINAL_U /100.0;

	for(i = 0; i< MAX_LINE_NUM;i++){
		PtCtReg[i].StartI = NOMINAL_I * (scada_cfg.d_5054.Data.StartI/100.0);//0.02;
		PtCtReg[i].CriticalU = NOMINAL_U * (scada_cfg.d_5054.Data.StartU/100.0);//*0.3;
		PtCtReg[i].limitT =10000;//10秒；
	}

	return 1;
}

//输入遥信ID 搜索属性，并对属性进行甄别1：把用户的设置的属性值转换成系统定义的属性值，
//由于遥信表属性表中d_50AD----d_50CA中含有双点遥信，双点遥信的属性有不在此表中。
//双点遥信的必须从双点遥信配置表d_5089，中提取双点遥信的配置项 去寻找属性。
//此函数适合在数据表初始化中使用，遥信发生是使用效率会很低。

/*
#define SETBIT(dest,mask)     (dest |= mask) //按照掩码位置   置位
#define CLRBIT(dest,mask)     (dest &= ~mask)//按照掩码位置   清位
#define TGLBIT(dest,mask)     (dest ^= mask) //按照掩码位置   取反
#define CHKBIT(dest,mask)     (dest & mask)  //按照掩码位置  查阅 不改变状态
*/
/**
*************************************************************************************  
*  @brief		:根据遥信属性配置,返回逻辑意义上的分合.
*				物理上断路器分合的时候,有可能遥信电平为1,或0 ,需要将物理上分合信号转换成逻辑上的分合信号.
*				在系统中对于单点遥信	逻辑上	1代表合,0代表分
*							双点遥信	逻辑上	2代表合,1代表分
*				对于每一个遥信都有一个属性位,_0_KAI_1_HE or _1_KAI_0_HE
*				根据该属性位,结合物理遥信信号可以给出逻辑意义上的分合信号.
*  @param[in]	:data	物理电平信号
*  @param[in]	:yxtype	输入的物理信号是单点还是双点遥信
*  @param[out]
*  @return		:返回逻辑意义上的分合信号
*  @note		:根据GetYxNature函数描述,type类型没有意义
**************************************************************************************
*/
#pragma CODE_SECTION("CODEINIRAM")
int8 Scada:: GetLogicYx(uint8 data, uint8 yxtype)
{
	if(yxtype==COUPLE){
		YX_UNION dbyx;
		dbyx.all = data;

		if (dbyx.bit.bit1==_SYSHE_){
			dbyx.bit.bit1 =LOGIC_HE;
		}else {
			dbyx.bit.bit1 = LOGIC_FEN;
		}
		if (dbyx.bit.bit2==_SYSHE_){
			dbyx.bit.bit2 =LOGIC_HE;
		}else {
			dbyx.bit.bit2 = LOGIC_FEN;
		}
	    return dbyx.all;
	}else{
		if (data==_SYSHE_){
			return LOGIC_HE;
		}else {
			return LOGIC_FEN;
		}
	}
#if 0
	if(yxtype==COUPLE){
		YX_UNION dbyx;
		YX_UNION yxnature;
		yxnature.all = type;
		dbyx.all = data;

	    if (yxnature.bit.bit1== _0_KAI_1_HE){
			if (dbyx.bit.bit1==_SYSHE_){
				dbyx.bit.bit1 =1;
			}else {
				dbyx.bit.bit1 = 0;
			}
		}else{
			if (dbyx.bit.bit1==_SYSHE_){
				dbyx.bit.bit1 = 0;
			}else {
				dbyx.bit.bit1 = 1;
			}
		}
	    if (yxnature.bit.bit2== _0_KAI_1_HE){
			if (dbyx.bit.bit2==_SYSHE_){
				dbyx.bit.bit2 =1;
			}else {
				dbyx.bit.bit2 = 0;
			}
		}else{
			if (dbyx.bit.bit2==_SYSHE_){
				dbyx.bit.bit2 = 0;
			}else {
				dbyx.bit.bit2 = 1;
			}
		}
	    return dbyx.all;

	}else{
	    if (type== _0_KAI_1_HE){
			if (data==_SYSHE_){
				return 1;
			}else {
				return 0;
			}
		}else{
			if (data==_SYSHE_){
				return 0;
			}else {
				return 1;
			}
		}
	}
#endif 

}

/*
 * 所有遥信初始状态无故障
 */
int8 Scada:: YxDataInit(int type)
{
	uint16 i, j,k;
	Data<BaseDataType> tmp;
	DataType1 data;
	if(type==SW_YX){
		//1 PT, CT断线遥信
		for ( i = 0; i<MAX_LINE_NUM;i++ ){
			//for (j = 0;j < 3;j++){
				PtCtReg[i].PtSoeId.Data =LOGIC_FEN; 
				data.Data = PtCtReg[i].PtSoeId.Data;
				tmp.Data = &data;
				user_data.SetData(PtCtReg[i].PtSoeId.Id,&tmp);
	
				PtCtReg[i].CtSoeId.Data = LOGIC_FEN;
				data.Data = PtCtReg[i].CtSoeId.Data;
				tmp.Data = &data;
				user_data.SetData(PtCtReg[i].CtSoeId.Id,&tmp);
			//}
		}
		//2 越限遥信
		for ( i = 0; i<MAX_YUEXIAN_SOFT_YX_NUM;i++ ){
			scada_cfg.YueXianYx[i].DataId.Data = LOGIC_FEN;
			data.Data = scada_cfg.YueXianYx[i].DataId.Data;
			tmp.Data = &data;
			user_data.SetData(scada_cfg.YueXianYx[i].DataId.Id,&tmp);
		}
		//3 线路故障遥信
		for ( i = 0 ; i <MAX_LINE_NUM; i++ ){
			for ( j = 0;j < 2; j++){
			fa.GrdCurrenHandle[i].report_data =LOGIC_FEN; 
				data.Data = fa.GrdCurrenHandle[i].report_data;
				tmp.Data = &data;
				user_data.SetData(fa.GrdCurrenHandle[i].SoeId[j],&tmp);
			}
			for ( j = 0;j < 3; j++){
				for (k = 0; k< 2 ; k++){
					fa.CurrenHandle[i][j].report_data =LOGIC_FEN; 
					data.Data = fa.CurrenHandle[i][j].report_data;
					tmp.Data = &data;
					user_data.SetData(fa.CurrenHandle[i][j].SoeId[k],&tmp);
				}
			}
			for ( j = 0 ; j < 6; j++){
				fa.GzCheckHandle[i].GzSoeData = LOGIC_FEN; 
				data.Data = fa.GzCheckHandle[i].GzSoeData ;
				tmp.Data = &data;
				user_data.SetData(fa.GzCheckHandle[i].GzSoeId[j],&tmp);
			}
		}
	}else if(type == HW_YX){
		//4硬件相关的遥信
		GetFirstYxData();
		for ( i = 0; i < MaxUserYxNum; i++){
			yx_config.config_data[i].Yx.Data = GetLogicYx(yx_config.config_data[i].YxPortP[0],yx_config.config_data[i].Yx.Type );
			data.Data = yx_config.config_data[i].Yx.Data;
			tmp.Data = &data;
			user_data.SetData(yx_config.config_data[i].Id,&tmp);
		}
	}
	return 1;

}

/**
*************************************************************************************  
*  @brief		:配置所有遥信的属性(0开1合)
*				遥信包括:
	*			Scada类				PtCtReg
*					ScadaPara类		YueXianYx
*					yx_config		ad_compass.Yx
*				fa类			GrdCurrenHandle,CurrenHandle,GzCheckHandle,
*  @param[in]
*  @param[out]
*  @return  1
*  @note		遥信属性配置对于软遥信没有意义
**************************************************************************************
*/
#if 0
int8 Scada:: YxNatureInit(void)
{
	uint16 i,j;
	//1 PT, CT断线遥信
	for ( i = 0; i<MAX_LINE_NUM;i++ ){
		PtCtReg[i].PtSoeId.Nature= _0_KAI_1_HE;
		PtCtReg[i].CtSoeId.Nature= _0_KAI_1_HE;
	}
	//2 越限遥信
	for ( i = 0; i<MAX_YUEXIAN_SOFT_YX_NUM;i++ ){
		scada_cfg.YueXianYx[i].DataId.Nature=_0_KAI_1_HE;
	}
	//3 线路故障遥信
	for ( i = 0 ; i <MAX_LINE_NUM; i++ ){
		fa.GrdCurrenHandle[i].SoeNature[0] = _0_KAI_1_HE;
		fa.GrdCurrenHandle[i].SoeNature[1] =  _0_KAI_1_HE;
		for(j = 0 ; j< 3; j++){
			fa.CurrenHandle[i][j].SoeNature[0] =  _0_KAI_1_HE;
			fa.CurrenHandle[i][j].SoeNature[1] =  _0_KAI_1_HE;
		}
		fa.GzCheckHandle[i].GzSoeNature[0]=  _0_KAI_1_HE;
		fa.GzCheckHandle[i].GzSoeNature[1]=  _0_KAI_1_HE;
		fa.GzCheckHandle[i].GzSoeNature[2]=  _0_KAI_1_HE;
		fa.GzCheckHandle[i].GzSoeNature[3]=  _0_KAI_1_HE;
		fa.GzCheckHandle[i].GzSoeNature[4]=  _0_KAI_1_HE;
		fa.GzCheckHandle[i].GzSoeNature[5]=  _0_KAI_1_HE;
	}
	//4硬件相关的遥信
	for ( i = 0; i < MaxUserYxNum; i++){
		yx_config.config_data[i].Yx.Nature = _0_KAI_1_HE;
		yx_config.config_data[i].Yx.Type = SINGLE;
	}
	for (i = 0; i < DB_YX_NUM; i++){
		yx_config.config_data[DBYX(i)].Yx.Type = COUPLE;
	}
	return 1;
}
#endif 
float Scada::GetFloatMax(float ia,float ib,float ic)
{
	float ret = 0;
	if(ia>ib){
		if(ia>ic){
			ret = ia;
		}else{
			ret = ic;
		}
	}else{
		if(ib>ic){
			ret = ib;
		}else{
			ret = ic;
		}
	}
	return ret;
};

/*
*************************************************************************************
*  @brief:		软时钟,产生秒,分...时间,最小单位为ms
*  @param[in]
*  @param[out]
*  @return:
**************************************************************************************
*/
void Scada::TimeTick(CP56Time2a * T,Uint32 Per)
{
	T->millisecond++;
		if ( T->millisecond >= Per ) {
			T->millisecond = 0;
			T->minute++;
			if ( T->minute >= 60 ) {
				T->minute = 0;
				T->hour++;

				if ( T->hour >= 24 ) {
					T->hour = 0;
					T->day++;

					if ( ( T->month== 1 ) || ( T->month == 3 ) || ( T->month == 7 ) || ( T->month == 8 ) || ( T->month == 10 ) || ( T->month == 12 ) ) {
						if ( T->day>= 32 ) {

							T->day = 1;
							T->month++;

							if ( T->month >= 12 ) {
								T->month = 1;
								T->year++;
							}
						}
					} else if ( (  T->month == 4 ) || (  T->month == 6 ) || (  T->month == 9 ) || (  T->month == 11 ) ) {
						if ( T->day  >= 31 ) {
							T->day  = 1;
						}
					} else if (  T->month == 2 ) {
						if ( ( ( ( T->year % 4 ) == 0 ) && ( ( T->year % 100 ) != 0 ) ) || ( ( T->year % 400 ) == 0 ) ) {
							if ( T->day  >= 30 ) {
								T->day  = 1;
								T->month++;
							}
						} else {
							if ( T->day >= 29 ) {
								T->day = 1;
								T->month++;
							}
						}
					}
				}
			}
		}
}
/**
***********************************************************************
*  @brief		summon how many bit is 1 in Data,and locate which bit is set 
*  @param[in]	KR_UNION Data 
*  @param[out]	BitReg *Reg BitNum and BitLocatonMask 
*  @return  1
*  @note	
*  @see		
***********************************************************************
*/
int16 Scada::GetBitStat(union KR_UNION Data,BitReg *Reg)
{
	Uint16  i = 0;
	Uint16 j = 0;
	union KR_UNION  tmp;
	tmp.all = Data.all;

	for( i = 0;i<64;i++){
		if(CHKBIT(tmp.all.S1,MASKTAB[i])){
			Reg->BitLocationMask[j++]=i;
		}
	}
	for( i = 64;i<128;i++){
		if(CHKBIT(tmp.all.S2,MASKTAB[i])){
			Reg->BitLocationMask[j++]=i;
		}
	}
	for( i = 128;i<(128+MAX_LINE_NUM);i++){
		if(CHKBIT(tmp.all.D1,MASKTAB[i])){
			Reg->BitLocationMask[j++]=i;
		}
	}
	Reg->BitNum = j;
	return 1;
}


/*
 *  90/250
 */



void Scada::SetFistYxSynFlag(Uint16 YxBuffPos,BitReg *YxReg)
{
	if(YxSynFlag.GroupYxFlag==0){//找到首遥信
		YxSynFlag.GroupYxFlag = 1;
		YxSynFlag.FirstYxPos = YxReg->BitLocationMask[0];
		YxSynFlag.StartTimeMark = YxBuff.databuff[YxBuffPos].TimeCnt;
		YxSynFlag.StartTime =YxBuff.databuff[YxBuffPos].time;
		YxSynFlag.LastCn = YxBuff.databuff[YxBuffPos].TimeCnt;;
	}else{
		if((YxBuff.databuff[YxBuffPos].TimeCnt-YxSynFlag.LastCn)>=40){//间距10ms的遥信算是一组；
			YxSynFlag.GroupYxFlag = 1;
			YxSynFlag.FirstYxPos = YxReg->BitLocationMask[0];//同时的记录，时间是一样的，取第一个就可以了；
			YxSynFlag.StartTimeMark = YxBuff.databuff[YxBuffPos].TimeCnt;
			YxSynFlag.StartTime =YxBuff.databuff[YxBuffPos].time;
			YxSynFlag.LastCn = YxBuff.databuff[YxBuffPos].TimeCnt;
		}else{
			YxSynFlag.LastCn = YxBuff.databuff[YxBuffPos].TimeCnt;
		}
	}
}
CP56Time2a Scada::GetYxSynTime(Uint16 YxBuffPos,Uint16 YxPos)
{
	CP56Time2a Ttmp = YxBuff.databuff[YxBuffPos].time;
		if(YxPos!=YxSynFlag.FirstYxPos){//非首个遥信
		 Ttmp = YxSynFlag.StartTime;
		 Uint64 Tdif = YxBuff.databuff[YxBuffPos].TimeCnt-YxSynFlag.StartTimeMark;
		 Uint16 j;
		 YxSynFlag.Absms =Tdif /YXMULTIPLE;
			if ((Tdif%YXMULTIPLE)>=3){
				YxSynFlag.Absms+=1;
			}
			for(j=0;j<YxSynFlag.Absms;j++){
				TimeTick(&Ttmp,60000);
			}
		}
	return Ttmp;
}
void Scada::GetYxDataFromYxBuff(Uint16 YxBuffPos,Uint16 YxPos)
{
		YxTimeFlag.TimeFlag[YxPos].time = GetYxSynTime(YxBuffPos,YxPos);//YxBuff.databuff[YxBuff.CurRead].time;
		YxTimeFlag.TimeFlag[YxPos].TimeCnt = YxBuff.databuff[YxBuffPos].TimeCnt;
		union KR_UNION Yxtmp;
		GetBitData(&Yxtmp,&YxBuff.databuff[YxBuffPos].Yx,YxPos);
		YxBitFun(&YxTimeFlag.Statu,MASKTAB[YxPos],YxPos,CLR_BIT);
		YxTimeFlag.Statu.all.S1 |=Yxtmp.all.S1;
		YxTimeFlag.Statu.all.S2 |=Yxtmp.all.S2;
		YxTimeFlag.Statu.all.D1 |=Yxtmp.all.D1;

}
void Scada::GetOldYxDataFromYxBuff(Uint16 YxBuffPos,Uint16 YxPos)
{
	union KR_UNION Yxtmp;

	GetBitData(&Yxtmp,&YxBuff.databuff[YxBuffPos].OldYx,YxPos);
	YxBitFun(&YxTimeFlag.OldStatu,MASKTAB[YxPos],YxPos,CLR_BIT);
	YxTimeFlag.OldStatu.all.S1 |=Yxtmp.all.S1;
	YxTimeFlag.OldStatu.all.S2 |=Yxtmp.all.S2;
	YxTimeFlag.OldStatu.all.D1 |=Yxtmp.all.D1;
}
void Scada::GetBitData(union KR_UNION *DirYxData,union KR_UNION *SrcYxData,uint16 YxPos)
{
	DirYxData->all.S1 =0;
	DirYxData->all.S2 =0;
	DirYxData->all.D1 =0;
	if(YxPos<64){
		DirYxData->all.S1 = SrcYxData->all.S1&MASKTAB[YxPos];
	}else if (YxPos<128){
		DirYxData->all.S2 = SrcYxData->all.S2&MASKTAB[YxPos];
	}else if (YxPos<192){
		DirYxData->all.D1 = SrcYxData->all.D1&MASKTAB[YxPos];
	}
}

uint16 Scada::YxBitFun(	union KR_UNION *YxData,	Uint64 YxMask,Uint16 YxPos,Uint16 Type)
{
	uint16 ret = 0;
	if(YxPos<64){
		if(Type==CHECK_BIT){
			if(CHKBIT(YxData->all.S1,YxMask)){
				ret = 1;
			}
		}else if (Type==SET_BIT){
			SETBIT(YxData->all.S1,YxMask);
		}else if (Type==CLR_BIT){
			CLRBIT(YxData->all.S1,YxMask);
		}else if (Type==GET_BIT){
			if(CHKBIT(YxData->all.S1,YxMask)){
				ret = 1;
			}else{
				ret = 0;
			}
		}
	}else if (YxPos<128){
		if(Type==CHECK_BIT){
			if(CHKBIT(YxData->all.S2,YxMask)){
				ret = 1;
			}
		}else if (Type==SET_BIT){
			SETBIT(YxData->all.S2,YxMask);
		}else if (Type==CLR_BIT){
			CLRBIT(YxData->all.S2,YxMask);
		}else if (Type==GET_BIT){
			if(CHKBIT(YxData->all.S2,YxMask)){
				ret = 1;
			}else{
				ret = 0;
			}
		}
	}else if (YxPos<192){
			if(Type==CHECK_BIT){
				if(CHKBIT(YxData->all.D1,YxMask)){
					ret = 1;
				}
			}else if (Type==SET_BIT){
				SETBIT(YxData->all.D1,YxMask);
			}else if (Type==CLR_BIT){
				CLRBIT(YxData->all.D1,YxMask);
			}else if (Type==GET_BIT){//双点遥信的位置是固定的（关联的遥信点可以不是固定的）
				ret = (YxData->all.D1>>(YxPos-128)*2)&0x03;
			}
	}
	return ret;
}

void Scada::CheckYxBuff(void)
{
	Uint16 i;
	BitReg YxReg;
	Uint16 YxPos;
	Uint16 j;
	Uint16 YxName;
	Uint16 YxDataId;
	while(YxBuff.CurPrepare!=YxBuff.CurRead){//检测缓存中是否有新的未处理的遥信；
		if(YxBuff.CurRead>=MAX_HDYX_BUF){
			YxBuff.CurRead = 0;
		}
		GetBitStat(YxBuff.databuff[YxBuff.CurRead].ChangeFlag,&YxReg);//统计当前读取的缓存中变化的遥信个数，变化的遥信号。
		for(i = 0;i < YxReg.BitNum; i++){
			YxPos = YxReg.BitLocationMask[i];
			//step 1遥信计时开始
			if(YxBitFun(&YxTimeFlag.Flag,MASKTAB[YxPos],YxPos,CHECK_BIT)==0){
				YxBitFun(&YxTimeFlag.Flag,MASKTAB[YxPos],YxPos,SET_BIT);
				SetFistYxSynFlag(YxBuff.CurRead,&YxReg);
				GetOldYxDataFromYxBuff(YxBuff.CurRead, YxPos);
				GetYxDataFromYxBuff(YxBuff.CurRead, YxPos);
			}else{
				//step2  与STEP1 中的状态 超过防抖时间，则这两次变化 包含一次有效变位，产生SOE，并将第二次变化装载。
				if((YxBuff.databuff[YxBuff.CurRead].TimeCnt-YxTimeFlag.TimeFlag[YxPos].TimeCnt)>=scada_cfg.d_5090.Data*2){
			        YxCurrent[YxPos] = YxBitFun(&YxTimeFlag.Statu,MASKTAB[YxPos],YxPos,GET_BIT);
					for (j = 0; j < yx_config.yx_num; j++){
						YxName = yx_config.configed_yx[j];
						if (yx_config.config_data[YxName].Port == YxPos ){
							YxDataId = yx_config.config_data[YxName].Id;
							yx_config.config_data[YxName].Yx.Data = GetLogicYx(yx_config.config_data[YxName].YxPortP[0] ,yx_config.config_data[YxName].Yx.Type);
							PushSoe(HDYXSOE,YxDataId,yx_config.config_data[YxName].Yx.Data,YxTimeFlag.TimeFlag[YxPos].time);
						}
					}
					SetFistYxSynFlag(YxBuff.CurRead,&YxReg);
					GetOldYxDataFromYxBuff(YxBuff.CurRead, YxPos);
					GetYxDataFromYxBuff(YxBuff.CurRead, YxPos);
				}else{//step 2   与STEP1 中的状态 在防抖时间内，则是抖动，直接丢弃
					union KR_UNION Yxtmp1,Yxtmp2;

					GetBitData(&Yxtmp1,&YxBuff.databuff[YxBuff.CurRead].Yx,YxPos);
					GetBitData(&Yxtmp2,&YxTimeFlag.OldStatu,YxPos);
					if((Yxtmp1.all.S1==Yxtmp2.all.S1)||(Yxtmp1.all.S2==Yxtmp2.all.S2)||(Yxtmp1.all.D1==Yxtmp2.all.D1)){
						YxBitFun(&YxTimeFlag.Flag,MASKTAB[YxPos],YxPos,CLR_BIT);
						if(YxSynFlag.GroupYxFlag==1){
							if(YxSynFlag.FirstYxPos==YxPos){
								YxSynFlag.GroupYxFlag = 0;
							}
						}
					}else{
						if(YxSynFlag.GroupYxFlag==1){
							if(YxSynFlag.FirstYxPos==YxPos){
								YxSynFlag.GroupYxFlag = 0;
							}
						}
						SetFistYxSynFlag(YxBuff.CurRead,&YxReg);
						GetYxDataFromYxBuff(YxBuff.CurRead, YxPos);
					}
				}
			}
		}
		YxBuff.CurRead++;
		if(YxBuff.CurRead>=MAX_HDYX_BUF){
			YxBuff.CurRead = 0;
		}
	}
	if(YxSynFlag.GroupYxFlag==1){//一组遥信结束了
		if((Sys500us-YxSynFlag.LastCn)>=20){//间距10ms的遥信算是一组；
			YxSynFlag.GroupYxFlag = 0;
		}
	}
	if((YxTimeFlag.Flag.all.S1!=0)||(YxTimeFlag.Flag.all.S2!=0)||(YxTimeFlag.Flag.all.D1!=0)){
		GetBitStat(YxTimeFlag.Flag,&YxReg);
		for(i = 0;i<YxReg.BitNum;i++){
			YxPos = YxReg.BitLocationMask[i];
			 if((Sys500us-YxTimeFlag.TimeFlag[YxPos].TimeCnt)>=scada_cfg.d_5090.Data*2){//超过防抖时间 ,变位；
				 YxBitFun(&YxTimeFlag.Flag,MASKTAB[YxPos],YxPos,CLR_BIT);
				 YxCurrent[YxPos] = YxBitFun(&YxTimeFlag.Statu,MASKTAB[YxPos],YxPos,GET_BIT);
				for (j = 0; j < yx_config.yx_num; j++){
					YxName = yx_config.configed_yx[j];
					if (yx_config.config_data[YxName].Port == YxPos ){
						YxDataId = yx_config.config_data[YxName].Id;
						yx_config.config_data[YxName].Yx.Data = GetLogicYx(yx_config.config_data[YxName].YxPortP[0] ,yx_config.config_data[YxName].Yx.Type);
						PushSoe(HDYXSOE,YxDataId,yx_config.config_data[YxName].Yx.Data,YxTimeFlag.TimeFlag[YxPos].time);
					}
				}
			 }
		}
	}

}

/**
*************************************************************************************
*  @brief		毫秒执行操作,被时间终端调用
*  @param[in]
*  @param[out]
*  @return 1
*  @note		对于脉冲型继电器,需要延时后在这里将其关闭
**************************************************************************************
*/
int16 Scada::SoftMsTime()
{
	RelayReturn(clk0.GetSysMs());
	return 1;
}

/*
*************************************************************************************
*  @brief:		对子板在线状态进行检测,当发现插拔板子的时候:
*				1.存入板子异常遥信队列
*				2.如果是要侧板,要读取标定系数.
*  @param[in]
*  @param[out]
*  @return:
*  @note:		一秒钟一次
**************************************************************************************
*/
void Scada::TerCheck(void)
{

	CP56Time2a SoeTime;
	FpgaCheck();
	if(FpgaCfg.InitFlag==0){
		TerFaultStatu.all = TerFaultCheck.all;
		DataBuf.d_6801.Data = TerFaultCheck.all;
		FpgaCfg.InitFlag = 1;
	}
	if(TerFaultCheck.all!=TerFaultStatu.all){
		TerFaultStatu.all = TerFaultCheck.all;
		DataBuf.d_6801.Data = TerFaultCheck.all;
		if (scada_cfg.fault_buffer.CurPrepare>=MAX_FAULT_SOE_NUM){
			scada_cfg.fault_buffer.CurPrepare = 0;
		}
		SoeTime = clk0.GetCP56Time2a(GETSOFTTIME);
		scada_cfg.fault_buffer.fault_data[scada_cfg.fault_buffer.CurPrepare].Data = TerFaultCheck.all;
		scada_cfg.fault_buffer.fault_data[scada_cfg.fault_buffer.CurPrepare].Time  = SoeTime;
		scada_cfg.fault_buffer.CurPrepare++;
		FpgaCfgInit();
		if(CHKBIT(TerFaultCheck.all,0x0000001f)){
			if(TerFaultCheck.bit.yc1==1){
				YcRatioBoardRead(0);
			}
			if(TerFaultCheck.bit.yc2==1){
				YcRatioBoardRead(1);
			}
			if(TerFaultCheck.bit.yc3==1){
				YcRatioBoardRead(2);
			}
			if(TerFaultCheck.bit.yc4==1){
				YcRatioBoardRead(3);
			}
			if(TerFaultCheck.bit.yc5==1){
				YcRatioBoardRead(4);
			}
			XiShuChange();

		}

		//YxTimeSave(scada_cfg.d_5090.Data);
		//终端异常遥信
		if(TerFaultCheck.all==0){
			PushSoe(PROTECTSOE,YX_019C,LOGIC_FEN,SoeTime);
		}else{
			PushSoe(PROTECTSOE,YX_019C,LOGIC_HE,SoeTime);
		}
	}
	//电池管理部分
	//DataType4 d_5207;   ///**蓄电池欠压切除阈值*/
	//DataType2 d_5209;   ///**蓄电池活化充放电间隔时间*/
	//#define BATLOWV (YFJD+1)//电池欠压

	//定时活化
		/*
		电源手册上说:
		电池的活化，当电池长时间处于浮充电状态，应对电池进行活化以免电池极板钝化，
		活化可以由用户CPU控制的继电器把电源的活化端子HK与VG短接一次（不小于0.5秒，
		但端子不应长时间短接，否则电源将失去自动功能）电源进入活化状态，电源关闭输出，电池放电，
		当电池放电至活化完成点时，电源自动启动工作向负载供电并为电池充电；当中途需要提前退出活化时，
		可手动按电源面板上的活化OFF键退出活化，或由用户CPU控制的继电器触点把电源的HG与VG端子短接一次
		（不小于0.5秒）则可提前退出活化。
		*/
/*
	T_tool.StartCount(&scada_cfg.BatActTime,scada_cfg.d_5209.Data*3600000);//分钟转换成秒
	if(T_tool.CheckCount(&scada_cfg.BatActTime)==REACHED){
		if((yx_config.config_data[BAT_AC_SPL].YxPortP[0]==_SYSHE_)){//交流供电时
			DCOWORD DCO_Word;
			DCO_Word.DcoWord.D_S_CS = 0x01;
			DCO_Word.DcoWord.D_S = SCONTROL;
			YkOut(BATTARYACTION,DCO_Word);
		}
	}
	//电池欠压退出；
	if(scada_cfg.YcData.Dcv_4121.Data<scada_cfg.d_5207.Data){
		DCOWORD DCO_Word;
		DCO_Word.DcoWord.D_S_CS = 0x01;
		DCO_Word.DcoWord.D_S = SCONTROL;
		YkOut(BATTARYOFF,DCO_Word);
	}
	*/
}

/*
*************************************************************************************
*  @brief:	产生一条soe记录,将其放入soe_buffer中
*  @param[in]	uint8 User,soe记录可能被多个用户使用,需要区分记录放到哪个用户的soe_buffer中
*  @param[in]	uint16 DataId
*  @param[out]
*  @return:
**************************************************************************************
*/
#pragma CODE_SECTION("CODEINIRAM")
int16 Scada::PushSoe(uint8 User ,uint16 DataId,int8 data,CP56Time2a InTime)
{
	if (DataId==0xffff){
		return 1;
	}
	if (scada_cfg.soe_buffer[User].CurPrepare>=MAX_SOE_NUM){
		scada_cfg.soe_buffer[User].CurPrepare = 0;
	}
	scada_cfg.soe_buffer[User].soe_data[scada_cfg.soe_buffer[User].CurPrepare].DataId = DataId;
	scada_cfg.soe_buffer[User].soe_data[scada_cfg.soe_buffer[User].CurPrepare].Data = data;
	scada_cfg.soe_buffer[User].soe_data[scada_cfg.soe_buffer[User].CurPrepare].Time  = InTime;// clk0.GetCP56Time2a(GETSOFTTIME);
	scada_cfg.soe_buffer[User].CurPrepare++;

	return 1;
}
/*
*************************************************************************************
*  @brief:向系统其它线程发送Yx变位信息.
*		变位信息来自于soe_buffer,fault_buffer(终端故障信息).
*		当Buffer中有未发送的信息,则获取该信息,并向Varm类发送
*  @param[in]none
*  @param[out]none
*  @return:1
**************************************************************************************
*/
int16 Scada::SendYxMsg(void)
{
	uint8 j;
	uint16 curready;
	DataType1 data;
	DataType3 fault_data;
	Data<BaseDataType> yxtmp;
	for ( j = 0;j < MAX_USER_NUM;j++){
		while(scada_cfg.soe_buffer[j].CurPrepare!=scada_cfg.soe_buffer[j].CurRead){
			if (scada_cfg.soe_buffer[j].CurRead>=MAX_SOE_NUM){
				scada_cfg.soe_buffer[j].CurRead = 0;
			}
			curready = scada_cfg.soe_buffer[j].CurRead;
			data.Data = scada_cfg.soe_buffer[j].soe_data[curready].Data;
			yxtmp.Data = &data;
			msg.SendMsg(TASK_VARM,MSG_YX,scada_cfg.soe_buffer[j].soe_data[curready].DataId,&data,&scada_cfg.soe_buffer[j].soe_data[curready].Time);
			user_data.SetData(scada_cfg.soe_buffer[j].soe_data[curready].DataId,&yxtmp);//更新表
			scada_cfg.soe_buffer[j].CurRead++;

		}
	}
	while(scada_cfg.fault_buffer.CurPrepare!=scada_cfg.fault_buffer.CurRead){
		if (scada_cfg.fault_buffer.CurRead>=MAX_FAULT_SOE_NUM){
			scada_cfg.fault_buffer.CurRead = 0;
		}
		curready = scada_cfg.fault_buffer.CurRead;
		fault_data.Data = scada_cfg.fault_buffer.fault_data[curready].Data;
		yxtmp.Data = &fault_data;
		msg.SendMsg(TASK_VARM,MSG_PARA,PARA_6801,&fault_data,&scada_cfg.fault_buffer.fault_data[curready].Time);
		scada_cfg.fault_buffer.CurRead++;

	}
	return 1;
}
int g_cmd=0;
/**
*************************************************************************************  
*  @brief  运行模块。
*  @param [in] 
*  @param [out]  
*  @return  成功，返回1，否则返回0。
**************************************************************************************
*/	
int16 Scada::Run()
{
	Init();
#if ENABLE_FA
	fa.Init();
#endif
#if LINK_TO_ARM
	Semaphore_pend(varm_sem, BIOS_WAIT_FOREVER);
#endif
	doConfig();
	msg.Init();
	StartIr();
	UART_init(UART0,115200);
	while(1){
		if(scada_cfg.d_5213.Data==1){
			ManSet();
		}
		sleep(10);
		CheckYxBuff();
		Calculate();
		T_tool.StartCount(&scada_cfg.TerCheckTime,1000);
		if(T_tool.CheckCount(&scada_cfg.TerCheckTime)==REACHED){
			TerCheck();
			CheckPtCt();
		}
		do_test(g_cmd);
	}
}


/*
*************************************************************************************
*  @brief		search yc_id in configed_cal_yc table(which from YCMsgSet),if it is in table 
*  @param[in]
*  @param[out]
*  @return
*		1 on success;0 fail 
**************************************************************************************
*/
int Scada::is_yc_need_cal(int yc_id){
	int i;
	for(i=0;i<yc_config.configed_need_yc_num;i++){
		if(yc_config.configed_need_yc[i]==yc_id)
			return 1;
	}
	return 0;
}


/**
*************************************************************************************
*  @brief  
*  @param[in]
*  @param[out]
*  @return
*  @note 
**************************************************************************************
*/
int Scada::find_yc_by_channel(int channel){
	int i;
	int flag=0;
	for(i=0;MAX_YC_NUM;i++){
		if(yc_config.config_data[i].port == channel){
			flag=1;
			break;
		}
	}
	if(flag)
		return yc_config.config_data[i].id;
	else
		return -1;
}
/**
*************************************************************************************
*  @brief 根据ycid 返回所配置的端口
*  @param[in] yc_id  input ycid
*  @return  channel on success,-1 fail
*  @note 
**************************************************************************************
*/
int Scada::find_channel_by_yc_id(int yc_id){
	int port;
	int ret;
	ret=-1;
	if(yc_id>MAX_YC_NUM||yc_id<0)
		return ret;

	port=scada.yc_config.config_data[yc_id].port;
	if(port==NO_YC)
		return ret;
	ret=port;
	return ret;
}
/*
*************************************************************************************
*  @brief	config yc port ,set config_data,configed_yc,configed_yc_port array   
*  @note:
*
	ycdata_t	config_data[MAX_YC_NUM];	//端口配置参数,下标索引号即是YcId,每个配置项包括YcId对应的端口,YcId从0开始,无配置则端口号为0xffff. 数据来源于模拟量端口配置表0x5080
	uint16		configed_yc[MAX_YC_NUM];	//配置上端口的遥测量表,内容是YcId
	uint8		configed_yc_num;			//配置上端口的遥测量数量.根据该数组中的YcId去查找config_data中相应YcId的端口,再对该端口进行计算.
	uint16		configed_yc_port[CHANNEL_NUM];	//配置上遥测量的端口表,内容是port号;
	uint8		configed_yc_port_num;			//实际使用的端口数量；
**************************************************************************************
*/
void Scada::YcPortConfig(void)
{
	uint16	i,j;
	int		val;
	int		yc_id;
	int		yc_type;
	uint16	index;
	uint16	port_pos;
	uint16	port_flag;
	YCMsgSet		*p_yc_msg_set;
	SingleDigitSet	*p_yc_port_set ;

	memset(&yc_config,0xff,sizeof(yc_config));
	p_yc_port_set = &scada_cfg.d_5080[0];
	index=0;
	//1. save d_8050 data into config_data table 
	for ( i =0; i < YC_PORT_PARA_NUM; i++){
		for ( j = 0; j<SINGLE_DIGIT_SET_LEN; j++){
			yc_id=p_yc_port_set[i].Data.Define[j].Id - YC_DATA_ID_BEGIN;
			if(yc_id>=MAX_YC_NUM||yc_id<0){//dataId异常
				continue;
			}
			//make default 
			yc_config.config_data[yc_id].port = NO_YC;
			yc_config.config_data[yc_id].id=NO_YC;
			yc_config.config_data[yc_id].d_from=0;
			yc_config.config_data[yc_id].type=0;
			if ((p_yc_port_set[i].Data.Define[j].Port<=0)||(p_yc_port_set[i].Data.Define[j].Port>CHANNEL_NUM)){//PORT异常
				continue;
			}
			//now data is valid and set config_data 
			yc_config.config_data[yc_id].id=yc_id;
			if(p_yc_port_set[i].Data.Define[j].Port!=NO_YC){//配置端口
				yc_config.config_data[yc_id].port = p_yc_port_set[i].Data.Define[j].Port - 1;//因为配置参数端口号是从1开始的
				yc_config.config_data[yc_id].d_from=1;
				yc_type=yc_id%PER_LINE_YC_NUM;
				switch(yc_type){//高于16回线的yc量没有配置端口,所以可以不考虑16回线之后的遥测量
						case UA1:
						case UB1:
						case UC1:
						case UAB1:
						case UCB1:
						case U01:
							yc_config.config_data[yc_id].type=TYPE_U;
							break;
						case IA11:
						case IB11:
						case IC11:
						case I01:
						case IA12:
						case IB12:
						case IC12:
							yc_config.config_data[yc_id].type=TYPE_I;
							break;
						case P1:
						case Q1:
						case S1:
							yc_config.config_data[yc_id].type=TYPE_PQS;
							break;
				}
				index++;
			}
		}
	}
	yc_config.configed_yc_num=index;
	
	//2. make table of configed_yc and configed_yc_port 
	index = 0;
	port_pos = 0;
	port_flag = 0;
	for ( i = 0; i < MAX_YC_NUM; i++){//遍历整个遥测量端口配置表
		if (yc_config.config_data[i].port != NO_YC){//该遥测量配置了端口
			yc_id=yc_config.config_data[i].id;
			//设置有端口配置的Yc表,下标是索引号,内容是YcId,最多MAX_YC_NUM
			yc_config.configed_yc[index] = yc_id;//record this  
			index++;
			//设置有端口配置的port表,下标是索引号,内容是端口,最多CHANNEL_NUM
			port_flag = 0;
			for ( j = 0 ;j<port_pos; j++){//遍历端口表
				if(yc_config.configed_yc_port[j]==yc_config.config_data[i].port){//如果该遥测量的端口已经在端口配置表中
					port_flag = 1;
				}
			}
			if(port_flag!=1){//该遥测量的端口不在端口配置记录表中,是新记录,则添加该端口到记录表中
				yc_config.configed_yc_port[port_pos] = yc_config.config_data[i].port;
				port_pos++;
			}
		}
	}
	yc_config.configed_yc_num = index;
	yc_config.configed_yc_port_num = port_pos;

	//3. make table of configed_need_yc 
	p_yc_msg_set=&scada_cfg.d_5098[0];
	index=0;
	for(i=0;i<20;i++){
		for(j=0;j<50;j++){
			yc_id=p_yc_msg_set[i].Data.Define[j].DataId - YC_DATA_ID_BEGIN;
			if(yc_id>MAX_YC_NUM||yc_id<0)
				continue;
			val=p_yc_msg_set[i].Data.Define[j].MsgId;
			if(val!=NO_YC){
				yc_config.configed_need_yc[index]=yc_id;
				index++;
			}
		}
	}
	yc_config.configed_need_yc_num=index;

	//4. 对于其他一些可能不能直接采集的模拟量进行配置
	//零序电压,电流配置(采集or合成)
	for(i=0;i<MAX_LINE_NUM;i++){
		if(is_yc_need_cal(U0(i))){//需要计算
			if(find_channel_by_yc_id(U0(i))==-1){//但未配置端口
				yc_config.config_data[U0(i)].d_from=2;//设置为合成标志
			}
		}
		if(is_yc_need_cal(I0(i))){//需要计算
			if(find_channel_by_yc_id(I0(i))==-1){//但未配置端口
				yc_config.config_data[I0(i)].d_from=2;//设置为合成标志
			}
		}
	}
	//p,q,s,freq,有专门代码计算
	//5. 配置计算频率的端口,如果配置了电压,则认为可以计算频率
	uint16 port;
	for( i =0; i<MAX_LINE_NUM;i++){
		port = NO_YC;
		if(yc_config.config_data[UA(i)].port!=NO_YC){
			port = yc_config.config_data[UA(i)].port;
		}else if(yc_config.config_data[UB(i)].port!=NO_YC){
			port = yc_config.config_data[UB(i)].port;
		}else if(yc_config.config_data[UC(i)].port!=NO_YC){
			port = yc_config.config_data[UC(i)].port;
		}else if(yc_config.config_data[UAB(i)].port!=NO_YC){
			port = yc_config.config_data[UAB(i)].port;
		}else if(yc_config.config_data[UCB(i)].port!=NO_YC){
			port = yc_config.config_data[UCB(i)].port;
		}
		yc_config.configed_freq_port[i] = port;
	}
	uint16 ports[MAX_LINE_NUM];//避免重复计算频率
	cal_freq_num=uniq_int_array(yc_config.configed_freq_port,MAX_LINE_NUM,NO_YC,ports);
	for(i=0;i<cal_freq_num;i++){
		cal_freq[i].port=ports[i];
	}
	YcRatioRead();
}
#if 0
void Scada::YcPortCfgParaInit(void)
{
	uint8 i,j;
	uint16 k,h;
	uint16 line;
	SingleDigitSet *YcPortP = &scada_cfg.d_5080[0];

	for ( i = 0 ;i < YC_PORT_PARA_NUM;i++){
		for (j = 0;j < SINGLE_DIGIT_SET_LEN;j++){
			YcPortP[i].Data.Define[j].Port = NO_YC;
		}
	}
	k = 5;
	h = 21;
	i = 0;
	j = 0;
	for ( line = 0 ; line < MAX_LINE_NUM; line++){

		if (line==4){
			k=32;
		}
		if (line==8){
			k=48;
		}
		if (line==12){
			k=64;
		}
if(line<=7){
		YcPortP[i].Data.Define[j].Port = 1;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}

		YcPortP[i].Data.Define[j].Port = 2;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}

		YcPortP[i].Data.Define[j].Port = 3;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
}else{
	YcPortP[i].Data.Define[j].Port = 17;
	j++;
	if(j>=SINGLE_DIGIT_SET_LEN){
		i++;
		j = 0;
	}

	YcPortP[i].Data.Define[j].Port = 18;
	j++;
	if(j>=SINGLE_DIGIT_SET_LEN){
		i++;
		j = 0;
	}

	YcPortP[i].Data.Define[j].Port = 19;
	j++;
	if(j>=SINGLE_DIGIT_SET_LEN){
		i++;
		j = 0;
	}
}
		YcPortP[i].Data.Define[j].Port = NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}

		YcPortP[i].Data.Define[j].Port = NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		if(line<=7){
				YcPortP[i].Data.Define[j].Port = 4;
				j++;
				if(j>=SINGLE_DIGIT_SET_LEN){
					i++;
					j = 0;
				}
		}else{
			YcPortP[i].Data.Define[j].Port = 20;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}
		}
		if(line <= 2){
			YcPortP[i].Data.Define[j].Port =k++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}

			YcPortP[i].Data.Define[j].Port =k++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}

			YcPortP[i].Data.Define[j].Port = k++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}
		}
		else {

			YcPortP[i].Data.Define[j].Port =h++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}
			if(h==30){
				h=33;
			}
			if(h==45){
				h=49;
			}
			if(h==61){
				h=65;
			}
			if(h==77){
				//h==65;
			}
			YcPortP[i].Data.Define[j].Port =h++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}
			if(h==30){
				h=33;
			}
			if(h==45){
				h=49;
			}
			if(h==61){
				h=65;
			}
			if(h==77){
				//h==65;
			}
			YcPortP[i].Data.Define[j].Port = h++;
			j++;
			if(j>=SINGLE_DIGIT_SET_LEN){
				i++;
				j = 0;
			}
			if(h==30){
				h=33;
			}
			if(h==45){
				h=49;
			}
			if(h==61){
				h=65;
			}
			if(h==77){
				//h==65;
			}
		}


		YcPortP[i].Data.Define[j].Port =NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YcPortP[i].Data.Define[j].Port = NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YcPortP[i].Data.Define[j].Port =NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
		YcPortP[i].Data.Define[j].Port = NO_YC;
		j++;
		if(j>=SINGLE_DIGIT_SET_LEN){
			i++;
			j = 0;
		}
	}
}
#endif 
#if 0
void Scada::YkPortCfgInit(void)
{
	uint16 i;
	for ( i = 0; i < DB_DIGIT_SET_LEN; i++){
		scada_cfg.d_508A.Data.Define[i].ClosePort = NO_YK;
		scada_cfg.d_508A.Data.Define[i].OpenPort = NO_YK;

	}

	for ( i = 0;i < MAX_LINE_NUM; i++){
		scada_cfg.d_508A.Data.Define[i].ClosePort = 1+i*2;//合出口
		scada_cfg.d_508A.Data.Define[i].OpenPort = 2+i*2;//分出口
		scada_cfg.d_508A.Data.Define[i].Id = YK_6001+i;
	}

	scada_cfg.d_508A.Data.Define[WINGMOTOR].ClosePort = 1+i*2;//合出口
	scada_cfg.d_508A.Data.Define[WINGMOTOR].OpenPort = 2+i*2;//分出口
	scada_cfg.d_508A.Data.Define[WINGMOTOR].Id = 0x6065;
	i++;
	scada_cfg.d_508A.Data.Define[WATERMOTOR].ClosePort = 1+i*2;//合出口
	scada_cfg.d_508A.Data.Define[WATERMOTOR].OpenPort = 2+i*2;//分出口
	scada_cfg.d_508A.Data.Define[WATERMOTOR].Id = 0x6066;

	scada_cfg.d_505D.Data = 0xffffffff;//软压板 全部使能
	for(i=0;i<5;i++)
		scada_cfg.d_505E[i].Data = 0xffffffff;//分回线远方就地端口配置,默认没有
	scada_cfg.d_5063.Data = 0;//分回线远方就地不允许
	for(i=0;i<3;i++)
		scada_cfg.d_5064[i].Data = 0x0;//0 PULSE;1 LEVEL ;出口继电器1~16类型,两位一个继电器

}
#endif 
void Scada::YkPortConfig(void)
{
	uint16 i,j;
		
	DbDigitSet *p_yk_port_set;//开关输出量的硬件端口配置
	p_yk_port_set=&scada_cfg.d_508A;
	for ( i = 0; i < DB_DIGIT_SET_LEN; i++){
		yk_config.config_data[i].OpenPort = NO_YK;
		yk_config.config_data[i].ClosePort= NO_YK;
		yk_config.config_data[i].Id=p_yk_port_set->Data.Define[i].Id;//begin 0x6001
		if(p_yk_port_set->Data.Define[i].ClosePort!=NO_YK)
			yk_config.config_data[i].ClosePort = scada_cfg.d_508A.Data.Define[i].ClosePort-1 ;
		if(p_yk_port_set->Data.Define[i].OpenPort!=NO_YK)
			yk_config.config_data[i].OpenPort =  scada_cfg.d_508A.Data.Define[i].OpenPort-1 ;
	}
	//fixed 
	yk_config.config_data[BATTARYACTION].ClosePort = 36;
	yk_config.config_data[BATTARYACTION].OpenPort =  37;
	yk_config.config_data[BATTARYACTION].Id = YK_6064;
	yk_config.config_data[BATTARYOFF].ClosePort = 38;
	yk_config.config_data[BATTARYOFF].OpenPort =  0xff;
	yk_config.config_data[BATTARYOFF].Id = 0x6067;//自定义，上位机没有配置；
	//继电器出口类型配置
	for( i = 0 ,j = 0;  i < 32; i+=2,j++){
		RelayStatu.RelayReg[j].LevelType = (scada_cfg.d_5064[0].Data>>i)&0x03;
	}
	for( i = 0 ,j = 16;  i < 32; i+=2,j++){
		RelayStatu.RelayReg[j].LevelType = (scada_cfg.d_5064[1].Data>>i)&0x03;
	}
	for( i = 0 ,j = 32;  i < 32; i+=2,j++){
		RelayStatu.RelayReg[j].LevelType = (scada_cfg.d_5064[2].Data>>i)&0x03;
	}
	for (i = 0; i <MAX_OUT_NUM ; i++){
		RelayStatu.RelayReg[i].Td = scada_cfg.d_5093.Data;
	}
	//电池管理的继电器 脉冲输出 并且时间大于0.5秒
	RelayStatu.RelayReg[yk_config.config_data[BATTARYACTION].ClosePort].Td = 1000;
	RelayStatu.RelayReg[yk_config.config_data[BATTARYACTION].ClosePort].LevelType = PULSE;
	RelayStatu.RelayReg[yk_config.config_data[BATTARYACTION].OpenPort].Td= 1000;
	RelayStatu.RelayReg[yk_config.config_data[BATTARYACTION].OpenPort].LevelType= PULSE;
	RelayStatu.RelayReg[yk_config.config_data[BATTARYOFF].ClosePort].Td= 5000;
	RelayStatu.RelayReg[yk_config.config_data[BATTARYOFF].ClosePort].LevelType= PULSE;
	//故障检测
	for (i = 0; i < MAX_USER_YK_NUM; i++){
			if ((yk_config.config_data[i].ClosePort<8)&&(yk_config.config_data[i].Id<=YK_6010)){
				 SetParaMask(YXSTATUMASK+IO_BOARD1,SET_BIT);
			}else if ((yk_config.config_data[i].ClosePort<16)&&(yk_config.config_data[i].Id<=YK_6010)){
				 SetParaMask(YXSTATUMASK+IO_BOARD2,SET_BIT);
			}else if ((yk_config.config_data[i].ClosePort<24)&&(yk_config.config_data[i].Id<=YK_6010)){
				 SetParaMask(YXSTATUMASK+IO_BOARD3,SET_BIT);
			}else if ((yk_config.config_data[i].ClosePort<32)&&(yk_config.config_data[i].Id<=YK_6010)){
				 SetParaMask(YXSTATUMASK+IO_BOARD4,SET_BIT);
			}
	}

}
/*
*************************************************************************************
*  @brief  获取各个谐波的模值
*  @param[in]
*  @param[out]
*  @return   第0点，第32点为直流量
**************************************************************************************
*/
void Scada:: GetModulus(void)
{
	uint16	channel;
	int16	port1;
	int16	port2;
	int16	port3;
	int		flag;

	uint16	i,j;

	for ( i = 0 ; i < yc_config.configed_yc_port_num; i++){
		channel = yc_config.configed_yc_port[i];
		if(channel!=NO_YC){
			DSPF_sp_fftSPxSP (SAMP_NUM, (float *)&AD_BUF_scada_cal[channel][0], &ptr_w32[0], (float *)&fft_out_scada[channel][0], brev, 2, 0, SAMP_NUM);
			for (j = DCCOMPONENT; j < XIEBOLEN ; j++){//基波到15次谐波的有效值
				channel_modules[channel][j]=sqrtsp(fft_out_scada[channel][j].rel*fft_out_scada[channel][j].rel+ fft_out_scada[channel][j].img*fft_out_scada[channel][j].img);
			}
		}
	}
	//如果零序电流没有配置,这里采用合成方式计算
	for(i=0;i<MAX_LINE_NUM;i++){
		flag=0;
		if(yc_config.config_data[I0(i)].d_from == 2){//I0 is composed
			port1=find_channel_by_yc_id(IA1(i));
			port2=find_channel_by_yc_id(IB1(i));
			port3=find_channel_by_yc_id(IC1(i));
			channel=find_channel_by_yc_id(I0(i));
			if(port1==-1||port2==-1||port3==-1)
				continue;
			flag=1;
		}else if(yc_config.config_data[U0(i)].d_from == 2 ){
			port1=find_channel_by_yc_id(UA(i));
			port2=find_channel_by_yc_id(UB(i));
			port3=find_channel_by_yc_id(UC(i));
			channel=find_channel_by_yc_id(U0(i));
			if(port1==-1||port2==-1||port3==-1)
			flag=1;
		}
		if(flag){
			for( j = 0 ; j < SAMP_NUM; j++){
				fft_out_scada[channel][j].img = fft_out_scada[port1][j].img
											+fft_out_scada[port2][j].img
											+fft_out_scada[port3][j].img;
				fft_out_scada[channel][j].rel = fft_out_scada[port1][j].rel
											+fft_out_scada[port2][j].rel
											+fft_out_scada[port3][j].rel;
			}
			for (j = DCCOMPONENT; j < XIEBOLEN  ; j++){//计算各次谐波
				channel_modules[channel][j]=sqrtsp(fft_out_scada[channel][j].rel *fft_out_scada[channel][j].rel + fft_out_scada[channel][j].img*fft_out_scada[channel][j].img);
			}
		}
	}
}
/*
*************************************************************************************
*  @brief		有效值计算;将各次的谐波分量相加再开方
*  @param[in]   均方根
*  @param[out]
*  @return
**************************************************************************************
*/

float  Scada:: TotalRms(float *src , uint8 len)
{
	unsigned int i;
	float SumOfSquares = 0;
	float Radication = 0;
	for( i = 0; i < len; i++){
		SumOfSquares = SumOfSquares +  ( ( * ( src+ i ) ) * ( * ( src + i ) ) ) ;
	}
	Radication = sqrtsp(SumOfSquares);//*DEVXISHU;
	return Radication;
}
/*
*********************************************************************************************************
** 函数名称 ：cal_horizen()
** 函数功能 ：对于(x1,y1) (x2,y2)决定的直线，求该直线过X轴的坐标值
** 入口参数 ： 决定直线的两点
** 出口参数 ：该直线过X轴的坐标值
*********************************************************************************************************/
float cal_horizen(int16 x1,float y1,int16 x2,float y2)
{
	return (y1*((x1-x2)/(y2-y1)) + x1);
}

/**
*************************************************************************************
*  @brief	Get every line freq  
*  @param[in] void 
*  @param[out] FreqReg
*  @return  void 
*  @note In this program, use Vol physical channel to calculate freq,
*  so different line maybe has same port to calculate freq.
*  avoid this,i first uniq freq port in YcPortConfig function,and then 
*  only calculate uniq channel ,finally set line Freq of every line. 
**************************************************************************************
*/
void Scada::GetFreq(void)
{
	uint16 i,j;
	uint16 port;
	SIGRCD freq_req;
	
	//1. init freq_req
	freq_req.samp_t = SAMP_T;
	freq_req.samp_Freq = 50;
	freq_req.samp_adjcnt = 0;
	freq_req.Freq = 50;
	//2. do real calculate
	for( i =0; i<cal_freq_num;i++){
		port = cal_freq[i].port;
		if( port!=NO_YC){
			CalculateFreq(&AD_BUF_scada[port][0],_3Half_NUM,&freq_req);
		}
	}
	//3. now set every line freq req 
	for(i=0;i<MAX_LINE_NUM;i++){
		for(j=0;j<cal_freq_num;j++){
			if(yc_config.configed_freq_port[i]==cal_freq[j].port){
				FreqReg[i].Freq=cal_freq[j].freq;
			}
		}
	}
}
/*
*************************************************************************************
*  @brief  计算频率
*  @param[in] src: input data array  
*  @param[in] len: input data array len   
*  @param[in] SIGRCD freq_req: input ad acquire parameter   
*  @param[out] SIGRCD *freq_req->freq; 
*  @return 0 on success,-1 error 
**************************************************************************************
*/
int  Scada:: CalculateFreq(int16 *src , uint8 len,SIGRCD *freq_reg)
{
	float index[4];
	int i;
	int j = 0;
	float FreqTmp;
	for(i=0;(i<(len-1)) && (j<2);i++) {
		if((src[i]<=0 && src[i+1]>0))
			index[j++] = cal_horizen(i,src[i],i+1,src[i+1]);
	}
	if(j<2) {
		j=0;
		for(i=0;(i<(len-1)) && (j<2);i++) {
			if((src[i]>0 && src[i+1]<=0))
				index[j++] = cal_horizen(i,src[i],i+1,src[i+1]);
		}
	}
	if(j==2) {
		freq_reg->T = freq_reg->samp_t *(index[1] - index[0]);
	//	freq_reg->Freq = 1000.0/freq_reg->T ;
		FreqTmp= 1000.0/freq_reg->T ;
		if((FreqTmp>44)&&(FreqTmp<56)){
			freq_reg->difFreq = freq_reg->Freq - freq_reg->samp_Freq;
			if((freq_reg->difFreq>0.1)||(freq_reg->difFreq<-0.1)){
				freq_reg->samp_adjcnt++;
			}else{
				freq_reg->samp_adjcnt = 0;
			}
			if (freq_reg->samp_adjcnt>=100){
				freq_reg->samp_adjcnt = 0;
				freq_reg->Freq = FreqTmp;
				freq_reg->samp_Freq = FreqTmp ;
				freq_reg->samp_t = (1000.0/freq_reg->samp_Freq)/SAMP_NUM;
			}
		}else{
			freq_reg->samp_adjcnt++;
			if (freq_reg->samp_adjcnt>=10){
				freq_reg->Freq = 50;
				freq_reg->samp_adjcnt = 0;
				freq_reg->samp_Freq = 50 ;
				freq_reg->samp_t = (1000.0/freq_reg->samp_Freq)/SAMP_NUM;
			}
		}
		return 0;
	}
	return -1;
}
/*
*************************************************************************************
*  @brief  谐波含有率
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************/
harmonicdata_t Scada:: GetHarmonicRmsAndRatio(float BaseHarmonicRms,float *src)
{
	harmonicdata_t HmncData;
	HmncData.rms = TotalRms(src,1);
	HmncData.rario = HmncData.rms/BaseHarmonicRms;
	if (HmncData.rario > 1.001){
		HmncData.rario = 1.0000;
	}
	HmncData.rario = HmncData.rario *100.0;
	return HmncData ;
}
/*
*************************************************************************************
*  @brief  有效值计算
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada:: GetRms(void)
{
	uint16 yc_id;
	uint16 channel;
	uint16 line;
	uint16 yc_name;
	uint8 i;

	DataType4 *Pyc1=&scada_cfg.YcData.Ua1_4001;
	memset(&scada_cfg.YcData,0,sizeof(scada_cfg.YcData));
	for (i = 0; i < yc_config.configed_yc_num; i++ ){//key
		yc_id = yc_config.configed_yc[i];//只有在配置表中的遥测项才去计算
		channel = yc_config.config_data[yc_id].port;
		line =GetLineNoByYcId(yc_id);
		yc_name = yc_id%PER_LINE_YC_NUM;
		Pyc1[line*PER_LINE_YC_NUM + yc_name].Data = (TotalRms(&channel_modules[channel][BASEHARMONIC],xiebo_len))*YcRatio[line].PortRatio[yc_name];
	}
	for( i =0; i<MAX_LINE_NUM;i++){
		Pyc1[Freq(i)].Data=FreqReg[i].Freq;
	}
#if 0
	Data<BaseDataType> data;
//#define YC_4122  0x4122   //环境温度
	user_data.GetData(YC_4122,&data);
	memcpy(&scada_cfg.YcData.EnvTemp_4122,data.Data,sizeof(DataType4));
	//scada_cfg.YcData.EnvTemp_4122.Data = tttmp;
//#define YC_4123  0x4123   //终端温度
	user_data.GetData(YC_4123,&data);
	memcpy(&scada_cfg.YcData.TerTemp_4123,data.Data,sizeof(DataType4));
	//scada_cfg.YcData.TerTemp_4123.Data = tttmp;
//#define YC_4124  0x4124   //电缆温度
	user_data.GetData(YC_4124,&data);
	memcpy(&scada_cfg.YcData.LineTemp_4124,data.Data,sizeof(DataType4));
	//scada_cfg.YcData.LineTemp_4124.Data = tttmp;
//#define YC_4A01  0x4A01  // GPS经度
	user_data.GetData(YC_4A01,&data);
	memcpy(&scada_cfg.YcData.d_4A01,data.Data,sizeof(DataType4));
//#define YC_4A02  0x4A02  //GPS纬度
	user_data.GetData(YC_4A02,&data);
	memcpy(&scada_cfg.YcData.d_4A02,data.Data,sizeof(DataType4));
#endif
/*
#define YC_4121  0x4121   //直流电源电压
#define YC_4125  0x4125   //直流量1
#define YC_4126  0x4126   //直流量2   DcRation
4401 相位差,用于重合闸,单位°
4402 电压差,用于重合闸
*/
	scada_cfg.YcData.DC1_4125.Data = DcRation[0][scada_cfg.DcPara[0].DcType].K*GetAverage(&AD_BUF_scada[13][0] ,MAXN)+DcRation[0][scada_cfg.DcPara[0].DcType].B;
	scada_cfg.YcData.DC2_4126.Data = DcRation[1][scada_cfg.DcPara[1].DcType].K*GetAverage(&AD_BUF_scada[14][0] ,MAXN)+DcRation[1][scada_cfg.DcPara[1].DcType].B;
	scada_cfg.difangle_4401.Data = ((GetYcAngle(scada_cfg.DiffVolName[0],scada_cfg.DiffVolName[1]))/PI)*180.0;
	scada_cfg.difvol_4402.Data = scada_cfg.DiffVolP[0]->Data - scada_cfg.DiffVolP[1]->Data;
	if(	scada_cfg.difvol_4402.Data<0){
		scada_cfg.difvol_4402.Data = -1.0*scada_cfg.difvol_4402.Data;
	}
}

/**
*************************************************************************************  
*  @brief		:获得两个向量基波分量的相位差
*				原理是计算两个向量的基波相位,然后相减
*  @param[in]	:uint8 YcName1,YcName2	,两个向量名
*  @param[out]	:see return
*  @return		:相位差(弧度)
**************************************************************************************
*/
float Scada::GetYcAngle(uint8 YcName1,uint8 YcName2)
{
	SP_COMPLEX Yc1,Yc2;
	float rel;
	uint16 Port1,Port2;

	Port1 = yc_config.config_data[YcName1].port;
	Port2 = yc_config.config_data[YcName2].port;

	if((Port1>CHANNEL_NUM)||(Port2>CHANNEL_NUM)){
		return 0;
	}

	Yc1 =fft_out_scada[Port1][BASEHARMONIC];
	Yc2 = fft_out_scada[Port2][BASEHARMONIC];
	if((channel_modules[Port1][BASEHARMONIC]<100)||(channel_modules[Port2][BASEHARMONIC]<100)){
		return 0;
	}
	rel = GetCplAngle(Yc1) - GetCplAngle(Yc2);
	return rel;
}
float Scada::GetCplAngle(SP_COMPLEX CplData)
{
	float result;
    if ( fabs(CplData.rel) <=0.01 ) {
        if ( CplData.img > 0 ) {
            result = PI / 2.0; //90°
        } else {
            result = -PI / 2.0; // -90°
        }
    } else {
        if ( fabs(CplData.img) <=0.01 ) {
            if ( CplData.rel > 0 ) {
                result = 0;
            } else {
                result = PI;
            }
        } else {
            result = atan2sp ( CplData.img,CplData.rel );
        }
    }
	return result;
}
/*
*************************************************************************************
*  @brief 旋转矢量
*  @param[in] SP_COMPLEX *src ,the point to source 
*  @param[in] uint16 len ,total number of src 
*  @param[in] float offsetangle ,angle to rotate 
*  @param[out] SP_COMPLEX *dst ,the result point 
*  @return void
**************************************************************************************
*/
//逆时针旋转为正角度反之为负 ，如果是线电压，最终旋转成第一相位UAB, 第二项为UCB
//如果是相电压
void Scada:: PhaseChange(SP_COMPLEX *dst, SP_COMPLEX *src ,  uint16 len ,float offsetangle )
{
 uint8 i;
 for (i = 1; i < len ; i++){
	 dst[i]  = ComplexRotation(src[i] ,offsetangle* (i));
 }
}

SP_COMPLEX Scada:: ComplexRotation(SP_COMPLEX Input,float Angle)
{
	SP_COMPLEX Outdata;
	float cosa,sina;
	cosa = cossp(Angle);
	sina = sinsp(Angle);
	Outdata.rel = Input.rel*cosa - Input.img*sina;
	Outdata.img = Input.img*cosa + Input.rel*sina;
	return Outdata;

}
//cos(a-b) = cosa * cosb  + sina * sinb
//sin(a-b) = sina * cosb - cosa *sinb
angelvalue_t Scada::CosSinz1z2(SP_COMPLEX U,SP_COMPLEX I)//float UREAL,float UIMG,float IREAL,float IIMG,uint8 flag)
{

	float cosa, cosb,sina,sinb;
	SP_COMPLEX Utmp,Itmp;
	angelvalue_t sincosvalue = {0,0};
	float umod,imod;
	float  data;
	Utmp = U;
	Itmp = I;
	data  = Utmp.rel*Utmp.rel + Utmp.img*Utmp.img;
	umod =	sqrtsp(data);
	data = Itmp.rel*Itmp.rel +Itmp.img*Itmp.img;
	imod = sqrtsp(data);
	//if (umod<10.0){
	//	return sincosvalue;
	//}
	//if (imod<10.0){
	//	return sincosvalue;
	//}
	if (umod<0.10){
		return sincosvalue;
	}
	if (imod<0.10){
		return sincosvalue;
	}
	cosa  = (Utmp.rel/umod);
	cosb =  (Itmp.rel/imod);
	sina =  (Utmp.img/umod);
	sinb = (Itmp.img/imod);
//cos(a-b) = cosa * cosb  + sina * sinb
		sincosvalue.cos_a = cosa * cosb  + sina * sinb;
//sin(a-b) = sina * cosb - cosa *sinb
		sincosvalue.sin_a = sina * cosb - cosa *sinb;
	if ((sincosvalue.cos_a>1.1)||(sincosvalue.cos_a<-1.1)){
		sincosvalue.cos_a	= 0;
	}
	if ((sincosvalue.sin_a>1.1)||(sincosvalue.sin_a<-1.1)){
		sincosvalue.sin_a	= 0;
	}
	return sincosvalue ;
}

/**
*************************************************************************************
*  @brief		acquire p,q,s  
*  @param[in]	Uname: U id 
*  @param[in]	Iname: I id 
*  @param[out]
*  @return		struct power_t ,include p,q,s
*  @note		return value is not multipled by ratio   
**************************************************************************************
*/
power_t  Scada:: GetPower(uint8 Uname,uint8 Iname)
{
	uint8 j;
	SP_COMPLEX U;
	SP_COMPLEX I;
	angelvalue_t sincosvalue;
	power_t Power[XIEBOLEN];
	power_t TotalP;
	uint16 PortU;
	uint16 PortI;

	TotalP.p = 0;
	TotalP.q = 0;
	TotalP.s = 0;
	PortU = yc_config.config_data[Uname].port;
	PortI = yc_config.config_data[Iname].port;
	if(PortU == NO_YC || PortI == NO_YC){
		return TotalP;
	}
	for(j = 1; j<xiebo_len+1; j++){
		U = fft_out_scada[PortU][j];
		I = fft_out_scada[PortI][j];
		sincosvalue = CosSinz1z2(U,I);
		Power[j-1].s = channel_modules[PortU][j]*channel_modules[PortI][j];
		Power[j-1].p = Power[j-1].s*sincosvalue.cos_a;
		Power[j-1].q = Power[j-1].s*sincosvalue.sin_a;
	}
	for(j = 0; j<xiebo_len; j++){
		TotalP.p+=Power[j].p;
		TotalP.q+=Power[j].q;
		TotalP.s+=Power[j].s;
	}
	return TotalP;
}
/*
*************************************************************************************
*  @brief  功率值计算
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/

void Scada:: GetPQS(void)
{
	power_t powertmp[3];
	uint8 flag[3] = {0};
	uint8 i;
	uint8 phasenum;
	float p,q,s;
	DataType4	*p_yc= NULL;
	power_t Power[3];
	for ( i = 0; i < MAX_LINE_NUM; i++){
		if(scada_cfg.d_508B.Data<Pt_2){//都配置了采样通道，有数据可采
			powertmp[0] = GetPower(UA(i),IA1(i));
			flag[0] = 1;
			powertmp[1] = GetPower(UB(i),IB1(i));
			flag[1] = 1;
			powertmp[2] = GetPower(UC(i),IC1(i));
			flag[2] = 1;
		}else if(scada_cfg.d_508B.Data<=Pt_Ucb){
			powertmp[0]  = GetPower(UAB(i),IA1(i));
			flag[0] = 1;
			flag[1] = 0;
			powertmp[2] = GetPower(UCB(i),IC1(i));
			flag[2] = 1;
		}
		for (phasenum = 0; phasenum< 3; phasenum++){
			if (flag[phasenum] == 1){
					Power[phasenum].p = \
						(powertmp[phasenum].p *YcRatio[LINE(i)].PowerAngleRatio[phasenum].cos_a
						+powertmp[phasenum].q*YcRatio[LINE(i)].PowerAngleRatio[phasenum].sin_a)\
						*YcRatio[LINE(i)].PowerLineRatio[phasenum];//有功功率
					Power[phasenum].q = \
						(powertmp[phasenum].q *YcRatio[LINE(i)].PowerAngleRatio[phasenum].cos_a
						-powertmp[phasenum].p*YcRatio[LINE(i)].PowerAngleRatio[phasenum].sin_a)\
						*YcRatio[LINE(i)].PowerLineRatio[phasenum];	//无功功率
				}else{
					Power[phasenum].p = 0;
					Power[phasenum].q = 0;
					Power[phasenum].s = 0;
			}
		}
		p_yc = (DataType4 *)&scada_cfg.YcData.Ua1_4001;
		p_yc[P(i)].Data = Power[0].p+Power[1].p+Power[2].p;	///<总有功功率
		p = p_yc[P(i)].Data;
		p_yc[Q(i)].Data = Power[0].q+Power[1].q+Power[2].q;	///<总无功功率
		q = p_yc[Q(i)].Data;

		s = sqrtsp(p*p +q*q);
		p_yc[S(i)].Data = sqrtsp(p*p +q*q);	//总视在功率
		p_yc[Factor(i)].Data= (p/s)*100.0;	//总功率因数
		if(	p_yc[Factor(i)].Data<0.01){
			p_yc[Factor(i)].Data= 0;
		}
	}
}

/*
*************************************************************************************
*  @brief 根据数据ID号索引死区值
*  @param[in]
*  @param[out]
*  @return 死区值
**************************************************************************************
*/
float Scada::GetDead(int32 DataId)
{


return 1;
/*
	if
	(
		((DataId>=0x4001)&&(DataId<=0x4006))
		||
		((DataId>=0x40FA)&&(DataId<=0x40FF))
		||(DataId==0x4402)//压差
	)
	{
		return scada_cfg.Dead.Data.DeadU;
	}
	else if
	(
		((DataId>=0x4007)&&(DataId<=0x400d))
		||
		((DataId>=0x4100)&&(DataId<=0x4106))
	)
	{
		return scada_cfg.Dead.Data.DeadI;
	}
	else if
	(
		  (DataId==0x400e)
		||(DataId==0x400f)
		||(DataId==0x4011)
		||(DataId==0x4012)
		||(DataId==0x4014)
		||(DataId==0x4015)
		||(DataId==0x4017)
		||(DataId==0x4018)
		||(DataId==0x4019)
		||(DataId==0x4107)
		||(DataId==0x4108)
		||(DataId==0x410a)
		||(DataId==0x410b)
		||(DataId==0x410d)
		||(DataId==0x410e)
		||(DataId==0x4110)
		||(DataId==0x4111)
		||(DataId==0x4112)
	)
	{
		return scada_cfg.Dead.Data.DeadP;
	}
	else if
	(
		((DataId>=0x401C)&&(DataId<=0x402D))
		||
		((DataId>=0x4066)&&(DataId<=0x4077))
		||
		((DataId>=0x40B0)&&(DataId<=0x40C1))
		||
		((DataId>=0x4115)&&(DataId<=0x4126))
		||
		((DataId>=0x415F)&&(DataId<=0x4170))
		||
		((DataId>=0x41A9)&&(DataId<=0x41BA))
	)
	{
		return scada_cfg.Dead.Data.DeadHarmoicValueU;
	}
	else if
	(
		((DataId>=0x402D)&&(DataId<=0x403f))
		||
		((DataId>=0x4078)&&(DataId<=0x4089))
		||
		((DataId>=0x40C2)&&(DataId<=0x40d3))
		||
		((DataId>=0x4127)&&(DataId<=0x4138))
		||
		((DataId>=0x4171)&&(DataId<=0x4182))
		||
		((DataId>=0x41BB)&&(DataId<=0x41cc))
	)
	{
		return scada_cfg.Dead.Data.DeadHarmoicValueI;
	}

	else if
	(
		((DataId>=0x4040)&&(DataId<=0x4065))
		||
		((DataId>=0x408A)&&(DataId<=0x40af))
		||
		((DataId>=0x40D4)&&(DataId<=0x40f9))
		||
		((DataId>=0x4139)&&(DataId<=0x415e))
		||
		((DataId>=0x4183)&&(DataId<=0x41A8))
		||
		((DataId>=0x41CD)&&(DataId<=0x41f2))
	)
	{
		return scada_cfg.Dead.Data.DeadHarmoicCount;
	}
	else if
	(
		(DataId==0x4114)
		||(DataId==0x401b)
	)
	{
		return scada_cfg.Dead.Data.DeadFreq;
	}
	else if
	(
		(DataId==0x4401)
	)
	{
		return scada_cfg.Dead.Data.DeadAngle;
	}
	else if(DataId==0x4606)
	{
		return scada_cfg.Dead.Data.DeadTemp;
	}
	else if (DataId==0x4607)
	{
		return scada_cfg.Dead.Data.DeadHumidity;
	}else {
		return scada_cfg.Dead.Data.cos;
	}

	*/
}
/*
*************************************************************************************
*  @brief		:比较两个浮点数
*  @param[in]	float cmp1 第一个数
*  @param[in]	float cmp2 第二个数
*  @param[out]
*  @return		:0:相等;1 cmp1>cmp2;-1 cmp1<cmp2
*  @note		:equal means the in the smallest unit of 0.00001
**************************************************************************************
*/
//extern Data<DataType4> YcData[];
int16 Scada::FloatCompare ( float cmp1, float cmp2 )
{
	#define EPSINON  0.00001

    if ( ( cmp1 >= ( cmp2 - EPSINON ) ) && ( cmp1 <= ( cmp2 + EPSINON ) ) ) {
        return 0;
    } else if (  cmp1 > ( cmp2 + EPSINON ) ) {
        return 1;
    } else {
        return -1;
    }
}
/**
*************************************************************************************  
*  @brief		:判断两个数的差的绝对值是否超过设定值
*  @param[in]	:float value1
*  @param[in]	:float value2
*  @param[in]	:float db ,设定值
*  @param[out]
*  @return		:1 如果不相等;0 如果相等
**************************************************************************************
*/
int16 Scada::IsOverDeadband ( float value1, float value2, float db )
{
    short ret;
    float fluctuations = fabs ( value1 - value2 );
    //ret = float_compare (fluctuations, db * fabs(value1));//百分比比较,注意value 为负数时
	ret = FloatCompare (fluctuations, db );//基值比较
    if ( ret > 0 ) {
        return 1;
    } else {
        return 0;
    }
}

/*
*************************************************************************************
*  @brief  越死区值计算
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada:: CheckChangeYc(void)
{	uint16  i ;

	Data<BaseDataType> yctmp;
	Data<BaseDataType> *PycTab;
	DataType4 *Pyc = NULL;
	CHANGE_YC_REG *p_change_yc = NULL;
	CP56Time2a Time;
    Pyc  = &scada_cfg.YcData.Ua1_4001;//scada 遥测表
    p_change_yc = &change_yc.Ua1_4001;
    memcpy(&DataBuf.d_4001,&scada_cfg.YcData,sizeof(scada_cfg.YcData));
	for (i = 0; i < MAX_YC_NUM; i++){
		yctmp.Data = &Pyc[i];
		PycTab = user_data.RecallData(YCTAB,i);
		if (IsOverDeadband(Pyc[i].Data,p_change_yc[i].data,p_change_yc[i].dead[0])==1){
			if(T_tool.CheckCount(&p_change_yc[i].YcDeadTime)==REACHED){
				p_change_yc[i].data= Pyc[i].Data;
				user_data.SetData(PycTab->Id,&yctmp);//更新遥测表
				Time = clk0.GetCP56Time2a(GETSOFTTIME);
				msg.SendMsg(TASK_VARM,MSG_YC,PycTab->Id,&Pyc[i],&Time);
			}else{
				T_tool.StartCount(&p_change_yc[i].YcDeadTime,scada_cfg.Dead.Data.DeadTime);//默认10秒；
			}
		}else{
			T_tool.StopCount(&p_change_yc[i].YcDeadTime);
		}
	}
}

/*
*************************************************************************************
*  @brief  软遥信检测,包括断线，断相，越限告警；
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/

void Scada:: ChecksoftYx(void)
{
	uint16  i ;
	Data<BaseDataType> yxtmp;
	DataType1 data;
	CP56Time2a Time;
	DataType4 *YcP = (DataType4 *)&scada_cfg.YcData.Ua1_4001;
	for(i = 0; i < MAX_LINE_NUM; i++){
		MaxVol[i].Data = GetFloatMax(GetFloatMax(YcP[UA(i)].Data,YcP[UB(i)].Data,YcP[UC(i)].Data),
		YcP[UAB(i)].Data,YcP[UCB(i)].Data);
	}
	for (i = 0; i < MAX_YUEXIAN_SOFT_YX_NUM; i++){
		if(scada_cfg.YueXianYx[i].SrcYcP!=NULL){
			if (scada_cfg.YueXianYx[i].LimitType == SHANG_XIAN){
				if(scada_cfg.YueXianYx[i].SrcYcP->Data > scada_cfg.YueXianYx[i].LimitParaP->Data.Limit){//越上限
					T_tool.StopCount(&scada_cfg.YueXianYx[i].Tcn[1]);
					if(T_tool.CheckCount(&scada_cfg.YueXianYx[i].Tcn[0])==REACHED){
						scada_cfg.YueXianYx[i].DataId.Data = _SYSHE_;
						data.Data = GetLogicYx(_SYSHE_,SINGLE);
						Time = clk0.GetCP56Time2a(GETSOFTTIME);
						yxtmp.Data = &data;
						msg.SendMsg(TASK_VARM,MSG_YX,scada_cfg.YueXianYx[i].DataId.Id,&data,&Time);
						user_data.SetData(scada_cfg.YueXianYx[i].DataId.Id,&yxtmp);
						scada_cfg.YueXianYx[i].ChangeFlag = 1;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==0){
						T_tool.StartCount(&scada_cfg.YueXianYx[i].Tcn[0],scada_cfg.YueXianYx[i].LimitParaP->Data.ContinueTime);
					}
				}else if(scada_cfg.YueXianYx[i].SrcYcP->Data < scada_cfg.YueXianYx[i].LimitParaP->Data.Recover){//越上限恢复
					T_tool.StopCount(&scada_cfg.YueXianYx[i].Tcn[0]);
					if(T_tool.CheckCount(&scada_cfg.YueXianYx[i].Tcn[1])==REACHED){
						scada_cfg.YueXianYx[i].DataId.Data = _SYSFEN_;
						data.Data = GetLogicYx(_SYSFEN_,SINGLE);
						Time = clk0.GetCP56Time2a(GETSOFTTIME);
						yxtmp.Data = &data;
						msg.SendMsg(TASK_VARM,MSG_YX,scada_cfg.YueXianYx[i].DataId.Id,&data,&Time);
						user_data.SetData(scada_cfg.YueXianYx[i].DataId.Id,&yxtmp);
						scada_cfg.YueXianYx[i].ChangeFlag = 0;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==1){
						T_tool.StartCount(&scada_cfg.YueXianYx[i].Tcn[1],scada_cfg.YueXianYx[i].LimitParaP->Data.ContinueTime);
					}
				}
			}else if(scada_cfg.YueXianYx[i].LimitType == XIA_XIAN){
				if(scada_cfg.YueXianYx[i].SrcYcP->Data < scada_cfg.YueXianYx[i].LimitParaP->Data.Limit){//越下限
					T_tool.StopCount(&scada_cfg.YueXianYx[i].Tcn[1]);
					if(T_tool.CheckCount(&scada_cfg.YueXianYx[i].Tcn[0])==REACHED){
						scada_cfg.YueXianYx[i].DataId.Data = _SYSHE_;
						data.Data = GetLogicYx(_SYSHE_,SINGLE);
						Time = clk0.GetCP56Time2a(GETSOFTTIME);
						yxtmp.Data = &data;
						msg.SendMsg(TASK_VARM,MSG_YX,scada_cfg.YueXianYx[i].DataId.Id,&data,&Time);
						user_data.SetData(scada_cfg.YueXianYx[i].DataId.Id,&yxtmp);
						scada_cfg.YueXianYx[i].ChangeFlag=1;

					}else if(scada_cfg.YueXianYx[i].ChangeFlag==0){
						T_tool.StartCount(&scada_cfg.YueXianYx[i].Tcn[0],scada_cfg.YueXianYx[i].LimitParaP->Data.ContinueTime);
					}
				}else if(scada_cfg.YueXianYx[i].SrcYcP->Data > scada_cfg.YueXianYx[i].LimitParaP->Data.Recover){//越下限恢复
					T_tool.StopCount(&scada_cfg.YueXianYx[i].Tcn[0]);
					if(T_tool.CheckCount(&scada_cfg.YueXianYx[i].Tcn[1])==REACHED){
						scada_cfg.YueXianYx[i].DataId.Data=_SYSFEN_;
						data.Data = GetLogicYx(_SYSFEN_,SINGLE);
						Time = clk0.GetCP56Time2a(GETSOFTTIME);
						yxtmp.Data = &data;
						msg.SendMsg(TASK_VARM,MSG_YX,scada_cfg.YueXianYx[i].DataId.Id,&data,&Time);
						user_data.SetData(scada_cfg.YueXianYx[i].DataId.Id,&yxtmp);
						scada_cfg.YueXianYx[i].ChangeFlag=0;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==1){
						T_tool.StartCount(&scada_cfg.YueXianYx[i].Tcn[1],scada_cfg.YueXianYx[i].LimitParaP->Data.ContinueTime);
					}
				}
			}
		}
	}
}
void Scada::CheckPtCt(void)
{
	uint16 i;
	uint16 j;
	Data<BaseDataType> yxtmp;
	DataType1 data;
	CP56Time2a Time;
	for ( i = 0;i < MAX_LINE_NUM; i++){
		for ( j = 0;j < 3;j++){
			if (PtCtReg[i].SrcUa[j].SrcP!=NULL){
				if(PtCtReg[i].SrcIa[j].SrcP->Data>PtCtReg[i].StartI){//电流大于启动电流
					if(PtCtReg[i].SrcUa[j].SrcP->Data<PtCtReg[i].CriticalU){//电压小于零界电压
				 		T_tool.StartCount(&PtCtReg[i].PtT[0],PtCtReg[i].limitT);
						if (T_tool.CheckCount(&PtCtReg[i].PtT[0])==REACHED){
								PtCtReg[i].PtSoeId.Data = _SYSHE_;
								data.Data = GetLogicYx(_SYSHE_,SINGLE);
								Time = clk0.GetCP56Time2a(GETSOFTTIME);
								yxtmp.Data = &data;
								msg.SendMsg(TASK_VARM,MSG_YX,PtCtReg[i].PtSoeId.Id,&data,&Time);
								user_data.SetData(PtCtReg[i].PtSoeId.Id,&yxtmp);
							}
							T_tool.StopCount(&PtCtReg[i].PtT[1]);
							PtCtReg[i].PtStatu[j] = _SYSHE_;
					}else {//恢复
						     PtCtReg[i].PtStatu[j] = _SYSFEN_;
						     //三相都恢复才是真的恢复
						 	if ((PtCtReg[i].PtStatu[PHASE_A]==_SYSFEN_)&&\
								(PtCtReg[i].PtStatu[PHASE_B]==_SYSFEN_)\
								&&(PtCtReg[i].PtStatu[PHASE_C]==_SYSFEN_))
							{
						 		T_tool.StartCount(&PtCtReg[i].PtT[1],PtCtReg[i].limitT);
								if (T_tool.CheckCount(&PtCtReg[i].PtT[1])==REACHED){
									PtCtReg[i].PtSoeId.Data = _SYSFEN_;
									data.Data = GetLogicYx(_SYSFEN_,SINGLE);
									Time = clk0.GetCP56Time2a(GETSOFTTIME);
									yxtmp.Data = &data;
									msg.SendMsg(TASK_VARM,MSG_YX,PtCtReg[i].PtSoeId.Id,&data,&Time);
									user_data.SetData(PtCtReg[i].PtSoeId.Id,&yxtmp);
								}
							}
					}
				}
			}else{
				T_tool.StopCount(&PtCtReg[i].PtT[0]);
				T_tool.StopCount(&PtCtReg[i].PtT[1]);
			}
			if(PtCtReg[i].SrcUa[j].SrcP->Data>PtCtReg[i].CriticalU){//电压大于零界电压
				if(PtCtReg[i].SrcIa[j].SrcP->Data<PtCtReg[i].StartI){//电流小于启动电流
			 		T_tool.StartCount(&PtCtReg[i].CtT[0],PtCtReg[i].limitT);
					if (T_tool.CheckCount(&PtCtReg[i].CtT[0])==REACHED){
							PtCtReg[i].CtSoeId.Data = _SYSHE_;
							data.Data = GetLogicYx(_SYSHE_,SINGLE);
							Time = clk0.GetCP56Time2a(GETSOFTTIME);
							yxtmp.Data = &data;
							msg.SendMsg(TASK_VARM,MSG_YX,PtCtReg[i].CtSoeId.Id,&data,&Time);
							user_data.SetData(PtCtReg[i].CtSoeId.Id,&yxtmp);
					}
					T_tool.StopCount(&PtCtReg[i].CtT[1]);
					PtCtReg[i].CtStatu[j]=_SYSHE_;
				}else {//恢复
					PtCtReg[i].CtStatu[j]=_SYSFEN_;
					if((PtCtReg[i].CtStatu[PHASE_A]==_SYSFEN_)&&\
							(PtCtReg[i].CtStatu[PHASE_B]==_SYSFEN_)&&\
							(PtCtReg[i].CtStatu[PHASE_C]==_SYSFEN_)){
				 		T_tool.StartCount(&PtCtReg[i].CtT[1],PtCtReg[i].limitT);
						if (T_tool.CheckCount(&PtCtReg[i].CtT[1])==REACHED){
							PtCtReg[i].CtSoeId.Data = _SYSFEN_;
							data.Data = GetLogicYx(_SYSFEN_,SINGLE);
							Time = clk0.GetCP56Time2a(GETSOFTTIME);
							yxtmp.Data = &data;
							msg.SendMsg(TASK_VARM,MSG_YX,PtCtReg[i].CtSoeId.Id,&data,&Time);
							user_data.SetData(PtCtReg[i].CtSoeId.Id,&yxtmp);
						}
					}
				}
			}else{
				T_tool.StopCount(&PtCtReg[i].CtT[0]);
				T_tool.StopCount(&PtCtReg[i].CtT[1]);
			}

		}
	}
}
/**
*************************************************************************************
*  @brief  实时计算模拟量数据，更新数据表.
*  @param[in] 
*  @param[out]  
*  @return  成功，返回1，否则返回0。
**************************************************************************************
*/ 		
int16 Scada::Calculate(){
	if(need_calculate==0)
		return 0;
	need_calculate=0;
	AdCopy();
	GetModulus();
	GetFreq();
	if (scada_cfg.StartClbrt==BDSTART){
		T_tool.StartCount(&scada_cfg.ClbrtTime,10000);//默认10秒；
		Calibration();
	}else {
		GetRms();
		GetPQS();
		CheckChangeYc();
		if(scada_cfg.d_5055.Data.EnableYCEvent==ENABLE){
			ChecksoftYx();
		}
		SendYxMsg();
	}
	return 1;
}

/*
*************************************************************************************
*  @brief:	当执行遥控开合命令时,判断当前开关位置是否正确.
*			1.远方就地开关应当处于远方状态,如果处于就地状态,返回错误.(如果配置了分回线的远方就地,就判断分回线的远方就地开关状态);
*			2.检查遥控软压板是否投入,如果没有投入,则返回错误;
*			3.如果合闸,则判断开关分位置遥信有效,合位置遥信无效,储能位置遥信有效;
*			4.如果分闸,则判断开关合位置遥信有效,分位置遥信无效,储能位置遥信有效;
*  @param[in]:	int16 break_no,开关号,从0开始;
*  @param[in]:	DCOWORD DCO_Word,开合命令,单点命令:0开1合;双点命令:1开2合
*  @param[out]:	none
*  @return:		1表示位置正确,-1表示位置错误;
**************************************************************************************
*/
int16 Scada::BreakYkStateChk(int16 break_no,DCOWORD DCO_Word)
{
	uint8 hewz;
	uint8 fenwz;
    switch ( break_no ) {
		//对于断路器开合,需要判断状态
	    case BREAKER1:
	    case BREAKER2:
	    case BREAKER3:
	    case BREAKER4:
	    case BREAKER5:
	    case BREAKER6:
	    case BREAKER7:
	    case BREAKER8:
	    case BREAKER9:
	    case BREAKER10:
	    case BREAKER11:
	    case BREAKER12:
	    case BREAKER13:
	    case BREAKER14:
	    case BREAKER15:
	    case BREAKER16:
			hewz = HE(break_no);
			fenwz = FEN(break_no);
			break;
		//对于以下控制,认为无需判断状态
		case 	BATTARYACTION:	//电池活化遥控
		case 	WINGMOTOR:		//公共遥控1(开合,风机启停)
		case 	WATERMOTOR:		//公共遥控2(开合,水泵启停
		case 	BATTARYOFF:		//电池退出遥控
			return 1;
	    default:
	        return -1;
    }
	if(YfJdCheck(break_no)==-1)//远方就地开关位置判断,-1表示就地,1表示远方
		return -1;

    if(CHKBIT(scada_cfg.d_505D.Data,(0x00000001<<break_no))==0){//遥控软压板判断
    	return -1;
    }
	if( (( DCO_Word.DcoWord.D_S_CS == 0x01 )&&( DCO_Word.DcoWord.D_S == SCONTROL ))||
		(( DCO_Word.DcoWord.D_S_CS == 0x02 )&&( DCO_Word.DcoWord.D_S == DCONTROL ))){//合
		if(	//遥控合闸，要求断路器是 分 状态，且储能?,储能位要加上吗
			((yx_config.config_data[hewz].Yx.Data==LOGIC_FEN))//合位置断开
			&&
			((yx_config.config_data[fenwz].Yx.Data==LOGIC_HE))//分位置闭合
			//&&
			//((yx_config.config_data[cnwz].Yx.Data==CHUNENG))//储能 储能
		){
			return 1;
		}else{
			return -1;
		}

	} else if (	(( DCO_Word.DcoWord.D_S_CS == 0x00 )&&( DCO_Word.DcoWord.D_S == SCONTROL ))||
				(( DCO_Word.DcoWord.D_S_CS == 0x01 )&&( DCO_Word.DcoWord.D_S == DCONTROL ))	){//分
		if(//遥控跳闸，要求断路器是 合 状态
			((yx_config.config_data[hewz].Yx.Data==LOGIC_HE))//合位置闭合
			&&
			((yx_config.config_data[fenwz].Yx.Data==LOGIC_FEN))//分位置断开
		){
			return 1;
		}else{
			return -1;
		}
	}
	return -1;
}


int16 Scada::GetYkFpgaNum(uint8 YkPort)
{
	return YkPort/2;
}

int16 Scada::CheckNoVol(void)
{
	int16 rel;
	if((scada_cfg.DiffVolP[0]->Data > ((scada_cfg.d_5054.Data.StartU*NOMINAL_U)/100.0))
			&&
		(scada_cfg.DiffVolP[1]->Data > ((scada_cfg.d_5054.Data.StartU*NOMINAL_U)/100.0))
	)
	{
		rel = -1;
	}else {
		rel = 1;
	}
	return rel;
}
/**
*************************************************************************************  
*  @brief		:检查相位差
*  @param[in]
*  @param[out]
*  @return 
**************************************************************************************
*/
int16 Scada::CheckSyn(void)
{
	int16 rel;
	rel = 1;
	if (scada_cfg.difangle_4401.Data>0){
		if(scada_cfg.difangle_4401.Data>scada_cfg.DiffPhase){
			rel = -1;
		}
	}else{
		if((-1.0*scada_cfg.difangle_4401.Data)>scada_cfg.DiffPhase){
			rel = -1;
		}
	}
	return rel;
}

/*
*************************************************************************************
*  @brief:		重合闸检查.
*				当重合闸时,有几种方式
*				不检:0
*				检无压:1
*				检同期:2
*				检无压+检同期:3
*				检无压不成功自动转入检同期:4
*  @param[in]	uint8 CkTp	重合闸方式
*  @param[in]	uint8 Enable是否允许重合闸检查,如果不允许,则直接返回成功
*  @param[out]
*  @return:		success 1,error -1
**************************************************************************************
*/

int16 Scada::BreakReCloceCheck(uint8 CkTp,uint8 Enable)
{
	int16 rel;
	rel=-1;
	if(Enable == 0){
		return 1;
	}
	switch (CkTp)
	{
		case NOCHECK:
			rel = 1;
			break;
		case CKNOVOL://两侧都有压时返回无效
			rel = CheckNoVol();
			break;
		case CKSYN:
			rel = CheckSyn();
			break;
		case CKNOVOLTOSYN://并的关系
		case CKNOVOLSYN://并的关系
			if (CheckNoVol()!=1){//检无压不成功自动进入检同期
				rel = CheckSyn();
			}else {
				rel = 1;
			}
			break;
		default:
			break;
	}
	return rel;
}
/*
	DataType3 d_505E;   //分回线远方就地开关设置（1~4回）
	DataType3 d_505F;   //分回线远方就地开关设置（5~8回）
	DataType3 d_5060;   //分回线远方就地开关设置（9~12回）
	DataType3 d_5061;   //分回线远方就地开关设置（13~16回）
	DataType3 d_5062;   //分回线远方就地开关设置（17~20回）
 */
/*
*************************************************************************************
*  @brief:		对于分回线的远方就地开关位置进行判断
*  @param[in]	:Uint16 LineNum 回线号
*  @param[out]
*  @return:		就地-1;远方1
*  @todo		Judge direct from Physical ?
**************************************************************************************
*/
int8 Scada::YfJdCheck(Uint16 LineNum)
{

	uint16 x,y;
	int8 ret = -1;
	uint8 yx_no;
	if(scada_cfg.d_5063.Data==0){//分回线远方就地不允许
		yx_no=YFJD;//公共的远方就地信号

		if(yx_config.config_data[yx_no].Yx.Data==LOGIC_HE)//1 YFJD有信号,表示处于就地状态
			ret= -1;
		else
			ret= 1;
		
	}else{//分回线远方就地允许
		DataType3 *datap = &scada_cfg.d_505E[0];
		x = LineNum/4 ;
		y = LineNum%4 ;
		yx_no = datap[x].Data>>(8*y)&0xff;
		if(yx_no==0xff){//未配置
			goto end;
		}
		yx_no -= 1;
		if(yx_no<MAX_YX_PORT_NUM){
			if(yx_config.config_data[yx_no].Yx.Data ==LOGIC_HE){//就地位置直接退出
				ret= -1;
			}else{
				ret= 1;
			}
		}
	}
end:
	return ret;
}
typedef struct _data{
	int no;
	char name [10];
}DATA;
int compare1(const void *a,const void *b){
	DATA *a1,*b1;
	a1=(DATA*)a;
	b1=(DATA*)b;
	return a1->no>b1->no;
}
DATA data[3]={
	{
		1,"a"
	},
	{
		2,"b"
	},
	{
		3,"c"
	},
};
int16 Scada::YkOut(int16 break_no,DCOWORD DCO_Word)
{
	uint8 YkhePort;
	uint8 YkfenPort;
	YkhePort = yk_config.config_data[break_no].ClosePort;
	YkfenPort = yk_config.config_data[break_no].OpenPort;
	if( (( DCO_Word.DcoWord.D_S_CS == 0x01 )&&( DCO_Word.DcoWord.D_S == SCONTROL ))||
		(( DCO_Word.DcoWord.D_S_CS == 0x02 )&&( DCO_Word.DcoWord.D_S == DCONTROL ))
		)
	{//合
		 RelayOut(YkhePort,SET);

	}else if (
			(( DCO_Word.DcoWord.D_S_CS == 0x00 )&&( DCO_Word.DcoWord.D_S == SCONTROL ))||
			(( DCO_Word.DcoWord.D_S_CS == 0x01 )&&( DCO_Word.DcoWord.D_S == DCONTROL ))
			)
	{//跳
		 RelayOut(YkfenPort,CLR);
	}
	return 1;
}

/**
*************************************************************************************
*  @brief		根据指令执行遥控命令.
*  @param[in]	int16 break_no ,从0开始
*  @param[in]	DCOWORD DCO_Word,遥控命令的控制字
*  @param[out]
*  @return  1
**************************************************************************************
*/
//接收的 是DCO_Word ,返回的是传送原因
int16 Scada::ControlYk(int16 break_no,DCOWORD DCO_Word)
{
	uint16 Id;
	DCOWORD RetData;
	DataType1 data;
	volatile	uint8 minute;
	CP56Time2a Time;
	RetData = DCO_Word;
	if((DCO_Word.data==0x92)||(DCO_Word.data==0x82)){
		   RetData=DCO_Word;
	}
    if (( DCO_Word.DcoWord.S_E == SELECT )) {//选择
		if(BreakYkStateChk(break_no,DCO_Word)==1){
	    	RetData = DCO_Word;
		}else{//未选择上
			RetData.DcoWord.D_S_CS+=2;
		}
    }else if(( DCO_Word.DcoWord.S_E == EXECUTE )&&(BreakYkStateChk(break_no,DCO_Word)==1)){//执行
    	YkOut( break_no, DCO_Word);
    }
    Time = clk0.GetCP56Time2a(GETSOFTTIME);
    data.Data = RetData.data;
    Id = 	scada_cfg.d_508A.Data.Define[break_no].Id;
    msg.SendMsg(TASK_VARM,MSG_YK,Id,&data,&Time);
    return 1;
}
/*可记录断路器每次切断故障电流的水平、
动作持续时间（从出口动作命令发出到对应断路器遥信变位）
、动作次数，用于可以监视断路器触头受电腐蚀的程度以及断路器的机械性能
*/
int16 Scada::BreakRecord(int16 BreakId,uint8 RecordStep)
{
uint8 fenwz = 0;
DataType1 data;
uint32 dataId;
CP56Time2a Time;
   if(RecordStep==BRKRCDSTART){
	   scada_cfg.BrkPro[BreakId].step = BRKRCDSTART;
   }else if(scada_cfg.BrkPro[BreakId].step == BRKRCDCPL){
		 return -1;
	}

	switch ( BreakId )
 	{
	case 0://断路器1
		fenwz = FEN1;
		break;
	case 1://断路器2
		fenwz = FEN2;
		break;
	case 0x65://电机
		return 1;
	default:
		return -1;
	}
	if(scada_cfg.BrkPro[BreakId].step == BRKRCDSTART){
	DataType4 *YcP = (DataType4 *)&scada_cfg.YcData.Ua1_4001;
	scada_cfg.BrkPro[BreakId].BreakProSoe.ia = YcP[IA1(BreakId)].Data;//scada_cfg.YcData.ia11_4007.Data;
	scada_cfg.BrkPro[BreakId].BreakProSoe.ib = YcP[IB1(BreakId)].Data;//scada_cfg.YcData.ib11_4008.Data;
	scada_cfg.BrkPro[BreakId].BreakProSoe.ic = YcP[IC1(BreakId)].Data;//scada_cfg.YcData.ic11_4009.Data;
	scada_cfg.BrkPro[BreakId].BreakProSoe.cnt++;
	scada_cfg.BrkPro[BreakId].StartTime = clk0.GetSysMs();
	T_tool.StopCount(&scada_cfg.BrkPro[BreakId].Tcnt);
	T_tool.StartCount(&scada_cfg.BrkPro[BreakId].Tcnt,1000);//1秒
	scada_cfg.BrkPro[BreakId].step = BRKRCDCHECK;
	}else if(scada_cfg.BrkPro[BreakId].step == BRKRCDCHECK) {
		if(T_tool.CheckCount(&scada_cfg.BrkPro[BreakId].Tcnt)!=1){
			if (yx_config.config_data[fenwz].Yx.Data==FEN_FEN){
				scada_cfg.BrkPro[BreakId].EndTime  = clk0.GetSysMs();
				scada_cfg.BrkPro[BreakId].BreakProSoe.Time = scada_cfg.BrkPro[BreakId].EndTime - scada_cfg.BrkPro[BreakId].StartTime ;
				scada_cfg.BrkPro[BreakId].step = BRKRCDCPL;
			}
		}else{//断路器异常
			dataId = 0x0014+BreakId*0x1a;
			data.Data = 0x01;
			Time = clk0.GetCP56Time2a(GETSOFTTIME);
			msg.SendMsg(TASK_VARM,MSG_YX,dataId,&data,&Time);
			scada_cfg.BrkPro[BreakId].step = BRKRCDCPL;
		}
	}
	return 1;
}
void Scada::compare(float *max,float *min,float *avr,float var,float avr_num)
{
	if((var) > (*max)){//大于大值，更换大值
		(*avr) +=(*max)/(float)avr_num;
		(*max) = (var);
	}else if((var)< (*min)){//小于小指小值，更换小值
		(*avr) += (*min)/(float)avr_num ;
		(*min) = (var) ;
	}else{
		(*avr) += (var)/(float)avr_num ;
	}
}
//acquire min max
void Scada::first_compare(float *max,float *min,float var)
{
	if(FloatCompare ((*max), var)==1){
		(*min) = var;
	}else {
		(*min) = (*max);
		(*max) = var;
	}

}
/*
*************************************************************************************
*  @brief   获得标定系数
*			xishu=stand/current_measuare
*  @param[in] float source_yc 
*  @param[in] float stand_yc 
*  @param[out] float *xishu 
*  @return none
**************************************************************************************
*/
void Scada::GetRatio(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue )
{
	if((*sourceyc)>minvalue){

		(*xishu) = (float)(*standardyc)/(*sourceyc);
		//(*xishu) = PORTRATIO/(*sourceyc);

	}

}
/*
*************************************************************************************
*  @brief   标定      标定参数（DataId = 5097) 下发1 开始标定，标定后 向Varm 发送 参数（DataId = 5097)
*  			值为第一位为fpag1 板 1成功0失败，第二位为fpga2板 1成功0失败， 超时发送0xff；
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
float DcAdSource[2][2][2];//两个通道，电压电流，最小最大值
/*
 * 是指直流输入量的是采集电流，
 * 	int8 DcType; 低四位第一通道，高四位第二通道，0：代表电压；1：代表电流
	int16 NominalValue; 低八位   电压标称值，高八位  电流标称值；
	DcPara     d_508C;   //直流量配置
 */
/*
 * 	uint16 chenal;
	uint16 DcNum;//0 dc1 ,1 dc2
	uint16 type; //0电压。1电流
	uint16 step; //0最小值，1最大值
 */
void Scada::DcCalibration(void)
{
	uint16 chenal;
	uint16 DcNum;
	uint16 type;
	uint16 step;
	uint16 erro;
	Uint16 DcClbrtFlag = scada_cfg.StartDcClbrt;
	DataType2	d_5097;///<标定
	CP56Time2a Time;
	switch(DcClbrtFlag)
	{
		case BD_DC_VOL_CH1_STEP_1 :
			 chenal = 13;
			 DcNum = 0;
			 type = 0;
			 step = 0;
		break;
		case BD_DC_VOL_CH1_STEP_2 :
			 chenal = 13;
			 DcNum = 0;
			 type = 0;
			 step = 1;
		break;
		case BD_DC_VOL_CH1_STEP_3 :
			 chenal = 13;
			 DcNum = 0;
			 type = 0;
			 step = 2;
		break;
		case BD_DC_CUR_CH1_STEP_1 :
			 chenal = 13;
			 DcNum = 0;
			 type = 1;
			 step = 0;
		break;
		case BD_DC_CUR_CH1_STEP_2 :
			 chenal = 13;
			 DcNum = 0;
			 type = 1;
			 step = 1;
		break;
		case BD_DC_CUR_CH1_STEP_3 :
			 chenal = 13;
			 DcNum = 0;
			 type = 1;
			 step = 2;
		break;
		case BD_DC_VOL_CH2_STEP_1 :
			 chenal = 14;
			 DcNum = 1;
			 type = 0;
			 step = 0;
		break;
		case BD_DC_VOL_CH2_STEP_2 :
			 chenal = 14;
			 DcNum = 1;
			 type = 0;
			 step = 1;
		break;
		case BD_DC_VOL_CH2_STEP_3 :
			 chenal = 14;
			 DcNum = 1;
			 type = 0;
			 step = 2;
		break;
		case BD_DC_CUR_CH2_STEP_1 :
			 chenal = 14;
			 DcNum = 1;
			 type = 1;
			 step = 0;
		break;
		case BD_DC_CUR_CH2_STEP_2 :
			 chenal = 14;
			 DcNum = 1;
			 type = 1;
			 step = 1;

		break;
		case BD_DC_CUR_CH2_STEP_3 :
			 chenal = 14;
			 DcNum = 1;
			 type = 1;
			 step = 2;
		break;
		default:
		break;
	}
	if(step!=2){
		DcAdSource[DcNum][type][step] =  GetAverage(&AD_BUF_scada[chenal][0] ,2*SAMP_NUM);
	}else{//计算直线的斜率K=(y2-y2)/(x2-x1)和截距B=(x1y1-y1x2)/(x1-x2)
		DcRationTmp[DcNum][type].K = (scada_cfg.DcPara[DcNum].MaxStand[0]-scada_cfg.DcPara[DcNum].MinStand[0])/
				(DcAdSource[DcNum][type][1]-DcAdSource[DcNum][type][0]);
		DcRationTmp[DcNum][type].B = ((DcAdSource[DcNum][type][0]*scada_cfg.DcPara[DcNum].MaxStand[0])-(DcAdSource[DcNum][type][1]*scada_cfg.DcPara[DcNum].MinStand[0]))/
								       (DcAdSource[DcNum][type][0]-DcAdSource[DcNum][type][1]);

		if(DcRationTmp[DcNum][type].K>0.1){
			d_5097.Data = BD_FAIL;
			msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		}
		DcRation[DcNum][type].K =DcRationTmp[DcNum][type].K ;
		DcRation[DcNum][type].B =DcRationTmp[DcNum][type].B ;
		erro=RW_FlashData(WRITE,FpgaCfg.YcCard[0].SlotNum,DCRATIOADD,
					(uint8 *)&(DcRation[0][0]) ,sizeof(DcRation));
		if(erro==1){
			d_5097.Data  = BD_0K;
			msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		}else{
			d_5097.Data = BD_FAIL;
			Time = clk0.GetCP56Time2a(GETSOFTTIME);
			msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		}

	}
}
/*
*************************************************************************************
*  @brief  标定
*  @param[in]
*  @param[out]
*  @return
*  @note 标定系数保存在各个遥测板的eeprom中,只保存UI的线性系数,
*  角差矫正系数(UI之间的夹角)不好保存
*  因为无法跟板走,如果保存在cpu板又无法适应更换板子.因此,在此版本中不保存角差数据.
*  计算功率时的角差矫正系数采用固定系数(实测结果角差偏差不大).
**************************************************************************************
*/
void Scada::Calibration(void)
{
	int ret;
	int res;
	int i;
	int j;
	DataType2	d_5097;///<标定
	CP56Time2a	Time;
	//float tmp_power;

	ret=0;
	res=0;
	scada_cfg.Clbrtnum++;
	//step 1 init 
	if(scada_cfg.Clbrtnum==1){
		for (i = 0; i < CHANNEL_NUM; i++ ){
			scada_cfg.ClbrtPortTmp[i].max= TotalRms(&channel_modules[i][BASEHARMONIC],xiebo_len);
			scada_cfg.ClbrtPortTmp[i].avr = 0;
		}
#if 0
		//next routine is for pq,but how save this in every board,so i ignore it.
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				scada_cfg.ClbrtPowerTmp[i][j].max = GetPower(UA(i)+j,IA1(i)+j);
				scada_cfg.ClbrtPowerTmp[i][j].avr.p = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.q = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.s = 0.0;
			}
#endif 
	}
	//step 2 get min ,max ,avr
	if(scada_cfg.Clbrtnum==2){
		for (i = 0; i < CHANNEL_NUM; i++ ){
			 first_compare(&scada_cfg.ClbrtPortTmp[i].max,&scada_cfg.ClbrtPortTmp[i].min,TotalRms(&channel_modules[i][BASEHARMONIC],xiebo_len));
		}
#if 0
		//next routine is for pq,but how save this in every board,so i ignore it.
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				tmp_power = GetPower(UA(i)+j,IA1(i)+j);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,tmp_power.p);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,tmp_power.q);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,tmp_power.s);
			}
		}
#endif 
	}
	//step 3 get min ,max ,avr
	if((scada_cfg.Clbrtnum>=3)&&(scada_cfg.Clbrtnum<=(AVERAGENUM + 2))){
		for (i = 0; i < CHANNEL_NUM; i++ ){
			compare(&scada_cfg.ClbrtPortTmp[i].max,&scada_cfg.ClbrtPortTmp[i].min,&scada_cfg.ClbrtPortTmp[i].avr,TotalRms(&channel_modules[i][BASEHARMONIC],xiebo_len),AVERAGENUM);
		}
#if 0
		//next routine is for pq,but how save this in every board,so i ignore it.
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				tmp_power = GetPower(UA(i)+j,IA1(i)+j);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,&scada_cfg.ClbrtPowerTmp[i][j].avr.p,tmp_power.p,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,&scada_cfg.ClbrtPowerTmp[i][j].avr.q,tmp_power.q,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,&scada_cfg.ClbrtPowerTmp[i][j].avr.s,tmp_power.s,AVERAGENUM);
			}
		}
#endif 
	}
	//step 4 .finish
	if(scada_cfg.Clbrtnum==(AVERAGENUM + 2)){
		//float standyc = PORTRATIO;//????????????????????
		float standyc;
		int yc_id;
		int channel;
		float tmp_ratio;
		int crc;
		YcRatio_S *p = &YcPortRatio[0];
		for ( i = 0; i < YC_BOARD_NUM; i++){
			for(j=0;j<YC_PORT_PARA_NUM;j++){
				channel=i*YC_PORT_PARA_NUM+j;
				yc_id=find_yc_by_channel(channel);
				if(yc_config.config_data[yc_id].type==TYPE_U)
					standyc=NOMINAL_U;
				else if(yc_config.config_data[yc_id].type==TYPE_I){
					standyc=NOMINAL_I;
				}
				GetRatio(&tmp_ratio, &scada_cfg.ClbrtPortTmp[i].avr,&standyc,BIAODINGMIN );
				p[i].PortRatio[j] =tmp_ratio; 
			}
			p[i].InitFlag = RATIOINIT;
		}
#if 0
		//角差按照60°(U超前I)
		//next routine is for pq,but how can is save this in every board,so i ignore it.
		for ( i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				YcRatio[i].PowerLineRatio[j]  = YcRatio[i].PortRatio[UA1+j]* YcRatio[i].PortRatio[IA11+j];
				tmp_power = ClbrtPowerTmp[i][j].avr;
				angle = asin((YcRatio[i].PowerLineRatio[j]*tmp_power.q)/NOMINAL_S)-(PI / 6.0);
				YcRatio[i].PowerAngleRatio[0][j].angle = (angle / PI) * 180.0;
				YcRatio[i].PowerAngleRatio[0][j].sin_a =sin(angle);
				YcRatio[i].PowerAngleRatio[0][j].cos_a =cos(angle);
			}
		}
#endif 
		scada_cfg.Clbrtnum = 0;
		//step 5. now save ratio into board eeprom
		for(i = 0;i<YC_BOARD_NUM;i++){
			if(FpgaCfg.YcCard[i].Statu != IN_STATU){
				continue;
			}
			//make crc
			crc=calcrc((uint8 *)&YcPortRatio[i],sizeof(YcRatio_S)-2);
			YcPortRatio[i].crc=crc;
			ret=RW_FlashData(WRITE,FpgaCfg.YcCard[i].SlotNum,ACRATIOADD,
						(uint8 *)&(YcPortRatio[i]) ,
						(sizeof(YcRatio_S)));
			if(ret==1){//标定，存储成功
				res=1;
			}
		}
		if(res==1){
			XiShuChange();
			d_5097.Data  = BD_0K;
			Time = clk0.GetCP56Time2a(GETSOFTTIME);
			msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		}else{
			d_5097.Data  = BD_FAIL;
			Time = clk0.GetCP56Time2a(GETSOFTTIME);
			msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		}
	}
}
#include <xdc/std.h>
#include <list>
using namespace std;
static Uint32 clk_tick1,clk_tick2;
uint64 diff;
extern int stop;
void clk_fun(UArg arg){
	clk_tick2=Clock_getTicks();
	diff=clk_tick2-clk_tick1;
	clk_tick1=clk_tick2;
	return;
}
void Scada::do_test(int cmd){
	if(cmd==1){
		DCOWORD ctl;
		ctl.DcoWord.D_S=SCONTROL;
		ctl.DcoWord.D_S_CS=1;
		YkOut(0,ctl);
	}else if(cmd==2){
		DCOWORD ctl;
		ctl.DcoWord.D_S=SCONTROL;
		ctl.DcoWord.D_S_CS=0;
		YkOut(0,ctl);
	}else if(cmd==3){
		list<DATA> list;
		list.push_back(data[0]);
		list.push_back(data[1]);
		list.push_back(data[2]);

		DATA *res;

		qsort(&data[0],3,sizeof(DATA),compare1);
		DATA key;
		key.no=1;
		res=(DATA*)bsearch((void *)&key,(void *)&data[0],3,sizeof(DATA),compare1);

	}else if(cmd==4){
		DCOWORD dco;
		dco.data=0x80;
		ControlYk(BREAKER1,dco);
	}else if(cmd==5){
		DCOWORD dco;
		dco.data=0x00;
		ControlYk(BREAKER1,dco);
	}else if(cmd==6){
		PFUNC(TEM_DEBUG,"Hello World\r\n");
		g_cmd=0;
	}else if(cmd==7){
		g_cmd=0;
		Clock_Params clk_para;
		Clock_Handle clk_handle;

		Clock_Params_init(&clk_para);
		clk_para.period=125;
		clk_para.startFlag=false;
		clk_handle=Clock_create(clk_fun,10,&clk_para,NULL);
		Clock_start(clk_handle);
	}else if(cmd==8){
		stop=1;
		deinit_edma();
		g_cmd=0;
	}
}
