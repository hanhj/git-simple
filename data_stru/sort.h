/*
 * File Name: sort.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-21 01:31:05 week:5
*/
#ifndef __sort_h
#define __sort_h
#include "data_type.h"
#include "common.h"
template <class T>
void dump(T*data,int size){
	int i;
	i=0;
	while(size--){
		cout<<data[i]<<",";
		i++;
	}
	cout<<endl;
}
/* 
 * the theory of insert_sort:
 * in a list,i assume the front part is ordered,and the tail part
 * is unordered.
 * i get tail's element one by one,and comare it with front elements
 * if it is smaller,then move current data to tail ,and leave a space for
 * smaller data.otherwise not do this.
 * at first the ordered part is first data.
 * 
 * the essence of insert_sort is comapre and move.
 * the more of unordered elements,the more times of compare.
 * if list is ordered ,the complex of time is O(n),
 * if list is total unordered ,the complex of time is O(n^2)
 */
template <class T>
void insert_sort(T *data,int size){
	int i;
	int j;
	T tp;
	int counts;
	counts=0;
	cout<<"insert_sort "<<endl;
	for(i=1;i<size;i++){//assume ordered list is first data
		tp=data[i];
		j=i-1;
		while(tp<data[j]&&j>=0){
			data[j+1]=data[j];
			j--;
			counts++;
		}
		data[j+1]=tp;
		dump<T>(data,size);
		counts++;
	}
	cout<<"counts:"<<counts<<endl;
}

template <class T>
void shell_sort(T *data,int size){
	int d[3];//span of group
	int i;
	int j;
	int k;
	int counts;
	T tp;
	int loops=3;//loops
	d[0]=5;
	d[1]=3;
	d[2]=1;
	i=0;
	j=0;
	k=0;
	counts=0;
	cout<<"shell_sort "<<endl;
	while(k<loops){
		for(i=d[k];i<size;i++){
			tp=data[i];
			j=i-d[k];
			while(j>=0&&tp<data[j]){
				data[j+d[k]]=data[j];
				j-=d[k];
				counts++;
			}
			data[j+d[k]]=tp;
		}
		k++;
		counts++;
		dump<T>(data,size);
	}
	cout<<"counts:"<<counts<<endl;
}
void test_sort();

#endif

// vim:tw=72 
