/*
 * data.h
 *	���ݣ�ң�����ݣ��ն˲���������һЩ��ʹ�õĺ���
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


//�����ն���󴮿���

#define MAX_SERIAL_NUM 2
#define MAX_NET_NUM 1
#define MAX_SOCKET_NUM 1
#define MAX_COM_PORT_NUM MAX_SERIAL_NUM + MAX_NET_NUM*MAX_SOCKET_NUM

//�����ն���������
#ifndef MAX_LINE_NUM
#define MAX_LINE_NUM 1
#endif
#define MAX_SECTION_NUM 2 //������������
#define SEGMENT_NUM 3 //����ʽ����

#define ENABLE 1
#define DISENABLE 0


#define _0_HE_1_KAI 0
#define _1_HE_0_KAI 1
#define _1_KAI_0_HE 1 //�ն�Ĭ�ϵķֺ����ƽ��ϵ
#define _0_KAI_1_HE 0 //�ն�Ĭ�ϵķֺ����ƽ��ϵ


#define MAX_COUPLEYX_NUM 2
#define MAX_YK_NUM 2
//�����ն����ң����
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
	Uint16 priority;//���ȼ���
	Uint16 type;//��˫�㣻
}yxdata_t;
/*BL:Ƿѹ
 *AN:����ʧ��
 *BR:��ػ
 *BR:��ػ
 *OV����Դ����
 */
/* �ڱ���Ŀ��Hyx����Ϊ
0 	��λ		
1 	��λ		
2 	����λ		
3 	Զ���͵�λ	
4 	�ֶ���բ		
5 	�ֶ���բ		
6	VL 
7	POK
8	VH 
9	HOK 
10 ˫��ң��

VL	���Ƿѹ�澯
POK	����ʧ��澯
VH	��Դ���ϸ澯
HOK	�״̬
*/
typedef struct allyx{
	 yxdata_t pt_yx[MAX_LINE_NUM];
	 yxdata_t ct_yx[MAX_LINE_NUM];
	 yxdata_t Hyx[MAXHYXNUM];//ǰ�������û�ң�ţ���11����˫��ң�ţ�7(Ƿѹ),8��ʧ�磩,9����Դ�쳣��,10������ǵ�Դ����ң�š�
	 yxdata_t CurrentAlarmYx[MAX_LINE_NUM][3];//���ι����ź�
	 yxdata_t CurrentActionYx[MAX_LINE_NUM][3];//���ι��������ź�

	 yxdata_t GrdAlarmYx[MAX_LINE_NUM][3];//������������ź�
	 yxdata_t GrdActionYx[MAX_LINE_NUM][3];//����������������ź�

	 yxdata_t U0Alarm[MAX_LINE_NUM];//�����ѹ�����ź�(��ȡ���ñ������������ź�)
	 yxdata_t U0Action[MAX_LINE_NUM];//�����ѹ���������ź�(��ȡ���ñ������������ź�)

	 yxdata_t HjsActionYx[MAX_LINE_NUM];//����ٶ����ź�

	 yxdata_t ChzYx[MAX_LINE_NUM];//�غ�բ�����źš��������ɹ�/ʧ�ܣ�

	 yxdata_t JyYx[MAX_LINE_NUM];//���������Ծ�����ź�
	 yxdata_t JyActionYx[MAX_LINE_NUM];//���������Ծ�����ź�

	 yxdata_t JynegIYx[MAX_LINE_NUM];//���������Ծ�ź�
	 
	 yxdata_t P0Yx[MAX_LINE_NUM];//�����ʹ����ź�(��ȡ���ñ������������ź�)
	 yxdata_t P0ActionYx[MAX_LINE_NUM];//�����ʶ����ź�(��ȡ���ñ������������ź�)

	 yxdata_t JyP0Yx[MAX_LINE_NUM];//�����ʽ�Ծ�ź�(��ȡ���ñ������������ź�)
	 yxdata_t JyP0ActionYx[MAX_LINE_NUM];//�����ʽ�Ծ�����ź�(��ȡ���ñ������������ź�)

	 yxdata_t AngleYx[MAX_LINE_NUM];//��λ�����ź�
	 yxdata_t AngleActionYx[MAX_LINE_NUM];//��λ���������ź�

	 yxdata_t RaiseYx[MAX_LINE_NUM];//�������������ź�
	 yxdata_t RaiseActionYx[MAX_LINE_NUM];//�����������������ź�

	 yxdata_t gzfg[MAX_LINE_NUM][2];//���ϸ����źţ�0 ˲ʱ��1����
	 yxdata_t YueXianYx[MAX_LINE_NUM][10];//Խ���ź�
	 yxdata_t ActionCkYx[MAX_LINE_NUM];//����У���ź�
	 yxdata_t TerminalCkYx;//װ�ù����ź�
	 yxdata_t reverLineYx[MAX_LINE_NUM];//��·�����ź�
	 yxdata_t reverXuYx[MAX_LINE_NUM];//�����������ź�
	 
}allyx;
extern allyx TerYx;


typedef struct yx_table{
	//char *soe_p;
	yxdata_t *pdata;
}YX_TAB;
extern YX_TAB YxTable[TOTALYXNUM] ;//����POS�̶�˳������б�

typedef struct _sort_yx_table{
	yxdata_t *pdata;
}SORT_YX_TAB;
extern SORT_YX_TAB SortYxTable[TOTALYXNUM];// ���յ�ַ˳�����еı�

typedef struct limt{
    float Limit;
    float Recover;
    int32 ContinueTime;
}limt ;
typedef enum YXENUM{
	HE1 = 0,///<��·��1��λ
	FEN1,///<��·��1��λ
	CN1, ///<��·��1����λ��
	GLD1,///<��·��1���뵶բλ�ã�����ң��4��
	YX1_5,///<����ң��5��λ,�������ں�բ��ť
	YX1_6,//<����ң��6��λ���������ڷ�բ��ť
	XJGZ1,//����ָʾ��ָʾ��·2����·����
	JDGZ1,//����ָʾ��ָʾ��·2�ӵع���
	SP6GZ1,//SF6�����쳣�����ź�
	SP6BS1,//SF6��ѹ�����ź�
	HE2 ,///<��·��1��λ
	FEN2,///<��·��1��λ
	CN2, ///<��·��1����λ��
	GLD2,///<��·��1���뵶բλ�ã�����ң��4��
	YX25,///<����ң��5��λ
	YX26,//<����ң��6��λ
	XJGZ2,//����ָʾ��ָʾ��·2����·����
	JDGZ2, //����ָʾ��ָʾ��·2�ӵع���
	SP6GZ2,//SF6�����쳣�����ź�
	SP6BS2,//SF6��ѹ�����ź�
	YFJD   //Զ���ӵ�λ��
}YXENUM_E; //����21��
typedef enum YKENUM{
	BREAKER1 = 0, //��·��1ң��
	BREAKER1JHH,  //��·��1 ��ϻ�
	BREAKER2,     //��·��2ң��
	BREAKER2JHH,  //��·��2��ϻ�
	BATTARYACTION,//��ػң��
	BATTARYOFF,   //����˳�ң��
	MOTOR,	  //�������
}YKENUM_E;

#define HE(n) 	(HE1 + HE2*n)	///<��·��1��λ
#define FEN(n)	(FEN1 + HE2*n)	///<��·��1��λ
#define CN(n)	(CN1 + HE2*n)	///<��·��1����λ��
#define GLD(n)	(GLD1 + HE2*n)	///<��·��1���뵶բλ�ã�����ң��4��
#define BYX5(n)	(YX15 + HE2*n)	///<����ң��5��λ
#define BYX6(n)	(YX16 + HE2*n)	//<����ң��6��λ
#define XJGZ(n)	(XJGZ1 + HE2*n)	//����ָʾ��ָʾ��·2����·����
#define JDGZ(n)	(JDGZ1 + HE2*n)	//����ָʾ��ָʾ��·2�ӵع���
#define SP6GZ(n)(SP6GZ1 + HE2*n)	//SF6�����쳣�����ź�
#define SP6BS(n)(SP6BS1 + HE2*n)	//SF6��ѹ�����ź�

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
	short *Coef;//����ϵ����
	short *range;//����;
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
	uint16 ethnum;//��Ӧ�����ں�
	uint16 comtype;//ͨ����־��1 ��ͨ����0��ͨ����Ĭ����ͨ����
	int16 server_client;//1:�Ƿ�������2�ǿͻ��ˣ�Ĭ�Ͽͻ���
	int16 enable ;//ͨ�����ñ�־��
	int16 tcpiptype;//1:TCP;2:UDP Ĭ��tcp
	int16 net_port;//������������ʱ�ļ����˿ڣ�
	uint8 remote_ip[4];//Զ��IP
	int16 dport;//Զ�˶˿�
	int16 protocol;//��Լ
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
typedef struct terminal_para_tag {//�ն˲���

//	uint16 ter_addr;
//	uint16 master_addr;
	uint16 nulldata[8];//δ��������
	SERIAL_PARA ComPara[MAX_SERIAL_NUM];
	ETH_PARA EthPara[MAX_NET_NUM];
	ETH_COM_PARA EthSocketPara[MAX_SOCKET_NUM];
    uint32 yx_filter_time;//ң�ŷ���ʱ�䣻
    uint32 ykchecktime;//ң�ط�Уʱ�䣻
    uint32 yk_he_pulse;//ң�غ�����ʱ��
    uint32 yk_fen_pulse;//ң�ط�����ʱ�䣻
    uint32 ykdelaytime;//ң���ӳٶϿ���Դʱ�䡣
  //  unsigned int  ter_pswd[6];
    unsigned int  cascadeport;//CASCADE_PORT��
    BATTERYDATA_T battery_action_time ;
	sntpdata_t sntppara;
	uint16 show_data;//Һ����ʾһ��ֵ
	uint16 reverse[18];
	uint16 crc;
} TER_PARA;
extern TER_PARA TerPara;//�ն˲���

typedef struct time_data{
	 unsigned int millisecond;//0-999����
	 unsigned int minute;//IV--�Ƿ���Ч 0��Ч 1��Ч ռ�ֽ����λ ����λ���� ��6λΪ���ݣ�0-59��--��GB_T 18657.4-2002 Զ���豸��ϵͳ ���岿�� �����Լ ����ƪ Ӧ����ϢԪ�صı���Ͷ�������ͼ����д��0-99��
	 unsigned int hour;//SU--�Ƿ�����ʱ 0��׼ʱ�䣬1����ʱ ռ�ֽ����λ����������λ���ã���5λΪ���ݣ�0-23Сʱ��
	 unsigned int day;//day_week:weekռ�ֽڸ�3λ(1-7���ڵ���)��dayռ�ֽڵ�5λ ��1-31�µ��죩
	 unsigned int month;//����λ���� monthռ��4λ��1-12�£�
	 unsigned int year;//���λ���� yearռ��7λ��0-99�꣩

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
#define EVENTSOE 1 //˳��SOE
#define YKSOE    2   //ң��SOE
#define TERSOE 3  // �ն˹���SOE

#define PER_SOE_SIZE  (sizeof(SoeData))
#define PER_ACTION_SIZE  (sizeof(ActionData))
//������¼�ṹ
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
	uint16  type;//��˫�㣻
	CP56Time2a time;
	int txt_len; //txt len use for master  file transfer by ascii
}SoeData;
//soe �ļ�ͷ
typedef struct _SoeHeader{
	uint16 cur_index;//ָʾ��ǰsoe������
	uint16 total_cnt;//ָʾ��ǰ�ļ��������soe����
	uint16 total_txt_len;//ָʾ��ǰ�ļ������м�¼ת����ascii��ĳ���
	uint16 statu;//no use
	uint16 valid_flag[MAX_SOE_INFLASH];//ָʾ��ǰ��¼�Ƿ���Ч����Ϊsoe��¼������actsoe��¼�������ԣ�����ȡMAX_SOE_INFLASH
}SoeHeader;
typedef struct soe_data {
	Uint16 ReadFlag1[MAX_COM_PORT_NUM];//һ��
	Uint16 ReadFlag2[MAX_COM_PORT_NUM];//����
	Uint16 SaveFlag;//EVENTSOE  YKSOE TERCHECK
	SoeData soe;
} SOE_DATA;

typedef struct soe_buffer {
	 unsigned int  produce_cnt;//��������ָ�����ڶ������ݵ����ͣ�Ŀ����Ϊ�˱�֤ʱ��˳�򡣶���һ��������ʱ���ã�����һ�黺����
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
			unsigned int 	recv_time_out;//���ճ�ʱ
			unsigned int 	send_time_out;//���ͳ�ʱ
			unsigned int 	inbyte_cnt;//���ջ����������ݼ���
			unsigned int 	outbyte_cnt;//���ջ���ȡ�����ݼ���
			unsigned char short_pack_buffer[10];//�̶�֡����
			unsigned char long_pack_buffer[COM_RECV_BUFFER_SIZE];//�ɱ�֡����
			unsigned int 	short_pack_byte_cnt;//�̶�֡�ֽڼ���
			unsigned int    long_pack_byte_cnt;//�ɱ�֡�ֽڼ���
			unsigned int 	short_start_flag;//�̶�֡�����ʼ��־
			unsigned int 	long_start_flag;//�ɱ�֡�����ʼ��־
			unsigned int 	exp_len;//�ڴ���֡����
			unsigned int 	wait_com_flag;//�ȴ���־
			unsigned int 	wait_com_time;//�ȴ�ʱ��
			unsigned int	link_wait_flag;
			unsigned int	link_wait_time;//�ȴ���·���ӵ�ʱ��
			unsigned char	short_addr_buffer[10]; //�����Ϊ��ɽ��վ���ж��ն˵�ַ׼����֡��
			unsigned int	short_addr_byte_cnt;	//
			unsigned int	short_addr_flag;
			unsigned char 	com_send_buffer[COM_SEND_BUFFER_SIZE];//���ͻ���
			unsigned char 	com_recv_buffer[COM_RECV_BUFFER_SIZE];//���ջ���
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

typedef struct protocol_para_tag {  //��Լ����
	yxpara	yx_dataaddr ;
	ycpara	yc_dataaddr;
	uint16	yk_dataaddr[MAX_YK_NUM];//δ��
	ComPub	com_pub;
	uint16	yx_ack_type ;
	uint16     reverse[20];
	uint16     crc;
} PRO_PARA;

extern PRO_PARA ProPara;

typedef struct dead_band_para{
	int16 DeadTime;//ң������ʱ������
    float DeadU;   ///<��ѹ��������
    float DeadI;      ///<������������
    float DeadP;      ///<������������
    float DeadFreq;   ///<Ƶ����������
    float DeadAngle;///<�Ƕ���������//�͹�����������
    float DeadHarmoicValue;///<г����Чֵ��������
    float DeadHarmoicCount;///<   г����������������
    float DeadTemp;   ///<�¶���������
    float DeadHumidity;///<ʪ����������
    uint16 reverse[20];
    uint16 crc;
} DEAD_BAND_PARA;
extern DEAD_BAND_PARA DeadBandPara;
extern DEAD_BAND_PARA DeadBandParaValue;//ת���ɾ���ֵ

typedef struct unitary_table{
	int U_bdata;
	int I_bdata;
	int PQS_bdata;
	int COS_bdata;
	int F_bdata;
	int ANGLE_bdata;
	int DC_bdata;
	int TEMP_bdata;
}UNITARY_TAB;             //��һ��ϵ����

typedef struct angleratio{
	float angle;//�Ƕ����
	float cos_a;//�Ƕ���������ֵ
	float sin_a;//�Ƕ���������ֵ
}angleratio_t;
typedef struct ratio{
	float PortRatio[13];//UA1,UB1,UC1,UAB1,UBC1,UCA1,U01,IA11,IB11,IC11,I011,IA12,IB12,IC12,
	float PowerLineRatio[4];// Pa ; Pb ; Pc; Pall;  ����ϵ��
	angleratio_t PowerAngleRatio[2][4];//Qa ; Qb ;  Qc; Qall; �Ƕ�ϵ��
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

//����װ�ù�����Ϣλ
#define ERROR_FLASH 0
typedef struct _terminal_msg{//�ն���Ϣ
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
	uint32 	term_device_state;//װ�ù�����Ϣ
	yxdata_t *SoeId;
	int    soe_flag;
}TerminalMsg;
extern  TerminalMsg term_msg;
#define SETBIT(dest,mask)     (dest |= mask)//Ŀ��λ��λ
#define CLRBIT(dest,mask)     (dest &= ~mask)//Ŀ��λ����
#define TGLBIT(dest,mask)     (dest ^= mask)//Ŀ��λȡ��
#define CHKBIT(dest,mask)     (dest & mask)//Ŀ��λ���

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

extern float *ptr_pt;//������Һ�������в���ʹ���࣬���Ե���ʾһ��ֵ��Ҫʹ��pt��ctʱ��ֻ����ȫ�ֲ���
extern float *ptr_ct;
extern float *ptr_ct0;
extern float *ptr_pt0;
extern uint16 *ptr_d_yx;//˫��ң��
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
