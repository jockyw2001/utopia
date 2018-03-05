ifeq ($(PROJ),U3_OBAMA_2_0_UI2)

INSTALL_INC_FILES +=				\


endif


ifeq ($(PROJ),U3_ZUI_MM)

INSTALL_INC_FILES +=				\


endif


INSTALL_INC_FILES +=				\
	drvMVOP.h		                \



ifneq ($(LINK_TYPE),dynamic)

INSTALL_LIB_FILES +=			    \
	libdrvMVOP.a			        \

else

INSTALL_LIB_FILES +=			    \
	libdrvMVOP.so			        \

endif
