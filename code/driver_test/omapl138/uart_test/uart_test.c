
#include     <stdio.h> 
#include     <stdlib.h>
#include     <string.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>  
#include     <fcntl.h>  
#include     <termios.h>
#include     <errno.h> 
#include     <pthread.h> 
#include     <sys/ioctl.h> 

#include "../inc/at91_ioctl.h"
#include "../inc/at91_gpio.h"

#define FALSE 1
#define TRUE 0


#define RST 	AT91_PIN_PB25
#define IGT	AT91_PIN_PB27
#define MAX_UART 30

 int fd=-1;
int device;
int deviceid=1;
int functionid=1;
int functionidtmp=1;
int boadrate=115200;
char *recchr="We received: ";
char dev[20];
char buff[512];
//int speed_arr[] = { B921600, B460800, B230400, B115200, B57600, B38400, B19200, B9600, B4800,
		  //  B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int speed_arr[] = {  B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200};
int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400, 1200};

void reset_gprs( void )
{
#define DEV_GPIO "/dev/gpio"
	at91_gpio_arg arg_x1, arg_x2, arg_y1, arg_y2;
	int fd = -1;	
	int i;

	printf("AT91 GPIO test ...\n\n\n\n");
	
	fd = open(DEV_GPIO, O_RDWR);
	if (fd < 0) {
		printf("open GPIO device error! %d\n", fd);
		return ;
	}
	arg_x1.pin = RST;		    //reset pin
	arg_x1.data = 1;	
	ioctl(fd, IOCTL_GPIO_SETOUTPUT, &arg_x1);
	arg_x1.data = 1;	
	ioctl(fd, IOCTL_GPIO_SETVALUE, &arg_x1);
	sleep(2);
	arg_x1.data = 0;	
	ioctl(fd, IOCTL_GPIO_SETVALUE, &arg_x1);

	arg_x2.pin = IGT;		    //igt pin
	arg_x2.data = 1;	
	ioctl(fd, IOCTL_GPIO_SETOUTPUT, &arg_x2);
	arg_x2.data = 0;	
	ioctl(fd, IOCTL_GPIO_SETVALUE, &arg_x2);
	usleep(1000);					//1 ms
	arg_x2.data = 1;	
	ioctl(fd, IOCTL_GPIO_SETVALUE, &arg_x2);
	usleep(2000*1000);				//2s
	arg_x2.data = 0;	
	ioctl(fd, IOCTL_GPIO_SETVALUE, &arg_x2);

	printf("set ISI_D7 high level\n");
	close(fd);
}

void set_speed(int fd, int speed)
{
	int   i;
	int   status;
	struct termios   Opt;
	tcgetattr(fd, &Opt);
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
	{
		if (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if (status != 0)
				perror("tcsetattr fd1");
			return;
		}
		tcflush(fd,TCIOFLUSH);
	}
}
int set_Parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
	if  ( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n");
			return (FALSE);
	}
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   
			options.c_iflag &= ~INPCK;   
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); 
			options.c_iflag |= INPCK;           
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;     
			break;
		case 'S':
		case 's':  
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
	}
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported stop bits\n");
			return (FALSE);
	}

	options.c_cflag &= ~CRTSCTS;

	/* Set input parity option */

	if (parity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	//add by qianwei
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	options.c_lflag &= ~(ICANON |ISIG | ECHO | ECHOE);
	options.c_iflag &= ~(IXON | IXOFF|IXANY);
	options.c_oflag &= ~ONLCR;
	options.c_iflag &= ~ICRNL;

	tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);
}

int OpenDev(char *Dev)
{
	int	fd = open( Dev, O_RDWR );
	if (-1 == fd)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	else
		return fd;
}


void receivethread(void)
{
	int nread;

	while(1)    
	{

		if((nread = read(fd,buff,100))>0)
		{
			printf("[RECEIVE] Len is %d,content is :\n",nread);
			printf("Acsii Type:%s",buff);
			printf("Hex Type:\n",buff);
	        if (nread>0)
			{
				int i;
				for ( i = 0; i < nread; i++) {
					printf("%02x ", buff[i]);
				}
				printf("\n");
			}
			if(functionid==1)
			{
				write(fd, recchr, strlen(recchr)); 
				write(fd, buff, nread);
				write(fd, "\n", 2);	
			}

		}

		usleep(100*1000);
	} 

	return;
}

unsigned char c2i(unsigned char c)
{
	if(c>='a'&&c<='z') return c-'a'+0xa;
	if(c>='A'&&c<='Z') return c-'A'+0xa;
	if(c>='0'&&c<='9') return c-'0';
	return c;
}

int main(int argc, char *argv[])
{
	char str[500];
	pthread_t receiveid;
	int  c, ctrlbits;
	int i = 0;
	int ret = 0;
    
AGAIN:

    printf("Select mode:(1->ttyS , 2->ttySC , 3->ttySU , 4->ttyAT  5->ttyUSB) \n:");
	if(scanf("%d", &functionidtmp)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}
	printf("Select port (0~%d for DEBUG,COM1~COM%d respectively) \n:", (MAX_UART-1), (MAX_UART-1));
	if(scanf("%d", &device)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}
//reset_gprs();
	if (device >= 0 && device < MAX_UART) {
		if (functionidtmp == 1) {
			printf("ttyS%d is selected \n", device);
			sprintf(dev, "/dev/ttyS%d", device);
        } else if (functionidtmp == 2) {
			printf("ttySC%d is selected \n", device);
			sprintf(dev, "/dev/ttySC%d", device);
		} else if (functionidtmp == 3) {
			printf("ttySS%d is selected \n", device);
			sprintf(dev, "/dev/ttySU%d", device);
		} else if (functionidtmp == 4) {
			printf("ttyAT%d is selected \n", device);
			sprintf(dev, "/dev/ttyAT%d", device);
		} else if(functionidtmp==5){
			printf("ttyUSB%d is selected \n", device);
			sprintf(dev, "/dev/ttyUSB%d", device);
	    }else{
			printf("Invalid choice, please try again! \n:");
			goto AGAIN;
		}
	}
	printf("Select function:(1.auto read and  reply  2.write manully and auto read) \n:");
FUNTION: 

	if(scanf("%d", &functionid)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}

	if(functionid!=1&&functionid!=2)
	{
		printf("Invalid choice, please try again! \n:");	
		goto    FUNTION;	
	}

    	printf("Select speed:(1->1200, 2->2400, 3->4800, 4->9600, 5->19200, 6->57600, 7->115200) \n:");
	if(scanf("%d", &functionidtmp)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}
	sleep(1);    
	fd = OpenDev(dev);
	if (fd>0){
		if (functionidtmp == 1) {
			set_speed(fd,1200);
        } else if (functionidtmp == 2) {
			set_speed(fd,2400);
		} else if (functionidtmp == 3) {
			set_speed(fd,4800);
		} else if (functionidtmp == 4) {
			set_speed(fd,9600);
		} else if (functionidtmp == 5) {
			set_speed(fd,19200);
		} else if (functionidtmp == 6) {
			set_speed(fd,57600);
		} else if (functionidtmp == 7) {
			set_speed(fd,115200);
		} else {
			set_speed(fd,2400);
			printf("Selected 2400\n:");
		}
	}
	else{
		printf("Can't Open Serial Port!\n");
		exit(0);
	}
	printf("Select OEN:(1->N, 2->O, 3->E) \n:");
	if(scanf("%d", &functionidtmp)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}
	if (functionidtmp == 1) {
		ret = set_Parity(fd,8,1,'N');
	} else if (functionidtmp == 2) {
        ret = set_Parity(fd,8,1,'O');
	} else if (functionidtmp == 3) {
		ret = set_Parity(fd,8,1,'E');
	} else {
		ret = set_Parity(fd,8,1,'N');
		printf("Selected N\n:");
	}
	
	if (ret == FALSE)
	{
		printf("Set Parity Error\n");
		exit(1);
	}

	ioctl(fd, TIOCMGET, &ctrlbits);
	ctrlbits &= ~TIOCM_RTS;
	ioctl(fd, TIOCMSET, &ctrlbits);

	pthread_create(&receiveid,NULL,(void*)receivethread,NULL);
	sleep(2);
	while(1)
	{
		if ( functionid == 2 )
		{
			printf("Please Input string to send to %s\n:",dev);
			scanf("%s", str);
			printf("str = %s\r\n", str);
			//str[0] = 16*c2i(str[0]) + c2i(str[1]);
			//str[1] = 16*c2i(str[2]) + c2i(str[3]);
			//printf("\n");
			//for (i=0;i<10;i++) {
				//printf("str[%d] = %x \n", i, str[i]);
				//printf("str[1] = %x \n", str[i]);
			//}

			if(strlen(str)>0) {
			    write(fd, str, strlen(str));
			}
			write(fd, "\r\n", strlen("\r\n"));
		}   
		usleep(100*1000);	 
	}
	close(fd);
	exit(0);
}
