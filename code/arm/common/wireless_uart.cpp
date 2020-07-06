#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "datatype.h"
#include "task.h"
#include "debug.h"
#include "device.h"
#include "uart.h"
#include "com.h"
#include "wireless_uart.h"
#include "localtime.h"//for sec min_timer
#include "utils.h"


//private function
static int16 do_nothing ( void );
static int16 update_rssi ( void );

static uint8 	get_pack ( int channel );
static int 		check_pack();
static int	 	process_work( int channel );
static void  	send_at_cmd(int channel,uint8 command_id, char* parameter);

//private data
static AT_CMD M72_CMD[]=
{
	{ONOFF, (char *)"AT", (char *)"OK", (char *)"", 2, do_nothing},
	{CHECKSIMCARD,(char *)"AT+QSIMSTAT?",(char *)"+QSIMSTAT",(char *)"1",15,do_nothing},
	{CHECKNET,(char *)"AT+CREG?",(char *)"+CREG",(char *)"1",11,do_nothing},
	{CHECKRSSI,(char *)"AT+CSQ",(char *)"+CSQ",(char *)"",2,update_rssi}
};
static char module_buf[MAX_MODULE_PACK_SIZE];
static uint8   rcv_cnt = 0; //module_buf中有效数据长度
static int rcv_status = idle;
static  AT_CMD*  pCmd_table;
static int table_size;
static int check_order=0;
static int module_timer = 0;
static int waiting_module_respon = 0;
static WIRELESS_UART wireless_uart;
extern int start_gprs_flag;


static void *receive_thread ( void* parameter )
{
	int nread, i;
	THREAD_DATA *pdata;
	BUF_DATA* p_buf;
	char t_buf[100];
	int channel;
	fd_set rfds;
	struct timeval to;
	time_t freeze_second = 0;
	int tick=0;


	pdata = ( THREAD_DATA *) parameter;
	UartCfg*para;
	para=(UartCfg*)(pdata->para);
	
	p_buf = &(pdata->p_buf->recv);
	channel = pdata->fd_index;
	uart_quit_thread[channel]=0;

PFUNC(TEM_INFO,DEBUG_COM, "start uart thread:%d\n",UART_THD+channel);
	while (1) {
		g_task->Heart(UART_THD+channel);		
		
		if (sec_timer ( 1, &freeze_second ) == 1){
			
				if(start_gprs_flag)
				{
					wireless_uart.close_serial(channel);
					wireless_uart.init_serial(channel,para->BaudRate,para->DataBit,para->StopBit,para->CheckBit);
					start_gprs_flag=0;	
				}				
				tick++;
				if(tick>=CHECK_CYCLE_TIME){
					if(process_work(channel)==1)	
					tick=0;
				}
				//if(waiting_module_respon==1)
				module_timer ++;
				//PFUNC(TEM_INFO,DEBUG_COM,"workstatus:%x\n",wireless_uart.working_status);
					
				//PFUNC(TEM_INFO,DEBUG_COM,"module_timer:%d\n",module_timer);
				if (module_timer > STATUS_TIME_OUT) {
					wireless_uart.working_status &= ~ ( 1UL << wireless_uart.module_status );
					module_timer = 0;
					wireless_uart.module_status = idle;
					//continuous_errors = 0;
					PFUNC(TEM_WARNING,DEBUG_COM,"no response ,uart_dev:%d\n",uart_fd[channel]);
					waiting_module_respon=0;
					start_gprs_flag=1;//没有回应，可能串口被占用
				}
		}
		if(uart_quit_thread[channel]==1)
			break;
		if(uart_fd[channel]<0)continue;
		
		if(get_pack(channel)){	
		module_timer = 0;
		//module_timer_old = 0;		
		waiting_module_respon=0;
		check_pack();
		}
					
		memset(t_buf, 0, sizeof(t_buf) );
		FD_ZERO(&rfds);
		FD_SET(uart_fd[channel], &rfds);
		to.tv_sec = 0;
		to.tv_usec = 10000;
		if (select(uart_fd[channel] + 1,  &rfds, 0, 0, &to) > 0) {
			if (uart_fd[channel] < 0)
				break;
			if (!FD_ISSET(uart_fd[channel], &rfds))
				continue;
			if (( nread = read ( uart_fd[channel], t_buf, 100 ) ) > 0) {
				for (i = 0;i < nread;i++) {
					p_buf->buf[p_buf->in_cnt] = t_buf[i];
					if (++p_buf->in_cnt >= BUFFER_SIZE)
						p_buf->in_cnt = 0;

				}
			//PFUNC(TEM_INFO,DEBUG_COM, "recv uart channel:%d Len:%d\n",channel,nread);	
			t_buf[nread]=0;
			//fprintf(stderr,"%s\n",t_buf);
			//dump_binary((uint8*)t_buf,nread);
			}
		}
	}
	g_task->UnRegist(UART_THD+channel);
	PFUNC(TEM_ERROR ,DEBUG_COM, "exit uart thread:%d\n",UART_THD+channel);		
	return NULL;
}


/*****************************************************************************
																	public fun
******************************************************************************/
int WIRELESS_UART::init(int port,void *parameter)
{
	int channel;
	UartCfg*para;
	para=(UartCfg*)parameter;
	channel=get_channel(port);
	pCmd_table=M72_CMD;

	if(channel!=-1)
		{
			uart_thread_data[channel].fd_index = channel;
			uart_thread_data[channel].p_buf = para->Buff;
			uart_thread_data[channel].para = parameter;

			uart_thread_data[channel].p_buf->recv.in_cnt = 0;

			uart_thread_data[channel].p_buf->recv.out_cnt = 0;
			uart_thread_data[channel].p_buf->send.in_cnt = 0;
			uart_thread_data[channel].p_buf->send.out_cnt = 0;
			memset(uart_thread_data[channel].p_buf->recv.buf, 0, BUFFER_SIZE*sizeof(uint8));
			memset(uart_thread_data[channel].p_buf->send.buf, 0, BUFFER_SIZE*sizeof(uint8));		
			uart_fd[channel]=-1;
			table_size=TABLE_SIZE(M72_CMD);
			g_task->Regist(UART_THD+channel);
			return pthread_create ( &uart_tid[channel], NULL, receive_thread, ( void* ) &uart_thread_data[channel] );
		}
	return -1;
}	
int WIRELESS_UART::checkstate ( int port )
{
	return 1;
}

int WIRELESS_UART::wireless_status(void)
{
	int status = get_working_status();

	if(!(status & (1 << CHECKSIMCARD) )) {
		module_rssi=0;
		return NO_CARD;  //no card insert
	} else if(!(status & (1 << CHECKNET))) {
		return CARD_REGISTERING;  // registering
	} else if(!(status & (1 << CHECKPPP))) {
		return CARD_DIALING;  //dialing
	} else if(!(status & (1 << CHECKTCP))) {
		return CARD_CONNECTING;  //connecting
	} else {
		return CARD_CONNECTED;  //connected
	}
}


uint8 get_pack ( int channel )											//搜索CR LF ‘+’ 或CR LF ‘*’字符组合
{
	uint8 temp;
	uint8 loop_times = 0;

	BUF_DATA* p_buf;

	p_buf = &(uart_thread_data[channel].p_buf->recv);


	while (p_buf->in_cnt != p_buf->out_cnt) {
		if (loop_times++ >= 100) {
			break;									//不应在while里循环次数过多，将cpu时间让给其他任务
		}
		temp = p_buf->buf[p_buf->out_cnt];
		if (++p_buf->out_cnt >= BUFFER_SIZE)
			{
			p_buf->out_cnt = 0;
			}

		switch (rcv_status) { //没有检测到主站数据是，常规数据包检测，以CRLF作为数据包头尾进行收包。

			case status_idle:
			{
				if (temp == CR) {
					rcv_cnt = 0;
					memset(module_buf, 0, MAX_MODULE_PACK_SIZE);
					rcv_status = status_prepare1;
					module_buf[rcv_cnt++] = temp;
				}

				break;
			}

			case status_prepare1:
			{
				if (temp == CR) {
					rcv_cnt = 0;
					rcv_status = status_prepare1;
					module_buf[rcv_cnt++] = temp;
				} else if (temp == LF) {
					rcv_status = status_prepare2;
					module_buf[rcv_cnt++] = temp;
				} else {
					rcv_cnt = 0;
					rcv_status = status_idle;
				}

				break;
			}

			case status_prepare2:
			{
				if (temp == CR) {
					rcv_cnt = 0;
					module_buf[rcv_cnt++] = temp;
					rcv_status = status_prepare1;
				} else if (temp == LF) {
					rcv_cnt = 0;
					rcv_status = status_idle;
				} else if (temp == '>') {
					module_buf[rcv_cnt++] = temp;
					rcv_status = status_idle;
					return 1;
				} else {
					module_buf[rcv_cnt++] = temp;
					rcv_status = status_receiving;
				}

				break;
			}

			case status_receiving:
			{
        module_buf[rcv_cnt++] = temp;
				if (temp == CR) {
						rcv_status = status_prepare_finish;
				}
				break;
			}

			case status_prepare_finish:
			{
				module_buf[rcv_cnt++] = temp;

				if (temp == LF) {
					rcv_status = status_idle;
					//fprintf(stderr,"%s",module_buf);
					return 1;
				} else if (temp == CR) {
					rcv_status = status_prepare_finish;
				} else {
					rcv_status = status_receiving;
				}

				break;
			}
			default:
				break;
		}

	}

	return 0;
}

void send_at_cmd ( int channel,uint8 command_id, char* parameter )						//发送AT指令表中指令
{
	uint8 i;
	static uint8 temp;

	//PFUNC(TEM_INFO,DEBUG_COM, "command_id=%d, <%s%s>\n", command_id,  (pCmd_table + command_id )->CMD_STR, parameter);
	for (i = 0;* ( ( ( char* ) ( pCmd_table + command_id )->CMD_STR ) + i ) != '\0';i++) {			//取指令
		temp = * ( ( ( char* ) ( pCmd_table + command_id )->CMD_STR ) + i );
		wireless_uart.sendbyte ( channel, temp );
	}

	for (i = 0;* ( parameter + i ) != '\0';i++) {								//发送参数
		wireless_uart.sendbyte ( channel, * ( parameter + i ) );
	}

	wireless_uart.sendbyte (channel,  CR );

	wireless_uart.module_status = ( pCmd_table + command_id )->CMD_ID;
	module_timer = 0;
	//module_timer_old = 0;	
	rcv_status = status_idle;
}

int check_pack(){
	char *pbuf_rcv;
	int i,find;
	pbuf_rcv=module_buf;	
	find=0;
	for(i=0;i<table_size;i++){
		if(M72_CMD[i].CMD_ID==wireless_uart.module_status){
		find=1;	
		break;
		}
	}
	if(!find)return 0;
		
	if (str_compare ( ( char * ) pbuf_rcv, 2, ( pCmd_table +i )->CMD_STR_RETURN ) == 1) { //?￡??μ???°???é????3é???-?????o??-?
			if (str_compare ( ( char * ) pbuf_rcv, ( pCmd_table +i )->CMD_RETURN_NUM, ( pCmd_table +i )->CMD_RETURN ) == 1) {	//正确应答
			//PFUNC(TEM_INFO,DEBUG_COM, "* ( ( pCmd_table + module_status )->fp ) =%p, module_status = %d \n", * ( ( pCmd_table +i )->fp ),i );
				wireless_uart.working_status |= ( 1UL <<wireless_uart.module_status );
				//continuous_errors = 0;										//错误累计清0（此策略或待商榷）
				if((pCmd_table +i )->fp)
					( * ( ( pCmd_table +i )->fp ) ) ();//若fp包含发送at指令内容，则module_status会相应改变，此处需跳过以下的module_status复位
				return wireless_uart.working_status;
			}
	 		else {//错误应答
				wireless_uart.working_status &= ( ~ ( 1UL <<wireless_uart.module_status ) );
				//continuous_errors++;
				return 0;
			}
	wireless_uart.module_status = idle;
	}
	return 0;
}

int16 update_rssi ( void )  //更新无线信号强度
{
	uint8 i;
	char rssi[10];

	for (i = 0;i < 2;i++) {
		if (module_buf[i+8] != ',') {
			rssi[i] = module_buf[i+8];
		} else {
			rssi[i] = '\0';
			break;
		}
	}
	rssi[2] = '\0';
	wireless_uart.module_rssi=atoi(rssi);
	PFUNC(TEM_INFO,DEBUG_COM,"rssi:%d\n",wireless_uart.module_rssi);
	return wireless_uart.module_rssi;
}

int process_work(int channel){
int ret=0;	
	//PFUNC(TEM_INFO,DEBUG_COM,"-----waiting_module_respon:%d id %d \n",waiting_module_respon,uart_fd[channel]);
	if(waiting_module_respon)return 0;
	if(uart_fd[channel]<0)return 0;	
	send_at_cmd(channel,check_order,(char *)"");
	check_order++;
	if(check_order>=table_size){
	check_order=0;	
	ret=1;
	}
	waiting_module_respon=1;
	return ret;
}

int16 do_nothing ( void )
{
	return 1;
}




