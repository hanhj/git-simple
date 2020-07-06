#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <net/route.h>
#include <linux/if_ether.h>
#include <net/if_arp.h>
#include <pthread.h>
#include <signal.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include "debug.h"
#include "socket.h"
#include "os.h"
#include "com.h"
#include "utils.h"
#include "task.h"
#include "proxyclient.h"

#define TCP 1
#define UDP 2

struct cond thread_create_ok = {
	"",
 0,
 PTHREAD_MUTEX_INITIALIZER,
 PTHREAD_COND_INITIALIZER,
};


	

int sockfd[MAX_SOCK_NUM];
int sock_quit_thread[MAX_SOCK_NUM];
int need_reconnect[MAX_SOCK_NUM];
int net_connected[MAX_SOCK_NUM];
int nbytes[MAX_SOCK_NUM];
int link_time[MAX_SOCK_NUM];
THREAD_DATA sock_thread_data[MAX_SOCK_NUM];	
pthread_t sock_tid[MAX_SOCK_NUM];

static int server_fd[MAX_SOCK_NUM];
static int 	init2(int channel,void *parameter);//for thread

void SOCKET::set_interface_name(char *name)
{
	strcpy(interface_name,name);
}

void SOCKET::reset_mac(uint8 *mac)
{
	mac[0] = 0x00;
	mac[1] = 'j';
	mac[2] = 'y';
	mac[3] = rand() % 0xFD + 1;
	mac[4] = rand() % 0xFD + 1;
	mac[5] = rand() % 0xFD + 1;
}


int SOCKET::interface_up(char * interface_name)
{
	struct ifreq ifr;
	short flag;
	int sock;
//	char str[100];
	int ret;

	//if ineterface is not ifconfig ,i will ifconfig up it
/*
	sprintf(str,"ifconfig %s up",interface_name);
	ret=system(str);
	if(ret<0){
		PFUNC(TEM_ERROR ,DEBUG_COM, "up %s err:%d\n",interface_name,ret);
	}
	return ret;
*/	
	
	if((sock = socket(PF_INET,SOCK_STREAM,0)) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "Socket create erro\n");
		return -1;
	}

	strcpy(ifr.ifr_name,interface_name);

	flag = IFF_UP;
	if(ioctl(sock,SIOCGIFFLAGS,&ifr) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCGIFFLAGS\n",interface_name);
		ret = -1;
		goto out;
	}

	ifr.ifr_ifru.ifru_flags |= flag;

	if(ioctl(sock,SIOCSIFFLAGS,&ifr) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCSIFFLAGS\n",interface_name);
		ret = -1;
		goto out;
	}

out:
	close(sock);
	return ret;

}

int SOCKET::interface_down(char * interface_name)
{
	int sock;
	struct ifreq ifr;
	short flag;
//	char str[100];
	int ret = 0;
/*	
	sprintf(str,"ifconfig %s down",interface_name);
	ret=system(str);
	if(ret<0){
		PFUNC(TEM_ERROR ,DEBUG_COM, "down %s err:%d\n",interface_name,ret);
		return ret;
	}

	if(strcmp(interface_name,"lo") == 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "You can't pull down interface lo\n");
		return 0;
	}

	return ret;
*/	

	if((sock = socket(PF_INET,SOCK_STREAM,0)) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "Socket create error");
		return -1;
	}

	strcpy(ifr.ifr_name,interface_name);

	flag = ~IFF_UP;
	if(ioctl(sock,SIOCGIFFLAGS,&ifr) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCGIFFLAGS\n",interface_name);
		ret = -1;
		goto out;
	}

	ifr.ifr_ifru.ifru_flags &= flag;

	if(ioctl(sock,SIOCSIFFLAGS,&ifr) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCSIFFLAGS\n",interface_name);
		ret = -1;
	}

out:
	close(sock);
	return ret;
	
}



int32 SOCKET::set_mac(uint8 *mac)
{
	struct ifreq ifr;
	int32 sock;
	int i;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "Socket error\n");
		return -1;
	}
	strcpy(ifr.ifr_name, interface_name);
	if (ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCGIFHWADDR\n",interface_name);
		close(sock);
		return -1;
	}
	
	for (i = 0; i < 6; i++) {
		ifr.ifr_hwaddr.sa_data[i] = mac[i];
	}
	if (ioctl(sock, SIOCSIFHWADDR, &ifr) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCSIFHWADDR \n",interface_name);
		close(sock);
		return -1;
	}
	close(sock);
	return 0;
}

int SOCKET::set_subnet_mask(uint32 subNetMask)
{
	int sock;
	struct ifreq ifr;
	struct sockaddr_in netmask_addr;
	int ret = 0;

	if((sock = socket(PF_INET,SOCK_STREAM,0)) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "Socket error\n");
		return -1;
	}

	strcpy(ifr.ifr_name,  interface_name );

	bzero(&netmask_addr,sizeof(struct sockaddr_in));
	netmask_addr.sin_family = PF_INET;

	memcpy(&netmask_addr.sin_addr, &subNetMask, 4);
	memcpy(&ifr.ifr_ifru.ifru_netmask,&netmask_addr,sizeof(struct sockaddr_in));

	if(ioctl(sock,SIOCSIFNETMASK,&ifr) < 0)
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl error SIOCSIFNETMASK \n",interface_name);
		ret = -1;
	}

	close(sock);
	return ret;
}

int SOCKET::set_ip(uint32 ip)
{
	int                     sock;
	struct ifreq            ifr;
	struct sockaddr_in      *sin;
	int ret = 0;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "socket error");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, interface_name, sizeof(ifr.ifr_name)-1);

	sin = (struct sockaddr_in *)&ifr.ifr_addr;
	sin->sin_family = AF_INET;

	memcpy(&sin->sin_addr, &ip, 4);

	if (ioctl(sock, SIOCSIFADDR, &ifr) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl SIOCSIFADDR error",interface_name);
		ret = -1;
	}
	close(sock);
	return ret;
}

int SOCKET::set_gate_way(uint32 gate_ip)
{
	struct rtentry  rt;
	int sfd;
	int ret = 0;

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		PFUNC(TEM_ERROR ,DEBUG_COM, "socket error");
		return -1;
	}

	memset( &rt, 0, sizeof(rt) );
	SET_SA_FAMILY( rt.rt_dst, AF_INET );
	SET_SA_FAMILY( rt.rt_gateway, AF_INET );
	SET_SA_FAMILY( rt.rt_genmask, AF_INET );

	SIN_ADDR( rt.rt_dst ) = 0;
	SIN_ADDR( rt.rt_genmask ) = 0;
	SIN_ADDR( rt.rt_gateway ) = gate_ip;

	rt.rt_dev=interface_name;

	rt.rt_flags = RTF_UP | RTF_GATEWAY;

	if ( ioctl( sfd, SIOCADDRT, &rt ) < 0 )
	{
		PFUNC(TEM_ERROR ,DEBUG_COM, "%s ioctl SIOCADDRT error",interface_name);
		ret = -1;
	}

	close(sfd);
	return ret;
}



int SOCKET::get_channel(int port)
{
	int channel;
	if(port>=ETH_PORT1&&port<=WIRELESS_PORT4)
		channel=port-ETH_PORT1;
	else
		channel=-1;
return channel;
}	
int SOCKET::getbyte(int port, unsigned char *c)
{

	int index = -1;
	int channel;
	uint16 *pproduce, *pconsume;
	channel=get_channel(port);

	if (channel < 0 || channel > MAX_SOCK_NUM) {
		return -1;
	}

	index= channel;

	pproduce = &sock_thread_data[index].p_buf->recv.in_cnt;
	pconsume = &sock_thread_data[index].p_buf->recv.out_cnt;


	if (*pconsume != *pproduce) {
		*c = sock_thread_data[index].p_buf->recv.buf[*pconsume];

		PFUNC(TEM_INFO ,DEBUG_COM, "1. index = %d, produce = %d, consume = %d, receive char=%02x\n", index, *pproduce, *pconsume, *c);
		if (++(*pconsume) >= BUFFER_SIZE)
			*pconsume = 0;
		return 0;
	} else
		return -1;
}

int SOCKET::revert(int channel, int revert)
{
	int index = -1;
	uint16  *pconsume;
	index= channel;
	pconsume = &sock_thread_data[index].p_buf->recv.out_cnt;
		
	if (*pconsume >= revert) {
		*pconsume -= revert;
	} else {
		*pconsume += (BUFFER_SIZE - revert);
	}
	return 0;
}

int read_socket(int fd, void *buffer,int length)
{
	int    bytes_read = 0;

	bytes_read = read(fd, buffer, length);
	//PFUNC(TEM_INFO ,DEBUG_COM, "read_socket %d readed, in:%d, out:%d\n", bytes_read, queue[0].in, queue[0].out);
	if(bytes_read < 0) {
		PFUNC(TEM_WARNING ,DEBUG_COM, "------- fd = %d, errno = %d, EBADF = %d, EAGAIN: = %d\n", fd, errno, EBADF, EAGAIN);
		perror("read socket:");
		switch (errno) {
			case EINTR:
			case EAGAIN:
				bytes_read = 0;
				break;
			default:
				return -1;
		}
	} else if(bytes_read==0) {
		PFUNC(TEM_WARNING ,DEBUG_COM, "READ SOCKED RETURN 0\n");
		return -1;
	}

	return(bytes_read);
}


void reset_queue(int channel)
{
	sock_thread_data[channel].p_buf->recv.in_cnt = 0;
	sock_thread_data[channel].p_buf->recv.out_cnt = 0;
	sock_thread_data[channel].p_buf->send.in_cnt = 0;
	sock_thread_data[channel].p_buf->send.out_cnt = 0;
	memset(sock_thread_data[channel].p_buf->recv.buf, 0, BUFFER_SIZE*sizeof(uint8));
	memset(sock_thread_data[channel].p_buf->send.buf, 0, BUFFER_SIZE*sizeof(uint8));
}

static void *client_rev_thread(void *arg)
{
	uint8 buff[BUFFER_SIZE];
	uint8 proxy_type;
	uint16 host_port = 0;
	int i, id,  ret, fail_times = 0;
	uint32 host_ip = 0;
	fd_set rd_set;
	struct timeval tv;
	BUF_DATA* p_buf;
	THREAD_DATA *pdata;
	int max_link_time;
	int transpro;
	id = *(int*)arg;
	EthCfg *para;
	para=(EthCfg *)sock_thread_data[id].para;
	pdata=&sock_thread_data[id];
	p_buf = &(pdata->p_buf->recv);
	max_link_time=para->RelinkTime;
	transpro=para->TransPro;

PFUNC(TEM_INFO ,DEBUG_COM, "start client thread: %d\n",SOCKET_THD+id);
	
	while(thread_create_ok.count == 0)
	mdelay(5);
	thread_signal(&thread_create_ok);


	need_reconnect[id]=1;
	sock_quit_thread[id]=0;
CREATE_SOCKET_AGAIN:
	g_task->Heart(SOCKET_THD+id);
	if(need_reconnect[id]) {
		init2(id,para);
		fail_times = 0;
		need_reconnect[id] = 0;
	}

CONNECT_AGAIN:
	net_connected[id]=0;
	g_task->Heart(SOCKET_THD+id);

	if (sock_quit_thread[id]) {
		goto out;
	}
	if ( sockfd[id] > 0 ){
			close(sockfd[id]);
			sockfd[id]=-1;
	}
	if(transpro==TCP)
		sockfd[id] = mysocket(AF_INET, SOCK_STREAM, 0);
	else if(transpro==UDP)
		sockfd[id] = mysocket(AF_INET, SOCK_DGRAM, 0);
	else
		return NULL;


	//proxy_type = para->proxycfg.proxy_type;
	proxy_type=0;
	host_ip = get_ulong(para->HostIp);
	host_port = para->HostPort;


	PFUNC(TEM_INFO ,DEBUG_COM, "connect to ip:%x,port:%d\n",host_ip,host_port);
	if ((ret = connect_ms_by_proxy(sockfd[id], proxy_type, host_ip, host_port,NULL)) < 0) {
		g_task->Heart(SOCKET_THD+id);;
		if(ret == -2){
			sleep(1);
			if(++fail_times > MAX_FAIL_TIMES){
				need_reconnect[id] = 1;
				PFUNC(TEM_WARNING ,DEBUG_COM, "!!!no ip, connect fail:i will goto create socket again");
				goto CREATE_SOCKET_AGAIN;
			}
			PFUNC(TEM_WARNING ,DEBUG_COM, "connect fail2:i will goto cconnect again\n");
			goto CONNECT_AGAIN;
		}else {
			sleep(1);
			PFUNC(TEM_WARNING ,DEBUG_COM, "!!!connect fail: \n");
			if(++fail_times > MAX_FAIL_TIMES)
				need_reconnect[id] = 1;
			goto CREATE_SOCKET_AGAIN;
		}
	}

	PFUNC(TEM_INFO ,DEBUG_COM, "!!!connect success: \n");
	net_connected[id]=1;
	fail_times = 0;
	link_time[id]=0;

	reset_queue(id);
	g_task->Heart(SOCKET_THD+id);
	while (1) {
		FD_ZERO(&rd_set);
		FD_SET(sockfd[id], &rd_set);
		tv.tv_sec	= 1;
		tv.tv_usec	= 0;

		g_task->Heart(SOCKET_THD+id);;
		if (sock_quit_thread[id])
			break;
		if (need_reconnect[id]) {
			PFUNC(TEM_WARNING ,DEBUG_COM, "need_reconnect=1 :i will goto create socket again\n");
			goto CREATE_SOCKET_AGAIN;
		}
			ret = select(sockfd[id] + 1, &rd_set, 0, 0, &tv);	//5s
			if ( ret < 0 ) {
				PFUNC(TEM_WARNING ,DEBUG_COM, "!!!select ret = %d\n",  ret );
				//sleep(5);
				goto CREATE_SOCKET_AGAIN;
			} else if (ret == 0) {	//nodata be readed
					if(link_time[id]++>max_link_time)//超过时间没有数据交流，认为连接断开
						{
						PFUNC(TEM_WARNING ,DEBUG_COM, "linktime over %d s :i will goto create socket again\n",max_link_time);
						link_time[id]=0;
						goto CREATE_SOCKET_AGAIN;
					 	}
					 continue;
			}

			if( FD_ISSET(sockfd[id], &rd_set) ) {
				nbytes[id] = 0;
				/* produce data */
				if ( (nbytes[id] = read_socket(sockfd[id], buff, sizeof (buff)) ) < 0) {
					/* disconnet to the net */
					PFUNC(TEM_WARNING ,DEBUG_COM, "i will goto connect again\n");
					//sleep(5);
					goto CONNECT_AGAIN;
				}
				if (nbytes[id] > 0) {
				for (i = 0; i < nbytes[id]; i++) {
					p_buf->buf[p_buf->in_cnt] =  buff[i];
					if (++p_buf->in_cnt >= BUFFER_SIZE)
					 	p_buf->in_cnt = 0;
				}
				link_time[id]=0;
				PFUNC(TEM_INFO ,DEBUG_COM, "recv socket channel:%d Len:%d\n",id,nbytes[id]);	
				//dump_binary((uint8*)buff,nbytes[id]);
			}
		} 
	}
out:	
	g_task->UnRegist(SOCKET_THD+id);
	close(sockfd[id]);	
	PFUNC(TEM_ERROR ,DEBUG_COM, "exit client thread:%d\n",SOCKET_THD+id);	
	return NULL;
}



static void *server_tcp_rev_thread(void *arg)
{
	uint8 buff[BUFFER_SIZE];
	uint16 local_port=0;
	uint32 local_ip=0;
	int i, id,  ret ;
	fd_set rd_set;
	struct timeval tv;
	BUF_DATA* p_buf;
	THREAD_DATA *pdata;
	fd_set ser_rd_set;
	int max_link_time;
  

  	int new_fd = -1;
	char buf[64];
	struct sockaddr_in new_addr;    // connector's address information
	socklen_t sin_size;
	int yes = 1;
	struct sockaddr_in server_addr;  
	int backlog=20;	
	int tmp;
	
	id = *(int*)arg;
	EthCfg *para;
	para=(EthCfg *)sock_thread_data[id].para;
	pdata=&sock_thread_data[id];
	p_buf = &(pdata->p_buf->recv);
	max_link_time=para->RelinkTime;
	
	local_ip = get_ulong(para->LocalIp);
	local_port = para->LocalPort;	

	PFUNC(TEM_INFO ,DEBUG_COM, "start server thread: %d\n",SOCKET_THD+id);
	
	while(thread_create_ok.count == 0)
	mdelay(5);
	thread_signal(&thread_create_ok);
	
	net_connected[id]=0;
	need_reconnect[id]=1;
	sock_quit_thread[id]=0;

CREATE_SOCKET_AGAIN:
	g_task->Heart(SOCKET_THD+id);
	if(need_reconnect[id]) {
		init2(id,para);
		need_reconnect[id] = 0;
	}

	if ( server_fd[id] > 0 )
		close(server_fd[id]);
	server_fd[id] = mysocket(AF_INET, SOCK_STREAM, 0);

    if (  server_fd [id]< 0 ) {
        PFUNC(TEM_ERROR ,DEBUG_COM,"socket failed...\n" );
        goto err;
    }

    //配置侦听Socket
    //SO_REUSEADDR BOOL 允许套接口和一个已在使用中的地址捆绑。
    if ( setsockopt ( server_fd[id], SOL_SOCKET, SO_REUSEADDR , &yes, sizeof ( int ) ) == -1 ) {
        PFUNC(TEM_ERROR ,DEBUG_COM, "setsockopt SO_REUSEADDR error!\n" );
        goto err;
    }

    tmp = fcntl ( server_fd[id], F_GETFL, 0 );
    fcntl ( server_fd[id], F_SETFL,tmp | O_NONBLOCK );	  
	bzero(&server_addr, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(local_port);
	//memcpy(&server_addr.sin_addr, &local_ip, 4);  
	server_addr.sin_addr.s_addr =htonl(INADDR_ANY); 
	sin_size=sizeof ( sockaddr_in ); 

    ret = bind ( server_fd[id], ( struct sockaddr * ) & server_addr, sin_size );

    if ( ret < 0 ) {
       PFUNC(TEM_ERROR ,DEBUG_COM,"bind local_ip:%x,local_port:%d failed: errno:%d %d ...\n",local_ip,local_port ,errno,server_fd[id]);
       perror("bind error");
       goto err;
    }
    //listen the server
    ret = listen ( server_fd[id], backlog );
    if ( ret < 0 ) {
        PFUNC(TEM_ERROR ,DEBUG_COM,"listen failed...\n" );
        goto err;
    }

CONNECT_AGAIN:
	g_task->Heart(SOCKET_THD+id);
	if(need_reconnect[id]) {
		init2(id,para);
		need_reconnect[id] = 0;
	}	
	net_connected[id]=0;
	if (sock_quit_thread[id]){
		goto out;
	}

	if(sockfd[id]>0){
		FD_CLR ( sockfd[id], &rd_set );
		close(sockfd[id]);
		sockfd[id]=-1;
	}

    FD_ZERO ( &ser_rd_set );
    FD_SET ( server_fd[id], &ser_rd_set );

    if ( FD_ISSET ( server_fd[id], &ser_rd_set ) ) {
        //接受连接
         new_fd = accept ( server_fd[id], ( struct sockaddr * ) & new_addr, &sin_size );
        if ( new_fd <= 0 ) {
            //perror ( "accept socket error!" );
            mdelay(10);
            goto CONNECT_AGAIN;
        }

        sockfd[id]=new_fd;
        sprintf ( buf, "connected to Ftu is OK\n");
        send ( new_fd, buf, strlen ( buf ), 0 );

        PFUNC ( TEM_INFO ,DEBUG_COM, "ser-s=%d,new-s=%d\n", server_fd[id], new_fd );
	}
	else
		{
			mdelay(10);
			goto CONNECT_AGAIN;
		}
	//	PFUNC(TEM_WARNING ,DEBUG_COM, "!!!after connect_ms_by_proxy:%d\n",(int32)get_ticket(NULL));
	PFUNC(TEM_INFO ,DEBUG_COM, "!!!connect success: \n");
	net_connected[id]=1;
	reset_queue(id);
	link_time[id]=0;

	while (1) {
		g_task->Heart(SOCKET_THD+id);
			
	    FD_ZERO ( &ser_rd_set );
	    FD_SET ( server_fd[id], &ser_rd_set );
	
	    if ( FD_ISSET ( server_fd[id], &ser_rd_set ) ) {
	        //接受连接
	         new_fd = accept ( server_fd[id], ( struct sockaddr * ) & new_addr, &sin_size );
	        if ( new_fd > 0 ) {
	        	close(sockfd[id]);//连接后，再有连接进来关闭以前连接
		        sockfd[id]=new_fd;
		        link_time[id]=0;
	  	      	sprintf ( buf, "connected to Ftu is OK\n");
	    	    send ( new_fd, buf, strlen ( buf ), 0 );
	    	    PFUNC ( TEM_WARNING ,DEBUG_COM, "reconnect:ser-s=%d,new-s=%d\n", server_fd[id], new_fd );
	        }         
	    }
		if (sock_quit_thread[id])
			break;
		if (need_reconnect[id]) {
			PFUNC(TEM_WARNING ,DEBUG_COM, "!!!need_reconnect = %d\n", need_reconnect[id] );
			goto CONNECT_AGAIN;
		}
			
		tv.tv_sec	= 1;
		tv.tv_usec	= 0;

		FD_ZERO(&rd_set);
		FD_SET(sockfd[id], &rd_set);
			
		ret = select(sockfd[id] + 1, &rd_set, 0, 0, &tv);	//5s
		if ( ret < 0 ) {
			PFUNC(TEM_WARNING ,DEBUG_COM, "!!!select ret = %d\n", ret );
				//sleep(1);
				goto CONNECT_AGAIN;	
		} else if (ret == 0) {	//nodata be readed
				if(link_time[id]++>max_link_time){//超过时间没有数据交流，认为连接断开
					link_time[id]=0;
					PFUNC(TEM_WARNING ,DEBUG_COM, "linktime over %d s :i will goto connect socket again\n",max_link_time);
					goto CREATE_SOCKET_AGAIN;
				}
				continue;
		}
	
		if( FD_ISSET(sockfd[id], &rd_set) ) {
			nbytes[id] = 0;
			// produce data 
			if ( (nbytes[id] = read_socket(sockfd[id], buff, sizeof (buff)) ) < 0) {
				//disconnet to the net 
				PFUNC(TEM_WARNING ,DEBUG_COM, "!!!disconnect, i will goto connect_again\n");//断开连接这里返回
				//sleep(1);
				goto CONNECT_AGAIN;	
			}
			if (nbytes[id] > 0) {
				for (i = 0; i < nbytes[id]; i++) {
					p_buf->buf[p_buf->in_cnt] =  buff[i];
					if (++p_buf->in_cnt >= BUFFER_SIZE)
					p_buf->in_cnt = 0;
				}
				PFUNC(TEM_INFO ,DEBUG_COM, "recv socket channel:%d Len:%d\n",id,nbytes[id]);	
				//dump_binary((uint8*)buff,nbytes[id]);
				link_time[id]=0;
	
			}
		} 
	}

out:g_task->UnRegist(SOCKET_THD+id);
err:	
	close(sockfd[id]);	
	close(server_fd[id]);
	PFUNC(TEM_ERROR ,DEBUG_COM, "exit server thread:%d\n",SOCKET_THD+id);		

	return NULL;
}

static void *server_udp_rev_thread(void *arg)
{
	uint8 buff[BUFFER_SIZE];
	THREAD_DATA *pdata;
	fd_set rd_set;
	
	uint16 local_port=0;
	uint32 local_ip=0;
	int i, id,  ret ;
	struct timeval tv;
	BUF_DATA* p_buf;
	int max_link_time;
	socklen_t sin_size;
	int yes = 1;
	struct sockaddr_in server_addr;  
	struct sockaddr_in client_addr;  
	int tmp;
	
	id = *(int*)arg;
	EthCfg *para;
	para=(EthCfg *)sock_thread_data[id].para;
	pdata=&sock_thread_data[id];
	p_buf = &(pdata->p_buf->recv);
	max_link_time=para->RelinkTime;
	
	local_ip = get_ulong(para->LocalIp);
	local_port = para->LocalPort;	

	PFUNC(TEM_INFO ,DEBUG_COM, "start server thread: %d\n",SOCKET_THD+id);
	
	while(thread_create_ok.count == 0)
	mdelay(5);
	thread_signal(&thread_create_ok);
	
	net_connected[id]=0;
	need_reconnect[id]=1;
	sock_quit_thread[id]=0;

CREATE_SOCKET_AGAIN:
	g_task->Heart(SOCKET_THD+id);
	if(need_reconnect[id]) {
		init2(id,para);
		need_reconnect[id] = 0;
	}

	if ( server_fd[id] > 0 )
		close(server_fd[id]);
		server_fd[id] = mysocket(AF_INET, SOCK_DGRAM, 0);

    if (  server_fd [id]< 0 ) {
        PFUNC(TEM_ERROR ,DEBUG_COM,"socket failed...\n" );
        goto err;
    }

    //配置侦听Socket
    //SO_REUSEADDR BOOL 允许套接口和一个已在使用中的地址捆绑。
    if ( setsockopt ( server_fd[id], SOL_SOCKET, SO_REUSEADDR , &yes, sizeof ( int ) ) == -1 ) {
        PFUNC(TEM_ERROR ,DEBUG_COM, "setsockopt SO_REUSEADDR error!\n" );
        goto err;
    }

    tmp = fcntl ( server_fd[id], F_GETFL, 0 );
    fcntl ( server_fd[id], F_SETFL,tmp | O_NONBLOCK );	  
	bzero(&server_addr, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(local_port);
	//memcpy(&server_addr.sin_addr, &local_ip, 4);  
	server_addr.sin_addr.s_addr =htonl(INADDR_ANY); 
	sin_size=sizeof ( sockaddr_in ); 

    ret = bind ( server_fd[id], ( struct sockaddr * ) & server_addr, sin_size );

    if ( ret < 0 ) {
       PFUNC(TEM_ERROR ,DEBUG_COM,"bind local_ip:%x,local_port:%d failed: errno:%d %d ...\n",local_ip,local_port ,errno,server_fd[id]);
       perror("bind error");
       goto err;
    }

	sockfd[id]=server_fd[id];
	while (1) {
		g_task->Heart(SOCKET_THD+id);
		if (sock_quit_thread[id]){
			break;
		}		
		tv.tv_sec	= 1;
		tv.tv_usec	= 0;

		FD_ZERO(&rd_set);
		FD_SET(sockfd[id], &rd_set);
			
		ret = select(sockfd[id] + 1, &rd_set, 0, 0, &tv);	//5s

		if ( ret < 0 ) {
			PFUNC(TEM_WARNING ,DEBUG_COM, "!!!select ret = %d\n", ret );
				//sleep(1);
				goto CREATE_SOCKET_AGAIN;	
		} else if (ret == 0) {	//nodata be readed
				if(link_time[id]++>max_link_time){//超过时间没有数据交流，认为连接断开
					link_time[id]=0;
					PFUNC(TEM_WARNING ,DEBUG_COM, "linktime over %d s :i will goto connect socket again\n",max_link_time);
					goto CREATE_SOCKET_AGAIN;
				}
				continue;
		}
	
		if( FD_ISSET(sockfd[id], &rd_set) ) {
			nbytes[id] = 0;
			// produce data 
			if ( (nbytes[id] = recvfrom(sockfd[id], buff, sizeof (buff),0,(struct sockaddr*)&client_addr,&sin_size)) < 0) {
				//disconnet to the net 
				PFUNC(TEM_WARNING ,DEBUG_COM, "!!!disconnect, i will goto connect_again\n");//断开连接这里返回
				//sleep(1);
				goto CREATE_SOCKET_AGAIN;	
			}
			if (nbytes[id] > 0) {
				for (i = 0; i < nbytes[id]; i++) {
					p_buf->buf[p_buf->in_cnt] =  buff[i];
					if (++p_buf->in_cnt >= BUFFER_SIZE)
					p_buf->in_cnt = 0;
				}
				PFUNC(TEM_INFO ,DEBUG_COM, "recv socket channel:%d Len:%d\n",id,nbytes[id]);	
				//dump_binary((uint8*)buff,nbytes[id]);
				link_time[id]=0;
	
			}
		} 
	}

	g_task->UnRegist(SOCKET_THD+id);
err:	
	close(sockfd[id]);	
	close(server_fd[id]);
	PFUNC(TEM_ERROR ,DEBUG_COM, "exit server thread:%d\n",SOCKET_THD+id);		

	return NULL;
}

int SOCKET::create_socket(int channel)
{
	int ret;
	int mode;
	int transpro;
	EthCfg*para;
	
	para=(EthCfg*)sock_thread_data[channel].para;
	mode=para->Mode;
	transpro=para->TransPro;
	
	sockfd[channel]=-1;
	g_task->UnRegist(SOCKET_THD+channel);
	g_task->Regist(SOCKET_THD+channel);

	if(mode==SERVER_MODE)//服务器
	{
		thread_create_ok.count = 0;
		server_fd[channel]=-1;
		PFUNC(TEM_INFO ,DEBUG_COM, "create server thread %d\n",SOCKET_THD+channel);		
		if(transpro==TCP)
			ret = pthread_create(&sock_tid[channel], NULL, server_tcp_rev_thread, (void *)&channel);
		else if(transpro==UDP)
			ret = pthread_create(&sock_tid[channel], NULL, server_udp_rev_thread, (void *)&channel);
		else{
			PFUNC(TEM_WARNING ,DEBUG_COM, "thread %d transpro is: %d is error \n",SOCKET_THD+channel,transpro);		
			return -1;
		}
		thread_create_ok.count = 1;
		thread_wait_cond(&thread_create_ok);
		return ret;			

	}
	else if(mode==CLIENT_MODE)//客户端
	{
		thread_create_ok.count = 0;
		PFUNC(TEM_INFO ,DEBUG_COM, "create client thread %d\n",SOCKET_THD+channel);			
		ret = pthread_create(&sock_tid[channel], NULL, client_rev_thread, (void *)&channel);
		thread_create_ok.count = 1;
		thread_wait_cond(&thread_create_ok);
		return ret;	
	}	
return -1;	
}


/*****************************************************************************
																	public fun
******************************************************************************/
int SOCKET::up(void* parameter)
{
	uint32 local_ip, subnet_mask, gate_ip;
	uint8 *mac;
	EthCfg *para;
	para=(EthCfg*)parameter;
	set_interface_name(para->InterfaceName);
	local_ip = get_ulong(para->LocalIp);
	subnet_mask =get_ulong(para->Mask);
	gate_ip = get_ulong(para->Gate);
	mac = para->Mac;

	interface_up((char *)para->InterfaceName);
	sleep(1);
	if (set_mac(mac) < 0) {
		//return -1;
	}
		
	if(local_ip!=0){
		if ( set_ip(local_ip) < 0) {
			return -1;
		}
	}
	
	if(subnet_mask!=0){
		if ( set_subnet_mask(subnet_mask) < 0 ) {
			return -1;
		}
	}
	
	return 1;

//	if ( set_gate_way(gate_ip) < 0 ) {
//		return -1;
//	}	
}
int SOCKET::down(void* parameter)
{
	EthCfg*cfg=(EthCfg*)parameter;
	interface_down((char *)cfg->InterfaceName);
	return 1;
	
}
int SOCKET::init(int port,void*parameter)
{
	int channel;
	EthCfg *para;
	
	channel=get_channel(port);
	
	if (channel < 0 || channel > MAX_SOCK_NUM) {
		return -1;
	}	
	para=(EthCfg*)parameter;
	set_interface_name(para->InterfaceName);
	init2(channel,parameter);
	
	PFUNC(TEM_INFO ,DEBUG_COM, "init port %d channel %d interfacename %s\n",port,channel,para->InterfaceName);			
	return create_socket(channel);	

}
int init2(int channel,void*parameter)
{
	EthCfg *para;
	para=(EthCfg*)parameter;

	sock_thread_data[channel].fd_index = channel;
	sock_thread_data[channel].p_buf = para->Buff;
	sock_thread_data[channel].para = parameter;
	
	sock_thread_data[channel].p_buf->recv.in_cnt = 0;
	sock_thread_data[channel].p_buf->recv.out_cnt = 0;
	sock_thread_data[channel].p_buf->send.in_cnt = 0;
	sock_thread_data[channel].p_buf->send.out_cnt = 0;
	memset(sock_thread_data[channel].p_buf->recv.buf, 0, BUFFER_SIZE*sizeof(uint8));
	memset(sock_thread_data[channel].p_buf->send.buf, 0, BUFFER_SIZE*sizeof(uint8));

	return 0;
}

int SOCKET::sendbuf(int port, uint8* buffer, int length)
{
	int    bytes_left;
	int    written_bytes;
	char    *ptr;
	int channel;
	written_bytes=0;

	channel=get_channel(port);
	if (channel < 0 || channel > MAX_SOCK_NUM) {
		return -1;
	}		

	if(!net_connected[channel])
		return -1;


	ptr=(char *)buffer;
	bytes_left=length;
	while(bytes_left>0)
	{
		written_bytes=write(sockfd[channel],ptr,bytes_left);
		PFUNC(TEM_INFO ,DEBUG_COM, "send len:%d ret = %d  \n", length,written_bytes);
		if(written_bytes<=0)
		{
			if(errno==EINTR)
				written_bytes=0;
			else
				return(-1);
		}
		bytes_left-=written_bytes;
		ptr+=written_bytes;
	}
	return(0);
}
//复位通道函数
int SOCKET::reset(int port)
{
	int channel;
	channel=get_channel(port);
	if (channel < 0 || channel > MAX_SOCK_NUM) {
		return 0;
	}
	need_reconnect[channel]=1;	
	return 1;
}	
//判断网络掉线
int SOCKET::checkstate(int port)
{
	int channel;
	channel=get_channel(port);
	if (channel < 0 || channel > MAX_SOCK_NUM) {
		return 0;
	}
	return net_connected[channel];
}

int SOCKET::close_port(int port)
{
	int id;
	id=get_channel(port);
	sock_quit_thread[id] = 1;
	if (sock_tid[id] != 0) {
		pthread_join(sock_tid[id], NULL);
		sock_tid[id] = 0;
		return 1;
	}
	return 0;
}

int check_link(char* interface_name)
{
	int inet_sock,ret;
	struct ifreq ifr;

	ret=0;
	inet_sock = mysocket(AF_INET, SOCK_STREAM, 0);
	strcpy(ifr.ifr_name, interface_name);
	if (ioctl(inet_sock, SIOCGIFFLAGS, &ifr)==0) {
				if (ifr.ifr_flags & IFF_RUNNING) {	
					close(inet_sock);
					ret=0;
					}
				else{
					ret=-1;
					PFUNC(TEM_WARNING ,DEBUG_COM, "%s LINK DISCONNECT\n",interface_name);
				}	
	}
	close(inet_sock);
	return ret;
}


uint32 get_local_ip(char *interface_name)
{
	uint32 ip;
	unsigned char *p;
	int inet_sock;
	unsigned int v1, v2, v3, v4;
	struct ifreq ifr;

	p = (unsigned char *)&ip;
	inet_sock = mysocket(AF_INET, SOCK_STREAM, 0);
	strcpy(ifr.ifr_name, interface_name);
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr,sizeof(struct ifreq))<0) {
		close(inet_sock);
		return 0;
	}
	sscanf(inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr), "%u.%u.%u.%u", &v1, &v2, &v3, &v4);

	*p = (unsigned char)v1;
	*(p + 1) = (unsigned char)v2;
	*(p + 2) = (unsigned char)v3;
	*(p + 3) = (unsigned char)v4;

	close(inet_sock);
	return ip;
}
static void get_dev_fields(char *bp, struct net_interface *ife)
{
	sscanf(bp,
	       "%llu %llu %lu %lu %lu %lu %lu %lu %llu %llu %lu %lu %lu %lu %lu %lu",
	       &ife->rx_bytes, &ife->rx_packets,
	       &ife->rx_errors, &ife->rx_dropped,
	       &ife->rx_fifo_errors, &ife->rx_frame_errors,
	       &ife->rx_compressed, &ife->rx_multicast,

	       &ife->tx_bytes, &ife->tx_packets,
	       &ife->tx_errors, &ife->tx_dropped,
	       &ife->tx_fifo_errors, &ife->collisions,
	       &ife->tx_carrier_errors, &ife->tx_compressed
	       );
}

uint32 SOCKET::get_trans_bytes(char * interface_name,int8 flag){

	struct net_interface ife;
	char *str;
	int ret;
	char buf[1024],name[32];//注意：这个是从文本中分析数据，如果接口数量多，可能会超过1024，目前是550多字节，包括lo，eth0，ppp0。
	unsigned long long ife_bytes = 0;
	
 	memset(buf,0,1024);
 	ret=read_file(_PATH_PROCNET_DEV,buf,1024,"rt",0);
  if(ret>=0){
  		sprintf(name,"%s:",interface_name);
  		str=str_find(buf,name);
  		if(str==NULL) return 0;
  		str+=strlen(name);
			get_dev_fields(str, &ife);
			if(flag==GET_TX_BYTES)
			ife_bytes = ife.tx_bytes/1024;
			if(flag==GET_RX_BYTES)
			ife_bytes = ife.rx_bytes/1024 ;
			if(flag==GET_TOTAL_BYTES)
			ife_bytes = (ife.rx_bytes + ife.tx_bytes)/1024;
			return ife_bytes;
  }
  return 0;

}
