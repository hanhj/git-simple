#ifndef AD_H
#define AD_H
#include "types.h"

#define MAX_AD_CHANNEL 4

#define AD_CHANEL1	0
#define AD_CHANEL2	1

#define CAL_DC 1
#define CAL_AC 2

#define AD_TYPE_U 1
#define AD_TYPE_I 2
#define MAX_U_VALUE 48.0 //电压型0~48V
#define MIN_U_VALUE 0.0 //电压型0~48V
#define MAX_I_VALUE 20.0 //电流型4~20mA
#define MIN_I_VALUE 4.0 //电流型4~20mA

typedef struct tag_ad_coefficent{
	float base;//最小值
	float slope;//斜率
	int8  type;//电压还是电流
}AD_COEFFICENT;
class AD
{
public:
		AD_COEFFICENT ad_coefficent[MAX_AD_CHANNEL];
	  AD();
    /**
     * 获取AD管脚电平值
     *
     * @param chanle    AD通道
     *
     * @return 读取值
     *
     */
    int16 Read ( int32 channel );
};
extern AD ad;
#endif