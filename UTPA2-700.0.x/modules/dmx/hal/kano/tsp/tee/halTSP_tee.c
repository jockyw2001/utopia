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
// Copyright (c) 2015-2020 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTSP_tee.c
// @brief  Transport Stream Processer (TSP) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "regTSP_tee.h"
#include "halTSP_tee.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT                      _virtRegBase = 0;

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define TSP_TSP0_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x2A00 + ((addr)<<2))))
#define TSP_TSP1_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x2C00 + ((addr)<<2))))
#define TSP_MMFI_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x27E00 + ((addr)<<2))))
#define TSP_TSP3_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0xE0400 + ((addr)<<2))))

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSP_Tee_SetBank(MS_VIRT virtBankAddr)
{
    _virtRegBase = virtBankAddr;

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_FWBuf(MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32FwAddr = ((MS_U32)phyAddr >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT;
    MS_U32 u32FwSize = TSP_QMEM_SIZES;

    //printf("[%s][%d] u32FwAddr %x, u32FwSize %x \n", __FUNCTION__, __LINE__, (int)u32FwAddr, (int)u32FwSize);

    TSP_TSP0_REG(REG_TSP0_FW_DMA_ADDR_L) = ((MS_U16)u32FwAddr) & 0xFFFF;
    TSP_TSP1_REG(REG_TSP1_FW_DMA_ADDR_H) = (MS_U16)(u32FwAddr >> 16) & TSP_FW_DMA_ADDR_H_MASK;
    TSP_TSP0_REG(REG_TSP0_FW_DMA_NUM) = (MS_U16)u32FwSize;

    //lock down register
    TSP_TSP1_REG(REG_TSP1_ONEWAY) = TSP_TSP1_REG(REG_TSP1_ONEWAY) | TSP_FW_ONEWAY;

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_VQBuf(MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32VQSize = 0, u32UnitSize = 0;
    MS_PHY phyVQBufStart = phyAddr, phyMiuAddr = 0;
    MS_U8 u8ii = 0;

    u32VQSize = ((u32Size >> MIU_BUS) / TSP_VQ_NUM) << MIU_BUS; //miu alignment
    u32UnitSize = u32VQSize / TSP_VQ_PITCH;

    //printf("[%s][%d] phyAddr %x, u32Size %x \n", __FUNCTION__, __LINE__, (int)phyAddr, (int)u32Size);
    //printf("[%s][%d] u32VQSize %x, u32UnitSize %x \n", __FUNCTION__, __LINE__, (int)u32VQSize, (int)u32UnitSize);
    for(u8ii = 0; u8ii < TSP_VQ_NUM; u8ii++)
    {
        phyMiuAddr = phyVQBufStart >> MIU_BUS;
        if(u8ii == 0)
        {
            TSP_TSP1_REG(REG_TSP1_VQ0_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ0_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ0_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        else if(u8ii == 1)
        {
            TSP_TSP1_REG(REG_TSP1_VQ1_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ1_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ1_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        else if(u8ii == 2)
        {
            TSP_TSP1_REG(REG_TSP1_VQ2_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ2_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ2_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }

        else if(u8ii == 3)
        {
            TSP_TSP1_REG(REG_TSP1_VQ3_BASE_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ3_BASE_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
            TSP_TSP1_REG(REG_TSP1_VQ3_SIZE) = (MS_U16)(u32UnitSize & 0xFFFF);
        }
        phyVQBufStart += u32VQSize;
    }

    return TRUE;

}

MS_BOOL HAL_TSP_Tee_Set_PvrBuf(MS_U8 u8PvrId, MS_PHY phyAddr0, MS_U32 u32Size0, MS_PHY phyAddr1, MS_U32 u32Size1)
{
    MS_PHY phyEnd = phyAddr0 + u32Size0;
    MS_PHY phyMiuAddr = 0;

    if(u8PvrId == 0)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR1_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else if(u8PvrId == 1)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR2_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR2_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR2_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR2_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR2_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR2_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP0_REG(REG_TSP0_PVR2_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP0_REG(REG_TSP0_PVR2_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else if(u8PvrId == 2)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR3_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR3_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR3_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR3_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR3_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR3_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR3_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR3_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else if(u8PvrId == 3)
    {
        phyMiuAddr = phyAddr0 >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR4_HEAD1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR4_HEAD1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR4_TAIL1_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR4_TAIL1_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyEnd = phyAddr1 + u32Size1;
        phyMiuAddr = phyAddr1 >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR4_HEAD2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR4_HEAD2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
        phyMiuAddr = phyEnd >> MIU_BUS;
        TSP_TSP3_REG(REG_TSP3_PVR4_TAIL2_L) = (MS_U16)(phyMiuAddr & 0xFFFF);
        TSP_TSP3_REG(REG_TSP3_PVR4_TAIL2_H) = (MS_U16)((phyMiuAddr >> 16) & 0xFFFF);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Get_PVRWriteAddr(MS_U8 u8PvrId, MS_PHY *pphyAddr)
{
    MS_U16 u16addr = 0, u16addr1 = 0;

    *pphyAddr = 0;

    if(u8PvrId >= TSP_PVR_ENG_NUM)
        return FALSE;

    if(u8PvrId == 0)
    {
        u16addr = TSP_TSP0_REG(REG_TSP0_PVR1_MID1_L);
        u16addr1 = TSP_TSP0_REG(REG_TSP0_PVR1_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }
    else if(u8PvrId == 1)
    {
        u16addr = TSP_TSP0_REG(REG_TSP0_PVR2_MID1_L);
        u16addr1 = TSP_TSP0_REG(REG_TSP0_PVR2_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }

    else if(u8PvrId == 2)
    {
        u16addr = TSP_TSP3_REG(REG_TSP3_PVR3_MID1_L);
        u16addr1 = TSP_TSP3_REG(REG_TSP3_PVR3_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }
    else if(u8PvrId == 3)
    {
        u16addr = TSP_TSP3_REG(REG_TSP3_PVR4_MID1_L);
        u16addr1 = TSP_TSP3_REG(REG_TSP3_PVR4_MID1_H);
        *pphyAddr = (((MS_U32)u16addr) & 0xFFFF) + ((((MS_U32)u16addr1) << 16) & 0xFFFF0000);
        *pphyAddr <<= MIU_BUS;
    }
    else
    {
        return FALSE;
    }


    return TRUE;
}

MS_U32 HAL_TSP_Tee_FilePath2Tsif_Mapping(MS_U8 u8FileEng)
{
    switch (u8FileEng)
    {
        case 0:
            return TSP_TSIF1;
        case 1:
            return TSP_TSIF3;
        case 2:
            return TSP_TSIF0;
        case 3:
            return TSP_TSIF2;
        default:
            return 0xFF;
    }
}

MS_BOOL HAL_TSP_Tee_Set_FileinBuf(MS_U8 u8EngId, MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_U32 u32Tsif = HAL_TSP_Tee_FilePath2Tsif_Mapping(u8EngId);

    switch(u32Tsif)
    {
        case 0:
            TSP_TSP0_REG(REG_TSP0_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_TSP0_REG(REG_TSP0_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            break;
        case 1:
            TSP_TSP3_REG(REG_TSP3_FILE1_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE1_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE1_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE1_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            break;
        case 2:
            TSP_TSP3_REG(REG_TSP3_FILE2_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE2_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE2_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE2_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            break;
        case 3:
            TSP_TSP3_REG(REG_TSP3_FILE3_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE3_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE3_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_TSP3_REG(REG_TSP3_FILE3_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_Tee_Set_MMFIBuf(MS_U8 u8Id, MS_PHY phyAddr, MS_U32 u32Size)
{
    switch(u8Id)
    {
        case 0:
            TSP_MMFI_REG(REG_MMFI_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            break;
        case 1:
            TSP_MMFI_REG(REG_MMFI1_FILE_ADDR_L) = (MS_U16)(phyAddr & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_ADDR_H) = (MS_U16)((phyAddr >> 16) & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_SIZE_L) = (MS_U16)(u32Size & 0xFFFF);
            TSP_MMFI_REG(REG_MMFI1_FILE_SIZE_H) = (MS_U16)((u32Size >> 16) & 0xFFFF);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}
