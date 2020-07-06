/*
 * i2c.cpp
 *
 *  Created on: 2015-7-28
 *      Author: Administrator
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
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "i2c.h"
#include "utils.h"
typedef struct _i2c_bus{
	int scl_pin;
	int sda_pin;
}i2c_bus;
static i2c_bus i2c_buss[2]={
	[0]={
		.scl_pin=GPIO_TO_PIN(4,5),
		.sda_pin=GPIO_TO_PIN(4,6),
	},
	[1]={
		.scl_pin=GPIO_TO_PIN(1,5),
		.sda_pin=GPIO_TO_PIN(1,4),
	}
};

static unsigned int I2CDelay =3;
void nop()
{
	DelayUs(I2CDelay);
}
void IC_start(int bus_id)
{
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	g_gpio.set(i2c_buss[bus_id].scl_pin,1);
	g_gpio.set(i2c_buss[bus_id].sda_pin,1);;
	nop();
	nop();
	g_gpio.set(i2c_buss[bus_id].sda_pin,0);;
	nop();
	nop();
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);;
}


void IC_stop(int bus_id)
{
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);; /** Send Stop **/
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	g_gpio.set(i2c_buss[bus_id].sda_pin,0);;
	nop();
	nop();
	g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
	nop();
	nop();
	g_gpio.set(i2c_buss[bus_id].sda_pin,1);;
}


/////***************************************************
////** 函数原型: bit IC_writebyte(uchar wdata); **
////** 功 能: 向IC 总线发送8 位数据,并请求一个应答信 **
////** 号ACK.如果收到ACK 应答则返回1(TRUE), **
////** 否则返回0(FALSE). **
////***************************************************/
uint8 IC_WriteByte( uint8 indata,int bus_id)
{
	uint8 i;
	uint8 wdata = indata;
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	for(i=0;i<8;i++)
	{
		g_gpio.set(i2c_buss[bus_id].scl_pin,0);
		nop();
		//nop();
		if(wdata&0x80) 
			g_gpio.set(i2c_buss[bus_id].sda_pin,1);
		else 
			g_gpio.set(i2c_buss[bus_id].sda_pin,0);
		nop();
		//nop();
		g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
		nop();
		nop();
		wdata<<=1;
	}
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);
	nop();
	nop();
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_INPUT);
	g_gpio.set(i2c_buss[bus_id].scl_pin,1);
	i=0;
	while(g_gpio.get(i2c_buss[bus_id].sda_pin))
	{
		if(++i>12){
			g_gpio.set(i2c_buss[bus_id].scl_pin,0);
			return(0);
		}
	}
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);;
	return(1);
}

uint8 IC_ReadByte(int bus_id)
{
	uint8 i;
	uint8 IC_data=0;
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);;
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	g_gpio.set(i2c_buss[bus_id].sda_pin,1);;
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_INPUT);
	for(i=0;i<8;i++)
	{
		g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
		nop();
		nop();
		IC_data<<=1;
		IC_data|=g_gpio.get(i2c_buss[bus_id].sda_pin);
		nop();
		g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
		nop();
		g_gpio.set(i2c_buss[bus_id].scl_pin,0);;
		nop();
	}
	nop();
	nop();
	// g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
	nop();
	nop();
	nop();
	g_gpio.set(i2c_buss[bus_id].scl_pin,0);;
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	g_gpio.set(i2c_buss[bus_id].sda_pin,0);;
	return(IC_data);
}
/////////////////////////////////////////////////////////////////////////

/***********************************************************************
**  Time:        Blute 2015.3.2
**  Function:读取rx8025n内部寄存器
**  Input:
**  Output:
**  Attention
***********************************************************************/
uint16 ReadI2cDevBypes(I2CSLAVEREG_U *SlaveMsg,uint8 RegAdd,int8 ReadDatatLength,uint8 *Data,int bus_id)
{
	uint8 i;
	SlaveMsg->bit.RW = WRITE;
//ST启动
	IC_start(bus_id);
//ST写器件地址 --> 写命令
	if(IC_WriteByte(SlaveMsg->all,bus_id)==0){
		IC_stop(bus_id); 
		return(0);
	}
//ST写内存地址
	if(IC_WriteByte(RegAdd,bus_id)==0){
		IC_stop(bus_id); 
		return(0);
	}
//ST 启动
	IC_start(bus_id);
	SlaveMsg->bit.RW = READ;
//ST 写器件地址 --> 读命令
	if(IC_WriteByte(SlaveMsg->all,bus_id)==0){
		IC_stop(bus_id);
		return(0);
	}

	for(i=0;i<ReadDatatLength-1;i++){
		Data[i]=IC_ReadByte(bus_id);
		g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
		g_gpio.set(i2c_buss[bus_id].sda_pin,0);; /** Send ACK **/
		g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
	}
	Data[ReadDatatLength-1]=IC_ReadByte(bus_id);
	g_gpio.set_dir(i2c_buss[bus_id].sda_pin,I2C_OUTPUT);
	g_gpio.set(i2c_buss[bus_id].sda_pin,1);; /** Send Read End ACK **/
	nop();
	g_gpio.set(i2c_buss[bus_id].scl_pin,1);;
	IC_stop(bus_id);
	return(1);

}

/***********************************************************************
**  Time:     Blute 2015.3.2
**  Function:写rx8025n内部寄存器
**  Input:        ControlByte, Add, *wrptr.
**  Output:        None
**	return		:on success 1;fail 0
**  Attention:先发命令地址，再发地址，最后发数据
***********************************************************************/

uint16 WriteI2cDevBypes(I2CSLAVEREG_U *SlaveMsg,uint8 RegAdd,int8 WriteDataLength,uint8 *wrptr,int bus_id)
{
		uint8 i;
		SlaveMsg->bit.RW = WRITE;
		IC_start(bus_id);
		if(IC_WriteByte(SlaveMsg->all,bus_id)==0){
			IC_stop(bus_id);
			return(0);
		}
		if(IC_WriteByte(RegAdd,bus_id)==0){
			IC_stop(bus_id);
			return(0);
		}
		for(i=0;i<WriteDataLength;i++){
			if(IC_WriteByte(wrptr[i],bus_id)==0){
				IC_stop(bus_id); 
				return(0);
			}
		}
		IC_stop(bus_id);
		nop();
		nop();
		g_gpio.set(i2c_buss[bus_id].sda_pin,0);
		g_gpio.set(i2c_buss[bus_id].scl_pin,0);
		return(1);
}



