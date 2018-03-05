#CC := /tools/arm-2011.09-70-lite/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc
CC := arm-none-linux-gnueabi-gcc
UTOPIA_PATH := /home/bob.fu/PERFORCE/THEALE/utopia_develop/UTPA-14.5.1_utopia2/
MXLIB_PATH := $(UTOPIA_PATH)/mxlib
BSP_LIB_PATH := $(UTOPIA_PATH)/build/bsp/lib
BSP_INCLUDE_PATH := $(UTOPIA_PATH)/build/bsp/include

ALL_INCLUDE := -I$(BSP_INCLUDE_PATH) -DMSOS_TYPE_LINUX \
			   -I$(MXLIB_PATH)/utopia_core/ \
			   -I$(MXLIB_PATH)/drv/bdma/

# libdrvAESDMA.so must precedes liblinux.so
# or verify code would call weak definition liblinux.so
#ALL_LIB := -L$(BSP_LIB_PATH) 
ALL_LIB := -Llib       \
		   -ldrvAESDMA \
		   -ldrvDSCMB  \
		   -lapiGOP    \
		   -lapiGFX    \
		   -ldrvVE     \
		   -lapiXC     \
		   -lapiMBX    \
		   -ldrvTSP    \
		   -lapiPNL    \
		   -ldrvVBI    \
		   -lapiACE    \
		   -lapiDLC    \
		   -ldrvIR     \
		   -ldrvSAR    \
		   -ldrvPWS    \
		   -ldrvSERFLASH \
		   -ldrvSEAL   \
		   -ldrvCMDQ   \
		   -ldrvMMFI   \
		   -ldrvSEM    \
		   -ldrvSYS    \
		   -ldrvBOBBI  \
		   -lMsOS	   \
		   -llinux -pthread -lrt \


#    PREFIX(UTOPIA) \ included in liblinux.so
#    PREFIX(BDMA) \ included in liblinux.so
#    PREFIX(AESDMA) \
#    PREFIX(DSCMB) \
#    PREFIX(CI) \ !!! linking error
#    PREFIX(GOP) \
#    PREFIX(GFX) \
#    PREFIX(TVENCODER) \
#    PREFIX(XC) \ included in libapiXC.so
#    PREFIX(MBX) \
#    PREFIX(TSP) \
#    PREFIX(DIP) \ included in libapiXC.so
#    PREFIX(PNL) \
#    PREFIX(VBI) \ 
#    PREFIX(ACE) \
#    PREFIX(DLC) \
#    PREFIX(IR) \ !!! linking error
#    PREFIX(SAR) \
#    PREFIX(MIU) \ ???
#    PREFIX(PWS) \
#    PREFIX(FLASH) \
#    PREFIX(SEAL) \
#    PREFIX(CMDQ) \ need to manually compile it
#    PREFIX(MMFI) \
#    PREFIX(SEM) \ need to manually compile it
#    PREFIX(SYS) \ need to manually compile it
#    PREFIX(BOBBI) \ 



all: bobbi
	@$(CC) -o verify main.c $(ALL_INCLUDE) $(ALL_LIB)
#@$(CC) -shared -o libutopia.so utopia.c utopia_dapi.c utopia_module.c 
#@arm-none-linux-gnueabi-gcc -shared -o libaesdma.so aesdma.c -nostdlib
#@$(CC) -L. -laesdma -ldrvAESDMA -lpthread -llinux -lrt -o main main.c

bobbi:
	@$(CC) -shared -o libdrvBOBBI.so bobbi.c $(ALL_INCLUDE)
	@mv libdrvBOBBI.so lib

#test:
#	@$(CC) -L. -lpthread -llinux -lrt -o test1 test1.c
#	@$(CC) -L. -lpthread -llinux -lrt -o test2 test2.c
#
clean:
	@rm -rf verify libdrvBOBBI.so
