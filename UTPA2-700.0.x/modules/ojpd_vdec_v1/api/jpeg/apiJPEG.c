//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiJPEG.c
/// @brief  JPEG API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef CMODEL
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "jpeg_def.h"
#include "drvJPD.h"
//#include "Utl.h"
////#include "drvUART.h"
#else
#include "jpeg_cmodel_def.h"
#include "jpeg_cmodel.h"
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <setjmp.h>
#endif
#include "jpeg_memory.h"
#include "apiJPEG.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "apiJPEG_priv.h"
#include "apiJPEG_v2.h"
void* pInstantJPD = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
// PS. CMODEL always supports progressive mode decode
#define SUPPORT_PROGRESSIVE_MODE    1

#define ENABLE_JPEG_NO_SIZE_LOWER_BOUND 0

#define SUPPORT_PROGRESSIVE_SCLAEDOWN_MODE    1 //CL82399

#define SW_OPTIMIZE         0
#define SW_JPD_RGB_CMYK     1
#ifdef CMODEL
// This funcion is not ready in CMODEL
#undef SW_JPD_RGB_CMYK
#define SW_JPD_RGB_CMYK     0
#endif

/* Need to check whether OS support jump API or not */
#ifdef CMODEL
#define JPD_LONGJUMP_SUPPORT
#endif
#if 0
#if !defined(MSOS_TYPE_ECOS)
#define USE_LIBC
#endif
#ifndef USE_LIBC
#define jmp_buf             MS_U32
#define setjmp(jmp_state)   FALSE
#define longjmp(jmp_state, status)
#endif
#endif

#define PRINT_JPD_DECODE_TIME   0
#define LOG_DATA_TO_USB         0
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// Max. allocated blocks
#ifdef CMODEL
#define JPEG_MAXBLOCKS    100
#else
#define JPEG_MAXBLOCKS    50
#endif

#define DEFAULT_DECODE_TIMEOUT  100

//fractional bits in scale factors
#define IFAST_SCALE_BITS    2

#define JPEG_TIFF_SOI_OFFSET        0x0201
#define JPEG_TIFF_JPEG_IMG_BYTES    0x0202

#define JPEG_TIFF_BIG_ENDIAN        0x4D4D
#define JPEG_TIFF_LITTLE_ENDIAN     0x4949

//EXIF Tag
#define JPEG_EXIF_TAG_MANUFACTURER              0x010F
#define JPEG_EXIF_TAG_MODEL                     0x0110
#define JPEG_EXIF_TAG_ORIENTATION               0x0112
#define JPEG_EXIF_TAG_DATETIME_MOD              0x0132
#define JPEG_EXIF_TAG_IFD_POINTER               0x8769
#define JPEG_EXIF_TAG_EXPOSURE_PROGRAM          0x8822
#define JPEG_EXIF_TAG_ISO_SPEED_RATING          0x8827
#define JPEG_EXIF_TAG_DATETIME_ORI              0x9003
#define JPEG_EXIF_TAG_EXPOSURE_TIME             0x829A
#define JPEG_EXIF_TAG_F_NUMBER                  0x829D
#define JPEG_EXIF_TAG_SHUTTER_SPEED             0x9201
#define JPEG_EXIF_TAG_APERTURE                  0x9202
#define JPEG_EXIF_TAG_EXPOSURE_BIAS             0x9204
#define JPEG_EXIF_TAG_FLASH                     0x9209
#define JPEG_EXIF_TAG_FOCAL_LENGTH              0x920A
#define JPEG_EXIF_TAG_IMAGE_WIDTH               0xA002
#define JPEG_EXIF_TAG_IMAGE_HEIGHT              0xA003

#define JPEG_MANUFACTURER_SIZE                  32
#define JPEG_MODEL_SIZE                         128

#define MIN_READBUFFER_SIZE 128

#define MRC_BUFFER_ADDR         _u32ReadBufferAddr
#define MRC_BUFFER_SIZE         _u32ReadBufferSize
#define MWC_BUFFER_ADDR         _u32WriteBufferAddr
#define MWC_BUFFER_SIZE         _u32WriteBufferSize
#define INTERNAL_BUFFER_ADDR    _u32InternalBufferAddr
#define INTERNAL_BUFFER_SIZE    _u32InternalBufferSize

#define HUFF_EXTEND_TBL(x,s) ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

#define HUFF_EXTEND_P(x,s)  HUFF_EXTEND_TBL(x,s)

/* EXIF parsing section */
#define EndianChangeL(_x)   \
    ((((_x) & 0xff) << 24) | (((_x) & 0xff00) << 8) | (((_x) & 0xff0000) >> 8) | (((_x) & 0xff000000) >> 24))

#define EndianChangeS(_x)   \
    ((((_x) & 0xff) << 8) | (((_x) & 0xff00) >> 8))

#define JPEG_TAG_EXIF   EndianChangeL(0x45786966)

#define JPEG_ABS(x)     (((x)>=0)?(x):(-(x)))


static APP0_Unit App0_Unit_Data;

// No need to do ZAG order in JPD mode
#if SW_JPD_RGB_CMYK
JPEG_STATIC MS_U16 _u16PaddingMcuNumber;

MS_U32 u32_Decode_Line;
MS_U8 *u8Out_buf;
//#define DCTSIZE 8
//#define DCTELEM int
#define PASS1_BITS  2
#define FIX_3_0727  ((MS_S32)25172)  /* FIX(3.072711026) */
#define FIX_2_5629  ((MS_S32)20995)  /* FIX(2.562915447) */
#define FIX_2_0531  ((MS_S32)16819)  /* FIX(2.053119869) */
#define FIX_1_9615  ((MS_S32)16069)  /* FIX(1.961570560) */
#define FIX_1_8477  ((MS_S32)15137)  /* FIX(1.847759065) */
#define FIX_1_5013  ((MS_S32)12299)  /* FIX(1.501321110) */
#define FIX_1_1758  ((MS_S32)9633)   /* FIX(1.175875602) */
#define FIX_0_8999  ((MS_S32)7373)   /* FIX(0.899976223) */
#define FIX_0_7653  ((MS_S32)6270)   /* FIX(0.765366865) */
#define FIX_0_5411  ((MS_S32)4433)   /* FIX(0.541196100) */
#define FIX_0_3901  ((MS_S32)3196)   /* FIX(0.390180644) */
#define FIX_0_2986  ((MS_S32)2446)   /* FIX(0.298631336) */
#define SCALE_DONE ((MS_S32) 1)
#define DESCALE(a,m)  (((a) + (SCALE_DONE << ((m)-1))) >> (m))
#define clamp(i) if (i & 0xFF00) i = (((~i) >> 15) & 0xFF);
#define SCALEBITS 16
#define ONE_HALF ((MS_S32) 1 << (SCALEBITS-1))
#define FIX(x) ((MS_S32) ((x) * (1L<<SCALEBITS) + 0.5))


#define HUFF_EXTEND(x,s)    ((x) < extend_test[s] ? (x) + extend_offset[s] : (x))

#ifdef CMODEL
// Same as drvJPD.h
// JPD Downscale Ratio
// Bellows are 1, 1/2, 1/4 and 1/8 in order
typedef enum
{
    E_JPD_DOWNSCALE_ORG     = 0x00
  , E_JPD_DOWNSCALE_HALF    = 0x01
  , E_JPD_DOWNSCALE_FOURTH  = 0x02
  , E_JPD_DOWNSCALE_EIGHTH  = 0x03
} JPD_DownScale;
#endif
#endif

#if LOG_DATA_TO_USB
FILE *logBinfp;

#define JPEG_DEBUG_API_MSG(format, args...)  do{if(logBinfp) fprintf(logBinfp, format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(logBinfp)  fprintf(logBinfp, format, ##args);}while(0)
#else
#define JPEG_DEBUG_API_MSG(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) printf(format, ##args);}while(0)
#endif
#define printf_red(args...)           do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)
#define UTOPIA_20_DBG(args...)        //do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)

#define JPEG_OVER_BUFFER_RET(addr, buf_addr, buf_size) \
do \
{ \
    if(((MS_U32)(addr) < (MS_U32)(buf_addr)) \
    || ((MS_U32)(addr) >= ((MS_U32)(buf_addr) + (MS_U32)(buf_size)))) \
    { \
        JPEG_DEBUG_API_ERR("%s [%d] invalid address 0x%lx\n", __FUNCTION__, __LINE__, (MS_U32)(addr)); \
        return FALSE; \
    } \
} \
while(0)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
//JPEG header marker id
typedef enum
{
    E_JPEG_SOF0     = 0xC0
  , E_JPEG_SOF1     = 0xC1
  , E_JPEG_SOF2     = 0xC2
  , E_JPEG_SOF3     = 0xC3
  , E_JPEG_SOF5     = 0xC5
  , E_JPEG_SOF6     = 0xC6
  , E_JPEG_SOF7     = 0xC7
  , E_JPEG_JPG      = 0xC8
  , E_JPEG_SOF9     = 0xC9
  , E_JPEG_SOF10    = 0xCA
  , E_JPEG_SOF11    = 0xCB
  , E_JPEG_SOF13    = 0xCD
  , E_JPEG_SOF14    = 0xCE
  , E_JPEG_SOF15    = 0xCF
  , E_JPEG_DHT      = 0xC4
  , E_JPEG_DAC      = 0xCC
  , E_JPEG_RST0     = 0xD0
  , E_JPEG_RST1     = 0xD1
  , E_JPEG_RST2     = 0xD2
  , E_JPEG_RST3     = 0xD3
  , E_JPEG_RST4     = 0xD4
  , E_JPEG_RST5     = 0xD5
  , E_JPEG_RST6     = 0xD6
  , E_JPEG_RST7     = 0xD7
  , E_JPEG_SOI      = 0xD8
  , E_JPEG_EOI      = 0xD9
  , E_JPEG_SOS      = 0xDA
  , E_JPEG_DQT      = 0xDB
  , E_JPEG_DNL      = 0xDC
  , E_JPEG_DRI      = 0xDD
  , E_JPEG_DHP      = 0xDE
  , E_JPEG_EXP      = 0xDF
  , E_JPEG_APP0     = 0xE0
  , E_JPEG_APP1     = 0xE1
  , E_JPEG_APP2     = 0xE2
  , E_JPEG_APP3     = 0xE3
  , E_JPEG_APP4     = 0xE4
  , E_JPEG_APP5     = 0xE5
  , E_JPEG_APP6     = 0xE6
  , E_JPEG_APP7     = 0xE7
  , E_JPEG_APP8     = 0xE8
  , E_JPEG_APP9     = 0xE9
  , E_JPEG_APP10     = 0xEA
  , E_JPEG_APP11     = 0xEB
  , E_JPEG_APP12     = 0xEC
  , E_JPEG_APP13     = 0xED
  , E_JPEG_APP14     = 0xEE
  , E_JPEG_APP15    = 0xEF
  , E_JPEG_JPG0     = 0xF0
  , E_JPEG_JPG1     = 0xF1
  , E_JPEG_JPG2     = 0xF2
  , E_JPEG_JPG3     = 0xF3
  , E_JPEG_JPG4     = 0xF4
  , E_JPEG_JPG5     = 0xF5
  , E_JPEG_JPG6     = 0xF6
  , E_JPEG_JPG7     = 0xF7
  , E_JPEG_JPG8     = 0xF8
  , E_JPEG_JPG9     = 0xF9
  , E_JPEG_JPG10     = 0xFA
  , E_JPEG_JPG11     = 0xFB
  , E_JPEG_JPG12     = 0xFC
  , E_JPEG_JPG13    = 0xFD
  , E_JPEG_COM      = 0xFE
  , E_JPEG_TEM      = 0x01
  , E_JPEG_ERROR    = 0x100
} JPEG_HdrMarker;

//------------------------------------------------------------------------------
typedef enum
{
    E_RLE_DC = 0,
    E_RLE_AC,
    E_RLE_ZRL,
    E_RLE_EOB,
} JPEG_RLESymbol;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_CoeffBuf
/// @brief \b Struct \b Description: The info of coefficient for JPEG decode
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U8 *pu8Data;         ///<data of coefficient of DC, AC
    MS_U16 u16Block_num_x;  ///<the number of block for width
    MS_U16 u16Block_num_y;  ///<the number of block for height
    MS_U16 u16Block_size;   ///<block size
    MS_U8 u8Block_len_x;    ///<The width of block
    MS_U8 u8Block_len_y;    ///<The height of block
} JPEG_CoeffBuf, *PJPEG_CoeffBuf;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_SVLD
/// @brief \b Struct \b Description: The info of SVLD for JPEG decode
//-----------------------------------------------------------------------------
typedef struct
{
    union
    {
        struct
        {
            MS_U32 amp          :11;    ///<The amplitude of value of VLI
            MS_U32 sign         :1;     ///<The sign of value of VLI
            MS_U32 run          :4;     ///<run value
            MS_U32 sym_type     :2;     ///<symbol type
            MS_U32 blk_type     :2;     ///<YUV type
            MS_U32 EOP          :1;     ///<End of picture
            MS_U32 trash        :11;    ///<reserved
        };

        struct
        {
            MS_U8 byte0;    ///<byte0 of SVLD
            MS_U8 byte1;    ///<byte1 of SVLD
            MS_U8 byte2;    ///<byte2 of SVLD
            MS_U8 byte3;    ///<byte3 of SVLD
        };
    };
} JPEG_SVLD;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: JPEG_HdrChk
/// @brief \b Struct \b Description: The info of header checking for JPEG decode
//-----------------------------------------------------------------------------
typedef union
{
    struct
    {
        MS_U8 DQT:1;///<has Quant Table?
        MS_U8 DHT:1;///<has Huffman Table?
//      MS_U8 SOF:1;
//      MS_U8 SOS:1;
    };
    MS_U8 result;   ///<wildcard for header check
} JPEG_HdrChk;
//------------------------------------------------------------------------------
typedef MS_BOOL ( *Pdecode_block_func )( MS_U8, MS_U16, MS_U16 );


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#ifdef CMODEL
JPEG_STATIC MS_U16 JPEG_MAX_HEIGHT = 0xFFFF;
JPEG_STATIC MS_U16 JPEG_MAX_WIDTH = 0xFFFF;
#else
JPEG_STATIC MS_U16 JPEG_MAX_HEIGHT = 1200;
JPEG_STATIC MS_U16 JPEG_MAX_WIDTH = 1600;
#endif

// max progressive resolution setting
JPEG_STATIC MS_U16 JPEG_PRO_MAX_HEIGHT  = 768;
JPEG_STATIC MS_U16 JPEG_PRO_MAX_WIDTH   = 1024;

JPEG_STATIC MS_U16 MAX_JPEG_WIDTH_HD    = 0;
JPEG_STATIC MS_U16 MAX_JPEG_HEIGHT_HD   = 0;

#if SUPPORT_MPO_FORMAT
JPEG_STATIC MS_U16 JPEG_MPO_MAX_HEIGHT = 1200;
JPEG_STATIC MS_U16 JPEG_MPO_MAX_WIDTH = 1600;
JPEG_STATIC MS_U16 JPEG_MPO_PRO_MAX_HEIGHT  = 768;
JPEG_STATIC MS_U16 JPEG_MPO_PRO_MAX_WIDTH   = 1024;
#endif
//------------------------------------------------------------------------------
#if 0
// Default Table for JPEG
const MS_U16 g16GRPINFO_TBL[128]=
{
0x0000,
0x0000,
0x0000,
0x0100,
0x0010,
0x0140,
0x0060,
0x01e0,
0x0070,
0x01f0,
0x0080,
0x01f8,
0x0090,
0x01fc,
0x00a0,
0x01fe,
0x00b0,
0x01ff,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0100,
0x0030,
0x01c0,
0x0040,
0x01e0,
0x0050,
0x01f0,
0x0060,
0x01f8,
0x0070,
0x01fc,
0x0080,
0x01fe,
0x0090,
0x01ff,
0x80a0,
0x01ff,
0xc0b0,
0x01ff,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0100,
0x0002,
0x0180,
0x0003,
0x01a0,
0x0006,
0x01d0,
0x0009,
0x01e8,
0x000b,
0x01f0,
0x000f,
0x01f8,
0x0012,
0x01fb,
0x8017,
0x01fd,
0xc01c,
0x01fe,
0x4020,
0x01ff,
0x0000,
0x0000,
0x0000,
0x0000,
0x8024,
0x01ff,
0x8225,
0x01ff,
0x0000,
0x0000,
0x0000,
0x0100,
0x0002,
0x0180,
0x0003,
0x01a0,
0x0005,
0x01c0,
0x0009,
0x01e0,
0x000d,
0x01f0,
0x0010,
0x01f6,
0x0014,
0x01fa,
0x801b,
0x01fd,
0xc020,
0x01fe,
0x4024,
0x01ff,
0x0000,
0x0000,
0x8028,
0x01ff,
0x8429,
0x01ff,
0x882b,
0x01ff
};
#endif
#if 0
const MS_U16 g16SYMIDX_TBL[]=
{
0x0001,
0x0102,
0x0203,
0x0300,
0x1104,
0x0411,
0x0505,
0x2112,
0x3121,
0x0631,
0x1241,
0x4106,
0x5113,
0x0751,
0x6161,
0x7107,
0x1322,
0x2271,
0x3214,
0x8132,
0x0881,
0x1491,
0x42a1,
0x9108,
0xa123,
0xb142,
0xc1b1,
0x09c1,
0x2315,
0x3352,
0x52d1,
0xf0f0,
0x1524,
0x6233,
0x7262,
0xd172,
0x0a82,
0x1609,
0x240a,
0x3416,
0xe117,
0x2518,
0xf119,
0x171a,
0x1825,
0x1926,
0x1a27,
0x2628,
0x2729,
0x282a,
0x2934,
0x2a35,
0x3536,
0x3637,
0x3738,
0x3839,
0x393a,
0x3a43,
0x4344,
0x4445,
0x4546,
0x4647,
0x4748,
0x4849,
0x494a,
0x4a53,
0x5354,
0x5455,
0x5556,
0x5657,
0x5758,
0x5859,
0x595a,
0x5a63,
0x6364,
0x6465,
0x6566,
0x6667,
0x6768,
0x6869,
0x696a,
0x6a73,
0x7374,
0x7475,
0x7576,
0x7677,
0x7778,
0x7879,
0x797a,
0x7a83,
0x8284,
0x8385,
0x8486,
0x8587,
0x8688,
0x8789,
0x888a,
0x8992,
0x8a93,
0x9294,
0x9395,
0x9496,
0x9597,
0x9698,
0x9799,
0x989a,
0x99a2,
0x9aa3,
0xa2a4,
0xa3a5,
0xa4a6,
0xa5a7,
0xa6a8,
0xa7a9,
0xa8aa,
0xa9b2,
0xaab3,
0xb2b4,
0xb3b5,
0xb4b6,
0xb5b7,
0xb6b8,
0xb7b9,
0xb8ba,
0xb9c2,
0xbac3,
0xc2c4,
0xc3c5,
0xc4c6,
0xc5c7,
0xc6c8,
0xc7c9,
0xc8ca,
0xc9d2,
0xcad3,
0xd2d4,
0xd3d5,
0xd4d6,
0xd5d7,
0xd6d8,
0xd7d9,
0xd8da,
0xd9e1,
0xdae2,
0xe2e3,
0xe3e4,
0xe4e5,
0xe5e6,
0xe6e7,
0xe7e8,
0xe8e9,
0xe9ea,
0xeaf1,
0xf2f2,
0xf3f3,
0xf4f4,
0xf5f5,
0xf6f6,
0xf7f7,
0xf8f8,
0xf9f9,
0xfafa,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0000,
0x0101,
0x0202,
0x0303,
0x0404,
0x0505,
0x0606,
0x0707,
0x0808,
0x0909,
0x0a0a,
0x0b0b,
0x0000,
0x0000,
0x0000,
0x0000
};
#endif
#if 0
const MS_U16 g16IQ_TBL[128]=
{
0x0010,
0x000b,
0x000a,
0x0010,
0x0018,
0x0028,
0x0033,
0x003d,
0x000c,
0x000c,
0x000e,
0x0013,
0x001a,
0x003a,
0x003c,
0x0037,
0x000e,
0x000d,
0x0010,
0x0018,
0x0028,
0x0039,
0x0045,
0x0038,
0x000e,
0x0011,
0x0016,
0x001d,
0x0033,
0x0057,
0x0050,
0x003e,
0x0012,
0x0016,
0x0025,
0x0038,
0x0044,
0x006d,
0x0067,
0x004d,
0x0018,
0x0023,
0x0037,
0x0040,
0x0051,
0x0068,
0x0071,
0x005c,
0x0031,
0x0040,
0x004e,
0x0057,
0x0067,
0x0079,
0x0078,
0x0065,
0x0048,
0x005c,
0x005f,
0x0062,
0x0070,
0x0064,
0x0067,
0x0063,
0x0011,
0x0012,
0x0018,
0x002f,
0x0063,
0x0063,
0x0063,
0x0063,
0x0012,
0x0015,
0x001a,
0x0042,
0x0063,
0x0063,
0x0063,
0x0063,
0x0018,
0x001a,
0x0038,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x002f,
0x0042,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063,
0x0063
};
#endif

/* entry n is (-1 << n) + 1 */
static const MS_S32 extend_offset[16] =
{
    0, (( -1)<<1)+1, ((-1)<<2)+1, ((-1)<<3) + 1, ((-1)<<4)+1, ((-1)<<5)+1,
    ((-1)<<6)+1, ((-1)<<7)+1, ((-1)<<8)+1, ((-1)<<9)+1, ((-1) <<10)+1,
    ((-1)<<11)+1, ((-1)<<12)+1, ((-1)<<13)+1, ((-1)<<14)+1, ((-1)<<15)+1
};

/* entry n is 2**(n-1) */
static const MS_S32 extend_test[16] =
{
    0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000
};

static const MS_S32 extend_mask[] =
{
    0, (1<<0), (1<<1), (1<<2), (1<<3), (1<<4), (1<<5), (1<<6), (1<<7), (1<<8),
    (1<<9), (1<<10), (1<<11), (1<<12), (1<<13), (1<<14), (1<<15), (1<<16),
};


JPEG_STATIC MS_U32 _u32ReadBufferAddr;
JPEG_STATIC MS_U32 _u32ReadBufferSize;
JPEG_STATIC MS_U32 _u32WriteBufferAddr;
JPEG_STATIC MS_U32 _u32WriteBufferSize;
JPEG_STATIC MS_U32 _u32InternalBufferAddr;
JPEG_STATIC MS_U32 _u32InternalBufferSize;

JPEG_STATIC MS_U32 _u32RLEOffset;   // offset to record the current RLE access address

JPEG_STATIC JPEG_HdrChk _HeadCheck;

JPEG_STATIC JPEG_BLOCK_TYPE _s16dc_pred[3];

// The width/height may be the thumbnail or original image size, it based on decoding mode
JPEG_STATIC MS_U16 _u16Image_x_size;
JPEG_STATIC MS_U16 _u16Image_y_size;

// The original size of this JPEG file after alignment
JPEG_STATIC MS_U16 _u16OriginalImage_x_size;
JPEG_STATIC MS_U16 _u16OriginalImage_y_size;

// The original size before alignment
JPEG_STATIC MS_U16 _u16NonAlignmentImage_x_size;
JPEG_STATIC MS_U16 _u16NonAlignmentImage_y_size;

// The width/height/pitch of image for displaying.
JPEG_STATIC MS_U16 _u16AlignedImageWidth;
JPEG_STATIC MS_U16 _u16AlignedImagePitch;
JPEG_STATIC MS_U16 _u16AlignedImageHeight;
JPEG_STATIC MS_U16 _u16AlignedImagePitch_H;

// Scale Down Factor
JPEG_STATIC MS_U8 _u8ScaleDownFactor;

////JPEG_STATIC PJPEG_FILE_FileSystem_t _pStream;

JPEG_STATIC MS_U8 _u8DecodeType = E_JPEG_TYPE_MAIN;
JPEG_STATIC MS_BOOL _bProgressive_flag;

#ifndef CMODEL
JPEG_STATIC MS_U8 _u8DownScaleRatio;
#endif

JPEG_STATIC MS_BOOL _bFirstRLE;

/******* Thumbnail related *******/
JPEG_STATIC MS_BOOL _bThumbnailFound;
JPEG_STATIC MS_BOOL _bThumbnailAccessMode;
JPEG_STATIC MS_U32 _u32ThumbnailOffset;
JPEG_STATIC MS_U16 _u16ThumbnailSize;
JPEG_STATIC MS_BOOL _bTiffBigEndian;

JPEG_STATIC MS_U32 _u32ThumbnailBufferOffset;   // keep track of thumb buffer access address
JPEG_STATIC MS_U32 _u16ThumbnailBufferSize;   // keep track of thumb buffer size
/*****************************/

JPEG_STATIC JPEG_HuffInfo _Huff_info[JPEG_MAXHUFFTABLES];

JPEG_STATIC JPEG_QuantTbl _QuantTables[JPEG_MAXQUANTTABLES]; /* pointer to quantization tables */

JPEG_STATIC MS_U8 _u8Comps_in_frame;                             /* # of components in frame */
JPEG_STATIC MS_U8 _u8Comp_h_samp[JPEG_MAXCOMPONENTS];     /* component's horizontal sampling factor */
JPEG_STATIC MS_U8 _u8Comp_v_samp[JPEG_MAXCOMPONENTS];     /* component's vertical sampling factor */
JPEG_STATIC MS_U8 _u8Comp_quant[JPEG_MAXCOMPONENTS];      /* component's quantization table selector */
JPEG_STATIC MS_U8 _u8Comp_ident[JPEG_MAXCOMPONENTS];      /* component's ID */

// The Luma and Chroma (YU) component ID, default is 1 & 2
JPEG_STATIC MS_U8 _u8LumaCi = 1;
JPEG_STATIC MS_U8 _u8ChromaCi = 2;
JPEG_STATIC MS_U8 _u8Chroma2Ci = 3;

JPEG_STATIC MS_U16 _u16Comp_h_blocks[JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U16 _u16Comp_v_blocks[JPEG_MAXCOMPONENTS];

JPEG_STATIC MS_U8 _u8Comps_in_scan;                  /* # of components in scan */
JPEG_STATIC MS_U8 _u8Comp_list[JPEG_MAXCOMPSINSCAN];      /* components in this scan */
JPEG_STATIC MS_U8 _u8Comp_dc_tab[JPEG_MAXCOMPONENTS];     /* component's DC Huffman coding table selector */
JPEG_STATIC MS_U8 _u8Comp_ac_tab[JPEG_MAXCOMPONENTS];     /* component's AC Huffman coding table selector */

JPEG_STATIC MS_U8 _u8Spectral_start;                 /* spectral selection start */
JPEG_STATIC MS_U8 _u8Spectral_end;                   /* spectral selection end   */
JPEG_STATIC MS_U8 _u8Successive_low;                 /* successive approximation low */
JPEG_STATIC MS_U8 _u8Successive_high;                /* successive approximation high */

JPEG_STATIC MS_U8 _u8Blocks_per_mcu;
JPEG_STATIC MS_U32 _u32Max_blocks_per_row;
JPEG_STATIC MS_U16 _u16Mcus_per_row;
JPEG_STATIC MS_U16 _u16Mcus_per_col;

JPEG_STATIC MS_U8 _u8Mcu_org[JPEG_MAXBLOCKSPERMCU];

JPEG_STATIC MS_U8 gu8Max_mcu_x_size;          /* MCU's max. X size in pixels */
JPEG_STATIC MS_U8 gu8Max_mcu_y_size;                 /* MCU's max. Y size in pixels */
JPEG_STATIC MS_U16 gu16Max_mcus_per_row;

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
JPEG_STATIC MS_U16 _u16Total_lines_left;               /* total # lines left in image */
JPEG_STATIC MS_U32 _u32Block_y_mcu[JPEG_MAXCOMPONENTS];
JPEG_STATIC JPEG_HuffTbl _Huff_tbls[JPEG_MAXHUFFTABLES];
JPEG_STATIC PJPEG_CoeffBuf _DC_Coeffs[JPEG_MAXCOMPONENTS];
JPEG_STATIC PJPEG_CoeffBuf _AC_Coeffs[JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U32 _u32Last_dc_val[JPEG_MAXCOMPONENTS];
JPEG_STATIC MS_U32 _u32EOB_run;
#ifdef CMODEL
JPEG_STATIC MS_U16 gu16Mcu_lines_left;                 /* total # lines left in this MCU */
#endif
#endif

JPEG_STATIC MS_U16 _u16Max_blocks_per_mcu;
JPEG_STATIC MS_U16 _u16Max_mcus_per_col;

JPEG_STATIC MS_U8 gu8Scan_type;                 /* Grey, Yh1v1, Yh1v2, Yh2v1, Yh2v2,
                                       CMYK111, CMYK4114 */
JPEG_STATIC MS_U8 *_pu8In_buf_ofs;
JPEG_STATIC MS_U32 _u32In_buf_left;
JPEG_STATIC MS_U8 _u8Tem_flag;
JPEG_STATIC MS_BOOL _bEOF_flag;

JPEG_STATIC MS_U8 *_pu8In_buf;

JPEG_STATIC MS_S16 _s16Bits_left;
JPEG_STATIC MS_U32 _u32Bit_buf;

JPEG_STATIC MS_U16 _u16Restart_interval;
JPEG_STATIC MS_U16 _u16Restarts_left;
JPEG_STATIC MS_U16 _u16Next_restart_num;

JPEG_STATIC void *_pBlocks[JPEG_MAXBLOCKS];         /* list of all dynamically allocated blocks */

JPEG_STATIC JPEG_ErrCode _Error_code;

#ifdef CMODEL
JPEG_STATIC MS_BOOL _bReady_flag;
#endif

//#ifdef USE_LIBC
#ifdef JPD_LONGJUMP_SUPPORT
JPEG_STATIC jmp_buf _jmp_state;
#endif
//#endif

////JPEG_STATIC MS_S32 _Total_Decoded_Size = 0;
#if 0 // not implement for new MDDI - harold
JPEG_STATIC MS_U32  NumPics;
#endif
JPEG_STATIC MS_U32 _u32Total_bytes_read;

//JPEG_STATIC MS_U8 *_pu32ExifHeaderAddr;

JPEG_STATIC JPEG_FillHdrFunc _pFillHdrFunc = NULL;

#ifdef CMODEL
extern const MS_U8 _u8ZAG[64];
extern const MS_U8 _u8Jpeg_zigzag_order[64];
#elif SW_JPD_RGB_CMYK
static const MS_U8 _u8ZAG[64+1] =
{
    0, 1, 8, 16, 9, 2, 3, 10,
    17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
    ,0
};
#endif

JPEG_STATIC MSIF_Version _api_jpeg_version = {
    .DDI = { JPEG_API_VERSION },
};

JPEG_STATIC MS_U8 _u8JPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
JPEG_STATIC MS_BOOL bMHEG5 = FALSE;

JPEG_STATIC MS_U16 _JPD_PreVIdx = 0; //For H/W bug, some cases can not exit after decode done
JPEG_STATIC MS_BOOL _JPD_IsDecoding = FALSE; //For H/W bug, some cases can not exit after decode done
JPEG_STATIC MS_U32 _JPD_ReCheckTime = 0; //For H/W bug, some cases can not exit after decode done

JPEG_STATIC MS_BOOL _Progressive_ROI_flag = FALSE; //CL82399
JPEG_STATIC MS_U16 ROI_width; //CL82399

JPEG_STATIC JPEG_BuffLoadType u8PreLHFlag = E_JPEG_BUFFER_NONE;

JPEG_STATIC MS_U32 u32MRCheckCount = 0;

JPEG_STATIC MS_BOOL _bIsInit = FALSE;

///JPEG_STATIC MS_U32 jpeg_input = 0;
///#define JPEG_GO     do{scanf("%d", &jpeg_input);}while(0)

#if SW_JPD_RGB_CMYK
JPEG_BLOCK_TYPE *_ps16Block_seg[JPEG_MAXBLOCKSPERROW];
MS_U8 _u8Block_max_zag_set[JPEG_MAXBLOCKSPERROW];
MS_U8 *gpu8Sample_buf;
MS_S32 gs32Crr[256];
MS_S32 gs32Cbb[256];
MS_S32 gs32Crg[256];
MS_S32 gs32Cbg[256];

MS_U8 *pgu8Scan_line_0;
MS_U8 *pgu8scan_line_1;

//JPEG_STATIC MS_U16 _u16Real_dest_bytes_per_scan_line;
JPEG_STATIC MS_U16 _u16Dest_bytes_per_scan_line;        /* rounded up */
JPEG_STATIC MS_U8 _u8Dest_bytes_per_pixel;            /* currently, 4 (RGB) or 1 (Y) */

#if ( ! SUPPORT_PROGRESSIVE_MODE) && ( ! defined(CMODEL))
JPEG_STATIC MS_U16 _u16Total_lines_left;               /* total # lines left in image */
JPEG_STATIC MS_U32 _u32Block_y_mcu[JPEG_MAXCOMPONENTS];
//JPEG_STATIC MS_U16 gu16Mcu_lines_left;                 /* total # lines left in this MCU */
#endif
#endif

/*===========================================================================*/
JPEG_STATIC MS_BOOL __bIsMjpeg = FALSE;
JPEG_STATIC MS_U32 __u32RealMjpegBase = 0x00000000;
JPEG_STATIC MS_U32 __u32TrickyMjpegBase = 0x00000000;
/*===========================================================================*/

//The info of JPEG EXIF
JPEG_STATIC JPEG_EXIF_DateTime _stEXIF_DateTime;
JPEG_STATIC JPEG_EXIF_Orientation _eEXIF_Orientation;

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_STATIC MS_U8 _u8EXIF_Manufacturer[JPEG_MANUFACTURER_SIZE];
JPEG_STATIC MS_U8 _u8EXIF_Model[JPEG_MODEL_SIZE];
JPEG_STATIC MS_U16 _u16EXIF_Exposureprogram;
JPEG_STATIC MS_U16 _u16EXIF_Flash;
JPEG_STATIC MS_U32 _u32EXIF_ISOSpeedRatings;
JPEG_STATIC JPEG_RATIONAL _stEXIF_ExposureTime;
JPEG_STATIC JPEG_RATIONAL _stEXIF_FNumber;
JPEG_STATIC JPEG_RATIONAL _stEXIF_ShutterSpeedValue;
JPEG_STATIC JPEG_RATIONAL _stEXIF_ApertureValue;
JPEG_STATIC JPEG_RATIONAL _stEXIF_ExposureBiasValue;
JPEG_STATIC JPEG_RATIONAL _stEXIF_FocalLength;
JPEG_STATIC MS_U32 _u32EXIF_ImageWidth;
JPEG_STATIC MS_U32 _u32EXIF_ImageHeight;
#endif

//record data offset
JPEG_STATIC MS_U32 u32DataOffset = 0;
JPEG_STATIC MS_U32 u32SOFOffset = 0; //for SEC request, they need to know the offset of SOF marker

JPEG_STATIC MS_BOOL bIs3HuffTbl = FALSE;

#if SW_JPD_RGB_CMYK
JPEG_STATIC MS_BOOL bEnableCMYK = TRUE;
JPEG_STATIC MS_BOOL bEnableRGB = TRUE;
#endif


#if SUPPORT_MPO_FORMAT
//MPO buffer
static MS_U8 *_pu8In_buf_MPO_ofs;
static MS_U32 _u32In_buf_MPO_left = 0xFFFFFFFFUL;

static MS_BOOL mpo_load_data = TRUE;  //check whether load data when start decode or fill buffer.
static MS_BOOL bIsMPOFormat = FALSE;
static MS_U32 u32MPFOffset = 0;
#endif

#if PRINT_JPD_DECODE_TIME
static MS_U32 u32MeasureDecodeTime;
static MS_U32 u32MeasureDecodeTimeSW;
static MS_U32 u32MeasureDecodeTimeHW;
#endif

static unsigned long  u32CRC;

static void verJPD_CRC32_Init(void)
{
    u32CRC = 0xFFFFFFFF;
}


static MS_U32 verJPD_CRC32_GetResult(void)
{
    return (~u32CRC);
}


static void verJPD_Init_CRC_Table(MS_U32 *u32CRCtemptable)
{

    int i,j,tmp, result;

    for(i=0;i<=0xFF;i++)
    {
        tmp=i;
        result=0;

        for(j=1;j<9;j++)
        {
             if(tmp & 1)
             {
                        result |= (1 << (8 - j));
             }
             tmp >>= 1;
        }

        u32CRCtemptable[i]=result<<24;
        for(j=0; j<8; j++)
        {
            u32CRCtemptable[i] = (u32CRCtemptable[i] << 1) ^ ((u32CRCtemptable[i] & (1 << 31)) ? 0x04C11DB7 : 0);
        }

        tmp=u32CRCtemptable[i];
        result=0;

        for(j=1;j<33;j++)
        {
             if(tmp & 1)
             {
                result |= (1 << (32 - j));
             }
             tmp >>= 1;
        }
        u32CRCtemptable[i]=result;
        }
}

static void verJPD_CRC32_Update(const MS_U8 *pu8Data, MS_U32 u32Size)
{
    MS_U32 u32CRCTable[256];
    MS_U32  u32LoopCounter;

    if (pu8Data == NULL || u32Size == 0) return;

    verJPD_Init_CRC_Table(u32CRCTable);

    #if 0
        int i;
        for(i = 0;i<256;i++)
        {
            printf("0x%08x,\n", u32CRCTable[i]);
        }
    #endif

    for (u32LoopCounter=0; u32LoopCounter<u32Size; u32LoopCounter++)
    {
        u32CRC = (u32CRC >> 8) ^ u32CRCTable[ pu8Data[u32LoopCounter] ^ (unsigned char)(u32CRC & 0xFF) ];
    }
}

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
#if 0 // not implement for new MDDI - harold
JPEG_STATIC void MSAPI_MJPEG_Get_Pics(void);
#endif // not implement for new MDDI - harold

JPEG_STATIC void JPEG_terminate(JPEG_ErrCode status);

JPEG_STATIC MS_BOOL JPEG_decode_init(void);

#ifndef CMODEL
JPEG_STATIC MS_BOOL JPEG_StartDecode(void);
#endif

JPEG_STATIC void JPEG_GetAlignedResolution(MS_U16 *width, MS_U16 *height);

#if SW_JPD_RGB_CMYK
JPEG_STATIC MS_BOOL msAPI_JPEG_transform_row( void );
JPEG_STATIC JPEG_Result msAPI_JPEG_Baseline_Decode(void);
#endif //SW_JPD_RGB_CMYK

JPEG_STATIC MS_U32 JPEG_GetECS(void);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

JPEG_STATIC MS_BOOL JPEG_OVER_BUFFER(MS_U32 addr, MS_U32 buf_addr, MS_U32 buf_size)
{
    if((addr < buf_addr)
    || (addr >= (buf_addr+ buf_size)))
    {
        JPEG_DEBUG_API_ERR("%s [%d] invalid address 0x%lx\n", __FUNCTION__, __LINE__, addr);
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
// Refill the input buffer.
// This method will sit in a loop until (A) the buffer is full or (B)
// the stream's read() method reports an end of file condition.
JPEG_STATIC MS_BOOL JPEG_fill_read_buffer(void)
{
    JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer:\n");
    _u32In_buf_left = 0;
    _pu8In_buf_ofs = _pu8In_buf;

    if(_bEOF_flag
    && (FALSE == _bThumbnailAccessMode))
    {
        JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer : EOF!!!\n");
        return TRUE;
    }

    if(_bThumbnailAccessMode)
    {
        JPEG_DEBUG_API_MSG("Fill Thumbnail, buffer left = %ld, buffer offset = %ld\n", _u16ThumbnailBufferSize, _u32ThumbnailBufferOffset);
        if(_u16ThumbnailBufferSize>=MRC_BUFFER_SIZE)
        {
            JPEG_memcpy((void *)(_pu8In_buf + _u32In_buf_left), (void *)(INTERNAL_BUFFER_ADDR + _u32ThumbnailBufferOffset), MRC_BUFFER_SIZE);
            _u32In_buf_left = MRC_BUFFER_SIZE;
            _u32ThumbnailBufferOffset += MRC_BUFFER_SIZE;
            _u16ThumbnailBufferSize -= MRC_BUFFER_SIZE;
        }
        else
        {
            JPEG_memcpy((void *)(_pu8In_buf + _u32In_buf_left), (void *)(INTERNAL_BUFFER_ADDR + _u32ThumbnailBufferOffset), _u16ThumbnailBufferSize);
            _bEOF_flag = TRUE;
            _u32In_buf_left = _u16ThumbnailBufferSize;
            _u32ThumbnailBufferOffset += _u16ThumbnailBufferSize;
            _u16ThumbnailBufferSize = 0;
        }
    }
    else
    {
        if(_pFillHdrFunc)
        {
            MS_S32 byte_read = _pFillHdrFunc((MS_PHYADDR)MS_VA2PA((MS_U32)_pu8In_buf), MRC_BUFFER_SIZE);
            if(byte_read < 0)
            {
                JPEG_terminate(E_JPEG_STREAM_READ);
                return FALSE;
            }
            _u32In_buf_left = byte_read;
        }
        else
        {
            JPEG_DEBUG_API_MSG("_pFillHdrFunc is not registered!!\n");
            if (bMHEG5)
            {
                _u32In_buf_left = MRC_BUFFER_SIZE;
            }
            else
            {
                JPEG_terminate(E_JPEG_NOT_ENOUGH_HEADER_INFO);
                return FALSE;
            }
        }
    }
#if 0
    else
    {
        do
        {
            MS_S32 bytes_read = _pStream->read( _pu8In_buf + _u32In_buf_left, MRC_BUFFER_SIZE - _u32In_buf_left, &_bEOF_flag, _pStream);//, 0 );
            if ( bytes_read == -1 )
            {
                JPEG_terminate( E_JPEG_STREAM_READ );
            }
            _u32In_buf_left += bytes_read;
////            _Total_Decoded_Size += bytes_read;
        } while ( ( _u32In_buf_left < MRC_BUFFER_SIZE ) && ( !_bEOF_flag ) );
    }

    _u32Total_bytes_read += _u32In_buf_left;
#endif

    u32DataOffset += MRC_BUFFER_SIZE;
    return TRUE;
}
JPEG_STATIC MS_BOOL JPEG_force_fill_read_buffer(MS_U32 u32Size)
{
    JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer:\n");
    _u32In_buf_left = u32Size;
    _pu8In_buf_ofs = _pu8In_buf;

    if(_bEOF_flag)
    {
        JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer : EOF!!!\n");
        return TRUE;
    }

    if(_pFillHdrFunc)
    {
        MS_S32 byte_read = 0;
        if((MRC_BUFFER_SIZE-u32Size) > 0)
        {
            JPEG_DEBUG_API_MSG("[VA] _pu8In_buf=0x%lx\n", (MS_U32)_pu8In_buf);
            JPEG_DEBUG_API_MSG("[PA] _pu8In_buf=0x%lx\n", MS_VA2PA((MS_U32)_pu8In_buf));
            JPEG_DEBUG_API_MSG("[PA] callback addr=0x%lx\n", (MS_VA2PA((MS_U32)_pu8In_buf)+u32Size));
            byte_read  = _pFillHdrFunc((MS_PHYADDR)(MS_VA2PA((MS_U32)_pu8In_buf)+u32Size), MRC_BUFFER_SIZE-u32Size);
            JPEG_DEBUG_API_MSG("@@@fill byte_read=0x%lx\n", byte_read);
        }

        if(byte_read < 0)
        {
            JPEG_terminate(E_JPEG_STREAM_READ);
            return FALSE;
        }
        _u32In_buf_left += byte_read;
    }
    else
    {
        JPEG_DEBUG_API_MSG("_pFillHdrFunc is not registered!!\n");
        return FALSE;
    }

    return TRUE;
}
//------------------------------------------------------------------------------
// Logical rotate left operation.
JPEG_STATIC MS_U32 JPEG_rol(MS_U32 i, MS_U8 j)
{
    return ( ( i << j ) | ( i >> ( 32 - j ) ) );
}
//------------------------------------------------------------------------------
// Retrieve one character from the input stream.
JPEG_STATIC MS_U8 JPEG_get_char(void)
{
    MS_U8 c;

    // Any bytes remaining in buffer?
    if ( !_u32In_buf_left )
    {
        // Try to get more bytes.
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_get_char : JPEG_fill_read_buffer failed!!!\n");
            _u32In_buf_left = 0;
        }
        // Still nothing to get?
        if ( !_u32In_buf_left )
        {
            // Padd the end of the stream with 0xFF 0xD9 (EOI marker)
            // FIXME: Is there a better padding pattern to use?
            MS_U8 t = _u8Tem_flag;
            _u8Tem_flag ^= 1;
            if ( t )
            {
                return ( 0xD9 );
            }
            else
            {
                return ( 0xFF );
            }
        }
    }

    c = *_pu8In_buf_ofs++;
    _u32In_buf_left--;

    return ( c );
}
//------------------------------------------------------------------------------
// Same as previus method, except can indicate if the character is
// a "padd" character or not.
JPEG_STATIC MS_U8 JPEG_get_charP(MS_BOOL *Ppadding_flag)
{
    MS_U8 c;

    if ( !_u32In_buf_left )
    {
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_get_charP : JPEG_fill_read_buffer failed!!!\n");
            _u32In_buf_left = 0;
        }

        if ( !_u32In_buf_left )
        {
            *Ppadding_flag = TRUE;
#if 1
            _u8Tem_flag ^= 1;
            // This function is called during when progressive huffman decoding
            // Should not padding 0xFFD9 at this state
            return 0;
#else
            MS_U8 t;
            t = _u8Tem_flag;
            _u8Tem_flag ^= 1;
            if ( t )
            {
                return ( 0xD9 );
            }
            else
            {
                return ( 0xFF );
            }
#endif
        }
    }

    *Ppadding_flag = FALSE;

    c = *_pu8In_buf_ofs++;
    _u32In_buf_left--;

    return ( c );
}
//------------------------------------------------------------------------------
// Inserts a previously retrieved character back into the input buffer.
JPEG_STATIC void JPEG_stuff_char(MS_U8 q)
{
    *( --_pu8In_buf_ofs ) = q;
    _u32In_buf_left++;
}
//------------------------------------------------------------------------------
// Retrieves one character from the input stream, but does
// not read past markers. Will continue to return 0xFF when a
// marker is encountered.
// FIXME: Bad name?
JPEG_STATIC MS_U8 JPEG_get_octet(void)
{
    MS_BOOL padding_flag;
    MS_U8 c = JPEG_get_charP( &padding_flag );

    if ( c == 0xFF )
    {
        if ( padding_flag )
        {
            return ( 0xFF );
        }

        c = JPEG_get_charP( &padding_flag );
        if ( padding_flag )
        {
            JPEG_stuff_char( 0xFF );
            return ( 0xFF );
        }

        if ( c == 0x00 )
        {
            return ( 0xFF );
        }
        else
        {
            JPEG_stuff_char( c );
            JPEG_stuff_char( 0xFF );
            return ( 0xFF );
        }
    }

    return ( c );
}
//------------------------------------------------------------------------------
// Retrieves a variable number of bits from the input stream.
// Markers will not be read into the input bit buffer. Instead,
// an infinite number of all 1's will be returned when a marker
// is encountered.
// FIXME: Is it better to return all 0's instead, like the older implementation?
JPEG_STATIC MS_U32 JPEG_get_bits_2(MS_U8 numbits)
{
    MS_U32 i, c1, c2;

    i = ( _u32Bit_buf >> ( 16 - numbits ) ) & ( ( 1 << numbits ) - 1 );

    _s16Bits_left -= numbits;
    if ( _s16Bits_left <= 0 )
    {
        _u32Bit_buf = JPEG_rol( _u32Bit_buf, numbits += _s16Bits_left );

        c1 = JPEG_get_octet();
        c2 = JPEG_get_octet();

        _u32Bit_buf = ( _u32Bit_buf & 0xFFFF ) | ( ( ( MS_U32 )c1 ) << 24 ) | ( ( ( MS_U32 )c2 ) << 16 );

        _u32Bit_buf = JPEG_rol( _u32Bit_buf, -_s16Bits_left );

        _s16Bits_left += 16;
    }
    else
    {
        _u32Bit_buf = JPEG_rol( _u32Bit_buf, numbits );
    }

    return i;
}
//------------------------------------------------------------------------------
// Decodes a Huffman encoded symbol.
JPEG_STATIC MS_S32 JPEG_huff_decode(JPEG_HuffTbl *Ph)
{
    MS_S32 symbol;

    // Check first 8-bits: do we have a complete symbol?
    symbol = Ph->s16Look_up[( _u32Bit_buf >> 8 ) & 0xFF];
    if ( symbol < 0 )
    {
        // Decode more bits, use a tree traversal to find symbol.
        JPEG_get_bits_2( 8 );

        do
        {
            symbol = Ph->s16Tree[~symbol + ( 1 - JPEG_get_bits_2( 1 ) )];
        }
        while ( symbol < 0 );
    }
    else
    {
        JPEG_get_bits_2( Ph->u8Code_size[symbol] );
    }

    return symbol;
}
//------------------------------------------------------------------------------
// Unconditionally frees all allocated blocks.
JPEG_STATIC void JPEG_free_all_blocks(void)
{
    MS_U8 i;
#if 0
    if ( _pStream )
    {
        _pStream = NULL;
    }
#endif
    for ( i = 0; i < JPEG_MAXBLOCKS; i++ )
    {
        if(_pBlocks[i]!=NULL)
            JPEG_MEMORY_free( _pBlocks[i] );

        _pBlocks[i] = NULL;
    }
}
//------------------------------------------------------------------------------
// This method handles all errors.
// It could easily be changed to use C++ exceptions.
JPEG_STATIC void JPEG_terminate(JPEG_ErrCode status)
{
    _Error_code = status;

    JPEG_free_all_blocks();

    JPEG_DEBUG_API_ERR("%s:ERROR= %d\n", __FUNCTION__, _Error_code);
#ifdef JPD_LONGJUMP_SUPPORT
    longjmp( _jmp_state, status );
#endif
}
//------------------------------------------------------------------------------
// Allocate a block of memory-- store block's address in list for
// later deallocation by JPEG_free_all_blocks().
JPEG_STATIC void * JPEG_alloc(MS_U32 n)
{
    MS_U8 i;
    void *q;

    // Find a free slot. The number of allocated slots will
    // always be very low, so a linear search is good enough.
    for ( i = 0; i < JPEG_MAXBLOCKS; i++ )
    {
        if ( _pBlocks[i] == NULL )
        {
            break;
        }
    }

    if ( i >= JPEG_MAXBLOCKS )
    {
        JPEG_terminate( E_JPEG_TOO_MANY_BLOCKS );
        return NULL;
    }

    //FixMe: eCos does not support aligned allocate ???
    q = JPEG_MEMORY_malloc( n + 8 );
    //q = MsOS_AllocateAlignedMemory(n+8, 8, gs32CachedPoolID);
    //q = MsOS_AllocateMemory(n+8, gs32CachedPoolID);

    if ( q == NULL )
    {
        JPEG_terminate( E_JPEG_NOTENOUGHMEM );
        return NULL;
    }

    JPEG_memset((void *)q, 0, n + 8 );

    _pBlocks[i] = q;

    JPEG_DEBUG_API_MSG("JPEG %ld bytes allocated\n", n);

    return ( ( void* )q );
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U32 JPEG_Tiff_EndianChangeL(MS_U32 u32Val)
{
    if (_bTiffBigEndian)
        return EndianChangeL(u32Val);
    else
        return u32Val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U16 JPEG_Tiff_EndianChangeS(MS_U16 u16Val)
{
    if (_bTiffBigEndian)
        return EndianChangeS(u16Val);
    else
        return u16Val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U16 JPEG_GetU16(MS_U8 *data)
{
    MS_S8 i;
    MS_U16 val = 0;

    for(i = 1; i>=0; i--)
        val = (val << 8) + (MS_U8) *(data + i);


    return val;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_U32 JPEG_GetU32(MS_U8 *data)
{
    MS_S8 i;
    MS_U32 val = 0;

    for(i = 3; i>=0; i--)
        val = (val << 8) + (MS_U8) *(data + i);

    return val;
}
//------------------------------------------------------------------------------
//***************************************************
//Parse EXIF header
//***************************************************
JPEG_STATIC MS_BOOL JPEG_DecodeExifInfo(MS_U8 *data, MS_U32 data_length)
{
    MS_U8 *pJpegBuff = data;
//    MS_U32 u32tmp, u32Len;
    MS_U32 u32tmp;
    MS_U16 u16Marker, u16Len, u16NumOfEntry, i;
    MS_U8  *pTiffHdrPtr, *pNextIfd;
    MS_U32 u32DataValue;
    MS_U8 *pExifHdr = NULL, *pExifHdrOffet = NULL;
    MS_BOOL bIFD_pt_found = FALSE;

    u32tmp = JPEG_GetU32(pJpegBuff);
    if (JPEG_TAG_EXIF != u32tmp)
    {
        return FALSE;
    }

    // Exif starts here
    pJpegBuff += 6;
    pExifHdr = pJpegBuff;
    u16Marker = JPEG_GetU16(pJpegBuff);
    if (JPEG_TIFF_BIG_ENDIAN == u16Marker)
        _bTiffBigEndian = TRUE;
    else if (JPEG_TIFF_LITTLE_ENDIAN == u16Marker)
        _bTiffBigEndian = FALSE;
    else
        return FALSE;

    _u32ThumbnailOffset += 6;
    pTiffHdrPtr = pJpegBuff;

    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if (u16Marker != 0x002A)
        return FALSE;

    pJpegBuff += 2;
    u16Len = (MS_U16)JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
    pJpegBuff += (u16Len - 4); //ENDIAN(2byte) + 0x002A(2byte)

    // 0th IFD start here
    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    pJpegBuff += 2;

    // Boundary check, prevent from buffer over-run
    if((((MS_U32) pJpegBuff) - INTERNAL_BUFFER_ADDR + u16NumOfEntry*12)>=data_length)
    {
        return FALSE;
    }

//    pJpegBuff += 12*u16NumOfEntry;

    JPEG_DEBUG_API_MSG("IFD0 : u16NumOfEntry = %d\n", u16NumOfEntry);

    //Retrieve orientation
    for(i = 0; i < u16NumOfEntry; i++)
    {
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
        pJpegBuff += 8;

        if((JPEG_EXIF_TAG_ORIENTATION == u16Marker)
        && (E_JPEG_EXIF_ORIENT_NOT_FOUND == _eEXIF_Orientation))
        {
            //The datatype of orientation is short
            u32DataValue = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
            if((E_JPEG_EXIF_ORIENT_ROTATE_0 > u32DataValue)
            || (E_JPEG_EXIF_ORIENT_ROTATE_270 < u32DataValue))
            {
                _eEXIF_Orientation = E_JPEG_EXIF_ORIENT_RESERVED;
            }
            else
            {
                _eEXIF_Orientation = (JPEG_EXIF_Orientation)u32DataValue;
            }
            JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation);
        }
        else if(JPEG_EXIF_TAG_IFD_POINTER == u16Marker)
        {
            bIFD_pt_found = TRUE;
            //datavalue is an offset from exif marker for datetime
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            pExifHdrOffet = pExifHdr + u32DataValue;
            JPEG_OVER_BUFFER_RET(pExifHdrOffet, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
            JPEG_DEBUG_API_MSG("Exif IFD pointer = 0x%lx\n", (MS_U32)pExifHdrOffet);
        }
#if SUPPORT_EXIF_EXTRA_INFO
        else if(JPEG_EXIF_TAG_MANUFACTURER == u16Marker)
        {
            MS_U8 *pIFDPtrOffset;
            pIFDPtrOffset = pJpegBuff;
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pIFDPtrOffset));
            JPEG_DEBUG_API_MSG("MANUFACTURER Offset = 0x%lx\n", u32DataValue);
            pIFDPtrOffset = pExifHdr + u32DataValue;
            if(JPEG_OVER_BUFFER((MS_U32)pIFDPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE))
            {
                pJpegBuff += 4;
                continue;
            }
            //JPEG_DEBUG_API_MSG("MANUFACTURER addr = 0x%lx\n", (MS_U32)pIFDPtrOffset);
            MS_U8 u8i=0;
            while(*(pIFDPtrOffset+u8i) && u8i<JPEG_MANUFACTURER_SIZE)
            {
                _u8EXIF_Manufacturer[u8i] = *(pIFDPtrOffset+u8i);
                u8i++;
            }
        }
        else if(JPEG_EXIF_TAG_MODEL== u16Marker)
        {
            MS_U8 *pIFDPtrOffset;
            pIFDPtrOffset = pJpegBuff;
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pIFDPtrOffset));
            JPEG_DEBUG_API_MSG("MODEL Offset = 0x%lx\n", u32DataValue);
            pIFDPtrOffset = pExifHdr + u32DataValue;
            if(JPEG_OVER_BUFFER((MS_U32)pIFDPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE))
            {
                pJpegBuff += 4;
                continue;
            }
            //JPEG_DEBUG_API_MSG("MODEL addr = 0x%lx\n", (MS_U32)pIFDPtrOffset);
            MS_U8 u8i=0;
            while(*(pIFDPtrOffset+u8i) && u8i<JPEG_MODEL_SIZE)
            {
                _u8EXIF_Model[u8i] = *(pIFDPtrOffset+u8i);
                u8i++;
            }
        }
#endif
#if 0
        else if((JPEG_EXIF_TAG_DATETIME_MOD == u16Marker)
        && (FALSE == _stEXIF_DateTime.bHasDataTime))
        {
            _stEXIF_DateTime.bHasDataTime = TRUE;
            //datavalue is an offset from exif marker for datetime
            u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            pExifOffset = pExifHdr + u32DataValue;
            printf("pExifOffset1 = 0x%lx\n", (MS_U32)pExifOffset);
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Year);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Month);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Day);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Hour);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Minute);
            pExifOffset++;
            JPEG_AtoU32(pExifOffset, _stEXIF_DateTime.u32Second);
            JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation);
            JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime======= Exist = %d, Original = %d \n", _stEXIF_DateTime.bHasDataTime, bOriDateTimeFound);
            JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime.u32Year, _stEXIF_DateTime.u32Month, _stEXIF_DateTime.u32Day);
            JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime.u32Hour, _stEXIF_DateTime.u32Minute, _stEXIF_DateTime.u32Second);
        }
#endif
        pJpegBuff += 4;
    }

    if(TRUE == bIFD_pt_found)
    {
        u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifHdrOffet));
        pExifHdrOffet += 2;
        JPEG_DEBUG_API_MSG("EXIF IFD : u16NumOfEntry = %d\n", u16NumOfEntry);

        for(i = 0; i < u16NumOfEntry; i++)
        {
            u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifHdrOffet));
#if (SUPPORT_EXIF_EXTRA_INFO==false)
            if(JPEG_EXIF_TAG_DATETIME_ORI == u16Marker)
            {
                pExifHdrOffet += 8;
                _stEXIF_DateTime.bHasDataTime = TRUE;
                //datavalue is an offset from exif marker for datetime
                u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifHdrOffet));;
                pExifHdrOffet = pExifHdr + u32DataValue;
                JPEG_OVER_BUFFER_RET(pExifHdrOffet, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                JPEG_DEBUG_API_MSG("pExifOriDateTime Offset = 0x%lx\n", (MS_U32)pExifHdrOffet);
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Year);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Month);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Day);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Hour);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Minute);
                pExifHdrOffet++;
                JPEG_AtoU32(pExifHdrOffet, _stEXIF_DateTime.u32Second);
                JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime=======\n");
                JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime.u32Year, _stEXIF_DateTime.u32Month, _stEXIF_DateTime.u32Day);
                JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime.u32Hour, _stEXIF_DateTime.u32Minute, _stEXIF_DateTime.u32Second);
                break;
            }
#else
            MS_U8 *pExifPtrOffset;
            MS_U16 u16Type;
            MS_U32 u32Count;
            switch(u16Marker)
            {
                case JPEG_EXIF_TAG_DATETIME_ORI:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _stEXIF_DateTime.bHasDataTime = TRUE;
                    //datavalue is an offset from exif marker for datetime
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    JPEG_DEBUG_API_MSG("pExifOriDateTime Offset = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Year);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Month);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Day);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Hour);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Minute);
                    pExifPtrOffset++;
                    JPEG_AtoU32(pExifPtrOffset, _stEXIF_DateTime.u32Second);
                    JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime=======\n");
                    JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime.u32Year, _stEXIF_DateTime.u32Month, _stEXIF_DateTime.u32Day);
                    JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime.u32Hour, _stEXIF_DateTime.u32Minute, _stEXIF_DateTime.u32Second);
                    break;
                case JPEG_EXIF_TAG_ISO_SPEED_RATING:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 4;
                    u32Count = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        if(u32Count==2)
                        {
                            _u32EXIF_ISOSpeedRatings=JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                        }
                        else if(u32Count==1)
                        {
                            _u32EXIF_ISOSpeedRatings=JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                        }
                    }
                    else if(u16Type==4)
                    {
                        if(u32Count==1)
                        {
                            _u32EXIF_ISOSpeedRatings=JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                        }
                    }
                    JPEG_DEBUG_API_MSG("ISO Speed Rating=%ld\n", _u32EXIF_ISOSpeedRatings);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_TIME:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureTime Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("ExposureTime addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_ExposureTime.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ExposureTime.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureTime numerator=%ld, denominator=%ld\n", _stEXIF_ExposureTime.numerator, _stEXIF_ExposureTime.denominator);
                    break;
                case JPEG_EXIF_TAG_F_NUMBER:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FNumber Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("FNumber addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_FNumber.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_FNumber.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FNumber numerator=%ld, denominator=%ld\n", _stEXIF_FNumber.numerator, _stEXIF_FNumber.denominator);
                    break;

                case JPEG_EXIF_TAG_SHUTTER_SPEED:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ShutterSpeedValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("ShutterSpeedValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_ShutterSpeedValue.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ShutterSpeedValue.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ShutterSpeedValue numerator=%ld, denominator=%ld\n", _stEXIF_ShutterSpeedValue.numerator, _stEXIF_ShutterSpeedValue.denominator);
                    break;
                case JPEG_EXIF_TAG_APERTURE:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ApertureValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    //JPEG_DEBUG_API_MSG("ApertureValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    _stEXIF_ApertureValue.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ApertureValue.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ApertureValue numerator=%ld, denominator=%ld\n", _stEXIF_ApertureValue.s_numerator, _stEXIF_ApertureValue.s_denominator);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_BIAS:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureBiasValue Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("ExposureBiasValue addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_ExposureBiasValue.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_ExposureBiasValue.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("ExposureBiasValue numerator=%ld, denominator=%ld\n", _stEXIF_ExposureBiasValue.numerator, _stEXIF_ExposureBiasValue.denominator);
                    break;
                case JPEG_EXIF_TAG_FLASH:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _u16EXIF_Flash = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("EXIF_Flash=0x%x\n", _u16EXIF_Flash);
                    break;
                case JPEG_EXIF_TAG_FOCAL_LENGTH:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    u32DataValue = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FocalLength Offset = 0x%lx\n", u32DataValue);
                    pExifPtrOffset = pExifHdr + u32DataValue;
                    JPEG_OVER_BUFFER_RET(pExifPtrOffset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                    //JPEG_DEBUG_API_MSG("FocalLength addr = 0x%lx\n", (MS_U32)pExifPtrOffset);
                    _stEXIF_FocalLength.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    pExifPtrOffset+=4;
                    _stEXIF_FocalLength.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("FocalLength numerator=%ld, denominator=%ld\n", _stEXIF_FocalLength.s_numerator, _stEXIF_FocalLength.s_denominator);
                    break;
                case JPEG_EXIF_TAG_IMAGE_WIDTH:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        _u32EXIF_ImageWidth = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    }
                    else
                    {
                        _u32EXIF_ImageWidth = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    }
                    JPEG_DEBUG_API_MSG("EXIF_ImageWidth=%ld\n", _u32EXIF_ImageWidth);
                    break;
                case JPEG_EXIF_TAG_IMAGE_HEIGHT:
                    pExifPtrOffset = pExifHdrOffet + 2;
                    u16Type = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    pExifPtrOffset = pExifHdrOffet + 8;
                    if(u16Type==3)
                    {
                        _u32EXIF_ImageHeight = (MS_U32)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    }
                    else
                    {
                        _u32EXIF_ImageHeight = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pExifPtrOffset));
                    }
                    JPEG_DEBUG_API_MSG("EXIF_ImageHeight=%ld\n", _u32EXIF_ImageHeight);
                    break;
                case JPEG_EXIF_TAG_EXPOSURE_PROGRAM:
                    pExifPtrOffset = pExifHdrOffet + 8;
                    _u16EXIF_Exposureprogram = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pExifPtrOffset));
                    JPEG_DEBUG_API_MSG("EXIF_Exposureprogram=0x%x\n", _u16EXIF_Exposureprogram);
                    break;
                default:
                    break;
            }
#endif
            pExifHdrOffet += 12;
        }
    }

    if(E_JPEG_TYPE_THUMBNAIL != _u8DecodeType)
    {
        JPEG_DEBUG_API_MSG("decode type is not thumbnail...\n");
        return FALSE;
    }

    // 1th IFD
    u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
    if (u32tmp == 0)
        return FALSE;
    else
    {
        if(u32tmp >= data_length)
        {
            JPEG_DEBUG_API_ERR("u32tmp = %ld is invalid > %ld\n", u32tmp,data_length);
            return FALSE;
        }
        pNextIfd = &pTiffHdrPtr[u32tmp];
        JPEG_DEBUG_API_MSG("1st IFD pNextIfd = 0x%lx, u32tmp = %ld\n", (MS_U32)pNextIfd, u32tmp);
    }

    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pNextIfd));
    pNextIfd += 2;

    // Boundary check, prevent from buffer over-run
    if((((MS_U32) pNextIfd) - INTERNAL_BUFFER_ADDR + u16NumOfEntry*12)>=data_length)
    {
        return FALSE;
    }

    for (i = 0; i < u16NumOfEntry; i++)
    {
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pNextIfd));
//        u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pNextIfd + 4));
        u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pNextIfd + 8));

        switch (u16Marker)
        {
            case JPEG_TIFF_JPEG_IMG_BYTES:
                _u16ThumbnailSize = u32tmp;
                break;
            case JPEG_TIFF_SOI_OFFSET:
                JPEG_DEBUG_API_MSG("Thumbnail marker found in 0x%lx!!\n", (MS_U32)pNextIfd);
                _bThumbnailFound = TRUE;
                _u32ThumbnailOffset += u32tmp;
                break;
            default:
                break;
        }

        pNextIfd += 12;
    }

    // Boundary check, prevent from buffer over-run
    if(_bThumbnailFound)
    {
        if((_u32ThumbnailOffset + _u16ThumbnailSize) > data_length)
            _bThumbnailFound = FALSE;

        // means it only contains SOI header..
        if(_u16ThumbnailSize<=2)
            _bThumbnailFound = FALSE;
    }

    return _bThumbnailFound;
}

// Used to skip unrecognized markers.
JPEG_STATIC void JPEG_skip_bytes(MS_U32 count)
{
    while(count!=0)
    {
        // Any bytes remaining in buffer?
        if ( !_u32In_buf_left )
        {
            // Try to get more bytes.
            if(!JPEG_fill_read_buffer())
            {
                JPEG_DEBUG_API_MSG("JPEG_fill_read_buffer failed!!!\n");
                _u32In_buf_left = 0;
            }
            // Still nothing to get?
            if ( !_u32In_buf_left )
            {
                // should not happen
                break;
            }
        }

        if(count<_u32In_buf_left)
        {
            _u32In_buf_left -= count;
            _pu8In_buf_ofs += count;
            count = 0;
        }
        else
        {
            count -= _u32In_buf_left;
            _u32In_buf_left = 0;
        }
    }
}
//------------------------------------------------------------------------------
#if SUPPORT_MPO_FORMAT
#define JPEG_TAG_MPO                EndianChangeL(0x4D504600)
#define JPEG_MPO_TAG_MPF_VERSION        0xB000
#define JPEG_MPO_TAG_NUM_OF_IMAGES      0xB001
#define JPEG_MPO_TAG_MP_ENTRY           0xB002

static JPEG_MPO_INDEX_INFO _stMPOIndex;
static JPEG_MPO_ATTRIBUTE_INFO _stMPOAttri[JPEG_MPO_MAX_SUPPORT_IMAGE];
//For start of offset of MPO(only for first MPO picture)
static MS_U32 _u32App2MarkerOffset = 0;

typedef enum
{
    E_JPEG_MPO_ATTR_IMAGE_NO            = 0xB101
  , E_JPEG_MPO_ATTR_PAN_ORIENTATION     = 0xB201
  , E_JPEG_MPO_ATTR_PAN_OVERLAP_H       = 0xB202
  , E_JPEG_MPO_ATTR_PAN_OVERLAP_V       = 0xB203
  , E_JPEG_MPO_ATTR_BASE_VIEWPOINT_NO   = 0xB204
  , E_JPEG_MPO_ATTR_CONVERGENCE_ANGLE   = 0xB205
  , E_JPEG_MPO_ATTR_BASELINE_LENGTH     = 0xB206
  , E_JPEG_MPO_ATTR_DIVERGENCE_ANGLE    = 0xB207
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_X     = 0xB208
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_Y     = 0xB209
  , E_JPEG_MPO_ATTR_AXIS_DISTANCE_Z     = 0xB20A
  , E_JPEG_MPO_ATTR_YAW_ANGLE           = 0xB20B
  , E_JPEG_MPO_ATTR_PITCH_ANGLE         = 0xB20C
  , E_JPEG_MPO_ATTR_ROLL_ANGLE          = 0xB20D
} JPEG_MPO_ATTR_TAG;

#define MAX_MPO_ATTR_TAG_NUM        14          // from E_JPEG_MPO_ATTR_IMAGE_NO to E_JPEG_MPO_ATTR_ROLL_ANGLE

//***************************************************
//Initialize EXIF MPO database
//***************************************************
static void JPEG_InitMPO(void)
{
    MS_U32 i = 0;

    //init MPO Index structure
    _stMPOIndex.num_of_image = 1;
    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        _stMPOIndex.mp_entry[i].attribute = 0;
        _stMPOIndex.mp_entry[i].size = 0;
        _stMPOIndex.mp_entry[i].offset = 0;
        _stMPOIndex.mp_entry[i].image1_no = 0;
        _stMPOIndex.mp_entry[i].image2_no = 0;
    }

    // init MPO Attribute structure
    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        _stMPOAttri[i].MPIndividualNum = 0;
        _stMPOAttri[i].PanOrientation= 0;
        _stMPOAttri[i].PanOverlap_H.numerator = 0;
        _stMPOAttri[i].PanOverlap_H.denominator = 0;
        _stMPOAttri[i].PanOverlap_V.numerator = 0;
        _stMPOAttri[i].PanOverlap_V.denominator = 0;
        _stMPOAttri[i].BaseViewpointNum = 0;
        _stMPOAttri[i].ConvergenceAngle.s_numerator = 0;
        _stMPOAttri[i].ConvergenceAngle.s_denominator = 0;
        _stMPOAttri[i].BaselineLength.numerator = 0;
        _stMPOAttri[i].BaselineLength.denominator = 0;
        _stMPOAttri[i].VerticalDivergence.s_numerator = 0;
        _stMPOAttri[i].VerticalDivergence.s_denominator = 0;
        _stMPOAttri[i].AxisDistance_X.s_numerator = 0;
        _stMPOAttri[i].AxisDistance_X.s_denominator = 0;
        _stMPOAttri[i].AxisDistance_Y.s_numerator = 0;
        _stMPOAttri[i].AxisDistance_Y.s_denominator = 0;
        _stMPOAttri[i].AxisDistance_Z.s_numerator = 0;
        _stMPOAttri[i].AxisDistance_Z.s_denominator = 0;
        _stMPOAttri[i].YawAngle.s_numerator = 0;
        _stMPOAttri[i].YawAngle.s_denominator = 0;
        _stMPOAttri[i].PitchAngle.s_numerator = 0;
        _stMPOAttri[i].PitchAngle.s_denominator = 0;
        _stMPOAttri[i].RollAngle.s_numerator = 0;
        _stMPOAttri[i].RollAngle.s_denominator = 0;
        _stMPOAttri[i].used = FALSE;
    }
}
//***************************************************
//Parse EXIF MPO Attribute header
//***************************************************
static MS_BOOL JPEG_DecodeMPAttrIFD(MS_U8 *data, MS_U8 *start_of_offset, MS_U32 count)
{
    MS_U8 *pMPOAttrPtr = data;
    MS_U8 *pMPO_offset;
    MS_U16 u16Marker, i = 0, image_idx = 0;

    JPEG_DEBUG_API_MSG("JPEG_DecodeMPAttrIFD, count = %ld\n", count);

    if(count >MAX_MPO_ATTR_TAG_NUM)
    {
        JPEG_DEBUG_API_ERR("Warning!!!unreasonable attribute IFD number !!!\n");
        return TRUE;
    }

    for(i = 0; i < JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        if(FALSE == _stMPOAttri[i].used)
        {
            image_idx = i;
            break;
        }
    }

    if(JPEG_MPO_MAX_SUPPORT_IMAGE <= i)
    {
        JPEG_DEBUG_API_MSG("Not support too much images in MPO!!\n");
        return FALSE;
    }

    _stMPOAttri[image_idx].used = TRUE;

    for(i = 0; i < count; i++)
    {
        u16Marker = (JPEG_MPO_ATTR_TAG)JPEG_Tiff_EndianChangeS(JPEG_GetU16(pMPOAttrPtr));
        pMPOAttrPtr += 8;
        switch(u16Marker)
        {
            case E_JPEG_MPO_ATTR_IMAGE_NO:
                _stMPOAttri[image_idx].MPIndividualNum = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_PAN_ORIENTATION:
                _stMPOAttri[image_idx].PanOrientation = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_PAN_OVERLAP_H:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].PanOverlap_H.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].PanOverlap_H.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_PAN_OVERLAP_V:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].PanOverlap_V.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].PanOverlap_V.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_BASE_VIEWPOINT_NO:
                _stMPOAttri[image_idx].BaseViewpointNum = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_CONVERGENCE_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].ConvergenceAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].ConvergenceAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_BASELINE_LENGTH:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].BaselineLength.numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].BaselineLength.denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_DIVERGENCE_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].VerticalDivergence.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].VerticalDivergence.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_X:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].AxisDistance_X.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].AxisDistance_X.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_Y:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].AxisDistance_Y.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].AxisDistance_Y.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_AXIS_DISTANCE_Z:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].AxisDistance_Z.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].AxisDistance_Z.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_YAW_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].YawAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].YawAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_PITCH_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].PitchAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].PitchAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            case E_JPEG_MPO_ATTR_ROLL_ANGLE:
                pMPO_offset = start_of_offset + JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPOAttrPtr));
                JPEG_OVER_BUFFER_RET(pMPO_offset, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);
                _stMPOAttri[image_idx].RollAngle.s_numerator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                pMPO_offset += 4;
                _stMPOAttri[image_idx].RollAngle.s_denominator = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pMPO_offset));
                break;
            default:
                JPEG_DEBUG_API_ERR("Unknown MPO attribute marker =0x%x!!!!!!\n", u16Marker);
                break;
        }
        pMPOAttrPtr += 4;
    }
    return TRUE;
}
//***************************************************
//Parse EXIF MPO header
//***************************************************
static MS_BOOL JPEG_DecodeMPOInfo(MS_U8 *data)
{
    MS_U8 *pJpegBuff = data;
    MS_U8 *pMPOHdrPtr;
    MS_U32 u32tmp, u32Len;
    MS_U16 u16Marker, u16NumOfEntry, i;

    u32tmp = JPEG_GetU32(pJpegBuff);
    if (u32tmp != JPEG_TAG_MPO)
    {
        return FALSE;
    }

    // mpo header start from here
    pJpegBuff += 4;
    u16Marker = JPEG_GetU16(pJpegBuff);
    if (u16Marker == JPEG_TIFF_BIG_ENDIAN)
        _bTiffBigEndian = TRUE;
    else if (u16Marker == JPEG_TIFF_LITTLE_ENDIAN)
        _bTiffBigEndian = FALSE;
    else
        return FALSE;

//    _u32ThumbnailOffset += 4;
    pMPOHdrPtr = pJpegBuff;

    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if (u16Marker != 0x002A)
        return FALSE;

    //offset to 1st MPO IFD
    pJpegBuff += 2;
    u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));

    //1st MPO IFD start from here(either MP Index IFD or MP Attribute IFD)
    //Count Info
    pJpegBuff = pMPOHdrPtr + u32Len;
    JPEG_OVER_BUFFER_RET(pJpegBuff, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);

    u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));

    JPEG_DEBUG_API_MSG("num of entry = %d\n", u16NumOfEntry);

    //MPFVersion
    pJpegBuff += 2;
    u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
    if(u16Marker == JPEG_MPO_TAG_MPF_VERSION)
    {
        pJpegBuff += 12;
        u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
        if(u16Marker == JPEG_MPO_TAG_NUM_OF_IMAGES)
        {
            //It is a MP Index IFD
            //Check number of images
            pJpegBuff += 8;
            u32tmp = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));

            if(JPEG_MPO_MAX_SUPPORT_IMAGE < u32tmp)
            {
                JPEG_DEBUG_API_MSG("Not support %ld images in MPO!!\n", u32tmp);
                _stMPOIndex.num_of_image = 0xFFFFFFFF;
                return FALSE;
            }

            //Init MPO database
            JPEG_InitMPO();

            //start of offset for every individual picture
            _stMPOIndex.start_of_offset = _u32App2MarkerOffset + 8 - MRC_BUFFER_ADDR;

            //the numbers of image
            _stMPOIndex.num_of_image = u32tmp;

            printf_red("detected _stMPOIndex.num_of_image=0x%lx\n", _stMPOIndex.num_of_image);
            //MP Entry
            pJpegBuff += 4;
            u16Marker = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
            if(u16Marker != JPEG_MPO_TAG_MP_ENTRY)
            {
                JPEG_DEBUG_API_MSG("No MP Entry in MP index IFD!!\n");
                return FALSE;
            }

            //offset to next IFD(MP Attribute IFD)
            pJpegBuff = pMPOHdrPtr + 10 + u16NumOfEntry*12;
            u32Len = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
            JPEG_DEBUG_API_MSG("pJpegBuff = %lx, u32Len = %ld\n", (MS_U32)pJpegBuff, u32Len);

            //MP Entry Internal Fields
            pJpegBuff += 4;
            for(i = 0; i < _stMPOIndex.num_of_image; i++)
            {
                //Image Attribute
                _stMPOIndex.mp_entry[i].attribute = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex.mp_entry[i].size = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex.mp_entry[i].offset = JPEG_Tiff_EndianChangeL(JPEG_GetU32(pJpegBuff));
                //Image size
                pJpegBuff += 4;
                _stMPOIndex.mp_entry[i].image1_no = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
                //Image size
                pJpegBuff += 2;
                _stMPOIndex.mp_entry[i].image2_no = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));
                pJpegBuff += 2;
                JPEG_DEBUG_API_MSG("%d th : attribute = %lx\n", i, _stMPOIndex.mp_entry[i].attribute);
                JPEG_DEBUG_API_MSG("%d th : size = %lx\n", i, _stMPOIndex.mp_entry[i].size);
                JPEG_DEBUG_API_MSG("%d th : offset = %lx\n", i, _stMPOIndex.mp_entry[i].offset);
                JPEG_DEBUG_API_MSG("%d th : image1_no = %x\n", i, _stMPOIndex.mp_entry[i].image1_no);
                JPEG_DEBUG_API_MSG("%d th : image2_no = %x\n", i, _stMPOIndex.mp_entry[i].image2_no);
            }

            //get MPO Attribute count
            pJpegBuff = pMPOHdrPtr + u32Len;
            JPEG_OVER_BUFFER_RET(pJpegBuff, INTERNAL_BUFFER_ADDR, INTERNAL_BUFFER_SIZE);

            u16NumOfEntry = JPEG_Tiff_EndianChangeS(JPEG_GetU16(pJpegBuff));

            pJpegBuff += 2;
        }
    }

    //Decode MP Attr header
    return JPEG_DecodeMPAttrIFD(pJpegBuff, pMPOHdrPtr, u16NumOfEntry);
}


#endif

// Parse APP0 to get X&Y density.
static MS_BOOL JPEG_read_app0_marker( void )
{

    MS_U32 left;
    MS_U8 u8Xthumbnail, u8Ythumbnail;
    JPEG_DEBUG_API_MSG("APP0\n");

    //Get APP0 length
    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 2;

    if(left < 8)
    {
        JPEG_terminate( E_JPEG_BAD_APP0_MARKER );
        return FALSE;
    }

    //Skip Block ID "JFIF"
    JPEG_skip_bytes(5);

    left-= 5;

    //Skip Version Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Get units
    App0_Unit_Data.u8Unit = JPEG_get_char();
    App0_Unit_Data.u16Xdensity= (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    App0_Unit_Data.u16Ydensity= (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 5;

    //Get thumbnail x,y
    u8Xthumbnail = JPEG_get_char();
    u8Ythumbnail = JPEG_get_char();

    left-= 2;

    //Skip Version Number
    JPEG_skip_bytes(3*u8Xthumbnail*u8Ythumbnail);

    left-= 3*u8Xthumbnail*u8Ythumbnail;

    JPEG_DEBUG_API_MSG("APP0 : unit : %d\n",(MS_U16)App0_Unit_Data.u8Unit);
    JPEG_DEBUG_API_MSG("APP0 : Xdensity : %d\n",App0_Unit_Data.u16Xdensity);
    JPEG_DEBUG_API_MSG("APP0 : Ydensity : %d\n",App0_Unit_Data.u16Ydensity);

    if(left > 0)
        JPEG_skip_bytes(left);

    return TRUE;
}

void _msAPI_JPEG_get_APP0_info( MS_U8 *unit, MS_U16 *x, MS_U16 *y )
{
    *unit = App0_Unit_Data.u8Unit;
    *x = App0_Unit_Data.u16Xdensity;
    *y = App0_Unit_Data.u16Ydensity;
}
//------------------------------------------------------------------------------
// Read exif info
JPEG_STATIC MS_BOOL JPEG_read_app1_marker(void)
{
    MS_U16 length;
    MS_U8 *exif_buffer = (MS_U8 *) INTERNAL_BUFFER_ADDR;
    MS_U16 i = 0;

    JPEG_DEBUG_API_MSG("APP1\n");

    length = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( length < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
        return FALSE;
    }

    length -= 2;

#if SUPPORT_MPO_FORMAT
    u32MPFOffset = length;
    JPEG_DEBUG_API_MSG("u32MPFOffset=0x%lx\n", u32MPFOffset);
#endif
    while((length - i)!=0)
    {
        exif_buffer[i] = (MS_U8) JPEG_get_char();
        i++;
    }

    if(JPEG_DecodeExifInfo(exif_buffer, length) == TRUE)
    {
        JPEG_DEBUG_API_MSG("FOUND THUMBNAIL!\n");
        _u32ThumbnailBufferOffset = _u32ThumbnailOffset;
        _u16ThumbnailBufferSize = _u16ThumbnailSize;
    }
    else
    {
        JPEG_DEBUG_API_MSG("NO THUMBNAIL!\n");
    }
    return TRUE;
}

#if SUPPORT_MPO_FORMAT

// Read exif app2 info
static MS_BOOL JPEG_read_app2_marker( void )
{
    MS_U16 length;
    MS_U8 *exif_buffer = (MS_U8 *) (INTERNAL_BUFFER_ADDR + u32MPFOffset);
    MS_U16 i = 0;

    JPEG_DEBUG_API_MSG("APP2\n");

    //For start of offset of MPO(only for first MPO picture)
    JPEG_DEBUG_API_MSG("_pu8In_buf_ofs = %lx\n", (MS_U32)_pu8In_buf_ofs);
    _u32App2MarkerOffset = (MS_U32)_pu8In_buf_ofs - 2;

    length = (MS_U16)((JPEG_get_char()<<8)+JPEG_get_char());

    if ( length < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_APP2_MARKER );
        return FALSE;
    }

    length -= 2;

    if(length > INTERNAL_BUFFER_SIZE - u32MPFOffset)
    {
        JPEG_terminate( E_JPEG_NOTENOUGHMEM );
        return FALSE;
    }

    while((length - i)!=0)
    {
        exif_buffer[i] = (MS_U8) JPEG_get_char();
        i++;
    }

    if(JPEG_DecodeMPOInfo(exif_buffer)==TRUE)
    {
        JPEG_DEBUG_API_MSG("FOUND MPO!\n");
        bIsMPOFormat = TRUE;
        u32MPFOffset = 0;           // Reset to zero after FOUND MPO

        //// TODO: MPO format not support thumbnail now....
        //_u8DecodeType = E_JPEG_TYPE_MAIN;
        //printf_red("NOTE: MPO not support thumbnail now!!!\n");
    }
    else
    {
        JPEG_DEBUG_API_MSG("NO MPO!\n");
        // Not clear num_of_image for the image with 2 app2 marker
        // _stMPOIndex.num_of_image = 0;
    }
    return TRUE;
}


#endif

// Read app14 info
static MS_BOOL JPEG_read_app14_marker( void )
{

    MS_U32 left;
    MS_U8 u8Transform;

    JPEG_DEBUG_API_MSG("APP14\n");

    //Get APP14 length
    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    left-= 2;

    if(left < 8)
    {
        JPEG_terminate( E_JPEG_BAD_APP0_MARKER );
        return FALSE;
    }

    //Skip Block ID "Adobe"
    JPEG_skip_bytes(5);

    left-= 5;

    //Skip Version Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Skip F0 Number
    JPEG_skip_bytes(2);

    left-= 2;

    //Skip F1 Number
    JPEG_skip_bytes(2);

    left-= 2;

    u8Transform=  JPEG_get_char();

    left-= 1;

    if(u8Transform!=1)
    {
        // 0: RGB or CMYK, depends on SOF
        // 1: YCbCr
        // 2: YCCK
        JPEG_DEBUG_API_MSG("Unsupported Adobe Format with Transform=%d\n", u8Transform);
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    if(left > 0)
        JPEG_skip_bytes(left);

    return TRUE;
}

/* END OF EXIF PARSING SECTION */
//------------------------------------------------------------------------------
// Read a Huffman code table.
JPEG_STATIC MS_BOOL JPEG_read_dht_marker(void)
{
    MS_U16 i, index, count;
    MS_U32 left;
    MS_U8 u8Huff_num[17];
    MS_U8 u8Huff_val[256];

    JPEG_DEBUG_API_MSG("DHT\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
        return FALSE;
    }

    left -= 2;

    while ( left )
    {
        // set it to zero, initialize
        JPEG_memset((void *)u8Huff_num, 0, 17);
        JPEG_memset((void *)u8Huff_val, 0, 256);

        index = JPEG_get_char();

        u8Huff_num[0] = 0;

        count = 0;

        for ( i = 1; i <= 16; i++ )
        {
            u8Huff_num[i] = JPEG_get_char();
            count += u8Huff_num[i];
        }

        if ( count > 255 )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_COUNTS );
            return FALSE;
        }

        for ( i = 0; i < count; i++ )
        {
            u8Huff_val[i] = JPEG_get_char();
        }

        i = 1 + 16 + count;

        if ( left < ( MS_U32 )i )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_MARKER );
            return FALSE;
        }

        left -= i;

        index = ( index & 0x0F ) + ( ( index & 0x10 ) >> 4 ) * ( JPEG_MAXHUFFTABLES >> 1 ); //???

        if ( index >= JPEG_MAXHUFFTABLES )
        {
            JPEG_terminate( E_JPEG_BAD_DHT_INDEX );
            return FALSE;
        }

        if(_Huff_info[index].bValid==FALSE)
            _Huff_info[index].bValid = TRUE;

        JPEG_memcpy( (void *)_Huff_info[index].u8Huff_num, (void *)u8Huff_num, 17 );
        JPEG_memcpy( (void *)_Huff_info[index].u8Huff_val, (void *)u8Huff_val, 256 );

        // Compute the inverse order of HuffNum. this step is only needed in JPD mode (baseline)
        for(i = 1; i<=16; i++)
        {
            if(u8Huff_num[17 - i]!=0)
            {
                count = count - u8Huff_num[17 - i];
                u8Huff_num[17 - i] = count;
            }
            else
                u8Huff_num[17 - i] = 0xFF;
        }
        JPEG_memcpy( (void *)_Huff_info[index].u8Symbol, (void *)u8Huff_num, 17 );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a quantization table.
JPEG_STATIC MS_BOOL JPEG_read_dqt_marker(void)
{
    MS_U16 n, i, prec;
    MS_U32 left;
    MS_U32 temp;

    JPEG_DEBUG_API_MSG("DQT\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_DQT_MARKER );
        return FALSE;
    }

    left -= 2;

    while ( left )
    {
        n = JPEG_get_char();
        prec = n >> 4;
        n &= 0x0F;

        if ( n >= JPEG_MAXQUANTTABLES )
        {
            JPEG_terminate( E_JPEG_BAD_DQT_TABLE );
            return FALSE;
        }

        if(_QuantTables[n].bValid == FALSE)
            _QuantTables[n].bValid = TRUE;

        // read quantization entries, in zag order
        for ( i = 0; i < 64; i++ )
        {
            temp = JPEG_get_char();

            if ( prec )
            {
                temp = ( temp << 8 ) + JPEG_get_char();
            }

            _QuantTables[n].s16Value[i] = temp;
        }

        i = 64 + 1;

        if ( prec )
        {
            i += 64;
        }

        if ( left < ( MS_U32 )i )
        {
            JPEG_terminate( E_JPEG_BAD_DQT_LENGTH );
            return FALSE;
        }

        left -= i;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Read the start of frame (SOF) marker.
JPEG_STATIC MS_BOOL JPEG_read_sof_marker(void)
{
    MS_U8 i;
    MS_U32 left;
    MS_U16 JpegMaxWidth, JpegMaxHeight;
    MS_U8 c1;

    JPEG_DEBUG_API_MSG("SOF\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    if ( JPEG_get_char() != 8 )   /* precision: sorry, only 8-bit precision is supported right now */
    {
        JPEG_terminate( E_JPEG_BAD_PRECISION );
        return FALSE;
    }

    _u16Image_y_size = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    _u16Image_x_size = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    // save the original image size
    _u16OriginalImage_x_size = _u16Image_x_size;
    _u16OriginalImage_y_size = _u16Image_y_size;

    if((_bProgressive_flag)
    && (E_JPEG_TYPE_MAIN == _u8DecodeType))
    {
       //Define MAX picture size for progressive, 8 is the max factor of scale down
        JpegMaxWidth  = JPEG_PRO_MAX_WIDTH * 8;
        JpegMaxHeight = JPEG_PRO_MAX_HEIGHT * 8;

        //Calculate Internal Buffer size
        //check needed buffer for progressive jpeg decoding, for worst case:
        MS_U32 buff_size = _u16Image_x_size*_u16Image_y_size*3*2
                         + _u16Image_x_size*_u16Image_y_size*3*2/64
                         + JPEG_DEFAULT_EXIF_SIZE
                         + 8*1024;
//        if( (MS_U32)(_u16Image_x_size*_u16Image_y_size*3*2) > (MS_U32)(INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE) )
        if( buff_size > INTERNAL_BUFFER_SIZE )
        {
            JPEG_DEBUG_API_ERR("Progressive image size too big... do not handle it\n");
            JPEG_terminate( E_JPEG_NOTENOUGHMEM );
            return FALSE;
        }
    }
    else
    {
        //Define MAX picture size for Baseline, 8 is the max factor of scale down
        JpegMaxWidth  = JPEG_MAX_WIDTH*8;
        JpegMaxHeight = JPEG_MAX_HEIGHT*8;
    }

    if ((_u16Image_y_size < 1) || (_u16Image_y_size > JpegMaxHeight))
    {
        JPEG_terminate(E_JPEG_BAD_HEIGHT);
        return FALSE;
    }

    if ((_u16Image_x_size < 1) || (_u16Image_x_size > JpegMaxWidth))
    {
        JPEG_terminate(E_JPEG_BAD_WIDTH);
        return FALSE;
    }

    _u8Comps_in_frame = JPEG_get_char();

    if ( _u8Comps_in_frame > JPEG_MAXCOMPONENTS )
    {
        JPEG_terminate( E_JPEG_TOO_MANY_COMPONENTS );
        return FALSE;
    }

    if ( left != ( MS_U32 )( _u8Comps_in_frame * 3 + 8 ) )
    {
        JPEG_terminate( E_JPEG_BAD_SOF_LENGTH );
        return FALSE;
    }

    for ( i = 0; i < _u8Comps_in_frame; i++ )
    {
        _u8Comp_ident[i] = JPEG_get_char();
        if(_u8Comp_ident[i]==0)     // The component ID is start from 0 (0 1 2). The normal case is start from 1 (1 2 3) for YUV
        {
            _u8LumaCi = 0;
            _u8ChromaCi = 1;
            _u8Chroma2Ci = 2;
        }

        c1 = JPEG_get_char();

        _u8Comp_h_samp[i] = (c1 & 0xf0)>>4;
        _u8Comp_v_samp[i] = (c1 & 0x0f);
        _u8Comp_quant[i] = JPEG_get_char();

        // patch from LG driver for the file with the following problem:
        // only has one component, but its sampling factor is 1x2
        // Per the JPEG spec A.2.2 (see the attached file, "regardless of the values of H1 and V1"),
        // please always set H=1 & V=1 to hw, when mono image.
        if(_u8Comps_in_frame==1)    //allen.chang 2011/6/16 patch
        {
            _u8Comp_h_samp[0] =1;
            _u8Comp_v_samp[0] =1;
        }

    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Used to skip unrecognized markers.
JPEG_STATIC MS_BOOL JPEG_skip_variable_marker(void)
{
    MS_U32 left;

    JPEG_DEBUG_API_MSG("SKIP markers\n");

    left = (MS_U32)((JPEG_get_char()<<8)|JPEG_get_char());

    if ( left < 2 )
    {
        JPEG_terminate( E_JPEG_BAD_VARIABLE_MARKER );
        return FALSE;
    }

    left -= 2;

    JPEG_skip_bytes(left);
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a define restart interval (DRI) marker.
JPEG_STATIC MS_BOOL JPEG_read_dri_marker(void)
{
    JPEG_DEBUG_API_MSG("DRI\n");

    if ( (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char()) != 4 )
    {
        JPEG_terminate( E_JPEG_BAD_DRI_LENGTH );
        return FALSE;
    }

    _u16Restart_interval = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());
    return TRUE;
}
//------------------------------------------------------------------------------
// Read a start of scan (SOS) marker.
JPEG_STATIC MS_BOOL JPEG_read_sos_marker(void)
{
    MS_U32 left;
    MS_U16 i, ci, n, c, cc;
    MS_U8 c1;

    JPEG_DEBUG_API_MSG("SOS\n");

    left = (MS_U16) ((JPEG_get_char()<<8) + JPEG_get_char());

    n = JPEG_get_char();

    _u8Comps_in_scan = n;

    left -= 3;

    if ( ( left != ( MS_U32 )( n * 2 + 3 ) ) || ( n < 1 ) || ( n > JPEG_MAXCOMPSINSCAN ) )
    {
        JPEG_terminate( E_JPEG_BAD_SOS_LENGTH );
        return FALSE;
    }

    for ( i = 0; i < n; i++ )
    {
        cc = JPEG_get_char();
        c = JPEG_get_char();
        left -= 2;

        for ( ci = 0; ci < _u8Comps_in_frame; ci++ )
        {
            if ( cc == _u8Comp_ident[ci] )
            {
                break;
            }
        }

        if ( ci >= _u8Comps_in_frame )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_COMP_ID );
            return FALSE;
        }

        _u8Comp_list[i] = ci;
        _u8Comp_dc_tab[ci] = ( c >> 4 ) & 15;
        _u8Comp_ac_tab[ci] = ( c & 15 ) + ( JPEG_MAXHUFFTABLES >> 1 );
    }

#ifndef CMODEL
    //HW limitation, for baseline JPEG, U.V need to refer to the same DC and AC huffman table.
    if(!_bProgressive_flag && (_u8Comps_in_frame == 3)) //Y.U.V.
    {
        if((_u8Comp_dc_tab[1] != _u8Comp_dc_tab[2])
        || (_u8Comp_ac_tab[1] != _u8Comp_ac_tab[2]))
        {
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
            JPEG_DEBUG_API_MSG("U, V use different Huffman table~~\n");
            bIs3HuffTbl = TRUE;
#else
            JPEG_terminate( E_JPEG_UNSUPPORTED_HUFF_DECODE );
            return FALSE;
#endif
        }
    }
#endif

    _u8Spectral_start = JPEG_get_char();
    _u8Spectral_end = JPEG_get_char();
    c1 = JPEG_get_char();
    _u8Successive_high = (c1 & 0xf0)>>4;
    _u8Successive_low = (c1 & 0x0f);

    if ( !_bProgressive_flag )
    {
        _u8Spectral_start = 0;
        _u8Spectral_end = 63;
    }

    left -= 3;

    JPEG_skip_bytes(left);    /* read past whatever is left */
    return TRUE;
}
//------------------------------------------------------------------------------
// Finds the next marker.
JPEG_STATIC MS_U32 JPEG_next_marker(void) //ok
{
    MS_U32 c, bytes;

    bytes = 0;

    do
    {
        do
        {
            bytes++;

            c = JPEG_get_char();
            //JPEG_DEBUG_API_MSG("c = %X\n",c);
        }
        while ( c != 0xFF );

        do
        {
            c = JPEG_get_char();
            //JPEG_DEBUG_API_MSG("c = %X\n",c);
        }
        while ( c == 0xFF );
    }
    while ( c == 0 );

    // If bytes > 0 here, there where extra bytes before the marker (not good).

    return c;
}
//------------------------------------------------------------------------------
// Process markers. Returns when an SOFx, SOI, EOI, or SOS marker is
// encountered.
JPEG_STATIC MS_U32 JPEG_process_markers(void)
{
    MS_U32 c;

    JPEG_DEBUG_API_MSG("JPEG_process_markers:\n");
    for ( ; ; )
    {
        c = JPEG_next_marker();

        switch ( c )
        {
            case E_JPEG_APP1:
                // Prevent from there's thumbnail in thumbnail... & multiple APP1
                // Although it's impossible.. =_=
//                if((E_JPEG_TYPE_THUMBNAIL == _u8DecodeType)
//                && (FALSE == _bThumbnailFound))
                // We need to get EXIF info for decoding main picture or thumbnail.
                if((__bIsMjpeg == FALSE)
                && (FALSE == _bThumbnailFound))
                {
                    if( !JPEG_read_app1_marker() )
                        return FALSE;
                }
                else
                {
                    if( !JPEG_skip_variable_marker() )
                        return FALSE;
                }
                break;
#if SUPPORT_MPO_FORMAT
            case E_JPEG_APP2:
                if( !JPEG_read_app2_marker() )
                    return FALSE;
                break;

#endif
            case E_JPEG_SOF0:
            case E_JPEG_SOF1:
            case E_JPEG_SOF2:
            case E_JPEG_SOF3:
            case E_JPEG_SOF5:
            case E_JPEG_SOF6:
            case E_JPEG_SOF7:
                //      case E_JPEG_JPG:
            case E_JPEG_SOF9:
            case E_JPEG_SOF10:
            case E_JPEG_SOF11:
            case E_JPEG_SOF13:
            case E_JPEG_SOF14:
            case E_JPEG_SOF15:
            case E_JPEG_SOI:
            case E_JPEG_EOI:
            case E_JPEG_SOS:
                {
                    return c;
                }
            case E_JPEG_DHT:
                {
                    if( !JPEG_read_dht_marker() )
                        return FALSE;
                    _HeadCheck.DHT = TRUE;
                    break;
                }
                // Sorry, no arithmitic support at this time. Dumb patents!
            case E_JPEG_DAC:
                {
                    JPEG_terminate( E_JPEG_NO_ARITHMETIC_SUPPORT );
                    return FALSE;
                    break;
                }
            case E_JPEG_DQT:
                {
                    if( !JPEG_read_dqt_marker() )
                        return FALSE;
                    _HeadCheck.DQT = TRUE;
                    break;
                }
            case E_JPEG_DRI:
                {
                    if( !JPEG_read_dri_marker() )
                        return FALSE;
                    break;
                }
                //case E_JPEG_APP0:  /* no need to read the JFIF marker */

            case E_JPEG_JPG:
            case E_JPEG_RST0:
                /* no parameters */
            case E_JPEG_RST1:
            case E_JPEG_RST2:
            case E_JPEG_RST3:
            case E_JPEG_RST4:
            case E_JPEG_RST5:
            case E_JPEG_RST6:
            case E_JPEG_RST7:
            case E_JPEG_TEM:
                {
                    JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
                    return FALSE;
                    break;
                }
            case E_JPEG_APP0:
                if (!bMHEG5)
                {
                    if(!JPEG_skip_variable_marker())
                    {
                        return FALSE;
                    }
                    break;
                }
                if(!JPEG_read_app0_marker())
                {
                    return FALSE;
                }
                break;
            case E_JPEG_APP14:
                if(__bIsMjpeg == FALSE)
                {
                    if( !JPEG_read_app14_marker() )
                        return FALSE;
                }
                else
                {
                    if( !JPEG_skip_variable_marker() )
                        return FALSE;
                }
                break;
            case E_JPEG_DNL:
            case E_JPEG_DHP:
            case E_JPEG_EXP:
#if (SUPPORT_MPO_FORMAT == 0)
            case E_JPEG_APP2:
#endif
            case E_JPEG_APP3:
            case E_JPEG_APP4:
            case E_JPEG_APP5:
            case E_JPEG_APP6:
            case E_JPEG_APP7:
            case E_JPEG_APP8:
            case E_JPEG_APP9:
            case E_JPEG_APP10:
            case E_JPEG_APP11:
            case E_JPEG_APP12:
            case E_JPEG_APP13:
            case E_JPEG_APP15:
            case E_JPEG_JPG0:
            case E_JPEG_JPG1:
            case E_JPEG_JPG2:
            case E_JPEG_JPG3:
            case E_JPEG_JPG4:
            case E_JPEG_JPG5:
            case E_JPEG_JPG6:
            case E_JPEG_JPG7:
            case E_JPEG_JPG8:
            case E_JPEG_JPG9:
            case E_JPEG_JPG10:
            case E_JPEG_JPG11:
            case E_JPEG_JPG12:
            case E_JPEG_JPG13:
            case E_JPEG_COM:
                /* must be DNL, DHP, EXP, APPn, JPGn, COM, or RESn or APP0 */
                {
                    if(!JPEG_skip_variable_marker())
                    {
                        return FALSE;
                    }
                    break;
                }
            default:
                break;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Finds the start of image (SOI) marker.
// This code is rather defensive: it only checks the first 512 bytes to avoid
// false positives.
JPEG_STATIC MS_BOOL JPEG_locate_soi_marker(void)
{
    MS_U32 lastchar, thischar;
    MS_U32 bytesleft;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    lastchar = JPEG_get_char();

    //JPEG_DEBUG_API_MSG("0x%lx\n", lastchar);
    thischar = JPEG_get_char();

    //JPEG_DEBUG_API_MSG("0x%lx\n", thischar);
    /* ok if it's a normal JPEG file without a special header */

    if ((0xFF == lastchar)
    && (E_JPEG_SOI == thischar))
    {
        //JPEG_DEBUG_API_MSG("SOI\n");
        return TRUE;
    }

    // Set this value to 0x1000 for 4k alignment MPO case when parse 2nd/3rd/... JPEG file
    bytesleft = 0x1000; //Fix this number from 512 -> 640 for some cases

    for ( ; ; )
    {
        if ( --bytesleft == 0 )
        {
            JPEG_terminate( E_JPEG_NOT_JPEG );
            return FALSE;
        }

        lastchar = thischar;

        thischar = JPEG_get_char();

        //JPEG_DEBUG_API_MSG("%x ", (MS_U8)thischar);
        //if(bytesleft%8==0)
        //    JPEG_DEBUG_API_MSG("\n");

        if ((0xFF == lastchar)
        && (E_JPEG_SOI == thischar))
        {
            //JPEG_DEBUG_API_MSG("SOI\n");
            break;
        }
    }

    /* Check the next character after marker: if it's not 0xFF, it can't
       be the start of the next marker, so it probably isn't a JPEG */
/*  need to check for more detail, currently _u32Bit_buf is not updated during JPEG_get_char()
    thischar = ( _u32Bit_buf >> 8 ) & 0xFF;

    if ( thischar != 0xFF )
    {
        JPEG_terminate( E_JPEG_NOT_JPEG );
        return FALSE;
    }
*/
    return TRUE;
}
//------------------------------------------------------------------------------
// Find a start of frame (SOF) marker.
JPEG_STATIC MS_BOOL JPEG_locate_sof_marker(void)
{
    MS_U32 c;
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    if(!JPEG_locate_soi_marker())
        return FALSE;

    c = JPEG_process_markers();

    if( c == FALSE )
        return FALSE;

    switch ( c )
    {
        case E_JPEG_SOF2:
            {
                JPEG_DEBUG_API_MSG("Progressive\n");
                _bProgressive_flag = TRUE;
                u32SOFOffset = u32DataOffset + JPEG_GetECS() - 2;
                if(!JPEG_read_sof_marker())
                    return FALSE;
                break;
            }
        case E_JPEG_SOF0:
            /* baseline DCT */
        case E_JPEG_SOF1:
            /* extended sequential DCT */
            {
                JPEG_DEBUG_API_MSG("Baseline\n");
                u32SOFOffset = u32DataOffset + JPEG_GetECS() - 2;
                if(!JPEG_read_sof_marker())
                    return FALSE;
                break;
            }
        case E_JPEG_SOF9:
            /* Arithmitic coding */
            {
                JPEG_terminate( E_JPEG_NO_ARITHMETIC_SUPPORT );
                return FALSE;
                break;
            }

        default:
            {
                JPEG_DEBUG_API_MSG("parsed unsupported marker = 0x%04lX\n", c);
                JPEG_terminate( E_JPEG_UNSUPPORTED_MARKER );
                return FALSE;
                break;
            }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Find a start of scan (SOS) marker.
JPEG_STATIC MS_BOOL JPEG_locate_sos_marker(void)
{
    MS_U32 c;

    c = JPEG_process_markers();

    if ( c == E_JPEG_EOI )
    {
        return FALSE;
    }
    else if ( c != E_JPEG_SOS )
    {
        JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
        return FALSE;
    }

    if(!JPEG_read_sos_marker())
        return FALSE;

    JPEG_DEBUG_API_MSG(" _pu8In_buf_MPO_ofs=0x%lx, _u32In_buf_MPO_left=0x%lx===========================\n", (MS_U32)_pu8In_buf_ofs, (MS_U32)_u32In_buf_left);
    JPEG_DEBUG_API_MSG("%02x %02x %02x %02x %02x %02x %02x %02x \n",
    *_pu8In_buf_ofs, *(_pu8In_buf_ofs+1), *(_pu8In_buf_ofs+2), *(_pu8In_buf_ofs+3),
    *(_pu8In_buf_ofs+4), *(_pu8In_buf_ofs+5), *(_pu8In_buf_ofs+6), *(_pu8In_buf_ofs+7)
    );

    return TRUE;
}
//------------------------------------------------------------------------------
// Reset thumbnail parameters
JPEG_STATIC void JPEG_init_thumbnail(void)
{
    _bThumbnailFound = FALSE;
    _u32ThumbnailOffset = 0;
    _u16ThumbnailSize = 0;
    _bTiffBigEndian = FALSE;

    _u32ThumbnailBufferOffset = 0;
    _u16ThumbnailSize = 0;
    _bThumbnailAccessMode = FALSE;

    _stEXIF_DateTime.bHasDataTime = FALSE;
    _stEXIF_DateTime.u32Year = 0;
    _stEXIF_DateTime.u32Month = 0;
    _stEXIF_DateTime.u32Day = 0;
    _stEXIF_DateTime.u32Hour = 0;
    _stEXIF_DateTime.u32Minute = 0;
    _stEXIF_DateTime.u32Second = 0;
    _eEXIF_Orientation = E_JPEG_EXIF_ORIENT_NOT_FOUND;
#if SUPPORT_EXIF_EXTRA_INFO
    JPEG_memset((void *)_u8EXIF_Manufacturer, 0, JPEG_MANUFACTURER_SIZE);
    JPEG_memset((void *)_u8EXIF_Model, 0, JPEG_MODEL_SIZE);
    _u16EXIF_Flash = 0;
    _u16EXIF_Exposureprogram = 0;
    _u32EXIF_ISOSpeedRatings = 0;
    _stEXIF_ShutterSpeedValue.numerator = 0;
    _stEXIF_ShutterSpeedValue.denominator= 0;
    _stEXIF_ApertureValue.s_numerator= 0;
    _stEXIF_ApertureValue.s_denominator= 0;
    _stEXIF_ExposureBiasValue.numerator= 0;
    _stEXIF_ExposureBiasValue.denominator= 0;
    _stEXIF_FocalLength.s_numerator= 0;
    _stEXIF_FocalLength.s_denominator= 0;
    _u32EXIF_ImageWidth = 0;
    _u32EXIF_ImageHeight = 0;
    _stEXIF_ExposureTime.numerator = 0;
    _stEXIF_ExposureTime.denominator =0;
    _stEXIF_FNumber.numerator = 0;
    _stEXIF_FNumber.denominator =0;
#endif
}
//------------------------------------------------------------------------------
// Reset everything to default/uninitialized state.
JPEG_STATIC MS_BOOL JPEG_init(void)
{
    MS_U16 i;
    JPEG_DEBUG_API_MSG("%s!!\n", __FUNCTION__);
    _u8LumaCi = 1;
    _u8ChromaCi = 2;
    _u8Chroma2Ci = 3;
    bIs3HuffTbl = FALSE;

    _Error_code = E_JPEG_NO_ERROR;

#ifdef CMODEL
    _bReady_flag = FALSE;
#endif

    _u16Image_x_size = _u16Image_y_size = 0;
    _u16OriginalImage_x_size = _u16OriginalImage_y_size = 0;
    _u16AlignedImageWidth = _u16AlignedImagePitch = _u16AlignedImageHeight = 0;
    _u16AlignedImagePitch_H = 0;

////    _pStream = _Pstream;

    _bProgressive_flag = FALSE;
#ifndef CMODEL
    _u8DownScaleRatio = E_JPD_DOWNSCALE_ORG;
#endif
    _u32RLEOffset = 0;
    _bFirstRLE = TRUE;

    _s16dc_pred[0] = _s16dc_pred[1] = _s16dc_pred[2] = 0;

    _JPD_IsDecoding = FALSE; //For H/W bug, some cases can not exit after decode done.
    _JPD_ReCheckTime = 0; //For H/W bug, some cases can not exit after decode done, record repeat time.
    _JPD_PreVIdx = 0;     //For H/W bug, some cases can not exit after decode done, record previous Vidx.

    _Progressive_ROI_flag = FALSE; //CL82399
    ROI_width = 0; //CL82399

    u8PreLHFlag = E_JPEG_BUFFER_NONE;

    u32MRCheckCount = 0;

    for(i = 0; i<JPEG_MAXHUFFTABLES; i++)
    {
        _Huff_info[i].bValid = FALSE;
        JPEG_memset((void *)_Huff_info[i].u8Huff_num, 0, 17);
        JPEG_memset((void *)_Huff_info[i].u8Huff_val, 0, 256);
        JPEG_memset((void *)_Huff_info[i].u8Symbol, 0, 17);
        JPEG_memset((void *)_Huff_info[i].u16Code, 0, 17);
    }

    for(i = 0; i<JPEG_MAXQUANTTABLES; i++)
    {
        _QuantTables[i].bValid = FALSE;
        JPEG_memset(_QuantTables[i].s16Value, 0, 64);
    }

    gu8Scan_type = E_JPEG_GRAYSCALE;

    _u8Comps_in_frame = 0;

    JPEG_memset((void *)_u8Comp_h_samp, 0, sizeof( _u8Comp_h_samp ) );
    JPEG_memset((void *)_u8Comp_v_samp, 0, sizeof( _u8Comp_v_samp ) );
    JPEG_memset((void *)_u8Comp_quant, 0, sizeof( _u8Comp_quant ) );
    JPEG_memset((void *)_u8Comp_ident, 0, sizeof( _u8Comp_ident ) );
    JPEG_memset((void *)_u16Comp_h_blocks, 0, sizeof( _u16Comp_h_blocks ) );
    JPEG_memset((void *)_u16Comp_v_blocks, 0, sizeof( _u16Comp_v_blocks ) );

    _u8Comps_in_scan = 0;
    JPEG_memset((void *)_u8Comp_list, 0, sizeof( _u8Comp_list ) );
    JPEG_memset((void *)_u8Comp_dc_tab, 0, sizeof( _u8Comp_dc_tab ) );
    JPEG_memset((void *)_u8Comp_ac_tab, 0, sizeof( _u8Comp_ac_tab ) );

    _u8Spectral_start = 0;
    _u8Spectral_end = 0;
    _u8Successive_low = 0;
    _u8Successive_high = 0;

    gu8Max_mcu_x_size = 0;
    gu8Max_mcu_y_size = 0;

    _u8Blocks_per_mcu = 0;
    _u32Max_blocks_per_row = 0;
    _u16Mcus_per_row = 0;
    _u16Mcus_per_col = 0;

    JPEG_memset((void *)_u8Mcu_org, 0, sizeof( _u8Mcu_org ) );

#ifdef CMODEL
    gu16Real_dest_bytes_per_scan_line = 0;
    gu16Dest_bytes_per_scan_line = 0;
    gu8Dest_bytes_per_pixel = 0;
#endif

    JPEG_memset((void *)_pBlocks, 0, sizeof( _pBlocks ) );

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    _u16Total_lines_left = 0;
#ifdef CMODEL
    gu16Mcu_lines_left = 0;
#endif
    JPEG_memset((void *)_u32Block_y_mcu, 0, sizeof( _u32Block_y_mcu ) );
    JPEG_memset((void *)_Huff_tbls, 0, sizeof( _Huff_tbls ) );
    JPEG_memset((void *)_DC_Coeffs, 0, sizeof( _DC_Coeffs ) );
    JPEG_memset((void *)_AC_Coeffs, 0, sizeof( _AC_Coeffs ) );
    JPEG_memset((void *)_u32Last_dc_val, 0, sizeof( _u32Last_dc_val ) );

    _u32EOB_run = 0;
#endif

    _pu8In_buf_ofs = _pu8In_buf;
////    _u32In_buf_left = 0;
////    _bEOF_flag = FALSE;
    _u8Tem_flag = 0;

    //sharon JPEG_memset((void *)_pu8In_buf, 0, sizeof(MS_U8)*(MRC_BUFFER_SIZE + 128) );

    _u16Restart_interval = 0;
    _u16Restarts_left = 0;
    _u16Next_restart_num = 0;

    gu16Max_mcus_per_row = 0;
    _u16Max_blocks_per_mcu = 0;
    _u16Max_mcus_per_col = 0;

#ifdef CMODEL
    JPEG_memset((void *)gps16Block_seg, 0, sizeof( gps16Block_seg ) );
    gpu8Sample_buf = NULL;
#endif

#if SW_JPD_RGB_CMYK
    JPEG_memset( _ps16Block_seg, 0, sizeof( _ps16Block_seg ) );
    gpu8Sample_buf = NULL;
#endif

    // Tell the stream we're going to use it.
    //_pStream->attach();

    // Ready the input buffer.
    if(_bThumbnailAccessMode)
    {
        // It means that thumbnail is found and re-call JPEG_init()
        // to re-fill thumbnail data to internal buffer.
        if(!JPEG_fill_read_buffer())
        {
            JPEG_DEBUG_API_MSG("JPEG_init : JPEG_fill_read_buffer failed!!!\n");
            return FALSE;
        }
    }
    else
    {
////        _u32In_buf_left = MRC_BUFFER_SIZE;  //sharon
////        _Total_Decoded_Size = MRC_BUFFER_SIZE;  //sharon
////        _u32Total_bytes_read = MRC_BUFFER_SIZE;  //sharon
        _u32Total_bytes_read = _u32In_buf_left;
    }

    // Prime the bit buffer.
    _s16Bits_left = 0;
    _u32Bit_buf = 0;

//    _pu32ExifHeaderAddr = 0;

#ifdef CMODEL
    for ( i = 0; i < JPEG_MAXBLOCKSPERROW; i++ )
    {
        gu8Block_max_zag_set[i] = 64;
    }
#endif

#if SW_JPD_RGB_CMYK
    for ( i = 0; i < JPEG_MAXBLOCKSPERROW; i++ )
    {
        _u8Block_max_zag_set[i] = 64;
    }
#endif
    return TRUE;
}
//------------------------------------------------------------------------------
// The coeff_buf series of methods originally stored the coefficients
// into a "virtual" file which was located in EMS, XMS, or a disk file. A cache
// was used to make this process more efficient. Now, we can store the entire
// thing in RAM.
JPEG_STATIC PJPEG_CoeffBuf JPEG_coeff_buf_open(MS_U16 block_num_x, MS_U16 block_num_y, MS_U8 block_len_x, MS_U8 block_len_y)
{
    PJPEG_CoeffBuf cb = ( PJPEG_CoeffBuf )JPEG_alloc( sizeof( JPEG_CoeffBuf ) );

    if(cb == NULL)
        return NULL;

    cb->u16Block_num_x = block_num_x;
    cb->u16Block_num_y = block_num_y;

    cb->u8Block_len_x = block_len_x;
    cb->u8Block_len_y = block_len_y;

    cb->u16Block_size = ( block_len_x * block_len_y ) * sizeof( JPEG_BLOCK_TYPE );

    cb->pu8Data = ( MS_U8 * )JPEG_alloc( cb->u16Block_size * block_num_x * block_num_y );

    if(cb->pu8Data == NULL)
        return NULL;

    return cb;
}
//------------------------------------------------------------------------------
JPEG_STATIC JPEG_BLOCK_TYPE * JPEG_coeff_buf_getp( PJPEG_CoeffBuf cb, MS_U16 block_x, MS_U16 block_y )
{
    if ( block_x >= cb->u16Block_num_x )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return NULL;
    }

    if ( block_y >= cb->u16Block_num_y )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return NULL;
    }

    return ( JPEG_BLOCK_TYPE * )((MS_U32)( cb->pu8Data + block_x * cb->u16Block_size
                                            + block_y * (cb->u16Block_size * cb->u16Block_num_x)));
}
//------------------------------------------------------------------------------
// Creates the tables needed for efficient Huffman decoding.
JPEG_STATIC MS_BOOL JPEG_make_huff_table(MS_U8 index)
{
    MS_U16 p, i, l, si;
    MS_U8 huffsize[257];
    MS_U16 huffcode[257];
    MS_U16 _code;
    MS_U16 subtree;
    MS_U16 code_size;
    MS_U16 lastp;
#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    MS_S16 nextfreeentry;
#endif
    MS_S16 currententry;

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    JPEG_HuffTbl *hs = &_Huff_tbls[index];
#endif

    JPEG_memset((void *)huffsize, 0, sizeof(huffsize));
    JPEG_memset((void *)huffcode, 0, sizeof(huffcode));

    p = 0;

    for ( l = 1; l <= 16; l++ )
    {
        for ( i = 1; i <= _Huff_info[index].u8Huff_num[l]; i++ )
        {
            huffsize[p++] = l;

            //kevinhuang, add protection
            if ( p >= 257 )
            {
                JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
                return FALSE;
            }
        }
    }

    huffsize[p] = 0;

    lastp = p;

    _code = 0;
    si = huffsize[0];
    p = 0;

    while ( huffsize[p] )
    {
        while ( huffsize[p] == si )
        {
            huffcode[p++] = _code;
            _code++;

            //kevinhuang, add protection
            if ( p >= 257 )
            {
                JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
                return FALSE;
            }
        }

        _code <<= 1;
        si++;
    }

    // Calculate the min code
    for(i = 1; i<=16; i++)
        _Huff_info[index].u16Code[i] = huffcode[_Huff_info[index].u8Symbol[i]] << (15 - (i - 1));


#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    // In JPD mode, SW doesn't need huff table when baseline decoding
#if SW_JPD_RGB_CMYK
    if((E_JPEG_CMYK != gu8Scan_type)
    && (E_JPEG_RGB != gu8Scan_type))
#endif
    {
#ifndef CMODEL
        if(_bProgressive_flag==FALSE)
            return TRUE;
#endif
    }

    JPEG_DEBUG_API_MSG("Make HUFF TABLE\n");

    JPEG_memset((void *)(hs->s16Look_up), 0, sizeof( hs->s16Look_up ) );
    JPEG_memset((void *)(hs->s16Tree), 0, sizeof( hs->s16Tree ) );
    JPEG_memset((void *)(hs->u8Code_size), 0, sizeof( hs->u8Code_size ) );

    nextfreeentry = -1;

    p = 0;

    while ( p < lastp )
    {
        i = _Huff_info[index].u8Huff_val[p];
        _code = huffcode[p];
        code_size = huffsize[p];

        hs->u8Code_size[i] = code_size;

        if ( code_size <= 8 )
        {
            _code <<= ( 8 - code_size );

            for ( l = 1 << ( 8 - code_size ); l > 0; l-- )
            {
                hs->s16Look_up[_code] = i;
                _code++;
            }
        }
        else
        {
            subtree = ( _code >> ( code_size - 8 ) ) & 0xFF;

            currententry = hs->s16Look_up[subtree];

            if ( currententry == 0 )
            {
                hs->s16Look_up[subtree] = currententry = nextfreeentry;

                nextfreeentry -= 2;
            }

            _code <<= ( 16 - ( code_size - 8 ) );

            for ( l = code_size; l > 9; l-- )
            {
                if ( ( _code & 0x8000 ) == 0 )
                {
                    currententry--;
                }

                if ( hs->s16Tree[-currententry - 1] == 0 )
                {
                    hs->s16Tree[-currententry - 1] = nextfreeentry;

                    currententry = nextfreeentry;

                    nextfreeentry -= 2;
                }
                else
                {
                    currententry = hs->s16Tree[-currententry - 1];
                }

                _code <<= 1;
            }

            if ( ( _code & 0x8000 ) == 0 )
            {
                currententry--;
            }

            hs->s16Tree[-currententry - 1] = i;
        }

        p++;
    }
#endif  //SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    return TRUE;
}
//------------------------------------------------------------------------------
// Verifies the quantization tables needed for this scan are available.
JPEG_STATIC MS_BOOL JPEG_check_quant_tables( void ) //ok
{
    MS_U8 i;

    for ( i = 0; i < _u8Comps_in_scan; i++ )
    {
        if ( _QuantTables[_u8Comp_quant[_u8Comp_list[i]]].bValid==FALSE )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_QUANT_TABLE );
            return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Verifies that all the Huffman tables needed for this scan are available.
JPEG_STATIC MS_BOOL JPEG_check_huff_tables( void )
{
    MS_U8 i;

    for ( i = 0; i < _u8Comps_in_scan; i++ )
    {
        if ( ( _u8Spectral_start == 0 ) && ( _Huff_info[_u8Comp_dc_tab[_u8Comp_list[i]]].bValid== FALSE ) )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
            return FALSE;
        }

        if ( ( _u8Spectral_end > 0 ) && ( _Huff_info[_u8Comp_ac_tab[_u8Comp_list[i]]].bValid== FALSE ) )
        {
            JPEG_terminate( E_JPEG_UNDEFINED_HUFF_TABLE );
            return FALSE;
        }
    }

    for ( i = 0; i < JPEG_MAXHUFFTABLES; i++ )
    {
        if ( _Huff_info[i].bValid )
        {
            if(!JPEG_make_huff_table(i))
                return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Determines the component order inside each MCU.
// Also calcs how many MCU's are on each row, etc.
JPEG_STATIC void JPEG_calc_mcu_block_order( void ) //ok
{
    MS_U8 component_num, component_id;
    MS_U8 max_h_samp = 0, max_v_samp = 0;

    for ( component_id = 0; component_id < _u8Comps_in_frame; component_id++ )
    {
        if ( _u8Comp_h_samp[component_id] > max_h_samp )
        {
            max_h_samp = _u8Comp_h_samp[component_id];
        }

        if ( _u8Comp_v_samp[component_id] > max_v_samp )
        {
            max_v_samp = _u8Comp_v_samp[component_id];
        }
    }

    if((max_h_samp == 0) || (max_v_samp == 0))
    {
        JPEG_terminate( E_JPEG_NOT_ENOUGH_HEADER_INFO );
        return;
    }

    for ( component_id = 0; component_id < _u8Comps_in_frame; component_id++ )
    {
        _u16Comp_h_blocks[component_id] = ( ( ( ( _u16Image_x_size * _u8Comp_h_samp[component_id] ) + ( max_h_samp - 1 ) ) / max_h_samp ) + 7 ) / 8;
        _u16Comp_v_blocks[component_id] = ( ( ( ( _u16Image_y_size * _u8Comp_v_samp[component_id] ) + ( max_v_samp - 1 ) ) / max_v_samp ) + 7 ) / 8;
    }

    if ( _u8Comps_in_scan == 1 )
    {
        _u16Mcus_per_row = _u16Comp_h_blocks[_u8Comp_list[0]];
        _u16Mcus_per_col = _u16Comp_v_blocks[_u8Comp_list[0]];
    }
    else
    {
        _u16Mcus_per_row = ( ( ( _u16Image_x_size + 7 ) / 8 ) + ( max_h_samp - 1 ) ) / max_h_samp;
        _u16Mcus_per_col = ( ( ( _u16Image_y_size + 7 ) / 8 ) + ( max_v_samp - 1 ) ) / max_v_samp;
    }

    if ( _u8Comps_in_scan == 1 )
    {
        _u8Mcu_org[0] = _u8Comp_list[0];

        _u8Blocks_per_mcu = 1;
    }
    else
    {
        _u8Blocks_per_mcu = 0;

        for ( component_num = 0; component_num < _u8Comps_in_scan; component_num++ )
        {
            MS_U8 num_blocks;

            component_id = _u8Comp_list[component_num];

            num_blocks = _u8Comp_h_samp[component_id] * _u8Comp_v_samp[component_id];

            while ( num_blocks-- )
            {
                _u8Mcu_org[_u8Blocks_per_mcu++] = component_id;
            }
        }
    }
}
//------------------------------------------------------------------------------
/* Get current access byte address in MRC buffer relative to MRC start address */
JPEG_STATIC MS_U32 JPEG_GetECS(void)
{
    MS_U32 data_end_addr = (MS_U32)_pu8In_buf_ofs;
    MS_U32 data_start_addr =(MS_U32) _pu8In_buf;

    return (data_end_addr - data_start_addr);
}
//------------------------------------------------------------------------------
//*************************************************
//write symbol table
//*************************************************
#ifndef CMODEL
#if 0
JPEG_STATIC void JPEG_write_symidx(void)
{
    MS_U16 i, tbl_num_luma, tbl_num_chroma;
    MS_U8 ci, luma_ci = 0, chroma_ci = 0;

    MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_SYMIDX_BASE);
    if (_HeadCheck.DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];

        for ( i = 0; i < 256; i++ )
        {
            MDrv_Write2Byte( BK_JPD_TID_DAT, ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ) );
        }

        tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];

        for ( i = 0; i < 16; i++ )
        {
            MDrv_Write2Byte( BK_JPD_TID_DAT, ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ) );
        }
    }
    else
    {
        for(i=0;i<272;i++)
            MDrv_Write2Byte( BK_JPD_TID_DAT, g16SYMIDX_TBL[i]);
    }
}
#endif
//------------------------------------------------------------------------------
JPEG_STATIC void JPEG_WriteSymidx(void)
{
    //MS_U16 i, tbl_num_luma, tbl_num_chroma;
    MS_U16 ac_tbl_num_luma = 0, ac_tbl_num_chroma = 0;
    MS_U16 dc_tbl_num_luma = 0, dc_tbl_num_chroma = 0;
    MS_U8 ci, luma_ci = 0, chroma_ci = 0;
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
    MS_U16 ac_tbl_num_chroma2 = 0;
    MS_U16 dc_tbl_num_chroma2 = 0;
    MS_U8 chroma2_ci = 0;
#endif
    JPD_Symidx structSymidx;

    JPEG_memset((void *)(&structSymidx), 0, sizeof(structSymidx));

    // Moved to MDrv_JPD_WriteSymidx()
    //MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_SYMIDX_BASE);

    if (_HeadCheck.DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
        if(TRUE == bIs3HuffTbl)
        {
            for(ci = 0; ci<_u8Comps_in_frame; ci++)
            {
                if(_u8Chroma2Ci==_u8Comp_ident[ci])
                {
                    chroma2_ci = ci;
                    break;
                }
            }
        }
#endif

        //tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        ac_tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        //tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];
        ac_tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];

// Moved to MDrv_JPD_WriteSymidx()
#if 0
        for ( i = 0; i < 256; i++ )
        {
            MDrv_Write2Byte( BK_JPD_TID_DAT, ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ) );
        }
#endif

        dc_tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        dc_tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];
#if 0
        for ( i = 0; i < 16; i++ )
        {
            MDrv_Write2Byte( BK_JPD_TID_DAT, ( _Huff_info[tbl_num_chroma].u8Huff_val[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Huff_val[i] ) );
        }
#endif
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
        if(TRUE == bIs3HuffTbl)
        {
            ac_tbl_num_chroma2 = _u8Comp_ac_tab[chroma2_ci];
            dc_tbl_num_chroma2 = _u8Comp_dc_tab[chroma2_ci];
        }
#endif
    }
    else
    {
// Moved to MDrv_JPD_WriteSymidx()
#if 0
        for(i=0;i<272;i++)
            MDrv_Write2Byte( BK_JPD_TID_DAT, g16SYMIDX_TBL[i]);
#endif
    }

    structSymidx.DHT = _HeadCheck.DHT;
    structSymidx.bUVHuffman = bIs3HuffTbl;
    structSymidx.u8DcLumaHuffVal = _Huff_info[dc_tbl_num_luma].u8Huff_val;
    structSymidx.u8DcChromaHuffVal = _Huff_info[dc_tbl_num_chroma].u8Huff_val;
    structSymidx.u8AcLumaHuffVal = _Huff_info[ac_tbl_num_luma].u8Huff_val;
    structSymidx.u8AcChromaHuffVal = _Huff_info[ac_tbl_num_chroma].u8Huff_val;
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
    if(TRUE == bIs3HuffTbl)
    {
        structSymidx.u8DcChroma2HuffVal = _Huff_info[dc_tbl_num_chroma2].u8Huff_val;
        structSymidx.u8AcChroma2HuffVal = _Huff_info[ac_tbl_num_chroma2].u8Huff_val;
    }
#endif
    MDrv_JPD_WriteSymidx(structSymidx);
}
//------------------------------------------------------------------------------
//***************************************************
//write quantization table
//***************************************************
#if 0
JPEG_STATIC void JPEG_write_Qtbl(void)
{
    MS_U8 i, j;
    MS_U8 com_num = 0;
    MS_U8 comp[JPEG_MAXCOMPONENTS];

    MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_QTBL_BASE);

    if (_HeadCheck.DQT)
    {
        // Calculate how many valid quantization tables
        JPEG_memset((void *)comp, 0, JPEG_MAXCOMPONENTS);
        for(i = 0; i<_u8Comps_in_frame; i++)
        {
            comp[_u8Comp_quant[i]] = 1;
        }

        for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
        {
            if(comp[i]==1)
                com_num++;
        }

        for ( i = 0; i < com_num; i++ )
        {
            for(j = 0; j<64; j++)
            {
                MDrv_Write2Byte(BK_JPD_TID_DAT, _QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
            }
        }

        // if all compoents refer to the same Qtable, need to write Qtable twice
        if(com_num==1)
        {
            for ( i = 0; i < com_num; i++ )
            {
                for(j = 0; j<64; j++)
                {
                    MDrv_Write2Byte(BK_JPD_TID_DAT, _QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
                }
            }
        }
    }
    else
    {
        for (i=0; i<128; i++)
            MDrv_Write2Byte(BK_JPD_TID_DAT, g16IQ_TBL[i]);
    }
}
#endif
JPEG_STATIC void JPEG_WriteIQTbl(void)
{
    //MS_U8 i, j;
    MS_U8 i;
    MS_U8 com_num = 0;
    MS_U8 comp[JPEG_MAXCOMPONENTS];
    JPD_IQTbl structIqtbl;

    JPEG_memset((void *)&structIqtbl, 0, sizeof(structIqtbl));

    // Moved to MDrv_JPD_WriteIQTbl()
    //MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_QTBL_BASE);

    if (_HeadCheck.DQT)
    {
        // Calculate how many valid quantization tables
        JPEG_memset((void *)comp, 0, JPEG_MAXCOMPONENTS);
        for(i = 0; i<_u8Comps_in_frame; i++)
        {
            comp[_u8Comp_quant[i]] = 1;
        }

        for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
        {
            if(comp[i]==1)
                com_num++;
        }

        // Moved to MDrv_JPD_WriteIQTbl
#if 0
        for ( i = 0; i < com_num; i++ )
        {
            for(j = 0; j<64; j++)
            {
                MDrv_Write2Byte(BK_JPD_TID_DAT, _QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
            }
        }

        // if all compoents refer to the same Qtable, need to write Qtable twice
        if(com_num==1)
        {
            for ( i = 0; i < com_num; i++ )
            {
                for(j = 0; j<64; j++)
                {
                    MDrv_Write2Byte(BK_JPD_TID_DAT, _QuantTables[_u8Comp_quant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
                }
            }
        }
#endif
    }
    else
    {
        // Moved to MDrv_JPD_WriteIQTbl()
#if 0
        for (i=0; i<128; i++)
            MDrv_Write2Byte(BK_JPD_TID_DAT, g16IQ_TBL[i]);
#endif
    }

    structIqtbl.DQT = _HeadCheck.DQT;
    structIqtbl.u8CompNum = com_num;
    structIqtbl.u8CompQuant = _u8Comp_quant;
    structIqtbl.QuantTables = (JPD_QuanTbl *)_QuantTables;
    MDrv_JPD_WriteIQTbl(structIqtbl);
}
//------------------------------------------------------------------------------
//*************************************************
//write group information
//*************************************************
#if 0
JPEG_STATIC void JPEG_write_Scwgif(void) //type : luma=>0  chroma=>1
{
    MS_U32 reg_value;
    MS_U16 i, ci, valid, tbl_num_luma, tbl_num_chroma;
    MS_U8 luma_ci = 0, chroma_ci = 0;

    MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_SCWGIF_BASE);
    if (_HeadCheck.DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

        tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }

        tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }
    }
    else
    {
        for( i = 0; i < 128; i++ )
            MDrv_Write2Byte(BK_JPD_TID_DAT, g16GRPINFO_TBL[i]);
    }
}
#endif
JPEG_STATIC void JPEG_WriteGrpinf(void) //type : luma=>0  chroma=>1
{
    //MS_U32 reg_value;
    //MS_U16 i, ci, valid, tbl_num_luma, tbl_num_chroma;
    MS_U16 ci, dc_tbl_num_luma = 0, dc_tbl_num_chroma = 0;
    MS_U16 ac_tbl_num_luma = 0, ac_tbl_num_chroma = 0;
    MS_U8 luma_ci = 0, chroma_ci = 0;
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
    MS_U16 dc_tbl_num_chroma2 = 0;
    MS_U16 ac_tbl_num_chroma2 = 0;
    MS_U8 chroma2_ci = 0;
#endif
    JPD_Grpinf structGrpinf;

    JPEG_memset((void *)&structGrpinf, 0, sizeof(structGrpinf));

// Moved to MDrv_JPD_WriteGrpinf()
#if 0
    //MDrv_Write2Byte(BK_JPD_TID_ADR, JPD_MEM_SCWGIF_BASE);
#endif
    if (_HeadCheck.DHT)
    {
        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8LumaCi==_u8Comp_ident[ci])
            {
                luma_ci = ci;
                break;
            }
        }

        for(ci = 0; ci<_u8Comps_in_frame; ci++)
        {
            if(_u8ChromaCi==_u8Comp_ident[ci])
            {
                chroma_ci = ci;
                break;
            }
        }

#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
        if(TRUE == bIs3HuffTbl)
        {
            for(ci = 0; ci<_u8Comps_in_frame; ci++)
            {
                if(_u8Chroma2Ci==_u8Comp_ident[ci])
                {
                    chroma2_ci = ci;
                    break;
                }
            }
        }
#endif

        dc_tbl_num_luma = _u8Comp_dc_tab[luma_ci];
        dc_tbl_num_chroma = _u8Comp_dc_tab[chroma_ci];

// Moved to MDrv_JPD_WriteGrpinf()
#if 0
        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }
#endif

        ac_tbl_num_luma = _u8Comp_ac_tab[luma_ci];
        ac_tbl_num_chroma = _u8Comp_ac_tab[chroma_ci];
// Moved to MDrv_JPD_WriteGrpinf()
#if 0
        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_luma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_luma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_luma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(_Huff_info[tbl_num_chroma].u8Symbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( _Huff_info[tbl_num_chroma].u16Code[i] << 8 ) | ( _Huff_info[tbl_num_chroma].u8Symbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value & 0xffff);
            MDrv_Write2Byte(BK_JPD_TID_DAT, reg_value >> 16);
        }
#endif
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
        if(TRUE == bIs3HuffTbl)
        {
            dc_tbl_num_chroma2 = _u8Comp_dc_tab[chroma2_ci];
            ac_tbl_num_chroma2 = _u8Comp_ac_tab[chroma2_ci];
        }
#endif
    }
    else
    {
#if 0
        for( i = 0; i < 128; i++ )
            MDrv_Write2Byte(BK_JPD_TID_DAT, g16GRPINFO_TBL[i]);
#endif
    }

    structGrpinf.DHT = _HeadCheck.DHT;
    structGrpinf.bUVHuffman = bIs3HuffTbl;
    structGrpinf.u8DcLumaSymbol = _Huff_info[dc_tbl_num_luma].u8Symbol;
    structGrpinf.u16DcLumaCode = _Huff_info[dc_tbl_num_luma].u16Code;
    structGrpinf.u8DcChromaSymbol = _Huff_info[dc_tbl_num_chroma].u8Symbol;
    structGrpinf.u16DcChromaCode = _Huff_info[dc_tbl_num_chroma].u16Code;
    structGrpinf.u8AcLumaSymbol = _Huff_info[ac_tbl_num_luma].u8Symbol;
    structGrpinf.u16AcLumaCode = _Huff_info[ac_tbl_num_luma].u16Code;
    structGrpinf.u8AcChromaSymbol = _Huff_info[ac_tbl_num_chroma].u8Symbol;
    structGrpinf.u16AcChromaCode = _Huff_info[ac_tbl_num_chroma].u16Code;
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
    if(TRUE == bIs3HuffTbl)
    {
        structGrpinf.u8DcChroma2Symbol = _Huff_info[dc_tbl_num_chroma2].u8Symbol;
        structGrpinf.u16DcChroma2Code = _Huff_info[dc_tbl_num_chroma2].u16Code;
        structGrpinf.u8AcChroma2Symbol = _Huff_info[ac_tbl_num_chroma2].u8Symbol;
        structGrpinf.u16AcChroma2Code = _Huff_info[ac_tbl_num_chroma2].u16Code;
    }
#endif
    MDrv_JPD_WriteGrpinf(structGrpinf);
}
#endif /* #ifndef CMODEL */
//------------------------------------------------------------------------------
// Write RLE result
JPEG_STATIC MS_BOOL JPEG_write_RLE(JPEG_SVLD *pVld, MS_BOOL bDecodeNow)
{
#ifdef CMODEL
    JPEG_DEBUG_API_MSG("%02x%02x%02x%02x\n",(MS_U8)pVld->byte3, (MS_U8)pVld->byte2, (MS_U8)pVld->byte1, (MS_U8)pVld->byte0);
#else
    MS_U8 *mrc_buffer = (MS_U8 *) MRC_BUFFER_ADDR;
    MS_U16 status;
    MS_U32 start_time;
    MS_U16 cur_vIdx;

    JPEG_memcpy((void *)(mrc_buffer + _u32RLEOffset), (void *)pVld, 4);
    _u32RLEOffset += 4;

    // Check if buffer full
    if((MRC_BUFFER_SIZE == _u32RLEOffset)
    || (TRUE == bDecodeNow))
    {
        JPEG_DEBUG_API_MSG("Do RLE, LENG 0x%lx, bDecodeNow = %d\n", _u32RLEOffset, bDecodeNow);
        JPEG_DEBUG_API_MSG("CPU Sync and Flush Memory~~~~\n");
        MAsm_CPU_Sync();
        MsOS_FlushMemory();

        if(_bFirstRLE == TRUE)
        {
            // Trigger JPD decoding
            if(!JPEG_StartDecode())
                return FALSE;
            _bFirstRLE = FALSE;
        }
        else
        {
            // clear MRC low/high portion read complete event
            //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBH_DONE | E_JPD_EVENT_MRBL_DONE);
            MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBH_DONE | E_JPD_EVENT_MRBL_DONE);
            // mark low/high buffer valid
            //MDrv_Write2Byte( BK_JPD_MCONFIG, (MDrv_Read2Byte(BK_JPD_MCONFIG) & ~0x0003) | (JPD_H_VLD |JPD_L_VLD));
            MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | (JPD_H_VLD |JPD_L_VLD));
        }

#if MSOS_GET_SYSTEM_TIME
        start_time = MsOS_GetSystemTime();
#else
        start_time = 0;
#endif
        //cur_vIdx = MDrv_Read2Byte(BK_JPD_CUR_VIDX);
        cur_vIdx = MDrv_JPD_GetCurVidx();

        JPEG_DEBUG_API_MSG("cur_vIdx = 0x%04X\n",cur_vIdx);

        if( bDecodeNow )
        {
            return TRUE;  //wait done in main loop
        }

        while(1)
        {
            //status = MDrv_JPD_ReadJPDStatus();
            status = MDrv_JPD_GetEventFlag();

            if(status & E_JPD_EVENT_DEC_DONE)
            {
                JPEG_DEBUG_API_MSG("P deocde done\n");
                break;
            }

            if((status & E_JPD_EVENT_ECS_ERROR) || (status & E_JPD_EVENT_IS_ERROR) || (status & E_JPD_EVENT_RST_ERROR)
#if (JPD_SUPPORT_AUTO_PROTECT==true)
                || (status & E_JPD_EVENT_MWB_FULL)
#endif
                )
            {
                // temp patch for protect JPD from writing to illegal memory
                JPEG_DEBUG_API_MSG("CurVidx = %d, CurRow = %d, CurCol = %d ",
                        MDrv_JPD_GetCurVidx(),
                        MDrv_JPD_GetCurRow(),
                        MDrv_JPD_GetCurCol());
                JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_JPD_GetCurMRCAddr());
                //MDrv_JPD_SW_Pause_Reset();
                MDrv_JPD_Rst();

                JPEG_terminate( E_JPEG_JPD_DECODE_ERROR );
                return FALSE;
            }

            if((status & E_JPD_EVENT_MRBH_DONE) && (status & E_JPD_EVENT_MRBL_DONE))
            {
                JPEG_DEBUG_API_MSG("Partial SVLD decode done\n");
                break;
            }

            // Check the V index. If it is not changed withing 500ms, it means that the JPD has some problem.
            // We need to break the infinite loop
            //if(cur_vIdx!=MDrv_Read2Byte(BK_JPD_CUR_VIDX))
            if(cur_vIdx != MDrv_JPD_GetCurVidx())
            {
#if MSOS_GET_SYSTEM_TIME
                start_time = MsOS_GetSystemTime();
#else
                start_time = 0;
#endif
                //cur_vIdx = MDrv_Read2Byte(BK_JPD_CUR_VIDX);
                cur_vIdx = MDrv_JPD_GetCurVidx();
            }
            else
            {
#if MSOS_GET_SYSTEM_TIME
                if((MsOS_GetSystemTime() - start_time) >= DEFAULT_DECODE_TIMEOUT)
#else
                if(start_time++ >= DEFAULT_DECODE_TIMEOUT * 100)
#endif
                {
                    JPEG_DEBUG_API_ERR("ERROR: SVLD deocde time out, VIdx %d\n", cur_vIdx);
                    return FALSE;
                }
            }
        }

        _u32RLEOffset = 0;
    }
#endif
    return TRUE;
}
//------------------------------------------------------------------------------
// Do run length encode of coefficient buffer
//JPEG_STATIC void JPEG_do_RLE(JPEG_BLOCK_TYPE *p, MS_BOOL eop, MS_U8 comp_id)
JPEG_STATIC MS_BOOL JPEG_do_RLE(JPEG_BLOCK_TYPE *p, MS_BOOL eop, MS_U8 comp_id, MS_BOOL BlockInRange)
{
    JPEG_SVLD my_vld;
    MS_U8 counter;
    MS_S16 value;
    MS_U16 run;
    MS_U8 cur_blk;
    JPEG_BLOCK_TYPE predictor;

    if(comp_id==0)
        cur_blk = 1;     // Y
    else if(comp_id==1)
        cur_blk = 3;    // U
    else
        cur_blk = 2;    // V

    predictor = _s16dc_pred[cur_blk - 1];

    run = 0;
    my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
    my_vld.blk_type = cur_blk;

    //sent DC info
////    my_vld.run = 8;
    if( BlockInRange )//Current block is within display range.
        my_vld.run = 8;
    else
        my_vld.run = 0;

    value = (p[0] - predictor);
    my_vld.sign = (value<0)?1:0;
    my_vld.amp = JPEG_ABS(value);
    my_vld.sym_type = E_RLE_DC;
    if(!JPEG_write_RLE(&my_vld, FALSE))
        return FALSE;

    if( BlockInRange == FALSE )//Current block is not within display range.
        return TRUE;

    my_vld.byte0 = my_vld.byte1 =my_vld.byte2 = my_vld.byte3= 0;
    my_vld.blk_type = cur_blk;

    for(counter = 1;counter<64; counter++)
    {
        if(p[counter]==0)
        {
            run++;
        }
        else
        {
            while(run>15)
            {
                my_vld.sign = 0;
                my_vld.amp = 0;
                my_vld.sym_type = E_RLE_ZRL;
                my_vld.run = 15;
                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
                my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
                my_vld.blk_type = cur_blk;
                run -= 16;
            }

            my_vld.sign = (p[counter]<0)?1:0;
            my_vld.amp = JPEG_ABS(p[counter]);
            my_vld.sym_type = E_RLE_AC;
            my_vld.run = run;

            // Check if the last byte is non-zero. If it's non-zero & EOP, add the EOP flag
            if(counter==63&&eop&&p[63]!=0)
            {
                my_vld.EOP = 1;
                if(!JPEG_write_RLE(&my_vld, TRUE))
                    return FALSE;

                _s16dc_pred[cur_blk - 1] = p[0];//update predictor
                return TRUE;
            }
            else
            {
                if(!JPEG_write_RLE(&my_vld, FALSE))
                    return FALSE;
            }

            my_vld.byte0 = my_vld.byte1 = my_vld.byte2 = my_vld.byte3 = 0;
            my_vld.blk_type = cur_blk;
            run = 0;
        }
    }

    counter = 63;

    if(p[counter]==0)
    {
        my_vld.amp = JPEG_ABS(p[counter]);
        my_vld.sign = p[counter]<0?1:0;
        my_vld.sym_type = E_RLE_EOB;
        my_vld.run = 0;
        if(eop)
        {
            my_vld.EOP = 1;
            if(!JPEG_write_RLE(&my_vld, TRUE))
                return FALSE;
        }
        else
        {
            if(!JPEG_write_RLE(&my_vld, FALSE))
                return FALSE;
        }
    }

    _s16dc_pred[cur_blk - 1] = p[0];//update predictor
    return TRUE;
}
//------------------------------------------------------------------------------
// Starts a new scan.
JPEG_STATIC MS_BOOL JPEG_init_scan(void)
{
    if (!JPEG_locate_sos_marker())
        return FALSE;

    JPEG_calc_mcu_block_order();

    if (_HeadCheck.DHT)
    {
        if(!JPEG_check_huff_tables())
            return FALSE;
    }

    if (_HeadCheck.DQT)
    {
        if(!JPEG_check_quant_tables())
            return FALSE;
    }

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    JPEG_memset((void *)_u32Last_dc_val, 0, _u8Comps_in_frame * sizeof( MS_U32 ) );

    _u32EOB_run = 0;
#endif

    if ( _u16Restart_interval )
    {
        _u16Restarts_left = _u16Restart_interval;
        _u16Next_restart_num = 0;
    }

////    _Total_Decoded_Size = (MS_S32)JPEG_GetECS();

#ifdef CMODEL
    {
        // pre-fill bit buffer for later decoding
        _s16Bits_left = 16;
        JPEG_get_bits_2( 16 );
        JPEG_get_bits_2( 16 );
    }
#else
    {
#if SW_JPD_RGB_CMYK
        if(_bProgressive_flag
        || (E_JPEG_CMYK == gu8Scan_type)
        || (E_JPEG_RGB == gu8Scan_type))
#else
        if(_bProgressive_flag)
#endif
        {
            // pre-fill bit buffer for later decoding
            _s16Bits_left = 16;
            JPEG_get_bits_2( 16 );
            JPEG_get_bits_2( 16 );
        }
    }
#endif

    JPEG_DEBUG_API_MSG("JPEG_init_scan:ECS 0x%08lx\n", JPEG_GetECS());

    return TRUE;
}

#if SW_JPD_RGB_CMYK
//------------------------------------------------------------------------------
// Create a few tables that allow us to quickly convert YCbCr to RGB.
JPEG_STATIC void msAPI_JPEG_create_look_ups( void )
{
    MS_S16 i, k;
    //kevinhuang, modify
    /*
    for (i = 0; i <= 255; i++)
    {
      //k = (i * 2) - 255;
        k = (i * 2) - 256; // Dec. 28 2001- change so table[128] == 0
      gs32Crr[i] = ( FIX(1.40200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Cbb[i] = ( FIX(1.77200/2)  * k + ONE_HALF) >> SCALEBITS;
      gs32Crg[i] = (-FIX(0.71414/2)) * k;
      gs32Cbg[i] = (-FIX(0.34414/2)) * k + ONE_HALF;
    }
    */
    for ( i = 0; i <= 255; i++ )
    {
        k = i - 128;

        gs32Crr[i] = ( FIX( 1.40200 ) * k + ONE_HALF ) >> SCALEBITS;
        gs32Cbb[i] = ( FIX( 1.77200 ) * k + ONE_HALF ) >> SCALEBITS;

        gs32Crg[i] = ( -FIX( 0.71414 ) ) * k ; //+ ONE_HALF) >> SCALEBITS;???
        gs32Cbg[i] = ( -FIX( 0.34414 ) ) * k + ONE_HALF; //>> SCALEBITS;???

    }
}

#endif //SW_JPD_RGB_CMYK
//------------------------------------------------------------------------------
// Starts a frame. Determines if the number of components or sampling factors
// are supported.
JPEG_STATIC MS_BOOL JPEG_init_frame(void) //ok
{
#if SW_JPD_RGB_CMYK
    MS_U32 i;
    MS_U8 *q;
#endif

    JPEG_DEBUG_API_MSG("JPEG_init_frame:\n");
    if ( _u8Comps_in_frame == 1 )
    {
        gu8Scan_type = E_JPEG_GRAYSCALE;

        _u16Max_blocks_per_mcu = 1;

        gu8Max_mcu_x_size = 8;
        gu8Max_mcu_y_size = 8;
    }
    else if ( _u8Comps_in_frame == 3 )
    {
        if ( ( ( _u8Comp_h_samp[1] != 1 ) || ( _u8Comp_v_samp[1] != 1 ) ) ||   //support only U_H1V1 & V_H1V1
            ( ( _u8Comp_h_samp[2] != 1 ) || ( _u8Comp_v_samp[2] != 1 ) ) )
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }

        if ( ( _u8Comp_h_samp[0] == 1 ) && ( _u8Comp_v_samp[0] == 1 ) )
        {
            //set RGB based jpeg flag
            if(_u8Comp_ident[0] == 82 || _u8Comp_ident[0] == 71 || _u8Comp_ident[0] == 66)
            {
#if SW_JPD_RGB_CMYK
                if( FALSE == bEnableRGB )
                {
                    JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
                    return FALSE;
                }
                JPEG_DEBUG_API_MSG("Get JPEG_RGB\n");
                gu8Scan_type = E_JPEG_RGB; //RGB
#else
                JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
                return FALSE;
#endif
            }
            else
            {
                JPEG_DEBUG_API_MSG("Get JPEG_YH1V1\n");
                gu8Scan_type = E_JPEG_YH1V1; //4:4:4
            }

            _u16Max_blocks_per_mcu = 3;

            gu8Max_mcu_x_size = 8;
            gu8Max_mcu_y_size = 8;
        }
        else if ( ( _u8Comp_h_samp[0] == 2 ) && ( _u8Comp_v_samp[0] == 1 ) )
        {
            gu8Scan_type = E_JPEG_YH2V1; //4:2:2

            _u16Max_blocks_per_mcu = 4;

            gu8Max_mcu_x_size = 16;
            gu8Max_mcu_y_size = 8;
        }
        else if ( ( _u8Comp_h_samp[0] == 1 ) && ( _u8Comp_v_samp[0] == 2 ) )
        {
            gu8Scan_type = E_JPEG_YH1V2;

            _u16Max_blocks_per_mcu = 4;

            gu8Max_mcu_x_size = 8;
            gu8Max_mcu_y_size = 16;
        }
        else if ( ( _u8Comp_h_samp[0] == 2 ) && ( _u8Comp_v_samp[0] == 2 ) )
        {
            gu8Scan_type = E_JPEG_YH2V2; //4:2:0

            _u16Max_blocks_per_mcu = 6;

            gu8Max_mcu_x_size = 16;
            gu8Max_mcu_y_size = 16;
        }
        else if ( ( _u8Comp_h_samp[0] == 4 ) && ( _u8Comp_v_samp[0] == 1 ) )
        {
            // 4:1:1
            gu8Scan_type = E_JPEG_YH4V1;

            _u16Max_blocks_per_mcu = 6;

            gu8Max_mcu_x_size = 32;
            gu8Max_mcu_y_size = 8;
            //#if CMODEL
            //JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            //#endif
        }
        else
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }
    }
#if SW_JPD_RGB_CMYK
    else if(_u8Comps_in_frame == 4) //handle YCCK & CMYK case, must distinguish YCCK and CMYK later
    {
        if( FALSE == bEnableCMYK )
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
            return FALSE;
        }

        if ( ( _u8Comp_h_samp[0] == 1 ) && ( _u8Comp_v_samp[0] == 1 ) )
        {
            JPEG_DEBUG_API_MSG("Get JPEG_CMYK\n");
            gu8Scan_type = E_JPEG_CMYK;

            _u16Max_blocks_per_mcu = 4;

            gu8Max_mcu_x_size = 8;
            gu8Max_mcu_y_size = 8;
        }
        else
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }
    }
#endif
    else
    {
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    JPEG_DEBUG_API_MSG("JPEG_init_frame:gu8Scan_type = %d\n", gu8Scan_type);

    gu16Max_mcus_per_row = ( _u16Image_x_size + ( gu8Max_mcu_x_size - 1 ) ) / gu8Max_mcu_x_size;
    _u16Max_mcus_per_col = ( _u16Image_y_size + ( gu8Max_mcu_y_size - 1 ) ) / gu8Max_mcu_y_size;

#ifdef CMODEL
    /* these values are for the *destination* pixels: after conversion */

    if ( E_JPEG_GRAYSCALE == gu8Scan_type )
    {
        gu8Dest_bytes_per_pixel = 1;
    }
    else
        //kevinhuang, ToDo 4 -> 3 later
    {
        gu8Dest_bytes_per_pixel = 4;
    }

    gu16Dest_bytes_per_scan_line = ( ( _u16Image_x_size + 15 ) & 0xFFF0 ) * gu8Dest_bytes_per_pixel;
    gu16Real_dest_bytes_per_scan_line = ( _u16Image_x_size * gu8Dest_bytes_per_pixel );

    // Initialize two scan line buffers.
    // FIXME: Only the V2 sampling factors need two buffers.
    #define DC_ALIGNMENT    16
    //pgu8Scan_line_0         = (MS_U8 *)JPEG_alloc(gu16Dest_bytes_per_scan_line + 8);
    pgu8Scan_line_0 = ( MS_U8 * )JPEG_alloc( gu16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
    if(pgu8Scan_line_0 == NULL)
        return FALSE;
    JPEG_memset((void *)pgu8Scan_line_0, 0x7F, gu16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE

    //pgu8scan_line_1         = (MS_U8 *)JPEG_alloc(gu16Dest_bytes_per_scan_line + 8);
    pgu8scan_line_1 = ( MS_U8 * )JPEG_alloc( gu16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
    if(pgu8scan_line_1 == NULL)
        return FALSE;
    JPEG_memset((void *)pgu8scan_line_1, 0x7F, gu16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE
#endif
#if SW_JPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        /* these values are for the *destination* pixels: after conversion */

        //Reset image x size by new Max_mcus
        _u16Image_x_size = gu16Max_mcus_per_row*gu8Max_mcu_x_size;

        _u16NonAlignmentImage_x_size = _u16Image_x_size;

        if ( E_JPEG_GRAYSCALE == gu8Scan_type )
        {
            _u8Dest_bytes_per_pixel = 1;
        }
        else
            //kevinhuang, ToDo 4 -> 3 later
        {
            _u8Dest_bytes_per_pixel = 4;
        }

        _u16Dest_bytes_per_scan_line = ( ( _u16Image_x_size + 15 ) & 0xFFF0 ) * _u8Dest_bytes_per_pixel;
//        _u16Real_dest_bytes_per_scan_line = ( _u16Image_x_size * _u8Dest_bytes_per_pixel );

        // Initialize two scan line buffers.
        // FIXME: Only the V2 sampling factors need two buffers.
        #define DC_ALIGNMENT    16
        //pgu8Scan_line_0         = (U8 *)alloc(_u16Dest_bytes_per_scan_line + 8);
        pgu8Scan_line_0 = ( MS_U8 * )JPEG_alloc( _u16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
        if(pgu8Scan_line_0 == NULL)
            return FALSE;
        JPEG_memset( pgu8Scan_line_0, 0x7F, _u16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE

        //pgu8scan_line_1         = (U8 *)alloc(_u16Dest_bytes_per_scan_line + 8);
        pgu8scan_line_1 = ( MS_U8 * )JPEG_alloc( _u16Dest_bytes_per_scan_line + DC_ALIGNMENT * 4 );
        if(pgu8scan_line_1 == NULL)
            return FALSE;
        JPEG_memset( pgu8scan_line_1, 0x7F, _u16Dest_bytes_per_scan_line ); //kevinhuang, write dummy bytes for DC pitch alignment to display right border in certain color thru VE
    }
#endif

    _u32Max_blocks_per_row = gu16Max_mcus_per_row * _u16Max_blocks_per_mcu;

    // Should never happen
    if ( _u32Max_blocks_per_row > JPEG_MAXBLOCKSPERROW )
    {
        JPEG_terminate( E_JPEG_ASSERTION_ERROR );
        return FALSE;
    }

#ifdef CMODEL
    {
        MS_U32 i;
        MS_U8 *q;

        // Allocate the coefficient buffer, enough for one row's worth of MCU's
        q = ( MS_U8 * )JPEG_alloc( _u32Max_blocks_per_row * 64 * sizeof( JPEG_BLOCK_TYPE ) + 8 );
        if(q == NULL)
            return FALSE;

        // Align to 8-byte boundry, for MMX code
        q = ( MS_U8 * )( ( ( MS_U32 )q + 7 ) & ~7 );
        if(q == NULL)
            return FALSE;

        // The gps16Block_seg[] array's name dates back to the
        // 16-bit assembler implementation. "seg" stood for "segment".
        for ( i = 0; i < _u32Max_blocks_per_row; i++ )
        {
            gps16Block_seg[i] = ( JPEG_BLOCK_TYPE * )( q + i * 64 * sizeof( JPEG_BLOCK_TYPE ) );
        }

        for ( i = 0; i < _u32Max_blocks_per_row; i++ )
        {
            gu8Block_max_zag_set[i] = 64;
        }

        gpu8Sample_buf = ( MS_U8 * )( ( ( MS_U32 )JPEG_alloc( _u32Max_blocks_per_row * 64 + 8 ) + 7 ) & ~7 );
        if(gpu8Sample_buf == NULL)
            return FALSE;

        JPEG_CMODEL_create_look_ups();
    }
#endif
#if SW_JPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        // Allocate the coefficient buffer, enough for one row's worth of MCU's
        q = ( MS_U8 * )JPEG_alloc( _u32Max_blocks_per_row * 64 * sizeof( JPEG_BLOCK_TYPE ) + 8 );
        if(q == NULL)
            return FALSE;

        // Align to 8-byte boundry, for MMX code
        q = ( MS_U8 * )( ( ( MS_U32 )q + 7 ) & ~7 );

        // The _ps16Block_seg[] array's name dates back to the
        // 16-bit assembler implementation. "seg" stood for "segment".
        for ( i = 0; i < _u32Max_blocks_per_row; i++ )
        {
            _ps16Block_seg[i] = ( JPEG_BLOCK_TYPE * )( (MS_U32)q + i * 64 * sizeof( JPEG_BLOCK_TYPE ) );
        }

        for ( i = 0; i < _u32Max_blocks_per_row; i++ )
        {
            _u8Block_max_zag_set[i] = 64;
        }

        gpu8Sample_buf = ( MS_U8 * )( ( ( MS_U32 )JPEG_alloc( _u32Max_blocks_per_row * 64 + 8 ) + 7 ) & ~7 );
        if(gpu8Sample_buf == NULL)
            return FALSE;

        _u16Total_lines_left = _u16Image_y_size;

#ifdef CMODEL
        gu16Mcu_lines_left = 0;
#endif

        msAPI_JPEG_create_look_ups();
    }
#endif

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
    _u16Total_lines_left = _u16Max_mcus_per_col * gu8Max_mcu_y_size;
#ifdef CMODEL
    gu16Mcu_lines_left = 0;
#endif
#endif
    return TRUE;
}
//------------------------------------------------------------------------------
#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
//------------------------------------------------------------------------------
// Restart interval processing.
JPEG_STATIC MS_BOOL JPEG_process_restart(void)
{
    MS_U16 i, c = 0;

    // Let's scan a little bit to find the marker, but not _too_ far.
    // 1536 is a "fudge factor" that determines how much to scan.
    for ( i = 1536; i > 0; i-- )
    {
        if ( JPEG_get_char() == 0xFF )
        {
            break;
        }
    }

    if ( i == 0 )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    for ( ; i > 0; i-- )
    {
        c = JPEG_get_char();
        if ( c != 0xFF )
        {
            break;
        }
    }

    if ( i == 0 )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    // Is it the expected marker? If not, something bad happened.
    if ( c != ( _u16Next_restart_num + E_JPEG_RST0 ) )
    {
        JPEG_terminate( E_JPEG_BAD_RESTART_MARKER );
        return FALSE;
    }

    // Reset each component's DC prediction values.
    JPEG_memset((void *)&_u32Last_dc_val, 0, _u8Comps_in_frame * sizeof( MS_U32 ) );

    _u32EOB_run = 0;

    _u16Restarts_left = _u16Restart_interval;

    _u16Next_restart_num = ( _u16Next_restart_num + 1 ) & 7;

    // Get the bit buffer going again...
    {
        _s16Bits_left = 16;
        JPEG_get_bits_2( 16 );
        JPEG_get_bits_2( 16 );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// The following methods decode the various types of blocks encountered
// in progressively encoded images.
JPEG_STATIC MS_BOOL JPEG_decode_block_dc_first(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    MS_S32 s, r;
    JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _DC_Coeffs[component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    s = JPEG_huff_decode( &_Huff_tbls[_u8Comp_dc_tab[component_id]] );
    if ( s != 0 )
    {
        r = JPEG_get_bits_2( s );
        s = HUFF_EXTEND_P( r, s );
    }

    // In JPD mode, the DC coefficient is the difference of nearest DC
    _u32Last_dc_val[component_id] = ( s += _u32Last_dc_val[component_id] );

    p[0] = s << _u8Successive_low;
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_dc_refine(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    if ( JPEG_get_bits_2( 1 ) )
    {
        JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _DC_Coeffs[component_id], block_x, block_y );

        if(p == NULL)
        {
            JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        p[0] |= ( 1 << _u8Successive_low );
    }
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_ac_first(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    JPEG_BLOCK_TYPE *p;
    MS_S32 k, s, r;

    if ( _u32EOB_run )
    {
        _u32EOB_run--;
        return TRUE;
    }

    p = JPEG_coeff_buf_getp( _AC_Coeffs[component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    for ( k = _u8Spectral_start; k <= _u8Spectral_end; k++ )
    {
        s = JPEG_huff_decode( &_Huff_tbls[_u8Comp_ac_tab[component_id]] );

        r = s >> 4;
        s &= 15;

        if ( s )
        {
            k += r;
            if ( k > 63 )
            {
                JPEG_terminate( E_JPEG_DECODE_ERROR );
                return FALSE;
            }

            r = JPEG_get_bits_2( s );
            s = HUFF_EXTEND_P( r, s );

            // No need to do ZAG order in JPD mode
#ifdef CMODEL
            //p[_u8ZAG[k]] = s << _u8Successive_low;
            p[k] = s << _u8Successive_low;
#else
#if SW_JPD_RGB_CMYK
            if((E_JPEG_CMYK == gu8Scan_type)
            || (E_JPEG_RGB == gu8Scan_type))
            {
                p[_u8ZAG[k]] = s << _u8Successive_low;
            }
            else
#endif
            {
                p[k] = s << _u8Successive_low;
            }
#endif
        }
        else
        {
            if ( r == 15 )
            {
                k += 15;
                if ( k > 63 )
                {
                    JPEG_terminate( E_JPEG_DECODE_ERROR );
                    return FALSE;
                }
            }
            else
            {
                _u32EOB_run = 1 << r;

                if ( r )
                {
                    _u32EOB_run += JPEG_get_bits_2( r );
                }

                _u32EOB_run--;

                break;
            }
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_decode_block_ac_refine(//JPEG_DECODER *Pd,
  MS_U8 component_id, MS_U16 block_x, MS_U16 block_y)
{
    MS_S32 s, k, r;
    MS_S32 p1 = 1 << _u8Successive_low;
    MS_S32 m1 = ( -1 ) << _u8Successive_low;
    JPEG_BLOCK_TYPE *p = JPEG_coeff_buf_getp( _AC_Coeffs[component_id], block_x, block_y );

    if(p == NULL)
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    k = _u8Spectral_start;

    if ( _u32EOB_run == 0 )
    {
        for ( ; (k <= _u8Spectral_end) && ( k < 64 ); k++ )
        {
            s = JPEG_huff_decode( &_Huff_tbls[_u8Comp_ac_tab[component_id]] );

            r = s >> 4;
            s &= 15;

            if ( s )
            {
                if ( s != 1 )
                {
                    JPEG_terminate( E_JPEG_DECODE_ERROR );
                    return FALSE;
                }

                if ( JPEG_get_bits_2( 1 ) )
                {
                    s = p1;
                }
                else
                {
                    s = m1;
                }
            }
            else
            {
                if ( r != 15 )
                {
                    _u32EOB_run = 1 << r;

                    if ( r )
                    {
                        _u32EOB_run += JPEG_get_bits_2( r );
                    }

                    break;
                }
            }

            do
            {
                // No need to do ZAG order in JPD mode
#ifdef CMODEL
                //JPEG_BLOCK_TYPE *this_coef = p + _u8ZAG[k];
                JPEG_BLOCK_TYPE *this_coef = p + k;
#else
                JPEG_BLOCK_TYPE *this_coef;
#if SW_JPD_RGB_CMYK
                if((E_JPEG_CMYK == gu8Scan_type)
                || (E_JPEG_RGB == gu8Scan_type))
                {
                    this_coef = p + _u8ZAG[k];
                }
                else
#endif
                {
                    this_coef = p + k;
                }
#endif

                if ( *this_coef != 0 )
                {
                    if ( JPEG_get_bits_2( 1 ) )
                    {
                        if ( ( *this_coef & p1 ) == 0 )
                        {
                            if ( *this_coef >= 0 )
                            {
                                *this_coef += p1;
                            }
                            else
                            {
                                *this_coef += m1;
                            }
                        }
                    }
                }
                else
                {
                    if ( --r < 0 )
                    {
                        break;
                    }
                }

                k++;
            }
            while ( (k <= _u8Spectral_end) && ( k < 64 ) );

            if ( ( s ) && ( k < 64 ) )
            {
                // No need to do ZAG order in JPD mode
#ifdef CMODEL
                //p[_u8ZAG[k]] = s;
                p[k] = s;
#else
#if SW_JPD_RGB_CMYK
                if((E_JPEG_CMYK == gu8Scan_type)
                || (E_JPEG_RGB == gu8Scan_type))
                {
                    p[_u8ZAG[k]] = s;
                }
                else
#endif
                {
                    p[k] = s;
                }
#endif
            }
        }
    }

    if ( _u32EOB_run > 0 )
    {
        for ( ; (k <= _u8Spectral_end) && ( k < 64 ); k++ )
        {
            // No need to do ZAG order in JPD mode
#ifdef CMODEL
            //JPEG_BLOCK_TYPE *this_coef = p + _u8ZAG[k];
            JPEG_BLOCK_TYPE *this_coef = p + k;
#else
            JPEG_BLOCK_TYPE *this_coef;
#if SW_JPD_RGB_CMYK
            if((E_JPEG_CMYK == gu8Scan_type)
            || (E_JPEG_RGB == gu8Scan_type))
            {
                this_coef = p + _u8ZAG[k];
            }
            else
#endif
            {
                this_coef = p + k;
            }
#endif

            if ( *this_coef != 0 )
            {
                if ( JPEG_get_bits_2( 1 ) )
                {
                    if ( ( *this_coef & p1 ) == 0 )
                    {
                        if ( *this_coef >= 0 )
                        {
                            *this_coef += p1;
                        }
                        else
                        {
                            *this_coef += m1;
                        }
                    }
                }
            }
        }

        _u32EOB_run--;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Decode a scan in a progressively encoded image.
JPEG_STATIC MS_BOOL JPEG_decode_scan(Pdecode_block_func decode_block_func)
{
    MS_U16 mcu_row, mcu_col, mcu_block;
    MS_U32 block_x_mcu[JPEG_MAXCOMPONENTS], block_y_mcu[JPEG_MAXCOMPONENTS];

    JPEG_memset((void *)block_y_mcu, 0, sizeof( block_y_mcu ) );

    for ( mcu_col = 0; mcu_col < _u16Mcus_per_col; mcu_col++ )
    {
        int component_num, component_id;

        JPEG_memset((void *)block_x_mcu, 0, sizeof( block_x_mcu ) );

        for ( mcu_row = 0; mcu_row < _u16Mcus_per_row; mcu_row++ )
        {
            MS_U8 block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;

            if ( ( _u16Restart_interval ) && ( _u16Restarts_left == 0 ) )
            {
                if(!JPEG_process_restart())
                    return FALSE;
            }

            for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu; mcu_block++ )
            {
                component_id = _u8Mcu_org[mcu_block];

                if(!decode_block_func( component_id, block_x_mcu[component_id] + block_x_mcu_ofs, block_y_mcu[component_id] + block_y_mcu_ofs ))
                {
                    JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                    return FALSE;
                }

                if ( _u8Comps_in_scan == 1 )
                {
                    block_x_mcu[component_id]++;
                }
                else
                {
                    if ( ++block_x_mcu_ofs == _u8Comp_h_samp[component_id] )
                    {
                        block_x_mcu_ofs = 0;

                        if ( ++block_y_mcu_ofs == _u8Comp_v_samp[component_id] )
                        {
                            block_y_mcu_ofs = 0;

                            block_x_mcu[component_id] += _u8Comp_h_samp[component_id];
                        }
                    }
                }
            }

            _u16Restarts_left--;
        }

        if ( _u8Comps_in_scan == 1 )
        {
            block_y_mcu[_u8Comp_list[0]]++;
        }
        else
        {
            for ( component_num = 0; component_num < _u8Comps_in_scan; component_num++ )
            {
                component_id = _u8Comp_list[component_num];

                block_y_mcu[component_id] += _u8Comp_v_samp[component_id];
            }
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// Decode a progressively encoded image.
JPEG_STATIC MS_BOOL JPEG_init_progressive(void)
{
    MS_U8 i;

    if ( _u8Comps_in_frame == 4 )
    {
        JPEG_terminate( E_JPEG_UNSUPPORTED_COLORSPACE );
        return FALSE;
    }

    // Allocate the coefficient buffers.
    for ( i = 0; i < _u8Comps_in_frame; i++ )
    {
        _DC_Coeffs[i] = JPEG_coeff_buf_open( ((gu16Max_mcus_per_row+0x1)& ~0x1) * _u8Comp_h_samp[i], ((_u16Max_mcus_per_col+0x1)& ~0x1) * _u8Comp_v_samp[i], 1, 1 );
        if(_DC_Coeffs[i] == NULL)
        {
            return FALSE;
        }

        _AC_Coeffs[i] = JPEG_coeff_buf_open( ((gu16Max_mcus_per_row+0x1)& ~0x1) * _u8Comp_h_samp[i], ((_u16Max_mcus_per_col+0x1)& ~0x1) * _u8Comp_v_samp[i], 8, 8 );
        if(_AC_Coeffs[i] == NULL)
        {
            return FALSE;
        }
    }

    for ( ; ; )
    {
        MS_BOOL dc_only_scan, refinement_scan;
        Pdecode_block_func decode_block_func;

        if (!JPEG_init_scan())
        {
            break;
        }

        dc_only_scan = ( _u8Spectral_start == 0 );
        refinement_scan = ( _u8Successive_high != 0 );

        if ( ( _u8Spectral_start > _u8Spectral_end ) || ( _u8Spectral_end > 63 ) )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
            return FALSE;
        }

        if ( dc_only_scan )
        {
            if ( _u8Spectral_end )
            {
                JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
                return FALSE;
            }
        }
        else if ( _u8Comps_in_scan != 1 )  /* AC scans can only contain one component */
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SPECTRAL );
            return FALSE;
        }

        if ( ( refinement_scan ) && ( _u8Successive_low != _u8Successive_high - 1 ) )
        {
            JPEG_terminate( E_JPEG_BAD_SOS_SUCCESSIVE );
            return FALSE;
        }

        if ( dc_only_scan )
        {
            if ( refinement_scan )
            {
                decode_block_func = JPEG_decode_block_dc_refine;
            }
            else
            {
                decode_block_func = JPEG_decode_block_dc_first;
            }
        }
        else
        {
            if ( refinement_scan )
            {
                decode_block_func = JPEG_decode_block_ac_refine;
            }
            else
            {
                decode_block_func = JPEG_decode_block_ac_first;
            }
        }

        if(!JPEG_decode_scan( decode_block_func ))
            return FALSE;

        _s16Bits_left = 0;
    }

    _u8Comps_in_scan = _u8Comps_in_frame;

    for ( i = 0; i < _u8Comps_in_frame; i++ )
    {
        _u8Comp_list[i] = i;
    }

    JPEG_calc_mcu_block_order();
    return TRUE;
}
#endif
//------------------------------------------------------------------------------
JPEG_STATIC MS_BOOL JPEG_init_sequential(void)
{
    if ( !JPEG_init_scan() )
    {
        JPEG_DEBUG_API_ERR("JPEG_init_scan - E_JPEG_UNEXPECTED_MARKER\n");
        JPEG_terminate( E_JPEG_UNEXPECTED_MARKER );
        return FALSE;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
#if 0
JPEG_STATIC void JPEG_decode_start(void)
{
    JPEG_init_frame();

    if ( _bProgressive_flag )
    {
        JPEG_init_progressive();
    }
    else
    {
        JPEG_init_sequential();
    }
}
#endif
//------------------------------------------------------------------------------
// Find the start of the JPEG file and reads enough data to determine
// its size, number of components, etc.
JPEG_STATIC MS_BOOL JPEG_decode_init(void)
{
    MS_U16 pic_width, pic_height;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    _HeadCheck.result = 0;

    if(!JPEG_init())
        return FALSE;

#if SUPPORT_MPO_FORMAT
    _pu8In_buf_ofs = _pu8In_buf_MPO_ofs;
    _u32In_buf_left = _u32In_buf_MPO_left;
    JPEG_DEBUG_API_MSG("JPEG_decode_init(), set _pu8In_buf_ofs=0x%lx, _u32In_buf_left=0x%lx========\n", (MS_U32)_pu8In_buf_ofs, (MS_U32)_u32In_buf_left);
#endif

    if(!JPEG_locate_sof_marker())
        return FALSE;

    if (bMHEG5)
    {
    if(_Error_code == E_JPEG_UNSUPPORTED_MARKER)
        {
            return TRUE;
        }
    }

    if(_u8DecodeType == E_JPEG_TYPE_THUMBNAIL)
    {
        if(_bThumbnailFound)
        {
            _bThumbnailAccessMode = TRUE;
            ////MApi_JPEG_Finalize(); //replace with JPEG_free_all_blocks()
            JPEG_free_all_blocks();

            if(!JPEG_init())
                return FALSE;

            // save the original image size, because msAPI_JPEG_init will reset all variables to 0
            if(!JPEG_locate_sof_marker())
                return FALSE;

            if(_bProgressive_flag)
            {
                JPEG_DEBUG_API_ERR("Progressive image in thumbnail... do not handle it\n");
                JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
                return FALSE;
            }
        }
        else
        {
            JPEG_terminate( E_JPEG_NO_THUMBNAIL );
            return FALSE;
        }
    }

    // calculate aligned resolution
    JPEG_GetAlignedResolution(&pic_width, &pic_height);
    JPEG_DEBUG_API_MSG("pic_width = %d, pic_height = %d\n",pic_width, pic_height);

    _u16AlignedImagePitch = pic_width;
    _u16AlignedImagePitch_H = pic_height;
    _u16AlignedImageWidth = _u16Image_x_size;
    _u16AlignedImageHeight = _u16Image_y_size;

#if SUPPORT_MPO_FORMAT
    if(bIsMPOFormat)
    {
        JPEG_PRO_MAX_WIDTH = JPEG_MPO_PRO_MAX_WIDTH;
        JPEG_PRO_MAX_HEIGHT = JPEG_MPO_PRO_MAX_HEIGHT;
        JPEG_MAX_WIDTH  = JPEG_MPO_MAX_WIDTH;
        JPEG_MAX_HEIGHT   = JPEG_MPO_MAX_HEIGHT;
    }
#endif

    if(_bProgressive_flag)
    {
        MAX_JPEG_WIDTH_HD = JPEG_PRO_MAX_WIDTH;
        MAX_JPEG_HEIGHT_HD = JPEG_PRO_MAX_HEIGHT;
    }
    else
    {
        MAX_JPEG_WIDTH_HD = JPEG_MAX_WIDTH;
        MAX_JPEG_HEIGHT_HD = JPEG_MAX_HEIGHT;
    }

#ifndef CMODEL
    // calculate down scale factor
    if ((_u16Image_x_size > MAX_JPEG_WIDTH_HD * 4) || (_u16Image_y_size > MAX_JPEG_HEIGHT_HD * 4))
    {
        JPEG_DEBUG_API_MSG("down scale 1/8!!\n");
        _u8DownScaleRatio = E_JPD_DOWNSCALE_EIGHTH;
        _u8ScaleDownFactor = 8;
    }
    else if ((_u16Image_x_size > MAX_JPEG_WIDTH_HD * 2) || (_u16Image_y_size > MAX_JPEG_HEIGHT_HD * 2))
    {
        JPEG_DEBUG_API_MSG("down scale 1/4!!\n");
        _u8DownScaleRatio = E_JPD_DOWNSCALE_FOURTH;
        _u8ScaleDownFactor = 4;
    }
    else if ((_u16Image_x_size > MAX_JPEG_WIDTH_HD) || (_u16Image_y_size > MAX_JPEG_HEIGHT_HD))
    {
        JPEG_DEBUG_API_MSG("down scale 1/2!!\n");
        _u8DownScaleRatio = E_JPD_DOWNSCALE_HALF;
        _u8ScaleDownFactor = 2;
    }
    else
    {
        JPEG_DEBUG_API_MSG("down scale 1/1!!\n");
        _u8DownScaleRatio = E_JPD_DOWNSCALE_ORG;
        _u8ScaleDownFactor = 1;
    }

#if (ENABLE_TEST_11_JPEGScaleDownFunctionTest_2==true) \
    || (ENABLE_TEST_11_JPEGScaleDownFunctionTest_4==true) \
    || (ENABLE_TEST_11_JPEGScaleDownFunctionTest_8==true)
    //downscale test(only for unit test)

    #if (ENABLE_TEST_11_JPEGScaleDownFunctionTest_2==true)
    _u8DownScaleRatio = E_JPD_DOWNSCALE_HALF;
    #elif (ENABLE_TEST_11_JPEGScaleDownFunctionTest_4==true)
    _u8DownScaleRatio = E_JPD_DOWNSCALE_FOURTH;
    #elif (ENABLE_TEST_11_JPEGScaleDownFunctionTest_8==true)
    _u8DownScaleRatio = E_JPD_DOWNSCALE_EIGHTH;
    #else
    _u8DownScaleRatio = E_JPD_DOWNSCALE_ORG;
    #endif

    if(_u8DownScaleRatio == E_JPD_DOWNSCALE_HALF)
    {
        _u8ScaleDownFactor = 2;
    }
    else if(_u8DownScaleRatio == E_JPD_DOWNSCALE_FOURTH)
    {
        _u8ScaleDownFactor = 4;
    }
    else if(_u8DownScaleRatio == E_JPD_DOWNSCALE_EIGHTH)
    {
        _u8ScaleDownFactor = 8;
    }
#endif

    if (E_JPD_DOWNSCALE_ORG != _u8DownScaleRatio)
    {
        _u16AlignedImagePitch = pic_width / _u8ScaleDownFactor;
        _u16AlignedImagePitch_H =
            ((pic_height + ((MS_U32)_u8ScaleDownFactor - 1)) & (~((MS_U32)_u8ScaleDownFactor - 1)))/ _u8ScaleDownFactor;
        _u16AlignedImageWidth = _u16Image_x_size / _u8ScaleDownFactor;
        _u16AlignedImageHeight =
                ((_u16AlignedImageHeight + ((MS_U32)_u8ScaleDownFactor - 1)) & (~((MS_U32)_u8ScaleDownFactor - 1))) / _u8ScaleDownFactor;
    }

    switch (_u8DownScaleRatio)
    {
        case E_JPD_DOWNSCALE_HALF:
            ROI_width = (pic_width / 16) * 16;
            break;

        case E_JPD_DOWNSCALE_FOURTH:
            ROI_width = (pic_width / 32) * 32;
            break;

        case E_JPD_DOWNSCALE_EIGHTH:
            ROI_width = (pic_width / 64) * 64;
            break;

        default:
            ROI_width = pic_width;
            break;
    }
    JPEG_DEBUG_API_MSG("Original ROI_width = %d \n", ROI_width);

    if (ROI_width != pic_width)
    {
        _u16AlignedImagePitch = _u16AlignedImageWidth = ROI_width / _u8ScaleDownFactor;

        if(_bProgressive_flag)
            _Progressive_ROI_flag = TRUE;
    }
#endif //#ifndef CMODEL
    return TRUE;
}
#if 0 // not implement for new MDDI - harold
//------------------------------------------------------------------------------
JPEG_STATIC void msAPI_MJPEG_decode_init(PJPEG_FILE_FileSystem_t Pstream)
{
    JPEG_init( Pstream );
    MSAPI_MJPEG_Get_Pics();
    JPEG_locate_sof_marker();

    if(_u8DecodeType == E_JPEG_TYPE_THUMBNAIL)
    {
        if(_bThumbnailFound)
        {
            _bThumbnailAccessMode = TRUE;
            ////MApi_JPEG_Finalize(); //replace with JPEG_free_all_blocks()
            JPEG_free_all_blocks();
            JPEG_init( Pstream );

            JPEG_locate_sof_marker();

            if(_bProgressive_flag)
            {
                JPEG_DEBUG_API_MSG("Progressive image in thumbnail... do not handle it\n");
                JPEG_terminate( E_JPEG_BAD_APP1_MARKER );
            }
        }
        else
        {
            JPEG_terminate( E_JPEG_NO_THUMBNAIL );
        }
    }
}
//------------------------------------------------------------------------------
// Parse header of MJPEG to get total num of pics in the file
JPEG_STATIC void MSAPI_MJPEG_Get_Pics(void)
{
    //Num of pics located at Byte 49~52, hence we need to offset 48 bytes
    MS_U8 ByteOffset, ByteSkip = 48;

    //Total num of pics
    NumPics = 0;

    for(ByteOffset = 0; ByteOffset < ByteSkip; ByteOffset++)
    {
        JPEG_get_char();
    }

    //Handle Little-Ending
    NumPics += (JPEG_get_char());
    NumPics += (JPEG_get_char() << 8);
    NumPics += (JPEG_get_char() << 16);
    NumPics += (JPEG_get_char() << 24);

    JPEG_DEBUG_API_MSG("NumPics = %ld\n",NumPics);
}
#endif // not implement for new MDDI - harold

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
//------------------------------------------------------------------------------
// Loads and dequantizes the next row of (already decoded) coefficients.
// Progressive images only.
JPEG_STATIC MS_BOOL JPEG_load_next_row(void)
{
#ifndef CMODEL
    JPEG_BLOCK_TYPE p[64];
    MS_BOOL EOF_Flag = FALSE; //CL82399
#else
    MS_S16 i;
    JPEG_BLOCK_TYPE *p;
    MS_U16 row_block = 0;
    JPEG_QUANT_TYPE *q;
#endif

    MS_U16 mcu_row, mcu_block;
    MS_U8 component_num, component_id;
    MS_U16 block_x_mcu[JPEG_MAXCOMPONENTS];

    JPEG_memset((void *)block_x_mcu, 0, JPEG_MAXCOMPONENTS * sizeof( MS_U16 ) );

    for ( mcu_row = 0; mcu_row < _u16Mcus_per_row; mcu_row++ )
    {
        MS_U16 block_x_mcu_ofs = 0, block_y_mcu_ofs = 0;

        for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu; mcu_block++ )
        {
            JPEG_BLOCK_TYPE *pAC;
            JPEG_BLOCK_TYPE *pDC;

            component_id = _u8Mcu_org[mcu_block];

#ifdef CMODEL
            p = gps16Block_seg[row_block];
            q = _QuantTables[_u8Comp_quant[component_id]].s16Value;
#endif

            pAC = JPEG_coeff_buf_getp( _AC_Coeffs[component_id], block_x_mcu[component_id] + block_x_mcu_ofs, _u32Block_y_mcu[component_id] + block_y_mcu_ofs );
            if(pAC == NULL)
            {
                JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            pDC = JPEG_coeff_buf_getp( _DC_Coeffs[component_id], block_x_mcu[component_id] + block_x_mcu_ofs, _u32Block_y_mcu[component_id] + block_y_mcu_ofs );
            if(pDC == NULL)
            {
                JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            p[0] = pDC[0];
            JPEG_memcpy((void *)&p[1], (void *)&pAC[1], 63 * sizeof( JPEG_BLOCK_TYPE ) );

#ifdef CMODEL //cmodel do not support scale down feature.
            if(mcu_block==(_u8Blocks_per_mcu -1)&&mcu_row==(_u16Mcus_per_row - 1)&&(_u16Total_lines_left - gu8Max_mcu_y_size)==0)
            {
                if(!JPEG_do_RLE(p, TRUE, component_id, TRUE))   // means it is end of picture
                    return FALSE;
            }
            else
            {
                if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                    return FALSE;
            }
#else //CL82399
            if( _Progressive_ROI_flag == FALSE )
            {
                if((mcu_block == (_u8Blocks_per_mcu - 1))
                && (mcu_row == (_u16Mcus_per_row - 1))
                && (_u16Total_lines_left == gu8Max_mcu_y_size))
                {
                    JPEG_DEBUG_API_MSG("EOF!!!!!ROI enable!!!\n");
                    if(!JPEG_do_RLE(p, TRUE, component_id, TRUE))   // means it is end of picture
                        return FALSE;
                }
                else
                {
                    if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                        return FALSE;
                }
            }
            else
            {
                if( _u16Total_lines_left == gu8Max_mcu_y_size )//Last Line
                {
                    //JPEG_DEBUG_API_MSG("_u16Total_lines_left ==%d,%d,%d \n", gu8Max_mcu_y_size, mcu_block, mcu_row);

                    if((mcu_block == (_u8Blocks_per_mcu - 1))
                    && ((mcu_row + 2) * gu8Max_mcu_x_size >  ROI_width))//Last line last block within rang
                    {
                        if( EOF_Flag == FALSE )
                        {
                            EOF_Flag = TRUE;
                            JPEG_DEBUG_API_MSG("EOF!!!!!No ROI!!!\n");
                            if(!JPEG_do_RLE(p, TRUE, component_id, TRUE))   // means it is end of picture
                                return FALSE;
                        }
                    }
                    else
                    {
                         if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                            return FALSE;
                    }
                }
                else
                {
                    if((mcu_row + 1) * gu8Max_mcu_x_size >  ROI_width)//ever line out rang block
                    {
                        //JPEG_do_RLE(p, FALSE, component_id, FALSE);
                    }
                    else
                    {
                        if(!JPEG_do_RLE(p, FALSE, component_id, TRUE))
                            return FALSE;
                    }
                }
            }
#endif


#ifdef CMODEL
            {
                for ( i = 63; i > 0; i-- )
                {
                    if ( p[_u8ZAG[i]] )
                    {
                        break;
                    }
                }

                //block_num[row_block++] = i + 1;

                for ( ; i >= 0; i-- )
                {
                    if ( p[_u8ZAG[i]] )
                    {
                        p[_u8ZAG[i]] *= q[i];
                    }
                }
            }

            row_block++;
#endif

            if ( _u8Comps_in_scan == 1 )
            {
                block_x_mcu[component_id]++;
            }
            else
            {
                if ( ++block_x_mcu_ofs == _u8Comp_h_samp[component_id] )
                {
                    block_x_mcu_ofs = 0;

                    if ( ++block_y_mcu_ofs == _u8Comp_v_samp[component_id] )
                    {
                        block_y_mcu_ofs = 0;

                        block_x_mcu[component_id] += _u8Comp_h_samp[component_id];
                    }
                }
            }
        }
    }

    if ( _u8Comps_in_scan == 1 )
    {
        _u32Block_y_mcu[_u8Comp_list[0]]++;
    }
    else
    {
        for ( component_num = 0; component_num < _u8Comps_in_scan; component_num++ )
        {
            component_id = _u8Comp_list[component_num];

            _u32Block_y_mcu[component_id] += _u8Comp_v_samp[component_id];
        }
    }
    return TRUE;
}
#endif

#ifndef CMODEL
//------------------------------------------------------------------------------
/******************************************************************************/
///Start JPEG decoding
/******************************************************************************/
JPEG_STATIC MS_BOOL JPEG_StartDecode(void)
{
    MS_U16 pic_width, pic_height;
    MS_U8 Y_VSF = _u8Comp_v_samp[0];
    MS_U8 Y_HSF = _u8Comp_h_samp[0];
    MS_BOOL bUV_en;
    MS_U32 reg_value;
    MS_U8 i;
    MS_U8 com_num = 0;
    MS_U8 comp[JPEG_MAXCOMPONENTS];
    JPD_BufCfg structBuf;

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifdef JPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return FALSE;
    }
#endif

#if ENABLE_JPEG_NO_SIZE_LOWER_BOUND
    MDrv_JPD_SetPicDimension(0xFF, 0xFF);
#endif

    // reset JPD hardware
    //MDrv_JPD_Reset();
    if(_bProgressive_flag)
    {
        MDrv_JPD_Rst_SVLD();
    }
    else
    {
        MDrv_JPD_Rst();
    }


    // Calculate how many valid quantization tables for components
    JPEG_memset((void *)comp, 0, JPEG_MAXCOMPONENTS);
    for(i = 0; i<_u8Comps_in_frame; i++)
    {
        comp[_u8Comp_quant[i]] = 1;
    }

    for(i = 0; i<JPEG_MAXCOMPONENTS; i++)
    {
        if(comp[i]==1)
            com_num++;
    }

    if(_u8Comps_in_frame>1)
        bUV_en = TRUE;
    else
        bUV_en = FALSE;

    //Get Aligned width & height
    JPEG_GetAlignedResolution(&pic_width, &pic_height);

#if 1
    structBuf.bProgressive = _bProgressive_flag;
    structBuf.bThumbnailAccessMode = _bThumbnailAccessMode;
    structBuf.u32ThumbnailBufAddr = MS_VA2PA((MS_U32)INTERNAL_BUFFER_ADDR);
    structBuf.u32ThumbnailBufSize = JPEG_DEFAULT_EXIF_SIZE;
    structBuf.u32ThumbnailBufOffset = _u32ThumbnailOffset + JPEG_GetECS();
    structBuf.u32MRCBufAddr = MS_VA2PA((MS_U32)MRC_BUFFER_ADDR);
    structBuf.u32MRCBufSize = MRC_BUFFER_SIZE;
    structBuf.u32MRCBufOffset = JPEG_GetECS();
    structBuf.u32MWCBufAddr = MS_VA2PA((MS_U32)MWC_BUFFER_ADDR);
    //HW limitation:if we don't enable write-protect mode, set this value to zero.

#if (ENABLE_TEST_09_JPEGWriteProtectTest==false)
    structBuf.u16MWCBufLineNum = 0;
//    structBuf.u16MWCBufLineNum = ((pic_height/_u8ScaleDownFactor + 8) < 0x07FF)?(pic_height/_u8ScaleDownFactor + 8):0x7FF;
#else
    structBuf.u16MWCBufLineNum = 32;
#endif

    JPEG_DEBUG_API_MSG("bProgressive = %d, bThumbnailAccessMode = %d\n"
    , structBuf.bProgressive, structBuf.bThumbnailAccessMode);
    JPEG_DEBUG_API_MSG("u32ThumbnailBufAddr = 0x%lX, u32ThumbnailBufSize = 0x%lX, u32ThumbnailBufOffset = 0x%lX\n"
    , structBuf.u32ThumbnailBufAddr, structBuf.u32ThumbnailBufSize, structBuf.u32ThumbnailBufOffset);
    JPEG_DEBUG_API_MSG("u32MRCBufAddr = 0x%lX, u32MRCBufSize = 0x%lX, u32MRCBufOffset = 0x%lX\n"
    , structBuf.u32MRCBufAddr, structBuf.u32MRCBufSize, structBuf.u32MRCBufOffset);
    JPEG_DEBUG_API_MSG("u32MWCBufAddr = 0x%lX, u16MWCBufLineNum = %d\n"
    , structBuf.u32MWCBufAddr, structBuf.u16MWCBufLineNum);

    u32DataOffset += JPEG_GetECS();

    MDrv_JPD_InitBuf(structBuf);

//    JPEG_DEBUG_API_MSG("MWCLineNum = %d, pic_height = %d, DwnScaleRatio = %d\n"
//            , structBuf.u16MWCBufLineNum, pic_height, _u8ScaleDownFactor);
#else
    if(_bThumbnailAccessMode)
    {
        // Set MRC buffer for JPD
        MDrv_JPD_SetReadBuffer(INTERNAL_BUFFER_ADDR, JPEG_DEFAULT_EXIF_SIZE);
        // Set MRC start access byte address
        MDrv_JPD_SetMRCStartAddr(INTERNAL_BUFFER_ADDR + _u32ThumbnailOffset + JPEG_GetECS());
    }
    else
    {
        if(_bProgressive_flag)
        {
            // Set MRC buffer for JPD
            MDrv_JPD_SetReadBuffer(MRC_BUFFER_ADDR, MRC_BUFFER_SIZE);
            // Set MRC start access byte address
            MDrv_JPD_SetMRCStartAddr(MRC_BUFFER_ADDR);
        }
        else
        {
            // Set MRC buffer for JPD
            MDrv_JPD_SetReadBuffer(MRC_BUFFER_ADDR, MRC_BUFFER_SIZE);
            // Set MRC start access byte address
            MDrv_JPD_SetMRCStartAddr(MRC_BUFFER_ADDR + JPEG_GetECS());
        }
    }

    // Set MWC buffer for JPD
    MDrv_JPD_SetOutputFrameBuffer(MWC_BUFFER_ADDR);
#endif

    // Set picture width and height
#if (ENABLE_JPEG_NO_SIZE_LOWER_BOUND == 0)
    MDrv_JPD_SetPicDimension(pic_width, pic_height);
#endif

    _u16NonAlignmentImage_x_size = pic_width;
    _u16NonAlignmentImage_y_size = _u16Image_y_size;

    _u16Image_x_size = pic_width/_u8ScaleDownFactor;
    _u16Image_y_size = pic_height/_u8ScaleDownFactor;
    JPEG_DEBUG_API_MSG("ScaleDownFactor = %d\n", _u8ScaleDownFactor);

    // In JPD, software VLD mode, we don't need to write huff & symbol tables
    if(_bProgressive_flag==FALSE)
    {
        //JPEG_write_Scwgif();
        JPEG_WriteGrpinf();
        //JPEG_write_symidx();
        JPEG_WriteSymidx();
    }

    //JPEG_write_Qtbl();
    JPEG_WriteIQTbl();

    Y_VSF -= 1;
    if ( Y_HSF == 4 )
    {
        Y_HSF = 3;
    }

    if(_u16Restart_interval)
    {
        JPEG_DEBUG_API_MSG("RST found! Enable JPD_RST_EN! Restart_interval = %d\n", _u16Restart_interval);
        //MDrv_Write2Byte(BK_JPD_RSTINTV, _u16Restart_interval - 1);
        MDrv_JPD_SetRSTIntv(_u16Restart_interval - 1);
        reg_value = ( JPD_TBL_RDY | JPD_RST_EN | ((MS_U32) _u8DownScaleRatio) << 4 | ((MS_U32) bUV_en) << 3 | ( Y_VSF << 2 ) | Y_HSF );
    }
    else
    {
        reg_value = ( JPD_TBL_RDY | ((MS_U32) _u8DownScaleRatio) << 4 | ((MS_U32) bUV_en) << 3 | ( Y_VSF << 2 ) | Y_HSF );
    }

    // There're Q tables for U & V, respectively.
    if(com_num>2)
    {
        JPEG_DEBUG_API_MSG("More than two Q tables! Enable JPD_SUVQ! comnum = %d\n", com_num);
        reg_value = reg_value | JPD_SUVQ;
    }

    /* Check the read pointer. If it is in HIGH buffer, we need
       to preload data to LOW buffer and then start decode.
       Progressive mode & thumbnail don't need to check it. */
    if((FALSE == _bProgressive_flag)
    && (FALSE == _bThumbnailAccessMode))
    {
        MS_U32 start_offset = JPEG_GetECS();
        JPEG_DEBUG_API_MSG("start offset = 0x%lx\n", start_offset);
        if ( start_offset >= (MRC_BUFFER_SIZE/2) )
        {
            JPEG_DEBUG_API_MSG("ReadPtr is in HIGH, Load LOW!!\n");
            if(_pFillHdrFunc)
            {
                MS_S32 byte_read = _pFillHdrFunc((MS_PHYADDR)MS_VA2PA((MS_U32)_pu8In_buf), (MRC_BUFFER_SIZE/2));
                if(byte_read < 0)
                {
                    JPEG_terminate(E_JPEG_STREAM_READ);
                    return FALSE;
                }
                u8PreLHFlag = E_JPEG_BUFFER_LOW;
                u32MRCheckCount = 0;
            }
            else
            {
                JPEG_DEBUG_API_MSG("_pFillHdrFunc is not registered!!, still need to start decode.\n");
            }
        }
    }

    if(_bProgressive_flag)
        reg_value = reg_value | JPD_SVLD;

    // Check if it needs to do ROI
    if(E_JPD_DOWNSCALE_ORG != _u8DownScaleRatio)
    {
        if(ROI_width != pic_width)
        {
            JPEG_DEBUG_API_MSG("ROI!! ROI_width = %d, ROI_height = %d\n", ROI_width, pic_height);
            MDrv_JPD_SetROI(0, 0, (ROI_width>>3), (pic_height>>3));
            reg_value = reg_value | JPD_ROI_EN;
        }
        _u16Image_x_size = ROI_width/_u8ScaleDownFactor;
    }

    //Enable ECO support
    {
        MS_U16 reg_val = 0;
        reg_val = JPD_RST_STUFF_BYTE_HANDLE;
#if (JPD_SUPPORT_3_HUFFMAN_TABLE==true)
        if(TRUE == bIs3HuffTbl)
        {
            reg_val = reg_val | JPD_3_HUFFMAN_TABLE_SUPPORT;
        }
#endif
        MDrv_JPD_SetSpare(reg_val);
    }

    // mark low/high buffer valid
    //MDrv_Write2Byte( BK_JPD_MCONFIG, JPD_H_VLD |JPD_L_VLD);
    MDrv_JPD_Set_M_Config(JPD_H_VLD |JPD_L_VLD);

#if (ENABLE_TEST_18_miu_sel_128M==true) \
    ||(ENABLE_TEST_18_miu_sel_64M==true) \
    ||(ENABLE_TEST_18_miu_sel_32M==true)
// This is used for verification code in TEST_18_miu_sel
//        MDrv_JPD_Set_M_Config(MDrv_JPD_Get_M_Config());             // 0 1
    #if (ENABLE_TEST_18_miu_sel_128M==true)
        MDrv_JPD_Set_M_Config(MDrv_JPD_Get_M_Config() | JPD_BIT(9));          // 0 1 0 1
    #elif (ENABLE_TEST_18_miu_sel_64M==true)
        MDrv_JPD_Set_M_Config(MDrv_JPD_Get_M_Config() | JPD_BIT(10));   // 0 1 0 1 0 1 0 1
    #else
        MDrv_JPD_Set_M_Config(MDrv_JPD_Get_M_Config() | JPD_BIT(9) | JPD_BIT(10));    // 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
    #endif
#endif

    // enable JPD decoding
#if ENABLE_JPEG_NO_SIZE_LOWER_BOUND
    MDrv_JPD_SetPicDimension(pic_width, pic_height);
#endif

    //MDrv_Write2Byte( BK_JPD_SCONFIG, reg_value | JPD_DEC_EN | MDrv_JPD_GetSWResetMask());
    //MDrv_JPD_Set_S_Config(reg_value | JPD_DEC_EN | MDrv_JPD_GetSWResetMask());
    //MDrv_JPD_Set_S_Config(reg_value | JPD_DEC_EN | JPD_SWRST);
#if 1
    // decide if we need to set the JPD_PSAVE_EN register in hal layer
    MDrv_JPD_Set_S_Config(reg_value | JPD_SWRST | JPD_PSAVE_EN);
#else
#if (ENABLE_TEST_16_JPEGEnablePsaveModeTest==false)
#if defined(CHIP_A3) || defined(CHIP_E3) || defined(CHIP_A5) || defined(CHIP_A5P) ||  defined(CHIP_A6) || defined(CHIP_A7)
    // For safety, enable the JPD_PSAVE_EN as default since A5 (HW already fixed this bug since A1 or earlier chip)
    MDrv_JPD_Set_S_Config(reg_value | JPD_SWRST | JPD_PSAVE_EN);
#else
    //due to power saving mode will make system hang in some case(the image of RST error),we do not enable this function currently.
    MDrv_JPD_Set_S_Config(reg_value | JPD_SWRST);
#endif
#else
#if defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_JANUS) || defined(CHIP_U4) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A1) || defined(CHIP_A2) || defined(CHIP_A5) || defined(CHIP_A5P) || defined(CHIP_A6) || defined(CHIP_A7) || defined(CHIP_A3) || defined(CHIP_AMETHYST) || defined(CHIP_E3)
    //Enable power save mode on normal JPEG decoding.
    MDrv_JPD_Set_S_Config(reg_value | JPD_SWRST | JPD_PSAVE_EN);
#else
    MDrv_JPD_Set_S_Config(reg_value | JPD_SWRST);
#endif
#endif
#endif

// protect test(only for unit test)
#if (ENABLE_TEST_09_JPEGWriteProtectTest==false)
    //MDrv_JPD_SetWriteProtect(TRUE);
#else
    #if(JPD_SUPPORT_AUTO_PROTECT==false)
    MDrv_JPD_SetWriteProtect(TRUE);
    #endif
#endif

#if (JPD_SUPPORT_AUTO_PROTECT==true)
    MDrv_JPD_SetAutoProtect(TRUE);
    #if (ENABLE_TEST_22_AutoProtectFailTest == true)
        MDrv_JPD_SetWPENEndAddr(MWC_BUFFER_ADDR+0x200-1);
        JPEG_DEBUG_API_MSG("Set Auto protect address =0x%lx!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", MWC_BUFFER_ADDR+0x200);
    #else
        MDrv_JPD_SetWPENEndAddr(MWC_BUFFER_ADDR+MWC_BUFFER_SIZE-1);
    #endif
#endif

    MDrv_JPD_Set_S_Config(MDrv_JPD_Get_S_Config() | JPD_DEC_EN);
    JPEG_DEBUG_API_MSG("After Setting SCONFIG, JPD START!!\n");
    //JPEG_GO;
    return TRUE;
}
#endif

#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
//------------------------------------------------------------------------------
/******************************************************************************/
///Start Progressive JPEG decode for JPD
/******************************************************************************/
JPEG_STATIC JPEG_Result JPEG_Progressive_Decode(void)
{
    if(_bProgressive_flag==FALSE)
        return (E_JPEG_FAILED);

    if ( _u16Total_lines_left == 0 )
    {
        return ( E_JPEG_OKAY );
    }

#ifdef JPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return ( E_JPEG_FAILED );
    }
#endif

    //JPEG_DEBUG_API_MSG("_u16Mcus_per_row is %d!\n",_u16Mcus_per_row);
    //JPEG_DEBUG_API_MSG("_u16Mcus_per_col is %d!\n",_u16Mcus_per_col);
    //JPEG_DEBUG_API_MSG("_u8Blocks_per_mcu is %d!\n",_u8Blocks_per_mcu);
    //JPEG_DEBUG_API_MSG("gu8Max_mcu_x_size is %d!\n",gu8Max_mcu_x_size);
    //JPEG_DEBUG_API_MSG("gu8Max_mcu_y_size is %d!\n",gu8Max_mcu_y_size);

    if( _u16Total_lines_left > 0 )
    {
        JPEG_DEBUG_API_MSG("%s:_u16Total_lines_left = %d\n", __FUNCTION__ , _u16Total_lines_left);
        if(!JPEG_load_next_row())
            return E_JPEG_FAILED;

#if SW_JPD_RGB_CMYK
        if((E_JPEG_CMYK == gu8Scan_type)
        || (E_JPEG_RGB == gu8Scan_type))
        {
            if(!msAPI_JPEG_transform_row()) //IDCT
            {
                JPEG_DEBUG_API_MSG("CMYK and RGB decode failed!!\n");
                return ( E_JPEG_FAILED );
            }
        }
#endif

        _u16Total_lines_left -= gu8Max_mcu_y_size;
    }

    return (E_JPEG_DONE);
}
#endif

//------------------------------------------------------------------------------
JPEG_STATIC void JPEG_GetAlignedResolution(MS_U16 *width, MS_U16 *height)
{
    MS_U8 mcu_width, mcu_height;
    MS_U8 Y_VSF = _u8Comp_v_samp[0];
    MS_U8 Y_HSF = _u8Comp_h_samp[0];

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    *width = _u16OriginalImage_x_size;
    *height = _u16OriginalImage_y_size;

    mcu_width = _u16OriginalImage_x_size % (Y_HSF * 8);
    if (mcu_width)
    {
        *width += (Y_HSF * 8 - mcu_width);
    }

    mcu_height = _u16OriginalImage_y_size % (Y_VSF * 8);
    if (mcu_height)
    {
        *height += (Y_VSF * 8 - mcu_height);
    }

    JPEG_DEBUG_API_MSG("_u8Comp_v_samp = %d, _u8Comp_h_samp = %d\n", _u8Comp_v_samp[0], _u8Comp_h_samp[0]);
    JPEG_DEBUG_API_MSG("_u16OriginalImage_x_size = %d, _u16OriginalImage_y_size = %d\n", _u16OriginalImage_x_size, _u16OriginalImage_y_size);
    JPEG_DEBUG_API_MSG("AlignWidth = %d, AlignHeight = %d\n", *width, *height);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if 0
//------------------------------------------------------------------------------
/******************************************************************************/
///This will set MRC buffer address & size, MWC buffer address, and internal buffer
///address & size.
///@param *pInitParam \b The pointer of information for JPEG Buffer initialization
/// structure {
///         U32 u32MRCBufAddr, \b IN MRC buffer address
///         U32 u32MRCBufSize, \b IN MRC buffer size
///         U32 u32MWCBufAddr, \b IN MWC buffer address
///                                 JPEG decompress data (YUV422)
///         U32 u32InternalBufAddr, \b IN internal buffer address
///                                 (exif:64K + memory pool)
///         U32 u32InternalBufSize, \b IN internal buffer size
///         BOOL bInitMem, \b IN initialize memory pool or not
///         };
///@return true or false
/******************************************************************************/
MS_BOOL MApi_JPEG_SetInitParameter(JPEG_InitParam *pInitParam)
{
    // the buffer size must be multiple of 4 bytes
    if((!pInitParam)
    || (pInitParam->u32MRCBufSize < MIN_READBUFFER_SIZE))
    {
        JPEG_terminate(E_JPEG_READBUFFER_TOOSMALL);
        return false;
    }
    // the buffer size must be multiple of 8 bytes
    pInitParam->u32MRCBufSize = pInitParam->u32MRCBufSize & ~0x7;

    MRC_BUFFER_ADDR = pInitParam->u32MRCBufAddr | AEON_NON_CACHE_MASK;
    MWC_BUFFER_ADDR = pInitParam->u32MWCBufAddr | AEON_NON_CACHE_MASK;
    INTERNAL_BUFFER_ADDR = pInitParam->u32InternalBufAddr | AEON_NON_CACHE_MASK;
    MRC_BUFFER_SIZE = pInitParam->u32MRCBufSize;
    INTERNAL_BUFFER_SIZE = pInitParam->u32InternalBufSize;

    _pu8In_buf = (MS_U8 *) pInitParam->u32MRCBufAddr;

    if (pInitParam->bInitMem)
    {
        JPEG_MEMORY_init_mempool(((MS_U8 *)(pInitParam->u32InternalBufAddr + JPEG_DEFAULT_EXIF_SIZE))
                                , (pInitParam->u32InternalBufSize - JPEG_DEFAULT_EXIF_SIZE));
    }

    return TRUE;
}
//------------------------------------------------------------------------------
// Call get_error_code() after constructing to determine if the stream
// was valid or not. You may call the get_width(), get_height(), etc.
// methods after the constructor is called.
// You may then either destruct the object, or begin decoding the image
// by calling begin(), then decode().
void MApi_JPEG_Constructor( PJPEG_FILE_FileSystem_t Pstream, MS_U8 decode_type)
{
    if ( setjmp( _jmp_state ) )
    {
        return;
    }

    _u8DecodeType = decode_type;
    JPEG_init_thumbnail();
    JPEG_decode_init( Pstream);

#ifdef CMODEL
    _bReady_flag = TRUE;
#endif
}
#endif

#if SW_JPD_RGB_CMYK
MS_BOOL MApi_JPEG_Is_RGB_CMYK(void)
{
    if (_u8Comps_in_frame == 3)
    {
        //support only U_H1V1 & V_H1V1
        if ((_u8Comp_h_samp[1] != 1 || _u8Comp_v_samp[1] != 1)
            || (_u8Comp_h_samp[2] != 1 || _u8Comp_v_samp[2] != 1))
        {
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
        }

        if (_u8Comp_h_samp[0] == 1 && _u8Comp_v_samp[0] == 1)
        {
            if (_u8Comp_ident[0] == 82 || _u8Comp_ident[0] == 71 || _u8Comp_ident[0] == 66)
                return TRUE; // RGB
        }
    }
    else
    if (_u8Comps_in_frame == 4) //handle YCCK & CMYK case, must distinguish YCCK and CMYK later
    {
        if (_u8Comp_h_samp[0] == 1 && _u8Comp_v_samp[0] == 1)
            return TRUE; // CMYK
    }

    return FALSE;
}
#endif
//------------------------------------------------------------------------------
/********************************************************************/
///This function will\n
///1. set MRC buffer address & size, MWC buffer address, and internal buffer address & size.\n
///2. power on JPD\n
///3. reset thumbnail parameters\n
///4. set the stream and its size, the number of components, etc.\n
///Call MApi_JPEG_GetErrorCode() after it to determine if the stream was valid or not.\n
///@param -pInitParam \b IN : \n
/// structure {\n
///     MS_PHYADDR \b u32MRCBufAddr : MRC buffer address\n
///     MS_U32 \b u32MRCBufSize : MRC buffer size\n
///     MS_PHYADDR \b u32MWCBufAddr : MWC buffer address(JPEG decompress data (YUV422))\n
///     MS_U32 \b u32MWCBufSize : MWC buffer size\n
///     MS_PHYADDR \b u32InternalBufAddr : Internal buffer address(exif:64K + memory pool)\n
///     MS_U32 \b u32InternalBufSize : Internal buffer size\n
///     MS_U32 \b u32DecByteRead : how many byte read for JPEG decoder initialization\n
///     MS_BOOL \b bEOF : has read to the end of file\n
///     MS_U8 \b u8DecoderType : 3 types: JPEG main, thumbnail, or MJPEG\n
///     MS_BOOL \b bInitMem : initialize memory pool or not\n
///     JPEG_FillHdrFunc \b pFillHdrFunc : the function for fill header information\n
/// };
///@return \b JPEG_Result : JPEG init status
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_Init(JPEG_InitParam *pInitParam)
{
#if 0
    MApi_JPEG_SetDbgLevel(E_JPEG_DEBUG_ALL);
#endif

#if LOG_DATA_TO_USB
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','l','o', 'g', '0', '0', '0', '.', 'b', 'i', 'n',0};
    fileName2[15]++;
    if(fileName2[15] > '9')
    {
        fileName2[14]++;
        fileName2[15] = '0';
    }
    if(fileName2[14] > '9')
    {
        fileName2[13]++;
        fileName2[14] = '0';
    }

    if((logBinfp = fopen((char *)fileName2, "wb"))==NULL) {
        printf("Can't open /usb/sda1/log.bin\n");
    }
    else
    {
        printf("open /usb/sda1/log.bin success\n");
    }
#endif

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

    // jpeg constructor
#ifdef JPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return  E_JPEG_FAILED;
    }
#endif

    if(_bIsInit == TRUE)
    {
#if SUPPORT_MPO_FORMAT
        if(MApi_JPEG_IsMPOFormat()==FALSE)
        {
            JPEG_terminate(E_JPEG_RE_INIT);
            return E_JPEG_FAILED;
        }
#else
        JPEG_terminate(E_JPEG_RE_INIT);
        return E_JPEG_FAILED;
#endif
    }
    else
    {
        _bIsInit = TRUE;
    }

    if(!pInitParam)
    {
        JPEG_terminate(E_JPEG_DECODE_ERROR);
        return E_JPEG_FAILED;
    }

//    if(pInitParam->u32MRCBufSize < MIN_READBUFFER_SIZE)
    if(pInitParam->u32MRCBufSize < JPEG_DEFAULT_EXIF_SIZE)
    {
        JPEG_terminate(E_JPEG_READBUFFER_TOOSMALL);
        return E_JPEG_FAILED;
    }

    // the buffer size must be multiple of 8 bytes
    pInitParam->u32MRCBufSize = pInitParam->u32MRCBufSize & ~0x7;

    MRC_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32MRCBufAddr);// | AEON_NON_CACHE_MASK;
    MWC_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32MWCBufAddr);// | AEON_NON_CACHE_MASK;
    INTERNAL_BUFFER_ADDR = MS_PA2KSEG1((MS_U32)pInitParam->u32InternalBufAddr);// | AEON_NON_CACHE_MASK;
    if (__bIsMjpeg)
    {
        JPEG_DEBUG_API_MSG ("MJPEG Read buffer address CONVERT : 0x%lx - 0x%lx + 0x%lx ",
                MS_VA2PA(MRC_BUFFER_ADDR), MS_VA2PA(__u32RealMjpegBase), MS_VA2PA(__u32TrickyMjpegBase));
        MRC_BUFFER_ADDR -= __u32RealMjpegBase;
        MRC_BUFFER_ADDR += __u32TrickyMjpegBase;
        JPEG_DEBUG_API_MSG ("= 0x%lx\n", MS_VA2PA(MRC_BUFFER_ADDR));
    }
    _pu8In_buf = (MS_U8 *)MRC_BUFFER_ADDR;//pInitParam->u32MRCBufAddr;

    MRC_BUFFER_SIZE = pInitParam->u32MRCBufSize;
    MWC_BUFFER_SIZE = pInitParam->u32MWCBufSize;
    INTERNAL_BUFFER_SIZE = pInitParam->u32InternalBufSize;


    JPEG_DEBUG_API_MSG("MRC_ADDR = 0x%lX, MRC_BUFFER_SIZE = 0x%lX\n", _u32ReadBufferAddr, _u32ReadBufferSize);
    JPEG_DEBUG_API_MSG("MWC_ADDR = 0x%lX, MWC_BUFFER_SIZE = 0x%lX\n", _u32WriteBufferAddr, _u32WriteBufferSize);
    JPEG_DEBUG_API_MSG("INTERNAL_ADDR = 0x%lX, INTERNAL_BUFFER_SIZE = 0x%lX\n", _u32InternalBufferAddr, _u32InternalBufferSize);
    JPEG_DEBUG_API_MSG ("MRC_PA= 0x%lx\n", MS_VA2PA(MRC_BUFFER_ADDR));
    JPEG_DEBUG_API_MSG ("MWC_PA= 0x%lx\n", MS_VA2PA(MWC_BUFFER_ADDR));
    JPEG_DEBUG_API_MSG ("INTER_PA= 0x%lx\n", MS_VA2PA(INTERNAL_BUFFER_ADDR));

    _pFillHdrFunc = pInitParam->pFillHdrFunc;

#if SW_JPD_RGB_CMYK
    u8Out_buf = (MS_U8 *) MWC_BUFFER_ADDR;
    u32_Decode_Line = 0;
#endif

    //JPEG_GO;

    // powner on JPD
#ifndef CMODEL
    MDrv_JPD_PowerOn();
#endif
    JPEG_DEBUG_API_MSG("MDrv_JPD_PowerOn OK!!\n");

    _u8DecodeType = pInitParam->u8DecodeType;

    _u32In_buf_left = pInitParam->u32DecByteRead;
    _bEOF_flag = pInitParam->bEOF;

    JPEG_DEBUG_API_MSG("@1,  _pu8In_buf_ofs=0x%lx, _u32In_buf_left=0x%lx, _bEOF_flag=%d========\n",
    (MS_U32)_pu8In_buf_ofs, (MS_U32)_u32In_buf_left, _bEOF_flag);
    // Try to get more bytes.
    if(!JPEG_force_fill_read_buffer(_u32In_buf_left))
    {
        JPEG_DEBUG_API_MSG("JPEG_force_fill_read_buffer failed!!!\n");
        _u32In_buf_left = pInitParam->u32DecByteRead;
    }
    JPEG_DEBUG_API_MSG("@2,  _pu8In_buf_ofs=0x%lx, _u32In_buf_left=0x%lx, _bEOF_flag=%d========\n",
    (MS_U32)_pu8In_buf_ofs, (MS_U32)_u32In_buf_left, _bEOF_flag);


#if SUPPORT_MPO_FORMAT
    if(_u32In_buf_MPO_left ==0xFFFFFFFFUL)
    {
        _pu8In_buf_MPO_ofs = (MS_U8*)_pu8In_buf;
        _u32In_buf_MPO_left = _u32In_buf_left;
        mpo_load_data = TRUE;
        bIsMPOFormat = FALSE;
        u32MPFOffset = 0;
        _u32App2MarkerOffset = 0;

        //Init MPO database
        JPEG_InitMPO();
    }
#endif

    u32DataOffset = 0;
    u32SOFOffset = 0;

    JPEG_init_thumbnail();

    if (_u8DecodeType == E_JPEG_TYPE_MJPEG)
    {
        __bIsMjpeg = TRUE;
        __u32RealMjpegBase = MRC_BUFFER_ADDR;
        __u32TrickyMjpegBase = INTERNAL_BUFFER_ADDR;

        return E_JPEG_OKAY;
    }

    if(!JPEG_decode_init())
        return E_JPEG_FAILED;

    if (bMHEG5)
    {
        if(_Error_code == E_JPEG_UNSUPPORTED_MARKER)
            return E_JPEG_FAILED;
    }

#ifndef CMODEL
    //init mempool for progressive decoding.
#if SW_JPD_RGB_CMYK
    if (pInitParam->bInitMem && (_bProgressive_flag || MApi_JPEG_Is_RGB_CMYK()))
#else
    if (pInitParam->bInitMem && _bProgressive_flag)
#endif
#endif
    {
        if(!JPEG_MEMORY_init_mempool((void *)(INTERNAL_BUFFER_ADDR + JPEG_DEFAULT_EXIF_SIZE)
                                , (INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE)))
        {
            JPEG_DEBUG_API_MSG("JPEG_MEMORY_init_mempool FAIL!!\n");
            return E_JPEG_FAILED;
        }
        JPEG_DEBUG_API_MSG("JPEG_MEMORY_init_mempool OK!!\n");
    }

    //JPEG_GO;

#ifdef CMODEL
    _bReady_flag = TRUE;
#endif

    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Decode JPEG header
///@return \b JPEG_Result : JPEG header decode status
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_DecodeHdr(void)
{
#if SW_JPD_RGB_CMYK
    MS_U8 mcu_width, mcu_height;
    MS_U16 pic_width = _u16Image_x_size;
    MS_U16 pic_height = _u16Image_y_size;
    MS_U8 Y_VSF = _u8Comp_v_samp[0];
    MS_U8 Y_HSF = _u8Comp_h_samp[0];
#endif

#if PRINT_JPD_DECODE_TIME
    u32MeasureDecodeTime = MsOS_GetSystemTime();
    u32MeasureDecodeTimeSW = 0;
    u32MeasureDecodeTimeHW = 0;
#endif

    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);

#ifdef JPD_LONGJUMP_SUPPORT
    if ( setjmp( _jmp_state ) )
    {
        return ( E_JPEG_FAILED );
    }
#endif

    if(!JPEG_init_frame())
    {
        JPEG_DEBUG_API_ERR("%s [%d]\n", __FUNCTION__, __LINE__);
        return E_JPEG_FAILED;
    }

    if ( _bProgressive_flag )
    {
#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
        if(!JPEG_init_progressive())
            return E_JPEG_FAILED;
#endif
    }
    else
    {
        if(!JPEG_init_sequential())
            return E_JPEG_FAILED;
    }

#if SW_JPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        if ( ( mcu_width = pic_width % ( Y_HSF * 8 ) ) )
        {
            pic_width += ( Y_HSF * 8 - mcu_width );
        }

        if ( ( mcu_height = pic_height% ( Y_VSF * 8 ) ) )
        {
            pic_height += ( Y_VSF * 8 - mcu_height );
        }

        JPEG_DEBUG_API_MSG("Y_HSF = %d and Y_VSF = %d\n",Y_HSF, Y_VSF);

        JPEG_DEBUG_API_MSG("mcu_width = %d and mcu_height = %d\n",mcu_width, mcu_height);

        _u16NonAlignmentImage_x_size = pic_width;
        _u16NonAlignmentImage_y_size = _u16Image_y_size;
        _u16Image_x_size = pic_width;
        _u16Image_y_size = pic_height;

        JPEG_DEBUG_API_MSG("Width = %d and Height = %d\n",JPEG_MAX_WIDTH, JPEG_MAX_HEIGHT);

        //Set the Scale down variable
        if(_u16Image_x_size > JPEG_MAX_WIDTH*4 || _u16Image_y_size>JPEG_MAX_HEIGHT*4)
        {
            _u8ScaleDownFactor = E_JPD_DOWNSCALE_EIGHTH;
            _u16Image_x_size= (_u16Image_x_size/ 64)*64;
            _u16Image_x_size = _u16Image_x_size/ 8;
            _u16Image_y_size = _u16Image_y_size/ 8;
            JPEG_DEBUG_API_MSG("down scale 1/8!!\n");
        }
        else if(_u16Image_x_size > JPEG_MAX_WIDTH*2 || _u16Image_y_size>JPEG_MAX_HEIGHT*2)
        {
            _u8ScaleDownFactor = E_JPD_DOWNSCALE_FOURTH;
            _u16Image_x_size= (_u16Image_x_size/ 32)*32;
            _u16Image_x_size = _u16Image_x_size/ 4;
            _u16Image_y_size = _u16Image_y_size/ 4;
             JPEG_DEBUG_API_MSG("down scale 1/4!!\n");
        }
        else if (_u16Image_x_size > JPEG_MAX_WIDTH || _u16Image_y_size>JPEG_MAX_HEIGHT)
        {
            _u8ScaleDownFactor = E_JPD_DOWNSCALE_HALF;
            _u16Image_x_size= (_u16Image_x_size/ 16)*16;
            _u16Image_x_size = _u16Image_x_size/ 2;
            _u16Image_y_size = _u16Image_y_size/ 2;
             JPEG_DEBUG_API_MSG("down scale 1/2!!\n");
        }
        else
        {
             JPEG_DEBUG_API_MSG("down scale 1/1!!\n");
            _u8ScaleDownFactor = E_JPD_DOWNSCALE_ORG;

        }

        _u16PaddingMcuNumber = (_u16Image_x_size << _u8ScaleDownFactor)/gu8Max_mcu_x_size;
        _u32Max_blocks_per_row = _u16PaddingMcuNumber* _u16Max_blocks_per_mcu;

        JPEG_DEBUG_API_MSG("down scale width : %d\n",_u16Image_x_size);
        JPEG_DEBUG_API_MSG("down scale height : %d\n",_u16Image_y_size);
        JPEG_DEBUG_API_MSG("down scale MCU : %d\n",_u16PaddingMcuNumber);
        JPEG_DEBUG_API_MSG("down scale MCU : %ld\n",_u32Max_blocks_per_row);
    }
#endif

    if (__bIsMjpeg)
    {
        JPEG_DEBUG_API_MSG ("MJPEG Read buffer address RECOVER : 0x%lx - 0x%lx + 0x%lx ",
                MS_VA2PA(MRC_BUFFER_ADDR), MS_VA2PA(__u32TrickyMjpegBase), MS_VA2PA(__u32RealMjpegBase));
        MRC_BUFFER_ADDR -= __u32TrickyMjpegBase;
        MRC_BUFFER_ADDR += __u32RealMjpegBase;
        JPEG_DEBUG_API_MSG ("= 0x%lx\n", MS_VA2PA(MRC_BUFFER_ADDR));
    }

    return ( E_JPEG_OKAY );
}
//------------------------------------------------------------------------------
/********************************************************************/
///Decode JPEG data (baseline & progressive)
///@return \b JPEG_Result : JPEG decode status
///     - E_JPEG_DONE : For progressive decoding one line done.
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_Decode(void)
{
    JPEG_Result retVal = E_JPEG_FAILED;

    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    JPEG_DEBUG_API_MSG("%s: _bProgressive_flag = %d\n", __FUNCTION__ , _bProgressive_flag);

    if(_bProgressive_flag)
    {
#if SUPPORT_PROGRESSIVE_MODE || defined(CMODEL)
        retVal = JPEG_Progressive_Decode();
#endif
    }
    else
    {
#if SW_JPD_RGB_CMYK
        if((E_JPEG_CMYK == gu8Scan_type)
        || (E_JPEG_RGB == gu8Scan_type))
        {
            retVal = msAPI_JPEG_Baseline_Decode();
            return retVal;
        }
#endif

#ifndef CMODEL
        JPEG_StartDecode();
#if PRINT_JPD_DECODE_TIME
        u32MeasureDecodeTimeSW = MsOS_GetSystemTime()-u32MeasureDecodeTime;
        u32MeasureDecodeTime = MsOS_GetSystemTime();
#endif
        if(_Error_code == E_JPEG_NO_ERROR)
            retVal = E_JPEG_OKAY;
        else
            retVal = E_JPEG_FAILED;
#endif
    }
    return retVal;
}

#if LOG_DATA_TO_USB
static void _PrintOutputMem(MS_U32 u32addr, MS_U32 u32Size)
{
    u32Size = ALIGN_16(u32Size);
    static MS_U8 fileName2[]={'/','u','s','b','/','s','d','a','1','/','0','0', '0', '0', 'x','0','0', '0', '0', '_','0', '0', '0', '.', 'y', 'u', 'y', 'v',0};
    fileName2[10]=MApi_JPEG_GetWidth()/1000+'0';
    fileName2[11]=(MApi_JPEG_GetWidth()/100)%10+'0';
    fileName2[12]=(MApi_JPEG_GetWidth()/10)%10+'0';
    fileName2[13]=MApi_JPEG_GetWidth()%10+'0';
    fileName2[15]=MApi_JPEG_GetHeight()/1000+'0';
    fileName2[16]=(MApi_JPEG_GetHeight()/100)%10+'0';
    fileName2[17]=(MApi_JPEG_GetHeight()/10)%10+'0';
    fileName2[18]=MApi_JPEG_GetHeight()%10+'0';

    fileName2[22]++;
    if(fileName2[22] > '9')
    {
        fileName2[21]++;
        fileName2[22] = '0';
    }
    if(fileName2[21] > '9')
    {
        fileName2[20]++;
        fileName2[21] = '0';
    }

    FILE *outBinfp;
    if((outBinfp = fopen((char *)fileName2, "wb"))==NULL) {
        printf("Can't open output yuyv file\n");
    }
    else
    {
        printf("open output yuyv file success\n");
    }

    fwrite((void *)u32addr, 1, u32Size, outBinfp);
    fclose(outBinfp);

}
#endif

//------------------------------------------------------------------------------
/********************************************************************/
/// Free all block of JPD. Complete destroy the decoder object.
/// It could be called at any time.
///@return None
/********************************************************************/
void _MApi_JPEG_Exit(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    else
    {
        _bIsInit = FALSE;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifndef CMODEL
    JPEG_DEBUG_API_MSG("MApi_JPEG_GetCurReadStatus : \nCurVidx = %d, CurRow = %d, CurCol = %d ",
        MDrv_JPD_GetCurVidx(),
        MDrv_JPD_GetCurRow(),
        MDrv_JPD_GetCurCol());
    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_JPD_GetCurMRCAddr());

    MDrv_JPD_PowerOff();
#endif
    JPEG_free_all_blocks();

#if SW_JPD_RGB_CMYK
    bEnableCMYK = TRUE;
    bEnableRGB = TRUE;
#endif

#if SUPPORT_MPO_FORMAT
    _u32In_buf_MPO_left = 0xFFFFFFFFUL;         // reset _u32In_buf_MPO_left to U32_MAX
    bIsMPOFormat = FALSE;
#endif

#if LOG_DATA_TO_USB
fclose(logBinfp);
logBinfp = NULL;
#endif

    JPEG_DEBUG_API_MSG("%s: done!!\n", __FUNCTION__);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get JPEG decoder error code
///@return JPEG decoder error code
/********************************************************************/
JPEG_ErrCode _MApi_JPEG_GetErrorCode(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_NOT_INIT;
    }
    JPEG_DEBUG_API_MSG("%s: ErrCode = %d\n", __FUNCTION__ , _Error_code);
    return _Error_code;
}
//------------------------------------------------------------------------------
#if 0 //move to APP
/********************************************************************/
///Preload buffer from JPEG content
///The calling order must be HIGH -> LOW -> HIGH -> LOW....
///or LOW -> HIGH -> LOW -> HIGH.
///Since the file offset is maintained out side, we assume it won't be out of order.
///@param u8Type \b IN indicate which parts of buffer wants to load
///@return true or false
/********************************************************************/
MS_BOOL MApi_JPEG_PreLoadBuffer(MS_U8 u8Type)
{
    MS_S32 bytes_read;
    MS_U8 *pBuffer_addr, Position;
    MS_U32 buf_left = 0;

    JPEG_DEBUG_API_MSG("MApi_JPEG_PreLoadBuffer: type = %d\n", u8Type);
#if 1
    if(MAPi_JPEG_ProcessEOF(u8Type))
        return TRUE;
#else
    if ( _bEOF_flag )
    {
        JPEG_DEBUG_API_MSG("end of file!!\n");
        MDrv_UART_PutChar('t'); // DO NOT MARK THIS, THIS FUNCTION MUST BE CALLED. OR THE JPD DECODE WILL FAIL for LARGE FILES
        if(u8Type==E_JPEG_BUFFER_HIGH)
        {
            // clear MRC high portion read complete event
            //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBH_DONE);
            MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBH_DONE);
        }
        else
        {
            // clear MRC low portion read complete event
            //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBL_DONE);
            MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBL_DONE);
        }
        return TRUE;
    }
#endif

    if ( setjmp( _jmp_state ) )
    {
        JPEG_DEBUG_API_MSG("ERROR\n");
        return ( FALSE );
    }

    if(u8Type==E_JPEG_BUFFER_HIGH)
    {
        pBuffer_addr = _pu8In_buf + (MRC_BUFFER_SIZE/2);
        Position = 1;
    }
    else
    {
        pBuffer_addr = _pu8In_buf;
        Position = 0;
    }

#if 1
{
    MS_BOOL EOF_flag = FALSE;
    do
    {
        bytes_read = JPEG_FILE_read( pBuffer_addr + buf_left, MRC_BUFFER_SIZE/2 - buf_left, &EOF_flag, _pStream, Position );

        if ( bytes_read == -1 )
        {
            JPEG_terminate( E_JPEG_STREAM_READ );
        }

        buf_left += bytes_read;
//        _Total_Decoded_Size += bytes_read;
    } while(( buf_left < MRC_BUFFER_SIZE/2 ) && ( !EOF_flag ));
    MAPi_JPEG_FileReadInfo(buf_left, EOF_flag);
}
#else
    do
    {
        bytes_read = JPEG_FILE_read( pBuffer_addr + buf_left, MRC_BUFFER_SIZE/2 - buf_left, &_bEOF_flag, _pStream, Position );

        if ( bytes_read == -1 )
        {
            JPEG_terminate( E_JPEG_STREAM_READ );
        }

        buf_left += bytes_read;
////        _Total_Decoded_Size += bytes_read;
    } while(( buf_left < MRC_BUFFER_SIZE/2 ) && ( !_bEOF_flag ));
#endif


#if 1
    MApi_JPEG_SetMRBufferValid(u8Type);
#else
    if(u8Type==E_JPEG_BUFFER_HIGH)
    {
        // clear MRC high portion read complete event
        //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBH_DONE);
        MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBH_DONE);
        // mark high portion valid
        //MDrv_Write2Byte( BK_JPD_MCONFIG, (MDrv_Read2Byte(BK_JPD_MCONFIG) & ~0x0003) | JPD_H_VLD);
        MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | JPD_H_VLD);
    }
    else
    {
        // clear MRC low portion read complete event
        //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBL_DONE);
        MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBL_DONE);
        // mark low portion valid
        //MDrv_Write2Byte( BK_JPD_MCONFIG, (MDrv_Read2Byte(BK_JPD_MCONFIG) & ~0x0003) | JPD_L_VLD);
        MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | JPD_L_VLD);
    }
#endif
    return TRUE;
}
#else // for APP reference
#if 0
MS_BOOL MApi_JPEG_PreLoadBuffer(MS_U8 u8MRBuffType, MS_U8* u8In_buf, MS_U32 MaxBufSize)
{
    MS_S32 bytes_read;
    MS_U8 *pBuffer_addr, Position;
    MS_U32 buf_left = 0;
    MS_BOOL EOF_flag = FALSE;

    if(MAPi_JPEG_ProcessEOF(u8MRBuffType))
        return TRUE;

    if(MaxBufSize > (MRC_BUFFER_SIZE/2))
    {
        return FALSE;
    }

    if(u8MRBuffType==E_JPEG_BUFFER_HIGH)
    {
        pBuffer_addr = u8In_buf + (MRC_BUFFER_SIZE/2);
        Position = 1;
    }
    else
    {
        pBuffer_addr = u8In_buf;
        Position = 0;
    }

    do
    {
//        bytes_read = JPEG_FILE_read( pBuffer_addr + buf_left, MRC_BUFFER_SIZE/2 - buf_left, &EOF_flag, _pStream, Position );
        bytes_read = JPEG_FILE_read( pBuffer_addr + buf_left, MaxBufSize - buf_left, &EOF_flag, _pStream, Position );

        if ( bytes_read == -1 )
        {
            MApi_JPEG_SetErrCode( E_JPEG_STREAM_READ );
            return FALSE;
        }

        buf_left += bytes_read;
//        _Total_Decoded_Size += bytes_read;
    } while(( buf_left < MaxBufSize ) && ( !EOF_flag ));
    MAPi_JPEG_FileReadInfo(buf_left, EOF_flag);
    MApi_JPEG_SetMRBufferValid(u8MRBuffType);
    return TRUE;
}
#endif
#endif
//------------------------------------------------------------------------------
/********************************************************************/
///Get JPD EVENT FLAG
///@return \b JPEG_Event : JPEG event flag
/********************************************************************/
JPEG_Event _MApi_JPEG_GetJPDEventFlag(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_EVENT_DEC_ERROR_MASK;
    }

#if SW_JPD_RGB_CMYK
    // It's pure software decode
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        return E_JPEG_EVENT_DEC_DONE;
    }
#endif

#ifndef CMODEL
    {
        JPEG_Event reg_val = (JPEG_Event)MDrv_JPD_GetEventFlag();
        if(reg_val!=0)
        {
        JPEG_DEBUG_API_MSG("%s: reg_val = 0x%04X\n", __FUNCTION__ , reg_val);
        }
        if(E_JPEG_EVENT_DEC_DONE & reg_val)
        {
#if PRINT_JPD_DECODE_TIME
            u32MeasureDecodeTimeHW = MsOS_GetSystemTime()-u32MeasureDecodeTime;
            printf_red("SW decode header:[%ld]ms, HW decode: [%ld]ms\n", u32MeasureDecodeTimeSW, u32MeasureDecodeTimeHW);
#endif
            if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API)
            {
                verJPD_CRC32_Init();
                verJPD_CRC32_Update((MS_U8 *)MWC_BUFFER_ADDR,
                    (MS_U32)MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2);
                printf_red("[%dx%d], u32CRCResult=0x%lx\n",MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), verJPD_CRC32_GetResult());
            }

#if LOG_DATA_TO_USB
            if(MApi_JPEG_GetWidth() <=JPEG_MAX_WIDTH
                && MApi_JPEG_GetHeight() <=JPEG_MAX_HEIGHT)
            {
                MS_U32 u32Size;
                u32Size = MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2;
                printf("[%dx%d], MWC_BUFFER_ADDR=0x%lx, size=0x%lx", MApi_JPEG_GetWidth(),
                    MApi_JPEG_GetHeight(), MS_VA2PA(MWC_BUFFER_ADDR),u32Size);
                _PrintOutputMem(MWC_BUFFER_ADDR, u32Size);
            }
#endif
        }
        return reg_val;
    }
#else
    return E_JPEG_EVENT_DEC_DONE;
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
///Set JPD EVENT FLAG
///@param -eEvtVal \b IN : JPEG_Event
///@return None
/********************************************************************/
void _MApi_JPEG_SetJPDEventFlag(JPEG_Event eEvtVal)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }

#if SW_JPD_RGB_CMYK
    // It's pure software decode
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        return;
    }
#endif

#ifndef CMODEL
    MDrv_JPD_SetEventFlag((MS_U16)eEvtVal);
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Reset JPD -- Reset must be called before trigger JPD\n
///This function always issue pause command and wait for both MRC & MWC\n
///becoming inactive, then reset JPD.\n
///\b NOTE: The pause function can be only used by Pluto/T1/T2... not include ERIS.
///@return None
/********************************************************************/
void _MApi_JPEG_Rst(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifndef CMODEL
    JPEG_DEBUG_API_MSG("MApi_JPEG_GetCurReadStatus : \nCurVidx = %d, CurRow = %d, CurCol = %d ",
        MDrv_JPD_GetCurVidx(),
        MDrv_JPD_GetCurRow(),
        MDrv_JPD_GetCurCol());
    JPEG_DEBUG_API_MSG("CurMRCAddr = 0x%lx\n", MDrv_JPD_GetCurMRCAddr());

    MDrv_JPD_Rst();
#endif
    JPEG_DEBUG_API_MSG("%s:done!!\n", __FUNCTION__);
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Power On JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_PowerOn(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifndef CMODEL
    MDrv_JPD_PowerOn();
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Power Off JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_PowerOff(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifndef CMODEL
    MDrv_JPD_PowerOff();
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get current vertical line index written to memory
///@return Current vertical line index written to memory
/********************************************************************/
MS_U16 _MApi_JPEG_GetCurVidx(void) //new
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s:\n", __FUNCTION__);
#ifndef CMODEL
    return MDrv_JPD_GetCurVidx();
#else
    return 0;
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
///Check current JPEG image is progressive or not
///@return TRUE / FALSE
/// - TRUE : progressive
/// - FALSE : baseline
/********************************************************************/
MS_BOOL _MApi_JPEG_IsProgressive(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return FALSE;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _bProgressive_flag);
    return (_bProgressive_flag);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Check current JPEG image has thumbnail or not
///@return TRUE / FALSE
/// - TRUE : thumbnail found
/// - FALSE : no thumbnail
/********************************************************************/
MS_BOOL _MApi_JPEG_ThumbnailFound(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return FALSE;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _bThumbnailFound);
    return (_bThumbnailFound);
}
//------------------------------------------------------------------------------
/********************************************************************/
///The width may be the thumbnail or original image size, it based on decoding mode
///@return width
/********************************************************************/
MS_U16 _MApi_JPEG_GetWidth(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16Image_x_size);
    return (_u16Image_x_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///The height may be the thumbnail or original image size, it based on decoding mode
///@return height
/********************************************************************/
MS_U16 _MApi_JPEG_GetHeight(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16Image_y_size);
    return (_u16Image_y_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original width of this JPEG file after alignment
///@return original width
/********************************************************************/
MS_U16 _MApi_JPEG_GetOriginalWidth(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16OriginalImage_x_size);
    return (_u16OriginalImage_x_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original height of this JPEG file after alignment
///@return original height
/********************************************************************/
MS_U16 _MApi_JPEG_GetOriginalHeight(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16OriginalImage_y_size);
    return (_u16OriginalImage_y_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original width before alignment
///@return non-alignment width
/********************************************************************/
MS_U16 _MApi_JPEG_GetNonAlignmentWidth(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16NonAlignmentImage_x_size);
    return (_u16NonAlignmentImage_x_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the original height before alignment
///@return non-alignment height
/********************************************************************/
MS_U16 _MApi_JPEG_GetNonAlignmentHeight(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16NonAlignmentImage_y_size);
    return (_u16NonAlignmentImage_y_size);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed pitch of JPEG image
///@return displayed pitch
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedPitch(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImagePitch);
    return (_u16AlignedImagePitch);
}

MS_U16 _MApi_JPEG_GetAlignedPitch_H(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImagePitch_H);
    return (_u16AlignedImagePitch_H);
}


//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed width of JPEG image
///@return displayed width
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedWidth(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImageWidth);
    return (_u16AlignedImageWidth);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get the displayed height of JPEG image
///@return displayed height
/********************************************************************/
MS_U16 _MApi_JPEG_GetAlignedHeight(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }
    JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u16AlignedImageHeight);
    return (_u16AlignedImageHeight);
}
//------------------------------------------------------------------------------
/********************************************************************/
///Get scale down factor\n
///Depending on the real picture width & height, it will automatically set scale down\n
///factor to meet maximum JPEG width & height allowed.
///@return scale down factor
/********************************************************************/
MS_U8 _MApi_JPEG_GetScaleDownFactor(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }

#if SW_JPD_RGB_CMYK
    if((E_JPEG_CMYK == gu8Scan_type)
    || (E_JPEG_RGB == gu8Scan_type))
    {
        JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u8ScaleDownFactor*2);
        return (_u8ScaleDownFactor*2);
    }
#endif
    {
        JPEG_DEBUG_API_MSG("%s: %d\n", __FUNCTION__ , _u8ScaleDownFactor);
        return (_u8ScaleDownFactor);
    }
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution
///@param -u16Width \b IN : indicate max decoding width
///@param -u16Height \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
    JPEG_DEBUG_API_MSG("%s: Width = %d, Height = %d\n", __FUNCTION__, u16Width, u16Height);
    JPEG_MAX_WIDTH = u16Width;
    JPEG_MAX_HEIGHT = u16Height;
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution of Progressive JPEG
///@param -u16ProWidth \b IN : indicate max decoding width
///@param -u16ProHeight \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
    JPEG_DEBUG_API_MSG("%s: ProWidth = %d, ProHeight = %d\n", __FUNCTION__, u16ProWidth, u16ProHeight);
    JPEG_PRO_MAX_WIDTH = u16ProWidth;
    JPEG_PRO_MAX_HEIGHT = u16ProHeight;
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Valid the JPEG read buffer flag
///@param -u8MRBuffType \b IN : the type of JPEG read buffer
///@return None
/********************************************************************/
void _MApi_JPEG_SetMRBufferValid(JPEG_BuffLoadType u8MRBuffType)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s: MRBuffType = %d\n", __FUNCTION__ , u8MRBuffType);
#ifndef CMODEL
    switch(u8MRBuffType)
    {
        case E_JPEG_BUFFER_HIGH:
            MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBH_DONE);
            MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | JPD_H_VLD);
            break;
        case E_JPEG_BUFFER_LOW:
            MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBL_DONE);
            MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | JPD_L_VLD);
            break;
        default:
            break;
    }
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG EOF flag and the total byte-read of JPEG file
///@param -u32DataRead \b IN : the amount of byte of file reading
///@param -bEOFflag \b IN : the EOF flag of file reading
///@return None
/********************************************************************/
void _MApi_JPEG_UpdateReadInfo(MS_U32 u32DataRead, MS_BOOL bEOFflag)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s: DataRead = %ld, EOFflag = %d\n", __FUNCTION__ , u32DataRead, bEOFflag);
////    _Total_Decoded_Size += data_read;
    _u32Total_bytes_read += u32DataRead;
    _bEOF_flag = bEOFflag;

#ifndef CMODEL
    // Set buffer valid to avoid last data not decoded done
    if(bEOFflag)
    {
        MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | (JPD_H_VLD |JPD_L_VLD));
    }
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Process JPEG read buffer flag when EOF.
///@param -u8MRBuffType \b IN : The type of JPEG read buffer
///@return None
/********************************************************************/
void _MApi_JPEG_ProcessEOF(JPEG_BuffLoadType u8MRBuffType)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return;
    }
    JPEG_DEBUG_API_MSG("%s: end of file!! MRBuffType = %d\n", __FUNCTION__ , u8MRBuffType);
////    MDrv_UART_PutChar('t'); // DO NOT MARK THIS, THIS FUNCTION MUST BE CALLED. OR THE JPD DECODE WILL FAIL for LARGE FILES
#ifndef CMODEL
    if(u8MRBuffType == E_JPEG_BUFFER_HIGH)
    {
        // clear MRC high portion read complete event
        //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBH_DONE);
        MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBH_DONE);
    }
    else
    {
        // clear MRC low portion read complete event
        //MDrv_JPD_ClearJPDStatus(E_JPD_EVENT_MRBL_DONE);
        MDrv_JPD_SetEventFlag(E_JPD_EVENT_MRBL_DONE);
    }

    // Set buffer valid to avoid last data not decoded done
    MDrv_JPD_Set_M_Config((MDrv_JPD_Get_M_Config() & ~0x0003) | (JPD_H_VLD |JPD_L_VLD));
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG Error Code if something error on upper layer.
///@param -ErrCode \b IN : The error type of JPEG decoder
///@return None
/********************************************************************/
void _MApi_JPEG_SetErrCode(JPEG_ErrCode ErrCode)
{
    JPEG_DEBUG_API_ERR("%s: ErrCode = %d\n", __FUNCTION__ , ErrCode);
    _Error_code = ErrCode;
////    JPEG_free_all_blocks();
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set JPEG debug level.
///@param -u8DbgLevel \b IN : The JPEG debug level, refer to JPEG_DbgLevel.
///@return None
/********************************************************************/
void _MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
    JPEG_DEBUG_API_MSG("%s: DbgLevel = 0x%04X\n", __FUNCTION__, u8DbgLevel);
    _u8JPEG_ApiDbgLevel = u8DbgLevel;
#ifndef CMODEL
    MDrv_JPD_SetDbgLevel(u8DbgLevel);
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG debug level.
///@return JPEG debug level, refer to JPEG_DbgLevel.
/********************************************************************/
MS_U8 _MApi_JPEG_GetDbgLevel(void)
{
    JPEG_DEBUG_API_MSG("%s: DbgLevel = 0x%04X\n", __FUNCTION__, _u8JPEG_ApiDbgLevel);
    return _u8JPEG_ApiDbgLevel;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set MHEG5 Flag.
///@param -bEnable \b IN :
///@return None
/********************************************************************/
void _MApi_JPEG_SetMHEG5(MS_BOOL bEnable)
{
    bMHEG5 = bEnable;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG information
///@param -pJPEG_Info \b OUT : JPEG information\n
/// structure {\n
///    MS_U8* \b pu8DrvVer : JPEG DRV version\n
///    MS_U8* \b pu8HalVer : JPEG HAL version\n
///    MS_U8* \b pu8FwVer : JPEG FW version\n
///    JPEG_DrvCap \b stDrvCap : JPEG driver capability\n
///           };
///@return None
/********************************************************************/
void _MApi_JPEG_GetInfo(JPEG_Info *pJPEG_Info)
{
#ifndef CMODEL
    JPD_Info DrvInfo;
    const MSIF_Version *DrvVer;
    MDrv_JPD_GetInfo(&DrvInfo);
    MDrv_JPD_GetLibVer(&DrvVer);
    pJPEG_Info->pu8DrvVer = DrvVer;
    pJPEG_Info->pu8HalVer = DrvInfo.pu8HalVer;
    pJPEG_Info->pu8FwVer = DrvInfo.pu8FwVer;
    pJPEG_Info->stDrvCap.bBaseline = DrvInfo.stCap.bBaseline;
    pJPEG_Info->stDrvCap.bProgressive = DrvInfo.stCap.bProgressive;
    pJPEG_Info->stDrvCap.bMJPEG = DrvInfo.stCap.bMJPEG;
    pJPEG_Info->stDrvCap.u16MaxWidth = JPEG_MAX_WIDTH;
    pJPEG_Info->stDrvCap.u16MaxHeight = JPEG_MAX_HEIGHT;
    pJPEG_Info->stDrvCap.u16MaxProWidth = JPEG_PRO_MAX_WIDTH;
    pJPEG_Info->stDrvCap.u16MaxProHeight = JPEG_PRO_MAX_HEIGHT;

    JPEG_DEBUG_API_MSG("JPD %04x, %04x, %04x, %s\n",
        DrvVer->DDI.customer, DrvVer->DDI.model, DrvVer->DDI.chip, &(DrvVer->DDI.cpu));
    JPEG_DEBUG_API_MSG("pu8HalVer = %s, pu8FwVer = %s\n", pJPEG_Info->pu8HalVer, pJPEG_Info->pu8FwVer);
    JPEG_DEBUG_API_MSG("%s: DrvCap_BL = %d, DrvCap_PRO = %d, DrvCap_MJPEG = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.bBaseline, pJPEG_Info->stDrvCap.bProgressive, pJPEG_Info->stDrvCap.bMJPEG);
    JPEG_DEBUG_API_MSG("%s: MaxWidth = %d, MaxHeight = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.u16MaxWidth, pJPEG_Info->stDrvCap.u16MaxHeight);
    JPEG_DEBUG_API_MSG("%s: MaxProWidth = %d, MaxProHeight = %d\n", __FUNCTION__,
        pJPEG_Info->stDrvCap.u16MaxProWidth, pJPEG_Info->stDrvCap.u16MaxProHeight);
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG status.
///@param -pJPEG_Status \b OUT : JPEG status\n
/// structure {\n
///    MS_U32 \b u32CurMRCAddr : JPD current decode address\n
///    MS_U16 \b u16CurVidx : JPD current decode vertical index\n
///    MS_U16 \b u16CurRow : JPD current decode row\n
///    MS_U16 \b u16CurCol : JPD current decode column\n
///    MS_BOOL \b bDrvBusy : JPEG DRV busy status\n
///    MS_BOOL \b bIsrEnable : JPD ISR status\n
///           };
///@return None
/********************************************************************/
void _MApi_JPEG_GetStatus(JPEG_Status *pJPEG_Status)
{
#ifndef CMODEL
    JPD_Status *pDrvStatus = MDrv_JPD_GetStatus();
    pJPEG_Status->u32CurMRCAddr = pDrvStatus->u32CurMRCAddr;
    pJPEG_Status->u16CurVidx = pDrvStatus->u16CurVidx;
    pJPEG_Status->u16CurRow = pDrvStatus->u16CurRow;
    pJPEG_Status->u16CurCol = pDrvStatus->u16CurCol;
    pJPEG_Status->bDrvBusy = pDrvStatus->bIsBusy;
    pJPEG_Status->bIsrEnable = pDrvStatus->bIsrEnable;
    JPEG_DEBUG_API_MSG("%s: Drv_busy = %d, Isr_status = %d\n", __FUNCTION__ , pJPEG_Status->bDrvBusy, pJPEG_Status->bIsrEnable);
    JPEG_DEBUG_API_MSG("%s: CurVidx = %d, CurRow = %d, CurCol = %d\n", __FUNCTION__ ,
                        pJPEG_Status->u16CurVidx, pJPEG_Status->u16CurRow, pJPEG_Status->u16CurCol);
    JPEG_DEBUG_API_MSG("%s: CurMRCAddr = 0x%lx\n", __FUNCTION__ , pJPEG_Status->u32CurMRCAddr);
#endif
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get JPEG Version.
///@param -ppVersion \b OUT : JPEG Version
///@return \b JPEG_Result
///     - E_JPEG_OKAY : Success
///     - E_JPEG_FAILED : Failed
/********************************************************************/
JPEG_Result _MApi_JPEG_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_JPEG_FAILED;
    }
    *ppVersion = &_api_jpeg_version;
    JPEG_DEBUG_API_MSG("%s: JPEG %04x, %04x, %04x, %s\n", __FUNCTION__ ,
        _api_jpeg_version.DDI.customer, _api_jpeg_version.DDI.model, _api_jpeg_version.DDI.chip, &(_api_jpeg_version.DDI.cpu));
    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// For H/W bug, some cases can not exit after decode done, Check
/// Vidx to exit.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : Vidx checking is successful.
/// - E_JPEG_FAILED : Vidx checking is failed.
/********************************************************************/
JPEG_Result _MApi_JPEG_HdlVidxChk(void)
{
#ifndef CMODEL
    MS_U16 cur_vidx = 0;
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        _JPD_IsDecoding = FALSE;
        return E_JPEG_FAILED;
    }

    if(FALSE == _JPD_IsDecoding)
    {
        _JPD_IsDecoding = TRUE;
#if MSOS_GET_SYSTEM_TIME
        _JPD_ReCheckTime = MsOS_GetSystemTime();
#else
        _JPD_ReCheckTime = 0;
#endif
    }

    //For H/W bug, some cases can not exit after decode done. Check Vidx to exit.
    cur_vidx = MDrv_JPD_GetCurVidx();
    if(_JPD_PreVIdx != cur_vidx)
    {
        _JPD_PreVIdx = cur_vidx;
#if MSOS_GET_SYSTEM_TIME
        _JPD_ReCheckTime = MsOS_GetSystemTime();
#else
        _JPD_ReCheckTime = 0;
#endif
    }
    else
    {
#if MSOS_GET_SYSTEM_TIME
        if((MsOS_GetSystemTime() - _JPD_ReCheckTime) >= 1000)
#else
        if(_JPD_ReCheckTime++ >= 10000)
#endif
        {
            _JPD_IsDecoding = FALSE;
            JPEG_DEBUG_API_ERR("Decode timeout...!!!!\n");
            return E_JPEG_FAILED;
        }
    }
#endif
    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get Preload buffer information.
///@param -pBuffLoadType \b OUT : JPEG preload buffer type
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/// - E_JPEG_RETRY : we need to re-get preload buffer type
/********************************************************************/
JPEG_Result _MApi_JPEG_GetBuffLoadType(JPEG_BuffLoadType *pBuffLoadType)
{
#ifndef CMODEL
    MS_U16 reg_val = MDrv_JPD_GetEventFlag();

    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    if((E_JPD_EVENT_MRBL_DONE & reg_val)
    && (E_JPEG_BUFFER_LOW != u8PreLHFlag))
    {
        JPEG_DEBUG_API_MSG("%s : MRBuffer Load LOW!!!!\n", __FUNCTION__);
        u8PreLHFlag = E_JPEG_BUFFER_LOW;
        u32MRCheckCount = 0;
        *pBuffLoadType = u8PreLHFlag;
        return E_JPEG_OKAY;
    }
    else if((E_JPD_EVENT_MRBH_DONE & reg_val)
    && (E_JPEG_BUFFER_HIGH != u8PreLHFlag))
    {
        JPEG_DEBUG_API_MSG("%s : MRBuffer Load HIGH!!!!\n", __FUNCTION__);
        u8PreLHFlag = E_JPEG_BUFFER_HIGH;
        u32MRCheckCount = 0;
        *pBuffLoadType = u8PreLHFlag;
        return E_JPEG_OKAY;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s : MRBuffer Load NONE!!!!\n", __FUNCTION__);
        *pBuffLoadType = E_JPEG_BUFFER_NONE;
        u32MRCheckCount++;
        if(u32MRCheckCount >= 1000)
        {
            JPEG_DEBUG_API_ERR("Decoding time out!!\n");
            u32MRCheckCount = 0;
            return E_JPEG_FAILED;
        }
    }
    return E_JPEG_RETRY;
#else
    return E_JPEG_OKAY;
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Enable JPD ISR and register callback function.
///@param -IsrCb \b IN : JPD ISR callback function
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_EnableISR(JPEG_IsrFuncCb IsrCb)
{
#ifndef CMODEL
    if(TRUE == MDrv_JPD_EnableISR((JPD_IsrFuncCb)IsrCb))
    {
        JPEG_DEBUG_API_MSG("%s : Success!!!!\n", __FUNCTION__);
        return E_JPEG_OKAY;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s : Fail!!!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
#else
    return E_JPEG_OKAY;
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Disable JPD ISR and unregister callback function.
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_DisableISR(void)
{
#ifndef CMODEL
    if(TRUE == MDrv_JPD_DisableISR())
    {
        JPEG_DEBUG_API_MSG("%s : Success!!!!\n", __FUNCTION__);
        return E_JPEG_OKAY;
    }
    else
    {
        JPEG_DEBUG_API_MSG("%s : Fail!!!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
#else
    return E_JPEG_OKAY;
#endif
}
//------------------------------------------------------------------------------
/********************************************************************/
/// JPEG wait done function, call it to check JPEG decoding status.
///@return \b JPEG_DecodeStatus
/// - E_JPEG_DEC_DONE : JPEG decode done.
/// - E_JPEG_DEC_FAILED : JPEG decode failed.
/// - E_JPEG_DEC_MRBL_DONE : MRB low done, need to feed data.
/// - E_JPEG_DEC_MRBH_DONE : MRB high done, need to feed data.
/// - E_JPEG_DEC_DECODING : JPEG decoding
/********************************************************************/
JPEG_DecodeStatus _MApi_JPEG_WaitDone(void)
{
#ifndef CMODEL
    MS_U16 reg_val = E_JPD_EVENT_DEC_DONE;
    MS_U16 cur_vidx = 0;
#endif

    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_ERR("%s: JPD have not init!!\n", __FUNCTION__);
        _JPD_IsDecoding = FALSE;
        return E_JPEG_DEC_FAILED;
    }

#ifndef CMODEL
    if(FALSE == _JPD_IsDecoding)
    {
        _JPD_IsDecoding = TRUE;
#if MSOS_GET_SYSTEM_TIME
        _JPD_ReCheckTime = MsOS_GetSystemTime();
#else
        _JPD_ReCheckTime = 0;
#endif
    }

    //For H/W bug, some cases can not exit after decode done. Check Vidx to exit.
    cur_vidx = MDrv_JPD_GetCurVidx();
    if(_JPD_PreVIdx != cur_vidx)
    {
        _JPD_PreVIdx = cur_vidx;
#if MSOS_GET_SYSTEM_TIME
        _JPD_ReCheckTime = MsOS_GetSystemTime();
#else
        _JPD_ReCheckTime = 0;
#endif
    }
    else
    {
#if MSOS_GET_SYSTEM_TIME
        if((MsOS_GetSystemTime() - _JPD_ReCheckTime) >= 1000)
#else
        if(_JPD_ReCheckTime++ >= 10000)
#endif
        {
            JPEG_DEBUG_API_ERR("%s:Decode timeout.....!!!!\n", __FUNCTION__);
            _JPD_IsDecoding = FALSE;
            return E_JPEG_DEC_FAILED;
        }
        else
        {
            return E_JPEG_DEC_DECODING;
        }
    }

    reg_val = MDrv_JPD_GetEventFlag();

    if(E_JPD_EVENT_DEC_DONE & reg_val)
    {
        JPEG_DEBUG_API_MSG("%s:Decode Done!!\n", __FUNCTION__);
#if PRINT_JPD_DECODE_TIME
        u32MeasureDecodeTimeHW = MsOS_GetSystemTime()-u32MeasureDecodeTime;
        printf_red("SW decode header:[%ld]ms, HW decode: [%ld]ms\n", u32MeasureDecodeTimeSW, u32MeasureDecodeTimeHW);
#endif
        if(_u8JPEG_ApiDbgLevel & E_JPEG_DEBUG_API)
        {
            verJPD_CRC32_Init();
            verJPD_CRC32_Update((MS_U8 *)MWC_BUFFER_ADDR,
                (MS_U32)MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2);
            printf_red("[%dx%d], u32CRCResult=0x%lx\n",MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), verJPD_CRC32_GetResult());
        }

#if LOG_DATA_TO_USB
            if(MApi_JPEG_GetWidth() <=JPEG_MAX_WIDTH
                && MApi_JPEG_GetHeight() <=JPEG_MAX_HEIGHT)
            {
                MS_U32 u32Size;
                u32Size = MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2;
                printf("[%dx%d], MWC_BUFFER_ADDR=0x%lx, size=0x%lx", MApi_JPEG_GetWidth(),
                    MApi_JPEG_GetHeight(), MS_VA2PA(MWC_BUFFER_ADDR),u32Size);
                _PrintOutputMem(MWC_BUFFER_ADDR, u32Size);
            }
#endif
        _JPD_IsDecoding = FALSE;
        return E_JPEG_DEC_DONE;
    }
    else if(E_JPD_EVENT_RST_ERROR & reg_val)
    {
        JPEG_DEBUG_API_ERR("%s:RST Error!!\n", __FUNCTION__);
        _JPD_IsDecoding = FALSE;
        return E_JPEG_DEC_RST_ERROR;
    }
    else if((E_JPD_EVENT_ECS_ERROR & reg_val)
        || (E_JPD_EVENT_IS_ERROR & reg_val))
    {
        JPEG_DEBUG_API_ERR("%s:Bitstream Error, reg_val = 0x%02x!!\n", __FUNCTION__ , reg_val);
        _JPD_IsDecoding = FALSE;
        return E_JPEG_DEC_BITSTREAM_ERROR;
    }
#if (JPD_SUPPORT_AUTO_PROTECT==true)
    else if(E_JPD_EVENT_MWB_FULL & reg_val)
    {
        JPEG_DEBUG_API_MSG("[%s]MWB is full!!\n", __FUNCTION__);
        _JPD_IsDecoding = FALSE;
        return E_JPEG_DEC_FAILED;
    }
#endif
    else if((E_JPD_EVENT_MRBL_DONE & reg_val)
    && (E_JPEG_BUFFER_LOW != u8PreLHFlag))
    {
        u8PreLHFlag = E_JPEG_BUFFER_LOW;
        JPEG_DEBUG_API_MSG("[%s]MRBL done!!\n", __FUNCTION__);
        return E_JPEG_DEC_MRBL_DONE;
    }
    else if((E_JPD_EVENT_MRBH_DONE & reg_val)
    && (E_JPEG_BUFFER_HIGH != u8PreLHFlag))
    {
        u8PreLHFlag = E_JPEG_BUFFER_HIGH;
        JPEG_DEBUG_API_MSG("[%s]MRBH done!!\n", __FUNCTION__);
        return E_JPEG_DEC_MRBH_DONE;
    }
#endif
    return E_JPEG_DEC_DECODING;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get datetime info of JPEG EXIF
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_GetEXIFDateTime(JPEG_EXIF_DateTime *DateTime)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    DateTime->bHasDataTime = _stEXIF_DateTime.bHasDataTime;
    DateTime->u32Year = _stEXIF_DateTime.u32Year;
    DateTime->u32Month = _stEXIF_DateTime.u32Month;
    DateTime->u32Day = _stEXIF_DateTime.u32Day;
    DateTime->u32Hour = _stEXIF_DateTime.u32Hour;
    DateTime->u32Minute = _stEXIF_DateTime.u32Minute;
    DateTime->u32Second = _stEXIF_DateTime.u32Second;

    JPEG_DEBUG_API_MSG("=======JPEG EXIF DateTime======= Exist = %d \n", _stEXIF_DateTime.bHasDataTime);
    JPEG_DEBUG_API_MSG("Year = %ld, Month = %ld, Day = %ld\n", _stEXIF_DateTime.u32Year, _stEXIF_DateTime.u32Month, _stEXIF_DateTime.u32Day);
    JPEG_DEBUG_API_MSG("Hour = %ld, Minute = %ld, Second = %ld\n", _stEXIF_DateTime.u32Hour, _stEXIF_DateTime.u32Minute, _stEXIF_DateTime.u32Second);

    return E_JPEG_OKAY;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Get orientation info of JPEG EXIF
///@return \b JPEG_Result
/// - E_JPEG_OKAY : success.
/// - E_JPEG_FAILED : fail.
/********************************************************************/
JPEG_Result _MApi_JPEG_GetEXIFOrientation(JPEG_EXIF_Orientation *eOrientation)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *eOrientation = _eEXIF_Orientation;
    JPEG_DEBUG_API_MSG("JPEG EXIF Orientation = %d\n", _eEXIF_Orientation);
    return E_JPEG_OKAY;
}

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_Result _MApi_JPEG_GetEXIFManufacturer(MS_U8 *pu8Manufacturer, MS_U8 u8size)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    MS_U8 u8i=0;
    JPEG_memset((void*)(pu8Manufacturer), 0, u8size);
    JPEG_DEBUG_API_MSG("JPEG EXIF Manufacturer =");
    while(_u8EXIF_Manufacturer[u8i] && u8i<JPEG_MANUFACTURER_SIZE)
    {
        *(pu8Manufacturer+u8i) =_u8EXIF_Manufacturer[u8i];
        JPEG_DEBUG_API_MSG("%c", *(pu8Manufacturer+u8i));
        u8i++;
        if(u8i>=u8size)
        {
            JPEG_DEBUG_API_ERR("%s: NOT enough space!!\n", __FUNCTION__);
            return E_JPEG_FAILED;
        }
    }
    JPEG_DEBUG_API_MSG("\n");
//    JPEG_DEBUG_API_MSG("JPEG EXIF Manufacturer = %02x %02x %02x %02x %02x %02x %02x %02x\n",
//        *pu8Manufacturer, *(pu8Manufacturer+1), *(pu8Manufacturer+2), *(pu8Manufacturer+3),
//        *(pu8Manufacturer+4),*(pu8Manufacturer+5),*(pu8Manufacturer+6),*(pu8Manufacturer+7));
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFModel(MS_U8 *pu8Model, MS_U8 u8size)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    MS_U8 u8i=0;
    JPEG_memset((void*)(pu8Model), 0, u8size);
    JPEG_DEBUG_API_MSG("JPEG EXIF Model =");
    while(_u8EXIF_Model[u8i] && u8i<JPEG_MODEL_SIZE)
    {
        *(pu8Model+u8i) =_u8EXIF_Model[u8i];
        JPEG_DEBUG_API_MSG("%c", *(pu8Model+u8i));
        u8i++;
        if(u8i>=u8size)
        {
            JPEG_DEBUG_API_ERR("%s: NOT enough space!!\n", __FUNCTION__);
            return E_JPEG_FAILED;
        }
    }
    JPEG_DEBUG_API_MSG("\n");

//    JPEG_DEBUG_API_MSG("JPEG EXIF Model = %02x %02x %02x %02x %02x %02x %02x %02x\n",
//        *pu8Model, *(pu8Model+1), *(pu8Model+2), *(pu8Model+3),
//        *(pu8Model+4),*(pu8Model+5),*(pu8Model+6),*(pu8Model+7));
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFFlash(MS_U16 *pu16Flash)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu16Flash = _u16EXIF_Flash;
    JPEG_DEBUG_API_MSG("JPEG EXIF Flash = %d \n", *pu16Flash);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFISOSpeedRatings(MS_U32 *pu32ISOSpeedRatings)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ISOSpeedRatings = _u32EXIF_ISOSpeedRatings;
    JPEG_DEBUG_API_MSG("JPEG EXIF ISO = %ld \n", *pu32ISOSpeedRatings);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFShutterSpeedValue(JPEG_RATIONAL *pShutterSpeedValue)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pShutterSpeedValue->numerator = _stEXIF_ShutterSpeedValue.numerator;
    pShutterSpeedValue->denominator= _stEXIF_ShutterSpeedValue.denominator;
    JPEG_DEBUG_API_MSG("ShutterSpeedValue numerator=%ld, denominator=%ld\n", pShutterSpeedValue->numerator, pShutterSpeedValue->denominator);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFApertureValue(JPEG_RATIONAL *pApertureValue)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pApertureValue->s_numerator = _stEXIF_ApertureValue.s_numerator;
    pApertureValue->s_denominator= _stEXIF_ApertureValue.s_denominator;
    JPEG_DEBUG_API_MSG("ApertureValue numerator=%ld, denominator=%ld\n", pApertureValue->s_numerator, pApertureValue->s_denominator);
    return E_JPEG_OKAY;
}


JPEG_Result _MApi_JPEG_GetEXIFExposureBiasValue(JPEG_RATIONAL *pExposureBiasValue)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pExposureBiasValue->numerator = _stEXIF_ExposureBiasValue.numerator;
    pExposureBiasValue->denominator= _stEXIF_ExposureBiasValue.denominator;
    JPEG_DEBUG_API_MSG("ExposureBiasValue numerator=%ld, denominator=%ld\n", pExposureBiasValue->numerator, pExposureBiasValue->denominator);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFFocalLength(JPEG_RATIONAL *pFocalLength)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pFocalLength->s_numerator = _stEXIF_FocalLength.s_numerator;
    pFocalLength->s_denominator= _stEXIF_FocalLength.s_denominator;
    JPEG_DEBUG_API_MSG("FocalLength numerator=%ld, denominator=%ld\n", pFocalLength->s_numerator, pFocalLength->s_denominator);
    return E_JPEG_OKAY;
}
JPEG_Result _MApi_JPEG_GetEXIFImageWidth(MS_U32 *pu32ImageWidth)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ImageWidth = _u32EXIF_ImageWidth;
    JPEG_DEBUG_API_MSG("JPEG EXIF Image Width = %ld \n", *pu32ImageWidth);
    return E_JPEG_OKAY;
}

JPEG_Result _MApi_JPEG_GetEXIFImageHeight(MS_U32 *pu32ImageHeight)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    *pu32ImageHeight = _u32EXIF_ImageHeight;
    JPEG_DEBUG_API_MSG("JPEG EXIF Image Height = %ld \n", *pu32ImageHeight);
    return E_JPEG_OKAY;
}
JPEG_Result _MApi_JPEG_GetEXIFExposureTime(JPEG_RATIONAL *pExposureTime)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pExposureTime->s_numerator = _stEXIF_ExposureTime.s_numerator;
    pExposureTime->s_denominator= _stEXIF_ExposureTime.s_denominator;
    JPEG_DEBUG_API_MSG("ExposureTime numerator=%ld, denominator=%ld\n", pExposureTime->s_numerator, pExposureTime->s_denominator);
    return E_JPEG_OKAY;
}


JPEG_Result _MApi_JPEG_GetEXIFFNumber(JPEG_RATIONAL *pFNumber)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }
    pFNumber->s_numerator = _stEXIF_FNumber.s_numerator;
    pFNumber->s_denominator= _stEXIF_FNumber.s_denominator;
    JPEG_DEBUG_API_MSG("FNumber numerator=%ld, denominator=%ld\n", pFNumber->s_numerator, pFNumber->s_denominator);
    return E_JPEG_OKAY;
}
#endif


JPEG_Result _MApi_JPEG_GetControl(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32size)
{
    JPEG_Result eResult = E_JPEG_OKAY;

    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return E_JPEG_FAILED;
    }

    switch(eGetID)
    {
        case E_JPEG_GET_EXIF_MANUFACTURER:
            eResult = MApi_JPEG_GetEXIFManufacturer((MS_U8 *)param, u32size*4);
            JPEG_DEBUG_API_MSG("@JPEG EXIF Manufacturer = %c%c%c%c%c%c%c%c%c%c%c%c\n",
                *((MS_U8 *)param), *((MS_U8 *)param+1), *((MS_U8 *)param+2), *((MS_U8 *)param+3),
                *((MS_U8 *)param+4),*((MS_U8 *)param+5),*((MS_U8 *)param+6),*((MS_U8 *)param+7),
                *((MS_U8 *)param+8),*((MS_U8 *)param+9),*((MS_U8 *)param+10),*((MS_U8 *)param+11)
                );
            break;
        case E_JPEG_GET_EXIF_MODEL:
            eResult = MApi_JPEG_GetEXIFModel((MS_U8 *)param, u32size*4);
            JPEG_DEBUG_API_MSG("@JPEG EXIF Model = %c%c%c%c%c%c%c%c%c%c%c%c\n",
                *((MS_U8 *)param), *((MS_U8 *)param+1), *((MS_U8 *)param+2), *((MS_U8 *)param+3),
                *((MS_U8 *)param+4),*((MS_U8 *)param+5),*((MS_U8 *)param+6),*((MS_U8 *)param+7),
                *((MS_U8 *)param+8),*((MS_U8 *)param+9),*((MS_U8 *)param+10),*((MS_U8 *)param+11)
                );
            break;
        case E_JPEG_GET_EXIF_FLASH:
            *param = (MS_U32)_u16EXIF_Flash;
            JPEG_DEBUG_API_MSG("@JPEG EXIF Flash = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_ISO_SPEED_RATINGS:
            *param = _u32EXIF_ISOSpeedRatings;
            JPEG_DEBUG_API_MSG("@JPEG EXIF ISO = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = _stEXIF_ShutterSpeedValue.numerator;
            *(param+1) = _stEXIF_ShutterSpeedValue.denominator;
            JPEG_DEBUG_API_MSG("@ShutterSpeedValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_APERTURE_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_ApertureValue.s_numerator;
            *(param+1) = (MS_U32)_stEXIF_ApertureValue.s_denominator;
            JPEG_DEBUG_API_MSG("@ApertureValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = _stEXIF_ExposureBiasValue.numerator;
            *(param+1) = _stEXIF_ExposureBiasValue.denominator;
            JPEG_DEBUG_API_MSG("@ExposureBiasValue numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_FOCAL_LENGTH:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_FocalLength.s_numerator;
            *(param+1) = (MS_U32)_stEXIF_FocalLength.s_denominator;
            JPEG_DEBUG_API_MSG("@FocalLength numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_IMAGE_WIDTH:
            *param = _u32EXIF_ImageWidth;
            JPEG_DEBUG_API_MSG("@JPEG EXIF Image Width = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_IMAGE_HEIGHT:
            *param = _u32EXIF_ImageHeight;
            JPEG_DEBUG_API_MSG("@JPEG EXIF Image Height = %ld \n", *param);
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_TIME:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_ExposureTime.s_numerator;
            *(param+1) = (MS_U32)_stEXIF_ExposureTime.s_denominator;
            JPEG_DEBUG_API_MSG("@ExposureTime numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_F_NUMBER:
            if(u32size<2)
            {
                eResult = E_JPEG_FAILED;
            }
            *param = (MS_U32)_stEXIF_FNumber.s_numerator;
            *(param+1) = (MS_U32)_stEXIF_FNumber.s_denominator;
            JPEG_DEBUG_API_MSG("@FNumber numerator=%ld, denominator=%ld\n", *param, *(param+1));
            break;
        case E_JPEG_GET_EXIF_EXPOSURE_PROGRAM:
            *param = (MS_U32)_u16EXIF_Exposureprogram;
            JPEG_DEBUG_API_MSG("@JPEG EXIF Exposure Program = %ld \n", *param);
            break;
        default:
            eResult = E_JPEG_FAILED;
            break;
    }
    return eResult;
}

////////////////////////////////////////////////////////////////////////////////////////
///   current unused function, maybe removed in new MDDI
////////////////////////////////////////////////////////////////////////////////////////
#if 0 // not implement for new MDDI
//------------------------------------------------------------------------------
void msAPI_MJPEG_Parse_SOF(void)
{
    JPEG_locate_sof_marker();
}
//------------------------------------------------------------------------------
void msAPI_MJPEG_constructor( PJPEG_FILE_FileSystem_t Pstream, MS_U8 decode_type)
{
    _u8DecodeType = decode_type;
    JPEG_init_thumbnail();
    msAPI_MJPEG_decode_init( Pstream);
}
//------------------------------------------------------------------------------
//Set Total pics in the file
void msAPI_MJPEG_SetTotal_Pics(MS_U32 *DecodeTime)
{
    *DecodeTime = NumPics;
}
//------------------------------------------------------------------------------
MS_S16 msAPI_JPEG_Check_End_Flag( void )
{
    return ( _bEOF_flag );
}

#ifndef CMODEL
//------------------------------------------------------------------------------
/******************************************************************************/
///Set downscale ratio
///@param ratio \b OUT scale ratio
/******************************************************************************/
void msAPI_JPEG_SetDownScaleRatio(JPEG_DownScaleRatio ratio)
{
    switch(ratio)
    {
    case E_RATIO_HALF:
        _u8DownScaleRatio = E_JPD_DOWNSCALE_HALF;
        break;
    case E_RATIO_FOURTH:
        _u8DownScaleRatio = E_JPD_DOWNSCALE_FOURTH;
        break;
    case E_RATIO_EIGHTH:
        _u8DownScaleRatio = E_JPD_DOWNSCALE_EIGHTH;
        break;
    default:
        _u8DownScaleRatio = E_JPD_DOWNSCALE_ORG;
        break;
    }
}
//------------------------------------------------------------------------------
void msAPI_JPEG_GetOutputDimension(MS_U16 *width, MS_U16 *height)
{
    MS_U8 mcu_width, mcu_height;
    MS_U16 pic_width = _u16Image_x_size;
    MS_U16 pic_height = _u16Image_y_size;
    MS_U8 Y_VSF = _u8Comp_v_samp[0];
    MS_U8 Y_HSF = _u8Comp_h_samp[0];
    MS_U16 ROI_width;

    if ( ( mcu_width = pic_width % ( Y_HSF * 8 ) ) )
    {
        pic_width += ( Y_HSF * 8 - mcu_width );
    }

    if ( ( mcu_height = pic_height% ( Y_VSF * 8 ) ) )
    {
        pic_height += ( Y_VSF * 8 - mcu_height );
    }

    ROI_width = pic_width;

    if(_u8DownScaleRatio==E_JPD_DOWNSCALE_FOURTH||_u8DownScaleRatio==E_JPD_DOWNSCALE_EIGHTH)
    {
        MS_U16 ROI_width;

        if(_u8DownScaleRatio==E_JPD_DOWNSCALE_FOURTH)
        {
            // width must be multiple of 32
            ROI_width = (pic_width/32)*32;
        }
        else if(_u8DownScaleRatio==E_JPD_DOWNSCALE_EIGHTH)
        {
            // width must be multiple of 64
            ROI_width = (pic_width/64)*64;
        }
    }

    switch(_u8DownScaleRatio)
    {
        case E_JPD_DOWNSCALE_HALF:
            *width = ROI_width/2;
            break;

        case E_JPD_DOWNSCALE_FOURTH:
            *width = ROI_width/4;
            break;

        case E_JPD_DOWNSCALE_EIGHTH:
            *width = ROI_width/8;
            break;

        default:
            *width = ROI_width;
            break;
    }

    *height = pic_height;
}
#endif
//------------------------------------------------------------------------------
MS_BOOL msAPI_MJPEG_SetInitParameter(MS_U32 u32RBufAddr, MS_U32 u32RBufSize, MS_U32 u32WBufAddr, MS_U32 u32IBufAddr, MS_U32 u32IBufSize)
{
    // the buffer size must be multiple of 4 bytes
    //if((u32RBufSize%4)!=0)
     //   return FALSE;
    MS_U32 WBuffer = _u16Image_x_size*_u16Image_y_size*2;
#ifdef CMODEL
    _bReady_flag = FALSE;
#endif
    MRC_BUFFER_ADDR = _u32ReadBufferAddr + u32RBufAddr;
    JPEG_DEBUG_API_MSG("MRC_BUFFER_ADDR = %08lx\n",MRC_BUFFER_ADDR);
    //MRC_BUFFER_SIZE = _pu8In_buf + u32RBufSize;
   // JPEG_DEBUG_API_MSG("_u32WriteBufferAddr = %08X\n",_u32WriteBufferAddr);
   // JPEG_DEBUG_API_MSG("WBuffer = %08X\n",WBuffer);
    MWC_BUFFER_ADDR = _u32WriteBufferAddr + WBuffer;
    JPEG_DEBUG_API_MSG("MWC_BUFFER_ADDR = %08lx\n",MWC_BUFFER_ADDR);
    //JPEG_DEBUG_API_MSG("MWC_BUFFER_ADDR = %08X\n",MWC_BUFFER_ADDR);
    //INTERNAL_BUFFER_ADDR = u32IBufAddr;
    //INTERNAL_BUFFER_SIZE = u32IBufSize;
    //_u32In_buf_left = 0;
    _pu8In_buf = (MS_U8 *) (_u32ReadBufferAddr);
    _u32Total_bytes_read -= u32RBufAddr;
    //JPEG_MEMORY_init_mempool(((MS_U8 *) (u32IBufAddr + JPEG_DEFAULT_EXIF_SIZE)), u32IBufSize - JPEG_DEFAULT_EXIF_SIZE);
    //JPEG_locate_sof_marker();

    return TRUE;
}
//------------------------------------------------------------------------------
MS_BOOL msAPI_MJPEG_ReSetMWC(MS_U32 u32RBufAddr, MS_U32 u32RBufSize, MS_U32 u32WBufAddr, MS_U32 u32IBufAddr, MS_U32 u32IBufSize)
{

    _u32WriteBufferAddr = u32WBufAddr;

    JPEG_DEBUG_API_MSG("MWC_BUFFER_ADDR = %08lx\n",MWC_BUFFER_ADDR);

    return TRUE;
}
//------------------------------------------------------------------------------
void msAPI_JPEG_SetErrorCode(MS_U16 u16Code)
{
    _Error_code = u16Code;
}
#if 0
#ifdef CMODEL
//------------------------------------------------------------------------------
// If you wish to decompress the image, call this method after constructing
// the object. If E_JPEG_OKAY is returned you may then call decode() to
// fetch the scan lines.
JPEG_Result msAPI_JPEG_begin( void )
{
    if ( _bReady_flag )
    {
        JPEG_DEBUG_API_MSG("Flag!!!!\n");
        return ( E_JPEG_OKAY );
    }

    if ( _Error_code )
    {
        JPEG_DEBUG_API_MSG("Error!!!!\n");
        return ( E_JPEG_FAILED );
    }
    JPEG_locate_sof_marker();

    //JPEG_decode_start();

    _bReady_flag = TRUE;

    return ( E_JPEG_OKAY );
}
#endif
#endif
#endif // not implement for new MDDI


#if SW_JPD_RGB_CMYK
// Decodes and dequantizes the next row of coefficients.
JPEG_STATIC MS_BOOL msAPI_JPEG_decode_next_row( void )
{
    MS_U16 row_block = 0;
    MS_U16 mcu_row, mcu_block, k;

    // Clearing the entire row block buffer can take a lot of time!
    // Instead of clearing the entire buffer each row, keep track
    // of the number of nonzero entries written to each block and do
    // selective clears.
    //memset(_ps16Block_seg[0], 0, _u16Mcus_per_row * _u8Blocks_per_mcu * 64 * sizeof(BLOCK_TYPE));

    for ( mcu_row = 0; mcu_row < _u16Mcus_per_row; mcu_row++ )
    {
        if ( ( _u16Restart_interval ) && ( _u16Restarts_left == 0 ) )
        {
            if(!JPEG_process_restart())
                return FALSE;
        }

        for ( mcu_block = 0; mcu_block < _u8Blocks_per_mcu; mcu_block++ )
        {
            MS_U8 component_id = _u8Mcu_org[mcu_block];
            MS_U8 prev_num_set;
            JPEG_HuffTbl *Ph;

            JPEG_BLOCK_TYPE *p = _ps16Block_seg[row_block];
            JPEG_QUANT_TYPE *q = _QuantTables[_u8Comp_quant[component_id]].s16Value;
            MS_S32 r, s;

            if ( ( s = JPEG_huff_decode( &_Huff_tbls[_u8Comp_dc_tab[component_id]] ) ) != 0 )
            {
                r = JPEG_get_bits_2( s );
                s = HUFF_EXTEND( r, s );
            }

            //printf("r : %d s : %d\n",s,r);

            _u32Last_dc_val[component_id] = ( s += _u32Last_dc_val[component_id] );

            p[0] = s * q[0];

            prev_num_set = _u8Block_max_zag_set[row_block];

            Ph = &_Huff_tbls[_u8Comp_ac_tab[component_id]];

            for ( k = 1; k < 64; k++ )
            {
                s = JPEG_huff_decode( Ph );

                //printf("Decode s :%d\n",s);

                r = s >> 4;
                s &= 15;

                if ( s )
                {
                    if ( r )
                    {
                        if ( ( k + r ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                            return FALSE;
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U32 n = MIN( r, prev_num_set - k );
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += r;
                    }

                    r = JPEG_get_bits_2( s );
                    s = HUFF_EXTEND( r, s );

                    //assert(k < 64);

                    p[_u8ZAG[k]] = s * q[k];
                }
                else
                {
                    if ( r == 15 )
                    {
                        if ( ( k + 15 ) > 63 )
                        {
                            JPEG_terminate( E_JPEG_DECODE_ERROR );
                            return FALSE;
                        }

                        if ( k < prev_num_set )
                        {
                            MS_U16 n = MIN( 16, prev_num_set - k );        //bugfix Dec. 19, 2001 - was 15!
                            MS_U16 kt = k;
                            while ( n-- )
                            {
                                p[_u8ZAG[kt++]] = 0;
                            }
                        }

                        k += 15;
                    }
                    else
                    {
                        //while (k < 64)
                        //  p[ZAG[k++]] = 0;

                        break;
                    }
                }
            }

            if ( k < prev_num_set )
            {
                MS_U16 kt = k;
                while ( kt < prev_num_set && kt < 64)           // kt < 64 is for coverity check
                {
                    p[_u8ZAG[kt++]] = 0;
                }
            }

            _u8Block_max_zag_set[row_block] = k;

            //block_num[row_block++] = k;
            row_block++;
        }

        _u16Restarts_left--;
    }
    return TRUE;
}

/*----------------------------------------------------------------------------*/
JPEG_STATIC void msAPI_JPEG_idct( JPEG_BLOCK_TYPE *data, MS_U8 *Pdst_ptr )
{
    MS_S32 t0, t1, t2, t3;
    MS_S32 t10, t11, t12, t13;
    MS_S32 a1, a2, a3, a4, a5;
    MS_S32 rowctl;
    register JPEG_BLOCK_TYPE *pdata;

    //kevinhuang, use an internal array in idct to avoid memcpy to save time
    JPEG_BLOCK_TYPE workspace[64];
    JPEG_BLOCK_TYPE *wsptr;

    pdata = data;
    wsptr = workspace;
    for ( rowctl = 8 - 1; rowctl >= 0; rowctl-- )
    {
        if ( ( pdata[1] | pdata[2] | pdata[3] | pdata[4] | pdata[5] | pdata[6] | pdata[7] ) == 0 )
        {
            MS_S16 dc_val = ( MS_S16 )( pdata[0] << 2 );

            wsptr[0] = dc_val;
            wsptr[1] = dc_val;
            wsptr[2] = dc_val;
            wsptr[3] = dc_val;
            wsptr[4] = dc_val;
            wsptr[5] = dc_val;
            wsptr[6] = dc_val;
            wsptr[7] = dc_val;

            pdata += 8;       /* advance pointer to next row */
            wsptr += 8;
            continue;
        }

        a2 = ( MS_S32 )pdata[2];
        a3 = ( MS_S32 )pdata[6];

        a1 = (FIX_0_5411) * (a2 + a3);
        t2 = (-FIX_1_8477) * a3 + a1;
        t3 = (FIX_0_7653) * a2 + a1;

        t0 = ( ( MS_S32 )pdata[0] + ( MS_S32 )pdata[4] ) << 13;
        t1 = ( ( MS_S32 )pdata[0] - ( MS_S32 )pdata[4] ) << 13;

        t10 = t0 + t3;
        t13 = t0 - t3;
        t11 = t1 + t2;
        t12 = t1 - t2;

        t0 = ( MS_S32 )pdata[7];
        t1 = ( MS_S32 )pdata[5];
        t2 = ( MS_S32 )pdata[3];
        t3 = ( MS_S32 )pdata[1];

        a1 = t0 + t3;
        a2 = t1 + t2;
        a3 = t0 + t2;
        a4 = t1 + t3;
        a5 = (FIX_1_1758) * (a3 + a4);

        a1 = (-FIX_0_8999) * a1;
        a2 = (-FIX_2_5629) * a2;
        a3 = (-FIX_1_9615) * a3;
        a4 = (-FIX_0_3901) * a4;
        t0 = (FIX_0_2986) * t0;
        t1 = (FIX_2_0531) * t1;
        t2 = (FIX_3_0727) * t2;
        t3 = (FIX_1_5013) * t3;

        a3 += a5;
        a4 += a5;

        t0 += a1 + a3;
        t1 += a2 + a4;
        t2 += a2 + a3;
        t3 += a1 + a4;

        wsptr[0] = ( MS_S16 )DESCALE( t10 + t3, 13 - 2 );
        wsptr[1] = ( MS_S16 )DESCALE( t11 + t2, 13 - 2 );
        wsptr[2] = ( MS_S16 )DESCALE( t12 + t1, 13 - 2 );
        wsptr[3] = ( MS_S16 )DESCALE( t13 + t0, 13 - 2 );
        wsptr[4] = ( MS_S16 )DESCALE( t13 - t0, 13 - 2 );
        wsptr[5] = ( MS_S16 )DESCALE( t12 - t1, 13 - 2 );
        wsptr[6] = ( MS_S16 )DESCALE( t11 - t2, 13 - 2 );
        wsptr[7] = ( MS_S16 )DESCALE( t10 - t3, 13 - 2 );

        pdata += 8;
        wsptr += 8;
    }

    pdata = workspace;
    for ( rowctl = 8 - 1; rowctl >= 0; rowctl-- )
    {
        MS_S16 i;

        if ( ( pdata[8 * 1] | pdata[8 * 2] | pdata[8 * 3] | pdata[8 * 4] | pdata[8 * 5] | pdata[8 * 6] | pdata[8 * 7] ) == 0 )
        {
            MS_S16 dc_val = ( MS_S16 )DESCALE( ( MS_S32 )pdata[0], 2 + 3 ) + 128;

            clamp( dc_val );

            Pdst_ptr[8 * 0] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor == 3)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 4] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor == 2)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 2] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 6] = ( MS_U8 )dc_val;

#if SW_OPTIMIZE
            if(_u8ScaleDownFactor == 1)
            {
                pdata++;
                Pdst_ptr++;
                continue;
            }
#endif

            Pdst_ptr[8 * 1] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 5] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 3] = ( MS_U8 )dc_val;
            Pdst_ptr[8 * 7] = ( MS_U8 )dc_val;

            pdata++;
            Pdst_ptr++;
            continue;
        }

        a2 = ( MS_S32 )pdata[8 * 2];
        a3 = ( MS_S32 )pdata[8 * 6];

        a1 = (FIX_0_5411) * (a2 + a3);
        t2 = (-FIX_1_8477) * a3 + a1;
        t3 = (FIX_0_7653) * a2 + a1;

        t0 = ( ( MS_S32 )pdata[8 * 0] + ( MS_S32 )pdata[8 * 4] ) << 13;
        t1 = ( ( MS_S32 )pdata[8 * 0] - ( MS_S32 )pdata[8 * 4] ) << 13;

        t10 = t0 + t3;
        t13 = t0 - t3;
        t11 = t1 + t2;
        t12 = t1 - t2;

        t0 = ( MS_S32 )pdata[8 * 7];
        t1 = ( MS_S32 )pdata[8 * 5];
        t2 = ( MS_S32 )pdata[8 * 3];
        t3 = ( MS_S32 )pdata[8 * 1];

        a1 = t0 + t3;
        a2 = t1 + t2;
        a3 = t0 + t2;
        a4 = t1 + t3;
        a5 = (FIX_1_1758) * (a3 + a4);

        a1 = (-FIX_0_8999) * a1;
        a2 = (-FIX_2_5629) * a2;
        a3 = (-FIX_1_9615) * a3;
        a4 = (-FIX_0_3901) * a4;
        t0 = (FIX_0_2986) * t0;
        t1 = (FIX_2_0531) * t1;
        t2 = (FIX_3_0727) * t2;
        t3 = (FIX_1_5013) * t3;

        a3 += a5;
        a4 += a5;

        t0 += a1 + a3;
        t1 += a2 + a4;
        t2 += a2 + a3;
        t3 += a1 + a4;

        i = ( MS_S16 )DESCALE( t10 + t3, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 0] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor== 3)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t13 - t0, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 4] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor== 2)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t12 + t1, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 2] = ( MS_U8 )i;

        i = ( MS_S16 )DESCALE( t11 - t2, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 6] = ( MS_U8 )i;

#if SW_OPTIMIZE
        if(_u8ScaleDownFactor== 1)
        {
            pdata++;
            Pdst_ptr++;
            continue;
        }
#endif

        i = ( MS_S16 )DESCALE( t11 + t2, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 1] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t13 + t0, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 3] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t12 - t1, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 5] = ( MS_U8 )i;


        i = ( MS_S16 )DESCALE( t10 - t3, 13 + 2 + 3 ) + 128;
        clamp( i )
        Pdst_ptr[8 * 7] = ( MS_U8 )i;

        pdata++;
        Pdst_ptr++;
    }
}

JPEG_STATIC MS_BOOL msAPI_JPEG_Idct_CMYKConvert(void)
{
    JPEG_BLOCK_TYPE *Psrc_ptr = _ps16Block_seg[0];
    MS_U8 *Pdst_ptr = gpu8Sample_buf;
    MS_U8 *u8Start_Each_Block, *u8Start_Each_Row;
    MS_U32 u32_y_Blcok_Pos = 0, Max_Pixel_Per_Row;
    MS_S32 i;
    MS_U8  u8Row_in_block, k;
    MS_U16 u16Pixel_Per_Block_x = 8, u16Pixel_Per_Block_y = 8;
    MS_U16 u16Pixel_Per_Pack_row = 16, u16TotalBlock_Per_Row, u16ScaleDownStep = 1 << _u8ScaleDownFactor;

    u16TotalBlock_Per_Row = (_u32Max_blocks_per_row)/2;

    u16Pixel_Per_Pack_row = u16Pixel_Per_Pack_row >> _u8ScaleDownFactor;
    u16Pixel_Per_Block_x = u16Pixel_Per_Block_x/* >> _u8ScaleDownFactor*/;
    u16Pixel_Per_Block_y = u16Pixel_Per_Block_y >> _u8ScaleDownFactor;


    Max_Pixel_Per_Row = (u16TotalBlock_Per_Row*u16Pixel_Per_Block_x >> _u8ScaleDownFactor);

    //Get start position of each row
    u8Start_Each_Row = (u8Out_buf + (u32_Decode_Line*(Max_Pixel_Per_Row*u16Pixel_Per_Block_y)));

    for ( i = (MS_S32)_u32Max_blocks_per_row; i > 0;)
    {
        //===================GET Y1 BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Skip U. V block for 422 format
        Psrc_ptr += 256;
        Pdst_ptr += 256;
        i-=4;

        if(i == 0)
        {
            //Already decoded all blocks
            continue;
        }
        else if(i < 0)
        {
            printf("GET BLOCK NUMBER ERROR!\n");
            JPEG_terminate( E_JPEG_DECODE_ERROR );
            return FALSE;
        }

        //Move y block position in a row
        u32_y_Blcok_Pos++;
        //===================GET Y2 BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos--;
        //===================GET U BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 1) ) = 255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET V BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 3) ) =  255 - Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 128;
        Pdst_ptr += 128;
        i-=2;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos+=2;

    }

    u32_Decode_Line++;
    return TRUE;
}

JPEG_STATIC MS_BOOL msAPI_JPEG_Idct_RGBConvert(void)
{
    JPEG_BLOCK_TYPE *Psrc_ptr = _ps16Block_seg[0];
    MS_U8 *Pdst_ptr = gpu8Sample_buf;
    MS_U8 *u8Start_Each_Block, *u8Start_Each_Row;
    MS_U32 u32_y_Blcok_Pos = 0, Max_Pixel_Per_Row;
    MS_S32 i;
    MS_U8  u8Row_in_block, k;
    MS_U16 u16Pixel_Per_Block_x = 8, u16Pixel_Per_Block_y = 8;
    MS_U16 u16Pixel_Per_Pack_row = 16, u16TotalBlock_Per_Row, u16ScaleDownStep = 1 << _u8ScaleDownFactor;

    u16TotalBlock_Per_Row = (_u32Max_blocks_per_row*2)/3;

    u16Pixel_Per_Pack_row = u16Pixel_Per_Pack_row >> _u8ScaleDownFactor;
    u16Pixel_Per_Block_x = u16Pixel_Per_Block_x/* >> _u8ScaleDownFactor*/;
    u16Pixel_Per_Block_y = u16Pixel_Per_Block_y >> _u8ScaleDownFactor;


    Max_Pixel_Per_Row = (u16TotalBlock_Per_Row*u16Pixel_Per_Block_x >> _u8ScaleDownFactor);

    //Get start position of each row
    u8Start_Each_Row = (u8Out_buf + (u32_Decode_Line*(Max_Pixel_Per_Row*u16Pixel_Per_Block_y)));

    for ( i = (MS_S32)_u32Max_blocks_per_row; i > 0;)
    {

        //===================GET R BLOCKs========================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);

        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Skip U. V block for 422 format
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET G BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 1) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        //===================GET B BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + ((k*4) + 3) ) =  Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 64;
        Pdst_ptr += 64;
        i--;

        if(i == 0)
        {
            //Already decoded all blocks
            continue;
        }
        else if(i < 0)
        {
            printf("GET BLOCK NUMBER ERROR!\n");
            JPEG_terminate( E_JPEG_DECODE_ERROR );
            return FALSE;
        }

        //Move y block position in a row
        u32_y_Blcok_Pos++;
        //===================GET R BLOCKs=======================
        msAPI_JPEG_idct( Psrc_ptr, Pdst_ptr );

        //Get start position of block
        u8Start_Each_Block = u8Start_Each_Row + (u32_y_Blcok_Pos*u16Pixel_Per_Pack_row);
        //Write value to destination buffer
        for(u8Row_in_block=0;u8Row_in_block<u16Pixel_Per_Block_x;u8Row_in_block+=u16ScaleDownStep)
        {
            for(k=0;k<u16Pixel_Per_Block_y;k++)
            {
                *(u8Start_Each_Block + (k*2) ) = Pdst_ptr[(u8Row_in_block*8)+(k*u16ScaleDownStep)];
            }
            u8Start_Each_Block += Max_Pixel_Per_Row;
        }

        //Move idct data position
        Psrc_ptr += 192;
        Pdst_ptr += 192;
        i-=3;

        //Move y block position in a row, Reset position for U block
        u32_y_Blcok_Pos++;

    }

    u32_Decode_Line++;
    return TRUE;
}

//------------------------------------------------------------------------------
// Performs a 2D IDCT over the entire row's coefficient buffer.
JPEG_STATIC MS_BOOL msAPI_JPEG_transform_row( void )
{
    JPEG_DEBUG_API_MSG("Max block : %ld\n", _u32Max_blocks_per_row);

    switch ( gu8Scan_type )
    {
        case E_JPEG_CMYK:
            if(!msAPI_JPEG_Idct_CMYKConvert())
                return FALSE;
            break;
        case E_JPEG_RGB:
            if(!msAPI_JPEG_Idct_RGBConvert())
                return FALSE;
            break;
        default :
            JPEG_DEBUG_API_ERR("System don`t support this format!\n");
            JPEG_terminate( E_JPEG_UNSUPPORTED_SAMP_FACTORS );
            return FALSE;
            break;
    }
    return TRUE;

}

JPEG_STATIC JPEG_Result msAPI_JPEG_Baseline_Decode(void)
{

    if(_bProgressive_flag==TRUE)
        return (E_JPEG_FAILED);

    if ( _u16Total_lines_left == 0 )
    {
        return ( E_JPEG_OKAY );
    }

#ifdef JPD_LONGJUMP_SUPPORT
if ( setjmp( _jmp_state ) )
    {
        return ( E_JPEG_FAILED );
    }
#endif

    if( _u16Total_lines_left > 0 )
    {

        if(!msAPI_JPEG_decode_next_row())
            return E_JPEG_FAILED;

        JPEG_DEBUG_API_MSG("Max line num : %d\n",(_u16Total_lines_left));

        if(!msAPI_JPEG_transform_row()) //IDCT
            return E_JPEG_FAILED;

        _u16Total_lines_left -= gu8Max_mcu_y_size;
    }

    return (E_JPEG_DONE);
}
#endif //SW_JPD_RGB_CMYK



void MApi_JPEG_DisableAddressConvert(void)
{
    JPEG_DEBUG_API_MSG("%s\n", __FUNCTION__);
    __bIsMjpeg = FALSE;
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the free memory for temporary usage.
///@param size \b IN indicate the size of wanted free memory
///@return the physical address of free memory
/********************************************************************/
MS_PHYADDR _MApi_JPEG_GetFreeMemory(MS_U32 size)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bProgressive_flag == TRUE)
    {
        return 0;
    }

    if(size > (INTERNAL_BUFFER_SIZE - JPEG_DEFAULT_EXIF_SIZE))
    {
        return 0;
    }

    INTERNAL_BUFFER_SIZE = INTERNAL_BUFFER_SIZE - size;

    return (MS_VA2PA((MS_U32)INTERNAL_BUFFER_ADDR) + INTERNAL_BUFFER_SIZE);
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the data offset of JPEG file.
///@return the data offset of JPEG file
/********************************************************************/
MS_U32 _MApi_JPEG_GetDataOffset(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bThumbnailAccessMode)
    {
        JPEG_DEBUG_API_MSG("%s: Thumbnail mode!!\n", __FUNCTION__);
        return 0;
    }

    JPEG_DEBUG_API_MSG("%s : u32DataOffset =  0x%lx\n", __FUNCTION__ , u32DataOffset);
    return u32DataOffset;
}

//------------------------------------------------------------------------------
/********************************************************************/
///Get the sof marker offset of JPEG file.
///@return the sof marker offset of JPEG file
/********************************************************************/
MS_U32 _MApi_JPEG_GetSOFOffset(void)
{
    if(_bIsInit == FALSE)
    {
        JPEG_DEBUG_API_MSG("%s: JPD have not init!!\n", __FUNCTION__);
        return 0;
    }

    if(_bThumbnailAccessMode)
    {
        JPEG_DEBUG_API_MSG("%s: Thumbnail mode!!\n", __FUNCTION__);
        return 0;
    }

    JPEG_DEBUG_API_MSG("%s : u32SOFOffset =  0x%lx\n", __FUNCTION__ , u32SOFOffset);
    return u32SOFOffset;
}

//------------------------------------------------------------------------------
/********************************************************************/
///Enable/Disable JPD CMYK support.
///@param bEnable \b IN enable/disable supporting JPD CMYK
///@return E_JPEG_OKAY/E_JPEG_FAILED
///Note: It need to be called before init. The default is JPD CMYK support.
/********************************************************************/
JPEG_Result _MApi_JPEG_SupportCMYK(MS_BOOL bEnable)
{
#if SW_JPD_RGB_CMYK
    bEnableCMYK = bEnable;
    return E_JPEG_OKAY;
#else
    JPEG_DEBUG_API_MSG("Not Support CMYK formant!!!\n");
    UNUSED(bEnable);
    return E_JPEG_FAILED;
#endif
}

//------------------------------------------------------------------------------
/********************************************************************/
///Enable/Disable JPD RGB support.
///@param bEnable \b IN enable/disable supporting JPD RGB
///@return E_JPEG_OKAY/E_JPEG_FAILED
///Note: It need to be called before init. The default is JPD RGB support.
/********************************************************************/
JPEG_Result _MApi_JPEG_SupportRGB(MS_BOOL bEnable)
{
#if SW_JPD_RGB_CMYK
    bEnableRGB = bEnable;
    return E_JPEG_OKAY;
#else
    JPEG_DEBUG_API_MSG("Not Support RGB formant!!!\n");
    UNUSED(bEnable);
    return E_JPEG_FAILED;
#endif
}


#if SUPPORT_MPO_FORMAT
MS_BOOL _MApi_JPEG_IsMPOFormat( void )
{
    JPEG_DEBUG_API_MSG("MApi_JPEG_IsMPOFormat=%d\n", bIsMPOFormat);
    return bIsMPOFormat;
}

MS_BOOL _MApi_JPEG_GetMPOIndex(JPEG_MPO_INDEX_INFO **ppMPOIndex)
{
    if(_stMPOIndex.num_of_image == 0xFFFFFFFF)
    {
        JPEG_DEBUG_API_MSG("No MPO Index\n");
        return FALSE;
    }

    *ppMPOIndex = &_stMPOIndex;
    return TRUE;
}

MS_BOOL _MApi_JPEG_GetMPOAttr(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr)
{
    if((JPEG_MPO_MAX_SUPPORT_IMAGE < image_no)
    || (FALSE == _stMPOAttri[image_no - 1].used))
    {
        JPEG_DEBUG_API_MSG("No MPO Attribute with image no. %ld\n", image_no);
        return FALSE;
    }

    *ppMPOAttr = &_stMPOAttri[image_no - 1];
    return TRUE;
}

void _MApi_JPEG_DumpMPO(void)
{
    MS_U32 i = 0;

    if(_stMPOIndex.num_of_image == 0xFFFFFFFF)
    {
        JPEG_DEBUG_API_MSG("No MPO Index\n");
        return;
    }

    JPEG_DEBUG_API_MSG("start_of_offset = %ld, num_of_image = %ld\n", _stMPOIndex.start_of_offset, _stMPOIndex.num_of_image);
    for(i = 0; i<JPEG_MPO_MAX_SUPPORT_IMAGE; i++)
    {
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:attribute = 0x%lx\n", i, _stMPOIndex.mp_entry[i].attribute);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:size = 0x%lx\n", i, _stMPOIndex.mp_entry[i].size);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:offset = 0x%lx\n", i, _stMPOIndex.mp_entry[i].offset);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:image1_no = 0x%x\n", i, _stMPOIndex.mp_entry[i].image1_no);
        JPEG_DEBUG_API_MSG("mp_entry[%ld]:image2_no = 0x%x\n", i, _stMPOIndex.mp_entry[i].image2_no);

        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:MPIndividualNum = 0x%lx\n", i, _stMPOAttri[i].MPIndividualNum);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PanOrientation = 0x%lx\n", i, _stMPOAttri[i].PanOrientation);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PanOverlap_H.numerator = 0x%lx\n", i, _stMPOAttri[i].PanOverlap_H.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PanOverlap_H.denominator = 0x%lx\n", i, _stMPOAttri[i].PanOverlap_H.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PanOverlap_V.numerator = 0x%lx\n", i, _stMPOAttri[i].PanOverlap_V.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PanOverlap_V.denominator = 0x%lx\n", i, _stMPOAttri[i].PanOverlap_V.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:BaseViewpointNum = 0x%lx\n", i, _stMPOAttri[i].BaseViewpointNum);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:ConvergenceAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[i].ConvergenceAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:ConvergenceAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[i].ConvergenceAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:BaselineLength.numerator = 0x%lx\n", i, _stMPOAttri[i].BaselineLength.numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:BaselineLength.denominator = 0x%lx\n", i, _stMPOAttri[i].BaselineLength.denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:VerticalDivergence.s_numerator = 0x%lx\n", i, _stMPOAttri[i].VerticalDivergence.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:VerticalDivergence.s_denominator = 0x%lx\n", i, _stMPOAttri[i].VerticalDivergence.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_X.s_numerator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_X.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_X.s_denominator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_X.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_Y.s_numerator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_Y.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_Y.s_denominator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_Y.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_Z.s_numerator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_Z.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:AxisDistance_Z.s_denominator = 0x%lx\n", i, _stMPOAttri[i].AxisDistance_Z.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:YawAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[i].YawAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:YawAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[i].YawAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PitchAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[i].PitchAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:PitchAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[i].PitchAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:RollAngle.s_numerator = 0x%lx\n", i, _stMPOAttri[i].RollAngle.s_numerator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:RollAngle.s_denominator = 0x%lx\n", i, _stMPOAttri[i].RollAngle.s_denominator);
        JPEG_DEBUG_API_MSG("_stMPOAttri[%ld]:used = 0x%d\n", i, _stMPOAttri[i].used);
    }
    return;
}

MS_BOOL _MApi_JPEG_SetMPOBuffer(MS_U32 read_offset, MS_U32 output_start)
{
    read_offset = MS_PA2KSEG1((MS_U32)read_offset);// | AEON_NON_CACHE_MASK;
    output_start = MS_PA2KSEG1((MS_U32)output_start);// | AEON_NON_CACHE_MASK;

    if((read_offset < MRC_BUFFER_ADDR)
    || (read_offset > (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE)))
    {
        JPEG_DEBUG_API_MSG("JPEG_SetMPOBuffer, read_offset invalid\n");
        return FALSE;
    }

    if(read_offset > (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE/2))
    {
        mpo_load_data = FALSE;
    }

    if(output_start < MWC_BUFFER_ADDR)
    {
        JPEG_DEBUG_API_MSG("JPEG_SetMPOBuffer, output_start invalid\n");
        return FALSE;
    }

    _pu8In_buf_MPO_ofs = (MS_U8*)read_offset;
    _u32In_buf_MPO_left = (MRC_BUFFER_ADDR + MRC_BUFFER_SIZE) - read_offset;
    JPEG_DEBUG_API_MSG("After MApi_JPEG_SetMPOBuffer(), _pu8In_buf_MPO_ofs=0x%lx, _u32In_buf_MPO_left=0x%lx=============\n", (MS_U32)_pu8In_buf_MPO_ofs, (MS_U32)_u32In_buf_MPO_left);
    MWC_BUFFER_ADDR = output_start;
    return TRUE;
}

/********************************************************************/
/// Set Max decoding resolution for MPO
///@param -u16Width \b IN : indicate max decoding width
///@param -u16Height \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMPOMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
    JPEG_DEBUG_API_MSG("%s: Width = %d, Height = %d\n", __FUNCTION__, u16Width, u16Height);
    JPEG_MPO_MAX_WIDTH = u16Width;
    JPEG_MPO_MAX_HEIGHT = u16Height;
    return;
}
//------------------------------------------------------------------------------
/********************************************************************/
/// Set Max decoding resolution of Progressive MPO
///@param -u16ProWidth \b IN : indicate max decoding width
///@param -u16ProHeight \b IN : indicate max decoding height
///@return None
/********************************************************************/
void _MApi_JPEG_SetMPOProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
    JPEG_DEBUG_API_MSG("%s: ProWidth = %d, ProHeight = %d\n", __FUNCTION__, u16ProWidth, u16ProHeight);
    JPEG_MPO_PRO_MAX_WIDTH = u16ProWidth;
    JPEG_MPO_PRO_MAX_HEIGHT = u16ProHeight;
    return;
}

#endif

void msAPI_JPEG_get_APP0_info( MS_U8 *unit,MS_U16 *x, MS_U16 *y )
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETAPP0INFO_PARAM App0Info;
    memset(&App0Info , 0 ,sizeof(JPEG_GETAPP0INFO_PARAM));
    App0Info.unit= unit;
    App0Info.x= x;
    App0Info.y= y;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_get_APP0_info\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_get_APP0_info, (void*)(&App0Info)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _msAPI_JPEG_get_APP0_info(unit, x, y );
#endif
}

JPEG_Result MApi_JPEG_Init(JPEG_InitParam *pInitParam)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return E_JPEG_FAILED;
        }
    }

    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return E_JPEG_FAILED;
    }

    JPEG_INIT_PARAM InitPara;
    memset(&InitPara , 0 ,sizeof(JPEG_INIT_PARAM));
    InitPara.pInitParam = pInitParam;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Init\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_Init, (void*)(&InitPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        InitPara.retVal= E_JPEG_FAILED;
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
    return InitPara.retVal;
#else
    return _MApi_JPEG_Init(pInitParam);
#endif
}

JPEG_Result MApi_JPEG_DecodeHdr(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DecodeHdr\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_DecodeHdr, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_DecodeHdr();
#endif
}

JPEG_Result MApi_JPEG_Decode(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Decode\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_Decode, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_Decode();
#endif
}

void MApi_JPEG_Exit(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Exit\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_Exit, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    else
    {
        if(UtopiaClose(pInstantJPD) != UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Utopia Close JPD failed\n");
            return;
        }
        pInstantJPD = NULL;
    }

#else
    _MApi_JPEG_Exit();
#endif
}

JPEG_ErrCode MApi_JPEG_GetErrorCode(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_NOT_INIT;
    }

    JPEG_GETERRORCODE_PARAM GetErrorCode;
    memset(&GetErrorCode , 0 ,sizeof(JPEG_GETERRORCODE_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetErrorCode\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetErrorCode, (void*)(&GetErrorCode)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_NOT_INIT;
    }
    return GetErrorCode.ErrCode;
#else
    return _MApi_JPEG_GetErrorCode();
#endif
}

JPEG_Event MApi_JPEG_GetJPDEventFlag(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_JPEGEVENT_PARAM JpegEvent;
    memset(&JpegEvent , 0 ,sizeof(JPEG_JPEGEVENT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetJPDEventFlag\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetJPDEventFlag, (void*)(&JpegEvent)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return JpegEvent.eEvtVal;
#else
    return _MApi_JPEG_GetJPDEventFlag();
#endif
}

void MApi_JPEG_SetJPDEventFlag(JPEG_Event eEvtVal)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_JPEGEVENT_PARAM JpegEvent;
    memset(&JpegEvent , 0 ,sizeof(JPEG_JPEGEVENT_PARAM));
    JpegEvent.eEvtVal = eEvtVal;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetJPDEventFlag\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetJPDEventFlag, (void*)(&JpegEvent)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetJPDEventFlag(eEvtVal);
#endif
}

void MApi_JPEG_Rst(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_Rst\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_Rst, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_Rst();
#endif
}

void MApi_JPEG_PowerOn(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_PowerOn\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_PowerOn, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_PowerOn();
#endif
}

void MApi_JPEG_PowerOff(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_PowerOff\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_PowerOff, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_PowerOff();
#endif
}

MS_U16 MApi_JPEG_GetCurVidx(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetCurVidx\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetCurVidx, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetCurVidx();
#endif
}

MS_BOOL MApi_JPEG_IsProgressive(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_IsProgressive\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_IsProgressive, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_IsProgressive();
#endif
}

MS_BOOL MApi_JPEG_ThumbnailFound(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_ThumbnailFound\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_ThumbnailFound, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_ThumbnailFound();
#endif
}

MS_U16 MApi_JPEG_GetWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetWidth\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetWidth();
#endif
}

MS_U16 MApi_JPEG_GetHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetHeight\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetHeight();
#endif
}

MS_U16 MApi_JPEG_GetOriginalWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetOriginalWidth\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetOriginalWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetOriginalWidth();
#endif
}

MS_U16 MApi_JPEG_GetOriginalHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetOriginalHeight\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetOriginalHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetOriginalHeight();
#endif
}

MS_U16 MApi_JPEG_GetNonAlignmentWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetNonAlignmentWidth\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetNonAlignmentWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetNonAlignmentWidth();
#endif
}

MS_U16 MApi_JPEG_GetNonAlignmentHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetNonAlignmentHeight\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetNonAlignmentHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetWidth();
#endif
}

MS_U16 MApi_JPEG_GetAlignedPitch(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedPitch\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetAlignedPitch, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedPitch();
#endif
}

MS_U16 MApi_JPEG_GetAlignedPitch_H(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedPitch_H\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetAlignedPitch_H, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedPitch_H();
#endif
}

MS_U16 MApi_JPEG_GetAlignedWidth(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedWidth\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetAlignedWidth, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedWidth();
#endif
}

MS_U16 MApi_JPEG_GetAlignedHeight(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_U16_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetAlignedHeight\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetAlignedHeight, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U16PARA.u16Value;
#else
    return _MApi_JPEG_GetAlignedHeight();
#endif
}

MS_U8 MApi_JPEG_GetScaleDownFactor(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetScaleDownFactor\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetScaleDownFactor, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U8PARA.u8Value;
#else
    return _MApi_JPEG_GetScaleDownFactor();
#endif
}

void MApi_JPEG_SetMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16Width;
    Resol.u16Height= u16Height;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
#else
    _MApi_JPEG_SetMaxDecodeResolution(u16Width, u16Height);
#endif
}

void MApi_JPEG_SetProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16ProWidth;
    Resol.u16Height= u16ProHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetProMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetProMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
#else
    _MApi_JPEG_SetProMaxDecodeResolution(u16ProWidth, u16ProHeight);
#endif
}

void MApi_JPEG_SetMRBufferValid(JPEG_BuffLoadType u8MRBuffType)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_SETMBUFFERVALID_PARAM MBuffValid;
    memset(&MBuffValid , 0 ,sizeof(JPEG_SETMBUFFERVALID_PARAM));
    MBuffValid.u8MRBuffType = u8MRBuffType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMRBufferValid\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMRBufferValid, (void*)(&MBuffValid)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetMRBufferValid(u8MRBuffType);
#endif
}

void MApi_JPEG_UpdateReadInfo(MS_U32 u32DataRead, MS_BOOL bEOFflag)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_UPDATEREADINFO_PARAM ReadInfo;
    memset(&ReadInfo , 0 ,sizeof(JPEG_UPDATEREADINFO_PARAM));
    ReadInfo.u32DataRead = u32DataRead;
    ReadInfo.bEOFflag = bEOFflag;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_UpdateReadInfo\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_UpdateReadInfo, (void*)(&ReadInfo)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_UpdateReadInfo(u32DataRead, bEOFflag);
#endif
}

void MApi_JPEG_ProcessEOF(JPEG_BuffLoadType u8MRBuffType)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_PROCESSEOF_PARAM EofPara;
    memset(&EofPara , 0 ,sizeof(JPEG_PROCESSEOF_PARAM));
    EofPara.u8MRBuffType = u8MRBuffType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_ProcessEOF\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_ProcessEOF, (void*)(&EofPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_ProcessEOF(u8MRBuffType);
#endif
}

void MApi_JPEG_SetErrCode(JPEG_ErrCode ErrStatus)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_SETERRORCODE_PARAM ErrCode;
    memset(&ErrCode , 0 ,sizeof(JPEG_SETERRORCODE_PARAM));
    ErrCode.ErrStatus = ErrStatus;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetErrCode\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetErrCode, (void*)(&ErrCode)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_SetErrCode(ErrStatus);
#endif
}

void MApi_JPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    U8PARA.u8Value = u8DbgLevel;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetDbgLevel\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetDbgLevel, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
#else
    _MApi_JPEG_SetDbgLevel(u8DbgLevel);
#endif
}

MS_U8 MApi_JPEG_GetDbgLevel(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U8_PARAM U8PARA;
    memset(&U8PARA , 0 ,sizeof(JPEG_U8_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetDbgLevel\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetDbgLevel, (void*)(&U8PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U8PARA.u8Value;
#else
    return _MApi_JPEG_GetDbgLevel();
#endif
}

void MApi_JPEG_GetInfo(JPEG_Info *pJPEG_Info)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETINFO_PARAM eGETINFO;
    memset(&eGETINFO , 0 ,sizeof(JPEG_GETINFO_PARAM));
    eGETINFO.pJPEG_Info = pJPEG_Info;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetInfo\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetInfo, (void*)(&eGETINFO)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_GetInfo(pJPEG_Info);
#endif
}

void MApi_JPEG_GetStatus(JPEG_Status *pJPEG_Status)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_GETSTATUS_PARAM eGETSTATUS;
    memset(&eGETSTATUS , 0 ,sizeof(JPEG_GETSTATUS_PARAM));
    eGETSTATUS.pJPEG_Status = pJPEG_Status;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetStatus\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetStatus, (void*)(&eGETSTATUS)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_GetStatus(pJPEG_Status);
#endif
}

JPEG_Result MApi_JPEG_GetLibVer(const MSIF_Version **ppVersion)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETLIBVER_PARAM eLIBVER;
    memset(&eLIBVER , 0 ,sizeof(JPEG_GETLIBVER_PARAM));
    eLIBVER.ppVersion = ppVersion;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetLibVer\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetLibVer, (void*)(&eLIBVER)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eLIBVER.retVal;
#else
    return _MApi_JPEG_GetLibVer(ppVersion);
#endif
}

JPEG_Result MApi_JPEG_HdlVidxChk(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_HdlVidxChk\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_HdlVidxChk, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_HdlVidxChk();
#endif
}

JPEG_Result MApi_JPEG_GetBuffLoadType(JPEG_BuffLoadType *pBuffLoadType)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETBUFFLOADTYPE_PARAM eBufferLoad;
    memset(&eBufferLoad , 0 ,sizeof(JPEG_GETBUFFLOADTYPE_PARAM));
    eBufferLoad.pBuffLoadType = pBuffLoadType;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetBuffLoadType\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetBuffLoadType, (void*)(&eBufferLoad)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eBufferLoad.retVal;
#else
    return _MApi_JPEG_GetBuffLoadType(pBuffLoadType);
#endif
}

JPEG_Result MApi_JPEG_EnableISR(JPEG_IsrFuncCb IsrCb)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_ENABLEISR_PARAM eISR;
    memset(&eISR , 0 ,sizeof(JPEG_ENABLEISR_PARAM));
    eISR.IsrCb = IsrCb;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_EnableISR\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_EnableISR, (void*)(&eISR)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eISR.retVal;
#else
    return _MApi_JPEG_EnableISR(IsrCb);
#endif
}

JPEG_Result MApi_JPEG_DisableISR(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_RESULT_PARAM ResPara;
    memset(&ResPara , 0 ,sizeof(JPEG_RESULT_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DisableISR\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_DisableISR, (void*)(&ResPara)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return ResPara.retVal;
#else
    return _MApi_JPEG_DisableISR();
#endif
}

JPEG_DecodeStatus MApi_JPEG_WaitDone(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_DEC_DECODING;
    }

    JPEG_WAITDONE_PARAM eWaitDone;
    memset(&eWaitDone , 0 ,sizeof(JPEG_WAITDONE_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_WaitDone\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_WaitDone, (void*)(&eWaitDone)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eWaitDone.retVal;
#else
    return _MApi_JPEG_WaitDone();
#endif
}

JPEG_Result MApi_JPEG_GetEXIFDateTime(JPEG_EXIF_DateTime *DateTime)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFDATETIME_PARAM eEXIFDate;
    memset(&eEXIFDate , 0 ,sizeof(JPEG_GETEXIFDATETIME_PARAM));
    eEXIFDate.DateTime = DateTime;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFDateTime\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFDateTime, (void*)(&eEXIFDate)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFDate.retVal;
#else
    return _MApi_JPEG_GetEXIFDateTime(DateTime);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFOrientation(JPEG_EXIF_Orientation *eOrientation)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFORIENTATION_PARAM eEXIFOri;
    memset(&eEXIFOri , 0 ,sizeof(JPEG_GETEXIFORIENTATION_PARAM));
    eEXIFOri.eOrientation = eOrientation;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFOrientation\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFOrientation, (void*)(&eEXIFOri)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFOri.retVal;
#else
    return _MApi_JPEG_GetEXIFOrientation(eOrientation);
#endif
}

#if SUPPORT_EXIF_EXTRA_INFO
JPEG_Result MApi_JPEG_GetEXIFManufacturer(MS_U8 *pu8Manufacturer, MS_U8 u8size)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFSTRING_PARAM eEXIFStr;
    memset(&eEXIFStr , 0 ,sizeof(JPEG_GETEXIFSTRING_PARAM));
    eEXIFStr.pu8String = pu8Manufacturer;
    eEXIFStr.u8size = u8size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFManufacturer\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFManufacturer, (void*)(&eEXIFStr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFStr.retVal;
#else
    return _MApi_JPEG_GetEXIFManufacturer(pu8Manufacturer, u8size);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFModel(MS_U8 *pu8Model, MS_U8 u8size)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFSTRING_PARAM eEXIFStr;
    memset(&eEXIFStr , 0 ,sizeof(JPEG_GETEXIFSTRING_PARAM));
    eEXIFStr.pu8String = pu8Model;
    eEXIFStr.u8size = u8size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFModel\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFModel, (void*)(&eEXIFStr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return eEXIFStr.retVal;
#else
    return _MApi_JPEG_GetEXIFModel(pu8Model, u8size);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFlash(MS_U16 *pu16Flash)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU16_PARAM U16PARA;
    memset(&U16PARA , 0 ,sizeof(JPEG_GETEXIFU16_PARAM));
    U16PARA.pu16Value = pu16Flash;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFlash\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFFlash, (void*)(&U16PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U16PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFlash(pu16Flash);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFISOSpeedRatings(MS_U32 *pu32ISOSpeedRatings)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ISOSpeedRatings;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFISOSpeedRatings\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFISOSpeedRatings, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFISOSpeedRatings(pu32ISOSpeedRatings);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFShutterSpeedValue(JPEG_RATIONAL *pShutterSpeedValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pShutterSpeedValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFShutterSpeedValue\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFShutterSpeedValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFShutterSpeedValue(pShutterSpeedValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFApertureValue(JPEG_RATIONAL *pApertureValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pApertureValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFApertureValue\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFApertureValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFApertureValue(pApertureValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFExposureBiasValue(JPEG_RATIONAL *pExposureBiasValue)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pExposureBiasValue;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFExposureBiasValue\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFExposureBiasValue, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFExposureBiasValue(pExposureBiasValue);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFocalLength(JPEG_RATIONAL *pFocalLength)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pFocalLength;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFocalLength\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFFocalLength, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFocalLength(pFocalLength);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFImageWidth(MS_U32 *pu32ImageWidth)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ImageWidth;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFImageWidth\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFImageWidth, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFImageWidth(pu32ImageWidth);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFImageHeight(MS_U32 *pu32ImageHeight)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFU32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_GETEXIFU32_PARAM));
    U32PARA.pu32Value = pu32ImageHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFImageHeight\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFImageHeight, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return U32PARA.retVal;
#else
    return _MApi_JPEG_GetEXIFImageHeight(pu32ImageHeight);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFExposureTime(JPEG_RATIONAL *pExposureTime)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pExposureTime;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFExposureTime\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFExposureTime, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFExposureTime(pExposureTime);
#endif
}

JPEG_Result MApi_JPEG_GetEXIFFNumber(JPEG_RATIONAL *pFNumber)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GETEXIFRATIONAL_PARAM RATIONALPARA;
    memset(&RATIONALPARA , 0 ,sizeof(JPEG_GETEXIFRATIONAL_PARAM));
    RATIONALPARA.pRational = pFNumber;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetEXIFFNumber\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetEXIFFNumber, (void*)(&RATIONALPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return RATIONALPARA.retVal;
#else
    return _MApi_JPEG_GetEXIFFNumber(pFNumber);
#endif
}

#endif

MS_PHYADDR MApi_JPEG_GetFreeMemory(MS_U32 size)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETFREEMEMORY_PARAM MEMORYPARA;
    memset(&MEMORYPARA , 0 ,sizeof(JPEG_GETFREEMEMORY_PARAM));
    MEMORYPARA.size = size;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetFreeMemory\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetFreeMemory, (void*)(&MEMORYPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
    return MEMORYPARA.addr;
#else
    return _MApi_JPEG_GetFreeMemory(size);
#endif
}


MS_U32 MApi_JPEG_GetDataOffset(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_U32_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetDataOffset\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetDataOffset, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U32PARA.u32Value;
#else
    return _MApi_JPEG_GetDataOffset();
#endif
}

MS_U32 MApi_JPEG_GetSOFOffset(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_U32_PARAM U32PARA;
    memset(&U32PARA , 0 ,sizeof(JPEG_U32_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetSOFOffset\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetSOFOffset, (void*)(&U32PARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return U32PARA.u32Value;
#else
    return _MApi_JPEG_GetSOFOffset();
#endif
}

JPEG_Result MApi_JPEG_SupportCMYK(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return E_JPEG_FAILED;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return E_JPEG_FAILED;
    }

    JPEG_SUPPORTCMYK_PARAM CMYKRGB;
    memset(&CMYKRGB , 0 ,sizeof(JPEG_SUPPORTCMYK_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SupportCMYK\n");
    CMYKRGB.bEnable = bEnable;
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SupportCMYK, (void*)(&CMYKRGB)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
        return E_JPEG_FAILED;
    }
    return CMYKRGB.retVal;
#else
    return _MApi_JPEG_SupportCMYK(bEnable);
#endif
}

JPEG_Result MApi_JPEG_SupportRGB(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return E_JPEG_FAILED;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return E_JPEG_FAILED;
    }

    JPEG_SUPPORTCMYK_PARAM CMYKRGB;
    memset(&CMYKRGB , 0 ,sizeof(JPEG_SUPPORTCMYK_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SupportRGB\n");
    CMYKRGB.bEnable = bEnable;
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SupportRGB, (void*)(&CMYKRGB)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
        return E_JPEG_FAILED;
    }
    return CMYKRGB.retVal;
#else
    return _MApi_JPEG_SupportRGB(bEnable);
#endif
}

void MApi_JPEG_SetMHEG5(MS_BOOL bEnable)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    bPARA.bValue= bEnable;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMHEG5\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMHEG5, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }
    return;
#else
    _MApi_JPEG_SetMHEG5(bEnable);
#endif
}


#if SUPPORT_MPO_FORMAT
//For MPO
MS_BOOL MApi_JPEG_IsMPOFormat(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_BOOL_PARAM bPARA;
    memset(&bPARA , 0 ,sizeof(JPEG_BOOL_PARAM));
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_IsMPOFormat\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_IsMPOFormat, (void*)(&bPARA)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return bPARA.bValue;
#else
    return _MApi_JPEG_IsMPOFormat();
#endif
}


MS_BOOL MApi_JPEG_GetMPOIndex(JPEG_MPO_INDEX_INFO **ppMPOIndex)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETMPOINDEX_PARAM eMPOIndex;
    memset(&eMPOIndex , 0 ,sizeof(JPEG_GETMPOINDEX_PARAM));
    eMPOIndex.ppMPOIndex = ppMPOIndex;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetMPOIndex\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetMPOIndex, (void*)(&eMPOIndex)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOIndex.bretVal;
#else
    return _MApi_JPEG_GetMPOIndex(ppMPOIndex);
#endif
}

MS_BOOL MApi_JPEG_GetMPOAttr(MS_U32 image_no, JPEG_MPO_ATTRIBUTE_INFO **ppMPOAttr)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_GETMPOATTR_PARAM eMPOAttr;
    memset(&eMPOAttr , 0 ,sizeof(JPEG_GETMPOATTR_PARAM));
    eMPOAttr.image_no = image_no;
    eMPOAttr.ppMPOAttr = ppMPOAttr;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetMPOAttr\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetMPOAttr, (void*)(&eMPOAttr)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOAttr.bretVal;
#else
    return _MApi_JPEG_GetMPOAttr(image_no, ppMPOAttr);
#endif
}

void MApi_JPEG_DumpMPO(void)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return;
    }

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_DumpMPO\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_DumpMPO, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }
#else
    _MApi_JPEG_DumpMPO();
#endif
}

MS_BOOL MApi_JPEG_SetMPOBuffer(MS_U32 read_offset, MS_U32 output_start)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    JPEG_SETMPOBUFFER_PARAM eMPOBuff;
    memset(&eMPOBuff , 0 ,sizeof(JPEG_SETMPOBUFFER_PARAM));
    eMPOBuff.read_offset = read_offset;
    eMPOBuff.output_start = output_start;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOBuffer\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMPOBuffer, (void*)(&eMPOBuff)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    return eMPOBuff.bretVal;
#else
    return _MApi_JPEG_SetMPOBuffer(read_offset, output_start);
#endif
}

void MApi_JPEG_SetMPOMaxDecodeResolution(MS_U16 u16Width, MS_U16 u16Height)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16Width;
    Resol.u16Height= u16Height;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMPOMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
#else
    _MApi_JPEG_SetMPOMaxDecodeResolution(u16Width, u16Height);
#endif
}

void MApi_JPEG_SetMPOProMaxDecodeResolution(MS_U16 u16ProWidth, MS_U16 u16ProHeight)
{
#if (JPEG_UTOPIA20)
    if (pInstantJPD == NULL)
    {
        if(UtopiaOpen(MODULE_JPEG, &pInstantJPD, 0, NULL) !=  UTOPIA_STATUS_SUCCESS)
        {
            JPEG_DEBUG_API_ERR("Open JPD fail\n");
            return;
        }
    }
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        return;
    }

    JPEG_RESOLUTION_PARAM Resol;
    memset(&Resol , 0 ,sizeof(JPEG_RESOLUTION_PARAM));
    Resol.u16Width= u16ProWidth;
    Resol.u16Height= u16ProHeight;
    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_SetMPOProMaxDecodeResolution\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_SetMPOProMaxDecodeResolution, (void*)(&Resol)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        UtopiaClose(pInstantJPD);
        pInstantJPD = NULL;
    }
#else
    _MApi_JPEG_SetMPOProMaxDecodeResolution(u16ProWidth, u16ProHeight);
#endif
}

#endif

JPEG_Result MApi_JPEG_GetControl(EN_JPEG_GET_CTRL_ID eGetID, MS_U32 *param, MS_U32 u32size)
{
#if (JPEG_UTOPIA20)
    if(pInstantJPD==NULL)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }

    JPEG_GET_CTRL_PARAM eGetCTRL;
    memset(&eGetCTRL , 0 ,sizeof(JPEG_GET_CTRL_PARAM));
    eGetCTRL.eGetID = eGetID;
    eGetCTRL.param = param;
    eGetCTRL.u32size = u32size;

    UTOPIA_20_DBG("UtopiaIoctl(): MApi_CMD_JPEG_GetControl\n");
    if(UtopiaIoctl(pInstantJPD,MApi_CMD_JPEG_GetControl, (void*)(&eGetCTRL)) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_JPEG_FAILED;
    }
    return eGetCTRL.retVal;
#else
    return _MApi_JPEG_GetControl(eGetID, param, u32size);
#endif
}

void MApi_JPEG_EnableOJPD(MS_BOOL bOnOff)
{
    // wrapper only
    UNUSED(bOnOff);
    return;
}
JPEG_Result MApi_JPEG_Init_UsingOJPD(JPEG_InitParam *pInitParam)
{
    // wrapper only
    UNUSED(pInitParam);
    return E_JPEG_FAILED;
}
void MApi_JPEG_SetNJPDInstance(MS_U8 JPDNum)
{
    // wrapper only
    UNUSED(JPDNum);
    return;
}
void MApi_JPEG_SetVerificationMode(NJPEG_VerificationMode VerificationMode)
{
    // wrapper only
    UNUSED(VerificationMode);
    return;
}
NJPEG_VerificationMode MApi_JPEG_GetVerificationMode(void)
{
    // wrapper only
    return E_NJPEG00_NONE;
}
void MApi_NJPD_Debug(void)
{
    // wrapper only
    return;
}


