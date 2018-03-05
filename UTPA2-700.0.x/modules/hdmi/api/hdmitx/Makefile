
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
API_NAME = HDMITX


# **********************************************
# DRV Source Files
# **********************************************
NOHDMI_CHIPS = keltic

ifeq ($(CHIP), $(filter $(CHIP),$(NOHDMI_CHIPS)))
DRV_DIR = \

else
DRV_DIR = \
            hdmitx                                                    \

endif


# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk
API_INCDIR += $(DDIDRV)/hdmitx/include

