/*
 * File Name: protocol.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 13:21:19 week:5
*/
#ifndef __protocol_h
#define __protocol_h
using namespace std;
#include "data.h"
#include "com.h"
#include "utilities.h"
#include "configurable.h"
#include "system.h"
#define MAX_SEND_YX_PER_FRAME 28 
#define MAX_SEND_YC_PER_FRAME 16 
 /****************************
 * protocol macros
****************************/
#define PRO_101 1
#define PRO_104 2

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
#define FRAME_BUFF 300
class frame{
	public:
		int len;
		int type;
		int id;//frame no
		unsigned char data[FRAME_BUFF];
		frame(){
			len=0;
			id=0;
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
//data struct for 101
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
//define data struct for 104 
typedef struct __byte_data{
	unsigned char d1;
	unsigned char d2;
	unsigned char d3;
	unsigned char d4;
}_byte_data;
struct sfmt_bit{
	unsigned long res1:1;
	unsigned long res2:1;
	unsigned long res3:15;
	unsigned long r_no:15;
};
typedef union _sfmt{
	struct sfmt_bit bit;
	_byte_data data;
	_sfmt(){
		bit.res1=1;
		bit.res2=0;
		bit.res3=0;
		bit.r_no=0;
	}
}sfmt;
struct ifmt_bit{
	unsigned long res1:1;
	unsigned long s_no:15;
	unsigned long res2:1;
	unsigned long r_no:15;
};
typedef union _ifmt{
	struct ifmt_bit bit;
	_byte_data data;
	_ifmt(){
		bit.res1=0;
		bit.res2=0;
		bit.s_no=0;
		bit.r_no=0;
	}
}ifmt;
typedef struct __ufmt_bit{
	unsigned long res1:1;
	unsigned long res2:1;
	unsigned long startdt_cmd:1;
	unsigned long startdt_ack:1;
	unsigned long stopdt_cmd:1;
	unsigned long stopdt_ack:1;
	unsigned long testfr_cmd:1;
	unsigned long testfr_ack:1;
	unsigned long res3:8;
	unsigned long res4:1;
	unsigned long res5:15;
}_ufmt_bit;
typedef union _ufmt{
	_ufmt_bit  bit;
	_byte_data data;
	_ufmt(){
		data.d1=3;
		data.d2=0;
		data.d3=0;
		data.d4=0;
	}
}ufmt;
//define data struct for asdu
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
typedef struct _siq_bit{
	unsigned char spi:1;//0开，1合
	unsigned char res:3;//保留
	unsigned char bl:1;//0未被闭锁
	unsigned char sb:1;//0未被取代
	unsigned char nt:1;//0当前值
	unsigned char iv:1;//0有效
}siq_bit;
typedef struct _diq_bit{
	unsigned char dpi:2;//1开，2合
	unsigned char res:2;//保留
	unsigned char bl:1;
	unsigned char sb:1;
	unsigned char nt:1;
	unsigned char iv:1;
}diq_bit;
typedef union _siq{
	siq_bit bit;
	unsigned char data;
}siq;
typedef union _diq{
	diq_bit bit;
	unsigned char data;
}diq;
typedef struct _sco_bit{//单命令
	unsigned char scs:1;//0开，1合
	unsigned char res:1;
	unsigned char qu:5;
	unsigned char sel:1;
}sco_bit;
typedef struct _dco_bit{//双命令
	unsigned char dcs:2;//1开，2合
	unsigned char qu:5;
	unsigned char sel:1;
}dco_bit;
typedef union _sco{
	sco_bit bit;
	unsigned char data;
}Sco;
typedef union _dco{
	dco_bit bit;
	unsigned char data;
}Dco;
typedef struct _cio_bit{//初始化限定词
	unsigned char b1:7;//1本地电源合上，1本地复位，2远程复位
	unsigned char b2:1;//0未改变参数，1改变参数。
}cio_bit;
typedef union _cio{
	cio_bit bit;
	unsigned char data;
}cio;
typedef struct _qoi{//召唤限定词
	unsigned char data;//20总召唤，21第一组，。。。36第三十六组
}qoi;
//gb101中的传输规则：
//线路上低位在前，高位在后；低字节在前，高字节在后。
//define link state
#define LINK_DISCONNECT		0
#define LINK_CONNECT		1
#define LINK_CLOSE			3
#define LINK_OPEN			4
#define LINK_ERROR			5
//规约系统参数
#define ADDR_SIZE			2
#define CAUSE_SIZE			2
#define MSG_ID_SIZE_101		2
#define MSG_ID_SIZE_104		3
#define BALANCE				1
#define UNBALANCE			0
//正在执行的过程
#define PROCESS_LINK		0x01
#define PROCESS_SUMMON		0x02	
#define PROCESS_CLOCK		0x04
#define PROCESS_EVENT		0x08
#define PROCESS_RM_CTL		0x10
#define PROCESS_TEST_LINK	0x20
#define PROCESS_YC_CHANGE	0x40
#define PROCESS_RESET		0x80
#define PROCESS_FILE		0x200
#define PROCESS_RD_DZ		0x800
#define PROCESS_WR_DZ		0x1000
#define PROCESS_RD_UNIT		0x2000
#define PROCESS_WR_UNIT		0x4000
#define PROCESS_SUMMON_ACC	0x8000//累计量
#define PROCESS_UPDATE		0x10000
//功能码
#define COMMAND_SP			1
#define COMMAND_DP			2
#define COMMAND_GUIYI		9
#define COMMAND_BIAODU		11
#define COMMAND_FLOAT		13
#define COMMAND_SP_TIME		30
#define COMMAND_DP_TIME		31
#define COMMAND_RM_CTL		45
#define COMMAND_RM_CTL_D	46
#define COMMAND_LINK_FINI	70
#define COMMAND_SUMMON		100
#define COMMAND_SUMMON_ACC	101//累计量
#define COMMAND_CLOCK		103
#define COMMAND_TEST_LINK	104
#define COMMAND_RESET		105
#define COMMAND_WR_UNIT		200
#define COMMAND_RD_UNIT		201
#define COMMAND_RD_DZ		202
#define COMMAND_WR_DZ		203
#define COMMAND_ACC			206
#define COMMAND_ACC_TIME	207
#define COMMAND_FILE		210
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
/****************************
 * app_layer
****************************/
class link_layer;
//process data
typedef struct __summon_data{
	int sended_yx_num;
	int sended_yc_num;
	int sended_acc_num;
	int step;
}_summon_data;
typedef struct __clock_data{
	int clock_syn;
	int clock_rd;
	CP56Time2a time;
}_clock_data;
#define YK_SEL 1
#define YK_ACT 2
#define YK_ACTOVER 3
#define YK_DEACTIVE 4
typedef struct __yk_data{
	int cur_state;
	int cmd_id;
	int ctrl_id;
	int fail;
	Sco sco;
	Dco dco;
	timer time;
}_yk_data;
typedef struct __event_data{
	int need_ack[4];
	int need_yc_ack[4];
}_event_data;
typedef struct __rd_dir{
	long id;
	int  len;
	char name[30];
	int flag;
	union{
		CP56Time2a time;
		unsigned char data[7];
	}start_time;
	union{
		CP56Time2a time;
		unsigned char data[7];
	}end_time;
	dir_list res_list;
	int cur_read;
	int con;
}_rd_dir;
typedef struct __rd_file{
	long ack_offset;
	long cur_offset;
	char con;//0无后续，1有后续
	int suc;//successful
	int	step;
	dir_node req_file;
	dir_node res_file;
	long result;
	long file_size;
	unsigned char sum;
	buffer segment;
}_rd_file;
#define FILE_NO_ERROR 0
#define FILE_UNKNOWN_ERROR 1
#define FILE_UNSUPPORTNAME_ERROR 2
#define FILE_LONGNAME_ERROR 3
#define FILE_CHECK_ERROR 2
#define FILE_FILE_SIZE_ERROR 3
#define FILE_FILE_ID_ERROR 4
typedef struct __update_flag{
	int start;
	int save_ok;
}_update_flag;
typedef struct __file_data{
	int op;
	_rd_dir rd_dir;
	_rd_file rd_file;
	_rd_file wt_file;
	_update_flag update_flag;
}_file_data;
typedef struct _para_node{
	int id;
	int tag;
	int len;
	char para[30];
}para_node;
typedef struct _para_iden_bit{
	unsigned char con:1;//0 no succed;1 have succeed
	unsigned char res:5;
	unsigned char cr:1;//0 -no use. 1-cancel prepare set
	unsigned char se:1;//0 -fix.1- prepare set
}para_iden_bit;
typedef union _para_iden{
	para_iden_bit bit;
	unsigned char data;
}para_iden;
typedef struct __para_list{
	int op;//操作:0- none,1-read mul para,2-read all para,3-prepare set,4-fix para,5-cancel set
	para_iden pi;//parameter identification
	int unit;//unit
	int max_unit;
	int min_unit;
	int result;
	int cur_read;
	int req_num;
	int req_id[40];
	int res_num;
	para_node nodes[40];
}_para_list;
//app_layer deal asdu part
class app_layer{
	public:
		vsq vsq_lo;
		send_cause cause_lo;
		int cmd_id;
		int offset_asdu;//position of asdu in frame.
		int addr_size;
		int cause_size;
		int msg_id_size;
		int addr;
		int yc_data_type;
		int need_reset;
		int need_update;
	public:
		app_layer(){
			vsq_lo.data=0;
			cause_lo.data=0;
			get_yx_data=NULL;
			get_yc_data=NULL;
			get_acc_yc_data=NULL;
			get_event_data=NULL;
			get_clock = NULL;
			do_yk=NULL;
			do_update=NULL;
			do_reset=NULL;
			get_yc_cg_data=NULL;
			get_dir_data=NULL;
			get_file_segment=NULL;
			save_file_segment=NULL;
			get_dz_unit=NULL;
			set_dz_unit=NULL;
			get_dz_data=NULL;
			set_dz=NULL;
			yc_data_type=9;
			need_reset=0;
			need_update=0;
		}
		int get_link_info(link_layer*);
		void set_yc_data_type(int da){
			yc_data_type=da;
		}
		//abbreviation: 
		//act->activation
		//deact->stop active
		//con->confirm
		//resp->response
		//term->termination
		//fini->finish
		//cg->change
		int build_asdu_header(frame *out);
		int build_link_fini(frame *out,link_layer *link);
		int build_summon_con(frame *out,link_layer *link);
		int build_summon_term(frame *out,link_layer *link);
		yxdata_t * (*get_yx_data)(int);
		int build_yx_data(frame *out,link_layer *link);//cause 20
		int build_dyx_data(frame *out,link_layer *link);//cause 20
		ycdata_t * (*get_yc_data)(int);
		int build_yc_data(frame *out,link_layer *link);//cause 20

		int (*get_event_data)(int type,event *&e,int change);
		int build_event_data(frame *out,link_layer *link,event *e);//cause 3

		int build_clock(frame *out,link_layer *link,CP56Time2a *time);//cause	5,7
		int (*get_clock)(CP56Time2a &);
		int (*set_clock)(CP56Time2a &);
		
		int (*do_yk)(int id,int type,int cmd);
		int build_yk(frame *out,link_layer *link);//cause=7,sel=0 or 1
		
		int build_link_test_con(frame *out,link_layer *link);//cause 7
		
		int (*get_yc_cg_data)(int port ,event_yc *&);
		int build_yc_cg_data(frame *out,link_layer *link,event_yc *e);//cause 3
		
		void (*do_reset)();
		int build_reset_con(frame *out,link_layer *link);//reset terminal cause 7

		
		int (*get_dir_data)(_rd_dir *);
		int build_rd_dir_resp(frame *out,link_layer *link,_rd_dir *);//cause 5
		
		int (*get_file_data)(_rd_file *);
		int build_rd_file_con(frame *out,link_layer *link,_rd_file *);//cause 7
		int (*get_file_segment)(_rd_file *);
		int build_rd_file_resp(frame *out,link_layer *link,_rd_file *file);//cause 5

		int (*save_file_data)(_rd_file *);
		int build_wr_file_con(frame *out,link_layer *link,_rd_file *file);//cause 7
		int (*save_file_segment)(_rd_file *);
		int build_wr_file_resp(frame *out,link_layer *link,_rd_file *file);//cause 5
		
		int (*get_dz_unit)(_para_list *);
		int build_rd_unit_con(frame *out,link_layer *link,_para_list *);//cause 7
		int (*set_dz_unit)(int);
		int build_wr_unit_con(frame *out,link_layer *link,_para_list *);//cause 7

		int (*get_dz_data)(para_node * );
		int build_rd_dz_con(frame *out,link_layer *link,_para_list *);//cause 7
		int (*set_dz)(int,para_node *);
		int build_wr_dz_con(frame *out,link_layer *link,_para_list *);//cause 7,sel =0 or 1 ,cr=0

		ycdata_t * (*get_acc_yc_data)(int);
		int build_summon_acc_con(frame *out,link_layer *link);//cause 7
		int build_summon_acc_term(frame *out,link_layer *link);//cause 10
		int build_summon_acc_resp(frame *out,link_layer *link);//cause 37

		int (*do_update)();
		int build_update_con(frame *out,link_layer *link,_update_flag *);//cause 7 sel=1 start,0 stop
};
/****************************
 * common link_layer
****************************/
class link_layer{
	public:
		int balance;//1 balance,0 no balance
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
		int has_data;

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
		int offset_len;//for 101,offset of length 


		unsigned long process;//which process is in.
		_summon_data summon_data;
		_clock_data clock_data;
		_yk_data yk_data;
		_event_data event_data;
		_file_data file_data;
		_para_list para_data;

	public:
		link_layer(){
			memset(&summon_data,0,sizeof(summon_data));
			memset(&clock_data,0,sizeof(clock_data));
			memset(&yk_data,0,sizeof(yk_data));
			memset(&event_data,0,sizeof(event_data));
			memset(&file_data,0,sizeof(file_data));
			memset(&para_data,0,sizeof(para_data));
			port=0;
			com=NULL;
			all_call_data_type=11;
			link_state=0;
			link_step=0;
			addr=18;
			rm_addr=0;
			exp_len=0;
		
			vsq_rm.data=0;
			cause_rm.data=0;

			summon_data.sended_yx_num =0;
			summon_data.sended_yc_num =0;
			summon_data.sended_acc_num=0;
			summon_data.step = 0;
			process=0;
			offset_len=1;
			has_data=0;
		}
	public:
		//the next functions will build link layer frame by call same name function in app_layer.
		//different link layer has different link frame(eg 101 and 104).but they have same app_layer frame(asdu).
		//the inheritance class must realize these virtual function.
		virtual int build_link_layer(frame *out,int)=0;//by asdu build link frame 
		virtual void deal_timeout()=0;
		virtual int get_frame()=0;//get a valid frame from port.
		virtual int link_time()=0;
		virtual int deal_frame(frame *)=0;
		virtual void set_loc_ctl()=0;

		void set_balance(int da){
			balance = da;
		}
		int set_link_com(com_port*);
		int set_app(app_layer*);
		void reset_yk_data();
		int send_frame(frame *);
		int check_state();//cycle check link state

		int build_link_fini(frame *out);//
		int on_link(frame *in,frame *out);
		int process_link(frame *out);//
		int on_summon(frame *in,frame *out);
		int process_summon(frame *out);//set fc=3(balance) or fc=8(unbalance),var frame
		int on_summon_acc(frame *in,frame *out);
		int process_summon_acc(frame *out);
		int on_clock(frame *in,frame *out);
		int process_clock(frame *out);
		int on_yk(frame *in,frame *out);
		int process_yk(frame *out);
		int process_event(frame *out);
		int on_test_link(frame *in,frame *out);
		int process_test_link(frame *out);
		int process_yc_change(frame *out);
		int on_reset_terminal(frame *in,frame *out);
		int process_reset_terminal(frame *out);
		int on_file(frame *in,frame *out);
		int process_file(frame *out);
		int on_rd_dz(frame *in,frame *out);
		int process_rd_dz(frame *out);
		int on_rd_unit(frame *in,frame *out);
		int process_rd_unit(frame *out);
		int on_wr_dz(frame *in,frame *out);
		int process_wr_dz(frame *out);
		int on_wr_unit(frame *in,frame *out);
		int process_wr_unit(frame *out);
		int on_update(frame *in,frame *out);
		int process_update(frame *out);

		void on_notify(message *msg);
		void notify(message *msg);
};
/****************************
 * link_layer_101
****************************/
class link_layer_101:public link_layer{
	public:
		//fix frame is only for 101,so i define it in this.
		//101的可变帧与104的帧用同一缓冲区
		var_frame r_var_frame;
		var_frame s_var_frame;
		fix_frame r_fix_frame;
		fix_frame s_fix_frame;
		int start_rcv_fix_flag;
		int start_rcv_var_flag;
		int r_fix_pos;
		int r_var_pos;
		int s_var_pos;

		ctrl_word ctl_rm;//saved control word from remote.
		ctrl_word ctl_lo;
		int offset_control;
		int offset_addr;

		var_frame last_send_frame;
		fix_frame last_fix_frame;
		int last_type;
		int rep_times;
		timer rep_timer;
		timer rcv_var_timer;//接收数据超时计时器
		timer rcv_fix_timer;//接收数据超时计时器
	public:
		link_layer_101(){
			balance=0;
			start_rcv_fix_flag=0;
			r_fix_pos=0;

			rep_times=0;
			rep_timer.stop();
			rcv_var_timer.stop();
			rcv_fix_timer.stop();
			start_rcv_var_flag=0;
			r_var_pos=0;
			s_var_pos=0;

			ctl_rm.data=0;
			ctl_lo.data=0;

			protocol=PRO_101;
			addr_size=ADDR_SIZE;
			cause_size=CAUSE_SIZE;
			msg_id_size=MSG_ID_SIZE_101;//101 is 2,104 is 3,so generation class 104 need redefine it.----- 
			offset_control=4;
			offset_addr=5;
			offset_asdu=offset_addr+addr_size;
			offset_ti=offset_asdu;
			offset_vsq=offset_asdu+1;
			offset_cause=offset_vsq+1;
			offset_pub_addr=offset_cause+cause_size;
			offset_msg_id=offset_pub_addr+addr_size;
			offset_data=offset_msg_id+msg_id_size;
		}
	public:
		int on_fc0(frame *);
		int on_fc1(frame *);
		int on_fc2(frame *);
		int on_fc3(frame *);
		int on_fc4(frame *);
		int on_fc5(frame *);
		int on_fc6(frame *);
		int on_fc7(frame *);
		int on_fc8(frame *);
		int on_fc9(frame *);
		int on_fc10(frame *);
		int on_fc11(frame *);
	public:
		int save_frame(frame *,int );
		int send_last_frame();
		//build fix frame
		int build_ack(frame *out,int has_data=0);//set fc=0,fix frame,has_data indicator if have class 1 data,used for unbalance.
		int build_nak(frame *out);//set fc=1, fix frame
		int build_link_ack(frame *out);//set fc=11,fix frame,
		int build_link_req(frame *out);//set fc=9,fix frame,for balance.
		int build_reset_link(frame *out);//set fc=0,fix frame,for balance
		int on_req_class_1(frame *in,frame *out);//set fc=8 or fc=9,response fc10,for unbalance,fix frame or var frame,for unbalance
		int on_req_class_2(frame *in,frame *out);//set fc=8 or fc=9,response fc11,for unbalance,fix frame or var frame,for unbalance.
		int on_req(frame *in,frame *out);//set fc=0 or fc=1,response fc3,fix frame or var frame,for balance and unbalance.
		int on_ack(frame *in,frame *out);//response fc0,used by balance;

		void set_loc_ctl();	//set respose frame's control word.
//the next functions  is implement of parent virtual function. inheritance 继承
		int build_link_layer(frame *out,int asdu_len);//by asdu build link frame.
		void deal_timeout();
		int get_frame();
		int link_time();//for balance
		int deal_frame(frame *in);
};
/****************************
 * link_layer_104
****************************/
#define TYPE_I 1
#define TYPE_S 2
#define TYPE_U 3
#define T3_TIME 20 
#define T2_TIME 5//10 
#define T1_TIME 15
#define T0_TIME 30
class link_layer_104:public link_layer{
	public:
		//fix frame is only for 101,so i define it in this.
		frame r_s_frame;
		frame r_u_frame;
		frame r_i_frame;
		frame s_s_frame;
		frame s_u_frame;
		frame s_i_frame;
		frame r_tmp_frame;
		//CircleQueue<var_frame> s_i_frames;
		CircleQueue<frame> s_i_frames;
		int start_rcv_s_flag;
		int start_rcv_u_flag;
		int start_rcv_i_flag;
		int r_s_pos;
		int r_u_pos;
		int r_i_pos;
		int r_tmp_pos;
		int frame_type;
		int N;
		//record current receive frame no. mod by n.
		//when receive a valid frame r_no++;
		//if peer send no not equal to r_no,the error will
		//raise
		int r_no;
		int s_no;//record current send frame no.mod by n.when send a valid frame s_no++.
		int ack_no;
		int send_num;//连续发送send_num个数据包后未被确认停止发送数据。
		int rcv_num;//接受rcv_num个数据包后，发送确认帧。
		int send_count;
		int rcv_count;
		int offset_control;
		timer t1_timer;//发送数据计时器，发送数据后等待确认。超时重新建立连接
		timer t2_timer;//接收数据计时器，接收I帧数据后，超时则发送S帧。
		timer t3_timer;//接收数据计时器，超时发送测试帧
		timer t0_timer;//连接计时器，超时重新建立连接
		int t1_count;
	public:
		link_layer_104(){
			balance=BALANCE;
			s_i_frames.init(100);
			N=32767;
			send_num=12;
			rcv_num=8;
			r_s_pos=0;
			r_u_pos=0;
			r_i_pos=0;
			r_tmp_pos=0;
			r_no=0;
			s_no=0;
			ack_no=0;
			r_s_frame.type=TYPE_S;
			r_u_frame.type=TYPE_U;
			r_i_frame.type=TYPE_I;
			s_s_frame.type=TYPE_S;
			s_u_frame.type=TYPE_U;	
			s_i_frame.type=TYPE_I;	
			start_rcv_u_flag=0;
			start_rcv_s_flag=0;
			start_rcv_i_flag=0;
			t1_timer.stop();
			t2_timer.stop();
			t3_timer.stop();
			t1_count=0;

			protocol=PRO_104;
			addr_size=ADDR_SIZE;
			cause_size=CAUSE_SIZE;
			msg_id_size=MSG_ID_SIZE_104;//101 is 2,104 is 3,so generation class 104 need redefine it.----- 
			offset_len=1;
			offset_control=2;
			offset_asdu=6;
			offset_ti=offset_asdu;
			offset_vsq=offset_asdu+1;
			offset_cause=offset_vsq+1;
			offset_pub_addr=offset_cause+cause_size;
			offset_msg_id=offset_pub_addr+addr_size;
			offset_data=offset_msg_id+msg_id_size;
		}
	public:
		int check_type(unsigned char);
		void resend();
	public:
		//build fix frame
		int build_sframe(frame *out,sfmt &);//
		int build_uframe(frame *out,ufmt &);//
		int build_test_link();
		int deal_process();
		int clear_sq();
		//the next functions  is implement of parent virtual function. inheritance 继承
		void set_loc_ctl();	//set respose frame's control word.
		int get_frame();
		int deal_frame(frame *in);
		int build_link_layer(frame *out,int asdu_len);//by asdu build link frame.
		void deal_timeout();
		int link_time();//for balance
};
#define REP_TIMES 3
#define REP_TIME  1//for s2 service
#define REP_TIME_S3 10 //for s3 service

void set_app_interface(app_layer *app);
#endif //__protocol_h
// vim:tw=72 
