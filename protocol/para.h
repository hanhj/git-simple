/*
	���ݣ����屣���õĲ���
 */
#ifndef _PARA__H_
#define _PARA__H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct GDJHHYB{
   unsigned int losepower:1;
   unsigned int get_volt:1;
   unsigned int bi_loss_volt:1;
   unsigned int bi_get_volt:1;
   unsigned int rsvd:12;
}GDJHHYB;

typedef union gd_jhhyb{

  unsigned int all;
  GDJHHYB bit;

}gdjhhyb;

//������ʱ��
typedef struct protect_timer {
    unsigned int start_flag;// ����--1���ر�--0��
    //unsigned int over_flag;// 1--over,0--counting
    unsigned int jdtime;//��բʧ�ܼ��ʱ��
    unsigned int timer_cnt;
    unsigned int timer_limit;
} PROTECT_TIMER;

typedef struct bzt {
    gdjhhyb enable;
    unsigned int lock;
    PROTECT_TIMER losepower;
    PROTECT_TIMER get_volt_timer;
    PROTECT_TIMER bi_loss_volt_timer;
    PROTECT_TIMER bi_get_volt_timer;

} BZT;
extern BZT BztPara[2];

typedef struct gdjhh {

    float losepower_timer;
    float get_volt_timer;
    float bi_loss_volt_timer;
    float bi_get_volt_timer;
	gdjhhyb enable;

} GDJHH;
/***************Խ�޵������ֵ*********/
typedef struct max_and_min {
	float upper_limit;                  //����
	float lower_limit;                  //����
}BOUNDS;

typedef struct boundyb{
	unsigned int	ua:1;
	unsigned int	ub:1;
	unsigned int	uc:1;
	unsigned int	ia:1;
	unsigned int	ib:1;
	unsigned int	ic:1;
	unsigned int	i0:1;
	unsigned int	p:1;
	unsigned int	q:1;
	unsigned int	cos:1;
	unsigned int	s:1;
	unsigned int    freq:1;

}BOUNDYB;

typedef union YUEXIANYB{

  unsigned int all;
  BOUNDYB bit;

}YUEXIANYB;


typedef struct para_bounds{
	 BOUNDS  ua;
	 BOUNDS  ub;
	 BOUNDS  uc;
	 BOUNDS  ia;
	 BOUNDS  ib;
	 BOUNDS  ic;
	 BOUNDS  i0;
	 BOUNDS  p;
	 BOUNDS  q;
	 BOUNDS  cos;
	 BOUNDS  s;
	 BOUNDS  freq;
	 float t ;                   //����ʱ��
	 YUEXIANYB enable;           //Խ�޸澯ѹ��


} PARABOUNDS;

/******************************������������***********************/
typedef struct ctrword{
	unsigned short en :8;       //0:��ʱ�ޣ�1:��ʱ��
	unsigned short out:8;  		//��ʱ�޶�ֵ��� 1: 2: 3: 4:

}CTRWD;
//typedef
//__attribute__ ((_packed_))
typedef struct i_protect{
	float i_limit;             //������ֵ
	int32 t_limit ;            //ʱ��
	struct{
		uint16 enable;         //�澯ѹ��
		uint16 enable_out;      // ����ѹ��
		uint16 dir_enable;      //����ѹ��
		uint16 ulose_enable;    //��ѹ����ѹ��
	}Enable;
}IPROTECT;

typedef struct jyprotect{
	float i_limit;              //
	//int32 t_limit ;           //
	int32 det_t;                //�ۼƴ���ʱ�䣻
	struct{
		uint16 enable;          //�澯ѹ��
		uint16 enable_out;       //����ѹ��
		uint16 dir_enable;       //����ѹ��
		uint16 ulose_enable;     //��ѹ����ѹ��
	}Enable;
}JYPROTECT;
typedef struct angprotect{
	uint16 rever_line;          //Ͷ�뷴���ֶ��жϷ���ʱ���á�
	float Edz;					//�綯�ƶ�ֵ
	float limit;                //�Ƕȶ�ֵ
	int32 t_limit;              //ʱ�䶨ֵ
	float i0_lock_down;			//������λ�����������ޣ�Ŀ���Ƿ�ֹ���������С
	float u0_lock_down;			//���򱣻���ѹ������ֵ���ޣ�Ŀ���Ƿ�ֹ�����ѹ��С
	struct{
		uint16 enable;          //�澯ѹ��
		uint16 auto_judge_dir;  //�Զ�������ѹ��
		uint16 enable_out;       //����ѹ��
		uint16 dir_enable;       //����ѹ��
		uint16 ulose_enable;     //��ѹ����ѹ��
	}Enable;
}ANGPROTECT;
/*************************���ʷ���������*************************/
typedef struct pir_protect{
	unsigned int ContralWord;    //���ʷ�������֣�
	unsigned int Dir ;
	unsigned int Pdirvalue;      //���ʷ���Ƕȣ�0-30��1-45
}PIRPROTECT;
/*****************************�غ�բ����***************************/

#define MAX_CHZ_NUM 3
typedef struct chz_protect{
	int16  enable[3];		// 0 �������غ�բ��1 ��������غ�բ��3 ������λ�غ�բ
	int16  Times;			//<�غ�բ����
	int16  Type;			//<�غ�բ��ʽ
	int32  cdtime;			//���ʱ�䣻
	int32 Time[3];			//<�����غ�բʱ��

	int32 LockTime;			//����2���غ���ʱ
	float DiffVol;			//���������ѹ��
	float DiffPhase;		//��λ������
	int16 DiffVolDefine[2];	//���������ѹ����

}CHZPROTECT;
/*****************************�����������õĶ�ֵ***************************/
typedef struct pub_value{
	float StartI;				//��������,����ʹ��
	float StartU;				//��ѹ
	float StopU;				//��ѹ������ʹ��
	int32 InstantFaultChkTime;	//˲ʱ�����ж�ʱ��
	int32 ResetFaTime;			//��ʱ����ʱ��
	int16 ResetFaType;			//���ϸ�������  1��ʱ�临�� 2�ָ����縴�� 3�ֶ�����
	int16 GroundLinkType;		//1С�����ӵ�2 ������ӵ�
}PUBVALUE;

typedef   struct AllEnableSet{
    int16 EnableYCEvent;		//ң��Խ�޼�⹦��Ͷ��
    int16 EnableCurrentProtect; //������������Ͷ��
    int16 EnableFa;             //FA����Ͷ��
    int16 EnablePtFun;          //��PT����ʱ�Ƿ��Զ��˳�����Ԫ���Լ���ͬ��Ԫ��,FA����
}AllEnableSet ;
/********************************�����������ֵ*********************/
typedef struct protect_limit{
	 IPROTECT I_d1;				//�ٶϵ����������ܲ���
	 IPROTECT I_d2;      	    //���������������ܲ���
	 IPROTECT I_d3;      	    //�����ɵ����������ܲ���
	 IPROTECT I0_d1;     	    //�����ٶϵ����������ܲ���
	 IPROTECT I0_d2;     	    //����ʱ�޵����������ܲ���
	 IPROTECT I0_d3;     	    //��������ɵ����������ܲ���
	 JYPROTECT JY;              //���������Ծ����
	 IPROTECT U0;				//�����ѹ������ֵU0,�����ѹ����Ͷ�붨ֵ,�����ѹ��������ѹ��
	 IPROTECT Ihjs;
	 IPROTECT I0hjs;
	 IPROTECT I0PhaseHjs;
	 uint32 hjschecktime;
	 CHZPROTECT chz;            //�غ�բ���ܲ������ܲ���
	 ANGPROTECT Angle_dz;       //������λ��������	 
	 ANGPROTECT Raise_dz;       //����������������	 
	 float U0_Lock;				//���򱣻���ѹ������ֵ
	 uint16 pt_vol_lock;		//PT���߱��������ѹ
	 PIRPROTECT P_direction;    //���ʷ���Ԫ������
	 PIRPROTECT P0_direction;   //���ʷ���Ԫ������
	 float P0_coff;				//p0ϵ����for 3U0 or U0 ;3I0 or I0 ,default is 3U0 3I0
} PROTECTLIMIT;

/***************ϵͳ����,Խ�޶�ֵ*********/
typedef struct total_limit_para{
	PROTECTLIMIT  Para_limit[MAX_LINE_NUM][MAX_SECTION_NUM]; //�����������ֵ116*2*6
	int16 ctlock;					//CT���߱������򱣻���
	uint16 totalenable;				// ��������ѹ�壻
	uint16 undefine;				//δ���� ����λ�ã�
	AllEnableSet protect_enable;
	PUBVALUE public_value;			//һЩ���õĶ���//8]
	uint16 U0_source;				//0 �ɼ���1������
	uint16 I0_source;				//0�ɼ���1������
	uint32 GZDefaulte;				//1 �����飻
	uint16 section_num;				//��ֵ����
	//unsigned long DefaultTime;
	uint16 tort[MAX_LINE_NUM];      //���߹��ʼ��㷽����2��/3����
	int temp_lmt;                   //�¶ȶ�ֵ
	float pt_one_set;				//pt�һ��ֵ
	float pt_two_set;				//pt�����ֵ
	float ct_one_set;				//ct�һ��ֵ
	float ct_two_set;				//ct�����ֵ
	float ct0_one_set;				//����ct�һ��ֵ
	float ct0_two_set;				//����ct�����ֵ
	float pt0_one_set;				//����pt�һ��ֵ
	float pt0_two_set;				//����pt�����ֵ
	float ct_accuracy;				//ct����
	float ct_low_accuracy;			//ct�������޾���
	float ct_low_ratio;				//ct�������ޱ���
	
	///*��ѹ���޸澯��ֵ���ָ���ֵ������ʱ��*/
	limt UupData;
	///*��ѹ���޸澯��ֵ���ָ���ֵ������ʱ��*/
	limt UdownData;
	///*����������ֵ���ָ���ֵ������ʱ��*/
	limt IupData;
	///*����Խ������ֵ���ָ���ֵ������ʱ��*/
	limt SupData;
	//�����ѹ���޸澯����
	limt U0upData;
	//����������޸澯����
	limt I0upData;
	uint16 cur_unit;
	uint16 min_unit;
	uint16 max_unit;
	uint16 reverse[17];//Ԥ��
	uint16 crc;
} TOTAL_LIMIT_BOUNDS;
extern TOTAL_LIMIT_BOUNDS LimitBounds;

//===================================================================
#define HARMONIC_RANK 15
#define LINE_FUHE_DATA_NUM 12
#define S_YX 1
#define D_YX 2

typedef struct block_para_tab1 {
    Uint16 dataaddr;
    char unitName[5];
    Uint16 datatype;		//��������
    Uint16 len;				//��Ϊ��λ
    Uint16 *ppara;			//����

    Uint16 secperlen;		//��ƫ����
    Uint16 *Para;			//�������ڵ����ݽṹ
	int16 (*ParaInit)(void);
}BLOCK_PARA_TABLE1;


#define SELECT 1
#define NO_SELECT 0 

typedef BLOCK_PARA_TABLE1 DB;
extern const int ParaNum ;//all para num
extern DB*	g_db;
/***************************************************************************************/
/****************************EXTERN FUNCTIONS*******************************************/
extern short  get_block_para ( unsigned short datasign, unsigned char *buf );
extern short  set_block_para ( unsigned short datasign, unsigned char *buf );
extern short  deal_para ( uint8 *buf,uint8 *outbuff,uint8 R_W);
extern short fuhe_flag[4];
extern void TotalSummon_yc(void);


#ifdef __cplusplus
}
#endif
/***************************************************************************************/
/***************************************************************************************/
#endif
