/*
 * fft.cpp
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */
#include "product.h"
#include "DSPF_sp_fftSPxSP.h"
#include "fa_fft_cal.h"
#include "adsamp.h"
#include "DSPF_util.h"
#include <math.h>
#include "mathlist.h"
#include <ti/sysbios/knl/Clock.h>
#include "interruptapp.h"
#include "datatype.h"
//#include "timer.h"
#include "evmomapl138_timer.h"
#include "fa.h"
#include "task.h"

/*
void DSPF_sp_fftSPxSP(int N, float *ptr_x, float *ptr_w, float *ptr_y, unsigned char *brev, int n_min, int offset, int n_max)
Parameters 		N 	Number of complex values in input and output arrays. Must be a power of 2 and satisfy 8 ≤ N ≤ 65536.
		ptr_x 	Pointer to complex input array of length 2 * N. Must be double word aligned.
		ptr_w 	Pointer to complex twiddle factor array of length 2 * N. Must be double word aligned. The demonstration app includes a reference function to compute this array.
		ptr_y 	Pointer to complex output array of length 2 * N. Must be double word aligned.
		brev 	Pointer to bit reverse table containing 64 entries. This table is given in the demonstration app.
		n_min 	Smallest FFT butterfly used in computation. If N is a power of 4, this is typically set to 4. Otherwise, it is typically set to 2.
		offset 	Index (in complex samples) from start of main FFT. Typically equals 0.
		n_max 	Size of main FFT in complex samples. Typically equals N.
*/


#ifdef ENABLE_FA

#pragma DATA_SECTION("DATAINIRAM");
#pragma DATA_ALIGN(8);
static SP_COMPLEX fft_out_fa[CHANNEL_NUM][FA_SAMP_NUM] = {0};

#pragma DATA_SECTION("DATAINIRAM");
#pragma DATA_ALIGN(8);
static float w16[M];
float *const ptr_w16 = w16;

#pragma DATA_SECTION("DATAINIRAM");
float JB_YC[CHANNEL_NUM];//基波有效值 有单位的的值；
#pragma DATA_SECTION("DATAINIRAM");
static float JB_RMS[CHANNEL_NUM];//基波有效值


#pragma DATA_SECTION("DATAINIRAM");
unsigned char brev[64] = {
    0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
    0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
    0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
    0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
    0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
    0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
    0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
    0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};
#define MODULUSRADIO (2.0)  // MODULUSRADIO=  SAMP_NUM/FA_SAMP_NUM

static int jb_yc_has_cal[CHANNEL_NUM];//indicate if channel rms data has calculate.
static float tmp_yc_rms[CHANNEL_NUM];//template save rms data;


#pragma CODE_SECTION("CODEINIRAM")
Void SwiFaFxn(UArg arg1, UArg arg2)
{
	uint16 i;
	uint16 yc_id;
	uint16  channel;
	FaAdCopy();
	//calculate all channel rms 
	for(i=0;i<scada.yc_config.configed_yc_port_num;i++){
		channel=scada.yc_config.configed_yc_port[i];
		if(channel!=NO_YC){
			DSPF_sp_fftSPxSP (FA_SAMP_NUM, (float *)&AD_BUF_fa_cal[channel][0], &ptr_w16[0], (float *)&fft_out_fa[channel][0], brev, 4, 0, FA_SAMP_NUM);
			JB_RMS[channel] =fft_out_fa[channel][1].rel*fft_out_fa[channel][1].rel+fft_out_fa[channel][1].img*fft_out_fa[channel][1].img;
			JB_RMS[channel] = sqrtsp( JB_RMS[channel]);
			jb_yc_has_cal[i]=0;
		}
	}
	//calculate all jb_yc if has configed port 
	for(i=0;i<scada.yc_config.configed_yc_num;i++){
 		yc_id = scada.yc_config.configed_yc[i]; //获取配置的遥测项
		channel = scada.yc_config.config_data[yc_id].port;//通过遥测项获取对应的通道号
		if(jb_yc_has_cal[channel]==0){//rms only calculate once if dirrerent yc_id has same channel 
			JB_YC[yc_id] =JB_RMS[channel]* RATIO(0,yc_id)*MODULUSRADIO;
			tmp_yc_rms[channel]=JB_YC[yc_id];
		}else{
			JB_YC[yc_id]=tmp_yc_rms[channel];
		}
	}
	//next calculate all yc that not configed port 
 	fa.FixTimeProtect();
//	time2 = clk0.GetSysMicSec();
//	if (time2 > time1){
//		time3 = (time2 - time1);
//	}
}
#endif 
/*
*************************************************************************************
*  @brief  生成 旋转因子，提供给FFT
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void tw_gen (float *w, int n)
{
    int i, j, k;
    double x_t, y_t, theta1, theta2, theta3;
    const double P_I = 3.141592654;

    for (j = 1, k = 0; j <= n >> 2; j = j << 2)
    {
        for (i = 0; i < n >> 2; i += j)
        {
            theta1 = 2 * P_I * i / n;
            x_t = cos (theta1);
            y_t = sin (theta1);
            w[k] = (float) x_t;
            w[k + 1] = (float) y_t;

            theta2 = 4 * P_I * i / n;
            x_t = cos (theta2);
            y_t = sin (theta2);
            w[k + 2] = (float) x_t;
            w[k + 3] = (float) y_t;

            theta3 = 6 * P_I * i / n;
            x_t = cos (theta3);
            y_t = sin (theta3);
            w[k + 4] = (float) x_t;
            w[k + 5] = (float) y_t;
            k += 6;
        }
    }
}

