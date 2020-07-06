/*
 * fft.cpp
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */



#include "DSPF_sp_fftSPxSP.h"
#include "fft.h"
#include "adsamp.h"
#include "DSPF_util.h"
#include <math.h>
#include "mathlist.h"
#include <ti/sysbios/knl/Clock.h>
#include "interruptapp.h"
#include "datatype.h"
//#include "timer.h"
#include "omapl138timer.h"
#include "fa.h"

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
//float *const ptr_x_asm = x_asm + PAD;
float *const ptr_w64 = w64 + PAD;
float *const ptr_w16 = w16 + PAD;
//float *const ptr_y_asm = y_asm + PAD;
#pragma DATA_SECTION("JB_RMSDATA");
float JB_RMS[CHENAL_NUM];//基波有效值
#pragma DATA_SECTION("brevDATA");
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
//#pragma DATA_ALIGN(brev, 8);
//#pragma CODE_SECTION(fft, ".fftSect")
//30K的数据搬移要考虑耗时


/*
*************************************************************************************
*  @brief  FFT
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
uint16 fftnum = 16;
#pragma CODE_SECTION("codeFFT")
Void Swifft(UArg arg1, UArg arg2)
{
	uint16  N ;
  	int YcNum;
  	uint8 YcName;

	ProAdCopy();
 	for(YcNum = 0; YcNum < yc_configure.yc_num; YcNum++){//检索所有的配置的遥测采样项
 		YcName = yc_configure.name[YcNum]; //获取配置的遥测项
		N = yc_configure.ad_compass[YcName];//通过遥测项获取对应的通道号
		if(N!=NO_YC){
			DSPF_sp_fftSPxSP (fftnum, (float *)&AD16_BUF[N][PAD], &ptr_w16[0], (float *)&FFT_OUT[YcName][PAD], brev, 4, 0, fftnum);
			JB_RMS[YcName] =FFT_OUT[YcName][1].rel*FFT_OUT[YcName][1].rel+FFT_OUT[YcName][1].img*FFT_OUT[YcName][1].img;//相差100US(powsp(FFT_OUT[N][PAD+i+1].rel,2)+ powsp(FFT_OUT[N][PAD+i+1].img,2));
			JB_RMS[YcName] = sqrtsp( JB_RMS[YcName]);
		}//最终得到的FFT_OUT和JB_RMS 是以遥测ID为固定顺序的
 	}

 	LineVol[Uab1].rel =  FFT_OUT[UA1][BASEHARMONIC].rel  - FFT_OUT[UB1][BASEHARMONIC].rel ;
 	LineVol[Uab1].img =  FFT_OUT[UA1][BASEHARMONIC].img  - FFT_OUT[UB1][BASEHARMONIC].img ;
 	LineVol[Ubc1].rel =  FFT_OUT[UB1][BASEHARMONIC].rel  - FFT_OUT[UC1][BASEHARMONIC].rel ;
 	LineVol[Ubc1].img =  FFT_OUT[UB1][BASEHARMONIC].img  - FFT_OUT[UC1][BASEHARMONIC].img ;
 	LineVol[Uca1].rel =  FFT_OUT[UC1][BASEHARMONIC].rel  - FFT_OUT[UA1][BASEHARMONIC].rel ;
 	LineVol[Uca1].img =  FFT_OUT[UC1][BASEHARMONIC].img  - FFT_OUT[UA1][BASEHARMONIC].img ;
 	LineVol[Uab2].rel =  FFT_OUT[UA2][BASEHARMONIC].rel  - FFT_OUT[UA2][BASEHARMONIC].rel ;
 	LineVol[Uab2].img =  FFT_OUT[UA2][BASEHARMONIC].img  - FFT_OUT[UA2][BASEHARMONIC].img ;
 	LineVol[Ubc2].rel =  FFT_OUT[UB2][BASEHARMONIC].rel  - FFT_OUT[UC2][BASEHARMONIC].rel ;
 	LineVol[Ubc2].img =  FFT_OUT[UB2][BASEHARMONIC].img  - FFT_OUT[UC2][BASEHARMONIC].img ;
 	LineVol[Uca2].rel =  FFT_OUT[UC2][BASEHARMONIC].rel  - FFT_OUT[UA2][BASEHARMONIC].rel ;
 	LineVol[Uca2].img =  FFT_OUT[UC2][BASEHARMONIC].img  - FFT_OUT[UA2][BASEHARMONIC].img ;
 	//time1 = TMR0->TIM34;//Clock_getTicks();//
 	fa.FixTimeProtect();
	//time2 = TMR0->TIM34;//Clock_getTicks();//
	//time3 = (time2 - time1)/24;
}


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

