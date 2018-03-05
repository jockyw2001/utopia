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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef _HAL_MIU_H_
#define _HAL_MIU_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _FUNC_NOT_USED()        do {} while ( 0 )

#define MIU_MAX_DEVICE           (2)
//Max MIU Group number
#define MIU_MAX_GROUP            (8)
#define MIU_MAX_GP_CLIENT        (16)
#define MIU_MAX_TBL_CLIENT       (MIU_MAX_GROUP*MIU_MAX_GP_CLIENT)
#define MIU_PAGE_SHIFT           (13) //Unit for MIU protect
#define MIU_PROTECT_ADDRESS_UNIT (0x20) //Unit for MIU hitted address
#define MIU_MAX_PROTECT_BLOCK    (4)
#define MIU_MAX_PROTECT_ID       (16)
#define MIU_BLOCK0_CLIENT_NUMBER (16)
#define MIU_BLOCK1_CLIENT_NUMBER (16)
#define MIU_BLOCK2_CLIENT_NUMBER (16)
#define MIU_BLOCK3_CLIENT_NUMBER (16)

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#define MIU_OPM_R_MASK 0x0667
#define MIU_OPM_W_MASK 0x0666
#define MIU_MVD_R_MASK 0x06F6
#define MIU_MVD_W_MASK 0x06F7

//$ MIU0 Request Mask functions
#define _MaskMiuReq_OPM_R( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1)

#define _MaskMiuReq_DNRB_W( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2)
#define _MaskMiuReq_DNRB_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT3)
#define _MaskMiuReq_DNRB_RW( m )   HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2|BIT3)

#define _MaskMiuReq_SC_RW( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1|BIT2|BIT3)

#define _MaskMiuReq_MVOP_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1L_MASK, m, BIT3)

#define _MaskMiuReq_MVD_R( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiuReq_MVD_W( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiuReq_MVD_RW( m )    do { _MaskMiuReq_MVD_R( m ); _MaskMiuReq_MVD_W( m ); } while (0)

#define _MaskMiuReq_AUDIO_RW( m )  _FUNC_NOT_USED()


//$ MIU1 Request Mask functions
#define _MaskMiu1Req_OPM_R( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1)

#define _MaskMiu1Req_DNRB_W( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2)
#define _MaskMiu1Req_DNRB_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT3)
#define _MaskMiu1Req_DNRB_RW( m )   HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT2|BIT3)

#define _MaskMiu1Req_SC_RW( m )     HAL_MIU_WriteRegBit(MIU_RQ1H_MASK, m, BIT1|BIT2|BIT3)

#define _MaskMiu1Req_MVOP_R( m )    HAL_MIU_WriteRegBit(MIU_RQ1L_MASK, m, BIT3)

#define _MaskMiu1Req_MVD_R( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiu1Req_MVD_W( m )     do { HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT4); HAL_MIU_WriteRegBit(MIU_RQ3L_MASK, m, BIT5); } while(0)
#define _MaskMiu1Req_MVD_RW( m )    do { _MaskMiuReq_MVD_R( m ); _MaskMiuReq_MVD_W( m ); } while (0)

#define _MaskMiu1Req_AUDIO_RW( m )  _FUNC_NOT_USED()

#define MIU_GET_CLIENT_POS(x)       (x & 0x0F)
#define MIU_GET_CLIENT_GROUP(x)     ((x & 0xF0) >> 4)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
  E_MIU_BLOCK_0 = 0,
  E_MIU_BLOCK_1,
  E_MIU_BLOCK_2,
  E_MIU_BLOCK_3,
  E_MIU_BLOCK_NUM,
} MIU_BLOCK_ID;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_MIU_SetIOMapBase(MS_U32 u32Base);
void    HAL_MIU_SetPMIOMapBase(MS_U32 u32Base);
MS_BOOL HAL_MIU_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_U32  HAL_MIU_ProtectAlign(void);
MS_BOOL HAL_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize);
MS_BOOL HAL_MIU_Dram_ReadSize(MS_U8 MiuID, MIU_DDR_SIZE *pDramSize);
MS_U8 HAL_MIU_ClinetNumber(MS_U8 u8Blockx);
MS_BOOL HAL_MIU_Protect(MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_U32 u32Start,
                        MS_U32 u32End, MS_BOOL  bSetFlag);
MS_BOOL HAL_MIU_SetSsc(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable);
MS_BOOL HAL_MIU_IsInitMiu1(void);
MS_BOOL HAL_MIU_SelMIU(eMIU_SelType eType, eMIUClientID eClientID);
MS_BOOL HAL_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo);
MS_BOOL HAL_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority);
MS_BOOL HAL_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask);

MS_U8* HAL_MIU_GetDefaultClientID_KernelProtect(void);

MS_U16  HAL_MIU_GET_MUX(void);
MS_U16  HAL_MIU_IsI64Mode(void);
MS_U16  HAL_MIU_GET_Group0(void);
MS_U16  HAL_MIU_GET_Group1(void);
MS_U16  HAL_MIU_GET_Group2(void);
MS_S16 HAL_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID);

void HAL_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID u16ClientID);
void HAL_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID u16ClientID);
void HAL_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu);
void HAL_MIU_VOP_SwitchMIU(MS_U8 u8MiuID);
void HAL_MIU_PrintMIUProtectInfo(void);

MS_BOOL HAL_MIU_EnableScramble(MS_BOOL bEnable);
MS_BOOL HAL_MIU_IsScrambleEnabled(void);
MS_BOOL HAL_MIU_SetLoadingRequest(MS_U8 u8MiuDev);
MS_BOOL HAL_MIU_GetLoadingRequest(MS_U8 u8MiuDev, MS_U32 *u32Loading);
MS_BOOL HAL_MIU_ParseOccupiedResource(void);
MS_BOOL HAL_MIU_GetClientWidth(MS_U8 u8MiuDevi, eMIUClientID eClientID, eMIU_ClientWidth *pClientWidth);

#endif // _HAL_MIU_H_

