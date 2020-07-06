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


#define CHENAL_NUM 40
#define SAMP_NUM 64
#define PROSAMP_NUM 16
extern uint8 FFT_GAP ; // fft计算周期因子
#define MAXHDYXNUM 21
#define MAXYCNUM 26
#define DATA_NAME_LEN 100
#define MAXN (64)
#define M    (2*MAXN)
#define PAD  (0)//(16)
//#define Y_TOL (1e-06)

#define FFT_NUM SAMP_NUM
#define NO_YC 0xff
#define NO_YX 0xff
#define FPGA_ON 1
#define FPGA_OFF 0

#define  MAX_LINE_NUM 2
#define HARMONIC_DATA_NUM 13

#define MAX_INNODE_NUM 2
#define INPUT_FUNNAME(n) (n)

#define MAX_HARDYX_NUM 12
#define MAX_SOE_NUM 100
#define MAX_USER_NUM 3
#define HDYXSOE 0
#define PROTECTSOE 1
#define DPYXSOE 2
#define NOREAD 1
#define READCLEAR 0


typedef struct complex{
	float rel;
	float img;
}SP_COMPLEX;

typedef struct settime{
	CP56Time2a T;
	uint8 SetFlag;
} settime_t;
typedef struct setmstime{
	uint64 T;
	uint8 SetFlag;
	uint64 *Psystime;
} setmstime_t;
typedef struct soe_data {
	uint8 ReadFlag;
    uint16 Data;
    uint16  PosId;
    CP56Time2a Time;
} SOE_DATA;
typedef struct soe_buffer {
    uint16 SoeCnt;
    SOE_DATA SoeData[MAX_SOE_NUM];
} SOE_BUFFER;
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
	setmstime_t  Tcnt;//
	int8 step;
}breakerproreg_t;
/*
class PowerWay:public BaseDataType{
public:
	PowerWay(){DataType = 33;}
	struct{
		uint8 line[MAX_LINE_NUM];
	}Data;
};

class DataType34:public BaseDataType{
public:
	DataType34(){DataType = 34;}
	struct{
		float a;
		float b;
		float c;
	}Data;
};
class DataType34_S{
public:
	DataType34_S(){Data.a=0;Data.b=0;Data.c=0;}
	struct{
		float a;
		float b;
		float c;
	}Data;
};
*/
typedef struct AdConfig{
	uint8 name[MAXYCNUM];//里面装载的是配置后的遥测项无间隔排序
	uint8 ad_compass[MAXYCNUM];//装载的通道编号；如果是255的代表此交采量没有对应的硬件通道；下表为遥测项
	uint8 fpga_board[4];// 0表示硬件不存在，1表示硬件存在；
	uint8 yc_num;//实际配置的遥测项数；
}ADCONFIG;

typedef struct AdConfigyx{
	uint8 name[MAXHDYXNUM];//里面装载的是配置后的遥测项无间隔排序
	uint8 ad_compass[MAXHDYXNUM];//装载的通道编号；如果是255的代表此交采量没有对应的硬件通道；下表为遥测项
	uint8 fpga_board[4];// 0表示硬件不存在，1表示硬件存在；
	uint8 yx_num;//实际配置的遥测项数；
}ADCONFIGYX;

typedef struct BASEPROSTATU2{
	uint64 CurrentMs1;
	uint64 CurrentMs2;
	bool CurrentBool;
	bool Trigger;
}BASEPROSTATU2_T;
typedef struct BASEPROSTATU{
	uint64 CurrentMs;
	uint64 CurrentMs2;
	bool CurrentBool;
	bool Trigger;
}BASEPROSTATU_T;
typedef struct angleratio{
	float angle;//角度误差
	float cos_a;//角度误差的余弦值
	float sin_a;//角度误差的正弦值
}angleratio_t;
//fpga 128 BYTE 可存
typedef struct ratio{
	float PortRatio[13];//UA1,UB1,UC1,UAB1,UBC1,U01,IA11,IB11,IC11,I011,IA12,IB12,IC12,
	float PowerLineRatio[4];// Pa ; Pb ; Pc; Pall;  线性系数
	angleratio_t PowerAngleRatio[2][4];//Qa ; Qb ;  Qc; Qall; 角度系数
}ratio_t;

extern ADCONFIG yc_configure;
extern ratio_t YcRatio[MAX_LINE_NUM];

extern uint8 BreakLockState[2];
/*class AdConfigPara:public BaseDataType{
public:
	struct AdConfig ad_cfg;
	AdConfigPara(){DataType=33;}
};
*/
#define PI 3.1415926
#define TWO_PI 6.2831852
#define TWOOFTHREE_PI 2.094395
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
HARMONIC2,
HARMONIC3,
HARMONIC4,
HARMONIC5,
HARMONIC6,
HARMONIC7,
HARMONIC8,
HARMONIC9,
HARMONIC10,
HARMONIC11,
HARMONIC12,
HARMONIC13,
HARMONIC14,
HARMONIC15,
HARMONIC16,
HARMONIC17,
HARMONIC18,
HARMONIC19,
HARMONIC20,
HARMONIC21,
HARMONIC22,
HARMONIC23,
HARMONIC24,
HARMONIC25,
HARMONIC26,
HARMONIC27,
HARMONIC28,
HARMONIC29,
HARMONIC30,
HARMONIC31,
CJGT_DCCOMPONENT,//共轭点 直流量
CJGT_HARMONIC31,
CJGT_HARMONIC30,
CJGT_HARMONIC29,
CJGT_HARMONIC28,
CJGT_HARMONIC27,
CJGT_HARMONIC26,
CJGT_HARMONIC25,
CJGT_HARMONIC24,
CJGT_HARMONIC23,
CJGT_HARMONIC22,
CJGT_HARMONIC21,
CJGT_HARMONIC20,
CJGT_HARMONIC19,
CJGT_HARMONIC18,
CJGT_HARMONIC17,
CJGT_HARMONIC16,
CJGT_HARMONIC15,
CJGT_HARMONIC14,
CJGT_HARMONIC13,
CJGT_HARMONIC12,
CJGT_HARMONIC11,
CJGT_HARMONIC10,
CJGT_HARMONIC9,
CJGT_HARMONIC8,
CJGT_HARMONIC7,
CJGT_HARMONIC6,
CJGT_HARMONIC5,
CJGT_HARMONIC4,
CJGT_HARMONIC3,
CJGT_HARMONIC2,
CJGT_BASEHARMONIC
}COMPLEXPOS_E;
typedef enum ADCP{
	UA1 = 0,
	UB1,
	UC1,
	UAB1,
	UCB1,
	U01,
	IA11,
	IB11,
	IC11,
	I011,
	IA12,
	IB12,
	IC12,
	UA2,
	UB2,
	UC2,
	UAB2,
	UCB2,
	U02,
	IA21,
	IB21,
	IC21,
	I021,
	IA22,
	IB22,
	IC22
/*
	UA3,
	UB3,
	UC3,
	U03,
	IA31,
	IB31,
	IC31,
	I031,
	IA32,
	IB32,
	IC32,
	I032,
	IA33,
	IB33,
	IC33,
	I033,
	IA34,
	IB34,
	IC34,
	I034,

	UA4,
	UB4,
	UC4,
	U04,
	IA41,
	IB41,
	IC41,
	I041,
	IA42,
	IB42,
	IC42,
	I042,
	IA43,
	IB43,
	IC43,
	I043,
	IA44,
	IB44,
	IC44,
	I044
*/
}JCENUM ;
typedef enum YXENUM{
	HE1 = 0,///<断路器1合位
	FEN1,///<断路器1分位
	CN1, ///<断路器1储能位置
	GLD1,///<断路器1隔离刀闸位置（备用遥信4）
	YX15,///<备用遥信5变位
	YX16,//<备用遥信6变位
	XJGZ1,//故障指示器指示线路2相间短路故障
	JDGZ1,//故障指示器指示线路2接地故障
	SP6GZ1,//SF6气体异常报警信号
	SP6BS1,//SF6气压闭锁信号
	HE2 ,///<断路器1合位
	FEN2,///<断路器1分位
	CN2, ///<断路器1储能位置
	GLD2,///<断路器1隔离刀闸位置（备用遥信4）
	YX25,///<备用遥信5变位
	YX26,//<备用遥信6变位
	XJGZ2,//故障指示器指示线路2相间短路故障
	JDGZ2, //故障指示器指示线路2接地故障
	SP6GZ2,//SF6气体异常报警信号
	SP6BS2,//SF6气压闭锁信号
	YFJD   //远方接地位置
}YXENUM_E; //共计21项
typedef enum YKENUM{
	BREAKER1 = 0, //断路器1遥控
	BREAKER1JHH,  //断路器1 解合环
	BREAKER2,     //断路器2遥控
	BREAKER2JHH,  //断路器2解合环
	BATTARYACTION,//电池活化遥控
	BATTARYOFF,   //电池退出遥控
	MOTOR		  //电机控制
}YKENUM_E;
#define UA(n)  (UA1+UA2*n)
#define UB(n)  (UB1+UA2*n)
#define UC(n)  (UC1+UA2*n)
#define UAB(n) (UAB1+UA2*n)
#define UCB(n) (UCB1+UA2*n)
#define U0(n)  (U01+UA2*n)
#define IA1(n) (IA11+UA2*n)
#define IB1(n) (IB11+UA2*n)
#define IC1(n) (IC11+UA2*n)
#define I01(n) (I011+UA2*n)
#define IA2(n) (IA12+UA2*n)
#define IB2(n) (IB12+UA2*n)
#define IC2(n) (IC12+UA2*n)

#define HE(n) 	(HE1 + HE2*n)	///<断路器1合位
#define FEN(n)	(FEN1 + HE2*n)	///<断路器1分位
#define CN(n)	(CN1 + HE2*n)	///<断路器1储能位置
#define GLD(n)	(GLD1 + HE2*n)	///<断路器1隔离刀闸位置（备用遥信4）
#define YX5(n)	(YX15 + HE2*n)	///<备用遥信5变位
#define YX6(n)	(YX16 + HE2*n)	//<备用遥信6变位
#define XJGZ(n)	(XJGZ1 + HE2*n)	//故障指示器指示线路2相间短路故障
#define JDGZ(n)	(JDGZ1 + HE2*n)	//故障指示器指示线路2接地故障
#define SP6GZ(n)(SP6GZ1 + HE2*n)	//SF6气体异常报警信号
#define SP6BS(n)(SP6BS1 + HE2*n)	//SF6气压闭锁信号
extern JCENUM jcenum;
extern SP_COMPLEX AD_BUF[CHENAL_NUM][SAMP_NUM+2*PAD];
extern SP_COMPLEX AD16_BUF[CHENAL_NUM][PROSAMP_NUM+2*PAD];
extern int16 AD_BU[CHENAL_NUM][SAMP_NUM+2*PAD];
extern SP_COMPLEX FFT_OUT[CHENAL_NUM][PROSAMP_NUM+2*PAD];
extern SP_COMPLEX LineVol[CHENAL_NUM];
extern float w64[M + 2 * PAD];
extern float w16[M + 2 * PAD];
//extern float *const ptr_w ;
extern float *const ptr_w64;
extern float *const ptr_w16;
#define  YXTAB 0
#define  YCTAB 1
#define  PARATAB 2
#define  YKTAB 3

#endif /* DSPDATATYPE_H_ */
