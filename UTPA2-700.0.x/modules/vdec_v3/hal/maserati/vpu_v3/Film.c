#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#endif

#include "Film.h"
#include "drvBDMA.h"
#define USE_NEON
#define REMOVE_T0B0
//#define REMOVE_T1B1
#define ONLY_Detect_Crop_Region
//W  x H                (x_start, x_end) (y_start, y_end)
//720x576   -> window = (  0,  720 ) x (  0, 576 )
//1920x1080 -> window = (288, 1632 ) x (192, 896 )
//#define FILE_DBG

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE) && !defined(MSOS_TYPE_LINUX_KERNEL) && !defined(ANDROID)) || defined(SUPPORT_X_MODEL_FEATURE)

#ifdef USE_NEON
#ifdef __arm__
#include <arm_neon.h>
#else
#include "NEON_2_SSE.h"
#endif
#endif

static int g_Height = 0;
static int g_Width = 0;
static int g_Height_remain = 0;
static int g_Width_remain  = 0;
static int g_pitch         = 0;
static int g_tile_width    = 0;
static int g_tile_height   = 0;
static int g_tile_w_mod    = 0;
static int g_tile_w_shift  = 0;
static int g_fod_win_en    = 0;
static int g_base_pos      = 0;
static int g_sceneChangeTh = 800000;

#define X_START 288
#define X_END   1632
#define Y_START 192
#define Y_END   896

/*MPEG-2 tile:
16x32, stripe align 16bytes
32x32, stripe align 32bytes*/
/*
AVS/AVS+:
tile16x32, stripe align 16bytes.
*/

//tile width is 16 or 32
#define TILE_Width1 16
#define TILE_Width2 32
#define W_shift1    4
#define W_shift2    5
#define W_Mod1      ((1<<W_shift1)- 1)
#define W_Mod2      ((1<<W_shift2)- 1)

//tile height is 32
#define TILE_Height 32
#define H_shift     5
#define H_Mod       ((1<<H_shift)- 1)


#define det_length 4
#define lowTh  196
#define highTh 320
#define MAXFRAME (det_length + 1) /* on purpose */

static int g_ary_idx = -1;
static int g_bInitPhase = 1;
static int ary_frame_motion_ratio[MAXFRAME];
static int ary_IsSceneChangeTh[MAXFRAME];

static int frame_motion_T1B0 = 0;
static int frame_motion_T0B1 = 0;
static int frame_motion_T0B0 = 0;
static int frame_motion_T1B1 = 0;

static int reg_fod_mot_diff_th = 0;
static int reg_fod_med_filter_th = 0;
static int reg_fod_mot_diff_fm_th = 0;
static int reg_fod_med_filter_fm_th = 0;
#define OneFrameStep 96
#define minmax(v,a,b)       (((v)<(a))? (a) : ((v)>(b)) ? (b) : (v))
//#define DEBUG_INFO
#ifdef DEBUG_INFO
static unsigned long g_checksum = 0;
#define BASE 65521      /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {adler += (buf)[i]; sum2 += adler;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

#  define MOD(a) a %= BASE
#  define MOD28(a) a %= BASE
#  define MOD63(a) a %= BASE


/* ========================================================================= */
unsigned long adler32( unsigned long adler, const unsigned char *buf, unsigned int len )
{
  unsigned long sum2;
  unsigned n;

  /* split Adler-32 into component sums */
  sum2 = (adler >> 16) & 0xffff;
  adler &= 0xffff;

  /* in case user likes doing a byte at a time, keep it fast */
  if (len == 1) {
    adler += buf[0];
    if (adler >= BASE)
      adler -= BASE;
    sum2 += adler;
    if (sum2 >= BASE)
      sum2 -= BASE;
    return adler | (sum2 << 16);
  }

  /* initial Adler-32 value (deferred check for len == 1 speed) */
  if (buf == 0)
    return 1L;

  /* in case short lengths are provided, keep it somewhat fast */
  if (len < 16) {
    while (len--) {
      adler += *buf++;
      sum2 += adler;
    }
    if (adler >= BASE)
      adler -= BASE;
    MOD28(sum2);            /* only added so many BASE's */
    return adler | (sum2 << 16);
  }

  /* do length NMAX blocks -- requires just one modulo operation */
  while (len >= NMAX) {
    len -= NMAX;
    n = NMAX / 16;          /* NMAX is divisible by 16 */
    do {
      DO16(buf);          /* 16 sums unrolled */
      buf += 16;
    } while (--n);
    MOD(adler);
    MOD(sum2);
  }

  /* do remaining bytes (less than NMAX, still just one modulo) */
  if (len) {                  /* avoid modulos if none remaining */
    while (len >= 16) {
      len -= 16;
      DO16(buf);
      buf += 16;
    }
    while (len--) {
      adler += *buf++;
      sum2 += adler;
    }
    MOD(adler);
    MOD(sum2);
  }

  /* return recombined sums */
  return adler | (sum2 << 16);
}
#endif

void InitFilmDetection(void)
{
  g_ary_idx = -1;
  g_bInitPhase = 1;
  memset((uint8_t*) ary_frame_motion_ratio, 0, sizeof(int)*MAXFRAME);
  memset((uint8_t*) ary_IsSceneChangeTh, 0, sizeof(int)*MAXFRAME);
  frame_motion_T1B0 = 0;
  frame_motion_T0B1 = 0;
  frame_motion_T0B0 = 0;
  frame_motion_T1B1 = 0;

  reg_fod_mot_diff_th = 0;
  reg_fod_med_filter_th = 0;
  reg_fod_mot_diff_fm_th = 0;
  reg_fod_med_filter_fm_th = 0;
  g_Height = g_Width = g_Height_remain = g_Width_remain = 0;
  g_pitch  = g_tile_width = g_tile_height = g_tile_w_mod = 0;
  g_tile_w_shift = g_fod_win_en = 0;
  g_sceneChangeTh = 800000;
  g_base_pos = 0;
}

extern void _MApi_PrintMem(MS_VIRT u32Addr, MS_U32 u32Size);

void copyFrameBuffer(unsigned char *dst, unsigned char *src, int bufsize)
{
#ifdef DEBUG_INFO
  unsigned long curimg = 0;
  g_checksum = adler32(g_checksum, src, 0x100);
  curimg = adler32(curimg, src, 0x100);
  printf("[prv-chk=0x%lx][cur-chk=0x%lx][size=0x100][osize=%d]\n", g_checksum, curimg, bufsize);

//  unsigned long curimg = 0;
//  g_checksum = adler32(g_checksum, src, bufsize);
//  curimg = adler32(curimg, src, bufsize);
//  printf("[prv-chk=0x%lx][cur-chk=0x%lx][size=%d]\n", g_checksum, curimg, bufsize);
#endif
//                MS_U32 u32Timer= MsOS_GetSystemTime();

#ifdef SUPPORT_TILE
#ifdef ONLY_Detect_Crop_Region
  if (g_fod_win_en == 1) {
     int base_pos = g_base_pos;
     int y;
     for( y = Y_START ; y < Y_END ; y += TILE_Height ) {
       //memcpy(dst+base_pos, src+base_pos, (TILE_Height * (X_END - X_START)));
       MDrv_BDMA_MemCopy(MsOS_VA2PA((MS_VIRT)(src+base_pos)), MsOS_VA2PA((MS_VIRT)(dst+base_pos)), (TILE_Height * (X_END - X_START)));

       base_pos += (TILE_Height * g_pitch);
     }
  } else
#endif
#endif
  {
//    memcpy(dst, src, bufsize);
       MDrv_BDMA_MemCopy(MsOS_VA2PA((MS_VIRT)src), MsOS_VA2PA((MS_VIRT)dst), bufsize);
  }
//                printf("cp:[%dms]\n", MsOS_GetSystemTime()-u32Timer);

}

int setHeight_Width(FODInfo *fod)
{
  int retval = 0;
  int pad_w;
  g_Height = fod->height;
  g_Width  = fod->width;
  g_Height_remain = (g_Height % fod->tile_height);
  g_tile_width  = fod->tile_width;
  g_tile_height = fod->tile_height;

  if (g_tile_height != TILE_Height) {
    printf("[FOD][Warning] TILE Height is invalid\n");
    g_tile_height = TILE_Height;
    retval = 1;
  }
  if (g_tile_width == TILE_Width1) {
    g_tile_w_mod = W_Mod1;
    g_tile_w_shift = W_shift1;
  } else if (g_tile_width == TILE_Width2) {
    g_tile_w_mod = W_Mod2;
    g_tile_w_shift = W_shift2;
  } else {
    printf("[FOD][Warning] TILE Width is invalid\n");
    g_tile_w_mod = W_Mod1;
    g_tile_width = TILE_Width1;
    g_tile_w_shift = W_shift1;
    retval = 1;
  }
  g_Width_remain  = (g_Width  % g_tile_width);
  pad_w = (g_Width_remain != 0) ? g_tile_width - g_Width_remain : 0;
  g_pitch = g_Width + pad_w;

#ifdef ONLY_Detect_Crop_Region
  if (g_Height >= 1080 && g_Width >= 1920) {
    int base_y = g_tile_width*(Y_START & H_Mod) + (Y_START  >> H_shift) * (TILE_Height * g_pitch);
    int base_x =  (X_START >> g_tile_w_shift) * (g_tile_width * TILE_Height);
    int x_pos  =  (X_START & g_tile_w_mod) + base_x;
    g_fod_win_en = 1;
    g_base_pos = base_y + x_pos;
    g_sceneChangeTh = 200000;

  } else
#endif
  {
    g_fod_win_en = 0;
    g_sceneChangeTh = 800000;
  }

  if (g_pitch != fod->pitch) {
    printf("[FOD][Warning] pitch mismatch: h=%d/w=%d/p=%d/Tw=%d/Th=%d\n",
            g_Height,
            g_Width,
            g_pitch,
            fod->tile_width,
            fod->tile_height);
    retval = 1;
  }
  return retval;
}

int  getTileWidth(void)
{
  return g_tile_width;
}
int  getTileHeight(void)
{
  return g_tile_height;
}

int getFodWinEnable(void)
{
  return g_fod_win_en;
}

int detFieldOrderFW(void)
{
  int inv_det = 1;
  int ratio_sum2 = 0;
  int i;

  if ( g_bInitPhase == 1 && g_ary_idx < det_length) {
    return 0;
  }
  g_bInitPhase = 0;

    for (i = 0; i < det_length; i++)
    {
      int ary_idx = ((g_ary_idx-i) + MAXFRAME) % MAXFRAME;

      if(ary_IsSceneChangeTh[ary_idx] == 1) {
#ifdef DEBUG_INFO
        printf("[FOD] ret=0 scene changed\n");
#endif
        return 0;
      }

      ratio_sum2 += minmax( (256 - ary_frame_motion_ratio[ary_idx]), 0, OneFrameStep);
     }

     ratio_sum2 = minmax( ratio_sum2, 0, 255 );

    if ( ratio_sum2 > lowTh ) {
      inv_det = 1;
    } else {
      inv_det = 0;
    }
#ifdef DEBUG_INFO
    printf("[FOD] ret=%d\n", inv_det);
#endif
    return inv_det;
  }


#ifdef USE_NEON
static uint8x8_t vmed3_i8 (uint8x8_t A, uint8x8_t B, uint8x8_t C)
{
  return vmax_u8(vmin_u8(A, B), vmin_u8(vmax_u8(A, B), C));
}

uint8_t* Create16ByteAligned(uint8_t** org, int size)
{
    *org = (uint8_t *) malloc(size+15);
    return (uint8_t *) (((uintptr_t)*org+15) & ~ (uintptr_t)0x0F);
}
#ifdef FILE_DBG2
static inline void myprintf(uint8x8_t rst, int x, int y, char* myname)
{
  FILE *fptr = fopen("debug.txt","a");
  fprintf(fptr, "%s[x=%d, y=%d] %x %x %x %x %x %x %x %x\n",
           myname, x, y,
           vget_lane_u8(rst, 0),
           vget_lane_u8(rst, 1),
           vget_lane_u8(rst, 2),
           vget_lane_u8(rst, 3),
           vget_lane_u8(rst, 4),
           vget_lane_u8(rst, 5),
           vget_lane_u8(rst, 6),
           vget_lane_u8(rst, 7));
  fclose(fptr);
}
#endif

void detFieldOrderBot( unsigned char*preField, unsigned char*curField, unsigned char*nxtField, unsigned char*nx2Field, int CurIsBot)
{
  uint8x8_t mot_diff_th      = vdup_n_u8(2);    //o_reg_fod_mot_diff_th 2
  uint8x8_t med_filter_th    = vdup_n_u8(2);    //o_reg_fod_med_filter_th 2
  uint8x8_t mot_diff_fm_th   = vdup_n_u8(2);    //o_reg_fod_mot_diff_fm_th 2
  uint8x8_t med_filter_fm_th = vdup_n_u8(2);    //o_reg_fod_med_filter_fm_th 2


  uint8x8_t filter1, filter2, diff2;
  uint8x8_t filter4;
  uint8x8_t rst;
#ifndef REMOVE_T0B0
  uint8x8_t filter3;
#endif

  int yu, yd, y;

  int x_start = (g_fod_win_en == 1) ? X_START : 0;
  int x_end = (g_fod_win_en == 1) ? X_END : g_Width;
  int y_start = (g_fod_win_en == 1) ? Y_START : 0;
  int y_end = (g_fod_win_en == 1) ? Y_END : g_Height;
#ifdef SUPPORT_TILE
  int stride = g_tile_width;
  int base_x = 0;
#else
  int stride = g_Width;
#endif

  frame_motion_T1B0 = 0;
  frame_motion_T0B1 = 0;
#ifndef REMOVE_T0B0
  frame_motion_T0B0 = 0; //keep T1B1, remove T0B0
#endif
  frame_motion_T1B1 = 0;

  for( y = y_start ; y < y_end ; y+=2 )
  {
    int base_yu, base_y, base_yd, x;
    yu = y - 2;
    if (yu <= y_start)
      yu = y;

    yd = y + 2;
    if (yd >= y_end)
      yd = y;


#ifdef SUPPORT_TILE
    base_yu = stride*(yu & H_Mod) + (yu >> H_shift) * (TILE_Height * g_pitch);
    base_y  = stride*(y  & H_Mod) + (y  >> H_shift) * (TILE_Height * g_pitch);
    base_yd = stride*(yd & H_Mod)+  (yd >> H_shift) * (TILE_Height * g_pitch);
#else
    base_yu = stride*yu;
    base_y  = stride*y;
    base_yd = stride*yd;
#endif

    for( x = x_start ; x < x_end; x+=8 )
    {
      uint8x8_t f0_y0, f1_y0, f2_y0, f3_y0;
      uint8x8_t f0_y1, f2_y1;
      uint8x8_t r_filter1, r_filter2, r_filter4;
      uint8x8_t r_diff2;
      int x_pos;
#ifndef REMOVE_T0B0
      uint8x8_t  r_filter3;
#endif
#ifdef SUPPORT_TILE
      base_x = (x >> g_tile_w_shift) * (g_tile_width * TILE_Height);
      x_pos =  (x & g_tile_w_mod) + base_x;
#else
      x_pos = x;
#endif

      /*
      int test_f1_yu = curField.Y(x, yu)>>5;
      int test_f3_yu = nx2Field.Y(x, yu)>>5;
      int test_f0_y0 = preField.Y(x, y)>>5;
      int test_f1_y0 = curField.Y(x, y)>>5;
      int test_f2_y0 = nxtField.Y(x, y)>>5;
      int test_f3_y0 = nx2Field.Y(x, y)>>5;
      int test_f0_y1 = preField.Y(x, yd)>>5;
      int test_f2_y1 = nxtField.Y(x, yd)>>5;
      */

      f0_y0 = vshr_n_u8(vld1_u8(preField + base_y + x_pos ), 3);
      f1_y0 = vshr_n_u8(vld1_u8(curField + base_y + x_pos ), 3);
      f2_y0 = vshr_n_u8(vld1_u8(nxtField + base_y + x_pos ), 3);
      f3_y0 = vshr_n_u8(vld1_u8(nx2Field + base_y + x_pos ), 3);
      f0_y1 = vshr_n_u8(vld1_u8(preField + base_yd + x_pos), 3);
      f2_y1 = vshr_n_u8(vld1_u8(nxtField + base_yd + x_pos), 3);
      //myprintf(f0_y0, x, y, "f0_y0");

      /*
      f1_yu = vld1_u8(p_curField+base_yu);
      f3_yu = vld1_u8(p_nx2Field+base_yu);
      f0_y0 = vld1_u8(p_preField+base_y );
      f1_y0 = vld1_u8(p_curField+base_y );
      f2_y0 = vld1_u8(p_nxtField+base_y );
      f3_y0 = vld1_u8(p_nx2Field+base_y );
      f0_y1 = vld1_u8(p_preField+base_yd);
      f2_y1 = vld1_u8(p_nxtField+base_yd);

      f1_yu = vshr_n_u8(f1_yu, 5);
      f3_yu = vshr_n_u8(f3_yu, 5);
      f0_y0 = vshr_n_u8(f0_y0, 5);
      f1_y0 = vshr_n_u8(f1_y0, 5);
      f2_y0 = vshr_n_u8(f2_y0, 5);
      f3_y0 = vshr_n_u8(f3_y0, 5);
      f0_y1 = vshr_n_u8(f0_y1, 5);
      f2_y1 = vshr_n_u8(f2_y1, 5);*/


      /*
      diff1 = abs(f0_y0 - f2_y0);
      if (diff1 > mot_diff_th)
      {
      filter1 = abs(med3(f1_yu, f2_y0, f1_y0) - f2_y0);
      if ( (filter1 > med_filter_th) )
      frame_motion_T1B0++;

      filter2 = abs(med3(f3_yu, f0_y0, f3_y0) - f0_y0);
      if ( (filter2 > med_filter_th) )
      frame_motion_T0B1++;
      }
      */

      //if(realline_cycl==1)
      /*
      diff2 = abs(f1_y0 - f3_y0);
      if (diff2 > mot_diff_th)
      {
        filter1 = abs(med3(f2_y0, f1_y0, f2_y1) - f1_y0);
        if ( (filter1 > med_filter_th) )
          frame_motion_T1B0++;

        filter2 = abs(med3(f0_y0, f3_y0, f0_y1) - f3_y0);
        if ( (filter2 > med_filter_th) )
          frame_motion_T0B1++;
      }*/

      diff2   = vabd_u8(f1_y0, f3_y0);                                   //diff2 = abs(f1_y0 - f3_y0);
      r_diff2 = vcgt_u8(diff2, mot_diff_th);                             //if (diff2 > mot_diff_th)
      filter1 = vabd_u8(vmed3_i8(f2_y0, f1_y0, f2_y1), f1_y0);           //filter1 = abs(med3(f2_y0, f1_y0, f2_y1) - f1_y0);
      r_filter1 = vcgt_u8(filter1, med_filter_th);                       //if ( (filter1 > med_filter_th) )
      rst = vtst_u8(r_diff2, r_filter1);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T1B0  += vget_lane_u8(rst, 0);               //frame_motion_T1B0++;
      frame_motion_T1B0  += vget_lane_u8(rst, 1);
      frame_motion_T1B0  += vget_lane_u8(rst, 2);
      frame_motion_T1B0  += vget_lane_u8(rst, 3);
      frame_motion_T1B0  += vget_lane_u8(rst, 4);
      frame_motion_T1B0  += vget_lane_u8(rst, 5);
      frame_motion_T1B0  += vget_lane_u8(rst, 6);
      frame_motion_T1B0  += vget_lane_u8(rst, 7);

      filter2 = vabd_u8(vmed3_i8(f0_y0, f3_y0, f0_y1), f3_y0);           //filter2 = abs(med3(f0_y0, f3_y0, f0_y1) - f3_y0);
      r_filter2 = vcgt_u8(filter2, med_filter_th);                       //if ( (filter2 > med_filter_th) )
      rst = vtst_u8(r_diff2, r_filter2);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T0B1  += vget_lane_u8(rst, 0);               //frame_motion_T0B1++;
      frame_motion_T0B1  += vget_lane_u8(rst, 1);
      frame_motion_T0B1  += vget_lane_u8(rst, 2);
      frame_motion_T0B1  += vget_lane_u8(rst, 3);
      frame_motion_T0B1  += vget_lane_u8(rst, 4);
      frame_motion_T0B1  += vget_lane_u8(rst, 5);
      frame_motion_T0B1  += vget_lane_u8(rst, 6);
      frame_motion_T0B1  += vget_lane_u8(rst, 7);
      /*
      if (diff2 > mot_diff_fm_th)
      {
        filter3 = abs(med3(f0_y0, f1_y0, f0_y1) - f1_y0);
        if ( (filter3 > med_filter_fm_th) )
          frame_motion_T0B0++;

        filter4 = abs(med3(f2_y0, f3_y0, f2_y1) - f3_y0);
        if ( (filter4 > med_filter_fm_th) )
          frame_motion_T1B1++;
      }*/
      r_diff2 = vcgt_u8(diff2, mot_diff_fm_th);                          //if (diff2 > mot_diff_fm_th)
#ifndef REMOVE_T0B0
      filter3 = vabd_u8(vmed3_i8(f0_y0, f1_y0, f0_y1), f1_y0);           //filter3 = abs(med3(f0_y0, f1_y0, f0_y1) - f1_y0);
      r_filter3 = vcgt_u8(filter3, med_filter_fm_th);                    //if ( (filter3 > med_filter_fm_th) )
      rst = vtst_u8(r_diff2, r_filter3);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T0B0  += vget_lane_u8(rst, 0);               //frame_motion_T0B0++;
      frame_motion_T0B0  += vget_lane_u8(rst, 1);
      frame_motion_T0B0  += vget_lane_u8(rst, 2);
      frame_motion_T0B0  += vget_lane_u8(rst, 3);
      frame_motion_T0B0  += vget_lane_u8(rst, 4);
      frame_motion_T0B0  += vget_lane_u8(rst, 5);
      frame_motion_T0B0  += vget_lane_u8(rst, 6);
      frame_motion_T0B0  += vget_lane_u8(rst, 7);
#endif
#ifndef REMOVE_T1B1
      filter4 = vabd_u8(vmed3_i8(f2_y0, f3_y0, f2_y1), f3_y0);           //filter4 = abs(med3(f2_y0, f3_y0, f2_y1) - f3_y0);
      r_filter4 = vcgt_u8(filter4, med_filter_fm_th);                    //if ( (filter4 > med_filter_fm_th) )
      rst = vtst_u8(r_diff2, r_filter4);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T1B1  += vget_lane_u8(rst, 0);               //frame_motion_T1B1++;
      frame_motion_T1B1  += vget_lane_u8(rst, 1);
      frame_motion_T1B1  += vget_lane_u8(rst, 2);
      frame_motion_T1B1  += vget_lane_u8(rst, 3);
      frame_motion_T1B1  += vget_lane_u8(rst, 4);
      frame_motion_T1B1  += vget_lane_u8(rst, 5);
      frame_motion_T1B1  += vget_lane_u8(rst, 6);
      frame_motion_T1B1  += vget_lane_u8(rst, 7);
#endif
    }
  }
}
void detFieldOrderTop( unsigned char*preField, unsigned char*curField, unsigned char*nxtField, unsigned char*nx2Field, int CurIsBot )
{
  uint8x8_t mot_diff_th      = vdup_n_u8(2);    //o_reg_fod_mot_diff_th 2
  uint8x8_t med_filter_th    = vdup_n_u8(2);    //o_reg_fod_med_filter_th 2
  uint8x8_t mot_diff_fm_th   = vdup_n_u8(2);    //o_reg_fod_mot_diff_fm_th 2
  uint8x8_t med_filter_fm_th = vdup_n_u8(2);    //o_reg_fod_med_filter_fm_th 2

  uint8x8_t filter1, filter2;
  uint8x8_t filter4;
#ifndef REMOVE_T0B0
  uint8x8_t filter3;
#endif

  int yu, yd, y;

  int x_start = (g_fod_win_en == 1) ? X_START : 0;
  int x_end = (g_fod_win_en == 1) ? X_END : g_Width;
  int y_start = (g_fod_win_en == 1) ? Y_START : 0;
  int y_end = (g_fod_win_en == 1) ? Y_END : g_Height;
#ifdef SUPPORT_TILE
  int stride = g_tile_width;
  int base_x = 0;
#else
  int stride = g_Width;
#endif

  frame_motion_T1B0 = 0;
  frame_motion_T0B1 = 0;
#ifndef REMOVE_T0B0
  frame_motion_T0B0 = 0;
#endif
  frame_motion_T1B1 = 0;

  for( y = y_start ; y < y_end ; y+=2 )
  {
    int base_yu, base_y, base_yd, x;
    yu = y - 2;
    if (yu < y_start)
      yu = y;

    yd = y + 2;
    if (yd >= y_end)
      yd = y;

#ifdef SUPPORT_TILE
    base_yu = stride*(yu & H_Mod) + (yu >> H_shift) * (TILE_Height * g_pitch);
    base_y  = stride*(y  & H_Mod) + (y  >> H_shift) * (TILE_Height * g_pitch);
    base_yd = stride*(yd & H_Mod)+  (yd >> H_shift) * (TILE_Height * g_pitch);
#else
    base_yu = stride*yu;
    base_y  = stride*y;
    base_yd = stride*yd;
#endif



    for( x = x_start ; x < x_end; x+=8 )
    {
      uint8x8_t f0_y0, f1_y0, f2_y0, f3_y0, f2_yu, f0_yu;
      uint8x8_t  r_filter1, r_filter2, r_filter4;
#ifndef REMOVE_T0B0
      uint8x8_t r_filter3;
      uint8x8_t diff3;
#endif
      uint8x8_t rst;

      uint8x8_t diff1, r_diff1;
      uint8x8_t diff2, r_diff2;
      uint8x8_t r_diff3;
      int x_pos;

#ifdef SUPPORT_TILE
      base_x = (x >> g_tile_w_shift) * (g_tile_width * TILE_Height);
      x_pos =  (x & g_tile_w_mod) + base_x;
#else
      x_pos = x;
#endif



      f0_y0 = vshr_n_u8(vld1_u8(preField + base_y + x_pos ), 3);
      f0_yu = vshr_n_u8(vld1_u8(preField + base_yu + x_pos ), 3);
      f1_y0 = vshr_n_u8(vld1_u8(curField + base_y + x_pos ), 3);
      f2_y0 = vshr_n_u8(vld1_u8(nxtField + base_y + x_pos ), 3);
      f2_yu = vshr_n_u8(vld1_u8(nxtField + base_yu + x_pos ), 3);
      f3_y0 = vshr_n_u8(vld1_u8(nx2Field + base_y + x_pos ), 3);
      /*
      //if(realline_cycl==0)
      diff = abs((f0_y0>>5) - (f2_y0>>5) );
      diff = (diff > mot_diff_th);
      filter1 = abs(med3( (f1_y0>>5), (f2_y0>>5), (f1_yd>>5) ) - (f2_y0>>5) );
      if ( diff && (filter1 > med_filter_th) )
        frame_motion_T1B0++;*/

      /*
      //if(realline_cycl==1)
      diff = abs((f1_y0>>5) - (f3_y0>>5) );
      diff = (diff > mot_diff_th);
      filter1 = abs(med3( (f2_yu>>5), (f1_y0>>5), (f2_yd>>5) ) - (f1_y0>>5) );
      if (diff && (filter1 > med_filter_th) )
        frame_motion_T1B0++;
      */

      diff1   = vabd_u8(f1_y0, f3_y0);
      r_diff1 = vcgt_u8(diff1, mot_diff_th);
      filter1 = vabd_u8(vmed3_i8(f2_yu, f1_y0, f2_y0), f1_y0);
      r_filter1 = vcgt_u8(filter1, med_filter_th);
      rst = vtst_u8(r_diff1, r_filter1);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T1B0  += vget_lane_u8(rst, 0);
      frame_motion_T1B0  += vget_lane_u8(rst, 1);
      frame_motion_T1B0  += vget_lane_u8(rst, 2);
      frame_motion_T1B0  += vget_lane_u8(rst, 3);
      frame_motion_T1B0  += vget_lane_u8(rst, 4);
      frame_motion_T1B0  += vget_lane_u8(rst, 5);
      frame_motion_T1B0  += vget_lane_u8(rst, 6);
      frame_motion_T1B0  += vget_lane_u8(rst, 7);
      /*
      filter2 = abs(med3( (f0_yu>>5), (f3_y0>>5), (f0_yd>>5) ) - (f3_y0>>5) );
      if ( diff && (filter2 > med_filter_th) )
        frame_motion_T0B1++;*/

      diff2   = vabd_u8(f3_y0, f1_y0);
      r_diff2 = vcgt_u8(diff2, mot_diff_th);
      filter2 = vabd_u8(vmed3_i8(f0_yu, f3_y0, f0_y0), f3_y0);
      r_filter2 = vcgt_u8(filter2, med_filter_th);
      rst = vtst_u8(r_diff2, r_filter2);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T0B1  += vget_lane_u8(rst, 0);
      frame_motion_T0B1  += vget_lane_u8(rst, 1);
      frame_motion_T0B1  += vget_lane_u8(rst, 2);
      frame_motion_T0B1  += vget_lane_u8(rst, 3);
      frame_motion_T0B1  += vget_lane_u8(rst, 4);
      frame_motion_T0B1  += vget_lane_u8(rst, 5);
      frame_motion_T0B1  += vget_lane_u8(rst, 6);
      frame_motion_T0B1  += vget_lane_u8(rst, 7);

      r_diff3 = vcgt_u8(diff1, mot_diff_fm_th);
#ifndef REMOVE_T0B0
      /*
      filter3 = abs(med3( (f0_yu>>5), (f1_y0>>5), (f0_y0>>5) ) - (f1_y0>>5) );
      if ( diff && (filter3 > med_filter_fm_th) )
        frame_motion_T0B0++;*/
      filter3 = vabd_u8(vmed3_i8(f0_yu, f1_y0, f0_y0), f1_y0);
      r_filter3 = vcgt_u8(filter3, med_filter_fm_th);
      rst = vtst_u8(r_diff3, r_filter3);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T0B0  += vget_lane_u8(rst, 0);
      frame_motion_T0B0  += vget_lane_u8(rst, 1);
      frame_motion_T0B0  += vget_lane_u8(rst, 2);
      frame_motion_T0B0  += vget_lane_u8(rst, 3);
      frame_motion_T0B0  += vget_lane_u8(rst, 4);
      frame_motion_T0B0  += vget_lane_u8(rst, 5);
      frame_motion_T0B0  += vget_lane_u8(rst, 6);
      frame_motion_T0B0  += vget_lane_u8(rst, 7);
#endif
#ifndef REMOVE_T1B1
      /*
      filter4 = abs(med3( (f2_yu>>5), (f3_y0>>5), (f2_yd>>5) ) - (f3_y0>>5) );
      if ( diff && (filter4 > med_filter_fm_th) )
        frame_motion_T1B1++;*/
      filter4 = vabd_u8(vmed3_i8(f2_yu, f3_y0, f2_y0), f3_y0);
      r_filter4 = vcgt_u8(filter4, med_filter_fm_th);
      rst = vtst_u8(r_diff3, r_filter4);
      rst = vshr_n_u8(rst, 7);
      frame_motion_T1B1  += vget_lane_u8(rst, 0);
      frame_motion_T1B1  += vget_lane_u8(rst, 1);
      frame_motion_T1B1  += vget_lane_u8(rst, 2);
      frame_motion_T1B1  += vget_lane_u8(rst, 3);
      frame_motion_T1B1  += vget_lane_u8(rst, 4);
      frame_motion_T1B1  += vget_lane_u8(rst, 5);
      frame_motion_T1B1  += vget_lane_u8(rst, 6);
      frame_motion_T1B1  += vget_lane_u8(rst, 7);
#endif
    }
  }

}
#endif
#define mymax(a,b)            (((a) > (b)) ? (a) : (b))
static unsigned int fastabs(int v)
{
  unsigned int r;
  int const mask = (v >> sizeof(int)) * CHAR_BIT - 1;
  r = (v + mask) ^ mask;
  return r;
}
void dumpDetFieldOrder_status(int CurIsBot)
{
  float ratio;
  int motion_T0B1, motion_T1B0;
  int bT1B0, bT0B1, bRatio;

#ifdef FILE_DBG
    char ofile[128] = "DUMP_DetFieldOrder.txt";
    FILE *fptr = fopen(ofile, "a");
    if( fptr == NULL ) return;
#endif
    g_ary_idx++;
    if (g_ary_idx == MAXFRAME)
      g_ary_idx = 0;
#ifdef FILE_DBG
    if(g_ary_idx == 0 && g_bInitPhase == 1)
    {
      fprintf(fptr, "// motionT0B1(TFF), motionT1B0(BFF), motionT0B0, curField(Top0/Bot1)\n");
    }
#endif
    //dump1 << "// " << dec << item_idx++ << " Motion_cnt_all_status_pre32 " << _dumpPnrStatus.Motion_cnt_all_status_pre32 << "\n";
    //dump1 << hex << setw(5) << frame_motion_T0B1[idx] << ", " << frame_motion_T1B0[idx] << "\n";
    motion_T0B1 = mymax(frame_motion_T0B1, 1);
    motion_T1B0 = mymax(frame_motion_T1B0, 1);

    bT1B0 = frame_motion_T1B0 > g_sceneChangeTh;
    bT0B1 = frame_motion_T0B1 > g_sceneChangeTh;

    ratio = (float)motion_T0B1/(float)motion_T1B0;

    ary_frame_motion_ratio[g_ary_idx] = (int)(ratio*256.0);
    bRatio = (fastabs(ary_frame_motion_ratio[g_ary_idx] - 256) < 16);
    ary_IsSceneChangeTh[g_ary_idx] = (bT0B1 && bT1B0 && bRatio);
#ifdef FILE_DBG
    fprintf(fptr, "%05x_%05x_%05x_%d, ratio=%d, bT0B1=%d bT1B0=%d bRatio=%d\n",
                  frame_motion_T0B1,
                  frame_motion_T1B0,
                  frame_motion_T1B1,
                  CurIsBot, ary_frame_motion_ratio[g_ary_idx], bT0B1, bT1B0, bRatio);
    fclose(fptr);
#endif

#ifdef DEBUG_INFO
    printf("[FOD] %05x_%05x_%05x_%d, ratio=%d, bT0B1=%d bT1B0=%d bRatio=%d\n",
      frame_motion_T0B1,
      frame_motion_T1B0,
      frame_motion_T1B1,
      CurIsBot, ary_frame_motion_ratio[g_ary_idx], bT0B1, bT1B0, bRatio);
#endif

}
#endif