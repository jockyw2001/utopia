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
/// @file   apiCEC_private.h
/// @author MStar Semiconductor Inc.
/// @brief  CEC driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
    1. Check whether MHL cable is plugged or not in Rx side
    2. Disable R-term and output Vbus
    3. Check whether Cbus is connected(discovery pulse completed)
    4. Sink -> Source: "SET_HPD"
    5. Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    6. Sink -> Source: Write_Stat - "PATH_EN"
*/


#ifndef _API_CEC_PRIVATE_H_
#define _API_CEC_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"
#include "apiCEC.h"


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
typedef struct __attribute__ ((packed)) _CEC_RESOURCE_PRIVATE
{
    MS_BOOL bCECIsSource;
    MS_BOOL bRIUAddrReady;
    MS_BOOL bCecInitFinish;
    MS_BOOL bCecISRAttach;
    MS_BOOL CecDevicesExisted[CEC_DEVICE_EXIST_COUNT];
    MS_BOOL bAttachDrvISR;
    MS_U8 ucRetryCount;
    MS_U8 ucInitialIndex;
    MS_U8 CecFifoIdxS;
    MS_U8 CecFifoIdxE;
    MS_U8 bCecMsgCnt;
    MS_U8 ActiveDeviceCECVersion;
    MS_U8 MyPhysicalAddress[CEC_PHY_ADDR_COUNT];
    MS_U8 ActivePhysicalAddress[CEC_PHY_ADDR_COUNT];
    MS_U8 ucVendorID[CEC_VENDOR_ID_COUNT];
    MS_U32 ulXTAL_CLK_Hz;
    MS_VIRT ulXCRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulXCRiuBaseAddressDummy;
#endif
    MS_VIRT ulPMRiuBaseAddress;
#if !defined (__aarch64__)
    MS_U32 ulPMRiuBaseAddressDummy;
#endif
    MsCEC_DEVICELA MyLogicalAddress;                         //TV related
    MsCEC_DEVICE_TYPE MyDeviceType;
    MsCEC_MSG_POWER_STATUS_PARM MyPowerStatus;
    MsCEC_DEVICE_TYPE ActiveDeviceType;
    MsCEC_DEVICELA ActiveLogicalAddress;    //the remoter controller's active device
    MsCEC_MSG_POWER_STATUS_PARM ActivePowerStatus;
    EN_POWER_MODE usPrePowerState;
    MS_CEC_RxData_Info CecRxBuf[CEC_FIFO_NUM_COUNT];   //TV CEC H/W part

#if ENABLE_CEC_MULTIPLE
    MS_U8 MyPhysicalAddress2[CEC_PHY_ADDR_COUNT];
    MsCEC_DEVICELA MyLogicalAddress2;
    MsCEC_DEVICE_TYPE MyDeviceType2;
    MsCEC_DEVICE_TYPE IniLogicalAddress;
#endif
}CEC_RESOURCE_PRIVATE;

typedef MS_BOOL           (*IOCTL_DDC2BI_GET_INFO)                    (void*, MS_CEC_INFO_LIST*);
typedef void              (*IOCTL_CEC_INIT_CHIP)                       (void*, MS_U32);
typedef void              (*IOCTL_CEC_PORT_SELECT)                    (void*, MsCEC_INPUT_PORT);
typedef void              (*IOCTL_CEC_EXIT)                             (void*);
typedef void              (*IOCTL_CEC_SET_MY_LOGICAL_ADDRESS)       (void*, MsCEC_DEVICELA);
typedef void              (*IOCTL_CEC_INIT)                             (void*, MS_U32);
typedef void              (*IOCTL_CEC_CHECK_EXIST_DEVICES)          (void*);
typedef MsCEC_DEVICELA    (*IOCTL_CEC_NEXT_DEVICE)                    (void*);
typedef void              (*IOCTL_CEC_CHK_RX_BUF)                      (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_MSG)                    (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MS_U8*, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_MSG2)                   (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MS_U8*, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_TX_SEND_POLLING_MSG)          (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ACTIVE_SOURCE)             (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ROUTING_CHANGE)            (void*, MS_U8*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ACTIVE_SOURCE)        (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_SET_STREAM_PATH)           (void*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_STANDBY)                     (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_RECORD_OFF)                 (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_RECORD_ON)                  (void*, MsCEC_DEVICELA, MsCEC_MSG_RECORD_SOURCE_TYPE);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_CEC_VERSION)       (void*, MsCEC_DEVICELA, MS_U8);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_CEC_VERSION)           (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS)    (void*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_PHYCAL_ADDRESS)        (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DECK_CONTROL)               (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_DECK_CTRL_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DEC_STATUS)                 (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_DECK_STATUS_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_GIVE_DECK_STATUS)          (void*, MsCEC_DEVICELA, MsCEC_MSG_STATUS_REQUEST_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_DC_PLAY)                     (void*, MsCEC_DEVICELA, MsCEC_MSG_DC_PLAY_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_MENU_STATUS)           (void*, MsCEC_DEVICELA, MsCEC_MSG_MENU_REQ_TYPE);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_USER_CTRL_PRESSED)         (void*, MS_BOOL, MsCEC_DEVICELA, MsCEC_MSG_USER_CTRL_PARM);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_USER_CTRL_RELEASED)        (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_GIVE_AUDIO_STATUS)         (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_POWER_STATUS)       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_POWER_STATUS)          (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_FEATURE_ABORT)              (void*, MsCEC_DEVICELA, MsCEC_MSGLIST, MsCEC_MSG_ABORT_REASON);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_ABORT)                       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_SEND_MENU_LANGUAGE)       (void*, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ARC_INITIATION)       (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REQ_ARC_TERMINATION)      (void*, MsCEC_DEVICELA);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_AUDIO_MODE_REQ)            (void*, MsCEC_DEVICELA, MS_BOOL, MS_U8*);
typedef CEC_ERROR_CODE    (*IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS2)   (void*);
typedef MS_BOOL           (*IOCTL_CEC_CHECK_FRAME)                      (void*, MsCEC_MSG_TRANS_TYPE, MS_U8);
typedef void              (*IOCTL_CEC_CONFIG_WAKE_UP)                  (void*);
typedef void              (*IOCTL_CEC_ENABLED)                          (void*, MS_BOOL);
typedef MS_U8             (*IOCTL_CEC_GET_TX_STATUS)                   (void*);
typedef MS_BOOL           (*IOCTL_CEC_DEVICE_IS_TX)                     (void*);
typedef MS_U32            (*IOCTL_CEC_SET_POWER_STATE)                 (void*, EN_POWER_MODE);
typedef void              (*IOCTL_CEC_SET_MY_LOGICAL_ADDRESS2)       (void*, MsCEC_DEVICELA);
typedef MS_U8             (*IOCTL_CEC_GET_HEADER)                       (void*);
typedef MS_U8             (*IOCTL_CEC_GET_OPCODE)                       (void*);
typedef MS_U8             (*IOCTL_CEC_GET_PARA)                          (void*, MS_U8);
typedef MS_U8             (*IOCTL_CEC_GET_CMD_LEN)                      (void*);
typedef MS_BOOL           (*IOCTL_CEC_IS_RX_BUF_EMPTY)                 (void*);
typedef void              (*IOCTL_CEC_SET_ACTIVE_LOGICAL_ADDRESS)   (void*, MsCEC_DEVICELA);
typedef MsCEC_DEVICELA    (*IOCTL_CEC_GET_ACTIVE_LOGICAL_ADDRESS)   (void*);
typedef MsCEC_MSG_POWER_STATUS_PARM    (*IOCTL_CEC_GET_POWER_STATUS)   (void*);
typedef MS_U8             (*IOCTL_CEC_GET_FIFO_IDX)                     (void*);
typedef void              (*IOCTL_CEC_SET_FIFO_IDX)                     (void*, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_POWER_STATUS)       (void*, MsCEC_MSG_POWER_STATUS_PARM);
typedef MsCEC_MSG_POWER_STATUS_PARM    (*IOCTL_CEC_GET_ACTIVE_POWER_STATUS)       (void*);
typedef void              (*IOCTL_CEC_SET_ACTIVE_PHYSICAL_ADDRESS)   (void*, MS_U8, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_DEVICE_CEC_VERSION) (void*, MS_U8);
typedef void              (*IOCTL_CEC_SET_ACTIVE_DEVICE_TYPE)          (void*, MsCEC_DEVICE_TYPE);
typedef MS_U8             (*IOCTL_CEC_GET_MSG_CNT)                       (void*);
typedef void              (*IOCTL_CEC_SET_MSG_CNT)                       (void*, MS_U8);
typedef MS_U8             (*IOCTL_CEC_GET_RX_DATA)                       (void*, MS_U8, MS_U8);
typedef void              (*IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS)        (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS2)       (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_INITIATOR)                     (void*, MsCEC_DEVICELA);
typedef void              (*IOCTL_CEC_CONFIG_WAKEUP_INFO_VENDOR_ID)  (void*, MS_U8*);
typedef void              (*IOCTL_CEC_SET_RETRY_COUNT)                  (void*, MS_U8);
typedef void              (*IOCTL_CEC_ATTACH_DRIVER_ISR)               (void*, MS_BOOL);
typedef stCEC_INITIAL_CONFIG_INFO     (*IOCTL_CEC_GET_CONFIGURATION)    (void*);

typedef struct _CEC_INSTANT_PRIVATE
{
    MS_U32			                         ulDeviceID;
    IOCTL_DDC2BI_GET_INFO                    fpDDC2BIGetInfo;
    IOCTL_CEC_INIT_CHIP        	             fpCECInitChip;
    IOCTL_CEC_PORT_SELECT                    fpCECPortSelect;
    IOCTL_CEC_EXIT                           fpCECExit;
    IOCTL_CEC_SET_MY_LOGICAL_ADDRESS         fpCECSetMyLogicalAddress;
    IOCTL_CEC_INIT           	             fpCECInit;
    IOCTL_CEC_CHECK_EXIST_DEVICES            fpCECCheckExistDevices;
    IOCTL_CEC_NEXT_DEVICE                    fpCECNextDevice;
    IOCTL_CEC_CHK_RX_BUF                     fpCECChkRxBuf;
    IOCTL_CEC_TX_SEND_MSG                    fpCECTxSendMsg;
    IOCTL_CEC_TX_SEND_MSG2                   fpCECTxSendMsg2;
    IOCTL_CEC_TX_SEND_POLLING_MSG            fpCECTxSendPollingMsg;
    IOCTL_CEC_MSG_ACTIVE_SOURCE              fpCECMsgActiveSource;
    IOCTL_CEC_MSG_ROUTING_CHANGE             fpCECMsgRoutingChange;
    IOCTL_CEC_MSG_REQ_ACTIVE_SOURCE          fpCECMsgReqActiveSource;
    IOCTL_CEC_MSG_SET_STREAM_PATH            fpCECMsgSetStreamPath;
    IOCTL_CEC_MSG_STANDBY                    fpCECMsgStandby;
    IOCTL_CEC_MSG_RECORD_OFF                 fpCECMsgRecordOff;
    IOCTL_CEC_MSG_RECORD_ON                  fpCECMsgRecordOn;
    IOCTL_CEC_MSG_REPORT_CEC_VERSION         fpCECMsgReportCECVersion;
    IOCTL_CEC_MSG_REQ_CEC_VERSION            fpCECMsgReqCECVersion;
    IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS      fpCECMsgReportPhycalAddress;
    IOCTL_CEC_MSG_REQ_PHYCAL_ADDRESS         fpCECMsgReqPhycalAddress;
    IOCTL_CEC_MSG_DECK_CONTROL               fpCECMsgDeckControl;
    IOCTL_CEC_MSG_DEC_STATUS                 fpCECMsgDecStatus;
    IOCTL_CEC_MSG_GIVE_DECK_STATUS           fpCECMsgGiveDeckStatus;
    IOCTL_CEC_MSG_DC_PLAY                    fpCECMsgDCPlay;
    IOCTL_CEC_MSG_REQ_MENU_STATUS            fpCECMsgReqMenuStatus;
    IOCTL_CEC_MSG_USER_CTRL_PRESSED          fpCECMsgUserCtrlPressed;
    IOCTL_CEC_MSG_USER_CTRL_RELEASED         fpCECMsgUserCtrlReleased;
    IOCTL_CEC_MSG_GIVE_AUDIO_STATUS          fpCECMsgGiveAudioStatus;
    IOCTL_CEC_MSG_REPORT_POWER_STATUS        fpCECMsgReportPowerStatus;
    IOCTL_CEC_MSG_REQ_POWER_STATUS           fpCECMsgReqPowerStatus;
    IOCTL_CEC_MSG_FEATURE_ABORT              fpCECMsgFeatureAbort;
    IOCTL_CEC_MSG_ABORT                      fpCECMsgAbort;
    IOCTL_CEC_MSG_SEND_MENU_LANGUAGE         fpCECMsgSendMenuLanguage;
    IOCTL_CEC_MSG_REQ_ARC_INITIATION         fpCECMsgReqARCInitiation;
    IOCTL_CEC_MSG_REQ_ARC_TERMINATION        fpCECMsgReqARCTermination;
    IOCTL_CEC_MSG_AUDIO_MODE_REQ             fpCECMsgAudioModeReq;
    IOCTL_CEC_MSG_REPORT_PHYCAL_ADDRESS2     fpCECMsgReportPhycalAddress2;
    IOCTL_CEC_CHECK_FRAME                    fpCECCheckFrame;
    IOCTL_CEC_CONFIG_WAKE_UP                 fpCECConfigWakeUp;
    IOCTL_CEC_ENABLED                        fpCECEnabled;
    IOCTL_CEC_GET_TX_STATUS                  fpCECGetTxStatus;
    IOCTL_CEC_DEVICE_IS_TX                   fpCECDeviceIsTx;
    IOCTL_CEC_SET_POWER_STATE                fpCECSetPowerState;
    IOCTL_CEC_SET_MY_LOGICAL_ADDRESS2        fpCECSetMyLogicalAddress2;
    IOCTL_CEC_GET_HEADER                     fpCECGetHeader;
    IOCTL_CEC_GET_OPCODE                     fpCECGetOpCode;
    IOCTL_CEC_GET_PARA                       fpCECGetPara;
    IOCTL_CEC_GET_CMD_LEN                    fpCECGetCmdLen;
    IOCTL_CEC_IS_RX_BUF_EMPTY                fpCECIsRxBufEmpty;
    IOCTL_CEC_SET_ACTIVE_LOGICAL_ADDRESS     fpCECSetActiveLogicalAddress;
    IOCTL_CEC_GET_ACTIVE_LOGICAL_ADDRESS     fpCECGetActiveLogicalAddress;
    IOCTL_CEC_GET_POWER_STATUS               fpCECGetPowerStatus;
    IOCTL_CEC_GET_FIFO_IDX                   fpCECGetFifoIdx;
    IOCTL_CEC_SET_FIFO_IDX                   fpCECSetFifoIdx;
    IOCTL_CEC_SET_ACTIVE_POWER_STATUS        fpCECSetActivePowerStatus;
    IOCTL_CEC_GET_ACTIVE_POWER_STATUS        fpCECGetActivePowerStatus;
    IOCTL_CEC_SET_ACTIVE_PHYSICAL_ADDRESS    fpCECSetActivePhysicalAddress;
    IOCTL_CEC_SET_ACTIVE_DEVICE_CEC_VERSION  fpCECSetActiveDeviceCECVersion;
    IOCTL_CEC_SET_ACTIVE_DEVICE_TYPE         fpCECSetActiveDeviceType;
    IOCTL_CEC_GET_MSG_CNT                    fpCECGetMsgCnt;
    IOCTL_CEC_SET_MSG_CNT                    fpCECSetMsgCnt;
    IOCTL_CEC_GET_RX_DATA                    fpCECGetRxData;
    IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS        fpCECSetMyPhysicalAddress;
    IOCTL_CEC_SET_MY_PHYSICAL_ADDRESS2       fpCECSetMyPhysicalAddress2;
    IOCTL_CEC_SET_INITIATOR                  fpCECSetInitiator;
    IOCTL_CEC_CONFIG_WAKEUP_INFO_VENDOR_ID   fpCECConfigWakeUpInfoVendorID;
    IOCTL_CEC_SET_RETRY_COUNT                fpCECSetRetryCount;
    IOCTL_CEC_ATTACH_DRIVER_ISR              fpCECAttachDriverISR;
    IOCTL_CEC_GET_CONFIGURATION              fpCECGetConfiguration;
}CEC_INSTANT_PRIVATE;

typedef enum
{
    CEC_POOL = 0,
    CEC_POOL_MAX,
}eCECPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------

#endif //_API_CEC_PRIVATE_H_
