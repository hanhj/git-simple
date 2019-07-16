/*
 * File Name: test_graph.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-07-15 12:47:21 week:1
*/
#include "graph.h"
int elements[]={
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11
};
int edge_table[11][11]={
	{0,1,1,1,INFI,INFI,INFI,INFI,INFI,INFI,INFI},
	{INFI,0,INFI,INFI,1,INFI,INFI,INFI,INFI,INFI,INFI},
	{INFI,INFI,0,INFI,INFI,1,INFI,INFI,INFI,INFI,INFI},
	{INFI,INFI,INFI,0,INFI,INFI,1,INFI,INFI,INFI,INFI},
	{INFI,INFI,INFI,INFI,0,INFI,INFI,1,INFI,INFI,INFI},
	{INFI,INFI,INFI,INFI,INFI,0,INFI,INFI,INFI,INFI,INFI},
	{INFI,INFI,INFI,INFI,INFI,INFI,0,INFI,INFI,INFI,INFI},
	{INFI,INFI,INFI,INFI,INFI,INFI,INFI,0,INFI,INFI,INFI},
	{INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,0,1,1},
	{INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,0,INFI},
	{INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,INFI,0},
};
typedef struct _edge{
	int v;
	int w;
	int weight;
}edge;
edge edges[]={
	{0,1,1},
	{0,2,1},
	{0,3,1},
	{1,4,1},
	{2,5,1},
	{3,6,1},
	{4,7,9},
	{8,9,9},
	{8,10,9},
};
void test_graph(){
	int i;
	int size;
	cout<<"test graph...."<<endl;
	size=sizeof(elements)/sizeof(int);
	MGraph<int> mg(size);
	for(i=0;i<size;i++)
		mg.insert_element(elements[i]);
	mg.create_edge((int **)&edge_table[0],size);
//	for(i=0;i<(int)sizeof(edges)/(int)sizeof(edge);i++){
//		mg.insert_edge(edges[i].v,edges[i].w,edges[i].weight);
//	}
	cout<<mg;
	cout<<"dfs:";
	mg.dfs();
	cout<<endl;
	mg.clear_visitor();
	cout<<"bfs:";
	mg.bfs();
	cout<<endl;
	AdjGraph<int> adj;
	for(i=0;i<size;i++)
		adj.insert_element(elements[i]);
	for(i=0;i<size;i++){
		adj.insert_edge(edges[i].v,edges[i].w,edges[i].weight);
	}
	cout<<adj;
	cout<<"dfs:";
	adj.dfs();
	cout<<endl;
	cout<<"bfs:";
	adj.clear_visitor();
	adj.bfs();
	cout<<endl;
}

// vim:tw=72 
