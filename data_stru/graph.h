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
		T data[MaxPoint];
		int n,e;//point and edges
};
void test_graph();
#endif //__graph_h
// vim:tw=72 
