/*
 * Scada.cpp
 *
 *  Created on: 2015-2-4
 *      Author: Administrator
 */
#include "data.h"
#include "Scada.h"


ADCONFIG yc_configure;
Scada scada;
extern setmstime_t YcDeadTime[MAX_YC_NUM];
#if 0
//����ģ����������ͨ��
void YcConfigure(void){
	uint16 i;
	uint16 name_pos;
	for(i = 0; i<MAXYCNUM; i++){
		yc_configure.ad_compass[i] = 0xffff;
	}
	yc_configure.ad_compass[UA(0)] = 0;
	yc_configure.ad_compass[UB(0)] = 1;
	yc_configure.ad_compass[UC(0)] = 2;
	yc_configure.ad_compass[UAB(0)] = 0xFFFF;
	yc_configure.ad_compass[UCB(0)] = 0xFFFF;
	yc_configure.ad_compass[UAC(0)] = 0xFFFF;
	yc_configure.ad_compass[U0(0)] = 3;
	yc_configure.ad_compass[IA1(0)] = 4;
	yc_configure.ad_compass[IB1(0)] = 5;
	yc_configure.ad_compass[IC1(0)] = 6;
	yc_configure.ad_compass[I01(0)] = 7;


	yc_configure.yc_num=MAXYCNUM;
	name_pos=0;
	for ( i = 0; i < MAXYCNUM; i++){
		if (yc_configure.ad_compass[i] == NO_YC){
			yc_configure.yc_num--;
		}else{
			yc_configure.name[name_pos++] = i;
		}
	}
}
// ����������������Խ��ֵ�����ֵ �ı����Ҫ����
int16 ScadaParaInit( void )
{
	uint8 i;
	uint16 YcName;
	YcConfigure();
	CoefTableInit();
	scada.scada_cfg.d_508B.line[0] = (uint8)LimitBounds.tort[0];
	scada.scada_cfg.ClbrtTime.SetFlag = 0;
	scada.scada_cfg.Clbrtnum = 0;

	memset (scada.scada_cfg.StandYc,0,sizeof(scada.scada_cfg.StandYc));
	for (i = 0; i < MAXYCNUM; i++ ){//key
		YcName = yc_configure.name[i];//ֻҪ�����ñ��е�ң�����ȥ����
		if(YcName<=U01){
				scada.scada_cfg.StandYc[LINE1].PortRatio[YcName] = NOMINAL_U;
		}else if((YcName>=IA11)&&(YcName<=I011)){
				scada.scada_cfg.StandYc[LINE1].PortRatio[YcName] = NOMINAL_I;
		}
	}
	for(i = 0; i< 3;i++){
		scada.scada_cfg.StandYc[LINE1].PowerLineRatio[i] = NOMINAL_S ;//pa,pb,pc
	}
	scada.scada_cfg.StandYc[LINE1].PowerLineRatio[3] = NOMINAL_TOTALS;//pall
	scada.YueXianYxInit();
	scada.PtCtParaInit();
	DeadParaInit();
	return 1;


}
#endif
Scada::Scada() {
	scada_cfg.d_508B.line[0] = Pt_3;
	scada_cfg.ClbrtTime.SetFlag = 0;
	scada_cfg.Clbrtnum = 0;
	scada_cfg.looptime.SetFlag = 0;
}
Scada::~Scada() {

}
#if 0
int16 Scada::Init(){
	memset(&YxBuff,0,sizeof(YxBuff));
	memset(&YxTimeFlag,0,sizeof(YxTimeFlag));
	memset(&krtmp,0,sizeof(krtmp));
	memset(&lastkrtmp,0,sizeof(lastkrtmp));
	memset(&scada_cfg.YueXianYx,0,sizeof(scada_cfg.YueXianYx));
	memset(&scada_cfg.looptime,0,sizeof(scada_cfg.looptime));
	memset(&PtCtReg,0,sizeof(PtCtReg));

	for(int i = 0 ; i<MAX_LINE_NUM ; i++){
		FreqReg[i].smpT = 0.625;
		FreqReg[i].smpFreq = 50;
		FreqReg[i].smpadjcnt = 0;
		FreqReg[i].Freq = 50;
	}
	return 1;
}

void Scada::run(void)
{
	static Uint16 i = 0;
	if ((i++<100)){
		i = 0;
	}
}

#define PERLINESOFTYX 0x10
#define PERPTCTSOFTYX 0x17

void Scada::PtCtParaInit( void ){
	
	PtCtReg[0].ua = &scada_cfg.YcData.ua1_4001.Data;
	PtCtReg[0].ub = &scada_cfg.YcData.ub1_4002.Data;
	PtCtReg[0].uc = &scada_cfg.YcData.uc1_4003.Data;
	PtCtReg[0].u0 = &scada_cfg.YcData.u011_4006.Data;
	PtCtReg[0].u1 = &scada_cfg.YcData.Up.Data;
	PtCtReg[0].u2 = &scada_cfg.YcData.Un.Data;
	PtCtReg[0].ia = &scada_cfg.YcData.ia11_4007.Data;
	PtCtReg[0].ib = &scada_cfg.YcData.ib11_4008.Data;
	PtCtReg[0].ic = &scada_cfg.YcData.ic11_4009.Data;
	PtCtReg[0].i0 = &scada_cfg.YcData.i011_400A.Data;
	PtCtReg[0].i1 = &scada_cfg.YcData.Ip.Data;
	PtCtReg[0].i2 = &scada_cfg.YcData.In.Data;

	PtCtReg[0].limitT =10000;//10�룻
	PtCtReg[0].pt_flag = 0;
	PtCtReg[0].ct_flag = 0;

	int len;

	load_db( 1, 0x53f0 , (char *)&PtCtReg[0].sys_type, &len);
	load_db( 1, 0x53f3 , (char *)&PtCtReg[0].pt_type, &len);
	
	load_db( 1, 0x53ec , (char *)&PtCtReg[0].no_voltage, &len);
	load_db( 1, 0x53eb , (char *)&PtCtReg[0].have_voltage, &len);
	load_db( 1, 0x53ea , (char *)&PtCtReg[0].no_current, &len);
	load_db( 1, 0x508d , (char *)&PtCtReg[0].nominal_cur, &len);
	load_db( 1, 0x508e , (char *)&PtCtReg[0].nominal_vol, &len);
		
}
int16 Scada::YueXianYxInit(void)
{
	uint16 i;
	uint16 j;
	float	*YcDataP = NULL	;

	scada_cfg.UupData.ContinueTime =LimitBounds.UupData.ContinueTime;
	scada_cfg.UupData.Limit = LimitBounds.UupData.Limit*NOMINAL_U*0.01;
	scada_cfg.UupData.Recover = LimitBounds.UupData.Recover*NOMINAL_U*0.01;

	scada_cfg.UdownData.ContinueTime =LimitBounds.UdownData.ContinueTime;
	scada_cfg.UdownData.Limit = LimitBounds.UdownData.Limit*NOMINAL_U*0.01;
	scada_cfg.UdownData.Recover = LimitBounds.UdownData.Recover*NOMINAL_U*0.01;

	scada_cfg.U0upData.ContinueTime =LimitBounds.U0upData.ContinueTime;
	scada_cfg.U0upData.Limit = LimitBounds.U0upData.Limit*NOMINAL_U*0.01;
	scada_cfg.U0upData.Recover = LimitBounds.U0upData.Recover*NOMINAL_U*0.01;

	scada_cfg.IupData.ContinueTime =LimitBounds.IupData.ContinueTime;
	scada_cfg.IupData.Limit = LimitBounds.IupData.Limit*NOMINAL_I*0.01;
	scada_cfg.IupData.Recover = LimitBounds.IupData.Recover*NOMINAL_I*0.01;

	scada_cfg.SupData.ContinueTime =LimitBounds.SupData.ContinueTime;
	scada_cfg.SupData.Limit = LimitBounds.SupData.Limit*NOMINAL_S*0.03;
	scada_cfg.SupData.Recover = LimitBounds.SupData.Recover*NOMINAL_S*0.03;
	YcDataP = (float *)&scada_cfg.YcData.ua1_4001;
	j = 0;
	for ( i = 0; i<MAX_LINE_NUM;i++ )
	{
		//DataType1 d_0201;///<��·1 A���ѹԽ���޸澯

		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_UA(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		// DataType1 d_0202;///<��·1 B���ѹԽ���޸澯
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_UB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;

		// DataType1 d_0203;///<��·1 C���ѹԽ���޸澯
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_UC(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		//��·1 AB�ߵ�ѹԽ���޸澯
		j++;

		scada_cfg.YueXianYx[j].LimitParaP =&scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIAXIAN;
		scada_cfg.YueXianYx[j].SrcYcP =&YcDataP[YcData_UA(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		// DataType1 d_0205;///<��·1 B���ѹԽ���޸澯
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIAXIAN;
		scada_cfg.YueXianYx[j].SrcYcP =  &YcDataP[YcData_UB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		// DataType1 d_0206;///<��·1 C���ѹԽ���޸澯
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.UdownData;
		scada_cfg.YueXianYx[j].LimitType = XIAXIAN;
		scada_cfg.YueXianYx[j].SrcYcP =  &YcDataP[YcData_UC(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_IA(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		// DataType1 d_0208;///<��·1 B�����Խ���޸澯
		j++;

		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_IB(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		// DataType1 d_0209;///<��·1 C�����Խ���޸澯
		j++;

		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.IupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_IC(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;

		// DataType1 d_020A;///<��·1 ����Խ���޸澯
		j++;
		scada_cfg.YueXianYx[j].LimitParaP = &scada_cfg.SupData;
		scada_cfg.YueXianYx[j].LimitType = SHANGXIAN;
		scada_cfg.YueXianYx[j].SrcYcP = &YcDataP[YcData_S(i)] ;
		scada_cfg.YueXianYx[j].Enable = 1 ;
		scada_cfg.YueXianYx[j].ChangeFlag = 0;
		j++;
	}

	return 1;
}

int16 Scada::FloatCompare ( float cmp1, float cmp2 )
{
	#define EPSINON  0.00001

    if ( ( cmp1 >= ( cmp2 - EPSINON ) ) && ( cmp1 <= ( cmp2 + EPSINON ) ) ) {
        return 0;
    } else if (  cmp1 > ( cmp2 + EPSINON ) ) {
        return 1;
    } else {
        return -1;
    }
}
#define DEADTIME DeadBandPara.DeadTime

void Scada:: CheckDeadData(uint16 calltype)
{	uint16  i ;

if(calltype){
	for (i = 0; i < CONFIG_YC_NUM; i++){//MAXYCNUM

		YcTable[i].ycdata->deadpass->floatdata.fdata = YcTable[i].ycdata->src_yc[0];//Խ������ װ�븡��������
		YcTable[i].ycdata->deadpass->intdata = YcTable[i].ycdata->src_yc[0]*YcTable[i].ycdata->Coef[0];//Խ������ װ��������������
		YcTable[i].ycdata->changeflag = 0;

	}
}
	for (i = 0; i < CONFIG_YC_NUM; i++){//MAXYCNUM
		if (IsOverDeadband(YcTable[i].ycdata->src_yc[0],YcTable[i].ycdata->deadpass->floatdata.fdata,YcTable[i].ycdata->dead[0])==1)
		{
			if(TCount.CheckCounter(clk0.GetSysMs(),&YcDeadTime[i])==REACHED)
			{
				YcTable[i].ycdata->deadpass->floatdata.fdata = YcTable[i].ycdata->src_yc[0];//Խ������ װ�븡��������
				YcTable[i].ycdata->deadpass->intdata = YcTable[i].ycdata->src_yc[0]*YcTable[i].ycdata->Coef[0];//Խ������ װ��������������
				YcTable[i].ycdata->changeflag = 1;
			}else{
				TCount.StartCounter(DEADTIME,clk0.GetSysMs(),&YcDeadTime[i]);//Ĭ��5ms
			}
		}else{
			TCount.StopCounter(&YcDeadTime[i]);
		}
	}

}

/*
*************************************************************************************
*  @brief  ��Ҫ�ż����ߣ����࣬Խ�޸澯��
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada:: ChecksoftYx(void)
{
	uint16  i ;
	if(LimitBounds.protect_enable.EnableYCEvent==0){
		return;
	}
	for (i = 0; i < YUEXIANYXNUM; i++){
		if(scada_cfg.YueXianYx[i].SrcYcP!=NULL){
			if (scada_cfg.YueXianYx[i].LimitType == SHANGXIAN){
				if(scada_cfg.YueXianYx[i].SrcYcP->Data > scada_cfg.YueXianYx[i].LimitParaP->Limit){//Խ����
					TCount.StopCounter(&scada_cfg.YueXianYx[i].Tcn[1]);
					if(TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[0])==REACHED){
						TerYx.YueXianYx[0][i].statu = _SYSHE_;
						ProduceSoe (&TerYx.YueXianYx[0][i],_SYSHE_, clk0.GetCP56Time2a(0));
						scada_cfg.YueXianYx[i].ChangeFlag = 1;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==0){
						TCount.StartCounter(scada_cfg.YueXianYx[i].LimitParaP->ContinueTime,clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[0]);
					}
				}else if(scada_cfg.YueXianYx[i].SrcYcP->Data < scada_cfg.YueXianYx[i].LimitParaP->Recover){//Խ���߻ָ�
					TCount.StopCounter(&scada_cfg.YueXianYx[i].Tcn[0]);
					if(TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[1])==REACHED){
						TerYx.YueXianYx[0][i].statu = _SYSFEN_;
						ProduceSoe (&TerYx.YueXianYx[0][i],_SYSFEN_, clk0.GetCP56Time2a(0));
						scada_cfg.YueXianYx[i].ChangeFlag = 0;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==1){
						TCount.StartCounter(scada_cfg.YueXianYx[i].LimitParaP->ContinueTime,clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[1]);
					}
				}
			}else if(scada_cfg.YueXianYx[i].LimitType == XIAXIAN){
				if(scada_cfg.YueXianYx[i].SrcYcP->Data < scada_cfg.YueXianYx[i].LimitParaP->Limit){//Խ����
					TCount.StopCounter(&scada_cfg.YueXianYx[i].Tcn[1]);
					if(TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[0])==REACHED){
						TerYx.YueXianYx[0][i].statu = _SYSHE_;
						ProduceSoe (&TerYx.YueXianYx[0][i],_SYSHE_, clk0.GetCP56Time2a(0));
						scada_cfg.YueXianYx[i].ChangeFlag=1;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==0){
						TCount.StartCounter(scada_cfg.YueXianYx[i].LimitParaP->ContinueTime,clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[0]);
					}
				}else if(scada_cfg.YueXianYx[i].SrcYcP ->Data > scada_cfg.YueXianYx[i].LimitParaP->Recover){//Խ���߻ָ�
					TCount.StopCounter(&scada_cfg.YueXianYx[i].Tcn[0]);
					if(TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[1])==REACHED){
						TerYx.YueXianYx[0][i].statu = _SYSFEN_;
						ProduceSoe (&TerYx.YueXianYx[0][i],_SYSFEN_, clk0.GetCP56Time2a(0));
						scada_cfg.YueXianYx[i].ChangeFlag=0;
					}else if(scada_cfg.YueXianYx[i].ChangeFlag==1){
						TCount.StartCounter(scada_cfg.YueXianYx[i].LimitParaP->ContinueTime,clk0.GetSysMs(),&scada_cfg.YueXianYx[i].Tcn[1]);
					}
				}
			}
		}
	}

}
#define NO_VALID_EARTH 	1
#define VALID_EARTH 	2
/*
�ж�������������RCS985
Pt�����жϣ�
1�������ѹС��30V������һ���������0.04In
2�������ѹU2����8V��
������������������ʱ10s�󱣸澯���쳣��ʧ����ʱ10s�Զ����ء�
Ct�����жϣ�
I2>0.2A+0.25Imax,����һ�����С����0.04In
��������������ʱ10s�󱣸澯���쳣��ʧ����ʱ10s�Զ����ء�
*/
void Scada::CheckPtCt(void)
{
	float ia,ib,ic,u1,u2,i2;
	int state = 0;
	int cnt_statu;
	ia = *PtCtReg[0].ia;
	ib = *PtCtReg[0].ib;
	ic = *PtCtReg[0].ic;
	u1 = *PtCtReg[0].u1;
	u2 = *PtCtReg[0].u2;
	//i1 = *PtCtReg[0].i1;
	i2 = *PtCtReg[0].i2;
	
	float Imax = max(ia,ib);
	Imax = max(Imax,ic);
	float Imin = min(ia,ib);
	Imin = min(Imin,ic);
	//fot pt
	
	if( PtCtReg[0].sys_type == NO_VALID_EARTH){
		if((u1<PtCtReg[0].nominal_vol*0.15 && Imin>PtCtReg[0].no_current) || u2>8 ){
			state = 1;
		}else
			state = 0;
		if( state == 1){
			TCount.StopCounter(&PtCtReg[0].pt_recover_cnt);
			TCount.StartCounter(10000,clk0.GetSysMs(),&PtCtReg[0].pt_check_cnt);
			cnt_statu = TCount.CheckCounter(clk0.GetSysMs(),&PtCtReg[0].pt_check_cnt);
			if( cnt_statu == REACHED){
				if( PtCtReg[0].pt_flag == 0){
					PtCtReg[0].pt_flag = 1;
					ProduceSoe (&TerYx.pt_yx[0],_SYSHE_, clk0.GetCP56Time2a(0));
				}
			}
		}else{
			TCount.StopCounter(&PtCtReg[0].pt_check_cnt);
			TCount.StartCounter(10000,clk0.GetSysMs(),&PtCtReg[0].pt_recover_cnt);
			cnt_statu = TCount.CheckCounter(clk0.GetSysMs(),&PtCtReg[0].pt_recover_cnt);
			if( cnt_statu  == REACHED){
				if( PtCtReg[0].pt_flag ==1 ){
					PtCtReg[0].pt_flag = 0;
					ProduceSoe (&TerYx.pt_yx[0],_SYSFEN_, clk0.GetCP56Time2a(0));
				}
			}
		}
	}

	//for ct
	if(i2>(0.25*Imax) && Imin<PtCtReg[0].no_current){
		state = 1;
	}else
		state = 0;
	if( state == 1){
		TCount.StopCounter(&PtCtReg[0].ct_recover_cnt);
		TCount.StartCounter(10000,clk0.GetSysMs(),&PtCtReg[0].ct_check_cnt);
		cnt_statu = TCount.CheckCounter(clk0.GetSysMs(),&PtCtReg[0].ct_check_cnt);
		if( cnt_statu == REACHED){
			if( PtCtReg[0].ct_flag == 0){
				PtCtReg[0].ct_flag = 1;
				ProduceSoe (&TerYx.ct_yx[0],_SYSHE_, clk0.GetCP56Time2a(0));
			}
		}
	}else{
		TCount.StopCounter(&PtCtReg[0].ct_check_cnt);
		TCount.StartCounter(10000,clk0.GetSysMs(),&PtCtReg[0].ct_recover_cnt);
		cnt_statu = TCount.CheckCounter(clk0.GetSysMs(),&PtCtReg[0].ct_recover_cnt);
		if( cnt_statu  == REACHED){
			if( PtCtReg[0].ct_flag ==1 ){
				PtCtReg[0].ct_flag = 0;
				ProduceSoe (&TerYx.ct_yx[0],_SYSFEN_, clk0.GetCP56Time2a(0));
			}
		}
	}
}

void Scada::Caculate(void)
{
	TCount.StartCounter(30,clk0.GetSysMs(),&scada_cfg.looptime);
	if(TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.looptime)==REACHED){
		if(CaculateFlag==1){
			GetModulus();
			GetFreq( 0 );
			if (scada_cfg.StartClbrt==BDSTART){
				TCount.StartCounter(10000,clk0.GetSysMs(),&scada_cfg.ClbrtTime);//Ĭ��10�룻
				Calibration();
			}else {
 				GetRms();
				//GetHarmonicData();
 				GetPQS();
				Filter();
 				CheckDeadData(0);
 				CheckPtCt();
 				ChecksoftYx();
			}
			CaculateFlag = 0;
		}
	}

}

void Scada:: GetModulus(void)
{
	uint16 portnum;
	uint16 YcNum;
	uint16 YcName;

	FftDeal();//
	
 	for(YcNum = 0; YcNum < yc_configure.yc_num; YcNum++){//�������е����õ�ң�������
 		YcName = yc_configure.name[YcNum];//��ȡ���õ�ң����
 		portnum = yc_configure.ad_compass[YcName];//ͨ��ң�����ȡ��Ӧ��ͨ����
		if(portnum!=NO_YC){
			mymemcpy(&scada_cfg.Modulus[YcName],&RFFTmagBuff[portnum],sizeof(scada_cfg.Modulus[0]));// use memcpy will influence interrupt Time0?
			mymemcpy(&scada_cfg.RFFTPhaseBuff[YcName],&RFFTPhaseBuff[portnum],sizeof(scada_cfg.RFFTPhaseBuff[0]));
		}else{
			//memset(&scada_cfg.Modulus[YcName],0,sizeof(scada_cfg.Modulus[0]));
			//memset(&scada_cfg.RFFTPhaseBuff[YcName],0,sizeof(scada_cfg.RFFTPhaseBuff[0]));
		}
 	}
 	
}

float  Scada:: TotalRms(float *src , uint8 len)
{
	unsigned int i;
	float SumOfSquares = 0;
	float Radication = 0;
	for( i = 0; i < len; i++){
		SumOfSquares = SumOfSquares +  ( ( * ( src+ i ) ) * ( * ( src + i ) ) ) ;
	}
	Radication = sqrt(SumOfSquares);//*DEVXISHU;
	return Radication;
}


harmonicdata_t Scada:: GetHarmonicRmsAndRatio(float BaseHarmonicRms,float *src)
{
	harmonicdata_t HmncData;
	HmncData.rms = TotalRms(src,1);
	HmncData.rario = HmncData.rms/BaseHarmonicRms;
	if (HmncData.rario > 1.001){
		HmncData.rario = 1.0000;
	}
	HmncData.rario = HmncData.rario *100.0;
	return HmncData ;
}

void Scada:: GetHarmonicData(void)
{
	uint8 i;
	//float *Pycia=&scada_cfg.YcData.ia1rms2_402E;
	//float *Pycib=&scada_cfg.YcData.ib1rms2_4078;
	//float *Pycic=&scada_cfg.YcData.ic1rms2_40C2;
	for (i = 0; i < 14; i++ ){//key
//		Pycia[i].Data= RFFTmagBuff[IA1(0)][BASEHARMONIC+i]*YcRatio[0].PortRatio[IA1(0)];
//		Pycib[i].Data= RFFTmagBuff[IB1(0)][BASEHARMONIC+i]*YcRatio[0].PortRatio[IB1(0)];
//		Pycic[i].Data= RFFTmagBuff[IC1(0)][BASEHARMONIC+i]*YcRatio[0].PortRatio[IC1(0)];
	}
}

/*********************************************************************************************************
** �������� ��CaculateXU(int8_t flag,Complex*d1,Complex*d2,Complex*d3)
** �������� ���������򣬸�������
** ��ڲ��� ��flag 0 ����1����2����Complex*d1,Complex*d2,Complex*d3 ��������г������ʵ���鲿��ʵ���õ����ǻ���������
** ���ڲ��� ����Ӧ������ķ�ֵ
*********************************************************************************************************/
float Scada::CaculateXU(int8_t flag,Complex*d1,Complex*d2,Complex*d3)
{
	//����	
	//R=(Rua0-.5*Rub0+.866*Iub0-.5*Ruc0-.866*Iuc0);
	//I=(Iua0-.5*Iub0-.866*Rub0-.5*Iuc0+.866*Ruc0);	
	//����
	//R=(Rua0-.5*Rub0-.866*Iub0-.5*Ruc0+.866*Iuc0);
	//I=(Iua0-.5*Iub0+.866*Rub0-.5*Iuc0-.866*Ruc0);
	float R1,R2,R3;
	float I1,I2,I3;

	float R,I;
	float res;
	res=0;
	R1=d1[0].rel;
	R2=d2[0].rel;
	R3=d3[0].rel;
	I1=d1[0].img;
	I2=d2[0].img;
	I3=d3[0].img;

	R=I=0;

	if(flag==0)
		{
			R=R1+R2+R3;
			I=I1+I2+I3;
		}
	else if(flag==1)
		{
		R=(R1-.5*R2-.866*I2-.5*R3+.866*I3);
		I=(I1-.5*I2+.866*R2-.5*I3-.866*R3);
		}
	else if(flag==2)
		{
		R=(R1-.5*R2+.866*I2-.5*R3-.866*I3);
		I=(I1-.5*I2-.866*R2-.5*I3+.866*R3);	
		}
		res=R*R+I*I;
		res=sqrt(res)/3.0;
			
		return res;
}
/*********************************************************************************************************
** �������� ��CaculateXU2(int8_t flag,Complex*d1,Complex*d2,Complex*d3)
** �������� ���������򣬸�������
** ��ڲ��� ��flag 0 ����1����2����Complex*d1,Complex*d2,Complex*d3 ��������г������ʵ���鲿��ʵ���õ����ǻ���������
** ���ڲ��� ����Ӧ�����
*********************************************************************************************************/
Complex Scada::CaculateXU2(int8_t flag,Complex*d1,Complex*d2,Complex*d3)
{
	//����	
	//R=(Rua0-.5*Rub0+.866*Iub0-.5*Ruc0-.866*Iuc0);
	//I=(Iua0-.5*Iub0-.866*Rub0-.5*Iuc0+.866*Ruc0);	
	//����
	//R=(Rua0-.5*Rub0-.866*Iub0-.5*Ruc0+.866*Iuc0);
	//I=(Iua0-.5*Iub0+.866*Rub0-.5*Iuc0-.866*Ruc0);
	float R1,R2,R3;
	float I1,I2,I3;

	float R,I;
	Complex res;
	res.rel=res.img=0;
	R1=d1[0].rel;
	R2=d2[0].rel;
	R3=d3[0].rel;
	I1=d1[0].img;
	I2=d2[0].img;
	I3=d3[0].img;

	R=I=0;

	if(flag==0)
		{
			R=R1+R2+R3;
			I=I1+I2+I3;
		}
	else if(flag==1)
		{
		R=(R1-.5*R2-.866*I2-.5*R3+.866*I3);
		I=(I1-.5*I2+.866*R2-.5*I3-.866*R3);
		}
	else if(flag==2)
		{
		R=(R1-.5*R2+.866*I2-.5*R3-.866*I3);
		I=(I1-.5*I2-.866*R2-.5*I3+.866*R3);	
		}
	res.rel=R;
	res.img=I;
	return res;
}

void Scada:: GetRms(void)
{
	uint8 i;
	uint16 YcName;
	Complex A,B,C;
	float *Pyc1=&scada_cfg.YcData.ua1_4001;
//	float *Pyc2=&scada_cfg.YcData.ua2_40FA;
	//memset(&scada_cfg.YcData,0,sizeof(scada_cfg.YcData)); // ��Ӱ���ж� ?
	for (i = 0; i < CHANNELNUM; i++ ){//key 8��ģ����ͨ��
		YcName = yc_configure.name[i];//ֻҪ�����ñ��е�ң�����ȥ����
			if((YcName>=UA2)&&(YcName<=25)){
				//Pyc2[YcName-13].Data= (TotalRms(&scada_cfg.Modulus[YcName][BASEHARMONIC],rmstype)*YcRatio[1].PortRatio[YcName-13]);
			}else if(YcName!=NO_YC){
				Pyc1[YcName].Data= (TotalRms(&scada_cfg.Modulus[YcName][BASEHARMONIC],XIEBOLEN)*YcRatio[0].PortRatio[YcName]);
			}
	}
	scada_cfg.YcData.freq1_401B.Data=FreqReg[0].Freq;
	A = Complex((float)RFFToutBuff[0][BASEHARMONIC_REL]*YcRatio[0].PortRatio[UA1],(float)RFFToutBuff[0][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[UA1]);
	B = Complex(RFFToutBuff[1][BASEHARMONIC_REL]*YcRatio[0].PortRatio[UB1],RFFToutBuff[1][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[UB1]);
	C = Complex(RFFToutBuff[2][BASEHARMONIC_REL]*YcRatio[0].PortRatio[UC1],RFFToutBuff[2][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[UC1]);
	scada_cfg.YcData.Up.Data = CaculateXU(1, &A, &B, &C);
	scada_cfg.YcData.Un.Data = CaculateXU(2, &A, &B, &C);
	if(LimitBounds.U0_source==1){//�ϳ� ���ֻ������pt,�ϳ��Ǵ����
		scada_cfg.YcData.u011_4006.Data = CaculateXU(0, &A, &B, &C)*3.0;
	}

	A = Complex(RFFToutBuff[4][BASEHARMONIC_REL]*YcRatio[0].PortRatio[IA11],RFFToutBuff[4][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[IA11]);
	B = Complex(RFFToutBuff[5][BASEHARMONIC_REL]*YcRatio[0].PortRatio[IB11],RFFToutBuff[5][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[IB11]);
	C = Complex(RFFToutBuff[6][BASEHARMONIC_REL]*YcRatio[0].PortRatio[IC11],RFFToutBuff[6][BASEHARMONIC_IMG]*YcRatio[0].PortRatio[IC11]);
	scada_cfg.YcData.Ip.Data = CaculateXU(1, &A, &B, &C);
	scada_cfg.YcData.In.Data = CaculateXU(2, &A, &B, &C);
	if(LimitBounds.I0_source==1){//
		scada_cfg.YcData.i011_400A.Data = CaculateXU(0, &A, &B, &C)*3.0;
	  }
}

/*
*************************************************************************************
*  @brief  ����Ƶ��
*  @author   zf
*  @param[in]   ������
*  @param[out]
*  @return
**************************************************************************************
*/
/*
*********************************************************************************************************
** �������� ��cal_horizen()
** �������� ������(x1,y1) (x2,y2)������ֱ�ߣ����ֱ�߹�X�������ֵ
** ��ڲ��� �� ����ֱ�ߵ�����
** ���ڲ��� ����ֱ�߹�X�������ֵ
*********************************************************************************************************/
float cal_horizen(int16 x1,float y1,int16 x2,float y2)
{
	return (y1*((x1-x2)/(y2-y1)) + x1);
}
void Scada::GetFreq( int port )
{
	CaculateFreq(&ScadaAdBuf[port][0],48,0);
}
float  Scada:: CaculateFreq(int16 *src , uint8 len, uint8 line)
{
	float index[4];
	int i;
	int j = 0;
	float FreqTmp;
	for(i=0;(i<(len-1)) && (j<2);i++) {
		if((src[i]<=0 && src[i+1]>0))
			index[j++] = cal_horizen(i,src[i],i+1,src[i+1]);
	}
	if(j<2) {
		j=0;
		for(i=0;(i<(len-1)) && (j<2);i++) {
			if((src[i]>0 && src[i+1]<=0))
				index[j++] = cal_horizen(i,src[i],i+1,src[i+1]);
		}
	}
	if(j==2) {
		FreqReg[line].T = FreqReg[line].smpT *(index[1] - index[0]);
	//	FreqReg[line].Freq = 1000.0/FreqReg[line].T ;
		FreqTmp= 1000.0/FreqReg[line].T ;
		if((FreqTmp>44)&&(FreqTmp<56)){
			FreqReg[line].difFreq = FreqTmp - FreqReg[line].smpFreq;
			if((FreqReg[line].difFreq>0.1)||(FreqReg[line].difFreq<-0.1)){
				FreqReg[line].smpadjcnt++;
			}else{
				FreqReg[line].smpadjcnt = 0;
			}
			if (FreqReg[line].smpadjcnt>=10){
				FreqReg[line].smpadjcnt = 0;
				FreqReg[line].Freq = FreqTmp;
				FreqReg[line].smpFreq = FreqTmp ;
				//FreqReg[line].smpT = (1000.0/FreqReg[line].smpFreq)/32.0;
				//SetCpuTimerPerfiod ( &CpuTimer0, 150, FreqReg.smpT*1000.0);//���ȼ� CPU 5��PIE 7
			}
		}else{
			FreqReg[line].smpadjcnt++;
			if (FreqReg[line].smpadjcnt>=10){
				FreqReg[line].Freq = 50;
				FreqReg[line].smpadjcnt = 0;
				FreqReg[line].smpFreq = 50 ;
				//FreqReg[line].smpT = (1000.0/FreqReg[line].smpFreq)/32.0;
				//SetCpuTimerPerfiod ( &CpuTimer0, 150, FreqReg.smpT*1000.0);//���ȼ� CPU 5��PIE 7
			}
		}
		return 0;
	}
	return -1;
}

float Scada::GetCplAngle(Complex CplData)
{
	float result;
    if ( fabs(CplData.rel) <=0.01 ) {
        if ( CplData.img > 0 ) {
            result = PI / 2.0; //90��
        } else {
            result = -PI / 2.0; // -90��
        }
    } else {
        if ( fabs(CplData.img) <=0.01 ) {
            if ( CplData.rel > 0 ) {
                result = 0;
            } else {
                result = PI;
            }
        } else {
            result = atan2 ( CplData.img,CplData.rel );
        }
    }
	return result;
}



//��ʱ����תΪ���Ƕȷ�֮Ϊ�� ��������ߵ�ѹ��������ת�ɵ�һ��λUAB, �ڶ���ΪUCB
//��������ѹ
void Scada:: PhaseChange(Complex *dir, Complex *src ,  uint16 len ,float offsetangle )
{
 uint8 i;
 for (i = 1; i < len ; i++){
	 dir[i]  = ComplexRotation(src[i] ,offsetangle* (i));
 }
}
//
Complex Scada:: ComplexRotation(Complex Input,float Angle)
{
	Complex Outdata;
	float cosa,sina;
	cosa = cos(Angle);
	sina = sin(Angle);
	Outdata.rel = Input.rel*cosa - Input.img*sina;
	Outdata.img = Input.img*cosa + Input.rel*sina;
	return Outdata;

}

Complex Scada::TypeChange(float rel,float img)
{
	Complex C;
	C.rel = rel;
	C.img = img;
	return C;
}

power_t  Scada:: GetPower(uint8 Uname,uint8 Iname)
{
	uint8 j;
	anglevalue_t sincosvalue;
	power_t Power[XIEBOLEN];
	power_t TotalP;
	float32 angle;
	for(j = 1; j<XIEBOLEN+1; j++){
		angle = scada_cfg.RFFTPhaseBuff[Uname][j] - scada_cfg.RFFTPhaseBuff[Iname][j];
		sincosvalue.cos_a = cos(angle);
		sincosvalue.sin_a = sin(angle);
		Power[j-1].s = scada_cfg.Modulus[Uname][j]*scada_cfg.Modulus[Iname][j];
		Power[j-1].p = Power[j-1].s*sincosvalue.cos_a;
		Power[j-1].q = Power[j-1].s*sincosvalue.sin_a;
	}
	TotalP.p = 0;
	TotalP.q = 0;
	TotalP.s = 0;
	for(j = 0; j<XIEBOLEN; j++){
		TotalP.p+=Power[j].p;
		TotalP.q+=Power[j].q;
		TotalP.s+=Power[j].s;
	}

	return TotalP;
}


void Scada:: GetPQS(void)
{
	power_t powertmp;
	uint8 flag = 0,i;
	float p,q,s;
	float *Pyc;

	for ( i = 0; i < MAX_LINE_NUM; i++){
		if(scada_cfg.d_508B.line[LINE(i)]==Pt_3){//�������˲���ͨ���������ݿɲ�
			powertmp = GetPower(UA(i),IA1(i));
			flag = 1;
		}else if(scada_cfg.d_508B.line[LINE(i)]==Pt_2){
			//powertmp = GetPower(UAB(i),IA1(i));
			powertmp = GetPower(UA(i),IA1(i));
			flag = 1;
		}
		if (i==0){
			Pyc = &scada_cfg.YcData.pa1_400E;//��һ���� PA;
		}else {
			//Pyc = &scada_cfg.YcData.pa2_4107;//�ڶ����� PA;
		}
		if (flag == 1){
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_A];///<��·1 A���й�����
			Pyc[1].Data = \
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_A].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_A];	///<��·1 A���޹�����
			Pyc[2].Data =Pyc[0].Data*100 /sqrt(Pyc[0].Data*Pyc[0].Data + Pyc[1].Data*Pyc[1].Data); //(powertmp.p/powertmp.s)*100.0;	///<��·1 A�๦������
			if((Pyc[2].Data<0.01)||((Pyc[0].Data<1.0)&&(Pyc[0].Data>-1.0))){
				Pyc[0].Data = 0;
				Pyc[2].Data = 0;
			}
			if((Pyc[1].Data<4.0)&&(Pyc[1].Data>-4.0)){
				Pyc[1].Data = 0;
			}
		}else{
			Pyc[0].Data = 0;//scada_cfg.YcData.d_400E.Data = 0;
			Pyc[1].Data = 0;//scada_cfg.YcData.d_400F.Data = 0;
			Pyc[2].Data = 0;//scada_cfg.YcData.d_4010.Data = 0;
		}

		if (i==0){
			Pyc = &scada_cfg.YcData.pb1_4011;//��һ���� Pb;
		}else {
		//	Pyc = &scada_cfg.YcData.pb2_410A;//�ڶ����� Pb;
		}
		if(scada_cfg.d_508B.line[LINE(i)]==Pt_3){
			powertmp = GetPower(UB(i),IB1(i));
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_B];	///<��·1 B���й�����
			Pyc[1].Data = \
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_B].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_B];	///<��·1 B���޹�����
			Pyc[2].Data = Pyc[0].Data*100 /sqrt(Pyc[0].Data*Pyc[0].Data + Pyc[1].Data*Pyc[1].Data); //(powertmp.p/powertmp.s)*100.0;	///<��·1 B�๦������
			if((Pyc[2].Data<0.01)||((Pyc[0].Data<1.0)&&(Pyc[0].Data>-1.0))){
						Pyc[0].Data = 0;
						Pyc[2].Data = 0;
			}
			if((Pyc[1].Data<4.0)&&(Pyc[1].Data>-4.0)){
				Pyc[1].Data = 0;
			}
		}else{
			Pyc[0].Data = 0; //scada_cfg.YcData.d_4011.Data = 0;
			Pyc[1].Data = 0; //scada_cfg.YcData.d_4012.Data = 0;
			Pyc[2].Data = 0; //scada_cfg.YcData.d_4013.Data = 0;
		}
		flag = 0;
		if(scada_cfg.d_508B.line[LINE(i)]==Pt_3){
			powertmp = GetPower(UC(i),IC1(i));
			flag = 1;

		}else if(scada_cfg.d_508B.line[LINE(i)]==Pt_2){
			//powertmp = GetPower(UCB(i),IC1(i));
			powertmp = GetPower(UC(i),IC1(i));
			flag = 1;
		}
		if (i==0){
			Pyc = &scada_cfg.YcData.pc1_4014;//��һ���� Pc;
		}else {
		//	Pyc = &scada_cfg.YcData.pc2_410D;//�ڶ����� Pc;
		}
		if (flag==1){
			Pyc[0].Data = \
				(powertmp.p *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].cos_a
				+powertmp.q*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_C];	///<��·1 C���й�����
			Pyc[1].Data =\
				(powertmp.q *YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].cos_a
				-powertmp.p*YcRatio[LINE(i)].PowerAngleRatio[0][PHASE_C].sin_a)\
				*YcRatio[LINE(i)].PowerLineRatio[PHASE_C];	///<��·1 C���޹�����
			Pyc[2].Data = Pyc[0].Data*100 /sqrt(Pyc[0].Data*Pyc[0].Data + Pyc[1].Data*Pyc[1].Data); //(powertmp.p/powertmp.s) * 100.0;	///<��·1 C�๦������
			if((Pyc[2].Data<0.01)||((Pyc[0].Data<1.0)&&(Pyc[0].Data>-1.0))){
						Pyc[0].Data = 0;
						Pyc[2].Data = 0;
			}
			if((Pyc[1].Data<4.0)&&(Pyc[1].Data>-4.0)){
				Pyc[1].Data = 0;
			}
		}else {
			Pyc[0].Data = 0;//scada_cfg.YcData.d_4014.Data = 0;
			Pyc[1].Data = 0;//scada_cfg.YcData.d_4015.Data = 0;
			Pyc[2].Data = 0;//scada_cfg.YcData.d_4016.Data = 0;
		}

	}
	scada_cfg.YcData.P1_4017.Data = scada_cfg.YcData.pa1_400E.Data
								   +scada_cfg.YcData.pb1_4011.Data
								   +scada_cfg.YcData.pc1_4014.Data;	///<��·1 ���й�����
	p = scada_cfg.YcData.P1_4017.Data;
	scada_cfg.YcData.Q1_4018.Data = scada_cfg.YcData.qa1_400F.Data
								   +scada_cfg.YcData.qb1_4012.Data
								   +scada_cfg.YcData.qc1_4015.Data;	///<��·1 ���޹�����
	q = scada_cfg.YcData.Q1_4018.Data;

	s = sqrt(p*p +q*q);
	scada_cfg.YcData.S1_4019.Data = sqrt(p*p +q*q);	///<��·1 �����ڹ���
	scada_cfg.YcData.COS1_401A.Data = (p/s)*100.0;	///<��·1 �ܹ�������
	if(	scada_cfg.YcData.COS1_401A.Data<0.01){
		scada_cfg.YcData.COS1_401A.Data= 0;
	}
}
void Scada::first_compare(float *max,float *min,float var)
{
	if(FloatCompare ((*max), var)==1){
		(*min) = var;
	}else {
		(*min) = (*max);
		(*max) = var;
	}

}
int16 Scada::IsOverDeadband ( float value1, float value2, float db )
{
    short ret;
    float fluctuations = fabs ( value1 - value2 );
    //ret = float_compare (fluctuations, db * fabs(value1));//�ٷֱȱȽ�,ע��value Ϊ����ʱ
	ret = FloatCompare (fluctuations, db );//��ֵ�Ƚ�
    if ( ret > 0 ) {
        return 1;
    } else {
        return 0;
    }
}
// �������ֵ,��Сֵ,ƽ��ֵ.ƽ��ֵ��ȥ�����ֵ,��Сֵ
void Scada::compare(float *max,float *min,float *avr,float var,float avr_num)
{
	if((var) > (*max)){//���ڴ�ֵ��������ֵ
		(*avr) +=(*max)/(float)avr_num;
		(*max) = (var);
	}else if((var)< (*min)){//С��СָСֵ������Сֵ
		(*avr) += (*min)/(float)avr_num ;
		(*min) = (var) ;
	}else{
		(*avr) += (var)/(float)avr_num ;
	}
}



void Scada::GetXishu(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue )
{
	if((*sourceyc)>minvalue){

		(*xishu) = (float)(*standardyc)/(*sourceyc);

	}

}
/*
*************************************************************************************
*  @brief   �궨      �궨������DataId = 5097) �·�1 ��ʼ�궨���궨�� ��Varm ���� ������DataId = 5097)
*  			ֵΪ��һλΪfpag1 �� 1�ɹ�0ʧ�ܣ��ڶ�λΪfpga2�� 1�ɹ�0ʧ�ܣ� ��ʱ����0xff��
*  @author   zf
*  @param[in]
*  @param[out]
*  @return
**************************************************************************************
*/
void Scada::Calibration(void)
{
	volatile	int8 erro[2];
	power_t  powertmp;
	uint8 Clbrtnum;
	uint8 i,j;
	float angle;
	scada_cfg.Clbrtnum++;
	Clbrtnum = scada_cfg.Clbrtnum;
	if(Clbrtnum==1){ //����clbrtemp�еĳ�ʼֵ
		for (i = 0; i < MAXYCNUM; i++ ){
			scada_cfg.ClbrtTmp[i].max= TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN);
			scada_cfg.ClbrtTmp[i].avr = 0;
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				scada_cfg.ClbrtPowerTmp[i][j].max = GetPower(UA(i)+j,IA1(i)+j);
				scada_cfg.ClbrtPowerTmp[i][j].avr.p = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.q = 0.0;
				scada_cfg.ClbrtPowerTmp[i][j].avr.s = 0.0;
			}
		}
	}
	if(Clbrtnum==2){
		for (i = 0; i< MAXYCNUM;i++){
			 first_compare(&scada_cfg.ClbrtTmp[i].max,&scada_cfg.ClbrtTmp[i].min,TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN));
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				powertmp = GetPower(UA(i)+j,IA1(i)+j);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,powertmp.p);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,powertmp.q);
				first_compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,powertmp.s);
			}
		}
	}
	if((Clbrtnum>=3)&&(Clbrtnum<=(AVERAGENUM + 2))){ //����clbrttemp��min,max,avr
		for (i = 0; i< MAXYCNUM;i++){
			compare(&scada_cfg.ClbrtTmp[i].max,&scada_cfg.ClbrtTmp[i].min,&scada_cfg.ClbrtTmp[i].avr,TotalRms(&scada_cfg.Modulus[i][BASEHARMONIC],XIEBOLEN),AVERAGENUM);
		}
		for (i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				powertmp = GetPower(UA(i)+j,IA1(i)+j);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.p,&scada_cfg.ClbrtPowerTmp[i][j].min.p,&scada_cfg.ClbrtPowerTmp[i][j].avr.p,powertmp.p,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.q,&scada_cfg.ClbrtPowerTmp[i][j].min.q,&scada_cfg.ClbrtPowerTmp[i][j].avr.q,powertmp.q,AVERAGENUM);
				compare(&scada_cfg.ClbrtPowerTmp[i][j].max.s,&scada_cfg.ClbrtPowerTmp[i][j].min.s,&scada_cfg.ClbrtPowerTmp[i][j].avr.s,powertmp.s,AVERAGENUM);
			}
		}
	}
	if(Clbrtnum==(AVERAGENUM + 2)){ //����ȥ�������ֵ,��Сֵ,����ƽ������Ҫ��2
		memcpy(&scada_cfg.YcRatioTmp,&YcRatio,sizeof(scada_cfg.YcRatioTmp));
		for (i = 0; i < 13; i++){
			GetXishu(&scada_cfg.YcRatioTmp[0].PortRatio[i], &scada_cfg.ClbrtTmp[i].avr,&scada_cfg.StandYc[0].PortRatio[i],BIAODINGMIN );
		}
		//for (i = 0; i < 13; i++){//�ڶ�����
		//	GetXishu(&scada_cfg.YcRatioTmp[1].PortRatio[i], &scada_cfg.ClbrtTmp[i+13].avr,&scada_cfg.StandYc[1].PortRatio[i],BIAODINGMIN );
		//}
		for ( i = 0; i < MAX_LINE_NUM; i++){
			for ( j = 0; j < 3; j++){
				scada_cfg.YcRatioTmp[i].PowerLineRatio[j]  = scada_cfg.YcRatioTmp[i].PortRatio[UA1+j]
				                                          * scada_cfg.YcRatioTmp[i].PortRatio[IA11+j];
				powertmp = scada_cfg.ClbrtPowerTmp[i][j].avr;
				angle = asin((scada_cfg.YcRatioTmp[i].PowerLineRatio[j]*powertmp.q)/scada_cfg.StandYc[i].PowerLineRatio[j])-(PI / 6.0);
				scada_cfg.YcRatioTmp[i].PowerAngleRatio[0][j].angle = (angle / PI) * 180.0;
				scada_cfg.YcRatioTmp[i].PowerAngleRatio[0][j].sin_a =sin(angle);
				scada_cfg.YcRatioTmp[i].PowerAngleRatio[0][j].cos_a =cos(angle);
			}
			//scada_cfg.YcRatioTmp[i].PortRatio[UAB1]=scada_cfg.YcRatioTmp[i].PortRatio[UA1];
			//scada_cfg.YcRatioTmp[i].PortRatio[UCB1]=scada_cfg.YcRatioTmp[i].PortRatio[UC1];
		}
		scada_cfg.StartClbrt=BDSTOP;
		scada_cfg.Clbrtnum = 0;
		TCount.StopCounter(&scada_cfg.ClbrtTime);
		memcpy(&YcRatio,&scada_cfg.YcRatioTmp,sizeof(YcRatio));
		ParaSave((Uint16 *)&YcRatio,WRITE);
	}
	if (TCount.CheckCounter(clk0.GetSysMs(),&scada_cfg.ClbrtTime)==REACHED){//��ʱ��
		scada_cfg.StartClbrt=BDSTOP;
		scada_cfg.Clbrtnum = 0;
	}
}

void Scada::Filter(void)
{
	if(scada_cfg.YcData.COS1_401A.Data>100.0){
		scada_cfg.YcData.COS1_401A.Data = 100;
	}
	if	(scada_cfg.YcData.COS1_401A.Data<0.01){
		scada_cfg.YcData.COS1_401A.Data = 0;
	}
	if((scada_cfg.YcData.freq1_401B.Data>60.0)||(scada_cfg.YcData.freq1_401B.Data<40.0)){
		scada_cfg.YcData.freq1_401B.Data = 50;
	}
}
#endif
