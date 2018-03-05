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

#ifndef _HAL_PCMCIA_C_
#define _HAL_PCMCIA_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvPCMCIA.h"

// Internal Definition
#include "regCHIP.h"
#include "regPCMCIA.h"
#include "halPCMCIA.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// Address bus of RIU is 16 bits.
#define PCMCIA_READ_WORD(addr)         READ_WORD(PCMCIA_BASE_ADDRESS + ((addr)<<2))
#define PCMCIA_WRITE_WORD(addr, val)   WRITE_WORD((PCMCIA_BASE_ADDRESS + ((addr)<<2)), (val))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_VIRT u32PCMCIA_RIU_BaseAdd;

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
// Put this function here because hwreg_utility2 only for hal.
void HAL_PCMCIA_Set_RIU_base(MS_VIRT ptrRIU_base)
{
    u32PCMCIA_RIU_BaseAdd = ptrRIU_base;
}

void HAL_PCMCIA_Write_Byte(MS_VIRT ptrAddr, MS_U8 u8Val)
{
    WRITE_BYTE(PCMCIA_BASE_ADDRESS+((ptrAddr<<1)-(ptrAddr&0x01)), u8Val);
}

MS_U8 HAL_PCMCIA_Read_Byte(MS_VIRT ptrAddr)
{
    return READ_BYTE(PCMCIA_BASE_ADDRESS+((ptrAddr<<1)-(ptrAddr&0x01)));    
}

MS_BOOL HAL_PCMCIA_GetIntStatus(ISR_STS* isr_status)
{
    MS_U8 u8Tmp = 0;
    u8Tmp = HAL_PCMCIA_Read_Byte(REG_PCMCIA_STAT_INT_RAW_INT1);
    if(u8Tmp & 0x01)
    {
        isr_status->bCardAInsert = TRUE;
    }
    if(u8Tmp & 0x02)
    {
        isr_status->bCardARemove = TRUE;
    }
    if(u8Tmp & 0x04)
    {
        isr_status->bCardAData = TRUE;
    }
    if(u8Tmp & 0x08)
    {
        isr_status->bCardBInsert = TRUE;
    }
    if(u8Tmp & 0x10)
    {
        isr_status->bCardBRemove = TRUE;
    }
    if(u8Tmp & 0x20)
    {
        isr_status->bCardBData = TRUE;
    }        
    
    
    return TRUE;
}

void   HAL_PCMCIA_ClrInt(MS_U32 bits)
{
    MS_U8 u8Tmp = 0;
    u8Tmp = (0x1 << 0) | // cardA insert
            (0x1 << 1) | // cardA remove
            (0x1 << 2) | // IRQ from cardA
            (0x1 << 3) | // cardB insert
            (0x1 << 4) | // cardB remove
            (0x1 << 5);  // IRQ from cardB
    
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR1, u8Tmp);        
    u8Tmp = 0;
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR1, u8Tmp);            
}

void   HAL_PCMCIA_MaskInt(MS_U32 bits, MS_BOOL bMask)
{
    MS_U8 u8Tmp = 0;
    
    if(bMask == TRUE)
    {
        u8Tmp = (0x1 << 0) | // cardA insert
                (0x1 << 1) | // cardA remove
                (0x1 << 2) | // IRQ from cardA
                (0x1 << 3) | // cardB insert            
                (0x1 << 4) | // cardB remove
                (0x1 << 5);  // IRQ from cardB
    }
    else // unmask
    {
        u8Tmp = 0x0;
    }
    HAL_PCMCIA_Write_Byte(REG_PCMCIA_INT_MASK_CLEAR, u8Tmp);        
}

void   HAL_PCMCIA_ClkCtrl(MS_BOOL bEnable)
{
    if(bEnable)
    {
        PCM_CLKGEN0_REG(REG_CLKGEN0_PCM_CLK) &= ~REG_CLKGEN0_PCM_CLK_MASK;
    }
    else
    {
        PCM_CLKGEN0_REG(REG_CLKGEN0_PCM_CLK) |= REG_CLKGEN0_PCM_CLK_DIS;
    }
}
#endif
