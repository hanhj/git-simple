#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "com.h"
#include "debug.h"
//#include "com_util/com_util.h"

#define MAX_SOCKT_CONNECT_TIME 10//单位：秒
#define false -1
#define true 0

int auto_connect( int sockfd, uint32 ip, uint16 port );
int connect_socks5( int sockfd , uint32 ip, uint16 port,ProxyCfg*);
int connect_socks4( int sockfd, uint32 host_ip, uint16 host_port ,ProxyCfg*);
int connect_http( int sockfd, uint32 host_ip, uint16 host_port,ProxyCfg* );
void ipv4n_to_ipv4p( uint32 addr, char *dst, int cnt);

int connect_ms_by_proxy(int sockfd, uint8_t proxy_type, uint32 hostip, uint16 hostport ,ProxyCfg*proxy)
{
	int ret = 0;

	switch ( proxy_type ) {
		case 1:		//http proxy
			connect_http( sockfd, hostip, hostport, proxy);
			break;
		case 2:		//socks4 proxy
			ret = connect_socks4( sockfd, hostip, hostport , proxy);
			break;
		case 3:		//socks5 proxy
			ret = connect_socks5( sockfd, hostip, hostport , proxy);
			break;
		case 0:		//no proxy
		default:
			ret = auto_connect(sockfd, hostip, hostport);
			break;
	}
	//PFUNC(TEM_INFO ,DEBUG_COM, "connect ret = %d\n", ret );

	return ret;
}
uint32 get_socket_ip(int sockfd)
{
	struct ifreq ifr;
	struct   sockaddr_in   sin;
	strcpy(ifr.ifr_name, "ppp0");
	if(ioctl(sockfd, SIOCGIFADDR, &ifr,sizeof(struct ifreq))<0)
		return 0;
	else{
		memcpy(&sin,   &ifr.ifr_addr,   sizeof(sin));
		return (uint32)sin.sin_addr.s_addr;
	}
}
/*
   原理上是这样的：
   1．建立socket
   2．将该socket设置为非阻塞模式
   3．调用connect()
   4．使用select()检查该socket描述符是否可写（注意，是可写）
   5．根据select()返回的结果判断connect()结果
   6．将socket设置为阻塞模式（如果你的程序不需要用阻塞模式的，这步就省了，不过一般情况下都是用阻塞模式的，这样也容易管理）
   需要说明的几点是，虽然我们用ioctl把套接口设置为非阻塞模式，不过select本身是阻塞的，
 * 阻塞的时间就是其超时的时间由调用select 的时候的最后一个参数timeval类型的变量指针指向的timeval结构变量来决定的，
 * timeval结构由一个表示秒数的和一个表示微秒数（long类型）的成员组成，一般我们设置了秒数就行了，把微妙数设为0（注：1秒等于100万微秒）。
 * 而select函数另一个值得一提的参数就是上面我们用到的fd_set类型的变量指针。
 * 调用之前，这个变量里面存了要用select来检查的描述符，调用之后，针对上面的程序这里面是可写的描述符，我们可以用宏FD_ISSET来检查某个描述符是否在其中。
 * 由于我这里只有一个套接口描述符，我就没有使用FD_ISSET宏来检查调用select之后这个sockfd是否在set里面，其实是需要加上这个判断的。
 * 不过我用了getsockopt来检查，这样才可以判断出这个套接口是否是真的连接上了，因为我们只是变相的用select来检查它是否连接上了，
 * 实际上select检查的是它是否可写，而对于可写，是针对以下三种条件任一条件满足时都表示可写的：
 1）套接口发送缓冲区中的可用字节数大于等于套接口发送缓冲区低潮限度的当前值，且或者i)套接口已连接，或者ii)套接口不要求连接（UDP方式的）
 2）连接的写这一半关闭
 3）有一个套接口错误待处理。
 这样，我们就需要用getsockopt函数来获取套接口目前的一些信息来判断是否真的是连接上了，没有连接上的时候还能给出发生了什么错误，当然我程序中并没有标出那么多状态，
 * 只是简单的表示可连接/不可连接。
 下面我来谈谈对这个程序测试的结果。我针对3种情形做了测试：
 1． 目标机器网络正常的情况
 可以连接到目标主机，并能成功以阻塞方式进行发包收包作业。
 2． 目标机器网络断开的情况
 在等待设置的超时时间（上面的程序中为20秒）后，显示目标主机不能连接。
 3． 程序运行前断开目标机器网络，超时时间内，恢复目标机器的网络
 在恢复目标主机网络连接之前，程序一只等待，恢复目标主机后，程序显示连接目标主机成功，并能成功以阻塞方式进行发包收包作业。
 以上各种情况的测试结果表明，这种设置connect超时的方法是完全可行的。
 * 同时要注意，如果采用PPPD，必须等到获得ip后才能使用，否则会直接返回，而且不报错误。
 *
 */
extern uint32 get_pppd_ip(void);
int connect_with_timeout(int sockfd, uint32 ip, uint16 port,int16 timeout)
{
	int ret, len = 4, error = -1;
	struct sockaddr_in server_addr;
	struct timeval tm;
	struct in_addr host_ip;
	int flags;
	fd_set w_st;
	int t;

	ret = -1;
	flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags|O_NONBLOCK); //设置为非阻塞模式
	memcpy(&host_ip, &ip, 4);

	bzero(&server_addr, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = host_ip;

	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
		if (errno == EINPROGRESS) {
			tm.tv_sec = timeout;
			tm.tv_usec = 0;
			FD_ZERO(&w_st);
			FD_SET(sockfd, &w_st);
			t=select(sockfd+1, NULL, &w_st,NULL, &tm);
			if( t > 0){
				if(FD_ISSET(sockfd,&w_st)){
					getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
					PFUNC(TEM_WARNING ,DEBUG_COM, "error = %d\n", error);
					if(error == 0)
						ret = 0;
					else
						ret = -1;
				}
			}else{
				ret = -1;
				PFUNC(TEM_WARNING ,DEBUG_COM, "ret:%d \n",ret);
			}
		}else  {
			ret = -1;
			PFUNC(TEM_WARNING ,DEBUG_COM, "ret:%d \n",ret);
		}
	}else{
		ret = 0;
	}
	flags=fcntl(sockfd,F_GETFL,0);
	fcntl(sockfd, F_SETFL, flags&~O_NONBLOCK); //设置为阻塞模式

	return ret;
}

int auto_connect( int sockfd, uint32 ip, uint16 port )
{
	return connect_with_timeout(sockfd, ip, port, MAX_SOCKT_CONNECT_TIME);
}

int connect_socks5( int sockfd, uint32 host_ip, uint16 host_port,ProxyCfg*proxy )
{
	uint8_t proxy_type, proxy_style, proxy_user_len, proxy_pwd_len;
	uint16 proxy_port;
	uint32 proxy_ip;
	int ret, len = 0;
	uint8_t cmd[256], proxy_user[21], proxy_pwd[21];
	memset(cmd, 0, 64);
	if(proxy==NULL)
		return -1;
	//thNetPara.lock();
	proxy_ip = proxy->proxy_ip;
	proxy_port = proxy->proxy_port;
	proxy_type = proxy->proxy_type;
	proxy_style = proxy->proxy_style;
	proxy_user_len = proxy->proxy_user_len;
	proxy_pwd_len = proxy->proxy_pwd_len;
	memcpy(proxy_user, proxy->proxy_user, proxy_user_len);
	memcpy(proxy_pwd, proxy->proxy_pwd, proxy_pwd_len);
	//thNetPara.unlock();

	ret = auto_connect(sockfd, proxy_ip, proxy_port);
	if ( ret < 0 )
		return -1;
	cmd[0] = 5;	//socks5 version
	cmd[1] = 1;	//methods count
	if (proxy_style)	//auth:user, passwd
		cmd[2] = 2;
	else				//no auth
		cmd[2] = 0;

	PFUNC(TEM_INFO ,DEBUG_COM, "\n");
	ret = send( sockfd, cmd, 3, 0);
	PFUNC(TEM_INFO ,DEBUG_COM, "ret = %d\n", ret);
	if ( ret < 0 ) {
		return -1;
	}
	ret = recv( sockfd, cmd, sizeof(cmd) , 0);
	PFUNC(TEM_INFO ,DEBUG_COM, "recv ret = %d, %d:%d\n", ret, cmd[0], cmd[1]);
	if ( ret <= 0 )
		return -1;

	if ( cmd[0] == 5 ) {
		if ( cmd[1] == 0 )	//no auth
			;
		else if ( cmd[1] == 2 ) {//auth

			cmd[len++] = 1;
			cmd[len++] = proxy_user_len;
			memcpy(&cmd[len], proxy_user, proxy_user_len);
			len += proxy_user_len;
			cmd[len++] = proxy_pwd_len;
			memcpy(&cmd[len], proxy_pwd, proxy_pwd_len);
			len += proxy_pwd_len;
			ret = send( sockfd, cmd, len, 0);
			if ( ret < 0 )
				return -1;
			ret = recv( sockfd, cmd, sizeof(cmd), 0);
			PFUNC(TEM_INFO ,DEBUG_COM, "auth recv:%d %d:%d\n", ret, cmd[0], cmd[1]);
			if ( ret <= 0 )
				return -1;
			if (cmd[0] != 1 || cmd[1] != 0)		//auth failed
				return -1;
		}
		else
			return -1;	//failed
	} else
		return -1;

	PFUNC(TEM_INFO ,DEBUG_COM, "\n");
	//target ip and port
	len = 0;
	cmd[len++] = 5; //socks 5 version
	cmd[len++] = 1; //CONNECT
	cmd[len++] = 0;	//RESERVED
	cmd[len++] = 1; //ipv4
	cmd[len++] = (host_ip & 0xFF000000) >> 24;
	cmd[len++] = (host_ip & 0x00FF0000) >> 16;
	cmd[len++] = (host_ip & 0x0000FF00) >> 8;
	cmd[len++] = host_ip & 0x000000FF;
	PFUNC(TEM_INFO ,DEBUG_COM, "%02x %02x %02x %02x\n", cmd[len-4], cmd[len - 3], cmd[len-2], cmd[len-1]);
	cmd[len++] = (host_port & 0xFF00) >> 8;
	cmd[len++] = host_port & 0x00FF;
	ret = send( sockfd, cmd, len, 0 );
	PFUNC(TEM_INFO ,DEBUG_COM, "send ret = %d\n", ret);
	if ( ret < 0 )
		return -1;
	ret = recv( sockfd, cmd, sizeof(cmd), 0);
	PFUNC(TEM_INFO ,DEBUG_COM, "hostip recv:%d %d:%d\n", ret, cmd[0], cmd[1]);
	if ( ret <= 0 )
		return -1;
	if ( cmd[0] == 5) {
		int res;

		switch(cmd[1]) {
			case 0:
				PFUNC(TEM_INFO ,DEBUG_COM, "connect ms success\n");
				res = 0;
				break;
			case 1:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "general socks server failure\n");
				break;
			case 2:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "connection isn't fit to server\n");
				break;
			case 3:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "taget net cann't connect\n");
				break;
			case 4:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "target ip cann't connect\n");
				break;
			case 5:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "connection refused\n");
				break;
			case 6:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "TTL expired\n");
				break;
			case 7:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "command not support\n");
				break;
			case 8:
				res = -1;
				PFUNC(TEM_INFO ,DEBUG_COM, "address format not support\n");
				break;
			default:
				res = -1;
				break;
		}

		return 0;
	} else
		return -1;
}

int connect_socks4( int sockfd, uint32 host_ip, uint16 host_port,ProxyCfg*proxy )
{
	uint8_t cmd[256];
	uint16 proxy_port;
	uint32 proxy_ip;
	int ret;
	int len = 0;
	if(proxy==NULL)
		return -1;

	//thNetPara.lock();
	proxy_ip = proxy->proxy_ip;
	proxy_port = proxy->proxy_port;
	//thNetPara.unlock();

	memset(cmd, 0, 256);
	ret = auto_connect(sockfd, proxy_ip, proxy_port);
	PFUNC(TEM_INFO ,DEBUG_COM, "ret = %d\n", ret);
	if ( ret < 0 )
		return -1;
	cmd[len++] = 4;	//socks4 version
	cmd[len++] = 1;	//command: CONNECT
	cmd[len++] = (host_port & 0xFF00) >> 8;
	cmd[len++] = host_port & 0x00FF;
	cmd[len++] = (host_ip & 0xFF000000) >> 24;
	cmd[len++] = (host_ip & 0x00FF0000) >> 16;
	cmd[len++] = (host_ip & 0x0000FF00) >> 8;
	cmd[len++] = host_ip & 0x000000FF;
	cmd[len++] = 0; //END
	ret = send( sockfd, cmd, len, 0);
	PFUNC(TEM_INFO ,DEBUG_COM, "ret = %d\n", ret);
	if ( ret < 0 ) {
		return -1;
	}
	memset(cmd, 0, sizeof(cmd));
	ret = recv( sockfd, cmd, sizeof(cmd) , 0);
	PFUNC(TEM_INFO ,DEBUG_COM, "recv ret = %d, %d:%d\n", ret, cmd[0], cmd[1]);
	if ( ret <= 0 )
		return -1;

	if ( cmd[0] == 4 ) {
		int res = 0;
		switch( cmd[1] ) {
			case 0x5A:		//request granted
				res = 0;
				break;
			case 0x5B:		//request rejected or failed
				res = -1;
				break;
			case 0x5C:		//request rejected becasue SOCKS server cannot connect to identd on the client
				res = -1;
				break;
			case 0x5D:		//request rejected because the client program and identd report different user-ids
				res = -1;
				break;
			default:
				res = -1;
				break;
		}
		return res;
	} else
		return -1;

}

int connect_http( int sockfd, uint32 host_ip, uint16 host_port, ProxyCfg*proxy)
{
	int wlen, bpos;
	char str_ip[128];
	char rw_buf[256];
	int codenum;                // proxy response number
	int httpvn1, httpvn2;       // minor/major numbers of HTTP version in proxy response
	int ret;
	uint16 proxy_port;
	uint32 proxy_ip;

#define HTTP_VERSION    "1.1"
	//thNetPara.lock();
	proxy_ip = proxy->proxy_ip;
	proxy_port = proxy->proxy_port;
	//thNetPara.unlock();

	ret = auto_connect(sockfd, proxy_ip, proxy_port);
	if ( ret < 0 )
		return -1;
	ipv4n_to_ipv4p(host_ip, str_ip, sizeof(str_ip));
	bpos = wlen =
		snprintf(rw_buf, sizeof(rw_buf), "CONNECT %s:%d HTTP/" HTTP_VERSION "\n",
				str_ip, host_port);
	if( wlen >= 0)
		bpos += (wlen = snprintf(rw_buf+bpos, sizeof(rw_buf)-bpos, "HOST %s:%d\n", str_ip, host_port));
	// write terminating newline
	if (wlen >= 0)
		wlen = snprintf(rw_buf + bpos, sizeof(rw_buf) - bpos,"\n");

	// was there any overflow?
	if (wlen < 0)
	{
		return -1;
	}
	if (send(sockfd, rw_buf, strlen(rw_buf), 0) < 0)
	{
		return -1;
	}


	ret = recv( sockfd, rw_buf, sizeof(rw_buf), 0);
	if ( ret < 0 )
		return -1;
	if (!sscanf(rw_buf, "HTTP/%d.%d %d", &httpvn1, &httpvn2, &codenum))
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "unable to parse received line\n");
		return -1;
	}
	PFUNC(TEM_INFO ,DEBUG_COM, "codenum = %d\n", codenum);
	if ((codenum / 100) != 2)
		return -1;
	return 0;
}

void ipv4n_to_ipv4p(uint32 addr, char *dst, int cnt)
{
	snprintf(dst, cnt, "%ld.%ld.%ld.%ld",
			(addr>>24)&0xff, (addr>>16)&0xff,
			(addr>>8)&0xff, addr&0xff);
}
