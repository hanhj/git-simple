/*
 * File Name: search.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-21 01:21:56 week:5
*/
#ifndef __search_h
#define __search_h
#include "data_type.h"
#include "common.h"
/* type means :
  0 means all da is used for data
  1 means all da is used for data except pos 0.pos 0 is reverse for check data.
 */
template <class T>
int array_search(T key,T *da,int size,int *pos,int type=0){
	int counts;
	counts=0;
	cout<<"test array search,size:"<<size<<" type "<<type<<endl;
	int i;
	if(type==0){
		for(i=size-1;i>=0;i--){
			if(da[i]==key)
				break;
			counts++;
		}
		*pos=i;
		cout<<"counts:"<<counts<<endl;
		return i>=0?1:0;
	}else if(type == 1){
		i=size;
		da[0]=key;
		while(da[0]!=da[i]){
			counts++;
			i--;
		}
		*pos=i;
		cout<<"counts:"<<counts<<endl;
		return i==0?0:1;
	}
	return 0;
}
template <class T>
int binary_search(T key,T *da,int size,int *pos){
	int counts=0;
	cout<<"binary_search size :"<<size<<endl;
	int mid,up,down;
	int ret;
	ret=0;
	up=size-1;
	down=0;
	while(up>=down){
		counts++;
		mid=(up+down)/2;
		cout<<down<<","<<mid<<","<<up<<"("<<da[down]<<","<<da[mid]<<","<<da[up]<<")"<<endl;
		if(key>da[mid]){
			down=mid+1;
		}else if(key<da[mid]){
			up=mid-1;
		}else{
			ret=1;
			break;
		}
	}
	if(ret){
		*pos=mid;
	}
	cout<<"counts:"<<counts<<endl;
	return ret;
}
template <class T>
class IndexTable{
	public:
	T key;//max key of a block.
	int pos;
};
template <class T>
class DataTable{
	public:
	T key;
	int other_data;
};

/* @brief search data by index.
   @param[in] ls is index table;
   @param[in] s is data table;
   @param[in] key is key;
   @param[in] m is blocks of index table.
   @param[out] pos is postion of data in data table.
   @return on sucess 1
			on error 0
   @note: ls must sequence from low to high
		in index table,key is max key of this block,pos is first
		data position 
*/
template <class T>
int index_search(IndexTable<T>*ls,DataTable<T>*s,T key,int m,int *pos){
	int i,j;
	i=0;
	cout<<"test index search"<<endl;
	while(i<m && key>ls[i].key)/* in index table:key1<key2<key3,so if
		key is < ls[i].key ,this means data is in this block.*/
		i++;
	if(i==m)
		return 0;
	else{//find in index table
		j=ls[i].pos;
		while(key!=s[j].key&&j!=ls[i+1].pos)
			j++;
		if(key==s[j].key){
			*pos=j;
			return 1;
		}
	}
	return 0;
}
//1 use open addr for collision;2 use link addr for collision
#define HashType 2  
#define TableSize 16 
#if HashType==1 //use extend address
template <class T>
class HashMap{
	public:
		T key;
		int  hash;
};
#elif HashType==2 //use link address
template <class T>
class HashAddr{
	public:
		T key;
		int hash;
		HashAddr<T> *next;
};
template <class T>
class HashMap{
	public:
		HashAddr<T> *hashs;
};
#endif 
/*
* 哈希查找是将关键字与地址建立一一映射关系，从而使得查找时间复杂度为O(1)
* 哈希函数建立的映射关系是一种压缩存储，容易引起冲突，即不同关键字具有相同地址，因此需要解决冲突问题。
* 哈希函数评价：1）计算简单;2）地址均匀分布，冲突少。
* 方法：
* 1）数据分析方法
* n个d位的数据，由r个字符构成，取其中s分布比较均匀的位
* 比如 80条记录
* 8 1 3 4 3 2 4
* 8 1 3 7 2 1 4
* 8 1 3 1 5 4 4
* 8 1 3 6 2 1 4
* 8 1 3 9 1 0 4
* 8 1 3 3 1 5 4
* 取其中第3,5位
* 2）为了离散地址，可以将关键字平方，再取其中几位。
* 3）关键字如果特别长，可以将关键字叠加。比如：
* 	d7	d6	d5	d4	d3	d2	d1 d0
* + d15 d14 d13 d12 d11 d10 d9 d8
* 除余法:
* H(key)=key%M
* M是小于Hash表的最大质数，hash table must > all of data size 
* 字符串的哈希函数：
* 可以取其中几位做移位
* 
* 解决冲突的办法：
* 1）开放地址
*	Hi(key)=（H(key)+di）mod m;
* 	Hi是第i次的hash值，H是第一次hash值，m是hash表长
*	1.1）线性探测再散列
*		di可以=c*i
*	 	c可以取1,则每次计算时是将当前hash地址向后移一位。c也可以取2,4.。。
*	1.2）平方探测在散列	
*		di 可以取 1^2,-1^,2^2,-2^2...
*	1.3） 伪随机探测再散列	
*		di 取随机数,需要保证每次的随机数是一样的。
* 2) 再哈希
*	地址冲突时，用不同的哈希函数产生地址。
* 3）链地址
*	链地址方法是将相同hash值的关键字放在同一个地址链表中，该链表中保存
*	地址和关键字。（地址可以不用）。搜索时，查找地址链表，如果关键字相同则
*	返回成功，如果搜索完也没有找到则返回失败。
*	开放地址法是在地址冲突时，将关键字放到别处。
*	链地址法是在地址冲突时，将关键字放到相同地址的链表中。
* 
*/

int Hash(const char * key,int size,int count);
template <class T>
void init_hash_map(HashMap<T> **map,int size);
template <class T>
void clear_hash_map(HashMap<T> *map,int size);
template <class T>
void dump_hash_map(HashMap<T> *map,int size);
int Hash(const char * key,int size,int count);
template <class T>
int make_hash(const char *  key,HashMap<T> * map,int size,int *r);
template <class T>
int search_hash(const char *key,HashMap<T> *map,int size,int *r);

#if HashType==1 //use extend address
/* 用开放地址解决冲突*/
//冲突后地址将发生变化
/* key is key
 * size is data table size
 * count is collision
 */
template <class T>
void init_hash_map(HashMap<T>**m,int size){
	int i;
	HashMap<T> *map;
	map=(HashMap<T>*)malloc(sizeof(HashMap<T>)*size);
	for(i=0;i<size;i++){
		map[i].hash=-1;
	}
	*m=map;
}
template <class T>
void dump_hash_map(HashMap<T> *map,int size){
	int i;
	cout<<"dump key map:"<<endl;
	cout<<"pos\tkey\thash"<<endl;
	for(i=0;i<size;i++){
		cout<<i<<"\t"<<map[i].key<<"\t"<<map[i].hash<<endl;
	}
}
template <class T>
void clear_hash_map(HashMap<T> *map,int size){
	if(map){
			free(map);
	}
}
template <class T>
int make_hash(const char *  key,HashMap<T> * map,int size,int *r){
	int hash;
	int count=0;
	hash=Hash(key,size,count);
	while(map[hash].hash!=-1){
		hash=Hash(key,size,++count);
		if(hash==-1){
			cout<<"hash value is invalid"<<endl;
			return -1;
		}
	}
	map[hash].hash=hash;
	map[hash].key=key;
	*r=hash;
	return 0;
}
/*
 * step:
 * 1) caculate hash
 * 2) if data is not exist return false
 * 3) if data equal key return true,otherwise calculate next_hash repeat
 * to 2)
 *
 */
template <class T>
int search_hash(const char *key,HashMap<T> *map,int size,int *r){
	int hash;
	int ret;
	int count;
	count=0;
	ret=0;
	hash=Hash(key,size,count);
	if(hash<0){
		cout<<"not find "<<key<<",hash valuse is invalidl"<<endl;
		return ret;
	}
	while(map[hash].hash!=-1&&(map[hash].key!=key)){//while collision or not equal
		hash=Hash(key,size,++count);
		if(hash==-1){
			cout<<"not find ,hash value is invalid"<<endl;
			return ret;
		}
	}
	if(map[hash].hash==-1){//not find
		cout<<"not find "<<key<<endl;
	}else if(map[hash].key==key){//find
		ret=1;
		*r=hash;
		cout<<"find key:"<<key<<" hash "<<hash<<" collision "<<count<<endl;
	}
	return ret;
}
#elif HashType==2 //use link address
/* 用链地址解决冲突*/
//在链地址解决方法中冲突后地址不发生变化
/* key is key
 * size is data table size
 * count is collision
 */
template <class T>
void init_hash_map(HashMap<T>**m,int size){
	int i;
	HashMap<T> *map;
	map=(HashMap<T>*)malloc(sizeof(HashMap<T>)*size);
	for(i=0;i<size;i++){
		map[i].hashs=NULL;
	}
	*m=map;
}
template <class T>
void dump_hash_map(HashMap<T> *map,int size){
	int i;
	cout<<"dump key map:"<<endl;
	cout<<"hash\tkey1\tkey2..."<<endl;
	for(i=0;i<size;i++){
		cout<<i<<"\t";
		HashAddr<T> *node;
		node=map[i].hashs;
		while(node){
			cout<<node->key<<"\t";
			node=node->next;
		}
		cout<<endl;
	}
}
template <class T>
void clear_hash_map(HashMap<T> *map,int size){
	int i;
	cout<<"clean key map"<<endl;
	for(i=0;i<size;i++){
		HashAddr<T> *node;
		HashAddr<T> *tp;
		node=map[i].hashs;
		while(node){
			tp=node;
			node=node->next;
			delete tp;
		}
	}
		free(map);
}
template <class T>
int make_hash(const char *  key,HashMap<T> * map,int size,int *r){
	int hash;
	int count=0;
	hash=Hash(key,size,count);
	HashAddr<T> *node;
	node=map[hash].hashs;
	while(node!=NULL){
		node=node->next;
	}
	node=new HashAddr<T>;
	node->hash=hash;
	node->key=key;
	node->next=map[hash].hashs;
	map[hash].hashs=node;
	*r=hash;
	return 0;
}
/*
 * step:
 * 1) caculate hash
 * 2) if data is not exist return false
 * 3) if data equal key return true,otherwise calculate next_hash repeat
 * to 2)
 *
 */
template <class T>
int search_hash(const char *key,HashMap<T> *map,int size,int *r){
	int hash;
	int ret;
	int count;
	HashAddr<T> *node;
	count=0;
	ret=0;
	hash=Hash(key,size,count);
	node=map[hash].hashs;
	while(node!=NULL&&(node->key!=key)){//while collision or not equal
		node=node->next;
		++count;
	}
	if(node==NULL){//not find
		cout<<"not find "<<key<<" node null"<<endl;
	}else if(node->key==key){//find
		ret=1;
		*r=node->hash;
		cout<<"find key:"<<key<<" hash "<<hash<<" collision "<<count<<endl;
	}
	return ret;
}
#endif


void test_search();
#endif
// vim:tw=72 
