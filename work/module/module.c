/*
 * File Name: main.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-04-11 17:27:44 week:6
*/

/*
 *	1. open serial
 *	1.1. send a command,start time out
 *	2. read serial n number ,reurn m number
 *	3. compare get m number str with send str and key str
 *	4. if compare successful do action and move compare point
 *	5. if not compare successful continue read serial until time out
 *
 *	com[i]=open(port[1]);
 *	while(1){
 *		wait command;
 *		key_str=command;
 *		com[i].send(key_str);
 *		com[i].timeout[1].start(1000);
 *		while(1){
 *			if(com[i].timeout[1].is_reached)
 *				break;
 *			m=com[i].read(n);
 *			ret=compare(key_str[index],com[i].data_buff.data,m);
 *			if(ret){
 *				do_action(key_str[index].action);
 *				adjust_point(com[i].data_buff.point,m);
 *			}
 *		}
 *	}
 *	
 * */
#include<stdio.h>

int main(int argc,char **argv){
	printf("hello world\r\n");

	return 0;
}

// vim:tw=72 
