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
/// file    drv_ACE.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#define  _MDRV_ACE_C_
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "apiXC_Ace.h"
#include "drvACE.h"
#include "mhal_ace.h"
#include "hwreg_ace.h"
#include "ace_hwreg_utility2.h"
#include "MsAce_LIB.h"

////////////////////////////////////////////////////////////////////////////////
//
//  ACE library function start
//
////////////////////////////////////////////////////////////////////////////////

MS_U8 ACE_VBuf[32] =  {
    'M','S','V','C','0','0',                //0,0 fixed
    'A','4',                                //LIB ID
    '0','0',                                //Interface
    '0','1','0','4',                        //Build number
    '0','0','0','8','9','9','8','3',        // Change List
    'A','E','0','0','0','0','0','0','0',    // AE:Saturn4LE  0: MStar Trunk
    'T'                                     // check sum
};

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MAIN_WINDOW     0
#define SUB_WINDOW      1

// for Utopia 2.0 to Utopia 1.0 compatibility.
#include "ACE_private.h"
#include "utopia_dapi.h"
#include "utopia.h"

extern void* pu32ACEInst;
extern void* pu32ACERes[E_ACE_POOL_ID_MAX];
extern ACE_DRV_HDRinit g_ACEinitParameters;
#if 0
typedef struct
{
    MS_U16 u16RedGain;
    MS_U16 u16GreenGain;
    MS_U16 u16BlueGain;
} ACE_PostRGBGainSetting;

static ACE_PostRGBGainSetting stACEPostGainSetting[XC_ACE_EX_MAX_DEVICE_NUM][2];
#else
#define stACEPostGainSetting psACEResPri->stdrvACE.stACEPostGainSetting
#define MWEPanel psACEResPri->stdrvACE.MWEPanel
#endif
/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetVersion()
/// @brief \b Function \b Description:  Get version of ACE library
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b Version
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_GetVersion(void)
{
    return msACE_GetVersion();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: void MDrv_ACE_Init(  MS_BOOL bScalerWin, MS_S16* psColorCorrectionTable, MS_S16* psPCsRGBTable )
/// @brief \b Function \b Description:  Initialize ACE varible
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////

void MDrv_ACE_Init(void* pInstance, MS_BOOL bScalerWin, MS_S16* psColorCorrectionTable, MS_S16* psPCsRGBTable )
{
    _ACE_GET_VARIABLE();
    memset(stACEPostGainSetting, 0, sizeof(stACEPostGainSetting));

    if(psACEResPri->stapiACE.g_bACEModuleInited[bScalerWin] == FALSE)
    {
        InitACEVar(pInstance);
        msACEPCForceYUVtoRGB(pInstance, bScalerWin, TRUE);
    }
    msACESetColorCorrectionTable(pInstance, bScalerWin, psColorCorrectionTable);
    msACESetPCsRGBTable(pInstance, bScalerWin, psPCsRGBTable);
    Hal_ACE_Set_Device_Bank_Offset();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PCForceYUVtoRGB()
/// @brief \b Function \b Description:  Force transfer YUV to RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b bEnable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PCForceYUVtoRGB(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable )
{
    msACEPCForceYUVtoRGB(pInstance, bScalerWin, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetContrast()
/// @brief \b Function \b Description:  Adjust Contrast
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Contrast
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetContrast(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast )
{
    msAdjustVideoContrast(pInstance, bScalerWin, u8Contrast);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetRGB()
/// @brief \b Function \b Description:  Adjust RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Red
/// @param <IN>        \b u8Green
/// @param <IN>        \b u8Blue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetRGB(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U8 u8Red, MS_U8 u8Green, MS_U8 u8Blue )
{
    if(bUseYUVSpace)
    {
        msAdjustVideoRGB(pInstance, bScalerWin, u8Red, u8Green, u8Blue);
    }
    else
    {
        msAdjustPCRGB(pInstance, bScalerWin, u8Red, u8Green, u8Blue);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetRGB_10Bits()
/// @brief \b Function \b Description:  Adjust RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u16Red
/// @param <IN>        \b u16Green
/// @param <IN>        \b u16Blue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetRGB_10Bits( void* pInstance, MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U16 u16Red, MS_U16 u16Green, MS_U16 u16Blue )
{
    if(bUseYUVSpace)
    {
        msAdjustVideoRGB_10Bits(pInstance, bScalerWin, u16Red, u16Green, u16Blue);
    }
    else
    {
        msAdjustPCRGB_10Bits(pInstance, bScalerWin, u16Red, u16Red, u16Red);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBGainOffset()
/// @brief \b Function \b Description:  Adjust Post RGB Gain and Offset simultaneously
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8RedGain
/// @param <IN>        \b u8GreenGain
/// @param <IN>        \b u8BlueGain
/// @param <IN>        \b u16RedOffset
/// @param <IN>        \b u16GreenOffset
/// @param <IN>        \b u16BlueOffset
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBGainOffset(void* pInstance, MS_BOOL bScalerWin,
                                      MS_U16 u8RedGain,    MS_U16 u8GreenGain,    MS_U16 u8BlueGain,
                                      MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset, MS_U8 u8Step)
{
    MS_U8 u8Idx;

    u8Idx = (bScalerWin == TRUE) ? 1 : 0;
    _ACE_GET_VARIABLE();

    stACEPostGainSetting[u8Idx].u16RedGain = u8RedGain ;
    stACEPostGainSetting[u8Idx].u16GreenGain = u8GreenGain ;
    stACEPostGainSetting[u8Idx].u16BlueGain = u8BlueGain ;

    Hal_ACE_SetPostRGBGainOffsetTogether(pInstance, bScalerWin,
                                         stACEPostGainSetting[u8Idx].u16RedGain,
                                         stACEPostGainSetting[u8Idx].u16GreenGain,
                                         stACEPostGainSetting[u8Idx].u16BlueGain,
                                         u16RedOffset   << u8Step,
                                         u16GreenOffset << u8Step,
                                         u16BlueOffset  << u8Step);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBGain()
/// @brief \b Function \b Description:  Adjust Post RGB Gain
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8RedGain
/// @param <IN>        \b u8GreenGain
/// @param <IN>        \b u8BlueGain
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBGain(void* pInstance, MS_BOOL bScalerWin, MS_U16 u8RedGain, MS_U16 u8GreenGain, MS_U16 u8BlueGain)
{
    MS_U8 u8Idx;

    u8Idx = (bScalerWin == TRUE) ? 1 : 0;
    _ACE_GET_VARIABLE();

    stACEPostGainSetting[u8Idx].u16RedGain = u8RedGain ;
    stACEPostGainSetting[u8Idx].u16GreenGain = u8GreenGain ;
    stACEPostGainSetting[u8Idx].u16BlueGain = u8BlueGain ;

    Hal_ACE_SetPostRGBGain(pInstance, bScalerWin,
        stACEPostGainSetting[u8Idx].u16RedGain,
        stACEPostGainSetting[u8Idx].u16GreenGain,
        stACEPostGainSetting[u8Idx].u16BlueGain);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicGetPostRGBGain()
/// @brief \b Function \b Description:  Get ACE Post RGB Gain Info
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucInfoType
/// @param <OUT>       \b None
/// @param <RET>       \b wReturnValue
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_PicGetPostRGBGain(void* pInstance, MS_BOOL bWindow, MS_U16 u16ACEinfo)
{
    EnuACEInfoType eACEInfoType   = (EnuACEInfoType) u16ACEinfo;
    MS_U16         u16ReturnValue = 0;
    MS_U8           u8Idx;
    _ACE_GET_VARIABLE();

    u8Idx = (bWindow == TRUE) ? 1 : 0;

    switch( eACEInfoType )
    {
        case ACE_INFO_TYPE_R_GAIN:
            u16ReturnValue = stACEPostGainSetting[u8Idx].u16RedGain;
            break;

        case ACE_INFO_TYPE_G_GAIN:
	        u16ReturnValue = stACEPostGainSetting[u8Idx].u16GreenGain;
	        break;

        case ACE_INFO_TYPE_B_GAIN:
            u16ReturnValue = stACEPostGainSetting[u8Idx].u16BlueGain;
            break;

        default:
        {
            MS_DEBUG_MSG(printf("Get Post RGB Gain fails\n"));
            u16ReturnValue = 0;
            break;
        }
    }
    return u16ReturnValue;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBOffset()
/// @brief \b Function \b Description:  Adjust Post RGB Offset
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u16RedOffset
/// @param <IN>        \b u16GreenOffset
/// @param <IN>        \b u16BlueOffset
/// @param <IN>        \b u8Step
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBOffset(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset, MS_U8 u8Step)
{
    Hal_ACE_SetPostRGBOffset(pInstance, bScalerWin, u16RedOffset << u8Step, u16GreenOffset << u8Step, u16BlueOffset << u8Step);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicGetPostRGBGain()
/// @brief \b Function \b Description:  Get ACE Post RGB Gain Info
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucInfoType
/// @param <OUT>       \b None
/// @param <RET>       \b wReturnValue
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_PicGetPostRGBOffset(void* pInstance, MS_BOOL bWindow, MS_U16 u16ACEinfo)
{
    EnuACEInfoType eACEInfoType   = (EnuACEInfoType) u16ACEinfo;
    MS_U16         u16ReturnValue = 0;
    _ACE_GET_INST_PRIVATE();

    switch( eACEInfoType )
    {
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R:
	 	{
             if(MAIN_WINDOW == bWindow)
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_21_L, 0x07FF);
			}
			else
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_27_L, 0x07FF);
			}
			break;
		}
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G:
		{
			if(MAIN_WINDOW == bWindow)
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_22_L, 0x07FF);
			}
			else
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_28_L, 0x07FF);
			}
			break;
        }
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B:
		{
			if(MAIN_WINDOW == bWindow)
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_23_L, 0x07FF);
			}
			else
			{
				u16ReturnValue = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_29_L, 0x07FF);
			}
			break;
        }
        default:
        {
            MS_DEBUG_MSG(printf("Get Post RGB Offset fails\n"));
            u16ReturnValue = 0;
            break;
        }
    }
    return u16ReturnValue;
}

MS_U8 MDrv_ACE_GetPostRGBGain(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16ACEinfo)
{
    EnuACEInfoType eACEInfoType;
    eACEInfoType = (EnuACEInfoType) u16ACEinfo;
    MS_U8 ret = 0;
    _ACE_GET_INST_PRIVATE();

    if(MAIN_WINDOW == bScalerWin)
    {
        switch(eACEInfoType)
        {
            case ACE_INFO_TYPE_R_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_R_CHANNEL_GAIN_MAIN, POST_R_CHANNEL_GAIN_MASK)>> 3);
                break;
            case ACE_INFO_TYPE_G_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_G_CHANNEL_GAIN_MAIN, POST_G_CHANNEL_GAIN_MASK)>> 3);
                break;
            case ACE_INFO_TYPE_B_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_B_CHANNEL_GAIN_MAIN, POST_B_CHANNEL_GAIN_MASK)>> 3);
                break;
            default:
                ret = 0;
                break;
        }
    }
    else
#if (MAX_WINDOW_NUM > 1)
    {
        switch(eACEInfoType)
        {
            case ACE_INFO_TYPE_R_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_R_CHANNEL_GAIN_SUB, POST_R_CHANNEL_GAIN_MASK)>>3);
                break;
            case ACE_INFO_TYPE_G_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_G_CHANNEL_GAIN_SUB, POST_G_CHANNEL_GAIN_MASK)>>3);
                break;
            case ACE_INFO_TYPE_B_GAIN:
                ret = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, POST_B_CHANNEL_GAIN_SUB, POST_B_CHANNEL_GAIN_MASK)>>3);
                break;
            default:
                ret = 0;
                break;
        }
    }
#else
    {
        ret = 0;
    }
#endif
    MS_DEBUG_MSG(printf("[MDrv_ACE_GetPostRGBGain] ret = %d\n", ret));
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SelectYUVtoRGBMatrix()
/// @brief \b Function \b Description:  Select YUV to RGB Matrix
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucMatrix
/// @param <IN>        \b psUserYUVtoRGBMatrix
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SelectYUVtoRGBMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix )
{
    msACESelectYUVtoRGBMatrix(pInstance, bScalerWin, ucMatrix, psUserYUVtoRGBMatrix);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetColorCorrectionTable()
/// @brief \b Function \b Description:  Set Color Correction Table
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b psColorCorrectionTable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetColorCorrectionTable(void* pInstance, MS_BOOL bScalerWin)
{
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetPCsRGBTable()
/// @brief \b Function \b Description:  Set PCs RGB Table
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b psPCsRGBTable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetPCsRGBTable(void* pInstance, MS_BOOL bScalerWin)
{
    msSetPCColorMatrix(pInstance, bScalerWin);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetSaturation()
/// @brief \b Function \b Description:  Adjust Video Saturation
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Saturation
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetSaturation(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Saturation )
{
    msAdjustVideoSaturation(pInstance, bScalerWin, u8Saturation);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetHue()
/// @brief \b Function \b Description:  Adjust Video Hue
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Hue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetHue(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Hue )
{
    msAdjustVideoHue(pInstance, bScalerWin, u8Hue);
}

MS_BOOL MDrv_ACE_IsPostRGBGain(void* pInstance, MS_BOOL bWindow)
{
    MS_BOOL bRet;
    _ACE_GET_INST_PRIVATE();

    if (MAIN_WINDOW == bWindow)
    {
        bRet = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_01_L, BIT(5)) ? TRUE: FALSE;
    }
    else
    {
        bRet = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_11_L, BIT(5)) ? TRUE: FALSE;
    }
    return bRet;
}

MS_BOOL MDrv_ACE_IsPostRGBOffset(void* pInstance, MS_BOOL bWindow)
{
    MS_BOOL bRet = FALSE;
    _ACE_GET_INST_PRIVATE();

    if (MAIN_WINDOW == bWindow)
    {
        bRet = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_01_L, BIT(4)) ? TRUE: FALSE;
    }
    else
    {
        bRet = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_11_L, BIT(4)) ? TRUE: FALSE;
    }
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetInfo()
/// @brief \b Function \b Description:  Get ACE Info
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucInfoType
/// @param <OUT>       \b None
/// @param <RET>       \b wReturnValue
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_GetACEInfo(void* pInstance, MS_BOOL bWindow, MS_U16 u16ACEinfo )
{
    EnuACEInfoType eACEInfoType;
    eACEInfoType = (EnuACEInfoType) u16ACEinfo;
    _ACE_GET_INST_PRIVATE();

    if(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK25_01_L, BIT(5)) && ((eACEInfoType == ACE_INFO_TYPE_R_GAIN) || (eACEInfoType == ACE_INFO_TYPE_G_GAIN) || (eACEInfoType == ACE_INFO_TYPE_B_GAIN)))
    {
        return MDrv_ACE_PicGetPostRGBGain(pInstance, bWindow, u16ACEinfo);
    }
    else if ((eACEInfoType == ACE_INFO_TYPE_BRIGHTNESS_R) || (eACEInfoType == ACE_INFO_TYPE_BRIGHTNESS_G) || (eACEInfoType == ACE_INFO_TYPE_BRIGHTNESS_B))
    {
        return MDrv_ACE_GetBrightness(pInstance, bWindow, eACEInfoType);
    }
    else
        return msACE_GetInfo(pInstance, bWindow, eACEInfoType);
}

////////////////////////////////////////////////////////////////////////////////
//
//  ACE library function end
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  DrvACE.c start
//
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_DMS(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bisATV )
{
    Hal_ACE_DMS(pInstance, bScalerWin, bisATV);
}

void MDrv_ACE_GetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix )
{
    Hal_ACE_GetColorMatrix(pInstance, bScalerWin, pu16Matrix);
}

void MDrv_ACE_SetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix )
{
    Hal_ACE_SetColorMatrix(pInstance, bScalerWin, pu16Matrix);
}

MS_BOOL MDrv_ACE_SetBypassColorMatrix(void* pInstance, MS_BOOL bEnable )
{
    msACESetBypassColorMatrix(pInstance, bEnable);
    return TRUE;
}

void MDrv_ACE_PatchDTGColorChecker(void* pInstance, MS_U8 u8Mode)
{
    Hal_ACE_PatchDTGColorChecker(pInstance, u8Mode);
}


void MDrv_ACE_SetFleshTone(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEn, MS_U8 u8FleshTone)
{
    Hal_ACE_SetFCC_En(pInstance, bScalerWin, 0, bEn);
    Hal_ACE_SetFCC_Cb(pInstance, 0, u8FleshTone);
}

void MDrv_ACE_SetBlackAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8BlackAdjust)
{
    Hal_ACE_SetSlopValue(pInstance, bScalerWin, u8BlackAdjust);
}

void MDrv_ACE_init_riu_base(MS_VIRT ptr_riu_base )
{
    Hal_ACE_init_riu_base(ptr_riu_base);
}

void MDrv_ACE_SetSharpness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Sharpness )
{
    Hal_ACE_SetSharpness(pInstance, bScalerWin, u8Sharpness);
}

/******************************************************************************/
///Set brightness
///@param u8Brightness \b IN
///- brightness value from 0 -0xff
/******************************************************************************/
void MDrv_ACE_SetBrightness (void* pInstance, MS_BOOL bScalerWin, MS_U8 u8RedBrightness, MS_U8 u8GreenBrightness, MS_U8 u8BlueBrightness )
{
    Hal_ACE_SetBrightness(pInstance, bScalerWin, u8RedBrightness, u8GreenBrightness, u8BlueBrightness);
}

/******************************************************************************/
///Set brightness
///@param u16Brightness \b IN
///- brightness value from 0-0x7ff
/******************************************************************************/
void MDrv_ACE_SetBrightnessPrecise(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedBrightness, MS_U16 u16GreenBrightness, MS_U16 u16BlueBrightness)
{
    Hal_ACE_SetBrightnessPrecise(pInstance, bScalerWin, u16RedBrightness, u16GreenBrightness, u16BlueBrightness);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetBrightness()
/// @brief \b Function \b Description:  Get ACE brightness
/// @param <IN>        \b pInstance  pInstance
/// @param <IN>        \b bScalerWin  bScalerWin
/// @param <IN>        \b eACEInfoType
/// @param <OUT>      \b None
/// @param <RET>       \b u16Val Brightness value from 0-0xff
/// @param <GLOBAL>  \b None
////////////////////////////////////////////////////////////////////////////////

MS_U16 MDrv_ACE_GetBrightness(void* pInstance, MS_BOOL bScalerWin, EnuACEInfoType eACEInfoType)
{
    MS_U16 u16Val = 0;
    MS_U8 u8RedBrightness = 0;
    MS_U8 u8GreenBrightness = 0;
    MS_U8 u8BlueBrightness = 0;

    Hal_ACE_GetBrightness(pInstance, bScalerWin, &u8RedBrightness, &u8GreenBrightness, &u8BlueBrightness);

    switch(eACEInfoType)
    {
        case ACE_INFO_TYPE_BRIGHTNESS_R:
            u16Val = (MS_U16) u8RedBrightness;
            break;

        case ACE_INFO_TYPE_BRIGHTNESS_G:
            u16Val = (MS_U16) u8GreenBrightness;
            break;

        case ACE_INFO_TYPE_BRIGHTNESS_B:
            u16Val = (MS_U16) u8BlueBrightness;
            break;

        default:
            u16Val = 0;
            break;
    }

    return u16Val;
}


#ifdef ENABLE_SET_PRE_Y_OFFSET
extern void* pu32ACE_XCInst;
extern MLOAD_TYPE MApi_XC_MLoad_GetStatus_U2(void *pInstance);
extern MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate);
extern MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
#endif
/******************************************************************************/
///Set Pre Y offset
///@param bScalerMainWin \b IN
///@param u8PreYOffset \b IN
///- u16PreYOffset value from 0 -0xff
/// @return @ref MS_BOOL
/******************************************************************************/
MS_BOOL MDrv_ACE_SetPreYOffset(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8PreYOffset)
{
#ifdef ENABLE_SET_PRE_Y_OFFSET
    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);
    if((MApi_XC_MLoad_GetStatus_U2(pu32ACE_XCInst) == E_MLOAD_ENABLED) && (!Hal_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {
        if (bScalerWin == MAIN_WINDOW)
        {
            MApi_XC_MLoad_WriteCmd(pu32ACE_XCInst, REG_SC_BK1A_0F_L, ((MS_U16)u8PreYOffset), 0xFF);
        }
        else
        {
            MApi_XC_MLoad_WriteCmd(pu32ACE_XCInst, REG_SC_BK1A_0F_L, ((MS_U16)u8PreYOffset<<8), 0xFF00);
        }

        MApi_XC_MLoad_Fire(pInstance, TRUE);

    }
    else
    {
        if (bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK1A_0F_L, ((MS_U16)u8PreYOffset), 0xFF);
        }
        else
        {
            SC_W2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK1A_0F_L, ((MS_U16)u8PreYOffset<<8), 0xFF00);
        }
    }
    return TRUE;
#else
    return FALSE;
#endif
}

/******************************************************************************/
///Get Pre Y offset
///@param bScalerMainWin \b IN
///@param pu8PreYOffset \b OUT
///- pu8PreYOffset value from 0 -0xff
/// @return @ref MS_U8
/******************************************************************************/
MS_U8 MDrv_ACE_GetPreYOffset(void* pInstance, MS_BOOL bScalerWin)
{
    MS_U8 u8PreYOffset= 0;
#ifdef ENABLE_SET_PRE_Y_OFFSET
    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);
    if (bScalerWin == MAIN_WINDOW)
    {
        u8PreYOffset = SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK1A_0F_L, 0xFF);
    }
    else
    {
        u8PreYOffset = (MS_U8)(SC_R2BYTEMSK(psACEInstPri->u32DeviceID, REG_SC_BK1A_0F_L, 0xFF00) >> 8);
    }
#endif
    return u8PreYOffset;
}

////////////////////////////////////////////////////////////////////////////////
//
//  MWE start
//
////////////////////////////////////////////////////////////////////////////////


MS_U8 MWE_Border_Color[] =
{
    0x00, // E_XC_FREE_RUN_COLOR_BLACK
    0xFF, // E_XC_FREE_RUN_COLOR_WHITE
    0x03, // E_XC_FREE_RUN_COLOR_BLUE
    0xE0, // E_XC_FREE_RUN_COLOR_RED
    0x1C, // E_XC_FREE_RUN_COLOR_GREEN
};

void MDrv_XC_ACE_MWEinit(void* pInstance, MS_U16 u16MWEHstart, MS_U16 u16MWEVstart, MS_U16 u16MWEWidth, MS_U16 u16MWEHeight)
{
    _ACE_GET_VARIABLE();
    MWEPanel.u16Pnl_Height = u16MWEHeight;
    MWEPanel.u16Pnl_Width  = u16MWEWidth;
    MWEPanel.u16Pnl_Hstart = u16MWEHstart;
    MWEPanel.u16Pnl_Vstart = u16MWEVstart;
}

//===============================================================================

void MDrv_XC_ACE_MWESetWin(void* pInstance, MWE_RECT *pRect)
{
    MS_U16 u16hstart, u16hend, u16vstart, u16vend;
    _ACE_GET_VARIABLE();

    u16hstart = pRect->Xpos + MWEPanel.u16Pnl_Hstart;
    u16hend   = u16hstart + pRect->Xsize - 1;
    u16vstart = pRect->Ypos + MWEPanel.u16Pnl_Vstart;
    u16vend   = u16vstart + pRect->Ysize - 1;

    if(u16hend > MWEPanel.u16Pnl_Hstart + MWEPanel.u16Pnl_Width - 1)
        u16hend = MWEPanel.u16Pnl_Hstart + MWEPanel.u16Pnl_Width - 1;

    if(u16vend > MWEPanel.u16Pnl_Vstart + MWEPanel.u16Pnl_Height - 1)
        u16vend = MWEPanel.u16Pnl_Vstart + MWEPanel.u16Pnl_Height - 1;
    MS_DEBUG_MSG(printf("[MWE] Hstart %4x  Hend %4x  Vstart %4x Vend %4x \n",
            u16hstart, u16hend, u16vstart, u16vend));
    Hal_ACE_MWESetWin(pInstance, u16hstart, u16hend, u16vstart, u16vend);
}

void MDrv_XC_ACE_MWECloneVisualEffect(void* pInstance)
{
    Hal_ACE_MWECloneVisualEffect(pInstance);
}

void MDrv_ACE_3DClonePQMap(void* pInstance, XC_ACE_WEAVETYPE enWeaveType)
{
    Hal_ACE_3DClonePQMap(pInstance, enWeaveType);
}

void MDrv_XC_ACE_MWESetRegRow(void* pInstance, MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask)
{
    _ACE_GET_INST_PRIVATE();
    if(u32Addr & 0x1)
    {
        SC_W2BYTEMSK(psACEInstPri->u32DeviceID, (MS_U32)(u32Addr & 0xFFFFFE), (MS_U16)u8Value << 8, (MS_U16)u8Mask << 8);
    }
    else
    {
        SC_W2BYTEMSK(psACEInstPri->u32DeviceID, (MS_U32)u32Addr, (MS_U16)u8Value, (MS_U16)u8Mask);
    }
}

//===============================================================================

void MDrv_XC_ACE_MWESetBorder(void* pInstance, MS_BOOL bScalerWin, MS_BOOL ben, MS_U8 u8color, MS_U16 u16_l, MS_U16 u16_r, MS_U16 u16_t, MS_U16 u16_d)
{
    MS_U16 u16color =  (u8color >= MWE_BORDER_COLOR_MAX)
                    ? MWE_Border_Color[MWE_BORDER_COLOR_BLACK]
                    : MWE_Border_Color[u8color];

    if(ben)
    {
        Hal_ACE_MWESetBorder(pInstance, bScalerWin, u16color, u16_l, u16_r, u16_t, u16_d);
    }

}

//===============================================================================


void MDrv_XC_ACE_MWEEnable(void* pInstance, MS_BOOL bEnable, MS_BOOL bLoadFromTable)
{
     Hal_ACE_MWEEnable(pInstance, bEnable, bLoadFromTable);
}

////////////////////////////////////////////////////////////////////////////////
//
//  MWE end
//
////////////////////////////////////////////////////////////////////////////////

MS_BOOL MDrv_XC_ACE_Set_ICC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    if(u16ByteSize > 256*2)
        return FALSE;

    Hal_ACE_Set_ICC_SRAM(pInstance, pBuf, u16ByteSize/2);
    return TRUE;
}

MS_BOOL MDrv_XC_ACE_Set_IHC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    #define H_SIZE  17
    #define V_SIZE  17
    #define MAX_SRAM_SIZE 0x100
    #define BUF_BYTE_SIZE 17*17

    MS_U8 v_idx, h_idx;
    MS_U8 row, col;
    MS_U16 data;
    MS_U16 cont1, cont2, cont3, cont4;
    MS_U16 SRAM1_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM2_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM3_IHC[MAX_SRAM_SIZE];
    MS_U16 SRAM4_IHC[MAX_SRAM_SIZE];

    memset(SRAM1_IHC, 0, sizeof(SRAM1_IHC));
    memset(SRAM2_IHC, 0, sizeof(SRAM2_IHC));
    memset(SRAM3_IHC, 0, sizeof(SRAM3_IHC));
    memset(SRAM4_IHC, 0, sizeof(SRAM4_IHC));

    if(u16ByteSize > BUF_BYTE_SIZE)
        return FALSE;

    cont1 = cont2 = cont3 = cont4 = 0;

    for (v_idx=1; v_idx <= V_SIZE; v_idx++)
        for (h_idx=1; h_idx <= H_SIZE-1; h_idx++)
        {
            data = pBuf[(v_idx-1)*H_SIZE + h_idx-1]; //pBuf[v_idx-1][h_idx-1]

            row = (int)(v_idx-(v_idx/2)*2);
            col = (int)(h_idx-(h_idx/2)*2);

            if ( (row==1) & (col==1) )
            {
                SRAM1_IHC[cont1]=data;
                cont1 = cont1 < MAX_SRAM_SIZE-1 ? cont1+1 :  MAX_SRAM_SIZE-1;
            }

            if ( (row==1) & (col==0) )
            {
                SRAM2_IHC[cont2]=data;
                cont2 = cont2 < MAX_SRAM_SIZE-1 ? cont2+1 :  MAX_SRAM_SIZE-1;
            }

            if ( (row==0) & (col==1) )
            {
                SRAM3_IHC[cont3]=data;
                cont3 = cont3 < MAX_SRAM_SIZE-1 ? cont3+1 :  MAX_SRAM_SIZE-1;
            }

            if ( (row==0) & (col==0) )
            {
                SRAM4_IHC[cont4]=data;
                cont4 = cont4 < MAX_SRAM_SIZE-1 ? cont4+1 :  MAX_SRAM_SIZE-1;
            }

        }

    for (v_idx=1; v_idx <= V_SIZE; v_idx++)
    {
        row = (v_idx-(v_idx/2)*2);

        data = pBuf[(v_idx-1)*H_SIZE+16];

        if (row==1)
        {
            SRAM1_IHC[cont1]=data;
            cont1 = cont1 < MAX_SRAM_SIZE-1 ? cont1+1 :  MAX_SRAM_SIZE-1;
        }
        else
        {
            SRAM3_IHC[cont3]=data;
            cont3 = cont3 < MAX_SRAM_SIZE-1 ? cont3+1 :  MAX_SRAM_SIZE-1;
        }
    }

    Hal_ACE_Set_IHC_SRAM(pInstance, &SRAM1_IHC[0], 0, cont1);
    Hal_ACE_Set_IHC_SRAM(pInstance, &SRAM2_IHC[0], 1, cont2);
    Hal_ACE_Set_IHC_SRAM(pInstance, &SRAM3_IHC[0], 2, cont3);
    Hal_ACE_Set_IHC_SRAM(pInstance, &SRAM4_IHC[0], 3, cont4);

    return TRUE;
}

MS_U8 MDrv_XC_ACE_DNR_GetMotion(void* pInstance)
{
    MS_U8 u8Ret;
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    u8Ret = Hal_ACE_DNR_GetMotion(pInstance);

    RECOVER_SC_BANK

    return u8Ret;
}

MS_U8 MDrv_ACE_DNR_GetCoringThreshold(void* pInstance, MS_BOOL bScalerWin)
{
    return Hal_ACE_DNR_GetCoringThreshold(pInstance, bScalerWin);
}

MS_U8 MDrv_ACE_DNR_GetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin)
{
    return Hal_ACE_DNR_GetSharpnessAdjust(pInstance, bScalerWin);
}

MS_U8 MDrv_ACE_DNR_GetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin)
{
    return Hal_ACE_DNR_GetGuassin_SNR_Threshold(pInstance, bScalerWin);
}

void MDrv_ACE_DNR_SetCoringThreshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetCoringThreshold(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetSharpnessAdjust(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK

}

void MDrv_ACE_DNR_SetNM_V(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetNM_V(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetGNR_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetGNR_0(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetGNR_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetGNR_1(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetCP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetCP(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetDP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetDP(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetNM_H_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetNM_H_0(pInstance, bScalerWin, u16val);
}

void MDrv_ACE_DNR_SetNM_H_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetNM_H_1(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetGray_Ground_Gain(void* pInstance, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetGray_Ground_Gain(pInstance, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetGray_Ground_En(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetGray_Ground_En(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}


void MDrv_ACE_DNR_SetSC_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetSC_Coring(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}


void MDrv_ACE_DNR_SetSpikeNR_0(void* pInstance, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetSpikeNR_0(pInstance, u16val);
}

void MDrv_ACE_DNR_SetSpikeNR_1(void* pInstance, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetSpikeNR_1(pInstance, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetSNR_NM(void* pInstance, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetSNR_NM(pInstance, u16val);

    RECOVER_SC_BANK
}


void MDrv_ACE_DNR_SetBank_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetBank_Coring(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK
}

void MDrv_ACE_DNR_SetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    DECLARA_BANK_VARIABLE
    BACKUP_SC_BANK

    Hal_ACE_DNR_SetGuassin_SNR_Threshold(pInstance, bScalerWin, u16val);

    RECOVER_SC_BANK

}

void MDrv_ACE_DNR_SetNRTable(void* pInstance, MS_U8 *pTbl)
{
    MS_U8 i;
    MS_U16 u16val;

    DECLARA_BANK_VARIABLE

    for(i=0; i<8; i=i+2)
    {
        u16val = ((MS_U16)pTbl[i+1]<<8) | (MS_U16) pTbl[i];

        BACKUP_SC_BANK

        Hal_ACE_DNR_SetNRTbl_Y(pInstance, i, u16val);
        Hal_ACE_DNR_SetNRTbl_C(pInstance, i, u16val);

        RECOVER_SC_BANK

    }
}

void MDrv_ACE_ColorCorrectionTable(void* pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable )
{
    msACESetColorCorrectionTable(pInstance, bScalerWin, psColorCorrectionTable);
}

void MDrv_ACE_SetColorMatrixControl(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable )
{
    msACE_SetColorMatrixControl(pInstance, bScalerWin, bEnable);
}

void MDrv_ACE_SetRBChannelRange(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bRange)
{
    msACE_SetRBChannelRange(pInstance, bScalerWin, bRange);
    HAL_ACE_Set_RBChannelOffset(pInstance, bScalerWin, bRange);
}

void MDrv_ACE_WriteColorMatrixBurst(void* pInstance, MS_BOOL bWindow, MS_U16* psMatrix )
{
    Hal_ACE_Write_Color_Matrix_Burst(pInstance, bWindow,psMatrix);
}

MS_BOOL MDrv_ACE_IsSupportMLoad(void* pInstance)
{
    return Hal_ACE_Is_Support_MLoad(pInstance);
}

//-------------------------------------------------------------------------------------------------
///Set HDR Init
/// @param  pstDLC_HDRInitData       \b IN: the HDR Init Data
/// @return TRUE: DDI call success
/// @attention
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_ACE_SetHDRInit(void* pInstance,ACE_DRV_HDRinit * pstACE_HDRInitData)
{
    memset(&g_ACEinitParameters, 0 , sizeof(ACE_DRV_HDRinit));
    memcpy(&g_ACEinitParameters, pstACE_HDRInitData, sizeof(ACE_DRV_HDRinit));

    msACE_HDR_Main(pInstance);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
void MDrv_ACE_SetSkipWaitVsync(void* pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
   Hal_ACE_SetSkipWaitVsync(pInstance, eWindow, bIsSkipWaitVsyn);
}

MS_BOOL MDrv_ACE_GetSkipWaitVsync(void* pInstance, MS_BOOL bScalerWin)
{
   return Hal_ACE_GetSkipWaitVsync(pInstance, bScalerWin);
}

MS_BOOL MDrv_ACE_SetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val)
{
    return Hal_ACE_SetIHC(pInstance, bScalerWin, eIHC, u8Val);
}

MS_BOOL MDrv_ACE_GetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8* pu8Val)
{
    return Hal_ACE_GetIHC(pInstance, bScalerWin, eIHC, pu8Val);
}

MS_BOOL MDrv_ACE_SetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val)
{
    return Hal_ACE_SetICC(pInstance, bScalerWin, eICC, u8Val);
}

MS_BOOL MDrv_ACE_GetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8* pu8Val)
{
    return Hal_ACE_GetICC(pInstance, bScalerWin, eICC, pu8Val);
}

MS_BOOL MDrv_ACE_SetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val)
{
    return Hal_ACE_SetIBC(pInstance, bScalerWin, eIBC, u8Val);
}

MS_BOOL MDrv_ACE_GetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8* pu8Val)
{
    return Hal_ACE_GetIBC(pInstance, bScalerWin, eIBC, pu8Val);
}

MS_U32 MDrv_ACE_GetDeviceNum(void)
{
    return XC_ACE_SUPPORT_DEVICE_NUM;
}

#undef  _MDRV_ACE_C_

