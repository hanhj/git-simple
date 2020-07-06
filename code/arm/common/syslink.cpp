/*
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "datatype.h"


/* cstdlib header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//for usleep
///* host header files */
#ifdef CONFIG_SYSLINK
#include "syslink.h"
/* local header files */
#include "AppCommon.h"
#include "SystemCfg.h"
#include "debug.h"
#include "utils.h"
#include "custom.h"

#define DSPRUNDATA 		RTDATA_ROOT"dsprun.dat"
#define DSP_CONFIG_EXEC APP_ROOT"dsp.xe674"

static UInt32 App_waitForEvent(Event_Queue* eventQueue);
static void App_notifyCB(UInt16 procId, UInt16 lineId, UInt32 eventId, UArg arg,UInt32 payload);
SysLink syslink;


void SysLink::ShutDownDsp(void)
{
	system("slaveloader shutdown DSP");
}
/*
slaveloader list可以列出当前DSP核的运行状况，其输出如下：
Current status of slave cores:
Slave core 0:      DSP [Reset]
其状态有如下：
Powered
Reset
Unknown
*/
void SysLink::StartUpDsp(void)
{
	char buf[100];
	int ret;
	char * ptr;

	//system("slaveloader list> /home/root/rtdata/dsp_run");
	sprintf(buf,"slaveloader list> %s",(char *)DSPRUNDATA);
	system(buf);
	ret=read_file((char *)DSPRUNDATA,buf,100,"rt",0);
	if(ret>=0){
		ptr=str_find(buf,(char *)"[");
		if(ptr!=NULL){
			ret=str_compare(ptr,1,(char *)"Unknown");
			if(ret!=1)
				{
					ShutDownDsp();
					sleep(1);
				}
		}
	}
	//system("slaveloader startup DSP /home/root/bin/dsp.xe674");
	sprintf(buf,"slaveloader startup DSP %s",(char *)DSP_CONFIG_EXEC);
	system(buf);
}


/* private data */
int SysLink::SysLinkInit(void)
{
	int i;
	int numProcs;
	String name;
	int status = 0;
	memcpy(remote_proc_name,"DSP",4);
	proc_num = 0;
	PFUNC(TEM_INFO,DEBUG_VDSP,"SysLinkInit: Processor List\n");
    SysLink_setup();
    numProcs = MultiProc_getNumProcessors();
    for (i = 0; i < numProcs; i++) {
         name = MultiProc_getName(i);
         PFUNC(TEM_INFO,DEBUG_VDSP,"SysLinkInit: procId=%d, procName=%s\n", i, name);
	} 
	remote_proc_id=MultiProc_getId(remote_proc_name);


	status = Ipc_control(remote_proc_id, Ipc_CONTROLCMD_LOADCALLBACK, NULL);

    if (status < 0) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"SysLinkInit: load callback failed\n");
        goto leave;
    }

    /* invoke the SysLink start callback */
    status = Ipc_control(remote_proc_id, Ipc_CONTROLCMD_STARTCALLBACK, NULL);

    if (status < 0) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"SysLinkInit: start callback failed\n");
        goto leave;
    }
	PFUNC(TEM_INFO,DEBUG_VDSP,"SysLinkInit: SysLink Initial Ok!\n");
leave:
	CreateShareRegion(remote_proc_id);
	status = (status >= 0 ? 0 : status);
	return status;
}

int SysLink::SysLinkFnit(void)
{
    Int             status      = 0;
    UInt32          event       = 0;
    IHeap_Handle    heap; 

    PFUNC(TEM_INFO,DEBUG_VDSP,"--> App_delete:\n");
    SendCmd(APP_CMD_SHUTDOWN,0);
    event=RecvCmd();
    if((event&APP_CMD_MASK)!=APP_CMD_SHUTDOWN_ACK)
    	goto leave;
    /* 2. free buffer memory */
    heap = (IHeap_Handle) SharedRegion_getHeap(SHARED_REGION_1);
    
    Memory_free(heap,Module.bufferPtr[0],DATA_BUF_SIZE);

    /* 3. unregister notify callback */
    status = Notify_unregisterEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, App_notifyCB, (UArg) &Module.eventQueue);

    if (status < 0) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"App_delete: Unregistering notify callback failed\n"); 
        goto leave;
    }	
	/* invoke the SysLink stop callback */
    status = Ipc_control(remote_proc_id, Ipc_CONTROLCMD_STOPCALLBACK, NULL);

    if (status < 0) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"SysLinkFnit: stop callback failed\n");
        goto leave;
	}
	 /* SysLink finalization */
    SysLink_destroy();
	PFUNC(TEM_INFO,DEBUG_VDSP,"SysLinkInit: SysLink Finalization Ok!\n");
leave:
	ShutDownDsp();
	status = (status >= 0 ? 0 : status);
	return status;
}


int SysLink::CreateShareRegion(UInt16 remoteProcId)
{
	Int status =0;
    int retStatus;
    
    
    PFUNC(TEM_INFO,DEBUG_VDSP,"--> CreateShareRegion:\n");
    
    /* setting default values */
    Module.eventQueue.head      = 0;               
    Module.eventQueue.tail      = 0;              
    Module.eventQueue.error     = 0;       
    Module.lineId               = SystemCfg_LineId;
    Module.eventId              = SystemCfg_EventId;
    Module.remoteProcId         = remoteProcId;
    
    /* 1. create sync object */
    retStatus = sem_init(&Module.eventQueue.semH, 0, 0);
    if(retStatus == -1) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Could not initialize a semaphore\n");
        status = -1;
        goto leave;
    }

    /* 2. register notify callback */
    status = Notify_registerEvent(Module.remoteProcId, Module.lineId,Module.eventId, App_notifyCB, (UArg) &Module.eventQueue);

    if (status < 0) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Host failed to register event\n");
        goto leave;
    }

    /* 3. wait until remote core has also registered notify callback */
    do {
        status = Notify_sendEvent(Module.remoteProcId, Module.lineId,Module.eventId,APP_CMD_NOP, TRUE);
				PFUNC(TEM_INFO,DEBUG_VDSP,"CreateShareRegion: 3. wait until remote core has also registered notify callback\n");
        if (status == Notify_E_EVTNOTREGISTERED) {
            usleep(100);
        }
        
    } while (status == Notify_E_EVTNOTREGISTERED);
		PFUNC(TEM_INFO,DEBUG_VDSP,"CreateShareRegion: event rigist ok\n");
    if (status != Notify_S_SUCCESS) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Waiting for remote core to register has failed\n");
        status = -1;
        goto leave;
    }

    /* get handle to shared region 1 heap */
    heap = (IHeap_Handle) SharedRegion_getHeap(SHARED_REGION_1);

    if (heap == NULL) {
        status = -1;
        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Shared region heap does not exist\n");
        goto leave;
    }

    /* 4. create data buffer from shared region 1 heap */
    Module.bufferPtr[0] = (Char *) Memory_calloc(heap,DATA_BUF_SIZE,0,NULL);
    
    if(Module.bufferPtr[0] == NULL) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Failed to create data buffer from shared region 1 ""heap\n");
        status = -1;
        goto leave;
    }
    /* 5. create msg buffer from shared region 1 heap */
//    Module.bufferPtr[1] = (Char *) Memory_calloc(heap,MSG_BUF_SIZE,0,NULL);
//    
//    if(Module.bufferPtr[1] == NULL) {
//        PFUNC(TEM_ERROR,DEBUG_VDSP,"CreateShareRegion: Failed to create msg buffer from shared region 1 ""heap\n");
//        status = -1;
//        goto leave;
//    }

	SendShareBufferAddr2Dsp(Module.bufferPtr[0]);

  PFUNC(TEM_INFO,DEBUG_VDSP,"CreateShareRegion: Host is ready\n");

leave: 
    PFUNC(TEM_INFO,DEBUG_VDSP,"<-- CreateShareRegion:\n");
    return (status);
}
 int SysLink::Write(char *buf ,UInt32 len)
 {
	if(len>DATA_BUF_SIZE)
	{
		return -1;
	}
	memcpy(Module.bufferPtr[0],buf,len);
	SendCmd(CMD_WRITE_DATA,len);
	while((RecvCmd()&APP_CMD_MASK)!=CMD_WRITE_DATA_ACK);
	return 1;
 }
 int SysLink::Read(char *buf ,UInt32 *len)
 {
    int cmd ;
	if(*len>DATA_BUF_SIZE)
	{
		return -1;
	}
	memcpy(Module.bufferPtr[0],buf,*len);
	SendCmd(CMD_READ_DATA,*len);
	while(((cmd=RecvCmd())&APP_CMD_MASK)!=CMD_READ_DATA_ACK);
	if((cmd&APP_SPTR_MASK)<*len)
		*len=cmd&APP_SPTR_MASK;
	memcpy(buf,Module.bufferPtr[0],*len);
	return 1;
 }

 int SysLink::CheckEvent(char *buf ,UInt32 *len)
 {
	UInt32 event;
	SendCmd(CMD_CHECK_MSG,*len);
	do{
		event=RecvCmd();
	}while((event&APP_CMD_MASK)!=CMD_CHECK_MSG_ACK&&(event&APP_CMD_MASK)!=CMD_CHECK_MSG_NACK);
    if((event&APP_CMD_MASK)==CMD_CHECK_MSG_ACK) {
		memcpy(buf,Module.bufferPtr[0],event&APP_SPTR_MASK);
		*len=event&APP_SPTR_MASK;
		return 1;
	 }
	 else {
	 	*len=0;
		return 0;
	 }
 }
int SysLink::SendShareBufferAddr2Dsp(char *buf_addr)
{       
    int status =0;
    int event =0;
		int command[2];
		command[0]=APP_SPTR_LADDR;
		command[1]=APP_SPTR_HADDR;
    /* 1. convert local address space pointer to generic shared region pointer */
		SharedRegion_SRPtr  sharedBufferPtr;
    sharedBufferPtr = SharedRegion_getSRPtr(buf_addr, SHARED_REGION_1);
    SendCmd(command[0],sharedBufferPtr & 0xFFFF);
		event=RecvCmd();
		if(event!=APP_SPTR_ADDR_ACK)
		{
				PFUNC(TEM_ERROR,DEBUG_VDSP,"SendShareBufferAddr2Dsp: LADDR Received queue error: %x\n",event); 
				goto leave;
		}
		SendCmd(command[1],((sharedBufferPtr >> 16) & 0xFFFF));
		event=RecvCmd();
		if(event!=APP_SPTR_ADDR_ACK)
		{
			PFUNC(TEM_ERROR,DEBUG_VDSP,"SendShareBufferAddr2Dsp: HADDR Received queue error: %x\n",event); 
			goto leave;	
    }
	
 leave:
    PFUNC(TEM_INFO,DEBUG_VDSP,"--> SendShareBufferAddr2Dsp:\n");
    return (status);
}

int SysLink::RecvCmd(void)
{
	 UInt32              event           = 0;
	 /* wait for shared region address acknowledge command */
    event = App_waitForEvent(&Module.eventQueue);

    if (event >= APP_E_FAILURE) {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"RecvCmd: Received queue error: %lx\n",event); 

        goto leave;
    }
leave:
    PFUNC(TEM_INFO,DEBUG_VDSP,"<-- RecvCmd:%lx\n",event);	
	return event;
}


int SysLink::SendCmd(UInt32 cmd,UInt32 data)
{
	 int                 status          = 0;
    UInt32 command;
    command = cmd| data;

    /* 2. send shared region pointer address lower two bytes */
    status = Notify_sendEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, command, TRUE);

    if (status < 0 )  {
        PFUNC(TEM_ERROR,DEBUG_VDSP,"SendCmd: Error send:%lx \n",command);
        goto leave;
    }

leave:
    PFUNC(TEM_INFO,DEBUG_VDSP,"--> SendCmd:%lx\n",command);
    return (status);
}
/*
 *  ======== App_notifyCB ========
 */
static void App_notifyCB(UInt16 procId, UInt16 lineId, UInt32 eventId, UArg arg,UInt32 payload)
{
    UInt32            next;
    Event_Queue*    eventQueue = (Event_Queue *) arg;

    /* ignore no-op events */
    if (payload == APP_CMD_NOP) {
        return;
    }

    /* compute next slot in queue */
    next = (eventQueue->head + 1) % QUEUESIZE;

    if (next == eventQueue->tail) {
        /* queue is full, drop event and set error flag */
        eventQueue->error = APP_E_OVERFLOW;
    }
    else {
        /* queue head is only written to within this function */
        eventQueue->queue[eventQueue->head] = payload;
        eventQueue->head = next;
        
        /* signal semaphore (counting) that new event is in queue */
        sem_post(&eventQueue->semH);
    }

    return;
}

/*
 *  ======== App_waitForEvent ========
 */
static UInt32 App_waitForEvent(Event_Queue* eventQueue)
{
    UInt32 event;

    if (eventQueue->error >= APP_E_FAILURE) {
        event = eventQueue->error;
    }
    else {
        /* use counting semaphore to wait for next event */

        sem_wait(&eventQueue->semH);

        /* remove next command from queue */
        event = eventQueue->queue[eventQueue->tail];
        
        /* queue tail is only written to within this function */
        eventQueue->tail = (eventQueue->tail + 1) % QUEUESIZE;
    }

    return (event);
}
#endif
