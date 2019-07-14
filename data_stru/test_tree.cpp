/*
 * File Name: test_tree.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-07-09 09:15:21 week:2
*/
#include "tree.h"
void test_tree(){
	cout<<"\ntest tree ............"<<endl;
	BTree<char> btree;
	char pre_order[]={
		'a','b','d','c','e','f'
	};
	char in_order[]={
		'd','b','a','e','f','c'
	};
	btree.create_tree(&pre_order[0],&in_order[0],sizeof(pre_order));
	btree.pre_order();
	cout<<endl;
	btree.in_order();
	cout<<endl;
	btree.back_order();
	cout<<endl;
	btree.get_size();
	btree.level_order();
	cout<<endl;
	cout<<"height:"<<btree.get_height();
	cout<<endl;
	cout<<"leafs:"<<btree.get_leafs();
	cout<<endl;
/*	
	char bst_order[]={
		'd','b','c','a','e','f'
	};
	BstTree<int> bst_tree;
*/	
	int bst_order[]={7,9,10,2,3,1};//用这个会有问题
//	int bst_order[]={7,9,2,3,10,1};//用这个会有问题
//	int bst_order[]={1,2,3,4,5,6};
	BstTree<int> bst_tree;
	int i;
	for(i=0;i<6;i++){
		bst_tree.insert_tree(bst_order[i]);
	}
	bst_tree.pre_order();
	cout<<endl;
	bst_tree.in_order();
	cout<<endl;
	bst_tree.del_tree(7);
	bst_tree.pre_order();
	cout<<endl;
	bst_tree.in_order();
	cout<<endl;

	AvlTree<int> avl_tree;
	for(i=0;i<6;i++){
		avl_tree.insert_tree(bst_order[i]);
	}
	avl_tree.pre_order();
	cout<<endl;
	avl_tree.in_order();
	cout<<endl;

	cout<<"heap ...."<<endl;
	Heap<int> heap;
	heap.sort(bst_order,6);
	cout<<heap;
}

// vim:tw=72 
