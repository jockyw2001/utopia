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
#ifndef DRV_SC_SCALING_H
#define DRV_SC_SCALING_H

#include "UFO.h"

#ifdef DRV_SC_SCALING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

extern void MDrv_SC_set_fbl(void *pInstance, MS_BOOL bEn);  //TODO
extern MS_BOOL MDrv_SC_get_fbl(void *pInstance);  //TODO

#define HVMODE_HDE_VALIDATION(x)    (((x) >= 1200) && ((x) <= 1360))


#define DS_MUTEX
#define DS_MUTEX_DBG 0


#ifdef DRV_SC_SCALING_C
#define INTERFACE
INTERFACE MS_S32            _DS_MUTEX = -1;
#else
#define INTERFACE extern
INTERFACE MS_S32            _DS_MUTEX;
#endif

#define _DS_MUTEX_TIME_OUT                 MSOS_WAIT_FOREVER

#ifdef DS_MUTEX
#if(DS_MUTEX_DBG)
#define _DS_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_DS);                                      \
            printf("0,==========================Prepare to get mutex\n");                             \
            printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
            if(!MsOS_ObtainMutex(_DS_MUTEX, _DS_MUTEX_TIME_OUT))                     \
            {                                                                        \
                printf("==========================\n");                              \
                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
            }\
            printf("[%s][%06d] DS mutex got\n",__FUNCTION__,__LINE__);

#define _DS_RETURN(pInstance)                                                                \
            printf("1,==========================\n");                              \
            printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
            MsOS_ReleaseMutex(_DS_MUTEX);                                          \
            printf("==========================prepare to release semaphore\n");                    \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_DS);                           \
            printf("[%s][%06d] ds mutex released\n",__FUNCTION__,__LINE__);
                                //return _ret;
#else
#define _DS_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_DS);                  \
            if(!MsOS_ObtainMutex(_DS_MUTEX, _MLOAD_MUTEX_TIME_OUT))                     \
            {                                                                        \
            }
#define _DS_RETURN(pInstance)                                                                    \
            MsOS_ReleaseMutex(_DS_MUTEX);                                               \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_DS);
                                //return _ret;

#endif
#else // #if not def DS_MUTEX
#define _DS_ENTRY()         while(0)
#define _DS_RETURN()    while(0)
#endif // #ifdef _DS_MUTEX


INTERFACE void MDrv_SC_SetPQHSDFlag(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL MDrv_SC_GetPQHSDFlag(void *pInstance);
INTERFACE void MDrv_sc_set_capture_window(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_display_window(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_std_display_window(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_crop_window(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_prescaling_ratio(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_postscaling_ratio(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_fetch_number_limit(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_frcm_fetch_number_limit(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_dual_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_dual_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_SetSkipWaitVsync(void *pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn);
INTERFACE MS_BOOL MDrv_SC_GetSkipWaitVsync(void *pInstance, MS_BOOL bScalerWin);
INTERFACE void MDrv_SC_SetCMAHeapID(void *pInstance, MS_U8 u8CMAHeapID,SCALER_WIN eWindow);
INTERFACE void MDrv_SC_sw_db(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow );
INTERFACE void MDrv_SC_dual_sw_db(void *pInstance, P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg);
INTERFACE void MDrv_SC_set_shift_line(void *pInstance, MS_BOOL bFBL, MS_DEINTERLACE_MODE eDeInterlaceMode, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_mirror(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_EnableMirrorModeEx(void *pInstance, MirrorMode_t eMirrorMode,  SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_mirrorEx(void *pInstance, MirrorMode_t eMirrorMode, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_init_fbn_win(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void MDrv_SWDS_Fire(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_DTVPatch(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_PHY MDrv_XC_Get_FRCM_W_BaseAddr(void *pInstance,MS_U8 u8id,SCALER_WIN eWindow);

INTERFACE void MDrv_XC_FilLineBuffer(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetFRCMFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_SetDualFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_EnableMiuDualMode(void* pInstance, MS_BOOL bEnable, MS_U32 u32LeftFactor, MS_U32 u32RightFactor, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetFrameBufferAddressSilently(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetFRCMFrameBufferAddressSilently(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
INTERFACE MS_U8 MDrv_XC_TransFrameNumToFactor(void *pInstance, XC_FRAME_STORE_NUMBER eFrameStoreNumber, MS_BOOL bLinearMode, MS_BOOL bFieldPackingMode);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_XC_TransFactorToFrameNum(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNumFactor);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_XC_TransFactorToFRCMFrameNum(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNumFactor);

INTERFACE MS_BOOL MDrv_XC_GetHSizeChangeManuallyFlag(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_PHY MDrv_XC_GetIPMBase(void *pInstance, MS_U8 num, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_GetDNRBufSize(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetDNRBufSize(void *pInstance, MS_U32 u32DNRBufSize, SCALER_WIN eWindow);
INTERFACE MS_PHY MDrv_XC_GetDualIPMBase(void *pInstance, MS_U8 num, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_GetDualDNRBufSize(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetDualDNRBufSize(void *pInstance, MS_U32 u32DNRBufSize, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_Set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_ClearScalingFactorForInternalCalib(void *pInstance);
INTERFACE void MDrv_SC_Enable_PreScaling(void *pInstance, MS_BOOL bHSDEnable, MS_BOOL bVSDEnable, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_Enable_PostScaling(void *pInstance, MS_BOOL bHSPEnable, MS_BOOL bVSPEnable, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_SetFullRangeCapture(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_GetScmiV1Speical4frame(void *pInstance, XC_FRAME_STORE_NUMBER enStoreNum);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_XC_GetPQSuggestedFrameNum(void *pInstance, SCALER_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER MDrv_XC_GetPQSuggestedFRCMFrameNum(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_GetAvailableSize(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize);

#define MDrv_XC_IsRequestFrameBufferLessMode Hal_SC_Detect_RequestFBL_Mode
INTERFACE MS_BOOL MDrv_XC_IsRequestFrameBufferLessMode(void *pInstance);
INTERFACE E_APIXC_ReturnValue MDrv_XC_SetScalerMemoryRequest(void *pInstance, MS_BOOL bEnable, E_XC_MEMORY_REQUEST_TYPE eRequestType, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_Set_MemFmtEx(void *pInstance, MS_XC_MEM_FMT eMemFmt, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_Adjust_Prescaling_Ratio(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_SC_Cal_Skip_OPM_line(void *pInstance, SCALER_WIN eWindow, MS_U16 u16Src, MS_U16 u16Dst, XC_InternalStatus *pSrcInfo);
INTERFACE void MDrv_SC_Adjust_H_Crop(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_Adjust_Skip_OPM_line(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_Enable_LegacyMode(void *pInstance, MS_BOOL bEnable);
INTERFACE void MDrv_SC_SwitchIPMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_IPM_CONNECT_ID eIPMConnect);
INTERFACE void MDrv_SC_SwitchFRCMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_FRCMW_CONNECT_ID eFRCMConnect);
INTERFACE void MDrv_SC_set_2p_mode(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow);
INTERFACE void MDrv_SC_set_frcm_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_MemControl_Switch_Method_By_Vcnt(void *pInstance, MS_BOOL bEnable, MS_U16 u16Vcnt);
INTERFACE MS_BOOL MDrv_XC_edclk_pd_control(void *pInstance, MS_BOOL bEnable, MS_U16 u16ControlStart, MS_U16 u16ControlEnd);
INTERFACE void Mdrv_SC_set_frcm_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_setBwdConfig(void *pInstance, E_XC_BWD_CONFIG_TYPE eType, void *pstParam, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_IsSupport2StepScaling(void);
INTERFACE void MDrv_XC_SetIsSupport2StepScalingFlag(MS_BOOL bEnable);
INTERFACE MS_BOOL MDrv_XC_GetPQPathStatus(void* pInstance, E_XC_PQ_Path_Type ePqPathType, MS_U16 u16Width, MS_U16 u16Height);
INTERFACE void MDrv_SC_SetForceWrite( EN_MUTE_TYPE muteType, MS_BOOL bIsSkipWaitVsyn);
INTERFACE MS_BOOL MDrv_SC_GetForceWrite( EN_MUTE_TYPE muteType);
INTERFACE void MDrv_XC_SetForceCurrentReadBank(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow);

#undef INTERFACE

#endif /* DRV_SC_SCALING_H */

