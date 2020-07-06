#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2000


int read_adc(int device_id)
{
	char device[256];
	char data[32];
	int i;
	int len;
	FILE *fp;

	memset(data, 0, 32);
	sprintf(device, "/sys/devices/platform/i2c-gpio.1/i2c-1/1-0049/in%d_input", device_id);
	fp = fopen(device, "rb");
	if (fp == NULL) {
		//printf("-- read_adc, open adc[%d] fail! \n", device_id);
		return 0;
	}
        //printf("-- read_adc, open device[%s] OK!! \n", device);
	len = fread(data, 20, 1, fp);
	//printf("-- read_adc, data[%s] \n", data);
	fclose(fp);
        //printf("-- read_adc, read adc[%d] len [%d] \n", device_id, len);
	for (i = 0; i < len; i++) {
		if (data[i] > '9' || data[i] < '0') {
			data[i] = '\0';
			//printf("-- read_adc, adc[%d] data[%s] \n", device_id, data);
			break;
		}
	}
	return atoi(data);
}

int main(int argc, char *argv[])
{
	int i = 0;
	int value = 0;
    while(1){    
	for (i=0;i<8;i++) {
	    value  = read_adc(i);
            printf("-- value[%d] = [%d] \n", i, value);
	}
	printf("\n");
	sleep(1);
	}
	exit(0);
}
