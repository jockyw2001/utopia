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
// Copyright (c) 2010-2012 MStar Semiconductor, Inc.
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
// file   halMMFilein.c
// @brief  Multimedia File In (MMFILEIN) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "regMMFilein.h"
#include "halMMFilein.h"
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define MIU_BUS                     4

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl_MMFI*  _MFCtrl= NULL;
static REG_Ctrl_MMFI1* _MFCtrl1= NULL;


static MS_VIRT   _u32MMFIRegBase = 0;

//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                    (reg)->H = ((value) >> 16); } while(0)

#define _HAL_REG16_W(reg, value)    do { (reg)->data = (MS_U16)((value) & 0xFFFF);} while(0)
//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32 _HAL_REG32_R(REG32_MM *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_REG16_R(REG16_MM *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

void HAL_MMFI_SetBank(MS_VIRT u32BankAddr)
{
    _u32MMFIRegBase = u32BankAddr;
    _MFCtrl = (REG_Ctrl_MMFI*)(_u32MMFIRegBase + REG_CTRL_BASE_MMFI0);
    _MFCtrl1 = (REG_Ctrl_MMFI1*)(_u32MMFIRegBase + REG_CTRL_BASE_MMFI1);
}

// ------------------------------------------------------
//   APIs
//-------------------------------------------------------
void HAL_MMFI_PidFlt_Set(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID, MS_U32 u32entype)
{
    _HAL_REG32_W(&(_MFCtrl[u8Eng].PidFlt[u8Idx]), (MS_U32)u16PID | u32entype);
}

void HAL_MMFI_PidFlt_SetPid(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID)
{
    MS_U32 u32data;

    u32data = (_HAL_REG32_R(&(_MFCtrl[u8Eng].PidFlt[u8Idx])) & ~MMFI_PIDFLT_PID_MASK) | (MS_U32)u16PID;
    _HAL_REG32_W(&(_MFCtrl[u8Eng].PidFlt[u8Idx]), u32data);
}

void HAL_MMFI_PidFlt_Enable(MS_U8 u8Eng, MS_U8 u8Idx, MS_U32 u32entype, MS_BOOL benable)
{
    MS_U32 u32data;

    if(!u32entype)
    {
        printf("[MMFI_ERR][%s][%d] Wrong filter destination !!!\n", __FUNCTION__, __LINE__);
        return;
    }

    u32data = _HAL_REG32_R(&(_MFCtrl[u8Eng].PidFlt[u8Idx])) & ~MMFI_PIDFLT_EN_MASK;

    if(benable)
        u32data |= u32entype;

    _HAL_REG32_W(&(_MFCtrl[u8Eng].PidFlt[u8Idx]), u32data);
}

void HAL_MMFI_PidFlt_Reset(MS_U8 u8Eng, MS_U8 u8Idx)
{
    _HAL_REG32_W(&(_MFCtrl[u8Eng].PidFlt[u8Idx]), MMFI_PID_NULL);
}

void HAL_MMFI_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_U32 u32Addr)
{
    MS_U8 u8MiuSel = 0;
    MS_PHY phyMiuOffsetMMFiBuf = 0;
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetMMFiBuf, u32Addr);

    _HAL_REG32_W(&(_MFCtrl[u8Eng].FileIn_RAddr), phyMiuOffsetMMFiBuf);
}

void HAL_MMFI_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U32 u32len)
{
    _HAL_REG32_W(&(_MFCtrl[u8Eng].FileIn_RNum), u32len);
}

void HAL_MMFI_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U16 u16ctrl, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_MFCtrl[u8Eng].FileIn_Ctrl), _HAL_REG16_R(&(_MFCtrl[u8Eng].FileIn_Ctrl)) | u16ctrl);
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl[u8Eng].FileIn_Ctrl), _HAL_REG16_R(&(_MFCtrl[u8Eng].FileIn_Ctrl)) & ~u16ctrl);
    }
}

MS_U16 HAL_MMFI_Get_Filein_Ctrl(MS_U8 u8Eng)
{
    return (_HAL_REG16_R(&(_MFCtrl[u8Eng].FileIn_Ctrl)) & MMFI_FILEIN_CTRL_MASK);
}

MS_BOOL HAL_MMFI_Filein_Pause(MS_U8 u8Eng)
{
    _HAL_REG16_W(&(_MFCtrl1->CFG2[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG2[u8Eng])) | MMFI_CFG2_FILEIN_PAUSE);

    return TRUE;
}

MS_BOOL HAL_MMFI_Filein_Resume(MS_U8 u8Eng)
{
    _HAL_REG16_W(&(_MFCtrl1->CFG2[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG2[u8Eng])) & ~MMFI_CFG2_FILEIN_PAUSE);

    return TRUE;
}

void HAL_MMFI_Set_FileinTimer(MS_U8 u8Eng, MS_U8 u8timer)
{
    MS_U16 u16data = (_HAL_REG16_R(&(_MFCtrl[u8Eng].FileIn_Ctrl)) & ~MMFI_FILEIN_TIMER_MASK) | ((MS_U16)(u8timer&0xFF) << MMFI_FILEIN_TIMER_SHIFT);
    _HAL_REG16_W(&(_MFCtrl[u8Eng].FileIn_Ctrl), u16data);
}

MS_U32 HAL_MMFI_Get_Filein_WriteAddr(MS_U8 u8Eng)
{
    MS_U32 u32addr = 0;

    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_RADDR_READ, FALSE);
    u32addr = _HAL_REG32_R(&(_MFCtrl[u8Eng].RAddr));
    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_RADDR_READ, TRUE);

    return u32addr;
}

MS_U16 HAL_MMFI_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng)
{
    return (_HAL_REG16_R(&(_MFCtrl[u8Eng].CmdQSts)) & MMFI_CMDQSTS_WRCNT_MASK);
}

MS_BOOL HAL_MMFI_CmdQ_FIFO_IsFull(MS_U8 u8Eng)
{
    return (MS_BOOL)(_HAL_REG16_R(&(_MFCtrl[u8Eng].CmdQSts)) & MMFI_CMDQSTS_FIFO_FULL);
}

MS_BOOL HAL_MMFI_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng)
{
    return (MS_BOOL)(_HAL_REG16_R(&(_MFCtrl[u8Eng].CmdQSts)) & MMFI_CMDQSTS_FIFO_EMPTY);
}

MS_U16 HAL_MMFI_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng)
{
    return ((_HAL_REG16_R(&(_MFCtrl[u8Eng].CmdQSts)) & MMFI_CMDQSTS_FIFO_WRLEVEL_MASK) >> MMFI_CMDQSTS_FIFO_WRLEVEL_SHIFT);
}

void HAL_MMFI_Cfg_Enable(MS_U8 u8Eng, MS_U32 u32CfgItem, MS_BOOL benable)
{
    if(benable)
    {
        _HAL_REG32_W(&(_MFCtrl[u8Eng].Cfg), (_HAL_REG32_R(&(_MFCtrl[u8Eng].Cfg)) | u32CfgItem));
    }
    else
    {
        _HAL_REG32_W(&(_MFCtrl[u8Eng].Cfg), (_HAL_REG32_R(&(_MFCtrl[u8Eng].Cfg)) & ~u32CfgItem));
    }
}

void HAL_MMFI_Cfg_Set(MS_U8 u8Eng, MS_U32 u32CfglItem)
{
    _HAL_REG32_W(&(_MFCtrl[u8Eng].Cfg), u32CfglItem);
}

MS_U32 HAL_MMFI_Cfg_Get(MS_U8 u8Eng)
{
    return (_HAL_REG32_R(&(_MFCtrl[u8Eng].Cfg)));
}

void HAL_MMFI_Cfg2_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable)
{
    if(benable)
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG2[u8Eng]), (_HAL_REG16_R(&(_MFCtrl1->CFG2[u8Eng])) | u16CfgItem));
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG2[u8Eng]), (_HAL_REG16_R(&(_MFCtrl1->CFG2[u8Eng])) & ~u16CfgItem));
    }
}

void HAL_MMFI_Cfg2_Set(MS_U8 u8Eng, MS_U16 u16CfglItem)
{
    _HAL_REG16_W(&(_MFCtrl1->CFG2[u8Eng]), u16CfglItem);
}

MS_U16 HAL_MMFI_Cfg2_Get(MS_U8 u8Eng)
{
    return (_HAL_REG16_R(&(_MFCtrl1->CFG2[u8Eng])));
}

void HAL_MMFI_Cfg3_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable)
{
    if(benable)
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), (_HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) | u16CfgItem));
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), (_HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) & ~u16CfgItem));
    }
}

void HAL_MMFI_Cfg3_Set(MS_U8 u8Eng, MS_U16 u16CfglItem)
{
    _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), u16CfglItem);
}

MS_U16 HAL_MMFI_Cfg3_Get(MS_U8 u8Eng)
{
    return (_HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])));
}

void HAL_MMFI_Get_TsHeaderInfo(MS_U8 u8Eng, MS_U32 *pu32header)
{
    *pu32header = _HAL_REG32_R(&(_MFCtrl[u8Eng].TsHeader));
}

void HAL_MMFI_Get_APid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    u16temp = (MS_U16)_HAL_REG16_R(&(_MFCtrl[u8Eng].APid_Status));

    *pu16pid = u16temp & MMFI_APID_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_APID_CHANGE);
}

void HAL_MMFI_Get_APidB_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    u16temp = _HAL_REG16_R(&(_MFCtrl[u8Eng].APidB_Status));

    *pu16pid = u16temp & MMFI_APIDB_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_APIDB_CHANGE);
}

void HAL_MMFI_Get_APidC_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    REG16_MM *reg;
    MS_U16 u16temp;

    reg = (u8Eng == 0)? &(_MFCtrl1->APidC_Status_MMFI0) : &(_MFCtrl1->APidC_Status_MMFI1);
    u16temp = _HAL_REG16_R(reg);

    *pu16pid = u16temp & MMFI_APIDC_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_APIDC_CHANGE);
}

void HAL_MMFI_Get_APidD_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    REG16_MM *reg;
    MS_U16 u16temp;

    reg = (u8Eng == 0)? &(_MFCtrl1->APidD_Status_MMFI0) : &(_MFCtrl1->APidD_Status_MMFI1);
    u16temp = _HAL_REG16_R(reg);

    *pu16pid = u16temp & MMFI_APIDD_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_APIDD_CHANGE);
}

void HAL_MMFI_Get_VPid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    u16temp = _HAL_REG16_R(&(_MFCtrl[u8Eng].VPID_Status));

    *pu16pid = u16temp & MMFI_VPID_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_VPID_CHANGE);
}

void HAL_MMFI_Get_VPid3D_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    u16temp = _HAL_REG16_R(&(_MFCtrl[u8Eng].VPID3D_Status));

    *pu16pid = u16temp & MMFI_VPID3D_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_VPID3D_CHANGE);
}

void HAL_MMFI_Get_VPid3_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    REG16_MM *reg;
    MS_U16 u16temp;

    reg = (u8Eng == 0)? &(_MFCtrl1->VPid3_Status_MMFI0) : &(_MFCtrl1->VPid3_Status_MMFI1);
    u16temp = _HAL_REG16_R(reg);

    *pu16pid = u16temp & MMFI_VPID3_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_VPID3_CHANGE);
}

void HAL_MMFI_Get_VPid4_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    REG16_MM *reg;
    MS_U16 u16temp;

    reg = (u8Eng == 0)? &(_MFCtrl1->VPid4_Status_MMFI0) : &(_MFCtrl1->VPid4_Status_MMFI1);
    u16temp = _HAL_REG16_R(reg);

    *pu16pid = u16temp & MMFI_VPID4_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_VPID4_CHANGE);
}

void HAL_MMFI_LPcr2_Set(MS_U8 u8Eng, MS_U32 u32lpcr2)
{
    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_LPCR2_WLD, TRUE);
    _HAL_REG32_W(&(_MFCtrl[u8Eng].LPcr2_Buf), u32lpcr2);
    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_LPCR2_WLD, FALSE);
}

MS_U32 HAL_MMFI_LPcr2_Get(MS_U8 u8Eng)
{
    MS_U32 u32value;

    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_LPCR2_LD, TRUE);
    u32value = _HAL_REG32_R(&(_MFCtrl[u8Eng].LPcr2_Buf));
    HAL_MMFI_Cfg_Enable(u8Eng, MMFI_CFG_LPCR2_LD, FALSE);

    return u32value;
}

MS_U32 HAL_MMFI_TimeStamp_Get(MS_U8 u8Eng)
{
    return _HAL_REG32_R(&(_MFCtrl[u8Eng].TimeStamp_FIn));
}

void HAL_MMFI_PktChkSize_Set(MS_U8 u8Eng, MS_U16 u16size)
{
    _HAL_REG16_W(&(_MFCtrl[u8Eng].PktChkSize), (_HAL_REG16_R(&(_MFCtrl[u8Eng].PktChkSize)) & ~MMFI_PKTCHK_SIZE_MASK) | u16size);
}

void HAL_MMFI_MOBF_Set_FileinKey(MS_U8 u8Eng, MS_U16 u16KeyIndex)
{
    _HAL_REG16_W(&(_MFCtrl[u8Eng].MOBFKey), u16KeyIndex);
}

void HAL_MMFI_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable)
{
    MS_U16 u16data = _HAL_REG16_R(&(_MFCtrl[u8Eng].FileIn_Ctrl)) & ~MMFI_FILEIN_CTRL_MOBF_EN;

    if(bEnable)
    {
        u16data |= MMFI_FILEIN_CTRL_MOBF_EN;
    }

    _HAL_REG16_W(&(_MFCtrl[u8Eng].FileIn_Ctrl), u16data);
}

void HAL_MMFI_AU_RemoveDupPkt(MS_U8 u8Eng, MS_BOOL bEnable)
{
    MS_U32 u32data = _HAL_REG32_R(&(_MFCtrl[u8Eng].Cfg)) & ~MMFI_CFG_DUP_PKT_SKIP;

    if(bEnable)
        u32data |= MMFI_CFG_DUP_PKT_SKIP;

    _HAL_REG32_W(&(_MFCtrl[u8Eng].Cfg), u32data);
}

//
// General API
//
void HAL_MMFI_Reset(void)
{
    _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) &~ MMFI_SW_RSTZ_MMFILEIN_DISABLE));
    _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) | MMFI_SW_RSTZ_MMFILEIN_DISABLE));
}

void HAL_MMFI_Reset_SubItem(MS_U16 u16RstItem)
{
    _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) | u16RstItem));
    _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) & ~u16RstItem));
}

void HAL_MMFI_Reset_All(void)
{
    MS_U16 u16data = _HAL_REG16_R(&(_MFCtrl[0].SWRst)) & ~MMFI_SWRST_MASK;

    _HAL_REG16_W(&(_MFCtrl[0].SWRst), u16data | MMFI_RST_ALL);
    _HAL_REG16_W(&(_MFCtrl[0].SWRst), (u16data & ~MMFI_SWRST_MASK) | MMFI_SW_RSTZ_MMFILEIN_DISABLE);
}

void HAL_MMFI_HWInt_Enable(MS_BOOL benable, MS_U16 u16init)
{
    MS_U16 u16data = _HAL_REG16_R(&(_MFCtrl[0].HWInt)) & ~u16init;

    if(benable)
    {
        _HAL_REG16_W(&(_MFCtrl[0].HWInt), (u16data | u16init));
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl[0].HWInt), u16data);
    }
}

void HAL_MMFI_HWInt_Clear(MS_U16 u16Int)
{
    _HAL_REG16_W(&(_MFCtrl[0].HWInt), (_HAL_REG16_R(&(_MFCtrl[0].HWInt)) & ~u16Int));
}

MS_U32 HAL_MMFI_HWInt_Status(void)
{
    return (MS_U32)(_HAL_REG16_R(&(_MFCtrl[0].HWInt)) & MMFI_HWINT_STS_MASK);
}

void HAL_MMFI_TimeStampClk_Set(MS_U8 u8Eng, MS_U32 u32ClkSrc)
{
    MS_U16 u16Mask = (u8Eng)? MMFI_RST_LPCR_27M_EN_MMFI1 : MMFI_RST_LPCR_27M_EN_MMFI0;

    if(u32ClkSrc == 0x0UL) // 90k
    {
        _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) & ~u16Mask));
    }
    else                   // 27M
    {
        _HAL_REG16_W(&(_MFCtrl[0].SWRst), (_HAL_REG16_R(&(_MFCtrl[0].SWRst)) | u16Mask));
    }
}

MS_BOOL HAL_MMFI_RVU_Enable(MS_U8 u8Eng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) | MMFI_CFG3_RVU_EN);
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) & ~MMFI_CFG3_RVU_EN);
    }

    return TRUE;
}

MS_BOOL HAL_MMFI_RVU_Timestamp_Enable(MS_U8 u8Eng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) | MMFI_CFG3_RVU_TIMESTAMP_EN);
    }
    else
    {
        _HAL_REG16_W(&(_MFCtrl1->CFG3[u8Eng]), _HAL_REG16_R(&(_MFCtrl1->CFG3[u8Eng])) & ~MMFI_CFG3_RVU_TIMESTAMP_EN);
    }

    return TRUE;
}

// Not implement
void HAL_MMFI_SaveRegs(void)
{

}

// Not implement
void HAL_MMFI_RestoreRegs(void)
{

}

