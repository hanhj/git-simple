#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <linux/ioctl.h>

#define DEV_MISC "/dev/omapl138_misc"

#define GET_TIMER2_COUNTER 10
struct timer_counter{
u_int32_t counter12;
u_int32_t counter34;
};



int main(int argc, void *argv[])
{
	int fd = -1;	
	struct timer_counter data;
	int cmd;
	int ret;

	printf("MISC test ...\n\n\n\n");
	
	fd = open(DEV_MISC, O_RDWR);
	if (fd < 0) {
		printf("open MISC device error! %d\n", fd);
		return -1;
	}

	while (1) {
		printf("\n\r******************************************");
		printf("\n\rplease input cmd:");
		printf("\n\r[10]:SetLow Volatge  [11]:SetHigh Volatge");
		printf("\n\r[12]:Get Value       [13]:Set Input ");
		printf("\n\r[-1] quit\n");
		printf("\n\r******************************************");
		scanf("%d",&cmd);
		if (cmd==-1)
			break;
		ioctl(fd, cmd, (unsigned long)&data);
		printf("%d %d",data.counter12,data.counter34);
	}
	close (fd);
	return 1;
}

