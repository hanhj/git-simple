/*
 * ad_samp.cpp
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */


//#include <math.h>
#include "adsamp.h"
#include "mathlist.h"
#include "datatype.h"
#include "types.h"
#include "message.h"
#include "scada.h"
#include "interruptapp.h"
#include "fpga.h"
#include "gpio.h"
#include "fft.h"
#include "omapl138.h"
#include "omapl138timer.h"
#include "dspdatatype.h"
#include "yx.h"
#pragma DATA_SECTION(".ADBUFDATA");
#pragma DATA_ALIGN(8);
SP_COMPLEX AD_BUF[CHENAL_NUM][SAMP_NUM+2*PAD] = {0};

#pragma DATA_SECTION(".ADBUFDATA");
SP_COMPLEX AD16_BUF[CHENAL_NUM][PROSAMP_NUM+2*PAD] = {0};
#pragma DATA_ALIGN(8);
#pragma DATA_SECTION(".ADBUDATA");
int16 AD_BU[CHENAL_NUM][SAMP_NUM+2*PAD] = {0};
//SP_COMPLEX AD_BU[CHENAL_NUM][SAMP_NUM+2*PAD] = {0};

#pragma DATA_SECTION(".FFTOUTDATA");
#pragma DATA_ALIGN(8);
SP_COMPLEX FFT_OUT[CHENAL_NUM][PROSAMP_NUM+2*PAD] = {0};

#pragma DATA_SECTION(".LINEVOLDATA");
#pragma DATA_ALIGN(8);
SP_COMPLEX LineVol[CHENAL_NUM]= {0};


#pragma DATA_SECTION(".ptr_wDATA");
#pragma DATA_ALIGN(8);
float w64[M + 2 * PAD];

#pragma DATA_SECTION(".ptr_wDATA");
#pragma DATA_ALIGN(8);
float w16[M + 2 * PAD];
#pragma DATA_SECTION(".yc_configureDATA");
ADCONFIG yc_configure;
//JCENUM jcenum;

uint8 BreakLockState[2];

ratio_t  YcRatio[MAX_LINE_NUM];





MANVALUE man_set;
#define YXFPGA 0
#define YCFPGA 1
#define YKFPGA 2
#define FPGAADDR1 0
#define FPGAADDR2 0
#define FPGAADDR3 0
#define FPGAADDR4 0
//最多有四块FPGA板，每块板上固定有20个交采量(共计80个交采量)，虽然每块板没有20个交采量，但是还是需要读出来
uint8 FFT_GAP = 16;
#pragma DATA_SECTION(".ADBUDATA");
unsigned int ad_point = 0;
void check_hyx(void);
void ManSet(void);


/*
*************************************************************************************
*  @brief  不可屏蔽中断   功能：从FPGA 读取遥测数据 GPIO2_0
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************


/*
*************************************************************************************
*  @brief   从FPGA 读取遥测值，放入AD_BU中
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
/*
 Ua 16
 ub 17
 uc 18
 u0 19
 ia1 8
 ib1 9
 ic1 10
 i0 11
 ia2 12
 ib2 13
 ic2 14
 */
int32 IrtNum  = 0;
#pragma CODE_SECTION("codenmifxn")
Void nmifxn(UArg arg)
{
	uint16 tpm =0;
	uint16 ycfpganum = 0;
	uint16 pos = 0;
	uint16 i;

//CLRBIT(GPIO_BANK8_->OUT_DATA, 0x00008000);

if(scada.scada_cfg.d_5211.Data == 0){
	//if(FpgaHost.DATA_IRQ.bit.fpga4 == 1){
	for( i = 0;i<FpgaCfg.YcCardSize;i++){
		pos = i*12;
		ycfpganum =FpgaCfg.YcCard[i].num;
		/*AD_BU[12][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//0
		AD_BU[13][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//1
		AD_BU[14][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//2
		AD_BU[15][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//3
		AD_BU[16][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//4
		AD_BU[17][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//5
		AD_BU[18][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//6
		AD_BU[19][ad_point] = FpgaSlave[FpgaCfg.YcCard[i]].YC;//7*/
		tpm = FpgaSlave[ycfpganum].YC;//0
		tpm = FpgaSlave[ycfpganum].YC;//1
		tpm = FpgaSlave[ycfpganum].YC;//2
		tpm = FpgaSlave[ycfpganum].YC;//3
		tpm = FpgaSlave[ycfpganum].YC;//4
		tpm = FpgaSlave[ycfpganum].YC;//5
		tpm = FpgaSlave[ycfpganum].YC;//6
		tpm = FpgaSlave[ycfpganum].YC;//7
		AD_BU[4+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//8
		AD_BU[5+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//9
		AD_BU[6+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//10
		AD_BU[7+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//11
		AD_BU[8+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//12
		AD_BU[9+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//13
		AD_BU[10+pos][ad_point] = FpgaSlave[ycfpganum].YC;//14
		AD_BU[11+pos][ad_point] = FpgaSlave[ycfpganum].YC;//15
		AD_BU[0+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//16
		AD_BU[1+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//17
		AD_BU[2+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//18
		AD_BU[3+pos][ad_point] =  FpgaSlave[ycfpganum].YC;//19
	}
	//}else{
	//	tpm = FpgaSlave[4].YC;
	//	return ;
//	}
}else{
	tpm = FpgaSlave[ycfpganum].YC;
	return ;
}
	ad_point++;
	if (ad_point>=SAMP_NUM){//采样到一个周期
		ad_point = 0;
	}
	if (ad_point%3==0){
		GetYx();
	}
	switch (ad_point){
		case 15:
		case 31:
		case 47:
		case 64:
		Swi_post(SwiFftHandle);
		break;
		default:
		break;
	}
}
/*
*************************************************************************************
*  @brief   人工制设置
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
/*struct
{
	typedef struct _BaseSet{
		float BaseValue;//基波有效值
		float BaseHz;
		int16 HarmoDgree;//谐波次数
		float HarmContent;//谐波含量
	}BaseSet;
	BaseSet Ua;
	BaseSet Ub;
	BaseSet Uc;
	BaseSet Ia;
	BaseSet Ib;
	BaseSet Ic;
	float   IaAngle;//电压电流角度
	float   IbAngle;
	float   IcAngle;
}Data;*/


float Urms(uint8 n,uint8 line)
{
	switch (n)
	{case 0:
		return  (scada.scada_cfg.d_5212.Data.Ua.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[UA1];//

	case 1:
		return  (scada.scada_cfg.d_5212.Data.Ub.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[UB1];//Ubrms;

	case 2:
		return  (scada.scada_cfg.d_5212.Data.Uc.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[UC1];//Ucrms;

	default:

		return 0;
	}
	//return 0;
}
float Irms(uint8 n,uint8 line)
{
	switch (n)
	{case 0:
		return  (scada.scada_cfg.d_5212.Data.Ia.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[IA11];//Iarms;

	case 1:
		return  (scada.scada_cfg.d_5212.Data.Ib.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[IB11];;//Ibrms;

	case 2:
		return  (scada.scada_cfg.d_5212.Data.Ic.BaseValue * RMSRATIO)/YcRatio[line].PortRatio[IC11];;//Icrms;

	default:

		return 0;
	}
	//return 0;
}
float U_harmonic_num(uint8 n)
{
	switch (n)
	{case 0:
		return   scada.scada_cfg.d_5212.Data.Ua.HarmoDgree;//Ua_harmonic_num;

	case 1:
		return   scada.scada_cfg.d_5212.Data.Ub.HarmoDgree;//Ub_harmonic_num;

	case 2:
		return   scada.scada_cfg.d_5212.Data.Uc.HarmoDgree;//Uc_harmonic_num;

	default:

		return 0;
	}
	//return 0;
}
float U_harmonic_radio(uint8 n)
{
	switch (n)
	{
	case 0:
		return  scada.scada_cfg.d_5212.Data.Ua.HarmContent;//Ua_harmonic_radio;

	case 1:
		return  scada.scada_cfg.d_5212.Data.Ub.HarmContent;//Ub_harmonic_radio;

	case 2:
		return  scada.scada_cfg.d_5212.Data.Uc.HarmContent;//Uc_harmonic_radio;

	default:

		return 0;
	}
	//return 0;
}
float I_harmonic_num(uint8 n)
{
	switch (n)
	{case 0:
		return  (scada.scada_cfg.d_5212.Data.Ia.HarmoDgree );//Ia_harmonic_num;

	case 1:
		return  (scada.scada_cfg.d_5212.Data.Ib.HarmoDgree );//Ib_harmonic_num;

	case 2:
		return  (scada.scada_cfg.d_5212.Data.Ic.HarmoDgree );//Ic_harmonic_num;

	default:

		return 0;
	}
	//return 0;
}
float I_harmonic_radio(uint8 n)
{
	switch (n)
	{case 0:
		return  scada.scada_cfg.d_5212.Data.Ia.HarmContent;//Ia_harmonic_radio;

	case 1:
		return  scada.scada_cfg.d_5212.Data.Ib.HarmContent;//Ib_harmonic_radio;

	case 2:
		return  scada.scada_cfg.d_5212.Data.Ic.HarmContent;//Ic_harmonic_radio;

	default:

		return 0;
	}
	//return 0;
}
float AngleUI(uint8 n)
{
	switch (n)
	{
		case 0:
			return  scada.scada_cfg.d_5212.Data.IaAngle*HUDUXISHI;//AngleUaIa;
		case 1:
			return  scada.scada_cfg.d_5212.Data.IbAngle*HUDUXISHI;//AngleUbIb;
		case 2:
			return  scada.scada_cfg.d_5212.Data.IcAngle*HUDUXISHI;//AngleUcIc;
		default:
			return 0;
	}
	//return 0;
}

void ManSet(void)
{
	uint8 i;
	uint8 j;
	for ( i = 0; i < SAMP_NUM;i++){

		// 1 #进线零序电压
//		AD_BU[yc_configure.ad_compass[U01]][i+PAD] = U0_App * cossp (TWO_PI *( i /SAMP_NUM) - U0_angle)+  U0_App * U0_harmonic_radio * cossp (U0_harmonic_num * TWO_PI * (i /SAMP_NUM));

		// 2 #进线零序电压
//		AD_BU[yc_configure.ad_compass[U02]][i+PAD] = AD_BU[yc_configure.ad_compass[U01]][i+PAD];

		// 3 #进线零序电压
/*		AD_BU[yc_configure.ad_compass[U03]][i+PAD].rel = AD_BU[yc_configure.ad_compass[U01]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[U03]][i+PAD].img = 0;
		// 4 #进线零序电压
		AD_BU[yc_configure.ad_compass[U04]][i+PAD].rel = AD_BU[yc_configure.ad_compass[U01]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[U04]][i+PAD].img = 0;
*/

		// 1 #进线零序电流1
//		AD_BU[yc_configure.ad_compass[I011]][i+PAD] = I0_App * cossp (TWO_PI *( i /SAMP_NUM) - I0_angle)+  I0_App * I0_harmonic_radio * cossp (I0_harmonic_num * TWO_PI * (i /SAMP_NUM));
/*
		// 1 #进线零序电流2
		AD_BU[yc_configure.ad_compass[I012]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];

        // 1 #进线零序电流3
		AD_BU[yc_configure.ad_compass[I013]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];

        // 1 #进线零序电流4
		AD_BU[yc_configure.ad_compass[I014]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];
		// 2 #进线零序电流1
		AD_BU[yc_configure.ad_compass[I021]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];

		// 2 #进线零序电流2
		AD_BU[yc_configure.ad_compass[I022]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];
		// 2 #进线零序电流3
		AD_BU[yc_configure.ad_compass[I023]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];

       	// 2 #进线零序电流4
		AD_BU[yc_configure.ad_compass[I024]][i+PAD] = AD_BU[yc_configure.ad_compass[I011]][i+PAD];
		// 3 #进线零序电流1
*/
	/*	AD_BU[yc_configure.ad_compass[I031]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I031]][i+PAD].img = 0;
		// 3 #进线零序电流2
		AD_BU[yc_configure.ad_compass[I032]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I032]][i+PAD].img = 0;
		// 3 #进线零序电流3
		AD_BU[yc_configure.ad_compass[I033]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I033]][i+PAD].img = 0;
		// 3 #进线零序电流4
		AD_BU[yc_configure.ad_compass[I034]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I034]][i+PAD].img = 0;
		// 4 #进线零序电流1
		AD_BU[yc_configure.ad_compass[I041]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I041]][i+PAD].img = 0;
		// 4 #进线零序电流2
		AD_BU[yc_configure.ad_compass[I042]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I042]][i+PAD].img = 0;
		// 4 #进线零序电流3
		AD_BU[yc_configure.ad_compass[I043]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I043]][i+PAD].img = 0;
		// 4 #进线零序电流4
		AD_BU[yc_configure.ad_compass[I044]][i+PAD].rel = AD_BU[yc_configure.ad_compass[I011]][i+PAD].rel;
		AD_BU[yc_configure.ad_compass[I044]][i+PAD].img = 0;*/
	}

	for ( j = 0; j < 3; j++){

		// 1 #进线电压
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[UA1+j]][i+PAD] =
			Urms(j,0) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI)+
			Urms(j,0) * U_harmonic_radio(j) * cossp (U_harmonic_num(j) * TWO_PI * ((float)i /(float)SAMP_NUM));
		}
		// 1 # 电流1
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA11+j]][i+PAD] =
			Irms(j,0) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI - AngleUI(j))+
			Irms(j,0) * I_harmonic_radio(j) * cossp (I_harmonic_num (j)* TWO_PI * ((float)i /(float)SAMP_NUM));
		}

		// 1 # 电流2
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA12+j]][i+PAD] =
			(Irms(j,0)/10.0) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI - AngleUI(j))+
			(Irms(j,0)/10.0) * I_harmonic_radio(j) * cossp (I_harmonic_num(j) * TWO_PI * ((float)i /(float)SAMP_NUM));
		}

		// 1 # 电流3
/*		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA13+j]][i+PAD] = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+  I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));

		}

		// 1 # 电流4
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA14+j]][i+PAD] = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+  I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));

		}

*/

		// 2 #进线电压
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[UA2+j]][i+PAD] =
			Urms(j,1) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI)+ 100.0+
			Urms(j,1) * U_harmonic_radio(j) * cossp (U_harmonic_num(j) * TWO_PI * ((float)i /(float)SAMP_NUM));

		}
		// 2 # 电流1
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA21+j]][i+PAD] =
			Irms(j,1) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI - AngleUI(j))+
			Irms(j,1) * I_harmonic_radio(j) * cossp (I_harmonic_num (j)* TWO_PI * ((float)i /(float)SAMP_NUM));
		}

		// 2 # 电流2
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA22+j]][i+PAD] =
			(Irms(j,1)/10.0) * cossp (TWO_PI *( (float)i /(float)SAMP_NUM) - j*TWOOFTHREE_PI - AngleUI(j))+
			(Irms(j,1)/10.0) * I_harmonic_radio(j) * cossp (I_harmonic_num(j) * TWO_PI * ((float)i /(float)SAMP_NUM));
		}

/*		// 2 # 电流3
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA23+j]][i+PAD] = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
		}

		// 2 # 电流4
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA24+j]][i+PAD] = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
		}
*/
/*
		// 3 #进线电压
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[UA3+j]][i+PAD].rel = U_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI)+  U_App * U_harmonic_radio * cossp (U_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[UA3+j]][i+PAD].img = 0;
		}
		// 3 # 电流1
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA31+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA31+j]][i+PAD].img = 0;
		}

		// 3 # 电流2
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA32+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA32+j]][i+PAD].img = 0;
		}
		// 3 # 电流3
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA33+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA33+j]][i+PAD].img = 0;
		}

		// 3 # 电流4
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA34+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA34+j]][i+PAD].img = 0;
		}


		// 4 #进线电压
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[UA4+j]][i+PAD].rel = U_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI)+  U_App * U_harmonic_radio * cossp (U_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[UA4+j]][i+PAD].img = 0;
		}
		// 4 # 电流1
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA41+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA41+j]][i+PAD].img = 0;
		}
		// 4 # 电流2
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA42+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA42+j]][i+PAD].img = 0;
		}
		// 4 # 电流3
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA43+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA43+j]][i+PAD].img = 0;
		}
		// 4 # 电流4
		for ( i = 0; i < SAMP_NUM;i++){
			AD_BU[yc_configure.ad_compass[IA44+j]][i+PAD].rel = I_App * cossp (TWO_PI *( i /SAMP_NUM) - j*TWOOFTHREE_PI - angle)+	I_App * I_harmonic_radio * cossp (I_harmonic_num * TWO_PI * (i /SAMP_NUM));
			AD_BU[yc_configure.ad_compass[IA44+j]][i+PAD].img = 0;
		}*/
	}
}

#pragma CODE_SECTION("codenmifxn")
void AdCopy(void)
{
	uint8 i;
	unsigned int N;
	unsigned int yc_pos;
 	for(yc_pos = 0; yc_pos < yc_configure.yc_num; yc_pos++){
		N = yc_configure.ad_compass[yc_configure.name[yc_pos]];
		if(N!=NO_YC){
				for ( i = 0; i < 64; i++){
					AD_BUF[N][i].img = 0;
				}
				AD_BUF[N][0].rel = AD_BU[N][0];
				AD_BUF[N][1].rel = AD_BU[N][1];
				AD_BUF[N][2].rel = AD_BU[N][2];
				AD_BUF[N][3].rel = AD_BU[N][3];
				AD_BUF[N][4].rel = AD_BU[N][4];
				AD_BUF[N][5].rel = AD_BU[N][5];
				AD_BUF[N][6].rel = AD_BU[N][6];
				AD_BUF[N][7].rel = AD_BU[N][7];///

				AD_BUF[N][8].rel =  AD_BU[N][8];
				AD_BUF[N][9].rel =  AD_BU[N][9];
				AD_BUF[N][10].rel = AD_BU[N][10];
				AD_BUF[N][11].rel = AD_BU[N][11];
				AD_BUF[N][12].rel = AD_BU[N][12];
				AD_BUF[N][13].rel = AD_BU[N][13];
				AD_BUF[N][14].rel = AD_BU[N][14];
				AD_BUF[N][15].rel = AD_BU[N][15];///

				AD_BUF[N][16].rel = AD_BU[N][16];
				AD_BUF[N][17].rel = AD_BU[N][17];
				AD_BUF[N][18].rel = AD_BU[N][18];
				AD_BUF[N][19].rel = AD_BU[N][19];
				AD_BUF[N][20].rel = AD_BU[N][20];
				AD_BUF[N][21].rel = AD_BU[N][21];
				AD_BUF[N][22].rel = AD_BU[N][22];
				AD_BUF[N][23].rel = AD_BU[N][23];///

				AD_BUF[N][24].rel = AD_BU[N][24];
				AD_BUF[N][25].rel = AD_BU[N][25];
				AD_BUF[N][26].rel = AD_BU[N][26];
				AD_BUF[N][27].rel = AD_BU[N][27];
				AD_BUF[N][28].rel = AD_BU[N][28];
				AD_BUF[N][29].rel = AD_BU[N][29];
				AD_BUF[N][30].rel = AD_BU[N][30];
				AD_BUF[N][31].rel = AD_BU[N][31];///

				AD_BUF[N][32].rel = AD_BU[N][32];
				AD_BUF[N][33].rel = AD_BU[N][33];
				AD_BUF[N][34].rel = AD_BU[N][34];
				AD_BUF[N][35].rel = AD_BU[N][35];
				AD_BUF[N][36].rel = AD_BU[N][36];
				AD_BUF[N][37].rel = AD_BU[N][37];
				AD_BUF[N][38].rel = AD_BU[N][38];
				AD_BUF[N][39].rel = AD_BU[N][39];///

				AD_BUF[N][40].rel = AD_BU[N][40];
				AD_BUF[N][41].rel = AD_BU[N][41];
				AD_BUF[N][42].rel = AD_BU[N][42];
				AD_BUF[N][43].rel = AD_BU[N][43];
				AD_BUF[N][44].rel = AD_BU[N][44];
				AD_BUF[N][45].rel = AD_BU[N][45];
				AD_BUF[N][46].rel = AD_BU[N][46];
				AD_BUF[N][47].rel = AD_BU[N][47];///

				AD_BUF[N][48].rel = AD_BU[N][48];
				AD_BUF[N][49].rel = AD_BU[N][49];
				AD_BUF[N][50].rel = AD_BU[N][50];
				AD_BUF[N][51].rel = AD_BU[N][51];
				AD_BUF[N][52].rel = AD_BU[N][52];
				AD_BUF[N][53].rel = AD_BU[N][53];
				AD_BUF[N][54].rel = AD_BU[N][54];
				AD_BUF[N][55].rel = AD_BU[N][55];///

				AD_BUF[N][56].rel = AD_BU[N][56];
				AD_BUF[N][57].rel = AD_BU[N][57];
				AD_BUF[N][58].rel = AD_BU[N][58];
				AD_BUF[N][59].rel = AD_BU[N][59];
				AD_BUF[N][60].rel = AD_BU[N][60];
				AD_BUF[N][61].rel = AD_BU[N][61];
				AD_BUF[N][62].rel = AD_BU[N][62];
				AD_BUF[N][63].rel = AD_BU[N][63];///


		}
	}
}

#pragma CODE_SECTION("codenmifxn")
void ProAdCopy(void)
{
	unsigned int N;
	unsigned int yc_pos;
 	for(yc_pos = 0; yc_pos < yc_configure.yc_num; yc_pos++){
		N = yc_configure.ad_compass[yc_configure.name[yc_pos]];
		if(N!=NO_YC){
			{
				AD16_BUF[N][0].rel = AD_BU[N][0];
				AD16_BUF[N][1].rel = AD_BU[N][4];
				AD16_BUF[N][2].rel = AD_BU[N][8];
				AD16_BUF[N][3].rel = AD_BU[N][12];
				AD16_BUF[N][4].rel = AD_BU[N][16];
				AD16_BUF[N][5].rel = AD_BU[N][20];
				AD16_BUF[N][6].rel = AD_BU[N][24];
				AD16_BUF[N][7].rel = AD_BU[N][28];///
				AD16_BUF[N][8].rel =  AD_BU[N][32];
				AD16_BUF[N][9].rel =  AD_BU[N][36];
				AD16_BUF[N][10].rel = AD_BU[N][40];
				AD16_BUF[N][11].rel = AD_BU[N][44];
				AD16_BUF[N][12].rel = AD_BU[N][48];
				AD16_BUF[N][13].rel = AD_BU[N][52];
				AD16_BUF[N][14].rel = AD_BU[N][56];
				AD16_BUF[N][15].rel = AD_BU[N][60];///
				AD16_BUF[N][0].img = 0;
				AD16_BUF[N][1].img = 0;
				AD16_BUF[N][2].img = 0;
				AD16_BUF[N][3].img = 0;
				AD16_BUF[N][4].img = 0;
				AD16_BUF[N][5].img = 0;
				AD16_BUF[N][6].img = 0;
				AD16_BUF[N][7].img = 0;
				AD16_BUF[N][8].img = 0;
				AD16_BUF[N][9].img = 0;
				AD16_BUF[N][10].img = 0;
				AD16_BUF[N][11].img = 0;
				AD16_BUF[N][12].img = 0;
				AD16_BUF[N][13].img = 0;
				AD16_BUF[N][14].img = 0;
				AD16_BUF[N][15].img = 0;
			}
		}
	}
}


