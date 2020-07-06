#ifndef STACK_H
#define STACK_H
#include "datatype.h"

typedef struct StackNode_ {
    MessageData<BaseDataType> data;
    StackNode_ *next;
    StackNode_ *pre;
}Node;

/**
* /brief 堆栈类，先入先出
*/

class Stack
{
    int32 current_count;
    Node*head;
    Node*last;

public:
    Stack();
    /**
    * /brief 向堆栈中压入数据
    * /param MessageData<BaseDataType>
    * /return 0 表示失败
    *     1 表示成功
    */
    int8 Push ( MessageData<BaseDataType> );
    int8 Push ( MessageData<BaseDataType> msg ,char *data);
    /**
    * /brief 从堆栈中弹出最后一组数拿
    * /param 1 代表从堆栈中删除数据ﺿ代表不删除数拿
    * /return MessageData*，失败返回NULL
    */
    Node* Pop ( int8 );
    int32 GetNum() {
        return current_count;
    }

    /**
    * /brief 清除堆栈中数拿
    * /param
    * /return
    */
    void Flush();
};
#endif