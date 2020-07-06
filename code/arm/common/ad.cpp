#include <stdlib.h>
#include <string.h>
#include "ad.h"
#include "debug.h"
#include "utils.h"
AD ad;
AD::AD()
{
	

}
int16 AD::Read ( int32 channel )
{
	char device[256];
	char data[32];
	int i;
	int len;
	FILE *fp=NULL;

	memset(data, 0, 32);
	sprintf(device, "/sys/devices/platform/i2c-gpio.1/i2c-1/1-0049/in%d_input", (int)channel);
	fp = myfopen(device, "rb");
	if (fp == NULL) {
		PFUNC(TEM_ERROR,DEBUG_NORMAL,"error on open %s\n",device);
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
