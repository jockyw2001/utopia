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
/// @file   Mapi_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
    1. Check whether MHL cable is plugged or not in Rx side
    2. Disable R-term and output Vbus
    3. Check whether Cbus is connected(discovery pulse completed)
    4. Sink -> Source: "SET_HPD"
    5. Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    6. Sink -> Source: Write_Stat - "PATH_EN"
*/


#ifndef _API_MHL_PRIVATE_H_
#define _API_MHL_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#include "mdrv_mhl_st.h"
#include "apiMHL.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _MHL_RESOURCE_PRIVATE
{
    MS_U8 ucInitialIndex;
    MS_U8 ucMHLSupportPath;
    MS_U8 ucCurrentHDMIPort;
    MS_U8 ucCurrentQueueIndex;
    MS_U8 ucChipCapability;
    MS_U8 ucDeviceVenderID;
    MS_U8 ucTMDSClockMode;
    MS_U8 ucAskData;
    MS_U8 ucMSGData[2];
    MS_U8 ucReceiveCounter;
    MS_U8 ucEMSCReceiveCounter;
    MS_U8 ucSourceVenderID;
    MS_U8 ucProcDoneIndex;
    MS_U8 ucWriteBurstState;
    MS_U8 ucFeatureRequestState;
    MS_U8 ucECbusState;
    MS_U8 ucECbusTrainState;
    MS_U8 ucBISTModeState;
    MS_U8 ucPreRCPKeyCode;
    MS_U8 ucWriteBurstSequence;
    MS_U8 uc3DTotalEntryDTD;
    MS_U8 uc3DTotalEntryVIC;
    MS_U8 ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE];
    MS_U8 ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE];
    MS_U8 uc3DInformationDTD[MHL_3D_DTD_INFORMATION_LENGTH];
    MS_U8 uc3DInformationVIC[MHL_3D_VIC_INFORMATION_LENGTH];
    MS_U8 ucWriteBurstAddress;
    MS_U8 ucWriteBurstLength;
    MS_U8 ucWriteBurstInformation[MHL_MSC_SCRATCHPAD_SIZE +1];
    MS_U8 ucEMSCSendStatus;
    MS_U8 ucCallBackFunctionIndex;
    MS_U8 ucMHLPollingStack[MHL_POLLING_STACK_SIZE];
    MS_U8 ucPowerOnPatchState;
    MS_U8 ucHighEndVICTotalEntry;
    MS_U16 usDisplayIndex;
    MS_U16 usDeviceIndex;
    MS_U16 usEMSCRxBufferSize;
    MS_U16 usEMSCTxBufferSize;
    MS_U16 usReadDevcapMask;
    MS_U16 usReadExtendDevcapMask;
    MS_U16 usMSGKeyInfo;
    MS_U16 usHighEndVICTable[MHL_HEV_VIC_INFORMATION_LENGTH];
    MS_U16 usEMSCSupportTable[MHL_EMSC_SUPPORT_INFORMATION_LENGTH];
    MS_U32 ulCurrentTimerTick;
    MS_U32 ulSystemIndex;
    MS_U32 ulTimerIndex;
    MS_U32 ulCbusReceiveIndex;
    MS_U32 ulCbusSendIndex;
    MS_U32 ulEnableIndex;
    MS_U32 ulAudioTunnelTable[MHL_AUDIO_TUNNEL_INFORMATION_LENGTH];
    MS_S32 slMHLPollingTaskID;
    MS_VIRT ulRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulRiuBaseAddressDummy;
#endif
    MS_VIRT ulPMRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulPMRiuBaseAddressDummy;
#endif
    EN_POWER_MODE usPrePowerState;
    stMHL_TIMER_EVENT_INFO stTimerEventInfo[MHL_TIMER_EVENT_MASK];
    stMHL_QUEUE_INFO stCbusQueueInfo[MHL_CBUS_TOTAL_QUEUE_NUM];
    stMHL_VIRTUAL_CHANNEL_INFO stVirtualChannelInfo[MHL_VIRTUAL_CHANNEL_LENGTH];
    stMHL_BIST_MODE_INFO stBISTParameterInfo;
    stMHL_SIGNAL_STATUS_INFO stSignalStatusInfo;
}MHL_RESOURCE_PRIVATE;

typedef void    (*IOCTL_MHL_SUPPORT_PATH)           (void*, MS_U8);
typedef void    (*IOCTL_MHL_INITIAL)                 (void*, MS_U8*, MS_U8*);
typedef MS_U16  (*IOCTL_MHL_HANDLER)                 (void*);
typedef MS_BOOL (*IOCTL_MHL_AUTO_SWITCH)            (void*, MS_BOOL, MS_U8*);
typedef MS_U32  (*IOCTL_MHL_SET_POWER_STATE)        (void*, EN_POWER_MODE);
typedef void    (*IOCTL_MHL_CBUS_CONTROL)            (void*, MS_U8);
typedef void    (*IOCTL_MHL_LOAD_EDID)               (void*, MS_U8*);
typedef void    (*IOCTL_MHL_READ_EDID)               (void*, MS_U8, MS_U8*);
typedef void    (*IOCTL_MHL_LOAD_DEVCAP)             (void*, MS_U8*);
typedef void    (*IOCTL_MHL_SET_VENDER_ID)           (void*, MS_U8);
typedef void    (*IOCTL_MHL_CABLE_DETECT_INVERT)    (void*, MS_BOOL);
typedef void    (*IOCTL_MHL_VBUS_CONFIG)             (void*, MS_U8);
typedef void    (*IOCTL_MHL_ADJUST_I_CONTROL)        (void*, MS_U8);
typedef void    (*IOCTL_MHL_ADJUST_IMPEDANCE)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_CABLE_DETECT_FLAG)       (void*);
typedef MS_BOOL (*IOCTL_MHL_CBUS_CONNECT_FLAG)       (void*);
typedef MS_BOOL (*IOCTL_MHL_CBUS_WAKEUP_FLAG)        (void*);
typedef MS_BOOL (*IOCTL_MHL_SOURCE_SUPPORT_RCP)      (void*);
typedef MS_BOOL (*IOCTL_MHL_SOURCE_SUPPORT_RAP)      (void*);
typedef MS_BOOL (*IOCTL_MHL_GET_STATUS_FLAG)         (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_RAP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_RCP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_UCP_COMMAND)        (void*, MS_U8);
typedef MS_BOOL (*IOCTL_MHL_SEND_WRITE_BURST)        (void*, MS_U8, MS_U8, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_SEND_USER_WRITE_BURST)  (void*, MS_U8, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_DEVCAP)               (void*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_EXTEND_DEVCAP)       (void*, MS_U16, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_VENDER_ID)           (void*, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_GET_WRITE_BURST_DATA)   (void*, MS_U8*);
typedef MS_BOOL (*IOCTL_MHL_CALL_BACK_FUNCTION)      (void*, MS_BOOL, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RCP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RAP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_UCP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_ATT_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef MS_BOOL (*IOCTL_MHL_RBP_CALL_BACK_FUNCTION)  (void*, MS_BOOL);
typedef stMHL_INITIAL_CONFIG_INFO  (*IOCTL_MHL_GET_CONFIGURATION)        (void*);

typedef struct _MHL_INSTANT_PRIVATE
{
    MS_U32                              ulDeviceID;
    IOCTL_MHL_SUPPORT_PATH              fpMHLSupportPath;
    IOCTL_MHL_INITIAL                   fpMHLInitial;
    IOCTL_MHL_HANDLER                   fpMHLHandler;
    IOCTL_MHL_AUTO_SWITCH               fpMHLAutoSwitchHandler;
    IOCTL_MHL_SET_POWER_STATE           fpMHLSetPowerState;
    IOCTL_MHL_CBUS_CONTROL              fpMHLCbusControl;
    IOCTL_MHL_LOAD_EDID                 fpMHLLoadEDID;
    IOCTL_MHL_READ_EDID                 fpMHLReadEDID;
    IOCTL_MHL_LOAD_DEVCAP               fpMHLLoadDeviceCapability;
    IOCTL_MHL_SET_VENDER_ID             fpMHLSetVenderID;
    IOCTL_MHL_CABLE_DETECT_INVERT       fpMHLCableDetectInvert;
    IOCTL_MHL_VBUS_CONFIG               fpMHLVbusConfigSetting;
    IOCTL_MHL_ADJUST_I_CONTROL          fpMHLAdjustIControl;
    IOCTL_MHL_ADJUST_IMPEDANCE          fpMHLAdjustImpedance;
    IOCTL_MHL_CABLE_DETECT_FLAG         fpMHLCableDetectFlag;
    IOCTL_MHL_CBUS_CONNECT_FLAG         fpMHLCbusConnectFlag;
    IOCTL_MHL_CBUS_WAKEUP_FLAG          fpMHLCbusWakeupIntFlag;
    IOCTL_MHL_SOURCE_SUPPORT_RCP        fpMHLSourceSupportRCP;
    IOCTL_MHL_SOURCE_SUPPORT_RAP        fpMHLSourceSupportRAP;
    IOCTL_MHL_GET_STATUS_FLAG           fpMHLGetStatusFlag;
    IOCTL_MHL_SEND_RAP_COMMAND          fpMHLSendRAPCommand;
    IOCTL_MHL_SEND_RCP_COMMAND          fpMHLSendRCPCommand;
    IOCTL_MHL_SEND_UCP_COMMAND          fpMHLSendUCPCommand;
    IOCTL_MHL_SEND_WRITE_BURST          fpMHLSendWriteBurst;
    IOCTL_MHL_SEND_USER_WRITE_BURST     fpMHLSendUserWriteBurst;
    IOCTL_MHL_GET_DEVCAP                fpMHLGetDeviceCapability;
    IOCTL_MHL_GET_EXTEND_DEVCAP         fpMHLGetExtendDeviceCapability;
    IOCTL_MHL_GET_VENDER_ID             fpMHLGetVenderID;
    IOCTL_MHL_GET_WRITE_BURST_DATA      fpMHLGetWriteBurstData;
    IOCTL_MHL_CALL_BACK_FUNCTION        fpMHLRegisterCallBackfunction;
    IOCTL_MHL_RCP_CALL_BACK_FUNCTION    fpMHLRegisterRCPCallBackfunction;
    IOCTL_MHL_RAP_CALL_BACK_FUNCTION    fpMHLRegisterRAPCallBackfunction;
    IOCTL_MHL_UCP_CALL_BACK_FUNCTION    fpMHLRegisterUCPCallBackfunction;
    IOCTL_MHL_ATT_CALL_BACK_FUNCTION    fpMHLRegisterATTCallBackfunction;
    IOCTL_MHL_RBP_CALL_BACK_FUNCTION    fpMHLRegisterRBPCallBackfunction;
    IOCTL_MHL_GET_CONFIGURATION         fpMHLGetConfiguration;
}MHL_INSTANT_PRIVATE;

typedef enum
{
    MHL_POOL = 0,
    MHL_POOL_MAX,
}eMHLPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif //_API_MHL_PRIVATE_H_
