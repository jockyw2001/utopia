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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _API_PQ_PRIV_H_
#define _API_PQ_PRIV_H_


////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "apiPNL.h"
#include "utopia_relation.h"
typedef void                                 (*IOCTL_PQ_INIT)                                    (void* ,MS_PQ_Init_Info *);
typedef    MS_BOOL                                (*IOCTL_PQ_EXIT)                                    (void*    );
typedef    MS_U32                                (*IOCTL_PQ_SET_POWER_STATE)                            (void* ,EN_POWER_MODE );
typedef MS_U16                                (*IOCTL_PQ_GET_IP_NUM)                                (void* ,PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_TABLE_NUM)                            (void* ,PQ_WIN , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_CURRENT_TABLE_INDEX)                    (void* ,PQ_WIN , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_TABLE_INDEX)                            (void* ,PQ_WIN , MS_U16 );
typedef MS_U16                                 (*IOCTL_PQ_GET_CUSTOMER_TABLE_INDEX)                (void* ,PQ_WIN , MS_U16 );
//typedef MS_U16                                (*IOCTL_PQ_GET_XRULE_IP_INDEX)                        (void* ,PQ_WIN , MS_U16 , MS_U16 );
//typedef    MS_U16                                (*IOCTL_PQ_GET_XRULE_TABLE_INDEX)                    (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U16 );
//typedef    MS_U16                                (*IOCTL_PQ_GET_XRULE_IP_NUM)                        (void* ,PQ_WIN , MS_U16 );
typedef    void                                (*IOCTL_PQ_LOAD_TABLE_DATA)                            (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U8 *, MS_U16 );
typedef    void                                (*IOCTL_PQ_CLONE_TABLE)                                (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U16 );
typedef void                                (*IOCTL_PQ_LOAD_TABLE)                                (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef void                                (*IOCTL_PQ_LOAD_CUSTOMER_TABLE)                        (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef void                                (*IOCTL_PQ_CHECK_SETTINGS)                            (void* ,PQ_WIN );
#if (ENABLE_PQ_MLOAD)
typedef void                                (*IOCTL_PQ_SET_MLOAD_EN)                            (void* ,PQ_WIN , MS_BOOL );
#endif
typedef    MS_BOOL                                (*IOCTL_PQ_SKIP_DUPLICATED_SETTING)                    (void* ,PQ_WIN , MS_BOOL );
typedef MS_BOOL                                (*IOCTL_PQ_GET_SKIP_DUPLICATED_SETTING_STATUS)        (void* ,PQ_WIN );
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
typedef    void                                (*IOCTL_PQ_SET_FRCM_CB_CR_SWAP)                        (void* ,PQ_WIN );
#endif
typedef void                                (*IOCTL_PQ_LOAD_SETTINGS)                            (void* ,PQ_WIN );
typedef    void                                (*IOCTL_PQ_LOAD_CUSTONER_SETTINGS)                    (void* ,PQ_WIN );
typedef    void                                (*IOCTL_PQ_SET_DTVINFO)                                (void* ,PQ_WIN , MS_PQ_Dtv_Info *);
typedef    void                                (*IOCTL_PQ_SET_MULTIMEDIAINFO)                        (void* ,PQ_WIN , MS_PQ_MuliMedia_Info *);
typedef    void                                (*IOCTL_PQ_SET_DVINFO)                                (void* ,PQ_WIN , MS_PQ_Vd_Info *);
typedef    void                                (*IOCTL_PQ_SET_MODEINFO)                            (void* ,PQ_WIN , PQ_INPUT_SOURCE_TYPE , MS_PQ_Mode_Info *);
typedef void                                (*IOCTL_PQ_SET_HDMIINFO)                            (void* ,PQ_WIN , const MS_PQ_Hdmi_Info *const );
typedef    void                                (*IOCTL_PQ_SET_HDMI_PC)                                   (void* ,PQ_WIN , MS_BOOL );
typedef MS_BOOL                                (*IOCTL_PQ_GET_HDMI_PC_STATUS)                        (void* ,PQ_WIN );
typedef void                                (*IOCTL_PQ_DESIDE_SRC_TYPE)                            (void* ,PQ_WIN , PQ_INPUT_SOURCE_TYPE );
typedef    PQ_INPUT_SOURCE_TYPE                (*IOCTL_PQ_GET_INPUTSOURCE_TYPE)                    (void* ,PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_SRC_TYPE)                            (void* ,PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_DELAY_TIME_BY_MEMC)                    (void* ,SCALER_WIN, MS_U32 );
typedef    MS_U16                                (*IOCTL_PQ_GET_DELAY_TIME)                            (void* ,SCALER_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_LOAD_SCALING_TABLE)                        (void* , PQ_WIN
                                                                                                        , MS_U8
                                                                                                        , MS_BOOL
                                                                                                        , MS_BOOL
                                                                                                        , MS_BOOL
                                                                                                        , MS_U16
                                                                                                        , MS_U16 );
typedef    void                                (*IOCTL_PQ_SET_COLOR_RANGE)                            (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_CSC_XRULEIDX)                        (void* ,
                                                                                                        MS_BOOL * ,
                                                                                                        MS_U16 * ,
                                                                                                        PQ_WIN ,
                                                                                                        PQ_FOURCE_COLOR_FMT ,
                                                                                                        MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_MEM_YUVFMT)                                (void* ,PQ_WIN , PQ_FOURCE_COLOR_FMT );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_CSC)                                    (void* ,PQ_WIN , PQ_FOURCE_COLOR_FMT );
#if (PQ_XRULE_DB_ENABLE == 1)
typedef    void                                (*IOCTL_PQ_SET_DBRULE)                                (void* ,PQ_WIN , MS_U16 );
#endif
typedef    void                                (*IOCTL_PQ_ENABLE_MADI_FORCE)                        (void* ,PQ_WIN , MS_BOOL );
typedef    void                                (*IOCTL_PQ_REDUCE_BW_FOR_PVR)                        (void* ,PQ_WIN , MS_BOOL );
typedef void                                (*IOCTL_PQ_SET_420UP_SAMPLEING)                        (void* ,
                                                                                                          PQ_WIN ,
                                                                                                          MS_BOOL ,
                                                                                                          MS_BOOL ,
                                                                                                          MS_U16 );
typedef    void                                (*IOCTL_PQ_SET_FILEMODE)                            (void* ,PQ_WIN , MS_BOOL );
typedef    void                                (*IOCTL_PQ_SET_NONLINEAR_SCALING)                    (void* ,PQ_WIN , MS_U8 , MS_BOOL );
typedef    void                                (*IOCTL_PQ_3D_CLONE_FOR_PIP)                        (void* ,MS_BOOL );
typedef    void                                (*IOCTL_PQ_DISABLE_FILEMODE)                        (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_DISABLE_UC_FEATURE)                        (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_CURRENT_UC_FEATURE)                    (void* ,
                                                                                                           PQ_WIN ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_UC_FEATURE_FROM_PQ)                    (void* ,
                                                                                                           PQ_WIN ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * ,
                                                                                                           MS_U16 * );
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
typedef    MS_BOOL                                (*IOCTL_PQ_ENABLE_UC_FEATURE)                        (void* ,PQ_WIN );
#endif
typedef    MS_BOOL                                (*IOCTL_PQ_SET_UC_FEATURE)                            (void* pInstance,
                                                                                                           PQ_WIN ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 ,
                                                                                                           MS_U16 );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_DMSV12L)                                (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_DMSV12L_FROM_XRULE_TABLE)            (void* ,PQ_WIN );
typedef    E_DRVPQ_ReturnValue                    (*IOCTL_PQ_GET_LIB_VER)                                (void* ,const MSIF_Version ** );
typedef    PQ_ReturnValue                        (*IOCTL_PQ_GET_INFO)                                (void* ,MS_PQ_INFO * );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_STATUS)                                (void* ,MS_PQ_Status * );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_DBGLEVEL)                            (void* ,MS_U16 );
typedef    void                                (*IOCTL_PQ_MADI_FORCE_MOTIONC)                        (void* ,PQ_WIN , MS_BOOL );
typedef    void                                (*IOCTL_PQ_MADI_FORCE_MOTIONY)                        (void* ,PQ_WIN , MS_BOOL );
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
typedef    void                                (*IOCTL_PQ_MADI_FORCE_MOTION)                        (void* ,PQ_WIN , MS_BOOL, MS_U16 , MS_BOOL, MS_U16);
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
typedef    void                                (*IOCTL_PQ_SET_COLOR_THROUGH_MODE)                 (void* ,PQ_WIN , MS_BOOL);
#endif

typedef    void                                (*IOCTL_PQ_SET_RFBL_MODE)                            (void* ,MS_BOOL , MS_BOOL );
typedef    void                                (*IOCTL_PQ_GET_RFBL_MODE)                            (void* ,MS_PQ_RFBL_INFO * );
#if (PQ_ENABLE_VD_SAMPLING)
typedef    MS_BOOL                                (*IOCTL_PQ_GET_VDSAMPLING_INFO)                        (void* ,
                                                                                                        PQ_WIN ,
                                                                                                        PQ_INPUT_SOURCE_TYPE ,
                                                                                                        PQ_VIDEOSTANDARD_TYPE ,
                                                                                                        MS_PQ_VD_Sampling_Info * );
#endif
typedef    MS_BOOL                                (*IOCTL_PQ_INTER_IOCTL)                                (void* ,PQ_WIN , MS_U32 , void * , MS_U32 );
typedef    MS_PQ_ADC_SAMPLING_TYPE                (*IOCTL_PQ_GET_ADC_SAMPLING_INTO)                    (void* ,PQ_WIN , PQ_INPUT_SOURCE_TYPE , MS_PQ_Mode_Info * );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_3D_ONOFF)                            (void* ,MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_CAPS)                                (void* ,MS_PQ_CAP_INFO * );
typedef    void                                (*IOCTL_PQ_PRINT_LOADTABLE_INFO)                    (void* ,MS_PQ_PQLOADTABLE_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_SETDS_ONOFF)                                (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_RFBL_INFO)                            (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_CHECK_HD_MODE)                            (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_INTERLACE_SD_MODE)                    (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_DUAL_VIEW_STATE)                        (void* ,MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_VGASUBCASE_FORCETO_422MODE)            (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_DUAL_VIEW_STATE)                        (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_FORCE_VIDEO_INPUT_MODE)                    (void* ,MS_BOOL , PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_IS_FORCE_VIDEO_INPUTMODE)                (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_POINT2POINT)                            (void* ,
                                                                                                        XC_SETWIN_INFO * ,
                                                                                                           MS_BOOL ,
                                                                                                           PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_POINT2POINT)                            (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_UPDATE_MEMFORMAT)                        (void* );
typedef    MS_U16                                (*IOCTL_PQ_GET_HSIZE)                                (void* ,PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_VSIZE)                                (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_IS_INTERLACE)                            (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_LOW_3D_QUALITY)                            (void* ,MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_MWE_SET_ENHANCE_QUALITY)                    (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_MWE_RESTORE_ENHANCE_QUALITY)                (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_MWE_RESTORE_OFF_QUALITY)                    (void* );
typedef    MS_BOOL                                (*IOCTL_PQ_MWE_SET_OFF_QUALITY)                        (void* );
typedef    void                                (*IOCTL_PQ_MWE_CLONE_WINDOW)                        (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_3D_SETTING_FOR_LBL)                        (void* ,PQ_WIN );
typedef    void                                (*IOCTL_PQ_3D_CLONE_WINDOW)                            (void* ,PQ_3DTYPE , PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_SENSIOMODE)                            (void* );
typedef    void                                (*IOCTL_PQ_SET_SENSIOMODE)                            (void* ,MS_BOOL );
typedef    void                                (*IOCTL_PQ_SET_DIPF)                                (void* );
typedef    void                                (*IOCTL_PQ_SET_SELECT_CSC)                            (void* ,MS_U16 , PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_R2YEQ_SELECT)                        (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_MEMFMT_IN_GENERAL)                    (void* ,
                                                                                                        PQ_WIN ,
                                                                                                        MS_BOOL ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U16 * );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_MADI_FOR_RFBL)                        (void* ,
                                                                                                        PQ_WIN ,
                                                                                                        MS_BOOL ,
                                                                                                         MS_U16 * ,
                                                                                                        MS_U16 * );
typedef    MS_BOOL                                (*IOCTL_PQ_PATCH2RTO4R_FOR_FIELDPACKINGMODE)        (void* ,PQ_WIN , MS_U16 );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_BPPINFO_FROM_MADI)                    (void* ,
                                                                                                          PQ_WIN ,
                                                                                                          MS_BOOL ,
                                                                                                          MS_U16 ,
                                                                                                          MS_U16 ,
                                                                                                          MS_U8 * ,
                                                                                                          PQ_DEINTERLACE_MODE * ,
                                                                                                          MS_U8 * );
typedef    MS_BOOL                                (*IOCTL_PQ_CHANGE_MEMCONFIG_FOR3D)                    (void* ,
                                                                                                        PQ_WIN ,
                                                                                                        MS_BOOL ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U8 * ,
                                                                                                        MS_U16 * ,
                                                                                                        MS_U16 * );
typedef    MS_BOOL                                (*IOCTL_PQ_FORCE_BPP_FOR_DYNAMIC_MEMFMT)            (void* ,PQ_WIN , MS_BOOL , MS_U8 * );
typedef    MS_BOOL                                (*IOCTL_PQ_SUGGEST_FRAMENUM)                        (void* ,PQ_WIN , MS_U16 , MS_U8 );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_GAMEMODE)                            (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_GAMEMODE_STATUS)                        (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_BYPASS_MODE)                            (void* ,PQ_WIN , MS_BOOL );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_BYPASS_MODE_STATUS)                    (void* ,PQ_WIN );
typedef    char *                                (*IOCTL_PQ_GET_VERSION)                                (void* ,PQ_WIN );
typedef    char *                                (*IOCTL_PQ_CUS_GET_VERSION)                                (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_GRULE_STATUS)                        (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_GRULE_LV1NUM)                        (void* ,PQ_WIN , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_GRULE_GRULENUM)                        (void* ,PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_GRULE_LEVELINDEX)                    (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_GRULE_IPINDEX)                        (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_GRULE_TABLEINDEX)                    (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_CUSTOMERGRULE_IPINDEX)                (void* ,PQ_WIN , MS_U16 , MS_U16 );
typedef    MS_U16                                (*IOCTL_PQ_GET_CUSTOMERGRULE_TABLEINDEX)            (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U16 );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_XVYCC_MATRIXCOEFFICIENT)                (void* ,
                                                                                                        MS_U64 , MS_U64 ,
                                                                                                        MS_U64 , MS_U64 ,
                                                                                                        MS_U64 , MS_U64 ,
                                                                                                        MS_U64 , MS_U64 ,
                                                                                                        PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_SET_XVYCC_MATRIXENABLE)                    (void* ,MS_BOOL , PQ_WIN );
typedef    MS_U16                                (*IOCTL_PQ_GET_DNR_WHOLE_REG)                            (void* ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_Up_Layer_Set_Config)                        (void* ,MS_U16 , PQ_MISC_GROUP );
typedef    MS_PQ_EXECUTOR                        (*IOCTL_PQ_GetQmapExecutor)                            (void* ,MS_PQ_FUNCTION_TYPE );
typedef    void                                (*IOCTL_PQ_SetPhotoYUVStandard)                        (void* ,PQ_WIN , PQ_YUV_STD );
typedef    char *                                (*IOCTL_PQ_Ex_GetVersion)                            (void* ,PQ_WIN );
typedef    MS_U32                                (*IOCTL_PQ_GetAdaptiveVersion)                        (void* ,PQ_WIN );
typedef    void                                (*IOCTL_PQ_LoadUFSCSettings)                        (void* ,PQ_WIN );
#if (ENABLE_PQ_BIN_CF)
typedef    void                                (*IOCTL_PQ_LoadCFSettings)                          (void* ,PQ_WIN );
#endif
typedef    MS_BOOL                                (*IOCTL_PQ_SET_PQBYPASS)                            (void* ,XC_SETWIN_INFO * ,MS_BOOL ,PQ_WIN );
typedef    MS_BOOL                                (*IOCTL_PQ_GET_PQBYPASS)                            (void* ,PQ_WIN );
typedef    void                                (*IOCTL_PQ_SET_PQBIN_PATH)                            (void* ,PQ_BIN_PATH ,MS_U8 ,char* );
//drvPQ_cus.c
typedef void                                 (*IOCTL_PQ_LOAD_FILMMODE_TABLE)                        (void* ,PQ_WIN , PQ_FILM_MODE_FUNCTION_TYPE);
typedef void                                 (*IOCTL_PQ_LOAD_DYNAMIC_CONTRAST_TABLE)                (void* ,PQ_WIN , PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE );
typedef void                                 (*IOCTL_PQ_LOAD_NR_TABLE)                            (void* ,PQ_WIN , PQ_3D_NR_FUNCTION_TYPE );
typedef void                                 (*IOCTL_PQ_LOAD_MPEG_NR_TABLE)                        (void* ,PQ_WIN , PQ_MPEG_NR_FUNCTION_TYPE );
typedef void                                 (*IOCTL_PQ_LOAD_ULTRACLEAR_TABLE)                    (void* ,PQ_WIN , PQ_FEATURE_SWITCH_TYPE );
typedef void                                 (*IOCTL_PQ_LOAD_XVYCC_TABLE)                           (void* ,PQ_WIN , PQ_XVYCC_TYPE );
typedef void                                 (*IOCTL_PQ_LOAD_HDR_TABLE)                             (void* ,PQ_WIN , PQ_HDR_MODE );
typedef MS_BOOL                                 (*IOCTL_PQ_GRULE_GET_SUPPORT_STATUS)                (void* ,PQ_SUPPORTED_TYPE );
typedef PQ_DEINTERLACE_MODE                     (*IOCTL_PQ_SET_MEMFORMAT)                             (void* ,PQ_WIN , MS_BOOL , MS_BOOL , MS_U8 *);
typedef void                                 (*IOCTL_PQ_REDUCEBW_ForOSD)                            (void* ,PQ_WIN , MS_BOOL );
typedef void                                (*IOCTL_PQ_LOAD_PTP_TABLE)                            (void* ,PQ_WIN , PQ_PTP_FUNCTION_TYPE );
typedef void                                 (*IOCTL_PQ_INIT_DISPLAY_TYPE)                        (void* ,MS_U16 , PQ_DISPLAY_TYPE );
typedef void                                 (*IOCTL_PQ_SET_DISPLAY_TYPE)                        (void* ,MS_U16 , PQ_DISPLAY_TYPE );
typedef MS_BOOL                                 (*IOCTL_PQ_GET_MADI_FROM_OSDBW_GRULE)                (void* ,PQ_WIN , MS_U16 * ,MS_U16 * );
typedef MS_BOOL                                 (*IOCTL_PQ_GET_MADI_IN_GENERAL)                        (void* ,PQ_WIN , MS_U16 * , MS_U16 * );
typedef MS_BOOL                                 (*IOCTL_PQ_CHECK_POINT2POINT_CONDITION)                (void* ,XC_SETWIN_INFO * ,PQ_WIN );
typedef MS_BOOL                             (*IOCTL_PQ_SET_POSTCCS_ONOFF)                        (void* ,PQ_WIN , MS_BOOL );
typedef void                                 (*IOCTL_PQ_ADAPTIVE_TUNING)                            (void* );
typedef void                                 (*IOCTL_PQ_FILMMODE_ANYCANDENCE_ENABLE)                (void* ,MS_BOOL );
typedef MS_BOOL                                 (*IOCTL_PQ_SETH264_ONOFF)                            (void* ,MS_BOOL );
typedef MS_BOOL                                (*IOCTL_PQ_SETG3D_ONOFF)                            (void* ,MS_BOOL );
typedef MS_BOOL                                 (*IOCTL_PQ_SETMVC4KX1K_ONOFF)                        (void* ,MS_BOOL );
typedef MS_BOOL                             (*IOCTL_PQ_SET_NETWORKMM_ONOFF)                        (void* ,MS_BOOL );
typedef MS_BOOL                                 (*IOCTL_PQ_SETMM_ONOFF)                                (void* ,MS_BOOL );
typedef MS_BOOL                                 (*IOCTL_PQ_SET_RMVB_ONOFF)                            (void* ,MS_BOOL , PQ_WIN );
typedef MS_BOOL                             (*IOCTL_PQ_SET_VIPBYPASS_ONOFF)                        (void* ,MS_BOOL , PQ_WIN );
typedef MS_BOOL                             (*IOCTL_PQ_SET_PEAKING_ONOFF)                        (void* ,MS_BOOL , PQ_WIN );
typedef MS_BOOL                                (*IOCTL_PQ_SET_FAKE_OUT_ENABLE)                        (void* ,MS_BOOL );
typedef MS_U16                              (*IOCTL_PQ_GET_TABLE_INDEX_EX )                     (void* ,PQ_WIN  , MS_U16 , PQ_ENGINE_TYPE );
typedef void                                (*IOCTL_PQ_LOAD_TABLE_EX )                          (void* ,PQ_WIN  , MS_U16 , MS_U16 , PQ_ENGINE_TYPE );
typedef MS_BOOL                             (*IOCTL_PQ_DEMO_CLONE_WINDOW )                      (void*, PQ_WIN , MS_U8 );
typedef MS_U16                              (*IOCTL_PQ_GET_GRULE_LVLNUM_EX )                    (void* ,PQ_WIN , MS_U16 , PQ_ENGINE_TYPE );
typedef MS_U16                              (*IOCTL_PQ_GET_GRULE_LEVEILNDEX_EX)                 (void* ,PQ_WIN , MS_U16 , MS_U16 , PQ_ENGINE_TYPE );
typedef MS_U16                              (*IOCTL_PQ_GET_GRULE_IPINDEX_EX)                    (void* ,PQ_WIN , MS_U16 , MS_U16 , PQ_ENGINE_TYPE );
typedef MS_U16                              (*IOCTL_PQ_GET_GRULE_TABLEINDEX_EX)                 (void* ,PQ_WIN , MS_U16 , MS_U16 , MS_U16 , PQ_ENGINE_TYPE );
typedef void                                (*IOCTL_PQ_EBABLE_SCALERGAMMA)                        (void*, PQ_WIN , MS_BOOL );
typedef void                                (*IOCTL_PQ_SET_GAMMATBl)                            (void*, APIPNL_GAMMA_TYPE , MS_U8** , APIPNL_GAMMA_MAPPEING_MODE );
typedef MS_BOOL                        (*IOCTL_PQ_SET_1KTRAVELINGENABLE)                            (void*,MS_BOOL );
//drvPQ_text.c
typedef    char *                                (*IOCTL_PQ_GET_SRC_TYPE_NAME)                        (void* ,PQ_WIN );
typedef    char *                                (*IOCTL_PQ_GET_IP_NAME)                                (void* ,MS_U8 );
typedef    char *                                (*IOCTL_PQ_GET_TABLE_NAME)                            (void* ,MS_U8 ,MS_U8 );
//drvbw.c
typedef void                                 (*IOCTL_PQ_BW_LOAD_INIT_TABLE)                        (void* );
typedef void                                (*IOCTL_PQ_BW_LOAD_TABLE_BY_CONTEXT)                (void* ,PQ_WIN );
typedef void                                (*IOCTL_PQ_BW_CUSMODE_LOAD_TABLE)                   (void* , MS_U8 );
typedef void                                (*IOCTL_PQ_BW_RESET_TO_DEFAULT)                        (void* );
typedef MS_BOOL                                 (*IOCTL_PQ_GETCONFIG)                              (void* ,MS_PQ_Init_Info *);
typedef MS_BOOL                             (*IOCTL_PQ_SET_FRAMEBUFF_MODE)                       (void* ,MS_BOOL);

typedef struct
{
    //IOCTL_PNL_GETLIBVER                         fpPNL_GetLibVer;
    IOCTL_PQ_INIT                                fpPQ_Init;
    IOCTL_PQ_EXIT                                fpPQ_Exit;
    IOCTL_PQ_SET_POWER_STATE                    fpPQ_SetPowerState;
    IOCTL_PQ_GET_IP_NUM                            fpPQ_GetIPNum;
    IOCTL_PQ_GET_TABLE_NUM                        fpPQ_GetTableNum;
    IOCTL_PQ_GET_CURRENT_TABLE_INDEX            fpPQ_GetCurrentTableIndex;
    IOCTL_PQ_GET_TABLE_INDEX                    fpPQ_GetTableIndex;
    IOCTL_PQ_GET_CUSTOMER_TABLE_INDEX            fpPQ_GetCustomerTableIndex;
    //IOCTL_PQ_GET_XRULE_IP_INDEX                    fpPQ_GetXRuleIPIndex;
    //IOCTL_PQ_GET_XRULE_TABLE_INDEX                fpPQ_GetXRuleTableIndex;
    //IOCTL_PQ_GET_XRULE_IP_NUM                    fpPQ_GetXRuleIPNum;
    IOCTL_PQ_LOAD_TABLE_DATA                    fpPQ_LoadTableData;
    IOCTL_PQ_CLONE_TABLE                        fpPQ_CloneTable;
    IOCTL_PQ_LOAD_TABLE                            fpPQ_LoadTable;
    IOCTL_PQ_LOAD_CUSTOMER_TABLE                fpPQ_LoadCustomerTable;
    IOCTL_PQ_CHECK_SETTINGS                        fpPQ_CheckSettings;
#if (ENABLE_PQ_MLOAD)
    IOCTL_PQ_SET_MLOAD_EN    fpPQ_MLoadEn;
#endif
    IOCTL_PQ_SKIP_DUPLICATED_SETTING            fpPQ_SkipDuplicatedSetting;
    IOCTL_PQ_GET_SKIP_DUPLICATED_SETTING_STATUS    fpPQ_GetSkipDuplicatedSettingStatus;
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
    IOCTL_PQ_SET_FRCM_CB_CR_SWAP                fpPQ_SetFrcmCbCrSwap;
#endif
    IOCTL_PQ_LOAD_SETTINGS                        fpPQ_LoadSettings;
    IOCTL_PQ_LOAD_CUSTONER_SETTINGS                fpPQ_LoadCustomerSettings;
    IOCTL_PQ_SET_DTVINFO                        fpPQ_Set_DTVInfo;
    IOCTL_PQ_SET_MULTIMEDIAINFO                    fpPQ_Set_MultiMediaInfo;
    IOCTL_PQ_SET_DVINFO                            fpPQ_Set_VDInfo;
    IOCTL_PQ_SET_MODEINFO                        fpPQ_Set_ModeInfo;
    IOCTL_PQ_SET_HDMIINFO                        fpPQ_SetHDMIInfo;
    IOCTL_PQ_SET_HDMI_PC                        fpPQ_SetHDMI_PC;
    IOCTL_PQ_GET_HDMI_PC_STATUS                    fpPQ_GetHDMI_PC_Status;
    IOCTL_PQ_DESIDE_SRC_TYPE                    fpPQ_DesideSrcType;
    IOCTL_PQ_GET_INPUTSOURCE_TYPE                fpPQ_GetInputSourceType;
    IOCTL_PQ_GET_SRC_TYPE                        fpPQ_GetSrcType;
    IOCTL_PQ_GET_DELAY_TIME_BY_MEMC                fpPQ_GetDelayTimeByMemc;
    IOCTL_PQ_GET_DELAY_TIME                        fpPQ_GetDelayTime;
    IOCTL_PQ_LOAD_SCALING_TABLE                    fpPQ_LoadScalingTable;
    IOCTL_PQ_SET_COLOR_RANGE                    fpPQ_SetColorRange;
    IOCTL_PQ_GET_CSC_XRULEIDX                    fpPQ_Get_CSC_XRuleIdx;
    IOCTL_PQ_MEM_YUVFMT                            fpPQ_Get_MemYUVFmt;
    IOCTL_PQ_SET_CSC                            fpPQ_SetCSC;
#if (PQ_XRULE_DB_ENABLE == 1)
    IOCTL_PQ_SET_DBRULE                            fpPQ_Set_DBRule;
#endif
    IOCTL_PQ_ENABLE_MADI_FORCE                    fpPQ_EnableMADIForce;
    IOCTL_PQ_REDUCE_BW_FOR_PVR                    fpPQ_ReduceBW_ForPVR;
    IOCTL_PQ_SET_420UP_SAMPLEING                fpPQ_Set420upsampling;
    IOCTL_PQ_SET_FILEMODE                        fpPQ_SetFilmMode;
    IOCTL_PQ_SET_NONLINEAR_SCALING                fpPQ_SetNonLinearScaling;
    IOCTL_PQ_3D_CLONE_FOR_PIP                    fpPQ_3DCloneforPIP;
    IOCTL_PQ_DISABLE_FILEMODE                    fpPQ_DisableFilmMode;
    IOCTL_PQ_DISABLE_UC_FEATURE                    fpPQ_DisableUCFeature;
    IOCTL_PQ_GET_CURRENT_UC_FEATURE                fpPQ_GetCurrentUCFeature;
    IOCTL_PQ_GET_UC_FEATURE_FROM_PQ                fpPQ_GetUCFeatureFromPQ;
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    IOCTL_PQ_ENABLE_UC_FEATURE                    fpPQ_EnableUCFeature;
#endif
    IOCTL_PQ_SET_UC_FEATURE                        fpPQ_SetUCFeature;
    IOCTL_PQ_SET_DMSV12L                        fpPQ_SetDMSV12L;
    IOCTL_PQ_GET_DMSV12L_FROM_XRULE_TABLE        fpPQ_GetDMSV12LFromXRuleTable;
    IOCTL_PQ_GET_LIB_VER                        fpPQ_GetLibVer;
    IOCTL_PQ_GET_INFO                            fpPQ_GetInfo;
    IOCTL_PQ_GET_STATUS                            fpPQ_GetStatus;
    IOCTL_PQ_SET_DBGLEVEL                        fpPQ_SetDbgLevel;
    IOCTL_PQ_MADI_FORCE_MOTIONC                    fpPQ_MADiForceMotionC;
    IOCTL_PQ_MADI_FORCE_MOTIONY                    fpPQ_MADiForceMotionY;
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
    IOCTL_PQ_MADI_FORCE_MOTION                    fpPQ_MADiForceMotion;
#endif
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
    IOCTL_PQ_SET_COLOR_THROUGH_MODE               fpPQ_SetColorThroughMode;
#endif
    IOCTL_PQ_SET_RFBL_MODE                        fpPQ_SetRFblMode;
    IOCTL_PQ_GET_RFBL_MODE                        fpPQ_GetRFblMode;
#if (PQ_ENABLE_VD_SAMPLING)
    IOCTL_PQ_GET_VDSAMPLING_INFO                fpPQ_Get_VDSampling_Info;
#endif
    IOCTL_PQ_INTER_IOCTL                        fpPQ_INTER_IOCTL;
    IOCTL_PQ_GET_ADC_SAMPLING_INTO                fpPQ_Get_ADCSampling_Info;
    IOCTL_PQ_SET_3D_ONOFF                        fpPQ_Set3D_OnOff;
    IOCTL_PQ_GET_CAPS                            fpPQ_GetCaps;
    IOCTL_PQ_PRINT_LOADTABLE_INFO                fpPQ_PrintLoadTableInfo;
    IOCTL_PQ_SETDS_ONOFF                        fpPQ_SetDS_OnOFF;
    IOCTL_PQ_GET_RFBL_INFO                        fpPQ_Get_RFBL_Info;
    IOCTL_PQ_CHECK_HD_MODE                        fpPQ_CheckHDMode;
    IOCTL_PQ_GET_INTERLACE_SD_MODE                fpPQ_Interlace_SD_mode;
    IOCTL_PQ_SET_DUAL_VIEW_STATE                fpPQ_SetDualViewState;
    IOCTL_PQ_GET_VGASUBCASE_FORCETO_422MODE        fpPQ_GetVGASubCaseForceTo422Mode;
    IOCTL_PQ_GET_DUAL_VIEW_STATE                fpPQ_GetDualViewState;
    IOCTL_PQ_FORCE_VIDEO_INPUT_MODE                fpPQ_ForceVideoInputMode;
    IOCTL_PQ_IS_FORCE_VIDEO_INPUTMODE            fpPQ_IsForceVideoInputMode;
    IOCTL_PQ_SET_POINT2POINT                    fpPQ_Set_PointToPoint;
    IOCTL_PQ_GET_POINT2POINT                    fpPQ_Get_PointToPoint;
    IOCTL_PQ_UPDATE_MEMFORMAT                    fpPQ_Update_MemFormat;
    IOCTL_PQ_GET_HSIZE                            fpPQ_GetHsize;
    IOCTL_PQ_GET_VSIZE                            fpPQ_GetVsize;
    IOCTL_PQ_IS_INTERLACE                        fpPQ_IsInterlace;
    IOCTL_PQ_LOW_3D_QUALITY                        fpPQ_LOW_3dQuality;
    IOCTL_PQ_MWE_SET_ENHANCE_QUALITY            fpPQ_MWE_SetEnhanceQuality;
    IOCTL_PQ_MWE_RESTORE_ENHANCE_QUALITY        fpPQ_MWE_RestoreEnhanceQuality;
    IOCTL_PQ_MWE_RESTORE_OFF_QUALITY            fpPQ_MWE_RestoreOffQuality;
    IOCTL_PQ_MWE_SET_OFF_QUALITY                fpPQ_MWE_SetOffQuality;
    IOCTL_PQ_MWE_CLONE_WINDOW                    fpPQ_MWE_CloneWindow;
    IOCTL_PQ_3D_SETTING_FOR_LBL                    fpPQ_3D_SettingForLBL;
    IOCTL_PQ_3D_CLONE_WINDOW                    fpPQ_3D_CloneWindow;
    IOCTL_PQ_GET_SENSIOMODE                        fpPQ_Get_SensioMode;
    IOCTL_PQ_SET_SENSIOMODE                        fpPQ_Set_SensioMode;
    IOCTL_PQ_SET_DIPF                            fpPQ_Set_DIPF;
    IOCTL_PQ_SET_SELECT_CSC                        fpPQ_SetSelectCSC;
    IOCTL_PQ_GET_R2YEQ_SELECT                    fpPQ_GetR2YEqSelect;
    IOCTL_PQ_GET_MEMFMT_IN_GENERAL                fpPQ_GetMemFmtInGeneral;
    IOCTL_PQ_GET_MADI_FOR_RFBL                    fpPQ_GetMADiForRFBL;
    IOCTL_PQ_PATCH2RTO4R_FOR_FIELDPACKINGMODE    fpPQ_Patch2Rto4RForFieldPackingMode;
    IOCTL_PQ_GET_BPPINFO_FROM_MADI                fpPQ_GetBPPInfoFromMADi;
    IOCTL_PQ_CHANGE_MEMCONFIG_FOR3D                fpPQ_ChangeMemConfigFor3D;
    IOCTL_PQ_FORCE_BPP_FOR_DYNAMIC_MEMFMT        fpPQ_ForceBPPForDynamicMemFmt;
    IOCTL_PQ_SUGGEST_FRAMENUM                    fpPQ_SuggestFrameNum;
    IOCTL_PQ_SET_GAMEMODE                        fpPQ_SetGameMode;
    IOCTL_PQ_GET_GAMEMODE_STATUS                fpPQ_GetGameMode_Status;
    IOCTL_PQ_SET_BYPASS_MODE                    fpPQ_SetBypassMode;
    IOCTL_PQ_GET_BYPASS_MODE_STATUS                fpPQ_GetBypassModeStatus;
    IOCTL_PQ_GET_VERSION                        fpPQ_GetVersion;
    IOCTL_PQ_CUS_GET_VERSION                    fpPQ_Cus_GetVersion;
    IOCTL_PQ_SET_GRULE_STATUS                    fpPQ_SetGRuleStatus;
    IOCTL_PQ_GET_GRULE_LV1NUM                    fpPQ_GetGRule_LvlNum;
    IOCTL_PQ_GET_GRULE_GRULENUM                    fpPQ_GetGRule_GRuleNum;
    IOCTL_PQ_GET_GRULE_LEVELINDEX                fpPQ_GetGRule_LevelIndex;
    IOCTL_PQ_GET_GRULE_IPINDEX                    fpPQ_GetGRule_IPIndex;
    IOCTL_PQ_GET_GRULE_TABLEINDEX                fpPQ_GetGRule_TableIndex;
    IOCTL_PQ_GET_CUSTOMERGRULE_IPINDEX            fpPQ_GetCustomerGRule_IPIndex;
    IOCTL_PQ_GET_CUSTOMERGRULE_TABLEINDEX        fpPQ_GetGRule_CustomerTableIndex;
    IOCTL_PQ_SET_XVYCC_MATRIXCOEFFICIENT        fpPQ_Set_xvYCC_MatrixCoefficient;
    IOCTL_PQ_SET_XVYCC_MATRIXENABLE                fpPQ_Set_xvYCC_MatrixEnable;
    IOCTL_PQ_GET_DNR_WHOLE_REG                  fpPQ_Get_DNR_Whole_reg;
    IOCTL_PQ_Up_Layer_Set_Config                fpPQ_Up_Layer_Set_Config;
    IOCTL_PQ_GetQmapExecutor                    fpPQ_GetQmapExecutor;
    IOCTL_PQ_SetPhotoYUVStandard                fpPQ_SetPhotoYUVStandard;
    IOCTL_PQ_Ex_GetVersion                        fpPQ_Ex_GetVersion;
    IOCTL_PQ_GetAdaptiveVersion                    fpPQ_GetAdaptiveVersion;
    IOCTL_PQ_LoadUFSCSettings                    fpPQ_LoadUFSCSettings;
#if (ENABLE_PQ_BIN_CF)
    IOCTL_PQ_LoadCFSettings                      fpPQ_LoadCFSettings;
#endif
    IOCTL_PQ_GET_TABLE_INDEX_EX                                  fpPQ_GetTableIndex_Ex;
    IOCTL_PQ_LOAD_TABLE_EX                                       fpPQ_LoadTable_Ex;
    IOCTL_PQ_DEMO_CLONE_WINDOW                                   fpPQ_Demo_CloneWindow;
    IOCTL_PQ_GET_GRULE_LVLNUM_EX                                 fpPQ_GetGRule_LvlNum_Ex;
    IOCTL_PQ_GET_GRULE_LEVEILNDEX_EX                             fpPQ_GetGRule_LevelIndex_Ex;
    IOCTL_PQ_GET_GRULE_IPINDEX_EX                                fpPQ_GetGRule_IPIndex_Ex;
    IOCTL_PQ_GET_GRULE_TABLEINDEX_EX                             fpPQ_GetGRule_TableIndex_Ex;
    IOCTL_PQ_GET_PQBYPASS                                         fpPQ_Get_PqBypass;
    IOCTL_PQ_SET_PQBYPASS                                         fpPQ_Set_PqBypass;
    IOCTL_PQ_SET_PQBIN_PATH                                    fpPQ_Set_PqBin_Path;
//drvPQ_cus.c
    IOCTL_PQ_LOAD_FILMMODE_TABLE                fpPQ_LoadFilmModeTable;
    IOCTL_PQ_LOAD_DYNAMIC_CONTRAST_TABLE        fpPQ_LoadDynamicContrastTable;
    IOCTL_PQ_LOAD_NR_TABLE                        fpPQ_LoadNRTable;
    IOCTL_PQ_LOAD_MPEG_NR_TABLE                    fpPQ_LoadMPEGNRTable;
    IOCTL_PQ_LOAD_ULTRACLEAR_TABLE                fpPQ_Load_ULTRACLEAR_Table;
    IOCTL_PQ_LOAD_XVYCC_TABLE                    fpPQ_Load_XVYCC_Table;
    IOCTL_PQ_LOAD_HDR_TABLE                    fpPQ_Load_HDR_Table;
    IOCTL_PQ_GRULE_GET_SUPPORT_STATUS            fpPQ_GRULE_Get_Support_Status;
    IOCTL_PQ_SET_MEMFORMAT                        fpPQ_SetMemFormat;
    IOCTL_PQ_REDUCEBW_ForOSD                    fpPQ_ReduceBW_ForOSD;
    IOCTL_PQ_LOAD_PTP_TABLE                        fpPQ_LoadPTPTable;
    IOCTL_PQ_INIT_DISPLAY_TYPE                    fpPQ_Init_DisplayType;
    IOCTL_PQ_SET_DISPLAY_TYPE                    fpPQ_Set_DisplayType;
    IOCTL_PQ_GET_MADI_FROM_OSDBW_GRULE            fpPQ_GetMADiFromOSDBWGrule;
    IOCTL_PQ_GET_MADI_IN_GENERAL                fpPQ_GetMADiInGeneral;
    IOCTL_PQ_CHECK_POINT2POINT_CONDITION        fpPQ_Check_PointToPoint_Condition;
    IOCTL_PQ_SET_POSTCCS_ONOFF                    fpPQ_SetPostCCSOnOff;
    IOCTL_PQ_ADAPTIVE_TUNING                    fpPQ_AdaptiveTuning;
    IOCTL_PQ_FILMMODE_ANYCANDENCE_ENABLE        fpPQ_FilmMode_AnyCandence_Enable;
    IOCTL_PQ_SETH264_ONOFF                        fpPQ_SetH264_OnOff;
    IOCTL_PQ_SETG3D_ONOFF                        fpPQ_SetG3D_OnOff;
    IOCTL_PQ_SETMVC4KX1K_ONOFF                    fpPQ_SetMVC4kx1k_OnOff;
    IOCTL_PQ_SET_NETWORKMM_ONOFF                fpPQ_SetNetworkMM_OnOff;
    IOCTL_PQ_SETMM_ONOFF                        fpPQ_SetMM_OnOff;
    IOCTL_PQ_SET_RMVB_ONOFF                        fpPQ_SetRmvb_OnOff;
    IOCTL_PQ_SET_VIPBYPASS_ONOFF                fpPQ_SetVIPBypass_OnOff;
    IOCTL_PQ_SET_PEAKING_ONOFF                    fpPQ_SetPeaking_OnOff;
    IOCTL_PQ_SET_FAKE_OUT_ENABLE                fpPQ_SetFakeOutEnable;
    IOCTL_PQ_EBABLE_SCALERGAMMA                    fpPQ_EnableScalerGamma;
    IOCTL_PQ_SET_GAMMATBl                                fpPQ_SetGammaTbl;
    IOCTL_PQ_SET_1KTRAVELINGENABLE                         fpPQ_Set1KTravelingEnable;
//drvPQ_Text.c
    IOCTL_PQ_GET_SRC_TYPE_NAME                    fpPQ_GetSrcTypeName;
    IOCTL_PQ_GET_IP_NAME                        fpPQ_GetIPName;
    IOCTL_PQ_GET_TABLE_NAME                        fpPQ_GetTableName;
//drvbw.c
    IOCTL_PQ_BW_LOAD_INIT_TABLE                    fpPQ_BW_LoadInitTable;
    IOCTL_PQ_BW_LOAD_TABLE_BY_CONTEXT            fpPQ_BW_LoadTableByContext;
    IOCTL_PQ_BW_CUSMODE_LOAD_TABLE                           fpPQ_BW_CusMode_LoadTable;
    IOCTL_PQ_BW_RESET_TO_DEFAULT                fpPQ_BW_ResetToDefault;
    IOCTL_PQ_GETCONFIG                           fpPQ_GetConfig;
    IOCTL_PQ_SET_FRAMEBUFF_MODE                  fpPQ_SET_FrameBuff_Mode;
    // Device ID
    MS_U32 u32DeviceID;
}PQ_INSTANCE_PRIVATE;

////////////////////////////////////////////////////////////////////////////////
// Reduce driver code size
////////////////////////////////////////////////////////////////////////////////

#define _MODULE_PQ_U1_U2_RELATION \
        URELATION(fpPQ_Init,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_Exit,(IOCTL_PQ_EXIT)MDrv_PQ_Exit_U2,MDrv_PQ_Exit) \
        URELATION(fpPQ_SetPowerState,(IOCTL_PQ_SET_POWER_STATE)MDrv_PQ_SetPowerState_U2,MDrv_PQ_SetPowerState) \
        URELATION(fpPQ_GetIPNum,(IOCTL_PQ_GET_IP_NUM)MDrv_PQ_GetIPNum_U2,MDrv_PQ_GetIPNum) \
        URELATION(fpPQ_GetTableNum,(IOCTL_PQ_GET_TABLE_NUM)MDrv_PQ_GetTableNum_U2,MDrv_PQ_GetTableNum) \
        URELATION(fpPQ_GetCurrentTableIndex,(IOCTL_PQ_GET_CURRENT_TABLE_INDEX)MDrv_PQ_GetCurrentTableIndex_U2,MDrv_PQ_GetCurrentTableIndex) \
        URELATION(fpPQ_GetTableIndex,(IOCTL_PQ_GET_TABLE_INDEX)MDrv_PQ_GetTableIndex_U2,MDrv_PQ_GetTableIndex) \
        URELATION(fpPQ_GetCustomerTableIndex,(IOCTL_PQ_GET_CUSTOMER_TABLE_INDEX)MDrv_PQ_GetCustomerTableIndex_U2,MDrv_PQ_GetCustomerTableIndex) \
        URELATION(fpPQ_CloneTable,(IOCTL_PQ_CLONE_TABLE)MDrv_PQ_CloneTable_U2,MDrv_PQ_CloneTable) \
        URELATION(fpPQ_LoadTable,(IOCTL_PQ_LOAD_TABLE)MDrv_PQ_LoadTable_U2,MDrv_PQ_LoadTable) \
        URELATION(fpPQ_LoadCustomerTable,(IOCTL_PQ_LOAD_CUSTOMER_TABLE)MDrv_PQ_LoadCustomerTable_U2,MDrv_PQ_LoadCustomerTable) \
        URELATION(fpPQ_CheckSettings,(IOCTL_PQ_CHECK_SETTINGS)MDrv_PQ_CheckSettings_U2,MDrv_PQ_CheckSettings) \
        URELATION(fpPQ_MLoadEn,(IOCTL_PQ_SET_MLOAD_EN)MDrv_PQ_Set_MLoadEn_U2,MDrv_PQ_Set_MLoadEn) \
        URELATION(fpPQ_SkipDuplicatedSetting,(IOCTL_PQ_SKIP_DUPLICATED_SETTING)MDrv_PQ_SkipDuplicatedSetting_U2,MDrv_PQ_SkipDuplicatedSetting) \
        URELATION(fpPQ_GetSkipDuplicatedSettingStatus,(IOCTL_PQ_GET_SKIP_DUPLICATED_SETTING_STATUS)MDrv_PQ_GetSkipDuplicatedSettingStatus_U2,MDrv_PQ_GetSkipDuplicatedSettingStatus) \
        URELATION(fpPQ_LoadSettings,(IOCTL_PQ_LOAD_SETTINGS)MDrv_PQ_LoadSettings_U2,MDrv_PQ_LoadSettings) \
        URELATION(fpPQ_LoadCustomerSettings,(IOCTL_PQ_LOAD_CUSTONER_SETTINGS)MDrv_PQ_LoadCustomerSettings_U2,MDrv_PQ_LoadCustomerSettings) \
        URELATION(fpPQ_Set_DTVInfo,(IOCTL_PQ_SET_DTVINFO)MDrv_PQ_Set_DTVInfo_U2,MDrv_PQ_Set_DTVInfo) \
        URELATION(fpPQ_Set_MultiMediaInfo,(IOCTL_PQ_SET_MULTIMEDIAINFO)MDrv_PQ_Set_MultiMediaInfo_U2,MDrv_PQ_Set_MultiMediaInfo) \
        URELATION(fpPQ_Set_VDInfo,(IOCTL_PQ_SET_DVINFO)MDrv_PQ_Set_VDInfo_U2,MDrv_PQ_Set_VDInfo) \
        URELATION(fpPQ_Set_ModeInfo,(IOCTL_PQ_SET_MODEINFO)MDrv_PQ_Set_ModeInfo_U2,MDrv_PQ_Set_ModeInfo) \
        URELATION(fpPQ_SetHDMIInfo,(IOCTL_PQ_SET_HDMIINFO)MDrv_PQ_SetHDMIInfo_U2,MDrv_PQ_SetHDMIInfo) \
        URELATION(fpPQ_SetHDMI_PC,(IOCTL_PQ_SET_HDMI_PC)MDrv_PQ_SetHDMI_PC_U2,MDrv_PQ_SetHDMI_PC) \
        URELATION(fpPQ_GetHDMI_PC_Status,(IOCTL_PQ_GET_HDMI_PC_STATUS)MDrv_PQ_GetHDMI_PC_Status_U2,MDrv_PQ_GetHDMI_PC_Status) \
        URELATION(fpPQ_DesideSrcType,(IOCTL_PQ_DESIDE_SRC_TYPE)MDrv_PQ_DesideSrcType_U2,MDrv_PQ_DesideSrcType) \
        URELATION(fpPQ_GetInputSourceType,(IOCTL_PQ_GET_INPUTSOURCE_TYPE)MDrv_PQ_GetInputSourceType_U2,MDrv_PQ_GetInputSourceType) \
        URELATION(fpPQ_GetSrcType,(IOCTL_PQ_GET_SRC_TYPE)MDrv_PQ_GetSrcType_U2,MDrv_PQ_GetSrcType) \
        URELATION(fpPQ_GetDelayTimeByMemc,(IOCTL_PQ_GET_DELAY_TIME_BY_MEMC)MDrv_PQ_GetDelayTimeByMemc_U2,MDrv_PQ_GetDelayTimeByMemc) \
        URELATION(fpPQ_GetDelayTime,(IOCTL_PQ_GET_DELAY_TIME)MDrv_PQ_GetDelayTime_U2,MDrv_PQ_GetDelayTime) \
        URELATION(fpPQ_LoadScalingTable,(IOCTL_PQ_LOAD_SCALING_TABLE)MDrv_PQ_LoadScalingTable_U2,MDrv_PQ_LoadScalingTable) \
        URELATION(fpPQ_SetColorRange,(IOCTL_PQ_SET_COLOR_RANGE)MDrv_PQ_SetColorRange_U2,MDrv_PQ_SetColorRange) \
        URELATION(fpPQ_Get_MemYUVFmt,(IOCTL_PQ_MEM_YUVFMT)MDrv_PQ_Get_MemYUVFmt_U2,MDrv_PQ_Get_MemYUVFmt) \
        URELATION(fpPQ_SetCSC,(IOCTL_PQ_SET_CSC)MDrv_PQ_SetCSC_U2,MDrv_PQ_SetCSC) \
        URELATION(fpPQ_EnableMADIForce,(IOCTL_PQ_ENABLE_MADI_FORCE)MDrv_PQ_EnableMADIForce_U2,MDrv_PQ_EnableMADIForce) \
        URELATION(fpPQ_ReduceBW_ForPVR,(IOCTL_PQ_REDUCE_BW_FOR_PVR)MDrv_PQ_ReduceBW_ForPVR_U2,MDrv_PQ_ReduceBW_ForPVR) \
        URELATION(fpPQ_Set420upsampling,(IOCTL_PQ_SET_420UP_SAMPLEING)MDrv_PQ_Set420upsampling_U2,MDrv_PQ_Set420upsampling) \
        URELATION(fpPQ_SetFilmMode,(IOCTL_PQ_SET_FILEMODE)MDrv_PQ_SetFilmMode_U2,MDrv_PQ_SetFilmMode) \
        URELATION(fpPQ_SetNonLinearScaling,(IOCTL_PQ_SET_NONLINEAR_SCALING)MDrv_PQ_SetNonLinearScaling_U2,MDrv_PQ_SetNonLinearScaling) \
        URELATION(fpPQ_3DCloneforPIP,(IOCTL_PQ_3D_CLONE_FOR_PIP)MDrv_PQ_3DCloneforPIP_U2,MDrv_PQ_3DCloneforPIP) \
        URELATION(fpPQ_DisableFilmMode,(IOCTL_PQ_DISABLE_FILEMODE)MDrv_PQ_DisableFilmMode_U2,MDrv_PQ_DisableFilmMode) \
        URELATION(fpPQ_DisableUCFeature,(IOCTL_PQ_DISABLE_UC_FEATURE)MDrv_PQ_DisableUCFeature_U2,MDrv_PQ_DisableUCFeature) \
        URELATION(fpPQ_GetCurrentUCFeature,(IOCTL_PQ_GET_CURRENT_UC_FEATURE)MDrv_PQ_GetCurrentUCFeature_U2,MDrv_PQ_GetCurrentUCFeature) \
        URELATION(fpPQ_SetUCFeature,(IOCTL_PQ_SET_UC_FEATURE)MDrv_PQ_SetUCFeature_U2,MDrv_PQ_SetUCFeature) \
        URELATION(fpPQ_SetDMSV12L,(IOCTL_PQ_SET_DMSV12L)MDrv_PQ_SetDMSV12L_U2,MDrv_PQ_SetDMSV12L) \
        URELATION(fpPQ_GetDMSV12LFromXRuleTable,(IOCTL_PQ_GET_DMSV12L_FROM_XRULE_TABLE)MDrv_PQ_GetDMSV12LFromXRuleTable_U2,MDrv_PQ_GetDMSV12LFromXRuleTable) \
        URELATION(fpPQ_GetLibVer,(IOCTL_PQ_GET_LIB_VER)MDrv_PQ_GetLibVer_U2,MDrv_PQ_GetLibVer) \
        URELATION(fpPQ_GetInfo,(IOCTL_PQ_GET_INFO)MDrv_PQ_GetInfo_U2,MDrv_PQ_GetInfo) \
        URELATION(fpPQ_GetStatus,(IOCTL_PQ_GET_STATUS)MDrv_PQ_GetStatus_U2,MDrv_PQ_GetStatus) \
        URELATION(fpPQ_SetDbgLevel,(IOCTL_PQ_SET_DBGLEVEL)MDrv_PQ_SetDbgLevel_U2,MDrv_PQ_SetDbgLevel) \
        URELATION(fpPQ_MADiForceMotionC,(IOCTL_PQ_MADI_FORCE_MOTIONC)MDrv_PQ_MADiForceMotionC_U2,MDrv_PQ_MADiForceMotionC) \
        URELATION(fpPQ_MADiForceMotionY,(IOCTL_PQ_MADI_FORCE_MOTIONY)MDrv_PQ_MADiForceMotionY_U2,MDrv_PQ_MADiForceMotionY) \
        URELATION(fpPQ_MADiForceMotion,(IOCTL_PQ_MADI_FORCE_MOTION)MDrv_PQ_MADiForceMotion_U2,MDrv_PQ_MADiForceMotion) \
        URELATION(fpPQ_SetRFblMode,(IOCTL_PQ_SET_RFBL_MODE)MDrv_PQ_SetRFblMode_U2,MDrv_PQ_SetRFblMode) \
        URELATION(fpPQ_GetRFblMode,(IOCTL_PQ_GET_RFBL_MODE)MDrv_PQ_GetRFblMode_U2,MDrv_PQ_GetRFblMode) \
        URELATION(fpPQ_Get_VDSampling_Info,(IOCTL_PQ_GET_VDSAMPLING_INFO)MDrv_PQ_Get_VDSampling_Info_U2,MDrv_PQ_Get_VDSampling_Info) \
        URELATION(fpPQ_INTER_IOCTL,(IOCTL_PQ_INTER_IOCTL)MDrv_PQ_IOCTL_U2,MDrv_PQ_IOCTL) \
        URELATION(fpPQ_Get_ADCSampling_Info,(IOCTL_PQ_GET_ADC_SAMPLING_INTO)MDrv_PQ_Get_ADCSampling_Info_U2,MDrv_PQ_Get_ADCSampling_Info) \
        URELATION(fpPQ_Set3D_OnOff,(IOCTL_PQ_SET_3D_ONOFF)MDrv_PQ_Set3D_OnOff_U2,MDrv_PQ_Set3D_OnOff) \
        URELATION(fpPQ_GetCaps,(IOCTL_PQ_GET_CAPS)MDrv_PQ_GetCaps_U2,MDrv_PQ_GetCaps) \
        URELATION(fpPQ_PrintLoadTableInfo,(IOCTL_PQ_PRINT_LOADTABLE_INFO)MDRV_PQ_PrintLoadTableInfo_U2,MDRV_PQ_PrintLoadTableInfo) \
        URELATION(fpPQ_SetDS_OnOFF,(IOCTL_PQ_SETDS_ONOFF)MDrv_PQ_SetDS_OnOFF_U2,MDrv_PQ_SetDS_OnOFF) \
        URELATION(fpPQ_Get_RFBL_Info,(IOCTL_PQ_GET_RFBL_INFO)MDrv_PQ_Get_RFBL_Info_U2,MDrv_PQ_Get_RFBL_Info) \
        URELATION(fpPQ_CheckHDMode,(IOCTL_PQ_CHECK_HD_MODE)MDrv_PQ_CheckHDMode_U2,MDrv_PQ_CheckHDMode) \
        URELATION(fpPQ_Interlace_SD_mode,(IOCTL_PQ_GET_INTERLACE_SD_MODE)MDrv_PQ_Get_Interlace_SD_mode_U2,MDrv_PQ_Get_Interlace_SD_mode) \
        URELATION(fpPQ_SetDualViewState,(IOCTL_PQ_SET_DUAL_VIEW_STATE)MDrv_PQ_SetDualViewState_U2,MDrv_PQ_SetDualViewState) \
        URELATION(fpPQ_GetVGASubCaseForceTo422Mode,(IOCTL_PQ_GET_VGASUBCASE_FORCETO_422MODE)MDrv_PQ_GetVGASubCaseForceTo422Mode_U2,MDrv_PQ_GetVGASubCaseForceTo422Mode) \
        URELATION(fpPQ_GetDualViewState,(IOCTL_PQ_GET_DUAL_VIEW_STATE)MDrv_PQ_GetDualViewState_U2,MDrv_PQ_GetDualViewState) \
        URELATION(fpPQ_ForceVideoInputMode,(IOCTL_PQ_FORCE_VIDEO_INPUT_MODE)MDrv_PQ_ForceVideoInputMode_U2,MDrv_PQ_ForceVideoInputMode) \
        URELATION(fpPQ_IsForceVideoInputMode,(IOCTL_PQ_IS_FORCE_VIDEO_INPUTMODE)MDrv_PQ_IsForceVideoInputMode_U2,MDrv_PQ_IsForceVideoInputMode) \
        URELATION(fpPQ_Set_PointToPoint,(IOCTL_PQ_SET_POINT2POINT)MDrv_PQ_Set_PointToPoint_U2,MDrv_PQ_Set_PointToPoint) \
        URELATION(fpPQ_Get_PointToPoint,(IOCTL_PQ_GET_POINT2POINT)MDrv_PQ_Get_PointToPoint_U2,MDrv_PQ_Get_PointToPoint) \
        URELATION(fpPQ_Update_MemFormat,(IOCTL_PQ_UPDATE_MEMFORMAT)MDrv_PQ_Update_MemFormat_U2,MDrv_PQ_Update_MemFormat) \
        URELATION(fpPQ_GetHsize,(IOCTL_PQ_GET_HSIZE)MDrv_PQ_GetHsize_U2,MDrv_PQ_GetHsize) \
        URELATION(fpPQ_GetVsize,(IOCTL_PQ_GET_VSIZE)MDrv_PQ_GetVsize_U2,MDrv_PQ_GetVsize) \
        URELATION(fpPQ_IsInterlace,(IOCTL_PQ_IS_INTERLACE)MDrv_PQ_IsInterlace_U2,MDrv_PQ_IsInterlace) \
        URELATION(fpPQ_LOW_3dQuality,(IOCTL_PQ_LOW_3D_QUALITY)MDrv_PQ_LOW_3dQuality_U2,MDrv_PQ_LOW_3dQuality) \
        URELATION(fpPQ_MWE_SetEnhanceQuality,(IOCTL_PQ_MWE_SET_ENHANCE_QUALITY)MDrv_PQ_MWE_SetEnhanceQuality_U2,MDrv_PQ_MWE_SetEnhanceQuality) \
        URELATION(fpPQ_MWE_RestoreEnhanceQuality,(IOCTL_PQ_MWE_RESTORE_ENHANCE_QUALITY)MDrv_PQ_MWE_RestoreEnhanceQuality_U2,MDrv_PQ_MWE_RestoreEnhanceQuality) \
        URELATION(fpPQ_MWE_RestoreOffQuality,(IOCTL_PQ_MWE_RESTORE_OFF_QUALITY)MDrv_PQ_MWE_RestoreOffQuality_U2,MDrv_PQ_MWE_RestoreOffQuality) \
        URELATION(fpPQ_MWE_SetOffQuality,(IOCTL_PQ_MWE_SET_OFF_QUALITY)MDrv_PQ_MWE_SetOffQuality_U2,MDrv_PQ_MWE_SetOffQuality) \
        URELATION(fpPQ_MWE_CloneWindow,(IOCTL_PQ_MWE_CLONE_WINDOW)MDrv_PQ_MWE_CloneWindow_U2,MDrv_PQ_MWE_CloneWindow) \
        URELATION(fpPQ_3D_SettingForLBL,(IOCTL_PQ_3D_SETTING_FOR_LBL)MDrv_PQ_3D_SettingForLBL_U2,MDrv_PQ_3D_SettingForLBL) \
        URELATION(fpPQ_3D_CloneWindow,(IOCTL_PQ_3D_CLONE_WINDOW)MDrv_PQ_3D_CloneWindow_U2,MDrv_PQ_3D_CloneWindow) \
        URELATION(fpPQ_SetSelectCSC,(IOCTL_PQ_SET_SELECT_CSC)MDrv_PQ_SetSelectCSC_U2,MDrv_PQ_SetSelectCSC) \
        URELATION(fpPQ_GetR2YEqSelect,(IOCTL_PQ_GET_R2YEQ_SELECT)MDrv_PQ_GetR2YEqSelect_U2,MDrv_PQ_GetR2YEqSelect) \
        URELATION(fpPQ_GetMemFmtInGeneral,(IOCTL_PQ_GET_MEMFMT_IN_GENERAL)MDrv_PQ_GetMemFmtInGeneral_U2,MDrv_PQ_GetMemFmtInGeneral \
        URELATION(fpPQ_GetMADiForRFBL,(IOCTL_PQ_GET_MADI_FOR_RFBL)MDrv_PQ_GetMADiForRFBL_U2,MDrv_PQ_GetMADiForRFBL_U2) \
        URELATION(fpPQ_Patch2Rto4RForFieldPackingMode,(IOCTL_PQ_PATCH2RTO4R_FOR_FIELDPACKINGMODE)MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2,MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2) \
        URELATION(fpPQ_GetBPPInfoFromMADi,(IOCTL_PQ_GET_BPPINFO_FROM_MADI)MDrv_PQ_GetBPPInfoFromMADi_U2,MDrv_PQ_GetBPPInfoFromMADi) \
        URELATION(fpPQ_ChangeMemConfigFor3D,(IOCTL_PQ_CHANGE_MEMCONFIG_FOR3D)MDrv_PQ_ChangeMemConfigFor3D_U2,MDrv_PQ_ChangeMemConfigFor3D) \
        URELATION(fpPQ_ForceBPPForDynamicMemFmt,(IOCTL_PQ_FORCE_BPP_FOR_DYNAMIC_MEMFMT)MDrv_PQ_ForceBPPForDynamicMemFmt_U2,MDrv_PQ_ForceBPPForDynamicMemFmt) \
        URELATION(fpPQ_SuggestFrameNum,(IOCTL_PQ_SUGGEST_FRAMENUM)MDrv_PQ_SuggestFrameNum_U2,MDrv_PQ_SuggestFrameNum) \
        URELATION(fpPQ_SetGameMode,(IOCTL_PQ_SET_GAMEMODE)MDrv_PQ_SetGameMode_U2,MDrv_PQ_SetGameMode) \
        URELATION(fpPQ_GetGameMode_Status,(IOCTL_PQ_GET_GAMEMODE_STATUS)MDrv_PQ_GetGameMode_Status_U2,MDrv_PQ_GetGameMode_Status) \
        URELATION(fpPQ_SetBypassMode,(IOCTL_PQ_SET_BYPASS_MODE)MDrv_PQ_SetBypassMode_U2,MDrv_PQ_SetBypassMode) \
        URELATION(fpPQ_GetBypassModeStatus,(IOCTL_PQ_GET_BYPASS_MODE_STATUS)MDrv_PQ_GetBypassModeStatus_U2,MDrv_PQ_GetBypassModeStatus) \
        URELATION(fpPQ_GetVersion,(IOCTL_PQ_GET_VERSION)MDrv_PQ_GetVersion_U2,MDrv_PQ_GetVersion) \
        URELATION(fpPQ_Cus_GetVersion,(IOCTL_PQ_CUS_GET_VERSION)MDrv_PQ_Cus_GetVersion_U2,MDrv_PQ_Cus_GetVersion) \
        URELATION(fpPQ_SetGRuleStatus,(IOCTL_PQ_SET_GRULE_STATUS)MDrv_PQ_SetGRuleStatus_U2,MDrv_PQ_SetGRuleStatus) \
        URELATION(fpPQ_GetGRule_LvlNum,(IOCTL_PQ_GET_GRULE_LV1NUM)MDrv_PQ_GetGRule_LvlNum_U2,MDrv_PQ_GetGRule_LvlNum) \
        URELATION(fpPQ_GetGRule_GRuleNum,(IOCTL_PQ_GET_GRULE_GRULENUM)MDrv_PQ_GetGRule_GRuleNum_U2,MDrv_PQ_GetGRule_GRuleNum) \
        URELATION(fpPQ_GetGRule_LevelIndex,(IOCTL_PQ_GET_GRULE_LEVELINDEX)MDrv_PQ_GetGRule_LevelIndex_U2,MDrv_PQ_GetGRule_LevelIndex) \
        URELATION(fpPQ_GetGRule_IPIndex,(IOCTL_PQ_GET_GRULE_IPINDEX)MDrv_PQ_GetGRule_IPIndex_U2,MDrv_PQ_GetGRule_IPIndex) \
        URELATION(fpPQ_GetGRule_TableIndex,(IOCTL_PQ_GET_GRULE_TABLEINDEX)MDrv_PQ_GetGRule_TableIndex_U2,MDrv_PQ_GetGRule_TableIndex) \
        URELATION(fpPQ_GetCustomerGRule_IPIndex,(IOCTL_PQ_GET_CUSTOMERGRULE_IPINDEX)MDrv_PQ_GetCustomerGRule_IPIndex_U2,MDrv_PQ_GetCustomerGRule_IPIndex) \
        URELATION(fpPQ_GetGRule_CustomerTableIndex,(IOCTL_PQ_GET_CUSTOMERGRULE_TABLEINDEX)MDrv_PQ_GetGRule_CustomerTableIndex_U2,MDrv_PQ_GetGRule_CustomerTableIndex) \
        URELATION(fpPQ_Set_xvYCC_MatrixCoefficient ,(IOCTL_PQ_SET_XVYCC_MATRIXCOEFFICIENT)MDrv_PQ_Set_xvYCC_MatrixCoefficient_U2,MDrv_PQ_Set_xvYCC_MatrixCoefficient) \
        URELATION(fpPQ_Get_DNR_Whole_reg ,(IOCTL_PQ_GET_DNR_WHOLE_REG)MDrv_PQ_Get_DNR_Whole_Reg_U2,MDrv_PQ_Get_DNR_Whole_Reg) \
        URELATION(fpPQ_Up_Layer_Set_Config,(IOCTL_PQ_Up_Layer_Set_Config)MDrv_PQ_Up_Layer_Set_Config_U2,MDrv_PQ_Up_Layer_Set_Config) \
        URELATION(fpPQ_GetQmapExecutor,(IOCTL_PQ_GetQmapExecutor)MDrv_PQ_GetQmapExecutor_U2,MDrv_PQ_GetQmapExecutor) \
        URELATION(fpPQ_SetPhotoYUVStandard,(IOCTL_PQ_SetPhotoYUVStandard)MDrv_PQ_SetPhotoYUVStandard_U2,MDrv_PQ_SetPhotoYUVStandard) \
        URELATION(fpPQ_Ex_GetVersion,(IOCTL_PQ_Ex_GetVersion)MDrv_PQ_Ex_GetVersion_U2,MDrv_PQ_Ex_GetVersion) \
        URELATION(fpPQ_GetAdaptiveVersion,(IOCTL_PQ_GetAdaptiveVersion)MDrv_PQ_GetAdaptiveVersion_U2,MDrv_PQ_GetAdaptiveVersion) \
        URELATION(fpPQ_LoadUFSCSettings ,(IOCTL_PQ_LoadUFSCSettings)MDrv_PQ_LoadUFSCSettings_U2,MDrv_PQ_LoadUFSCSettings) \
        URELATION(fpPQ_LoadCFSettings,(IOCTL_PQ_LoadCFSettings)MDrv_PQ_LoadCFSettings_U2,MDrv_PQ_LoadCFSettings) \
        URELATION(fpPQ_Set_PqBypass,(IOCTL_PQ_SET_PQBYPASS)MDrv_PQ_Set_PqBypass_U2,MDrv_PQ_Set_PqBypass) \
        URELATION(fpPQ_Get_PqBypass,(IOCTL_PQ_GET_PQBYPASS)MDrv_PQ_Get_PqBypass_U2,MDrv_PQ_Get_PqBypass) \
        URELATION(fpPQ_Set_PqBin_Path,(IOCTL_PQ_SET_PQBIN_PATH)MDrv_PQ_SetPQBinPath_U2,MDrv_PQ_SetPQBinPath) \
        URELATION(fpPQ_LoadFilmModeTable,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_LoadDynamicContrastTable,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_LoadNRTable,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_LoadMPEGNRTable,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_Load_ULTRACLEAR_Table,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_Load_XVYCC_Table,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_Load_HDR_Table,(IOCTL_PQ_INIT)MDrv_PQ_Init_U2,MDrv_PQ_Init) \
        URELATION(fpPQ_GRULE_Get_Support_Status,(IOCTL_PQ_GRULE_GET_SUPPORT_STATUS)MDrv_PQ_GRULE_Get_Support_Status_U2,MDrv_PQ_GRULE_Get_Support_Status) \
        URELATION(fpPQ_SetMemFormat,(IOCTL_PQ_SET_MEMFORMAT)MDrv_PQ_SetMemFormat_U2,MDrv_PQ_SetMemFormat) \
        URELATION(fpPQ_ReduceBW_ForOSD,(IOCTL_PQ_REDUCEBW_ForOSD)MDrv_PQ_ReduceBW_ForOSD_U2,MDrv_PQ_ReduceBW_ForOSD) \
        URELATION(fpPQ_LoadPTPTable,(IOCTL_PQ_LOAD_PTP_TABLE)MDrv_PQ_LoadPTPTable_U2,MDrv_PQ_LoadPTPTable) \
        URELATION(fpPQ_Init_DisplayType,(IOCTL_PQ_INIT_DISPLAY_TYPE)MDrv_PQ_Init_DisplayType_U2,MDrv_PQ_Init_DisplayType) \
        URELATION(fpPQ_Set_DisplayType,(IOCTL_PQ_SET_DISPLAY_TYPE)MDrv_PQ_Set_DisplayType_U2,MDrv_PQ_Set_DisplayType) \
        URELATION(fpPQ_GetMADiFromOSDBWGrule,(IOCTL_PQ_GET_MADI_FROM_OSDBW_GRULE)MDrv_PQ_GetMADiFromOSDBWGrule_U2,MDrv_PQ_GetMADiFromOSDBWGrule) \
        URELATION(fpPQ_GetMADiInGeneral,(IOCTL_PQ_GET_MADI_IN_GENERAL)MDrv_PQ_GetMADiInGeneral_U2,MDrv_PQ_GetMADiInGeneral) \
        URELATION(fpPQ_Check_PointToPoint_Condition,(IOCTL_PQ_CHECK_POINT2POINT_CONDITION)MDrv_PQ_Check_PointToPoint_Condition_U2,MDrv_PQ_Check_PointToPoint_Condition) \
        URELATION(fpPQ_SetPostCCSOnOff,(IOCTL_PQ_SET_POSTCCS_ONOFF)MDrv_PQ_SetPostCCSOnOff_U2,MDrv_PQ_SetPostCCSOnOff) \
        URELATION(fpPQ_AdaptiveTuning,(IOCTL_PQ_ADAPTIVE_TUNING)MDrv_PQ_AdaptiveTuning_U2,MDrv_PQ_AdaptiveTuning) \
        URELATION(fpPQ_FilmMode_AnyCandence_Enable,(IOCTL_PQ_FILMMODE_ANYCANDENCE_ENABLE)MDrv_PQ_FilmMode_AnyCandence_Enable_U2,MDrv_PQ_FilmMode_AnyCandence_Enable) \
        URELATION(fpPQ_SetH264_OnOff,(IOCTL_PQ_SETH264_ONOFF)MDrv_PQ_SetH264_OnOff_U2,MDrv_PQ_SetH264_OnOff) \
        URELATION(fpPQ_SetG3D_OnOff,(IOCTL_PQ_SETG3D_ONOFF)MDrv_PQ_SetG3D_OnOff_U2,MDrv_PQ_SetG3D_OnOff) \
        URELATION(fpPQ_SetMVC4kx1k_OnOff,(IOCTL_PQ_SETMVC4KX1K_ONOFF)MDrv_PQ_SetMVC4kx1k_OnOff_U2,MDrv_PQ_SetMVC4kx1k_OnOff) \
        URELATION(fpPQ_SetNetworkMM_OnOff,(IOCTL_PQ_SET_NETWORKMM_ONOFF)MDrv_PQ_SetNetworkMM_OnOff_U2,MDrv_PQ_SetNetworkMM_OnOff) \
        URELATION(fpPQ_SetMM_OnOff,(IOCTL_PQ_SETMM_ONOFF)MDrv_PQ_SetMM_OnOff_U2,MDrv_PQ_SetMM_OnOff) \
        URELATION(fpPQ_SetRmvb_OnOff,(IOCTL_PQ_SET_RMVB_ONOFF)MDrv_PQ_SetRmvb_OnOff_U2,MDrv_PQ_SetRmvb_OnOff) \
        URELATION(fpPQ_SetVIPBypass_OnOff,(IOCTL_PQ_SET_VIPBYPASS_ONOFF)MDrv_PQ_SetVIPBypass_OnOff_U2,MDrv_PQ_SetVIPBypass_OnOff) \
        URELATION(fpPQ_SetPeaking_OnOff,(IOCTL_PQ_SET_PEAKING_ONOFF)MDrv_PQ_SetPeaking_OnOff_U2,MDrv_PQ_SetPeaking_OnOff) \
        URELATION(fpPQ_SetFakeOutEnable,(IOCTL_PQ_SET_FAKE_OUT_ENABLE)MDrv_PQ_SetFakeOutEnable_U2,MDrv_PQ_SetFakeOutEnable) \
        URELATION(fpPQ_GetTableIndex_Ex,(IOCTL_PQ_GET_TABLE_INDEX_EX)MDrv_PQ_GetTableIndex_Ex_U2,MDrv_PQ_GetTableIndex_Ex) \
        URELATION(fpPQ_LoadTable_Ex,(IOCTL_PQ_LOAD_TABLE_EX)MDrv_PQ_LoadTable_Ex_U2,MDrv_PQ_LoadTable_Ex) \
        URELATION(fpPQ_Demo_CloneWindow,(IOCTL_PQ_DEMO_CLONE_WINDOW)MDrv_PQ_Demo_CloneWindow_U2,MDrv_PQ_Demo_CloneWindow) \
        URELATION(fpPQ_GetGRule_LvlNum_Ex,(IOCTL_PQ_GET_GRULE_LVLNUM_EX)MDrv_PQ_GetGRule_LvlNum_Ex_U2,MDrv_PQ_GetGRule_LvlNum_Ex) \
        URELATION(fpPQ_GetGRule_LevelIndex_Ex,(IOCTL_PQ_GET_GRULE_LEVEILNDEX_EX)MDrv_PQ_GetGRule_LevelIndex_Ex_U2,MDrv_PQ_GetGRule_LevelIndex_Ex) \
        URELATION(fpPQ_GetGRule_IPIndex_Ex,(IOCTL_PQ_GET_GRULE_IPINDEX_EX)MDrv_PQ_GetGRule_IPIndex_Ex_U2,MDrv_PQ_GetGRule_IPIndex_Ex) \
        URELATION(fpPQ_GetGRule_TableIndex_Ex,(IOCTL_PQ_GET_GRULE_TABLEINDEX_EX)MDrv_PQ_GetGRule_TableIndex_Ex_U2,MDrv_PQ_GetGRule_TableIndex_Ex) \
        URELATION(fpPQ_EnableScalerGamma,(IOCTL_PQ_EBABLE_SCALERGAMMA)MDrv_PQ_EnableScalerGamma_U2,MDrv_PQ_EnableScalerGamma) \
        URELATION(fpPQ_SetGammaTbl,(IOCTL_PQ_SET_GAMMATBl)MDrv_PQ_SetGammaTbl_U2,MDrv_PQ_SetGammaTbl) \
        URELATION(fpPQ_Set1KTravelingEnable,(IOCTL_PQ_SET_1KTRAVELINGENABLE)MDrv_PQ_Set1KTravelingEnable_U2,MDrv_PQ_Set1KTravelingEnable) \
        URELATION(fpPQ_GetSrcTypeName,(IOCTL_PQ_GET_SRC_TYPE_NAME)MDrv_PQ_GetSrcTypeName_U2,MDrv_PQ_GetSrcTypeName) \
        URELATION(fpPQ_GetIPName,(IOCTL_PQ_GET_IP_NAME)MDrv_PQ_GetIPName_U2,MDrv_PQ_GetIPName) \
        URELATION(fpPQ_GetTableName,(IOCTL_PQ_GET_TABLE_NAME)MDrv_PQ_GetTableName_U2,MDrv_PQ_GetTableName) \
        URELATION(fpPQ_BW_LoadInitTable,(IOCTL_PQ_BW_LOAD_INIT_TABLE)MDrv_BW_LoadInitTable_U2,MDrv_BW_LoadInitTable) \
        URELATION(fpPQ_BW_LoadTableByContext,(IOCTL_PQ_BW_LOAD_TABLE_BY_CONTEXT)MDrv_BW_LoadTableByContext_U2,MDrv_BW_LoadTableByContext) \
        URELATION(fpPQ_BW_ResetToDefault,(IOCTL_PQ_BW_RESET_TO_DEFAULT)MDrv_BW_ResetToDefault_U2,MDrv_BW_resetToDefault) \
        URELATION(fpPQ_BW_CusMode_LoadTable,(IOCTL_PQ_BW_CUSMODE_LOAD_TABLE)MDrv_BW_CusMode_LoadTable_U2,MDrv_BW_CusMode_LoadTable) \
        URELATION(fpPQ_GetConfig,(IOCTL_PQ_GETCONFIG)MDrv_PQ_GetConfig_U2,MDrv_PQ_GetConfig) \
        URELATION(fpPQ_SET_FrameBuff_Mode,(IOCTL_PQ_SET_FRAMEBUFF_MODE)MDrv_PQ_SetFrameBuffMode_U2,MDrv_PQ_SetFrameBuffMode)

#define PQ_FILE_PATH_LENGTH 30
typedef struct
{
    char pm_PQ_CUSTOMER_PATH[PQ_FILE_PATH_LENGTH];
    char pm_PQ_DEFAULT_PATH[PQ_FILE_PATH_LENGTH];
    char pm_PQ_INI_PATH[PQ_FILE_PATH_LENGTH];
    char pm_PQ_BANDWIDTH_PATH[PQ_FILE_PATH_LENGTH];
}ST_PQBIN_PATH;

typedef struct
{
    MS_BOOL bResourceRegistered;
    ST_PQBIN_PATH stPQBin_Path;

#ifdef UFO_XC_PQ_SUPPORT_INITVAR_INTO_SHAREMEMORY
    MS_PQ_Init_Info PQ_InitInfo;
#endif
}PQ_RESOURCE_PRIVATE;

// U2_INTERFACE
typedef struct
{
    MS_U16 reg;
}PQ_REGS_SAVE_AREA;

void MDrv_PQ_Init_U2(void* pInstance,MS_PQ_Init_Info *pstPQInitInfo);
MS_BOOL MDrv_PQ_Exit_U2(void* pInstance);
MS_U32 MDrv_PQ_SetPowerState_U2(void* pInstance,EN_POWER_MODE enPowerState);
MS_U16 MDrv_PQ_GetIPNum_U2(void* pInstance,PQ_WIN eWindow);
MS_U16 MDrv_PQ_GetTableNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16PQIPIdx);
MS_U16 MDrv_PQ_GetCurrentTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16PQIPIdx);
MS_U16 MDrv_PQ_GetTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx);
MS_U16 MDrv_PQ_GetCustomerTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx);
//MS_U16 MDrv_PQ_GetXRuleIPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIP);
//MS_U16 MDrv_PQ_GetXRuleTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP);
//MS_U16 MDrv_PQ_GetXRuleIPNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16XRuleType);
void MDrv_PQ_LoadTableData_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_U8 *pTable, MS_U16 u16TableSize);
void MDrv_PQ_CloneTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQMainIPIdx, MS_U16 u16PQSubIPIdx);
void MDrv_PQ_LoadTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx);
void MDrv_PQ_LoadCustomerTable_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx);
void MDrv_PQ_CheckSettings_U2(void* pInstance,PQ_WIN eWindow);
#if (ENABLE_PQ_MLOAD)
void MDrv_PQ_Set_MLoadEn_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEn);
#endif
MS_BOOL MDrv_PQ_SkipDuplicatedSetting_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bSkip);
MS_BOOL MDrv_PQ_GetSkipDuplicatedSettingStatus_U2(void* pInstance,PQ_WIN eWindow);
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
void MDrv_PQ_SetFrcmCbCrSwap_U2(void* pInstance,PQ_WIN eWindow);
#endif
void MDrv_PQ_LoadSettings_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_LoadCustomerSettings_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_Set_DTVInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_Dtv_Info *pstPQDTVInfo);
void MDrv_PQ_Set_MultiMediaInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_MuliMedia_Info *pstPQMMInfo);
void MDrv_PQ_Set_VDInfo_U2(void* pInstance,PQ_WIN eWindow, MS_PQ_Vd_Info *pstPQVDInfo);
void MDrv_PQ_Set_ModeInfo_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo);
void MDrv_PQ_SetHDMIInfo_U2(void* pInstance,PQ_WIN eWindow, const MS_PQ_Hdmi_Info* const pstPQHDMIInfo);
void MDrv_PQ_SetHDMI_PC_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bIsTrue);
MS_BOOL MDrv_PQ_GetHDMI_PC_Status_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_DesideSrcType_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType);
PQ_INPUT_SOURCE_TYPE MDrv_PQ_GetInputSourceType_U2(void* pInstance,PQ_WIN eWindow);
MS_U16 MDrv_PQ_GetSrcType_U2(void* pInstance,PQ_WIN eWindow);
MS_U16 MDrv_PQ_GetDelayTimeByMemc_U2(void* pInstance, SCALER_WIN eWindow, MS_U32 u32MemcFrameDelay);
MS_U16 MDrv_PQ_GetDelayTime_U2(void* pInstance,SCALER_WIN eWindow);
MS_BOOL MDrv_PQ_LoadScalingTable_U2(void* pInstance,PQ_WIN eWindow,MS_U8 eXRuleType,MS_BOOL bPreV_ScalingDown,MS_BOOL bInterlace,MS_BOOL bColorSpaceYUV,MS_U16 u16InputSize,MS_U16 u16SizeAfterScaling);
void MDrv_PQ_SetColorRange_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bColorRange0_255);
MS_BOOL MDrv_PQ_Get_CSC_XRuleIdx_U2(void* pInstance,MS_BOOL *pbInputColorSpaceRGB,MS_U16 * pu16DoCSC,PQ_WIN eWindow,PQ_FOURCE_COLOR_FMT enForceColor,MS_BOOL bLoadPQTable);
MS_BOOL MDrv_PQ_Get_MemYUVFmt_U2(void* pInstance,PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor);
MS_BOOL MDrv_PQ_SetCSC_U2(void* pInstance,PQ_WIN eWindow, PQ_FOURCE_COLOR_FMT enForceColor);
#if (PQ_XRULE_DB_ENABLE == 1)
void MDrv_PQ_Set_DBRule_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16MADi_Idx);
#endif
void MDrv_PQ_EnableMADIForce_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bFullMotion);
void MDrv_PQ_ReduceBW_ForPVR_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bPVR_On);
void MDrv_PQ_Set420upsampling_U2(void* pInstance,PQ_WIN eWindow,MS_BOOL bFBL,MS_BOOL bPreV_ScalingDown,MS_U16 u16V_CropStart);
void MDrv_PQ_SetFilmMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
void MDrv_PQ_SetNonLinearScaling_U2(void* pInstance,PQ_WIN eWindow, MS_U8 u8Level, MS_BOOL bEnable);
void MDrv_PQ_3DCloneforPIP_U2(void* pInstance,MS_BOOL bIpSync);
void MDrv_PQ_DisableFilmMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bOn);
MS_BOOL MDrv_PQ_DisableUCFeature_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_GetCurrentUCFeature_U2(void* pInstance,PQ_WIN eWindow,MS_U16 *pu16TabIdx_MADi,MS_U16 *pu16TabIdx_MADi_Mot,MS_U16 *pu16TabIdx_MADi_DFK,MS_U16 *pu16TabIdx_MADi_SST,MS_U16 *pu16TabIdx_MADi_EODiW,MS_U16 *pu16TabIdx_UCNR,MS_U16 *pu16TabIdx_UCDi,MS_U16 *pu16TabIdx_UCCTL,MS_U16 *pu16TabIdx_SwDriver,MS_U16 *pu16TabIdx_PostCCS,MS_U16 *pu16TabIdx_DHD);
MS_BOOL MDrv_PQ_GetUCFeatureFromPQ_U2(void* pInstance,PQ_WIN eWindow,MS_U16 *pu16TabIdx_MADi_DFK,MS_U16 *pu16TabIdx_MADi_SST,MS_U16 *pu16TabIdx_MADi_EODiW,MS_U16 *pu16TabIdx_UCNR,MS_U16 *pu16TabIdx_UCDi,MS_U16 *pu16TabIdx_UCCTL,MS_U16 *pu16TabIdx_SwDriver,MS_U16 *pu16TabIdx_PostCCS,MS_U16 *pu16TabIdx_DHD);
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
MS_BOOL MDrv_PQ_EnableUCFeature_U2(void* pInstance,PQ_WIN eWindow);
#endif
MS_BOOL MDrv_PQ_SetUCFeature_U2(void* pInstance,PQ_WIN eWindow,MS_U16 u16TabIdx_MADi,MS_U16 u16TabIdx_MADi_Mot,MS_U16 u16TabIdx_MADi_DFK,MS_U16 u16TabIdx_MADi_SST,MS_U16 u16TabIdx_MADi_EODiW,MS_U16 u16TabIdx_UCNR,MS_U16 u16TabIdx_UCDi,MS_U16 u16TabIdx_UCCTL,MS_U16 u16TabIdx_SwDriver,MS_U16 u16TabIdx_PostCCS,MS_U16 u16TabIdx_DHD);
MS_BOOL MDrv_PQ_SetDMSV12L_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
MS_BOOL MDrv_PQ_GetDMSV12LFromXRuleTable_U2(void* pInstance,PQ_WIN eWindow);
E_DRVPQ_ReturnValue MDrv_PQ_GetLibVer_U2(void* pInstance,const MSIF_Version **ppVersion);
PQ_ReturnValue MDrv_PQ_GetInfo_U2(void* pInstance,MS_PQ_INFO *pInfo);
MS_BOOL  MDrv_PQ_GetStatus_U2(void* pInstance,MS_PQ_Status *pStatus);
MS_BOOL  MDrv_PQ_SetDbgLevel_U2(void* pInstance,MS_U16 u16DbgSwitch);
void MDrv_PQ_MADiForceMotionC_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
void MDrv_PQ_MADiForceMotionY_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
void MDrv_PQ_MADiForceMotion_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnableY, MS_U16 u16DataY,
                             MS_BOOL bEnableC, MS_U16 u16DataC);
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
void MDrv_PQ_SetColorThroughMode_U2(void* pInstance, PQ_WIN eWindow, MS_BOOL bEn);
#endif
void MDrv_PQ_SetRFblMode_U2(void* pInstance,MS_BOOL bEnable, MS_BOOL bFilm);
void MDrv_PQ_GetRFblMode_U2(void* pInstance,MS_PQ_RFBL_INFO * pstInfo);
#if (PQ_ENABLE_VD_SAMPLING)
MS_BOOL MDrv_PQ_Get_VDSampling_Info_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, PQ_VIDEOSTANDARD_TYPE eStandard, MS_PQ_VD_Sampling_Info *pInfo);
#endif
MS_BOOL MDrv_PQ_IOCTL_U2(void* pInstance,PQ_WIN eWindow, MS_U32 u32Flag, void *pBuf, MS_U32 u32BufSize);
MS_PQ_ADC_SAMPLING_TYPE MDrv_PQ_Get_ADCSampling_Info_U2(void* pInstance,PQ_WIN eWindow, PQ_INPUT_SOURCE_TYPE enInputSourceType, MS_PQ_Mode_Info *pstPQModeInfo);
MS_BOOL MDrv_PQ_Set3D_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_GetCaps_U2(void* pInstance,MS_PQ_CAP_INFO *pInfo);
void MDRV_PQ_PrintLoadTableInfo_U2(void* pInstance,MS_PQ_PQLOADTABLE_WIN enPQWin);
MS_BOOL MDrv_PQ_SetDS_OnOFF_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEn);
MS_BOOL MDrv_PQ_Get_RFBL_Info_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_CheckHDMode_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Get_Interlace_SD_mode_U2(void* pInstance);
MS_BOOL MDrv_PQ_SetDualViewState_U2(void* pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_PQ_GetVGASubCaseForceTo422Mode_U2(void* pInstance);
MS_BOOL MDrv_PQ_GetDualViewState_U2(void* pInstance);
MS_BOOL MDrv_PQ_ForceVideoInputMode_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow);
MS_BOOL MDrv_PQ_IsForceVideoInputMode_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Set_PointToPoint_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,MS_BOOL bEnable,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Get_PointToPoint_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Set_PqBypass_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,MS_BOOL bEnable,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Get_PqBypass_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_SetPQBinPath_U2(void* pInstance,PQ_BIN_PATH ePqBinPath,MS_U8 u8size,char* b_PQBinFilePath);
MS_BOOL MDrv_PQ_Update_MemFormat_U2(void* pInstance);
MS_U16 MDrv_PQ_GetHsize_U2(void* pInstance,PQ_WIN enPQWin);
MS_U16 MDrv_PQ_GetVsize_U2(void* pInstance,PQ_WIN enPQWin);
MS_BOOL MDrv_PQ_IsInterlace_U2(void* pInstance,PQ_WIN enPQWin);
MS_BOOL MDrv_PQ_LOW_3dQuality_U2(void* pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_PQ_MWE_SetEnhanceQuality_U2(void* pInstance);
MS_BOOL MDrv_PQ_MWE_RestoreEnhanceQuality_U2(void* pInstance);
MS_BOOL MDrv_PQ_MWE_RestoreOffQuality_U2(void* pInstance);
MS_BOOL MDrv_PQ_MWE_SetOffQuality_U2(void* pInstance);
void MDrv_PQ_MWE_CloneWindow_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_3D_SettingForLBL_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_3D_CloneWindow_U2(void* pInstance,PQ_3DTYPE enPQ3DType, PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Get_SensioMode_U2(void* pInstance);
void  MDrv_PQ_Set_SensioMode_U2(void* pInstance,MS_BOOL  bEnable);
void  MDrv_PQ_Set_DIPF_U2(void* pInstance);
void MDrv_PQ_SetSelectCSC_U2(void* pInstance,MS_U16 u16selection, PQ_WIN eWindow);
MS_BOOL MDrv_PQ_GetR2YEqSelect_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_GetMemFmtInGeneral_U2(void* pInstance,PQ_WIN eWindow,MS_BOOL bMemFmt422,MS_U16 *pu16TabIdx_MemFormat,MS_U16 *pu16TabIdx_444To422,MS_U16 *pu16TabIdx_422To444);
MS_BOOL MDrv_PQ_GetMADiForRFBL_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bFBL, MS_U16 *pu16TabIdx_MADi_Motion, MS_U16 *pu16TabIdx_MADi);
MS_BOOL MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx_MADi);
MS_BOOL MDrv_PQ_GetBPPInfoFromMADi_U2(void* pInstance,PQ_WIN eWindow,MS_BOOL bMemFmt422,MS_U16 u16TabIdx_MADi,MS_U16 u16TabIdx_MemFormat,MS_U8 *pu8BitsPerPixel,PQ_DEINTERLACE_MODE *peDeInterlaceMode,MS_U8 *pu8FrameCount);
MS_BOOL MDrv_PQ_ChangeMemConfigFor3D_U2(void* pInstance,PQ_WIN eWindow,MS_BOOL bMemFmt422,MS_U16 *pu16TabIdx_MemFormat,MS_U16 *pu16TabIdx_444To422,MS_U16 *pu16TabIdx_422To444,MS_U8 *pu8BitsPerPixel,MS_U16 *pu16TabIdx_MADi_Motion,MS_U16 *pu16TabIdx_MADi);
MS_BOOL MDrv_PQ_ForceBPPForDynamicMemFmt_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_U8 *pu8BitsPerPixel);
MS_BOOL MDrv_PQ_SuggestFrameNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx_MADi, MS_U8 u8FrameCount);
MS_BOOL MDrv_PQ_SetGameMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
MS_BOOL MDrv_PQ_GetGameMode_Status_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_SetBypassMode_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
MS_BOOL MDrv_PQ_GetBypassModeStatus_U2(void* pInstance,PQ_WIN eWindow);
char*  MDrv_PQ_GetVersion_U2(void* pInstance,PQ_WIN eWindow);
char*  MDrv_PQ_Cus_GetVersion_U2(void* pInstance,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_SetGRuleStatus_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GruleType, MS_U16 u16GruleStatus);
MS_U16 MDrv_PQ_GetGRule_LvlNum_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType);
MS_U16 MDrv_PQ_GetGRule_GRuleNum_U2(void* pInstance,PQ_WIN eWindow);
MS_U16 MDrv_PQ_GetGRule_LevelIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex);
MS_U16 MDrv_PQ_GetGRule_IPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex);
MS_U16 MDrv_PQ_GetGRule_TableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex);
MS_U16 MDrv_PQ_GetCustomerGRule_IPIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex);
MS_U16 MDrv_PQ_GetGRule_CustomerTableIndex_U2(void* pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex);
MS_BOOL MDrv_PQ_Set_xvYCC_MatrixCoefficient_U2(void* pInstance,MS_U64 u64Red_x,   MS_U64 u64Red_y,MS_U64 u64Green_x, MS_U64 u64Green_y,MS_U64 u64Blue_x,  MS_U64 u64Blue_y,MS_U64 u64White_x, MS_U64 u64White_y,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_Set_xvYCC_MatrixEnable_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow);
MS_U16 MDrv_PQ_Get_DNR_Whole_Reg_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_Config_Lite_SN_U2(void* pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_PQ_Up_Layer_Set_Config_U2(void* pInstance,MS_U16 uplayerSetting, PQ_MISC_GROUP group);
MS_PQ_EXECUTOR MDrv_PQ_GetQmapExecutor_U2(void* pInstance,MS_PQ_FUNCTION_TYPE eFunctionType);
void MDrv_PQ_SetPhotoYUVStandard_U2(void* pInstance,PQ_WIN eWindow, PQ_YUV_STD enStd);
char* MDrv_PQ_Ex_GetVersion_U2(void* pInstance,PQ_WIN eWindow);
MS_U32 MDrv_PQ_GetAdaptiveVersion_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_PQ_LoadUFSCSettings_U2(void* pInstance,PQ_WIN eWindow);
#if (ENABLE_PQ_BIN_CF)
void MDrv_PQ_LoadCFSettings_U2(void* pInstance,PQ_WIN eWindow);
#endif
MS_U16 MDrv_PQ_GetTableIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16IPIdx, PQ_ENGINE_TYPE ePQEngineType);
void MDrv_PQ_LoadTable_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, PQ_ENGINE_TYPE ePQEngineType);
MS_BOOL MDrv_PQ_Demo_CloneWindow_U2(void *pInstance, PQ_WIN eWindow, MS_U8 u8Mode);
MS_U16 MDrv_PQ_GetGRule_LvlNum_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, PQ_ENGINE_TYPE ePQEngineType);
MS_U16 MDrv_PQ_GetGRule_LevelIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleLevelIndex, PQ_ENGINE_TYPE ePQEngineType);
MS_U16 MDrv_PQ_GetGRule_IPIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType);
MS_U16 MDrv_PQ_GetGRule_TableIndex_Ex_U2(void *pInstance,PQ_WIN eWindow, MS_U16 u16GRuleType, MS_U16 u16PQ_NRIdx, MS_U16 u16GRuleIPIndex, PQ_ENGINE_TYPE ePQEngineType);
void MDrv_PQ_EnableScalerGamma_U2(void *pInstance, PQ_WIN eWindow, MS_BOOL bEnable);
void MDrv_PQ_SetGammaTbl_U2(void *pInstance, APIPNL_GAMMA_TYPE enGammType, MS_U8 **pu8GammTable, APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode);
//drvPQ_cus.c
void MDrv_PQ_LoadFilmModeTable_U2(void* pInstance,PQ_WIN eWindow, PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType);
void MDrv_PQ_LoadDynamicContrastTable_U2(void* pInstance,PQ_WIN eWindow, PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType);
void MDrv_PQ_LoadNRTable_U2(void* pInstance,PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType);
void MDrv_PQ_LoadMPEGNRTable_U2(void* pInstance,PQ_WIN eWindow, PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType);
void MDrv_PQ_Load_ULTRACLEAR_Table_U2(void* pInstance,PQ_WIN eWindow, PQ_FEATURE_SWITCH_TYPE enCtrlType);
void MDrv_PQ_Load_XVYCC_Table_U2(void* pInstance,PQ_WIN eWindow, PQ_XVYCC_TYPE enXvyccType);
void MDrv_PQ_Load_HDR_Table_U2(void *pInstance,PQ_WIN eWindow, PQ_HDR_MODE enHdrMode);
MS_BOOL MDrv_PQ_GRULE_Get_Support_Status_U2(void* pInstance,PQ_SUPPORTED_TYPE enType);
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_BOOL bFBL, MS_U8 *pu8BitsPerPixel);
void MDrv_PQ_ReduceBW_ForOSD_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bOSD_On);
void MDrv_PQ_LoadPTPTable_U2(void* pInstance,PQ_WIN eWindow, PQ_PTP_FUNCTION_TYPE enPTP_Type);
void MDrv_PQ_Init_DisplayType_U2(void* pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType);
void MDrv_PQ_Set_DisplayType_U2(void* pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType);
MS_BOOL MDrv_PQ_GetMADiFromOSDBWGrule_U2(void* pInstance,PQ_WIN eWindow,MS_U16 *pu16TabIdx_MADi,MS_U16 *pu16TabIdx_MADi_Motion);
MS_BOOL MDrv_PQ_GetMADiInGeneral_U2(void* pInstance,PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi, MS_U16 *pu16TabIdx_MADi_Motion);
MS_BOOL MDrv_PQ_Check_PointToPoint_Condition_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,PQ_WIN eWindow);
MS_BOOL MDrv_PQ_SetPostCCSOnOff_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable);
void MDrv_PQ_AdaptiveTuning_U2(void* pInstance);
void MDrv_PQ_FilmMode_AnyCandence_Enable_U2(void* pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_PQ_SetH264_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_SetG3D_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_SetMVC4kx1k_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_SetNetworkMM_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_SetMM_OnOff_U2(void* pInstance,MS_BOOL bEn);
MS_BOOL MDrv_PQ_SetRmvb_OnOff_U2(void* pInstance,MS_BOOL bEn, PQ_WIN ePQWindow);
MS_BOOL MDrv_PQ_SetVIPBypass_OnOff_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow);
MS_BOOL MDrv_PQ_SetPeaking_OnOff_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow);
MS_BOOL MDrv_PQ_SetFakeOutEnable_U2(void* pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_PQ_Set1KTravelingEnable_U2(void* pInstance,MS_BOOL bEn);
//drvPQ_Text.c
char* MDrv_PQ_GetSrcTypeName_U2(void *pInstance,PQ_WIN eWindow);
char* MDrv_PQ_GetIPName_U2(void *pInstance,MS_U8 u8PQIPIdx);
char* MDrv_PQ_GetTableName_U2(void *pInstance,MS_U8 u8PQIPIdx, MS_U8 u8TabIdx );
//drvbw.c
void MDrv_BW_LoadInitTable_U2(void *pInstance);
void MDrv_BW_LoadTableByContext_U2(void* pInstance,PQ_WIN eWindow);
void MDrv_BW_ResetToDefault_U2(void* pInstance);
void MDrv_BW_CusMode_LoadTable_U2(void *pInstance, MS_U8 u8TabIdx);
MS_BOOL MDrv_PQ_GetConfig_U2(void* pInstance, MS_PQ_Init_Info *pstPQInitInfo);
void PQRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PQOpen(void** ppInstance,MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 PQIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 PQClose(void* pInstance);

MS_U32 _MDrv_PQ_GET_DEVICE_NUM(void);

void MDrv_PQ_SetFrameBuffMode_U2(void* pInstance,MS_BOOL bFrameBuffLess);

#ifdef __cplusplus
}
#endif
#undef _API_PQ_PRIV_H_
#endif
