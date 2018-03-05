
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

# **********************************************
# DRV Name
# **********************************************
DRV_NAME = MHL


# **********************************************
# HAL Source Files
# **********************************************
ifeq ($(EXTERNAL_MHL_CHIP),y)
	ifeq ($(MSTAR_MHL_CHIP),y)
		HAL_DIR = \
		mhl/external/mstar
	else
		HAL_DIR = \
		mhl/external/silicon
	endif
else
	HAL_DIR = \
           mhl/internal
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

# MHL chip configuration
ifeq ($(EXTERNAL_MHL_CHIP),y)
	CC_OPTS += -DEXTERNAL_MHL
	ifeq ($(MSTAR_MHL_CHIP),y)
		CC_OPTS += -DMSTAR_MHL
		DRV_INCDIR += $(DDIHAL)/$(CHIP)/mhl/external/mstar
	else
		DRV_INCDIR += $(DDIHAL)/$(CHIP)/mhl/external/silicon
	endif
else
	DRV_INCDIR += $(DDIHAL)/$(CHIP)/mhl/internal
endif

DRV_INCDIR += $(ROOTLIB)/api/mhl $(DDIHAL)/$(CHIP)/xc/include $(DDIDRV)/xc/include
DRV_INCDIR += $(DDIDRV)/gpio $(DDIDRV)/cpu