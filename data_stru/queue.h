/*
 * File Name: queue.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-17 19:07:16 week:1
*/
#ifndef __queue_h
#define __queue_h
#include "data_type.h"
#include "common.h"
#define MaxQueue 100
/*Queue is FIFO struct.First in First out*/
template <class T>
class Queue{
	protected:
		int size;
	public:
		Queue(){
			size=0;
		}
		~Queue(){
		}
		virtual int add(T)=0;
		virtual	int dequeue(T &)=0;
		virtual	int get_size()=0;
		virtual int isEmpty()=0;
		virtual	void print()=0;
};
/*ArrayQueue use front for first element,tail for end element*/
/*add data at tail,dequeue data at front*/
/* if front=tail=-1:
 *	front means ahead of first data
 *	tail means pos of last data
 *	so 
 *		add		:tail+=1;	set data
 *		deque	:front+=1;	get data
 *		is_empty:front=tail
 *		is_full	:i reverse a data for judge full
 *			tail+1==front?1:0;
 *			if front=-1£¬if tail=max-1 return 1;
 *
 * if front=tail=0;
 * front means first data
 * tail means last next data pos
 * so
 *		add		:set data tail+=1
 *		deque	:get data front+=1
 *		is_empty	:front=tail
 *		is_full	:tail+1=front?1:0
 *
 */
template <class T>
class ArrayQueue:public Queue<T>{
	private:
		T data[MaxQueue];
		int front;
		int tail;
	public:
		ArrayQueue(){
			front=tail=-1;
			this->size=0;
			cout<<"use ArrayQueue"<<endl;
		}
		virtual int add(T);
		virtual int dequeue(T &);
		virtual int get_size();
		virtual int isEmpty();
		virtual int isFull();
		virtual void print();
};
template <class T>
class CircleQueue:public Queue<T>{
	private:
		T data[MaxQueue];
		int front;
		int tail;
	public:
		CircleQueue(){
			front=tail=-1;
			this->size=0;
			cout<<"use CircleQueue"<<endl;
		}
		virtual int add(T);
		virtual int dequeue(T &);
		virtual int get_size();
		virtual int isEmpty();
		virtual int isFull();
		virtual void print();
};
/* LinkQueue have a head point.
 * use front,tail point.
 * front point to ahead of first data.front->next is first data.
 * tail point to last data
 */
template <class T>
class LinkQueue:public Queue<T>{
	private:
		Node<T> *head;
		Node<T> *front;
		Node<T> *tail;
	public:
		LinkQueue(){
			head=(Node<T>*)malloc(sizeof(Node<T>));
			head->next=NULL;
			front=head;
			tail=head;
			this->size=0;
			cout<<"use LinkQueue"<<endl;
		}
		~LinkQueue(){
			Node<T>*tp;
			Node<T>*tmp;
			tp=head->next;
			while(tp){
				tmp=tp;
				tp=tp->next;
				free(tmp);
			}
			free(head);
		}
		int add(T);
		int dequeue(T &);
		int get_size();
		int isEmpty();
		int isFull();
		void print();
};
template <class T>
int ArrayQueue<T>::add(T da){
	if(isFull())
		return -1;
	tail+=1;
	data[tail]=da;
	this->size++;
	return 0;
}

template <class T>
int ArrayQueue<T>::dequeue(T &da){
	if(isEmpty())
		return -1;
	front+=1;
	da=data[front];
	this->size--;
	return 0;
}
template<class T>
int ArrayQueue<T>::get_size(){
	return this->size;
}
template <class T>
int ArrayQueue<T>::isEmpty(){
	return front==tail?1:0;
}
template<class T>
int ArrayQueue<T>::isFull(){
	return tail==MaxQueue-1?1:0;
}
template <class T>
void ArrayQueue<T>::print(){
	if(isEmpty())
		return;
	for(int i=front+1;i<=tail;i++)
		cout<<data[i]<<" ";
	cout<<endl;
}
template <class T>
int CircleQueue<T>::add(T da){
	if(isFull()){
		cout<<"full"<<endl;
		return -1;
	}
	tail=(tail+1)%MaxQueue;
	data[tail]=da;
	this->size++;
	return 0;
}
template <class T>
int CircleQueue<T>::dequeue(T &da){
	if(isEmpty()){
		cout<<"is empty"<<endl;
		return -1;
	}
	front=(front+1)%MaxQueue;
	da=data[front];
	this->size--;
	return 0;
}
template<class T>
int CircleQueue<T>::get_size(){
	return this->size;
}
template <class T>
int CircleQueue<T>::isEmpty(){
	return front==tail?1:0;
}
template <class T>
int CircleQueue<T>::isFull(){
	if(front==-1){
		return tail==MaxQueue-1?1:0;
	}else{
		return (tail+1)%MaxQueue==front?1:0;
	}
}
template <class T>
void CircleQueue<T>::print(){
	int i;
	i=(front+1)%MaxQueue;
	for(;i!=tail;){
		cout<<data[i]<<" ";
		i=(i+1)%MaxQueue;
	}
	cout<<endl;
}
template <class T>
int LinkQueue<T>::add(T da){
	Node<T>*tp;
	tp=(Node<T>*)malloc(sizeof(Node<T>));
	if(tp==NULL){
		cout<<"fail malloc"<<endl;
		return -1;
	}
	tp->data=da;
	tp->next=NULL;
	tail->next=tp;
	tail=tp;
	this->size++;
	return 0;
}
template <class T>
int LinkQueue<T>::dequeue(T &da){
	Node<T> *tp;
	if(isEmpty())
		return -1;
	tp=front->next;
	da=tp->data;
	front->next=front->next->next;
	free(tp);
	this->size--;
	if(this->size==0)
		tail=head;
	return 0;
}
template <class T>
int LinkQueue<T>::get_size(){
	return this->size;
}
template <class T>
int LinkQueue<T>::isEmpty(){
	return this->size==0?1:0;
}
template <class T>
int LinkQueue<T>::isFull(){
	return 0;
}
template <class T>
void LinkQueue<T>::print(){
	Node<T>*tp;
	tp=front->next;
	while(tp){
		cout<<tp->data<<" ";
		tp=tp->next;
	}
	cout<<endl;
}

void test_queue();
#endif
