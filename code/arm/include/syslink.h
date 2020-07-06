#ifndef _SYSLINK_2_21_01_05__H_
#define _SYSLINK_2_21_01_05__H_

#ifdef CONFIG_SYSLINK
#include <xdc/std.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>


/* package header files */
#include <ti/ipc/Ipc.h>
#include <ti/ipc/MultiProc.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/ipc/Notify.h>

#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/System.h>
/* package header files */


#include <ti/sysbios/hal/Timer.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sdo/utils/List.h> /* List module in IPC */
#include <xdc/cfg/global.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <semaphore.h>//for sem_t
#include <ti/syslink/utils/Memory.h>//for IHeap_Handle
#include <ti/ipc/SharedRegion.h>
///* package header files */
#include <ti/syslink/IpcHost.h>
#include <ti/syslink/SysLink.h>



/* max number of outstanding commands minus one */
#define QUEUESIZE   8   

/* queue structure */
typedef struct event_queue_
{
    unsigned int              queue[QUEUESIZE];   /* command queue */
    unsigned int                head;               /* queue head pointer */
    unsigned int                tail;               /* queue tail pointer */
    unsigned int              error;              /* error flag */
    sem_t               semH;               /* handle to object above  */
} Event_Queue;   

/* module structure */
typedef struct share_region_module_
{
    Event_Queue     eventQueue;
    UInt16          remoteProcId;
    UInt16          lineId;     /* notify line id */
    UInt32          eventId;    /* notify event id */
    Char*           bufferPtr[2];  /* string buffer */
} SH_Module;
#define DATA_BUF_SIZE 32*1024
#define MSG_BUF_SIZE 2*1024
class SysLink
{
	public:

		int SysLinkInit(void);
		int SysLinkFnit(void);
		int Write(char *buf ,UInt32 len);
		int Read(char *buf ,UInt32 *len);
		int CheckEvent(char *buf ,UInt32 *len);
		void ShutDownDsp(void);
		void StartUpDsp(void);

	private:
		int SendShareBufferAddr2Dsp(char* buf_addr);
		int CreateShareRegion(UInt16 remoteProcId);
		char   remote_proc_name[16] ;
		unsigned char proc_num;
		unsigned short remote_proc_id;		
		//char data_buf[DATA_BUF_SIZE];
		SH_Module Module;
		IHeap_Handle heap;
		int SendCmd(UInt32 cmd,UInt32 data);
		int RecvCmd(void);
};
extern SysLink syslink;
#endif //CONFIG_SYSLINK
#endif//_SYSLINK_2_21_01_05__H_