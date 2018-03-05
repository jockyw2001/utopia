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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_BDMA_H_
#define _HAL_BDMA_H_

////////////////////////////////////////////////////////////////////////////////
/// @file HALbdma.h
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "drvBDMA.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define HAL_BDMA_MS(x)          (3955 * (x))
//two-level fifo
#define HAL_BDMA_DMY_WRCNT      (4)
//Alignment 16
#define HAL_BDMA_ALIGN(x)       (((x)+15) & ~15)
#define BDMA_MEM_FILL           (ENABLE)
#define BDMA_FLASH_COPY         (ENABLE)
#define BDMA_DUMMY_WRCNT        (ENABLE)
#define BDMA_DEV_DATA_WIDTH     (ENABLE)   //device data width
#define BDMA_DMDMCU             (ENABLE)
#define BDMA_TSP                (DISABLE)
#define BDMA_DSP                (ENABLE)
#define BDMA_HK51_1KSRAM        (ENABLE)

#define BDMA_MIN_SIZE           (0x1000)
typedef enum _BDMA_HalDev  //map to BDMA_Dev
{
    E_BDMA_HALDEV_MIU0
    ,E_BDMA_HALDEV_MIU1
    ,E_BDMA_HALDEV_SEARCH
    ,E_BDMA_HALDEV_CRC32
    ,E_BDMA_HALDEV_MEM_FILL
    ,E_BDMA_HALDEV_FLASH
    ,E_BDMA_HALDEV_VDMCU
    ,E_BDMA_HALDEV_DMDMCU
    ,E_BDMA_HALDEV_DSP
    ,E_BDMA_HALDEV_TSP
    ,E_BDMA_HALDEV_1KSRAM_HK51
    ,E_BDMA_HALDEV_MIU2
    ,E_BDMA_HALDEV_MIU3
    ,E_BDMA_HALDEV_NOT_SUPPORT
}BDMA_HalDev;

typedef enum _Hal_BDMA_Ch
{
    E_BDMA_CH_START
    ,E_BDMA_CH0 = E_BDMA_CH_START
    ,E_BDMA_CH1
    ,E_BDMA_CH_END
}BDMA_ChNum;

typedef enum _Hal_BDMA_ACT
{
    E_BDMA_ACT_COPY_MEM
    ,E_BDMA_ACT_COPY_FLASH
    ,E_BDMA_ACT_MEM_FILL
    ,E_BDMA_ACT_SEARCH
    ,E_BDMA_ACT_CRC32
    ,E_BDMA_ACT_MAX
}BDMA_Act;

//Data width
typedef enum _BDMA_Dw
{
    E_BDMA_DW_1BYTE = 0x00
    ,E_BDMA_DW_2BYTE = 0x10
    ,E_BDMA_DW_4BYTE = 0x20
    ,E_BDMA_DW_8BYTE = 0x30
    ,E_BDMA_DW_16BYTE = 0x40
    ,E_BDMA_DW_MAX = E_BDMA_DW_16BYTE
}BDMA_Dw;

//Data width
typedef enum _BDMA_Miu_Alias
{
    E_BDMA_MIU0_ALIAS = 0x00
    ,E_BDMA_MIU1_ALIAS
    ,E_BDMA_MIU2_ALIAS
    ,E_BDMA_MIU3_ALIAS
    ,E_BDMA_MIU_ALIAS_DW_MAX
}BDMA_Miu_Alias;

////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
void HAL_BDMA_Dump_AllReg(void);
void HAL_BDMA_SetIOMapBase(MS_VIRT phy64Base);
MS_U8 HAL_BDMA_ReadByte(MS_PHY phy64RegAddr);
MS_U8 HAL_BDMA_GetDevDw(BDMA_Dev eDev);
MS_U8 HAL_BDMA_GetDevId(BDMA_Dev eDev);
MS_U16 HAL_BDMA_Read2Byte(MS_PHY phy64RegAddr);
MS_U32 HAL_BDMA_Read4Byte(MS_PHY phy64RegAddr);
MS_U32 HAL_BDMA_GetLen(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetSrcAddr(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetDstAddr(BDMA_Act eAct, MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd0(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd1(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCmd2(MS_U8 u8Ch);
MS_U32 HAL_BDMA_GetCRC32(MS_U8 u8Ch);
MS_VIRT HAL_BDMA_GetIOMapBase(void);
MS_U32 HAL_BDMA_GetMatched(MS_PHY phy64SrcAddr, MS_U8 u8Ch);
MS_BOOL HAL_DMA_Is_FlashReady(void);
MS_BOOL HAL_BDMA_WriteRegBit(MS_PHY phy64RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_BOOL HAL_BDMA_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val);
MS_BOOL HAL_BDMA_Write2Byte(MS_PHY phy64u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_BDMA_Write4Byte(MS_PHY phy64u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_BDMA_Stop(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Queued(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Busy(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Int(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Found(MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Is_Done(BDMA_Act eAct, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Clear_Status(BDMA_Act eAct, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Enable_INT(MS_BOOL bEnable, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_MOBF(MS_BOOL bEnable, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetSrcAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDstAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetLen(BDMA_Act eAct, MS_U32 u32Len, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetSrcDev(MS_U8 u8DevCfg, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDstDev(MS_U8 u8DevCfg, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd0(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd1(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetCmd2(MS_U32 u32Cmd, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Set_Addr_Dec(MS_BOOL bDec, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_Set_CRC_Reflect(MS_BOOL bReflect, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_SetDmyWrCnt(MS_U8 u8Cnt, MS_U8 u8Ch);
MS_BOOL HAL_BDMA_TrigOn(MS_U8 u8Src, MS_U8 u8Dst, MS_U8 u8Ch);
void HAL_BDMA_SetSPIOffsetForMCU(void);
#endif  //_MHAL_BDMA_H_

