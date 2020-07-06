/*
 * fft.h
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */

#ifndef FFT_H_
#define FFT_H_

#include <ti/sdo/utils/List.h>
#include "adsamp.h"
#include "types.h"
#include "datatype.h"


typedef enum BaseLineVol{
	Uab1 = 0,
	Ubc1,
	Uca1,
	Uab2,
	Ubc2,
	Uca2,
}BaseLineVol_E;


extern Void Swifft(UArg arg1, UArg arg2);
extern void tw_gen (float *w, int n);
extern void get_rms(void);

extern 	uint64 time1, time2,time3;
extern float JB_RMS[CHENAL_NUM];
extern unsigned char brev[64];
#endif /* FFT_H_ */
