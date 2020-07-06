/*
 * yk.cpp
 *
 *  Created on: 2014-1-7
 *      Author: Administrator
 */

#include "adsamp.h"
#include "dspdatatype.h"
#include "fpga.h"
#include "clock.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include "yk.h"
#include "scada.h"
#include "yx.h"
settime_t YKtimeOut[5];//遥控超时计时器
settime_t YkGapTime[5];//遥控间隔时间计时器
breakerproreg_t BrkPro[5];// 断路器在线监测
FaCommSet d_5054;///<FA功能公用参数设置
uint8 HzCkType; //合闸检定类型
float DiffVol;///<开关两侧电压差
float DiffPhase;///<相位差设置
DataType4 *DiffVolP[2];///指向两侧的电压；
uint8 DiffVolName[2];//开关两侧电压定义  里面装的YC的下标号
//遥控类数据
UserPortSet d_508A;///<开关量输出配置
DataType3	d_5091;///<遥控返校失败取消遥控时间
DataType3	d_5092;///<遥控保持时间
DataType3	d_5093;///<遥控脉冲宽度
DataType3	d_5094;///<延时断开操作电源时间
DataType1 d_6001;///<断路器1（开/合）
DataType1 d_6002;///<断路器2（开/合）
DataType1 d_6066;///<电动机操作机构电源投入/退出
void YkInit(void)
{
		d_508A.Data.Define[0].Port = 0x0;//遥控通道0
		d_508A.Data.Define[1].Port = 0x1;//遥控通道1
		d_508A.Data.Define[2].Port = 0x2;//遥控通道2
		d_5094.Data = 1000;
		d_5092.Data = 1000;
		d_5089.Data.Define[0].ClosePort = 0;//合位置
		d_5089.Data.Define[0].OpenPort = 1;//分位置
}
int16 YkReadCfg(uint32 Id)
{	uint16 i;
	Data<BaseDataType> data;
	RecloseSet d_504B;
	DCOWORD Dco;
	DataType4 *Pyc1=&YcData.ua1_4001;
	user_data.GetData(Id,&data);
	switch ( Id ){
	case 0x508a:
		//UserPortSet d_508A;///<开关量输出配置
		memcpy(&d_508A,data.Data,sizeof(d_508A));
		break;
	case 0x6001:
		memcpy(&d_6001,data.Data,sizeof(d_6001));
		Dco.data = d_6001.Data;
		ControlYk(Id,Dco);
		//DataType1 d_6001;///<断路器1（开/合）
		break;
	case 0x6002:
		memcpy(&d_6002,data.Data,sizeof(d_6002));
		Dco.data = d_6002.Data;
		ControlYk(Id,Dco);
		break;
	case 0x6066:
		memcpy(&d_6066,data.Data,sizeof(d_6066));
	//DataType1 d_6066;///<电动机操作机构电源投入/退出
		Dco.data = d_6066.Data;
		ControlYk(Id,Dco);
		break;
	case 0x504b:
		memcpy(&d_504B,data.Data,sizeof(d_504B));
		DiffVol = d_504B.Data.DiffVol;
		DiffPhase = d_504B.Data.DiffPhase;
		HzCkType = d_504B.Data.Type;
		for ( i = 0; i < 2; i++){
			if(d_504B.Data.DiffVolDefine[i]<=0x4402){
				DiffVolP[i] = &Pyc1[d_504B.Data.DiffVolDefine[i] - 0x4001];
			}
		}
		for ( i = 0; i < 2; i++){
			if(d_504B.Data.DiffVolDefine[i]<=0x400d){
				DiffVolName[i] = d_504B.Data.DiffVolDefine[i] - 0x4001;
			}else if ((d_504B.Data.DiffVolDefine[i]<=0x4106)&&(d_504B.Data.DiffVolDefine[i]>=0x40fa)){
				DiffVolName[i] = d_504B.Data.DiffVolDefine[i] - 0x40fa+13;
			}
		}
		break;
	case 0x5054:
		memcpy(&d_5054,data.Data,sizeof(d_5054));
		break;
	default:
		break;
	}
	return 1;
}
int16 CheckYkState(int16 breaknum,DCOWORD DCO_Word)
{
	uint8 hewz;
	uint8 fenwz;
	uint8 cnwz;
	uint8 JhhEnable;
    switch ( breaknum ) {
    case BREAKER1://断路器1
    	hewz = HE1;
    	fenwz = FEN1;
    	cnwz = CN1;
    	JhhEnable = 0;
        break;
    case BREAKER1JHH://断路器1
    	hewz = HE1;
    	fenwz = FEN1;
    	cnwz = CN1;
    	JhhEnable = 1;
    	break;
    case BREAKER2://断路器2
    	hewz = HE2;
    	fenwz = FEN2;
    	cnwz = CN2;
    	JhhEnable = 0;
        break;
    case BREAKER2JHH://断路器2
    	hewz = HE2;
    	fenwz = FEN2;
    	cnwz = CN2;
    	JhhEnable = 1;
    	break;
	case MOTOR://电机
		return 1;
    default:
        return -1;
    }
	if ( DCO_Word.DcoWord.DCS == 0x02 ) {//合
				if(
						(YxNature[hewz]==FEN_FEN)//断路器位置 分
						&&
						(YxNature[cnwz]==CHUNENG)//储能 储能
						&&
						(BreakLockState[breaknum]==NOLOCK)//闭锁状态
						&&
						(BreakCloseCheck(HzCkType,JhhEnable)==1)//结合环功能
				)
				{
					return 1;
				}else{
					return -1;
				}
			} else if ( DCO_Word.DcoWord.DCS == 0x01 ) {//分
				if(
						(YxNature[fenwz]==HE_HE)//断路器位置 合
						//&&
						//(YxNature[YxConfigure.ad_compass[CN1]]==CHUNENG)//储能 储能
						&&
						(BreakLockState[breaknum]==NOLOCK)//闭锁状态
				)
				{
					return 1;
				}else{
					return -1;
				}
			}
	return -1;
}

int16 CheckNoVol(void)
{
	int16 rel;
	if((DiffVolP[0]->Data > d_5054.Data.StartU)
			&&
		(DiffVolP[1]->Data > d_5054.Data.StartU)
	)
	{
		rel = -1;
	}else {
		rel = 1;
	}
	return rel;
}

int16 CheckSyn(void)
{
	int16 rel;
	rel = 1;
	if (YcData.difangle_4401.Data>0){
		if(YcData.difangle_4401.Data>DiffPhase){
			rel = -1;
		}
	}else{
		if((-1.0*YcData.difangle_4401.Data)>DiffPhase){
			rel = -1;
		}
	}
	return rel;
}
/*
 * 非同期:0、检无压:1、检同期:2、检无压+检同期:3（检无压不成功自动转入检同期）,默认非同期。
 */

int16 BreakCloseCheck(uint8 CkTp,uint8 Enable)
{
	int16 rel;
	if(Enable == 0){
		return 1;
	}
	switch (CkTp)
	{
		case NOCHECK:
			rel = 1;
			break;
		case CKNOVOL://两侧都有压时返回无效
			rel = CheckNoVol();
			break;
		case CKSYN:
			rel = CheckSyn();
			break;
		case CKNOVOLSYN://并的关系
			if (CheckNoVol()!=1){//检无压不成功自动进入检同期
				rel = CheckSyn();
			}else {
				rel = 1;
			}
			break;
		default:
			break;
	}
	return rel;
}

int16 YkOut(int16 YkDataId,DCOWORD DCO_Word)
{
	uint8 YkPort;
	uint8 FpgaNum;
	int16 erro;
	uint32 dataId;
	DataType1 data;
	uint16 YkTd;
	CP56Time2a Time;
	YkPort = d_508A.Data.Define[YkDataId].Port;
	if((FpgaCfg.YxCardSize<YkPort)||(FpgaCfg.YxCardSize==0)){
		return -1;
	}
	FpgaNum = FpgaCfg.YxCard[YkPort].num;
	if(YkDataId==MOTOR){//电机控制
		YkTd = d_5094.Data;
	}else {
		YkTd = d_5092.Data;
	}
	if ( DCO_Word.DcoWord.DCS == 0x02 ) {//合
		erro = FpgaOut(FpgaNum,YkPort,CLOSEBREAKER,YkTd);
		data.Data = 2;
	}else if ( DCO_Word.DcoWord.DCS == 0x01 ) {//跳
		erro = FpgaOut(FpgaNum,YkPort,OPENBREAKER,YkTd);
		BreakRecord(YkDataId,BRKRCDSTART);
		data.Data = 1;
	}
	Time = clk0.GetCP56Time2a();
	if (erro==1){
		dataId = 0x003b+YkDataId;
		msg.SendMsg(TASK_VARM,MSG_YX,dataId,&data,&Time);
	}else{//断路器异常
		dataId = 0x0014+YkDataId*0x1a;
		msg.SendMsg(TASK_VARM,MSG_YX,dataId,&data,&Time);
	}
	return 1;
}

//接收的 是DCO_Word ,返回的是传送原因
int16 ControlYk(int16 YkDataId,DCOWORD DCO_Word)
{
	uint8 breaknum;
	DataType1 BsCause;
	BsCause.Data = 0;
	uint16 millisecond;
	uint8 minute;
	CP56Time2a Time;
	breaknum = YkDataId - 0x6001;
    if (( DCO_Word.DcoWord.S_E == 1 )) { //选择
		if(CheckYkState(breaknum,DCO_Word)==1){//选择上
			clk0.StopClock(&YKtimeOut[breaknum]);
			millisecond = d_5091.Data * 1000;
			minute = millisecond/60000;
			millisecond = millisecond%60000;
	    	clk0.StartClock(&YKtimeOut[breaknum],0,0,0,0,minute,millisecond);//遥控选择超时默认一分钟后不准执行
	    	BsCause.Data = CAUSE_Actcon;
		}else{//未选择上
			BsCause.Data = CAUSE_Deactcon;
		 }
    } else if
    (
		(CheckYkState(breaknum,DCO_Word)==1)
		&&
		(clk0.CheckClock(&YKtimeOut[breaknum])!=1)//未超时
    )
    {//执行
    	clk0.StopClock(&YKtimeOut[breaknum]);
    	BsCause.Data = CAUSE_Actcon;
    	YkOut( breaknum, DCO_Word);
    } else {//取消
    	BsCause.Data = CAUSE_Deactcon;
    }
    Time = clk0.GetCP56Time2a();
    msg.SendMsg(TASK_VARM,MSG_YK,YkDataId,&BsCause,&Time);
    return 1;
}
/*可记录断路器每次切断故障电流的水平、
动作持续时间（从出口动作命令发出到对应断路器遥信变位）
、动作次数，用于可以监视断路器触头受电腐蚀的程度以及断路器的机械性能
*/
int16 BreakRecord(int16 BreakId,uint8 RecordStep)
{
uint8 fenwz = 0;
DataType1 data;
uint32 dataId;
CP56Time2a Time;
   if(RecordStep==BRKRCDSTART){
	  BrkPro[BreakId].step = BRKRCDSTART;
   }else if(BrkPro[BreakId].step == BRKRCDCPL){
		 return -1;
	}
	switch ( BreakId )
 	{
	case 0://断路器1
		fenwz = FEN1;
		break;
	case 1://断路器2
		fenwz = FEN2;
		break;
	case 0x65://电机
		return 1;
	default:
		return -1;
	}
	if(BrkPro[BreakId].step == BRKRCDSTART){
	BrkPro[BreakId].BreakProSoe.ia = YcData.ia11_4007.Data;
	BrkPro[BreakId].BreakProSoe.ib = YcData.ib11_4008.Data;
	BrkPro[BreakId].BreakProSoe.ic = YcData.ic11_4009.Data;
	BrkPro[BreakId].BreakProSoe.cnt++;
	BrkPro[BreakId].StartTime = clk0.GetSysMs();
	clk0.StopMsClock(&BrkPro[BreakId].Tcnt);
	clk0.StartMsClock(&BrkPro[BreakId].Tcnt,1000);//1秒
	BrkPro[BreakId].step = BRKRCDCHECK;
	}else if(BrkPro[BreakId].step == BRKRCDCHECK) {
		if(clk0.CheckMsClock(&BrkPro[BreakId].Tcnt)!=1){
			if (YxNature[fenwz]==FEN_FEN){
				BrkPro[BreakId].EndTime  = clk0.GetSysMs();
				BrkPro[BreakId].BreakProSoe.Time = BrkPro[BreakId].EndTime - BrkPro[BreakId].StartTime ;
				BrkPro[BreakId].step = BRKRCDCPL;
			}
		}else{//断路器异常
			dataId = 0x0014+BreakId*0x1a;
			data.Data = 0x01;
			Time = clk0.GetCP56Time2a();
			msg.SendMsg(TASK_VARM,MSG_YX,dataId,&data,&Time);
			BrkPro[BreakId].step = BRKRCDCPL;
		}
	}
	return 1;
}
