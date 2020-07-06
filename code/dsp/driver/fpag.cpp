/*
 * fpag.cpp
 *
 *  Created on: 2013-9-25
 *      Author: Administrator
 */
#include "omapl138.h"
//#include "evmomapl138_timer.h"
#include "gpio.h"
#include "fpga.h"
#include "psc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <mem.h>

#pragma DATA_SECTION(".FpgaHostData");
struct HOST FpgaHost;

#pragma DATA_SECTION(".FpgaSlaveData");
struct SLAVE FpgaSlave[SLAVE_NUM];

FPGACFG_T FpgaCfg;

uint16 GetId(void)
{
return FpgaHost.ID.all;

}
uint16 GetSlaveID(uint8 num)
{
	 return FpgaSlave[num].ID.all;
}
#define WRITEHEAD       0x8686
#define READHEAD        0x8786
#define READACKFIR  0x8687
#define READACKSEC  0x0001
#define READACKOK   0x0002
#define READACKFAIL 0x0001
#define WRITEFIR    0x0001
#define WRITESEC    0x0003
#define FIFONUM 20
#define RECEHEAD    0x8888
//³É¹¦·µ»Ø1 Ê§°Ü·µ»Ø0
int16 SendReady(uint8 FpgaNum)
{
	int16 timeout = 1000;
	while(timeout>0){
		timeout--;
		if(FpgaSlave[FpgaNum].FIFO_ISR.bit.sendcml==1){
			return 1;
		}
	}
	return 0;
}
int16 ReceiveTimerOut(uint8 FpgaNum)
{	uint16 num = 0;
	int16 timeout = 2000;
	while(timeout>0){
		timeout--;
		if(FpgaSlave[FpgaNum].IRQ_IIR.bit.dataover==1){
			num = FpgaSlave[FpgaNum].FIFO_ISR.bit.receivefifonum;
			return num;
		}
	}
	return 0;
}

int16 ReceiveFull(uint8 FpgaNum)
{
	int16 timeout = 1000;
	while(timeout>0){
		timeout--;
		if(FpgaSlave[FpgaNum].IRQ_IIR.bit.timeover==1){
			return 1;
		}
	}
	return 0;
}
uint32 TIMEOUT = 200000;
int16 ReceiveFlag(uint8 FpgaNum)
{
	uint32 num = 0;
		int32 timeout = TIMEOUT;
		while(timeout>0){
			timeout--;
			if(FpgaSlave[FpgaNum].IRQ_IIR.bit.dataover==1){
				num = FpgaSlave[FpgaNum].FIFO_ISR.bit.receivefifonum;//num = 20;
				return num;
			}else if(FpgaSlave[FpgaNum].IRQ_IIR.bit.timeover==1){
		     	num = FpgaSlave[FpgaNum].FIFO_ISR.bit.receivefifonum;
				return num;
			}else {
				num = FpgaSlave[FpgaNum].FIFO_ISR.bit.receivefifonum;
				if(num!=0){
					return num;
				}
			}
		}
		return 0;
}
//¶¼ÊÇÒÔ×ÖÎªµ¥Î»µÄ Ê¹ÓÃÊ±Çë×¢Òâ£»
uint16 FpgaWriteData(uint8 FpgaNum,uint16 *DataAdd ,uint16 DataLen)
{
	int16 i;
	int16 ret = 0;
	uint16 Sum = 0;
	uint16 rectmp = 0;
	uint16 ackstep = 0;
	if(DataAdd==NULL){
		return 0;
	}
	FpgaSlave[FpgaNum].IRQ_ENABLE.bit.dataover = 1;//Êý¾Ý½ÓÊÕ³¬Ê±ÖÐ¶Ï
	FpgaSlave[FpgaNum].IRQ_ENABLE.bit.timeover = 1;//Êý¾Ý½ÓÊÕÂúÖÐ¶Ï

	FpgaSlave[FpgaNum].DATA_TX = WRITEHEAD;
	if(SendReady(FpgaNum)==1){
		FpgaSlave[FpgaNum].DATA_TX  = DataLen;
	}else{
		return 0;
	}
	for (i = 0; i < DataLen; i++){
		if(SendReady(FpgaNum)==1){
			FpgaSlave[FpgaNum].DATA_TX  = DataAdd[i];
			Sum+=DataAdd[i];
		}else{
			return 0;
		}
	}
	if(SendReady(FpgaNum)==1){
		FpgaSlave[FpgaNum].DATA_TX  = Sum;
	}else{
		return 0;
	}

	while(ReceiveFlag(FpgaNum)!=0){
		rectmp = FpgaSlave[FpgaNum].DATA_RX;
		if(rectmp == READACKFIR){
			ackstep = 1;
		}
		if((rectmp==READACKSEC)&&(ackstep ==1)){
			ackstep = 2;
		}
		if((rectmp == READACKOK)&&(ackstep ==2)){
			ret = 1;
			ackstep = 3;
			//break;
		}
	}
	if(ret==0){
		ret = 0;
	}
	return ret ;
}
//³É¹¦·µ»Ø1 Ê§°Ü·µ»Ø0
//uint16 FPGATMP[100];
uint16 FpgaReadData(uint8 FpgaNum,uint16 *ExpDataAdd ,uint16 ExpDataLen)
{
	uint16 Sum = 0;
	uint16 TmpSum = 0xffff;
	uint16 recevdata = 0;
	uint16 DataLen = 0;
	uint16 DateLenTmp = 0;
	uint16 receivepos = 0;
	uint16 *DataAdd = NULL;
	uint8 tmpmemeryflag = 0;
	uint16 ackstep = 0;
	//memset(FPGATMP,0x4321,sizeof(FPGATMP));
	if(ExpDataAdd==NULL){
		return 0;
	}
	FpgaSlave[FpgaNum].IRQ_ENABLE.bit.dataover = 1;//Êý¾Ý½ÓÊÕ³¬Ê±ÖÐ¶Ï
	FpgaSlave[FpgaNum].IRQ_ENABLE.bit.timeover = 1;//Êý¾Ý½ÓÊÕÂúÖÐ¶Ï

	FpgaSlave[FpgaNum].DATA_TX = READHEAD;
	if(SendReady(FpgaNum)==1){
		FpgaSlave[FpgaNum].DATA_TX  = WRITEFIR;
	}else{
		return 0;
	}
	if(SendReady(FpgaNum)==1){
		FpgaSlave[FpgaNum].DATA_TX  = WRITESEC;
	}else{
		return 0;
	}
	if(SendReady(FpgaNum)==1){

	}
	while(ReceiveFlag(FpgaNum)!=0){

		recevdata  = FpgaSlave[FpgaNum].DATA_RX ;
		//FPGATMP[receivepos] = recevdata;
		receivepos++;
		if((recevdata==RECEHEAD)&&(ackstep == 0)){
			ackstep = 1;
		}else if (ackstep==1){
			DataLen = recevdata ;
			DateLenTmp =0;
			if((DataLen>300)||(DataLen<=0)){
				return 0;
			}
			DataAdd =  (uint16 *)malloc(DataLen*sizeof(uint16));
			if(DataAdd!=NULL){
				tmpmemeryflag = 1;
			}else {
				return 0;
			}
			ackstep=2;
		}else if((ackstep==2)&&(DateLenTmp<DataLen)){
			DataAdd[DateLenTmp] = recevdata;
			Sum+=DataAdd[DateLenTmp];
			DateLenTmp++;
		}else if ((ackstep==2)&&(DateLenTmp==DataLen)){
			TmpSum = recevdata ;
			DateLenTmp++;
			ackstep=3;
		}
	}

	if(Sum==TmpSum){
		if(tmpmemeryflag == 1){
			memcpy(ExpDataAdd,DataAdd,DateLenTmp*2);
			free(DataAdd);
			DataAdd = NULL;
			tmpmemeryflag = 0;
		}

	}else {
		if(tmpmemeryflag == 1){
			free(DataAdd);
			DataAdd = NULL;
			tmpmemeryflag = 0;
		}
		return 0;
	}
	return 1;
}


//³É¹¦·µ»Ø1 Ê§°Ü·µ»Ø0

int16 FpgaOut(uint8 FpgaNum,uint8 YkNum,uint8 OutType,uint16 Td)
{
   uint8 i;
	if (FpgaSlave[FpgaNum].CARD_MODE!=YXBOARD)	{
		return -1 ;
	}
	FpgaSlave[FpgaNum].YK_TIME = Td;
	//if (FpgaSlave[FpgaNum].YK_TIME != Td) {
	//	return -1;
	//}
//	FpgaSlave[FpgaNum].YK[YkNum].bit.qd = 1;
	if (OutType == CLOSEBREAKER){
		FpgaSlave[FpgaNum].YK[YkNum].all = 0x05;
		for(i = 0;i<5;i++);
		FpgaSlave[FpgaNum].YK[YkNum].all = 0;
	}else if (OutType == OPENBREAKER){
		FpgaSlave[FpgaNum].YK[YkNum].all = 0x06;
		for(i = 0;i<5;i++);
		FpgaSlave[FpgaNum].YK[YkNum].all = 0;
	}

	return 1;
}

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   initialize the nand flash for use.
//
// \param   none.
//
// \return  uint32
//    ERR_NO_ERROR - everything is ok...nand ready to use.
//    ERR_INIT_FAIL - something happened during initialization.
//-----------------------------------------------------------------------------
uint32 FpgaInit(void)
{
   uint32 rtn = 0;

   // make sure the ui board is connected before proceeding.
  // if (!UTIL_isUIBoardAttached())
   //   return (ERR_NO_UI_BOARD);
   SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
   SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
   // enable the psc and config pinmux for FPGA
   EVMOMAPL138_lpscTransition(PSC0, 0, LPSC_EMIFA, PSC_ENABLE);
  // EVMOMAPL138_pinmuxConfig(5 ,0xFF000000,0x21000000); // EMA_BA[1],EMA_BA[0]
  // EVMOMAPL138_pinmuxConfig(7 ,0xF0FFFFFF,0x00110010); //cs3// EMA_WAIT[0],nEMA_RNW , nEMA_OE, nEMA_WE, nEMA_CS[5], nEMA_CS[4], nEMA_CS[3],nEMA_CS[2]
   EVMOMAPL138_pinmuxConfig(7 ,0xF0FFFFFF,0x10118811); //cs2// EMA_WAIT[0],nEMA_RNW , nEMA_OE, nEMA_WE, nEMA_CS[5], nEMA_CS[4], nEMA_CS[3],nEMA_CS[2]
  // EVMOMAPL138_pinmuxConfig(8 ,0xFFFFFFFF,0x11111111); // EMA_D[15:8]
  // EVMOMAPL138_pinmuxConfig(9 ,0xFFFFFFFF,0x11111111); // EMA_D[7:0]
 //  EVMOMAPL138_pinmuxConfig(10,0xFFFFFFFF,0x11111111); // EMA_A[23..16]
 //  EVMOMAPL138_pinmuxConfig(11,0xFFFFFFFF,0x11111111); // EMA_A[15..8]
 //  EVMOMAPL138_pinmuxConfig(12,0xFFFFFFFF,0x11111111); // EMA_A[7..0]
   // config FPGA fcr reg.
   EMIFA->CE2CFG =0x1c57fffd;// 0x0C37FFFD;//0x3FFFFFFD;
   CLRBIT(EMIFA->NANDFCR, ECCSEL_MASK);
   SETBIT(EMIFA->NANDFCR, ECCSEL_CS2);
   SETBIT(EMIFA->NANDFCR, CS2NAND);

   // send reset cmd to nand.
 //  writeLatchEnable(NAND_OFFSET_CLE, NAND_CMD_RESET);
 //  rtn = waitFlashReady(TIMEOUT_NAND);
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

   return (ERR_NO_ERROR);
}

void FpgaCfgInit(void)
{
	uint8 i;
	FpgaCfg.YcCardSize = 0;
	FpgaCfg.YxCardSize = 0;
	memset(&FpgaCfg ,0 ,sizeof(FpgaCfg));
	for ( i = 0; i < SLAVEFPGANUM; i++){
		if(FpgaSlave[i].ID.all!=0){
			if(FpgaSlave[i].CARD_MODE==YXBOARD){
				FpgaCfg.YxCard[FpgaCfg.YxCardSize].num = i;
				FpgaCfg.YxCard[FpgaCfg.YxCardSize++].ID = FpgaSlave[i].ID.all;
			}else if(FpgaSlave[i].CARD_MODE==YCBOARD){
				FpgaCfg.YcCard[FpgaCfg.YcCardSize].num = i;
				FpgaCfg.YcCard[FpgaCfg.YcCardSize++].ID = FpgaSlave[i].ID.all;
			}
		}
	}

	FpgaSlave[3].YX_TIME = 200;
	//FpgaHost.M_REG_IRQ_ENABLE.bit.fpga4 = 1;
	//FpgaHost.M_REG_IRQ_ENABLE.bit.fpga3 = 1;
	//FpgaHost.M_REG_IRQ_ENABLE.bit.fpga5 = 1;
	FpgaHost.M_REG_IRQ_ENABLE.all = 0x1c0;

}
