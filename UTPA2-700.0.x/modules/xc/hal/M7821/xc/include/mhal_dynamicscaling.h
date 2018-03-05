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
#ifndef MHAL_DYNAMICSCALING_H
#define MHAL_DYNAMICSCALING_H

#ifdef MHAL_DYNAMICSCALING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DS_OP_CMD_LEN 4  //byte
#define MS_DS_NULL_CMD_32BITS    0xFFFF0000;         //Dummy cmd BKFF_FF=0x0000
#define MS_DS_NULL_CMD_64BITS    0x00000009FFFF0000; //Dummy cmd BKFF_7F=0x0000
//----------------------------------------------
//  BKFF_7F just for dummy ds cmd
//----------------------------------------------
#define REG_SC_BKFF_7F_L      _PK_L_(0xFF, 0x7F)
#define REG_SC_BKFF_7F_H      _PK_H_(0xFF, 0x7F)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    DS_IP,
    DS_OP,
} ds_reg_ip_op_sel;

typedef enum
{
    DS_XC,
    DS_MVOP,
    DS_GOP,
} ds_reg_source_sel;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


INTERFACE MS_BOOL MHAL_SC_Set_DynamicScaling(void *pInstance,MS_PHY u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On,SCALER_WIN eWindow);
INTERFACE void MHAL_SC_Set_DynamicScalingFlag(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL MHAL_SC_Get_DynamicScaling_Status(void *pInstance);
INTERFACE MS_BOOL MHAL_SC_Enable_IPMTuneAfterDS(void *pInstance, MS_BOOL bEnable);
INTERFACE void MHal_SC_DynamicScaling_SWReset(void);
INTERFACE void Hal_SC_ResetSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U8 u8DSIndex);
INTERFACE void Hal_SC_WriteSWDSCommand(void *pInstance,SCALER_WIN eWindow, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue,ds_reg_ip_op_sel IPOP_Sel,ds_reg_source_sel Source_Select,XC_DS_CMDCNT *pstXC_DS_CmdCnt);
INTERFACE void Hal_SC_WriteSWDSCommand_Mask(void *pInstance,SCALER_WIN eWindow, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue,ds_reg_ip_op_sel IPOP_Sel,ds_reg_source_sel Source_Select,XC_DS_CMDCNT *pstXC_DS_CmdCnt,MS_U16 u16Mask);
INTERFACE void Hal_SC_WriteSWDSCommand_NonXC(void *pInstance,SCALER_WIN eWindow,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask,ds_reg_ip_op_sel IPOP_Sel,XC_DS_CMDCNT *pstXC_DS_CmdCnt);
INTERFACE void Hal_SC_Add_NullCommand(void *pInstance,SCALER_WIN eWindow,ds_reg_ip_op_sel IPOP_Sel,XC_DS_CMDCNT *pstXC_DS_CmdCnt);

#define Hal_SC_SetSeamlessZapping(args...) 0
#define Hal_SC_GetSeamlessZappingStatus(args...) 0
#define Hal_SC_PrepareSeamlessZapping(args...)
INTERFACE MS_BOOL MHal_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MHal_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow);
INTERFACE void MHal_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow);
INTERFACE void Hal_XC_Enable_DS_64Bits_Command(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Enable_DS_64Bits_Sread_Mode(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Enable_DS_4_Baseaddress_Mode(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_XC_Set_DS_64Bits_MIU_Bus_Sel(void *pInstance);
INTERFACE void Hal_XC_Set_DS_BaseAddress(void *pInstance,MS_PHY u32Base);
INTERFACE void Hal_XC_Set_DS_IndexDepth(void *pInstance);
INTERFACE void Hal_XC_Set_DS_MIU_Sel(void *pInstance,MS_U16 u16DSOnOff,MS_U16 u16DS_MIU_Sel_bit1);
INTERFACE MS_U64 Hal_SC_GenSpreadModeCmd(void *pInstance, MS_U32 u32CmdRegAddr, MS_U16 u16CmdRegValue, MS_U16 u16Mask, ds_reg_source_sel eSourceSelect);
#undef INTERFACE
#endif //MHAL_DYNAMICSCALING_H

