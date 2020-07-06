/*
 * ad_samp.h
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */

#ifndef ADSAMP_H_
#define ADSAMP_H_
#include "os.h"
/*
*************************************************************************************
*  @brief  不可屏蔽中断   功能：从FPGA 读取数据
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/

#define RMSRATIO 1.414213
#define HUDUXISHI 0.0174532922
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

extern Void nmifxn(UArg arg);
extern void AdCopy(void);
extern void ProAdCopy(void);
extern void ManSet(void);
#endif /* ADSAMP_H_ */
