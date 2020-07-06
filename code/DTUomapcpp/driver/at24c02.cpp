/*
 * at24c02.cpp
 *
 *  Created on: 2015-7-29
 *      Author: Administrator
 */
/*
 AT24C02是一个2K位串行CMOS E2PROM， 内部含有256个8位字节
 */
#include "gpio.h"
#include "fpga.h"
#include "evmomapl138_psc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "datatype.h"
#include "dspdatatype.h"
#include "clock.h"
#include "scada.h"
#include "evmomapl138.h"
#include "i2c.h"
#include "at24c02.h"
static const uint8 SlaveAdd[YC_BOARD_NUM] = {
		0xA0,0xA2,0xA4,0xA6,0xA8
};
/*
 * SlotNum:板槽号
 */
uint16 RW_FlashData(uint8 RW,uint8 SlotNum,uint8 StartAdd,uint8 *ExpDataAdd ,uint16 ExpDataLen)
{
	uint8 i ;
	uint8 page;
	uint8 addr = StartAdd;
	uint8 firlen;
	uint8 endlen;
	I2CSLAVEREG_U FlashAdd;
	FlashAdd.all = SlaveAdd[SlotNum];
	if(RW==READ){
		return  (ReadI2cDevBypes(&FlashAdd,StartAdd,ExpDataLen,ExpDataAdd,0));
	}else{

		firlen = 8-(StartAdd%8);
		WriteI2cDevBypes(&FlashAdd,addr,firlen,(uint8 *)&ExpDataAdd[0],0);

		page  =  (ExpDataLen-firlen)/8;
		endlen = (ExpDataLen-firlen)%8;
		addr+=firlen;


		for (i = 0; i<page ; i++){
			WriteI2cDevBypes(&FlashAdd,addr,8,(uint8 *)&ExpDataAdd[firlen+i*8],0);
			addr+=8;
		}
		//addr = page*8;
		return (WriteI2cDevBypes(&FlashAdd,addr,endlen,(uint8 *)&ExpDataAdd[firlen+page*8],0));
	}
}


