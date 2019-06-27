/*
 * File Name: test_search.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-06-22 09:04:04 week:6
*/
#include <string.h>
#include "search.h"

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
int Hash(const char * key,int size,int count){
	int hash;
	hash=((key[0]^2)+((key[1]<<4)^2))%size+count;
	if(hash>size-1)
		return -1;
	return hash;
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
int Hash(const char * key,int size,int count){
	int hash;
	hash=((key[0]^2)+((key[1]<<4)^2))%size;
	if(hash>size-1)
		return -1;
	return hash;
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
