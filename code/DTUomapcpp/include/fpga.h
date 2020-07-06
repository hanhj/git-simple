/*
 * fpga.h
 *
 *  Created on: 2013-9-22
 *      Author: Administrator
 */

#ifndef FPGA_H_
#define FPGA_H_
#include "types.h"
#include "datatype.h"
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
#define GLALARM    1 //过流故障
#define JDALARM    2 //接地故障
#define YXALARM    3 //越线故障
#define ALARMRESET 0 //故障复归
///////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// Register Structure & Defines
//-----------------------------------------------------------------------------
typedef struct
{
   volatile uint32_t REVID;               // 0x0000
   volatile uint32_t AWCC;                // 0x0004
   volatile uint32_t SDCR;                // 0x0008
   volatile uint32_t SDRCR;               // 0x000C
   volatile uint32_t CE2CFG;              // 0x0010
   volatile uint32_t CE3CFG;              // 0x0014
   volatile uint32_t CE4CFG;              // 0x0018
   volatile uint32_t CE5CFG;              // 0x001C
   volatile uint32_t SDTIMR;              // 0x0020
   volatile uint32_t RSVD0[6];            // 0x0024
   volatile uint32_t SDSRETR;             // 0x003C
   volatile uint32_t INTRAW;              // 0x0040
   volatile uint32_t INTMASK;             // 0x0044
   volatile uint32_t INTMASKSET;          // 0x0048
   volatile uint32_t INTMASKCLR;          // 0x004C
   volatile uint32_t RSVD1[4];            // 0x0050
   volatile uint32_t NANDFCR;             // 0x0060
   volatile uint32_t NANDFSR;             // 0x0064
   volatile uint32_t PMCR;                // 0x0068
   volatile uint32_t NANDF1ECC;           // 0x0070
   volatile uint32_t NANDF2ECC;           // 0x0074
   volatile uint32_t NANDF3ECC;           // 0x0078
   volatile uint32_t NANDF4ECC;           // 0x007C
   volatile uint32_t RSVD2[15];           // 0x0080
   volatile uint32_t NAND4BITECCLOAD;     // 0x00BC
   volatile uint32_t NAND4BITECC1;        // 0x00C0
   volatile uint32_t NAND4BITECC2;        // 0x00C4
   volatile uint32_t NAND4BITECC3;        // 0x00C8
   volatile uint32_t NAND4BITECC4;        // 0x00CC
   volatile uint32_t NANDERRADDR1;        // 0x00D0
   volatile uint32_t NANDERRADDR2;        // 0x00D4
   volatile uint32_t NANDERRVAL1;         // 0x00D8
   volatile uint32_t NANDERRVAL2;         // 0x00DC
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
// emifa nor pinmux defines.
#define PINMUX_EMIFA_NOR_REG_0      (5)
#define PINMUX_EMIFA_NOR_MASK_0     (0xFF000000)
#define PINMUX_EMIFA_NOR_VAL_0      (0x11000000)
#define PINMUX_EMIFA_NOR_REG_1      (6)
#define PINMUX_EMIFA_NOR_MASK_1     (0x0F00000F)
#define PINMUX_EMIFA_NOR_VAL_1      (0x01000001)
#define PINMUX_EMIFA_NOR_REG_2      (7)
#define PINMUX_EMIFA_NOR_MASK_2     (0x00FF000F)
#define PINMUX_EMIFA_NOR_VAL_2      (0x00110001)
#define PINMUX_EMIFA_NOR_REG_3      (8)
#define PINMUX_EMIFA_NOR_MASK_3     (0xFFFFFFFF)
#define PINMUX_EMIFA_NOR_VAL_3      (0x11111111)
#define PINMUX_EMIFA_NOR_REG_4      (9)
#define PINMUX_EMIFA_NOR_MASK_4     (0xFFFFFFFF)
#define PINMUX_EMIFA_NOR_VAL_4      (0x11111111)
#define PINMUX_EMIFA_NOR_REG_5      (10)
#define PINMUX_EMIFA_NOR_MASK_5     (0xFFFFFFFF)
#define PINMUX_EMIFA_NOR_VAL_5      (0x11111111)
#define PINMUX_EMIFA_NOR_REG_6      (11)
#define PINMUX_EMIFA_NOR_MASK_6     (0xFFFFFFFF)
#define PINMUX_EMIFA_NOR_VAL_6      (0x11111111)
#define PINMUX_EMIFA_NOR_REG_7      (12)
#define PINMUX_EMIFA_NOR_MASK_7     (0xFFFFFFFF)
#define PINMUX_EMIFA_NOR_VAL_7      (0x11111111)

#define CLOSEBREAKER 1
#define OPENBREAKER  0
#define YXBOARD 0x01
#define YCBOARD 0x02
#define IGBTBOARD 0x03
#define SLAVE_BOARD_NUM 10//5
#define PORTRATIO	10.0
#define U300RATIO	0.0002267663 //0.005575142
#define I10RATIO	0.0000192
#define U1VRATIO	0.000094259 //0.00000302355064
#define I1ARATIO	0.0003379//0.00000384
#define I100RATIO	0.000192
#define AngleCos	0.99996192306417128873735516482698;//0.99987663248166059863890712773125;//0.99994516936551213198209710144628;
#define AngleSin	0.00087266451523514954330458929907378;//0.015707317311820675753295353309907;//0.010471784116245793463225306967096;
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

typedef struct SLAVEFPGABITID{
	uint16 revl:4;//未定义
	uint16 VerL:4;//版本号低四
	uint16 VerH:4;//版本号高四
	uint16 rev2:4;
}SLAVEFPGABITID_t;
typedef union SLAVEFPGAID{
	struct SLAVEFPGABITID bit;
	uint16 all;
}SLAVEFPGAID_t;
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
	union FPGAID VER;          //R
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
	uint16 hesj:1;//合继电器烧结状态 0烧结1 正常
	uint16 fensj:1;//分继电器烧结状态 0烧结1 正常
	uint16 gzfg:1;//故障复归；
	uint16 hexh:1;//合继电器无压吸合 0故障1正常
	uint16 fenxh:1;//分继电器无压吸合 0故障1正常
	uint16 fyjd:1;//远方接地遥信，1就地，0远方
	uint16 yx15:1;
	uint16 yx16:1;
}YXBIT_t;

typedef union YXDATA{
	struct YXBIT bit;
	uint16 all;
}YXDATA_t;
#define HZMASK 0x0001
#define TZMASK 0x0002
#define QDMASK 0x0004
#define MOTORMASK 0x0008
#define YUEXIANMASK 0x0010
#define GUOLIUMASK 0x0020
#define JIEDIMASK 0x0040
typedef struct YKBIT{
	uint16 hz:1;//下降沿有效
	uint16 tz:1;//下降沿有效
	uint16 qd:1;//下降沿有效
	uint16 motor:1;//0打开1关闭
	uint16 yuexianled:1;//0打开1关闭
	uint16 guoliuled:1;//0打开1关闭
	uint16 jiediled:1;//0打开1关闭
	uint16 rsvl1:5;
	uint16 cmd:2;
	uint16 rsvl2:2;
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

typedef struct CARDMD{
	uint16 MODE: 2;//功能 1遥控遥信板， 2遥测，3 IGBT板；
	uint16 IdNum:2;// 0,1,2,3
	uint16 rsv:12;
}CARDMD_t;
typedef union CARDMD_U{
	CARDMD_t bit;
	uint16 all;
}CARDMD_UNIOM;

typedef struct SLAVE{
	SLAVEFPGAID_t VER;
	union YXDATA  YX1;
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
	CARDMD_UNIOM CARD_MODE;//001遥测板，010 遥信遥控板；
	uint16 YX_TIME;
	int16  YC;
	uint16 YC_PERIOD;
}SLAVE_t;

typedef struct SLAVEDATA{
	uint8 SlotNum;//槽号 ，表明FPGA所在的板槽号 0 1 2 3 4
	uint8 Statu; //是否存在的状态；
	uint8 IdNum;//FPGA板号；如果不是FPGA板的话 和SlotNum必须一样的；
}SLAVEDATA_T;
typedef struct FPGACFG{
	uint8 YcCardSize;//遥测板数量；
	SLAVEDATA_T YcCard[SLAVE_BOARD_NUM];//遥测板对应FPGA子板号；从0 开始
	uint8 InOutCardSize;//遥信板数量
	SLAVEDATA_T InOutCard[SLAVE_BOARD_NUM];//遥信板对应的fpga子板号，从0开始
	uint8 InitFlag;
}FPGACFG_T;
typedef struct YKSTATA{
	uint8 DeviceType;
	uint8 SlotNum;
	uint8 YkNum;
	uint8 OutType;
	uint16 Tqd;
	uint16 Tpulse;
	uint8 YkQdFlag;
	uint8 YkTrFlag;
	uint32 cnt;
	int32 Tdiff;
}YKSTATA_T;
typedef struct BREAKOUTCHECK{
	uint16 OperateType;
	uint16 OperateFlag;
	uint32 CheckTd;
	uint32 CheckCnt;
}BREAKOUTCHECK_t;
extern BREAKOUTCHECK_t BreakOutCheck[MAX_LINE_NUM];
//extern YKSTATA_T YkSt;
extern FPGACFG_T FpgaCfg;
extern const uint16 FpgaIrqMask[8];
extern struct SLAVE FpgaSlave[SLAVE_BOARD_NUM];
extern uint16 GetId(void);
uint16 GetSlaveID(uint8 num);
extern uint32_t FpgaInit(void);
extern void FpgaCfgInit(void);
//extern uint16 FpgaReadData(uint8 SlotNum,uint16 *ExpDataAdd ,uint16 ExpDataLen);
//extern uint16 FpgaWriteData(uint8 SlotNum,uint16 *DataAdd ,uint16 DataLen);
extern int16 FpgaOut(uint8 SlotNum,uint8 YkNum,uint8 OutType,uint16 Td);
extern void FpgaCheck(void);
extern void SetParaMask(uint16 SlotNum,uint16 SetType);
//extern void FpgaIrqCheck(void);
//extern void YxTimeSave(uint16 Data);
extern int16 YkQd(uint8 SlotNum,uint8 YkNum,uint8 OutType,uint16 Td);
extern int16 YkTr(uint8 SlotNum,uint8 YkNum,uint8 OutType,uint16 Td);
extern int16 MotorControl(uint8 SlotNum,uint8 YkNum,uint8 OutType);
extern int16 AlarmLed(uint16 IdNum,uint16 AlarmName,uint16 LedType);
extern void YkMaskSet(uint16 SlotNum,uint16 Mask,uint16 SetType);
extern void YcRatioInit(uint8 line);
extern void YcRatioRead(void);
extern void YcRatioBoardRead(uint16 bordnum);
extern void XiShuChange(void);
#endif /* FPGA_H_ */
