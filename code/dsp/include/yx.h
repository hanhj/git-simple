/*
 * yx.h
 *
 *  Created on: 2014-1-7
 *      Author: Administrator
 */

#ifndef YX_H_
#define YX_H_

#define YUEXIANYXNUM 0x16
#define SHANGXIAN 1
#define XIAXIAN 2
#include "datatype.h"
typedef struct softyxtab{
	DataType4 *SrcYcP;
	WarnPara *LimitParaP;
	uint8 LimitType;
	uint16 DataId;
	uint8 Type;
	setmstime_t Tcn[2];//越线时间与越线恢复时间；
}softyxtab_t;


extern UserPortSet     d_5085;
extern DbDigitInSet  	d_5089;
extern SOE_BUFFER SoeBuffer[MAX_USER_NUM];
extern YXMsgSet	  d_50AD;///<遥信信息体属性配置
extern YXMsgSet	  d_50AE;///<遥信信息体属性配置
extern YXMsgSet	  d_50AF;///<遥信信息体属性配置

extern int8 YxCurrent[MAX_HARDYX_NUM];//
extern int8 YxStatus[MAX_HARDYX_NUM];
extern int8 YxNature[MAXYCNUM];//按照遥测名字顺序排列的遥测状态，并且经过属性变化。
extern int8 DpYxCurrent[2];
extern int8 DpYx[2];
extern ADCONFIGYX YxConfigure;
extern softyxtab_t YueXianYx[YUEXIANYXNUM];
extern int8 GetHardYxId(int portnum);
extern int16 PushSoe(uint8 User ,uint16 PosId,uint16 data);
extern int8 YxNatureChange(int8 data, int Id);
extern int16 SendYx(void);
extern int16 GetYx(void);
extern void YxCompassInt(void);
extern void YxInit(void);
extern void  ChecksoftYx(void);
extern int16 YxReadCfg(uint32 Id);

#endif /* YX_H_ */
