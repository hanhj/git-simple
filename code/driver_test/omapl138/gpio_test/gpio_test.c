#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <linux/ioctl.h>

#define DEV_GPIO "/dev/gpio"

#define SET_OUTPUT_LOW 10
#define SET_OUTPUT_HIGH 11
#define GET_VALUE 12
#define SET_INPUT 13

int main(int argc, void *argv[])
{
	int fd = -1;	
	unsigned int pin;
	int cmd;
	int ret;

	printf("GPIO test ...\n\n\n\n");
	
	fd = open(DEV_GPIO, O_RDWR);
	if (fd < 0) {
		printf("open GPIO device error! %d\n", fd);
		return -1;
	}

	printf("\n\rplease input pin:bank*100+offset\n");
	scanf("%d",&pin);
	printf("\n\r******************************************");
	printf("\n\rplease input cmd:");
	printf("\n\r[10]:SetLow Volatge  [11]:SetHigh Volatge");
	printf("\n\r[12]:Get Value       [13]:Set Input ");
	printf("\n\r[-1] quit\n");
	printf("\n\r******************************************");
	scanf("%d",&cmd);
/*
	ioctl(fd, SET_OUTPUT_LOW, (unsigned long)pin);
	while (1) {
		ioctl(fd, SET_OUTPUT_HIGH, (unsigned long)pin);
		sleep(1);
		ioctl(fd, SET_OUTPUT_LOW, (unsigned long)pin);
		sleep(1);
		ioctl(fd, GET_VALUE, (unsigned long)pin);
		sleep(1);
	}
*/
	while (1) {
		if (cmd==-1)
			break;
		ret=ioctl(fd, cmd, (unsigned long)pin);
		printf("\n\r got pin: %d \n", ret);
		printf("\n\r******************************************");
		printf("\n\rplease input cmd:");
		printf("\n\r[10]:SetLow Volatge  [11]:SetHigh Volatge");
		printf("\n\r[12]:Get Value       [13]:Set Input ");
		printf("\n\r[-1] quit\n");
		printf("\n\r******************************************");
		scanf("%d",&cmd);
	}
	close (fd);
	return 1;
}

