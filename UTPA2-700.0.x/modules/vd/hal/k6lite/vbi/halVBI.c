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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define _HAL_VBI_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


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
void VBI_TTXInit(MS_U32 u32Addr)
{
    return;
}

void VBI_WSSInit(MS_VIRT ptrAddr)
{
    return;
}

MS_BOOL VBI_TTX_CheckCircuitReady(void)
{
    return TRUE;
}

MS_U16 VBI_TTX_GetPacketCount(void)
{
    return 0;
}

void VBI_TTX_InitSlicer(MS_PHYADDR addr, MS_U16 packetCount)
{
    return;
}

void VBI_TTX_EnableSlicer(MS_BOOL bEnable)
{
    return;
}

MS_U8 VBI_TTX_GetHardware_Indication(void)
{
    return 0;
}

void VBI_GetVPS_Data(MS_U8 *byte1, MS_U8 *byte2, MS_U8 *byte3, MS_U8 *byte4)
{
    return;
}

MS_U16 VBI_GetWSS_Data(void)
{
    return 0;
}

void VBI_Set_PalNC_VideoStandard(void)
{
    return;
}

void VBI_Set_Secam_VideoStandard(void)
{
    return;
}

void VBI_Set_Pal_VideoStandard(void)
{
    return;
}

void VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine)
{
    return;
}

/******************************************************************************/
// API for VBI Slicer Initialization::
// Initializing VBI Slicer HW
/******************************************************************************/
void VBI_CC_Init(MS_U32 u32RiuAddr, MS_PHYADDR u32Addr, MS_U16 u8Len)
{
    return;
}

/******************************************************************************/
/// API to turn on VBI from YPbPr Initialization::
/// Initializing VBI Slicer HW
/******************************************************************************/
void VBI_CC_YPbPr_Init(MS_U8 cvbs_no)
{
    return;
}

/******************************************************************************/
/// API to Set CC data rate::
/// Set CC's data rate
/// @Param  u8Mode \b IN video system mode
/// @return TRUE:: Successfully set
///         FALSE:: Fail
/******************************************************************************/
MS_U8 VBI_CC_DataRateSet(MS_U8 *ptable)
{
    return TRUE;
}

/******************************************************************************/
/// API to get packet count:
///
/******************************************************************************/
MS_U8 VBI_CC_GetPacketCnt(void)
{
    return 0;
}

/******************************************************************************/
/// API to get odd/even byte-found-identification:
/// 2 bits
/******************************************************************************/
MS_U8 VBI_CC_GetByteFoundIndication(void)
{
    return 0;
}

/******************************************************************************/
/// API to get packet data:
///
/// @return (U32) (Odd_1st_Byte + Odd_2nd_Byte + Even_1st_Byte + Even_2nd_Byte)
/******************************************************************************/
MS_U32 VBI_CC_GetPacket(void)
{
    return 0;
}

/******************************************************************************/
/// API to set CC frame count:
///
/******************************************************************************/
void VBI_SetCCFrameCnt(MS_U8 cnt)
{
    return;
}

/******************************************************************************/
/// API to enable CC slicer:
///
/******************************************************************************/
void VBI_CC_EnableSlicer(MS_BOOL bEnable)
{
    return;
}

void VBI_CC_SetCCLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 val) // val -> 0:NTSC, 1:PAL
{
    return;
}
/******************************************************************************/
/// API to set CC SC window length
///
/******************************************************************************/
MS_BOOL VBI_CC_SetSCWindowLen(MS_U8 u8Len)
{
    return FALSE;
}

/******************************************************************************/
/// API to set WSS VPS byte number:
///
/******************************************************************************/
void VBI_SetWssVpsByteNum(MS_U8 cnt)
{
    return;
}

/******************************************************************************/
/// API to enable memory protect (the memory range vbi can access)
///
/******************************************************************************/
MS_BOOL VBI_ProtectMemory(MS_BOOL bEnable, MS_PHYADDR phyAddr, MS_U32 u32Size)
{
    return FALSE;
}

void VBI_RegStateStore(void)
{
    return;
}

/******************************************************************************/
/// API to store register value for STR usage.
///
/******************************************************************************/
void VBI_RegStateRestore(void)
{
    return;
}

#undef _HAL_VBI_C
