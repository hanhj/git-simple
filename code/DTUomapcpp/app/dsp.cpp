#include "scada.h"
#include "fa.h"
#include "task.h"
#include "message.h"
#include "varm.h"
#include "interruptapp.h"
#include "gpio.h"
#include "fpga.h"
#include "evmomapl138.h"
#include "product.h"
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>

void SendMsg(int32 Id);
void DealMsg(int32 Id);
Task task;
Int main(Int argc, Char* argv[])
{
	//FpgaInit();
	FpgaCfgInit();
	interrupt_init();
	//register thread
	task.Regist(&scada,scada_main);
#if LINK_TO_ARM
	task.Regist(&varm,varm_main);
#endif

	//start thread
	task.StartThread(scada.Id);
#if LINK_TO_ARM
	task.StartThread(varm.Id);
#endif
	BIOS_start();

}

