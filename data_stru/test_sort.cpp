/*
 * File Name: test_sort.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-24 12:50:40 week:1
*/
#include <sys/time.h>
#include <unistd.h>
#include "sort.h"
#include "tree.h"
static int compare(const void *a,const void *b){
	return *(int*)a>*(int *)b;
}
void test_sort(){
	cout<<"\ntest sort............"<<endl;
	int size=1000;
	int i;
	/*
	int tmp_data[size]={
		4,0,5,3,2,1,9,21,8,18
		//21,18,9,8,5,4,3,2,1,0
		//0,1,2,3,4,5,8,9,18,21
	};
	int data[size];
	*/
	struct timeval t1,t2;
	float diff;
	int *tmp_data;
	int *data;
	tmp_data=(int *)malloc(sizeof(int)*size);
	data=(int *)malloc(sizeof(int)*size);
	for(i=0;i<size;i++){
		tmp_data[i]=rand()*1.0/RAND_MAX*100;
	}

	cout<<"sort:"<<size<<" elements"<<endl;
	
	memcpy(data,tmp_data,sizeof(int)*size);
	//dump<int>(data,size,"\norigin:");
	cout<<"insert_sort:T(n)=O(n^2) "<<endl;
	gettimeofday(&t1,NULL);
	insert_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	//dump<int>(data,size,"\norigin:"  );
	cout<<"shell_sort:T(n)=O(n^2) "<<endl;
	gettimeofday(&t1,NULL);
	shell_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	//dump<int>(data,size,"\norigin:");
	cout<<"select_sort:T(n)=O(n^2) "<<endl;
	gettimeofday(&t1,NULL);
	select_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	cout<<"merge_sort:T(n)=O(nlogn) "<<endl;
	//dump<int>(data,size,"\norigin:");
	gettimeofday(&t1,NULL);
	merge_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	cout<<"bubble_sort:T(n)=O(n^2) "<<endl;
	gettimeofday(&t1,NULL);
	bubble_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	cout<<"qsort:T(n)=O(nlogn) "<<endl;
	gettimeofday(&t1,NULL);
	qsort(data,size,sizeof(int*),compare);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;

	memcpy(data,tmp_data,sizeof(int)*size);
	//dump(data,size);
	cout<<"myqsort: T(n)=O(nlogn)"<<endl;
	gettimeofday(&t1,NULL);
	quick_sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	//dump(data,size,"sort:");
	cout<<"run time(ms):"<<diff<<endl;

	cout<<"heap sort...."<<endl;
	Heap<int> heap;
	gettimeofday(&t1,NULL);
	heap.sort(data,size);
	gettimeofday(&t2,NULL);
	diff=t2.tv_sec*1000+(float)t2.tv_usec/1000-(t1.tv_sec*1000+(float)t1.tv_usec/1000);
	cout<<"run time(ms):"<<diff<<endl;
	free(data);
	free(tmp_data);
}
// vim:tw=72 
