/*
 * File Name: edma3_interface.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-06-14 07:26:33 week:0
*/
#include <xdc/std.h>
#include <ti/sysbios/hal/Hwi.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include "soc_OMAPL138.h"
#include "hw_psc_OMAPL138.h"
#include "interrupt.h"
#include "evmOMAPL138.h"
#include "edma.h"
#include "psc.h"
#include "edma3_interface.h"

/*
 * use sequence								call function
2. fill src data
3. setup HWI Interrupt						init_edma
4. request channel							request_channel
1. set complete callback 					set_callback
5. fill param								set_paraRamSet
6. trigger  dma transfer					trigger_dma 
7. wait INT flag 
*/
typedef struct _edma3_control{
    unsigned int chType    ;//DMA or QDMA 
    unsigned int chNum     ;//DMA Channel Number,from 0 to 64
    unsigned int tccNum    ;//Transmit Complete Code,this 6 bit code will set relevant bit for Chain Enable Register (CER[tcc]) or IPR[tcc]
    unsigned int edmaTC    ;
    unsigned int syncType  ;//Transmit type ,A or AB mode
    unsigned int trigMode  ;//Trig Mode ,Manual,Chain,Event
    unsigned int paRAMId   ;
    unsigned int evtQ      ;
    unsigned int opt       ;
	unsigned char *src_addr;
	unsigned char *dst_addr;
	EDMA3CCPaRAMEntry paramSet;
}edma3_control;

static void edma3_CC_0_ComplIsr(UArg arg);
static void edma3_CC_0_ErrIsr(UArg arg);
static edma3_cc_callback cbFxn[EDMA3_NUM_TCC];//callbacks

int irqRaised = 0;

static edma3_control edma_ctrl[3]={
	[0]={//DMA channel 0
		.chType     = EDMA3_CHANNEL_TYPE_DMA,
    	.chNum      = 0,
    	.tccNum     = 0,
    	.edmaTC     = 0,
    	.syncType     = EDMA3_SYNC_AB,
    	.trigMode     = EDMA3_TRIG_MODE_MANUAL,
    	.evtQ           = 0,/* Event Queue used                     */
    	.opt            = 0,
	},
	[1]={//DMA
		.chType     = EDMA3_CHANNEL_TYPE_DMA,
    	.chNum      = 1,
    	.tccNum     = 1,
    	.edmaTC     = 0,
    	.syncType     = EDMA3_SYNC_AB,
    	.trigMode     = EDMA3_TRIG_MODE_MANUAL,
    	.evtQ           = 0,/* Event Queue used                     */
    	.opt            = 0,
	},
	[2]={//QDMA
		.chType     = EDMA3_CHANNEL_TYPE_QDMA,
    	.chNum      = 0,
    	.tccNum     = 0,
    	.edmaTC     = 0,
    	.syncType     = EDMA3_SYNC_A,
    	.trigMode     = EDMA3_TRIG_MODE_QDMA,
    	.paRAMId        = 32u,
    	.evtQ           = 0,/* Event Queue used                     */
    	.opt            = 0,
	}
};

/* Callback function */
void default_callback(unsigned int tccNum, unsigned int status, void *appData)
{
    (void)tccNum;
    (void)appData;

    if(EDMA3_XFER_COMPLETE == status)
    {
            /* Transfer completed successfully */
            irqRaised = 1;
    }

    else if(EDMA3_CC_DMA_EVT_MISS == status)
    {
            /* Transfer resulted in DMA event miss error. */
            irqRaised = -1;
    }

    else if(EDMA3_CC_QDMA_EVT_MISS == status)
    {
            /* Transfer resulted in QDMA event miss error. */
            irqRaised = -2;
    }
}

/****************************************************************************/
/*                                                                          */
/*                       DMA Interrupt             							*/
/*                                                                          */
/****************************************************************************/
void edma3_CC_0_ComplIsr(UArg arg)
{
    volatile unsigned int pendingIrqs;
    volatile unsigned int isIPR = 0;

    unsigned int indexl;
    unsigned int Cnt = 0;
    indexl = 1u;
#ifdef _TMS320C6X
    IntEventClear(SYS_INT_EDMA3_0_CC0_INT1);
#else
    IntSystemStatusClear(SYS_INT_CCINT0);
#endif
    isIPR = EDMA3GetIntrStatus(SOC_EDMA30CC_0_REGS);
    if(isIPR)
    {
        while ((Cnt < EDMA3CC_COMPL_HANDLER_RETRY_COUNT)&& (indexl != 0u))
        {
			indexl = 0u;
			pendingIrqs = EDMA3GetIntrStatus(SOC_EDMA30CC_0_REGS);
			while (pendingIrqs)
			{
				if(TRUE == (pendingIrqs & 1u))
				{
					/**
					 * If the user has not given any callback function
					 * while requesting the TCC, its TCC specific bit
					 * in the IPR register will NOT be cleared.
					 */
					/* Here write to ICR to clear the corresponding IPR bits */
					EDMA3ClrIntr(SOC_EDMA30CC_0_REGS, indexl);
					EDMA3ClrEvt(SOC_EDMA30CC_0_REGS, indexl);
					(*cbFxn[indexl])(indexl, EDMA3_XFER_COMPLETE, NULL);
				}
				++indexl;
				pendingIrqs >>= 1u;
			}
            Cnt++;
        }
    }
}
/**
 * edma3CCErrIsr
 * \brief   Interrupt ISR for Channel controller error.
 *
 * \note    This function first disables its own interrupt to make it non-
 *          entrant.
 *
 * \return  None.
 */
void edma3_CC_0_ErrIsr(UArg arg)
{
    volatile unsigned int pendingIrqs;
    unsigned int Cnt = 0u;
    unsigned int index;
    unsigned int evtqueNum = 0;  /* Event Queue Num */

    pendingIrqs = 0u;
    index = 1u;

#ifdef _TMS320C6X
    IntEventClear(SYS_INT_EDMA3_0_CC0_ERRINT);
#else
    IntSystemStatusClear(SYS_INT_CCERRINT);
#endif

    if((EDMA3GetErrIntrStatus(SOC_EDMA30CC_0_REGS) != 0 )
        || (EDMA3QdmaGetErrIntrStatus(SOC_EDMA30CC_0_REGS) != 0)
        || (EDMA3GetCCErrStatus(SOC_EDMA30CC_0_REGS) != 0))
    {
        /* Loop for EDMA3CC_ERR_HANDLER_RETRY_COUNT number of time,
         * breaks when no pending interrupt is found
         */
        while ((Cnt < EDMA3CC_ERR_HANDLER_RETRY_COUNT)
                    && (index != 0u))
        {
            index = 0u;
            pendingIrqs = EDMA3GetErrIntrStatus(SOC_EDMA30CC_0_REGS);
            while (pendingIrqs)
            {
            /*Process all the pending interrupts*/
            if(TRUE == (pendingIrqs & 1u))
            {
                    /* Write to EMCR to clear the corresponding EMR bits.
                     */
                    /*Clear any SER*/
                EDMA3ClrMissEvt(SOC_EDMA30CC_0_REGS, index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = EDMA3QdmaGetErrIntrStatus(SOC_EDMA30CC_0_REGS);
            while (pendingIrqs)
            {
                /*Process all the pending interrupts*/
                if(TRUE == (pendingIrqs & 1u))
                {
            /* Here write to QEMCR to clear the corresponding QEMR bits*/
                    /*Clear any QSER*/
                    EDMA3QdmaClrMissEvt(SOC_EDMA30CC_0_REGS, index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = EDMA3GetCCErrStatus(SOC_EDMA30CC_0_REGS);
            if (pendingIrqs != 0u)
            {
            /* Process all the pending CC error interrupts. */
            /* Queue threshold error for different event queues.*/
            for (evtqueNum = 0u; evtqueNum < SOC_EDMA3_NUM_EVQUE; evtqueNum++)
                {
                if((pendingIrqs & (1u << evtqueNum)) != 0u)
                {
                        /* Clear the error interrupt. */
                        EDMA3ClrCCErr(SOC_EDMA30CC_0_REGS, (1u << evtqueNum));
                    }
                }

            /* Transfer completion code error. */
            if ((pendingIrqs & (1 << EDMA3CC_CCERR_TCCERR_SHIFT)) != 0u)
            {
                EDMA3ClrCCErr(SOC_EDMA30CC_0_REGS,
                                      (0x01u << EDMA3CC_CCERR_TCCERR_SHIFT));
            }
                ++index;
            }
            Cnt++;
        }
    }
}
/**
*************************************************************************************
*  @brief		set up a paramSet for transfer 
*  @param[in]	cc_id  edma3 channel control id,0 is for cc0,1 is for cc1
*  @param[in]	ctrl_id  edma3_control array id,
*  @param[in]	unsigned char *srcBuff	srcBuff addr
*  @param[in]	unsigned char *dstBuff	dstBuff addr
*  @param[in]	int syncType A or AB mode;0 A mode;1 AB mode 
*  @param[out]
*  @return		return numbers of need trigger.
*  @note		you should set edma3_control array first before use this
*  fuction.
*				in this function,interrupt only once when transfer
*				complete,because TaranMode set 0;
*				Tansmode default is AB Mode 
**************************************************************************************
*/
int set_paraRamSet(int cc_id,int ctrl_id,unsigned char *srcBuff,unsigned char *dstBuff,int acnt,int bcnt,int ccnt,int syncType){
	int cc;
	int numenabled;
	cc=cc_id==0?SOC_EDMA30CC_0_REGS:SOC_EDMA31CC_0_REGS;
	if(ctrl_id>2||ctrl_id<0)
		return FALSE;
	/* Fill the PaRAM Set with transfer specific information */
	edma_ctrl[ctrl_id].paramSet.srcAddr    = (unsigned int)(srcBuff);
	edma_ctrl[ctrl_id].paramSet.destAddr   = (unsigned int)(dstBuff);
	edma_ctrl[ctrl_id].syncType   = syncType;
	
	edma_ctrl[ctrl_id].paramSet.aCnt = (unsigned short)acnt;
	edma_ctrl[ctrl_id].paramSet.bCnt = (unsigned short)bcnt;
	edma_ctrl[ctrl_id].paramSet.cCnt = (unsigned short)ccnt;
	/* Setting up the SRC/DES Index */
	edma_ctrl[ctrl_id].paramSet.srcBIdx = (short)acnt;
	edma_ctrl[ctrl_id].paramSet.destBIdx = (short)acnt;
	if (edma_ctrl[ctrl_id].syncType == EDMA3_SYNC_A){
	    /* A Sync Transfer Mode */
		edma_ctrl[ctrl_id].paramSet.srcCIdx = (short)acnt;
		edma_ctrl[ctrl_id].paramSet.destCIdx = (short)acnt;
	}else{
		/* AB Sync Transfer Mode */
		edma_ctrl[ctrl_id].paramSet.srcCIdx = ((short)acnt * (short)bcnt);
	    edma_ctrl[ctrl_id].paramSet.destCIdx = ((short)acnt * (short)bcnt);
	}
	edma_ctrl[ctrl_id].paramSet.linkAddr = (unsigned short)0xFFFFu;
	edma_ctrl[ctrl_id].paramSet.bCntReload = (unsigned short)0u;
	edma_ctrl[ctrl_id].paramSet.opt = 0u;
	/* Src & Dest are in INCR modes */
	edma_ctrl[ctrl_id].paramSet.opt &= 0xFFFFFFFCu;
	/* Program the TCC */
	edma_ctrl[ctrl_id].paramSet.opt |= ((edma_ctrl[ctrl_id].tccNum << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);
	
	//if you want enable Intermediate interrutp open down
	//code,otherwise disable Intermediate interrupt .
	/* Enable Intermediate & Final transfer completion interrupt */
	//edma_ctrl[ctrl_id].paramSet.opt |= (1 << EDMA3CC_OPT_ITCINTEN_SHIFT);
	edma_ctrl[ctrl_id].paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);
	
	if (edma_ctrl[ctrl_id].syncType == EDMA3_SYNC_A){
	    edma_ctrl[ctrl_id].paramSet.opt &= 0xFFFFFFFBu;
    }else{
	    /* AB Sync Transfer Mode */
	    edma_ctrl[ctrl_id].paramSet.opt |= (1 << EDMA3CC_OPT_SYNCDIM_SHIFT);
	}
	
	/* Now, write the PaRAM Set. */
	EDMA3SetPaRAM(cc, edma_ctrl[ctrl_id].chNum, &edma_ctrl[ctrl_id].paramSet);
	
    /*Need to activate next param*/
    if (syncType == EDMA3_SYNC_A){
        numenabled = bcnt * ccnt;
    }else{
        /* AB Sync Transfer Mode */
         numenabled = ccnt;
     }
	return numenabled;
}

/**
*************************************************************************************
*  @brief		Trige a DMA transfer by Manual  
*  @param[in]	cc_id  edma3 channel control id,0 is for cc0,1 is for cc1
*  @param[in]	chNum  edma3 channel Number
*  @param[out]
*  @return   success 1;fail 0
*	 @note
**************************************************************************************
*/
int trigger_dma(int cc_id,int chNum){
	int cc;
	if(cc_id>2||cc_id<0)
		return FALSE;
	irqRaised=0;
	cc=cc_id==0?SOC_EDMA30CC_0_REGS:SOC_EDMA31CC_0_REGS;
	return EDMA3EnableTransfer(cc,chNum,EDMA3_TRIG_MODE_MANUAL);
}
int wait_complete(){
	while(irqRaised==0){
		printf("watitting\r\n");
	};
	return 1;
}

void set_callback(int chNum,edma3_cc_callback callback){
	if(callback==NULL)
		cbFxn[chNum]=default_callback;
	else
		cbFxn[chNum]=callback;
}
/**
*************************************************************************************
*  @brief		init edma3,setup interrupt 
*  @param[out]	callback callback function when complete 
*  @return		no 
*  @note		in this function ,only use CC0 Interrupt.
*				CC0 Complete Event is 8,cpuInt use 8
*				Error Event is 56,cpuInt use 9
**************************************************************************************
*/

void init_edma(void){
	Hwi_Params hwi_para;
    Error_Block eb;
	int evtQ=0;//use eventQ 0

    Error_init(&eb);
    /* Enabling the PSC for EDMA3CC_0.*/
    PSCModuleControl(SOC_PSC_0_REGS, 0, 0, PSC_MDCTL_NEXT_ENABLE);
    /* Enabling the PSC for EDMA3TC_0.*/
    PSCModuleControl(SOC_PSC_0_REGS, 1, 0, PSC_MDCTL_NEXT_ENABLE);

	Hwi_enable();
    EDMA3Init(SOC_EDMA30CC_0_REGS, evtQ);

    Hwi_Params_init(&hwi_para);
    hwi_para.enableInt = 1;
	hwi_para.eventId = SYS_INT_EDMA3_0_CC0_INT1;//EDMA3 CC0 Complete Interrupt Event 
	hwi_para.maskSetting = Hwi_MaskingOption_SELF;
    Hwi_create(8,edma3_CC_0_ComplIsr,&hwi_para,&eb);
	hwi_para.eventId = SYS_INT_EDMA3_0_CC0_ERRINT;//EDMA3 CC0 ERR Interrutp Event 
    Hwi_create(9,edma3_CC_0_ErrIsr,&hwi_para,&eb);
}
void deinit_edma(){
	EDMA3Deinit(SOC_EDMA30CC_0_REGS, 0);
}

/**
*************************************************************************************
*  @brief request a channel for transmit 
*  @param[in]	cc_id	EDMA3 Channel Control,0 is for CC0,1 is for CC1
*  @param[in]   ctrl_id edma3 control index;0 is for edma_crtl[0],1 is
*  for edma_ctrl[1]
*  @param[out]
*  @return  1 success;0 fail
*  @note
**************************************************************************************
*/
int request_channel(int cc_id,int ctrl_id){
	int cc;

	cc=cc_id==0?SOC_EDMA30CC_0_REGS:SOC_EDMA31CC_0_REGS;
	if(ctrl_id>2||ctrl_id<0)
		return FALSE;
	return EDMA3RequestChannel(cc,edma_ctrl[ctrl_id].chType,edma_ctrl[ctrl_id].chNum,edma_ctrl[ctrl_id].tccNum,edma_ctrl[ctrl_id].evtQ);
}
// vim:tw=72 
