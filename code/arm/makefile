DEPOT = /home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00
libxml= $(DEPOT)/board-support/libxml2/include
TRG1=arm
TRG2=../manage/manage
all:$(TRG1) $(TRG2)
SRC1	=\
./app/arm.cpp\
./app/message.cpp\
./app/baseapp.cpp\
./app/timer.cpp\
./app/com.cpp\
./app/protocol.cpp\
./app/statis.cpp\
./app/vdsp.cpp\
./common/stack.cpp\
./common/data.cpp\
./common/debug.cpp\
./common/task.cpp\
./common/thread.cpp\
./common/delay.cpp\
./common/gps.cpp\
./common/hisdata.cpp\
./common/gb101.cpp\
./common/gpio.cpp\
./common/localtime.cpp\
./common/utils.cpp\
./common/sys_call.cpp\
./common/ad.cpp\
./common/syslink.cpp\
./common/uart.cpp\
./common/little_wireless_uart.cpp\
./common/socket.cpp\
./common/proxyclient.cpp\
./common/wireless_socket.cpp\
./common/gb104.cpp\
./common/process.cpp\
./common/wireless_uart.cpp\
./common/update_pro.cpp\
./common/sntp.cpp
SRC2	=\
../manage/manage.cpp\
./common/process.cpp\
./common/delay.cpp\
./common/debug.cpp\
./common/localtime.cpp\
./common/sys_call.cpp\
./common/gpio.cpp\
./common/utils.cpp

CC	=	arm-arago-linux-gnueabi-g++  -MD -MF $@.dep -march=armv5t
#CC	=	g++
INCDIR	=./include  -I../shared $(addprefix -I,$(subst +, ,$(PKGPATH))) -I $(libxml) 
LIB		=$(DEPOT)/linux-devkit/arm-arago-linux-gnueabi/usr/lib $(libs) -lrt -lpthread  -lxml2
#LIB	= ./ -lrt -lpthread -ldb -lxml2
RM    	=rm -f


OBJS1	= 	$(SRC1:%.cpp=%.o)
OBJS2	= 	$(SRC2:%.cpp=%.o)
DEPS	=		$(SRC1:%.cpp=%.o.dep)
DEPS	+=		$(SRC2:%.cpp=%.o.dep)


#CPFLAGS = -Wall -O2  -fno-strict-aliasing -D CONFIG_EMULATE 
CPFLAGS =  -Wall -O2 -D_REENTRANT -Dxdc_target_name__=GCArmv5T  -Dxdc_target_types__=gnu/targets/arm/std.h -std=gnu++0x
%.o : %.c
	$(CC) -c $(CPFLAGS) -I$(INCDIR) $< -o $@
%.o : %.cpp
	$(CC) -c $(CPFLAGS) -I$(INCDIR) $< -o $@
$(TRG1)	:	$(OBJS1)
	$(CC) $(OBJS1)  $(LDFLAGS) -L$(LIB)  -o  $@
$(TRG2)	:	$(OBJS2)
	$(CC) $(OBJS2)  $(LDFLAGS) -L$(LIB)  -o  $@

SYSLINK_INSTALL_DIR     = $(DEPOT)/ti/syslink_2_21_01_05
LINUXKERNEL             = $(DEPOT)/board-support/linux-3.1.10
CGT_ARM_INSTALL_DIR     = $(DEPOT)/linux-devkit
CGT_ARM_PREFIX          = $(CGT_ARM_INSTALL_DIR)/bin/arm-arago-linux-gnueabi-
IPC_INSTALL_DIR         = $(DEPOT)/ti/ipc_1_24_02_27
BIOS_INSTALL_DIR        = $(DEPOT)/ti/bios_6_33_02_31
XDC_INSTALL_DIR         = $(DEPOT)/ti/xdctools_3_23_01_43
CGT_C674_ELF_INSTALL_DIR=  $(DEPOT)/ti/TI_CGT_C6000_7.3.3

PKGPATH := $(SYSLINK_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(BIOS_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(IPC_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(XDC_INSTALL_DIR)/packages

libs = $(SYSLINK_INSTALL_DIR)/packages/ti/syslink/lib/syslink.a_debug
#-------------------
clean:
	$(RM) $(OBJS1)
	$(RM) $(OBJS2)
	$(RM) $(TRG1)		
	$(RM) $(TRG2)		
	$(RM) $(DEPS)		
	@echo "Errors: none"	
	 @echo "$@.dep"

#-------------------
# Create object files directory
#$(shell mkdir $(OBJDIR) 2>/dev/null)
#$(shell mkdir $(PROGDIR) 2>/dev/null)	
