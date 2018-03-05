
TOOLCHAIN   = mips-linux-gnu-

ENDIAN      = EL

INCLUDE_DIR = -I../../../RedLion/2.6.28.9/include/ 

INCLUDE_DIR += -I../../../RedLion/2.6.28.9/drivers/mstar/include/


CC_OPTS     = -mips32 -$(ENDIAN) $(INCLUDE_DIR) -o

CC          = $(TOOLCHAIN)gcc

SRC_FILE    = spi_load.c main.c

BIN_FILE    = gr

.PHONY : all clean

all : $(SRC_FILE)
	$(CC)  $(CC_OPTS)  $(BIN_FILE) $(SRC_FILE)  

