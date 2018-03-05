
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
# Makefile used for building DDI
#
#
################################################################################

ROOT      = .
TRUNK     = .
PRJ      ?= t2_nos
TARGET   ?= s4le_nos
ADD      ?=
PROJECT_DIR = $(TRUNK)/../../../../project/$(PRJ)
LIB_DIR_  = $(PROJECT_DIR)/bsp/lib
INC_DIR_  = $(PROJECT_DIR)/bsp/include
TARGET_ROOT ?= Chakra2

OUT_TRUNK = $(TRUNK)/../../../../../../DAILEO/Yoga/$(TARGET_ROOT)
OUT_LIB_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/lib
OUT_INC_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/include

include install.mk

all: wble cec pnl ace dlc xc ddc2bi ve 
wble cec pnl ace dlc xc ddc2bi ve hdmitx dac mfc: init

ifneq ($(LINK_TYPE),dynamic)
	@make -C $(PROJECT_DIR) --no-print-directory MAKE_TYPE=lint $@ > /dev/null
	@make -C $(PROJECT_DIR) --no-print-directory $@ lint
else
	@make -C $(PROJECT_DIR) --no-print-directory $@
endif

install: init bsp
	@for i in $(INSTALL_INC_FILES); do	\
	    ./diff_cp.sh $(INC_DIR_)/$$i $(OUT_INC_DIR)/$$i;	\
	done;
	@for i in $(INSTALL_LIB_FILES); do	\
	    ./diff_cp.sh $(LIB_DIR_)/$$i $(OUT_LIB_DIR)/$$i;	\
	done;

init:
	@rm -rfd $(PROJECT_DIR)/bsp
	@make -C $(PROJECT_DIR) --no-print-directory clean

clean:
	@make -C $(PROJECT_DIR) --no-print-directory $@
	@rm -rfd $(PROJECT_DIR)/bsp
	@rm -f $(PROJECT_DIR)/lint_test.log

bsp:
	@make -C $(PROJECT_DIR) --no-print-directory $@ > /dev/null
