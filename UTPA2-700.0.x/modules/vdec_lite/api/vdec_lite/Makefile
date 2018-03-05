
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

# Chips that default using VDEC 2.0
VDEC_EX_CHIPS = j2 a1 a7 a5 a3 amethyst agate eagle edison emerald kaiser eiffel nike madison miami nugget einstein einstein3 napoli monaco clippers muji keres kirin monet manhattan kano kastor curry
ifneq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
API_NAME = VDEC_V2
else
API_NAME = VDEC
endif

# **********************************************
# DRV Source Files
# **********************************************
DRV_DIR = \
            hvd_lite                                           \
            mjpeg_lite                                         \
            mvd_lite                                           \

ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
all: update_hdr
update_hdr:
	cp $(PUBINC)/apiVDEC.h ./
endif 

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk


# **********************************************
# Incremental Build 
# **********************************************
ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvIPAUTH -lapiJPEG
endif


# **********************************************
# Clock Resource Management 
# **********************************************
ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
ifeq ($(CONFIG_MSTAR_CLKM),y)
LD_LIBS_INC += -L$(DDI_PATH)  -ldrvCLKM
endif
endif