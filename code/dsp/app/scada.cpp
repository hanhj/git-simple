#include <string.h>///<memcpy
//#include <unistd.h>///<for sleep
#include "scada.h"
#include "message.h"///<用到message类中的消息
//#include "timer.h"///<用到Timer类中的计时器
#include "task.h"///<用到Task类中的g_task全局变量中Heart函数
#include "debug.h"
#include "mathlist.h"
#include "datatype.h"
#include "gpio.h"
#include "adsamp.h"
#include "DSPF_sp_fftSPxSP.h"
#include "fft.h"
#include "DSPF_util.h"
#include <math.h>
#include "mathlist.h"
#include "fpga.h"
#include "omapl138.h"
#include "omapl138timer.h"
#include "clock.h"
#include "interruptapp.h"
#include "localprotect.h"
#include "yx.h"
#include "yk.h"
extern Semaphore_Handle varm_sem;
void adcompass_int(void);
void YcRatioInit(uint8 line);
void YcStandInit(void);
//#pragma DATA_SECTION("DataInDDR2")
scada_yc_t YcData;
#pragma DATA_SECTION("DataInDDR2")
setmstime_t YcDeadTime[0x200];
#pragma DATA_SECTION("scadaDATA")
Scada scada;
/** /brief 需要定义一个全局函数供注册消息类处理消息
*/
 int16 GetMsg2(MessageData<BaseDataType>message){
//scada.GetMsg(message);
return 1;
}	

/** /brief 需要定义一个全局函数Task类处理该类的入口
*/
void* scada_main(void*){
	scada.Run();
	return NULL;
}
/** /brief 定义一个全局函数处理时间回调函数
*/
/* void  callback_time(void)
{
	PFUNC(TEM_INFO,"scada call time\r\n");
	scada.Heart();

}*/
///////////////公有函数//////////////////////////////////////////////////

#pragma CODE_SECTION("scadainit_code")
Scada::Scada(){
	strcpy(name,"Scada");
	Id=TASK_SCADA;
}
#pragma CODE_SECTION("scadainit_code")
int16 Scada:: ReadCfg(uint32 Id)
{
	uint16 i;
	uint16 name_pos = 0;
	Data<BaseDataType> data;
	user_data.GetData(Id,&data);
	switch(Id)
	{
	case 0x5080:
		//DataType1 	d_5080;///<电压电流有效值计算方法  0全波 1 基波
		memcpy(&scada_cfg.d_5080,data.Data,sizeof(scada_cfg.d_5080));
		break;
	case 0x5081:
		//UserPortSet  	d_5081;///<模拟量输入配置
		memcpy(&scada_cfg.d_5081,data.Data,sizeof(scada_cfg.d_5081));
		for ( i = 0; i < MAXYCNUM; i++){
			yc_configure.ad_compass[i] = scada_cfg.d_5081.Data.Define[i].Port;
		}
		yc_configure.yc_num = MAXYCNUM;
		name_pos = 0;
		for ( i = 0; i < MAXYCNUM; i++){
			if (yc_configure.ad_compass[i] == NO_YC){
				yc_configure.yc_num--;
			}else{
				yc_configure.name[name_pos++] = i;
			}
		}
		YcStandInit();
		break;
	case 0x5082:
		//UserPortSet  	d_5082;///<模拟量输入配置
		//memcpy(&scada_cfg.d_5082,data.Data,sizeof(scada_cfg.d_5082));
		break;
	case 0x5083:
		//UserPortSet  	d_5083;///<模拟量输入配置
		//memcpy(&scada_cfg.d_5083,data.Data,sizeof(scada_cfg.d_5083));
		break;
	case 0x5084:
		//UserPortSet  	d_5084;///<模拟量输入配置
		//memcpy(&scada_cfg.d_5084,data.Data,sizeof(scada_cfg.d_5084));
		break;
	case 0x508b:
		//DataType1		d_508B;///<功率计算方法配置
		memcpy(&scada_cfg.d_508B,data.Data,sizeof(scada_cfg.d_508B));
		break;
	case 0x508c:
		//DataType1		d_508C;///<直流输入量配置
		memcpy(&scada_cfg.d_508C,data.Data,sizeof(scada_cfg.d_508C));
		break;
	case 0x508d:
		//DataType1		d_508D;///<电流标称值
		memcpy(&scada_cfg.d_508D,data.Data,sizeof(scada_cfg.d_508D));
		YcStandInit();
		break;
	case 0x508e:
		//DataType1		d_508E;///<电压标称值
		memcpy(&scada_cfg.d_508E,data.Data,sizeof(scada_cfg.d_508E));
		YcStandInit();
	break;
	case 0x508f:
		//DeadAreaSet	d_508F;///<遥测死区值设置
		memcpy(&scada_cfg.d_508F,data.Data,sizeof(scada_cfg.d_508F));
		break;
	case 0x5095:
		//DataType2		d_5095;///<PT
		memcpy(&scada_cfg.d_5095,data.Data,sizeof(scada_cfg.d_5095));
		break;
	case 0x5096:
		//DataType2		d_5096;///<CT
		memcpy(&scada_cfg.d_5096,data.Data,sizeof(scada_cfg.d_5096));
		break;
	case 0x5097:
		//DataType2		d_5097;///<标定
		memcpy(&scada_cfg.d_5097,data.Data,sizeof(scada_cfg.d_5097));
		 if (scada_cfg.d_5097.Data == BDSTEP_1){
			 scada_cfg.StartClbrt = BDSTART;
		 }
		break;
	case 0x5098:
		//YCMsgSet	  d_5098;///<遥测信息体属性配置
		memcpy(&scada_cfg.d_5098,data.Data,sizeof(scada_cfg.d_5098));
		break;
	case 0x5099:
		//YCMsgSet	  d_5099;///<遥测信息体属性配置
		memcpy(&scada_cfg.d_5099,data.Data,sizeof(scada_cfg.d_5099));
		break;
	case 0x50b8:
		//YKMsgSet	  d_50B8;///<遥控信息体属性配置
		memcpy(&scada_cfg.d_50B8,data.Data,sizeof(scada_cfg.d_50B8));
		break;

	//PassWdPara  d_5200;///<密码参数
	//PassWdPara  d_5201;///<版本号
	//HisDataDefinePara d_520E;///<遥测历史数据项定义
	//DataType2		d_520F;///<遥测历史数据存储间隔
	//HisDataDefinePara d_5210;///<遥测极值数据项定义
	case 0x5211:
		//DataType1   d_5211;///<人工置数使能
		memcpy(&scada_cfg.d_5211,data.Data,sizeof(scada_cfg.d_5211));
		break;
	case 0x5212:
		//UserRealDataDefPara d_5212;///<人工置数参数
		memcpy(&scada_cfg.d_5212,data.Data,sizeof(scada_cfg.d_5212));
		break;
	default:
		break;
	}
	return 1;
}
#pragma CODE_SECTION("scadainit_code")
int16 Scada::Init(){

	uint16 i;
	adcompass_int();
	YcRatioInit(0);
	YcRatioInit(1);
	for(i = 0;i<FpgaCfg.YcCardSize;i++){
		if(FpgaReadData(FpgaCfg.YcCard[i].num,(uint16 *)&(YcRatio[i]) ,sizeof(ratio_t)/2)==1){

		}else{
			YcRatioInit(i);
		}
	}
	YcStandInit();
	scada_cfg.ycavrnum = 0;
	scada_cfg.d_508B.Data.line[0] = Pt_3;//三表法
	scada_cfg.d_508B.Data.line[1] = 0xff;//不计算
	scada_cfg.StartClbrt = 0;
	scada_cfg.d_5212.Data.Ua.BaseValue = 100;
	scada_cfg.d_5212.Data.Ua.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Ua.HarmContent = 0;
	scada_cfg.d_5212.Data.Ub.BaseValue = 100;
	scada_cfg.d_5212.Data.Ub.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Ub.HarmContent = 0;
	scada_cfg.d_5212.Data.Uc.BaseValue = 100;
	scada_cfg.d_5212.Data.Uc.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Uc.HarmContent = 0;
	scada_cfg.d_5212.Data.Ia.BaseValue = 5;
	scada_cfg.d_5212.Data.Ia.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Ia.HarmContent = 0;
	scada_cfg.d_5212.Data.Ib.BaseValue = 5;
	scada_cfg.d_5212.Data.Ib.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Ib.HarmContent = 0;
	scada_cfg.d_5212.Data.Ic.BaseValue = 5;
	scada_cfg.d_5212.Data.Ic.HarmoDgree = 0;
	scada_cfg.d_5212.Data.Ic.HarmContent = 0;
	scada_cfg.d_5212.Data.IaAngle = 60;
	scada_cfg.d_5212.Data.IbAngle = 60;
	scada_cfg.d_5212.Data.IcAngle = 60;

/////谐波计算数据表初始化
   scada_cfg.HmncDataP[0][0].Rms = &YcData.ua1rms2_401C;
   scada_cfg.HmncDataP[0][0].Ratio = &YcData.ua1ratio2_4041;
   scada_cfg.HmncDataP[0][0].TotalRatio = &YcData.ua1totalratio_4040;
   scada_cfg.HmncDataP[0][0].YcName = UA1;
   scada_cfg.HmncDataP[0][1].Rms = &YcData.ub1rms2_4066;
   scada_cfg.HmncDataP[0][1].Ratio = &YcData.ub1ratio2_408B;
   scada_cfg.HmncDataP[0][1].TotalRatio = &YcData.ub1totalratio_408A;
   scada_cfg.HmncDataP[0][1].YcName = UB1;
   scada_cfg.HmncDataP[0][2].Rms = &YcData.uc1rms2_40B0;
   scada_cfg.HmncDataP[0][2].Ratio = &YcData.uc1ratio2_40D5;
   scada_cfg.HmncDataP[0][2].TotalRatio = &YcData.uc1totalratio_40D4;
   scada_cfg.HmncDataP[0][2].YcName = UC1;
   scada_cfg.HmncDataP[0][3].Rms = &YcData.ia1rms2_402E;
   scada_cfg.HmncDataP[0][3].Ratio = &YcData.ia1ratio2_4054;
   scada_cfg.HmncDataP[0][3].TotalRatio = &YcData.ia1totalratio_4053;
   scada_cfg.HmncDataP[0][3].YcName = IA11;
   scada_cfg.HmncDataP[0][4].Rms = &YcData.ib1rms2_4078;
   scada_cfg.HmncDataP[0][4].Ratio = &YcData.ib1ratio2_409E;
   scada_cfg.HmncDataP[0][4].TotalRatio = &YcData.ib1totalratio_409D;
   scada_cfg.HmncDataP[0][4].YcName = IB11;
   scada_cfg.HmncDataP[0][5].Rms = &YcData.ic1rms2_40C2;
   scada_cfg.HmncDataP[0][5].Ratio = &YcData.ic1ratio2_40E8;
   scada_cfg.HmncDataP[0][5].TotalRatio = &YcData.ic1totalratio_40E7;
   scada_cfg.HmncDataP[0][5].YcName = IC11;
   scada_cfg.HmncDataP[1][0].Rms = &YcData.ua2rms2_4115;
   scada_cfg.HmncDataP[1][0].Ratio = &YcData.ua2ratio2_413A;
   scada_cfg.HmncDataP[1][0].TotalRatio = &YcData.ua2totalratio_4139;
   scada_cfg.HmncDataP[1][0].YcName = UA2;
   scada_cfg.HmncDataP[1][1].Rms = &YcData.ub2rms2_415F;
   scada_cfg.HmncDataP[1][1].Ratio = &YcData.ub2ratio2_4184;
   scada_cfg.HmncDataP[1][1].TotalRatio = &YcData.ub2totalratio_4183;
   scada_cfg.HmncDataP[1][1].YcName = UB2;
   scada_cfg.HmncDataP[1][2].Rms = &YcData.uc2rms2_41A9;
   scada_cfg.HmncDataP[1][2].Ratio = &YcData.uc2ratio2_41CE;
   scada_cfg.HmncDataP[1][2].TotalRatio = &YcData.uc2totalratio_41CD;
   scada_cfg.HmncDataP[1][2].YcName = UC2;
   scada_cfg.HmncDataP[1][3].Rms = &YcData.ia2rms2_4127;
   scada_cfg.HmncDataP[1][3].Ratio = &YcData.ia2ratio2_414D;
   scada_cfg.HmncDataP[1][3].TotalRatio = &YcData.ia2totalratio_414C;
   scada_cfg.HmncDataP[1][3].YcName = IA21;
   scada_cfg.HmncDataP[1][4].Rms = &YcData.ib2rms2_4171;
   scada_cfg.HmncDataP[1][4].Ratio = &YcData.ib2ratio_4197;
   scada_cfg.HmncDataP[1][4].TotalRatio = &YcData.ib2totalratio_4196;
   scada_cfg.HmncDataP[1][4].YcName = IB21;
   scada_cfg.HmncDataP[1][5].Rms = &YcData.ic2rms2_41BB;
   scada_cfg.HmncDataP[1][5].Ratio = &YcData.ic2ratio2_41E1;
   scada_cfg.HmncDataP[1][5].TotalRatio = &YcData.ic2totalratio_41E0;
   scada_cfg.HmncDataP[1][5].YcName = IC21;
   scada_cfg.ClbrtTime.SetFlag = 0;
   scada_cfg.Clbrtnum = 0;
   // 人工值初始化
   scada_cfg.d_508F.Data.DeadAngle = 0;
   scada_cfg.d_508F.Data.DeadFreq = 0;
   scada_cfg.d_508F.Data.DeadHarmoicCount = 0;
   scada_cfg.d_508F.Data.DeadHarmoicValue = 0;
   scada_cfg.d_508F.Data.DeadHumidity = 0;
   scada_cfg.d_508F.Data.DeadI = 0;
   scada_cfg.d_508F.Data.DeadP = 0;
   scada_cfg.d_508F.Data.DeadTemp = 0;
   scada_cfg.d_508F.Data.DeadU = 0;
   scada_cfg.d_5211.Data = 0;
   //
   MAXYCDATANUM  = ((sizeof(YcData))/sizeof(DataType4));
	return 1;
}
int16 Scada::Run()
{
	uint8 led=0;
//	Semaphore_pend(varm_sem, BIOS_WAIT_FOREVER);
	Init();
	YxInit();
	YkInit();
	interrupt_int();
	fa.Init();
	lp.Init();
	msg.Init();
	while(1)
	{
		lp.fun();
		if (led==0){
			led = 1;
		}else {
			led = 0;
		}
		sleep(100);
		GPIO_setOutput(GPIO_BANK0, GPIO_PIN6, led);
		Caculate();
	}
}

int16 Scada::GetMsg(MessageData<BaseDataType>message){
	//PFUNC(TEM_INFO,"deal %s Message\r\n",name);
	if(message.MessageId==MSG_YK)
		{
			DataType1*yk_data;
			yk_data=(DataType1*)&message.Data;
			//ControlYk(message.Id,yk_data);
		}
	if(message.MessageId==MSG_PARA)
		{
			//if(message.Id==NO1_FIRST_SECTION_CURRENT_SET){
				if(message.Id==PARA_5001){
			memcpy(&scada_cfg,(ScadaPara*)&message.Data,sizeof(scada_cfg));
		}
		}
return 1;		
}


/*
*************************************************************************************
*  @brief  有效值计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void adcompass_int(void)
{
	unsigned char i;
	unsigned char name_pos = 0;
	for ( i = 0; i < MAXYCNUM; i++){
		yc_configure.ad_compass[i] = i;
	}
yc_configure.ad_compass[UA1] = 0;
yc_configure.ad_compass[UB1] = 1;
yc_configure.ad_compass[UC1] = 2;
yc_configure.ad_compass[UAB1] = 0xff;
yc_configure.ad_compass[UCB1] = 0xff;
yc_configure.ad_compass[U01] = 3;
yc_configure.ad_compass[IA11] = 4;
yc_configure.ad_compass[IB11] = 5;
yc_configure.ad_compass[IC11] = 6;
yc_configure.ad_compass[I011] = 7;
yc_configure.ad_compass[IA12] = 8;
yc_configure.ad_compass[IB12] = 9;
yc_configure.ad_compass[IC12] = 10;
yc_configure.ad_compass[UA2] = 11;
yc_configure.ad_compass[UB2] = 12;
yc_configure.ad_compass[UC2] = 13;
yc_configure.ad_compass[UAB2] = 0xff;
yc_configure.ad_compass[UCB2] = 0xff;
yc_configure.ad_compass[U02] = 14;
yc_configure.ad_compass[IA21] = 15;
yc_configure.ad_compass[IB21] = 16;
yc_configure.ad_compass[IC21] = 17;
yc_configure.ad_compass[I021] = 18;
yc_configure.ad_compass[IA22] = 19;
yc_configure.ad_compass[IB22] = 20;
yc_configure.ad_compass[IC22] = 21;
	yc_configure.yc_num =MAXYCNUM;
	for ( i = 0; i < MAXYCNUM; i++){
		if (yc_configure.ad_compass[i] == NO_YC){
			yc_configure.yc_num--;
		}else{
			yc_configure.name[name_pos++] = i;
		}
	}
}

void YcRatioInit(uint8 line)
{
	//////////标定系数初始化
	uint16 i;
	YcRatio[line].PortRatio[UA1] = U300RATIO;
	YcRatio[line].PortRatio[UB1] = U300RATIO;
	YcRatio[line].PortRatio[UC1] = U300RATIO;
	YcRatio[line].PortRatio[UAB1] = U300RATIO;
	YcRatio[line].PortRatio[UCB1] = U300RATIO;
	YcRatio[line].PortRatio[U01] = U300RATIO;
	YcRatio[line].PortRatio[IA11] = I10RATIO;
	YcRatio[line].PortRatio[IB11] = I10RATIO;
	YcRatio[line].PortRatio[IC11] = I10RATIO;
	YcRatio[line].PortRatio[I011] = I100RATIO;
	YcRatio[line].PortRatio[IA12] = I100RATIO;
	YcRatio[line].PortRatio[IB12] = I100RATIO;
	YcRatio[line].PortRatio[IC12] = I100RATIO;
	for( i = 0; i < 4 ; i++){
		YcRatio[line].PowerLineRatio[i] = U300RATIO * I10RATIO ;//1.0;
		YcRatio[line].PowerAngleRatio[0][i].angle = 0;
		YcRatio[line].PowerAngleRatio[0][i].cos_a = 1;
		YcRatio[line].PowerAngleRatio[0][i].sin_a = 0;
		YcRatio[line].PowerAngleRatio[1][i].angle = 0;
		YcRatio[line].PowerAngleRatio[1][i].cos_a = 1;
		YcRatio[line].PowerAngleRatio[1][i].sin_a = 0;
	}

}
void YcStandInit(void)
{
	uint8 i;
	int8 YcName;
	memset (scada.scada_cfg.StandYc,0,sizeof(scada.scada_cfg.StandYc));
	for (i = 0; i < 26; i++ ){//key
		YcName = yc_configure.name[i];//只要在配置表中的遥测项才去计算
			if(YcName>=13){
				if((YcName>=UA2)&&(YcName<=U02)){
					scada.scada_cfg.StandYc[LINE2].PortRatio[YcName]  =100; //scada.scada_cfg.d_508E.Data;
				}else if((YcName>=IA21)&&(YcName<=IC22)){
					scada.scada_cfg.StandYc[LINE2].PortRatio[YcName-13] =5;// scada.scada_cfg.d_508D.Data;
				}
			}else{
				if((YcName>=UA1)&&(YcName<=U01)){
								scada.scada_cfg.StandYc[LINE1].PortRatio[YcName] =100;// scada.scada_cfg.d_508E.Data;
				}else if((YcName>=IA11)&&(YcName<=IC12)){
								scada.scada_cfg.StandYc[LINE1].PortRatio[YcName] =5;// scada.scada_cfg.d_508D.Data;
				}
			}
	}
	for(i = 0; i< 3;i++){
		scada.scada_cfg.StandYc[LINE1].PowerLineRatio[i] =100*5;//scada.scada_cfg.d_508E.Data * scada.scada_cfg.d_508E.Data * cossp(PI/6);
	}
	scada.scada_cfg.StandYc[LINE1].PowerLineRatio[3] = 3*100*5;
	for(i = 0; i< 3;i++){
		scada.scada_cfg.StandYc[LINE2].PowerLineRatio[i] =100*5;//scada.scada_cfg.d_508E.Data * scada.scada_cfg.d_508E.Data * cossp(PI/6);
	}
	scada.scada_cfg.StandYc[LINE2].PowerLineRatio[3] = 3*100*5;
}
/*
*************************************************************************************
*  @brief 数据补全
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada:: PtAdapt(void)
{
	uint8 i;
	uint8 powerway;
	for (i = 0; i < 2; i++)
	{
		powerway = scada_cfg.d_508B.Data.line[i];
		switch(powerway){
		case Pt_Ua:
			PhaseChange(scada_cfg.FFT_RESULT[UB1+i*13],scada_cfg.FFT_RESULT[UA1+i*13],FFT_NUM/2,(-1) * U_120ANGLE);//负为滞后，顺时针转动
			PhaseChange(scada_cfg.FFT_RESULT[UC1+i*13],scada_cfg.FFT_RESULT[UA1+i*13],FFT_NUM/2, U_120ANGLE);
			memcpy(scada_cfg.Modulus[UB1+i*13],scada_cfg.Modulus[UA1+i*13],FFT_NUM/2);
			memcpy(scada_cfg.Modulus[UC1+i*13],scada_cfg.Modulus[UA1+i*13],FFT_NUM/2);
			break;
		case Pt_Ub:
			PhaseChange(scada_cfg.FFT_RESULT[UA1+i*13],scada_cfg.FFT_RESULT[UB1+i*13],FFT_NUM/2,U_60ANGLE);
			PhaseChange(scada_cfg.FFT_RESULT[UC1+i*13],scada_cfg.FFT_RESULT[UB1+i*13],FFT_NUM/2,(-1) *U_120ANGLE);
			memcpy(scada_cfg.Modulus[UA1+i*13],scada_cfg.Modulus[UB1+i*13],FFT_NUM/2);
			memcpy(scada_cfg.Modulus[UC1+i*13],scada_cfg.Modulus[UB1+i*13],FFT_NUM/2);
			break;
		case Pt_Uc:
			PhaseChange(scada_cfg.FFT_RESULT[UA1+i*13],scada_cfg.FFT_RESULT[UC1+i*13],FFT_NUM/2,(-1) *U_60ANGLE);
			PhaseChange(scada_cfg.FFT_RESULT[UB1+i*13],scada_cfg.FFT_RESULT[UC1+i*13],FFT_NUM/2, U_120ANGLE);
			memcpy(scada_cfg.Modulus[UA1+i*13],scada_cfg.Modulus[UC1+i*13],FFT_NUM/2);
			memcpy(scada_cfg.Modulus[UB1+i*13],scada_cfg.Modulus[UC1+i*13],FFT_NUM/2);
			break;
		case Pt_Uab:
			PhaseChange(scada_cfg.FFT_RESULT[UCB1+i*13],scada_cfg.FFT_RESULT[UAB1+i*13],FFT_NUM/2,U_60ANGLE);
			memcpy(scada_cfg.Modulus[UCB1+i*13],scada_cfg.Modulus[UAB1+i*13],FFT_NUM/2);
			break;
		case Pt_Ucb:
			PhaseChange(scada_cfg.FFT_RESULT[UAB1+i*13],scada_cfg.FFT_RESULT[UCB1+i*13],FFT_NUM/2,(-1)*U_60ANGLE);
			memcpy(scada_cfg.Modulus[UAB1+i*13],scada_cfg.Modulus[UCB1+i*13],FFT_NUM/2);
			break;
		default:
			break;
		}
	}
}


/*
*************************************************************************************
*  @brief  模值计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return   第0点，第32点为直流量
**************************************************************************************
*/
void Scada:: GetModulus(void)
{
	int portnum;
	int i;
	int YcNum;
	uint8 YcName;
	memset(&scada_cfg.FFT_RESULT,0,sizeof(scada_cfg.FFT_RESULT));
	memset(&scada_cfg.Modulus,0,sizeof(scada_cfg.Modulus));
 	for(YcNum = 0; YcNum < yc_configure.yc_num; YcNum++){//检索所有的配置的遥测采样项
 		YcName = yc_configure.name[YcNum];//获取配置的遥测项
 		portnum = yc_configure.ad_compass[YcName];//通过遥测项获取对应的通道号
		if(portnum!=NO_YC){
			DSPF_sp_fftSPxSP (64, (float *)&AD_BUF[portnum][PAD], &ptr_w64[0], (float *)&scada_cfg.FFT_RESULT[YcName][PAD], brev, 4, 0, 64);
			for (i = DCCOMPONENT; i < HARMONIC19  ; i++){//多余不加
				scada_cfg.Modulus[YcName][i]=sqrtsp((powsp(scada_cfg.FFT_RESULT[YcName][PAD+i].rel,2)+ powsp(scada_cfg.FFT_RESULT[YcName][PAD+i].img,2)));
			}//最终得到的FFT_RESULT和Modulus 是以遥测ID为固定顺序的
		}
 	}
 	PtAdapt();//完成单PT是其他数据的推出；
}

/*
*************************************************************************************
*  @brief  有效值计算
*  @author   zf
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
*************************************************************************************
*  @brief  谐波含有率
*  @author   zf
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
	return HmncData;
}
/*
*************************************************************************************
*  @brief  谐波有效值
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************/
void Scada:: GetHarmonicData(void)
{
	uint8 i,j;
	uint8 line;
	uint8 YcName ;
	harmonictab_t *HmncData_P = NULL;
	float BaseHarmonicRms;
	harmonicdata_t HmncData;
	for(line = 0; line < MAX_LINE_NUM; line++){
		for (j = 0; j < 6; j++){
			HmncData_P = &scada_cfg.HmncDataP[line][j];
			if((HmncData_P->Rms==NULL)||(HmncData_P->YcName>25)||(HmncData_P->Ratio==NULL)){
				break;
			}
			YcName = (HmncData_P->YcName)-(line*13);
			HmncData_P->TotalRatio->Data = 0;
			for (i = 0; i < 18; i++){
				BaseHarmonicRms = TotalRms(&scada_cfg.Modulus[HmncData_P->YcName][BASEHARMONIC],SINGLEHARMONIC);
				HmncData=GetHarmonicRmsAndRatio(BaseHarmonicRms,&scada_cfg.Modulus[YcName][HARMONIC2+i]);
				HmncData_P->Rms[i].Data= HmncData.rms*YcRatio[line].PortRatio[YcName];
				HmncData_P->Ratio[i].Data = HmncData.rario;
				HmncData_P->TotalRatio->Data += HmncData_P->Ratio[i].Data;
			}
		}
	}
}
/*
*************************************************************************************
*  @brief  有效值计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************/
#define PINJUNZHI 20.0
void Scada:: GetRms(void)
{
	uint8 YcName;
	uint8 i;
	float SOrh,SOt;
	DataType4 *Pyc1=&YcData.ua1_4001;
	DataType4 *Pyc2=&YcData.ua2_40FA;
	uint8 rmstype = 1;
	if (scada_cfg.d_5080.Data == 0){
		rmstype = XIEBOLEN;
	}
	for (i = 0; i < yc_configure.yc_num; i++ ){//key
		YcName = yc_configure.name[i];//只要在配置表中的遥测项才去计算
			if(YcName>=13){
				Pyc2[YcName-13].Data= (TotalRms(&scada_cfg.Modulus[YcName][BASEHARMONIC],rmstype)*YcRatio[LINE2].PortRatio[YcName-13]);
			}else{
				Pyc1[YcName].Data= (TotalRms(&scada_cfg.Modulus[YcName][BASEHARMONIC],rmstype)*YcRatio[LINE1].PortRatio[YcName]);
			}
	}

	//scada_cfg.ycavrnum++;
    //if (scada_cfg.ycavrnum>=PINJUNZHI){
    //	memcpy(&YcData,&scada_cfg.YcTmp,sizeof(&YcData));
    //	memset(&scada_cfg.YcTmp,0,sizeof(&YcData));
    //	scada_cfg.ycavrnum = 0;
   // }




	//RH=c1 + c2*SOrh + c3* SOrh*SOrh  c1= -2.0468,c2=0.0367,c3=-1.5955E(-6)
	// T = d1 + d* SOt ;d1 = -39.7 d2 = 0.01
	YcData.difangle_4401.Data = GetYcAngle(DiffVolName[0],DiffVolName[1]);
	YcData.difvol_4402.Data = DiffVolP[0]->Data - DiffVolP[1]->Data;
	SOrh = FpgaHost.HUMIDITY;
	SOt = FpgaHost.TEM;
	YcData.tem_4606.Data = (-39.7) +  0.01* SOt;
	YcData.hum_4607.Data = (-2.0468) + 0.0367*SOrh + (-0.00000015955)* SOrh*SOrh;

	YcData.freq1_401B.Data = (1000000.0/(FpgaSlave[4].YC_PERIOD));
}

float Scada::GetYcAngle(uint8 YcName1,uint8 YcName2)
{
	SP_COMPLEX Yc1,Yc2;
	float rel;
	Yc1 = scada_cfg.FFT_RESULT[YcName1][BASEHARMONIC];
	Yc2 = scada_cfg.FFT_RESULT[YcName2][BASEHARMONIC];
	rel = GetCplAngle(Yc1) - GetCplAngle(Yc2);
	return rel;
}
float Scada::GetCplAngle(SP_COMPLEX CplData)
{
	float result;
    if ( CplData.rel <=0.01 ) {
        if ( CplData.img > 0 ) {
            result = PI / 2.0; //90°
        } else {
            result = -PI / 2.0; // -90°
        }
    } else {
        if ( CplData.img <=0.01 ) {
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



//逆时针旋转为正角度反之为负 ，如果是线电压，最终旋转成第一相位UAB, 第二项为UCB
//如果是相电压
void Scada:: PhaseChange(SP_COMPLEX *dir, SP_COMPLEX *src ,  uint16 len ,float offsetangle )
{
 uint8 i;
 for (i = 0; i < len ; i++){
	 dir[i]  = ComplexRotation(src[i] ,offsetangle* (i+1));
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
	if (umod<10.0){
		return sincosvalue;
	}
	if (imod<10.0){
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
power_t  Scada:: GetPower(uint8 Uname,uint8 Iname)
{
	uint8 j;
	SP_COMPLEX U;
	SP_COMPLEX I;
	angelvalue_t sincosvalue;
	power_t Power[XIEBOLEN];
	power_t TotalP;
	for(j = 1; j<XIEBOLEN+1; j++){
		U = scada_cfg.FFT_RESULT[Uname][j];
		I = scada_cfg.FFT_RESULT[Iname][j];
		sincosvalue = CosSinz1z2(U,I);
		Power[j-1].s = scada_cfg.Modulus[Uname][j]*scada_cfg.Modulus[Iname][j];
		Power[j-1].p = Power[j-1].s*sincosvalue.cos_a;
		Power[j-1].q = Power[j-1].s*sincosvalue.sin_a;
	}
	TotalP.p = 0;
	TotalP.q = 0;
	TotalP.s = 0;
	for(j = 0; j<XIEBOLEN; j++){
		TotalP.p+=Power[j].p;
		TotalP.q+=Power[j].q;
		TotalP.s+=Power[j].s;
	}
	//TotalP.p *=TWODEVXISHU;
	//TotalP.q *=TWODEVXISHU;
	//TotalP.s *=TWODEVXISHU;

	return TotalP;
}
/*
*************************************************************************************
*  @brief  功率值计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/

void Scada:: GetPQS(void)
{
	power_t powertmp;
	uint8 flag = 0,i;
	float p,q,s;
	DataType4 *Pyc;

	for ( i = 0; i < MAX_LINE_NUM; i++){
		if(scada_cfg.d_508B.Data.line[LINE(i)]<Pt_2){//都配置了采样通道，有数据可采
			powertmp = GetPower(UA(i),IA1(i));
			flag = 1;
		}else if(scada_cfg.d_508B.Data.line[LINE(i)]<=Pt_Ucb){
			powertmp = GetPower(UAB(i),IA1(i));
			flag = 1;
		}
		if (i==0){
			Pyc = &YcData.pa1_400E;//第一回线 PA;
		}else {
			Pyc = &YcData.pa2_4107;//第二回线 PA;
		}
		if (flag == 1){
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_A];///<线路1 A相有功功率
			Pyc[1].Data = \
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_A];	///<线路1 A相无功功率
			Pyc[2].Data = powertmp.p/powertmp.s;	///<线路1 A相功率因数
		}else{
			Pyc[0].Data = 0;//YcData.d_400E.Data = 0;
			Pyc[1].Data = 0;//YcData.d_400F.Data = 0;
			Pyc[2].Data = 0;//YcData.d_4010.Data = 0;
		}

		if (i==0){
			Pyc = &YcData.pb1_4011;//第一回线 Pb;
		}else {
			Pyc = &YcData.pb2_410A;//第二回线 Pb;
		}
		if(scada_cfg.d_508B.Data.line[LINE(i)]<Pt_2){
			powertmp = GetPower(UB(i),IB1(i));
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_B];	///<线路1 B相有功功率
			Pyc[1].Data = \
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_B];	///<线路1 B相无功功率
			Pyc[2].Data = powertmp.p/powertmp.s;	///<线路1 B相功率因数
		}else{
			Pyc[0].Data = 0; //YcData.d_4011.Data = 0;
			Pyc[1].Data = 0; //YcData.d_4012.Data = 0;
			Pyc[2].Data = 0; //YcData.d_4013.Data = 0;
		}
		flag = 0;
		if(scada_cfg.d_508B.Data.line[LINE(i)]<Pt_2){
			powertmp = GetPower(UC(i),IC1(i));
			flag = 1;

		}else if(scada_cfg.d_508B.Data.line[LINE(i)]<=Pt_Ucb){
			powertmp = GetPower(UCB(i),IC1(i));
			flag = 1;
		}
		if (i==0){
			Pyc = &YcData.pc1_4014;//第一回线 Pc;
		}else {
			Pyc = &YcData.pc2_410D;//第二回线 Pc;
		}
		if (flag==1){
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_C];	///<线路1 C相有功功率
			Pyc[1].Data =\
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_C];	///<线路1 C相无功功率
			Pyc[2].Data = powertmp.p/powertmp.s;	///<线路1 C相功率因数
		}else {
			Pyc[0].Data = 0;//YcData.d_4014.Data = 0;
			Pyc[1].Data = 0;//YcData.d_4015.Data = 0;
			Pyc[2].Data = 0;//YcData.d_4016.Data = 0;
		}

		YcData.P1_4017.Data = YcData.pa1_400E.Data
									   +YcData.pb1_4011.Data
									   +YcData.pc1_4014.Data;	///<线路1 总有功功率
		p = YcData.P1_4017.Data;
		YcData.Q1_4018.Data = YcData.qa1_400F.Data
									   +YcData.qb1_4012.Data
									   +YcData.qc1_4015.Data;	///<线路1 总无功功率
		q = YcData.Q1_4018.Data;

		s = sqrtsp(p*p +q*q);
		YcData.S1_4019.Data = sqrtsp(p*p +q*q);	///<线路1 总视在功率
		YcData.COS1_401A.Data = p/s;	///<线路1 总功率因数
	}
}

/*
*************************************************************************************
*  @brief 根据数据ID号索引死区值
*  @author   zf
*  @param[in]
*  @param[out]
*  @return 死区值
**************************************************************************************
*/
float Scada::GetDead(int32 DataId)
{
	if
	(
		((DataId>=0x4001)&&(DataId<=0x4006))
		||
		((DataId>=0x40FA)&&(DataId>=0x40FF))
		||(DataId==0x4402)//压差
	)
	{
		return scada_cfg.d_508F.Data.DeadU;
	}
	else if
	(
		((DataId>=0x4007)&&(DataId<=0x400d))
		||
		((DataId>=0x4100)&&(DataId>=0x4106))
	)
	{
		return scada_cfg.d_508F.Data.DeadI;
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
		||(DataId>=0x4108)
		||(DataId==0x410a)
		||(DataId>=0x410b)
		||(DataId==0x410d)
		||(DataId>=0x410e)
		||(DataId==0x4110)
		||(DataId>=0x4111)
		||(DataId>=0x4112)
	)
	{
		return scada_cfg.d_508F.Data.DeadP;
	}
	else if
	(
		((DataId>=0x401D)&&(DataId<=0x403f))
		||
		((DataId>=0x4066)&&(DataId>=0x4089))
		||
		((DataId>=0x40B0)&&(DataId>=0x40d3))
		||
		((DataId>=0x4115)&&(DataId>=0x4138))
		||
		((DataId>=0x415F)&&(DataId>=0x4182))
		||
		((DataId>=0x41A9)&&(DataId>=0x41cc))
	)
	{
		return scada_cfg.d_508F.Data.DeadHarmoicValue;
	}
	else if
	(
		((DataId>=0x4040)&&(DataId<=0x4065))
		||
		((DataId>=0x408A)&&(DataId>=0x40af))
		||
		((DataId>=0x40D4)&&(DataId>=0x40f9))
		||
		((DataId>=0x4139)&&(DataId>=0x415e))
		||
		((DataId>=0x4183)&&(DataId>=0x41A8))
		||
		((DataId>=0x41CD)&&(DataId>=0x41f2))
	)
	{
		return scada_cfg.d_508F.Data.DeadHarmoicCount;
	}
	else if
	(
		(DataId==0x4114)
		||(DataId==0x401b)
	)
	{
		return scada_cfg.d_508F.Data.DeadFreq;
	}
	else if
	(
		(DataId==0x4401)
	)
	{
		return scada_cfg.d_508F.Data.DeadAngle;
	}
	else if(DataId==0x4606)
	{
		return scada_cfg.d_508F.Data.DeadHumidity;
	}
	else if (DataId==0x4605)
	{
		return scada_cfg.d_508F.Data.DeadTemp;
	}else {
		return 0.01;
	}
}
/*
*************************************************************************************
*  @brief  越死区值计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
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

void Scada:: CheckDeadData(void)
{	uint16  i ;

	Data<BaseDataType> yctmp;
	DataType4 *Pyc = NULL;
	DataType4 *PycDatatab = NULL;
	CP56Time2a Time;
    Pyc  = &YcData.ua1_4001;//scada 遥测表
	for (i = 0; i < MAXYCDATANUM; i++){//MAXYCDATANUM
		PycDatatab =(DataType4 *)(user_data.RecallData(YCTAB,i)->Data);//遥测表
		if (IsOverDeadband(Pyc[i].Data,PycDatatab->Data,GetDead(user_data.RecallData(YCTAB,i)->Id)==1)){
			if(clk0.CheckMsClock(&YcDeadTime[i])==1){
				yctmp.Data = &Pyc[i];
				user_data.SetData(user_data.RecallData(YCTAB,i)->Id,&yctmp);//更新遥测表
				Time = clk0.GetCP56Time2a();
				msg.SendMsg(TASK_VARM,MSG_YC,user_data.RecallData(YCTAB,i)->Id,&Pyc[i],&Time);
			}else{
				clk0.StartMsClock(&YcDeadTime[i],20000);//默认20秒；
			}
		}else{
			clk0.StopMsClock(&YcDeadTime[i]);
		}
	}
}
/*
*************************************************************************************
*  @brief  交采量计算
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
int16 Scada::Caculate(){
	AdCopy();//AD采集数据的备份；
	GetModulus();//各个通道的模值；
	if (scada_cfg.StartClbrt==BDSTOP){
		GetRms();//有效值；
		GetHarmonicData();//谐波计算；
		GetPQS();//功率计算；
		CheckDeadData();//越死区；
		ChecksoftYx();//越限；
		SendYx();//保护产生的遥信，硬遥信，发送到Varm
	}else {
		clk0.StartClock(&scada_cfg.ClbrtTime,0,0,0,0,0,10000);//标定超时，10秒，
		Calibration();//标定
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

void Scada::first_compare(float *max,float *min,float var)
{
	if(FloatCompare ((*max), var)==1){
		(*min) = var;
	}else {
		(*min) = (*max);
		(*max) = var;
	}

}
void Scada::GetXishu(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue )
{
	if((*sourceyc)>minvalue){

		(*xishu) = (float)(*standardyc)/(*sourceyc);

	}

}
/*
*************************************************************************************
*  @brief   标定      标定参数（DataId = 5097) 下发1 开始标定，标定后 向Varm 发送 参数（DataId = 5097)
*  			值为第一位为fpag1 板 1成功0失败，第二位为fpga2板 1成功0失败， 超时发送0xff；
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada::Calibration(void)
{
	int8 erro[2];
	power_t  powertmp;
	uint8 Clbrtnum;
	uint8 i,j;
	DataType2		d_5097;///<标定
	float angle;
	CP56Time2a Time;
	scada_cfg.Clbrtnum++;
	Clbrtnum = scada_cfg.Clbrtnum;
	if(Clbrtnum==1){
		for (i = 0; i < 26; i++ ){
			scada_cfg.ClbrtTmp[i].max= TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN);
			scada_cfg.ClbrtTmp[i].avr = 0;
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				scada_cfg.ClbrtPowerTmp[i][j].max = GetPower(UA(i)+j,IA1(i)+j);
				scada_cfg.ClbrtPowerTmp[i][j].avr.p = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.q = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.s = 0.0;
			}
		}
	}
	if(Clbrtnum==2){
		for (i = 0; i< 26;i++){
			 first_compare(&scada_cfg.ClbrtTmp[i].max,&scada_cfg.ClbrtTmp[i].min,TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN));
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				powertmp = GetPower(UA(i)+j,IA1(i)+j);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,powertmp.p);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,powertmp.q);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,powertmp.s);
			}
		}
	}
	if((Clbrtnum>=3)&&(Clbrtnum<=(AVERAGENUM + 2))){
		for (i = 0; i< 26;i++){
			compare(&scada_cfg.ClbrtTmp[i].max,&scada_cfg.ClbrtTmp[i].min,&scada_cfg.ClbrtTmp[i].avr,TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN),AVERAGENUM);
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				powertmp = GetPower(UA(i)+j,IA1(i)+j);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,&scada_cfg.ClbrtPowerTmp[i][j].avr.p,powertmp.p,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,&scada_cfg.ClbrtPowerTmp[i][j].avr.q,powertmp.q,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,&scada_cfg.ClbrtPowerTmp[i][j].avr.s,powertmp.s,AVERAGENUM);
			}
		}
	}
	if(Clbrtnum==(AVERAGENUM + 2)){
		memcpy(&scada_cfg.YcRatioTmp,&YcRatio,sizeof(scada_cfg.YcRatioTmp));
		for (i = 0; i < 13; i++){
			GetXishu(&scada_cfg.YcRatioTmp[0].PortRatio[i], &scada_cfg.ClbrtTmp[i].avr,&scada_cfg.StandYc[0].PortRatio[i],BIAODINGMIN );
		}
		for (i = 0; i < 13; i++){
			GetXishu(&scada_cfg.YcRatioTmp[1].PortRatio[i], &scada_cfg.ClbrtTmp[i+13].avr,&scada_cfg.StandYc[1].PortRatio[i],BIAODINGMIN );
		}
		for ( i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				scada_cfg.YcRatioTmp[i].PowerLineRatio[j]  = scada_cfg.YcRatioTmp[i].PortRatio[UA1+j]
				                                          * scada_cfg.YcRatioTmp[i].PortRatio[IA11+j];
				powertmp = scada_cfg.ClbrtPowerTmp[i][j].avr;
				angle = asin((scada_cfg.YcRatioTmp[i].PowerLineRatio[j]*powertmp.q)/scada_cfg.StandYc[i].PowerLineRatio[j])-(PI / 6.0);
				scada_cfg.YcRatioTmp[i].PowerAngleRatio[0][j].sin_a =sinsp(angle);
				scada_cfg.YcRatioTmp[i].PowerAngleRatio[0][j].cos_a =cossp(angle);
			}
		}
		scada_cfg.StartClbrt=BDSTOP;
		scada_cfg.Clbrtnum = 0;
		//scada_cfg.ClbrtTime.SetFlag = 0;
		clk0.StopClock(&scada_cfg.ClbrtTime);
		for(i = 0;i<FpgaCfg.YcCardSize;i++){
			erro[i]=FpgaWriteData(FpgaCfg.YcCard[i].num,(uint16 *)&(scada_cfg.YcRatioTmp[i]) ,sizeof(ratio_t)/2);
			if(erro[i]==1){
				memcpy(&YcRatio[i],&scada_cfg.YcRatioTmp[i],sizeof(ratio_t));
			}
		}
		d_5097.Data = erro[0] + erro[1]<<1 ;
		Time = clk0.GetCP56Time2a();
		msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
	}
	if (clk0.CheckClock(&scada_cfg.ClbrtTime)){//超时了
		d_5097.Data = BD_TIMEOVER;
		Time = clk0.GetCP56Time2a();
		msg.SendMsg(TASK_VARM,MSG_PARA,0x5097,&d_5097,&Time);
		scada_cfg.StartClbrt=BDSTOP;
		scada_cfg.Clbrtnum = 0;
	}
}
