#ifndef __list_h
#define __list_h
#include "data_type.h"
#include "common.h"
/* Model class it's define and realize must in same file,otherwise will report compile error*/
template <class T>
class List{
	public:
		/*in template class,the child class use parent member must use this->member*/
		int size;
    public:
        virtual int init()=0;
        virtual void print()=0;
        virtual int search_element(T element,int*)=0;
        virtual int locate_element(int pos,T*&)=0;
        virtual int locate_element(int pos,Node<T>*&)=0;
        virtual int insert_head(T element)=0;
        virtual int insert_tail(T element)=0;
        virtual int insert_pos(T element,int pos)=0;
        virtual int delete_element(T element)=0;
        virtual int delete_head()=0;
        virtual int delete_tail()=0;
        virtual int delete_pos(int pos)=0;
        virtual int get_size()=0;
        virtual int merge_list(List<T>* la,List<T>*  lb);
        virtual int merge_sort_list(List<T>* la,List<T>* lb,List<T>*lc);
		virtual void clear()=0;
		virtual void destory()=0;
		virtual int isEmpty();
		virtual void reverse_list(List<T>*);
};

#define MaxData 100
template <class T>
class ArrayList:public List<T>{
	public:
		T *data;
		ArrayList();
		ArrayList(ArrayList<T>&);
		~ArrayList();
        int init();
        void print();
        int search_element(T element,int*);
        int locate_element(int pos,T*&);
        int locate_element(int pos,Node<T>*&);
        int insert_head(T element);
        int insert_tail(T element);
        int insert_pos(T element,int pos);
        int delete_element(T element);
        int delete_head();
        int delete_tail();
        int delete_pos(int pos);
        int get_size();
		void clear();
		void destory();
};

template <class T>
class LinkList:public List<T>{
    public:
        Node<T> *head;
	public:
		LinkList();
		~LinkList();
		LinkList(LinkList<T>&);
		LinkList<T> & operator = (LinkList <T> &);
        int init();
        void print();
        int search_element(T element,int*);
        int locate_element(int pos,T*&);
        int locate_element(int pos,Node<T>*&);
        int insert_head(T element);
        int insert_tail(T element);
        int insert_pos(T element,int pos);
        int delete_element(T element);
        int delete_head();
        int delete_tail();
        int delete_pos(int pos);
        int get_size();
		void clear();
		void destory();
};

/*relaize List*/
template <class T>
int List<T>::isEmpty(){
	return size>0?0:1;
}
/*
 * @brief merge elements in list b which not in list a into list a
 * @param[in] List*la,List *lb
 * @param[out] List*la
 * @return on sucess 0,on error -1
 */
template <class T>
int List<T>::merge_list(List<T>* la,List<T>*  lb){
	int ret;
	ret=0;
	int st;
	int i;
	int pos;
	T *da;
	for(i=1;i<=lb->size;i++){
		lb->locate_element(i,da);//get elemnet of list b
		st=la->search_element(*da,&pos);//check if this data is in list a
		if(st==-1){
			st=la->insert_tail(*da);//if not find insert it into list a
			if(st==-1){
				ret=-1;
				break;
			}
		}
	}
	return ret;
}
template <class T>
int List<T>::merge_sort_list(List<T>* la,List<T>* lb,List<T>*lc){
	int i,j,m,n;
	T *da,*db;
	if(la==NULL||lb==NULL||lc==NULL){
		cout<<"list is invalid"<<endl;
		return -1;
	}
	m=la->size;
	n=lb->size;
	for(i=1,j=1;i<=m&&j<=n;){
		la->locate_element(i,da);
		lb->locate_element(j,db);
		if(*da<*db){
			lc->insert_tail(*da);
			i++;
		}else if(*da>*db){
			lc->insert_tail(*db);
			j++;
		}else{
			lc->insert_tail(*da);
			i++;
			j++;
		}
	}
	if(i<=m){
		for(;i<=m;++i){
			la->locate_element(i,da);
			lc->insert_tail(*da);
		}
	}
	if(j<=n){
		for(;j<=n;++j){
			lb->locate_element(j,db);
			lc->insert_tail(*db);
		}
	}
	return 0;
}
/* @brief reverse elements in list
 * @param[in] List *l
 * @param[out] List l
 * @retrun void
 * @note
 *	1.get element from 1 to n/2
 *	2.set element
 */
template <class T>
void List<T>::reverse_list(List<T>*l){
	T *da;
	T *db;
	T dc;
	int i;
	int st;
	for(i=1;i<=l->size/2;i++){
		st=l->locate_element(i,da);
		if(st==-1)
			break;
		st=l->locate_element(l->size-i+1,db);
		if(st==-1)
			break;
		dc=*da;
		*da=*db;
		*db=dc;
	}
}
/*relaize Array list*/
template <class T>
ArrayList<T>::ArrayList(){
	data=NULL;
	init();
	cout<<"use ArrayList"<<endl;
}
template <class T>
ArrayList<T>::~ArrayList(){
	destory();
}
template<class T>
void ArrayList<T>::destory(){
	this->size=0;
	if(data)
		free(data);
	data=NULL;
}
template<class T>
void ArrayList<T>::clear(){
	this->size=0;
}

template <class T>
ArrayList<T>::ArrayList(ArrayList<T>&list){
	init();
	int i;
	for(i=0;i<list.size;i++){
		data[i]=list.data[i];
	}
}
template <class T>
int ArrayList<T>::init(){
	this->size=0;
	if(data==NULL){
		data=(T*)malloc(sizeof(T)*MaxData);
		if(data==NULL)
			return -1;
	}
	return 0;
}
template <class T>
void ArrayList<T>::print(){
	int i;
	if(this->size==0)
		return ;
	for(i=0;i<this->size;i++){
		cout<<data[i]<<" ";
	}
	cout<<endl;
}
template <class T>
int ArrayList<T>::search_element(T element,int *pos){
	int i;
	int ret=-1;

	for(i=0;i<this->size;i++){
		if(element==data[i]){
			*pos=i+1;
			ret=0;
			break;
		}
	}
	return ret;
}
template <class T>
int ArrayList<T>::locate_element(int pos,T*&da){
	if(pos<=0||pos>this->size)
		return -1;
	da=&data[pos-1];
	return 0;
}
template <class T>
int ArrayList<T>::locate_element(int pos,Node<T>*&da){
	return 0;
}
template <class T>
int ArrayList<T>::insert_head(T element){
	int i;
	for(i=this->size-1;i>=0;--i){
		data[i+1]=data[i];
	}
	data[0]=element;
	this->size++;
	return 0;
}
template <class T>
int ArrayList<T>::insert_tail(T element){
	if(this->size>=MaxData){
		cout<<"list is full"<<endl;
		return -1;
	}
	data[this->size]=element;
	this->size++;
	return 0;
}

template <class T>
int ArrayList<T>::insert_pos(T element,int pos){
	if(pos<=0||pos>this->size+1){
		cout<<"range error,pos must 1 to "<<this->size+1<<endl;
		return -1;
	}
	if(this->size>=MaxData){
		cout<<"list is full"<<endl;
		return -1;
	}
	int i;
	for(i=this->size-1;i>=pos-1;--i){
		data[i+1]=data[i];
	}
	data[pos-1]=element;
	this->size++;
	return 0;
}
template <class T>
int ArrayList<T>::delete_element(T element){
	int i;
	int j;
	if(data==NULL){
		cout<<"data is valid"<<endl;
		return -1;
	}
	if(this->size==0){
		cout<<"list is empty"<<endl;
		return -1;
	}
	for(i=0;i<this->size;i++){
		if(data[i]==element)
			break;
	}
	if(i>=this->size){
		cout<<"not find element "<<element<<endl;
		return -1;
	}
	for(j=i;j<this->size;j++){
		data[j]=data[j+1];
	}
	this->size--;
	return 0;
}
template <class T>
int ArrayList<T>::delete_head(){
	int i;
	if(data==NULL){
		cout<<"data is valid"<<endl;
		return -1;
	}
	if(this->size==0){
		cout<<"list is empty"<<endl;
		return -1;
	}
	for(i=0;i<this->size;i++){
		data[i]=data[i+1];
	}
	this->size--;
	return 0;
}
template <class T>
int ArrayList<T>::delete_tail(){
	if(data==NULL){
		cout<<"data is valid"<<endl;
		return -1;
	}
	if(this->size==0){
		cout<<"list is empty"<<endl;
		return -1;
	}
	this->size--;
	return 0;
}

template <class T>
int ArrayList<T>::delete_pos(int pos){
	int i;
	if(pos<=0||pos>this->size){
		cout<<"range error,pos must 1 to "<<this->size<<endl;
		return -1;
	}
	for(i=pos-1;i<this->size;i++){
		data[i]=data[i+1];
	}
	this->size--;
	return 0;
}
template <class T>
int ArrayList<T>::get_size(){
	return this->size;
}
/*relaize link list */
template <class T>
LinkList<T>::LinkList(){
	head=NULL;
	init();
	cout<<"use LinkList"<<endl;
}
template<class T>
LinkList<T>::~LinkList(){
	destory();
}
template <class T>
void LinkList<T>::destory(){
	Node<T> *tp;
	Node<T> *tmp;
	tp=head->next;
	while(tp!=NULL){
		tmp=tp;
		tp=tp->next;
		free(tmp);
		this->size--;
	}
	if(head)
		free(head);
}
template <class T>
void LinkList<T>::clear(){
	if(this->size==0){
		return;
	}
	Node<T> *tp;
	Node<T> *tmp;
	tp=head->next;
	while(tp!=NULL){
		tmp=tp;
		tp=tp->next;
		free(tmp);
		this->size--;
	}
	head->next=NULL;
}
template <class T>
LinkList<T>& LinkList<T>::operator = (LinkList<T>&list){
	if(!this->isEmpty()){
		clear();
	}
	T *da;
	int i;
	int st;
	for(i=1;i<=list.size;i++){
		st=list.locate_element(i,da);
		if(st==-1)
			break;
		st=insert_tail(*da);
		if(st==-1)
			break;
	}
	return *this;
}
template <class T>
int LinkList<T>::init(){
    Node<T> *node;
	if(head==NULL){
		node =(Node<T>*)malloc(sizeof(Node<T>));
		head=node;
		node->next=NULL;
	}
    this->size=0;
    return 0;
}
template <class T>
int LinkList<T>::search_element(T element,int *pos){
    Node<T> *tp;
    tp=head->next;
    int i;
    i=1;
	*pos=-1;
    while(tp){
        if(tp->data==element)
            break;
        tp=tp->next;
        i++;
    }
    if(tp!=NULL){
        *pos=i;
        return 0;
    }else{
		cout<<"not find or list is empty"<<endl;
        return -1;
	}
}
template <class T>
int LinkList<T>::locate_element(int pos,T*&da){
    Node<T> *tp;
    tp=head->next;
    int i=1;
    while(tp){
        if(i==pos)
            break;
        tp=tp->next;
        i++;
    }
    if(tp!=NULL){
       da=&tp->data;
       return 0;
    }else{
		cout<<"range error or list is empty"<<endl;
        return -1;
    }
}
template <class T>
int LinkList<T>::locate_element(int pos,Node<T>*&da){
    Node<T> *tp;
    tp=head->next;
    int i=1;
    while(tp){
        if(i==pos)
            break;
        tp=tp->next;
        i++;
    }
    if(tp!=NULL){
		da=tp;
        return 0;
    }else{
		cout<<"range error or list is empty"<<endl;
        return -1;
    }
}
template<class T>
int LinkList<T>::insert_head(T element){
    int ret;
    ret=-1;
    Node<T> *tp;
    tp=(Node<T>*)malloc(sizeof(Node<T>));
    if(tp!=NULL){
        tp->next=head->next;
        head->next=tp;
        tp->data=element;
        this->size++;
        ret=0;
    }else{
		cout<<"fail malloc "<<endl;
    }
    return ret;
}
template<class T>
int LinkList<T>::insert_tail(T element){
    int ret;
    ret=-1;
    Node<T> *tp;
	Node<T> *tail;
	tail=head->next;
	while(tail){
		if(tail->next==NULL)
			break;
		tail=tail->next;
	}
    tp=(Node<T>*)malloc(sizeof(Node<T>));
    if(tp!=NULL){
        tp->next=NULL;
		if(tail)
			tail->next=tp;
		else
			head->next=tp;
		tp->data=element;
        this->size++;
        ret=0;
}else{
		cout<<"fail malloc "<<endl;
    }
    return ret;
}
template<class T>
int LinkList<T>::insert_pos(T element,int pos){
    int ret;
    ret=-1;
	int i;
	i=1;
    Node<T> *tp;
	Node<T> *pri;
	Node<T> *p;
	pri=head;
	p=head->next;
	while(p){
		if(pos==i){
			break;
		}
		pri=pri->next;
		p=p->next;
		i++;
	}
	if(p==NULL){
		if(pos!=this->size+1){
			cout<<"range error,must 1 to "<<this->size+1<<endl;
			return -1;
		}
	}
    tp=(Node<T>*)malloc(sizeof(Node<T>));
    if(tp!=NULL){
        tp->next=pri->next;
        pri->next=tp;
        tp->data=element;
        this->size++;
        ret=0;
    }else{
		cout<<"fail malloc "<<endl;
    }
    return ret;
}
template<class T>
void LinkList<T>::print(){
    Node<T>*tp;
    if(this->size==0)
        cout<<"list is empty"<<endl;
    else{
        tp=head->next;
        while(tp!=NULL){
            cout<<tp->data<<" ";
            tp=tp->next;
        }
        cout<<endl;
    }
}

template <class T>
ostream & operator <<(ostream &os,LinkList<T> &l){
    Node<T>*tp;
    if(l.size==0)
        os<<"list is empty"<<endl;
    else{
        tp=l.head->next;
        while(tp!=NULL){
            os<<tp->data<<" ";
            tp=tp->next;
        }
        os<<endl;
    }
	return os;
}
template <class T>
int LinkList<T>::delete_element(T element){
	int ret;
	ret=-1;
	Node<T> *tp;
	Node<T>*pri;
	pri=head;
	tp=head->next;
	while(tp!=NULL){
			if(tp->data==element){
				pri->next=tp->next;
				free(tp);
				ret=0;
				this->size--;
				break;
			}
			tp=tp->next;
			pri=pri->next;
	}
	return ret;
}
template <class T>
int LinkList<T>::delete_head(){
	Node<T> *tp;

	if(this->size==0){
		cout<<"list is empty"<<endl;
		return -1;
	}
	tp=head->next;
	head->next=tp->next;
	free(tp);
	this->size--;
	return 0;
}
template <class T>
int LinkList<T>::delete_tail(){
	Node<T> *tp;
	Node<T> *pri;
	pri=head;
	tp=head->next;
	if(this->size==0){
		cout<<"list is empty"<<endl;
		return -1;
	}
	while(tp){
		if(tp->next==NULL){
			break;
		}
		tp=tp->next;
		pri=pri->next;
	}
	pri->next=tp->next;
	free(tp);
	this->size--;
	return 0;
}
template <class T>
int LinkList<T>::delete_pos(int pos){
	int ret;
	ret=-1;
	Node<T> *tp;
	Node<T>*pri;
	tp=head->next;
	pri=head;
	int i=1;
	while(tp!=NULL){
			if(pos==i){
				pri->next=tp->next;
				free(tp);
				this->size--;
				ret=0;
				break;
			}
			pri=pri->next;
			tp=tp->next;
			i++;
	}
	return ret;
}
template <class T>
int LinkList<T>::get_size(){
	return this->size;
}
template <class T>
class AA{
	public:
		T aa;
		AA<T>(){
			aa=1;
		}
};
template <class T>
class BB:public AA<T>{
	public:
		T bb;
		BB<T>():AA<T>(){
			bb=20;
		}
		virtual void fun(T* da);
};
template <class T>
void BB<T>::fun(T* da){
	cout<<"bb fun,aa is "<<this->aa<<" bb is "<<bb<<" "<<da<<" "<<*da<<" "<<" "<<endl;
	da=&bb;
}
void test_bas_list();

#endif
