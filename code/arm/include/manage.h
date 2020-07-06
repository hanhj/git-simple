#ifndef MANAGE_H
#define MANAGE_H
#include "datatype.h"

class Manage: public BaseApp
{

private:

    int16 StartProcess ( char*procename );
    int16 StoptProcess ( char*stopprocname );
    int16 CheckProcess();
    int16 Update();
    WatchDog dog ( FEED_DOG_PIN );//FEED_DOG_PIN

public:
    DataType11 manage_cfg; ///<进程管理类的配置参数
    Manage() {};

    ~Manage() {};

    int16 ReadCfg();
    int16 WriteCfg();
    int16 Init();
    int16 Run ( void*arg );
};
#endif