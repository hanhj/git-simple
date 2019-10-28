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

unsigned int SelectYxNum;
unsigned int SelectDpYxNum;
unsigned int SelectYcNum;

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
	for ( i = 0;i < TOTALYXNUM ;i++ ) {
		ProPara.yx_dataaddr.addpos[i].addr= i + 1;
		ProPara.yx_dataaddr.addpos[i].dataid= i ;
		ProPara.yx_dataaddr.addpos[i].attrb= 0;//0开1合
	}
}

void InitYxTable( void )
{
	Uint16 i = 0,l,m;
	for ( i = 0;i < TOTALYXNUM ;i++ ) {
		YxTable[i].pdata=NULL;
	}
	i=0;
	//STEP1 将所有的软硬件的遥信按照固定的顺序注册到表格中，这个顺序和上位机遥信表是遥信的意义顺序是一致的
	for (m = 0; m < 6; m++){
		YxTable[i].pdata = (yxdata_t*)&TerYx.Hyx[m];
		YxTable[i].pdata->priority = 1;
		  i++;
	}
	for ( l= 0;l<MAX_LINE_NUM;l++){
		 for(m=0;m<3;m++){
	 	   YxTable[i].pdata = (yxdata_t*)&TerYx.CurrentAlarmYx[l][m];
	 	   YxTable[i].pdata->priority = 1;
	 	   i++;
	 	 }
	 	 for(m=0;m<3;m++){
	 	   YxTable[i].pdata = (yxdata_t*)&TerYx.GrdAlarmYx[l][m];
	 	   YxTable[i].pdata->priority = 1;
	 	   i++;
	 	 }
		YxTable[i].pdata = (yxdata_t*)&TerYx.U0Alarm[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.JyYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		 for(m=0;m<3;m++){
	 	     YxTable[i].pdata = (yxdata_t*)&TerYx.CurrentActionYx[l][m];
	 	     YxTable[i].pdata->priority = 1;
	 	     i++;
	 	 }
	 	 for(m=0;m<3;m++){
	 	     YxTable[i].pdata = (yxdata_t*)&TerYx.GrdActionYx[l][m];
	 	     YxTable[i].pdata->priority = 1;
	 	     i++;
	 	 }
		YxTable[i].pdata = (yxdata_t*)&TerYx.U0Action[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.JyActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.P0Yx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.P0ActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.JyP0Yx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.JyP0ActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.ChzYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.HjsActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.pt_yx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.ct_yx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.gzfg[l][0];//瞬时
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.gzfg[l][1];//永久
		YxTable[i].pdata->priority = 1;
		i++;
		for (m = 6;m < 8; m++){//电源管理的2个遥信
			YxTable[i].pdata = (yxdata_t*)&TerYx.Hyx[m];
			YxTable[i].pdata->priority = 1;
			i++;
		}
		YxTable[i].pdata = (yxdata_t*)&TerYx.TerminalCkYx;//装置故障
		YxTable[i].pdata->priority = 1;
		i++;
		for(m=0;m<10;m++){
		    YxTable[i].pdata = (yxdata_t*)&TerYx.YueXianYx[l][m];
		    YxTable[i].pdata->priority = 2;
		    i++;
		}
		YxTable[i].pdata = (yxdata_t*)&TerYx.AngleYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.AngleActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.RaiseYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.RaiseActionYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.reverLineYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.reverXuYx[l];
		YxTable[i].pdata->priority = 1;
		i++;
		YxTable[i].pdata = (yxdata_t*)&TerYx.Hyx[10];
		YxTable[i].pdata->priority = 1;
	}
	for ( i = 0;i < TOTALYXNUM ;i++ ) {
		if(YxTable[i].pdata!=NULL){
			YxTable[i].pdata->id = ProPara.yx_dataaddr.addpos[i].dataid;
			YxTable[i].pdata->addr = ProPara.yx_dataaddr.addpos[i].addr;
			YxTable[i].pdata->nature= ProPara.yx_dataaddr.addpos[i].attrb;
			if (YxTable[i].pdata->id==0x33){//双点遥信在配置表中的位置，配置表更改时注意变化(是数据id 不是addr,遥信数据表xls中addr-1)
				YxTable[i].pdata->type = D_YX;

			}else{
				YxTable[i].pdata->type = S_YX;
			}
		}else{
			YxTable[i].pdata=NULL;
		}
	}
}

void  InitSortYxTable ( void )
{

	uint16 postemp;
    uint16 datasign;
	//uint16 n=0;
	uint16 m=0;
   // uint16 i;
    uint16 k = 0;
	uint16 j = 0;
	uint16 number_yx;
	number_yx=sizeof(TerYx)/sizeof(yxdata_t);
    for ( datasign = 0x0001;datasign < number_yx+1;datasign++ ) {
        for ( postemp = 0;postemp< number_yx;postemp++ ) {
            if ( datasign == YxTable[postemp].pdata->addr ) {
				m = YxTable[postemp].pdata->type;
				if(m==S_YX){
                	SortYxTable[k].pdata = YxTable[postemp].pdata;
					k++;
				}
                break;
            }
        }
    }
    for ( datasign = 0x0001;datasign < number_yx+1;datasign++ ) {
        for ( postemp = 0;postemp < number_yx;postemp++ ) {
            if ( datasign ==  YxTable[postemp].pdata->addr ) {
				if(YxTable[postemp].pdata->type==D_YX){
					SortYxTable[j+k].pdata = YxTable[postemp].pdata;
					j++;
				}
                break;
            }
        }
    }
    if ( k == 0 ) {
     //   return -1;
    } else {
    	SelectYxNum = ( k );
    	SelectDpYxNum = ( j );

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
	TerYx.Hyx[10].statu= (TerYx.Hyx[0].statu<<1) +	TerYx.Hyx[1].statu ;
	
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
// vim:tw=72 
