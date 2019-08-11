/*
 * File Name: test_queue.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-18 20:54:56 week:2
*/
#include "queue.h"
#include <queue>
#include <list>
using namespace std;
ostream &operator<<(ostream &os,list<int>&da){
	list<int>::iterator it;
	it=da.begin();
	while(it!=da.end()){
		os<<*it<<",";
		it++;
	}
	return os;
}
void test_queue(){
	CircleQueue<int> c_queue;
	//ArrayQueue<int> a_queue;
	LinkQueue<int> l_queue;

	int da;
	int i;
	cout<<"test l_queue................................"<<endl;
	l_queue.add(1);
	l_queue.add(2);
	l_queue.add(3);
	l_queue.add(4);
	l_queue.add(5);
	l_queue.print();
	cout<<"size "<<l_queue.get_size()<<endl;
	cout<<"is empty "<<l_queue.isEmpty()<<endl;
	cout<<"is full "<<l_queue.isFull()<<endl;
	int len;
	len=l_queue.get_size();
	for(i=0;i<len;i++){
		l_queue.dequeue(da);
		cout<<"deque "<<da<<endl;
	}
	for(i=0;i<120;i++){
		c_queue.add(i);
	}
	int a;
	c_queue.print();
	c_queue.dequeue(a);
	cout<<a<<endl;
	c_queue.print();

	list<int>qq;
	cout<<qq.front();
	qq.push_back(1);
	qq.push_back(2);
	qq.push_back(3);
	qq.push_back(4);
	cout<<qq.front();
	cout<<qq<<endl;
}

// vim:tw=72 
