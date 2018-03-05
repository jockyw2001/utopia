
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
DRV_NAME = DMS_VSYNCBRIDGE


# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            dms

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

# Customize for utopia. Please Do not change this area.
include ./option.mk
DRV_INCDIR += ./include
DRV_INCDIR += $(DDIDRV)/../api/dms
DRV_INCDIR += $(DDIDRV)/pq
DRV_INCDIR += $(DDIHAL)/$(CHIP)/dms/include
DRV_INCDIR += $(DDIHAL)/$(CHIP)/hvd_v3
DRV_INCDIR += $(DDIHAL)/$(CHIP)/vpu_v3