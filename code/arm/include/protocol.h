#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h"  ///<用到基础类定义
#include "gb101.h"
#include "user.h"
#define MAX_DEAL_PORT 50
#pragma pack(1)
//>define pro_sel
#define PRO101 				1
#define PRO104 				2
#define PRO_645_97			3	
#define PRO_645_07 			4
#define PRO_MODUL_BUS_RTU 	5
#define PRO_MODUL_BUF_TCP 	6
#define PRO_376_1			7
#define PRO_SNTP  			8
#define PROUPDATE 			9
#define PROGPS 				10
#define PRO_RARETRANS		20
#define GUANGDONG101 		30
#define GUANGDONG104 		31

#ifdef CONFIG_SYSLINK
#define PARA_MSG_TASK_ALL TASK_STATIS|TASK_COM|TASK_TIMER|TASK_VDSP|TASK_PRO
#define CTRL_MSG_TASK_ALL TASK_STATIS|TASK_COM|TASK_TIMER|TASK_VDSP//TASK_PRO
#else
#define PARA_MSG_TASK_ALL TASK_STATIS|TASK_COM|TASK_TIMER|TASK_PRO
#define CTRL_MSG_TASK_ALL TASK_STATIS|TASK_COM|TASK_TIMER//TASK_PRO
#endif

typedef struct buf_addr_regist_tab {
	uint8 port_status;
    uint16 port_id;
    BUF_MUX *paddr;
    uint8 pro_id;
    char ( *handle ) ( BUF_MUX *buf, uint16 port_id );
} __attribute__ ((packed))BUF_ADDR_REGIST_TABLE;

class Protocol : public BaseApp
{
public:
    //int16 Read(char *in_buf,uint16 len,pack_buf_t *pro_data);
    //int16 Write(char *in_buf,uint16 len,pack_buf_t *pro_data);
	struct {
	    BUF_ADDR_REGIST_TABLE buf_addr_regist_table[MAX_DEAL_PORT];
	    uint8 buf_addr_regist_num;
		uint16 YkPortNum;//避免两个主站同时遥控的通道绑定变量
		uint16 BtjPortNum;
		uint16 ParaPortNum;
		uint8 pc_mse_nx;
		uint8 data_type;
    } __attribute__ ((packed)) member;
    int16 SendMsg ( void );
    int16 AddBufRegistTable ( BUF_MUX *buf, uint16 port_id, uint8 pro_id );
    int16 ClearBufRegistTable ( void );
	int16 ClearBufRegistTable ( uint8 pro_id );
    //int16 CheckEvent(void);
	BUF_ADDR_REGIST_TABLE * SearchBufAddrRegistTable ( uint16 port_id );
	BUF_ADDR_REGIST_TABLE * SearchBufAddrRegistTable2 ( uint8 pro_id );
    int16 GetMsg ( void );
    int16 GetMsg ( MessageData<BaseDataType>message );
    void CopySendFrame ( BUF_MUX *p, char *buf, uint8 len );
    void AddSendBuf ( BUF_MUX *p, char *buf, uint8 len );
   	uint8 ForwordSendData ( uint16 fport, char *buf, uint16 len );
    uint8 GetBufAddrRegistNum ( void ) {
        return member.buf_addr_regist_num;
    };

    int16 SetBuff ( BUF_MUX *buf );
    Protocol();
    ~Protocol();
    int16 ReadCfg();
    int16 WriteCfg();
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
    *  @brief  运行模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Run(void*arg);


    int16 CheckError ( void ) {
        return 1;
    };
};

extern Protocol protocol;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* protocol_main ( void* );
////////////////////////////////////??ú???/////////////////////////

extern uint8 ProSetTime ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProGetData ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProSetData ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProGetPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProSetPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProSetYk ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProGetYk ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProRstLink ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProRstTer ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProRstTer ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProReadFile ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProWriteFile ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProGetLinkSt ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProReadFileInfo ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern uint8 ProActPara ( char *inbuf, uint16 inlen, char *outbuf, uint16 *outlen );
extern char ProSendMsg ( uint32 to_id, uint32 msg_id, uint32 dataid, char *data, uint8 len);
extern int isDyx(int32 id);


#endif
