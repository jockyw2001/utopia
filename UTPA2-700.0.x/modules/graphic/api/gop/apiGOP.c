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

////////////////////////////////////////////////////////////////////////////////
//  [Doxygen]
/// file apiGOP.c
/// This file includes MStar application interface for Graphic output path
/// @brief API for Graphics output path (GOP)
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "utopia.h"
#include "MsTypes.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiGOP.h"
#include "apiGOP_priv.h"
#include "util_symbol.h"
#include "drvGOP.h"
#include "MsOS.h"


#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_v2.h"
#include "mvideo_context.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
#ifndef GOP_UTOPIA2K
#define GOP_UTOPIA2K
#endif
#endif

#define DUMP_INFO   0UL

#define FPGA_TEST   0UL

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0)
#endif

#if defined(MSOS_TYPE_LINUX)
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(MSOS_TYPE_LINUX_KERNEL)
    #define getpid()    pInstantGOP
#else
#define getpid() 0UL
#endif

#define GetMaxActiveGwinFalse 4UL
#define GetMaxActiveGwinFalse_op 5UL
#define GetMaxActiveGwinFalse_opened 6UL
#define PALETTE_ENTRY_NUM   32UL
#define GWIN_SDRAM_NULL 0x30UL
#define msWarning(c)    do {} while (0)
#define XC_MAIN_WINDOW  0UL

#define SCALING_MULITPLIER 0x1000UL
#define UNUSED( var )                   (void)((var) = (var))

//=============================================================
#include "ULog.h"
MS_U32 u32GOPDbgLevel_api;

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define GOP_INFO(x, args...) if(u32GOPDbgLevel_api >= E_GOP_Debug_Level_HIGH) { ULOGI("GOP API", x, ##args); }
// Warning, illegal paramter but can be self fixed in functions
#define GOP_WARN(x, args...) if(u32GOPDbgLevel_api >= E_GOP_Debug_Level_HIGH) { ULOGW("GOP API", x, ##args); }
//  Need debug, illegal paramter.
#define GOP_DBUG(x, args...) if(u32GOPDbgLevel_api >= E_GOP_Debug_Level_MED) { ULOGD("GOP API", x, ##args); }
// Error, function will be terminated but system not crash
#define GOP_ERR(x, args...) if(u32GOPDbgLevel_api >= E_GOP_Debug_Level_LOW) { ULOGE("GOP API", x, ##args); }
// Critical, system crash. (ex. assert)
#define GOP_FATAL(x, args...) if(u32GOPDbgLevel_api >= E_GOP_Debug_Level_LOW) { ULOGF("GOP API", x, ##args); }

//=============================================================

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#include <unistd.h>
#define GOP_ASSERT(_bool, pri)  if (!(_bool)) {GOP_FATAL("\nAssert in %s,%d\n", __FUNCTION__, __LINE__); (pri); MsOS_DelayTask(100); assert(0);}
#else
#define GOP_ASSERT(_bool, pri)  if (!(_bool)) {GOP_FATAL("\nAssert in %s,%d\n", __FUNCTION__, __LINE__); (pri);};
#endif

#ifdef MSOS_TYPE_LINUX
static pthread_mutex_t          GOP_XC_Instance_Mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void _GOP_XC_Instance_Lock(void)
{

#ifdef MSOS_TYPE_LINUX
    pthread_mutex_lock(&GOP_XC_Instance_Mutex);
#endif

}

static void _GOP_XC_Instance_UnLock(void)
{

#ifdef MSOS_TYPE_LINUX
    pthread_mutex_unlock(&GOP_XC_Instance_Mutex);
#endif

}


#define CheckGOPInstanceOpen() do{\
    if(pInstantGOP == NULL)\
    {\
        if(UtopiaOpen(MODULE_GOP | GOPDRIVER_BASE, &pInstantGOP, 0, pAttributeGOP) !=  UTOPIA_STATUS_SUCCESS)\
        {\
            GOP_ERR("Open GOP fail\n");\
            return GOP_API_FAIL;\
        }\
    }\
}while(0)


void* pInstantGOP_XC = NULL;

#define CheckXCOpen()   do{\
     _GOP_XC_Instance_Lock();\
     if(pInstantGOP_XC==NULL)\
     {\
         XC_INSTANT_ATTRIBUTE stXCInstantAttribute;\
         memset(&stXCInstantAttribute, 0, sizeof(stXCInstantAttribute));\
         stXCInstantAttribute.u32DeviceID = 0;\
         stXCInstantAttribute.eXCType     = E_XC_INSTANT_TYPE_GOP;\
        if(UtopiaOpen(MODULE_XC, &pInstantGOP_XC, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)\
        {\
            GOP_ERR("UtopiaOpen XC failed\n");\
            return GOP_API_FAIL;\
        }\
     }\
     _GOP_XC_Instance_UnLock();\
}while(0)


#define GOP_ENTRY()                 do{ \
                                        APIGOP_ASSERT(bInit, GOP_FATAL("\n [Function   %s] [%d] Need GOP Driver Init First !!!!!!!\n",__FUNCTION__,__LINE__));\
                                        _GOP_Entry();\
                                    }while(0);
//{if(TRUE != MDrv_GOP_GWIN_BeginDraw()){GOP_ASSERT(FALSE, printf("\nError: Obtain GOP mutex fail\n"));}}
#define GOP_RETURN(_ret)             do{\
                                        return _ret;\
                                     }while(0);//{if(TRUE == MDrv_GOP_GWIN_EndDraw()){return _ret;}else{GOP_ASSERT(FALSE, printf("\nError: Release GOP mutex fail\n"));return _ret;}}

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//GOP Lib Version Control
static MSIF_Version _api_gop_version =
{
    .DDI = { GOP_API_VERSION },
};


static MS_U32 (*fpSetFBFmt)(MS_U16 ,MS_PHY , MS_U16 ) = NULL;
static MS_BOOL (*fpXCIsInterlace)(void) = NULL;
static MS_U16 (*fpXCGetCapHStart)(void) = NULL;
static void (*fpXCReduceBWForOSD)(MS_U8 , MS_BOOL) = NULL;
static void (*fpEventNotify)(MS_U32 , void* ) = NULL;
static MS_BOOL (*fpXCSetDwinInfo)(MS_U16 cmd,MS_U32 *arg,MS_U16 size) = NULL;
static MS_BOOL bInit = FALSE;
MS_U32 u32TransColor = 0x00000000;

//-------------------------------------------------------------------------------------------------
//  Utopia 2.0
//-------------------------------------------------------------------------------------------------

void* pInstantGOP = NULL;
void* pAttributeGOP = NULL;

GOP_Palette PaletteEntry[PALETTE_ENTRY_NUM] =
{
    //   A,    R,    G,    B
    { 0x00, 0x00, 0x00, 0x00 },   // black           0
    { 0x00, 0xff, 0x00, 0x00 },   // red             1
    { 0x00, 0x00, 0xff, 0x00 },   // green           2
    { 0x00, 0xff, 0xff, 0x00 },   // yellow          3
    { 0x00, 0x00, 0x00, 0xff },   // blue            4
    { 0x00, 0xff, 0x00, 0xff },   // magenta         5
    { 0x00, 0x00, 0xff, 0xff },   // cyan            6
    { 0x00, 0xff, 0xff, 0xff },   // white           7
    { 0x00, 0xff, 0x08, 0xff },   // transparent     8
    { 0x00, 0x7f, 0x00, 0x00 },   // half red        9
    { 0x00, 0xCC, 0xCC, 0xCC },   // half green      10
    { 0x00, 0x54, 0x54, 0x54 },   // half yellow     11
    { 0x00, 0x00, 0x00, 0x7f },   // half blue       12
    { 0x00, 0x7f, 0x00, 0x7f },   // half magenta    13
    { 0x00, 0x00, 0x7f, 0x7f },   // half cyan       14
    { 0x00, 0x7f, 0x7f, 0x7f },   // gray            15
    { 0x00, 0x0f, 0x0f, 0x0f },
    { 0x00, 0xff, 0x00, 0x00 },
    { 0x00, 0x00, 0xff, 0x00 },
    { 0x00, 0xff, 0xff, 0x00 },
    { 0x00, 0x00, 0x00, 0xff },
    { 0x00, 0xff, 0x00, 0xff },
    { 0x00, 0x00, 0xff, 0xff },
    { 0x00, 0xff, 0xff, 0xff },
    { 0x00, 0x00, 0x00, 0x00 },   // Transparent      24
    { 0x00, 0x7f, 0x00, 0x00 },
    { 0x00, 0x00, 0x7f, 0x00 },
    { 0x00, 0x7f, 0x7f, 0x00 },
    { 0x00, 0x00, 0x00, 0x7f },
    { 0x00, 0x7f, 0x00, 0x7f },
    { 0x00, 0x00, 0x7f, 0x7f },
    { 0x00, 0x7f, 0x7f, 0x7f },
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------
static MS_U32 GOP_CalcPitch(MS_U8 fbFmt, MS_U16 width)
{
    MS_U16 pitch=0;

    switch ( fbFmt )
    {
    case E_MS_FMT_I1 :
        pitch = (width) >> 3;
        break;
    case E_MS_FMT_I2 :
        pitch = (width<<1) >> 3;
        break;
    case E_MS_FMT_I4 :
        pitch = (width<<2) >> 3;
        break;
    case E_MS_FMT_I8 :
        pitch = width;
        break;
    case E_MS_FMT_RGB565 :
    case E_MS_FMT_BGR565 :
    case E_MS_FMT_ARGB1555 :
    case E_MS_FMT_ABGR1555 :
    case E_MS_FMT_BGRA5551 :
    case E_MS_FMT_RGBA5551 :
    case E_MS_FMT_ARGB4444 :
    case E_MS_FMT_RGBA4444 :
    case E_MS_FMT_ABGR4444 :
    case E_MS_FMT_BGRA4444 :
    case E_MS_FMT_1ABFgBg12355:
    case E_MS_FMT_FaBaFgBg2266:
    case E_MS_FMT_YUV422:
    case E_MS_FMT_ARGB1555_DST :
        pitch = width << 1;
        break;
    case E_MS_FMT_AYUV8888 :
    case E_MS_FMT_ARGB8888 :
    case E_MS_FMT_RGBA8888 :
    case E_MS_FMT_BGRA8888:
    case E_MS_FMT_ABGR8888 :
        pitch = width << 2;
        break;
    default :
        //print err
        pitch = 0;
        break;
    }
    return pitch;
}

static MS_U16 GOP_GetBPP(EN_GOP_COLOR_TYPE fbFmt)
{
    MS_U16 bpp=0;

    switch ( fbFmt )
    {
    case E_GOP_COLOR_RGB555_BLINK :
    case E_GOP_COLOR_RGB565 :
    case E_GOP_COLOR_BGR565 :
    case E_GOP_COLOR_ARGB1555:
    case E_GOP_COLOR_ABGR1555:
    case E_GOP_COLOR_ARGB4444 :
    case E_GOP_COLOR_RGBA4444 :
    case E_GOP_COLOR_ABGR4444 :
    case E_GOP_COLOR_BGRA4444 :
    case E_GOP_COLOR_RGB555YUV422:
    case E_GOP_COLOR_YUV422:
    case E_GOP_COLOR_2266:
    case E_GOP_COLOR_RGBA5551:
    case E_GOP_COLOR_BGRA5551:
        bpp = 16;
        break;
    case E_GOP_COLOR_AYUV8888 :
    case E_GOP_COLOR_ARGB8888 :
    case E_GOP_COLOR_ABGR8888 :
    case E_GOP_COLOR_RGBA8888 :
    case E_GOP_COLOR_BGRA8888 :
        bpp = 32;
        break;

    case E_GOP_COLOR_I8 :
        bpp = 8;
        break;
    default :
        //print err
        //__ASSERT(0);
        bpp = FB_FMT_AS_DEFAULT;
        break;
    }
    return bpp;

}

#ifdef UTOPIA_V2
static E_GOP_API_Result _XCGetCapHStartCB(void)
{
    MS_XC_DST_DispInfo dstDispInfo;
    MS_U32 u32XCHStart=0;
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    CheckXCOpen();

    MApi_XC_GetDstInfo(&dstDispInfo,sizeof(MS_XC_DST_DispInfo),E_GOP_XCDST_IP1_MAIN);
    u32XCHStart = dstDispInfo.DEHST;

    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_XC_PANEL_HSTART;
        ioctl_info.pCB = (void*)&u32XCHStart;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
    return GOP_API_SUCCESS;
}

static E_GOP_API_Result _XCGetXCInterlaceCB(void)
{
    XC_ApiStatus xcStatus;
    MS_BOOL bInterlace=0;
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    CheckXCOpen();

    MApi_XC_GetStatus(&xcStatus,MAIN_WINDOW);
    bInterlace = xcStatus.bInterlace;

    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_XC_INTERLACE;
        ioctl_info.pCB = &bInterlace;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
    return GOP_API_SUCCESS;
}
#endif

static void _GOP_Entry(void)
{
#ifdef UTOPIA_V2
    //_XCGetCapHStartCB();
    //_XCGetXCInterlaceCB();
#endif
}
E_GOP_API_Result MApi_GOP_GWIN_ResetPool(void)
{
    MS_U32 i;
    static MS_BOOL bPoolReseted = FALSE;

    if (bPoolReseted)
    {
        return GOP_API_SUCCESS;
    }
    else
    {
        bPoolReseted = TRUE;
    }
    CheckGOPInstanceOpen();
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_RESETPOOL,&i) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
    return GOP_API_SUCCESS;

}

E_GOP_API_Result Mapi_GOP_GWIN_ResetGOP(MS_U32 u32Gop)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32 i;

    CheckGOPInstanceOpen();
    ioctl_info.en_pro   = E_GOP_RESOURCE;
    ioctl_info.gop_idx  = u32Gop;
    ioctl_info.pSetting = (void*)&i;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return GOP_API_SUCCESS;
}

/********************************************************************************/
/// Open API function:
///     Read GWIN information from GOP registers
/// @param u8win \b IN  GWINID
/// @param pinfo \b OUT buffer to store GWIN information
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetWinInfo(MS_U8 u8win, GOP_GwinInfo* pinfo)
{
    GOP_ENTRY();

    GOP_GWIN_SETWININFO_PARAM ioctl_info;
    GOP_BUFFER_INFO win_info;

    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&win_info, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.GwinId = u8win;
    ioctl_info.pinfo = (MS_U32*)&win_info;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_WININFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    pinfo->u32DRAMRBlkStart   =  win_info.addr;
    pinfo->u16DispHPixelStart =  win_info.disp_rect.x ;
    pinfo->u16DispVPixelStart =  win_info.disp_rect.y;
    pinfo->u16DispHPixelEnd   =  win_info.disp_rect.x + win_info.disp_rect.w;
    pinfo->u16DispVPixelEnd   =  win_info.disp_rect.y + win_info.disp_rect.h;
    pinfo->u16RBlkHPixSize    =  win_info.width;
    pinfo->u16RBlkVPixSize    =  win_info.height;
    pinfo->clrType            =  (EN_GOP_COLOR_TYPE)win_info.fbFmt;
    pinfo->u16RBlkHRblkSize   =  win_info.pitch;


    GOP_RETURN(GOP_API_SUCCESS);

}
MS_U32 _GOP_RegisterAllCBFunc(void);

/********************************************************************************/
/// Set transparent color (ARGB domain). Note that this funcion just set transparent color to gop hareware but
/// does not enable gop transparent function yet.
/// @param clr \b IN: transparent color
/// @param mask \b IN: alpha value
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_8888_EX(MS_U8 u8GOP,MS_U32 clr, MS_U32 mask)
{

    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TRANSCOLOR trans_set;

    GOP_ENTRY();

    trans_set.bEn = TRUE;
    trans_set.color = clr;
    trans_set.fmt = GOPTRANSCLR_FMT2;
    trans_set.transclr_property = EN_TRANSCLR_SET_COLOR;

    ioctl_info.en_pro = E_GOP_TRANSCOLOR;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (void*)&trans_set;
    ioctl_info.u32Size = sizeof(GOP_TRANSCOLOR);

    u32TransColor = clr;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_8888(MS_U32 clr, MS_U32 mask)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetTransClr_8888_EX(u8GOP,clr,mask);
    return ret;
}


E_GOP_API_Result MApi_GOP_GWIN_EnableT3DMode(MS_BOOL bEnable)
{
    GOP_MISC_PARAM ioctl_info;
    MS_BOOL  bEna;

    GOP_ENTRY();

    bEna = bEnable;
    ioctl_info.misc_type = E_GOP_MISC_ENABLE_T3D;
    ioctl_info.pMISC = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Set clock for gop dwin (for dwin source is OP)
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetClkForCapture(void)
{
    GOP_MISC_PARAM ioctl_info;
    MS_U32  value = 0;

    GOP_ENTRY();

    ioctl_info.misc_type = E_GOP_MISC_SET_CAPTURE_CLK;
    ioctl_info.pMISC = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

GOP_CAPTURE_INFO g_CaptureInfo;

/********************************************************************************/
/// Set gop dwin capture one frame
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_CaptureOneFrame(void)
{
    GOP_ENTRY();

    GOP_DWIN_CAPTURE_PARAM ioctl_info;
    GOP_CAPTURE_INFO dwin_info;

    ioctl_info.pDwin = (MS_U32*)&dwin_info;
    ioctl_info.u32Size = sizeof(GOP_CAPTURE_INFO);

    g_CaptureInfo.framecount = 1;
    g_CaptureInfo.bEn = FALSE;
    g_CaptureInfo.bWait = TRUE;
    memcpy(&dwin_info, &g_CaptureInfo, sizeof(GOP_CAPTURE_INFO));

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_CAPTURE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    MsOS_ReadMemory();  //make mips can read the dram without cache coherence issue.
    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Set gop dwin capture one frame
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_CaptureOneFrame2(void)
{
    GOP_ENTRY();

    GOP_DWIN_CAPTURE_PARAM ioctl_info;
    GOP_CAPTURE_INFO dwin_info;

    ioctl_info.pDwin = (MS_U32*)&dwin_info;
    ioctl_info.u32Size = sizeof(GOP_CAPTURE_INFO);

    g_CaptureInfo.framecount = 1;
    g_CaptureInfo.bEn = FALSE;
    g_CaptureInfo.bWait = FALSE;
    memcpy(&dwin_info, &g_CaptureInfo, sizeof(GOP_CAPTURE_INFO));

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_CAPTURE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    MsOS_ReadMemory();  //make mips can read the dram without cache coherence issue.
    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Config Dwin source (OP or MVOP).
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return GOP_API_SUCCESS - Success
///            GOP_API_INVALID_PARAMETERS - don't support this dwin source
/********************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetSourceSel(EN_GOP_DWIN_SRC_SEL enSrcSel)
{
    GOP_ENTRY();

    g_CaptureInfo.src = (EN_GOP_DWIN_SRC)enSrcSel;

    GOP_RETURN(GOP_API_SUCCESS);
}

/******************************************************************************/
/// Set interrupt mask of GOP DWIN.
/// @param u8mask \b IN bitmask of interrupt type
///   - bit7 DWIN VSYNC interrupt mask
///   - bit6 Dump Window interlace Bottom Field ACK Interrupt Mask
///   - bit5 Dump Window interlace Top Field ACK Interrupt Mask
///   - bit4 Dump Window Progressive ACK Interrupt Mask
///   - bit3:1 Reserved
///   - bit0 GWIN VSYNC Interrupt Mask
/// @param bEnable \b IN
///   - # TRUE enable interrupts specified by u8mask
///   - # FALSE disable interrupts specified by u8mask
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_EnableIntr(MS_U16 u8mask, MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_DWIN_INTR_STATUS_PARAM ioctl_info;
    GOP_DWIN_INTR_CTRL intrInfo;

    ioctl_info.intr_type = E_GOP_DWIN_INTR_CONTROL;
    ioctl_info.pIntrInfo = (MS_U32*)&intrInfo;
    ioctl_info.u32Size = sizeof(GOP_DWIN_INTR_CTRL);

    intrInfo.mask = u8mask;
    intrInfo.intrEn = (MS_U32)bEnable;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INTR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Set gop output color type
/// @param type \b IN: gop output color type
///   - # GOPOUT_RGB => RGB mode
///   - # GOPOUT_YUV => YUV mode
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_OutputColor_EX(MS_U8 u8GOP,EN_GOP_OUTPUT_COLOR type)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    EN_GOP_OUTPUT_COLOR  output;

    GOP_ENTRY();

    output = type;
    ioctl_info.en_pro   = E_GOP_OUTPUT_COLOR;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&output;
    ioctl_info.u32Size  = sizeof(EN_GOP_OUTPUT_COLOR);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_OutputColor(EN_GOP_OUTPUT_COLOR type)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_OutputColor_EX(u8GOP,type);
    return ret;
}


/********************************************************************************/
/// Get GOP Dwin interrupt status
/// @return GOP Dwin interrupt status
/********************************************************************************/
MS_U16 MApi_GOP_DWIN_GetIntrStatus(void)
{
    MS_U16 ret = 0;
    GOP_ENTRY();

    GOP_DWIN_INTR_STATUS_PARAM ioctl_info;
    MS_U16 intrStatus;

    ioctl_info.intr_type = E_GOP_DWIN_INTR_GETINFO;
    ioctl_info.pIntrInfo = (void*)&intrStatus;
    ioctl_info.u32Size = sizeof(MS_U16);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INTR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }


    GOP_RETURN(ret);

}


//******************************************************************************
/// clear DWIN interrupt
/// @param u8mask \b IN: interrup mask
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_ClearIntr(MS_U16 u8mask)
{
    GOP_ENTRY();

    GOP_DWIN_INTR_STATUS_PARAM ioctl_info;
    GOP_DWIN_INTR_CTRL intrInfo;

    ioctl_info.intr_type = E_GOP_DWIN_INTR_CONTROL;
    ioctl_info.pIntrInfo = (MS_U32*)&intrInfo;
    ioctl_info.u32Size = sizeof(GOP_DWIN_INTR_CTRL);

    intrInfo.mask = u8mask;
    intrInfo.intrEn  = 0x0;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INTR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// Get DWIN interrupt Information.
/// @param pDWinIntInfo \b OUT: dwin interrup flag
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
#define DWIN_TIMEOUT_CNT     60UL
E_GOP_API_Result MApi_GOP_DWIN_GetDWinIntInfoTimeout(GOP_DWinIntInfo *pDWinIntInfo,MS_U32 u32Timeout)
{
    MS_U16 ret = 0;
    GOP_ENTRY();

    GOP_DWIN_INTR_STATUS_PARAM ioctl_info;
    GOP_DWIN_INTR_TIME_OUT DWinIntinfoTimeout;

    memset(&DWinIntinfoTimeout, 0x0, sizeof(GOP_DWIN_INTR_TIME_OUT));
    memcpy(&DWinIntinfoTimeout.DWinIntInfo, pDWinIntInfo, sizeof(GOP_DWinIntInfo));
    DWinIntinfoTimeout.u32Timeout = u32Timeout;

    ioctl_info.intr_type = E_GOP_DWIN_INTR_GETINFO_TIMEOUT;
    ioctl_info.pIntrInfo = (MS_U32*)&DWinIntinfoTimeout;
    ioctl_info.u32Size = sizeof(GOP_DWIN_INTR_TIME_OUT);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INTR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    memcpy(pDWinIntInfo, &DWinIntinfoTimeout.DWinIntInfo, sizeof(GOP_DWinIntInfo));

    GOP_RETURN(ret);

}


//******************************************************************************
/// Get DWIN interrupt Information.
/// @param pDWinIntInfo \b OUT: dwin interrup flag
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_GetDWinIntInfo(GOP_DWinIntInfo *pDWinIntInfo)
{
    GOP_ENTRY();

    GOP_DWIN_INTR_STATUS_PARAM ioctl_info;
    GOP_DWIN_INTR_TIME_OUT DWinIntinfoTimeout;

    memset(&DWinIntinfoTimeout, 0x0, sizeof(GOP_DWIN_INTR_TIME_OUT));
    memcpy(&DWinIntinfoTimeout.DWinIntInfo, pDWinIntInfo, sizeof(GOP_DWinIntInfo));
    DWinIntinfoTimeout.u32Timeout = 0;

    ioctl_info.intr_type = E_GOP_DWIN_INTR_GETINFO_TIMEOUT;
    ioctl_info.pIntrInfo = (MS_U32*)&DWinIntinfoTimeout;
    ioctl_info.u32Size = sizeof(GOP_DWIN_INTR_TIME_OUT);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INTR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    memcpy(pDWinIntInfo, &DWinIntinfoTimeout.DWinIntInfo, sizeof(GOP_DWinIntInfo));

    GOP_RETURN(GOP_API_SUCCESS);

}
/******************************************************************************/
/// Enable/Disable gop alpha inverse
/// @param bEnable \b IN: TRUE or FALSE
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetAlphaInverse_EX(MS_U8 u8GOP,MS_BOOL bEnable)
{
    MS_BOOL bAlphaInv;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bAlphaInv = bEnable;


    ioctl_info.en_pro   = E_GOP_ALPHAINVERSE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bAlphaInv;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);


}

E_GOP_API_Result MApi_GOP_GWIN_SetAlphaInverse(MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetAlphaInverse_EX(u8GOP,bEnable);
    return ret;
}

/******************************************************************************/
/// Get gop alpha inverse setting
/// @param bEnable \b OUT: TRUE or FALSE
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetAlphaInverse_EX(MS_U8 u8GOP,MS_BOOL *bEnable)
{
    MS_BOOL bAlphaInv;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_ALPHAINVERSE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bAlphaInv;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    *bEnable      = bAlphaInv;

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_GetAlphaInverse(MS_BOOL *bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_GetAlphaInverse_EX(u8GOP,bEnable);
    return ret;
}

/********************************************************************************/
/// Set GOP bank force write mode for update register. When enable bank force write mode, update gop num registers action will directly
/// take effect (do not wait next v-sync to update gop register!).
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetBnkForceWrite(MS_U8 u8GOP, MS_BOOL bEnable)
{
    MS_BOOL bEna;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    CheckGOPInstanceOpen();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bEna = bEnable;

    ioctl_info.en_pro   = E_GOP_BANK_FORCE_WRITE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return GOP_API_SUCCESS;
}


/********************************************************************************/
/// Set GOP force write mode for update register. When enable force write mode, all update gop registers action will directly
/// take effect (do not wait next v-sync to update gop register!).
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetForceWrite(MS_BOOL bEnable)
{
    MS_BOOL bEna;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    CheckGOPInstanceOpen();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bEna = bEnable;

    ioctl_info.en_pro   = E_GOP_FORCE_WRITE;
    ioctl_info.gop_idx  = 0x0;
    ioctl_info.pSetting = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return GOP_API_SUCCESS;
}


/********************************************************************************/
/// Write optional palette values to GOP0 registers
/// Note that this GOP0 palette table only support 256 group palette data
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/// @param ePalType \b IN Pallete format, true/false ARGB8888
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetPaletteOpt_EX(MS_U8 u8GOP,GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType)
{
    GOP_PALETTE_SET_PARAM  ioctl_info;
    GOP_PALETTE_ENTRY  palEntry;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_SET_PARAM));
    memset(&palEntry, 0x0, sizeof(GOP_PALETTE_ENTRY));

    palEntry.gop_idx = u8GOP;
    palEntry.pClut = (void*)pPalArray;
    palEntry.start = u32PalStart;
    palEntry.end   = u32PalEnd;
    palEntry.palType = ePalType;
    palEntry.palSrc = E_GOP_4G_PALETTE;

    ioctl_info.pClut = (void*)&palEntry;
    ioctl_info.u32Size = sizeof(GOP_PALETTE_ENTRY);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_SET_ENTRY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_SetPaletteOpt( GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, EN_GOP_PALETTE_TYPE ePalType)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();//GOP0 support only(X) , STB is GOP2 support palette
    ret = MApi_GOP_GWIN_SetPaletteOpt_EX(u8GOP,pPalArray,u32PalStart,u32PalEnd,ePalType);
    return ret;

}
/********************************************************************************/
/// Set GOP0 palette table read out control
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetPaletteRead_EX(MS_U8 u8GOP,EN_GOP_PALETTE_READTYPE type)
{

    GOP_PALETTE_CONFIG_PARAM  ioctl_info;
    GOP_PAL_OPT  palOpt;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_CONFIG_PARAM));
    memset(&palOpt, 0x0, sizeof(GOP_PAL_OPT));

    palOpt.gop_idx = u8GOP;
    palOpt.paltype = type;
    palOpt.palSrc = E_GOP_4G_PALETTE;

    ioctl_info.gop_idx = palOpt.gop_idx;
    ioctl_info.pPaletteInfo = (MS_U32*)&palOpt;
    ioctl_info.u32Size = sizeof(GOP_PAL_OPT);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_SET_CONFIG,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}
E_GOP_API_Result MApi_GOP_GWIN_SetPaletteRead(EN_GOP_PALETTE_READTYPE type)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetPaletteRead_EX(u8GOP,type);
    return ret;
}
/********************************************************************************/
/// Read GOP0 palette values From Pallete
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_ReadPalette_EX(MS_U8 u8GOP,MS_U8 idx, MS_U32* clr)
{
    GOP_PALETTE_SET_PARAM  ioctl_info;
    GOP_PALETTE_ENTRY  palEntry;
    MS_U32 u32Color;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_SET_PARAM));
    memset(&palEntry, 0x0, sizeof(GOP_PALETTE_ENTRY));

    palEntry.gop_idx = u8GOP;
    palEntry.pClut = (MS_U32*)&u32Color;
    palEntry.start = idx;
    palEntry.end   = idx+1;
    palEntry.palType = E_GOP_PAL_ARGB8888;
    palEntry.palSrc = E_GOP_4G_PALETTE;

    ioctl_info.pClut = (void*)&palEntry;
    ioctl_info.u32Size = sizeof(GOP_PALETTE_ENTRY);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_GET_ENTRY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    *clr = u32Color;
    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_ReadPalette(MS_U8 idx, MS_U32* clr)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_ReadPalette_EX(u8GOP,idx,clr);
    return ret;
}

/********************************************************************************/
/// Write optional palette values to GOP1 registers
/// Note that this GOP1 palette table only support 64 group palette data
/// @param *pPalArray \b IN Pointer to pallete list gonna set into pallete table
/// @param u32PalStart \b IN start of pallete table gonna be overwrite
/// @param u32PalEnd \b IN end of pallete table gonna be overwrite
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteOpt_EX(MS_U8 u8GOP,GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
#ifndef CONFIG_GOP_2ND_PAL
    GOP_ERR("\33[0;36m   %s: #define CONFIG_GOP_2ND_PAL not opened\33[m \n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
#else
    GOP_PALETTE_SET_PARAM  ioctl_info;
    GOP_PALETTE_ENTRY  palEntry;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_SET_PARAM));
    memset(&palEntry, 0x0, sizeof(GOP_PALETTE_ENTRY));

    palEntry.gop_idx = u8GOP;
    palEntry.pClut = (MS_U32*)(void*)pPalArray;
    palEntry.start = u32PalStart;
    palEntry.end   = u32PalEnd;
    palEntry.palType = E_GOP_PAL_ARGB8888;
    palEntry.palSrc = E_GOP_2G_PALETTE;

    ioctl_info.pClut = (void*)&palEntry;
    ioctl_info.u32Size = sizeof(GOP_PALETTE_ENTRY);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_SET_ENTRY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
#endif
}
E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteOpt(GOP_PaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_2GSetPaletteOpt_EX(u8GOP,pPalArray,u32PalStart,u32PalEnd);
    return ret;

}
/********************************************************************************/
/// Set GOP1 palette table read out control
/// @param type \b IN \copydoc EN_GOP_PALETTE_READTYPE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteRead_EX(MS_U8 u8GOP,EN_GOP_PALETTE_READTYPE type)
{
#ifndef CONFIG_GOP_2ND_PAL
    GOP_ERR("\33[0;36m   %s: #define CONFIG_GOP_2ND_PAL not opened\33[m \n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
#else
    GOP_PALETTE_CONFIG_PARAM  ioctl_info;
    GOP_PAL_OPT  palOpt;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_CONFIG_PARAM));
    memset(&palOpt, 0x0, sizeof(GOP_PAL_OPT));

    palOpt.gop_idx = u8GOP;
    palOpt.paltype = type;
    palOpt.palSrc = E_GOP_2G_PALETTE;

    ioctl_info.gop_idx = palOpt.gop_idx;
    ioctl_info.pPaletteInfo = (MS_U32*)&palOpt;
    ioctl_info.u32Size = sizeof(GOP_PAL_OPT);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_SET_CONFIG,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

#endif
}
E_GOP_API_Result MApi_GOP_GWIN_2GSetPaletteRead(EN_GOP_PALETTE_READTYPE type)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_2GSetPaletteRead_EX(u8GOP,type);
    return ret;
}
/********************************************************************************/
/// Read GOP2G(GOP1) palette values From Pallete
/// @param idx \b IN Index for pallete wanna retrieve
/// @param clr \b OUT color of specific pallete index
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_2GReadPalette_EX(MS_U8 u8GOP,MS_U8 idx, MS_U32* clr)
{
#ifndef CONFIG_GOP_2ND_PAL
    GOP_ERR("\33[0;36m   %s: #define CONFIG_GOP_2ND_PAL not opened\33[m \n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
#else
    GOP_PALETTE_SET_PARAM  ioctl_info;
    GOP_PALETTE_ENTRY  palEntry;
    MS_U32 u32Color;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_SET_PARAM));
    memset(&palEntry, 0x0, sizeof(GOP_PALETTE_ENTRY));

    palEntry.gop_idx = u8GOP;
    palEntry.pClut = (MS_U32*)&u32Color;
    palEntry.start = idx;
    palEntry.end   = idx+1;
    palEntry.palType = E_GOP_PAL_ARGB8888;
    palEntry.palSrc = E_GOP_2G_PALETTE;

    ioctl_info.pClut = (void*)&palEntry;
    ioctl_info.u32Size = sizeof(GOP_PALETTE_ENTRY);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_GET_ENTRY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    *clr = u32Color;
    GOP_RETURN(GOP_API_SUCCESS);
#endif
}
E_GOP_API_Result MApi_GOP_GWIN_2GReadPalette(MS_U8 idx, MS_U32* clr)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_2GReadPalette_EX(u8GOP,idx,clr);
    return ret;
}
/********************************************************************************/
/// Set stretch window H-Stretch ratio.
/// Example: gwin size:960*540  target gwin size: 1920*1080
///     step1: MApi_GOP_GWIN_Set_HSCALE(TRUE, 960, 1920);
///     step2: MApi_GOP_GWIN_Set_VSCALE(TRUE, 540, 1080);
///     step3: MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_OP0, 0, 0, 960, 540);
/// @param bEnable \b IN:
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN: original size
/// @param dst \b IN: target size
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_HSCALE_EX(MS_U8 u8GOP,MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.SrcRect.w = src;
    if (bEnable)
    {
        stretch_info.DstRect.w = dst;
    }
    else
    {
        stretch_info.DstRect.w = src;
    }
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.enStrtchType = E_GOP_STRETCH_HSCALE;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_Set_HSCALE(MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_Set_HSCALE_EX(u8GOP,bEnable,src,dst);
    return ret;
}
/********************************************************************************/
/// Set stretch window V-Stretch ratio.
/// Example: gwin size:960*540  target gwin size: 1920*1080
///     step1: MApi_GOP_GWIN_Set_HSCALE(TRUE, 960, 1920);
///     step2: MApi_GOP_GWIN_Set_VSCALE(TRUE, 540, 1080);
///     step3: MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_OP0, 0, 0, 960, 540);
/// @param bEnable \b IN:
///   - # TRUE enable
///   - # FALSE disable
/// @param src \b IN: original size
/// @param dst \b IN: target size
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_VSCALE_EX(MS_U8 u8GOP,MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.SrcRect.h = src;
    if (bEnable)
    {
        stretch_info.DstRect.h = dst;
    }
    else
    {
        stretch_info.DstRect.h = src;
    }
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.enStrtchType = E_GOP_STRETCH_VSCALE;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_Set_VSCALE(MS_BOOL bEnable, MS_U16 src, MS_U16 dst)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_Set_VSCALE_EX(u8GOP,bEnable,src,dst);
    return ret;
}
/********************************************************************************/
/// Set GOP H stretch mode
/// @param HStrchMode \b IN:
///   - # E_GOP_HSTRCH_6TAPE
///   - # E_GOP_HSTRCH_DUPLICATE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_HStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRETCH_HMODE HStrchMode)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.enHMode = HStrchMode;

    ioctl_info.gop_idx = u8GOP;
    ioctl_info.enStrtchType = E_GOP_STRETCH_HSTRETCH_MODE;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_Set_HStretchMode(EN_GOP_STRETCH_HMODE HStrchMode)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_Set_HStretchMode_EX(u8GOP,HStrchMode);
    return ret;
}

/********************************************************************************/
/// Set GOP Transparent color Stretch Mode
/// @param VStrchMode \b IN:
///   - # E_GOP_TRANSPCOLOR_STRCH_DUPLICATE
///   - # E_GOP_TRANSPCOLOR_STRCH_ASNORMAL
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_TranspColorStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRCH_TRANSPCOLORMODE  TColorStrchMode)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.enTColorMode = TColorStrchMode;

    ioctl_info.gop_idx = u8GOP;
    ioctl_info.enStrtchType = E_GOP_STRETCH_TRANSPCOLOR_MODE;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_Set_TranspColorStretchMode(EN_GOP_STRCH_TRANSPCOLORMODE  TColorStrchMode)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_Set_TranspColorStretchMode_EX(u8GOP,TColorStrchMode);
    return ret;
}

/********************************************************************************/
/// Set GOP V stretch mode
/// @param VStrchMode \b IN:
///   - # E_GOP_VSTRCH_LINEAR
///   - # E_GOP_VSTRCH_DUPLICATE
///   - # E_GOP_VSTRCH_NEAREST
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_VStretchMode_EX(MS_U8 u8GOP,EN_GOP_STRETCH_VMODE VStrchMode)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.enVMode = VStrchMode;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.enStrtchType = E_GOP_STRETCH_VSTRETCH_MODE;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_Set_VStretchMode(EN_GOP_STRETCH_VMODE VStrchMode)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_Set_VStretchMode_EX(u8GOP,VStrchMode);
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// set gop destination path clock
/// @param gopNum \b IN: Number of GOP
/// @param eDstType \b IN: gop destination type
/// @return GOP_API_SUCCESS - Success
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MApi_GOP_SetGOPClk(MS_U8 gopNum, EN_GOP_DST_TYPE eDstType)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    EN_GOP_DST_TYPE eType;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    eType = eDstType;


    ioctl_info.en_pro   = E_GOP_CLK;
    ioctl_info.gop_idx  = gopNum;
    ioctl_info.pSetting = (void*)&eType;
    ioctl_info.u32Size  = sizeof(EN_GOP_DST_TYPE);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


//******************************************************************************
/// Enable gop progressive mode
/// @param bEnable \b IN:
///   - # TRUE Enable gop progressive mode
///   - # FALSE Disable gop progressive mode
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_EnableProgressive_EX(MS_U8 u8GOP,MS_BOOL bEnable)
{

    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32  Progressive;

    GOP_ENTRY();

    Progressive = bEnable;

    ioctl_info.en_pro   = E_GOP_PROGRESSIVE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&Progressive;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_EnableProgressive(MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_EnableProgressive_EX(u8GOP,bEnable);
    return ret;

}

//******************************************************************************
/// Set gop update register method by only once.
/// Example: if you want to update GOP function A, B, C in the same V sync, please write down your code like below
/// MApi_GOP_GWIN_UpdateRegOnce(TRUE);
/// GOP_FUN_A;
/// GOP_FUN_B;
/// GOP_FUN_C;
/// MApi_GOP_GWIN_UpdateRegOnce(FALSE);
/// @param bWriteRegOnce \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnce(MS_BOOL bWriteRegOnce)
{
    return MApi_GOP_GWIN_UpdateRegOnceEx(bWriteRegOnce, TRUE);
}

//******************************************************************************
/// Extend MApi_GOP_GWIN_UpdateRegOnceEx, update special gop.
/// Set gop update register method by only once.
/// Example: if you want to update GOP function A, B, C in the same V sync, please write down your code like below
/// MApi_GOP_GWIN_UpdateRegOnceByMask(u16GopMask,TRUE, TRUE);
/// GOP_FUN_A;
/// GOP_FUN_B;
/// GOP_FUN_C;
/// MApi_GOP_GWIN_UpdateRegOnceByMask(u16GopMask,FALSE, TRUE);
/// @param u16GopMask    \b IN:bit0-gop0, bit1-gop1...
/// @param bWriteRegOnce    \b IN: TRUE/FALSE
/// @param bSync            \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceByMask(MS_U16 u16GopMask,MS_BOOL bWriteRegOnce, MS_BOOL bSync)
{
    GOP_UPDATE_PARAM ioctl_info;
    GOP_UPDATE_INFO update;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_UPDATE_PARAM));
    memset(&update, 0x0, sizeof(GOP_UPDATE_INFO));

    update.gop_idx = u16GopMask;
    update.update_type = E_GOP_UPDATE_CURRENT_ONCE;
    update.bEn = bWriteRegOnce;
    update.bSync = bSync;

    ioctl_info.pUpdateInfo = (MS_U32*)&update;
    ioctl_info.u32Size = sizeof(GOP_UPDATE_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_UPDATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// Extend MApi_GOP_GWIN_UpdateRegOnce, add parameter bSync.
/// Set gop update register method by only once.
/// Example: if you want to update GOP function A, B, C in the same V sync, please write down your code like below
/// MApi_GOP_GWIN_UpdateRegOnceEx(TRUE, TRUE);
/// GOP_FUN_A;
/// GOP_FUN_B;
/// GOP_FUN_C;
/// MApi_GOP_GWIN_UpdateRegOnceEx(FALSE, TRUE);
/// @param bWriteRegOnce    \b IN: TRUE/FALSE
/// @param bSync            \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceEx2(MS_U8 u8GOP,MS_BOOL bWriteRegOnce, MS_BOOL bSync)
{
    GOP_UPDATE_PARAM ioctl_info;
    GOP_UPDATE_INFO update;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_UPDATE_PARAM));
    memset(&update, 0x0, sizeof(GOP_UPDATE_INFO));

    update.gop_idx = u8GOP;
    update.update_type = E_GOP_UPDATE_ONCE;
    update.bEn = bWriteRegOnce;
    update.bSync = bSync;

    ioctl_info.pUpdateInfo = (MS_U32*)&update;
    ioctl_info.u32Size = sizeof(GOP_UPDATE_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_UPDATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_UpdateRegOnceEx(MS_BOOL bWriteRegOnce, MS_BOOL bSync)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_UpdateRegOnceEx2(u8GOP,bWriteRegOnce,bSync);
    return ret;

}

//******************************************************************************
/// API for initialize the GOP DWIN
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_Init(void)
{
    MS_U32 ioctl_info = 0;

    CheckGOPInstanceOpen();
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_INIT,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
    bInit =TRUE;
    return GOP_API_SUCCESS;
}

//******************************************************************************
/// API for enable or disable DWIN
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_Enable(MS_BOOL bEnable)
{
    GOP_ENTRY();


    GOP_DWIN_CAPTURE_PARAM ioctl_info;
    GOP_CAPTURE_INFO dwin_info;
    MS_U32 bpp;
    MS_U16 fCnt;


    ioctl_info.pDwin = (MS_U32*)&dwin_info;
    ioctl_info.u32Size = sizeof(GOP_CAPTURE_INFO);

    g_CaptureInfo.bEn = bEnable;

    if(g_CaptureInfo.fmt == GOP_DWIN_FMT_ARGB8888)
        bpp = 4;
    else
        bpp = 2;

    if(g_CaptureInfo.rect.w*g_CaptureInfo.rect.h *bpp ==0)
    {
        fCnt=0;
    }
    else
    {
        fCnt = (g_CaptureInfo.addr1[0]-g_CaptureInfo.addr[0])/(g_CaptureInfo.rect.w*g_CaptureInfo.rect.h *bpp);
    }
    g_CaptureInfo.framecount = fCnt;


    memcpy(&dwin_info, &g_CaptureInfo, sizeof(GOP_CAPTURE_INFO));

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_CAPTURE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// API for enable or disable DWIN
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_EnableR2YCSC(MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    MS_BOOL bEna;

    bEna = bEnable;
    ioctl_info.en_dwin_property = E_GOP_DWIN_R2Y;
    ioctl_info.pDwinProp = (void*)&bEna;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


//******************************************************************************
/// API for enable or disable DWIN UV SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_SetUVSwap(MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    MS_BOOL bEna;

    bEna = bEnable;
    ioctl_info.en_dwin_property = E_GOP_DWIN_UV_SWAP;
    ioctl_info.pDwinProp = (void*)&bEna;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


//******************************************************************************
/// Get DWIN property
/// @param pproperty \b OUT buffer to store DWIN property
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_GetWinProperty(GOP_DwinProperty *pproperty)
{
    CheckGOPInstanceOpen();
    pproperty->u16x = g_CaptureInfo.rect.x;
    pproperty->u16y = g_CaptureInfo.rect.y;
    pproperty->u16w = g_CaptureInfo.rect.w;
    pproperty->u16h = g_CaptureInfo.rect.h;
    pproperty->u16fbw = g_CaptureInfo.pitch;

    pproperty->u32fbaddr0 = g_CaptureInfo.addr[0];
    pproperty->u32fbaddr1 = g_CaptureInfo.addr1[0];

    return GOP_API_SUCCESS;
}

//******************************************************************************
/// Set DWIN property
/// @param pproperty \b IN buffer to store DWIN property
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_SetWinProperty(GOP_DwinProperty *pproperty)
{
    GOP_ENTRY();

    if((pproperty->u16h <=0) || (pproperty->u16w<=0))
    {
        GOP_ERR("\n %s DWIN Width(%d) or Height(%d) <= 0\n", __FUNCTION__,pproperty->u16w,pproperty->u16h);
        GOP_RETURN(GOP_API_FAIL);
    }

    g_CaptureInfo.rect.x = pproperty->u16x;
    g_CaptureInfo.rect.y = pproperty->u16y;
    g_CaptureInfo.rect.w = pproperty->u16w;
    g_CaptureInfo.rect.h = pproperty->u16h;
    g_CaptureInfo.pitch  = pproperty->u16fbw;

    g_CaptureInfo.addr[0] = pproperty->u32fbaddr0;
    g_CaptureInfo.addr1[0] = pproperty->u32fbaddr1;

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// Set DWIN data source scan type
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_SelectSourceScanType(EN_GOP_DWIN_SCAN_MODE enScanMode)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    EN_GOP_DWIN_SCAN_MODE enMode;

    enMode = enScanMode;
    ioctl_info.en_dwin_property = E_GOP_DWIN_SOURCE_SCAN_TYPE;
    ioctl_info.pDwinProp = (MS_U32*)&enMode;
    ioctl_info.u32Size = sizeof(EN_GOP_DWIN_SCAN_MODE);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// Set DWIN Capture format
/// @param fmt \b IN: buffer format to store DWIN content
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_DWIN_SetDataFmt(EN_GOP_DWIN_DATA_FMT fmt)
{
    GOP_ENTRY();

    g_CaptureInfo.fmt = (EN_GOP_DWIN_FMT)fmt;

    GOP_RETURN(GOP_API_SUCCESS);

}

/*****************************************************************************/
/// Set DWIN Capture constant alpha value for format ARGB8888
/// @param u8AlphaVal \b IN: constant alpha value
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetAlphaValue(MS_U8 u8AlphaVal)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    MS_U8 u8Value;

    u8Value = u8AlphaVal;
    ioctl_info.en_dwin_property = E_GOP_DWIN_ALPHA_VALUE;
    ioctl_info.pDwinProp = (void*)&u8Value;
    ioctl_info.u32Size = sizeof(MS_U8);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*****************************************************************************/
/// Set DWIN Capture alpha source
/// @param source \b IN: alpha source
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetAlphaSrc(EN_GOP_DWIN_ALPHA_SRC source)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    EN_GOP_DWIN_ALPHA_SRC enSource;

    enSource = source;
    ioctl_info.en_dwin_property = E_GOP_DWIN_ALPHA_SOURCE;
    ioctl_info.pDwinProp = (MS_U32*)&enSource;
    ioctl_info.u32Size = sizeof(EN_GOP_DWIN_ALPHA_SRC);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*****************************************************************************/
/// Set DWIN Capture alpha inverse
/// @param source \b IN: enable alpha inverse
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetAlphaInverse(MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    MS_BOOL bEna;

    bEna = bEnable;
    ioctl_info.en_dwin_property = E_GOP_DWIN_ALPHA_INVERSE;
    ioctl_info.pDwinProp = (void*)&bEna;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*****************************************************************************/
/// Set DWIN UVSample mode
/// @param mode \b IN: UVSample mode
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetUVSample(EN_GOP_DWIN_UVSAMPLE_Mode mode)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    EN_GOP_DWIN_UVSAMPLE_Mode enMode;

    enMode = mode;
    ioctl_info.en_dwin_property = E_GOP_DWIN_UV_SAMPLE;
    ioctl_info.pDwinProp = (MS_U32*)&enMode;
    ioctl_info.u32Size = sizeof(EN_GOP_DWIN_UVSAMPLE_Mode);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*****************************************************************************/
/// Set DWIN skipframe count
/// @param mode \b IN: skipframe count
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_DWIN_SetSkipFrame(MS_U32 framecount)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    MS_U32 u32Count;

    u32Count = framecount;
    ioctl_info.en_dwin_property = E_GOP_DWIN_SKIP_FRAME;
    ioctl_info.pDwinProp = (MS_U32*)&u32Count;
    ioctl_info.u32Size = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


//-------------------------------------------------------------------------------------------------
/// Get current active gop
/// @return current gop number
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_GOP_GWIN_GetCurrentGOP(void)
{
    GOP_GET_STATUS_PARAM ioctl_info;
    MS_U32 u32Gop = INVALID_GOP_NUM;
    GOP_ENTRY();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_CURRENT_GOP;
    ioctl_info.pStatus = (MS_U32*) &u32Gop;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN((MS_U8)(u32Gop&0xFF));
}

/********************************************************************************/
/// Set GWIN information to GOP registers
/// @param u8win \b IN: GWINID
/// @param pinfo \b IN: GWIN Information
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetWinInfo(MS_U8 u8win, GOP_GwinInfo* pinfo)
{
    GOP_GWIN_SETWININFO_PARAM ioctl_info;
    GOP_BUFFER_INFO win_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&win_info, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.GwinId = u8win;
    ioctl_info.pinfo = (MS_U32*)&win_info;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);


    win_info.addr = pinfo->u32DRAMRBlkStart;
    win_info.disp_rect.x = pinfo->u16DispHPixelStart;
    win_info.disp_rect.y = pinfo->u16DispVPixelStart;
    win_info.disp_rect.w = pinfo->u16DispHPixelEnd - pinfo->u16DispHPixelStart;
    win_info.disp_rect.h = pinfo->u16DispVPixelEnd - pinfo->u16DispVPixelStart;
    win_info.width  = pinfo->u16RBlkHPixSize;
    win_info.height = pinfo->u16RBlkVPixSize;
    win_info.fbFmt  = pinfo->clrType;
    win_info.pitch  = pinfo->u16RBlkHRblkSize;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_WININFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/******************************************************************************/
/// Get current frame buffer pool id
/// @return current frame buffer pool id. 0: first frame buffer pool, 1: second frame buffer pool
/******************************************************************************/
MS_U8 MApi_GOP_FB_Get(void)
{
    GOP_FB_PROPERTY_PARAM ioctl_info;
    MS_U8 u8PoolId = 0x0;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_FB_POOLID;
    ioctl_info.FBId = 0x0;
    ioctl_info.pSet = (void*)&u8PoolId;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u8PoolId);
    }

    GOP_RETURN(u8PoolId);
}

//******************************************************************************
/// Set GWin Attribute to Shared. If shared GWin, More than one process could
/// access this GWin.
/// @param winId \b IN: GWIN ID for shared
/// @param bIsShared \b IN: shared or not
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetGWinShared(MS_U8 winId, MS_BOOL bIsShared)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    MS_BOOL bShared;

    GOP_ENTRY();

    bShared = bIsShared;
    ioctl_info.en_property = E_GOP_GWIN_SHARE;
    ioctl_info.GwinId = winId;
    ioctl_info.pSet = (void*)&bShared;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// Set Reference cnt of shared GWin.
/// @param winId \b IN: GWIN ID for shared
/// @param u16SharedCnt \b IN: shared reference cnt.
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetGWinSharedCnt(MS_U8 winId, MS_U16 u16SharedCnt)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    MS_U16 u16Cnt;

    GOP_ENTRY();

    u16Cnt = u16SharedCnt;
    ioctl_info.en_property = E_GOP_GWIN_SHARE_CNT;
    ioctl_info.GwinId = winId;
    ioctl_info.pSet = (void*)&u16Cnt;
    ioctl_info.u32Size = sizeof(MS_U16);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// Create GWIN
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: frame buffer pixel format
///   - #0xFFFF default FB format
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateWin(MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    MS_U32 u32FBId = INVALID_POOL_NEXT_FBID;
    MS_U8 winId =INVALID_GWIN_ID;
    GOP_ENTRY();

    u32FBId = MApi_GOP_GWIN_GetFree32FBID();

    MApi_GOP_GWIN_Create32FB(u32FBId,0,0,width,height,fbFmt);

    winId = MApi_GOP_GWIN_GetFreeWinID();

    MApi_GOP_GWIN_Map32FB2Win(u32FBId,winId);

    GOP_RETURN(winId);
}


//******************************************************************************
/// Create GWIN
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: frame buffer pixel format
///   - #0xFFFF default FB format
/// @param FBString \b IN: open by which one application
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateWin2(MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString)
{
    MS_U32 u32FBId = INVALID_POOL_NEXT_FBID;
    MS_U8 winId =INVALID_GWIN_ID;
    GOP_ENTRY();

    u32FBId = MApi_GOP_GWIN_GetFree32FBID();

    MApi_GOP_GWIN_Create32FB2_EXT(u32FBId, 0, 0, width, height, fbFmt ,FBString);

    winId = MApi_GOP_GWIN_GetFreeWinID();

    MApi_GOP_GWIN_Map32FB2Win(u32FBId,winId);

    GOP_RETURN(winId);


}


//******************************************************************************
/// Create GWIN and assign frame buffer to this GWIN
/// @param u8GOP \b IN: GOP number
/// @param FBId \b IN: frame buffer ID
/// @param dispX \b IN: frame buffer starting x position
/// @param dispY \b IN: frame buffer starting y position
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateWin_Assign_FB(MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY)
{
    return MApi_GOP_GWIN_CreateWin_Assign_32FB(u8GOP, (MS_U32)FBId, dispX, dispY);
}

MS_U8 MApi_GOP_GWIN_CreateWin_Assign_32FB(MS_U8 u8GOP, MS_U32 u32FBId, MS_U16 dispX, MS_U16 dispY)
{
    MS_U8 winId =INVALID_GWIN_ID;
    GOP_ENTRY();

    MApi_GOP_GWIN_SwitchGOP(u8GOP);

    winId = MApi_GOP_GWIN_GetFreeWinID();

    MApi_GOP_GWIN_Map32FB2Win(u32FBId,winId);

    GOP_RETURN(winId);

}

//******************************************************************************
/// Create given GWIN id
/// @param u8WinId \b IN: GWIN id
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: GWIN color format
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateStaticWin(MS_U8 u8WinId, MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    E_GOP_API_Result eRet=GOP_API_FAIL;
    MS_U32 u32FBId = INVALID_POOL_NEXT_FBID;

    GOP_ENTRY();
    u32FBId = MApi_GOP_GWIN_GetFree32FBID();
    MApi_GOP_GWIN_Create32FB(u32FBId,0,0,width,height,fbFmt);
    eRet = MApi_GOP_GWIN_Map32FB2Win(u32FBId,u8WinId);
    GOP_RETURN(eRet);
}


//******************************************************************************
/// Create GWIN by given GWIN id
/// @param u8WinId \b IN: GWIN id
/// @param width \b IN: GWIN width
/// @param height \b IN: GWIN height
/// @param fbFmt \b IN: GWIN color format
/// @param FBString \b IN: open by which one application
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateStaticWin2(MS_U8 u8WinId, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString)
{
    E_GOP_API_Result eRet=GOP_API_FAIL;
    MS_U32 u32FBId = INVALID_POOL_NEXT_FBID;

    GOP_ENTRY();
    u32FBId = MApi_GOP_GWIN_GetFree32FBID();
    MApi_GOP_GWIN_Create32FB2_EXT(u32FBId, 0, 0, width, height, fbFmt ,FBString);
    eRet = MApi_GOP_GWIN_Map32FB2Win(u32FBId,u8WinId);
    GOP_RETURN(eRet);

}


//******************************************************************************
/// Create GWIN by given GWIN id and frame buffer id
/// @param u8WinId \b IN: GWIN id
/// @param u8GOP \b IN: GOP number
/// @param FBId \b IN: frame buffer id
/// @param dispX \b IN: gwin's frame buffer starting x
/// @param dispY \b IN: gwin's frame buffer starting y
/// @return The id for the created GWIN, if return 0xff, it represents create gwin fail
//******************************************************************************
MS_U8 MApi_GOP_GWIN_CreateStaticWin_Assign_FB(MS_U8 u8WinId, MS_U8 u8GOP, MS_U8 FBId, MS_U16 dispX, MS_U16 dispY)
{
    return MApi_GOP_GWIN_CreateStaticWin_Assign_32FB(u8WinId, u8GOP, (MS_U32)FBId, dispX, dispY);
}

MS_U8 MApi_GOP_GWIN_CreateStaticWin_Assign_32FB(MS_U8 u8WinId, MS_U8 u8GOP, MS_U32 u32FBId, MS_U16 dispX, MS_U16 dispY)
{
    GOP_ENTRY();

    MApi_GOP_GWIN_Map32FB2Win(u32FBId,u8WinId);

    GOP_RETURN(u8WinId);
}


/******************************************************************************/
/// Check if GWIN is created
/// @param gId \b IN: GWINID
/// @return TRUE or FALSE
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_IsGwinExist(MS_U8 gId)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    MS_U8 u8Exist = FALSE;

    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_EXIST;
    ioctl_info.GwinId = gId;
    ioctl_info.pSet = (void*)&u8Exist;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u8Exist);
    }

    GOP_RETURN(u8Exist);
}

/******************************************************************************/
/// Get total active gwin number
/// @return total active gwin number
/******************************************************************************/
MS_U16   MApi_GOP_GWIN_GetActiveGWIN(void)
{
    MS_U8 gwinNum = 0;
    MS_U8 i;
    GOP_ENTRY();

    for(i = 0; i < MAX_GWIN_SUPPORT ; i++)
    {
        if(MApi_GOP_GWIN_IsGWINEnabled(i) == TRUE)
        {
            gwinNum |= 1<<i;
        }
    }
    GOP_RETURN(gwinNum);

}

//******************************************************************************
/// Enable GWIN for display
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Show GWIN
///   - # FALSE Hide GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_Enable(MS_U8 winId, MS_BOOL bEnable)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_ENABLE;
    ioctl_info.GwinId = winId;
    ioctl_info.pSet = (void*) &bEnable;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Check if GWIN is enabled
/********************************************************************************/
MS_BOOL MApi_GOP_GWIN_IsEnabled(void)
{
    MS_U8 i = 0;
    MS_BOOL bEnable =FALSE;
    GOP_ENTRY();

    while(i<MAX_GWIN_SUPPORT)
    {
        bEnable = MApi_GOP_GWIN_IsGWINEnabled(i);
        if (bEnable==TRUE)
        {
            GOP_RETURN(TRUE);
        }
        i++;
    }
    GOP_RETURN(FALSE);
}

//******************************************************************************
/// Check if all GWIN is currently enabled
/// @return  - # TRUE GWins are enabled
/// @return  - # FALSE not all GWins are enabled
//******************************************************************************
MS_BOOL MApi_GOP_GWIN_IsAllGWINDisabled(void)
{
    MS_BOOL ret;
    GOP_ENTRY();

    ret =  MApi_GOP_GWIN_IsEnabled();
    GOP_RETURN(ret);

}

//******************************************************************************
/// Check if all some GWIN is currently enabled
/// @param  winId \b IN: gwin id
/// @return  - the according GWin is enabled or not
//******************************************************************************
MS_BOOL MApi_GOP_GWIN_IsGWINEnabled(MS_U8 winId)
{
    MS_BOOL bEn;
    GOP_GWIN_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    ioctl_info.en_property = E_GOP_GWIN_ENABLE;
    ioctl_info.GwinId = winId;
    ioctl_info.pSet = (void*) &bEn;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(bEn);


}

//******************************************************************************
/// [Obsolete Function]
/// @param hext \b IN
///   - 0 disable horizontal pixel duplication
///   - 1 enable  horizontal pixel duplication
/// @param vext \b IN
///   - 0 disable vertical line duplication
///   - 1 enable  vertical line duplication
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetDuplication(MS_U8 hext, MS_U8 vext)
{
//NOT USE FOR COVERITY HAPPY -_-~
// THE CODE BELOW is useless
    GOP_ENTRY();
    UNUSED(hext);
    UNUSED(vext);
    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Delete the GWIN, free corresponding frame buffer
/// @param winId \b IN GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_DeleteWin(MS_U8 winId)
{
    GOP_GWIN_DESTROY_PARAM ioctl_info;
    MS_U32 u32FBID=DRV_MAX_GWIN_FB_SUPPORT;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_DESTROY_PARAM));
    ioctl_info.GwinId = winId;

    u32FBID = MApi_GOP_GWIN_Get32FBfromGWIN(winId);
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_DESTROY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    if(MApi_GOP_GWIN_Is32FBExist(u32FBID)==TRUE)
    {
        MApi_GOP_GWIN_Destroy32FB(u32FBID);
    }
    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// Set the GWIN's position
/// @param winId  \b IN: GWIN id
/// @param dispX \b IN: x coordinate
/// @param dispY \b IN: y coordinate
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetWinPosition(MS_U8 winId, MS_U16 dispX, MS_U16 dispY)
{

    GOP_GWIN_SETWININFO_PARAM ioctl_info;
    GOP_BUFFER_INFO win_info;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&win_info, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.GwinId = winId;
    ioctl_info.pinfo = (MS_U32*)&win_info;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_WININFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }


    win_info.disp_rect.x = dispX;
    win_info.disp_rect.y = dispY;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_WININFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// [Obstacle Function]
/// @return 0
/******************************************************************************/
MS_U16 MApi_GOP_GWIN_Get_HSTART(void)
{
    return 0;
}


/******************************************************************************/
/// Set the GWIN's frame buffer offset
/// @param winId  \b IN: GWIN id
/// @param x \b IN: x coordinate
/// @param y \b IN: y coordinate
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetWinPositionOffset(MS_U8 winId, MS_U16 x, MS_U16 y)
{
    GOP_GwinInfo gwin;
    GOP_ENTRY();
    memset(&gwin, 0, sizeof(GOP_GwinInfo));
    MApi_GOP_GWIN_GetWinInfo(winId, &gwin);
    gwin.u16WinX = x;
    gwin.u16WinY = y;
    MApi_GOP_GWIN_SetWinInfo(winId, &gwin);
    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Set gwin property
/// @param WinProperty  \b IN: gwin property info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetWinProperty(GOP_GwinDispProperty WinProperty)
{
    GOP_GwinInfo gWin;
    MS_U32 u32fbId;
    GOP_GwinFBAttr  fbInfo;

    GOP_ENTRY();

    u32fbId = MApi_GOP_GWIN_GetFBfromGWIN(WinProperty.gId);

    memset(&gWin, 0, sizeof(GOP_GwinInfo));
    memset(&fbInfo, 0x0, sizeof(GOP_GwinFBAttr));

    if(MApi_GOP_GWIN_GetWinInfo(WinProperty.gId,&gWin)!=GOP_API_SUCCESS)
    {
        GOP_WARN("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
    }
    if(MApi_GOP_GWIN_Get32FBInfo(u32fbId,  &fbInfo)!=GOP_API_SUCCESS)
    {
        GOP_WARN("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
    }

    gWin.u16DispHPixelStart = WinProperty.StartX;
    gWin.u16DispVPixelStart = WinProperty.StartY;
    gWin.u16DispHPixelEnd = WinProperty.StartX + WinProperty.Win_Width;
    gWin.u16DispVPixelEnd = WinProperty.StartY + WinProperty.Win_Height;
    gWin.u16WinX = WinProperty.FBOffset_X;
    gWin.u16WinY = WinProperty.FBOffset_Y;
    gWin.u16RBlkVPixSize = fbInfo.height;
    gWin.u16RBlkHPixSize = fbInfo.width;
    gWin.u16RBlkHRblkSize = GOP_CalcPitch(fbInfo.fbFmt, fbInfo.width);
    gWin.u32DRAMRBlkStart = fbInfo.addr;
    gWin.clrType = (EN_GOP_COLOR_TYPE)fbInfo.fbFmt;

    MApi_GOP_GWIN_SetWinInfo(WinProperty.gId, &gWin);
    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// Switch GE to render on a GWIN's frame buffer
/// @param winId \b IN: GWIN id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Switch2Gwin(MS_U8 winId)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    GOP_CBFmtInfo* pCBFmtInfo=NULL;

    GOP_ENTRY();
    pCBFmtInfo = (PGOP_CBFmtInfo)malloc(sizeof(GOP_CBFmtInfo));
    ioctl_info.en_property = E_GOP_GWIN_SWITCH_2_GWIN;
    ioctl_info.GwinId = winId;
    ioctl_info.pSet = (MS_U32*)pCBFmtInfo;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        free(pCBFmtInfo);
        GOP_RETURN(GOP_API_FAIL);
    }

    free(pCBFmtInfo);

    GOP_RETURN(GOP_API_SUCCESS);
}


/*****************************************************************************/
/// Get current active GWIN id
/// @return current active GWIN id
/*****************************************************************************/
MS_U8 MApi_GOP_GWIN_GetCurrentWinId()
{
    GOP_GET_STATUS_PARAM ioctl_info;
    MS_U32 u32GWin = INVALID_WIN_ID;
    GOP_ENTRY();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_CURRENT_GWIN;
    ioctl_info.pStatus = (MS_U32*) &u32GWin;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN((MS_U8)(u32GWin&0xFF));
}

/********************************************************************************/
/// Set which one gop show in the top when different gop do alpha blending.
/// @param u8GOP \b IN: GOP number
///   - # u8GOP is 0 => gop0's gwins will be shown in the top
///   - # u8GOP is 1 => gop1's gwins will be shown in the top
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_OutputLayerSwitch(MS_U8 u8GOP)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32 value = 0;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_OUTPUT_LAYER_SWITCH;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


/*******************************************************************************/
//Set which MUX select which GOP ,when different gop do the alpha blending
//@param pGopMuxConfig \b IN:information about GOP and corresponding level
//   #u8GopNum: the total GOP counts who need to select change Mux
//   #GopMux[i].u8GopIndex :the GOP which need to change Mux
//   #GopMux[i].u8MuxIndex :the GOP corresponding Mux
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/

E_GOP_API_Result MApi_GOP_GWIN_SetMux(GOP_MuxConfig * pGopMuxConfig, MS_U32 u32SizeOfMuxInfo)
{
    GOP_SETMUX_PARAM  ioctl_info;
    GOP_SETMUX  MuxSet;
    MS_U32 i;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SETMUX_PARAM));
    memset(&MuxSet, 0x0, sizeof(GOP_MuxConfig));

    MuxSet.MuxCount = pGopMuxConfig->u8MuxCounts;

    for(i=0; i< pGopMuxConfig->u8MuxCounts; i++)
    {
        MuxSet.gop[i] = pGopMuxConfig->GopMux[i].u8GopIndex;
        MuxSet.mux[i] = pGopMuxConfig->GopMux[i].u8MuxIndex;
    }

    ioctl_info.pMuxInfo = (MS_U32*)&MuxSet;
    ioctl_info.u32Size  = sizeof(GOP_SETMUX);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_MUX,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*******************************************************************************/
//Set which OSD Layer select which GOP
//@param pGopLayer \b IN:information about GOP and corresponding Layer
//   #u32LayerCounts: the total GOP/Layer counts to set
//   #stGopLayer[i].u32GopIndex :the GOP which need to change Layer
//   #stGopLayer[i].u32LayerIndex :the GOP corresponding Layer
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetLayer(GOP_LayerConfig *pGopLayer, MS_U32 u32SizeOfLayerInfo)
{
    GOP_SETLAYER_PARAM  ioctl_info;
    GOP_SETLayer  stLayerSetting;
    MS_U32 i;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SETLAYER_PARAM));
    memset(&stLayerSetting, 0x0, sizeof(GOP_SETLayer));

    stLayerSetting.u32LayerCount = pGopLayer->u32LayerCounts;

    for(i=0; i< stLayerSetting.u32LayerCount; i++)
    {
        stLayerSetting.u32Gop[i] = pGopLayer->stGopLayer[i].u32GopIndex;
        stLayerSetting.u32Layer[i] = pGopLayer->stGopLayer[i].u32LayerIndex;
    }

    ioctl_info.pLayerInfo= (MS_U32*)&stLayerSetting;
    ioctl_info.u32Size  = sizeof(GOP_SETLayer);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_LAYER,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*******************************************************************************/
//Get which OSD Layer select which GOP
//@param pGopLayer \b OUT:information about GOP and corresponding Layer
//   #u32LayerCounts: the total GOP/Layer inited
//   #stGopLayer[i].u32GopIndex :the GOP which need to change Layer
//   #stGopLayer[i].u32LayerIndex :the GOP corresponding Layer
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetLayer(GOP_LayerConfig *pGopLayer, MS_U32 u32SizeOfLayerInfo)
{
    GOP_SETLAYER_PARAM  ioctl_info;
    GOP_SETLayer  stLayerSetting;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SETLAYER_PARAM));
    memset(&stLayerSetting, 0x0, sizeof(GOP_SETLayer));

    ioctl_info.pLayerInfo= (MS_U32*)&stLayerSetting;
    ioctl_info.u32Size  = sizeof(GOP_SETLayer);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_LAYER,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    if (pGopLayer != NULL)
    {
        MS_U32 u32GOP;
        pGopLayer->u32LayerCounts = stLayerSetting.u32LayerCount;
        for (u32GOP = 0; u32GOP < pGopLayer->u32LayerCounts; u32GOP++)
        {
            pGopLayer->stGopLayer[u32GOP].u32GopIndex = stLayerSetting.u32Gop[u32GOP];
            pGopLayer->stGopLayer[u32GOP].u32LayerIndex = stLayerSetting.u32Layer[u32GOP];
        }
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*******************************************************************************/
//Get Layer From GOP
//@param u32GOP \b IN: gop id
//@return MS_U32 - the layer id which the gop is on, LAYER_ID_INVALID for invalid
/*******************************************************************************/
MS_U32 MApi_GOP_GWIN_GetLayerFromGOP(MS_U32 u32GOP)
{
    GOP_LayerConfig stGOPLayerConfig;
    MS_U32 u32Layer = LAYER_ID_INVALID;
    memset(&stGOPLayerConfig, 0, sizeof(GOP_LayerConfig));
    if (GOP_API_SUCCESS == MApi_GOP_GWIN_GetLayer(&stGOPLayerConfig, sizeof(GOP_LayerConfig)))
    {
        MS_U32 u32Index;
        for (u32Index = 0; u32Index < stGOPLayerConfig.u32LayerCounts; u32Index++)
        {
            if (stGOPLayerConfig.stGopLayer[u32Index].u32GopIndex == u32GOP)
            {
                u32Layer = stGOPLayerConfig.stGopLayer[u32Index].u32LayerIndex;
                break;
            }
        }
    }
    return u32Layer;
}

/*******************************************************************************/
//Get GOP From layer
//@param u32Layer \b IN: layer id
//@return MS_U32 - the gop id which the layer have,  INVALID_GOP_NUM for invalid
/*******************************************************************************/
MS_U32 MApi_GOP_GWIN_GetGOPFromLayer(MS_U32 u32Layer)
{
    MS_U32 u32GOP = INVALID_GOP_NUM;
    GOP_LayerConfig stGOPLayerConfig;
    memset(&stGOPLayerConfig, 0, sizeof(GOP_LayerConfig));
    if (GOP_API_SUCCESS == MApi_GOP_GWIN_GetLayer(&stGOPLayerConfig, sizeof(GOP_LayerConfig)))
    {
        MS_U32 u32Index;
        for (u32Index = 0; u32Index < stGOPLayerConfig.u32LayerCounts; u32Index++)
        {
            if (stGOPLayerConfig.stGopLayer[u32Index].u32LayerIndex == u32Layer)
            {
                u32GOP = stGOPLayerConfig.stGopLayer[u32Index].u32GopIndex;
                break;
            }
        }
    }

    return u32GOP;
}

/******************************************************************************/
/// Switch current active GOP
/// @param u8GOP \b IN : Number of GOP
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SwitchGOP(MS_U8 u8GOP)
{
    GOP_SELECTION_PROPERTY_PARAM ioctl_info;
    GOP_CBFmtInfo* pCBFmtInfo=NULL;

    GOP_ENTRY();
    pCBFmtInfo = (PGOP_CBFmtInfo)malloc(sizeof(GOP_CBFmtInfo));
    ioctl_info.sel_type = EN_GOP_SEL_GOP;
    ioctl_info.id = u8GOP;
    ioctl_info.pinfo = (MS_U32*)pCBFmtInfo;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SELECTION,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        free(pCBFmtInfo);
        GOP_RETURN(GOP_API_FAIL);
    }

    free(pCBFmtInfo);

    GOP_RETURN(GOP_API_SUCCESS);
}

/******************************************************************************/
/// Configure the destination of a specific GOP
/// @param u8GOP \b IN : Number of GOP
/// @param dsttype \b IN : GOP destination
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_ENUM_NOT_SUPPORTED - GOP destination not support
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetGOPDst(MS_U8 u8GOP, EN_GOP_DST_TYPE dsttype)
{

    GOP_SETDST_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SETDST_PARAM));

    ioctl_info.en_dst  = dsttype;
    ioctl_info.gop_idx = u8GOP;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_DST,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN (GOP_API_FAIL);
    }

    GOP_RETURN( GOP_API_SUCCESS);
}


/*****************************************************************************/
/// Get the free GWIN id
/// @return the GWIN id
/*****************************************************************************/
MS_U8 MApi_GOP_GWIN_GetFreeWinID(void)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    MS_U8 u8FreeId = (MS_U8)INVALID_POOL_NEXT_FBID;

    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_FREE_ID;
    ioctl_info.GwinId = 0x0;
    ioctl_info.pSet = (void*)&u8FreeId;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u8FreeId);
    }

    GOP_RETURN(u8FreeId);

}

/******************************************************************************/
/// Check if a frame buffer is allocated
/// @param fbId \b IN frame buffer id
/// @return
///   - # TRUE fbId is allocated
///   - # FALSE fbId is invalid or not allocated
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_IsFBExist(MS_U8 fbId)
{
    return MApi_GOP_GWIN_Is32FBExist((MS_U32)fbId);
}

MS_U8 MApi_GOP_GWIN_Is32FBExist(MS_U32 u32fbId)
{
    GOP_FB_PROPERTY_PARAM ioctl_info;
    MS_U8 u8Exist = FALSE;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_FB_EXIST;
    ioctl_info.FBId = u32fbId;
    ioctl_info.pSet = (void*)&u8Exist;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u8Exist);
    }

    GOP_RETURN(u8Exist);
}


/******************************************************************************/
/// Get free frame buffer id
/// @return frame buffer id. If return oxFF, it represents no free frame buffer id for use.
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_GetFreeFBID(void)
{
    MS_U32 u32FreeFBId;

    u32FreeFBId = MApi_GOP_GWIN_GetFree32FBID();
    if( u32FreeFBId > 256) //Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( u32FreeFBId:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_GetFree32FBID()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32FreeFBId );
    }

    return u32FreeFBId;
}

MS_U32 MApi_GOP_GWIN_GetFree32FBID(void)
{
    GOP_FB_PROPERTY_PARAM ioctl_info;
    MS_U32 u32FBId = 0xFFFFFFFF;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_FB_OBTAIN;
    ioctl_info.FBId = 0x0;
    ioctl_info.pSet = (MS_U32*)&u32FBId;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u32FBId);
    }
    GOP_RETURN(u32FBId);
}


/******************************************************************************/
/// Create frame buffer - take alignment factor into consideration
/// @param FBId  \b IN frame buffer id
/// @param x  \b IN frame buffer starting x
/// @param y  \b IN frame buffer starting y
/// @param width \b IN width
/// @param height \b IN height
/// @param fbFmt \b IN frame buffer format
/// @return TRUE: create OK,  FALSE: create fail
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFB2(MS_U8 FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    return MApi_GOP_GWIN_Create32FB2((MS_U32)FBId, x, y, width, height, fbFmt);
}

MS_U8 MApi_GOP_GWIN_Create32FB2(MS_U32 u32FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt)
{
    GOP_CREATE_BUFFER_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_CREATE_BUFFER_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.fb_type  = GOP_CREATE_BUFFER;
    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32FBId;

    BuffInfo.addr = 0x0;
    BuffInfo.fbFmt = fbFmt;
    BuffInfo.disp_rect.x = x;
    BuffInfo.disp_rect.y = y;
    BuffInfo.disp_rect.w = width;
    BuffInfo.disp_rect.h = height;

    BuffInfo.width = width;
    BuffInfo.height = height;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_CREATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);
}

/******************************************************************************/
/// Destroy the frame buffer and return the memory to mmgr
/// @param fbId  \b IN frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - destory frame buffer fail
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_DestroyFB(MS_U8 fbId)
{
    return MApi_GOP_GWIN_Destroy32FB((MS_U32)fbId);
}

MS_U8 MApi_GOP_GWIN_Destroy32FB(MS_U32 u32fbId)
{
    GOP_DELETE_BUFFER_PARAM ioctl_info;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_DELETE_BUFFER_PARAM));

    ioctl_info.pBufId = (MS_U32*)&u32fbId;
    ioctl_info.u32Size = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_DESTROY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GWIN_OK);
}


/******************************************************************************/
/// Destroy a frame buffer
/// @param fbId \b IN frame buffer id
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_DeleteFB(MS_U8 fbId)
{
    return MApi_GOP_GWIN_Delete32FB((MS_U32)fbId);
}

E_GOP_API_Result MApi_GOP_GWIN_Delete32FB(MS_U32 u32fbId)
{
    GOP_ENTRY();
    GOP_INFO("\33[0;36m   %s:%d   FBId = %td \33[m \n",__FUNCTION__,__LINE__, (ptrdiff_t)u32fbId);
    if ((MApi_GOP_GWIN_Destroy32FB(u32fbId)) != GWIN_OK)
    {
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Set GWIN alpha blending
/// @param u8win \b IN GWIN id
/// @param bEnable \b IN
///   - # TRUE enable pixel alpha
///   - # FALSE disable pixel alpha
/// @param u8coef \b IN alpha blending coefficient (0-7)
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetBlending(MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    GOP_GWIN_BLENDING blendInfo;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_BLENDING;
    ioctl_info.GwinId = u8win;
    ioctl_info.pSet = (MS_U32*)&blendInfo;
    ioctl_info.u32Size = sizeof(GOP_GWIN_BLENDING);

    blendInfo.Coef = u8coef;
    blendInfo.bEn = bEnable;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Set transparent color for the GWIN
/// @param fmt \b IN:
///  # GOPTRANSCLR_FMT0: RGB mode
///  # GOPTRANSCLR_FMT1: index mode
/// @param clr \b IN Transparent color for in specific color format. Note that this is only for
/// GOPTRANSCLR_FMT0(RGB mode) use. If you use GOPTRANSCLR_FMT1 (index mode), do not care this
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetTransClr_EX(MS_U8 u8GOP,EN_GOP_TRANSCLR_FMT fmt, MS_U16 clr)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TRANSCOLOR trans_set;

    GOP_ENTRY();

    trans_set.bEn = TRUE;
    trans_set.color = clr;
    trans_set.fmt = fmt;
    trans_set.transclr_property = EN_TRANSCLR_SET_COLOR;

    ioctl_info.en_pro = E_GOP_TRANSCOLOR;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (void*)&trans_set;
    ioctl_info.u32Size = sizeof(GOP_TRANSCOLOR);

    u32TransColor = clr;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_SetTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_U16 clr)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetTransClr_EX(u8GOP,fmt,clr);
    return ret;

}
/********************************************************************************/
/// Set transparent color for the GWIN
/// @param fmt \b IN GOP number
/// @param fmt \b IN:
///  # GOPTRANSCLR_FMT0: RGB mode
///  # GOPTRANSCLR_FMT1: index mode
///  # GOPTRANSCLR_FMT2: ARGB8888 mode
///  # GOPTRANSCLR_FMT3: YUV mode
/// @param ST_Transparent_Color \b IN This structure has an element color which is used for trans color.
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetTransparentClr(MS_U8 u8GOP, EN_GOP_TRANSCLR_FMT fmt, ST_Transparent_Color *st_Transcolor)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TRANSCOLOR trans_set;

    GOP_ENTRY();

    trans_set.bEn = TRUE;
    trans_set.color = st_Transcolor->color;
    trans_set.fmt = fmt;
    trans_set.transclr_property = EN_TRANSCLR_SET_COLOR;

    u32TransColor = st_Transcolor->color;

    ioctl_info.en_pro = E_GOP_TRANSCOLOR;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (void*)&trans_set;
    ioctl_info.u32Size = sizeof(GOP_TRANSCOLOR);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
/********************************************************************************/
/// Set transparent color (ARGB domain) and enable it for the GWIN
/// @param clr \b IN: transparent color
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetFMT0TransClr_EX(MS_U8 u8GOP,MS_U16 clr)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TRANSCOLOR trans_set;

    GOP_ENTRY();

    trans_set.bEn = TRUE;
    trans_set.color = clr;
    trans_set.fmt = GOPTRANSCLR_FMT0;
    trans_set.transclr_property = EN_TRANSCLR_SET_COLOR;

    u32TransColor = clr;

    ioctl_info.en_pro = E_GOP_TRANSCOLOR;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (void*)&trans_set;
    ioctl_info.u32Size = sizeof(GOP_TRANSCOLOR);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_SetFMT0TransClr(MS_U16 clr)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetFMT0TransClr_EX(u8GOP,clr);
    return ret;
}
//-------------------------------------------------------------------------------------------------
/// Get maximum support frame buffer number
/// @return maximum support frame buffer number
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_GOP_GWIN_GetMAXFBID(void)
{
    MS_U32 u32MaxFBID;
    u32MaxFBID = MApi_GOP_GWIN_GetMAX32FBID();
    if( u32MaxFBID> 256) //Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( u32MaxFBID:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_GetMAX32FBID()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MaxFBID );
    }
    return u32MaxFBID;
}

MS_U32 MApi_GOP_GWIN_GetMAX32FBID(void)
{
    return MApi_GOP_GWIN_GetMax32FBNum();
}

//-------------------------------------------------------------------------------------------------
/// Get maximum support gop number
/// @return maximum support gop number
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_GOP_GWIN_GetMaxGOPNum(void)
{
    MS_U8 u8MaxGop;
    GOP_GET_STATUS_PARAM ioctl_info;

    CheckGOPInstanceOpen();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_GOP_MAXNUM;
    ioctl_info.pStatus = (void*) &u8MaxGop;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return u8MaxGop;

}


//-------------------------------------------------------------------------------------------------
/// Get maximum gwin number by individual gop
/// @param u8GopNum \b IN: Number of GOP
/// @return maximum gwin number by individual gop
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_GOP_GWIN_GetGwinNum(MS_U8 u8GopNum)
{
    GOP_GET_STATUS_PARAM ioctl_info;
    GOP_GWIN_NUM  gwin_num;
    CheckGOPInstanceOpen();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_GWIN_MAXNUM;
    ioctl_info.pStatus = (MS_U32*)&gwin_num;
    ioctl_info.u32Size = sizeof(GOP_GWIN_NUM);

    gwin_num.gop_idx = u8GopNum;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return gwin_num.gwin_num;
}


//-------------------------------------------------------------------------------------------------
/// Get maximum support gwin number by all gop
/// @return maximum support gwin number by all gop
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_GOP_GWIN_GetTotalGwinNum(void)
{
    MS_U8 u8TotalWin;
    GOP_GET_STATUS_PARAM ioctl_info;
    CheckGOPInstanceOpen();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_GWIN_TOTALNUM;
    ioctl_info.pStatus = (void*)&u8TotalWin;
    ioctl_info.u32Size = sizeof(MS_U8);
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    return u8TotalWin;
}


/*****************************************************************************/
/// Get max GWIN id
/// @return the maximum number of GWIN id
/*****************************************************************************/
MS_U8 MApi_GOP_GWIN_GetMAXWinID(void)
{
    return MApi_GOP_GWIN_GetTotalGwinNum();
}

/******************************************************************************/
/// Enable/Disable multi alpha for one gop
/// @param u32GopIdx \b IN: gop ID
/// @param bEnable \b IN: TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_EnableMultiAlpha(MS_U32 u32GopIdx, MS_BOOL bEnable)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_ENTRY();
    ioctl_info.en_pro = E_GOP_MULTI_ALPHA_ENABLE;
    ioctl_info.gop_idx = u32GopIdx;
    ioctl_info.pSetting = (void*)&bEnable;
    ioctl_info.u32Size = sizeof(MS_BOOL);
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// get multi alpha status for one gop
/// @param u32GopIdx \b IN: gop ID
/// @return multi alpha status
/******************************************************************************/
MS_BOOL MApi_GOP_GWIN_IsMultiAlphaEnable(MS_U32 u32GopIdx)
{
    MS_BOOL bEnable = FALSE;
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_ENTRY();
    ioctl_info.en_pro = E_GOP_MULTI_ALPHA_ENABLE;
    ioctl_info.gop_idx = u32GopIdx;
    ioctl_info.pSetting = (void*)&bEnable;
    ioctl_info.u32Size = sizeof(MS_BOOL);
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(bEnable);
}

/******************************************************************************/
/// Set alpha value of GOP0 4 Gwins
/// @param Gwin01AlphaValue \b IN: Alpha coefficient and Enable status of Gwin 0/1
/// @param Gwin23AlphaValue \b IN: Alpha coefficient and Enable status of Gwin 2/3
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetAlphaValue(MS_U16 * Gwin01AlphaValue, MS_U16 * Gwin23AlphaValue)
{
    GOP_ENTRY();

    GOP_ERR("[%s] Not Support \n",__FUNCTION__);

    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Get GOP0 4 gwin alpha blending setting
/// @param Gwin01AlphaValue \b OUT: gwin0 and gwin1 alpha setting
/// @param Gwin23AlphaValue \b OUT: gwin2 and gwin3 alpha setting
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetAlphaValue(MS_U16 * Gwin01AlphaValue, MS_U16 * Gwin23AlphaValue)
{
    GOP_ENTRY();

    GOP_ERR("[%s] Not Support \n",__FUNCTION__);

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_SetPreAlphaMode(MS_U8 u8GOP, MS_BOOL bEnble)
{
    MS_BOOL bPreAlpha;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bPreAlpha = bEnble;


    ioctl_info.en_pro   = E_GOP_PREALPHAMODE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bPreAlpha;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Set gop driver internal palette table to GOP0
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetPalette(void)
{
    MS_U32 i;
    GOP_PALETTE_SET_PARAM  ioctl_info;
    GOP_PALETTE_ENTRY  palEntry;

    GOP_ENTRY();
    for (i=0;i<(256/PALETTE_ENTRY_NUM);i++)
    {
        memset(&ioctl_info, 0x0, sizeof(GOP_PALETTE_SET_PARAM));
        memset(&palEntry, 0x0, sizeof(GOP_PALETTE_ENTRY));

        palEntry.gop_idx = 0x0;
        palEntry.pClut = (void*)&PaletteEntry;
        palEntry.start = i;
        palEntry.end   = ((i+1)*PALETTE_ENTRY_NUM - 1);
        palEntry.palType = E_GOP_PAL_ARGB8888;
        palEntry.palSrc = E_GOP_4G_PALETTE;

        ioctl_info.pClut = (void*)&palEntry;
        ioctl_info.u32Size = sizeof(GOP_PALETTE_ENTRY);

        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_PALETTE_SET_ENTRY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            GOP_RETURN( GOP_API_FAIL);
        }
    }
    GOP_RETURN(GOP_API_SUCCESS);
}


/******************************************************************************/
/// Get GWin position info
/// @param winId  \b IN: gwin id
/// @param dispX \b OUT: position-X
/// @param dispY \b OUT: position-Y
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetWinPosition(MS_U8 winId, MS_U16 * dispX, MS_U16 *dispY)
{
    GOP_ENTRY();
    GOP_GwinInfo info;
    MApi_GOP_GWIN_GetWinInfo(winId,&info);
    *dispX = info.u16DispHPixelStart;
    *dispY = info.u16DispVPixelStart;
    GOP_RETURN(GOP_API_SUCCESS);
}


/******************************************************************************/
/// Get frame buffer address
/// @param fbID \b IN: frame buffer id
/// @param pAddr \b OUT: frame buffer address
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetFBAddr(MS_U8 fbID, MS_PHY *pAddr)
{
    return MApi_GOP_GWIN_Get32FBAddr((MS_U32)fbID, pAddr);
}

E_GOP_API_Result MApi_GOP_GWIN_Get32FBAddr(MS_U32 u32fbID, MS_PHY *pAddr)
{
    GOP_GwinFBAttr fbAttr;
    GOP_ENTRY();

    if(MApi_GOP_GWIN_Get32FBInfo(u32fbID, &fbAttr) == GOP_API_SUCCESS)
    {
        *pAddr = fbAttr.addr;
        GOP_RETURN(GOP_API_SUCCESS);
    }
    else
    {
        GOP_RETURN(GOP_API_FAIL);
    }

}

/******************************************************************************/
/// Reset the gwin info. Note that no free any frame buffer in this interface!!
/// @param gId  \b IN: gwin id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_ReleaseWin(MS_U8 gId)
{
    GOP_ENTRY();

    MApi_GOP_GWIN_Enable(gId, FALSE);
    MApi_GOP_GWIN_DestroyWin(gId);

    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Get frame bufferid from the gwin id
/// @param gwinId  \b IN gwin id
/// @return return frame buffer id. If return 0xFF, it represents this function fail
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_GetFBfromGWIN(MS_U8 gwinId)
{
    MS_U32 u32CurFBId;

    u32CurFBId = MApi_GOP_GWIN_Get32FBfromGWIN(gwinId);
    if( u32CurFBId > 256) //Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( u32CurFBId:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_Get32FBfromGWIN()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32CurFBId );
    }
    return u32CurFBId;
}

MS_U32 MApi_GOP_GWIN_Get32FBfromGWIN(MS_U8 gwinId)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    MS_U32 u32FB = INVALID_POOL_NEXT_FBID;
    GOP_ENTRY();

    ioctl_info.en_property = E_GOP_GWIN_GET_FB;
    ioctl_info.GwinId = gwinId;
    ioctl_info.pSet = (MS_U32*)&u32FB;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(u32FB);
    }

    GOP_RETURN(u32FB);

}


/******************************************************************************/
/// Get region info for a frame buffer
/// @param fbID  \b IN frame buffer id 0 ~ (MAX_GWIN_FB_SUPPORT - 1)
/// @param pX \b OUT position-X
/// @param pY \b OUT position-Y
/// @param pW \b OUT width
/// @param pH \b OUT height
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetFBRegion(MS_U8 fbID, MS_U16 *pX, MS_U16 *pY, MS_U16 *pW, MS_U16 *pH)
{
    return MApi_GOP_GWIN_Get32FBRegion((MS_U32)fbID, pX, pY, pW, pH);
}

E_GOP_API_Result MApi_GOP_GWIN_Get32FBRegion(MS_U32 u32fbID, MS_U16 *pX, MS_U16 *pY, MS_U16 *pW, MS_U16 *pH)
{
    GOP_GwinFBAttr fbAttr;
    GOP_ENTRY();

    if(MApi_GOP_GWIN_Get32FBInfo(u32fbID, &fbAttr) == GOP_API_SUCCESS)
    {
        *pX = fbAttr.x0;
        *pY = fbAttr.y0;
        *pW = fbAttr.width;
        *pH = fbAttr.height;
        GOP_RETURN(GOP_API_SUCCESS);
    }
    else
    {
        GOP_RETURN(GOP_API_FAIL);
    }

}

/******************************************************************************/
/// Get position info for a frame buffer
/// @param fbID  \b IN frame buffer id 0 ~ (MAX_GWIN_FB_SUPPORT - 1)
/// @param pX \b OUT position-X
/// @param pY \b OUT position-Y
/// @return GOP_API_SUCCESS - Success
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetFBPosition(MS_U8 fbID, MS_U16 *pX, MS_U16 *pY)
{
    return MApi_GOP_GWIN_Get32FBPosition((MS_U32)fbID, pX, pY);
}

E_GOP_API_Result MApi_GOP_GWIN_Get32FBPosition(MS_U32 u32fbID, MS_U16 *pX, MS_U16 *pY)
{
    GOP_GwinFBAttr fbAttr;
    GOP_ENTRY();
    if(MApi_GOP_GWIN_Get32FBInfo(u32fbID, &fbAttr) == GOP_API_SUCCESS)
    {
        *pX = fbAttr.x0;
        *pY = fbAttr.y0;
        GOP_RETURN(GOP_API_SUCCESS);
    }
    else
    {
        GOP_RETURN(GOP_API_FAIL);
    }

}


/******************************************************************************/
/// Create frame buffer - take alignment factor into consideration
/// @param FBId  \b IN frame buffer id
/// @param x  \b IN frame buffer starting x
/// @param y  \b IN frame buffer starting y
/// @param width \b IN width
/// @param height \b IN height
/// @param fbFmt \b IN frame buffer format
/// @param FBString \n IN create frame buffer by which one application
/// @return TRUE: create OK,  FALSE: create fail
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFB2_EXT(MS_U8 FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString)
{
    return MApi_GOP_GWIN_Create32FB2_EXT((MS_U32)FBId, x, y, width, height, fbFmt ,FBString);
}

MS_U8 MApi_GOP_GWIN_Create32FB2_EXT(MS_U32 u32FBId, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height, MS_U16 fbFmt, EN_GOP_FRAMEBUFFER_STRING FBString)
{
    GOP_CREATE_BUFFER_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_CREATE_BUFFER_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.fb_type  = GOP_CREATE_BUFFER;
    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32FBId;

    BuffInfo.addr = 0x0;
    BuffInfo.fbFmt = fbFmt;
    BuffInfo.disp_rect.x = x;
    BuffInfo.disp_rect.y = y;
    BuffInfo.disp_rect.w = width;
    BuffInfo.disp_rect.h = height;

    BuffInfo.width = width;
    BuffInfo.height = height;
    BuffInfo.FBString = FBString;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_CREATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Check if GOP destination is op mode
/// @return TRUE:op mode  FALSE: not in op mode
/********************************************************************************/
MS_BOOL MApi_GOP_GWIN_CheckOpmodeIsOn_EX(MS_U8 u8GOP)
{
    EN_GOP_DST_TYPE enGopDst;
    GOP_GETDST_PARAM ioctl_info;
    memset(&ioctl_info,0x0, sizeof(GOP_GETDST_PARAM));

    GOP_ENTRY();

    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pDst = (MS_U32*) &enGopDst;
    ioctl_info.u32Size = sizeof(EN_GOP_DST_TYPE);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_DST,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    if (enGopDst == E_GOP_DST_OP0)
    {
        GOP_RETURN(TRUE);
    }
    else
    {
        GOP_RETURN(FALSE);
    }
}

MS_BOOL MApi_GOP_GWIN_CheckOpmodeIsOn(void)
{
    MS_U8 u8GOP;
    MS_BOOL ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_CheckOpmodeIsOn_EX(u8GOP);
    return ret;
}
/*****************************************************************************/
/// Get Active GWIN id with max vertical pixel length
/// Active means that displaying OSD currently on screen
/// @return the active GWIN's length  with the maximum vertical pixel length
/*****************************************************************************/
MS_U16 MApi_GOP_GWIN_GetMaxActiveGwin_v(void)
{
    MS_U16 v_length=0;
    MS_U8 i=0;
    GOP_GwinInfo gwin;
    MS_U16 maxvlength=0;
    MS_U16 h_length=0;
    GOP_ENTRY();

    if (MApi_GOP_GWIN_CheckOpmodeIsOn()==false)
    {
        GOP_RETURN(GetMaxActiveGwinFalse_op);
    }
    else
    {
        // Check which GWINS are active
        if (MApi_GOP_GWIN_IsEnabled()==false)
        {
            GOP_RETURN(GetMaxActiveGwinFalse_opened);
        }
        else
        //if gwin are active then compare their vertical size
        // Initialize
        {
            for (i=0;i<MAX_GWIN_SUPPORT;i++)
            {
                //printf("\n(1) i:0x%bx",i);
                if (MApi_GOP_GWIN_IsGWINEnabled(i)==true)
                {
                    memset(&gwin, 0, sizeof(GOP_GwinInfo));
                    MApi_GOP_GWIN_GetWinInfo(i,&gwin);
                    h_length=gwin.u16DispHPixelEnd-gwin.u16DispHPixelStart;
                    if (gwin.u16DispVPixelEnd>gwin.u16DispVPixelStart)
                    {

                        if (h_length>4)
                        {
                            v_length=gwin.u16DispVPixelEnd-gwin.u16DispVPixelStart;
                            if (v_length>maxvlength)
                            {
                                maxvlength=v_length;
                            }
                        }
                    }
                    else
                    {
                        GOP_RETURN(GetMaxActiveGwinFalse);
                    }
                }

            }

            GOP_RETURN(maxvlength);

        }

    }
}


/*****************************************************************************/
/// API for acquiring displaying GWIN's properties
/// @param WinProperty  \b OUT: pointer of GOP_GwinDispProperty
/// @return GOP_API_SUCCESS - Success
/*****************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetWinProperty(GOP_GwinDispProperty *WinProperty)
{
    GOP_GwinInfo gWin;
    GOP_ENTRY();

    memset(&gWin, 0, sizeof(GOP_GwinInfo));

    if(GOP_API_SUCCESS == MApi_GOP_GWIN_GetWinInfo(WinProperty->gId,&gWin))
    {
        WinProperty->StartX=gWin.u16DispHPixelStart;
        WinProperty->StartY=gWin.u16DispVPixelStart;
        //WinProperty->GOPWinWidth=gWin.u16DispVPixelEnd-gWin.u16DispHPixelStart;
        WinProperty->Win_Width=gWin.u16DispHPixelEnd-gWin.u16DispHPixelStart+1;
        WinProperty->Win_Height=gWin.u16DispVPixelEnd-gWin.u16DispVPixelStart+1;
        GOP_RETURN(GOP_API_SUCCESS);
    }
    else
    {
        GOP_RETURN(GOP_API_FAIL);
    }

}

/*****************************************************************************/
/// API for acquiring color format of some Framebuffer
/// @param fbId  \b IN frame buffer id
/// @return the format of Frame buffer. If return 0xF, it represents get frame buffer format fail.
/*****************************************************************************/
MS_U16 MApi_GOP_GWIN_GetFBFmt(MS_U8 fbId)
{
    return MApi_GOP_GWIN_Get32FBFmt((MS_U32)fbId);
}

MS_U16 MApi_GOP_GWIN_Get32FBFmt(MS_U32 u32fbId)
{
   GOP_GwinFBAttr fbAttr;
   GOP_ENTRY();

    if (!MApi_GOP_GWIN_Is32FBExist(u32fbId))
    {
        MS_DEBUG_MSG( GOP_ERR("MApi_GOP_GWIN_Get32FBFmt: u32fbId=%d is not in existence\n",u32fbId));
        GOP_RETURN(GWIN_NO_AVAILABLE);
    }
    else
    {
        if(MApi_GOP_GWIN_Get32FBInfo(u32fbId, &fbAttr) == GOP_API_SUCCESS)
        {
            GOP_RETURN(fbAttr.fbFmt);
        }
        else
        {
            GOP_RETURN(GOP_API_FAIL);
        }
    }
}


/*****************************************************************************/
/// API for acquiring current FB's ID
/// @ return current FB'id. if return is 0xFF, it represents get current frame buffer fail
/*****************************************************************************/
MS_U8 MApi_GOP_GWIN_GetCurrentFBID(void)
{
    MS_U32 u32CurFBID;

    u32CurFBID = MApi_GOP_GWIN_GetCurrent32FBID();
    if( u32CurFBID > 256) //Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( u32CurFBID:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_GetCurrent32FBID()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32CurFBID );
    }
    return u32CurFBID;
}

MS_U32 MApi_GOP_GWIN_GetCurrent32FBID(void)
{
    GOP_GET_STATUS_PARAM ioctl_info;
    MS_U32 u32FbId = INVALID_POOL_NEXT_FBID;
    GOP_ENTRY();
    ioctl_info.type = 0;
    ioctl_info.en_status = E_GOP_STATUS_CURRENT_FBID;
    ioctl_info.pStatus = (MS_U32*) &u32FbId;
    ioctl_info.u32Size = sizeof(MS_U32);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_STATUS,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN(u32FbId);

}


/********************************************************************************/
/// Enable/disable GOP field inverse
/// @param enable  \b IN TRUE/FALSE
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetFieldInver_EX(MS_U8 u8GOP,MS_BOOL enable)
{
    MS_BOOL bFieldInv;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bFieldInv = enable;

    ioctl_info.en_pro   = E_GOP_FIELD_INVERSE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bFieldInv;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}
E_GOP_API_Result MApi_GOP_GWIN_SetFieldInver(MS_BOOL enable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetFieldInver_EX(u8GOP,enable);
    return ret;
}

//******************************************************************************
/// Enable gop transparent color
/// @param fmt \b IN: RGB mode or u8Index mode color format
/// @param bEnable \b IN:
///   - # TRUE Enable gop transparent color
///   - # FALSE Disable gop transparent color
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_EnableTransClr_EX(MS_U8 u8GOP,EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TRANSCOLOR trans_set;

    GOP_ENTRY();
    trans_set.bEn = bEnable;
    trans_set.color = u32TransColor;
    trans_set.fmt = fmt;
    trans_set.transclr_property =  EN_TRANSCLR_ENABLE;

    ioctl_info.en_pro = E_GOP_TRANSCOLOR;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (void*)&trans_set;
    ioctl_info.u32Size = sizeof(GOP_TRANSCOLOR);


    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}
E_GOP_API_Result MApi_GOP_GWIN_EnableTransClr(EN_GOP_TRANSCLR_FMT fmt, MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_EnableTransClr_EX(u8GOP,fmt,bEnable);
    return ret;
}
/********************************************************************************/
/// Set GWIN Blink
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param u8rate \b IN Blink frame rate
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetBlink(MS_BOOL bEnable, MS_U8 u8rate)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    GOP_GWIN_BLINK blinkInfo;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_BLINK;
    ioctl_info.GwinId = 0x0;
    ioctl_info.pSet = (MS_U32*)&blinkInfo;
    ioctl_info.u32Size = sizeof(GOP_GWIN_BLINK);

    blinkInfo.rate = u8rate;
    blinkInfo.bEn = bEnable;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_Enable_BGWIN(MS_U8 bgwID, MS_BOOL bEnable)
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not Support \n",__FUNCTION__);
    GOP_RETURN(GOP_API_SUCCESS);
}


E_GOP_API_Result MApi_GOP_GWIN_Set_BGWIN(MS_U8 bgwID, MS_U16 x0, MS_U16 y0, MS_U16 w, MS_U16 h, MS_U16 color)
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not Support \n",__FUNCTION__);
    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_Set_BGWIN_Alpha(MS_U8 bgwID, MS_BOOL bEnable, MS_U8 alpha)
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not Support \n",__FUNCTION__);
    GOP_RETURN(GOP_API_SUCCESS);
}

// Alignment stretch window value

/********************************************************************************/
/// Set stretch window property
/// @param u8GOP_num \b IN 0: GOP0  1:GOP1
/// @param eDstType \b IN \copydoc EN_GOP_DST_TYPE
/// @param x \b IN stretch window horizontal start position
/// @param y \b IN stretch window vertical start position
/// @param width \b IN stretch window width
/// @param height \b IN stretch window height
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Set_STRETCHWIN(MS_U8 u8GOP_num, EN_GOP_DST_TYPE eDstType,MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height)
{
    GOP_STRETCH_SET_PARAM ioctl_info;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_info,0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.DstRect.x = x;
    stretch_info.DstRect.y = y;
    stretch_info.DstRect.w = width;
    stretch_info.DstRect.h = height;

    stretch_info.SrcRect.x = x;
    stretch_info.SrcRect.y = y;
    stretch_info.SrcRect.w = width;
    stretch_info.SrcRect.h = height;

    ioctl_info.gop_idx = u8GOP_num;
    ioctl_info.enStrtchType = E_GOP_STRETCH_WIN;
    ioctl_info.pStretch = (MS_U32*)&stretch_info;
    ioctl_info.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Set stretch window position
/// @param u8GOPNum \b IN 0: GOP0  1:GOP1
/// @param u16DispX \b IN stretch window horizontal start position
/// @param u16DispY \b IN stretch window vertical start position
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_INVALID_PARAMETERS - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetStretchWinPosition(MS_U8 u8GOPNum, MS_U16 u16DispX, MS_U16 u16DispY)
{
    GOP_STRETCH_SET_PARAM ioctl_StrrtchInfo;
    GOP_STRETCH_INFO stretch_info;
    GOP_ENTRY();

    memset(&ioctl_StrrtchInfo, 0x0, sizeof(GOP_STRETCH_SET_PARAM));
    memset(&stretch_info, 0x0, sizeof(GOP_STRETCH_INFO));

    stretch_info.SrcRect.x = u16DispX;
    stretch_info.SrcRect.y = u16DispY;

    ioctl_StrrtchInfo.gop_idx = u8GOPNum;
    ioctl_StrrtchInfo.enStrtchType = E_GOP_STRETCH_WIN_POSITION;
    ioctl_StrrtchInfo.pStretch = (MS_U32*)&stretch_info;
    ioctl_StrrtchInfo.u32Size = sizeof(GOP_STRETCH_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_STRETCH,(void*)&ioctl_StrrtchInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Restore initial stretch window setting of GOP. It will set gop to OP path and stretch window width/height
/// to panel width/height
/// @param u8GOPNum \b IN : Number of GOP
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_Initialize_StretchWindow(MS_U8 u8GOPNum)
{
#ifndef CONFIG_GOP_GWIN_MISC
    GOP_ERR("\33[0;36m   %s: #define CONFIG_GOP_GWIN_MISC not opened\33[m \n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
#else
    ST_GOP_TIMING_INFO stTimingInfo;
    MS_PHY *plist;

    GOP_ENTRY();
    plist = (MS_PHY *)(void*)&stTimingInfo;

    MApi_GOP_GetConfigEx(u8GOPNum,E_GOP_TIMING_INFO,plist);

#if (FPGA_TEST)
    MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_VOP,0, 0
        , stTimingInfo.u16DEHSize, stTimingInfo.u16DEVSize);
#else
    MApi_GOP_GWIN_Set_STRETCHWIN(u8GOPNum, E_GOP_DST_OP0,0, 0
        , stTimingInfo.u16DEHSize, stTimingInfo.u16DEVSize);
#endif
    GOP_RETURN(GOP_API_SUCCESS);
#endif
}



/********************************************************************************/
/// Set GOP H-Mirror
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetHMirror_EX(MS_U8 u8GOP,MS_BOOL bEnable)
{
    GOP_SETMIRROR_PARAM ioctl_info;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SETMIRROR_PARAM));


    if(bEnable == TRUE)
        ioctl_info.dir = E_GOP_MIRROR_H_ONLY;
    else
        ioctl_info.dir = E_GOP_MIRROR_H_NONE;

    ioctl_info.gop_idx = (MS_U32)u8GOP;  //get current gop for driver capability
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_MIRROR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }


    GOP_RETURN(GOP_API_SUCCESS);
}
E_GOP_API_Result MApi_GOP_GWIN_SetHMirror(MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetHMirror_EX(u8GOP,bEnable);
    return ret;
}
//******************************************************************************
/// Get gop h-mirror and v-mirror status
/// @param *bHMirror \b OUT TRUE:h-Mirror on, FALSE: h-Mirror off
/// @param *bVMirror \b OUT TRUE:v-Mirror on, FALSE: v-Mirror off
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_IsMirrorOn_EX(MS_U8 u8GOP,MS_BOOL *bHMirror, MS_BOOL *bVMirror)
{
    GOP_GETMIRROR_PARAM ioctl_info;
    EN_GOP_MIRROR_TYPE mirror_type;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GETMIRROR_PARAM));

    ioctl_info.gop_idx = (MS_U32)u8GOP;  //get current gop for driver capability
    ioctl_info.pdir    = &mirror_type;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_MIRROR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    if(mirror_type == E_GOP_MIRROR_HV)
    {
        *bHMirror = TRUE;
        *bVMirror = TRUE;
    }
    else if(mirror_type == E_GOP_MIRROR_H_ONLY)
    {
        *bHMirror = TRUE;
        *bVMirror = FALSE;
    }
    else if(mirror_type == E_GOP_MIRROR_V_ONLY)
    {
        *bHMirror = FALSE;
        *bVMirror = TRUE;
    }
    else
    {
        *bHMirror = FALSE;
        *bVMirror = FALSE;
    }

    GOP_RETURN(GOP_API_SUCCESS);

}
E_GOP_API_Result MApi_GOP_GWIN_IsMirrorOn(MS_BOOL *bHMirror, MS_BOOL *bVMirror)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;

    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_IsMirrorOn_EX(u8GOP,bHMirror,bVMirror);
    return ret;
}
/********************************************************************************/
/// Set GOP V-Mirror
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetVMirror_EX(MS_U8 u8GOP,MS_BOOL bEnable)
{
    GOP_SETMIRROR_PARAM ioctl_info;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SETMIRROR_PARAM));

    if( bEnable == TRUE )
        ioctl_info.dir = E_GOP_MIRROR_V_ONLY;
    else
        ioctl_info.dir = E_GOP_MIRROR_V_NONE;

    ioctl_info.gop_idx = (MS_U32)u8GOP;  //get current gop for driver capability
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_MIRROR,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_SetVMirror(MS_BOOL bEnable)
{
    MS_U8 u8GOP;
    E_GOP_API_Result ret;
    u8GOP = MApi_GOP_GWIN_GetCurrentGOP();
    ret = MApi_GOP_GWIN_SetVMirror_EX(u8GOP,bEnable);
    return ret;
}

/********************************************************************************/
/// Set GWIN Tile Mode
/// @param u8win \b IN GWIN id
/// @param bEnable \b IN
///   - # TRUE Gwin Tile Mode
///   - # FALSE Gwin Tile Mode
/// @param u8coef \b IN alpha blending coefficient (0-7)
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetGPUTile(MS_U8 u8win, EN_GOP_GPU_TILE_MODE TileMode)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    EN_GOP_GPU_TILE_MODE mode;
    mode = TileMode;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_GWIN_GPUTILE;
    ioctl_info.GwinId = u8win;
    ioctl_info.pSet = (MS_U32*)&mode;
    ioctl_info.u32Size = sizeof(EN_GOP_GPU_TILE_MODE);


    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Set GOP TLB Mode
/// @param u8GOP \b IN: only init by which gop
/// @param bEnable \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @param tlbaddr \b TLB entry base address
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_TLB_Enable(MS_U8 u8GOP,MS_BOOL bEnable, MS_PHY phytlbaddr)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_TLB_Enable tlb_enable;

    GOP_ENTRY();
    tlb_enable.gop_idx = (MS_U32)u8GOP;
    tlb_enable.bEn = bEnable;
    tlb_enable.u64TLBAddr = phytlbaddr;

    ioctl_info.en_pro = E_GOP_GWIN_TLB_ENABLE;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&tlb_enable;
    ioctl_info.u32Size = sizeof(GOP_TLB_Enable);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

//-------------------------------------------------------------------------------------------------
/// GOP Exit
/// @param  void                \b IN: none
//-------------------------------------------------------------------------------------------------
void MApi_GOP_Exit()
{
    //App call exit, should wait mutex return to avoid mutex conflict use with other thread.
    //But do not need GOP_RETURN, because all mutex and resouce has been released in atexit.
    GOP_INFO("---%s %d: PID[%td], TID[%td] tries to exit\n", __FUNCTION__, __LINE__, (ptrdiff_t)getpid(), (ptrdiff_t)MsOS_GetOSThreadID());
    GOP_ENTRY();
#ifdef  MSOS_TYPE_LINUX
    GOP_MISC_PARAM ioctl_info;
    MS_U32  value = 0;

    ioctl_info.misc_type = E_GOP_MISC_AT_EXIT;
    ioctl_info.pMISC = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return;
    }
#else
    GOP_INFO("not enable MSOS_TYPE_LINUX\n");
#endif
}

//-------------------------------------------------------------------------------------------------
/// Initial individual GOP driver
/// @param pGopInit \b IN:gop driver init info
/// @param u8GOP \b IN: only init by which gop
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MApi_GOP_InitByGOP(GOP_InitInfo *pGopInit, MS_U8 u8GOP)
{

    GOP_INIT_PARAM  ioctl_info;

    CheckXCOpen();
    CheckGOPInstanceOpen();
    _GOP_RegisterAllCBFunc();

    ioctl_info.gop_idx = u8GOP;
    ioctl_info.pInfo = (MS_U32*) pGopInit;
    ioctl_info.u32Size = sizeof(GOP_InitInfo);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_INIT,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
#ifdef  MSOS_TYPE_LINUX
    atexit(MApi_GOP_Exit);
#endif
    bInit =TRUE;
    return GOP_API_SUCCESS;
}


//-------------------------------------------------------------------------------------------------
/// Initial all GOP driver (include gop0, gop1 ext..)
/// @param pGopInit \b IN:gop driver init info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//-------------------------------------------------------------------------------------------------
E_GOP_API_Result MApi_GOP_Init(GOP_InitInfo *pGopInit)
{
    GOP_INIT_PARAM  ioctl_info;
    MS_U32 i =0;

    CheckXCOpen();
    CheckGOPInstanceOpen();
    //init GOP0
    _GOP_RegisterAllCBFunc();
    ioctl_info.gop_idx = 0;
    ioctl_info.pInfo = (MS_U32*) pGopInit;
    ioctl_info.u32Size = sizeof(GOP_InitInfo);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_INIT,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
    //init others
    for(i = 1;i<MApi_GOP_GWIN_GetMaxGOPNum();i++)
    {
        ioctl_info.gop_idx = i;
        if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_INIT,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
    bInit =TRUE;
    return GOP_API_SUCCESS;
}

/******************************************************************************/
/// Set gwin fade in/out
/// @param gId  \b IN: gwin id
/// @param Width \b IN: gwin display width
/// @param X_Start \b IN: gwin display H display starting position
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetFadeInOut(MS_U8 gId, EN_GOP_FADE_TYPE eGwinFadeType, MS_BOOL bEnable, MS_U8 u8FadeRate)
{
    E_GOP_API_Result ret = GOP_API_SUCCESS;
    GOP_SET_FADE_PARAM ioctl_info;
    GOP_FADE_INFO fade_info;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_FADE_PARAM));
    memset(&fade_info, 0x0, sizeof(GOP_FADE_INFO));

    fade_info.gwin_idx = gId;
    fade_info.bEnable = bEnable;
    fade_info.eGwinFadeType = (EN_GOP_SET_FADE_TYPE)eGwinFadeType;
    fade_info.u8FadeRate = u8FadeRate;

    ioctl_info.pSetting = (MS_U32*)&fade_info;
    ioctl_info.u32Size  = sizeof(GOP_FADE_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_FADE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(ret);
}

/******************************************************************************/
/// Set a GWIN's H display size
/// @param gId  \b IN: gwin id
/// @param Width \b IN: gwin display width
/// @param X_Start \b IN: gwin display H display starting position
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetHDisplaySize(MS_U8 gId, MS_U16 Width,MS_U16 X_Start)
{
    GOP_GWIN_SETWININFO_PARAM WinInfo;
    GOP_BUFFER_INFO  BuffInfo;


    GOP_ENTRY();

    memset(&WinInfo, 0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&BuffInfo, 0, sizeof(GOP_BUFFER_INFO));


    WinInfo.GwinId = gId;
    WinInfo.pinfo = (MS_U32*)&BuffInfo;
    WinInfo.u32Size = sizeof(GOP_BUFFER_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_WININFO,(void*)&WinInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }


    if (X_Start % 4 != 0 )
    {
       X_Start = (X_Start +2) / 4; // rounding
       X_Start = X_Start * 4;
    }

    BuffInfo.disp_rect.x = X_Start;
    BuffInfo.disp_rect.w = Width;
    BuffInfo.width = Width;


    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_WININFO,(void*)&WinInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// Move a GWIN's display left top position
/// @param gId  \b IN : GWINID
/// @param dispX \b IN: left top x
/// @param dispY \b IN: left top y
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetWinDispPosition(MS_U8 gId, MS_U16 dispX, MS_U16 dispY)
{

    GOP_GWIN_SETWININFO_PARAM WinInfo;
    GOP_BUFFER_INFO  BuffInfo;


    GOP_ENTRY();

    memset(&WinInfo, 0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&BuffInfo, 0, sizeof(GOP_BUFFER_INFO));


    WinInfo.GwinId = gId;
    WinInfo.pinfo = (MS_U32*)&BuffInfo;
    WinInfo.u32Size = sizeof(GOP_BUFFER_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_WININFO,(void*)&WinInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    BuffInfo.disp_rect.x = dispX;
    BuffInfo.disp_rect.y = dispY;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_WININFO,(void*)&WinInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Delete the GWIN, free corresponding frame buffer
/// @param gId \b IN \copydoc GWINID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_DestroyWin(MS_U8 gId)
{
    GOP_GWIN_DESTROY_PARAM ioctl_info;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_DESTROY_PARAM));
    ioctl_info.GwinId = gId;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_DESTROY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Get corresponding frame buffer attribute from a GWIN id
/// @param gId \b IN GWINID
/// @param x0 \b OUT buffer to store left top x
/// @param y0 \b OUT buffer to store left top y
/// @param width \b OUT buffer to store width
/// @param height \b OUT buffer to store height
/// @param fbPitch \b OUT buffer to store pitch
/// @param fbFmt \b OUT buffer to store FB format
/// @param addr \b OUT buffer to store addr
/// @param size \b OUT buffer to store size
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetWinAttr(MS_U8 gId, MS_U16 *x0, MS_U16 *y0, MS_U16 *width, MS_U16 *height,  MS_U16 *fbPitch, MS_U16 *fbFmt, MS_PHY *addr, MS_U32 *size)
{

    GOP_GWIN_SETWININFO_PARAM WinInfo;
    GOP_BUFFER_INFO  BuffInfo;


    GOP_ENTRY();

    memset(&WinInfo, 0, sizeof(GOP_GWIN_SETWININFO_PARAM));
    memset(&BuffInfo, 0, sizeof(GOP_BUFFER_INFO));


    WinInfo.GwinId = gId;
    WinInfo.pinfo = (MS_U32*)&BuffInfo;
    WinInfo.u32Size = sizeof(GOP_BUFFER_INFO);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_WININFO,(void*)&WinInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    *x0 = BuffInfo.disp_rect.x;
    *y0 = BuffInfo.disp_rect.y;
    *width = BuffInfo.width;
    *height = BuffInfo.height;
    *fbPitch = BuffInfo.pitch;
    *fbFmt = BuffInfo.fbFmt;
    *addr = BuffInfo.addr;
    *size = BuffInfo.width * BuffInfo.height * GOP_GetBPP((EN_GOP_COLOR_TYPE)BuffInfo.fbFmt)/8;

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Get max V-End from all GWIN
/// @param bActive \b IN
///   - # TRUE enable
///   - # FALSE disable
/// @return max V-End value from all GWIN
/********************************************************************************/
MS_U16 MApi_GOP_GWIN_GetMaxVEnd(MS_BOOL bActive)
{

    GOP_ENTRY();
    GOP_ERR("[%s] Not SUPPORT\n",__FUNCTION__);
    GOP_RETURN(0);
}


/******************************************************************************/
/// Get maximum frame buffer number
/// @return maximum frame buffer number
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_GetMaxFBNum(void)
{
    MS_U32 u32MaxFBNum;
    u32MaxFBNum = MApi_GOP_GWIN_GetMax32FBNum ();
    if( u32MaxFBNum > 256) //Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( u32MaxFBNum:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_GetMax32FBNum()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MaxFBNum );
    }
    return u32MaxFBNum;
}

MS_U32 MApi_GOP_GWIN_GetMax32FBNum(void)
{
    MS_PHY u32FBIDNum=INVALID_POOL_NEXT_FBID;
    MApi_GOP_GetConfigEx(0, E_GOP_GET_MAXFBNUM, &u32FBIDNum);
    return u32FBIDNum;
}


/********************************************************************************/
/// Set second frame buffer pool
/// @param u32DblHeapAdr \b IN: second frame buffer pool starting address
/// @param u32DblHeapLen \b IN: second frame buffer pool length
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetDoubleHeap(MS_U32 u32DblHeapAdr, MS_U32 u32DblHeapLen)
{
    GOP_ERR("[%s] Not SUPPORT\n",__FUNCTION__);
    return GOP_API_SUCCESS;
}


/******************************************************************************/
/// Create frame buffer from given frame buffer id . User should call MApi_GOP_GWIN_GetFree32FBID()
/// first and then use this frame buffer id as this function input parameter
/// @param fbId  \b IN frame buffer id
/// @param dispX \b IN display x
/// @param dispY \b IN display y
/// @param width \b IN width
/// @param height \b IN height
/// @param fbFmt \b IN frame buffer format
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - Failure
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFB(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt)
{
    return MApi_GOP_GWIN_Create32FB((MS_U32)fbId, dispX, dispY, width, height, fbFmt);
}

MS_U8 MApi_GOP_GWIN_Create32FB(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt)
{
    GOP_CREATE_BUFFER_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_CREATE_BUFFER_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.fb_type  = GOP_CREATE_BUFFER;
    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32fbId;

    BuffInfo.addr = 0x0;
    BuffInfo.fbFmt = fbFmt;
    BuffInfo.disp_rect.x = dispX;
    BuffInfo.disp_rect.y = dispY;
    BuffInfo.disp_rect.w = width;
    BuffInfo.disp_rect.h = height;

    BuffInfo.width = width;
    BuffInfo.height = height;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_CREATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Create frame buffer from given frame buffer id and dram address. User should call MApi_GOP_GWIN_GetFree32FBID()
/// first and then use this frame buffer id as this function input parameter
/// @param fbId  \b IN: frame buffer id
/// @param dispX \b IN: frame buffer starting x
/// @param dispY \b IN: frame buffer starting y
/// @param width \b IN: frame buffer width
/// @param height \b IN: frame buffer height
/// @param fbFmt \b IN: frame buffer color format
/// @param u32FbAddr \b IN: frame buffer starting address
/// @param FBString \b IN: create frame buffer by which one application
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_CRT_GWIN_NOAVAIL - Failure
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFBbyStaticAddr2(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr ,EN_GOP_FRAMEBUFFER_STRING FBString)
{
    return MApi_GOP_GWIN_Create32FBbyStaticAddr2((MS_U32)fbId, dispX, dispY, width, height, fbFmt, phyFbAddr, FBString);
}

MS_U8 MApi_GOP_GWIN_Create32FBbyStaticAddr2(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr ,EN_GOP_FRAMEBUFFER_STRING FBString)
{
    GOP_CREATE_BUFFER_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_CREATE_BUFFER_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.fb_type  = GOP_CREATE_BUFFER_BYADDR;
    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32fbId;

    BuffInfo.addr = phyFbAddr;
    BuffInfo.fbFmt = fbFmt;
    BuffInfo.disp_rect.x = dispX;
    BuffInfo.disp_rect.y = dispY;
    BuffInfo.disp_rect.w = width;
    BuffInfo.disp_rect.h = height;

    BuffInfo.width = width;
    BuffInfo.height = height;
    BuffInfo.FBString = FBString;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_CREATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }


    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Create frame buffer from given dram address
/// @param width  \b IN: frame buffer width
/// @param height \b IN: frame buffer height
/// @param fbFmt \b IN: frame buffer color format
/// @param u32FbAddr \b IN: frame buffer starting address
/// @param pitch \b IN: frame buffer pitch
/// @param ret_FBId \b OUT: frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFBFrom3rdSurf(MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr, MS_U16 pitch, MS_U8* ret_FBId)
{
    MS_U32 u32ret_FBID = 0;
    MS_U8 u8result = 0;
    u8result = MApi_GOP_GWIN_Create32FBFrom3rdSurf(width, height, fbFmt, phyFbAddr, pitch, &u32ret_FBID);
    if(u32ret_FBID > 256)//Out of MS_U8
    {
        GOP_WARN( "[Warning] %s %d( ret_FBID:%td....)out of MS_U8 bound, please to use MApi_GOP_GWIN_Create32FBFrom3rdSurf()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32ret_FBID );
    }
    *ret_FBId = (MS_U8)u32ret_FBID;
    return u8result;
}

MS_U8 MApi_GOP_GWIN_Create32FBFrom3rdSurf(MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr, MS_U16 pitch, MS_U32* u32ret_FBId)
{

    GOP_CREATE_BUFFER_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;
    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_CREATE_BUFFER_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.fb_type  = GOP_CREATE_BUFFER_BYADDR;
    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    *u32ret_FBId = MApi_GOP_GWIN_GetFree32FBID();
    ioctl_info.fbid = *u32ret_FBId;

    BuffInfo.addr = phyFbAddr;
    BuffInfo.fbFmt = fbFmt;
    BuffInfo.disp_rect.x = 0;
    BuffInfo.disp_rect.y = 0;
    BuffInfo.disp_rect.w = width;
    BuffInfo.disp_rect.h = height;

    BuffInfo.width = width;
    BuffInfo.height = height;
    BuffInfo.pitch = pitch;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_CREATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }


    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Create frame buffer from given frame buffer id and dram address. User should call MApi_GOP_GWIN_GetFree32FBID()
/// first and then use this frame buffer id as this function input parameter
/// @param fbId  \b IN: frame buffer id
/// @param dispX \b IN: frame buffer starting x
/// @param dispY \b IN: frame buffer starting y
/// @param width \b IN: frame buffer width
/// @param height \b IN: frame buffer height
/// @param fbFmt \b IN: frame buffer color format
/// @param u32FbAddr \b IN: frame buffer starting address
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
MS_U8 MApi_GOP_GWIN_CreateFBbyStaticAddr(MS_U8 fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr)
{
     return MApi_GOP_GWIN_Create32FBbyStaticAddr((MS_U32)fbId, dispX, dispY, width, height,  fbFmt, phyFbAddr);
}

MS_U8 MApi_GOP_GWIN_Create32FBbyStaticAddr(MS_U32 u32fbId, MS_U16 dispX, MS_U16 dispY, MS_U16 width, MS_U16 height,  MS_U16 fbFmt, MS_PHY phyFbAddr)
{
     return MApi_GOP_GWIN_Create32FBbyStaticAddr2(u32fbId, dispX, dispY, width, height,  fbFmt, phyFbAddr, E_GOP_FB_NULL);
}

/******************************************************************************/
/// Set GE to render on the frame buffer
/// @param fbId \b IN: frame buffer id
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_Switch2FB(MS_U8 fbId)
{
    return MApi_GOP_GWIN_Switch2_32FB((MS_U32)fbId);
}

E_GOP_API_Result MApi_GOP_GWIN_Switch2_32FB(MS_U32 u32fbId)
{
    GOP_SELECTION_PROPERTY_PARAM ioctl_info;
    GOP_CBFmtInfo* pCBFmtInfo=NULL;

    GOP_ENTRY();
    pCBFmtInfo = (PGOP_CBFmtInfo)malloc(sizeof(GOP_CBFmtInfo));
	memset(pCBFmtInfo, 0x0, sizeof(GOP_CBFmtInfo));
    memset(&ioctl_info, 0x0, sizeof(GOP_SELECTION_PROPERTY_PARAM));
    ioctl_info.sel_type = EN_GOP_SEL_FB;
    ioctl_info.id = u32fbId;
    ioctl_info.pinfo = (MS_U32*)pCBFmtInfo;
    ioctl_info.u32Size = sizeof(GOP_CBFmtInfo);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SELECTION,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        free(pCBFmtInfo);
        GOP_RETURN( GOP_API_FAIL);
    }
    fpSetFBFmt(pCBFmtInfo->u16Pitch,pCBFmtInfo->u64Addr,pCBFmtInfo->u16Fmt);

    free(pCBFmtInfo);
    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Set Flip GWIN frame buffer
/// @param gWinId \b IN: gwin id
/// @param fbId \b IN: frame buffer id
/// @param u16WaitTagID \b IN: WaitTagID
/// @param pU16QueueCnt \b IN: Queue count
/// @return TRUE or FALSE
/******************************************************************************/
MS_BOOL MApi_GOP_Switch_GWIN_2_FB(MS_U8 gWinId, MS_U8 fbId, MS_U16 u16WaitTagID, MS_U16 *pU16QueueCnt)
{
    MS_U32 u32QueueCnt = 0;
    MS_BOOL bResult = FALSE;

    u32QueueCnt = (MS_U32)*pU16QueueCnt;
    bResult = MApi_GOP_Switch_GWIN_2_32FB(gWinId, (MS_U32)fbId, (MS_U32)u16WaitTagID, &u32QueueCnt);
    *pU16QueueCnt = (MS_U16)u32QueueCnt;
    if( u32QueueCnt > 65536) //Out of MS_U16
    {
        printf( "[Warning] %s %d( QueueCnt:%td....)out of MS_U16 bound, please to use MApi_GOP_Switch_GWIN_2_32FB()\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32QueueCnt);
    }

    return bResult;

}

MS_BOOL MApi_GOP_Switch_GWIN_2_32FB(MS_U8 gWinId, MS_U32 u32fbId, MS_U32 u32WaitTagID, MS_U32* pU32QueueCnt)
{

    GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM ioctl_info;
    GOP_GWIN_FLIP_WIN_INFO flipInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM));
    memset(&flipInfo, 0x0, sizeof(GOP_GWIN_FLIP_WIN_INFO));

    flipInfo.enFlip    = GOP_FLIP_BY_FBID;
    flipInfo.b3DEnable = FALSE;
    flipInfo.GwinId    = gWinId;
    flipInfo.FbId      = u32fbId;
    flipInfo.SubFbId   = u32fbId;
    flipInfo.WaitTagID = u32WaitTagID;
    flipInfo.pQueueCnt = (MS_U32*)pU32QueueCnt;

    ioctl_info.pFlipInfo = (void*)&flipInfo;
    ioctl_info.u32Size   = sizeof(GOP_GWIN_FLIP_WIN_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GFLIP_SWITCHGWIN,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(FALSE);
    }

    GOP_RETURN(TRUE);
}
MS_BOOL MApi_GOP_Switch_Multi_GWIN_2_FB_BY_ADDR(GOP_MultiFlipInfo GopMultiFlipInfo)
{
    GOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM ioctl_info;
    GOP_GWIN_FLIP_MULTI_WIN_INFO multiflipInfo;
    MS_U32 u32QueueCnt[GOP_MULTIINFO_NUM] = {0};
    MS_U8 i = 0;

    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_GFLIP_SWITCH_MULTI_GWIN_PARAM));
    memset(&multiflipInfo, 0x0, sizeof(GOP_GWIN_FLIP_MULTI_WIN_INFO));
    for(i =0;i < GOP_MULTIINFO_NUM;i++ )
        multiflipInfo.FlipInfo[i].pQueueCnt = &u32QueueCnt[i];

    multiflipInfo.u8InfoCnt = GopMultiFlipInfo.u8InfoCnt;
    for(i =0;i < (multiflipInfo.u8InfoCnt);i++ )
    {
        multiflipInfo.FlipInfo[i].enFlip = GOP_FLIP_BY_ADDR;
        multiflipInfo.FlipInfo[i].b3DEnable = FALSE;
        multiflipInfo.FlipInfo[i].GwinId = GopMultiFlipInfo.astGopInfo[i].gWinId;;
        multiflipInfo.FlipInfo[i].FlipAddr = GopMultiFlipInfo.astGopInfo[i].u32FlipAddr;;
        multiflipInfo.FlipInfo[i].SubFlipAddr = GopMultiFlipInfo.astGopInfo[i].u32SubAddr;
        multiflipInfo.FlipInfo[i].WaitTagID = GopMultiFlipInfo.astGopInfo[i].u16WaitTagID;

        /*One platform would coredump, check*/
        if(GopMultiFlipInfo.astGopInfo[i].pU16QueueCnt==NULL)
        {
            u32QueueCnt[i]=0;
        }
        else
        {
            memcpy(&u32QueueCnt[i], GopMultiFlipInfo.astGopInfo[i].pU16QueueCnt, sizeof(MS_U16));
        }
        //u32QueueCnt[i] =(MS_U32) *GopMultiFlipInfo.astGopInfo[i].pU16QueueCnt;
    }
    ioctl_info.pMultiFlipInfo = (void*)&multiflipInfo;
    ioctl_info.u32Size   = sizeof(GOP_GWIN_FLIP_MULTI_WIN_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GFLIP_SWITCHMULTIGWIN,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return FALSE;
    }
    for(i =0;i < (multiflipInfo.u8InfoCnt);i++ )
    {
        *(GopMultiFlipInfo.astGopInfo[i].pU16QueueCnt) = (MS_U16)u32QueueCnt[i];
    }
    return TRUE;
}

MS_BOOL MApi_GOP_Switch_GWIN_2_FB_BY_ADDR(MS_U8 gWinId, MS_PHY phyFlipAddr, MS_U16 u16WaitTagID, MS_U16 *pU16QueueCnt)
{
    GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM ioctl_info;
    GOP_GWIN_FLIP_WIN_INFO flipInfo;
    MS_U32 QueueCnt;


    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM));
    memset(&flipInfo, 0x0, sizeof(GOP_GWIN_FLIP_WIN_INFO));
    GOP_ENTRY();
    QueueCnt = *pU16QueueCnt;

    flipInfo.enFlip    = GOP_FLIP_BY_ADDR;
    flipInfo.b3DEnable = FALSE;
    flipInfo.GwinId    = gWinId;
    flipInfo.FlipAddr  = phyFlipAddr;
    flipInfo.SubFlipAddr = phyFlipAddr;
    flipInfo.WaitTagID = u16WaitTagID;
    flipInfo.pQueueCnt = &QueueCnt;

    ioctl_info.pFlipInfo = (void*)&flipInfo;
    ioctl_info.u32Size   = sizeof(GOP_GWIN_FLIP_WIN_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GFLIP_SWITCHGWIN,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(FALSE);
    }
    *pU16QueueCnt = QueueCnt;
    GOP_RETURN(TRUE);
}

//Flip out GOP 3D output
MS_BOOL MApi_GOP_Switch_3DGWIN_2_FB_BY_ADDR(MS_U8 gWinId, MS_PHY phyMainFlipAddr, MS_PHY phySubFlipAddr, MS_U16 u16WaitTagID, MS_U16 *pu16QueueCnt)
{

    GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM ioctl_info;
    GOP_GWIN_FLIP_WIN_INFO flipInfo;
    MS_U32 QueueCnt;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_GFLIP_SWITCH_GWIN_PARAM));
    memset(&flipInfo, 0x0, sizeof(GOP_GWIN_FLIP_WIN_INFO));

    QueueCnt = *pu16QueueCnt;

    flipInfo.enFlip    = GOP_FLIP_BY_ADDR;
    flipInfo.b3DEnable = TRUE;
    flipInfo.GwinId    = gWinId;
    flipInfo.FlipAddr  = phyMainFlipAddr;
    flipInfo.SubFlipAddr = phySubFlipAddr;
    flipInfo.WaitTagID = u16WaitTagID;
    flipInfo.pQueueCnt = &QueueCnt;

    ioctl_info.pFlipInfo = (void*)&flipInfo;
    ioctl_info.u32Size   = sizeof(GOP_GWIN_FLIP_WIN_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GFLIP_SWITCHGWIN,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(FALSE);
    }
    *pu16QueueCnt = QueueCnt;
    GOP_RETURN(TRUE);
}

/******************************************************************************/
/// Change a GWIN's frame buffer, this enables an off screen buffer to be shown
/// @param fbId \b IN frame buffer id
/// @param gwinId \b IN \copydoc GWINID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_MapFB2Win(MS_U8 fbId, MS_U8 gwinId)
{
    return MApi_GOP_GWIN_Map32FB2Win((MS_U32)fbId, gwinId);
}

E_GOP_API_Result MApi_GOP_GWIN_Map32FB2Win(MS_U32 u32fbId, MS_U8 u8gwinId)
{

    GOP_GWIN_MAPFBINFO_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_MAPFBINFO_PARAM));

    GOP_ENTRY();
    ioctl_info.fbid = u32fbId;
    ioctl_info.GwinId = u8gwinId;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_MAPFB2WIN,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Get frame buffer information
/// @param fbId \b IN frame buffer id
/// @param fbAttr \b OUT buffer to store \copydoc GOP_GwinFBAttr
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_GetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr)
{
    return MApi_GOP_GWIN_Get32FBInfo((MS_U32)fbId, fbAttr);
}

E_GOP_API_Result MApi_GOP_GWIN_Get32FBInfo(MS_U32 u32fbId, GOP_GwinFBAttr *fbAttr)
{
    GOP_FB_INFO_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_FB_INFO_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32fbId;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_GETINFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    memset(fbAttr, 0x0, sizeof(GOP_GwinFBAttr));

    fbAttr->width   = BuffInfo.width;
    fbAttr->height  = BuffInfo.height;
    fbAttr->fbFmt   = BuffInfo.fbFmt;
    fbAttr->pitch   = BuffInfo.pitch;
    fbAttr->x0      = BuffInfo.disp_rect.x;
    fbAttr->y0      = BuffInfo.disp_rect.y;
    fbAttr->x1      = BuffInfo.disp_rect.x + BuffInfo.disp_rect.w;
    fbAttr->y1      = BuffInfo.disp_rect.y + BuffInfo.disp_rect.h;
    fbAttr->s_x     = BuffInfo.disp_rect.x;
    fbAttr->s_y     = BuffInfo.disp_rect.y;
    fbAttr->addr    = BuffInfo.addr;

    fbAttr->size    = GOP_CalcPitch(BuffInfo.fbFmt, BuffInfo.width) * BuffInfo.height;

    //printf("[%s] buff size:%lx addr:%lx (%d,%d)\n",__FUNCTION__,fbAttr->size,fbAttr->addr, fbAttr->width, fbAttr->height);

    fbAttr->dispWidth  = BuffInfo.disp_rect.w;
    fbAttr->dispHeight = BuffInfo.disp_rect.h;

    //fbAttr->allocated = pwinFB->in_use;
    //fbAttr->enable = pwinFB->enable;
    //fbAttr->gWinId = pwinFB->gWinId;
    //fbAttr->string = pwinFB->string;

    GOP_RETURN(GOP_API_SUCCESS);

}


/******************************************************************************/
/// Set frame buffer info
/// @param fbId  \b IN: frame buffer id
/// @param fbAttr \b IN: frame buffer attribute
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetFBInfo(MS_U8 fbId, GOP_GwinFBAttr *fbAttr)
{
    return MApi_GOP_GWIN_Set32FBInfo((MS_U32)fbId, fbAttr);
}

E_GOP_API_Result MApi_GOP_GWIN_Set32FBInfo(MS_U32 u32fbId, GOP_GwinFBAttr *fbAttr)
{
    GOP_FB_INFO_PARAM ioctl_info;
    GOP_BUFFER_INFO BuffInfo;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_FB_INFO_PARAM));
    memset(&BuffInfo, 0x0, sizeof(GOP_BUFFER_INFO));

    BuffInfo.addr = fbAttr->addr;
    BuffInfo.disp_rect.x = fbAttr->x0;
    BuffInfo.disp_rect.y = fbAttr->y0;
    BuffInfo.disp_rect.w = fbAttr->x1 - fbAttr->x0;
    BuffInfo.disp_rect.h = fbAttr->y1 - fbAttr->y0;
    BuffInfo.fbFmt       = fbAttr->fbFmt;
    BuffInfo.pitch       = fbAttr->pitch;

    BuffInfo.width  = fbAttr->width;
    BuffInfo.height = fbAttr->height;


    ioctl_info.pBufInfo = (MS_U32*)&BuffInfo;
    ioctl_info.u32Size = sizeof(GOP_BUFFER_INFO);
    ioctl_info.fbid = u32fbId;

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_SETINFO,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


/********************************************************************************/
/// Set GWIN high priority when GWIN overlap in the same gop. If different gwins overlap in the same gop, only
/// high priority gwin can be shown
/// @param u8GOP \b IN:  GOP number
/// @param u8win \b IN: Gwin ID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SwapOverlapWin(MS_U8 u8GOP, MS_U8 u8win)
{
    MS_U8 u8WinId;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    u8WinId = u8win;

    ioctl_info.en_pro   = E_GOP_SWAP_OVERLAP_WIN;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&u8WinId;
    ioctl_info.u32Size  = sizeof(MS_U8);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Set GWIN relative Priority
/// Will Ensure Gwin Priority in the same sequence in hw as pGwinPri defined
/// @param u8GOP \b IN:  GOP number
/// @param pGwinPri \b IN: Relative Gwin ID Priority Defines
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_SetRelativeWinPrio(MS_U8 u8GOP, GOP_GwinPri *pGwinPri)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_GwinPri GwinPri;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    memcpy(&GwinPri, pGwinPri, sizeof(GOP_GwinPri));

    ioctl_info.en_pro   = E_GOP_RELATIVE_WIN_PRIO;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&GwinPri;
    ioctl_info.u32Size  = sizeof(GOP_GwinPri);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Switch frame buffer pool. This function is used for when user use MApi_GOP_GWIN_SetDoubleHeap to set second
/// frame buffer pool. User can control frame buffer opened in first or second buffer pool pool
/// @param u8FB \b IN 0: first frame buffer pool , 1: second frame buffer pool
/// @return GOP_API_SUCCESS - Success
/********************************************************************************/
E_GOP_API_Result MApi_GOP_FB_SEL(MS_U8 u8FB)
{
    GOP_FB_PROPERTY_PARAM ioctl_info;
    MS_U8 u8PoolId = u8FB;
    GOP_ENTRY();
    ioctl_info.en_property = E_GOP_FB_POOLID;
    ioctl_info.FBId = 0x0;
    ioctl_info.pSet = (void*)&u8PoolId;
    ioctl_info.u32Size = sizeof(MS_U8);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_FB_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Get GOP version number
///@param ppVersion \b OUT: GOP Version
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return GOP_API_INVALID_PARAMETERS;
    }
    *ppVersion = &_api_gop_version;
    return GOP_API_SUCCESS;
}

/********************************************************************************/
/// Set Debug level of GOP
/// @param level \b IN debug level
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetDbgLevel(EN_GOP_DEBUG_LEVEL level)
{
	GOP_MISC_PARAM ioctl_info;

    u32GOPDbgLevel_api= level;

    GOP_ENTRY();

    ioctl_info.misc_type = E_GOP_MISC_SET_DBG_LEVEL;
    ioctl_info.pMISC = (MS_U32*)&level;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);
}


/********************************************************************************/
/// GOP call back function: Set destination buffer pitch,addr,fmt to GE.
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterFBFmtCB(MS_U32 (*fpGOP_CB)(MS_U16 pitch,MS_PHY addr , MS_U16 fmt )  )
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpSetFBFmt = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_GFX_FBFMT;
        ioctl_info.pCB = fpSetFBFmt;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}



/********************************************************************************/
/// GOP call back function: Get vedio status (is interlace mode?) from scaler.
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterXCIsInterlaceCB(MS_BOOL (*fpGOP_CB)(void))
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpXCIsInterlace = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_XC_INTERLACE;
        ioctl_info.pCB = fpXCIsInterlace;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}

/********************************************************************************/
/// GOP call back function: Get capture window h-start from scaler.
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterXCGetCapHStartCB(MS_U16 (*fpGOP_CB)(void))
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpXCGetCapHStart = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_XC_PANEL_HSTART;
        ioctl_info.pCB = fpXCGetCapHStart;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}


/********************************************************************************/
/// GOP call back function: Notify callback for the end of GOP events such as buffer rearrange.
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterEventNotify(void (*fpGOP_CB)(MS_U32 u32EventID, void* reserved0))
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpEventNotify = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_EVENT_NOTIFY;
        ioctl_info.pCB = fpEventNotify;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}


/********************************************************************************/
/// GOP call back function: Reduce scaler bandwidth when OSD on.
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterXCReduceBWForOSDCB(void (*fpGOP_CB)(MS_U8 PqWin, MS_BOOL enable))
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpXCReduceBWForOSD = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_XC_REDUCE_BW;
        ioctl_info.pCB = fpXCReduceBWForOSD;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}
/********************************************************************************/
/// GOP call back function: Register XC DIP call back
/// This function should be registered before MApi_GOP_Init!!
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_RegisterXCSetDwinInfo(MS_BOOL (*fpGOP_CB)(MS_U16 cmd,MS_U32 *arg,MS_U16 size))
{
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    fpXCSetDwinInfo = fpGOP_CB;

#ifdef GOP_UTOPIA2K
#else
    if(pInstantGOP != NULL)
    {
        ioctl_info.cb_type = GOP_CB_DWIN_INFO;
        ioctl_info.pCB = fpXCSetDwinInfo;
        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
#endif
    return GOP_API_SUCCESS;
}


MS_U32 _GOP_RegisterAllCBFunc()
{
#ifdef GOP_UTOPIA2K
#else
    GOP_REGISTER_CB_PARAM ioctl_info;
    memset(&ioctl_info, 0x0, sizeof(GOP_REGISTER_CB_PARAM));

    if(fpSetFBFmt == NULL)
    {
#ifdef MSOS_TYPE_NOS
        GOP_INFO("[%s][%d] fpSetFBFmt is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
#endif
    }
    ioctl_info.cb_type = GOP_CB_GFX_FBFMT;
    ioctl_info.pCB = fpSetFBFmt;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    if(fpXCIsInterlace == NULL)
    {
        GOP_INFO("[%s][%d] fpXCIsInterlace is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
    }
    ioctl_info.cb_type = GOP_CB_XC_INTERLACE;
    ioctl_info.pCB = fpXCIsInterlace;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    if(fpXCGetCapHStart == NULL)
    {
        GOP_INFO("[%s][%d] fpXCGetCapHStart is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
    }
    ioctl_info.cb_type = GOP_CB_XC_PANEL_HSTART;
    ioctl_info.pCB = fpXCGetCapHStart;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    if(fpXCReduceBWForOSD == NULL)
    {
#ifdef MSOS_TYPE_NOS
        GOP_INFO("[%s][%d] fpXCReduceBWForOSD is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
#endif
    }
    ioctl_info.cb_type = GOP_CB_XC_REDUCE_BW;
    ioctl_info.pCB = fpXCReduceBWForOSD;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    if(fpEventNotify == NULL)
    {
#ifdef MSOS_TYPE_NOS
        GOP_INFO("[%s][%d] fpEventNotify is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
#endif
    }
    ioctl_info.cb_type = GOP_CB_EVENT_NOTIFY;
    ioctl_info.pCB = fpEventNotify;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }

    if(fpXCSetDwinInfo == NULL)
    {
#ifdef MSOS_TYPE_NOS
        GOP_INFO("[%s][%d] fpXCSetDwinInfo is NULL.(If on STR state, this is a normal message) \n",__FUNCTION__,__LINE__);
#endif
    }
    ioctl_info.cb_type = GOP_CB_DWIN_INFO;
    ioctl_info.pCB = fpXCSetDwinInfo;
    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_REGISTER_CB,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
#endif
    return GOP_API_SUCCESS;
}


/********************************************************************************/
/// Get gop static api info.
/// @param *pGopApiInfo \b OUT pointer to api info structure
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetInfo(GOP_ApiInfo* pGopApiInfo)
{
    GOP_MISC_PARAM ioctl_info;
    GOP_ApiInfo  GopApiInfo;

    GOP_ENTRY();

    memcpy(&GopApiInfo, pGopApiInfo, sizeof(GOP_ApiInfo));
    ioctl_info.misc_type = E_GOP_MISC_GET_INFO;
    ioctl_info.pMISC = (MS_U32*)&GopApiInfo;
    ioctl_info.u32Size  = sizeof(GOP_ApiInfo);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    memcpy(pGopApiInfo, &GopApiInfo, sizeof(GOP_ApiInfo));
    GOP_RETURN(GOP_API_SUCCESS);
}


/********************************************************************************/
/// Get gop run time api status.
/// @param *GOP_ApiStatus \b OUT pointer to api status structure
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetStatus(GOP_ApiStatus *pGopApiSts)
{
    GOP_MISC_PARAM ioctl_info;
    GOP_ApiStatus  GopApiSts;

    GOP_ENTRY();

    memcpy(&GopApiSts, pGopApiSts, sizeof(GOP_ApiStatus));
    ioctl_info.misc_type = E_GOP_MISC_GET_RT_STATUS;
    ioctl_info.pMISC = (MS_U32*)&GopApiSts;
    ioctl_info.u32Size  = sizeof(GOP_ApiStatus);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    memcpy(pGopApiSts, &GopApiSts, sizeof(GOP_ApiStatus));
    GOP_RETURN(GOP_API_SUCCESS);
}


/********************************************************************************/
/// Set GOP Power ON.
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_PowerOn(void)
{
    GOP_MISC_PARAM ioctl_info;
    MS_U32  value = 0;

    GOP_ENTRY();

    ioctl_info.misc_type = E_GOP_MISC_POWER_ON;
    ioctl_info.pMISC = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}
/********************************************************************************/
/// Set GOP Power OFF for saving power.
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_PowerOff(void)
{
    GOP_MISC_PARAM ioctl_info;
    MS_U32  value = 0;

    GOP_ENTRY();

    ioctl_info.misc_type = E_GOP_MISC_POWER_OFF;
    ioctl_info.pMISC = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


/********************************************************************************/
/// Set GOP bandwidth saving mode.
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetGOPBWStrength(EN_GOP_BW_STRENGTH eGOPBWStr, MS_U8 u8GOP)
{
    EN_GOP_BW_STRENGTH eBWStr;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    eBWStr = eGOPBWStr;

    ioctl_info.en_pro   = E_GOP_BW_STRENGTH;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&eBWStr;
    ioctl_info.u32Size  = sizeof(EN_GOP_BW_STRENGTH);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


/********************************************************************************/
/// Get GOP Power OFF for saving power.
/// @return EN_GOP_BW_STRENGTH - mode of GOP bw saving .
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetGOPBWStrength(MS_U8 u8GOP, EN_GOP_BW_STRENGTH* curBW)
{

    EN_GOP_BW_STRENGTH eBWStr;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_BW_STRENGTH;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&eBWStr;
    ioctl_info.u32Size  = sizeof(EN_GOP_BW_STRENGTH);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }
    * curBW = eBWStr;
    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Set GOP bandwidth saving mode.
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetGOPYUV(MS_U8 u8GOP)
{
    MS_U32  value = 0;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_YUV;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @param u8GOP \b IN:  GOP number
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetGOPHStart(MS_U8 u8GOP,MS_U16 u16PanelHStr)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32 u32PHstart;

    GOP_ENTRY();

    u32PHstart = u16PanelHStr;

    ioctl_info.en_pro   = E_GOP_HSTART;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = &u32PHstart;
    ioctl_info.u32Size  = sizeof(MS_U16);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_VE_SetOutputTiming(GOP_VE_TIMINGTYPE mode)
{
    GOP_ENTRY();

    GOP_VE_PROPERTY_PARAM ioctl_info;
    GOP_VE_TIMING_INFO VETimingInfo;

    VETimingInfo.mode = mode;

    ioctl_info.en_ve_property = E_GOP_VE_OUTPUT_TIMING;
    ioctl_info.pVEProp = (MS_U32*)&VETimingInfo;
    ioctl_info.u32Size = sizeof(GOP_VE_TIMING_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_VE_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Configure panel hstart timing for GOP; For configuring correct OSD position.
/// @param u16PanelHStr \b IN horizontal start
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_MIXER_SetOutputTiming(GOP_MIXER_TIMINGTYPE mode,  GOP_MixerTiming *pMT)
{
    GOP_ENTRY();

    GOP_MIXER_PROPERTY_PARAM ioctl_info;
    GOP_MIXER_TIMING_INFO MixerTimingInfo;

    memcpy(&MixerTimingInfo.pMT, pMT, sizeof(GOP_MixerTiming));
    MixerTimingInfo.mode = mode;

    ioctl_info.en_mixer_property = E_GOP_MIXER_OUTPUT_TIMING;
    ioctl_info.pMixerProp = (MS_U32*)&MixerTimingInfo;
    ioctl_info.u32Size = sizeof(GOP_MIXER_TIMING_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MIXER_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Configure mixer timing for GOP destination MIXER2OP; For configuring correct OSD position.
/// @param pMT \b IN:  mixer timing
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_MIXER_SetMIXER2OPOutputTiming(GOP_Mixer2OPTiming *pMT)
{
    GOP_ENTRY();

    GOP_MIXER_PROPERTY_PARAM ioctl_info;
    GOP_Mixer2OPTiming Mixer2OPTiming;

    memcpy(&Mixer2OPTiming, pMT, sizeof(GOP_Mixer2OPTiming));

    ioctl_info.en_mixer_property = E_GOP_MIXER_TO_OP_OUTPUT_TIMING;
    ioctl_info.pMixerProp = (MS_U32*)&Mixer2OPTiming;
    ioctl_info.u32Size = sizeof(GOP_Mixer2OPTiming);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MIXER_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Enable Mixer for Vfilter
/// @param bEnable \b IN:
///   - # TRUE Enable Vfilter
///   - # FALSE Disable Vfilter
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_MIXER_EnableVfilter(MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_MIXER_PROPERTY_PARAM ioctl_info;
    MS_BOOL bEna;

    bEna = bEnable;

    ioctl_info.en_mixer_property = E_GOP_MIXER_V_FILTER;
    ioctl_info.pMixerProp = (void*)&bEna;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MIXER_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/*******************************************************************************/
//Set which MUX select which GOP of mixer, when different gop do the alpha blending
/// @param u8GOP \b IN:  GOP number
/// @param u8Mux \b IN:  Mux number
/// @param bEn \b IN:
///   - # TRUE Enable u8GOP to mixer u8Mux
///   - # FALSE Disable u8GOP to mixer u8Mux
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
E_GOP_API_Result MApi_GOP_MIXER_SetMux(MS_U8 u8GOP,MS_U8 u8Mux, MS_BOOL bEn) //jasmine2
{
    GOP_ENTRY();

    //Need to implement


    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Enable Mixer for old mode blending
/// @param bEnable \b IN:
///   - # TRUE Enable old mode
///   - # FALSE Disable old mode
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_MIXER_EnableOldBlendMode(MS_U8 u8GOP, MS_BOOL bEnable)
{
    GOP_ENTRY();

    GOP_MIXER_PROPERTY_PARAM ioctl_info;
    GOP_MixerOldBlendingMode MixerOldBlendingMode;
    MixerOldBlendingMode.bEnable = bEnable;
    MixerOldBlendingMode.u8GOP = u8GOP;

    ioctl_info.en_mixer_property = E_GOP_MIXER_OLD_BLENDING_MODE;
    ioctl_info.pMixerProp = (void*)&MixerOldBlendingMode;
    ioctl_info.u32Size = sizeof(GOP_MixerOldBlendingMode);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MIXER_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}


//******************************************************************************
/// Call this API to Nofify GOP could restore from vsync. limitation. That
/// means App. could switch back to its dst.
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_RestoreFromVsyncLimitation(void)
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not SUPPORT\n",__FUNCTION__);
    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Configure GOP brightness
/// @param u8GOP \b IN:  GOP number
/// @param u16BriVal \b IN brightness value
/// @param bMSB \b IN MSB
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/

E_GOP_API_Result MApi_GOP_SetGOPBrightness(MS_U8 u8GOP,MS_U16 u16BriVal,MS_BOOL bMSB)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_BRIGHTNESS  bri;

    GOP_ENTRY();

    bri.bMSB = bMSB;
    bri.Brightness = u16BriVal;

    ioctl_info.en_pro   = E_GOP_BRIGHTNESS;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&bri;
    ioctl_info.u32Size  = sizeof(GOP_BRIGHTNESS);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/********************************************************************************/
/// Enable LB Couple Feature to enlarge GOP FIFO for BW issue
/// @param u8GOP \b IN:  GOP number
/// @param bEnable \b IN Enable or Disable
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/

E_GOP_API_Result MApi_GOP_EnableLBCouple(MS_U8 u8GOP, MS_BOOL bEnable)
{
    MS_BOOL bEna;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bEna = bEnable;

    ioctl_info.en_pro   = E_GOP_LB_COUPLE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/********************************************************************************/
/// Get GOP brightness value
/// @param u8GOP \b IN:  GOP number
/// @param u16BriVal \b OUT brightness value
/// @param bMSB \b OUT MSB
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetGOPBrightness(MS_U8 u8GOP,MS_U16* u16BriVal,MS_BOOL* bMSB)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_BRIGHTNESS  bri;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    memset(&bri, 0x0, sizeof(GOP_BRIGHTNESS));

    ioctl_info.en_pro   = E_GOP_BRIGHTNESS;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bri;
    ioctl_info.u32Size  = sizeof(GOP_BRIGHTNESS);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    *bMSB      = bri.bMSB;
    *u16BriVal = bri.Brightness;

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// API for enable or disable Tile mode for GWIN
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure

//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_EnableTileMode(MS_U8 winId, MS_BOOL bEnable, EN_GOP_TILE_DATA_TYPE tiletype )
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not SUPPORT bEn:%d type:%d\n",__FUNCTION__,bEnable, tiletype);
    GOP_RETURN(GOP_API_SUCCESS);
}


//******************************************************************************
/// API for set gop miu selection
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure

//******************************************************************************
E_GOP_API_Result MApi_GOP_MIUSel(MS_U8 u8GOP, EN_GOP_SEL_TYPE MiuSel )
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    EN_GOP_SEL_TYPE  miu;

    GOP_ENTRY();

    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    memset(&miu, 0x0, sizeof(EN_GOP_SEL_TYPE));


    miu = MiuSel;

    ioctl_info.en_pro   = E_GOP_MIUSEL;
    ioctl_info.pSetting = (MS_U32*)&miu;
    ioctl_info.gop_idx = u8GOP;
    ioctl_info.u32Size  = sizeof(EN_GOP_SEL_TYPE);


    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

//******************************************************************************
/// API for query gop miu selection
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure

//******************************************************************************
MS_U8 MApi_GOP_GetMIUSel(MS_U8 u8GOP)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    EN_GOP_SEL_TYPE  miu;
    GOP_ENTRY();
    ioctl_info.en_pro   = E_GOP_MIUSEL;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&miu;
    ioctl_info.u32Size  = sizeof(EN_GOP_SEL_TYPE);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN( (MS_U8)miu);
}


//******************************************************************************
/// API for set gop pinpong
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure

//******************************************************************************
E_GOP_API_Result MApi_GOP_SetPINPON(MS_U8 winId, MS_BOOL bEn,GOP_PINPON_MODE PINPON_Mode)
{
    GOP_SET_PINPON_INFO pinpon_info;
    switch(PINPON_Mode)
    {
        case GOP_PINPON_G3D:
            pinpon_info.mode = E_GOP_PINPON_G3D;
            break;
        case GOP_PINPON_VE:
            pinpon_info.mode = E_GOP_PINPON_VE;
            break;
        case GOP_PINPON_DWIN:
            pinpon_info.mode = E_GOP_PINPON_DWIN;
            break;
        case GOP_PINPON_DIP:
            pinpon_info.mode = E_GOP_PINPON_DIP;
            break;
        case GOP_PINPON_DWIN0:
            pinpon_info.mode = E_GOP_PINPON_DWIN0;
            break;
        case GOP_PINPON_DWIN1:
            pinpon_info.mode = E_GOP_PINPON_DWIN1;
            break;
        default:
            GOP_ERR("MApi_GOP_SetPINPON Enum not mapping!\n");
            return GOP_API_INVALID_PARAMETERS;
    }
    pinpon_info.GwinId  = winId;
    pinpon_info.bEnable = bEn;

    GOP_SET_PINPON_PARAM ioctl_info;
    GOP_ENTRY();
    ioctl_info.en_pro   = E_GOP_SET_PINPON;
    ioctl_info.pSetting = (MS_U32*)&pinpon_info;
    ioctl_info.u32Size  = sizeof(GOP_SET_PINPON_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PINPON,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_DWIN_SetBufferPINPON(MS_PHY phyfbaddr0,MS_PHY phyfbaddr1)
{
    GOP_ENTRY();

    GOP_DWIN_PROPERTY_PARAM ioctl_info;
    GOP_PINPON_INFO stPinpon;

    stPinpon.u64Addr0 = phyfbaddr0;
    stPinpon.u64Addr1 = phyfbaddr1;
    ioctl_info.en_dwin_property = E_GOP_DWIN_PINPON;
    ioctl_info.pDwinProp = (MS_U32*)&stPinpon;
    ioctl_info.u32Size = sizeof(GOP_PINPON_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_DWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}
//******************************************************************************
/// API for set U/V SWAP
/// @param u8GOP \b IN: GOP number
/// @param bEnable \b IN:
///   - # TRUE Enable U/V SWAP for GOP
///   - # FALSE Disable U/V SWAP for GOP
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_SetUVSwap(MS_U8 u8GOP, MS_BOOL bEn)
{

    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32  UVSwapEn;

    GOP_ENTRY();

    UVSwapEn = bEn;

    ioctl_info.en_pro   = E_GOP_UV_SWAP;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&UVSwapEn;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for set Y/C SWAP
/// @param u8GOP \b IN: GOP number
/// @param bEnable \b IN:
///   - # TRUE Enable Y/C SWAP for GOP
///   - # FALSE Disable Y/C SWAP for GOP
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_SetYCSwap(MS_U8 u8GOP, MS_BOOL bEn)
{

    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32  YCSwapEn;

    GOP_ENTRY();

    YCSwapEn = bEn;

    ioctl_info.en_pro   = E_GOP_YC_SWAP;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&YCSwapEn;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for set GWIN New Alpha Mode
/// @param gWinId \b IN: GWin ID
/// @param bEnable \b IN:
///   - # TRUE enable new alpha mode
///   - # FALSE disable new alpha mode
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetNewAlphaMode(MS_U8 gWinId, MS_BOOL bEnable)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();

    ioctl_info.en_property = E_GOP_GWIN_NEWAPLHA;
    ioctl_info.GwinId = gWinId;
    ioctl_info.pSet = (void*) &bEnable;
    ioctl_info.u32Size = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for Trigger reg writes in
/// @param u8win \b IN: GWin ID
/// @param bForceWriteIn \b IN:
///   - # TRUE , Register will take effect immediately
///   - # FALSE, Register will take effect after the next vsync
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_TriggerRegWriteIn(MS_U8 u8win, MS_BOOL bForceWriteIn)
{
    E_GOP_API_Result ret;
    ret = MApi_GOP_TriggerRegWriteIn_Ex(u8win, bForceWriteIn, TRUE);
    return ret;
}

//******************************************************************************
/// API for Trigger reg writes in
/// @param u8win \b IN: GWin ID
/// @param bForceWriteIn \b IN:
///   - # TRUE , Register will take effect immediately
///   - # FALSE, Register will take effect after the next vsync
/// @param bSync \b IN:
///   - # FALSE , Register will fire without waiting vsync (driver will not polling the vsync)
///   - # TRUE, Register will fire and waiting vsync
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_TriggerRegWriteIn_Ex(MS_U8 u8win, MS_BOOL bForceWriteIn, MS_BOOL bSync)
{
    GOP_UPDATE_PARAM ioctl_info;
    GOP_UPDATE_INFO update;
    GOP_GWIN_PROPERTY_PARAM ioctl_gwin_info;
    MS_U32 gop = INVAILD_GOP_NUM;
    GOP_ENTRY();

    ioctl_gwin_info.en_property = E_GOP_GWIN_GET_GOP;
    ioctl_gwin_info.GwinId = u8win;
    ioctl_gwin_info.pSet = (MS_U32*)&gop;
    ioctl_gwin_info.u32Size = sizeof(MS_U32);
    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_GET_PROPERTY,(void*)&ioctl_gwin_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    memset(&ioctl_info, 0x0, sizeof(GOP_UPDATE_PARAM));
    memset(&update, 0x0, sizeof(GOP_UPDATE_INFO));

    update.gop_idx = gop;
    update.update_type = E_GOP_UPDATE_FORCEWRITE;
    update.bEn = bForceWriteIn;
    update.bSync = bSync;

    ioctl_info.pUpdateInfo = (MS_U32*)&update;
    ioctl_info.u32Size = sizeof(GOP_UPDATE_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_UPDATE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);


}



E_GOP_API_Result MApi_GOP_SetGOPContrast( MS_U8 u8GOP, MS_U16 u16YContrast
    , MS_U16 u16UContrast, MS_U16 u16VContrast )
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_CONTRAST  contrast;

    GOP_ENTRY();

    contrast.y = u16YContrast;
    contrast.u = u16UContrast;
    contrast.v = u16VContrast;

    ioctl_info.en_pro   = E_GOP_CONTRAST;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&contrast;
    ioctl_info.u32Size  = sizeof(GOP_CONTRAST);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GetGOPContrast( MS_U8 u8GOP, MS_U16* u16YContrast
    , MS_U16* u16UContrast, MS_U16* u16VContrast )
{

    GOP_SET_PROPERTY_PARAM ioctl_info;
    GOP_CONTRAST  contrast;

    GOP_ENTRY();

    ioctl_info.en_pro   = E_GOP_CONTRAST;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&contrast;
    ioctl_info.u32Size  = sizeof(GOP_CONTRAST);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    *u16YContrast = contrast.y;
    *u16UContrast = contrast.u;
    *u16VContrast = contrast.v;

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GetVECaptureState(PMS_GOP_VECAPTURESTATE pstVECapState)
{
    GOP_ERR("[%s] Not SUPPORT in utopia2\n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
}

E_GOP_API_Result MApi_GOP_VECaptureWaitOnFrame(PMS_GOP_VECAPTURESTATE pstVECapState)
{
    GOP_ERR("[%s] Not SUPPORT in utopia2\n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
}

E_GOP_API_Result MApi_GOP_EnaVECapture(PMS_GOP_VECAPTURESTATE pstVECapState)
{
    GOP_ERR("[%s] Not SUPPORT in utopia2\n",__FUNCTION__);
    return GOP_API_FUN_NOT_SUPPORTED;
}

//******************************************************************************
/// API for Enable VEOSD
/// @param u8win \b IN: GOP ID
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_Enable_VEOSD(MS_U8 u8GOP, MS_BOOL bEn)
{
    MS_BOOL bEna;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));
    bEna = bEn;

    ioctl_info.en_pro   = E_GOP_VE_OSD;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


/*******************************************************************************/
//Set configuration for special cases
///        - This is for special case usage. Do NOT overuse this function
/// @param type \b IN:  Config type
///        - Think twice if you are going to add one type
///        - Be careful to create config type. Do NOT affect the normal code flow
/// @param plist \b IN:  Config list for the specific type
//         - This list could be an enum or a structure
//@return GOP_API_SUCCESS - Success
/*******************************************************************************/
E_GOP_API_Result MApi_GOP_SetConfig(EN_GOP_CONFIG_TYPE type, void *plist)
{
    GOP_SETCONFIG_PARAM ioctl_info;
    MS_U32 u32Size =0;

    CheckGOPInstanceOpen();

    switch(type)
    {
        case E_GOP_IGNOREINIT:
        {
            u32Size = sizeof(EN_GOP_IGNOREINIT);
            break;
        }
        case E_GOP_CONSALPHA_BITS:
        {
            u32Size = sizeof(EN_GOP_CONSALPHA_BITS);
            break;
        }
        default:
            u32Size = 0;
            break;
    }


    ioctl_info.cfg_type = type;
    ioctl_info.pCfg = plist;
    ioctl_info.u32Size = u32Size;

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_CONFIG,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }


    return GOP_API_SUCCESS;


}

/********************************************************************************/
/// Set config by GOP; For dynamic usage.
/// @param u8GOP \b IN:  GOP number
/// @param pstInfo \b IN misc info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_SetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, void *plist)
{
    E_GOP_API_CMD_TYPE enApiCmdType;
    GOP_SETCONFIG_PARAM ioctl_ConfigInfo;
    GOP_SET_PROPERTY_PARAM ioctl_PropInfo;
    void *pInfo = NULL;

    CheckGOPInstanceOpen();

    switch(enType)
    {
        case E_GOP_IGNOREINIT:
        {
            ioctl_ConfigInfo.cfg_type = enType;
            ioctl_ConfigInfo.pCfg = plist;
            ioctl_ConfigInfo.u32Size = sizeof(EN_GOP_IGNOREINIT);
            pInfo = (void*)&ioctl_ConfigInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_CONFIG;
            break;
        }
        case E_GOP_CONSALPHA_BITS:
        {
            ioctl_ConfigInfo.cfg_type = enType;
            ioctl_ConfigInfo.pCfg = plist;
            ioctl_ConfigInfo.u32Size = sizeof(EN_GOP_CONSALPHA_BITS);
            pInfo = (void*)&ioctl_ConfigInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_CONFIG;
            break;
        }
        case E_GOP_TIMING_INFO:
        {
            ioctl_PropInfo.en_pro = E_GOP_OUTPUT_TIMING;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(ST_GOP_TIMING_INFO);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_PROPERTY;
            break;
        }
        case E_GOP_IS_PIXELBASE_ENABLE:
        {
            ioctl_PropInfo.en_pro = E_GOP_PIXELBASE_ENABLE;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(MS_BOOL);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_PROPERTY;
            break;
        }
        case E_GOP_AFBC_RESET:
        {
            ioctl_PropInfo.en_pro = E_GOP_AFBC_CORE_RESET;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(MS_BOOL);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_PROPERTY;
            break;
        }        
        case E_GOP_AFBC_ENABLE:
        {
            ioctl_PropInfo.en_pro = E_GOP_AFBC_CORE_ENABLE;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(MS_BOOL);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_SET_PROPERTY;
            break;
        }
        default:
            return GOP_API_INVALID_PARAMETERS;
            break;
    }

    if (UtopiaIoctl(pInstantGOP, enApiCmdType, pInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
    return GOP_API_SUCCESS;
}

/********************************************************************************/
/// Get config by GOP.
/// @param u8GOP \b IN:  GOP number
/// @param pstInfo \b IN misc info
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/********************************************************************************/
E_GOP_API_Result MApi_GOP_GetConfigEx(MS_U8 u8Gop, EN_GOP_CONFIG_TYPE enType, void *plist)
{
    E_GOP_API_CMD_TYPE enApiCmdType;
    GOP_SETCONFIG_PARAM ioctl_ConfigInfo;
    GOP_SET_PROPERTY_PARAM ioctl_PropInfo;
    void *pInfo = NULL;

    CheckGOPInstanceOpen();

    switch(enType)
    {
        case E_GOP_IGNOREINIT:
        {
            ioctl_ConfigInfo.cfg_type = enType;
            ioctl_ConfigInfo.pCfg = plist;
            ioctl_ConfigInfo.u32Size = sizeof(EN_GOP_IGNOREINIT);
            pInfo = (void*)&ioctl_ConfigInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_CONFIG;
            break;
        }
        case E_GOP_CONSALPHA_BITS:
        {
            ioctl_ConfigInfo.cfg_type = enType;
            ioctl_ConfigInfo.pCfg = plist;
            ioctl_ConfigInfo.u32Size = sizeof(EN_GOP_CONSALPHA_BITS);
            pInfo = (void*)&ioctl_ConfigInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_CONFIG;
            break;
        }
        case E_GOP_TIMING_INFO:
        {
            ioctl_PropInfo.en_pro = E_GOP_OUTPUT_TIMING;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(ST_GOP_TIMING_INFO);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_PROPERTY;
            break;
        }
        case E_GOP_IS_PIXELBASE_ENABLE:
        {
            ioctl_PropInfo.en_pro = E_GOP_PIXELBASE_ENABLE;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(MS_BOOL);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_PROPERTY;
            break;
        }
        case E_GOP_GET_MAXFBNUM:
        {
            ioctl_PropInfo.en_pro = E_GOP_MAXFBNUM;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(MS_U32);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_PROPERTY;
            break;
        }
        case E_GOP_IS_INIT:
        {
            ioctl_PropInfo.en_pro = E_GOP_NUM_IS_INIT;
            ioctl_PropInfo.gop_idx = u8Gop;
            ioctl_PropInfo.pSetting = plist;
            ioctl_PropInfo.u32Size = sizeof(GOP_InitInfo);
            pInfo = (void*)&ioctl_PropInfo;
            enApiCmdType = MAPI_CMD_GOP_GET_PROPERTY;
            break;
        }
        default:
            return GOP_API_INVALID_PARAMETERS;
            break;
    }

    if (UtopiaIoctl(pInstantGOP, enApiCmdType, pInfo) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        return GOP_API_FAIL;
    }
    return GOP_API_SUCCESS;
}

//******************************************************************************
/// API for set GWIN resolution in one function
/// @param u8GwinId \b IN: GWin ID
/// @param u8FbId \b IN: Frame Buffer ID
/// @param pGwinInfo \b IN: pointer to GOP_GwinInfo structure
/// @param pStretchInfo \b IN: pointer to GOP_StretchInfo
/// @param direction \b IN: to decide which direction to stretch
/// @param u16DstWidth \b IN: set scaled width if H direction is specified
/// @param u16DstHeight \b IN: set scaled height if V direction is specified
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetResolution(MS_U8 u8GwinId, MS_U8 u8FbId
    , GOP_GwinInfo* pGwinInfo, GOP_StretchInfo* pStretchInfo, EN_GOP_STRETCH_DIRECTION direction
    , MS_U16 u16DstWidth, MS_U16 u16DstHeight)
{
    return MApi_GOP_GWIN_SetResolution_32FB(u8GwinId, (MS_U32)u8FbId, pGwinInfo, pStretchInfo, direction, u16DstWidth, u16DstHeight);
}

E_GOP_API_Result MApi_GOP_GWIN_SetResolution_32FB(MS_U8 u8GwinId, MS_U32 u32FbId
    , GOP_GwinInfo* pGwinInfo, GOP_StretchInfo* pStretchInfo, EN_GOP_STRETCH_DIRECTION direction
    , MS_U16 u16DstWidth, MS_U16 u16DstHeight)
{

    GOP_GWIN_DISPLAY_PARAM ioctl_info;
    GOP_GWINDISPLAY_INFO DispInfo;
    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_DISPLAY_PARAM));
    memset(&DispInfo, 0x0, sizeof(GOP_GWINDISPLAY_INFO));

    DispInfo.dir = direction;
    DispInfo.gwin = u8GwinId;
    DispInfo.fbid = u32FbId;
    DispInfo.dst_size.w = u16DstWidth;
    DispInfo.dst_size.h = u16DstHeight;

    memcpy(&DispInfo.gwin_info, pGwinInfo, sizeof(GOP_GwinInfo));
    memcpy(&DispInfo.stretch_info, pStretchInfo, sizeof(GOP_StretchInfo));

    ioctl_info.GwinId = u8GwinId;
    ioctl_info.u32Size = sizeof(GOP_GWINDISPLAY_INFO);
    ioctl_info.pDisplayInfo = (MS_U32*)&DispInfo;


    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SETDISPLAY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for change L/R frame for GOP 3D function
/// @param u8GOP \b IN: GOP ID
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_Set3D_LR_FrameExchange(MS_U8 u8GOP)
{
    GOP_SET_PROPERTY_PARAM ioctl_info;
    MS_U32 value = 0;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_3D_LR_EXCHANGE;
    ioctl_info.gop_idx  = u8GOP;
    ioctl_info.pSetting = (MS_U32*)&value;
    ioctl_info.u32Size  = sizeof(MS_U32);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for set GWIN 3D Mode
/// @param u8GwinId \b IN: GWin ID
/// @param u16MainFBId \b IN: Main-Frame Buffer ID
/// @param u16SubFBId \b IN: Sub-Frame Buffer ID
/// @param EN_3DOSD_Mode \b IN: 3D Osd mode select.
/// @return GOP_API_SUCCESS - Success
//******************************************************************************

E_GOP_API_Result MApi_GOP_Set3DOSDMode(MS_U8 u8Gwin, MS_U16 u16MainFBId, MS_U16 u16SubFBId,EN_GOP_3D_MODETYPE en3DMode)
{
    GOP_GWIN_PROPERTY_PARAM ioctl_info;
    GOP_GWIN_3D_MODE st3DMode;

    GOP_ENTRY();

    st3DMode.u32MainFBId = u16MainFBId;
    st3DMode.u32SubFBId = u16SubFBId;
    st3DMode.en3DMode = en3DMode;

    ioctl_info.en_property = E_GOP_GWIN_3D_MODE;
    ioctl_info.GwinId = u8Gwin;
    ioctl_info.pSet = (MS_U32*)&st3DMode;
    ioctl_info.u32Size = sizeof(GOP_GWIN_3D_MODE);

    if (UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GWIN_SET_PROPERTY,(void*)&ioctl_info) != UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

//******************************************************************************
/// API for notify kernel about 3D gpio pin
/// @param u32GPIO3DPin \b IN: GPIO 3D pin
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_Set_GPIO3DPin(MS_U32 u32GPIO3DPin)
{
    GOP_ERR("[%s] Not SUPPORT in utopia2\n",__FUNCTION__);

    return GOP_API_FAIL;
}

//******************************************************************************
/// API for Clear GOP GWIN FlipQueue
/// @param u8GwinId \b IN: GWin ID
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_ClearFlipQueue(MS_U8 gWinId)
{
    GOP_ENTRY();

    GOP_GWIN_CLEARQUEUE_PARAM ioctl_info;
    GOP_GWIN_CLEARQUEUE_INFO flipQinfo;

    memset(&ioctl_info, 0x0, sizeof(GOP_GWIN_CLEARQUEUE_PARAM));
    memset(&flipQinfo, 0x0, sizeof(GOP_GWIN_CLEARQUEUE_INFO));

    flipQinfo.GwinId = gWinId;

    ioctl_info.pClearInfo = (MS_U32*)&flipQinfo;
    ioctl_info.u32Size = sizeof(GOP_GWIN_CLEARQUEUE_INFO);


    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GFLIP_CLEARQUEUE,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN( GOP_API_SUCCESS);

}


//******************************************************************************
/// API for Query GOP Capability
/// @param eCapType \b IN: Capability type
/// @param pRet     \b OUT: return value
/// @param ret_size \b IN: input structure size
/// @return GOP_API_SUCCESS - Success
//******************************************************************************
E_GOP_API_Result MApi_GOP_GetChipCaps(EN_GOP_CAPS eCapType, void* pRet, MS_U32 ret_size)
{
    CheckGOPInstanceOpen();
    if(eCapType == E_GOP_CAP_VERSION)
    {
        char ver[] = MSIF_GOP_LIBVER;
        char blnum[] = MSIF_GOP_BUILDNUM;
        char cl[] = MSIF_GOP_CHANGELIST;
        GOP_LIB_VER * pLibVer;

        if(ret_size != sizeof(GOP_LIB_VER))
        {
            GOP_ERR("[%s] [%d]ERROR, invalid input structure size :0x%tx on E_GOP_CAP_VERSION\n",__FUNCTION__,__LINE__,(ptrdiff_t)ret_size);
            return GOP_API_FAIL;
        }

        pLibVer = (GOP_LIB_VER *)pRet;

        pLibVer->GOP_LibVer = atoi(ver);
        pLibVer->GOP_BuildNum = atoi(blnum);
        pLibVer->GOP_ChangeList = atoi(cl);

    }
    else if (eCapType == E_GOP_CAP_WORD_UNIT)
    {
        GOP_GETCAPS_PARAM gopcaps;
        MS_U16 u16Align = 0x0;

        memset(&gopcaps, 0x0, sizeof(GOP_GETCAPS_PARAM));

        gopcaps.caps = eCapType;
        gopcaps.pInfo = &u16Align;
        gopcaps.u32Size = sizeof(u16Align);

        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_CHIPCAPS,&gopcaps)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }

        if (ret_size == sizeof(MS_U8))
        {
            *((MS_U8 *)pRet) = (MS_U8)u16Align;
        }
        if (ret_size == sizeof(MS_U16))
        {
            *((MS_U16 *)pRet) = u16Align;
        }
        else if (ret_size == sizeof(MS_U32))
        {
            *((MS_U32 *)pRet) = (MS_U32)u16Align;
        }
    }
    else if (eCapType == E_GOP_CAP_GOP_MUX)
    {
        GOP_GETCAPS_PARAM gopcaps;
        PST_GOP_MUX_CAP pstDrvMuxCap = (PST_GOP_MUX_CAP)pRet;
        ST_DRV_GOP_MUX_CAP_EX stDrvMuxCapEx;
        MS_U8 u8num = 0;
        memset(&gopcaps, 0x0, sizeof(GOP_GETCAPS_PARAM));
        memset(&stDrvMuxCapEx, 0x0, sizeof(ST_DRV_GOP_MUX_CAP_EX));

        stDrvMuxCapEx.u8MuxNumber=pstDrvMuxCap->u8MuxNumber;
        for(u8num=0;u8num<pstDrvMuxCap->u8MuxNumber;u8num++)
        {
            stDrvMuxCapEx.pu8MuxProrityArray[u8num]=pstDrvMuxCap->pu8MuxProrityArray[u8num];
        }
        stDrvMuxCapEx.bNewMux = pstDrvMuxCap->bNewMux;
        stDrvMuxCapEx.bNeedSetMUX1ToIP0 = pstDrvMuxCap->bNeedSetMUX1ToIP0;
        stDrvMuxCapEx.bNeedSetMUX3ToIP0 = pstDrvMuxCap->bNeedSetMUX3ToIP0;

        gopcaps.caps = eCapType;
        gopcaps.pInfo = (MS_U32*)&stDrvMuxCapEx;
        gopcaps.u32Size = sizeof(ST_DRV_GOP_MUX_CAP_EX);;

        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_CHIPCAPS,&gopcaps)!= UTOPIA_STATUS_SUCCESS)
        {
            printf("Ioctl %s fail\n",__FUNCTION__);
            GOP_RETURN(GOP_API_FAIL);
        }
        pstDrvMuxCap->u8MuxNumber=stDrvMuxCapEx.u8MuxNumber;
        for(u8num=0;u8num<stDrvMuxCapEx.u8MuxNumber;u8num++)
        {
            pstDrvMuxCap->pu8MuxProrityArray[u8num]=stDrvMuxCapEx.pu8MuxProrityArray[u8num];
        }
        pstDrvMuxCap->bNewMux = stDrvMuxCapEx.bNewMux;
        pstDrvMuxCap->bNeedSetMUX1ToIP0 = stDrvMuxCapEx.bNeedSetMUX1ToIP0;
        pstDrvMuxCap->bNeedSetMUX3ToIP0 = stDrvMuxCapEx.bNeedSetMUX3ToIP0;
    }
    else
    {
        GOP_GETCAPS_PARAM gopcaps;

        memset(&gopcaps, 0x0, sizeof(GOP_GETCAPS_PARAM));

        gopcaps.caps = eCapType;
        gopcaps.pInfo = pRet;
        gopcaps.u32Size = ret_size;

        if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_CHIPCAPS,&gopcaps)!= UTOPIA_STATUS_SUCCESS)
        {
            GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
            return GOP_API_FAIL;
        }
    }
    return GOP_API_SUCCESS;
}

//******************************************************************************
/// API for Set Scroll rate for Gwin
/// @param winId \b IN: GWIN id
/// @param bEnable \b IN:
///   - # TRUE Enable Tile Mode for GWIN
///   - # FALSE Disable Tile Mode for GWIN
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure

//******************************************************************************
E_GOP_API_Result MApi_GOP_GWIN_SetScrollRate(MS_U8 u8rate)
{
    GOP_ENTRY();
    GOP_SCROLL_PARAM ioctl_info;
    MS_U8 u8ScrollRate;

    u8ScrollRate = u8rate;
    ioctl_info.scroll_type = E_GOP_SCROLL_RATE;
    ioctl_info.pCfg = (void*)&u8ScrollRate;
    ioctl_info.u32Size  = sizeof(MS_U8);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_SCROLL,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_SetScrollType(MS_U8 u8WinId, GOP_SCROLL_TYPE type,  MS_U16 u16Offset, MS_BOOL bEnable )
{
    GOP_ENTRY();
    GOP_SCROLL_PARAM ioctl_info;
    GOP_SCROLL_INFO scroll_info;

    scroll_info.u8WinId = u8WinId;
    scroll_info.type = type;
    scroll_info.u16Offset = u16Offset;
    scroll_info.bEnable = bEnable;

    ioctl_info.scroll_type = E_GOP_SCROLL_CONFIG;
    ioctl_info.pCfg = (MS_U32*)&scroll_info;
    ioctl_info.u32Size  = sizeof(GOP_SCROLL_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_SET_SCROLL,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


E_GOP_API_Result MApi_GOP_GWIN_SetSWScrollBuffer(MS_U16 bufNum, MS_U32* pBuffer)
{
    GOP_ENTRY();
    GOP_ERR("[%s] Not SUPPORT in utopia2\n",__FUNCTION__);
    GOP_RETURN(GOP_API_SUCCESS);


}

/******************************************************************************/
/// Map FBinfo to OCinfo
/// @param u32Addr\b IN Bank Start Addr
/// @param BankMode\b IN Bank Mode
/// @param enBufSize\b IN Bank Sizee
/******************************************************************************/
E_GOP_API_Result MApi_GOP_OC_SetOCFBinfo(MS_PHY phyAddr, EN_GOP_OC_SEL_TYPE BankMode, MS_U32 enBufSize)
{
    GOP_MISC_PARAM ioctl_info;
    GOP_OC_FB_INFO stOCFbInfo;

    GOP_ENTRY();

    stOCFbInfo.u64Addr = phyAddr;
    stOCFbInfo.BankMode = BankMode;
    stOCFbInfo.enBufSize = enBufSize;

    ioctl_info.misc_type = E_GOP_MISC_OC_FB_INFO;
    ioctl_info.pMISC = (MS_U32*)&stOCFbInfo;
    ioctl_info.u32Size  = sizeof(GOP_OC_FB_INFO);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}


//******************************************************************************
/// API for Test Pattern Type Select
/// @param eTstPatternType \b IN: Test Pattern Type
/// @param MS_U32 u32ARGB  \b IN: ARGB Color
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
//******************************************************************************
E_GOP_API_Result MApi_GOP_TestPatternAlpha_Enable(MS_BOOL bTSTAlpha)
{
    GOP_MISC_PARAM ioctl_info;
    MS_BOOL  bEna;

    GOP_ENTRY();

    bEna = bTSTAlpha;
    ioctl_info.misc_type = E_GOP_MISC_TESTPATTERN_ALPHA;
    ioctl_info.pMISC = (void*)&bEna;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_IsRegUpdated(MS_U8 u8GopType)
{
    MS_BOOL bUpdate = FALSE;
    GOP_SET_PROPERTY_PARAM ioctl_info;

    GOP_ENTRY();
    memset(&ioctl_info, 0x0, sizeof(GOP_SET_PROPERTY_PARAM));

    ioctl_info.en_pro   = E_GOP_REG_UPDATED;
    ioctl_info.gop_idx  = u8GopType;
    ioctl_info.pSetting = (void*)&bUpdate;
    ioctl_info.u32Size  = sizeof(MS_BOOL);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_GET_PROPERTY,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN((E_GOP_API_Result)bUpdate);
    }
    GOP_RETURN((E_GOP_API_Result)bUpdate);

}

E_GOP_API_Result MApi_GOP_TestPattern(EN_GOP_TST_PATTERN eTstPatternType, MS_U32 u32ARGB)
{
    GOP_TEST_PATTERN_PARAM TestPatternPar;

    GOP_ENTRY();

    memset(&TestPatternPar,0x0,sizeof(GOP_TEST_PATTERN_PARAM));

    TestPatternPar.gop_idx = 0x0;
    TestPatternPar.TestPatternType = eTstPatternType;
    TestPatternPar.u32ARGB = u32ARGB;
    TestPatternPar.u32Size = sizeof(GOP_TEST_PATTERN_PARAM);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_TESTPATTERN,&TestPatternPar)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

/******************************************************************************/
/// set GOP power state
/// @param enPowerState\b IN power status
/******************************************************************************/
E_GOP_API_Result MApi_GOP_SetPowerState(EN_POWER_MODE enPowerState)
{

    CheckGOPInstanceOpen();
    GOP_ENTRY();
    _GOP_RegisterAllCBFunc();
    GOP_POWERSTATE_PARAM PowerState;

    memset(&PowerState,0x0,sizeof(GOP_POWERSTATE_PARAM));
    PowerState.pInfo = &enPowerState;
    PowerState.u32Size = sizeof(GOP_POWERSTATE_PARAM);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_POWERSTATE,&PowerState)!= UTOPIA_STATUS_SUCCESS)
            {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }
    GOP_RETURN(GOP_API_SUCCESS);
}

E_GOP_API_Result MApi_GOP_GWIN_BeginDraw(void)
{
    GOP_MUTEX_PARAM stMutexPara;

    GOP_ENTRY();

    memset(&stMutexPara,0x0,sizeof(GOP_MUTEX_PARAM));
    stMutexPara.en_mutex = E_GOP_LOCK;
    stMutexPara.u32Size = sizeof(GOP_MUTEX_PARAM);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MUTEX,&stMutexPara)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);

}

E_GOP_API_Result MApi_GOP_GWIN_EndDraw(void)
{
    GOP_MUTEX_PARAM stMutexPara;

    GOP_ENTRY();

    memset(&stMutexPara,0x0,sizeof(GOP_MUTEX_PARAM));
    stMutexPara.en_mutex = E_GOP_UNLOCK;
    stMutexPara.u32Size = sizeof(GOP_MUTEX_PARAM);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MUTEX,&stMutexPara)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN(GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

/******************************************************************************/
/// API for GOP delete upexpect H/V win size for output
/// @param MS_U8 u8GOP\b IN: GOP ID
/// @param MS_U32 u32ARGB  \b IN: ARGB Color
/// @return GOP_API_SUCCESS - Success
/// @return GOP_API_FAIL - Failure
/******************************************************************************/
E_GOP_API_Result MApi_GOP_GWIN_DeleteWinHVSize(MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize)
{
    GOP_MISC_PARAM ioctl_info;
    GOP_DeleteWinSize_PARAM GOP_VaildSize;

    GOP_ENTRY();
    memset(&ioctl_info,0x0,sizeof(GOP_MISC_PARAM));
    memset(&GOP_VaildSize,0x0,sizeof(GOP_DeleteWinSize_PARAM));

    GOP_VaildSize.u32GOP_idx = u8GOP;
    GOP_VaildSize.u32HSize = (MS_U32)u16HSize;
    GOP_VaildSize.u32VSize = (MS_U32)u16VSize;

    ioctl_info.misc_type = E_GOP_MISC_DELETE_GWINSIZE;
    ioctl_info.pMISC = (MS_U32*)&GOP_VaildSize;
    ioctl_info.u32Size  = sizeof(GOP_DeleteWinSize_PARAM);

    if(UtopiaIoctl(pInstantGOP,MAPI_CMD_GOP_MISC,&ioctl_info)!= UTOPIA_STATUS_SUCCESS)
    {
        GOP_ERR("Ioctl %s fail\n",__FUNCTION__);
        GOP_RETURN( GOP_API_FAIL);
    }

    GOP_RETURN(GOP_API_SUCCESS);
}

#undef MApi_GOP_C
