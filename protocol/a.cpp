/*
 * File Name: a.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-11-08 15:08:48 week:5
*/
#include<iostream>
using namespace std;
#include <list>
#include <vector>
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
	data da;
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
int main(){
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


	return 0;
}

// vim:tw=72 
