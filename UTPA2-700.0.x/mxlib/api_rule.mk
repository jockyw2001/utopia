
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
unexport DRV_DIR
unexport API_SRC
unexport API_CC_INCS
unexport VPATH


# **********************************************
# DRV Name
# **********************************************
#API_NAME = aaa

# **********************************************
# DRV Source Files
# **********************************************
#DRV_DIR = \
#            ddd		                                             \


# **********************************************
# Rules
# **********************************************
LIB_NAME = api$(API_NAME)

ifeq ($(API_NAME),HDMITX)
ifeq ($(CHIP),keltic)
API_SRC	    = $(filter-out $(wildcard $(CURDIR)/apiHDMITx.c), $(wildcard $(CURDIR)/*.c))
else
API_SRC	    = $(filter-out $(wildcard $(CURDIR)/apiHDMITx_Wrap.c), $(wildcard $(CURDIR)/*.c))
endif
else
API_SRC     = $(wildcard $(CURDIR)/*.c)
endif

API_OBJ     = $(foreach file, $(notdir $(patsubst %.c, %.o, $(API_SRC))), $(OBJ_PATH)/$(file))
API_IMPORT_SRC = $(wildcard $(CURDIR)/obj_import/$(OS_TYPE)/$(MCU_TYPE)/*.src)
API_IMPORT_OBJ = $(foreach file, $(notdir $(patsubst %.src, %.o, $(API_IMPORT_SRC))), $(OBJ_PATH)/$(file))
API_INCDIR  = $(foreach dir,$(DRV_DIR),$(DDIDRV)/$(dir))
API_CC_INCS = $(foreach dir,$(API_INCDIR),-I$(dir) -I$(PUBINC))

ifeq ($(MAKE_TYPE),lint)
ifeq ($(shell uname -o),Cygwin)
LINT_INC   += $(foreach dir,$(shell cygpath -m $(API_INCDIR)),-i"$(dir)")
else
LINT_INC   += $(foreach dir,$(API_INCDIR),-i"$(dir)")
endif
endif

VPATH =

ifeq ("$(UTOPIA_BSP_VERSION)", "")
UTOPIA_BSP_VERSION = 000000
endif

ifeq ("$(P4PORT)", "")
SW_VERSION_API_$(API_NAME) = 0
else
SW_VERSION_API_$(API_NAME) := $(shell p4 changes -m1 "./...\#have" | egrep Change | awk '{print $$2}')
ifeq ("$(SW_VERSION_API_$(API_NAME))", "")
SW_VERSION_API_$(API_NAME) = 0
endif
endif
SW_API_FLOW_ENABLE = TRUE
CC_DEFS += -DSW_VERSION_API_$(API_NAME)=$(SW_VERSION_API_$(API_NAME))

VDEC_USE_NEW_LAYOUT = nike madison miami edison eiffel einstein einstein3 napoli kaiser monaco munich keres kirin clippers muji monet manhattan messi maserati maxim kano curry k6 k6lite k7u M7821 M7621
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_USE_NEW_LAYOUT)))
CC_DEFS += -DSUPPORT_NEW_MEM_LAYOUT
#CC_DEFS += -DSUPPORT_NEW_VDEC_FLOW   # Use old layout but with new share mem handshake flow
endif

#ifeq ($(V3_DECODE),true)
CC_DEFS += -DVDEC3
CC_DEFS += -DVDEC3_FB
#endif

# Note: Use old layout *CANNOT* enable EVD
VDEC_SUPPORT_EVD = einstein napoli madison monaco munich keres kirin clippers muji monet manhattan messi maserati maxim kano curry k6 k6lite k7u M7821 M7621
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_EVD)))
CC_DEFS += -DSUPPORT_EVD=1
else
CC_DEFS += -DSUPPORT_EVD=0
endif

VDEC_SUPPORT_G2VP9 = monaco muji
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_G2VP9)))
CC_DEFS += -DSUPPORT_G2VP9=1
else
CC_DEFS += -DSUPPORT_G2VP9=0
endif

VDEC_SUPPORT_MSVP9 = manhattan messi maserati maxim kano curry k6 k6lite k7u M7821 M7621
ifeq ($(CHIP), $(filter $(CHIP),$(VDEC_SUPPORT_MSVP9)))
CC_DEFS += -DSUPPORT_MSVP9=1
else
CC_DEFS += -DSUPPORT_MSVP9=0
endif

.NOTPARALLEL : hvd_v3 mjpeg_v3 mvd_v3
.PHONY : all clean $(DRV_DIR) depend

all : $(API_OBJ) $(API_IMPORT_OBJ) $(DRV_DIR)

lib : setup all
ifeq ($(OS_TYPE),nos)
endif
ifeq ($(OS_TYPE),linux)
ifeq ($(PLATFORM),android)
endif
endif

ifeq ($(LINK_TYPE),dynamic)
    ifeq ($(CONFIG_INCREMENTAL_BUILD),y)
	@$(CC) $(CFG_CC_OPTS) $(LD_LIBS_INC) $(LD_OPTS) $(wildcard $(OBJ_PATH)/$(LIB_NAME)_T/*.T) $(API_OBJ) $(API_IMPORT_OBJ) $(HAL_IMPORT_OBJ) $(MLOG_LIB_PATH) -o $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) ;
    ifeq ($(CONFIG_VERSION_INFO_EMBADE), y)
	@sh $(VERSION)/versionembadder.sh $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp ./version_info
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	@mv -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
    endif
    else
	@$(CC) $(CFG_CC_OPTS) $(LD_OPTS) $(wildcard $(OBJ_PATH)/$(LIB_NAME)_T/*.T) $(API_OBJ) $(API_IMPORT_OBJ) $(MLOG_LIB_PATH) -o $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) ;
	@$(OBJCOPY) --add-section .mmodule_version=./version_info $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
    ifeq ($(CONFIG_VERSION_INFO_EMBADE), y)
	@sh $(VERSION)/versionembadder.sh $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp ./version_info
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	@mv -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
    endif
    endif
#	@rm -f $(OBJ_PATH)/*.T
endif

ifeq ($(TOOLCHAIN),mips-linux-gnu)
ifeq ($(OS_TYPE),nos)
SW_MBOOT_FLOW = TRUE
endif
endif

$(DRV_DIR) :
ifeq ($(LINK_TYPE),dynamic)
	@$(MAKE) BUILDING_API=1 -s -C $(DDIDRV)/$@
else
	@$(MAKE) -C $(DDIDRV)/$@
endif

$(API_IMPORT_OBJ) :
	echo find api import_obj cp from $(filter %$(notdir $*).src,$(API_IMPORT_SRC)) to $@
	cp $(filter %$(notdir $*).src,$(API_IMPORT_SRC)) $@
ifneq ($(LINK_TYPE),dynamic)
	@$(AR) cru $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $@;
    ifeq ($(CONFIG_VERSION_INFO_EMBADE), y)
	@sh $(VERSION)/versionembadder.sh $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp ./version_info
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	@mv -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
    endif
ifeq ($(BLT_TYPE),retail)
	@$(STRIP) -S $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
endif
endif

$(API_OBJ) : $(OBJ_PATH)/%.o : %.c
ifneq ($(MAKE_TYPE),lint)
	@echo [CC] $(notdir $<)
	@$(CC) $(CC_OPTS) $(CC_DEFS) $(CC_INCS) $(API_CC_INCS) -o $@ $<;
ifneq ($(LINK_TYPE),dynamic)
	@$(AR) cru $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $@;
    ifeq ($(CONFIG_VERSION_INFO_EMBADE), y)
	@sh $(VERSION)/versionembadder.sh $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT) $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp ./version_info
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
	@mv -f $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT).temp $(DDI_PATH)/lib$(LIB_NAME).$(LIB_EXT)
    endif
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

ifeq ($(PARA), 1)
	@if [ ! -f $(CURDIR)/cpptestscan.bdf ]; then                                         \
		echo ERROR! No $(CURDIR)/cpptestscan.bdf;                                    \
	fi
	@if [ -f $(CURDIR)/cpptestscan.bdf ]; then                                           \
		cat $(CURDIR)/cpptestscan.bdf >> $(PROJ)/cpptestscan.bdf;    \
		rm -f $(CURDIR)/cpptestscan.bdf;                                                             \
	fi
endif

setup :
ifeq ($(LINK_TYPE),dynamic)
	@echo [api_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
#	@rm -f $(OBJ_PATH)/*.T
else
	@echo [api_rule.mk][lib$(LIB_NAME).$(LIB_EXT)]
endif

clean :
	@$(foreach dir, $(DRV_DIR), $(MAKE) -s -C $(DDIDRV)/$(dir) clean;)
	@rm -f $(API_OBJ)
	@rm -f $(API_IMPORT_OBJ)
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).a
	@rm -f $(DDI_PATH)/lib$(LIB_NAME).so


