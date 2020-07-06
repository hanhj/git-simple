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
#include "omapl138.h"
//#include "evmomapl138_timer.h"
#include "gpio.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define GPIO_ODD_BANK_PIN_ADDER     (16)

/*
const uint32 bitval_u32[32] =
{
   0x00000001, 0x00000002, 0x00000004, 0x00000008,
   0x00000010, 0x00000020, 0x00000040, 0x00000080,
   0x00000100, 0x00000200, 0x00000400, 0x00000800,
   0x00001000, 0x00002000, 0x00004000, 0x00008000,
   0x00010000, 0x00020000, 0x00040000, 0x00080000,
   0x00100000, 0x00200000, 0x00400000, 0x00800000,
   0x01000000, 0x02000000, 0x04000000, 0x08000000,
   0x10000000, 0x20000000, 0x40000000, 0x80000000
};
*/

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static gpio_regs_t * getRegisterBankAndBit(uint32 in_bank, uint8 in_pin_num, uint32 *reg_bit);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// \brief   sets the direction (input/output) of the given gpio pin.
//
// \param   uint32 in_bank - gpio bank for desired pin.
//
// \param   uint8 in_pin_num - gpio pin within bank to be set.
//
// \param   uint8 in_dir - input or output.
//
// \return  uint32
//    ERR_NO_ERROR - input in bounds, gpio direction set.
//    ERR_INVALID_PARAMETER - input out of bounds.
//-----------------------------------------------------------------------------
uint32 GPIO_setDir(uint32 in_bank, uint8 in_pin_num, uint8 in_dir)
{
   uint32 rtn = ERR_INVALID_PARAMETER;

   if ((in_bank < MAX_GPIO_BANK_NUM) && (in_pin_num < MAX_GPIO_PIN_NUM))
   {
      gpio_regs_t *gpio_bank;
      uint32 gpio_bit;

      // get the register bank and bitmask for the input bank and pin.
      gpio_bank = getRegisterBankAndBit(in_bank, in_pin_num, &gpio_bit);

      if (GPIO_OUTPUT == in_dir)
      {
         // clear the bit to config pin for output.
         CLRBIT(gpio_bank->DIR, gpio_bit);
      }
      else
      {
         // set the bit to config pin for input.
         SETBIT(gpio_bank->DIR, gpio_bit);
      }

      rtn = ERR_NO_ERROR;
   }

   return (rtn);
}

uint32 GpioSetBankITR(uint32 in_bank, uint8 in_pin_num,uint8 type)
{
   uint32 rtn = ERR_INVALID_PARAMETER;

   if ((in_bank < MAX_GPIO_BANK_NUM) && (in_pin_num < MAX_GPIO_PIN_NUM))
   {
      gpio_regs_t *gpio_bank;
      uint32 gpio_bit;
      gpio_bank = getRegisterBankAndBit(in_bank, in_pin_num, &gpio_bit);
      if (type == FAL_TRIG){
    	  SETBIT(gpio_bank->SET_FAL_TRIG, 0x00000001);
      }else if (type == RIS_TRIG){
    	  SETBIT(gpio_bank->SET_RIS_TRIG, 0x00000001);
      }

   }
   return (rtn);
}
//-----------------------------------------------------------------------------
// \brief   returns the current input of the given gpio pin.
//
// \param   uint32 in_bank - gpio bank for desired pin.
//
// \param   uint8 in_pin_num - gpio pin within bank to get input.
//
// \param   uint8 *gpio_val - gpio pin data
//                      0 - pin is cleared.
//                      1 - pin is set.
//
// \return  uint32
//    ERR_NO_ERROR - input in bounds, data returned in gpio_val.
//    ERR_INVALID_PARAMETER - input out of bounds.
//-----------------------------------------------------------------------------
uint32 GPIO_getInput(uint32 in_bank, uint8 in_pin_num, uint8 *gpio_val)
{
   uint32 rtn = ERR_INVALID_PARAMETER;

   if ((in_bank < MAX_GPIO_BANK_NUM) && (in_pin_num < MAX_GPIO_PIN_NUM))
   {
      gpio_regs_t *gpio_bank;
      uint32 gpio_bit;

      // get the register bank and bitmask for the input bank and pin.
      gpio_bank = getRegisterBankAndBit(in_bank, in_pin_num, &gpio_bit);

      if (CHKBIT(gpio_bank->IN_DATA, gpio_bit))
      {
         // in data is set, return 1.
         *gpio_val = 1;
      }
      else
      {
         // in data is clear, return 0.
         *gpio_val = 0;
      }

      rtn = ERR_NO_ERROR;
   }

   return (rtn);
}

//-----------------------------------------------------------------------------
// \brief   sets the given gpio pin output.
//
// \param   uint32 in_bank - gpio bank for desired pin.
//
// \param   uint8 in_pin_num - gpio pin within bank to be set.
//
// \param   uint8 in_val - 0/1 to set or clear the pin.
//
// \return  uint32
//    ERR_NO_ERROR - input in bounds, gpio pin set.
//    ERR_INVALID_PARAMETER - input out of bounds.
//-----------------------------------------------------------------------------
#pragma CODE_SECTION("codeGPIO_setOutput")
uint32 GPIO_setOutput(uint32 in_bank, uint8 in_pin_num, uint8 in_val)
{
   uint32 rtn = ERR_INVALID_PARAMETER;

   if ((in_bank < MAX_GPIO_BANK_NUM) && (in_pin_num < MAX_GPIO_PIN_NUM))
   {
      gpio_regs_t *gpio_bank;
      uint32 gpio_bit;

      // get the register bank and bitmask for the input bank and pin.
      gpio_bank = getRegisterBankAndBit(in_bank, in_pin_num, &gpio_bit);

      if (in_val)
         SETBIT(gpio_bank->OUT_DATA, gpio_bit);
      else
         CLRBIT(gpio_bank->OUT_DATA, gpio_bit);

      rtn = ERR_NO_ERROR;
   }

   return (rtn);
}

//-----------------------------------------------------------------------------
// \brief   sits in a forever loop toggling a given set of pins.
//
// \param   uint32 *in_bank - gpio bank for desired pins
//
// \param   uint8 *in_pin_num - gpio pins within bank to be set.
//
// \param   uint8 in_num_pins - number of pins to be toggled.
//
// \return  none.
//-----------------------------------------------------------------------------
void GPIO_toggleLoop(uint8 in_bank, uint8 in_pin, uint8 in_dir ,uint8 in_pins_value)
{
  // uint8 pin_val = 0;

   //GPIO_setDir(in_bank, in_pin, GPIO_OUTPUT);
   GPIO_setDir(in_bank, in_pin, in_dir);
   //while (in_pins_value--) {
	//  GPIO_setOutput(in_bank, in_pin, pin_val);

	  //UTIL_waitLoop(200000);

      //if (pin_val)
         //pin_val = 0;
      //else
         //pin_val = 1;
   //}
}

/*
void GPIO_toggleLoop(uint32 *in_bank, uint8 *in_pin_num, uint8 in_num_pins)
{
   uint8 pin_val = 0;
   uint8 i;

   for (i = 0; i < in_num_pins; i++)
   {
      GPIO_setDir(in_bank[i], in_pin_num[i], GPIO_OUTPUT);
   }

   while (1)
   {
      for (i = 0; i < in_num_pins; i++)
      {
         GPIO_setOutput(in_bank[i], in_pin_num[i], pin_val);
      }
      USTIMER_delay(5000);

      if (pin_val)
         pin_val = 0;
      else
         pin_val = 1;
   }
}
*/
//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// converts a bank (0-8) / pin (0-15) to a register bank (0-3) and bit (0-31).
//-----------------------------------------------------------------------------
#pragma CODE_SECTION("codeGPIO_setOutput")
static gpio_regs_t * getRegisterBankAndBit(uint32 in_bank, uint8 in_pin_num, uint32 *reg_bit)
{
   gpio_regs_t *rtn;

   switch (in_bank)
   {
      case GPIO_BANK0:
         rtn = GPIO_BANK01;
         break;

      case GPIO_BANK1:
         rtn = GPIO_BANK01;
         in_pin_num += GPIO_ODD_BANK_PIN_ADDER;
         break;

      case GPIO_BANK2:
         rtn = GPIO_BANK23;
         break;

      case GPIO_BANK3:
         rtn = GPIO_BANK23;
         in_pin_num += GPIO_ODD_BANK_PIN_ADDER;
         break;

      case GPIO_BANK4:
         rtn = GPIO_BANK45;
         break;

      case GPIO_BANK5:
         rtn = GPIO_BANK45;
         in_pin_num += GPIO_ODD_BANK_PIN_ADDER;
         break;

      case GPIO_BANK6:
         rtn = GPIO_BANK67;
         break;

      case GPIO_BANK7:
         rtn = GPIO_BANK67;
         in_pin_num += GPIO_ODD_BANK_PIN_ADDER;
         break;

      case GPIO_BANK8:
         rtn = GPIO_BANK8_;
         break;
   }

   *reg_bit = bitval_u32[in_pin_num];

   return (rtn);
}

void AdStartIoIinit(void)
{
uint32 *PgpioBintEn  ((uint32    *)GPIO_BINTEN);
	EVMOMAPL138_pinmuxConfig(1,0x000000f0,0x00000080);//GPIO0_6 IO口 LED
	EVMOMAPL138_pinmuxConfig(6,0xF0000000,0x80000000);//GPIO2_0  设置 为IO
	EVMOMAPL138_pinmuxConfig(18,0x00000F00,0x00000800);//GPIO8_15  设置 为IO口

	GPIO_setDir(GPIO_BANK0, GPIO_PIN6,GPIO_OUTPUT);
	GPIO_setDir(GPIO_BANK2, GPIO_PIN0,GPIO_INPUT);
	GPIO_setDir(GPIO_BANK8, GPIO_PIN15, GPIO_OUTPUT);
	*PgpioBintEn = 0x00000004;//BANTEN 2
	GpioSetBankITR(GPIO_BANK2,GPIO_PIN0,RIS_TRIG);//上升中断
}
