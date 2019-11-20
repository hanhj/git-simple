/*
	内容：定义保护用的参数
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

//保护定时器
typedef struct protect_timer {
    unsigned int start_flag;// 启动--1；关闭--0；
    //unsigned int over_flag;// 1--over,0--counting
    unsigned int jdtime;//跳闸失败检测时间
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
/***************越限电参量定值*********/
typedef struct max_and_min {
	float upper_limit;                  //上限
	float lower_limit;                  //下限
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
	 float t ;                   //报警时限
	 YUEXIANYB enable;           //越限告警压板


} PARABOUNDS;

/******************************电流保护数据***********************/
typedef struct ctrword{
	unsigned short en :8;       //0:定时限，1:反时限
	unsigned short out:8;  		//反时限定值类别 1: 2: 3: 4:

}CTRWD;
//typedef
//__attribute__ ((_packed_))
typedef struct i_protect{
	float i_limit;             //电流定值
	int32 t_limit ;            //时限
	struct{
		uint16 enable;         //告警压板
		uint16 enable_out;      // 出口压板
		uint16 dir_enable;      //方向压板
		uint16 ulose_enable;    //电压闭锁压板
	}Enable;
}IPROTECT;

typedef struct jyprotect{
	float i_limit;              //
	//int32 t_limit ;           //
	int32 det_t;                //累计窗口时间；
	struct{
		uint16 enable;          //告警压板
		uint16 enable_out;       //出口压板
		uint16 dir_enable;       //方向压板
		uint16 ulose_enable;     //电压闭锁压板
	}Enable;
}JYPROTECT;
typedef struct angprotect{
	uint16 rever_line;          //投入反向。手动判断方向时有用。
	float Edz;					//电动势定值
	float limit;                //角度定值
	int32 t_limit;              //时间定值
	float i0_lock_down;			//零序相位保护电流下限，目的是防止零序电流过小
	float u0_lock_down;			//零序保护电压闭锁定值下限，目的是防止零序电压过小
	struct{
		uint16 enable;          //告警压板
		uint16 auto_judge_dir;  //自动方向检测压板
		uint16 enable_out;       //出口压板
		uint16 dir_enable;       //方向压板
		uint16 ulose_enable;     //电压闭锁压板
	}Enable;
}ANGPROTECT;
/*************************功率方向功能数据*************************/
typedef struct pir_protect{
	unsigned int ContralWord;    //功率方向控制字；
	unsigned int Dir ;
	unsigned int Pdirvalue;      //功率方向角度；0-30；1-45
}PIRPROTECT;
/*****************************重合闸保护***************************/

#define MAX_CHZ_NUM 3
typedef struct chz_protect{
	int16  enable[3];		// 0 相间过流重合闸，1 零序过流重合闸，3 零序相位重合闸
	int16  Times;			//<重合闸次数
	int16  Type;			//<重合闸方式
	int32  cdtime;			//充电时间；
	int32 Time[3];			//<三次重合闸时间

	int32 LockTime;			//闭锁2次重合延时
	float DiffVol;			//开关两侧电压差
	float DiffPhase;		//相位差设置
	int16 DiffVolDefine[2];	//开关两侧电压定义

}CHZPROTECT;
/*****************************其他保护共用的定值***************************/
typedef struct pub_value{
	float StartI;				//启动电流,不再使用
	float StartU;				//有压
	float StopU;				//无压，不再使用
	int32 InstantFaultChkTime;	//瞬时故障判断时间
	int32 ResetFaTime;			//延时复归时间
	int16 ResetFaType;			//故障复归类型  1按时间复归 2恢复供电复归 3手动复归
	int16 GroundLinkType;		//1小电流接地2 大电流接地
}PUBVALUE;

typedef   struct AllEnableSet{
    int16 EnableYCEvent;		//遥测越限检测功能投退
    int16 EnableCurrentProtect; //基本保护功能投退
    int16 EnableFa;             //FA功能投退
    int16 EnablePtFun;          //当PT断线时是否自动退出方向元件以及检同期元件,FA功能
}AllEnableSet ;
/********************************保护电参量定值*********************/
typedef struct protect_limit{
	 IPROTECT I_d1;				//速断电流保护功能参数
	 IPROTECT I_d2;      	    //过流电流保护功能参数
	 IPROTECT I_d3;      	    //过负荷电流保护功能参数
	 IPROTECT I0_d1;     	    //零序速断电流保护功能参数
	 IPROTECT I0_d2;     	    //零序定时限电流保护功能参数
	 IPROTECT I0_d3;     	    //零序过负荷电流保护功能参数
	 JYPROTECT JY;              //零序电流阶跃参数
	 IPROTECT U0;				//零序电压保护定值U0,零序电压保护投入定值,零序电压保护出口压板
	 IPROTECT Ihjs;
	 IPROTECT I0hjs;
	 IPROTECT I0PhaseHjs;
	 uint32 hjschecktime;
	 CHZPROTECT chz;            //重合闸功能参数功能参数
	 ANGPROTECT Angle_dz;       //零序相位保护参数	 
	 ANGPROTECT Raise_dz;       //增量电流保护参数	 
	 float U0_Lock;				//零序保护电压闭锁定值
	 uint16 pt_vol_lock;		//PT断线闭锁零序电压
	 PIRPROTECT P_direction;    //功率方向元件参数
	 PIRPROTECT P0_direction;   //功率方向元件参数
	 float P0_coff;				//p0系数，for 3U0 or U0 ;3I0 or I0 ,default is 3U0 3I0
} PROTECTLIMIT;

/***************系统保护,越限定值*********/
typedef struct total_limit_para{
	PROTECTLIMIT  Para_limit[MAX_LINE_NUM][MAX_SECTION_NUM]; //保护电参量定值116*2*6
	int16 ctlock;					//CT断线闭锁方向保护；
	uint16 totalenable;				// 出口总软压板；
	uint16 undefine;				//未定义 保留位置；
	AllEnableSet protect_enable;
	PUBVALUE public_value;			//一些共用的定制//8]
	uint16 U0_source;				//0 采集，1计算获得
	uint16 I0_source;				//0采集，1计算获得
	uint32 GZDefaulte;				//1 代表复归；
	uint16 section_num;				//定值区号
	//unsigned long DefaultTime;
	uint16 tort[MAX_LINE_NUM];      //馈线功率计算方法（2表法/3表法）
	int temp_lmt;                   //温度定值
	float pt_one_set;				//pt额定一次值
	float pt_two_set;				//pt额定二次值
	float ct_one_set;				//ct额定一次值
	float ct_two_set;				//ct额定二次值
	float ct0_one_set;				//零序ct额定一次值
	float ct0_two_set;				//零序ct额定二次值
	float pt0_one_set;				//零序pt额定一次值
	float pt0_two_set;				//零序pt额定二次值
	float ct_accuracy;				//ct精度
	float ct_low_accuracy;			//ct精度下限精度
	float ct_low_ratio;				//ct精度下限比例
	
	///*电压上限告警阈值，恢复阈值，持续时间*/
	limt UupData;
	///*电压下限告警阈值，恢复阈值，持续时间*/
	limt UdownData;
	///*电流上限阈值，恢复阈值，持续时间*/
	limt IupData;
	///*负荷越上限阈值，恢复阈值，持续时间*/
	limt SupData;
	//零序电压上限告警参数
	limt U0upData;
	//零序电流上限告警参数
	limt I0upData;
	uint16 cur_unit;
	uint16 min_unit;
	uint16 max_unit;
	uint16 reverse[17];//预备
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
    Uint16 datatype;		//数据类型
    Uint16 len;				//字为单位
    Uint16 *ppara;			//参数

    Uint16 secperlen;		//段偏移量
    Uint16 *Para;			//参数所在的数据结构
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
