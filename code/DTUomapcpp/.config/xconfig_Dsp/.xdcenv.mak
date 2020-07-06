#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = I:/ti/bios_6_34_02_18/packages;I:/ti/ipc_1_25_00_04/packages;I:/ti/xdais_7_21_01_07/packages;I:/ti/xdais_7_21_01_07/examples;I:/ti/ccsv5/syslink_2_21_01_05/packages;I:/ti/ccsv5/ccs_base;F:/share_doc/omap/dtu/code/DTUomapcpp/platform;F:/share_doc/omap/dtu/code/DTUomapcpp/.config
override XDCROOT = I:/ti/xdctools_3_24_05_48
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = I:/ti/bios_6_34_02_18/packages;I:/ti/ipc_1_25_00_04/packages;I:/ti/xdais_7_21_01_07/packages;I:/ti/xdais_7_21_01_07/examples;I:/ti/ccsv5/syslink_2_21_01_05/packages;I:/ti/ccsv5/ccs_base;F:/share_doc/omap/dtu/code/DTUomapcpp/platform;F:/share_doc/omap/dtu/code/DTUomapcpp/.config;I:/ti/xdctools_3_24_05_48/packages;..
HOSTOS = Windows
endif
