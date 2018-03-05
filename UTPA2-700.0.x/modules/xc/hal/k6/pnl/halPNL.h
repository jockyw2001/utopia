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
/// @file   halPNL.h
/// @brief  Panel Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_PNL_H_
#define _HAL_PNL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _HAL_PNL_C_
#define HAL_PNL_INTERFACE
#else
#define HAL_PNL_INTERFACE extern
#endif

//   Current platform is DAC out or not
#define IS_DAC_OUT      TRUE

// version0: Not support TV chip as HDMITx
// version1: Maserati + Raptor
// version2: Maxim + inside HDMITx
#define HW_DESIGN_HDMITX_VER                (0)

// XC register serpead define
#define XC_REGISTER_SPREAD 1
#define SUPPORT_FRC       0
#define PNL_SUPPORT_2P_MODE 0

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define  GAMMA_10BIT              BIT(0)            ///< gamma value range up to 10 BIt
#define  GAMMA_12BIT              BIT(1)            ///< gamma value range up to 12 BIT

#define  GAMMA_8BIT_MAPPING       BIT(0)            ///< mapping GAMMA value to 256 sampline entries
#define  GAMMA_10BIT_MAPPING      BIT(1)            ///< mapping GAMMA value to 1024 sampling entries

typedef struct
{
    MS_U8 eSupportGammaType;                        ///< refer to HAL_PNL_GAMMA_TYPE
    MS_U8 eSupportGammaMapMode;                       ///< refero to HAL_PNL_GAMMA_MAPPEING_MODE
} PNL_HalInfo;

#define SUPPORT_OVERDRIVE                   1
#define GAMMA_TYPE                  (GAMMA_10BIT | GAMMA_12BIT)
#define GAMMA_MAPPING               (GAMMA_8BIT_MAPPING)
#define SUPPORT_SYNC_FOR_DUAL_MODE			TRUE  //New feature after T7
#define ENABLE_MODE_PATCH	0
#define PNL_SUPPORT_DEVICE_NUM	2

// MIU Word (Bytes)
#define BYTE_PER_WORD           (8)

#define SUPPORT_TCON            FALSE
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

// NONPM
#define REG_CHIPTOP_BASE            0x100B00  // 0x1E00 - 0x1EFF
#if XC_REGISTER_SPREAD
#define REG_SCALER_BASE             0x130000UL
#else
#define REG_SCALER_BASE             0x102F00UL
#endif
#define REG_HDGEN_BASE              0x103000
#define REG_LPLL_BASE               0x103100
#define REG_MOD_BASE                0x103200
#define REG_UTMI1_BASE              0x103A00

#define XC_DAC_BASE                 0x101A00
#define HDMITX_MISC_REG_BASE        0x172A00



#define L_BK_DAC(x)                 BK_REG_L(XC_DAC_BASE, x)
#define H_BK_DAC(x)                 BK_REG_H(XC_DAC_BASE, x)
#define L_BK_HDMITX(x)              BK_REG_L(HDMITX_MISC_REG_BASE, x)
#define H_BK_HDMITX(x)              BK_REG_H(HDMITX_MISC_REG_BASE, x)


/* TCON */
#define L_BK_TCON(x)                BK_REG_L(REG_HDGEN_BASE, x)
#define H_BK_TCON(x)                BK_REG_H(REG_HDGEN_BASE, x)

/* LPLL */
#define L_BK_LPLL(x)                BK_REG_L(REG_LPLL_BASE, x)
#define H_BK_LPLL(x)                BK_REG_H(REG_LPLL_BASE, x)

/* UTMI1 */
#define L_BK_UTMI1(x)               BK_REG_L(REG_UTMI1_BASE, x)
#define H_BK_UTMI1(x)               BK_REG_H(REG_UTMI1_BASE, x)

#define REG_CLKGEN0_52_L            (REG_CHIPTOP_BASE + 0xA4)
#define REG_CLKGEN0_53_L            (REG_CHIPTOP_BASE + 0xA6)
#define REG_CLKGEN0_57_L            (REG_CHIPTOP_BASE + 0xAE)
#define REG_CLKGEN0_58_L            (REG_CHIPTOP_BASE + 0xB0)
#define REG_CLKGEN0_5E_L            (REG_CHIPTOP_BASE + 0xBC)



#define XC_PAFRC_DITH_NOISEDITH_EN          (0x01)
#define XC_PAFRC_DITH_TAILCUT_DISABLE       (0x00)

#define LVDS_DUAL_OUTPUT          0
#define LVDS_DUAL_OUTPUT_SPECIAL  1// only for use with T8 board
#define LVDS_SINGLE_OUTPUT_A      2
#define LVDS_SINGLE_OUTPUT_B      3
#define LVDS_OUTPUT_User          4

// SCALER CLK select
#define REG_CKG_ODCLK           REG_CLKGEN0_53_L
    #define CKG_ODCLK_GATED         BIT(0)
    #define CKG_ODCLK_INVERT        BIT(1)
    #define CKG_ODCLK_MASK          BMASK(3:2)
    #define CKG_ODCLK_CLK_OUT_PIX   (0 << 2)
    #define CKG_ODCLK_CLK_LPLL      CKG_ODCLK_CLK_OUT_PIX    //Mapping

#define REG_CKG_SC1_ODCLK           REG_CLKGEN0_5E_L
    #define CKG_SC1_ODCLK_GATED         BIT(8)
    #define CKG_SC1_ODCLK_INVERT        BIT(9)
    #define CKG_SC1_ODCLK_MASK          BMASK(11:10)
    #define CKG_SC1_ODCLK_13M           (0 << 10)
    #define CKG_SC1_ODCLK_EDCLK         (1 << 10)

#define PANEL_LPLL_INPUT_DIV_1st          0x00
#define PANEL_LPLL_INPUT_DIV_2nd          0x00 // 0:/1, 1:/2, 2:/4, 3:/8
#define PANEL_LPLL_LOOP_DIV_1st           0x03 // 0:/1, 1:/2, 2:/4, 3:/8
#define PANEL_LPLL_LOOP_DIV_2nd           0x01 //
#define PANEL_LPLL_OUTPUT_DIV_1st         0x00 // 0:/1, 1:/2, 2:/4, 3:/8
#define PANEL_LPLL_OUTPUT_DIV_2nd         0x00
#define LPLL_LOOPGAIN                     0x08

#define LVDS_MPLL_CLOCK_MHZ     216 // For crystal 24Mhz, mm = 9
#define LVDS_SPAN_FACTOR        131072

#define REG_DAC_SD_CTRL             (0x00)
#define REG_DAC_SD_CLK              (0x01)
#define REG_DAC_SD_SEL              (0x02)
#define REG_DAC_HD_CTRL             (0x03)
#define REG_DAC_HD_CLK              (0x04)
#define REG_DAC_HD_SEL              (0x05)
#define REG_DAC_LEVEL_CTRL          (0x08)
#define REG_VE_CONFIG_01             0x01

#define VOP_DE_HSTART_MASK      (0x3FFF) //BK_10_04
#define VOP_DE_HEND_MASK        (0x3FFF) //BK_10_05
#define VOP_DE_VSTART_MASK      (0x1FFF) //BK_10_06
#define VOP_DE_VEND_MASK        (0x1FFF) //BK_10_07

#define VOP_VTT_MASK            (0x1FFF) //BK_10_0D
#define VOP_HTT_MASK            (0x3FFF) //BK_10_0C

#define VOP_VSYNC_END_MASK      (0x1FFF) //BK_10_03
#define VOP_DISPLAY_HSTART_MASK (0x3FFF) //BK_10_08
#define VOP_DISPLAY_HEND_MASK   (0x3FFF) //BK_10_09
#define VOP_DISPLAY_VSTART_MASK (0x1FFF) //BK_10_0A
#define VOP_DISPLAY_VEND_MASK   (0x1FFF) //BK_10_0B

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HALPNL_DEVICE0_XC_BANK_OFFSET    = 0,
    E_HALPNL_DEVICE1_XC_BANK_OFFSET    = 0x80
}PNL_HAL_DEVICE_XC_BANK_OFFSET;

typedef enum
{
    E_DRVPNL_ALLIN_MODE      = 1,
    E_DRVPNL_2X_MODE         = 2,
    E_DRVPNL_SEPARATE_MODE   = 3,
    E_DRVPNL_TYPE_NUM
}DRVPNL_OUT_SWING_TYPE;

typedef enum
{
    HAL_TI_10BIT_MODE = 0,
    HAL_TI_8BIT_MODE = 2,
    HAL_TI_6BIT_MODE = 3,
} PNL_HAL_TIMODES;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
HAL_PNL_INTERFACE MS_VIRT g_ptr_PnlRiuBaseAddr;
HAL_PNL_INTERFACE MS_VIRT g_ptr_PMRiuBaseAddr;

MS_U8 MHal_MOD_PowerOn(void *pInstance, MS_BOOL bEn, MS_U8 u8LPLL_Type,MS_U8 DualModeType, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21);
void MHal_PNL_TCON_Init(void *pInstance);

void MHal_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping);

void MHal_Shift_LVDS_Pair(void *pInstance, MS_U8 Type);
void MHal_Output_LVDS_Pair_Setting(void *pInstance, MS_U8 Type, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21);
void MHal_PQ_Clock_Gen_For_Gamma(void *pInstance);

void MHal_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping);
MS_BOOL Hal_VOP_Is_GammaMappingMode_enable(void *pInstance);
MS_BOOL Hal_VOP_Is_GammaSupportSignalWrite(void *pInstance, DRVPNL_GAMMA_MAPPEING_MODE u8Mapping);
void hal_PNL_WriteGamma12Bit(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue);
void hal_PNL_SetMaxGammaValue( void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue);
void Hal_PNL_Set12BitGammaPerChannel( void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode );
#define Hal_PNL_Get12BitGammaPerChannel(args...)
void MHal_PNL_Init_LPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz);
//void _MDrv_PNL_Set_12BIT_Gamma( MS_U8 u8Channel, MS_U8 * u8Tab );
MS_U16 MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);
#define MHal_PNL_FRC_lpll_src_sel(args...)

MS_U8 MHal_PNL_Get_Loop_DIV( void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);
MS_BOOL Hal_PNL_SkipTimingChange_GetCaps(void *pInstance);
void MHal_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode);
void MHal_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo);
void MHal_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_MSB_limit, MS_PHY u32OD_LSB_Addr, MS_PHY u32OD_LSB_limit, MS_U8 u8MIUSel);
void MHal_PNL_OverDriver_Enable(void *pInstance, MS_BOOL bEnable);
void MHal_PNL_OverDriver_TBL(void *pInstance, MS_U8 u8ODTbl[1056]);

void MHal_PNL_PreInit(void *pInstance, PNL_OUTPUT_MODE eParam);
PNL_OUTPUT_MODE MHal_PNL_Get_Output_MODE(void *pInstance);
void MHal_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type);
MS_BOOL MHal_PNL_MOD_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level);
MS_BOOL MHal_PNL_MOD_Control_Out_PE_Current (void *pInstance, MS_U16 u16Current_Level);
#define MHal_PNL_MOD_PECurrent_Setting(args...)
MS_BOOL MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (void *pInstance, MS_BOOL  u16TTL_OP_Level);

void MHal_PNL_Init_MOD(void *pInstance, PNL_InitData *pstPanelInitData);
void MHal_PNL_Init_XC_Clk(void *pInstance, PNL_InitData *pstPanelInitData);
void MHal_PNL_DumpMODReg(void *pInstance, MS_U32 u32Addr, MS_U16 u16Value, MS_BOOL bHiByte, MS_U16 u16Mask);
void MHal_MOD_Calibration_Init(void *pInstance, PNL_ModCali_InitData *pstModCaliInitData);
void MHal_BD_LVDS_Output_Type(void *pInstance, MS_U16 Type);
PNL_Result MHal_PNL_MOD_Calibration(void *pInstance);
PNL_Result MHal_PNL_En(void *pInstance, MS_BOOL bPanelOn, MS_BOOL bCalEn);
void MHal_PNL_SetOutputPattern(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue);

void MHal_PNL_Switch_LPLL_SubBank(void *pInstance, MS_U16 u16Bank);
void Mhal_PNL_Flock_LPLLSet(void *pInstance, MS_U64 ldHz);

void MHal_PNL_Switch_TCON_SubBank(void *pInstance, MS_U16 u16Bank);
MS_U16 MHal_PNL_Read_TCON_SubBank(void *pInstance);
MS_BOOL MHal_PNL_IsYUVOutput(void *pInstance);

MS_BOOL MHal_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable);
void MHal_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable);

/// Set pair swap for user mode
#define MHal_FRC_MOD_PairSwap_UserMode(args...)

#define MHal_PNL_Is_Support120Hz(args...) SUPPORT_FRC

#define MHal_PNL_CalExtLPLLSETbyDClk(args...)

#define MHal_PNL_VBY1_Handshake(args...) TRUE
#define MHal_PNL_VBY1_OC_Handshake(args...) TRUE


#define MHal_PNL_SetOutputInterlaceTiming(args...) 0
MS_BOOL MHal_PNL_GetOutputInterlaceTiming(void *pInstance);
#define MHal_PNL_SetOSDCOutputType(args...)

#define MHal_PNL_Set_T3D_Setting(args...)

void MHal_PNL_Set_Device_Bank_Offset(void *pInstance);
void MHal_PNL_Init(void *pInstance);
#define MHal_PNL_ChannelFIFOPointerADjust(args...)
#define MHal_Pnl_Get_SupportMaxDclk(args...) 0

MS_U16 MHal_PNL_GetPanelVStart(void);
#define MHal_PNL_Check_VBY1_Handshake_Status(args...) FALSE
#define MHal_PNL_VBY1_Hardware_TrainingMode_En(args...)
#define MHal_PNL_VBY1_IsSupport_Hardware_TrainingMode(args...) FALSE
#define MHal_PNL_TCON_Patch(args...)

#ifdef __cplusplus
}
#endif

#endif // _HAL_PNL_H_

