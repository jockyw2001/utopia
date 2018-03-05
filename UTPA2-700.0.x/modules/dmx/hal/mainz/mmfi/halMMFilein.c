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

#include "halMMFilein.h"
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HAL_REG_SAFE_MODE       1                                   // Register protection access between 1 task and 1+ ISR
                                                               
#define MIU_BUS                     4

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl_MMFI* _MFCtrl_AU = NULL;
static REG_Ctrl_MMFI* _MFCtrl_V3D = NULL;

static MS_VIRT      _virtMMFIRegBase = 0;
static MS_PHY       _phyMMFIMiuOffset[2] = {0, 0};

//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)                do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                                        (reg)->H = ((value) >> 16); } while(0)

#define TSP_TSP3_REG(addr)       (*((volatile MS_U32*)(_virtMMFIRegBase + 0xC1400UL + ((addr)<<2UL))))
    #define REG_TSP3_INIT_TIMESTAMP             0x25UL
        #define REG_TSP3_INIT_MMFI0_TIMESTAMP   2UL
        #define REG_TSP3_INIT_MMFI1_TIMESTAMP   4UL
    #define REG_TSP3_TIMESTAMP_MMFI0            0x28UL
    #define REG_TSP3_TIMESTAMP_MMFI1            0x2AUL

                                  
//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32 _HAL_REG32_R(REG32_M *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_PHY _HAL_MMFI_MIU_OFFSET(MS_PHY Phyaddr)
{
    #ifdef HAL_MIU2_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU2_BASE)
        return (MS_PHY)HAL_MIU2_BASE;
    else
    #endif  //HAL_MIU2_BASE
    #ifdef HAL_MIU1_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU1_BASE)
        return (MS_PHY)HAL_MIU1_BASE;
    else
    #endif //HAL_MIU1_BASE
        return (MS_PHY)HAL_MIU0_BASE;
}          

void HAL_MMFI_SetBank(MS_VIRT virtBankAddr)
{
    _virtMMFIRegBase = virtBankAddr;
    _MFCtrl_AU = (REG_Ctrl_MMFI*)(_virtMMFIRegBase+ REG_CTRL_BASE_MMFI);
    _MFCtrl_V3D= (REG_Ctrl_MMFI*)(_virtMMFIRegBase+ REG_CTRL_BASE_MMFI_V3D);
}

// ------------------------------------------------------
//   Audio APIs
//-------------------------------------------------------
void HAL_MMFI_AudPidFlt_Set(MS_U8 u8Idx, MS_U16 u16PID, MS_U16 u16entype)
{
    MS_U32 u32shift = ((u8Idx == 0) ? MMFI_PIDFLT_A_SHIFT : MMFI_PIDFLT_B_SHIFT);
    MS_U32 u32mask = ((u8Idx == 0) ? MMFI_PIDFLT_A_MASK : MMFI_PIDFLT_B_MASK);
    MS_U32 u32data;

    u32data = ((MS_U32)(u16PID|u16entype)) << u32shift;
    u32data |=(_HAL_REG32_R(&_MFCtrl_AU->PidFlt) & ~u32mask);           
    _HAL_REG32_W(&(_MFCtrl_AU->PidFlt), u32data);   
}

void   HAL_MMFI_AudPidFlt_SetPid(MS_U8 u8Idx, MS_U16 u16PID)
{
    MS_U32 u32shift = ((u8Idx == 0) ? MMFI_PIDFLT_A_SHIFT : MMFI_PIDFLT_B_SHIFT);
    MS_U32 u32mask = MMFI_PIDFLT_PID_MASK << u32shift;
    MS_U32 u32data;

    u32data = ((MS_U32)u16PID) << u32shift;
    u32data |= (_HAL_REG32_R(&_MFCtrl_AU->PidFlt) & ~u32mask);  
    _HAL_REG32_W(&(_MFCtrl_AU->PidFlt), u32data);
}

void   HAL_MMFI_AudPidFlt_Enable(MS_U8 u8Idx, MS_U16 u16entype, MS_BOOL benable)
{
    MS_U32 u32shift = ((u8Idx == 0) ? MMFI_PIDFLT_A_SHIFT : MMFI_PIDFLT_B_SHIFT);
    MS_U32 u32mask = MMFI_PIDFLT_EN_MASK << u32shift;
    MS_U32 u32data;

    u32data = _HAL_REG32_R(&(_MFCtrl_AU->PidFlt)) & ~u32mask;
                    
    if(benable) 
        u32data |= ((MS_U32)u16entype << u32shift);
    
    _HAL_REG32_W(&(_MFCtrl_AU->PidFlt), u32data);
}

void   HAL_MMFI_AudPidFlt_Reset(MS_U8 u8Idx)
{
    MS_U32 u32shift = ((u8Idx == 0) ? MMFI_PIDFLT_A_SHIFT : MMFI_PIDFLT_B_SHIFT);
    MS_U32 u32mask = ((u8Idx == 0) ? MMFI_PIDFLT_A_MASK : MMFI_PIDFLT_B_MASK);
    MS_U32 u32data;

    u32data = (_HAL_REG32_R(&(_MFCtrl_AU->PidFlt)) & ~u32mask) | 
                    (((MS_U32)MMFI_PID_NULL) << u32shift);  
    _HAL_REG32_W(&(_MFCtrl_AU->PidFlt), u32data);
}

void HAL_MMFI_AU_Set_Filein_ReadAddr(MS_PHY phyAddr)
{   
    _phyMMFIMiuOffset[0] = _HAL_MMFI_MIU_OFFSET(phyAddr);
    
    _HAL_REG32_W(&_MFCtrl_AU->FileIn_RAddr, (MS_U32)(phyAddr-_phyMMFIMiuOffset[0]));
}

void HAL_MMFI_AU_Set_Filein_ReadLen(MS_U32 u32len)
{
    _HAL_REG32_W(&_MFCtrl_AU->FileIn_RNum, u32len);
}

void   HAL_MMFI_AU_Set_Filein_Ctrl(MS_U32 u32ctrl)
{
    MS_U32 u32data;
    
    u32data = (_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & ~MMFI_FILEIN_CTRL_MASK) | u32ctrl;
    _HAL_REG32_W(&_MFCtrl_AU->Ctrl_CmdQSts, u32data);
}

MS_U32 HAL_MMFI_AU_Get_Filein_Ctrl(void)
{
    return (_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & MMFI_FILEIN_CTRL_MASK); 
}

void   HAL_MMFI_AU_Set_FileinTimer(MS_U8 u8timer)
{
    MS_U32 u32data; 

    u32data = (_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & ~MMFI_TIMER_MASK) | ((MS_U32)(u8timer & 0xFF) << MMFI_TIMER_SHIFT);        
    _HAL_REG32_W(&_MFCtrl_AU->Ctrl_CmdQSts, u32data);       
}

MS_U32   HAL_MMFI_AU_CmdQ_FIFO_Get_WRCnt(void)
{
    MS_U32 u32data = (_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & MMFI_CMQ_WR_CNT_MASK) >> MMFI_CMQ_STATUS_SHIFT;
        
    return u32data;
}

MS_BOOL HAL_MMFI_AU_CmdQ_FIFO_IsFull(void)
{
    return (MS_BOOL)(_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & MMFI_CMQ_STATUS_FIFO_FULL);
}

MS_BOOL HAL_MMFI_AU_CmdQ_FIFO_IsEmpty(void)
{
    return (MS_BOOL)(_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & MMFI_CMQ_STATUS_FIFO_EMPTY);
}

MS_U8   HAL_MMFI_AU_CmdQ_FIFO_Get_WRLevel(void)
{
    MS_U32 u32data = (_HAL_REG32_R(&_MFCtrl_AU->Ctrl_CmdQSts) & MMFI_CMQ_STATU_WR_LEVEL_MASK) >> MMFI_CMQ_STATU_WR_LEVEL_SHIFT;
        
    return ((MS_U8)u32data);
}

void HAL_MMFI_AU_Cfg_Enable(MS_U32 u32CfgItem, MS_BOOL benable)
{
    if(benable)
    {
        _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&(_MFCtrl_AU->Cfg)) | u32CfgItem));
    }
    else
    {
        _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&(_MFCtrl_AU->Cfg)) & ~u32CfgItem));
    }       
}

void   HAL_MMFI_AU_Cfg_Set(MS_U32 u32CfglItem)
{
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, u32CfglItem);
}

MS_U32 HAL_MMFI_AU_Cfg_Get(void)
{
    return (_HAL_REG32_R(&_MFCtrl_AU->Cfg));    
}

void HAL_MMFI_AU_Get_TsHeaderInfo(MS_U32 *pu32header)
{
    *pu32header = _HAL_REG32_R(&_MFCtrl_AU->TsHeader);
}

void   HAL_MMFI_AU_Get_APid_Status(MS_U8 u8idx, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    if(u8idx == 0)
    {
        u16temp = (MS_U16)_HAL_REG32_R(&_MFCtrl_AU->Pid_Status);
    }
    else
    {
        u16temp = (MS_U16)(_HAL_REG32_R(&_MFCtrl_AU->Pid_Status) >> MMFI_PIFSTS_B_SHIFT);
    }

    *pu16pid = u16temp & MMFI_PID_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_PID_CHANGE);
}

void   HAL_MMFI_AU_LPcr2_Set(MS_U32 u32lpcr2)
{
    _HAL_REG32_W(&_MFCtrl_AU->LPcr2_Buf, u32lpcr2);
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) | MMFI_LPCR2_WLD));  
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) & ~MMFI_LPCR2_WLD)); 
}

MS_U32 HAL_MMFI_AU_LPcr2_Get(void)
{
    MS_U32 u32temp;
    
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) | MMFI_LPCR2_LOAD));
    u32temp = _HAL_REG32_R(&_MFCtrl_AU->LPcr2_Buf);
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) & ~MMFI_LPCR2_LOAD));

    return u32temp;
}

MS_U32 HAL_MMFI_AU_TimeStamp_Get(void)
{
    return _HAL_REG32_R(&_MFCtrl_AU->TimeStamp_FIn);
}

void HAL_MMFI_AU_PktChkSize_Set(MS_U8 u8size)
{
    MS_U32 u32temp;
    
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) | MMFI_ALT_TS_SIZE));

    u32temp = ((_HAL_REG32_R(&_MFCtrl_AU->PktChkSize) & ~MMFI_PKTCHK_SIZE_MASK) | ((MS_U32)(u8size & 0xFF)));
    _HAL_REG32_W(&_MFCtrl_AU->PktChkSize, u32temp);
}

void HAL_MMFI_AU_RemoveDupPkt(MS_BOOL bEnable)
{
    _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) | MMFI_DUP_PKT_SKIP));
}

MS_BOOL HAL_MMFI_AU_MOBF_Set_FileinKey(MS_U32 u32Key)
{
    MS_U32 u32temp; 

    u32temp = (_HAL_REG32_R(&_MFCtrl_AU->PktChkSize) & ~MMFI_MOBFKEY_MASK) | ((u32Key << MMFI_MOBFKEY_SHIFT) & MMFI_MOBFKEY_MASK);
    _HAL_REG32_W(&_MFCtrl_AU->PktChkSize, u32temp);

    return TRUE;
}

MS_BOOL HAL_MMFI_AU_MOBF_Enable(MS_BOOL bEnable)
{   
    return FALSE;
}

MS_BOOL HAL_MMFI_AU_MOBF_SetLevel(MS_U8 u8level)
{
    return FALSE;
}

// ------------------------------------------------------
//   Video 3D APIs
//-------------------------------------------------------
void HAL_MMFI_VD3DPidFlt_Set(MS_U8 u8Idx, MS_U16 u16PID, MS_U16 u16entype)
{   
    MS_U32 u32data;

    u32data = (MS_U32)(u16PID|u16entype);
    u32data |=(_HAL_REG32_R(&_MFCtrl_V3D->PidFlt) & ~MMFI_PIDFLT_A_MASK);           
    _HAL_REG32_W(&(_MFCtrl_V3D->PidFlt), u32data);  
}

void   HAL_MMFI_VD3DPidFlt_SetPid(MS_U8 u8Idx, MS_U16 u16PID)
{   
    MS_U32 u32data;

    u32data = (MS_U32)u16PID;
    u32data |= (_HAL_REG32_R(&_MFCtrl_V3D->PidFlt) & ~MMFI_PIDFLT_PID_MASK);    
    _HAL_REG32_W(&(_MFCtrl_V3D->PidFlt), u32data);
}

void   HAL_MMFI_VD3DPidFlt_Enable(MS_U8 u8Idx, MS_U16 u16entype, MS_BOOL benable)
{
    MS_U32 u32data;
    
    u32data = _HAL_REG32_R(&(_MFCtrl_V3D->PidFlt)) & ~MMFI_PIDFLT_EN_MASK;
                        
    if(benable) 
        u32data |= ((MS_U32)u16entype);
        
    _HAL_REG32_W(&(_MFCtrl_V3D->PidFlt), u32data);
}

void   HAL_MMFI_VD3DPidFlt_Reset(MS_U8 u8Idx)
{   
    MS_U32 u32data;

    u32data = (_HAL_REG32_R(&(_MFCtrl_V3D->PidFlt)) & ~MMFI_PIDFLT_A_MASK) | ((MS_U32)MMFI_PID_NULL);   
    _HAL_REG32_W(&(_MFCtrl_V3D->PidFlt), u32data);
}

void HAL_MMFI_V3D_Set_Filein_ReadAddr(MS_PHY phyAddr)
{    
    _phyMMFIMiuOffset[1] = _HAL_MMFI_MIU_OFFSET(phyAddr);
    
    _HAL_REG32_W(&_MFCtrl_V3D->FileIn_RAddr, (MS_U32)(phyAddr-_phyMMFIMiuOffset[1]));
}

void HAL_MMFI_V3D_Set_Filein_ReadLen(MS_U32 u32len)
{
    _HAL_REG32_W(&_MFCtrl_V3D->FileIn_RNum, u32len);
}

void   HAL_MMFI_V3D_Set_Filein_Ctrl(MS_U32 u32ctrl)
{   
    MS_U32 u32data;
    
    u32data = (_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & ~MMFI_FILEIN_CTRL_MASK) | u32ctrl;
    _HAL_REG32_W(&_MFCtrl_V3D->Ctrl_CmdQSts, u32data);       
}

MS_U32 HAL_MMFI_V3D_Get_Filein_Ctrl(void)
{
    return (_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & MMFI_FILEIN_CTRL_MASK); 
}

void   HAL_MMFI_V3D_Set_FileinTimer(MS_U8 u8timer)
{
    MS_U32 u32data; 

    u32data = (_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & ~MMFI_TIMER_MASK) | ((MS_U32)(u8timer & 0xFF) << MMFI_TIMER_SHIFT);       
    _HAL_REG32_W(&_MFCtrl_V3D->Ctrl_CmdQSts, u32data);  
}

MS_U8   HAL_MMFI_V3D_CmdQ_FIFO_Get_WRCnt(void)
{
    MS_U32 u32data = (_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & MMFI_CMQ_WR_CNT_MASK) >> MMFI_CMQ_STATUS_SHIFT;
        
    return u32data;
}

MS_BOOL HAL_MMFI_V3D_CmdQ_FIFO_IsFull(void)
{
    return (MS_BOOL)(_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & MMFI_CMQ_STATUS_FIFO_FULL);
}

MS_BOOL HAL_MMFI_V3D_CmdQ_FIFO_IsEmpty(void)
{
    return (MS_BOOL)(_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & MMFI_CMQ_STATUS_FIFO_EMPTY);
}

MS_U8   HAL_MMFI_V3D_CmdQ_FIFO_Get_WRLevel(void)
{
    MS_U32 u32data = (_HAL_REG32_R(&_MFCtrl_V3D->Ctrl_CmdQSts) & MMFI_CMQ_STATU_WR_LEVEL_MASK) >> MMFI_CMQ_STATU_WR_LEVEL_SHIFT;
        
    return ((MS_U8)u32data);
}

void HAL_MMFI_V3D_Cfg_Enable(MS_U32 u32CfgItem, MS_BOOL benable)
{
    if(benable)
    {
        _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&(_MFCtrl_V3D->Cfg)) | u32CfgItem));
    }
    else
    {
        _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&(_MFCtrl_V3D->Cfg)) & ~u32CfgItem));
    }       
}

void   HAL_MMFI_V3D_Cfg_Set(MS_U32 u32CfglItem)
{
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, u32CfglItem);
}

MS_U32 HAL_MMFI_V3D_Cfg_Get(void)
{
    return (_HAL_REG32_R(&_MFCtrl_V3D->Cfg));   
}

void HAL_MMFI_V3D_Get_TsHeaderInfo(MS_U32 *pu32header)
{
    *pu32header = _HAL_REG32_R(&_MFCtrl_V3D->TsHeader);
}

void   HAL_MMFI_V3D_Get_VPid_Status(MS_U8 u8idx, MS_U16 *pu16pid, MS_BOOL *pbchanged)
{
    MS_U16 u16temp;

    u16temp = (MS_U16)_HAL_REG32_R(&_MFCtrl_V3D->Pid_Status);   
    *pu16pid = u16temp & MMFI_PID_MATCHED_MASK;
    *pbchanged = (MS_BOOL)(u16temp & MMFI_PID_CHANGE);
}

void   HAL_MMFI_V3D_LPcr2_Set(MS_U32 u32lpcr2)
{   
    _HAL_REG32_W(&_MFCtrl_V3D->LPcr2_Buf, u32lpcr2);
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) | MMFI_LPCR2_WLD));    
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) & ~MMFI_LPCR2_WLD));   
}

MS_U32 HAL_MMFI_V3D_LPcr2_Get(void)
{
    MS_U32 u32temp;
    
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) | MMFI_LPCR2_LOAD));
    u32temp = _HAL_REG32_R(&_MFCtrl_V3D->LPcr2_Buf);
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) & ~MMFI_LPCR2_LOAD));

    return u32temp;
}

MS_U32 HAL_MMFI_V3D_TimeStamp_Get(void)
{
    return _HAL_REG32_R(&_MFCtrl_V3D->TimeStamp_FIn);
}

void HAL_MMFI_V3D_PktChkSize_Set(MS_U8 u8size)
{
    MS_U32 u32temp;
    
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) | MMFI_ALT_TS_SIZE));

    u32temp = ((_HAL_REG32_R(&_MFCtrl_V3D->PktChkSize) & ~MMFI_PKTCHK_SIZE_MASK) | ((MS_U32)(u8size & 0xFF)));
    _HAL_REG32_W(&_MFCtrl_V3D->PktChkSize, u32temp);
}

void HAL_MMFI_V3D_RemoveDupPkt(MS_BOOL bEnable)
{
    _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) | MMFI_DUP_PKT_SKIP));
}

MS_BOOL HAL_MMFI_V3D_MOBF_Set_FileinKey(MS_U32 u32Key)
{
    MS_U32 u32temp; 

    u32temp = (_HAL_REG32_R(&_MFCtrl_V3D->PktChkSize) & ~MMFI_MOBFKEY_MASK) | ((u32Key << MMFI_MOBFKEY_SHIFT) & MMFI_MOBFKEY_MASK);
    _HAL_REG32_W(&_MFCtrl_V3D->PktChkSize, u32temp);

    return TRUE;
}

MS_BOOL HAL_MMFI_V3D_MOBF_Enable(MS_BOOL bEnable)
{
    return FALSE;
}

MS_BOOL HAL_MMFI_V3D_MOBF_SetLevel(MS_U8 u8level)
{
    return FALSE;
}

//
// General API
//
void HAL_MMFI_Reset(void)
{   
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) &~ MMFI_SW_RSTZ_MMFILEIN_DISABLE));  
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) | MMFI_SW_RSTZ_MMFILEIN_DISABLE));
}

void HAL_MMFI_Reset_SubItem(MS_U32 u32RstItem)
{
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) | u32RstItem));
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) & ~u32RstItem));  
}

void HAL_MMFI_Reset_All(void)
{   
    MS_U32 u32data = _HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) & ~MMFI_SWRST_MASK; 
        
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, u32data | MMFI_RST_ALL); 
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, u32data | MMFI_SW_RSTZ_MMFILEIN_DISABLE);
}

void HAL_MMFI_HWInt_Enable(MS_BOOL benable, MS_U32 u32init)
{
    MS_U32 u32data = _HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) & ~u32init;
        
    if(benable)
    {
        _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (u32data | u32init));
    }
    else
    {
        _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, u32data);
    }
}

void HAL_MMFI_HWInt_Clear(MS_U32 u32Int)
{   
    _HAL_REG32_W(&_MFCtrl_AU->SWRst_HWInt, (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) & ~u32Int));
}

MS_U32 HAL_MMFI_HWInt_Status(void)
{
    return (_HAL_REG32_R(&_MFCtrl_AU->SWRst_HWInt) & MMFI_HWINT_STS_MASK);
}

MS_BOOL HAL_MMFI_Chk_CmdQResetDone(MS_U32 u32Path)
{
    int ii = 0;
    REG32_M* pReg = NULL;

    if(u32Path == MMFI_RST_CMDQ_AU)
    {
        pReg = &_MFCtrl_AU->Ctrl_CmdQSts;
        _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) | MMFI_WB_FSM_RESET));
    }
    else if(u32Path == MMFI_RST_CMDQ_VD)
    {
        pReg = &_MFCtrl_V3D->Ctrl_CmdQSts;
        _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) | MMFI_WB_FSM_RESET));
    }
    else
    {
        return FALSE;
    }

    for(ii = 0; ii < 100; ii++)
    {
        if(_HAL_REG32_R(pReg) & MMFI_FILEIN_DONE)
            break;
        
        MsOS_DelayTask(1);
    }

    if(u32Path == MMFI_RST_CMDQ_AU)
    {
        _HAL_REG32_W(&_MFCtrl_AU->Cfg, (_HAL_REG32_R(&_MFCtrl_AU->Cfg) & ~MMFI_WB_FSM_RESET));
    }
    else
    {
        _HAL_REG32_W(&_MFCtrl_V3D->Cfg, (_HAL_REG32_R(&_MFCtrl_V3D->Cfg) & ~MMFI_WB_FSM_RESET));
    }

    if(ii == 100)
    {
        printf("%s, wait fine in reset timeout\n", __FUNCTION__);
        return FALSE;
    }

    return TRUE;
}



