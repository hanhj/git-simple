/*
 * Scada.h
	���ݣ�ң�����ݣ��������صĲ��������ݽṹ
 *
 *  Created on: 2015-2-4
 *      Author: Administrator
 */

#ifndef SCADA_H_
#define SCADA_H_
#include "datatype.h"
#include "clock.h"
#define CHANAL_ONE 0
#define CHANAL_TWO 1
#define CHANAL_THREE 2
#define LINE1 0
#define LINE2 1
#define LINE(n) (n)
#define PHASE_A 0
#define PHASE_B 1
#define PHASE_C 2
#define TOTALLPHASE 3

#define BDSTEP_1 0x2000
#define BDSTART  1
#define BDSTOP   0
#define BD_0K  14
#define BD_FAIL 15
#define BD_TIMEOVER 15
#define CLBRTTIMEOUT 10
#define DEVXISHU 0.02209375   // 1/(32*����2��
#define TWODEVXISHU 0.00048828125 // 1/(32*����2��* 1/(32*����2��
#define AVERAGENUM 10.0
#define BIAODINGMIN 100.0


#define SAMP_NUM 32
#define PAD 0
#define MAXYCNUM 26
#define MAXSTATUBORDNUM 4
//����UIPQ����Чֵ�����õ���г������
#define XIEBOLEN 14 //1  ��ʾ���������14    ��ʾ����14��г����������ֱ����  

#define NO_YC 0xffff
#define LINE(n) (n)
//���ʼ������Ӧ2PT��3CT��2PT��2CT��1PT��2CT��1PT��3CT��//�����Ӧ
#define Pt_3 1  //3PT��3CT
#define Pt_Ua 1 //1PT��3CT��
#define Pt_Ub 2 //1PT��3CT��
#define Pt_Uc 3 //1PT��3CT��
#define Pt_2 0 //2PT��3CT��2PT��2CT��
#define Pt_Uab 5//1PT��2CT��
#define Pt_Ucb 6//1PT��2CT��
//2PT��3CT��2PT��2CT��1PT��2CT��1PT��3CT��3PT��3CT


#define  NOMINAL_U LimitBounds.pt_two_set //���ѹ
#define  NOMINAL_I LimitBounds.ct_two_set //�����
#define  NOMINAL_S (NOMINAL_U * NOMINAL_I)
#define  NOMINAL_TOTALS (NOMINAL_S*3)
#define U300RATIO 0.02763//100ohm sample register
//#define I10RATIO  0.0003380//100ohm sample register
#define I10RATIO  0.00004971//680ohm sample register

typedef enum ADCP{
	UA1 = 0,
	UB1,			//1
	UC1,			//2
	UAB1,			//3
	UCB1,			//4
	UCA1,			//5
	U01,			//6
	IA11,			//7
	IB11,			//8
	IC11,			//9
	I011,			//10
	negI1,//�������//11
	posI1,//�������//12
	IA12,			//13
	IB12,
	IC12,
	UA2,
	UB2,
	UC2,
	UAB2,
	UCB2,
	UCA2,
	U02,
	IA21,
	IB21,
	IC21,
	I021,
}JCENUM ;

typedef enum YCDATAENUM{
	YcDataUA = 0,
	YcDataUB,
	YcDataUC,
	YcDataUAB,
	YcDataUCB,
	YcDataUCA,
	YcDataU0,
	YcDataIA,
	YcDataIB,
	YcDataIC,
	YcDataI0,
	YcDataPOW,
	YcDataQ,
	YcDataS,
	YcDataCOS,
	YcDataFreq,
}YCDATAENUM ;
#define PERLINEYCDATANUN 16
#define  YcData_UA(n) (YcDataUA+PERLINEYCDATANUN*n)
#define  YcData_UB(n) (YcDataUB+PERLINEYCDATANUN*n)
#define  YcData_UC(n) (YcDataUC+PERLINEYCDATANUN*n)
#define  YcData_UAB(n) (YcDataUAB+PERLINEYCDATANUN*n)
#define  YcData_UCB(n) (YcDataUCB+PERLINEYCDATANUN*n)
#define  YcData_UCA(n) (YcDataUCA+PERLINEYCDATANUN*n)
#define  YcData_U0(n) (YcDataU0+PERLINEYCDATANUN*n)
#define  YcData_IA(n) (YcDataIA+PERLINEYCDATANUN*n)
#define  YcData_IB(n) (YcDataIB+PERLINEYCDATANUN*n)
#define  YcData_IC(n) (YcDataIC+PERLINEYCDATANUN*n)
#define  YcData_I0(n) (YcDataI0+PERLINEYCDATANUN*n)
//#define  YcData_ia(n) (YcDataia+PERLINEYCDATANUN*n)
//#define  YcData_ib(n) (YcDataib+PERLINEYCDATANUN*n)
//#define  YcData_ic(n) (YcDataic+PERLINEYCDATANUN*n)
#define  YcData_P(n) (YcDataPOW+PERLINEYCDATANUN*n)
#define  YcData_Q(n) (YcDataQ+PERLINEYCDATANUN*n)
#define  YcData_S(n) (YcDataS+PERLINEYCDATANUN*n)
#define  YcData_COS(n) (YcDataCOS+PERLINEYCDATANUN*n)
#define  YcData_Freq(n) (YcDataFreq+PERLINEYCDATANUN*n)



#define UA(n)  (UA1+UA2*n)
#define UB(n)  (UB1+UA2*n)
#define UC(n)  (UC1+UA2*n)
#define UAB(n) (UAB1+UA2*n)
#define UCB(n) (UCB1+UA2*n)
#define UAC(n) (UCA1+UA2*n)
#define U0(n)  (U01+UA2*n)
#define IA1(n) (IA11+UA2*n)
#define IB1(n) (IB11+UA2*n)
#define IC1(n) (IC11+UA2*n)
#define I01(n) (I011+UA2*n)
//#define IA2(n) (IA12+UA2*n)
//#define IB2(n) (IB12+UA2*n)
//#define IC2(n) (IC12+UA2*n)



typedef struct AdConfig{
	uint16 name[MAXYCNUM];//����װ�ص������ú��ң�����޼������
	uint16 ad_compass[MAXYCNUM];//װ�ص�ͨ����ţ������0xffff�Ĵ���˽�����û�ж�Ӧ��Ӳ��ͨ�����±�Ϊң����
	uint8 fpga_board[MAXSTATUBORDNUM];// 0����ҪӲ��֧�֣�1��ҪӲ��֧��
	uint8 yc_num;//ʵ�����õ�ң��������
}ADCONFIG;
extern ADCONFIG yc_configure;

typedef struct scada_yc{
	//ң��������
	float ua1_4001;	///<��·1 A���ѹ
	float ub1_4002;	///<��·1 B���ѹ
	float uc1_4003;	///<��·1 C���ѹ
	float uab1_4004;	///<��·1 AB�ߵ�ѹ
	float ucb1_4005;	///<��·1 CB�ߵ�ѹ
	float uca1;
	float u011_4006;	///<��·1 �����ѹ
	float ia11_4007;	///<��·1 A���������
	float ib11_4008;	///<��·1 B���������
	float ic11_4009;	///<��·1 C���������
	float i011_400A;	///<��·1 �������
	float P1_4017;	///<��·1 ���й�����
	float Q1_4018;	///<��·1 ���޹�����
	float S1_4019;	///<��·1 �����ڹ���
	float COS1_401A;	///<��·1 �ܹ�������
	float freq1_401B;	///<��·1 Ƶ��
//	float ia12_400B;	///<��·1 A�ౣ������
//	float ib12_400C;	///<��·1 B�ౣ������
//	float ic12_400D;	///<��·1 C�ౣ������
	float Ip;//���������
	float In;//���������
	float pa1_400E;	///<��·1 A���й�����
	float qa1_400F;	///<��·1 A���޹�����
	float cosa1_4010;	///<��·1 A�๦������
	float pb1_4011;	///<��·1 B���й�����
	float qb1_4012;	///<��·1 B���޹�����
	float cosb1_4013;	///<��·1 B�๦������
	float pc1_4014;	///<��·1 C���й�����
	float qc1_4015;	///<��·1 C���޹�����
	float cosc1_4016;	///<��·1 C�๦������

	float Up;//����
	float Un;//����
	
	float pos_P	;//�����й�
	float neg_P	;//�����й�
	float pos_Q	;//�����޹�
	float neg_Q	;//�����޹�

}scada_yc_t;



typedef struct ptctdata{

	float *ua,*ub,*uc,*u0,*u1,*u2;
	float *ia,*ib,*ic,*i0,*i1,*i2;

	int pt_flag;
	int ct_flag;
	setmstime_t pt_check_cnt;
	setmstime_t pt_recover_cnt;
	setmstime_t ct_check_cnt;
	setmstime_t ct_recover_cnt;
	uint16 limitT; //check time 

	int sys_type;
	int pt_type;
	float no_voltage;
	float have_voltage;
	float no_current;
	float nominal_vol;
	float nominal_cur;

}ptctdata_t;
typedef struct SIGRCD{
	float smpT;//�������ڣ�ms
	float smpFreq;//����Ƶ��
	float difFreq;//��ǰƵ�������Ƶ�ʲ�ֵ��
	uint16 smpadjcnt;//����Ƶ�ʵ�������
	float T;//�ź�����
	float Freq;//�ź�Ƶ��ʵʱ

}SIGRCD;


typedef struct power{
	float p;
	float q;
	float s;
}power_t;
typedef struct anglevalue{
	float sin_a;
	float cos_a;
}anglevalue_t;
typedef struct harmonicdata{
	float rms;
	float rario;
}harmonicdata_t;

typedef struct harmonictab{
	float *Rms;
	float *Ratio;
	float *TotalRatio;
	uint16 YcName;
}harmonictab_t;
typedef struct maxavrmin{
    float max;
	float avr;
	float min;
}maxavrmin_t;

typedef struct maxavrminpower{
	power_t max;
	power_t avr;
	power_t min;
}maxavrminpower_t;
typedef struct softyxtab{
	float *SrcYcP;
	limt *LimitParaP;
	uint8 LimitType;
	uint8 Enable;
	setmstime_t Tcn[2];//Խ��ʱ����Խ�߻ָ�ʱ�䣻
	uint8 ChangeFlag;
}softyxtab_t;
typedef struct _PowerWay{
	uint8 line[MAX_LINE_NUM];
}PowerWay;
//extern ratio_t YcRatio;
#define YUEXIANYXNUM (10)
#define SHANGXIAN 1
#define XIAXIAN 2
typedef struct  _ScadaPara
{
	//////////����Ϊ�Զ���/////////
	float		pt;//�������
	float		ct;
	float		ct0;
	float		pt0;

	scada_yc_t YcData;
	harmonictab_t HmncDataP[MAX_LINE_NUM][6];
	PowerWay		d_508B;///<���ʼ��㷽������
	setmstime_t BatActTime;

	float32 Modulus[MAXYCNUM][SAMP_NUM/2+1];//ֱ������������... N��г��
	float32 RFFTPhaseBuff[MAXYCNUM][SAMP_NUM/2+1]; //  N��г����λ

	ratio_t  YcRatioTmp[MAX_LINE_NUM];
	ratio_t  StandYc[MAX_LINE_NUM];
	maxavrmin_t ClbrtTmp[MAXYCNUM];
	maxavrminpower_t ClbrtPowerTmp[MAX_LINE_NUM][4];
	uint8 Clbrtnum;
	uint8 StartClbrt;
	setmstime_t ClbrtTime;
	setmstime_t looptime;
	softyxtab_t YueXianYx[YUEXIANYXNUM];


	limt UupData;///*520a��ѹ���޸澯��ֵ���ָ���ֵ������ʱ��*/
	limt UdownData;///*520b��ѹ���޸澯��ֵ���ָ���ֵ������ʱ��*/
	limt IupData;///*520c����������ֵ���ָ���ֵ������ʱ��*/
	limt SupData; ///*520d����Խ������ֵ���ָ���ֵ������ʱ��*/
	limt U0upData;//�����ѹ���޸澯����
	limt I0upData; //����������޸澯����
	limt LoseVol;
}ScadaPara;

class Scada {
public:
	ScadaPara	scada_cfg;///<Scada���ò���
	ptctdata_t PtCtReg[MAX_LINE_NUM];
	SIGRCD FreqReg[MAX_LINE_NUM];
	float MaxVol[MAX_LINE_NUM];
	Counter TCount;
	Scada();

	virtual ~Scada();
	void run(void);
	void Caculate(void);
	void  CheckDeadData(uint16 calltype);
	int16 Init();
	int16 YueXianYxInit(void);
	void PtCtParaInit();
	float CaculateXU(int8_t flag,Complex*d1,Complex*d2,Complex*d3)	;
	Complex CaculateXU2(int8_t flag,Complex*d1,Complex*d2,Complex*d3)	;
private:

	harmonicdata_t GetHarmonicRmsAndRatio(float BaseHarmonicRms,float *src);
	void GetHarmonicData(void);
	void PtAdapt(void);
	power_t   GetPower(uint8 Uname,uint8 Iname);
	void PhaseChange( Complex *dir, Complex *src , uint16 len ,float offsetangle );
	Complex ComplexRotation(Complex Input,float32 Angle);
	int16 FloatCompare ( float cmp1, float cmp2 );
	int16 IsOverDeadband ( float value1, float value2, float db );
	void compare(float *max,float *min,float *avr,float var,float avr_num);
	void first_compare(float *max,float *min,float var);
	void GetXishu(float *xishu, float *sourceyc,float *standardyc,uint8 minvalue );
	Complex TypeChange(float rel,float img);
	float TotalRms(float *src , uint8 len);
	void  GetModulus(void);
	void  GetRms(void);
	void  GetPQS(void);
	float GetCplAngle(Complex CplData);
	void GetFreq(int port );
	float CaculateFreq(int16 *src , uint8 len, uint8 line);

	void CheckPtCt(void);
	void ChecksoftYx(void);
	void  Calibration(void);
	void  Filter(void);
	float GeFloatMax(float ia,float ib,float ic)
	{
		float ret = 0;
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

};
extern Scada scada;
extern int16 ScadaParaInit(void);
#endif /* SCADA_H_ */
