
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
DRV_NAME = HVD


# **********************************************
# HAL Source Files
# **********************************************
ifeq ($(CHIP),t2)
HAL_DIR = \
            svd		                                                \

else
HAL_DIR = \
            hvd_v3	                                                \
            vpu_v3	                                                \

endif


# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/drv_rule.mk


# **********************************************
# Config. Release drvflash for BDMA
# **********************************************
DRV_INCDIR += ../flash/serial


