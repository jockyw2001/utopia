
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

#define RIU_MAP 0xFA000000
#define R2_MAU_BASE     (RIU_MAP+(0x122B00<<1))

typedef enum
{
    SECR2_MAU_2Gx2G = 0, //will not be applied in real projects
    SECR2_MAU_1Gx1G,
    SECR2_MAU_512Mx512M,
    SECR2_MAU_256Mx256M,
} SECR2_MAU_TYPE;

MS_U32 HAL_MsOS_MPool_VA2PA(MS_U32 u32Virt)
{
//for aeon
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }
    if((eSecR2MauType==SECR2_MAU_1Gx1G)||(eSecR2MauType==SECR2_MAU_2Gx2G))
    {
        return ((MS_U32)(u32Virt) & ~(0x80000000));
    }
    else
    {
        return ((MS_U32)(u32Virt) & ~(0xC0000000));
    }

    
    return 0xffffffff;
}

MS_U32 HAL_MsOS_MPool_PA2KSEG0(MS_U32 u32Phys)
{
//for aeon
    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }

    if(eSecR2MauType==SECR2_MAU_1Gx1G)
    {

        // MIU0 (1GB) - PA 0x0000:0000~0x4000:0000 -> (VA) 0x0000:0000~0x4000:0000 cached
        // MIU1 (1GB) - PA 0x4000:0000~0x8000:0000 -> (VA) 0x4000:0000~0x8000:0000 cached
        return (u32Phys + 0x00000000);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0x4000:0000~0x6000:0000 cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0x40000000);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0x6000:0000~0x8000:0000 cached
        if((u32Phys>=0x20000000)&&(u32Phys<0x40000000))
        {
        	return (u32Phys + 0x40000000);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0x4000:0000~0x5000:0000 cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x10000000))
        {
        	return (u32Phys + 0x40000000);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0x5000:0000~0x6000:0000 cached
        if((u32Phys>=0x10000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0x40000000);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffff;
    }

    return 0xFFFFFFFF;
}

MS_U32 HAL_MsOS_MPool_PA2KSEG1(MS_U32 u32Phys)
{

    SECR2_MAU_TYPE eSecR2MauType=SECR2_MAU_1Gx1G;
    MS_U8 u8MauVal=0;
    u8MauVal =(MS_U8)((*((volatile MS_U32 *) (R2_MAU_BASE +(0x01 << 2)) ))>>8);
    switch(u8MauVal)
    {
        case 0x87 : eSecR2MauType=SECR2_MAU_2Gx2G; break;
        case 0x85 : eSecR2MauType=SECR2_MAU_1Gx1G; break;
        case 0x83 : eSecR2MauType=SECR2_MAU_512Mx512M; break;
        case 0x81 : eSecR2MauType=SECR2_MAU_256Mx256M; break;
        default: eSecR2MauType=SECR2_MAU_1Gx1G; break;
    }

    if(eSecR2MauType==SECR2_MAU_1Gx1G)
    {

        // MIU0 (1GB) - PA 0x0000:0000~0x4000:0000 -> (VA) 0x8000:0000~0xC000:0000 non-cached
        // MIU1 (1GB) - PA 0x4000:0000~0x8000:0000 -> (VA) 0xC000:0000~0x10000:0000 non-cached
        return (u32Phys + 0x80000000);
    }
    else if(eSecR2MauType==SECR2_MAU_512Mx512M)
    {
        //512Mx512M
        // MIU0 (512MB) - PA 0x0000:0000~0x2000:0000 -> (VA) 0xC000:0000~0xE000:0000 non-cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0xC0000000);
        }

        // MIU1 (512MB) - PA 0x2000:0000~0x4000:0000 -> (VA) 0xE000:0000~0x10000:0000 non-cached
        if((u32Phys>=0x20000000)&&(u32Phys<0x40000000))
        {
        	return (u32Phys + 0xC0000000);
        }
    }
    else if(eSecR2MauType==SECR2_MAU_256Mx256M)
    {
        // MIU0 (256MB) - PA 0x0000:0000~0x1000:0000 -> (VA) 0xC000:0000~0xD000:0000 non-cached
        if((u32Phys>=0x00000000)&&(u32Phys<0x10000000))
        {
        	return (u32Phys + 0xC0000000);
        }

        // MIU1 (256MB) - PA 0x1000:0000~0x2000:0000 -> (VA) 0xD000:0000~0xE000:0000 non-cached
        if((u32Phys>=0x10000000)&&(u32Phys<0x20000000))
        {
        	return (u32Phys + 0xC0000000);
        }
    }
    else //SECR2_MAU_2Gx2G
    {
    	return 0xffffffff;
    }

    return 0xFFFFFFFF;
}
