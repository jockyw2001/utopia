
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
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <sys/ioctl.h>  // ioctl
#endif

MS_PHY HAL_MsOS_MPool_VA2PA(MS_VIRT VirtAddr, MS_SIZE tVirt2PhysOff)
{
    return (MS_PHY)(VirtAddr - tVirt2PhysOff);
}

MS_VIRT  HAL_MsOS_MPool_PA2VA(MS_PHY u64Phys, MS_PHY u64MappedPhys, MS_U64 u64MappedSize, MS_SIZE tVirt2PhysOff, MS_BOOL bPartialMapping)
{
    if(bPartialMapping)
    {
        if((u64MappedPhys <= u64Phys) && (u64Phys < (u64MappedPhys + u64MappedSize)))
            return (MS_VIRT)(u64Phys + tVirt2PhysOff);
    }
    else
    {
        return (MS_VIRT)(u64Phys + tVirt2PhysOff);
    }

    return 0;
}
#if !defined(MSOS_TYPE_LINUX_KERNEL)
MS_BOOL HAL_MsOS_MPool_Dcache_Flush(int fd, MS_VIRT addrVirt, MS_SIZE u64Size, MS_PHY u64Phys)
{
    DevMalloc_MPool_Flush_Info_t stFlushInfo;

    stFlushInfo.virtAddrVirt = addrVirt;
    stFlushInfo.u64AddrPhys = u64Phys;
    stFlushInfo.u64Size = u64Size;

    if (ioctl(fd, MALLOC_IOC_FLUSHDCACHE_PAVA, &stFlushInfo))
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_MsOS_MPool_Dcache_Flush_All(int fd)
{
    DevMalloc_MPool_Flush_Info_t stFlushInfo;

    if (ioctl(fd, MALLOC_IOC_FLUSHDCACHE_ALL, &stFlushInfo))
    {
        return FALSE;
    }
    return TRUE;
}
#endif
MS_PHY HAL_MsOS_MPool_PA2BA(MS_PHY u64PhyAddr)
{
    MS_PHY u64BusAddr = 0x0;

    // ba = pa + offset
	if( (u64PhyAddr >= HAL_MIU0_BASE) && (u64PhyAddr < HAL_MIU1_BASE) )	// MIU0
		u64BusAddr = u64PhyAddr - HAL_MIU0_BASE + HAL_MIU0_BUS_BASE;
	else																// MIU1
		u64BusAddr = u64PhyAddr - HAL_MIU1_BASE + HAL_MIU1_BUS_BASE;

	return u64BusAddr;
}

MS_PHY HAL_MsOS_MPool_BA2PA(MS_PHY u64BusAddr)
{
    MS_PHY u64PhyAddr = 0x0;

    // pa = ba - offset
	if( (u64BusAddr >= HAL_MIU0_BUS_BASE) && (u64BusAddr < HAL_MIU1_BUS_BASE) )	// MIU0
		u64PhyAddr = u64BusAddr - HAL_MIU0_BUS_BASE + HAL_MIU0_BASE;
	else																		// MIU1
		u64PhyAddr = u64BusAddr - HAL_MIU1_BUS_BASE + HAL_MIU1_BASE;

	return u64PhyAddr;
}
