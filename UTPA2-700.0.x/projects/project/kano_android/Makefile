
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
unexport SRCDIR_CHIP SRCDIR_LIB SRCDIR_SYS
unexport OBJ_C_CHIP0 OBJ_C_ASM0 OBJ_C_LIB0 OBJ_C_SYS0
unexport LIB_INC

# **********************************************
# Environment
# **********************************************
PROJ        = $(CURDIR)
ROOT        = $(PROJ)/../..
ROOTLIB     = $(ROOT)/mxlib
DDIAPI      = $(ROOTLIB)/api
DDIDRV      = $(ROOTLIB)/drv
DDIHAL      = $(ROOTLIB)/hal
PUBINC      = $(ROOTLIB)/include

export PUBINC
# **********************************************
# Import configure file
# Required:
#	$(ROOT), $(ROOTLIB)
# **********************************************
-include config


# **********************************************
# Project specific
# **********************************************
# Base Operating System
OS_BUILT    =


# **********************************************
# Directories
# **********************************************
# Source files of specific chip in library
SRCDIR_CHIP = \
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/hal/$(CHIP)/bdma                             \
            $(ROOTLIB)/hal/$(CHIP)/sem                              \
            $(ROOTLIB)/hal/$(CHIP)/mmio                             \
            $(ROOTLIB)/hal/$(CHIP)/uart                             \
            $(ROOTLIB)/hal/$(CHIP)/irq                              \
            $(ROOTLIB)/hal/$(CHIP)/sys                              \
            $(ROOTLIB)/hal/$(CHIP)/miu                              \

# Source files of library
SRCDIR_LIB  = \
            $(ROOTLIB)/drv/bdma                                     \
            $(ROOTLIB)/drv/sem                                      \
            $(ROOTLIB)/drv/mmio                                     \
            $(ROOTLIB)/drv/mbx/$(OS_TYPE)                           \
            $(ROOTLIB)/drv/uart                                     \
            $(ROOTLIB)/drv/irq                                      \
            $(ROOTLIB)/drv/sys                                      \
            $(ROOTLIB)/drv/miu                                      \


SRCDIR_SYS = \
            $(ROOTLIB)/msos/$(MCU_TYPE)                             \
            $(ROOTLIB)/msos/$(OS_TYPE)                              \
            $(ROOTLIB)/hal/$(CHIP)/msos/$(OS_TYPE)                  \
            $(ROOTLIB)/msos/common                                  \
            $(ROOTLIB)/utopia_core/$(OS_TYPE)

SRCDIR_FS = \
            $(ROOTLIB)/msfs/$(OS_TYPE)                              \

# Header file of specific chip
INCDIR_CHIP = \
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/hal/$(CHIP)/bdma                             \
            $(ROOTLIB)/hal/$(CHIP)/sem                              \
            $(ROOTLIB)/hal/$(CHIP)/mmio                             \
            $(ROOTLIB)/hal/$(CHIP)/uart                             \
            $(ROOTLIB)/hal/$(CHIP)/irq                              \
            $(ROOTLIB)/hal/$(CHIP)/sys                              \
            $(ROOTLIB)/hal/$(CHIP)/miu                              \

# Header file of inline assembly in library
INCDIR_ASM  = \

# Header files of library
INCDIR_LIB  = \
            $(PROJ)                                                 \
            $(ROOTLIB)                                              \
            $(ROOTLIB)/drv/bdma                                     \
            $(ROOTLIB)/drv/miu                                      \
            $(ROOTLIB)/drv/mbx                                      \
            $(ROOTLIB)/drv/mbx/$(OS_TYPE)                           \
            $(ROOTLIB)/drv/sem                                      \
            $(ROOTLIB)/drv/mmio                                     \
            $(ROOTLIB)/drv/irq                                      \
            $(ROOTLIB)/drv/sys                                      \
            $(ROOTLIB)/drv/mmio                                     \
            $(ROOTLIB)/drv/uart                                     \
            $(ROOTLIB)/hal/$(CHIP)                                  \
            $(ROOTLIB)/msos                                         \
            $(ROOTLIB)/msos/$(OS_TYPE)                              \
            $(ROOTLIB)/utopia_core/                                 \
            $(ROOTLIB)/msfs                                         \
            $(ROOTLIB)/drv/audio                                    \
            $(ROOTLIB)/hal/$(CHIP)/msos/$(OS_TYPE)                  \

INCDIR_LIB_H = \
            $(ROOTLIB)/drv/miu                                      \

# FW binary files of library
BINDIR_LIB  = \
            $(ROOTLIB)/hal/$(CHIP)/hvd                              \
            $(ROOTLIB)/hal/$(CHIP)/mvd                              \
            $(ROOTLIB)/hal/$(CHIP)/rvd                              \
            $(ROOTLIB)/drv/$(CHIP)/rvd                              \
            $(ROOTLIB)/hal/$(CHIP)/audsp                            \
            $(ROOTLIB)/hal/$(CHIP)/tsp                              \

BIN_PATH    = $(PROJ)/bin
OBJ_PATH    = $(PROJ)/obj
BSPLIB_PATH = $(PROJ)/bsp/lib
BSPINC_PATH = $(PROJ)/bsp/include
BSPBIN_PATH = $(PROJ)/bsp/bin


# Search source file path
VPATH       = $(SRCDIR_CHIP) $(SRCDIR_SYS) $(SRCDIR_FS) $(SRCDIR_ASM) $(SRCDIR_LIB)

include $(ROOTLIB)/$(CHIP)_android.mk
include $(ROOTLIB)/ddi.mk

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
SRC_C_FS    = $(foreach dir, $(SRCDIR_FS),    $(wildcard $(dir)/*.c))

OBJ_C_CHIP0 = $(notdir $(patsubst %.c, %.o,   $(SRC_C_CHIP)))
OBJ_C_ASM0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_ASM) ))
OBJ_C_LIB0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_LIB) ))
OBJ_C_SYS0  = $(notdir $(patsubst %.c, %.o,   $(SRC_C_SYS) ))
OBJ_C_FS0   = $(notdir $(patsubst %.c, %.o,   $(SRC_C_FS) ))
OBJ_C_CHIP  = $(foreach file, $(OBJ_C_CHIP0), $(OBJ_PATH)/$(file))
OBJ_C_ASM   = $(foreach file, $(OBJ_C_ASM0),  $(OBJ_PATH)/$(file))
OBJ_C_LIB   = $(foreach file, $(OBJ_C_LIB0),  $(OBJ_PATH)/$(file))
OBJ_C_SYS   = $(foreach file, $(OBJ_C_SYS0),  $(OBJ_PATH)/$(file))
OBJ_C_FS    = $(foreach file, $(OBJ_C_FS0),   $(OBJ_PATH)/$(file))


# **********************************************
# Libraries
# **********************************************

DDI_PATH = $(ROOT)/lib/$(CHIP)_$(OS_TYPE)/$(MCU_TYPE)_$(BLT_TYPE)


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

LIB_INC    += $(foreach dir, $(MXLIB), $(wildcard $(dir)/api*.h))
LIB_INC    += $(foreach dir, $(MXLIB), $(wildcard $(dir)/drv*.h))

# FW Binary files of output library
LIB_FW_BIN  =
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/fw*.bin))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/fw*.dat))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/out_*.bin))
LIB_FW_BIN += $(foreach dir, $(BINDIR_LIB), $(wildcard $(dir)/aucode_*.h))

# Public include files
PUB_INC    =
PUB_INC    += $(foreach dir, $(PUBINC), $(wildcard $(dir)/*.h))

# **********************************************
# Compiler and linker options
# **********************************************
ANDROID_DIR = $(ANDROID_BUILD_TOP)
OS_INC_DIR  = $(ANDROID_DIR)/bionic/libc/arch-arm/include \
              $(ANDROID_DIR)/bionic/libc/include \
              $(ANDROID_DIR)/bionic/libstdc++/include \
              $(ANDROID_DIR)/bionic/libc/kernel/common \
              $(ANDROID_DIR)/bionic/libc/kernel/arch-arm \
              $(ANDROID_DIR)/bionic/libm/include \
              $(ANDROID_DIR)/bionic/libm/include/arch/arm \
              $(ANDROID_DIR)/bionic/libthread_db/include \
              $(ANDROID_DIR)/system/core/include
OS_LIB_DIR  =


ALL_INC_DIR = $(INCDIR_ASM) $(INCDIR_CHIP) $(INCDIR_LIB) $(INCDIR_LIB_H) $(OS_INC_DIR)

CC_INCS     = $(foreach dir, $(ALL_INC_DIR), -I$(dir) -I$(PUBINC))

CC_DEFS     = $(CFG_CC_DEFS) -D'OS_LINUX' -D'LIB_NAME="$(IMAGENAME).lib"' -D'PWS_AUTOGEN' -D'SK_RELEASE' -D'MS_OPTIMIZE' -D'ANDROID' -D'TV_OS' -D'CONFIG_MANHATTAN'

# GCC options:
CC_OPTS     = $(CFG_CC_OPTS) -c

#----------------------------------------------------------------
# thum mode support, but use arm mode to build.
ARCH_ARM_HAVE_THUMB_SUPPORT := true
LOCAL_ARM_MODE := arm

# $(1): os/arch
define select-android-config-h
$(ANDROID_DIR)/build/core/combo/include/arch/$(1)/AndroidConfig.h
endef

arch_variant_cflags := -march=armv7-a \
                       -mfloat-abi=softfp \
                       -mfpu=neon

arch_variant_ldflags := -Wl,--fix-cortex-a8

COMMON_GLOBAL_CFLAGS := -DANDROID -fmessage-length=0 -W -Wall -Wno-unused -Winit-self -Wpointer-arith
COMMON_GLOBAL_CPPFLAGS := $(COMMON_GLOBAL_CFLAGS) -Wsign-promo
COMMON_RELEASE_CFLAGS := -DNDEBUG -UDEBUG
COMMON_RELEASE_CPPFLAGS := $(COMMON_RELEASE_CFLAGS)

TARGET_ERROR_FLAGS := -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point
TARGET_GLOBAL_CFLAGS := -fno-exceptions -Wno-multichar
TARGET_GLOBAL_CPPFLAGS :=
TARGET_RELEASE_CFLAGS := -O2 -g -fno-strict-aliasing
TARGET_RELEASE_CPPFLAGS :=

#----------------------------------------------------------------
TARGET_arm_CFLAGS :=    -O2 \
                        -fomit-frame-pointer \
                        -fstrict-aliasing    \
                        -funswitch-loops

# Modules can choose to compile some source as thumb. As
# non-thumb enabled targets are supported, this is treated
# as a 'hint'. If thumb is not enabled, these files are just
# compiled as ARM.
ifeq ($(ARCH_ARM_HAVE_THUMB_SUPPORT),true)
TARGET_thumb_CFLAGS :=  -mthumb \
                        -Os \
                        -fomit-frame-pointer \
                        -fno-strict-aliasing
else
TARGET_thumb_CFLAGS := $(TARGET_arm_CFLAGS)
endif

# Set FORCE_ARM_DEBUGGING to "true" in your buildspec.mk
# or in your environment to force a full arm build, even for
# files that are normally built as thumb; this can make
# gdb debugging easier.  Don't forget to do a clean build.
#
# NOTE: if you try to build a -O0 build with thumb, several
# of the libraries (libpv, libwebcore, libkjs) need to be built
# with -mlong-calls.  When built at -O0, those libraries are
# too big for a thumb "BL <label>" to go from one end to the other.
ifeq ($(FORCE_ARM_DEBUGGING),true)
  TARGET_arm_CFLAGS += -fno-omit-frame-pointer -fno-strict-aliasing
  TARGET_thumb_CFLAGS += -marm -fno-omit-frame-pointer
endif

ifeq ($(strip $(LOCAL_ARM_MODE)),)
    TARGET_ARCH_CFLAGS := $(TARGET_thumb_CFLAGS)
else
    TARGET_ARCH_CFLAGS := $(TARGET_arm_CFLAGS)
endif

TARGET_GLOBAL_CFLAGS += \
			-msoft-float -fpic -fPIE \
			-ffunction-sections \
			-fdata-sections \
			-funwind-tables \
			-fstack-protector \
			-Wa,--noexecstack \
			-Werror=format-security \
			-D_FORTIFY_SOURCE=1 \
			-fno-short-enums \
			$(arch_variant_cflags)

android_config_h := $(call select-android-config-h,linux-arm)
TARGET_ANDROID_CONFIG_CFLAGS := -include $(android_config_h) -I $(dir $(android_config_h))
TARGET_GLOBAL_CFLAGS += $(TARGET_ANDROID_CONFIG_CFLAGS)

# This warning causes dalvik not to build with gcc 4.6.x and -Werror.
# We cannot turn it off blindly since the option is not available
# in gcc-4.4.x.  We also want to disable sincos optimization globally
# by turning off the builtin sin function.
ifneq ($(filter 4.6 4.6.%, $(shell $(CC) --version)),)
TARGET_GLOBAL_CFLAGS += -Wno-unused-but-set-variable -fno-builtin-sin \
			-fno-strict-volatile-bitfields
endif

# This is to avoid the dreaded warning compiler message:
#   note: the mangling of 'va_list' has changed in GCC 4.4
#
# The fact that the mangling changed does not affect the NDK ABI
# very fortunately (since none of the exposed APIs used va_list
# in their exported C++ functions). Also, GCC 4.5 has already
# removed the warning from the compiler.
#
TARGET_GLOBAL_CFLAGS += -Wno-psabi

# We only need thumb interworking in cases where thumb support
# is available in the architecture, and just to be sure, (and
# since sometimes thumb-interwork appears to be default), we
# specifically disable when thumb support is unavailable.
ifeq ($(ARCH_ARM_HAVE_THUMB_SUPPORT),true)
TARGET_GLOBAL_CFLAGS += -mthumb-interwork
else
TARGET_GLOBAL_CFLAGS += -mno-thumb-interwork
endif

TARGET_GLOBAL_CPPFLAGS += -fvisibility-inlines-hidden

# More flags/options can be added here
TARGET_RELEASE_CFLAGS := \
			-DNDEBUG \
			-g \
			-Wstrict-aliasing=2 \
			-fgcse-after-reload \
			-frerun-cse-after-loop \
			-frename-registers


TARGET_GLOBAL_CFLAGS += $(COMMON_GLOBAL_CFLAGS)
TARGET_GLOBAL_CPPFLAGS += $(COMMON_GLOBAL_CPPFLAGS)
TARGET_RELEASE_CFLAGS += $(COMMON_RELEASE_CFLAGS)
TARGET_RELEASE_CPPFLAGS += $(COMMON_RELEASE_CPPFLAGS)

TARGET_GLOBAL_CFLAGS += $(TARGET_ERROR_FLAGS) $(TARGET_RELEASE_CFLAGS) $(TARGET_ARCH_CFLAGS)
TARGET_GLOBAL_CPPFLAGS += $(TARGET_ERROR_FLAGS) $(TARGET_RELEASE_CPPFLAGS) $(TARGET_ARCH_CFLAGS)

#----------------------------------------------------------------
CC_OPTS     += $(TARGET_GLOBAL_CFLAGS)

# AS options:
AS_OPTS     = $(CFG_AS_OPTS) -G0 -O2


# LD options:
LD_SCRIPT   =
LD_LIBS     = -L$(DDI_PATH) -L$(OS_LIB_DIR) -L$(CFG_STDLIB_DIR) -T$(LD_SCRIPT)
LD_OPTS     = $(CFG_LD_OPTS)

TARGET_GLOBAL_LDFLAGS += \
			-Wl,-z,noexecstack \
			-Wl,-z,relro \
			-Wl,-z,now \
			-Wl,--warn-shared-textrel \
			-Wl,--icf=safe \
			$(arch_variant_ldflags)

LD_OPTS     += $(TARGET_GLOBAL_LDFLAGS)


# **********************************************
# C Helper Utilities
# **********************************************
# Generate relative path for Doxygen
DXY_SRC     = $(subst $(PROJ)/,,$(LIB_INC) $(SRC_C_LIB) $(SRCDIR_SYS))
DXY_SRC     += $(subst $(PROJ)/,,$(foreach dir,$(MXLIB),$(wildcard $(dir)/*.c)))

# Generate options list for Lint
ifeq ($(shell uname -o),Cygwin)                                     #TODO: revise cygwin related
LINT_LOG    = $(shell cygpath -m $(CURDIR)/lint_test.log)
LINT_FILES  = $(shell cygpath -m $(CURDIR)/lint_files.lnt)
LINT_INC    = $(foreach dir,$(shell cygpath -m $(ALL_INC_DIR)),-i"$(dir)") -i"$(shell cygpath -m $(CFG_LINT))"
else
LINT_LOG    = $(CURDIR)/lint_test.log
LINT_FILES  = $(CURDIR)/lint_files.lnt
LINT_INC    = $(foreach dir,$(ALL_INC_DIR),-i"$(dir)") -i"$(CFG_LINT)"
LINT_INC    += -i"$(CURDIR)/../../tools/lint/aeon_include"
LINT_INC    += -i"$(CURDIR)/../../tools/lint/mips-linux-gnu_include"
LINT_INC    += -i"$(CURDIR)/../../tools/lint/mips-sde-elf_include"
endif


# **********************************************
# Rules
# **********************************************
.PHONY : all setup clean bsp depend docgen $(MXLIB)


# Project Build
all :  setup $(MXLIB) syslib

# Library Package
bsp :
	@rm -f -r                               $(BSPLIB_PATH)
	@rm -f -r                               $(BSPINC_PATH)
	@rm -f -r                               $(BSPBIN_PATH)
	-@mkdir -p                              $(BSPLIB_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPINC_PATH) 2> /dev/null;
	-@mkdir -p                              $(BSPBIN_PATH) 2> /dev/null;
	@cp         $(DDI_PATH)/*.$(LIB_EXT)    $(BSPLIB_PATH)
	@cp         $(LIB_INC)                  $(BSPINC_PATH)
	@cp         $(PUB_INC)                  $(BSPINC_PATH)
	@cp         $(LIB_FW_BIN)               $(BSPBIN_PATH)
	@$(AR) crsP $(BSPLIB_PATH)/libutopia.a $(OBJ_PATH)/*
	@echo "Make $(CHIP) $(PROJNAME) board support package...";

# Library Document Auto Generation
docgen :
	rm -f -r                                dxy_files;
	-mkdir -p                               dxy_files 2> /dev/null;
	@echo 'INPUT += \ ' >                   dxy.infile;
	@echo $(DXY_SRC) >>                     dxy.infile;
	doxygen                                 $(IMAGENAME).dxyfile;
#	rm -f                                   dxy.infile;
#	@echo DXY_SRC = $(DXY_SRC)


# Project Lint Report
lint :
	@echo "Generate $(CHIP) $(PROJNAME) lint report $(LINT_LOG)...";
	@echo `grep 'Error' $(LINT_LOG) | wc -l` LINT Errors


# Project Library Build
SYS_MSG :
	@echo [Makefile][$(notdir $(DDI_LIB))]

SYS_LIB : $(OBJ_C_CHIP) $(OBJ_C_SYS) $(OBJ_C_ASM) $(OBJ_C_LIB)
#Add section :MsOS
	@$(OBJCOPY) --add-section .mmodule_version=$(ROOTLIB)/msos/version_info $(dir $<)MsOS_$(OS_TYPE).o
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_LIB) $^;
else
	@$(AR) crsP $(DDI_LIB) $^;
endif
endif
	@date;

syslib : SYS_MSG SYS_LIB

$(MXLIB) :
	@echo [$(CHIP)_android.mk][$(notdir $@)]
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
$(OBJ_C_CHIP) $(OBJ_C_ASM) $(OBJ_C_SYS) : $(OBJ_PATH)/%.o : %.c
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

$(OBJ_C_LIB) $(OBJ_C_FS): $(OBJ_PATH)/%.o : %.c
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
	@$(OBJCOPY) --add-section .mmodule_version=$(subst /$(OS_TYPE),,$(dir $<D))version_info $@

# Single Driver Build
$(DRV_DIR): setup
	@echo [ddi.mk] $@
	@$(MAKE) -s -C $(DDIDRV)/$@ clean lib

$(API_DIR): setup
	@echo [ddi.mk] $@
	@$(MAKE) -s -C $(DDIAPI)/$@ clean lib

msos_setup: setup
	@echo [Makefile][libMsOS.$(LIB_EXT)]
	@if	[	-f $(OBJ_PATH)/MsOS_$(OS_TYPE).o	];\
		then\
		rm $(OBJ_PATH)/MsOS_$(OS_TYPE).o;\
	fi

msos: msos_setup $(DDI_PATH)/libMsOS.$(LIB_EXT)
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC)  $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_PATH)/libMsOS.$(LIB_EXT) $(OBJ_C_SYS);
	@$(OBJCOPY) --add-section .mmodule_version=$(ROOTLIB)/msos/version_info $(DDI_PATH)/libMsOS.$(LIB_EXT)
else
	@$(AR) crsP $(DDI_PATH)/libMsOS.$(LIB_EXT) $(OBJ_C_SYS);
endif
endif

$(DDI_PATH)/libMsOS.$(LIB_EXT) : $(OBJ_C_SYS)


msfs_setup: setup
	@echo [Makefile][libMsFS.$(LIB_EXT)]

msfs: msfs_setup $(DDI_PATH)/libMsFS.$(LIB_EXT)
ifneq ($(MAKE_TYPE),lint)
ifeq ($(LINK_TYPE),dynamic)
	@$(CC)  $(CFG_CC_OPTS) $(LD_OPTS) -o $(DDI_PATH)/libMsFS.$(LIB_EXT) $(OBJ_C_FS);
else
	@$(AR) crsP $(DDI_PATH)/libMsFS.$(LIB_EXT) $(OBJ_C_FS);
endif
endif

$(DDI_PATH)/libMsFS.$(LIB_EXT) : $(OBJ_C_FS)

# Project Setup
setup :
	-@mkdir -p $(OBJ_PATH) 2> /dev/null;
	-@mkdir -p $(BIN_PATH) 2> /dev/null;
	-@mkdir -p $(DDI_PATH) 2> /dev/null;
	@echo "Make $(CHIP) $(PROJNAME) library...";
	@date;
ifeq ($(MAKE_TYPE),lint)
	@rm -rf $(LINT_FILES)
	@rm -rf $(LINT_LOG)
endif


# Project Clean
clean :
	@$(foreach dir, $(MXLIB), $(MAKE) -s -C $(dir) clean;)
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