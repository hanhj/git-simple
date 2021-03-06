/*
 * File Name: Configurable.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 11:40:42 week:5
*/
#include<iostream>
#include <string.h>
using namespace std;
#include "data.h"
#include "configurable.h"
#include "para.h"
#include "UserGpio.h"
#include "Scada.h"

_config_scada_data config_scada_data;
setmstime_t YcDeadTime[MAX_YC_NUM];
passdata  DeadPass[MAX_YC_NUM];//越过死区的值；
//上送标度化，归一化数据的时候放大数据的倍数
short coeftable[MAX_YC_NUM] = {
		10,//ua
		10,//ub
		10,//uc
		10,//u0
		1000,//ia
		1000,//ib
		1000,//ic
		1000,//i0
		10,//p
		10,//q
//10
		10,//s
		100,//cos
		100,//hz
};
ycdata_t Yc[MAX_YC_NUM];
/***************************************************************************************
* 函数名:void default_init_yc_addr(void)
* 输  入:
* 输  出:
* 功  能:ProPara.yc_dataaddr表出厂设置
* 编制人:郑飞
* 时  间:2011-6-24
* 备  注:出厂设置默认表
****************************************************************************************/
void default_init_yc_addr(void)
{	
	uint16 i;
	int j;
	j=0;
	for ( i = 0; j < CONFIG_YC_NUM; i++,j++){
		ProPara.yc_dataaddr.addpos[i].addr = i + 0x4001;
		ProPara.yc_dataaddr.addpos[i].pos = i;
	}
	j=0;
	for(;j<CONFIG_ENERGY_NUM;i++,j++){
		ProPara.yc_dataaddr.addpos[i].addr = j + 0x6401;
		ProPara.yc_dataaddr.addpos[i].pos = i;
	}
	for ( ; i < MAX_YC_NUM; i++){
		ProPara.yc_dataaddr.addpos[i].addr = 0xffff;
		ProPara.yc_dataaddr.addpos[i].pos = i;
	}
	ProPara.yc_dataaddr.num = i;
}


void init_ycdata ( void )
{
	uint16 i;
	i = 0;
	Yc[i].src_yc = &scada.scada_cfg.YcData.ua1_4001;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadU;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.ub1_4002;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadU;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.uc1_4003;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadU;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.u011_4006;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadU;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.ia11_4007;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadI;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.ib11_4008;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadI;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.ic11_4009;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadI;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.i011_400A;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadI;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;

	Yc[i].src_yc = &scada.scada_cfg.YcData.P1_4017;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadP;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.Q1_4018;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadP;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.S1_4019;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadP;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.COS1_401A;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadAngle;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	i++;

	Yc[i].src_yc = &scada.scada_cfg.YcData.freq1_401B;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadFreq;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;
	/*
	i++;
	Yc[i].src_yc = &fa.P0[0];
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = &coeftable[i];
	Yc[i].range = NULL;
	Yc[i].dead = &DeadBandParaValue.DeadP;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;	
	*/
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.pos_P;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = NULL;
	Yc[i].range = NULL;
	Yc[i].dead = NULL;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;	
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.neg_P;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = NULL;
	Yc[i].range = NULL;
	Yc[i].dead = NULL;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;	
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.pos_Q;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = NULL;
	Yc[i].range = NULL;
	Yc[i].dead = NULL;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;	
	i++;
	Yc[i].src_yc = &scada.scada_cfg.YcData.neg_Q;
	Yc[i].deadpass = &DeadPass[i];
	Yc[i].Coef = NULL;
	Yc[i].range = NULL;
	Yc[i].dead = NULL;
	Yc[i].addr = 0xffff;
	Yc[i].changeflag = 0;	
	memset(&scada.scada_cfg.YcData,0,sizeof(scada.scada_cfg.YcData));
}
/***************************************************************************************
* 函数名:unsigned int init_yc_table ( void )
* 输  入:
* 输  出:
* 功  能:YcTable[]表初始化，
* 编制人:郑飞
* 时  间:2011-6-24
* 备  注:必须在ProPara.yc_dataaddr表初始化后 也就是每次对ProPara.yc_dataaddr改变
或者系统重启时 需要对对ProPara.yc_dataaddr赋值 并同时初始YcTable[]表;
****************************************************************************************/
uint16 init_yc_table ( void )
{
	uint16 i = 0 ;
	int j;
	int m;
	uint16 pos = 0;
	uint16 addr = 0xffff;
	config_scada_data.SelectYcNum = 0;
	config_scada_data.SelectAccNum = 0;
	m=0;
	j=0;
    for ( i = 0; j < CONFIG_YC_NUM ;i++ ,j++) {
    	pos =ProPara.yc_dataaddr.addpos[i].pos;
    	addr =ProPara.yc_dataaddr.addpos[i].addr ;
		if ((addr != 0xffff ) && (addr <0x6401) ){
			Yc[pos].addr = addr;
			YcTable[m]= &Yc[pos];
			m++;
			config_scada_data.SelectYcNum ++;
		}
	}
	config_scada_data.pos_acc=m;
	j=0;
    for ( ; j < CONFIG_ENERGY_NUM ;j++,i++ ) {
    	pos =ProPara.yc_dataaddr.addpos[i].pos;
    	addr =ProPara.yc_dataaddr.addpos[i].addr ;
		if ( addr != 0xffff && addr>=0x6401 ){
			Yc[pos].addr = addr;
			YcTable[m++] = &Yc[pos];
			config_scada_data.SelectAccNum++;
		}
	}
  return  config_scada_data.SelectYcNum ;
}

/***************************************************************************************
* 函数名:void init_yc(void)
* 输  入:
* 输  出:
* 功  能:YcTable[]表初始化，
* 编制人:郑飞
* 时  间:2011-6-24
* 备  注:必须在ProPara.yc_dataaddr表初始化后 也就是每次对ProPara.yc_dataaddr改变
或者系统重启时 需要对对ProPara.yc_dataaddr赋值 并同时初始YcTable[]表;
1:开机初始化调用，
2：系统恢复出厂设置调用
3：ProPara.yc_dataaddr表重新设置调用
****************************************************************************************/
int16 init_yc(void)
{
	CoefTableInit();
	init_ycdata();
	init_yc_table();
	return 1;
}

///////////////////////////////////遥信数据可配置////////////////////////////////////////
/***************************************************************************************
* 函数名:void default_init_yx_addr(void)
* 输  入:
* 输  出:
* 功  能:ProPara.yx_dataaddr_pos表出厂设置
* 编制人:郑飞
* 时  间:2011-6-24
* 备  注:出厂设置默认表
****************************************************************************************/
void default_init_yx_addr(void)
{	
 uint16 i;
	for ( i = 0;i < MAX_YX_NUM ;i++ ) {
		ProPara.yx_dataaddr.addpos[i].addr= i + 1;
		ProPara.yx_dataaddr.addpos[i].dataid= i ;
		ProPara.yx_dataaddr.addpos[i].attrb= 0;//0开1合
	}
	ProPara.yx_dataaddr.num=i;
}

void InitYxTable( void )
{
	Uint16 i = 0,l,m;
	for ( i = 0;i < MAX_YX_NUM ;i++ ) {
		YxTable[i]=NULL;
	}
	i=0;
	//STEP1 将所有的软硬件的遥信按照固定的顺序注册到表格中，这个顺序和上位机遥信表是遥信的意义顺序是一致的
	for (m = 0; m < 6; m++){
		YxTable[i] = (yxdata_t*)&TerYx.Hyx[m];
		YxTable[i]->priority = 1;
		  i++;
	}
	for ( l= 0;l<MAX_LINE_NUM;l++){
		 for(m=0;m<3;m++){
	 	   YxTable[i] = (yxdata_t*)&TerYx.CurrentAlarmYx[l][m];
	 	   YxTable[i]->priority = 1;
	 	   i++;
	 	 }
	 	 for(m=0;m<3;m++){
	 	   YxTable[i] = (yxdata_t*)&TerYx.GrdAlarmYx[l][m];
	 	   YxTable[i]->priority = 1;
	 	   i++;
	 	 }
		YxTable[i] = (yxdata_t*)&TerYx.U0Alarm[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.JyYx[l];
		YxTable[i]->priority = 1;
		i++;
		 for(m=0;m<3;m++){
	 	     YxTable[i] = (yxdata_t*)&TerYx.CurrentActionYx[l][m];
	 	     YxTable[i]->priority = 1;
	 	     i++;
	 	 }
	 	 for(m=0;m<3;m++){
	 	     YxTable[i] = (yxdata_t*)&TerYx.GrdActionYx[l][m];
	 	     YxTable[i]->priority = 1;
	 	     i++;
	 	 }
		YxTable[i] = (yxdata_t*)&TerYx.U0Action[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.JyActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.P0Yx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.P0ActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.JyP0Yx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.JyP0ActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.ChzYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.HjsActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.pt_yx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.ct_yx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.gzfg[l][0];//瞬时
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.gzfg[l][1];//永久
		YxTable[i]->priority = 1;
		i++;
		for (m = 6;m < 8; m++){//电源管理的2个遥信
			YxTable[i] = (yxdata_t*)&TerYx.Hyx[m];
			YxTable[i]->priority = 1;
			i++;
		}
		YxTable[i] = (yxdata_t*)&TerYx.TerminalCkYx;//装置故障
		YxTable[i]->priority = 1;
		i++;
		for(m=0;m<10;m++){
		    YxTable[i] = (yxdata_t*)&TerYx.YueXianYx[l][m];
		    YxTable[i]->priority = 2;
		    i++;
		}
		YxTable[i] = (yxdata_t*)&TerYx.AngleYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.AngleActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.RaiseYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.RaiseActionYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.reverLineYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.reverXuYx[l];
		YxTable[i]->priority = 1;
		i++;
		YxTable[i] = (yxdata_t*)&TerYx.Hyx[10];
		YxTable[i]->priority = 1;
	}
	for ( i = 0;i < MAX_YX_NUM ;i++ ) {
		if(YxTable[i]!=NULL){
			YxTable[i]->id = ProPara.yx_dataaddr.addpos[i].dataid;
			YxTable[i]->addr = ProPara.yx_dataaddr.addpos[i].addr;
			YxTable[i]->nature= ProPara.yx_dataaddr.addpos[i].attrb;
			if (YxTable[i]->id==0x33){//双点遥信在配置表中的位置，配置表更改时注意变化(是数据id 不是addr,遥信数据表xls中addr-1)
				YxTable[i]->type = D_YX;

			}else{
				YxTable[i]->type = S_YX;
			}
		}
	}
}

void  InitSortYxTable ( void )
{
	uint16 i;
    uint16 addr;
	uint16 m=0;
    uint16 k = 0;
	uint16 j = 0;
    for ( addr = 0x0001;addr < MAX_YX_NUM+1;addr++ ) {
        for ( i = 0;i< MAX_YX_NUM && YxTable[i]!=NULL;i++ ) {
            if ( addr == YxTable[i]->addr ) {
				m = YxTable[i]->type;
				if(m==S_YX){
                	SortYxTable[k] = YxTable[i];
					k++;
				}
                break;
            }
        }
    }
    for ( addr = 0x0001;addr < MAX_YX_NUM+1;addr++ ) {
        for ( i = 0;i < MAX_YX_NUM&& YxTable[i]!=NULL;i++ ) {
            if ( addr ==  YxTable[i]->addr ) {
				if(YxTable[i]->type==D_YX){
					SortYxTable[j+k] = YxTable[i];
					j++;
				}
                break;
            }
        }
    }
    if ( k != 0 ) {
    	config_scada_data.SelectYxNum = ( k );
    	config_scada_data.SelectDpYxNum = ( j + k );
    }
}
/* 在本项目中Hyx定义为
0 	合位
1 	分位
2 	储能位
3 	远方就地位
4 	手动合闸
5     手动分闸
6	VL
7	POK
8	VH
9	HOK
10 双点遥信
*/
Uint16  YxStartStatu (void)
{
	Uint16 i;
	yxdata_t * p_yx;
	p_yx = ( yxdata_t * )&TerYx;
	
	memset(&TerYx,0,sizeof(TerYx));
	for ( i = 0 ;i< sizeof(TerYx)/sizeof(yxdata_t); i++ ){
		( p_yx + i )->addr = 0xffff;
	}
	TerYx.Hyx[0].statu =( ~YX1 ) & 1;
	TerYx.Hyx[1].statu =( ~YX2 ) & 1;
	TerYx.Hyx[2].statu =( ~YX3 ) & 1;
	TerYx.Hyx[3].statu =( ~YX4 ) & 1;
	TerYx.Hyx[4].statu= ( ~YX5 ) & 1;
	TerYx.Hyx[5].statu =( ~YX6 ) & 1;
	TerYx.Hyx[10].statu= (TerYx.Hyx[0].statu<<1) +	(TerYx.Hyx[1].statu) ;
	
	TerYx.Hyx[6].statu= ( ~YX7 )  & 1;
	TerYx.Hyx[7].statu= (~ YX8 ) & 1;
	TerYx.Hyx[8].statu= ( ~YX9 )  & 1;
	TerYx.Hyx[9].statu = ( ~YX10 ) & 1;
	
//mistake???	
	/*
	for ( i =0 ;i<MAXHYXNUM ;i++){
		if(TerYx.Hyx[i].nature == _0_HE_1_KAI){
			TerYx.Hyx[i].statu =((~TerYx.Hyx[i].statu) &0x0001);
		}
	}*/


	return 0;
}

/***************************************************************************************
* 函数名:init_yx()
* 输  入:
* 输  出:
* 功  能: Yx[]表初始化， YxTable[]表初始化，
* 编制人:郑飞
* 时  间:2011-6-24
* 备  注:1开机时调用，2出厂设置时调用，3重新设置ProPara.yx_dataaddr_pos表时调用，
****************************************************************************************/
int16 init_yx ( void )
{
	YxStartStatu ();
	InitYxTable();
	InitSortYxTable ();
	return 0;
}
//当pt,ct变比被设置时的调用
//涉及的对象coftable ,影响上送的遥测值，保护定值，
int16 CoefTableInit(void ){
	scada.scada_cfg.pt 	= LimitBounds.pt_one_set/LimitBounds.pt_two_set*1000;//pt一次值单位是KV
	scada.scada_cfg.ct 	= LimitBounds.ct_one_set/LimitBounds.ct_two_set;
	scada.scada_cfg.ct0 = LimitBounds.ct0_one_set/LimitBounds.ct0_two_set;	
	scada.scada_cfg.pt0 = LimitBounds.pt0_one_set/LimitBounds.pt0_two_set*1000;

	if(LimitBounds.U0_source == 1){
		scada.scada_cfg.pt0 = scada.scada_cfg.pt;
	}
	if(LimitBounds.I0_source == 1){
		scada.scada_cfg.ct0 = scada.scada_cfg.ct;
	}

	coeftable[0] = scada.scada_cfg.pt;
	coeftable[1] = scada.scada_cfg.pt;
	coeftable[2] = scada.scada_cfg.pt;
	coeftable[3] = scada.scada_cfg.pt0;
	coeftable[4] = scada.scada_cfg.ct;
	coeftable[5] = scada.scada_cfg.ct;
	coeftable[6] = scada.scada_cfg.ct;
	coeftable[7] = scada.scada_cfg.ct0;
	coeftable[8] = scada.scada_cfg.pt*scada.scada_cfg.ct/1000;
	coeftable[9] = scada.scada_cfg.pt*scada.scada_cfg.ct/1000;
	coeftable[10] = scada.scada_cfg.pt*scada.scada_cfg.ct/1000;
	coeftable[11] = 1;
	coeftable[12] = 1;
//	coeftable[13] =scada.scada_cfg.pt*scada.scada_cfg.ct0/1000;
	//LimitInit();
	return 1;
}
// vim:tw=72 
