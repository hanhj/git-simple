/*
 * stack.h
 *
 *  Created on: 2013-8-14
 *      Author: Administrator
 */

#ifndef STACK_H_
#define STACK_H_


#include "datatype.h"

typedef struct StackNode_{
	MessageData<BaseDataType> data;
	StackNode_ *next;
	StackNode_ *pre;
}Node;

	/**
	* /brief 堆栈类，先入先出
	*/
class Stack{
	int32 current_count;
	Node*head;
	Node*last;
public:
	Stack();
	/**
	* /brief 向堆栈中压入数据
	* /param MessageData<BaseDataType>
	* /return 0 表示失败
	*					1 表示成功
	*/
    int8 Push ( MessageData<BaseDataType> );
    int8 Push ( MessageData<BaseDataType> msg ,char *data);
	/**
	* /brief 从堆栈中弹出最后一组数据
	* /param 1 代表从堆栈中删除数据，0代表不删除数据
	* /return MessageData*，失败返回NULL
	*/
	Node* Pop(int8);
    int32 GetNum() {
        return current_count;
    }
	/**
	* /brief 清除堆栈中数据
	* /param
	* /return
	*/
	void Flush();
};


#endif /* STACK_H_ */
