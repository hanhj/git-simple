/*
 * File Name: test_recurse.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-30 09:03:05 week:0
*/
#include <sys/time.h>
#include "common.h"
#include "recurse.h"
/*fractionate and recurse*/
long fact(long n){
	if(n==1)
		return 1;
	return n*fact(n-1);
}
long norc_fact(int n){
	int i;
	i=1;
	long ret=1;
	while(i<=n){
		ret=ret*i;
		i++;
	}
	return ret;
}
long fabonacci(long n){
	if(n<2)
		return n;
	return fabonacci(n-2)+fabonacci(n-1);
}
long norc_fabonacci(long n){
	long pre,now,next;
	pre=0;
	now=1;
	if(n<2)
		return n;
	int i=2;
	while(i<=n){
		next=pre+now;
		pre=now;
		now=next;
		i++;
	}
	return next;
}
int isPalindrome(const string &s,int h,int t){
	if(h>=t||s.empty()==1)
		return 1;
	if(s[h]!=s[t])
		return 0;
	else{
		return isPalindrome(s,++h,--t);
	}
}
int norc_isPalindrome(const string &s){
	int h=0;
	int t=s.length()-1;
	while(h<=t){
		if(s[h]!=s[t])
			return 0;
		h++;
		t--;
	}
	return 1;
}
//打印数组的全排列
void swap(int& m,int &n){
	int b;
	b=m;
	m=n;
	n=b;
}
void permutation(int a[],int k,int n){
	int i;
	if(k==n){
		int nn;
		int j;
		nn=n;
		j=0;
		while(nn--){
			cout<<a[j++]<<",";
		}
		cout<<endl;
	}else{
		for(i=k;i<n;i++){
			swap(a[i],a[k]);
			permutation(a,k+1,n);
			swap(a[k],a[i]);
		}
	}
}
long q_power(int x,int n){
	int mid;
	long t;
	if(n==0)
		return 1;
	if(n==1)
		return x;
	mid=n/2;
	t=q_power(x,mid);
	if(n%2==0)
		return t*t;
	else
		return t*t*x;
}
long n_power(int x,int n){
	int i;
	long t;
	t=1;
	for(i=0;i<n;i++){
		t=t*x;
	}
	return t;
}
void test_recurse(){
	struct timeval t1,t2;
	float diff;
	cout<<"\ntest_recurse....."<<endl;
	cout<<"recurse fact(6)="<<fact(6)<<endl;
	cout<<"norecurse fact(6)="<<norc_fact(6)<<endl;
	int i;
	int N;
	N=12;
	cout<<"recurse fabonacci:"<<N<<endl;
	gettimeofday(&t1,NULL);
	for(i=0;i<N;i++){
		cout<<fabonacci(i)<<",";
	}
	cout<<endl;
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;
	
	cout<<"norecurse fabonacci:"<<N<<endl;
	gettimeofday(&t1,NULL);
	for(i=0;i<N;i++){
		cout<<norc_fabonacci(i)<<",";
	}
	cout<<endl;
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;
	
	string str="helleh";
	cout<<str<<" is palindrome:"<<isPalindrome(str,0,str.length()-1)<<endl;
	cout<<str<<" is palindrome:"<<norc_isPalindrome(str)<<endl;

	int array[]={
		1,2,3,4
	};
	cout<<"全排列"<<endl;
	permutation(array,0,4);

	long ret;
	gettimeofday(&t1,NULL);
	ret=q_power(2,20);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;
	cout<<"res:"<<ret<<endl;

	gettimeofday(&t1,NULL);
	ret=n_power(2,20);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;
	cout<<"res:"<<ret;

}
// vim:tw=72 
