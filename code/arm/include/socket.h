#ifndef SOCKET_H
#define SOCKET_H

#define _PATH_PROCNET_DEV "/proc/net/dev"
#define MAX_FAIL_TIMES 120
#define SIN_ADDR(x) (((struct sockaddr_in *)(&(x)))->sin_addr.s_addr)

#define SET_SA_FAMILY(addr, family)                        \
	            memset ((char *) &(addr), '\0', sizeof(addr));        \
    addr.sa_family = (family);

struct net_interface
{
	unsigned long long rx_packets;	/* total packets received       */
	unsigned long long tx_packets;	/* total packets transmitted    */
	unsigned long long rx_bytes;	/* total bytes received         */
	unsigned long long tx_bytes;	/* total bytes transmitted      */
	unsigned long rx_errors;	/* bad packets received         */
	unsigned long tx_errors;	/* packet transmit problems     */
	unsigned long rx_dropped;	/* no space in linux buffers    */
	unsigned long tx_dropped;	/* no space available in linux  */
	unsigned long rx_multicast;	/* multicast packets received   */
	unsigned long rx_compressed;
	unsigned long tx_compressed;
	unsigned long collisions;

	/* detailed rx_errors: */
	unsigned long rx_frame_errors;	/* recv'd frame alignment error */
	unsigned long rx_fifo_errors;	/* recv'r fifo overrun          */
	/* detailed tx_errors */
	unsigned long tx_carrier_errors;
	unsigned long tx_fifo_errors;
};

#define GET_TX_BYTES 1
#define GET_RX_BYTES 2
#define GET_TOTAL_BYTES 3

class SOCKET{
public:
	int 	init(int port,void *parameter);
	//复位通道函数
	int 	reset(int port);
	int 	sendbuf(int port, uint8 *buf, int len);
	int 	getbyte( int port, uint8 *c );
	int 	checkstate( int port );	
	int 	close_port(int port);	
	uint32 	get_trans_bytes(char *interfacename,int8 flag);	//获取流量单位K
	int		up(void*);//启动以太口
	int		down(void*);//关闭以太口
	
protected:
	int 	get_channel(int port);
	void 	set_interface_name(char *interface_name);

private:
	int 	revert(int channel, int revert);
	int 	interface_up(char * interface_name);
	int 	interface_down(char * interface_name);	
	int32 set_mac(uint8 *mac);
	void 	reset_mac(uint8 *mac);
	int 	set_subnet_mask(uint32 subNetMask);
	int 	set_ip(uint32 ip);
	int 	set_gate_way(uint32 gate_ip);

	int 	route_config(int channel,int interface);
	int 	create_socket(int id);
	void	 close_socket(int id);
	
	char interface_name[32];

};

extern int 	read_socket(int fd, void *buffer,int length);	//for thread
extern int 	check_link( char *interface_name );	//for thread
extern void  reset_queue(int channel)	;	//for thread
extern uint32 get_local_ip(char *interface_name);

	
extern int connection_st_changed[MAX_SOCK_NUM];
extern int need_reconnect[MAX_SOCK_NUM];
extern int net_connected[MAX_SOCK_NUM];
extern int sockfd[MAX_SOCK_NUM], nbytes[MAX_SOCK_NUM];
extern int sock_quit_thread[MAX_SOCK_NUM];
extern THREAD_DATA sock_thread_data[MAX_SOCK_NUM];	
extern pthread_t sock_tid[MAX_SOCK_NUM];
extern int net_connect_time[MAX_SOCK_NUM];
extern int link_time[MAX_SOCK_NUM];
extern struct cond thread_create_ok;

//用于创建一个不被子进程继承句柄的socket
int mysocket(int socket_family, int socket_type, int protocol)	;

#endif