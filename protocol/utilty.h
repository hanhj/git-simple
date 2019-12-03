/*
 * File Name: utilty.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 11:55:10 week:5
*/
#ifndef __utilty_h
#define __utilty_h
#include "all_h.h"
#define DEBUG_LEVEL		2
#define DEBUG_ERROR		1
#define DEBUG_WARNING	2
#define DEBUG_INFO		3
#define DEBUG_NORMAL	0
#define DEBUG_BUFF 1000
#define pfunc(mode,msg...) debug(mode,__FILE__,__func__,__LINE__,msg)
#define pdump(mode,head,data,len) dump(mode,__FILE__,__func__,__LINE__,data,len,head)
void sleep_ms(int t);
unsigned char sum(unsigned char *data,int len);
time_t get_system_time();
int debug(int mode,const char*file,const char*func,int line,const char	*fmt...);
int dump(int mode,const char*file,const char*func,int line,unsigned char*data,int len,const char *head=NULL);
class timer{
		int duration;
	public:
		timer(){
			start_flag=0;
		}
		int exp_time;
		int start_flag;
		int start(int T);
		int restart(int T);
		int restart();
		int stop();
		int is_reached();
		int is_start();
};
extern void init_kb();
extern void close_kb();
extern int kbhit();
extern int readch();
#endif //__utilty_h
// vim:tw=72 
