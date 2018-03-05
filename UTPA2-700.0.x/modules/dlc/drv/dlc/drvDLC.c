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
/// file    drv_ACE.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#define  _MDRV_DLC_C_
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include "MsTypes.h"
#include "utopia.h"
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "mhal_dlc.h"
#include "drvDLC.h"
#include "drvDLC_Cus.h"
#include "drvDBC_Cus.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#if !(defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2))
    #include "../../../graphic/drv/gop/mdrv_gflip_io.h"
#endif

#elif defined(MSOS_TYPE_LINUX_KERNEL)

#if !(defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2))
    #include "../../../graphic/drv/gop/mdrv_gflip_io.h"
#endif

#endif

#include "../../../xc/drv/xc/mdrv_xc_st.h"
#include "../../../xc/drv/xc/mdrv_xc_io.h"

#include "UFO.h"

#define DLC_DEBUG(x)        //(x)
#define DLC_PRINT(fmt, args...)      printf("[Dlc (Driver)][%05d] " fmt, __LINE__, ## args)

#define EN_DLC_METHOD_AVERAGELUMA           0x10

#define DLC_DEFLICK_BLEND_FACTOR            32UL
#define DLC_DEFLICK_PRECISION_SHIFT         64UL
#define DLC_AVERAGELUMA_HISTORGRAM_FACTOR   32UL

extern MS_S32 _DLC_Mutex;

#if(ENABLE_AVERAGELUMA)
static MS_U16 gu16AverageLuma_Delta;
static MS_U16 gu16AverageLuma_boundary_L;
static MS_U16 gu16AverageLuma_boundary_ML;
static MS_U16 gu16AverageLuma_boundary_MH;
static MS_U16 gu16AverageLuma_boundary_H;
static MS_U16 gu16LumaSum;
static MS_U16 gu16PreLumaSum;
static MS_U16 gu16PixelSum;
static MS_BOOL gbCGain_Ctrl;
static MS_U16 g_u16PreDLC_CGain;
static MS_U16 gu16FlickAlpha;
#endif

extern MS_U8  g_ucTable[16];
extern MS_U16 g_uwPreTable[16]; // New de-Flick
extern MS_U8  g_ucTmpAvgN_1; // Avg value (N-1)
extern MS_U8  g_ucTmpAvgN;  // Avg value (current N)
//extern StuDlc_FinetuneParamaters_Lib g_DlcParameters;
extern StuDlc_FinetuneParamaters g_DlcParameters;
extern MS_U8 g_u8Chroma_OriGain;

#if(ENABLE_AVERAGELUMA)
static DLC_init_Ext gstDLCParamExt;
static MS_U16 g_u16TableExt[16];
static MS_U32 g_u32PreTableExt[16];
static MS_U16 g_u16SlowDonwStep = 0;
#endif
static MS_BOOL  g_bSetDlcHandlerOn = TRUE;

#ifdef MSOS_TYPE_LINUX
extern MS_S32   s32FdDlcBle ;
#endif

////////////////////////////////////////////////////////////////////////////////
//
//  DLC library function start
//
////////////////////////////////////////////////////////////////////////////////


MS_U8 DLC_VBuf[32] =  {
    'M','S','V','C','0','0',                //0,0 fixed
    'D','9',                                //LIB ID
    '3','2',                                //Interface
    '0','0','4','5',                        //Build number
    '0','0','1','0','5','9','6','4',        // Change List
    'A','E','0','0','0','0','0','0','0',    // AE:Saturn4LE  0: MStar Trunk
    'T'                                     // check sum
};

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetVersion()
/// @brief \b Function \b Description:  Get version of DLC library
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b Version
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_DLC_GetMFVer(void)
{
    return msDlcLibVersionCheck();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_InitCurve()
/// @brief \b Function \b Description:  Initiate DLC HW
/// @param <IN>        \b wHStart
/// @param <IN>        \b wHEnd
/// @param <IN>        \b wVStart
/// @param <IN>        \b wVEnd
/// @param <IN>        \b dwCustomMode
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
StuDbc_CAPTURE_Range g_DlcCapRange={0};

void MDrv_DLC_InitCurve (MS_U16 u16HStart, MS_U16 u16HEnd, MS_U16 u16VStart, MS_U16 u16VEnd)
{
    g_DlcCapRange.wHStart = u16HStart;
    g_DlcCapRange.wHEnd= u16HEnd;
    g_DlcCapRange.wVStart = u16VStart;
    g_DlcCapRange.wVEnd= u16VEnd;

    msDlcInitWithCurve(u16HStart, u16HEnd, u16VStart, u16VEnd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetOnOff()
/// @brief \b Function \b Description:  On/Off DLC function
/// @param <IN>        \b bSwitch
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_SetOnOff(MS_BOOL bSwitch, MS_BOOL bWindow)
{
    DLC_DEBUG(printf("MDrv_DLC_SetOnOff(): bSwitch: %x\n", bSwitch));

#if(ENABLE_AVERAGELUMA)
    if( Hal_DLC_get_dlc_method() & EN_DLC_METHOD_AVERAGELUMA)
    {
        Hal_DLC_set_onoff(bWindow, bSwitch);
    }
    else
#endif
    {
        msDlcOnOff(bSwitch, bWindow);
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable/disable DLC to control MDrv_DLC_Handler .
/// @param  bDlcEnable       \b IN:    # TRUE:  DLC Enable  # FALSE: DLC Disable
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set dlc info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DLC_SetDlcHandlerOnOff(MS_BOOL bSwitch)
{
    g_bSetDlcHandlerOn= bSwitch ;

#ifdef MSOS_TYPE_LINUX

    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("DLC device not opened!!!!\n");
            return FALSE;
        }
    }
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCONOFFINFO, &g_bSetDlcHandlerOn))
#else
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_DLCONOFFINFO, &g_bSetDlcHandlerOn))
#endif
    {
        DLC_PRINT("MDrv_DLC_SetDlcInfo fail!!!!\n");
        return FALSE;
    }

#endif
return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Enable/disable BLE control BLE function
/// @param  bBleEnable       \b IN:    # TRUE:  BLE Enable  # FALSE: BLE Disable
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set ble info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DLC_SetBleOnOff(MS_BOOL bSwitch)
{
    MS_BOOL  g_bSetBleOn = TRUE;

    msDlcSetBleOnOff(bSwitch);
    g_bSetBleOn = bSwitch ;

#ifdef MSOS_TYPE_LINUX

    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("BLE device not opened!!!!\n");
            return FALSE;
        }
    }

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCBLEONOFFINFO, &g_bSetBleOn))
#else
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_BLEONOFFINFO, &g_bSetBleOn))
#endif
    {
        DLC_PRINT("MDrv_DLC_SetBleOnOff fail!!!!\n");
        return FALSE;
    }

#endif
return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_EnableMainSubCurveSynchronization()
/// @brief \b Function \b Description:  On/Off DLC Set Curve both for main and sub function
/// @param <IN>        \b bEnable
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_EnableMainSubCurveSynchronization(MS_BOOL bEnable)
{
    msDlcEnableSetDLCCurveBoth(bEnable);
}

#if(ENABLE_AVERAGELUMA)
static MS_U16 MDrv_DLC_Get_AverageLuma_Alpha(void)
{
    MS_U16 u16AvgDiff;
    MS_U16 u16FlickAlpha;

    u16AvgDiff = (gu16LumaSum >= gu16PreLumaSum) ?
                  gu16LumaSum - gu16PreLumaSum :
                  gu16PreLumaSum - gu16LumaSum;

    u16FlickAlpha = (u16AvgDiff < gu16AverageLuma_Delta) ?
                    gu16FlickAlpha :
                    DLC_DEFLICK_BLEND_FACTOR;

    return u16FlickAlpha;

}


void MDrv_DLC_Set_SlowDwonStep(MS_BOOL bEn, MS_U8 u8Step)
{
    if(bEn)
    {
        g_u16SlowDonwStep = u8Step;
    }
    else
    {
        g_u16SlowDonwStep = 0;
    }
}

static void MDrv_DLC_AverateLuma_Handler_Ext(MS_BOOL bWindow)
{
    MS_U16 u16FlickAlpha;
    MS_U16 u16YAvg;
    MS_U16 *pStaticCurveTbl_a, *pStaticCurveTbl_b;
    MS_U16 u16Weight, u16Wegiht_Total;
    MS_U8 i;
    MS_U16 u16Step;

    u16FlickAlpha = MDrv_DLC_Get_AverageLuma_Alpha();

    if(u16FlickAlpha < 1)
        u16FlickAlpha = 1;
    if(u16FlickAlpha > DLC_DEFLICK_BLEND_FACTOR)
        u16FlickAlpha = DLC_DEFLICK_BLEND_FACTOR;

    if(gu16PixelSum != 0)
    {
        u16YAvg = (MS_U16)((256UL * (MS_U32)gu16LumaSum + (MS_U32)gu16PixelSum/2UL) / (MS_U32)gu16PixelSum);
    }
    else
    {
        u16YAvg = 0;
    }

    DLC_DEBUG(printf("FlickAlpha=%u, AvgLum=%u\n", u16FlickAlpha, u16YAvg));


    if(u16YAvg <= gu16AverageLuma_boundary_L)
    { // all dark
        DLC_DEBUG(printf("All Dark\n"));
        pStaticCurveTbl_a = &gstDLCParamExt.uwLumaCurve2_b[0];
        pStaticCurveTbl_b = &gstDLCParamExt.uwLumaCurve2_b[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_L;
        u16Weight       = gu16AverageLuma_boundary_L;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_ML)
    { // dark + normal
        DLC_DEBUG(printf("dark + normal\n"));
        pStaticCurveTbl_a = &gstDLCParamExt.uwLumaCurve2_b[0];
        pStaticCurveTbl_b = &gstDLCParamExt.uwLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_ML -  gu16AverageLuma_boundary_L;
        u16Weight = gu16AverageLuma_boundary_ML - u16YAvg;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_MH)
    { // all normal
        DLC_DEBUG(printf("all normal\n"));
        pStaticCurveTbl_a = &gstDLCParamExt.uwLumaCurve[0];
        pStaticCurveTbl_b = &gstDLCParamExt.uwLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_MH -  gu16AverageLuma_boundary_ML;
        u16Weight = u16Wegiht_Total;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_H)
    { // normal + light

        DLC_DEBUG(printf("normal light \n"));
        pStaticCurveTbl_a = &gstDLCParamExt.uwLumaCurve2_a[0];
        pStaticCurveTbl_b = &gstDLCParamExt.uwLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_H -  gu16AverageLuma_boundary_MH;
        u16Weight = u16YAvg - gu16AverageLuma_boundary_MH;

    }
    else
    { // all light

        DLC_DEBUG(printf("all light\n"));
        pStaticCurveTbl_a = &gstDLCParamExt.uwLumaCurve2_a[0];
        pStaticCurveTbl_b = &gstDLCParamExt.uwLumaCurve2_a[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_H;
        u16Weight = gu16AverageLuma_boundary_H;
    }

    DLC_DEBUG(printf("AverageLuma Target Curve\n"));
    /* update luma curve */
    for(i=0; i<16; i++)
    {
        if (u16Wegiht_Total == 0 )
        {
            return;
        }

        g_u16TableExt[i] = (MS_U16)((((MS_U32)(*pStaticCurveTbl_a) * (MS_U32) u16Weight) +
                             (MS_U32)(*pStaticCurveTbl_b) * (MS_U32)(u16Wegiht_Total - u16Weight)) / u16Wegiht_Total);
        pStaticCurveTbl_a++;
        pStaticCurveTbl_b++;

        DLC_DEBUG(printf("%04x, ", g_u16TableExt[i]));
    }
    DLC_DEBUG(printf("\n"));


    u16Step =  (g_u16SlowDonwStep == 0) ? Hal_DLC_get_step() : g_u16SlowDonwStep;


    DLC_DEBUG(printf("AverageLuma Final Curve: S:%x F:%x\n", u16Step, u16FlickAlpha));
    for (i = 0; i <16; i++)
    {
        MS_U32 u32Blend;
        MS_U32 u32a, u32b;
        MS_U32 u32F;

        u32a = ((MS_U32)g_u16TableExt[i] * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT);
        u32b = g_u32PreTableExt[i];

        if((u32a >= u32b))
        {
            u32F = ((u32a-u32b) > (DLC_DEFLICK_PRECISION_SHIFT<<2)) ? DLC_DEFLICK_BLEND_FACTOR : (MS_U32)u16FlickAlpha;
        }
        else if(u32b > u32a)
        {
            u32F = ((u32b - u32a) > (DLC_DEFLICK_PRECISION_SHIFT<<2)) ? DLC_DEFLICK_BLEND_FACTOR : (MS_U32)u16FlickAlpha;
        }

        /*++ de-flicker (Step2) ++*/
        u32Blend =( ( (u32F * (MS_U32)g_u16TableExt[i] * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT) +
                           ((MS_U32)DLC_DEFLICK_BLEND_FACTOR-u32F) * (MS_U32)g_u32PreTableExt[i]) / DLC_DEFLICK_BLEND_FACTOR);

        // the difference can't be too big
        if(u32Blend > g_u32PreTableExt[i])
        {
            if((u32Blend - g_u32PreTableExt[i])> ((DLC_DEFLICK_PRECISION_SHIFT<<2) * (MS_U32)u16Step))
                u32Blend = g_u32PreTableExt[i] + ((DLC_DEFLICK_PRECISION_SHIFT<<2) * (MS_U32)u16Step);
        }
        else if(u32Blend < g_u32PreTableExt[i])
        {
            if((g_u32PreTableExt[i] - u32Blend)> ((DLC_DEFLICK_PRECISION_SHIFT<<2) * (MS_U32)u16Step))
                u32Blend = g_u32PreTableExt[i] - ((DLC_DEFLICK_PRECISION_SHIFT<<2) * (MS_U32)u16Step);
        }

        g_u32PreTableExt[i] = u32Blend;

        g_u16TableExt[i] = (MS_U16)((g_u32PreTableExt[i] +(DLC_DEFLICK_PRECISION_SHIFT/2))/ DLC_DEFLICK_PRECISION_SHIFT);
        DLC_DEBUG(printf("%04x, ", g_u16TableExt[i]));
        /*-- de-flicker (Step2) --*/

    }
    DLC_DEBUG(printf("\n"));
    // patch first level for YUV 6~235
    // the middle point between 1st & 2nd level must be small or equal 0x10
    if(g_u16TableExt[1] + g_u16TableExt[0] > 0x80)
    {
        g_u16TableExt[0] = (g_u16TableExt[1] < 0x80) ? 0x80 - g_u16TableExt[1] : 0;
    }


    Hal_DLC_set_curveExt(&g_u16TableExt[0]);

}

static void MDrv_DLC_AverateLuma_Handler(MS_BOOL bWindow)
{
    MS_U16 u16FlickAlpha;
    MS_U16 u16YAvg;
    MS_U8 *pStaticCurveTbl_a, *pStaticCurveTbl_b;
    MS_U16 u16Weight, u16Wegiht_Total;
    MS_U8 i;
    MS_U16 u16Step;

    u16FlickAlpha = MDrv_DLC_Get_AverageLuma_Alpha();

    if(u16FlickAlpha < 1)
        u16FlickAlpha = 1;
    if(u16FlickAlpha > DLC_DEFLICK_BLEND_FACTOR)
        u16FlickAlpha = DLC_DEFLICK_BLEND_FACTOR;


    if(gu16PixelSum != 0)
    {
        u16YAvg = (MS_U16)((256UL * (MS_U32)gu16LumaSum + (MS_U32)gu16PixelSum/2UL) / (MS_U32)gu16PixelSum);
    }
    else
    {
        u16YAvg = 0;
    }

    DLC_DEBUG(printf("CurLumaSum=%u, PreLumaSum=%u, FlickAlpha=%u, AvgLum=%u\n",
              gu16LumaSum, gu16PreLumaSum, u16FlickAlpha, u16YAvg));


    if(u16YAvg <= gu16AverageLuma_boundary_L)
    { // all dark
        DLC_DEBUG(printf("All Dark\n"));
        pStaticCurveTbl_a = &g_DlcParameters.ucLumaCurve2_b[0];
        pStaticCurveTbl_b = &g_DlcParameters.ucLumaCurve2_b[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_L;
        u16Weight       = gu16AverageLuma_boundary_L;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_ML)
    { // dark + normal
        DLC_DEBUG(printf("dark + normal\n"));
        pStaticCurveTbl_a = &g_DlcParameters.ucLumaCurve2_b[0];
        pStaticCurveTbl_b = &g_DlcParameters.ucLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_ML -  gu16AverageLuma_boundary_L;
        u16Weight = gu16AverageLuma_boundary_ML - u16YAvg;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_MH)
    { // all normal
        DLC_DEBUG(printf("all normal\n"));
        pStaticCurveTbl_a = &g_DlcParameters.ucLumaCurve[0];
        pStaticCurveTbl_b = &g_DlcParameters.ucLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_MH -  gu16AverageLuma_boundary_ML;
        u16Weight = u16Wegiht_Total;

    }
    else if(u16YAvg <= gu16AverageLuma_boundary_H)
    { // normal + light

        DLC_DEBUG(printf("normal light \n"));
        pStaticCurveTbl_a = &g_DlcParameters.ucLumaCurve2_a[0];
        pStaticCurveTbl_b = &g_DlcParameters.ucLumaCurve[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_H -  gu16AverageLuma_boundary_MH;
        u16Weight = u16YAvg - gu16AverageLuma_boundary_MH;

    }
    else
    { // all light

        DLC_DEBUG(printf("all light\n"));
        pStaticCurveTbl_a = &g_DlcParameters.ucLumaCurve2_a[0];
        pStaticCurveTbl_b = &g_DlcParameters.ucLumaCurve2_a[0];
        u16Wegiht_Total = gu16AverageLuma_boundary_H;
        u16Weight = gu16AverageLuma_boundary_H;
    }

    DLC_DEBUG(printf("AverageLuma Target Curve\n"));
    /* update luma curve */
    for(i=0; i<16; i++)
    {

        if (u16Wegiht_Total == 0 )
        {
            DLC_DEBUG(printf(" =================================================================== \n"));
            DLC_DEBUG(printf(" ==== [DLC]Error divide 0 !!!!!!! ================================== \n"));
            DLC_DEBUG(printf(" =================================================================== \n"));
            return;
        }
        else
        {
        g_ucTable[i] = (MS_U8)((((MS_U32)(*pStaticCurveTbl_a) * (MS_U32) u16Weight) +
                             (MS_U32)(*pStaticCurveTbl_b) * (MS_U32)(u16Wegiht_Total - u16Weight)) / u16Wegiht_Total);
        }
        pStaticCurveTbl_a++;
        pStaticCurveTbl_b++;

        DLC_DEBUG(printf("%02x, ", g_ucTable[i]));
    }
    DLC_DEBUG(printf("\n"));


    u16Step =  Hal_DLC_get_step();


    DLC_DEBUG(printf("AverageLuma Final Curve\n"));
    for (i = 0; i <16; i++)
    {
        MS_U16 u16Blend;
        MS_U32 u32a, u32b;
        MS_U16 u16F;

        u32a = ((MS_U32)g_ucTable[i] * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT);
        u32b = (MS_U32)g_uwPreTable[i];

        if((u32a >= u32b))
        {
            u16F = ((u32a-u32b) > DLC_DEFLICK_PRECISION_SHIFT) ? DLC_DEFLICK_BLEND_FACTOR : u16FlickAlpha;
        }
        else if(u32b > u32a)
        {
            u16F = ((u32b - u32a) > DLC_DEFLICK_PRECISION_SHIFT) ? DLC_DEFLICK_BLEND_FACTOR : u16FlickAlpha;
        }

        /*++ de-flicker (Step2) ++*/
        u16Blend =(MS_U16)( ( (((MS_U32)u16F) * (MS_U32)g_ucTable[i] * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT) +
                           ((MS_U32)DLC_DEFLICK_BLEND_FACTOR-(MS_U32)u16F) * (MS_U32)g_uwPreTable[i]) / DLC_DEFLICK_BLEND_FACTOR);

        // the difference can't be too big
        if(u16Blend > g_uwPreTable[i])
        {
            if((u16Blend - g_uwPreTable[i])> (DLC_DEFLICK_PRECISION_SHIFT * (MS_U16)u16Step))
                u16Blend = g_uwPreTable[i] + (DLC_DEFLICK_PRECISION_SHIFT * (MS_U16)u16Step);
        }
        else if(u16Blend < g_uwPreTable[i])
        {
            if((g_uwPreTable[i] - u16Blend)> (DLC_DEFLICK_PRECISION_SHIFT * (MS_U16)u16Step))
                u16Blend = g_uwPreTable[i] - (DLC_DEFLICK_PRECISION_SHIFT * (MS_U16)u16Step);
        }

        g_uwPreTable[i] = u16Blend;

        g_ucTable[i] = (MS_U8)(g_uwPreTable[i] / DLC_DEFLICK_PRECISION_SHIFT);

        DLC_DEBUG(printf("%02x, ", g_ucTable[i]));
        /*-- de-flicker (Step2) --*/

    }
    DLC_DEBUG(printf("\n"));

    // patch first level for YUV 6~235
    // the middle point between 1st & 2nd level must be small or equal 0x10

    if(g_ucTable[1] + g_ucTable[0] > 0x20)
    {
        g_ucTable[0] = (g_ucTable[1] < 0x20) ? 0x20 - g_ucTable[1] : 0;
    }


    Hal_DLC_set_curve(&g_ucTable[0]);

}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_Handler()
/// @brief \b Function \b Description:  Handle DLC function
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_Handler(MS_BOOL bWindow)
{
    static MS_U8 s_uLastMsg = 0;

#if(ENABLE_AVERAGELUMA)
    MS_U8 u8Method;


    u8Method = Hal_DLC_get_dlc_method();


    if( u8Method & EN_DLC_METHOD_AVERAGELUMA)
    {
        if (s_uLastMsg != 1)
        {
            s_uLastMsg = 1;
            DLC_DEBUG(printf("MDrv_DLC_Handler(): Hal_DLC_get_dlc_method() EN_DLC_METHOD_AVERAGELUMA on\n"));
        }

        if(gstDLCParamExt.b10BitsCruveEn)
        {
            MDrv_DLC_AverateLuma_Handler_Ext(bWindow);
        }
        else
        {
            MDrv_DLC_AverateLuma_Handler(bWindow);
        }
    }
    else
#endif
    {
        if (s_uLastMsg != 2)
        {
            s_uLastMsg = 2;
            DLC_DEBUG(printf("MDrv_DLC_Handler(): Hal_DLC_get_dlc_method() EN_DLC_METHOD_AVERAGELUMA off\n"));
        }

        if(g_bSetDlcHandlerOn)
        {
            msDlcHandler(bWindow);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetHistogram()
/// @brief \b Function \b Description:  Get Histogram
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b ucReturn
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_GetHistogram(MS_BOOL bWindow)
{
    MS_BOOL Ret;

    Ret = msGetHistogramHandler(bWindow);

#if(ENABLE_AVERAGELUMA)
    if(Hal_DLC_get_dlc_method() & EN_DLC_METHOD_AVERAGELUMA)
    {
        gu16AverageLuma_Delta = Hal_DLC_get_averageluma_delta();
        gbCGain_Ctrl = Hal_DLC_get_c_gain_ctrl();
        gu16FlickAlpha = Hal_DLC_get_flick_alpha();

        if(Ret == TRUE)
        {
            gu16PreLumaSum = gu16LumaSum;
            gu16LumaSum = Hal_DLC_get_histogram_y_sum();
            gu16PixelSum = Hal_DLC_get_histogam_pixel_sum();
        }

        DLC_DEBUG(printf("PreYSum=%u, YSum=%u, PixelSum=%u\n", gu16PreLumaSum , gu16LumaSum, gu16PixelSum));
        gu16AverageLuma_boundary_L  = Hal_DLC_get_averageluma_boundary_low();
        gu16AverageLuma_boundary_ML = Hal_DLC_get_averageluma_boundary_midlow();
        gu16AverageLuma_boundary_MH = Hal_DLC_get_averageluma_boundary_midhigh();
        gu16AverageLuma_boundary_H  = Hal_DLC_get_averageluma_boundary_high();
    }
#endif

    return Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetAverageValue()
/// @brief \b Function \b Description:  Get Average of Luminous Value
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b uwTmpAvr
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_DLC_GetAverageValue(void)
{
    MS_U8 u8ret;

    u8ret = msGetAverageLuminous();

    return u8ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetAverageValue_x4()
/// @brief \b Function \b Description:  Get Average exactness of Luminous Value
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b uwTmpAvr
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_DLC_GetAverageValue_x4(void)
{
    MS_U16 u16ret;

    u16ret = msGetAverageLuminous_x4();

    return u16ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SpeedupTrigger()
/// @brief \b Function \b Description:  Speedup Trigger
/// @param <IN>        \b ucLoop
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_SpeedupTrigger(MS_U8 u8Loop)
{
    msDlcSpeedupTrigger(u8Loop);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ResetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_ResetCGain(void)
{
    Hal_DLC_CGC_ResetCGain();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_DLC_CGC_GetCGain(void)
{
    MS_U8 u8ret;

    u8ret = Hal_DLC_CGC_GetCGain();
    return u8ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_SetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_SetCGain(MS_U8 u8CGain)
{
    Hal_DLC_CGC_SetCGain(u8CGain);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ResetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_ResetYGain(void)
{
    Hal_DLC_CGC_ResetYGain();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_DLC_CGC_GetYGain(void)
{
    MS_U8 u8ret;

    u8ret = Hal_DLC_CGC_GetYGain();
    return u8ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_SetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_SetYGain(MS_U8 u8YGain)
{
    Hal_DLC_CGC_SetYGain(u8YGain);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_Init(void)
{
#if(ENABLE_AVERAGELUMA)
    if( Hal_DLC_get_dlc_method() & EN_DLC_METHOD_AVERAGELUMA)
    {
        DLC_DEBUG(printf("MDrv_DLC_CGC_Init(): Hal_DLC_get_dlc_method() EN_DLC_METHOD_AVERAGELUMA on\n"));
        if(Hal_DLC_get_c_gain_ctrl())
        {
            g_u16PreDLC_CGain = 0x40 * DLC_DEFLICK_PRECISION_SHIFT;
            g_u8Chroma_OriGain = Hal_DLC_CGC_GetCGain();
        }
    }
    else
#endif
    {
        DLC_DEBUG(printf("MDrv_DLC_CGC_Init(): Hal_DLC_get_dlc_method() EN_DLC_METHOD_AVERAGELUMA off\n"));
        msDlc_CGC_Init();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ReInit()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_ReInit(void)
{
    msDlc_CGC_ReInit();
}

#if(ENABLE_AVERAGELUMA)
static void MDrv_DLC_AverageLuma_CGC_Handler(void)
{
    MS_U16 u16CGain_Blend;
    MS_U16 u16FlickAlpha;
    MS_U16 u16Step;
    MS_U16 u16NewDLC_CGain;
    MS_U16 u16YAvg;
    MS_U16 u16Strength;
    MS_U16 u16Center_Boundary;

    u16YAvg = msGetAverageLuminous();

    u16Strength = Hal_DLC_get_c_gain_strength();

    u16Center_Boundary = (gu16AverageLuma_boundary_ML  + gu16AverageLuma_boundary_MH)/2;


    if(u16YAvg > u16Center_Boundary)
        u16YAvg = u16Center_Boundary - (u16YAvg - u16Center_Boundary);

    if(u16YAvg < gu16AverageLuma_boundary_L)
    {
        u16NewDLC_CGain = (u16YAvg * u16Strength)/256;
    }
    else if(u16YAvg < gu16AverageLuma_boundary_ML)
    {
        u16NewDLC_CGain = (u16YAvg * u16Strength)/192;
    }
    else //(u16YAvg <= u16Center_Boundary)
    {
        u16NewDLC_CGain = (u16YAvg * u16Strength)/128;
    }

    u16NewDLC_CGain += g_u8Chroma_OriGain;
    // DLC_DEBUG(printk("CenterP =%x, AvergaeLuma=%x, YAvg=%bx, New_CGain=%x\n",
    //     u16Center_Boundary, u16YAvg, u16YAvg, u16NewDLC_CGain));

    if(u16NewDLC_CGain > 0x60)
        u16NewDLC_CGain = 0x60;


    u16FlickAlpha = MDrv_DLC_Get_AverageLuma_Alpha();

    u16Step = Hal_DLC_get_step();

    u16CGain_Blend =(MS_U16)( ( (((MS_U32)u16FlickAlpha) * (MS_U32)u16NewDLC_CGain * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT) +
                       ((MS_U32)DLC_DEFLICK_BLEND_FACTOR-(MS_U32)u16FlickAlpha) * (MS_U32)g_u16PreDLC_CGain) / DLC_DEFLICK_BLEND_FACTOR);
    if((u16CGain_Blend > g_u16PreDLC_CGain) )
    {
        if((u16CGain_Blend - g_u16PreDLC_CGain) > (DLC_DEFLICK_PRECISION_SHIFT * u16Step))
            u16CGain_Blend =  g_u16PreDLC_CGain + (DLC_DEFLICK_PRECISION_SHIFT * u16Step);
    }
    else if(u16CGain_Blend < g_u16PreDLC_CGain)
    {
        if((g_u16PreDLC_CGain - u16CGain_Blend ) > (DLC_DEFLICK_PRECISION_SHIFT * u16Step))
            u16CGain_Blend =  g_u16PreDLC_CGain - (DLC_DEFLICK_PRECISION_SHIFT * u16Step);

    }

    g_u16PreDLC_CGain = u16CGain_Blend;

    u16NewDLC_CGain = u16CGain_Blend / DLC_DEFLICK_PRECISION_SHIFT;

    Hal_DLC_CGC_SetCGain((MS_U8)(u16NewDLC_CGain & 0xFF));
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_Handler()
/// @brief \b Function \b Description:  Handle DLC CGC function
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_CGC_Handler(void)
{
#if(ENABLE_AVERAGELUMA)
    MS_U8 u8Method;
    MS_BOOL bCGainCtrl;

    u8Method = Hal_DLC_get_dlc_method();
    bCGainCtrl = Hal_DLC_get_c_gain_ctrl();

    if( u8Method & EN_DLC_METHOD_AVERAGELUMA)
    {
        if(bCGainCtrl)
        {
            MDrv_DLC_AverageLuma_CGC_Handler();
        }
    }
    else
#endif
    {
        msDlc_CGC_Handler();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetMFVer()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_DLC_CGC_GetMFVer(void)
{
    return msDlcCGCLibVersionCheck();
}

////////////////////////////////////////////////////////////////////////////////
//
//  DLC library function end
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  DrvDLC.c start
//
////////////////////////////////////////////////////////////////////////////////
extern  MS_U16 g_wLumaHistogram32H[32];
extern  MS_U8 g_ucHistogramMax, g_ucHistogramMin;
extern  MS_U16 g_wLumiAverageTemp;
extern  MS_U8 g_ucTable[16];
extern  MS_U16 g_wLumiTotalCount;

void MDrv_DLC_SetCurve(MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark )
{
    MS_U8 i;
    for (i=0; i<16; ++i)
    {
#if(ENABLE_AVERAGELUMA)
        gstDLCParamExt.uwLumaCurve[i]     = ((MS_U16)pNormal[i])<<2;
        gstDLCParamExt.uwLumaCurve2_a[i]  = ((MS_U16)pLight[i])<<2;
        gstDLCParamExt.uwLumaCurve2_b[i]  = ((MS_U16)pDark[i])<<2;
#endif

        g_DlcParameters.ucLumaCurve[i]     = pNormal[i];
        g_DlcParameters.ucLumaCurve2_a[i]  = pLight[i];
        g_DlcParameters.ucLumaCurve2_b[i]  = pDark[i];

        DLC_DEBUG(printf("%x %x %x\n",
            gstDLCParamExt.uwLumaCurve[i] ,
            gstDLCParamExt.uwLumaCurve2_a[i],
            gstDLCParamExt.uwLumaCurve2_b[i]));
    }
}

#ifdef MSOS_TYPE_LINUX_KERNEL
extern MS_BOOL KDrv_DLC_SetInitInfo(ST_KDRV_XC_DLC_INIT_INFO stDlcInitInfo);
#endif
void MDrv_DLC_Init(StuDlc_FinetuneParamaters DLC_MFinit)
{
    MS_U8 i;

    char word[] = {"_XC_Mutex"};
    _DLC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, word, MSOS_PROCESS_SHARED);

    if (_DLC_Mutex == -1)
    {
        (printf("[MAPI DLC][%06d] create mutex fail\n", __LINE__));
    }

    memcpy(&g_DlcParameters, &DLC_MFinit, sizeof(StuDlc_FinetuneParamaters));
    for(i=0; i<16; i++)
    {
        g_uwPreTable[i] = (MS_U16)g_DlcParameters.ucLumaCurve[i] * (MS_U16)DLC_DEFLICK_PRECISION_SHIFT;
    }

    Hal_DLC_init();


#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    ST_KDRV_XC_DLC_INIT_INFO stDlcInitInfo;
    memset(&stDlcInitInfo, 0, sizeof(ST_KDRV_XC_DLC_INIT_INFO));

    stDlcInitInfo.bInitInfoChange = TRUE;

    for (i=0; i<16; ++i)
    {
        stDlcInitInfo.au8LumaCurve[i]     = g_DlcParameters.ucLumaCurve[i];
        stDlcInitInfo.au8LumaCurve2_a[i]  = g_DlcParameters.ucLumaCurve2_a[i];
        stDlcInitInfo.au8LumaCurve2_b[i]  = g_DlcParameters.ucLumaCurve2_b[i];
    }

    for (i=0; i<17; ++i)
    {
        stDlcInitInfo.au8DlcHistogramLimitCurve[i]  = g_DlcParameters.ucDlcHistogramLimitCurve[i];
    }

    stDlcInitInfo.u8DlcPureImageMode     = g_DlcParameters.ucDlcPureImageMode; // Compare difference of max and min bright
    stDlcInitInfo.u8DlcLevelLimit        = g_DlcParameters.ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    stDlcInitInfo.u8DlcAvgDelta          = g_DlcParameters.ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    stDlcInitInfo.u8DlcAvgDeltaStill     = g_DlcParameters.ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    stDlcInitInfo.u8DlcFastAlphaBlending = g_DlcParameters.ucDlcFastAlphaBlending; // min 17 ~ max 32
    stDlcInitInfo.u8DlcYAvgThresholdL    = g_DlcParameters.ucDlcYAvgThresholdL; // default value: 0
    stDlcInitInfo.u8DlcYAvgThresholdH    = g_DlcParameters.ucDlcYAvgThresholdH; // default value: 128
    stDlcInitInfo.u8DlcBLEPoint          = g_DlcParameters.ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    stDlcInitInfo.u8DlcWLEPoint          = g_DlcParameters.ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    stDlcInitInfo.bEnableBLE             = g_DlcParameters.bEnableBLE; // 1: enable; 0: disable
    stDlcInitInfo.bEnableWLE             = g_DlcParameters.bEnableWLE; // 1: enable; 0: disable
    stDlcInitInfo.u8DlcYAvgThresholdM    = g_DlcParameters.ucDlcYAvgThresholdM;
    stDlcInitInfo.u8DlcCurveMode         = g_DlcParameters.ucDlcCurveMode;
    stDlcInitInfo.u8DlcCurveModeMixAlpha = g_DlcParameters.ucDlcCurveModeMixAlpha;
    stDlcInitInfo.u8DlcAlgorithmMode     = g_DlcParameters.ucDlcAlgorithmMode;
    stDlcInitInfo.u8DlcSepPointH         = g_DlcParameters.ucDlcSepPointH;
    stDlcInitInfo.u8DlcSepPointL         = g_DlcParameters.ucDlcSepPointL;
    stDlcInitInfo.u16DlcBleStartPointTH   = g_DlcParameters.uwDlcBleStartPointTH;
    stDlcInitInfo.u16DlcBleEndPointTH     = g_DlcParameters.uwDlcBleEndPointTH;
    stDlcInitInfo.u8DlcCurveDiff_L_TH    = g_DlcParameters.ucDlcCurveDiff_L_TH;
    stDlcInitInfo.u8DlcCurveDiff_H_TH    = g_DlcParameters.ucDlcCurveDiff_H_TH;
    stDlcInitInfo.u16DlcBLESlopPoint_1    = g_DlcParameters.uwDlcBLESlopPoint_1;
    stDlcInitInfo.u16DlcBLESlopPoint_2    = g_DlcParameters.uwDlcBLESlopPoint_2;
    stDlcInitInfo.u16DlcBLESlopPoint_3    = g_DlcParameters.uwDlcBLESlopPoint_3;
    stDlcInitInfo.u16DlcBLESlopPoint_4    = g_DlcParameters.uwDlcBLESlopPoint_4;
    stDlcInitInfo.u16DlcBLESlopPoint_5    = g_DlcParameters.uwDlcBLESlopPoint_5;
    stDlcInitInfo.u16DlcDark_BLE_Slop_Min = g_DlcParameters.uwDlcDark_BLE_Slop_Min;
    stDlcInitInfo.u8DlcCurveDiffCoringTH = g_DlcParameters.ucDlcCurveDiffCoringTH;
    stDlcInitInfo.u8DlcAlphaBlendingMin  = g_DlcParameters.ucDlcAlphaBlendingMin;
    stDlcInitInfo.u8DlcAlphaBlendingMax  = g_DlcParameters.ucDlcAlphaBlendingMax;
    stDlcInitInfo.u8DlcFlicker_alpha     = g_DlcParameters.ucDlcFlicker_alpha;
    stDlcInitInfo.u8DlcYAVG_L_TH         = g_DlcParameters.ucDlcYAVG_L_TH;
    stDlcInitInfo.u8DlcYAVG_H_TH         = g_DlcParameters.ucDlcYAVG_H_TH;
    stDlcInitInfo.u8DlcDiffBase_L        = g_DlcParameters.ucDlcDiffBase_L;
    stDlcInitInfo.u8DlcDiffBase_M        = g_DlcParameters.ucDlcDiffBase_M;
    stDlcInitInfo.u8DlcDiffBase_H        = g_DlcParameters.ucDlcDiffBase_H;
    stDlcInitInfo.u8LMaxThreshold        = g_DlcParameters.ucLMaxThreshold;
    stDlcInitInfo.u8LMinThreshold        = g_DlcParameters.ucLMinThreshold;
    stDlcInitInfo.u8LMaxCorrection       = g_DlcParameters.ucLMaxCorrection;
    stDlcInitInfo.u8LMinCorrection       = g_DlcParameters.ucLMinCorrection;
    stDlcInitInfo.u8RMaxThreshold        = g_DlcParameters.ucRMaxThreshold;
    stDlcInitInfo.u8RMinThreshold        = g_DlcParameters.ucRMinThreshold;
    stDlcInitInfo.u8RMaxCorrection       = g_DlcParameters.ucRMaxCorrection;
    stDlcInitInfo.u8RMinCorrection       = g_DlcParameters.ucRMinCorrection;
    stDlcInitInfo.u8AllowLoseContrast    = g_DlcParameters.ucAllowLoseContrast;
#else
    MS_DLC_INIT_INFO MsDlcInitInfo;

    MsDlcInitInfo.bInitInfoChange = TRUE;

    for (i=0; i<16; ++i)
    {
        MsDlcInitInfo.ucLumaCurve[i]     = g_DlcParameters.ucLumaCurve[i];
        MsDlcInitInfo.ucLumaCurve2_a[i]  = g_DlcParameters.ucLumaCurve2_a[i];
        MsDlcInitInfo.ucLumaCurve2_b[i]  = g_DlcParameters.ucLumaCurve2_b[i];
    }

    for (i=0; i<17; ++i)
    {
        MsDlcInitInfo.ucDlcHistogramLimitCurve[i]  = g_DlcParameters.ucDlcHistogramLimitCurve[i];
    }

    MsDlcInitInfo.ucDlcPureImageMode     = g_DlcParameters.ucDlcPureImageMode; // Compare difference of max and min bright
    MsDlcInitInfo.ucDlcLevelLimit        = g_DlcParameters.ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MsDlcInitInfo.ucDlcAvgDelta          = g_DlcParameters.ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    MsDlcInitInfo.ucDlcAvgDeltaStill     = g_DlcParameters.ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    MsDlcInitInfo.ucDlcFastAlphaBlending = g_DlcParameters.ucDlcFastAlphaBlending; // min 17 ~ max 32
    MsDlcInitInfo.ucDlcYAvgThresholdL    = g_DlcParameters.ucDlcYAvgThresholdL; // default value: 0
    MsDlcInitInfo.ucDlcYAvgThresholdH    = g_DlcParameters.ucDlcYAvgThresholdH; // default value: 128
    MsDlcInitInfo.ucDlcBLEPoint          = g_DlcParameters.ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    MsDlcInitInfo.ucDlcWLEPoint          = g_DlcParameters.ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    MsDlcInitInfo.bEnableBLE             = g_DlcParameters.bEnableBLE; // 1: enable; 0: disable
    MsDlcInitInfo.bEnableWLE             = g_DlcParameters.bEnableWLE; // 1: enable; 0: disable
    MsDlcInitInfo.ucDlcYAvgThresholdM    = g_DlcParameters.ucDlcYAvgThresholdM;
    MsDlcInitInfo.ucDlcCurveMode         = g_DlcParameters.ucDlcCurveMode;
    MsDlcInitInfo.ucDlcCurveModeMixAlpha = g_DlcParameters.ucDlcCurveModeMixAlpha;
    MsDlcInitInfo.ucDlcAlgorithmMode     = g_DlcParameters.ucDlcAlgorithmMode;
    MsDlcInitInfo.ucDlcSepPointH         = g_DlcParameters.ucDlcSepPointH;
    MsDlcInitInfo.ucDlcSepPointL         = g_DlcParameters.ucDlcSepPointL;
    MsDlcInitInfo.uwDlcBleStartPointTH   = g_DlcParameters.uwDlcBleStartPointTH;
    MsDlcInitInfo.uwDlcBleEndPointTH     = g_DlcParameters.uwDlcBleEndPointTH;
    MsDlcInitInfo.ucDlcCurveDiff_L_TH    = g_DlcParameters.ucDlcCurveDiff_L_TH;
    MsDlcInitInfo.ucDlcCurveDiff_H_TH    = g_DlcParameters.ucDlcCurveDiff_H_TH;
    MsDlcInitInfo.uwDlcBLESlopPoint_1    = g_DlcParameters.uwDlcBLESlopPoint_1;
    MsDlcInitInfo.uwDlcBLESlopPoint_2    = g_DlcParameters.uwDlcBLESlopPoint_2;
    MsDlcInitInfo.uwDlcBLESlopPoint_3    = g_DlcParameters.uwDlcBLESlopPoint_3;
    MsDlcInitInfo.uwDlcBLESlopPoint_4    = g_DlcParameters.uwDlcBLESlopPoint_4;
    MsDlcInitInfo.uwDlcBLESlopPoint_5    = g_DlcParameters.uwDlcBLESlopPoint_5;
    MsDlcInitInfo.uwDlcDark_BLE_Slop_Min = g_DlcParameters.uwDlcDark_BLE_Slop_Min;
    MsDlcInitInfo.ucDlcCurveDiffCoringTH = g_DlcParameters.ucDlcCurveDiffCoringTH;
    MsDlcInitInfo.ucDlcAlphaBlendingMin  = g_DlcParameters.ucDlcAlphaBlendingMin;
    MsDlcInitInfo.ucDlcAlphaBlendingMax  = g_DlcParameters.ucDlcAlphaBlendingMax;
    MsDlcInitInfo.ucDlcFlicker_alpha     = g_DlcParameters.ucDlcFlicker_alpha;
    MsDlcInitInfo.ucDlcYAVG_L_TH         = g_DlcParameters.ucDlcYAVG_L_TH;
    MsDlcInitInfo.ucDlcYAVG_H_TH         = g_DlcParameters.ucDlcYAVG_H_TH;
    MsDlcInitInfo.ucDlcDiffBase_L        = g_DlcParameters.ucDlcDiffBase_L;
    MsDlcInitInfo.ucDlcDiffBase_M        = g_DlcParameters.ucDlcDiffBase_M;
    MsDlcInitInfo.ucDlcDiffBase_H        = g_DlcParameters.ucDlcDiffBase_H;
#endif

#ifdef MSOS_TYPE_LINUX
    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("DLC device not opened!!!!\n");
            return;
        }
    }
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCSETDLCINITINFO, &stDlcInitInfo))
#else
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_SETDLCINITINFO, &MsDlcInitInfo))
#endif
    {
        DLC_PRINT("MDrv_DLC_SetDlcInitInfo fail!!!!\n");
        return;
    }
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    KDrv_DLC_SetInitInfo(stDlcInitInfo);
#else
    //_MDrv_GFLIPIO_IOC_DlcInitInfo is obsolete.
#endif

#endif

}

void MDrv_DLC_Init_Ext(DLC_init_Ext *pDLC_InitExt)
{
#if(ENABLE_AVERAGELUMA)

    MS_U8 i;

    memcpy(&gstDLCParamExt, pDLC_InitExt, sizeof(DLC_init_Ext));

    for(i=0; i<16; i++)
    {
        g_u32PreTableExt[i] = (MS_U32)gstDLCParamExt.uwLumaCurve[i] * (MS_U32)DLC_DEFLICK_PRECISION_SHIFT;
    }
#endif
}


MS_BOOL MDrv_DLC_GetDLCInfo(StuDlc_FinetuneParamaters *DLC_MFinit, MS_U32 u32InitDataLen)
{
    if(u32InitDataLen != sizeof(StuDlc_FinetuneParamaters))
    {
        return FALSE;
    }
    memcpy(DLC_MFinit, &g_DlcParameters, u32InitDataLen);
    return TRUE;
}

MS_BOOL MDrv_DLC_GetLumaInfo(DLC_LUMADATA *pDLCLumaData, MS_U32 u32LumaDataLen)
{
    MS_U8 i;
    if(u32LumaDataLen != sizeof(DLC_LUMADATA))
    {
        return FALSE;
    }
    for(i=0; i<32; i++)
    {
        pDLCLumaData->g_wLumaHistogram32H[i] = g_wLumaHistogram32H[i];
    }
    for(i=0; i<16; i++)
    {
        pDLCLumaData->g_ucTable[i]    = g_ucTable[i];
    }
    pDLCLumaData->g_wLumiTotalCount   = g_wLumiTotalCount;
    pDLCLumaData->g_wLumiAverageTemp  = g_wLumiAverageTemp;
    pDLCLumaData->g_ucHistogramMax    = g_ucHistogramMax;
    pDLCLumaData->g_ucHistogramMin    = g_ucHistogramMin;
    return TRUE;
}

void MDrv_DLC_init_riu_base( MS_VIRT ptr_riu_base )
{
    Hal_DLC_init_riu_base(ptr_riu_base);
}

MS_BOOL MDrv_DLC_GetLumaCurveStatus( void )
{
    MS_BOOL bret;

    bret = Hal_DLC_GetLumaCurveStatus();

    return bret;
}

////////////////////////////////////////////////////////////////////////////////
//
//  DrvDLC.c end
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  DBC library function start
//
////////////////////////////////////////////////////////////////////////////////

extern MS_U8 g_ucDBC_DebugMode;

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/
extern  StuDbc_FinetuneParamaters_Lib g_DbcParameters;

MS_U8 MDrv_DLC_DBC_GetDebugMode(void)
{
    return g_ucDBC_DebugMode;
}

void MDrv_DLC_DBC_SetDebugMode(MS_U8 u8DBC_DebugMode)
{
    g_ucDBC_DebugMode = u8DBC_DebugMode;
}

void MDrv_DLC_DBC_UpdatePWM(MS_U8 u8PWMvalue)
{
    g_DbcParameters.ucCurrentPWM = u8PWMvalue;
}

void MDrv_DLC_DBC_Init(StuDbc_FinetuneParamaters_Lib DLC_DBC_MFinit)
{
    memcpy(&g_DbcParameters, &DLC_DBC_MFinit, sizeof(StuDbc_FinetuneParamaters_Lib));
}

MS_U16 MDrv_DLC_DBC_Handler(void)
{
    return msBackLightHandler();
}

MS_U16 MDrv_DLC_DBC_GetMFVer(void)
{
    return msDbcLibVersionCheck();
}

MS_BOOL MDrv_DLC_GetDBCInfo(StuDbc_FinetuneParamaters_Lib *pDLCDBCData, MS_U32 u32DBCDataLen)
{

    if(u32DBCDataLen != sizeof(StuDbc_FinetuneParamaters_Lib))
    {
        return FALSE;
    }

    memcpy(pDLCDBCData, &g_DbcParameters, sizeof(StuDbc_FinetuneParamaters_Lib));
    return TRUE;
}

void MDrv_DLC_DBC_AdjustYCGain(void)
{
    msAdjustYCGain(g_DbcParameters.ucY_Gain_H, g_DbcParameters.ucC_Gain_H);
}

void MDrv_DLC_DBC_YCGainInit(MS_U8 u8YGain_M, MS_U8 u8YGain_L, MS_U8 u8CGain_M, MS_U8 u8CGain_L, MS_U8 u8YGain_H, MS_U8 u8CGain_H)
{

    g_DbcParameters.ucY_Gain_H = u8YGain_H;
    g_DbcParameters.ucC_Gain_H = u8CGain_H;
    if((u8YGain_M == 0) && (u8YGain_L == 0) && (u8CGain_M == 0) && (u8CGain_L == 0))
    {
        g_DbcParameters.ucY_Gain_M = g_DbcParameters.ucY_Gain_H;
        g_DbcParameters.ucY_Gain_L = g_DbcParameters.ucY_Gain_H;
        g_DbcParameters.ucC_Gain_M = g_DbcParameters.ucC_Gain_H;
        g_DbcParameters.ucC_Gain_L = g_DbcParameters.ucC_Gain_H;
    }
    else
    {
        g_DbcParameters.ucY_Gain_M = u8YGain_M;
        g_DbcParameters.ucY_Gain_L = u8YGain_L;
        g_DbcParameters.ucC_Gain_M = u8CGain_M;
        g_DbcParameters.ucC_Gain_L = u8CGain_L;
    }

    msDBCInit();
}

void MDrv_DLC_WriteCurve(MS_U8 *pu8Table)
{
#if(ENABLE_AVERAGELUMA)
    Hal_DLC_set_curve(pu8Table);
#endif
}

void MDrv_DLC_WriteCurve_Sub(MS_U8 *pu8Table)
{
#if(ENABLE_AVERAGELUMA)
    Hal_DLC_set_curve_sub(pu8Table);
#endif
}


MS_BOOL MDrv_DLC_get_histogram32(MS_U16 *pu16Histogram32)
{
#if(ENABLE_AVERAGELUMA)

    MS_BOOL bret = FALSE;
    if(Hal_DLC_get_histogram_ack())
    {
        Hal_DLC_get_histogram32(pu16Histogram32);

        Hal_DLC_set_histogram_req();
        bret = TRUE;
    }
    return bret;
#else
    return FALSE;
#endif
}

void MDrv_DLC_set_CaptureRange(StuDbc_CAPTURE_Range *pu16Cap_rang)
{
//    if(pu16Cap_rang!=NULL)
    memcpy(&g_DlcCapRange, pu16Cap_rang, sizeof(StuDbc_CAPTURE_Range));
/*
    else
        memcpy(pu16Cap_rang, &g_DlcCapRange, sizeof(StuDbc_CAPTURE_Range));
*/
    msDlc_SetCaptureRange(pu16Cap_rang->wHStart, pu16Cap_rang->wHEnd, pu16Cap_rang->wVStart, pu16Cap_rang->wVEnd);

}

//-------------------------------------------------------------------------------------------------
///Send Dlc Init Curve Change Info to Dlc kernel
/// @param  bCurveChange       \b IN: the dlc static curve change
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set dlc info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DLC_SendDlcInitCurveChangeInfo(MS_BOOL bCurveChange,MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark)
{

#ifdef MSOS_TYPE_LINUX

    MS_U8 i;
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    ST_KDRV_XC_DLC_INFO stDlcInfo;

    stDlcInfo.bCurveChange = bCurveChange;

    for (i=0; i<16; ++i)
    {
        stDlcInfo.u8LumaCurve[i]     = pNormal[i];
        stDlcInfo.u8LumaCurve2_a[i]  = pLight[i];
        stDlcInfo.u8LumaCurve2_b[i]  = pDark[i];
    }
#else
    MS_DLC_INFO MsDlcInfo;

    MsDlcInfo.bCurveChange = bCurveChange;

    for (i=0; i<16; ++i)
    {
        MsDlcInfo.u8LumaCurve[i]     = pNormal[i];
        MsDlcInfo.u8LumaCurve2_a[i]  = pLight[i];
        MsDlcInfo.u8LumaCurve2_b[i]  = pDark[i];
    }
#endif
    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("DLC device not opened!!!!\n");
            return FALSE;
        }
    }

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCCHANGECURVE, &stDlcInfo))
#else
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_DLCCHANGECURVE, &MsDlcInfo))
#endif
    {
        DLC_PRINT("MDrv_DLC_SetDlcInfo fail!!!!\n");
        return FALSE;
    }

#endif
return TRUE;
}

//-------------------------------------------------------------------------------------------------
///Send BLE Slop Point Change Info to Dlc kernel
/// @param  pBLESlopPoint       \b IN: the BLE Slop Point Change
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set ble info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DLC_SendBleChangeInfo(MS_BOOL bBLEPointChange,MS_U16 *pBLESlopPoint)
{

#ifdef MSOS_TYPE_LINUX

    MS_U8 i;
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    ST_KDRV_XC_DLC_BLE_INFO stBleInfo;

    stBleInfo.bBLEPointChange = bBLEPointChange;

    for (i=0; i<6; ++i)
    {
        stBleInfo.u16BLESlopPoint[i]     = pBLESlopPoint[i];
    }
#else
    MS_BLE_INFO MsBleInfo;

    MsBleInfo.bBLEPointChange = bBLEPointChange;

    for (i=0; i<6; ++i)
    {
        MsBleInfo.u16BLESlopPoint[i]     = pBLESlopPoint[i];
    }
#endif
    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("BLE device not opened!!!!\n");
            return FALSE;
        }
    }

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCBLECHANGECURVE, &stBleInfo))
#else
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_BLECHANGEPOINT, &MsBleInfo))
#endif
    {
        DLC_PRINT("MDrv_DLC_SendBleChangeInfo fail!!!!\n");
        return FALSE;
    }

#endif
return TRUE;
}

//-------------------------------------------------------------------------------------------------
///Send HDR Init Info to Dlc kernel
/// @param  pstDLC_HDRInitData       \b IN: the HDR Init Data
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set dlc info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DLC_SetHDRInit(DLC_HDRinit * pstDLC_HDRInitData)
{

#ifdef MSOS_TYPE_LINUX

    if(0 > s32FdDlcBle)
    {
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
        s32FdDlcBle = open("/dev/scaler", O_RDWR);
#else
        s32FdDlcBle = open("/dev/gflip", O_RDWR);
#endif

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("BLE device not opened!!!!\n");
            return FALSE;
        }
    }
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
#else
    MS_HDR_INFO_EXTEND MsHDRInfo;
    memset(&MsHDRInfo, 0 , sizeof(MS_HDR_INFO_EXTEND));
    memcpy(&MsHDRInfo, pstDLC_HDRInitData, sizeof(MS_HDR_INFO_EXTEND));
    MsHDRInfo.u16HDRVerInfo = MS_HDR_VERSION;
    MsHDRInfo.u16HDRInitLength = sizeof(MS_HDR_INFO_EXTEND);
    MsHDRInfo.DLC_HDRCustomerDlcCurve.pucDlcCurve = NULL;
#if !defined (__aarch64__)
    MsHDRInfo.DLC_HDRCustomerDlcCurve.pDummy = NULL;
#endif

    MS_U16 u16BufSize = sizeof(MS_HDR_INFO_EXTEND) + sizeof(MS_U8)*pstDLC_HDRInitData->DLC_HDRCustomerDlcCurve.u16DlcCurveSize;
    MS_U8 *pu8Buf = (MS_U8*)malloc(u16BufSize);
    MS_U16 u16Offset = 0;

    memset(pu8Buf, 0, u16BufSize);
    memcpy(pu8Buf, &MsHDRInfo, sizeof(MS_HDR_INFO_EXTEND));
    u16Offset += sizeof(MS_HDR_INFO_EXTEND);

    if (pstDLC_HDRInitData->DLC_HDRCustomerDlcCurve.u16DlcCurveSize > 0)
    {
        memcpy(pu8Buf + u16Offset, pstDLC_HDRInitData->DLC_HDRCustomerDlcCurve.pucDlcCurve, sizeof(MS_U8)*pstDLC_HDRInitData->DLC_HDRCustomerDlcCurve.u16DlcCurveSize);
        u16Offset += (sizeof(MS_U8)*pstDLC_HDRInitData->DLC_HDRCustomerDlcCurve.u16DlcCurveSize);
    }
#endif

#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
    printf("not support MDrv_DLC_SetHDRInit.\n");
#else
    // Send MS_HDR_INFO_EXTEND
    if (ioctl(s32FdDlcBle, MDRV_GFLIP_IOC_DLCSETHDRINFO, pu8Buf))
    {
        DLC_PRINT("MDrv_DLC_SetHDR_Init send MS_HDR_INFO_EXTEND fail!!!!\n");
        free(pu8Buf);
        return FALSE;
    }

    free(pu8Buf);
#endif
#endif
return TRUE;
}
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#ifdef MSOS_TYPE_LINUX_KERNEL
extern MS_BOOL KDrv_DLC_SetTMOInfo(ST_KDRV_XC_TMO_INFO *pstTMOInfo);
#endif
MS_BOOL MDrv_DLC_SetTMOInfo(ST_DLC_TMO_INFO * pstDLCTMOInfo)
{
    ST_KDRV_XC_TMO_INFO *pstKdrvTmoInfo = (ST_KDRV_XC_TMO_INFO*)malloc(sizeof(ST_DLC_TMO_INFO));
    pstKdrvTmoInfo->u16SrcMinRatio = pstDLCTMOInfo->u16SrcMinRatio;
    pstKdrvTmoInfo->u16SrcMedRatio = pstDLCTMOInfo->u16SrcMedRatio;
    pstKdrvTmoInfo->u16SrcMaxRatio = pstDLCTMOInfo->u16SrcMaxRatio;
    pstKdrvTmoInfo->u16TgtMin = pstDLCTMOInfo->u16TgtMin;
    pstKdrvTmoInfo->u16TgtMed = pstDLCTMOInfo->u16TgtMed;
    pstKdrvTmoInfo->u16TgtMax = pstDLCTMOInfo->u16TgtMax;
    pstKdrvTmoInfo->u16FrontSlopeMin = pstDLCTMOInfo->u16FrontSlopeMin;
    pstKdrvTmoInfo->u16FrontSlopeMax = pstDLCTMOInfo->u16FrontSlopeMax;
    pstKdrvTmoInfo->u16BackSlopeMin = pstDLCTMOInfo->u16BackSlopeMin;
    pstKdrvTmoInfo->u16BackSlopeMax = pstDLCTMOInfo->u16BackSlopeMax;
    pstKdrvTmoInfo->u16SceneChangeThrd = pstDLCTMOInfo->u16SceneChangeThrd;
    pstKdrvTmoInfo->u16SceneChangeRatioMax = pstDLCTMOInfo->u16SceneChangeRatioMax;
    pstKdrvTmoInfo->u8IIRRatio = pstDLCTMOInfo->u8IIRRatio;
    pstKdrvTmoInfo->u8TMOTargetMode = pstDLCTMOInfo->u8TMOTargetMode;
    pstKdrvTmoInfo->u16SDRPanelGain = pstDLCTMOInfo->u16SDRPanelGain;
    pstKdrvTmoInfo->u16Smin = pstDLCTMOInfo->u16Smin;
    pstKdrvTmoInfo->u16Smed = pstDLCTMOInfo->u16Smed;
    pstKdrvTmoInfo->u16Smax = pstDLCTMOInfo->u16Smax;
    pstKdrvTmoInfo->u16Tmin = pstDLCTMOInfo->u16Tmin;
    pstKdrvTmoInfo->u16Tmed = pstDLCTMOInfo->u16Tmed;
    pstKdrvTmoInfo->u16Tmax = pstDLCTMOInfo->u16Tmax;
    pstKdrvTmoInfo->bRefMode = pstDLCTMOInfo->bRefMode;
    pstKdrvTmoInfo->u8TMOUseIniControls = pstDLCTMOInfo->u8TMOUseIniControls;
   
#ifdef MSOS_TYPE_LINUX
    if(0 > s32FdDlcBle)
    {
        s32FdDlcBle = open("/dev/scaler", O_RDWR);

        if(0 > s32FdDlcBle)
        {
            DLC_PRINT("BLE device not opened!!!!\n");
            return FALSE;
        }
    }

    if (ioctl(s32FdDlcBle, MDRV_XC_IOC_DLCSETTMOINFO, pstKdrvTmoInfo))
    {
        DLC_PRINT("MDrv_DLC_SetTMOInfo send MDRV_XC_IOC_DLCSETTMOINFO fail!!!!\n");
        return FALSE;
    }
#elif defined(MSOS_TYPE_LINUX_KERNEL)
    KDrv_DLC_SetTMOInfo(pstKdrvTmoInfo);
#endif
    free(pstKdrvTmoInfo);
    return TRUE;

}
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
//
//  DBC library function End
//
////////////////////////////////////////////////////////////////////////////////

#undef  _MDRV_DLC_C_

