/*
 * File Name: a.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-11-08 15:08:48 week:5
*/
#include<iostream>
#include<stdlib.h>
using namespace std;
#include <list>
#include <vector>
#include "it.h"
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <iconv.h>
#define STDIN 0
//getchar from console by press key and return 
//this function is polling
int mygetc(){
	timeval tm={0,0};
	fd_set r;
	FD_ZERO(&r);
	FD_SET(STDIN,&r);
	int ret;
	int ch;
	ret=select(STDIN+1,&r,NULL,NULL,&tm);
	switch(ret){
		case -1:
			return -1;//error
		case 0:
			return 0;//timeout
		default:
			if(FD_ISSET(STDIN,&r)){
				read(0,&ch,1);
				return ch;
			}
	}
	return ret;
}
termios init_termios,new_termios;
int peek_char=-1;
void init_kb(){
	tcgetattr(0,&init_termios);
	new_termios=init_termios;
	new_termios.c_lflag &=~ICANON;
	new_termios.c_lflag &=~ECHO;
	new_termios.c_lflag &=~ISIG;
	new_termios.c_cc[VMIN]=1;
	new_termios.c_cc[VTIME]=0;
}
void close_kb(){
	tcsetattr(0,TCSANOW,&init_termios);
}
/*
int kbhit(){
	int ch;
	int nread;
	if(peek_char!=-1){
		return 1;
	}
	new_termios.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW,&new_termios);
	nread=read(0,&ch,1);
	new_termios.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&new_termios);
	if(nread>0){
		peek_char=ch;
		return 1;
	}
	return 0;
}
int readch(){
	int ch;
	if(peek_char!=-1){
		ch=peek_char&0xff;
		peek_char=-1;
		return ch;
	}
	read(0,&ch,1);
	return ch;
}
*/
int kbhit(){
	int ch;
	int nread;
	new_termios.c_cc[VMIN]=0;
	tcsetattr(0,TCSANOW,&new_termios);
	nread=read(0,&ch,1);
	new_termios.c_cc[VMIN]=1;
	tcsetattr(0,TCSANOW,&new_termios);
	if(nread>0){
		peek_char=ch;
		return 1;
	}
	return 0;
}
int readch(){
	int ch=-1;
	if(peek_char!=-1){
		ch=peek_char&0xff;
		peek_char=-1;
		return ch;
	}
	return ch;
}
/*如何构造迭代器：
 * 1、迭代器需要的类：
 * 节点类，用于封装数据。
 * 容器类，用于封装容器,容纳数据。
 * 迭代类，用于封装指针。遍历容器类中的节点数据。
 * 2.各个类中的要求
 * 2.1节点类没有什么特殊要求。
 * 2.2迭代类，
 *	定义一个节点指针用于记录当前数据位置。
 *	定义构造函数，赋值节点指针。
 *	重载* 解 操作函数，返回值为节点的数据引用。
 *	重载前++，后++操作符函数，
 *		返回值是迭代类的引用或数。
 *		前者返回引用，因为改变了数据指针位置，或者数。前++：本身变，返回变
 *		后者返回一个数。后++：本身变，返回不变。
 *	->指向函数
 *		如果数据是一个结构，可能需要一个指向其中这个结构的指针，将迭代器的指针转换成数据结构指针。
 *	均是单目运算符。
 *2.3容器类
 *	用typedef _list_iterator<T> iterator
 *	定义一个迭代类的名，方便使用。符合stl通用做法。
 *	定义begin，end函数，用于赋值迭代类实例。返回值是迭代类实例。
 *
 * */
template <typename T>
class _node{
	public:
		T data;
		_node<T> *next;
		_node(const T& da){
			data=da;
			next=NULL;
		}
};

template<typename T>
class _list_iterator{
	public:
/*		
	typedef typename T::differenct_type differenct_type;
	typedef typename T::value_type value_type;
	typedef typename T::pointer pointer;
*/
	typedef _node<T> Node;
	typedef _list_iterator<T> self;
	Node *node;
	_list_iterator(){

	}
	_list_iterator(Node *n):node(n){
		
	}
	T& operator *(){
		return node->data;
	}
	Node* operator ->(){
		return node;
	}
	self& operator++(){
		node=node->next;
		return (self&)node;
	}
	self operator ++(int){
		_node<T> *tmp;
		tmp=node;
		node=node->next;
		return (self)tmp;
	}
	bool operator !=(const self &da){
		return node!=da.node;
	}
	bool operator ==(const self &da){
		return node == da.node;
	}

};
template <typename T>
class List{
	public:
		int size;
		typedef _list_iterator<T>  iterator;
		_node<T> *node;

		_node<T> *head;
		_node<T> *tail;
		List(){
			head =(_node<T>*)malloc(sizeof(_node<T>));
			head->next=NULL;
			tail=head;
			size=0;
			cout<<head<<endl;
		}
		~List(){
			_node<T> *node,*tmp;
			node=head;
			while(node!=NULL){
				tmp=node;
				node=tmp->next;
				free(tmp);
				cout<<"delete :"<<tmp<<endl;
			}
			head=NULL;
		}
		void push_back(T &da){
			_node<T> * node;
			node =(_node<T>*)malloc(sizeof(_node<T>));
			node->data=da;
			node->next=NULL;
			tail->next=node;
			tail=node;
			size++;
			cout<<node<<endl;
		}
		_node<T> * pop(){
			if(isEmpty())
				return NULL;
			_node<T> *node;
			node=head->next;
			head->next=node->next;
			size--;
			return node;
		}
		iterator begin(){
			return iterator(head->next);
		}
		iterator end(){
			return iterator(tail->next);
		}
		int isEmpty(){
			return size==0?1:0;
		}
};

template <typename T>
ostream & operator<<(ostream &os,List<T>&da){
	_node<T> *node;
	node=da.head->next;
	while(node!=NULL){
		os<<node->data<<" ";
		node=node->next;
	}
	os<<endl;
	return os;
}

typedef struct _data{
	int value;
	int flag;
}data;
list<int> ll;
vector<data> vec;
ostream& operator <<(ostream &os,list<int>l){
	list<int>::iterator it;
	it=l.begin();
	while(it!=l.end()){
		os<<*it<<" ";
		it++;
	}
	os<<endl;
	return os;
}
ostream& operator <<(ostream &os,vector<data>l){
	vector<data>::iterator it;
	data da;
	it=l.begin();
	while(it!=l.end()){
		da=*it;
		os<<da.value<<" ";
		it++;
	}
	os<<endl;
	return os;
}
template <typename Iter>
void myswap(Iter a,Iter b){
	cout<<"before:"<<*a<<" "<<*b<<endl;
	typename std::iterator_traits<Iter>::value_type tmp=*a;
	*a=*b;
	*b=tmp;
	cout<<"after:"<<*a<<" "<<*b<<endl;
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
		T & operator [](int p){
			return data[p];
		}
		T * operator &(){
			return &data[pos];
		}
		T * operator &(int p){
			return &data[p];
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
	if(data)
		free(data);
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
CircleQueue<int> cq;
void get_data(int * &d){
	CircleQueue<int>::iterator it;
	it=cq.begin();
	d=&it;
}
#define PROCESS_LINK		1<<0
#define PROCESS_SUMMON		1<<1
#define PROCESS_CLOCK		1<<2
#define PROCESS_EVENT		1<<3
#define PROCESS_RM_CTL		1<<4
#define PROCESS_TEST_LINK	1<<5
#define PROCESS_YC_CHANGE	0x40	
#define PROCESS_RESET		1<<7
typedef struct _dir_node{//directory
	long name_len;
	char name[30];
	long file_id;
	long file_size;
}dir_node;
typedef CircleQueue<dir_node> dir_list;
#include "string.h"
void read_file(){
	FILE *f;
	char buff[1000];
	int tmp[500];
	f=fopen("test.dat","r");
	if(!f)
		return;
	char * ret;
	while(!feof(f)){
		ret=fgets(buff,1000,f);
		if(ret!=NULL){
			if(buff[0]!='-'&&buff[1]!='>')
				continue;
			int len;
			int i;
			int j;
			int m;
			int num;
			len=strlen(buff);
			i=5;
			m=0;
			for(j=i;j<len;j++){
				if(buff[j]==' ')
					continue;
				if(isxdigit(buff[j])){
					sscanf(&buff[j],"%x",&num);
					tmp[m]=num;
					m++;
					j++;
				}
			}
			printf("rcv:%d:",m);
			for(j=0;j<m;j++){
				printf("%x ",tmp[j]);
			}
			cout<<endl;

		}
	}
	fclose(f);
}
void dump(const char * pre,const char *data,int len){
	int i;
	i=0;
	unsigned char c;
	printf("%s",pre);
	while(i<len){
		c=data[i++];
		printf("%x ",c);
		if(i%8==0)
			printf("\n");
	}
	printf("\n");
}
typedef struct _hanzi{
	unsigned long number;
	char data[24];
}hanzi;
hanzi hanzi_tab[10]={
	{0xe5958a,{1,2,3,4}},//啊
	{0xe788b1,{4,5,6,7}},//爱
	{0xe79a84,{7,8,9,0}},//的
	{0xc2b0,{0,1,2,3}}//°
};
void print_hz(const char *str){
	unsigned long code;
	unsigned char c;
	int i;
	int len;
	c=*str;
	printf("%s",str);
	len=2;
	if((c&0xe0)==0xe0)
		len=3;
	else if((c&0xc0)==0xc0)
		len=2;
	if(len>=2){
		code=0;
		for(i=0;i<len;i++){
			c=*str++;
			code|=c<<(8*(len-i-1));
		}
		for(i=0;i<(int)sizeof(hanzi_tab)/(int)sizeof(hanzi);i++){
			if(hanzi_tab[i].number==code){
				dump("dot:",hanzi_tab[i].data,4);
				return;
			}
		}
	}
	printf("not find\n");
		
}
void test_iconv(){
	iconv_t cd;
	string src="啊";
	char dst[10];
	char *in;
	char *out;
	size_t l1,l2;

	in=(char *)src.c_str();
	out=&dst[0];
	l1=src.length();
	l2=10;
	printf("%s",in);
	printf("l1:%d l2:%d\n",l1,l2);
	dump("utf8:",in,4);
	cd = iconv_open("GBK","utf-8");
	iconv(cd,&in,&l1,&out,&l2);
	dump("gbk:",&dst[0],4);
	printf("l1:%d l2:%d\n",l1,l2);
	iconv_close(cd);
	
	in=(char *)src.c_str();
	out=&dst[0];
	l1=src.length();
	l2=10;
	printf("%s",in);
	printf("l1:%d l2:%d\n",l1,l2);
	dump("utf8:",in,4);
	cd = iconv_open("ucs-2","utf-8");
	iconv(cd,&in,&l1,&out,&l2);
	dump("unicode-16:",&dst[0],4);
	printf("l1:%d l2:%d\n",l1,l2);
	iconv_close(cd);


	print_hz("啊");
	print_hz("爱");
	print_hz("反");
	print_hz("°");
}
int main(){
	vector<std::string> bb {"1","2"};
	myswap(begin(bb),begin(bb)+1);
	
	int a;
	int i;
	int *c;
	int *d;
	int ch;
	unsigned long e,f;
	unsigned char ddd;
	long eee;
	List<int> list;
	a=1;
	list.push_back(a);
	a=2;
	list.push_back(a);
	cout<<list<<endl;

	List<int>::iterator it=list.begin();
	List<int>::iterator it2;
	it2=it;

	while(it!=list.end()){
		cout<<*it<<endl;
		a=it->data;
		it++;
	}
	cout<<ll.max_size()<<endl;
	cout<<ll.max_size()<<endl;
	ll.push_back(1);
	ll.push_back(2);
	ll.push_back(3);
	ll.push_back(4);
	ll.push_back(5);
	ll.push_back(6);
	ll.push_back(7);
	cout<<ll<<endl;
	
	cq.init(10);
	for(i=0;i<23;i++){
		a=i;
		cq.push(a);
		cq.pop();
	}
	for(;i<148;i++){
		a=i;
		cq.push_pop(a);
	}
	CircleQueue<int>::iterator cit(cq.MaxQueue);
	cit=cq.begin();
	cout<<*cit<<endl;
	a=cit[0];
	cout<<a<<endl;
	a=cit[1];
	cout<<a<<endl;
	a=cit[2];
	cout<<a<<endl;
	a=cit[3];
	cout<<a<<endl;
	c=&cit[0];
	cout<<*c<<endl;
	c=&cit[1];
	cout<<*c<<endl;
	c=&cit[2];
	cout<<*c<<endl;
	c=&cit[3];
	cout<<*c<<endl;

	while(cit!=cq.end()){
		cout<<*cit<<"(";
		c=&cit;
		cout<<c<<"),";
		cit++;
	}
	cout<<endl;
	d=NULL;
	get_data(d);
	cout<<d<<endl;
	e=PROCESS_YC_CHANGE;
	f=~e;
	printf("%x %x ",PROCESS_YC_CHANGE,~PROCESS_YC_CHANGE);
	printf("%lx %lx ",e,f);
	dir_list::iterator dir_it;
	ddd=1;
	eee=ddd;
	eee|=ddd<<8;
	eee|=ddd<<16;
	eee|=ddd<<24;
	printf("%lx \n",eee);
	a='/';
	cout<<isascii(a)<<","<<isalpha(a)<<","<<isxdigit(a)<<","<<isdigit(a)<<endl;
	read_file();
	test_iconv();
	init_kb();
	while(1){
		//printf("press any key\n");
		//a=getchar();
		//a=mygetc();
		if(kbhit()){
			ch=readch();
			printf("get %c %x %x\n",ch,ch,'q');
			if(ch=='q')
				break;
		}
	}
	close_kb();
	return 0;
}

// vim:tw=72 
