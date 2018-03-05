ifeq ($(PROJ),U3_OBAMA_2_0_UI2)

INSTALL_INC_FILES +=				\
	apiVDEC.h				\

endif

ifeq ($(PROJ),U3_ZUI_MM)

INSTALL_INC_FILES +=				\
	apiVDEC.h		\

endif

INSTALL_INC_FILES +=				\
	apiVDEC.h		\

ifneq ($(LINK_TYPE),dynamic)

INSTALL_LIB_FILES +=			\
	libapiVDEC.a			\

else

INSTALL_LIB_FILES +=			\
	libapiVDEC.so			\

endif



ifeq ($(FW_TYPE),external)

INSTALL_FW_FILES +=				\
	fwRVD.dat				\
	fwRVD.bin				\
	fwRVDvlcmem.dat				\
	fwRVDvlcmem.bin				\
	fwMVD.dat				\
	fwMVD.bin				\

ifeq ($(PRJ),t2_nos)

INSTALL_FW_FILES +=				\
	fwSVD.dat				\
	fwSVD.bin				\

else ifeq ($(PRJ),t2_nos_mips)

INSTALL_FW_FILES +=				\
	fwSVD.dat				\
	fwSVD.bin				\

else

INSTALL_FW_FILES +=				\
	fwHVD.dat				\
	fwHVD.bin				\

endif

endif
