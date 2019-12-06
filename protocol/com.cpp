/*
 * File Name: com.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 12:04:46 week:5
*/
#include<iostream>
using namespace std;
#include "com.h"
#include "protocol.h"
#include "system.h"
#include "utilty.h"
/****************************
 * physical layer
****************************/
int basic_com::get_byte(unsigned char *c){
	if(read_produce!=read_consume){
		*c=read_buff_ptr[read_consume];
		read_consume++;
		read_consume=(read_consume) % MAX_COM_BUFFER;
		pfunc(DEBUG_DEBUG,"get byte:%02hhx\n",*c);
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
	strcpy(file_name,"test.dat");
	return 0;
}
int serial::connect(){
	pfunc(DEBUG_DEBUG,"connect serial");
	f=fopen(file_name,"rt");
	return 0;

}
int serial::close(){
	pfunc(DEBUG_DEBUG,"close serial");
	fclose(f);
	return 0;
}
int serial::read(int len){
	int l;
	int i;
	int m;
	int num;
	char *ret;
	char buff[1000];
	unsigned char tmpbuf[500]; 
	m=0;
	if(f==NULL)
		return 0;
	if(feof(f))
		return 0;
	ret=fgets(buff,len,f);
	if(ret==NULL)
		return -1;
	if(buff[0]!='-' && buff[1]!='>')
		return -1;
	l=strlen(buff);
	i=5;
	for(;i<l;i++){
		if(buff[i]==' ')
			continue;
		if(isxdigit(buff[i])){
			sscanf(&buff[i],"%x",&num);
			tmpbuf[m]=num;
			i++;
			m++;
			*(read_buff_ptr+read_produce)=num;
			read_produce++;
			read_produce=read_produce % MAX_COM_BUFFER;
		}
	}
	pdump(DEBUG_DEBUG,"read serial",&tmpbuf[0],m);
	return m;
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
	pfunc(DEBUG_DEBUG,"get com state of serial\n");
	return LINK_CONNECT;
}
int serial::set_set(void *){
	pfunc(DEBUG_DEBUG,"set set of serial");
	return 0;
}
int serial::get_set(void *){
	pfunc(DEBUG_DEBUG,"get set of serial");
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
	strcpy(file_name,"test2.dat");
	return 0;
}
int ethernet::connect(){
	pfunc(DEBUG_DEBUG,"connect ethernet");
	f=fopen(file_name,"rt");
	return 0;

}
int ethernet::close(){
	pfunc(DEBUG_DEBUG,"close ethernet");
	fclose(f);
	return 0;
}
int ethernet::read(int len){
	int l;
	int i;
	int m;
	int num;
	char *ret;
	char buff[1000];
	unsigned char tmpbuf[500]; 
	m=0;
	if(f==NULL)
		return 0;
	if(feof(f))
		return 0;
	ret=fgets(buff,len,f);
	if(ret==NULL)
		return -1;
	if(buff[0]!='-' && buff[1]!='>')
		return -1;
	l=strlen(buff);
	i=5;
	for(;i<l;i++){
		if(buff[i]==' ')
			continue;
		if(isxdigit(buff[i])){
			sscanf(&buff[i],"%x",&num);
			tmpbuf[m]=num;
			i++;
			m++;
			*(read_buff_ptr+read_produce)=num;
			read_produce++;
			read_produce=read_produce % MAX_COM_BUFFER;
		}
	}
	pdump(DEBUG_DEBUG,"read ethernet:",&tmpbuf[0],m);
	return m;
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
	int ret;
	ret=0;
	ret=pdump(DEBUG_INFO,"send ethernet",data,len);
	if(ret<0){
		errno=ERR_SEND;
		pfunc(DEBUG_ERROR,"ethernet fail send\n");
		goto err;
	}
err:
	return ret;
}
int ethernet::get_com_state(){
	pfunc(DEBUG_DEBUG,"get com state of ethernet\n");
	return LINK_CONNECT;
}
int ethernet::set_set(void *){
	pfunc(DEBUG_DEBUG,"set set of ethernet");
	return 0;
}
int ethernet::get_set(void *){
	pfunc(DEBUG_DEBUG,"get set of ethernet");
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
	strcpy(file_name,"test3.dat");
	return 0;
}
int wireless::connect(){
	pfunc(DEBUG_DEBUG,"connect wireless\n");
	f=fopen(file_name,"rt");
	return 0;

}
int wireless::close(){
	pfunc(DEBUG_DEBUG,"close wireless\n");
	fclose(f);
	return 0;
}
int wireless::read(int len){
	int l;
	int i;
	int m;
	int num;
	char *ret;
	char buff[1000];
	unsigned char tmpbuf[500]; 
	m=0;
	if(f==NULL)
		return 0;
	if(feof(f))
		return 0;
	ret=fgets(buff,len,f);
	if(ret==NULL)
		return -1;
	if(buff[0]!='-' && buff[1]!='>')
		return -1;
	l=strlen(buff);
	i=5;
	for(;i<l;i++){
		if(buff[i]==' ')
			continue;
		if(isxdigit(buff[i])){
			sscanf(&buff[i],"%x",&num);
			tmpbuf[m]=num;
			i++;
			m++;
			*(read_buff_ptr+read_produce)=num;
			read_produce++;
			read_produce=read_produce % MAX_COM_BUFFER;
		}
	}
	pdump(DEBUG_DEBUG,"read wireless",&tmpbuf[0],m);
	return m;
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
	int ret;
	ret=0;
	ret=pdump(DEBUG_INFO,"send wireless",data,len);
	if(ret<0){
		errno=ERR_SEND;
		pfunc(DEBUG_ERROR,"wireless fail send\n");
		goto err;
	}
err:
	return ret;
}
int wireless::get_com_state(){
	pfunc(DEBUG_DEBUG,"get com state of wireless\n");
	return LINK_CONNECT;
}
int wireless::set_set(void *){
	pfunc(DEBUG_DEBUG,"set set of wireless\n");
	return 0;
}
int wireless::get_set(void *){
	pfunc(DEBUG_DEBUG,"get set of wireless\n");
	return 0;
}

// vim:tw=72 
