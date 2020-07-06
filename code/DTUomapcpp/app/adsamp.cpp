/*
 * ad_samp.cpp
 *
 *  Created on: 2013-8-8
 *      Author: Administrator
 */
#include <stdio.h>
#include <string.h>
#include "product.h"
#include "adsamp.h"
#include "mathlist.h"
#include "datatype.h"
#include "types.h"
#include "message.h"
#include "scada.h"
#include "interruptapp.h"
#include "fpga.h"
#include "gpio.h"
#include "fa_fft_cal.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "dspdatatype.h"
#include "clock.h"
#include "utils.h"
#include "edma3_interface.h"

//for faster set data into DATAINIRAM region
//#pragma DATA_SECTION("DATAINIRAM");
//origional ad data
#pragma DATA_ALIGN(8);
#pragma DATA_SECTION("DATAINDDR2");
int16 AD_BUF_ori[CHANNEL_NUM][MAXN] = {0};
#pragma DATA_ALIGN(8);

//frozen ad data
#pragma DATA_SECTION("DATAINDDR2");
int16 AD_BUF_scada[CHANNEL_NUM][MAXN] = {0};
#if ENABLE_FA
#pragma DATA_SECTION("DATAINDDR2");
int16 AD_BUF_fa[CHANNEL_NUM][MAXN] = {0};
#endif

//complex ad data for calculate 
#pragma DATA_SECTION("DATAINDDR2");
#pragma DATA_ALIGN(8);
SP_COMPLEX AD_BUF_scada_cal[CHANNEL_NUM][SAMP_NUM] = {0};
#if ENABLE_FA
#pragma DATA_SECTION("DATAINDDR2");
#pragma DATA_ALIGN(8);
SP_COMPLEX AD_BUF_fa_cal[CHANNEL_NUM][FA_SAMP_NUM] = {0};
#endif


#pragma DATA_SECTION("DATAINIRAM");
#pragma DATA_ALIGN(8);

unsigned int ad_point=0;
int need_calculate=0;
int stop=0;
/*
 * 遥测1板:AD7606_1:0x60000300,AD7606_2:0x60000310
 * 遥测2板:AD7606_1:0x60000320,AD7606_2:0x60000330
 * 遥测3板:AD7606_1:0x60000340,AD7606_2:0x60000350
 * 遥测4板:AD7606_1:0x60000360,AD7606_2:0x60000370
 * 遥测5板:AD7606_1:0x60000380,AD7606_2:0x60000390
 */
#pragma DATA_SECTION("YCDATA");
int16 ADREG[CHANNEL_NUM];
uint8 StartAdFlag = 0;

void AdReset(void)//RESET high pulse width min 50 ns
{
    //复位AD
    AD_RST_0;
    DelayUs(1000);
    AD_RST_1;
    DelayUs(50);
    AD_RST_0;
}
/*Ad中断响应*/
#pragma CODE_SECTION("CODEINL1PSRAM")
Void HwiAdFinFxn(UArg arg)
{
	asm ( " DINT" );
	Swi_post(SwiGetAdHandle);
}

#pragma CODE_SECTION("CODEINL1PSRAM")
Void SwiGetAdFxn(UArg arg1, UArg arg2)
{

	time1 =  TMR2->TIM12;
	time1 =  TMR2->TIM34;
	if(scada.scada_cfg.d_5213.Data==1){//人工置数
		return ;
	}
	if(YC_SLOT1_STATU==IN_STATU){
	//AD7606_1
		AD_BUF_ori[7][ad_point] = ADREG[0];
		AD_BUF_ori[6][ad_point] = ADREG[1];
		AD_BUF_ori[5][ad_point] = ADREG[2];
		AD_BUF_ori[4][ad_point] = ADREG[3];
		AD_BUF_ori[3][ad_point] = ADREG[4];
		AD_BUF_ori[2][ad_point] = ADREG[5];
		AD_BUF_ori[1][ad_point] = ADREG[6];
		AD_BUF_ori[0][ad_point] = ADREG[7];
	//AD7606_2
		AD_BUF_ori[15][ad_point] = ADREG[8];
		AD_BUF_ori[14][ad_point] = ADREG[9];
		AD_BUF_ori[13][ad_point] = ADREG[10];
		AD_BUF_ori[12][ad_point] = ADREG[11];
		AD_BUF_ori[11][ad_point] = ADREG[12];
		AD_BUF_ori[10][ad_point] = ADREG[13];
		AD_BUF_ori[9][ad_point] = ADREG[14];
		AD_BUF_ori[8][ad_point] = ADREG[15];
	}
	if(YC_SLOT2_STATU==IN_STATU){
	//AD7606_3
		AD_BUF_ori[23][ad_point] = ADREG[16];
		AD_BUF_ori[22][ad_point] = ADREG[17];
		AD_BUF_ori[21][ad_point] = ADREG[18];
		AD_BUF_ori[20][ad_point] = ADREG[19];
		AD_BUF_ori[19][ad_point] = ADREG[20];
		AD_BUF_ori[18][ad_point] = ADREG[21];
		AD_BUF_ori[17][ad_point] = ADREG[22];
		AD_BUF_ori[16][ad_point] = ADREG[23];
	//AD7606_4
		AD_BUF_ori[31][ad_point] = ADREG[24];
		AD_BUF_ori[30][ad_point] = ADREG[25];
		AD_BUF_ori[29][ad_point] = ADREG[26];
		AD_BUF_ori[28][ad_point] = ADREG[27];
		AD_BUF_ori[27][ad_point] = ADREG[28];
		AD_BUF_ori[26][ad_point] = ADREG[29];
		AD_BUF_ori[25][ad_point] = ADREG[30];
		AD_BUF_ori[24][ad_point] = ADREG[31];
	}
	if(YC_SLOT3_STATU==IN_STATU){
	//AD7606_5
		AD_BUF_ori[39][ad_point] = ADREG[32];
		AD_BUF_ori[38][ad_point] = ADREG[33];
		AD_BUF_ori[37][ad_point] = ADREG[34];
		AD_BUF_ori[36][ad_point] = ADREG[35];
		AD_BUF_ori[35][ad_point] = ADREG[36];
		AD_BUF_ori[34][ad_point] = ADREG[37];
		AD_BUF_ori[33][ad_point] = ADREG[38];
		AD_BUF_ori[32][ad_point] = ADREG[39];
	//AD7606_6
		AD_BUF_ori[47][ad_point] = ADREG[40];
		AD_BUF_ori[46][ad_point] = ADREG[41];
		AD_BUF_ori[45][ad_point] = ADREG[42];
		AD_BUF_ori[44][ad_point] = ADREG[43];
		AD_BUF_ori[43][ad_point] = ADREG[44];
		AD_BUF_ori[42][ad_point] = ADREG[45];
		AD_BUF_ori[41][ad_point] = ADREG[46];
		AD_BUF_ori[40][ad_point] = ADREG[47];
	}
	if(YC_SLOT4_STATU==IN_STATU){
	//AD7606_7
		AD_BUF_ori[55][ad_point] = ADREG[48];
		AD_BUF_ori[54][ad_point] = ADREG[49];
		AD_BUF_ori[53][ad_point] = ADREG[50];
		AD_BUF_ori[52][ad_point] = ADREG[51];
		AD_BUF_ori[51][ad_point] = ADREG[52];
		AD_BUF_ori[50][ad_point] = ADREG[53];
		AD_BUF_ori[49][ad_point] = ADREG[54];
		AD_BUF_ori[48][ad_point] = ADREG[55];
	//AD7606_8
		AD_BUF_ori[63][ad_point] = ADREG[56];
		AD_BUF_ori[62][ad_point] = ADREG[57];
		AD_BUF_ori[61][ad_point] = ADREG[58];
		AD_BUF_ori[60][ad_point] = ADREG[59];
		AD_BUF_ori[59][ad_point] = ADREG[60];
		AD_BUF_ori[58][ad_point] = ADREG[61];
		AD_BUF_ori[57][ad_point] = ADREG[62];
		AD_BUF_ori[56][ad_point] = ADREG[63];
	}
	if(YC_SLOT5_STATU==IN_STATU){
	//AD7606_9
		AD_BUF_ori[71][ad_point] = ADREG[64];
		AD_BUF_ori[70][ad_point] = ADREG[65];
		AD_BUF_ori[69][ad_point] = ADREG[66];
		AD_BUF_ori[68][ad_point] = ADREG[67];
		AD_BUF_ori[67][ad_point] = ADREG[68];
		AD_BUF_ori[66][ad_point] = ADREG[69];
		AD_BUF_ori[65][ad_point] = ADREG[70];
		AD_BUF_ori[64][ad_point] = ADREG[71];
	//AD7606_10
		AD_BUF_ori[79][ad_point] = ADREG[72];
		AD_BUF_ori[78][ad_point] = ADREG[73];
		AD_BUF_ori[77][ad_point] = ADREG[74];
		AD_BUF_ori[76][ad_point] = ADREG[75];
		AD_BUF_ori[75][ad_point] = ADREG[76];
		AD_BUF_ori[74][ad_point] = ADREG[77];
		AD_BUF_ori[73][ad_point] = ADREG[78];
		AD_BUF_ori[72][ad_point] = ADREG[79];
	}
#if 0
	time2 =  TMR2->TIM12;
	time2 =  TMR2->TIM34;
	time3 =  time2 -time1;
#endif 
	ad_point++;
	if(ad_point%Quarter_NUM==0){//	1/4 period  calculate fa
		if(!stop){
			set_paraRamSet(0,1,(unsigned char *)&AD_BUF_ori[0][0],(unsigned char *)&AD_BUF_fa[0][0],(MAXN)*2,CHANNEL_NUM,1,1);
			trigger_dma(0,1);
		}
	}else if (ad_point>=MAXN){//将AD_BUF_ori拷贝到AD_BUCP以防止后续采样破坏数据,采样两个周期数据，主要是为计算频率用。
		ad_point = 0;
#if 0
time1 =  TMR2->TIM12;
time1 =  TMR2->TIM34;
//45us
#endif
		if(!stop){
			set_paraRamSet(0,0,(unsigned char *)&AD_BUF_ori[0][0],(unsigned char *)&AD_BUF_scada[0][0],(MAXN)*2,CHANNEL_NUM,1,1);
			trigger_dma(0,0);
			//memcpy(&AD_BUF_scada,&AD_BUF_ori,7680);
		}
#if 0
time2 =  TMR2->TIM12;
time2 =  TMR2->TIM34;
time3 =  time2 -time1;
#endif 
	}
}
/*拉格朗日三点插值又称为抛物线插值法
y= y1*(x-x2)(x-x3)/(x1-x2)(x1-x3)  + y2*(x-x1)(x-x3)/(x2-x1)(x2-x3) +y3*(x-x1)(x-x2)/(x3-x1(x3-x2)
其中，(x1,y1)、(x2,y2)、(x3,y3)为三个样点坐标,(x,y)为要获得的点

*/
//count 为插值的点数
float Lagrange(float x, int16 count, SData d[])
{
    float y=0.0;
    float p=1.0;//初始化p
    int16_t k = 0;
    int16_t j =0;
    for( k=0; k<count; k++)//这儿默认为count－1次插值
    {
       p = 1.0;
       for(j=0; j<count; j++)
       {//计算p的值
          if(k==j)continue;//判断是否为同一个数
          p=p*(x-d[j].x)/(d[k].x-d[j].x);
       }
       y=y+p*d[k].y;//求和
    }
    return y;//返回y的值
}

//*********采样数据修正****************
//这里假设AD每次采样完成所需的时间是一致的、理想的，
void SampleDataModify(int16* pWaveCode,float Freq,SP_COMPLEX *piRetValue)
{
   int16 i;
   int16 j;
   int16 iCurIndex;
   float fADSampleInterval; //AD采样的时间间隔
   float fFFtNeedInterval; //当前频率dwFreq， 所对应的FFT间隔。
   float fCurTime;
   float ftemp;
   int16_t itemp;
    //例如，当前频率50HZ,NUM_FFT＝128（2个周波），AD采样200点。最好的采样率应为64*50=3200Hz,则fCurInterval＝1/3200 = 0.3125ms
	//这里采用拉格朗日三点插值，把200个采样点，插值为128个点，则插值后的采样频率为3200Hz。然后再进行FFT运算，达到了频率的跟踪目的
   SData sData[3];
   int16_t* iValue;
   iValue=pWaveCode;
   fADSampleInterval = (float)1.0/(float)3200.0; //0.3125ms
   fFFtNeedInterval =  (float)1.0 / ((float)3200.0 * (float)Freq /(float)5000); //因50Hz时插值后的采样率应为3200Hz，那么若当前频率为55Hz则插值后的采样率 ＝ 3200*55/50; 而50Hz对应AD频率寄存器的0x19999。

   piRetValue[0].rel = iValue[0]; //第一个点取 iValue的第二个点，iValue的第一个点为可能的第一次插值用，也可能用不到，依据当前的实时频率，见下面的语句
   fCurTime = fFFtNeedInterval;
   j=1;
   for(i=1; i<SAMP_NUM; i++) //fft的点数
   {
	   iCurIndex = (int16_t)(fCurTime / fADSampleInterval); //因第一个取的是iValue的第二个点，所以以后的要都后推
	   if(fCurTime < ( (float)iCurIndex*fADSampleInterval + fADSampleInterval/(float)2.0)) //本次FFT插值的时间点，位于当前iCurIndexAD采样时间点与iCurIndex＋1采样点的前半部的时间
       iCurIndex--;  //
	   sData[0].x = (float)((float)iCurIndex * fADSampleInterval);
	   sData[1].x = (float)(iCurIndex + 1) * (fADSampleInterval);
	   sData[2].x = (float)(iCurIndex + 2)* (fADSampleInterval);
	   sData[0].y = (float)iValue[iCurIndex];
	   sData[1].y = (float)iValue[iCurIndex+1];
	   sData[2].y = (float)iValue[iCurIndex+2];
	   ftemp = Lagrange(fCurTime, 3,sData);
	   itemp = ftemp;
	   piRetValue[j++].rel = itemp;
       fCurTime +=fFFtNeedInterval;
   }
}
//将AD_BUF_scada中的数据拷贝到复数数组AD_BUF_scada_cal中,被Scada类的计算函数调用
#pragma CODE_SECTION("CODEINIRAM")
void AdCopy(void)
{
	uint8 i;
	uint16 channel;
	uint16 k;
	int16 freq;
	int16 diff_freq;
	int yc_id;
	int line;
	int std_freq;
	std_freq=STD_FREQ;
 	for(k = 0; k < scada.yc_config.configed_yc_port_num; k++){
 		channel = scada.yc_config.configed_yc_port[k];
		if(channel==NO_YC)//没有配置该通道，则跳过
			continue;
		yc_id=scada.find_yc_by_channel(channel);//该通道关联的yc
		line=GetLineNoByYcId(yc_id);//yc所在的线路号
		if(yc_id!=-1){
			freq=scada.GetFreq(line);
 		}else{//如果没有找到关联的yc，则采用线路1的频率
 			freq=scada.GetFreq(0);
 		}
		if(freq>std_freq){
			diff_freq = freq - std_freq;
		}else{
			diff_freq = std_freq - freq;
		}
		if (diff_freq>10){//频率与采样频率相差0.1HZ 去调整，其他不去调整
			for (i = 0; i< SAMP_NUM;i++){
				AD_BUF_scada_cal[channel][i].img =0;
			}
			SampleDataModify((int16 *)&AD_BUF_scada[channel],freq,(SP_COMPLEX *)&AD_BUF_scada_cal[channel]);
		}else{
			for (i = 0; i < SAMP_NUM; i++){
				AD_BUF_scada_cal[channel][i].img =0;
				AD_BUF_scada_cal[channel][i].rel = AD_BUF_scada[channel][i];
			}
		}
	}
}
#if ENABLE_FA
#pragma CODE_SECTION("CODEINIRAM")
void FaAdCopy(void)
{
	unsigned int channel;
	int i;
 	for(i = 0; i < scada.yc_config.configed_yc_port_num; i++){
		channel = scada.yc_config.configed_yc_port[i];
		if(channel!=NO_YC){
			{
				AD_BUF_fa_cal[channel][0].rel = AD_BUF_fa[channel][0];
				AD_BUF_fa_cal[channel][1].rel = AD_BUF_fa[channel][2];
				AD_BUF_fa_cal[channel][2].rel = AD_BUF_fa[channel][4];
				AD_BUF_fa_cal[channel][3].rel = AD_BUF_fa[channel][6];
				AD_BUF_fa_cal[channel][4].rel = AD_BUF_fa[channel][8];
				AD_BUF_fa_cal[channel][5].rel = AD_BUF_fa[channel][10];
				AD_BUF_fa_cal[channel][6].rel = AD_BUF_fa[channel][12];
				AD_BUF_fa_cal[channel][7].rel = AD_BUF_fa[channel][14];
				AD_BUF_fa_cal[channel][8].rel = AD_BUF_fa[channel][16];
				AD_BUF_fa_cal[channel][9].rel = AD_BUF_fa[channel][18];
				AD_BUF_fa_cal[channel][10].rel = AD_BUF_fa[channel][20];
				AD_BUF_fa_cal[channel][11].rel = AD_BUF_fa[channel][22];
				AD_BUF_fa_cal[channel][12].rel = AD_BUF_fa[channel][24];
				AD_BUF_fa_cal[channel][13].rel = AD_BUF_fa[channel][26];
				AD_BUF_fa_cal[channel][14].rel = AD_BUF_fa[channel][28];
				AD_BUF_fa_cal[channel][15].rel = AD_BUF_fa[channel][30];
				AD_BUF_fa_cal[channel][0].img = 0;
				AD_BUF_fa_cal[channel][1].img = 0;
				AD_BUF_fa_cal[channel][2].img = 0;
				AD_BUF_fa_cal[channel][3].img = 0;
				AD_BUF_fa_cal[channel][4].img = 0;
				AD_BUF_fa_cal[channel][5].img = 0;
				AD_BUF_fa_cal[channel][6].img = 0;
				AD_BUF_fa_cal[channel][7].img = 0;
				AD_BUF_fa_cal[channel][8].img = 0;
				AD_BUF_fa_cal[channel][9].img = 0;
				AD_BUF_fa_cal[channel][10].img = 0;
				AD_BUF_fa_cal[channel][11].img = 0;
				AD_BUF_fa_cal[channel][12].img = 0;
				AD_BUF_fa_cal[channel][13].img = 0;
				AD_BUF_fa_cal[channel][14].img = 0;
				AD_BUF_fa_cal[channel][15].img = 0;
			}
		}
	}
}
#endif 
void callback_scada(unsigned int tcc,unsigned int status,void *appData){
	(void) tcc;
	(void) status;
	need_calculate=1;
}
void callback_fa(unsigned int tcc,unsigned int status,void *appData){
	(void) tcc;
	(void) status;
#if ENABLE_FA
	Swi_post(SwiFaHandle);
#endif
}
void init_ad_edma(){
	init_edma();
	request_channel(0,0);
	set_callback(0,callback_scada);
	request_channel(0,1);
	set_callback(1,callback_fa);

}

