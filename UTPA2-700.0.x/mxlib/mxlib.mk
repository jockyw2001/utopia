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

MXLIB-y :=

###############################################################################
#
# Utopia Drviver Library Import
#
################################################################################
MXLIB-$(CONFIG_DRV_MIU) += $(ROOTLIB)/drv/miu
MXLIB-$(CONFIG_MSTAR_CLKM) += $(ROOTLIB)/drv/clkm
MXLIB-$(CONFIG_DRV_BDMA) += $(ROOTLIB)/drv/bdma
MXLIB-$(CONFIG_DRV_IR_TX) += $(ROOTLIB)/drv/ir_tx
MXLIB-$(CONFIG_DRV_ACE) += $(ROOTLIB)/drv/ace
MXLIB-$(CONFIG_DRV_AESDMA) += $(ROOTLIB)/drv/aesdma
MXLIB-$(CONFIG_DRV_AUDIO) += $(ROOTLIB)/drv/audio
MXLIB-$(CONFIG_DRV_AUDSP) += $(ROOTLIB)/drv/audsp
MXLIB-$(CONFIG_DRV_AVD) += $(ROOTLIB)/drv/avd
MXLIB-$(CONFIG_DRV_CA2) += $(ROOTLIB)/drv/ca2
MXLIB-$(CONFIG_DRV_CEC) += $(ROOTLIB)/drv/cec
MXLIB-$(CONFIG_DRV_CH34) += $(ROOTLIB)/drv/ch34
MXLIB-$(CONFIG_DRV_CIPHER) += $(ROOTLIB)/drv/cipher
MXLIB-$(CONFIG_DRV_CMDQ) += $(ROOTLIB)/drv/cmdq
MXLIB-$(CONFIG_DRV_CPU) += $(ROOTLIB)/drv/cpu
MXLIB-$(CONFIG_DRV_DAC) += $(ROOTLIB)/drv/dac
MXLIB-$(CONFIG_DRV_DDC2BI) += $(ROOTLIB)/drv/ddc2bi
MXLIB-$(CONFIG_DRV_DEMOD) += $(ROOTLIB)/drv/demod
MXLIB-$(CONFIG_DRV_DIP) += $(ROOTLIB)/drv/dip
MXLIB-$(CONFIG_DRV_DLC) += $(ROOTLIB)/drv/dlc
MXLIB-$(CONFIG_DRV_DMD) += $(ROOTLIB)/drv/dmd
MXLIB-$(CONFIG_DRV_DSCMB) += $(ROOTLIB)/drv/dscmb
MXLIB-$(CONFIG_DRV_DSCMB2) += $(ROOTLIB)/drv/dscmb2
MXLIB-$(CONFIG_DRV_MSB1233) += $(ROOTLIB)/drv/dvb_extdemod
MXLIB-$(CONFIG_DRV_NOR_FLASH) += $(ROOTLIB)/drv/flash/nor
MXLIB-$(CONFIG_DRV_SERIAL_FLASH) += $(ROOTLIB)/drv/flash/serial
MXLIB-$(CONFIG_DRV_GE) += $(ROOTLIB)/drv/ge
MXLIB-$(CONFIG_DRV_GOP) += $(ROOTLIB)/drv/gop
MXLIB-$(CONFIG_DRV_GOPSC) += $(ROOTLIB)/drv/gopsc
MXLIB-$(CONFIG_DRV_GPD) += $(ROOTLIB)/drv/gpd
MXLIB-$(CONFIG_DRV_GPIO) += $(ROOTLIB)/drv/gpio
MXLIB-$(CONFIG_DRV_HDCP) += $(ROOTLIB)/drv/hdcp
MXLIB-$(CONFIG_DRV_HDMIX) += $(ROOTLIB)/drv/hdmix
MXLIB-$(CONFIG_DRV_HDMIX2) += $(ROOTLIB)/drv/hdmix2
MXLIB-$(CONFIG_DRV_HVD) += $(ROOTLIB)/drv/hvd
MXLIB-$(CONFIG_DRV_HVD_EX) += $(ROOTLIB)/drv/hvd_ex
MXLIB-$(CONFIG_DRV_HVD_V3) += $(ROOTLIB)/drv/hvd_v3
MXLIB-$(CONFIG_DRV_HVD_LITE) += $(ROOTLIB)/drv/hvd_lite
MXLIB-$(CONFIG_DRV_HWI2C) += $(ROOTLIB)/drv/hwi2c
MXLIB-$(CONFIG_DRV_IPAUTH) += $(ROOTLIB)/drv/ipauth
MXLIB-$(CONFIG_DRV_IR) += $(ROOTLIB)/drv/ir
MXLIB-$(CONFIG_DRV_IRQ) += $(ROOTLIB)/drv/irq
MXLIB-$(CONFIG_DRV_JPD) += $(ROOTLIB)/drv/jpd
MXLIB-$(CONFIG_DRV_JPD_EX) += $(ROOTLIB)/drv/jpd_ex
MXLIB-$(CONFIG_DRV_MBX) += $(ROOTLIB)/drv/mbx
MXLIB-$(CONFIG_DRV_MFC) += $(ROOTLIB)/drv/mfc
MXLIB-$(CONFIG_DRV_MFE) += $(ROOTLIB)/drv/mfe
MXLIB-$(CONFIG_DRV_MFE_EX) += $(ROOTLIB)/drv/mfe_ex
MXLIB-$(CONFIG_DRV_MHL) += $(ROOTLIB)/drv/mhl
MXLIB-$(CONFIG_DRV_MINITSP) += $(ROOTLIB)/drv/minitsp
MXLIB-$(CONFIG_DRV_MMFI) += $(ROOTLIB)/drv/mmfi
MXLIB-$(CONFIG_DRV_MMFI2) += $(ROOTLIB)/drv/mmfi2
MXLIB-$(CONFIG_DRV_MMIO) += $(ROOTLIB)/drv/mmio
MXLIB-$(CONFIG_DRV_MPIF) += $(ROOTLIB)/drv/mpif
MXLIB-$(CONFIG_DRV_MVD) += $(ROOTLIB)/drv/mvd
MXLIB-$(CONFIG_DRV_MVD_EX) += $(ROOTLIB)/drv/mvd_ex
MXLIB-$(CONFIG_DRV_MVD_V3) += $(ROOTLIB)/drv/mvd_v3
MXLIB-$(CONFIG_DRV_MVD_LITE) += $(ROOTLIB)/drv/mvd_lite
MXLIB-$(CONFIG_DRV_MVOP) += $(ROOTLIB)/drv/mvop
MXLIB-$(CONFIG_DRV_MSPI) += $(ROOTLIB)/drv/mspi
MXLIB-$(CONFIG_DRV_NSK2) += $(ROOTLIB)/drv/nsk2
MXLIB-$(CONFIG_DRV_NJPD_EX) += $(ROOTLIB)/drv/njpd_ex
MXLIB-$(CONFIG_DRV_OTV) += $(ROOTLIB)/drv/otv
MXLIB-$(CONFIG_DRV_PCMCIA) += $(ROOTLIB)/drv/pcmcia
MXLIB-$(CONFIG_DRV_PM) += $(ROOTLIB)/drv/pm
MXLIB-$(CONFIG_DRV_PNL) += $(ROOTLIB)/drv/pnl
MXLIB-$(CONFIG_DRV_PVR_IFRAMELUT) += $(ROOTLIB)/drv/pvr_iframelut
MXLIB-$(CONFIG_DRV_PWM) += $(ROOTLIB)/drv/pwm
MXLIB-$(CONFIG_DRV_PWS) += $(ROOTLIB)/drv/pws
MXLIB-$(CONFIG_DRV_RASP) += $(ROOTLIB)/drv/rasp
MXLIB-$(CONFIG_DRV_RTC) += $(ROOTLIB)/drv/rtc
MXLIB-$(CONFIG_DRV_RVD) += $(ROOTLIB)/drv/rvd
MXLIB-$(CONFIG_DRV_SAR) += $(ROOTLIB)/drv/sar
MXLIB-$(CONFIG_DRV_SC) += $(ROOTLIB)/drv/sc
MXLIB-$(CONFIG_DRV_SEM) += $(ROOTLIB)/drv/sem
MXLIB-$(CONFIG_DRV_SMBX) += $(ROOTLIB)/drv/smbx
MXLIB-$(CONFIG_DRV_STBDC) += $(ROOTLIB)/drv/stbdc
MXLIB-$(CONFIG_DRV_SYS) += $(ROOTLIB)/drv/sys
MXLIB-$(CONFIG_DRV_TSO) += $(ROOTLIB)/drv/tso
MXLIB-$(CONFIG_DRV_TSP) += $(ROOTLIB)/drv/tsp
MXLIB-$(CONFIG_DRV_TSP2) += $(ROOTLIB)/drv/tsp2
MXLIB-$(CONFIG_DRV_TSP3) += $(ROOTLIB)/drv/tsp3
MXLIB-$(CONFIG_DRV_UART) += $(ROOTLIB)/drv/uart
MXLIB-$(CONFIG_DRV_URDMA) += $(ROOTLIB)/drv/urdma
MXLIB-$(CONFIG_DRV_USB_ECOS) += $(ROOTLIB)/drv/usb_ecos/newhost
MXLIB-$(CONFIG_DRV_USB_EC) += $(ROOTLIB)/drv/usb_ecos
MXLIB-$(CONFIG_DRV_USB_HID_P1) += $(ROOTLIB)/drv/usbhost/source/usb_hid_p1/
MXLIB-$(CONFIG_DRV_USB_HOST_P1) += $(ROOTLIB)/drv/usbhost/source/usb_host_p1/
MXLIB-$(CONFIG_DRV_USB_HID_P2) += $(ROOTLIB)/drv/usbhost/source2/usb_hid_p2/
MXLIB-$(CONFIG_DRV_USB_HOST_P2) += $(ROOTLIB)/drv/usbhost/source2/usb_host_p2/
MXLIB-$(CONFIG_DRV_USB_HID_P3) += $(ROOTLIB)/drv/usbhost/source3/usb_hid_p3/
MXLIB-$(CONFIG_DRV_USB_HOST_P3) += $(ROOTLIB)/drv/usbhost/source3/usb_host_p3/
MXLIB-$(CONFIG_DRV_VBI) += $(ROOTLIB)/drv/vbi
MXLIB-$(CONFIG_DRV_VE) += $(ROOTLIB)/drv/ve
MXLIB-$(CONFIG_DRV_VIF) += $(ROOTLIB)/drv/vif
MXLIB-$(CONFIG_DRV_WBLE) += $(ROOTLIB)/drv/wble
MXLIB-$(CONFIG_DRV_WDT) += $(ROOTLIB)/drv/wdt
MXLIB-$(CONFIG_DRV_XC) += $(ROOTLIB)/drv/xc
MXLIB-$(CONFIG_MSFS)+= $(ROOTLIB)/msfs/$(OS_TYPE)
MXLIB-$(CONFIG_DRV_MBX) += $(ROOTLIB)/drv/seal
MXLIB-$(CONFIG_CMDQ) += $(ROOTLIB)/drv/cmdq
MXLIB-$(CONFIG_DRV_PQ) += $(ROOTLIB)/drv/pq
MXLIB-$(CONFIG_DRV_LDM) += $(ROOTLIB)/drv/ldm
MXLIB-$(CONFIG_DRV_LDMA) += $(ROOTLIB)/drv/ldma

###############################################################################
#
# Utopia API Library Import
#
################################################################################
MXLIB-$(CONFIG_API_ACE) += $(ROOTLIB)/api/ace
MXLIB-$(CONFIG_API_ACP) += $(ROOTLIB)/api/acp
MXLIB-$(CONFIG_API_AUDIO) += $(ROOTLIB)/api/audio
MXLIB-$(CONFIG_API_CEC) += $(ROOTLIB)/api/cec
MXLIB-$(CONFIG_API_DAC) += $(ROOTLIB)/api/dac
MXLIB-$(CONFIG_API_DLC) += $(ROOTLIB)/api/dlc
MXLIB-$(CONFIG_API_DMX) += $(ROOTLIB)/api/dmx
MXLIB-$(CONFIG_API_DMX_TEE) += $(ROOTLIB)/api/dmx_tee
MXLIB-$(CONFIG_API_GFX) += $(ROOTLIB)/api/gfx
MXLIB-$(CONFIG_API_GOP) += $(ROOTLIB)/api/gop
MXLIB-$(CONFIG_API_GOPSC) += $(ROOTLIB)/api/gopsc
MXLIB-$(CONFIG_API_GOPSCD) += $(ROOTLIB)/api/gopscd
MXLIB-$(CONFIG_API_GPD) += $(ROOTLIB)/api/gpd
MXLIB-$(CONFIG_API_HDMITX) += $(ROOTLIB)/api/hdmitx
MXLIB-$(CONFIG_API_HDMITX2) += $(ROOTLIB)/api/hdmitx2
MXLIB-$(CONFIG_API_HSL) += $(ROOTLIB)/api/hsl
MXLIB-$(CONFIG_API_JPEG) += $(ROOTLIB)/api/jpeg
MXLIB-$(CONFIG_API_JPEG_EX) += $(ROOTLIB)/api/jpeg_ex
MXLIB-$(CONFIG_API_MFE) += $(ROOTLIB)/api/mfe
MXLIB-$(CONFIG_API_MFE_EX) += $(ROOTLIB)/api/mfe_ex
MXLIB-$(CONFIG_API_MHL) += $(ROOTLIB)/api/mhl
MXLIB-$(CONFIG_API_NJPEG_EX) += $(ROOTLIB)/api/njpeg_ex
MXLIB-$(CONFIG_API_PNL) += $(ROOTLIB)/api/pnl
MXLIB-$(CONFIG_API_SWI2C) += $(ROOTLIB)/api/swi2c
MXLIB-$(CONFIG_API_VDEC) += $(ROOTLIB)/api/vdec
MXLIB-$(CONFIG_API_VDEC_EX) += $(ROOTLIB)/api/vdec_ex
MXLIB-$(CONFIG_API_VDEC_V3) += $(ROOTLIB)/api/vdec_v3
MXLIB-$(CONFIG_API_VDEC_LITE) += $(ROOTLIB)/api/vdec_lite
MXLIB-$(CONFIG_API_XC) += $(ROOTLIB)/api/xc
MXLIB-$(CONFIG_API_MBX) += $(ROOTLIB)/api/mbx
#MXLIB-$(CONFIG_API_LOCALDIMM) += $(ROOTLIB)/api/localdimm

