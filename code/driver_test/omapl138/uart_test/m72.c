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

#define FALSE 1
#define TRUE 0

#define DEV_GPIO "/dev/gpio"


#define SET_OUTPUT_LOW 10
#define SET_OUTPUT_HIGH 11
#define GET_VALUE 12
#define SET_INPUT 13

#define WIRELESS_POWER 		711 	/*gprs power enable    */  
#define WIRELESS_ON_OFF 	714 	/*gprs on/off    */        
#define WIRELESS_RST 		600 	/*gprs reset    */         
#define WIRELESS_INDICATE 	712 	/*gprs indicate led    */  


int speed_arr[] = {  B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200};
int name_arr[] = {115200, 57600, 38400,  19200,  9600,  4800,  2400, 1200};

void set_pin(int fd,int pin,int value)
{
	if(value==0)
	ioctl(fd, SET_OUTPUT_LOW, (unsigned long)pin);
	else
	ioctl(fd, SET_OUTPUT_HIGH, (unsigned long)pin);	
}

void start_gprs( void )
{
	int fd = -1;	
	int i;

	printf("AT91 GPIO test ...\n\n\n\n");
	
	fd = open(DEV_GPIO, O_RDWR);
	if (fd < 0) {
		printf("open GPIO device error! %d\n", fd);
		return ;
	}
        set_pin(fd,WIRELESS_POWER, 1);
        set_pin(fd,WIRELESS_ON_OFF, 1);
        usleep(30*1000);//need 30ms
        set_pin(fd,WIRELESS_ON_OFF, 0);
        usleep(1000*1000);//need >1s
        set_pin(fd,WIRELESS_ON_OFF, 1);

	printf("start module\r\n");
	close(fd);
}


#define PPPD_PATH "/usr/sbin/pppd"
#define PPPD_CMD "pppd"
#define PPPD_ARG1 "call"
#define PPPD_ARG2 "wireless"


#define MUX_PATH "./gsmMuxd"
#define MUX_CMD "gsmMuxd"
//#define MUX_ARG1 "-p /dev/ttyS1 -w -r -s /dev/mux /dev/ptmx /dev/ptmx /dev/ptmx"
#define MUX_ARG1 "-p "
#define MUX_ARG2 "/dev/ttyS1 "
#define MUX_ARG3 "-w "
#define MUX_ARG4 "-r "
#define MUX_ARG5 "-s "
#define MUX_ARG6 "/dev/mux "
#define MUX_ARG7 "/dev/ptmx "
#define MUX_ARG8 "/dev/ptmx "
#define MUX_ARG9 "/dev/ptmx"

pid_t pppd_pid;
pid_t mux_pid;
void start_pppd(){
//pid_t pid;	
//	if((pid = fork()) == 0){
//		execl(PPPD_PATH, PPPD_CMD, PPPD_ARG1, PPPD_ARG2, NULL, NULL);
//		printf("error, pppd cant be executed\n");
//		exit(-1);
//	}else if(pid > 0){
//		pppd_pid = pid;
//	}
system("./pppd call wireless &");	
}
static void stop_pppd_prog(void)
{
	system("killall -q -9 pppd");
	system("killall -q -9 chat");
}
void stop_pppd(){

	//kill(pppd_pid, 9);
	//sleep(1);
	//waitpid(pppd_pid, NULL, WNOHANG);
	stop_pppd_prog();
	sleep(1);
	pppd_pid = 0;
}
void start_muxd(){
/*	
pid_t pid;	

	if((pid = fork()) == 0){
		execl(MUX_PATH, MUX_CMD, MUX_ARG1, MUX_ARG2,MUX_ARG3,MUX_ARG4,MUX_ARG5,MUX_ARG6,MUX_ARG7,MUX_ARG8,MUX_ARG9,NULL);
		printf("error, gsmMuxd cant be executed\n");
		exit(-1);
	}else if(pid > 0){
		mux_pid = pid;
	}
*/
	system("./gsmMuxd -p /dev/ttySU0 -w -r -s /dev/mux /dev/ptmx /dev/ptmx /dev/ptmx");	
}
static void stop_mux_prog(void)
{
	system("killall -q -9 gsmMuxd");
}
void stop_muxd(){
	//kill(mux_pid, 9);
	//sleep(1);
	//waitpid(mux_pid, NULL, WNOHANG);
	stop_mux_prog();
	sleep(1);
	mux_pid = 0;
}
void stop_gprs( void )
{
	int fd = -1;	
	int i;

	printf("AT91 GPIO test ...\n\n\n\n");
	
	fd = open(DEV_GPIO, O_RDWR);
	if (fd < 0) {
		printf("open GPIO device error! %d\n", fd);
		return ;
	}
        set_pin(fd,WIRELESS_ON_OFF, 0);
        usleep(800*1000);//need 0.6~1s
        set_pin(fd,WIRELESS_ON_OFF, 1);
        usleep(3000*1000);//need 2~12s
        set_pin(fd,WIRELESS_POWER, 0);

	printf("stop module\r\n");
	close(fd);
}
int functionidtmp;
int main(int argc, char *argv[])
{
char c;
	while(1){
  printf("Select funmode:\n");
  printf("0->quit \n");
  printf("1->start module			2->stop module \n");
  printf("3->start mux			4->stop mux\n");
  printf("5->start pppd			6->stop pppd \n");
	if(scanf("%d", &functionidtmp)!=EOF){
		while((c=getchar())!='\n' && c!=EOF){
			;
		}
	}
	if(functionidtmp==0)
		{
			break;
		}
	if(functionidtmp==1)
		{
			start_gprs();
		}
	if(functionidtmp==2)
		{
			stop_gprs();
		}
	if(functionidtmp==3)
		{
			start_muxd();
		}
	if(functionidtmp==4)
		{
			stop_muxd();
		}
	if(functionidtmp==5)
		{
			start_pppd();
		}
	if(functionidtmp==6)
		{
			stop_pppd();
		}
	}
}