/*
 * gpio.cpp
 *
 *  Created on: 2013-9-22
 *      Author: Administrator
 */



//-----------------------------------------------------------------------------
// \file    evmomapl138_gpio.c
// \brief   implementation of the gpio driver for the OMAP-L138 EVM.
//
//-----------------------------------------------------------------------------

#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "gpio.h"

gpio_regs_u *GPIO_BANK01_BIT =(gpio_regs_u*)(GPIO_BANK01_BASE);
gpio_regs_u *GPIO_BANK23_BIT =(gpio_regs_u*)(GPIO_BANK23_BASE);
gpio_regs_u *GPIO_BANK45_BIT =(gpio_regs_u*)(GPIO_BANK45_BASE);
gpio_regs_u *GPIO_BANK67_BIT =(gpio_regs_u*)(GPIO_BANK67_BASE);
gpio_regs_u *GPIO_BANK8_BIT =(gpio_regs_u*)(GPIO_BANK8_BASE);
//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define GPIO_ODD_BANK_PIN_ADDER     (16)
GPIO	g_gpio;
uint8_t GPIO::get(int pin){
	int bank;
	int offset;
	uint8_t val;
	bank=pin/16;
	offset=pin%16;
	GPIO_getInput(bank,offset,&val);
	return val;

}
uint32_t GPIO::set(int pin,int value){
	int bank;
	int offset;
	bank=pin/16;
	offset=pin%16;
	return GPIO_setOutput(bank,offset,value);

}

/*
*************************************************************************************
*  @brief:		set dir of gpio 
*  @param[in]	int pin: pin index
*  @param[in]	int dir: 0 is output GPIO_OUTPUT;1 is input GPIO_INPUT 
*  @param[out]
*  @return:
**************************************************************************************
*/
uint32_t GPIO::set_dir(int pin,int dir){
	int bank;
	int offset;
	bank=pin/16;
	offset=pin%16;
	return	GPIO_setDir(bank,offset,dir); 
}
void AdStartIoIinit(void)
{
#if 0
//管脚配置在Arm侧已经配置过了
	uint32 *PgpioBintEn  ((uint32_t    *)GPIO_BINTEN);
//STEP 1  gpio 属性配置
	 /*
	 GPIO0_9:  终端故障LED  PINMUX[0]. bit27-24 :1000B
	 GPIO0_10: 故障复归按键   PINMUX[0]. bit23-20 :1000B
	 GPIO0_15: 线路故障LED  PINMUX[0]. bit3-0   :1000B
	 */
	EVMOMAPL138_pinmuxConfig(0,0x0ff0000f,0x08800008);
	 /*
	 GPIO2_12:  遥测1板 PINMUX[5]. bit15-12 :1000B
	 */
	EVMOMAPL138_pinmuxConfig(5,0x0000f000,0x00008000);
	 /*
	 GPIO4_5: i2c SDA  PINMUX[10]. bit11-8 :1000B
	 GPIO4_6: i2c SCL  PINMUX[10]. bit7-4  :1000B
	 */
	EVMOMAPL138_pinmuxConfig(10,0x00000ff0,0x00000880);
	 /*
	 GPIO6_3:  启动继电器2  PINMUX[19]. bit15-12 :1000B
	 GPIO6_8:  开入开出板4  PINMUX[13]. bit31-28 :1000B
	 GPIO6_9:  开入开出板1  PINMUX[13]. bit27-24 :1000B
	 GPIO6_10: 开入开出板2  PINMUX[13]. bit23-20 :1000B
	 GPIO6_11: 开入开出板3  PINMUX[13]. bit19-16 :1000B
	 GPIO6_12: 扩展板                  PINMUX[13]. bit15-12 :1000B
	 */
	EVMOMAPL138_pinmuxConfig(19,0x0000f000,0x00008000);
	EVMOMAPL138_pinmuxConfig(13,0xfffff000,0x88888000);
	 /*
	 GPIO7_8:  AD7606中断        PINMUX[17]. bit7-4 :1000B
	 GPIO7_9:  启动AD7606   PINMUX[17]. bit3-0 :1000B
	 GPIO7_10: 复位AD7606   PINMUX[16]. bit31-28 :1000B
	 GPIO7_15: 运行指示灯         PINMUX[16]. bit11-8   :1000B
	 */
	EVMOMAPL138_pinmuxConfig(17,0x000000ff,0x00000088);
	EVMOMAPL138_pinmuxConfig(16,0xf0000f00,0x80000800);
	 /*
	 GPIO8_9:  遥测5板             PINMUX[19]. bit3-0 :1000B
	 GPIO8_10: 遥测2板              PINMUX[18]. bit31-28 :1000B
	 GPIO8_12: 遥测4板              PINMUX[18]. bit23-20 :1000B
	 GPIO8_13: 启动继电器1 PINMUX[18]. bit19-16 :1000B
	 GPIO8_15: 遥测3板             PINMUX[18]. bit11-8 :1000B
	 */
	EVMOMAPL138_pinmuxConfig(19,0x0000000f,0x00000008);
	EVMOMAPL138_pinmuxConfig(18,0xf0ff0f00,0x80880800);

	//step2 gpio口极性配置；
	/*
	GPIO0_9:  终端故障LED  PINMUX[0]. bit27-24 :1000B
	GPIO0_10: 故障复归按键   PINMUX[0]. bit23-20 :1000B
	GPIO0_15: 线路故障LED  PINMUX[0]. bit3-0   :1000B
	GPIO2_12:  遥测1板 PINMUX[5]. bit15-12 :1000B
	GPIO4_5: i2c SDA  PINMUX[10]. bit11-8 :1000B
	GPIO4_6: i2c SCL  PINMUX[10]. bit7-4  :1000B
	GPIO6_3:  启动继电器2  PINMUX[19]. bit15-12 :1000B
	GPIO6_8:  开入开出板4  PINMUX[13]. bit31-28 :1000B
	GPIO6_9:  开入开出板1  PINMUX[13]. bit27-24 :1000B
	GPIO6_10: 开入开出板2  PINMUX[13]. bit23-20 :1000B
	GPIO6_11: 开入开出板3  PINMUX[13]. bit19-16 :1000B
	GPIO6_12: 扩展板                  PINMUX[13]. bit15-12 :1000B
	GPIO7_8:  AD7606中断        PINMUX[17]. bit7-4 :1000B
	GPIO7_9:  启动AD7606   PINMUX[17]. bit3-0 :1000B
	GPIO7_10: 复位AD7606   PINMUX[16]. bit31-28 :1000B
	GPIO7_15: 运行指示灯         PINMUX[16]. bit11-8   :1000B
	GPIO8_9:  遥测5板               PINMUX[19]. bit3-0 :1000B
	GPIO8_10: 遥测2板              PINMUX[18]. bit31-28 :1000B
	GPIO8_12: 遥测4板              PINMUX[18]. bit23-20 :1000B
	GPIO8_13: 启动继电器1 PINMUX[18]. bit19-16 :1000B
	GPIO8_15: 遥测3板             PINMUX[18]. bit11-8 :1000B
	*/

	SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
	SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;

	GPIO_setDir(GPIO_BANK0, GPIO_PIN9,GPIO_OUTPUT);//	GPIO0_9:  终端故障LED  PINMUX[0]. bit27-24 :1000B
	GPIO_setDir(GPIO_BANK0, GPIO_PIN10,GPIO_INPUT);//	GPIO0_10: 故障复归按键   PINMUX[0]. bit23-20 :1000B
	GPIO_setDir(GPIO_BANK0, GPIO_PIN15,GPIO_OUTPUT);//	GPIO0_15: 线路故障LED  PINMUX[0]. bit3-0   :1000B

	GPIO_setDir(GPIO_BANK2, GPIO_PIN12,GPIO_INPUT);//	GPIO2_12:  遥测1板 PINMUX[5]. bit15-12 :1000B

	GPIO_setDir(GPIO_BANK4, GPIO_PIN5,GPIO_INPUT);//	GPIO4_5: i2c SDA  PINMUX[10]. bit11-8 :1000B
	GPIO_setDir(GPIO_BANK4, GPIO_PIN6,GPIO_OUTPUT);//	GPIO4_6: i2c SCL  PINMUX[10]. bit7-4  :1000B

	GPIO_setDir(GPIO_BANK6, GPIO_PIN3,GPIO_OUTPUT);//	GPIO6_3:  启动继电器2  PINMUX[19]. bit15-12 :1000B
	GPIO_setDir(GPIO_BANK6, GPIO_PIN8,GPIO_INPUT);//	GPIO6_8:  开入开出板4  PINMUX[13]. bit31-28 :1000B
	GPIO_setDir(GPIO_BANK6, GPIO_PIN9,GPIO_INPUT);//	GPIO6_9:  开入开出板1  PINMUX[13]. bit27-24 :1000B
	GPIO_setDir(GPIO_BANK6, GPIO_PIN10,GPIO_INPUT);//	GPIO6_10: 开入开出板2  PINMUX[13]. bit23-20 :1000B
	GPIO_setDir(GPIO_BANK6, GPIO_PIN11,GPIO_INPUT);//	GPIO6_11: 开入开出板3  PINMUX[13]. bit19-16 :1000B
	GPIO_setDir(GPIO_BANK6, GPIO_PIN12,GPIO_INPUT);//	GPIO6_12: 扩展板                  PINMUX[13]. bit15-12 :1000B

	GPIO_setDir(GPIO_BANK7, GPIO_PIN8,GPIO_INPUT);//	GPIO7_8:  AD7606中断        PINMUX[17]. bit7-4 :1000B
	GPIO_setDir(GPIO_BANK7, GPIO_PIN9,GPIO_OUTPUT);//	GPIO7_9:  启动AD7606   PINMUX[17]. bit3-0 :1000B
	GPIO_setDir(GPIO_BANK7, GPIO_PIN10,GPIO_OUTPUT);//	GPIO7_10: 复位AD7606   PINMUX[16]. bit31-28 :1000B
	GPIO_setDir(GPIO_BANK7, GPIO_PIN15,GPIO_OUTPUT);//	GPIO7_15: 运行指示灯         PINMUX[16]. bit11-8   :1000B

	GPIO_setDir(GPIO_BANK8, GPIO_PIN9,GPIO_INPUT);//	GPIO8_9:  遥测5板               PINMUX[19]. bit3-0 :1000B
	GPIO_setDir(GPIO_BANK8, GPIO_PIN10,GPIO_INPUT);//	GPIO8_10: 遥测2板              PINMUX[18]. bit31-28 :1000B
	GPIO_setDir(GPIO_BANK8, GPIO_PIN12,GPIO_INPUT);//	GPIO8_12: 遥测4板              PINMUX[18]. bit23-20 :1000B
	GPIO_setDir(GPIO_BANK8, GPIO_PIN13,GPIO_OUTPUT);//	GPIO8_13: 启动继电器1 PINMUX[18]. bit19-16 :1000B
	GPIO_setDir(GPIO_BANK8, GPIO_PIN15,GPIO_INPUT);//	GPIO8_15: 遥测3板             PINMUX[18]. bit11-8 :1000B


	SYSCONFIG->KICKR[0] = KICK0R_UNLOCK;
	SYSCONFIG->KICKR[1] = KICK1R_UNLOCK;
	//step3 配置GPIO中断
	// *PgpioBintEn = 0x00000080;//BANTEN 7     BANK7中断使能打开
	CLRBIT((*PgpioBintEn),0x00000080);
	GPIO_BANK67_BIT->SET_FAL_TRIG.bit.Hbit8 = 0;

	SYSCONFIG->KICKR[0] = KICK0R_LOCK;
	SYSCONFIG->KICKR[1] = KICK1R_LOCK;
#endif
}
