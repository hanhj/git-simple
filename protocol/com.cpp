/*
 * File Name: com.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 12:04:46 week:5
*/
#include<iostream>
using namespace std;
#include "all_h.h"
#include "system.h"
#include "utilty.h"
#include "com.h"
extern char g_filename[];
/****************************
 * physical layer
****************************/
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
ostream & operator<<(ostream &os,serial_set &a){
	return os<<"baund:"<<a.baund<<",even:"<<a.even<<",start_bits:"<<a.start_bits<<",stop_bits:"<<a.stop_bits<<",data_bits:"<<a.data_bits<<endl;
}
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
/**
***********************************************************************
*  @brief	send data by serial
*  @param[in] data data buffer 
*  @param[in] len data len 
*  @return upon successful return number of actiual send.\n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
int serial::send(unsigned char *data,int len){
	int ret;
	ret=0;
	ret=pdump(DEBUG_INFO,"send serial",data,len);
	if(ret<0){
		errno=ERR_SEND;
		pfunc(DEBUG_ERROR,"serial fail send\n");
		goto err;
	}
err:
	return ret;
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

ostream & operator<<(ostream &os,ethernet_set &a){
	return os<<"dst_ip:"<<a.dst_ip[0]<<"."<<a.dst_ip[1]<<"."<<a.dst_ip[2]<<"."<<a.dst_ip[3]<<endl\
		<<",local_ip:"<<a.local_ip[0]<<"."<<a.local_ip[1]<<"."<<a.local_ip[2]<<"."<<a.local_ip[3]<<endl\
		<<",remote_port:"<<a.remote_port<<endl\
		<<",local_port:"<<a.local_port<<endl\
		<<",server:"<<a.server<<endl;
}
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
/**
***********************************************************************
*  @brief	send data by ethernet	
*  @param[in]  
*  @param[out]  
*  @return upon successful \n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
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

ostream & operator<<(ostream &os,wireless_set &a){
	return os<<"dst_ip:"<<a.ip_set.dst_ip[0]<<"."<<a.ip_set.dst_ip[1]<<"."<<a.ip_set.dst_ip[2]<<"."<<a.ip_set.dst_ip[3]<<endl\
		<<",local_ip:"<<a.ip_set.local_ip[0]<<"."<<a.ip_set.local_ip[1]<<"."<<a.ip_set.local_ip[2]<<"."<<a.ip_set.local_ip[3]<<endl\
		<<",remote_port:"<<a.ip_set.remote_port<<endl\
		<<",local_port:"<<a.ip_set.local_port<<endl\
		<<",server:"<<a.ip_set.server<<endl\
		<<",module_type:"<<a.module_type<<endl;
}
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
/**
***********************************************************************
*  @brief send data by wireless	
*  @param[in] data data buffer 
*  @param[out]  len data len 
*  @return upon successful \n
*	if fail a negative value returned.
*  @note	
*  @see		
***********************************************************************
*/
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

// vim:tw=72 
