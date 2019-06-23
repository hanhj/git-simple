/*
 * File Name: stack.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-17 19:07:16 week:1
*/

#ifndef __stack_h
#define __stack_h
#include "data_type.h"
#include "common.h"
#define MaxStack 100
/* Stack is LIFO :last in first out*/
template <class T>
class Stack{
	public:
		int size;
	public:
		Stack();
		~Stack();
		virtual void push(T da)=0;
		virtual int pop(T *da)=0;
		virtual int top(T *da)=0;
		virtual int isEmpty()=0;
		virtual void print()=0;
		int get_size();
};
/*ArrayStack use pos for top element positon.Init pos is -1*/
template <class T>
class ArrayStack:public Stack<T>{
	private:
		T data[MaxStack];//use array as data
		int pos;//reocrd current top position
	public:
		ArrayStack();
		~ArrayStack();
		void push(T da);
		int pop(T *da);
		int top(T *da);
		int isEmpty();
		void print();
};
/*LinkStack use top_prt as top element.Init top_prt is NULL*/
/*LinkStack push ,pop data at head*/
template <class T>
class LinkStack:public Stack<T>{
	private:
		Node<T>*top_ptr;//use first data point as data list.Not use head.
	public:
		LinkStack();
		~LinkStack();
		void push(T da);//change first data ptr
		int pop(T *da);//chage first data ptr
		int top(T *da);
		int isEmpty();
		void print();
};

/* realize Stack*/
template <class T>
Stack<T>::Stack(){
	size=0;
}
template <class T>
Stack<T>::~Stack(){
}
template <class T>
int Stack<T>::get_size(){
	return size;
}
/* realize ArrayStack*/
template <class T>
ArrayStack<T>::ArrayStack(){
	cout<<"use ArrayStack"<<endl;
	pos=-1;
}

template <class T>
ArrayStack<T>::~ArrayStack(){
}

template <class T>
void ArrayStack<T>::push(T da){
	if(pos==MaxStack-1){
		cout<<"stack is full"<<endl;
		return;
	}
	pos++;
	data[pos]=da;
	this->size++;
}

template <class T>
int ArrayStack<T>::pop(T *da){
	if(isEmpty())
		return -1;
	*da=data[pos];
	pos--;
	this->size--;
	return 0;
}

template <class T>
int ArrayStack<T>::top(T *da){
	if(isEmpty())
		return -1;
	*da=data[pos];
	return 0;
}

template <class T>
int ArrayStack<T>::isEmpty(){
	return pos==-1?1:0;
}
template <class T>
void ArrayStack<T>::print(){
	int i;
	if(isEmpty()){
		cout<<"stack is empty"<<endl;
		return;
	}
	for(i=0;i<=pos;i++){
		cout<<data[i]<<" ";
	}
	cout<<endl;
}
/* realize LinkStack*/
template <class T>
LinkStack<T>::LinkStack(){
	cout<<"use LinkStack"<<endl;
	top_ptr=NULL;
}

template <class T>
LinkStack<T>::~LinkStack(){
	Node<T>*tp;
	Node<T>*tmp;
	tp=top_ptr;
	while(tp){
		tmp=tp;
		tp=tp->next;
		free(tmp);
	}
}

template <class T>
void LinkStack<T>::push(T da){
	Node<T>*tp;
	tp=(Node<T>*)malloc(sizeof(Node<T>));
	if(tp==NULL){
		cout<<"fail malloc"<<endl;
		return ;
	}
	tp->next=top_ptr;
	tp->data=da;
	top_ptr=tp;
	this->size++;
}

template <class T>
int LinkStack<T>::pop(T *da){
	if(isEmpty())
		return -1;
	*da=top_ptr->data;
	Node<T>*tp;
	tp=top_ptr;
	top_ptr=top_ptr->next;
	free(tp);
	this->size--;
	return 0;
}

template <class T>
int LinkStack<T>::top(T*da){
	if(isEmpty())
		return -1;
	*da=top_ptr->data;
	return 0;
}

template <class T>
int LinkStack<T>::isEmpty(){
	return top_ptr==NULL?1:0;
}

template <class T>
void LinkStack<T>::print(){
	Node<T>*tp;
	if(isEmpty()){
		cout<<"stack is empty"<<endl;
		return;
	}
	tp=top_ptr;
	while(tp){
		cout<<tp->data<<" ";
		tp=tp->next;
	}
	cout<<endl;
}

void test_stack();
#endif
// vim:tw=72 
