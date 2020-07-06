/*
 * interruptapp.cpp
 *
 *  Created on: 2013-8-7
 *      Author: Administrator
 */

//#include "os.h"
#include "message.h"
#include "scada.h"
#include "types.h"
#include "datatype.h"
#include "interruptapp.h"
#include "fft.h"
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "fpga.h"
#include "gpio.h"
#include "omapl138.h"
#include "clock.h"
#include "fa.h"
#include "omapl138timer.h"
usr_Clock_Object clk1, clk2;

Timer_Handle HwiTimerHandle;//
Timer_Params HwiTimerParams;

Timer_Handle DynamictimerHandle;
Timer_Params DynamictimerParams;

Timer_Handle Null0TimeHandle;
Timer_Handle Null1TimeHandle;

Clock_Handle ClockHandle1;
Clock_Params ClockParams;

//extern Timer_Handle  SampTimer;


Semaphore_Handle sem;
Semaphore_Params semparams;

Swi_Handle SwiFftHandle;

List_Handle clockList;
Clock_Handle obj1;


Hwi_Params hwiParams;
Hwi_Handle HwiHandle;

Hwi_Params GpioB0Int0Params;
Hwi_Handle GpioB0Int0Handle;
uint64 time1;
uint64 time2;
uint64 time3;
#define TIME1_12_COUNT TIMER64P0DSP

int16 IrtFlag  = 0;
uint8 led = 0;
uint16 Hwienable = 5;
uint16 enbIR  = 1;
#pragma CODE_SECTION("codeHwiDynamic")
Void  HwiDynamic(UArg arg )//Timer_FuncPtr
{
	uint8 i;
	//if(enbIR==0){
	//	return ;
	//}
	//GPIO_setOutput(GPIO_BANK8, GPIO_PIN15, 1);
	SETBIT(GPIO_BANK8_->OUT_DATA, 0x00008000);
	for (i = 0; i< Hwienable;i++);
	CLRBIT(GPIO_BANK8_->OUT_DATA, 0x00008000);
	//IrtFlag++;
	//IrtNum++;
	//GPIO_setOutput(GPIO_BANK8, GPIO_PIN15, 0);
	// Swifft(0, 0);
	i = FpgaSlave[0].ID.all;
}

#pragma CODE_SECTION("codeHwiDynamic")
void softtime(void)
{

}
#pragma CODE_SECTION("codeHwiDynamic")
Void clk1Fxn(UArg arg)
{

	//printf("clk1Fxn run\r\n");
	//softtime();
	//task.CheckHeart();
	clk1.arg++;
}
#pragma CODE_SECTION("codeHwiDynamic")
Void clk2Fxn(UArg sem)
{
	//printf("clk2Fxn run\r\n");

	//Semaphore_post((Semaphore_Object *)sem);

}

/*
*************************************************************************************
*  @brief
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
uint8 yxnum = 3;
uint16 yxdata = 0;
#pragma CODE_SECTION("codeHwiDynamic")
Void HwiTimer(UArg arg)
{
	//printf("hwiFxn run\r\n");
	clk0.tick();
	if(scada.scada_cfg.d_5211.Data==1){
		ManSet();
		scada.scada_cfg.d_5211.Data=0;
	}

}
/*
*************************************************************************************
*  @brief
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
/*
Void swiFxn(UArg arg1, UArg arg2)
{
	List_Elem *elem;
	usr_Clock_Object *obj;
	elem =(List_Elem *) List_next(clockList, NULL);
	while (elem != NULL) {
		obj = (usr_Clock_Object *)elem;
		obj->timeout -= 1;
		if (obj->timeout == 0) {
			obj->timeout = obj->period;
			(obj->fxn)(obj->arg);
		}
		elem =(List_Elem *)List_next(clockList, elem);
	}
}*/
/*
*************************************************************************************
*  @brief
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void interrupt_int(void)
{

	//Timer_Params_init(&HwiTimerParams);
	Timer_Params_init(&DynamictimerParams);
	Clock_Params_init(&ClockParams);
	Hwi_Params_init(&hwiParams);

	SwiFftHandle = Swi_create(Swifft, NULL, NULL);
	semparams.mode =Semaphore_Mode_BINARY;// Semaphore_Mode_COUNTING;//Semaphore_Mode_BINARY;
	sem = Semaphore_create(1, &semparams, NULL);

	ClockParams.period = 1000;
	ClockParams.startFlag = 1;
	Clock_create(HwiTimer,1,&ClockParams,NULL);

	DynamictimerParams.period = 312.5;//46875
	DynamictimerParams.periodType = Timer_PeriodType_MICROSECS;
	//DynamictimerParams.half =  Timer_Half_UPPER;
	DynamictimerParams.startMode =Timer_StartMode_USER;
	DynamictimerHandle = Timer_create(2, NULL, &DynamictimerParams, NULL);
	TMR2->PRD12 = 46875;

	hwiParams.arg = 12;
	hwiParams.enableInt = FALSE;
	hwiParams.eventId = 25;//T1_12 40  ;; T2 25
	hwiParams.priority = 11;
	HwiHandle = Hwi_create(14, HwiDynamic, &hwiParams, NULL);
	Hwi_enableInterrupt(14);

	GpioB0Int0Params.enableInt = FALSE;
	GpioB0Int0Params.eventId = 49;//GPIO Bank 2 Interrupt
	GpioB0Int0Params.priority = 14;
	GpioB0Int0Handle=Hwi_create(6, nmifxn, &GpioB0Int0Params, NULL);
	Hwi_enableInterrupt(6);
	memset(&AD_BUF,0,sizeof(AD_BUF));
	memset(&AD16_BUF,0,sizeof(AD16_BUF));
	tw_gen (ptr_w64, 64);
	tw_gen (ptr_w16, 16);
	Timer_start(DynamictimerHandle);
}
/*
*************************************************************************************
*  @brief
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void FreqFollow(int16 cyc)
{

}
