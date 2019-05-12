/*************************************************************************
	> File Name: pseudo.cpp
	> Author: hanhj
	> Mail: hanhj@zx-jy.com 
	> Created Time: 2019年02月19日 星期二 22时09分40秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include<unistd.h>
#include<string.h>
#include<time.h>
//the physical layer
#define MAX_COMM_BUFFER 1000
#define TYPE_SERIAL 1
#define TYPE_ETHERNET 2
#define TYPE_WIRELESS 3


unsigned char sum(unsigned char *data,int len);
time_t get_system_time();

class basic_com{
	private:
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
		virtual int get_byte(char*);
		virtual int send(unsigned char *data,int len)=0;
		virtual int get_com_state()=0;
		virtual int set_set(void *)=0;
		virtual int get_set(void *)=0;
};
int basic_com::get_byte(char *c){
	cout<<"get byte"<<endl;
	if(read_produce!=read_consume){
		*c=read_buff_ptr[read_consume];
		read_consume++;
		read_consume=(read_consume) % MAX_COMM_BUFFER;
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
	return 0;

}
int serial::close(){
	cout<<"close serial"<<endl;
	return 0;
}
int serial::read(int len){
	cout<<"read serial:"<<len<<endl;
	return len;
}
int serial::send(unsigned char *data,int len){
	cout<<"send serial:"<<len<<endl;
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
		int com_type;//1:serial,2:wireless net(gprs or cdma),3:enthtnet
		basic_com *com_handle;
		unsigned char read_buff[MAX_COMM_BUFFER];
		unsigned char send_buff[MAX_COMM_BUFFER];
	public:
		com_port(){
			port_no=0;
			com_type=0;
			com_handle=NULL;
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
			return com_handle->connect();
		};
		int close(){
			return com_handle->close();
		}
		int read(int len){
			return com_handle->read(len);
		};
		int get_byte(char*c){
			return com_handle->get_byte(c);
		};
		int send(unsigned char *data,int len){
			return com_handle->send(data,len);
		};
		int get_com_state(){
			return com_handle->get_com_state();
		};
};
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
class frame{
	public:
		int len;
		int valid;
		int type;
		unsigned char data[300];
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
typedef struct _prm_ctl{
		unsigned char fc:4;
		unsigned char fcv:1;
		unsigned char fcb:1;
		unsigned char prm:1;
		unsigned char dir:1;
}prm_ctl;
typedef struct _slv_ctl{
		unsigned char fc:4;
		unsigned char dfc:1;
		unsigned char rev:1;
		unsigned char prm:1;
		unsigned char dir:1;
}slv_ctl;
typedef union _ctrl_word{
	prm_ctl pm;
	slv_ctl sl;
	unsigned char data;
}ctrl_word;

typedef struct _Vsq {
	unsigned char n:7;
	unsigned char sq:1;
}Vsq;
typedef struct _send_cause_bit{
	unsigned char cause:6;
	unsigned char p_n:1;
	unsigned char iv:1;
}send_cause_bit;
typedef union _send_cause{
	send_cause_bit scb;
	unsigned char  scd;
}send_cause;
typedef struct _asdu_header{
	unsigned char ti;
	Vsq vsq;
	send_cause cause;
	unsigned short pub_addr;
	unsigned short data_id;
}asdu_header;
//define link state
#define LINK_NOCONNECT 0
#define LINK_CONNECT 1
#define ADDR_SIZE 2
#define CAUSE_SIZE 2
#define MSG_ID_SIZE 2			
class link_layer{
	public:
		int port;
		com_port *com;

		int protocol;//101 104
		int addr_size;//default 2,value 1 or 2
		int cause_size;//default 2,value 1 or 2
		int all_call_data_type;//default 11,value 9 guiyi,11 biaodu,13 float
		int msg_id_size;//default 2,value 2 or 3
		int addr;

		int link_state;
		int link_step;
		asdu_header asdu;

		timer rep_timer;
		int rep_times;
	public:
		link_layer(){
			port=0;
			com=NULL;
			protocol=101;
			addr_size=ADDR_SIZE;
			cause_size=CAUSE_SIZE;
			msg_id_size=MSG_ID_SIZE;
			all_call_data_type=11;
			addr=1;

			link_state=0;
			link_step=0;
			addr=0;
			memset(&asdu,0,sizeof(asdu));
			
			rep_times=0;
			rep_timer.stop();
		}
		int set_link_com(com_port*,int port);
		virtual int get_frame()=0;//get a valid frame from port .if ok notify uplayer;
		virtual int active_send()=0;//when receive message from uplayer ,analyze data and fill control word,len,cs.and save last frame.start timer.
		virtual int deal_frame(frame *)=0;
		int check_state();//cycle check link.do resend 
		int send_frame(frame *);
	public:
		void on_notify(message *msg);
		void notify(message *msg);
};

int link_layer::set_link_com(com_port*c,int p){
	if(c!=NULL){
		port=p;
		com=c;
		cout<<"link link_layer "<<port<<" to com_port "<<c->port_no<<endl;
		return 0;
	}
	return -1;
}
int link_layer::send_frame(frame *f){
	if(com==NULL)
		return -1;
	if(f==NULL)
		return -1;
	cout<<"send frame of link "<<port<<",physical port :"<<com->port_no<<endl;
	return com->send(f->data,f->len);
}
int link_layer::check_state(){
	cout<<"check state of link "<<port<<",state:"<<link_state<<endl;
	return 0;
}
typedef struct _fc_table{
	int fc;
	int (*func)(frame*);
}fc_table;

#define BALANCE 1

#define PROCESS_LINK		1
#define PROCESS_SUMMON		2
#define PROCESS_CLOCKSYN	3
#define PROCESS_CLOCKRD		4
#define PROCESS_RM_CTL		5
#define PROCESS_TEST_LINK	6
#define PROCESS_HART		7
#define PROCESS_RESET		8
#define PROCESS_RD_DIR		9
#define PROCESS_RD_FILE		10 
#define PROCESS_WR_FILE		11
#define PROCESS_RD_DZ		12
#define PROCESS_WR_DZ		13
#define PROCESS_RD_DZ_UNIT	14
#define PROCESS_WR_DZ_UNIT	15
#define PROCESS_SUMMON_ACC	16//累计量
#define PROCESS_UPDATE		17

#define COMMAND_SUMMON		100
#define COMMAND_CLOCK		103
#define COMMAND_RM_CTL		45
#define COMMAND_RM_CTL_D	46
#define COMMAND_TEST_LINK	104
#define COMMAND_RESET		105
#define COMMAND_FILE		210
#define COMMAND_RD_DZ		202
#define COMMAND_RD_DZ_UNIT	201
#define COMMAND_WR_DZ_UNIT	200
#define COMMAND_WR_DZ		203
#define COMMAND_SUMMON_ACC	101//累计量
#define COMMAND_UPDATE		211

#define REP_TIMES 3
#define REP_TIME  1
class link_layer_101:public link_layer{
	public:
		int balance;//1 balance,0 no balance
		
		var_frame last_send_frame;
		var_frame r_var_frame;
		var_frame s_var_frame;
		int start_rcv_var_flag;
		int r_var_pos;
		int s_var_pos;

		fix_frame r_fix_frame;
		fix_frame s_fix_frame;
		int start_rcv_fix_flag;
		int r_fix_pos;
		int s_fix_pos;

		ctrl_word ctl_wd_rm;//saved control word from remote.
		ctrl_word ctl_wd_lo;
		int offset_len;
		int offset_control;
		int offset_addr;
		int offset_asdu;
		int offset_ti;
		int offset_vsq;
		int offset_cause;
		int offset_pub_addr;
		int offset_msg_id;
		int offset_data;

		int has_data;
		
		timer rcv_var_timer;//接收数据超时计时器
		timer rcv_fix_timer;//接收数据超时计时器
	public:
		link_layer_101(){
			balance=0;
			memset(&last_send_frame,0,sizeof(last_send_frame));
			memset(&r_var_frame,0,sizeof(r_var_frame));
			memset(&s_var_frame,0,sizeof(s_var_frame));
			start_rcv_var_flag=0;
			r_var_pos=0;
			s_var_pos=0;

			memset(&r_fix_frame,0,sizeof(r_fix_frame));
			memset(&s_fix_frame,0,sizeof(s_fix_frame));
			start_rcv_fix_flag=0;
			r_fix_pos=0;
			s_fix_pos=0;

			rcv_var_timer.stop();
			rcv_fix_timer.stop();

			ctl_wd_rm.data=0;
			ctl_wd_lo.data=0;
			offset_len=1;
			offset_control=4;
			offset_addr=5;
			offset_asdu=6;
			offset_ti=6;
			offset_vsq=7;
			offset_cause=8;
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
		int process;//which process is in.
		int build_ack();
		int build_nak();
		int build_ack_data();
		int build_nak_data();
		int build_link_ack(int has_data);
		int build_link_req();
		int build_link_fini();
		int build_reset_link();
		int get_frame();
		int active_send();//only for balance
		int deal_frame(frame *);
		int on_req_class_1(frame *);
		int on_req_class_2(frame *);
		int on_req(frame*,int balance);
		int save_frame(frame *);
};
int link_layer_101::build_ack(){
	return 0;
}
int link_layer_101::build_nak(){
	return 0;
}
int link_layer_101::build_link_req(){
	return 0;
}
int link_layer_101::build_link_fini(){
	return 0;
}
int link_layer_101::build_link_ack(int h){
	return 0;
}
int link_layer_101::build_reset_link(){
	return 0;
}
int link_layer_101::build_ack_data(){
	return 0;
}
int link_layer_101::build_nak_data(){
	return 0;
}
int link_layer_101::on_req_class_1(frame*f){
	cout<<"on_req_class_1"<<endl;
	return 0;
}
int link_layer_101::on_req(frame*f,int balance){
	int ti;
	ti=f->data[offset_ti];
	int cause;
	cause=f->data[offset_cause];
	int act=0;//for file command
	switch(ti){
		case COMMAND_SUMMON://total sum
			process=PROCESS_SUMMON;
			break;
		case COMMAND_CLOCK://clock 
			if(cause==6){
				process=PROCESS_CLOCKSYN;
			}else if(cause==5){
				process=PROCESS_CLOCKRD;
			}
			break;
		case COMMAND_RM_CTL:
		case COMMAND_RM_CTL_D:
			process=PROCESS_RM_CTL;
			break;
		case COMMAND_TEST_LINK:
			process=PROCESS_TEST_LINK;
			break;
		case COMMAND_RESET:
			process=PROCESS_RESET;
			break;
		case COMMAND_FILE:
			if(act==1){
				process=PROCESS_RD_DIR;
			}else if(act==3){
				process=PROCESS_RD_FILE;
			}else if(act==7){
				process=PROCESS_WR_FILE;
			}
			break;
		case COMMAND_RD_DZ_UNIT:
			process=PROCESS_RD_DZ_UNIT;
			break;
		case COMMAND_RD_DZ:
			process=PROCESS_RD_DZ;
			break;
		case COMMAND_WR_DZ:
			process=PROCESS_WR_DZ;
			break;
		case COMMAND_WR_DZ_UNIT:
			process=PROCESS_WR_DZ_UNIT;
			break;
		case COMMAND_SUMMON_ACC:
			process=PROCESS_SUMMON_ACC;
			break;
		case COMMAND_UPDATE:
			process=PROCESS_UPDATE;
			break;
	}
	return 0;
}
int link_layer_101::on_req_class_2(frame *f){
	cout<<"on_req_class_2"<<endl;
	return 0;
}
int link_layer_101::save_frame(frame *f){
	if(f->type==VAR_FRAME){
		memcpy(&last_send_frame,f,sizeof(last_send_frame));
	}
	return 0;
}
int link_layer_101::get_frame(){
	int ret;
	char c;
	int exp_len;
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
			if(r_fix_frame.data[r_fix_pos]==0x16){
				if(sum(&r_fix_frame.data[0],4)==r_fix_frame.data[r_fix_pos-2]){
					r_fix_frame.len=r_fix_pos;
					r_fix_frame.valid=1;
					rcv_fix_timer.stop();
					start_rcv_fix_flag=0;
					r_fix_pos=0;
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
			exp_len=0;
		}
	}
	if(start_rcv_var_flag){
		r_var_frame.data[r_var_pos]=c;
		r_var_pos++;
		if(r_var_pos==3){
			exp_len=(r_var_frame.data[r_var_pos]+5);
		}
		if(r_var_pos==exp_len){
			if(r_var_frame.data[r_var_pos]==0x16&&r_var_frame.data[offset_len]==r_var_frame.data[offset_len+1]){
				if(r_var_frame.data[r_var_pos-2]==sum(&r_var_frame.data[offset_control],exp_len-5)){
					r_var_frame.len=r_var_pos;
					r_var_frame.valid=1;
					r_var_pos=0;
					start_rcv_var_flag=0;
					rcv_var_timer.stop();
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
	cout<<"active send  101 frame of link "<<port<<" len="<<s_var_frame.len<<endl;
	if(rep_timer.is_reached()==1){
		send_frame(&last_send_frame);
		rep_times++;
		if(rep_times==REP_TIMES){
			link_state=LINK_NOCONNECT;
		}
	}
	return 0;
}
int link_layer_101::deal_frame(frame*f){
	int ret;
	ctrl_word ctl;
	ctl.data=f->data[offset_control];
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
			build_ack();
			send_frame(&s_fix_frame);
			link_step++;//4
		}
	}else if(balance==BALANCE){
		if(link_step==2){
			link_step++;//3
			build_ack();
			send_frame(&s_fix_frame);
			link_step++;//4
			build_link_req();
			send_frame(&s_fix_frame);
			link_step++;//5
		}else if(link_step==7){
			link_step++;//8
			build_ack();
			send_frame(&s_fix_frame);
			build_link_fini();
			send_frame(&s_var_frame);
			link_step++;//9
		}else if(link_step==9){
			link_step++;//10
			link_state=LINK_CONNECT;
			process=0;
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
	if(balance==BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_wd_rm.pm.fcb){
				ctl_wd_rm.data=ctl.data;//save control
				build_ack();
				send_frame(&s_fix_frame);//ack
				ret=on_req(f,balance);
				if(ret==1){
					send_frame(&s_var_frame);
					save_frame(&s_var_frame);//save frame
					rep_timer.start(REP_TIME);
				}
			}else{
				send_frame(&last_send_frame);
			}
		}else{
			build_nak();
			send_frame(&s_fix_frame);
		}
	}else if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_wd_rm.pm.fcb){
				ctl_wd_rm.data=ctl.data;//save control
				build_ack();
				send_frame(&s_fix_frame);//ack
			}else{
				build_nak();
				send_frame(&s_fix_frame);
			}
		}else{
			build_nak();
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
	build_link_ack(0);
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
			build_link_fini();
			send_frame(&s_var_frame);
			link_step++;//6
			link_state=LINK_CONNECT;
			process=0;
		}else{
			if(ctl.pm.fcv){
				if(ctl.pm.fcb!=ctl_wd_rm.pm.fcb){
					ctl_wd_rm.data=ctl.data;//save control
					ret=on_req_class_1(f);
					if(ret==1){
						send_frame(&s_var_frame);
						save_frame(&s_var_frame);//save frame
					}
				}else{
					send_frame(&last_send_frame);
				}
			}else{
				if(has_data){
					build_ack_data();
					send_frame(&s_fix_frame);
				}else{
					build_nak_data();
					send_frame(&s_fix_frame);
				}
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
	if(balance==BALANCE){
		if(link_step==5){
			link_step++;//6
			build_reset_link();
			send_frame(&s_fix_frame);
			link_step++;//7
		}
	}else if(balance!=BALANCE){
		if(ctl.pm.fcv){
			if(ctl.pm.fcb!=ctl_wd_rm.pm.fcb){
				ctl_wd_rm.data=ctl.data;//save control
				ret=on_req_class_2(f);
				if(ret==1){
					send_frame(&s_var_frame);
					save_frame(&s_var_frame);//save frame
				}
			}else{
				send_frame(&last_send_frame);
			}
		}else{
			if(has_data){
				build_ack_data();
				send_frame(&s_fix_frame);
			}else{
				build_nak_data();
				send_frame(&s_fix_frame);
			}
		}
	}
	return 0;
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
	}
	return _sum;
}
int main(){
	int i;
	com_port com[3];
	serial_set serial_set_1;
	serial serial_1;
	ethernet_set eth_set_1;
	ethernet eth_1;
	wireless_set wire_set_1;
	wireless wire_1;
	link_layer_101 link[3];

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
	}
	int loops=0;
	
	while(1){

		cout<<endl<<"run loop:"<<loops<<endl;
		loops++;
		sleep(2);
		for(i=0;i<3;i++){
			com[i].connect();
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
