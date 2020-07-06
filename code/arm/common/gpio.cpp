#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <device.h>
#include "gpio.h"
#include "debug.h"
#include "utils.h"


GPIO gpio;

GPIO::GPIO()
{
	fd = -1;	
	fd = myopen(DEV_GPIO, O_RDWR);
	if (fd < 0) {
		PFUNC(TEM_ERROR,DEBUG_NORMAL,"open GPIO device error! %d\n", fd);
	}
}
GPIO::~GPIO(){
	if(fd>0)
		close(fd);
}

int16 GPIO::Read ( int16 PIN )
{
int16 ret=0;	
	if(fd>0){
	ret=ioctl(fd, GET_VALUE, (unsigned long)PIN);	
	}
	else{
		ret=-1;
	PFUNC(TEM_ERROR,DEBUG_NORMAL,"PIN:%d not work\n",PIN);
	}
//PFUNC(TEM_INFO,DEBUG_NORMAL,"get pin:%d value:%d\n",PIN,ret);		
return ret;
}
int GPIO::Write ( int16 PIN ,int16 value)
{

	PFUNC(TEM_INFO,DEBUG_NORMAL,"set pin:%d value:%d\n",PIN,value);		
	if(fd>0){
	if(value==0)
	ioctl(fd, SET_OUTPUT_LOW, (unsigned long)PIN);
	else
	ioctl(fd, SET_OUTPUT_HIGH, (unsigned long)PIN);	
	return 0;
	}
	else{
	PFUNC(TEM_ERROR,DEBUG_NORMAL,"PIN:%d not work\n",PIN);
	return -1;
	}
}

int GPIO::Init ( int16 PIN,int16 dir,int16 value )
{
	
	if(fd>0){
		if(!dir)
			ioctl(fd, SET_INPUT, (unsigned long)PIN);
		else{
			if(value==0)
			ioctl(fd, SET_OUTPUT_LOW, (unsigned long)PIN);
			else
			ioctl(fd, SET_OUTPUT_HIGH, (unsigned long)PIN);	
		}
	return 0;	
	}
	return -1;	
}
