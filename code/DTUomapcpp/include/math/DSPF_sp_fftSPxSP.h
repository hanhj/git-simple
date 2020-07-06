/* ======================================================================= */
/* DSPF_sp_fftSPxSP.h -- Complex Forward FFT with Mixed Radix              */
/*                 Assembly Implementation                                 */
/*                                                                         */
/* Rev 0.0.2                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_fftSPxSP_H_
#define _DSPF_SP_fftSPxSP_H_
extern "C"{
/*
 * n_min : N = 2的 rad次方，rad%2=0  n_min = 4; 否则等于2
 */
void DSPF_sp_fftSPxSP(int N, float *ptr_x, float *ptr_w, float *ptr_y, unsigned char *brev, int n_min, int offset, int n_max);
}
#endif /* _DSPF_SP_fftSPxSP_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_fftSPxSP.h                                       */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */
