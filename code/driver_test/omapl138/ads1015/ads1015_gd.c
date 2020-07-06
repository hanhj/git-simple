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
	sprintf(device, "/sys/devices/platform/i2c-gpio/i2c-0/0-0049/in%d_input", device_id + 4);
	fp = fopen(device, "rb");
	if (fp == NULL) {
		return 0;
	}
	len = fread(data, 20, 1, fp);
	fclose(fp);
	for (i = 0; i < len; i++) {
		if (data[i] > '9' || data[i] < '0') {
			data[i] = '\0';
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
