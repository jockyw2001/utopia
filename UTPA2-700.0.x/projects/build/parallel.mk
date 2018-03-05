
################################################################################
#
# Copyright (c) 2008-2009 MStar Semiconductor, Inc.
# All rights reserved.
#
# Unless otherwise stipulated in writing, any and all information contained
# herein regardless in any format shall remain the sole proprietary of
# MStar Semiconductor Inc. and be kept in strict confidence
# ("MStar Confidential Information") by the recipient.
# Any unauthorized act including without limitation unauthorized disclosure,
# copying, use, reproduction, sale, distribution, modification, disassembling,
# reverse engineering and compiling of the contents of MStar Confidential
# Information is unlawful and strictly prohibited. MStar hereby reserves the
# rights to any and all damages, losses, costs and expenses resulting therefrom.
#
#
# Makefile used for building application.
#
#
################################################################################

# **********************************************
# Environment
# **********************************************
PROJ        = $(PWD)
ROOT        = $(PROJ)/../..
ROOTLIB     = $(ROOT)/mxlib
CONFIG      = $(PROJ)/.config

-include $(CONFIG)
.NOTPARALLEL :

all:
	@$(MAKE) -s -f Makefile check_build_toolver
	@$(MAKE) -s -f Makefile setup
	@if [ -n "$(MXLIB-y)" ]; then                   \
        $(MAKE) -s -f Makefile $(MXLIB-y);          \
     fi
	@if [ -n "$(MXLIB-m)" ]; then                   \
        $(MAKE) -s -f Makefile $(MXLIB-m);          \
     fi
ifeq ($(CONFIG_MSOS),y)
	@$(MAKE) -s -f Makefile msos
endif
	@$(MAKE) -s -f Makefile syslib

include $(ROOTLIB)/mxlib.mk
include $(ROOTLIB)/ddi.mk

