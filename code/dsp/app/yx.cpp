/*
 * yx.cpp
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
#include "yx.h"
#include "scada.h"


int8 YxCurrent[MAX_HARDYX_NUM];//
int8 YxStatus[MAX_HARDYX_NUM];
int8 YxNature[MAXYCNUM];//按照遥测名字顺序排列的遥测状态，并且经过属性变化。
int8 DpYxCurrent[2];
int8 DpYx[2];
UserPortSet     d_5085;
DbDigitInSet  	d_5089;
SOE_BUFFER    SoeBuffer[MAX_USER_NUM];
YXMsgSet	  d_50AD;///<遥信信息体属性配置
YXMsgSet	  d_50AE;///<遥信信息体属性配置
YXMsgSet	  d_50AF;///<遥信信息体属性配置
WarnPara    d_520A;///<电压上限告警阈值，恢复阈值，持续时间
WarnPara    d_520B;///<电压下限告警阈值，恢复阈值，持续时间
WarnPara    d_520C;///<电流上限阈值，恢复阈值，持续时间
WarnPara    d_520D;///<负荷越上限阈值，恢复阈值，持续时间
WarnPara TemLimit;
WarnPara HumLimit;
softyxtab_t YueXianYx[YUEXIANYXNUM];
ADCONFIGYX YxConfigure;
DataType3 d_5090;///<遥信防抖时间
#pragma CODE_SECTION("YXCODEINDDR2")
int16 YxReadCfg(uint32 Id)
{
	uint16 i;
	uint16 name_pos = 0;
	Data<BaseDataType> data;
	DataType4   tmp;///<温度定值
	user_data.GetData(Id,&data);
	switch(Id)
	{
	case 0x5090:
		//DataType3		d_5090;///<遥信防抖时间
		memcpy(&d_5090,data.Data,sizeof(d_5090));
		break;
	case 0x5085:
		//UserPortSet  d_5085;///<单点遥信开关量输入配置
		memcpy(&d_5085,data.Data,sizeof(d_5085));

		for (i = 0; i < MAXHDYXNUM; i++){
			YxConfigure.ad_compass[i] = d_5085.Data.Define[i].Port;
		}
		YxConfigure.yx_num = MAXHDYXNUM;
		name_pos = 0;
		for (i = 0; i < MAXHDYXNUM; i++){
			if (YxConfigure.ad_compass[i] == NO_YX){
				YxConfigure.yx_num--;
			}else{
				YxConfigure.name[name_pos++] = i;
			}
		}
		break;
	case 0x5086:
		//UserPortSet  d_5086;///<单点遥信开关量输入配置
		break;
	case 0x5087:
		//UserPortSet  d_5087;///<单点遥信开关量输入配置
		break;
	case 0x5088:
		//UserPortSet  d_5088;///<单点遥信开关量输入配置
		break;
	case 0x5089:
		//DbDigitInSet  d_5089;///<双点遥信开关量输入配置
		memcpy(&d_5089,data.Data,sizeof(d_5089));
		break;
	case 0x50ad:
		//YXMsgSet	  d_50AD;///<遥信信息体属性配置
		memcpy(&d_50AD,data.Data,sizeof(d_50AD));
		break;
	case 0x50ae:
		//YXMsgSet	  d_50AE;///<遥信信息体属性配置
		memcpy(&d_50AE,data.Data,sizeof(d_50AE));
		break;
	case 0x50af:
		//YXMsgSet	  d_50AF;///<遥信信息体属性配置
		memcpy(&d_50AF,data.Data,sizeof(d_50AF));
		break;
	case 0x5202:
		memcpy(&tmp,data.Data,sizeof(tmp));
		TemLimit.Data.Limit = tmp.Data;
		TemLimit.Data.Recover = tmp.Data - 0.5;
		TemLimit.Data.ContinueTime = 1000;////1秒；
		break;
	case 0x5203:
		memcpy(&tmp,data.Data,sizeof(tmp));
		HumLimit.Data.Limit = tmp.Data;
		HumLimit.Data.Recover = tmp.Data - 1.0;
		HumLimit.Data.ContinueTime = 1000;////1秒；
		break;
	case 0x520A:
		//WarnPara    d_520A;///<电压上限告警阈值，恢复阈值，持续时间
		memcpy(&d_520A,data.Data,sizeof(d_520A));
		break;
	case 0x520B:
		//WarnPara    d_520B;///<电压下限告警阈值，恢复阈值，持续时间
		memcpy(&d_520B,data.Data,sizeof(d_520B));
		break;
	case 0x520C:
		//WarnPara    d_520C;///<电流上限阈值，恢复阈值，持续时间
		memcpy(&d_520C,data.Data,sizeof(d_520C));
		break;
	case 0x520D:
		//WarnPara    d_520D;///<零序电流越上限阈值，恢复阈值，持续时间
		memcpy(&d_520D,data.Data,sizeof(d_520D));
		break;
	default:
		break;
	}
}
#pragma CODE_SECTION("YXCODEINDDR2")
void YxCompassInt(void)
{
	unsigned char i;
	unsigned char name_pos = 0;
    d_5085.Data.Define[HE1].Id =0x0001;
    d_5085.Data.Define[HE1].Port = 0;
    d_5085.Data.Define[FEN1].Id =0x0002;
    d_5085.Data.Define[FEN1].Port = 1;
    d_5085.Data.Define[CN1].Id =0x0003;
    d_5085.Data.Define[CN1].Port = 2;
    d_5085.Data.Define[GLD1].Id =0x0004;
    d_5085.Data.Define[GLD1].Port = 3;
    d_5085.Data.Define[YX15].Id =0x0005;
    d_5085.Data.Define[YX15].Port = 4;
    d_5085.Data.Define[YX16].Id =0x0006;
    d_5085.Data.Define[YX16].Port = 5;
    d_5085.Data.Define[XJGZ1].Id =0x0011;
    d_5085.Data.Define[XJGZ1].Port = 0xff;
    d_5085.Data.Define[JDGZ1].Id =0x0012;
    d_5085.Data.Define[JDGZ1].Port =0xff;
    d_5085.Data.Define[SP6GZ1].Id =0x0015;
    d_5085.Data.Define[SP6GZ1].Port = 0xff;
    d_5085.Data.Define[SP6BS1].Id =0x0015;
    d_5085.Data.Define[SP6BS1].Port = 0xff;

    d_5085.Data.Define[HE2].Id =0x001b;
    d_5085.Data.Define[HE2].Port = 0xff;
    d_5085.Data.Define[FEN2].Id =0x001c;
    d_5085.Data.Define[FEN2].Port = 0xff;
    d_5085.Data.Define[CN2].Id =0x001d;
    d_5085.Data.Define[CN2].Port = 0xff;
    d_5085.Data.Define[GLD2].Id =0x001e;
    d_5085.Data.Define[GLD2].Port = 0xff;
    d_5085.Data.Define[YX25].Id =0x001f;
    d_5085.Data.Define[YX25].Port = 0xff;
    d_5085.Data.Define[YX26].Id =0x0020;
    d_5085.Data.Define[YX26].Port = 0xff;
    d_5085.Data.Define[XJGZ2].Id =0x002b;
    d_5085.Data.Define[XJGZ2].Port = 0xff;
    d_5085.Data.Define[JDGZ2].Id =0x002c;
    d_5085.Data.Define[JDGZ2].Port =0xff;
    d_5085.Data.Define[SP6GZ2].Id =0x002f;
    d_5085.Data.Define[SP6GZ2].Port = 0xff;
    d_5085.Data.Define[SP6BS2].Id =0x0030;
    d_5085.Data.Define[SP6BS2].Port = 0xff;
    d_5085.Data.Define[SP6BS2].Id =0x0035;
    d_5085.Data.Define[YFJD].Port = 0xff;

	for ( i = 0; i < MAXHDYXNUM; i++){
		YxConfigure.ad_compass[i] = i;
	}
	YxConfigure.yx_num = MAXHDYXNUM;
	for ( i = 0; i < MAXHDYXNUM; i++){
		if (YxConfigure.ad_compass[i] == NO_YX){
			YxConfigure.yx_num--;
		}else{
			YxConfigure.name[name_pos++] = i;
		}
	}
}

#pragma CODE_SECTION("YXCODEINDDR2")
int16 PopSoe(uint8 User,uint16 Index,SOE_DATA *SoeData)
{
	if (SoeBuffer[User].SoeData[Index].ReadFlag == NOREAD){
		SoeBuffer[User].SoeData[Index].ReadFlag = READCLEAR;
		(*SoeData) = SoeBuffer[User].SoeData[Index];
		return 1;
	}else {
		return 0;
	}
}

int16 PushSoe(uint8 User ,uint16 PosId,uint16 data)
{
	if (PosId==0xffff){
		return 1;
	}
	SoeBuffer[User].SoeData[SoeBuffer[User].SoeCnt].PosId = PosId;
	SoeBuffer[User].SoeData[SoeBuffer[User].SoeCnt].Data = data;
	SoeBuffer[User].SoeData[SoeBuffer[User].SoeCnt].Time  = clk0.GetCP56Time2a();
	SoeBuffer[User].SoeData[SoeBuffer[User].SoeCnt].ReadFlag = NOREAD;
	if (SoeBuffer[User].SoeCnt++ >=99){
		SoeBuffer[User].SoeCnt = 0;
	}
	return 1;
}

int8 GetHardYxId(int portnum)
{
	return d_5085.Data.Define[portnum].Id;
}

int8 ReadYxFromFpga(void)
{
	int8 i ;
	YXDATA_t yxdata;
	for (i = 0 ; i < FpgaCfg.YxCardSize; i++){
		if(FpgaCfg.YxCard[i].num>=SLAVEFPGANUM){
			return -1;
		}
		yxdata.all =FpgaSlave[FpgaCfg.YxCard[i].num].YX1.all;//FpgaSlave[3].YX1.all;
		YxCurrent[i*6] = yxdata.bit.yx1;
		YxCurrent[i*6+1] = yxdata.bit.yx2;
		YxCurrent[i*6+2] = yxdata.bit.yx3;
		YxCurrent[i*6+3] = yxdata.bit.yx4;
		YxCurrent[i*6+4] = yxdata.bit.yx5;
		YxCurrent[i*6+5] = yxdata.bit.yx6;
	}
 return 1;
}
//输入遥信ID 返回属性
int8 YxNatureChange(int8 data, int Id)
{
	uint16 pos;
	uint16 type;
	if (Id<=0x28){
		pos = Id;
		type = d_50AD.Data.Define[pos].Type;
	}else if (Id<=0x3A){
		pos = Id-0x29;
		type = d_50AE.Data.Define[pos].Type;
	}else if (Id<=0x216){
		pos = Id-0x201+18;
		type = d_50AE.Data.Define[pos].Type;
	}else if (Id<=0x21A){
		pos = Id-0x217;
		type = d_50AF.Data.Define[pos].Type;
	}
	if ( type== 1){
		if (data==0){
			return 1;
		}else if (data==1){
			return 0;
		}
	}else
	return data;
}
#pragma CODE_SECTION("YXCODEINDDR2")
int16 SendYx(void)
{
	uint8 i,j;
	DataType1 data;
	Data<BaseDataType> yxtmp;
	for ( j = 0;j < 3;j++){//
		for ( i =0 ; i<100; i++){
			if (SoeBuffer[j].SoeData[i].ReadFlag == NOREAD){
				SoeBuffer[j].SoeData[i].ReadFlag = READCLEAR;
				data.Data = SoeBuffer[j].SoeData[i].Data;
				yxtmp.Data = &data;
				msg.SendMsg(TASK_VARM,MSG_YX,SoeBuffer[j].SoeData[i].PosId,&data,&SoeBuffer[j].SoeData[i].Time);
				user_data.SetData(SoeBuffer[j].SoeData[i].PosId,&yxtmp);//更新遥测表
			}
		}
	}
	return 1;
}
int16 GetYx(void )
{
	uint8 i,j;
	uint8 YxDataId;
	ReadYxFromFpga();
	uint8 heport;
	uint8 fenport;
	for (i = 0; i < MAX_HARDYX_NUM; i++){
		if(YxCurrent[i]!=YxNature[i]){
			YxStatus[i]= YxCurrent[i];
			for (j = 0; j < MAXYCNUM; j++){
				if (d_5085.Data.Define[j].Port == i ){
					YxDataId = d_5085.Data.Define[j].Id;
					YxNature[j] = YxNatureChange(YxStatus[i],YxDataId);
					PushSoe(HDYXSOE,YxDataId,YxNature[i]);
				}
			}
		}
	}
/*	for(i = 0;i < 2;i++){//太大了  断路器记录
		BreakRecord(i,BRKRCDCHECK);
	}*/

	for (i = 0; i < 2; i++){
		heport = d_5089.Data.Define[i].ClosePort;
		fenport = d_5089.Data.Define[i].OpenPort;
		if((heport>=MAX_HARDYX_NUM)||(heport>=MAX_HARDYX_NUM)){
			break;
		}
		DpYxCurrent[i] = (YxStatus[heport]<<1) + YxStatus[fenport];
		if(DpYx[i]!=DpYxCurrent[i]){
			DpYx[i] = DpYxCurrent[i];
			YxDataId = d_5089.Data.Define[i].Id;
			PushSoe(DPYXSOE,YxDataId,DpYx[i]);
		}
	}
return 1;
}
//越线遥信DataId 需要从 d_50AD ，d_50AE，d_50AF中获取,由于DataId是固定的，此处也可以直接赋值；
#pragma CODE_SECTION("YXCODEINDDR2")
int16 YueXianYxInit(void)
{
	//DataType1 d_0201;///<线路1 A相电压越上限告警
	YueXianYx[0].DataId =YX_0201;// 0x201;
	YueXianYx[0].LimitParaP = &d_520A;
	YueXianYx[0].LimitType = SHANGXIAN;
	YueXianYx[0].SrcYcP = &YcData.ua1_4001 ;
	YueXianYx[0].Type = 1;
	// DataType1 d_0202;///<线路1 B相电压越上限告警
	YueXianYx[1].DataId = YX_0202;
	YueXianYx[1].LimitParaP = &d_520A;
	YueXianYx[1].LimitType = SHANGXIAN;
	YueXianYx[1].SrcYcP = &YcData.ub1_4002 ;
	YueXianYx[1].Type = 1 ;
	// DataType1 d_0203;///<线路1 C相电压越上限告警
	YueXianYx[2].DataId = YX_0203;
	YueXianYx[2].LimitParaP = &d_520A;
	YueXianYx[2].LimitType = SHANGXIAN;
	YueXianYx[2].SrcYcP = &YcData.uc1_4003 ;
	YueXianYx[2].Type = 1 ;
	// DataType1 d_0204;///<线路1 A相电压越下限告警
	YueXianYx[3].DataId = YX_0204;
	YueXianYx[3].LimitParaP = &d_520B;
	YueXianYx[3].LimitType = XIAXIAN;
	YueXianYx[3].SrcYcP = &YcData.ua1_4001 ;
	YueXianYx[3].Type = 1 ;
	// DataType1 d_0205;///<线路1 B相电压越下限告警
	YueXianYx[4].DataId = YX_0205;
	YueXianYx[4].LimitParaP = &d_520B;
	YueXianYx[4].LimitType = XIAXIAN;
	YueXianYx[4].SrcYcP = &YcData.ub1_4002 ;
	YueXianYx[4].Type = 1 ;
	// DataType1 d_0206;///<线路1 C相电压越下限告警
	YueXianYx[5].DataId = YX_0206;
	YueXianYx[5].LimitParaP = &d_520B;
	YueXianYx[5].LimitType = XIAXIAN;
	YueXianYx[5].SrcYcP = &YcData.uc1_4003 ;
	YueXianYx[5].Type = 1 ;
	// DataType1 d_0207;///<线路1 A相电流越上限告警
	YueXianYx[6].DataId = YX_0207;
	YueXianYx[6].LimitParaP = &d_520C;
	YueXianYx[6].LimitType = SHANGXIAN;
	YueXianYx[6].SrcYcP = &YcData.ia11_4007 ;
	YueXianYx[6].Type = 1 ;
	// DataType1 d_0208;///<线路1 B相电流越上限告警
	YueXianYx[7].DataId = YX_0208;
	YueXianYx[7].LimitParaP = &d_520C;
	YueXianYx[7].LimitType = SHANGXIAN;
	YueXianYx[7].SrcYcP = &YcData.ib11_4008 ;
	YueXianYx[7].Type = 1 ;
	// DataType1 d_0209;///<线路1 C相电流越上限告警
	YueXianYx[8].DataId = YX_0209;
	YueXianYx[8].LimitParaP = &d_520C;
	YueXianYx[8].LimitType = SHANGXIAN;
	YueXianYx[8].SrcYcP = &YcData.ic11_4009 ;
	YueXianYx[8].Type = 1 ;
	// DataType1 d_020A;///<线路1 负荷越上限告警
	YueXianYx[9].DataId = YX_020A;
	YueXianYx[9].LimitParaP = &d_520D;
	YueXianYx[9].LimitType = SHANGXIAN;
	YueXianYx[9].SrcYcP = &YcData.S1_4019 ;
	YueXianYx[9].Type = 1 ;
	// DataType1 d_020B;///<线路2 A相电压越上限告警
	YueXianYx[10].DataId = YX_020B;
	YueXianYx[10].LimitParaP = &d_520A;
	YueXianYx[10].LimitType = SHANGXIAN;
	YueXianYx[10].SrcYcP = &YcData.ua2_40FA ;
	YueXianYx[10].Type = 1 ;
	// DataType1 d_020C;///<线路2 B相电压越上限告警
	YueXianYx[11].DataId = YX_020C;
	YueXianYx[11].LimitParaP = &d_520A;
	YueXianYx[11].LimitType = SHANGXIAN;
	YueXianYx[11].SrcYcP = &YcData.ub2_40FB ;
	YueXianYx[11].Type = 1 ;
	// DataType1 d_020D;///<线路2 C相电压越上限告警
	YueXianYx[12].DataId = YX_020D;
	YueXianYx[12].LimitParaP = &d_520A;
	YueXianYx[12].LimitType = SHANGXIAN;
	YueXianYx[12].SrcYcP = &YcData.uc2_40FC ;
	YueXianYx[12].Type = 1 ;
	// DataType1 d_020E;///<线路2 A相电压越下限告警
	YueXianYx[13].DataId = YX_020E;
	YueXianYx[13].LimitParaP = &d_520B;
	YueXianYx[13].LimitType = XIAXIAN;
	YueXianYx[13].SrcYcP = &YcData.ua2_40FA ;
	YueXianYx[13].Type = 1 ;
	// DataType1 d_020F;///<线路2 B相电压越下限告警
	YueXianYx[14].DataId = YX_020F;
	YueXianYx[14].LimitParaP = &d_520B;
	YueXianYx[14].LimitType = XIAXIAN;
	YueXianYx[14].SrcYcP = &YcData.ub2_40FB ;
	YueXianYx[14].Type = 1 ;
	// DataType1 d_0210;///<线路2 C相电压越下限告警
	YueXianYx[15].DataId = YX_0210;
	YueXianYx[15].LimitParaP = &d_520B;
	YueXianYx[15].LimitType = XIAXIAN;
	YueXianYx[15].SrcYcP = &YcData.uc2_40FC ;
	YueXianYx[15].Type = 1 ;
	// DataType1 d_0211;///<线路2 A相电流越上限告警
	YueXianYx[16].DataId = YX_0211;
	YueXianYx[16].LimitParaP = &d_520C;
	YueXianYx[16].LimitType = SHANGXIAN;
	YueXianYx[16].SrcYcP = &YcData.ia21_4100 ;
	YueXianYx[16].Type = 1 ;
	// DataType1 d_0212;///<线路2 B相电流越上限告警
	YueXianYx[17].DataId = YX_0212;
	YueXianYx[17].LimitParaP = &d_520C;
	YueXianYx[17].LimitType = SHANGXIAN;
	YueXianYx[17].SrcYcP = &YcData.ib21_4101 ;
	YueXianYx[17].Type = 1 ;
	// DataType1 d_0213;///<线路2 C相电流越上限告警
	YueXianYx[18].DataId = YX_0213;
	YueXianYx[18].LimitParaP = &d_520C;
	YueXianYx[18].LimitType = SHANGXIAN;
	YueXianYx[18].SrcYcP = &YcData.ic21_4102 ;
	YueXianYx[18].Type = 1 ;
	// DataType1 d_0214;///<线路2 负荷越上限告警
	YueXianYx[19].DataId = YX_0214;
	YueXianYx[19].LimitParaP = &d_520D;
	YueXianYx[19].LimitType = SHANGXIAN;
	YueXianYx[19].SrcYcP = &YcData.S2_4112 ;
	YueXianYx[19].Type = 1 ;
	// DataType1 d_0215;///<温度越限
	YueXianYx[20].DataId = YX_0215;
	YueXianYx[20].LimitParaP = &TemLimit;
	YueXianYx[20].LimitType = SHANGXIAN;
	YueXianYx[20].SrcYcP = &YcData.tem_4606 ;
	YueXianYx[20].Type = 1 ;
	// DataType1 d_0216;///<湿度越限
	YueXianYx[21].DataId = YX_0216;
	YueXianYx[21].LimitParaP = &HumLimit;
	YueXianYx[21].LimitType = SHANGXIAN;
	YueXianYx[21].SrcYcP = &YcData.hum_4607 ;
	YueXianYx[21].Type = 1 ;
	return 1;
}

/*
*************************************************************************************
*  @brief  软要信检测断线，断相，越限告警；
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
#pragma CODE_SECTION("YXCODEINDDR2")
void  ChecksoftYx(void)
{
	uint16  i ;
	Data<BaseDataType> yxtmp;
	DataType1 data;
	CP56Time2a Time;
	for (i = 0; i < YUEXIANYXNUM; i++){
		if (YueXianYx[i].LimitType == SHANGXIAN){
			if(YueXianYx[i].SrcYcP->Data> YueXianYx[i].LimitParaP->Data.Limit){//越上线
				clk0.StopMsClock(&YueXianYx[i].Tcn[1]);
				if(clk0.CheckMsClock(&YueXianYx[i].Tcn[0])==1){
					data.Data = YxNatureChange(1,YueXianYx[i].DataId);
					Time = clk0.GetCP56Time2a();
					yxtmp.Data = &data;
					msg.SendMsg(TASK_VARM,MSG_YX,YueXianYx[i].DataId,&data,&Time);
					user_data.SetData(YueXianYx[i].DataId,&yxtmp);
				}else{
					clk0.StartMsClock(&YueXianYx[i].Tcn[0],YueXianYx[i].LimitParaP->Data.ContinueTime);
				}
			}else if(YueXianYx[i].SrcYcP->Data < YueXianYx[i].LimitParaP->Data.Recover){//越上线恢复
				clk0.StopMsClock(&YueXianYx[i].Tcn[0]);
				if(clk0.CheckMsClock(&YueXianYx[i].Tcn[1])==1){
					data.Data = YxNatureChange(0,YueXianYx[i].DataId);
					Time = clk0.GetCP56Time2a();
					yxtmp.Data = &data;
					msg.SendMsg(TASK_VARM,MSG_YX,YueXianYx[i].DataId,&data,&Time);
					user_data.SetData(YueXianYx[i].DataId,&yxtmp);
				}else{
					clk0.StartMsClock(&YueXianYx[i].Tcn[1],YueXianYx[i].LimitParaP->Data.ContinueTime);
				}
			}
		}else if(YueXianYx[i].LimitType == XIAXIAN){
			if(YueXianYx[i].SrcYcP->Data < YueXianYx[i].LimitParaP->Data.Limit){//越下线
				clk0.StopMsClock(&YueXianYx[i].Tcn[1]);
				if(clk0.CheckMsClock(&YueXianYx[i].Tcn[0])==1){
					data.Data = YxNatureChange(1,YueXianYx[i].DataId);
					Time = clk0.GetCP56Time2a();
					yxtmp.Data = &data;
					msg.SendMsg(TASK_VARM,MSG_YX,YueXianYx[i].DataId,&data,&Time);
					user_data.SetData(YueXianYx[i].DataId,&yxtmp);
				}else{
					clk0.StartMsClock(&YueXianYx[i].Tcn[0],YueXianYx[i].LimitParaP->Data.ContinueTime);
				}
			}else if(YueXianYx[i].SrcYcP->Data > YueXianYx[i].LimitParaP->Data.Recover){//越下线恢复
				clk0.StopMsClock(&YueXianYx[i].Tcn[0]);
				if(clk0.CheckMsClock(&YueXianYx[i].Tcn[1])==1){
					data.Data = YxNatureChange(0,YueXianYx[i].DataId);
					Time = clk0.GetCP56Time2a();
					yxtmp.Data = &data;
					msg.SendMsg(TASK_VARM,MSG_YX,YueXianYx[i].DataId,&data,&Time);
					user_data.SetData(YueXianYx[i].DataId,&yxtmp);
				}else{
					clk0.StartMsClock(&YueXianYx[i].Tcn[1],YueXianYx[i].LimitParaP->Data.ContinueTime);
				}
			}
		}
	}
}
#pragma CODE_SECTION("YXCODEINDDR2")
void YxInit(void)
{
   ReadYxFromFpga();
   memcpy(&YxNature,&YxCurrent,sizeof(YxNature));
   memset(&SoeBuffer,0,sizeof(SoeBuffer));
   YxCompassInt();
}
