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
/* 
 * the theory of insert_sort:
 * in a list,i assume the front part is ordered,and the tail part
 * is unordered.
 * i get tail's element one by one,and comare it with front elements
 * if it is smaller,then move current data to tail ,and leave a space for
 * smaller data,otherwise not do this.
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
	for(i=1;i<size;i++){//assume ordered list is first data
		tp=data[i];
		j=i-1;
		while(j>=0 && tp<data[j]){
			data[j+1]=data[j];
			j--;
			counts++;
		}
		data[j+1]=tp;
		counts++;
	}
	//cout<<"counts:"<<counts<<endl;
//	dump<T>(data,size,"result:");
}
/*
  shell sort is based insert_sort.
  it build some lists by a sperator.
  then sort this list by insert_sort
  and repeat this routune by decrease sperator,until 1
*/
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
	}
	//cout<<"counts:"<<counts<<endl;
//	dump<T>(data,size,"result:");
}
/* 
   select sort theory:
   if tail element is smaller of head element,swap it.

*/
template <class T>
void select_sort(T *data,int size){
	int i;
	int j;
	int k;
	int counts=0;
	T x;
	for(i=0;i<size-1;i++){
		k=i;
		for(j=i+1;j<size;j++){
			if(data[k]>data[j])
				k=j;
			counts++;
		}
		if(i!=k){
			x=data[i];
			data[i]=data[k];
			data[k]=x;
		}
		counts++;
	}
	//cout<<"counts:"<<counts<<endl;
//	dump<T>(data,size,"result:");
}
//merge sort theory:
//like merge two sorted list into a list.
//at first i assume there n sorted list,and then merge adjoin elemnt
//into a list,then this list is sorted.
//do this repeat until all data is combined.
//for example:
//3 0 2 9
//first merge: merge 3,0 and 2,9 result is 0 3 2 9
//second merge:merge 0 3,2 9 result is 0 2 3 9
//time :O(nlog2()n)
//space O(n)
template <class T>
void merge(T *data,T *tmp,int left,int center,int right,int& counts){
	int i,j;
	int total_count;
	int k=0;
	i=left;
	j=center+1;
	k=left;
	total_count=right-left+1;
	//cout<<"merge:"<<" l:"<<left<<" r:"<<right<<" c:"<<center<<endl;
	while(i<=center&&j<=right){
		if(data[i]<data[j]){
			tmp[k++]=data[i];
			i++;
		}else{
			tmp[k++]=data[j];
			j++;
		}
		counts++;
	}
	while(i<=center){
		tmp[k++]=data[i++];
		counts++;
	}
	while(j<=right){
		tmp[k++]=data[j++];
		counts++;
	}
	i=left;
	int c;
	c=0;
	while(c<total_count){
		data[i]=tmp[i];
		i++;
		c++;
	}
	//dump(&data[left],c);
}
template <class T>
void msort(T*data,T*tmp,int left,int right,int& counts){
	int center;
	if(left<right){
		center=(left+right)/2;
	//	cout<<"l:"<<left<<" r:"<<right<<" c:"<<center<<endl;
		msort(data,tmp,left,center,counts);
		msort(data,tmp,center+1,right,counts);
		merge(data,tmp,left,center,right,counts);

	}else{
	//	cout<<"ignore l:"<<left<<" r:"<<right<<endl;
	}
}
template <class T>
void merge_sort(T *data,int size){
	T *tmp;
	int counts=0;
	tmp=(T*)malloc(sizeof(T)*size);
	if(tmp!=NULL){
		msort(data,tmp,0,size-1,counts);
		free(tmp);
	//	cout<<"counts:"<<counts<<endl;
//		dump<T>(data,size,"result:");
	}else
		cout<<"fail malloc"<<endl;
}

template <class T>
void bubble_sort(T *data,int size){
	int counts;
	counts=0;
	int m;
	int lastEx;
	m=size-1;
	lastEx=m;
	int exFlag;
	exFlag=1;
	int i;
	i=0;
	T x;
	while(m>0&&exFlag){
		exFlag=0;
		for(i=0;i<m;i++){
			if(data[i]>data[i+1]){
				x=data[i];
				data[i]=data[i+1];
				data[i+1]=x;
				lastEx=i;
				exFlag=1;
			}
			counts++;
		}
		//cout<<"m:"<<m<<endl;
		//dump(data,size);
		m=lastEx;
	}
	//cout<<"counts:"<<counts<<endl;
//	dump<T>(data,size,"result:");
}
template <class T>
T get_mid(T a,T b,T c){
	if(a>b&&b>c)
		return b;
	if(a<b&&b>c&&a>c)
		return a;
	if(a<b&&b>c&&a<c)
		return c;
	if(a<b&&b<c)
		return b;
	return a;
}
template <class T>
void swap(T*a,T*b){
	T c;
	c=*a;
	*a=*b;
	*b=c;
}
template <class T>
void _quick_sort(T *data,int left,int right){
	T x;
	int i;
	int j;
	if(left>right)
		return;
	i=left;
	j=right;
//	int mid=(left+right)/2;
//	x=get_mid(data[left],data[mid],data[right]);
	x=data[left];
	while(i<j){
		while(i<j&&data[j]>x)
			j--;
		if(i<j){
			data[i]=data[j];
			data[j]=x;
			i++;
		}
		while(i<j&&data[i]<x)
			i++;
		if(i<j){
			data[j]=data[i];
			data[i]=x;
			j--;
		}
	}
	_quick_sort(data,left,j-1);
	_quick_sort(data,j+1,right);
}
template <class T>
void _quick_sort2(T *data,int left,int right){
	int i,j;
	T x;
	if(left>right)
		return;
	x=data[left];
	i=left;
	j=right+1;
	while(1){
		while(i<right && data[++i]<x);
		while(j>left && data[--j]>x);
		if(i>=j)break;
		swap(&data[i],&data[j]);
	}
	swap(&data[left],&data[j]);
	_quick_sort2(data,left,j-1);
	_quick_sort2(data,j+1,right);
}
template <class T>
void quick_sort(T *data,int size){
	_quick_sort(data,0,size-1);
}

template <class T>
void heap_sort(T *data,int size);

template <class T1,class T2,class T3>
class mul_key{
	public:
		T1 key1;
		T2 key2;
		T3 key3;
};
template <class T1,class T2,class T3>
void mul_sort(mul_key<T1,T2,T3> *data,int size);

void test_sort();
#endif

// vim:tw=72 
