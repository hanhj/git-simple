/*
 * File Name: search.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-21 01:21:56 week:5
*/
#ifndef __search_h
#define __search_h
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
void test_search();
#endif
// vim:tw=72 
