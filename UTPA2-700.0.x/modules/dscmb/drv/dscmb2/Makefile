
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

BOX_CHIPS = u4 j2 k1 k2 keltic
TSP3_CHIPS = agate
TSP4_CHIPS = kaiser keltic keres kirin
# **********************************************
# DRV Name
# **********************************************
DRV_NAME = DSCMB2


# **********************************************
# HAL Source Files
# **********************************************
HAL_DIR = \
            dscmb                                                   \


# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk

# TODO : not good for driver alling driver, plan to obsolete
ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp2

else ifeq ($(CHIP), $(filter $(CHIP),$(TSP3_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp3

else ifeq ($(CHIP), $(filter $(CHIP),$(TSP4_CHIPS)))
DRV_INCDIR += $(DDIDRV)/tsp4

else
DRV_INCDIR += $(DDIDRV)/tsp
endif
