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
/// file    drvJPD.c
/// @brief  JPD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "halCHIP.h"
// Internal Definition
//#include "regCHIP.h"
#include "jpeg_def.h"
#if SUPPORT_OJPD
#include "osalJPD.h"
#include "drvJPD.h"
#include "regJPD.h"
#include "halJPD.h"
#include "drvMIU.h"
#include "halCHIP.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//JPD driver debug macro

#ifndef ANDROID
#define JPD_DEBUG_DRV_MSG(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_MSG) printf(format, ##args);}while(0)
#define JPD_DEBUG_DRV_ERR(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_ERR) printf(format, ##args);}while(0)
#else
#define LOG_TAG "DRV_JPD"
#include <cutils/log.h>
#ifndef LOGD
#define JPD_DEBUG_DRV_MSG(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_MSG) ALOGI(format, ##args);}while(0)
#define JPD_DEBUG_DRV_ERR(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_ERR) ALOGE(format, ##args);}while(0)
#else
#define JPD_DEBUG_DRV_MSG(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_MSG) LOGI(format, ##args);}while(0)
#define JPD_DEBUG_DRV_ERR(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_ERR) LOGE(format, ##args);}while(0)
#endif

#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define HVD_FW_MEM_OFFSET           0x100000UL  // 1M
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
//MS_BOOL gbEnableTurboFWMode = FALSE;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_jpd_version =
{
    .DDI = { JPD_DRV_VERSION },
};

/* capability */
static JPD_Cap _stJPD_Cap = {
                                TRUE, //support baseline
                                TRUE, //support progressive
                                TRUE  //support MJPEG
                            };

static JPD_Status _stJPD_Status = {
                                      0,        //Current MRC address
                                      0,        //Current decoded Vidx
                                      0,        //Current decoded Row
                                      0,        //Current decoded Col
                                      FALSE,    //busy or not
                                      FALSE     //Isr is enabled or not
                                  };

static MS_U8 u8JPD_DrvDbgLevel = E_JPD_DEBUG_DRV_NONE;


static JPD_IsrFuncCb _pJPDIsrCb = NULL;


#define MJPEG_MAX_SUPPORT_STREAM_NUM 2


static const MS_U8 _u8Jpeg_zigzag_order[64] =
{
    0, 1, 5, 6, 14, 15, 27, 28,
    2, 4, 7, 13, 16, 26, 29, 42,
    3, 8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

// Default Table for JPEG
static const MS_U16 g16GRPINFO_TBL[128]=
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

static const MS_U16 g16SYMIDX_TBL[]=
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

static const MS_U16 g16IQ_TBL[128]=
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

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _JPD_ISRHandler(MS_U32 IsrNum)
{
    JPD_DEBUG_DRV_MSG("_JPD_ISRHandler: IsrNum = %ld\n", IsrNum);
    if(_pJPDIsrCb)
    {
        _pJPDIsrCb();
    }
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/********************************************************************/
///Reset JPD -- Reset must be called before trigger JPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_JPD_Rst(void)
{
    HAL_JPD_Rst();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 MDrv_JPD_SetMIU(MS_U32 u32Addr)
{
    MS_U8 u8MiuSel;
    MS_U32 u32Offset;
    _phy_to_miu_offset(u8MiuSel, u32Offset, u32Addr);

    switch(u8MiuSel)
    {
        case 0:
            JPD_DEBUG_DRV_MSG ("   JPD on MIU 0 !!!\n");
            MDrv_MIU_SelMIU(MIU_CLIENT_JPD_RW, MIU_SELTYPE_MIU0);
            break;
        case 1:
            JPD_DEBUG_DRV_MSG ("   JPD on MIU 1 !!!\n");
            MDrv_MIU_SelMIU(MIU_CLIENT_JPD_RW, MIU_SELTYPE_MIU1);
            break;
        case 2:
            JPD_DEBUG_DRV_MSG ("   JPD on MIU 2 !!!\n");
            MDrv_MIU_SelMIU(MIU_CLIENT_JPD_RW, MIU_SELTYPE_MIU2);
            break;
        case 3:
            JPD_DEBUG_DRV_MSG ("   JPD on MIU 3 !!!\n");
            MDrv_MIU_SelMIU(MIU_CLIENT_JPD_RW, MIU_SELTYPE_MIU3);
            break;
        default:
            JPD_DEBUG_DRV_MSG ("   JPD on MIU 0 !!!@\n");
            MDrv_MIU_SelMIU(MIU_CLIENT_JPD_RW, MIU_SELTYPE_MIU0);
            break;
    }

    return u32Offset;
}

/******************************************************************************/
///Set RCReadBuffer for JPD
///@param u32BufAddr \b IN Read buffer linear address in memory
///@param u16BufSize \b IN Read buffer size
///@return none
/******************************************************************************/
void MDrv_JPD_SetReadBuffer(MS_PHYADDR u32BufAddr, MS_U32 u32BufSize)
{

    u32BufAddr = MDrv_JPD_SetMIU(u32BufAddr);

    // set start address of read buffer
    HAL_JPD_SetMRCBufFloorLow(( u32BufAddr >> 3 ) & 0xffff);
    HAL_JPD_SetMRCBufFloorHigh(( u32BufAddr >> 3 ) >> 16);

    // set end address of read buffer
    HAL_JPD_SetMRCBufCeilLow((( ( u32BufAddr + u32BufSize ) >> 3 ) - 1)& 0xffff);
    HAL_JPD_SetMRCBufCeilHigh((( ( u32BufAddr + u32BufSize ) >> 3 ) - 1) >> 16);

    //JPD_DEBUG_DRV_MSG("BK_JPD_RBUF_FLOOR_L = %08lX\n",( u32BufAddr >> 3 ) & 0xffff);
    //JPD_DEBUG_DRV_MSG("BK_JPD_RBUF_FLOOR_H = %08lX\n",( u32BufAddr >> 3 ) >> 16);
    //JPD_DEBUG_DRV_MSG("BK_JPD_RBUF_CEIL_L = %08lX\n",((( u32BufAddr + u32BufSize ) >> 3) - 1) & 0xffff);
    //JPD_DEBUG_DRV_MSG("BK_JPD_RBUF_CEIL_H = %08lX\n",((( u32BufAddr + u32BufSize ) >> 3) - 1) >> 16);
}

/******************************************************************************/
///Set MRC start address for JPD
///@param u32ByteOffset \b IN Start address for JPD reading in MRC buffer
///@return none
/******************************************************************************/
void MDrv_JPD_SetMRCStartAddr(MS_PHYADDR u32ByteOffset)
{
    MS_U8 u8MiuSel;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);

    HAL_JPD_SetRCSMAddrLow((u32ByteOffset & 0xffff));
    HAL_JPD_SetRCSMAddrHigh((u32ByteOffset >> 16));
    //JPD_DEBUG_DRV_MSG("BK_JPD_RCSMADR_L = %08lX\n",(u32ByteOffset & 0xffff));
    //JPD_DEBUG_DRV_MSG("BK_JPD_RCSMADR_H = %08lX\n",(u32ByteOffset >> 16));
}

/******************************************************************************/
///Set output frame buffer address for JPD writing JPEG uncompressed data
///@param u32BufAddr \b IN Start address for JPD reading in MRC buffer
///@return none
/******************************************************************************/
void MDrv_JPD_SetOutputFrameBuffer(MS_PHYADDR u32BufAddr, MS_U16 u16LineNum)
{
    MS_U8 u8MiuSel;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);

    HAL_JPD_SetMWCBufStAddrLow(( u32BufAddr >> 3 ) & 0xffff);
    HAL_JPD_SetMWCBufStAddrHigh(( u32BufAddr >> 3 ) >> 16);
    //JPD_DEBUG_DRV_MSG("BK_JPD_MWBF_SADR_L = %08lX\n",( u32BufAddr >> 3 ) & 0xffff);
    //JPD_DEBUG_DRV_MSG("BK_JPD_MWBF_SADR_H = %08lX\n",( u32BufAddr >> 3 ) >> 16);

#if 1//defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_JANUS) || defined(CHIP_U4) || defined(CHIP_T8) || defined(CHIP_J2)
    HAL_JPD_SetMWBuffLineNum(u16LineNum);
#else
    UNUSED(u16LineNum);
#endif
}

/******************************************************************************/
///Setting the address of JPD MRC, MWC buffer
///@param JPD_BufCfg \b IN
/// structure {
///     MS_U32 u32ThumbnailBufAddr; \b IN Thumbnail buffer address
///     MS_U32 u32ThumbnailBufSize; \b IN Thumbnail buffer size
///     MS_U32 u32ThumbnailBufOffset; \b IN Access byte address offset in
///                                         Thumbnail buffer relative to
///                                         Thumbnail start address
///     MS_U32 u32MRCBufAddr; \b IN MRC buffer address
///     MS_U32 u32MRCBufSize; \b IN MRC buffer size
///     MS_U32 u32MRCBufOffset; \b IN Access byte address offset in MRC buffer
///                                   relative to MRC start address
///     MS_U32 u32MWCBufAddr; \b IN MWC buffer address
///     MS_BOOL bProgressive;
///     MS_BOOL bThumbnailAccessMode;
///           };
///@return none
/******************************************************************************/
void MDrv_JPD_InitBuf(JPD_BufCfg in)
{
    if(in.bThumbnailAccessMode)
    {
        // Set MRC buffer for JPD
        MDrv_JPD_SetReadBuffer(in.u32ThumbnailBufAddr, in.u32ThumbnailBufSize);
        // Set MRC start access byte address
        MDrv_JPD_SetMRCStartAddr(in.u32ThumbnailBufAddr + in.u32ThumbnailBufOffset);
    }
    else
    {
        if(in.bProgressive)
        {
            // Set MRC buffer for JPD
            MDrv_JPD_SetReadBuffer(in.u32MRCBufAddr, in.u32MRCBufSize);
            // Set MRC start access byte address
            MDrv_JPD_SetMRCStartAddr(in.u32MRCBufAddr);
        }
        else
        {
            // Set MRC buffer for JPD
            MDrv_JPD_SetReadBuffer(in.u32MRCBufAddr, in.u32MRCBufSize);
            // Set MRC start access byte address
            MDrv_JPD_SetMRCStartAddr(in.u32MRCBufAddr + in.u32MRCBufOffset);
        }
    }

    // Set MWC buffer for JPD
    MDrv_JPD_SetOutputFrameBuffer(in.u32MWCBufAddr, in.u16MWCBufLineNum);
}

/******************************************************************************/
///Set width and height of picture
///@param u16Width \b IN picture width
///@param u16Height \b IN picture height
///@return none
/******************************************************************************/
void MDrv_JPD_SetPicDimension(MS_U16 u16PicWidth, MS_U16 u16PicHeight)
{
    HAL_JPD_SetPic_H(( u16PicWidth >> 3 ) );
    HAL_JPD_SetPic_V(( u16PicHeight >> 3 ) );
}

/******************************************************************************/
///Get JPD Event Flag
///@return EventFlag
/******************************************************************************/
MS_U16 MDrv_JPD_GetEventFlag(void)
{
    return HAL_JPD_GetEventFlag();
}

/******************************************************************************/
///Set JPD Event Flag
///@param u16Value \n IN EventFlag value
///@return none
/******************************************************************************/
void MDrv_JPD_SetEventFlag(MS_U16 u16Value)
{
    // clear by write
    HAL_JPD_SetEventFlag(u16Value);
}

/******************************************************************************/
///Set ROI region
///@param start_x \n IN start X position
///@param start_y \n IN start Y position
///@param width \n IN width
///@param height \n IN height
///@return none
/******************************************************************************/
void MDrv_JPD_SetROI(MS_U16 start_x, MS_U16 start_y, MS_U16 width, MS_U16 height)
{
//    MS_U16 reg_val;

//    reg_val = HAL_JPD_Get_S_Config();

    HAL_JPD_SetROI_H(start_x);
    HAL_JPD_SetROI_V(start_y);
    HAL_JPD_SetROIWidth(width);
    HAL_JPD_SetROIHeight(height);
}

/********************************************************************/
///Power On JPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_JPD_PowerOn(void)
{
    HAL_JPD_InitRegBase();
    HAL_JPD_PowerOn();
    _stJPD_Status.bIsBusy = TRUE;
}

/********************************************************************/
///Power Off JPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_JPD_PowerOff(void)
{
    HAL_JPD_PowerOff();
    _stJPD_Status.bIsBusy = FALSE;
}

/********************************************************************/
///Set RSTIntv
///@param u16Value \n IN register RSTIntv
///@return none
/********************************************************************/
void MDrv_JPD_SetRSTIntv(MS_U16 u16Value)
{
    HAL_JPD_SetRSTIntv(u16Value);
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_JPD_GetCurVidx(void)
{
    return HAL_JPD_GetCurVidx();
}

#if (JPD_SUPPORT_3_HUFFMAN_TABLE==TRUE)
/********************************************************************/
///Write Grpinf of Table IndirectAccess
///@param in \b IN structure for Grpinf of Table IndirectAccess
///@return none
/********************************************************************/
void MDrv_JPD_WriteGrpinf(JPD_Grpinf in)
{
    MS_U32 reg_value;
    MS_U16 i, valid;

    HAL_JPD_Set_TID_Addr(JPD_MEM_SCWGIF_BASE);

    if (in.DHT)
    {
        // DC Luma
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8DcLumaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16DcLumaCode[i] << 8 ) | ( in.u8DcLumaSymbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        // AC Luma
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8AcLumaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16AcLumaCode[i] << 8 ) | ( in.u8AcLumaSymbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        // DC Chroma
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8DcChromaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16DcChromaCode[i] << 8 ) | ( in.u8DcChromaSymbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        // AC Chroma
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8AcChromaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16AcChromaCode[i] << 8 ) | ( in.u8AcChromaSymbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        if(TRUE == in.bUVHuffman)
        {
            // DC Chroma2
            for ( i = 1; i <= 16; i++ )
            {
                if(in.u8DcChroma2Symbol[i] == 0xFF)
                    valid = 0;
                else
                    valid = 1;

                if ( valid )
                {
                    reg_value = ( valid << 24 ) | ( in.u16DcChroma2Code[i] << 8 ) | ( in.u8DcChroma2Symbol[i] << 4 );
                }
                else
                {
                    reg_value = 0;
                }

                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(reg_value >> 16);
            }

            // AC Chroma2
            for ( i = 1; i <= 16; i++ )
            {
                if(in.u8AcChroma2Symbol[i] == 0xFF)
                    valid = 0;
                else
                    valid = 1;

                if ( valid )
                {
                    reg_value = ( valid << 24 ) | ( in.u16AcChroma2Code[i] << 8 ) | ( in.u8AcChroma2Symbol[i] );
                }
                else
                {
                    reg_value = 0;
                }

                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(reg_value >> 16);
            }
        }//if(TRUE == in.bUVHuffman)
    }
    else//if (in.DHT)
    {
        JPD_DEBUG_DRV_MSG("MDrv_JPD_WriteGrpinf : Not Support NULL huffman table!!\n");
    }
}

/********************************************************************/
///Write Symidx of Table Indirect Access
///@param in \b IN structure for Symidx of Table Indirect Access
///@return none
/********************************************************************/
void MDrv_JPD_WriteSymidx(JPD_Symidx in)
{
    MS_U16 i = 0;
    MS_U32 reg_value;

    HAL_JPD_Set_TID_Addr(JPD_MEM_SYMIDX_BASE);

    if(FALSE == in.bUVHuffman)
    {
        if (in.DHT)
        {
            for ( i = 0; i < 256; i++ )
            {
                reg_value = ((MS_U32)(in.u8AcChromaHuffVal[i]) << 8) | (in.u8AcLumaHuffVal[i]);
                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(0);
            }

            for ( i = 0; i < 16; i++ )
            {
                reg_value = ((MS_U32)(in.u8DcChromaHuffVal[i]) << 8) | (in.u8DcLumaHuffVal[i]) ;
                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(0);
            }
        }
        else
        {
            for(i=0;i<272;i++)
            {
                HAL_JPD_Set_TID_Dat(g16SYMIDX_TBL[i]);
                HAL_JPD_Set_TID_Dat(0);
            }
        }
    }
    else
    {
        if (in.DHT)
        {
            for ( i = 0; i < 256; i++ )
            {
                reg_value = ((MS_U32)(in.u8AcChroma2HuffVal[i]) << 16) | ((MS_U32)(in.u8AcChromaHuffVal[i]) << 8) | (in.u8AcLumaHuffVal[i]) ;
                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(reg_value >> 16);
            }

            for ( i = 0; i < 16; i++ )
            {
                reg_value = ((MS_U32)(in.u8DcChroma2HuffVal[i]) << 16) | ((MS_U32)(in.u8DcChromaHuffVal[i]) << 8) | (in.u8DcLumaHuffVal[i]) ;
                HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
                HAL_JPD_Set_TID_Dat(reg_value >> 16);
            }
        }
        else
        {
            JPD_DEBUG_DRV_MSG("MDrv_JPD_WriteSymidx : Not Support NULL huffman table!!\n");
        }
    }
}
#else
/********************************************************************/
///Write Grpinf of Table IndirectAccess
///@param in \b IN structure for Grpinf of Table IndirectAccess
///@return none
/********************************************************************/
void MDrv_JPD_WriteGrpinf(JPD_Grpinf in)
{
    MS_U32 reg_value;
    MS_U16 i, valid;

    HAL_JPD_Set_TID_Addr(JPD_MEM_SCWGIF_BASE);
    if (in.DHT)
    {
        // DC
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8DcLumaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16DcLumaCode[i] << 8 ) | ( in.u8DcLumaSymbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8DcChromaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16DcChromaCode[i] << 8 ) | ( in.u8DcChromaSymbol[i] << 4 );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        // AC
        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8AcLumaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16AcLumaCode[i] << 8 ) | ( in.u8AcLumaSymbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

        for ( i = 1; i <= 16; i++ )
        {
            if(in.u8AcChromaSymbol[i] == 0xFF)
                valid = 0;
            else
                valid = 1;

            if ( valid )
            {
                reg_value = ( valid << 24 ) | ( in.u16AcChromaCode[i] << 8 ) | ( in.u8AcChromaSymbol[i] );
            }
            else
            {
                reg_value = 0;
            }

            HAL_JPD_Set_TID_Dat(reg_value & 0xffff);
            HAL_JPD_Set_TID_Dat(reg_value >> 16);
        }

    }
    else
    {
        for( i = 0; i < 128; i++ )
            HAL_JPD_Set_TID_Dat(g16GRPINFO_TBL[i]);
    }
}

/********************************************************************/
///Write Symidx of Table Indirect Access
///@param in \b IN structure for Symidx of Table Indirect Access
///@return none
/********************************************************************/
void MDrv_JPD_WriteSymidx(JPD_Symidx in)
{
    MS_U16 i = 0;

    HAL_JPD_Set_TID_Addr(JPD_MEM_SYMIDX_BASE);

    if (in.DHT)
    {
        for ( i = 0; i < 256; i++ )
        {
            HAL_JPD_Set_TID_Dat( (in.u8AcChromaHuffVal[i] << 8) | (in.u8AcLumaHuffVal[i]) );
        }

        for ( i = 0; i < 16; i++ )
        {
            HAL_JPD_Set_TID_Dat( (in.u8DcChromaHuffVal[i] << 8) | (in.u8DcLumaHuffVal[i]) );
        }
    }
    else
    {
        for(i=0;i<272;i++)
            HAL_JPD_Set_TID_Dat(g16SYMIDX_TBL[i]);
    }
}
#endif
/********************************************************************/
///Write IQtbl of Table Indirect Access
///@param in \b IN structure for IQtbl of Table Indirect Access
///@return none
/********************************************************************/
void MDrv_JPD_WriteIQTbl(JPD_IQTbl in)
{
    MS_U8 i, j;

    HAL_JPD_Set_TID_Addr(JPD_MEM_QTBL_BASE);

    if (in.DQT)
    {
        for ( i = 0; i < in.u8CompNum; i++ )
        {
            for(j = 0; j<64; j++)
            {
                HAL_JPD_Set_TID_Dat(in.QuantTables[in.u8CompQuant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
            }
        }

        // if all compoents refer to the same Qtable, need to write Qtable twice
        if (in.u8CompNum == 1)
        {
            //for ( i = 0; i < in.u8CompNum; i++ )
            {
                for(j = 0; j<64; j++)
                {
                    HAL_JPD_Set_TID_Dat(in.QuantTables[in.u8CompQuant[i]].s16Value[_u8Jpeg_zigzag_order[j]]);
                }
            }
        }
    }
    else
    {
        for (i=0; i<128; i++)
            HAL_JPD_Set_TID_Dat(g16IQ_TBL[i]);
    }
}

/********************************************************************/
///Set MCONFIG
///@param u16Value \n IN register MCONFIG
///@return none
/********************************************************************/
void MDrv_JPD_Set_M_Config(MS_U16 u16Value)
{
    HAL_JPD_Set_M_Config(u16Value);
}

/********************************************************************/
///Get MCONFIG
///@return MCONFIG
/********************************************************************/
MS_U16 MDrv_JPD_Get_M_Config(void)
{
    return HAL_JPD_Get_M_Config();
}

/********************************************************************/
///Set SCONFIG
///@param u16Value \n IN register SCONFIG
///@return none
/********************************************************************/
void MDrv_JPD_Set_S_Config(MS_U16 u16Value)
{
    HAL_JPD_Set_S_Config(u16Value);
}

/********************************************************************/
///Get SCONFIG
///@return SCONFIG
/********************************************************************/
MS_U16 MDrv_JPD_Get_S_Config(void)
{
    return HAL_JPD_Get_S_Config();
}

/********************************************************************/
///Set JPD Debug level
///@para MS_U8 u8DbgLevel \b IN The JPD debug level
///@return none
/********************************************************************/
void MDrv_JPD_SetDbgLevel(MS_U8 u8DbgLevel)
{
    u8JPD_DrvDbgLevel = u8DbgLevel;
    HAL_JPD_SetDbgLevel(u8DbgLevel);
}

/********************************************************************/
///Get JPD driver information
///@param const JPD_Info *pJPD_Inf \b OUT The pointer of JPD info.
///@return none
/********************************************************************/
void MDrv_JPD_GetInfo(JPD_Info *pJPD_Info)
{
    JPD_HAL_Version HalVer;
    HAL_JPD_GetLibVer(&HalVer);
    pJPD_Info->pu8HalVer = HalVer.pu8HalVer;
    pJPD_Info->pu8FwVer = HalVer.pu8FwVer;
    pJPD_Info->stCap = _stJPD_Cap;
}

/********************************************************************/
///Get JPD driver status
///@param none
///@return the pointer of JPD_Status
/********************************************************************/
JPD_Status* MDrv_JPD_GetStatus(void)
{
    _stJPD_Status.u32CurMRCAddr = HAL_JPD_GetCurMRCAddr();
    _stJPD_Status.u16CurVidx = HAL_JPD_GetCurVidx();
    _stJPD_Status.u16CurRow = HAL_JPD_GetCurRow();
    _stJPD_Status.u16CurCol = HAL_JPD_GetCurCol();
    return &_stJPD_Status;
}

/********************************************************************/
///Get JPD driver version
///@param MS_U8 **pVerString \b OUT JPD version
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_JPD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return FALSE;
    }
    *ppVersion = &_drv_jpd_version;
    return TRUE;
}

/********************************************************************/
///Enable JPD ISR and register callback function.
///@param JPD_IsrFunc IsrCb \b IN Interrupt callback function
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_JPD_EnableISR(JPD_IsrFuncCb IsrCb)
{
    if(TRUE == _stJPD_Status.bIsrEnable)
    {
        JPD_DEBUG_DRV_MSG("JPD ISR has been enabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_JPD_ISR_Attach((OSAL_JPD_IsrFuncCb)_JPD_ISRHandler))
    {
        JPD_DEBUG_DRV_MSG("Attach JPD ISR Success!!\n");
        if(TRUE == OSAL_JPD_ISR_Enable())
        {
            JPD_DEBUG_DRV_MSG("MDrv_JPD_EnableISR : Success!!\n");
            _stJPD_Status.bIsrEnable = TRUE;
            if(IsrCb)
            {
                _pJPDIsrCb = IsrCb;
            }
            return TRUE;
        }
    }
    JPD_DEBUG_DRV_MSG("MDrv_JPD_EnableISR Failed!!\n");
    return FALSE;
}

/********************************************************************/
///Disable JPD ISR and unregister callback function.
///@param JPD_IsrFunc IsrCb \b IN Interrupt callback function
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_JPD_DisableISR(void)
{
    if(FALSE == _stJPD_Status.bIsrEnable)
    {
        JPD_DEBUG_DRV_MSG("JPD ISR has been disabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_JPD_ISR_Disable())
    {
        JPD_DEBUG_DRV_MSG("Disable JPD ISR Success!!\n");
        if(TRUE == OSAL_JPD_ISR_Detach())
        {
            JPD_DEBUG_DRV_MSG("MDrv_JPD_DisableISR : Success!!\n");
            _stJPD_Status.bIsrEnable = FALSE;
            _pJPDIsrCb = NULL;
            return TRUE;
        }
    }
    return FALSE;
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U32 MDrv_JPD_GetCurMRCAddr(void)
{
    return HAL_JPD_GetCurMRCAddr();
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_JPD_GetCurRow(void)
{
    return HAL_JPD_GetCurRow();
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_JPD_GetCurCol(void)
{
    return HAL_JPD_GetCurCol();
}
#if (JPD_SUPPORT_AUTO_PROTECT==TRUE)
/********************************************************************/
///Set write protect flag
///@return none
/********************************************************************/
void MDrv_JPD_SetAutoProtect(MS_BOOL enable)
{
    HAL_JPD_SetAutoProtect(enable);
}

/******************************************************************************/
///Set WPEN end address for JPD
///@param u32ByteOffset \b IN End address for JPD writing to MWC buffer
///@return none
/******************************************************************************/
void MDrv_JPD_SetWPENEndAddr(MS_PHYADDR u32ByteOffset)
{
    MS_U8 u8MiuSel;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);

    HAL_JPD_SetWPENAddrLow(( u32ByteOffset >> 3 ) & 0xffff);
    HAL_JPD_SetWPENAddrHigh(( u32ByteOffset >> 3 ) >> 16);
    //JPD_DEBUG_DRV_MSG("BK_JPD_MWC_WPEN_L = %08lX\n",(u32ByteOffset & 0xffff));
    //JPD_DEBUG_DRV_MSG("BK_JPD_MWC_WPEN_H = %08lX\n",(u32ByteOffset >> 16));
}

#else
#if 1//defined(CHIP_T3) || defined(CHIP_T4) || defined(CHIP_T7) || defined(CHIP_JANUS) || defined(CHIP_U4) || defined(CHIP_T8) || defined(CHIP_J2)
/********************************************************************/
///Set write protect flag
///@return none
/********************************************************************/
void MDrv_JPD_SetWriteProtect(MS_BOOL enable)
{
    HAL_JPD_SetWriteProtect(enable);
}
#endif
#endif
/********************************************************************/
///Set Spare Register
///@param u16Value \n IN register SCONFIG
///@return none
/********************************************************************/
void MDrv_JPD_SetSpare(MS_U16 u16Value)
{
    HAL_JPD_SetSpare(u16Value);
}

/********************************************************************/
///Get Spare Register
///@return Spare Register
/********************************************************************/
MS_U16 MDrv_JPD_GetSpare(void)
{
    return HAL_JPD_GetSpare();
}

void MDrv_JPD_PrintQTable(void)
{
       MS_U16 u16Cnt;
       MDrv_JPD_Set_S_Config(MDrv_JPD_Get_S_Config() & ~JPD_TBL_RDY);
        HAL_JPD_Set_TID_Addr(JPD_MEM_QTBL_BASE);
        for(u16Cnt=0; u16Cnt<192; u16Cnt++)
        {
            printf("%03d ", (MS_U8)HAL_JPD_Get_TID_Dat());
            if(u16Cnt%16==15)
            {
                printf("\n");
            }
        }

}


#endif
