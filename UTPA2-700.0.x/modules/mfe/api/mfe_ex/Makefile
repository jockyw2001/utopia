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
API_NAME = MFE


# **********************************************
# DRV Source Files
# **********************************************
DRV_DIR = \
            mfe_ex \
			mfe_ex/cModel

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

API_INCDIR += ./cModel
API_INCDIR += $(ROOTLIB)/hal/$(CHIP)/mfe_ex
#API_INCDIR += /home/cy.lai/PERFORCE/AEGIS/android/gingerbread/system/core/include
#API_INCDIR += /home/cy.lai/PERFORCE/AEGIS/android/gingerbread/system/core/include/cutils
