/*
 * File Name: test_sort.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-24 12:50:40 week:1
*/
#include "sort.h"
void test_sort(){
	cout<<"\ntest sort............"<<endl;
	int tmp_data[]={
		//0,4,5,3,2,1,9,21,8,18
		//21,18,9,8,5,4,3,2,1,0
		0,1,2,3,4,5,8,9,18,21
	};
	int data[10];
	memcpy(data,tmp_data,sizeof(tmp_data));
	int size=sizeof(data)/sizeof(int);
	int i;
	for(i=0;i<size;i++)
		cout<<data[i]<<",";
	cout<<endl;
	insert_sort<int>(data,size);

	memcpy(data,tmp_data,sizeof(tmp_data));
	for(i=0;i<size;i++)
		cout<<data[i]<<",";
	cout<<endl;
	shell_sort<int>(data,size);

}
// vim:tw=72 
