/*
 * File Name: datatype.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 10:31:27 week:5
*/
#ifndef __datatype_h
#define __datatype_h
#include "all_h.h"
using namespace std;
typedef unsigned char uint8; 
typedef signed char int8_t; 
typedef int int16;
typedef unsigned int uint16; 
typedef unsigned int Uint16;
typedef long int32;
typedef unsigned long uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned long long Uint64;
typedef float float32;
template<typename T>
class tmpComplex{
	public:
		T min;
		T rel,img;
		tmpComplex();
		tmpComplex(tmpComplex<T> &);
		tmpComplex(T ,T );
		T get_rad();
		T get_mod();
		void set_min(T);
		void rotate(T);
		tmpComplex<T>  tmp_rotate(T);
		tmpComplex<T>  operator +(tmpComplex<T> &);
		tmpComplex<T>  operator -(tmpComplex<T> &);
		tmpComplex<T>  operator *(tmpComplex<T> &);
		tmpComplex<T>  operator /(tmpComplex<T> &);
		tmpComplex<T>  operator /(float);
		tmpComplex<T>  operator /(int);
		tmpComplex<T>& operator =(int);
		tmpComplex<T>& operator =(float);
		int operator ==(tmpComplex<T> &);
};
template<typename T>
tmpComplex<T>::tmpComplex(){
	rel=0;
	img=0;
	min=0;
}
template<typename T>
tmpComplex<T>::tmpComplex(tmpComplex <T> &da){
	rel=da.rel;
	img=da.img;
	min=da.min;
}
template<typename T>
tmpComplex<T>::tmpComplex(T da,T db){
	rel=da;
	img=db;
	min=0;
}
template<typename T>
T tmpComplex<T>::get_rad(){
	return (T)atan2(img,rel);
}
template<typename T>
T tmpComplex<T>::get_mod(){
	return (T)sqrt(rel*rel+img*img);
}
template<typename T>
void tmpComplex<T>::set_min(T da){
	min=da;
}
//anticlockwise direction
template <typename T>
void tmpComplex<T>::rotate(T da){
	T mag;
	T theata;
	mag=sqrt(rel*rel+img*img);
	theata=atan2(img,rel);
	rel=mag*cos(theata+da);
	img=mag*sin(theata+da);
}
template <typename T>
tmpComplex<T> tmpComplex<T>::tmp_rotate(T da){
	tmpComplex<T>tmp;
	T mag;
	T theata;
	T tmp_rel;
	T tmp_img;
	mag=sqrt(rel*rel+img*img);
	theata=atan2(img,rel);
	tmp_rel=mag*cos(theata+da);
	tmp_img=mag*sin(theata+da);
	tmp.rel=tmp_rel;
	tmp.img=tmp_img;
	tmp.min=min;
	return tmp;
}
template<typename T>
tmpComplex<T>  tmpComplex<T>::operator + (tmpComplex<T> &da){
	tmpComplex<T> c;
	c.rel=rel+da.rel;
	c.img=img+da.img;
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T>  tmpComplex<T>::operator - (tmpComplex<T> &da){
	tmpComplex<T> c;
	c.rel=rel-da.rel;
	c.img=img-da.img;
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T> tmpComplex<T>::operator *(tmpComplex<T> &da){
	tmpComplex<T> c;
	c.rel=rel*da.rel-img*da.img;
	c.img=rel*da.img+img*da.rel;
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T> tmpComplex<T>::operator /(tmpComplex<T> &da){
	tmpComplex<T> c;
	c.rel=(rel*da.rel+img*da.img)/(da.rel*da.rel+da.img*da.img);
	c.img=(img*da.rel-rel*da.img)/(da.rel*da.rel+da.img*da.img);
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T> tmpComplex<T>::operator /(float da){
	tmpComplex<T> c;
	c.rel=rel/da;
	c.img=img/da;
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T> tmpComplex<T>::operator /(int da){
	tmpComplex<T> c;
	c.rel=rel/da;
	c.img=img/da;
	c.min=min;
	return c;
}
template<typename T>
tmpComplex<T>& tmpComplex<T>::operator =(int da){
	rel=da;
	img=da;
	return *this;
}
template<typename T>
tmpComplex<T>& tmpComplex<T>::operator =(float da){
	rel=da;
	img=da;
	return *this;
}
template<typename T>
int tmpComplex<T>::operator==(tmpComplex<T> &da){
	T diff_rel=fabs(rel-da.rel);
	T diff_img=fabs(img-da.img);
	if(diff_rel<=min && diff_img<=min){
		return 1;
	}
	return 0;
}
template <typename T>
class _cir_iterator{
	public:
		typedef _cir_iterator<T> self;
		T *data;
		int pos;	
		int max;
		_cir_iterator(){
			pos=0;
		}
		_cir_iterator(int da){
			max=da;
		}
		_cir_iterator(T *da,int p){
			data=da;
			pos=p;
		}
		self & operator =(self da){
			data=da.data;
			pos=da.pos;
			return *this;
		}
		self operator ++(int){
			int tmp;
			tmp=pos;
			pos=(pos+1)%max;
			return (self)tmp;
		}
		self operator --(int){
			int tmp;
			tmp=pos;
			pos=(pos-1)%max;
			return (self)tmp;
		}
		self & operator ++(){
			pos=(pos+1)%max;
			return (self)pos;
		}
		self & operator --(){
			pos=(pos-1)%max;
			return (self)pos;
		}
		T & operator *(){
			return data[pos];
		}
		T & operator [](int n){
			return data[n];
		}
		T * operator &(){
			return &data[pos];
		}
		T * operator &(int n){
			return &data[n];
		}
		T * operator ->(){
			return &data[pos];
		}
		bool operator !=(const self &da ){
			return pos!=da.pos;
		}
		bool operator ==(const self &da){
			return pos==da.pos;
		}
};
/*Queue is FIFO struct.First in First out*/
//#define MaxQueue 100
template <typename T>
class CircleQueue{
	private:
		//T data[MaxQueue];
		T *data;
		int _front;
		int _tail;
		int _size;
	public:
		typedef _cir_iterator<T> iterator;
		int MaxQueue;
		iterator begin(){
			return iterator(data,_front);
		}
		iterator end(){
			return iterator(data,_tail);
		}
		CircleQueue(){
			_front=_tail=0;
			this->_size=0;
			MaxQueue=0;
			data=NULL;
		}
		~CircleQueue();
		void init(int size);
		int push(T&);
		int pop();
		int push_pop(T&);
		T & front();
		int size();
		int empty();
		int full();
		void clear();
};
template <typename T>
void CircleQueue<T>::init(int size){
	if(data!=NULL)
		return;
	if(size>0){
		data=(T*)malloc(sizeof(T)*size);
		MaxQueue=size;
		_front=_tail=0;
		this->_size=0;
	}
}
template <typename T>
CircleQueue<T>::~CircleQueue(){
	if(data){
		free(data);
		data=NULL;
	}
	_size=0;
	_front=_tail=0;
}

template <typename T>
int CircleQueue<T>::push(T &da){
	if(full()||data==NULL){
		return -1;
	}
	data[_tail]=da;
	_tail=(_tail+1)%MaxQueue;
	this->_size++;
	return 0;
}
template <typename T>
int CircleQueue<T>::pop(){
	if(empty()){
		return -1;
	}
	_front=(_front+1)%MaxQueue;
	this->_size--;
	return 0;
}
template <typename T>
int CircleQueue<T>::push_pop(T &da){
	if(full()){
		pop();
	}
	return push(da);
}
template <typename T>
T & CircleQueue<T>::front(){
	return data[_front];
}
template<typename T>
int CircleQueue<T>::size(){
	return this->_size;
}
template <typename T>
int CircleQueue<T>::empty(){
	return _front ==_tail;
}
template <typename T>
int CircleQueue<T>::full(){
	return (_tail+1) % MaxQueue==_front;
}
template <typename T>
void CircleQueue<T>::clear(){
	this->_size=0;
	_front=_tail=0;
}
typedef tmpComplex<float> Complex;

#endif //__datatype_h
// vim:tw=72 
