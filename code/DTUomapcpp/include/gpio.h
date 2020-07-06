/*
 * gpio.h
 *
 *  Created on: 2013-9-22
 *      Author: Administrator
 */

#ifndef GPIO_H_
#define GPIO_H_
#include "types.h"
/*-----------------------------------------------------------------------------
// Register Structure & Defines   I/O 功能设置
//-----------------------------------------------------------------------------
 *
 */
typedef struct _32bit{
	uint32_t Lbit0:1;
	uint32_t Lbit1:1;
	uint32_t Lbit2:1;
	uint32_t Lbit3:1;
	uint32_t Lbit4:1;
	uint32_t Lbit5:1;
	uint32_t Lbit6:1;
	uint32_t Lbit7:1;
	uint32_t Lbit8:1;
	uint32_t Lbit9:1;
	uint32_t Lbit10:1;
	uint32_t Lbit11:1;
	uint32_t Lbit12:1;
	uint32_t Lbit13:1;
	uint32_t Lbit14:1;
	uint32_t Lbit15:1;
	uint32_t Hbit0:1;
	uint32_t Hbit1:1;
	uint32_t Hbit2:1;
	uint32_t Hbit3:1;
	uint32_t Hbit4:1;
	uint32_t Hbit5:1;
	uint32_t Hbit6:1;
	uint32_t Hbit7:1;
	uint32_t Hbit8:1;
	uint32_t Hbit9:1;
	uint32_t Hbit10:1;
	uint32_t Hbit11:1;
	uint32_t Hbit12:1;
	uint32_t Hbit13:1;
	uint32_t Hbit14:1;
	uint32_t Hbit15:1;
}_32bit;
typedef union _32bit_u{
	_32bit bit;
	volatile uint32_t all;
}_32bitunion;
typedef struct
{
   volatile _32bitunion DIR;
   volatile _32bitunion OUT_DATA;
   volatile _32bitunion SET_DATA;
   volatile _32bitunion CLR_DATA;
   volatile _32bitunion IN_DATA;
   volatile _32bitunion SET_RIS_TRIG;
   volatile _32bitunion CLR_RIS_TRIG;
   volatile _32bitunion SET_FAL_TRIG;
   volatile _32bitunion CLR_FAL_TRIG;
   volatile _32bitunion IRQ_STAT;
} gpio_regs_u;

#define RIS_TRIG 0
#define FAL_TRIG 1
// define all the available gpio peripherals for the processor.
#define GPIOBINTEN               ((uint32_t*)GPIO_BINTEN)
extern gpio_regs_u *GPIO_BANK01_BIT;
extern gpio_regs_u *GPIO_BANK23_BIT;
extern gpio_regs_u *GPIO_BANK45_BIT;
extern gpio_regs_u *GPIO_BANK67_BIT;
extern gpio_regs_u *GPIO_BANK8_BIT;


//-----------------------------------------------------------------------------
// Public Defines and Macros
//-----------------------------------------------------------------------------


#define IN_STATU 0
#define OUT_STATU 1

#define GPIO_TO_PIN(bank,pin)	bank*16+pin

//GPIO0_9:  终端故障LED  PINMUX[0]. bit27-24 :1000B
#define TERALARM_1 {GPIO_BANK01_BIT->OUT_DATA.bit.Lbit9 =1;}
#define TERALARM_0 {GPIO_BANK01_BIT->OUT_DATA.bit.Lbit9 =0;}

//GPIO0_10: 故障复归按键   PINMUX[0]. bit23-20  :1000B
#define DEFAULTKEY (GPIO_BANK01_BIT->IN_DATA.bit.Lbit10)

//GPIO0_15: 线路故障LED  PINMUX[0]. bit3-0    :1000B
#define LINEALARM_1 {GPIO_BANK01_BIT->OUT_DATA.bit.Lbit15=1;}
#define LINEALARM_0 {GPIO_BANK01_BIT->OUT_DATA.bit.Lbit15=0;}



//GPIO7_15: 运行指示灯         PINMUX[16]. bit11-8  :1000B
#define RUN_1 (GPIO_BANK67_BIT->OUT_DATA.bit.Hbit15 =1)
#define RUN_0 (GPIO_BANK67_BIT->OUT_DATA.bit.Hbit15 =0)

////AD7606
//GPIO7_8:  AD7606中断        PINMUX[17]. bit7-4 :1000B
//GPIO7_9:  启动AD7606   PINMUX[17]. bit3-0 :1000B
//GPIO7_10: 复位AD7606   PINMUX[16]. bit31-28 :1000B
#define CONVST1A_0 {GPIO_BANK67_BIT->OUT_DATA.bit.Hbit9=0;}
#define CONVST1A_1 {GPIO_BANK67_BIT->OUT_DATA.bit.Hbit9=1;}
#define AD_RST_0 {GPIO_BANK67_BIT->OUT_DATA.bit.Hbit10=0;}
#define AD_RST_1 {GPIO_BANK67_BIT->OUT_DATA.bit.Hbit10=1;}


//启动继电器
//GPIO6_3:  启动继电器2  PINMUX[19]. bit15-12 :1000B
//GPIO8_13: 启动继电器1 PINMUX[18]. bit19-16 :1000B
//GPIO8[13]低电平，GPIO6[3]高电平，打开启动继电器
#define CLOSESTARTRELAY {GPIO_BANK8_BIT->OUT_DATA.bit.Lbit13=1;GPIO_BANK67_BIT->OUT_DATA.bit.Lbit3=0;}
#define OPENSTARTRELAY{GPIO_BANK8_BIT->OUT_DATA.bit.Lbit13=0;GPIO_BANK67_BIT->OUT_DATA.bit.Lbit3=1;}
///板载检测
#define YC_SLOT1_STATU GPIO_BANK23_BIT ->IN_DATA.bit.Lbit12//GPIO2_12:  遥测1板 PINMUX[5]. bit15-12 :1000B
#define YC_SLOT2_STATU GPIO_BANK8_BIT->IN_DATA.bit.Lbit10//GPIO8_10: 遥测2板              PINMUX[18]. bit31-28 :1000B
#define YC_SLOT3_STATU GPIO_BANK8_BIT->IN_DATA.bit.Lbit15//GPIO8_15: 遥测3板             PINMUX[18]. bit11-8 :1000B
#define YC_SLOT4_STATU GPIO_BANK8_BIT->IN_DATA.bit.Lbit12//GPIO8_12: 遥测4板              PINMUX[18]. bit23-20 :1000B
#define YC_SLOT5_STATU GPIO_BANK8_BIT->IN_DATA.bit.Lbit9//GPIO8_9:  遥测5板               PINMUX[19]. bit3-0 :1000B
#define INOUT_SLOT1_STATU GPIO_BANK67_BIT->IN_DATA.bit.Lbit9//GPIO6_9:  开入开出板1  PINMUX[13]. bit27-24 :1000B
#define INOUT_SLOT2_STATU GPIO_BANK67_BIT->IN_DATA.bit.Lbit10//GPIO6_10: 开入开出板2  PINMUX[13]. bit23-20 :1000B
#define INOUT_SLOT3_STATU GPIO_BANK67_BIT->IN_DATA.bit.Lbit11//GPIO6_11: 开入开出板3  PINMUX[13]. bit19-16 :1000B
#define INOUT_SLOT4_STATU GPIO_BANK67_BIT->IN_DATA.bit.Lbit8//GPIO6_8:  开入开出板4  PINMUX[13]. bit31-28 :1000B
#define EXT_SLOT1_STATU GPIO_BANK67_BIT->IN_DATA.bit.Lbit12//GPIO6_12: 扩展板                  PINMUX[13]. bit15-12 :1000B
//I2C
#define  SDAOUT GPIO_BANK45_BIT->OUT_DATA.bit.Lbit5       //GPIO4_5: i2c SDA  PINMUX[10]. bit11-8 :1000B
#define  SDAIN  GPIO_BANK45_BIT->IN_DATA.bit.Lbit5
#define  SCL  	GPIO_BANK45_BIT->OUT_DATA.bit.Lbit6       //GPIO4_6: i2c SCL  PINMUX[10]. bit7-4  :1000B

#define SDA_DIR(dir)     {GPIO_BANK45_BIT->DIR.bit.Lbit5 = dir;}
#define I2C_OUTPUT       GPIO_OUTPUT
#define I2C_INPUT        GPIO_INPUT


#define I2C_SDA_PIN					GPIO_TO_PIN(4,5)		/*for 24c02 sda ,used by dsp*/
#define I2C_SCL_PIN					GPIO_TO_PIN(4,6)		/*for 24c02 scl,used by dsp*/
#define IO_BOARD1_STATUS_PIN	GPIO_TO_PIN(6,9)		/*遥信遥控1板 	在线检测    */
#define IO_BOARD2_STATUS_PIN	GPIO_TO_PIN(6,10)		/*遥信遥控2板 	在线检测    */
#define IO_BOARD3_STATUS_PIN	GPIO_TO_PIN(6,11)		/*遥信遥控3板 	在线检测    */
#define IO_BOARD4_STATUS_PIN	GPIO_TO_PIN(6,8)		/*遥信遥控4板 	在线检测    */
#define EXT_BOARD_STATUS_PIN		GPIO_TO_PIN(6,12)		/*扩展板 		在线检测    */
#define YC_BOARD1_STATUS_PIN		GPIO_TO_PIN(2,12)		/*遥测1板		在线检测    */
#define YC_BOARD2_STATUS_PIN		GPIO_TO_PIN(8,10)		/*遥测2板		在线检测		*/
#define YC_BOARD3_STATUS_PIN		GPIO_TO_PIN(8,15)		/*遥测3板		在线检测    */
#define YC_BOARD4_STATUS_PIN		GPIO_TO_PIN(8,12)		/*遥测4板		在线检测    */
#define YC_BOARD5_STATUS_PIN		GPIO_TO_PIN(8,9)		/*遥测5板		在线检测    */
//GPIO8[13]低电平，GPIO6[3]高电平，打开启动继电器
#define POWER_UP_RELAY_PIN1			GPIO_TO_PIN(8,13)		/*启动继电器口1    */
#define POWER_UP_RELAY_PIN2			GPIO_TO_PIN(6,3)		/*启动继电器口2    */
#define CONVST_AD_PIN				GPIO_TO_PIN(7,9)		/*启动ad7606    */
#define RESET_AD_PIN				GPIO_TO_PIN(7,10)		/*reset 7606    */
#define INT_AD_PIN					GPIO_TO_PIN(7,8)		/*int of 7606    */
#define FAULT_RESET_PIN				GPIO_TO_PIN(0,10)		/*线路故障复归按键检测   */
#define TRE_ERR_IND_PIN				GPIO_TO_PIN(0,9)		/*终端故障指示灯     */
#define LINE_ERR_IND_PIN			GPIO_TO_PIN(0,15)		/*线路故障指示灯     */

//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------
/**GPIO管脚类
*/
class GPIO{
public:
	uint8_t		get(int pin);
	uint32_t	set(int pin,int value);
	uint32_t	set_dir(int pin,int dir);
};
extern GPIO g_gpio;
#endif /* GPIO_H_ */
