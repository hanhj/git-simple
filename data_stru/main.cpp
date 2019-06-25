#include "common.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "search.h"
#include "sort.h"
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

	string str;
	str="abc";
	cout<<str<<" "<<str[2]<<endl;
	int ret;
	ret=(str=="aaaa");
	cout<<ret<<endl;
    return 0;

}
