#ifndef WIRELESS_UART_H
#define WIRELESS_UART_H
//comman command
#define CHECKSIMCARD 0
#define CHECKNET 1
#define CHECKPPP 2
#define CHECKTCP 3
#define CHECKIP 4
#define CHECKRSSI 5
#define SETAPN 6
#define OPENPPP 7
#define OPENTCP 8
#define CLOSETCP 9
#define CLOSEPPP 10
#define SENDDATA 11
#define ONOFF 12
#define SENDMSG 13
#define SMSMODE 14
#define TIMEOUT 31
#define STOPDIS 15
//mg815 & CM180
#define SETIDPWD 16
#define KEEPLINK 17
#define DELETESMS 18
#define READMSG 19
#define SETSMSNOTEC 20

//sim300 & m72-D
#define SETRCV 16
#define SETRATE 17
#define SETTRANSMODE 18
#define SETMSGCHAR  19
#define READSMS  20
#define DELSMS  21
#define SETSMSNOTEG  22
#define SWITCHTOAT  23
#define SWITCHTOGPRS  24

//MC52I only
#define SETMODE 16
#define	SETSRVTYPE 17
#define	SETSRV 18
#define	SETIP 19
#define	SETUSER 20
#define	SETPWD 21
#define READDATA 22
#define REOPENTCP 23

#define idle 0xff

enum status
{
	status_idle,
	status_prepare1,
	status_prepare2,
	status_receiving,
	status_prepare_finish
};

#define MAX_MODULE_PACK_SIZE 300//存储模块返回指令数据包长度
#define STATUS_TIME_OUT	15	//模块超时时间（秒）
#define CHECK_CYCLE_TIME 10//检查模块周期（秒）
enum CONNECT_STATUS
{
	NO_CARD,
	CARD_DETECTING,
	CARD_REGISTERING,
	CARD_DIALING,
	CARD_CONNECTING,
	CARD_CONNECTED,
};

typedef struct tag_AT_CMD												//AT指令表
{
	uint8 CMD_ID;											//指令ID，同时可指示指令状态
	char* CMD_STR;											//指令字符串
	char* CMD_STR_RETURN;									//返回对应与指令的关键字串
	char* CMD_RETURN;										//指令成功时返回字串关键字
	uint8 CMD_RETURN_NUM;									//关键字于返回字串的位置
	int16 (*fp)(void);										//处理函数
}AT_CMD;

class WIRELESS_UART:public UART{
public:
	int 	init(int port,void *parameter);
	int 	checkstate( int port );
	int 	wireless_status();
	
	int 	module_status;	
	int 	working_status;
	int16 module_rssi;
private:
	int get_working_status(){return working_status;}
};

#endif
