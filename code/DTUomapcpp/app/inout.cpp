/*
 * RelayOut.cpp
 *
 *  Created on: 2015-7-27
 *      Author: Administrator
 */

#include "evmomapl138.h"
#include "gpio.h"
#include "evmomapl138_psc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datatype.h"
#include "clock.h"
#include "inout.h"
#include "gpio.h"
/*
 * 高电平打开继电器
 * 32个回线继电器+四个公共遥控继电器+电源管理遥控继电器
 *  起始地址1:0x60000200
 *  bit0:断路器1合；bit1:断路器1分；
 *  bit2:断路器2合；bit3:断路器2分；
 *  bit4:断路器3合；bit5:断路器3分；
 *  bit6:断路器4合；bit7:断路器4分；
 *  起始地址2:0x60000201；
 *  bit0:断路器5合；bit1:断路器5分；
 *  bit2:断路器6合；bit3:断路器6分；
 *  bit4:断路器7合；bit5:断路器7分；
 *  bit6:断路器8合；bit7:断路器8分；
 *  起始地址3:0x60000202；
 *  bit0:断路器9合；bit1:断路器9分；
 *  bit2:断路器10合；bit3:断路器10分；
 *  bit4:断路器11合；bit5:断路器11分；
 *  bit6:断路器12合；bit7:断路器12分；
 *  地址4:0x60000203；
 *  bit0:断路器13合；bit1:断路器13分；
 *  bit2:断路器14合；bit3:断路器14分；
 *  bit4:断路器15合；bit5:断路器15分；
 *  bit6:断路器16合；bit7:断路器16分；
 *  起始地址5:0x60000204；
 *  bit0:公共遥控1合；bit1:公共遥控1分；
 *  bit2:公共遥控2合；bit3:公共遥控2分；
 *  bit4:电池活化；bit5:活化退出；
 *  bit6:电池退出；
 */
#pragma DATA_SECTION("YKDATA");
Uint16 YKREG[5];

Uint16 YKMEMREG[5];
/*
const Uint16 RelayOutMask[MAX_OUT_NUM]={
	0x8000,
	0x4000,
	0x2000,
	0x1000,
	0x0800,
	0x0400,
	0x0200,
	0x0100,
	0x0080,
	0x0040,
	0x0020,
	0x0010,
	0x0008,
	0x0004,
	0x0002,
	0x0001,
};
const Uint16 RelayStatuMask[MAX_OUT_NUM]={
	0x0001,
	0x0002,
	0x0004,
	0x0008,
	0x0010,
	0x0020,
	0x0040,
	0x0080,
	0x0100,
	0x0200,
	0x0400,
	0x0800,
	0x1000,
	0x2000,
	0x4000,
	0x8000,
};*/
#pragma DATA_SECTION("DATAINDDR2")
const Uint16 RelayOutMask[8]={
		0x0080,
		0x0040,
		0x0020,
		0x0010,
		0x0008,
		0x0004,
		0x0002,
		0x0001,
};
/*const Uint64 RelayOutMask[64]={
		0x0000000000000001,
		0x0000000000000002,
		0x0000000000000004,
		0x0000000000000008,
		0x0000000000000010,
		0x0000000000000020,
		0x0000000000000040,
		0x0000000000000080,
		0x0000000000000100,
		0x0000000000000200,
		0x0000000000000400,
		0x0000000000000800,
		0x0000000000001000,
		0x0000000000002000,
		0x0000000000004000,
		0x0000000000008000,
		0x0000000000010000,
		0x0000000000020000,
		0x0000000000040000,
		0x0000000000080000,
		0x0000000000100000,
		0x0000000000200000,
		0x0000000000400000,
		0x0000000000800000,
		0x0000000001000000,
		0x0000000002000000,
		0x0000000004000000,
		0x0000000008000000,
		0x0000000010000000,
		0x0000000020000000,
		0x0000000040000000,
		0x0000000080000000,
		0x0000000100000000,
		0x0000000200000000,
		0x0000000400000000,
		0x0000000800000000,
		0x0000001000000000,
		0x0000002000000000,
		0x0000004000000000,
		0x0000008000000000,
		0x0000010000000000,
		0x0000020000000000,
		0x0000040000000000,
		0x0000080000000000,
		0x0000100000000000,
		0x0000200000000000,
		0x0000400000000000,
		0x0000800000000000,
		0x0001000000000000,
		0x0002000000000000,
		0x0004000000000000,
		0x0008000000000000,
		0x0010000000000000,
		0x0020000000000000,
		0x0040000000000000,
		0x0080000000000000,
		0x0100000000000000,
		0x0200000000000000,
		0x0400000000000000,
		0x0800000000000000,
		0x1000000000000000,
		0x2000000000000000,
		0x4000000000000000,
		0x8000000000000000,
};*/
RELAYSTREGU RelaySt={
		0,
};
#pragma DATA_SECTION("DATAINIRAM");
RELREG RelayStatu;


void YkBitOut(Uint16 RelayNum,Uint16 BitType)
{
	uint16 i,j;
	i = RelayNum/8;
	j = RelayNum%8;
	if(BitType==SET){
		SETBIT(RelayStatu.BitStatu[i],RelayOutMask[j]);
		SETBIT(YKMEMREG[i],RelayStatu.BitStatu[i]);
		CLRBIT(RelayStatu.BitStatu[i],RelayOutMask[j]);
		YKREG[i] = YKMEMREG[i];

	}else{
		SETBIT(RelayStatu.BitStatu[i],RelayOutMask[j]);
		CLRBIT(YKMEMREG[i],RelayStatu.BitStatu[i]);
		YKREG[i] = YKMEMREG[i];
		CLRBIT(RelayStatu.BitStatu[i],RelayOutMask[j]);
	}
}
int16 RelayOut(Uint16 RelayNum,Uint16 OutType)
 {
	if (RelayNum>MAX_OUT_NUM){
		return -1;
	}
	if (RelayStatu.RelayReg[RelayNum].LevelType==PULSE){//当继电器类型为脉冲型的  OutType是无效的
		if(RelayStatu.RelayReg[RelayNum].StartFlag == CLR){
			OPENSTARTRELAY;
			RelayStatu.RelayReg[RelayNum].StartFlag = SET;
			RelayStatu.RelayReg[RelayNum].TimeCnt = clk0.GetSysMs() + RelayStatu.RelayReg[RelayNum].Td;
			YkBitOut(RelayNum,SET);
		}
	}else if(RelayStatu.RelayReg[RelayNum].LevelType==LEV) {//当继电器类型为电平型的  出口电平是依据OutType
		if(OutType==SET){
			OPENSTARTRELAY;
			RelayStatu.RelayReg[RelayNum].StartFlag = SET;
			YkBitOut(RelayNum,SET);
		}else{
			OPENSTARTRELAY;
			RelayStatu.RelayReg[RelayNum].StartFlag = CLR;
			RelayStatu.RelayReg[RelayNum].TimeCnt =0;
			YkBitOut(RelayNum,CLR);
		}
	}
	return 1;
 }

void RelayReturn(Uint64 SysMs)
{
	Uint16 i;
	Uint16 j = 0;
	for ( i = 0; i<MAX_OUT_NUM; i++){
		if (RelayStatu.RelayReg[i].LevelType==PULSE){
			if(RelayStatu.RelayReg[i].StartFlag == SET){
				if(SysMs>=RelayStatu.RelayReg[i].TimeCnt ){
					//CLRBIT(YKREG,RelayOutMask[i]);
					YkBitOut(i,CLR);
					RelayStatu.RelayReg[i].StartFlag =CLR;
					CLOSESTARTRELAY;
				}
			}
		}
		if(RelayStatu.RelayReg[i].StartFlag == CLR){
						j++;
		}
	}
	if(j==MAX_OUT_NUM){//当所有的出口继电器关闭时，启动继电器关闭
		CLOSESTARTRELAY;
	}
}
