/*
 * File Name: inc/edma3_interface.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-06-14 10:41:30 week:0
*/
#ifndef _edma3_interface_h
#define _edma3_interface_h
#ifdef __cplusplus
extern "C"{
#endif

typedef void (*edma3_cc_callback)(unsigned int tcc,unsigned int status,void *appData);

void init_edma(void);
int request_channel(int cc_id,int ctrl_id);
void set_callback(int chNum,edma3_cc_callback fun);
int set_paraRamSet(int cc_id,int ctrl_id,unsigned char *srcBuff,unsigned char
		*dstBuff,int acnt,int bcnt,int ccnt,int syncType);
int trigger_dma(int cc_id,int chNum);
int wait_complete();
void deinit_edma(void);
#ifdef __cplusplus
}
#endif
extern int irqRaised;
#endif //_edma3_interface_h
// vim:tw=72 
