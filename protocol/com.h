/*
 * File Name: com.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 12:02:54 week:5
*/
#ifndef __com_h
#define __com_h
#include "all_h.h"

#define MAX_COM_BUFFER 2000
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
typedef struct _serial_set{
	int baund;
	int start_bits;
	int stop_bits;
	int data_bits;
	int even;
}serial_set;
class serial:public basic_com{
	private:
		serial_set set;
		FILE *f;
		char file_name[20];
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
typedef struct _ethernet_set{
	int dst_ip[4];
	int local_ip[4];
	int remote_port;
	int local_port;
	int server;
}ethernet_set;
class ethernet:public basic_com{
	private:
		ethernet_set set;
		FILE *f;
		char file_name[20];
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

typedef struct _wireless_set{
	ethernet_set ip_set;
	int module_type;
}wireless_set;
class wireless:public basic_com{
	private:
		wireless_set set;
		FILE *f;
		char file_name[20];
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
#endif //__com_h
// vim:tw=72 
