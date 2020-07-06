#
#  Copyright (c) 2012, Texas Instruments Incorporated
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  *  Neither the name of Texas Instruments Incorporated nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
#  ======== makefile ========
#

EXBASE = .
include $(EXBASE)/products.mak

srcs=\
./app/dsp.cpp\
./app/message.cpp\
./app/clock.cpp\
./app/scada.cpp\
./app/fa.cpp\
./app/baseapp.cpp\
./app/varm.cpp\
./app/interruptapp.cpp\
./app/fft.cpp\
./app/adsamp.cpp\
./app/clock.cpp\
./app/localprotect.cpp\
./app/yx.cpp\
./app/yk.cpp\
./common/stack.cpp\
./common/data.cpp\
./common/debug.cpp\
./common/task.cpp\
./common/thread.cpp\
./common/delay.cpp\
./common/syslink.cpp\
./driver/gpio.cpp\
./driver/fpag.cpp\
./driver/omapl138.cpp
objs = $(patsubst %.cpp,%.oe674,$(srcs))

srcs2=\
./mathlib/powsp.asm\
./mathlib/sqrtsp.asm\
./mathlib/DSPF_sp_fftSPxSP.asm\
./mathlib/expsp.asm\
./mathlib/logsp.asm\
./mathlib/atan2sp.asm\
./mathlib/cossp.asm\
./mathlib/sinsp.asm

objs2 = $(patsubst %.asm,%.oe674,$(srcs2))
libs = configuro/linker.cmd

all: configuro/linker.cmd
	$(MAKE) PROFILE=debug dsp.x
	$(MAKE) PROFILE=release dsp.x

dsp.x: dsp.xe674
dsp.xe674: $(objs) $(objs2) $(libs)
	@$(ECHO) "#"
	@$(ECHO) "# Making $@ ..."
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.oe674: %.h
%.oe674: %.cpp
	@$(ECHO) "#"
	@$(ECHO) "# Making $@ ..."
	$(CC) $(CPPFLAGS)  --output_file=$@  --gcc -fp=$<

%.oe674: %.asm
	@$(ECHO) "#"
	@$(ECHO) "# Making $@ ..."
	$(AR) -mv6740 --abi=eabi -g $< --output_file=$@   

configuro/linker.cmd: Dsp.cfg ../shared/config.bld
	@$(ECHO) "#"
	@$(ECHO) "# Making $@ ..."
	$(XDC_INSTALL_DIR)/xs --xdcpath="$(subst +,;,$(PKGPATH))" \
            xdc.tools.configuro -o configuro \
            -t ti.targets.elf.C674 -c $(CGT_C674_ELF_INSTALL_DIR) \
            -p ti.platforms.evmOMAPL138:dsp -b ../shared/config.bld \
            -r release Dsp.cfg

##install:
#	@$(ECHO) "#"
#	@$(ECHO) "# Making $@ ..."
#	@$(MKDIR) $(EXEC_DIR)/debug
#	$(CP)     dsp.xe674 $(EXEC_DIR)/debug
#	@$(MKDIR) $(EXEC_DIR)/release
#	$(CP)     dsp.xe674 $(EXEC_DIR)/release
install:
#	@$(ECHO) "#"
#	@$(ECHO) "# Making $@ ..."
#	@$(MKDIR) $(EXEC_DIR)/debug
	cp dsp.xe674 ../install/
help:
	@$(ECHO) "make                   # build executable"
	@$(ECHO) "make clean             # clean everything"

clean::
	$(RMDIR) configuro bin 
	$(RM) $(objs) $(dsp.x)
	$(RM) dsp.xe674 dsp.xe674.map
	
PKGPATH := $(SYSLINK_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(BIOS_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(IPC_INSTALL_DIR)/packages
PKGPATH := $(PKGPATH)+$(XDC_INSTALL_DIR)/packages

#  ======== install validation ========
#ifeq (install,$(MAKECMDGOALS))
#ifeq (,$(EXEC_DIR))
#$(error must specify EXEC_DIR)
#endif
#endif

#  ======== toolchain macros ========
CGTOOLS = $(CGT_C674_ELF_INSTALL_DIR)

CC = $(CGTOOLS)/bin/cl6x   
#AR = $(CGTOOLS)/bin/ar6x rq
AR = $(CGTOOLS)/bin/cl6x 
LD = $(CGTOOLS)/bin/lnk6x --abi=eabi
ST = $(CGTOOLS)/bin/strip6x

CPPFLAGS = $(CCPROFILE_$(PROFILE))  -I. -I./include -I../shared $(COMPILER_OPTS)
CFLAGS = -qq $(CCPROFILE_$(PROFILE))  -I. -I./include -I../shared $(COMPILER_OPTS)

LDFLAGS = -w -q -c -m $(@D)/$(@F).map
LDLIBS = -l $(CGTOOLS)/lib/rts6740_elf.lib

#CCPROFILE_debug = -D_DEBUG_=1 --symdebug:dwarf
CCPROFILE_debug = -g
CCPROFILE_release = -O2
COMPILER_OPTS = $(shell cat configuro/compiler.opt)

#  ======== standard macros ========
ifneq (,$(wildcard $(XDC_INSTALL_DIR)/bin/echo.exe))
    # use these on Windows
    CP      = $(XDC_INSTALL_DIR)/bin/cp
    ECHO    = $(XDC_INSTALL_DIR)/bin/echo
    MKDIR   = $(XDC_INSTALL_DIR)/bin/mkdir -p
    RM      = $(XDC_INSTALL_DIR)/bin/rm -f
    RMDIR   = $(XDC_INSTALL_DIR)/bin/rm -rf
else
    # use these on Linux
    CP      = cp
    ECHO    = echo
    MKDIR   = mkdir -p
    RM      = rm -f
    RMDIR   = rm -rf
endif

#  ======== create output directories ========
ifneq (clean,$(MAKECMDGOALS))
ifneq (,$(PROFILE))
ifeq (,$(wildcard bin/$(PROFILE)/obj))
    $(shell $(MKDIR) -p bin/$(PROFILE)/obj)
endif
endif
endif
