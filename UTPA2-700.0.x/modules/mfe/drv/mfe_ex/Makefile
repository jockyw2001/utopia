
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
DRV_NAME = MFE


# **********************************************
# HAL Source Files
# **********************************************
DRV_SUB += ./cModel
HAL_DIR = \
            mfe_ex

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

DRV_INCDIR += ./cModel
DRV_INCDIR += $(DDIHAL)/$(CHIP)/mfe_ex
#DRV_INCDIR += $(ROOTLIB)/hal/$(CHIP)/mfe


# **********************************************
# CLK Management
# **********************************************

ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
ifeq ($(CONFIG_MSTAR_CLKM),y)
LD_LIBS_INC += -L$(DDI_PATH) -ldrvCLKM
endif
endif

