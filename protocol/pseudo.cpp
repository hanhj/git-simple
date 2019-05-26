/*************************************************************************
	> File Name: pseudo.cpp
	> Author: hanhj
	> Mail: hanhj@zx-jy.com 
	> Created Time: 2019年02月19日 星期二 22时09分40秒
 ************************************************************************/
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#include<sys/select.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
using namespace std;
int g_balance=0;
char g_filename[20];
/****************************
 * utilty
****************************/
void sleep_ms(int t){
	struct timeval tm;
	tm.tv_sec=t/1000;
	tm.tv_usec=t*1000;
	select(4,NULL,NULL,NULL,&tm);
}
unsigned char sum(unsigned char *data,int len){
	int i;
	unsigned char _sum;
	_sum=0;
	i=0;
	if(len<0)
		return 0;
	while(len--){
		_sum+=*(data+i);
		i++;
	}
	return _sum;
}
time_t get_system_time(){
	return time(NULL);
}
class timer{
	public:
		timer(){
			start_flag=0;
		}
		int exp_time;
		int start_flag;
		int start(int T);
		int stop();
		int is_reached();
};
int timer::is_reached(){
	if(!start_flag)
		return -1;
	if(get_system_time()>exp_time){
		stop();
		return 1;
	}
	return 0;
}
int timer::start(int T){
	if(start_flag == 1){
		cout<<"timer has started"<<endl;
		return -1;
	}
	start_flag=1;
	exp_time=get_system_time()+T;
	cout<<"start timer"<<endl;
	return 0;
}
int timer::stop(){
	if(start_flag == 1){
		cout<<"stop timer"<<endl;
		start_flag =0;
		return 0;
	}
	return -1;
}
#define DEBUG_LEVEL		3
#define DEBUG_ERROR		1
#define DEBUG_WARNING	2
#define DEBUG_INFO		3
#define DEBUG_NORMAL	4
#define DEBUG_BUFF 1000
unsigned char debug_buff[DEBUG_BUFF];
#define pfunc(mode,msg...) debug(mode,__FILE__,__func__,__LINE__,msg)
#define pdump(mode,head,data,len) dump(mode,__FILE__,__func__,__LINE__,data,len,head)
void debug(int mode,const char*file,const char*func,int line,const char *fmt...){
	const char *color="";
	const char *default_col="\033[0;m";
	if(mode>DEBUG_LEVEL)
		return;
	switch(mode){
		case DEBUG_ERROR:
			color="\033[031m";
			break;
		case DEBUG_WARNING:
			color="\033[033m";
			break;
		case DEBUG_INFO:
			color="\033[032m";
			break;
		case DEBUG_NORMAL:
			color="\033[0;m";
			break;
	}
	va_list arg;
	va_start(arg,fmt);
	vsnprintf((char*)&debug_buff[0],DEBUG_BUFF,fmt,arg);
	va_end(arg);
	fprintf(stderr,"[%s %s %d]%s%s%s",file,func,line,color,debug_buff,default_col);
}
void dump(int mode,const char*file,const char*func,int line,unsigned char *data,int len,const char *head=NULL){
	int i;
	i=0;
	const char *color="";
	const char *default_col="\033[0;m";
	if(mode>DEBUG_LEVEL)
		return;
	switch(mode){
		case DEBUG_ERROR:
			color="\033[031m";
			break;
		case DEBUG_WARNING:
			color="\033[033m";
			break;
		case DEBUG_INFO:
			color="\033[032m";
			break;
		case DEBUG_NORMAL:
			color="\033[0;m";
			break;
	}
	if(head!=NULL){
		fprintf(stderr,"[%s %s %d]%s%s %d:%s",file,func,line,color,head,len,default_col);
	}
	while(len--){
		fprintf(stderr,"%02hhx ",*(data+i));
		i++;
		if(i%16==0)
			fprintf(stderr,"\n");
	}
	fprintf(stderr,"\n");
}
/****************************
 * physical layer
****************************/
#define MAX_COM_BUFFER 1000
#define TYPE_SERIAL 1
#define TYPE_ETHERNET 2
#define TYPE_WIRELESS 3

class basic_com{
	protected:
		int read_produce;
		int read_consume;
		int send_produce;
		int send_consume;
	public:
		int port_no;
		int com_type;
		void *para;
		unsigned char *read_buff_ptr;
		unsigned char *send_buff_ptr;
		int state;
	public:
		basic_com(){
			read_produce=0;
			read_consume=0;
			send_produce=0;
			send_consume=0;
			read_buff_ptr=NULL;
			send_buff_ptr=NULL;
			state=0;
			port_no=0;
			com_type=0;
			para=NULL;
		};
		virtual int init(void *para)=0;
		virtual int connect()=0;
		virtual int close()=0;
		virtual int read(int len)=0;
		virtual int get_byte(unsigned char*);
		virtual int send(unsigned char *data,int len)=0;
		virtual int get_com_state()=0;
		virtual int set_set(void *)=0;
		virtual int get_set(void *)=0;
};
int basic_com::get_byte(unsigned char *c){
	if(read_produce!=read_consume){
		*c=read_buff_ptr[read_consume];
		read_consume++;
		read_consume=(read_consume) % MAX_COM_BUFFER;
		printf("get byte:%02hhx\n",*c);
		return 0;
	}
	return -1;
}
typedef struct _serial_set{
	int baund;
	int start_bits;
	int stop_bits;
	int data_bits;
	int even;
}serial_set;
ostream & operator<<(ostream &os,serial_set &a){
	return os<<"baund:"<<a.baund<<",even:"<<a.even<<",start_bits:"<<a.start_bits<<",stop_bits:"<<a.stop_bits<<",data_bits:"<<a.data_bits<<endl;
}
class serial:public basic_com{
	private:
		serial_set set;
		FILE *f;
	public:	
		serial(){
			com_type=TYPE_SERIAL;
		}
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
		virtual int set_set(void *);
		virtual int get_set(void *);
};
int	serial::init(void *para){
	cout<<"init serial"<<",port no:"<<port_no<<",com_type:"<<com_type<<endl;
	cout<<"para: "<<set<<endl;
	return 0;
}
int serial::connect(){
	cout<<"connect serial"<<endl;
	f=fopen(g_filename,"rt");
	return 0;

}
int serial::close(){
	cout<<"close serial"<<endl;
	fclose(f);
	return 0;
}
int serial::read(int len){
	cout<<"read serial:"<<len<<endl;
	int l;
	int i;
	int c;
	int m;
	char *ret;
	char buff[100];
	unsigned char tmpbuf[100];
	if(len>100){
		pfunc(DEBUG_ERROR,"too many read size,limit to 100");
		len=100;
	}
	m=0;
	ret=fgets(buff,len,f);
	if(ret==NULL)
		return -1;
	if(buff[0]=='R')
		return -1;
	l=strlen(buff);
	for(i=0;i<l;i++){
		if(buff[i]=='T'||buff[i]=='X'||buff[i]==':'||buff[i]==' ')
			continue;

		c=strtol(&buff[i],NULL,16);
		tmpbuf[m]=c;
		i++;
		m++;
		*(read_buff_ptr+read_produce)=c;
		read_produce++;
		read_produce=read_produce % MAX_COM_BUFFER;
	}
	pdump(DEBUG_NORMAL,"read serial",&tmpbuf[0],m);
	return l;
}
int serial::send(unsigned char *data,int len){
	pdump(DEBUG_INFO,"send serial",data,len);
	return len;
}
int serial::get_com_state(){
	cout<<"get com state of serial"<<endl;
	return 0;
}
int serial::set_set(void *){
	cout<<"set set of serial"<<endl;
	return 0;
}
int serial::get_set(void *){
	cout<<"get set of serial"<<endl;
	return 0;
}

typedef struct _ethernet_set{
	int dst_ip[4];
	int local_ip[4];
	int remote_port;
	int local_port;
	int server;
}ethernet_set;
ostream & operator<<(ostream &os,ethernet_set &a){
	return os<<"dst_ip:"<<a.dst_ip[0]<<"."<<a.dst_ip[1]<<"."<<a.dst_ip[2]<<"."<<a.dst_ip[3]<<endl\
		<<",local_ip:"<<a.local_ip[0]<<"."<<a.local_ip[1]<<"."<<a.local_ip[2]<<"."<<a.local_ip[3]<<endl\
		<<",remote_port:"<<a.remote_port<<endl\
		<<",local_port:"<<a.local_port<<endl\
		<<",server:"<<a.server<<endl;
}
class ethernet:public basic_com{
	private:
		ethernet_set set;
	public:
		ethernet(){
			com_type=TYPE_ETHERNET;
		}
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
		virtual int set_set(void *);
		virtual int get_set(void *);
};

int	ethernet::init(void *para){
	cout<<"init ethernet"<<",port no:"<<port_no<<",com_type:"<<com_type<<endl;
	cout<<"para: "<<endl<<set<<endl;
	return 0;
}
int ethernet::connect(){
	cout<<"connect ethernet"<<endl;
	return 0;

}
int ethernet::close(){
	cout<<"close ethernet"<<endl;
	return 0;
}
int ethernet::read(int len){
	cout<<"read ethernet:"<<len<<endl;
	return len;
}
int ethernet::send(unsigned char *data,int len){
	cout<<"send ethernet:"<<len<<endl;
	return len;
}
int ethernet::get_com_state(){
	cout<<"get com state of ethernet"<<endl;
	return 0;
}
int ethernet::set_set(void *){
	cout<<"set set of ethernet"<<endl;
	return 0;
}
int ethernet::get_set(void *){
	cout<<"get set of ethernet"<<endl;
	return 0;
}

typedef struct _wireless_set{
	ethernet_set ip_set;
	int module_type;
}wireless_set;
ostream & operator<<(ostream &os,wireless_set &a){
	return os<<"dst_ip:"<<a.ip_set.dst_ip[0]<<"."<<a.ip_set.dst_ip[1]<<"."<<a.ip_set.dst_ip[2]<<"."<<a.ip_set.dst_ip[3]<<endl\
		<<",local_ip:"<<a.ip_set.local_ip[0]<<"."<<a.ip_set.local_ip[1]<<"."<<a.ip_set.local_ip[2]<<"."<<a.ip_set.local_ip[3]<<endl\
		<<",remote_port:"<<a.ip_set.remote_port<<endl\
		<<",local_port:"<<a.ip_set.local_port<<endl\
		<<",server:"<<a.ip_set.server<<endl\
		<<",module_type:"<<a.module_type<<endl;
}
class wireless:public basic_com{
	private:
		wireless_set set;
	public:
		wireless(){
			com_type=TYPE_WIRELESS;
		}
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
		virtual int set_set(void *);
		virtual int get_set(void *);
};

int	wireless::init(void *para){
	cout<<"init wireless"<<",port no:"<<port_no<<",com_type:"<<com_type<<endl;
	cout<<"para: "<<endl<<set<<endl;
	return 0;
}
int wireless::connect(){
	cout<<"connect wireless"<<endl;
	return 0;

}
int wireless::close(){
	cout<<"close wireless"<<endl;
	return 0;
}
int wireless::read(int len){
	cout<<"read wireless:"<<len<<endl;
	return len;
}
int wireless::send(unsigned char *data,int len){
	cout<<"send wireless:"<<len<<endl;
	return len;
}
int wireless::get_com_state(){
	cout<<"get com state of wireless"<<endl;
	return 0;
}
int wireless::set_set(void *){
	cout<<"set set of wireless"<<endl;
	return 0;
}
int wireless::get_set(void *){
	cout<<"get set of wireless"<<endl;
	return 0;
}
class com_port{
	public:
		int port_no;//port number
		int com_type;//1:serial,3:wireless net(gprs or cdma),2:enthtnet
		basic_com *com_handle;
		unsigned char read_buff[MAX_COM_BUFFER];
		unsigned char send_buff[MAX_COM_BUFFER];
	public:
		com_port(){
			port_no=0;
			com_type=0;
			com_handle=NULL;
			memset(&read_buff,0,sizeof(read_buff));
			memset(&send_buff,0,sizeof(send_buff));
		}
		void set_com_handle(basic_com*handle){
			com_handle=handle;
		}
		int set_com_para(void *set,int port){
			if(com_handle!=NULL){
				com_handle->para=set;
				com_handle->read_buff_ptr=&read_buff[0];
				com_handle->send_buff_ptr=&send_buff[0];
				com_handle->port_no=port;
				com_type=com_handle->com_type;
				port_no=port;
				return 0;
			}
			return -1;
		}
		int init(){
			if(com_handle&&com_handle->para!=NULL){
				return com_handle->init(com_handle->para);
			}
			return -1;
		}
		int connect(){
			if(com_handle==NULL)
				return -1;
			return com_handle->connect();
		};
		int close(){
			if(com_handle==NULL)
				return -1;
			return com_handle->close();
		}
		int read(int len){
			if(com_handle==NULL)
				return -1;
			return com_handle->read(len);
		};
		int get_byte(unsigned char*c){
			if(com_handle==NULL)
				return -1;
			return com_handle->get_byte(c);
		};
		int send(unsigned char *data,int len){
			if(com_handle==NULL)
				return -1;
			return com_handle->send(data,len);
		};
		int get_com_state(){
			if(com_handle==NULL)
				return -1;
			return com_handle->get_com_state();
		};
};
/****************************
 * protocol macros
****************************/
class message{
	public:
		int type;
		int from;
		int to;
		char *data;
		int len;
};
#define VAR_FRAME 1
#define FIX_FRAME 2
#define FRAME_104 3
#define FRAME_BUFF 300
class frame{
	public:
		int len;
		int valid;
		int type;
		unsigned char data[FRAME_BUFF];
		frame(){
			valid=0;
			len=0;
			memset(&data,0,sizeof(data));
		}
};
class var_frame:public frame{
	public:
		var_frame(){
			type=VAR_FRAME;	
		}
};
class fix_frame:public frame{
	public:
		fix_frame(){
			type=FIX_FRAME;
		}
};
class frame_104:public frame{
	public:
		frame_104(){
			type=FRAME_104;
		}
};

//dir:0 is master,1 is terminal
typedef struct _prm_ctl{
		unsigned char fc:4;
		unsigned char fcv:1;
		unsigned char fcb:1;
		unsigned char prm:1;
		unsigned char rev_dir:1;//unbalance is reverse/balance is dir
}prm_ctl;
typedef struct _slv_ctl{
		unsigned char fc:4;
		unsigned char dfc:1;
		unsigned char acd_rev:1;//unbalance is acd/balance is reverse
		unsigned char prm:1;
		unsigned char rev_dir:1;//unbalance is reverse/balance is dir
}slv_ctl;
typedef union _ctrl_word{
	prm_ctl pm;
	slv_ctl sl;
	unsigned char data;
}ctrl_word;
typedef struct _vsq_bit{
	unsigned char n:7;
	unsigned char sq:1;
}vsq_bit;
typedef union _vsq {
	vsq_bit	bit;
	unsigned char data;
}vsq;
typedef struct _send_cause_bit{
	unsigned char cause:6;
	unsigned char p_n:1;
	unsigned char iv:1;
	unsigned char src:8;//可选的源地址。指的是主站地址
}send_cause_bit;
typedef union _send_cause{
	send_cause_bit bit;
	unsigned short  data;
}send_cause;

//gb101中的传输规则：
//线路上低位在前，高位在后；低字节在前，高字节在后。
//define link state
#define LINK_NOCONNECT		0
#define LINK_CONNECT		1
#define ADDR_SIZE			2
#define CAUSE_SIZE			2
#define MSG_ID_SIZE			2
#define BALANCE				1

#define PROCESS_LINK		1<<0
#define PROCESS_SUMMON		1<<1
#define PROCESS_CLOCKSYN	1<<2
#define PROCESS_CLOCKRD		1<<3
#define PROCESS_RM_CTL		1<<4
#define PROCESS_TEST_LINK	1<<5
#define PROCESS_HART		1<<6
#define PROCESS_RESET		1<<7
#define PROCESS_RD_DIR		1<<8
#define PROCESS_RD_FILE		1<<9
#define PROCESS_WR_FILE		1<<10
#define PROCESS_RD_DZ		1<<11
#define PROCESS_WR_DZ		1<<12
#define PROCESS_RD_DZ_UNIT	1<<13
#define PROCESS_WR_DZ_UNIT	1<<14
#define PROCESS_SUMMON_ACC	1<<15//累计量
#define PROCESS_UPDATE		1<<16

#define COMMAND_SUMMON		100
#define COMMAND_CLOCK		103
#define COMMAND_RM_CTL		45
#define COMMAND_RM_CTL_D	46
#define COMMAND_LINK_FINI	70
#define COMMAND_TEST_LINK	104
#define COMMAND_RESET		105
#define COMMAND_FILE		210
#define COMMAND_RD_DZ		202
#define COMMAND_RD_DZ_UNIT	201
#define COMMAND_WR_DZ_UNIT	200
#define COMMAND_WR_DZ		203
#define COMMAND_SUMMON_ACC	101//累计量
#define COMMAND_UPDATE		211

//宏定义传送原因中的CODE
#define CAUSE_Per_Cyc	1
#define CAUSE_Back		2
#define CAUSE_Spont		3
#define CAUSE_Init		4
#define CAUSE_Req		5//请求，响应
#define CAUSE_Act		6//激活
#define CAUSE_Actcon	7//激活确认
#define CAUSE_Deact		8//停止激活
#define CAUSE_Deactcon	9//停止激活确认
#define CAUSE_Actterm	10//激活终止
#define CAUSE_Retrem	11
#define CAUSE_Retloc	12
#define CAUSE_File		13
/*
#define PRESERVATION   14
#define PRESERVATION   15
#define PRESERVATION   16
#define PRESERVATION   17
#define PRESERVATION   18
#define PRESERVATION   19
*/
#define CAUSE_Introgen	20
#define CAUSE_Intro1	21
#define CAUSE_Intro2	22
#define CAUSE_Intro3	23
#define CAUSE_Intro4	24
#define CAUSE_Intro5	25
#define CAUSE_Intro6	26
#define CAUSE_Intro7	27
#define CAUSE_Intro8	28
#define CAUSE_Intro9	29
#define CAUSE_Intro10	30
#define CAUSE_Intro11	31
#define CAUSE_Intro12	32
#define CAUSE_Intro13	33
#define CAUSE_Intro14	34
#define CAUSE_Intro15	35
#define CAUSE_Intro16	36
#define CAUSE_Reqcogen	37
#define CAUSE_Reqco1	38
#define CAUSE_Reqco2	39
#define CAUSE_Reqco3	40
#define CAUSE_Reqco4	41
/*
#define PRESERVATION   42
#define PRESERVATION   43
*/
#define CAUSE_Unknowntype		44
#define CAUSE_Unknowncause		45
#define CAUSE_Unknownmasteraddr	46
#define CAUSE_Unknowndataaddr	47
/*
#define PRESERVATION   48~63
*/
class cp56time2a{
	public:

};
class event{
	public:
		int id;
		cp56time2a time;
		int state;
		int read_flag;
};
#include <list>
typedef list<event> event_list;
class dir{//directory
	public:
		char file_name[30];
		int file_id;
		int file_size;
};
typedef list<dir> dir_list;
typedef list<float>cg_yc_list;
class file_segment{
	public:
		int pos;
		char content[300];
};
class buffer{
	public:
		int len;
		char content[300];
};
/****************************
 * app_layer
****************************/
class link_layer;
//app_layer is only deal asdu part
class app_layer{
	public:
		vsq vsq_lo;
		send_cause cause_lo;
		int offset;
		int addr_size;
		int cause_size;
		int msg_id_size;
		int addr;
	public:
		app_layer(){
			vsq_lo.data=0;
			cause_lo.data=0;
			get_yx_data=NULL;
			get_yc_data=NULL;
			get_event_list=NULL;
			get_clock=NULL;
			do_yk=NULL;
			get_yc_cg_data=NULL;
			get_dir_list=NULL;
			get_file_segment=NULL;
			save_file_segment=NULL;
			get_dz_unit=NULL;
			set_dz_unit=NULL;
			get_dz_data=NULL;
			set_dz=NULL;
			get_summon_acc_data=NULL;
			save_update_file=NULL;
		}
		void get_link_info(link_layer*);
		//abbreviation: 
		//act->activation
		//deact->stop active
		//con->confirm
		//resp->response
		//term->termination
		//fini->finish
		//cg->change
		int build_link_fini(frame *out,link_layer *link);
		int build_summon_con(frame *out,link_layer *link);
		int build_summon_term(frame *out,link_layer *link);
		int (*get_yx_data)(buffer*data);
		int build_yx_data(frame *out,link_layer *link,buffer*data);//cause 20
		int (*get_yc_data)(buffer*data);
		int build_yc_data(frame *out,link_layer *link,buffer*data);//cause 20

		int (*get_event_list)(event_list*from,int pos,buffer *data);
		int build_event_data(frame *out,link_layer *link,buffer *data);//cause 3

		int build_clock_con(frame *out,link_layer *link);//cause 7
		int (*get_clock)(buffer*data);
		int build_clock_resp(frame *out,link_layer *link,buffer *data);//cause 5
		
		int on_yk(frame *in,link_layer *link);//deal yk command in
		int (*do_yk)(int id,int sel);
		int build_yk_con(frame *out,link_layer *link,int sel);//cause=7,sel=0 or 1
		int build_yk_deact_con(frame *out,link_layer *link);//cause=9,sel=0
		
		int build_link_test_con(frame *out,link_layer *link);//cause 7
		
		int (*get_yc_cg_data)(cg_yc_list *list,int pos,buffer*data);
		int build_yc_cg_data(frame *out,link_layer *link,buffer*data);//cause 3
		
		int build_reset_con(frame *out,link_layer *link);//reset terminal cause 7
		
		int on_file(frame *in,link_layer *link);
		int (*get_dir_list)(dir_list *list,buffer*data);
		int build_rd_dir_resp(frame *out,link_layer *link,buffer *data);//cause 5
		int build_rd_file_con(frame *out,link_layer *link);//cause 7
		int (*get_file_segment)(char *filename,int pos,file_segment *file);
		int build_rd_file_resp(frame *out,link_layer *link,file_segment *file);//cause 5
		int build_wr_file_con(frame *out,link_layer *link);//cause 7
		int build_wr_file_resp(frame *out,link_layer *link);//cause 5
		int (*save_file_segment)(char *filename,int pos,file_segment *file);
		
		int (*get_dz_unit)(buffer*data);
		int build_rd_dz_unit_con(frame *out,link_layer *link,buffer*data);//cause 7
		int (*set_dz_unit)(int);
		int build_wr_dz_unit_con(frame *out,link_layer *link);//cause 7

		int (*get_dz_data)(int id,buffer*data);
		int build_rd_dz_con(frame *out,link_layer *link,buffer*data);//cause 7
		int on_set_dz(frame *in,link_layer *link);
		int (*set_dz)(int id,int sel,buffer*data);
		int build_dz_con(frame *out,link_layer *link,int sel);//cause 7,sel =0 or 1 ,cr=0
		int build_dz_dact_con(frame *out,link_layer *link);//cause 9,sel=0,cr=1

		int build_summon_acc_con(frame *out,link_layer *link);//cause 7
		int build_summon_acc_term(frame *out,link_layer *link);//cause 10
		int build_summon_acc_resp(frame *out,link_layer *link,buffer*data);//cause 37
		int (*get_summon_acc_data)(buffer*data);

		int on_update(frame *in,link_layer *link);
		int (*save_update_file)(char *filename,file_segment *file);
		int build_update_con(frame *out,link_layer *link,int sel);//cause 7 sel=1 start,0 stop
};
/****************************
 * common link_layer
****************************/
class link_layer{
	public:
		int port;
		com_port *com;
		app_layer*app;

		int protocol;//101 104
		int addr_size;//default 2,value 1 or 2
		int cause_size;//default 2,value 1 or 2
		int all_call_data_type;//default 11,value 9 guiyi,11 biaodu,13 float
		int msg_id_size;//default 2,value 2 or 3
		int addr;
		int rm_addr;

		int link_state;
		int link_step;

		//101的可变帧与104的帧用同一缓冲区
		var_frame last_send_frame;
		var_frame r_var_frame;
		var_frame s_var_frame;
		int start_rcv_var_flag;
		int r_var_pos;
		int s_var_pos;
		int exp_len;

		vsq vsq_rm;
		send_cause cause_rm;

		int offset_asdu;
		int offset_ti;
		int offset_vsq;
		int offset_cause;
		int offset_pub_addr;
		int offset_msg_id;
		int offset_data;

		timer rep_timer;
		int rep_times;
		timer rcv_var_timer;//接收数据超时计时器
	public:
		link_layer(){
			port=0;
			com=NULL;
			protocol=101;
			addr_size=ADDR_SIZE;
			cause_size=CAUSE_SIZE;
			msg_id_size=MSG_ID_SIZE;//101 is 2,104 is 3,so generation class 104 need redefine it.----- 
			all_call_data_type=11;

			link_state=0;
			link_step=0;
			addr=18;
			rm_addr=0;
		
			start_rcv_var_flag=0;
			r_var_pos=0;
			s_var_pos=0;

			vsq_rm.data=0;
			cause_rm.data=0;
	
			rep_times=0;
			rep_timer.stop();
			rcv_var_timer.stop();
		}
		int set_link_com(com_port*,int port);
		int set_app(app_layer*);
		virtual int get_frame()=0;//get a valid frame from port.
		virtual int active_send()=0;
		virtual int deal_frame(frame *)=0;
		virtual void set_loc_ctl()=0;
		int check_state();//cycle check link state
		int send_frame(frame *);
	public:
//the next function will build link layer frame by call app_layer same name function.
//different link layer has different link frame(eg 101 and 104).but they have same app_layer frame(asdu).
//the inheritance class must realize these virtual function.
		virtual int build_link_layer(frame *out,int)=0;//by asdu build link frame 
		virtual int build_link_fini(frame *out)=0;
		virtual int build_summon_con(frame *out)=0;
		virtual int build_summon_term(frame *out)=0;
		virtual int build_yx_data(frame *out,buffer*data)=0;
		virtual int build_yc_data(frame *out,buffer*data)=0;
		virtual int build_event_data(frame *out,buffer *data)=0;
		virtual int build_clock_con(frame *out)=0;
		virtual int build_clock_resp(frame *out,buffer *data)=0;
		virtual int build_yk_con(frame *out,int sel)=0;
		virtual int build_yk_deact_con(frame *out)=0;
		virtual int build_link_test_con(frame *out)=0;
		virtual int build_yc_cg_data(frame *out,buffer*data)=0;
		virtual int build_reset_con(frame *out)=0;
		virtual int build_rd_dir_resp(frame *out,buffer *data)=0;
		virtual int build_rd_file_con(frame *out)=0;
		virtual int build_rd_file_resp(frame *out,file_segment *file)=0;
		virtual int build_wr_file_con(frame *out)=0;
		virtual int build_wr_file_resp(frame *out)=0;
		virtual int build_rd_dz_unit_con(frame *out,buffer*data)=0;
		virtual int build_wr_dz_unit_con(frame *out)=0;
		virtual int build_rd_dz_con(frame *out,buffer*data)=0;
		virtual int build_dz_con(frame *out,int sel)=0;
		virtual int build_dz_dact_con(frame *out)=0;
		virtual int build_summon_acc_con(frame *out)=0;
		virtual int build_summon_acc_term(frame *out)=0;
		virtual int build_summon_acc_resp(frame *out,buffer*data)=0;
		virtual int build_update_con(frame *out,int sel)=0;

		void on_notify(message *msg);
		void notify(message *msg);
};
#define REP_TIMES 3
#define REP_TIME  1
int link_layer::set_link_com(com_port*c,int p){
	if(c!=NULL){
		port=p;
		com=c;
		cout<<"link link_layer "<<port<<" to com_port "<<c->port_no<<endl;
		return 0;
	}
	return -1;
}
int link_layer::set_app(app_layer*ap){
	app=ap;
	return 0;
}
int link_layer::send_frame(frame *f){
	int ret;
	if(com==NULL||f==NULL)
		return -1;
	cout<<"send frame of link "<<port<<",physical port :"<<com->port_no<<endl;
	if(f->valid==1){
		ret=com->send(f->data,f->len);
		f->valid=0;
	}
	return ret;
}
int link_layer::check_state(){
	cout<<"check state of link "<<port<<",state:"<<link_state<<endl;
	return 0;
}
/****************************
 * link_layer_101
****************************/
class link_layer_101:public link_layer{
	public:
		int balance;//1 balance,0 no balance
		//fix frame is only for 101,so i define it in this.
		fix_frame r_fix_frame;
		fix_frame s_fix_frame;
		int start_rcv_fix_flag;
		int r_fix_pos;
		int s_fix_pos;

		ctrl_word ctl_rm;//saved control word from remote.
		ctrl_word ctl_lo;
		int offset_len;
		int offset_control;
		int offset_addr;

		int has_data;
		
		timer rcv_fix_timer;//接收数据超时计时器
	public:
		link_layer_101(){
			balance=g_balance;

			start_rcv_fix_flag=0;
			r_fix_pos=0;
			s_fix_pos=0;

			rcv_fix_timer.stop();

			ctl_rm.data=0;
			ctl_lo.data=0;

			offset_len=1;
			offset_control=4;
			offset_addr=5;
			offset_asdu=offset_addr+addr_size;
			offset_ti=offset_asdu;
			offset_vsq=offset_asdu+1;
			offset_cause=offset_vsq+1;
			offset_pub_addr=offset_cause+cause_size;
			offset_msg_id=offset_pub_addr+addr_size;
			offset_data=offset_msg_id+msg_id_size;
			has_data=0;
		}
	public:
		int fc_0(frame *);
		int fc_1(frame *);
		int fc_2(frame *);
		int fc_3(frame *);
		int fc_4(frame *);
		int fc_5(frame *);
		int fc_6(frame *);
		int fc_7(frame *);
		int fc_8(frame *);
		int fc_9(frame *);
		int fc_10(frame *);
		int fc_11(frame *);
	public:
		long process;//which process is in.
		int get_frame();
		int active_send();//for balance
		int deal_frame(frame *in);
		int save_frame(frame *);
		//build fix frame
		int build_ack(frame *out,int has_data=0);//set fc=0,fix frame,has_data indicator if have class 1 data,used for unbalance.
		int build_nak(frame *out);//set fc=1, fix frame
		int build_err_rep(frame *out,int err);//?
		int build_link_ack(frame *out);//set fc=11,fix frame,
		int build_link_req(frame *out);//set fc=9,fix frame,for balance.
		int build_reset_link(frame *out);//set fc=0,fix frame,for balance
		int on_req_class_1(frame *in,frame *out);//set fc=8 or fc=9,response fc10,for unbalance,fix frame or var frame
		int on_req_class_2(frame *in,frame *out);//set fc=8 or fc=9,response fc11,for unbalance,fix frame or var frame
		int on_req(frame *in,frame *out);//set fc=0 or fc=1,response fc3,fix frame or var frame
		//set respose frame's control word.
		void set_loc_ctl();
//the next function  is implement of parent virtual function. inheritance 继承
		int build_link_layer(frame *out,int asdu_len);//by asdu build link frame.
		int build_link_fini(frame *out);//set fc=3(balance) or fc=8(unbalance),var frame
		int build_summon_con(frame *out);//set fc=3(balance) or fc=8(unbalance),var frame
		int build_summon_term(frame *out);
		int build_yx_data(frame *out,buffer*data);
		int build_yc_data(frame *out,buffer*data);
		int build_event_data(frame *out,buffer *data);
		int build_clock_con(frame *out);
		int build_clock_resp(frame *out,buffer *data);
		int build_yk_con(frame *out,int sel);
		int build_yk_deact_con(frame *out);
		int build_link_test_con(frame *out);
		int build_yc_cg_data(frame *out,buffer*data);
		int build_reset_con(frame *out);
		int build_rd_dir_resp(frame *out,buffer *data);
		int build_rd_file_con(frame *out);
		int build_rd_file_resp(frame *out,file_segment *file);
		int build_wr_file_con(frame *out);
		int build_wr_file_resp(frame *out);
		int build_rd_dz_unit_con(frame *out,buffer*data);
		int build_wr_dz_unit_con(frame *out);
		int build_rd_dz_con(frame *out,buffer*data);
		int build_dz_con(frame *out,int sel);
		int build_dz_dact_con(frame *out);
		int build_summon_acc_con(frame *out);
		int build_summon_acc_term(frame *out);
		int build_summon_acc_resp(frame *out,buffer*data);
		int build_update_con(frame *out,int sel);
};
/****************************
 *  relaize link_layer_101
****************************/
int link_layer_101::build_ack(frame*out,int has_data){
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=0;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=has_data;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=0;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	out->valid=1;
	return i;
}
int link_layer_101::build_nak(frame*out){
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=1;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=1;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	out->valid=1;
	return i;
}
int link_layer_101::build_link_ack(frame*out){
	int i;
	i=0;
	if(!balance){
		ctl_lo.sl.fc=11;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;
	}else if(balance==1){
		ctl_lo.sl.fc=11;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=0;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=1;
	}
	out->data[i++]=0x10;
	out->data[i++]=ctl_lo.data;
	out->data[i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[i++]=addr>>8&0x00ff;
	}
	out->data[i++]=sum(&out->data[1],addr_size+1);
	out->data[i++]=0x16;
	out->len=i;
	out->valid=1;
	return i;
}
int link_layer_101::build_link_req(frame*out){
	int i=0;
	if(balance==1){
		ctl_lo.pm.fc=9;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl_lo.data;
		out->data[i++]=addr&0x00ff;
		if(addr_size==2){
			out->data[i++]=addr>>8&0x00ff;
		}
		out->data[i++]=sum(&out->data[1],addr_size+1);
		out->data[i++]=0x16;
		out->len=i;
		out->valid=1;
	}	
	return 0;
}
int link_layer_101::build_reset_link(frame*out){
	int i=0;
	if(balance==1){
		ctl_lo.pm.fc=0;
		ctl_lo.pm.fcv=0;
		ctl_lo.pm.fcb=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
		out->data[i++]=0x10;
		out->data[i++]=ctl_lo.data;
		out->data[i++]=addr&0x00ff;
		if(addr_size==2){
			out->data[i++]=addr>>8&0x00ff;
		}
		out->data[i++]=sum(&out->data[1],addr_size+1);
		out->data[i++]=0x16;
		out->len=i;
		out->valid=1;
	}	
	return 0;
}
int link_layer_101::build_link_layer(frame*out,int asdu_len){
	int l;
	int len;
	switch(addr_size){
		case 0:
			l=asdu_len+1;
			break;
		case 1:
			l=asdu_len+2;
			break;
		case 2:
			l=asdu_len+3;
			break;
		default:
			pfunc(DEBUG_ERROR,"error addr_size:%d",addr_size);
			return -1;
	}
	len=l+6;
	out->data[0]=0x68;
	out->data[1]=l;
	out->data[2]=l;
	out->data[3]=0x68;
	out->data[4]=ctl_lo.data;
	out->data[5]=addr&0x00ff;
	if(addr_size==2){
		out->data[6]=addr>>8&0x00ff;
	}
	out->data[len-2]=sum(&out->data[offset_control],l);
	out->data[len-1]=0x16;
	out->len=len;
	out->valid=1;
	return len;
}
void link_layer_101::set_loc_ctl(){
	if(!balance){
		ctl_lo.sl.fc=8;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=1;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;

	}else if(balance==BALANCE){
		ctl_lo.pm.fc=3;
		ctl_lo.pm.fcv=1;
		ctl_lo.pm.fcb=!ctl_lo.pm.fcb;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
	}
}
int link_layer_101::build_link_fini(frame *out){
	int ret;
	if(!balance){
		ctl_lo.sl.fc=8;
		ctl_lo.sl.dfc=0;
		ctl_lo.sl.acd_rev=1;
		ctl_lo.sl.prm=0;
		ctl_lo.sl.rev_dir=0;

	}else if(balance==BALANCE){
		ctl_lo.pm.fc=3;
		ctl_lo.pm.fcv=1;
		ctl_lo.pm.fcb=0;
		ctl_lo.pm.prm=1;
		ctl_lo.pm.rev_dir=1;
	}
	ret=app->build_link_fini(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_summon_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_summon_con(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_summon_term(frame *out){	
	int ret;
	set_loc_ctl();
	if(!balance){
		ctl_lo.sl.acd_rev=0;//no more data;
	}
	ret=app->build_summon_term(out,this);
	return build_link_layer(out,ret);	
}
int link_layer_101::build_yx_data(frame *out,buffer*data){
	int ret;
	set_loc_ctl();
	ret=app->build_yx_data(out,this,data);
	return build_link_layer(out,ret);		
}
int link_layer_101::build_yc_data(frame *out,buffer*data){	
	int ret;
	set_loc_ctl();
	ret=app->build_yc_data(out,this,data);
	return build_link_layer(out,ret);			
}
int link_layer_101::build_event_data(frame *out,buffer *data){	
	int ret;
	set_loc_ctl();
	ret=app->build_event_data(out,this,data);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_clock_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_clock_con(out,this);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_clock_resp(frame *out,buffer *data){	
	int ret;
	set_loc_ctl();
	ret=app->build_clock_resp(out,this,data);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_yk_con(frame *out,int sel){	
	int ret;
	set_loc_ctl();
	ret=app->build_yk_con(out,this,sel);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_yk_deact_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_yk_deact_con(out,this);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_link_test_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_link_test_con(out,this);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_yc_cg_data(frame *out,buffer*data){
	int ret;
	set_loc_ctl();
	ret=app->build_yc_cg_data(out,this,data);
	return build_link_layer(out,ret);					
}
int link_layer_101::build_reset_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_reset_con(out,this);
	return build_link_layer(out,ret);					
}
int link_layer_101::build_rd_dir_resp(frame *out,buffer *data){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dir_resp(out,this,data);
	return build_link_layer(out,ret);					
}
int link_layer_101::build_rd_file_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_file_con(out,this);
	return build_link_layer(out,ret);				
}
int link_layer_101::build_rd_file_resp(frame *out,file_segment *file){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_file_resp(out,this,file);
	return build_link_layer(out,ret);								
}
int link_layer_101::build_wr_file_con(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_wr_file_con(out,this);
	return build_link_layer(out,ret);					
}
int link_layer_101::build_wr_file_resp(frame *out){
	int ret;
	set_loc_ctl();
	ret=app->build_wr_file_resp(out,this);
	return build_link_layer(out,ret);					
}
int link_layer_101::build_rd_dz_unit_con(frame *out,buffer*data){
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dz_unit_con(out,this,data);
	return build_link_layer(out,ret);
}
int link_layer_101::build_wr_dz_unit_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_wr_dz_unit_con(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_rd_dz_con(frame *out,buffer*data){	
	int ret;
	set_loc_ctl();
	ret=app->build_rd_dz_con(out,this,data);
	return build_link_layer(out,ret);
}
int link_layer_101::build_dz_con(frame *out,int sel){	
	int ret;
	set_loc_ctl();
	ret=app->build_dz_con(out,this,sel);
	return build_link_layer(out,ret);
}
int link_layer_101::build_dz_dact_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_dz_dact_con(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_summon_acc_con(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_con(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_summon_acc_term(frame *out){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_term(out,this);
	return build_link_layer(out,ret);
}
int link_layer_101::build_summon_acc_resp(frame *out,buffer*data){	
	int ret;
	set_loc_ctl();
	ret=app->build_summon_acc_resp(out,this,data);
	return build_link_layer(out,ret);
}
int link_layer_101::build_update_con(frame *out,int sel){	
	int ret;
	set_loc_ctl();
	ret=app->build_update_con(out,this,sel);
	return build_link_layer(out,ret);
}

int link_layer_101::on_req_class_1(frame *in,frame *out){
	cout<<"on_req_class_1"<<endl;
	return 0;
}
int link_layer_101::on_req_class_2(frame *in,frame *out){
	cout<<"on_req_class_2"<<endl;
	return 0;
}
int link_layer_101::on_req(frame *in,frame *out){
	int ret;
	ret=0;
	int ti;
	ti=in->data[offset_ti];
	int cause;
	cause=in->data[offset_cause];
	int act=0;//for file command
	switch(ti){
		case COMMAND_SUMMON://total sum
			process|=PROCESS_SUMMON;
			build_summon_con(out);
			break;
		case COMMAND_CLOCK://clock 
			if(cause==6){
				process|=PROCESS_CLOCKSYN;
			}else if(cause==5){
				process|=PROCESS_CLOCKRD;
			}
			break;
		case COMMAND_RM_CTL:
		case COMMAND_RM_CTL_D:
			process|=PROCESS_RM_CTL;
			break;
		case COMMAND_TEST_LINK:
			process|=PROCESS_TEST_LINK;
			break;
		case COMMAND_RESET:
			process|=PROCESS_RESET;
			break;
		case COMMAND_FILE:
			if(act==1){
				process|=PROCESS_RD_DIR;
			}else if(act==3){
				process|=PROCESS_RD_FILE;
			}else if(act==7){
				process|=PROCESS_WR_FILE;
			}
			break;
		case COMMAND_RD_DZ_UNIT:
			process|=PROCESS_RD_DZ_UNIT;
			break;
		case COMMAND_RD_DZ:
			process|=PROCESS_RD_DZ;
			break;
		case COMMAND_WR_DZ:
			process|=PROCESS_WR_DZ;
			break;
		case COMMAND_WR_DZ_UNIT:
			process|=PROCESS_WR_DZ_UNIT;
			break;
		case COMMAND_SUMMON_ACC:
			process|=PROCESS_SUMMON_ACC;
			break;
		case COMMAND_UPDATE:
			process|=PROCESS_UPDATE;
			break;
		default:
			ret=-1;
			break;
	}
	return ret;
}
int link_layer_101::save_frame(frame *f){
	if(f->type==VAR_FRAME){
		memcpy(&last_send_frame,f,sizeof(last_send_frame));
	}
	return 0;
}
int link_layer_101::get_frame(){
	int ret;
	unsigned char c;
	int fail=0;
	if(com==NULL)
		return -1;
	cout<<"get frame of link "<<port<<",physical port :"<<com->port_no<<endl;
	ret=com->get_byte(&c);
	if(ret==-1)
		return -1;
	if(c==0x10){
		if(!start_rcv_fix_flag){
			start_rcv_fix_flag=1;
			rcv_fix_timer.start(3);
			r_fix_pos=0;
		}
	}
	if(start_rcv_fix_flag){
		r_fix_frame.data[r_fix_pos]=c;
		r_fix_pos++;
		if(r_fix_pos==(addr_size+4)){
			if(r_fix_frame.data[r_fix_pos-1]==0x16){
				if(sum(&r_fix_frame.data[1],addr_size+1)==r_fix_frame.data[r_fix_pos-2]){
					r_fix_frame.len=r_fix_pos;
					r_fix_frame.valid=1;
					rcv_fix_timer.stop();
					start_rcv_fix_flag=0;
					r_fix_pos=0;
					pdump(DEBUG_INFO,"get fix frame",&r_fix_frame.data[0],r_fix_frame.len);
					deal_frame(&r_fix_frame);
					return 1;
				}else{
					fail=1;
				}
			}else{
				fail=1;
			}
		}else if(r_fix_pos>(addr_size+4)){
			fail=1;
		}
		if(rcv_fix_timer.is_reached()==1){//receive timeout
			start_rcv_fix_flag=0;
			r_fix_pos=0;
		}
	}
	if(fail){
			start_rcv_fix_flag=0;
			r_fix_pos=0;
			rcv_fix_timer.stop();
	}
	fail=0;
	if(c==0x68){
		if(!start_rcv_var_flag){
			start_rcv_var_flag=1;
			rcv_var_timer.start(3);
			r_var_pos=0;
			exp_len=3;
		}
	}
	if(start_rcv_var_flag){
		r_var_frame.data[r_var_pos]=c;
		r_var_pos++;
		if(r_var_pos==3){
			exp_len=(r_var_frame.data[r_var_pos-1]+6);
		}
		if(r_var_pos==exp_len){
			if(r_var_frame.data[r_var_pos-1]==0x16&&r_var_frame.data[offset_len]==r_var_frame.data[offset_len+1]){
				if(r_var_frame.data[r_var_pos-2]==sum(&r_var_frame.data[offset_control],exp_len-6)){
					r_var_frame.len=r_var_pos;
					r_var_frame.valid=1;
					r_var_pos=0;
					start_rcv_var_flag=0;
					rcv_var_timer.stop();
					pdump(DEBUG_INFO,"get var frame",&r_var_frame.data[0],r_var_frame.len);
					deal_frame(&r_var_frame);
					return 1;
				}else{
					fail=1;
				}
			}else{
				fail=1;
			}
		}else if(r_var_pos>exp_len){
			fail=1;
		}
	}
	if(rcv_var_timer.is_reached()==1){
			start_rcv_var_flag=0;
			r_var_pos=0;
	}
	if(fail){
			start_rcv_var_flag=0;
			r_var_pos=0;
			rcv_var_timer.stop();
	}
	return 0;		
}
int link_layer_101::active_send(){
	cout<<"active send  101 frame of link "<<port<<endl;
	if(balance==0)
		return -1;
	if(rep_timer.is_reached()==1){
		send_frame(&last_send_frame);
		rep_times++;
		if(rep_times>=REP_TIMES){
			link_state=LINK_NOCONNECT;
		}else{
			rep_timer.start(REP_TIME);
		}
	}
	return 0;
}
#define BROADCASET_ADDR 0xffff
int link_layer_101::deal_frame(frame*f){
	int ret;
	int tmp;
	ctrl_word ctl;//tmp ctl  
	send_cause cause;//tmp cause.
	int req_addr;
	if(f->type==FIX_FRAME){
		ctl.data=f->data[1];
		req_addr=f->data[2];
		if(addr_size==2){
			tmp=f->data[3];
			req_addr|=(tmp<<8&0xff00);
		}
		if(req_addr!=addr&&req_addr!=BROADCASET_ADDR){
			pfunc(DEBUG_ERROR,"invalid address\n");
			return -1;
		}
	}else if(f->type==VAR_FRAME){
		ctl.data=f->data[offset_control];
		//next routine for get src addr 
		cause.data=f->data[offset_cause];
		if(cause_size==2){
			tmp=f->data[offset_cause+1];
			cause.data|=(tmp<<8&0xff00);
			rm_addr=cause.bit.src;
		}

		req_addr=f->data[offset_addr];
		if(addr_size==2){
			tmp=f->data[offset_addr+1];
			req_addr|=(tmp<<8&0xff00);
		}
		if(req_addr!=addr&&req_addr!=BROADCASET_ADDR){
			pfunc(DEBUG_ERROR,"invalid address\n");
			return -1;
		}
	}else{
		pfunc(DEBUG_ERROR,"invalid frame type\n");
		return -1;
	}
	ret=0;
	switch(ctl.pm.fc){
		case 0:
			ret=fc_0(f);
			break;
		case 1:
			ret=fc_1(f);
			break;
		case 2:
			ret=fc_2(f);
			break;
		case 3:
			ret=fc_3(f);
			break;
		case 4:
			ret=fc_4(f);
			break;
		case 5:
			ret=fc_5(f);
			break;
		case 6:
			ret=fc_6(f);
			break;
		case 7:
			ret=fc_7(f);
			break;
		case 8:
			ret=fc_8(f);
			break;
		case 9:
			ret=fc_9(f);
			break;
		case 10:
			ret=fc_10(f);
			break;
		case 11:
			ret=fc_11(f);
			break;
	}
	return ret;
}
int link_layer_101::fc_0(frame*f){
	cout<<"fc_0"<<endl;
	if(balance!=BALANCE){
		if(link_step==2){
			link_step++;//3
			//because remote reset link so reset ctl_lo and ctl_rm
			ctl_lo.pm.fcb=0;
			ctl_rm.pm.fcb=0;
			build_ack(&s_fix_frame,1);
			send_frame(&s_fix_frame);
			link_step++;//4
		}
	}else if(balance==BALANCE){
		if(link_step==2){
			//because remote reset link so reset ctl_lo and ctl_rm
			ctl_lo.pm.fcb=0;
			ctl_rm.pm.fcb=0;
			link_step++;//3
			build_ack(&s_fix_frame);
			send_frame(&s_fix_frame);
			link_step++;//4
			build_link_req(&s_fix_frame);
			send_frame(&s_fix_frame);
			link_step++;//5
		}else if(link_step==7){
			link_step++;//8
			build_link_fini(&s_var_frame);
			send_frame(&s_var_frame);
			link_step++;//9
			save_frame(&s_var_frame);//save frame
			rep_timer.start(REP_TIME);
		}else if(link_step==9){
			link_step++;//10
			link_state=LINK_CONNECT;
			process=0;
			pfunc(DEBUG_ERROR,"connect\n");
		}else{//receive ack frame
			rep_timer.stop();
			rep_times=0;
		}
	}
	return 0;
}
int link_layer_101::fc_1(frame*f){
	cout<<"fc_1"<<endl;
	return 0;
}
int link_layer_101::fc_2(frame*f){
	cout<<"fc_2"<<endl;
	return 0;
}
int link_layer_101::fc_3(frame *f){
	cout<<"fc_3"<<endl;
	ctrl_word ctl;
	int ret;
	ret=0;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret==0){
					build_ack(&s_fix_frame,has_data);
					send_frame(&s_fix_frame);//ack
				}else{
					build_nak(&s_fix_frame);
					send_frame(&s_fix_frame);
				}
			}else{
				build_nak(&s_fix_frame);
				send_frame(&s_fix_frame);
			}
		}else{
			build_nak(&s_fix_frame);
			send_frame(&s_fix_frame);
		}
	}else if(balance==BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req(f,&s_var_frame);
				if(ret==0){
					build_ack(&s_fix_frame);
					send_frame(&s_fix_frame);//ack
					send_frame(&s_var_frame);
					save_frame(&s_var_frame);//save frame
					rep_timer.start(REP_TIME);
				}else{
					build_nak(&s_fix_frame);
					send_frame(&s_fix_frame);
				}
			}else{
				send_frame(&last_send_frame);
			}
		}else{
			build_nak(&s_fix_frame);
			send_frame(&s_fix_frame);
		}
	}
	return 0;
}
int link_layer_101::fc_4(frame*f){
	cout<<"fc_4"<<endl;
	return 0;
}
int link_layer_101::fc_5(frame*f){
	cout<<"fc_5"<<endl;
	return 0;
}
int link_layer_101::fc_6(frame*f){
	cout<<"fc_6"<<endl;
	return 0;
}
int link_layer_101::fc_7(frame*f){
	cout<<"fc_7"<<endl;
	return 0;
}
int link_layer_101::fc_8(frame*f){
	cout<<"fc_8"<<endl;
	return 0;
}
int link_layer_101::fc_9(frame*f){
	cout<<"fc_9"<<endl;
	process=PROCESS_LINK;
	link_step=1;
	link_state=LINK_NOCONNECT;
	build_link_ack(&s_fix_frame);
	send_frame(&s_fix_frame);
	link_step++;//2
	return 0;
}
int link_layer_101::fc_10(frame*f){
	cout<<"fc_10"<<endl;
	ctrl_word ctl;
	int ret;
	ret=0;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(link_step==4){
			link_step++;//5
			build_link_fini(&s_var_frame);
			send_frame(&s_var_frame);
			link_step++;//6
			link_state=LINK_CONNECT;
			pfunc(DEBUG_ERROR,"connect\n");
			process=0;
		}else{
			if(ctl.pm.fcv){
				if(ctl.pm.fcb!=ctl_rm.pm.fcb){
					ctl_rm.data=ctl.data;//save control
					ret=on_req_class_1(f,&s_var_frame);
					if(ret==0){
						send_frame(&s_var_frame);
						save_frame(&s_var_frame);//save frame
					}
				}else{
					send_frame(&last_send_frame);
				}
			}else{
				build_nak(&s_fix_frame);
				send_frame(&s_fix_frame);
			}
		}
	}
	return 0;
}
int link_layer_101::fc_11(frame *f){
	cout<<"fc_11"<<endl;
	ctrl_word ctl;
	int ret;
	ret=0;
	ctl.data=f->data[offset_control];
	if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_rm.pm.fcb){
				ctl_rm.data=ctl.data;//save control
				ret=on_req_class_2(f,&s_var_frame);
				if(ret==0){
					send_frame(&s_var_frame);
					save_frame(&s_var_frame);//save frame
				}
			}else{
				send_frame(&last_send_frame);
			}
		}else{
			build_nak(&s_fix_frame);
			send_frame(&s_fix_frame);
		}
	}else if(balance==BALANCE){
		if(link_step==5){
			link_step++;//6
			build_reset_link(&s_fix_frame);
			send_frame(&s_fix_frame);
			link_step++;//7
		}
	}	
	return 0;
}
/****************************
 *	relaize app_layer 
****************************/
void app_layer::get_link_info(link_layer*link){
	offset=link->offset_asdu;
	addr_size=link->addr_size;
	cause_size=link->cause_size;
	msg_id_size=link->msg_id_size;
	addr=link->addr;
}
//build asdu of link_fini command 
int app_layer::build_link_fini(frame*out,link_layer*link){
	int i;
	get_link_info(link);

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Init;

	i=0;
	out->data[offset+i++]=COMMAND_LINK_FINI;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	return i;
}

int app_layer::build_summon_con(frame*out,link_layer*link){
	int i;
	get_link_info(link);

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actcon;

	i=0;
	out->data[offset+i++]=COMMAND_SUMMON;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	out->data[offset+i++]=20;
	return i;
}
int app_layer::build_summon_term(frame *out,link_layer *link){
	int i;

	vsq_lo.bit.n=1;
	vsq_lo.bit.sq=0;
	cause_lo.bit.cause=CAUSE_Actterm;

	i=0;
	out->data[offset+i++]=COMMAND_SUMMON;
	out->data[offset+i++]=vsq_lo.data;
	out->data[offset+i++]=cause_lo.data;
	if(cause_size==2){
		out->data[offset+i++]=(cause_lo.data>>8&0x00ff);
	}
	out->data[offset+i++]=addr&0x00ff;
	if(addr_size==2){
		out->data[offset+i++]=addr>>8&0x00ff;
	}
	out->data[offset+i++]=0x0;
	out->data[offset+i++]=0x0;
	if(msg_id_size==3)
		out->data[offset+i++]=0;
	out->data[offset+i++]=20;
	return i;
}
int app_layer::build_yx_data(frame *out,link_layer *link,buffer*data){//cause 20
	int i;
	i=0;
	return i;
}
int app_layer::build_yc_data(frame *out,link_layer *link,buffer*data){//cause 20
	int i;
	i=0;
	return i;
}
int app_layer::build_event_data(frame *out,link_layer *link,buffer *data){//cause 3
	int i;
	i=0;
	return i;
}
int app_layer::build_clock_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_clock_resp(frame *out,link_layer *link,buffer *data){//cause 5
	int i;
	i=0;
	return i;
}
int app_layer::on_yk(frame *in,link_layer *link){//deal yk command in
	int i;
	i=0;
	return i;
}
int app_layer::build_yk_con(frame *out,link_layer *link,int sel){//cause=7,sel=0 or 1
	int i;
	i=0;
	return i;
}
int app_layer::build_yk_deact_con(frame *out,link_layer *link){//cause=9,sel=0
	int i;
	i=0;
	return i;
}
int app_layer::build_link_test_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_yc_cg_data(frame *out,link_layer *link,buffer*data){//cause 3
	int i;
	i=0;
	return i;
}
int app_layer::build_reset_con(frame *out,link_layer *link){//reset terminal cause 7
	int i;
	i=0;
	return i;
}
int app_layer::on_file(frame *in,link_layer *link){
	int i;
	i=0;
	return i;
}
int app_layer::build_rd_dir_resp(frame *out,link_layer *link,buffer *data){//cause 5
	int i;
	i=0;
	return i;
}
int app_layer::build_rd_file_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_rd_file_resp(frame *out,link_layer *link,file_segment *file){//cause 5
	int i;
	i=0;
	return i;
}
int app_layer::build_wr_file_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_wr_file_resp(frame *out,link_layer *link){//cause 5
	int i;
	i=0;
	return i;
}
int app_layer::build_rd_dz_unit_con(frame *out,link_layer *link,buffer*data){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_wr_dz_unit_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_rd_dz_con(frame *out,link_layer *link,buffer*data){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::on_set_dz(frame *in,link_layer *link){
	int i;
	i=0;
	return i;
}
int app_layer::build_dz_con(frame *out,link_layer *link,int sel){//cause 7,sel =0 or 1 ,cr=0
	int i;
	i=0;
	return i;
}
int app_layer::build_dz_dact_con(frame *out,link_layer *link){//cause 9,sel=0,cr=1
	int i;
	i=0;
	return i;
}
int app_layer::build_summon_acc_con(frame *out,link_layer *link){//cause 7
	int i;
	i=0;
	return i;
}
int app_layer::build_summon_acc_term(frame *out,link_layer *link){//cause 10
	int i;
	i=0;
	return i;
}
int app_layer::build_summon_acc_resp(frame *out,link_layer *link,buffer*data){//cause 37
	int i;
	i=0;
	return i;
}
int app_layer::on_update(frame *in,link_layer *link){
	int i;
	i=0;
	return i;
}
int app_layer::build_update_con(frame *out,link_layer *link,int sel){//cause 7 sel=1 start,0 stop
	int i;
	i=0;
	return i;
}
/****************************
 * extern interface functio 
****************************/
int get_yx_data(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_yc_data(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_event_list(event_list*from,int pos,buffer *data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_clock(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int do_yk(int id,int sel){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_yc_cg_data(cg_yc_list *list,int pos,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dir_list(dir_list *list,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_file_segment(char *filename,int pos,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int save_file_segment(char *filename,int pos,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dz_unit(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int set_dz_unit(int){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_dz_data(int id,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int set_dz(int id,int sel,buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int get_summon_acc_data(buffer*data){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
int save_update_file(char *filename,file_segment *file){
	pfunc(DEBUG_NORMAL,"\n");
	return 0;
}
/*
int get_yx_data(buffer*data);
int get_yc_data(buffer*data);
int get_event_list(event_list*from,int pos,buffer *data);
int get_clock(buffer*data);
int do_yk(int id,int sel);
int get_yc_cg_data(cg_yc_list *list,int pos,buffer*data);
int get_dir_list(dir_list *list,buffer*data);
int get_file_segment(char *filename,int pos,file_segment *file);
int save_file_segment(char *filename,int pos,file_segment *file);
int get_dz_unit(buffer*data);
int set_dz_unit(int);
int get_dz_data(int id,buffer*data);
int set_dz(int id,int sel,buffer*data);
int get_summon_acc_data(buffer*data);
int save_update_file(char *filename,file_segment *file);
*/
void set_app_interface(app_layer *app){
	app->get_yx_data=get_yx_data;
	app->get_yc_data=get_yc_data;
	app->get_event_list=get_event_list;
	app->get_clock=get_clock;
	app->do_yk=do_yk;
	app->get_yc_cg_data=get_yc_cg_data;
	app->get_dir_list=get_dir_list;
	app->get_file_segment=get_file_segment;
	app->save_file_segment=save_file_segment;
	app->get_dz_unit=get_dz_unit;
	app->set_dz_unit=set_dz_unit;
	app->get_dz_data=get_dz_data;
	app->set_dz=set_dz;
	app->get_summon_acc_data=get_summon_acc_data;
	app->save_update_file=save_update_file;
}
/****************************
 *	main 
****************************/
int main(int arg,char **argv){
	if(arg<=1){
		g_balance=1;
		strcpy(g_filename,"test.dat");
	}else if(arg<=2){
		g_balance=atoi(argv[1]);
		strcpy(g_filename,"test.dat");
	}else if(arg<=3){
		g_balance=atoi(argv[1]);
		strcpy(g_filename,argv[2]);
	}
	int i;
	com_port com[3];
	serial_set serial_set_1;
	serial serial_1;
	ethernet_set eth_set_1;
	ethernet eth_1;
	wireless_set wire_set_1;
	wireless wire_1;
	link_layer_101 link[3];
	app_layer app;
	set_app_interface(&app);

	com[0].set_com_handle(&serial_1);
	com[0].set_com_para((void *)&serial_set_1,1);
	com[0].init();
	com[1].set_com_handle(&eth_1);
	com[1].set_com_para((void *)&eth_set_1,2);
	com[1].init();
	com[2].set_com_handle(&wire_1);
	com[2].set_com_para((void *)&wire_set_1,3);
	com[2].init();
	for(i=0;i<3;i++){
		link[i].set_link_com(&com[i],i+1);
		link[i].set_app(&app);
	}
	int loops=0;
	for(i=0;i<3;i++){
			com[i].connect();
	}
	while(1){
		cout<<endl<<"run loop:"<<loops<<endl;
		loops++;
		sleep_ms(1);
		for(i=0;i<3;i++){
			com[i].read(100);
		}
		for(i=0;i<3;i++){
			link[i].check_state();
			link[i].get_frame();
			link[i].active_send();
		}

	}
	return 0;
}
// vim:tw=72
