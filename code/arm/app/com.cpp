#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "datatype.h"
#include "os.h"// for mutex
#include "debug.h"
#include "message.h"
#include "com.h"
#include "task.h"
#include "protocol.h"
#include "localtime.h"
#include "utils.h"
#include "uart.h"
#include "socket.h"
#include "wireless_socket.h"
#include "wireless_uart.h"
#include "little_wireless_uart.h"
#include "timer.h"
#include "device.h"
#include "sntp.h"

#define COMRUNDATA RTDATA_ROOT"comrun.dat"

static UART uart;
static SOCKET eth_socket;
static WIRELESS_SOCKET wireless_socket;
static WIRELESS_UART wireless_uart;
static LITTLE_WIRELESS_UART little_wireless_uart;
Comm comm;
/*
		for uart common funtion
*/
static int 	uart_init(int port,void *parameter)
{
	return uart.init(port,parameter);
}
static int 	uart_reset(int port)
{
	return uart.reset(port);
}
static int 	uart_sendbuf(int port, uint8 *buf, int len)
{
	return uart.sendbuf(port, buf,len);
}
static int 	uart_getbyte( int port, uint8 *c )
{
	return uart.getbyte(port, c );
}    

static int 	uart_checkstate( int port )
{
	return uart.checkstate( port );
}    

static int 	uart_close( int port )
{
	return uart.close_port( port );
} 
static int 	wireless_uart_init(int port,void *parameter)
{
	return wireless_uart.init(port,parameter);
}

static int 	little_wireless_uart_init(int port,void *parameter)
{
	return little_wireless_uart.init(port,parameter);
}
static int 	little_wireless_uart_reset(int port)
{
	return little_wireless_uart.reset(port);
}
/*
		for eth common funtion
*/

static int 	socket_init(int port,void *parameter)
{
	return eth_socket.init(port,parameter);
}
static int 	socket_reset(int port)
{
	return eth_socket.reset(port);
}
static int 	socket_sendbuf(int port, uint8 *buf, int len)
{
	return eth_socket.sendbuf(port, buf,len);
}
static int 	socket_getbyte( int port, uint8 *c )
{
	return eth_socket.getbyte(port, c );
}    

static int 	socket_checkstate( int port )
{
	return eth_socket.checkstate( port );
} 
static int 	socket_close( int port )
{
	return eth_socket.close_port( port );
} 

/*
		for wireless common funtion
*/

static int 	wireless_socket_init(int port,void *parameter)
{
	return wireless_socket.init(port,parameter);
}
static int 	wireless_socket_reset(int port)
{
	return wireless_socket.reset(port);
}
static int 	wireless_socket_sendbuf(int port, uint8 *buf, int len)
{
	return wireless_socket.sendbuf(port, buf,len);
}
static int 	wireless_socket_getbyte( int port, uint8 *c )
{
	return wireless_socket.getbyte(port, c );
}    

static int 	wireless_socket_checkstate( int port )
{
	return wireless_socket.checkstate( port );
} 

static int 	wireless_socket_close( int port )
{
	return wireless_socket.close_port( port );
} 



/** /brief需要定义一个全局函数Task类处理该类的入口
*/
void *comm_main ( void *arg )
{
    comm.Run ( arg );
    return NULL;
}

/** 需要定义一个全局函数供注册消息类处理消息
*/
static int16 GetMsg2 ( MessageData<BaseDataType>message )
{
    comm.GetMsg ( message );
    return 1;
}

///////////////公有函数//////////////////////////////////////////////////
Comm::Comm ( void )
{
    strcpy ( name, "Comm" );
    Id = TASK_COM;
}

Comm::~Comm ( void )
{
    PFUNC ( TEM_WARNING,DEBUG_COM, "Destroy Comm\r\n" );
}

int16 Comm::Init()
{
	int i,j;
    Data<BaseDataType> para;
    EthPara *pethpara,ethpara;
    WirelessPara *pwirelesspara,wirelesspara;

    msg.RegistMsg ( Id, GetMsg2 );///<向message类注册线程号以及消息回调函数
    for(i=1;i<MAX_PORT;i++)
    {
    com_cfg.com_use[i]=0;
    com_cfg.com_olduse[i]=0;
    com_cfg.Para[i]=NULL;
    memset(&com_cfg.Buff[i],0,sizeof(BUF_MUX));
    com_cfg.Buff[i].fun_init=NULL;
    }
	memset(&com_cfg.uart_cfg,0,(MAX_UART)*sizeof(com_cfg.uart_cfg[0]));
	memset(&com_cfg.eth_cfg,0,MAX_ETH_SOCK_NUM*sizeof(com_cfg.eth_cfg[0]));
	memset(&com_cfg.wireless_cfg,0,MAX_PPP_SOCK_NUM*sizeof(com_cfg.wireless_cfg[0]));
		
	if(first_run==0){
	thread_wait_cond(&user_data.cond);//等待参数装载完毕		
	first_run=1;
	}

    ReadCfg();
    
    //start or stop any used physical channel
    for(i=0;i<MAX_ETH_NUM;i++)
    {
    	if(com_cfg.eth_para[i].Use){
    		eth_socket.up(&com_cfg.eth_para[i]);
    	}
    	else
    		eth_socket.down(&com_cfg.eth_para[i]);
    }
    
    for(i=0;i<MAX_PPP_NUM;i++)
    {
	    if(com_cfg.wireless_para[i].Use)
	    	wireless_socket.up(&com_cfg.wireless_para[i]);
	    else
	    	wireless_socket.down(&com_cfg.wireless_para[i]);
    }
    
	//init any used port,从1开始0不用，因为所定义的端口号从1开始
    for(i=1;i<MAX_PORT;i++)
    {
    	if(com_cfg.com_use[i]==1)
    		{
    			if(com_cfg.Buff[i].fun_init!=NULL){
    				com_cfg.com_olduse[i]=1;
    				com_cfg.Buff[i].fun_init(i,com_cfg.Para[i]);
    			}
    		}
    }
    
    //init physical channel para,for update channel flow and link state date.
    for(i=0;i<MAX_ETH_NUM;i++){
		user_data.GetData(PARA_5102+i,&para);	
		pethpara=(EthPara*)para.Data;
		memcpy(&ethpara,pethpara,sizeof(EthPara));
		ethpara.Data.FlowSend=com_data.mon_flow_send[i][0];
		ethpara.Data.FlowRcv=com_data.mon_flow_rcv[i][0];
		ethpara.Data.LinkState=0;	
		para.Data=&ethpara;
		user_data.SetData(PARA_5102+i,&para);//init linkstate is 0,and update flow data
		
	}
    for(i=0;i<MAX_PPP_NUM;i++){
		user_data.GetData(PARA_5106+i,&para);	
		pwirelesspara=(WirelessPara*)para.Data;
		memcpy(&wirelesspara,pwirelesspara,sizeof(WirelessPara));
		wirelesspara.Data.LinkState=0;	
		wirelesspara.Data.FlowSend=com_data.mon_flow_send[i+MAX_ETH_NUM][0];
		wirelesspara.Data.FlowRcv=com_data.mon_flow_rcv[i+MAX_ETH_NUM][0];
		para.Data=&wirelesspara;
		user_data.SetData(PARA_5106+i,&para);//init linkstate is 0,and update flow data
	}
    for(i=1;i<MAX_PORT;i++){//set retransfer port	
    	for(j=0;j<MAX_RETRANS_PORT;j++){
		    if(i==fppt[j].port1)
		    	com_cfg.Buff[i].forward_port_id=fppt[j].port2;
		    if(i==fppt[j].port2)
		    	com_cfg.Buff[i].forward_port_id=fppt[j].port1;
	    }
	}
    return 1;
}


int16 Comm::Run ( void*arg )
{
    int i,j,index,ret;
    static time_t freeze_second = 0;    
    SYSTIME sys_time;
    Data<BaseDataType> para;
    EthPara *pethpara,ethpara;
    WirelessPara *pwirelesspara,wirelesspara;
	CommCfg*cfg;    

    NtpBasicInfo nbi;
    int nSendSize = sizeof(NtpBasicInfo);
    memset(&nbi,0, nSendSize);
	nbi.m_LiVnMode = 27; //Encoded representation which represents NTP Client Request & NTP version 3.0
	int channel_sntp=0;

    Init();

    while ( 1 ) {
        mdelay ( 100 );
        for(i=1;i<MAX_PORT;i++){
	        if(com_cfg.com_use[i]==0)
	        	continue;	
	        if(com_cfg.Buff[i].fun_checkstate(i)==1){		
	      			protocol.SetBuff ( &com_cfg.Buff[i] );	
	      	}
	        if(com_cfg.Buff[i].fun_sendbuf!=NULL){
	        	j=0;
	        	if(com_cfg.Buff[i].pro_id==PRO_SNTP)
	        		channel_sntp=i;
			        		
	    		while(com_cfg.Buff[i].send.in_cnt!=com_cfg.Buff[i].send.out_cnt){
	    				tmp_buf[j++]=com_cfg.Buff[i].send.buf[com_cfg.Buff[i].send.out_cnt++];
	    				if(com_cfg.Buff[i].send.out_cnt>=BUFFER_SIZE)
	    					com_cfg.Buff[i].send.out_cnt=0;
	    		}
	    		if(j>0){
	    				PFUNC(TEM_INFO,DEBUG_COM,"Port:%d send_len:%d\n",i,j);
	   					//dump_binary(tmp_buf,j);
	   					com_cfg.Buff[i].fun_sendbuf(i,tmp_buf,j);
	   			}
		   	}

		}	
			
		//检查流量,连接状态
		if (sec_timer ( 10, &freeze_second ) == 1){
			
    		nbi.m_TransmitTimestamp = CNtpTime::GetCurrentTime();
    		if(channel_sntp!=0)
    			com_cfg.Buff[channel_sntp].fun_sendbuf(channel_sntp,(uint8*)&nbi,nSendSize);	
    				
			for(i=0;i<MAX_ETH_NUM+MAX_PPP_NUM;i++){
				index=i-MAX_ETH_NUM;//由于comdata中的数据是连着的,而流量检查调用不同的sock接口,所以需要分开,针对无线接口用index
				if(i<MAX_ETH_NUM){
					cfg=&com_cfg.eth_para[i];
					com_data.mon_flow_send[i][1]=eth_socket.get_trans_bytes(com_cfg.eth_para[i].InterfaceName,GET_TX_BYTES);
					com_data.mon_flow_rcv[i][1]=eth_socket.get_trans_bytes(com_cfg.eth_para[i].InterfaceName,GET_RX_BYTES);
				}
				else{
					cfg=&com_cfg.wireless_para[index];
					com_data.mon_flow_send[i][1]=wireless_socket.get_trans_bytes(com_cfg.wireless_para[index].InterfaceName,GET_TX_BYTES);	
					com_data.mon_flow_rcv[i][1]=wireless_socket.get_trans_bytes(com_cfg.wireless_para[index].InterfaceName,GET_RX_BYTES);	
				}

				if(com_data.mon_flow_send[i][1]!=com_data.mon_flow_send[i][2]){
					if(com_data.mon_flow_send[i][1]>=com_data.mon_flow_send[i][2]){
						com_data.mon_flow_send[i][0]+=(com_data.mon_flow_send[i][1]-com_data.mon_flow_send[i][2]);
					}
					com_data.mon_flow_send[i][2]=com_data.mon_flow_send[i][1];	
				}
				if(com_data.mon_flow_rcv[i][1]!=com_data.mon_flow_rcv[i][2]){
					if(com_data.mon_flow_rcv[i][1]>=com_data.mon_flow_rcv[i][2]){
						com_data.mon_flow_rcv[i][0]+=(com_data.mon_flow_rcv[i][1]-com_data.mon_flow_rcv[i][2]);
					}
					com_data.mon_flow_rcv[i][2]=com_data.mon_flow_rcv[i][1];	
				}
				PFUNC(TEM_INFO,DEBUG_COM,"%s total send:%d(kb) rcv:%d(kb)\n",cfg->InterfaceName,com_data.mon_flow_send[i][0],com_data.mon_flow_rcv[i][0]);
					
				if(i<MAX_ETH_NUM){
						if(com_cfg.eth_para[i].Use){
							user_data.GetData(PARA_5102+i,&para);	
							pethpara=(EthPara*)para.Data;
							memcpy(&ethpara,pethpara,sizeof(EthPara));
							ethpara.Data.FlowSend=com_data.mon_flow_send[i][0];	//update physical channel flow data
							ethpara.Data.FlowRcv=com_data.mon_flow_rcv[i][0];	//update physical channel flow data
							ret=check_link(com_cfg.eth_para[i].InterfaceName);
							if(ret>=0)
								ethpara.Data.LinkState=1;
							else
								ethpara.Data.LinkState=0;						//update physical channel linkstate	

							para.Data=&ethpara;
							user_data.SetData(PARA_5102+i,&para);					//refresh  data
						}else{
							user_data.GetData(PARA_5102+i,&para);	
							pethpara=(EthPara*)para.Data;
							memcpy(&ethpara,pethpara,sizeof(EthPara));
							ethpara.Data.LinkState=0;							//update physical channel linkstate	
							para.Data=&ethpara;
							user_data.SetData(PARA_5102+i,&para);					//refresh  data
						}
				}
				else{//检查无线月流量是否超标及连接状态
						if(com_cfg.wireless_para[index].Use){
							user_data.GetData(PARA_5106+index,&para);	
							pwirelesspara=(WirelessPara*)para.Data;
							memcpy(&wirelesspara,pwirelesspara,sizeof(WirelessPara));
							wirelesspara.Data.FlowSend=com_data.mon_flow_send[i][0];
							wirelesspara.Data.FlowRcv=com_data.mon_flow_rcv[i][0];
							ret=check_link(com_cfg.wireless_para[index].InterfaceName);
							if(ret>=0)
								wirelesspara.Data.LinkState=1;
							else
								wirelesspara.Data.LinkState=0;	
							para.Data=&wirelesspara;	
							user_data.SetData(PARA_5106+index,&para);				//refresh  data
							//PFUNC ( TEM_INFO,DEBUG_COM, "%s Link:%d\r\n",com_cfg.wireless_para[index].InterfaceName,pwirelesspara->Data.LinkState );
	
							if((com_data.mon_flow_send[i][0]+com_data.mon_flow_rcv[i][0])>com_cfg.wireless_cfg[0].MonFlowSet){//if flow is up to limit ,we shut down it.
								wireless_socket.down(&com_cfg.wireless_para[index]);
								wireless_socket.up(&com_cfg.wireless_para[index]);
								com_data.mon_flow_send[i][0]=0;
								com_data.mon_flow_rcv[i][0]=0;
							}
						}else{
							user_data.GetData(PARA_5106+index,&para);	
							pwirelesspara=(WirelessPara*)para.Data;
							memcpy(&wirelesspara,pwirelesspara,sizeof(WirelessPara));
							wirelesspara.Data.LinkState=0;	
							para.Data=&wirelesspara;
							user_data.SetData(PARA_5106+index,&para);				//refresh  data
						}
				}					
			}
			
			timer.GetLocalTime(&sys_time);
			if(sys_time.mon!=com_data.cur_mon){
			memset(&com_data,0,sizeof(com_data));
			com_data.cur_mon=sys_time.mon;
			}
			save_file(COMRUNDATA,&com_data,sizeof(com_data),"wb",0);
		}
		
	    Heart();
        CHECK_TASK_EXIT ( ( ( TASK* ) arg ) );
    }
		msg.UnRegistMsg ( Id );
		for(i=1;i<MAX_PORT;i++)
		{
			if(com_cfg.com_use[i])
				com_cfg.Buff[i].fun_close(i);
		}
    PFUNC ( TEM_WARNING,DEBUG_COM, "Exit Com Run\r\n" );

    return 1;
}

int16 Comm::WriteCfg()
{

    PFUNC ( TEM_INFO,DEBUG_COM, "%s write config \n", name );
    return 0;
}

int16 Comm::ReadCfg()
{
    PFUNC ( TEM_INFO,DEBUG_COM, " read config [%s]\n", name );
    Data<BaseDataType> para;
    EthPara *ethpara[MAX_ETH_NUM];
    SockPara *sockpara;
    UartPara *uartpara;
    WirelessPara*wirelesspara[MAX_PPP_NUM];
    ComBasePara *basepara;
    RetransPortSet *rp[MAX_RETRANS_PORT];
    char buf[500];
    int i,k;
    int ret;

	//读取流量数据
	ret=read_file(COMRUNDATA,&com_data,sizeof(com_data),"rb",0);
	if(ret<0)
		memset(&com_data,0, sizeof(com_data));
	//读取通道公共参数
	user_data.GetData(PARA_5139,&para);
	basepara=(ComBasePara*)para.Data;   
	PFUNC(TEM_INFO,DEBUG_COM,"relink time:%d\n",basepara->Data.RelinkTime);
	//basepara->Data.RelinkTime=300;
	PFUNC(TEM_INFO,DEBUG_COM,"LocalId:%d\n",basepara->Data.LocalId);
	//basepara->Data.LocalId=1;
	//读取转发口参数,16个转发口设置
	for(i=0;i<MAX_RETRANS_PORT;i++){
	user_data.GetData(PARA_513A+i,&para);
	rp[i]=(RetransPortSet*)para.Data;   
	fppt[i].port1=rp[i]->Data.TransPort>>8&0x00ff;
	fppt[i].port2=rp[i]->Data.TransPort&0x00ff;
	PFUNC(TEM_INFO,DEBUG_COM,"TransPort:%d->%d\n",fppt[i].port1,fppt[i].port2);
	}
 
 	//读取以太网参数,最多有4个以太口
 	for(i=0;i<MAX_ETH_NUM;i++)
 	{
	    user_data.GetData(PARA_5102+i,&para);
	    ethpara[i]=(EthPara*)para.Data; 
   		memset(&com_cfg.eth_para[i],0,sizeof(com_cfg.eth_para[0]));

		PFUNC(TEM_INFO,DEBUG_COM,"Eth%d-------------------------\n",i);
		PFUNC(TEM_INFO,DEBUG_COM,"localip:%d.%d.%d.%d\n",
		ethpara[i]->Data.LocalIp[0],
		ethpara[i]->Data.LocalIp[1],
		ethpara[i]->Data.LocalIp[2],
		ethpara[i]->Data.LocalIp[3]
		);   
		//ethpara[i]->Data.LocalIp[0]=192;
		//ethpara[i]->Data.LocalIp[1]=168;
		//ethpara[i]->Data.LocalIp[2]=15;
		//ethpara[i]->Data.LocalIp[3]=92;
		
		PFUNC(TEM_INFO,DEBUG_COM,"mask:%d.%d.%d.%d\n",
		ethpara[i]->Data.Mask[0],
		ethpara[i]->Data.Mask[1],
		ethpara[i]->Data.Mask[2],
		ethpara[i]->Data.Mask[3]
		);   
		//ethpara[i]->Data.Mask[0]=255;/**<  子网掩码  */
		//ethpara[i]->Data.Mask[1]=255;
		//ethpara[i]->Data.Mask[2]=255;
		//ethpara[i]->Data.Mask[3]=0;
				
		PFUNC(TEM_INFO,DEBUG_COM,"gate:%d.%d.%d.%d\n",
		ethpara[i]->Data.Gate[0],
		ethpara[i]->Data.Gate[1],
		ethpara[i]->Data.Gate[2],
		ethpara[i]->Data.Gate[3]
		);   
		//ethpara[i]->Data.Gate[0]=192;/**<  网关  */
		//ethpara[i]->Data.Gate[1]=168;
		//ethpara[i]->Data.Gate[2]=15;
		//ethpara[i]->Data.Gate[3]=1;
		PFUNC(TEM_INFO,DEBUG_COM,"mac:%x.%x.%x.%x.%x.%x\n",
		ethpara[i]->Data.Mac[0],
		ethpara[i]->Data.Mac[1],
		ethpara[i]->Data.Mac[2],
		ethpara[i]->Data.Mac[3],
		ethpara[i]->Data.Mac[4],
		ethpara[i]->Data.Mac[5]
		);   
		//ethpara[i]->Data.Mac[0]=0x7e;/**<  mac地址  */
		//ethpara[i]->Data.Mac[1]=0x1d;
		//ethpara[i]->Data.Mac[2]=0x97;
		//ethpara[i]->Data.Mac[3]=0xd8;
		//ethpara[i]->Data.Mac[4]=0xc1;
		//ethpara[i]->Data.Mac[5]=0x5e;
		if(i==0)
			strcpy(com_cfg.eth_para[i].InterfaceName,ETH1_NAME);
		if(i==1)
			strcpy(com_cfg.eth_para[i].InterfaceName,ETH2_NAME);
		if(i==2)
			strcpy(com_cfg.eth_para[i].InterfaceName,ETH3_NAME);
		if(i==3)
			strcpy(com_cfg.eth_para[i].InterfaceName,ETH4_NAME);

		com_cfg.eth_para[i].proxycfg.proxy_type=0;	
		com_cfg.eth_para[i].Use=ethpara[i]->Data.Use;	
		com_cfg.eth_para[i].InterfaceId=INTERFACE_NET1+i;
		com_cfg.eth_para[i].cfg=&com_cfg.eth_para[i];
			
			
		PFUNC(TEM_INFO,DEBUG_COM,"eth %d,interface name:%s Use:%d\n",i,com_cfg.eth_para[i].InterfaceName,com_cfg.eth_para[i].Use);
		
		com_cfg.eth_para[i].LocalIp[0]=ethpara[i]->Data.LocalIp[0];
		com_cfg.eth_para[i].LocalIp[1]=ethpara[i]->Data.LocalIp[1];
		com_cfg.eth_para[i].LocalIp[2]=ethpara[i]->Data.LocalIp[2];
		com_cfg.eth_para[i].LocalIp[3]=ethpara[i]->Data.LocalIp[3];
		
		
		com_cfg.eth_para[i].Mask[0]=ethpara[i]->Data.Mask[0];
		com_cfg.eth_para[i].Mask[1]=ethpara[i]->Data.Mask[1];
		com_cfg.eth_para[i].Mask[2]=ethpara[i]->Data.Mask[2];
		com_cfg.eth_para[i].Mask[3]=ethpara[i]->Data.Mask[3];
		
		com_cfg.eth_para[i].Gate[0]=ethpara[i]->Data.Gate[0];
		com_cfg.eth_para[i].Gate[1]=ethpara[i]->Data.Gate[1];
		com_cfg.eth_para[i].Gate[2]=ethpara[i]->Data.Gate[2];
		com_cfg.eth_para[i].Gate[3]=ethpara[i]->Data.Gate[3];
		
		com_cfg.eth_para[i].Mac[0]=ethpara[i]->Data.Mac[0];
		com_cfg.eth_para[i].Mac[1]=ethpara[i]->Data.Mac[1];
		com_cfg.eth_para[i].Mac[2]=ethpara[i]->Data.Mac[2];
		com_cfg.eth_para[i].Mac[3]=ethpara[i]->Data.Mac[3];
		com_cfg.eth_para[i].Mac[4]=ethpara[i]->Data.Mac[4];
		com_cfg.eth_para[i].Mac[5]=ethpara[i]->Data.Mac[5];
	}

		//读取无线网络参数
	PFUNC(TEM_INFO,DEBUG_COM,"Wireless-------------------------\n");
	user_data.GetData(PARA_5106,&para);
	wirelesspara[0]=(WirelessPara*)para.Data; 		
	strcpy(com_cfg.wireless_para[0].InterfaceName,PPP_NAME);
	com_cfg.wireless_para[0].Use=wirelesspara[0]->Data.Use;
	com_cfg.wireless_para[0].InterfaceId=INTERFACE_NET5;
	com_cfg.wireless_para[0].cfg=&com_cfg.wireless_para[0];
	
	PFUNC(TEM_INFO,DEBUG_COM,"wireless vpn:%s\n",wirelesspara[0]->Data.Vpn);  		
	PFUNC(TEM_INFO,DEBUG_COM,"wireless UserName:%s\n",wirelesspara[0]->Data.UserName);  		
	PFUNC(TEM_INFO,DEBUG_COM,"wireless passwd:%s\n",wirelesspara[0]->Data.Passwd);  
	PFUNC(TEM_INFO,DEBUG_COM,"wireless MonFlowSet:%dkb\n",wirelesspara[0]->Data.MonFlowSet);
	PFUNC(TEM_INFO,DEBUG_COM,"wireless WireLessType:%d\n",wirelesspara[0]->Data.WireLessType);	    
	PFUNC(TEM_INFO,DEBUG_COM,"wireless Use:%d\n",wirelesspara[0]->Data.Use);	    

	//配置以太网sock参数.
	for(k=0;k<MAX_ETH_SOCK_NUM;k++)
	{
		user_data.GetData(PARA_5107+k,&para);
		sockpara=(SockPara*)para.Data; 
	
	    PFUNC(TEM_INFO,DEBUG_COM,"Sock%d-------------------------\n",k);
		PFUNC(TEM_INFO,DEBUG_COM,"HostIp:%d.%d.%d.%d\n",
		sockpara->Data.HostIp[0],
		sockpara->Data.HostIp[1],
		sockpara->Data.HostIp[2],
		sockpara->Data.HostIp[3]
		);   
		//sockpara->Data.HostIp[0]=192;  
		//sockpara->Data.HostIp[1]=168;  
		//sockpara->Data.HostIp[2]=15;  
		//sockpara->Data.HostIp[3]=62;  
		                          
		PFUNC(TEM_INFO,DEBUG_COM,"localport:%d\n",sockpara->Data.LocalPort);    
		//sockpara->Data.LocalPort=3333;    
	
		PFUNC(TEM_INFO,DEBUG_COM,"hostport:%d\n",sockpara->Data.HostPort);    
		//sockpara->Data.HostPort=3333;  
		
		PFUNC(TEM_INFO,DEBUG_COM,"ProSel:%d\n",sockpara->Data.ProSel);      
		//sockpara->Data.ProSel=PRO101;
		
		PFUNC(TEM_INFO,DEBUG_COM,"Mode:%d\n",sockpara->Data.Mode);      
		//sockpara->Data.Mode=SERVER_MODE;
	
		PFUNC(TEM_INFO,DEBUG_COM,"Use:%d\n",sockpara->Data.Use);      
		//sockpara->Data.Use=1;
		memset(&com_cfg.eth_cfg[k],0,sizeof(com_cfg.eth_cfg[k]));
		i=sockpara->Data.channel;//i is index of physical channel .
	    
		if(sockpara->Data.Use==1){
			com_cfg.eth_cfg[k].cfg=com_cfg.eth_para[i].cfg;//cp physical channel parameter
		   	com_cfg.eth_cfg[k].proxycfg.proxy_type=0;	
		   	
		   	PFUNC(TEM_INFO,DEBUG_COM,"sock %d,interface:%s\n",k,com_cfg.eth_cfg[k].cfg->InterfaceName);
		   	
		    com_cfg.eth_cfg[k].LocalIp[0]=com_cfg.eth_para[i].LocalIp[0];
		    com_cfg.eth_cfg[k].LocalIp[1]=com_cfg.eth_para[i].LocalIp[1];
		    com_cfg.eth_cfg[k].LocalIp[2]=com_cfg.eth_para[i].LocalIp[2];
		    com_cfg.eth_cfg[k].LocalIp[3]=com_cfg.eth_para[i].LocalIp[3];
		
		    
		    com_cfg.eth_cfg[k].Mask[0]=com_cfg.eth_para[i].Mask[0];
		    com_cfg.eth_cfg[k].Mask[1]=com_cfg.eth_para[i].Mask[1];
		    com_cfg.eth_cfg[k].Mask[2]=com_cfg.eth_para[i].Mask[2];
		    com_cfg.eth_cfg[k].Mask[3]=com_cfg.eth_para[i].Mask[3];
		    
		    com_cfg.eth_cfg[k].Gate[0]=com_cfg.eth_para[i].Gate[0];
		    com_cfg.eth_cfg[k].Gate[1]=com_cfg.eth_para[i].Gate[1];
		    com_cfg.eth_cfg[k].Gate[2]=com_cfg.eth_para[i].Gate[2];
		    com_cfg.eth_cfg[k].Gate[3]=com_cfg.eth_para[i].Gate[3];
		
		    com_cfg.eth_cfg[k].Mac[0]=com_cfg.eth_para[i].Mac[0];
		    com_cfg.eth_cfg[k].Mac[1]=com_cfg.eth_para[i].Mac[1];
		    com_cfg.eth_cfg[k].Mac[2]=com_cfg.eth_para[i].Mac[2];
		    com_cfg.eth_cfg[k].Mac[3]=com_cfg.eth_para[i].Mac[3];
		    com_cfg.eth_cfg[k].Mac[4]=com_cfg.eth_para[i].Mac[4];
		    com_cfg.eth_cfg[k].Mac[5]=com_cfg.eth_para[i].Mac[5];
	
		    com_cfg.eth_cfg[k].LocalPort=sockpara->Data.LocalPort;
		
		    com_cfg.eth_cfg[k].HostIp[0]=sockpara->Data.HostIp[0];
		    com_cfg.eth_cfg[k].HostIp[1]=sockpara->Data.HostIp[1];
		    com_cfg.eth_cfg[k].HostIp[2]=sockpara->Data.HostIp[2];
		    com_cfg.eth_cfg[k].HostIp[3]=sockpara->Data.HostIp[3];
		    
		    com_cfg.eth_cfg[k].HostPort=sockpara->Data.HostPort;
		    com_cfg.eth_cfg[k].Mode=sockpara->Data.Mode;
		    com_cfg.eth_cfg[k].isMaster=sockpara->Data.isMaster;
		    com_cfg.eth_cfg[k].TransPro=sockpara->Data.TransPro;
		    
		    com_cfg.eth_cfg[k].RelinkTime=basepara->Data.RelinkTime;
		    
		    com_cfg.eth_cfg[k].Buff=&com_cfg.Buff[ETH_PORT1+k];    		
		    
		    com_cfg.Buff[ETH_PORT1+k].port_id=ETH_PORT1+k;
		    com_cfg.Buff[ETH_PORT1+k].pro_id=sockpara->Data.ProSel;
		
			com_cfg.Buff[ETH_PORT1+k].fun_init=socket_init;
			com_cfg.Buff[ETH_PORT1+k].fun_reset=socket_reset;
			com_cfg.Buff[ETH_PORT1+k].fun_sendbuf=socket_sendbuf;
			com_cfg.Buff[ETH_PORT1+k].fun_getbyte=socket_getbyte;
			com_cfg.Buff[ETH_PORT1+k].fun_checkstate=socket_checkstate;   	   	    
			com_cfg.Buff[ETH_PORT1+k].fun_close=socket_close;
		
			if(com_cfg.eth_cfg[k].cfg->Use)//if physical channel is not use ,this sock can't be used
		    com_cfg.com_use[ETH_PORT1+k]=1;   	
		    else
		    com_cfg.com_use[ETH_PORT1+k]=0;   		

		   	com_cfg.Para[ETH_PORT1+k]=&com_cfg.eth_cfg[k];
		}else{
			com_cfg.com_use[ETH_PORT1+k]=0;
		}
	}	
	//配置无线网sock参数,每个无线口最多有4个sock通道
	for(k=0;k<MAX_PPP_SOCK_NUM;k++)
	{  		
		int32 local_ip=get_local_ip((char *)PPP_NAME);
		put_ulong(local_ip,(uint8*)&wirelesspara[0]->Data.LocalIp[0]);

		user_data.GetData(PARA_5107+MAX_ETH_SOCK_NUM+k,&para);//eth和ppp sock参数是连着的
		sockpara=(SockPara*)para.Data; 
	    PFUNC(TEM_INFO,DEBUG_COM,"Wireless Sock%d-------------------------\n",k);
		PFUNC(TEM_INFO,DEBUG_COM,"HostIp:%d.%d.%d.%d\n",
		sockpara->Data.HostIp[0],
		sockpara->Data.HostIp[1],
		sockpara->Data.HostIp[2],
		sockpara->Data.HostIp[3]
		);   
		//sockpara->Data.HostIp[0]=192;  
		//sockpara->Data.HostIp[1]=168;  
		//sockpara->Data.HostIp[2]=15;  
		//sockpara->Data.HostIp[3]=62;  
		                          
		PFUNC(TEM_INFO,DEBUG_COM,"localport:%d\n",sockpara->Data.LocalPort);    
		//sockpara->Data.LocalPort=3333;    
	
		PFUNC(TEM_INFO,DEBUG_COM,"hostport:%d\n",sockpara->Data.HostPort);    
		//sockpara->Data.HostPort=3333;  
		
		PFUNC(TEM_INFO,DEBUG_COM,"ProSel:%d\n",sockpara->Data.ProSel);      
		//sockpara->Data.rProSel=PRO_RARETRANS;
		
		PFUNC(TEM_INFO,DEBUG_COM,"Mode:%d\n",sockpara->Data.Mode);      
		//sockpara->Data.Mode=SERVER_MODE;
	
		PFUNC(TEM_INFO,DEBUG_COM,"Use:%d\n",sockpara->Data.Use);      
		//sockpara->Data.Use=1;
		
		memset(&com_cfg.wireless_cfg[k],0,sizeof(com_cfg.wireless_cfg[k]));
		if(sockpara->Data.Use==1){   
			com_cfg.wireless_cfg[k].cfg=com_cfg.wireless_para[0].cfg;//cp physical channel parameter

			memcpy(com_cfg.wireless_cfg[k].Vpn,wirelesspara[0]->Data.Vpn,sizeof(wirelesspara[0]->Data.Vpn));
			memcpy(com_cfg.wireless_cfg[k].UserName,wirelesspara[0]->Data.UserName,sizeof(wirelesspara[0]->Data.UserName));
			memcpy(com_cfg.wireless_cfg[k].Passwd,wirelesspara[0]->Data.Passwd,sizeof(wirelesspara[0]->Data.Passwd));
			
			memset(buf,0,500);
			ret=read_file(CHAT_CFG_BAK,buf,500,"rt",0);
			if(ret>=0){
				str_replace(buf,(char *)"vpn",(char*)&(com_cfg.wireless_cfg[0].Vpn));
				save_file(CHAT_CFG,buf,strlen(buf),"wt",0);
			}
			memset(buf,0,500);
			ret=read_file(PAP_CFG_BAK,buf,500,"rt",0);
			if(ret>=0){
				str_replace(buf,(char *)"user",(char*)&(com_cfg.wireless_cfg[0].UserName));
				str_replace(buf,(char *)"passwd",(char*)&(com_cfg.wireless_cfg[0].Passwd));
				save_file(PAP_CFG,buf,strlen(buf),"wt",0);
			}
			memset(buf,0,500);
			ret=read_file(CHAP_CFG_BAK,buf,500,"rt",0);
			if(ret>=0){
				str_replace(buf,(char *)"user",(char*)&(com_cfg.wireless_cfg[0].UserName));
				str_replace(buf,(char *)"passwd",(char*)&(com_cfg.wireless_cfg[0].Passwd));
				save_file(CHAP_CFG,buf,strlen(buf),"wt",0);
			}
		    com_cfg.wireless_cfg[k].LocalIp[0]=wirelesspara[0]->Data.LocalIp[0];
		    com_cfg.wireless_cfg[k].LocalIp[1]=wirelesspara[0]->Data.LocalIp[1];
		    com_cfg.wireless_cfg[k].LocalIp[2]=wirelesspara[0]->Data.LocalIp[2];
		    com_cfg.wireless_cfg[k].LocalIp[3]=wirelesspara[0]->Data.LocalIp[3];
		
		    com_cfg.wireless_cfg[k].LocalPort=sockpara->Data.LocalPort;     	
		    
		    com_cfg.wireless_cfg[k].HostIp[0]=sockpara->Data.HostIp[0];
		    com_cfg.wireless_cfg[k].HostIp[1]=sockpara->Data.HostIp[1];
		    com_cfg.wireless_cfg[k].HostIp[2]=sockpara->Data.HostIp[2];
		    com_cfg.wireless_cfg[k].HostIp[3]=sockpara->Data.HostIp[3];
		    com_cfg.wireless_cfg[k].HostPort=sockpara->Data.HostPort;
		    com_cfg.wireless_cfg[k].MonFlowSet=wirelesspara[0]->Data.MonFlowSet;
			com_cfg.wireless_cfg[k].Mode=sockpara->Data.Mode;
		  	com_cfg.wireless_cfg[k].isMaster=sockpara->Data.isMaster; 
		    com_cfg.wireless_cfg[k].TransPro=sockpara->Data.TransPro;
		    com_cfg.wireless_cfg[k].RelinkTime=basepara->Data.RelinkTime;
		  	com_cfg.wireless_cfg[k].Buff=&com_cfg.Buff[WIRELESS_PORT1+k];  
		
		
		    com_cfg.Buff[WIRELESS_PORT1+k].port_id=WIRELESS_PORT1+k;
		    com_cfg.Buff[WIRELESS_PORT1+k].pro_id=sockpara->Data.ProSel;
		  
			com_cfg.Buff[WIRELESS_PORT1+k].fun_init=		wireless_socket_init;       
			com_cfg.Buff[WIRELESS_PORT1+k].fun_reset=		wireless_socket_reset;      
			com_cfg.Buff[WIRELESS_PORT1+k].fun_sendbuf=	wireless_socket_sendbuf;    
			com_cfg.Buff[WIRELESS_PORT1+k].fun_getbyte=	wireless_socket_getbyte;          
			com_cfg.Buff[WIRELESS_PORT1+k].fun_checkstate=wireless_socket_checkstate;       
			com_cfg.Buff[WIRELESS_PORT1+k].fun_close=wireless_socket_close;
			if(com_cfg.wireless_cfg[k].cfg->Use)//if physical channel is not use ,this sock can't be used
		    com_cfg.com_use[WIRELESS_PORT1+k]=1;
		    else
		    com_cfg.com_use[WIRELESS_PORT1+k]=0;	
			com_cfg.Para[WIRELESS_PORT1+k]=&com_cfg.wireless_cfg[k];
		 }
		 else
			com_cfg.com_use[WIRELESS_PORT1+k]=0;
	}

   	//读取串口参数
   	for(i=0;i<MAX_UART;i++){
	    user_data.GetData(PARA_512F+i,&para);
	    uartpara=(UartPara*)para.Data;
	    /*
	    uartpara->Data.ProSel=PRO101;
	    uartpara->Data.BaudRate=9600;
	    uartpara->Data.DataBit=8;
	    uartpara->Data.StopBit=1;
	    uartpara->Data.CheckBit=0;
	    */
	    
	    memset(&com_cfg.uart_cfg[i],0,sizeof(UartCfg));		
		if(i==0)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART1_DEV);
		if(i==1)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART2_DEV);
		if(i==2)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART3_DEV);
		if(i==3)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART4_DEV);
		if(i==4)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART5_DEV);
		if(i==5)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART6_DEV);
		if(i==6)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART7_DEV);
		if(i==7)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART8_DEV);
		if(i==8)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART9_DEV);
		if(i==9)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART10_DEV);
		if(i==10)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART11_DEV);
		if(i==11)
			strcpy(com_cfg.uart_cfg[i].InterfaceName,UART12_DEV);
	    
	    if(uartpara->Data.BaudRate){
	    	com_cfg.uart_cfg[i].BaudRate=uartpara->Data.BaudRate;
	    	com_cfg.uart_cfg[i].DataBit=uartpara->Data.DataBit;
	    	com_cfg.uart_cfg[i].StopBit=uartpara->Data.StopBit;
	    	if(uartpara->Data.CheckBit==0)
	    	com_cfg.uart_cfg[i].CheckBit='N';
	    	else if(uartpara->Data.CheckBit==1)
	    	com_cfg.uart_cfg[i].CheckBit='O';
	    	else if(uartpara->Data.CheckBit==2)
	    	com_cfg.uart_cfg[i].CheckBit='E';
		    com_cfg.uart_cfg[i].isMaster=1;
		    com_cfg.uart_cfg[i].Buff=&com_cfg.Buff[UART_PORT1+i];   
	
	
		    com_cfg.Buff[UART_PORT1+i].port_id=UART_PORT1+i;
		    com_cfg.Buff[UART_PORT1+i].pro_id=uartpara->Data.ProSel;
	    	com_cfg.Buff[UART_PORT1+i].fun_init=uart_init;
	    	com_cfg.Buff[UART_PORT1+i].fun_reset=uart_reset;
	    	com_cfg.Buff[UART_PORT1+i].fun_sendbuf=uart_sendbuf;
	    	com_cfg.Buff[UART_PORT1+i].fun_getbyte=uart_getbyte;
	    	com_cfg.Buff[UART_PORT1+i].fun_checkstate=uart_checkstate;
	    	com_cfg.Buff[UART_PORT1+i].fun_close=uart_close;
		    com_cfg.com_use[UART_PORT1+i]=1;
	    	com_cfg.Para[UART_PORT1+i]=&com_cfg.uart_cfg[i];
			PFUNC(TEM_INFO,DEBUG_COM,"Cfg %s\n",com_cfg.uart_cfg[i].InterfaceName);      	    	
		}    		
	}

   //GPS通讯
   com_cfg.uart_cfg[UART_PORT8-1].BaudRate=9600;
   com_cfg.uart_cfg[UART_PORT8-1].DataBit=8;
   com_cfg.uart_cfg[UART_PORT8-1].StopBit=1;
   com_cfg.uart_cfg[UART_PORT8-1].CheckBit='N';
   com_cfg.uart_cfg[UART_PORT8-1].Buff=&com_cfg.Buff[UART_PORT8];    		
   com_cfg.com_use[UART_PORT8]=0;			
   //GPRS通讯
   com_cfg.com_use[UART_PORT9]=0;
   //小无线	
   com_cfg.com_use[UART_PORT10]=0;
   com_cfg.Buff[UART_PORT10].fun_init=little_wireless_uart_init;
   com_cfg.Buff[UART_PORT10].fun_reset=little_wireless_uart_reset;
   //GPRS监控通道	
   com_cfg.com_use[UART_PORT11]=0;
   com_cfg.Buff[UART_PORT11].fun_init=wireless_uart_init;
   //sms 通道	
   com_cfg.com_use[UART_PORT12]=0;

	return 1;
}

int16 Comm::GetMsg ( MessageData<BaseDataType>message )
{
	int i,j,id[MAX_PORT],num;
	SOCKET sock;
	WIRELESS_SOCKET wsock;
	int index;

    PFUNC ( TEM_INFO,DEBUG_COM, "%s get message Id:%x DataId:%x \n", name,message.MessageId,message.Id );
    if ( message.MessageId == MSG_PARA ) {
    	num=0;
    	if(message.Id>=PARA_5102&&message.Id<=PARA_5149){
		    ReadCfg();
		    
			if(message.Id>=PARA_513A&&message.Id<=PARA_5149){//for retransfer para
			    for(i=1;i<MAX_PORT;i++){//set retransfer port	
			    	for(j=0;j<MAX_RETRANS_PORT;j++){
					    if(i==fppt[j].port1)
					    	com_cfg.Buff[i].forward_port_id=fppt[j].port2;
					    if(i==fppt[j].port2)
					    	com_cfg.Buff[i].forward_port_id=fppt[j].port1;
				    }
				}		    
			}
			
		    if(message.Id==PARA_5139){//for common para
				id[0]	=	WIRELESS_PORT1;
				id[1]	=	WIRELESS_PORT2;
				id[2]	=	WIRELESS_PORT3;
				id[3]	=	WIRELESS_PORT4;
				id[4]	=	ETH_PORT1;
				id[5]	=	ETH_PORT2;
				id[6]	=	ETH_PORT3;
				id[7]	=	ETH_PORT4;
				id[8]	=	ETH_PORT5;
				id[9]	=	ETH_PORT6;
				id[10]	=	ETH_PORT7;
				id[11]	=	ETH_PORT8;
				id[12]	=	ETH_PORT9;
				id[13]	=	ETH_PORT10;
				id[14]	=	ETH_PORT11;
				id[15]	=	ETH_PORT12;
				id[16]	=	ETH_PORT13;
				id[17]	=	ETH_PORT14;
				id[18]	=	ETH_PORT15;
				id[19]	=	ETH_PORT16;
	    		num=20;

		    }
		    if(message.Id>=PARA_5102&&message.Id<=PARA_5105){//以太参数
				 index=message.Id-PARA_5102;
				 j=0;
				 if(com_cfg.eth_para[index].Use==0)
				 	sock.down(&com_cfg.eth_para[index]);
				 else	
				 	sock.up(&com_cfg.eth_para[index]);
				 for(i=0;i<MAX_ETH_SOCK_NUM;i++){
				 	if(com_cfg.eth_cfg[i].cfg==NULL)
				 		continue;
				 	if(com_cfg.eth_cfg[i].cfg->InterfaceId==(index+INTERFACE_NET1)){//only chang socket which  physical channel is mach this.
				 		id[j]	=com_cfg.Buff[ETH_PORT1+i].port_id;
				 		j++;
				 	}
				 		
				 }
				num=j;
			}
			if(message.Id==PARA_5106){//无线参数
	    		if(com_cfg.wireless_para[0].Use==0)
	    			wsock.down(&com_cfg.wireless_para[0]);
	    		else	
	    			wsock.up(&com_cfg.wireless_para[0]);
				id[0]	=	WIRELESS_PORT1;//because only use one wireless physical channel ,so we change all wireless socket in this.
				id[1]	=	WIRELESS_PORT2;
				id[2]	=	WIRELESS_PORT3;
				id[3]	=	WIRELESS_PORT4;
	    		num=4;
		    }
			
			if(message.Id>=PARA_5107&&message.Id<=PARA_512E){//网口参数
	    		id[0]=message.Id-PARA_5107+ETH_PORT1;
	    		num=1;
	    	}
			
			if(message.Id>=PARA_512F&&message.Id<=PARA_5138){//串口参数
	    		id[0]=message.Id-PARA_512F+UART_PORT1;
	    		num=1;
	    	}
			for(i=0;i<num;i++){
	    		if(com_cfg.com_use[id[i]]==1&&com_cfg.com_olduse[id[i]]==1){//参数变更但是启用标志未变
	    			if(com_cfg.Buff[id[i]].fun_close!=NULL){
	    				com_cfg.Buff[id[i]].fun_close(id[i]);
	    				sleep(5);
	    			}
				    if(com_cfg.Buff[id[i]].fun_init!=NULL){
	    				com_cfg.com_olduse[id[i]]=1;
	    				com_cfg.Buff[id[i]].fun_init(id[i],com_cfg.Para[id[i]]);
	    			}
	    		}else if(com_cfg.com_use[id[i]]==0&&com_cfg.com_olduse[id[i]]==1){//停用通讯口
	    			if(com_cfg.Buff[id[i]].fun_close!=NULL){
	    				com_cfg.com_olduse[id[i]]=0;
	    				com_cfg.Buff[id[i]].fun_close(id[i]);
		    		}
				}else if(com_cfg.com_use[id[i]]==1&&com_cfg.com_olduse[id[i]]==0){//启用通讯口
	    			if(com_cfg.Buff[id[i]].fun_init!=NULL){
		    			com_cfg.com_olduse[id[i]]=1;
		    			com_cfg.Buff[id[i]].fun_init(id[i],com_cfg.Para[id[i]]);
				    }
				}
				    	
			}
				    		
		}

    }
    return 1;
}

uint8 Comm::GetPortStatus ( uint16 port )
{

   return com_cfg.Buff[port].fun_checkstate (port);
}

int16 Comm::CheckError ( uint16 port )
{
    PFUNC ( TEM_INFO,DEBUG_COM, "comm check error,no error\n" );
    return 0;
}

int16 Comm::Open ( uint16 port )
{
    if(com_cfg.com_use[port]==1){
    	if(com_cfg.Buff[port].fun_init!=NULL)
  		return com_cfg.Buff[port].fun_init (port,com_cfg.Para[port]);
  	}
  return 0;	
}

int16 Comm::Close ( uint16 port )
{
    if(com_cfg.com_use[port]==1){
  		if(com_cfg.Buff[port].fun_close!=NULL)
  		return com_cfg.Buff[port].fun_close (port);
  	}
  return 0;	
}

int16 Comm::Reset ( uint16 port )
{
	if(com_cfg.com_use[port]==1&&com_cfg.com_olduse[port]==1){
//		if(com_cfg.Buff[port].fun_close!=NULL)
//		com_cfg.Buff[port].fun_close(port);
//		if(com_cfg.Buff[port].fun_init!=NULL)
//		com_cfg.Buff[port].fun_init(port,com_cfg.Para[port]);
		
		if(com_cfg.Buff[port].fun_reset!=NULL)
		com_cfg.Buff[port].fun_reset(port);	
		return 1;
	}
    return 0;
}


/////////////////////私有函数/////////////////////////////////////////////////
int16 Comm::Read ( uint16 port, uint8 *buf,  uint16 *ret_len )
{
	if(com_cfg.com_use[port]==1){
		if(com_cfg.Buff[port].fun_getbyte!=NULL){
			while(com_cfg.Buff[port].fun_getbyte(port,buf)>=0){
			buf++;
			(*ret_len)++;
			}
			return *ret_len;
		}
	}
    return 0;
}

int16 Comm::Write ( uint16 port, uint8 *buf, int len )
{
	if(com_cfg.com_use[port]==1){
		if(com_cfg.Buff[port].fun_sendbuf!=NULL)
			if(com_cfg.Buff[port].fun_sendbuf(port,buf,len)>=0)
				return 1;
	}
    return 0;
}
