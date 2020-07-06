/*
 * ad_samp.h
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */

#ifndef ADSAMP_H_
#define ADSAMP_H_
#include "os.h"
#define PERCENT 100.0
#define RMSRATIO 1.414213
#define HUDUXISHI 0.0174532922
#define SAMPHZ 5000
#define Uarms  (scada.scada_cfg.d_5212.Data.Ua.BaseValue * RMSRATIO)
#define Ubrms  (scada.scada_cfg.d_5212.Data.Ub.BaseValue * RMSRATIO)
#define Ucrms  (scada.scada_cfg.d_5212.Data.Uc.BaseValue * RMSRATIO)
#define Ua_harmonic_num  scada.scada_cfg.d_5212.Data.Ua.HarmoDgree
#define Ub_harmonic_num  scada.scada_cfg.d_5212.Data.Ub.HarmoDgree
#define Uc_harmonic_num  scada.scada_cfg.d_5212.Data.Uc.HarmoDgree
#define Ua_harmonic_radio  scada.scada_cfg.d_5212.Data.Ua.HarmContent
#define Ub_harmonic_radio  scada.scada_cfg.d_5212.Data.Ub.HarmContent
#define Uc_harmonic_radio  scada.scada_cfg.d_5212.Data.Uc.HarmContent
#define Iarms  (scada.scada_cfg.d_5212.Data.Ia.BaseValue * RMSRATIO)
#define Ibrms  (scada.scada_cfg.d_5212.Data.Ib.BaseValue * RMSRATIO)
#define Icrms  (scada.scada_cfg.d_5212.Data.Ic.BaseValue * RMSRATIO)
#define Ia_harmonic_num  scada.scada_cfg.d_5212.Data.Ia.HarmoDgree
#define Ib_harmonic_num  scada.scada_cfg.d_5212.Data.Ib.HarmoDgree
#define Ic_harmonic_num  scada.scada_cfg.d_5212.Data.Ic.HarmoDgree
#define Ia_harmonic_radio  scada.scada_cfg.d_5212.Data.Ia.HarmContent
#define Ib_harmonic_radio  scada.scada_cfg.d_5212.Data.Ib.HarmContent
#define Ic_harmonic_radio  scada.scada_cfg.d_5212.Data.Ic.HarmContent
#define AngleUaIa     scada.scada_cfg.d_5212.Data.IaAngle
#define AngleUbIb     scada.scada_cfg.d_5212.Data.IbAngle
#define AngleUcIc     scada.scada_cfg.d_5212.Data.IcAngle

typedef struct SDDATA{
	float x;
	float y;
}SData;
typedef struct EXTRAD_REGS {
    int16                 ADCRESULT0;    // Conversion Result Buffer 0
    int16                 ADCRESULT1;    // Conversion Result Buffer 1
    int16                 ADCRESULT2;    // Conversion Result Buffer 2
    int16                 ADCRESULT3;    // Conversion Result Buffer 3
    int16                 ADCRESULT4;    // Conversion Result Buffer 4
    int16                 ADCRESULT5;    // Conversion Result Buffer 5
    int16                 ADCRESULT6;    // Conversion Result Buffer 3
    int16                 ADCRESULT7;    // Conversion Result Buffer 4
}EXTRAD_REGS;
extern EXTRAD_REGS ADREG1,ADREG2,ADREG4,ADREG5,ADREG6,ADREG7,ADREG8;
extern void AdCopy(void);
extern void FaAdCopy(void);
extern void ManSet(void);
extern void AdReset(void);

extern Void HwiAdFinFxn(UArg arg);
extern int need_calculate;
extern void init_ad_edma();
#endif /* ADSAMP_H_ */
