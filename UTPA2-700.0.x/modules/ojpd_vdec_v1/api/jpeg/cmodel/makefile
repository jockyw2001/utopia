JPEG_SRC    =   ./src/main.c            \
                ./src/idct.c            \
                ./src/h2v2.c            \
                ./src/jpeg_cmodel.c     \
                ./src/jpeg_cmodel_io.c  \
                ../apiJPEG.c            \
                ../jpeg_memory.c

JPEG_INC    =   -I./inc       \
                -I../

JPEG_DEF    =   -DCMODEL

all:
	gcc -Wall -o a.exe $(JPEG_DEF) $(JPEG_SRC) $(JPEG_INC)

clean:
	@rm a.exe