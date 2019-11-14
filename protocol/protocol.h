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
#include "utilty.h"
#include "configurable.h"
#include "system.h"
#define MAX_SEND_YX_PER_FRAME 28 
#define MAX_SEND_YC_PER_FRAME 16 
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
	unsigned char qu:6;
	unsigned char sel:1;
}sco_bit;
typedef struct _dco_bit{//双命令
	unsigned char dcs:2;//1开，2合
	unsigned char qu:6;
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
#define LINK_NOCONNECT		0
#define LINK_CONNECT		1
//规约系统参数
#define ADDR_SIZE			2
#define CAUSE_SIZE			2
#define MSG_ID_SIZE			2
#define BALANCE				1
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
#define PROCESS_RD_DZ_UNIT	0x2000
#define PROCESS_WR_DZ_UNIT	0x4000
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
#define COMMAND_FILE		210
#define COMMAND_RD_DZ		202
#define COMMAND_RD_DZ_UNIT	201
#define COMMAND_WR_DZ_UNIT	200
#define COMMAND_WR_DZ		203
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
#include <list>
typedef struct _dir{//directory
	char file_name[30];
	int file_id;
	int file_size;
}dir;
typedef list<dir> dir_list;
typedef _file_segment{
	int pos;
	char content[300];
}file_segment;
/****************************
 * app_layer
****************************/
class link_layer;
//process data
typedef struct _summon_data{
	int sended_yx_num;
	int sended_yc_num;
	int summon_step;
}SummonData;
typedef struct _clock_data{
	int clock_syn;
	int clock_rd;
}ClockData;
typedef struct _yk_data{
	int sel;
	int act;
	int act_over;
	int deactive;
	int cmd_id;
	int ctrl_id;
	int start_timeout;
	int timeout;
	int fail;
	Sco sco;
	Dco dco;
}YkData;
typedef struct _event_data{
	int need_ack[4];
	int need_yc_ack[4];
}EventData;
typedef struct _file_data{
	int act;
	struct {
		long id;
		int len;
		char name[10];
		int flag;
		union{
			CP56Time2a time;
			unsigned char data[7];
		}start_time;
		union{
			CP56Time2a time;
			unsigned char data[7];
		}end_time;
	}rd_dir;
	struct {
		int len;
		char name[10];
		long id;
		long ack_offset;
		long cur_offset;
		char suc;//0无后续，1有后续
		int step;
	}rd_file;
}FileData;

//app_layer is y deal asdu part
class app_layer{
	public:
		vsq vsq_lo;
		send_cause cause_lo;
		int offset;//position of asdu in frame.
		int addr_size;
		int cause_size;
		int msg_id_size;
		int addr;
		int yc_data_type;
		int need_reset;
	public:
		app_layer(){
			vsq_lo.data=0;
			cause_lo.data=0;
			get_yx_data=NULL;
			get_yc_data=NULL;
			get_event_data=NULL;
			get_clock = NULL;
			do_yk=NULL;
			do_reset=NULL;
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
			yc_data_type=9;
			need_reset=0;
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
		int build_link_fini(frame *out,link_layer *link);
		int build_summon_con(frame *out,link_layer *link);
		int build_summon_term(frame *out,link_layer *link);
		SORT_YX_TAB * (*get_yx_data)(int);
		int build_yx_data(frame *out,link_layer *link);//cause 20
		int build_dyx_data(frame *out,link_layer *link);//cause 20
		YC_TAB * (*get_yc_data)(int);
		int build_yc_data(frame *out,link_layer *link);//cause 20

		int (*get_event_data)(int type,event *&e,int change);
		int build_event_data(frame *out,link_layer *link,event *e);//cause 3

		int build_clock(frame *out,link_layer *link);//cause	5,7
		int (*get_clock)(CP56Time2a &);
		
		int (*do_yk)(int id,int type,int cmd);
		int build_yk(frame *out,link_layer *link);//cause=7,sel=0 or 1
		
		int build_link_test_con(frame *out,link_layer *link);//cause 7
		
		int (*get_yc_cg_data)(int port ,event_yc *&);
		int build_yc_cg_data(frame *out,link_layer *link,event_yc *e);//cause 3
		
		void (*do_reset)();
		int build_reset_con(frame *out,link_layer *link);//reset terminal cause 7

		
		int (*get_dir_list)(char *dir_name,dir_list *&list);
		int build_rd_dir_resp(frame *out,link_layer *link,dir_list *);//cause 5
		int build_rd_file_con(frame *out,link_layer *link);//cause 7
		int (*get_file_segment)(char *filename,int pos,file_segment *file);
		int build_rd_file_resp(frame *out,link_layer *link,file_segment *file);//cause 5
		int build_wr_file_con(frame *out,link_layer *link);//cause 7
		int build_wr_file_resp(frame *out,link_layer *link);//cause 5
		int (*save_file_segment)(char *filename,int pos,file_segment *file);
		
		int (*get_dz_unit)(buffer*data);
		int build_rd_dz_unit_con(frame *out,link_layer *link);//cause 7
		int (*set_dz_unit)(int);
		int build_wr_dz_unit_con(frame *out,link_layer *link);//cause 7

		int (*get_dz_data)(int id,buffer*data);
		int build_rd_dz_con(frame *out,link_layer *link);//cause 7
		int (*set_dz)(int id,int sel,buffer*data);
		int build_dz_con(frame *out,link_layer *link,int sel);//cause 7,sel =0 or 1 ,cr=0
		int build_dz_dact_con(frame *out,link_layer *link);//cause 9,sel=0,cr=1

		int build_summon_acc_con(frame *out,link_layer *link);//cause 7
		int build_summon_acc_term(frame *out,link_layer *link);//cause 10
		int build_summon_acc_resp(frame *out,link_layer *link);//cause 37
		int (*get_summon_acc_data)(buffer*data);

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

		unsigned long process;//which process is in.
		SummonData summon_data;
		ClockData clock_data;
		YkData yk_data;
		EventData event_data;
		FileData file_data;

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

			summon_data.sended_yx_num =0;
			summon_data.summon_step = 0;
			process=0;
		}
		int set_link_com(com_port*,int port);
		int set_app(app_layer*);
		virtual void deal_timeout()=0;
		virtual int get_frame()=0;//get a valid frame from port.
		virtual int link_time()=0;
		virtual int deal_frame(frame *)=0;
		virtual void set_loc_ctl()=0;
		void reset_yk_data();
		int check_state();//cycle check link state
		int send_frame(frame *);
	public:
		//the next functions will build link layer frame by call same name function in app_layer.
		//different link layer has different link frame(eg 101 and 104).but they have same app_layer frame(asdu).
		//the inheritance class must realize these virtual function.
		virtual int build_link_layer(frame *out,int)=0;//by asdu build link frame 

		virtual int on_summon(frame *in,frame *out)=0;
		virtual int process_summon(frame *out)=0;//set fc=3(balance) or fc=8(unbalance),var frame
		virtual int on_summon_acc(frame *in,frame *out)=0;
		virtual int process_summon_acc(frame *out)=0;
		virtual int on_clock(frame *in,frame *out)=0;
		virtual int process_clock(frame *out)=0;
		virtual int on_yk(frame *in,frame *out)=0;
		virtual int process_yk(frame *out)=0;
		virtual int process_event(frame *out)=0;
		virtual int on_test_link(frame *in,frame *out)=0;
		virtual int process_test_link(frame *out)=0;
		virtual int process_yc_change(frame *out)=0;
		virtual int on_reset_terminal(frame *in,frame *out)=0;
		virtual int process_reset_terminal(frame *out)=0;
		virtual int on_file(frame *in,frame *out)=0;
		virtual int process_file(frame *out)=0;
		virtual int on_rd_dz(frame *in,frame *out)=0;
		virtual int process_rd_dz(frame *out)=0;
		virtual int on_rd_unit(frame *in,frame *out)=0;
		virtual int process_rd_unit(frame *out)=0;
		virtual int on_wt_dz(frame *in,frame *out)=0;
		virtual int process_wt_dz(frame *out)=0;
		virtual int on_wt_unit(frame *in,frame *out)=0;
		virtual int process_wt_unit(frame *out)=0;
		virtual int on_update(frame *in,frame *out)=0;
		virtual int process_update(frame *out)=0;


		void on_notify(message *msg);
		void notify(message *msg);
};
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
			balance=0;
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
		void set_balance(int da){
			balance = da;
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
		void deal_timeout();
		int get_frame();
		int link_time();//for balance
		int deal_frame(frame *in);
		int save_frame(frame *);
		//build fix frame
		int build_ack(frame *out,int has_data=0);//set fc=0,fix frame,has_data indicator if have class 1 data,used for unbalance.
		int build_nak(frame *out);//set fc=1, fix frame
		int build_err_rep(frame *out,int err);//?
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
		int build_link_fini(frame *out);//set fc=3(balance) or fc=8(unbalance),var frame
		
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
		int on_wt_dz(frame *in,frame *out);
		int process_wt_dz(frame *out);
		int on_wt_unit(frame *in,frame *out);
		int process_wt_unit(frame *out);
		int on_update(frame *in,frame *out);
		int process_update(frame *out);
};
#define REP_TIMES 3
#define REP_TIME  1


void set_app_interface(app_layer *app);
#endif //__protocol_h
// vim:tw=72 
