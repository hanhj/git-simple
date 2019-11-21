/*
 * File Name: Configurable.h
 * Author: hanhj
 * Mail: hanhj@zx-jy.com 
 * Created Time: 2019-10-18 11:08:08 week:5
*/
#ifndef __Configurable_h
#define __Configurable_h
#include "datatype.h"
typedef struct __config_scada_data{
	unsigned int SelectYxNum ; 
	unsigned int SelectDpYxNum ; 
	unsigned int SelectYcNum ; 
	unsigned int SelectAccNum ; 
	int pos_acc;
}_config_scada_data;
extern _config_scada_data config_scada_data;
void default_init_yx_addr(void);
void default_init_yc_addr(void);
int16 init_yx();
int16 init_yc();
int16 CoefTableInit(void );
#endif //__Configurable_h
// vim:tw=72 
