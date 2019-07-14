/*
 * File Name: test_recurse.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-30 09:03:05 week:0
*/
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
void test_recurse(){
	cout<<"\ntest_recurse....."<<endl;
	cout<<"recurse fact(6)="<<fact(6)<<endl;
	cout<<"norecurse fact(6)="<<norc_fact(6)<<endl;
	int i;
	cout<<"recurse fabonacci(21)=";
	for(i=0;i<21;i++){
		cout<<fabonacci(i)<<",";
	}
	cout<<endl;
	cout<<"norecurse fabonacci(21)=";
	for(i=0;i<21;i++){
		cout<<norc_fabonacci(i)<<",";
	}
	cout<<endl;
	string str="helleh";
	cout<<str<<" is palindrome:"<<isPalindrome(str,0,str.length()-1)<<endl;
	cout<<str<<" is palindrome:"<<norc_isPalindrome(str)<<endl;
}
// vim:tw=72 
