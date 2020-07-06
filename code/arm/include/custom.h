/****************************************************************
 * $ID: custom.h       一, 09  8月 2010 13:45:49 +0800  grant $ *
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
 * Last modified: 二, 07 12月 2010 19:29:17 +0800    by grant #
 ****************************************************************/
#ifndef CUSTOM_H
#define CUSTOM_H


#define ROOT   				"/home/root/"
#define APP_ROOT   			"/usr/sbin/"

#define DEFAULT_PARA_ROOT 	"/etc/zxjy/para/"
#define PARA_ROOT  ROOT 	"para/"
#define DATA_ROOT  ROOT 	"data/"
#define EVENT_ROOT  ROOT 	"event/"
#define RTDATA_ROOT  ROOT 	"rtdata/"



#define PARA_CFG PARA_ROOT"para.xml"//参数配置文件
#define DC_CALIBRATE_FILE PARA_ROOT"dc.cfg"//直流标定参数配置文件
#define PARA_YC PARA_ROOT"ConfigYc.ini"//遥测系数配置文件
#define  CONFIG_XML_PATH PARA_ROOT  "gb101cfg.xml"//protocol 101 para config file root
#define  CONFIG_XML_PATH_4 PARA_ROOT  "gb104cfg.xml"//protocol 104 config file root

//默认参数
#define DEFAULT_PARA_CFG DEFAULT_PARA_ROOT"para.xml"//默认参数配置文件
#define DEFAULT_DC_CALIBRATE_FILE DEFAULT_PARA_ROOT"dc.cfg"//直流标定参数配置文件
#define DEFAULT_PARA_YC DEFAULT_PARA_ROOT"ConfigYc.ini"//遥测系数配置文件
#define DEFAULT_CONFIG_XML_PATH DEFAULT_PARA_ROOT  "gb101cfg.xml"//protocol 101 para config file root
#define DEFAULT_CONFIG_XML_PATH_4 DEFAULT_PARA_ROOT  "gb104cfg.xml"//protocol 104 config file root


#define VERSION  "SV01.001"//第一字节u_boot，第二字节u_image版本，第三四五字节应用程序版本



// update pack save file root

#define UPGRADE_FILE_NAME DATA_ROOT "update.tgz"
#define UPGRADE_CFG_PARA  DATA_ROOT  "update_para"




#endif

