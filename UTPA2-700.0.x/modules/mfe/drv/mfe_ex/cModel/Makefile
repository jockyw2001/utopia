
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
HAL_DIR = \
            mfe_ex
#			mfe/hif_lib
#			$(ROOTLIB)/hal/$(CHIP)/mfe/globalinc  \
#            $(ROOTLIB)/hal/$(CHIP)/mfe/Aeon  \
#            $(ROOTLIB)/hal/$(CHIP)/mfe/hif_lib  \

#HAL_DIR += $(DDIHAL)/$(CHIP)/mfe/globalinc
#HAL_DIR += $(ROOTLIB)/hal/$(CHIP)/mfe/Aeon
#HAL_DIR += $(DDIHAL)/$(CHIP)/mfe/hif_lib

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

DRV_INCDIR += ./cModel
#DRV_INCDIR += $(DDIHAL)/$(CHIP)/mfe/globalinc
DRV_INCDIR += $(ROOTLIB)/hal/$(CHIP)/mfe_ex

