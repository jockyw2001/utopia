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
#define MHAL_ACE_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "apiXC_Ace.h"
#include "mhal_ace.h"
#include "ace_hwreg_utility2.h"
#include "drvACE.h"
#include "hwreg_ace.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define MAIN_WINDOW     0
#define SUB_WINDOW      1

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#include "ACE_private.h"

#include "utopia_dapi.h"
#include "utopia.h"

extern void* pu32ACEInst;
extern void* pu32ACERes[E_ACE_POOL_ID_MAX];

#define SHARPNESS_OFFEST 14 // AP set 14 as default
#define SHARPNESS_MAX    63 // 0x3F

extern MLOAD_TYPE MApi_XC_MLoad_GetStatus(void);
extern MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate);
extern MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static MS_BOOL  bMainWinSkipWaitVsyn = 0, bSubWinSkipWaitVsyn = 0;
#ifdef SUPPORT_BWD
    static MS_BOOL  bNeedStore3DAutoMode = FALSE;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void Hal_ACE_DMS(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bisATV )
{
    if(bisATV)
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(0, REG_SC_BK18_70_L, 0x01, 0x01);
            SC_W2BYTEMSK(0, REG_SC_BK18_71_L, 0x20, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_72_L, 0x06, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_72_L, 0x01 <<8, HBMASK);

            SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x01, 0x01);
            SC_W2BYTEMSK(0, REG_SC_BK19_7C_L, 0x20, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK19_7D_L, 0x06, LBMASK);
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK18_70_L, 0x10, 0x10);
            SC_W2BYTEMSK(0, REG_SC_BK18_73_L, 0x20, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_74_L, 0x06, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_74_L, 0x01 <<8, HBMASK);

            SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x10, 0x10);
            SC_W2BYTEMSK(0, REG_SC_BK19_7E_L, 0x20, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK19_7F_L, 0x06, LBMASK);
        }
    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(0, REG_SC_BK18_70_L, 0x00, 0x01);
            SC_W2BYTEMSK(0, REG_SC_BK18_71_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_72_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_72_L, 0x00 <<8, HBMASK);

            SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x00, 0x01);
            SC_W2BYTEMSK(0, REG_SC_BK19_7C_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK19_7D_L, 0x00, LBMASK);
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK18_70_L, 0x00, 0x10);
            SC_W2BYTEMSK(0, REG_SC_BK18_73_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_74_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK18_74_L, 0x00 <<8, HBMASK);

            SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x00, 0x10);
            SC_W2BYTEMSK(0, REG_SC_BK19_7E_L, 0x00, LBMASK);
            SC_W2BYTEMSK(0, REG_SC_BK19_7F_L, 0x00, LBMASK);
        }
    }
}

MS_VIRT _ACE_RIU_BASE;

// Put this function here because hwreg_utility2 only for hal.
void Hal_ACE_init_riu_base(MS_VIRT ptr_riu_base)
{
    _ACE_RIU_BASE = ptr_riu_base;
}
void Hal_ACE_SetSharpness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Sharpness)
{
#if 0
    MS_U32 u32MappingResult = 0 ;
    MS_BOOL bSigned = FALSE;
    // At Manhattan architecture, make AP sharpness range mapping to -0x3F ~ 0x3F
    // AP set 14 as default sharpness, need to mapping this to 0
    if (u8Sharpness > SHARPNESS_OFFEST)
    {
        u32MappingResult = u8Sharpness - SHARPNESS_OFFEST;
        bSigned = FALSE;
    }
    else
    {
        u32MappingResult = SHARPNESS_OFFEST - u8Sharpness;
        bSigned = TRUE;
    }

    u32MappingResult = u32MappingResult * SHARPNESS_MAX / (SHARPNESS_MAX - SHARPNESS_OFFEST);

    if((MApi_XC_MLoad_GetStatus_U2(pInstance) == E_MLOAD_ENABLED) && (!Hal_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {

        if(bScalerWin == MAIN_WINDOW)
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK19_13_L, ((u32MappingResult&0x3F)<<8 | (bSigned? BIT(14):0x00)), 0x7F00); // 7 bit sharpness!
        }
        else
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK19_17_L, ((u32MappingResult&0x3F)<<8 | (bSigned? BIT(14):0x00)), 0x7F00);
        }
        MApi_XC_MLoad_Fire(pInstance, TRUE);

    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(0, REG_SC_BK19_13_L, ((u32MappingResult&0x3F)<<8 | (bSigned? BIT(14):0x00)), 0x7F00); // 7 bit sharpness!
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK19_17_L, ((u32MappingResult&0x3F)<<8 | (bSigned? BIT(14):0x00)), 0x7F00);
        }
    }
#else

    if((MApi_XC_MLoad_GetStatus_U2(pInstance) == E_MLOAD_ENABLED) && (!Hal_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK22_7F_L, u8Sharpness, 0x7F);
        }
        else
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK19_17_L, (u8Sharpness)<<8, 0x7F00);
        }
        MApi_XC_MLoad_Fire(pInstance, TRUE);

    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(0, REG_SC_BK22_7F_L, u8Sharpness, 0x7F); // 7 bit sharpness!
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK19_17_L, (u8Sharpness)<<8, 0x7F00);
        }
    }
#endif
}

void Hal_ACE_SetBrightness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8RedBrightness, MS_U8 u8GreenBrightness, MS_U8 u8BlueBrightness )
{
    if((MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED) && (!Hal_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {

        if(bScalerWin == MAIN_WINDOW)
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_36_L, ((MS_U16)u8RedBrightness<<3), 0xFFFF );
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_37_L, ((MS_U16)u8GreenBrightness<<3), 0xFFFF );
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_38_L, ((MS_U16)u8BlueBrightness<<3), 0xFFFF );
        }
        else
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_39_L, ((MS_U16)u8RedBrightness<<3), 0xFFFF );
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_3A_L, ((MS_U16)u8GreenBrightness<<3), 0xFFFF );
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_3B_L, ((MS_U16)u8BlueBrightness<<3), 0xFFFF );
        }
        MApi_XC_MLoad_Fire(pInstance, TRUE);


    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTE(0, REG_SC_BK0F_36_L, ((MS_U16)u8RedBrightness<<3) );
            SC_W2BYTE(0, REG_SC_BK0F_37_L, ((MS_U16)u8GreenBrightness<<3) );
            SC_W2BYTE(0, REG_SC_BK0F_38_L, ((MS_U16)u8BlueBrightness<<3) );
        }
        else
        {
            SC_W2BYTE(0, REG_SC_BK0F_39_L, ((MS_U16)u8RedBrightness<<3) );
            SC_W2BYTE(0, REG_SC_BK0F_3A_L, ((MS_U16)u8GreenBrightness<<3) );
            SC_W2BYTE(0, REG_SC_BK0F_3B_L, ((MS_U16)u8BlueBrightness<<3) );
        }
    }
}

void Hal_ACE_SetBrightnessPrecise(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedBrightness, MS_U16 u16GreenBrightness, MS_U16 u16BlueBrightness)
{
    if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
    {

        if(bScalerWin == MAIN_WINDOW)
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_36_L, u16RedBrightness, 0xFFFF);
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_37_L, u16GreenBrightness, 0xFFFF);
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_38_L, u16BlueBrightness, 0xFFFF);
        }
        else
        {
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_39_L, u16RedBrightness, 0xFFFF);
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_3A_L, u16GreenBrightness, 0xFFFF);
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_3B_L, u16BlueBrightness, 0xFFFF);
        }

        MApi_XC_MLoad_Fire(pInstance, TRUE);

    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
        {
            SC_W2BYTE(0, REG_SC_BK0F_36_L, u16RedBrightness);
            SC_W2BYTE(0, REG_SC_BK0F_37_L, u16GreenBrightness);
            SC_W2BYTE(0, REG_SC_BK0F_38_L, u16BlueBrightness);
        }
        else
        {
            SC_W2BYTE(0, REG_SC_BK0F_39_L, u16RedBrightness);
            SC_W2BYTE(0, REG_SC_BK0F_3A_L, u16GreenBrightness);
            SC_W2BYTE(0, REG_SC_BK0F_3B_L, u16BlueBrightness);
        }
    }
}

void Hal_ACE_GetBrightness(void* pInstance, MS_BOOL bScalerWin,  MS_U8* u8RedBrightness, MS_U8* u8GreenBrightness, MS_U8* u8BlueBrightness )
{
    *u8RedBrightness   = 0;
    *u8GreenBrightness = 0;
    *u8BlueBrightness  = 0;

    if(bScalerWin == MAIN_WINDOW)
    {
        *u8RedBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_36_L) & 0xFFFF)>>3);
        *u8GreenBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_37_L) & 0xFFFF)>>3);
        *u8BlueBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_38_L) & 0xFFFF)>>3);
    }
    else
    {
        *u8RedBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_39_L) & 0xFFFF)>>3);
        *u8GreenBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_3A_L) & 0xFFFF)>>3);
        *u8BlueBrightness = (MS_U8)((SC_R2BYTE(0,REG_SC_BK0F_3B_L) & 0xFFFF)>>3);
    }


}


void Hal_ACE_SetPostRGBGain(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain)
{
    if(MAIN_WINDOW == bScalerWin)
    {
        SC_W2BYTE(0, REG_SC_BK25_24_L, u16RedGain);
        SC_W2BYTE(0, REG_SC_BK25_25_L, u16GreenGain);
        SC_W2BYTE(0, REG_SC_BK25_26_L, u16BlueGain);
        SC_W2BYTEMSK(0, REG_SC_BK25_01_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
    }
    else
    {
        SC_W2BYTE(0, REG_SC_BK25_2A_L, u16RedGain);
        SC_W2BYTE(0, REG_SC_BK25_2B_L, u16GreenGain);
        SC_W2BYTE(0, REG_SC_BK25_2C_L, u16BlueGain);
        SC_W2BYTEMSK(0, REG_SC_BK25_11_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
    }
}

void Hal_ACE_SetPostRGBOffset(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
    if(MAIN_WINDOW == bScalerWin)
    {
        SC_W2BYTE(0, REG_SC_BK25_21_L, u16RedOffset);
        SC_W2BYTE(0, REG_SC_BK25_22_L, u16GreenOffset);
        SC_W2BYTE(0, REG_SC_BK25_23_L, u16BlueOffset);
        SC_W2BYTEMSK(0, REG_SC_BK25_01_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
    }
    else
    {
        SC_W2BYTE(0, REG_SC_BK25_27_L, u16RedOffset);
        SC_W2BYTE(0, REG_SC_BK25_28_L, u16GreenOffset);
        SC_W2BYTE(0, REG_SC_BK25_29_L, u16BlueOffset);
        SC_W2BYTEMSK(0, REG_SC_BK25_11_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
    }
}

void Hal_ACE_SetPostRGBGainOffsetTogether(void* pInstance, MS_BOOL bScalerWin,
                                          MS_U16 u16RedGain,   MS_U16 u16GreenGain,   MS_U16 u16BlueGain,
                                          MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
     if(MAIN_WINDOW == bScalerWin)
     {
         SC_W2BYTE(0, REG_SC_BK25_21_L, u16RedOffset);
         SC_W2BYTE(0, REG_SC_BK25_22_L, u16GreenOffset);
         SC_W2BYTE(0, REG_SC_BK25_23_L, u16BlueOffset);
         SC_W2BYTE(0, REG_SC_BK25_24_L, u16RedGain);
         SC_W2BYTE(0, REG_SC_BK25_25_L, u16GreenGain);
         SC_W2BYTE(0, REG_SC_BK25_26_L, u16BlueGain);
         SC_W2BYTEMSK(0, REG_SC_BK25_01_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
     }
     else
     {
        SC_W2BYTE(0, REG_SC_BK25_27_L, u16RedOffset);
        SC_W2BYTE(0, REG_SC_BK25_28_L, u16GreenOffset);
        SC_W2BYTE(0, REG_SC_BK25_29_L, u16BlueOffset);
        SC_W2BYTE(0, REG_SC_BK25_2A_L, u16RedGain);
        SC_W2BYTE(0, REG_SC_BK25_2B_L, u16GreenGain);
        SC_W2BYTE(0, REG_SC_BK25_2C_L, u16BlueGain);
        SC_W2BYTEMSK(0, REG_SC_BK25_11_L, BIT(6) | BIT(5) | BIT(4), BIT(6) | BIT(5) | BIT(4));
     }
}

void Hal_ACE_GetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix)
{
    MS_U8 i;

    if(bScalerWin == MAIN_WINDOW)
    {
        for (i=0; i<9; i++)
        {
            *pu16Matrix = SC_R2BYTE(0, (REG_SC_BK10_26_L + (2 * i)));
            pu16Matrix++;
        }
    }
    else
    {
        for (i=0; i<9; i++)
        {
            *pu16Matrix = SC_R2BYTE(0, (REG_SC_BK0F_1D_L + (2 * i)));
            pu16Matrix++;
        }
    }
}

void Hal_ACE_SetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix)
{
    MS_U8 i,j;
    MS_S16 sTmp;

    for(i=0; i!=3; i++)
    {
        for(j=0; j!=3; j++)
        {
            sTmp = pu16Matrix[i*3+j];
            if( sTmp >= 0x1000 )
            {
                sTmp = ~sTmp +1;
                sTmp &= 0x1fff;
                sTmp |= 0x1000;
                pu16Matrix[i*3+j] = sTmp;
            }
        }
    }

    if(bScalerWin == MAIN_WINDOW)
    {
        for (i=0; i<9; i++)
        {
            SC_W2BYTE(0, (REG_SC_BK10_26_L+(2 * i)), *pu16Matrix);
            pu16Matrix++;
        }
    }
    else
    {
        for (i=0; i<9; i++)
        {
            SC_W2BYTE(0, (REG_SC_BK0F_1D_L+(2 * i)), *pu16Matrix);
            pu16Matrix++;
        }
    }
}

MS_BOOL Hal_ACE_SetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val)
{
    MS_U16 u16Valtmp = (u8Val > 63)?(u8Val - 64):(63 - u8Val);
    MS_U16 u16Direction = (u8Val > 63?0:1) << 6;
	MS_U16 u16Val = u16Direction|u16Valtmp;
    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eIHC)
        {
            case E_ACE_IHC_COLOR_R:
                SC_W2BYTEMSK(0, REG_SC_BK1C_25_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_G:
                SC_W2BYTEMSK(0, REG_SC_BK1C_26_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_B:
                SC_W2BYTEMSK(0, REG_SC_BK1C_26_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_C:
                SC_W2BYTEMSK(0, REG_SC_BK1C_27_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_M:
                SC_W2BYTEMSK(0, REG_SC_BK1C_27_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_Y:
                SC_W2BYTEMSK(0, REG_SC_BK1C_28_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_F:
                SC_W2BYTEMSK(0, REG_SC_BK1C_28_L, u16Val<<8, BITMASK(14:8));
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eIHC)
        {
            case E_ACE_IHC_COLOR_R:
                SC_W2BYTEMSK(0, REG_SC_BK1C_2D_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_G:
                SC_W2BYTEMSK(0, REG_SC_BK1C_2E_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_B:
                SC_W2BYTEMSK(0, REG_SC_BK1C_2E_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_C:
                SC_W2BYTEMSK(0, REG_SC_BK1C_2F_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_M:
                SC_W2BYTEMSK(0, REG_SC_BK1C_2F_L, u16Val<<8, BITMASK(14:8));
                break;
            case E_ACE_IHC_COLOR_Y:
                SC_W2BYTEMSK(0, REG_SC_BK1C_30_L, u16Val, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_F:
                SC_W2BYTEMSK(0, REG_SC_BK1C_30_L, u16Val<<8, BITMASK(14:8));
                break;
            default:
            break;
        }
    }
    return TRUE;
}

MS_BOOL Hal_ACE_GetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8* pu8Val)
{
    MS_U16 u16Value = 0;
    MS_BOOL bPositive = FALSE;

    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eIHC)
        {
            case E_ACE_IHC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_25_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_26_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_26_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_27_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_27_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_28_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_28_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eIHC)
        {
            case E_ACE_IHC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_2D_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_2E_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_2E_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_2F_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_2F_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IHC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_30_L, BITMASK(6:0));
                break;
            case E_ACE_IHC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0, REG_SC_BK1C_30_L, BITMASK(14:8));
                u16Value = u16Value >>8;
                break;
            default:
            break;
        }
    }

    *pu8Val = (MS_U8) (u16Value & 0xFF);
    bPositive = (*pu8Val > 63) ? FALSE:TRUE;
    *pu8Val = (*pu8Val > 63) ? (*pu8Val -64) : (*pu8Val + 64);
    *pu8Val = bPositive ? (*pu8Val) : (63 - *pu8Val);

    return TRUE;
}

MS_BOOL Hal_ACE_SetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val)
{
    MS_U16 u16Valtmp = (u8Val > 15)?(u8Val - 16):(15 - u8Val);
    MS_U16 u16Direction = (u8Val > 15?0:1);
	MS_U16 u16Val = u16Valtmp;
    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eICC)
        {
            case E_ACE_ICC_COLOR_R:
                SC_W2BYTEMSK(0,REG_SC_BK2B_61_L, u16Val<<8, BITMASK(11:8));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<1, BIT(1));
                break;
            case E_ACE_ICC_COLOR_G:
                SC_W2BYTEMSK(0,REG_SC_BK2B_62_L, u16Val, BITMASK(3:0));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<2, BIT(2));
                break;
            case E_ACE_ICC_COLOR_B:
                SC_W2BYTEMSK(0,REG_SC_BK2B_62_L, u16Val<<8, BITMASK(11:8));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<3, BIT(3));
                break;
            case E_ACE_ICC_COLOR_C:
                SC_W2BYTEMSK(0,REG_SC_BK2B_63_L, u16Val, BITMASK(3:0));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<4, BIT(4));
                break;
            case E_ACE_ICC_COLOR_M:
                SC_W2BYTEMSK(0,REG_SC_BK2B_63_L, u16Val<<8, BITMASK(11:8));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<5, BIT(5));
                break;
            case E_ACE_ICC_COLOR_Y:
                SC_W2BYTEMSK(0,REG_SC_BK2B_64_L, u16Val, BITMASK(3:0));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<6, BIT(6));
                break;
            case E_ACE_ICC_COLOR_F:
                SC_W2BYTEMSK(0,REG_SC_BK2B_64_L, u16Val<<8, BITMASK(11:8));
                SC_W2BYTEMSK(0,REG_SC_BK2B_69_L, u16Direction<<7, BIT(7));
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eICC)
        {
            case E_ACE_ICC_COLOR_R:
                SC_W2BYTEMSK(0,REG_SC_BK2B_61_L, u16Val<<12, BITMASK(15:12));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<1, BIT(1));
                break;
            case E_ACE_ICC_COLOR_G:
                SC_W2BYTEMSK(0,REG_SC_BK2B_62_L, u16Val<<4, BITMASK(7:4));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<2, BIT(2));
                break;
            case E_ACE_ICC_COLOR_B:
                SC_W2BYTEMSK(0,REG_SC_BK2B_62_L, u16Val<<12, BITMASK(15:12));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<3, BIT(3));
                break;
            case E_ACE_ICC_COLOR_C:
                SC_W2BYTEMSK(0,REG_SC_BK2B_63_L, u16Val<<4, BITMASK(7:4));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<4, BIT(4));
                break;
            case E_ACE_ICC_COLOR_M:
                SC_W2BYTEMSK(0,REG_SC_BK2B_63_L, u16Val<<12, BITMASK(15:12));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<5, BIT(5));
                break;
            case E_ACE_ICC_COLOR_Y:
                SC_W2BYTEMSK(0,REG_SC_BK2B_64_L, u16Val<<4, BITMASK(7:4));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<6, BIT(6));
                break;
            case E_ACE_ICC_COLOR_F:
                SC_W2BYTEMSK(0,REG_SC_BK2B_64_L, u16Val<<12, BITMASK(15:12));
                SC_W2BYTEMSK(0,REG_SC_BK2B_6A_L, u16Direction<<7, BIT(7));
                break;
            default:
            break;
        }
    }
    return TRUE;
}

MS_BOOL Hal_ACE_GetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8* pu8Val)
{
    MS_U16 u16Value = 0;
    MS_U16 u16Negative = 0;

    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eICC)
        {
            case E_ACE_ICC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_61_L, BITMASK(11:8));
                u16Value = u16Value >> 8;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(1));
                u16Negative = u16Negative >> 1;
                break;
            case E_ACE_ICC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_62_L, BITMASK(3:0));
                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(2));
                u16Negative = u16Negative >> 2;
                break;
            case E_ACE_ICC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_62_L, BITMASK(11:8));
                u16Value = u16Value >> 8;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(3));
                u16Negative = u16Negative >> 3;
                break;
            case E_ACE_ICC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_63_L, BITMASK(3:0));
                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(4));
                u16Negative = u16Negative >> 4;
                break;
            case E_ACE_ICC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_63_L, BITMASK(11:8));
                u16Value = u16Value >> 8;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(5));
                u16Negative = u16Negative >> 5;
                break;
            case E_ACE_ICC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_64_L, BITMASK(3:0));
                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(6));
                u16Negative = u16Negative >> 6;
                break;
            case E_ACE_ICC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_64_L, BITMASK(11:8));
                u16Value = u16Value >> 8;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_69_L, BIT(7));
                u16Negative = u16Negative >> 7;
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eICC)
        {
            case E_ACE_ICC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_61_L, BITMASK(15:12));
                u16Value = u16Value >> 12;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(1));
                u16Negative = u16Negative >> 1;
                break;
            case E_ACE_ICC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_62_L, BITMASK(7:4));
                u16Value = u16Value >> 4;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(2));
                u16Negative = u16Negative >> 2;
                break;
            case E_ACE_ICC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_62_L, BITMASK(15:12));
                u16Value = u16Value >> 12;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(3));
                u16Negative = u16Negative >> 3;
                break;
            case E_ACE_ICC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_63_L, BITMASK(7:4));
                u16Value = u16Value >> 4;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(4));
                u16Negative = u16Negative >> 4;
                break;
            case E_ACE_ICC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_63_L, BITMASK(15:12));
                u16Value = u16Value >> 12;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(5));
                u16Negative = u16Negative >> 5;
                break;
            case E_ACE_ICC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_64_L, BITMASK(7:4));
                u16Value = u16Value >> 4;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(6));
                u16Negative = u16Negative >> 6;
                break;
            case E_ACE_ICC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK2B_64_L, BITMASK(15:12));
                u16Value = u16Value >> 12;

                u16Negative = SC_R2BYTEMSK(0,REG_SC_BK2B_6A_L, BIT(7));
                u16Negative = u16Negative >> 7;
                break;
            default:
            break;
        }
    }

    *pu8Val = (MS_U8) (u16Value & 0xFF);
    *pu8Val = u16Negative ? (15 - *pu8Val) : (16 + *pu8Val);
    return TRUE;
}


MS_BOOL Hal_ACE_SetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val)
{

    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eIBC)
        {
            case E_ACE_IBC_COLOR_R:
                SC_W2BYTEMSK(0,REG_SC_BK1C_11_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_G:
                SC_W2BYTEMSK(0,REG_SC_BK1C_12_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_B:
                SC_W2BYTEMSK(0,REG_SC_BK1C_12_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_C:
                SC_W2BYTEMSK(0,REG_SC_BK1C_13_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_M:
                SC_W2BYTEMSK(0,REG_SC_BK1C_13_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_Y:
                SC_W2BYTEMSK(0,REG_SC_BK1C_14_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_F:
                SC_W2BYTEMSK(0,REG_SC_BK1C_14_L, u8Val<<8, BITMASK(13:8));
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eIBC)
        {
            case E_ACE_IBC_COLOR_R:
                SC_W2BYTEMSK(0,REG_SC_BK1C_19_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_G:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1A_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_B:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1A_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_C:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1B_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_M:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1B_L, u8Val<<8, BITMASK(13:8));
                break;
            case E_ACE_IBC_COLOR_Y:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1C_L, u8Val, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_F:
                SC_W2BYTEMSK(0,REG_SC_BK1C_1C_L, u8Val<<8, BITMASK(13:8));
                break;
            default:
            break;
        }
    }
    return TRUE;
}

MS_BOOL Hal_ACE_GetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8* pu8Val)
{
    MS_U16 u16Value = 0;
    if (bScalerWin > SUB_WINDOW)
    {
        return FALSE;
    }

    if (bScalerWin == MAIN_WINDOW)
    {
        switch(eIBC)
        {
            case E_ACE_IBC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_11_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_12_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_12_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_13_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_13_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_14_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_14_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            default:
            break;
        }
    }
    else
    {
        switch(eIBC)
        {
            case E_ACE_IBC_COLOR_R:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_19_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_G:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1A_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_B:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1A_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_C:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1B_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_M:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1B_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            case E_ACE_IBC_COLOR_Y:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1C_L, BITMASK(5:0));
                break;
            case E_ACE_IBC_COLOR_F:
                u16Value = SC_R2BYTEMSK(0,REG_SC_BK1C_1C_L, BITMASK(13:8));
                u16Value = u16Value >>8;
                break;
            default:
            break;
        }
    }

    *pu8Val = (MS_U8) (u16Value & 0xFF);

    return TRUE;
}


void Hal_ACE_PatchDTGColorChecker(void* pInstance, MS_U8 u8Mode)
{
    static MS_U8 u8Red, u8Green, u8Blue;
    static MS_U8 u8VOP_50L, u8XVYCC_01L, u8VOP_16H;

    if(u8Mode)
    {
        // specific settings for MHEG5
        u8Red      = SC_R2BYTEMSK(0, REG_SC_BK10_17_L, LBMASK);
        u8Green    = SC_R2BYTEMSK(0, REG_SC_BK10_17_L, HBMASK);
        u8Blue     = SC_R2BYTEMSK(0, REG_SC_BK10_18_L, LBMASK);
        u8VOP_50L  = SC_R2BYTEMSK(0, REG_SC_BK10_50_L, LBMASK);

        u8XVYCC_01L= SC_R2BYTEMSK(0, REG_SC_BK25_01_L, LBMASK);
        u8VOP_16H  = SC_R2BYTEMSK(0, REG_SC_BK10_16_L, HBMASK);

        SC_W2BYTEMSK(0, REG_SC_BK10_16_L, 0x0000, 0x0100);
        SC_W2BYTEMSK(0, REG_SC_BK25_01_L, 0x0000, 0x00F0);

        SC_W2BYTEMSK(0, REG_SC_BK10_17_L, 0x80, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_17_L, 0x80 <<8, HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_50_L, 0x00, LBMASK);

        // add from YT
        SC_W2BYTE(0, REG_SC_BK18_41_L, 0x080C);
        SC_W2BYTE(0, REG_SC_BK18_42_L, 0x0808);
        SC_W2BYTE(0, REG_SC_BK18_43_L, 0x1008);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK10_17_L, u8Red, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_17_L, ((MS_U16)u8Green) <<8, HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_18_L, u8Blue, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_50_L, u8VOP_50L, LBMASK);

        SC_W2BYTEMSK(0, REG_SC_BK25_01_L, u8XVYCC_01L, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK10_16_L, ((MS_U16)u8VOP_16H)<<8, HBMASK);
    }
}

void Hal_ACE_SetSlopValue(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8SlopValue)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK1A_10_L, ((MS_U16)u8SlopValue) <<8, HBMASK);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK1A_12_L, ((MS_U16)u8SlopValue) <<8, HBMASK);
    }
}

void Hal_ACE_SetFCC_En(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Reg, MS_BOOL bEn)
{
    MS_U16 u16value;

    if(u8Reg > 7)
        u8Reg = 7;

    if (bEn)
    {
        if(bScalerWin == MAIN_WINDOW)
            u16value = (SC_R2BYTE(0, REG_SC_BK18_10_L) & 0x000F) | (1 << u8Reg);
        else
            u16value = (SC_R2BYTE(0, REG_SC_BK18_11_L) & 0x000F) | (1 << u8Reg);
    }
    else
    {
        if(bScalerWin == MAIN_WINDOW)
            u16value = (SC_R2BYTE(0, REG_SC_BK18_10_L) & 0x000F) & ~(1 << u8Reg);
        else
            u16value = (SC_R2BYTE(0, REG_SC_BK18_11_L) & 0x000F) & ~(1 << u8Reg);
    }

    SC_W2BYTEMSK(0, REG_SC_BK18_10_L, u16value, 0x000F);
}


void Hal_ACE_SetFCC_Cb(void* pInstance, MS_U8 u8Reg, MS_U8 u8value)
{
    MS_U32 u32reg_addr;

    if(u8Reg > 7)
        u8Reg = 7;

    u32reg_addr = REG_SC_BK18_18_L + u8Reg * 2;
    SC_W2BYTEMSK(0, u32reg_addr, (MS_U16)u8value, LBMASK);
}

void Hal_ACE_Set_IHC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt)
{
    MS_U16 i;

    if(u8SRAM_Idx > 3)
    {
        u8SRAM_Idx = 0;
    }

    SC_W2BYTEMSK(0, REG_SC_BK1C_7C_L, BIT(0), BIT(0)); // io_en enable
    SC_W2BYTEMSK(0, REG_SC_BK1C_7C_L, u8SRAM_Idx<<1, BIT(2)|BIT(1)); // sram select

    for(i=0; i<u16Cnt; i++)
    {
        while (SC_R2BYTE(0, REG_SC_BK1C_7E_L) & BIT(15));

        SC_W2BYTEMSK(0, REG_SC_BK1C_7D_L, i, 0x01FF); // address
        SC_W2BYTEMSK(0, REG_SC_BK1C_7E_L, pBuf[i], 0x03FF); //data

        SC_W2BYTEMSK(0, REG_SC_BK1C_7E_L, BIT(15), BIT(15)); // io_w enable
    }

    SC_W2BYTEMSK(0, REG_SC_BK1C_7C_L, 0, BIT(0)); // io_en disable
}

void Hal_ACE_Set_ICC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16Cnt)
{
    MS_U16 i;
    SC_W2BYTEMSK(0, REG_SC_BK1C_78_L, BIT(0), BIT(0)); // io_en enable

    for(i=0; i<u16Cnt; i++)
    {
        while (SC_R2BYTE(0, REG_SC_BK1C_7A_L) & BIT(15));

        SC_W2BYTEMSK(0, REG_SC_BK1C_79_L, i, 0x03FF); // address
        SC_W2BYTEMSK(0, REG_SC_BK1C_7A_L, (pBuf[i] & 0x1FF), 0x01FF);       //data

        SC_W2BYTEMSK(0, REG_SC_BK1C_7A_L, BIT(15), BIT(15)); // io_w enable
    }

    SC_W2BYTEMSK(0, REG_SC_BK1C_78_L, 0, BIT(0)); // io_en disable
}

////////////////////////////////////////////////////////////////////////////////
//
//  MWE start
//
////////////////////////////////////////////////////////////////////////////////
void MHal_SC_SetMWEQuality(void* pInstance)
{
    MS_U16 value ;

    //////////////////////////////////////
    /// Copy main window setting
    //////////////////////////////////////
    // Copy H-Peak
    value = SC_R2BYTE(0, REG_SC_BK19_10_L);
    SC_W2BYTE(0, REG_SC_BK19_14_L, value);
    value = SC_R2BYTE(0, REG_SC_BK19_11_L);
    SC_W2BYTE(0, REG_SC_BK19_15_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_12_L);
    SC_W2BYTE(0, REG_SC_BK19_16_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_13_L);
    SC_W2BYTE(0, REG_SC_BK19_17_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_18_L);
    SC_W2BYTE(0, REG_SC_BK19_28_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_19_L);
    SC_W2BYTE(0, REG_SC_BK19_29_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1A_L);
    SC_W2BYTE(0, REG_SC_BK19_2A_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1B_L);
    SC_W2BYTE(0, REG_SC_BK19_2B_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1C_L);
    SC_W2BYTE(0, REG_SC_BK19_2C_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1D_L);
    SC_W2BYTE(0, REG_SC_BK19_2D_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1E_L);
    SC_W2BYTE(0, REG_SC_BK19_2E_L, value );
    value = SC_R2BYTE(0, REG_SC_BK19_1F_L);
    SC_W2BYTE(0, REG_SC_BK19_2F_L, value );


    // Copy FCC from main window
    value = SC_R2BYTE(0, REG_SC_BK18_10_L);
    SC_W2BYTE(0, REG_SC_BK18_11_L, value);

    // Copy ICC
    value = SC_R2BYTE(0, REG_SC_BK18_30_L) ;
    SC_W2BYTEMSK(0, REG_SC_BK18_30_L, value >> 4, (BIT(2)|BIT(3)));
    value = SC_R2BYTE(0, REG_SC_BK18_31_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_31_L, (value  << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
    value = SC_R2BYTE(0, REG_SC_BK18_32_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_32_L, ( value << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
    value = SC_R2BYTE(0, REG_SC_BK18_33_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_33_L, ( value << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
    value = SC_R2BYTE(0, REG_SC_BK18_34_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_34_L, (value  << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)));
    value = SC_R2BYTE(0, REG_SC_BK18_35_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_35_L, (value << 8), 0xFF00);

    // IBC
    value = SC_R2BYTE(0, REG_SC_BK18_40_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_40_L, value >> 1, BIT(6));
    value = SC_R2BYTE(0, REG_SC_BK18_41_L);
    SC_W2BYTE(0, REG_SC_BK18_45_L, value );
    value = SC_R2BYTE(0, REG_SC_BK18_42_L);
    SC_W2BYTE(0, REG_SC_BK18_46_L, value );
    value = SC_R2BYTE(0, REG_SC_BK18_43_L);
    SC_W2BYTE(0, REG_SC_BK18_47_L, value );
    value = SC_R2BYTE(0, REG_SC_BK18_44_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_48_L, value ,(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)));

    // Y/C noise masking
    value = SC_R2BYTE(0, REG_SC_BK18_55_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_5D_L, value ,(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(7)|BIT(8)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13)|BIT(15)));

    // IHC
    value = SC_R2BYTE(0, REG_SC_BK18_60_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_60_L, value >> 1, BIT(6));
    // YC-Coring
    value = SC_R2BYTE(0, REG_SC_BK18_50_L);
    SC_W2BYTE(0, REG_SC_BK18_58_L, value );
    value = SC_R2BYTE(0, REG_SC_BK18_51_L);
    SC_W2BYTE(0, REG_SC_BK18_59_L, value);
    value = SC_R2BYTE(0, REG_SC_BK18_52_L);
    SC_W2BYTE(0, REG_SC_BK18_5A_L, value);
    value = SC_R2BYTE(0, REG_SC_BK18_53_L);
    SC_W2BYTE(0, REG_SC_BK18_5B_L, value);
    value = SC_R2BYTE(0, REG_SC_BK18_54_L);
    SC_W2BYTE(0, REG_SC_BK18_5C_L, value);

    // copy Y-Adjust
    value = SC_R2BYTE(0, REG_SC_BK1A_0E_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_0E_L, (value << 8), BIT(8) | BIT(9));
    value = SC_R2BYTE(0, REG_SC_BK1A_0F_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_0F_L, (value << 8), 0xFF00);

    // copy BLE
    value = SC_R2BYTE(0, REG_SC_BK1A_10_L);
    SC_W2BYTE(0, REG_SC_BK1A_12_L, value );
    // copy WLE
    value =  SC_R2BYTE(0, REG_SC_BK1A_11_L);
    SC_W2BYTE(0, REG_SC_BK1A_13_L, value );
    // copy Y/C gain control
    value = SC_R2BYTE(0, REG_SC_BK1A_14_L);
    SC_W2BYTE(0, REG_SC_BK1A_15_L, value );
    // Histogram data
    value = SC_R2BYTE(0, REG_SC_BK1A_01_L);
    SC_W2BYTE(0, REG_SC_BK1A_03_L, value );

    // Copy DLC table
    value = SC_R2BYTE(0, REG_SC_BK1A_30_L);
    SC_W2BYTE(0, REG_SC_BK1A_38_L, value);
    value = SC_R2BYTE(0, REG_SC_BK1A_31_L);
    SC_W2BYTE(0, REG_SC_BK1A_39_L, value );
    value = SC_R2BYTE(0, REG_SC_BK1A_32_L);
    SC_W2BYTE(0, REG_SC_BK1A_3A_L, value );
    value = SC_R2BYTE(0, REG_SC_BK1A_33_L);
    SC_W2BYTE(0, REG_SC_BK1A_3B_L, value );
    value = SC_R2BYTE(0, REG_SC_BK1A_34_L);
    SC_W2BYTE(0, REG_SC_BK1A_3C_L, value );
    value = SC_R2BYTE(0, REG_SC_BK1A_35_L);
    SC_W2BYTE(0, REG_SC_BK1A_3D_L, value);
    value = SC_R2BYTE(0, REG_SC_BK1A_36_L);
    SC_W2BYTE(0, REG_SC_BK1A_3E_L, value );
    value = SC_R2BYTE(0, REG_SC_BK1A_37_L);
    SC_W2BYTE(0, REG_SC_BK1A_3F_L, value);

    // Copy Statistic
    value = SC_R2BYTE(0, REG_SC_BK1A_04_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_04_L, value >> 1 , BIT(0));

    // Copy Luma curve
    value = SC_R2BYTE(0, REG_SC_BK1A_04_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_04_L, value  >> 1 , BIT(6));
    // Read Brightness setting from main window
    value = SC_R2BYTE(0, REG_SC_BK0F_36_L);
    SC_W2BYTE(0, REG_SC_BK0F_39_L, value );
    value = SC_R2BYTE(0, REG_SC_BK0F_37_L);
    SC_W2BYTE(0, REG_SC_BK0F_3A_L, value );
    value = SC_R2BYTE(0, REG_SC_BK0F_38_L);
    SC_W2BYTE(0, REG_SC_BK0F_3B_L, value );

    // copy Post Scaling filter selection from main to sub
    value = SC_R2BYTE(0, REG_SC_BK23_0B_L);
    SC_W2BYTE(0, REG_SC_BK23_2B_L, value );

    //copy rgb2yuv enable flag
    value = SC_R2BYTE(0, REG_SC_BK18_06_L);
    SC_W2BYTE(0, REG_SC_BK18_76_L, value);

    // copy pre Y gain control
    value = SC_R2BYTE(0, REG_SC_BK1A_16_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_16_L, (value << 8), 0xFF00);
    //clear sub window low 4bit to keep difference with main
    SC_W2BYTEMSK(0, REG_SC_BK1A_16_L, 0x0000, 0x0F00);

    // color matrix

    {

        value = SC_R2BYTE(0, REG_SC_BK10_26_L);
        SC_W2BYTE(0, REG_SC_BK0F_1D_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_27_L);
        SC_W2BYTE(0, REG_SC_BK0F_1E_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_28_L);
        SC_W2BYTE(0, REG_SC_BK0F_1F_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_29_L);
        SC_W2BYTE(0, REG_SC_BK0F_20_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2A_L);
        SC_W2BYTE(0, REG_SC_BK0F_21_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2B_L);
        SC_W2BYTE(0, REG_SC_BK0F_22_L,value);

        value = SC_R2BYTE(0, REG_SC_BK10_2C_L);
        SC_W2BYTE(0, REG_SC_BK0F_23_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2D_L);
        SC_W2BYTE(0, REG_SC_BK0F_24_L,value );

        value = SC_R2BYTE(0, REG_SC_BK10_2E_L);
        SC_W2BYTE(0, REG_SC_BK0F_25_L,value );

        value = SC_R2BYTE(0, REG_SC_BK10_2F_L);
        SC_W2BYTE(0, REG_SC_BK0F_26_L,value);

    }

    //////////////////////////////////////
    /// Disable
    //////////////////////////////////////
    //disable ICC

    SC_W2BYTEMSK(0, REG_SC_BK18_30_L, 0x00 ,BIT(2));
    //disable IBC
    SC_W2BYTEMSK(0, REG_SC_BK18_40_L, 0x00 ,BIT(6));
    //disable IHC
    SC_W2BYTEMSK(0, REG_SC_BK18_60_L, 0x00 ,BIT(6));
    //disable DLC ( fresh contrast )
    SC_W2BYTEMSK(0, REG_SC_BK18_40_L, 0x00 ,BIT(6));

    // main window DLC
    //REG_WI(REG_SC_DLC(0x04), 1, BIT(7));

}

void Hal_ACE_MWESetWin(void* pInstance, MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend)
{
    if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
    {

        MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_07_L, u16hstart, 0x3FFF);
        MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_08_L, u16hend,   0x3FFF);
        MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_09_L, u16vstart, 0x1FFF);
        MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_0A_L, u16vend,   0x1FFF);

        MApi_XC_MLoad_Fire(pInstance, TRUE);

    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_07_L, u16hstart, 0x3FFF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_08_L, u16hend,   0x3FFF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_09_L, u16vstart, 0x1FFF);
        SC_W2BYTEMSK(0, REG_SC_BK0F_0A_L, u16vend,   0x1FFF);
    }
}

void Hal_ACE_MWECloneVisualEffect(void *pInstance)
{
    MS_U16 value ;

    //////////////////////////////////////
    /// Clone Main Window
    //////////////////////////////////////

    // copy pre Y gain control
    value = SC_R2BYTE(0, REG_SC_BK1A_16_L);
    SC_W2BYTEMSK(0, REG_SC_BK1A_16_L, (value << 8), 0xFF00);
    //copy rgb2yuv enable flag
    value = SC_R2BYTE(0, REG_SC_BK18_06_L);
    SC_W2BYTE(0, REG_SC_BK18_76_L, value);

    //copy color temp
    value = SC_R2BYTE(0, REG_SC_BK25_21_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_27_L, value, 0x07ff);
    value = SC_R2BYTE(0, REG_SC_BK25_22_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_28_L, value, 0x07ff);
    value = SC_R2BYTE(0, REG_SC_BK25_23_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_29_L, value, 0x07ff);

    value = SC_R2BYTE(0, REG_SC_BK25_24_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_2A_L, value, 0x0fff);
    value = SC_R2BYTE(0, REG_SC_BK25_25_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_2B_L, value, 0x0fff);
    value = SC_R2BYTE(0, REG_SC_BK25_26_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_2C_L, value, 0x0fff);
    value = SC_R2BYTE(0, REG_SC_BK25_01_L);
    SC_W2BYTEMSK(0, REG_SC_BK25_11_L, value, BIT(6) | BIT(5) | BIT(4));
    // copy brightness enable flag
    value = SC_R2BYTE(0, REG_SC_BK10_16_L);
    SC_W2BYTEMSK(0, REG_SC_BK0F_18_L, value, 0xFD00);
    // copy color table enable flag
    value = SC_R2BYTE(0, REG_SC_BK10_2F_L);
    SC_W2BYTEMSK(0, REG_SC_BK0F_26_L, value, 0x0037);
    //clone ICC
    value = SC_R2BYTE(0, REG_SC_BK18_30_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_30_L, value >> 4, BIT(2));
    //clone IBC
    value = SC_R2BYTE(0, REG_SC_BK18_40_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_40_L, value >> 1, BIT(6));
    //clone IHC
    value = SC_R2BYTE(0, REG_SC_BK18_60_L);
    SC_W2BYTEMSK(0, REG_SC_BK18_60_L, value >> 1, BIT(6));
    // Settings start from here is a patch to fix registers mismatch for Main/Sub window.
    value = SC_R2BYTE(0,REG_SC_BK1A_76_L);
    SC_W2BYTEMSK(0,REG_SC_BK1A_7E_L, (value ), 0x81FF);
    value = SC_R2BYTE(0,REG_SC_BK1A_77_L);
    SC_W2BYTEMSK(0,REG_SC_BK1A_7F_L, (value ), 0x01FF);
    value = SC_R2BYTE(0,REG_SC_BK1A_78_L);
    SC_W2BYTEMSK(0,REG_SC_BK1A_7B_L, (value ), 0xFFFF);
    value = SC_R2BYTE(0,REG_SC_BK1A_79_L);
    SC_W2BYTEMSK(0,REG_SC_BK1A_7C_L, (value ), 0xFFFF);
    value = SC_R2BYTE(0,REG_SC_BK1A_7A_L);
    SC_W2BYTEMSK(0,REG_SC_BK1A_7D_L, (value ), 0x000F);
}

void Hal_ACE_3DClonePQMap(void *pInstance, XC_ACE_WEAVETYPE enWeaveType)
{
    MS_U16 value = 0;

    if((enWeaveType == E_ACE_WEAVETYPE_NONE) ||
       (enWeaveType == E_ACE_WEAVETYPE_DUALVIEW))
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_10_L, 0x8F00 ,0xFF00);
        SC_W2BYTE(0, REG_SC_BK19_12_L, 0x0000);
        SC_W2BYTEMSK(0, REG_SC_BK19_60_L, 0x00, 0x00FF);
        SC_W2BYTEMSK(0, REG_SC_BK19_31_L, 0x00, BIT(4));
        SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x00, BIT(0));
        //HSP, VSP
        SC_W2BYTEMSK(0, REG_SC_BK1E_10_L, 0x00, BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK2B_10_L, 0x00, 0x07);

        if(enWeaveType == E_ACE_WEAVETYPE_DUALVIEW)
        {
            return;
        }
    }

    //FCC
    {
        value = SC_R2BYTE(0, REG_SC_BK18_10_L);
        SC_W2BYTE(0, REG_SC_BK18_11_L, value);
    }

    //Y/C noise masking
    {
        value = SC_R2BYTE(0, REG_SC_BK18_55_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_5D_L, value ,(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(7)|BIT(8)|BIT(9)|BIT(10)|BIT(11)|BIT(12)|BIT(13)|BIT(15)));
    }

    //YC-Coring
    {
        value = SC_R2BYTE(0, REG_SC_BK18_50_L);
        SC_W2BYTE(0, REG_SC_BK18_58_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_51_L);
        SC_W2BYTE(0, REG_SC_BK18_59_L, value);
        value = SC_R2BYTE(0, REG_SC_BK18_52_L);
        SC_W2BYTE(0, REG_SC_BK18_5A_L, value);
        value = SC_R2BYTE(0, REG_SC_BK18_53_L);
        SC_W2BYTE(0, REG_SC_BK18_5B_L, value);
        value = SC_R2BYTE(0, REG_SC_BK18_54_L);
        SC_W2BYTE(0, REG_SC_BK18_5C_L, value);
    }

    //Y-Adjust
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_0E_L);
        SC_W2BYTEMSK(0, REG_SC_BK1A_0E_L, (value << 8), BIT(8) | BIT(9));
        value = SC_R2BYTE(0, REG_SC_BK1A_0F_L);
        SC_W2BYTEMSK(0, REG_SC_BK1A_0F_L, (value << 8), 0xFF00);
    }

    //Histogram data
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_01_L);
        SC_W2BYTE(0, REG_SC_BK1A_03_L, value );
    }

    //DLC table
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_30_L);
        SC_W2BYTE(0, REG_SC_BK1A_38_L, value);
        value = SC_R2BYTE(0, REG_SC_BK1A_31_L);
        SC_W2BYTE(0, REG_SC_BK1A_39_L, value );
        value = SC_R2BYTE(0, REG_SC_BK1A_32_L);
        SC_W2BYTE(0, REG_SC_BK1A_3A_L, value );
        value = SC_R2BYTE(0, REG_SC_BK1A_33_L);
        SC_W2BYTE(0, REG_SC_BK1A_3B_L, value );
        value = SC_R2BYTE(0, REG_SC_BK1A_34_L);
        SC_W2BYTE(0, REG_SC_BK1A_3C_L, value );
        value = SC_R2BYTE(0, REG_SC_BK1A_35_L);
        SC_W2BYTE(0, REG_SC_BK1A_3D_L, value);
        value = SC_R2BYTE(0, REG_SC_BK1A_36_L);
        SC_W2BYTE(0, REG_SC_BK1A_3E_L, value );
        value = SC_R2BYTE(0, REG_SC_BK1A_37_L);
        SC_W2BYTE(0, REG_SC_BK1A_3F_L, value);
    }

    //CTI
    {
        value = SC_R2BYTE(0, REG_SC_BK23_60_L);
        SC_W2BYTE(0, REG_SC_BK23_70_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_61_L);
        SC_W2BYTE(0, REG_SC_BK23_71_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_62_L);
        SC_W2BYTE(0, REG_SC_BK23_72_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_63_L);
        SC_W2BYTE(0, REG_SC_BK23_73_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_64_L);
        SC_W2BYTE(0, REG_SC_BK23_74_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_65_L);
        SC_W2BYTE(0, REG_SC_BK23_75_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_66_L);
        SC_W2BYTE(0, REG_SC_BK23_76_L, value );

        value = SC_R2BYTE(0, REG_SC_BK23_67_L);
        SC_W2BYTE(0, REG_SC_BK23_77_L, value );
    }


    //rgb2yuv enable flag
    {
        //copy rgb2yuv enable flag
        value = SC_R2BYTE(0, REG_SC_BK18_06_L);
        SC_W2BYTE(0, REG_SC_BK18_76_L, value);
    }

    //color matrix
    {
        value = SC_R2BYTE(0, REG_SC_BK10_26_L);
        SC_W2BYTE(0, REG_SC_BK0F_1D_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_27_L);
        SC_W2BYTE(0, REG_SC_BK0F_1E_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_28_L);
        SC_W2BYTE(0, REG_SC_BK0F_1F_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_29_L);
        SC_W2BYTE(0, REG_SC_BK0F_20_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2A_L);
        SC_W2BYTE(0, REG_SC_BK0F_21_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2B_L);
        SC_W2BYTE(0, REG_SC_BK0F_22_L,value);

        value = SC_R2BYTE(0, REG_SC_BK10_2C_L);
        SC_W2BYTE(0, REG_SC_BK0F_23_L, value );

        value = SC_R2BYTE(0, REG_SC_BK10_2D_L);
        SC_W2BYTE(0, REG_SC_BK0F_24_L,value );

        value = SC_R2BYTE(0, REG_SC_BK10_2E_L);
        SC_W2BYTE(0, REG_SC_BK0F_25_L,value );

        value = SC_R2BYTE(0, REG_SC_BK10_2F_L);
        SC_W2BYTE(0, REG_SC_BK0F_26_L,value);
    }

    //DNR
    {
        value = SC_R2BYTE(0, REG_SC_BK06_21_L);
        SC_W2BYTE(0, REG_SC_BK06_01_L,value);

        value = SC_R2BYTE(0, REG_SC_BK06_26_L);
        SC_W2BYTEMSK(0, REG_SC_BK06_06_L, value, 0x00FF);

        value = SC_R2BYTE(0, REG_SC_BK06_27_L);
        SC_W2BYTE(0, REG_SC_BK06_07_L,value);
    }

    //DBK
    {
        value = SC_R2BYTE(0, REG_SC_BK0C_10_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_10_L, value<<4, BIT(6) | BIT(5) | BIT(4));

        // De-blocking strength F2 [3:0], De-blocking strength F1[11:8]
        // De-blocking motion ratio enable F2 [12], De-blocking motion ratio enable F1 [13],
        value = SC_R2BYTE(0, REG_SC_BK0C_11_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_11_L, value<<8, 0x0F00);
        SC_W2BYTEMSK(0, REG_SC_BK0C_11_L, value<<1, 0x2000);

        value = SC_R2BYTE(0, REG_SC_BK0C_39_L);
        SC_W2BYTE(0, REG_SC_BK0C_3B_L,value);

        value = SC_R2BYTE(0, REG_SC_BK0C_3A_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_3C_L,value,0x00FF);

        value = SC_R2BYTE(0, REG_SC_BK0C_40_L);
        SC_W2BYTE(0, REG_SC_BK0C_43_L,value);

        value = SC_R2BYTE(0, REG_SC_BK0C_41_L);
        SC_W2BYTE(0, REG_SC_BK0C_44_L,value);

        value = SC_R2BYTE(0, REG_SC_BK0C_42_L);
        SC_W2BYTE(0, REG_SC_BK0C_46_L,value);
    }

    //SNR
    {
        value = SC_R2BYTE(0, REG_SC_BK0C_30_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_30_L, value<<4, BIT(6) | BIT(5) | BIT(4));

        value = SC_R2BYTE(0, REG_SC_BK0C_31_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_31_L, value<<8, 0x0F00);
    }

    //NMR
    {
        value = SC_R2BYTE(0, REG_SC_BK0C_50_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_50_L, value<<4, BIT(5) | BIT(4));

        value = SC_R2BYTE(0, REG_SC_BK0C_54_L);
        SC_W2BYTEMSK(0, REG_SC_BK0C_54_L, value<<8, 0xFF00);
    }

    //HSP, VSP
    {
        value = SC_R2BYTE(0, REG_SC_BK23_0B_L);
        SC_W2BYTE(0, REG_SC_BK23_2B_L,value);
        // 6 tap clone
        value = SC_R2BYTE(0, REG_SC_BK23_13_L);
        SC_W2BYTE(0, REG_SC_BK23_33_L,value);
    }

    //MemFormat
    {
        value = SC_R2BYTE(0, REG_SC_BK12_01_L);
        SC_W2BYTE(0, REG_SC_BK12_41_L, value);

        value = SC_R2BYTE(0, REG_SC_BK12_02_L);
        SC_W2BYTE(0, REG_SC_BK12_42_L, value);

        value = SC_R2BYTE(0, REG_SC_BK12_03_L);
        SC_W2BYTEMSK(0, REG_SC_BK12_43_L, value, 0x00FF);

        value = SC_R2BYTE(0, REG_SC_BK12_04_L);
        SC_W2BYTE(0, REG_SC_BK12_44_L, value);

        value = SC_R2BYTE(0, REG_SC_BK12_05_L);
        SC_W2BYTE(0, REG_SC_BK12_45_L, value);

        value = SC_R2BYTE(0, REG_SC_BK12_06_L);
        SC_W2BYTE(0, REG_SC_BK12_46_L, value);

        value = SC_R2BYTE(0, REG_SC_BK23_0C_L);
        SC_W2BYTEMSK(0, REG_SC_BK23_2C_L, value, 0x00FF);
    }

    //Peaking
    {
        value = SC_R2BYTE(0, REG_SC_BK19_10_L);
        SC_W2BYTE(0, REG_SC_BK19_14_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_11_L);
        SC_W2BYTE(0, REG_SC_BK19_15_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_12_L);
        SC_W2BYTE(0, REG_SC_BK19_16_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_13_L);
        SC_W2BYTE(0, REG_SC_BK19_17_L,value);
    }

    //Sharpness
    {
        value = SC_R2BYTE(0, REG_SC_BK19_18_L);
        SC_W2BYTE(0, REG_SC_BK19_28_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_19_L);
        SC_W2BYTE(0, REG_SC_BK19_29_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1A_L);
        SC_W2BYTE(0, REG_SC_BK19_2A_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1B_L);
        SC_W2BYTE(0, REG_SC_BK19_2B_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1C_L);
        SC_W2BYTE(0, REG_SC_BK19_2C_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1D_L);
        SC_W2BYTE(0, REG_SC_BK19_2D_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1E_L);
        SC_W2BYTE(0, REG_SC_BK19_2E_L,value);

        value = SC_R2BYTE(0, REG_SC_BK19_1F_L);
        SC_W2BYTE(0, REG_SC_BK19_2F_L,value);
    }

    //ICC
    {
        value = SC_R2BYTE(0, REG_SC_BK18_30_L) ;
        SC_W2BYTEMSK(0, REG_SC_BK18_30_L, value >> 4, (BIT(2)|BIT(3)));
        value = SC_R2BYTE(0, REG_SC_BK18_31_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_31_L, (value  << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
        value = SC_R2BYTE(0, REG_SC_BK18_32_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_32_L, ( value << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
        value = SC_R2BYTE(0, REG_SC_BK18_33_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_33_L, ( value << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)|BIT(12)|BIT(13)|BIT(14)|BIT(15)));
        value = SC_R2BYTE(0, REG_SC_BK18_34_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_34_L, (value  << 4), (BIT(4)|BIT(5)|BIT(6)|BIT(7)));
        value = SC_R2BYTE(0, REG_SC_BK18_35_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_35_L, (value << 8), 0xFF00);
        value = SC_R2BYTE(0, REG_SC_BK27_30_L) ;
        SC_W2BYTEMSK(0, REG_SC_BK27_30_L, value >> 1, (BIT(6)|BIT(0)));
    }

    //IBC
    {
        value = SC_R2BYTE(0, REG_SC_BK18_40_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_40_L, value >> 1, BIT(6));
        value = SC_R2BYTE(0, REG_SC_BK18_41_L);
        SC_W2BYTE(0, REG_SC_BK18_45_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_42_L);
        SC_W2BYTE(0, REG_SC_BK18_46_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_43_L);
        SC_W2BYTE(0, REG_SC_BK18_47_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_44_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_48_L, value ,(BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5)));
    }

    //IHC
    {
        value = SC_R2BYTE(0, REG_SC_BK18_60_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_60_L, value >> 1, BIT(6));
        value = SC_R2BYTE(0, REG_SC_BK18_61_L);
        SC_W2BYTE(0, REG_SC_BK18_65_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_62_L);
        SC_W2BYTE(0, REG_SC_BK18_66_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_63_L);
        SC_W2BYTE(0, REG_SC_BK18_67_L, value );
        value = SC_R2BYTE(0, REG_SC_BK18_64_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_68_L, value, 0x00FF);
    }

    //ACK
    {
        value = SC_R2BYTE(0, REG_SC_BK18_38_L);
        SC_W2BYTEMSK(0, REG_SC_BK18_38_L, value>>4, BIT(3) | BIT(2));
    }

    //Y offset
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_0F_L);
        SC_W2BYTEMSK(0, REG_SC_BK1A_0F_L, value<<8, 0xFF00);
    }

    //Y/C gain
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_14_L);
        SC_W2BYTE(0, REG_SC_BK1A_15_L,value);

        value = SC_R2BYTE(0, REG_SC_BK1A_16_L);
        SC_W2BYTEMSK(0, REG_SC_BK1A_16_L, value<<8, 0xFF00);
    }

    //DMS
    {
        value = SC_R2BYTE(0, REG_SC_BK26_10_L);
        SC_W2BYTE(0, REG_SC_BK26_14_L,value);

        value = SC_R2BYTE(0, REG_SC_BK26_11_L);
        SC_W2BYTE(0, REG_SC_BK26_15_L,value);

        value = SC_R2BYTE(0, REG_SC_BK26_12_L);
        SC_W2BYTE(0, REG_SC_BK26_16_L,value);

        value = SC_R2BYTE(0, REG_SC_BK26_13_L);
        SC_W2BYTE(0, REG_SC_BK26_17_L,value);

        value = SC_R2BYTE(0, REG_SC_BK26_18_L);
        SC_W2BYTE(0, REG_SC_BK26_1B_L,value);
    }

    //Spike NR
    {
        value = SC_R2BYTE(0, REG_SC_BK26_50_L);
        SC_W2BYTEMSK(0, REG_SC_BK26_50_L, value<<4, 0X00F0);
    }

    //Bri
    {
        value = SC_R2BYTE(0, REG_SC_BK10_16_L);
        SC_W2BYTEMSK(0, REG_SC_BK0F_18_L, value, BIT(8));

        value = SC_R2BYTE(0, REG_SC_BK0F_36_L);
        SC_W2BYTE(0, REG_SC_BK0F_39_L, value );

        value = SC_R2BYTE(0, REG_SC_BK0F_37_L);
        SC_W2BYTE(0, REG_SC_BK0F_3A_L, value );

        value = SC_R2BYTE(0, REG_SC_BK0F_38_L);
        SC_W2BYTE(0, REG_SC_BK0F_3B_L, value );
    }

    //Gamma
    {
        value = SC_R2BYTE(0, REG_SC_BK10_50_L);
        SC_W2BYTEMSK(0, REG_SC_BK0F_18_L, value, BIT(0));
    }

    //DLC
    {
        value = SC_R2BYTE(0, REG_SC_BK1A_04_L);
        SC_W2BYTEMSK(0, REG_SC_BK1A_04_L, value >> 1 , BIT(0) | BIT(6));
    }

    //BLE&WLE
    {
        // copy BLE
        value = SC_R2BYTE(0, REG_SC_BK1A_10_L);
        SC_W2BYTE(0, REG_SC_BK1A_12_L, value );
        // copy WLE
        value =  SC_R2BYTE(0, REG_SC_BK1A_11_L);
        SC_W2BYTE(0, REG_SC_BK1A_13_L, value );
    }

    //HBC
    {
        value = SC_R2BYTE(0, REG_SC_BK10_16_L);
        SC_W2BYTEMSK(0, REG_SC_BK0F_18_L, value, 0xFF00);
    }

    //Color temp
    {
        value = SC_R2BYTE(0, REG_SC_BK25_21_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_27_L, value, 0x07ff);
        value = SC_R2BYTE(0, REG_SC_BK25_22_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_28_L, value, 0x07ff);
        value = SC_R2BYTE(0, REG_SC_BK25_23_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_29_L, value, 0x07ff);

        value = SC_R2BYTE(0, REG_SC_BK25_24_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_2A_L, value, 0x0fff);
        value = SC_R2BYTE(0, REG_SC_BK25_25_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_2B_L, value, 0x0fff);
        value = SC_R2BYTE(0, REG_SC_BK25_26_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_2C_L, value, 0x0fff);
        value = SC_R2BYTE(0, REG_SC_BK25_01_L);
        SC_W2BYTEMSK(0, REG_SC_BK25_11_L, value, 0x00ff);
    }

    if(enWeaveType & E_ACE_WEAVETYPE_H)
    {
        SC_W2BYTEMSK(0, REG_SC_BK23_54_L, 0x00 ,0x80);//disable HDSDD
        SC_W2BYTEMSK(0, REG_SC_BK26_50_L, 0x00 ,0xFF);//disable snr_vertical vertical c low pass and spike NR
        SC_W2BYTEMSK(0, REG_SC_BK0C_70_L, 0x01 ,0x01);//SPF off
        SC_W2BYTEMSK(0, REG_SC_BK0C_10_L, 0x00 ,0x17);//disable SPF DBK
        SC_W2BYTE(0, REG_SC_BK0C_30_L, 0x0000);//disable SPF SNR
        SC_W2BYTE(0, REG_SC_BK0C_50_L, 0x0000);//disable SPF NMR
        SC_W2BYTE(0, REG_SC_BK26_10_L, 0x0000);//disable SPF DMS
        SC_W2BYTEMSK(0, REG_SC_BK23_0B_L, 0x00 ,0xFF00);//disable vsp filter
        SC_W2BYTEMSK(0, REG_SC_BK23_0C_L, 0x00 ,0x0C00);//disable VSP CoRing
        SC_W2BYTEMSK(0, REG_SC_BK23_11_L, 0x00 ,0x80);//disable VSP DeRing
        SC_W2BYTEMSK(0, REG_SC_BK23_13_L, 0x00 ,0x08);//disable VSP PreVBound
        SC_W2BYTE(0, REG_SC_BK19_12_L, 0x0000);//disable VNMR and VLPF
        SC_W2BYTEMSK(0, REG_SC_BK19_7B_L, 0x00 ,0x01);//disable VNMR Y lowY
        SC_W2BYTEMSK(0, REG_SC_BK19_10_L, 0x00 ,0x7000);//disable vertical peaking band
        SC_W2BYTEMSK(0, REG_SC_BK19_31_L, 0x00 ,0x10);//disable vertical adaptive peaking band
        SC_W2BYTEMSK(0, REG_SC_BK19_60_L, 0x00 ,0x01);//disable Post SNR
    }
    if(enWeaveType & E_ACE_WEAVETYPE_V)
    {
        SC_W2BYTEMSK(0, REG_SC_BK22_08_L, 0x00 ,0xFF00);
        // diable H scaling filter
        SC_W2BYTEMSK(0, REG_SC_BK23_0B_L, 0x00 ,0x00FF);
        // RGB 8bits
        SC_W2BYTEMSK(0, REG_SC_BK12_01_L, 0x10 ,0x00FF);
        SC_W2BYTEMSK(0, REG_SC_BK12_02_L, 0x00 ,0xFF00);
        // disable 422To444
        SC_W2BYTEMSK(0, REG_SC_BK23_0C_L, 0x40 ,0x00FF);
        // disable 444To422
        SC_W2BYTEMSK(0, REG_SC_BK02_0A_L, 0x00 ,0x00FF);
        // disable DNR
        SC_W2BYTEMSK(0, REG_SC_BK06_21_L, 0x10 ,0x00FF);
        // disable 2D peaking
        SC_W2BYTEMSK(0, REG_SC_BK19_10_L, 0x80 ,0x00FF);
        // disable Precti
        SC_W2BYTE(0, REG_SC_BK23_60_L, 0x00);
        SC_W2BYTE(0, REG_SC_BK23_61_L, 0x00);
        // disable Postcti
        SC_W2BYTE(0, REG_SC_BK27_20_L, 0x00);
        SC_W2BYTE(0, REG_SC_BK27_21_L, 0x00);
        SC_W2BYTE(0, REG_SC_BK18_50_L, 0x00);
    }

    //disable dms_v12
    SC_W2BYTEMSK(0, REG_SC_BK20_10_L, 0x0000 ,BIT(4));
}


void Hal_ACE_MWEEnable(void *pInstance, MS_BOOL ben, MS_BOOL bLoadFromTable)
{
    if (ben)
    {
        if(!bLoadFromTable)
        {
            MHal_SC_SetMWEQuality(pInstance);
        }
        if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
        {

            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK58_10_L,  BIT(0) , BIT(0));
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_19_L,  0x00, BIT(5));
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L,  BIT(2) , BIT(2) );
#ifdef SUPPORT_BWD // designer request clear this bit when enable mwe
            if (SC_R2BYTEMSK(0, REG_SC_BK46_3F_L, BIT(0)) != 0)
            {
                bNeedStore3DAutoMode = TRUE;
                MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK46_3F_L,  0 , BIT(0));
            }
#endif
            MApi_XC_MLoad_Fire(pInstance, TRUE);

        }
        else
        {
#ifdef SUPPORT_BWD // designer request clear this bit when enable mwe
            if (SC_R2BYTEMSK(0, REG_SC_BK46_3F_L, BIT(0)) != 0)
            {
                bNeedStore3DAutoMode = TRUE;
                SC_W2BYTEMSK(0, REG_SC_BK46_3F_L,  0 , BIT(0));
            }
#endif
            SC_W2BYTEMSK(0, REG_SC_BK20_10_L,  BIT(2) , BIT(2));
            SC_W2BYTEMSK(0, REG_SC_BK10_19_L,  0x00, BIT(5));
            SC_W2BYTEMSK(0, REG_SC_BK58_10_L,  BIT(0) , BIT(0));
        }
    }
    else
    {
        if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
        {
#ifdef SUPPORT_BWD // designer request enable this bit when disable mwe
            if (bNeedStore3DAutoMode == TRUE)
            {
                bNeedStore3DAutoMode = FALSE;
                MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK46_3F_L,  BIT(0), BIT(0));
            }
#endif
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L,  0x00, BIT(2) );
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK58_10_L,  0x00, BIT(0));
            MApi_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_19_L,  BIT(5), BIT(5));
            MApi_XC_MLoad_Fire(pInstance, TRUE);

        }
        else
        {
#ifdef SUPPORT_BWD // designer request enable this bit when disable mwe
            if (bNeedStore3DAutoMode == TRUE)
            {
                bNeedStore3DAutoMode = FALSE;
                SC_W2BYTEMSK(0, REG_SC_BK46_3F_L,  BIT(0), BIT(0));
            }
#endif
            SC_W2BYTEMSK(0, REG_SC_BK20_10_L,  0x00, BIT(2) );
            SC_W2BYTEMSK(0, REG_SC_BK58_10_L,  0x00, BIT(0));
            SC_W2BYTEMSK(0, REG_SC_BK10_19_L,  BIT(5), BIT(5));
        }
    }
}

void Hal_ACE_MWESetBorder(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16color, MS_U16 u16_l, MS_U16 u16_r, MS_U16 u16_t, MS_U16 u16_d)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK10_24_L, ((u16color & 0xFF) <<8), HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_02_L, ((u16_l & 0xFF) <<8), HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_03_L, ((u16_r & 0xFF) <<8), HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_04_L, ((u16_t & 0xFF) <<8), HBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_05_L, ((u16_d & 0xFF) <<8), HBMASK);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_17_L, u16color & 0xFF,  LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_02_L, u16_l & 0xFF, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_03_L, u16_r & 0xFF, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_04_L, u16_t & 0xFF, LBMASK);
        SC_W2BYTEMSK(0, REG_SC_BK0F_05_L, u16_d & 0xFF, LBMASK);
    }
}
////////////////////////////////////////////////////////////////////////////////
//
//  MWE end
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//  DynamicNR start
//
////////////////////////////////////////////////////////////////////////////////

MS_U8 Hal_ACE_DNR_GetMotion(void *pInstance)
{
    MS_U8 u8motion = (MS_U8)(SC_R2BYTE(0, REG_SC_BK0A_1A_L) & 0x007F);
    return u8motion;
}

MS_U8 Hal_ACE_DNR_GetCoringThreshold(void *pInstance, MS_BOOL bScalerWin)
{
    MS_U8 u8val;

    if(bScalerWin == MAIN_WINDOW)
    {
        u8val = SC_R2BYTE(0, REG_SC_BK19_13_L) & 0xFF;
    }
    else
    {
        u8val = SC_R2BYTE(0, REG_SC_BK19_17_L) & 0xFF;
    }

    return u8val;
}

MS_U8 Hal_ACE_DNR_GetSharpnessAdjust(void *pInstance, MS_BOOL bScalerWin)
{
    MS_U8 u8val;

    if(bScalerWin == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(0, REG_SC_BK19_13_L) & 0xFF00)>>8);
    }
    else
    {
        u8val = ((SC_R2BYTE(0, REG_SC_BK19_17_L) & 0xFF00)>>8);
    }

    return u8val;
}

MS_U8 Hal_ACE_DNR_GetGuassin_SNR_Threshold(void *pInstance, MS_BOOL bScalerWin)
{
    MS_U8 u8val;

    if(bScalerWin == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(0, REG_SC_BK19_60_L) & 0xFF00)>>8);
    }
    else
    {
        u8val = ((SC_R2BYTE(0, REG_SC_BK19_61_L) & 0xFF00)>>8);
    }

    return u8val;
}

void Hal_ACE_DNR_SetCoringThreshold(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_13_L, u16val, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_17_L, u16val, 0x00FF);
    }
}

void Hal_ACE_DNR_SetSharpnessAdjust(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_13_L, (u16val<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_17_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetNM_V(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_12_L, u16val, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_16_L, u16val, 0x00FF);
    }
}

void Hal_ACE_DNR_SetGNR_0(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_60_L, u16val, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_61_L, u16val, 0x00FF);
    }
}

void Hal_ACE_DNR_SetGNR_1(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_60_L, (u16val<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_61_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetCP(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_30_L, u16val, 0x00C1); // BIT(0), BIT(1) are reserved.
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_30_L, u16val, 0x000E); // BIT(0), BIT(1) are reserved.
    }
}

void Hal_ACE_DNR_SetDP(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_31_L, u16val, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_31_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetNM_H_0(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_55_L, u16val, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_5D_L, u16val, 0x00FF);
    }
}

void Hal_ACE_DNR_SetNM_H_1(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_55_L, (u16val<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_5D_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetGray_Ground_Gain(void *pInstance, MS_U16 u16val)
{
    SC_W2BYTEMSK(0, REG_SC_BK18_21_L, (u16val<<8), 0xF000);
}

void Hal_ACE_DNR_SetGray_Ground_En(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_10_L, (u16val?(BIT(7)):(0)), 0x0080);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_11_L, (u16val?(BIT(7)):(0)), 0x0080);
    }
}

void Hal_ACE_DNR_SetSC_Coring(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK23_0D_L, (u16val<<8), 0xFF00); // BK23_1B (8bit address)
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK23_2D_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetSpikeNR_0(void *pInstance, MS_U16 u16val)
{
    SC_W2BYTEMSK(0, REG_SC_BK26_50_L, u16val, 0x0001);
}

void Hal_ACE_DNR_SetSpikeNR_1(void *pInstance, MS_U16 u16val)
{
    SC_W2BYTEMSK(0, REG_SC_BK26_50_L, (u16val<<8), 0x0F00);  //BK26_A0 (8bit address)
    SC_W2BYTEMSK(0, REG_SC_BK26_53_L, (u16val>>5), 0x0007);  //BK26_A6 (8bit address)
}

void Hal_ACE_DNR_SetSNR_NM(void *pInstance, MS_U16 u16val)
{
    SC_W2BYTEMSK(0, REG_SC_BK0C_54_L, u16val, 0x00FF);
}

void Hal_ACE_DNR_SetBank_Coring(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTE(0, REG_SC_BK19_33_L, u16val);
        SC_W2BYTE(0, REG_SC_BK19_34_L, u16val);
    }
    else
    {
        SC_W2BYTE(0, REG_SC_BK19_35_L, u16val);
        SC_W2BYTE(0, REG_SC_BK19_36_L, u16val);
    }
}


void Hal_ACE_DNR_SetGuassin_SNR_Threshold(void *pInstance, MS_BOOL bScalerWin, MS_U16 u16val)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_60_L, (u16val<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK19_61_L, (u16val<<8), 0xFF00);
    }
}

void Hal_ACE_DNR_SetNRTbl_Y(void *pInstance, MS_U8 u8Idx, MS_U16 u16val)
{
    SC_W2BYTE(0, (REG_SC_BK06_40_L+u8Idx), u16val);
}

void Hal_ACE_DNR_SetNRTbl_C(void *pInstance, MS_U8 u8Idx, MS_U16 u16val)
{
    SC_W2BYTE(0, (REG_SC_BK06_44_L+u8Idx), u16val);
}

// Write command by Menuload
void Hal_ACE_Write_Color_Matrix_Burst( void *pInstance, MS_BOOL bWindow, MS_U16* psMatrix )
{
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
#else
    MS_U8 i,j;
    MS_U32 ucAddr;
    MS_S16 sTmp;
    //DECLARA_BANK_VARIABLE

    //BACKUP_SC_BANK
    if( bWindow == MAIN_WINDOW )
    {
        //SET_SC_BANK(BANK_CM_MAIN);
        ucAddr = REG_SC_BK10_26_L;

    }
    else
    {
        //SET_SC_BANK(BANK_CM_SUB);
        ucAddr = REG_SC_BK0F_1D_L;

    }

    for(i=0; i!=3; i++)
    {
        for(j=0; j!=3; j++)
        {
            sTmp = psMatrix[i*3+j];
            if( sTmp >= 0 )
            {
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }

            }
            else
            {
                sTmp = sTmp * -1;
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                sTmp = ~sTmp +1;

            }
            MApi_XC_MLoad_WriteCmd(pInstance, ucAddr, sTmp , 0x1FFF);

            ucAddr += 2;
        } // for
    } // for
    MApi_XC_MLoad_Fire(pInstance, TRUE);
#endif
}

// Write command by Menuload
MS_BOOL Hal_ACE_Is_Support_MLoad(void *pInstance)
{
    if (MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
        return TRUE;
    else
        return FALSE;
}

//-------------------------------------------------------------------------------------------------
// Skip Wait Vsync
// @param	eWindow      \b IN: Indicates the window where the ACE function applies to.
// @param Skip wait Vsync    \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
void Hal_ACE_SetSkipWaitVsync(void *pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{

    if(eWindow ==  MAIN_WINDOW )
    {
        bMainWinSkipWaitVsyn= bIsSkipWaitVsyn;
    }
    else
    {
        bSubWinSkipWaitVsyn = bIsSkipWaitVsyn;
    }
}

MS_BOOL Hal_ACE_GetSkipWaitVsync(void *pInstance, MS_BOOL bScalerWin)
{
    if(bScalerWin == MAIN_WINDOW)
    {
        return bMainWinSkipWaitVsyn;
    }
    else
    {
        return bSubWinSkipWaitVsyn;
    }
}

void HAL_ACE_Set_RBChannelOffset(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable)
{
}

void Hal_ACE_Set_Device_Bank_Offset(void)
{
    memset(u32XCDeviceBankOffset, 0, sizeof(MS_U32)*XC_ACE_MAX_DEVICE_NUM);
    u32XCDeviceBankOffset[XC_ACE_DEVICE0] = E_HALACE_DEVICE0_XC_BANK_OFFSET; // Set SC0 reg bank offset
}


////////////////////////////////////////////////////////////////////////////////
//
//  DynamicNR end
//
////////////////////////////////////////////////////////////////////////////////

#undef MHAL_ACE_C

