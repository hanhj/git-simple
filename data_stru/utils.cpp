/*
 * File Name: utils.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-29 15:44:34 week:6
*/
#include<iostream>
using namespace std;
#include "utils.h"
unsigned char debug_buff[DEBUG_BUFF];
int debug(int mode,const char*file,const char*func,int line,const char *fmt...){
	const char *color="";
	const char *default_col="\033[0;m";
	if(mode>DEBUG_LEVEL)
		return -1;
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
	fprintf(stderr,"[%s %s:%d]%s%s%s",file,func,line,color,debug_buff,default_col);
	return 0;
}
int dump(int mode,const char*file,const char*func,int line,unsigned char *data,int len,const char *head=NULL){
	int i;
	i=0;
	const char *color="";
	const char *default_col="\033[0;m";
	if(mode>DEBUG_LEVEL)
		return -1;
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
	return 0;
}

// vim:tw=72 
