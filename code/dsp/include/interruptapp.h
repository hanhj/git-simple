/*
 * interruptapp.h
 *
 *  Created on: 2013-8-7
 *      Author: Administrator
 */




#ifndef INTERRUPTAPP_H_
#define INTERRUPTAPP_H_
/*
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>

#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>


#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sdo/utils/List.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/hal/Hwi.h>
#include <stdio.h>
#include <ti/sysbios/knl/Clock.h>
*/


/*
*************************************************************************************
*  @brief  定制器硬中断 功能：1 负责启动FPGA，2：将FPGA读取的数据备份 3：遥测防抖处理 4:启动一个软中断
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
extern Void hwiFxn(UArg arg);

/*
*************************************************************************************
*  @brief  软中断     功能：1 定时fft计算，2：软时钟  3: 对其他线程心跳监控
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
extern Void swiFxn(UArg arg1, UArg arg2);

/*
*************************************************************************************
*  @brief  硬中断，软中断服务的启动
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
extern void interrupt_int(void);


typedef struct {
	List_Elem elem;
	UInt32 timeout;
	UInt32 period;
	Void (*fxn)(UArg);
	UArg arg;
} usr_Clock_Object;





//静态定时器
extern Timer_Handle HwiTimerHandle;//
extern Timer_Params HwiTimerParams;
//动态定时器
extern Timer_Handle DynamictimerHandle;
extern Timer_Params DynamictimerParams;
//同步信号量
extern Semaphore_Handle sem;
extern Semaphore_Params semparams;
//软中断
extern Swi_Handle SwiFftHandle;
//时钟列表
extern List_Handle clockList;
extern Clock_Handle obj1;

//硬中断
extern Hwi_Params hwiParams;

extern int16 IrtFlag ;
extern int32 IrtNum ;
#endif /* INTERRUPTAPP_H_ */
