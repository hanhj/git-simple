/*
 * File Name: data_type.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-25 13:04:49 week:2
*/
#include "data_type.h"
#include "common.h"

mystring::mystring(){
	data=NULL;
}
mystring::mystring(const char *da){
	int size;
	size=strlen(da);
	data=(char *)malloc(size);
	if(data!=NULL){
		strcpy(data,da);
	}
}
mystring::~mystring(){
	if(data!=NULL)
		free(data);
}
mystring & mystring::operator = (const char *da){
	if(data!=NULL)
		free(data);
	data=(char*)malloc(strlen(da));
	if(data!=NULL){
		strcpy(data,da);
	}
	return *this;
}
int mystring::operator ==(const char *b){
	return strcmp(data,b)?0:1;
}
int mystring::operator !=(const char *b){
	return strcmp(data,b)?1:0;
}
ostream & operator<<(ostream &os,mystring &da){
	return os<<da.data;
}

// vim:tw=72 
