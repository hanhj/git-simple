#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_33_02_31/packages;C:/ti/ipc_1_24_02_27/packages;C:/ti/xdais_7_21_01_07/packages;C:/ti/xdais_7_21_01_07/examples;C:/ti/uia_1_01_01_14/packages;C:/ti/syslink_2_21_01_05/packages;C:/ti/platform;C:/ti/ccsv5/ccs_base;E:/cc52workplace/DSPCPP/.config
override XDCROOT = C:/ti/xdctools_3_23_01_43
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_33_02_31/packages;C:/ti/ipc_1_24_02_27/packages;C:/ti/xdais_7_21_01_07/packages;C:/ti/xdais_7_21_01_07/examples;C:/ti/uia_1_01_01_14/packages;C:/ti/syslink_2_21_01_05/packages;C:/ti/platform;C:/ti/ccsv5/ccs_base;E:/cc52workplace/DSPCPP/.config;C:/ti/xdctools_3_23_01_43/packages;..
HOSTOS = Windows
endif
