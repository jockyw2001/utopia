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

#ifndef _DRV_CEC_H_
#define _DRV_CEC_H_




//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MDRV_IRQ_BEGIN      0x40
#define MDRV_IRQ_END        0x7F
#define IRQ_ADCDVI2RIU     (MDRV_IRQ_BEGIN + 31)//31 (level)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef _DRV_CEC_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

MS_U32 mdrv_CEC_STREventProc(void* pModule, EN_POWER_MODE usPowerState);

INTERFACE MS_BOOL MDrv_CEC_DDC2BI_GetInfo(void* pInstance, MS_CEC_INFO_LIST *pInfo);
INTERFACE void MDrv_CEC_PortSelect(void* pInstance, MsCEC_INPUT_PORT InputPort);
INTERFACE void MDrv_CEC_Init(void* pInstance, MS_U32 u32XTAL_CLK_Hz);
INTERFACE void MDrv_CEC_Exit(void* pInstance);
INTERFACE void MDrv_CEC_CheckExistDevices(void* pInstance);
INTERFACE MsCEC_DEVICELA MDrv_CEC_NextDevice(void* pInstance);
INTERFACE void MDrv_CEC_ChkRxBuf(void* pInstance);
// command call _MDrv_CEC_TxApi
INTERFACE CEC_ERROR_CODE MDrv_CEC_TxSendMsg(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8 *cmd, MS_U8 len);
INTERFACE CEC_ERROR_CODE MDrv_CEC_TxSendMsg2(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* cmd, MS_U8 len);
INTERFACE CEC_ERROR_CODE MDrv_CecTxSendPollingMsg(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ActiveSource(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RoutingChange(void* pInstance, MS_U8* orig_address, MS_U8* new_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqActiveSource(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_SetStreamPath(void* pInstance, MS_U8* new_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_Standby(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RecordOff(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_RecordOn(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_RECORD_SOURCE_TYPE cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportCECVersion(void* pInstance, MsCEC_DEVICELA dst_address, MS_U8 version);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqCECVersion(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress(void* pInstance);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqPhycalAddress(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_DeckControl(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_CTRL_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_DecStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_STATUS_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_MSg_GiveDeckStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_STATUS_REQUEST_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_MSg_DCPlay(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_PLAY_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqMenuStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_MENU_REQ_TYPE cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlPressed(void* pInstance, MS_BOOL bUserCtrlEn, MsCEC_DEVICELA dst_address, MsCEC_MSG_USER_CTRL_PARM cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlReleased(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_GiveAudioStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReqPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_FeatureAbort(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MsCEC_MSG_ABORT_REASON cmd);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_Abort(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_SendMenuLanguage(void* pInstance, MS_U8* pu8MenuLanguageCode);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_ReqARCInitiation(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_ReqARCTermination(void* pInstance, MsCEC_DEVICELA dst_address);
INTERFACE CEC_ERROR_CODE MDrv_CecMsg_AudioModeReq(void* pInstance, MsCEC_DEVICELA dst_address, MS_BOOL amp_switch, MS_U8* my_address);
// command end
INTERFACE MS_BOOL MDrv_CEC_CheckFrame(void* pInstance, MsCEC_MSG_TRANS_TYPE msg_type, MS_U8 ucLen);
INTERFACE void MDrv_CEC_ConfigWakeUp(void* pInstance);
INTERFACE void MDrv_CEC_Enabled(void* pInstance, MS_BOOL bEnableFlag);
INTERFACE void MDrv_CEC_SetMyLogicalAddress(void* pInstance, MsCEC_DEVICELA MyLogicalAddress);
INTERFACE void MDrv_CEC_InitChip(void* pInstance, MS_U32 u32XTAL_CLK_Hz);
INTERFACE MS_U8 MDrv_CEC_TxStatus(void* pInstance);
INTERFACE MS_BOOL MDrv_CEC_CheckDeviceIsTx(void* pInstance);
INTERFACE MS_U32 MDrv_CEC_SetPowerState(void* pInstance, EN_POWER_MODE u16PowerState);

#if ENABLE_CEC_MULTIPLE
INTERFACE void MDrv_CEC_SetMyLogicalAddress2(void* pInstance, MsCEC_DEVICELA MyLogicalAddress);
INTERFACE CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress2(void* pInstance);
INTERFACE void MDrv_CEC_SetMyPhysicalAddress2(void* pInstance, MS_U8 *pdata);
INTERFACE void MDrv_CEC_SetInitiator(void* pInstance, MsCEC_DEVICELA IniLA);
#endif



INTERFACE void MDrv_CEC_SetRetryCount(void* pInstance, MS_U8 u8RetryCount);
INTERFACE void MDrv_CEC_ConfigWakeupInfoVendorID(void* pInstance, MS_U8 *u8CecVendorID);

MS_U8 MDrv_CEC_Get_Header(void* pInstance);
MS_U8 MDrv_CEC_Get_OpCode(void* pInstance);
MS_U8 MDrv_CEC_Get_Para(void* pInstance, MS_U8 u8Idx);
MS_U8 MDrv_CEC_GetCmdLen(void* pInstance);
MS_BOOL MDrv_CEC_IsRxBufEmpty(void* pInstance);
void MDrv_CEC_SetActiveLogicalAddress(void* pInstance, MsCEC_DEVICELA Addr);
MsCEC_DEVICELA MDrv_CEC_GetActiveLogicalAddress(void* pInstance);
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetPowerStatus(void* pInstance);
MS_U8 MDrv_CEC_GetFifoIdx(void* pInstance);
void MDrv_CEC_SetFifoIdx(void* pInstance, MS_U8 u8Idx);
void MDrv_CEC_SetActivePowerStatus(void* pInstance, MsCEC_MSG_POWER_STATUS_PARM Status);
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetActivePowerStatus(void* pInstance);
void MDrv_CEC_SetActivePhysicalAddress(void* pInstance, MS_U8 u8Para1, MS_U8 u8Para2);
void MDrv_CEC_SetActiveDeviceCECVersion(void* pInstance, MS_U8 u8Ver);
void MDrv_CEC_SetActiveDeviceType(void* pInstance, MsCEC_DEVICE_TYPE Type);
MS_U8 MDrv_CEC_GetMsgCnt(void* pInstance);
void MDrv_CEC_SetMsgCnt(void* pInstance, MS_U8 u8Cnt);
MS_U8 MDrv_CEC_GetRxData(void* pInstance, MS_U8 u8Fifoidx, MS_U8 u8Idx);
void MDrv_CEC_SetMyPhysicalAddress(void* pInstance, MS_U8 *pdata);
void MDrv_CEC_AttachDriverISR(void* pInstance, MS_BOOL bAttachDrvFlag);
stCEC_INITIAL_CONFIG_INFO MDrv_CEC_GetConfiguration(void* pInstance);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void mdrv_CEC_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine);
void mdrv_CEC_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl);
#endif

////////////////////////////////////////////////////////////////////////////////
#undef INTERFACE

////////////////////////////////////////////////////////////////////////////////
#endif //_DRV_CEC_H_

