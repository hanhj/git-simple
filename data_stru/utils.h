/*
 * File Name: utils.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-29 15:44:50 week:6
*/
#ifndef __utils_h
#define __utils_h
#include<iostream>
using namespace std;
#include<stdarg.h>
#include <iomanip>
//for debug message 
#define DEBUG_LEVEL		3
#define DEBUG_ERROR		1
#define DEBUG_WARNING	2
#define DEBUG_INFO		3
#define DEBUG_NORMAL	4
#define DEBUG_BUFF 1000

int debug(int mode,const char*file,const char*func,int line,const char*fmt...);
int dump(int mode,const char*file,const char*func,int line,unsigned	char*data,int len,const char *head);
#define pfunc(mode,msg...) debug(mode,__FILE__,__func__,__LINE__,msg)
#define pdump(mode,head,data,len) dump(mode,__FILE__,__func__,__LINE__,data,len,head)

template <class T>
void dump(T*data,int size,const char *head=NULL){
	int i;
	i=0;
	if(head)
		cout<<head<<endl;
	while(size--){
		cout<<setw(2)<<data[i]<<",";
		i++;
	}
	cout<<endl;
}
#endif //__utils_h
// vim:tw=72 
