/*
 * data.h
 *	内容：遥信数据，终端参数，声明一些被使用的函数
 *  Created on: 2014-10-18
 *      Author: Administrator
 */

#ifndef DATA_H_
#define DATA_H_
#include "datatype.h"
#ifdef __cplusplus
extern "C" {
#endif
#define LOCK 1
#define UNLOCK 0


//定义终端最大串口数

#define MAX_SERIAL_NUM 2
#define MAX_NET_NUM 1
#define MAX_SOCKET_NUM 1
#define MAX_COM_PORT_NUM MAX_SERIAL_NUM + MAX_NET_NUM*MAX_SOCKET_NUM

//定义终端最大回线数
#ifndef MAX_LINE_NUM
#define MAX_LINE_NUM 1
#endif
#define MAX_SECTION_NUM 2 //保护参数套数
#define SEGMENT_NUM 3 //三段式保护

#define ENABLE 1
#define DISENABLE 0


#define _0_HE_1_KAI 0
#define _1_HE_0_KAI 1
#define _1_KAI_0_HE 1 //终端默认的分合与电平关系
#define _0_KAI_1_HE 0 //终端默认的分合与电平关系


#define MAX_COUPLEYX_NUM 2
#define MAX_YK_NUM 2
//定义终端最大遥测数
#define MAX_YC_NUM  26
#define CONFIG_YC_NUM  13

#define PI 3.1415926
#define TWO_PI 6.2831852
#define TWOOFTHREE_PI 2.094395

#define _SYSHE_ 1
#define _SYSFEN_ 0

#define CLOSEBREAKER 1
#define OPENBREAKER  0
#define YUANFANG 0
#define JIUDI 0

#define MAXHYXNUM 11
#define TOTALYXNUM 100

typedef struct yxdata{
	uint16 id;
	uint16 addr;
	uint16 nature;
	Uint16 statu;
	Uint16 priority;//优先级别
	Uint16 type;//单双点；
}yxdata_t;
/*BL:欠压
 *AN:交流失电
 *BR:电池活化
 *BR:电池活化
 *OV：电源故障
 */
/* 在本项目中Hyx定义为
0 	合位		
1 	分位		
2 	储能位		
3 	远方就地位	
4 	手动分闸		
5 	手动合闸		
6	VL 
7	POK
8	VH 
9	HOK 
10 双点遥信

VL	电池欠压告警
POK	交流失电告警
VH	电源故障告警
HOK	活化状态
*/
typedef struct allyx{
	 yxdata_t pt_yx[MAX_LINE_NUM];
	 yxdata_t ct_yx[MAX_LINE_NUM];
	 yxdata_t Hyx[MAXHYXNUM];//前六个是用户遥信，第11个是双点遥信，7(欠压),8（失电）,9（电源异常）,10（活化）是电源管理遥信。
	 yxdata_t CurrentAlarmYx[MAX_LINE_NUM][3];//三段过流信号
	 yxdata_t CurrentActionYx[MAX_LINE_NUM][3];//三段过流动作信号

	 yxdata_t GrdAlarmYx[MAX_LINE_NUM][3];//三段零序过流信号
	 yxdata_t GrdActionYx[MAX_LINE_NUM][3];//三段零序过流动作信号

	 yxdata_t U0Alarm[MAX_LINE_NUM];//零序电压保护信号(已取消该保护，但保留信号)
	 yxdata_t U0Action[MAX_LINE_NUM];//零序电压保护动作信号(已取消该保护，但保留信号)

	 yxdata_t HjsActionYx[MAX_LINE_NUM];//后加速动作信号

	 yxdata_t ChzYx[MAX_LINE_NUM];//重合闸动作信号。启动，成功/失败；

	 yxdata_t JyYx[MAX_LINE_NUM];//零序电流阶跃故障信号
	 yxdata_t JyActionYx[MAX_LINE_NUM];//零序电流阶跃动作信号

	 yxdata_t JynegIYx[MAX_LINE_NUM];//负序电流阶跃信号
	 
	 yxdata_t P0Yx[MAX_LINE_NUM];//零序功率故障信号(已取消该保护，但保留信号)
	 yxdata_t P0ActionYx[MAX_LINE_NUM];//零序功率动作信号(已取消该保护，但保留信号)

	 yxdata_t JyP0Yx[MAX_LINE_NUM];//零序功率阶跃信号(已取消该保护，但保留信号)
	 yxdata_t JyP0ActionYx[MAX_LINE_NUM];//零序功率阶跃动作信号(已取消该保护，但保留信号)

	 yxdata_t AngleYx[MAX_LINE_NUM];//相位保护信号
	 yxdata_t AngleActionYx[MAX_LINE_NUM];//相位保护动作信号

	 yxdata_t RaiseYx[MAX_LINE_NUM];//增量电流保护信号
	 yxdata_t RaiseActionYx[MAX_LINE_NUM];//增量电流保护动作信号

	 yxdata_t gzfg[MAX_LINE_NUM][2];//故障复归信号；0 瞬时，1永久
	 yxdata_t YueXianYx[MAX_LINE_NUM][10];//越限信号
	 yxdata_t ActionCkYx[MAX_LINE_NUM];//动作校验信号
	 yxdata_t TerminalCkYx;//装置故障信号
	 yxdata_t reverLineYx[MAX_LINE_NUM];//线路反向信号
	 yxdata_t reverXuYx[MAX_LINE_NUM];//电流逆相序信号
	 
}allyx;
extern allyx TerYx;


typedef struct yx_table{
	//char *soe_p;
	yxdata_t *pdata;
}YX_TAB;
extern YX_TAB YxTable[TOTALYXNUM] ;//按照POS固定顺序的排列表

typedef struct _sort_yx_table{
	yxdata_t *pdata;
}SORT_YX_TAB;
extern SORT_YX_TAB SortYxTable[TOTALYXNUM];// 按照地址顺序排列的表

typedef struct limt{
    float Limit;
    float Recover;
    int32 ContinueTime;
}limt ;
typedef enum YXENUM{
	HE1 = 0,///<断路器1合位
	FEN1,///<断路器1分位
	CN1, ///<断路器1储能位置
	GLD1,///<断路器1隔离刀闸位置（备用遥信4）
	YX1_5,///<备用遥信5变位,本次用于合闸按钮
	YX1_6,//<备用遥信6变位，本次用于分闸按钮
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
	MOTOR,	  //电机控制
}YKENUM_E;

#define HE(n) 	(HE1 + HE2*n)	///<断路器1合位
#define FEN(n)	(FEN1 + HE2*n)	///<断路器1分位
#define CN(n)	(CN1 + HE2*n)	///<断路器1储能位置
#define GLD(n)	(GLD1 + HE2*n)	///<断路器1隔离刀闸位置（备用遥信4）
#define BYX5(n)	(YX15 + HE2*n)	///<备用遥信5变位
#define BYX6(n)	(YX16 + HE2*n)	//<备用遥信6变位
#define XJGZ(n)	(XJGZ1 + HE2*n)	//故障指示器指示线路2相间短路故障
#define JDGZ(n)	(JDGZ1 + HE2*n)	//故障指示器指示线路2接地故障
#define SP6GZ(n)(SP6GZ1 + HE2*n)	//SF6气体异常报警信号
#define SP6BS(n)(SP6BS1 + HE2*n)	//SF6气压闭锁信号

typedef struct fourchar{
	long d1:8;
	long d2:8;
	long d3:8;
	long d4:8;
}fourchar;
typedef union ycfloatdata{
	float fdata;
	fourchar bitdata;
}ycfloatdata;

typedef struct passdata{
	ycfloatdata floatdata;
	int16      intdata;
}passdata;
typedef struct yc_data{
	float *src_yc;
	passdata *deadpass;
	short *Coef;//工程系数；
	short *range;//量程;
	float *dead;
	uint16 dataid;
	uint16 datasign;
	uint8 changeflag;
} YC_DATA ;
typedef struct yc_table {
	YC_DATA *ycdata;
} YC_TAB;
extern YC_TAB YcTable[MAX_YC_NUM ];
typedef struct dpyx_table{
	uint16 paddr;
	uint16 changeflag;
	unsigned int *yxdata;
						
} dpYX_TAB;
extern YC_DATA Yc[MAX_YC_NUM];

typedef struct BATTERYDATA{
	unsigned long gap;
	unsigned int StartHour;
	unsigned int StartMinute;

}BATTERYDATA_T;

typedef struct serial_para_tag {
	uint16 num:16;
    uint32 baudrate:32;
    uint16 bitnum:8;
    uint16 stopbit:8;
    uint16 checkbit:8;
    uint16 protocol:8;
} SERIAL_PARA;
typedef struct charip{
    unsigned long d1:8;
	unsigned long d2:8;
	unsigned long d3:8;
	unsigned long d4:8;
}charip_t;
typedef struct eth_para_tag {//lenth 37
	uint8 local_ip[4];
	uint8 mac[6];
	uint8 sub_mask[4];
	uint8 net_gate[4];
    uint8 enable;
    uint8 linkstatu;
    uint32 sendbyte;
    uint32 recvbyte;
} ETH_PARA;
typedef struct eth_com_tag {
	uint16 ethnum;//对应的网口好
	uint16 comtype;//通道标志，1 主通道，0子通道，默认子通道；
	int16 server_client;//1:是服务器，2是客户端，默认客户端
	int16 enable ;//通道启用标志；
	int16 tcpiptype;//1:TCP;2:UDP 默认tcp
	int16 net_port;//本地做服务器时的监听端口；
	uint8 remote_ip[4];//远端IP
	int16 dport;//远端端口
	int16 protocol;//规约
} ETH_COM_PARA;

typedef union charipunion{
	charip_t bit;
	long all;
}charip_u;

typedef struct sntpdata{
		charip_u severip;
		unsigned int Enable;
		//unsigned int TimeGap;
		unsigned int StartHour;
		unsigned int StartMinute;
}sntpdata_t;
typedef struct terminal_para_tag {//终端参数

//	uint16 ter_addr;
//	uint16 master_addr;
	uint16 nulldata[8];//未定义数据
	SERIAL_PARA ComPara[MAX_SERIAL_NUM];
	ETH_PARA EthPara[MAX_NET_NUM];
	ETH_COM_PARA EthSocketPara[MAX_SOCKET_NUM];
    uint32 yx_filter_time;//遥信防抖时间；
    uint32 ykchecktime;//遥控返校时间；
    uint32 yk_he_pulse;//遥控合脉冲时间
    uint32 yk_fen_pulse;//遥控分脉冲时间；
    uint32 ykdelaytime;//遥控延迟断开电源时间。
  //  unsigned int  ter_pswd[6];
    unsigned int  cascadeport;//CASCADE_PORT；
    BATTERYDATA_T battery_action_time ;
	sntpdata_t sntppara;
	uint16 show_data;//液晶显示一次值
	uint16 reverse[18];
	uint16 crc;
} TER_PARA;
extern TER_PARA TerPara;//终端参数

typedef struct time_data{
	 unsigned int millisecond;//0-999毫秒
	 unsigned int minute;//IV--是否有效 0有效 1无效 占字节最高位 第七位备用 低6位为数据（0-59分--在GB_T 18657.4-2002 远动设备及系统 第五部分 传输规约 第四篇 应用信息元素的编码和定义中例图有误写成0-99）
	 unsigned int hour;//SU--是否夏令时 0标准时间，1夏令时 占字节最高位，第六、七位备用，低5位为数据（0-23小时）
	 unsigned int day;//day_week:week占字节高3位(1-7星期的天)，day占字节低5位 （1-31月的天）
	 unsigned int month;//高四位备用 month占低4位（1-12月）
	 unsigned int year;//最高位备用 year占低7位（0-99年）

} CP56Time2a;
typedef struct time16_data{
	unsigned int millisecond;
}CP16Time2a;
typedef struct setmstime{
	uint64 T;
	uint8 SetFlag;
	uint64 *Psystime;
} setmstime_t;

#define MAXEVENTSOENUM 300
#define MAXYKSOENUM 60
#define MAXTERSOENUM 20
#define MAX_SOE_NUM 50
#define MAX_ACT_SOE_NUM 10
#define MAX_SOE_INFLASH		300
#define MAX_ACTSOE_INFLASH	100
#define EVENTSOE 1 //顺序SOE
#define YKSOE    2   //遥控SOE
#define TERSOE 3  // 终端故障SOE

#define PER_SOE_SIZE  (sizeof(SoeData))
#define PER_ACTION_SIZE  (sizeof(ActionData))
//动作记录结构
typedef struct _ActionData{
	uint16 dataid;		// dataid use for local show,from 0
	uint16 datasign;     //dagasign use for master communication
	uint16 status;
	float ua,ub,uc,u0,ia,ib,ic,i0,p0;
	int switch_state;
	CP56Time2a time;
	int txt_len;
}ActionData;

typedef struct _SoeData{
	uint16 dataid;	// dataid use for local show,from 0
	uint16 datasign; //dagasign use for master communication
	uint16  status;
	uint16  type;//单双点；
	CP56Time2a time;
	int txt_len; //txt len use for master  file transfer by ascii
}SoeData;
//soe 文件头
typedef struct _SoeHeader{
	uint16 cur_index;//指示当前soe索引号
	uint16 total_cnt;//指示当前文件中所存的soe数量
	uint16 total_txt_len;//指示当前文件中所有记录转换成ascii码的长度
	uint16 statu;//no use
	uint16 valid_flag[MAX_SOE_INFLASH];//指示当前记录是否有效。因为soe记录数大于actsoe记录数，所以，这里取MAX_SOE_INFLASH
}SoeHeader;
typedef struct soe_data {
	Uint16 ReadFlag1[MAX_COM_PORT_NUM];//一级
	Uint16 ReadFlag2[MAX_COM_PORT_NUM];//二级
	Uint16 SaveFlag;//EVENTSOE  YKSOE TERCHECK
	SoeData soe;
} SOE_DATA;

typedef struct soe_buffer {
	 unsigned int  produce_cnt;//生产消费指针用于二级数据的上送，目的是为了保证时间顺序。对于一级数据暂时不用，遍历一遍缓冲区
	 unsigned int  consume_cnt;
    SOE_DATA SoeData[MAX_SOE_NUM];
} SOE_BUFFER;

typedef struct action_soe_data {
	ActionData soe;
	uint16 SaveFlag;
} ACTION_DATA;
typedef struct action_soe_buffer {
	 unsigned int  produce_cnt;
	 unsigned int  consume_cnt;
	 ACTION_DATA SoeData[MAX_ACT_SOE_NUM];
} ACTION_SOE_BUFF;
extern ACTION_SOE_BUFF ActionSoeBuf;
#define SCIA 0
#define SCIB 1
#define SCIC 2
#define COM_SEND_BUFFER_SIZE 256
#define COM_RECV_BUFFER_SIZE 512
typedef struct combox{
			unsigned int 	recv_time_out;//接收超时
			unsigned int 	send_time_out;//发送超时
			unsigned int 	inbyte_cnt;//接收缓存填入数据计数
			unsigned int 	outbyte_cnt;//接收缓存取出数据计数
			unsigned char short_pack_buffer[10];//固定帧缓存
			unsigned char long_pack_buffer[COM_RECV_BUFFER_SIZE];//可变帧缓存
			unsigned int 	short_pack_byte_cnt;//固定帧字节计数
			unsigned int    long_pack_byte_cnt;//可变帧字节计数
			unsigned int 	short_start_flag;//固定帧检包开始标志
			unsigned int 	long_start_flag;//可变帧检包开始标志
			unsigned int 	exp_len;//期待的帧长度
			unsigned int 	wait_com_flag;//等待标志
			unsigned int 	wait_com_time;//等待时间
			unsigned int	link_wait_flag;
			unsigned int	link_wait_time;//等待链路连接的时间
			unsigned char	short_addr_buffer[10]; //这个是为马鞍山主站的招读终端地址准备的帧。
			unsigned int	short_addr_byte_cnt;	//
			unsigned int	short_addr_flag;
			unsigned char 	com_send_buffer[COM_SEND_BUFFER_SIZE];//发送缓存
			unsigned char 	com_recv_buffer[COM_RECV_BUFFER_SIZE];//接收缓存
} COMBOX;

typedef struct YC_ADDR_POS {
	unsigned int pos;
	unsigned int addr;
} YCADDRPOS ;

typedef struct ycpara{
	int16 num;
	YCADDRPOS addpos[MAX_YC_NUM];
}ycpara;

typedef struct YXPARA{
	uint16 dataid;
	uint16 addr;
	uint16 attrb;
}YXPARA_T;
typedef struct yxpara{
	uint16 num;
	YXPARA_T addpos[60];
}yxpara;

typedef struct tagComPub{
	uint16	ter_addr;
	uint16	master_addr;
	uint16	yc_ack_type;
	uint16	relink_time;
	uint16	per_grp_yx_cnt;
	uint16	per_grp_yc_cnt;
}ComPub;

typedef struct protocol_para_tag {  //规约参数
	yxpara	yx_dataaddr ;
	ycpara	yc_dataaddr;
	uint16	yk_dataaddr[MAX_YK_NUM];//未用
	ComPub	com_pub;
	uint16	yx_ack_type ;
	uint16     reverse[20];
	uint16     crc;
} PRO_PARA;

extern PRO_PARA ProPara;

typedef struct dead_band_para{
	int16 DeadTime;//遥测死区时间设置
    float DeadU;   ///<电压死区设置
    float DeadI;      ///<电流死区设置
    float DeadP;      ///<功率死区设置
    float DeadFreq;   ///<频率死区设置
    float DeadAngle;///<角度死区设置//和功率因数公用
    float DeadHarmoicValue;///<谐波有效值死区设置
    float DeadHarmoicCount;///<   谐波含有率死区设置
    float DeadTemp;   ///<温度死区设置
    float DeadHumidity;///<湿度死区设置
    uint16 reverse[20];
    uint16 crc;
} DEAD_BAND_PARA;
extern DEAD_BAND_PARA DeadBandPara;
extern DEAD_BAND_PARA DeadBandParaValue;//转换成绝对值

typedef struct unitary_table{
	int U_bdata;
	int I_bdata;
	int PQS_bdata;
	int COS_bdata;
	int F_bdata;
	int ANGLE_bdata;
	int DC_bdata;
	int TEMP_bdata;
}UNITARY_TAB;             //归一化系数表

typedef struct angleratio{
	float angle;//角度误差
	float cos_a;//角度误差的余弦值
	float sin_a;//角度误差的正弦值
}angleratio_t;
typedef struct ratio{
	float PortRatio[13];//UA1,UB1,UC1,UAB1,UBC1,UCA1,U01,IA11,IB11,IC11,I011,IA12,IB12,IC12,
	float PowerLineRatio[4];// Pa ; Pb ; Pc; Pall;  线性系数
	angleratio_t PowerAngleRatio[2][4];//Qa ; Qb ;  Qc; Qall; 角度系数
	uint16 reverse[20];
	uint16 crc;
}ratio_t;
extern ratio_t YcRatio[MAX_LINE_NUM];

typedef struct BASEPROSTATU{
	uint64 CurrentMs1;
	uint64 CurrentMs2;
	bool CurrentBool;
	bool Trigger;
}BASEPROSTATU_T;

//定义装置故障信息位
#define ERROR_FLASH 0
typedef struct _terminal_msg{//终端信息
	char term_type[32];
	char term_operating[32];
	char term_manufactor[32];
	char term_hw_ver[32];
	char term_sw_ver[32];
	char term_sw_verify[32];
	char term_model[32];
	char term_id[32];
	char term_mac[6];
	char term_sw_date[12];
	uint32 	term_device_state;//装置故障信息
	yxdata_t *SoeId;
	int    soe_flag;
}TerminalMsg;
extern  TerminalMsg term_msg;
#define SETBIT(dest,mask)     (dest |= mask)//目标位置位
#define CLRBIT(dest,mask)     (dest &= ~mask)//目标位清零
#define TGLBIT(dest,mask)     (dest ^= mask)//目标位取反
#define CHKBIT(dest,mask)     (dest & mask)//目标位检测

#define SET_BIT 0
#define CLR_BIT 1
#define CHECK_BIT 2
#define GET_BIT 3
#define GET_BITNOMOVE 4
extern unsigned int Para_buffer[2048];
//for  DEBUG_TIME 
extern Uint64 t1,t2;
extern int64 t3,max_t3,min_t3;
extern Uint64 t4,t5;
extern int64 t6,max_t6,min_t6;
extern Uint64 t7,t8;
extern int64 t9,max_t9,min_t9;

extern float *ptr_pt;//由于在液晶程序中不能使用类，所以当显示一次值需要使用pt，ct时，只能用全局参数
extern float *ptr_ct;
extern float *ptr_ct0;
extern float *ptr_pt0;
extern uint16 *ptr_d_yx;//双点遥信
extern int16 DogFood;
extern COMBOX ComBox[MAX_COM_PORT_NUM];
extern SOE_BUFFER SoeBuffer;
extern void FloatFFTInit(void);
extern void _FFTdel(float *inbuff,float *outbuff,float *magbuff,float *phasebuff);
extern void _16fft(Uint16 Chanel);
extern int16 LimitParaDefault ( void);
extern int16 LimitInit ( void );
extern int16 ResetFault( void  );
extern int16 TerParaDefault(void);
extern int16  YcRatioDefault(void);
extern int16  DeadParaDefault(void );
extern int16 DeadParaInit(void );
extern int16 ProParaDefault (void) ;
extern int16 ProParaInit(void);
extern void DataBuffClear(void);
extern int16 do_clear_soe_info(void );
extern int16 do_clear_action_info(void );
extern int16 do_set_default_para(void );
extern void ComBoxInit(void);
extern int    CheckTerminalState();
extern void InitTerminalState();
extern void show_ratio();
#ifdef __cplusplus
}
#endif
//////////////////
#endif /* DATA_H_ */
