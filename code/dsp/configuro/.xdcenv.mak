#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/syslink_2_21_01_05/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/bios_6_33_02_31/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/ipc_1_24_02_27/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/xdctools_3_23_01_43/packages
override XDCROOT = /home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/xdctools_3_23_01_43
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/syslink_2_21_01_05/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/bios_6_33_02_31/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/ipc_1_24_02_27/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/xdctools_3_23_01_43/packages;/home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00/ti/xdctools_3_23_01_43/packages;..
HOSTOS = Linux
endif
