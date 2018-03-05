#ifndef _FILM_H_
#define _FILM_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "FieldDetection.h"

#define SUPPORT_TILE
void InitFilmDetection(void);
int  setHeight_Width(FODInfo *fod);
int  getTileWidth(void);
int  getTileHeight(void);
void detFieldOrderBot( unsigned char*preField, unsigned char*curField, unsigned char*nxtField, unsigned char*nx2Field, int CurIsBot);
void detFieldOrderTop( unsigned char*preField, unsigned char*curField, unsigned char*nxtField, unsigned char*nx2Field, int CurIsBot);
int detFieldOrderFW(void);
void dumpDetFieldOrder_status(int CurIsBot);
unsigned char* Create16ByteAligned(unsigned char** org, int size);
void copyFrameBuffer(unsigned char *dst, unsigned char *src, int bufsize);
int getFodWinEnable(void);
#ifdef __cplusplus
}
#endif
#endif
