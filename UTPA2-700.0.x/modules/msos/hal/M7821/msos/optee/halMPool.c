
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
#include "halMPool.h"
#include "halCHIP.h"

MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr)
{
     MS_PHY u64BusAddr = 0x0;
     // ba = pa + offset
     if( (u64PhyAddr >= HAL_MIU0_BASE) && (u64PhyAddr < HAL_MIU1_BASE) )     // MIU0
                u64BusAddr = u64PhyAddr - HAL_MIU0_BASE + HAL_MIU0_BUS_BASE;
     else if( (u64PhyAddr >= HAL_MIU1_BASE) && (u64PhyAddr < HAL_MIU2_BASE) )        // MIU1
                u64BusAddr = u64PhyAddr - HAL_MIU1_BASE + HAL_MIU1_BUS_BASE;
     else
                u64BusAddr = u64PhyAddr - HAL_MIU2_BASE + HAL_MIU2_BUS_BASE;    // MIU2

     return u64BusAddr;
}

MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT u64Virt)
{
    //===========================================
    // MIU0 (0 ~ 1024MB) (VA) 0x2000:0000 ~ 0x5fff:ffff -> (PA) 0x0000:0000 ~ 0x3fff:ffff (cached)
    if ((HAL_MIU0_BUS_BASE <= u64Virt) && ((HAL_MIU0_BUS_BASE + 0x3fffffff) >= u64Virt))
    {
        return (MS_PHY)(u64Virt - HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU0 (0 ~ 1024MB) (VA) 0x6000:0000 ~ 0x9fff:ffff -> (PA) 0x0000:0000 ~ 0x3fff:ffff (uncached)
    if (((HAL_MIU0_BUS_BASE + 0x40000000UL) <= u64Virt) && ((HAL_MIU0_BUS_BASE + 0x7fffffffUL) >= u64Virt))
    {
        return (MS_PHY)(u64Virt - (HAL_MIU0_BUS_BASE+0x40000000UL));
    }

    //===========================================
    // MIU1 (low 512MB) (VA) 0xa000:0000 ~ 0xbfff:ffff -> (PA) 0x8000:0000 ~ 0x9fff:ffff (cached)
    if ((HAL_MIU1_BUS_BASE <= u64Virt) && ((HAL_MIU1_BUS_BASE + 0x1fffffffUL) >= u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x1fffffffUL) + HAL_MIU1_BASE);
    }

    //===========================================
    // MIU1 (0 ~ 1024MB) (VA) 0xc000:0000 ~ 0xffff:ffff -> (PA) 0x8000:0000 ~ 0xbfff:ffff (uncached)
    if (((HAL_MIU1_BUS_BASE + 0x20000000UL) <= u64Virt) && ((HAL_MIU1_BUS_BASE + 0x5fffffff) >= u64Virt))
    {
        return (MS_PHY)((u64Virt & 0x3fffffffUL) + HAL_MIU1_BASE);
    }
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG0(MS_PHY u64Phys)
{
    //===========================================
    // MIU0 (0~1024MB) - (PA) 0x0000:0000 ~ 0x3fff:ffff -> (VA) 0x2000:0000 ~ 0x5fff:ffff (cached)
    if ((0x00000000UL <= u64Phys) && (0x3fffffffUL >= u64Phys))
    {
        return (MS_VIRT)((u64Phys & 0x3fffffffUL) + HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU1 (low 512MB) - (PA) 0x8000:0000 ~ 0x9fff:ffff -> (VA) 0xA000:0000 ~ 0xbfff:ffff (cached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x1fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE);
    }
}

MS_VIRT HAL_MsOS_MPool_PA2KSEG1(MS_PHY u64Phys)
{
    //===========================================
    // MIU0 (0~1024MB) - (PA) 0x0000:0000 ~ 0x3fff:ffff -> (VA) 0x6000:0000 ~ 0x9fff:ffff (uncached)
    if ((0x00000000UL <= u64Phys) && (0x3fffffffUL >= u64Phys))
    {
        return (MS_VIRT)((u64Phys & 0x3fffffffUL) + HAL_MIU0_BUS_BASE);
    }

    //===========================================
    // MIU1 (0~1024MB) - (PA) 0x8000:0000 ~ 0xBfff:ffff -> (VA) 0xc000:0000 ~ 0xFfff:ffff (uncached)
    if ((HAL_MIU1_BASE <= u64Phys) && ((HAL_MIU1_BASE + 0x3fffffffUL) >= u64Phys))
    {
        return (MS_VIRT)((u64Phys - HAL_MIU1_BASE) + HAL_MIU1_BUS_BASE);
    }
}
