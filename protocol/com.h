/*
 * File Name: com.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 12:02:54 week:5
*/
#ifndef __com_h
#define __com_h
#include "all_h.h"

#define COM_RECV_BUFFER_SIZE 2000 
#define COM_SEND_BUFFER_SIZE 512
#define TYPE_SERIAL 1
#define TYPE_ETHERNET 2
#define TYPE_WIRELESS 3

class com_port{
	protected:
		int read_produce;
		int read_consume;
		int send_produce;
		int send_consume;
	public:
		int port_no;//port number
		int com_type;//1:serial,2:enthtnet,3:wireless net(gprs or cdma)
		unsigned char read_buff[COM_RECV_BUFFER_SIZE];
		int state;
	public:
		com_port(){
			read_produce=0;
			read_consume=0;
			send_produce=0;
			send_consume=0;
			state=0;
			port_no=0;
			com_type=0;
			memset(&read_buff,0,sizeof(read_buff));
		};
		com_port(int port){
			read_produce=0;
			read_consume=0;
			send_produce=0;
			send_consume=0;
			state=0;
			port_no=port;
			com_type=0;
			memset(&read_buff,0,sizeof(read_buff));
		}
		virtual int init()=0;
		virtual int init(void *para)=0;
		virtual int connect()=0;
		virtual int close()=0;
		virtual int read(int len)=0;
		virtual int get_byte(unsigned char*);
		virtual int send(unsigned char *data,int len)=0;
		virtual int get_com_state()=0;
				void set_port(int p){
					port_no=p;
				}
};
typedef struct _serial_set{
	int baund;
	int start_bits;
	int stop_bits;
	int data_bits;
	int even;
}serial_set;
class serial:public com_port{
	private:
		serial_set set;
		FILE *f;
		char file_name[20];
	public:	
		serial(){
			com_type=TYPE_SERIAL;
			memset(&set,0,sizeof(set));
		}
		serial(int port):com_port(port){

		}
		virtual int init();
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
};
typedef struct _ethernet_set{
	int dst_ip[4];
	int local_ip[4];
	int remote_port;
	int local_port;
	int server;
}ethernet_set;
class ethernet:public com_port{
	private:
		ethernet_set set;
		FILE *f;
		char file_name[20];
	public:
		ethernet(){
			com_type=TYPE_ETHERNET;
			memset(&set,0,sizeof(set));
		}
		ethernet(int port):com_port(port){
		}
		virtual int init();
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
};

typedef struct _wireless_set{
	ethernet_set ip_set;
	int module_type;
}wireless_set;
class wireless:public com_port{
	private:
		wireless_set set;
		FILE *f;
		char file_name[20];
		unsigned char send_buff[COM_SEND_BUFFER_SIZE];
	public:
		wireless(){
			com_type=TYPE_WIRELESS;
			memset(&set,0,sizeof(set));
			memset(&send_buff,0,sizeof(send_buff));
		}
		wireless(int port):com_port(port){
		}
		virtual int init();
		virtual int init(void *para);
		virtual int connect();
		virtual int close();
		virtual int read(int len);
		virtual int send(unsigned char *data,int len);
		virtual int get_com_state();
};
#endif //__com_h
// vim:tw=72 
