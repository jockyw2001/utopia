
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


PROJECT_DIR = $(TRUNK)/../../../../project/$(PRJ)
LIB_DIR_  = $(PROJECT_DIR)/bsp/lib
INC_DIR_  = $(PROJECT_DIR)/bsp/include

OUT_TRUNK = $(TRUNK)/../../../../../../DAILEO/Yoga/Chakra2
OUT_LIB_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/lib
OUT_INC_DIR ?= $(OUT_TRUNK)/core/drv_bsp/$(TARGET)/include

include install.mk


all: mvop
mvop: init


ifneq ($(LINK_TYPE),dynamic)
	@make -C $(PROJECT_DIR) --no-print-directory MAKE_TYPE=lint $@ > /dev/null
	@make -C $(PROJECT_DIR) --no-print-directory $@ lint
else
	@make -C $(PROJECT_DIR) --no-print-directory $@
endif

install: init bsp
	@for i in $(INSTALL_INC_FILES); do	\
	    if [ $(INC_DIR_)/$$i -nt $(OUT_INC_DIR)/$$i ]; \
		then cp -f $(INC_DIR_)/$$i $(OUT_INC_DIR); \
	    fi 						\
	done;
	@for i in $(INSTALL_LIB_FILES); do	\
	    if [ $(LIB_DIR_)/$$i -nt $(OUT_LIB_DIR)/$$i ]; \
		then cp -f $(LIB_DIR_)/$$i $(OUT_LIB_DIR); \
	    fi 						\
	done;

init:
	@rm -rfd $(PROJECT_DIR)/bsp

clean:
	@make -C $(PROJECT_DIR) --no-print-directory $@
	@rm -rfd $(PROJECT_DIR)/bsp
	@rm -f $(PROJECT_DIR)/lint_test.log

bsp:
	@make -C $(PROJECT_DIR) --no-print-directory $@
