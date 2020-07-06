#ifndef FA_H
#define FA_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h" ///<用到BaseApp类.
#include "interruptapp.h"
#include "clock.h"
#include "dspdatatype.h"
#include "fa_fft_cal.h"
#include <stdio.h>
#include <stdlib.h>
#include "fpga.h"
#include "scada.h"
#include "atan2sp.h"
#include <math.h>
#include "product.h"
#define ONESHOT 0
#define NODELAY 0
#define FANXIANG 0
#define ZHENGXIANG 1
#define CHECKHE 0
#define CHECKFEN 1

#define STARTCHZ 1
#define CHECKCHZ 0

#define LOCK 1
#define UNLOCK 0

#define ENABLE 1
#define DISENABLE 0


extern CP56Time2a PROTECTTIME;
extern Counter T_tool;
#define SaveProSoe(SoeId,DATA) scada.PushSoe(PROTECTSOE,SoeId,scada.GetLogicYx(DATA, SINGLE),PROTECTTIME);

//#define GLFX  //功率方向
//#define BHCK  //保护出口+重合闸


typedef enum GZTATUE{
	DEFAULTRESET =0 , //故障复归
	INSTANTANEOUR ,
	FOREVER ,
	INSTANTANEOUR_END,
	FOREVER_END
}GZTATUE_EM;
typedef enum PROTECTSTATUE{
	PROTECTSTOP = 0,
	PROTECTIN,
	PROTECTOUT
}PROTECTSTATU_EM;
typedef enum BRKOUTSTATU{
	OUTSTOP = 0,
	OUTIN,
	OUTCK,
	OUTFAIL,
	OUTCKOK,
	OUTCKFAIL
}BRKOUTSTATU_E;
typedef enum CHZSTATU{
	CHZSTOP = 0,
	CHZIN,
	CHZOUT,
	CHZOUTFAIL,
	CHZCK,
	CHZCKOK,
	CHZCKFAIL,
	HJSTIME
}CHZSTATU_E;
typedef struct CURRENTPRO
{
	ProtectSet Id1;///< I段电流定值（瞬时电流速断保护）, I段时间定值, I段投入定值,I段电流保护出口压板
	ProtectSet Id2;///< II段电流定值（定时限电流速断保护）,II段时间定值,II段投入定值,II段电流保护出口压板
	ProtectSet Id3;///< III段电流定值（定时限过电流保护）,III段时间定值,III段投入定值,III段电流保护出口压板
	ProtectSet I0d1;///< 零序I段电流保护定值I0,零序I段电流保护时间定值,零序I段电流保护投入定值,零序I段电流保护出口压板
	ProtectSet I0d2;///< 零序II段电流保护定值I0,零序II段电流保护时间定值,零序II段电流保护投入定值,零序II段电流保护出口压板
	ProtectSet I0d3;///< 零序III段电流保护定值I0,零序III段电流保护时间定值,零序III段电流保护投入定值,零序III段电流保护出口压板
	ProtectSet U0d;///< 零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
	ProtectDirSet Pdir;///< 电流方向保护投入定值
	ProtectDirSet P0dir;///< 零序电流方向保护投入定值
}CurrentProSets_t;
typedef struct _FaPara
{
	CurrentProSets_t CurrentProSets[MAX_LINE_NUM];
	DataType3  d_5049;   //线路1~8保护定值区号设置
	DataType3  d_504A;   //线路9~16保护定值区号设置
	RecloseSet d_504B;///<重合闸设置
	FaFunSet   d_504C;///<FA功能设置参数
	FaFunSet   d_504F;///<FA功能设置参数
	//DataType1  		d_5052;///<线路1开关定值区号设置
	//DataType1  		d_5053;///<线路2开关定值区号设置

	DataType2 d_5096;///<CT
	DataType1 d_5052;///<线路1开关定值区号设置
	DataType1 d_5053;///<线路2开关定值区号设置
	FaCommSet  d_5054;//启动电流，失压阀值，故障复归时间；
	FaDevotionSet d_5055;///<保护功能投退设置

	DataType3 d_5056;   //故障信号复归
	/*DataType3 d_5057;   //故障信号关联出口（1~4回）
	DataType3 d_5058;   //故障信号关联出口（5~8回）
	DataType3 d_5059;   //故障信号关联出口（9~12回）
	DataType3 d_505A;   //故障信号关联出口（13~16回）
	DataType3 d_505B;   //故障信号关联出口（17~20回）*/
	uint8 OutPort[20];
	DataType3 d_505C;   //故障信号关联出口功能允许
	PassWdPara d_5216;   //终端类型,三遥型：D30；二遥标准型：D21；二遥动作型：D22
	//////////自定义/////////////////
	BASEPROSTATU_T OneShotStatu;
	float NoShutOffCurr[MAX_LINE_NUM];///<非遮断电流

}FaPara;


class GetCplAngle{
	public:

	float GetAngle (SP_COMPLEX CplData){
		float result;
		if ( fabs(CplData.rel) <=0.01 ) {
			if ( CplData.img > 0 ) {
				result = PI / 2.0; //90°
			} else {
				result = -PI / 2.0; // -90°
			}
		} else {
			if ( fabs(CplData.img) <=0.01 ) {
				if ( CplData.rel > 0 ) {
					result = 0;
				} else {
					result = PI;
				}
			} else {
				result = atan2sp ( CplData.img,CplData.rel );
			}
		}
		return result;
	}
};

//////////////////////////////////保护的一些基本单元类//////////
/*
 * 动作公式： -90-a <= arg(U/I) <=90 - a
 * a 为功率方向继电器内角；
 也就是矢量 U ，I 的除数矢量旋转a (逆时针); 此时只要看他的实数部分是否大于零来决定他在
 1 2相限还是2,3相限；
 */
//1功率方向计算
//公式：0.866(Ur*Ir+Ui*Ii) -0.5(Ui*Ir -Ur*Ii)
class DirecBaseCalc:public GetCplAngle{
public:
       int16 Fun(SP_COMPLEX U,SP_COMPLEX I,float cosvalue,float sinvalue,int8 dir){
    	   float  ret = 0;
    	 //  float diffangle = 0;
    	   ret = ((cosvalue*(U.rel*I.rel + U.img*I.img)) -  (sinvalue*(U.img*I.rel - U.rel*I.img)));
    	  // diffangle =(( GetAngle(U) - GetAngle(I) )/PI)*180.0;
    	   if (ret>=0){
    		   if(dir==ZHENGXIANG)
    		   return UNLOCK;
    		   else
    		   return LOCK;
    	   }else{
    		   if(dir==ZHENGXIANG)
			   return LOCK;
			   else
			   return UNLOCK;
    	   }

    	  // return ret ;
       };
};
class PowerDirect :private DirecBaseCalc{
	public:
	SP_COMPLEX *Ubc;
	SP_COMPLEX *Ia;
	SP_COMPLEX *Uca;
	SP_COMPLEX *Ib;
	SP_COMPLEX *Uab;
	SP_COMPLEX *Ic;
	SP_COMPLEX *U0 ;
	SP_COMPLEX *I0;
	float Ulimit ;
	float Ilimit ;
	float *Ua;
	float *Ub;
	float *Uc;
	float *Uzero;
	float *ia;
	float *ib;
	float *ic;
	float *i0;
	float cosa[2];//相间与零序功率方向角余弦值
	float sina[2];//相间与零序功率方向角正弦值
	int16 LockStatu[2];//相间与零序功率方向闭锁状态；
	int16 Phase[4];//分项功率方向闭锁状态；
	int16 Enable[4];
	int8 dir;
	int8 dir0;
	void PdFun(void){
		if(Enable[0]==1){
			if((*Ua<Ulimit)||(*ia<Ilimit)){//约2.5V
				Phase[0] = UNLOCK;
			}else{
				Phase[0] = Fun(*Ubc,*Ia,cosa[0],sina[0],dir);
			}

		}else{
			Phase[0] = UNLOCK;
		}
		if(Enable[1]==1){
			if((*Ub<Ulimit)||(*ib<Ilimit)){//约2.5V
				Phase[1] = UNLOCK;
			}else{
				Phase[1] = Fun(*Uca,*Ib,cosa[0],sina[0],dir);
			}
		}else{
			Phase[1] = UNLOCK;
		}
		if(Enable[2]==1){
			if((*Uc<Ulimit)||(*ic<Ilimit)){//约2.5V
				Phase[2] = UNLOCK;
			}else{
				Phase[2] = Fun(*Uab,*Ic,cosa[0],sina[0],dir);
			}
		}else{
			Phase[2] = UNLOCK;
		}

		if(Enable[3]==1){
			if((*Uzero<Ulimit)||(*i0<Ilimit)){//约2.5V
				Phase[3] = UNLOCK;
			}else{
				Phase[3] = Fun(*U0,*I0,cosa[1],sina[1],dir0);
			}
		}else{
			Phase[3] = UNLOCK;
		}
		LockStatu[0] = Phase[0]+Phase[1]+Phase[2];
		LockStatu[1] = Phase[3];
	}
};


/*
 * 比较大小单元
 */
class HighCurrent{
public:
	uint16 report_data;//soe防抖 后的状态；  此处0为故障1为恢复
	bool HighFun(float InData, float Dz)
	{
		if (InData>Dz){
			return 1;
		}else
			return 0;
	};
};


class CurrentCheck :public HighCurrent{
	public:
	float *I;//输入电流
	float Id;//电流定值

	uint32 Td[2];//定时限/后加速时限
	uint16 Enable;//使能
	//uint16 StartChz;//是否启动重合闸
	int16 *GlfxFlag;
	BASEPROSTATU_T Statu;// 时间状态
	int16 GjYx;
	int8 state;// 保护所处状态
	uint16 SoeId[2];//soe数据Id  定时限/后加速时限 SOEid
	uint16 SoeNature[2];//soe数据Id  定时限/后加速时限 SOEid

	uint32 SoeT01;//告警状态0变成1 时间定值
	uint32 SoeT10;//告警状态1变成0 时间定值;
	BASEPROSTATU_T SoeStatu;//SOE 时间状态
	uint8 SoeFlag;
	bool SoeIRF;//告警产生标志位，注意读取后清零；
	CurrentCheck()
	{
		 Statu.CurrentBool = 0;
		 Statu.CurrentMs1 = 0;
		 Statu.CurrentMs2 = 0;
		 Statu.Trigger = 0;
		 Statu.BASEPROSTATU::CurrentBool = 0;
		 Statu.BASEPROSTATU::Trigger = 0;
		 state= 0;
		 report_data= _SYSFEN_;
		 GjYx= _SYSFEN_;
		 SoeStatu.CurrentBool  =_SYSFEN_;//SOE 时间状态
		 SoeStatu.CurrentMs1 = 0;
		 SoeStatu.CurrentMs2 = 0;
		 SoeStatu.Trigger = 0;
		 SoeStatu.BASEPROSTATU::CurrentBool = _SYSFEN_;
		 SoeStatu.BASEPROSTATU::CurrentMs1 =0;
		 SoeStatu.BASEPROSTATU::CurrentMs2 =0;
		 SoeStatu.BASEPROSTATU::Trigger = 0;
		 SoeFlag= 0;
	     SoeIRF= 0;
	 	 SoeT01 = 0;//告警状态0变成1 时间定值
	 	 SoeT10 = 0;//告警状态1变成0 时间定值;
	}
	// 输入实时时间，后加速标志，输出电流告警所处不同的状态
	//PROTECTSTOP = 0,
	//PROTECTIN,
	//PROTECTOUT
	int16 fun(uint64 SysMs,uint16 HjsFlag)
	{
		state = PROTECTSTOP;
		if(Enable==0){
			//if(DefaultType==DEFAULTRESET){
				if (report_data == _SYSHE_){
					SoeFlag = 0;
					SoeIRF = TRUE;                                      //变位标志
					report_data =_SYSFEN_;
				}
			//}
			GjYx = _SYSFEN_;
			return state;
		}

		//大于定值并达到时限时 出段，超过这个时限，入段中，任何时刻小于定值 属于段外，清临时告警状态GjYx = 0；
#ifdef GLFX
		if((HighFun(*I,Id))&&((*GlfxFlag)!=1)){
#else
			if((HighFun(*I,Id))){
#endif
			if (T_tool.TCell(1,Td[HjsFlag],&Statu)){
				state = PROTECTOUT;                                   //出段
				T_tool.TCell(0,Td[HjsFlag],&Statu);                    //状态清零
				GjYx = _SYSHE_;
			}else{
				state = PROTECTIN;                                    //入段
			}
		}else{
			GjYx = _SYSFEN_;
			state = T_tool.TCell(0,Td[HjsFlag],&Statu);                //段外
		}

		if(T_tool.Plus01_T1_1_10_T2_0(GjYx,SoeT01,SoeT10,&SoeStatu)==_SYSFEN_){//输出 _SYSFEN_ 故障恢复
			if (SoeFlag==1){
				SoeFlag = 0;
				SoeIRF = TRUE;                                      //变位标志
			}
			report_data = _SYSFEN_;                                     //软遥信值，

		}else{                                                      //输出  _SYSHE_ 故障发生
			if (SoeFlag==0){
				SoeFlag = 1;
				SoeIRF = TRUE;                                      //变位标志
			}
			report_data = _SYSHE_;										//软遥信值，
		}
		return state;
	};

	bool ReadSoeIRF(void){                                              //变位标志读取，读取后自动清零,
		if (SoeIRF){											        //所以查看此状态就知道软遥信是否发生过变化
			SoeIRF = FALSE;
			return TRUE;
		}else{
			return FALSE;
		}
	};

};
#define SSGZDEFAULT 1//瞬时故障复归
#define GDHFDEFAULT 2//供电恢复复归
#define ELSEDEFAULT 3//瞬时故障复归
#define NOGZ 0
#define PHA 0x01
#define GRD_I 0x02
#define GRD_U 0x04

class GzCheck :public HighCurrent{
	public:
	float *I;//输入电流
	float Ino;//无流定值
	bool PowerStatu;//供电状态；
	uint16 DefaultType;//故障类型 1瞬时故障，2:永久故障 ;3 其他；
	uint16 ManDefault;//手动复归；
	uint16 Enable;//使能

	uint8 GzYx;//告警SOE瞬时值，没有防抖前的值；此处0为故障1为恢复；
	uint32 Tdefault;//延时复归时间
	uint32 Tinstantaneour;////瞬时故障判断时间
	uint16 GzSoeId[6];//
	uint16 GzSoeNature[6];//
	uint16 GzTypeFlag;//[3];//相间短路/接地故障(电流，电压)
	uint16 GzSoeData;//
	uint16 GzStatu;//永久/瞬时
	BASEPROSTATU_T InstantaneourStatu;// 瞬时故障判断时间寄存器
	BASEPROSTATU_T DefaultStatu;// 故障恢复时间状态
	BASEPROSTATU_T GjStatu;// 瞬时故障判断时间寄存器
	BASEPROSTATU_T PhGjStatu[2];
	BASEPROSTATU_T PzGjStatu[2];
	BASEPROSTATU_T PzIGjStatu[2];
	BASEPROSTATU_T PzUGjStatu[2];
	BASEPROSTATU_T GzSoeStatu;//SOE 时间状态
	uint8 GzSoeFlag;
	bool GzSoeIRF;//告警产生标志位，注意读取后清零；
		GzCheck(){
		InstantaneourStatu.CurrentBool = 0;
		InstantaneourStatu.CurrentMs1 = 0;
		InstantaneourStatu.CurrentMs2 = 0;
		InstantaneourStatu.Trigger = 0;
		InstantaneourStatu.BASEPROSTATU::CurrentBool = 0;
		InstantaneourStatu.BASEPROSTATU::Trigger = 0;
		DefaultStatu.CurrentBool = 0;
		DefaultStatu.CurrentMs1 = 0;
		DefaultStatu.CurrentMs2 = 0;
		DefaultStatu.Trigger = 0;
		DefaultStatu.BASEPROSTATU::CurrentBool = 0;
		DefaultStatu.BASEPROSTATU::Trigger = 0;

		GjStatu.CurrentBool = 0;
		GjStatu.CurrentMs1 = 0;
		GjStatu.CurrentMs2 = 0;
		GjStatu.Trigger = 0;
		GjStatu.BASEPROSTATU::CurrentBool = 0;
		GjStatu.BASEPROSTATU::Trigger = 0;
		int i;
		for (i = 0 ; i < 2 ; i++){
			PhGjStatu[i].CurrentBool = 0;
			PhGjStatu[i].CurrentMs1 = 0;
			PhGjStatu[i].CurrentMs2 = 0;
			PhGjStatu[i].Trigger = 0;
			PhGjStatu[i].BASEPROSTATU::CurrentBool = 0;
			PhGjStatu[i].BASEPROSTATU::Trigger = 0;

			PzGjStatu[i].CurrentBool = 0;
			PzGjStatu[i].CurrentMs1 = 0;
			PzGjStatu[i].CurrentMs2 = 0;
			PzGjStatu[i].Trigger = 0;
			PzGjStatu[i].BASEPROSTATU::CurrentBool = 0;
			PzGjStatu[i].BASEPROSTATU::Trigger = 0;

			PzIGjStatu[i].CurrentBool = 0;
			PzIGjStatu[i].CurrentMs1 = 0;
			PzIGjStatu[i].CurrentMs2 = 0;
			PzIGjStatu[i].Trigger = 0;
			PzIGjStatu[i].BASEPROSTATU::CurrentBool = 0;
			PzIGjStatu[i].BASEPROSTATU::Trigger = 0;

			PzUGjStatu[i].CurrentBool = 0;
			PzUGjStatu[i].CurrentMs1 = 0;
			PzUGjStatu[i].CurrentMs2 = 0;
			PzUGjStatu[i].Trigger = 0;
			PzUGjStatu[i].BASEPROSTATU::CurrentBool = 0;
			PzUGjStatu[i].BASEPROSTATU::Trigger = 0;
		}

		GzStatu= 0;
		GzSoeData= _SYSFEN_;
		GzYx= _SYSFEN_;
		GzSoeStatu.CurrentBool  =_SYSFEN_;//SOE 时间状态
		GzSoeStatu.CurrentMs1 = 0;
		GzSoeStatu.CurrentMs2 = 0;
		GzSoeStatu.Trigger = 0;
		GzSoeStatu.BASEPROSTATU::CurrentBool = _SYSFEN_;
		GzSoeStatu.BASEPROSTATU::CurrentMs1 =0;
		GzSoeStatu.BASEPROSTATU::CurrentMs2 =0;
		GzSoeStatu.BASEPROSTATU::Trigger = 0;
		GzSoeFlag= 0;
		GzSoeIRF= 0;
		ManDefault = 0;
		GzTypeFlag = NOGZ;
		//Trigger(1,Tinstantaneour,NULL,&InstantaneourStatu);
	}
	// 输入实时时间，后加速标志，输出电流告警所处不同的状态
	//PROTECTSTOP = 0,
	//PROTECTIN,
	//PROTECTOUT
	/*
	 *PhA	A相电流相间短路告警
	 *PhB	B相电流相间短路告警
	 *PhC	C相电流相间短路告警
	 *PhZi	零序电流故障告警
	 *Phzu	零序电压故障告警
	 * */
	int16 fun(uint64 SysMs,uint16 PhA,uint16 PhB,uint16 PhC,uint16 PhZi,uint16 PhZu)
	{

#if 0
		uint16 GzType;
		uint16 GjYxInData;
//相间短路故障
		GjYxInData = _SYSFEN_;
		if((PhA==_SYSHE_)||(PhB==_SYSHE_)||(PhC==_SYSHE_)){
			GjYxInData = _SYSHE_;
		}
		if(Trigger(GjYxInData==_SYSHE_,1,SysMs,&PhGjStatu[0])){
			SaveProSoe(GzSoeId[0],_SYSHE_);
		}
		if(Trigger(GjYxInData==_SYSFEN_,1,SysMs,&PhGjStatu[1])){
			SaveProSoe(GzSoeId[0],_SYSFEN_);
		}
/*
//单相接地故障信号
		GjYxInData = _SYSFEN_;
		if((PhZi==_SYSHE_)||(PhZu==_SYSHE_)){
			GjYxInData = _SYSHE_;
		}
		if(Trigger(GjYxInData==_SYSHE_,1,SysMs,&PzGjStatu[0])){
			SaveProSoe(GzSoeId[1],_SYSHE_);
		}
		if(Trigger(GjYxInData==_SYSFEN_,1,SysMs,&PzGjStatu[1])){
			SaveProSoe(GzSoeId[1],_SYSFEN_);
		}
*/
//零序电流故障信号
		if(Trigger(PhZi==_SYSHE_,1,SysMs,&PzIGjStatu[0])){
			SaveProSoe(GzSoeId[5],_SYSHE_);
		}
		if(Trigger(PhZi==_SYSFEN_,1,SysMs,&PzIGjStatu[1])){
			SaveProSoe(GzSoeId[5],_SYSFEN_);
		}
//零序电压故障信号
		if(Trigger(PhZu==_SYSHE_,1,SysMs,&PzUGjStatu[0])){
			SaveProSoe(GzSoeId[4],_SYSHE_);
		}
		if(Trigger(PhZu==_SYSFEN_,1,SysMs,&PzUGjStatu[1])){
			SaveProSoe(GzSoeId[4],_SYSFEN_);
		}

		GjYxInData = _SYSFEN_;
		GzType = NOGZ;
		if((PhA==_SYSHE_)||(PhB==_SYSHE_)||(PhC==_SYSHE_)){
			GjYxInData = _SYSHE_;
			GzType = PHA;
		}
		if(PhZi==_SYSHE_){
			GjYxInData = _SYSHE_;
			GzType+=GRD_I;
		}

		if(PhZu==_SYSHE_){
			GjYxInData = _SYSHE_;
			GzType+=GRD_U;
		}
		if(GzTypeFlag != GzType){
			if(GzStatu == DEFAULTRESET){
				Trigger(0,1,SysMs,&GjStatu);
			}
		}
		GzTypeFlag = GzType;
		if(Trigger(GjYxInData==_SYSHE_,1,SysMs,&GjStatu)){
			Trigger(0,Tinstantaneour,SysMs,&InstantaneourStatu);
		}
		//瞬时永久故障判别
		PowerStatu = HighFun(*I,Ino);
		if(Trigger(1,Tinstantaneour,SysMs,&InstantaneourStatu)){
			if((PowerStatu)){                                   //恢复供电   瞬时故障
				if(GzStatu == DEFAULTRESET){
					GzStatu = INSTANTANEOUR;
					GzYx = _SYSHE_;
					SaveProSoe(GzSoeId[2],GzYx);
					Trigger(0,Tdefault,SysMs,&DefaultStatu);
				}
			}else{                                              //没有恢复供电    永久故障
				if(GzStatu == DEFAULTRESET){
					GzStatu = FOREVER;
					GzYx = _SYSHE_;
					SaveProSoe(GzSoeId[3],GzYx);
				}
			}
		}
		//故障复归类型判别
		if(GzStatu!= DEFAULTRESET){                             //进入复归阶段
			if(ManDefault!=1){                                  // 手动复归
				ManDefault = 0;
				GzYx = _SYSFEN_;
				if(GzStatu==INSTANTANEOUR){
					SaveProSoe(GzSoeId[2],GzYx);
				}else{
					SaveProSoe(GzSoeId[3],GzYx);
				}
				GzStatu = DEFAULTRESET;
				Trigger(0,Tdefault,SysMs,&DefaultStatu);        //复归时间开始清零；
			}
			if(GzStatu== INSTANTANEOUR){                        //瞬时故障的复归      不管什么类型都是一样的；
				if(Trigger(1,Tdefault,SysMs,&DefaultStatu)){
					GzStatu = DEFAULTRESET;
					GzYx = _SYSFEN_;
					 //瞬时故障
					 //线路i瞬时故障信号
					SaveProSoe(GzSoeId[2],GzYx);

				}
			}else {                                               //永久故障的复归；
				if(DefaultType==SSGZDEFAULT){                     //瞬时故障复归---->等待人工复归

				}else if(DefaultType==GDHFDEFAULT){               //供电恢复复归

					if((PowerStatu)){                            //恢复供电   瞬时故障
						if(Trigger(1,Tdefault,SysMs,&DefaultStatu)){
							GzStatu = DEFAULTRESET;
							GzYx = _SYSFEN_;
							SaveProSoe(GzSoeId[3],GzYx);
						}
					}else{                                         //没有恢复供电    永久故障
						Trigger(0,Tdefault,SysMs,&DefaultStatu);   //复归时间开始清零；
					}
				}else if(DefaultType==ELSEDEFAULT){                //其他---->等待人工复归

				}
			}
		}
#endif
		return 1;

	};
	void CheckKey(uint16 KeyData){
		ManDefault = KeyData;
	}
};

class ActionOut {
public:

	uint32 Td;//跳闸返校时间；
	set_ms_time Statu;
	uint16 IRF;
	uint16 BreakNum;
	uint16 ActionType;
	uint16 SoeNum;
	int8 state;
	uint16 ChzQd;
	uint16 ActEnable[3][2];
	uint16 SoeId[3][2];//soe数据Id
	uint16 report_data[3][2];//soe防抖 后的状态；
	uint8 BhYx[3][2];//告警SOE瞬时值，没有防抖前的值；
	uint32 SoeT01[3][2];//告警状态0变成1 时间定值
	uint32 SoeT10[3][2];//告警状态1变成0 时间定值
	BASEPROSTATU_T SoeStatu[3][2];//SOE 时间状态
	uint8 SoeFlag[3][2];
	uint8 SoeIRF[3][2];//告警产生标志位，注意读取后清零；
	uint16 MaskEnable[3][2];
	int16 (*BreakerAction)(uint16 num,uint8 type);
	int16 (*BreakerStatus)(uint16 num,uint8 type);
	ActionOut(){
		uint8 i;
		uint8 j;
		printf("ActionOut Creat \n");
		Td = 0;//跳闸返校时间；
		Statu.Psystime = NULL;
		Statu.SetFlag = 0;
		Statu.T = 0;
		IRF =0 ;
		BreakNum =0 ;
		ActionType =0 ;
		SoeNum =0 ;
		state =0 ;
		for ( i = 0; i < 3 ; i++){
			for( j = 0 ; j< 2;j++){
				ActEnable[i][j] = 0;
				SoeId[i][j]  = 0;
				report_data[i][j]  = _SYSFEN_;
				BhYx[i][j]  = _SYSFEN_;
				SoeT01[i][j]  = 10;
				SoeT10[i][j]  = 10;
				SoeStatu[i][j] .CurrentBool  =0;
				SoeStatu[i][j] .CurrentMs1 = 0;
				SoeStatu[i][j] .CurrentMs2 = 0;
				SoeStatu[i][j] .Trigger = 0;
				SoeStatu[i][j] .BASEPROSTATU::CurrentBool = 0;
				SoeStatu[i][j] .BASEPROSTATU::CurrentMs1 = 0;
				SoeStatu[i][j] .BASEPROSTATU::Trigger = 0;
				SoeFlag[i][j]  = 0;
				SoeIRF[i][j]  = 0;//告警产生标志位，注意读取后清零；
			}
		}
	};
	//输入实时时间 ， 告警遥信的状态，决定是否跳闸，输出不同的状态；
	//OUTSTOP = 0,
	//OUTIN,
	//OUTCK,
	//OUTFAIL,
	//OUTCKOK,
	//OUTCKFAIL
	int16 fun(uint64 SysMs ,uint16 BhNum,uint16 BhType ,uint16 GjYxInData)
	{
		int16 ConterStatu;
		if (ActEnable[BhNum][BhType] == 1){
			if(GjYxInData==_SYSHE_){//有故障信号
				if(T_tool.StartCount(&Statu,Td)==1){//启动返校时间，
					BhYx[BhNum][BhType] = _SYSHE_;   //记录第一个启动定时器成功的保护号，
					ChzQd = MaskEnable[BhNum][BhType];
				}
				//BhYx[BhNum][BhType] = _SYSHE_;   //记录第一个启动定时器成功的保护号，
				if (BreakerAction(BreakNum,ActionType)==1){
					state = OUTIN;
				}else {//跳闸失败
					state = OUTFAIL;
				}
			}else{
				BhYx[BhNum][BhType]= _SYSFEN_;
				state = OUTSTOP;
			}
			ConterStatu = T_tool.CheckCount(&Statu);

			if (ConterStatu==RUNING){
				if((BreakerStatus(BreakNum,CHECKFEN)==1)&&(state != OUTFAIL)){
					state = OUTCKOK;
					T_tool.StopCount(&Statu);
				}
			}else if(ConterStatu==REACHED){
					state = OUTCKFAIL;
					T_tool.StopCount(&Statu);
			}
			if(T_tool.Plus01_T1_1_10_T2_0(BhYx[BhNum][BhType],SoeT01[BhNum][BhType],SoeT10[BhNum][BhType],&SoeStatu[BhNum][BhType])==_SYSHE_){
				if (SoeFlag[BhNum][BhType]==0){
					SoeFlag[BhNum][BhType] = 1;
					SoeIRF[BhNum][BhType]= TRUE;
				}
				report_data[BhNum][BhType] = _SYSHE_;
			}else{
				if (SoeFlag[BhNum][BhType]==1){
					SoeFlag[BhNum][BhType] = 0;
					SoeIRF[BhNum][BhType] = TRUE;
				}
				report_data[BhNum][BhType] = _SYSFEN_;
			}
		}
		return state;
	};
	bool ReadSoeIRF(uint16 BhNum,uint16 BhType){
		if (SoeIRF[BhNum][BhType]==1){
			SoeIRF[BhNum][BhType] = FALSE;
			return TRUE;
		}else{
			return FALSE;
		}
	};
};

//4重合闸类
class ChzOut :public Counter{
public:
	int8  Times;///<重合闸次数
	int32 Time[3];///<三次重合闸时间
	int8  HjsEnable;///<后加速允许
	int32 LockTime;///<闭锁2次重合延时
	uint16 breakernum;
	uint16 CHZtype;
	uint32 OutCheckT;
	int8 state;
	//uint16 MaskEnable;
	uint16 ChzEnable;
	uint8 StartFlag;
	set_ms_time chztime;
	set_ms_time chzcktime;
	set_ms_time hjsetime;
	uint8 ChzCn;
	uint16 HjsLock;
	int16(*BreakerAction)(uint16 num,uint8 type);
	int16(*BreakerStatus)(uint16 num,uint8 type);
	int16(*BreakCloseCheck)(uint8 ChzType,uint8 Enable);
	uint8 *he;
	uint8 *fen;
	int16 manyhstatu;
	ChzOut(){
		 state = 0;
		// ChzEnable= 0;
		 StartFlag= 0;
		 ChzCn= 0;
		 HjsLock= 0;
		 chztime.Psystime = NULL;
		 chztime.SetFlag = 0;
		 chztime.T = 0;
		 chzcktime.Psystime = NULL;
		 chzcktime.SetFlag = 0;
		 chzcktime.T = 0;
		 hjsetime.Psystime = NULL;
		 hjsetime.SetFlag = 0;
		 hjsetime.T = 0;
	}
	//手合断路器监测
	int16 CheckManYh(void){
		if((*he==1)||(*fen==0)){
			if(manyhstatu==0){
				manyhstatu =1;
				return 1;
			}
			return -1;
		}else{
			manyhstatu =0;
			return -1;
		}
	}
	//输入实时时间，断路器跳闸所处状态，是否启动重合闸 ，输出重合闸所处的状态；
	//CHZSTOP = 0,
	//CHZIN,
	//CHZOUT,
	//CHZOUTFAIL,
	//CHZCK,
	//CHZCKOK,
	//CHZCKFAIL,
	//HJSTIME
	int16 Fun(uint64 SysMs,uint16 GzStatu,uint16 ChzQd)
	{
		int16 conterstatu;
		state = CHZSTOP;
		if(CheckManYh()==1){
			T_tool.StartCount(&hjsetime,LockTime);           //手合故障线路后加速
			HjsLock = LOCK;
		}
		if(HjsLock==LOCK){
			conterstatu = T_tool.CheckCount(&hjsetime);
			if (conterstatu!=RUNING){
				HjsLock = UNLOCK;
			}
			state = HJSTIME;
			return state;
		}
		if((ChzQd== DISENABLE)||(ChzEnable == DISENABLE)){
			state = CHZSTOP;
			return CHZSTOP;
		}
		if(GzStatu == OUTCKOK){
			T_tool.StartCount(&chztime,Time[ChzCn]);               //启动重合闸计时
		}
		conterstatu = T_tool.CheckCount(&chztime);
		if (conterstatu==REACHED){
			if (BreakCloseCheck(CHZtype,1)==1){
				if (BreakerAction(breakernum,CLOSEBREAKER)==1){
					state = CHZOUT;
					T_tool.StartCount(&chzcktime,OutCheckT);       //重合闸
					return state;
				}else{
					state = CHZOUTFAIL;
				}
			}else{
				state = CHZOUTFAIL;
			}
		}else if (conterstatu==RUNING){
			state = CHZIN;
		}

		conterstatu = T_tool.CheckCount(&chzcktime);              //重合闸返校
		if (conterstatu==RUNING){
			state = CHZCK;
			if(BreakerStatus(breakernum,CHECKHE)==1){
					//合闸成功
				state = CHZCKOK;
				ChzCn++;
				if(ChzCn>=Times){
					ChzCn = 0;
					ChzEnable = 0;
					T_tool.StopCount(&chztime);
					T_tool.StopCount(&chzcktime);
				}
				T_tool.StartCount(&hjsetime,LockTime);             // 重合闸后，启动后加速监测事件吗，此时间内闭锁二次重合闸，
				HjsLock = LOCK;
				T_tool.StopCount(&chzcktime);                     //合闸出口返校及时停止；
				return state;
			}
		}else if (conterstatu==REACHED){
			state = CHZCKFAIL;
		}else if(state == CHZOUTFAIL){
			state = CHZCKFAIL;
		}else{//SLEEP

		}

		conterstatu = T_tool.CheckCount(&hjsetime);
		if (conterstatu==RUNING){
			state = HJSTIME;
			HjsLock = LOCK;
		}else{
			HjsLock = UNLOCK;
		}
		return state;
	}

};
#define RATIO(line,Phase) YcRatio[line].PortRatio[Phase]
class FA :public BaseApp{
private: 
	uint16 timer_id;///<内部定时器id
#ifdef GLFX
	PowerDirect GlfxHandle[MAX_LINE_NUM];
#endif

	uint16 HjsFlag;
	uint64 SysMs;
#ifdef BHCK
	ActionOut ActionHandle[MAX_LINE_NUM];
	ChzOut ChzHandle[MAX_LINE_NUM];
	int16 gzstatu[MAX_LINE_NUM];
	int16 chzstatu[MAX_LINE_NUM];
#endif
public:
	FaPara	fa_cfg;///配置参数
	FA();
	~FA(){};
	CurrentCheck CurrenHandle[MAX_LINE_NUM][3];//相间短路故障
	CurrentCheck GrdCurrenHandle[MAX_LINE_NUM];//电流接地故障
	CurrentCheck GrdVoltHansdle[MAX_LINE_NUM];
	GzCheck  GzCheckHandle[MAX_LINE_NUM];      //瞬时/永久故障 判别，故障复归；
	int16 FixTimeProtect(void);
	float Imax[MAX_LINE_NUM];
	float I0[MAX_LINE_NUM];
	float U0[MAX_LINE_NUM];
	float Fmax(float a,float b,float c){
		float ret = 0;
		if(a>b){
			if(a>c){
				ret = a;
			}else{
				ret = c;
			}
		}else{
			if(b>c){
				ret = b;
			}else{
				ret = c;
			}
		}
		return ret;
	}
	float GetMax(float ia,float ib,float ic,uint8 line)
	{
		float ret = 0;
		if(ia>ib){
			if(ia>ic){
				ret = ia*RATIO(line,IA12)*4.0;
			}else{
				ret = ic*RATIO(line,IC12)*4.0;
			}
		}else{
			if(ib>ic){
				ret = ib*RATIO(line,IB12)*4.0;
			}else{
				ret = ic*RATIO(line,IC12)*4.0;
			}
		}
		return ret;
	};
	float GeYcMax(float ia,float ib,float ic,uint8 line)
	{
		float ret = 0;
		if(ia>ib){
			if(ia>ic){
				ret = ia*RATIO(line,IA11)*4.0;
			}else{
				ret = ic*RATIO(line,IC11)*4.0;
			}
		}else{
			if(ib>ic){
				ret = ib*RATIO(line,IB11)*4.0;
			}else{
				ret = ic*RATIO(line,IC11)*4.0;
			}
		}
		return ret;
	};
	int16 GetParaId(int8 SecNum,int8 LineNum);
	int16 ReadCfg(int32 DataId);
	int16 Init();
	void ParaInit(void);
	void ParaChange(void);
	void FunHandleInit(void);
    void HandleInit(void);
	int16 Run();	

};

extern FA fa;


/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* fa_main(void*);
#endif
