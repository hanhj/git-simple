/*
 * yk.h
 *
 *  Created on: 2014-1-7
 *      Author: Administrator
 */

#ifndef YK_H_
#define YK_H_
//定义遥控命令限定词
#include"dspdatatype.h"
typedef struct dco_data {
	uint8 DCS:2; //1－－开，2－－合
    uint8 QU:5; //1--短脉冲，2--长脉冲，3－－持续输出
    uint8 S_E: 1; //1--选择，0－－执行
} DCO_WORD;
typedef union dcodata {
    DCO_WORD DcoWord;
    uint8 data;
} DCOWORD;
extern 	settime_t YKtimeOut[5];//遥控超时计时器
extern	settime_t YkGapTime[5];//遥控间隔时间计时器

extern	breakerproreg_t BrkPro[5];// 断路器在线监测
	//RecloseSet d_504B;
extern	FaCommSet d_5054;///<FA功能公用参数设置
extern	uint8 HzCkType; //合闸检定类型
extern	float DiffVol;///<开关两侧电压差
extern	float DiffPhase;///<相位差设置
extern	DataType4 *DiffVolP[2];///指向两侧的电压；
extern	uint8 DiffVolName[2];//开关两侧电压定义  里面装的YC的下标号
	//遥控类数据
extern	UserPortSet d_508A;///<开关量输出配置
extern	DataType3	d_5091;///<遥控返校失败取消遥控时间
extern	DataType3	d_5092;///<遥控保持时间
extern	DataType3	d_5093;///<遥控脉冲宽度
extern	DataType3	d_5094;///<延时断开操作电源时间
extern	DataType1 d_6001;///<断路器1（开/合）
extern	DataType1 d_6002;///<断路器2（开/合）
	//DataType1 d_6064;///<电池活化（激活/退出）
	//DataType1 d_6065;///<电池退出
extern	DataType1 d_6066;///<电动机操作机构电源投入/退出
extern int16 ControlYk(int16 YkDataId,DCOWORD DCO_Word);
extern int16 BreakCloseCheck(uint8 CkTp,uint8 Enable);
extern int16 BreakRecord(int16 BreakId,uint8 RecordStep);
extern void YkInit(void);
extern int16 YkReadCfg(uint32 Id);
#endif /* YK_H_ */
