#ifndef DATATYPE_H
#define DATATYPE_H
#include "types.h"
#include "user.h"
#include "os.h"
#include "dspdatatype.h"
#define MAXYXDATA 100//目前86
#define MAXYCDATA 500//目前107
#define MAXYKDATA 10//目前5
#define MAXPARADATA 200//目前167
#define MAX_LINE_NUM 2//回线数
/**
* /brief 遥信类数据标识
*/
#define YX_0001 0x1 ///<断路器1合位
#define YX_0002 0x2 ///<断路器1分位
#define YX_0003 0x3 ///<断路器1储能位置
#define YX_0004 0x4 ///<断路器1隔离刀闸位置（备用遥信4）
#define YX_0005 0x5 ///<备用遥信5变位
#define YX_0006 0x6 ///<备用遥信6变位
#define YX_0007 0x7 ///<线路1一段故障信号
#define YX_0008 0x8 ///<线路1二段故障信号
#define YX_0009 0x9 ///<线路1三段故障信号
#define YX_000A 0xA ///<线路1单相接地故障信号
#define YX_000B 0xB ///<线路1零序电压故障信号
#define YX_000C 0xC ///<线路1一段保护动作信号
#define YX_000D 0xD ///<线路1二段保护动作信号
#define YX_000E 0xE ///<线路1三段保护动作信号
#define YX_000F 0xF ///<线路1单相接地零序电流保护动作信号
#define YX_0010 0x10///<线路1零序电压保护动作信号
#define YX_0011 0x11 ///<故障指示器指示线路1相间短路故障
#define YX_0012 0x12 ///<故障指示器指示线路1接地故障
#define YX_0013 0x13 ///<断路器1闭锁
#define YX_0014 0x14 ///<断路器1异常
#define YX_0015 0x15 ///<断路器1 SF6气体异常报警信号
#define YX_0016 0x16 ///<断路器1 SF6气压闭锁信号
#define YX_0017 0x17 ///<线路1PT断线
#define YX_0018 0x18 ///<线路1CT断线
#define YX_0019 0x19 ///<线路1启动重合闸
#define YX_001A 0x1A ///<线路1重合闸成功/失败
#define YX_001B 0x1B ///<断路器2合位
#define YX_001C 0x1C ///<断路器2分位
#define YX_001D 0x1D ///<断路器2储能位置
#define YX_001E 0x1E ///<断路器2隔离刀闸位置（备用遥信10）
#define YX_001F 0x1F ///<备用遥信11
#define YX_0020 0x20 ///<备用遥信12
#define YX_0021 0x21 ///<线路2一段故障信号
#define YX_0022 0x22 ///<线路2二段故障信号
#define YX_0023 0x23 ///<线路2三段故障信号
#define YX_0024 0x24 ///<线路2单相接地故障信号
#define YX_0025 0x25 ///<线路2零序电压故障信号
#define YX_0026 0x26 ///<线路2一段保护动作信号
#define YX_0027 0x27 ///<线路2二段保护动作信号
#define YX_0028 0x28 ///<线路2三段保护动作信号
#define YX_0029 0x29 ///<线路2单相接地保护动作信号
#define YX_002A 0x2A ///<线路2零序电压保护动作信号
#define YX_002B 0x2B ///<故障指示器指示线路2相间短路故障
#define YX_002C 0x2C ///<故障指示器指示线路2接地故障
#define YX_002D 0x2D ///<断路器2闭锁
#define YX_002E 0x2E ///<断路器2异常
#define YX_002F 0x2F ///<断路器2 SF6气体异常报警信号
#define YX_0030 0x30 ///<断路器2 SF6气压闭锁信号
#define YX_0031 0x31 ///<线路2PT断线
#define YX_0032 0x32 ///<线路2CT断线
#define YX_0033 0x33 ///<线路2启动重合闸
#define YX_0034 0x34 ///<线路2重合闸成功/失败
#define YX_0035 0x35 ///<远方/就地位置
#define YX_0036 0x36 ///<电池低压告警/恢复
#define YX_0037 0x37 ///<电池欠压告警/恢复
#define YX_0038 0x38 ///<交流失电告警/恢复
#define YX_0039 0x39 ///<装置工作电源异常告警/恢复
#define YX_003A 0x3A ///<终端异常/恢复
#define YX_003B 0x3B ///<线路1就地动作信号
#define YX_003C 0x3C ///<线路2就地动作信号


#define YX_0201 0x201 ///<线路1 A相电压越上限告警
#define YX_0202 0x202 ///<线路1 B相电压越上限告警
#define YX_0203 0x203 ///<线路1 C相电压越上限告警
#define YX_0204 0x204 ///<线路1 A相电压越下限告警
#define YX_0205 0x205 ///<线路1 B相电压越下限告警
#define YX_0206 0x206 ///<线路1 C相电压越下限告警
#define YX_0207 0x207 ///<线路1 A相电流越上限告警
#define YX_0208 0x208 ///<线路1 B相电流越上限告警
#define YX_0209 0x209 ///<线路1 C相电流越上限告警
#define YX_020A 0x20A ///<线路1 负荷越上限告警
#define YX_020B 0x20B ///<线路2 A相电压越上限告警
#define YX_020C 0x20C ///<线路2 B相电压越上限告警
#define YX_020D 0x20D ///<线路2 C相电压越上限告警
#define YX_020E 0x20E ///<线路2 A相电压越下限告警
#define YX_020F 0x20F ///<线路2 B相电压越下限告警
#define YX_0210 0x210 ///<线路2 C相电压越下限告警
#define YX_0211 0x211 ///<线路2 A相电流越上限告警
#define YX_0212 0x212 ///<线路2 B相电流越上限告警
#define YX_0213 0x213 ///<线路2 C相电流越上限告警
#define YX_0214 0x214 ///<线路2 负荷越上限告警
#define YX_0215 0x215 ///<温度越限
#define YX_0216 0x216 ///<湿度越限
#define YX_0217 0x217 ///<主通道月流量超标越限
#define YX_0218 0x218 ///<备通道月流量超标越限
#define YX_0219 0x219 ///<断路器1双点遥信
#define YX_021A 0x21A ///<断路器2双点遥信


/**
* /brief 遥测类数据标识
*/
#define YC_4001	0x4001	 ///<线路1 A相电压
#define YC_4002	0x4002	 ///<线路1 B相电压
#define YC_4003	0x4003	 ///<线路1 C相电压
#define YC_4004	0x4004	 ///<线路1 AB线电压
#define YC_4005	0x4005	 ///<线路1 BC线电压
#define YC_4006	0x4006	 ///<线路1 零序电压
#define YC_4007	0x4007	 ///<线路1 A相测量电流
#define YC_4008	0x4008	 ///<线路1 B相测量电流
#define YC_4009	0x4009	 ///<线路1 C相测量电流
#define YC_400A	0x400A	 ///<线路1 零序电流
#define YC_400B	0x400B	 ///<线路1 A相保护电流
#define YC_400C	0x400C	 ///<线路1 B相保护电流
#define YC_400D	0x400D	 ///<线路1 C相保护电流
#define YC_400E	0x400E	 ///<线路1 A相有功功率
#define YC_400F	0x400F	 ///<线路1 A相无功功率
#define YC_4010	0x4010	 ///<线路1 A相功率因数
#define YC_4011	0x4011	 ///<线路1 B相有功功率
#define YC_4012	0x4012	 ///<线路1 B相无功功率
#define YC_4013	0x4013	 ///<线路1 B相功率因数
#define YC_4014	0x4014	 ///<线路1 C相有功功率
#define YC_4015	0x4015	 ///<线路1 C相无功功率
#define YC_4016	0x4016	 ///<线路1 C相功率因数
#define YC_4017	0x4017	 ///<线路1 总有功功率
#define YC_4018	0x4018	 ///<线路1 总无功功率
#define YC_4019	0x4019	 ///<线路1 总视在功率
#define YC_401A	0x401A	 ///<线路1 总功率因数
#define YC_401B	0x401B	 ///<线路1 频率
#define YC_401C	0x401C	///<线路1 A相 2次电压谐波有效值													
#define YC_401D	0x401D  ///<线路1 A相 3次电压谐波有效值		                       
#define YC_401E	0x401E  ///<线路1 A相 4次电压谐波有效值		                       
#define YC_401F	0x401F  ///<线路1 A相 5次电压谐波有效值		                       
#define YC_4020	0x4020  ///<线路1 A相 6次电压谐波有效值		                       
#define YC_4021	0x4021  ///<线路1 A相 7次电压谐波有效值		                       
#define YC_4022	0x4022  ///<线路1 A相 8次电压谐波有效值		                       
#define YC_4023	0x4023  ///<线路1 A相 9次电压谐波有效值		                       
#define YC_4024	0x4024  ///<线路1 A相 10次电压谐波有效值	                     
#define YC_4025	0x4025  ///<线路1 A相 11次电压谐波有效值	                     
#define YC_4026	0x4026  ///<线路1 A相 12次电压谐波有效值	                     
#define YC_4027	0x4027  ///<线路1 A相 13次电压谐波有效值	                     
#define YC_4028	0x4028  ///<线路1 A相 14次电压谐波有效值	                     
#define YC_4029	0x4029  ///<线路1 A相 15次电压谐波有效值	                     
#define YC_402A	0x402A  ///<线路1 A相 16次电压谐波有效值	                     
#define YC_402B	0x402B  ///<线路1 A相 17次电压谐波有效值	                     
#define YC_402C	0x402C  ///<线路1 A相 18次电压谐波有效值	                     
#define YC_402D	0x402D  ///<线路1 A相 19次电压谐波有效值	                     
#define YC_402E	0x402E  ///<线路1 A相 2次电流谐波有效值		
#define YC_402F	0x402F  ///<线路1 A相 3次电流谐波有效值		
#define YC_4030	0x4030  ///<线路1 A相 4次电流谐波有效值		
#define YC_4031	0x4031  ///<线路1 A相 5次电流谐波有效值		
#define YC_4032	0x4032  ///<线路1 A相 6次电流谐波有效值		
#define YC_4033	0x4033  ///<线路1 A相 7次电流谐波有效值		
#define YC_4034	0x4034  ///<线路1 A相 8次电流谐波有效值		
#define YC_4035	0x4035  ///<线路1 A相 9次电流谐波有效值		
#define YC_4036	0x4036  ///<线路1 A相 10次电流谐波有效值	
#define YC_4037	0x4037  ///<线路1 A相 11次电流谐波有效值	
#define YC_4038	0x4038  ///<线路1 A相 12次电流谐波有效值	
#define YC_4039	0x4039  ///<线路1 A相 13次电流谐波有效值	
#define YC_403A	0x403A  ///<线路1 A相 14次电流谐波有效值	
#define YC_403B	0x403B  ///<线路1 A相 15次电流谐波有效值	
#define YC_403C	0x403C  ///<线路1 A相 16次电流谐波有效值	
#define YC_403D	0x403D  ///<线路1 A相 17次电流谐波有效值	
#define YC_403E	0x403E  ///<线路1 A相 18次电流谐波有效值	
#define YC_403F	0x403F  ///<线路1 A相 19次电流谐波有效值	
#define YC_4040	0x4040  ///<线路1 A相 电压谐波总含有率		
#define YC_4041	0x4041  ///<线路1 A相 2次电压谐波含有率		
#define YC_4042	0x4042  ///<线路1 A相 3次电压谐波含有率		
#define YC_4043	0x4043  ///<线路1 A相 4次电压谐波含有率		
#define YC_4044	0x4044  ///<线路1 A相 5次电压谐波含有率		
#define YC_4045	0x4045  ///<线路1 A相 6次电压谐波含有率		
#define YC_4046	0x4046  ///<线路1 A相 7次电压谐波含有率		
#define YC_4047	0x4047  ///<线路1 A相 8次电压谐波含有率		
#define YC_4048	0x4048  ///<线路1 A相 9次电压谐波含有率		
#define YC_4049	0x4049  ///<线路1 A相 10次电压谐波含有率	
#define YC_404A	0x404A  ///<线路1 A相 11次电压谐波含有率	
#define YC_404B	0x404B  ///<线路1 A相 12次电压谐波含有率	
#define YC_404C	0x404C  ///<线路1 A相 13次电压谐波含有率	
#define YC_404D	0x404D  ///<线路1 A相 14次电压谐波含有率	
#define YC_404E	0x404E  ///<线路1 A相 15次电压谐波含有率	
#define YC_404F	0x404F  ///<线路1 A相 16次电压谐波含有率	
#define YC_4050	0x4050  ///<线路1 A相 17次电压谐波含有率	
#define YC_4051	0x4051  ///<线路1 A相 18次电压谐波含有率	
#define YC_4052	0x4052  ///<线路1 A相 19次电压谐波含有率	
#define YC_4053	0x4053  ///<线路1 A相 电流谐波总含有率		
#define YC_4054	0x4054  ///<线路1 A相 2次电流谐波含有率		
#define YC_4055	0x4055  ///<线路1 A相 3次电流谐波含有率		
#define YC_4056	0x4056  ///<线路1 A相 4次电流谐波含有率		
#define YC_4057	0x4057  ///<线路1 A相 5次电流谐波含有率		
#define YC_4058	0x4058  ///<线路1 A相 6次电流谐波含有率		
#define YC_4059	0x4059  ///<线路1 A相 7次电流谐波含有率		
#define YC_405A	0x405A  ///<线路1 A相 8次电流谐波含有率		
#define YC_405B	0x405B  ///<线路1 A相 9次电流谐波含有率		
#define YC_405C	0x405C  ///<线路1 A相 10次电流谐波含有率	
#define YC_405D	0x405D  ///<线路1 A相 11次电流谐波含有率	
#define YC_405E	0x405E  ///<线路1 A相 12次电流谐波含有率	
#define YC_405F	0x405F  ///<线路1 A相 13次电流谐波含有率	
#define YC_4060	0x4060  ///<线路1 A相 14次电流谐波含有率	
#define YC_4061	0x4061  ///<线路1 A相 15次电流谐波含有率	
#define YC_4062	0x4062  ///<线路1 A相 16次电流谐波含有率	
#define YC_4063	0x4063  ///<线路1 A相 17次电流谐波含有率	
#define YC_4064	0x4064  ///<线路1 A相 18次电流谐波含有率	
#define YC_4065	0x4065  ///<线路1 A相 19次电流谐波含有率	
#define YC_4066	0x4066  ///<线路1 B相 2次电压谐波有效值		
#define YC_4067	0x4067  ///<线路1 B相 3次电压谐波有效值		
#define YC_4068	0x4068  ///<线路1 B相 4次电压谐波有效值		
#define YC_4069	0x4069  ///<线路1 B相 5次电压谐波有效值		
#define YC_406A	0x406A  ///<线路1 B相 6次电压谐波有效值		
#define YC_406B	0x406B  ///<线路1 B相 7次电压谐波有效值		
#define YC_406C	0x406C  ///<线路1 B相 8次电压谐波有效值		
#define YC_406D	0x406D  ///<线路1 B相 9次电压谐波有效值		
#define YC_406E	0x406E  ///<线路1 B相 10次电压谐波有效值	
#define YC_406F	0x406F  ///<线路1 B相 11次电压谐波有效值	
#define YC_4070	0x4070  ///<线路1 B相 12次电压谐波有效值	
#define YC_4071	0x4071  ///<线路1 B相 13次电压谐波有效值	
#define YC_4072	0x4072  ///<线路1 B相 14次电压谐波有效值	
#define YC_4073	0x4073  ///<线路1 B相 15次电压谐波有效值	
#define YC_4074	0x4074  ///<线路1 B相 16次电压谐波有效值	
#define YC_4075	0x4075  ///<线路1 B相 17次电压谐波有效值	
#define YC_4076	0x4076  ///<线路1 B相 18次电压谐波有效值	
#define YC_4077	0x4077  ///<线路1 B相 19次电压谐波有效值	
#define YC_4078	0x4078  ///<线路1 B相 2次电流谐波有效值		
#define YC_4079	0x4079  ///<线路1 B相 3次电流谐波有效值		
#define YC_407A	0x407A  ///<线路1 B相 4次电流谐波有效值		
#define YC_407B	0x407B  ///<线路1 B相 5次电流谐波有效值		
#define YC_407C	0x407C  ///<线路1 B相 6次电流谐波有效值		
#define YC_407D	0x407D  ///<线路1 B相 7次电流谐波有效值		
#define YC_407E	0x407E  ///<线路1 B相 8次电流谐波有效值		
#define YC_407F	0x407F  ///<线路1 B相 9次电流谐波有效值		
#define YC_4080	0x4080  ///<线路1 B相 10次电流谐波有效值	
#define YC_4081	0x4081  ///<线路1 B相 11次电流谐波有效值	
#define YC_4082	0x4082  ///<线路1 B相 12次电流谐波有效值	
#define YC_4083	0x4083  ///<线路1 B相 13次电流谐波有效值	
#define YC_4084	0x4084  ///<线路1 B相 14次电流谐波有效值	
#define YC_4085	0x4085  ///<线路1 B相 15次电流谐波有效值	
#define YC_4086	0x4086  ///<线路1 B相 16次电流谐波有效值	
#define YC_4087	0x4087  ///<线路1 B相 17次电流谐波有效值	
#define YC_4088	0x4088  ///<线路1 B相 18次电流谐波有效值	
#define YC_4089	0x4089  ///<线路1 B相 19次电流谐波有效值	
#define YC_408A	0x408A  ///<线路1 B相 电压谐波总含有率		
#define YC_408B	0x408B  ///<线路1 B相 2次电压谐波含有率		
#define YC_408C	0x408C  ///<线路1 B相 3次电压谐波含有率		
#define YC_408D	0x408D  ///<线路1 B相 4次电压谐波含有率		
#define YC_408E	0x408E  ///<线路1 B相 5次电压谐波含有率		
#define YC_408F	0x408F  ///<线路1 B相 6次电压谐波含有率		
#define YC_4090	0x4090  ///<线路1 B相 7次电压谐波含有率		
#define YC_4091	0x4091  ///<线路1 B相 8次电压谐波含有率		
#define YC_4092	0x4092  ///<线路1 B相 9次电压谐波含有率		
#define YC_4093	0x4093  ///<线路1 B相 10次电压谐波含有率	
#define YC_4094	0x4094  ///<线路1 B相 11次电压谐波含有率	
#define YC_4095	0x4095  ///<线路1 B相 12次电压谐波含有率	
#define YC_4096	0x4096  ///<线路1 B相 13次电压谐波含有率	
#define YC_4097	0x4097  ///<线路1 B相 14次电压谐波含有率	
#define YC_4098	0x4098  ///<线路1 B相 15次电压谐波含有率	
#define YC_4099	0x4099  ///<线路1 B相 16次电压谐波含有率	
#define YC_409A	0x409A  ///<线路1 B相 17次电压谐波含有率	
#define YC_409B	0x409B  ///<线路1 B相 18次电压谐波含有率	
#define YC_409C	0x409C  ///<线路1 B相 19次电压谐波含有率	
#define YC_409D	0x409D  ///<线路1 B相 电流谐波总含有率		
#define YC_409E	0x409E  ///<线路1 B相 2次电流谐波含有率		
#define YC_409F	0x409F  ///<线路1 B相 3次电流谐波含有率		
#define YC_40A0	0x40A0  ///<线路1 B相 4次电流谐波含有率		
#define YC_40A1	0x40A1  ///<线路1 B相 5次电流谐波含有率		
#define YC_40A2	0x40A2  ///<线路1 B相 6次电流谐波含有率		
#define YC_40A3	0x40A3  ///<线路1 B相 7次电流谐波含有率		
#define YC_40A4	0x40A4  ///<线路1 B相 8次电流谐波含有率		
#define YC_40A5	0x40A5  ///<线路1 B相 9次电流谐波含有率		
#define YC_40A6	0x40A6  ///<线路1 B相 10次电流谐波含有率	
#define YC_40A7	0x40A7  ///<线路1 B相 11次电流谐波含有率	
#define YC_40A8	0x40A8  ///<线路1 B相 12次电流谐波含有率	
#define YC_40A9	0x40A9  ///<线路1 B相 13次电流谐波含有率	
#define YC_40AA	0x40AA  ///<线路1 B相 14次电流谐波含有率	
#define YC_40AB	0x40AB  ///<线路1 B相 15次电流谐波含有率	
#define YC_40AC	0x40AC  ///<线路1 B相 16次电流谐波含有率	
#define YC_40AD	0x40AD  ///<线路1 B相 17次电流谐波含有率	
#define YC_40AE	0x40AE  ///<线路1 B相 18次电流谐波含有率	
#define YC_40AF	0x40AF  ///<线路1 B相 19次电流谐波含有率	
#define YC_40B0	0x40B0  ///<线路1 C相	2次电压谐波有效值		
#define YC_40B1	0x40B1  ///<线路1 C相 3次电压谐波有效值		
#define YC_40B2	0x40B2  ///<线路1 C相 4次电压谐波有效值		
#define YC_40B3	0x40B3  ///<线路1 C相 5次电压谐波有效值		
#define YC_40B4	0x40B4  ///<线路1 C相 6次电压谐波有效值		
#define YC_40B5	0x40B5  ///<线路1 C相 7次电压谐波有效值		
#define YC_40B6	0x40B6  ///<线路1 C相 8次电压谐波有效值		
#define YC_40B7	0x40B7  ///<线路1 C相 9次电压谐波有效值		
#define YC_40B8	0x40B8  ///<线路1 C相 10次电压谐波有效值	
#define YC_40B9	0x40B9  ///<线路1 C相 11次电压谐波有效值	
#define YC_40BA	0x40BA  ///<线路1 C相 12次电压谐波有效值	
#define YC_40BB	0x40BB  ///<线路1 C相 13次电压谐波有效值	
#define YC_40BC	0x40BC  ///<线路1 C相 14次电压谐波有效值	
#define YC_40BD	0x40BD  ///<线路1 C相 15次电压谐波有效值	
#define YC_40BE	0x40BE  ///<线路1 C相 16次电压谐波有效值	
#define YC_40BF	0x40BF  ///<线路1 C相 17次电压谐波有效值	
#define YC_40C0	0x40C0  ///<线路1 C相 18次电压谐波有效值	
#define YC_40C1	0x40C1  ///<线路1 C相 19次电压谐波有效值	
#define YC_40C2	0x40C2  ///<线路1 C相	2次电流谐波有效值		
#define YC_40C3	0x40C3  ///<线路1 C相 3次电流谐波有效值		
#define YC_40C4	0x40C4  ///<线路1 C相 4次电流谐波有效值		
#define YC_40C5	0x40C5  ///<线路1 C相 5次电流谐波有效值		
#define YC_40C6	0x40C6  ///<线路1 C相 6次电流谐波有效值		
#define YC_40C7	0x40C7  ///<线路1 C相 7次电流谐波有效值		
#define YC_40C8	0x40C8  ///<线路1 C相 8次电流谐波有效值		
#define YC_40C9	0x40C9  ///<线路1 C相 9次电流谐波有效值		
#define YC_40CA	0x40CA  ///<线路1 C相 10次电流谐波有效值	
#define YC_40CB	0x40CB  ///<线路1 C相 11次电流谐波有效值	
#define YC_40CC	0x40CC  ///<线路1 C相 12次电流谐波有效值	
#define YC_40CD	0x40CD  ///<线路1 C相 13次电流谐波有效值	
#define YC_40CE	0x40CE  ///<线路1 C相 14次电流谐波有效值	
#define YC_40CF	0x40CF  ///<线路1 C相 15次电流谐波有效值	
#define YC_40D0	0x40D0  ///<线路1 C相 16次电流谐波有效值	
#define YC_40D1	0x40D1  ///<线路1 C相 17次电流谐波有效值	
#define YC_40D2	0x40D2  ///<线路1 C相 18次电流谐波有效值	
#define YC_40D3	0x40D3  ///<线路1 C相 19次电流谐波有效值	
#define YC_40D4	0x40D4  ///<线路1 C相 电压谐波总含有率		
#define YC_40D5	0x40D5  ///<线路1 C相	2次电压谐波含有率		
#define YC_40D6	0x40D6  ///<线路1 C相 3次电压谐波含有率		
#define YC_40D7	0x40D7  ///<线路1 C相 4次电压谐波含有率		
#define YC_40D8	0x40D8  ///<线路1 C相 5次电压谐波含有率		
#define YC_40D9	0x40D9  ///<线路1 C相 6次电压谐波含有率		
#define YC_40DA	0x40DA  ///<线路1 C相 7次电压谐波含有率		
#define YC_40DB	0x40DB  ///<线路1 C相 8次电压谐波含有率		
#define YC_40DC	0x40DC  ///<线路1 C相 9次电压谐波含有率		
#define YC_40DD	0x40DD  ///<线路1 C相 10次电压谐波含有率	
#define YC_40DE	0x40DE  ///<线路1 C相 11次电压谐波含有率	
#define YC_40DF	0x40DF  ///<线路1 C相 12次电压谐波含有率	
#define YC_40E0	0x40E0  ///<线路1 C相 13次电压谐波含有率	
#define YC_40E1	0x40E1  ///<线路1 C相 14次电压谐波含有率	
#define YC_40E2	0x40E2  ///<线路1 C相 15次电压谐波含有率	
#define YC_40E3	0x40E3  ///<线路1 C相 16次电压谐波含有率	
#define YC_40E4	0x40E4  ///<线路1 C相 17次电压谐波含有率	
#define YC_40E5	0x40E5  ///<线路1 C相 18次电压谐波含有率	
#define YC_40E6	0x40E6  ///<线路1 C相 19次电压谐波含有率	
#define YC_40E7	0x40E7  ///<线路1 C相 电流谐波总含有率		
#define YC_40E8	0x40E8  ///<线路1 C相	2次电流谐波含有率		
#define YC_40E9	0x40E9  ///<线路1 C相 3次电流谐波含有率		
#define YC_40EA	0x40EA  ///<线路1 C相 4次电流谐波含有率		
#define YC_40EB	0x40EB  ///<线路1 C相 5次电流谐波含有率		
#define YC_40EC	0x40EC  ///<线路1 C相 6次电流谐波含有率		
#define YC_40ED	0x40ED  ///<线路1 C相 7次电流谐波含有率		
#define YC_40EE	0x40EE  ///<线路1 C相 8次电流谐波含有率		
#define YC_40EF	0x40EF  ///<线路1 C相 9次电流谐波含有率		
#define YC_40F0	0x40F0  ///<线路1 C相 10次电流谐波含有率	
#define YC_40F1	0x40F1  ///<线路1 C相 11次电流谐波含有率	
#define YC_40F2	0x40F2  ///<线路1 C相 12次电流谐波含有率	
#define YC_40F3	0x40F3  ///<线路1 C相 13次电流谐波含有率	
#define YC_40F4	0x40F4  ///<线路1 C相 14次电流谐波含有率	
#define YC_40F5	0x40F5  ///<线路1 C相 15次电流谐波含有率	
#define YC_40F6	0x40F6  ///<线路1 C相 16次电流谐波含有率	
#define YC_40F7	0x40F7  ///<线路1 C相 17次电流谐波含有率	
#define YC_40F8	0x40F8  ///<线路1 C相 18次电流谐波含有率	
#define YC_40F9	0x40F9  ///<线路1 C相 19次电流谐波含有率	
#define YC_40FA	0x40FA  ///<线路2 A相电压									
#define YC_40FB	0x40FB  ///<线路2 B相电压									
#define YC_40FC	0x40FC  ///<线路2 C相电压									
#define YC_40FD	0x40FD  ///<线路2 AB线电压								
#define YC_40FE	0x40FE  ///<线路2 BC线电压								
#define YC_40FF	0x40FF  ///<线路2 零序电压								
#define YC_4100	0x4100  ///<线路2 A相测量电流							
#define YC_4101	0x4101  ///<线路2 B相测量电流							
#define YC_4102	0x4102  ///<线路2 C相测量电流							
#define YC_4103	0x4103  ///<线路2 零序电流								
#define YC_4104	0x4104  ///<线路2 A相保护电流							
#define YC_4105	0x4105  ///<线路2 B相保护电流							
#define YC_4106	0x4106  ///<线路2 C相保护电流							
#define YC_4107	0x4107  ///<线路2 A相有功功率							
#define YC_4108	0x4108  ///<线路2 A相无功功率							
#define YC_4109	0x4109  ///<线路2 A相功率因数							
#define YC_410A	0x410A  ///<线路2 B相有功功率							
#define YC_410B	0x410B  ///<线路2 B相无功功率							
#define YC_410C	0x410C  ///<线路2 B相功率因数							
#define YC_410D	0x410D  ///<线路2 C相有功功率							
#define YC_410E	0x410E  ///<线路2 C相无功功率							
#define YC_410F	0x410F  ///<线路2 C相功率因数							
#define YC_4110	0x4110  ///<线路2 总有功功率							
#define YC_4111	0x4111  ///<线路2 总无功功率							
#define YC_4112	0x4112  ///<线路2 总视在功率							
#define YC_4113	0x4113  ///<线路2 总功率因数							
#define YC_4114	0x4114  ///<线路2 频率										
#define YC_4115	0x4115  ///<线路2 A相 2次电压谐波有效值		
#define YC_4116	0x4116  ///<线路2 A相 3次电压谐波有效值		
#define YC_4117	0x4117  ///<线路2 A相 4次电压谐波有效值		
#define YC_4118	0x4118  ///<线路2 A相 5次电压谐波有效值		
#define YC_4119	0x4119  ///<线路2 A相 6次电压谐波有效值		
#define YC_411A	0x411A  ///<线路2 A相 7次电压谐波有效值		
#define YC_411B	0x411B  ///<线路2 A相 8次电压谐波有效值		
#define YC_411C	0x411C  ///<线路2 A相 9次电压谐波有效值		
#define YC_411D	0x411D  ///<线路2 A相 10次电压谐波有效值	
#define YC_411E	0x411E  ///<线路2 A相 11次电压谐波有效值	
#define YC_411F	0x411F  ///<线路2 A相 12次电压谐波有效值	
#define YC_4120	0x4120  ///<线路2 A相 13次电压谐波有效值	
#define YC_4121	0x4121  ///<线路2 A相 14次电压谐波有效值	
#define YC_4122	0x4122  ///<线路2 A相 15次电压谐波有效值	
#define YC_4123	0x4123  ///<线路2 A相 16次电压谐波有效值	
#define YC_4124	0x4124  ///<线路2 A相 17次电压谐波有效值	
#define YC_4125	0x4125  ///<线路2 A相 18次电压谐波有效值	
#define YC_4126	0x4126  ///<线路2 A相 19次电压谐波有效值	
#define YC_4127	0x4127  ///<线路2 A相 2次电流谐波有效值		
#define YC_4128	0x4128  ///<线路2 A相 3次电流谐波有效值		
#define YC_4129	0x4129  ///<线路2 A相 4次电流谐波有效值		
#define YC_412A	0x412A  ///<线路2 A相 5次电流谐波有效值		
#define YC_412B	0x412B  ///<线路2 A相 6次电流谐波有效值		
#define YC_412C	0x412C  ///<线路2 A相 7次电流谐波有效值		
#define YC_412D	0x412D  ///<线路2 A相 8次电流谐波有效值		
#define YC_412E	0x412E  ///<线路2 A相 9次电流谐波有效值		
#define YC_412F	0x412F  ///<线路2 A相 10次电流谐波有效值	
#define YC_4130	0x4130  ///<线路2 A相 11次电流谐波有效值	
#define YC_4131	0x4131  ///<线路2 A相 12次电流谐波有效值	
#define YC_4132	0x4132  ///<线路2 A相 13次电流谐波有效值	
#define YC_4133	0x4133  ///<线路2 A相 14次电流谐波有效值	
#define YC_4134	0x4134  ///<线路2 A相 15次电流谐波有效值	
#define YC_4135	0x4135  ///<线路2 A相 16次电流谐波有效值	
#define YC_4136	0x4136  ///<线路2 A相 17次电流谐波有效值	
#define YC_4137	0x4137  ///<线路2 A相 18次电流谐波有效值	
#define YC_4138	0x4138  ///<线路2 A相 19次电流谐波有效值	
#define YC_4139	0x4139  ///<线路2 A相 电压谐波总含有率		
#define YC_413A	0x413A  ///<线路2 A相 2次电压谐波含有率		
#define YC_413B	0x413B  ///<线路2 A相 3次电压谐波含有率		
#define YC_413C	0x413C  ///<线路2 A相 4次电压谐波含有率		
#define YC_413D	0x413D  ///<线路2 A相 5次电压谐波含有率		
#define YC_413E	0x413E  ///<线路2 A相 6次电压谐波含有率		
#define YC_413F	0x413F  ///<线路2 A相 7次电压谐波含有率		
#define YC_4140	0x4140  ///<线路2 A相 8次电压谐波含有率		
#define YC_4141	0x4141  ///<线路2 A相 9次电压谐波含有率		
#define YC_4142	0x4142  ///<线路2 A相 10次电压谐波含有率	
#define YC_4143	0x4143  ///<线路2 A相 11次电压谐波含有率	
#define YC_4144	0x4144  ///<线路2 A相 12次电压谐波含有率	
#define YC_4145	0x4145  ///<线路2 A相 13次电压谐波含有率	
#define YC_4146	0x4146  ///<线路2 A相 14次电压谐波含有率	
#define YC_4147	0x4147  ///<线路2 A相 15次电压谐波含有率	
#define YC_4148	0x4148  ///<线路2 A相 16次电压谐波含有率	
#define YC_4149	0x4149  ///<线路2 A相 17次电压谐波含有率	
#define YC_414A	0x414A  ///<线路2 A相 18次电压谐波含有率	
#define YC_414B	0x414B  ///<线路2 A相 19次电压谐波含有率	
#define YC_414C	0x414C  ///<线路2 A相 电流谐波总含有率		
#define YC_414D	0x414D  ///<线路2 A相 2次电流谐波含有率		
#define YC_414E	0x414E  ///<线路2 A相 3次电流谐波含有率		
#define YC_414F	0x414F  ///<线路2 A相 4次电流谐波含有率		
#define YC_4150	0x4150  ///<线路2 A相 5次电流谐波含有率		
#define YC_4151	0x4151  ///<线路2 A相 6次电流谐波含有率		
#define YC_4152	0x4152  ///<线路2 A相 7次电流谐波含有率		
#define YC_4153	0x4153  ///<线路2 A相 8次电流谐波含有率		
#define YC_4154	0x4154  ///<线路2 A相 9次电流谐波含有率		
#define YC_4155	0x4155  ///<线路2 A相 10次电流谐波含有率	
#define YC_4156	0x4156  ///<线路2 A相 11次电流谐波含有率	
#define YC_4157	0x4157  ///<线路2 A相 12次电流谐波含有率	
#define YC_4158	0x4158  ///<线路2 A相 13次电流谐波含有率	
#define YC_4159	0x4159  ///<线路2 A相 14次电流谐波含有率	
#define YC_415A	0x415A  ///<线路2 A相 15次电流谐波含有率	
#define YC_415B	0x415B  ///<线路2 A相 16次电流谐波含有率	
#define YC_415C	0x415C  ///<线路2 A相 17次电流谐波含有率	
#define YC_415D	0x415D  ///<线路2 A相 18次电流谐波含有率	
#define YC_415E	0x415E  ///<线路2 A相 19次电流谐波含有率	
#define YC_415F	0x415F  ///<线路2 B相 2次电压谐波有效值		
#define YC_4160	0x4160  ///<线路2 B相 3次电压谐波有效值		
#define YC_4161	0x4161  ///<线路2 B相 4次电压谐波有效值		
#define YC_4162	0x4162  ///<线路2 B相 5次电压谐波有效值		
#define YC_4163	0x4163  ///<线路2 B相 6次电压谐波有效值		
#define YC_4164	0x4164  ///<线路2 B相 7次电压谐波有效值		
#define YC_4165	0x4165  ///<线路2 B相 8次电压谐波有效值		
#define YC_4166	0x4166  ///<线路2 B相 9次电压谐波有效值		
#define YC_4167	0x4167  ///<线路2 B相 10次电压谐波有效值	
#define YC_4168	0x4168  ///<线路2 B相 11次电压谐波有效值	
#define YC_4169	0x4169  ///<线路2 B相 12次电压谐波有效值	
#define YC_416A	0x416A  ///<线路2 B相 13次电压谐波有效值	
#define YC_416B	0x416B  ///<线路2 B相 14次电压谐波有效值	
#define YC_416C	0x416C  ///<线路2 B相 15次电压谐波有效值	
#define YC_416D	0x416D  ///<线路2 B相 16次电压谐波有效值	
#define YC_416E	0x416E  ///<线路2 B相 17次电压谐波有效值	
#define YC_416F	0x416F  ///<线路2 B相 18次电压谐波有效值	
#define YC_4170	0x4170  ///<线路2 B相 19次电压谐波有效值	
#define YC_4171	0x4171  ///<线路2 B相 2次电流谐波有效值		
#define YC_4172	0x4172  ///<线路2 B相 3次电流谐波有效值		
#define YC_4173	0x4173  ///<线路2 B相 4次电流谐波有效值		
#define YC_4174	0x4174  ///<线路2 B相 5次电流谐波有效值		
#define YC_4175	0x4175  ///<线路2 B相 6次电流谐波有效值		
#define YC_4176	0x4176  ///<线路2 B相 7次电流谐波有效值		
#define YC_4177	0x4177  ///<线路2 B相 8次电流谐波有效值		
#define YC_4178	0x4178  ///<线路2 B相 9次电流谐波有效值		
#define YC_4179	0x4179  ///<线路2 B相 10次电流谐波有效值	
#define YC_417A	0x417A  ///<线路2 B相 11次电流谐波有效值	
#define YC_417B	0x417B  ///<线路2 B相 12次电流谐波有效值	
#define YC_417C	0x417C  ///<线路2 B相 13次电流谐波有效值	
#define YC_417D	0x417D  ///<线路2 B相 14次电流谐波有效值	
#define YC_417E	0x417E  ///<线路2 B相 15次电流谐波有效值	
#define YC_417F	0x417F  ///<线路2 B相 16次电流谐波有效值	
#define YC_4180	0x4180  ///<线路2 B相 17次电流谐波有效值	
#define YC_4181	0x4181  ///<线路2 B相 18次电流谐波有效值	
#define YC_4182	0x4182  ///<线路2 B相 19次电流谐波有效值	
#define YC_4183	0x4183  ///<线路2 B相 电压谐波总含有率		
#define YC_4184	0x4184  ///<线路2 B相 2次电压谐波含有率		
#define YC_4185	0x4185  ///<线路2 B相 3次电压谐波含有率		
#define YC_4186	0x4186  ///<线路2 B相 4次电压谐波含有率		
#define YC_4187	0x4187  ///<线路2 B相 5次电压谐波含有率		
#define YC_4188	0x4188  ///<线路2 B相 6次电压谐波含有率		
#define YC_4189	0x4189  ///<线路2 B相 7次电压谐波含有率		
#define YC_418A	0x418A  ///<线路2 B相 8次电压谐波含有率		
#define YC_418B	0x418B  ///<线路2 B相 9次电压谐波含有率		
#define YC_418C	0x418C  ///<线路2 B相 10次电压谐波含有率	
#define YC_418D	0x418D  ///<线路2 B相 11次电压谐波含有率	
#define YC_418E	0x418E  ///<线路2 B相 12次电压谐波含有率	
#define YC_418F	0x418F  ///<线路2 B相 13次电压谐波含有率	
#define YC_4190	0x4190  ///<线路2 B相 14次电压谐波含有率	
#define YC_4191	0x4191  ///<线路2 B相 15次电压谐波含有率	
#define YC_4192	0x4192  ///<线路2 B相 16次电压谐波含有率	
#define YC_4193	0x4193  ///<线路2 B相 17次电压谐波含有率	
#define YC_4194	0x4194  ///<线路2 B相 18次电压谐波含有率	
#define YC_4195	0x4195  ///<线路2 B相 19次电压谐波含有率	
#define YC_4196	0x4196  ///<线路2 B相 电流谐波总含有率		
#define YC_4197	0x4197  ///<线路2 B相 2次电流谐波含有率		
#define YC_4198	0x4198  ///<线路2 B相 3次电流谐波含有率		
#define YC_4199	0x4199  ///<线路2 B相 4次电流谐波含有率		
#define YC_419A	0x419A  ///<线路2 B相 5次电流谐波含有率		
#define YC_419B	0x419B  ///<线路2 B相 6次电流谐波含有率		
#define YC_419C	0x419C  ///<线路2 B相 7次电流谐波含有率		
#define YC_419D	0x419D  ///<线路2 B相 8次电流谐波含有率		
#define YC_419E	0x419E  ///<线路2 B相 9次电流谐波含有率		
#define YC_419F	0x419F  ///<线路2 B相 10次电流谐波含有率	
#define YC_41A0	0x41A0  ///<线路2 B相 11次电流谐波含有率	
#define YC_41A1	0x41A1  ///<线路2 B相 12次电流谐波含有率	
#define YC_41A2	0x41A2  ///<线路2 B相 13次电流谐波含有率	
#define YC_41A3	0x41A3  ///<线路2 B相 14次电流谐波含有率	
#define YC_41A4	0x41A4  ///<线路2 B相 15次电流谐波含有率	
#define YC_41A5	0x41A5  ///<线路2 B相 16次电流谐波含有率	
#define YC_41A6	0x41A6  ///<线路2 B相 17次电流谐波含有率	
#define YC_41A7	0x41A7  ///<线路2 B相 18次电流谐波含有率	
#define YC_41A8	0x41A8 ///< 线路2 B相 19次电流谐波含有率	
#define YC_41A9	0x41A9 ///< 线路2 C相	2次电压谐波有效值		
#define YC_41AA	0x41AA ///< 线路2 C相 3次电压谐波有效值		
#define YC_41AB	0x41AB ///< 线路2 C相 4次电压谐波有效值		
#define YC_41AC	0x41AC ///< 线路2 C相 5次电压谐波有效值		
#define YC_41AD	0x41AD ///< 线路2 C相 6次电压谐波有效值		
#define YC_41AE	0x41AE ///< 线路2 C相 7次电压谐波有效值		
#define YC_41AF	0x41AF ///< 线路2 C相 8次电压谐波有效值		
#define YC_41B0	0x41B0 ///< 线路2 C相 9次电压谐波有效值		
#define YC_41B1	0x41B1 ///< 线路2 C相 10次电压谐波有效值	
#define YC_41B2	0x41B2 ///< 线路2 C相 11次电压谐波有效值	
#define YC_41B3	0x41B3 ///< 线路2 C相 12次电压谐波有效值	
#define YC_41B4	0x41B4 ///< 线路2 C相 13次电压谐波有效值	
#define YC_41B5	0x41B5 ///< 线路2 C相 14次电压谐波有效值	
#define YC_41B6	0x41B6 ///< 线路2 C相 15次电压谐波有效值	
#define YC_41B7	0x41B7 ///< 线路2 C相 16次电压谐波有效值	
#define YC_41B8	0x41B8 ///< 线路2 C相 17次电压谐波有效值	
#define YC_41B9	0x41B9 ///< 线路2 C相 18次电压谐波有效值	
#define YC_41BA	0x41BA ///< 线路2 C相 19次电压谐波有效值	
#define YC_41BB	0x41BB ///< 线路2 C相	2次电流谐波有效值		
#define YC_41BC	0x41BC ///< 线路2 C相 3次电流谐波有效值		
#define YC_41BD	0x41BD ///< 线路2 C相 4次电流谐波有效值		
#define YC_41BE	0x41BE ///< 线路2 C相 5次电流谐波有效值		
#define YC_41BF	0x41BF ///< 线路2 C相 6次电流谐波有效值		
#define YC_41C0	0x41C0 ///< 线路2 C相 7次电流谐波有效值		
#define YC_41C1	0x41C1 ///< 线路2 C相 8次电流谐波有效值		
#define YC_41C2	0x41C2 ///< 线路2 C相 9次电流谐波有效值		
#define YC_41C3	0x41C3 ///< 线路2 C相 10次电流谐波有效值	
#define YC_41C4	0x41C4 ///< 线路2 C相 11次电流谐波有效值	
#define YC_41C5	0x41C5 ///< 线路2 C相 12次电流谐波有效值	
#define YC_41C6	0x41C6 ///< 线路2 C相 13次电流谐波有效值	
#define YC_41C7	0x41C7 ///< 线路2 C相 14次电流谐波有效值	
#define YC_41C8	0x41C8 ///< 线路2 C相 15次电流谐波有效值	
#define YC_41C9	0x41C9 ///< 线路2 C相 16次电流谐波有效值	
#define YC_41CA	0x41CA ///< 线路2 C相 17次电流谐波有效值	
#define YC_41CB	0x41CB ///< 线路2 C相 18次电流谐波有效值	
#define YC_41CC	0x41CC ///< 线路2 C相 19次电流谐波有效值	
#define YC_41CD	0x41CD ///< 线路2 C相 电压谐波总含有率		
#define YC_41CE	0x41CE ///< 线路2 C相	2次电压谐波含有率		
#define YC_41CF	0x41CF ///< 线路2 C相 3次电压谐波含有率		
#define YC_41D0	0x41D0 ///< 线路2 C相 4次电压谐波含有率		
#define YC_41D1	0x41D1 ///< 线路2 C相 5次电压谐波含有率		
#define YC_41D2	0x41D2 ///< 线路2 C相 6次电压谐波含有率		
#define YC_41D3	0x41D3 ///< 线路2 C相 7次电压谐波含有率		
#define YC_41D4	0x41D4 ///< 线路2 C相 8次电压谐波含有率		
#define YC_41D5	0x41D5 ///< 线路2 C相 9次电压谐波含有率		
#define YC_41D6	0x41D6 ///< 线路2 C相 10次电压谐波含有率	
#define YC_41D7	0x41D7 ///< 线路2 C相 11次电压谐波含有率	
#define YC_41D8	0x41D8 ///< 线路2 C相 12次电压谐波含有率	
#define YC_41D9	0x41D9 ///< 线路2 C相 13次电压谐波含有率	
#define YC_41DA	0x41DA ///< 线路2 C相 14次电压谐波含有率	
#define YC_41DB	0x41DB ///< 线路2 C相 15次电压谐波含有率	
#define YC_41DC	0x41DC ///< 线路2 C相 16次电压谐波含有率	
#define YC_41DD	0x41DD ///< 线路2 C相 17次电压谐波含有率	
#define YC_41DE	0x41DE ///< 线路2 C相 18次电压谐波含有率	
#define YC_41DF	0x41DF ///< 线路2 C相 19次电压谐波含有率	
#define YC_41E0	0x41E0 ///< 线路2 C相 电流谐波总含有率		
#define YC_41E1	0x41E1 ///< 线路2 C相	2次电流谐波含有率		
#define YC_41E2	0x41E2 ///< 线路2 C相 3次电流谐波含有率		
#define YC_41E3	0x41E3 ///< 线路2 C相 4次电流谐波含有率		
#define YC_41E4	0x41E4 ///< 线路2 C相 5次电流谐波含有率		
#define YC_41E5	0x41E5 ///< 线路2 C相 6次电流谐波含有率		
#define YC_41E6	0x41E6 ///< 线路2 C相 7次电流谐波含有率		
#define YC_41E7	0x41E7 ///< 线路2 C相 8次电流谐波含有率		
#define YC_41E8	0x41E8 ///< 线路2 C相 9次电流谐波含有率		
#define YC_41E9	0x41E9 ///< 线路2 C相 10次电流谐波含有率	
#define YC_41EA	0x41EA ///< 线路2 C相 11次电流谐波含有率	
#define YC_41EB	0x41EB ///< 线路2 C相 12次电流谐波含有率	
#define YC_41EC	0x41EC ///< 线路2 C相 13次电流谐波含有率	
#define YC_41ED	0x41ED ///< 线路2 C相 14次电流谐波含有率	
#define YC_41EE	0x41EE ///< 线路2 C相 15次电流谐波含有率	
#define YC_41EF	0x41EF ///< 线路2 C相 16次电流谐波含有率	
#define YC_41F0	0x41F0 ///< 线路2 C相 17次电流谐波含有率	
#define YC_41F1	0x41F1 ///< 线路2 C相 18次电流谐波含有率	
#define YC_41F2	0x41F2 ///< 线路2 C相 19次电流谐波含有率	


#define YC_4401 0x4401 ///<两线路相角差
#define YC_4402 0x4402 ///<两线路电压差

#define YC_4601 0x4601 ///<GPS经度
#define YC_4602 0x4602 ///<GPS纬度
#define YC_4603 0x4603 ///<直流量1
#define YC_4604 0x4604 ///<直流量2
#define YC_4605 0x4605 ///<蓄电池电压
#define YC_4606 0x4606 ///<温度
#define YC_4607 0x4607 ///<湿度



/**
* /brief 遥控类数据标识
*/
#define YK_6001 0x6001 ///<断路器1（开/合）
#define YK_6002 0x6002 ///<断路器1（开/合解合环）
#define YK_6003 0x6003 ///<断路器2（开/合）
#define YK_6004 0x6004 ///<断路器2（开/合解合环）

#define YK_6064 0x6064 ///<电池活化（激活/退出）         
#define YK_6065 0x6065 ///<电池退出                
#define YK_6066 0x6066 ///<电动机操作机构电源投入/退出   



/**
* /brief 参数类数据标识
*/
/*第一套保护定值*/
#define PARA_5001 0x5001 ///< I段电流定值（瞬时电流速断保护）
#define PARA_5002 0x5002 ///< II段电流定值（定时限电流速断保护）
#define PARA_5003 0x5003 ///<III段电流定值（定时限过电流保护）
#define PARA_5004 0x5004 ///<零序I段电流保护定值
#define PARA_5005 0x5005 ///<零序II段电流保护定值
#define PARA_5006 0x5006 ///<零序III段电流保护定值
#define PARA_5007 0x5007 ///<零序电压保护定值
#define PARA_5008 0x5008 ///<电流方向保护投入定值
#define PARA_5009 0x5009 ///<零序电流方向保护投入定值
/*第二套保护定值*/
#define PARA_500A 0x500A ///< I段电流定值（瞬时电流速断保护）    
#define PARA_500B 0x500B ///< II段电流定值（定时限电流速断保护）  
#define PARA_500C 0x500C ///<III段电流定值（定时限过电流保护）   
#define PARA_500D 0x500D ///<零序I段电流保护定值           
#define PARA_500E 0x500E ///<零序II段电流保护定值          
#define PARA_500F 0x500F ///<零序III段电流保护定值         
#define PARA_5010 0x5010 ///<零序电压保护定值             
#define PARA_5011 0x5011 ///<电流方向保护投入定值           
#define PARA_5012 0x5012 ///<零序电流方向保护投入定值         
/*第三套保护定值*/
#define PARA_5013 0x5013 ///< I段电流定值（瞬时电流速断保护）    
#define PARA_5014 0x5014 ///< II段电流定值（定时限电流速断保护）  
#define PARA_5015 0x5015 ///<III段电流定值（定时限过电流保护）   
#define PARA_5016 0x5016 ///<零序I段电流保护定值           
#define PARA_5017 0x5017 ///<零序II段电流保护定值          
#define PARA_5018 0x5018 ///<零序III段电流保护定值         
#define PARA_5019 0x5019 ///<零序电压保护定值             
#define PARA_501A 0x501A ///<电流方向保护投入定值           
#define PARA_501B 0x501B ///<零序电流方向保护投入定值         

/*第四套保护定值*/
#define PARA_501C 0x501C ///< I段电流定值（瞬时电流速断保护）    
#define PARA_501D 0x501D ///< II段电流定值（定时限电流速断保护）  
#define PARA_501E 0x501E ///<III段电流定值（定时限过电流保护）   
#define PARA_501F 0x501F ///<零序I段电流保护定值           
#define PARA_5020 0x5020 ///<零序II段电流保护定值          
#define PARA_5021 0x5021 ///<零序III段电流保护定值         
#define PARA_5022 0x5022 ///<零序电压保护定值             
#define PARA_5023 0x5023 ///<电流方向保护投入定值           
#define PARA_5024 0x5024 ///<零序电流方向保护投入定值         

/*第五套保护定值*/
#define PARA_5025 0x5025 ///< I段电流定值（瞬时电流速断保护）    
#define PARA_5026 0x5026 ///< II段电流定值（定时限电流速断保护）  
#define PARA_5027 0x5027 ///<III段电流定值（定时限过电流保护）   
#define PARA_5028 0x5028 ///<零序I段电流保护定值           
#define PARA_5029 0x5029 ///<零序II段电流保护定值          
#define PARA_502A 0x502A ///<零序III段电流保护定值         
#define PARA_502B 0x502B ///<零序电压保护定值             
#define PARA_502C 0x502C ///<电流方向保护投入定值           
#define PARA_502D 0x502D ///<零序电流方向保护投入定值         

/*第六套保护定值*/
#define PARA_502E 0x502E ///< I段电流定值（瞬时电流速断保护）    
#define PARA_502F 0x502F ///< II段电流定值（定时限电流速断保护）  
#define PARA_5030 0x5030 ///<III段电流定值（定时限过电流保护）   
#define PARA_5031 0x5031 ///<零序I段电流保护定值           
#define PARA_5032 0x5032 ///<零序II段电流保护定值          
#define PARA_5033 0x5033 ///<零序III段电流保护定值         
#define PARA_5034 0x5034 ///<零序电压保护定值             
#define PARA_5035 0x5035 ///<电流方向保护投入定值           
#define PARA_5036 0x5036 ///<零序电流方向保护投入定值         

/*第七套保护定值*/
#define PARA_5037 0x5037 ///< I段电流定值（瞬时电流速断保护）    
#define PARA_5038 0x5038 ///< II段电流定值（定时限电流速断保护）  
#define PARA_5039 0x5039 ///<III段电流定值（定时限过电流保护）   
#define PARA_503A 0x503A ///<零序I段电流保护定值           
#define PARA_503B 0x503B ///<零序II段电流保护定值          
#define PARA_503C 0x503C ///<零序III段电流保护定值         
#define PARA_503D 0x503D ///<零序电压保护定值             
#define PARA_503E 0x503E ///<电流方向保护投入定值           
#define PARA_503F 0x503F ///<零序电流方向保护投入定值         

/*第八套保护定值*/
#define PARA_5040  0x5040 ///< I段电流定值（瞬时电流速断保护）    
#define PARA_5041  0x5041 ///< II段电流定值（定时限电流速断保护）  
#define PARA_5042  0x5042 ///<III段电流定值（定时限过电流保护）   
#define PARA_5043  0x5043 ///<零序I段电流保护定值           
#define PARA_5044  0x5044 ///<零序II段电流保护定值          
#define PARA_5045  0x5045 ///<零序III段电流保护定值         
#define PARA_5046  0x5046 ///<零序电压保护定值             
#define PARA_5047  0x5047 ///<电流方向保护投入定值           
#define PARA_5048  0x5048 ///<零序电流方向保护投入定值         
#define PARA_5049  0x5049 ///<线路1保护定值区号设置
#define PARA_504A  0x504A ///<线路2保护定值区号设置

#define PARA_504B  0x504B ///<重合闸设置

/*第一套开关FA功能设置*/
#define PARA_504C  0x504C ///<FA功能设置
#define PARA_504D  0x504D ///<分段开关设置
#define PARA_504E  0x504E ///<联络开关设置
/*第二套开关FA功能设置*/
#define PARA_504F  0x504F ///<FA功能设置   
#define PARA_5050  0x5050 ///<分段开关设置   
#define PARA_5051  0x5051 ///<联络开关设置   
#define PARA_5052  0x5052 ///<线路1 FA定值区号设置
#define PARA_5053  0x5053 ///<线路2 FA定值区号设置
#define PARA_5054  0x5054 ///<FA功能公用参数设置

#define PARA_5055  0x5055 ///<保护功能投退设置

/**
* /brief 数据采集相关参数设置
*/
#define PARA_5080 0x5080 ///<基波算法还是全波算法
#define PARA_5081 0x5081 ///<0x5081~0x5084模拟量输入配置 指模拟输入量的硬件与需要采集的模拟量之间联系配置
#define PARA_5082 0x5082 ///<0x5081~0x5084模拟量输入配置 指模拟输入量的硬件与需要采集的模拟量之间联系配置
#define PARA_5083 0x5083 ///<0x5081~0x5084模拟量输入配置 指模拟输入量的硬件与需要采集的模拟量之间联系配置
#define PARA_5084 0x5084 ///<0x5081~0x5084模拟量输入配置 指模拟输入量的硬件与需要采集的模拟量之间联系配置
#define PARA_5085 0x5085 ///<0x5085~0x5088单点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置
#define PARA_5086 0x5086 ///<0x5085~0x5088单点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置
#define PARA_5087 0x5087 ///<0x5085~0x5088单点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置
#define PARA_5088 0x5088 ///<0x5085~0x5088单点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置
#define PARA_5089 0x5089 ///<双点遥信开关量输入配置 指开关输入量的硬件与需要采集的开关量之间联系配置
#define PARA_508a 0x508a ///<开关量输出配置 指开关输出量的硬件与需要控制的开关量之间联系配置
#define PARA_508B 0x508b ///<功率计算方法配置 2PT，3CT；2PT，2CT；1PT，2CT；1PT，3CT；3PT，3CT等
#define PARA_508C 0x508c  ///<直流输入量配置 是指直流输入量的是采集电流（4~20mA）还是电压（0~48V）；
#define PARA_508D 0x508d  ///<电流标称值是5A还是1A,
#define PARA_508E 0x508e ///<电压220V还是110V，角度60°还是30°
#define PARA_508F 0x508f ///<遥测死区值设置 按满刻度百分比,有哪些值需要设置？？电压、电流、功率、频率
#define PARA_5090 0x5090 ///<遥信防抖时间 
#define PARA_5091 0x5091 ///<遥控返校失败取消遥控时间 
#define PARA_5092 0x5092  ///<遥控保持时间 
#define PARA_5093 0x5093 ///<遥控脉冲宽度  
#define PARA_5094 0x5094  ///<延时断开操作电源时间
#define PARA_5095 0x5095  ///<PT
#define PARA_5096 0x5096 ///<CT
#define PARA_5097 0x5097  ///<标定
#define PARA_5098 0x5098 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_5099 0x5099 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509A 0x509A ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509B 0x509B ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509C 0x509C ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509D 0x509D ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509E 0x509E ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_509F 0x509F ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A0 0x50A0 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A1 0x50A1 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A2 0x50A2 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A3 0x50A3 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A4 0x50A4 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A5 0x50A5 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A6 0x50A6 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A7 0x50A7 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A8 0x50A8 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50A9 0x50A9 ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50AA 0x50AA ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50AB 0x50AB ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置
#define PARA_50AC 0x50AC ///<遥测信息体属性配置 0x5098~0x50Ac均属于遥测信息体配置


#define PARA_50AD 0x50AD  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50AE 0x50AE  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50AF 0x50AF  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B0 0x50B0  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B1 0x50B1  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B2 0x50B2  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B3 0x50B3  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B4 0x50B4  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B5 0x50B5  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B6 0x50B6  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置
#define PARA_50B7 0x50B7  ///<遥信信息体属性配置 0x50Ad~0x50B7均属于遥测信息体配置



#define PARA_50B8 0x50B8  ///<遥控信息体属性配置


/**
* /brief 通讯相关参数设置
*/
#define PARA_5100 0x5100   ///<101相关参数：
#define PARA_5101 0x5101   ///<104相关参数：
#define PARA_5102 0x5102   ///<以太网通讯参数
#define PARA_5103 0x5103   ///<无线通讯参数
#define PARA_5104 0x5104   ///<串行口1配置
#define PARA_5105 0x5105   ///<串行口2配置
#define PARA_5106 0x5106   ///<通讯公共参数  


/**
* /brief 杂项相关参数设置
*/
#define PARA_5200 0x5200/**密码*/
#define PARA_5201 0x5201/**版本*/
#define PARA_5202 0x5202/**温度定值*/
#define PARA_5203 0x5203/**湿度定值*/
#define PARA_5204 0x5204/**经度坐标*/
#define PARA_5205 0x5205/**纬度坐标*/
#define PARA_5206 0x5206/**恢复出厂设置*/
#define PARA_5207 0x5207/**蓄电池欠压切除阈值*/
#define PARA_5208 0x5208/**蓄电池低压告警阈值*/
#define PARA_5209 0x5209/**蓄电池活化充放电间隔时间*/
#define PARA_520A 0x520A/*电压上限告警阈值，恢复阈值，持续时间*/
#define PARA_520B 0x520B/*电压下限告警阈值，恢复阈值，持续时间*/
#define PARA_520C 0x520C/*电流上限阈值，恢复阈值，持续时间*/
#define PARA_520D 0x520D/*零序电流越上限阈值，恢复阈值，持续时间*/
#define PARA_520E 0x520E/*遥测历史数据项定义*/
#define PARA_520F 0x520F/*遥测历史数据存储间隔*/
#define PARA_5210 0x5210/*遥测极值数据项定义*/
#define PARA_5211 0x5211/*人工值设置使能*/
#define PARA_5212 0x5212/*人工设置数据*/
#define PARA_5213 0x5213/*CP56Time2a格式终端时间*/



#define SYS_7001  0x7001/*链路断开*/
#define SYS_7002  0x7002/*链路建立*/
#define SYS_7003  0x7003/*交流标定系数*/
#define SYS_7004  0x7003/*直流标定系数*/

#define SYS_7010  0x7010/*硬遥信事件记录*/
#define SYS_7011  0x7011/*软遥信事件记录*/
#define SYS_7012  0x7012/*掉电事件记录*/
#define SYS_7013  0x7013/*动作事件记录*/
#define SYS_7014  0x7014/*终端故障事件记录*/


/**
* /brief 基础应用数据类型
*/

class BaseDataType
{

public :
    int32 DataType ;///<数据类型ﺿ-类型1,2-类型2.。⾿
};

/**
* /brief 数据类型1 ：int8
*/

class DataType1: public BaseDataType
{

public:
    int8   Data ;
    DataType1() {
        DataType = 1;
    }
};

/**
* /brief 数据类型2 ：int16
*/

class DataType2: public BaseDataType
{

public:
    int16 __attribute__ ((packed)) Data ;
    DataType2() {
        DataType = 2;
    }
};

/**
* /brief 数据类型3 ：int32
*/

class DataType3: public BaseDataType
{

public:
    int32 __attribute__ ((packed)) Data ;
    DataType3() {
        DataType = 3;
    }
};

/**
* /brief 数据类型4 ：float
*/

class DataType4: public BaseDataType
{

public:
    float __attribute__ ((packed)) Data ;
    DataType4() {
        DataType = 4;
        Data = 0;
    }
};

/**
* /brief 数据类型5 ：double
*/

class DataType5: public BaseDataType
{

public:
    double __attribute__ ((packed)) Data ;
    DataType5() {
        DataType = 5;
    }
};


///////////////////////////////////////////////////////////////////////////////
//参数类
/**
* /brief 数据类型6：保护参数
*/

class ProtectSet: public BaseDataType
{

public:
    ProtectSet() {
        DataType = 6;
    }

    struct {
        float LimitValue;///<限值
        int32 LimitTime;///<时间限值
        int8 Enable;///<是否投入保护
        int8  EnableOut;///<是否允许出口
    }  __attribute__ ((packed)) Data;
};

/**
* /brief 数据类型7：保护方向参数
*/

class ProtectDirSet: public BaseDataType
{

public:
    ProtectDirSet() {
        DataType = 7;
    }

    struct {
        int8 Enable;
        int8 Dir;
        int8 Anagle;
    }Data  ;
};

/**
* /brief 数据类型8：重合闸参数
*/

class RecloseSet: public BaseDataType
{

public:
    RecloseSet() {
        DataType = 8;
    }

    struct {
        int8  Times;///<重合闸次数
        int8  Type;///<重合闸方式
        int32 Time[3];///<三次重合闸时间
        int8  Enable;///<后加速允许
        int32 AcceTimes[4];///<后加速时间
        int32 LockTime;///<闭锁2次重合延时
        float DiffVol;///<开关两侧电压差
        float DiffPhase;///<相位差设置
        int16 DiffVolDefine[2];///<开关两侧电压定义
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型9：FA功能设置参数
*/

class FaFunSet: public BaseDataType
{

public:
    FaFunSet() {
        DataType = 10;
    }

    struct {
        float NoShutOffCurr;///<非遮断电流
        int8  Type;///<开关方式
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型11：FA功能中分段开关参数
*/

class FaIsolationSet: public BaseDataType
{

public:
    FaIsolationSet() {
        DataType = 11;
    }

    struct {
        int32 LosePowerTime;///<双侧失电延时时间（T时限）
        int8  EnableOpenSwitch;///<双侧失电延时分闸功能压板
        int32 GetPowerTime;///<单侧得电延时时间（X时限）
        int8  EnableCloseSwitch;///<单侧得电延时合闸功能压板
        int32 LockCloseSwitchTime;///<闭锁合闸时间（Y时限）
        int8  EnableLockCloseSwitch;///<闭锁合闸功能压板
        float FaultCurrent;///<闭锁合闸故障电流定值
        int8  EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压)
        int32 LockOpenSwitchTime;///<无故障闭锁分闸时限
        int8  LockOpenSwitchResetTime;///<无故障闭锁分闸复归时限
        int8  EnableLockOpenSwitch;///<无故障闭锁分闸功能压板
        float RemainVol;///<残压定值
        int8  EnableChkRemainVol;///<残压检测功能允许
        float LimitU0;///<零序电压保护定值U0
        int32 LimitU0Time;///<零序电压保护时间定值
        int8  EnableU0;///<零序电压保护投入定值
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型12：FA功能中联络开关参数
*/

class FaConnectionSet: public BaseDataType
{

public:
    FaConnectionSet() {
        DataType = 12;
    }

    struct {
        int32 CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
        int8  EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许
        int32 LockCloseSwitchTime;///<双侧得电闭锁开关合闸时间
        int8  EnableLockCloseSwitch;///<双侧得电闭锁开关合闸功能允许
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型13：FA功能中公共参数
*/

class FaCommSet: public BaseDataType
{

public:
    FaCommSet() {
        DataType = 13;
    }

    struct {
        float StartI;
        float StartU;
        int32 ResetFaTime;
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型14：FA功能中投退参数
*/

class FaDevotionSet: public BaseDataType
{

public:
    FaDevotionSet() {
        DataType = 14;
    }

    struct {
        int8 EnableYCEvent;     ///<遥测越限检测功能投退
        int8 EnableCurrentProtect;  ///<基本保护功能投退
        int8 EnableFa;              ///<FA功能投退
        int8 EnableReclose;         ///<重合闸投退
        int8 EnablePtFun;           ///<当PT断线时是否自动退出方向元件以及检同期元件,FA功能
    }Data ;
};


/**
* /brief SCADA相关参数
*/
/**
* /brief 数据类型15：硬件配置
*/

class UserPortSet: public BaseDataType
{

public:
    UserPortSet() {
        DataType = 15;
    }

    struct {
        int8 Num;

        struct {
            int16 Id;
            int16 Port;
        }Define[50];
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型16：开关量输入配置
*/

class DbDigitInSet: public BaseDataType
{

public:
    DbDigitInSet() {
        DataType = 16;
    }

    struct {
        int8 Num;

        struct {
            int16 Id;
            int16 ClosePort;
            int16 OpenPort;
        }Define[18];
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型18：遥测死区配置
*/

class DeadAreaSet: public BaseDataType
{

public:
    DeadAreaSet() {
        DataType = 18;
    }

    struct {
        float DeadU;   ///<电压死区设置
        float DeadI;      ///<电流死区设置
        float DeadP;      ///<功率死区设置
        float DeadFreq;   ///<频率死区设置
        float DeadAngle;///<角度死区设置
        float DeadHarmoicValue;///<谐波有效值死区设置
        float DeadHarmoicCount;///<   谐波含有率死区设置
        float DeadTemp;   ///<温度死区设置
        float DeadHumidity;///<湿度死区设置
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型19：遥测信息体配置
*/

class YCMsgSet: public BaseDataType
{

public:
    YCMsgSet() {
        DataType = 19;
    }

    struct {
        int16 Num;

        struct {
            int16 DataId;
            int16 MsgId;
        }Define[50];
    }__attribute__ ((packed)) Data ;

};

/**
* /brief 数据类型20：遥信信息体配置
*/

class YXMsgSet: public BaseDataType
{

public:
    YXMsgSet() {
        DataType = 20;
    }

    struct {
        int16 Num;

        struct {
            int16 DataId;
            int16 MsgId;
            int16 Type;
        }Define[35];
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型21：遥控信息体配置
*/

class YKMsgSet: public BaseDataType
{

public:
    YKMsgSet() {
        DataType = 21;
    }

    struct {
        int16 Num;

        struct {
            int16 DataId;
            int16 MsgId;
        }Define[50];
    }__attribute__ ((packed)) Data ;
};




/**通讯相关参数**/
/**
* /brief 数据类型23：101规约参数
*/

class Pro101Para: public BaseDataType
{

public:
    Pro101Para() {
        DataType = 23;
    }

    struct {
        int8  TransMode;///<传输方式
        int8  AddressLen;///<地址域
        int16 RepeatTimes;///<重复帧传输时间间隔
        int8  TransCauseLen;///<传送原因
        int8  CommAddresLen;///<应用数据单元公共地址
        int8  MsgAddrLen;///<信息对象地址
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型24：104规约参数
*/

class Pro104Para: public BaseDataType
{

public:
    Pro104Para() {
        DataType = 24;
    }

    struct {
        int8  CommAddresLen;///<应用数据单元公共地址
        int8  MsgAddrLen;///<信息对象地址
        int16 T0;///<连接建立的超时t0
        int16 T1;///<发送或测试 APDU 的超时t1
        int16 T2;///<无数据报文 t2<t1时确认的超时t2
        int16 T3;///<长期空闲 t3 >t1状态下发送测试帧的超时 t3
        int16 T4;///<应用报文确认超时t4
        int16 K;///<发送方最大发送未被确认的I格式报文数k
        int16 W;///<接收方最大接受未被确认I格式报文数w
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型25：以太网参数
*/

class EthPara : public BaseDataType
{

public:
    EthPara() {
        DataType = 25;
    }

    struct {
        uint8 LocalIp[4];/**<  本地ip  */
        uint16 LocalPort;/**<  本地端口  */
        uint8 Mask[4];/**<  子网掩码  */
        uint8 Gate[4];/**<  网关  */
        uint8 Mac[6];/**<  mac地址  */
        uint8 Dns[4];/**<  域名解析服务器地址 */
        uint8 HostIp[4];/**<  主机ip  */
        uint16 HostPort;/**<  主机端口  */
        uint8 BakHostIp[4];/**<  备份主机ip  */
        uint16 BakHostPort;/**<  备份主机端口  */
        uint16 BakListenPort;/**<  备份通道监控端口  */
        int8 MasterProSel;/**<  通道规约选择  */
        int8 MasterMode;/**<  工作模式1-服务器模式，2-客户端模式*/
        int8 MasterUse;/**<  启用标志*/
        int8 BakProSel;/**<  通道规约选择  */
        int8 BakMode;/**<  工作模式1-服务器模式，2-客户端模式*/
        int8 BakUse;/**<  启用标志*/
    }__attribute__ ((packed)) Data ;
} ;

/**
* /brief 数据类型26：无线通讯参数
*/

class WirelessPara : public BaseDataType
{

public:
    WirelessPara() {
        DataType = 26;
    }

    struct {
        uint8 Vpn[32];/**<  cmnet etc. ascii 码 */
        uint8 UserName[32];/**<  用户名称  ascii 码*/
        uint8 Passwd[32];/**<  密码ascii 码 */
        uint8 LocalIp[4];/**<  本地ip  */
        uint16 LocalPort;/**<  本地端口  */
        
        uint8 HostIp[4];/**<  主机ip  */
        uint16 HostPort;/**<  主机端口  */
        uint8 BakHostIp[4];/**<  备份主机ip  */
        uint16 BakHostPort;/**<  备份主机端口  */
        uint16 BakListenPort;/**<  备份通道监控端口  */        
        int32 MonFlowSet;/**<  月流量限制 上下行 单位KB  */
        int8 MasterProSel;/**<  通道规约选择  */
        int8 MasterMode;/**<  工作模式1-服务器模式，2-客户端模式*/
        int8 MasterUse;/**<  启用标志*/
        int8 BakProSel;/**<  通道规约选择  */
        int8 BakMode;/**<  工作模式1-服务器模式，2-客户端模式*/
        int8 BakUse;/**<  启用标志*/
        int8 WireLessType;/**<  网络选择 */
        //char imei[32];/**< sim卡串卿ascii 码 */
        //char brand[32];/**<  通信运营哿ascii 码 */
        //char type[8];/**<   2G 3G 4G ascii 码*/
        //char phone_num[16];/**<  电话号码 ascii 码 */
        //char sn[32];/**<  无线模块串号 ascii 码 */
        //char substation_info[32];/**<  基站信息 ascii 码*/
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型27：串口参数
*/

class UartPara : public BaseDataType
{

public:
    UartPara() {
        DataType = 27;
    }

    struct {
        int32 BaudRate;/**<  波特率:300,600,1200,2400,4800,9600,115200,38400 */
        int8 DataBit;/**<  数据位 7,8*/
        int8 StopBit;/**<  停止位 1-1位停止位,2-2位停止位,3-1。5位停止位*/
        int8 CheckBit;/**<  校验位 0:无校验，1 奇校验 2 偶校验*/
        int8  ProSel;/**<  通道规约选择  */
    }__attribute__ ((packed)) Data ;


} ;

/**
* /brief 数据类型28：基本通讯参数
*/

class ComBasePara: public BaseDataType
{

public:
    ComBasePara() {
        DataType = 28;
    }

    struct {
        int16 LocalId;/**<终端地址*/
        int16 HostId;/**<主站地址*/
        int8  YcDataType;/**<上送遥测数据类型：标度化还是归一化，短浮点数传输.默认标度化.*/
        int16 RelinkTime;/**<  断线重连间隔时间 单位 秒 */
        int16 TransPort;/**<  转发端口号，如果为零，不转发  */
        int16 ByteOverTime;/**<  字节超时等待时间 单位 ms  */
		int16 PointsYXGroup;///<每组遥信点数
		int16 PointsYCGroup;///<每组遥测点数
 
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型29：密码
*/

class PassWdPara: public BaseDataType
{

public:
    PassWdPara() {
        DataType = 29;
    }

    struct {
        uint8 pass[6];
    }Data  ;
};

/**
* /brief 数据类型30：告警参数
*/

class WarnPara: public BaseDataType
{

public:
    WarnPara() {
        DataType = 30;
    }

    struct {
        float Limit;
        float Recover;
        int32 ContinueTime;
    }__attribute__ ((packed)) Data ;
};

/**
* /brief 数据类型31：历史数据项定义参数
*/

class HisDataDefinePara: public BaseDataType
{

public:
    HisDataDefinePara() {
        DataType = 31;
    }

    struct {
        int8 Num;
        int8 DefineData[30];
    }Data  ;
};

/**
* /brief 数据类型32：人工置数参数
*/

class UserRealDataDefPara: public BaseDataType
{

public:
    UserRealDataDefPara() {
        DataType = 32;
    }

    struct {

        typedef struct _BaseSet {
            float BaseValue;
            float BaseHz;
            int16 HarmoDgree;
            float HarmContent;
        }BaseSet;

        BaseSet Ua;
        BaseSet Ub;
        BaseSet Uc;
        BaseSet Ia;
        BaseSet Ib;
        BaseSet Ic;
        float   IaAngle;
        float   IbAngle;
        float   IcAngle;
    }__attribute__ ((packed)) Data ;
};

class PowerWay:public BaseDataType{
public:
	PowerWay(){DataType = 33;}
	struct{
		uint8 line[MAX_LINE_NUM];
	}__attribute__ ((packed)) Data;
};
class TeminalTime:public BaseDataType{
public:
	TeminalTime(){DataType = 34;}
	struct{
		CP56Time2a time;
	}__attribute__ ((packed)) Data;
};
/**
* /brief 应用数据类
*/

template  <class T>  class Data
{

public:
    int32 Id;//数据标识
    int16 Size;//Data成员中的Data成员（实际数值）数据长度
    T* Data;
    int32 MsgAddr;//信息体地址
    float UnitaryCoff;///<归一化系数
    float ScaleCoff;///<标度化系数
    float ShorFloCoff;///<短浮点系数
};

/**
* /brief 消息数据类
*/
#define MSG_YX    1
#define MSG_YC    2
#define MSG_YK    3
#define MSG_PARA   4
#define MSG_TIMER   5
#define MSG_STEP 6

template  <class T>  class MessageData
{

public:
    uint32 Owner;//消息所有者
    int32 MessageId;//1代表遥信变化ﺿ代表遥测变化ﺿ代表遥控变化,4代表参数变化
    int32 Id;//数据标识
    int16 Size;//Data成员中的Data成员（实际数值）数据长度
    T* Data;
    CP56Time2a Time;//年月日时分秒毫秒
}__attribute__ ((packed)) ;

class UserData
{
private:		
		int16 SaveParaAll();
		int16(*UserSetDataFun)(int32 );//用户调用的当调用SetData后需要调用的函数
public:

    struct cond cond;
    UserData();
    ~UserData();
    int16 GetData ( int32 Id, Data<BaseDataType>* data );
    int16 SetData ( int32 Id, Data<BaseDataType>*data );
    int16 GetData ( int32 Id, Data<BaseDataType>* data,char * r_data);
    int16 SetData ( int32 Id, char* data );
        
    int16 GetDataSize ( int32 Id );//返回数据长度
    int16 GetDataType ( int32 Id );//返回数据类型
    Data<BaseDataType>* GetDataPtr ( int32 Id );//返回数据指针
	
	Data<BaseDataType> *RecallData(uint16 DataType, uint16 DataPosInTab);

    int16 GetDataTableSize ( int32 tb );//返回数据表大小
    Data<BaseDataType>* GetDataTablePtr ( int32 tb );//返回数据表指针


    int16 LoadPara();
    int16 SavePara ( int32 Id );
    int16 Init();
	void SetUserDataFun ( int16(*fun)(int32 Id) );
};

extern  UserData user_data;
#endif
