/*
 * RelayOut.h
 *
 *  Created on: 2015-7-27
 *      Author: Administrator
 */

#ifndef INOUT_H_
#define INOUT_H_
typedef enum RelayNum{
JDQ1 = 0,
JDQ2,
JDQ3,
JDQ4,
JDQ5,
JDQ6,
JDQ7,
JDQ8,
JDQ9,
JDQ10,
JDQ11,
JDQ12,
JDQ13,
JDQ14,
JDQ15,
JDQ16,
JDQ17,
JDQ18,
JDQ19,
JDQ20,
JDQ21,
JDQ22,
JDQ23,
JDQ24,
JDQ25,
JDQ26,
JDQ27,
JDQ28,
JDQ29,
JDQ30,
JDQ31,
JDQ32,
JDQ33,
JDQ34,
JDQ35,
JDQ36,
JDQ37,
JDQ38,
JDQ39,
JDQ40,
JDQ41,
JDQ42,
JDQ43,
JDQ44,
JDQ45,
JDQ46,
JDQ47,
JDQ48,
}RelayNum;

#define MAX_OUT_NUM (JDQ48+1)
#define PULSE 0
#define LEV 1
typedef struct outdata{
	Uint16 StartFlag;
	Uint8 LevelType;//出口类型；PULSE LEV
	//Uint8 Lock;
	Uint32 Td;//毫秒
	Uint64 TimeCnt;
}OUTDATAREG;
typedef struct RELREG{
	OUTDATAREG RelayReg[MAX_OUT_NUM];
	Uint16 BitStatu[5];
}RELREG;
extern RELREG RelayStatu;
typedef struct RELAYSTATU{
	Uint16 KR1:1;
	Uint16 KR2:1;
	Uint16 KR3:1;
	Uint16 KR4:1;
	Uint16 KR5:1;
	Uint16 KR6:1;
	Uint16 KR7:1;
	Uint16 KR8:1;
	Uint16 KR9:1;
	Uint16 KR10:1;
	Uint16 KR11:1;
	Uint16 KR12:1;
	Uint16 KR13:1;
	Uint16 KR14:1;
	Uint16 KR15:1;
	Uint16 KR16:1;
} RELAYSTATU;
typedef union RELAYSTREG {
	Uint16 all;
	RELAYSTATU  bit;
}RELAYSTREGU;
extern RELAYSTREGU RelaySt;
#define SET 1
#define CLR 0
#define DelaySet 0x0e
#define DelayClr 0xff

extern void RelayReturn(Uint64 SysMs);
extern int16 RelayOut(Uint16 RelayNum,Uint16 OutType);
extern Uint16 YKREG[5];
extern Uint16 YKMEMREG[5];
#endif /* RELAYOUT_H_ */
