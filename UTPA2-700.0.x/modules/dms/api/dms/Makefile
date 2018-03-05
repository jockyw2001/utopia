
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
API_NAME = DMS


# **********************************************
# DRV Source Files
# **********************************************
DIPGOP_CHIPS = kano k6 curry k6lite k7u M7221
VSYNCBRIDGE_CHIPS = M5621 maxim M7621
ifeq ($(CHIP),$(filter $(CHIP),$(DIPGOP_CHIPS)))
DRV_DIR = \
            dms_dipgop         \

endif
ifeq ($(CHIP),$(filter $(CHIP),$(VSYNCBRIDGE_CHIPS)))
DRV_DIR = \
            dms_vsyncbridge         \

endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk
API_INCDIR += $(DDIHAL)/$(CHIP)/dms/include
