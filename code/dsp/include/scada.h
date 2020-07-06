#ifndef SCADA_H
#define SCADA_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到BaseApp类.

#define XIEBOLEN 19
#define SINGLEHARMONIC 1
#define COS 0
#define SIN 1
typedef struct scada_yc{
	//遥测类数据
	DataType4 ua1_4001;	///<线路1 A相电压
	DataType4 ub1_4002;	///<线路1 B相电压
	DataType4 uc1_4003;	///<线路1 C相电压
	DataType4 uab1_4004;	///<线路1 AB线电压
	DataType4 ucb1_4005;	///<线路1 CB线电压
	DataType4 u011_4006;	///<线路1 零序电压
	DataType4 ia11_4007;	///<线路1 A相测量电流
	DataType4 ib11_4008;	///<线路1 B相测量电流
	DataType4 ic11_4009;	///<线路1 C相测量电流
	DataType4 i011_400A;	///<线路1 零序电流
	DataType4 ia12_400B;	///<线路1 A相保护电流
	DataType4 ib12_400C;	///<线路1 B相保护电流
	DataType4 ic12_400D;	///<线路1 C相保护电流
	DataType4 pa1_400E;	///<线路1 A相有功功率
	DataType4 qa1_400F;	///<线路1 A相无功功率
	DataType4 cosa1_4010;	///<线路1 A相功率因数
	DataType4 pb1_4011;	///<线路1 B相有功功率
	DataType4 qb1_4012;	///<线路1 B相无功功率
	DataType4 cosb1_4013;	///<线路1 B相功率因数
	DataType4 pc1_4014;	///<线路1 C相有功功率
	DataType4 qc1_4015;	///<线路1 C相无功功率
	DataType4 cosc1_4016;	///<线路1 C相功率因数
	DataType4 P1_4017;	///<线路1 总有功功率
	DataType4 Q1_4018;	///<线路1 总无功功率
	DataType4 S1_4019;	///<线路1 总视在功率
	DataType4 COS1_401A;	///<线路1 总功率因数
	DataType4 freq1_401B;	///<线路1 频率

	DataType4 ua1rms2_401C;   //线路1 2次电压谐波有效值(以下为A相)	401C
	DataType4 d_401D;   //线路1 3次电压谐波有效值	401D
	DataType4 d_401E;   //线路1 4次电压谐波有效值	401E
	DataType4 d_401F;   //线路1 5次电压谐波有效值	401F
	DataType4 d_4020;    //线路1 6次电压谐波有效值	4020
	DataType4 d_4021;    //线路1 7次电压谐波有效值	4021
	DataType4 d_4022;    //线路1 8次电压谐波有效值	4022
	DataType4 d_4023;    //线路1 9次电压谐波有效值	4023
	DataType4 d_4024;    //线路1 10次电压谐波有效值	4024
	DataType4 d_4025;    //线路1 11次电压谐波有效值	4025
	DataType4 d_4026;    //线路1 12次电压谐波有效值	4026
	DataType4 d_4027;    //线路1 13次电压谐波有效值	4027
	DataType4 d_4028;    //线路1 14次电压谐波有效值	4028
	DataType4 d_4029;    //线路1 15次电压谐波有效值	4029
	DataType4 d_402A;    //线路1 16次电压谐波有效值	402A
	DataType4 d_402B;    //线路1 17次电压谐波有效值	402B
	DataType4 d_402C;    //线路1 18次电压谐波有效值	402C
	DataType4 d_402D;    //线路1 19次电压谐波有效值	402D
	DataType4 ia1rms2_402E;    //线路1 2次电流谐波有效值	402E
	DataType4 d_402F;    //线路1 3次电流谐波有效值	402F
	DataType4 d_4030;   //线路1 4次电流谐波有效值	4030
	DataType4 d_4031;   //线路1 5次电流谐波有效值	4031
	DataType4 d_4032;   //线路1 6次电流谐波有效值	4032
	DataType4 d_4033;   //线路1 7次电流谐波有效值	4033
	DataType4 d_4034;   //线路1 8次电流谐波有效值	4034
	DataType4 d_4035;   //线路1 9次电流谐波有效值	4035
	DataType4 d_4036;   //线路1 10次电流谐波有效值	4036
	DataType4 d_4037;   //线路1 11次电流谐波有效值	4037
	DataType4 d_4038;   //线路1 12次电流谐波有效值	4038
	DataType4 d_4039;   //线路1 13次电流谐波有效值	4039
	DataType4 d_403A;   //线路1 14次电流谐波有效值	403A
	DataType4 d_403B;   //线路1 15次电流谐波有效值	403B
	DataType4 d_403C;   //线路1 16次电流谐波有效值	403C
	DataType4 d_403D;   //线路1 17次电流谐波有效值	403D
	DataType4 d_403E;   //线路1 18次电流谐波有效值	403E
	DataType4 d_403F;   //线路1 19次电流谐波有效值	403F
	DataType4 ua1totalratio_4040;   //线路1 电压谐波总含有率	4040
	DataType4 ua1ratio2_4041;   //线路1 2次电压谐波含有率	4041
	DataType4 d_4042;   //线路1 3次电压谐波含有率	4042
	DataType4 d_4043;   //线路1 4次电压谐波含有率	4043
	DataType4 d_4044;   //线路1 5次电压谐波含有率	4044
	DataType4 d_4045;   //线路1 6次电压谐波含有率	4045
	DataType4 d_4046;   //线路1 7次电压谐波含有率	4046
	DataType4 d_4047;   //线路1 8次电压谐波含有率	4047
	DataType4 d_4048;   //线路1 9次电压谐波含有率	4048
	DataType4 d_4049;   //线路1 10次电压谐波含有率	4049
	DataType4 d_404A;   //线路1 11次电压谐波含有率	404A
	DataType4 d_404B;   //线路1 12次电压谐波含有率	404B
	DataType4 d_404C;   //线路1 13次电压谐波含有率	404C
	DataType4 d_404D;   //线路1 14次电压谐波含有率	404D
	DataType4 d_404E;   //线路1 15次电压谐波含有率	404E
	DataType4 d_404F;   //线路1 16次电压谐波含有率	404F
	DataType4 d_4050;   //线路1 17次电压谐波含有率	4050
	DataType4 d_4051;   //线路1 18次电压谐波含有率	4051
	DataType4 d_4052;   //线路1 19次电压谐波含有率	4052
	DataType4 ia1totalratio_4053;   //线路1 电流谐波总含有率	4053
	DataType4 ia1ratio2_4054;   //线路1 2次电流谐波含有率	4054
	DataType4 d_4055;   //线路1 3次电流谐波含有率	4055
	DataType4 d_4056;   //线路1 4次电流谐波含有率	4056
	DataType4 d_4057;   //线路1 5次电流谐波含有率	4057
	DataType4 d_4058;   //线路1 6次电流谐波含有率	4058
	DataType4 d_4059;   //线路1 7次电流谐波含有率	4059
	DataType4 d_405A;   //线路1 8次电流谐波含有率	405A
	DataType4 d_405B;   //线路1 9次电流谐波含有率	405B
	DataType4 d_405C;   //线路1 10次电流谐波含有率	405C
	DataType4 d_405D;   //线路1 11次电流谐波含有率	405D
	DataType4 d_405E;   //线路1 12次电流谐波含有率	405E
	DataType4 d_405F;   //线路1 13次电流谐波含有率	405F
	DataType4 d_4060;   //线路1 14次电流谐波含有率	4060
	DataType4 d_4061;   //线路1 15次电流谐波含有率	4061
	DataType4 d_4062;   //线路1 16次电流谐波含有率	4062
	DataType4 d_4063;   //线路1 17次电流谐波含有率	4063
	DataType4 d_4064;   //线路1 18次电流谐波含有率	4064
	DataType4 d_4065;   //线路1 19次电流谐波含有率	4065
	DataType4 ub1rms2_4066;   //线路1 2次电压谐波有效值(以下为b相)	4066
	DataType4 d_4067;   //线路1 3次电压谐波有效值	4067
	DataType4 d_4068;   //线路1 4次电压谐波有效值	4068
	DataType4 d_4069;   //线路1 5次电压谐波有效值	4069
	DataType4 d_406A;   //线路1 6次电压谐波有效值	406A
	DataType4 d_406B;   //线路1 7次电压谐波有效值	406B
	DataType4 d_406C;   //线路1 8次电压谐波有效值	406C
	DataType4 d_406D;   //线路1 9次电压谐波有效值	406D
	DataType4 d_406E;   //线路1 10次电压谐波有效值	406E
	DataType4 d_406F;   //线路1 11次电压谐波有效值	406F
	DataType4 d_4070;   //线路1 12次电压谐波有效值	4070
	DataType4 d_4071;   //线路1 13次电压谐波有效值	4071
	DataType4 d_4072;   //线路1 14次电压谐波有效值	4072
	DataType4 d_4073;   //线路1 15次电压谐波有效值	4073
	DataType4 d_4074;   //线路1 16次电压谐波有效值	4074
	DataType4 d_4075;   //线路1 17次电压谐波有效值	4075
	DataType4 d_4076;   //线路1 18次电压谐波有效值	4076
	DataType4 d_4077;   //线路1 19次电压谐波有效值	4077
	DataType4 ib1rms2_4078;   //线路1 2次电流谐波有效值	4078
	DataType4 d_4079;   //线路1 3次电流谐波有效值	4079
	DataType4 d_407A;   //线路1 4次电流谐波有效值	407A
	DataType4 d_407B;   //线路1 5次电流谐波有效值	407B
	DataType4 d_407C;   //线路1 6次电流谐波有效值	407C
	DataType4 d_407D;   //线路1 7次电流谐波有效值	407D
	DataType4 d_407E;   //线路1 8次电流谐波有效值	407E
	DataType4 d_407F;   //线路1 9次电流谐波有效值	407F
	DataType4 d_4080;   //线路1 10次电流谐波有效值	4080
	DataType4 d_4081;   //线路1 11次电流谐波有效值	4081
	DataType4 d_4082;   //线路1 12次电流谐波有效值	4082
	DataType4 d_4083;   //线路1 13次电流谐波有效值	4083
	DataType4 d_4084;   //线路1 14次电流谐波有效值	4084
	DataType4 d_4085;   //线路1 15次电流谐波有效值	4085
	DataType4 d_4086;   //线路1 16次电流谐波有效值	4086
	DataType4 d_4087;   //线路1 17次电流谐波有效值	4087
	DataType4 d_4088;   //线路1 18次电流谐波有效值	4088
	DataType4 d_4089;   //线路1 19次电流谐波有效值	4089
	DataType4 ub1totalratio_408A;   //线路1 电压谐波总含有率	408A
	DataType4 ub1ratio2_408B;   //线路1 2次电压谐波含有率	408B
	DataType4 d_408C;   //线路1 3次电压谐波含有率	408C
	DataType4 d_408D;   //线路1 4次电压谐波含有率	408D
	DataType4 d_408E;   //线路1 5次电压谐波含有率	408E
	DataType4 d_408F;   //线路1 6次电压谐波含有率	408F
	DataType4 d_4090;   //线路1 7次电压谐波含有率	4090
	DataType4 d_4091;   //线路1 8次电压谐波含有率	4091
	DataType4 d_4092;   //线路1 9次电压谐波含有率	4092
	DataType4 d_4093;   //线路1 10次电压谐波含有率	4093
	DataType4 d_4094;   //线路1 11次电压谐波含有率	4094
	DataType4 d_4095;   //线路1 12次电压谐波含有率	4095
	DataType4 d_4096;   //线路1 13次电压谐波含有率	4096
	DataType4 d_4097;   //线路1 14次电压谐波含有率	4097
	DataType4 d_4098;   //线路1 15次电压谐波含有率	4098
	DataType4 d_4099;   //线路1 16次电压谐波含有率	4099
	DataType4 d_409A;   //线路1 17次电压谐波含有率	409A
	DataType4 d_409B;   //线路1 18次电压谐波含有率	409B
	DataType4 d_409C;   //线路1 19次电压谐波含有率	409C
	DataType4 ib1totalratio_409D;   //线路1 电流谐波总含有率	409D
	DataType4 ib1ratio2_409E;   //线路1 2次电流谐波含有率	409E
	DataType4 d_409F;   //线路1 3次电流谐波含有率	409F
	DataType4 d_40A0;   //线路1 4次电流谐波含有率	40A0
	DataType4 d_40A1;   //线路1 5次电流谐波含有率	40A1
	DataType4 d_40A2;   //线路1 6次电流谐波含有率	40A2
	DataType4 d_40A3;   //线路1 7次电流谐波含有率	40A3
	DataType4 d_40A4;   //线路1 8次电流谐波含有率	40A4
	DataType4 d_40A5;   //线路1 9次电流谐波含有率	40A5
	DataType4 d_40A6;   //线路1 10次电流谐波含有率	40A6
	DataType4 d_40A7;   //线路1 11次电流谐波含有率	40A7
	DataType4 d_40A8;   //线路1 12次电流谐波含有率	40A8
	DataType4 d_40A9;   //线路1 13次电流谐波含有率	40A9
	DataType4 d_40AA;   //线路1 14次电流谐波含有率	40AA
	DataType4 d_40AB;   //线路1 15次电流谐波含有率	40AB
	DataType4 d_40AC;   //线路1 16次电流谐波含有率	40AC
	DataType4 d_40AD;   //线路1 17次电流谐波含有率	40AD
	DataType4 d_40AE;   //线路1 18次电流谐波含有率	40AE
	DataType4 d_40AF;   //线路1 19次电流谐波含有率	40AF
	DataType4 uc1rms2_40B0;   //	线路1 2次电压谐波有效值(以下为c相)	40B0
	DataType4 d_40B1;   //线路1 3次电压谐波有效值	40B1
	DataType4 d_40B2;   //线路1 4次电压谐波有效值	40B2
	DataType4 d_40B3;   //线路1 5次电压谐波有效值	40B3
	DataType4 d_40B4;   //线路1 6次电压谐波有效值	40B4
	DataType4 d_40B5;   //线路1 7次电压谐波有效值	40B5
	DataType4 d_40B6;   //线路1 8次电压谐波有效值	40B6
	DataType4 d_40B7;   //线路1 9次电压谐波有效值	40B7
	DataType4 d_40B8;   //线路1 10次电压谐波有效值	40B8
	DataType4 d_40B9;   //线路1 11次电压谐波有效值	40B9
	DataType4 d_40BA;   //线路1 12次电压谐波有效值	40BA
	DataType4 d_40BB;   //线路1 13次电压谐波有效值	40BB
	DataType4 d_40BC;   //线路1 14次电压谐波有效值	40BC
	DataType4 d_40BD;   //线路1 15次电压谐波有效值	40BD
	DataType4 d_40BE;   //线路1 16次电压谐波有效值	40BE
	DataType4 d_40BF;   //线路1 17次电压谐波有效值	40BF
	DataType4 d_40C0;   //线路1 18次电压谐波有效值	40C0
	DataType4 d_40C1;   //线路1 19次电压谐波有效值	40C1
	DataType4 ic1rms2_40C2;   //线路1 2次电流谐波有效值	40C2
	DataType4 d_40C3;   //线路1 3次电流谐波有效值	40C3
	DataType4 d_40C4;   //线路1 4次电流谐波有效值	40C4
	DataType4 d_40C5;   //线路1 5次电流谐波有效值	40C5
	DataType4 d_40C6;   //线路1 6次电流谐波有效值	40C6
	DataType4 d_40C7;   //线路1 7次电流谐波有效值	40C7
	DataType4 d_40C8;   //线路1 8次电流谐波有效值	40C8
	DataType4 d_40C9;   //线路1 9次电流谐波有效值	40C9
	DataType4 d_40CA;   //线路1 10次电流谐波有效值	40CA
	DataType4 d_40CB;   //线路1 11次电流谐波有效值	40CB
	DataType4 d_40CC;   //线路1 12次电流谐波有效值	40CC
	DataType4 d_40CD;   //线路1 13次电流谐波有效值	40CD
	DataType4 d_40CE;   //线路1 14次电流谐波有效值	40CE
	DataType4 d_40CF;   //线路1 15次电流谐波有效值	40CF
	DataType4 d_40D0;   //线路1 16次电流谐波有效值	40D0
	DataType4 d_40D1;   //线路1 17次电流谐波有效值	40D1
	DataType4 d_40D2;   //线路1 18次电流谐波有效值	40D2
	DataType4 d_40D3;   //线路1 19次电流谐波有效值	40D3
	DataType4 uc1totalratio_40D4;   //线路1 电压谐波总含有率	40D4
	DataType4 uc1ratio2_40D5;   //线路1 2次电压谐波含有率	40D5
	DataType4 d_40D6;   //线路1 3次电压谐波含有率	40D6
	DataType4 d_40D7;   //线路1 4次电压谐波含有率	40D7
	DataType4 d_40D8;   //线路1 5次电压谐波含有率	40D8
	DataType4 d_40D9;   //线路1 6次电压谐波含有率	40D9
	DataType4 d_40DA;   //线路1 7次电压谐波含有率	40DA
	DataType4 d_40DB;   //线路1 8次电压谐波含有率	40DB
	DataType4 d_40DC;   //线路1 9次电压谐波含有率	40DC
	DataType4 d_40DD;   //线路1 10次电压谐波含有率	40DD
	DataType4 d_40DE;   //线路1 11次电压谐波含有率	40DE
	DataType4 d_40DF;   //线路1 12次电压谐波含有率	40DF
	DataType4 d_40E0;   //线路1 13次电压谐波含有率	40E0
	DataType4 d_40E1;   //线路1 14次电压谐波含有率	40E1
	DataType4 d_40E2;   //线路1 15次电压谐波含有率	40E2
	DataType4 d_40E3;   //线路1 16次电压谐波含有率	40E3
	DataType4 d_40E4;   //线路1 17次电压谐波含有率	40E4
	DataType4 d_40E5;   //线路1 18次电压谐波含有率	40E5
	DataType4 d_40E6;   //线路1 19次电压谐波含有率	40E6
	DataType4 ic1totalratio_40E7;   //线路1 电流谐波总含有率	40E7
	DataType4 ic1ratio2_40E8;   //线路1 2次电流谐波含有率	40E8
	DataType4 d_40E9;   //线路1 3次电流谐波含有率	40E9
	DataType4 d_40EA;   //线路1 4次电流谐波含有率	40EA
	DataType4 d_40EB;   //线路1 5次电流谐波含有率	40EB
	DataType4 d_40EC;   //线路1 6次电流谐波含有率	40EC
	DataType4 d_40ED;   //线路1 7次电流谐波含有率	40ED
	DataType4 d_40EE;   //线路1 8次电流谐波含有率	40EE
	DataType4 d_40EF;   //线路1 9次电流谐波含有率	40EF
	DataType4 d_40F0;   //线路1 10次电流谐波含有率	40F0
	DataType4 d_40F1;   //线路1 11次电流谐波含有率	40F1
	DataType4 d_40F2;   //线路1 12次电流谐波含有率	40F2
	DataType4 d_40F3;   //线路1 13次电流谐波含有率	40F3
	DataType4 d_40F4;   //线路1 14次电流谐波含有率	40F4
	DataType4 d_40F5;   //线路1 15次电流谐波含有率	40F5
	DataType4 d_40F6;   //线路1 16次电流谐波含有率	40F6
	DataType4 d_40F7;   //线路1 17次电流谐波含有率	40F7
	DataType4 d_40F8;   //线路1 18次电流谐波含有率	40F8
	DataType4 d_40F9;   //线路1 19次电流谐波含有率	40F9
	DataType4 ua2_40FA;	///<线路2 A相电压
	DataType4 ub2_40FB;	///<线路2 B相电压
	DataType4 uc2_40FC;	///<线路2 C相电压
	DataType4 uab2_40FD;	///<线路2 AB线电压
	DataType4 ucb2_40FE;	///<线路2 CB线电压
	DataType4 u02_40FF;	///<线路2 零序电压
	DataType4 ia21_4100;	///<线路2 A相测量电流
	DataType4 ib21_4101;	///<线路2 B相测量电流
	DataType4 ic21_4102;	///<线路2 C相测量电流
	DataType4 i021_4103;	///<线路2 零序电流
	DataType4 ia22_4104;	///<线路2 A相保护电流
	DataType4 ib22_4105;	///<线路2 B相保护电流
	DataType4 ic22_4106;	///<线路2 C相保护电流
	DataType4 pa2_4107;	///<线路2 A相有功功率
	DataType4 qa2_4108;	///<线路2 A相无功功率
	DataType4 cosa2_4109;	///<线路2 A相功率因数
	DataType4 pb2_410A;	///<线路2 B相有功功率
	DataType4 qb2_410B;	///<线路2 B相无功功率
	DataType4 cosc2_410C;	///<线路2 B相功率因数
	DataType4 pc2_410D;	///<线路2 C相有功功率
	DataType4 qc2_410E;	///<线路2 C相无功功率
	DataType4 cosc2_410F;	///<线路2 C相功率因数
	DataType4 P2_4110;	///<线路2 总有功功率
	DataType4 Q2_4111;	///<线路2 总无功功率
	DataType4 S2_4112;	///<线路2 总视在功率
	DataType4 COS2_4113;	///<线路2 总功率因数
	DataType4 freq2_4114;	///<线路2 频率
	DataType4 ua2rms2_4115;   //线路2 2次电压谐波有效值(以下为A相)	4115
	DataType4 d_4116;   //线路2 3次电压谐波有效值	4116
	DataType4 d_4117;   //线路2 4次电压谐波有效值	4117
	DataType4 d_4118;   //线路2 5次电压谐波有效值	4118
	DataType4 d_4119;   //线路2 6次电压谐波有效值	4119
	DataType4 d_411A;   //线路2 7次电压谐波有效值	411A
	DataType4 d_411B;   //线路2 8次电压谐波有效值	411B
	DataType4 d_411C;   //线路2 9次电压谐波有效值	411C
	DataType4 d_411D;   //线路2 10次电压谐波有效值	411D
	DataType4 d_411E;   //线路2 11次电压谐波有效值	411E
	DataType4 d_411F;   //线路2 12次电压谐波有效值	411F
	DataType4 d_4120;   //线路2 13次电压谐波有效值	4120
	DataType4 d_4121;   //线路2 14次电压谐波有效值	4121
	DataType4 d_4122;   //线路2 15次电压谐波有效值	4122
	DataType4 d_4123;   //线路2 16次电压谐波有效值	4123
	DataType4 d_4124;   //线路2 17次电压谐波有效值	4124
	DataType4 d_4125;   //线路2 18次电压谐波有效值	4125
	DataType4 d_4126;   //线路2 19次电压谐波有效值	4126
	DataType4 ia2rms2_4127;   //线路2 2次电流谐波有效值	4127
	DataType4 d_4128;   //线路2 3次电流谐波有效值	4128
	DataType4 d_4129;   //线路2 4次电流谐波有效值	4129
	DataType4 d_412A;   //线路2 5次电流谐波有效值	412A
	DataType4 d_412B;   //线路2 6次电流谐波有效值	412B
	DataType4 d_412C;   //线路2 7次电流谐波有效值	412C
	DataType4 d_412D;   //线路2 8次电流谐波有效值	412D
	DataType4 d_412E;   //线路2 9次电流谐波有效值	412E
	DataType4 d_412F;   //线路2 10次电流谐波有效值	412F
	DataType4 d_4130;   //线路2 11次电流谐波有效值	4130
	DataType4 d_4131;   //线路2 12次电流谐波有效值	4131
	DataType4 d_4132;   //线路2 13次电流谐波有效值	4132
	DataType4 d_4133;   //线路2 14次电流谐波有效值	4133
	DataType4 d_4134;   //线路2 15次电流谐波有效值	4134
	DataType4 d_4135;   //线路2 16次电流谐波有效值	4135
	DataType4 d_4136;   //线路2 17次电流谐波有效值	4136
	DataType4 d_4137;   //线路2 18次电流谐波有效值	4137
	DataType4 d_4138;   //线路2 19次电流谐波有效值	4138
	DataType4 ua2totalratio_4139;   //	线路2 电压谐波总含有率	4139
	DataType4 ua2ratio2_413A;   //线路2 2次电压谐波含有率	413A
	DataType4 d_413B;   //线路2 3次电压谐波含有率	413B
	DataType4 d_413C;   //线路2 4次电压谐波含有率	413C
	DataType4 d_413D;   //线路2 5次电压谐波含有率	413D
	DataType4 d_413E;   //线路2 6次电压谐波含有率	413E
	DataType4 d_413F;   //线路2 7次电压谐波含有率	413F
	DataType4 d_4140;   //线路2 8次电压谐波含有率	4140
	DataType4 d_4141;   //线路2 9次电压谐波含有率	4141
	DataType4 d_4142;   //线路2 10次电压谐波含有率	4142
	DataType4 d_4143;   //线路2 11次电压谐波含有率	4143
	DataType4 d_4144;   //线路2 12次电压谐波含有率	4144
	DataType4 d_4145;   //线路2 13次电压谐波含有率	4145
	DataType4 d_4146;   //线路2 14次电压谐波含有率	4146
	DataType4 d_4147;   //线路2 15次电压谐波含有率	4147
	DataType4 d_4148;   //线路2 16次电压谐波含有率	4148
	DataType4 d_4149;   //线路2 17次电压谐波含有率	4149
	DataType4 d_414A;   //线路2 18次电压谐波含有率	414A
	DataType4 d_414B;   //线路2 19次电压谐波含有率	414B
	DataType4 ia2totalratio_414C;   //线路2 电流谐波总含有率	414C
	DataType4 ia2ratio2_414D;   //线路2 2次电流谐波含有率	414D
	DataType4 d_414E;   //线路2 3次电流谐波含有率	414E
	DataType4 d_414F;   //线路2 4次电流谐波含有率	414F
	DataType4 d_4150;   //线路2 5次电流谐波含有率	4150
	DataType4 d_4151;   //线路2 6次电流谐波含有率	4151
	DataType4 d_4152;   //线路2 7次电流谐波含有率	4152
	DataType4 d_4153;   //线路2 8次电流谐波含有率	4153
	DataType4 d_4154;   //线路2 9次电流谐波含有率	4154
	DataType4 d_4155;   //线路2 10次电流谐波含有率	4155
	DataType4 d_4156;   //线路2 11次电流谐波含有率	4156
	DataType4 d_4157;   //线路2 12次电流谐波含有率	4157
	DataType4 d_4158;   //线路2 13次电流谐波含有率	4158
	DataType4 d_4159;   //线路2 14次电流谐波含有率	4159
	DataType4 d_415A;   //线路2 15次电流谐波含有率	415A
	DataType4 d_415B;   //线路2 16次电流谐波含有率	415B
	DataType4 d_415C;   //线路2 17次电流谐波含有率	415C
	DataType4 d_415D;   //线路2 18次电流谐波含有率	415D
	DataType4 d_415E;   //线路2 19次电流谐波含有率	415E
	DataType4 ub2rms2_415F;//线路2 2次电压谐波有效值(以下为b相)	415F
	DataType4 d_4160;   //线路2 3次电压谐波有效值	4160
	DataType4 d_4161;   //线路2 4次电压谐波有效值	4161
	DataType4 d_4162;   //线路2 5次电压谐波有效值	4162
	DataType4 d_4163;   //线路2 6次电压谐波有效值	4163
	DataType4 d_4164;   //线路2 7次电压谐波有效值	4164
	DataType4 d_4165;   //线路2 8次电压谐波有效值	4165
	DataType4 d_4166;   //线路2 9次电压谐波有效值	4166
	DataType4 d_4167;   //线路2 10次电压谐波有效值	4167
	DataType4 d_4168;   //线路2 11次电压谐波有效值	4168
	DataType4 d_4169;   //线路2 12次电压谐波有效值	4169
	DataType4 d_416A;   //线路2 13次电压谐波有效值	416A
	DataType4 d_416B;   //线路2 14次电压谐波有效值	416B
	DataType4 d_416C;   //线路2 15次电压谐波有效值	416C
	DataType4 d_416D;   //线路2 16次电压谐波有效值	416D
	DataType4 d_416E;   //线路2 17次电压谐波有效值	416E
	DataType4 d_416F;   //线路2 18次电压谐波有效值	416F
	DataType4 d_4170;   //线路2 19次电压谐波有效值	4170
	DataType4 ib2rms2_4171;//线路2 2次电流谐波有效值	4171
	DataType4 d_4172;   //线路2 3次电流谐波有效值	4172
	DataType4 d_4173;   //线路2 4次电流谐波有效值	4173
	DataType4 d_4174;   //线路2 5次电流谐波有效值	4174
	DataType4 d_4175;   //线路2 6次电流谐波有效值	4175
	DataType4 d_4176;   //线路2 7次电流谐波有效值	4176
	DataType4 d_4177;   //线路2 8次电流谐波有效值	4177
	DataType4 d_4178;   //线路2 9次电流谐波有效值	4178
	DataType4 d_4179;   //线路2 10次电流谐波有效值	4179
	DataType4 d_417A;   //线路2 11次电流谐波有效值	417A
	DataType4 d_417B;   //线路2 12次电流谐波有效值	417B
	DataType4 d_417C;   //线路2 13次电流谐波有效值	417C
	DataType4 d_417D;   //线路2 14次电流谐波有效值	417D
	DataType4 d_417E;   //线路2 15次电流谐波有效值	417E
	DataType4 d_417F;   //线路2 16次电流谐波有效值	417F
	DataType4 d_4180;   //线路2 17次电流谐波有效值	4180
	DataType4 d_4181;   //线路2 18次电流谐波有效值	4181
	DataType4 d_4182;   //线路2 19次电流谐波有效值	4182
	DataType4 ub2totalratio_4183;   //线路2 电压谐波总含有率	4183
	DataType4 ub2ratio2_4184;   //线路2 2次电压谐波含有率	4184
	DataType4 d_4185;   //线路2 3次电压谐波含有率	4185
	DataType4 d_4186;   //线路2 4次电压谐波含有率	4186
	DataType4 d_4187;   //	线路2 5次电压谐波含有率	4187
	DataType4 d_4188;   //线路2 6次电压谐波含有率	4188
	DataType4 d_4189;   //线路2 7次电压谐波含有率	4189
	DataType4 d_418A;   //线路2 8次电压谐波含有率	418A
	DataType4 d_418B;   //线路2 9次电压谐波含有率	418B
	DataType4 d_418C;   //线路2 10次电压谐波含有率	418C
	DataType4 d_418D;   //线路2 11次电压谐波含有率	418D
	DataType4 d_418E;   //线路2 12次电压谐波含有率	418E
	DataType4 d_418F;   //线路2 13次电压谐波含有率	418F
	DataType4 d_4190;   //线路2 14次电压谐波含有率	4190
	DataType4 d_4191;   //线路2 15次电压谐波含有率	4191
	DataType4 d_4192;   //线路2 16次电压谐波含有率	4192
	DataType4 d_4193;   //线路2 17次电压谐波含有率	4193
	DataType4 d_4194;   //线路2 18次电压谐波含有率	4194
	DataType4 d_4195;   //线路2 19次电压谐波含有率	4195
	DataType4 ib2totalratio_4196;   //线路2 电流谐波总含有率	4196
	DataType4 ib2ratio_4197;   //线路2 2次电流谐波含有率	4197
	DataType4 d_4198;   //线路2 3次电流谐波含有率	4198
	DataType4 d_4199;   //线路2 4次电流谐波含有率	4199
	DataType4 d_419A;   //线路2 5次电流谐波含有率	419A
	DataType4 d_419B;   //线路2 6次电流谐波含有率	419B
	DataType4 d_419C;   //线路2 7次电流谐波含有率	419C
	DataType4 d_419D;   //线路2 8次电流谐波含有率	419D
	DataType4 d_419E;   //线路2 9次电流谐波含有率	419E
	DataType4 d_419F;   //线路2 10次电流谐波含有率	419F
	DataType4 d_41A0;   //线路2 11次电流谐波含有率	41A0
	DataType4 d_41A1;   //线路2 12次电流谐波含有率	41A1
	DataType4 d_41A2;   //线路2 13次电流谐波含有率	41A2
	DataType4 d_41A3;   //线路2 14次电流谐波含有率	41A3
	DataType4 d_41A4;   //线路2 15次电流谐波含有率	41A4
	DataType4 d_41A5;   //线路2 16次电流谐波含有率	41A5
	DataType4 d_41A6;   //线路2 17次电流谐波含有率	41A6
	DataType4 d_41A7;   //线路2 18次电流谐波含有率	41A7
	DataType4 d_41A8;   //线路2 19次电流谐波含有率	41A8
	DataType4 uc2rms2_41A9;   //线路2 2次电压谐波有效值(以下为c相)	41A9
	DataType4 d_41AA;   //线路2 3次电压谐波有效值	41AA
	DataType4 d_41AB;   //线路2 4次电压谐波有效值	41AB
	DataType4 d_41AC;   //线路2 5次电压谐波有效值	41AC
	DataType4 d_41AD;   //线路2 6次电压谐波有效值	41AD
	DataType4 d_41AE;   //线路2 7次电压谐波有效值	41AE
	DataType4 d_41AF;   //线路2 8次电压谐波有效值	41AF
	DataType4 d_41B0;   //线路2 9次电压谐波有效值	41B0
	DataType4 d_41B1;   //线路2 10次电压谐波有效值	41B1
	DataType4 d_41B2;   //线路2 11次电压谐波有效值	41B2
	DataType4 d_41B3;   //线路2 12次电压谐波有效值	41B3
	DataType4 d_41B4;   //线路2 13次电压谐波有效值	41B4
	DataType4 d_41B5;   //线路2 14次电压谐波有效值	41B5
	DataType4 d_41B6;   //线路2 15次电压谐波有效值	41B6
	DataType4 d_41B7;   //线路2 16次电压谐波有效值	41B7
	DataType4 d_41B8;   //线路2 17次电压谐波有效值	41B8
	DataType4 d_41B9;   //线路2 18次电压谐波有效值	41B9
	DataType4 d_41BA;   //线路2 19次电压谐波有效值	41BA
	DataType4 ic2rms2_41BB;   //线路2 2次电流谐波有效值	41BB
	DataType4 d_41BC;   //线路2 3次电流谐波有效值	41BC
	DataType4 d_41BD;   //线路2 4次电流谐波有效值	41BD
	DataType4 d_41BE;   //线路2 5次电流谐波有效值	41BE
	DataType4 d_41BF;   //线路2 6次电流谐波有效值	41BF
	DataType4 d_41C0;   //线路2 7次电流谐波有效值	41C0
	DataType4 d_41C1;   //线路2 8次电流谐波有效值	41C1
	DataType4 d_41C2;   //线路2 9次电流谐波有效值	41C2
	DataType4 d_41C3;   //线路2 10次电流谐波有效值	41C3
	DataType4 d_41C4;   //线路2 11次电流谐波有效值	41C4
	DataType4 d_41C5;   //线路2 12次电流谐波有效值	41C5
	DataType4 d_41C6;   //线路2 13次电流谐波有效值	41C6
	DataType4 d_41C7;   //线路2 14次电流谐波有效值	41C7
	DataType4 d_41C8;   //线路2 15次电流谐波有效值	41C8
	DataType4 d_41C9;  //线路2 16次电流谐波有效值	41C9
	DataType4 d_41CA;   //线路2 17次电流谐波有效值	41CA
	DataType4 d_41CB;   //线路2 18次电流谐波有效值	41CB
	DataType4 d_41CC;   //线路2 19次电流谐波有效值	41CC
	DataType4 uc2totalratio_41CD;   //线路2 电压谐波总含有率	41CD
	DataType4 uc2ratio2_41CE;   //线路2 2次电压谐波含有率	41CE
	DataType4 d_41CF;   //线路2 3次电压谐波含有率	41CF
	DataType4 d_41D0;   //线路2 4次电压谐波含有率	41D0
	DataType4 d_41D1;   //线路2 5次电压谐波含有率	41D1
	DataType4 d_41D2;   //线路2 6次电压谐波含有率	41D2
	DataType4 d_41D3;   //线路2 7次电压谐波含有率	41D3
	DataType4 d_41D4;   //线路2 8次电压谐波含有率	41D4
	DataType4 d_41D5;   //线路2 9次电压谐波含有率	41D5
	DataType4 d_41D6;   //线路2 10次电压谐波含有率	41D6
	DataType4 d_41D7;   //线路2 11次电压谐波含有率	41D7
	DataType4 d_41D8;   //线路2 12次电压谐波含有率	41D8
	DataType4 d_41D9;   //线路2 13次电压谐波含有率	41D9
	DataType4 d_41DA;   //线路2 14次电压谐波含有率	41DA
	DataType4 d_41DB;   //线路2 15次电压谐波含有率	41DB
	DataType4 d_41DC;   //线路2 16次电压谐波含有率	41DC
	DataType4 d_41DD;   //线路2 17次电压谐波含有率	41DD
	DataType4 d_41DE;   //线路2 18次电压谐波含有率	41DE
	DataType4 d_41DF;   //线路2 19次电压谐波含有率	41DF
	DataType4 ic2totalratio_41E0;   //线路2 电流谐波总含有率	41E0
	DataType4 ic2ratio2_41E1;   //线路2 2次电流谐波含有率	41E1
	DataType4 d_41E2;   //线路2 3次电流谐波含有率	41E2
	DataType4 d_41E3;   //线路2 4次电流谐波含有率	41E3
	DataType4 d_41E4;   //线路2 5次电流谐波含有率	41E4
	DataType4 d_41E5;   //线路2 6次电流谐波含有率	41E5
	DataType4 d_41E6;   //线路2 7次电流谐波含有率	41E6
	DataType4 d_41E7;   //线路2 8次电流谐波含有率	41E7
	DataType4 d_41E8;   //线路2 9次电流谐波含有率	41E8
	DataType4 d_41E9;   //线路2 10次电流谐波含有率	41E9
	DataType4 d_41EA;   //线路2 11次电流谐波含有率	41EA
	DataType4 d_41EB;   //线路2 12次电流谐波含有率	41EB
	DataType4 d_41EC;   //线路2 13次电流谐波含有率	41EC
	DataType4 d_41ED;   //线路2 14次电流谐波含有率	41ED
	DataType4 d_41EE;   //线路2 15次电流谐波含有率	41EE
	DataType4 d_41EF;   //线路2 16次电流谐波含有率	41EF
	DataType4 d_41F0;   //线路2 17次电流谐波含有率	41F0
	DataType4 d_41F1;   //线路2 18次电流谐波含有率	41F1
	DataType4 d_41F2;   //线路2 19次电流谐波含有率	41F2


	DataType4 difangle_4401;///<两线路相角差
	DataType4 difvol_4402;///<两线路电压差

	DataType4 d_4601;///<GPS经度
	DataType4 d_4602;///<GPS纬度
	DataType4 d_4603;///<直流量1
	DataType4 d_4604;///<直流量2
    DataType4 d_4605;///<蓄电池电压
    DataType4 tem_4606;///<温度
    DataType4 hum_4607;///<湿度

}scada_yc_t;



#define Pt_3 0
#define Pt_Ua 1
#define Pt_Ub 2
#define Pt_Uc 3
#define Pt_2 4
#define Pt_Uab 5
#define Pt_Ucb 6
//2PT，3CT；2PT，2CT；1PT，2CT；1PT，3CT；3PT，3CT
#define U_60ANGLE 1.0471953
#define U_120ANGLE 2.094395
#define U_30ANGLE 0.52359877
#define CHANAL_ONE 0
#define CHANAL_TWO 1
#define CHANAL_THREE 2
#define LINE1 0
#define LINE2 1
#define LINE(n) (n)
#define PHASE_A 0
#define PHASE_B 1
#define PHASE_C 2
#define TOTALLPHASE 3
#define U300RATIO 0.011262827952//0.009155552842
#define I10RATIO  0.000305185094
#define I100RATIO 0.003051850947
#define BDSTEP_1 1
#define BDSTART  1
#define BDSTOP   0
//#define BD_0K  1
#define BD_TIMEOVER 0xff
#define CLBRTTIMEOUT 10
#define DEVXISHU 0.02209375   // 1/(32*根号2）
#define TWODEVXISHU 0.00048828125 // 1/(32*根号2）* 1/(32*根号2）
#define AVERAGENUM 10.0
#define BIAODINGMIN 100.0
#define CHUNENG 0
#define HE_HE 1  //合位置 合
#define HE_FEN 0
#define FEN_HE 0
#define FEN_FEN 1
#define NOLOCK 0

#define CAUSE_Per_Cyc  1
#define CAUSE_Back    2
#define CAUSE_Spont    3
#define CAUSE_Init    4
#define CAUSE_Req    5
#define CAUSE_Act    6
#define CAUSE_Actcon   7
#define CAUSE_Deact    8
#define CAUSE_Deactcon   9
#define CAUSE_Actterm   10
#define CAUSE_Retrem   11
#define CAUSE_Retloc   12
#define CAUSE_File    13
#define BRKRCDSTART 0
#define BRKRCDCHECK 1
#define BRKRCDCPL   3

#define NOCHECK 0
#define CKNOVOL 1
#define CKSYN 2
#define CKNOVOLSYN 3

#define YUEXIANYXNUM 0x16
#define SHANGXIAN 1
#define XIAXIAN 2


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
	uint8 YcName;
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

#define TIMECLEAR {0,0,0,0,0,0,0,}

typedef struct  _ScadaPara
{
	DataType1 	    d_5080;///<电压电流有效值计算方法  0全波 1 基波
	UserPortSet  	d_5081;///<模拟量输入配置
	PowerWay		d_508B;///<功率计算方法配置
	DataType1		d_508C;///<直流输入量配置
	DataType1		d_508D;///<电流标称值
	DataType1		d_508E;///<电压标称值
	DeadAreaSet		d_508F;///<遥测死区值设置
	DataType2		d_5095;///<PT
	DataType2		d_5096;///<CT
	DataType2		d_5097;///<标定
	YCMsgSet	  d_5098;///<遥测信息体属性配置
	YCMsgSet	  d_5099;///<遥测信息体属性配置
	YKMsgSet	  d_50B8;///<遥控信息体属性配置
	DataType4   d_5202;///<温度定值
	DataType4   d_5203;///<湿度定值
	DataType1   d_5211;///<人工置数使能
	UserRealDataDefPara d_5212;///<人工置数参数
	//////////以下为自定义/////////
	SP_COMPLEX FFT_RESULT[CHENAL_NUM][SAMP_NUM+2*PAD];
	float Modulus[CHENAL_NUM][SAMP_NUM/2];//直流量，基波，... N次谐波
	//scada_yc_t YcData;
	uint16 ycavrnum;
	ratio_t  YcRatioTmp[MAX_LINE_NUM];
	ratio_t  StandYc[MAX_LINE_NUM];
	harmonictab_t HmncDataP[MAX_LINE_NUM][6];
	maxavrmin_t ClbrtTmp[CHENAL_NUM];
	maxavrminpower_t ClbrtPowerTmp[MAX_LINE_NUM][4];
	uint8 Clbrtnum;
	uint8 StartClbrt;
	settime_t ClbrtTime;
	//setmstime_t YcDeadTime[0x200];

}ScadaPara;
extern scada_yc_t YcData;
extern setmstime_t YcDeadTime[0x200];
/**
*************************************************************************************  
*  @brief  数据采集类定义。
*/
class Scada :public BaseApp{
private: 
	uint16 timer_id;///<内部定时器id
	harmonicdata_t GetHarmonicRmsAndRatio(float BaseHarmonicRms,float *src);
	void GetHarmonicData(void);
	void PtAdapt(void);
	power_t   GetPower(uint8 Uname,uint8 Iname);
	angelvalue_t CosSinz1z2(SP_COMPLEX U,SP_COMPLEX I);
	void PhaseChange( SP_COMPLEX *dir, SP_COMPLEX *src , uint16 len ,float offsetangle );
	SP_COMPLEX ComplexRotation(SP_COMPLEX Input,float32 Angle);
	void compare(float *max,float *min,float *avr,float var,float avr_num);
	void first_compare(float *max,float *min,float var);
	void GetXishu(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue );
	float GetDead(int32 DataId);
	float GetYcAngle(uint8 YcName1,uint8 YcName2);
	float GetCplAngle(SP_COMPLEX CplData);
	int16 FloatCompare ( float cmp1, float cmp2);
	int16 IsOverDeadband ( float value1, float value2, float db );

	/**
	*************************************************************************************  
	*  @brief  获取一定长度的全波有效值
	*  @author
	*  @param[in]
	*  @param[out]
	*  @return  成功，返回1，否则返回0。
	**************************************************************************************
	*/
	float TotalRms(float *src , uint8 len);
	/**
	*************************************************************************************
	*  @brief  实时计算模拟量数据，更新数据表.
	*  @author
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 		
	int16 Caculate();
	/*
	*************************************************************************************
	*  @brief  获取各个谐波的模值
	*  @author   zf
	*  @param[in]
	*  @param[out]
	*  @return
	**************************************************************************************
	*/
	void GetModulus(void);
	/*
	*************************************************************************************
	*  @brief  有效值计算
	*  @author   zf
	*  @param[in]
	*  @param[out]
	*  @return
	**************************************************************************************
	*/
	void  GetRms(void);

	/*
	*************************************************************************************
	*  @brief  功率值计算
	*  @author   zf
	*  @param[in]
	*  @param[out]
	*  @return
	**************************************************************************************
	*/
	void  GetPQS(void);

	/*
	*************************************************************************************
	*  @brief  越死区值计算
	*  @author   zf
	*  @param[in]
	*  @param[out]
	*  @return
	**************************************************************************************
	*/
	void  CheckDeadData(void);

	/*
	*************************************************************************************
	*  @brief  标定
	*  @author   zf
	*  @param[in]
	*  @param[out]
	*  @return
	**************************************************************************************
	*/
	void  Calibration(void);

public:

	ScadaPara	scada_cfg;///<Scada配置参数
	uint16  MAXYCDATANUM;
	Scada();
	~Scada(){};	
	/**
	*************************************************************************************
	*  @brief  实时采样开关量，更新数据表.
	*  @author  liht
	*  @param[in]
	*  @param[out]
	*  @return  成功，返回1，否则返回0。
	**************************************************************************************
	*/
//	int16 GetYx();
	/**
	*************************************************************************************
	*  @brief  根据指令执行遥控命令.
	*  @author  liht
	*  @param[in]
	*  @param[out]
	*  @return  成功，返回1，否则返回0。
	**************************************************************************************
	*/
//	int16 ControlYk(int16 YkDataId,DCOWORD DCO_Word);


	/**
	*************************************************************************************  
	*  @brief  读取配置文件.
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 	
	int16 ReadCfg(uint32 Id);
	/**
	*************************************************************************************  
	*  @brief  写配置文件.
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 	
	int16 WriteCfg(){return 1;};
	/**
	*************************************************************************************  
	*  @brief  初始化本模块。 
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 
	int16 Init();
	/**
	*************************************************************************************  
	*  @brief  处理本模块获得的消息. 
	*  @author  liht 
	*  @param [in] MessageData*message，消息类型定义参见datatype.h.
	*  @param [out]  .
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/ 	
	int16 GetMsg(MessageData<BaseDataType>message);	
	/**
	*************************************************************************************  
	*  @brief  运行模块。 
	*  @author  liht 
	*  @param [in] 
	*  @param [out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/	
	int16 Run();	
	/**
	*************************************************************************************  
	*  @brief  过程中错误检查。 
	*  @author  liht 
	*  @param[in] 
	*  @param[out]  
	*  @return  成功，返回1，否则返回0。 
	**************************************************************************************
	*/		
	int16 CheckError(int8 Id){return 1;};	
};
extern Scada scada;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* scada_main(void*);
#endif




