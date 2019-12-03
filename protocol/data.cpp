/*
 * File Name: data.cpp
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 11:17:49 week:5
*/
#include<iostream>
#include "configurable.h"
#include "data.h"
using namespace std;
allyx TerYx;
YX_TAB YxTable[MAX_YX_NUM];
SORT_YX_TAB SortYxTable[MAX_YX_NUM];
YC_TAB YcTable[MAX_YC_NUM];
EventList event_list;
EventYcList event_yc_list;
dir_list g_dir_list;
void init_data(){
	default_init_yx_addr();
	default_init_yc_addr();
	init_yx();
	init_yc();
	event_list.init(100);
	event_yc_list.init(100);
	g_dir_list.init(20);
	load_file_list();
}

// vim:tw=72 
