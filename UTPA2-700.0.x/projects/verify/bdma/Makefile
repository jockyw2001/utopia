#CC := /tools/arm-2011.09-70-lite/Sourcery_CodeBench_Lite_for_ARM_GNU_Linux/bin/arm-none-linux-gnueabi-gcc
CC := arm-none-linux-gnueabi-gcc
UTOPIA_PATH := /home/bob.fu/PERFORCE/THEALE/utopia_develop/UTPA-14.5.1_utopia2/
MXLIB_PATH := $(UTOPIA_PATH)/mxlib
BSP_LIB_PATH := $(UTOPIA_PATH)/build/bsp/lib
BSP_INCLUDE_PATH := $(UTOPIA_PATH)/build/bsp/include

all:
	@$(CC) -o verify main.c  -I$(BSP_INCLUDE_PATH) -I$(MXLIB_PATH)/utopia_core/linux -DMSOS_TYPE_LINUX -I$(MXLIB_PATH)/drv/bdma/ \
	-L$(BSP_LIB_PATH) -llinux -pthread -lrt
#@$(CC) -shared -o libutopia.so utopia.c utopia_dapi.c utopia_module.c 
#@arm-none-linux-gnueabi-gcc -shared -o libaesdma.so aesdma.c -nostdlib
#@$(CC) -L. -laesdma -ldrvAESDMA -lpthread -llinux -lrt -o main main.c

#test:
#	@$(CC) -L. -lpthread -llinux -lrt -o test1 test1.c
#	@$(CC) -L. -lpthread -llinux -lrt -o test2 test2.c
#
#clean:
#	@rm -rf libutopia.so libaesdma.so main
