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
/// @file   MapiHDMIRx_private.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _API_HDMI_RX_PRIVATE_H_
#define _API_HDMI_RX_PRIVATE_H_

#include "MsTypes.h"
#include "MsDevice.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "mhal_hdmi.h"
#include "drvXC_HDMI_if.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define GET_HDMI_FLAG(a, b)                 (MS_BOOL)((a &b) ?TRUE :FALSE)
#define SET_HDMI_FLAG(a, b)                 (a |= b)
#define CLR_HDMI_FLAG(a, b)                 (a &= ~b)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMI_HDCP_KEY_LENGTH                289U
#define HDMI_HDCP_BKSV_LENGTH               5U
#define HDMI_HDCP22_RIV_LENGTH              8U
#define HDMI_HDCP22_SESSION_KEY_LENGTH      16U
#define HDMI_HDCP22_MESSAGE_LENGTH          129U
#define HDMI_POLLING_STACK_SIZE             1U
#define HDMI_HDCP_INTERRUPT_STACK_SIZE      1U
#define HDMI_PACKET_RECEIVE_COUNT           5U    // 5 x 10ms = 50ms
#define HDMI_HDCP14_RI_COUNT                550U  // 550 x 10ms = 5.5s
#define HDMI_HDCP_KSV_LIST_LENGTH           635U  // max: 127x5
#define HDMI_HDCP_VPRIME_LENGTH             20U
#define HDMI_HDCP_EVENT_POLLING_STATUS      0x100U
#define HDMI_HDCP_EVENT_RECEIVE_AKE_INIT    0x200U
#define HDMI_HDCP_EVENT_WAIT_MESSAGE        0x300U
#define HDMI_HDCP_EVENT_WRITE_DONE          0x400U
#define HDMI_HDCP_EVENT_STR_SUSPEND         0x500U
#define HDMI_HDCP_EVENT_STR_RESUME          0x600U
#define HDMI_HDCP_EVENT_INDEX_MASK          0xFFFFFF00U
#define HDMI_HDCP_EVENT_STATUS_MASK         0x000000FFU
#define HDMI_HDCP_ALL_EVENT                 0xFFFFFFFFU

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _HDMI_RX_RESOURCE_PRIVATE
{
    MS_BOOL bSelfCreateTaskFlag;
    MS_BOOL bHDMITaskProcFlag;
    MS_BOOL bImmeswitchSupport;
    MS_BOOL bHDCP14KeyVaildFlag;
    MS_BOOL bHPDInvertFlag[4];
    MS_BOOL bHDCPIRQAttachFlag;
    MS_BOOL bHDCP22IRQMaskFlag[4];
    MS_BOOL bHDCP14RiFlag[4];
    MS_BOOL bHDCP14RxREEFlag;
    MS_U8 ucInitialIndex;
    MS_U8 ucMHLSupportPath;
    MS_U8 ucHDCPKeyTable[HDMI_HDCP_KEY_LENGTH];
    MS_U8 ucCheckPacketState[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveInterval[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucReceiveIntervalCount[HDMI_INFO_SOURCE_MAX];
    MS_U8 ucHDMIPollingStack[HDMI_POLLING_STACK_SIZE];
    MS_U8 ucHDCP22RecvIDListSend[4];
    MS_U8 ucHDCPWriteDoneIndex;
    MS_U8 ucHDCPReadDoneIndex;
    MS_U16 usHDCP14RiCount[4];
    MS_U32 ulPacketStatus[HDMI_INFO_SOURCE_MAX];
    MS_S32 slHDMIPollingTaskID;
    MS_S32 slHDMIHDCPEventID;
    EN_POWER_MODE usPrePowerState;
    stHDMI_POLLING_INFO stHDMIPollingInfo[4];
}HDMI_RX_RESOURCE_PRIVATE;

typedef void        (*IOCTL_HDMIRX_INITIAL)                      (void*, stHDMI_INITIAL_TABLE);
typedef void        (*IOCTL_HDMIRX_STABLE_POLLING)              (void*);
typedef MS_U8       (*IOCTL_HDMIRX_GET_SOURCE_VERSION)          (void*, E_MUX_INPUTPORT);
typedef MS_U8       (*IOCTL_HDMIRX_CHECK_HDCP_STATE)            (void*, E_MUX_INPUTPORT);
typedef MS_U8       (*IOCTL_HDMIRX_CHECK_HDCP_ENC_STATE)        (void*, E_MUX_INPUTPORT);
typedef void        (*IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD)      (void*, MS_U8*);
typedef MS_BOOL     (*IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG)  (void*);
typedef void        (*IOCTL_HDMIRX_SET_HPD_INVERT_FLAG)         (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U32      (*IOCTL_HDMIRX_GET_PACKET_RECEIVE)          (void*, MS_U8);
/*************************** HDCP Repeater ***************************/
typedef void        (*IOCTL_HDMIRX_WRITE_X74)                     (void*, E_MUX_INPUTPORT, MS_U8, MS_U8);
typedef MS_U8       (*IOCTL_HDMIRX_READ_X74)                      (void*, E_MUX_INPUTPORT, MS_U8);
typedef void        (*IOCTL_HDMIRX_SET_REPEATER)                  (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef void        (*IOCTL_HDMIRX_SET_BSTATUS)                   (void*, E_MUX_INPUTPORT, MS_U16);
typedef void        (*IOCTL_HDMIRX_SET_HDMI_MODE)                 (void*, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U8       (*IOCTL_HDMIRX_GET_INTERRUPT_STATUS)        (void*, E_MUX_INPUTPORT);
typedef void        (*IOCTL_HDMIRX_WRITE_KSV_LIST)                (void*, E_MUX_INPUTPORT, MS_U8*, MS_U32);
typedef void        (*IOCTL_HDMIRX_SET_VPRIME)                     (void*, E_MUX_INPUTPORT, MS_U8*);
/*************************** HDCP Repeater ***************************/
/*************************** HDCP22 ***************************/
typedef void        (*IOCTL_HDMIRX_FILL_CIPHER_KEY)              (void*, MS_U8, MS_U8*, MS_U8*);
typedef void        (*IOCTL_HDMIRX_PORT_INIT)                     (void*, MS_U8);
typedef MS_BOOL     (*IOCTL_HDMIRX_POLLING_READ_DONE)            (void*, MS_U8);
typedef void        (*IOCTL_HDMIRX_ENABLE_CIPHER)                 (void*, MS_U8, MS_U8, MS_BOOL);
typedef void        (*IOCTL_HDMIRX_SEND_MESSAGE)                  (void*, MS_U8, MS_U8, MS_U8*, MS_U32, void*);
typedef MS_BOOL     (*IOCTL_HDMIRX_HDCP22_HANDER)                 (void*, MS_U8, MS_U8*, MS_U8*, MS_U32*, MS_BOOL);
typedef MS_U8       (*IOCTL_HDMIRX_HDCP22_WAIT_EVENT)            (void*);
/*************************** HDCP22 ***************************/

typedef struct _HDMI_RX_INSTANT_PRIVATE
{
    MS_U32 ulDeviceID;
    IOCTL_HDMIRX_INITIAL                    fpHDMIRxInitial;
    IOCTL_HDMIRX_STABLE_POLLING             fpHDMIRxStablePolling;
    IOCTL_HDMIRX_GET_SOURCE_VERSION         fpHDMIRxGetSourceVersion;
    IOCTL_HDMIRX_CHECK_HDCP_STATE           fpHDMIRxCheckHDCPState;
    IOCTL_HDMIRX_CHECK_HDCP_ENC_STATE       fpHDMIRxCheckHDCPENCState;
    IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD     fpHDMIRxCheckHDCP14KeyVaild;
    IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG  fpHDMIRxGetHDCP14KeyVaildFlag;
    IOCTL_HDMIRX_SET_HPD_INVERT_FLAG        fpHDMIRxSetHPDInvertFlag;
    IOCTL_HDMIRX_GET_PACKET_RECEIVE         fpHDMIRxGetPacketReceive;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_WRITE_X74                  fpHDMIRxWriteX74;
    IOCTL_HDMIRX_READ_X74                   fpHDMIRxReadX74;
    IOCTL_HDMIRX_SET_REPEATER               fpHDMIRxSetRepeater;
    IOCTL_HDMIRX_SET_BSTATUS                fpHDMIRxSetBstatus;
    IOCTL_HDMIRX_SET_HDMI_MODE              fpHDMIRxSetHDMIMode;
    IOCTL_HDMIRX_GET_INTERRUPT_STATUS       fpHDMIRxGetInterruptStatus;
    IOCTL_HDMIRX_WRITE_KSV_LIST             fpHDMIRxWriteKSVList;
    IOCTL_HDMIRX_SET_VPRIME                 fpHDMIRxSetVPrime;
    /*************************** HDCP Repeater ***************************/
    IOCTL_HDMIRX_FILL_CIPHER_KEY            fpHDMIRxFillCipherKey;
    IOCTL_HDMIRX_PORT_INIT                  fpHDMIRxPortInit;
    IOCTL_HDMIRX_POLLING_READ_DONE          fpHDMIRxPollingReadDone;
    IOCTL_HDMIRX_ENABLE_CIPHER              fpHDMIRxEnableCipher;
    IOCTL_HDMIRX_SEND_MESSAGE               fpHDMIRxSendMessage;
    IOCTL_HDMIRX_HDCP22_HANDER              fpHDMIRxHDCP22Handler;
    IOCTL_HDMIRX_HDCP22_WAIT_EVENT          fpHDMIRxHDCP22WaitEvent;
}HDMI_RX_INSTANT_PRIVATE;

typedef enum
{
    HDMI_RX_POOL = 0,
    HDMI_RX_POOL_MAX,
}eHDMIRxPoolID;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
MS_U32 mdrv_HDMI_STREventProc(void* pModule, EN_POWER_MODE usPowerState);
void mdrv_HDMI_InitialSetting(void* pInstance, stHDMI_INITIAL_TABLE stInitialTable);
void mdrv_HDMI_StablePolling(void* pInstance);
MS_U8 mdrv_HDMI_GetSourceVersion(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_CheckHDCPState(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_U8 mdrv_HDMI_CheckHDCPENCState(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDMI_CheckHDCP14KeyVaild(void* pInstance, MS_U8 *ucHDCPKeyData);
MS_BOOL mdrv_HDMI_GetHDCP14KeyVaildFlag(void* pInstance);
void mdrv_HDMI_SetHPDInvertFlag(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHPDInvertFlag);
MS_U32 mdrv_HDMI_GetPacketReceiveStatus(void* pInstance, MS_U8 ucHDMIInfoSource);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void mdrv_HDMI_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine);
void mdrv_HDMI_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl);
#endif
/*************************** HDCP Repeater ***************************/
void mdrv_HDCP_WriteX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset, MS_U8 ucData);
MS_U8 mdrv_HDCP_ReadX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset);
void mdrv_HDCP_SetRepeater(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsRepeater);
void mdrv_HDCP_SetBstatus(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 usBstatus);
void mdrv_HDCP_SetHDMIMode(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMIMode);
MS_U8 mdrv_HDCP_GetInterruptStatus(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void mdrv_HDCP_WriteKSVList(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucKSV, MS_U32 ulDataLen);
void mdrv_HDCP_SetVPrime(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucVPrime);
/*************************** HDCP Repeater ***************************/
MS_BOOL MDrv_HDMI_IOCTL(MS_U32 ulCommand, void *pBuffer, MS_U32 ulBufferSize);
void mdrv_HDCP22_FillCipherKey(void* pInstance, MS_U8 ucPortIdx, MS_U8* pucRiv, MS_U8* pucSessionKey);
void mdrv_HDCP22_PortInit(void* pInstance, MS_U8 ucPortIdx);
MS_BOOL mdrv_HDCP22_PollingReadDone(void* pInstance, MS_U8 ucPortIdx);
void mdrv_HDCP22_EnableCipher(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable);
void mdrv_HDCP22_SendMessage(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy);
MS_BOOL mdrv_HDCP22_Handler(void* pInstance, MS_U8 ucPortIdx, MS_U8* ucPortType, MS_U8* pucMsgData, MS_U32* wMsgLen, MS_BOOL bIRQModeFlag);
MS_U8 mdrv_HDCP22_WaitEvent(void* pInstance);

#endif //_API_HDMI_RX_PRIVATE_H_
