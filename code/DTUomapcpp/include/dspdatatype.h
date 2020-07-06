/*
 * dspdatatype.h
 *
 *  Created on: 2013-11-5
 *      Author: Administrator
 */
#include "types.h"
#include "user.h"

#ifndef DSPDATATYPE_H_
#define DSPDATATYPE_H_

#define PI 3.1415926
#define TWO_PI 6.2831852
#define TWOOFTHREE_PI 2.094395

#define _1_KAI_0_HE 1 //终端默认的分合与电平关系
#define _0_KAI_1_HE 0 //终端默认的分合与电平关系
#define _SYSHE_ 0     //在本终端中：硬遥信闭合时有信号，低电平；
#define _SYSFEN_ 1

#define LOGIC_HE   1//主站默认的分合意义
#define LOGIC_FEN  0//主站默认的分合意义

#define SW_YX 1		//软遥信
#define HW_YX 2		//硬遥信

#define NO_YC 0xffff
#define NO_YX 0xffff
#define	NO_YK 0xffff

#define MAX_LINE_NUM 16//回线数

/**********************about yx**************************/
#define IO_BOARD_NUM			4  //最大开入开出板的数量
#define PER_BOARD_YX_PORT_NUM	24 //每块开入开出板 遥信端口数目
#define IO_BOARD1 0
#define IO_BOARD2 1
#define IO_BOARD3 2
#define IO_BOARD4 3

typedef enum YXENUM{
	HE1 = 0,///<断路器1合位
	FEN1,///<断路器1分位
	CN1, ///<断路器1储能位置
	YX14,///<备用遥信4
	YX15,///<备用遥信5变位
	YX16,//<备用遥信6变位
	YKHE1,
	YKFEN1,
	//XJGZ1,//故障指示器指示线路2相间短路故障
	//JDGZ1,//故障指示器指示线路2接地故障
	//SP6GZ1,//SF6气体异常报警信号
	//SP6BS1,//SF6气压闭锁信号
	HE2,
	FEN2,///<断路器1分位
	CN2, ///<断路器1储能位置
	YX24,///<备用遥信4
	YX25,///<备用遥信5变位
	YX26,//<备用遥信6变位
	YKHE2,
	YKFEN2,
	//XJGZ2,//故障指示器指示线路2相间短路故障
	//JDGZ2, //故障指示器指示线路2接地故障
	//SP6GZ2,//SF6气体异常报警信号
	//SP6BS2,//SF6气压闭锁信号
}YXENUM_E; //共计10项

#define PER_LINE_HW_YX_NUM (YKFEN1+1)         	//每回线用户可配置端口的遥信数量
#define HE(n) 	(HE1 + PER_LINE_HW_YX_NUM*(n))	//<断路器1合位
#define FEN(n)	(FEN1 + PER_LINE_HW_YX_NUM*(n))	//<断路器1分位
#define CN(n)	(CN1 + PER_LINE_HW_YX_NUM*(n))	//<断路器1储能位置
#define YX4(n)	(GLD1 + PER_LINE_HW_YX_NUM*(n))	//<断路器1隔离刀闸位置（备用遥信4）
#define YX5(n)	(YX15 + PER_LINE_HW_YX_NUM*(n))	//<备用遥信5变位
#define YX6(n)	(YX16 + PER_LINE_HW_YX_NUM*(n))	//<备用遥信6变位
#define YKHE(n) (YKHE1 + PER_LINE_HW_YX_NUM*(n))
#define YKFEN(n) (YKFEN1 + PER_LINE_HW_YX_NUM*(n))
//#define XJGZ(n)	(XJGZ1 + PER_LINE_HW_YX_NUM*n)	//故障指示器指示线路2相间短路故障
//#define JDGZ(n)	(JDGZ1 + PER_LINE_HW_YX_NUM*n)	//故障指示器指示线路2接地故障
//#define SP6GZ(n)(SP6GZ1 + PER_LINE_HW_YX_NUM*n)	//SF6气体异常报警信号
//#define SP6BS(n)(SP6BS1 + PER_LINE_HW_YX_NUM*n)	//SF6气压闭锁信号
#define PUBLIC_YX1  (YKFEN(MAX_LINE_NUM-1)+1)  //128
#define PUBLIC_YX2  (YKFEN(MAX_LINE_NUM-1)+2)  //129
#define PUBLIC_YX3  (YKFEN(MAX_LINE_NUM-1)+3)  //130
#define PUBLIC_YX4  (YKFEN(MAX_LINE_NUM-1)+4)  //131
#define YFJD (PUBLIC_YX4+1)  					//远方接地位置      //132
#define BAT_LOWV (YFJD+1)						//电池低压   //134
#define BAT_AC_SPL (BAT_LOWV+1)					//直流/交流供电 //134
#define BAT_ANOMALY (BAT_AC_SPL+1)				//电源异常 //136
#define BAT_ACTION (BAT_ANOMALY+1)				//电池活化状态//137
#define FIRST_AC (BAT_ACTION+1)   				//主交流//138
#define SECEND_AC (FIRST_AC+1)    				//备用交流//139
#define DBYX(LineNum) (SECEND_AC+1+(LineNum)) 	//备用交流//140-155


#define DB_YX_NUM			16                                      //终端最大的双点遥信数量
#define PUBLIC_YX_NUM		4
#define FAULT_YX_NUM		28
#define MAX_YX_PORT_NUM  ((IO_BOARD_NUM * PER_BOARD_YX_PORT_NUM) + PUBLIC_YX_NUM + FAULT_YX_NUM)     //终端最大遥信量端口数量
#define MAX_SG_DB_YXPORT_NUM (MAX_YX_PORT_NUM+DB_YX_NUM) //终端最大遥信量端口与虚拟出来的双点遥信端口数目
#define TER_USER_YX_NUM 0                                              //非回线相关的用户可配置端口的遥信数量
#define MAX_USER_SG_YX_NUM   (SECEND_AC+1)  //终端最大的可配置端口的遥信数量
#define MaxUserYxNum    (MAX_USER_SG_YX_NUM+DB_YX_NUM) //终端最大的可配置端口的遥信数量+虚拟的双点遥信


#define MAX_SOE_NUM 200
#define MAX_FAULT_SOE_NUM 10
#define MAX_USER_NUM 3
#define HDYXSOE 	0
#define PROTECTSOE 	1

#define YUANFANG 0
#define JIUDI 1
#define YXSTATUMASK 5


typedef struct yxdata{
	uint16 Id;
	uint16 Port;  //如果是单点 此口为物理口，如果是双点，其实此口为虚拟口
	uint16 ClosePort;
	uint16 OpenPort;
	struct Yx{
		uint8 Data :2;
		uint8 Nature:2;
		uint8 Type:2;//单双点；
		uint8 res:2;
	}Yx;
	uint8 *YxPortP; //端口对应的遥信；原始值
}yxdata_t;

typedef struct _CONFIG_YX{
	uint16 configed_yx[MaxUserYxNum];//被配置上遥测端口的遥信量
	yxdata_t config_data[MaxUserYxNum];//装载的通道编号；如果是0xffff的代表此遥信没有对应的硬件通道；下标为遥信项
	uint8 yx_num;//实际配置的遥信项数；
	uint8 InitFlag;
}CONFIG_YX;

/**********************about yc**************************/
#define YC_BOARD_NUM 5
#define PER_BOARD_YC_PORT_NUM 16
#define CHANNEL_NUM  80

#define YC_BOARD1 0
#define YC_BOARD2 1
#define YC_BOARD3 2
#define YC_BOARD4 3
#define YC_BOARD5 4
#define PER_LINE_YC_NUM 18
typedef enum _YcEnum{
	UA1 = 0,
	UB1,
	UC1,
	UAB1,
	UCB1,
	U01,
	IA11,//测量电流
	IB11,
	IC11,
	I01,
	IA12,//10,保护电流
	IB12,
	IC12,
	P1,
	Q1,
	S1,
	Factor1,
	Freq1,
	UA2,
	Freq16 =16*PER_LINE_YC_NUM-1,
	BattaryVol,
	EnvTemp,
	TerTemp,
	LineTemp,
	Dc1,
	Dc2,
	Other1,
	Other2,
	Other3,
	Other4,
	GPS_x,
	GPS_y,
}YcEnum;
#define MAX_YC_NUM GPS_y + 1
#define UA(n)  (UA1+UA2*n)
#define UB(n)  (UB1+UA2*n)
#define UC(n)  (UC1+UA2*n)
#define UAB(n) (UAB1+UA2*n)
#define UCB(n) (UCB1+UA2*n)
#define U0(n)  (U01+UA2*n)
#define IA1(n) (IA11+UA2*n)
#define IB1(n) (IB11+UA2*n)
#define IC1(n) (IC11+UA2*n)
#define I0(n)  (I01+UA2*n)
#define IA2(n) (IA12+UA2*n)
#define IB2(n) (IB12+UA2*n)
#define IC2(n) (IC12+UA2*n)
#define	P(n)	(P1+UA2*n)
#define	Q(n) 	(Q1+UA2*n)
#define	S(n)	(S1+UA2*n)
#define Factor(n)	(Factor1+UA2*n)
#define Freq(n)	(Freq1+UA2*n)
#define GetLineNoByYcId(id) id/PER_LINE_YC_NUM

typedef struct ycdata{
	int		id;//从0开始
	int		d_from;//where data from:1 is from physical channel,2 is from calculated,0 is invalid . by defult is 0.
	uint16	port;//if acquire data from physical port=channel,otherwise is 0xffff 
	int		type;//1:U,2:I
}ycdata_t;

typedef struct _CONFIG_YC{
	ycdata_t	config_data[MAX_YC_NUM];	//端口配置参数,每个配置项包括YcId,YcId对应的端口,YcId从0开始,无配置则端口号为0xffff. 数据来源于模拟量端口配置表0x5080
	uint16		configed_yc[MAX_YC_NUM];	//配置了端口的遥测量表,内容是YcId
	uint8		configed_yc_num;			//配置了端口的遥测量数量.
										//根据该数组中的YcId去查找config_data中相应YcId的端口,再对该端口进行计算.
	uint16	configed_yc_port[CHANNEL_NUM];	//配置了遥测量的端口表,内容是port号;
	uint8	configed_yc_port_num;			//配置了遥测量的端口数量；

	uint16	configed_need_yc[MAX_YC_NUM];//需要计算的模拟量,数据来源于信息体配置0x5098...
	int		configed_need_yc_num;

	uint16 configed_freq_port[MAX_LINE_NUM];//每回线使用那一个口采集的信号作为基准频率；
}CONFIG_YC;

/*******************about yk******************************/
typedef enum YKENUM{
	BREAKER1 = 0, //断路器1遥控
	BREAKER2,
	BREAKER3,
	BREAKER4,
	BREAKER5,
	BREAKER6,
	BREAKER7,
	BREAKER8,
	BREAKER9,
	BREAKER10,
	BREAKER11,
	BREAKER12,
	BREAKER13,
	BREAKER14,
	BREAKER15,
	BREAKER16,
	BATTARYACTION,//电池活化遥控
	WINGMOTOR,    //公共遥控1(开合,风机启停)
	WATERMOTOR,   //公共遥控2(开合,水泵启停
	BATTARYOFF,   //电池退出遥控
}YKENUM_E;
#define MAX_USER_YK_NUM (BATTARYOFF+1)
typedef struct ykdata{
	uint16 Id;
	uint16 ClosePort;
	uint16 OpenPort;
}ykdata_t;
typedef struct _CONFIG_YK{
	//装载的通道编号；如果是0xffff的代表此遥信没有对应的硬件通道；下标为遥信项,并且比上位机设置的多了两个电池管理的遥控
	ykdata_t config_data[MAX_USER_YK_NUM];
	uint8 yk_num;//实际配置的遥信项数；
	uint8 InitFlag;
}CONFIG_YK;

//about calculate 
#define SAMP_NUM 32
#define FA_SAMP_NUM SAMP_NUM/2
#define Quarter_NUM SAMP_NUM/4
#define _3Half_NUM	SAMP_NUM/2*3
#define SAMP_T	20/SAMP_NUM //defult sample time ms
#define STD_FREQ 5000//50HZ
#define XIEBOLEN		SAMP_NUM/2-1

#define MAXN		 (2*SAMP_NUM)
#define M			 (2*MAXN)

//about datastruct
typedef struct complex{
	float rel;
	float img;
}SP_COMPLEX;

typedef struct _set_ms_time{
	uint64 T;
	uint8 SetFlag;
	uint64 *Psystime;
}set_ms_time;

typedef struct BASEPROSTATU{
	uint64 CurrentMs1;
	uint64 CurrentMs2;
	bool CurrentBool;
	bool Trigger;
}BASEPROSTATU_T;

typedef struct _SOE_DATA {
    uint16 Data;
    uint16  DataId;
    CP56Time2a Time;
} SOE_DATA;
typedef struct _FAULT_DATA {
    int32 Data;
    CP56Time2a Time;
} FAULT_DATA;
typedef struct BREAKERPRO{
	float ia;//跳闸时刻电流
	float ib;
	float ic;
	float i0;
	uint16 cnt;//跳闸次数
	uint32 Time;//跳闸持续时间ms；
}BREAKERPRO_t;
typedef struct breakerproreg{
	BREAKERPRO_t BreakProSoe;
	uint64 StartTime;//跳闸开始时间；
	uint64 EndTime;//跳闸结束时间；
	set_ms_time  Tcnt;//
	int8 step;
}breakerproreg_t;

typedef union _YX_UNION{
	 struct YXBIT{
		uint8 bit1 :1;
		uint8 bit2 :1;
		uint8 res:6;
	} bit;
	uint8 all;
}YX_UNION;

#define COUPLE 1
#define SINGLE 0

typedef struct _YC_FREQ_PORT_REG{
	uint16 YcNum;
	uint16 configed_yc[MAX_LINE_NUM*2];
	float *Freq;
}YC_FREQ_PORT_REG;

typedef struct _CURRENT_COMPOSITION{
  uint16 YcName;
  uint16 Line;
  uint16 Port1;
  uint16 YcName1;
  uint16 Port2;
  uint16 YcName2;
  uint16 Port3;
  uint16 YcName3;
}CURRENT_COMPOSITION;

typedef struct angleratio{
	float angle;//角度误差
	float cos_a;//角度误差的余弦值
	float sin_a;//角度误差的正弦值
}angleratio_t;
//fpga 128 BYTE 可存
typedef struct ratio{
	float PortRatio[PER_LINE_YC_NUM];//UA1,UB1,UC1,UAB1,UBC1,U01,IA11,IB11,IC11,I01,IA12,IB12,IC12,
	float PowerLineRatio[3];// Pa ; Pb ; Pc; Pall;  线性系数
	angleratio_t PowerAngleRatio[3];//Qa ; Qb ;  Qc; Qall; 角度系数
}ratio_t;

#define RATIOINIT 0x55
typedef struct YcRatio_S{
	float PortRatio[PER_BOARD_YC_PORT_NUM];
	uint8 InitFlag;//indicate if this board has write  ratios,0x55 if has write. 
	int crc;
}__attribute__ ((packed))YcRatio_S;
extern YcRatio_S YcPortRatio[YC_BOARD_NUM];
typedef struct DcRatio{
	float K;
	float B;
	uint8 InitFlag;
}__attribute__ ((packed))DcRatio_S;
extern DcRatio_S DcPortRation[2][2],DcRation[2][2],DcRationTmp[2][2];//两个通道，两种类型（电流，电压）;//两路直流量量，每路直流量包含电流型电压型两种系数；

#define ACRATIOADD 0
#define DCRATIOADD (sizeof(YcRatio_S))

#define YC_MASK1 0x00000001
#define YC_MASK2 0x00000002
#define YC_MASK3 0x00000004
#define YC_MASK4 0x00000008
#define YC_MASK5 0x00000010
#define IO_MASK1 0x00000020
#define IO_MASK2 0x00000040
#define IO_MASK3 0x00000080
#define IO_MASK4 0x00000100
#define RESMASK1 0x00000200


typedef struct _32BIT_TERSTATU{
	uint32 yc1 : 1;
	uint32 yc2 : 1;
	uint32 yc3 : 1;
	uint32 yc4 : 1;
	uint32 yc5 : 1;
	uint32 inout1 : 1;
	uint32 inout2 : 1;
	uint32 inout3 : 1;
	uint32 inout4 : 1;
	uint32 ext	  : 1;
	uint32 bit11 : 1;
	uint32 bit12 : 1;
	uint32 bit13 : 1;
	uint32 bit14 : 1;
	uint32 bit15 : 1;
	uint32 bit16 : 1;
	uint32 bit17 : 1;
	uint32 bit18 : 1;
	uint32 bit19 : 1;
	uint32 bit20 : 1;
	uint32 bit21 : 1;
	uint32 bit22 : 1;
	uint32 bit23 : 1;
	uint32 bit24 : 1;
	uint32 bit25 : 1;
	uint32 bit26 : 1;
	uint32 bit27 : 1;
	uint32 bit28 : 1;
	uint32 bit29 : 1;
	uint32 bit30 : 1;
	uint32 bit31 : 1;
}_32BIT_TERSTATU_T;

typedef union TERSTATU{
	_32BIT_TERSTATU_T bit;
	uint32 all;
}TER_STATU;

extern const uint32 TerStatuMask[10];
extern TER_STATU 	TerFaultStatu;
extern TER_STATU 	TerFaultCheck;
extern TER_STATU 	UserParaMask;//参数配置需要的板支持的位置1参数配置0参数未有配置；
extern ratio_t  	YcRatio[MAX_LINE_NUM];
extern uint8 		YxCurrent[MAX_SG_DB_YXPORT_NUM];;//

typedef struct manvalue
{
	unsigned int U_harmonic_num;
	unsigned int U_harmonic_radio;
	unsigned int U_App;
	unsigned int I_harmonic_num;
	unsigned int I_harmonic_radio;
	unsigned int I_App;
	unsigned int angle;

	unsigned int U0_harmonic_num;
	unsigned int U0_harmonic_radio;
	unsigned int U0_App;
	unsigned int U0_angle;
	unsigned int I0_harmonic_num;
	unsigned int I0_harmonic_radio;
	unsigned int I0_App;
	unsigned int I0_angel;
	unsigned int enable;
}MANVALUE;
extern MANVALUE man_set;
typedef enum COMPLEXPOS{
DCCOMPONENT = 0,
BASEHARMONIC,
}COMPLEXPOS_E;

extern int16 AD_BUF_ori[CHANNEL_NUM][MAXN];
extern int16 AD_BUF_scada[CHANNEL_NUM][MAXN];
extern int16 AD_BUF_fa[CHANNEL_NUM][MAXN];
extern SP_COMPLEX AD_BUF_scada_cal[CHANNEL_NUM][SAMP_NUM];
extern SP_COMPLEX AD_BUF_fa_cal[CHANNEL_NUM][FA_SAMP_NUM];

extern uint8 FFT_GAP ; // fft计算周期因子
extern float w16[M];
extern float *const ptr_w16;
extern float w32[M];
extern float *const ptr_w32;
extern uint16 KeyStatu[IO_BOARD_NUM];
#endif /* DSPDATATYPE_H_ */
