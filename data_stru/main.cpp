#include "common.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "search.h"
#include "sort.h"
#include "recurse.h"
#include "tree.h"
#include "graph.h"
extern void case1();
extern void case2();
extern void case3();
int main()
{
	test_bas_list();
	test_stack();
	test_queue();
	case1();
	case2();
	case3();
	test_search();
	test_sort();
	test_recurse();
	test_tree();
	test_graph();
    return 0;
}
