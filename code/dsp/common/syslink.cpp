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
//#include <unistd.h>//for usleep
///* host header files */
#ifdef CONFIG_SYSLINK

#define MODULE_NAME "Server"
#include "syslink.h"

/* local header files */
#include "AppCommon.h"
#include "SystemCfg.h"


static UInt32 Server_waitForEvent(Event_Queue* eventQueue);
static void Server_notifyCB(UInt16 procId, UInt16 lineId, UInt32 eventId, UArg arg,UInt32 payload);
SysLink syslink;


Semaphore_Handle varm_sem;
/* private data */
int SysLink::SysLinkInit(void)
{
    Registry_Result result;
		int status;
		int remoteProcId;
		UserOnReadFun=NULL;
		UserOnWriteFun=NULL;
		UserOnCheckFun=NULL;
		varm_sem=Semaphore_create(0,NULL,NULL);
    //if (Module_curInit++ != 0) {
       // return 0;  /* already initialized */
    //}

    /* register with xdc.runtime to get a diags mask */
    result = Registry_addModule(&Registry_CURDESC, MODULE_NAME);
    Assert_isTrue(result == Registry_SUCCESS, (Assert_Id)NULL);


        /* only one thread must call start */
        do {
            status = Ipc_start();
            //Task_sleep(10);
        } while (status == Ipc_E_NOTREADY);

        if (status < 0) {
            Log_error0("smain: Ipc_start() failed");
            goto leave;
        }

        /* attach to the remote processor */
        remoteProcId = MultiProc_getId("HOST");

        /* connect to remote processor */
        do {
            status = Ipc_attach(remoteProcId);

            if (status == Ipc_E_NOTREADY) {
            }

        } while (status == Ipc_E_NOTREADY);

        if (status < 0) {
            Log_error0("smain: Ipc_attach() failed");
            goto leave;
        }    

		RegisterSharereion(remoteProcId);
		GetShareBufferAddr();
leave:
	 	Semaphore_post(varm_sem);
    Log_print1(Diags_EXIT, "<-- Server_create: %d", (IArg)status);
    return (status);        
}

int SysLink::SysLinkFnit(void)
{
    Int     status = 0; 

    Log_print0(Diags_ENTRY, "--> Server_delete:");
    
    /* send shutdown command acknowledge */
    status = Notify_sendEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, APP_CMD_SHUTDOWN_ACK, TRUE);

    if (status < 0 )  {
        Log_error0("Server_delete: Error sending shutdown command acknowledge");
        goto leave;
    }

    /* 2. unregister notify callback */
    status = Notify_unregisterEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, Server_notifyCB,(UArg) &Module.eventQueue);

    if (status < 0) {
        Log_error0("Server_delete: Unregistering event has failed");
        goto leave;
    }

    /* 3. delete sync object */
    Semaphore_destruct(&Module.eventQueue.semObj);
    
    Log_print0(Diags_INFO,"Server_delete: Cleanup complete");
    
        /* END server phase */

        /* disconnect from remote processor */
        do {
            status = Ipc_detach(Module.remoteProcId);

            if (status == Ipc_E_NOTREADY) {
                Task_sleep(100);
            }

        } while (status == Ipc_E_NOTREADY);

        if (status < 0) {
            Log_error0("smain: Failed to disconnect from remote process");
            goto leave;
        }

        /* only one thread must call stop */
        Ipc_stop();

    /* finalize modules */
    if (Module_curInit-- != 1) {
        return 0;  /* object still being used */
    }  

leave:
    Log_print1(Diags_EXIT, "<-- Server_delete: %d", (IArg)status);
    return (status);
}

int16 SysLink::RegisterSharereion(UInt16 remoteProcId){
	    Int                 status      = 0;
	    volatile Int        status2     = 0;
	    Semaphore_Params    semParams;
	    Log_print0(Diags_ENTRY, "--> Server_create:");
	    printf("RegisterSharereion \n");
	    /* setting default values */
	    Module.eventQueue.head      = 0;               
	    Module.eventQueue.tail      = 0;              
	    Module.eventQueue.error     = 0;       
	    Module.eventQueue.semH      = NULL;
	    Module.lineId               = SystemCfg_LineId;
	    Module.eventId              = SystemCfg_EventId;
	    Module.remoteProcId         = remoteProcId;
    	    
	    // enable some log events //
	    Diags_setMask(MODULE_NAME"+EXF");

	    // 1. create sync object //
	    Semaphore_Params_init(&semParams);
	    semParams.mode = Semaphore_Mode_COUNTING;

	    Semaphore_construct(&Module.eventQueue.semObj, 0, &semParams);

	    Module.eventQueue.semH = Semaphore_handle(&Module.eventQueue.semObj);

	    // 2. register notify callback //
	    status = Notify_registerEvent(Module.remoteProcId, Module.lineId,
	            Module.eventId, (Notify_FnNotifyCbck)Server_notifyCB,(UArg)  &Module.eventQueue);


	    if (status < 0) {
	        Log_error0("Server_create: Device failed to register notify event");
	        goto leave;
	    }

	    // 3. wait until remote core has also registered notify callback //
	    do {
	    	printf("notifu event register no ready\n");
	        status = Notify_sendEvent(Module.remoteProcId,Module.lineId,
	                Module.eventId, APP_CMD_NOP, TRUE);

	        if (status == Notify_E_EVTNOTREGISTERED) {
	            Task_sleep(100);

	        }

	    } while (status == Notify_E_EVTNOTREGISTERED);

	    if (status < 0 ) {
	        Log_error0("Server_create: Host failed to register callback");
	        goto leave;
	    }
	    printf("RegisterSharereion ok \n");
	    Log_print0(Diags_INFO, "Server_create: Slave is ready");
        return 1;
	leave:
		printf("RegisterSharereion out \n");
	    Log_print1(Diags_EXIT, "<-- Server_create: %d", (IArg)status);
	    return (status);
}




 int SysLink::OnRead(char *buf ,UInt32 len)
 {
		 	int ret;
			if(len>DATA_BUF_SIZE)
			{
				return -1;
			}
			ret=0;
			memcpy(buf,Module.bufferPtr,len);
			if(UserOnReadFun!=NULL)
			{
			ret=(*UserOnReadFun)(buf,len);
			memcpy(Module.bufferPtr,buf,ret);
			}
			SendCmd(CMD_READ_DATA_ACK,ret);
			return 1;
 }
 int SysLink::OnWrite(char *buf ,UInt32 len)
 {
			if(len>DATA_BUF_SIZE)
			{
				return -1;
			}
			memcpy(buf,Module.bufferPtr,len);
			if(UserOnWriteFun!=NULL)
			{
			(*UserOnWriteFun)(buf,len);
			}
			SendCmd(CMD_WRITE_DATA_ACK,len);
			return 1;
 }

 int SysLink::OnCheckEvent(char *buf ,UInt32 len)
 {

			if(UserOnReadFun!=NULL)
			{
				if((*UserOnCheckFun)(buf,len)>0)
					{
					memcpy(Module.bufferPtr,buf,len);
					SendCmd(CMD_CHECK_MSG_ACK,len);
					return 1;
					}
			}		
			SendCmd(CMD_CHECK_MSG_NACK,0);
			return 0;
 }
 int SysLink::GetShareBufferAddr()
{       
   Int                 status              = 0;
    SharedRegion_SRPtr  sharedBufferPtr     = 0;
    UInt32              event;

    Log_print0(Diags_ENTRY | Diags_INFO, "--> Server_exec:");

    /* 1. wait for shared region pointer address lower two bytes*/
    event = Server_waitForEvent(&Module.eventQueue);
    
    if (event >= APP_E_FAILURE) {
        Log_error1("Server_exec: Received queue error: %d",event); 
        status = -1;       
        goto leave;
    } 
   
    /* 2. save only the payload which represents the shared region pointer 
     *    address lower two bytes 
     */ 
    sharedBufferPtr = event & APP_SPTR_MASK;
    
    /* send shared region pointer address acknowledge */
    status = Notify_sendEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, APP_SPTR_ADDR_ACK, TRUE);

    if (status < 0 )  {
        Log_print0(Diags_INFO,"Server_exec: Error sending shared region pointer"
                "address acknowledge");
        goto leave;
    }    
    
    /* 3. wait for shared region pointer address upper two bytes*/
    event = Server_waitForEvent(&Module.eventQueue);
    
    if (event >= APP_E_FAILURE) {
        Log_error1("Server_exec: Received queue error: %d",event); 
        status = -1;       
        goto leave;
    } 
    
    /* 4. join shared region pointer address upper and lower two bytes */
    sharedBufferPtr = ((event & APP_SPTR_MASK) << 16) | sharedBufferPtr;
    
    /* send shared region pointer address acknowledge */
    status = Notify_sendEvent(Module.remoteProcId, Module.lineId, 
            Module.eventId, APP_SPTR_ADDR_ACK, TRUE);

    if (status < 0 )  {
        Log_print0(Diags_INFO,"Server_exec: Error sending shared region pointer"
                "address acknowledge");
        goto leave;
    }        
    /* 5. translate shared region pointer to local address space pointer */
    Module.bufferPtr = (Char *)SharedRegion_getPtr(sharedBufferPtr);
leave:
    Log_print1(Diags_EXIT, "<-- Server_exec: %d", (IArg)status);
    return (status);    
}

int SysLink::RecvCmd(void)
{
	 UInt32              event           = 0;
	 /* wait for shared region address acknowledge command */
    event = Server_waitForEvent(&Module.eventQueue);

    if (event >= APP_E_FAILURE) {
        printf("SendCmd: Received queue error: %d\n",event); 
        goto leave;
    }
leave:
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
        printf("SendCmd: Error sending shared region pointer address lower "
                "two bytes\n");
        goto leave;
    }

leave:
    printf("<-- SendCmd:%x\n",cmd);
    return (status);
}

/*
 *  ======== Server_notifyCB ========
 *
 *  This function is called from within an ISR
 */
Void Server_notifyCB( UInt16 procId, UInt16 lineId, UInt32  eventId, UArg arg,
        UInt32 payload)
{
    UInt            next;
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
        eventQueue->queue[eventQueue->head] = payload;
        
        /* queue head is only written to within this function */
        eventQueue->head = next;
        
        /* signal semaphore (counting) that new event is in queue */
        Semaphore_post(eventQueue->semH);
    }

    return;
}

/*
 *  ======== App_waitForEvent ========
 */
static UInt32 Server_waitForEvent(Event_Queue* eventQueue)
{
    UInt32 event;

    if (eventQueue->error >= APP_E_FAILURE) {
        event = eventQueue->error;
    }
    else {
        /* use counting semaphore to wait for next event */
        Semaphore_pend(eventQueue->semH,  BIOS_WAIT_FOREVER);

        /* remove next command from queue */
        event = eventQueue->queue[eventQueue->tail];
        
        /* queue tail is only written to within this function */
        eventQueue->tail = (eventQueue->tail + 1) % QUEUESIZE;
    }

    return (event);
}


void SysLink::SetOnWriteFun(int16 (*fun)(char *buf ,uint32 len)){
	UserOnWriteFun=fun;
}
void SysLink::SetOnReadFun(uint32 (*fun)(char *buf ,uint32 len)){
	UserOnReadFun=fun;
}
void SysLink::SetOnCheckFun(uint32 (*fun)(char *buf ,uint32 len)){
	UserOnCheckFun=fun;
}

#endif //CONFIG_SYSLINK