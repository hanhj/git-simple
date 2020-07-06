#include	<sys/ioctl.h>
#include	<sys/fcntl.h>
#include	<unistd.h>
#include	<sys/mman.h>
#include	<stdio.h> 
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>  
#include	<fcntl.h>  
#include	<termios.h>
#include	<errno.h> 
#include	<pthread.h> 
#include	<assert.h>
#include	<time.h>
#include	<linux/rtc.h>
#include	<signal.h>
#include 	<sys/wait.h>
#include 	<netinet/in.h>
#include 	<linux/sockios.h>
#include 	<net/if.h>
#include 	<net/route.h>
#include 	<linux/if_ether.h>
#include 	<net/if_arp.h>
#include 	<arpa/inet.h>
#include 	<arpa/inet.h>
#include 	<linux/ioctl.h>

#define FALSE 1
#define TRUE 0

/* esam */
#define ESAM_CMD_RESET	     		11
#define ESAM_CMD_INIT	     		22
#define ESAM_CMD_WRITE	     		33
#define ESAM_CMD_READ	     		44
struct esam_user_data{
	uint16_t len;
	uint8_t  *buf;
};

int main(void){
	int i, j, ret;
	int fd_esam;
	struct esam_user_data esam_data;
	uint8_t test[8] = {0x55, 0x80, 0x0E, 0x00, 0x02, 0x00, 0x00, 0x73};
	uint8_t rcv[20] = {0};

	/* open easm */
	printf("open esam ...\n");
	fd_esam = open("/dev/esam", O_RDWR);	
	if (fd_esam < 0) {
		printf("open esam error! %d\n", fd_esam);
	}
	printf("open esam OK !\n");

	do {
		printf("--------------------------------------\n");

		esam_data.len = 8;
		esam_data.buf = test;
		printf("write data: %x %x %x %x %x %x %x %x \n", test[0], test[1], test[2], test[3], test[4], test[5], test[6], test[7]);
		if (ioctl(fd_esam, ESAM_CMD_WRITE, &esam_data) < 0) {
			printf("write esam error !\n");
		}
		else
		printf("write esam OK !\n");

		sleep(1);

		memset(rcv, 0, 20);
		esam_data.len = 19;
		esam_data.buf = &rcv[0];
		if (ioctl(fd_esam, ESAM_CMD_READ, &esam_data) < 0) {
			printf("read esam error !\n");
		}else{
		printf("read data: len %d \n", esam_data.len);
		for (i=0;i<esam_data.len;i++) {
			printf("%x ", rcv[i]);
		}
		printf("\n");
		}

	} while (1);

	close(fd_esam);	
}
/******************* End Of File: lcd_test.c *******************/
