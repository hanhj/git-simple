#ifndef FTU_H
#define FTU_H

#define BUFFER_SIZE 1024
#define FRAME_SIZE	512

/*物理串口使用
串口0:调试口;
串口1:cpu板通讯口1;
串口2:液晶通讯口;
扩展串口1:扩展板gprs;
扩展串口2:扩展板gps;
扩展串口3:cpu板通讯口3
扩展串口4:cpu板通讯口2;
扩展串口5:cpu板通讯口4
扩展串口6:扩展板短距离无线;
扩展串口7:扩展板串口1;
扩展串口8:扩展板串口2
扩展串口最高通讯速率57600
*/
#define MAX_UART 12//串口数量：
#define MAX_ETH_NUM	4//物理以太接口的数量
#define MAX_PPP_NUM	1//物理无线以太接口的数量
#define INTERFACE_NET1 0	//物理以太通道1 eth0
#define INTERFACE_NET2 1	//物理以太通道1 eth1
#define INTERFACE_NET3 2	//物理以太通道1 eth2
#define INTERFACE_NET4 3	//物理以太通道1 eth3
#define INTERFACE_NET5 4	//物理以太通道1 ppp0
#define INTERFACE_NET_NULL 0xff


#define MAX_SOCK_NUM	20//采用SOCK接口的数量
#define MAX_ETH_SOCK_NUM	16//采用以太网的SOCK接口的数量
#define MAX_PPP_SOCK_NUM	4//采用无线网的SOCK接口的数量

#define MAX_PORT 33
#define UART_PORT1 1
#define UART_PORT2 2
#define UART_PORT3 3
#define UART_PORT4 4
#define UART_PORT5 5//reverse for lcd
#define UART_PORT6 6
#define UART_PORT7 7
#define UART_PORT8 8
#define UART_PORT9 9//reverse for gprs
#define UART_PORT10 10//短距离无线
#define UART_PORT11 11//for gprs signal mux1
#define UART_PORT12 12//for sms mux2

#define ETH_PORT1  13
#define ETH_PORT2  14
#define ETH_PORT3  15
#define ETH_PORT4  16
#define ETH_PORT5  17
#define ETH_PORT6  18
#define ETH_PORT7  19
#define ETH_PORT8  20
#define ETH_PORT9  21
#define ETH_PORT10  22
#define ETH_PORT11  23
#define ETH_PORT12  24
#define ETH_PORT13  25
#define ETH_PORT14  26
#define ETH_PORT15  27
#define ETH_PORT16  28
#define WIRELESS_PORT1 29
#define WIRELESS_PORT2 30
#define WIRELESS_PORT3 31
#define WIRELESS_PORT4 32


#define UPGRADE_PORT 9999//远程升级监听端口号
/**
* /brief 最多保护参数套数
*/
#define MAXCURRENTPROSETS 8
/**
* /brief 最多回线数
*/


#define TM_FACTORY	"ZXJY"
#define TM_DEVICE	"FTU"

#endif
