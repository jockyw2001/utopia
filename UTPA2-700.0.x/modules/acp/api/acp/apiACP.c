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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiACP.c
/// @author MStar Semiconductor,Inc.
/// @brief  ACP Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

#include "apiACP.h"
#include "drvDAC.h"
#include "drvTVEncoder.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiACP_private.h"
#include "apiACP_v2.h"

typedef struct
{
    MS_U8   u8CPC[2];
    MS_U8   u8CPS[34];
}MS_VE_MV_ByteInfo;

typedef struct
{
    //Versioning
    MS_U32  u32VE_MV_BitControl_version;     ///<Version of current structure.
    MS_U32  u32VE_MV_BitControl_Length;      ///<Length of this structure, u32VE_MV_BitControl_Length=sizeof(MS_VE_MV_BitControl)
    //Data
    MS_BOOL bIsYPbPr;
    union
    {
        MS_U8              u8MV_ByteInfo[36];
        MS_VE_MV_ByteInfo  MV_ByteInfo;
    }__attribute__ ((__packed__)) ;
}MS_VE_MV_BitControl;

VE_Result MDrv_VE_SetMV(MS_BOOL bEnble, MS_U32 eType);
VE_Result MDrv_VE_SetMV_BitControl(MS_BOOL bEnable, MS_VE_MV_BitControl MV_BitControl_Data);
VE_Result MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType);
VE_Result MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);
//-------------------------------------------------------------------------------------------------
//  local data structures
//-------------------------------------------------------------------------------------------------
void* g_pACPInst;

/*
 * This Enum type was defined base on EncoderICTest_Forms Rev1_3_K2_0819a.doc, ROVI CONFIDENTIAL
 * - PAL denotes signal standards PAL-B, -D, -G, -H, -I, -K, -L
 * - NTSC TYPE 1/2/3 include NTSC-M, NTSC-J, PAL-M and NTSC-443
 * - PAL TYPE 1/2/3 include PAL, PAL-N, PAL-Nc and SECAM
 */
typedef enum
{
    MS_ACP_CVBS_NTSC_TYPE1 = 0,         //NTSC, AGC only (Table2)
    MS_ACP_CVBS_NTSC_TYPE2,             //NTSC, AGC + 2-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TYPE3,             //NTSC, AGC + 4-line Colorstripe (Table2)
    MS_ACP_CVBS_NTSC_TEST_N01,          //NTSC, RGB Copy Protect on (N01) (Table2)
    MS_ACP_CVBS_NTSC_TEST_N02,          //NTSC, RGB Copy Protect on (N02) (Table2)
    MS_ACP_CVBS_NTSC_TYPE2_TTX,         //NTSC, AGC + 2-line Colorstripe (Table2) with TTX compatibility
    MS_ACP_CVBS_NTSC_TEST_N01_TTX,      //NTSC, RGB Copy Protect on (N01) (Table2) with TTX compatibility
    MS_ACP_CVBS_PAL_TYPE1 = 0x10,       //PAL, Type1, BPP = 0x40 (Table2)
    MS_ACP_CVBS_PAL_TYPE2,              //PAL, Type2, BPP = 0x60 (Table2)
    MS_ACP_CVBS_PAL_TYPE3,              //PAL, Type3, BPP = 0x50 (Table2)
    MS_ACP_CVBS_PAL_TEST_P01,           //PAL, RGB Copy Protect on (P01) (Table2)
    MS_ACP_CVBS_PAL_TEST_P02,           //PAL, RGB Copy Protect on (P02) (Table2)
    MS_ACP_CVBS_PAL_TYPE2_TTX,          //PAL, Type2, BPP = 0x60 (Table2) with TTX compatibility
    MS_ACP_CVBS_PAL_TEST_P01_TTX,       //PAL, RGB Copy Protect on (P01) (Table2) with TTX compatibility
    MS_ACP_COMPONENT_480I_TYPE2 = 0x20, //YPbPr, NTSC-Type2:700/300 Levels (Table4)
    MS_ACP_COMPONENT_480I_TYPE3,        //YPbPr, NTSC-Type3:714/286 Levels (Table4)
    MS_ACP_COMPONENT_480P_EIA_7701,     //YPbPr, 525P EIA-770.1 (Table6)
    MS_ACP_COMPONENT_480P_EIA_7702,     //YPbPr, 525P EIA-770.2 (Table6)
    MS_ACP_COMPONENT_576I_TEST_P01,     //YPbPr, Test P01 (Table8)
    MS_ACP_COMPONENT_576I_TEST_P02,     //YPbPr, Test P02 (Table8)
    MS_ACP_COMPONENT_576P,              //YPbPr, 625P (Table9)
    MS_ACP_SVIDEO_NTSC_TYPE2 = 0x40,    //S-video, NTSC-Type2 (Table4)
    MS_ACP_SVIDEO_NTSC_TYPE3,           //S-video, NTSC-Type3 (Table4)
    MS_ACP_SVIDEO_PAL_TEST_P01,         //S-video, PAL Test P01 (Table8)
    MS_ACP_SVIDEO_PAL_TEST_P02,         //S-video, PAL Test P02 (Table8)
    MS_ACP_TYPE_MAX,
} MS_ACP_TYPE;

/*
 * This Enum type was defined base on DCS for composite (CVBS) and component outputs
 */
typedef enum
{
    MS_ACP_DCS_TYPE_MIN = 0,

    MS_ACP_DCS_CVBS_MIN = MS_ACP_DCS_TYPE_MIN,
    MS_ACP_DCS_CVBS_NTSC = MS_ACP_DCS_CVBS_MIN,
    MS_ACP_DCS_CVBS_PAL,
    MS_ACP_DCS_CVBS_MAX = MS_ACP_DCS_CVBS_PAL,

    MS_ACP_DCS_COMPONENT_MIN = 0x10,
    MS_ACP_DCS_COMPONENT_480I = MS_ACP_DCS_COMPONENT_MIN,
    MS_ACP_DCS_COMPONENT_480P,
    MS_ACP_DCS_COMPONENT_576I,
    MS_ACP_DCS_COMPONENT_576P,
    MS_ACP_DCS_COMPONENT_720P_24,
    MS_ACP_DCS_COMPONENT_720P_25,
    MS_ACP_DCS_COMPONENT_720P_30,
    MS_ACP_DCS_COMPONENT_720P_50,
    MS_ACP_DCS_COMPONENT_720P_60,
    MS_ACP_DCS_COMPONENT_1080I_50,
    MS_ACP_DCS_COMPONENT_1080I_60,
    MS_ACP_DCS_COMPONENT_1080P_24,
    MS_ACP_DCS_COMPONENT_1080P_25,
    MS_ACP_DCS_COMPONENT_1080P_30,
    MS_ACP_DCS_COMPONENT_1080P_50,
    MS_ACP_DCS_COMPONENT_1080P_60,
    MS_ACP_DCS_COMPONENT_MAX = MS_ACP_DCS_COMPONENT_1080P_60,

    MS_ACP_DCS_TYPE_MAX = MS_ACP_DCS_COMPONENT_MAX,
} MS_ACP_DCS_TYPE;

//----------------------------------------------------------------
// MApi_ACP_SetProtection
// enable/disable protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_ACP_SetProtection_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type)
{
    MS_S32 ret = 0;
    MS_ACP_TYPE eType = (MS_ACP_TYPE)u8Type;
    MS_VE_MV_TYPE eVeMVtype = MS_VE_MV_NTSC_START;

    if (bIsYPbPr)
    {
        // component protection, not implemented
        printf("component Macrovision is not ready now! \n");
        ret = E_ACP_FAIL;
    }
    else
    {
        // CVBS protection
        eVeMVtype = (MS_VE_MV_TYPE)eType;
        ret = MDrv_VE_SetMV(bEnable, eVeMVtype);
        if (ret == E_VE_OK)
        {
            ret = E_ACP_OK;
        }
        else if (ret == E_VE_NOT_SUPPORT)
        {
            ret = E_ACP_NOT_SUPPORT;
            if (bEnable)
            {
                goto chip_not_support;
            }
        }
        else
        {
            ret = E_ACP_FAIL;
        }
    }

    return (ACP_Result)ret;

chip_not_support:
    // disconnect output signals (CVBS and component) in this case
    printf("chip-not-support error\n");
    MDrv_DAC_Enable(FALSE, TRUE);
    MDrv_DAC_Enable(FALSE, FALSE);
    return (ACP_Result)ret;
}

ACP_Result MApi_ACP_SetProtection(MS_BOOL bEnable, MS_BOOL bIsYPbPr, MS_U8 u8Type)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_SET_PROTECTION ACPArgs;
    ACPArgs.bEnable = bEnable;
    ACPArgs.bIsYPbPr = bIsYPbPr;
    ACPArgs.u8Type = u8Type;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_SET_PROTECTION, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

//----------------------------------------------------------------
// MApi_ACP_SetMV_BitControl
// enable/disable protection by Bit Control (CPC/CPS) for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @MS_ACP_MV_BitControl: protection data structure
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_ACP_SetMV_BitControl_U2(MS_BOOL bEnable, MS_ACP_MV_BitControl MV_BitControl_Data)
{
    MS_S32 ret = 0;

    if (MV_BitControl_Data.bIsYPbPr)
    {
        // component protection, not implemented
        printf("component Macrovision is not ready now! \n");
        ret = E_ACP_FAIL;
    }
    else
    {
        // CVBS protection by Bit Control (CPC/CPS)
        MS_VE_MV_BitControl VE_MV_BitControl;
        memcpy(&VE_MV_BitControl, &MV_BitControl_Data, sizeof(MV_BitControl_Data));

        ret = MDrv_VE_SetMV_BitControl(bEnable, VE_MV_BitControl);
        if (ret == E_VE_OK)
        {
            ret = E_ACP_OK;
        }
        else if (ret == E_VE_NOT_SUPPORT)
        {
            ret = E_ACP_NOT_SUPPORT;
            if (bEnable)
            {
                goto chip_not_support;
            }
        }
        else
        {
            ret = E_ACP_FAIL;
        }
    }

    return (ACP_Result)ret;

chip_not_support:
    // disconnect output signals (CVBS and component) in this case
    printf("chip-not-support error\n");
    MDrv_DAC_Enable(FALSE, TRUE);
    MDrv_DAC_Enable(FALSE, FALSE);
    return (ACP_Result)ret;
}

ACP_Result MApi_ACP_SetMV_BitControl(MS_BOOL bEnable, MS_ACP_MV_BitControl MV_BitControl_Data)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_SET_MV_BITCONTROL ACPArgs;
    ACPArgs.bEnable = bEnable;
    memcpy(&ACPArgs.MV_BitControl_Data, &MV_BitControl_Data, sizeof(MS_ACP_MV_BitControl));
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_SET_MV_BITCONTROL, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

//----------------------------------------------------------------
// MApi_DCS_SetProtection
// enable/disable DCS protection for composite (CVBS) and component outputs
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @u8Type: protection type
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetProtection_U2(MS_BOOL bEnable, MS_U8 u8Type)
{
    MS_S32 s32Ret = FALSE;
    MS_ACP_DCS_TYPE eType = (MS_ACP_DCS_TYPE)u8Type;

    s32Ret = MDrv_VE_DCS_SetType(bEnable, (MS_U32)eType);
    if(s32Ret > E_VE_OK || s32Ret < E_VE_NOT_SUPPORT)
    {
        return E_ACP_FAIL;
    }

    return (ACP_Result)s32Ret;
}

ACP_Result MApi_DCS_SetProtection(MS_BOOL bEnable, MS_U8 u8Type)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_DCS_PROTECTION ACPArgs;
    ACPArgs.bEnable = bEnable;
    ACPArgs.u8Type = u8Type;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_DCS_PROTECTION, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

//----------------------------------------------------------------
// MApi_DCS_SetActivationKey
// DCS protection Activation Key Setting By External
// @pu8ActivationKeyTbl: External Activation Key Table
// @u8ActivationKeyTblSize: External Activation Key Table Size
// @return ACP_Result
//----------------------------------------------------------------
ACP_Result MApi_DCS_SetActivationKey_U2(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    MS_S32 s32Ret = FALSE;

    s32Ret = MDrv_VE_DCS_SetActivationKey(pu8ActivationKeyTbl, u8ActivationKeyTblSize);
    if(s32Ret > E_ACP_OK || s32Ret < E_ACP_NOT_SUPPORT)
    {
        return E_ACP_FAIL;
    }

    return (ACP_Result)s32Ret;
}

ACP_Result MApi_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    if (g_pACPInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACP, &g_pACPInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACP failed\n");
    		return FALSE;
        }
    }

    stACP_DCS_SETACTIVATIONKEY ACPArgs;
    ACPArgs.pu8ActivationKeyTbl = pu8ActivationKeyTbl;
    ACPArgs.u8ActivationKeyTblSize = u8ActivationKeyTblSize;
    ACPArgs.eReturnValue = E_ACP_FAIL;

    if(UtopiaIoctl(g_pACPInst, E_ACP_DCS_SETACTIVATIONKEY, (void*)&ACPArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return ACPArgs.eReturnValue;
}

