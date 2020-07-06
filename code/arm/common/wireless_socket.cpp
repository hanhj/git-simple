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
#include "wireless_socket.h"
#include "os.h"
#include "com.h"
#include "utils.h"
#include "task.h"
#include "proxyclient.h"
#include "gpio.h"
#include "device.h"

static int server_fd[MAX_SOCK_NUM];
static int 	init2(int channel,void *parameter);//for thread
//int localip;
static void start_gprs( void )
{	
	fprintf(stderr,"start module\r\n");
  gpio.Write(WIRELESS_POWER, 1);
  gpio.Write(WIRELESS_ON_OFF, 1);
  usleep(30*1000);//need 30ms
  gpio.Write(WIRELESS_ON_OFF, 0);
  usleep(1000*1000);//need >1s
  gpio.Write(WIRELESS_ON_OFF, 1);

}

static void start_muxd(){
	char buf[100];
	sprintf(buf,"gsmMuxd -p %s -b 57600 -w  -r -s /dev/mux /dev/ptmx /dev/ptmx /dev/ptmx ",UART9_DEV);
	system(buf);	
//#define MUX_PATH "/usr/sbin/gsmMuxd"
//#define MUX_CMD "gsmMuxd"
//#define MUX_ARG1 "-p"
//#define MUX_ARG2 "/dev/ttyS1"
//#define MUX_ARG3 "-w"
//#define MUX_ARG4 "-r"
//#define MUX_ARG5 "-s"
//#define MUX_ARG6 "/dev/mux"
//#define MUX_ARG7 "/dev/ptmx"
//#define MUX_ARG8 "/dev/ptmx"
//#define MUX_ARG9 "/dev/ptmx"
//	pid_t pid;
//	if ((pid = fork()) == 0) {
//		dup2(STDERR_FILENO, STDOUT_FILENO);
//		execl(MUX_PATH, MUX_CMD, MUX_ARG1, MUX_ARG2,MUX_ARG3,MUX_ARG4,MUX_ARG5,MUX_ARG6,MUX_ARG7,MUX_ARG8,MUX_ARG9, NULL);
//		PFUNC(TEM_WARNING, DEBUG_NORMAL,"error, mux cant be executed\n");
//		exit(-1);
//	} else if (pid > 0) {
//		//pppd_pid = pid;
//	}	
}

static void start_pppd(){
system("pppd call wireless &");	
//#define PPPD_PATH "/usr/sbin/pppd"
//#define PPPD_CMD "pppd"
//#define PPPD_ARG1 "call"
//#define PPPD_ARG2 "wireless "
//	pid_t pid;
//	if ((pid = fork()) == 0) {
//		dup2(STDERR_FILENO, STDOUT_FILENO);
//		execl(PPPD_PATH, PPPD_CMD, PPPD_ARG1, PPPD_ARG2, NULL);
//		PFUNC(TEM_WARNING, DEBUG_NORMAL,"error, pppd cant be executed\n");
//		exit(-1);
//	} else if (pid > 0) {
//		//pppd_pid = pid;
//	}	
}


static void stop_pppd(){

	system("killall -q -9 pppd");
	system("killall -q -9 chat");
}

static void stop_muxd(){

	system("killall -q -9 gsmMuxd");
}

static void stop_gprs( void )
{	
	fprintf(stderr,"stop module\r\n");
  gpio.Write(WIRELESS_ON_OFF, 0);
  usleep(800*1000);//need 0.6~1s
  gpio.Write(WIRELESS_ON_OFF, 1);
  usleep(3000*1000);//need 2~12s
  gpio.Write(WIRELESS_POWER, 0);

}
int start_gprs_flag;
static void user_reset_link(void)
{
		stop_pppd();
		stop_muxd();
		stop_gprs();
		sleep(2);
		start_gprs();
		sleep(2);
		start_muxd();
		sleep(2);
		start_pppd();
		start_gprs_flag=1;

}

static void user_close_link(void)
{
		stop_pppd();
		stop_muxd();
		stop_gprs();
}

static void *client_rev_thread(void *arg)
{
	uint8 buff[BUFFER_SIZE];
	Data<BaseDataType> data_para;
	WirelessPara*wirelesspara;	
	uint8 proxy_type;
	uint16 host_port = 0;
	int i, id,  ret, fail_times = 0;
	uint32 host_ip = 0;
	uint32 local_ip = 0;
	fd_set rd_set;
	struct timeval tv;
	BUF_DATA* p_buf;
	THREAD_DATA *pdata;
	int max_link_time;
	id = *(int*)arg;
	WireLessCfg *para;
	para=(WireLessCfg *)sock_thread_data[id].para;
	pdata=&sock_thread_data[id];
	p_buf = &(pdata->p_buf->recv);
	max_link_time=para->RelinkTime;
  user_data.GetData(PARA_5106,&data_para);
  wirelesspara=(WirelessPara*)data_para.Data;	

PFUNC(TEM_INFO ,DEBUG_COM, "start client thread: %d\n",SOCKET_THD+id);
	
	while(thread_create_ok.count == 0)
	mdelay(5);
	thread_signal(&thread_create_ok);


	need_reconnect[id]=1;
	sock_quit_thread[id]=0;
CREATE_SOCKET_AGAIN:
	g_task->Heart(SOCKET_THD+id);
	if(need_reconnect[id]) {
		if(para->isMaster){
			user_reset_link();
		}
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
	sockfd[id] = mysocket(AF_INET, SOCK_STREAM, 0);
	//proxy_type = para->proxycfg.proxy_type;
	proxy_type=0;
	host_ip = get_ulong(para->HostIp);
	//host_ip = localip;
	host_port = para->HostPort;
	local_ip=get_local_ip((char *)PPP_NAME);
	put_ulong(local_ip,(uint8*)&wirelesspara->Data.LocalIp[0]);	

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
	g_task->Heart(SOCKET_THD+id);;
	while (1) {
		FD_ZERO(&rd_set);
		FD_SET(sockfd[id], &rd_set);
		tv.tv_sec	= 1;
		tv.tv_usec	= 0;

		g_task->Heart(SOCKET_THD+id);
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
				if(check_link((char*)PPP_NAME) < 0) {
					PFUNC(TEM_WARNING ,DEBUG_COM, "checknet err :i will goto create socket again\n");
					goto CREATE_SOCKET_AGAIN;
				} else {
					if(link_time[id]++>max_link_time)//超过时间没有数据交流，认为连接断开
						{
						PFUNC(TEM_WARNING ,DEBUG_COM, "linktime over %d s :i will goto create socket again\n",max_link_time);
						link_time[id]=0;
						goto CREATE_SOCKET_AGAIN;
						}
					continue;
				}
			}

			if( FD_ISSET(sockfd[id], &rd_set) ) {
				nbytes[id] = 0;
				/* produce data */
				if ( (nbytes[id] = read_socket(sockfd[id], buff, sizeof (buff)) ) < 0) {
					/*disconnet to the net */
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
	if(para->isMaster){
			user_close_link();
		}	
	PFUNC(TEM_WARNING ,DEBUG_COM, "exit client thread:%d\n",SOCKET_THD+id);	
	return NULL;
}

static void *server_rev_thread(void *arg)
{
	uint8 buff[BUFFER_SIZE];
	Data<BaseDataType> data_para;
	WirelessPara*wirelesspara;
//	uint8 proxy_type;
//	uint16 host_port = 0,
	uint16 local_port=0;
//	uint32 host_ip = 0,
	uint32 local_ip=0;
	int i, id,  ret ,fail_times=0;
	fd_set rd_set;
	struct timeval tv;
	BUF_DATA* p_buf;
	WireLessCfg *para;
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
	para=(WireLessCfg *)sock_thread_data[id].para;
	pdata=&sock_thread_data[id];
	p_buf = &(pdata->p_buf->recv);
	max_link_time=para->RelinkTime;
	

  	user_data.GetData(PARA_5106,&data_para);
  	wirelesspara=(WirelessPara*)data_para.Data;
	//proxy_type = para->proxycfg.proxy_type;
	//host_ip = get_ulong(para->HostIp);
	//host_port = para->HostPort;	
	//local_ip = get_ulong(para->LocalIp);
	local_ip=0;
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
		if(para->isMaster){
		user_reset_link();
		}
		init2(id,para);
		need_reconnect[id] = 0;
	}

	if ( server_fd[id] > 0 )
		close(server_fd[id]);
	server_fd[id] = mysocket(AF_INET, SOCK_STREAM, 0);

    if (  server_fd[id] < 0 ) {
        PFUNC(TEM_WARNING ,DEBUG_COM,"socket failed...\n" );
        goto err;
    }

    //配置侦听Socket
    //SO_REUSEADDR BOOL 允许套接口和一个已在使用中的地址捆绑。
    if ( setsockopt ( server_fd[id], SOL_SOCKET, SO_REUSEADDR , &yes, sizeof ( int ) ) == -1 ) {
        PFUNC(TEM_WARNING ,DEBUG_COM, "setsockopt SO_REUSEADDR error!\n" );
        goto err;
    }

	local_ip=get_local_ip((char *)PPP_NAME);
	//localip=local_ip;
	put_ulong(local_ip,(uint8*)&wirelesspara->Data.LocalIp[0]);	

	PFUNC(TEM_INFO ,DEBUG_COM,"local ip:%x \n",local_ip);
	if(local_ip==0){
		sleep(1)	;
		PFUNC(TEM_WARNING ,DEBUG_COM, "fail_times:%d\n",fail_times );
		if(++fail_times > MAX_FAIL_TIMES){
			need_reconnect[id] = 1;
			fail_times=0;
		}
		goto CREATE_SOCKET_AGAIN;
	}
    tmp = fcntl ( server_fd[id], F_GETFL, 0 );
    fcntl ( server_fd[id], F_SETFL,tmp | O_NONBLOCK );	  
	bzero(&server_addr, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(local_port);
	memcpy(&server_addr.sin_addr, &local_ip, 4);  
	//server_addr.sin_addr.s_addr =htonl(INADDR_ANY); 
	sin_size=sizeof ( sockaddr_in ); 

    ret = bind ( server_fd[id], ( struct sockaddr * ) & server_addr, sin_size );

    if ( ret < 0 ) {
       PFUNC(TEM_WARNING ,DEBUG_COM,"bind local_ip:%x,local_port:%d failed: errno:%d %d ...\n",local_ip,local_port ,errno,server_fd[id]);
       perror("bind error");
       goto err;
    }
    //listen the server
    ret = listen ( server_fd[id], backlog );
    if ( ret < 0 ) {
        PFUNC(TEM_WARNING ,DEBUG_COM,"listen failed...\n" );
        goto err;
    }

	fail_times=0;		
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
			
	if(check_link((char*)PPP_NAME)<0){//检查网络是否断开
		goto CREATE_SOCKET_AGAIN;	
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
	else{
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
				if(check_link((char*)PPP_NAME) < 0) {
				goto CREATE_SOCKET_AGAIN;	
				} else {
					if(link_time[id]++>max_link_time){//超过时间没有数据交流，认为连接断开
						PFUNC(TEM_WARNING ,DEBUG_COM, "linktime over %d s :i will goto connect socket again\n",max_link_time);
						link_time[id]=0;
						goto CONNECT_AGAIN;
					}
					continue;
				}
		}

		if( FD_ISSET(sockfd[id], &rd_set) ) {
			nbytes[id] = 0;
			/* produce data */
			if ( (nbytes[id] = read_socket(sockfd[id], buff, sizeof (buff)) ) < 0) {
				/*disconnet to the net */
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
	if(para->isMaster){
			user_close_link();
		}	

	PFUNC(TEM_WARNING ,DEBUG_COM, "exit server thread:%d\n",SOCKET_THD+id);		

	return NULL;
}
int WIRELESS_SOCKET::up(void *parameter)
{
	parameter=parameter;
	gpio.Init(WIRELESS_ON_OFF,GPIO_DIR_OUT,0);	
	gpio.Init(WIRELESS_POWER,GPIO_DIR_OUT,0);	
	user_reset_link	();
	return 1;
	
}
int WIRELESS_SOCKET::down(void *parameter)
{
	parameter=parameter;
	user_close_link();
	return 1;
}
int WIRELESS_SOCKET::init(int port,void*parameter)
{
	int channel;

	channel=get_channel(port);
	PFUNC(TEM_INFO ,DEBUG_COM, "port %d channel %d\n",port,channel);			
	set_interface_name((char*)PPP_NAME);
	init2(channel,parameter);
	return create_socket(channel);	

}
int init2(int channel,void*parameter)
{
	WireLessCfg *para;
	para=(WireLessCfg*)parameter;

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

int WIRELESS_SOCKET::create_socket(int channel)
{
	int ret;
	int mode;
	WireLessCfg*para;
	
	para=(WireLessCfg*)sock_thread_data[channel].para;
	mode=para->Mode;
	sockfd[channel]=-1;
	if(mode==SERVER_MODE)//服务器
	{
		thread_create_ok.count = 0;
		server_fd[channel]=-1;
		PFUNC(TEM_INFO ,DEBUG_COM, "create server thread %d\n",SOCKET_THD+channel);		
		g_task->Regist(SOCKET_THD+channel);	
		ret = pthread_create(&sock_tid[channel], NULL, server_rev_thread, (void *)&channel);
		thread_create_ok.count = 1;
		thread_wait_cond(&thread_create_ok);
		return ret;			
	}
	else if(mode==CLIENT_MODE)//客户端
	{
		thread_create_ok.count = 0;
		PFUNC(TEM_INFO ,DEBUG_COM, "create client thread %d\n",SOCKET_THD+channel);			
		g_task->Regist(SOCKET_THD+channel);
		ret = pthread_create(&sock_tid[channel], NULL, client_rev_thread, (void *)&channel);
		thread_create_ok.count = 1;
		thread_wait_cond(&thread_create_ok);
		return ret;	
	}	
return -1;	
}

