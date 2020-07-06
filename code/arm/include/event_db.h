/****************************************************************
 * $ID: db_interface.h 二, 10  8月 2010 09:29:26 +0800  grant $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  (grant)                                         *
 *                                                              *
 * Copyright (C)  2010  zx-jy                                   *
 *   www.zx-jy.com                                              *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it            *
 *   under the terms of the GNU General Public Licence (GPL).   *
 *                                                              *
 * Last modified: 五, 17 12月 2010 15:48:14 +0800    by grant #
 ****************************************************************/
#ifndef EVENT_DB_H
#define EVENT_DB_H
#include "datatype.h"
int remove_event_db ( void );
/**
 * 保存事件记录
 * @param void *p
 * @param int len
 * @param enum EVENT_TYPE type
 * @return: 无
 */
int set_event_record ( void *p, int len, enum EVENT_TYPE type );
/**
 * 修改事件记录
 * @param void *p
 * @param int index
 * @param int len
 * @param enum EVENT_TYPE type
 * @return: 无
 */
int fix_event_record ( void *p, int index, int len, enum EVENT_TYPE type );
/**
 * 读取事件记录
 * @param void *p
 * @param int index
 * @param int len
 * @param enum EVENT_TYPE type
 * @return: 无
 */
int get_event_record ( void *p, int index, int len, enum EVENT_TYPE type );
/**
 * 获取事件记录索引
 * @param enum EVENT_TYPE type
 * @return:
 *    成功：索引值
 *    失败：-1
 */
int get_event_index ( enum EVENT_TYPE type );
#define TABLE_SIZE(table) (sizeof(table)/sizeof(table[0]))
#endif
