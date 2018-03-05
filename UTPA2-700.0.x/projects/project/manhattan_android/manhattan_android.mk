#<MStar Software>
#******************************************************************************
# MStar Software
# Copyright (c) 2010 - 2014 MStar Semiconductor, Inc. All rights reserved.
# All software, firmware and related documentation herein ("MStar Software") are
# intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
# law, including, but not limited to, copyright law and international treaties.
# Any use, modification, reproduction, retransmission, or republication of all
# or part of MStar Software is expressly prohibited, unless prior written
# permission has been granted by MStar.
#
# By accessing, browsing and/or using MStar Software, you acknowledge that you
# have read, understood, and agree, to be bound by below terms ("Terms") and to
# comply with all applicable laws and regulations:
#
# 1. MStar shall retain any and all right, ownership and interest to MStar
#    Software and any modification/derivatives thereof.
#    No right, ownership, or interest to MStar Software and any
#    modification/derivatives thereof is transferred to you under Terms.
#
# 2. You understand that MStar Software might include, incorporate or be
#    supplied together with third party's software and the use of MStar
#    Software may require additional licenses from third parties.
#    Therefore, you hereby agree it is your sole responsibility to separately
#    obtain any and all third party right and license necessary for your use of
#    such third party's software.
#
# 3. MStar Software and any modification/derivatives thereof shall be deemed as
#    MStar's confidential information and you agree to keep MStar's
#    confidential information in strictest confidence and not disclose to any
#    third party.
#
# 4. MStar Software is provided on an "AS IS" basis without warranties of any
#    kind. Any warranties are hereby expressly disclaimed by MStar, including
#    without limitation, any warranties of merchantability, non-infringement of
#    intellectual property rights, fitness for a particular purpose, error free
#    and in conformity with any international standard.  You agree to waive any
#    claim against MStar for any loss, damage, cost or expense that you may
#    incur related to your use of MStar Software.
#    In no event shall MStar be liable for any direct, indirect, incidental or
#    consequential damages, including without limitation, lost of profit or
#    revenues, lost or damage of data, and unauthorized system use.
#    You agree that this Section 4 shall still apply without being affected
#    even if MStar Software has been modified by MStar in accordance with your
#    request or instruction for your use, except otherwise agreed by both
#    parties in writing.
#
# 5. If requested, MStar may from time to time provide technical supports or
#    services in relation with MStar Software to you for your use of
#    MStar Software in conjunction with your or your customer's product
#    ("Services").
#    You understand and agree that, except otherwise agreed by both parties in
#    writing, Services are provided on an "AS IS" basis and the warranty
#    disclaimer set forth in Section 4 above shall apply.
#
# 6. Nothing contained herein shall be construed as by implication, estoppels
#    or otherwise:
#    (a) conferring any license or right to use MStar name, trademark, service
#        mark, symbol or any other identification;
#    (b) obligating MStar or any of its affiliates to furnish any person,
#        including without limitation, you and your customers, any assistance
#        of any kind whatsoever, or any information; or
#    (c) conferring any license or right under any intellectual property right.
#
# 7. These terms shall be governed by and construed in accordance with the laws
#    of Taiwan, R.O.C., excluding its conflict of law rules.
#    Any and all dispute arising out hereof or related hereto shall be finally
#    settled by arbitration referred to the Chinese Arbitration Association,
#    Taipei in accordance with the ROC Arbitration Law and the Arbitration
#    Rules of the Association by three (3) arbitrators appointed in accordance
#    with the said Rules.
#    The place of arbitration shall be in Taipei, Taiwan and the language shall
#    be English.
#    The arbitration award shall be final and binding to both parties.
#
#******************************************************************************
#<MStar Software>

# Source files folder
source_dir := \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)                               \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/bdma                          \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/sem                           \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/mmio                          \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/uart                          \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/irq                           \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/sys                           \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/miu                           \
    ./mxlib/drv/bdma                                                   \
    ./mxlib/drv/sem                                                    \
    ./mxlib/drv/mmio                                                   \
    ./mxlib/drv/uart                                                   \
    ./mxlib/drv/irq                                                    \
    ./mxlib/drv/sys                                                    \
    ./mxlib/drv/miu                                                    \
    ./mxlib/msos/arm_ca53                                              \
    ./mxlib/msos/linux                                                 \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/msos/linux                    \
    ./mxlib/msos/common                                                \
    ./mxlib/utopia_core/linux                                          \
    ./mxlib/drv/cpu                                                    \
    ./mxlib/api/swi2c                                                  \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/cpu

# Header file of specific chip
chip_includes := \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)                 \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/bdma            \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/sem             \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/mmio            \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/uart            \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/irq             \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/sys             \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/miu             \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/cpu

# Header files of library
lib_includes := \
    $(LOCAL_PATH)/mxlib                                              \
    $(LOCAL_PATH)/mxlib/drv/bdma                                     \
    $(LOCAL_PATH)/mxlib/drv/miu                                      \
    $(LOCAL_PATH)/mxlib/drv/sem                                      \
    $(LOCAL_PATH)/mxlib/drv/mmio                                     \
    $(LOCAL_PATH)/mxlib/drv/irq                                      \
    $(LOCAL_PATH)/mxlib/drv/sys                                      \
    $(LOCAL_PATH)/mxlib/drv/mmio                                     \
    $(LOCAL_PATH)/mxlib/drv/uart                                     \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)                 \
    $(LOCAL_PATH)/mxlib/msos                                         \
    $(LOCAL_PATH)/mxlib/msos/linux                                   \
    $(LOCAL_PATH)/mxlib/utopia_core/                                 \
    $(LOCAL_PATH)/mxlib/utopia_api_relation/                         \
    $(LOCAL_PATH)/mxlib/msfs                                         \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/msos/linux      \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/cpu

# Public Header files
public_includes := \
    $(LOCAL_PATH)/mxlib/include       \
    $(LOCAL_PATH)/mxlib/utopia_core   \
    $(LOCAL_PATH)/project/manhattan_android

# C flags
cflags := \
    -DMSOS_TYPE_LINUX      \
    -DMCU_ARM_CA12         \
    -DMS_OPTIMIZE          \
    -DOS_LINUX             \
    -DANDROID              \
    -DTV_OS                \
    -DCHIP_MANHATTAN          \
    -DMS_C_STDLIB          \
    -DFW_EMBEDDED_ASC      \
    -DCONFIG_MANHATTAN     \
    -DCONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER    \
    -DAUDIO_UTOPIA_20    \
    -DMSOS_TYPE_LINUX    \
    -DVDEC_UTOPIA_2K    \
    -DVDEC_UTOPIA_2K_KDRV    \
    -DSUPPORT_NEW_MEM_LAYOUT    \
    -DVDEC3    \
    -DVDEC3_FB    \
    -DSUPPORT_EVD=1    \
    -DSUPPORT_G2VP9=1    \
    -DSUPPORT_MSVP9=0    \
    -DDEMOD_UTOPIA_2K    \
    -DVIF_UTOPIA2K    \
    -DDMX_UTOPIA_2K    \
    -DDSCMB_UTOPIA_2K    \
    -DNJPEG_UTOPIA_2K    \
    -DGPD_UTOPIA_2K    \
    -DGOP_UTOPIA2K    \
    -DGFX_UTOPIA2K    \
    -DDIP_UTOPIA2K    \
    -DCONFIG_UTOPIAXP_PATCH

#=ACE==================
source_dir += \
    ./mxlib/api/ace \
    ./mxlib/drv/ace \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/ace
chip_includes += $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/ace/include
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/ace \
    $(LOCAL_PATH)/mxlib/api/ace \
    $(LOCAL_PATH)/mxlib/drv/ace/include
    
#=CEC==================
source_dir += \
    ./mxlib/api/cec \
    ./mxlib/drv/cec \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/cec
chip_includes += $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/cec/include
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/cec \
    $(LOCAL_PATH)/mxlib/api/cec \
    $(LOCAL_PATH)/mxlib/drv/cec/include

#=AUDIO================
source_dir += \
    ./mxlib/api/audio  \
    ./mxlib/drv/audio  \
    ./mxlib/drv/audsp  \
    ./mxlib/drv/ipauth \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/audio \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/audsp
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)       \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/audio \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/audsp
lib_includes +=

#=XC===================
source_dir += \
    ./mxlib/api/xc \
    ./mxlib/drv/xc \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/xc
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/xc/include
lib_includes += \
    $(LOCAL_PATH)/mxlib/api/xc \
    $(LOCAL_PATH)/mxlib/drv/xc/include
cflags += -DUTOPIA_V2

#=MBX==================
source_dir += \
    ./mxlib/api/mbx \
    ./mxlib/drv/mbx \
    ./mxlib/drv/mbx/api_mbx \
    ./mxlib/drv/mbx/linux \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/mbx
chip_includes +=
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/mbx

#=PNL==================
source_dir += \
    ./mxlib/api/pnl \
    ./mxlib/drv/pnl \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/pnl
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/pnl
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/pnl         \
    $(LOCAL_PATH)/mxlib/drv/pnl/include \
    $(LOCAL_PATH)/mxlib/api/pnl

#=GFX==================
source_dir += \
    ./mxlib/api/gfx \
    ./mxlib/drv/ge  \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/ge
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/ge
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/ge

#=GOP==================
source_dir += \
    ./mxlib/api/gop \
    ./mxlib/drv/gop \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/gop
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/gop
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/gop

#=VE===================
source_dir += \
    ./mxlib/drv/ve \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/ve
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/ve/include
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/ve \
    $(LOCAL_PATH)/mxlib/drv/ve/include

#=CMDQ=================
source_dir += \
    ./mxlib/drv/cmdq \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/cmdq
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/cmdq
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/cmdq

#=GPD==================
source_dir += \
    ./mxlib/api/gpd \
    ./mxlib/drv/gpd \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/gpd
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/gpd
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/gpd

#=NJPD_EX==============
source_dir += \
    ./mxlib/api/njpeg_ex \
    ./mxlib/drv/njpd_ex  \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/njpd_ex
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/njpd_ex
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/njpd_ex

#=MFE_EX===============
source_dir += \
    ./mxlib/api/mfe_ex \
    ./mxlib/drv/mfe_ex \
    ./mxlib/drv/mfe_ex/cModel \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/mfe_ex
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/mfe_ex
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/mfe_ex \
    $(LOCAL_PATH)/mxlib/drv/mfe_ex/cModel

#=VDEC_V3==============
source_dir += \
    ./mxlib/api/vdec_v3  \
    ./mxlib/drv/hvd_v3   \
    ./mxlib/drv/mjpeg_v3 \
    ./mxlib/drv/mvd_v3   \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/hvd_v3 \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/vpu_v3 \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/mvd_v3
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/hvd_v3 \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/vpu_v3 \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/mvd_v3
lib_includes += \
    $(LOCAL_PATH)/mxlib/api/vdec_v3  \
    $(LOCAL_PATH)/mxlib/drv/mjpeg_v3 \
    $(LOCAL_PATH)/mxlib/drv/hvd_v3 \
    $(LOCAL_PATH)/mxlib/drv/mvd_v3 \
    $(LOCAL_PATH)/mxlib/drv/flash/serial
cflags += \
    -DVDEC3           \
    -DVDEC3_FB        \
    -DSUPPORT_EVD=1   \
    -DSUPPORT_G2VP9=0 \
    -DSUPPORT_MSVP9=1 \
    -DSUPPORT_NEW_MEM_LAYOUT

##=DIP==================
#source_dir += \
#    ./mxlib/drv/dip \
#    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/dip
#chip_includes += \
#    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/dip
#lib_includes += \
#    $(LOCAL_PATH)/mxlib/drv/dip

#=FLASH================
source_dir += \
     ./mxlib/drv/flash/serial \
     ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/flash/serial
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/flash/serial
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/flash/serial

#=MVOP=================
source_dir += \
    ./mxlib/drv/mvop \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/mvop
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/mvop
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/mvop

#=GPIO=================
source_dir += \
    ./mxlib/drv/gpio \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/gpio
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/gpio
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/gpio

#=PWM==================
source_dir += \
    ./mxlib/drv/pwm \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/pwm
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/pwm
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/pwm

#=SAR==================
source_dir += \
    ./mxlib/drv/sar \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/sar
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/sar
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/sar

#=AESDMA===============
source_dir += \
    ./mxlib/drv/aesdma \
    ./mxlib/hal/$(TARGET_BOARD_PLATFORM)/aesdma
chip_includes += \
    $(LOCAL_PATH)/mxlib/hal/$(TARGET_BOARD_PLATFORM)/aesdma
lib_includes += \
    $(LOCAL_PATH)/mxlib/drv/aesdma

# =====================================================================
#
# Do not modify following section !!!!
# Unless you really want to modify make rule !!!!
#

#
# static
#
include $(CLEAR_VARS)

LOCAL_MODULE := libutopia
LOCAL_MODULE_TAGS := optional

ARCH_ARM_HAVE_THUMB_SUPPORT := true
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := \
    $(call find-subdir-subdir-files, $(source_dir), *.c, ) \
    $(call find-subdir-subdir-files, $(source_dir), *.s, )

LOCAL_C_INCLUDES := \
    $(chip_includes) \
    $(lib_includes) \
    $(public_includes)

LOCAL_CFLAGS := $(cflags)

LOCAL_SHARED_LIBRARIES := libcutils

include $(BUILD_STATIC_LIBRARY)

#
# shared
#
include $(CLEAR_VARS)

LOCAL_MODULE := libutopia
LOCAL_MODULE_TAGS := optional
LOCAL_WHOLE_STATIC_LIBRARIES := libutopia
LOCAL_SHARED_LIBRARIES := libcutils
include $(BUILD_SHARED_LIBRARY)

#last line is for chip UFO unify
define _install_bsp
   $(shell sed -i -e 's/#define DLL_PACKED.*/#define DLL_PACKED __attribute__((__packed__))/g' $(LOCAL_PATH)/mxlib/include/MsTypes.h)
   $(shell sed -i -e 's/#define UFO_PUBLIC_HEADER.*/#define UFO_PUBLIC_HEADER_700/g' $(LOCAL_PATH)/mxlib/include/UFO/UFO_manhattan.h)
   $(shell rm -rf $(LOCAL_PATH)/bsp)
   $(shell mkdir $(LOCAL_PATH)/bsp)
   $(shell cp $(call intermediates-dir-for,STATIC_LIBRARIES,$(LOCAL_MODULE))/$(LOCAL_MODULE)$($(my_prefix)STATIC_LIB_SUFFIX) $(LOCAL_PATH)/bsp)
   $(shell cp $(ANDROID_PRODUCT_OUT)/system/lib/$(LOCAL_MODULE).so $(LOCAL_PATH)/bsp)
   $(shell cp -r $(LOCAL_PATH)/mxlib/include $(LOCAL_PATH)/bsp/include)
   $(shell cp $(LOCAL_PATH)/mxlib/include/UFO/UFO_manhattan.h $(LOCAL_PATH)/mxlib/include/UFO.h)
endef

$(eval	$(call _install_bsp))
