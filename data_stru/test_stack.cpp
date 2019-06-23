/*
 * File Name: stack.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-17 19:10:25 week:1
*/
#include "stack.h"

void test_stack(){
	//ArrayStack<int> stack;
	LinkStack<int> stack;
	cout<<"test stack................................."<<endl;
	stack.push(1);
	stack.push(2);
	stack.push(3);
	stack.push(4);
	stack.push(5);
	stack.print();
	if(stack.isEmpty()){
		cout<<"stack is empty"<<endl;
	}else
		cout<<"stack is not empty"<<endl;
	int da;
	stack.pop(&da);
	cout<<"pop "<<da<<endl;
	stack.print();
	stack.top(&da);
	cout<<"top "<<da<<endl;
	stack.print();
}
// vim:tw=72 
