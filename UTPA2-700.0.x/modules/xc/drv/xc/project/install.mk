ifeq ($(PROJ),U3_OBAMA_2_0_UI2)

INSTALL_INC_FILES +=				\
	apiHDMITx.h		\

endif

ifeq ($(PROJ),U3_ZUI_MM)

INSTALL_INC_FILES +=				\
	apiHDMITx.h		\

endif

INSTALL_INC_FILES +=				\
	apiCEC.h		\
	apiPNL.h		\
	apiXC.h			\
	apiXC_PCMonitor.h	\
	apiXC_ModeParse.h	\
	apiXC_Auto.h		\
	apiXC_Adc.h		\
	apiXC_Ace.h		\
	apiXC_Dlc.h		\
	drvDDC2BI.h		\
	drvTVEncoder.h		\
	drvXC_HDMI_if.h		\
	drvXC_IOPort.h		\
	drvMFC.h		\

ifneq ($(LINK_TYPE),dynamic)

INSTALL_LIB_FILES +=			\
	libapiACE.a			\
	libapiCEC.a			\
	libapiDLC.a			\
	libapiPNL.a			\
	libapiXC.a			\
	libdrvDDC2BI.a			\
	libdrvVE.a			\
	libdrvMFC.a			\

else

INSTALL_LIB_FILES +=			\
	libapiACE.so			\
	libapiCEC.so			\
	libapiDLC.so			\
	libapiPNL.so			\
	libapiXC.so			\
	libdrvDDC2BI.so			\
	libdrvVE.so			\

endif
