
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
# Makefile used for building application.
#
#
################################################################################

export
unexport VPATH
unexport SRCDIR_CHIP SRCDIR_LIB SRCDIR_SYS SRCDIR_FS
unexport OBJ_C_CHIP0 OBJ_C_ASM0 OBJ_C_LIB0 OBJ_C_SYS0
unexport LIB_INC


# **********************************************
# Environment
# **********************************************
PROJ        = $(PWD)
ROOT        = $(PROJ)/../..
ROOTLIB     = $(ROOT)/mxlib
DDIAPI      = $(ROOTLIB)/api
DDIDRV      = $(ROOTLIB)/drv
DDIHAL      = $(ROOTLIB)/hal
CONFIG      = $(PROJ)/.config
AUTOCONFIG  = $(PROJ)/include/generated/
PUBINC      = $(ROOTLIB)/include
MODULESPATH = $(ROOT)/modules

export DDIAPI DDIDRV PROJ PUBINC

# Import configure file
# Required:
#	$(ROOT), $(ROOTLIB)
# **********************************************
#-include config
-include $(CONFIG)
# **********************************************


# **********************************************
# Parse env arguments from $(CONFIG)
# **********************************************
TOOLCHAIN = $(subst ",,$(CONFIG_TOOLCHAIN))
TOOLCHAIN_VER = $(subst ",,$(CONFIG_TOOLCHAIN_VER))
CHIP = $(subst ",,$(CONFIG_CHIP))
CHIP_NAME = $(shell echo $(CHIP) | tr a-z A-Z)
OS_TYPE = $(subst ",,$(CONFIG_OS_TYPE))
LINK_TYPE = $(subst ",,$(CONFIG_LINK_TYPE))
FW_TYPE = $(subst ",,$(CONFIG_FW_TYPE))
ENDIAN = $(subst ",,$(CONFIG_ENDIAN))
PROCESSOR = $(subst ",,$(CONFIG_PROCESSOR))
MCU_TYPE = $(subst ",,$(CONFIG_MCU_TYPE))
COPRO_TYPE = $(subst ",,$(CONFIG_COPRO_TYPE))
XC_SEAMLESS_SCALING = $(subst ",,$(CONFIG_XC_SEAMLESS_SCALING))
XC_4K2K_PATCH_ENABLE = $(subst ",,$(CONFIG_XC_4K2K_PATCH_ENABLE))
MSTAR_DVFS_ENABLE = $(subst ",,$(CONFIG_MSTAR_DVFS_ENABLE))
BLT_TYPE = $(subst ",,$(CONFIG_BLT_TYPE))
FPU = $(subst ",,$(CONFIG_FPU))
EXT_PLATFORM_FLAG = $(subst ",,$(CONFIG_EXT_PLATFORM_FLAG))
MAKE_TYPE = $(subst ",,$(CONFIG_MAKE_TYPE))
DEBUG_LEVEL = $(subst ",,$(CONFIG_DEBUG_LEVEL))
AUDIO_UTOPIA_20_ENABLE = $(subst ",,$(CONFIG_AUDIO_UTOPIA_20_ENABLE))
AUDIO_UTOPIA2K_EN = $(subst ",,$(CONFIG_DRV_AUDIO))
KERN_VER = $(subst ",,$(CONFIG_KERN_VER))
FPGA_ENV = $(subst ",,$(CONFIG_FPGA))
ASSERT_TYPE = $(subst ",,$(CONFIG_ASSERT_TYPE))
CIPHER_UTOPIA_20_ENABLE = $(subst ",,$(CONFIG_CIPHER_UTOPIA_20_ENABLE))
CIPHER_NO_PRINTF_ENABLE = $(subst ",,$(CONFIG_CIPHER_NO_PRINTF_ENABLE))
CA_UTOPIA_20_ENABLE = $(subst ",,$(CONFIG_CA2_UTOPIA_20_ENABLE))
CA_NO_PRINTF_ENABLE = $(subst ",,$(CONFIG_CA2_NO_PRINTF_ENABLE))
STRUCT_PACK = $(subst ",,$(CONFIG_UTOPIA_DRIVER_STRUCT_PACKED))
# ******************************************************************************************************
# Customization Flag
# ******************************************************************************************************
ROLLS_ROYCE = $(subst ",,$(CONFIG_ROLLS_ROYCE))

# ******************************************************************************************************
# Parse DMX & DSCMB env arguments from $(CONFIG)
# ******************************************************************************************************
DMX_UTOPIA_20_ENABLE = $(subst ",,$(CONFIG_DMX_UTOPIA_20_ENABLE))
DMX_UTOPIA_2K_EN = $(subst ",,$(CONFIG_API_DMX))
STC64_ENABLE = $(subst ",,$(CONFIG_STC64_ENABLE))
VQ_ENABLE = $(subst ",,$(CONFIG_VQ_ENABLE))
FILE_VQ_ENABLE = $(subst ",,$(CONFIG_FIVQ_ENABLE))
FQ_ENABLE = $(subst ",,$(CONFIG_FQ_ENABLE))
EXT_FILTER_ENABLE = $(subst ",,$(CONFIG_EXT_FILTER_ENABLE))
TS2_ENABLE = $(subst ",,$(CONFIG_TSP2_ENABLE))
TS3_ENABLE = $(subst ",,$(CONFIG_TSP3_ENABLE))
MMFILEIN_ENABLE = $(subst ",,$(CONFIG_MMFILEIN_ENABLE))
MMFI3D_ENABLE = $(subst ",,$(CONFIG_MMFI3D_ENABLE))
MMFI2_EXTRA_AUFIFO_ENABLE = $(subst ",,$(CONFIG_MMFI2_EXTRA_AUFIFO_ENABLE))
MOBF_ENABLE = $(subst ",,$(CONFIG_MOBF_ENABLE))
MMFIMOBF_ENABLE = $(subst ",,$(CONFIG_MMFIMOBF_ENABLE))
USE_HWPCR_ENABLE = $(subst ",,$(CONFIG_USE_HWPCR_ENABLE))
DMX_DEBUG_TABLE_ENABLE = $(subst ",,$(CONFIG_DMX_DEBUG_TABLE_ENABLE))
EXTRA_PVR1_FLT_ENABLE = $(subst ",,$(CONFIG_EXTRA_PVR1_FLT_ENABLE))
TS2_LIVE_ENABLE = $(subst ",,$(CONFIG_TS2_LIVE_ENABLE))
TSIO_SUPPORT = $(subst ",,$(CONFIG_TSIO_SUPPORT))
TSO_SUPPORT = $(subst ",,$(CONFIG_TSO_SUPPORT))
RASP_SUPPORT = $(subst ",,$(CONFIG_RASP_SUPPORT))
#RASP_SUPPORT = $(subst ",,$(CONFIG_DRV_RASP))
TSO_20_SUPPORT = $(subst ",,$(CONFIG_TSO_VER_20_SUPPORT))
TSO_BOXSERIES_SUPPORT = $(subst ",,$(CONFIG_TSO_HW_BOX_SERIES))
TSP3_DULFLT_ENABLE = $(subst ",,$(CONFIG_TSP3_DULFLT_ENABLE))
MERGE_STR_SUPPORT = $(subst ",,$(CONFIG_MERGE_STREAM_SUPPORT))
SECURE_PVR_SUPPORT = $(subst ",,$(CONFIG_SECURE_PVR_SUPPORT))
TSO_PVR_SUPPORT = $(subst ",,$(CONFIG_TSO_PVR_SUPPORT))
PVR_IFRAMELUT_SUPPORT = $(subst ",,$(CONFIG_PVR_IFRAMELUT_SUPPORT))
GENERAL_DSCMBTYPE_SUPPORT = $(subst ",,$(CONFIG_MERGE_STREAM_SUPPORT))
DSCMB_CSA_ENABLE = $(subst ",,$(CONFIG_GENERAL_DSCMB_SUPPORT))
DSCMB_MULTI2_ENABLE = $(subst ",,$(CONFIG_DSCMB_MULTI2_ENABLE))
PCMCIA_MODULE_TYPE = $(subst ",,$(CONFIG_DRV_PCMCIA))
DONT_USE_CMA = $(subst ",,$(CONFIG_DONT_USE_CMA))
OTV_SUPPORT = $(subst ",,$(CONFIG_OTV_SUPPORT))
DMX_BOX_MULTIPROCESS_SUPPORT = $(subst ",,$(CONFIG_DMX_BOX_MULTIPROCESS_SUPPORT))
ifeq ($(CONFIG_DRV_DSCMB2), y)
DSCMB_UTOPIA_20_ENABLE = $(subst ",,$(CONFIG_DSCMB2_UTOPIA_20_ENABLE))
DSCMB_NSK_ENABLE = $(subst ",,$(CONFIG_DSCMB2_NSK_ENABLE))
DSCMB_MULTI2_ENABLE = $(subst ",,$(CONFIG_DSCMB2_MULTI2_ENABLE))
DSCMB_NO_PRINTF_ENABLE = $(subst ",,$(CONFIG_DSCMB2_NO_PRINTF_ENABLE))
DSCMB_HW_SEM_ENABLE = $(subst ",,$(CONFIG_DSCMB2_HW_SEM_ENABLE))
DSCMB_SECURE_MODE_ENABLE = $(subst ",,$(CONFIG_DSCMB2_SECURE_MODE_ENABLE))
endif
# ******************************************************************************************************

ifeq ($(CONFIG_HARD),y)
    FPU_ENABLE = 1
else
    FPU_ENABLE = 0
endif

# Image name
IMAGENAME = $(CHIP)_$(OS_TYPE)

# Lint Command, option file
LINT_OPT = opt_linux.lnt

ifeq ($(shell uname -o),Cygwin)
    LINT_CMD    = $(CURDIR)/../../tools/lint/lint-nt.exe -b -v -zero -fff
    LINT_CMD    += -i$(shell cygpath -m $(CURDIR)/../../tools/lint/lnt) $(LINT_OPT)
else
    LINT_CMD    = $(CURDIR)/../../tools/lint/flint -b -v -zero -fff
    LINT_CMD    += -i$(CURDIR)/../../tools/lint/lnt $(LINT_OPT)
endif


# **********************************************
# Build Option
# **********************************************


CFG_CC_DEFS = -D'CHIP_$(CHIP_NAME)' -D'MS_C_STDLIB'
CFG_CC_OPTS =
CFG_LD_OPTS =
CFG_CC_EXTRA_DEFS =
CFG_CC_EXTRA_OPTS =
CPU_NAME =


ifeq ($(CONFIG_CHIP_INT_SPI_MODE),y)
    CFG_CC_DEFS += -D'CHIP_INT_SPI_MODE'
endif


ifeq ($(ASSERT_TYPE), block)
CFG_CC_DEFS += -D'BLOCK_ASSERT'
endif

ifeq ($(MCU_TYPE),aeon)
    CFG_CC_DEFS += -D'MCU_AEON'
    ifeq ($(CONFIG_R2),y)
        CPU_NAME = r2
        COPRO_TYPE = R2
    else
        CPU_NAME = aeon
        COPRO_TYPE = AEON
    endif
endif
ifeq ($(MCU_TYPE),mips4ke)
    CFG_CC_DEFS += -D'MCU_MIPS_4KE'
    CPU_NAME = mips4ke
endif
ifeq ($(MCU_TYPE),mips34k)
    CFG_CC_DEFS += -D'MCU_MIPS_34K'
    CPU_NAME = mips34k
endif
ifeq ($(MCU_TYPE),mips74k)
    CFG_CC_DEFS += -D'MCU_MIPS_74K'
    CPU_NAME = mips74k
endif
ifeq ($(MCU_TYPE),arm_ca12)
    CFG_CC_DEFS += -D'MCU_ARM_CA12'
    CPU_NAME = armca12
endif
ifeq ($(MCU_TYPE),arm9)
    CFG_CC_DEFS += -D'MCU_ARM_9'
    CPU_NAME = armca9
endif

ifeq ($(MCU_TYPE),arm_ca7)
   CFG_CC_DEFS += -D'MCU_ARM_CA7'
    CPU_NAME = armca7
endif

ifeq ($(MCU_TYPE),arm_ca53)
    CFG_CC_DEFS += -D'MCU_ARM_CA53'
    CPU_NAME = armca53
endif

ifeq ($(CHIP),edison)
CFG_CC_DEFS += -D'edison'
endif

ifeq ($(CHIP),nike)
CFG_CC_DEFS += -D'nike'
endif

ifeq ($(CHIP),madison)
CFG_CC_DEFS += -D'madison'
endif

ifeq ($(CHIP),clippers)
CFG_CC_DEFS += -D'clippers'
endif

ifeq ($(CHIP),miami)
CFG_CC_DEFS += -D'miami'
endif

ifeq ($(OS_TYPE),ecos)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_ECOS'
	CFG_CC_EXTRA_DEFS += -D'UTPA2'
endif
ifeq ($(OS_TYPE),ucos)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_UCOS'
	CFG_CC_EXTRA_DEFS += -D'UTPA2'
endif
ifeq ($(OS_TYPE),linux)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_LINUX'
	CFG_CC_EXTRA_DEFS += -D'UTPA2'
endif
ifeq ($(OS_TYPE),nos)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_NOS'
	CFG_CC_EXTRA_DEFS += -D'UTPA2'

    ifeq ($(EXT_PLATFORM_FLAG),mboot)
        CFG_CC_EXTRA_DEFS += -D'MBOOT'
    endif

endif
ifeq ($(OS_TYPE),nuttx)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_NUTTX'
endif

ifeq ($(OS_TYPE),optee)
    CFG_CC_EXTRA_DEFS += -D'MSOS_TYPE_OPTEE'
endif

ifeq ($(FW_TYPE),external)
    CFG_CC_DEFS += -D'FW_EXTERNAL_BIN'
endif
ifeq ($(FW_TYPE),embedded)
    CFG_CC_DEFS += -D'FW_EMBEDDED_ASC'
endif

ifeq ($(CONFIG_UTOPIA_20),y)
    CFG_CC_DEFS += -D'UTOPIA_20'
endif

ifeq ($(AUDIO_UTOPIA2K_EN),m)
CFG_CC_DEFS += -D'AUDIO_UTOPIA2K'
endif

ifeq ($(FPGA_ENV),y)
    CFG_CC_DEFS += -D'FPGA_ENV'
endif

ifeq ($(CONFIG_DLMALLOC),y)
    CFG_CC_DEFS += -D'ENABLE_DLMALLOC'
endif

ifeq ($(CONFIG_KERNEL_DLMALLOC),y)
    CFG_CC_DEFS += -D'ENABLE_KERNEL_DLMALLOC'
	endif

ifeq ($(CONFIG_BIFROST_32BIT_MODE),y)
    CFG_CC_DEFS += -D'BIFROST_32BIT_MODE'
endif

ifeq ($(CONFIG_MLOG_ARM_472), y)
    MLOG_LIB_PATH = $(ROOT)/tools/MLog/arm-gnueabi/4.7.2/hardfloat/static/libMLog.a
endif
ifeq ($(CONFIG_MLOG_ARM_483), y)
    MLOG_LIB_PATH = $(ROOT)/tools/MLog/arm-gnueabi/4.8.3/hardfloat/static/libMLog.a
endif
ifeq ($(CONFIG_MLOG_ARM_49X), y)
    MLOG_LIB_PATH = $(ROOT)/tools/MLog/armv7a-gnueabi/4.9.x-google/hardfloat/static/libMLog.a
endif

# **********************************************
# DMX & DSCMB Build Option
# **********************************************

TSP4_CHIPS = kaiser keltic keres kano k6 k6lite k7u curry

CFG_CC_DEFS += -D'DMX_DEBUG=0'

ifeq ($(DMX_UTOPIA_20_ENABLE),y)
CFG_CC_DEFS += -D'DMX_UTOPIA_20'
CFG_CC_DEFS += -D'TSP_UTOPIA_20'
CFG_CC_DEFS += -D'MMFI_UTOPIA_20'
CFG_CC_DEFS += -D'RASP_UTOPIA_20'
CFG_CC_DEFS += -D'TSO_UTOPIA_20'
CFG_CC_DEFS += -D'OTV_UTOPIA_20'
endif

ifeq ($(DMX_UTOPIA_2K_EN),m)
CFG_CC_DEFS += -D'DMX_UTOPIA_2K'
endif

ifeq ($(CONFIG_API_VDEC_EX),m)
CFG_CC_DEFS += -D'VDEC_UTOPIA_2K'
endif

ifeq ($(CONFIG_API_VDEC_V3),m)
CFG_CC_DEFS += -D'VDEC_UTOPIA_2K'
endif

ifeq ($(CONFIG_API_NJPEG_EX),m)
CFG_CC_DEFS += -D'NJPEG_UTOPIA_2K'
endif

ifeq ($(CONFIG_API_GPD),m)
CFG_CC_DEFS += -D'GPD_UTOPIA_2K'
endif

ifeq ($(STC64_ENABLE),y)
CFG_CC_DEFS += -D'STC64_SUPPORT'
endif
ifeq ($(VQ_ENABLE),y)
CFG_CC_DEFS += -D'VQ_ENABLE'
endif
ifeq ($(FILE_VQ_ENABLE),y)
CFG_CC_DEFS += -D'FIVQ_ENABLE'
endif
ifeq ($(FQ_ENABLE),y)
CFG_CC_DEFS += -D'FQ_ENABLE'
endif
ifeq ($(EXT_FILTER_ENABLE),y)
CFG_CC_DEFS += -D'EXT_FILTER_SUPPORT'
endif
ifeq ($(TS2_ENABLE),y)
CFG_CC_DEFS += -D'TS2_IF_SUPPORT'
endif
ifeq ($(TS3_ENABLE),y)
CFG_CC_DEFS += -D'TS3_IF_SUPPORT'
endif
ifeq ($(MMFILEIN_ENABLE),y)
CFG_CC_DEFS += -D'MMFILEIN'
endif
ifeq ($(MMFI3D_ENABLE),y)
CFG_CC_DEFS += -D'MMFI_VD3D'
endif
ifeq ($(MMFI2_EXTRA_AUFIFO_ENABLE),y)
CFG_CC_DEFS += -D'MMFI2_EXTRA_AUFIFO'
endif
ifeq ($(MOBF_ENABLE),y)
CFG_CC_DEFS += -D'MOBF_ENABLE'
endif
ifeq ($(MMFIMOBF_ENABLE),y)
CFG_CC_DEFS += -D'MM_MOBF'
endif
ifeq ($(DMX_DEBUG_TABLE_ENABLE),y)
CFG_CC_DEFS += -D'DEBUG_TABLE_SUPPORT'
endif
ifeq ($(EXTRA_PVR1_FLT_ENABLE),y)
CFG_CC_DEFS += -D'EXT_PVR1_FLT'
endif

ifeq ($(TS2_LIVE_ENABLE),y)
CFG_CC_DEFS += -D'TS2_LIVE_SUPPORT'
endif

ifeq ($(TSIO_SUPPORT),y)
CFG_CC_DEFS += -D'TSIO_ENABLE'
endif

ifeq ($(TSO_SUPPORT),y)
CFG_CC_DEFS += -D'TSO_ENABLE'
endif

ifeq ($(RASP_SUPPORT),y)
CFG_CC_DEFS += -D'RASP_ENABLE'
endif

ifeq ($(TSO_20_SUPPORT),y)
CFG_CC_DEFS += -D'TSO_20_ENABLE'
endif

ifeq ($(TSO_BOXSERIES_SUPPORT),y)
CFG_CC_DEFS += -D'TSO_HW_BOX_SERIES'
endif

ifeq ($(TSP3_DULFLT_ENABLE),y)
    CFG_CC_DEFS += -D'DUP_PIDFLT'
endif

ifeq ($(DSCMB_CSA_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_CSA_ENABLE'
endif
ifeq ($(DSCMB_MULTI2_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_MULTI2_ENABLE'
endif

ifeq ($(DSCMB_UTOPIA_20_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_UTOPIA_20'
endif

ifeq ($(DSCMB_NO_PRINTF_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_NO_PRINTF'
endif

ifeq ($(DSCMB_HW_SEM_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_HW_SEM'
endif

ifeq ($(DSCMB_SECURE_MODE_ENABLE),y)
CFG_CC_DEFS += -D'DSCMB_SECURE_MODE'
endif


ifeq ($(CIPHER_UTOPIA_20_ENABLE),y)
CFG_CC_DEFS += -D'CIPHER_UTOPIA_20'
endif

ifeq ($(CIPHER_NO_PRINTF_ENABLE),y)
CFG_CC_DEFS += -D'CIPHER_NO_PRINTF'
endif

ifeq ($(CA_UTOPIA_20_ENABLE),y)
CFG_CC_DEFS += -D'CA_UTOPIA_20'
endif

ifeq ($(CA_NO_PRINTF_ENABLE),y)
CFG_CC_DEFS += -D'CA_NO_PRINTF'
endif

ifeq ($(PCMCIA_MODULE_TYPE),m)
CFG_CC_DEFS += -D'PCMCIA_UTPA2K'
endif

ifeq ($(OTV_SUPPORT),y)
CFG_CC_DEFS += -D'OTV_ENABLE'
endif

ifeq ($(DMX_BOX_MULTIPROCESS_SUPPORT),y)
CFG_CC_DEFS += -D'DMX_BOX_MULTIPROCESS_ENABLE'
endif

ifeq ($(DONT_USE_CMA),y)
CFG_CC_DEFS += -D'DONT_USE_CMA'
endif

ifeq ($(USE_HWPCR_ENABLE),y)
    ifeq ($(CHIP),$(filter $(CHIP),$(TSP4_CHIPS)))
    CFG_CC_DEFS += -D'HW_PCRFLT_ENABLE=1'
    else
    CFG_CC_DEFS += -D'HWPCR_ENABLE'
    endif
endif

ifeq ($(MERGE_STR_SUPPORT),y)
CFG_CC_DEFS += -D'MERGE_STR_SUPPORT'
endif

ifeq ($(SECURE_PVR_SUPPORT),y)
CFG_CC_DEFS += -D'SECURE_PVR_ENABLE'
endif

ifeq ($(CHIP), $(filter $(CHIP),monaco miami clippers muji munich milan monet))
CFG_CC_DEFS += -D'DMX_RESET_FI_TIMESTAMP'
endif

ifeq ($(TSO_PVR_SUPPORT),y)
CFG_CC_DEFS += -D'TSO_PVR_SUPPORT'
endif

ifeq ($(PVR_IFRAMELUT_SUPPORT),y)
CFG_CC_DEFS += -D'PVR_IFRAMELUT_SUPPORT'
endif

ifeq ($(CHIP), $(filter $(CHIP),curry))
CFG_CC_DEFS += -D'DMX_OPT_BOUND'
endif

ifeq ($(GENERAL_DSCMBTYPE_SUPPORT),y)
CFG_CC_DEFS += -D'GENERAL_DSCMB'
endif

ifeq ($(XC_4K2K_PATCH_ENABLE),y)
CFG_CC_DEFS += -D'ENABLE_4K2K_PATCH'
endif

# **********************************************
# Customization Flag
# **********************************************
ifeq ($(ROLLS_ROYCE),y)
CFG_CC_DEFS += -D'ROLLS_ROYCE'
endif

# **********************************************

# **********************************************
# Tool Chain Configuration
#
# Required ENV
#       $(ROOT) e.g //utopia/
#
# Supported TOOLCHAIN
#       mips-linux-gnu
#       mips-sde-elf
#       mips2_fp_le
#       aeon
#       mipsisa32-elf (OBSOLETE)
#       arm-none-linux-gnueabi

# Effected ENV
#       CFG_STDLIB_DIR =
#       CFG_LINT       =
#       CFG_CC_OPTS    +=
#       CFG_LD_OPTS    +=
#       CFG_AS_OPTS    +=
# **********************************************

include $(ROOTLIB)/cfg_tool.mk


ifeq ($(LINK_TYPE),dynamic)
    CFG_LD_OPTS += -shared
    LIB_EXT     = so
else
    CFG_LD_OPTS += --gc-sections -static
    LIB_EXT     = a
endif



# Project specific
# **********************************************
# Base Operating System
OS_BUILT    =


# **********************************************
# Directories
# **********************************************
# Source files of specific CHIP in library
SRCDIR_CHIP = \
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/hal/$(CHIP)/bdma                             \
            $(ROOTLIB)/hal/$(CHIP)/sem                              \
            $(ROOTLIB)/hal/$(CHIP)/mmio                             \
            $(ROOTLIB)/hal/$(CHIP)/mbx                              \
            $(ROOTLIB)/hal/$(CHIP)/uart                             \
            $(ROOTLIB)/hal/$(CHIP)/irq                              \
            $(ROOTLIB)/hal/$(CHIP)/sys                              \
            $(ROOTLIB)/hal/$(CHIP)/miu                              \
            $(ROOTLIB)/hal/$(CHIP)/gpio                             \
	        $(ROOTLIB)/hal/$(CHIP)/cpu                              \
	 	  	$(ROOTLIB)/hal/$(CHIP)/dac                              \
			$(ROOTLIB)/api/swi2c                                  	\
			$(ROOTLIB)/drv/cpu                                  	\
			$(ROOTLIB)/drv/gpio

# Source files of library
SRCDIR_LIB  = \
            $(ROOTLIB)/drv/bdma                                     \
            $(ROOTLIB)/drv/sem                                      \
            $(ROOTLIB)/drv/mmio                                     \
            $(ROOTLIB)/drv/mbx/$(OS_TYPE)                           \
            $(ROOTLIB)/drv/mbx/api_mbx                              \
            $(ROOTLIB)/drv/uart                                     \
            $(ROOTLIB)/drv/irq                                      \
            $(ROOTLIB)/drv/sys                                      \
            $(ROOTLIB)/drv/miu                                      \


SRCDIR_SYS = \
            $(ROOTLIB)/msos/$(MCU_TYPE)                             \
            $(ROOTLIB)/msos/$(OS_TYPE)                              \
			$(ROOTLIB)/hal/$(CHIP)/msos/$(OS_TYPE)					\
            $(ROOTLIB)/msos/common                                  \
			$(ROOTLIB)/utopia_core/$(OS_TYPE)

ifeq ($(OS_TYPE),$(filter $(OS_TYPE),linux ecos))
SRCDIR_FS = $(ROOTLIB)/msfs/$(OS_TYPE)
endif

# Header file of specific chip
INCDIR_CHIP = \
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/hal/$(CHIP)/bdma                             \
            $(ROOTLIB)/hal/$(CHIP)/sem                              \
            $(ROOTLIB)/hal/$(CHIP)/mmio                             \
            $(ROOTLIB)/hal/$(CHIP)/mbx                              \
            $(ROOTLIB)/hal/$(CHIP)/uart                             \
            $(ROOTLIB)/hal/$(CHIP)/irq                              \
            $(ROOTLIB)/hal/$(CHIP)/sys                              \
            $(ROOTLIB)/hal/$(CHIP)/miu                              \
            $(ROOTLIB)/hal/$(CHIP)/dac/include                      \
            $(ROOTLIB)/hal/$(CHIP)/gpio

ifeq ($(OS_TYPE),ucos)
INCDIR_CHIP +=  $(ROOTLIB)/hal/$(CHIP)/cpu
endif

# Header file of inline assembly in library
INCDIR_ASM  = \

# Header files of library
INCDIR_LIB  = \
            $(PROJ)                                                 \
            $(ROOTLIB)                                              \
            $(ROOTLIB)/api/vdec                                     \
            $(ROOTLIB)/drv/bdma                                     \
            $(ROOTLIB)/drv/mbx                                      \
            $(ROOTLIB)/drv/mbx/$(OS_TYPE)                           \
            $(ROOTLIB)/drv/sem                                      \
            $(ROOTLIB)/drv/mmio                                     \
            $(ROOTLIB)/drv/irq                                      \
            $(ROOTLIB)/drv/sys                                      \
            $(ROOTLIB)/drv/uart                                     \
            $(ROOTLIB)/drv/miu                                     	\
            $(ROOTLIB)/drv/dac                                      \
            $(ROOTLIB)/drv/audio/internal                          	\
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/msos                                         \
            $(ROOTLIB)/msos/$(OS_TYPE)                              \
			$(ROOTLIB)/hal/$(CHIP)/msos/$(OS_TYPE)					\
			$(ROOTLIB)/utopia_core/                                 \
            $(ROOTLIB)/utopia_api_relation/
ifeq ($(OS_TYPE),$(filter $(OS_TYPE),linux ecos))
INCDIR_LIB  += $(ROOTLIB)/msfs
endif

INCDIR_LIB_H = \
            $(ROOTLIB)/drv/miu                                      \

# FW binary files of library
BINDIR_LIB  = \
            $(ROOTLIB)/hal/$(CHIP)/hvd                              \
            $(ROOTLIB)/hal/$(CHIP)/mvd                              \
            $(ROOTLIB)/hal/$(CHIP)/audsp                            \
            $(ROOTLIB)/hal/$(CHIP)/tsp                              \

ifeq ($(CHIP),monaco)
BINDIR_LIB  += $(ROOTLIB)/hal/$(CHIP)/vpu_v3
else
BINDIR_LIB  += $(ROOTLIB)/hal/$(CHIP)/vpu_ex
endif

BIN_PATH    = $(PROJ)/bin
OBJ_PATH    = $(PROJ)/obj
BSPLIB_PATH = $(PROJ)/bsp/lib
BSPINC_PATH = $(PROJ)/bsp/include
BSPBIN_PATH = $(PROJ)/bsp/bin
BSPMSC_PATH = $(PROJ)/bsp/misc


# Search source file path
VPATH       = $(SRCDIR_CHIP) $(SRCDIR_SYS) $(SRCDIR_ASM) $(SRCDIR_LIB)
ifeq ($(OS_TYPE),linux)
VPATH       +=  $(SRCDIR_FS)
endif

include $(ROOTLIB)/ddi.mk
include $(ROOTLIB)/mxlib.mk


# **********************************************
# Image file names
# **********************************************
IMAGE_BIN   = $(BIN_PATH)/$(IMAGENAME).bin
IMAGE_REC   = $(BIN_PATH)/$(IMAGENAME).rec
IMAGE_ELF   = $(BIN_PATH)/$(IMAGENAME).elf
IMAGE_MAP   = $(BIN_PATH)/$(IMAGENAME).map
IMAGE_DIS   = $(BIN_PATH)/$(IMAGENAME).dis
IMAGE_SIZ   = $(BIN_PATH)/$(IMAGENAME).siz
IMAGE_SYM   = $(BIN_PATH)/$(IMAGENAME).sym


# **********************************************
# Files to be compiled
# **********************************************
SRC_C_CHIP  = $(foreach dir, $(SRCDIR_CHIP),  $(wildcard $(dir)/*.c))
SRC_C_ASM   = $(foreach dir, $(SRCDIR_ASM),   $(wildcard $(dir)/*.c))
SRC_C_LIB   = $(foreach dir, $(SRCDIR_LIB),   $(wildcard $(dir)/*.c))
SRC_C_SYS   = $(foreach dir, $(SRCDIR_SYS),   $(wildcard $(dir)/*.c))

OBJ_C_CHIP0 = $(notdir $(patsubst %.c, %.o,   $(SRC_C_CHIP)))
OBJ_C_ASM0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_ASM) ))
OBJ_C_LIB0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_LIB) ))
OBJ_C_SYS0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_SYS) ))

OBJ_C_CHIP  = $(foreach file, $(OBJ_C_CHIP0), $(OBJ_PATH)/$(file))
OBJ_C_ASM   = $(foreach file, $(OBJ_C_ASM0),  $(OBJ_PATH)/$(file))
OBJ_C_LIB   = $(foreach file, $(OBJ_C_LIB0),  $(OBJ_PATH)/$(file))
OBJ_C_SYS   = $(foreach file, $(OBJ_C_SYS0),  $(OBJ_PATH)/$(file))

ifeq ($(OS_TYPE),linux)
SRC_C_FS    = $(foreach dir, $(SRCDIR_FS),    $(wildcard $(dir)/*.c))
OBJ_C_FS0   = $(notdir $(patsubst %.c, %.o,   $(SRC_C_FS) ))
OBJ_C_FS    = $(foreach file, $(OBJ_C_FS0),   $(OBJ_PATH)/$(file))
endif

# **********************************************
# Libraries
# **********************************************

DDI_PATH = $(ROOT)/lib/$(CHIP)_$(OS_TYPE)_$(MCU_TYPE)/$(MCU_TYPE)_$(BLT_TYPE)

# Standard Libraries Path


# Middleware Libraries Path


# Libraries
DDI_LIB = $(DDI_PATH)/lib$(OS_TYPE).$(LIB_EXT)

# Include files of output library
LIB_INC     =
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/asm*.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/api*.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/drv*.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/Ms*.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/regCHIP.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/$(CFG_BOARD).h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/utopia.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/utopia_dapi.h))
LIB_INC    += $(foreach dir, $(INCDIR_LIB), $(wildcard $(dir)/utopia_driver_id.h))
ifeq ($(CONFIG_API_DAC), y)
LIB_INC    += $(foreach dir, $(INCDIR_CHIP), $(wildcard $(dir)/mdrv_dac_tbl.h))
endif

LIB_INC_2   =
LIB_INC_2  += $(foreach dir, $(MXLIB-y), $(wildcard $(dir)/api*.h))
LIB_INC_2  += $(foreach dir, $(MXLIB-y), $(wildcard $(dir)/drv*.h))
LIB_INC_2  += $(foreach dir, $(MXLIB-m), $(wildcard $(dir)/api*.h))
LIB_INC_2  += $(foreach dir, $(MXLIB-m), $(wildcard $(dir)/drv*.h))

# FW Binary files of output library
LIB_FW_BIN  =
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/fw*.bin))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/fw*.dat))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/out_*.bin))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/aucode_*.h))

# Misc files
LIB_MISC    =
ifeq ($(CONFIG_API_DAC), y)
LIB_MISC   += $(foreach dir, $(INCDIR_CHIP), $(wildcard $(dir)/mdrv_dac_tbl.c))
endif

# Public include files
PUB_INC    =
PUB_INC    += $(foreach dir, $(PUBINC), $(wildcard $(dir)/*.h))

# **********************************************
# Compiler and linker options
# **********************************************
ifeq ($(OS_TYPE),linux)

ifeq ($(EXT_PLATFORM_FLAG),android)
# **********************************************
# Compiler and linker options for android
# **********************************************

ANDROID_DIR = $(ANDROID_BUILD_TOP)
OS_INC_DIR  = \
              $(ANDROID_DIR)/bionic/libc/arch-arm/include \
              $(ANDROID_DIR)/bionic/libc/include \
              $(ANDROID_DIR)/bionic/libc/kernel/common \
              $(ANDROID_DIR)/bionic/libc/kernel/arch-arm \
              $(ANDROID_DIR)/bionic/libm/include \
              $(ANDROID_DIR)/bionic/libm/include/arch/arm \
              $(ANDROID_DIR)/bionic/libthread_db/include \
              $(ANDROID_DIR)/system/core/include
OS_LIB_DIR  = $(ANDROID_DIR)/bionic/lib

ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)

CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))

CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_LINUX' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'
CC_DEFS     += -D'TV_OS' -D'ANDROID' -D'SK_RELEASE'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -c
# TARGET_GLOBAL_CFLAGS
CC_OPTS    += -fno-exceptions -Wno-multichar \
              -fpic \
              -ffunction-sections \
              -funwind-tables \
              -fstack-protector \
              -Wa,--noexecstack \
              -Werror=format-security \
              -fno-short-enums \
              -Wno-psabi \
              -mthumb-interwork \
              -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith
#TARGET_ERROR_FLAGS
CC_OPTS    += -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point
# TARGET_RELEASE_CFLAGS
CC_OPTS    += -DNDEBUG \
              -g \
              -Wstrict-aliasing=2 \
              -finline-functions \
              -fno-inline-functions-called-once \
              -fgcse-after-reload \
              -frerun-cse-after-loop \
              -frename-registers \
              -DNDEBUG -UDEBUG \
              -DMS_OPTIMIZE
# TARGET_ARCH_CFLAGS
CC_OPTS    += -Os \
              -fomit-frame-pointer \
              -fstrict-aliasing \
              -funswitch-loops \
              -finline-limit=300

else
# **********************************************
# Compiler and linker options for linux
# **********************************************

OS_INC_DIR  =

ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)

CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)

CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_LINUX' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

ifeq ($(EXT_PLATFORM_FLAG),tvos)
CC_DEFS     += -D'TV_OS'
endif

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef
CC_OPTS    += -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions
CC_OPTS    += -ffunction-sections -fdata-sections
CC_OPTS    += -c
ifeq ($(TOOLCHAIN),arm-buildroot-linux-uclibcgnueabi)
CC_OPTS    += -fPIC
endif

ifeq ($(CONFIG_STACK_PROTECTOR),y)
    # currently we use 4.8.2 and 4.9.2
    ifeq ($(TOOLCHAIN_VER), $(filter $(TOOLCHAIN_VER), 4.8.2 4.9.2))
        CC_OPTS += -fstack-protector
    else
        $(error "Only toolchain ver > 4.8 supports -fstack-protector (current = $(TOOLCHAIN_VER))")
    endif
endif

# for symbol backtrace
CC_OPTS    += -rdynamic
endif

else
ifeq ($(OS_TYPE),ucos)
# **********************************************
# Compiler and linker options for ucos
# **********************************************
OS_INC_DIR  = $(ROOT)/../../GreenLion/platform/ucos/include
OS_INC_DIR  += $(ROOT)/../../GreenLion/platform/ucos/HDI3/inc

ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)
CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)
CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_UCOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef
CC_OPTS    += -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions
ifeq ($(TOOLCHAIN),arm-none-eabi)
CC_OPTS    += -c
else
CC_OPTS    += -c -G0
endif

else
ifeq ($(OS_TYPE),ecos)
# **********************************************
# Compiler and linker options for Ecos
# **********************************************
ifeq ($(FPU_ENABLE),1)
ifeq ($(CHIP),kano)
    OS_INC_DIR  = $(ROOT)/../../GreenLion/platform/ecos/ecos_kano_CA53_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else ifeq ($(CHIP),k6)
    OS_INC_DIR  = $(ROOT)/../../../GreenLion/platform/ecos/ecos_k6_CA53_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else ifeq ($(CHIP),k6lite)
    OS_INC_DIR  = $(ROOT)/../../../GreenLion/platform/ecos/ecos_k6lite_CA7_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else
    OS_INC_DIR  = $(ROOT)/../GreenLion/platform/ecos/ecos_keres_74kf_net_fileio_posix_fat_ntfs_mipsisa32_c++/include
endif
else
ifeq ($(CHIP),kano)
    OS_INC_DIR  = $(ROOT)/../../GreenLion/platform/ecos/ecos_kano_CA53_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else ifeq ($(CHIP),k6)
    OS_INC_DIR  = $(ROOT)/../../../GreenLion/platform/ecos/ecos_k6_CA53_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else ifeq ($(CHIP),k6lite)
    OS_INC_DIR  = $(ROOT)/../../../GreenLion/platform/ecos/ecos_k6lite_CA7_net_fileio_posix_fat_cpuload_c++_none_armeabi/include
else
    OS_INC_DIR  = $(ROOT)/../GreenLion/platform/ecos/ecos_keres_74kc_net_fileio_posix_fat_ntfs_mipsisa32_c++/include
endif
endif

ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)
CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)
CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_ECOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef
CC_OPTS    += -fno-strict-aliasing -fno-optimize-sibling-calls -fno-exceptions
ifeq ($(TOOLCHAIN),arm-none-eabi)
CC_OPTS    += -c
else
CC_OPTS    += -c -G0
endif

else
ifeq ($(OS_TYPE),nuttx)
# **********************************************
# Compiler and linker options for Nuttx
# **********************************************

OS_INC_DIR  =
ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)
CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)
CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_NOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Wcast-align
CC_OPTS    += -fomit-frame-pointer -fno-exceptions
CC_OPTS    += -ffunction-sections -fdata-sections
CC_OPTS    += -c -march=aeonR2 -EL

else
ifeq ($(OS_TYPE),optee)
# **********************************************
# Compiler and linker options for Optee
# **********************************************

OS_INC_DIR  =
ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)
CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)
CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_NOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Wcast-align
CC_OPTS    += -fomit-frame-pointer -fno-exceptions
CC_OPTS    += -ffunction-sections -fdata-sections
CC_OPTS    += -c

else
# **********************************************
# Compiler and linker options for Non-OS
# **********************************************

OS_INC_DIR  =
ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)
CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))
CC_INCS     += -I$(AUTOCONFIG)
CC_DEFS     = $(CFG_CC_DEFS) $(CFG_CC_EXTRA_DEFS) -D'OS_NOS' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef -Wcast-align
CC_OPTS    += -fomit-frame-pointer -fno-exceptions
CC_OPTS    += -ffunction-sections -fdata-sections

ifeq ($(MCU_TYPE),aeon)
CC_OPTS    += -march=aeonR2 -EL
endif


ifeq ($(MCU_TYPE), arm_ca53)
ifeq ($(OS_TYPE), nos)
CC_OPTS    += -mno-unaligned-access
else
CC_OPTS    += -mstrict-align
endif
endif
ifeq ($(MCU_TYPE), arm_ca12)
CC_OPTS    += -mno-unaligned-access
endif
ifeq ($(MCU_TYPE), arm_ca7)
CC_OPTS    += -mno-unaligned-access
endif
ifeq ($(MCU_TYPE),arm9)
CC_OPTS    += -mno-unaligned-access
endif

ifeq ($(EXT_PLATFORM_FLAG),mboot)
CC_OPTS    += -fno-short-enums -Wno-unused -Wno-cast-align -fno-strict-aliasing
endif

CC_OPTS    += -c

endif
endif
endif
endif
endif

# AS options:
AS_OPTS     = $(CFG_AS_OPTS) -G0 -Os


# LD options:
LD_SCRIPT   =
LD_LIBS     = -L$(DDI_PATH) -L$(OS_LIB_DIR) -L$(CFG_STDLIB_DIR) -T$(LD_SCRIPT)
LD_OPTS     = $(CFG_LD_OPTS)

ifeq ($(OS_TYPE),nos)
LD_OPTS     += -nostdlib
endif

ifeq ($(OS_TYPE),ecos)
LD_OPTS     += -nostdlib -Map $(IMAGE_MAP)
endif


ifeq ($(BLT_TYPE),debug)
CC_OPTS    += -O0 -g3
CC_DEFS    += -D'MS_DEBUG'
else
CC_OPTS    += -Os
CC_DEFS    += -D'MS_OPTIMIZE'
endif

CC_OPTS    += $(CFG_CC_EXTRA_OPTS)
CC_DEFS    += -D'CONFIG_ENABLE_MENUCONFIG'
# **********************************************
# C Helper Utilities
# **********************************************
# Generate relative path for Doxygen
ifeq ($(OS_TYPE),linux)
DXY_SRC     = $(subst $(PROJ)/,,$(PUBINC))
else
DXY_SRC     = $(subst $(PROJ)/,,$(LIB_INC) $(SRC_C_LIB) $(SRCDIR_SYS))
endif
DXY_SRC    += $(subst $(PROJ)/,,$(foreach dir,$(MXLIB),$(wildcard $(dir)/*.c)))

# Generate options list for Lint
ifeq ($(shell uname -o),Cygwin)                                     #TODO: revise cygwin related
LINT_LOG    = $(shell cygpath -m $(CURDIR)/lint_test.log)
LINT_FILES  = $(shell cygpath -m $(CURDIR)/lint_files.lnt)
LINT_INC    = $(foreach dir,$(shell cygpath -m $(ALL_INC_DIR)),-i"$(dir)") -i"$(shell cygpath -m $(CFG_LINT))"
else
LINT_LOG    = $(CURDIR)/lint_test.log
LINT_FILES  = $(CURDIR)/lint_files.lnt
LINT_INC    = $(foreach dir,$(ALL_INC_DIR),-I"$(dir)")
ifeq ($(OS_TYPE),linux)
LINT_INC    += -I/tools/arm-2010.09/arm-none-linux-gnueabi/libc/usr/include/
LINT_INC    += -I/tools/arm-2010.09/lib/gcc/arm-none-linux-gnueabi/4.5.1/include/
LINT_INC    += -I/tools/arm/Linaro/gcc-linaro-aarch64-linux-gnu-4.9-2014.07_linux/
endif
endif


# **********************************************
# Rules
# **********************************************
.PHONY : all setup clean bsp depend docgen $(MXLIB-y) $(MXLIB-m) parallel

parallel:
	@$(MAKE) -s -f parallel.mk

# Project Build

all: parallel

log_gccver:
	@$(CC) -v > .GCCver.log 2>&1

check_build_toolver: log_gccver
	@echo         "### Start checking workspace tool chain version against $(TOOLCHAIN) $(TOOLCHAIN_VER) ###"
	@if [ '$(shell grep "gcc version $(TOOLCHAIN_VER)" .GCCver.log)' ] ;\
		then echo "### Pass checking workspace tool chain version ###" ;\
			rm .GCCver.log ;\
		else echo -e "\033[0;31m[Error] Tool chain version should be $(TOOLCHAIN) $(TOOLCHAIN_VER). please check it!!\033[0m" ;\
		echo "Your workspace tool chain version:" ;\
		$(CC) -v ;\
		echo "### End checking workspace tool chain version  ###" ;\
		rm .GCCver.log ;exit 99;\
	fi

# Library Document Auto Generation
docgenOneUtopia :
	rm -f -r                               dxyOneUtopia_files;
	-mkdir -p                               dxyOneUtopia_files 2> /dev/null;
	doxygen -u                               bspOneUtopia.dxyfile;
# Library Package
bsp :
	@rm -f -r                               $(BSPLIB_PATH)
	@rm -f -r                               $(BSPINC_PATH)
	@rm -f -r                               $(BSPBIN_PATH)
	@rm -f -r                               $(BSPMSC_PATH)
	-@mkdir -p                              $(BSPLIB_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPINC_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPBIN_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPMSC_PATH) 2> /dev/null;
	@cp         $(DDI_PATH)/*.$(LIB_EXT)    $(BSPLIB_PATH)
ifeq ($(ByPassOneUtopia),y)
	@echo -e "\033[41;36mBypass OneUtopia!!!\033[0m"
	@cp         $(PUB_INC)                  $(BSPINC_PATH)
	@cp         $(LIB_INC)                  $(BSPINC_PATH)
	@cp         $(LIB_INC_2)                $(BSPINC_PATH)

else
ifeq ($(CHIP),$(filter $(CHIP),maserati maxim M7821 M7621))
	@echo -e "\033[41;36m$(CHIP) Enable OneUtopia\033[0m"
	@sh Reduced.sh api_with_header_file.csv $(shell echo $(PROJ) |sed -e "s/\/build[/]*$$//g") $$Version
else
	@cp         $(PUB_INC)                  $(BSPINC_PATH)
	@cp         $(LIB_INC)                  $(BSPINC_PATH)
	@cp         $(LIB_INC_2)                $(BSPINC_PATH)
endif
endif
	@cp         $(LIB_FW_BIN)               $(BSPBIN_PATH)
	@test -f $(BSPLIB_PATH)/libdrvCIPHER.so && ln -s libdrvCIPHER.so $(BSPLIB_PATH)/libdrvAESDMA.so  || { echo ""; }
ifeq ($(CONFIG_API_DAC), y)
	@cp         $(LIB_MISC)                 $(BSPMSC_PATH)
endif

ifeq ($(EXT_PLATFORM_FLAG),android)
	@$(AR) crsP $(BSPLIB_PATH)/libutopia.a $(OBJ_PATH)/*
endif
ifeq ($(OS_TYPE),ecos)
ifeq ($(FPU_ENABLE), 1)
	@echo "Add Dummy Hardfloat Section for check in ...";
#	for i in `ls $(BSPLIB_PATH)/*.a`; do $(AR) -r $$i $(CHIP)/hardfloat.o; done
endif
endif
ifeq ($(OS_TYPE),linux)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) $(OBJ_PATH)/*.o $(MLOG_LIB_PATH) -o $(PROJ)/bsp/libutopia.so
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) $(OBJ_PATH)/*.o $(MLOG_LIB_PATH) -s -o $(PROJ)/bsp/libutopia.so.strip
endif
endif
ifeq ($(CHIP),$(filter $(CHIP),maserati maxim M7821 M7621))
	@echo "CHIP: $(CHIP) use Oneutopia-docgen";
	@make docgenOneUtopia
	@tar zcvf $(CHIP)_$(OS_TYPE)_$(EXT_PLATFORM_FLAG)_$(CPU_NAME)_$(FPU)_$(LINK_TYPE)_$(BLT_TYPE).doc.tgz dxyOneUtopia_files 1> /dev/null
	@tar zcvf $(CHIP)_$(OS_TYPE)_$(EXT_PLATFORM_FLAG)_$(CPU_NAME)_$(FPU)_$(LINK_TYPE)_$(BLT_TYPE).tgz bsp 1> /dev/null
else
	@echo "Make $(CHIP) $(PROJNAME) board support package...";
	@make docgen
	@tar zcvf $(CHIP)_$(OS_TYPE)_$(EXT_PLATFORM_FLAG)_$(CPU_NAME)_$(FPU)_$(LINK_TYPE)_$(BLT_TYPE).doc.tgz dxy_files 1> /dev/null
	@tar zcvf $(CHIP)_$(OS_TYPE)_$(EXT_PLATFORM_FLAG)_$(CPU_NAME)_$(FPU)_$(LINK_TYPE)_$(BLT_TYPE).tgz bsp 1> /dev/null
endif

# Library Document Auto Generation
docgen :
	rm -f -r                                dxy_files;
	-mkdir -p                               dxy_files 2> /dev/null;
	@echo 'INPUT += \ ' >                   dxy.infile;
	@echo $(DXY_SRC) >>                     dxy.infile;
	doxygen -u                                bsp.dxyfile;
#	rm -f                                   dxy.infile;
	@echo DXY_SRC = $(DXY_SRC)


# Project Lint Report
lint :
	@echo "Generate $(CHIP) $(PROJNAME) lint report $(LINT_LOG)...";
	@echo `grep 'Error' $(LINT_LOG) | wc -l` LINT Errors


# Project Library Build
SYS_MSG :
	@echo [Makefile][$(notdir $(DDI_LIB))]
ifeq ($(OS_TYPE),ecos)
    @echo OS_INC_DIR = $(OS_INC_DIR)
endif

SYS_LIB : $(OBJ_C_CHIP) $(OBJ_C_SYS) $(OBJ_C_ASM) $(OBJ_C_LIB)
	@echo "syslib" $(OBJ_C_SYS)
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_LIB) $^ $(MLOG_LIB_PATH);
	@$(OBJCOPY) --add-section .mmodule_version=$(ROOTLIB)/msos/version_info $(DDI_LIB)
else
ifeq ($(EXT_PLATFORM_FLAG),android)
	@$(AR) crsP $(DDI_LIB) $^;
else
	@$(AR) -cru $(DDI_LIB) $^;
endif
endif
endif
	@date;
	@echo '*************************************** Building environment *****************************************'
	@echo '#'
	@echo '# CHIP[$(CHIP)] OS[$(OS_TYPE)] CPU[$(CPU_NAME)] FPU[$(FPU)] link_type[$(LINK_TYPE)] release_type[$(BLT_TYPE)] Platform[$(EXT_PLATFORM_FLAG)]'
	@echo '#'

ifeq (1,1)
syslib : check_build_toolver SYS_MSG SYS_LIB
else
syslib :
endif

$(MXLIB-m):
	@$(MAKE) -s -C $@ lib

$(MXLIB-y) :
	@$(MAKE) -s -C $@ lib
ifeq ($(PARA), 1)
	@if [ ! -f $@/cpptestscan.bdf ]; then					\
		echo ERROR! No $@/cpptestscan.bdf;					\
	fi
	@if [ -f $@/cpptestscan.bdf ]; then						\
		cat $@/cpptestscan.bdf >> $(PROJ)/cpptestscan.bdf;	\
		rm $@/cpptestscan.bdf;								\
	fi
endif

# Project Source Build
$(OBJ_C_CHIP) $(OBJ_C_ASM) $(OBJ_C_SYS): $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@echo [CC] $(notdir $@)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;
else
	@echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
	@echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
	@echo $< >>$(LINT_FILES)
endif
endif

#	@$(AR) -cru $(DDI_LIB) $@;

ifeq ($(OS_TYPE),linux)
$(OBJ_C_LIB) $(OBJ_C_FS): $(OBJ_PATH)/%.o : %.c
else
$(OBJ_C_LIB) : $(OBJ_PATH)/%.o : %.c
endif
ifneq ($(MAKE_TYPE),lint)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -o $@ $<;
else
	echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
	echo $< >>$(LINT_FILES)
endif
endif
#ifeq ($(OS_TYPE),$(notdir $(<D)))


# Single Driver Build
$(DRV_DIR): check_build_toolver setup
	@echo [ddi.mk] $@
	@$(MAKE) -C $(DDIDRV)/$@ clean lib
	@echo '**************************************** Building environment ****************************************'
	@echo '#'
	@echo '# CHIP[$(CHIP)] OS[$(OS_TYPE)] CPU[$(CPU_NAME)] FPU[$(FPU)] link_type[$(LINK_TYPE)] release_type[$(BLT_TYPE)] Platform[$(EXT_PLATFORM_FLAG)]'
	@echo '#'

$(API_DIR): check_build_toolver setup
	@echo [ddi.mk] $@
	@$(MAKE) -s -C $(DDIAPI)/$@ clean lib
	@echo '**************************************** Building environment ****************************************'
	@echo '#'
	@echo '# CHIP[$(CHIP)] OS[$(OS_TYPE)] CPU[$(CPU_NAME)] FPU[$(FPU)] link_type[$(LINK_TYPE)] release_type[$(BLT_TYPE)] Platform[$(EXT_PLATFORM_FLAG)]'
	@echo '#'

msos_setup: setup
	@echo [Makefile][libMsOS.$(LIB_EXT)]
#	@if	[	-f $(OBJ_PATH)/MsOS_$(OS_TYPE).o	];\
		then\
		rm $(OBJ_PATH)/MsOS_$(OS_TYPE).o;\
	fi

msos: check_build_toolver msos_setup $(DDI_PATH)/libMsOS.$(LIB_EXT)
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC)  $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_PATH)/libMsOS.$(LIB_EXT) $(OBJ_C_SYS) $(MLOG_LIB_PATH);
	@$(OBJCOPY) --add-section .mmodule_version=$(ROOTLIB)/msos/version_info $(DDI_PATH)/libMsOS.$(LIB_EXT)
else
ifeq ($(EXT_PLATFORM_FLAG),android)
	@$(AR) crsP $(DDI_PATH)/libMsOS.$(LIB_EXT) $(OBJ_C_SYS);
else
	@$(AR) -cru $(DDI_PATH)/libMsOS.$(LIB_EXT) $(OBJ_C_SYS);
endif
endif
endif

$(DDI_PATH)/libMsOS.$(LIB_EXT) : $(OBJ_C_SYS)

build_env: $(DRV_DIR)
	@echo 'haha'

msfs_setup: setup
	@echo [Makefile][libMsFS.$(LIB_EXT)]

msfs: msfs_setup $(DDI_PATH)/libMsFS.$(LIB_EXT)
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC)  $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_PATH)/libMsFS.$(LIB_EXT) $(OBJ_C_FS);
else
ifeq ($(EXT_PLATFORM_FLAG),android)
	@$(AR) crsP $(DDI_PATH)/libMsFS.$(LIB_EXT) $(OBJ_C_FS);
else
	@$(AR) -cru $(DDI_PATH)/libMsFS.$(LIB_EXT) $(OBJ_C_FS);
endif
endif
endif

$(DDI_PATH)/libMsFS.$(LIB_EXT) : $(OBJ_C_FS)

# Project Setup
setup :
ifeq ($(CHIP),$(filter $(CHIP),maserati maxim M7821 M7621))
	@echo -e "\033[41;36m $(shell echo $(PROJ) |sed -e "s/\/build[/]*$$//g") \033[0m"
	@sh MXlibBk.sh $(shell echo $(PROJ) |sed -e "s/\/build[/]*$$//g")
	@cp $(PROJ)/MsVersion.h $(ROOTLIB)/include/MsVersion.h
endif
	@cp $(ROOTLIB)/include/UFO/UFO_$(CHIP).h $(ROOTLIB)/include/UFO.h
	@sed -i -e 's/#define UFO_PUBLIC_HEADER.*/#define UFO_PUBLIC_HEADER_700/g' $(PUBINC)/UFO.h
ifeq ($(STRUCT_PACK),y)
	@sed -i -e 's/#define DLL_PACKED.*/#define DLL_PACKED __attribute__((__packed__))/g' $(PUBINC)/MsTypes.h
else
	@sed -i -e 's/#define DLL_PACKED.*/#define DLL_PACKED/g' $(PUBINC)/MsTypes.h
endif
	-@mkdir -p $(OBJ_PATH) 2> /dev/null;
	-@mkdir -p $(BIN_PATH) 2> /dev/null;
	-@mkdir -p $(DDI_PATH) 2> /dev/null;
	@echo "Make $(CHIP) $(PROJNAME) library...";
	@date;
ifeq ($(MAKE_TYPE),lint)
	@rm -rf $(LINT_FILES)
	@rm -rf $(LINT_LOG)
endif


# Kernel Driver
# ## LKM2K, KERNELDIR, and EXTRA_CFLAGS were defined in utpa2k.mk
CFG_2K_DEFS = $(CFG_CC_DEFS)

include $(PROJ)/utpa2k.mk

EXTRA_CFLAGS += $(foreach dir, $(ALL_INC_DIR), -I$(dir))
EXTRA_CFLAGS += -I$(AUTOCONFIG)
ifeq ($(BLT_TYPE),debug)
EXTRA_CFLAGS    += -D'MS_DEBUG'
else
EXTRA_CFLAGS    += -D'MS_OPTIMIZE'
endif
utpa2k-srcs = $(foreach dir, $(LKM2K), $(wildcard $(dir)/*.c))
utpa2k-objs-tmp := $(patsubst %.c,%.o,$(utpa2k-srcs))
utpa2k-objs := $(subst $(PWD), , $(utpa2k-objs-tmp))
obj-m := utpa2k.o
#obj-m := hello.o

EXTRA_CFLAGS += -D'SUPPORT_EVD=1'
EXTRA_CFLAGS += -D'SUPPORT_NEW_MEM_LAYOUT'

kdrv:
	@cp $(ROOTLIB)/include/UFO/UFO_$(CHIP).h $(ROOTLIB)/include/UFO.h
	@sed -i -e 's/#define UFO_PUBLIC_HEADER.*/#define UFO_PUBLIC_HEADER_700/g' $(PUBINC)/UFO.h
ifeq ($(STRUCT_PACK),y)
	@sed -i -e 's/#define DLL_PACKED.*/#define DLL_PACKED __attribute__((__packed__))/g' $(PUBINC)/MsTypes.h
else
	@sed -i -e 's/#define DLL_PACKED.*/#define DLL_PACKED/g' $(PUBINC)/MsTypes.h
endif
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

####lsy++
ifeq ($(ExportAllSymbol),y)
	@sh ./CreateKsym.sh
	$(MAKE) -s clean
	@cp $(ROOTLIB)/include/UFO/UFO_$(CHIP).h $(ROOTLIB)/include/UFO.h
	@sed -i -e 's/#define UFO_PUBLIC_HEADER.*/#define UFO_PUBLIC_HEADER_700/g' $(PUBINC)/UFO.h
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules EXTRA_CFLAGS="$(EXTRA_CFLAGS)"
endif

# Project Clean
clean :
	@rm -f $(ROOTLIB)/include/UFO.h
	@$(foreach dir, $(MXLIB-y), $(MAKE) -s -C $(dir) clean;)
	@$(foreach dir, $(MXLIB-m), $(MAKE) -s -C $(dir) clean;)
	@rm -rf $(OBJ_PATH)
	@rm -rf $(BIN_PATH)
	@rm -rf $(DDI_PATH)
	@rm -rf $(PKG_DIR)/include/$(CHIP);
	@rm -rf $(PKG_DIR)/os;
	@rm -rf lint_files
	@rm -rf dxy_files
	@rm -rf bsp
	@rm -f *.tgz
	@rm -f dxy.*
	@find $(MODULESPATH) \
		\( -name '*.[os]' -o -name '*.ko' -o -name '.*.cmd' \
		-o -name '.*.d' -o -name '.*.tmp' -o -name '*.mod.c' \
		-o -name '*.symtypes' -o -name '*.symvers' -o -name 'modules.order' \
		-o -name 'Module.markers' -o -name '.tmp_*.o.*' \) \
		-type f -print | xargs rm -f
	@find . \
		\( -name 'utpa2k.ko' -o -name 'utpa2k.mod.c' \
		-o -name 'utpa2k.mod.o' -o -name 'utpa2k.o' \
		-o -name '.utpa2k.*' -o -name 'modules.order' -o -name 'Module.symvers' \) \
		-type f -print | xargs rm -f
	@rm -rf .tmp_versions
ifeq ($(PARA), 1)
	@rm -f $(PROJ)/cpptestscan.bdf
endif
	@echo "Clean $(CHIP) $(PROJNAME) library..."


# Project Dependence
depend :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC_C_CHIP) $(SRC_C_ASM) $(SRC_C_LIB) > $(BIN_PATH)/depend.mk


# Project Dependence Full version
dep :
	$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) -M $(SRC_C_CHIP) $(SRC_C_ASM) $(SRC_C_LIB) > $(BIN_PATH)/depend.mk
	cat $(BIN_PATH)/depend.mk | sed -e 's/\(.*\)\.o/\$$\(OBJ_PATH\)\/\1.o/g' > depend.mk


-include depend.mk

# **********************************************
# Menuconfig
# **********************************************
PHONY += all
_all: all

srctree		:= $(if $(KBUILD_SRC),$(KBUILD_SRC),$(PWD))
#src		:= $(srctree)

export srctree objtree

KCONFIG_CONFIG	?= .config
export KCONFIG_CONFIG

# SHELL used by kbuild
CONFIG_SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
	  else if [ -x /bin/bash ]; then echo /bin/bash; \
	  else echo sh; fi ; fi)

HOSTCC       = gcc
HOSTCXX      = g++
HOSTCFLAGS   = -Wall -Wmissing-prototypes -Wstrict-prototypes -Os -fomit-frame-pointer
HOSTCXXFLAGS = -Os

# Beautify output
# ---------------------------------------------------------------------------
#
# Normally, we echo the whole command before executing it. By making
# that echo $($(quiet)$(cmd)), we now have the possibility to set
# $(quiet) to choose other forms of output instead, e.g.
#
#         quiet_cmd_cc_o_c = Compiling $(RELDIR)/$@
#         cmd_cc_o_c       = $(CC) $(c_flags) -c -o $@ $<
#
# If $(quiet) is empty, the whole command will be printed.
# If it is set to "quiet_", only the short version will be printed.
# If it is set to "silent_", nothing will be printed at all, since
# the variable $(silent_cmd_cc_o_c) doesn't exist.
#
# A simple variant is to prefix commands with $(Q) - that's useful
# for commands that shall be hidden in non-verbose mode.
#
#	$(Q)ln $@ :<
#
# If KBUILD_VERBOSE equals 0 then the above command will be hidden.
# If KBUILD_VERBOSE equals 1 then the above command is displayed.

ifeq ($(KBUILD_VERBOSE),1)
  quiet =
  Q =
else
  quiet=quiet_
  Q = @
endif

# If the user is running make -s (silent mode), suppress echoing of
# commands

ifneq ($(findstring s,$(MAKEFLAGS)),)
  quiet=silent_
endif

export quiet Q KBUILD_VERBOSE


# Look for make include files relative to root of kernel src
MAKEFLAGS += --include-dir=$(srctree)

# We need some generic definitions (do not try to remake the file).
$(srctree)/scripts/Kbuild.include: ;
include $(srctree)/scripts/Kbuild.include

# Make variables (CC, etc...)

AWK		= awk

export HOSTCC

# ===========================================================================
# Rules shared between *config targets and build targets

# Basic helpers built in scripts/
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic
	$(Q)rm -f .tmp_quiet_recordmcount

# To avoid any implicit rule to kick in, define an empty command.
scripts/basic/%: scripts_basic ;

PHONY += outputmakefile
# outputmakefile generates a Makefile in the output directory, if using a
# separate output directory. This allows convenient use of make in the
# output directory.
outputmakefile:


# To make sure we do not include .config for any of the *config targets
# catch them early, and hand them over to scripts/kconfig/Makefile
# It is allowed to specify more targets when calling make, including
# mixing *config targets and build targets.
# For example 'make oldconfig all'.
# Detect when mixed targets is specified, and make a second invocation
# of make so .config is not included in this case either (for *config).


config-targets := 0
mixed-targets  := 0
dot-config     := 1

ifneq ($(filter $(no-dot-config-targets), $(MAKECMDGOALS)),)
	ifeq ($(filter-out $(no-dot-config-targets), $(MAKECMDGOALS)),)
		dot-config := 0
	endif
endif

ifeq ($(KBUILD_EXTMOD),)
        ifneq ($(filter config %config,$(MAKECMDGOALS)),)
                config-targets := 1
                ifneq ($(filter-out config %config,$(MAKECMDGOALS)),)
                        mixed-targets := 1
                endif
        endif
endif

ifeq ($(mixed-targets),1)
# ===========================================================================
# We're called with mixed targets (*config and build targets).
# Handle them one by one.

%:: FORCE
	$(Q)$(MAKE) -C $(srctree) KBUILD_SRC= $@

else
ifeq ($(config-targets),1)
# ===========================================================================
# *config targets only - make sure prerequisites are updated, and descend
# in scripts/kconfig to make the *config target

# Read arch specific Makefile to set KBUILD_DEFCONFIG as needed.
# KBUILD_DEFCONFIG may point out an alternative default configuration
# used for 'make defconfig'
export KBUILD_DEFCONFIG KBUILD_KCONFIG

config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include/config
	$(Q)$(MAKE) $(build)=scripts/kconfig $@

%config: scripts_basic outputmakefile FORCE
	$(Q)mkdir -p include/config
	$(Q)$(MAKE) $(build)=scripts/kconfig $@

else


# Read in config
-include include/config/auto.conf

# Read in dependencies to all Kconfig* files, make sure to run
# oldconfig if changes are detected.
-include include/config/auto.conf.cmd

# To avoid any implicit rule to kick in, define an empty command
$(KCONFIG_CONFIG) include/config/auto.conf.cmd: ;

# If .config is newer than include/config/auto.conf, someone tinkered
# with it and forgot to run make oldconfig.
# if auto.conf.cmd is missing then we are probably in a cleaned tree so
# we execute the config step to be sure to catch updated Kconfig files
include/config/%.conf: $(KCONFIG_CONFIG) include/config/auto.conf.cmd
	$(Q)$(MAKE) -f $(srctree)/Makefile silentoldconfig


endif #ifeq ($(config-targets),1)
endif #ifeq ($(mixed-targets),1)

PHONY += checkstack kernelrelease kernelversion

PHONY += FORCE
FORCE:

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)
