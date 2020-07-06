#ifndef BASEAPP_H
#define BASEAPP_H
#include "datatype.h"


class BaseApp //任务线程的基类
{

private:

public:
    uint32   Id;
    char name[20];
    int timeout;/**<  When timeout, main loop should reset thread  */
    int restart_times;/**<  restart the child times when child is  timeout  */

    BaseApp() {
        timeout = 3;
        restart_times = 3;
    };

    ~BaseApp() {};

    int16 Heart();
    /**
    *************************************************************************************
    *  @brief  读取本模块的配置文件。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    virtual int16 ReadCfg() {
        return 1;
    };

    /**
    *************************************************************************************
    *  @brief  写本模块的配置文件。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    virtual int16 WriteCfg() {
        return 1;
    };

    /**
    *************************************************************************************
    *  @brief  初始化本模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    virtual int16 Init() {
        return 1;
    };

    /**
    *************************************************************************************
    *  @brief  处理本模块获得的消息。
    *  @author  liht
    *  @param[in] MessageData*message，消息类型定义参见datatype.h
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    virtual int16 GetMsg ( MessageData<BaseDataType>message ) {
        return 1;
    };

    /**
    *************************************************************************************
    *  @brief  运行模块。
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    virtual int16 Run ( void*arg ) {
        return 1;
    };
};

#endif
