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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    mdrv_frc.h
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_FRC_H_
#define _MDRV_FRC_H_
#include "mhal_frc.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if 0
typedef struct MST_PANEL_INFO_s
{
    // Basic
    MS_U16 u16HStart; //ursa scaler
    MS_U16 u16VStart; //ursa scaler
    MS_U16 u16Width; //ursa scaler
    MS_U16 u16Height; //ursa scaler
    MS_U16 u16HTotal; //ursa scaler
    MS_U16 u16VTotal; //ursa scaler

    MS_U16 u16DE_VStart;

    MS_U16 u16DefaultVFreq;

    // LPLL
    MS_U16 u16LPLL_InputDiv;
    MS_U16 u16LPLL_LoopDiv;
    MS_U16 u16LPLL_OutputDiv;

    MS_U8  u8LPLL_Type;
    MS_U8  u8LPLL_Mode;

    // sync
    MS_U8  u8HSyncWidth;
    MS_BOOL bPnlDblVSync;

    // output control
    MS_U16 u16OCTRL;
    MS_U16 u16OSTRL;
    MS_U16 u16ODRV;
    MS_U16 u16DITHCTRL;

    // MOD
    MS_U16 u16MOD_CTRL0;  // BIT2: tiMode, BIT5: lvdsSwapPol, BIT6: lvdsSwapCh
    MS_U8  u8MOD_CTRL2;
    MS_U16 u16MOD_CTRL9;
    MS_U16 u16MOD_CTRLA;  // BIT2: invertDE, BIT3: invertVS, BIT12: invertHS
    MS_U8  u8MOD_CTRLB;   // BIT0~1: panelBitNums

    MS_U8  u8MOD_CTRL77;    //pre-emphasis level
    MS_U8  u8MOD_CTRL78;
    //LGE [vivakjh] 2008/11/12     Add for DVB PDP Panel
    //Additional Info.(V Total)
    MS_U16 u16VTotal60Hz; //ursa scaler
    MS_U16 u16VTotal50Hz; //ursa scaler
    MS_U16 u16VTotal48Hz; //ursa scaler
    //[vivakjh] 2008/12/23    Add for adjusting the MRE in PDP S6
    MS_U16 u16VStart60Hz;
    MS_U16 u16VStart50Hz;
    MS_U16 u16VStart48Hz;
    MS_U16 u16VBackPorch60Hz;
    MS_U16 u16VBackPorch50Hz;
    MS_U16 u16VBackPorch48Hz;

    /*Panel Option
    0: LCD
    1: PDP
    2: LCD_NO_FRC
    3: LCD_TCON
    */
    MS_U8    u8LCDorPDP;

    MS_U32 u32LimitD5d6d7; //thchen 20081216
    MS_U16 u16LimitOffset; //thchen 20081216
    MS_U8  u8LvdsSwingUp;
    MS_BOOL bTTL_10BIT;
    MS_BOOL bOD_DataPath;

//-------------------------------------------------------------------------------------------------
// FRC Control
//-------------------------------------------------------------------------------------------------
    MS_BOOL bFRC;
    MS_U16 u16MOD_SwingLevel;
    MS_U16 u16MOD_SwingCLK;
    MS_U16 u16output_cfg_10;
    MS_U16 u16output_cfg_11;
    MS_U16 u16output_cfg_12;
    MS_U8 u8output_cfg_13;
    MS_U8 u8PanelNoiseDith;
    MS_U8 u8lvdsSwapCh;
    MS_U8 u8FRC3DPanelType;

    MS_BOOL bdither6Bits;
    MS_BOOL blvdsShiftPair;
    MS_BOOL blvdsSwapPair;

// TGEN
    MS_U16  u16HSyncStart;
    MS_U16  u16HSyncEnd;
    MS_U16  u16VSyncStart;
    MS_U16  u16VSyncEnd;
    MS_U16 u16VTrigX;
    MS_U16 u16VTrigY;

// EPI
    MS_BOOL bepiLRSwap;
    MS_BOOL bepiLMirror;
    MS_BOOL bepiRMirror;

} MST_PANEL_INFO_t, *PMST_PANEL_INFO_t;
#endif
typedef struct
{
//-------------------------------------------------------------------------------------------------
// FRC Control
//-------------------------------------------------------------------------------------------------
    MS_U16 u16MOD_SwingLevel;
    MS_U16 u16MOD_SwingCLK;
    MS_U8 u8PanelNoiseDith;
    MS_U8 u8lvdsSwapCh;
    MS_BOOL bdither6Bits;
    MS_BOOL blvdsShiftPair;
    MS_BOOL blvdsSwapPair;

// EPI
    MS_BOOL bepiLRSwap;
    MS_BOOL bepiLMirror;
    MS_BOOL bepiRMirror;
} MST_PANEL_FRC_INFO_t, *PMST_PANEL_FRC_INFO_t;


typedef struct
{
    MS_PHY u32MfcBase;
    MS_U32 u32MfcSize;

    MS_PHY u32IpYcoutBase;
    MS_U32 u32IpYcoutSize;
    MS_PHY u32IpMrBase;
    MS_U32 u32IpMrSize;

    MS_PHY u32GammaBase;
    MS_U32 u32GammaSize;

    MS_PHY u32OdBase;
    MS_U32 u32OdSize;
    MS_PHY u32OdLsbBase;
    MS_U32 u32OdLsbSize;

    MS_U32 u32AutoBist;

    MS_U8  u8MfcMode;
    MS_U8  u8IpMode;
    MS_U8  u8GammaMode;
    MS_U8  u8OdMode;
}MST_MFC_MiuBaseAddr_t, *PMST_MFC_MiuBaseAddr_t;

typedef struct MST_FRC_SYS_INFO_s
{
    //PMST_PANEL_INFO_t pPanelInfo;    // panel information
    MS_U16 u16HStart; //ursa scaler
    MS_U16 u16VStart; //ursa scaler
    MS_U16 u16Width; //ursa scaler
    MS_U16 u16Height; //ursa scaler
    MS_U16 u16HTotal; //ursa scaler
    MS_U16 u16VTotal; //ursa scaler

    //titania to URSA
    MS_U8  u8LVDSTxChannel;         //Single, Dual
    MS_U8  u8LVDSTxBitNum;          //8bits, 10 bits
    MS_U8  u8LVDSTxTiMode;          //Thin/Ti mode scaler 40-bit2
    MS_U8  u8LVDSTxSwapMsbLsb;     //ursa scaler
    MS_U8  u8LVDSTxSwap_P_N;         //ursa scaler
    MS_U8  u8LVDSTxSwapOddEven;     //ursa scaler

    //URSA to Panel info
    MS_U8  u8PanelVfreq;             //Panel frame rate 60Hz, 120Hz, 240Hz
    MS_U8  u8PanelChannel;         //Single, Dual, Quad, Quad_LR
    MS_U8    u8PanelLVDSSwapCH;         //LVDS chenel swap ABCD
    MS_U8  u8PanelBitNum;             //Panel bit number
    MS_U8    u8PanelLVDSShiftPair;     //ursa scaler
    MS_U8    u8PanelLVDSTiMode;         //Panel TI/Thin mode
    MS_U8    u8PanelLVDSSwapPol;     //Panel LVDS polarity swap
    MS_U8    u8PanelLVDSSwapPair;     //Panel LVDS pair swap

    MS_U16    u16MFCMemoryClk;                //MFC memory clock MHz
    MS_U16    u16MFCMemoryType;            //MFC memory type
    MS_U8    u8PanelIncVtotalFor50Hz;    //Change Vtotal for DCLK
    MS_U8    u8PanelType;                  //TTL, Mini_LVDS, LVDS
    MS_U8    u8PanelDither;
    MS_U8    u8PanelBlankCPVC;            //Panel Mini LVDS use
    MS_U8    u8PanelBlankOEC;            //Panel Mini LVDS use
    MS_U8    u8PanelBlankTPC;            //Panel Mini LVDS use
    MS_U8    u8PanelBlankSTHC;            //Panel Mini LVDS use
    MS_U8    u8PanelCSC;                    //LVDS CSC enable/disable
    MS_U8    u8PanelGAMMA;                //Panel GAMMA enable/disable
    MS_U8  u8UseMPIF;
}MST_FRC_SYS_INFO_t, *PMST_FRC_SYS_INFO_t;

typedef struct __attribute__((packed))
{
    MS_PHY PhyAddr; 
    MS_U16 u16MaxCmdCnt;
    MS_BOOL bEnable;
}MS_AutoDownLoad_Info;

typedef struct
{
    MS_U32 u32B;
    MS_U32 u32G;
    MS_U32 u32R;
    MS_U32 u32Enable;
} MS_FRC_ADLG_TBL;

typedef enum
{
    E_FRC_PNL_TYPE_2D = 0,
    E_FRC_PNL_TYPE_PASSIVE,
    E_FRC_PNL_TYPE_ACTIVE,
    E_FRC_PNL_TYPE_PDP,
    E_FRC_PANEL_3D_TYPE_ACTIVE_240_1920_540 =9,
} E_FRC_3D_PANEL_TYPE;

typedef enum
{
    E_FRC_INPUT_2D = 0,       // 2D
    E_FRC_INPUT_3D_SBS = 1,   // side by side
    E_FRC_INPUT_3D_TD = 2,    // Top-Down
    E_FRC_INPUT_3D_CB = 3,    // Check Board
    E_FRC_INPUT_3D_FI = 4,    // Frame Interleave
    E_FRC_INPUT_3D_LA = 6,    // Line Alternative
    E_FRC_INPUT_3D_FPP = 7,   // Frame Packing
}E_FRC_3D_INPUT_FMT;

typedef enum
{
    E_FRC_3D_CFG_NORMAL = 0,    // normal 3D
    E_FRC_3D_CFG_LR_SWAP,       // L/R swap
    E_FRC_3D_CFG_L_ONLY,        // Only L side
    E_FRC_3D_CFG_R_ONLY,        // Only R side
}E_FRC_3D_MODE_CFG;

typedef enum
{
    E_FRC_COLOR_PATH_RGB_444 = 0,
    E_FRC_COLOR_PATH_RGB_422,
} E_FRC_COLOR_PATH;

typedef struct
{
    MS_U16 u16RedGain;
    MS_U16 u16GreenGain;
    MS_U16 u16BlueGain;

    MS_U16 u16RedOffset;
    MS_U16 u16GreenOffset;
    MS_U16 u16BlueOffset;
}MS_FRC_RGBGAINOFFSET_INFO;

#define FRC_NOTSUPPORT_MODE 0xFFFF

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void MDrv_FRC_ByPass_Enable(void *pInstance, MS_BOOL bEnable);

void MDrv_FRC_Tx_SetTgen(void *pInstance, PMST_PANEL_INFO_t pPanelInfo);
void MDrv_FRC_TGEN_SetHTotal(void *pInstance, MS_U16 u16HTotal);
void MDrv_FRC_TGEN_SetVTotal(void *pInstance, MS_U16 u16VTotal);

void MDrv_FRC_SetMemFormat(void *pInstance, PFRC_INFO_t pFRCInfo);
void MDrv_FRC_IPM_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo);
void MDrv_FRC_IPM_SetOffset(void *pInstance, MS_U16 offset);
void MDrv_FRC_IPM_SetFetchNum(void *pInstance, MS_U16 fetch);
void MDrv_FRC_OPM_Init(void *pInstance);
void MDrv_FRC_OPM_SetOffset(void *pInstance, MS_U16 offset);
void MDrv_FRC_OPM_SetFetchNum(void *pInstance, MS_U16 fetch);
void MDrv_FRC_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo);
void MDrv_FRC_Set_3D_QMap(void *pInstance, MS_U8 u8FRC3DPanelType, MS_U16 u16sc_in_3dformat, MS_U16 u16sc_out_3dformat, MS_U8 u83D_FI_out);
void MDrv_FRC_AutoDownLoad_init(MS_PHY PhyAddr, MS_U32 u32BufByteLen); //NO_USE
void MDrv_FRC_ADLG_Trigger(FRC_CLIENT_TABLE client,MS_PHY startAddr, MS_U16 u16CmdCnt); //NO_USE
void MDrv_FRC_ADLG_WritetoDRAM(FRC_CLIENT_TABLE client,MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue); //NO_USE
void MDrv_FRC_SetGammaTbl(MS_U8* pu8GammaTab[3]); //NO_USE
void MDrv_FRC_ADLG_Fire(FRC_CLIENT_TABLE client,MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue); //NO_USE
void MDrv_FRC_OPM_SetBaseOfset(void *pInstance, FRC_INFO_t *FRCInfo, E_XC_3D_OUTPUT_MODE u16out_3dformat);

MS_BOOL MDrv_XC_SendCmdToFRC(void *pInstance, MS_U8 u8Cmd, MS_U8 count, FRC_R2_CMD_PARAMETER_t pFRC_R2_Para );
MS_BOOL MDrv_XC_GetMsgFromFRC(void *pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para);

void MDrv_XC_set_OC_Reg(void *pInstance, PMST_PANEL_INFO_t pPanelInfo);

void MDrv_FRC_OP2_ColorMatrixEn(void *pInstance, MS_BOOL bEnable);
void MDrv_FRC_OP2_CscDitherEn(void *pInstance, MS_BOOL bEnable);
void MDrv_FRC_OP2_BrightnessEn(void *pInstance, MS_BOOL bEnable);
void MDrv_FRC_OP2_ContrastEn(void *pInstance, MS_BOOL bEnable);
void MDrv_FRC_OP2_NoiseRoundEn(void *pInstance, MS_BOOL bEnable);
void MDrv_FRC_OP2_SetRGBGain(void *pInstance, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain);
void MDrv_FRC_OP2_SetRGBOffset(void *pInstance, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void MDrv_FRC_OP2_SetDither(void *pInstance, MS_U16 u16dither);
void MDrv_FRC_HSU_SetScalingSize(void *pInstance, MS_U16 input, MS_U16 output);
void MDrv_FRC_VSU_SetScalingSize(void *pInstance, MS_U16 input, MS_U16 output);
void MDrv_FRC_CSC_SelectPath(void *pInstance, MS_U8 type);

void MDrv_FRC_PNLInfo_Transform(void *pInstance, XC_PANEL_INFO *XC_PNLInfo, XC_PREINIT_INFO_t *XC_PNLInfo_Adv, MST_PANEL_INFO_t *PanelInfo);
void MDrv_XC_FRC_SetWindow(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode, E_XC_3D_OUTPUT_MODE e3dOutputMode, E_XC_3D_PANEL_TYPE e3dPanelType);
void MDrv_FRC_Mute(void *pInstance, MS_BOOL bEnable);
MS_BOOL MDrv_XC_Set_FRC_InputTiming(void *pInstance, E_XC_FRC_InputTiming enFRC_InputTiming);
void MDrv_FRC_UpdateMDE(void *pInstance, MS_WINDOW_TYPE stDisplayWin);
void MDrv_XC_FRC_Set_Input3DFormat(void *pInstance, E_XC_3D_INPUT_MODE enFrcInput3DType);
MS_BOOL MDrv_XC_FRC_R2_Set_InputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize );
MS_BOOL MDrv_XC_FRC_R2_Set_OutputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize );
MS_BOOL MDrv_FRC_IsDdr4Dram(void *pInstance,PFRC_INFO_t pFRCInfo);
void MDrv_FRC_SetMemoryAddress(void *pInstance, PXC_FRC_ADDRESS_INFO pFRCAddrInfo);

#endif
