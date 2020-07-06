#include <string.h>
#include "os.h"
#include "datatype.h"
#include "debug.h"

/**
* /brief scada数据
*/

typedef struct da {
//遥信类数据
    DataType1 d_0001;///<断路器1合位
    DataType1 d_0002;///<断路器1分位
    DataType1 d_0003;///<断路器1储能位置
    DataType1 d_0004;///<断路器1隔离刀闸位置（备用遥信4）
    DataType1 d_0005;///<备用遥信5变位
    DataType1 d_0006;///<备用遥信6变位
    DataType1 d_0007;///<线路1一段故障信号
    DataType1 d_0008;///<线路1二段故障信号
    DataType1 d_0009;///<线路1三段故障信号
    DataType1 d_000A;///<线路1单相接地故障信号
    DataType1 d_000B;///<线路1零序电压故障信号
    DataType1 d_000C;///<线路1一段保护动作信号
    DataType1 d_000D;///<线路1二段保护动作信号
    DataType1 d_000E;///<线路1三段保护动作信号
    DataType1 d_000F;///<线路1单相接地零序电流保护动作信号
    DataType1 d_0010;///<线路1零序电压保护动作信号
    DataType1 d_0011;///<故障指示器指示线路1相间短路故障
    DataType1 d_0012;///<故障指示器指示线路1接地故障
    DataType1 d_0013;///<断路器1闭锁
    DataType1 d_0014;///<断路器1异常
    DataType1 d_0015;///<断路器1 SF6气体异常报警信号
    DataType1 d_0016;///<断路器1 SF6气压闭锁信号
    DataType1 d_0017;///<线路1PT断线
    DataType1 d_0018;///<线路1CT断线
    DataType1 d_0019;///<线路1启动重合闸
    DataType1 d_001A;///<线路1重合闸成功/失败
    DataType1 d_001B;///<断路器2合位
    DataType1 d_001C;///<断路器2分位
    DataType1 d_001D;///<断路器2储能位置
    DataType1 d_001E;///<断路器2隔离刀闸位置（备用遥信10）
    DataType1 d_001F;///<备用遥信11
    DataType1 d_0020;///<备用遥信12
    DataType1 d_0021;///<线路2一段故障信号
    DataType1 d_0022;///<线路2二段故障信号
    DataType1 d_0023;///<线路2三段故障信号
    DataType1 d_0024;///<线路2单相接地故障信号
    DataType1 d_0025;///<线路2零序电压故障信号
    DataType1 d_0026;///<线路2一段保护动作信号
    DataType1 d_0027;///<线路2二段保护动作信号
    DataType1 d_0028;///<线路2三段保护动作信号
    DataType1 d_0029;///<线路2单相接地保护动作信号
    DataType1 d_002A;///<线路2零序电压保护动作信号
    DataType1 d_002B;///<故障指示器指示线路2相间短路故障
    DataType1 d_002C;///<故障指示器指示线路2接地故障
    DataType1 d_002D;///<断路器2闭锁
    DataType1 d_002E;///<断路器2异常
    DataType1 d_002F;///<断路器2 SF6气体异常报警信号
    DataType1 d_0030;///<断路器2 SF6气压闭锁信号
    DataType1 d_0031;///<线路2PT断线
    DataType1 d_0032;///<线路2CT断线
    DataType1 d_0033;///<线路2启动重合闸
    DataType1 d_0034;///<线路2重合闸成功/失败
    DataType1 d_0035;///<远方/就地位置
    DataType1 d_0036;///<电池低压告警/恢复
    DataType1 d_0037;///<电池欠压告警/恢复
    DataType1 d_0038;///<交流失电告警/恢复
    DataType1 d_0039;///<装置工作电源异常告警/恢复
    DataType1 d_003A;///<终端异常/恢复
    DataType1 d_003B;///<线路1就地动作信号
    DataType1 d_003C;///<线路2就地动作信号


    DataType1 d_0201;///<线路1 A相电压越上限告警
    DataType1 d_0202;///<线路1 B相电压越上限告警
    DataType1 d_0203;///<线路1 C相电压越上限告警
    DataType1 d_0204;///<线路1 A相电压越下限告警
    DataType1 d_0205;///<线路1 B相电压越下限告警
    DataType1 d_0206;///<线路1 C相电压越下限告警
    DataType1 d_0207;///<线路1 A相电流越上限告警
    DataType1 d_0208;///<线路1 B相电流越上限告警
    DataType1 d_0209;///<线路1 C相电流越上限告警
    DataType1 d_020A;///<线路1 负荷越上限告警
    DataType1 d_020B;///<线路2 A相电压越上限告警
    DataType1 d_020C;///<线路2 B相电压越上限告警
    DataType1 d_020D;///<线路2 C相电压越上限告警
    DataType1 d_020E;///<线路2 A相电压越下限告警
    DataType1 d_020F;///<线路2 B相电压越下限告警
    DataType1 d_0210;///<线路2 C相电压越下限告警
    DataType1 d_0211;///<线路2 A相电流越上限告警
    DataType1 d_0212;///<线路2 B相电流越上限告警
    DataType1 d_0213;///<线路2 C相电流越上限告警
    DataType1 d_0214;///<线路2 负荷越上限告警
    DataType1 d_0215;///<温度越限
    DataType1 d_0216;///<温度越限
    DataType1 d_0217;///<主通道月流量超标越限
    DataType1 d_0218;///<备通道月流量超标越限
    DataType1 d_0219;///<<断路器1双点遥信
    DataType1 d_021A;///<<断路器2双点遥信
//遥测类数据
    DataType4 d_4001; ///<线路1 A相电压
    DataType4 d_4002; ///<线路1 B相电压
    DataType4 d_4003; ///<线路1 C相电压
    DataType4 d_4004; ///<线路1 AB线电压
    DataType4 d_4005; ///<线路1 BC线电压
    DataType4 d_4006; ///<线路1 零序电压
    DataType4 d_4007; ///<线路1 A相测量电流
    DataType4 d_4008; ///<线路1 B相测量电流
    DataType4 d_4009; ///<线路1 C相测量电流
    DataType4 d_400A; ///<线路1 零序电流
    DataType4 d_400B; ///<线路1 A相保护电流
    DataType4 d_400C; ///<线路1 B相保护电流
    DataType4 d_400D; ///<线路1 C相保护电流
    DataType4 d_400E; ///<线路1 A相有功功率
    DataType4 d_400F; ///<线路1 A相无功功率
    DataType4 d_4010; ///<线路1 A相功率因数
    DataType4 d_4011; ///<线路1 B相有功功率
    DataType4 d_4012; ///<线路1 B相无功功率
    DataType4 d_4013; ///<线路1 B相功率因数
    DataType4 d_4014; ///<线路1 C相有功功率
    DataType4 d_4015; ///<线路1 C相无功功率
    DataType4 d_4016; ///<线路1 C相功率因数
    DataType4 d_4017; ///<线路1 总有功功率
    DataType4 d_4018; ///<线路1 总无功功率
    DataType4 d_4019; ///<线路1 总视在功率
    DataType4 d_401A; ///<线路1 总功率因数
    DataType4 d_401B; ///<线路1 频率
		DataType4 d_401C;	///<线路1 A相 2次电压谐波有效值													
		DataType4 d_401D;  ///<线路1 A相 3次电压谐波有效值		                       
		DataType4 d_401E;  ///<线路1 A相 4次电压谐波有效值		                       
		DataType4 d_401F;  ///<线路1 A相 5次电压谐波有效值		                       
		DataType4 d_4020;  ///<线路1 A相 6次电压谐波有效值		                       
		DataType4 d_4021;  ///<线路1 A相 7次电压谐波有效值		                       
		DataType4 d_4022;  ///<线路1 A相 8次电压谐波有效值		                       
		DataType4 d_4023;  ///<线路1 A相 9次电压谐波有效值		                       
		DataType4 d_4024;  ///<线路1 A相 10次电压谐波有效值	                     
		DataType4 d_4025;  ///<线路1 A相 11次电压谐波有效值	                     
		DataType4 d_4026;  ///<线路1 A相 12次电压谐波有效值	                     
		DataType4 d_4027;  ///<线路1 A相 13次电压谐波有效值	                     
		DataType4 d_4028;  ///<线路1 A相 14次电压谐波有效值	                     
		DataType4 d_4029;  ///<线路1 A相 15次电压谐波有效值	                     
		DataType4 d_402A;  ///<线路1 A相 16次电压谐波有效值	                     
		DataType4 d_402B;  ///<线路1 A相 17次电压谐波有效值	                     
		DataType4 d_402C;  ///<线路1 A相 18次电压谐波有效值	                     
		DataType4 d_402D;  ///<线路1 A相 19次电压谐波有效值	                     
		DataType4 d_402E;  ///<线路1 A相 2次电流谐波有效值		
		DataType4 d_402F;  ///<线路1 A相 3次电流谐波有效值		
		DataType4 d_4030;  ///<线路1 A相 4次电流谐波有效值		
		DataType4 d_4031;  ///<线路1 A相 5次电流谐波有效值		
		DataType4 d_4032;  ///<线路1 A相 6次电流谐波有效值		
		DataType4 d_4033;  ///<线路1 A相 7次电流谐波有效值		
		DataType4 d_4034;  ///<线路1 A相 8次电流谐波有效值		
		DataType4 d_4035;  ///<线路1 A相 9次电流谐波有效值		
		DataType4 d_4036;  ///<线路1 A相 10次电流谐波有效值	
		DataType4 d_4037;  ///<线路1 A相 11次电流谐波有效值	
		DataType4 d_4038;  ///<线路1 A相 12次电流谐波有效值	
		DataType4 d_4039;  ///<线路1 A相 13次电流谐波有效值	
		DataType4 d_403A;  ///<线路1 A相 14次电流谐波有效值	
		DataType4 d_403B;  ///<线路1 A相 15次电流谐波有效值	
		DataType4 d_403C;  ///<线路1 A相 16次电流谐波有效值	
		DataType4 d_403D;  ///<线路1 A相 17次电流谐波有效值	
		DataType4 d_403E;  ///<线路1 A相 18次电流谐波有效值	
		DataType4 d_403F;  ///<线路1 A相 19次电流谐波有效值	
		DataType4 d_4040;  ///<线路1 A相 电压谐波总含有率		
		DataType4 d_4041;  ///<线路1 A相 2次电压谐波含有率		
		DataType4 d_4042;  ///<线路1 A相 3次电压谐波含有率		
		DataType4 d_4043;  ///<线路1 A相 4次电压谐波含有率		
		DataType4 d_4044;  ///<线路1 A相 5次电压谐波含有率		
		DataType4 d_4045;  ///<线路1 A相 6次电压谐波含有率		
		DataType4 d_4046;  ///<线路1 A相 7次电压谐波含有率		
		DataType4 d_4047;  ///<线路1 A相 8次电压谐波含有率		
		DataType4 d_4048;  ///<线路1 A相 9次电压谐波含有率		
		DataType4 d_4049;  ///<线路1 A相 10次电压谐波含有率	
		DataType4 d_404A;  ///<线路1 A相 11次电压谐波含有率	
		DataType4 d_404B;  ///<线路1 A相 12次电压谐波含有率	
		DataType4 d_404C;  ///<线路1 A相 13次电压谐波含有率	
		DataType4 d_404D;  ///<线路1 A相 14次电压谐波含有率	
		DataType4 d_404E;  ///<线路1 A相 15次电压谐波含有率	
		DataType4 d_404F;  ///<线路1 A相 16次电压谐波含有率	
		DataType4 d_4050;  ///<线路1 A相 17次电压谐波含有率	
		DataType4 d_4051;  ///<线路1 A相 18次电压谐波含有率	
		DataType4 d_4052;  ///<线路1 A相 19次电压谐波含有率	
		DataType4 d_4053;  ///<线路1 A相 电流谐波总含有率		
		DataType4 d_4054;  ///<线路1 A相 2次电流谐波含有率		
		DataType4 d_4055;  ///<线路1 A相 3次电流谐波含有率		
		DataType4 d_4056;  ///<线路1 A相 4次电流谐波含有率		
		DataType4 d_4057;  ///<线路1 A相 5次电流谐波含有率		
		DataType4 d_4058;  ///<线路1 A相 6次电流谐波含有率		
		DataType4 d_4059;  ///<线路1 A相 7次电流谐波含有率		
		DataType4 d_405A;  ///<线路1 A相 8次电流谐波含有率		
		DataType4 d_405B;  ///<线路1 A相 9次电流谐波含有率		
		DataType4 d_405C;  ///<线路1 A相 10次电流谐波含有率	
		DataType4 d_405D;  ///<线路1 A相 11次电流谐波含有率	
		DataType4 d_405E;  ///<线路1 A相 12次电流谐波含有率	
		DataType4 d_405F;  ///<线路1 A相 13次电流谐波含有率	
		DataType4 d_4060;  ///<线路1 A相 14次电流谐波含有率	
		DataType4 d_4061;  ///<线路1 A相 15次电流谐波含有率	
		DataType4 d_4062;  ///<线路1 A相 16次电流谐波含有率	
		DataType4 d_4063;  ///<线路1 A相 17次电流谐波含有率	
		DataType4 d_4064;  ///<线路1 A相 18次电流谐波含有率	
		DataType4 d_4065;  ///<线路1 A相 19次电流谐波含有率	
		DataType4 d_4066;  ///<线路1 B相 2次电压谐波有效值		
		DataType4 d_4067;  ///<线路1 B相 3次电压谐波有效值		
		DataType4 d_4068;  ///<线路1 B相 4次电压谐波有效值		
		DataType4 d_4069;  ///<线路1 B相 5次电压谐波有效值		
		DataType4 d_406A;  ///<线路1 B相 6次电压谐波有效值		
		DataType4 d_406B;  ///<线路1 B相 7次电压谐波有效值		
		DataType4 d_406C;  ///<线路1 B相 8次电压谐波有效值		
		DataType4 d_406D;  ///<线路1 B相 9次电压谐波有效值		
		DataType4 d_406E;  ///<线路1 B相 10次电压谐波有效值	
		DataType4 d_406F;  ///<线路1 B相 11次电压谐波有效值	
		DataType4 d_4070;  ///<线路1 B相 12次电压谐波有效值	
		DataType4 d_4071;  ///<线路1 B相 13次电压谐波有效值	
		DataType4 d_4072;  ///<线路1 B相 14次电压谐波有效值	
		DataType4 d_4073;  ///<线路1 B相 15次电压谐波有效值	
		DataType4 d_4074;  ///<线路1 B相 16次电压谐波有效值	
		DataType4 d_4075;  ///<线路1 B相 17次电压谐波有效值	
		DataType4 d_4076;  ///<线路1 B相 18次电压谐波有效值	
		DataType4 d_4077;  ///<线路1 B相 19次电压谐波有效值	
		DataType4 d_4078;  ///<线路1 B相 2次电流谐波有效值		
		DataType4 d_4079;  ///<线路1 B相 3次电流谐波有效值		
		DataType4 d_407A;  ///<线路1 B相 4次电流谐波有效值		
		DataType4 d_407B;  ///<线路1 B相 5次电流谐波有效值		
		DataType4 d_407C;  ///<线路1 B相 6次电流谐波有效值		
		DataType4 d_407D;  ///<线路1 B相 7次电流谐波有效值		
		DataType4 d_407E;  ///<线路1 B相 8次电流谐波有效值		
		DataType4 d_407F;  ///<线路1 B相 9次电流谐波有效值		
		DataType4 d_4080;  ///<线路1 B相 10次电流谐波有效值	
		DataType4 d_4081;  ///<线路1 B相 11次电流谐波有效值	
		DataType4 d_4082;  ///<线路1 B相 12次电流谐波有效值	
		DataType4 d_4083;  ///<线路1 B相 13次电流谐波有效值	
		DataType4 d_4084;  ///<线路1 B相 14次电流谐波有效值	
		DataType4 d_4085;  ///<线路1 B相 15次电流谐波有效值	
		DataType4 d_4086;  ///<线路1 B相 16次电流谐波有效值	
		DataType4 d_4087;  ///<线路1 B相 17次电流谐波有效值	
		DataType4 d_4088;  ///<线路1 B相 18次电流谐波有效值	
		DataType4 d_4089;  ///<线路1 B相 19次电流谐波有效值	
		DataType4 d_408A;  ///<线路1 B相 电压谐波总含有率		
		DataType4 d_408B;  ///<线路1 B相 2次电压谐波含有率		
		DataType4 d_408C;  ///<线路1 B相 3次电压谐波含有率		
		DataType4 d_408D;  ///<线路1 B相 4次电压谐波含有率		
		DataType4 d_408E;  ///<线路1 B相 5次电压谐波含有率		
		DataType4 d_408F;  ///<线路1 B相 6次电压谐波含有率		
		DataType4 d_4090;  ///<线路1 B相 7次电压谐波含有率		
		DataType4 d_4091;  ///<线路1 B相 8次电压谐波含有率		
		DataType4 d_4092;  ///<线路1 B相 9次电压谐波含有率		
		DataType4 d_4093;  ///<线路1 B相 10次电压谐波含有率	
		DataType4 d_4094;  ///<线路1 B相 11次电压谐波含有率	
		DataType4 d_4095;  ///<线路1 B相 12次电压谐波含有率	
		DataType4 d_4096;  ///<线路1 B相 13次电压谐波含有率	
		DataType4 d_4097;  ///<线路1 B相 14次电压谐波含有率	
		DataType4 d_4098;  ///<线路1 B相 15次电压谐波含有率	
		DataType4 d_4099;  ///<线路1 B相 16次电压谐波含有率	
		DataType4 d_409A;  ///<线路1 B相 17次电压谐波含有率	
		DataType4 d_409B;  ///<线路1 B相 18次电压谐波含有率	
		DataType4 d_409C;  ///<线路1 B相 19次电压谐波含有率	
		DataType4 d_409D;  ///<线路1 B相 电流谐波总含有率		
		DataType4 d_409E;  ///<线路1 B相 2次电流谐波含有率		
		DataType4 d_409F;  ///<线路1 B相 3次电流谐波含有率		
		DataType4 d_40A0;  ///<线路1 B相 4次电流谐波含有率		
		DataType4 d_40A1;  ///<线路1 B相 5次电流谐波含有率		
		DataType4 d_40A2;  ///<线路1 B相 6次电流谐波含有率		
		DataType4 d_40A3;  ///<线路1 B相 7次电流谐波含有率		
		DataType4 d_40A4;  ///<线路1 B相 8次电流谐波含有率		
		DataType4 d_40A5;  ///<线路1 B相 9次电流谐波含有率		
		DataType4 d_40A6;  ///<线路1 B相 10次电流谐波含有率	
		DataType4 d_40A7;  ///<线路1 B相 11次电流谐波含有率	
		DataType4 d_40A8;  ///<线路1 B相 12次电流谐波含有率	
		DataType4 d_40A9;  ///<线路1 B相 13次电流谐波含有率	
		DataType4 d_40AA;  ///<线路1 B相 14次电流谐波含有率	
		DataType4 d_40AB;  ///<线路1 B相 15次电流谐波含有率	
		DataType4 d_40AC;  ///<线路1 B相 16次电流谐波含有率	
		DataType4 d_40AD;  ///<线路1 B相 17次电流谐波含有率	
		DataType4 d_40AE;  ///<线路1 B相 18次电流谐波含有率	
		DataType4 d_40AF;  ///<线路1 B相 19次电流谐波含有率	
		DataType4 d_40B0;  ///<线路1 C相	2次电压谐波有效值		
		DataType4 d_40B1;  ///<线路1 C相 3次电压谐波有效值		
		DataType4 d_40B2;  ///<线路1 C相 4次电压谐波有效值		
		DataType4 d_40B3;  ///<线路1 C相 5次电压谐波有效值		
		DataType4 d_40B4;  ///<线路1 C相 6次电压谐波有效值		
		DataType4 d_40B5;  ///<线路1 C相 7次电压谐波有效值		
		DataType4 d_40B6;  ///<线路1 C相 8次电压谐波有效值		
		DataType4 d_40B7;  ///<线路1 C相 9次电压谐波有效值		
		DataType4 d_40B8;  ///<线路1 C相 10次电压谐波有效值	
		DataType4 d_40B9;  ///<线路1 C相 11次电压谐波有效值	
		DataType4 d_40BA;  ///<线路1 C相 12次电压谐波有效值	
		DataType4 d_40BB;  ///<线路1 C相 13次电压谐波有效值	
		DataType4 d_40BC;  ///<线路1 C相 14次电压谐波有效值	
		DataType4 d_40BD;  ///<线路1 C相 15次电压谐波有效值	
		DataType4 d_40BE;  ///<线路1 C相 16次电压谐波有效值	
		DataType4 d_40BF;  ///<线路1 C相 17次电压谐波有效值	
		DataType4 d_40C0;  ///<线路1 C相 18次电压谐波有效值	
		DataType4 d_40C1;  ///<线路1 C相 19次电压谐波有效值	
		DataType4 d_40C2;  ///<线路1 C相	2次电流谐波有效值		
		DataType4 d_40C3;  ///<线路1 C相 3次电流谐波有效值		
		DataType4 d_40C4;  ///<线路1 C相 4次电流谐波有效值		
		DataType4 d_40C5;  ///<线路1 C相 5次电流谐波有效值		
		DataType4 d_40C6;  ///<线路1 C相 6次电流谐波有效值		
		
		DataType4 d_40C7;  ///<线路1 C相 7次电流谐波有效值		
		DataType4 d_40C8;  ///<线路1 C相 8次电流谐波有效值		
		DataType4 d_40C9;  ///<线路1 C相 9次电流谐波有效值		
		DataType4 d_40CA;  ///<线路1 C相 10次电流谐波有效值	
		DataType4 d_40CB;  ///<线路1 C相 11次电流谐波有效值	
		DataType4 d_40CC;  ///<线路1 C相 12次电流谐波有效值	
		DataType4 d_40CD;  ///<线路1 C相 13次电流谐波有效值	
		DataType4 d_40CE;  ///<线路1 C相 14次电流谐波有效值	
		DataType4 d_40CF;  ///<线路1 C相 15次电流谐波有效值	
		DataType4 d_40D0;  ///<线路1 C相 16次电流谐波有效值	
		DataType4 d_40D1;  ///<线路1 C相 17次电流谐波有效值	
		DataType4 d_40D2;  ///<线路1 C相 18次电流谐波有效值	
		DataType4 d_40D3;  ///<线路1 C相 19次电流谐波有效值	
		DataType4 d_40D4;  ///<线路1 C相 电压谐波总含有率		
		DataType4 d_40D5;  ///<线路1 C相	2次电压谐波含有率		
		DataType4 d_40D6;  ///<线路1 C相 3次电压谐波含有率		
		DataType4 d_40D7;  ///<线路1 C相 4次电压谐波含有率		
		DataType4 d_40D8;  ///<线路1 C相 5次电压谐波含有率		
		DataType4 d_40D9;  ///<线路1 C相 6次电压谐波含有率		
		DataType4 d_40DA;  ///<线路1 C相 7次电压谐波含有率		
		DataType4 d_40DB;  ///<线路1 C相 8次电压谐波含有率		
		DataType4 d_40DC;  ///<线路1 C相 9次电压谐波含有率		
		DataType4 d_40DD;  ///<线路1 C相 10次电压谐波含有率	
		DataType4 d_40DE;  ///<线路1 C相 11次电压谐波含有率	
		DataType4 d_40DF;  ///<线路1 C相 12次电压谐波含有率	
		DataType4 d_40E0;  ///<线路1 C相 13次电压谐波含有率	
		DataType4 d_40E1;  ///<线路1 C相 14次电压谐波含有率	
		DataType4 d_40E2;  ///<线路1 C相 15次电压谐波含有率	
		DataType4 d_40E3;  ///<线路1 C相 16次电压谐波含有率	
		DataType4 d_40E4;  ///<线路1 C相 17次电压谐波含有率	
		DataType4 d_40E5;  ///<线路1 C相 18次电压谐波含有率	
		DataType4 d_40E6;  ///<线路1 C相 19次电压谐波含有率	
		DataType4 d_40E7;  ///<线路1 C相 电流谐波总含有率		
		DataType4 d_40E8;  ///<线路1 C相	2次电流谐波含有率		
		DataType4 d_40E9;  ///<线路1 C相 3次电流谐波含有率		
		DataType4 d_40EA;  ///<线路1 C相 4次电流谐波含有率		
		DataType4 d_40EB;  ///<线路1 C相 5次电流谐波含有率		
		DataType4 d_40EC;  ///<线路1 C相 6次电流谐波含有率		
		DataType4 d_40ED;  ///<线路1 C相 7次电流谐波含有率		
		DataType4 d_40EE;  ///<线路1 C相 8次电流谐波含有率		
		DataType4 d_40EF;  ///<线路1 C相 9次电流谐波含有率		
		DataType4 d_40F0;  ///<线路1 C相 10次电流谐波含有率	
		DataType4 d_40F1;  ///<线路1 C相 11次电流谐波含有率	
		DataType4 d_40F2;  ///<线路1 C相 12次电流谐波含有率	
		DataType4 d_40F3;  ///<线路1 C相 13次电流谐波含有率	
		DataType4 d_40F4;  ///<线路1 C相 14次电流谐波含有率	
		DataType4 d_40F5;  ///<线路1 C相 15次电流谐波含有率	
		DataType4 d_40F6;  ///<线路1 C相 16次电流谐波含有率	
		DataType4 d_40F7;  ///<线路1 C相 17次电流谐波含有率	
		DataType4 d_40F8;  ///<线路1 C相 18次电流谐波含有率	
		DataType4 d_40F9;  ///<线路1 C相 19次电流谐波含有率	
		DataType4 d_40FA;  ///<线路2 A相电压									
		DataType4 d_40FB;  ///<线路2 B相电压									
		DataType4 d_40FC;  ///<线路2 C相电压									
		DataType4 d_40FD;  ///<线路2 AB线电压								
		DataType4 d_40FE;  ///<线路2 BC线电压								
		DataType4 d_40FF;  ///<线路2 零序电压								
		DataType4 d_4100;  ///<线路2 A相测量电流							
		DataType4 d_4101;  ///<线路2 B相测量电流							
		DataType4 d_4102;  ///<线路2 C相测量电流							
		DataType4 d_4103;  ///<线路2 零序电流								
		DataType4 d_4104;  ///<线路2 A相保护电流							
		DataType4 d_4105;  ///<线路2 B相保护电流							
		DataType4 d_4106;  ///<线路2 C相保护电流							
		DataType4 d_4107;  ///<线路2 A相有功功率							
		DataType4 d_4108;  ///<线路2 A相无功功率							
		DataType4 d_4109;  ///<线路2 A相功率因数							
		DataType4 d_410A;  ///<线路2 B相有功功率							
		DataType4 d_410B;  ///<线路2 B相无功功率							
		DataType4 d_410C;  ///<线路2 B相功率因数							
		DataType4 d_410D;  ///<线路2 C相有功功率							
		DataType4 d_410E;  ///<线路2 C相无功功率							
		DataType4 d_410F;  ///<线路2 C相功率因数							
		DataType4 d_4110;  ///<线路2 总有功功率							
		DataType4 d_4111;  ///<线路2 总无功功率							
		DataType4 d_4112;  ///<线路2 总视在功率							
		DataType4 d_4113;  ///<线路2 总功率因数							
		DataType4 d_4114;  ///<线路2 频率										
		DataType4 d_4115;  ///<线路2 A相 2次电压谐波有效值		
		DataType4 d_4116;  ///<线路2 A相 3次电压谐波有效值		
		DataType4 d_4117;  ///<线路2 A相 4次电压谐波有效值		
		DataType4 d_4118;  ///<线路2 A相 5次电压谐波有效值		
		DataType4 d_4119;  ///<线路2 A相 6次电压谐波有效值		
		DataType4 d_411A;  ///<线路2 A相 7次电压谐波有效值		
		DataType4 d_411B;  ///<线路2 A相 8次电压谐波有效值		
		DataType4 d_411C;  ///<线路2 A相 9次电压谐波有效值		
		DataType4 d_411D;  ///<线路2 A相 10次电压谐波有效值	
		DataType4 d_411E;  ///<线路2 A相 11次电压谐波有效值	
		DataType4 d_411F;  ///<线路2 A相 12次电压谐波有效值	
		DataType4 d_4120;  ///<线路2 A相 13次电压谐波有效值	
		DataType4 d_4121;  ///<线路2 A相 14次电压谐波有效值	
		DataType4 d_4122;  ///<线路2 A相 15次电压谐波有效值	
		DataType4 d_4123;  ///<线路2 A相 16次电压谐波有效值	
		DataType4 d_4124;  ///<线路2 A相 17次电压谐波有效值	
		DataType4 d_4125;  ///<线路2 A相 18次电压谐波有效值	
		DataType4 d_4126;  ///<线路2 A相 19次电压谐波有效值	
		DataType4 d_4127;  ///<线路2 A相 2次电流谐波有效值		
		DataType4 d_4128;  ///<线路2 A相 3次电流谐波有效值		
		DataType4 d_4129;  ///<线路2 A相 4次电流谐波有效值		
		DataType4 d_412A;  ///<线路2 A相 5次电流谐波有效值		
		DataType4 d_412B;  ///<线路2 A相 6次电流谐波有效值		
		DataType4 d_412C;  ///<线路2 A相 7次电流谐波有效值		
		DataType4 d_412D;  ///<线路2 A相 8次电流谐波有效值		
		DataType4 d_412E;  ///<线路2 A相 9次电流谐波有效值		
		DataType4 d_412F;  ///<线路2 A相 10次电流谐波有效值	
		DataType4 d_4130;  ///<线路2 A相 11次电流谐波有效值	
		DataType4 d_4131;  ///<线路2 A相 12次电流谐波有效值	
		DataType4 d_4132;  ///<线路2 A相 13次电流谐波有效值	
		DataType4 d_4133;  ///<线路2 A相 14次电流谐波有效值	
		DataType4 d_4134;  ///<线路2 A相 15次电流谐波有效值	
		DataType4 d_4135;  ///<线路2 A相 16次电流谐波有效值	
		DataType4 d_4136;  ///<线路2 A相 17次电流谐波有效值	
		DataType4 d_4137;  ///<线路2 A相 18次电流谐波有效值	
		DataType4 d_4138;  ///<线路2 A相 19次电流谐波有效值	
		DataType4 d_4139;  ///<线路2 A相 电压谐波总含有率		
		DataType4 d_413A;  ///<线路2 A相 2次电压谐波含有率		
		DataType4 d_413B;  ///<线路2 A相 3次电压谐波含有率		
		DataType4 d_413C;  ///<线路2 A相 4次电压谐波含有率		
		DataType4 d_413D;  ///<线路2 A相 5次电压谐波含有率		
		DataType4 d_413E;  ///<线路2 A相 6次电压谐波含有率		
		DataType4 d_413F;  ///<线路2 A相 7次电压谐波含有率		
		DataType4 d_4140;  ///<线路2 A相 8次电压谐波含有率		
		DataType4 d_4141;  ///<线路2 A相 9次电压谐波含有率		
		DataType4 d_4142;  ///<线路2 A相 10次电压谐波含有率	
		DataType4 d_4143;  ///<线路2 A相 11次电压谐波含有率	
		DataType4 d_4144;  ///<线路2 A相 12次电压谐波含有率	
		DataType4 d_4145;  ///<线路2 A相 13次电压谐波含有率	
		DataType4 d_4146;  ///<线路2 A相 14次电压谐波含有率	
		DataType4 d_4147;  ///<线路2 A相 15次电压谐波含有率	
		DataType4 d_4148;  ///<线路2 A相 16次电压谐波含有率	
		DataType4 d_4149;  ///<线路2 A相 17次电压谐波含有率	
		DataType4 d_414A;  ///<线路2 A相 18次电压谐波含有率	
		DataType4 d_414B;  ///<线路2 A相 19次电压谐波含有率	
		DataType4 d_414C;  ///<线路2 A相 电流谐波总含有率		
		DataType4 d_414D;  ///<线路2 A相 2次电流谐波含有率		
		DataType4 d_414E;  ///<线路2 A相 3次电流谐波含有率		
		DataType4 d_414F;  ///<线路2 A相 4次电流谐波含有率		
		DataType4 d_4150;  ///<线路2 A相 5次电流谐波含有率		
		DataType4 d_4151;  ///<线路2 A相 6次电流谐波含有率		
		DataType4 d_4152;  ///<线路2 A相 7次电流谐波含有率		
		DataType4 d_4153;  ///<线路2 A相 8次电流谐波含有率		
		DataType4 d_4154;  ///<线路2 A相 9次电流谐波含有率		
		DataType4 d_4155;  ///<线路2 A相 10次电流谐波含有率	
		DataType4 d_4156;  ///<线路2 A相 11次电流谐波含有率	
		DataType4 d_4157;  ///<线路2 A相 12次电流谐波含有率	
		DataType4 d_4158;  ///<线路2 A相 13次电流谐波含有率	
		DataType4 d_4159;  ///<线路2 A相 14次电流谐波含有率	
		DataType4 d_415A;  ///<线路2 A相 15次电流谐波含有率	
		DataType4 d_415B;  ///<线路2 A相 16次电流谐波含有率	
		DataType4 d_415C;  ///<线路2 A相 17次电流谐波含有率	
		DataType4 d_415D;  ///<线路2 A相 18次电流谐波含有率	
		DataType4 d_415E;  ///<线路2 A相 19次电流谐波含有率	
		DataType4 d_415F;  ///<线路2 B相 2次电压谐波有效值		
		DataType4 d_4160;  ///<线路2 B相 3次电压谐波有效值		
		DataType4 d_4161;  ///<线路2 B相 4次电压谐波有效值		
		DataType4 d_4162;  ///<线路2 B相 5次电压谐波有效值		
		DataType4 d_4163;  ///<线路2 B相 6次电压谐波有效值		
		DataType4 d_4164;  ///<线路2 B相 7次电压谐波有效值		
		DataType4 d_4165;  ///<线路2 B相 8次电压谐波有效值		
		DataType4 d_4166;  ///<线路2 B相 9次电压谐波有效值		
		DataType4 d_4167;  ///<线路2 B相 10次电压谐波有效值	
		DataType4 d_4168;  ///<线路2 B相 11次电压谐波有效值	
		DataType4 d_4169;  ///<线路2 B相 12次电压谐波有效值	
		DataType4 d_416A;  ///<线路2 B相 13次电压谐波有效值	
		DataType4 d_416B;  ///<线路2 B相 14次电压谐波有效值	
		DataType4 d_416C;  ///<线路2 B相 15次电压谐波有效值	
		DataType4 d_416D;  ///<线路2 B相 16次电压谐波有效值	
		DataType4 d_416E;  ///<线路2 B相 17次电压谐波有效值	
		DataType4 d_416F;  ///<线路2 B相 18次电压谐波有效值	
		DataType4 d_4170;  ///<线路2 B相 19次电压谐波有效值	
		DataType4 d_4171;  ///<线路2 B相 2次电流谐波有效值		
		DataType4 d_4172;  ///<线路2 B相 3次电流谐波有效值		
		DataType4 d_4173;  ///<线路2 B相 4次电流谐波有效值		
		DataType4 d_4174;  ///<线路2 B相 5次电流谐波有效值		
		DataType4 d_4175;  ///<线路2 B相 6次电流谐波有效值		
		DataType4 d_4176;  ///<线路2 B相 7次电流谐波有效值		
		DataType4 d_4177;  ///<线路2 B相 8次电流谐波有效值		
		DataType4 d_4178;  ///<线路2 B相 9次电流谐波有效值		
		DataType4 d_4179;  ///<线路2 B相 10次电流谐波有效值	
		DataType4 d_417A;  ///<线路2 B相 11次电流谐波有效值	
		DataType4 d_417B;  ///<线路2 B相 12次电流谐波有效值	
		DataType4 d_417C;  ///<线路2 B相 13次电流谐波有效值	
		DataType4 d_417D;  ///<线路2 B相 14次电流谐波有效值	
		DataType4 d_417E;  ///<线路2 B相 15次电流谐波有效值	
		DataType4 d_417F;  ///<线路2 B相 16次电流谐波有效值	
		DataType4 d_4180;  ///<线路2 B相 17次电流谐波有效值	
		DataType4 d_4181;  ///<线路2 B相 18次电流谐波有效值	
		DataType4 d_4182;  ///<线路2 B相 19次电流谐波有效值	
		DataType4 d_4183;  ///<线路2 B相 电压谐波总含有率		
		DataType4 d_4184;  ///<线路2 B相 2次电压谐波含有率		
		DataType4 d_4185;  ///<线路2 B相 3次电压谐波含有率		
		DataType4 d_4186;  ///<线路2 B相 4次电压谐波含有率		
		DataType4 d_4187;  ///<线路2 B相 5次电压谐波含有率		
		DataType4 d_4188;  ///<线路2 B相 6次电压谐波含有率		
		DataType4 d_4189;  ///<线路2 B相 7次电压谐波含有率		
		DataType4 d_418A;  ///<线路2 B相 8次电压谐波含有率		
		DataType4 d_418B;  ///<线路2 B相 9次电压谐波含有率		
		DataType4 d_418C;  ///<线路2 B相 10次电压谐波含有率	
		DataType4 d_418D;  ///<线路2 B相 11次电压谐波含有率	
		DataType4 d_418E;  ///<线路2 B相 12次电压谐波含有率	
		DataType4 d_418F;  ///<线路2 B相 13次电压谐波含有率	
		DataType4 d_4190;  ///<线路2 B相 14次电压谐波含有率	
		DataType4 d_4191;  ///<线路2 B相 15次电压谐波含有率	
		DataType4 d_4192;  ///<线路2 B相 16次电压谐波含有率	
		DataType4 d_4193;  ///<线路2 B相 17次电压谐波含有率	
		DataType4 d_4194;  ///<线路2 B相 18次电压谐波含有率	
		DataType4 d_4195;  ///<线路2 B相 19次电压谐波含有率	
		DataType4 d_4196;  ///<线路2 B相 电流谐波总含有率		
		DataType4 d_4197;  ///<线路2 B相 2次电流谐波含有率		
		DataType4 d_4198;  ///<线路2 B相 3次电流谐波含有率		
		DataType4 d_4199;  ///<线路2 B相 4次电流谐波含有率		
		DataType4 d_419A;  ///<线路2 B相 5次电流谐波含有率		
		DataType4 d_419B;  ///<线路2 B相 6次电流谐波含有率		
		DataType4 d_419C;  ///<线路2 B相 7次电流谐波含有率		
		DataType4 d_419D;  ///<线路2 B相 8次电流谐波含有率		
		DataType4 d_419E;  ///<线路2 B相 9次电流谐波含有率		
		DataType4 d_419F;  ///<线路2 B相 10次电流谐波含有率	
		DataType4 d_41A0;  ///<线路2 B相 11次电流谐波含有率	
		DataType4 d_41A1;  ///<线路2 B相 12次电流谐波含有率	
		DataType4 d_41A2;  ///<线路2 B相 13次电流谐波含有率	
		DataType4 d_41A3;  ///<线路2 B相 14次电流谐波含有率	
		DataType4 d_41A4;  ///<线路2 B相 15次电流谐波含有率	
		DataType4 d_41A5;  ///<线路2 B相 16次电流谐波含有率	
		DataType4 d_41A6;  ///<线路2 B相 17次电流谐波含有率	
		DataType4 d_41A7;  ///<线路2 B相 18次电流谐波含有率	
		DataType4 d_41A8; ///< 线路2 B相 19次电流谐波含有率	
		DataType4 d_41A9; ///< 线路2 C相	2次电压谐波有效值		
		DataType4 d_41AA; ///< 线路2 C相 3次电压谐波有效值		
		DataType4 d_41AB; ///< 线路2 C相 4次电压谐波有效值		
		DataType4 d_41AC; ///< 线路2 C相 5次电压谐波有效值		
		DataType4 d_41AD; ///< 线路2 C相 6次电压谐波有效值		
		DataType4 d_41AE; ///< 线路2 C相 7次电压谐波有效值		
		DataType4 d_41AF; ///< 线路2 C相 8次电压谐波有效值		
		DataType4 d_41B0; ///< 线路2 C相 9次电压谐波有效值		
		DataType4 d_41B1; ///< 线路2 C相 10次电压谐波有效值	
		DataType4 d_41B2; ///< 线路2 C相 11次电压谐波有效值	
		DataType4 d_41B3; ///< 线路2 C相 12次电压谐波有效值	
		DataType4 d_41B4; ///< 线路2 C相 13次电压谐波有效值	
		DataType4 d_41B5; ///< 线路2 C相 14次电压谐波有效值	
		DataType4 d_41B6; ///< 线路2 C相 15次电压谐波有效值	
		DataType4 d_41B7; ///< 线路2 C相 16次电压谐波有效值	
		DataType4 d_41B8; ///< 线路2 C相 17次电压谐波有效值	
		DataType4 d_41B9; ///< 线路2 C相 18次电压谐波有效值	
		DataType4 d_41BA; ///< 线路2 C相 19次电压谐波有效值	
		DataType4 d_41BB; ///< 线路2 C相	2次电流谐波有效值		
		DataType4 d_41BC; ///< 线路2 C相 3次电流谐波有效值		
		DataType4 d_41BD; ///< 线路2 C相 4次电流谐波有效值		
		DataType4 d_41BE; ///< 线路2 C相 5次电流谐波有效值		
		DataType4 d_41BF; ///< 线路2 C相 6次电流谐波有效值		
		DataType4 d_41C0; ///< 线路2 C相 7次电流谐波有效值		
		DataType4 d_41C1; ///< 线路2 C相 8次电流谐波有效值		
		DataType4 d_41C2; ///< 线路2 C相 9次电流谐波有效值		
		DataType4 d_41C3; ///< 线路2 C相 10次电流谐波有效值	
		DataType4 d_41C4; ///< 线路2 C相 11次电流谐波有效值	
		DataType4 d_41C5; ///< 线路2 C相 12次电流谐波有效值	
		DataType4 d_41C6; ///< 线路2 C相 13次电流谐波有效值	
		DataType4 d_41C7; ///< 线路2 C相 14次电流谐波有效值	
		DataType4 d_41C8; ///< 线路2 C相 15次电流谐波有效值	
		DataType4 d_41C9; ///< 线路2 C相 16次电流谐波有效值	
		DataType4 d_41CA; ///< 线路2 C相 17次电流谐波有效值	
		DataType4 d_41CB; ///< 线路2 C相 18次电流谐波有效值	
		DataType4 d_41CC; ///< 线路2 C相 19次电流谐波有效值	
		DataType4 d_41CD; ///< 线路2 C相 电压谐波总含有率		
		DataType4 d_41CE; ///< 线路2 C相	2次电压谐波含有率		
		DataType4 d_41CF; ///< 线路2 C相 3次电压谐波含有率		
		DataType4 d_41D0; ///< 线路2 C相 4次电压谐波含有率		
		DataType4 d_41D1; ///< 线路2 C相 5次电压谐波含有率		
		DataType4 d_41D2; ///< 线路2 C相 6次电压谐波含有率		
		DataType4 d_41D3; ///< 线路2 C相 7次电压谐波含有率		
		DataType4 d_41D4; ///< 线路2 C相 8次电压谐波含有率		
		DataType4 d_41D5; ///< 线路2 C相 9次电压谐波含有率		
		DataType4 d_41D6; ///< 线路2 C相 10次电压谐波含有率	
		DataType4 d_41D7; ///< 线路2 C相 11次电压谐波含有率	
		DataType4 d_41D8; ///< 线路2 C相 12次电压谐波含有率	
		DataType4 d_41D9; ///< 线路2 C相 13次电压谐波含有率	
		DataType4 d_41DA; ///< 线路2 C相 14次电压谐波含有率	
		DataType4 d_41DB; ///< 线路2 C相 15次电压谐波含有率	
		DataType4 d_41DC; ///< 线路2 C相 16次电压谐波含有率	
		DataType4 d_41DD; ///< 线路2 C相 17次电压谐波含有率	
		DataType4 d_41DE; ///< 线路2 C相 18次电压谐波含有率	
		DataType4 d_41DF; ///< 线路2 C相 19次电压谐波含有率	
		DataType4 d_41E0; ///< 线路2 C相 电流谐波总含有率		
		DataType4 d_41E1; ///< 线路2 C相	2次电流谐波含有率		
		DataType4 d_41E2; ///< 线路2 C相 3次电流谐波含有率		
		DataType4 d_41E3; ///< 线路2 C相 4次电流谐波含有率		
		DataType4 d_41E4; ///< 线路2 C相 5次电流谐波含有率		
		DataType4 d_41E5; ///< 线路2 C相 6次电流谐波含有率		
		DataType4 d_41E6; ///< 线路2 C相 7次电流谐波含有率		
		DataType4 d_41E7; ///< 线路2 C相 8次电流谐波含有率		
		DataType4 d_41E8; ///< 线路2 C相 9次电流谐波含有率		
		DataType4 d_41E9; ///< 线路2 C相 10次电流谐波含有率	
		DataType4 d_41EA; ///< 线路2 C相 11次电流谐波含有率	
		DataType4 d_41EB; ///< 线路2 C相 12次电流谐波含有率	
		DataType4 d_41EC; ///< 线路2 C相 13次电流谐波含有率	
		DataType4 d_41ED; ///< 线路2 C相 14次电流谐波含有率	
		DataType4 d_41EE; ///< 线路2 C相 15次电流谐波含有率	
		DataType4 d_41EF; ///< 线路2 C相 16次电流谐波含有率	
		DataType4 d_41F0; ///< 线路2 C相 17次电流谐波含有率	
		DataType4 d_41F1; ///< 线路2 C相 18次电流谐波含有率	
		DataType4 d_41F2; ///< 线路2 C相 19次电流谐波含有率	


    DataType4 d_4401;///<两线路相角差
    DataType4 d_4402;///<两线路电压差

    DataType4 d_4601;///<GPS经度
    DataType4 d_4602;///<GPS纬度
    DataType4 d_4603;///<直流量1
    DataType4 d_4604;///<直流量2
    DataType4 d_4605;///<蓄电池电压
    DataType4 d_4606;///<温度
    DataType4 d_4607;///<湿度

//遥控类数据
    DataType1 d_6001;///<断路器1（开/合）
    DataType1 d_6002;///<断路器2（开/合）
    DataType1 d_6064;///<电池活化（激活/退出）
    DataType1 d_6065;///<电池退出
    DataType1 d_6066;///<电动机操作机构电源投入/退出

//参数类数据
    /*第1套保护定值*/
    ProtectSet d_5001;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_5002;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5003;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_5004;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5005;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_5006;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5007;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_5008;///< 电流方向保护投入定值
    ProtectDirSet  d_5009;///< 零序电流方向保护投入定值
    /*第2套保护定值*/
    ProtectSet d_500A;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_500B;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_500C;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_500D;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_500E;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_500F;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5010;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_5011;///< 电流方向保护投入定值
    ProtectDirSet  d_5012;///< 零序电流方向保护投入定值
    /*第3套保护定值*/
    ProtectSet d_5013;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_5014;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5015;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_5016;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5017;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_5018;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5019;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_501A;///< 电流方向保护投入定值
    ProtectDirSet  d_501B;///< 零序电流方向保护投入定值
    /*第4套保护定值*/
    ProtectSet d_501C;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_501D;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_501E;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_501F;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5020;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_5021;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5022;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_5023;///< 电流方向保护投入定值
    ProtectDirSet  d_5024;///< 零序电流方向保护投入定值
    /*第5套保护定值*/
    ProtectSet d_5025;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_5026;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5027;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_5028;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5029;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_502A;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_502B;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_502C;///< 电流方向保护投入定值
    ProtectDirSet  d_502D;///< 零序电流方向保护投入定值
    /*第6套保护定值*/
    ProtectSet d_502E;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_502F;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5030;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_5031;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5032;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_5033;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5034;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_5035;///< 电流方向保护投入定值
    ProtectDirSet  d_5036;///< 零序电流方向保护投入定值
    /*第7套保护定值*/
    ProtectSet d_5037;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_5038;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5039;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_503A;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_503B;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_503C;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_503D;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_503E;///< 电流方向保护投入定值
    ProtectDirSet  d_503F;///< 零序电流方向保护投入定值
    /*第8套保护定值*/
    ProtectSet d_5040;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
    ProtectSet d_5041;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
    ProtectSet d_5042;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
    ProtectSet d_5043;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
    ProtectSet d_5044;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
    ProtectSet d_5045;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
    ProtectSet d_5046;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
    ProtectDirSet  d_5047;///< 电流方向保护投入定值
    ProtectDirSet  d_5048;///< 零序电流方向保护投入定值

    DataType1  d_5049;///<线路1保护定值区号设置
    DataType1  d_504A;///<线路2保护定值区号设置

    RecloseSet d_504B;///<重合闸设置
    /*第一套开关参数*/
    FaFunSet      d_504C;///<FA功能设置参数
    FaIsolationSet  d_504D;///<FA功能中分段开关参数
    FaConnectionSet d_504E;///<FA功能中联络开关参数
    /*第二套开关参数*/
    FaFunSet      d_504F;///<FA功能设置参数
    FaIsolationSet  d_5050;///<FA功能中分段开关参数
    FaConnectionSet d_5051;///<FA功能中联络开关参数

    DataType1    d_5052;///<线路1开关定值区号设置
    DataType1    d_5053;///<线路2开关定值区号设置
    FaCommSet    d_5054;///<FA功能公用参数设置
    FaDevotionSet d_5055;///<保护功能投退设置

    DataType1  d_5080;///<电压电流有效值计算方法
    UserPortSet   d_5081;///<模拟量输入配置
    UserPortSet   d_5082;///<模拟量输入配置
    UserPortSet   d_5083;///<模拟量输入配置
    UserPortSet   d_5084;///<模拟量输入配置
    UserPortSet  d_5085;///<单点遥信开关量输入配置
    UserPortSet  d_5086;///<单点遥信开关量输入配置
    UserPortSet  d_5087;///<单点遥信开关量输入配置
    UserPortSet  d_5088;///<单点遥信开关量输入配置
    DbDigitInSet  d_5089;///<双点遥信开关量输入配置
    UserPortSet d_508A;///<开关量输出配置
    PowerWay  d_508B;///<功率计算方法配置
    DataType1  d_508C;///<直流输入量配置
    DataType1  d_508D;///<电流标称值
    DataType1  d_508E;///<电压标称值
    DeadAreaSet d_508F;///<遥测死区值设置
    DataType3  d_5090;///<遥信防抖时间
    DataType3  d_5091;///<遥控返校失败取消遥控时间
    DataType3  d_5092;///<遥控保持时间
    DataType3  d_5093;///<遥控脉冲宽度
    DataType3  d_5094;///<延时断开操作电源时间
    DataType2  d_5095;///<PT
    DataType2  d_5096;///<CT
    DataType2  d_5097;///<标定
    YCMsgSet   d_5098;///<遥测信息体属性配置
    YCMsgSet   d_5099;///<遥测信息体属性配置
    YCMsgSet   d_509A;///<遥测信息体属性配置
    YCMsgSet   d_509B;///<遥测信息体属性配置
    YCMsgSet   d_509C;///<遥测信息体属性配置
    YCMsgSet   d_509D;///<遥测信息体属性配置
    YCMsgSet   d_509E;///<遥测信息体属性配置
    YCMsgSet   d_509F;///<遥测信息体属性配置
    YCMsgSet   d_50A0;///<遥测信息体属性配置
    YCMsgSet   d_50A1;///<遥测信息体属性配置
    YCMsgSet   d_50A2;///<遥测信息体属性配置
    YCMsgSet   d_50A3;///<遥测信息体属性配置
    YCMsgSet   d_50A4;///<遥测信息体属性配置
    YCMsgSet   d_50A5;///<遥测信息体属性配置
    YCMsgSet   d_50A6;///<遥测信息体属性配置
    YCMsgSet   d_50A7;///<遥测信息体属性配置
    YCMsgSet   d_50A8;///<遥测信息体属性配置
    YCMsgSet   d_50A9;///<遥测信息体属性配置
    YCMsgSet   d_50AA;///<遥测信息体属性配置
    YCMsgSet   d_50AB;///<遥测信息体属性配置
    YCMsgSet   d_50AC;///<遥测信息体属性配置

    YXMsgSet   d_50AD;///<遥信信息体属性配置
    YXMsgSet   d_50AE;///<遥信信息体属性配置
    YXMsgSet   d_50AF;///<遥信信息体属性配置
    YXMsgSet   d_50B0;///<遥信信息体属性配置
    YXMsgSet   d_50B1;///<遥信信息体属性配置
    YXMsgSet   d_50B2;///<遥信信息体属性配置
    YXMsgSet   d_50B3;///<遥信信息体属性配置
    YXMsgSet   d_50B4;///<遥信信息体属性配置
    YXMsgSet   d_50B5;///<遥信信息体属性配置
    YXMsgSet   d_50B6;///<遥信信息体属性配置
    YXMsgSet   d_50B7;///<遥信信息体属性配置

    YKMsgSet   d_50B8;///<遥控信息体属性配置



    Pro101Para  d_5100;///<101规约参数
    Pro104Para  d_5101;///<104规约参数
    EthPara    d_5102;///<以太网参数
    WirelessPara d_5103;///<无线通讯参数
    UartPara    d_5104;///<串口1参数
    UartPara    d_5105;///<串口2参数
    ComBasePara d_5106;///<通讯基本参数


    PassWdPara  d_5200;///<密码参数
    PassWdPara  d_5201;///<版本号
    DataType4   d_5202;///<温度定值
    DataType4   d_5203;///<湿度定值
    DataType4   d_5204;///<经度定值
    DataType4   d_5205;///<纬度定值
    DataType1   d_5206;///<恢复出厂设置
    DataType4   d_5207;///<蓄电池欠压切除阈值
    DataType4   d_5208;///<蓄电池低压告警阈值
    DataType2   d_5209;///<蓄电池活化充放电间隔时间
    WarnPara    d_520A;///<电压上限告警阈值，恢复阈值，持续时间
    WarnPara    d_520B;///<电压下限告警阈值，恢复阈值，持续时间
    WarnPara    d_520C;///<电流上限阈值，恢复阈值，持续时间
    WarnPara    d_520D;///<零序电流越上限阈值，恢复阈值，持续时间
    HisDataDefinePara d_520E;///<遥测历史数据项定义
    DataType2  d_520F;///<遥测历史数据存储间隔
    HisDataDefinePara d_5210;///<遥测极值数据项定义
    DataType1   d_5211;///<人工置数使能
    UserRealDataDefPara d_5212;///<人工置数参数

}DATA_BUF;


UserData user_data;
DATA_BUF data_buf;


#define MK_DSTB(ID) 0x##ID, sizeof(data_buf.d_##ID.Data),&data_buf.d_##ID,0x##ID,1,1,1

static Data<DataType1> YxData[] = {
    {MK_DSTB ( 0001 ) },
    {MK_DSTB ( 0002 ) },
    {MK_DSTB ( 0003 ) },
    {MK_DSTB ( 0004 ) },
    {MK_DSTB ( 0005 ) },
    {MK_DSTB ( 0006 ) },
    {MK_DSTB ( 0007 ) },
    {MK_DSTB ( 0008 ) },
    {MK_DSTB ( 0009 ) },
    {MK_DSTB ( 000A ) },
    {MK_DSTB ( 000B ) },
    {MK_DSTB ( 000C ) },
    {MK_DSTB ( 000D ) },
    {MK_DSTB ( 000E ) },
    {MK_DSTB ( 000F ) },
    {MK_DSTB ( 0010 ) },
    {MK_DSTB ( 0011 ) },
    {MK_DSTB ( 0012 ) },
    {MK_DSTB ( 0013 ) },
    {MK_DSTB ( 0014 ) },
    {MK_DSTB ( 0015 ) },
    {MK_DSTB ( 0016 ) },
    {MK_DSTB ( 0017 ) },
    {MK_DSTB ( 0018 ) },
    {MK_DSTB ( 0019 ) },
    {MK_DSTB ( 001A ) },
    {MK_DSTB ( 001B ) },
    {MK_DSTB ( 001C ) },
    {MK_DSTB ( 001D ) },
    {MK_DSTB ( 001E ) },
    {MK_DSTB ( 001F ) },
    {MK_DSTB ( 0020 ) },
    {MK_DSTB ( 0021 ) },
    {MK_DSTB ( 0022 ) },
    {MK_DSTB ( 0023 ) },
    {MK_DSTB ( 0024 ) },
    {MK_DSTB ( 0025 ) },
    {MK_DSTB ( 0026 ) },
    {MK_DSTB ( 0027 ) },
    {MK_DSTB ( 0028 ) },
    {MK_DSTB ( 0029 ) },
    {MK_DSTB ( 002A ) },
    {MK_DSTB ( 002B ) },
    {MK_DSTB ( 002C ) },
    {MK_DSTB ( 002D ) },
    {MK_DSTB ( 002E ) },
    {MK_DSTB ( 002F ) },
    {MK_DSTB ( 0030 ) },
    {MK_DSTB ( 0031 ) },
    {MK_DSTB ( 0032 ) },
    {MK_DSTB ( 0033 ) },
    {MK_DSTB ( 0034 ) },
    {MK_DSTB ( 0035 ) },
    {MK_DSTB ( 0036 ) },
    {MK_DSTB ( 0037 ) },
    {MK_DSTB ( 0038 ) },
    {MK_DSTB ( 0039 ) },
    {MK_DSTB ( 003A ) },
    {MK_DSTB ( 003B ) },
    {MK_DSTB ( 003C ) },

    {MK_DSTB ( 0201 ) },
    {MK_DSTB ( 0202 ) },
    {MK_DSTB ( 0203 ) },
    {MK_DSTB ( 0204 ) },
    {MK_DSTB ( 0205 ) },
    {MK_DSTB ( 0206 ) },
    {MK_DSTB ( 0207 ) },
    {MK_DSTB ( 0208 ) },
    {MK_DSTB ( 0209 ) },
    {MK_DSTB ( 020A ) },
    {MK_DSTB ( 020B ) },
    {MK_DSTB ( 020C ) },
    {MK_DSTB ( 020D ) },
    {MK_DSTB ( 020E ) },
    {MK_DSTB ( 020F ) },
    {MK_DSTB ( 0210 ) },
    {MK_DSTB ( 0211 ) },
    {MK_DSTB ( 0212 ) },
    {MK_DSTB ( 0213 ) },
    {MK_DSTB ( 0214 ) },
    {MK_DSTB ( 0215 ) },
    {MK_DSTB ( 0216 ) },
    {MK_DSTB ( 0217 ) },
    {MK_DSTB ( 0218 ) },
    {MK_DSTB ( 0219 ) },
    {MK_DSTB ( 021A ) },
};
static Data<DataType4> YcData[] = {
    {MK_DSTB ( 4001 ) },
    {MK_DSTB ( 4002 ) },
    {MK_DSTB ( 4003 ) },
    {MK_DSTB ( 4004 ) },
    {MK_DSTB ( 4005 ) },
    {MK_DSTB ( 4006 ) },
    {MK_DSTB ( 4007 ) },
    {MK_DSTB ( 4008 ) },
    {MK_DSTB ( 4009 ) },
    {MK_DSTB ( 400A ) },
    {MK_DSTB ( 400B ) },
    {MK_DSTB ( 400C ) },
    {MK_DSTB ( 400D ) },
    {MK_DSTB ( 400E ) },
    {MK_DSTB ( 400F ) },
    {MK_DSTB ( 4010 ) },
    {MK_DSTB ( 4011 ) },
    {MK_DSTB ( 4012 ) },
    {MK_DSTB ( 4013 ) },
    {MK_DSTB ( 4014 ) },
    {MK_DSTB ( 4015 ) },
    {MK_DSTB ( 4016 ) },
    {MK_DSTB ( 4017 ) },
    {MK_DSTB ( 4018 ) },
    {MK_DSTB ( 4019 ) },
    {MK_DSTB ( 401A ) },
    {MK_DSTB ( 401B ) },
		{MK_DSTB ( 401C	 ) },	
		{MK_DSTB ( 401D  ) }, 
		{MK_DSTB ( 401E  ) }, 
		{MK_DSTB ( 401F  ) }, 
		{MK_DSTB ( 4020  ) }, 
		{MK_DSTB ( 4021  ) }, 
		{MK_DSTB ( 4022  ) }, 
		{MK_DSTB ( 4023  ) }, 
		{MK_DSTB ( 4024  ) }, 
		{MK_DSTB ( 4025  ) }, 
		{MK_DSTB ( 4026  ) }, 
		{MK_DSTB ( 4027  ) }, 
		{MK_DSTB ( 4028  ) }, 
		{MK_DSTB ( 4029  ) }, 
		{MK_DSTB ( 402A  ) }, 
		{MK_DSTB ( 402B  ) }, 
		{MK_DSTB ( 402C  ) }, 
		{MK_DSTB ( 402D  ) }, 
		{MK_DSTB ( 402E  ) }, 
		{MK_DSTB ( 402F  ) }, 
		{MK_DSTB ( 4030  ) }, 
		{MK_DSTB ( 4031  ) }, 
		{MK_DSTB ( 4032  ) }, 
		{MK_DSTB ( 4033  ) }, 
		{MK_DSTB ( 4034  ) }, 
		{MK_DSTB ( 4035  ) }, 
		{MK_DSTB ( 4036  ) }, 
		{MK_DSTB ( 4037  ) }, 
		{MK_DSTB ( 4038  ) }, 
		{MK_DSTB ( 4039  ) }, 
		{MK_DSTB ( 403A  ) }, 
		{MK_DSTB ( 403B  ) }, 
		{MK_DSTB ( 403C  ) }, 
		{MK_DSTB ( 403D  ) }, 
		{MK_DSTB ( 403E  ) }, 
		{MK_DSTB ( 403F  ) }, 
		{MK_DSTB ( 4040  ) }, 
		{MK_DSTB ( 4041  ) }, 
		{MK_DSTB ( 4042  ) }, 
		{MK_DSTB ( 4043  ) }, 
		{MK_DSTB ( 4044  ) }, 
		{MK_DSTB ( 4045  ) }, 
		{MK_DSTB ( 4046  ) }, 
		{MK_DSTB ( 4047  ) }, 
		{MK_DSTB ( 4048  ) }, 
		{MK_DSTB ( 4049  ) }, 
		{MK_DSTB ( 404A  ) }, 
		{MK_DSTB ( 404B  ) }, 
		{MK_DSTB ( 404C  ) }, 
		{MK_DSTB ( 404D  ) }, 
		{MK_DSTB ( 404E  ) }, 
		{MK_DSTB ( 404F  ) }, 
		{MK_DSTB ( 4050  ) }, 
		{MK_DSTB ( 4051  ) }, 
		{MK_DSTB ( 4052  ) }, 
		{MK_DSTB ( 4053  ) }, 
		{MK_DSTB ( 4054  ) }, 
		{MK_DSTB ( 4055  ) }, 
		{MK_DSTB ( 4056  ) }, 
		{MK_DSTB ( 4057  ) }, 
		{MK_DSTB ( 4058  ) }, 
		{MK_DSTB ( 4059  ) }, 
		{MK_DSTB ( 405A  ) }, 
		{MK_DSTB ( 405B  ) }, 
		{MK_DSTB ( 405C  ) }, 
		{MK_DSTB ( 405D  ) }, 
		{MK_DSTB ( 405E  ) }, 
		{MK_DSTB ( 405F  ) }, 
		{MK_DSTB ( 4060  ) }, 
		{MK_DSTB ( 4061  ) }, 
		{MK_DSTB ( 4062  ) }, 
		{MK_DSTB ( 4063  ) }, 
		{MK_DSTB ( 4064  ) }, 
		{MK_DSTB ( 4065  ) }, 
		{MK_DSTB ( 4066  ) }, 
		{MK_DSTB ( 4067  ) }, 
		{MK_DSTB ( 4068  ) }, 
		{MK_DSTB ( 4069  ) }, 
		{MK_DSTB ( 406A  ) }, 
		{MK_DSTB ( 406B  ) }, 
		{MK_DSTB ( 406C  ) }, 
		{MK_DSTB ( 406D  ) }, 
		{MK_DSTB ( 406E  ) }, 
		{MK_DSTB ( 406F  ) }, 
		{MK_DSTB ( 4070  ) }, 
		{MK_DSTB ( 4071  ) }, 
		{MK_DSTB ( 4072  ) }, 
		{MK_DSTB ( 4073  ) }, 
		{MK_DSTB ( 4074  ) }, 
		{MK_DSTB ( 4075  ) }, 
		{MK_DSTB ( 4076  ) }, 
		{MK_DSTB ( 4077  ) }, 
		{MK_DSTB ( 4078  ) }, 
		{MK_DSTB ( 4079  ) }, 
		{MK_DSTB ( 407A  ) }, 
		{MK_DSTB ( 407B  ) }, 
		{MK_DSTB ( 407C  ) }, 
		{MK_DSTB ( 407D  ) }, 
		{MK_DSTB ( 407E  ) }, 
		{MK_DSTB ( 407F  ) }, 
		{MK_DSTB ( 4080  ) }, 
		{MK_DSTB ( 4081  ) }, 
		{MK_DSTB ( 4082  ) }, 
		{MK_DSTB ( 4083  ) }, 
		{MK_DSTB ( 4084  ) }, 
		{MK_DSTB ( 4085  ) }, 
		{MK_DSTB ( 4086  ) }, 
		{MK_DSTB ( 4087  ) }, 
		{MK_DSTB ( 4088  ) }, 
		{MK_DSTB ( 4089  ) }, 
		{MK_DSTB ( 408A  ) }, 
		{MK_DSTB ( 408B  ) }, 
		{MK_DSTB ( 408C  ) }, 
		{MK_DSTB ( 408D  ) }, 
		{MK_DSTB ( 408E  ) }, 
		{MK_DSTB ( 408F  ) }, 
		{MK_DSTB ( 4090  ) }, 
		{MK_DSTB ( 4091  ) }, 
		{MK_DSTB ( 4092  ) }, 
		{MK_DSTB ( 4093  ) }, 
		{MK_DSTB ( 4094  ) }, 
		{MK_DSTB ( 4095  ) }, 
		{MK_DSTB ( 4096  ) }, 
		{MK_DSTB ( 4097  ) }, 
		{MK_DSTB ( 4098  ) }, 
		{MK_DSTB ( 4099  ) }, 
		{MK_DSTB ( 409A  ) }, 
		{MK_DSTB ( 409B  ) }, 
		{MK_DSTB ( 409C  ) }, 
		{MK_DSTB ( 409D  ) }, 
		{MK_DSTB ( 409E  ) }, 
		{MK_DSTB ( 409F  ) }, 
		{MK_DSTB ( 40A0  ) }, 
		{MK_DSTB ( 40A1  ) }, 
		{MK_DSTB ( 40A2  ) }, 
		{MK_DSTB ( 40A3  ) }, 
		{MK_DSTB ( 40A4  ) }, 
		{MK_DSTB ( 40A5  ) }, 
		{MK_DSTB ( 40A6  ) }, 
		{MK_DSTB ( 40A7  ) }, 
		{MK_DSTB ( 40A8  ) }, 
		{MK_DSTB ( 40A9  ) }, 
		{MK_DSTB ( 40AA  ) }, 
		{MK_DSTB ( 40AB  ) }, 
		{MK_DSTB ( 40AC  ) }, 
		{MK_DSTB ( 40AD  ) }, 
		{MK_DSTB ( 40AE  ) }, 
		{MK_DSTB ( 40AF  ) }, 
		{MK_DSTB ( 40B0  ) }, 
		{MK_DSTB ( 40B1  ) }, 
		{MK_DSTB ( 40B2  ) }, 
		{MK_DSTB ( 40B3  ) }, 
		{MK_DSTB ( 40B4  ) }, 
		{MK_DSTB ( 40B5  ) }, 
		{MK_DSTB ( 40B6  ) }, 
		{MK_DSTB ( 40B7  ) }, 
		{MK_DSTB ( 40B8  ) }, 
		{MK_DSTB ( 40B9  ) }, 
		{MK_DSTB ( 40BA  ) }, 
		{MK_DSTB ( 40BB  ) }, 
		{MK_DSTB ( 40BC  ) }, 
		{MK_DSTB ( 40BD  ) }, 
		{MK_DSTB ( 40BE  ) }, 
		{MK_DSTB ( 40BF  ) }, 
		{MK_DSTB ( 40C0  ) }, 
		{MK_DSTB ( 40C1  ) }, 
		{MK_DSTB ( 40C2  ) }, 
		{MK_DSTB ( 40C3  ) }, 
		{MK_DSTB ( 40C4  ) }, 
		{MK_DSTB ( 40C5  ) }, 
		{MK_DSTB ( 40C6  ) }, 
	
		{MK_DSTB ( 40C7  ) }, 
		{MK_DSTB ( 40C8  ) }, 
		{MK_DSTB ( 40C9  ) }, 
		{MK_DSTB ( 40CA  ) }, 
		{MK_DSTB ( 40CB  ) }, 
		{MK_DSTB ( 40CC  ) }, 
		{MK_DSTB ( 40CD  ) }, 
		{MK_DSTB ( 40CE  ) }, 
		{MK_DSTB ( 40CF  ) }, 
		{MK_DSTB ( 40D0  ) }, 
		{MK_DSTB ( 40D1  ) }, 
		{MK_DSTB ( 40D2  ) }, 
		{MK_DSTB ( 40D3  ) }, 
		{MK_DSTB ( 40D4  ) }, 
		{MK_DSTB ( 40D5  ) }, 
		{MK_DSTB ( 40D6  ) }, 
		{MK_DSTB ( 40D7  ) }, 
		{MK_DSTB ( 40D8  ) }, 
		{MK_DSTB ( 40D9  ) }, 
		{MK_DSTB ( 40DA  ) }, 
		{MK_DSTB ( 40DB  ) }, 
		{MK_DSTB ( 40DC  ) }, 
		{MK_DSTB ( 40DD  ) }, 
		{MK_DSTB ( 40DE  ) }, 
		{MK_DSTB ( 40DF  ) }, 
		{MK_DSTB ( 40E0  ) }, 
		{MK_DSTB ( 40E1  ) }, 
		{MK_DSTB ( 40E2  ) }, 
		{MK_DSTB ( 40E3  ) }, 
		{MK_DSTB ( 40E4  ) }, 
		{MK_DSTB ( 40E5  ) }, 
		{MK_DSTB ( 40E6  ) }, 
		{MK_DSTB ( 40E7  ) }, 
		{MK_DSTB ( 40E8  ) }, 
		{MK_DSTB ( 40E9  ) }, 
		{MK_DSTB ( 40EA  ) }, 
		{MK_DSTB ( 40EB  ) }, 
		{MK_DSTB ( 40EC  ) }, 
		{MK_DSTB ( 40ED  ) }, 
		{MK_DSTB ( 40EE  ) }, 
		{MK_DSTB ( 40EF  ) }, 
		{MK_DSTB ( 40F0  ) }, 
		{MK_DSTB ( 40F1  ) }, 
		{MK_DSTB ( 40F2  ) }, 
		{MK_DSTB ( 40F3  ) }, 
		{MK_DSTB ( 40F4  ) }, 
		{MK_DSTB ( 40F5  ) }, 
		{MK_DSTB ( 40F6  ) }, 
		{MK_DSTB ( 40F7  ) }, 
		{MK_DSTB ( 40F8  ) }, 
		{MK_DSTB ( 40F9  ) }, 
		{MK_DSTB ( 40FA  ) }, 
		{MK_DSTB ( 40FB  ) }, 
		{MK_DSTB ( 40FC  ) }, 
		{MK_DSTB ( 40FD  ) }, 
		{MK_DSTB ( 40FE  ) }, 
    {MK_DSTB ( 40FF  ) }, 
    {MK_DSTB ( 4100  ) }, 
    {MK_DSTB ( 4101  ) }, 
    {MK_DSTB ( 4102  ) }, 
    {MK_DSTB ( 4103  ) }, 
    {MK_DSTB ( 4104  ) }, 
    {MK_DSTB ( 4105  ) }, 
    {MK_DSTB ( 4106  ) }, 
    {MK_DSTB ( 4107  ) }, 
    {MK_DSTB ( 4108  ) }, 
    {MK_DSTB ( 4109  ) }, 
    {MK_DSTB ( 410A  ) }, 
    {MK_DSTB ( 410B  ) }, 
    {MK_DSTB ( 410C  ) }, 
    {MK_DSTB ( 410D  ) }, 
    {MK_DSTB ( 410E  ) }, 
    {MK_DSTB ( 410F  ) }, 
    {MK_DSTB ( 4110  ) }, 
    {MK_DSTB ( 4111  ) }, 
    {MK_DSTB ( 4112  ) }, 
    {MK_DSTB ( 4113  ) }, 
    {MK_DSTB ( 4114  ) }, 
    {MK_DSTB ( 4115  ) }, 
    {MK_DSTB ( 4116  ) }, 
    {MK_DSTB ( 4117  ) }, 
    {MK_DSTB ( 4118  ) }, 
    {MK_DSTB ( 4119  ) }, 
    {MK_DSTB ( 411A  ) }, 
    {MK_DSTB ( 411B  ) }, 
    {MK_DSTB ( 411C  ) }, 
    {MK_DSTB ( 411D  ) }, 
    {MK_DSTB ( 411E  ) }, 
    {MK_DSTB ( 411F  ) }, 
    {MK_DSTB ( 4120  ) }, 
    {MK_DSTB ( 4121  ) }, 
    {MK_DSTB ( 4122  ) }, 
    {MK_DSTB ( 4123  ) }, 
    {MK_DSTB ( 4124  ) }, 
    {MK_DSTB ( 4125  ) }, 
    {MK_DSTB ( 4126  ) }, 
    {MK_DSTB ( 4127  ) }, 
    {MK_DSTB ( 4128  ) }, 
    {MK_DSTB ( 4129  ) }, 
    {MK_DSTB ( 412A  ) }, 
    {MK_DSTB ( 412B  ) }, 
    {MK_DSTB ( 412C  ) }, 
    {MK_DSTB ( 412D  ) }, 
    {MK_DSTB ( 412E  ) }, 
    {MK_DSTB ( 412F  ) }, 
    {MK_DSTB ( 4130  ) }, 
    {MK_DSTB ( 4131  ) }, 
    {MK_DSTB ( 4132  ) }, 
    {MK_DSTB ( 4133  ) }, 
    {MK_DSTB ( 4134  ) }, 
    {MK_DSTB ( 4135  ) }, 
    {MK_DSTB ( 4136  ) }, 
    {MK_DSTB ( 4137  ) }, 
    {MK_DSTB ( 4138  ) }, 
    {MK_DSTB ( 4139  ) }, 
    {MK_DSTB ( 413A  ) }, 
    {MK_DSTB ( 413B  ) }, 
    {MK_DSTB ( 413C  ) }, 
    {MK_DSTB ( 413D  ) }, 
    {MK_DSTB ( 413E  ) }, 
    {MK_DSTB ( 413F  ) }, 
    {MK_DSTB ( 4140  ) }, 
    {MK_DSTB ( 4141  ) }, 
    {MK_DSTB ( 4142  ) }, 
    {MK_DSTB ( 4143  ) }, 
    {MK_DSTB ( 4144  ) }, 
    {MK_DSTB ( 4145  ) }, 
    {MK_DSTB ( 4146  ) }, 
    {MK_DSTB ( 4147  ) }, 
    {MK_DSTB ( 4148  ) }, 
    {MK_DSTB ( 4149  ) }, 
    {MK_DSTB ( 414A  ) }, 
    {MK_DSTB ( 414B  ) }, 
    {MK_DSTB ( 414C  ) }, 
    {MK_DSTB ( 414D  ) }, 
    {MK_DSTB ( 414E  ) }, 
    {MK_DSTB ( 414F  ) }, 
    {MK_DSTB ( 4150  ) }, 
    {MK_DSTB ( 4151  ) }, 
    {MK_DSTB ( 4152  ) }, 
    {MK_DSTB ( 4153  ) }, 
    {MK_DSTB ( 4154  ) }, 
    {MK_DSTB ( 4155  ) }, 
    {MK_DSTB ( 4156  ) }, 
    {MK_DSTB ( 4157  ) }, 
    {MK_DSTB ( 4158  ) }, 
    {MK_DSTB ( 4159  ) }, 
    {MK_DSTB ( 415A  ) }, 
    {MK_DSTB ( 415B  ) }, 
    {MK_DSTB ( 415C  ) }, 
    {MK_DSTB ( 415D  ) }, 
    {MK_DSTB ( 415E	 ) }, 
    {MK_DSTB ( 415F  ) }, 
    {MK_DSTB ( 4160  ) }, 
    {MK_DSTB ( 4161  ) }, 
    {MK_DSTB ( 4162  ) }, 
    {MK_DSTB ( 4163  ) }, 
    {MK_DSTB ( 4164  ) }, 
    {MK_DSTB ( 4165  ) }, 
    {MK_DSTB ( 4166  ) }, 
    {MK_DSTB ( 4167  ) }, 
    {MK_DSTB ( 4168  ) }, 
    {MK_DSTB ( 4169  ) }, 
    {MK_DSTB ( 416A  ) }, 
    {MK_DSTB ( 416B  ) }, 
    {MK_DSTB ( 416C  ) }, 
    {MK_DSTB ( 416D  ) }, 
    {MK_DSTB ( 416E  ) }, 
    {MK_DSTB ( 416F  ) }, 
    {MK_DSTB ( 4170  ) }, 
    {MK_DSTB ( 4171  ) }, 
    {MK_DSTB ( 4172  ) }, 
    {MK_DSTB ( 4173  ) }, 
    {MK_DSTB ( 4174  ) }, 
    {MK_DSTB ( 4175  ) }, 
    {MK_DSTB ( 4176  ) }, 
    {MK_DSTB ( 4177  ) }, 
    {MK_DSTB ( 4178  ) }, 
    {MK_DSTB ( 4179  ) }, 
    {MK_DSTB ( 417A  ) }, 
    {MK_DSTB ( 417B  ) }, 
    {MK_DSTB ( 417C  ) }, 
    {MK_DSTB ( 417D  ) }, 
    {MK_DSTB ( 417E  ) }, 
    {MK_DSTB ( 417F  ) }, 
    {MK_DSTB ( 4180  ) }, 
    {MK_DSTB ( 4181  ) }, 
    {MK_DSTB ( 4182  ) }, 
    {MK_DSTB ( 4183  ) }, 
    {MK_DSTB ( 4184  ) }, 
    {MK_DSTB ( 4185  ) }, 
    {MK_DSTB ( 4186  ) }, 
    {MK_DSTB ( 4187  ) }, 
    {MK_DSTB ( 4188  ) }, 
    {MK_DSTB ( 4189  ) }, 
    {MK_DSTB ( 418A  ) }, 
    {MK_DSTB ( 418B  ) }, 
    {MK_DSTB ( 418C  ) }, 
    {MK_DSTB ( 418D  ) }, 
    {MK_DSTB ( 418E  ) }, 
    {MK_DSTB ( 418F  ) }, 
    {MK_DSTB ( 4190  ) }, 
    {MK_DSTB ( 4191  ) }, 
    {MK_DSTB ( 4192  ) }, 
    {MK_DSTB ( 4193  ) }, 
    {MK_DSTB ( 4194  ) }, 
    {MK_DSTB ( 4195  ) }, 
    {MK_DSTB ( 4196  ) }, 
    {MK_DSTB ( 4197  ) }, 
    {MK_DSTB ( 4198  ) }, 
    {MK_DSTB ( 4199  ) }, 
    {MK_DSTB ( 419A  ) }, 
    {MK_DSTB ( 419B  ) }, 
    {MK_DSTB ( 419C  ) }, 
    {MK_DSTB ( 419D  ) }, 
    {MK_DSTB ( 419E  ) }, 
    {MK_DSTB ( 419F  ) }, 
    {MK_DSTB ( 41A0  ) }, 
    {MK_DSTB ( 41A1  ) }, 
    {MK_DSTB ( 41A2  ) }, 
    {MK_DSTB ( 41A3  ) }, 
    {MK_DSTB ( 41A4  ) }, 
    {MK_DSTB ( 41A5  ) }, 
    {MK_DSTB ( 41A6  ) }, 
    {MK_DSTB ( 41A7  ) }, 
    {MK_DSTB ( 41A8  ) }, 
    {MK_DSTB ( 41A9  ) },                            
    {MK_DSTB ( 41AA  ) },                            
    {MK_DSTB ( 41AB  ) },                            
    {MK_DSTB ( 41AC  ) },                            
    {MK_DSTB ( 41AD  ) },                            
    {MK_DSTB ( 41AE  ) },                            
    {MK_DSTB ( 41AF  ) },                            
    {MK_DSTB ( 41B0  ) },                            
    {MK_DSTB ( 41B1  ) },                            
    {MK_DSTB ( 41B2  ) },                            
    {MK_DSTB ( 41B3  ) },                            
    {MK_DSTB ( 41B4  ) },                            
    {MK_DSTB ( 41B5  ) },                            
    {MK_DSTB ( 41B6  ) },                            
    {MK_DSTB ( 41B7  ) },                            
    {MK_DSTB ( 41B8  ) },                            
    {MK_DSTB ( 41B9  ) },                            
    {MK_DSTB ( 41BA  ) },                            
    {MK_DSTB ( 41BB  ) },                            
    {MK_DSTB ( 41BC  ) },                            
    {MK_DSTB ( 41BD  ) },                            
    {MK_DSTB ( 41BE  ) },                            
    {MK_DSTB ( 41BF  ) },                            
    {MK_DSTB ( 41C0  ) },                            
    {MK_DSTB ( 41C1  ) },                            
    {MK_DSTB ( 41C2  ) },                            
    {MK_DSTB ( 41C3  ) },                            
    {MK_DSTB ( 41C4  ) },                            
    {MK_DSTB ( 41C5  ) },                            
    {MK_DSTB ( 41C6  ) },                            
    {MK_DSTB ( 41C7  ) },                            
    {MK_DSTB ( 41C8  ) },                            
    {MK_DSTB ( 41C9  ) },                            
    {MK_DSTB ( 41CA  ) },                            
    {MK_DSTB ( 41CB  ) },                            
    {MK_DSTB ( 41CC  ) },                            
    {MK_DSTB ( 41CD  ) },                            
    {MK_DSTB ( 41CE  ) },                            
    {MK_DSTB ( 41CF  ) },                            
    {MK_DSTB ( 41D0  ) },                            
    {MK_DSTB ( 41D1  ) },                            
    {MK_DSTB ( 41D2  ) },                            
    {MK_DSTB ( 41D3  ) },                            
    {MK_DSTB ( 41D4  ) },                            
    {MK_DSTB ( 41D5  ) },                            
    {MK_DSTB ( 41D6  ) },                            
    {MK_DSTB ( 41D7  ) },                            
    {MK_DSTB ( 41D8  ) },                            
    {MK_DSTB ( 41D9  ) },                            
    {MK_DSTB ( 41DA  ) },                            
    {MK_DSTB ( 41DB  ) },                            
    {MK_DSTB ( 41DC  ) },                            
    {MK_DSTB ( 41DD  ) },                            
    {MK_DSTB ( 41DE  ) },                            
    {MK_DSTB ( 41DF  ) },                            
    {MK_DSTB ( 41E0  ) },                            
    {MK_DSTB ( 41E1  ) },                            
    {MK_DSTB ( 41E2  ) },                            
    {MK_DSTB ( 41E3  ) },                            
    {MK_DSTB ( 41E4  ) },                            
    {MK_DSTB ( 41E5  ) },                            
    {MK_DSTB ( 41E6  ) },                            
    {MK_DSTB ( 41E7  ) },                            
    {MK_DSTB ( 41E8  ) },                            
    {MK_DSTB ( 41E9  ) },                            
    {MK_DSTB ( 41EA  ) },                            
    {MK_DSTB ( 41EB  ) },                            
    {MK_DSTB ( 41EC  ) },                            
    {MK_DSTB ( 41ED  ) },                            
    {MK_DSTB ( 41EE  ) },                            
    {MK_DSTB ( 41EF  ) },                            
    {MK_DSTB ( 41F0  ) },                            
    {MK_DSTB ( 41F1  ) },                            
    {MK_DSTB ( 41F2  ) },                            

    {MK_DSTB ( 4401 ) },
    {MK_DSTB ( 4402 ) },

    {MK_DSTB ( 4601 ) },
    {MK_DSTB ( 4602 ) },
    {MK_DSTB ( 4603 ) },
    {MK_DSTB ( 4604 ) },
    {MK_DSTB ( 4605 ) },
    {MK_DSTB ( 4606 ) },
    {MK_DSTB ( 4607 ) },
};
static Data<DataType1> YkData[] = {
    {MK_DSTB ( 6001 ) },
    {MK_DSTB ( 6002 ) },
    {MK_DSTB ( 6064 ) },
    {MK_DSTB ( 6065 ) },
    {MK_DSTB ( 6066 ) },
};

static Data<BaseDataType> ParaData[] = {
    {MK_DSTB ( 5001 ) },
    {MK_DSTB ( 5002 ) },
    {MK_DSTB ( 5003 ) },
    {MK_DSTB ( 5004 ) },
    {MK_DSTB ( 5005 ) },
    {MK_DSTB ( 5006 ) },
    {MK_DSTB ( 5007 ) },
    {MK_DSTB ( 5008 ) },
    {MK_DSTB ( 5009 ) },
    {MK_DSTB ( 500A ) },
    {MK_DSTB ( 500B ) },
    {MK_DSTB ( 500C ) },
    {MK_DSTB ( 500D ) },
    {MK_DSTB ( 500E ) },
    {MK_DSTB ( 500F ) },
    {MK_DSTB ( 5010 ) },
    {MK_DSTB ( 5011 ) },
    {MK_DSTB ( 5012 ) },
    {MK_DSTB ( 5013 ) },
    {MK_DSTB ( 5014 ) },
    {MK_DSTB ( 5015 ) },
    {MK_DSTB ( 5016 ) },
    {MK_DSTB ( 5017 ) },
    {MK_DSTB ( 5018 ) },
    {MK_DSTB ( 5019 ) },
    {MK_DSTB ( 501A ) },
    {MK_DSTB ( 501B ) },
    {MK_DSTB ( 501C ) },
    {MK_DSTB ( 501D ) },
    {MK_DSTB ( 501E ) },
    {MK_DSTB ( 501F ) },
    {MK_DSTB ( 5020 ) },
    {MK_DSTB ( 5021 ) },
    {MK_DSTB ( 5022 ) },
    {MK_DSTB ( 5023 ) },
    {MK_DSTB ( 5024 ) },
    {MK_DSTB ( 5025 ) },
    {MK_DSTB ( 5026 ) },
    {MK_DSTB ( 5027 ) },
    {MK_DSTB ( 5028 ) },
    {MK_DSTB ( 5029 ) },
    {MK_DSTB ( 502A ) },
    {MK_DSTB ( 502B ) },
    {MK_DSTB ( 502C ) },
    {MK_DSTB ( 502D ) },
    {MK_DSTB ( 502E ) },
    {MK_DSTB ( 502F ) },
    {MK_DSTB ( 5030 ) },
    {MK_DSTB ( 5031 ) },
    {MK_DSTB ( 5032 ) },
    {MK_DSTB ( 5033 ) },
    {MK_DSTB ( 5034 ) },
    {MK_DSTB ( 5035 ) },
    {MK_DSTB ( 5036 ) },
    {MK_DSTB ( 5037 ) },
    {MK_DSTB ( 5038 ) },
    {MK_DSTB ( 5039 ) },
    {MK_DSTB ( 503A ) },
    {MK_DSTB ( 503B ) },
    {MK_DSTB ( 503C ) },
    {MK_DSTB ( 503D ) },
    {MK_DSTB ( 503E ) },
    {MK_DSTB ( 503F ) },
    {MK_DSTB ( 5040 ) },
    {MK_DSTB ( 5041 ) },
    {MK_DSTB ( 5042 ) },
    {MK_DSTB ( 5043 ) },
    {MK_DSTB ( 5044 ) },
    {MK_DSTB ( 5045 ) },
    {MK_DSTB ( 5046 ) },
    {MK_DSTB ( 5047 ) },
    {MK_DSTB ( 5048 ) },
    {MK_DSTB ( 5049 ) },
    {MK_DSTB ( 504A ) },
    {MK_DSTB ( 504B ) },
    {MK_DSTB ( 504C ) },
    {MK_DSTB ( 504D ) },
    {MK_DSTB ( 504E ) },
    {MK_DSTB ( 504F ) },
    {MK_DSTB ( 5050 ) },
    {MK_DSTB ( 5051 ) },
    {MK_DSTB ( 5052 ) },
    {MK_DSTB ( 5053 ) },
    {MK_DSTB ( 5054 ) },
    {MK_DSTB ( 5055 ) },

    {MK_DSTB ( 5080 ) },
    {MK_DSTB ( 5081 ) },
    {MK_DSTB ( 5082 ) },
    {MK_DSTB ( 5083 ) },
    {MK_DSTB ( 5084 ) },
    {MK_DSTB ( 5085 ) },
    {MK_DSTB ( 5086 ) },
    {MK_DSTB ( 5087 ) },
    {MK_DSTB ( 5088 ) },
    {MK_DSTB ( 5089 ) },
    {MK_DSTB ( 508A ) },
    {MK_DSTB ( 508B ) },
    {MK_DSTB ( 508C ) },
    {MK_DSTB ( 508D ) },
    {MK_DSTB ( 508E ) },
    {MK_DSTB ( 508F ) },
    {MK_DSTB ( 5090 ) },
    {MK_DSTB ( 5091 ) },
    {MK_DSTB ( 5092 ) },
    {MK_DSTB ( 5093 ) },
    {MK_DSTB ( 5094 ) },
    {MK_DSTB ( 5095 ) },
    {MK_DSTB ( 5096 ) },
    {MK_DSTB ( 5097 ) },
    {MK_DSTB ( 5098 ) },
    {MK_DSTB ( 5099 ) },
    {MK_DSTB ( 509A ) },
    {MK_DSTB ( 509B ) },
    {MK_DSTB ( 509C ) },
    {MK_DSTB ( 509D ) },
    {MK_DSTB ( 509E ) },
    {MK_DSTB ( 509F ) },
    {MK_DSTB ( 50A0 ) },
    {MK_DSTB ( 50A1 ) },
    {MK_DSTB ( 50A2 ) },
    {MK_DSTB ( 50A3 ) },
    {MK_DSTB ( 50A4 ) },
    {MK_DSTB ( 50A5 ) },
    {MK_DSTB ( 50A6 ) },
    {MK_DSTB ( 50A7 ) },
    {MK_DSTB ( 50A8 ) },
    {MK_DSTB ( 50A9 ) },
    {MK_DSTB ( 50AA ) },
    {MK_DSTB ( 50AB ) },
    {MK_DSTB ( 50AC ) },
    {MK_DSTB ( 50AD ) },
    {MK_DSTB ( 50AE ) },
    {MK_DSTB ( 50AF ) },
    {MK_DSTB ( 50B0 ) },
    {MK_DSTB ( 50B1 ) },
    {MK_DSTB ( 50B2 ) },
    {MK_DSTB ( 50B3 ) },
    {MK_DSTB ( 50B4 ) },
    {MK_DSTB ( 50B5 ) },
    {MK_DSTB ( 50B6 ) },
    {MK_DSTB ( 50B7 ) },
    {MK_DSTB ( 50B8 ) },

    {MK_DSTB ( 5100 ) },
    {MK_DSTB ( 5101 ) },
    {MK_DSTB ( 5102 ) },
    {MK_DSTB ( 5103 ) },
    {MK_DSTB ( 5104 ) },
    {MK_DSTB ( 5105 ) },
    {MK_DSTB ( 5106 ) },

    {MK_DSTB ( 5200 ) },
    {MK_DSTB ( 5201 ) },
    {MK_DSTB ( 5202 ) },
    {MK_DSTB ( 5203 ) },
    {MK_DSTB ( 5204 ) },
    {MK_DSTB ( 5205 ) },
    {MK_DSTB ( 5206 ) },
    {MK_DSTB ( 5207 ) },
    {MK_DSTB ( 5208 ) },
    {MK_DSTB ( 5209 ) },
    {MK_DSTB ( 520A ) },
    {MK_DSTB ( 520B ) },
    {MK_DSTB ( 520C ) },
    {MK_DSTB ( 520D ) },
    {MK_DSTB ( 520E ) },
    {MK_DSTB ( 520F ) },
    {MK_DSTB ( 5210 ) },
    {MK_DSTB ( 5211 ) },
    {MK_DSTB ( 5212 ) },


};
static struct mutex data_mutex;
UserData::UserData()
{
	mutex_init(&data_mutex, "data mutex");
	thread_cond_init(&cond,"cond");
	UserSetDataFun=NULL;
}

UserData::~UserData()
{
    mutex_destroy ( &data_mutex );
}

int16 UserData::GetData ( int32 Id, Data<BaseDataType>* data )
{
    uint16 i;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( data, &YxData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( data, &YcData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( data, &YkData[i], sizeof ( Data<BaseDataType> ) );
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    mutex_unlock ( &data_mutex );

    return 0;
}

int16 UserData::SetData ( int32 Id, Data<BaseDataType>* data )
{  
	
    uint16 i;
    int16 find=0;
    mutex_lock ( &data_mutex );
    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( & ( YxData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YxData[i].Size );
                find=1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( & ( YcData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YcData[i].Size );
                find=1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( & ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), ParaData[i].Size );
                find=1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( & ( YkData[i].Data->Data ), & ( ( ( DataType1* ) ( data->Data ) )->Data ), YkData[i].Size );
                find=1;
                goto Leave;
            }
        }
    }
Leave:		
	if(UserSetDataFun!=NULL&&find==1)
			(*UserSetDataFun)(Id);
    mutex_unlock ( &data_mutex );
		if(find==1)
    return 1;
    else
    return 0;

}

int16 UserData::GetDataSize ( int32 Id )
{
    uint16 i;

    mutex_lock ( &data_mutex );
    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YxData[i].Size;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YcData[i].Size;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ParaData[i].Size;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return YkData[i].Size;
            }
        }
    }

    mutex_unlock ( &data_mutex );
    return 0;
}

Data<BaseDataType>* UserData::GetDataPtr ( int32 Id )
{
    uint16 i;

    mutex_lock ( &data_mutex );
    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YxData[i];
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YcData[i];
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return &ParaData[i];
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                mutex_unlock ( &data_mutex );
                return ( Data<BaseDataType>* ) &YkData[i];
            }
        }
    }
    mutex_unlock ( &data_mutex );
    return 0;
}
int16 UserData::GetDataType ( int32 Id )
{

    if ( Id < 0x4001 ) {
                return MSG_YX;
    }

    if ( Id < 0x5001 ) {
                return MSG_YC;
    }

    if ( Id < 0x6001 ) {
                return MSG_PARA;
    }

    if ( Id < 0x6201 ) {
                return MSG_YK;
    }
    return 0;
}

int16 UserData::GetData ( int32 Id, Data<BaseDataType>* data,char *r_data)
{
    uint16 i;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy ( data, &YxData[i], sizeof ( Data<BaseDataType> ) );
                memcpy(r_data,&(YxData[i].Data->Data),YxData[i].Size);
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy ( data, &YcData[i], sizeof ( Data<BaseDataType> ) );
                memcpy(r_data,&(YcData[i].Data->Data),YcData[i].Size);
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy ( data, &ParaData[i], sizeof ( Data<BaseDataType> ) );
                memcpy(r_data,& ( ( ( DataType1* ) ( ParaData[i].Data ) )->Data ),ParaData[i].Size);
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy ( data, &YkData[i], sizeof ( Data<BaseDataType> ) );
                memcpy(r_data,&(YkData[i].Data->Data),YkData[i].Size);
                mutex_unlock ( &data_mutex );
                return 1;
            }
        }
    }

    mutex_unlock ( &data_mutex );

    return 0;

}	
int16 UserData::SetData ( int32 Id, char* data )	
{
    uint16 i;
    int16 find=0;
    mutex_lock ( &data_mutex );

    if ( Id < 0x4001 ) {
        for ( i = 0;i < sizeof ( YxData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YxData[i].Id ) {
                memcpy(YxData[i].Data,data,YxData[i].Size);
                find=1;
                goto Leave;
            }
        }
    }

    if ( Id < 0x5001 ) {
        for ( i = 0;i < sizeof ( YcData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YcData[i].Id ) {
                memcpy(YcData[i].Data,data,YcData[i].Size);
                find=1;
                goto Leave;            
            }
        }
    }

    if ( Id < 0x6001 ) {
        for ( i = 0;i < sizeof ( ParaData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == ParaData[i].Id ) {
                memcpy(ParaData[i].Data,data,ParaData[i].Size);
                find=1;
                goto Leave;            
            }
        }
    }

    if ( Id < 0x6201 ) {
        for ( i = 0;i < sizeof ( YkData ) / sizeof ( Data<BaseDataType> );i++ ) {
            if ( Id == YkData[i].Id ) {
                memcpy(YkData[i].Data,data,YkData[i].Size);
                find=1;
                goto Leave;            }
        }
    }

Leave:		
	if(UserSetDataFun!=NULL&&find==1)
			(*UserSetDataFun)(Id);
    mutex_unlock ( &data_mutex );
		if(find==1)
    return 1;
    else
    return 0;
	
}

#define  YXTAB 0
#define  YCTAB 1
#define  PARATAB 2
#define  YKTAB 3
Data<BaseDataType> *UserData::RecallData(uint16 DataType, uint16 DataPosInTab)
{
//DataPosInTab ²ÎÊý´øÈëÎ´×÷Òç³ö´¦Àí Ê¹ÓÃÊ±Çë×¢Òâ£¡
	switch (DataType)
	{
	case YXTAB:
		return (Data<BaseDataType> *)&YxData[DataPosInTab];
	case YCTAB:
		return (Data<BaseDataType> *)&YcData[DataPosInTab];
	case PARATAB:
		return (Data<BaseDataType> *)&ParaData[DataPosInTab];
	case YKTAB:
		return (Data<BaseDataType> *)&YkData[DataPosInTab];
	default:
		return NULL;
	}
}
