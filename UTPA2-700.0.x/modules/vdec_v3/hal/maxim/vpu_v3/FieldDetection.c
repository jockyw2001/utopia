#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#else
#include <linux/string.h>
#endif
#include "Film.h"
#include "FieldDetection.h"
//#define SIMULATE_MEMCPY
#define REMOVE_Duplicate_Detect /*Enable: it is the original code flow  */
#define Fix_CropRegion
//#define CONSOLE_DBG
#define RING_BUFF_SIZE 3

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE) && !defined(MSOS_TYPE_LINUX_KERNEL)&& !defined(ANDROID)) || defined(SUPPORT_X_MODEL_FEATURE)

#if 0
static unsigned char* g_ringbuf[RING_BUFF_SIZE];
static unsigned char* g_curTop = NULL;
static unsigned char* g_curBot = NULL;
static unsigned char* g_nxtTop = NULL;
static unsigned char* g_nxtBot = NULL;
static unsigned char* g_nxt2Top = NULL;
static unsigned char* g_nxt2Bot = NULL;
#endif

static unsigned char* g_16_ringbuf[RING_BUFF_SIZE];
static unsigned char* g_16_curTop = NULL;
static unsigned char* g_16_curBot = NULL;
static unsigned char* g_16_nxtTop = NULL;
static unsigned char* g_16_nxtBot = NULL;
static unsigned char* g_16_nxt2Top = NULL;
static unsigned char* g_16_nxt2Bot = NULL;
#ifdef SIMULATE_MEMCPY
#ifdef __arm__
static unsigned char* g_sim_buf    = NULL;
static unsigned char* g_16_sim_buf = NULL;

static unsigned char* g_sim_crop_buf    = NULL;
static unsigned char* g_16_sim_crop_buf = NULL;

#endif
#endif

static int bInit = 0;
static int g_bufidx = 0;
static int g_prvFieldOrderSet = -1; //0-> bottom first, 1-> top first, -1: initial

static unsigned char * AsKOneBuffer(void)
{
  unsigned char *tmp = g_16_ringbuf[g_bufidx];
  g_bufidx++;
  g_bufidx = g_bufidx % RING_BUFF_SIZE;
  return tmp;
}

#if 0
static void ReleaseMem (unsigned char* pbuf)
{
  if (pbuf != NULL)
    free(pbuf);
}
#endif
static void ReleaseBuffer(void)
{
#if 0
  ReleaseMem(g_ringbuf[0]);
  ReleaseMem(g_ringbuf[1]);
  ReleaseMem(g_ringbuf[2]);
#endif
#ifdef SIMULATE_MEMCPY
#ifdef __arm__
  ReleaseMem(g_sim_buf);
  ReleaseMem(g_sim_crop_buf);
#endif
#endif
}


void InitMemoryFromVPU(MS_PHY phyAddr1, MS_PHY phyAddr2, MS_PHY phyAddr3)
{
    g_16_ringbuf[0] = (unsigned char*)MsOS_PA2KSEG0(phyAddr1);
    g_16_ringbuf[1] = (unsigned char*)MsOS_PA2KSEG0(phyAddr2);
    g_16_ringbuf[2] = (unsigned char*)MsOS_PA2KSEG0(phyAddr3);
}

static void InitMemory(int width, int height, int tile_height, int tile_width)
{
//  int remain_h = (height % tile_height);
//  int remain_w = (width % tile_width);
//  int pad_w    = (remain_w != 0) ? tile_width - remain_w : 0;
//  int pad_h    = (remain_h != 0) ? tile_height - remain_h : 0;
  if (bInit == 1) {
    ReleaseBuffer();
  }

#if 0
#if 1
    g_16_ringbuf[0] = MsOS_PA2KSEG0(0x84600000);
    g_16_ringbuf[1] = MsOS_PA2KSEG0(0x85600000);
    g_16_ringbuf[2] = MsOS_PA2KSEG0(0x86600000);
//    g_16_ringbuf[0] = (0x84600000);
//    g_16_ringbuf[1] = (0x85600000);
//    g_16_ringbuf[2] = (0x86600000);
#else
#ifdef SUPPORT_TILE
  g_16_ringbuf[0] = Create16ByteAligned(&g_ringbuf[0], (width + pad_w) * (height + pad_h));
  g_16_ringbuf[1] = Create16ByteAligned(&g_ringbuf[1], (width + pad_w) * (height + pad_h));
  g_16_ringbuf[2] = Create16ByteAligned(&g_ringbuf[2], (width + pad_w) * (height + pad_h));
#else
  g_16_ringbuf[0] = Create16ByteAligned(&g_ringbuf[0], width * height);
  g_16_ringbuf[1] = Create16ByteAligned(&g_ringbuf[1], width * height);
  g_16_ringbuf[2] = Create16ByteAligned(&g_ringbuf[2], width * height);
#endif
#endif
#endif

}

static int RunDI( MS_BOOL field_order_set,
                  unsigned char *curFieldTop,
                  unsigned char *curFieldBot,
                  unsigned char *nxtFieldTop,
                  unsigned char *nxtFieldBot,
                  unsigned char *nx2FieldTop,
                  unsigned char *nx2FieldBot)
{
  int field_inv;
#ifndef REMOVE_Duplicate_Detect
  if(field_order_set == 1)//top field first
  {
    detFieldOrderTop(curFieldBot, nxtFieldTop, nxtFieldBot, nx2FieldTop, 0);
    dumpDetFieldOrder_status(0);
  }
  else//bot field first
  {
    detFieldOrderBot(curFieldTop, nxtFieldBot, nxtFieldTop, nx2FieldBot, 1);
    dumpDetFieldOrder_status(1);
  }
#endif
  if(field_order_set == 1)//top field first
  {
    detFieldOrderBot(nxtFieldTop, nxtFieldBot, nx2FieldTop, nx2FieldBot, 1);
    dumpDetFieldOrder_status(1);
    field_inv = detFieldOrderFW();
  }
  else//bot field first
  {
    detFieldOrderTop(nxtFieldBot, nxtFieldTop, nx2FieldBot, nx2FieldTop, 0);
    dumpDetFieldOrder_status(0);
    field_inv = detFieldOrderFW();
  }
#ifdef FILE_DBG
  {
    FILE *fptr = fopen("/usb/sda1/field-firmware.txt","a");
    fprintf(fptr, "%d\n", field_inv);
    fclose(fptr);
  }
#endif
#ifdef CONSOLE_DBG
  printf("[FOD-inv] ret=%d\n", field_inv);
#endif
  return field_inv;
}

eIsFieldOrderInverse IsFieldOrderInverse (FrameInfo *stFieldInfo)
{

  unsigned char *p_curbuff = AsKOneBuffer();
  unsigned char *p_curTop, *p_curBot;
  int field_order_set;
  eIsFieldOrderInverse bRet = E_VDEC_IFOI_FALSE;
  int isInv;
  int Fod_win_enable = getFodWinEnable();
#ifdef Fix_CropRegion
  if (Fod_win_enable == 1) {
    field_order_set = (g_prvFieldOrderSet != -1) ? g_prvFieldOrderSet : stFieldInfo->bFieldOrderSet;
  }
  else
#endif
  {
    field_order_set = stFieldInfo->bFieldOrderSet;
  }

  copyFrameBuffer(p_curbuff, (unsigned char *) MsOS_PA2KSEG0(stFieldInfo->phyBuffAddr), stFieldInfo->u32BuffSize);
  if (bInit == 0) {
    p_curTop = p_curbuff;
#ifdef SUPPORT_TILE
    p_curBot = p_curbuff + getTileWidth();
#else
    p_curBot = p_curbuff + stFieldInfo->u16Width;
#endif

    g_16_nxtTop = g_16_curTop = p_curTop;
    g_16_nxtBot = g_16_curBot = p_curBot;
    bInit = 1;
    return E_VDEC_IFOI_FALSE;
  }
  g_16_nxt2Top = p_curbuff;
#ifdef SUPPORT_TILE
  g_16_nxt2Bot = p_curbuff + getTileWidth();
#else
  g_16_nxt2Bot = p_curbuff + stFieldInfo->u16Width;
#endif

  isInv = RunDI(field_order_set,
                g_16_curTop,
                g_16_curBot,
                g_16_nxtTop,
                g_16_nxtBot,
                g_16_nxt2Top,
                g_16_nxt2Bot);

  g_16_curTop = g_16_nxtTop;
  g_16_curBot = g_16_nxtBot;
  g_16_nxtTop = g_16_nxt2Top;
  g_16_nxtBot = g_16_nxt2Bot;

#ifdef Fix_CropRegion
  if (Fod_win_enable == 1) {
    g_prvFieldOrderSet = (isInv == 1) ? (1 - field_order_set) : field_order_set;
    if (g_prvFieldOrderSet != stFieldInfo->bFieldOrderSet) {
      bRet = E_VDEC_IFOI_TRUE;
    }
  } else {
    if (isInv == 1) {
      bRet = E_VDEC_IFOI_TRUE;
    }
  }
#else
  if (isInv == 1) {
    bRet = E_VDEC_IFOI_TRUE;
  }
#endif
#ifdef CONSOLE_DBG
  if (bRet == E_VDEC_IFOI_TRUE)
    printf("[FOD] field_inv 1\n");
  else
    printf("[FOD] field_inv 0\n");
#endif
  return bRet;
}

int InitFieldDetection(FODInfo *fod)
{
  bInit = 0;
  g_bufidx = 0;
  g_prvFieldOrderSet = -1;
  InitFilmDetection();
  InitMemory(fod->width, fod->height, fod->tile_height, fod->tile_width);
  return setHeight_Width(fod);
}

void FInitFieldDetection(void)
{
  bInit = 0;
  g_bufidx = 0;
  g_prvFieldOrderSet = -1;
  ReleaseBuffer();
}
#endif