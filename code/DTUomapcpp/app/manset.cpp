/*
 * manset.cpp
 *
 *  Created on: 2020-6-18
 *      Author: hanhj
 */
#include <stdio.h>
#include <string.h>
#include "adsamp.h"
#include "mathlist.h"
#include "datatype.h"
#include "types.h"
#include "message.h"
#include "scada.h"
#include "interruptapp.h"
#include "fpga.h"
#include "gpio.h"
#include "fa_fft_cal.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "dspdatatype.h"
#include "clock.h"
#include "utils.h"
#include "edma3_interface.h"
/*
*************************************************************************************
*  @brief   人工置数
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
float Urms(uint8 n,uint8 line)
{
	switch (n)
	{
	case 0:
		return  (scada.scada_cfg.d_5214.Data.Ua.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[UA1];//
	case 1:
		return  (scada.scada_cfg.d_5214.Data.Ub.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[UB1];//Ubrms;
	case 2:
		return  (scada.scada_cfg.d_5214.Data.Uc.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[UC1];//Ucrms;
	case 3:
		return  (scada.scada_cfg.d_5214.Data.Uc.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[U01];//Ucrms;
	default:

		return 0;
	}
	//return 0;
}
float Irms(uint8 n,uint8 line)
{
	switch (n)
	{
	case 0:
		return  (scada.scada_cfg.d_5214.Data.Ia.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IA11];//Iarms;

	case 1:
		return  (scada.scada_cfg.d_5214.Data.Ib.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IB11];//Ibrms;

	case 2:
		return  (scada.scada_cfg.d_5214.Data.Ic.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IC11];//Icrms;

	case 3:
		return  (scada.scada_cfg.d_5214.Data.Ia.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[I01];//Iarms;

	case 4:
		return  (scada.scada_cfg.d_5214.Data.Ia.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IA12];//Iarms;

	case 5:
		return  (scada.scada_cfg.d_5214.Data.Ib.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IB12];//Ibrms;

	case 6:
		return  (scada.scada_cfg.d_5214.Data.Ic.BaseValue * RMSRATIO*DEVXISHU)/YcRatio[line].PortRatio[IC12];//Icrms;

	default:

		return 0;
	}
	//return 0;
}

float U_harmonic_num(uint8 n)
{
	switch (n)
	{case 0:
		return   scada.scada_cfg.d_5214.Data.Ua.HarmoDgree;//Ua_harmonic_num;

	case 1:
		return   scada.scada_cfg.d_5214.Data.Ub.HarmoDgree;//Ub_harmonic_num;

	case 2:
		return   scada.scada_cfg.d_5214.Data.Uc.HarmoDgree;//Uc_harmonic_num;

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
		return  scada.scada_cfg.d_5214.Data.Ua.HarmContent/PERCENT;//Ua_harmonic_radio;

	case 1:
		return  scada.scada_cfg.d_5214.Data.Ub.HarmContent/PERCENT;//Ub_harmonic_radio;

	case 2:
		return  scada.scada_cfg.d_5214.Data.Uc.HarmContent/PERCENT;//Uc_harmonic_radio;

	default:

		return 0;
	}
	//return 0;
}
float I_harmonic_num(uint8 n)
{
	switch (n)
	{case 0:
		return  (scada.scada_cfg.d_5214.Data.Ia.HarmoDgree );//Ia_harmonic_num;

	case 1:
		return  (scada.scada_cfg.d_5214.Data.Ib.HarmoDgree );//Ib_harmonic_num;

	case 2:
		return  (scada.scada_cfg.d_5214.Data.Ic.HarmoDgree );//Ic_harmonic_num;

	default:

		return 0;
	}
	//return 0;
}
float I_harmonic_radio(uint8 n)
{
	switch (n)
	{case 0:
		return  scada.scada_cfg.d_5214.Data.Ia.HarmContent/PERCENT;//Ia_harmonic_radio;

	case 1:
		return  scada.scada_cfg.d_5214.Data.Ib.HarmContent/PERCENT;//Ib_harmonic_radio;

	case 2:
		return  scada.scada_cfg.d_5214.Data.Ic.HarmContent/PERCENT;//Ic_harmonic_radio;

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
			return  scada.scada_cfg.d_5214.Data.IaAngle*HUDUXISHI;//AngleUaIa;
		case 1:
			return  scada.scada_cfg.d_5214.Data.IbAngle*HUDUXISHI;//AngleUbIb;
		case 2:
			return  scada.scada_cfg.d_5214.Data.IcAngle*HUDUXISHI;//AngleUcIc;
		default:
			return 0;
	}
	//return 0;
}
float freqdert = 1.0;
void ManSet(void)
{
	uint8 line;
	uint8 i;
	uint8 phase;
	uint16 port;
	float yc;
	for ( line=0 ; line < MAX_LINE_NUM; line++){
		for ( phase = 0; phase < 3; phase++){
			//三相电压
			port = scada.yc_config.config_data[UA(line)+phase].port;
			if(port!=NO_YC){
				for ( i = 0; i < (SAMP_NUM);i++){
					yc=
					Urms(phase,LINE(line)) * cossp (TWO_PI *freqdert*( (float)i /(float)SAMP_NUM) - phase*TWOOFTHREE_PI)+
					Urms(phase,LINE(line)) * U_harmonic_radio(phase) * cossp (U_harmonic_num(phase) * TWO_PI * freqdert*((float)i /(float)SAMP_NUM));
					yc/=YcRatio[line].PortRatio[UA1+phase];
					AD_BUF_ori[port][i] = yc;
				}
			}
			//三相测量电流
			port =scada.yc_config.config_data[IA1(line)+phase].port;
			if(port!=NO_YC){
				for ( i = 0; i < (SAMP_NUM);i++){
					yc=
					Irms(phase,LINE(line)) * cossp (TWO_PI *freqdert*( (float)i /(float)SAMP_NUM) - phase*TWOOFTHREE_PI - AngleUI(phase))+
					Irms(phase,LINE(line)) * I_harmonic_radio(phase) * cossp (I_harmonic_num (phase)* TWO_PI * freqdert*((float)i /(float)SAMP_NUM));
					yc/=YcRatio[line].PortRatio[IA11+phase];
					AD_BUF_ori[port][i] = yc;
				}
			}
			//三相保护测量电流
			port =scada.yc_config.config_data[IA2(line)+phase].port;
			if(port!=NO_YC){
				for ( i = 0; i < (SAMP_NUM);i++){
					yc=
					AD_BUF_ori[scada.yc_config.config_data[IA2(line)+phase].port][i] =
					Irms(phase+4,LINE(line)) * cossp (TWO_PI *freqdert*( (float)i /(float)SAMP_NUM) - phase*TWOOFTHREE_PI - AngleUI(phase))+
					Irms(phase+4,LINE(line)) * I_harmonic_radio(phase) * cossp (I_harmonic_num(phase) * TWO_PI * freqdert*((float)i /(float)SAMP_NUM));
					yc/=YcRatio[line].PortRatio[IA12+phase];
					AD_BUF_ori[port][i] = yc;
				}
			}
		}

		port =scada.yc_config.config_data[U0(line)].port;
		if(port!=NO_YC){
			for ( i = 0; i < (SAMP_NUM);i++){
				yc=
				Urms(3,LINE(line)) * cossp (TWO_PI *freqdert*( (float)i /(float)SAMP_NUM) - 0*TWOOFTHREE_PI)+
				Urms(3,LINE(line)) * U_harmonic_radio(0) * cossp (U_harmonic_num(0) * TWO_PI * freqdert*((float)i /(float)SAMP_NUM));
				yc/=YcRatio[line].PortRatio[U01];
				AD_BUF_ori[port][i] = yc;
			}
		}
		port =scada.yc_config.config_data[I0(line)].port;
		if(port!=NO_YC){
			for ( i = 0; i < (SAMP_NUM);i++){
				yc =
				Irms(3,LINE(line)) * cossp (TWO_PI *freqdert*( (float)i /(float)SAMP_NUM) - 0*TWOOFTHREE_PI - AngleUI(0))+
				Irms(3,LINE(line)) * I_harmonic_radio(0) * cossp (I_harmonic_num (0)* TWO_PI * freqdert*((float)i /(float)SAMP_NUM));
				yc/=YcRatio[line].PortRatio[I01];
				AD_BUF_ori[port][i] = yc;
			}
		}
	}
	memcpy(&AD_BUF_scada,&AD_BUF_ori,7680);
}


