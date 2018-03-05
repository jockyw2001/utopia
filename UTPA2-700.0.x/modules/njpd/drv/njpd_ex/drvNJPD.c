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
/// file    drvNJPD.c
/// @brief  NJPD Driver Interface
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
#include "njpeg_def.h"
#include "osalNJPD.h"
#include "drvNJPD.h"
#include "regNJPD.h"
#include "halNJPD.h"

#if (defined CONFIG_MLOG)
#include "ULog.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//NJPD driver debug macro
#if (defined CONFIG_MLOG)
#define NJPD_DEBUG_DRV_MSG(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_MSG) ULOGI("NJPD", format, ##args);}while(0)
#define NJPD_DEBUG_DRV_ERR(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_ERR) ULOGE("NJPD", format, ##args);}while(0)

#else

#ifndef ANDROID
#define NJPD_DEBUG_DRV_MSG(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_MSG) printf(format, ##args);}while(0)
#define NJPD_DEBUG_DRV_ERR(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_ERR) printf(format, ##args);}while(0)
#else
#define LOG_TAG "DRV_JPD"
#include <cutils/log.h>
#ifndef LOGD
#define NJPD_DEBUG_DRV_MSG(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_MSG) ALOGI(format, ##args);}while(0)
#define NJPD_DEBUG_DRV_ERR(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_ERR) ALOGE(format, ##args);}while(0)
#else
#define NJPD_DEBUG_DRV_MSG(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_MSG) LOGI(format, ##args);}while(0)
#define NJPD_DEBUG_DRV_ERR(format, args...)  do{if(u8NJPD_DrvDbgLevel & E_NJPD_DEBUG_DRV_ERR) LOGE(format, ##args);}while(0)
#endif

#endif
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define ENABLE_FF00FFD_TO_FFFFFFD    FALSE

#if ENABLE_FF00FFD_TO_FFFFFFD
#include "../wdt/drvWDT.h"
#endif

#define HVD_FW_MEM_OFFSET           0x100000UL  // 1M
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL gbEnableTurboFWMode = FALSE;
#ifdef MSOS_TYPE_LINUX_KERNEL
static MS_BOOL gbCallbackFlag = FALSE;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_jpd_version = {
    .DDI = { NJPD_DRV_VERSION },
};

/* capability */
static NJPD_Cap _stNJPD_Cap = {
                                TRUE, //support baseline
                                TRUE, //support progressive
                                TRUE  //support MJPEG
                            };

static NJPD_Status _stNJPD_Status = {
                                      0,        //Current MRC address
                                      0,        //Current decoded Vidx
                                      0,        //Current decoded Row
                                      0,        //Current decoded Col
                                      FALSE,    //busy or not
                                      FALSE     //Isr is enabled or not
                                  };

static MS_U8 u8NJPD_DrvDbgLevel = E_NJPD_DEBUG_DRV_NONE;


static NJPD_IsrFuncCb _pNJPDIsrCb = NULL;

static MS_U32 u32TablePhyAddr[E_NJPD_NJPD_TOTAL];
static MS_BOOL bIs3HuffTbl[E_NJPD_NJPD_TOTAL];
static MS_U8 u8DcLumaCnt[E_NJPD_NJPD_TOTAL];
static MS_U8 u8DcChromaCnt[E_NJPD_NJPD_TOTAL];
static MS_U8 u8DcChroma2Cnt[E_NJPD_NJPD_TOTAL];
static MS_U8 u8AcLumaCnt[E_NJPD_NJPD_TOTAL];
static MS_U8 u8AcChromaCnt[E_NJPD_NJPD_TOTAL];
static MS_U8 u8AcChroma2Cnt[E_NJPD_NJPD_TOTAL];
static JPD_Number eNJPDNum = E_NJPD_NJPD1;


static MS_PHY  u32MRC0_Start[E_NJPD_NJPD_TOTAL];
static MS_PHY  u32MRC0_End[E_NJPD_NJPD_TOTAL];
static MS_PHY  u32MRC1_Start[E_NJPD_NJPD_TOTAL];
static MS_PHY  u32MRC1_End[E_NJPD_NJPD_TOTAL];
static MS_PHY  u32READ_Start[E_NJPD_NJPD_TOTAL];


#if 1
// This is for zigzag inverse for njpd
static const MS_U8 _u8Jpeg_zigzag_order[64] =
{
    0, 2, 3, 9, 10, 20, 21, 35,
    1, 4, 8, 11, 19, 22, 34, 36,
    5, 7, 12, 18, 23, 33, 37, 48,
    6, 13, 17, 24, 32, 38, 47, 49,
    14, 16, 25, 31, 39, 46, 50, 57,
    15, 26, 30, 40, 45, 51, 56, 58,
    27, 29, 41, 44, 52, 55, 59, 62,
    28, 42, 43, 53, 54, 60, 61, 63
};

#else
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
#endif

#if 0
static const MS_U16 g16IQ_TBL[128]=
{
0x0010,0x000b,0x000a,0x0010,0x0018,0x0028,0x0033,0x003d,
0x000c,0x000c,0x000e,0x0013,0x001a,0x003a,0x003c,0x0037,
0x000e,0x000d,0x0010,0x0018,0x0028,0x0039,0x0045,0x0038,
0x000e,0x0011,0x0016,0x001d,0x0033,0x0057,0x0050,0x003e,
0x0012,0x0016,0x0025,0x0038,0x0044,0x006d,0x0067,0x004d,
0x0018,0x0023,0x0037,0x0040,0x0051,0x0068,0x0071,0x005c,
0x0031,0x0040,0x004e,0x0057,0x0067,0x0079,0x0078,0x0065,
0x0048,0x005c,0x005f,0x0062,0x0070,0x0064,0x0067,0x0063,

0x0011,0x0012,0x0018,0x002f,0x0063,0x0063,0x0063,0x0063,
0x0012,0x0015,0x001a,0x0042,0x0063,0x0063,0x0063,0x0063,
0x0018,0x001a,0x0038,0x0063,0x0063,0x0063,0x0063,0x0063,
0x002f,0x0042,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063
};
#endif

static const MS_U16 g16IQ_TBL_NJPD[128]=
{
0x0010,0x000c,0x000e,0x000e,0x0012,0x0018,0x0031,0x0048,
0x000b,0x000c,0x000d,0x0011,0x0016,0x0023,0x0040,0x005c,
0x000a,0x000e,0x0010,0x0016,0x0025,0x0037,0x004e,0x005f,
0x0010,0x0013,0x0018,0x001d,0x0038,0x0040,0x0057,0x0062,
0x0018,0x001a,0x0028,0x0033,0x0044,0x0051,0x0067,0x0070,
0x0028,0x003a,0x0039,0x0057,0x006d,0x0068,0x0079,0x0064,
0x0033,0x003c,0x0045,0x0050,0x0067,0x0071,0x0078,0x0067,
0x003d,0x0037,0x0038,0x003e,0x004d,0x005c,0x0065,0x0063,

0x0011,0x0012,0x0018,0x002f,0x0063,0x0063,0x0063,0x0063,
0x0012,0x0015,0x001a,0x0042,0x0063,0x0063,0x0063,0x0063,
0x0018,0x001a,0x0038,0x0063,0x0063,0x0063,0x0063,0x0063,
0x002f,0x0042,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,
0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063,0x0063

};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _NJPD_ISRHandler(MS_U32 IsrNum)
{
    NJPD_DEBUG_DRV_MSG("_NJPD_ISRHandler: IsrNum = %td\n", (ptrdiff_t)IsrNum);
 #ifdef MSOS_TYPE_LINUX_KERNEL
    gbCallbackFlag = TRUE;
#else
   if(_pNJPDIsrCb)
    {
        _pNJPDIsrCb();
    }
#endif
}

void _NJPD_PrintMem(MS_VIRT u32Addr, MS_U32 u32Size)
{
    MS_U32 u32i;
    NJPD_DEBUG_DRV_MSG("===========================================================\n");
    NJPD_DEBUG_DRV_MSG("print memory addr=0x%tx, size=0x%tx\n", (ptrdiff_t)u32Addr, (ptrdiff_t)u32Size);
    NJPD_DEBUG_DRV_MSG("===========================================================\n");
#if 1
    for(u32i=0; u32i<u32Size/8+((u32Size%8)? 1:0); u32i++)
    {
        NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*8)), *((MS_U8*)(u32Addr+u32i*8+1)), *((MS_U8*)(u32Addr+u32i*8+2)), *((MS_U8*)(u32Addr+u32i*8+3)),
        *((MS_U8*)(u32Addr+u32i*8+4)), *((MS_U8*)(u32Addr+u32i*8+5)), *((MS_U8*)(u32Addr+u32i*8+6)), *((MS_U8*)(u32Addr+u32i*8+7))
        );
    }
#else
    for(u32i=0; u32i<u32Size/16+((u32Size%16)? 1:0); u32i++)
    {
        NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
        *((MS_U8*)(u32Addr+u32i*16)), *((MS_U8*)(u32Addr+u32i*16+1)), *((MS_U8*)(u32Addr+u32i*16+2)), *((MS_U8*)(u32Addr+u32i*16+3)),
        *((MS_U8*)(u32Addr+u32i*16+4)), *((MS_U8*)(u32Addr+u32i*16+5)), *((MS_U8*)(u32Addr+u32i*16+6)), *((MS_U8*)(u32Addr+u32i*16+7)),
        *((MS_U8*)(u32Addr+u32i*16+8)), *((MS_U8*)(u32Addr+u32i*16+9)), *((MS_U8*)(u32Addr+u32i*16+10)), *((MS_U8*)(u32Addr+u32i*16+11)),
        *((MS_U8*)(u32Addr+u32i*16+12)), *((MS_U8*)(u32Addr+u32i*16+13)), *((MS_U8*)(u32Addr+u32i*16+14)), *((MS_U8*)(u32Addr+u32i*16+15))
        );
    }
#endif
    NJPD_DEBUG_DRV_MSG("===========================================================\n");
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/********************************************************************/
///Reset NJPD -- Reset must be called before trigger NJPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_NJPD_Rst(void)
{
    HAL_NJPD_Rst();
}


void MDrv_NJPD_SetReadBuffer0(MS_PHY u32BufAddr, MS_U32 u32BufSize)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);
    HAL_NJPD_SetMIU(u8MiuSel);

    u32MRC0_Start[eNJPDNum] = u32BufAddr;

    // set start address of read buffer
    HAL_NJPD_SetMRCBuf0_StartLow(u32BufAddr & 0xffffUL);
    HAL_NJPD_SetMRCBuf0_StartHigh(u32BufAddr >> 16);

    u32MRC0_End[eNJPDNum] = (( u32BufAddr + u32BufSize ) - 1);
    // set end address of read buffer
    HAL_NJPD_SetMRCBuf0_EndLow((( u32BufAddr + u32BufSize ) - 1)& 0xffffUL);
    HAL_NJPD_SetMRCBuf0_EndHigh((( u32BufAddr + u32BufSize ) - 1) >> 16);

}

void MDrv_NJPD_SetReadBuffer1(MS_PHY u32BufAddr, MS_U32 u32BufSize)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);
    HAL_NJPD_SetMIU(u8MiuSel);

    u32MRC1_Start[eNJPDNum] = u32BufAddr;

    // set start address of read buffer
    HAL_NJPD_SetMRCBuf1_StartLow(u32BufAddr & 0xffffUL);
    HAL_NJPD_SetMRCBuf1_StartHigh(u32BufAddr >> 16);

    u32MRC1_End[eNJPDNum] = (( u32BufAddr + u32BufSize ) - 1);
    // set end address of read buffer
    HAL_NJPD_SetMRCBuf1_EndLow((( u32BufAddr + u32BufSize ) - 1)& 0xffffUL);
    HAL_NJPD_SetMRCBuf1_EndHigh((( u32BufAddr + u32BufSize ) - 1) >> 16);

}


void MDrv_NJPD_SetMRCStartAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    u32READ_Start[eNJPDNum] =u32ByteOffset;

    HAL_NJPD_SetMRCStart_Low((u32ByteOffset & 0xffffUL));
    HAL_NJPD_SetMRCStart_High((u32ByteOffset >> 16));
}

MS_U32 MDrv_NJPD_GetMWCStartAddr(void)
{
    MS_U32 u32Addr;
    u32Addr = (((MS_U32)HAL_NJPD_GetMWCBuf_StartHigh())<<16 | HAL_NJPD_GetMWCBuf_StartLow());
    return u32Addr;
}

MS_U32 MDrv_NJPD_GetWritePtrAddr(void)
{
    MS_U32 u32Addr;
    u32Addr = (((MS_U32)HAL_NJPD_GetMWCBuf_WritePtrHigh())<<16 | HAL_NJPD_GetMWCBuf_WritePtrLow());
    return u32Addr;
}

#if SUPPORT_HIGH_LOW_REVERSE
MS_BOOL MDrv_NJPD_SetReadBuffer_Patch(MS_PHY u32BufAddr, MS_U32 u32BufSize, MS_PHY u32BufStartAddr)
{
    MS_BOOL bHighLowReverse = FALSE;
    MS_U8 u8i;
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);
    HAL_NJPD_SetMIU(u8MiuSel);
    _phy_to_miu_offset(u8MiuSel, u32BufStartAddr, u32BufStartAddr);

    u32READ_Start[eNJPDNum] =u32BufStartAddr;

    MDrv_NJPD_SetMRCStartAddr(u32READ_Start[eNJPDNum]);
    if(u32BufStartAddr >= u32BufAddr+u32BufSize/2)
    {
        bHighLowReverse = TRUE;
        u32MRC1_Start[eNJPDNum] = u32BufAddr;
        u32MRC1_End[eNJPDNum] = (( u32BufAddr + u32BufSize/2 ) - 1);
        u32MRC0_Start[eNJPDNum] = u32BufAddr + u32BufSize/2;
        u32MRC0_End[eNJPDNum] = (( u32BufAddr + u32BufSize ) - 1);
        for(u8i=0; u8i<16; u8i++)
        {
            if((( ( u32MRC1_End[eNJPDNum]/16 - u32READ_Start[eNJPDNum]/16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC1_End[eNJPDNum]/16 - u32MRC1_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) )
            {
                u32MRC1_End[eNJPDNum] += 16;
                u32MRC0_Start[eNJPDNum]+=16;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        u32MRC0_Start[eNJPDNum] = u32BufAddr;
        u32MRC0_End[eNJPDNum] = (( u32BufAddr + u32BufSize/2 ) - 1);
        u32MRC1_Start[eNJPDNum] = u32BufAddr + u32BufSize/2;
        u32MRC1_End[eNJPDNum] = (( u32BufAddr + u32BufSize ) - 1);
        for(u8i=0; u8i<16; u8i++)
        {
            if((( ( u32MRC0_End[eNJPDNum]/16 - u32READ_Start[eNJPDNum]/16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC1_End[eNJPDNum]/16 - u32MRC1_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) )
            {
                u32MRC0_End[eNJPDNum] += 16;
                u32MRC1_Start[eNJPDNum]+=16;
            }
            else
            {
                break;
            }
        }
    }
    HAL_NJPD_SetMRCBuf0_StartLow(u32MRC0_Start[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf0_StartHigh(u32MRC0_Start[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf0_EndLow(u32MRC0_End[eNJPDNum]& 0xffffUL);
    HAL_NJPD_SetMRCBuf0_EndHigh(u32MRC0_End[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf1_StartLow(u32MRC1_Start[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf1_StartHigh(u32MRC1_Start[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf1_EndLow(u32MRC1_End[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf1_EndHigh(u32MRC1_End[eNJPDNum] >> 16);

    NJPD_DEBUG_DRV_MSG("[offset: MRC0: MRC1]=[%ld, %ld, %ld], u8i=%d\n",
        (u32READ_Start[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16,
        (u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16,
        (u32MRC1_End/16 - u32MRC1_Start/16 +1)%16, u8i
        );
    return bHighLowReverse;
}
#else
MS_BOOL MDrv_NJPD_SetReadBuffer_New(MS_PHY u32BufAddr, MS_U32 u32BufSize, MS_PHY u32BufStartAddr)
{
    MS_BOOL bDifferentHL = FALSE;

    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);
    HAL_NJPD_SetMIU(u8MiuSel);
    _phy_to_miu_offset(u8MiuSel, u32BufStartAddr, u32BufStartAddr);

    u32READ_Start[eNJPDNum] =u32BufStartAddr;


    u32MRC0_Start[eNJPDNum] = u32BufAddr;
    u32MRC0_End[eNJPDNum] = (( u32BufAddr + u32BufSize/2 ) - 1);
    u32MRC1_Start[eNJPDNum] = u32BufAddr + u32BufSize/2;
    u32MRC1_End[eNJPDNum] = (( u32BufAddr + u32BufSize ) - 1);
    if(u32READ_Start[eNJPDNum] > u32MRC0_End[eNJPDNum])
    {
        u32MRC0_End[eNJPDNum] = u32READ_Start[eNJPDNum] | 0xF;
        u32MRC1_Start[eNJPDNum] = u32MRC0_End[eNJPDNum] + 1;
        bDifferentHL = TRUE;
    }

    MDrv_NJPD_SetMRCStartAddr(u32READ_Start[eNJPDNum]);
    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
    {
        MS_U8 u8i;
        for(u8i=0; u8i<16; u8i++)
        {
            if((( ( u32MRC0_End[eNJPDNum]/16 - u32READ_Start[eNJPDNum]/16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) ||
                (( ( u32MRC1_End[eNJPDNum]/16 - u32MRC1_Start[eNJPDNum] /16 ) + 1 ) % 16 == 1) )
            {
                u32MRC0_End[eNJPDNum] += 16;
                u32MRC1_Start[eNJPDNum]+=16;
                bDifferentHL = TRUE;
            }
            else
            {
               break;
            }
        }
    }

    HAL_NJPD_SetMRCBuf0_StartLow(u32MRC0_Start[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf0_StartHigh(u32MRC0_Start[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf0_EndLow(u32MRC0_End[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf0_EndHigh(u32MRC0_End[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf1_StartLow(u32MRC1_Start[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf1_StartHigh(u32MRC1_Start[eNJPDNum] >> 16);
    HAL_NJPD_SetMRCBuf1_EndLow(u32MRC1_End[eNJPDNum] & 0xffffUL);
    HAL_NJPD_SetMRCBuf1_EndHigh(u32MRC1_End[eNJPDNum] >> 16);

    NJPD_DEBUG_DRV_MSG("MRC0 start:%tx, MRC0 end:%tx, Offset:%tx\n",
        (ptrdiff_t)u32MRC0_Start[eNJPDNum], (ptrdiff_t)u32MRC0_End[eNJPDNum] , (ptrdiff_t)u32READ_Start[eNJPDNum]);
    NJPD_DEBUG_DRV_MSG("[offset: MRC0: MRC1]=[%td, %td, %td]\n",
        (ptrdiff_t)((u32READ_Start[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16),
        (ptrdiff_t)((u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16),
        (ptrdiff_t)((u32MRC1_End[eNJPDNum]/16 - u32MRC1_Start[eNJPDNum]/16 +1)%16)
        );


    return bDifferentHL;
}
#endif
/******************************************************************************/
///Set output frame buffer address for NJPD writing NJPEG uncompressed data
///@param u32BufAddr \b IN Start address for NJPD reading in MRC buffer
///@return none
/******************************************************************************/
void MDrv_NJPD_SetOutputFrameBuffer(MS_PHY u32BufAddr, MS_U16 u16LineNum)
{

    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32BufAddr, u32BufAddr);
    UNUSED(u8MiuSel);

    HAL_NJPD_SetMWCBuf_StartLow(u32BufAddr & 0xffffUL);
    HAL_NJPD_SetMWCBuf_StartHigh(u32BufAddr >> 16);

    UNUSED(u16LineNum);
}

/******************************************************************************/
///Setting the address of NJPD MRC, MWC buffer
///@param NJPD_BufCfg \b IN
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
#if SUPPORT_HIGH_LOW_REVERSE
MS_BOOL MDrv_NJPD_InitBuf(NJPD_BufCfg in, MS_BOOL bMJPEG)
{
    MS_BOOL bHighLowReverse = FALSE;
    if(in.bThumbnailAccessMode)
    {
        if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
        {
            bHighLowReverse = MDrv_NJPD_SetReadBuffer_Patch(in.u32ThumbnailBufAddr, in.u32ThumbnailBufSize, in.u32ThumbnailBufAddr + in.u32ThumbnailBufOffset);
        }
        else
        {
            // Set MRC start access byte address
            MDrv_NJPD_SetMRCStartAddr(in.u32ThumbnailBufAddr + in.u32ThumbnailBufOffset);
            // Set MRC buffer for NJPD
            if(in.u32ThumbnailBufOffset < in.u32ThumbnailBufSize/2)
            {
                MDrv_NJPD_SetReadBuffer0(in.u32ThumbnailBufAddr, in.u32ThumbnailBufSize/2);
                MDrv_NJPD_SetReadBuffer1(in.u32ThumbnailBufAddr+in.u32ThumbnailBufSize/2, in.u32ThumbnailBufSize/2);
                bHighLowReverse = FALSE;
            }
            else
            {
                NJPD_DEBUG_DRV_MSG("Reverse the MRC High/Low buffer\n");
                MDrv_NJPD_SetReadBuffer1(in.u32ThumbnailBufAddr, in.u32ThumbnailBufSize/2);
                MDrv_NJPD_SetReadBuffer0(in.u32ThumbnailBufAddr+in.u32ThumbnailBufSize/2, in.u32ThumbnailBufSize/2);
                bHighLowReverse = TRUE;
            }
        }
    }
    else
    {
        if(in.bProgressive)
        {
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                bHighLowReverse = MDrv_NJPD_SetReadBuffer_Patch(in.u32MRCBufAddr, in.u32MRCBufSize, in.u32MRCBufAddr);
            }
            else
            {
                // Set MRC buffer for NJPD
                MDrv_NJPD_SetReadBuffer0(in.u32MRCBufAddr, in.u32MRCBufSize/2);
                MDrv_NJPD_SetReadBuffer1(in.u32MRCBufAddr+in.u32MRCBufSize/2, in.u32MRCBufSize/2);
                // Set MRC start access byte address
                MDrv_NJPD_SetMRCStartAddr(in.u32MRCBufAddr);
            }
        }
        else
        {
            NJPD_DEBUG_DRV_MSG("%lx, %lx, %lx", in.u32MRCBufAddr, in.u32MRCBufAddr + in.u32MRCBufSize, in.u32MRCBufAddr + in.u32MRCBufOffset);
            if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
            {
                bHighLowReverse = MDrv_NJPD_SetReadBuffer_Patch(in.u32MRCBufAddr, in.u32MRCBufSize, in.u32MRCBufAddr + in.u32MRCBufOffset);
            }
            else
            {
                // Set MRC start access byte address
                NJPD_DEBUG_DRV_MSG("in.u32MRCBufOffset=0x%lx\n", in.u32MRCBufOffset);
                MDrv_NJPD_SetMRCStartAddr(in.u32MRCBufAddr + in.u32MRCBufOffset);
                // Set MRC buffer for NJPD
                if(in.u32MRCBufOffset < in.u32MRCBufSize/2)
                {
                    MDrv_NJPD_SetReadBuffer0(in.u32MRCBufAddr, in.u32MRCBufSize/2);
                    MDrv_NJPD_SetReadBuffer1(in.u32MRCBufAddr+in.u32MRCBufSize/2, in.u32MRCBufSize/2);
                    bHighLowReverse = FALSE;
                }
                else
                {
                    NJPD_DEBUG_DRV_MSG("Reverse the MRC High/Low buffer\n");
                    MDrv_NJPD_SetReadBuffer1(in.u32MRCBufAddr, in.u32MRCBufSize/2);
                    MDrv_NJPD_SetReadBuffer0(in.u32MRCBufAddr+in.u32MRCBufSize/2, in.u32MRCBufSize/2);
                    bHighLowReverse = TRUE;
                }
            }
        }
    }
    MDrv_NJPD_SetOutputFrameBuffer(in.u32MWCBufAddr, in.u16MWCBufLineNum);

    // Set the end of MWC buffer for NJPD to save the table
    if(bMJPEG==FALSE)
    {
        u32TablePhyAddr[eNJPDNum] = in.u32MWCBufAddr;
    }
    else
    {
        u32TablePhyAddr[eNJPDNum] = in.u32ThumbnailBufAddr;
    }
    NJPD_DEBUG_DRV_MSG("u32TablePhyAddr[eNJPDNum]=0x%x, VA=0x%x\n", u32TablePhyAddr[eNJPDNum], MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]));

    return bHighLowReverse;
}
#else
MS_BOOL MDrv_NJPD_InitBuf(NJPD_BufCfg in, MS_BOOL bMJPEG)
{
    MS_BOOL bDifferentHL = FALSE;
    if(in.bThumbnailAccessMode)
    {
        bDifferentHL = MDrv_NJPD_SetReadBuffer_New(in.u32ThumbnailBufAddr, in.u32ThumbnailBufSize, in.u32ThumbnailBufAddr + in.u32ThumbnailBufOffset);
    }
    else
    {
        if(in.bProgressive)
        {
            bDifferentHL = MDrv_NJPD_SetReadBuffer_New(in.u32MRCBufAddr, in.u32MRCBufSize, in.u32MRCBufAddr);
        }
        else
        {
            NJPD_DEBUG_DRV_MSG("MRC start:%tx, MRC end:%tx, Offset:%tx\n", (ptrdiff_t)in.u32MRCBufAddr, (ptrdiff_t)(in.u32MRCBufAddr + in.u32MRCBufSize), (ptrdiff_t)(in.u32MRCBufAddr + in.u32MRCBufOffset));
            bDifferentHL = MDrv_NJPD_SetReadBuffer_New(in.u32MRCBufAddr, in.u32MRCBufSize, in.u32MRCBufAddr + in.u32MRCBufOffset);
        }
    }
    MDrv_NJPD_SetOutputFrameBuffer(in.u32MWCBufAddr, in.u16MWCBufLineNum);

    // Set the end of MWC buffer for NJPD to save the table
    if(bMJPEG==FALSE)
    {
        u32TablePhyAddr[eNJPDNum] = in.u32MWCBufAddr;
    }
    else
    {
        u32TablePhyAddr[eNJPDNum] = in.u32ThumbnailBufAddr;
    }
    NJPD_DEBUG_DRV_MSG("u32TablePhyAddr[eNJPDNum]=0x%tx, VA=0x%tx\n", (ptrdiff_t)u32TablePhyAddr[eNJPDNum], (ptrdiff_t)MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]));
    return bDifferentHL;
}
#endif

/******************************************************************************/
///Set width and height of picture
///@param u16Width \b IN picture width
///@param u16Height \b IN picture height
///@return none
/******************************************************************************/
void MDrv_NJPD_SetPicDimension(MS_U16 u16PicWidth, MS_U16 u16PicHeight)
{
    HAL_NJPD_SetPic_H(( u16PicWidth >> 3 ) );
    HAL_NJPD_SetPic_V(( u16PicHeight >> 3 ) );
}

/******************************************************************************/
///Get NJPD Event Flag
///@return EventFlag
/******************************************************************************/
MS_U16 MDrv_NJPD_GetEventFlag(void)
{
    return HAL_NJPD_GetEventFlag();
}

/******************************************************************************/
///Set NJPD Event Flag
///@param u16Value \n IN EventFlag value
///@return none
/******************************************************************************/
void MDrv_NJPD_SetEventFlag(MS_U16 u16Value)
{
    // clear by write
    HAL_NJPD_ClearEventFlag(u16Value);
}

/******************************************************************************/
///Set ROI region
///@param start_x \n IN start X position
///@param start_y \n IN start Y position
///@param width \n IN width
///@param height \n IN height
///@return none
/******************************************************************************/
void MDrv_NJPD_SetROI(MS_U16 start_x, MS_U16 start_y, MS_U16 width, MS_U16 height)
{
//    MS_U16 reg_val;

//    reg_val = HAL_NJPD_Get_S_Config();

    HAL_NJPD_SetROI_H(start_x);
    HAL_NJPD_SetROI_V(start_y);
    HAL_NJPD_SetROIWidth(width);
    HAL_NJPD_SetROIHeight(height);
}

/********************************************************************/
///Power On NJPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_NJPD_PowerOn(void)
{
    HAL_NJPD_InitRegBase();
    HAL_NJPD_PowerOn();
#if ENABLE_FF00FFD_TO_FFFFFFD
    MDrv_WDT_Init((WDT_DbgLv)E_WDT_DBGLV_ERR_ONLY);
    MDrv_TIMER_Count(E_TIMER_1, 1);
#endif
    _stNJPD_Status.bIsBusy = TRUE;
}

/********************************************************************/
///Power Off NJPD
///@param NULL
///@return none
/********************************************************************/
void MDrv_NJPD_PowerOff(void)
{
    HAL_NJPD_PowerOff();
    _stNJPD_Status.bIsBusy = FALSE;
}

/********************************************************************/
///Set RSTIntv
///@param u16Value \n IN register RSTIntv
///@return none
/********************************************************************/
void MDrv_NJPD_SetRSTIntv(MS_U16 u16Value)
{
    HAL_NJPD_SetRSTIntv(u16Value);
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_NJPD_GetCurVidx(void)
{
    NJPD_DEBUG_DRV_ERR("Error!!! Do not support %s() in NJPD!!!!!!!!!!!!!!!!\n", __FUNCTION__);
    return 0;
}

/********************************************************************/
///Write Grpinf of Table IndirectAccess
///@param in \b IN structure for Grpinf of Table IndirectAccess
///@return none
/********************************************************************/
void MDrv_NJPD_WriteGrpinf(NJPD_Grpinf in)
{
    MS_U16 i;

    if (in.DHT)
    {
        u8DcLumaCnt[eNJPDNum] = in.u8DcLumaCnt;
        u8DcChromaCnt[eNJPDNum] = in.u8DcChromaCnt;
        u8DcChroma2Cnt[eNJPDNum] = in.u8DcChroma2Cnt;
        u8AcLumaCnt[eNJPDNum] = in.u8AcLumaCnt;
        u8AcChromaCnt[eNJPDNum] = in.u8AcChromaCnt;
        u8AcChroma2Cnt[eNJPDNum] = in.u8AcChroma2Cnt;
        //JPEG_memset((void *)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE)), 0, 0x400);
#if ENABLE_NJPD_DEBUG_MSG
        for(i= 1; i<=16; i++)
        {
            NJPD_DEBUG_DRV_MSG("dc luma i=%d, [%02x %04x %02x]\n", i, in.u8DcLumaValid[i], in.u16DcLumaCode[i], in.u8DcLumaSymbol[i]);
        }
        for(i= 1; i<=16; i++)
        {
            NJPD_DEBUG_DRV_MSG("ac luma i=%d, [%02x %04x %02x]\n", i, in.u8AcLumaValid[i], in.u16AcLumaCode[i], in.u8AcLumaSymbol[i]);
        }
        for(i= 1; i<=16; i++)
        {
            NJPD_DEBUG_DRV_MSG("dc chroma i=%d, [%02x %04x %02x]\n", i, in.u8DcChromaValid[i], in.u16DcChromaCode[i], in.u8DcChromaSymbol[i]);
        }
        for(i= 1; i<=16; i++)
        {
            NJPD_DEBUG_DRV_MSG("ac chroma i=%d, [%02x %04x %02x]\n",i , in.u8AcChromaValid[i], in.u16AcChromaCode[i], in.u8AcChromaSymbol[i]);
        }
        if(bIs3HuffTbl[eNJPDNum])
        {
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("dc chroma2 i=%d, [%02x %04x %02x]\n", i, in.u8DcChroma2Valid[i], in.u16DcChroma2Code[i], in.u8DcChroma2Symbol[i]);
            }
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("ac chroma2 i=%d, [%02x %04x %02x]\n",i, in.u8AcChroma2Valid[i], in.u16AcChroma2Code[i], in.u8AcChroma2Symbol[i]);
            }
        }
#else
        if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
        {
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("dc luma i=%d, [%02x %04x %02x]\n", i, in.u8DcLumaValid[i], in.u16DcLumaCode[i], in.u8DcLumaSymbol[i]);
            }
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("ac luma i=%d, [%02x %04x %02x]\n", i, in.u8AcLumaValid[i], in.u16AcLumaCode[i], in.u8AcLumaSymbol[i]);
            }
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("dc chroma i=%d, [%02x %04x %02x]\n", i, in.u8DcChromaValid[i], in.u16DcChromaCode[i], in.u8DcChromaSymbol[i]);
            }
            for(i= 1; i<=16; i++)
            {
                NJPD_DEBUG_DRV_MSG("ac chroma i=%d, [%02x %04x %02x]\n",i , in.u8AcChromaValid[i], in.u16AcChromaCode[i], in.u8AcChromaSymbol[i]);
            }
            if(bIs3HuffTbl[eNJPDNum])
            {
                for(i= 1; i<=16; i++)
                {
                    NJPD_DEBUG_DRV_MSG("dc chroma2 i=%d, [%02x %04x %02x]\n", i, in.u8DcChroma2Valid[i], in.u16DcChroma2Code[i], in.u8DcChroma2Symbol[i]);
                }
                for(i= 1; i<=16; i++)
                {
                    NJPD_DEBUG_DRV_MSG("ac chroma2 i=%d, [%02x %04x %02x]\n",i, in.u8AcChroma2Valid[i], in.u16AcChroma2Code[i], in.u8AcChroma2Symbol[i]);
                }
            }
        }
#endif
       // DC
        for ( i = 0; i < 16; i++ )
        {
            if(in.u8DcLumaValid[i+1])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+0)) = in.u8DcLumaSymbol[i+1];
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+1)) = in.u16DcLumaCode[i+1]&0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+2)) = (in.u16DcLumaCode[i+1]>>8) &0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+3)) = in.u8DcLumaValid[i+1];
            }
            else
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+0)) = in.u8DcLumaSymbol[i+1];
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+i*4+3)) = in.u8DcLumaValid[i+1];
            }
        }

        // AC
        for ( i = 0; i < 16; i++ )
        {
            if(in.u8AcLumaValid[i+1])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+0)) = ( in.u8AcLumaSymbol[i+1]+in.u8DcLumaCnt);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+1)) = (in.u16AcLumaCode[i+1] & 0xff);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+2)) = ((in.u16AcLumaCode[i+1]>>8) & 0xff);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+3)) = in.u8AcLumaValid[i+1];
            }
            else
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+0)) = ( in.u8AcLumaSymbol[i+1]+in.u8DcLumaCnt);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+16*4+i*4+3)) = in.u8AcLumaValid[i+1];
            }
        }

        for ( i = 0; i < 16; i++ )
        {
            if(in.u8DcChromaValid[i+1])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+0)) = in.u8DcChromaSymbol[i+1];
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+1)) = in.u16DcChromaCode[i+1] &0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+2)) = (in.u16DcChromaCode[i+1]>>8)&0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+3)) = in.u8DcChromaValid[i+1];
            }
            else
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+0)) = in.u8DcChromaSymbol[i+1];
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+32*4+i*4+3)) = in.u8DcChromaValid[i+1];
            }
        }

        for ( i = 0; i < 16; i++ )
        {
            if(in.u8AcChromaValid[i+1])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+0)) = ( in.u8AcChromaSymbol[i+1]+in.u8DcChromaCnt);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+1)) = (in.u16AcChromaCode[i+1] & 0xff);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+2)) = ((in.u16AcChromaCode[i+1]>>8) & 0xff);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+3)) = in.u8AcChromaValid[i+1];
            }
            else
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+0)) = ( in.u8AcChromaSymbol[i+1]+in.u8DcChromaCnt);
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+48*4+i*4+3)) = in.u8AcChromaValid[i+1];
            }
        }

        if(bIs3HuffTbl[eNJPDNum])
        {
            for ( i = 0; i < 16; i++ )
            {
                if(in.u8DcChroma2Valid[i+1])
                {
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+0)) = in.u8DcChroma2Symbol[i+1];
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+1)) = in.u16DcChroma2Code[i+1]&0xff;
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+2)) = (in.u16DcChroma2Code[i+1]>>8) &0xff;
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+3)) = in.u8DcChroma2Valid[i+1];
                }
                else
                {
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+0)) = in.u8DcChroma2Symbol[i+1];
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+64*4+i*4+3)) = in.u8DcChroma2Valid[i+1];
               }
            }

            for ( i = 0; i < 16; i++ )
            {
                if(in.u8AcChroma2Valid[i+1])
                {
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+0)) = ( in.u8AcChroma2Symbol[i+1]+in.u8DcChroma2Cnt );
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+1)) = in.u16AcChroma2Code[i+1]&0xff;
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+2)) =  (in.u16AcChroma2Code[i+1]>>8)&0xff;
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+3)) = in.u8AcChroma2Valid[i+1];
                }
                else
                {
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+0)) = ( in.u8AcChroma2Symbol[i+1]+in.u8DcChroma2Cnt );
                    *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE+80*4+i*4+3)) = in.u8AcChroma2Valid[i+1];
                }
            }
        }
    }

    MsOS_FlushMemory();
    MDrv_NJPD_GTable_Reload_Enable(ENABLE);
#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Group table!!!!!!!!!!!!!!!!\n");
    if(bIs3HuffTbl[eNJPDNum])
    {
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 96*4);
    }
    else
    {
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 64*4);
    }
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        NJPD_DEBUG_DRV_MSG("print the Group table!!!!!!!!!!!!!!!!\n");
        if(bIs3HuffTbl[eNJPDNum])
        {
            _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 96*4);
        }
        else
        {
            _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 64*4);
        }
    }
#endif
    MDrv_NJPD_SetGTableStartAddr(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE);
}
/********************************************************************/
///Write Symidx of Table Indirect Access
///@param in \b IN structure for Symidx of Table Indirect Access
///@return none
/********************************************************************/
void MDrv_NJPD_WriteSymidx(NJPD_Symidx in)
{
    MS_U16 i = 0;
    MS_U16 u16LumaCnt = 0;
    MS_U16 u16ChromaCnt = 0;
    MS_U16 u16Chroma2Cnt = 0;
    MS_U16 u16MaxCnt;

//    JPEG_memset((void *)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE)), 0, 0x400);
    if (in.DHT)
    {
//        for ( i = 0; i < 16; i++ )
        for ( i = 0; i < 12; i++ )
        {
            if(i < u8DcLumaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16LumaCnt*4+0)) = in.u8DcLumaHuffVal[i];
                u16LumaCnt++;
            }
            if(i < u8DcChromaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16ChromaCnt*4+1)) = in.u8DcChromaHuffVal[i];
                u16ChromaCnt++;
            }
            if(bIs3HuffTbl[eNJPDNum] && i < u8DcChroma2Cnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16Chroma2Cnt*4+2)) = in.u8DcChroma2HuffVal[i];
                u16Chroma2Cnt++;
            }
            else if(i < u8DcChromaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16Chroma2Cnt*4+2)) = in.u8DcChromaHuffVal[i];
                u16Chroma2Cnt++;
            }
        }

//        for ( i = 0; i < 240; i++ )
        for ( i = 0; i < 162; i++ )
        {
            if(i < u8AcLumaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16LumaCnt*4+0)) = in.u8AcLumaHuffVal[i];
                u16LumaCnt++;
            }
            if(i < u8AcChromaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16ChromaCnt*4+1)) = in.u8AcChromaHuffVal[i];
                u16ChromaCnt++;
            }
            if(bIs3HuffTbl[eNJPDNum] && i < u8AcChroma2Cnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16Chroma2Cnt*4+2)) = in.u8AcChroma2HuffVal[i];
                u16Chroma2Cnt++;
            }
            else if(i < u8AcChromaCnt[eNJPDNum])
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+u16Chroma2Cnt*4+2)) = in.u8AcChromaHuffVal[i];
                u16Chroma2Cnt++;
            }
        }
    }

    MsOS_FlushMemory();
    MDrv_NJPD_HTable_Reload_Enable(ENABLE);
    u16MaxCnt = (u16LumaCnt>u16ChromaCnt)? u16LumaCnt:u16ChromaCnt;
    u16MaxCnt = (u16MaxCnt>u16Chroma2Cnt)?  u16MaxCnt:u16Chroma2Cnt;
    u16MaxCnt = ((u16MaxCnt%4)==2 || (u16MaxCnt%4)==3)? u16MaxCnt: ((u16MaxCnt/4)*4+2);

#if 0
    if(u16MaxCnt > u16LumaCnt)
    {
        for(i=u16LumaCnt; i<u16MaxCnt; i++)
            JPEG_memset((void*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+i*4+0)), 0, 1);
    }
    if(u16MaxCnt > u16ChromaCnt)
    {
        for(i=u16ChromaCnt; i<u16MaxCnt; i++)
            JPEG_memset((void*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+i*4+1)), 0, 1);
    }
    if(u16MaxCnt > u16Chroma2Cnt)
    {
        for(i=u16Chroma2Cnt; i<u16MaxCnt; i++)
        {
            JPEG_memset((void*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+i*4+2)), 0, 1);
            JPEG_memset((void*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE+i*4+3)), 0, 1);
        }
    }
#endif

#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Huffman table!!!!!!!!!!!!!!!!\n");
    _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE), u16MaxCnt*4);
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        NJPD_DEBUG_DRV_MSG("print the Huffman table!!!!!!!!!!!!!!!!\n");
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE), u16MaxCnt*4);
    }
#endif
    MDrv_NJPD_SetHTableStartAddr(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE);

    if(MDrv_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
    {
        HAL_NJPD_SetHTableSize(0xff);
    }
    else
    {
        HAL_NJPD_SetHTableSize(u16MaxCnt);
    }

}
/********************************************************************/
///Write IQtbl of Table Indirect Access
///@param in \b IN structure for IQtbl of Table Indirect Access
///@return none
/********************************************************************/
void MDrv_NJPD_WriteIQTbl(NJPD_IQTbl in)
{
    MS_U8 i;

    if (in.DQT)
    {
        for(i = 0; i<64; i++)
        {
            if(in.u8CompNum==1)
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+0)) = in.QuantTables[in.u8CompQuant[0]].s16Value[_u8Jpeg_zigzag_order[i]] & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+1)) = in.QuantTables[in.u8CompQuant[0]].s16Value[_u8Jpeg_zigzag_order[i]] & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+2)) = in.QuantTables[in.u8CompQuant[0]].s16Value[_u8Jpeg_zigzag_order[i]] & 0xff;
            }
            else if(in.u8CompNum==2)
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+0)) = (in.QuantTables[in.u8CompQuant[0]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+1)) = (in.QuantTables[in.u8CompQuant[1]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+2)) = (in.QuantTables[in.u8CompQuant[1]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
            }
            else if(in.u8CompNum==3)
            {
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+0)) = (in.QuantTables[in.u8CompQuant[0]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+1)) = (in.QuantTables[in.u8CompQuant[1]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
                *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+2)) = (in.QuantTables[in.u8CompQuant[2]].s16Value[_u8Jpeg_zigzag_order[i]]) & 0xff;
            }
        }
    }
    else
    {
        MS_U8 u8Value;
        for (i=0; i<64; i++)
        {
            u8Value = g16IQ_TBL_NJPD[i] & 0xff;
            *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+i*4+0)) = u8Value;
        }
        for (i=64; i<128; i++)
        {
            u8Value = g16IQ_TBL_NJPD[i] & 0xff;
            *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+(i-64)*4+1)) = u8Value;
            *(MS_U8*)(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE+(i-64)*4+2)) = u8Value;
        }
    }

    if(in.u8CompNum==3)
    {
        MDrv_NJPD_SetDifferentQTable(ENABLE);
    }
    MsOS_FlushMemory();
    MDrv_NJPD_QTable_Reload_Enable(ENABLE);
#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Quantization table!!!!!!!!!!!!!!!!\n");
    _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE), 64*4);
#else
    if(MDrv_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        NJPD_DEBUG_DRV_MSG("print the Quantization table!!!!!!!!!!!!!!!!\n");
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE), 64*4);
    }
#endif
    MDrv_NJPD_SetQTableStartAddr(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE);
}

void MDrv_NJPD_TableRead(void)
{
    MS_U16 u16Value =0;
    MS_U8 u8Table;
    MS_U16 u16Addr;
    MS_U8 u8TableCnt;
    MS_U16 u16RIUValue = HAL_NJPD_GetRIUInterface();

    HAL_NJPD_Set_GlobalSetting02(HAL_NJPD_Get_GlobalSetting02() & ~NJPD_TBC_MODE);

    NJPD_DEBUG_DRV_MSG("\n%s(), read h table start================\n", __FUNCTION__);
    u8Table = 0;        // 2'b00: h table, 2'b01: g table, 2'b10: q table
    u8TableCnt = HAL_NJPD_GetHTableSize();
    for(u16Addr=0; u16Addr<u8TableCnt; u16Addr++)
    {
        u16Value =NJPD_JPD_TBC_TABLE_READ | ((u8Table & 0x3)<<1 & NJPD_JPD_TBC_SEL) | u16Addr<<8;
        HAL_NJPD_SetRIUInterface(u16Value);
        HAL_NJPD_SetWriteOneClearReg_2(NJPD_TBC_EN);
        while(1)
        {
            if(HAL_NJPD_GetRIUInterface() & NJPD_JPD_TBC_TABLE_READ)
            {
                NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x ",
                HAL_NJPD_TBCReadData_L()&0xff,
                (HAL_NJPD_TBCReadData_L()&0xff00) >>8,
                HAL_NJPD_TBCReadData_H()&0xff,
                (HAL_NJPD_TBCReadData_H()&0xff00) >>8
                );
                if(u16Addr%2==1)
                {
                    NJPD_DEBUG_DRV_MSG("\n");
                }
                break;
            }
        }
    }
    NJPD_DEBUG_DRV_MSG("%s(), read h table end  ================\n", __FUNCTION__);

    NJPD_DEBUG_DRV_MSG("\n%s(), read g table start================\n", __FUNCTION__);
    u8Table = 1;        // 2'b00: h table, 2'b01: g table, 2'b10: q table
    if(bIs3HuffTbl[eNJPDNum])
    {
        u8TableCnt = 96;
    }
    else
    {
        u8TableCnt = 64;
    }

    for(u16Addr=0; u16Addr<u8TableCnt; u16Addr++)
    {
        u16Value =NJPD_JPD_TBC_TABLE_READ | ((u8Table & 0x3)<<1 & NJPD_JPD_TBC_SEL) | u16Addr<<8;
        HAL_NJPD_SetRIUInterface(u16Value);
        HAL_NJPD_SetWriteOneClearReg_2(NJPD_TBC_EN);
        while(1)
        {
            if(HAL_NJPD_GetRIUInterface() & NJPD_JPD_TBC_TABLE_READ)
            {
                NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x ",
                HAL_NJPD_TBCReadData_L()&0xff,
                (HAL_NJPD_TBCReadData_L()&0xff00) >>8,
                HAL_NJPD_TBCReadData_H()&0xff,
                (HAL_NJPD_TBCReadData_H()&0xff00) >>8
                );
                if(u16Addr%2==1)
                {
                    NJPD_DEBUG_DRV_MSG("\n");
                }
                break;
            }
        }
    }
    NJPD_DEBUG_DRV_MSG("%s(), read g table end  ================\n", __FUNCTION__);

    NJPD_DEBUG_DRV_MSG("\n%s(), read q table start================\n", __FUNCTION__);
    u8Table = 2;        // 2'b00: h table, 2'b01: g table, 2'b10: q table
    u8TableCnt = 64;
    for(u16Addr=0; u16Addr<u8TableCnt; u16Addr++)
    {
        u16Value =NJPD_JPD_TBC_TABLE_READ | ((u8Table & 0x3)<<1 & NJPD_JPD_TBC_SEL) | u16Addr<<8;
        HAL_NJPD_SetRIUInterface(u16Value);
        HAL_NJPD_SetWriteOneClearReg_2(NJPD_TBC_EN);
        while(1)
        {
            if(HAL_NJPD_GetRIUInterface() & NJPD_JPD_TBC_TABLE_READ)
            {
                NJPD_DEBUG_DRV_MSG("%02x %02x %02x %02x ",
                HAL_NJPD_TBCReadData_L()&0xff,
                (HAL_NJPD_TBCReadData_L()&0xff00) >>8,
                HAL_NJPD_TBCReadData_H()&0xff,
                (HAL_NJPD_TBCReadData_H()&0xff00) >>8
                );
                if(u16Addr%2==1)
                {
                    NJPD_DEBUG_DRV_MSG("\n");
                }
                break;
            }
        }
    }
    NJPD_DEBUG_DRV_MSG("%s(), read q table end  ================\n", __FUNCTION__);
    HAL_NJPD_Set_GlobalSetting02(HAL_NJPD_Get_GlobalSetting02() | NJPD_TBC_MODE);
    HAL_NJPD_SetRIUInterface(u16RIUValue);
}

void MDrv_NJPD_SetVerificationMode(NJPD_VerificationMode VerificationMode)
{
    HAL_NJPD_SetVerificationMode(VerificationMode);
}

NJPD_VerificationMode MDrv_NJPD_GetVerificationMode(void)
{
    return HAL_NJPD_GetVerificationMode();
}

void MDrv_NJPD_Debug()
{
#if (ENABLE_TEST_NJPD_01_table_read_write_test == TRUE)
    MDrv_NJPD_TableRead();
#else
    if(HAL_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        MDrv_NJPD_TableRead();
    }
#endif
    HAL_NJPD_Debug();

    NJPD_DEBUG_DRV_MSG("[offset: MRC0: MRC1]=[%td, %td, %td]\n",
        (ptrdiff_t)((u32READ_Start[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16),
        (ptrdiff_t)((u32MRC0_End[eNJPDNum]/16 - u32MRC0_Start[eNJPDNum]/16 +1)%16),
        (ptrdiff_t)((u32MRC1_End[eNJPDNum]/16 - u32MRC1_Start[eNJPDNum]/16 +1)%16)
        );

    //_NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]), 2048);

}
/********************************************************************/
///Set NJPD Debug level
///@para MS_U8 u8DbgLevel \b IN The NJPD debug level
///@return none
/********************************************************************/
void MDrv_NJPD_SetDbgLevel(MS_U8 u8DbgLevel)
{
    u8NJPD_DrvDbgLevel = u8DbgLevel;
    HAL_NJPD_SetDbgLevel(u8DbgLevel);
}

/********************************************************************/
///Get NJPD driver information
///@param const NJPD_Info *pNJPD_Inf \b OUT The pointer of NJPD info.
///@return none
/********************************************************************/
void MDrv_NJPD_GetInfo(NJPD_Info *pNJPD_Info)
{
    NJPD_HAL_Version HalVer;
    HAL_NJPD_GetLibVer(&HalVer);
    pNJPD_Info->pu8HalVer = HalVer.pu8HalVer;
    pNJPD_Info->pu8FwVer = HalVer.pu8FwVer;
    pNJPD_Info->stCap = _stNJPD_Cap;
}

/********************************************************************/
///Get NJPD driver status
///@param none
///@return the pointer of NJPD_Status
/********************************************************************/
NJPD_Status* MDrv_NJPD_GetStatus(void)
{
    _stNJPD_Status.u32CurMRCAddr = HAL_NJPD_GetCurMRCAddr();
    _stNJPD_Status.u16CurVidx = 0;
    _stNJPD_Status.u16CurRow = HAL_NJPD_GetCurRow();
    _stNJPD_Status.u16CurCol = HAL_NJPD_GetCurCol();
    return &_stNJPD_Status;
}

/********************************************************************/
///Get NJPD driver version
///@param MS_U8 **pVerString \b OUT NJPD version
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_NJPD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return FALSE;
    }
    *ppVersion = &_drv_jpd_version;
    return TRUE;
}

/********************************************************************/
///Enable NJPD ISR and register callback function.
///@param NJPD_IsrFunc IsrCb \b IN Interrupt callback function
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_NJPD_EnableISR(NJPD_IsrFuncCb IsrCb)
{
    if(TRUE == _stNJPD_Status.bIsrEnable)
    {
        NJPD_DEBUG_DRV_MSG("NJPD ISR has been enabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_NJPD_ISR_Attach((OSAL_NJPD_IsrFuncCb)_NJPD_ISRHandler))
    {
        NJPD_DEBUG_DRV_MSG("Attach NJPD ISR Success!!\n");
        if(TRUE == OSAL_NJPD_ISR_Enable())
        {
            NJPD_DEBUG_DRV_MSG("MDrv_NJPD_EnableISR : Success!!\n");
            _stNJPD_Status.bIsrEnable = TRUE;
            if(IsrCb)
            {
                _pNJPDIsrCb = IsrCb;
            }
            return TRUE;
        }
    }
    NJPD_DEBUG_DRV_MSG("MDrv_NJPD_EnableISR Failed!!\n");
    return FALSE;
}

/********************************************************************/
///Disable NJPD ISR and unregister callback function.
///@param NJPD_IsrFunc IsrCb \b IN Interrupt callback function
///@return TRUE/FALSE
/********************************************************************/
MS_BOOL MDrv_NJPD_DisableISR(void)
{
    if(FALSE == _stNJPD_Status.bIsrEnable)
    {
        NJPD_DEBUG_DRV_MSG("NJPD ISR has been disabled!!\n");
        return FALSE;
    }

    if(TRUE == OSAL_NJPD_ISR_Disable())
    {
        NJPD_DEBUG_DRV_MSG("Disable NJPD ISR Success!!\n");
        if(TRUE == OSAL_NJPD_ISR_Detach())
        {
            NJPD_DEBUG_DRV_MSG("MDrv_NJPD_DisableISR : Success!!\n");
            _stNJPD_Status.bIsrEnable = FALSE;
            _pNJPDIsrCb = NULL;
            return TRUE;
        }
    }
    return FALSE;
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U32 MDrv_NJPD_GetCurMRCAddr(void)
{
    return HAL_NJPD_GetCurMRCAddr();
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_NJPD_GetCurRow(void)
{
    return HAL_NJPD_GetCurRow();
}

/********************************************************************/
///Get current vertical line index written to memory
///@return current vertical line index written to memory
/********************************************************************/
MS_U16 MDrv_NJPD_GetCurCol(void)
{
    return HAL_NJPD_GetCurCol();
}
/********************************************************************/
///Set write protect flag
///@return none
/********************************************************************/
void MDrv_NJPD_SetAutoProtect(MS_BOOL enable)
{
    HAL_NJPD_SetAutoProtect(enable);
}

/******************************************************************************/
///Set WPEN end address for NJPD
///@param u32ByteOffset \b IN End address for NJPD writing to MWC buffer
///@return none
/******************************************************************************/
void MDrv_NJPD_SetWPENEndAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    NJPD_DEBUG_DRV_MSG("MDrv_NJPD_SetWPENEndAddr=0x%tx\n", (ptrdiff_t)u32ByteOffset);
    HAL_NJPD_SetWPENUBound_0_L(( u32ByteOffset >> 3 ) & 0xffffUL);
    HAL_NJPD_SetWPENUBound_0_H(( u32ByteOffset >> 3 ) >> 16);
}

void MDrv_NJPD_SetWPENStartAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    NJPD_DEBUG_DRV_MSG("MDrv_NJPD_SetWPENStartAddr=0x%tx\n", (ptrdiff_t)u32ByteOffset);
    HAL_NJPD_SetWPENLBound_0_L(( u32ByteOffset >> 3 ) & 0xffffUL);
    HAL_NJPD_SetWPENLBound_0_H(( u32ByteOffset >> 3 ) >> 16);
}

/********************************************************************/
///Set Spare Register
///@param u16Value \n IN register SCONFIG
///@return none
/********************************************************************/
void MDrv_NJPD_SetSpare(MS_U16 u16Value)
{
    HAL_NJPD_SetSpare00(u16Value);
}

/********************************************************************/
///Get Spare Register
///@return Spare Register
/********************************************************************/
MS_U16 MDrv_NJPD_GetSpare(void)
{
    return HAL_NJPD_GetSpare00();
}

void MDrv_NJPD_SetMRC_Valid(MS_U16 u16Value)
{
    NJPD_DEBUG_DRV_MSG("%s() with u16Value=0x%x\n", __FUNCTION__, u16Value);
#if ENABLE_FF00FFD_TO_FFFFFFD
    MS_U32 u32Time =MDrv_TIMER_GetUs(E_TIMER_1);
    if(u16Value & NJPD_MRC0_VALID)
    {
        MS_U32 u32addr;
        for(u32addr=MS_PA2KSEG1(u32MRC0_Start[eNJPDNum]); u32addr<MS_PA2KSEG1(u32MRC0_End[eNJPDNum]); u32addr++)
        {
            if(((*(MS_U8 *)u32addr)==0xff )&& ((*(MS_U8 *)(u32addr+1))==0x00)
                &&((*(MS_U8 *)(u32addr+2))==0xff )&&(((*(MS_U8 *)(u32addr+3)) >>4)==0xd ))
            {
                (*(MS_U8 *)(u32addr+1)) = 0xff;
            }
        }
    }
    if(u16Value & NJPD_MRC1_VALID)
    {
        MS_U32 u32addr;
        for(u32addr=MS_PA2KSEG1(u32MRC1_Start[eNJPDNum]); u32addr<MS_PA2KSEG1(u32MRC1_End[eNJPDNum]); u32addr++)
        {
            if(((*(MS_U8 *)u32addr)==0xff )&& ((*(MS_U8 *)(u32addr+1))==0x00)
                &&((*(MS_U8 *)(u32addr+2))==0xff )&&(((*(MS_U8 *)(u32addr+3)) >>4)==0xd ))
            {
                (*(MS_U8 *)(u32addr+1)) = 0xff;
            }
        }

    }

    if(u16Value & NJPD_MRC0_VALID && u16Value & NJPD_MRC1_VALID)
        printf("[B]waste %ldus, size=%ld\n", MDrv_TIMER_GetUs(E_TIMER_1)-u32Time, u32MRC1_End[eNJPDNum]-u32MRC0_Start[eNJPDNum]);
    else if(u16Value & NJPD_MRC0_VALID)
        printf("[0]waste %ldus, size=%ld\n", MDrv_TIMER_GetUs(E_TIMER_1)-u32Time, u32MRC0_End[eNJPDNum]-u32MRC0_Start[eNJPDNum]);
    else if(u16Value & NJPD_MRC1_VALID)
        printf("[1]waste %ldus, size=%ld\n", MDrv_TIMER_GetUs(E_TIMER_1)-u32Time, u32MRC1_End[eNJPDNum]-u32MRC1_Start[eNJPDNum]);
#endif

    HAL_NJPD_SetWriteOneClearReg(u16Value);
}

void MDrv_NJPD_DecodeEnable()
{

#if 0
    MDrv_NJPD_TableRead();
#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Group table!!!!!!!!!!!!!!!!\n");
    if(bIs3HuffTbl[eNJPDNum])
    {
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 96*4);
    }
    else
    {
        _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SCWGIF_BASE), 64*4);
    }
#endif

#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Huffman table!!!!!!!!!!!!!!!!\n");
    _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_SYMIDX_BASE), 0x2b8);
#endif

#if ENABLE_NJPD_DEBUG_MSG
    NJPD_DEBUG_DRV_MSG("print the Quantization table!!!!!!!!!!!!!!!!\n");
    _NJPD_PrintMem(MS_PA2KSEG1(u32TablePhyAddr[eNJPDNum]+NJPD_MEM_QTBL_BASE), 64*4);
#endif
#endif

    NJPD_DEBUG_DRV_MSG("%s().....\n", __FUNCTION__);
    HAL_NJPD_SetWriteOneClearReg(NJPD_DECODE_ENABLE);
}


void MDrv_NJPD_TableLoadingStart()
{
    NJPD_DEBUG_DRV_MSG("%s().....\n", __FUNCTION__);
    HAL_NJPD_SetWriteOneClearReg(NJPD_TABLE_LOADING_START);
}


void MDrv_NJPD_ReadLastBuffer()
{
    NJPD_DEBUG_DRV_MSG("%s().....\n", __FUNCTION__);
    HAL_NJPD_SetWriteOneClearReg(NJPD_MRC_LAST);
}

void MDrv_NJPD_SetScalingDownFactor(NJPD_SCALING_DOWN_FACTOR eScalingFactor)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_DOWN_SCALE) | ((MS_U16)eScalingFactor<<0));
    HAL_NJPD_Set_GlobalSetting01(u16Value);
}

void MDrv_NJPD_GTable_Rst(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    if(bEnable)
    {
        u16Value = (HAL_NJPD_Get_GlobalSetting01() & ~NJPD_GTABLE_RST);
        HAL_NJPD_Set_GlobalSetting01(u16Value);
        u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_GTABLE_RST) | ((MS_U16)bEnable<<10));
        HAL_NJPD_Set_GlobalSetting01(u16Value);
    }
}

void MDrv_NJPD_HTable_Reload_Enable(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_HTABLE_RELOAD_EN) | ((MS_U16)bEnable<<11));
    HAL_NJPD_Set_GlobalSetting01(u16Value);
}

void MDrv_NJPD_GTable_Reload_Enable(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_GTABLE_RELOAD_EN) | ((MS_U16)bEnable<<12));
    HAL_NJPD_Set_GlobalSetting01(u16Value);
}

void MDrv_NJPD_QTable_Reload_Enable(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_QTABLE_RELOAD_EN) | ((MS_U16)bEnable<<13));
    HAL_NJPD_Set_GlobalSetting01(u16Value);
}

void MDrv_NJPD_SetSoftwareVLD(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting02() & ~(NJPD_REMOVE_0xFF00 | NJPD_REMOVE_0xFFFF | NJPD_FFD9_EN)) | NJPD_BITSTREAM_LE);
    HAL_NJPD_Set_GlobalSetting02(u16Value);
    u16Value = ((HAL_NJPD_Get_GlobalSetting01() & ~NJPD_SVLD) | ((MS_U16)bEnable<<6));
    HAL_NJPD_Set_GlobalSetting01(u16Value);
    if(bEnable)
    {
        // 20120619, clear last bit when SVJD or NJPD_REMOVE_0xFFFF ==0
        HAL_NJPD_SetSpare00(HAL_NJPD_GetSpare00() & 0xfffeUL);
    }
}

void MDrv_NJPD_SetDifferentQTable(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    u16Value = ((HAL_NJPD_Get_GlobalSetting00() & ~NJPD_SUVQ) | ((MS_U16)bEnable<<7));
    HAL_NJPD_Set_GlobalSetting00(u16Value);
}

void MDrv_NJPD_SetDifferentHTable(MS_BOOL bEnable)
{
    MS_U16 u16Value;
    bIs3HuffTbl[eNJPDNum] = bEnable;
    u16Value = ((HAL_NJPD_Get_GlobalSetting00() & ~NJPD_SUVH) | ((MS_U16)bEnable<<8));
    HAL_NJPD_Set_GlobalSetting00(u16Value);
}

void MDrv_NJPD_Set_GlobalSetting00(MS_U16 u16Value)
{
    HAL_NJPD_Set_GlobalSetting00(u16Value);
}

MS_U16 MDrv_NJPD_Get_GlobalSetting00(void)
{
    return HAL_NJPD_Get_GlobalSetting00();
}

void MDrv_NJPD_SetHTableStartAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    HAL_NJPD_SetHTableStart_Low((u32ByteOffset & 0xffffUL));
    HAL_NJPD_SetHTableStart_High((u32ByteOffset >> 16));
}

void MDrv_NJPD_SetQTableStartAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    HAL_NJPD_SetQTableStart_Low((u32ByteOffset & 0xffffUL));
    HAL_NJPD_SetQTableStart_High((u32ByteOffset >> 16));
}


void MDrv_NJPD_SetGTableStartAddr(MS_PHY u32ByteOffset)
{
    MS_U8 u8MiuSel = 0;
    _phy_to_miu_offset(u8MiuSel, u32ByteOffset, u32ByteOffset);
    UNUSED(u8MiuSel);

    HAL_NJPD_SetGTableStart_Low((u32ByteOffset & 0xffffUL));
    HAL_NJPD_SetGTableStart_High((u32ByteOffset >> 16));
}


void MDrv_NJPD_EnablePowerSaving()
{
    HAL_NJPD_SetClockGate(0xFF);
}

void MDrv_NJPD_SetNJPDInstance(JPD_Number JPDNum)
{
    eNJPDNum = JPDNum;
    HAL_NJPD_SetNJPDInstance(JPDNum);
}

MS_BOOL MDrv_NJPD_IsNeedToPatch(NJPD_PATCH_INDEX eIndex)
{
    return HAL_NJPD_IsNeedToPatch(eIndex);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_NJPD_Get_ISR(void)
{
    return gbCallbackFlag;
}

void MDrv_NJPD_Set_ISR(MS_BOOL val)
{
    gbCallbackFlag = val;
}
#endif

MS_U8 MDrv_NJPD_JPDCount(void)
{
    return HAL_NJPD_JPDCount();
}


