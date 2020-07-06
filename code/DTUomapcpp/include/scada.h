#ifndef SCADA_H
#define SCADA_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到BaseApp类.

#define SINGLEHARMONIC	1
#define COS				0
#define SIN				1
#define TOTALRMS		1
#define BASERMS			0
#define MAX_YC_DATA_NUM	300
#define NOMINAL_U scada.scada_cfg.d_508E.Data //额定电压
#define NOMINAL_I scada.scada_cfg.d_508D.Data //额定电流
#define TYPE_U 		1
#define TYPE_I 		2
#define TYPE_PQS 	3

#define MAX_HDYX_BUF 200
#define YXMULTIPLE 2

#define  NOMINAL_S (scada.scada_cfg.d_508E.Data * scada.scada_cfg.d_508D.Data)
#define  NOMINAL_TOTALS (scada.scada_cfg.d_508E.Data * scada.scada_cfg.d_508D.Data*3)

typedef struct SIGRCD{
	float samp_t;//采样周期；ms
	float Freq;	//计算出来的信号频率
	float T;//计算出来的信号周期，正常应为20ms
	float samp_Freq;//预期的信号频率，默认50Hz
	float difFreq;//当前频率与采样频率差值
	uint16 samp_adjcnt;//采样频率调整计数
}SIGRCD;

#define DB_YX_ID_START	YX_0171//用于配置双点遥信属性GetYxNature()
#define DB_YX_ID_END	YX_0180
typedef struct kr{
	Uint64 KR1:1;
	Uint64 KR2:1;
	Uint64 KR3:1;
	Uint64 KR4:1;
	Uint64 KR5:1;
	Uint64 KR6:1;
	Uint64 KR7:1;
	Uint64 KR8:1;
	Uint64 KR9:1;
	Uint64 KR10:1;
	Uint64 KR11:1;
	Uint64 KR12:1;
	Uint64 KR13:1;
	Uint64 KR14:1;
	Uint64 KR15:1;
	Uint64 KR16:1;
	Uint64 KR17:1;
	Uint64 KR18:1;
	Uint64 KR19:1;
	Uint64 KR20:1;
	Uint64 KR21:1;
	Uint64 KR22:1;
	Uint64 KR23:1;
	Uint64 KR24:1;
	Uint64 KR25:1;
	Uint64 KR26:1;
	Uint64 KR27:1;
	Uint64 KR28:1;
	Uint64 KR29:1;
	Uint64 KR30:1;
	Uint64 KR31:1;
	Uint64 KR32:1;
	Uint64 KR33:1;
	Uint64 KR34:1;
	Uint64 KR35:1;
	Uint64 KR36:1;
	Uint64 KR37:1;
	Uint64 KR38:1;
	Uint64 KR39:1;
	Uint64 KR40:1;
	Uint64 KR41:1;
	Uint64 KR42:1;
	Uint64 KR43:1;
	Uint64 KR44:1;
	Uint64 KR45:1;
	Uint64 KR46:1;
	Uint64 KR47:1;
	Uint64 KR48:1;
	Uint64 KR49:1;
	Uint64 KR50:1;
	Uint64 KR51:1;
	Uint64 KR52:1;
	Uint64 KR53:1;
	Uint64 KR54:1;
	Uint64 KR55:1;
	Uint64 KR56:1;
	Uint64 KR57:1;
	Uint64 KR58:1;
	Uint64 KR59:1;
	Uint64 KR60:1;
	Uint64 KR61:1;
	Uint64 KR62:1;
	Uint64 KR63:1;
	Uint64 KR64:1;
	Uint64 KR65:1;
	Uint64 KR66:1;
	Uint64 KR67:1;
	Uint64 KR68:1;
	Uint64 KR69:1;
	Uint64 KR70:1;
	Uint64 KR71:1;
	Uint64 KR72:1;
	Uint64 KR73:1;
	Uint64 KR74:1;
	Uint64 KR75:1;
	Uint64 KR76:1;
	Uint64 KR77:1;
	Uint64 KR78:1;
	Uint64 KR79:1;
	Uint64 KR80:1;
	Uint64 KR81:1;
	Uint64 KR82:1;
	Uint64 KR83:1;
	Uint64 KR84:1;
	Uint64 KR85:1;
	Uint64 KR86:1;
	Uint64 KR87:1;
	Uint64 KR88:1;
	Uint64 KR89:1;
	Uint64 KR90:1;
	Uint64 KR91:1;
	Uint64 KR92:1;
	Uint64 KR93:1;
	Uint64 KR94:1;
	Uint64 KR95:1;
	Uint64 KR96:1;
	Uint64 KR97:1;
	Uint64 KR98:1;
	Uint64 KR99:1;
	Uint64 KR100:1;
	Uint64 KR101:1;
	Uint64 KR102:1;
	Uint64 KR103:1;
	Uint64 KR104:1;
	Uint64 KR105:1;
	Uint64 KR106:1;
	Uint64 KR107:1;
	Uint64 KR108:1;
	Uint64 KR109:1;
	Uint64 KR110:1;
	Uint64 KR111:1;
	Uint64 KR112:1;
	Uint64 KR113:1;
	Uint64 KR114:1;
	Uint64 KR115:1;
	Uint64 KR116:1;
	Uint64 KR117:1;
	Uint64 KR118:1;
	Uint64 KR119:1;
	Uint64 KR120:1;
	Uint64 KR121:1;
	Uint64 KR122:1;
	Uint64 KR123:1;
	Uint64 KR124:1;
	Uint64 KR125:1;
	Uint64 KR126:1;
	Uint64 KR127:1;
	Uint64 KR128:1;
	//从第129位开始是虚拟双点遥信
	Uint64 KR129:2;
	Uint64 KR130:2;
	Uint64 KR131:2;
	Uint64 KR132:2;
	Uint64 KR133:2;
	Uint64 KR134:2;
	Uint64 KR135:2;
	Uint64 KR136:2;
	Uint64 KR137:2;
	Uint64 KR138:2;
	Uint64 KR139:2;
	Uint64 KR140:2;
	Uint64 KR141:2;
	Uint64 KR142:2;
	Uint64 KR143:2;
	Uint64 KR144:2;
	Uint64 RES :32;

} KR;

union KR_UNION {
 struct {
	 Uint64 S1;
	 Uint64 S2;
	 Uint64 D1;
 }all;
   KR  bit;
};

typedef struct SKR{
	Uint64 KR1:1;
	Uint64 KR2:1;
	Uint64 KR3:1;
	Uint64 KR4:1;
	Uint64 KR5:1;
	Uint64 KR6:1;
	Uint64 KR7:1;
	Uint64 KR8:1;
	Uint64 KR9:1;
	Uint64 KR10:1;
	Uint64 KR11:1;
	Uint64 KR12:1;
	Uint64 KR13:1;
	Uint64 KR14:1;
	Uint64 KR15:1;
	Uint64 KR16:1;
	Uint64 KR17:1;
	Uint64 KR18:1;
	Uint64 KR19:1;
	Uint64 KR20:1;
	Uint64 KR21:1;
	Uint64 KR22:1;
	Uint64 KR23:1;
	Uint64 KR24:1;
	Uint64 KR25:1;
	Uint64 KR26:1;
	Uint64 KR27:1;
	Uint64 KR28:1;
	Uint64 KR29:1;
	Uint64 KR30:1;
	Uint64 KR31:1;
	Uint64 KR32:1;
	Uint64 KR33:1;
	Uint64 KR34:1;
	Uint64 KR35:1;
	Uint64 KR36:1;
	Uint64 KR37:1;
	Uint64 KR38:1;
	Uint64 KR39:1;
	Uint64 KR40:1;
	Uint64 KR41:1;
	Uint64 KR42:1;
	Uint64 KR43:1;
	Uint64 KR44:1;
	Uint64 KR45:1;
	Uint64 KR46:1;
	Uint64 KR47:1;
	Uint64 KR48:1;
	Uint64 KR49:1;
	Uint64 KR50:1;
	Uint64 KR51:1;
	Uint64 KR52:1;
	Uint64 KR53:1;
	Uint64 KR54:1;
	Uint64 KR55:1;
	Uint64 KR56:1;
	Uint64 KR57:1;
	Uint64 KR58:1;
	Uint64 KR59:1;
	Uint64 KR60:1;
	Uint64 KR61:1;
	Uint64 KR62:1;
	Uint64 KR63:1;
	Uint64 KR64:1;
	Uint64 KR65:1;
	Uint64 KR66:1;
	Uint64 KR67:1;
	Uint64 KR68:1;
	Uint64 KR69:1;
	Uint64 KR70:1;
	Uint64 KR71:1;
	Uint64 KR72:1;
	Uint64 KR73:1;
	Uint64 KR74:1;
	Uint64 KR75:1;
	Uint64 KR76:1;
	Uint64 KR77:1;
	Uint64 KR78:1;
	Uint64 KR79:1;
	Uint64 KR80:1;
	Uint64 KR81:1;
	Uint64 KR82:1;
	Uint64 KR83:1;
	Uint64 KR84:1;
	Uint64 KR85:1;
	Uint64 KR86:1;
	Uint64 KR87:1;
	Uint64 KR88:1;
	Uint64 KR89:1;
	Uint64 KR90:1;
	Uint64 KR91:1;
	Uint64 KR92:1;
	Uint64 KR93:1;
	Uint64 KR94:1;
	Uint64 KR95:1;
	Uint64 KR96:1;
	Uint64 KR97:1;
	Uint64 KR98:1;
	Uint64 KR99:1;
	Uint64 KR100:1;
	Uint64 KR101:1;
	Uint64 KR102:1;
	Uint64 KR103:1;
	Uint64 KR104:1;
	Uint64 KR105:1;
	Uint64 KR106:1;
	Uint64 KR107:1;
	Uint64 KR108:1;
	Uint64 KR109:1;
	Uint64 KR110:1;
	Uint64 KR111:1;
	Uint64 KR112:1;
	Uint64 KR113:1;
	Uint64 KR114:1;
	Uint64 KR115:1;
	Uint64 KR116:1;
	Uint64 KR117:1;
	Uint64 KR118:1;
	Uint64 KR119:1;
	Uint64 KR120:1;
	Uint64 KR121:1;
	Uint64 KR122:1;
	Uint64 KR123:1;
	Uint64 KR124:1;
	Uint64 KR125:1;
	Uint64 KR126:1;
	Uint64 KR127:1;
	Uint64 KR128:1;
	Uint64 KR129:1;
	Uint64 KR130:1;
	Uint64 KR131:1;
	Uint64 KR132:1;
	Uint64 KR133:1;
	Uint64 KR134:1;
	Uint64 KR135:1;
	Uint64 KR136:1;
	Uint64 KR137:1;
	Uint64 KR138:1;
	Uint64 KR139:1;
	Uint64 KR140:1;
	Uint64 KR141:1;
	Uint64 KR142:1;
	Uint64 KR143:1;
	Uint64 KR144:1;
	Uint64 KR145:1;
	Uint64 KR146:1;
	Uint64 KR147:1;
	Uint64 KR148:1;
	Uint64 KR149:1;
	Uint64 KR150:1;
	Uint64 KR151:1;
	Uint64 KR152:1;
	Uint64 KR153:1;
	Uint64 KR154:1;
	Uint64 KR155:1;
	Uint64 KR156:1;
	Uint64 KR157:1;
	Uint64 KR158:1;
	Uint64 KR159:1;
	Uint64 KR160:1;
	Uint64 KR161:1;
	Uint64 KR162:1;
	Uint64 KR163:1;
	Uint64 KR164:1;
	Uint64 KR165:1;
	Uint64 KR166:1;
	Uint64 KR167:1;
	Uint64 KR168:1;
	Uint64 KR169:1;
	Uint64 KR170:1;
	Uint64 KR171:1;
	Uint64 KR172:1;
	Uint64 KR173:1;
	Uint64 KR174:1;
	Uint64 KR175:1;
	Uint64 KR176:1;
	Uint64 KR177:1;
	Uint64 KR178:1;
	Uint64 KR179:1;
	Uint64 KR180:1;
	Uint64 KR181:1;
	Uint64 KR182:1;
	Uint64 KR183:1;
	Uint64 KR184:1;
	Uint64 KR185:1;
	Uint64 KR186:1;
	Uint64 KR187:1;
	Uint64 KR188:1;
	Uint64 KR189:1;
	Uint64 KR190:1;
	Uint64 KR191:1;
	Uint64 KR192:1;
}SKR;

union SKR_UNION {
	struct {
		Uint64 S1;
		Uint64 S2;
		Uint64 D1;
	}all;
	SKR  bit;
};

typedef struct dyxport {
    int16 ClosePort;
    int16 OpenPort;
}dyxport;
extern dyxport DyxportTab[DB_YX_NUM];
extern Uint64 FenYxMask[DB_YX_NUM];
extern Uint64 HeYxMask[DB_YX_NUM];
extern Uint64  MASKTAB[144];


typedef struct _YX_SYN_REG{
	Uint16 GroupYxFlag;//一组遥信同步开始标志
	Uint64 StartTimeMark;//首遥信时间标记
	CP56Time2a StartTime;//首遥信时间;
	Uint16 FirstYxPos;//一组遥信首遥信号；
	Uint64 LastCn;//最近的遥信变位到目前的时间寄存器；
	Uint32 Absms;//与首遥信的相对毫秒时间间隔；
	Uint64 TotalCn;//从同步开始计时的时间寄存器；
}YX_SYN_REG;
extern YX_SYN_REG YxSynFlag;
typedef struct _YX_BUFF{
    uint16 CurRead;
    uint16 CurPrepare;
    struct buff{
    	CP56Time2a time;
    	Uint64 TimeCnt;
    	union KR_UNION ChangeFlag;
    	union KR_UNION Yx;
    	union KR_UNION OldYx;
    }databuff[MAX_HDYX_BUF];
}YX_BUFF;

extern YX_BUFF YxBuff;
extern union SKR_UNION  krtmp;
extern union SKR_UNION  lastkrtmp;
typedef struct BitReg{
    uint16 BitNum;  //整数的BIT位 1的数目
    uint16 BitLocationMask[MAX_YX_PORT_NUM];//遥信bit位与遥信号的对应关系；
}BitReg;
typedef struct _YX_TIME_FLAG{
	union KR_UNION Flag;
	CP56Time2a time;
	Uint64 TimeCnt;
}YX_TIME_FLAG;
typedef struct _YX_REG{
	YX_TIME_FLAG TimeFlag[MAX_SG_DB_YXPORT_NUM];
	union KR_UNION  Flag;
	union KR_UNION  Statu;
	union KR_UNION  OldStatu;
}YX_REG;
extern YX_REG YxTimeFlag;


typedef struct _ALL_YC{
	DataType4	Ua1_4001;
	DataType4	Ub1_4002;
	DataType4	Uc1_4003;
	DataType4	Uab1_4004;
	DataType4	Ucb1_4005;
	DataType4	U01_4006;
	DataType4	Ia1_4007;
	DataType4	Ib1_4008;
	DataType4	Ic1_4009;
	DataType4	I01_400A;
	DataType4	ia1_400B;
	DataType4	ib1_400C;
	DataType4	ic1_400D;
	DataType4	P1_400E	;
	DataType4	Q1_400F	;
	DataType4	S1_4010	;
	DataType4	COS1_4011;
	DataType4	Freq1_4012;
	DataType4	Ua2_4013;
	DataType4	Ub2_4014;
	DataType4	Uc2_4015;
	DataType4	Uab2_4016;
	DataType4	Ucb2_4017;
	DataType4	U02_4018;
	DataType4	Ia2_4019;
	DataType4	Ib2_401A;
	DataType4	Ic2_401B;
	DataType4	I02_401C;
	DataType4	ia2_401D;
	DataType4	ib2_401E;
	DataType4	ic2_401F;
	DataType4	P2_4020;
	DataType4	Q2_4021;
	DataType4	S2_4022;
	DataType4	COS2_4023;
	DataType4	Freq2_4024;
	DataType4	Ua3_4025;
	DataType4	Ub3_4026;
	DataType4	Uc3_4027;
	DataType4	Uab3_4028;
	DataType4	Ucb3_4029;
	DataType4	U03_402A;
	DataType4	Ia3_402B;
	DataType4	Ib3_402C;
	DataType4	Ic3_402D;
	DataType4	I03_402E;
	DataType4	ia3_402F;
	DataType4	ib3_4030;
	DataType4	ic3_4031;
	DataType4	P3_4032;
	DataType4	Q3_4033;
	DataType4	S3_4034;
	DataType4	COS3_4035;
	DataType4	Freq3_4036;
	DataType4	Ua4_4037;
	DataType4	Ub4_4038;
	DataType4	Uc4_4039;
	DataType4	Uab4_403A;
	DataType4	Ucb4_403B;
	DataType4	U04_403C;
	DataType4	Ia4_403D;
	DataType4	Ib4_403E;
	DataType4	Ic4_403F;
	DataType4	I04_4040;
	DataType4	ia4_4041;
	DataType4	ib4_4042;
	DataType4	ic4_4043;
	DataType4	P4_4044	;
	DataType4	Q4_4045	;
	DataType4	S4_4046	;
	DataType4	COS4_4047;
	DataType4	Freq4_4048;
	DataType4	Ua5_4049;
	DataType4	Ub5_404A;
	DataType4	Uc5_404B;
	DataType4	Uab5_404C;
	DataType4	Ucb5_404D;
	DataType4	U05_404E;
	DataType4	Ia5_404F;
	DataType4	Ib5_4050;
	DataType4	Ic5_4051;
	DataType4	I05_4052;
	DataType4	ia5_4053;
	DataType4	ib5_4054;
	DataType4	ic5_4055;
	DataType4	P5_4056	;
	DataType4	Q5_4057	;
	DataType4	S5_4058	;
	DataType4	COS5_4059	;
	DataType4	Freq5_405A	;
	DataType4	Ua6_405B	;
	DataType4	Ub6_405C	;
	DataType4	Uc6_405D	;
	DataType4	Uab6_405E	;
	DataType4	Ucb6_405F	;
	DataType4	U06_4060	;
	DataType4	Ia6_4061	;
	DataType4	Ib6_4062	;
	DataType4	Ic6_4063	;
	DataType4	I06_4064	;
	DataType4	ia6_4065	;
	DataType4	ib6_4066	;
	DataType4	ic6_4067	;
	DataType4	P6_4068	;
	DataType4	Q6_4069	;
	DataType4	S6_406A	;
	DataType4	COS6_406B	;
	DataType4	Freq6_406C	;
	DataType4	Ua7_406D	;
	DataType4	Ub7_406E	;
	DataType4	Uc7_406F	;
	DataType4	Uab7_4070	;
	DataType4	Ucb7_4071	;
	DataType4	U07_4072	;
	DataType4	Ia7_4073	;
	DataType4	Ib7_4074	;
	DataType4	Ic7_4075	;
	DataType4	I07_4076	;
	DataType4	ia7_4077	;
	DataType4	ib7_4078	;
	DataType4	ic7_4079	;
	DataType4	P7_407A	;
	DataType4	Q7_407B	;
	DataType4	S7_407C	;
	DataType4	COS7_407D	;
	DataType4	Freq7_407E	;
	DataType4	Ua8_407F	;
	DataType4	Ub8_4080	;
	DataType4	Uc8_4081	;
	DataType4	Uab8_4082	;
	DataType4	Ucb8_4083	;
	DataType4	U08_4084	;
	DataType4	Ia8_4085	;
	DataType4	Ib8_4086	;
	DataType4	Ic8_4087	;
	DataType4	I08_4088	;
	DataType4	ia8_4089	;
	DataType4	ib8_408A	;
	DataType4	ic8_408B	;
	DataType4	P8_408C	;
	DataType4	Q8_408D	;
	DataType4	S8_408E	;
	DataType4	COS8_408F	;
	DataType4	Freq8_4090	;
	DataType4	Ua9_4091	;
	DataType4	Ub9_4092	;
	DataType4	Uc9_4093	;
	DataType4	Uab9_4094	;
	DataType4	Ucb9_4095	;
	DataType4	U09_4096	;
	DataType4	Ia9_4097	;
	DataType4	Ib9_4098	;
	DataType4	Ic9_4099	;
	DataType4	I09_409A	;
	DataType4	ia9_409B	;
	DataType4	ib9_409C	;
	DataType4	ic9_409D	;
	DataType4	P9_409E	;
	DataType4	Q9_409F	;
	DataType4	S9_40A0	;
	DataType4	COS9_40A1	;
	DataType4	Freq9_40A2	;
	DataType4	Ua10_40A3	;
	DataType4	Ub10_40A4	;
	DataType4	Uc10_40A5	;
	DataType4	Uab10_40A6	;
	DataType4	Ucb10_40A7	;
	DataType4	U010_40A8	;
	DataType4	Ia10_40A9	;
	DataType4	Ib10_40AA	;
	DataType4	Ic10_40AB	;
	DataType4	I010_40AC	;
	DataType4	ia10_40AD	;
	DataType4	ib10_40AE	;
	DataType4	ic10_40AF	;
	DataType4	P10_40B0	;
	DataType4	Q10_40B1	;
	DataType4	S10_40B2	;
	DataType4	COS10_40B3	;
	DataType4	Freq10_40B4	;
	DataType4	Ua11_40B5	;
	DataType4	Ub11_40B6	;
	DataType4	Uc11_40B7	;
	DataType4	Uab11_40B8	;
	DataType4	Ucb11_40B9	;
	DataType4	U011_40BA	;
	DataType4	Ia11_40BB	;
	DataType4	Ib11_40BC	;
	DataType4	Ic11_40BD	;
	DataType4	I011_40BE	;
	DataType4	ia11_40BF	;
	DataType4	ib11_40C0	;
	DataType4	ic11_40C1	;
	DataType4	P11_40C2	;
	DataType4	Q11_40C3	;
	DataType4	S11_40C4	;
	DataType4	COS11_40C5	;
	DataType4	Freq11_40C6	;
	DataType4	Ua12_40C7	;
	DataType4	Ub12_40C8	;
	DataType4	Uc12_40C9	;
	DataType4	Uab12_40CA	;
	DataType4	Ucb12_40CB	;
	DataType4	U012_40CC	;
	DataType4	Ia12_40CD	;
	DataType4	Ib12_40CE	;
	DataType4	Ic12_40CF	;
	DataType4	I012_40D0	;
	DataType4	ia12_40D1	;
	DataType4	ib12_40D2	;
	DataType4	ic12_40D3	;
	DataType4	P12_40D4	;
	DataType4	Q12_40D5	;
	DataType4	S12_40D6	;
	DataType4	COS12_40D7	;
	DataType4	Freq12_40D8	;
	DataType4	Ua13_40D9	;
	DataType4	Ub13_40DA	;
	DataType4	Uc13_40DB	;
	DataType4	Uab13_40DC	;
	DataType4	Ucb13_40DD	;
	DataType4	U013_40DE	;
	DataType4	Ia13_40DF	;
	DataType4	Ib13_40E0	;
	DataType4	Ic13_40E1	;
	DataType4	I013_40E2	;
	DataType4	ia13_40E3	;
	DataType4	ib13_40E4	;
	DataType4	ic13_40E5	;
	DataType4	P13_40E6	;
	DataType4	Q13_40E7	;
	DataType4	S13_40E8	;
	DataType4	COS13_40E9	;
	DataType4	Freq13_40EA	;
	DataType4	Ua14_40EB	;
	DataType4	Ub14_40EC	;
	DataType4	Uc14_40ED	;
	DataType4	Uab14_40EE	;
	DataType4	Ucb14_40EF	;
	DataType4	U014_40F0	;
	DataType4	Ia14_40F1	;
	DataType4	Ib14_40F2	;
	DataType4	Ic14_40F3	;
	DataType4	I014_40F4	;
	DataType4	ia14_40F5	;
	DataType4	ib14_40F6	;
	DataType4	ic14_40F7	;
	DataType4	P14_40F8	;
	DataType4	Q14_40F9	;
	DataType4	S14_40FA	;
	DataType4	COS14_40FB	;
	DataType4	Freq14_40FC	;
	DataType4	Ua15_40FD	;
	DataType4	Ub15_40FE	;
	DataType4	Uc15_40FF	;
	DataType4	Uab15_4100	;
	DataType4	Ucb15_4101	;
	DataType4	U015_4102	;
	DataType4	Ia15_4103	;
	DataType4	Ib15_4104	;
	DataType4	Ic15_4105	;
	DataType4	I015_4106	;
	DataType4	ia15_4107	;
	DataType4	ib15_4108	;
	DataType4	ic15_4109	;
	DataType4	P15_410A	;
	DataType4	Q15_410B	;
	DataType4	S15_410C	;
	DataType4	COS15_410D	;
	DataType4	Freq15_410E	;
	DataType4	Ua16_410F	;
	DataType4	Ub16_4110	;
	DataType4	Uc16_4111	;
	DataType4	Uab16_4112	;
	DataType4	Ucb16_4113	;
	DataType4	U016_4114	;
	DataType4	Ia16_4115	;
	DataType4	Ib16_4116	;
	DataType4	Ic16_4117	;
	DataType4	I016_4118	;
	DataType4	ia16_4119	;
	DataType4	ib16_411A	;
	DataType4	ic16_411B	;
	DataType4	P16_411C	;
	DataType4	Q16_411D	;
	DataType4	S16_411E	;
	DataType4	COS16_411F	;
	DataType4	Freq16_4120	;
	DataType4	Dcv_4121	;    //蓄电池电压
	DataType4	EnvTemp_4122	;//环境温度
	DataType4	TerTemp_4123	;//终端温度
	DataType4	LineTemp_4124	;//电缆温度
	DataType4	DC1_4125	;
	DataType4	DC2_4126	;
	DataType4	d_4201;   //其他数据1
	DataType4	d_4401;   //其他数据2
	DataType4	d_4601;   //其他数据3
	DataType4	d_4801;   //其他数据4
	DataType4	d_4A01;   // GPS经度
	DataType4	d_4A02;   //GPS纬度
	////////
}ALL_YC;
#define YC_DATA_ID_BEGIN 0x4001//data id bagin,not msg id ,msg id is for protocol

typedef struct _CHANGE_YC_REG{
	float data;
	float *dead;
	set_ms_time YcDeadTime;
}CHANGE_YC_REG;
typedef struct _CHANGE_YC{
	//遥测类数据
	CHANGE_YC_REG	Ua1_4001	;
	CHANGE_YC_REG	Ub1_4002	;
	CHANGE_YC_REG	Uc1_4003	;
	CHANGE_YC_REG	Uab1_4004	;
	CHANGE_YC_REG	Ucb1_4005	;
	CHANGE_YC_REG	U01_4006	;
	CHANGE_YC_REG	Ia1_4007	;
	CHANGE_YC_REG	Ib1_4008	;
	CHANGE_YC_REG	Ic1_4009	;
	CHANGE_YC_REG	I01_400A	;
	CHANGE_YC_REG	ia1_400B	;
	CHANGE_YC_REG	ib1_400C	;
	CHANGE_YC_REG	ic1_400D	;
	CHANGE_YC_REG	P1_400E	;
	CHANGE_YC_REG	Q1_400F	;
	CHANGE_YC_REG	S1_4010	;
	CHANGE_YC_REG	COS1_4011	;
	CHANGE_YC_REG	Freq1_4012	;
	CHANGE_YC_REG	Ua2_4013	;
	CHANGE_YC_REG	Ub2_4014	;
	CHANGE_YC_REG	Uc2_4015	;
	CHANGE_YC_REG	Uab2_4016	;
	CHANGE_YC_REG	Ucb2_4017	;
	CHANGE_YC_REG	U02_4018	;
	CHANGE_YC_REG	Ia2_4019	;
	CHANGE_YC_REG	Ib2_401A	;
	CHANGE_YC_REG	Ic2_401B	;
	CHANGE_YC_REG	I02_401C	;
	CHANGE_YC_REG	ia2_401D	;
	CHANGE_YC_REG	ib2_401E	;
	CHANGE_YC_REG	ic2_401F	;
	CHANGE_YC_REG	P2_4020	;
	CHANGE_YC_REG	Q2_4021	;
	CHANGE_YC_REG	S2_4022	;
	CHANGE_YC_REG	COS2_4023	;
	CHANGE_YC_REG	Freq2_4024	;
	CHANGE_YC_REG	Ua3_4025	;
	CHANGE_YC_REG	Ub3_4026	;
	CHANGE_YC_REG	Uc3_4027	;
	CHANGE_YC_REG	Uab3_4028	;
	CHANGE_YC_REG	Ucb3_4029	;
	CHANGE_YC_REG	U03_402A	;
	CHANGE_YC_REG	Ia3_402B	;
	CHANGE_YC_REG	Ib3_402C	;
	CHANGE_YC_REG	Ic3_402D	;
	CHANGE_YC_REG	I03_402E	;
	CHANGE_YC_REG	ia3_402F	;
	CHANGE_YC_REG	ib3_4030	;
	CHANGE_YC_REG	ic3_4031	;
	CHANGE_YC_REG	P3_4032	;
	CHANGE_YC_REG	Q3_4033	;
	CHANGE_YC_REG	S3_4034	;
	CHANGE_YC_REG	COS3_4035	;
	CHANGE_YC_REG	Freq3_4036	;
	CHANGE_YC_REG	Ua4_4037	;
	CHANGE_YC_REG	Ub4_4038	;
	CHANGE_YC_REG	Uc4_4039	;
	CHANGE_YC_REG	Uab4_403A	;
	CHANGE_YC_REG	Ucb4_403B	;
	CHANGE_YC_REG	U04_403C	;
	CHANGE_YC_REG	Ia4_403D	;
	CHANGE_YC_REG	Ib4_403E	;
	CHANGE_YC_REG	Ic4_403F	;
	CHANGE_YC_REG	I04_4040	;
	CHANGE_YC_REG	ia4_4041	;
	CHANGE_YC_REG	ib4_4042	;
	CHANGE_YC_REG	ic4_4043	;
	CHANGE_YC_REG	P4_4044	;
	CHANGE_YC_REG	Q4_4045	;
	CHANGE_YC_REG	S4_4046	;
	CHANGE_YC_REG	COS4_4047	;
	CHANGE_YC_REG	Freq4_4048	;
	CHANGE_YC_REG	Ua5_4049	;
	CHANGE_YC_REG	Ub5_404A	;
	CHANGE_YC_REG	Uc5_404B	;
	CHANGE_YC_REG	Uab5_404C	;
	CHANGE_YC_REG	Ucb5_404D	;
	CHANGE_YC_REG	U05_404E	;
	CHANGE_YC_REG	Ia5_404F	;
	CHANGE_YC_REG	Ib5_4050	;
	CHANGE_YC_REG	Ic5_4051	;
	CHANGE_YC_REG	I05_4052	;
	CHANGE_YC_REG	ia5_4053	;
	CHANGE_YC_REG	ib5_4054	;
	CHANGE_YC_REG	ic5_4055	;
	CHANGE_YC_REG	P5_4056	;
	CHANGE_YC_REG	Q5_4057	;
	CHANGE_YC_REG	S5_4058	;
	CHANGE_YC_REG	COS5_4059	;
	CHANGE_YC_REG	Freq5_405A	;
	CHANGE_YC_REG	Ua6_405B	;
	CHANGE_YC_REG	Ub6_405C	;
	CHANGE_YC_REG	Uc6_405D	;
	CHANGE_YC_REG	Uab6_405E	;
	CHANGE_YC_REG	Ucb6_405F	;
	CHANGE_YC_REG	U06_4060	;
	CHANGE_YC_REG	Ia6_4061	;
	CHANGE_YC_REG	Ib6_4062	;
	CHANGE_YC_REG	Ic6_4063	;
	CHANGE_YC_REG	I06_4064	;
	CHANGE_YC_REG	ia6_4065	;
	CHANGE_YC_REG	ib6_4066	;
	CHANGE_YC_REG	ic6_4067	;
	CHANGE_YC_REG	P6_4068	;
	CHANGE_YC_REG	Q6_4069	;
	CHANGE_YC_REG	S6_406A	;
	CHANGE_YC_REG	COS6_406B	;
	CHANGE_YC_REG	Freq6_406C	;
	CHANGE_YC_REG	Ua7_406D	;
	CHANGE_YC_REG	Ub7_406E	;
	CHANGE_YC_REG	Uc7_406F	;
	CHANGE_YC_REG	Uab7_4070	;
	CHANGE_YC_REG	Ucb7_4071	;
	CHANGE_YC_REG	U07_4072	;
	CHANGE_YC_REG	Ia7_4073	;
	CHANGE_YC_REG	Ib7_4074	;
	CHANGE_YC_REG	Ic7_4075	;
	CHANGE_YC_REG	I07_4076	;
	CHANGE_YC_REG	ia7_4077	;
	CHANGE_YC_REG	ib7_4078	;
	CHANGE_YC_REG	ic7_4079	;
	CHANGE_YC_REG	P7_407A	;
	CHANGE_YC_REG	Q7_407B	;
	CHANGE_YC_REG	S7_407C	;
	CHANGE_YC_REG	COS7_407D	;
	CHANGE_YC_REG	Freq7_407E	;
	CHANGE_YC_REG	Ua8_407F	;
	CHANGE_YC_REG	Ub8_4080	;
	CHANGE_YC_REG	Uc8_4081	;
	CHANGE_YC_REG	Uab8_4082	;
	CHANGE_YC_REG	Ucb8_4083	;
	CHANGE_YC_REG	U08_4084	;
	CHANGE_YC_REG	Ia8_4085	;
	CHANGE_YC_REG	Ib8_4086	;
	CHANGE_YC_REG	Ic8_4087	;
	CHANGE_YC_REG	I08_4088	;
	CHANGE_YC_REG	ia8_4089	;
	CHANGE_YC_REG	ib8_408A	;
	CHANGE_YC_REG	ic8_408B	;
	CHANGE_YC_REG	P8_408C	;
	CHANGE_YC_REG	Q8_408D	;
	CHANGE_YC_REG	S8_408E	;
	CHANGE_YC_REG	COS8_408F	;
	CHANGE_YC_REG	Freq8_4090	;
	CHANGE_YC_REG	Ua9_4091	;
	CHANGE_YC_REG	Ub9_4092	;
	CHANGE_YC_REG	Uc9_4093	;
	CHANGE_YC_REG	Uab9_4094	;
	CHANGE_YC_REG	Ucb9_4095	;
	CHANGE_YC_REG	U09_4096	;
	CHANGE_YC_REG	Ia9_4097	;
	CHANGE_YC_REG	Ib9_4098	;
	CHANGE_YC_REG	Ic9_4099	;
	CHANGE_YC_REG	I09_409A	;
	CHANGE_YC_REG	ia9_409B	;
	CHANGE_YC_REG	ib9_409C	;
	CHANGE_YC_REG	ic9_409D	;
	CHANGE_YC_REG	P9_409E	;
	CHANGE_YC_REG	Q9_409F	;
	CHANGE_YC_REG	S9_40A0	;
	CHANGE_YC_REG	COS9_40A1	;
	CHANGE_YC_REG	Freq9_40A2	;
	CHANGE_YC_REG	Ua10_40A3	;
	CHANGE_YC_REG	Ub10_40A4	;
	CHANGE_YC_REG	Uc10_40A5	;
	CHANGE_YC_REG	Uab10_40A6	;
	CHANGE_YC_REG	Ucb10_40A7	;
	CHANGE_YC_REG	U010_40A8	;
	CHANGE_YC_REG	Ia10_40A9	;
	CHANGE_YC_REG	Ib10_40AA	;
	CHANGE_YC_REG	Ic10_40AB	;
	CHANGE_YC_REG	I010_40AC	;
	CHANGE_YC_REG	ia10_40AD	;
	CHANGE_YC_REG	ib10_40AE	;
	CHANGE_YC_REG	ic10_40AF	;
	CHANGE_YC_REG	P10_40B0	;
	CHANGE_YC_REG	Q10_40B1	;
	CHANGE_YC_REG	S10_40B2	;
	CHANGE_YC_REG	COS10_40B3	;
	CHANGE_YC_REG	Freq10_40B4	;
	CHANGE_YC_REG	Ua11_40B5	;
	CHANGE_YC_REG	Ub11_40B6	;
	CHANGE_YC_REG	Uc11_40B7	;
	CHANGE_YC_REG	Uab11_40B8	;
	CHANGE_YC_REG	Ucb11_40B9	;
	CHANGE_YC_REG	U011_40BA	;
	CHANGE_YC_REG	Ia11_40BB	;
	CHANGE_YC_REG	Ib11_40BC	;
	CHANGE_YC_REG	Ic11_40BD	;
	CHANGE_YC_REG	I011_40BE	;
	CHANGE_YC_REG	ia11_40BF	;
	CHANGE_YC_REG	ib11_40C0	;
	CHANGE_YC_REG	ic11_40C1	;
	CHANGE_YC_REG	P11_40C2	;
	CHANGE_YC_REG	Q11_40C3	;
	CHANGE_YC_REG	S11_40C4	;
	CHANGE_YC_REG	COS11_40C5	;
	CHANGE_YC_REG	Freq11_40C6	;
	CHANGE_YC_REG	Ua12_40C7	;
	CHANGE_YC_REG	Ub12_40C8	;
	CHANGE_YC_REG	Uc12_40C9	;
	CHANGE_YC_REG	Uab12_40CA	;
	CHANGE_YC_REG	Ucb12_40CB	;
	CHANGE_YC_REG	U012_40CC	;
	CHANGE_YC_REG	Ia12_40CD	;
	CHANGE_YC_REG	Ib12_40CE	;
	CHANGE_YC_REG	Ic12_40CF	;
	CHANGE_YC_REG	I012_40D0	;
	CHANGE_YC_REG	ia12_40D1	;
	CHANGE_YC_REG	ib12_40D2	;
	CHANGE_YC_REG	ic12_40D3	;
	CHANGE_YC_REG	P12_40D4	;
	CHANGE_YC_REG	Q12_40D5	;
	CHANGE_YC_REG	S12_40D6	;
	CHANGE_YC_REG	COS12_40D7	;
	CHANGE_YC_REG	Freq12_40D8	;
	CHANGE_YC_REG	Ua13_40D9	;
	CHANGE_YC_REG	Ub13_40DA	;
	CHANGE_YC_REG	Uc13_40DB	;
	CHANGE_YC_REG	Uab13_40DC	;
	CHANGE_YC_REG	Ucb13_40DD	;
	CHANGE_YC_REG	U013_40DE	;
	CHANGE_YC_REG	Ia13_40DF	;
	CHANGE_YC_REG	Ib13_40E0	;
	CHANGE_YC_REG	Ic13_40E1	;
	CHANGE_YC_REG	I013_40E2	;
	CHANGE_YC_REG	ia13_40E3	;
	CHANGE_YC_REG	ib13_40E4	;
	CHANGE_YC_REG	ic13_40E5	;
	CHANGE_YC_REG	P13_40E6	;
	CHANGE_YC_REG	Q13_40E7	;
	CHANGE_YC_REG	S13_40E8	;
	CHANGE_YC_REG	COS13_40E9	;
	CHANGE_YC_REG	Freq13_40EA	;
	CHANGE_YC_REG	Ua14_40EB	;
	CHANGE_YC_REG	Ub14_40EC	;
	CHANGE_YC_REG	Uc14_40ED	;
	CHANGE_YC_REG	Uab14_40EE	;
	CHANGE_YC_REG	Ucb14_40EF	;
	CHANGE_YC_REG	U014_40F0	;
	CHANGE_YC_REG	Ia14_40F1	;
	CHANGE_YC_REG	Ib14_40F2	;
	CHANGE_YC_REG	Ic14_40F3	;
	CHANGE_YC_REG	I014_40F4	;
	CHANGE_YC_REG	ia14_40F5	;
	CHANGE_YC_REG	ib14_40F6	;
	CHANGE_YC_REG	ic14_40F7	;
	CHANGE_YC_REG	P14_40F8	;
	CHANGE_YC_REG	Q14_40F9	;
	CHANGE_YC_REG	S14_40FA	;
	CHANGE_YC_REG	COS14_40FB	;
	CHANGE_YC_REG	Freq14_40FC	;
	CHANGE_YC_REG	Ua15_40FD	;
	CHANGE_YC_REG	Ub15_40FE	;
	CHANGE_YC_REG	Uc15_40FF	;
	CHANGE_YC_REG	Uab15_4100	;
	CHANGE_YC_REG	Ucb15_4101	;
	CHANGE_YC_REG	U015_4102	;
	CHANGE_YC_REG	Ia15_4103	;
	CHANGE_YC_REG	Ib15_4104	;
	CHANGE_YC_REG	Ic15_4105	;
	CHANGE_YC_REG	I015_4106	;
	CHANGE_YC_REG	ia15_4107	;
	CHANGE_YC_REG	ib15_4108	;
	CHANGE_YC_REG	ic15_4109	;
	CHANGE_YC_REG	P15_410A	;
	CHANGE_YC_REG	Q15_410B	;
	CHANGE_YC_REG	S15_410C	;
	CHANGE_YC_REG	COS15_410D	;
	CHANGE_YC_REG	Freq15_410E	;
	CHANGE_YC_REG	Ua16_410F	;
	CHANGE_YC_REG	Ub16_4110	;
	CHANGE_YC_REG	Uc16_4111	;
	CHANGE_YC_REG	Uab16_4112	;
	CHANGE_YC_REG	Ucb16_4113	;
	CHANGE_YC_REG	U016_4114	;
	CHANGE_YC_REG	Ia16_4115	;
	CHANGE_YC_REG	Ib16_4116	;
	CHANGE_YC_REG	Ic16_4117	;
	CHANGE_YC_REG	I016_4118	;
	CHANGE_YC_REG	ia16_4119	;
	CHANGE_YC_REG	ib16_411A	;
	CHANGE_YC_REG	ic16_411B	;
	CHANGE_YC_REG	P16_411C	;
	CHANGE_YC_REG	Q16_411D	;
	CHANGE_YC_REG	S16_411E	;
	CHANGE_YC_REG	COS16_411F	;
	CHANGE_YC_REG	Freq16_4120	;
	CHANGE_YC_REG	Dcv_4121	;
	CHANGE_YC_REG	EnvTemp_4122	;//环境温度
	CHANGE_YC_REG	TerTemp_4123	;//终端温度
	CHANGE_YC_REG	LineTemp_4124	;//电缆温度
	CHANGE_YC_REG	DC1_4125	;
	CHANGE_YC_REG	DC2_4126	;
	CHANGE_YC_REG	d_4201;   //其他数据1
	CHANGE_YC_REG	d_4401;   //其他数据2
	CHANGE_YC_REG	d_4601;   //其他数据3
	CHANGE_YC_REG	d_4801;   //其他数据4
	CHANGE_YC_REG	d_4A01;   // GPS经度
	CHANGE_YC_REG	d_4A02;   //GPS纬
}CHANGE_YC;

//功率计算可适应2PT，3CT；2PT，2CT；1PT，2CT；1PT，3CT，//需求对应
#define Pt_3 0  //3PT，3CT
#define Pt_Ua 1 //1PT，3CT，
#define Pt_Ub 2 //1PT，3CT，
#define Pt_Uc 3 //1PT，3CT，
#define Pt_2 4 //2PT，3CT；2PT，2CT；
#define Pt_Uab 5//1PT，2CT；
#define Pt_Ucb 6//1PT，2CT；
//2PT，3CT；2PT，2CT；1PT，2CT；1PT，3CT；3PT，3CT
#define LINE(n) (n)
#define PHASE_A 0
#define PHASE_B 1
#define PHASE_C 2
#define TOTALLPHASE 3

/*
	 * 第一字节：高四位代表类型1-直流标定，2-交流标定。
	 *           低四位代表数据类型：1-代表直流电压（0~48V），2-代表直流电流（4~20mA）。
	 * 第二字节：高四位代表通道号，每一位代表一个通道。
	 *          低四位代表标定步奏，对于直流标定分为1-代表输入最小值，
	 *                            2-输入最大值，3-结束标定。
	 * 每一步骤返回14表示标定成功，返回15表示标定失败。
*/


#define BDSTEP_1 0x2000
#define BD_DC_VOL_CH1_STEP_1 0x1111
#define BD_DC_VOL_CH1_STEP_2 0x1112
#define BD_DC_VOL_CH1_STEP_3 0x1113

#define BD_DC_VOL_CH2_STEP_1 0x1121
#define BD_DC_VOL_CH2_STEP_2 0x1122
#define BD_DC_VOL_CH2_STEP_3 0x1123

#define BD_DC_CUR_CH1_STEP_1 0x1211
#define BD_DC_CUR_CH1_STEP_2 0x1212
#define BD_DC_CUR_CH1_STEP_3 0x1213

#define BD_DC_CUR_CH2_STEP_1 0x1221
#define BD_DC_CUR_CH2_STEP_2 0x1222
#define BD_DC_CUR_CH2_STEP_3 0x1223

#define BDSTART  1
#define BDSTOP   0
#define BD_0K  14
#define BD_FAIL 15
#define BD_TIMEOVER 15
#define CLBRTTIMEOUT 10
//64 点
//#define DEVXISHU 0.02209375   // 1/(32*根号2）
//#define TWODEVXISHU 0.00048828125 // 1/(32*根号2）* 1/(32*根号2）
//32 点
#define DEVXISHU 0.044194173   // 1/(16*根号2）
#define TWODEVXISHU 0.001953125 // 1/(16*根号2）* 1/(16*根号2）

#define AVERAGENUM 10.0
#define BIAODINGMIN 100.0
#define CHUNENG 0
#define HE_HE 1  //合位置 合
#define HE_FEN 0
#define FEN_HE 0
#define FEN_FEN 1
#define NOLOCK 0

#define BRKRCDSTART 0
#define BRKRCDCHECK 1
#define BRKRCDCPL   3

#define NOCHECK 0
#define CKNOVOL 1
#define CKSYN 2
#define CKNOVOLSYN 3
#define CKNOVOLTOSYN 4 //检无压不成功自动检同期

#define YUEXIANYXNUM (256)//(160)
/*
#define YX_0801  0x801   //配电房水位告警(1)
#define YX_0802  0x802   //配电房火灾告警(1)
#define YX_0803  0x803   //配电房环境温度告警(1)
#define YX_0804  0x804   //电缆温度告警(1)
#define YX_0805  0x805   //直流母线电压越上限告警(1)
#define YX_0806  0x806   //直流母线电压越下限告警(1)
//电流电压负荷越线告警(256)+失压告警(16)+电池欠压告警(1)+终端温度告警(1)+
 */
#define MAX_YUEXIAN_SOFT_YX_NUM (YUEXIANYXNUM+MAX_LINE_NUM+8)
#define SHANG_XIAN	1
#define XIA_XIAN	2
typedef struct soe_buffer {
    uint16 CurRead;
    uint16 CurPrepare;
    SOE_DATA soe_data[MAX_SOE_NUM];
} SOE_BUFFER;

typedef struct _FAULT_BUFFER {
    uint16 CurRead;
    uint16 CurPrepare;
    FAULT_DATA fault_data[MAX_FAULT_SOE_NUM];
} FAULT_BUFFER;
typedef struct sftyxdata{
	uint16 Id;
	uint8 Data;
	uint8 Nature;
}sftyxdata_t;
typedef struct power{
	float p;
	float q;
	float s;
}power_t;
typedef struct anglevalue{
	float sin_a;
	float cos_a;
}angelvalue_t;
typedef struct harmonicdata{
	float rms;
	float rario;
}harmonicdata_t;

typedef struct harmonictab{
	DataType4 *Rms;
	DataType4 *Ratio;
	DataType4 *TotalRatio;
	uint16 YcName;
}harmonictab_t;
typedef struct maxavrmin{
    float max;
	float avr;
	float min;
}maxavrmin_t;

typedef struct maxavrminpower{
	power_t max;
	power_t avr;
	power_t min;
}maxavrminpower_t;
//定义遥控命令限定词
#define S_SELECT_HE
#define S_SELDEFUALT
#define S_EXECUTE
/*
typedef struct dco_data {
	uint8 D_S_CS:2; //1－－开，2－－合
    uint8 QU:5; //1--短脉冲，2--长脉冲，3－－持续输出
    uint8 S_E: 1; //1--选择，0－－执行
} DCO_WORD;
*/
//此定义是101规约的变形，是ARM端解析后的，ARM和DSP之间的私有协议；
#define SELECT 1
#define EXECUTE 0
#define DCONTROL 1
#define SCONTROL 0
typedef struct dco_data {
	uint8 D_S_CS:2; //1－－开，2－－合
	uint8 revl1:2;
	uint8 D_S:1; //1 双点，0单点
	uint8 revl2:2;
    uint8 S_E: 1; //1--选择，0－－执行
}DCO_WORD;
typedef union dcodata {
    DCO_WORD DcoWord;
    uint8 data;
} DCOWORD;

#define TIMECLEAR {0,0,0,0,0,0,0,}
typedef struct softyxtab{
	DataType4 *SrcYcP;
	WarnPara *LimitParaP;
	uint8 LimitType;
	sftyxdata_t DataId;
	uint8 Enable;
	set_ms_time Tcn[2];//越线时间与越线恢复时间；
	uint8 ChangeFlag;
}softyxtab_t;

typedef struct YcP{
	DataType4 *SrcP;
}YcP_T;
typedef struct ptctdata{

	YcP_T SrcUa[3];
	YcP_T SrcIa[3];
	float StartI;
	float CriticalU;
	uint16 limitT;
	set_ms_time PtT[2];//[3];
	set_ms_time CtT[2];//[3];
	set_ms_time PhT[2];//[3];
	sftyxdata_t PtSoeId;//[3];
	sftyxdata_t CtSoeId;//[3];
	sftyxdata_t PhSoeId;//[3];
	uint8 PtStatu[3];
	uint8 CtStatu[3];
}ptctdata_t;
class DeadSet: public BaseDataType
{

public:
    DeadSet() {
        DataType = 180;
    }

    struct {
        float DeadU;   ///<电压死区设置
        float DeadI;      ///<电流死区设置
        float DeadP;      ///<功率死区设置
        float DeadFreq;   ///<频率死区设置
        float DeadAngle;///<角度死区设置
        float DeadHarmoicValueU;///<谐波有效值死区设置
        float DeadHarmoicValueI;///<谐波有效值死区设置
        float DeadHarmoicCount;///<   谐波含有率死区设置
        float DeadTemp;   ///<温度死区设置
        float DeadHumidity;///<湿度死区设置
        float cos;
        uint32 DeadTime;
    }__attribute__ ((packed)) Data ;
};

class DSPDbDigitInSet: public BaseDataType
{
public:
	DSPDbDigitInSet() {
        DataType = 16;
    }

    struct {
        int8 Num;

        struct {
            int16 Id;
            int16 ClosePort;
            int16 OpenPort;
        }Define[25];
    }__attribute__ ((packed)) Data ;
};


typedef struct  _ScadaPara
{
	FaCommSet  d_5054;//保护公共定值:启动电流，失压阀值，故障复归时间；

	//DataType3 d_5056;   //故障信号复归
	//DataType3 d_5057;   //故障信号关联出口（1~4回）
	//DataType3 d_5058;   //故障信号关联出口（5~8回）
	//DataType3 d_5059;   //故障信号关联出口（9~12回）
	//DataType3 d_505A;   //故障信号关联出口（13~16回）
	//DataType3 d_505B;   //故障信号关联出口（17~20回）
	//DataType3 d_505C;   //故障信号关联出口功能允许
	DataType3 d_505D;   //出口继电器软压板
	DataType3 d_505E[5];   //分回线远方就地开关设置（1~20回）,每个地址有4回
	DataType3 d_5063;   //分回线远方就地开关设置允许
	DataType3 d_5064[3];   //出口继电器1~48类型   //两位一个继电器,每个地址有16个继电器
	DataType1 d_507F;   //<基波算法还是全波算法
	SingleDigitSet d_5080[5];   //<0x5080~0x5084模拟量输入配置 指模拟输入量的硬件与需要采集的模拟量之间联系配置,每个地址可配置50个模拟量
	SingleDigitSet d_5085[4];   //<0x5085~0x5088单点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置,每个地址可配置50个开入量.
	DbDigitSet     d_5089;   //<双点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置,每个地址可配置35个开入量
	DbDigitSet d_508A;   //开关输出量的硬件端口配置  回线遥控和公共遥控
	DataType1  d_508B;   //功率计算方法配置
	/*
	 *
	 * 	int8 DcType; 低四位第一通道，高四位第二通道，0：代表电压；1：代表电流
		int16 NominalValue; 低八位第电压 标称值，高八位电流标称值 ；
	 *
	 * 	int8 DcType; 低四位第二通道，高四位第一通道，0：代表电压；1：代表电流
		int16 NominalValue;低八位第二通道标称值，高八位第一通道标称值 ；

	 * 还是电压；默认电压。
	 */
	DcPara     d_508C;   //直流量配置
	struct {
		uint8 DcType;
		uint8 MaxStand[1];//两种类型（电压，电流）
		uint8 MinStand[1];//两种类型（电压，电流）
	}DcPara[2];//两个通道，
	DataType2  d_508D;   //电流标称值是5A还是1A,
	DataType4  d_508E;   //电压标称值 220V还是110V
	DeadAreaSet	d_508F;///<遥测死区值设置
	DataType3	d_5090;///<遥信防抖时间
	DataType3	d_5091;///<遥控返校失败取消遥控时间
	DataType3	d_5092;///<遥控保持时间
	DataType3	d_5093;///<遥控脉冲宽度
	DataType3	d_5094;///<延时断开操作电源时间
	DataType2	d_5095;///<PT
	DataType2	d_5096;///<CT
	DataType2	d_5097;///<标定
  	YCMsgSet d_5098[20];		//遥测信息体地址配置
#if 0
	//遥控信息体配置dsp侧不用理会
	YKMsgSet d_50CB;		//<遥控信息体属性配置
	YXMsgSet d_50AD[30];	//遥信信息体属性配置,在dsp侧可以不用理会
#endif 

	TianJinProjPara d_514A;   //项目参数
#if 0 
	以下参数dsp侧不用理会
	PassWdPara		d_5200;   ///**密码*/
	SwVersionPara	d_5201;///**版本*/
	DataType4		d_5202;   ///**温度定值*/
	DataType4		d_5203;   ///**湿度定值*/
	DataType4		d_5204;   ///**经度坐标*/
	DataType4		d_5205;   ///**纬度坐标*/
	DataType1		d_5206;   ///**恢复出厂设置*/
#endif
	DataType4	d_5207;   ///**蓄电池欠压切除阈值*/
	DataType4	d_5208;   ///**蓄电池低压告警阈值*/
	DataType2	d_5209;   ///**蓄电池活化充放电间隔时间*/ 分钟为单位；
	set_ms_time BatActTime;
	WarnPara	d_520A;   ///*电压上限告警阈值，恢复阈值，持续时间*/
	WarnPara	UupData;
	WarnPara	d_520B;   ///*电压下限告警阈值，恢复阈值，持续时间*/
	WarnPara	UdownData;
	WarnPara	d_520C;   ///*电流上限阈值，恢复阈值，持续时间*/
	WarnPara	IupData;
	WarnPara	d_520D;   ///*负荷越上限阈值，恢复阈值，持续时间*/
	WarnPara	SupData;
	WarnPara	d_520E;   //零序电压上限告警参数
	WarnPara	U0upData;
	WarnPara	d_520F;   //零序电流上限告警参数
	WarnPara	I0upData;
	//HisDataDefinePara d_5210;   ///*遥测历史数据项定义*/
	//DataType2 d_5211;   ///*遥测历史数据存储间隔*/
	//HisDataDefinePara d_5212;   ///*遥测极值数据项定义*/
	DataType1			d_5213;   ///*人工值设置使能*/
	UserRealDataDefPara d_5214;   ///*人工设置数据*/
	TeminalTime			d_5215;   ///*CP56Time2a格式终端时间*/
#if 0
	PassWdPara			d_5216;   //终端类型,三遥型：D30；二遥标准型：D21；二遥动作型：D22
#endif 
	DataType3			d_6801;   //32比特串

	//遥控类数据
	 DataType1 d_6001;   //断路器1（开/合）
	 DataType1 d_6002;   //断路器2（开/合）
	 DataType1 d_6003;   //断路器3（开/合）
	 DataType1 d_6004;   //断路器4（开/合）
	 DataType1 d_6005;   //断路器5（开/合）
	 DataType1 d_6006;   //断路器6（开/合）
	 DataType1 d_6007;   //断路器7（开/合）
	 DataType1 d_6008;   //断路器8（开/合）
	 DataType1 d_6009;   //断路器9（开/合）
	 DataType1 d_600A;   //断路器10（开/合）
	 DataType1 d_600B;   //断路器11（开/合）
	 DataType1 d_600C;   //断路器12（开/合）
	 DataType1 d_600D;   //断路器13（开/合）
	 DataType1 d_600E;   //断路器14（开/合）
	 DataType1 d_600F;   //断路器15（开/合）
	 DataType1 d_6010;   //断路器16（开/合）
	 DataType1 d_6064;   //电池活化
	 DataType1 d_6065;   //公共遥控1(开合,风机启停)
	 DataType1 d_6066;   //公共遥控2(开合,水泵启停)

	FaDevotionSet d_5055;///<保护功能投退设置

	//////////以下为自定义/////////

	DeadSet		Dead;///<遥测死区值设置  绝对值
	ALL_YC		YcData;
	DataType4	difangle_4401;
	DataType4	difvol_4402;
	uint16		ycavrnum;

	SOE_BUFFER		soe_buffer[MAX_USER_NUM];
	FAULT_BUFFER	fault_buffer;
	maxavrmin_t		ClbrtPortTmp[CHANNEL_NUM];
	maxavrmin_t		ClbrtPowerTmp[MAX_LINE_NUM];
	uint8			Clbrtnum;
	uint8			StartClbrt;
	uint16			StartDcClbrt;
	set_ms_time		ClbrtTime;
	set_ms_time		YKtimeOut[5];//遥控超时计时器
	set_ms_time		YkGapTime[5];//遥控间隔时间计时器
	set_ms_time		TerCheckTime;
	breakerproreg_t BrkPro[5];// 断路器在线监测

	uint8			HzCkType; //合闸检定类型
	float			DiffVol;///<开关两侧电压差
	float			DiffPhase;///<相位差设置
	DataType4		*DiffVolP[2];///指向两侧的电压；
	uint8			DiffVolName[2];//开关两侧电压定义  里面装的YC的下标号
//	终端所有越限告警
/*
	#define YX_0801  0x801   //配电房水位告警(1)
	#define YX_0802  0x802   //配电房火灾告警(1)
	#define YX_0803  0x803   //配电房环境温度告警(1)
	#define YX_0804  0x804   //电缆温度告警(1)
	#define YX_0805  0x805   //直流母线电压越上限告警(1)
	#define YX_0806  0x806   //直流母线电压越下限告警(1)
	//电流电压负荷越限告警(256)+失压告警(16)+电池欠压告警(1)+终端温度告警(1)+
 */
	softyxtab_t		YueXianYx[YUEXIANYXNUM+MAX_LINE_NUM+8];
	WarnPara		TemLimit;
	WarnPara		HumLimit;
	WarnPara		BatLowData;
	WarnPara		LoseVol;
}ScadaPara;
/**
*************************************************************************************  
*  @brief  数据采集类定义。
*/
class Scada :public BaseApp{
private: 
	ptctdata_t		PtCtReg[MAX_LINE_NUM];
	SIGRCD			FreqReg[MAX_LINE_NUM];
	DataType4		MaxVol[MAX_LINE_NUM];
	uint64			Sec;///<内部定时器id
	harmonicdata_t	GetHarmonicRmsAndRatio(float BaseHarmonicRms,float *src);
	SP_COMPLEX		ComplexRotation(SP_COMPLEX Input,float32 Angle);
	power_t			GetPower(uint8 Uname,uint8 Iname);
	angelvalue_t	CosSinz1z2(SP_COMPLEX U,SP_COMPLEX I);
	int				xiebo_len;

	typedef struct __cal_freq{
		int port;
		float freq;
	}_cal_freq;
	_cal_freq cal_freq[MAX_LINE_NUM];
	int cal_freq_num;
//about Yc 
	//void	YcPortCfgParaInit(void);
	void	YcPortConfig(void);
	float 	GetFloatMax(float ia,float ib,float ic);
	float	TotalRms(float *src , uint8 len);
	void	GetFreq(void);
	int 	CalculateFreq(int16 *src , uint8 len, SIGRCD *freq_req);
	int16	Calculate();
	void	GetModulus(void);
	void	GetRms(void);
	void	GetPQS(void);
	void	CheckChangeYc(void);
	void	ChecksoftYx(void);
	void	CheckPtCt(void);
	void	DcCalibration(void);
	void	Calibration(void);
	void	GetHarmonicData(void);
	void	PhaseChange( SP_COMPLEX *dir, SP_COMPLEX *src , uint16 len ,float offsetangle );
	void	compare(float *max,float *min,float *avr,float var,float avr_num);
	void	first_compare(float *max,float *min,float var);
	void	GetRatio(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue );
	float	GetDead(int32 DataId);
	float	GetYcAngle(uint8 YcName1,uint8 YcName2);
	float	GetCplAngle(SP_COMPLEX CplData);
	int16	FloatCompare ( float cmp1, float cmp2);
	int16	IsOverDeadband ( float value1, float value2, float db );
	int16	CheckSyn(void);
	int16	CheckNoVol(void);
	int16	YueXianYxCfgParaInit(void);
	int16	YueXianYxConfig(void);
	int16	DeadConfig(void);
	int		is_yc_need_cal(int);
//about Yx 
	void	YxPortCfgParaInit(void);
	void	YxPortConfig(void);
	int16	YxMsgAddrCfgParaInit(void);
	//int16	YxNatureUpdata(YXMsgSet *Old, YXMsgSet  *New);
	int16	SendYxMsg(void);
//about Yk 
	void	YkPortCfgInit(void);
	void	YkPortConfig(void);
	int16	BreakYkStateChk(int16 break_no,DCOWORD DCO_Word);
	int16	YkOut(int16 break_no,DCOWORD DCO_Word);
	int16	GetYkFpgaNum(uint8 YkPort);//no use
public:
	ScadaPara	scada_cfg;///<Scada配置参数
	CONFIG_YC	yc_config;
	CONFIG_YX	yx_config;
	CONFIG_YK	yk_config;
	Scada();
	~Scada(){};	
//about Yc 
	int find_channel_by_yc_id(int);
	int find_yc_by_channel(int);
	float GetFreq(int line){
		if(line<MAX_LINE_NUM)
			return FreqReg[line].Freq;
		return 5000;
	}
//about Yx 
	int16	BreakRecord(int16 BreakId,uint8 RecordStep);
	int8	GetLogicYx(uint8 data,uint8 yxtype);
	int8	YxDataInit(int);
	//int8	YxNatureInit(void);
	int16	PushSoe(uint8 User ,uint16 Id,int8 data,CP56Time2a InTime);
	int16	BreakReCloceCheck(uint8 CkTp,uint8 Enable);
	int8	YfJdCheck(Uint16 break_no);
	void	TerCheck(void);
	int16	GetBitStat(union KR_UNION Data,BitReg *Reg);
	void	TimeTick(CP56Time2a * T,Uint32 Per);
	void	SetFistYxSynFlag(Uint16 YxBuffPos,BitReg *YxReg);
	CP56Time2a GetYxSynTime(Uint16 YxBuffPos,Uint16 YxPos);
	void	GetYxDataFromYxBuff(Uint16 YxBuffPos,Uint16 YxPos);
	void	GetOldYxDataFromYxBuff(Uint16 YxBuffPos,Uint16 YxPos);
	void	GetBitData(union KR_UNION *DirYxData,union KR_UNION *SrcYxData,uint16 YxPos);
	uint16	YxBitFun(union KR_UNION *YxData,Uint64 YxMask,Uint16 YxPos,Uint16 Type);
	void	CheckYxBuff(void);
//about Yk 
	int16	ControlYk(int16 break_no,DCOWORD DCO_Word);
//common
	int16	SoftMsTime();
	int16	ReadCfg(int32 Id);
	void	doConfig();
	int16	Init();
	int16	Run();	
	void	do_test(int);
};
extern Scada scada;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* scada_main(void*);
#endif

