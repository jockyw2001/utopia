#ifndef _FIELDDET_H_
#define _FIELDDET_H_
#ifdef __cplusplus
extern "C" {
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#else
#include <linux/string.h>
#endif
//#define SIMULATE_MEMCPY
#include "MsCommon.h"

//typedef unsigned short MS_U16;
//typedef unsigned int* MS_PHY;
//typedef unsigned int  MS_U32;
//typedef unsigned int  MS_BOOL;

typedef struct stFrameInfo
{
  MS_U16 u16Width;
  MS_U16 u16Height;
  MS_PHY phyBuffAddr;
  MS_U32 u32BuffSize;
  MS_BOOL bFieldOrderSet;     // 0-> bottom first, 1-> top first
} FrameInfo;

typedef struct stFodInfo
{
  int width;
  int height;
  int pitch;
  int tile_width;
  int tile_height;
} FODInfo;

typedef enum EIsFieldOrder
{
  E_VDEC_IFOI_TRUE,
  E_VDEC_IFOI_FALSE,
  E_VDEC_IFOI_Invalid,
} eIsFieldOrderInverse;

#if (!defined(MSOS_TYPE_LINUX_KERNEL)&& !defined(ANDROID))
eIsFieldOrderInverse IsFieldOrderInverse (FrameInfo *stFieldInfo);
int  InitFieldDetection(FODInfo *fod);
void FInitFieldDetection(void);
void InitMemoryFromVPU(MS_PHY phyAddr1, MS_PHY phyAddr2, MS_PHY phyAddr3);
#endif

#ifdef __cplusplus
}
#endif
#endif