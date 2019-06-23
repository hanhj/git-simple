/*
 * File Name: test_queue.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-18 20:54:56 week:2
*/
#include "queue.h"
void test_queue(){
	CircleQueue<int> c_queue;
	//ArrayQueue<int> queue;
	LinkQueue<int> queue;

	int da;
	int i;
	cout<<"test queue................................"<<endl;
	queue.add(1);
	queue.add(2);
	queue.add(3);
	queue.add(4);
	queue.add(5);
	queue.print();
	cout<<"size "<<queue.get_size()<<endl;
	cout<<"is empty "<<queue.isEmpty()<<endl;
	cout<<"is full "<<queue.isFull()<<endl;
	int len;
	len=queue.get_size();
	for(i=0;i<len;i++){
		queue.dequeue(da);
		cout<<"deque "<<da<<endl;
	}
	for(i=0;i<120;i++){
		c_queue.add(i);
	}
	c_queue.print();

}

// vim:tw=72 
