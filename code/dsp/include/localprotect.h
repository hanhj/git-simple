/*
 * localprotect.h
 *
 *  Created on: 2013-11-27
 *      Author: Administrator
 */

#ifndef LOCALPROTECT_H_
#define LOCALPROTECT_H_
//#include "fa.h"
//#include "datatype.h" ///<用到数据类型定义
//#include "interruptapp.h"
//#include "clock.h"
//#include "dspdatatype.h"
//#include "fft.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include "fpga.h"
#include "fa.h"
#include <stdio.h>
#include <string.h>
extern int16 ProtectOut(uint16 breakernum, uint8 type);
typedef enum TOOLNAME{
D_BaseTool = 0,
D_EQUAL ,
D_NOT ,
D_AND ,
D_NAND,
D_OR ,
D_XOR ,
D_XNOR ,
D_STEADY,
D_HIGHANALOG ,
D_LOWANALOG ,
D_COMPARE ,
D_ANALOGSTEADY,
D_DelayStart ,
D_Pulse,
D_DelayStartDelay ,
D_StartConter,
D_StopConter,
D_ConterReached,
D_ConterRuning,
D_ConterSleep,
D_NewLogicTool,
D_bool,
D_int8,
D_uint8,
D_int16,
D_uint16,
D_int32,
D_uint32,
D_float,
D_uin64
}TOOLNAME_E;

#define NEEDNEW 0

typedef struct BaseReg{
 bool *pA;
 bool *pB;
}BaseReg_t;
typedef struct Int8Reg{
 int8 *pA;
 int8 *pB;
}Int8Reg_t;
typedef struct HighLowReg{
	 float *pA;
	 float *pB;
}HighLowReg_t;
typedef struct RegionReg{
	 float *pA;
	 float *pB;
	 float Dz;
	 float Region;
	 bool dir;
}RegionReg_t;
typedef struct TriggerReg{
	bool *PA;
	int32 Td1;
	int32 Td2;
	uint64 *sysms;
}TriggerReg_t;
typedef struct ConterReg{
	bool *pA;
	int32 Td;
	uint64 *sysms;
	uint8 Id;
	setmstime_t *RegTimer;
}ConterReg_t;

typedef struct TYPEP{
	Int8Reg_t Int8_Reg;
	BaseReg_t Base_Reg;
	HighLowReg_t HighLow_Reg;
	RegionReg_t  Region_Reg;
	TriggerReg_t Trigger_Reg;
	ConterReg_t Conter_Reg;
}TYPEP_T;
typedef struct JOINDATA{
	uint8 Name;
	uint8 Num;
	TYPEP_T InputPara;
}JOINDATA_t;
typedef struct NODEP{
	uint8 Num;
}NODEP_t;
typedef struct NODEDATA{
	JOINDATA_t Local;
	NODEP_t Node[MAX_INNODE_NUM];
	uint8 NodeNumber;
}NODEDATA_t;
typedef struct SbaseCfg{
	uint8 Name;
	uint8 Num;
	uint8 Node[MAX_INNODE_NUM];
	uint8 NodeNumber;
}SbaseCfg_t;

typedef struct LOCKSTATU{
	int8 openlock;//闭锁跳闸状态；
	int8 closlock;//闭锁合闸状态；
	int8 LsPwEnb;//失电跳闸闭锁检测； 0 使能打开，
	int8 BiGtPwEnb;//单侧得电合闸闭锁监测； 0 使能打开
	int8 U0Enb;//U0保护跳闸闭锁监测； 0 使能打开
	int8 IUEnb;// 和故障线路跳闸闭锁监测； 0 使能打开
	int8 BiLsPwEnb;//失电合闸闭锁监测； 0 使能打开
}LOCKSTATU_t;


class Nullbase{
	public:
	int8  Name;
	bool *A;
	bool *B;
	bool C;
	bool fun(){
		return 0;
	}
};
//////////////////////
class BaseTool{
	public:
	bool *A;
	bool *B;
	bool C;
	//NODEDATA_t CfgReg;
	virtual void Config(NODEDATA_t ModleCfg){
	//	CfgReg = ModleCfg;
	//	if(CfgReg.NodeNumber > MAX_INNODE_NUM){
	//		CfgReg.NodeNumber = 0;
	//	}
	};
	virtual bool fun(){
		return C;
	};
};

class EQUAL:public BaseTool{
public:
	int8 * A;

	EQUAL(){//( Int8Reg_t reg){
	       A = NULL;// reg.pA;

	   	C =0;
	};
	void Config(NODEDATA_t ModleCfg){
	       A = ModleCfg.Local.InputPara.Int8_Reg.pA;
		 //  B = reg.pB;
	   	C =0;
	};
	bool fun ()
	{
		if (*A==1){
			C = 1;
			return 1;
		}else{
			C = 0;
			return 0;
		}
		//C = (*A);
		//return C;
	}
};
#define EQUALTABNUM (20*MAX_LINE_NUM)  //16
class EQUALTAB{
public:
	EQUALTAB(){
		used = 0;
	}
	EQUAL *GetLab(){
		if (used<EQUALTABNUM){
			return &tab[used++];
		}else {
			printf("EQUALTAB NO USRED!");
			return NULL;
		}

	}
private:
	EQUAL tab[EQUALTABNUM];
	uint16 used;
};
extern EQUALTAB EqualTab ;

class NOT:public BaseTool{//非
public:
	NOT(){//( BaseReg_t reg){
	      // A = reg.pA;
		  // B = reg.pB;
			//C =0;
	};
	void Config(NODEDATA_t ModleCfg){
		 A = ModleCfg.Local.InputPara.Base_Reg.pA;
		// B = ModleCfg.Local.InputPara.Base_Reg.pB;
		C =0;
	}
	bool fun ()
	{
		C = (!(*A));
		return C;
	}
};
#define NOTTABNUM (5*MAX_LINE_NUM) //4
class NOTTAB{
public:
	NOTTAB(){
		used = 0;
	}
	NOT *GetLab(){
		if (used<NOTTABNUM){
			return &tab[used++];
		}else {
			printf("NOTTAB NO USRED!");
			return NULL;
		}

	}
private:
	NOT tab[NOTTABNUM];
	uint16 used;
};
extern NOTTAB NotTab ;

class AND :public BaseTool{//与
public:
	AND(){//( BaseReg_t reg){
	      // A = reg.pA;
		 //  B = reg.pB;
		//	C =0;
	};
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.Base_Reg.pA;
			 B = ModleCfg.Local.InputPara.Base_Reg.pB;
			C =0;
	};
	bool fun ()
	{
		C=(*A)&&(*B);
		return C;
	}
};
#define ANDTABNUM (35*MAX_LINE_NUM) //29
class ANDTAB{
public:
	ANDTAB(){
		used = 0;
	}
	AND *GetLab(){
		if (used<ANDTABNUM){
			return &tab[used++];
		}else {
			printf("ANDTAB NO USRED!");
			return NULL;
		}

	}
private:
	AND tab[ANDTABNUM];
	uint16 used;
};
extern ANDTAB AndTab ;

class NAND:public BaseTool{//与非
public:
	NAND(){//( BaseReg_t reg){
	    //   A = reg.pA;
		//   B = reg.pB;
		//C =0;
	};
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.Base_Reg.pA;
			 B = ModleCfg.Local.InputPara.Base_Reg.pB;
			C =0;
	};
	bool fun ()
	{
		C=!(*A)&&(*B);
		return C;
	}
};
#define NANDTABNUM (1*MAX_LINE_NUM) //0
class NANDTAB{
public:
	NANDTAB(){
		used = 0;
	}
	NAND *GetLab(){
		if (used<NANDTABNUM){
			return &tab[used++];
		}else {
			printf("NANDTAB NO USRED!");
			return NULL;
		}

	}
private:
	NAND tab[NANDTABNUM];
	uint16 used;
};
extern NANDTAB NandTab ;

class OR:public BaseTool{//或
public:
	OR(){//( BaseReg_t reg){
	      // A = reg.pA;
		  // B = reg.pB;
		  // C =0;
	}
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.Base_Reg.pA;
			 B = ModleCfg.Local.InputPara.Base_Reg.pB;
			C =0;
	};
	bool fun ()
	{
		C=(*A)||(*B);
		return C;

	}
};
#define ORTABNUM (9*MAX_LINE_NUM) //2
class ORTAB{
public:
	ORTAB(){
		used = 0;
	}
	OR*GetLab(){
		if (used<ORTABNUM){
			return &tab[used++];
		}else {
			printf("ORTAB NO USRED!");
			return NULL;
		}

	}
private:
	OR tab[ORTABNUM];
	uint16 used;
};
extern ORTAB OrTab ;

class XOR:public BaseTool{
public:
	XOR(){//( BaseReg_t reg){
	      // A = reg.pA;
		  // B = reg.pB;
		  // C =0;
	}
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.Base_Reg.pA;
			 B = ModleCfg.Local.InputPara.Base_Reg.pB;
			C =0;
	};
	bool fun ()//异或门
	{
		if((*A)==(*B)){
			C = 0;
		}else{
			C = 1;
		}
		return C;
	}
};
#define XORTABNUM (3*MAX_LINE_NUM) //2
class XORTAB{
public:
	XORTAB(){
		used = 0;
	}
	XOR*GetLab(){
		if (used<XORTABNUM){
			return &tab[used++];
		}else {
			printf("XORTAB NO USRED!");
			return NULL;
		}

	}
private:
	XOR tab[XORTABNUM];
	uint16 used;
};
extern XORTAB XorTab ;

class XNOR:public BaseTool{
public:
	XNOR(){//( BaseReg_t reg){
	       //A = reg.pA;
		   //B = reg.pB;
		  // C =0;
	}
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.Base_Reg.pA;
			 B = ModleCfg.Local.InputPara.Base_Reg.pB;
			C =0;
	};
	bool fun ()//同或门
	{
		if((*A)==(*B)){
			C = 1;
		}else{
			C = 0;
		}
		return C;
	}
};
#define XNORTABNUM (1*MAX_LINE_NUM) //0
class XNORTAB{
public:
	XNORTAB(){
		used = 0;
	}
	XNOR*GetLab(){
		if (used<XNORTABNUM){
			return &tab[used++];
		}else {
			printf("XNORTAB NO USRED!");
			return NULL;
		}

	}
private:
	XNOR tab[XNORTABNUM];
	uint16 used;
};
extern XNORTAB XnorTab ;


class STEADY:public BaseTool{// 全为1 时为1，全为0 时为零，其他状态维持不变
public:
	 STEADY(){//(BaseReg_t reg){
		//A = reg.pA;
		//B = reg.pB;
		//C = 0;
	}
	void Config(NODEDATA_t ModleCfg){
				A = ModleCfg.Local.InputPara.Base_Reg.pA;
				B = ModleCfg.Local.InputPara.Base_Reg.pB;
				C =0;
	};
	 bool fun(){
		 if(((*A)==1)&&((*B)==1)){
			 C = 1;
		 }else if(((*A)==0)&&((*B)==0)){
			 C = 0;
		 }
		 return C;
	 }

};
#define STEADYTABNUM (1*MAX_LINE_NUM)//0
class STEADYTAB{
public:
	STEADYTAB(){
		used = 0;
	}
	STEADY*GetLab(){
		if (used<STEADYTABNUM){
			return &tab[used++];
		}else {
			printf("STEADYTAB NO USRED!");
			return NULL;
		}

	}
private:
	STEADY tab[STEADYTABNUM];
	uint16 used;
};
extern STEADYTAB SteadyTab ;
#define PADDR(n)    (*(float*)(n))
class HIGHANALOG:public BaseTool{
public:
	float *A;
  	float *B;
	HIGHANALOG(){//( HighLowReg_t reg){
	      // A =reg.pA;
		   //B =reg.pB;
		  // C = 0;
	}
	void Config(NODEDATA_t ModleCfg){
			 A =(float*)ModleCfg.Local.InputPara.HighLow_Reg.pA;
			 B =(float*)ModleCfg.Local.InputPara.HighLow_Reg.pB;
			C =0;
	};

	bool fun()
	{//	uint8 i;
	//	float a;
	//	float b;
		//uint32 addr;
		/*uint8 *pB;
		uint8 *pb;
		uint8 *pA;
		uint8 *pa;
		pb= (uint8*)&b;
		pB = (uint8*)B;
		pa= (uint8*)&a;
		pA = (uint8*)A;
		for(i = 0;i<4;i++){
			pb[i] = pB[i];
			pa[i] = pA[i];
		}*/
	//	a = (float)*(&A[0]);
	//	b =(*(B));
	//	printf("b=%f\n",b);
	//	printf("a=%f\n",a);

		if (*A>*B){
			C = 1;
		}else{
			C = 0;
		}
		return C;
	}
};
#define HIGHANALOGTABNUM (5*MAX_LINE_NUM) //4
class HIGHANALOGTAB{
public:
	HIGHANALOGTAB(){
		used = 0;
	}
	HIGHANALOG*GetLab(){
		if (used<HIGHANALOGTABNUM){
			return &tab[used++];
		}else {
			printf("HIGHANALOGTAB NO USRED!");
			return NULL;
		}

	}
private:
	HIGHANALOG tab[HIGHANALOGTABNUM];
	uint16 used;
};
extern HIGHANALOGTAB HighanalogTab ;

class LOWANALOG:public BaseTool{
public:

	float *A;
	float *B;
	LOWANALOG(){//( HighLowReg_t reg){
	       //A =reg.pA;
		   //B =reg.pB;
		   //C = 0;
	}
	void Config(NODEDATA_t ModleCfg){
			 A = ModleCfg.Local.InputPara.HighLow_Reg.pA;
			 B = ModleCfg.Local.InputPara.HighLow_Reg.pB;
			 C =0;
	};
	bool fun()
	{

		if((*A)<(*B)){
			C = 1;
		}else
			C = 0;
		return C;
	}
};
#define LOWANALOGTABNUM (5*MAX_LINE_NUM) //4
class LOWANALOGTAB{
public:
	LOWANALOGTAB(){
		used = 0;
	}
	LOWANALOG*GetLab(){
		if (used<LOWANALOGTABNUM){
			return &tab[used++];
		}else {
			printf("LOWANALOGTAB NO USRED!");
			return NULL;
		}

	}
private:
	LOWANALOG tab[LOWANALOGTABNUM];
	uint16 used;
};
extern LOWANALOGTAB LowanalogTab ;

class  COMPARE:public BaseTool{
public:
	float *A;
	float min;
	float max;
	COMPARE(){//( RegionReg_t reg){
	     //  A =reg.pA;
	     //  min = reg.Dz - reg.Region;
	     //  max = reg.Dz + reg.Region;
	     //  C = 0;
	}
	void Config(NODEDATA_t ModleCfg){
		   A =ModleCfg.Local.InputPara.Region_Reg.pA;
		   min = ModleCfg.Local.InputPara.Region_Reg.Dz - ModleCfg.Local.InputPara.Region_Reg.Region;
		   max = ModleCfg.Local.InputPara.Region_Reg.Dz + ModleCfg.Local.InputPara.Region_Reg.Region;
		   C = 0;
	};
	bool fun()
	{
		if((*A)>(min)&&(*A)<(max)){
			C = 1;
		}else{
			C = 0;
		}
		return C;
	}
};
#define COMPARETABNUM (3*MAX_LINE_NUM) //2
class COMPARETAB{
public:
	COMPARETAB(){
		used = 0;
	}
	COMPARE*GetLab(){
		if (used<COMPARETABNUM){
			return &tab[used++];
		}else {
			printf("COMPARETAB NO USRED!");
			return NULL;
		}

	}
private:
	COMPARE tab[COMPARETABNUM];
	uint16 used;
};
extern COMPARETAB CompareTab ;

class ANALOGSTEADY:public BaseTool{
public:
	float *A;
	float *B;
	float Limit;
	bool Dir;
	ANALOGSTEADY(){//(RegionReg_t reg){
		//A =  reg.pA;
		//B = reg.pB;
		//Limit = reg.Dz;
		//Dir = reg.dir;
		//C = 0;
	}
	void Config(NODEDATA_t ModleCfg){
		   A = ModleCfg.Local.InputPara.Region_Reg.pA;
		   B = ModleCfg.Local.InputPara.Region_Reg.pB;
		   Limit = ModleCfg.Local.InputPara.Region_Reg.Dz ;
		   Dir = ModleCfg.Local.InputPara.Region_Reg.dir;
		   C = 0;
	};
	bool fun(){
		if(((*A)>Limit)&&((*B)>Limit)){
			 C = Dir;
		}else if (((*A)<Limit)&&((*B)<Limit)){
			C = !Dir;
		}
		return C;
	}

};
#define ANALOGSTEADYTABNUM (3*MAX_LINE_NUM) //2
class ANALOGSTEADYTAB{
public:
	ANALOGSTEADYTAB(){
		used = 0;
	}
	ANALOGSTEADY*GetLab(){
		if (used<ANALOGSTEADYTABNUM){
			return &tab[used++];
		}else {
			printf("ANALOGSTEADYTAB NO USRED!");
			return NULL;
		}

	}
private:
	ANALOGSTEADY tab[ANALOGSTEADYTABNUM];
	uint16 used;
};
extern ANALOGSTEADYTAB AnalogsteadyTab ;


/*当输入为“0”时，输出为“0”；当输入由“0”变“1”时，
 *经过时间T1延时后，输出为“1”，此前输出一直为“0
 */

class DelayStart:public BaseTool{
public:

	int32 TdMs;
	uint64 *SysMs;
	BASEPROSTATU_T statu;
	DelayStart(){//(TriggerReg_t reg){
		//A = reg.PA;
		//TdMs = reg.Td1;
		//SysMs = reg.sysms;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Trigger_Reg.PA;
		TdMs = ModleCfg.Local.InputPara.Trigger_Reg.Td1;
		SysMs = ModleCfg.Local.InputPara.Trigger_Reg.sysms;
		C = 0;
	};
	bool fun ()
	{
		if(&statu==NULL){
			while(1);
		}
		if (*A){
			if(*SysMs >= statu.CurrentMs){
				statu.CurrentBool = *A;
			}
		}else{
			statu.CurrentMs = *SysMs + TdMs;
			statu.CurrentBool = *A;
		}
		C = statu.CurrentBool ;
		return C ;
	}

};
#define DelayStartTABNUM (8*MAX_LINE_NUM) //6
class DelayStartTAB{
public:
	DelayStartTAB(){
		used = 0;
	}
	DelayStart*GetLab(){
		if (used<DelayStartTABNUM){
			return &tab[used++];
		}else {
			printf("DelayStartTAB NO USRED!");
			return NULL;
		}

	}
private:
	DelayStart tab[DelayStartTABNUM];
	uint16 used;
};
extern 	DelayStartTAB DelayStartTab ;


/*
当输入为“0”时，输出为“0”；输入由“0”变“1”时，
输出为“1”，经过时间T2延时后，输出为“0”。
 */

class Pulse:public BaseTool{
	public:
//	bool *A;
	int32 TdMs;
	uint8 StartFlag;
	uint64 *SysMs;
	BASEPROSTATU_T statu;
	Pulse(){//(TriggerReg_t reg){
		//A = reg.PA;
		//TdMs = reg.Td1;
		//SysMs = reg.sysms;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Trigger_Reg.PA;
		TdMs = ModleCfg.Local.InputPara.Trigger_Reg.Td1;
		if (TdMs==ONESHOT){
			statu.Trigger = 1;
		}
		SysMs = ModleCfg.Local.InputPara.Trigger_Reg.sysms;
		C = 0;
		StartFlag = 0;
	};
		bool fun()
		{
		//	if(statu==NULL){
		//		while(1);
		//	}
			if(statu.Trigger==1){
				if (*A){
					if(StartFlag == 1){//如果 定时赋值为零，会出现一次输出为1的脉冲
						statu.CurrentBool = 1;
						StartFlag = 0;
					}else{
						statu.CurrentBool = StartFlag;//*A;
					}
				}else{
					StartFlag = 1;
					//statu.CurrentMs = *SysMs + TdMs;
					statu.CurrentBool =0;// *A;
				}
				C = statu.CurrentBool;
			}else{
				if (*A){
					if(*SysMs > statu.CurrentMs){//如果 定时赋值为零，会出现一次输出为1的脉冲
						statu.CurrentBool = 0;
					}else{
						statu.CurrentBool = StartFlag;//*A;
					}
				}else{
					StartFlag = 1;
					statu.CurrentMs = *SysMs + TdMs;
					statu.CurrentBool =0;// *A;
				}
				C = statu.CurrentBool;
			}

			return C;
		}
};
#define PulseTABNUM (15*MAX_LINE_NUM) //11
class PulseTAB{
public:
	PulseTAB(){
		used = 0;
	}
	Pulse*GetLab(){
		if (used<PulseTABNUM){
			return &tab[used++];
		}else {
			printf("PulseTAB NO USRED!");
			return NULL;
		}

	}
private:
	Pulse tab[PulseTABNUM];
	uint16 used;
};
extern 	PulseTAB PulseTab ;

/*
当输入为“0”时，输出为“0”；输入由“0”变“1”时，经过时间T1延时后，输出为“1”，此前输出一直为“0”；
当输入由“1”变“0”时，经过时间T2延时后，输出为“0”，此前输出一直为“1”。
*/
class DelayStartDelay:public BaseTool{
	public:
		int32 TdMs1;
		int32 TdMs2;
		uint64 *SysMs;
		BASEPROSTATU2_T statu;
		DelayStartDelay(){//(TriggerReg_t reg){
			//A = reg.PA;
			//TdMs1 = reg.Td1;
			//TdMs2 = reg.Td2;
			//SysMs = reg.sysms;
		}
		void Config(NODEDATA_t ModleCfg){
			A = ModleCfg.Local.InputPara.Trigger_Reg.PA;
			TdMs1 = ModleCfg.Local.InputPara.Trigger_Reg.Td1;
			TdMs2 = ModleCfg.Local.InputPara.Trigger_Reg.Td2;
			SysMs = ModleCfg.Local.InputPara.Trigger_Reg.sysms;
			C = 0;
		};
		bool fun()
		{
		//	if(statu==NULL){
		//		while(1);
		//	}
			if (*A){
				statu.CurrentMs2 = *SysMs + TdMs2 ;
				if(*SysMs >= statu.CurrentMs1){
					statu.CurrentBool = *A;
				}
			}else{
				statu.CurrentMs1 = *SysMs + TdMs1 ;
				if(*SysMs >= statu.CurrentMs2){
					statu.CurrentBool = *A;
				}
			}
			C = statu.CurrentBool;
			return C;
		}
};
#define DelayStartDelayTABNUM (1*MAX_LINE_NUM) //0
class DelayStartDelayTAB{
public:
	DelayStartDelayTAB(){
		used = 0;
	}
	DelayStartDelay*GetLab(){
		if (used<DelayStartDelayTABNUM){
			return &tab[used++];
		}else {
			printf("DelayStartDelayTAB NO USRED!");
			return NULL;
		}

	}
private:
	DelayStartDelay tab[DelayStartDelayTABNUM];
	uint16 used;
};
extern 	DelayStartDelayTAB DelayStartDelayTab ;

class StartConter:public BaseTool{
	public:
		int32 TdMs;
		uint64 *SysMs;
		setmstime_t RTUTimer;
		uint8 ConterId;
		StartConter(){//(ConterReg_t reg){
			//A = reg.pA;
			//TdMs = reg.Id;
			//SysMs = reg.sysms;
			//ConterId = reg.Id;
			//reg.RegTimer = &RTUTimer;
		}
		void Config(NODEDATA_t ModleCfg){

			A = ModleCfg.Local.InputPara.Conter_Reg.pA;
			TdMs = ModleCfg.Local.InputPara.Conter_Reg.Id;
			SysMs = ModleCfg.Local.InputPara.Conter_Reg.sysms;
			ConterId = ModleCfg.Local.InputPara.Conter_Reg.Id;
			ModleCfg.Local.InputPara.Conter_Reg.RegTimer = &RTUTimer;

		};
		bool fun()
		{

			if (*A){//打开
				if (RTUTimer.SetFlag!=1){
					RTUTimer.SetFlag  = 1;
					RTUTimer.T = *SysMs +  TdMs;
					C = 1;
					return C;
				}
				C = 0;
				return C;
			}else{
				C = 0;
				return C;
			}

		};
};
#define StartConterTABNUM (1*MAX_LINE_NUM) //0
class StartConterTAB{
public:
	StartConterTAB(){
		used = 0;
	}
	StartConter*GetLab(){
		if (used<StartConterTABNUM){
			return &tab[used++];
		}else {
			printf("StartConterTAB NO USRED!");
			return NULL;
		}

	}
private:
	StartConter tab[StartConterTABNUM];
	uint16 used;
};
extern 	StartConterTAB StartConterTab ;


class StopConter:public BaseTool{
public:
	setmstime_t *RTUTimer;
	StopConter(){//(ConterReg_t reg){
	//	A = reg.pA;
	//	RTUTimer =reg.RegTimer;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Conter_Reg.pA;
		RTUTimer =ModleCfg.Local.InputPara.Conter_Reg.RegTimer;
	}
	bool fun()
	{  if(*A){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			C = 1;
			return C;
		}else{
			C = 0;
			return C;
		}
	};
};
#define StopConterTABNUM (1*MAX_LINE_NUM) //0
class StopConterTAB{
public:
	StopConterTAB(){
		used = 0;
	}
	StopConter*GetLab(){
		if (used<StopConterTABNUM){
			return &tab[used++];
		}else {
			printf("StopConterTAB NO USRED!");
			return NULL;
		}

	}
private:
	StopConter tab[StopConterTABNUM];
	uint16 used;
};
extern 	StopConterTAB StopConterTab ;

class ConterReached:public BaseTool{
public:
	uint64 *SysMs;
	setmstime_t *RTUTimer;
	ConterReached(){//(ConterReg_t reg){
		//A = reg.pA;
		//RTUTimer = reg.RegTimer;
		//SysMs = reg.sysms;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Conter_Reg.pA;
		RTUTimer =ModleCfg.Local.InputPara.Conter_Reg.RegTimer;
		SysMs = ModleCfg.Local.InputPara.Conter_Reg.sysms;
	}
	bool fun()
	{   if (*A==0){
		    C = 0;
		    return C;
		}
	    if (RTUTimer->SetFlag!=1){//未开始计时
				C = 0;
				return C;
	    }
		if(*SysMs >=RTUTimer->T){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			C = 1;
			return C;
		}else {
			C = 0;
			return C;
		}
	};
};
#define ConterReachedTABNUM (1*MAX_LINE_NUM) //0
class ConterReachedTAB{
public:
	ConterReachedTAB(){
		used = 0;
	}
	ConterReached*GetLab(){
		if (used<ConterReachedTABNUM){
			return &tab[used++];
		}else {
			printf("ConterReachedTAB NO USRED!");
			return NULL;
		}

	}
private:
	ConterReached tab[ConterReachedTABNUM];
	uint16 used;
};
extern 	ConterReachedTAB ConterReachedTab ;

class ConterRuning:public BaseTool{
public:
	uint64 *SysMs;
	setmstime_t *RTUTimer;
	ConterRuning(){//(ConterReg_t reg){
		//A = reg.pA;
		//RTUTimer = reg.RegTimer;
		//SysMs = reg.sysms;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Conter_Reg.pA;
		RTUTimer =ModleCfg.Local.InputPara.Conter_Reg.RegTimer;
		SysMs = ModleCfg.Local.InputPara.Conter_Reg.sysms;
	}
	bool fun()
	{
		if (*A==0){
			C = 0;
			return C;
		}

	    if (RTUTimer->SetFlag!=1){//未开始计时
	    	C = 0;
			return C;
	    }
		if(*SysMs >=RTUTimer->T){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			C = 0;
			return C;
		}else {
			C = 1;
			return C;
		}
	};
};
#define ConterRuningTABNUM (1*MAX_LINE_NUM)//0
class ConterRuningTAB{
public:
	ConterRuningTAB(){
		used = 0;
	}
	ConterRuning*GetLab(){
		if (used<ConterRuningTABNUM){
			return &tab[used++];
		}else {
			printf("ConterRuningTAB NO USRED!");
			return NULL;
		}

	}
private:
	ConterRuning tab[ConterRuningTABNUM];
	uint16 used;
};
extern 	ConterRuningTAB ConterRuningTab ;



class ConterSleep:public BaseTool{
public:
	uint64 *SysMs;
	setmstime_t *RTUTimer;
	ConterSleep(){//(ConterReg_t reg){
		//A = reg.pA;
		//RTUTimer = reg.RegTimer;
		//SysMs = reg.sysms;
	}
	void Config(NODEDATA_t ModleCfg){
		A = ModleCfg.Local.InputPara.Conter_Reg.pA;
		RTUTimer =ModleCfg.Local.InputPara.Conter_Reg.RegTimer;
		SysMs = ModleCfg.Local.InputPara.Conter_Reg.sysms;
	}
	bool fun()
	{
		if (*A==0){
			C = 0;
			return C;
		}
	    if (RTUTimer->SetFlag!=1){//未开始计时
	    	C = 1;
			return C;
	    }
		if(*SysMs >=RTUTimer->T){
			RTUTimer->SetFlag = 0;
			RTUTimer->T = 0;
			C = 0;
			return C;
		}else {
			C = 0;
			return C;
		}
	};
};
#define ConterSleepTABNUM (1*MAX_LINE_NUM) //0
class ConterSleepTAB{
public:
	ConterSleepTAB(){
		used = 0;
	}
	ConterSleep*GetLab(){
		if (used<ConterSleepTABNUM){
			return &tab[used++];
		}else {
			printf("ConterSleepTAB NO USRED!");
			return NULL;
		}

	}
private:
	ConterSleep tab[ConterSleepTABNUM];
	uint16 used;
};
extern 	ConterSleepTAB ConterSleepTab ;

/*
template  <class Tout,class Tin> class NewLogicTool{
public:
	NODEDATA_t CfgReg;
	bool C;
	Tin   *NodeHanle[MAX_INNODE_NUM];
	Tout  *HostHandle;
	NewLogicTool(NODEDATA_t ModleCfg){
		CfgReg = ModleCfg;
		if(CfgReg.NodeNumber > MAX_INNODE_NUM){
			CfgReg.NodeNumber = 0;
		}
	};
	~NewLogicTool(){
	};
	bool fun(){
			uint8 i;
			bool sloop;
			sloop  = NodeHanle[0]->C;
			HostHandle->A = &sloop;
			for(i = 0;i<CfgReg.NodeNumber;i++){
				i++;
				if(i<CfgReg.NodeNumber){
					HostHandle->B = &NodeHanle[i]->C;
				}
				sloop = HostHandle->fun();
			}
			C = sloop;
			return C;
	}
};
*/
template  <class Tout> class NewTool{
public:
	SbaseCfg_t CfgReg;
	bool C;
	Tout  *HostHandle;
	NewTool(){
		//printf("NewTool creat ..\n");
		C = 0;
		HostHandle = NULL;
	};
	~NewTool(){
		//if(HostHandle!=NULL){
		//	printf("NewTool discreat ..\n");
		//	delete HostHandle;
		//	HostHandle = NULL;
		//}
	}
	void Config(NODEDATA_t ModleCfg){
		CfgReg.Name = ModleCfg.Local.Name;
		CfgReg.Num = ModleCfg.Local.Num;
		CfgReg.NodeNumber =  ModleCfg.NodeNumber;
		CfgReg.Node[0] =  ModleCfg.Node[0].Num;
		CfgReg.Node[1] =  ModleCfg.Node[1].Num;
		if(CfgReg.NodeNumber > MAX_INNODE_NUM){
			CfgReg.NodeNumber = 0;
		}
		if(HostHandle!=NULL){
			HostHandle->Config(ModleCfg);
		}
	};
	bool fun(){
		C = HostHandle->fun();
	//	printf("MdNum :%d----Name:%x-----Status:%d\n",CfgReg.Num,CfgReg.Name,C);
		return C;
	}
};
typedef struct FAPARA{
	FaFunSet   		d_504C;///<FA功能设置参数
	FaIsolationSet  d_504D;///<FA功能中分段开关参数
	FaConnectionSet d_504E;///<FA功能中联络开关参数
} __attribute__ ((packed))FAPARA_T;

typedef struct LPPARA{
	/*第一套开关参数*/
	/*FaFunSet   		d_504C;///<FA功能设置参数
	FaIsolationSet  d_504D;///<FA功能中分段开关参数
	FaConnectionSet d_504E;///<FA功能中联络开关参数*/
	/*第二套开关参数*/
	/*FaFunSet   		d_504F;///<FA功能设置参数
	FaIsolationSet  d_5050;///<FA功能中分段开关参数
	FaConnectionSet d_5051;///<FA功能中联络开关参数*/
	FAPARA_T dz[2];
	DataType1  		d_5052;///<线路1开关定值区号设置
	DataType1  		d_5053;///<线路2开关定值区号设置
	FaCommSet	 		d_5054;///<FA功能公用参数设置
}lppara_t;


typedef struct BASEPARA{
	float NoShutOffCurr ;//非遮断电流
    int32 LosePowerTime;///<双侧失电延时时间（T时限）
    int8  EnableOpenSwitch;///<双侧失电延时分闸功能压板   1
    int32 GetPowerTime;///<单侧得电延时时间（X时限）
    int8  EnableCloseSwitch;///<单侧得电延时合闸功能压板  2
    int32 LockCloseSwitchTime1;///<闭锁合闸时间（Y时限）
    int8  EnableLockCloseSwitch1;///<闭锁合闸功能压板      3
    float FaultCurrent;///<闭锁合闸故障电流定值
    int8  EnableChkFaultCurrent;///<闭锁合闸功能中是否检测故障电流功能允许(如果不允许,则只检测无压) 4
    int32 LockOpenSwitchTime;///<无故障闭锁分闸时限
    int32  LockOpenSwitchResetTime;///<无故障闭锁分闸复归时限
    int8  EnableLockOpenSwitch;///<无故障闭锁分闸功能压板 5
    float RemainVol;///<残压定值
    int8  EnableChkRemainVol;///<残压检测功能允许 6
    float LimitU0;///<零序电压保护定值U0
    int32 LimitU0Time;///<零序电压保护时间定值
    int8  EnableU0;///<零序电压保护投入定值  7
	/////
    int32 CloseSwitchOneSideTime;///<单侧失压延时后合闸设定时间（XL时限）
    int8  EnableCloseSwitchOneSide;///<单侧失压延时后合闸功能允许
    int32 LockCloseSwitchTime2;///<双侧得电闭锁开关合闸时间
    int8  EnableLockCloseSwitch2;///<双侧得电闭锁开关合闸功能允许
}BASEPARA_T;


typedef struct USERPATA{
	//

	//
	BASEPARA_T BrkPara[2];
	int8 secnum1;
	int8 secnum2;
	///<FA功能公用参数设置
    float StartI;
    float StartU;
    int32 ResetFaTime;
}USERPARA_t;

class LocalProtect{
public:
	lppara_t lp_cfg;
	USERPARA_t usr_cfg;
	NewTool<BaseTool> *BaseMdHandle;
	NewTool<BaseTool> BaseTabl[MAX_LINE_NUM][100];
	//NewTool<BaseTool>**BaseMdHandle;
	//NewLogicTool<BaseTool,BaseTool>*MainFunHandle;
	NODEDATA_t BaseMd[100];
	uint8 BaseMdNum;
	LOCKSTATU_t  LockStatu[MAX_LINE_NUM];
	uint64 SystermMs;
	int8 HeStatu[MAX_LINE_NUM];
	int8 FenStatu[MAX_LINE_NUM];
	//bool heSta[MAX_LINE_NUM];
	//bool fenSta[MAX_LINE_NUM];
	float Uleft[MAX_LINE_NUM];
	float Uright[MAX_LINE_NUM];
	float Imax[MAX_LINE_NUM];
	float Umax[MAX_LINE_NUM];
	float U0[MAX_LINE_NUM];
	LocalProtect(){
		printf("LocalProtect creat....\n");
		memset(&LockStatu,0,sizeof(LockStatu));
	};
	float GetMax(float ia,float ib,float ic)
	{
		int32 ret = 0;
		if(ia>ib){
			if(ia>ic){
				ret = ia;
			}else{
				ret = ic;
			}
		}else{
			if(ib>ic){
				ret = ib;
			}else{
				ret = ic;
			}
		}
		return ret;
	};
	void ParaInit(void);
	void Paracpy(void);
	void BaseMdInit(void);
	void fun(void);
	void Init();
	int16 GetParaId(int8 SecNum,int8 LineNum);
	int16 ReadCfg(uint32 Id);
private:

};

//////////////////////////
class tttt{
public:
	template <class Type>
	Type GeMax(Type a,Type b)
	{   Type ret;
		ret = a>b?a:b;
		return ret;
	}
};

//extern tttt GET_MAX;
extern LocalProtect lp;
#endif /* LOCALPROTECT_H_ */
