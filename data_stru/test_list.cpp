#include "list.h"
class BigInt{
	private:
		LinkList<int> list;
		char ori[100];
	public:
		BigInt();
		BigInt(BigInt &);
		void init();
		void set_data(const char*);
		BigInt& operator+(BigInt& );
		friend ostream& operator <<(ostream &,BigInt &);
};

BigInt::BigInt(){
	memset(ori,0,sizeof(ori));
}
BigInt::BigInt(BigInt &da){
	list=da.list;
	cout<<"copy"<<endl;
}
/*
BigInt& BigInt::operator = (BigInt& da){
    list=da.list;
	cout<<"bbb"<<endl;
	return *this;
}
*/
void BigInt::set_data(const char *da){
	strcpy(ori,da);
	init();
}
/*
 * @brief transfer a number str into a number list. low byte is at head ,high byte at tail.
 * @param[in]
 * @param[out]
 * @return none
 * @note .4 byte per node
 */
void BigInt::init(){
	int i;
	int len;
	int da;
	char tmp[5];

	len=strlen(ori);
	for(i=0;i<len;){
		memset(tmp,0,5);
		strncpy(tmp,&ori[i],4);
		i+=4;
		da=atoi(tmp);
		list.insert_head(da);
	}
	list.print();
}
/*
 * @brief : add two big int number
 * @param[in] BigInt &b
 * @param[out]
 * @return : result of BigInt
 */
BigInt& BigInt::operator+(BigInt &b){
	BigInt a;
	a=*this;
	list.clear();
	int res;
	char tmp[10];
	int i,j;
	int m,n;
	int up,low;
	int *da,*db;
	m=a.list.get_size();
	n=b.list.get_size();
	int st;
	up=0;
	for(i=1,j=1;i<=m&&j<=n;){
		st=a.list.locate_element(i,da);
		if(st==-1)
			break;
		i++;
		st=b.list.locate_element(j,db);
		if(st==-1)
			break;
		j++;
		res=(*da)+(*db)+up;
		sprintf(tmp,"%d",res);
		if(strlen(tmp)>4){
			char tt[2];
			tt[0]=tmp[0];
			tt[1]=0;
			up=atoi(tt);
			low=atoi(&tmp[1]);
			list.insert_tail(low);
		}else{
			list.insert_tail(res);
			up=0;
		}
	}
	if(i<=m){
		for(;i<=m;++i){
			a.list.locate_element(i,da);
			if(up){
				*da=*da+up;
				up=0;
			}
			list.insert_tail(*da);
		}
	}
	if(j<=n){
		for(;j<=n;++j){
			b.list.locate_element(j,db);
			if(up){
				*db=*db+up;
				up=0;
			}
			list.insert_tail(*db);
		}
	}
	return *this;
}
ostream& operator <<(ostream &os,BigInt& da){
	return os<<da.list<<endl;
}
	
void test_bas_list(){
	//ArrayList<int> list;
	//ArrayList<int> list2;
	//ArrayList<int> list3;
	LinkList<int> list;
	LinkList<int> list2;
	LinkList<int> list3;

    int pos;
    int *data;
	cout<<"test list.........................."<<endl;
	
    list.insert_head(15);
    list.insert_head(5);
    list.insert_head(4);
    list.insert_head(3);
    list.insert_head(2);
    list.insert_head(1);
	list.print();

	list2.insert_head(10);
	list2.insert_head(9);
	list2.insert_head(8);
	list2.insert_head(7);
	list2.print();

	list.merge_sort_list(&list,&list2,&list3);
	list3.print();
	Node<int> *node;
	list.locate_element(3,node);
	cout<<"node "<<node<<" data "<<node->data<<endl;

	list.print();
    list.locate_element(3,data);
    cout<<"pos 3 data is "<<*data<<endl;
    list.search_element(3,&pos);
    cout<<"element 3 pos is "<<pos<<endl;
	cout<<"insert pos 0 element is -1"<<endl;
	list.insert_pos(-1,0);
	list.print();
	cout<<"insert pos 10 element is 10"<<endl;
	list.insert_pos(10,10);
	list.print();
	cout<<"insert pos 101 element is 101"<<endl;
	list.insert_pos(101,101);
	list.print();
	cout<<"delete element 3"<<endl;
	list.delete_element(3);
	list.print();
	cout<<"delete pos 3 "<<endl;
	list.delete_pos(3);
	list.print();
	cout<<"delet pos 1"<<endl;
	list.delete_pos(1);
	list.print();

	list.merge_list(&list,&list2);
	list.print();

	BB<int> b;
	int tt=10;
	int *p_tt;
	p_tt=&tt;
	cout<<p_tt<<endl;
	b.fun(p_tt);
	cout<<p_tt<<" "<<*p_tt<<endl;

	BigInt big,big2,big3;
	big.set_data("123456789000");
	big2.set_data("66664560");
	big3=big+big2;
	cout<<big3;

	BigInt bb=big3;
	//big3.list.reverse_list(&big3.list);
	//big3.list.print();

}

