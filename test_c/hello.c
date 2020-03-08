/*
 * File Name: hello.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-03-06 22:30:45 week:5
*/
#include<stdio.h>
#include<unistd.h>
int main(){
	int i=1;
	daemon(1,1);
	while(1){
		printf("%d hello world\r\n",i++);
		sleep(1);
	}
	return 0;
}

// vim:tw=72 
