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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// @file  apiXC_PCMonitor.h
/// @brief header file
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_FE XC_FE modules
 *  \ingroup Video 
 
    PC monitor is used to detect signal information.\n
    For more information,see \link PC_Monitor PC_Monitor interface (apiXC_PCMonitor.h) \endlink

 * \defgroup PC_Monitor PC_Monitor interface (apiXC_PCMonitor.h)
 *  \ingroup XC_FE
 
    PC monitor is used to detect signal information such as
    - input source type : Component / HDMI / DVI / VGA
    - input horizontal frequency
    - input vertical frequency
    - input vertical total
    - input sync detection : H/V polarity, scan type
    - HDMI/DVI only : Data Enable section detection result
    - Euro HDTV detection result
    and the sync stable result will be passed back and make sure these signal characteristics are steady.

    TO BE NOTICED :
    signal characteristics being steady does not mean that this signal are valid signal.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiXC_PCMonitor_base_flow.png

 * \defgroup PC_Monitor_INIT PC_Monitor init control
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_FEATURE PC_Monitor feature operation
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_INFO PC_Monitor Infomation pool
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_ToBeModified PC_Monitor api to be modified
 *  \ingroup PC_Monitor

 *! \defgroup PC_Monitor_ToBeRemove PC_Monitor api to be removed
 *  \ingroup PC_Monitor
 */


#ifndef _API_XC_PC_MONITOR_H_
#define _API_XC_PC_MONITOR_H_
#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define ENABLE_IP_AUTO_COAST    (0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Indicate the status of pc monitor.
typedef enum
{
    E_XC_PCMONITOR_STABLE_NOSYNC = 0,       ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_STABLE_SYNC ,            ///< Input timing stable, no input sync detected
    E_XC_PCMONITOR_UNSTABLE,                ///< Timing change, has to wait InfoFrame if HDMI input
} XC_PCMONITOR_STATUS;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// PC monitor Init
/// @ingroup PC_Monitor_INIT
/// @param  u8MaxWindowNum                 \b IN: max window number
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_Init(MS_U8 u8MaxWindowNum);
//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_FEATURE
/// @param  eWindow                 \b IN: window ID
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_Restart(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_ToBeModified
/// @param  u16TimingStableCounter                 \b IN: stable counter
/// @param  u16TimingNosyncCounter                 \b IN: no signal counter
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_SetTimingCount(MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

//-------------------------------------------------------------------------------------------------
/// PC monitor
/// @ingroup PC_Monitor_FEATURE
/// @param  src                         \b IN: stable counter
/// @param  eWindow                 \b IN: window ID
/// @return XC_PCMONITOR_STATUS
//-------------------------------------------------------------------------------------------------
XC_PCMONITOR_STATUS MApi_XC_PCMonitor(INPUT_SOURCE_TYPE_t src, SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get current status of PC monitor
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: window ID
/// @return XC_PCMONITOR_STATUS
//-------------------------------------------------------------------------------------------------
XC_PCMONITOR_STATUS MApi_XC_PCMonitor_GetCurrentState(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get the sync status of PC monitor
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: window ID
/// @return MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_PCMonitor_GetSyncStatus(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 10 
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 horizontal frequency * 10
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_HFreqx10(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get horizontal frequency * 1000
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 horizontal frequency * 1000
//-------------------------------------------------------------------------------------------------
// MS_U32 MApi_XC_PCMonitor_Get_HFreqx1K(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 10
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical frequency * 10
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_VFreqx10(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical frequency * 1000
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U32 vertical frequency * 1000
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_XC_PCMonitor_Get_VFreqx1K(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get vertical total line
/// @ingroup PC_Monitor_ToBeModified
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_U16 vertical total line
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_PCMonitor_Get_Vtotal(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Get DVI/HDMI DE information
/// @ingroup PC_Monitor_INFO
/// @param  eWindow                 \b IN: which window we are going to get
/// @param  msWin                    \b IN: DE window size
//-------------------------------------------------------------------------------------------------
void MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin);

//-------------------------------------------------------------------------------------------------
/// Get sync loss status
/// @ingroup PC_Monitor_INFO
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_SyncLoss(SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// Check the invalid timing
/// @ingroup PC_Monitor_INFO
/// @param  bPollingOnly            \b IN: enable polling only
/// @param  eWindow                 \b IN: which window we are going to get
/// @return MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_PCMonitor_InvalidTimingDetect(MS_BOOL bPollingOnly, SCALER_WIN eWindow);

//-------------------------------------------------------------------------------------------------
/// PC monitor restart
/// @ingroup PC_Monitor_ToBeModified
/// @param  u16TimingStableCounter                 \b IN: stable counter
/// @param  u16TimingNosyncCounter                 \b IN: no signal counter
/// @return E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_PCMonitor_SetTimingCountEx( INPUT_SOURCE_TYPE_t eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_PC_MONITOR_H_ */
