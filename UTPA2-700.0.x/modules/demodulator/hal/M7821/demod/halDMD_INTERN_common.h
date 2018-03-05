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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#ifndef _HAL_DMD_COMMON_H_
#define _HAL_DMD_COMMON_H_

#define MBRegBase   0x112600UL //Demod MailBox
#define VDMcuBase   0x103400UL //DmdMCU51 (40-4F)
#define DMDMcuBase  0x103480UL
#define XDMIU_REG_BASE  0x1d00
#define BACKEND_REG_BASE  0x1f00
#define TOP_REG_BASE  0x2000
#define TDP_REG_BASE  0x2100
#define FDP_REG_BASE  0x2200
#define	ISDBT_FDPE_REG_BASE  0x1600
#define FEC_REG_BASE  0x2300
#define TDF_REG_BASE  0x2800
#define TDFE_REG_BASE  0x2900
#define TDFE2_REG_BASE  0x2a00
#define DTOP_REG_BASE  0x2e00
#define T2TDP_REG_BASE  0x3000
#define T2FDP_REG_BASE  0x3100
#define T2FEC_REG_BASE  0x3300
#define DVBTM_REG_BASE  0x3400
#define T2L1_REG_BASE  0x2b00
#define T2SNR_REG_BASE  0x2c00
#define T2DJB_REG_BASE  0x2d00

#define REG_CMD_CTRL    MBRegBase + 0x1C
#define REG_DTA_CTRL    MBRegBase + 0x1D
#define REG_CMD_ADDR    MBRegBase + 0x1E
#define REG_CMD_DATA    MBRegBase + 0x1F

#define _REG_START      REG_CMD_CTRL
#define _REG_END        REG_CMD_CTRL
#define _REG_DRQ        REG_DTA_CTRL
#define _REG_FSM        REG_CMD_CTRL
#define _REG_ERR        REG_DTA_CTRL

#define _BIT_START      ((BIT_(1)))
#define _BIT_END        ((BIT_(0)))
#define _BIT_DRQ        ((BIT_(0)))
#define _BIT_FSM        ((BIT_(3)))
#define _BIT_ERR        ((BIT_(7)))

typedef struct _s_MDRV_DMD_INTERFACE_FUNCTION
{
    MS_BOOL (*MDrv_DMD_WriteReg)(MS_U32 u32Reg, MS_U8 u8Value);
    MS_BOOL (*MDrv_DMD_ReadReg)(MS_U32 u32Reg, MS_U8 *u8Value);
    MS_BOOL (*MDrv_DMD_WriteRegs)(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length);
    MS_BOOL (*MDrv_DMD_I2C_Channel_Change)(MS_U8 ch_num);
    MS_BOOL (*MDrv_DMD_I2C_Channel_Set)(MS_U8  ch_num);
    float        (*Log10Approx)(float flt_x);
}s_MDRV_DMD_INTERFACE_FUNCTION;



MS_BOOL HAL_DMD_RegInit (void);
MS_U8 HAL_DMD_RIU_ReadByte(MS_U32 u32Addr);
MS_U8 HAL_DMD_RIU_ReadRegBit(MS_U32 u32Addr, MS_U8 u8Mask);
MS_U16 HAL_DMD_RIU_Read2Byte(MS_U32 u32Addr);
void HAL_DMD_RIU_WriteByte(MS_U32 u32Addr, MS_U8 u8Value);
void HAL_DMD_RIU_WriteRegBit(MS_U32 u32Addr, MS_BOOL bEnable, MS_U8 u8Mask);
void HAL_DMD_RIU_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask);
void HAL_DMD_RIU_Write2Byte(MS_U32 u32Addr, MS_U16 u16Value);

//waiting add
MS_BOOL HAL_DMD_IFAGC_RegRead(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);
/*
MS_BOOL HAL_DMD_GetRFLevel(float *fRFPowerDbmResult, float fRFPowerDbm, MS_U8 u8SarValue,
                                                     DMD_RFAGC_SSI *pRfagcSsi, MS_U16 u16RfagcSsi_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_HiRef, MS_U16 u16IfagcSsi_HiRef_Size,
                                                     DMD_IFAGC_SSI *pIfagcSsi_LoRef, MS_U16 u16IfagcSsi_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_LoRef, MS_U16 u16IfagcErr_LoRef_Size,
                                                     DMD_IFAGC_ERR *pIfagcErr_HiRef, MS_U16 u16IfagcErr_HiRef_Size);
*/
void HAL_DMD_GetNordigSSI(float fPrel, MS_U16 *strength);
void HAL_DMD_IFAGC_TS_Tristate(MS_BOOL bEnable);
void HAL_DMD_TS1_Tristate(MS_BOOL bEnable);
MS_BOOL HAL_DMD_TS_GetClockRate(float *fTS_CLK);
void HAL_DMD_RFAGC_Tristate(MS_BOOL bEnable);
void HAL_DMD_IFAGC_Tristate(MS_BOOL bEnable);
MS_BOOL HAL_DMD_ADC_IQ_Switch(MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain);
MS_BOOL HAL_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array);
//float Log10Approx(float flt_x);
MS_U8 HAL_DMD_IIC_ReadByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *pu8Data);
MS_U8 HAL_DMD_IIC_WriteByte(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 u8Data);
MS_U8 HAL_DMD_IIC_WriteBytes(MS_U16 u16SlaveAddr, MS_U32 u32Addr, MS_U8 *u8Data, MS_U8 u8Len);
MS_BOOL HAL_DMD_I2C_Channel_Change(MS_U16 u16SlaveAddr, MS_U8 ch_num);
MS_BOOL HAL_DMD_I2C_Channel_Set(MS_U16 u16SlaveAddr, MS_U8 ch_num);
#endif

