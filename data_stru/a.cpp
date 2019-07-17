/*
 * File Name: a.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-23 16:50:41 week:0
*/
#include<iostream>
using namespace std;
#include <stdlib.h>
#include <stdio.h>
void t1(int a,int *&b){
	b=(int *)malloc(sizeof(int));
}
void t2(int a,int *b){
	b=(int *)malloc(sizeof(int));
}
void t3(int a,int **b){
	*b=(int *)malloc(sizeof(int));
}
void t4(int size,int **b){
	for(int i=0;i<size;i++){
		for(int j=0;j<size;j++)
			cout<<b[i][j]<<",";
	}
	cout<<"b[0]:"<<b[0]<<" b[1]:"<<b[1]<<endl;
	cout<<endl;
}
int main(){
	int i;
	int key;

	cout<<"随机数"<<endl;
	cout<<"input key"<<endl;
	scanf("%d",&key);
	srandom(key);
	for(i=0;i<3;i++){
		cout<<rand()<<endl;
		cout<<random()<<endl;
		cout<<(float)rand()/RAND_MAX<<endl;
		cout<<(float)random()/RAND_MAX<<endl;
	}
	cout<<"指针的使用"<<endl;
	int array[4];
	int **pp;
	int *p;

	cout<<"int **pp;pp=new int*[4];set pp[i]=some ptr;and show pp[i]"<<endl;
	pp=new int*[4];
	for(i=0;i<4;i++){
		pp[i]=&array[i];
		cout<<pp[i]<<endl;
	}
	delete []pp;

	cout<<"int *p;call fun(int *p),in fun p=malloc..;and show p "<<endl;
	p=NULL;
	cout<<"before call fun p:"<<p<<endl;
	t2(2,p);
	cout<<"after call fun p:"<<p<<endl;
	free(p);

	cout<<"int *p; call fun(int **p),in fun *p=malloc..;and show p "<<endl;
	p=NULL;
	cout<<"before call fun p:"<<p<<endl;
	t3(2,&p);
	cout<<"after call fun p:"<<p<<endl;
	free(p);
	
	p=NULL;
	cout<<"int *p; call fun(int *&p),in fun p=malloc..;and show p "<<endl;
	cout<<"before call fun p:"<<p<<endl;
	t1(2,p);
	cout<<"after call fun p:"<<p<<endl;
	free(p);

	cout<<"数组与指针"<<endl;
	int darray[][2]={
		{1,2},
		{3,4}
	};
	int *p2[2];
	p2[0]=darray[0];
	p2[1]=darray[1];
	cout<<"p2[0] "<<p2[0]<<" p2[1] "<<p2[1]<<endl;
	int **pp2;
	pp2=&p2[0];
	t4(2,&p2[0]);

	const char *s="abc";
	cout<<s<<endl;
	int date=123;
	int *dp=&date;
	cout<<"dp "<<dp<<",*dp "<<*dp<<endl;
	return 0;
}

// vim:tw=72 
