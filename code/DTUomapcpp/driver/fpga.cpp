/*
 * fpag.cpp
 *
 *  Created on: 2013-9-25
 *      Author: Administrator
 */
#include "evmomapl138.h"
//#include "evmomapl138_timer.h"
#include "gpio.h"
#include "fpga.h"
#include "evmomapl138_psc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datatype.h"
#include "clock.h"
#include "i2c.h"
#include "at24c02.h"
#include "scada.h"
#include "adsamp.h"
#include "fa_fft_cal.h"
#include "utils.h"
#define TERDEFAULT 0xffffffff

#pragma DATA_SECTION("DATAINDDR2");
FPGACFG_T FpgaCfg;

#pragma DATA_SECTION("DATAINDDR2");
BREAKOUTCHECK_t BreakOutCheck[MAX_LINE_NUM];
#pragma DATA_SECTION("DATAINDDR2");
TER_STATU TerFaultStatu;
#pragma DATA_SECTION("DATAINDDR2");
TER_STATU TerFaultCheck;
#pragma DATA_SECTION("DATAINDDR2");
const uint32 TerStatuMask[SLAVE_BOARD_NUM]= {
		YC_MASK1,YC_MASK2,YC_MASK3,YC_MASK4,YC_MASK5,
		IO_MASK1,IO_MASK2,IO_MASK3,IO_MASK4,
		RESMASK1
};
#pragma DATA_SECTION("DATAINDDR2");
TER_STATU UserParaMask;//参数配置需要的板支持的位置1；

uint32_t FpgaInit(void)
{

#if 0
//管脚配置在Arm侧已经配置过了
   uint32_t rtn = 0;

	// make sure the ui board is connected before proceeding.
	// if (!UTIL_isUIBoardAttached())
	//   return (ERR_NO_UI_BOARD);
	SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
	SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
	// enable the psc and config pinmux for FPGA
	//EVMOMAPL138_lpscTransition(PSC0, 0, LPSC_EMIFA, PSC_ENABLE);
	//EVMOMAPL138_lpscTransition(PSC0, 1, LPSC_EMIFA, PSC_ENABLE);
	EVMOMAPL138_pinmuxConfig(5 ,0xFF000000,0x11000000); // EMA_BA[1],EMA_BA[0]
	EVMOMAPL138_pinmuxConfig(7 ,0xF0FFFFFF,0x10118811); //cs2// EMA_WAIT[0],nEMA_RNW , nEMA_OE, nEMA_WE, nEMA_CS[5], nEMA_CS[4], nEMA_CS[3],nEMA_CS[2]
	EVMOMAPL138_pinmuxConfig(8 ,0xFFFFFFFF,0x11111111); // EMA_D[15:8]
	EVMOMAPL138_pinmuxConfig(9 ,0xFFFFFFFF,0x11111111); // EMA_D[7:0]
	EVMOMAPL138_pinmuxConfig(10,0xFFFFFFFF,0x11111111); // EMA_A[23..16]
	EVMOMAPL138_pinmuxConfig(11,0xFFFFFFFF,0x11111111); // EMA_A[15..8]
	EVMOMAPL138_pinmuxConfig(12,0xFFFFFFFF,0x11111111); // EMA_A[7..0]

	/*
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_0, PINMUX_EMIFA_NOR_MASK_0, PINMUX_EMIFA_NOR_VAL_0);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_1, PINMUX_EMIFA_NOR_MASK_1, PINMUX_EMIFA_NOR_VAL_1);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_2, PINMUX_EMIFA_NOR_MASK_2, PINMUX_EMIFA_NOR_VAL_2);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_3, PINMUX_EMIFA_NOR_MASK_3, PINMUX_EMIFA_NOR_VAL_3);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_4, PINMUX_EMIFA_NOR_MASK_4, PINMUX_EMIFA_NOR_VAL_4);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_5, PINMUX_EMIFA_NOR_MASK_5, PINMUX_EMIFA_NOR_VAL_5);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_6, PINMUX_EMIFA_NOR_MASK_6, PINMUX_EMIFA_NOR_VAL_6);
	EVMOMAPL138_pinmuxConfig(PINMUX_EMIFA_NOR_REG_7, PINMUX_EMIFA_NOR_MASK_7, PINMUX_EMIFA_NOR_VAL_7);
	*/
	// config FPGA fcr reg.
	EMIFA->CE2CFG =  0x1c5662BD;//0x1c57fffd;// 0x0C37FFFD;//0x3FFFFFFD;
	//  CLRBIT(EMIFA->NANDFCR, ECCSEL_MASK);
	//  SETBIT(EMIFA->NANDFCR, ECCSEL_CS2);
	//  SETBIT(EMIFA->NANDFCR, CS2NAND);

	//  send reset cmd to nand.
	//  writeLatchEnable(NAND_OFFSET_CLE, NAND_CMD_RESET);
	//  rtn = waitFlashReady(TIMEOUT_NAND);
	SYSCONFIG->KICKR[0] = KICK0R_LOCK;
	SYSCONFIG->KICKR[1] = KICK1R_LOCK;
	if (rtn != ERR_NO_ERROR)
	{
	#ifdef DEBUG
	printf("nand wait ready error:\t%d\r\n", rtn);
	#endif
	return (ERR_INIT_FAIL);
	}

	// read/verify device id.
	//  rtn = verifyDeviceId(TIMEOUT_NAND);
	if (rtn != ERR_NO_ERROR)
	{
	#ifdef DEBUG
	printf("nand device id error:\t%d\r\n", rtn);
	#endif
	return (ERR_INIT_FAIL);
	}
#endif
	return (ERR_NO_ERROR);
}
#if 0
float GetRang(uint16 port, float relative )
{
	if (YcPortRatio[port].InitFlag ==RATIOINIT){
		return	relative/PORTRATIO;
	}else{
		if(relative==NOMINAL_U){
			YcPortRatio[port].PortRatio=U1VRATIO*PORTRATIO;
		}else if(relative==NOMINAL_I) {
			YcPortRatio[port].PortRatio=I1ARATIO*PORTRATIO;
		}
	}
	return	relative/PORTRATIO;
}
#endif 
float GetPortRatio(uint16 port){
	if(port==NO_YC)
		return 0;
	else{
		int board;
		int off;
		board=port/PER_BOARD_YC_PORT_NUM;
		off=port % PER_BOARD_YC_PORT_NUM;
		return YcPortRatio[board].PortRatio[off];
	}
}


/**
*************************************************************************************
*  @brief		make YcRatio array by YcPortRatio array   
*  @param[in]
*  @param[out]
*  @return		void 
*  @note		YcRatio is orginized by Line ,YcPortRatio is orginized by Board . 
*				it is useful when calculte yc 
**************************************************************************************
*/
void XiShuChange(void){
	int16 i;
	for ( i = 0 ; i < MAX_LINE_NUM; i++){
		YcRatio[i].PortRatio[UA1]=  GetPortRatio(scada.yc_config.config_data[UA(i)].port);
		YcRatio[i].PortRatio[UB1]=  GetPortRatio(scada.yc_config.config_data[UB(i)].port);
		YcRatio[i].PortRatio[UC1]=  GetPortRatio(scada.yc_config.config_data[UC(i)].port);
		YcRatio[i].PortRatio[UAB1]= GetPortRatio(scada.yc_config.config_data[UAB(i)].port);
		YcRatio[i].PortRatio[UCB1]= GetPortRatio(scada.yc_config.config_data[UCB(i)].port);
		YcRatio[i].PortRatio[U01]=  GetPortRatio(scada.yc_config.config_data[U0(i)].port);
		YcRatio[i].PortRatio[IA11]= GetPortRatio(scada.yc_config.config_data[IA1(i)].port);
		YcRatio[i].PortRatio[IB11]= GetPortRatio(scada.yc_config.config_data[IB1(i)].port);
		YcRatio[i].PortRatio[IC11]= GetPortRatio(scada.yc_config.config_data[IC1(i)].port);
		YcRatio[i].PortRatio[I01]=	GetPortRatio(scada.yc_config.config_data[I0(i)].port);
		YcRatio[i].PortRatio[IA12]= GetPortRatio(scada.yc_config.config_data[IA2(i)].port);
		YcRatio[i].PortRatio[IB12]= GetPortRatio(scada.yc_config.config_data[IB2(i)].port);
		YcRatio[i].PortRatio[IC12]= GetPortRatio(scada.yc_config.config_data[IC2(i)].port);
		YcRatio[i].PowerLineRatio[0]=  YcRatio[i].PortRatio[UA1]*YcRatio[i].PortRatio[IA11];
		YcRatio[i].PowerLineRatio[1]=  YcRatio[i].PortRatio[UB1]*YcRatio[i].PortRatio[IB11];
		YcRatio[i].PowerLineRatio[2]=  YcRatio[i].PortRatio[UC1]*YcRatio[i].PortRatio[IC11];
		YcRatio[i].PowerAngleRatio[0].angle=0;
		YcRatio[i].PowerAngleRatio[1].angle=0;
		YcRatio[i].PowerAngleRatio[2].angle=0;
		YcRatio[i].PowerAngleRatio[0].cos_a=AngleCos;
		YcRatio[i].PowerAngleRatio[1].cos_a=AngleCos;
		YcRatio[i].PowerAngleRatio[2].cos_a=AngleCos;
		YcRatio[i].PowerAngleRatio[0].sin_a=AngleSin;
		YcRatio[i].PowerAngleRatio[1].sin_a=AngleSin;
		YcRatio[i].PowerAngleRatio[2].sin_a=AngleSin;
	}
}
#if 0
void XiShuChange(void){
	int16 i;
	for ( i = 0 ; i < MAX_LINE_NUM; i++){
		YcRatio[i].PortRatio[UA1]=  GetRang(scada.yc_config.config_data[UA(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[UB1]=  GetRang(scada.yc_config.config_data[UB(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[UC1]=  GetRang(scada.yc_config.config_data[UC(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[UAB1]= GetRang(scada.yc_config.config_data[UAB(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[UCB1]= GetRang(scada.yc_config.config_data[UCB(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[U01]=  GetRang(scada.yc_config.config_data[U0(i)].port,NOMINAL_U);
		YcRatio[i].PortRatio[IA11]= GetRang(scada.yc_config.config_data[IA1(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[IB11]= GetRang(scada.yc_config.config_data[IB1(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[IC11]= GetRang(scada.yc_config.config_data[IC1(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[I01]= GetRang(scada.yc_config.config_data[I0(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[IA12]= GetRang(scada.yc_config.config_data[IA2(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[IB12]= GetRang(scada.yc_config.config_data[IB2(i)].port,NOMINAL_I);
		YcRatio[i].PortRatio[IC12]= GetRang(scada.yc_config.config_data[IC2(i)].port,NOMINAL_I);
		YcRatio[i].PowerLineRatio[0]=  YcRatio[i].PortRatio[UA1]*YcRatio[i].PortRatio[IA11];
		YcRatio[i].PowerLineRatio[1]=  YcRatio[i].PortRatio[UB1]*YcRatio[i].PortRatio[IB11];
		YcRatio[i].PowerLineRatio[2]=  YcRatio[i].PortRatio[UC1]*YcRatio[i].PortRatio[IC11];
		YcRatio[i].PowerAngleRatio[0].angle=0;//0.9;
		YcRatio[i].PowerAngleRatio[1].angle=0;//0.9;
		YcRatio[i].PowerAngleRatio[2].angle=0;//0.9;
		YcRatio[i].PowerAngleRatio[0].cos_a=0.99996192306417128873735516482698;//0.99987663248166059863890712773125;//0.99994516936551213198209710144628;
		YcRatio[i].PowerAngleRatio[1].cos_a=0.99996192306417128873735516482698;//0.99987663248166059863890712773125;//0.99994516936551213198209710144628;
		YcRatio[i].PowerAngleRatio[2].cos_a=0.99996192306417128873735516482698;//0.99987663248166059863890712773125;//0.99994516936551213198209710144628;
		YcRatio[i].PowerAngleRatio[0].sin_a=0.00087266451523514954330458929907378;//0.015707317311820675753295353309907;//0.010471784116245793463225306967096;
		YcRatio[i].PowerAngleRatio[1].sin_a=0.00087266451523514954330458929907378;//0.015707317311820675753295353309907;//0.010471784116245793463225306967096;
		YcRatio[i].PowerAngleRatio[2].sin_a=0.00087266451523514954330458929907378;//0.015707317311820675753295353309907;//0.010471784116245793463225306967096;
	}
}
#endif 


/**
*************************************************************************************
*  @brief			init one LINE yc ratio ,make default ratio .
*  @param[in]		line	:line no,from 0
*  @param[out]
*  @return			void 
*  @note			one line yc ratio of ua,ub,uc,ia,ib,ic ..power,angle.  
*					because the u,i channel is distributed in sperate board,so angle ratio is 
*					diffcult to save (seperate save or concentrate save is all not suitable)
*					so i don't save it and make relative ratio as fixed data.
**************************************************************************************
*/
void YcRatioInit(uint8 line)
{
	//////////标定系数初始化
	uint16 i;
	YcRatio[line].PortRatio[UA1] =  U300RATIO;
	YcRatio[line].PortRatio[UB1] =  U300RATIO;
	YcRatio[line].PortRatio[UC1] =  U300RATIO;
	YcRatio[line].PortRatio[UAB1] = U300RATIO;
	YcRatio[line].PortRatio[UCB1] = U300RATIO;
	YcRatio[line].PortRatio[U01] =  U300RATIO;
	YcRatio[line].PortRatio[IA11] = I10RATIO;
	YcRatio[line].PortRatio[IB11] = I10RATIO;
	YcRatio[line].PortRatio[IC11] = I10RATIO;
	YcRatio[line].PortRatio[I01] = I10RATIO;
	YcRatio[line].PortRatio[IA12] = I100RATIO;
	YcRatio[line].PortRatio[IB12] = I100RATIO;
	YcRatio[line].PortRatio[IC12] = I100RATIO;
	for( i = 0; i < 3 ; i++){
		YcRatio[line].PowerLineRatio[i] = U300RATIO * I10RATIO ;//1.0;
		YcRatio[line].PowerAngleRatio[i].angle = 0;
		YcRatio[line].PowerAngleRatio[i].cos_a = 1;
		YcRatio[line].PowerAngleRatio[i].sin_a = 0;
	}
}

/**
*************************************************************************************
*  @brief		init all channel ratio in a board,ratio is default value . 
*  @param[in]	board	:yc board no 
*  @param[out]
*  @return		void 
*  @note		relative data struct:YcPortRatio[board][off]
*				only support U or I channel 
**************************************************************************************
*/
void YcPortRatioInit(int board){
	int i;
	int j;
	int channel;
	int yc_id;
	int type;

	for(j=0;j<PER_BOARD_YC_PORT_NUM;j++){
		channel=board*PER_BOARD_YC_PORT_NUM+j;
		yc_id=scada.find_yc_by_channel(channel);
		for(i=0;i<scada.yc_config.configed_yc_num;i++){
			if(yc_id==scada.yc_config.configed_yc[i]){
				type=scada.yc_config.config_data[yc_id].type;
				if(type==TYPE_U){
					YcPortRatio[board].PortRatio[j]=U300RATIO;
				}else if(type==TYPE_I){
					YcPortRatio[board].PortRatio[j]=I1ARATIO;
				}
				break;
			}
		}
	}
}
/**
*************************************************************************************
*  @brief			read ratio from all board   
*  @param[in]
*  @param[out]
*  @return			void 
*  @note			if fail (crc or read fail) will init ratio by default  
**************************************************************************************
*/
void YcRatioRead(void)
{   uint16 i;
	uint16 ret;
	uint16 crc;
	memset(&YcPortRatio ,0 ,sizeof(YcPortRatio));
	//read ac ratio
	for(i = 0;i<YC_BOARD_NUM;i++){
		if(FpgaCfg.YcCard[i].Statu !=IN_STATU){
			continue;
		}
		if(RW_FlashData(READ,i,ACRATIOADD,(uint8 *)&YcPortRatio[i] ,sizeof(YcRatio_S))==1){
			if(YcPortRatio[i].InitFlag!=0x55){//first run,the board has no ratio 
				YcPortRatioInit(i);
			}else{
				crc=calcrc((uint8 *)&YcPortRatio[i],sizeof(YcRatio_S)-2);
				if(crc!=YcPortRatio[i].crc)//crc fail
					YcPortRatioInit(i);
			}
		}else{
			YcPortRatioInit(i);
		}
	}
	XiShuChange();
	//read dc ratio,dc ratio is in board 1
	if(FpgaCfg.YcCard[0].Statu !=IN_STATU)
		return ;
	ret=RW_FlashData(READ,0,DCRATIOADD,(uint8 *)&(DcRation[0][0]) ,sizeof(DcPortRation));
	if(ret==1){
	}else{//if fail load default
		DcRation[0][0].K =0.0286533;
		DcRation[0][0].B =-0.9;
		DcRation[0][1].K =0.0286533;
		DcRation[0][1].B =-0.9;
		DcRation[1][0].K =0.0286533;
		DcRation[1][0].B =-0.9;
		DcRation[1][1].K =0.0286533;
		DcRation[1][1].B =-0.9;
	}
}
void YcRatioBoardRead(uint16 board_num)
{
	int crc;
	int ret;
	ret=RW_FlashData(READ,board_num,ACRATIOADD,(uint8 *)&(YcPortRatio[board_num]) ,sizeof(YcRatio_S));
	if(ret!=1){
		YcPortRatioInit(board_num);
		return;
	}
	if(YcPortRatio[board_num].InitFlag!=0x55){//first run,the board has no ratio
		YcPortRatioInit(board_num);
		return ;
	}
	crc=calcrc((uint8 *)&YcPortRatio[board_num],sizeof(YcRatio_S)-2);
	if(crc!=YcPortRatio[board_num].crc)//crc fail
		YcPortRatioInit(board_num);
}

void CheckBoardGpio(void)
{
	uint8 i,j;
	i = 0;
	j = 0;
	if(YC_SLOT1_STATU==IN_STATU){
		FpgaCfg.YcCard[i].SlotNum = 0;
		FpgaCfg.YcCard[i].Statu = IN_STATU;
		FpgaCfg.YcCard[i].IdNum = 0;
		i++;
	}else{

	}
	if(YC_SLOT2_STATU==IN_STATU){
		FpgaCfg.YcCard[i].SlotNum = 1;
		FpgaCfg.YcCard[i].Statu = IN_STATU;
		FpgaCfg.YcCard[i].IdNum = 1;
		i++;
	}else{

	}
	if(YC_SLOT3_STATU==IN_STATU){
		FpgaCfg.YcCard[i].SlotNum = 2;
		FpgaCfg.YcCard[i].Statu = IN_STATU;
		FpgaCfg.YcCard[i].IdNum = 2;
		i++;
	}else{

	}
	if(YC_SLOT4_STATU==IN_STATU){
		FpgaCfg.YcCard[i].SlotNum = 3;
		FpgaCfg.YcCard[i].Statu = IN_STATU;
		FpgaCfg.YcCard[i].IdNum = 3;
		i++;
	}else{

	}
	if(YC_SLOT5_STATU==IN_STATU){
		FpgaCfg.YcCard[i].SlotNum = 4;
		FpgaCfg.YcCard[i].Statu = IN_STATU;
		FpgaCfg.YcCard[i].IdNum = 4;
		i++;
	}else{

	}
	FpgaCfg.YcCardSize = i;


	if(INOUT_SLOT1_STATU==IN_STATU){
		FpgaCfg.InOutCard[j].SlotNum = 5;
		FpgaCfg.InOutCard[j].Statu = IN_STATU;
		FpgaCfg.InOutCard[j].IdNum = 5;
		j++;
	}else{

	}

	if(INOUT_SLOT2_STATU==IN_STATU){
		FpgaCfg.InOutCard[j].SlotNum = 6;
		FpgaCfg.InOutCard[j].Statu = IN_STATU;
		FpgaCfg.InOutCard[j].IdNum = 6;
		j++;
	}else{

	}

	if(INOUT_SLOT3_STATU==IN_STATU){
		FpgaCfg.InOutCard[j].SlotNum = 7;
		FpgaCfg.InOutCard[j].Statu = IN_STATU;
		FpgaCfg.InOutCard[j].IdNum = 7;
		j++;
	}else{

	}
	if(INOUT_SLOT4_STATU==IN_STATU){
		FpgaCfg.InOutCard[j].SlotNum = 8;
		FpgaCfg.InOutCard[j].Statu = IN_STATU;
		FpgaCfg.InOutCard[j].IdNum = 8;
		j++;
	}else{

	}

	if(EXT_SLOT1_STATU==IN_STATU){
		FpgaCfg.InOutCard[j].SlotNum = 9;
		FpgaCfg.InOutCard[j].Statu = IN_STATU;
		FpgaCfg.InOutCard[j].IdNum = 9;
		j++;
	}else{

	}
	FpgaCfg.InOutCardSize = j;

}
void FpgaCfgInit(void)
{
	uint8 i;
	i = FpgaCfg.InitFlag;
	memset(&FpgaCfg ,0 ,sizeof(FpgaCfg));
	FpgaCfg.InitFlag = i;
	CheckBoardGpio();
	AdReset();
}

void SetParaMask(uint16 SlotNum,uint16 SetType)
{
	if(SlotNum>=SLAVE_BOARD_NUM){
		return ;
	}
	if(SetType == SET_BIT){
		SETBIT(UserParaMask.all,TerStatuMask[SlotNum]);
	}else if(SetType == CLR_BIT){
		CLRBIT(UserParaMask.all,TerStatuMask[SlotNum]);
	}
}
#define BREAKERGOOD 1
//故障状态使用位来表示，初始状态全是故障，检测到正常的位清零；
void FpgaCheck(void)
{
	TER_STATU TerDefault;
	TerDefault.all = TERDEFAULT;
	TerDefault.bit.yc1 =YC_SLOT1_STATU;
	TerDefault.bit.yc2 =YC_SLOT2_STATU;
	TerDefault.bit.yc3 =YC_SLOT3_STATU;
	TerDefault.bit.yc4 =YC_SLOT4_STATU;
	TerDefault.bit.yc5= YC_SLOT5_STATU;
	TerDefault.bit.inout1 =INOUT_SLOT1_STATU;
	TerDefault.bit.inout2 =INOUT_SLOT2_STATU;
	TerDefault.bit.inout3 =INOUT_SLOT3_STATU ;
	TerDefault.bit.inout4 =INOUT_SLOT4_STATU ;
	TerDefault.bit.ext =EXT_SLOT1_STATU ;
	TerFaultCheck.all = TerDefault.all&UserParaMask.all;//与配置项相与；配置为1，没有配置为0；所以如果某项没有配置那算是正常的；

}
