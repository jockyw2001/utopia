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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

#ifndef MHAL_FRC_H
#define MHAL_FRC_H

#include "hwreg_frc_map.h"
#include "hwreg_frc.h"
#include "hwreg_sc.h"

#ifdef MHAL_FRC_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define FRC_MIU1_MEM_ADDR     0x20000000
#define CRYSTAL_CLOCK    12000000ul//24000000ul
#define MST_CLOCK_HZ    CRYSTAL_CLOCK
#define MST_CLOCK_KHZ    (MST_CLOCK_HZ / 1000)
#define MST_CLOCK_MHZ    (MST_CLOCK_KHZ / 1000)
#define FRC_MAP_REG(reg) (((reg)>>8)&0xFF), ((reg)&0xFF)
#define FRC_IP_ALL 0xff
#define FRC_IP_NULL 0xff
#define REG_ADDR_SIZE 2
#define REG_MASK_SIZE 1
//#define REG_TABLE_END          0xFFFF
#define _END_OF_TBL_        0xFFFF
#define  MS_ALG_CMD_LEN         16
#define FRC_BK_SWITCH(_x_)\
        MDrv_WriteByte(REG_FRC_BANK_BASE, _x_)
#define FRC_BK_CURRENT   \
        MDrv_ReadByte(REG_FRC_BANK_BASE)

#define VSTART_OFFSET 2
#define VSYNCSTART_OFFSET 14
#define VSYNC_FRONT_PORCH 11

#define FRCR2_MBX_QUEUESIZE 10

#define GOP_FSC_FHD_OFFSET_H    0x02
#define GOP_FSC_FHD_OFFSET_V    0x0C
#define GOP_FSC_4K_OFFSET_H     0x02
#define GOP_FSC_4K_OFFSET_V     0x0C
#define GOP_NO_FSC_OFFSET_H     0x00
#define GOP_NO_FSC_OFFSET_V     0x00

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
    MS_U8 u8TabIdx;
} EN_FRC_IP_Info;

typedef struct
{
    MS_U8 *pIPTable;
    MS_U8 u8TabNums;
} EN_FRC_IPTAB_INFO;

typedef struct  __attribute__((packed))
{
    MS_U8 u8FRC_InputType_Num;
    MS_U8 u8FRC_IP_Num;
}FRCTABLE_INFO;


typedef enum
{
    E_FRC_op2_gamma_table = 1,  // gamma table
    E_FRC_od_table                = 2, // OD table
    E_FRC_ld_table                 = 3, // Local diming table
} FRC_CLIENT_TABLE;

//-------------------------------------------------------------------------------------------------
//  Structure enum
//-------------------------------------------------------------------------------------------------

typedef enum
{
    FRC_IP_MEM_IP_YC_8              = 0x00,
    FRC_IP_MEM_IP_YC_10             = 0x01,
    FRC_IP_MEM_IP_RGB_8             = 0x02,
    FRC_IP_MEM_IP_RGB_10            = 0x03,
    FRC_IP_MEM_IP_YC_10_SPECIAL     = 0x04,
    FRC_IP_MEM_IP_YUV_8             = 0x05,
    FRC_IP_MEM_IP_RGB_10_SPECIAL    = 0x06,
    FRC_IP_MEM_IP_YUV_10_SPECIAL    = 0x07

} FRC_IP_MEM_MODE_e;

typedef enum
{
    FRC_MIRROR_OFF,
    FRC_MIRROR_H_MODE,
    FRC_MIRROR_V_MODE,
    FRC_MIRROR_HV_MODE
} FRC_MirrorModeType_e;

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

// for composer setting
typedef enum
{
    E_FRC_COMPOSER_SOURCE_MODE_VIP     = 0,  // only STGEN disable case use this!
    E_FRC_COMPOSER_SOURCE_MODE_FSC     = 1,
    E_FRC_COMPOSER_SOURCE_MODE_FRC     = 2,
    E_FRC_COMPOSER_SOURCE_MODE_VIP_FB  = 3,
} E_FRC_COMPOSER_SOURCE_MODE;

typedef enum
{
    E_FRC_PIPE_DELAY_MODE_FIXED     = 0,
    E_FRC_PIPE_DELAY_MODE_USER      = 1,
    E_FRC_PIPE_DELAY_MODE_AUTO      = 2,
} E_FRC_PIPE_DELAY_MODE;

typedef enum
{
    E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_IP     = 0,  // with FRC case
    E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN   = 1,  // no FRC case
} E_FRC_TGEN_LOCK_SOURCE_MODE;

typedef enum
{
    E_FRC_STGEN_ODCLK_1      = 0,  // ODLCK
    E_FRC_STGEN_ODCLK_2      = 1,  // ODLCK/2
    E_FRC_STGEN_ODCLK_4      = 2,  // ODCLK/4
} E_FRC_STGEN_ODCLK;

typedef enum
{
    E_FRC_FSC_SOURCE_MODE_MDE     = 0, // vip to fsc's de selection use MDE
    E_FRC_FSC_SOURCE_MODE_FDE     = 1,
} E_FRC_FSC_SOURCE_MODE;

typedef enum
{
    E_FRC_MLOAD_TRIG_MODE_FROM_OP1     = 0,  // no FRC case, menuload trig form stgen(FSC is enabled)
    E_FRC_MLOAD_TRIG_MODE_FROM_OP2     = 1,  // 1. with FRC case 2. no FRC and no FSC case, menuload trig form tgen
} E_FRC_MLOAD_TRIG_MODE;

typedef enum
{
    ///  MEMC level off
    MEMC_LEVEL_OFF,
    ///  MEMC level low
    MEMC_LEVEL_LOW,
    ///  MEMC level middle
    MEMC_LEVEL_MID,
    ///  MEMC level high
    MEMC_LEVEL_HIGH,
    ///  MEMC level bypass, its same as bypass 64 mode
    MEMC_LEVEL_BYPASS,
    ///  MEMC level bypass 64, one kind framerate converter of bypass.
    MEMC_LEVEL_BYPASS_64 = MEMC_LEVEL_BYPASS,
    ///  MEMC level bypass 55, one kind framerate convertr of bypass.
    MEMC_LEVEL_BYPASS_55,
}E_MEMC_LEVEL;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#define MHal_FRC_IsRGB(args...) 0

#define MHal_FRC_TGEN_SWReset(args...)
INTERFACE void MHal_FRC_TGEN_DoubleBuf(void *pInstance, MS_BOOL bEnDis);
INTERFACE void MHal_FRC_TGEN_SetVTotal(void *pInstance, MS_U16 u16VTotal);
INTERFACE void MHal_FRC_TGEN_SetHTotal(void *pInstance, MS_U16 u16HTotal);
#define MHal_FRC_TGEN_SetVTrigY(args...)
#define MHal_FRC_TGEN_SetVTrigX(args...)
INTERFACE void MHal_FRC_TGEN_SetVSyncStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetHSyncStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetFdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetFdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetSubMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End);
INTERFACE void MHal_FRC_TGEN_SetSubMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End);

#define MHal_FRC_TGEN_FpllRefPointY(args...)
#define MHal_FRC_TGEN_1Clk2PixOut(args...)
#define MHal_FRC_TGEN_SetSyncHcnt(args...)
#define MHal_FRC_TGEN_SetClrSyncAhead(args...)

INTERFACE void MHal_FRC_TGEN_Init(void *pInstance);
INTERFACE void MHal_FRC_TGEN_Enable_LockMode(void *pInstance, MS_BOOL bEn);
INTERFACE void MHal_FRC_TGEN_Enable_Source_Select_Mode(void *pInstance, MS_BOOL bEn);
//INTERFACE void MHal_FRC_TGEN_Set_Lock_Source(void *pInstance, E_FRC_TGEN_LOCK_SOURCE_MODE eMode);
INTERFACE void MHal_FRC_TGEN_Enable_Lock_Source(void *pInstance, MS_BOOL bEn, E_FRC_TGEN_LOCK_SOURCE_MODE eMode, MS_BOOL bMenuload);

INTERFACE void MHal_FRC_SetYTrig(void *pInstance, MS_U16 u16Trig1, MS_U16 u16Trig2);

INTERFACE void MHal_FRC_Enable_MiuMask(void *pInstance);
INTERFACE void MHal_FRC_Disable_MiuMask(void *pInstance);
INTERFACE void MHal_FRC_set_miusel(void *pInstance, MS_U8 u8MIUSel);

#define MHal_FRC_IPM_SoftwareReset(args...)
#define MHal_FRC_IPM_RWEn(args...)
#define MHal_FRC_IPM_RW_CEN_Select(args...)
#define MHal_FRC_IPM_SetYuv10Bit(args...)
#define MHal_FRC_IPM_SetMr(args...)
#define MHal_FRC_IPM_SetMemoryMode(args...)
#define MHal_FRC_IPM_YC444To422Control(args...)
#define MHal_FRC_IPM_SetFrameBufferNum(args...)
#define MHal_FRC_IPM_CeLineCountEn(args...)
INTERFACE void MHal_FRC_IPM_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32FrameYcout, MS_U16 u16FB_YcountLinePitch, MS_U8 u8MirrorMode );
INTERFACE void MHal_FRC_IPM_R_SetBaseAddr(void *pInstance, MS_PHY addr);
#define MHal_FRC_IPM_SetIp2Mc(args...)
#define MHal_FRC_IPM_SetYCoutLinePitch(args...)
#define MHal_FRC_IPM_SetReadFetchNumber(args...)
#define MHal_FRC_IPM_SetWriteFetchNumber(args...)
#define MHal_FRC_IPM_SetRfifoThr(args...)
#define MHal_FRC_IPM_SetWfifoThr(args...)
#define MHal_FRC_IPM_SetReadLength(args...)
#define MHal_FRC_IPM_SetWriteLength(args...)
#define MHal_FRC_IPM_SetHTotalPixellimit(args...)
#define MHal_FRC_IPM_SetVTotalPixellimit(args...)
#define MHal_FRC_IPM_SetRmaskNum(args...)
#define MHal_FRC_IPM_SetWmaskNum(args...)
#define MHal_FRC_IPM_SetMirrorMode(args...)
#define MHal_FRC_IPM_PacketInitCnt(args...)
#define MHal_FRC_IPM_SetLvdsInputMode(args...)
#define MHal_FRC_IPM_SetOsdWinIdx(args...)
#define MHal_FRC_IPM_SetVPulseLineRst(args...)
#define MHal_FRC_IPM_SetVPulseLoc(args...)
#define MHal_FRC_IPM_SetLockIntCtrl(args...)
#define MHal_FRC_IPM_SetBlankBoundary(args...)
#define MHal_FRC_IPM_GetHTotal(args...) 0
#define MHal_FRC_IPM_GetVTotal(args...) 0
#define MHal_FRC_IPM_GetHde(args...) 0
#define MHal_FRC_IPM_GetVde(args...) 0
#define MHal_FRC_IPM_Csc(args...)
#define MHal_FRC_IPM_CscDither(args...)
#define MHal_FRC_IPM_CscRound(args...)
#define MHal_FRC_IPM_CscSaturation(args...)
#define MHal_FRC_IPM_CheckBoardEn(args...)
#define MHal_FRC_IPM_SetIpCtrl(args...)
#define MHal_FRC_IPM_SetHRefLock(args...)
#define MHal_FRC_IPM_GetHdeCount(args...) 0
#define MHal_FRC_IPM_TestPattern(args...)
#define MHal_FRC_IPM_SetHTotal(args...)
#define MHal_FRC_IPM_SetVTotal(args...)
#define MHal_FRC_IPM_HActive(args...)
#define MHal_FRC_IPM_VActive(args...)
#define MHal_FRC_IPM_GetYcoutLinePitch(args...) 0
#define MHal_FRC_IPM_GetWriteFetchNum(args...) 0
#define MHal_FRC_IPM_GetReadFetchNum(args...) 0
#define MHal_FRC_IPM_GetLineLimit(args...) 0
#define MHal_FRC_IPM_SetPacketCount(args...)
#define MHal_FRC_IPM_GetFrameYcout(args...) 0
#define MHal_FRC_IPM_3DFlag_In_SWMode_En(args...)
#define MHal_FRC_IPM_3DFlag_In_SWMode_SetIdx(args...)
#define MHal_FRC_IPM_3DFlag_In_HWMode_SrcSel(args...)
#define MHal_FRC_IPM_GetIPVfreqX10(args...) 0



#define MHal_FRC_OPM_SetFbLineOffset_Data(args...)
#define MHal_FRC_OPM_SetFbLineOffset_Me3(args...)
#define MHal_FRC_OPM_SetSrcPixNum(args...)
#define MHal_FRC_OPM_SetSrcLineNum(args...)
#define MHal_FRC_OPM_CeLineCountEn(args...)
#define MHal_FRC_OPM_SetLbiSrcPixelNum_Left(args...)
#define MHal_FRC_OPM_SetLbiReadPixelNum_Left(args...)
#define MHal_FRC_OPM_SetLbiSkipEn(args...)
#define MHal_FRC_OPM_SetLbiSkipNum_Left(args...)
#define MHal_FRC_OPM_SetLbiSrcPixelNum_Right(args...)
#define MHal_FRC_OPM_SetLbiReadPixelNum_Right(args...)
#define MHal_FRC_OPM_SetLbiSkipEn3D(args...)
#define MHal_FRC_OPM_SetLbiSkipNum_Right(args...)
#define MHal_FRC_OPM_FuncEn(args...)
#define MHal_FRC_OPM_3dFuncEn(args...)
#define MHal_FRC_OPM_SetBaseOffset_Data0(args...)
#define MHal_FRC_OPM_SetBaseOffset_Data1(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me0(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me1(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me3(args...)
#define MHal_FRC_OPM_SetBaseOffset_Da0_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Da1_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me0_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me1_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Me3_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Mr1_L(args...)
#define MHal_FRC_OPM_SetBaseOffset_Mr1(args...)
#define MHal_FRC_OPM_SetDebugMask(args...)
#define MHal_FRC_OPM_SwReset(args...)
#define MHal_FRC_OPM_enableBaseAdrMr(args...)
INTERFACE void MHal_FRC_OPM_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32FrameYcout);
INTERFACE void MHal_FRC_OPM_R_SetBaseAddr(void *pInstance, MS_PHY addr);
#define MHal_FRC_OPM_SetMlbOutRstCycle(args...)
#define MHal_FRC_OPM_SetFifoTrigThr(args...)
#define MHal_FRC_OPM_SetFifoMaxReadReq(args...)
#define MHal_FRC_OPM_SetGatingClk(args...)
#define MHal_FRC_SetMemMode(args...)

#define MHal_FRC_OP2_ColorMatrixEn(args...)
#define MHal_FRC_OP2_CscDitherEn(args...)
#define MHal_FRC_OP2_DataPathEn(args...)
#define MHal_FRC_OP2_DebugFuncEn(args...)
#define MHal_FRC_OP2_SetGain_R(args...)
#define MHal_FRC_OP2_SetGain_G(args...)
#define MHal_FRC_OP2_SetGain_B(args...)
#define MHal_FRC_OP2_SetOffset_R(args...)
#define MHal_FRC_OP2_SetOffset_G(args...)
#define MHal_FRC_OP2_SetOffset_B(args...)
#define MHal_FRC_OP2_SetDither(args...)
#define MHal_FRC_OP2_PAFRC_FuncEn(args...)
#define MHal_FRC_OP2_PAFRC_FuncEn2(args...)
#define MHal_FRC_OP2_PAFRC_FuncEn3(args...)
#define MHal_FRC_OP2_PAFRC_Set2x2BlockRotDir(args...)
#define MHal_FRC_OP2_PAFRC_TopBoxFrSeq(args...)
#define MHal_FRC_OP2_PAFRC_TopBoxFrC2Seq(args...)
#define MHal_FRC_OP2_PAFRC_SetBoxRotDir(args...)
#define MHal_FRC_OP2_PAFRC_Set8x8BoxRotDir(args...)
#define MHal_FRC_OP2_PAFRC_SetBlockEntities(args...)
#define MHal_FRC_OP2_PAFRC_PolarityCtrl(args...)
#define MHal_FRC_OP2_PAFRC_SetBoxLsbValue(args...)

#define MHal_FRC_SCTOP_SCMI_Bypass_Enable(args...)
#define MHal_FRC_SCTOP_FRC_Bypass_Enable(args...)

#define MHal_FRC_SNR_SetPixelHorixontalNum(args...)
#define MHal_FRC_SNR_SetLineVerticalNum(args...)

#define MHal_FRC_SCP_HSU1_HSP(args...)
#define MHal_FRC_SCP_HSU1_Init_Position(args...)
#define MHal_FRC_SCP_HSU1_Scaling_Mode(args...)
#define MHal_FRC_SCP_HSU1_444to422_Mode(args...)
#define MHal_FRC_SCP_HSU1_VSU_Scaling_Mode(args...)
#define MHal_FRC_SCP_HSU1_VSU_Scaling_coef(args...)
#define MHal_FRC_SCP_HSU2_HSP(args...)
#define MHal_FRC_SCP_HSU2_Init_Position(args...)
#define MHal_FRC_SCP_HSU2_Scaling_Mode(args...)

#define MHal_FRC_OD_SetBaseAddr(args...)
#define MHal_FRC_LD_SetBaseAddr(args...)
#define MHal_FRC_LD_Edge2D_SetBaseAddr(args...)
INTERFACE void MHal_FRC_ME1_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size);
INTERFACE void MHal_FRC_ME2_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size);
#define MHal_FRC_2D3D_Render_SetBaseAddr(args...)
#define MHal_FRC_2D3D_Render_Detection_SetBaseAddr(args...)
INTERFACE void MHal_FRC_Halo_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size);
#define MHal_FRC_OverDriverSwitch(args...)
#define MHal_FRC_OD_Init(args...)

// for FRC init
INTERFACE void MHal_FRC_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo);
INTERFACE void MHal_FRC_ByPass_Enable(void *pInstance, MS_BOOL bEnable);

#define MHal_FRC_LoadTabelbySrcType(args...)
INTERFACE void MHal_FRC_Set_3D_QMap(void *pInstance, MS_U8 u8FRC3DPanelType, MS_U16 u16sc_in_3dformat, MS_U16 u16sc_out_3dformat, MS_U8 u83D_FI_out);


//FRC R2 Mail Box Control
INTERFACE MS_BOOL MHal_XC_SendCmdToFRC(void *pInstance, MS_U8 u8Cmd, MS_U8 count, MS_U8 p1, MS_U8 p2, MS_U8 p3, MS_U8 p4, MS_U8 p5, MS_U8 p6, MS_U8 p7, MS_U8 p8);
INTERFACE MS_BOOL MHal_XC_GetMsgFromFRC(void *pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para);

INTERFACE MS_U16 Hal_XC_FRC_R2_Get_SwVersion(void *pInstance);
INTERFACE MS_U16 Hal_XC_FRC_R2_Get_CmdVersion(void *pInstance);
INTERFACE MS_BOOL Hal_XC_FRC_R2_Init(void *pInstance, MS_U8 u8Panel3DType, MS_U8 u8PanelMaxFreq, MS_U8 u8LocalDimingPanelType, MS_U8 u8ChipRevision);
INTERFACE MS_BOOL Hal_XC_FRC_R2_Set_Timing(void *pInstance, MS_U8 u8InVFreq, MS_U8 u8OutVFreq);
INTERFACE MS_BOOL Hal_XC_FRC_R2_Set_InputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize );
INTERFACE MS_BOOL Hal_XC_FRC_R2_Set_OutputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize );
INTERFACE MS_BOOL Hal_XC_FRC_R2_Set_FPLL_Lockdone(void *pInstance, MS_BOOL bLockDone);
INTERFACE MS_BOOL Hal_XC_FRC_R2_Enable_MEMC(void *pInstance, MS_U16 u16MEMCLevel);

#define Hal_XC_FRC_R2_Set_Input3DFormat(args...)
#define Hal_XC_FRC_R2_Set_MfcMode(args...)
#define Hal_XC_FRC_R2_Set_MfcDemoMode(args...)
#define Hal_XC_FRC_R2_Set_LocalDimmingMode(args...)
#define Hal_XC_FRC_R2_Set_2DTo3DMode(args...) 0

//FRC auto download
#define Hal_FRC_ADLG_set_base_addr(args...)
#define Hal_FRC_ADLG_set_depth(args...)
#define Hal_FRC_ADLG_set_dma(args...)
#define Hal_FRC_ADLG_set_on_off(args...)

#define Hal_FRC_get_table_idx(args...) FRC_NOTSUPPORT_MODE

#define MHal_XC_OC_set_HSyncStartEnd(args...)
#define MHal_XC_OC_set_VSyncStartEnd(args...)
#define MHal_XC_OC_set_HFdeStartEnd(args...)
#define MHal_XC_OC_set_VFdeStartEnd(args...)
#define MHal_XC_OC_set_HTotal(args...)
#define MHal_XC_OC_set_VTotal(args...)

#define MHal_FRC_IPM_SetOffset(args...)
#define MHal_FRC_IPM_SetFetchNum(args...)
#define MHal_FRC_OPM_SetOffset(args...)
#define MHal_FRC_OPM_SetFetchNum(args...)
#define MHal_FRC_HSU_SetScalingSize(args...)
#define MHal_FRC_VSU_SetScalingSize(args...)
#define MHal_FRC_CSC_SelectPath(args...)

#define MHal_FRC_interrupt_mask(args...)
#define MHal_FRC_Mute(args...)
#define MHal_FRC_3DLR_Select(args...)

INTERFACE void MHal_FRC_Composer_User_Mode_Enable(void *pInstance, MS_BOOL bEnable);
INTERFACE void MHal_FRC_Set_Composer_User_Mode(void *pInstance, E_FRC_COMPOSER_SOURCE_MODE eMode, MS_BOOL bMenuload);

INTERFACE void MHal_FRC_Set_Pipe_Delay_Mode(void *pInstance, E_FRC_PIPE_DELAY_MODE eMode);
INTERFACE void MHal_FRC_Set_Pipe_Delay_Value(void *pInstance, MS_U16 u16Hcnt, MS_U16 u16Vcnt);
INTERFACE void MHal_FRC_Set_Pipe_Delay_Reset(void *pInstance, MS_BOOL bEnable, MS_BOOL bMenuload);

INTERFACE void MHal_FRC_Set_STGEN_ODCLK(void *pInstance, E_FRC_STGEN_ODCLK eMode, MS_BOOL bMenuload);
INTERFACE void MHal_FRC_Set_FSC_DE_Selection(void *pInstance, E_FRC_FSC_SOURCE_MODE eMode);
INTERFACE MS_BOOL MHal_FRC_IsFHDToFSC(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_FRC_IsSupportFRC_byEfuse(void *pInstance);
INTERFACE MS_BOOL MHal_FRC_Check_Condition(void *pInstance);
INTERFACE void MHal_FRC_AdjustGOPPosition(void *pInstance);
INTERFACE void MHal_FRC_GetGOPOffset(void *pInstance, MS_U16 *u16OffsetH, MS_U16 *u16OffsetV);
INTERFACE MS_BOOL MHal_FRC_IsEnableFRC3D(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MHal_FRC_Set_Mload_Trig_Mode(void *pInstance, E_FRC_MLOAD_TRIG_MODE eMode);
INTERFACE void MHal_SWDS_AddTGENMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End, SCALER_WIN eWindow);
INTERFACE void MHal_SWDS_AddTGENMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End, SCALER_WIN eWindow);

#define MHal_FRC_Set_FO_Path(args...)
#define MHal_FRC_Set_FO_VOP_H_Size(args...)
#define MHal_FRC_Set_FO_SPTP_Setting(args...)
#define MHal_FRC_Set_FO_2Line_Setting(args...)
#define MHal_FRC_Set_FO_OSDDS_Setting(args...)

#define MHal_FRC_FO_TGEN_SetVTotal(args...)
#define MHal_FRC_FO_TGEN_SetHTotal(args...)
#define MHal_FRC_FO_TGEN_SetVTrigY(args...)
#define MHal_FRC_FO_TGEN_SetVTrigX(args...)
#define MHal_FRC_FO_TGEN_SetVSyncStartEndY(args...)
#define MHal_FRC_FO_TGEN_SetHSyncStartEndX(args...)
#define MHal_FRC_FO_TGEN_SetFdeStartEndY(args...)
#define MHal_FRC_FO_TGEN_SetFdeStartEndX(args...)
#define MHal_FRC_FO_TGEN_SetMdeStartEndY(args...)
#define MHal_FRC_FO_TGEN_SetMdeStartEndX(args...)
#define MHal_FRC_FO_TGEN_SetMdeStartEndX_L(args...)
#define MHal_FRC_FO_TGEN_SetMdeStartEndX_R(args...)

INTERFACE MS_BOOL Hal_XC_Get_Stgen_Lock_Ip_status(void *pInstance);

#undef INTERFACE
#endif /* MHAL_FRC_H */

