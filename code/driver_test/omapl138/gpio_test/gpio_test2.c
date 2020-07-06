/*
 * File Name: test.c
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2020-04-26 23:29:45 week:0
*/
#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#define SET_OUTPUT_LOW 10
#define SET_OUTPUT_HIGH 11
#define GET_VALUE 12
#define SET_INPUT 13
int main(int argc,char **argv){
	int fd;
	fd=open("/dev/gpio",O_RDWR);
	if(!fd){
		perror("open gpio faile\n");
	}
	int pin;
	int count=100;
	pin=715;
	if(argc>1){
		pin=atoi(argv[1]);
	}
	printf("control gpio:%d\n",pin);
	ioctl(fd,SET_OUTPUT_LOW,pin);
	while(count){
		count--;
		sleep(1);
		ioctl(fd,SET_OUTPUT_HIGH,pin);
		sleep(1);
		ioctl(fd,SET_OUTPUT_LOW,pin);
	}
	close(fd);
}
// vim:tw=72 
