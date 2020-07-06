/*
 * File Name: utils.cpp
 * Author: hanhj
 * 
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of
 * any kind, whether express or implied.
 * 
 * Created Time: 2020-06-17 23:40:12 week:3
*/
#include <ti/sysbios/knl/Task.h>
#include "utils.h"

int udelay ( uint32 usecond )
{
#ifdef CONFIG_LINUX
    return select ( 0, NULL, NULL, NULL, &tval );
#else
    Task_sleep ( usecond*12 );
    return 1;
#endif
}

int mdelay ( uint32 msecond )
{
    return udelay ( msecond * 1000 );
}

int delay ( uint32 second )
{
    return mdelay ( second * 1000 );
}

//从小到大
void SortUint8(uint8_t *p,uint8_t p_size)
{
    uint8_t i,j,temp;
    uint8_t *p1=p;
    if (p_size<=1)
        return;
    for (i=1;i<=p_size;i++) {
        for (j=p_size-1;j>=i;j--) {
            if (p1[j]<p1[j-1]) {
                temp=p1[j-1];
                p1[j-1]=p1[j];
                p1[j]=temp;
            }
        }
    }
}
void SortUint16(uint16_t *p,int16_t p_size) //冒泡排序(小到大)
{
    uint16_t i,j,temp;
    uint16_t *p1=p;
    if (p_size<=1)
        return;
    for (i=0;i<p_size;i++) {
        for (j=0;j<p_size-i-1;j++) {
            if (p1[j]>p1[j+1]) {
                temp=p1[j];
                p1[j]=p1[j+1];
                p1[j+1]=temp;
            }
        }
    }
}

void SortFloat(float *p,int16 p_size) //泡排序(大到小)
{
    int16 i,j;
    float temp;
    float *p1=p;
    if (p_size<=1)
        return;
    for (i=0;i<p_size;i++) {
        for (j=0;j<p_size-i-1;j++) {
            if (p1[j]<p1[j+1]) {
                temp=p1[j];
                p1[j]=p1[j+1];
                p1[j+1]=temp;
            }
        }
    }
}

float GetAverage(int16 *p ,int16 size){
	int i;
	float avr=0;
	for(i=0;i<size;i++){
		avr+=p[i];
	}
	return avr/size;
}
#if 0
float GetAverage(int16 *p ,int16 p_size, int16 averager_size)
{
	int16 i;
	int16 hafsize;
	float avr = 0;
//	float *var = new float[p_size];
	float var[48];
	for ( i = 0 ; i < p_size; i++){
		var[i] = p[i];
	}
	SortFloat(var,p_size);
	hafsize = averager_size/2;
	for ( i = hafsize ; i < (p_size-hafsize); i++){
		avr+=var[i];
	}
	avr/=(p_size-averager_size);
//	delete var;
	return avr;
}
#endif 

void DelayUs(Uint32 us){
	uint32 i,j;
	for ( j = 0 ;j < us; j++){
		for ( i = 0; i<75; i++){//per us
			asm ("nop");
		}
	}
}

/***********************************************************************
**  Time:        liwenjin 2012.10
**  Function:BCD转BIN
**  Input:
**  Output:        None
**  Attention:
***********************************************************************/
uint8 bcd2bin(uint8 val)
{
	return ((val&0x0f)+(val >> 4)*10);
}
/***********************************************************************
**  Time:        liwenjin 2012.10
**  Function:BIN转BCD
**  Input:
**  Output:        None
**  Attention:
***********************************************************************/
uint8 bin2bcd(uint8 val)
{
	return ((val / 10) << 4) + val % 10;
}
/**
*************************************************************************************
*  @brief 消除int数组中的重复数字
*  @param[in] *in 输入数组
*  @param[in] *in_len 输入数组长度
*  @param[in] ignore ignore this data  
*  @param[out] *out 输出数组
*  @return  返回数组中不重复的数长度。
*  @note 需要用户提前分配好输入输出数组空间
*  时间复杂度O(N^2)
*  如果用折半查找时间复杂度为O(N*log_2N)
**************************************************************************************
*/
int uniq_int_array(uint16 *in,uint16 in_len,uint16 ignore,uint16 *out){
	int flag;
	int i,j;
	int out_len;
	flag=0;
	out_len=0;
	for(i=0;i<in_len;i++){
		flag=0;
		if(in[i]==ignore)
			continue;
		for(j=0;j<out_len;j++){
			if(in[i]==out[j]){
				flag=1;
				break;
			}
		}
		if(flag==0){
			out[out_len++]=in[i];
		}
	}
	return out_len;
}
int calcrc ( uint8 *ptr, int count )
{
    int crc = 0;
    int i;

    while ( --count >= 0 )
    {
        crc = crc ^ ( int16_t ) * ptr++ << 8;
        i = 8;

        do
        {
            if ( crc & 0x8000 )
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        }
        while ( --i );
    }
	crc+=1;
    return ( crc );
}
// vim:tw=72 
