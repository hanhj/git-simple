/*
 * File Name: tree.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-07-09 09:16:29 week:2
*/
#ifndef __tree_h
#define __tree_h
#include "data_type.h"
#include "common.h"
template <class T>
class BTree{
	protected:
		BiNode<T> *root;
		int size;
		int height;
		void visit(BiNode<T> *node){
			cout<<node->data<<",";
		}
	public:
		BTree(){
			root=NULL;
			size=0;
			height=0;
		}
		~BTree(){
			destory(root);
		}
		BiNode<T> *get_root(){
			return root;
		}
		int get_size(){
			size=0;
			_get_size(root);
			return size;
		}
		void _get_size(BiNode<T> *node){
			if(node==NULL)
				return;
			size++;
			_get_size(node->left);
			_get_size(node->right);
		}
		int _get_height(BiNode<T> *node){
			int lh;
			int rh;
			int h;
			if(node==NULL)
				return 0;
			if(node->left==NULL && node->right==NULL)
				return 1;
			lh=_get_height(node->left)+1;
			rh=_get_height(node->right)+1;
			h=lh>rh?lh:rh;
			return h;
		}
		int get_height(){
			height=0;
			height=_get_height(root);
			return height;
		}
		int _get_leafs(BiNode<T> *node){
			if(node==NULL)
				return 0;
			if(node->left==NULL && node->right==NULL)
				return 1;
			return _get_leafs(node->left)+_get_leafs(node->right);
		}
		int  get_leafs(){
			return _get_leafs(root);	
		}
		void destory(BiNode<T> *node){
			if(node==NULL)
				return;
			destory(node->left);
			destory(node->right);
			delete node;
		}
		void pre_order(){
			_pre_order(root);
		}
		void _pre_order(BiNode<T> *node){
			if(node==NULL)
				return;
			visit(node);
			_pre_order(node->left);
			_pre_order(node->right);
		}
		void in_order(){
			_in_order(root);
		}
		void _in_order(BiNode<T> *node){
			if(node==NULL)
				return;
			_in_order(node->left);
			visit(node);
			_in_order(node->right);
		}
		void back_order(){
			_back_order(root);
		}
		void _back_order(BiNode<T> *node){
			if(node==NULL)
				return;
			_back_order(node->left);
			_back_order(node->right);
			visit(node);
		}
		void level_order(){
			_level_order(root);
		}
		void _level_order(BiNode<T> *node){
			BiNode<T> **queue;
			queue=new BiNode<T>*[size];
			int front,tail;
			front=-1;
			tail=0;
			if(node==NULL)
				return;
			queue[tail]=node;//in queue
			while(front!=tail){
				++front;//out queue
				visit(queue[front]);
				if(queue[front]->left!=NULL){
					tail++;//in queue
					queue[tail]=queue[front]->left;
				}
				if(queue[front]->right!=NULL){
					tail++;//in queue
					queue[tail]=queue[front]->right;
				}
			}
			delete []queue;//对于数组删除，需要前面加上[]
		}
		int create_tree(T *pre_ord,T*in_ord,int size){
			if(size<=0)
				return -1;
			return _create_tree(pre_ord,in_ord,0,size-1,0,size-1,root);
		}
		/*
		pre_ord :pre_order data array
		in_ord :in_order data array
		l,r pre_order data array start and end pos
		k,h in_order data array start and end pos
		root : return node
		*/
		int _create_tree(T*pre_ord,T*in_ord,int l,int r,int k,int
				h,BiNode<T> *& root){

			int m;
			BiNode<T>*node;
			node=new BiNode<T>;
			if(node==NULL)
				return -1;
			node->data=pre_ord[l];
			root=node;
			m=k;
			while(m<=h && pre_ord[l]!=in_ord[m])
				m++;
			if(m==k){//node havn't left tree
				node->left=NULL;
			}else{//build left tree
				_create_tree(pre_ord,in_ord,l+1,l+m-k,k,m-1,(root)->left);
			}
			if(m==h){//node have not right tree
				node->right=NULL;
			}else{//build right tree
				_create_tree(pre_ord,in_ord,l+m-k+1,r,m+1,h,(root)->right);
			}
			return 0;
		}
};
#define BST_DEL_FUN 0//0 替换法，1 删除法
template <class T>
class BstTree:public BTree<T>{
	public:
		/*如果没有找到节点，即为空树或是叶子节点，则返回父节点，否则返回找到的节点*/
		int _search_tree(BiNode<T> *btree,BiNode<T>*par,T key,BiNode<T>*&r){
			if(btree==NULL){
				r=par;
				return 0;
			}
			if(key<btree->data){
				return _search_tree(btree->left,btree,key,r);
			}else if(key>btree->data){
				return _search_tree(btree->right,btree,key,r);
			}else{
				r=btree;
				return 1;
			}
		}
		int search_tree(T key,BiNode<T>*&r){
			return _search_tree(this->root,this->root,key,r);
		}
		int insert_tree(T key){
			BiNode<T>*node,*p;
			if(search_tree(key,p)==1)
				return 0;
			node =new BiNode<T>;
			node->data=key;
			node->left=node->right=NULL;
			if(p==NULL)
				this->root=node;
			else if(key>p->data)
				p->right=node;
			else 
				p->left=node;
			return 1;
		}
		/*1.如果是空树，返回。递归查找待删除的节点。 
		  2.待删除的节点分成3类：叶子节点，只有左树或右树的节点，有左右子树的节点。
		  对于叶子节点直接删除即可，并修改双亲节点的指针。
		  对于只有左树或右树的节点，删除该节点，并修改双亲节点指针。
		  对于有左右子树的节点，可以用删除法或替换法来删除节点
		*/
		int del_tree(T key){
			return _del_tree(this->root,key);
		}
		int _del_tree(BiNode<T>*&btree,T key){
			if(btree==NULL)
				return 0;
			if(btree->data==key){
				del_node(btree);
				return 1;
			}else if(key>btree->data){
				return _del_tree(btree->right,key);
			}else
				return _del_tree(btree->left,key);
		}
		void del_node(BiNode<T> *&node){
			BiNode<T> *q;//point to delete node 
			if(node->left==NULL && node->right==NULL){//leaf 
				q=node;
				node=NULL;
				delete q;
			}else if(node->left==NULL){//only have right tree
				q=node;
				node=node->right;
				delete q;
			}else if(node->right==NULL){//only have left tree 
				q=node;
				node=node->left;
				delete q;
			}else{//both left and right have tree 
#if BST_DEL_FUN==0//替换法
				BiNode<T> *s,*p;//s is search point,p is s's parent
				s=node->left;
				if(s->right==NULL){//s only have left tree 
					q=s;
					node->data=s->data;
					node->left=s->left;
					delete q;
				}else{//s have right tree 
					while(s->right){
						p=s;
						s=s->right;
					}
					node->data=s->data;
					q=s;
					p->right=NULL;
					delete q;
				}
#elif BST_DEL_FUN==1
				BiNode<T> *s;
				s=node->left;
				while(s->right){
					s=s->right;
				}
				s->right=node->right;
				q=node;
				node=node->left;
				delete q;
#endif
			}
		}
};
//平衡二叉排序树，以上的二叉排序树所构成的树根据插入顺序不同，可能构成一条斜线的树，即只有右子树或左子树，这样的查找效率是低的，平衡二叉排序树是所有节点其左右子树的深度只差不大于1，此树称为平衡二叉排序树。
enum {LH=-1,EQ,RH};
template <class T>
class AvlTree:public BstTree<T>{
	private:
		int taller;
	public:
		AvlTree():BstTree<T>(){
			taller=FALSE;
		}
	/*
	左旋
	   A	      B
	    \        /          				
	    B  ===>  A
		/        \
	   C          C
	*/   
	void l_rotate(BiNode<T>*& p)
	{
		BiNode<T>*rc;
		rc=p->right;
		p->right=rc->left;
		rc->left=p;
		p=rc;
	}
	/*
	右旋
	   A	  B
	  /        \
	 B  ===>    A
	  \	       /
	   C      C
	*/   
	void r_rotate(BiNode<T>*&p)
	{
		BiNode<T>*lc;
		lc=p->left;
		p->left=lc->right;
		lc->right=p;
		p=lc;
	}
	/*左平衡
	 *     A      A
	 *    /      /
	 *   B      B
	 *  /        \
	 * C          C
	 * (1)       (2)
	 * C是新插入的节点，如果是情况1,只要对A进行右旋处理
	 * 如果是情况2,需要先对B左旋，然后对A进行右旋。
	 旋转后
	
	     B        C
		/ \   0  / \      0 
	   C   A    B   A
	   (1)       (2)
	 */
	void left_balance(BiNode<T>*&bt)
	{
		BiNode<T>*lc;
		lc=bt->left;
		if(lc->bf==LH){
			lc->bf=bt->bf=EQ;
			r_rotate(bt);
		}else if(lc->bf==RH){
			BiNode<T>*rd;
			rd=lc->right;
			switch(rd->bf){
				case LH:
					lc->bf=RH;
					bt->bf=EQ;
					break;
				case EQ:
					bt->bf=lc->bf=EQ;
					break;
				case RH:
					lc->bf=EQ;
					bt->bf=LH;
					break;
			}
			rd->bf=EQ;
			l_rotate(lc);
			r_rotate(bt);
		}
	}
	/*右平衡
	 * A        A
	 *  \        \
	 *   B        B
	 *    \      /  
	 *     C    C
	 * (1)     (2)
	 * C是新插入的节点，如果是情况1,只要对A进行左旋处理
	 * 如果是情况2,需要先对B右旋，然后对A进行左旋。
	 旋转后
	
	     B        C
		/ \   0  / \    0 
	   A   C    A   B
	   (1)       (2)
	 */
	void right_balance(BiNode<T>*&bt)
	{
		BiNode<T>*rc;
		rc=bt->right;
		if(rc->bf==RH){
			rc->bf=bt->bf=EQ;
			l_rotate(bt);
		}else if(rc->bf==LH){
			BiNode<T>*ld;
			ld=rc->left;
			switch(ld->bf){
				case LH:
					rc->bf=EQ;
					bt->bf=LH;
					break;
				case EQ:
					bt->bf=rc->bf=EQ;
					break;
				case RH:
					rc->bf=RH;
					bt->bf=EQ;
					break;
			}
			ld->bf=EQ;
			r_rotate(rc);
			l_rotate(bt);
		}
	}
	//插入一个节点到avl中
	//taller表示树层次是否增长
	int _insert_avl(BiNode<T>*& bt,T key,int &taller)
	{
		if(bt==NULL){
			bt=new BiNode<T>;
			bt->left=NULL;
			bt->right=NULL;
			bt->data=key;
			bt->bf=EQ;
			taller=TRUE;
			return TRUE;
		}
		if(bt->data==key){
			taller=FALSE;
			return FALSE;
		}
		if(bt->data>key){
			if(!_insert_avl(bt->left,key,taller)) 
				return FALSE;
			switch(bt->bf){
				case LH:
					left_balance(bt);
					taller=FALSE;
					break;
				case EQ:
					taller=TRUE;
					bt->bf=LH;
					break;
				case RH:
					taller=FALSE;
					bt->bf=EQ;
					break;
				default:
					break;
			}
		}else{
			if(!_insert_avl(bt->right,key,taller))
				return FALSE;
			switch(bt->bf){
				case LH:
					taller=FALSE;
					bt->bf=EQ;
					break;
				case EQ:
					taller=TRUE;
					bt->bf=RH;
					break;
				case RH:
					right_balance(bt);
					taller=FALSE;
			}
		}
		return TRUE;
	}
	int insert_tree(T key){
		return _insert_avl(this->root,key,taller);
	}
	int del_tree(T key){
		return 1;
	}

};
/*
	Heap is a complentenss binary tree,so normaly it use a array
	to store data
*/
template <class T>
class Heap{
	private:
		int dir;//1 is increse,0 is decrese
		int size;
		T *head;
	public:
		Heap(){
			dir=1;
			size=0;
			head=NULL;
		}
		void set_dir(int d){
			dir=d;
		}
		int get_size(){
			return size;
		}
		T* get_head(){
			return head;
		}
		/*
		   swap data between root and child if root is bigger of child
		step:
		   1.hold k
		   2.compare k and smaller of left and right 
		   3.swap k and child
		   4 adjust root to child pos and loop
		*/
		//k is root position,n is last data pos of tree
		void shift(T *data,int k,int n){
			T x;
			int finish=0;
			int i=k;
			int j=2*k+1;
			x=data[i];
			while(j<=n && !finish){
				if(j<n && data[j]>data[j+1])//avoid j+1 exceed n,so j<n
					j++;
				if(x<data[j]){
					finish=1;
				}else{
					data[i]=data[j];
					i=j;//record last swap pos
					j=2*i+1;
				}
			}
			data[i]=x;
		}
		void rev_shift(T *data,int k,int n){
			T x;
			int finish=0;
			int i=k;
			int j=2*k+1;
			x=data[i];
			while(j<=n && !finish){
				if(j<n && data[j]<data[j+1])//avoid j+1 exceed n,so j<n
					j++;
				if(x>data[j]){
					finish=1;
				}else{
					data[i]=data[j];
					i=j;//record last swap pos
					j=2*i+1;
				}
			}
			data[i]=x;
		}
		void init(T *data,int s){
			int i;
			size=s;
			head=data;
			int k;
			k=size-1;
			for(i=size/2-1;i>=0;i--){
				if(dir==0)
					shift(data,i,k);
				else
					rev_shift(data,i,k);
			}
		}
		void sort(T *data,int size){
			T tmp;
			int i;
			init(data,size);
			//pop first data ,swap last data to first 
			//and then shift it.
			//note :when pop data ,the size must -1.
			for(i=size-1;i>=0;--i){
				tmp=data[0];	//swap data 
				data[0]=data[i];
				data[i]=tmp;
				if(dir==0)
					shift(data,0,i-1);//adjust size
				else
					rev_shift(data,0,i-1);
			}
		}
};
template <class T>
ostream & operator <<(ostream &os,Heap<T> &heap){
	int i;
	T *da;
	int size;
	size=heap.get_size();
	da=heap.get_head();
	for(i=0;i<size;i++)
		os<<da[i]<<",";
	os<<endl;
	return os;
}
void test_tree();
#endif //__tree_h
