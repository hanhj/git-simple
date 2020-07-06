#ifndef _SYSLINK_2_21_01_05__H_
#define _SYSLINK_2_21_01_05__H_
#ifdef CONFIG_SYSLINK
//#include "datatype.h"
/* xdctools header files */
#include <xdc/std.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Registry.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
/* package header files */
#include <ti/ipc/Notify.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>

/* max number of outstanding commands minus one */
#define QUEUESIZE   8   

/* queue structure */
typedef struct event_queue_
{
    UInt32              queue[QUEUESIZE];   /* command queue */
    UInt                head;               /* queue head pointer */
    UInt                tail;               /* queue tail pointer */
    UInt32              error;              /* error flag */
    Semaphore_Struct    semObj;             /* semaphore object */
    Semaphore_Handle    semH;               /* handle to object above */
} Event_Queue;   

/* module structure */
typedef struct share_region_module_
{
    Event_Queue     eventQueue;
    UInt16          remoteProcId;
    UInt16          lineId;     /* notify line id */
    UInt32          eventId;    /* notify event id */
    Char*           bufferPtr;  /* string buffer */
} SH_Module;
#define DATA_BUF_SIZE 32*1024
#define MSG_BUF_SIZE 2*1024
class SysLink
{
	public:
		int SysLinkInit(void);
		int SysLinkFnit(void);
		int OnWrite(char *buf ,UInt32 len);
		int OnRead(char *buf ,UInt32 len);
		int OnCheckEvent(char *buf ,UInt32 len);
		int SendCmd(UInt32 cmd,UInt32 data);
		int RecvCmd(void);
		//char data_buf[DATA_BUF_SIZE];
		void SetOnWriteFun(int16 (*fun)(char *buf ,uint32 len));
		void SetOnReadFun(uint32 (*fun)(char *buf ,uint32 len));
		void SetOnCheckFun(uint32 (*fun)(char *buf ,uint32 len));

	private:
		int GetShareBufferAddr();
		int16 RegisterSharereion(UInt16 remoteProcId);

		int16 (*UserOnWriteFun)(char *buf ,uint32 len);
		uint32 (*UserOnReadFun)(char *buf ,uint32 len);
		uint32 (*UserOnCheckFun)(char *buf ,uint32 len);

		Registry_Desc  Registry_CURDESC;
		SH_Module Module;
		int Module_curInit;
};
extern SysLink syslink;
#endif //CONFIG_SYSLINK
#endif//_SYSLINK_2_21_01_05__H_