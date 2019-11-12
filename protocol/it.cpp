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

/*如何使用迭代器：
 * 1、使用迭代器需要的类：
 * 节点类，用于封装数据。
 * 容器类，用于封装容器,容纳数据。
 * 迭代类，用于封装指针。用于遍历容器类中的节点数据。
 * 2.各个类中的要求
 * 2.1节点类没有什么特殊要求。
 * 2.2迭代类，
 *	定义一个节点指针用于记录当前数据位置。
 *	定义构造函数，赋值节点指针。
 *	重载* 解 操作函数，返回值为节点的数据引用。
 *	重载前++，后++操作符函数，
 *		返回值是迭代类的应用或数。
 *		前者返回引用，因为改变了数据指针位置，或者数。前++：本身变，返回变
 *		后者返回一个数。后++：本身变，返回不变。
 *	->指向函数
 *		返回节点指针。
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
int get_data(){
	int ret;
	ret=0;
	vector<data>::iterator it;
	it=vec.begin();
	while(it!=vec.end()){
		if(it->flag==0){
			it->flag=1;
			cout<<it->value<<endl;
			ret = 1;
			break;
		}
		it++;
	}
	return ret;
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
class AA{
	public:
		T aa;
		AA(const T &da):aa(da){

		}
		T & operator *(){
			cout<<"aaa:"<<aa<<endl;
			return aa;
		}
};
int main(){
	vector<std::string> bb {"1","2"};
	myswap(begin(bb),begin(bb)+1);
	
	int a;
	List<int> list;
	a=1;
	list.push_back(a);
	a=2;
	list.push_back(a);
	cout<<list<<endl;

	List<int>::iterator it=list.begin();
	List<int>::iterator it2;

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
/*
	data da;
	da.value=1;
	da.flag=0;
	vec.push_back(da);
	da.value=2;
	da.flag=0;
	vec.push_back(da);
	da.value=3;
	da.flag=0;
	vec.push_back(da);
	cout<<vec;

	get_data();
	get_data();
	get_data();
	get_data();
	get_data();
	get_data();
*/
	return 0;
}

// vim:tw=72 
