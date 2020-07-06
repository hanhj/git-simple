/*
 * fpga.h
 *
 *  Created on: 2013-9-22
 *      Author: Administrator
 */

#ifndef FPGA_H_
#define FPGA_H_
#include "types.h"
#define SLAVE_NUM 8




//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

// emifa nand pinmux defines.
#define PINMUX_EMIFA_NAND_REG_0        (7)
#define PINMUX_EMIFA_NAND_MASK_0       (0x00FF0FF0)
#define PINMUX_EMIFA_NAND_VAL_0        (0x00110110)
#define PINMUX_EMIFA_NAND_REG_1        (9)
#define PINMUX_EMIFA_NAND_MASK_1       (0xFFFFFFFF)
#define PINMUX_EMIFA_NAND_VAL_1        (0x11111111)
#define PINMUX_EMIFA_NAND_REG_2        (12)
#define PINMUX_EMIFA_NAND_MASK_2       (0x0FF00000)
#define PINMUX_EMIFA_NAND_VAL_2        (0x01100000)

#define NAND_SPARE_BYTE_SIZE           (64)
#define TIMEOUT_NAND                   (750) // in us.

// nand flash
#define NAND_FLASH_BASE                (0x62000000)
#define NAND_OFFSET_DATA               (0x00)
#define NAND_OFFSET_ALE                (0x08)
#define NAND_OFFSET_CLE                (0x10)

// nand flash command defines.
#define NAND_CMD_READ                  (0x00)
#define NAND_CMD_PAGE_PROG_CONFIRM     (0x10)
#define NAND_CMD_READ_CONFIRM          (0x30)
#define NAND_CMD_ERASE                 (0x60)
#define NAND_CMD_READ_STAT             (0x70)
#define NAND_CMD_PAGE_PROG             (0x80)
#define NAND_CMD_READ_ID               (0x90)
#define NAND_CMD_ERASE_CONFIRM         (0xD0)
#define NAND_CMD_RESET                 (0xFF)

// nand flash status reg defines.
#define NAND_STAT_READY                (0x40)
#define NAND_STAT_FAIL                 (0x01)

// nand flash address defines.
#define NAND_ADDR_READ_ID              (0x00)
#define NAND_ADDR_ONFI_ID              (0x20)

// nand flash addressing cycles.
#define ADDR_CYCLES_3                  (3)
#define ADDR_CYCLES_5                  (5)

// expected device id for Micron MT29F4F08AAC.
#define EXPECTED_MFG_ID                (0x2C)
#define EXPECTED_DEV_ID                (0xDC)

///////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
typedef struct
{
   volatile uint32 REVID;               // 0x0000
   volatile uint32 AWCC;                // 0x0004
   volatile uint32 SDCR;                // 0x0008
   volatile uint32 SDRCR;               // 0x000C
   volatile uint32 CE2CFG;              // 0x0010
   volatile uint32 CE3CFG;              // 0x0014
   volatile uint32 CE4CFG;              // 0x0018
   volatile uint32 CE5CFG;              // 0x001C
   volatile uint32 SDTIMR;              // 0x0020
   volatile uint32 RSVD0[6];            // 0x0024
   volatile uint32 SDSRETR;             // 0x003C
   volatile uint32 INTRAW;              // 0x0040
   volatile uint32 INTMASK;             // 0x0044
   volatile uint32 INTMASKSET;          // 0x0048
   volatile uint32 INTMASKCLR;          // 0x004C
   volatile uint32 RSVD1[4];            // 0x0050
   volatile uint32 NANDFCR;             // 0x0060
   volatile uint32 NANDFSR;             // 0x0064
   volatile uint32 PMCR;                // 0x0068
   volatile uint32 NANDF1ECC;           // 0x0070
   volatile uint32 NANDF2ECC;           // 0x0074
   volatile uint32 NANDF3ECC;           // 0x0078
   volatile uint32 NANDF4ECC;           // 0x007C
   volatile uint32 RSVD2[15];           // 0x0080
   volatile uint32 NAND4BITECCLOAD;     // 0x00BC
   volatile uint32 NAND4BITECC1;        // 0x00C0
   volatile uint32 NAND4BITECC2;        // 0x00C4
   volatile uint32 NAND4BITECC3;        // 0x00C8
   volatile uint32 NAND4BITECC4;        // 0x00CC
   volatile uint32 NANDERRADDR1;        // 0x00D0
   volatile uint32 NANDERRADDR2;        // 0x00D4
   volatile uint32 NANDERRVAL1;         // 0x00D8
   volatile uint32 NANDERRVAL2;         // 0x00DC
} emifa_regs_t;

#define EMIFA        ((emifa_regs_t *)EMIFA_REG_BASE)

// bitmask defines for CEnCFG.
#define SS                 (0x80000000)   // bit 31
#define EW                 (0x80000000)   // bit 30

#define ASIZE              (0x00000001)   // bits 0-1

// bitmask defines for NANDFCR.
#define FOURBITECC_START   (0x00001000)   // bit 12
#define CS5NAND            (0x00000008)   // bit 3
#define CS4NAND            (0x00000004)   // bit 2
#define CS3NAND            (0x00000002)   // bit 1
#define CS2NAND            (0x00000001)   // bit 0
#define ECCSEL_MASK        (0x00000030)   // bits 4-5
#define ECCSEL_CS2		   (0x00000000)
#define ECCSEL_CS3         (0x00000010)   // bits 4
#define ECCSEL_CS4         (0x00000020)   // bits 5
#define ECCSEL_CS5         (0x00000030)   // bits 4-5

// bitmask defines for NANDFSR.
#define WAITST             (0x00000001)   // bit 0


#define CLOSEBREAKER 1
#define OPENBREAKER  0
#define YXBOARD 0x01
#define YCBOARD 0x02
#define SLAVEFPGANUM 5
typedef struct IRQ{
	uint16 receive:1;
	uint16 send: 1;
	uint16 rsvd123:2;
	uint16 fpga0:1;
	uint16 fpga1:1;
	uint16 fpga2:1;
	uint16 fpga3:1;
	uint16 fpga4:1;
	uint16 fpga5:1;
	uint16 fpag6:1;
	uint16 fpag7:1;
	uint16 rsvl1215:4;
 }IRQ_t;
typedef union IRQDATA{
	struct IRQ bit;
	uint16 all;
}IRQDATA_t;

typedef struct FPGABITID{
	uint16 day: 5;
	uint16 month:4;
	uint16 year:5;
	uint16 rsvl:1;
}FPGABITID_t;
typedef union FPGAID{
	struct FPGABITID bit;
	uint16 all;
}FPGAID_t;


typedef struct MIRQE{
	uint16 dataout:1;
	uint16 nosend: 1;
	uint16 timeout:1;
	uint16 nc:1;
	uint16 fpga1:1;
	uint16 fpga2:1;
	uint16 fpga3:1;
	uint16 fpga4:1;
	uint16 fpga5:1;
	uint16 rsvl:7;
 }MIRQE_t;
typedef union MIRQEU{
	struct MIRQE bit;
	uint16 all;
}MIRQE_U;

typedef struct HOST{
	union FPGAID ID;          //R
	uint16 DATA_RX;     //R //reserved
	uint16 DATA_TX;     //W //reserved
	union IRQDATA DATA_IRQ;    //R
	int16 TEM;         //R
	uint16 HUMIDITY;    //R
	MIRQE_U M_REG_IRQ_ENABLE;
}HOST_t;

//#pragma DATA_SECTION(".FpgaHostData");
extern struct HOST FpgaHost;

typedef struct YXBIT{
	uint16 yx1:1;
	uint16 yx2:1;
	uint16 yx3:1;
	uint16 yx4:1;
	uint16 yx5:1;
	uint16 yx6:1;
	uint16 yx7:1;
	uint16 yx8:1;
	uint16 yx9:1;
	uint16 yx10:1;
	uint16 yx11:1;
	uint16 yx12:1;
	uint16 yx13:1;
	uint16 yx14:1;
	uint16 yx15:1;
	uint16 yx16:1;
}YXBIT_t;

typedef union YXDATA{
	struct YXBIT bit;
	uint16 all;
}YXDATA_t;
typedef struct YKBIT{
	uint16 hz:1;
	uint16 tz:1;
	uint16 qd:1;
	uint16 rsvl:13;
}YKBIT_t;
typedef union YKCMD{
	struct YKBIT bit;
	uint16 all;
}YKCMD_t;
typedef struct IRQFIFOBIT{
	uint16 dataover:1;
	uint16 timeover:1;
	uint16 rsvl:14;
}IRQFIFOBIT_t;
typedef union FIFOENABLE{
	struct IRQFIFOBIT bit;
	uint16 all;
}FIFOENABLE_t;
typedef struct IRQIIRBIT{
	uint16 yc:1;
	uint16 yx:1;
	uint16 dataover:1;//reserved
	uint16 timeover:1;//reserved
	uint16 rsvl:12;
}IRQIIRBIT_t;
typedef union IRQIIR{
	struct IRQIIRBIT bit;
	uint16 all;
}IRQIIR_t;
typedef struct FIFOISRBIT{
	uint16 sendfifonum: 5;
	uint16 rsvl0:1;
	uint16 receivefifonum:9;
	uint16 sendcml:1;
}FIFOISRBIT_t;
typedef union FIFOISR{
	struct FIFOISRBIT bit;
	uint16 all;
}FIFOISR_t;
typedef struct SLAVE{
	union FPGAID ID;
	union YXDATA YX1;
    union YXDATA  YX2;//reserved
	union YXDATA  YX3;//reserved
	uint16 YK_TIME;
	union YKCMD YK[2];
	//union YKCMD YK2;//reserved
	uint16 DATA_RX;//reserved
	uint16 DATA_TX;//reserved
	union FIFOENABLE IRQ_ENABLE;
	union IRQIIR IRQ_IIR;
	union FIFOISR FIFO_ISR;
	uint16 CARD_MODE;//001遥测板，010 遥信遥控板；
	uint16 YX_TIME;
	int16  YC;
	uint16 YC_PERIOD;
}SLAVE_t;

typedef struct SLAVEDATA{
	uint8 num;
	uint8 ID;
}SLAVEDATA_T;
typedef struct FPGACFG{
	uint8 YcCardSize;//遥测板数量；
	SLAVEDATA_T YcCard[SLAVEFPGANUM];//遥测板对应FPGA子板号；从0 开始
	uint8 YxCardSize;//要信板数量
	SLAVEDATA_T YxCard[SLAVEFPGANUM];//要信板对应的fpga子板号，从0开始
}FPGACFG_T;
extern FPGACFG_T FpgaCfg;

//#pragma DATA_SECTION(".FpgaSlaveData")；
extern struct SLAVE FpgaSlave[SLAVE_NUM];
extern uint16 GetId(void);
uint16 GetSlaveID(uint8 num);
extern uint32 FpgaInit(void);
extern void FpgaCfgInit(void);
extern uint16 FpgaReadData(uint8 FpgaNum,uint16 *ExpDataAdd ,uint16 ExpDataLen);
extern uint16 FpgaWriteData(uint8 FpgaNum,uint16 *DataAdd ,uint16 DataLen);
extern int16 FpgaOut(uint8 FpgaNum,uint8 YkNum,uint8 OutType,uint16 Td);
#endif /* FPGA_H_ */
