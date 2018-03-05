
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
API_NAME = DMX


# **********************************************
# DRV Source Files
# **********************************************
# tsp2 is prior than rasp
BOX_CHIPS = u4 j2 k1 k2 kappa
TSP3_CHIPS = agate eiffel monaco miami clippers muji munich manhattan maserati maxim
TSP4_CHIPS = kaiser keltic keres kirin kano k6 k6lite curry k7u

ifeq ($(CHIP), $(filter $(CHIP),$(BOX_CHIPS)))
DRV_DIR = \
            tsp2                                                    \
            rasp                                                    \

else ifeq ($(CHIP),maria10)
DRV_DIR = \
            minitsp                                                \

else
DRV_DIR = \
            tsp                            \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP3_CHIPS)))
DRV_DIR = \
            tsp3                                                \

endif

ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
DRV_DIR = tsp4

ifeq ($(RASP_SUPPORT),y)
DRV_DIR += rasp
endif

ifeq ($(OTV_SUPPORT),y)
DRV_DIR += otv
endif

endif

FQ_CHIPS = kappa keltic keres kirin monaco kano maserati maxim k6 k6lite curry k7u
ifeq ($(CHIP),$(filter $(CHIP),$(FQ_CHIPS)))
DRV_DIR += fq
endif

MMFI_CHIPS = t12 j2 a2 a1 k1 kappa a5 a7 a3 k2 a5p amethyst eagle edison macaw12 emerald eden euler einstein nike madison nugget nikon einstein3 napoli monet messi
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI_CHIPS)))
DRV_DIR += mmfi
endif

MMFI2_CHIPS = agate eiffel kaiser keltic keres kirin monaco kano miami clippers muji munich manhattan maserati maxim k6 k6lite curry k7u
ifeq ($(CHIP), $(filter $(CHIP),$(MMFI2_CHIPS)))
DRV_DIR += mmfi2
endif

TSO_CHIPS = agate einstein nike kaiser keltic einstein3 keres kirin napoli
ifeq ($(CHIP), $(filter $(CHIP),$(TSO_CHIPS)))
DRV_DIR += tso
endif

TSO2_CHIPS = monaco miami munich muji manhattan maserati maxim kano k6 k6lite curry k7u
ifeq ($(CHIP), $(filter $(CHIP),$(TSO2_CHIPS)))
DRV_DIR += tso2
endif

TSIO_CHIPS = k6 k6lite
ifeq ($(CHIP), $(filter $(CHIP),$(TSIO_CHIPS)))
ifeq ($(TSIO_SUPPORT),y)
DRV_DIR += tsio
endif
endif

# **********************************************
# Rules
# **********************************************
include $(ROOTLIB)/api_rule.mk

