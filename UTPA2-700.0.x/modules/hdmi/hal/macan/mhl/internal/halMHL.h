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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mdrv_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MMHL_MHL_H_
#define _MMHL_MHL_H_

#include "mdrv_mhl_st.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MHL_FUNCTION_SUPPORT_PORTA      1
#define MHL_FUNCTION_SUPPORT_PORTB      0
#define MHL_FUNCTION_SUPPORT_PORTC      0
#define MHL_FUNCTION_SUPPORT_PORTD      0

#define MHL_ICONTROL_PD_24BIT_MODE0     8U
#define MHL_ICONTROL_PD_24BIT_MODE1     7U
#define MHL_ICONTROL_PD_PP_MODE0        8U
#define MHL_ICONTROL_PD_PP_MODE1        7U
#define MHL_ICONTROL_PD_600_MODE        2U
#define MHL_ICONTROL_PD_300_MODE        2U
#define MHL_ICONTROL_PD_150_MODE        4U
#define MHL_ICONTROL_PFD_24BIT_MODE0    15U
#define MHL_ICONTROL_PFD_24BIT_MODE1    15U
#define MHL_ICONTROL_PFD_PP_MODE0       15U
#define MHL_ICONTROL_PFD_PP_MODE1       15U
#define MHL_ICONTROL_PFD_600_MODE       15U
#define MHL_ICONTROL_PFD_300_MODE       8U
#define MHL_ICONTROL_PFD_150_MODE       8U

#define MHL_IMPEDANCE_VALUE             0U
#define MHL_ICONTROL_PD_VALUE           4U
#define MHL_DIGITAL_UNLOCK_RANGE        0x3E8U
#define MHL_LOCK_RESOLUTION             0x1U
#define MHL_UNLOCK_RESOLUTION           0x1EU

#define MHL_EQ_20_SETTING_VALUE         4U
#define MHL_EQ_30_SETTING_VALUE         16U
#define MHL_HDMI_EQ_SETTING_VALUE       12U

#define MHL_LOCK_TIME_VALUE             0x7FU
#define MHL_HDMI_LOCK_TIME_VALUE        0x1FU

#define MHL_CHIP_ID_MANHATTAN           0U

#define MHL_CDR_STABLE_COUNTER          200U
#define MHL_CDR_FORCE_COUNTER           20U
#define MHL_CDR_STABLE_OFFSET           1U
#define MHL_CDR_STABLE_THRESHOLD        (MHL_CDR_STABLE_COUNTER +MHL_CDR_STABLE_OFFSET)
#define MHL_CDR_FORCE_THRESHOLD         (MHL_CDR_STABLE_THRESHOLD +MHL_CDR_FORCE_COUNTER)

#define CBUS_CDR_CLOCK_TARGET           20U
#define CBUS_CLOCK_DETECT_LEVEL         (CBUS_CDR_CLOCK_TARGET *128 /12)

#define CBUS_CDR_OFFSET_TARGET          0.5
#define CBUS_CLOCK_DETECT_OFFSET        (CBUS_CDR_OFFSET_TARGET *128 /12)

#define MHL_CBUS_SW_POLLING_MODE        0U   // Use SW polling to reply MSC command
#define MHL_CBUS_SW_ISR_MODE            1U   // Use SW interrupt to reply MSC command
#define MHL_CBUS_HW_ISR_MODE            2U   // Use SW interrupt to clear MSC receiver flag to trigger HW auto reply
#define MHL_CBUS_HW_REPLY_MODE          3U   // Use xdata to saving MSC command to trigger HW auto reply

#define MHL_CBUS_OPERATION_MODE         MHL_CBUS_HW_REPLY_MODE

#define MHL_VBUS_OUTPUT_MODE            1U
#define MHL_VBUS_INVERSE_MODE           2U
#define MHL_VBUS_LOW_ENABLE_MODE        4U
#define MHL_VBUS_HIGH_ENABLE_MODE       8U

#define MHL_SCALER_MUX_SELECT_DVI       0x1U
#define MHL_SCALER_MUX_SELECT_MHL       0xDU

#define MHL_CBUS_CONNECT_CHECK_VALUE    0xC8U

#define MHL_CLOCK_THRESHOLD_24BIT_MODE  50U
#define MHL_CLOCK_THRESHOLD_PP_MODE0    37U
#define MHL_CLOCK_THRESHOLD_PP_MODE1    62U

#define MHL_ECBUS_AUTO_TRAINING         0

#define MHL_ECBUS_COMMAND_PARSING       0
#define MHL_BUFFER_ECBUS_DATA_LENGTH    800

#define MHL_EFUSE_FUNCTION_CONTROL      0

#define MHL_TMDS_MULTIPLE_ENGINE        1
#define MHL_INTERRUPT_USE_PM_IRQ        0
#define MHL_ECBUS_ENGINE_SUPPORT        1

#define MHL_CHIP_CAPABILITY0            MHL_TMDS_MULTIPLE_ENGINE
#define MHL_CHIP_CAPABILITY1            MHL_INTERRUPT_USE_PM_IRQ
#define MHL_CHIP_CAPABILITY2            MHL_ECBUS_ENGINE_SUPPORT
#define MHL_CHIP_CAPABILITY3            0
#define MHL_CHIP_CAPABILITY4            0
#define MHL_CHIP_CAPABILITY5            0
#define MHL_CHIP_CAPABILITY6            0
#define MHL_CHIP_CAPABILITY7            0

#define MHL_CHIP_FUNCTION_CAPABILITY_L  (MHL_CHIP_CAPABILITY0 |(MHL_CHIP_CAPABILITY1 << 1) |(MHL_CHIP_CAPABILITY2 << 2) |(MHL_CHIP_CAPABILITY3 << 3))
#define MHL_CHIP_FUNCTION_CAPABILITY_H  (MHL_CHIP_CAPABILITY4 |(MHL_CHIP_CAPABILITY5 << 1) |(MHL_CHIP_CAPABILITY6 << 2) |(MHL_CHIP_CAPABILITY7 << 3))

#define MHL_CHIP_FUNCTION_CAPABILITY    (MHL_CHIP_FUNCTION_CAPABILITY_L |(MHL_CHIP_FUNCTION_CAPABILITY_H << 4))

enum MHL_DVI_PORT_TYPE
{
    MHL_DVI_PORT_A = 0,
    MHL_DVI_PORT_B,
    MHL_DVI_PORT_C,
    MHL_DVI_PORT_D,
};

enum MHL_CBUS_SELECT_TYPE
{
    MHL_CBUS_SELECT_PORTA = 0,
    MHL_CBUS_SELECT_PORTB,
    MHL_CBUS_SELECT_PORTC,
    MHL_CBUS_SELECT_PORTD,
    MHL_CBUS_SELECT_MASK,
};

enum MHL_PATH_SUPPORT_TYPE
{
    MHL_PATH_SUPPORT_PORTA_FLAG = 0x01,
    MHL_PATH_SUPPORT_PORTB_FLAG = 0x02,
    MHL_PATH_SUPPORT_PORTC_FLAG = 0x04,
    MHL_PATH_SUPPORT_PORTD_FLAG = 0x08,
};

enum MHL_CABLE_DETECT_SELECT_TYPE
{
    MHL_CABLE_DETECT_SELECT_PORTA = 0x01,
    MHL_CABLE_DETECT_SELECT_PORTB = 0x02,
    MHL_CABLE_DETECT_SELECT_PORTC = 0x04,
    MHL_CABLE_DETECT_SELECT_PORTD = 0x08,
};

enum MHL_DVI_MUX_SELECT_TYPE
{
    MHL_DVI_MUX_SELECT0 = 0,
    MHL_DVI_MUX_SELECT1,
};

#define MHL_CBUS_SELECT_PORT_INITIAL    MHL_CBUS_SELECT_PORTC

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
extern MS_U8 ucMHLSupportPort;

#define GET_MHL_PATH_SUPPORT_PORTA()    (MS_BOOL)((ucMHLSupportPort & MHL_PATH_SUPPORT_PORTA_FLAG) ?TRUE :FALSE)
#define SET_MHL_PATH_SUPPORT_PORTA()    (ucMHLSupportPort |= MHL_PATH_SUPPORT_PORTA_FLAG)
#define CLR_MHL_PATH_SUPPORT_PORTA()    (ucMHLSupportPort &= ~MHL_PATH_SUPPORT_PORTA_FLAG)

#define GET_MHL_PATH_SUPPORT_PORTB()    (MS_BOOL)((ucMHLSupportPort & MHL_PATH_SUPPORT_PORTB_FLAG) ?TRUE :FALSE)
#define SET_MHL_PATH_SUPPORT_PORTB()    (ucMHLSupportPort |= MHL_PATH_SUPPORT_PORTB_FLAG)
#define CLR_MHL_PATH_SUPPORT_PORTB()    (ucMHLSupportPort &= ~MHL_PATH_SUPPORT_PORTB_FLAG)

#define GET_MHL_PATH_SUPPORT_PORTC()    (MS_BOOL)((ucMHLSupportPort & MHL_PATH_SUPPORT_PORTC_FLAG) ?TRUE :FALSE)
#define SET_MHL_PATH_SUPPORT_PORTC()    (ucMHLSupportPort |= MHL_PATH_SUPPORT_PORTC_FLAG)
#define CLR_MHL_PATH_SUPPORT_PORTC()    (ucMHLSupportPort &= ~MHL_PATH_SUPPORT_PORTC_FLAG)

#define GET_MHL_PATH_SUPPORT_PORTD()    (MS_BOOL)((ucMHLSupportPort & MHL_PATH_SUPPORT_PORTD_FLAG) ?TRUE :FALSE)
#define SET_MHL_PATH_SUPPORT_PORTD()    (ucMHLSupportPort |= MHL_PATH_SUPPORT_PORTD_FLAG)
#define CLR_MHL_PATH_SUPPORT_PORTD()    (ucMHLSupportPort &= ~MHL_PATH_SUPPORT_PORTD_FLAG)

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
MS_U8 mhal_mhl_GetCbusSelect(MS_U8 ucPort);
MS_U8 mhal_mhl_GetInputPort(void);
MS_BOOL mhal_mhl_CheckInputPort(MS_U8 ucCbusSelect);
MS_BOOL mhal_mhl_CheckPIPWindow(void);
void mhal_mhl_MHLSupportPath(MS_U8 ucSelect);
MS_BOOL mhal_mhl_CheckEfuseControlFlag(MS_BOOL bEfuseFlag);
void mhal_mhl_InsertChipIDforCheck(MS_U8 ucChipID);
void mhal_mhl_CbusSelectSetMux(MS_U8 ucCbusSelect);
MS_BOOL mhal_mhl_CableDetect(MS_U8 ucCbusSelect);
void mhal_mhl_Accumulator_Clr(MS_U8 ucCbusSelect);
MS_BOOL mhal_mhl_CDRModeMonitor(MS_U8 ucCbusSelect, MS_BOOL bPathEnable);
void mhal_mhl_CbusIsolate(MS_U8 ucCbusSelect, MS_BOOL bFlag);
void mhal_mhl_VbusCharge(MS_U8 ucCbusSelect, MS_U8 bState);
void mhal_mhl_CbusFloating(MS_BOOL bFlag);
void mhal_mhl_CbusStucktoLow(MS_BOOL bFlag);
void mhal_mhl_CbusWakeupInterrupt(MS_BOOL bFlag);
void mhal_mhl_SetVenderID(MS_U8 ucVenderID);
void mhal_mhl_LoadEDID(MS_U8 *edid);
void mhal_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid);
void mhal_mhl_LoadDeviceCapability(MS_U8 *devcap);
MS_U8 mhal_mhl_initial(MS_U8 *edid, MS_U8 *devcap, MS_U8 ucVenderID);
void mhal_mhl_InvertCableDetect(MS_U8 ucCbusSelect, MS_BOOL bCableDetectInvert);
void mhal_mhl_VbusConfigSetting(MS_U8 ucCbusSelect, MS_U8 ucState);
void mhal_mhl_CableDetectPadSetting(MS_U8 ucSelect);
void mhal_mhl_CbusPadConfigSwitch(MS_U8 ucCbusSelect, MS_BOOL bFlag);
MS_U16 mhal_mhl_CbusStatus(void);

MS_BOOL mhal_mhl_CbusIsMscMsgReceived(void);
MS_BOOL mhal_mhl_CbusStucktoLowFlag(void);
MS_BOOL mhal_mhl_CbusWakeupIntFlag(void);
MS_BOOL mhal_mhl_GetECbusStateChangeFlag(void);
MS_BOOL mhal_mhl_GetEMSCReceiveFlag(void);
MS_U8 mhal_mhl_GetEMSCSendStatus(void);
MS_BOOL mhal_mhl_CBusWrite(mhalCbusFifo_S *pdatabuf);
void mhal_mhl_Cbus_SetPathEn(MS_BOOL bflag);
MS_BOOL mhal_mhl_CbusIntCB(MS_U8 *rcstate, MS_U8 *rccmd, MS_U8 *rcdata, MS_U8 *rclen, MS_U8 *bIsCmdInData);
MS_BOOL mhal_mhl_CBusCheckBCHError(void);

void mhal_mhl_CablePlugProc(MS_U8 ucCbusSelect);
void mhal_mhl_CableRemoveProc(MS_U8 ucCbusSelect);
void mhal_mhl_CbusConnectProc(MS_U8 ucCbusSelect);
void mhal_mhl_CbusStucktoLowProc(MS_U8 ucCbusSelect);
void mhal_mhl_SourceChangeProc(MS_U8 ucCbusSelect);
void mhal_mhl_ClockModeSwitchProc(MS_U8 ucCbusSelect, MS_BOOL bPPmode);
void mhal_mhl_CbusWakeupIntSetting(MS_U8 ucCbusSelect, MS_BOOL bFlag);
void mhal_mhl_RtermControlHWMode(MS_U8 ucCbusSelect, MS_BOOL bFlag);
void mhal_mhl_AdjustSettingIControl(MS_U8 ucIControl);
void mhal_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance);
MS_BOOL mhal_mhl_GetSignalStableFlag(void);

void mhal_mhl_LoadPowerOnTbl(void);
void mhal_mhl_LoadPowerStandbyTbl(void);
void mhal_mhl_LoadPowerDownTbl(void);
void mhal_mhl_SetHPD(MS_BOOL bflag);
MS_U8 mhal_mhl_GetDDCErrorCode(void);

#if(MHL_CBUS_OPERATION_MODE == MHL_CBUS_HW_REPLY_MODE)
MS_BOOL mhal_mhl_CheckSRAMReceiveBuffer(void);
MS_U16 mhal_mhl_GetSRAMReceiveData(void);
#endif

#if(DMHL_TEST_SIGNAL_SUPPORT)
void mhal_mhl_TestSignal(MS_BOOL bflag);
#endif

void mhal_mhl_SetMainLinkRate(MS_U8 ucCbusSelect, MS_U8 ucLinkRate);
MS_U8 mhal_mhl_GetECbusStatusFlag(void);
void mhal_mhl_ECbusEventProc(MS_U8 ucCbusSelect, MS_U8 ucECbusEvent);
void mhal_mhl_GetEMSCReceiveData(MS_U8 *bReveiceEMSC);
void mhal_mhl_InsertEMSCSendData(MS_U8 ucLength, MS_U8 *bSendEMSC);

#endif //_MHAL_MHL_H_

