################################################################################
#
# Copyright (c) 2008-2013 MStar Semiconductor, Inc.
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

ifeq ($(CHIP), nike)

MXLIB =
MXLIB += $(ROOTLIB)/api/ace
MXLIB += $(ROOTLIB)/api/audio
MXLIB += $(ROOTLIB)/api/cec
MXLIB += $(ROOTLIB)/api/dlc
MXLIB += $(ROOTLIB)/api/dmx
MXLIB += $(ROOTLIB)/api/gfx
MXLIB += $(ROOTLIB)/api/gop
MXLIB += $(ROOTLIB)/api/gpd
MXLIB += $(ROOTLIB)/api/mhl
MXLIB += $(ROOTLIB)/api/njpeg_ex
MXLIB += $(ROOTLIB)/api/pnl
MXLIB += $(ROOTLIB)/api/swi2c
MXLIB += $(ROOTLIB)/api/vdec_ex
MXLIB += $(ROOTLIB)/api/xc

MXLIB += $(ROOTLIB)/drv/aesdma
MXLIB += $(ROOTLIB)/drv/audio
MXLIB += $(ROOTLIB)/drv/audsp
MXLIB += $(ROOTLIB)/drv/avd
MXLIB += $(ROOTLIB)/drv/cpu
MXLIB += $(ROOTLIB)/drv/ddc2bi
MXLIB += $(ROOTLIB)/drv/demod
MXLIB += $(ROOTLIB)/drv/dscmb
MXLIB += $(ROOTLIB)/drv/flash/serial
MXLIB += $(ROOTLIB)/drv/gpio
MXLIB += $(ROOTLIB)/drv/hwi2c
MXLIB += $(ROOTLIB)/drv/ipauth
MXLIB += $(ROOTLIB)/drv/ir
MXLIB += $(ROOTLIB)/drv/mspi
MXLIB += $(ROOTLIB)/drv/mvop
MXLIB += $(ROOTLIB)/drv/pcmcia
MXLIB += $(ROOTLIB)/drv/pm
MXLIB += $(ROOTLIB)/drv/pwm
MXLIB += $(ROOTLIB)/drv/pws
MXLIB += $(ROOTLIB)/drv/rtc
MXLIB += $(ROOTLIB)/drv/sar
MXLIB += $(ROOTLIB)/drv/tsp
MXLIB += $(ROOTLIB)/drv/urdma
MXLIB += $(ROOTLIB)/drv/vbi
MXLIB += $(ROOTLIB)/drv/ve
MXLIB += $(ROOTLIB)/drv/vif
MXLIB += $(ROOTLIB)/drv/wdt

ifeq ($(MCU_TYPE), arm_ca7)
MXLIB += $(ROOTLIB)/drv/dip
endif

ifeq ($(OS_TYPE), linux) 

ifneq ($(PLATFORM), android)
MXLIB += $(ROOTLIB)/api/hsl
MXLIB += $(ROOTLIB)/msfs/$(OS_TYPE)
endif

MXLIB += $(ROOTLIB)/drv/sc
MXLIB += $(ROOTLIB)/api/mfe_ex
MXLIB += $(ROOTLIB)/drv/mfe_ex

else
MXLIB += $(ROOTLIB)/drv/usbhost/source/usb_hid_p1
MXLIB += $(ROOTLIB)/drv/usbhost/source2/usb_hid_p2
MXLIB += $(ROOTLIB)/drv/usbhost/source3/usb_hid_p3
MXLIB += $(ROOTLIB)/drv/usbhost/source/usb_host_p1
MXLIB += $(ROOTLIB)/drv/usbhost/source2/usb_host_p2
MXLIB += $(ROOTLIB)/drv/usbhost/source3/usb_host_p3

MXLIB += $(ROOTLIB)/drv/miu
MXLIB += $(ROOTLIB)/drv/irq
MXLIB += $(ROOTLIB)/drv/sys
MXLIB += $(ROOTLIB)/drv/uart
MXLIB += $(ROOTLIB)/drv/bdma
MXLIB += $(ROOTLIB)/drv/sem
MXLIB += $(ROOTLIB)/drv/mmio
MXLIB += $(ROOTLIB)/drv/mbx

endif
endif
