/*
 *  ======== main.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/knl/Thread.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/timers/timer64/Timer.h>//for device special timer
//#include <ti/sysbios/hal/Timer.h>//for common timer

#include "debug.h"
#include "edma3_interface.h"

void fill_buff(int data);
unsigned char src[100];
unsigned char dst[100];
int count=0;
int stop=0;

UInt32 time;
int _ms=0;
int _sec=0;
int _min=0;
/*
 *  ======== Hwi fun =======
 */
Void myIsr(UArg arg0,UArg arg1){


}

void MsTimeFun(UArg arg0){
	_ms++;
	if(_ms==1000){
		_sec++;
		_ms=0;
		PFUNC(TEM_INFO,"sec:%d\r\n",_sec);
		if(count==10){
			deinit_edma();
			stop=1;
			count++;
		}
		if(!stop){
			if(irqRaised){
				count++;
				fill_buff(count);
				set_paraRamSet(0,0,&src[0],&dst[0],20,5,1,1);
				trigger_dma(0,0);
			}
		}
		if(_sec==60){
			_min++;
			PFUNC(TEM_INFO,"min:%d\r\n",_min);
			_sec=0;
		}
	}
}
void delay_ms(int ms){
	while(ms--){
		Thread_sleep(1000,NULL);
	}
}
/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
	while(1){
		Task_sleep(10);
	}
}

void fill_buff(int data){
	int i;
	for(i=0;i<100;i++)
		src[i]=data;
}
/*
 *  ======== main ========
 */
Void main()
{ 

    Task_Params task_para;
    Timer_Handle MsTimeHandle;
    Timer_Params MsTimeParams;

	//Task
    Task_Params_init(&task_para);
	task_para.stackSize=0x1000;
	task_para.priority=15;
    Task_create(taskFxn,&task_para,NULL);

	fill_buff(0);
	init_edma();
	request_channel(0,0);
	set_callback(0,NULL);
	set_paraRamSet(0,0,&src[0],&dst[0],20,5,1,1);
	trigger_dma(0,0);

    Timer_Params_init(&MsTimeParams);
    MsTimeParams.period =1000;//0.125ms     //950;
    MsTimeParams.periodType = Timer_PeriodType_MICROSECS;
    MsTimeParams.half =  Timer_Half_LOWER;
    MsTimeParams.startMode =Timer_StartMode_USER;
    MsTimeParams.intNum = 6;
    MsTimeHandle = Timer_create(1, MsTimeFun, &MsTimeParams, NULL);
    Timer_start(MsTimeHandle);

    BIOS_start();     /* enable interrupts and start SYS/BIOS */
}
