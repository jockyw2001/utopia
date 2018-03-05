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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSYS.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include <unistd.h>     //close
#include <fcntl.h>      // open
#include <sys/ioctl.h>  // ioctl
#include <sys/mman.h>
#include "drvSysInfo.h"




//--------------------------------------------------------------------------------------------------
////  Internal Variable
////--------------------------------------------------------------------------------------------------

static MS_S32              _s32SysFd = -1;


//-------------------------------------------------------------------------------------------------
/// System information
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_SYS_Info(IO_Sys_Info_t *SysInfo)
{
    IO_Sys_Info_t LclSysInfo;  //local system information
    MS_S32 result;

    _s32SysFd = open("/dev/system", O_RDWR);

    if (0 > _s32SysFd)
    {
        return FALSE;
    }

    result = ioctl(_s32SysFd, IOCTL_SYS_INFO, &LclSysInfo);

    if(result)
    {
       return FALSE; 
    }

    SysInfo->LX_MEM_ADDR = LclSysInfo.LX_MEM_ADDR;
    SysInfo->LX_MEM_LENGTH = LclSysInfo.LX_MEM_LENGTH;
    SysInfo->LX_MEM2_ADDR = LclSysInfo.LX_MEM2_ADDR;
    SysInfo->LX_MEM2_LENGTH = LclSysInfo.LX_MEM2_LENGTH;
    SysInfo->EMAC_ADDR = LclSysInfo.EMAC_ADDR;
    SysInfo->EMAC_LENGTH = LclSysInfo.EMAC_LENGTH;
    SysInfo->DRAM_ADDR = LclSysInfo.DRAM_ADDR;
    SysInfo->DRAM_LENGTH = LclSysInfo.DRAM_LENGTH;
    SysInfo->BB_ADDR = LclSysInfo.BB_ADDR;
    SysInfo->BB_LENGTH = LclSysInfo.BB_LENGTH;
    SysInfo->MPOOL_MEM_ADDR = LclSysInfo.MPOOL_MEM_ADDR;
    SysInfo->MPOOL_MEM_LENGTH = LclSysInfo.MPOOL_MEM_LENGTH;
    SysInfo->G3D_MEM0_ADDR = LclSysInfo.G3D_MEM0_ADDR;
    SysInfo->G3D_MEM0_LENGTH = LclSysInfo.G3D_MEM0_LENGTH;
    SysInfo->G3D_MEM1_ADDR = LclSysInfo.G3D_MEM1_ADDR;
    SysInfo->G3D_MEM1_LENGTH = LclSysInfo.G3D_MEM1_LENGTH;
    SysInfo->G3D_CMDQ_ADDR = LclSysInfo.G3D_CMDQ_ADDR;
    SysInfo->G3D_CMDQ_LENGTH = LclSysInfo.G3D_CMDQ_LENGTH;    
    return TRUE;
}
