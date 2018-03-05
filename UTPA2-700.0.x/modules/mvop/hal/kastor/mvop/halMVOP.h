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

#ifndef _HAL_MVOP_H_
#define _HAL_MVOP_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define STB_DC                  0   //0 for TV series MVOP; 1 for STB DC.
#define STB_DC_MODE

#define MVOP_MIU_CLIENT_MAIN    MIU_CLIENT_MVOP_256BIT_R
#define MVOP_MIU_CLIENT_SUB     MIU_CLIENT_MVOP1_256BIT_R

#define _MVOP_128BIT_BUS        0//128-bit MIU bus
#define _MVOP_64BIT_BUS         1// 64-bit MIU bus
#define _MVOP_256BIT_BUS        2//256-bit MIU bus
#define MVOP_BUS_WIDTH          _MVOP_256BIT_BUS

#define HAL_MVOP_MODULE_CNT     2
#if (HAL_MVOP_MODULE_CNT >= 2)
#define MVOP_SUPPORT_SUB        1
#else
#define MVOP_SUPPORT_SUB        0
#endif
#if (HAL_MVOP_MODULE_CNT >= 3)
#define MVOP_SUPPORT_3RD        1
#else
#define MVOP_SUPPORT_3RD        0
#endif

#define NO_MVOP_PATCH           1
#define ENABLE_3D_LR_MODE           1 //Enable 3D L/R feature
#define SUPPORT_3DLR_INST_VBLANK    0
#define SUPPORT_3DLR_ALT_SBS        1

#define SUPPORT_EVD_MODE        1
#define SUPPORT_VP9_MODE        1

#if (STB_DC == 0)
#define MVOP_BASE_ADD_BITS          31  //29 - 1 (MSB is signed bit) + 3 (unit in 8-byte)
#endif

#define SUPPORT_KERNAL_STR            1

#define MVOP_BANK_REGNUM 0x80UL
#define MVOP_CLK_REGNUM 0x10UL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    HALMVOP_SYNCMODE,
    HALMVOP_FREERUNMODE,
    HALMVOP_27MHZ = 27000000ul,
    HALMVOP_54MHZ = 54000000ul,
    HALMVOP_320MHZ = 320000000ul,
    HALMVOP_108MHZ = 108000000ul,
    HALMVOP_123MHZ = 123000000ul,
    HALMVOP_144MHZ = 144000000ul,
    HALMVOP_160MHZ = 160000000ul,
    HALMVOP_172MHZ = 172000000ul,
    HALMVOP_192MHZ = 192000000ul,
    HALMVOP_CLK_MIU,
}HALMVOPFREQUENCY;

typedef enum
{
    E_MVOP_MASK_DB_REG,
    E_MVOP_EN_DB_REG,
    E_MVOP_DEBUG_SEL,
    E_MVOP_UPDATE_SEL,
    E_MVOP_RST_START_SEL,
    E_MVOP_SC_VSYNC_SEL,
    E_MVOP_READ_IP_BASE_SEL,
    E_MVOP_CHKSUM_422_SWAP,
    E_MVOP_CHKSUM_FULL_C,
    E_MVOP_OFF_LATCH_CRC,
    E_MVOP_SEL_OP_FIELD ,
    E_MVOP_INV_OP_FIELD,
    E_MVOP_INV_OP_VS
}MVOPMSAKDBREGCTRL;

//for Kappa patch
typedef enum
{
    E_MVOP_DEB2MVD_FRAME_MODE,
    E_MVOP_DEB2MVD_FIELD_INV,
    E_MVOP_SEQ_FROM_MVD,
    E_MVOP_SIZE_FROM_MVD
}MVOPDEB2MVDFRAMECTRL;

typedef enum
{
    E_MVOP_BASE_FROM_IP,
    E_MVOP_SRC_FROM_MVD,
    E_MVOP_FIELD_FROM_MVD,
    E_MVOP_FIELD_FROM_MVD_INV,
    E_MVOP_PITCH_FROM_IP,
    E_MVOP_FMT_FROM_MVD,
    E_MVOP_FD_MASK_CLR,
    E_MVOP_FD_MASK_INV
#if (NO_MVOP_PATCH == 0)
  , E_MVOP_SEQ_FROM_MVD_PATCH,
    E_MVOP_SIZE_FROM_MVD_PATCH
#endif
}MVOPBASEFROMIPCTRL;

typedef struct
{
    MS_U8 u8Gp:4;
    MS_U8 u8BitPos:4;
}HALMVOPMIUCLIENTINFO;

typedef enum
{
    E_MVOP_SEL_MIU0         = 0,
    E_MVOP_SEL_MIU1         = 0x01,
    E_MVOP_SEL_MIU2         = 0x02,
    E_MVOP_SEL_MIU3         = 0x03,
    E_MVOP_SEL_NONE         = 0xFF,

} HALMVOPMIUSEL;

typedef struct
{
    MS_U16 MVOP_BANK[3][0x80];
    MS_U16 CLK_BANK[0x10];

}MVOP_STR_SAVE_AREA;

typedef enum
{
    E_MVOP_POWER_SUSPEND     = 1,    // State is backup in DRAM, components power off.
    E_MVOP_POWER_RESUME      = 2,    // Resume from Suspend or Hibernate mode
    E_MVOP_POWER_MECHANICAL  = 3,    // Full power off mode. System return to working state only after full reboot
    E_MVOP_POWER_SOFT_OFF    = 4,    // The system appears to be off, but some components remain powered for trigging boot-up.
} EN_MVOP_POWER_MODE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_MVOP_RegSetBase(MS_VIRT u32Base);
void HAL_MVOP_Init(void);
void HAL_MVOP_SetFieldInverse(MS_BOOL b2MVD, MS_BOOL b2IP);
void HAL_MVOP_SetChromaWeighting(MS_BOOL bEnable);
void HAL_MVOP_LoadReg(void);
void HAL_MVOP_SetMIUReqMask(MS_BOOL bEnable);
void HAL_MVOP_Enable(MS_BOOL bEnable, MS_U8 u8Framerate);
MS_BOOL HAL_MVOP_GetEnableState(void);
HALMVOPFREQUENCY HAL_MVOP_GetMaxFreerunClk(void);
HALMVOPFREQUENCY HAL_MVOP_Get4k2kClk(void);
void HAL_MVOP_SetFrequency(HALMVOPFREQUENCY enFrequency);
HALMVOPFREQUENCY HAL_MVOP_GetMaximumClk(void);
HALMVOPFREQUENCY HAL_MVOP_GetCurrentClk(void);
void HAL_MVOP_Rst(void);
void HAL_MVOP_SetBlackBG(void);
void HAL_MVOP_SetOutputInterlace(MS_BOOL bEnable);
void HAL_MVOP_SetCropWindow(MVOP_InputCfg *pparam);
void HAL_MVOP_SetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion );
void HAL_MVOP_EnableUVShift(MS_BOOL bEnable);
void HAL_MVOP_SetEnable60P(MS_BOOL bEnable);
void HAL_MVOP_SetEnable4k2kClk(MS_BOOL bEnable);
void HAL_MVOP_SetEnableFixClk(MS_BOOL bEnable);
void HAL_MVOP_SetVSyncMode(MS_U8 u8Mode);
void HAL_MVOP_SetOutputTiming( MVOP_Timing *ptiming );
void HAL_MVOP_SetDCClk(MS_U8 clkNum, MS_BOOL bEnable);
void HAL_MVOP_SetSynClk(MVOP_Timing *ptiming);
void HAL_MVOP_SetMonoMode(MS_BOOL bEnable);
void HAL_MVOP_SetH264HardwireMode(void);
void HAL_MVOP_SetRMHardwireMode(void);
void HAL_MVOP_SetJpegHardwireMode(void);
void HAL_MVOP_SetEVDHardwireMode(MS_U16 u16ECOVersion);
void HAL_MVOP_SetVP9HardwireMode(void);
void HAL_MVOP_SetEVDFeature(MVOP_DevID eID, MVOP_EVDFeature* stEVDInput);
void HAL_MVOP_SetPattern(MVOP_Pattern enMVOPPattern);
MS_BOOL HAL_MVOP_SetTileFormat(MVOP_TileFormat eTileFmt);
MS_BOOL HAL_MVOP_SetRgbFormat(MVOP_RgbFormat eRgbFmt);
MS_BOOL HAL_MVOP_GetSupportRgbFormat(MVOP_RgbFormat eRgbFmt);
MS_BOOL HAL_MVOP_Enable3DLR(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Get3DLRMode(void);
MS_BOOL HAL_MVOP_GetTimingInfoFromRegisters(MVOP_TimingInfo_FromRegisters *mvopTimingInfo);
void HAL_MVOP_SetHorizontallMirrorMode(MS_BOOL bEnable);
void HAL_MVOP_SetVerticalMirrorMode(MS_BOOL bEnable);
void HAL_MVOP_SetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack);
void HAL_MVOP_SetRepeatField(MVOP_RptFldMode eMode);
void HAL_MVOP_EnableFreerunMode(MS_BOOL bEnable);
MS_PHY HAL_MVOP_GetYBaseAdd(void);
MS_PHY HAL_MVOP_GetUVBaseAdd(void);
MS_BOOL HAL_MVOP_Set3DLRAltOutput(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Set3DLRAltOutput_VHalfScaling(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Set3DLRAltSBSOutput(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Get3DLRAltOutput(void);
MS_BOOL HAL_MVOP_Get3DLRAltSBSOutput(void);
EN_MVOP_Output_3D_TYPE HAL_MVOP_GetOutput3DType(void);
MS_BOOL HAL_MVOP_Set3DLR2ndCfg(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Get3DLR2ndCfg(void);
MVOP_DrvMirror HAL_MVOP_GetMirrorMode(MVOP_DevID eID);
MS_BOOL HAL_MVOP_SetVerDup(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_GetVerDup(void);
MS_BOOL HAL_MVOP_SetVerx4Dup(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_GetVerx4Dup(void);
MS_BOOL HAL_MVOP_SetHorx4Dup(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_GetHorx4Dup(void);
MS_BOOL HAL_MVOP_SetYUVBaseAddMultiView(MVOP_BaseAddInput *stBaseAddInfo);
MS_BOOL HAL_MVOP_SetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo);
MS_PHY HAL_MVOP_GetYBaseAddMultiView(MVOP_3DView eView);
MS_PHY HAL_MVOP_GetUVBaseAddMultiView(MVOP_3DView eView);
MS_U16 HAL_MVOP_GetTopVStart(MVOP_DevID eID);
MS_U16 HAL_MVOP_GetBottomVStart(MVOP_DevID eID);
MS_U16 HAL_MVOP_GetVCount(MVOP_DevID eID);
MS_BOOL HAL_MVOP_SetVC1RangeMap(MVOP_DevID eID, MVOP_VC1RangeMapInfo *stVC1RangeMapInfo);
MS_BOOL HAL_MVOP_Set420BWSaveMode(MS_BOOL bEnable);
void HAL_MVOP_SetRptPreVsyncFrame(MVOP_DevID eID, MS_BOOL bEnable);
void HAL_MVOP_PowerStateSuspend(void);
MVOP_HSMode HAL_MVOP_GetHandShakeMode(MVOP_DevID eID);
MS_BOOL HAL_MVOP_CheckSTCCW(void);
MS_BOOL HAL_MVOP_GetIsMiuIPControl(HALMVOPMIUCLIENTINFO stInfo);
void HAL_MVOP_SelMIU(MVOP_DevID eDevID, HALMVOPMIUSEL eMiuMSB0, HALMVOPMIUSEL eMiuMSB1, HALMVOPMIUSEL eMiuLSB0, HALMVOPMIUSEL eMiuLSB1);
MS_BOOL HAL_MVOP_GetIsOnlyMiuIPControl(void);
void HAL_MVOP_SetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable);
void HAL_MVOP_SetEnable4k2k2P(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_Get4k2k2PMode(MVOP_DevID eID);
HALMVOPFREQUENCY HAL_MVOP_SetDefaultClk(MVOP_DevID eDevID);
void HAL_MVOP_GetMaxFramerate(MVOP_DevID eDevID, MVOP_GetMaxFps* stStreamInfo);
MS_BOOL HAL_MVOP_GetIsSendingData(MVOP_DevID eDevID);
void HAL_MVOP_SetTimingFromXC(MVOP_DevID eID, MS_BOOL bEnable);
void HAL_MVOP_ResetReg(MVOP_DevID eDevID, MS_U16 u16ECONumber);
MS_BOOL HAL_MVOP_GetIsCurrentHSK(MVOP_DevID eID);
MS_BOOL HAL_MVOP_GetIsCurrentXCGenTiming(MVOP_DevID eID);
HALMVOPFREQUENCY HAL_MVOP_SetFixClk(MVOP_DevID eID, MS_U32 u32MVOPClk);
void HAL_MVOP_SetHandShakeMode(MVOP_DevID eID, MS_BOOL bEnable, MS_U8 u8Framerate);
void HAL_MVOP_SetCropforXC(MVOP_DevID eID, MVOP_XCGetCrop* stXCCrop, MS_U16 u16Width, MS_U16 u16Height);
MS_U16 HAL_MVOP_ReadBank(MVOP_DevID eID ,MS_U16 u16Length);
void HAL_MVOP_WriteBank(MVOP_DevID eID ,MS_U16 u16Length,MS_U16 u16Data);
MS_U16 HAL_MVOP_ReadClkBank(MS_U16 u16Length);
void HAL_MVOP_WriteClkBank(MS_U16 u16Length,MS_U16 u16Data);
void HAL_MVOP_SetInterlaceType(MVOP_DevID eDevID, MS_U16 u16ECONumber, MS_U8 u8Interlace);
MS_BOOL HAL_MVOP_GetIsMVOPSupportBLKBackground(MVOP_DevID eID);
void HAL_MVOP_SetSramPower(MVOP_DevID eID ,MS_BOOL bEnable);

//for sub mvop
void HAL_MVOP_SubRegSetBase(MS_VIRT u32Base);
void HAL_MVOP_SubInit(void);
void HAL_MVOP_SubSetFieldInverse(MS_BOOL b2MVD, MS_BOOL b2IP);
void HAL_MVOP_SubSetChromaWeighting(MS_BOOL bEnable);
void HAL_MVOP_SubLoadReg(void);
void HAL_MVOP_SubSetMIUReqMask(MS_BOOL bEnable);
void HAL_MVOP_SubRst(void);
void HAL_MVOP_SubEnable(MS_BOOL bEnable, MS_U8 u8Framerate);
MS_BOOL HAL_MVOP_SubGetEnableState(void);
HALMVOPFREQUENCY HAL_MVOP_SubGetMaxFreerunClk(void);
HALMVOPFREQUENCY HAL_MVOP_SubGet4k2kClk(void);
void HAL_MVOP_SubSetFrequency(HALMVOPFREQUENCY enFrequency);
void HAL_MVOP_SubSetOutputInterlace(MS_BOOL bEnable);
void HAL_MVOP_SubSetPattern(MVOP_Pattern enMVOPPattern);
MS_BOOL HAL_MVOP_SubSetTileFormat(MVOP_TileFormat eTileFmt);
MS_BOOL HAL_MVOP_SubSetRgbFormat(MVOP_RgbFormat eRgbFmt);
MS_BOOL HAL_MVOP_SubEnable3DLR(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_SubGet3DLRMode(void);
void HAL_MVOP_SubSetBlackBG(void);
void HAL_MVOP_SubSetCropWindow(MVOP_InputCfg *pparam);
void HAL_MVOP_SubSetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion );
void HAL_MVOP_SubEnableUVShift(MS_BOOL bEnable);
void HAL_MVOP_SubSetEnable60P(MS_BOOL bEnable);
void HAL_MVOP_SubSetEnable4k2kClk(MS_BOOL bEnable);
void HAL_MVOP_SubSetOutputTiming( MVOP_Timing *ptiming );
void HAL_MVOP_SubSetDCClk(MS_U8 clkNum, MS_BOOL bEnable);
void HAL_MVOP_SubSetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable);
void HAL_MVOP_SubSetSynClk(MVOP_Timing *ptiming);
void HAL_MVOP_SubSetMonoMode(MS_BOOL bEnable);
void HAL_MVOP_SubSetH264HardwireMode(void);
void HAL_MVOP_SubSetRMHardwireMode(void);
void HAL_MVOP_SubSetJpegHardwireMode(void);
void HAL_MVOP_SubSetEVDHardwireMode(MS_U16 u16ECOVersion);
void HAL_MVOP_SubSetVP9HardwireMode(void);
void HAL_MVOP_SubEnableMVDInterface(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_SubGetTimingInfoFromRegisters(MVOP_TimingInfo_FromRegisters *pMvopTimingInfo);
void HAL_MVOP_SubSetHorizontallMirrorMode(MS_BOOL bEnable);
void HAL_MVOP_SubSetVerticalMirrorMode(MS_BOOL bEnable);
void HAL_MVOP_SubEnableFreerunMode(MS_BOOL bEnable);
void HAL_MVOP_SubSetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack);
void HAL_MVOP_SubSetRepeatField(MVOP_RptFldMode eMode);
MS_PHY HAL_MVOP_SubGetYBaseAdd(void);
MS_PHY HAL_MVOP_SubGetUVBaseAdd(void);
MS_BOOL HAL_MVOP_SubSet420BWSaveMode(MS_BOOL bEnable, MS_U16 u16ECOVersion);
MS_BOOL HAL_MVOP_SubSetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo);
MS_BOOL HAL_MVOP_SubCheckSTCCW(void);
void HAL_MVOP_SubSetEnable4k2k2P(MS_BOOL bEnable);
void HAL_MVOP_SubSetVSyncMode(MS_U8 u8Mode);
MS_BOOL HAL_MVOP_SubSet3DLRAltOutput_VHalfScaling(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_SubSet3DLRAltOutput(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_SubSet3DLRAltSBSOutput(MS_BOOL bEnable);
MS_BOOL HAL_MVOP_SubGet3DLRAltOutput(void);
MS_BOOL HAL_MVOP_SubGet3DLRAltSBSOutput(void);
EN_MVOP_Output_3D_TYPE HAL_MVOP_SubGetOutput3DType(void);

//for STB_DC
void HAL_MVOP_SetStartX(MVOP_DevID eID, MS_U16 u16Length);
void HAL_MVOP_SetStartY(MVOP_DevID eID, MS_U16 u16Length, MS_BOOL bIsInterlace);
void HAL_MVOP_SetPicWidthMinus(MVOP_DevID eID, MS_U16 u16Sizes, MS_U16 u16Width);
void HAL_MVOP_SetPicHeightMinus(MVOP_DevID eID, MS_U16 u16Sizes, MS_U16 u16Height);
void HAL_MVOP_MaskDBRegCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPMSAKDBREGCTRL eModeCtrl);
void HAL_MVOP_SetDeb2MVDFrameModeCtrl(MVOP_DevID eID, MS_BOOL bEnable, MVOPDEB2MVDFRAMECTRL eModeCtrl);
void HAL_MVOP_BaseFromIPCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPBASEFROMIPCTRL eModeCtrl );

#endif // _HAL_MVOP_H_
