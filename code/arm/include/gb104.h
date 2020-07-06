#ifndef _GB_104__H_
#define _GB_104__H_
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到基础类定义
//#include "user.h"
#include "stack.h"
#include "gb101.h"
#include "protocol.h"
#include "hisdata.h"
#include "debug.h"
#include "libxml/parser.h"
#pragma pack(1)

#define XML_PARA04_NUM 62

/*************************DEFINITION AND MICRO******************************************/
#define FRAME_ERR    -1
#define NO_ASDU      0
#define FRAME_OK     1
#define RECV_SEQ_ERR    2
#define TRANS_DISABLE     3
#define ASDU_ADDR_ERR   4

//定义默认规约关键字长度
#define FIX_FRAME_SIZE2 5
#define VAR_FRAME_SIZE2 250
#define LINK_ADDR_SIZE2 1
#define COMMON_UNIT_ADDR_SIZE2 1
#define SEND_CAUSE_SIZE2 1
#define DATASIGN_ADDR_SIZE2 2

#define NO_TYPE_FRAME   0x0
#define I_TYPE_FRAME   0x10
#define S_TYPE_FRAME   0x20
#define U_TYPE_FRAME   0x30

#define CONFIRM04     2
#define COMMAND04     1
#define CTRL_WORD_LEN 4


////////////////////////define the key word position in protocol of GuangDong 104 protocol/////////////////
#define APCI_FRAME_LEN   6
#define FIXED_MINUS04_LEN   2
#define CTRL_WORD04_LEN   4
#define DATA_LEN04_POS   1
#define CTRL_WORD04_POS   2
#define TYPE_SIGN04_POS   6
#define VSQ_WORD04_POS   7
#define SEND_CAUSE04_POS   8
#define ASDU_ADDR04_POS   10
#define DATA_ADDR04_POS   12


typedef struct I_ctrlword {

unsigned int Res1   :
    1;//=0

unsigned int SendSeq :
    15;

unsigned int Res2   :
    1;//=0

unsigned int RecvSeq :
    15;
} __attribute__ ( ( packed ) ) I_CTRLWORD;

typedef union Ictrlword {
    I_CTRLWORD I_CtrlWord;
    unsigned long data;
} __attribute__ ( ( packed ) ) ICTRLWORD;
///////////////////////////////////////

typedef struct S_ctrlword {

unsigned  int Res1   :
    16;//=1

unsigned  int Res2   :
    1; //=0

unsigned  int RecvSeq :
    15;
} __attribute__ ( ( packed ) ) S_CTRLWORD;

typedef union Sctrlword04 {
    S_CTRLWORD S_CtrlWord;
    unsigned  long data;
} __attribute__ ( ( packed ) ) SCTRLWORD04;
///////////////////////////////////////

typedef struct U_ctrlword {

unsigned  int Res1   :
    2;//=3

unsigned  int STARTDT :
    2;//1--confirm;2--command

unsigned  int STOPDT  :
    2;//1--confirm;2--command

unsigned  int TESTFR  :
    2;//1--confirm;2--command

unsigned  int Res2   :
    8;//=0

unsigned  int Res3   :
    16;//=0
} __attribute__ ( ( packed ) ) U_CTRLWORD;

typedef union Uctrlword {
    U_CTRLWORD U_CtrlWord;
    unsigned  long data;
} __attribute__ ( ( packed ) ) UCTRLWORD;

typedef struct _timeout104 {
    uint16 port_id;
    uint8 en;
    uint8 link;
    uint8 link_flag;
    uint8 rst_link;
    uint8 frame;
    uint8 frame_flag;
    uint8 byte;
    uint8 byte_flag;
    uint8 begin68;
    uint8 begin10;
    uint8 resend;
    uint8 resend_flag;
    uint8 resend_cnt;
    uint8 test_flag;
    uint8 test;
    uint8 test_cnt;
    uint8 yk_en;
    uint8 yk_cnt;
    uint8 yk_timeout_flag;
	uint8 btj_en;
    uint8 btj_cnt;
    uint8 btj_timeout_flag;
} __attribute__ ( ( packed ) ) PRO_TIMEOUT_4;


///////////////////定义104规约ASDU的关键字//////////////////////////////////////////
//应用服务数据单元基本格式
//数据单元标识符的结构定义
// ●一个八位位组            类型标识;
// ●一个八位位组            可变结构限定词;
// ●二个八位位组            传送原因;
// ●二个八位位组            应用服务数据单元公共地址;
// ●三个八位位组            信息对象地址。
// 一组信息元素集可以是单个信息元素/信息元素集合、单个信息元素序列或者信息元素集合序列。
//注∶类型标识定义了信息对象的结构、类型和格式。一个应用服务数据单元内全部信息对象有相同的结构、类型和格式。

typedef struct timer_cnt {
    uint16 cnt;
    uint8 flag;
} __attribute__ ( ( packed ) ) TIMER_CNT;

/////////////////////define the selectable timeout para///////////////////////////////////
//all the value are Recommended by Detailed rules of DL/T 634.5104-2002 of guangdong power grid
//Q/GD001.1138.2-2006

//the terminal act as a server,so the CONNECT_TIMEOUT is no use .
#define CONNECT_TIMEOUT     10 //unit:Second//T0

//counter begin send each APDU frame ,if no confirm frame comming after APDU_SEND_TIMEOUT,
//(the timeout equal to the test frame timeout),close the tcp conncet , reset the receive sequence and the send sequence,ack buffer.then
//open the tcp connect.
#define APDU_SEND_TIMEOUT    12//T1
///////////////////////////////////////////////////////
#define ASDU_CON_TIMEOUT    8//unit:Second//T2

//counter begin the last ASDU frame , after the NO_ASDU_CON_TIMEOUT ,if no follwo ASDU frames come
//send the S_frame to the counterpart.
#define NO_ASDU_CON_TIMEOUT    5 //unit:Second//T3

// if no data come and no data to send ,
//send the test frame after the IDLE_TIMEOUT from the last frame received,
#define IDLE_TIMEOUT     15 //unit:Second//T4

//counter begin send the test frame ,if no confirm frame comming after SEND_TEST_TIMEOUT,
//close the tcp conncet , reset the receive sequence and the send sequence,ack buffer.then
//open the tcp connect.
#define SEND_TEST_TIMEOUT    12 //unit:Second //T1
///////////////////////////////////////////////////////define the max num between send seqs and recv seqs/////////
//send apdu continous and haven't receive the response from counterpart ,
//if the send sequences minus the receive sequences no more than MAX_SEND_RECV_MATCH_NUM,the next APDU frame can be send,
//other wise ,stop the transfer.
#define MAX_SEND_RECV_MATCH_NUM   12//K

//receiving APDU continous , the max receptions is no more than MAX_RECV_APDU_SEND_CON_NUM,the confirm frame must be send
//to the counterpart;
#define MAX_RECV_APDU_SEND_CON_NUM 8//W

#define START 1
#define STOP 0
///////////////////////////////////////////////////////////////////////////////////


typedef struct pro104_inter_para{
	  int16 T0;///<连接建立的超时t0
      int16 T1;///<发送或测试 APDU 的超时t1
      int16 T2;///<无数据报文 t2<t1时确认的超时t2
      int16 T3;///<长期空闲 t3 >t1状态下发送测试帧的超时 t3
      int16 T4;///<应用报文确认超时t4
      int16 K;///<发送方最大发送未被确认的I格式报文数k
      int16 W;///<接收方最大接受未被确认I格式报文数w

} __attribute__ ( ( packed ) )INTER_PARA;

typedef struct pro_logic_buf04 {
    ICTRLWORD     ICtrlWord;     //I格式控制字
    SCTRLWORD     SCtrlWord;     //S格式控制字
    UCTRLWORD     UCtrlWord;     //U格式控制字
    uint16       CounterPartAck;    //对方收到的包数
    uint16      RecvSeqBack;
    uint16      SendSeqBack; //监控方向控制字
    TIMER_CNT no_asdu_con_timer;
    TIMER_CNT recv_asdu_con_timer;
    TIMER_CNT send_test_timer;
    TIMER_CNT link_idle_timer;
    PRO_LOGIC_BUF pro_logic_buf;
    SCTRLWORD04     CtrlWordBack;    //104中的101ASDU类型标识
    TIMER_CNT   send_asdu_con_timer;
    uint8       transfer_flag;
    uint8       enable_send_flag;
} __attribute__ ( ( packed ) ) PRO_LOGIC_BUF04;



typedef struct i_frame_map {
    uint8 head1;
    uint8 len1;
    uint8 len2;
    uint8 head2;
    uint8 ctrl_domain;
    uint16 link_addr;
    uint8 type_sign;
    uint8 qwd;
    uint8 reason;
    uint16 host_addr;
    uint8 data[240];
    uint8 acc;
    uint8 rear;
    uint8 datalen;
} __attribute__ ( ( packed ) ) i_frame_map_t;

typedef struct u_frame_map {
    uint8 head1;
    uint8 ctrl_domain;
    uint16 link_addr;
    uint8 acc;
    uint8 rear;
} __attribute__ ( ( packed ) ) u_frame_map_t;

typedef struct s_frame_map {
    uint8 head1;
    uint8 ctrl_domain;
    uint16 link_addr;
    uint8 acc;
    uint8 rear;
} __attribute__ ( ( packed ) ) s_frame_map_t;

#define DISABLE 0
#define ENABLE 1

#define MAX_PARSER04_FUNC_NUM 28
#define MAX_PACK04_FUNC_NUM 22
#define MAX_STEP_NUM 7
#define DATA_CLASS_TABLE_NUM 9
class Gb104
{

public:

    struct {
		prot_buf_t ProtBuf;
		INTRO_INFO introx_info[16][8];
        PRO_LOGIC_BUF04 pro_logic_buf04[MAX_PARSER_BUF_NUM];
        PRO_TIMEOUT_4 timeout104[MAX_PARSER_BUF_NUM];
        PRO_DATA_CLASS_CFG ProDataClassCfgTable[DATA_CLASS_TABLE_NUM];
        PRO_CFG pro_cfg;
        ID_MAP yx_id_map[MAXYXDATA];
        ID_MAP yc_id_map[MAXYCDATA];
        ID_MAP yk_id_map[MAXYKDATA];
        ID_MAP para_id_map[MAXPARADATA];
        ID_MAP bit32_id_map[MAX_BIT32_NUM];
		ID_MAP step_id_map[MAX_STEP_NUM];
        uint16 pro_yx_data_num;
        uint16 pro_dyx_data_num;
        uint16 pro_yc_data_num;
        uint16 pro_yk_data_num;
        uint16 pro_user_para_num;
        uint16 pro_user_bit32_num;
		uint16 pro_user_step_num;
        PARSER_FUNC parser_func[MAX_PARSER04_FUNC_NUM];
        PACK_FUNC pack_func[MAX_PACK04_FUNC_NUM];
        C1_DATA_BUF c1_data_buf;
        C2_DATA_BUF c2_data_buf;
        uint16 CurPortId;
        TERMINALSTATUS TerSt;
        DATA_GROUP_CFG data_group_cfg[6];
        PRO_DATA_TYPE_CFG pro_data_type_cfg;
        pack_buf_t pb;
        PRO_CLASS_DATA_TYPE_CFG class1_datatype_cfg;
        PRO_CLASS_DATA_TYPE_CFG class2_datatype_cfg;
		INTER_PARA InerPara;
		uint8 CurDataClass1Id;
        uint8 CurDataClass2Id;
        uint8 LastDataClass1Id;
        uint8 LastDataClass2Id;
		uint8 xml_para_cnt;
		uint8 InitOver;
		uint8 pc_mse_nx;
    }__attribute__ ( ( packed ) ) member;
	void SendFrame(BUF_MUX *m_buf,char *buf,uint16 len);
    char ScanSpontBuf ( uint16 port_id );
    Gb104 ( void );
    int WriteCfg ( uint32 para_id,char *para );
    int ReadCfg ( void );
    uint32 IsMyPara(uint32 paraid);
    char parser ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    //int16 AddBufRegistTable(BUF_MUX *buf);
    //int16 ClearBufRegistTable(void);
    char Parser ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    char Pack ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    char SetC1Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time );
    char GetC1Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag );
    char SetC2Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time );
    char GetC2Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag );

    uint16  DealComBuf ( BUF_MUX *buf, uint16 port_id );

    char GetKeyWordPositionAndLen ( uint8 keyword_id, uint8 *pos, uint8 *len );
		char GetServiceId ( uint16 port_id, uint8 type, uint8 *s_id );
    char ConfigTimeOut (uint16 port_id );
    PRO_TIMEOUT_4* SelectTimeOut ( uint16 port_id );

    char ConfigLogicBuf ( uint16 port_id );
    PRO_LOGIC_BUF04* SelectLogicBuf ( uint16 port_id );

    char DealTimeOut ( BUF_MUX *buf, uint16 port_id );

    int ParaConfig ( xmlNodePtr node );
    int ParaCheck ( void );
    ID_MAP* GetDataId ( uint32 dataaddr );
    ID_MAP* GetDataAddr ( uint16 dataid );
    char CheckFrame ( BUF_MUX *buf, uint16 port_id );

    //PRO_ACK_DATA_PRI pro_data_type_pri;
    void InitGb104(void);

    PARSER_FUNC * SearchParserFuncTable ( uint8 type );
    PACK_FUNC * SearchPackFuncTable ( uint8 type );
    char InitFunc ( void );
	char InitIdMapTable ( void );
    char SortYxIdMap ( void );
    char SortYcIdMap ( void );
    char SortYkIdMap ( void );
    void ProTimerCount ( uint16 pn );
    void ProTimerReset ( uint16 pn );
    void ProLinkReset ( uint16 pn );
	PRO_DATA_CLASS_CFG* SearchClassTable ( uint8 data_type );
	char AddServiceBuf ( uint16 port_id, uint8 type, uint8 s_id );
	void ConfigGroupData ( DATA_GROUP_CFG *pgf, INTRO_INFO ii[16][8], ID_MAP *pmap, uint16 data_num, uint8 data_type );
	void InitGroup ( void );
	
};
extern char deal_104(BUF_MUX *buf,uint16 port_id);

#endif

