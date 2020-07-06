/*
 * interruptapp.h
 *
 *  Created on: 2013-8-7
 *      Author: Administrator
 */
#ifndef INTERRUPTAPP_H_
#define INTERRUPTAPP_H_
typedef struct {
	List_Elem elem;
	UInt32 timeout;
	UInt32 period;
	Void (*fxn)(UArg);
	UArg arg;
} usr_Clock_Object;
/*
*************************************************************************************
*  @brief  定制器硬中断 功能：1 负责启动FPGA，2：将FPGA读取的数据备份 3：遥测防抖处理 4:启动一个软中断
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
extern void interrupt_init(void);
extern void StartIr(void);
//软中断
extern Swi_Handle SwiFftHandle;
extern Swi_Handle SwiGetYxHandle;
extern Swi_Handle SwiGetAdHandle;
extern Swi_Handle SwiFaHandle;


extern Uint64 Sys500us;
extern Uint64 HafSysms;
extern Void SwiGetAdFxn(UArg arg1, UArg arg2);
extern void GetFirstYxData(void);
#endif /* INTERRUPTAPP_H_ */
