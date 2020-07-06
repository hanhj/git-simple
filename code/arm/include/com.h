#ifndef COM_H
#define COM_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到基础类定义

#define CHAT_CFG "/etc/ppp/chat-evdo"//拨号文件
#define PAP_CFG "/etc/ppp/pap-secrets"//密码文件
#define CHAP_CFG "/etc/ppp/chap-secrets"//密码文件
#define CHAT_CFG_BAK "/etc/ppp/chat-evdo-bak"//拨号文件
#define PAP_CFG_BAK "/etc/ppp/pap-secrets-bak"//密码文件
#define CHAP_CFG_BAK "/etc/ppp/chap-secrets-bak"//密码文件



//>define eth work mode
#define SERVER_MODE 1
#define CLIENT_MODE 2
#define SERVER_CLIENT_MODE 3
#define MAX_RETRANS_PORT 16

typedef struct _forword_port_pair_tab {//第一个主口，第二个转发目标口
    uint8 flag;
    uint16 port1;
    uint16 port2;
} FORWORD_PORT_PAIR_TABLE;

typedef struct _proxyCfg{
	uint8 connect_sel;		//连接方式 0 ~ 3
	uint8 proxy_type;           //代理服务器类型 0：NULL、 1：HTTP、 2：SOCK4、3：SOCK5
	uint32 proxy_ip;                     //代理服务器地址
	uint16 proxy_port;          //代理服务器端口
	uint8 proxy_style;          //代理服务器连接方式 0：匿名、1：需身份验证
	uint8 proxy_user_len;       //代理服务器用户名长度
	uint8 proxy_user[21];       //代理服务器用户名
	uint8 proxy_pwd_len;        //代理服务器密码长度
	uint8 proxy_pwd[21];        //代理服务器密码
}ProxyCfg;

class CommCfg {
	public:
        BUF_MUX *Buff;
        char InterfaceName[20];
        uint8 InterfaceId;//标识物理通道
        int8 Use;
};

class EthCfg:public CommCfg{
	public:
        uint8 LocalIp[4];/**<  本地ip  */
        uint16 LocalPort;/**<  本地端口  */
        uint8 Mask[4];/**<  子网掩码  */
        uint8 Gate[4];/**<  网关  */
        uint8 Mac[6];/**<  mac地址  */
        uint8 Dns[4];/**<  域名解析服务器地址 */
        uint8 HostIp[4];/**<  主机ip  */
        uint16 HostPort;/**<  主机端口  */
		ProxyCfg	proxycfg;        
        int8 Mode;/**<  工作模式[1 服务器模式，2 客户端模式，3 混合模式  */
        int8 isMaster;//是否是主设备，因为可能多个口公用一个物理设备，比如sock1，sock2公用一个以太网口，所以只需一个sock去维护物理连接，无线类似。
        int16 RelinkTime;//连接断开后重连时间
		int16 TransPro;//udp or tcp. 1 tcp 2 udp
        CommCfg *cfg;
};

class UartCfg:public CommCfg{
	public:
        int32 BaudRate;/**<  波特率:300,600,1200,2400,4800,9600,115200,38400 */
        int8 DataBit;/**<  数据位 7,8*/
        int8 StopBit;/**<  停止位 1-1位停止位,2-2位停止位,3-1。5位停止位*/
        int8 CheckBit;/**<  校验位 0:无校验，1 奇校验 2 偶校验*/
        int8 isMaster;//是否是主设备
};

class WireLessCfg:public EthCfg{
	public:
        uint8 Vpn[32];/**<  cmnet etc. ascii 码 */
        uint8 UserName[32];/**<  用户名称  ascii 码*/
        uint8 Passwd[32];/**<  密码ascii 码 */
        int8 WireLessType;/**<  网络选择 */
        int32 MonFlowSet;/**<  月流量 上下行 单位KB  */
};
        
typedef struct _ComCfg {
    uint8 	com_use[MAX_PORT];//当前使用情况
    uint8 	com_olduse[MAX_PORT];
    void *	Para[MAX_PORT];
    BUF_MUX  Buff[MAX_PORT];
    EthCfg	eth_para[MAX_ETH_NUM];//用于配置以太物理接口
    WireLessCfg wireless_para[MAX_PPP_NUM];//用于配置无线物理接口
    UartCfg uart_cfg[MAX_UART];///<10个物理串口+GPRS监控通道参数,GPRS监控扩展通道
    EthCfg  eth_cfg[MAX_ETH_SOCK_NUM];//16个以太网口sock
    WireLessCfg wireless_cfg[MAX_PPP_SOCK_NUM];//4个无线网口sock
    
}ComCfg;

typedef struct _ComRunData {
    int32   mon_flow_send[MAX_ETH_NUM+MAX_PPP_NUM][3];//每个通道的月流量kb 0-月流量统计 1,2-临时统计
    int32   mon_flow_rcv[MAX_ETH_NUM+MAX_PPP_NUM][3];//每个通道的月流量kb 0-月流量统计 1,2-临时统计    
    int16   cur_mon; 
}ComRunData;
/**
*************************************************************************************
*  @brief  通讯类定义。
*/

class Comm : public BaseApp
{

private:
    FORWORD_PORT_PAIR_TABLE fppt[MAX_RETRANS_PORT];
    uint8 tmp_buf[BUFFER_SIZE];
    ComRunData com_data;
    /**
    *************************************************************************************
    *  @brief  内部读驱动数据接口
    *  @author  liht
    *  @param[in] uint16 port 模块内通讯端口号，由头文件统一定义
    *  @param[out] char *buf 读数据的缓存
    *  @param[out] uint16 *ret_len 实际读出的数据长度
    *  @return  成功返回1，否则返回0，返回1代表对应端口的数据读成功
    *  @note 无。
    *  @see  CCOM
    **************************************************************************************
    */
    int16 Read ( uint16 port, uint8 *buf,  uint16 *ret_len );
    /**
    *************************************************************************************
    *  @brief  内部写驱动数据接口
    *  @author  liht
    *  @param[in] uint16 port 模块内通讯端口号，由头文件统一定义
    *  @param[in] char *buf 写数据的缓存
    *  @param[in] uint16 len 通知模块预期写的数据长度
    *  @return  成功返回1，否则返回0，返回1代表对应端口的通讯参数写成功，
    *  @note 无。
    *  @see  CCOM
    **************************************************************************************
    */
    int16 Write ( uint16 port, uint8 *buf, int len );
public:
    ComCfg com_cfg;///<Com配置参数
    Comm();
    ~Comm();
    int16 ReadCfg();
    int16 WriteCfg();
//    BUF_MUX gprs_buf;/**<  无线通信的缓存  */
//    BUF_MUX usart_buf;/**<  串口通信缓存  */
//    BUF_MUX eth_buf;/**<  以太网通信缓存  */
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
    int16 GetMsg ( MessageData<BaseDataType>message );
    /**
    *************************************************************************************
    *  @brief  运行模块。
    *  @author  liht
    *  @param [in]
    *  @param [out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Run ( void*arg );
    /**
    *************************************************************************************
    *  @brief  通讯过程中错误检查。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 CheckError ( uint16 port );
    /**
    *************************************************************************************
    *  @brief  获得对应端口的状态。
    *  @author  liht
    *  @param[in] int8 port 
    *  @return  端口可用返回1 端口不可用，返回0。
    **************************************************************************************
    */
    uint8 GetPortStatus ( uint16 port );
    /**
    *************************************************************************************
    *  @brief  打开通讯口，启动该通讯口的通讯过程.
    *  @author  liht
    *  @param [in] .
    *  @param [out] .
    *  @return  成功，返回1，否则返回0.
    **************************************************************************************
    */
    int16 Open ( uint16 port );
    /**
    *************************************************************************************
    *  @brief  关闭通讯口。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Close ( uint16 port );
    /**
    *************************************************************************************
    *  @brief  复位通讯口。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Reset ( uint16 port );
};

extern Comm comm;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* comm_main ( void* );
#endif
