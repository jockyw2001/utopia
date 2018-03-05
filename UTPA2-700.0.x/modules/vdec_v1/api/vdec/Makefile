
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
VDEC_EX_CHIPS = j2 a1 a7 a5 a3 amethyst agate eagle edison emerald kaiser nugget einstein einstein3 monaco muji monet manhattan
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
API_NAME = VDEC_V1
else
API_NAME = VDEC
endif


# **********************************************
# DRV Source Files
# **********************************************

VDEC_MVD_ONLY = 

ifeq ("$(CHIP)", "$(VDEC_MVD_ONLY)")
DRV_DIR = mvd
else
DRV_DIR = mvd hvd mjpeg
endif

ifneq ($(CHIP), $(filter $(CHIP),$(VDEC_EX_CHIPS)))
all: update_hdr
update_hdr:
	echo " Copy apiVDEC_EX.h to vdec folder "
	cp $(PUBINC)/apiVDEC_EX.h ./
else
all: remove_hdr
remove_hdr:
	if [ -f ../vdec/apiVDEC_EX.h ] ; then \
	rm $(PUBINC)/apiVDEC_EX.h ; \
	echo "*** Remove apiVDEC_EX.h from vdec folder ***"; \
	fi
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

