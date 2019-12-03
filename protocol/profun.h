/*
 * File Name: profun.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-12-03 16:59:12 week:2
*/
#ifndef __profun_h
#define __profun_h
/****************************
 *	extern function for protocol app_layer 
****************************/
extern SORT_YX_TAB * get_yx_data(int);
extern YC_TAB * get_yc_data(int );
extern YC_TAB * get_acc_yc_data(int );
extern int get_event_data(int type,event *&e,int change);
extern int get_clock(CP56Time2a &);
extern int do_yk(int id,int type,int cmd);
extern void do_reset();
extern int get_yc_cg_data(int port ,event_yc *&e);
extern int get_dir_data(_rd_dir*);
extern int get_file_data(_rd_file*);
extern int get_file_segment(_rd_file *file);
extern int save_file_segment(_rd_file *file);
extern int get_dz_unit(_para_list *);
extern int set_dz_unit(int);
extern int get_dz_data(para_node *);
extern int set_dz(int,para_node *);
extern int do_update();
extern int save_file_data(_rd_file *file);
#endif //__profun_h
// vim:tw=72 
