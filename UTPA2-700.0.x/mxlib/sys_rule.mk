
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

export
unexport SYS_SRC
unexport SYS_CC_INCS
unexport VPATH


# **********************************************
# Rules
# **********************************************
LIB_NAME = $(SYS_NAME)

SYS_SRC     = $(wildcard $(CURDIR)/*.c)
SYS_OBJ     = $(foreach file, $(notdir $(patsubst %.c, %.o, $(SYS_SRC))), $(OBJ_PATH)/$(file))
SYS_IMPORT_SRC = $(wildcard $(CURDIR)/obj_import/$(OS_TYPE)/$(MCU_TYPE)/*.src)
SYS_IMPORT_OBJ = $(foreach file, $(notdir $(patsubst %.src, %.o, $(SYS_IMPORT_SRC))), $(OBJ_PATH)/$(file))
SYS_CC_INCS = $(foreach dir,$(SYS_INCDIR),-I$(dir))

ifeq ($(MAKE_TYPE),lint)
ifeq ($(shell uname -o),Cygwin)
LINT_INC   += $(foreach dir,$(shell cygpath -m $(SYS_INCDIR)),-i"$(dir)")
else
LINT_INC   += $(foreach dir,$(SYS_INCDIR),-i"$(dir)")
endif
endif

VPATH =

ifeq ("$(UTOPIA_BSP_VERSION)", "")
UTOPIA_BSP_VERSION = 000000
endif

ifeq ("$(P4PORT)", "")
SW_VERSION_SYS_$(SYS_NAME) = 0
else
SW_VERSION_SYS_$(SYS_NAME) := $(shell p4 changes -m1 "./...\#have" | egrep Change | awk '{print $$2}')
ifeq ("$(SW_VERSION_SYS_$(SYS_NAME))", "")
SW_VERSION_SYS_$(SYS_NAME) = 0
endif
endif
SW_SYS_FLOW_ENABLE = TRUE
CC_DEFS += -DSW_VERSION_SYS_$(SYS_NAME)=$(SW_VERSION_SYS_$(SYS_NAME))

.PHONY : all clean depend

all : $(SYS_OBJ) $(SYS_IMPORT_OBJ)

lib : setup all
ifeq ($(LINK_TYPE),dynamic)
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) $(wildcard $(OBJ_PATH)/*.T) $(SYS_OBJ) $(SYS_IMPORT_OBJ) -o $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) ;
	@rm -f $(OBJ_PATH)/*.T
endif

ifeq ($(TOOLCHAIN),mips-linux-gnu)
ifeq ($(OS_TYPE),nos)
SW_MBOOT_FLOW = TRUE
endif
endif

	@$(OBJCOPY) --add-section .mmodule_version=../version_info $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)

ifeq ($(PARA), 1)
	@if [ ! -f $(DDIDRV)/$@/cpptestscan.bdf ]; then						\
		echo ERROR! No $(DDIDRV)/$@/cpptestscan.bdf;					\
	fi
	@if [ -f $(DDIDRV)/$@/cpptestscan.bdf ]; then						\
		cat $(DDIDRV)/$@/cpptestscan.bdf >> $(PROJ)/cpptestscan.bdf;	\
		rm -f $(DDIDRV)/$@/cpptestscan.bdf;								\
	fi
endif

$(SYS_IMPORT_OBJ) :
	echo find api import_obj cp from $(filter %$(notdir $*).src,$(SYS_IMPORT_SRC)) to $@
	cp $(filter %$(notdir $*).src,$(SYS_IMPORT_SRC)) $@
ifneq ($(LINK_TYPE),dynamic)
	@$(AR) cru $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $@;
ifeq ($(BLT_TYPE),retail)
	@$(STRIP) -S $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
endif
endif

$(SYS_OBJ) : $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@echo [CC] $(notdir $<)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) $(SYS_CC_INCS) -o $@ $<;
ifneq ($(LINK_TYPE),dynamic)
	@$(AR) cru $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $@;
endif
else
	@echo [LINT] $(notdir $<)
ifeq ($(shell uname -o),Cygwin)
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $(shell cygpath -m $<) >>$(LINT_LOG)
#	echo $(shell cygpath -m $<) >>$(LINT_FILES)
else
	@$(LINT_CMD) $(CC_DEFS) $(LINT_INC) $< >>$(LINT_LOG)
#	echo $< >>$(LINT_FILES)
endif
endif

setup :
ifeq ($(LINK_TYPE),dynamic)
	@echo [api_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
	@rm -f $(OBJ_PATH)/*.T
else
	@echo [api_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
endif

clean :
	@rm -f $(SYS_OBJ)
	@rm -f $(SYS_IMPORT_OBJ)
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).a
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).so


