#ifndef STATIS_H
#define STATIS_H
#include "datatype.h" ///<用到数据类型定义
#include "baseapp.h"  ///<用到基础类定义
#include "event_db.h"  ///
#include "hisdata.h"  ///

typedef struct _StaPara {
    HisDataDefinePara HisDefine;///<遥测历史数据项定义
    DataType2  HisSaveSpan;///<遥测历史数据存储间隔
    HisDataDefinePara ExtreDefine;///<遥测极值数据项定义
}StaPara;

class Statis : public BaseApp
{

private:
    HisData his_data;
    BOOL FaultWaveRecordFlag;

    /**
    *************************************************************************************
    *  @brief  历史数据统计.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  无。
    **************************************************************************************
    */
    void HisDataSta ( void );
    /**
    *************************************************************************************
    *  @brief  极值数据统计.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  无。
    *  @see  Statis
    **************************************************************************************
    */
    void ExtremumDataSta ( void );
    /**
    *************************************************************************************
    *  @brief  停电纪录统计.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return 无。
    **************************************************************************************
    */
    void LoseElecSta ( void );
    /**
    *************************************************************************************
    *  @brief  录波数据记录.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return 无。
    **************************************************************************************
    */
    void FaultWaveRecord ( void );
    /**
    *************************************************************************************
    *  @brief  电能质量数据记录.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return 无。
    **************************************************************************************
    */
    void ElecQualSta ( void );
    /**
    *************************************************************************************
    *  @brief  保存Soe数据记录.
    *  @param MessageData<BaseDataType>message 消息
    *  @param EVENT_TYPE type事件类型
    *  @return 无。
    **************************************************************************************
    */
    void SaveSoe ( MessageData<BaseDataType>message, EVENT_TYPE type );
/**
    *************************************************************************************
    *  @brief  synchronize dsp and arm time.
	*  because the delay of message transfertime,so when dsp receive the time message,it is not really time from arm.
	*  in this function ,arm read the cp56time first,then secondary read the omapl138 timer2 counter,the third send this message to dsp.
	*  When dsp read this message,it can compare the read message's timer counter and timer2 counter really ,so it can caculate the delay time of message time.in this way ,dsp can synchronize arm time accurate.
    *  @param void
    *  @return -1 as fail 0 as success.
    **************************************************************************************
    */
    int SynDspTime ( void );


    int16 GetMsg();

public:
    StaPara sta_cfg; ///<统计数据类的配置
    Statis();
    ~Statis();
    int16 ReadCfg();
    int16 WriteCfg();

    /**
    *************************************************************************************
    *  @brief  初始化本模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Init();
    /**
    *************************************************************************************
    *  @brief  处理本模块获得的消息。
    *  @author  liht
    *  @param[in] MessageData*message，消息类型定义参见datatype.h
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 GetMsg ( MessageData<BaseDataType>message );

    /**
    *************************************************************************************
    *  @brief  运行模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 Run ( void*arg );
    /*************************************************************************************
    *  @brief  读取数据记录.
    *  @param void*p 读出的数据,int16 offset, int16 len,EVENT_TYPE type
    *  @return 无。
    **************************************************************************************
    */
    int16 ReadHisData ( void*p, int32 offset, int16 len, int32 Id );
    /*************************************************************************************
    *  @brief  读取历史数据记录长度.
    *  @param in Id文件Id
    *  @return 文件总长
    **************************************************************************************
    */
    int32 GetHisDataLen ( int32 Id );

};

extern Statis statis;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* statis_main ( void* );

#endif
