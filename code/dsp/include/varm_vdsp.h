/*
 * varm_vdsp.h
 *
 *  Created on: 2013-8-15
 *      Author: Administrator
 */

#ifndef VARM_VDSP_H_
#define VARM_VDSP_H_


#include <ti/ipc/MessageQ.h>

#define Server_MsgQueNameFmt "Server_MsgQ_%s" /* %s = remote proc name */

#define Server_Cmd_START        1
#define Server_Cmd_STOP         2
#define Server_Cmd_PROCESS      3

typedef struct {
    MessageQ_MsgHeader  reserved;

    UInt32      cmd;
    UInt32      slaveBufBase;   /* data buffer (slave virtual address) */
    UInt32      slaveBufCount;  /* number of data elements in buffer */
    UInt32      shift;          /* scaling factor (power of 2) */
} Server_Msg;

#endif /* VARM_VDSP_H_ */
