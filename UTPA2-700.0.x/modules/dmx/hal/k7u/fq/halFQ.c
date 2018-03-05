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
////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halFQ.c
// @brief  FQ HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "regFQ.h"
#include "halFQ.h"
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT      _u32RegBase = 0;
REG_FIQ*            _REGFIQ     = NULL;

// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define FQ32_W(reg, value);    { (reg)->L = ((value) & 0x0000FFFF);                          \
                                  (reg)->H = ((value) >> 16);}
#define FQ16_W(reg, value);    {(reg)->data = ((value) & 0x0000FFFF);}

#define MIU_BUS                     4

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
/*static MS_U32 _HAL_REG32_R(REG32_FQ *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}*/

static MS_U16 _HAL_REG16_R(REG16_FQ *reg)
{
    MS_U16     value;
    value = (reg)->data;
    return value;
}

static MS_U32 _HAL_REG32_R(REG32_FQ *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_FQ_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase                 = u32BankAddr;
    _REGFIQ = (REG_FIQ*)(_u32RegBase + FQ_REG_CTRL_BASE);

    return TRUE;
}

void HAL_FQ_PVR_SetBuf(MS_U32 u32FQEng, MS_PHYADDR u32StartAddr, MS_U32 u32BufSize)
{
    MS_U8 u8MiuSel = 0;
    MS_PHY phyMiuOffsetFQBuf = 0;
    _phy_to_miu_offset(u8MiuSel, phyMiuOffsetFQBuf, u32StartAddr);

    MS_PHYADDR u32EndAddr = phyMiuOffsetFQBuf + u32BufSize;
    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_head), MIU_FQ(phyMiuOffsetFQBuf) & FIQ_STR2MI2_ADDR_MASK);
    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_tail), MIU_FQ(u32EndAddr) & FIQ_STR2MI2_ADDR_MASK);
    FQ32_W(&(_REGFIQ[u32FQEng].str2mi_mid), MIU_FQ(phyMiuOffsetFQBuf) & FIQ_STR2MI2_ADDR_MASK);
}

void HAL_FQ_PVR_SetRushAddr(MS_U32 u32FQEng, MS_PHYADDR u32RushAddr)
{
    FQ32_W(&(_REGFIQ[u32FQEng].rush_addr), MIU_FQ(u32RushAddr) & FIQ_STR2MI2_ADDR_MASK);
}

void HAL_FQ_PVR_Start(MS_U32 u32FQEng)
{
    //reset write address
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RESET_WR_PTR));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RESET_WR_PTR));

    //enable string to miu
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
}

void HAL_FQ_PVR_Stop(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_PVR_ENABLE));
}

void HAL_FQ_Read_Enable(MS_U32 u32FQEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fig_config3), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fig_config3)), FIQ_CFGF_STREAM2MI_RD));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fig_config3), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fig_config3)), FIQ_CFGF_STREAM2MI_RD));
    }
}

void HAL_FQ_BurstLen(MS_U32 u32FQEng, MS_BOOL bRead, MS_U16 u16BurstLen)
{
    REG16_FQ    *Reg = NULL;

    if(bRead)
    {
        Reg = &_REGFIQ[u32FQEng].Reg_fiq_config1;
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIG_CFGB_READ_BURST_LEN_MASK) | (u16BurstLen << FIG_CFGB_READ_BURST_LEN_SHIFT));
    }
    else
    {
        Reg = &_REGFIQ[u32FQEng].Reg_fiq_config0;
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIQ_CFG0_BURST_LEN_MASK) | (u16BurstLen << FIQ_CFG0_BURST_LEN_SHIFT));
    }
}

void HAL_FQ_Rush_Enable(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RUSH_ENABLE));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_RUSH_ENABLE));
}

void HAL_FQ_Bypass(MS_U32 u32FQEng, MS_U8 u8Bypass)
{
    if(u8Bypass)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_FIQ_BYPASS));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_FIQ_BYPASS));
    }
}

void HAL_FQ_BypassFilein(MS_U32 u32FQEng, MS_BOOL bBypass)
{
    if(bBypass)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIG_CFGB_REG_BYPASS_FILEIN_TO_FIQ));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIG_CFGB_REG_BYPASS_FILEIN_TO_FIQ));
    }
}

void HAL_FQ_SWReset(MS_U32 u32FQEng, MS_U8 u8Reset)
{
    if(u8Reset)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_SW_RSTZ));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_SW_RSTZ));
    }
}

void HAL_FQ_AddrMode(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
    if(u8AddrMode)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_ADDR_MODE));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_ADDR_MODE));
    }
}

MS_U32 HAL_FQ_GetRead(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));

    return (_HAL_REG32_R(&(_REGFIQ[u32FQEng].Fiq2mi2_radr_r)) << MIU_BUS);
}

MS_U32 HAL_FQ_GetWrite(MS_U32 u32FQEng)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config0), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config0)), FIQ_CFG0_LOAD_WR_PTR));

    return (_HAL_REG32_R(&(_REGFIQ[u32FQEng].str2mi2_wadr_r)) << MIU_BUS);
}

/*
MS_U32 HAL_FQ_GetPktAddrOffset(MS_U32 u32FQEng)
{
    return REG32_R(&(_REGFIQ[u32FQEng].pkt_addr_offset)) << MIU_BUS;
}
*/

void HAL_FQ_SkipRushData(MS_U32 u32FQEng, MS_U16 u16SkipPath)
{
    //@NOTE: K7U don't have to implement (HAL_TSP_HwPatch @ halTSP.c)
}

void HAL_FQ_INT_Enable(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_int),  _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_int)),  u16Mask & FIQ_CFG10_INT_ENABLE_MASK));
}

void HAL_FQ_INT_Disable(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_int), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_int)), u16Mask & FIQ_CFG10_INT_ENABLE_MASK));
}

MS_U16 HAL_FQ_INT_GetHW(MS_U32 u32FQEng)
{
    return _HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_int)) & FIQ_CFG10_INT_STATUS_MASK;
}

void HAL_FQ_INT_ClrHW(MS_U32 u32FQEng, MS_U16 u16Mask)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_int), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_int)), u16Mask & FIQ_CFG10_INT_STATUS_MASK));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_int), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_int)), u16Mask & FIQ_CFG10_INT_STATUS_MASK));
}

void HAL_FQ_Timestamp_Sel(MS_U32 u32FQEng, MS_BOOL bSet) //0: 90K , 1: 27M
{
    if(bSet)
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fig_config2), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fig_config2)), FIQ_CFGE_C90K_SEL_27M));
    }
    else
    {
        FQ16_W(&(_REGFIQ[u32FQEng].Reg_fig_config2), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fig_config2)), FIQ_CFGE_C90K_SEL_27M));
    }
}

MS_U32 HAL_FQ_GetPVRTimeStamp(MS_U32 u32FQEng)
{
    MS_U32 u32Timestamp = 0;

    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_LPCR1_LOAD));
    u32Timestamp = _HAL_REG32_R(&(_REGFIQ[u32FQEng].lpcr1));
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_LPCR1_LOAD));

    return u32Timestamp;
}

void HAL_FQ_SetPVRTimeStamp(MS_U32 u32FQEng , MS_U32 u32Stamp)
{
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _SET_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_LPCR1_WLD));
    FQ32_W(&(_REGFIQ[u32FQEng].lpcr1), u32Stamp);
    FQ16_W(&(_REGFIQ[u32FQEng].Reg_fiq_config1), _CLR_(_HAL_REG16_R(&(_REGFIQ[u32FQEng].Reg_fiq_config1)), FIQ_CFGB_LPCR1_WLD));
}

// not implement
void HAL_FQ_SaveRegs(void)
{

}

// not implement
void HAL_FQ_RestoreRegs(void)
{

}

void HAL_FQ_BypassSrcFlt(MS_U32 u32FQEng, MS_BOOL bBypass)
{
    if(bBypass)
    {
        FQ16_W(&_REGFIQ[u32FQEng].Reg_fig_config3, _CLR_(_HAL_REG16_R(&_REGFIQ[u32FQEng].Reg_fig_config3), FIQ_CFGF_SRC_FILTER_EN));
    }
    else
    {
        FQ16_W(&_REGFIQ[u32FQEng].Reg_fig_config3, _SET_(_HAL_REG16_R(&_REGFIQ[u32FQEng].Reg_fig_config3), FIQ_CFGF_SRC_FILTER_EN));
    }
}

void HAL_FQ_SrcFlt_SetSyncByte(MS_U32 u32FQEng, MS_U32 u32SrcFltId, MS_U8 *pu8SyncByte, MS_BOOL bSet)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQEng].Fiq_Src_Filter[u32SrcFltId >> 1];
    MS_U16      u16Mask = (u32SrcFltId & 0x1)? FIQ_SRC_FILTER_SYNC_BYTE_ODD_MASK : FIQ_SRC_FILTER_SYNC_BYTE_EVEN_MASK;
    MS_U16      u16Shift = (u32SrcFltId & 0x1)? FIQ_SRC_FILTER_SYNC_BYTE_ODD_SHIFT : FIQ_SRC_FILTER_SYNC_BYTE_EVEN_SHIFT;

    if(bSet)
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), u16Mask) | (*pu8SyncByte << u16Shift));
    }
    else
    {
        *pu8SyncByte = (MS_U8)((_HAL_REG16_R(Reg) & u16Mask) >> u16Shift);
    }
}

void HAL_FQ_SrcFlt_Enable(MS_U32 u32FQEng, MS_U32 u32SrcFltId, MS_BOOL bEnable)
{
    if(bEnable)
    {
        FQ16_W(&_REGFIQ[u32FQEng].REG_FIQ_28, _SET_(_HAL_REG16_R(&_REGFIQ[u32FQEng].REG_FIQ_28), (FIQ_CFG28_REG_FIQ_SRC_FILTER_EN << u32SrcFltId)));
    }
    else
    {
        FQ16_W(&_REGFIQ[u32FQEng].REG_FIQ_28, _CLR_(_HAL_REG16_R(&_REGFIQ[u32FQEng].REG_FIQ_28), (FIQ_CFG28_REG_FIQ_SRC_FILTER_EN << u32SrcFltId)));
    }
}

void HAL_FQ_Flt_SetPid(MS_U32 u32FQEng, MS_U32 u32FltId, MS_U16 *pu16Pid, MS_BOOL bSet)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQEng].Fiq_Filter[u32FltId];

    if(bSet)
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIQ_FILTER_PID_MASK) | (*pu16Pid << FIQ_FILTER_PID_SHIFT));
    }
    else
    {
        *pu16Pid = (MS_U16)((_HAL_REG16_R(Reg) & FIQ_FILTER_PID_MASK) >> FIQ_FILTER_PID_SHIFT);
    }
}

void HAL_FQ_Flt_SetSyncByte(MS_U32 u32FQEng, MS_U32 u32FltId, MS_U8 *pu8SyncByte, MS_BOOL bSet)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQEng].Fiq_Filter_SyncByte[u32FltId >> 1];
    MS_U16      u16Mask = (u32FltId & 0x1)? FIQ_FILTER_SYNC_BYTE_ODD_MASK : FIQ_FILTER_SYNC_BYTE_EVEN_MASK;
    MS_U16      u16Shift = (u32FltId & 0x1)? FIQ_FILTER_SYNC_BYTE_ODD_SHIFT : FIQ_FILTER_SYNC_BYTE_EVEN_SHIFT;

    if(bSet)
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), u16Mask) | (*pu8SyncByte << u16Shift));
    }
    else
    {
        *pu8SyncByte = (MS_U8)((_HAL_REG16_R(Reg) & u16Mask) >> u16Shift);
    }
}

void HAL_FQ_Flt_Enable(MS_U32 u32FQEng, MS_U32 u32FltId, MS_BOOL bEnable)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQEng].Fiq_Filter[u32FltId];

    if(bEnable)
    {
        FQ16_W(Reg, _SET_(_HAL_REG16_R(Reg), FIQ_FILTER_EN));
    }
    else
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIQ_FILTER_EN));
    }
}

void HAL_FQ_MUX_Src(MS_U32 u32FQMuxEng, MS_U16 *pu16Path, MS_BOOL bSet)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQMuxEng + FQ_MUX_START_ID].Reg_fiq_config1;

    if(bSet)
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIG_CFGB_REG_FIQ_MUX_SRC_MASK) | (*pu16Path << FIG_CFGB_REG_FIQ_MUX_SRC_SHIFT));
    }
    else
    {
        *pu16Path = (MS_U16)((_HAL_REG16_R(Reg) & FIG_CFGB_REG_FIQ_MUX_SRC_MASK) >> FIG_CFGB_REG_FIQ_MUX_SRC_SHIFT);
    }
}

void HAL_FQ_MUX_RushModeEnable(MS_U32 u32FQMuxEng, MS_BOOL bEnable)
{
    REG16_FQ    *Reg = &_REGFIQ[u32FQMuxEng + FQ_MUX_START_ID].Reg_fig_config3;

    if(bEnable)
    {
        FQ16_W(Reg, _SET_(_HAL_REG16_R(Reg), FIQ_CFGF_RUSH_MODE_EN));
    }
    else
    {
        FQ16_W(Reg, _CLR_(_HAL_REG16_R(Reg), FIQ_CFGF_RUSH_MODE_EN));
    }
}