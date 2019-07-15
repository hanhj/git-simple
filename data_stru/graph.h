/*
 * File Name: graph.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-07-12 21:55:18 week:5
*/
#ifndef __graph_h
#define __graph_h
#include "data_type.h"
#include "common.h"
#define MaxPoint 100
template <class T>
class MGraph{
	public:
		int edge[MaxPoint][MaxPoint];
		T vec[MaxPoint];
		int n,e;//points and edges

		MGraph(){
			n=0;
			e=0;
			int i;
			for(i=0;i<MaxPoint;i++)
				vistor[i]=0;
		}
		void visit(int);
		int vistor[MaxPoint];
		void dfs(){

		}
		void bfs(){

		}

};
class AdjEdge{
	public:
		int v;
		AdjEdge *next;
};
template <class T>
class AdjNode{
	public:
		T data;
		AdjEdge*first;
};
template <class T>
class AdjGraph{
	public:
		void visit(AdjNode *);
		AdjNode vec[MaxPoint];
		AdjGraph(){
			int i;
			for(i=0;i<MaxPoint;i++){
				vec[i].first=NULL;
			}
		}
		void dfs(){

		}
		void bfs(){

		}
}
void test_graph();
#endif //__graph_h
// vim:tw=72 
