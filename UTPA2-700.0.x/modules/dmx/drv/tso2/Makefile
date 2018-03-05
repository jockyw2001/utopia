
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
DRV_NAME = TSO2

TSP4_CHIPS = kano k6 k6lite k7u

# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            tso                                                     \

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
ifeq ($(CONFIG_MSTAR_CLKM),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvCLKM
endif
endif

# **********************************************
# Customization
# **********************************************
ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_CHIPS)))
CC_DEFS += -DTSP_VERSION
CC_DEFS += -DTSP_VER_4_0
endif
