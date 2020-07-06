
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<stdio.h>
#include<fcntl.h>
#include<assert.h>
#include<time.h>
#include<linux/rtc.h>
#include<sys/ioctl.h>

#define DEV_RTC "/dev/rtc0"

char *monthstr[12]={
                 "Jan","Feb","Mar","Apr","May","Jun",
                 "Jul","Aug","Sep","Oct","Nov","Dec"
                };
char *weekstr[7]={"Monday", "Tuesday", "Wednesday", "Tursday", "Friday", "Saturday","Sunday"};

int main(void) {

    struct rtc_time wtime, testtime;	 
    int fd;
    int year,month,date,week,hour,minute,second,tmp;
    char val, test;
    unsigned int default_para = 6;
    struct rtc_wkalrm atm;

    fd = open(DEV_RTC, O_RDWR);
    if (fd <0){
	printf("Open %s error\n", DEV_RTC);
	return -1;
    }

    printf("\n\rDo you want to Reset Time(Y/N/T)");

    scanf("%c",&val);
    if(val=='Y'||val=='y'){
	#if 0
	while (1) {
		printf("\n\rread or write reg:(R/W): 1->R, 2->W: ");
		scanf("%d",&year);
		if(year==1){
		    ioctl(fd, RTC_ALM_READ, &testtime); 
		} else if (year==2) {
		    printf("\n\rwrite value: ");
		    scanf("%d",&tmp);
		    testtime.tm_sec = tmp;
		    ioctl(fd, RTC_ALM_SET, &testtime); 
		} else {
			break;
		}
	}
	#else
	printf("\n\ryear(2000-2099):");
	scanf("%d",&year);

	printf("month(1-12):");
	scanf("%d",&month);

	printf("date(1-31):");
	scanf("%d",&date);

	printf("week(1-7):");
	scanf("%d",&week);

	printf("hour(0-23):");
	scanf("%d",&hour);

	printf("minute(0-59):");
	scanf("%d",&minute);

	printf("second(0-59):");
	scanf("%d",&second);

	printf("\nYour input string is =%d %d %d %d %d %d %d\n\r\n\r",year,month,date,week,hour,minute,second);
	//set time
	wtime.tm_year = year;
	wtime.tm_mon  = month -1;
	wtime.tm_mday = date;
	wtime.tm_wday = week;
	wtime.tm_hour = hour;
	wtime.tm_min  = minute;
	wtime.tm_sec  = second;
	ioctl(fd, RTC_SET_TIME, &wtime);
	#endif
    }

    while (1)	// Set sequential part here
    {
	memset(&testtime, 0, sizeof(testtime));
#if 1
	ioctl(fd, RTC_RD_TIME, &testtime); 
#endif
	//read(fd, &wtime);
	printf("Now is =    ' %d--%d-%s-%d  %02d:%02d:%02d   %s '\n",\
		testtime.tm_mday,\
              testtime.tm_wday,\
		monthstr[testtime.tm_mon],\
		testtime.tm_year+1900,\
		testtime.tm_hour,\
		testtime.tm_min,\
		testtime.tm_sec,\
		weekstr[testtime.tm_wday-1]);
	sleep(1);
    }
}
