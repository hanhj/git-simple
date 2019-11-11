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
template <typename T>
class _node{
	public:
		T data;
		_node<T> *next;
};
template<typename T>
class _list_iterator{
	public:
/*		
	typedef typename T::differenct_type differenct_type;
	typedef typename T::value_type value_type;
	typedef typename T::pointer pointer;
*/
	typedef _list_iterator<T> self;
	_node<T> *node;
	_list_iterator(){

	}
	_list_iterator(_node<T> *n):node(n){
		
	}
	T&  operator *(){
		return (node->data);
	}
	T & operator ->(){
		return *node;
	}

	self  operator++(){
		node=node->next;
		return (self)node;
	}
	self operator ++(int){
		_node<T> *tmp;
		tmp=node;
		node=node->next;
		return (self)tmp;
	}
	bool operator !=(_list_iterator<T> *da){
		return node!=da->node;
	}
	bool operator ==(_list_iterator<T> *da){
		return node == da->node;
	}

};
template <typename T>
class List{
	public:
		int size;
		typedef _list_iterator<T>*  iterator;
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
			return iterator(tail);
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

	List<int>::iterator it;
	it=list.begin();
	//cout<<*it<<endl;
	cout<<it->data;

	
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
