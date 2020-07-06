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
#  ======== products.mak ========
#

DEPOT = /home/hanhj/opt/ti-sdk-omapl138-lcdk-01.00.00


SYSLINK_INSTALL_DIR     = $(DEPOT)/ti/syslink_2_21_01_05
LINUXKERNEL             = $(DEPOT)/board-support/linux-3.1.10
CGT_ARM_INSTALL_DIR     = $(DEPOT)/linux-devkit
CGT_ARM_PREFIX          = $(CGT_ARM_INSTALL_DIR)/bin/arm-arago-linux-gnueabi-
IPC_INSTALL_DIR         = $(DEPOT)/ti/ipc_1_24_02_27
BIOS_INSTALL_DIR        = $(DEPOT)/ti/bios_6_33_02_31
XDC_INSTALL_DIR         = $(DEPOT)/ti/xdctools_3_23_01_43
CGT_C674_ELF_INSTALL_DIR=  $(DEPOT)/ti/TI_CGT_C6000_7.3.3

# Use this goal to print your product variables.
.show:
	@echo "BIOS_INSTALL_DIR         = $(BIOS_INSTALL_DIR)"
	@echo "CGT_ARM_PREFIX           = $(CGT_ARM_PREFIX)"
	@echo "IPC_INSTALL_DIR          = $(IPC_INSTALL_DIR)"
	@echo "SYSLINK_INSTALL_DIR      = $(SYSLINK_INSTALL_DIR)"
	@echo "CGT_C674_ELF_INSTALL_DIR = $(CGT_C674_ELF_INSTALL_DIR)"
	@echo "XDC_INSTALL_DIR          = $(XDC_INSTALL_DIR)"

# look for other products.mak file to override local settings
ifneq (,$(wildcard $(EXBASE)/../products.mak))
include $(EXBASE)/../products.mak
else
ifneq (,$(wildcard $(EXBASE)/../../products.mak))
include $(EXBASE)/../../products.mak/
# fix SYSLINK_INSTALL_DIR because product uses $(CURDIR)
SYSLINK_INSTALL_DIR = $(word 1,$(subst /examples, examples,$(CURDIR)))
endif
endif
