ifeq ($(PROJ),U3_OBAMA_2_0_UI2)

INSTALL_INC_FILES +=				\
	apiJPEG.h				\
	apiMJPEG.h				\

endif

ifeq ($(PROJ),U3_ZUI_MM)

INSTALL_INC_FILES +=				\
	apiJPEG.h				\
	apiMJPEG.h				\

endif

INSTALL_INC_FILES +=				\
	apiJPEG.h				\
	apiMJPEG.h				\

ifneq ($(LINK_TYPE),dynamic)

INSTALL_LIB_FILES +=			\
	libapiJPEG.a			\

else

INSTALL_LIB_FILES +=			\
	libapiJPEG.so			\

endif



ifeq ($(FW_TYPE),external)

INSTALL_FW_FILES +=				

ifeq ($(PRJ),t2_nos)

INSTALL_FW_FILES +=				

else ifeq ($(PRJ),t2_nos_mips)

INSTALL_FW_FILES +=				

else

INSTALL_FW_FILES +=				

endif

endif
