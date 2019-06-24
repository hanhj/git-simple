/*
 * File Name: test_search.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-22 09:04:04 week:6
*/
#include <string.h>
#include "search.h"
typedef struct user_data{
	char data[10];
	char use;
}user_data;
//哈希查找是将关键字与地址建立一一映射关系，从而使得查找时间复杂度为O(1)
//哈希函数建立的映射关系是一种压缩存储，容易引起冲突，即不同关键字具有相同地址，因此需要解决冲突问题。
//哈希函数评价：1）计算简单;2）地址均匀分布，冲突少。
//方法：
//1）数据分析方法
//n个d位的数据，由r个字符构成，取其中s分布比较均匀的位
//比如 80条记录
//8 1 3 4 3 2 4
//8 1 3 7 2 1 4
//8 1 3 1 5 4 4
//8 1 3 6 2 1 4
//8 1 3 9 1 0 4
//8 1 3 3 1 5 4
//取其中第3,5位
//2）为了离散地址，可以将关键字平方，再取其中几位。
//3）关键字如果特别长，可以将关键字叠加。比如：
//	d7	d6	d5	d4	d3	d2	d1 d0
//+ d15 d14 d13 d12 d11 d10 d9 d8
//除余法:
//H(key)=key%M
//M是小于Hash表的最大质数，hash table must > all of data size 
//字符串的哈希函数：
//可以取其中几位做移位
//
//解决冲突的办法：
//1）开放地址
//Hi(key)=（H(key)+di）mod m;
//	Hi是第i次的hash值，H是第一次hash值，m是hash表长
//	di可以=c*i
//	,c可以取1,则每次计算时是将当前hash地址向后移一位。c也可以取2,4.。。
//	di 可以取 1^2,-1^,2^2,-2^2...
//	di 可以取随机数
int make_hash(const char * key,user_data * data,int size,int *r){
	int hash;
	int next_hash;
	int ret;
	ret=-1;
	hash=((key[0]^2)+((key[1]<<4)^2))%size;
	if(data[hash].use==0){
		data[hash].use=1;
		strcpy(data[hash].data,key);
		ret=0;
		*r=hash;
	}else{
		int i=1;
		while(1){
			next_hash=hash+i;
			i++;
			if(data[next_hash].use==0){
				data[next_hash].use=1;
				strcpy(data[hash].data,key);
				ret=0;
				*r=next_hash;
				break;
			}
			if(next_hash>size-1)
				break;
		}
	}
	return ret;
}
int search_hash(const char *key,user_data *data,int size,int *r){
	int hash;
	int next_hash;
	int ret;
	ret=0;
	hash=(key[0]+(key[1]<<4))%size;
	if(strcmp(data[hash].data,key)==0){
		ret=1;
		*r=hash;
	}else{
		int i=1;
		while(1){
			next_hash=hash+i;
			i++;
			if(strcmp(data[next_hash].data,key)==0){
				ret=1;
				*r=next_hash;
				break;
			}
			if(next_hash>size-1)
				break;
		}
	}
	return ret;
}
#define TableSize 100
void test_search(){
	//for array search,binary_search
	int array[]={
		0,1,2,3,4,6,9,10,12
	};	
	//for index search
	DataTable<int> data[]={
		{1,0},
		{2,0},
		{3,0},
		{4,0},
		{5,0},
		{6,0},
		{7,0},
		{8,0},
		{9,0},
	};
	IndexTable<int> index[]={
		{3,0},
		{6,3},
		{9,6},
		{9,9}//extern index 
	};
	//for hash search 
	user_data use_data[100];
	char key_map[][100]={
		{"1"},
		{"3"},
		{"10"},
		{"4"},
		{"6"},
		{"7"},
		{"9"},
		{"2"}
	};
	
	cout<<"\ntest search ............................"<<endl;
	int pos;
	int key;
	int ret;
	int i;
	key=1;
	ret=array_search<int>(key,array,sizeof(array)/sizeof(int),&pos,0);
	if(ret){
		cout<<"find key:"<<key<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<key<<endl;
	ret=array_search<int>(key,array,sizeof(array)/sizeof(int)-1,&pos,1);
	if(ret){
		cout<<"find key:"<<key<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<key<<endl;

	ret=binary_search<int>(key,array,sizeof(array)/sizeof(int),&pos);
	if(ret){
		cout<<"find key:"<<key<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<key<<endl;
	key=19;
	ret=index_search<int>(index,data,key,3,&pos);
	if(ret){
		cout<<"find key:"<<key<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<key<<endl;
	cout<<"test hash search_hash"<<endl;
	int hash;
	int key_map_size;
	key_map_size=(int)sizeof(key_map)/(int)sizeof(key_map[0]);
	for(i=0;i<key_map_size;i++){
		ret=make_hash(key_map[i],use_data,TableSize,&hash);
		if(ret==0){
			cout<<key_map[i]<<" hash is:"<<hash<<endl;
		}
	}
	char strkey[]="1";
	ret=search_hash(strkey,use_data,TableSize,&pos);
	if(ret){
		cout<<"find key:"<<strkey<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<strkey<<endl;
}

// vim:tw=72 
