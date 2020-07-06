#ifndef _GB_101__H_
#define _GB_101__H_
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到基础类定义
#include "protocol.h"
#include "hisdata.h"
#include "libxml/parser.h"
#include "custom.h"
#define XML_PARA_NUM 54
#pragma pack(1)


#define INVALID_PARA 0
//define data class
#define CLASS1 1
#define CLASS2 2
#define INIT_OVER_DATA 1	//初始化结束 
#define CHANGE_YX_DATA 2	//遥信变位
#define CTRL_RET_DATA 3		//遥控命令的镜像报文
#define CHANGE_YC_DATA 4	//遥测变化
#define TIME_TB_DATA 5		//时钟同步的镜像报文
#define SOE_DATA 6			//变位事件顺序记录（SOE）
#define ACK_STATION_ASK_DATA 7	//回答总召唤数据
#define ACK_GROUP_ASK_DATA 8	//组召唤回答
#define STEP_RET_DATA 9			//步进命令的镜像报文
#define VERIFY_RET_DATA 10


#define CHANGE_DYX_DATA 0x22
#define SOE2_DATA 0x26

#define GROUP_YC_DATA 2
#define GROUP_YX_DATA 1
#define GROUP_STEP_DATA 3
#define GROUP_BIT32_DATA 4

//////////define time tag

#define CP56 7
#define CP24 3
#define CP00 0
////////////////////////////define func code
//启动方向的功能码
#define M_Reset_Remote_Link 0
#define M_Reset_User_Process 1
#define M_User_Data_Confirm 3
#define M_User_No_Response 4
#define M_Access_Request 8
#define M_Request_Link_St 9
#define M_Request_Class1_Data 10
#define M_Request_Class2_Data 11
//从动方向的功能吗

#define S_ACK 0
#define S_NACK 1
#define S_ACK_Link_St 11
#define S_ACK_User_Data 8
#define S_NACK_User_Data 9



#define SET_TIME 1
#define GET_TIME 2
#define SET_DATA 3
#define GET_DATA 4
#define SET_PARA 5
#define GET_PARA 6

#define READ_FILE 7
#define WRITE_FILE 8
#define RESET_TER 9
#define RESET_LINK 10
#define GET_LINK_STATUS 11
#define YK_APP 12
#define ADJUST_APP 13
#define ACTIVE_PARA 14
#define READ_FILE_INFO 15

//#include "user.h"
#define M_SP_NA_1  1//>∶＝单点信息
#define M_SP_TA_1  2//>∶＝带时标的单点信息
#define M_DP_NA_1  3//>∶＝双点信息
#define M_DP_TA_1  4//>∶＝带时标的双点信息
#define M_ST_NA_1  5//>∶＝步位置信息
#define M_ST_TA_1  6//>∶＝带时标的步位置信息
#define M_BO_NA_1  7//>∶＝32比特单
#define M_BO_TA_1  8//>∶＝带时标的32比特串
#define M_ME_NA_1  9//>∶＝测量值，规一化值
#define M_ME_TA_1  10//>∶＝带时标的测量值,规一化值
#define M_ME_NB_1  11//>∶＝测量值，标度化值
#define M_ME_TB_1  12//>∶＝带时标的测量值，标度化值
#define M_ME_NC_1  13//>∶＝测量值，短浮点数
#define M_ME_TC_1  14//>∶＝带时标的测量值，短浮点数
#define M_IT_NA_1  15//>∶＝累计量
#define M_IT_TA_1  16//>∶＝带时标的累计量
#define M_EP_TA_1  17//>∶＝带时标的继电保护设备事件
#define M_EP_TB_1  18//>∶＝带时标的继电保护设备成组启动事件
#define M_EP_TC_1  19//>∶＝带时标的继电保护设备成组输出电路信息
#define M_PS_NA_1  20//>∶＝带变位检出的成组单点信息
#define M_ME_ND_1  21//>∶＝测量值，不带品质描述词的规一化值
#define M_SP_TB_1  30//>∶＝带CP56Time2a时标的单点信息
#define M_DP_TB_1  31//>∶＝带CP56Time2a时标的双点信息
#define M_ST_TB_1  32//>∶＝带CP56Time2a时标的步位置信息
#define M_BO_TB_1  33//>∶＝带CP56Time2a时标的32比特串
#define M_ME_TD_1  34//>∶＝带CP56Time2a时标的测量值，规一化值
#define M_ME_TE_1  35//>∶＝带CP56Time2a时标的测量值，标度化值
#define M_ME_TF_1  36//>∶＝带CP56Time2a时标的测量值，短浮点数
#define M_IT_TB_1  37//>∶＝带CP56Time2a时标的累计量
#define M_EP_TD_1  38//>∶＝带CP56Time2a时标的继电保护设备事件
#define M_EP_TE_1  39//>∶＝带CP56Time2a时标的继电保护设备成组启动事件
#define M_EP_TF_1  40//>∶＝带CP56Time2a时标的继电保护装置成组输出电路信息
// 控制方向的过程信息
#define C_SC_NA_1  45//>∶＝单点命令
#define C_DC_NA_1  46//>∶＝双点命令
#define C_RC_NA_1  47//>∶＝步调节命令
#define C_SE_NA_1  48//（直接控制命令）  48//>∶＝设定值命令、规一化值
#define C_SE_NB_1  49//>∶＝设定值命令、标度化值
#define C_SE_NC_1  50//>∶＝设定值命令，短浮点数
#define C_BO_NC_1  51//>∶＝32比特串

/////////////////////////////////////
#define C_SC_TA_1  58//>∶＝单点命令,带时标
#define C_DC_TA_1  59//>∶＝双点命令带时标
#define C_RC_TA_1  60//>∶＝步调节命令带时标
#define C_SE_TA_1  61//（直接控制命令） ＝设定值命令、规一化值带时标
#define C_SE_TB_1  62//>∶＝设定值命令、标度化值带时标
#define C_SE_TC_1  63//>∶＝设定值命令，短浮点数带时标
#define C_BO_TC_1  64//>∶＝32比特串带时标


//#define C_BO_NC_1  150//>∶＝比特串
// 在监视方向的系统命令（站特定参数）
#define M_EI_NA_1  70//>∶＝初始化结束
// 在控制方向的系统命令（站特定参数）
#define C_IC_NA_1  100//>∶＝总召唤命令（包括分组召唤）
#define C_CI_NA_1  101//>∶＝累计量召唤命令）
#define C_RD_NA_1  102//>∶＝读命令
#define C_CS_NA_1  103//>∶＝时钟同步命令
#define C_TS_NA_1  104//>∶＝测试命令
#define C_RP_NA_1  105//>∶＝复位进程命令
#define C_CD_NA_1  106//>∶＝延时获得命令
// 在控制方向的参数命令（站特定参数）
#define P_ME_NA_1  110//>∶＝测量值参数，规一化值
#define P_ME_NB_1  111//>∶＝测量值参数，标度化值
#define P_ME_NC_1  112//>∶＝测量值参数，短浮点数
#define P_AC_NA_1  113//>∶＝参数激活
// 文件传输（站特定参数）
#define F_FR_NA_1  120//>∶＝文件准备就绪
#define F_SR_NA_1  121//>∶＝节准备就绪
#define F_SC_NA_1  122//>∶＝召唤目录，选择文件，召唤文件召唤节
#define F_LS_NA_1  123//>∶＝最后的节，最后的段
#define F_AF_NA_1  124//>∶＝认可文件，认可节
#define F_SG_NA_1  125//>∶＝段
#define F_DR_TA_1  126//>∶＝目录
#define P_XX_XX_1  222//>: ==扩展终端参数设置，能够设置参数块
//宏定义传送原因中的CODE
#define CAUSE_Per_Cyc      1//周期,循环
#define CAUSE_Back    2//背景扫描
#define CAUSE_Spont    3//突发(自发)
#define CAUSE_Init    4//初始化
#define CAUSE_Req    5//请求或被请求
#define CAUSE_Act    6//激活
#define CAUSE_Actcon   7//激活确认
#define CAUSE_Deact    8//停止激活
#define CAUSE_Deactcon   9//停止激活确认
#define CAUSE_Actterm   10//激活终止
#define CAUSE_Retrem   11//远方命令引起的反送信息
#define CAUSE_Retloc   12//当地命令引起的反送信息
#define CAUSE_File    13//文件传输
/*
#define PRESERVATION   14
#define PRESERVATION   15
#define PRESERVATION   16
#define PRESERVATION   17
#define PRESERVATION   18
#define PRESERVATION   19
*/
#define CAUSE_Introgen   20//相应总召唤
#define CAUSE_Intro1   21//相应第一组召唤
#define CAUSE_Intro2   22//相应第二组召唤
#define CAUSE_Intro3   23//相应第三组召唤
#define CAUSE_Intro4   24//相应第四组召唤
#define CAUSE_Intro5   25//相应第五组召唤
#define CAUSE_Intro6   26//相应第六组召唤
#define CAUSE_Intro7   27//相应第七组召唤
#define CAUSE_Intro8   28//相应第八组召唤
#define CAUSE_Intro9   29//相应第九组召唤
#define CAUSE_Intro10   30//相应第10组召唤
#define CAUSE_Intro11   31//相应第11组召唤
#define CAUSE_Intro12   32//相应第12组召唤
#define CAUSE_Intro13   33//相应第13组召唤
#define CAUSE_Intro14   34//相应第14组召唤
#define CAUSE_Intro15   35//相应第15组召唤
#define CAUSE_Intro16   36//相应第16组召唤
#define CAUSE_Reqcogen   37//相应累积量总召唤
#define CAUSE_Reqco1   38//相应第一组累积量总召唤
#define CAUSE_Reqco2   39//相应第二组累积量总召唤
#define CAUSE_Reqco3   40//相应第三组累积量总召唤
#define CAUSE_Reqco4   41//相应第四组累积量总召唤

/*
#define PRESERVATION   42
#define PRESERVATION   43
*/
#define CAUSE_Unknowntype  44
#define CAUSE_Unknowncause  45
#define CAUSE_Unknownmasteraddr  46
#define CAUSE_Unknowndataaddr  47
/*
#define PRESERVATION   48~63
*/

/*文件限定词宏定义
*/
#define SelectFile   1
#define AskFile    2
#define DeactFile   3
#define DeleteFile   4
#define SelectSection  5
#define AskSection   6
#define DeactSection  7

#define NoAskRoom   1
#define VerifyError   2
#define UnexpectServer   3
#define UnexpectFileName 4
#define UnexpectSectionName 5
#define SelectFile 1

//遥控命令限定词宏定义
#define OPEN    1
#define CLOSE    2
#define SHORTPULSE   1
#define LONGPULSE   2
#define ContinuanceOutput  3


//定义默认规约关键字长度
#define FIX_FRAME_SIZE 5
#define VAR_FRAME_SIZE 250
#define LINK_ADDR_SIZE 1
#define COMMON_UNIT_ADDR_SIZE 1
#define SEND_CAUSE_SIZE 1
#define DATASIGN_ADDR_SIZE 2

typedef struct _timeout101 {
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
	uint8 res1;
}__attribute__ ( ( packed ) ) PRO_TIMEOUT;

#define MAX_PACK_BUF_SIZE 512

typedef struct pack_buf {
    uint16 data_num;
    char buf[MAX_PACK_BUF_SIZE];
    uint16 bytes;
} __attribute__ ( ( packed ) ) pack_buf_t;

//规约时标定义
/*typedef struct time_data{
  uint16 millisecond;
  uint8 minute;
  uint8 hour;
  uint8 day;
  uint8 month;
  uint8 year;
} CP56Time2a;
*/
//规约帧关键字长度参数定义
#define EQ_TRANS 1
#define UNEQ_TRANS 2

typedef struct pro_cfg {
    uint8 link_addr_size;//1,2
    uint8 common_unit_addr_size;//1,2
    uint8 send_cause_size;//1,2
    uint8 datasign_addr_size;//2,3
    uint8 trans_style; // 1平衡2非平衡
    uint8 enable_interrupt_process;//是否允许中断前一个流程
    uint8 time_tag;
	uint8 resend_times;
}__attribute__ ( ( packed ) ) PRO_CFG;

//上送三类数据的类型标识参数定义总召唤

typedef struct pro_ta_data_type_cfg {

    uint8 syx_type;
    uint8 dyx_type;
    uint8 yc_type;
    uint8 yk_type;

} __attribute__ ( ( packed ) ) PRO_DATA_TYPE_CFG;

//上送三类数据的类型标识参数定义1、2级数据

typedef struct pro_ca_data_type_cfg {

    uint8 syx_type;
    uint8 dyx_type;
    uint8 yc_type;
    uint8 yk_type;
} __attribute__ ( ( packed ) ) PRO_CLASS_DATA_TYPE_CFG;


//////////////////1、2级数据配置表

typedef struct pro_data_class_cfg_t {
    uint8 type;
    uint8 grade;
    uint8 pri;
	uint8 res1;
    uint8 ( *fp ) ( char *out_buf, uint16 *out_len );
} __attribute__ ( ( packed ) ) PRO_DATA_CLASS_CFG;

//规约解析的类型与功能函数表

//数据组配置表
/*
@data_type :indicate data type in this group .it defined like M_SP_NA_1...
@start_group_id:indicate group id
@max_group_num:indicate how many group maxium
@used_group_num:indicate how many  group actual.
@group_data_num:indicate how many data in this group 
*/

typedef struct data_group_cfg_t {
    uint8 data_type;
    uint8 start_group_id;
    uint8 max_group_num;
    uint8 used_group_num;
    uint16 group_data_num;
}__attribute__ ( ( packed ) ) DATA_GROUP_CFG;

typedef struct parser_func {
    uint8 type;
	uint8 res1;
    char ( *fp ) ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    uint8 func_id;
	uint8 res2;
    uint8 ( *pcb ) ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
} __attribute__ ( ( packed ) ) PARSER_FUNC;

//规约打包功能函数表

typedef struct pack_func {
    uint8 type;
	uint8 res1;
    char ( *fp ) ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
} __attribute__ ( ( packed ) ) PACK_FUNC;

//规约数据项标识与数据ID 映射表定义

typedef struct id_map {
    uint32 data_addr;
    uint16 data_id;
    uint8 bytes;
    uint8 type;
}__attribute__ ( ( packed ) ) ID_MAP;

#define MAX_PROT_BUF_SIZE 512

typedef struct prot_buf {
    uint16 parser_frame_len;
    uint16 pack_frame_len;
    uint16 valid_flag;//
    char pack_buf[MAX_PROT_BUF_SIZE];
    char parser_buf[MAX_PROT_BUF_SIZE];
	
} __attribute__ ( ( packed ) ) prot_buf_t;

#define PRO_READ 1
#define PRO_WRITE 2
#define PRO_NULL 0

#define PRO_PACK_MIRROR_FRAME 0x10
#define PRO_PACK_SHORT_FRAME 0x20
#define PRO_PACK_NULL 0


typedef struct terminal_status {

uint32 cpu1_com_status:
    1;//ram_status:1;     //0位，0－正常

uint32 cpu2_com_status:
    1;//command_status:1;    //1//0－未取消

uint32 cpu3_com_status:
    1;//:15;        //16

uint32 sd_card_status:
    1;//ups_status:1;     //17//0－正常

uint32 :
    15;        //18

uint32 remote_local_control:
    1;   //19//0－远方控

uint32 on_duty:
    1;      //20//0－无人值班

uint32 self_energy_switch:
    1;   //21//0－合上

uint32 :
    3;        //24

uint32 sys_restart:
    1;     //25//0－正常//1－启动

uint32 cold_start_up:
    1;     //26//0－未启动

uint32 rom_status:
    1;     //27//0－正常

uint32 flash_status:
    1;     //28//0－正常

uint32 cpu_status_under_power_fault:
    1; //29//0－正常

uint32 cpu_status_under_disturb:
    1;  //30//0－正常

uint32 power_fault:
    1;     //31//0－正常
}__attribute__ ( ( packed ) ) TERMINAL_STATUS;

typedef struct qos_word {

uint8 QL:
    7;

uint8 S_E:
    1;
} __attribute__ ( ( packed ) ) QOS_WORD;

typedef union qoword {
    QOS_WORD qosword;
    uint8 data;
} __attribute__ ( ( packed ) ) QOSWORD;
//////////////////////////定义遥信遥测的品质描述词////////////

typedef struct qds_word {

uint8 OV:
    1;//1--overflow

uint8 RES:
    3;//reserved

uint8 BL:
    1;//0--unlock

uint8 SB:
    1;//0--uncover

uint8 NT:
    1;//0--current value

uint8 IV:
    1;//0--valid
} __attribute__ ( ( packed ) ) QDS_WORD;

typedef union qsword {
    QDS_WORD qdsword;
    uint8 data;
} __attribute__ ( ( packed ) ) QDSWORD;
///////////////////状态和状态变位检出///////////////

typedef struct bit_st {

uint16 st0:
    1;//0--open;1--close

uint16 st1:
    1;

uint16 st2:
    1;

uint16 st3:
    1;

uint16 st4:
    1;

uint16 st5:
    1;

uint16 st6:
    1;

uint16 st7:
    1;

uint16 st8:
    1;

uint16 st9:
    1;

uint16 st10:
    1;

uint16 st11:
    1;

uint16 st12:
    1;

uint16 st13:
    1;

uint16 st14:
    1;

uint16 st15:
    1;
}__attribute__ ( ( packed ) ) BIT_ST;

typedef union st {
    BIT_ST BitSt;
    uint16 data;
}__attribute__ ( ( packed ) ) ST;

typedef struct bit_cd {

uint16 cd0:
    1;

uint16 cd1:
    1;

uint16 cd2:
    1;

uint16 cd3:
    1;

uint16 cd4:
    1;

uint16 cd5:
    1;

uint16 cd6:
    1;

uint16 cd7:
    1;

uint16 cd8:
    1;

uint16 cd9:
    1;

uint16 cd10:
    1;

uint16 cd11:
    1;

uint16 cd12:
    1;

uint16 cd13:
    1;

uint16 cd14:
    1;

uint16 cd15:
    1;
}__attribute__ ( ( packed ) ) BIT_CD;

typedef union cd {
    BIT_CD BitCd;
    uint16 data;
} __attribute__ ( ( packed ) ) CD;

typedef struct scd {
    ST stword;
    CD cdword;
    QDSWORD qdsword;
}__attribute__ ( ( packed ) ) SCD;

///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

typedef struct bs1_ctrlword { //

uint8 FuncCode:
    4; //功能码

uint8 FCV:
    1;  //帧计数有效位

uint8 FCB:
    1;  //帧计数位

uint8 PRM:
    1;  //启动报文位

uint8 DIR:
    1;  //传输方向位
} __attribute__ ( ( packed ) ) BS1_CTRLWORD;

typedef union Mctrlword {
    BS1_CTRLWORD cword;
    uint8 data;
} __attribute__ ( ( packed ) ) MCTRLWORD;

//子站-->主站

typedef struct bs2_ctrlword {

uint8 FuncCode:
    4; //功能码

uint8 DFC:
    1;  //数据流控制位

uint8 ACD:
    1;  //要求访问位

uint8 PRM:
    1;  //启动报文位

uint8 DIR:
    1;  //传输方向位
} __attribute__ ( ( packed ) ) BS2_CTRLWORD;

typedef union Sctrlword {
    BS2_CTRLWORD cword;
    uint8 data;
} __attribute__ ( ( packed ) ) SCTRLWORD;
//定义终端初始化原因限定词

typedef struct coi_data {

uint8 UI:
    7;

uint8 BS1:
    1;
}__attribute__ ( ( packed ) ) COI_DATA;

typedef union coidata {
    COI_DATA CoiData;
    uint8 data;
} __attribute__ ( ( packed ) ) COIDATA;
//可变结构限定词定义
//SQ=:0 Number 是信息对像的个数
//SQ=:1 Number 是每个应用服务数据单元中单个对象的信息元素或者信息元素的集合的数目

typedef struct bs_vsq {

uint8 Number:
    7;

uint8 SQ:
    1;
} __attribute__ ( ( packed ) ) BS_VSQ;

typedef union bsvsq {
    BS_VSQ BsVsq;
    uint8 data;
} __attribute__ ( ( packed ) ) BSVSQ;
//定义传送原因

typedef struct bs_cause {

uint16 code:
    6;

uint16 P_N:
    1;

uint16 T:
    1;//    //uint16 original_addr:8;

uint16 res:
    8;
} __attribute__ ( ( packed ) ) BS_CAUSE;

typedef union bscause {
    BS_CAUSE BsCause;
    uint16 data;
} __attribute__ ( ( packed ) ) BSCAUSE;
//定义设定参数限定词

typedef struct qpm_data {

uint8 KPA:
    6;//1～4：门限值，平滑系数，下限，上限

uint8 LPC:
    1;//0－－未改变，1－－改变

uint8 POP:
    1;//0－－运行，1－－未运行
} __attribute__ ( ( packed ) ) QPM_WORD;

typedef union qpmdata {
    QPM_WORD QmpWord;
    uint8 data;
} __attribute__ ( ( packed ) ) QPMWORD;
//定义选择和召唤限定词

typedef struct scq_data {

uint8 UI1:
    4;//1～7：选择文件，请求文件，停止激活文件，删除文件，选择节，请求节，停止激活节

uint8 UI2:
    4;//同afq说明
} SCQ_WORD;

typedef union scqdata {
    SCQ_WORD ScqWord;
    uint8 data;
} __attribute__ ( ( packed ) ) SCQWORD;
//定义文件认可或节认可限定词
//定义文件认可或节认


typedef struct afq_data {

uint8 UI1:
    4;//1--文件肯定认可,2--文件否定认可3--节肯定认可，4--节否定认可

uint8 UI2:
    4;//1--无所请求的存储空间,2--校验和错,3--非所期望的通信服务,4--文件名错，5－－节名错
} __attribute__ ( ( packed ) ) AFQ_WORD;

typedef union afqdata {
    AFQ_WORD AfqWord;
    uint8 data;
} __attribute__ ( ( packed ) ) AFQWORD;
//定义文件状态限定词

typedef struct sof_data {

uint8  STATUS:
    5;

uint8  LFD:
    1;//0－－后面还有目录文件，1－－最后目录文件

uint8  _FOR:
    1;//0－－定义文件名，1－－定义子目录名

uint8  FA:
    1;//1－－文件传输已激活，0－－文件等待传输
} __attribute__ ( ( packed ) ) SOF_WORD;

typedef union sofdata {
    SOF_WORD SofWord;
    uint8 data;
} __attribute__ ( ( packed ) ) SOFWORD;
//文件准备就绪限定词

typedef struct frq_data {

uint8  UI:
    7;

uint8  BS:
    1;
} __attribute__ ( ( packed ) ) FRQ_WORD;

typedef union frqdata {
    FRQ_WORD FrqWord;
    uint8 data;
}__attribute__ ( ( packed ) ) FRQWORD;
//节准备就绪限定词

typedef struct srq_data {

uint8  UI:
    7;

uint8  BS:
    1;
} __attribute__ ( ( packed ) ) SRQ_WORD;

typedef union srqdata {
    SRQ_WORD SrqWord;
    uint8 data;
} __attribute__ ( ( packed ) ) SRQWORD;


//定义遥控命令限定词

typedef struct dco_data {

uint8  DCS:
    2; //1－－开，2－－合

uint8  QU:
    5; //1--短脉冲，2--长脉冲，3－－持续输出

uint8  S_E:
    1; //1--选择，0－－执行
} __attribute__ ( ( packed ) ) DCO_WORD;

typedef union dcodata {
    DCO_WORD DcoWord;
    uint8 data;
} __attribute__ ( ( packed ) ) DCOWORD;

//定义步调节命令限定词

typedef struct rco_data {

uint8  RCS:
    2; //1－－down one step，2－－up one step

uint8  QU:
    5; //1--短脉冲，2--长脉冲，3－－持续输出

uint8  S_E:
    1; //1--选择，0－－执行
} __attribute__ ( ( packed ) ) RCO_WORD;

typedef union rcodata {
    RCO_WORD RcoWord;
    uint8 data;
} __attribute__ ( ( packed ) ) RCOWORD;

typedef struct diq_data {

uint8  DPI:
    2; //1－－开，2－－合 其 他状态不确定

uint8  RES:
    2; //未定义位

uint8  BL:
    1; //1--闭锁，0－－未被闭锁

uint8  SB:
    1; //1--被取代，0－－未被取代

uint8  NT:
    1; //1--非当前值，0－－当前值

uint8  IV:
    1; //1--无效，0－－有效
} __attribute__ ( ( packed ) ) DIQ_WORD;

typedef union diqdata {
    DIQ_WORD DiqWord;
    uint8 data;
} __attribute__ ( ( packed ) ) DIQWORD;

typedef struct  single_point_data {//单点信息（遥信和SOE）

uint8  SPI:
    1; //开合

uint8  res:
    3;   //保留

uint8  BL:
    1; //闭锁

uint8  SB:
    1; //取代

uint8  NT:
    1; //当前

uint8  IV:
    1; //有效
} __attribute__ ( ( packed ) ) SP_DATA;

typedef union  singlepointdata {//单点信息（遥信和SOE）
    SP_DATA SpData;
    uint8 data;
} __attribute__ ( ( packed ) ) SPDATA;

typedef struct double_point_data {//双点信息（遥控执行结果）

uint8  DPI:
    2;

uint8  res :
    2;

uint8   BL:
    1;

uint8   SB:
    1;

uint8   NT:
    1;

uint8   IV:
    1;
}__attribute__ ( ( packed ) ) DP_DATA;

typedef union doublepointdata {//双点信息（遥控执行结果）
    DP_DATA DaData;
    uint8 data;
} __attribute__ ( ( packed ) ) DPDATA;


typedef struct ter_status {
    unsigned long data;
    TERMINAL_STATUS status;
} __attribute__ ( ( packed ) ) TERMINALSTATUS;

#define MAX_SERVICE_ID 16

typedef struct service_id_buf {
    uint8 id[MAX_SERVICE_ID];
    uint8 flag[MAX_SERVICE_ID];
    uint8 in_cnt;
    uint8 out_cnt;
}__attribute__ ( ( packed ) ) SERVICE_ID_BUF;

#define SECT_MAXSEGNUM 4
#define MAX_SEGSIZE 230
#define FILE_FILL_BYTE 0xAA

typedef struct  file_info  {
    uint32 file_len;//文件长度
    uint16 file_seg_num;// 文件段数量
    uint16 file_name;    //文件名
    uint8 file_section_num;   //文件中节数量
    uint8 last_sect_seg_num;//最后一节段的数量
    uint8 sect_segment_num;   //节中段数量
    uint8 segment_size;   //段的大小
    uint8 last_segment_size;//最后段的大?
    uint8 res1;
}  __attribute__ ( ( packed ) ) FILE_INFO ;

typedef struct pro_logic_buf {
    uint16 port_id;
    MCTRLWORD CtrlWordFromMaster;
    SCTRLWORD CtrlWordFromSelf;   //监控方向控制字
    uint8   QOI;     //召唤限定词
    COIDATA    CauseOfInit;    //子站初始化原因
    BSVSQ      VSQ_Word;     //可变结构限定词
    BSCAUSE    SendCause;     //发送传送原因
    BSCAUSE    M_SendCause;//主站发来的传送原因
    QPMWORD    QPM_Word;   //参数设置限定词
    QOSWORD    QOS_Word;   //设定命令限定词
    uint16   linkaddr;//链路地址
    uint16   hostaddr;//主站地址
    uint8   M_type;//主站发来的命令
    uint8   S_type;//终端响应的命令
    SCQWORD    SCQ_Word;       //文件召唤限定词
    uint8     LSQ;         //最后的节和段限定词
    AFQWORD    AFQ_Word;       //文件认可或节认可限定词
    DCOWORD     DCO_Word;       //遥控命令限定词
    RCOWORD     RCO_Word;   //步调节命令限定词
    DIQWORD     DIQ_Word;       //双点信息品质描述词
    SOFWORD    SOF_Word;       //文件状态限定词
    SRQWORD    SRQ_Word;       //节准备就绪限定词
    FRQWORD    FRQ_Word;       //文件准备就绪限定词
    uint8 total_data_send_over;
    uint8 intro_col;
    uint8 intro_row;
    uint8 total_ask_begin;
    uint32 yk_datasign;
	uint32 btj_datasign;
    FILE_INFO FileInfo;
    uint8 file_section_cnt;
    uint8 file_segment_cnt;
    uint8 SectionCheckSum;      //节数据校验和
    uint8 FileCheckSum;      //文件校验和
    uint8 connect_st;
    uint8 confirm_flag;
    uint8 yk_st;
    uint8 yk_cmd;
	uint8 btj_st;
    uint8 btj_cmd;

    SERVICE_ID_BUF Service1Buf, Service2Buf;
} __attribute__ ( ( packed ) ) PRO_LOGIC_BUF;

#define MAX_PARSER_BUF_NUM 4
#define EVENT_READ 1
#define EVENT_NOREAD 2

typedef struct classx_data {
    uint16 port_id;
    uint32 dataid;
    char data[5];
    char datasize;
    char read_flag;
    char time[7];
    uint8 data_type;
	uint8 res1;
}__attribute__ ( ( packed ) ) CX_DATA;

#define MAX_C1_EVENT_NUM 100
#define MAX_C2_EVENT_NUM 200

typedef struct class1_data_buf {
    CX_DATA c1_data[MAX_C1_EVENT_NUM];
    uint8 in_cnt;
    uint8 out_cnt;

}__attribute__ ( ( packed ) ) C1_DATA_BUF;


typedef struct class2_data_buf {
    CX_DATA c2_data[MAX_C2_EVENT_NUM];
    uint8 in_cnt;
    uint8 out_cnt;

}__attribute__ ( ( packed ) ) C2_DATA_BUF;




#define APP_HANDLE 1
#define LINK_HANDLE 2


#define MAX_BIT32_NUM 16


//////////////define the protocol key word id for checking buf/////////////////////
#define KEY_WORD_LINKADDR 1
#define KEY_WORD_CTRLWORD 2
#define KEY_WORD_TYPE 3
#define KEY_WORD_VSQ 4
#define KEY_WORD_SENDCAUSE 5
#define KEY_WORD_COMADDR 6
#define KEY_WORD_DATA_START 7
#define KEY_WORD_COMADDR 6

typedef struct total_ask_counter_t {

    uint8 group_cnt;
    uint8 frame_cnt;
    uint16 data_cnt;

} __attribute__ ( ( packed ) ) TOTAL_ASK_COUNTER;
/*
@start_addr 该组的起始地址
@num 本组数据数量
@flag 本组是否有效
@typesign 本组的数据类型
@size 本组内每个数据大小(按照字节大小)
*/
typedef struct intro_info {
    uint32 start_addr;
    uint8  num;
    uint8  flag;
    uint8 typesign;
    uint8 size;
} __attribute__ ( ( packed ) ) INTRO_INFO;

#define MAX_PACK_FUNC_NUM 20
#define MAX_PARSER_FUNC_NUM 21
#define DATA_CLASS_TABLE_NUM_1 9
class Gb101//如果每个对象的的字节不是偶数，请对齐
{

public:

    struct {
        
		prot_buf_t ProtBuf;
        TERMINALSTATUS TerSt;
        PRO_TIMEOUT timeout101[MAX_PORT];
        INTRO_INFO introx_info[16][8];//描述组数据,第一个下标是组号,第二个下标是该组内分成多帧的标号
        uint16 CurPortId;
        PRO_CLASS_DATA_TYPE_CFG class1_datatype_cfg;
        PRO_CLASS_DATA_TYPE_CFG class2_datatype_cfg;
        PRO_DATA_CLASS_CFG ProDataClassCfgTable[DATA_CLASS_TABLE_NUM_1];       
        PRO_LOGIC_BUF pro_logic_buf[MAX_PORT];
        PRO_CFG pro_cfg;
        PRO_DATA_TYPE_CFG pro_data_type_cfg;
        ID_MAP yx_id_map[MAXYXDATA];
        ID_MAP yc_id_map[MAXYCDATA];
        ID_MAP yk_id_map[MAXYKDATA];
        ID_MAP para_id_map[MAXPARADATA];
        ID_MAP bit32_id_map[MAX_BIT32_NUM];
        uint16 pro_yx_data_num;
        uint16 pro_dyx_data_num;
        uint16 pro_yc_data_num;
        uint16 pro_yk_data_num;
        uint16 pro_user_para_num;
        uint16 pro_user_bit32_num;
        PARSER_FUNC parser_func[MAX_PARSER_FUNC_NUM];
        PACK_FUNC pack_func[MAX_PACK_FUNC_NUM];
        C1_DATA_BUF c1_data_buf;
        C2_DATA_BUF c2_data_buf;
        DATA_GROUP_CFG data_group_cfg[6];
        pack_buf_t pb;
		uint8 xml_para_cnt;
		uint8 pc_mse_nx;
		uint8 cur_type;
		 uint8 CurDataClass1Id;
        uint8 CurDataClass2Id;
        uint8 LastDataClass1Id;
        uint8 LastDataClass2Id;
		uint8 InitOver;
    } __attribute__ ( ( packed ) ) member;

    char AddServiceBuf ( uint16 port_id, uint8 type, uint8 s_id );
    char GetServiceId (uint16 port_id, uint8 type, uint8 *s_id );

    char CheckTerSt ( void );
    void SetCauseOfInit ( char coi );
    char ConfigLogicBuf ( uint16 port_id );
    PRO_LOGIC_BUF* SelectLogicBuf ( uint16 port_id );


    char ConfigTimeOut ( uint16 port_id );
    PRO_TIMEOUT* SelectTimeOut ( uint16 port_id );
    uint16  DealComBuf ( BUF_MUX *buf, uint16 port_id );
    char DealTimeOut ( BUF_MUX *buf, uint16 port_id );
    void ConfigGroupData ( DATA_GROUP_CFG *pgf, INTRO_INFO ii[16][8], ID_MAP *pmap, uint16 data_num, uint8 data_type );
    char ScanSpontBuf ( uint16 port_id );
    void InitGroup();

    Gb101 ( void );
    int WriteCfg ( uint32 para_id,char *para );
    int ReadCfg ( void );
    uint32 IsMyPara(uint32 paraid);
    int ParaConfig ( xmlNodePtr node );
    int ParaCheck ( void );
    char Parser ( char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    char Pack ( uint8 type, char *in_buf, uint16 in_len, char *out_buf, uint16 *out_len );
    char SetC1Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time );
    char GetC1Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag );
    char SetC2Event ( uint16 dataid, char* data, uint8 datasize, uint8 data_type, char *time );
    char GetC2Event ( uint16 port_id, pack_buf_t *out_buf, uint8 data_type, uint8 time_tag );
    char InitDataClassTable ( void );
    PRO_DATA_CLASS_CFG* SearchClassTable ( uint8 data_type );
    int16 CheckFrame ( BUF_MUX *buf, uint16 port_id );
    
    ID_MAP* GetDataId ( uint32 dataaddr );
    ID_MAP* GetDataAddr ( uint16 dataid );
    char  GetKeyWordPositionAndLen ( uint8 keyword_id, uint8 *pos, uint8 *len );
    PACK_FUNC * SearchPackFuncTable ( uint8 type );

    PARSER_FUNC * SearchParserFuncTable ( uint8 type );

    char InitFunc ( void );
    char InitIdMapTable ( void );
    char SortYxIdMap ( void );
    char SortYcIdMap ( void );
    char SortYkIdMap ( void );
	void InitGb101(void);
};

extern char deal_101 ( BUF_MUX *buf, uint16 port_id );
extern char VerifyRecvFormalPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern char SetFuncCode ( uint8 fc );
extern char SetAcdDfc ( uint8 acd , uint8 dfc );
extern char SetCuaseWord ( uint8 cause );
extern uint8  GetSum ( uint8 *pBuf, int16 len );
extern char CreateFileInfo ( uint16 file_name, FILE_INFO *fi );

extern int ModifyParaXmlFile ( char *filedir, char *para, char *paraname );
extern int ParaModifyConfig ( xmlNodePtr node, char *data, char *paraname );



#endif



