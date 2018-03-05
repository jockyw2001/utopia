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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
/// file    apiGFX.c
/// @brief  MStar Graphics API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#endif
#include "MsOS.h"
#include "MsCommon.h"
#include "MsTypes.h"
#include "MsVersion.h"
#include "_apigfx_type.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiGFX.h"
#include "apiGFX_v2.h"
#include "apiGFX_private.h"

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
#ifndef GFX_UTOPIA2K
#define GFX_UTOPIA2K
#endif
#endif
//----------------------------------------------------------------------------
// Compile options
//----------------------------------------------------------------------------
// #define GFX Utopia2.0 capibility
void*   pGEInstance=NULL;

static MS_S32           s32GEWrapperMutex;
static  void            (*fpGFX_CBGetFont)(MS_S8, GFX_FontInfo*)=NULL;
static  void            (*fpGFX_CBGetBMP)(MS_S16, GFX_BitmapInfo*)=NULL;

#ifdef MSOS_TYPE_LINUX
static pthread_mutex_t          GFXFireInfoMutex = PTHREAD_MUTEX_INITIALIZER;
#endif
GFX_FireInfo                    GFXFireInfo;
GFX_Set_VQ                      GFXSerVQ;
//----------------------------------------------------------------------------
// Local Defines
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Debug Macros
//----------------------------------------------------------------------------
//#define GFX_UNIT_TEST(fmt, ...)          printf(fmt, ##__VA_ARGS__)

#ifdef GFX_DEBUG

#define _GFX_CHECK_BUFFER_ALIGN0(addr, color_fmt)                                                \
    switch ((color_fmt)){                                                                       \
    case GFX_FMT_I1:                                                                             \
    case GFX_FMT_I2:                                                                             \
    case GFX_FMT_I4:                                                                             \
    case GFX_FMT_I8:                                                                             \
        break;                                                                                  \
    case GFX_FMT_YUV422:                                                                         \
    case GFX_FMT_1ABFGBG12355:                                                                   \
    case GFX_FMT_RGB565 :                                                                        \
    case GFX_FMT_FABAFGBG2266:                                                              \
    case GFX_FMT_ARGB1555 :                                                                      \
    case GFX_FMT_ARGB4444 :                                                                      \
    case GFX_FMT_RGBA5551:                                                                      \
    case GFX_FMT_RGBA4444:                                                                      \
    case GFX_FMT_BGRA5551:                                                                      \
    case GFX_FMT_ABGR1555 :                                                                        \
    case GFX_FMT_ABGR4444:                                                              \
    case GFX_FMT_BGRA4444 :                                                                      \
    case GFX_FMT_BGR565 :                                                                      \
        GFX_ASSERT(!(0x1 & (addr)), GFX_FATAL("[GE DRV][%06d] Bad buffer address (0x%08x, %d)\n", (addr), (color_fmt)));    \
        break;                                                                                  \
    case GFX_FMT_ARGB8888 :                                                                      \
    case GFX_FMT_ABGR8888:                                                                      \
    case GFX_FMT_RGBA8888 :                                                                      \
    case GFX_FMT_BGRA8888:                                                                      \
        GFX_ASSERT(!(0x3 & (addr)), GFX_FATAL("[GE DRV][%06d] Bad buffer address (0x%08x, %d)\n", (addr), (color_fmt)));    \
        break;                                                                                  \
    default:                                                                                    \
        GFX_ASSERT(0, GFX_FATAL("[GE DRV][%06d] Invalid color format\n"));                          \
        break;                                                                                  \
    }


#define _GFX_CHECK_BUFFER_ALIGN1(addr, width, height, pitch, color_fmt)                          \
    switch ((color_fmt)){                                                                       \
    case GFX_FMT_I1:                                                                             \
    case GFX_FMT_I2:                                                                             \
    case GFX_FMT_I4:                                                                             \
    case GFX_FMT_I8:                                                                             \
        break;                                                                                  \
    case GFX_FMT_YUV422:                                                                         \
    case GFX_FMT_1ABFGBG12355:                                                                   \
    case GFX_FMT_FABAFGBG2266:                                                              \
    case GFX_FMT_RGB565 :                                                                        \
    case GFX_FMT_ARGB1555 :                                                                      \
    case GFX_FMT_ARGB4444 :                                                                      \
    case GFX_FMT_RGBA5551:                                                                      \
    case GFX_FMT_RGBA4444:                                                                      \
    case GFX_FMT_BGRA5551:                                                                      \
    case GFX_FMT_ABGR1555 :                                                                        \
    case GFX_FMT_ABGR4444:                                                              \
    case GFX_FMT_BGRA4444 :                                                                      \
    case GFX_FMT_BGR565 :                                                                      \
        GFX_ASSERT(!(0x1 & (addr)), GFX_FATAL("[GE DRV][%06d] Bad buffer address (0x%08x, %d)\n", (addr), (color_fmt)));            \
        GFX_ASSERT(!(0x1 & (pitch)), GFX_FATAL("[GE DRV][%06d] Bad buffer pitch (%d, %d)\n", (pitch), (color_fmt)));                \
        GFX_ASSERT(((pitch)>>1)== (width), GFX_FATAL("[GE DRV][%06d] Bad buffer pitch/width (%d, %d)\n", (pitch), (width)));        \
        break;                                                                                  \
    case GFX_FMT_ARGB8888 :                                                                      \
    case GFX_FMT_ABGR8888:                                                                      \
    case GFX_FMT_RGBA8888 :                                                                      \
    case GFX_FMT_BGRA8888:                                                                      \
        GFX_ASSERT(!(0x3 & (addr)), GFX_FATAL("[GE DRV][%06d] Bad buffer address (0x%08x, %d)\n", (addr), (color_fmt)));            \
        GFX_ASSERT(!(0x3 & (pitch)), GFX_FATAL("[GE DRV][%06d] Bad buffer pitch (%d, %d)\n", (pitch), (color_fmt)));                \
        GFX_ASSERT(((pitch)>>2)== (width), GFX_FATAL("[GE DRV][%06d] Bad buffer pitch/width (%d, %d)\n", (pitch), (width)));        \
        break;                                                                                  \
    default:                                                                                    \
        GFX_ASSERT(0, GFX_FATAL("[GE DRV][%06d] Invalid color format\n"));                          \
        break;                                                                                  \
    }




#else // #ifdef GFX_DEBUG

#define _GFX_CHECK_BUFFER_ALIGN0(addr, color_fmt)                                while (0);
#define _GFX_CHECK_BUFFER_ALIGN1(addr, width, height, pitch, color_fmt)          while (0);

#endif // #ifdef GFX_DEBUG

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
//#define CHECK_IDLE() while(BLT_Reg(BLT_REG_STATUS)&BLT_VAL_BUSY) { MsOS_DelayTask(1);}
// #define CHECK_IDLE() while(BLT_Reg(BLT_REG_STATUS)&BLT_VAL_BUSY) { MsOS_YieldTask();}
#define delay(ms)
//#define delay(ms)    MsOS_DelayTask(ms)

#define MAKE_PRIMITIVE_COLOR_12355(_bg, _fg, _blink, _alpha) (((((0x7 & (_blink))<<3)|((0x3 & (_alpha))<<6)|0xFF00)<<16) | \
                                                                        (((0x1F & (_bg))<<3)|((0x1F & (_fg))<<11)))
#define MAKE_PRIMITIVE_COLOR_2266(_bg, _fg, _ba, _fa) ((((0x3 & (_ba)) | ((0x3 & (_fa))<<8))<<16) |\
                                                                ((0x1F & (_bg))|((0x1F & (_fg))<<8)))

//----------------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------------
//static MS_U32 _u32FrameWidth, _u32FrameHeight, _u32FramePitch;


#ifndef MSOS_TYPE_OPTEE
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_gfx_version = {
    .DDI = { GFX_DRV_VERSION },
};
#endif


/// 0: No DBG msg at all
/// 1: Default development level (All ERROR)
/// 2: Verbose level


#define APIGFX_CHECK_NULL(pParameter) \
{                               \
    if(pParameter == NULL)      \
    {                           \
        GFX_ERR("\n Input Parameter is NULL, please to check! \n");\
        return GFX_INIT_FAIL;   \
    }                           \
}
#define APIGFX_CHECK_INIT()      \
{                                \
    if(pGEInstance == NULL)\
    {                            \
        GFX_ERR("\nshould call MApi_GFX_Init first\n");\
        return GFX_INIT_FAIL;    \
    }                            \
}
//----------------------------------------------------------------------------
// Local Function Prototypes
//----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//[OBSOLETE]

#define GFXDRIVER_BASE 0UL


#define CheckGFXInstanceOpen() do{\
    if(NULL == pGEInstance)\
    {\
        if(UtopiaOpen(MODULE_GFX | GFXDRIVER_BASE, &pGEInstance, 0, NULL))\
        {\
            GFX_ERR("%s fail [LINE:%d]    GFXOpen Fail\n",__FUNCTION__, __LINE__);\
            return ;\
        }\
    }\
}while(0)
#ifndef MSOS_TYPE_OPTEE
static void GFX_FireInfoLock(void)
{

#ifdef MSOS_TYPE_LINUX
    pthread_mutex_lock(&GFXFireInfoMutex);
#endif

}

static void GFX_FireInfoUnLock(void)
{

#ifdef MSOS_TYPE_LINUX
    pthread_mutex_unlock(&GFXFireInfoMutex);
#endif

}
#endif
static GFX_Result _GFX_RegisterAllCBFunc(void)
{
    GFX_Result      eRet = GFX_SUCCESS;

#ifdef GFX_UTOPIA2K
#else
    GFX_CB_ARGS     GFXCBInfo;
    memset(&GFXCBInfo, 0, sizeof(GFX_CB_ARGS));

    if(fpGFX_CBGetFont!=NULL)
    {
        GFXCBInfo.eGFX_CBType =E_CB_GETFONT;
        GFXCBInfo.pGFX_CBInfo =fpGFX_CBGetFont;
        if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_CB, (void*)&GFXCBInfo) != UTOPIA_STATUS_SUCCESS)
        {
            GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
            eRet = GFX_FAIL;
        }
    }
    if(fpGFX_CBGetBMP!=NULL)
    {
        GFXCBInfo.eGFX_CBType =E_CB_GETBMP;
        GFXCBInfo.pGFX_CBInfo =fpGFX_CBGetBMP;
        if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_CB, (void*)&GFXCBInfo) != UTOPIA_STATUS_SUCCESS)
        {
            GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
            eRet = GFX_FAIL;
        }
    }
#endif
    return eRet;
}
#ifndef MSOS_TYPE_OPTEE
/******************************************************************************/
///This function will register callback for GetFont function.
///
///@param *fpGFX_CB \b IN: Pointer to a callback function for GetFont.
///@return  GFX_SUCCESS if sucess.
/******************************************************************************/
GFX_Result MApi_GFX_RegisterGetFontCB(MS_PHY (*fpGFX_CB)(MS_S8, GFX_FontInfo*)  )
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(fpGFX_CB == NULL)
    {
        return GFX_INVALID_PARAMETERS;
    }

    fpGFX_CBGetFont = (void*)fpGFX_CB;
#ifdef GFX_UTOPIA2K
#else
    if(pGEInstance !=NULL)
    {
        GFX_CB_ARGS   GFXCBInfo;
        memset(&GFXCBInfo, 0, sizeof(GFX_CB_ARGS));

        GFXCBInfo.eGFX_CBType =E_CB_GETFONT;
        GFXCBInfo.pGFX_CBInfo =fpGFX_CB;
        if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_CB, (void*)&GFXCBInfo) != UTOPIA_STATUS_SUCCESS)
        {
            GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
            eRet = GFX_FAIL;
        }
    }
#endif
    return eRet;
}

/******************************************************************************/
///This function will register callback for GetBMP function.
///
///@param *fpGFX_CB \b IN: Pointer to a callback function for GetBMP.
///@return  GFX_SUCCESS if sucess.
/******************************************************************************/
GFX_Result MApi_GFX_RegisterGetBMPCB(MS_PHY (*fpGFX_CB)(MS_S16, GFX_BitmapInfo*)  )
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(fpGFX_CB == NULL)
    {
        return GFX_INVALID_PARAMETERS;
    }
    fpGFX_CBGetBMP = (void*)fpGFX_CB;
#ifdef GFX_UTOPIA2K
#else
    if(pGEInstance !=NULL)
    {
        GFX_CB_ARGS   GFXCBInfo;
        memset(&GFXCBInfo, 0, sizeof(GFX_CB_ARGS));

        GFXCBInfo.eGFX_CBType =E_CB_GETBMP;
        GFXCBInfo.pGFX_CBInfo =fpGFX_CB;
        if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_CB, (void*)&GFXCBInfo) != UTOPIA_STATUS_SUCCESS)
        {
            GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
            eRet = GFX_FAIL;
        }
    }
#endif
    return eRet;
}
#endif

/******************************************************************************/
///This function will fill a specific 32-bit pattern in a specific buffer.
///
///@param StrAddr \b IN: Start address of the buffer to be filled with specific value. 4-Byte alignment.
///@param length \b IN: Length of the value pattern to be filled. 4-Byte alignment
///@param ClearValue \b IN: 32-bit pattern to be filled in the specified space.
///@return  GFX_SUCCESS if sucess.
/******************************************************************************/
GFX_Result MApi_GFX_ClearFrameBufferByWord(MS_PHY StrAddr, MS_U32 length, MS_U32 ClearValue)
{
    GFX_MISC_ARGS   GFXInfo;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFX_ClearFrameBuffer   Info;
    memset(&Info, 0, sizeof(GFX_ClearFrameBuffer));

    Info.StrAddr = StrAddr;
    Info.length = length;
    Info.ClearValue = ClearValue;

    GFXInfo.eGFX_MISCType = E_MISC_CLEAR_FRAME_BY_WORLD;
    GFXInfo.pGFX_Info     = (void*)&Info;
    GFXInfo.u32Size       = sizeof(GFX_ClearFrameBuffer);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;

}
#ifndef MSOS_TYPE_OPTEE

#if 1
/******************************************************************************/
///This function will fill a specific 8-bit pattern in a specific buffer.
///
///@param StrAddr \b IN: Start address of the buffer to be filled with specific value. Byte alignment.
///@param length \b IN: Length of the value pattern to be filled. Byte alignment
///@param ClearValue \b IN: 8 bit pattern to be filled in the specified space.
///@return  GFX_SUCCESS if sucess.
/******************************************************************************/
GFX_Result MApi_GFX_ClearFrameBuffer(MS_PHY StrAddr, MS_U32 length, MS_U8 ClearValue)
{
    GFX_MISC_ARGS   GFXInfo;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFX_ClearFrameBuffer   Info;
    memset(&Info, 0, sizeof(GFX_ClearFrameBuffer));

    Info.StrAddr = StrAddr;
    Info.length = length;
    Info.ClearValue = ClearValue;

    GFXInfo.eGFX_MISCType = E_MISC_CLEAR_FRAME;
    GFXInfo.pGFX_Info     = (void*)&Info;
    GFXInfo.u32Size       = sizeof(GFX_ClearFrameBuffer);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;
}
#endif

/******************************************************************************/
///This function will draw a Rectangle Frame (Framework) on screen
///
///@param *pblock \b IN: Pointer to a structure containing the rectangle coordinates, size and color information
///@param *plinefmt \b IN: Pointer to a structure containing the border format (using bolder line)
///@return  GFX_SUCCESS if sucess.
/******************************************************************************/
void MApi_GFX_RectFrame(GFX_RectFillInfo *pblock, GFX_DrawLineInfo *plinefmt)
{
    GFX_DrawLineInfo pline;

    //GE_GET_ReSource();

    //printf("MDrv_GE_RectFrame \n");

    memset(&pline, 0, sizeof(GFX_DrawLineInfo));
    memcpy(&pline.colorRange.color_s, &pblock->colorRange.color_s, 4);
    memcpy(&pline.colorRange.color_e, &pblock->colorRange.color_e, 4);

    // Top line
#if 0
    clrline.x1 = pblock->x;
    clrline.x2 = pblock->x + pblock->width ;
    clrline.y1 =
    clrline.y2 = pblock->y;
    MDrv_GE_LineTo( &clrline, plinefmt );
#endif
    pline.x1 = pblock->dstBlock.x;
    pline.x2 = pblock->dstBlock.x + pblock->dstBlock.width ;
    pline.y1 =
    pline.y2 = pblock->dstBlock.y;
    pline.flag = 0;
    MApi_GFX_DrawLine(&pline);

    // Bottom line
#if 0
    clrline.y1 =
    clrline.y2 = pblock->y + pblock->height -1;
    MDrv_GE_LineTo( &clrline, plinefmt );
#endif

    pline.y1 = pblock->dstBlock.y + pblock->dstBlock.height -1;
    pline.y2 = pblock->dstBlock.y + pblock->dstBlock.height -1;
    MApi_GFX_DrawLine(&pline);

    // Left line
#if 0
    clrline.x2 = pblock->x;
    clrline.y1 = pblock->y;
    clrline.y2 = pblock->y + pblock->height ;
    MDrv_GE_LineTo( &clrline, plinefmt );
#endif

    pline.x2 = pblock->dstBlock.x;
    pline.y1 = pblock->dstBlock.y;
    pline.y2 = pblock->dstBlock.y + pblock->dstBlock.height;
    MApi_GFX_DrawLine(&pline);


    // Right line
#if 0
    clrline.x1 = pblock->x + pblock->width -1;
    clrline.x2 = clrline.x1;
    MDrv_GE_LineTo( &clrline, plinefmt );
#endif
    pline.x1 = pblock->dstBlock.x + pblock->dstBlock.width -1;
    pline.x2 = pline.x1;
    MApi_GFX_DrawLine(&pline);


    //GE_Free_ReSource();

}

//-------------------------------------------------------------------------------------------------
/// GFX text out function for drawing characters in one line.
/// The function invokes the callback registered through MApi_GFX_RegisterGetFontCB for restrieving font info.
/// @param fhandle \b IN: font handle
/// @param pindex \b IN: pointer of character index array
/// @param strwidth \b IN: character index length(1:ONE_BYTE_CHAR, 2: TWO_BYTE_CHAR)
/// @param pfmt \b IN: pointer to text out info
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_FONT_HANDLE - Invalid font handle
/// @return GFX_INVALID_PARAMETERS - Inavlid input parameters
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_TextOut(MS_S32 fhandle, MS_U8 *pindex, MS_U32 strwidth, GFX_TextOutInfo *pfmt)
{
    GFX_FontInfo tfontTable;
    GFX_Result  eRet = GFX_SUCCESS;
    MS_U16 i;

    if(fpGFX_CBGetFont != NULL)
        fpGFX_CBGetFont(fhandle, &tfontTable);
    else
        return GFX_INVALID_FONT_HANDLE;

    if (!tfontTable.inUsed)
    {
        return GFX_INVALID_FONT_HANDLE;
    }

    if(pindex==NULL || pfmt ==NULL)
    {
        return GFX_FAIL;
    }
    APIGFX_CHECK_INIT();

    GFX_TEXTOUT_ARGS   GFXTextArgs;
    GFX_TextOut_V2     GFXTextOut;

    memset(&GFXTextArgs, 0 , sizeof(GFX_TEXTOUT_ARGS));
    memset(&GFXTextOut , 0 ,sizeof(GFX_TextOut_V2));

    GFX_FireInfoLock();
    GFXTextOut.fHandle                =fhandle;

    if (strwidth == 2)
    {
        MS_U16 * pCharAr = (MS_U16 *)(&GFXTextOut.charArray[0]);
        MS_U16 * pUnicodeChar = (MS_U16 *)pindex;
        for (i=0; i<128; i++)
        {
            *(pCharAr + i) = *(pUnicodeChar + i);
            if (*(pUnicodeChar + i) == '\0')
            {
                break;
            }
        }
    }
    else
    {
        for (i=0; i<256; i++)
        {
            GFXTextOut.charArray[i] = *(pindex + i);
            if (*(pindex + i) == '\0')
            {
                break;
            }
        }
    }

    GFXTextOut.u32StrWidth            =strwidth;
    GFXTextOut.pTextInfo            =pfmt;
    GFXTextOut.pFonttable           =&tfontTable;
    GFXTextArgs.eGFX_TextType         =E_LOADTEXTOUT;
    GFXTextArgs.pGFX_TextOut        =(void*)&GFXTextOut;
    GFXTextArgs.u32Size               =sizeof(GFX_TextOut_V2);
    GFXTextOut.pFireInfo          = &GFXFireInfo;
    if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_FONT, (void*)&GFXTextArgs) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}




//-------------------------------------------------------------------------------------------------
/// Query  text display length
/// The function invokes the callback registered through MApi_GFX_RegisterGetFontCB for restrieving font info.
/// @param fhandle \b IN: font handle
/// @param pu8index \b IN: pointer of character index array
/// @param u32strwidth \b IN: character index length(1:ONE_BYTE_CHAR, 2: TWO_BYTE_CHAR)
/// @param pfmt \b IN: pointer to text out info
/// @param pu32DispLength \b OUT: pointer to display length.
/// @return GFX_SUCCESS - Success
/// @return GFX_INVALID_FONT_HANDLE - Invalid font handle
/// @return GFX_INVALID_PARAMETERS - Inavlid input parameters
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_QueryTextDispLength(MS_S32 fhandle, MS_U8 *pu8index, MS_U32 u32strwidth,
                                        GFX_TextOutInfo *pfmt, MS_U32* pu32DispLength)
{
    MS_U16* pu16TmpIndex;
    MS_U8* pu8TmpIndex;
    MS_U32 u32Length = 0;
    MS_S8 dis;

//    OSD_RESOURCE_FONT_INFO osd_fonttable;
    GFX_FontInfo fonttable;
    GFX_FontInfo* pFont = &fonttable;
    MS_VIRT ptr_addr;

    APIGFX_CHECK_INIT();

    //msAPI_OSD_RESOURCE_GetFontInfo(fhandle, &osd_fonttable);
    if(fpGFX_CBGetFont != NULL)
        fpGFX_CBGetFont(fhandle, &fonttable);
    else
        return GFX_INVALID_FONT_HANDLE;
#if 0
    fonttable.addr = osd_fonttable.glyph_addr;
    fonttable.fmt = osd_fonttable.fmt;
    fonttable.height = osd_fonttable.height;
    fonttable.len = osd_fonttable.char_glyph_bytes;
    fonttable.offset = osd_fonttable.char_glyph_bytes;
    fonttable.pBBox = (GFX_GlyphBbox*)osd_fonttable.bbox_addr;
    fonttable.pitch = osd_fonttable.pitch;
    fonttable.width = osd_fonttable.width;
    fonttable.inUsed = osd_fonttable.inUsed;
#endif
    if (!pFont->inUsed)
    {
        return GFX_INVALID_FONT_HANDLE;
    }
    if (pu8index == NULL)
    {
        return GFX_INVALID_PARAMETERS;
    }

    if(pfmt->flag & GFXFONT_FLAG_COMPACT)
    {
        dis = pfmt->dis;
    }
    else if (pfmt->flag & GFXFONT_FLAG_GAP)
    {
        dis = (0-pfmt->dis);
    }
    else
    {
        dis = 0;
    }


    if (u32strwidth == 1)
    {
        pu8TmpIndex = (MS_U8*)pu8index;
        while(*pu8TmpIndex != 0)
        {
            if (pfmt->flag & GFXFONT_FLAG_SCALE)
            {
                u32Length = u32Length + pfmt->dstblk.width - dis;
            }
            else
            {
                if ((pfmt->flag & GFXFONT_FLAG_VARWIDTH) &&
                    (pFont->pBBox != NULL))
                {
                    u32Length = u32Length + pFont->pBBox[*pu8TmpIndex].u8Width - dis;
                }
                else
                {
                    u32Length = u32Length + pFont->width - dis;
                }
            }
            pu8TmpIndex++;
        }
        *pu32DispLength = u32Length + dis;
    }
    else if (u32strwidth == 2)
    {
        ptr_addr = (MS_VIRT)pu8index;
        if(ptr_addr % 2 != 0)
        {
            GFX_ERR("%s: %d\n", __FUNCTION__ , __LINE__);
            return GFX_INVALID_PARAMETERS;
        }

        pu16TmpIndex = (MS_U16*)ptr_addr;
        while(*pu16TmpIndex != 0)
        {
            if (pfmt->flag & GFXFONT_FLAG_SCALE)
            {
                u32Length = u32Length + pfmt->dstblk.width - dis;
            }
            else
            {
                if ((pfmt->flag & GFXFONT_FLAG_VARWIDTH) &&
                    (pFont->pBBox != NULL))
                {
                    u32Length = u32Length + pFont->pBBox[*pu16TmpIndex].u8Width - dis;
                }
                else
                {
                    u32Length = u32Length + pFont->width - dis;
                }
            }
            pu16TmpIndex++;
        }
        *pu32DispLength = u32Length + dis;
    }
    else
    {
        *pu32DispLength = u32Length;
        return GFX_INVALID_PARAMETERS;
    }

    return GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// GFX text out one character without load font.
/// @param pChar \b IN: pointer to character information
/// @param pfmt \b IN: pointer to text out info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_CharacterOut(GFX_CharInfo*  pChar, GFX_TextOutInfo *pfmt)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(pChar==NULL || pfmt ==NULL)
    {
        return GFX_FAIL;
    }
    APIGFX_CHECK_INIT();
    GFX_TEXTOUT_ARGS       GFXTextArgs;
    GFX_CharacterOut       GFXCharacterout;

    memset(&GFXTextArgs, 0 , sizeof(GFX_TEXTOUT_ARGS));
    memset(&GFXCharacterout , 0 ,sizeof(GFX_CharacterOut));

    GFX_FireInfoLock();
    GFXCharacterout.pChar            =pChar;
    GFXCharacterout.pTextInfo        =pfmt;
    GFXTextArgs.eGFX_TextType      =E_CHARACTEROUT;
    GFXTextArgs.pGFX_TextOut        =(void*)&GFXCharacterout;
    GFXTextArgs.u32Size            =sizeof(GFX_CharacterOut);
    GFXCharacterout.pFireInfo       = &GFXFireInfo;
    if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_FONT, (void*)&GFXTextArgs) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GFX draw bitmap function
/// The function invokes the callback registered through MApi_GFX_RegisterGetBMPCB for restrieving BMP info.
/// @param handle \b IN: handle of bitmap
/// @param pbmpfmt \b IN: bitmap format
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_INVALID_BMP_HANDLE - Invalid bitmap handle
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_DrawBitmap(MS_S32 handle, GFX_DrawBmpInfo *pbmpfmt)
{
    GFX_Result  eRet = GFX_SUCCESS;
    GFX_BitmapInfo bmpinfo;
    GFX_BufferInfo srcinfo;

    if(pbmpfmt ==NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();
    GFX_BITMAP_ARGS    pGFXBitmap;
    GFX_Bitmap         pGFXBitmapInfo;

    memset(&pGFXBitmap, 0 , sizeof(GFX_BITMAP_ARGS));
    memset(&pGFXBitmapInfo , 0 ,sizeof(GFX_Bitmap));

    if(fpGFX_CBGetBMP != NULL)
        fpGFX_CBGetBMP(handle, &bmpinfo);
    else
        return GFX_INVALID_BMP_HANDLE;

    if (!bmpinfo.inUsed)
    {
        return GFX_INVALID_BMP_HANDLE;
    }

    srcinfo.u32Addr = bmpinfo.addr;
    srcinfo.u32Width = bmpinfo.width;
    srcinfo.u32Height = bmpinfo.height;
    srcinfo.u32Pitch = bmpinfo.pitch;
    srcinfo.u32ColorFmt = bmpinfo.fmt;

    MApi_GFX_SetSrcBufferInfo(&srcinfo,0);

    GFX_FireInfoLock();
    pGFXBitmapInfo.handle              =handle;
    pGFXBitmapInfo.pBmpInfo            =pbmpfmt;
    pGFXBitmap.pGFX_Bitmap            =(void*)&pGFXBitmapInfo;
    pGFXBitmap.u32Size             =sizeof(GFX_Bitmap);
    pGFXBitmapInfo.pFireInfo      = &GFXFireInfo;
    GFXFireInfo.eFireInfo |= GFX_DST_INFO;
    if(UtopiaIoctl(pGEInstance , MAPI_CMD_GFX_BITMAP, (void*)&pGFXBitmap) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }
    else
    {
        eRet = GFX_SUCCESS;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;

}
#endif

//-------------------------------------------------------------------------------------------------
/// Initial PE engine
/// @return  None
//-------------------------------------------------------------------------------------------------
void MApi_GFX_Init(GFX_Config *geConfig)
{
    GFX_INIT_ARGS            GFXInit;
    GFX_Init_Config            GFXInitInfo;

    memset(&GFXInit , 0 , sizeof(GFX_INIT_ARGS));
    GFXInitInfo.u32VCmdQSize    = geConfig->u32VCmdQSize;
    GFXInitInfo.u32VCmdQAddr    = geConfig->u32VCmdQAddr;
    GFXInitInfo.bIsHK           = geConfig->bIsHK;
    GFXInitInfo.bIsCompt        = geConfig->bIsCompt;

    GFXInit.pGFX_Init            = (void*)&GFXInitInfo;
    GFXInit.u32Size                = sizeof(GFX_Init_Config);

    CheckGFXInstanceOpen();
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_INIT, (void*)&GFXInit) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]    GFXIoctl Fail\n",__FUNCTION__, __LINE__);
    }
    _GFX_RegisterAllCBFunc();

    s32GEWrapperMutex = MsOS_CreateMutex(E_MSOS_FIFO, "GEWrapper_Mutex", MSOS_PROCESS_SHARED);
#ifdef  MSOS_TYPE_LINUX
    atexit(MApi_GE_Exit);
#endif

}

#ifndef MSOS_TYPE_OPTEE
//-------------------------------------------------------------------------------------------------
/// Set GFX Engine dither
/// @param  enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDither(MS_BOOL enable)
{
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.bDither = enable;
    GFXFireInfo.eFireInfo |= GFX_DITHER_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX Engine dither
/// @param  enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetOnePixelMode(MS_BOOL enable)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 , sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_ONEPIXELMODE;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&enable;
    GFXSetConfig.u32Size              =sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set PE source color key
/// @param enable   \b IN: true/false\n
///                     When enable is FALSE, do not care the other parameters.\n
/// @param opMode   \b IN: source color key mode
///                      The alpha channel does not matter\n
/// @param fmt      \b IN: source color key format
/// @param ps_color \b IN: pointer of source color key start (GFX_RgbColor or GFX_BlinkData depend on color format).\n
///                        For all RGB color, the color set as the ARGB8888 format.\n
///                        Each color component need to shift to high bit.\n
///                        Use ARGB1555 as the example, the source color key as the following:\n
///                        ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
///                        *ps_color --> A0000000RRRRR000GGGGG000BBBBB000   (every character represents one bit)\n\n
///                        For GFX_FMT_I8 format, the index set to b component (ps_color->b = b).\n
///                        For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
/// @param pe_color \b IN: pointer of source color key end (GFX_RgbColor or GFX_BlinkData depend on color format).\n
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetSrcColorKey(MS_BOOL enable,
                                   GFX_ColorKeyMode opMode,
                                   GFX_Buffer_Format fmt,
                                   void *ps_color,
                                   void *pe_color)
{
    GFX_Result  eRet = GFX_SUCCESS;
    MS_U32 *pS_color = (MS_U32 *)ps_color;
    MS_U32 *pE_color = (MS_U32 *)pe_color;

    if(ps_color ==NULL || pe_color ==NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetSrcColorKey.bEnable               =enable;
    GFXFireInfo.GFXSetSrcColorKey.eOpMode               =opMode;
    GFXFireInfo.GFXSetSrcColorKey.eFmt                  =fmt;
    GFXFireInfo.GFXSetSrcColorKey.S_color               =*pS_color;
    GFXFireInfo.GFXSetSrcColorKey.E_color               =*pE_color;
    GFXFireInfo.eFireInfo |= GFX_SRC_CLRKEY_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set PE destination color key
/// @param enable   \b IN: true/false\n
///                     When enable is FALSE, do not care the other parameters.\n
/// @param opMode   \b IN: destination color key mode\n
///                      The alpha channel does not matter\n
/// @param fmt      \b IN: destination color key format
/// @param ps_color \b IN: pointer of destination color key start (GFX_RgbColor or GFX_BlinkData depend on color format).\n
///                        For all RGB color, the color set as the ARGB8888 format.\n
///                        Each color component need to shift to high bit.\n
///                        Use ARGB1555 as the example, the source color key as the following:\n
///                        ARGB1555  --> ARRRRRGGGGGBBBBB                   (every character represents one bit)\n
///                        *ps_color --> A0000000RRRRR000GGGGG000BBBBBB000  (every character represents one bit)\n\n
///                        For GFX_FMT_I8 format, the index set to b component (ps_color->b = b).\n
///                        For GFX_FMT_1BAAFGBG123433 foramt, the foramt set as the GFX_BlinkData.\n
/// @param pe_color \b IN: pointer of destination color key end (GFX_RgbColor or GFX_BlinkData depend on color format).\n
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDstColorKey(MS_BOOL enable,
                                   GFX_ColorKeyMode opMode,
                                   GFX_Buffer_Format fmt,
                                   void *ps_color,
                                   void *pe_color)
{
    GFX_Result  eRet = GFX_SUCCESS;
    MS_U32 *pS_color = (MS_U32 *)ps_color;
    MS_U32 *pE_color = (MS_U32 *)pe_color;

    if(ps_color ==NULL || pe_color ==NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetDstColorKey.bEnable               =enable;
    GFXFireInfo.GFXSetDstColorKey.eOpMode               =opMode;
    GFXFireInfo.GFXSetDstColorKey.eFmt                  =fmt;
    GFXFireInfo.GFXSetDstColorKey.S_color               =*pS_color;
    GFXFireInfo.GFXSetDstColorKey.E_color               =*pE_color;
    GFXFireInfo.eFireInfo |= GFX_DST_CLRKEY_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set PE intensity : total 16 color Palette in PE
/// @param id \b IN: id of intensity
/// @param fmt \b IN: intensity color format (GFX_FMT_ARGB8888 , GFX_FMT_1BAAFGBG123433 or GFX_FMT_I8)
/// @param pColor \b IN: pointer of intensity (GFX_RgbColor or GFX_BlinkData depend on color format)
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_INVALID_INTENSITY_ID - Inavlid index (id >= 16)
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetIntensity(MS_U32 id, GFX_Buffer_Format fmt, MS_U32 *pColor)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(pColor ==NULL)
    {
        return GFX_FAIL;
    }
    APIGFX_CHECK_INIT();

    GFX_ABL_ARGS            GFXSetABL_ARGS;
    GFX_Set_Intensity       GFXSetIntensity;

    memset(&GFXSetABL_ARGS, 0 , sizeof(GFX_ABL_ARGS));
    memset(&GFXSetIntensity , 0 ,sizeof(GFX_Set_Intensity));

    GFXSetABL_ARGS.eGFX_SetABL              =E_GFX_SET_INTENSITY;
    GFXSetABL_ARGS.pGFX_ABL                    =(void*)&GFXSetIntensity;
    GFXSetABL_ARGS.u32Size                    =sizeof(GFX_Set_Intensity);

    GFXSetIntensity.u32Id         = id;
    GFXSetIntensity.eFmt          = fmt;
    GFXSetIntensity.pColor        = pColor;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_ABL, (void*)&GFXSetABL_ARGS) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Get GFX intensity : total 16 color Palette in GFX for I1/I2/I4 mode.
/// @param idx \b IN: id of intensity
/// @param color \b IN: Pointer of start of intensity point to
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_INVALID_INTENSITY_ID - Inavlid index (id >= 16)
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_GetIntensity(MS_U32 idx, MS_U32 *color)
{
    GFX_Result eRet = GFX_SUCCESS;

    if(color == NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();
    GFX_GETINFO_ARGS   GFXGetInfo;
    GFX_Get_Intensity  GFXGetIntensity;

    memset(&GFXGetInfo, 0 , sizeof(GFX_GETINFO_ARGS));
    memset(&GFXGetIntensity , 0 ,sizeof(GFX_Get_Intensity));

    GFXGetInfo.eGFX_GetConfig       =E_GFX_GET_INTENSITY;
    GFXGetInfo.pGFX_GetInfo         =(void*)&GFXGetIntensity;
    GFXGetInfo.u32Size              =sizeof(GFX_Get_Intensity);

    GFXGetIntensity.u32Id           =idx;
    GFXGetIntensity.pColor          =color;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_INFO, (void*)&GFXGetInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set GFX raster operation
/// @param enable \b IN: true/false
/// @param eRopMode \b IN: raster operation
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetROP2(MS_BOOL enable, GFX_ROP2_Op eRopMode)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_ABL_ARGS            GFXSetABL_ARGS;
    GFX_Set_ROP             GFXSetROP;

    memset(&GFXSetABL_ARGS, 0 , sizeof(GFX_ABL_ARGS));
    memset(&GFXSetROP , 0 ,sizeof(GFX_Set_ROP));

    GFXSetABL_ARGS.eGFX_SetABL               =E_GFX_SET_ROP;
    GFXSetABL_ARGS.pGFX_ABL                  =(void*)&GFXSetROP;
    GFXSetABL_ARGS.u32Size                   =sizeof(GFX_Set_ROP);

    GFXSetROP.bEnable                   =enable;
    GFXSetROP.eRopMode                  =eRopMode;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_ABL, (void*)&GFXSetABL_ARGS) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// GFX draw line
/// @param pline \b IN: pointer to line info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_DrawLine(GFX_DrawLineInfo *pline)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(pline == NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();
    GFX_LINEDRAW_ARGS   GFXDrawLine;
    GFX_Set_DrawLineInfo    GFXDrawLineInfo;

    memset(&GFXDrawLine, 0 , sizeof(GFX_LINEDRAW_ARGS));
    memset(&GFXDrawLineInfo, 0 ,sizeof(GFX_Set_DrawLineInfo));

    GFX_FireInfoLock();
    GFXDrawLineInfo.pFireInfo                   = &GFXFireInfo;
    GFXDrawLineInfo.pDrawLineInfo               = pline;
    GFXDrawLine.pLineInfo                          =(void*)&GFXDrawLineInfo;
    GFXDrawLine.u32Size                            =sizeof(GFX_Set_DrawLineInfo);
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_LINEDRAW, (void*)&GFXDrawLine) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GFX rectangle fill
/// @param pfillblock \b IN: pointer to block info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_RectFill(GFX_RectFillInfo *pfillblock)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(pfillblock);
    APIGFX_CHECK_INIT();

    GFX_RECTFILL_ARGS       GFXRectFill;
    GFX_Set_RectFillInfo    GFXRectFillInfo;

    memset(&GFXRectFill, 0 , sizeof(GFX_RECTFILL_ARGS));
    memset(&GFXRectFillInfo, 0 ,sizeof(GFX_Set_RectFillInfo));

    GFX_FireInfoLock();
    GFXRectFillInfo.pFireInfo                   = &GFXFireInfo;
    GFXRectFillInfo.pRectFillInfo                 = pfillblock;
    GFXRectFill.pFillBlock                      =(void*)&GFXRectFillInfo;
    GFXRectFill.u32Size                            =sizeof(GFX_Set_RectFillInfo);
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_RECTFILL, (void*)&GFXRectFill) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GFX triangle fill
/// @param ptriblock \b IN: pointer to block info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_TriFill(GFX_TriFillInfo *ptriblock)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(ptriblock);
    APIGFX_CHECK_INIT();

    GFX_TRIFILL_ARGS       GFXTriFill;
    GFX_Set_TriFillInfo    GFXTriFillInfo;

    memset(&GFXTriFill, 0 , sizeof(GFX_TRIFILL_ARGS));
    memset(&GFXTriFillInfo, 0 ,sizeof(GFX_Set_TriFillInfo));

    GFX_FireInfoLock();
    GFXTriFillInfo.pFireInfo                    = &GFXFireInfo;
    GFXTriFillInfo.pTriFillInfo                 = ptriblock;
    GFXTriFill.pFillBlock                      =(void*)&GFXTriFillInfo;
    GFXTriFill.u32Size                            =sizeof(GFX_Set_TriFillInfo);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TRIFILL, (void*)&GFXTriFill) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GFX span fill
/// @param pspanblock \b IN: pointer to block info
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SpanFill(GFX_SpanFillInfo *pspanblock)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(pspanblock);
    APIGFX_CHECK_INIT();

    GFX_SPANFILL_ARGS       GFXSpanFill;
    GFX_Set_SpanFillInfo    GFXSpanFillInfo;

    memset(&GFXSpanFill, 0 , sizeof(GFX_SPANFILL_ARGS));
    memset(&GFXSpanFillInfo, 0 ,sizeof(GFX_Set_SpanFillInfo));

    GFX_FireInfoLock();
    GFXSpanFillInfo.pFireInfo                   = &GFXFireInfo;
    GFXSpanFillInfo.pSpanFillInfo                 = pspanblock;
    GFXSpanFill.pFillBlock                      =(void*)&GFXSpanFillInfo;
    GFXSpanFill.u32Size                            =sizeof(GFX_Set_SpanFillInfo);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SPANFILL, (void*)&GFXSpanFill) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GFX trapezoid fill
/// @param pGfxFillInfo \b IN: pointer to GFX_RectFillInfo which is union with Trapezoid struct.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_TrapezoidFill(GFX_RectFillInfo *pGfxFillInfo)
{
#if 0
    GE_Trapezoid geTrapezoid;
    MS_U32 u32ColorS, u32ColorE;
    MS_U32  flags = 0;

    APIGFX_CHECK_INIT();

    //Prepare Fill Block:
    geTrapezoid.u16X0 = pGfxFillInfo->dstTrapezoidBlk.u16X0;
    geTrapezoid.u16Y0 = pGfxFillInfo->dstTrapezoidBlk.u16Y0;
    geTrapezoid.u16X1 = pGfxFillInfo->dstTrapezoidBlk.u16X1;
    geTrapezoid.u16Y1 = pGfxFillInfo->dstTrapezoidBlk.u16Y1;
    geTrapezoid.u16DeltaTop = pGfxFillInfo->dstTrapezoidBlk.u16DeltaTop;
    geTrapezoid.u16DeltaBottom = pGfxFillInfo->dstTrapezoidBlk.u16DeltaBottom;

    //Prepare Color:
    u32ColorS = (pGfxFillInfo->colorRange.color_s.a<<24) | (pGfxFillInfo->colorRange.color_s.r<<16)
                    | (pGfxFillInfo->colorRange.color_s.g<<8) | (pGfxFillInfo->colorRange.color_s.b);
    u32ColorE = (pGfxFillInfo->colorRange.color_e.a<<24) | (pGfxFillInfo->colorRange.color_e.r<<16)
                    | (pGfxFillInfo->colorRange.color_e.g<<8) | (pGfxFillInfo->colorRange.color_e.b);

    if(GFX_FMT_1ABFGBG12355 == pGfxFillInfo->fmt)
    {
        u32ColorS = MAKE_PRIMITIVE_COLOR_12355(pGfxFillInfo->blkDataRange.blink_data_s.background, pGfxFillInfo->blkDataRange.blink_data_s.foreground,
                        pGfxFillInfo->blkDataRange.blink_data_s.Bits.Blink, pGfxFillInfo->blkDataRange.blink_data_s.Bits.Alpha);
        u32ColorE = MAKE_PRIMITIVE_COLOR_12355(pGfxFillInfo->blkDataRange.blink_data_e.background, pGfxFillInfo->blkDataRange.blink_data_e.foreground,
                        pGfxFillInfo->blkDataRange.blink_data_e.Bits.Blink, pGfxFillInfo->blkDataRange.blink_data_e.Bits.Alpha);
        }

    if(GFX_FMT_FABAFGBG2266 == pGfxFillInfo->fmt)
    {
        u32ColorS = MAKE_PRIMITIVE_COLOR_2266(pGfxFillInfo->blkDataRange.blink_data_s.background, pGfxFillInfo->blkDataRange.blink_data_s.foreground,
                        pGfxFillInfo->blkDataRange.blink_data_s.Bits3.Ba, pGfxFillInfo->blkDataRange.blink_data_s.Bits3.Fa);
        u32ColorE = MAKE_PRIMITIVE_COLOR_2266(pGfxFillInfo->blkDataRange.blink_data_e.background, pGfxFillInfo->blkDataRange.blink_data_e.foreground,
                        pGfxFillInfo->blkDataRange.blink_data_e.Bits3.Ba, pGfxFillInfo->blkDataRange.blink_data_e.Bits3.Fa);
    }

    flags |= (pGfxFillInfo->flag & GFXRECT_FLAG_COLOR_GRADIENT_X) ? E_GE_FLAG_TRAPEZOID_GRADIENT_X : 0;
    flags |= (pGfxFillInfo->flag & GFXRECT_FLAG_COLOR_GRADIENT_Y) ? E_GE_FLAG_TRAPEZOID_GRADIENT_Y : 0;
    flags |= (pGfxFillInfo->flag & GFXRECT_FLAG_TRAPE_DIRECTION_X) ? E_GE_FLAG_TRAPEZOID_X : 0;
    flags |= (pGfxFillInfo->flag & GFXRECT_FLAG_TRAPE_DIRECTION_Y) ? E_GE_FLAG_TRAPEZOID_Y : 0;

    if((flags & E_GE_FLAG_TRAPEZOID_X) && (flags & E_GE_FLAG_TRAPEZOID_Y))
    {
        return GFX_INVALID_PARAMETERS;
    }

    return (GFX_Result) MDrv_GE_FillTrapezoid(g_apiGFXLocal.g_pGEContext, &geTrapezoid, u32ColorS, u32ColorE, flags);
#endif
    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Enable Trapezoid Antialiasing
/// @param bEnable \b IN: Enable or Not
/// @return GFX_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_EnableTrapezoidAA(MS_BOOL bEnable)
{
#if 0

    APIGFX_CHECK_INIT();

    return (GFX_Result) MDrv_GE_EnableTrapezoidAA(g_apiGFXLocal.g_pGEContext, bEnable);
#endif
    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Enable Trapezoid  Subpixel Correction
/// @param bEnable \b IN: Enable or Not
/// @return GFX_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_EnableTrapSubPixCorr(MS_BOOL bEnable)
{
#if 0
    APIGFX_CHECK_INIT();

    return (GFX_Result) MDrv_GE_EnableTrapSubPixCorr(g_apiGFXLocal.g_pGEContext, bEnable);
#endif
    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX clipping window
/// @param v0 \b IN: left-top position
/// @param v1 \b IN: right-down position
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetClip(GFX_Point* v0, GFX_Point* v1)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if((v0==NULL) || (v1==NULL))
    {
        return GFX_FAIL;
    }
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetClip.V0.x                     =v0->x;
    GFXFireInfo.GFXSetClip.V0.y                     =v0->y;
    GFXFireInfo.GFXSetClip.V1.x                     =v1->x;
    GFXFireInfo.GFXSetClip.V1.y                     =v1->y;
    GFXFireInfo.eFireInfo |= GFX_CLIP_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Get GFX clipping window
/// @param v0 \b IN: left-top position
/// @param v1 \b IN: right-down position
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_GetClip(GFX_Point* v0, GFX_Point* v1)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(v0 ==NULL || v1==NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    v0->x = GFXFireInfo.GFXSetClip.V0.x;
    v0->y = GFXFireInfo.GFXSetClip.V0.y;
    v1->x = GFXFireInfo.GFXSetClip.V1.x;
    v1->y = GFXFireInfo.GFXSetClip.V1.y;
    GFX_FireInfoUnLock();

    return eRet;
}



//-------------------------------------------------------------------------------------------------
/// Set GFX alpha source
/// @param eMode \b IN: alpha source come from , this indicate alpha channel output source
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetAlphaSrcFrom(GFX_AlphaSrcFrom eMode)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetABL.eDb_abl                   = eMode;
    GFXFireInfo.eFireInfo |= GFX_ABL_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX alpha compare OP
/// @param enable \b IN: true: enable alpha compare, false: disable.
/// @param eMode \b IN: alpha source come from MIN/MAX compare between source/dst,
///                     this indicate alpha channel output source
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetAlphaCmp(MS_BOOL enable, GFX_ACmpOp eMode)
{
    GFX_Result  eRet = GFX_SUCCESS;
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetAlphaCmp.enable           = enable;
    GFXFireInfo.GFXSetAlphaCmp.eMode            = eMode;
    GFXFireInfo.eFireInfo |= GFX_ALPHA_CMP_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}
#endif

//-------------------------------------------------------------------------------------------------
/// Begin GFX Engine drawing, this function should be called before all PE drawing function,
/// and it will lock PE engine resource, reset all PE register and static variable.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_BeginDraw(void)
{
    GFX_MISC_ARGS   GFXInfo;
    MS_U32 value = 0;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_BEGINE_DRAW;
    GFXInfo.pGFX_Info     = (void*)&value;
    GFXInfo.u32Size       = sizeof(GFX_MISC_ARGS);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return (GFX_Result) GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// End GFX engine drawing (pair with MApi_GFX_BeginDraw), this function should be called after
/// all PE drawing function. And it will release PE engine resource.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_EndDraw(void)
{
    GFX_MISC_ARGS   GFXInfo;
    MS_U32 value = 0;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_END_DRAW;
    GFXInfo.pGFX_Info     = (void*)&value;
    GFXInfo.u32Size       = sizeof(GFX_MISC_ARGS);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }

    return (GFX_Result) GFX_SUCCESS;
}

#ifndef MSOS_TYPE_OPTEE
//-------------------------------------------------------------------------------------------------
/// Reset GFX line pattern
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_Line_Pattern_Reset(void)
{
    GFX_MISC_ARGS   GFXInfo;
    MS_U32 value = 0;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_LINE_PATTERN_RESET;
    GFXInfo.pGFX_Info     = (void*)&value;
    GFXInfo.u32Size       = sizeof(GFX_MISC_ARGS);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX line pattern
/// @param enable \b IN: true/false
/// @param linePattern \b IN: p0-0x3F one bit represent draw(1) or not draw(0)
/// @param repeatFactor \b IN: 0 : repeat once, 1 : repeat twice, 2: repeat 3, 3: repeat 4
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_Set_Line_Pattern(MS_BOOL enable, MS_U8 linePattern, MS_U8 repeatFactor)
{
    GFX_MISC_ARGS   GFXInfo;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFX_SetLinePattern   Info;
    memset(&Info, 0, sizeof(GFX_SetLinePattern));

    Info.enable = enable;
    Info.linePattern = linePattern;
    Info.repeatFactor = repeatFactor;

    GFXInfo.eGFX_MISCType = E_MISC_LINE_PATTERN;
    GFXInfo.pGFX_Info     = (void*)&Info;
    GFXInfo.u32Size       = sizeof(GFX_SetLinePattern);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX italic
/// @param enable \b IN: true/false
/// @param ini_line \b IN: initial line : default is 0, indicate which line start to get italic effect
/// @param ini_dis \b IN: initial distance : default is 0, indicate which pixel start to get italic effect
/// @param delta \b IN: italic delta 0-0x1f
///                     - D[4]: 0/1  left/right italic
///                     - D[3]-D[0] : delta value of italic
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @note
/// The italic process can't perform with rotate process or mirror process.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetItalic(MS_BOOL enable,MS_U8 ini_line, MS_U8 ini_dis, MS_U8 delta)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;
    GFX_Set_Italic         GFXSetItalic;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));
    memset(&GFXSetItalic, 0 ,sizeof(GFX_Set_Italic));

    GFXSetConfig.eGFX_SetConfig         =E_GFX_SET_ITALIC;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSetItalic;
    GFXSetConfig.u32Size                =sizeof(GFX_Set_Italic);

    GFXSetItalic.bEnable          =enable;
    GFXSetItalic.u8Ini_line       =ini_line;
    GFXSetItalic.u8Ini_dis        =ini_dis;
    GFXSetItalic.u8Delta          =delta;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX rotate
/// @param angle \b IN: rotate angle
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @note
/// The rotate process can't perform with italic process.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetRotate(GFX_RotateAngle angle)
{
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetAngle                        = angle;
    GFXFireInfo.eFireInfo |= GFX_ROTATE_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
// Description:
// Arguments:   eMode : ABL_FROM_CONST, ABL_FROM_ASRC, ABL_FROM_ADST
//              blendcoef : COEF_ONE,  COEF_CONST,   COEF_ASRC,   COEF_ADST
//                          COEF_ZERO, COEF_1_CONST, COEF_1_ASRC, COEF_1_ADST
//              blendfactor : value : [0,0xff]
// Return:      NONE
//
// Notes:       if any
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Set PE alpha blending. Dst = A * Src + (1 - A) Dst
/// @param blendcoef       \b IN: alpha source from
/// @param u8ConstantAlpha \b IN: Contant alpha when blendcoef is equal to COEF_CONST
///                               or COEF_1_CONST.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 u8ConstantAlpha)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetABL.eABLCoef                  = blendcoef;
    GFXFireInfo.GFXSetABL.u8Alpha_Const             = u8ConstantAlpha;
#ifndef  MSOS_TYPE_NOS
    GFXFireInfo.GFXSetDFB.sRGBColor.a               = u8ConstantAlpha;
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
#endif
    GFXFireInfo.eFireInfo |= GFX_ABL_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Enable GFX alpha blending
/// @param enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_EnableAlphaBlending(MS_BOOL enable)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetABL.bEnable                   = enable;
    GFXFireInfo.eFireInfo |= GFX_ABL_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}


//-------------------------------------------------------------------------------------------------
/// Query GFX supported DFB blending functions/flags.
/// @param pU16SupportedBldFlags \b OUT: DFB Blending Functions/Flags supported by GFX.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_QueryDFBBldCaps(MS_U16 *pU16SupportedBldFlags)
{
    GFX_Result gfxResult=GFX_FAIL;
    gfxResult = MApi_GFX_GetGECaps(E_GFX_CAP_DFB_BLENDING,(MS_U32*)(void*)pU16SupportedBldFlags,sizeof(MS_U16));
    return gfxResult;
}

//-------------------------------------------------------------------------------------------------
/// Enable GFX DFB blending
/// @param enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_EnableDFBBlending(MS_BOOL enable)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetDFB.bEnable                   =enable;
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
    GFX_FireInfoUnLock();
    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set GE DFB blending Flags.
/// @param u16DFBBldFlag       \b IN: DFB Blending Flags. The Flags will be
///                                   The combination of Flags
///                                   [GFX_DFB_BLD_FLAG_COLORALPHA, GFX_DFB_BLD_FLAG_ALPHACHANNEL,
///                                    GFX_DFB_BLD_FLAG_COLORIZE, GFX_DFB_BLD_FLAG_SRCPREMUL,
///                                    GFX_DFB_BLD_FLAG_SRCPREMULCOL, GFX_DFB_BLD_FLAG_DSTPREMUL,
///                                    GFX_DFB_BLD_FLAG_XOR, GFX_DFB_BLD_FLAG_DEMULTIPLY,
///                                    GFX_DFB_BLD_FLAG_SRCALPHAMASK, GFX_DFB_BLD_FLAG_SRCCOLORMASK]
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDFBBldFlags(MS_U16 u16DFBBldFlags)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetDFB.u16DFBBldFlags            =u16DFBBldFlags;
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX DFB blending Functions/Operations.
/// @param gfxSrcBldOP \b IN: source blending op
/// @param gfxDstBldOP \b IN: dst blending op
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDFBBldOP(GFX_DFBBldOP gfxSrcBldOP, GFX_DFBBldOP gfxDstBldOP)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetDFB.eSrcBldOP                     =gfxSrcBldOP;
    GFXFireInfo.GFXSetDFB.eDstBldOP                     =gfxDstBldOP;
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}


//-------------------------------------------------------------------------------------------------
/// Set GFX DFB blending const color.
/// @param u32ConstColor \b IN: DFB Blending constant color
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDFBBldConstColor(GFX_RgbColor gfxRgbColor)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    memcpy(&(GFXFireInfo.GFXSetDFB.sRGBColor), &gfxRgbColor, sizeof(GFX_RgbColor));
#ifndef  MSOS_TYPE_NOS
    GFXFireInfo.GFXSetABL.u8Alpha_Const             = gfxRgbColor.a;
    GFXFireInfo.eFireInfo |= GFX_ABL_INFO;
#endif
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set GFX DFB blending source color mask.
/// @param u32ConstColor \b IN: DFB Blending constant color
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDFBBldSrcColorMask(GFX_RgbColor gfxRgbColor)
{
#if 0
    GE_RgbColor geRgbColor;

    APIGFX_CHECK_INIT();

    geRgbColor.a = gfxRgbColor.a;
    geRgbColor.r = gfxRgbColor.r;
    geRgbColor.g = gfxRgbColor.g;
    geRgbColor.b = gfxRgbColor.b;

    return (GFX_Result) MDrv_GE_SetDFBBldSrcColorMask(g_apiGFXLocal.g_pGEContext, geRgbColor);
#endif
    return GFX_SUCCESS;
}


/******************************************************************************/
///Disable/Enable and set Alpha Blending parameters.
///
///Alpha blending function will mix the source color and destination color.
///The alpha constant are applied to determine the ratio of source color and destination color.
///The operation mode will depends on the source/destination coefficient.
///
///@param enable \b IN :
///-  0: Disable
///-  1: Enable
///@param coef \b IN  Source coefficient for blending. (source blending mode)
///@param db_abl \b IN  Source of Destination Alpha.
///@param abl_const \b IN  Alpha Blending constant
///@par Function Actions:
/******************************************************************************/
GFX_Result MApi_GFX_SetAlpha(MS_BOOL enable, GFX_BlendCoef coef, GFX_AlphaSrcFrom db_abl, MS_U8 abl_const)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetABL.bEnable        = enable;
    GFXFireInfo.GFXSetABL.eABLCoef       = coef;
    GFXFireInfo.GFXSetABL.eDb_abl        = db_abl;
    GFXFireInfo.GFXSetABL.u8Alpha_Const  = abl_const;
#ifndef  MSOS_TYPE_NOS
    GFXFireInfo.GFXSetDFB.sRGBColor.a    = abl_const;
    GFXFireInfo.eFireInfo |= GFX_DFB_INFO;
#endif
    GFXFireInfo.eFireInfo |= GFX_ABL_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Enable GFX mirror
/// @param isMirrorX \b IN: true/false
/// @param isMirrorY \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @note
/// The mirror process can't perform on the source format is GFX_FMT_I1, GFX_FMT_I2 or GFX_FMT_I4.
/// The mirror process can't perform with italic process.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetMirror(MS_BOOL isMirrorX, MS_BOOL isMirrorY)
{
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetMirror.bMirrorX                 =isMirrorX;
    GFXFireInfo.GFXSetMirror.bMirrorY                 =isMirrorY;
    GFXFireInfo.eFireInfo |= GFX_SRC_MIRROR_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Enable GFX destination mirror
/// @param isMirrorX \b IN: true/false
/// @param isMirrorY \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @note
/// The mirror process can't perform on the source format is GFX_FMT_I1, GFX_FMT_I2 or GFX_FMT_I4.
/// The mirror process can't perform with italic process.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDstMirror(MS_BOOL isMirrorX, MS_BOOL isMirrorY)
{
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetDstMirror.bMirrorX                 =isMirrorX;
    GFXFireInfo.GFXSetDstMirror.bMirrorY                 =isMirrorY;
    GFXFireInfo.eFireInfo |= GFX_DST_MIRROR_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Enable GFX NearestMode
/// @param enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetNearestMode(MS_BOOL enable)
{
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.bNearest = enable;
    GFXFireInfo.eFireInfo |= GFX_NEAREST_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// Configure GFX Patch mode
/// @param enable \b IN: true/false
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetPatchMode(MS_BOOL enable)
{
    GFX_MISC_ARGS   GFXInfo;

    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));
    GFXInfo.eGFX_MISCType = E_MISC_PATCH_MODE;
    GFXInfo.pGFX_Info     = (void*)&enable;
    GFXInfo.u32Size       = sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;
}

/******************************************************************************/
///Set GFX TLB
/// @param  TLBmode                     \b IN: TLB mode
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetTLBMode(EN_GFX_TLB_Mode TLBmode)
{
    GFX_Result  eRet = GFX_SUCCESS;
    MS_U32 value = 0;
    APIGFX_CHECK_INIT();

    GFX_TLB_ARGS     GFXSetTLBConfig;

    memset(&GFXSetTLBConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetTLBConfig.en_cmd           =E_GFX_TLB_SET_MODE;
    GFXSetTLBConfig.psetting         =(void*)&value;
    GFXSetTLBConfig.TLB_mode         =TLBmode;
    GFXSetTLBConfig.u32Size          =sizeof(GFX_TLB_ARGS);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TLB, (void*)&GFXSetTLBConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Set GFX TLB
///@param  u32TLBSrcMiuAddr                     \b IN: TLB Source base address
///@param  u32TLBDstMiuAddr                     \b IN: TLB Destination base address
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetTLBBaseADDR(MS_PHY phyTLBSrcMiuAddr,MS_PHY phyTLBDstMiuAddr)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_TLB_ARGS             GFXTLBARGS;
    GFX_SetTLB_Addr          GFXSetTLBAddr;

    memset(&GFXTLBARGS, 0 ,sizeof(GFX_TLB_ARGS));
    memset(&GFXSetTLBAddr, 0 ,sizeof(GFX_SetTLB_Addr));

    GFXTLBARGS.en_cmd           =E_GFX_TLB_SET_ADDR;
    GFXTLBARGS.psetting         =(void*)&GFXSetTLBAddr;
    GFXTLBARGS.u32Size          =sizeof(GFX_SetTLB_Addr);

    GFXSetTLBAddr.tlb_src_addr   =phyTLBSrcMiuAddr;
    GFXSetTLBAddr.tlb_dst_addr   =phyTLBDstMiuAddr;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TLB, (void*)&GFXTLBARGS) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Set GFX TLB
///@param  enable                     \b IN: TLB Flush Enable
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetTLBFlushTable(MS_BOOL enable)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_TLB_ARGS             GFXTLBARGS;

    memset(&GFXTLBARGS, 0 ,sizeof(GFX_TLB_ARGS));

    GFXTLBARGS.en_cmd           =E_GFX_TLB_FLUSH_TABLE;
    GFXTLBARGS.psetting         =(void*)&enable;
    GFXTLBARGS.u32Size          =sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TLB, (void*)&GFXTLBARGS) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Set GFX TLB
///@param  tag                     \b IN: TLB TAG
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetTLBTag(MS_U16 tag)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_TLB_ARGS             GFXTLBARGS;

    memset(&GFXTLBARGS, 0 ,sizeof(GFX_TLB_ARGS));

    GFXTLBARGS.en_cmd           =E_GFX_TLB_TAG;
    GFXTLBARGS.psetting         =(void*)&tag;
    GFXTLBARGS.u32Size          =sizeof(MS_U16);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TLB, (void*)&GFXTLBARGS) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX source buffer info
/// @param bufInfo \b IN: buffer handle
/// @param offsetofByte \b IN: start offset (should be 128 bit aligned)
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_NON_ALIGN_PITCH - The pitch is not 16 bytes alignment
/// @return GFX_Result_NON_ALIGN_ADDRESS - The address is not 16 bytes alignment
/// @note
/// The buffer start address must be 128 bits alignment.
/// In GFX_FMT_I1, GFX_FMT_I2 and GFX_FMT_I4 format, the pitch must be 8 bits alignment.
/// In other format, the pitch must be 128 bits alignment.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetSrcBufferInfo(PGFX_BufferInfo bufInfo, MS_U32 offsetofByte)
{
    GFX_Result    eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(bufInfo);
    APIGFX_CHECK_INIT();

    //Utopia2 not support this function. Only stroed in wrapper.
    GFX_FireInfoLock();
    memcpy(&(GFXFireInfo.SrcbufInfo), bufInfo, sizeof(GFX_BufferInfo));
    GFXFireInfo.u32SrcOffsetofByte          =offsetofByte;
    GFXFireInfo.eFireInfo |= GFX_SRC_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}
//-------------------------------------------------------------------------------------------------
/// Set GFX destination buffer info
/// @param bufInfo \b IN: buffer handle
/// @param offsetofByte \b IN: start offset (should be 128 bit aligned)
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @return GFX_Result_NON_ALIGN_PITCH - The pitch is not 16 bytes alignment
/// @return GFX_Result_NON_ALIGN_ADDRESS - The address is not 16 bytes alignment
/// @note
/// The buffer start address and pitch smust be 128 bits alignment.
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDstBufferInfo(PGFX_BufferInfo bufInfo, MS_U32 offsetofByte)
{
    GFX_Result    eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(bufInfo);
    APIGFX_CHECK_INIT();

    //Utopia2 not support this function. Only stroed in wrapper.
    GFX_FireInfoLock();
    memcpy(&(GFXFireInfo.DstbufInfo), bufInfo, sizeof(GFX_BufferInfo));
    GFXFireInfo.u32DstOffsetofByte          =offsetofByte;
    GFXFireInfo.eFireInfo |= GFX_DST_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Get GFX SRC/DST buffer info
/// @param srcbufInfo \b IN: Pointer of src buffer info.
/// @param dstbufInfo \b IN: Pointer of dst buffer info.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/// @note
/// none
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_GetBufferInfo(PGFX_BufferInfo srcbufInfo, PGFX_BufferInfo dstbufInfo)
{
    GFX_Result  eRet = GFX_SUCCESS;
    APIGFX_CHECK_NULL(srcbufInfo);
    APIGFX_CHECK_NULL(dstbufInfo);
    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    memcpy(srcbufInfo, &(GFXFireInfo.SrcbufInfo), sizeof(GFX_BufferInfo));
    memcpy(dstbufInfo, &(GFXFireInfo.DstbufInfo), sizeof(GFX_BufferInfo));
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set GFX Bit blt
/// @param drawbuf \b IN: pointer to drawbuf info
/// @param drawflag \b IN: draw flag \n
///                  GFXDRAW_FLAG_DEFAULT \n
///                  GFXDRAW_FLAG_SCALE \n
///                  GFXDRAW_FLAG_DUPLICAPE \n
///                  GFXDRAW_FLAG_TRAPEZOID \n
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_BitBlt(GFX_DrawRect *drawbuf,MS_U32 drawflag)
{
    GFX_Result  eRet = GFX_SUCCESS;
    GFX_ScaleInfo ScaleInfo;

    APIGFX_CHECK_NULL(drawbuf);
    APIGFX_CHECK_INIT();

    GFX_BITBLT_ARGS    GFXBitBlt;
    GFX_BitBltInfo     GFXBitBltInfo;

    memset(&GFXBitBlt, 0 , sizeof(GFX_BITBLT_ARGS));
    memset(&GFXBitBltInfo, 0 , sizeof(GFX_BitBltInfo));
    memset(&ScaleInfo, 0 , sizeof(GFX_ScaleInfo));

    GFX_FireInfoLock();
    GFXBitBlt.pGFX_BitBlt           =(void*)&GFXBitBltInfo;
    GFXBitBlt.u32Size               =sizeof(GFX_BitBltInfo);
    GFXBitBltInfo.pFireInfo         = &GFXFireInfo;
    GFXBitBltInfo.pDrawRect         =drawbuf;
    GFXBitBltInfo.u32DrawFlag       =drawflag;
    GFXBitBltInfo.pScaleInfo        = &ScaleInfo;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_BITBLT, (void*)&GFXBitBlt) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX Bit blt with explicitly scale info.
/// @param drawbuf \b IN: pointer to drawbuf info
/// @param drawflag \b IN: draw flag \n
///                  GFXDRAW_FLAG_DEFAULT \n
///                  GFXDRAW_FLAG_SCALE \n
///                  GFXDRAW_FLAG_DUPLICAPE \n
///                  GFXDRAW_FLAG_TRAPEZOID \n
/// @param ScaleInfo \b IN: pointer to GFX_ScaleInfo assigned by AP.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_BitBltEx(GFX_DrawRect * drawbuf, MS_U32 drawflag, GFX_ScaleInfo * ScaleInfo)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(drawbuf);
    APIGFX_CHECK_NULL(ScaleInfo);
    APIGFX_CHECK_INIT();

    GFX_BITBLT_ARGS     GFXBitBlt;
    GFX_BitBltInfo      GFXBitBltInfo;

    memset(&GFXBitBlt, 0 , sizeof(GFX_BITBLT_ARGS));
    memset(&GFXBitBltInfo, 0 , sizeof(GFX_BitBltInfo));

    GFX_FireInfoLock();
    GFXBitBlt.pGFX_BitBlt           =(void*)&GFXBitBltInfo;
    GFXBitBlt.u32Size               =sizeof(GFX_BitBltInfo);
    GFXBitBltInfo.pFireInfo                     = &GFXFireInfo;
    GFXBitBltInfo.pDrawRect                     = drawbuf;
    GFXBitBltInfo.u32DrawFlag                   = drawflag;
    GFXBitBltInfo.pScaleInfo                    = ScaleInfo;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_BITBLT, (void*)&GFXBitBlt) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX destination buffer info in two source buffer mode
/// Attention, when two source buffer mode enable, raw DstBuffer will be one of two source.
/// @param dstbufInfo   \b IN: pointer to destination frame buffer infomation.\n
/// @param pdrawbuf \b IN: pointer to two source rects info
/// @param drawflag \b IN: draw flag \n
///                  GFXDRAW_FLAG_DEFAULT \n
///                  GFXDRAW_FLAG_SCALE \n
///                  GFXDRAW_FLAG_DUPLICAPE \n
///                  GFXDRAW_FLAG_TRAPEZOID \n
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_BitbltByTwoSourceBuffer(PGFX_BufferInfo dstbufInfo, GFX_DrawTwoSourceRect *pdrawbuf, MS_U32 drawflag)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_NULL(dstbufInfo);
    APIGFX_CHECK_NULL(pdrawbuf);
    APIGFX_CHECK_INIT();

    GFX_TWOSRC_BITBLT_ARGS      GFXTwoSrcBitBlt;
    GFX_TwoSrc_BitBltInfo       GFXTwoSrcBitBltInfo;
    GFX_Src_BufferInfo          GFXDstBufferInfo;

    memset(&GFXTwoSrcBitBlt, 0 , sizeof(GFX_TWOSRC_BITBLT_ARGS));
    memset(&GFXTwoSrcBitBltInfo, 0 , sizeof(GFX_TwoSrc_BitBltInfo));
    memset(&GFXDstBufferInfo, 0 , sizeof(GFX_Dst_BufferInfo));

    GFX_FireInfoLock();
    GFXTwoSrcBitBlt.pGFX_TwoSrc_BitBlt    =&GFXTwoSrcBitBltInfo;
    GFXTwoSrcBitBlt.u32Size               =sizeof(GFX_TwoSrc_BitBltInfo);
    GFXTwoSrcBitBltInfo.pDstbufInfo         = &GFXDstBufferInfo;
    GFXTwoSrcBitBltInfo.pFireInfo           = &GFXFireInfo;
    GFXTwoSrcBitBltInfo.pDstbufInfo->pBufInfo         = dstbufInfo;
    GFXTwoSrcBitBltInfo.pDstbufInfo->u32OffsetByte    = 0;
    GFXTwoSrcBitBltInfo.pDrawTwoSrcRect               = pdrawbuf;
    GFXTwoSrcBitBltInfo.u32DrawFlag                   = drawflag;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_TWOSRCBITBLT, (void*)&GFXTwoSrcBitBlt) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;

}

//-------------------------------------------------------------------------------------------------
/// Set GFX palette table
/// Palette entries started from u32PalStart to u32PalEnd will be updated according to the data pointed by pPalArray
/// @param pPalArray \b IN: pointer to an array of GFX_PaletteEntry
/// @param u32PalStart \b IN: Palette index from which the palette will be updated.
/// @param u32PalEnd \b IN: Palette index to which the palette will be updated.
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetPaletteOpt( GFX_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(pPalArray == NULL)
    {
        GFX_ERR("[%s][%d]    pPalArray =NULL\n",__FUNCTION__,__LINE__);
        return GFX_FAIL;
    }
    APIGFX_CHECK_INIT();

    GFX_SETCONFIG_ARGS     GFXSetConfig;
    GFX_Set_PaletteOpt     GFXSetPaletteOpt;

    memset(&GFXSetConfig, 0 , sizeof(GFX_SETCONFIG_ARGS));
    memset(&GFXSetPaletteOpt , 0 ,sizeof(GFX_Set_PaletteOpt));

    GFXSetPaletteOpt.pPalArray            = pPalArray;
    GFXSetPaletteOpt.u32PalStart          = u32PalStart;
    GFXSetPaletteOpt.u32PalEnd            = u32PalEnd;

    GFXSetConfig.eGFX_SetConfig           = E_GFX_SET_PALETTEOPT;
    GFXSetConfig.pGFX_ConfigInfo          = (void*)&GFXSetPaletteOpt;
    GFXSetConfig.u32Size                  = sizeof(GFX_Set_PaletteOpt);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// Set GFX CSC format
/// YUV/RGB conversion will be performed according to the spec specified in this function.
/// @param mode \b IN: YUV mode: PC or 0~255
/// @param yuv_out_range \b IN: output YUV mode: PC or 0~255
/// @param uv_in_range \b IN: input YUV mode: 0~255 or -126~127
/// @param srcfmt \b IN: YUV packing format for source
/// @param dstfmt \b IN: YUV packing format for destination
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
//-------------------------------------------------------------------------------------------------
GFX_Result MApi_GFX_SetDC_CSC_FMT(GFX_YUV_Rgb2Yuv mode, GFX_YUV_OutRange yuv_out_range, GFX_YUV_InRange uv_in_range, GFX_YUV_422 srcfmt, GFX_YUV_422 dstfmt)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_FireInfoLock();
    GFXFireInfo.GFXSetCSC.mode                  = mode;
    GFXFireInfo.GFXSetCSC.yuv_out_range         = yuv_out_range;
    GFXFireInfo.GFXSetCSC.uv_in_range           = uv_in_range;
    GFXFireInfo.GFXSetCSC.srcfmt                = srcfmt;
    GFXFireInfo.GFXSetCSC.dstfmt                = dstfmt;
    GFXFireInfo.eFireInfo |= GFX_CSC_INFO;
    GFX_FireInfoUnLock();

    return eRet;
}

/******************************************************************************/
///Set alpha parameter for ARGB1555.
///@param coef \b IN  alpha coefficient for ARGB1555
///@par Function Actions:
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetAlpha_ARGB1555(MS_U8 coef)
{
    GFX_Result  eRet = GFX_SUCCESS;
    GFX_MISC_ARGS   GFXInfo;

    APIGFX_CHECK_INIT();
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_SET_ALB_1555;
    GFXInfo.pGFX_Info     = (void*)&coef;
    GFXInfo.u32Size       = sizeof(MS_U8);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return eRet;
}


/******************************************************************************/
///Get alpha parameter for ARGB1555.
///@param coef \b IN  alpha coefficient for ARGB1555
///@par Function Actions:
/// @return GFX_SUCCESS - Success
/// @return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_GetAlpha_ARGB1555(MS_U8* coef)
{
    GFX_Result  eRet = GFX_SUCCESS;
    GFX_MISC_ARGS   GFXInfo;

    APIGFX_CHECK_INIT();
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_GET_ALB_1555;
    GFXInfo.pGFX_Info     = (void*)coef;
    GFXInfo.u32Size       = sizeof(MS_U8);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return eRet;
}


/******************************************************************************/
///Set TAGID
///@param tagID \b IN  16-bit TAGID
///@par Function Actions:
/******************************************************************************/
GFX_Result MApi_GFX_SetTAGID(MS_U16 tagID)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 , sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_TAGID;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&tagID;
    GFXSetConfig.u32Size              =sizeof(MS_U32);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}

/******************************************************************************/
///Get TAGID
///@param tagID \b IN  Pointer to 16-bit TAGID.
///@par Function Actions:
/******************************************************************************/

GFX_Result MApi_GFX_GetTAGID(MS_U16 *tagID)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(tagID ==NULL)
    {
        eRet = GFX_FAIL;
    }
    APIGFX_CHECK_INIT();

    GFX_GETINFO_ARGS    GFXGetInfo;

    memset(&GFXGetInfo, 0 ,sizeof(GFX_GETINFO_ARGS));

    GFXGetInfo.eGFX_GetConfig         =E_GFX_GET_TAGID;
    GFXGetInfo.u32Size                =sizeof(MS_U16);

    GFXGetInfo.pGFX_GetInfo            =tagID;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_INFO, (void*)&GFXGetInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }

    return eRet;
}
#endif

/******************************************************************************/
///Wait for TagID.
///@param tagID \b IN: tag to wait
///@par Function Actions:
/******************************************************************************/
GFX_Result MApi_GFX_WaitForTAGID(MS_U16 tagID)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_WAITFORTAGID;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&tagID;
    GFXSetConfig.u32Size              =sizeof(MS_U32);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}

/******************************************************************************/
///Polling TagID.
///@param tagID \b IN: tag to polling
///@par Function Actions:
/******************************************************************************/
GFX_Result MApi_GFX_PollingTAGID(MS_U16 tagID)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_POLLINGTAGID;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&tagID;
    GFXSetConfig.u32Size              =sizeof(MS_U32);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        //GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}
#ifndef MSOS_TYPE_OPTEE
/******************************************************************************/
///Retrieve next TagID.
///@param bStepTagBefore \b IN: steps to the tagID.
///@par Function Actions:
/******************************************************************************/
MS_U16 MApi_GFX_GetNextTAGID(MS_BOOL bStepTagBefore)
{
    MS_U16    u16Ret;

    APIGFX_CHECK_INIT();

    GFX_GETINFO_ARGS           GFXGetInfo;
    GFX_Get_NextTagID          GFXGetNextID;

    memset(&GFXGetInfo, 0 , sizeof(GFX_GETINFO_ARGS));
    memset(&GFXGetNextID, 0 , sizeof(GFX_Get_NextTagID));

    GFXGetInfo.eGFX_GetConfig         =E_GFX_GET_NEXTTAGID;
    GFXGetInfo.pGFX_GetInfo            =(void*)&GFXGetNextID;
    GFXGetInfo.u32Size                =sizeof(GFX_Get_NextTagID);

    GFXGetNextID.bStepTagBefore     = bStepTagBefore;
    GFXGetNextID.pTagID                = &u16Ret;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_INFO, (void*)&GFXGetInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }

    return u16Ret;
}
#endif

/******************************************************************************/
///Set next TagID Auto to HW.
///@par The Tage ID which has been set to HW
/******************************************************************************/
MS_U16 MApi_GFX_SetNextTAGID(void)
{
    MS_U16 tagID=0xFF;

    APIGFX_CHECK_INIT();

    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_NEXTTAGID;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&tagID;
    GFXSetConfig.u32Size              =sizeof(MS_U32);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }

    return tagID;
}

#ifndef MSOS_TYPE_OPTEE
/******************************************************************************/
///Enable GFX Virtual Command Queue
///@param blEnable \b IN: true: Enable, false: Disable
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_EnableVCmdQueue(MS_U16 blEnable)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 , sizeof(GFX_SETCONFIG_ARGS));

    GFXSerVQ.bEnable                    =blEnable;

    GFXSetConfig.eGFX_SetConfig            =E_GFX_SET_VQ;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSerVQ;
    GFXSetConfig.u32Size                =sizeof(GFX_Set_VQ);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Configure GFX Virtual Command Queue buffer spec
///@param u32Addr \b IN: base adress for VCMQ buffer
///@param enBufSize \b IN: buffer size of VCMQ buffer
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetVCmdBuffer(MS_PHY PhyAddr, GFX_VcmqBufSize enBufSize)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig           =E_GFX_SET_VQ;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSerVQ;
    GFXSetConfig.u32Size                  =sizeof(GFX_Set_VQ);

    GFXSerVQ.u32Addr                      =PhyAddr;
    GFXSerVQ.enBufSize                    =enBufSize;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;

}

/******************************************************************************/
///Configure GFX Virtual Command Queue Write Threshold
///@param u8W_Threshold \b IN: Write threshold
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GE_SetVCmd_W_Thread(MS_U8 u8W_Threshold)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig           =E_GFX_SET_VQ;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSerVQ;
    GFXSetConfig.u32Size                  =sizeof(GFX_Set_VQ);

    GFXSerVQ.u8W_Threshold                =u8W_Threshold;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Configure GFX Virtual Command Queue Read Threshold
///@param u8R_Threshold \b IN: Read threshold
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GE_SetVCmd_R_Thread(MS_U8 u8R_Threshold)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0, sizeof(GFX_SETCONFIG_ARGS));

       GFXSetConfig.eGFX_SetConfig           =E_GFX_SET_VQ;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSerVQ;
    GFXSetConfig.u32Size                  =sizeof(GFX_Set_VQ);

    GFXSerVQ.u8R_Threshold                =u8R_Threshold;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;
}


/******************************************************************************/
///Configure GFX DBG level
///@param level \b IN: debug level. 0 => no dbg.
///@par Function Actions:
/******************************************************************************/
void MApi_GFX_SetDbgLevel(MS_U32 level)
{
    GFX_MISC_ARGS   GFXInfo;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));
    MS_U32 u32level;

    u32level = level;
    GFXInfo.eGFX_MISCType = E_MISC_DEBUG_LEVEL;
    GFXInfo.pGFX_Info     = (void*)&u32level;
    GFXInfo.u32Size       = sizeof(MS_U32);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
}


/******************************************************************************/
///Explicitly wait for GFX queue empty
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_FlushQueue(void)
{
    GFX_Result  eRet = GFX_SUCCESS;
    MS_U32 value = 0;
    APIGFX_CHECK_INIT();

    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0 ,sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_FLUSHQUEUE;
    GFXSetConfig.u32Size              =sizeof(0);
    GFXSetConfig.pGFX_ConfigInfo      =&value;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Configure the Color Key edge refine function.
///@param type \b IN: type of refine.
///@param color \b IN: when type is GFX_REPLACE_KEY_2_CUS, color of the customized color.
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetStrBltSckType(GFX_StretchCKType type, GFX_RgbColor *color)
{

    GFX_FireInfoLock();
    GFXFireInfo.sttype.type = type;
    GFXFireInfo.sttype.color = *color;
    GFXFireInfo.eFireInfo |= GFX_STR_BLT_SCK_INFO;
    GFX_FireInfoUnLock();

    return GFX_SUCCESS;
}


/******************************************************************************/
///Check if GFX lib is running as HouseKeeping.
///@param bIsHK \b IN: pointer to a bool denoting true/false
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_GetHK(MS_BOOL *bIsHK)
{
    GFX_MISC_ARGS   GFXInfo;
    MS_BOOL IsHK;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_GET_HK;
    GFXInfo.pGFX_Info     = (void*)&IsHK;
    GFXInfo.u32Size       = sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    *bIsHK = IsHK;
    return GFX_SUCCESS;
}

/******************************************************************************/
///Set the GFX lib running as HouseKeeping
///@param bIsHK \b IN:  true => As HK, false => Not HK
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetHK(MS_BOOL bIsHK)
{
    GFX_MISC_ARGS   GFXInfo;
    MS_BOOL IsHK;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));
    IsHK = bIsHK;
    GFXInfo.eGFX_MISCType = E_MISC_SET_HK;
    GFXInfo.pGFX_Info     = (void*)&IsHK;
    GFXInfo.u32Size       = sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
    return GFX_SUCCESS;
}


/******************************************************************************/
///Draw Oval. Oval is not directly supported by HW. Software implemented by DrawLine.
///@param pOval \b IN:  Oval info
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_DrawOval(GFX_OvalFillInfo *pOval)
{
    GFX_Result  eRet = GFX_SUCCESS;
    APIGFX_CHECK_INIT();

    GFX_DRAW_OVAL_ARGS   GFXDrawOval;
    GFX_Set_DrawOvalInfo    GFXDrawOvalInfo;

    memset(&GFXDrawOval, 0 ,sizeof(GFX_DRAW_OVAL_ARGS));
    memset(&GFXDrawOvalInfo, 0 ,sizeof(GFX_Set_DrawOvalInfo));

    GFX_FireInfoLock();
    GFXDrawOvalInfo.pFireInfo                   = &GFXFireInfo;
    GFXDrawOvalInfo.pDrawOvalInfo               = pOval;
    GFXDrawOval.psetting            =(void*)&GFXDrawOvalInfo;
    GFXDrawOval.u32Size             =sizeof(GFX_Set_DrawOvalInfo);
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_DRAW_OVAL, (void*)&GFXDrawOval) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }
    GFXFireInfo.eFireInfo = 0;
    GFX_FireInfoUnLock();

    return eRet;
}


/******************************************************************************/
///Get GFX version string
///@param ppVersion \b IN:  version string
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_GetLibVer(const MSIF_Version **ppVersion)
{
    APIGFX_CHECK_INIT();

    if (!ppVersion)
    {
        return GFX_FAIL;
    }

    *ppVersion = &_drv_gfx_version;

    return GFX_SUCCESS;
}

/******************************************************************************/
///Get GFX debug information
///@param dbg \b IN:  pointer to debug struct.
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_GetInfo( GFX_DbgInfo *dbg )
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(dbg == NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();
    GFX_GETINFO_ARGS   GFXGetInfo;
    GFX_DbgInfo        GFXDbgInfo;

    memset(&GFXGetInfo, 0, sizeof(GFX_GETINFO_ARGS));
    memset(&GFXDbgInfo, 0 ,sizeof(GFX_DbgInfo));

    GFXGetInfo.eGFX_GetConfig         =E_GFX_GET_DBGINFO;
    GFXGetInfo.pGFX_GetInfo           =(void*)&GFXDbgInfo;
    GFXGetInfo.u32Size                =sizeof(GFX_DbgInfo);

    GFXGetInfo.pGFX_GetInfo            =dbg;
    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_INFO, (void*)&GFXGetInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }
    return eRet;
}

/******************************************************************************/
///Get GFX internal status
///@param pStatus \b IN:  pointer to debug struct.
///@par Function Actions:
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_GetStatus( MS_BOOL *pStatus )
{
#if 0
    APIGFX_CHECK_INIT();

    MDrv_GE_GetStatus(g_apiGFXLocal.g_pGEContext, pStatus);
#endif
    *pStatus = TRUE;
    return GFX_SUCCESS;
}


/******************************************************************************/
///Set GFX Power On
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_PowerOn(void)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    MS_BOOL                    bVal                =TRUE;
    GFX_SETCONFIG_ARGS      GFXSetConfig;

    memset(&GFXSetConfig, 0, sizeof(GFX_SETCONFIG_ARGS));

    GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_POWERON;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&bVal;
    GFXSetConfig.u32Size              =sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}


/******************************************************************************/
///Set GFX Power Off
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_PowerOff(void)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();

    MS_BOOL                bVal            =FALSE;
    GFX_SETCONFIG_ARGS    GFXSetConfig;

    memset(&GFXSetConfig, 0 , sizeof(GFX_SETCONFIG_ARGS));

       GFXSetConfig.eGFX_SetConfig       =E_GFX_SET_POWERON;
    GFXSetConfig.pGFX_ConfigInfo      =(void*)&bVal;
    GFXSetConfig.u32Size              =sizeof(MS_BOOL);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }

    return eRet;
}

/******************************************************************************/
///Set GFX Write Protect
/// @param  miu                     \b IN: miu being procted
/// @param  addr_low                     \b IN: low address of specified area
/// @param  addr_high                     \b IN: high address of specified area
/// @param  eWPType                     \b IN: GE protect operator
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_WriteProtect(MS_U8 miu,MS_PHY addr_low,MS_PHY addr_high,GFX_WPType eWPType)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;
    GFX_Set_WriteProtect   GFXSetMiuProtect;

    memset(&GFXSetConfig, 0, sizeof(GFX_SETCONFIG_ARGS));
    memset(&GFXSetMiuProtect, 0 ,sizeof(GFX_Set_WriteProtect));

    GFXSetConfig.eGFX_SetConfig           =E_GFX_SET_WRITEPROTECT;
    GFXSetConfig.pGFX_ConfigInfo        =(void*)&GFXSetMiuProtect;
    GFXSetConfig.u32Size                  =sizeof(GFX_Set_WriteProtect);

    GFXSetMiuProtect.u8Miu            =miu;
    GFXSetMiuProtect.u32Addr_Low      =addr_low;
    GFXSetMiuProtect.u32Addr_High     =addr_high;
    GFXSetMiuProtect.eWPType          =eWPType;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}

GFX_Result MApi_GFX_GetAlignCaps(GFX_FmtAlignCapsInfo* pAlignCapsInfo)
{
    MApi_GFX_GetGECaps(E_GFX_CAP_ALIGN,(MS_U32* )(void*)pAlignCapsInfo,sizeof(GFX_FmtAlignCapsInfo));
    return GFX_SUCCESS;
}

/******************************************************************************/
///Set GFX Config
/// @param  enType                    	\b IN: enType
/// @param  plist                     	\b IN: plist
/// @param  u32Len                     	\b IN: u32Len
///@return GFX_SUCCESS - Success
///@return GFX_FAIL - Failure
/******************************************************************************/
GFX_Result MApi_GFX_SetConfig(EN_GFX_CONFIG_TYPE enType,void *plist,MS_U32 u32Len)
{
    GFX_Result  eRet = GFX_SUCCESS;

    APIGFX_CHECK_INIT();
    GFX_SETCONFIG_ARGS     GFXSetConfig;

    memset(&GFXSetConfig, 0, sizeof(GFX_SETCONFIG_ARGS));
    switch(enType)
    {
        case E_GFX_CONFIG_BURST_MIU_LENGTH:
        {
            if( u32Len != sizeof(GFX_Set_BurstMiuLen) )
                return GFX_INVALID_PARAMETERS;

            GFXSetConfig.eGFX_SetConfig             =   E_GFX_SET_BURSTMIULEN;
            GFXSetConfig.pGFX_ConfigInfo            =   plist;
            GFXSetConfig.u32Size                    =   u32Len;
            break;
        }
        default:
            return GFX_INVALID_PARAMETERS;
    }

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_SET_CONFIG, (void*)&GFXSetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        printf("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet= GFX_FAIL;
    }
    return eRet;
}

GFX_Result MApi_GFX_GetConfig(EN_GFX_CONFIG_INIT enType,void *plist,MS_U32 u32Len)
{
    GFX_Result  eRet = GFX_SUCCESS;
    GFX_GETCONFIG_ARGS     GFXGetConfig;

    if(NULL == pGEInstance)
    {
        if(UtopiaOpen(MODULE_GFX | GFXDRIVER_BASE, &pGEInstance, 0, NULL))
        {
            GFX_ERR("%s fail [LINE:%d]    GFXOpen Fail\n",__FUNCTION__, __LINE__);
            return GFX_FAIL;
        }
    }
    memset(&GFXGetConfig, 0, sizeof(GFX_GETCONFIG_ARGS));
    switch(enType)
    {
        case E_GFX_CONFIG_INIT:
        {
            if( u32Len != sizeof(GFX_GETCONFIG_ARGS) )
                return GFX_INVALID_PARAMETERS;

            GFXGetConfig.eGFX_GetConfig             =   E_GFX_GET_INIT;
            GFXGetConfig.pGFX_ConfigInfo            =   plist;
            GFXGetConfig.u32Size                    =   u32Len;
            break;
        }
        default:
            return GFX_INVALID_PARAMETERS;
    }

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_CONFIG, (void*)&GFXGetConfig) != UTOPIA_STATUS_SUCCESS)
    {
        printf("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        return GFX_FAIL;
    }

    return eRet;
}

//-------------------------------------------------------------------------------------------------
/// GE Exit
/// @param  void                \b IN: none
//-------------------------------------------------------------------------------------------------
void MApi_GE_Exit(void)
{

    GFX_MISC_ARGS   GFXInfo;
    MS_U32 value = 0;
    memset(&GFXInfo, 0, sizeof(GFX_MISC_ARGS));

    GFXInfo.eGFX_MISCType = E_MISC_EXIT;
    GFXInfo.pGFX_Info     = (void*)&value;
    GFXInfo.u32Size       = sizeof(GFX_MISC_ARGS);

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_MISC, (void*)&GFXInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_DBUG("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
    }
}

GFX_Result MApi_GFX_GetGECaps(EN_GFX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    GFX_Result  eRet = GFX_SUCCESS;

    if(pRet ==NULL)
    {
        return GFX_FAIL;
    }

    APIGFX_CHECK_INIT();

    GFX_GETCAPS_ARGS   GFXGetCaps;
    GFX_Get_CAPS       GetCaps;

    memset(&GFXGetCaps, 0 ,sizeof(GFX_GETCAPS_ARGS));
    memset(&GetCaps, 0 ,sizeof(GFX_Get_CAPS));

    GFXGetCaps.eCaps                    = eCapType;
    GFXGetCaps.pGFX_GetCaps            =(void*)&GetCaps;
    GFXGetCaps.u32Size                =sizeof(GFX_Get_CAPS);

    GetCaps.eCapType               =eCapType;
    GetCaps.pRet                   =pRet;
    GetCaps.Ret_Size               =ret_size;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_GET_CAPS, (void*)&GFXGetCaps) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        eRet = GFX_FAIL;
    }

    return eRet;
}

/// Wrapper ///

#if 1

/******************************************************************************/
///Wrapper of MApi_GFX_ClearFrameBuffer for backward compatibility.
///Please see MApi_GFX_ClearFrameBuffer section for detail.
/******************************************************************************/
void MDrv_GE_ClearFrameBuffer( MS_PHY StrAddr, MS_U32 length, MS_U8 ClearValue)   //jupiter no miu
{
    MApi_GFX_ClearFrameBuffer( StrAddr,  length,  ClearValue);
}
#endif

/******************************************************************************/
///Wrapper of MApi_GFX_SetPaletteOpt for backward compatibility.
///Please see MApi_GFX_SetPaletteOpt section for detail.
/******************************************************************************/
void MDrv_GE_SetPaletteOpt( GFX_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
    MApi_GFX_SetPaletteOpt( pPalArray,  u32PalStart,  u32PalEnd);
}


/******************************************************************************/
///Wrapper of MApi_GFX_SetAlpha for backward compatibility.
///Please see MApi_GFX_SetAlpha section for detail.
/******************************************************************************/
void MDrv_GE_SetAlpha(MS_BOOL enable, MS_U8 coef, MS_U8 db_abl, MS_U8 abl_const)
{
    MApi_GFX_SetAlpha(enable,  (GFX_BlendCoef)coef,  (GFX_AlphaSrcFrom)db_abl,  abl_const);          // DANGER
}

/******************************************************************************/
///Wrapper of MApi_GFX_SetPatchMode for backward compatibility.
///Please see MApi_GFX_SetPatchMode section for detail.
/******************************************************************************/
void MDrv_GE_SetPatchMode(MS_BOOL repeat)
{
    MApi_GFX_SetPatchMode(repeat);
}

/******************************************************************************/
///Wrapper of MApi_GFX_SetNearestMode for backward compatibility.
///Please see MApi_GFX_SetNearestMode section for detail.
/******************************************************************************/
void MDrv_GE_SetNearestMode(MS_BOOL enable)
{
    MApi_GFX_SetNearestMode(enable);
}

/******************************************************************************/
///Wrapper of MApi_GFX_EnableAlphaBlending for backward compatibility.
///Please see MApi_GFX_EnableAlphaBlending section for detail.
/******************************************************************************/
GFX_Result MDrv_GE_EnableAlphaBlending(MS_BOOL enable)
{
    APIGFX_CHECK_INIT();

    return  MApi_GFX_EnableAlphaBlending(enable);
}

/******************************************************************************/
///Wrapper of MApi_GFX_SetAlphaBlending for backward compatibility.
///Please see MApi_GFX_SetAlphaBlending section for detail.
/******************************************************************************/
GFX_Result MDrv_GE_SetAlphaBlending(GFX_BlendCoef blendcoef, MS_U8 u8ConstantAlpha)
{
    APIGFX_CHECK_INIT();

    return MApi_GFX_SetAlphaBlending( blendcoef, u8ConstantAlpha);  // DANGER
}

/******************************************************************************/
///Wrapper of MApi_GFX_ClearFrameBufferByWord for backward compatibility.
///Please see MApi_GFX_ClearFrameBufferByWord section for detail.
/******************************************************************************/
void msAPI_GE_ClearFrameBufferByWord (MS_PHY StrAddr, MS_U32 length, MS_U32 ClearValue)
{
    MApi_GFX_ClearFrameBufferByWord( StrAddr,  length,  ClearValue);
}


/******************************************************************************/
///Wrapper of MApi_GFX_SetIntensity for backward compatibility.
///Please see MApi_GFX_SetIntensity section for detail.
/******************************************************************************/
void MDrv_GE_PE_SetIntensity(MS_U8 id, GFX_Buffer_Format fmt, MS_U32* pcolor)
{
    MApi_GFX_SetIntensity( id,  fmt, pcolor);
}

/******************************************************************************/
///GFX STR Mode
/******************************************************************************/
MS_U32 MApi_GFX_SetPowerState(EN_POWER_MODE u16PowerState)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    APIGFX_CHECK_INIT();

    GFX_POWERSTATE_ARGS    pGFXPowerStateArg;

    memset(&pGFXPowerStateArg, 0 , sizeof(GFX_POWERSTATE_ARGS));
    pGFXPowerStateArg.eGFX_PowerStateType       = u16PowerState;

    if(UtopiaIoctl(pGEInstance, MAPI_CMD_GFX_POWERSTATE, (void*)&pGFXPowerStateArg) != UTOPIA_STATUS_SUCCESS)
    {
        GFX_ERR("%s fail [LINE:%d]\n",__FUNCTION__, __LINE__);
        u32Return= UTOPIA_STATUS_FAIL;
    }
    else
    {
        u32Return= UTOPIA_STATUS_SUCCESS;
    }

    return u32Return;

}
#endif

EXPORT_SYMBOL(MApi_GFX_Init);
EXPORT_SYMBOL(MApi_GFX_SetDFBBldOP);
EXPORT_SYMBOL(MApi_GFX_SetRotate);
EXPORT_SYMBOL(MApi_GFX_SetSrcColorKey);
EXPORT_SYMBOL(MApi_GFX_SetDC_CSC_FMT);
EXPORT_SYMBOL(MApi_GFX_SetSrcBufferInfo);
EXPORT_SYMBOL(MApi_GFX_SetDstBufferInfo);
EXPORT_SYMBOL(MApi_GFX_EndDraw);
EXPORT_SYMBOL(MApi_GFX_SetClip);
EXPORT_SYMBOL(MApi_GFX_SetMirror);
EXPORT_SYMBOL(MApi_GFX_EnableAlphaBlending);
EXPORT_SYMBOL(MApi_GFX_WaitForTAGID);
EXPORT_SYMBOL(MApi_GFX_BeginDraw);
EXPORT_SYMBOL(MApi_GFX_EnableDFBBlending);
EXPORT_SYMBOL(MApi_GFX_BitBlt);
EXPORT_SYMBOL(MApi_GFX_SetROP2);
EXPORT_SYMBOL(MApi_GFX_RectFill);
EXPORT_SYMBOL(MApi_GFX_SetDstColorKey);
EXPORT_SYMBOL(MApi_GFX_SetNextTAGID);
EXPORT_SYMBOL(MApi_GFX_PollingTAGID);
EXPORT_SYMBOL(MApi_GFX_EnableVCmdQueue);
EXPORT_SYMBOL(MApi_GFX_SetVCmdBuffer);
