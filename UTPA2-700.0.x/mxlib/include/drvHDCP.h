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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HDCP_H_
#define _DRV_HDCP_H_

#include "MsCommon.h"
/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/

/*********************************************************************/
/*                                                                                                                     */
/*                                                      Structure                                                  */
/*                                                                                                                     */
/*********************************************************************/

/*********************************************************************/
/*                                                                                                                     */
/*                                                 Proto-type                                                    */
/*                                                                                                                     */
/*********************************************************************/
//HDCP14
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxInitHdcp()
/// initiallze hardware setting of hdcp1.x Tx module
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxInitHdcp(MS_U8 u8PortIdx);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxLoadKey()
/// loading Hdcp1.x key
/// @param pu8KeyData	\b IN: hdcp key pointer
/// @param bUseKmNewMode \b IN: indicate decrypt mode is new or old mode
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxLoadKey(MS_U8* pu8KeyData, MS_BOOL bUseKmNewMode);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxSetAuthPass()
/// setting register when authentication flow is done
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxSetAuthPass(MS_U8 u8PortIdx);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxEnableENC_EN()
/// enable or disable ENC_EN signal
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bEnable	\b IN: indicate enable or disable ENC_EN signal
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxEnableENC_EN(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxProcessAn()
/// filling or fetching AN value depends on using external or internal AN
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bUseInternalAn	\b IN: indicate internal or external AN
/// @param pu8An \b IN: if using external AN, treat this as input AN; \b OUT: if using internal AN, treat this as return AN
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxProcessAn(MS_U8 u8PortIdx, MS_BOOL bUseInternalAn, MS_U8* pu8An);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGetAKSV()
/// fetch AKSV from hdcp1.x key
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Aksv	\b OUT: MS_U8 pointer to store AKSV for return
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGetAKSV(MS_U8 u8PortIdx, MS_U8* pu8Aksv);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxCompareRi()
/// compare Ri value of sink and source side
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8SinkRi	\b IN: MS_U8 pointer to store Ri value of sink device
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxCompareRi(MS_U8 u8PortIdx, MS_U8* pu8SinkRi);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxConfigMode()
/// config hdcp1.x encrypt mode
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param u8Mode	\b IN: encrypt mode of hdcp1.x
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxConfigMode(MS_U8 u8PortIdx, MS_U8 u8Mode);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGenerateCipher()
/// decrypt hdcp1.x key and init cipher engine
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Bksv	\b IN: MS_U8 pointer to store BKSV value
/// @return void
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGenerateCipher(MS_U8 u8PortIdx, MS_U8* pu8Bksv);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxProcessR0()
/// trigger HW module to do the R0 calculation and return if process is done
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxProcessR0(MS_U8 u8PortIdx);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14TxGetM0()
/// fetch M0 value from HW module
/// @param u8PortIdx    \b IN: HDMI Port Index
/// @param pu8M0        \b IN: pointer to store M0 value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14TxGetM0(MS_U8 u8PortIdx, MS_U8* pu8M0);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14GetM0()
/// fetch HDCP 1.4  M0 value from Sink device
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param pu8Data	\b IN: MS_U8 pointer to store returned M0 value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14GetM0(MS_U8 u8PortIdx, MS_U8 *pu8Data);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP14FillKey()
/// Load HDCP 1.4  key
/// @param pu8KeyData	\b IN: MS_U8 pointer to store HDCP 1.4 key value
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP14FillKey(MS_U8 *pu8KeyData);

//HDCP22
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2Init()
/// Initial Process of HDCP2.2
/// @param \b IN: none
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2Init(void);
//HDCP22 TX
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2Init()
/// Initial of HDCP2.2 tx hw module
/// @param u8PortIdx \b IN: HDMI Port Index
/// @param bEnable \b IN: init / de-init flag
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxModuleInit(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxEnableEncrypt()
/// Enable HDCP2.2 Encryption
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @param bEnable: 	\b IN: TRUE: enable encryption; FALSE: disable encryption
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxFillCipherKey()
/// Fill HDCP2.2 TX Cipher Key
/// @param u8PortIdx	\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8ContentKey);
//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 TX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 TX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2TxSetAuthPass()
/// Set or clear HDCP2.2 TX authentication pass
/// @u8PortIdx		\b IN: HDMI Port Index
/// @bEnable		       \b IN: disable or enable authentication pass bit
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2TxSetAuthPass(MS_U8 u8PortIdx, MS_BOOL bEnable);

//HDCP22 Rx
//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxInit()
/// hdcp22 rx init setting;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxInit(MS_U8 u8PortIdx);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxProcessCipher()
/// Fill HDCP2.2 RX Cipher Key and set SKE pass;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxProcessCipher(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxEnableENCEN()
/// set or clear SKE pass bit;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @bEnable		\b IN: flag to indicate set or clear
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxSetSKEPass(MS_U8 u8PortIdx, MS_BOOL bEnable);

//----------------------------------------------------------------
// MDrv_HDCP_HDCP2RxFillCipherKey()
/// Fill HDCP2.2 RX Cipher Key only;
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8Riv		\b IN: MS_U8 pointer which stored RIV data
/// @pu8ContentKey	\b IN: MS_U8 pointer which stored KS^LV128
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxFillCipherKey(MS_U8 u8PortIdx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey);

//----------------------------------------------------------------
/// MDrv_HDCP_HDCP2RxGetCipherState()
/// Get current HDCP2.2 RX status
/// @u8PortIdx		\b IN: HDMI Port Index
/// @pu8State		\b IN: MS_U8 pointer which return HDCP22 RX cipher state
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_HDCP2RxGetCipherState(MS_U8 u8PortIdx, MS_U8 *pu8State);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDCP2Status()
/// Get HDCP22 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDCPStatus	    \b IN: MS_U32 pointer which return encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDCP2Status(MS_U32 u32HdmiPort, MS_U32 *pu32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDCP2Status()
/// Set HDCP22 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDCPStatus	    \b IN: Encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDCP2Status(MS_U32 u32HdmiPort, MS_U32 u32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDCP1Status()
/// Get HDCP14 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDCPStatus	    \b IN: MS_U32 pointer which return encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDCP1Status(MS_U32 u32HdmiPort, MS_U32 *pu32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDCP1Status()
/// Set HDCP14 encrytion status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDCPStatus	    \b IN: Encrytion status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDCP1Status(MS_U32 u32HdmiPort, MS_U32 u32HDCPStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_GetHDMIStatus()
/// Get HDMI enable status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @pu32HDMIStatus	    \b IN: MS_U32 pointer which return HDMI enable status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_GetHDMIStatus(MS_U32 u32HdmiPort, MS_U32 *pu32HDMIStatus);

//----------------------------------------------------------------
/// MDrv_HDCP_SetHDMIStatus()
/// Set HDMI enable status based on HDMI port number
/// @u32HdmiPort		    \b IN: HDMI Port Index
/// @u32HDMIStatus	    \b IN: HDMI enable status
/// @return MS_BOOL, TRUE: success; FALSE: failed
//----------------------------------------------------------------
MS_BOOL MDrv_HDCP_SetHDMIStatus(MS_U32 u32HdmiPort, MS_U32 u32HDMIStatus);

#endif //#ifndef _DRV_HDCP_H_
