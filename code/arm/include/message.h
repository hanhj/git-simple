#ifndef MESSAGE_H
#define MESSAGE_H
#include "baseapp.h" ///<用到BaseApp类.
#include "stack.h" ///<用到stack类.
#include "debug.h"

/**
* /brief 消息类
* 该类有两种用法：
* 用法1：向该类注册，调用RegistMsg函数，向该类传递线程号以及消息处理函数，当有消息向该线程传递时，消息类自动调用该线程的消息处理函数
* 用法2：不向该类注册，调用GetMsg函数，向该类传递线程号以及消息数据指针MessageData<BaseDataType>**message，用户需要不断调用GetMsg函数以获取消息。
* 注意：如果某条消息未被所有线程处理，则默认4秒钟后从消息队列中删除。
*/

class Message : public BaseApp
{

private:
    int16 ( *call_fun[MAXTASK] ) ( MessageData<BaseDataType> );
    int8 call_num;
    uint32 call_id[MAXTASK];
    uint16 timer_id;///<内部定时器id


public:
    /**
    * /brief 消息数据链表
    */
    Stack  Msg;
    Message();
    ~Message();
    int16 Init();
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
    /**
    *************************************************************************************
    *  @brief  线程之间发送消息.
    *  @author  liht
    *  @param[in] MessageData<BaseDataType>*
    *  @param[out]
    *  @return  无
    **************************************************************************************
    */
    void SendMsg ( uint32 msg_to, int32 msg_id, int32 data_id, BaseDataType*data, CP56Time2a* time );
    void SendMsg ( uint32 msg_to, int32 msg_id, int32 data_id, char*data, CP56Time2a* time );
    /**
    *************************************************************************************
    *  @brief  线程之间获得消息.
    *  @author  liht
    *  @param[in] int32 Id 线程号,MessageData<BaseDataType>&message获得的数据
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 GetMsg ( uint32 Id, MessageData<BaseDataType>**message );
    int16 GetMsg ( );
    /**
    *************************************************************************************
    *  @brief 向消息类注册消息处理函数.
    *  @author  liht
    *  @param[in] int16 threadid,int16 (MessageData<BaseDataType>*)
    *  @param[out]
    *  @return  成功，返回1，否则返回0。
    **************************************************************************************
    */
    int16 RegistMsg ( uint32 Id, int16 ( *get_msg ) ( MessageData<BaseDataType> ) );
		int16 UnRegistMsg ( uint32 Id );
    /**
    *************************************************************************************
    *  @brief 检查并派发消息处理函数.
    *  @author  liht
    *  @param[in]
    *  @param[out]
    *  @return  无
    **************************************************************************************
    */
    void CheckMsg();
};

extern Message msg;
/** /brief需要定义一个全局函数Task类处理该类的入口
*/
extern void* message_main ( void* );
#endif
