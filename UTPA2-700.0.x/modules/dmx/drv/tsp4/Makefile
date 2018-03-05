
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
DRV_NAME = TSP4


# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            tsp                                                     \

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk


# **********************************************
# Customization
# **********************************************

TSP4_RASP_CHIPS = kaiser keltic keres 
ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_RASP_CHIPS)))
DRV_INCDIR += $(DDIDRV)/rasp
endif

TSP4_OTV_CHIPS = kris kirin kano k6 k6lite
ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_OTV_CHIPS)))
DRV_INCDIR += $(DDIDRV)/otv
endif

ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
ifeq ($(CONFIG_MSTAR_CLKM),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvCLKM
endif
endif
