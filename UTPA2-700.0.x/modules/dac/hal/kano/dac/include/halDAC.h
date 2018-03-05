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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   halDAC.h
/// @brief  DAC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_DAC_H_
#define _HAL_DAC_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define NONPM_BASE                  (0x100000)
#define XC_DAC_BASE                 (0x101A00)
#define XC_LPLL_BASE                (0x103100)
#define CHIP_REG_BASE               (0x100B00)
#define CHIPTOP_REG_BASE            (0x101E00)
#define HDMITX_MISC_REG_BASE        (0x172A00)
#define CLKGEN1_BASE                (0x103300)
#define ANA_MISC_HDMI_BASE          (0x110B00)
#define DAC_PLL_BASE                (0x121100)

// DAC PLL address offset
#define DAC_PLL_ADDR_OFFSET     0x120000
// HDMI Tx divider address offset
#define HDMITX_DIV_ADDR_OFFSET  0x110000

// SCALER_BASE
#define XC_SC_BASE                  (0x102F00)
#define XC_SC_BK_SELECT             XC_SC_BASE

// HDGEN BASE
#define HDGEN_BASE                 (0x103000)
#define HDGEN_BK_SELECT             HDGEN_BASE
#define HDGEN_P2I                   0x02
#define HDGEN_HDVBI                 0x03
// HDGEN address offset
#define HDGEN_ADDR_OFFSET           0x100000

// MOD_BASE
#define XC_MOD_BASE                 (0x103200)
#define XC_MOD_BK_SELECT            XC_MOD_BASE

#define BK_SC(u32Addr)          (XC_SC_BASE + (u32Addr))
#define BK_HDGEN(u32Addr)       (HDGEN_BASE + (u32Addr))
#define BK_MOD(u32Addr)         (XC_MOD_BASE + ((u32Addr) << 1))            ///< convert 16 bit address to 8 bit address

#define BK_CHIPTOP(u32Addr)         (XC_LPLL_BASE + ((u32Addr) << 1))           ///< convert 16 bit address to 8 bit address)
#define BK_LPLL(u32Addr)            (XC_LPLL_BASE + ((u32Addr) << 1))           ///< convert 16 bit address to 8 bit address
#define BK_DAC(u32Addr)             (XC_DAC_BASE + ((u32Addr) << 1))             ///< convert 16 bit address to 8 bit address
#define BK_HDGEN_MAP(u16Addr)       ((u16Addr>>8)&0xFF), (u16Addr&0xFF)

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

#define L_BK_DAC(x)                 BK_REG_L(XC_DAC_BASE, x)
#define H_BK_DAC(x)                 BK_REG_H(XC_DAC_BASE, x)
#define L_BK_CHIPTOP(x)             BK_REG_L(CHIP_REG_BASE, x)
#define H_BK_CHIPTOP(x)             BK_REG_H(CHIP_REG_BASE, x)
#define L_BK_LPLL(x)                BK_REG_L(XC_LPLL_BASE, x)
#define H_BK_LPLL(x)                BK_REG_H(XC_LPLL_BASE, x)
#define L_BK_HDMITX(x)              BK_REG_L(HDMITX_MISC_REG_BASE, x)
#define H_BK_HDMITX(x)              BK_REG_H(HDMITX_MISC_REG_BASE, x)
#define L_BK_CLKGEN1(x)             BK_REG_L(CLKGEN1_BASE, x)
#define H_BK_CLKGEN1(x)             BK_REG_H(CLKGEN1_BASE, x)
#define L_BK_ANA_MISC_HDMI(x)       BK_REG_L(ANA_MISC_HDMI_BASE, x)
#define H_BK_ANA_MISC_HDMI(x)       BK_REG_H(ANA_MISC_HDMI_BASE, x)
#define L_BK_CHIPTOP_TOP(x)         BK_REG_L(CHIPTOP_REG_BASE, x)
#define H_BK_CHIPTOP_TOP(x)         BK_REG_H(CHIPTOP_REG_BASE, x)


#define REG_DAC_SD_CTRL             (0x00)
#define REG_DAC_SD_CLK              (0x01)
#define REG_DAC_SD_SEL              (0x02)
#define REG_DAC_HD_CTRL             (0x03)
#define REG_DAC_HD_CLK              (0x04)
#define REG_DAC_HD_SEL              (0x05)
#define REG_DAC_LEVEL_CTRL          (0x08)
#define REG_DAC_PLUG_INOUT_PERIOD   (0x09)
#define REG_DAC_SD_DETECT_CTRL      (0x0a)
#define REG_DAC_HD_DETECT_CTRL      (0x0b)
#define REG_DAC_SD_CMP_RESULT       (0x0c)
#define REG_DAC_HD_CMP_RESULT       (0x0d)
#define REG_DAC_Delay_Sel           (0x0e)
#define REG_DAC_GCR_LEVEL_CTRL      (0x11)
#define REG_DAC_VSYNC_DETECT_CTRL   (0x14)
#define REG_DAC_HD_PLUG_IN_THD      (0x21)
#define REG_DAC_HD_PLUG_OUT_THD     (0x22)
#define REG_DAC_HD_IRQ_CTRL         (0x23)
#define REG_DAC_BIAS_CUR_CTRL       (0x27)
#define REG_DAC_CHN_BUF_EN          (0x28)
#define REG_DAC_SD_PLUG_IN_THD      (0x31)
#define REG_DAC_SD_PLUG_OUT_THD     (0x32)
#define REG_DAC_SD_IRQ_CTRL         (0x33)

//for check VDAC IC for IDAC IC , bank CHIPTOP_REG_BASE 0x101E00
#define REG_CHIPTOP_DACMODE         (0x60)

#define DAC_MODE_IDAC               (0x3F)
#define DAC_MODE_VDAC               (0x3E)

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
#define DAC_SC_OUTPUT_INTERLACE     (0x136B02)
#endif

#define R1BYTE(u32Addr, u8mask)            \
    (READ_BYTE (gu32DacRiuBaseAddr + (u32Addr << 1) - (u32Addr & 1)) & BMASK(u8mask))

// to write 0x2F02[4:3] with 2'b10, please use W1BYTE(0x2F02, 0x02, 4:3)
#define W1BYTE(u32Addr, u8Val, u8mask)     \
    (WRITE_BYTE(gu32DacRiuBaseAddr + (u32Addr << 1) - (u32Addr & 1), (R1BYTE(u32Addr, 7:0) & ~BMASK(u8mask)) | (BITS(u8mask, u8Val) & BMASK(u8mask))))

// u32Addr must be 16bit aligned
#define R2BYTE(u32Addr, u16mask)            \
    (READ_WORD (gu32DacRiuBaseAddr + (u32Addr << 1)) & BMASK(u16mask))

// u32Addr must be 16bit aligned
#define W2BYTE(u32Addr, u16Val, u16mask)    \
    (WRITE_WORD(gu32DacRiuBaseAddr + (u32Addr << 1), (R2BYTE(u32Addr, 15:0) & ~BMASK(u16mask)) | (BITS(u16mask, u16Val) & BMASK(u16mask))))

// u32Addr must be 16bit aligned
#define R2BYTE_TAB(u32Addr, u16mask)            \
    (READ_WORD (gu32DacRiuBaseAddr + ( (u32Addr)  << 1)) & u16mask)

// u32Addr must be 16bit aligned
#define W2BYTE_TAB(u32Addr, u16Val, u16mask)    \
    (WRITE_WORD(gu32DacRiuBaseAddr + ( (u32Addr)  << 1), (R2BYTE( (u32Addr) , 15:0) & ~u16mask) | (  (u16Val)  & u16mask)))

// u32Addr must be 16bit aligned
#define W1BYTE_TAB(u32Addr, u8Val, u8mask)    \
        (WRITE_BYTE(gu32DacRiuBaseAddr + ( (u32Addr)  << 1) - (u32Addr & 1), (R1BYTE( (u32Addr) , 7:0) & ~u8mask) | (  (u8Val)  & u8mask)))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#if defined(HAL_DAC_C) || defined(HAL_CLKSEL_C)
#define INTERFACE
#else
#define INTERFACE extern
#endif

extern MS_VIRT gu32DacRiuBaseAddr;

INTERFACE void Hal_DAC_SetIOMapBase(MS_VIRT u32NPMBase, MS_VIRT u32PMBase);
INTERFACE void Hal_HDMITx_InitSeq(void);
INTERFACE void Hal_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
INTERFACE void Hal_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
INTERFACE void Hal_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr);
INTERFACE void Hal_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr);
INTERFACE void Hal_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr);
INTERFACE void Hal_HDGEN_SetTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming);
//INTERFACE void Hal_ClkSel_Set(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming, EN_OUTPUT_BIT_TYPE enBits);
INTERFACE void Hal_DAC_Power_Saving(MS_BOOL POWER_SAVING);
INTERFACE void Hal_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit);
INTERFACE EN_DAC_SD_ENABLE_CTRL Hal_DAC_GetSDStatus(void);
INTERFACE void Hal_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit);
INTERFACE EN_DAC_HD_ENABLE_CTRL Hal_DAC_GetHDStatus(void);

INTERFACE void Hal_HDGEN_SetWSSOnOff(MS_BOOL bEnable);
INTERFACE MS_BOOL Hal_HDGEN_GetWSSStatus(void);
INTERFACE void Hal_HDGEN_ResetWSSData(void);
INTERFACE MS_BOOL Hal_HDGEN_Set_WSS_data(MS_BOOL ben, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, MS_U16 u16wssdata);
INTERFACE MS_U16 Hal_HDGEN_Get_WSS_data(void);
INTERFACE MS_BOOL Hal_HDGEN_Set_WSS525_A_data(MS_BOOL ben, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, MS_U32 u32wssdata);
INTERFACE MS_U32 Hal_HDGEN_Get_WSS525_A_data(void);

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
INTERFACE void Hal_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
INTERFACE void Hal_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
//----------------------------------------------------------------
// Hal_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
INTERFACE void Hal_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
//----------------------------------------------------------------
// Hal_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
INTERFACE MS_BOOL Hal_DAC_GetDACState(MS_BOOL bIsYPbPr);

//----------------------------------------------------------------
// Hal_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
INTERFACE MS_BOOL Hal_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *State);

//----------------------------------------------------------------
// Hal_DAC_DumpTable - Dump DAC tables
// @param: pDACTable: pointer to DAC table
// @param: u8DACtype: DAC table type
//----------------------------------------------------------------
INTERFACE void Hal_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype);

INTERFACE void Hal_DAC_SetDataReverse(MS_BOOL bTrue);

INTERFACE void Hal_DAC_EnableChannelBuf(MS_BOOL bTrue);

INTERFACE MS_BOOL Hal_DAC_EnableHotPlugDetectISR(MS_BOOL bEnabled);

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
INTERFACE MS_BOOL Hal_DAC_GetOutputInterlaceTiming(void);
#endif

INTERFACE MS_BOOL Hal_HDGEN_EnableICT(MS_BOOL bEnable);


//----------------------------------------------------------------
// DAC - set VGA Hsync Vsync
// @return none
//----------------------------------------------------------------
INTERFACE void Hal_DAC_SetVGAHsyncVsync(MS_BOOL bEnable);


#ifdef __cplusplus
}
#endif

#endif


