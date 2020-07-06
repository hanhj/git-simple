#ifndef HIS_DATA_H
#define HIS_DATA_H
//#include "datatype.h"

typedef struct file_index_table {
    enum  EVENT_TYPE type;
    int32 Id;//文件ID对应的信息体地址
    char  Name[50];//文件名
    int32 Len;//文件长度
    int16 Count;//计数
    int16 MaxCount;//最大计数
} FILE_INDEX_TABLE;

typedef struct soe_file {
    int32 Id;//数据ID
    int8  Status;//状态
    CP56Time2a Time;//时标
}__attribute__ ((packed)) SOE_MSG;

//SOE_MSG;

class HisData
{

public:
    HisData();
    /**
     * 读取事件记录表
     * @param int32 Id
     * @return: 无
     */
    FILE_INDEX_TABLE* FindHisDbById ( int32 Id );
    FILE_INDEX_TABLE* FindHisDbByType ( int16 type );

    /**
     * 保存事件记录
     * @param void *p
     * @param int len
     * @param int16 历史数据类型（与规约无关） type
     * @return: 无
     */
    int SaveHisRecord ( void *p, uint16 len, int16 type );
    /**
     * 读取事件记录
     * @param void *p
     * @param int index
     * @param int len
     * @param int32 历史数据Id（与规约相关） Id
     * @return: 读取成功1
           读取失败0
     */
    int16 ReadHisRecord ( void *p, int32 offset, uint16 len, int32 Id );
    /**
     * 清除事件记录
     * @param int32 Id
     * @return: 成功1
           失败0
     */
    int16 CleanHisData ( int32 Id );
};
#endif