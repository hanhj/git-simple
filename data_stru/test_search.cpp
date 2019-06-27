/*
 * File Name: test_search.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-22 09:04:04 week:6
*/
#include <string.h>
#include "search.h"
#if HashType ==1
int Hash(const char * key,int size,int count){
	int hash;
	hash=((key[0]^2)+((key[1]<<4)^2))%size+count;
	if(hash>size-1)
		return -1;
	return hash;
}
#elif HashType ==2
int Hash(const char * key,int size,int count){
	int hash;
	hash=((key[0]^2)+((key[1]<<4)^2))%size;
	if(hash>size-1)
		return -1;
	return hash;
}
#endif 

void test_search(){
	cout<<"\ntest search ............................"<<endl;
	int pos;
	int key;
	int ret;
	int i;
	//for array search,binary_search
	int array[]={
		0,1,2,3,4,6,9,10,12
	};	
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
	key=19;
	ret=index_search<int>(index,data,key,3,&pos);
	if(ret){
		cout<<"find key:"<<key<<" at "<<pos<<endl;
	}else
		cout<<"not find key:"<<key<<endl;
	cout<<"test hash search_hash"<<endl;
	
	//for hash search 
	HashMap<string> *key_map;
	char key_str[][10]={
		{"101"},
		{"3"  },
		{"10" },
		{"4"  },
		{"6"  },
		{"7"  },
		{"9"  },
		{"2"  }
	};
	
	int hash;
	int key_map_size;
	key_map_size=(int)sizeof(key_str)/(int)sizeof(key_str[0]);
	init_hash_map<string>(&key_map,TableSize);
	for(i=0;i<key_map_size;i++){
		make_hash<string>(key_str[i],key_map,TableSize,&hash);
	}
	dump_hash_map<string>(key_map,TableSize);
	char strkey[]="10";
	search_hash<string>(strkey,key_map,TableSize,&hash);
	clear_hash_map<string>(key_map,TableSize);
}
// vim:tw=72 
