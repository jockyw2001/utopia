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

#include "MsCommon.h"

//--------------------------------------------------------------------------------------------------
//  Internal define
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Internal macros
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL MsOS_MPool_Init(void)
{
    return TRUE;
}

MS_BOOL MsOS_MPool_Close(void)
{
    return TRUE;
}

// @FIXME: support one pool at this stage
MS_BOOL MsOS_MPool_Get(void** ppAddrVirt, MS_U32* pu32AddrPhys, MS_U32* pu32Size, MS_BOOL bNonCache)
{
    return TRUE;
}

inline MS_PHY MsOS_MPool_VA2PA(MS_VIRT pAddrVirt)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrVirt)&0x1fffffff);
#elif (defined(MCU_AEON))
    return ((MS_U32)(pAddrVirt) & ~(0xC0000000));
#else //ARM
    // miu0 (low 256MB) (VA)0x6000:0000~0x7000:0000 --> (PA)0x0000:0000~0x1000:0000 (cached)
    // miu0 (low 256MB) (VA)0x8000:0000~0x9000:0000 --> (PA)0x0000:0000~0x1000:0000 (uncached)
    if ((0x60000000 <= pAddrVirt) && (0x70000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x0fffffff);
    }
    if ((0x80000000 <= pAddrVirt) && (0x90000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x0fffffff);
    }
    // miu0 (high 256MB) (VA)0x7000:0000~0x8000:0000 -> (PA)0x1000:0000~0x2000:0000 (cached)
    // miu0 (high 256MB) (VA)0x9000:0000~0xA000:0000 -> (PA)0x1000:0000~0x2000:0000 (uncached)
    if ((0x70000000 <= pAddrVirt) && (0x80000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x1fffffff);
    }
    if ((0x90000000 <= pAddrVirt) && (0xA0000000 > pAddrVirt))
    {
        return (pAddrVirt & 0x1fffffff);
    }
    // miu1 (low 256MB) (VA)0xA000:0000~0xB000:0000 -> (PA)0x6000:0000~0x7000:0000 (cached)
    // miu1 (low 256MB) (VA)0xC000:0000~0xD000:0000 -> (PA)0x6000:0000~0x7000:0000 (uncached)
    if ((0xA0000000 <= pAddrVirt) && (0xB0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x0fffffff) + HAL_MIU1_BASE);
    }
    if ((0xC0000000 <= pAddrVirt) && (0xD0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x0fffffff) + HAL_MIU1_BASE);
    }
    // miu1 (high 256MB) (VA)0xB000:0000~0xC000:0000 -> (PA)0x7000:0000~0x8000:0000 (cached)
    // miu1 (high 256MB) (VA)0xD000:0000~0xE000:0000 -> (PA)0x7000:0000~0x8000:0000 (uncached)
    if ((0xB0000000 <= pAddrVirt) && (0xC0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x1fffffff) + HAL_MIU1_BASE);
    }
    if ((0xD0000000 <= pAddrVirt) && (0xE0000000 > pAddrVirt))
    {
        return ((pAddrVirt & 0x1fffffff) + HAL_MIU1_BASE);
    }
#endif
	return -1;
}

inline MS_VIRT MsOS_MPool_PA2KSEG1(MS_PHY pAddrPhys)
{
#ifdef MCU_MIPS_4KE
    return (MS_U32)(((MS_U32)pAddrPhys)|0xa0000000);
#elif (defined(MCU_AEON))
    // miu0 (512MB) - (PA) 0x0000:0000~0x2000:0000 -> 0xC000:0000~0xE000:0000

    if((0x00000000 <= pAddrPhys) && (0x20000000 > pAddrPhys))
    {
        return (pAddrPhys + 0xC0000000);
    }

    // miu1 (512MB) - (PA) 0x2000:0000~0x4000:0000 -> 0xE000:0000~0xFFFF:FFFF
    if((0x20000000 <= pAddrPhys) && (0x40000000 > pAddrPhys))
    {
        return (pAddrPhys + 0xC0000000);
    }
#else //ARM
    // miu0 (low 256MB) - (PA)000000:0000~0x1000:0000 -> (VA)0x8000:0000~0x9000:0000 uncached
    if ((0x00000000 <= pAddrPhys) && (0x10000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x80000000);
    }
    // miu0 (high 256MB) - (PA)0x1000:0000~0x2000:0000 -> (VA)0x9000:0000~0xA000:0000 uncached
    if ((0x10000000 <= pAddrPhys) && (0x20000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x80000000);
    }
    // miu1 (low 256MB) - (PA)0x6000:0000~0x7000:0000 -> (VA)0xC000:0000~0xD000:0000 uncached
    if ((0x60000000 <= pAddrPhys) && (0x70000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x60000000);
    }
    // miu1 (high 256MB) - (PA)0x7000:0000~0x8000:0000 -> (VA)0xD000:0000~0xE000:0000 uncached
    if ((0x70000000 <= pAddrPhys) && (0x80000000 > pAddrPhys))
    {
        return (pAddrPhys + 0x60000000);
    }
#endif

	return -1;
}
MS_BOOL MsOS_MPool_Mapping(MS_U8 u8MiuSel, MS_SIZE tOffset, MS_SIZE tMapSize, MS_BOOL bNonCache)
{
    return TRUE;
}

void  MsOS_MPool_SetDbgLevel(MsOSMPool_DbgLevel DbgLevel)
{
}
