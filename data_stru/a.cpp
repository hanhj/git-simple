/*
 * File Name: a.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-07-16 18:00:50 week:2
*/
#include<iostream>
#include <stdio.h>
using namespace std;
void foo1(int *d,int size){
	cout<<"foo1"<<endl;
	cout<<"(";
	for(int i=0;i<size;i++)
		cout<<d[i]<<" ";
	cout<<")";
	cout<<endl;
}
void foo2(int d[],int size){
	cout<<"foo2"<<endl;
	cout<<"(";
	for(int i=0;i<size;i++)
		cout<<d[i]<<" ";
	cout<<")";
	cout<<endl;
}
void foo3(int d[][2],int size){
	int i,j;
	cout<<"foo3"<<endl;
	cout<<"(";
	for(i=0;i<size;i++){
		cout<<"(";
		for(j=0;j<size;j++)
			printf("%d ",d[i][j]);
		cout<<") ";
	}
	cout<<")";
	cout<<endl;
}
void foo4(int **d,int size){
	int i,j;
	cout<<"foo4"<<endl;
	cout<<"(";
	for(i=0;i<size;i++){
		cout<<"(";
		for(j=0;j<size;j++)
			printf("%d ",d[i][j]);
		cout<<") ";
	}
	cout<<")";
	cout<<endl;
}
int array[]={
	5,6,7,8
};
int darray[2][2]={
	{1,2},
	{3,4}
};
int main(){
	int i,j;
	int *p;
	int **pp;
	int *p1[2];
	cout<<"array:"<<array<<endl;
	cout<<"array[0]:"<<array[0]<<endl;
	cout<<"darray:"<<darray<<endl;
	cout<<"darray[0]:"<<darray[0]<<endl;
	p=NULL;
	cout<<p<<endl;
	p=&darray[0][0];
	for(i=0;i<4;i++)
		cout<<p+i<<" "<<*(p+i)<<endl;
	p1[0]=darray[0];
	p1[1]=darray[1];
	pp=&p1[0];
	pp=p1;
	for(i=0;i<2;i++){
		for(j=0;j<2;j++){
			cout<<"&pp["<<i<<","<<j<<"] is "<<&pp[i][j]<<" data:"<<pp[i][j]<<endl;
		}
	}

	foo1(array,2);
	foo1(&array[0],2);
	foo2(array,2);
	foo2(&array[0],2);
	foo3(darray,2);
	foo4(pp,2);
	return 0;
}
// vim:tw=72 
