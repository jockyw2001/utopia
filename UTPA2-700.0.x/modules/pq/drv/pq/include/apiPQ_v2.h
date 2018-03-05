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

#ifndef _APIPQ_V2_H_
#define _APIPQ_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "MsTypes.h"
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "apiPNL.h"
//******** ENUM DEFINITIONS ********//
typedef enum
{
    E_PQ_POOL_ID_INTERNAL = 0,
    E_PQ_POOL_ID_INTERNAL1 = 1,

    E_PQ_POOL_ID_NUM,
    E_PQ_POOL_ID_MAX = E_PQ_POOL_ID_NUM,
} E_PQ_POOL_ID;

/// Define PQ device ID
typedef enum
{
    E_PQ_DEVICE_ID_0 = 0,
    E_PQ_DEVICE_ID_1 = 1,
    E_PQ_DEVICE_ID_NUM,
    E_PQ_DEVICE_ID_MAX = E_PQ_DEVICE_ID_NUM,
} E_PQ_DEVICE_ID;

typedef enum
{
    // APIPQ
    //drvPQ.c
    E_PQ_CMD_NONE = 0,
    E_PQ_CMD_INIT,
    E_PQ_CMD_EXIT,
    E_PQ_CMD_SET_POWER_STATE,
    E_PQ_CMD_GET_IP_NUM,
    E_PQ_CMD_GET_TABLE_NUM,
    E_PQ_CMD_GET_CURRENT_TABLE_INDEX,
    E_PQ_CMD_GET_TABLE_INDEX,
    E_PQ_CMD_GET_CUSTOMER_TABLE_INDEX,
    //E_PQ_CMD_GET_XRULE_IP_INDEX,
    //E_PQ_CMD_GET_XRULE_TABLE_INDEX,
    //E_PQ_CMD_GET_XRULE_IP_NUM,
    E_PQ_CMD_LOAD_TABLE_DATA,
    E_PQ_CMD_CLONE_TABLE,
    E_PQ_CMD_LOAD_TABLE,
    E_PQ_CMD_LOAD_CUSTOMER_TABLE,
    E_PQ_CMD_CHECK_SETTINGS,
#if (ENABLE_PQ_MLOAD)
    E_PQ_CMD_SET_MLOAD_EN,
#endif
    E_PQ_CMD_SKIP_DUPLICATED_SETTING,
    E_PQ_CMD_GET_SKIP_DUPLICATED_SETTING_STATUS,
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
    E_PQ_CMD_SET_FRCM_CB_CR_SWAP,
#endif
    E_PQ_CMD_LOAD_SETTINGS,
    E_PQ_CMD_LOAD_CUSTONER_SETTINGS,
    E_PQ_CMD_SET_DTVINFO,
    E_PQ_CMD_SET_MULTIMEDIAINFO,
    E_PQ_CMD_SET_VDINFO,
    E_PQ_CMD_SET_MODEINFO,
    E_PQ_CMD_SET_HDMIINFO,
    E_PQ_CMD_SET_HDMI_PC,
    E_PQ_CMD_GET_HDMI_PC_STATUS,
    E_PQ_CMD_DESIDE_SRC_TYPE,
    E_PQ_CMD_GET_INPUTSOURCE_TYPE,
    E_PQ_CMD_GET_SRC_TYPE,
    E_PQ_CMD_GET_DELAY_TIME_BY_MEMC,
    E_PQ_CMD_GET_DELAY_TIME,
    E_PQ_CMD_LOAD_SCALING_TABLE,
    E_PQ_CMD_SET_COLOR_RANGE,
    E_PQ_CMD_GET_CSC_XRULEIDX,
    E_PQ_CMD_GET_MEM_YUVFMT,
    E_PQ_CMD_SET_CSC,
#if (PQ_XRULE_DB_ENABLE == 1)
    E_PQ_CMD_SET_DBRULE,
#endif
    E_PQ_CMD_ENABLE_MADI_FORCE,
    E_PQ_CMD_REDUCE_BW_FOR_PVR,
    E_PQ_CMD_SET_420UP_SAMPLEING,
    E_PQ_CMD_SET_FILEMODE,
    E_PQ_CMD_SET_NONLINEAR_SCALING,
    E_PQ_CMD_3D_CLONE_FOR_PIP,
    E_PQ_CMD_DISABLE_FILEMODE,
    E_PQ_CMD_DISABLE_UC_FEATURE,
    E_PQ_CMD_GET_CURRENT_UC_FEATURE,
    E_PQ_CMD_GET_UC_FEATURE_FROM_PQ,
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    E_PQ_CMD_ENABLE_UC_FEATURE,
#endif
    E_PQ_CMD_SET_UC_FEATURE,
    E_PQ_CMD_SET_DMSV12L,
    E_PQ_CMD_GET_DMSV12L_FROM_XRULE_TABLE,
    E_PQ_CMD_GET_LIB_VER,
    E_PQ_CMD_GET_INFO,
    E_PQ_CMD_GET_STATUS,
    E_PQ_CMD_SET_DBGLEVEL,
    E_PQ_CMD_MADI_FORCE_MOTIONC,
    E_PQ_CMD_MADI_FORCE_MOTIONY,
    E_PQ_CMD_SET_RFBL_MODE,
    E_PQ_CMD_GET_RFBL_MODE,
#if (PQ_ENABLE_VD_SAMPLING)
    E_PQ_CMD_GET_VDSAMPLING_INFO,
#endif
    E_PQ_CMD_INTER_IOCTL,
    E_PQ_CMD_GET_ADC_SAMPLING_INTO,
    E_PQ_CMD_SET_3D_ONOFF,
    E_PQ_CMD_GET_CAPS,
    E_PQ_CMD_PRINT_LOADTABLE_INFO,
    E_PQ_CMD_SETDS_ONOFF,
    E_PQ_CMD_GET_RFBL_INFO,
    E_PQ_CMD_CHECK_HD_MODE,
    E_PQ_CMD_GET_INTERLACE_SD_MODE,
    E_PQ_CMD_SET_DUAL_VIEW_STATE,
    E_PQ_CMD_GET_VGASUBCASE_FORCETO_422MODE,
    E_PQ_CMD_GET_DUAL_VIEW_STATE,
    E_PQ_CMD_FORCE_VIDEO_INPUT_MODE,
    E_PQ_CMD_IS_FORCE_VIDEO_INPUTMODE,
    E_PQ_CMD_SET_POINT2POINT,
    E_PQ_CMD_GET_POINT2POINT,
    E_PQ_CMD_UPDATE_MEMFORMAT,
    E_PQ_CMD_GET_HSIZE,
    E_PQ_CMD_GET_VSIZE,
    E_PQ_CMD_IS_INTERLACE,
    E_PQ_CMD_LOW_3D_QUALITY,
    E_PQ_CMD_MWE_SET_ENHANCE_QUALITY,
    E_PQ_CMD_MWE_RESTORE_ENHANCE_QUALITY,
    E_PQ_CMD_MWE_RESTORE_OFF_QUALITY,
    E_PQ_CMD_MWE_SET_OFF_QUALITY,
    E_PQ_CMD_MWE_CLONE_WINDOW,
    E_PQ_CMD_3D_SETTING_FOR_LBL,
    E_PQ_CMD_3D_CLONE_WINDOW,
    E_PQ_CMD_GET_SENSIOMODE,
    E_PQ_CMD_SET_SENSIOMODE,
    E_PQ_CMD_SET_DIPF,
    E_PQ_CMD_SET_SELECT_CSC,
    E_PQ_CMD_GET_R2YEQ_SELECT,
    E_PQ_CMD_GET_MEMFMT_IN_GENERAL,
    E_PQ_CMD_GET_MADI_FOR_RFBL,
    E_PQ_CMD_PATCH2RTO4R_FOR_FIELDPACKINGMODE,
    E_PQ_CMD_GET_BPPINFO_FROM_MADI,
    E_PQ_CMD_CHANGE_MEMCONFIG_FOR3D,
    E_PQ_CMD_FORCE_BPP_FOR_DYNAMIC_MEMFMT,
    E_PQ_CMD_SUGGEST_FRAMENUM,
    E_PQ_CMD_SET_GAMEMODE,
    E_PQ_CMD_GET_GAMEMODE_STATUS,
    E_PQ_CMD_SET_BYPASS_MODE,
    E_PQ_CMD_GET_BYPASS_MODE_STATUS,
    E_PQ_CMD_GET_VERSION,
    E_PQ_CMD_CUS_GET_VERSION,
    E_PQ_CMD_SET_GRULE_STATUS,
    E_PQ_CMD_GET_GRULE_LV1NUM,
    E_PQ_CMD_GET_GRULE_GRULENUM,
    E_PQ_CMD_GET_GRULE_LEVELINDEX,
    E_PQ_CMD_GET_GRULE_IPINDEX,
    E_PQ_CMD_GET_GRULE_TABLEINDEX,
    E_PQ_CMD_GET_CUSTOMERGRULE_IPINDEX,
    E_PQ_CMD_GET_CUSTOMERGRULE_TABLEINDEX,
    E_PQ_CMD_SET_XVYCC_MATRIXCOEFFICIENT,
    E_PQ_CMD_SET_XVYCC_MATRIXENABLE,
    E_PQ_CMD_GET_DNR_WHOLE_REG,
    E_PQ_CMD_UP_LAYER_SET_CONFIG,
    E_PQ_CMD_GET_QMAP_EXECUTOR,
    E_PQ_CMD_SET_PHOTO_YUV_STD,
    E_PQ_CMD_EX_GET_VERSION,
    E_PQ_CMD_GET_ADAPTVIE_VERSION,
       E_PQ_CMD_LOAD_CUSTOMER_TABLE_EX,
       E_PQ_CMD_LOAD_TABLE_EX,
    E_PQ_CMD_LOAD_UFSC_SETTING,
#if (ENABLE_PQ_BIN_CF)
    E_PQ_CMD_LOAD_CF_SETTING,
#endif
       E_PQ_CMD_GET_TABLE_INDEX_EX,
       E_PQ_CMD_DEMO_CLONE_WINDOW,
       E_PQ_CMD_ENABLE_SCALER_GAMMA,
       E_PQ_CMD_SET_GAMMA_TBl,
       E_PQ_CMD_GET_GRULE_LV1NUM_EX,
       E_PQ_CMD_GET_GRULE_LEVELINDEX_EX,
       E_PQ_CMD_GET_GRULE_IPINDEX_EX,
       E_PQ_CMD_GET_GRULE_TABLEINDEX_EX,
       E_PQ_CMD_GET_PQBYPASS,
       E_PQ_CMD_SET_PQBYPASS,
       E_PQ_CMD_SET_PQBIN_PATH,
//drvPQ_cus.c
    E_PQ_CMD_LOAD_FILMMODE_TABLE,
    E_PQ_CMD_LOAD_DYNAMIC_CONTRAST_TABLE,
    E_PQ_CMD_LOAD_NR_TABLE,
    E_PQ_CMD_LOAD_MPEG_NR_TABLE,
    E_PQ_CMD_LOAD_ULTRACLEAR_TABLE,
    E_PQ_CMD_LOAD_XVYCC_TABLE,
    E_PQ_CMD_LOAD_HDR_TABLE,
    E_PQ_CMD_GRULE_GET_SUPPORT_STATUS,
    E_PQ_CMD_SET_MEMFORMAT,
    E_PQ_CMD_REDUCEBW_ForOSD,
    E_PQ_CMD_LOAD_PTP_TABLE,
    E_PQ_CMD_INIT_DISPLAY_TYPE,
    E_PQ_CMD_SET_DISPLAY_TYPE,
    E_PQ_CMD_GET_MADI_FROM_OSDBW_GRULE,
    E_PQ_CMD_GET_MADI_IN_GENERAL,
    E_PQ_CMD_CHECK_POINT2POINT_CONDITION,
    E_PQ_CMD_SET_POSTCCS_ONOFF,
    E_PQ_CMD_ADAPTIVE_TUNING,
    E_PQ_CMD_FILMMODE_ANYCANDENCE_ENABLE,
    E_PQ_CMD_SETH264_ONOFF,
    E_PQ_CMD_SETG3D_ONOFF,
    E_PQ_CMD_SETMVC4KX1K_ONOFF,
    E_PQ_CMD_SET_NETWORKMM_ONOFF,
    E_PQ_CMD_SETMM_ONOFF,
    E_PQ_CMD_SET_RMVB_ONOFF,
    E_PQ_CMD_SET_VIPBYPASS_ONOFF,
    E_PQ_CMD_SET_PEAKING_ONOFF,
    E_PQ_CMD_SET_FAKE_OUT_ENABLE,
    E_PQ_CMD_SET_1KTRAVELINGENABLE,
//drvPQ_Text.c
    E_PQ_CMD_GET_SRC_TYPE_NAME,
    E_PQ_CMD_GET_IP_NAME,
    E_PQ_CMD_GET_TABLE_NAME,
//drvbw.c
    E_PQ_CMD_BW_LOAD_INIT_TABLE,
    E_PQ_CMD_BW_LOAD_TABLE_BY_CONTEXT,
    E_PQ_CMD_BW_RESET_TO_DEFAULT,
    E_PQ_CMD_BW_CUSMODE_LOADTABLE,

#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
    E_PQ_CMD_MADI_FORCE_MOTION,
#endif

#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
    E_PQ_CMD_SET_COLOR_THROUGH_MODE,
#endif

    E_PQ_CMD_GETCONFIG,
    E_PQ_CMD_SET_FRAMEBUFF_MODE,
    E_PQ_CMD_NUM,
} E_PQ_IOCTL_CMDS;

//drvPQ.c
typedef struct DLL_PACKED
{
    MS_PQ_Init_Info *pstPQInitInfo;
} stPQ_INIT,*pstPQ_INIT;
typedef struct DLL_PACKED
{
    PQ_ReturnValue eRet;
    MS_PQ_Init_Info *pstPQInitInfo;
} stPQ_GetConfig,*pstPQ_GetConfig;
typedef struct
{
    EN_POWER_MODE enPowerState;
    MS_U32 u32Ret;
} stPQ_SetPowerState,*pstPQ_SetPowerState;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16Ret;
} stPQ_GetIPNum,*pstPQ_GetIPNum;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16PQIPIdx;
    MS_U16 u16Ret;
} stPQ_GetTableNum,*pstPQ_GetTableNum;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16PQIPIdx;
    MS_U16 u16Ret;
} stPQ_GetCurrentTableIndex,*pstPQ_GetCurrentTableIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16PQIPIdx;
    MS_U16 u16Ret;
} stPQ_GetTableIndex,*pstPQ_GetTableIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16PQIPIdx;
    MS_U16 u16Ret;
} stPQ_GetCustomerTableIndex,*pstPQ_GetCustomerTableIndex;
//typedef struct
//{
//    PQ_WIN eWindow;
//    MS_U16 u16XRuleType;
//    MS_U16 u16XRuleIP;
//    MS_U16 u16Ret;
//} stPQ_GetXRuleIPIndex,*pstPQ_GetXRuleIPIndex;
//typedef struct
//{
//    PQ_WIN eWindow;
//    MS_U16 u16XRuleType;
//    MS_U16 u16XRuleIdx;
//    MS_U16 u16XRuleIP;
//    MS_U16 u16Ret;
//} stPQ_GetXRuleTableIndex,*pstPQ_GetXRuleTableIndex;
//typedef struct
//{
//    PQ_WIN eWindow;
//    MS_U16 u16XRuleType;
//    MS_U16 u16Ret;
//} stPQ_GetXRuleIPNum,*pstPQ_GetXRuleIPNum;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx;
    MS_U16 u16PQIPIdx;
    MS_U8 *pTable;
    MS_U16 u16TableSize;
} stPQ_LoadTableData,*pstPQ_LoadTableData;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx;
    MS_U16 u16PQMainIPIdx;
    MS_U16 u16PQSubIPIdx;
} stPQ_CloneTable,*pstPQ_CloneTable;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx;
    MS_U16 u16PQIPIdx;
} stPQ_LoadTable,*pstPQ_LoadTable;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx;
    MS_U16 u16PQIPIdx;
} stPQ_LoadCustomerTable,*pstPQ_LoadCustomerTable;
typedef struct
{
    PQ_WIN eWindow;
} stPQ_CheckSettings,*pstPQ_CheckSettings;
#if (ENABLE_PQ_MLOAD)
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEn;
} stPQ_Set_MLoadEn,*pstPQ_Set_MLoadEn;
#endif
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bSkip;
    MS_BOOL bReturnValue;
} stPQ_SkipDuplicatedSetting,*pstPQ_SkipDuplicatedSetting;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetSkipDuplicatedSettingStatus,*pstPQ_GetSkipDuplicatedSettingStatus;
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
typedef struct
{
    PQ_WIN eWindow;
} stPQ_SetFrcmCbCrSwap,*pstPQ_SetFrcmCbCrSwap;
#endif
typedef struct
{
    PQ_WIN eWindow;
} stPQ_LoadSettings,*pstPQ_LoadSettings;
typedef struct
{
    PQ_WIN eWindow;
} stPQ_LoadCustomerSettings,*pstPQ_LoadCustomerSettings;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_PQ_Dtv_Info *pstPQDTVInfo;
} stPQ_Set_DTVInfo,*pstPQ_Set_DTVInfo;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_PQ_MuliMedia_Info *pstPQMMInfo;
} stPQ_Set_MultiMediaInfo,*pstPQ_Set_MultiMediaInfo;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_PQ_Vd_Info *pstPQVDInfo;
} stPQ_Set_VDInfo,*pstPQ_Set_VDInfo;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    PQ_INPUT_SOURCE_TYPE enInputSourceType;
    MS_PQ_Mode_Info *pstPQModeInfo;
} stPQ_Set_ModeInfo,*pstPQ_Set_ModeInfo;
typedef struct
{
    PQ_WIN eWindow;
    MS_PQ_Hdmi_Info pstPQHDMIInfo;
    //const MS_PQ_Hdmi_Info* const pstPQHDMIInfo;
} stPQ_SetHDMIInfo,*pstPQ_SetHDMIInfo;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bIsTrue;
} stPQ_SetHDMI_PC,*pstPQ_SetHDMI_PC;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetHDMI_PC_Status,*pstPQ_GetHDMI_PC_Status;
typedef struct
{
    PQ_WIN eWindow;
    PQ_INPUT_SOURCE_TYPE enInputSourceType;
} stPQ_DesideSrcType,*pstPQ_DesideSrcType;
typedef struct
{
    PQ_WIN eWindow;
    PQ_INPUT_SOURCE_TYPE bReturnValue;
} stPQ_GetInputSourceType,*pstPQ_GetInputSourceType;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16Ret;
} stPQ_GetSrcType,*pstPQ_GetSrcType;
typedef struct
{
    SCALER_WIN eWindow;
    MS_U32 u32MemcFrameDelay;
    MS_U16 u16Ret;
} stPQ_GetDelayTimeByMemc,*pstPQ_GetDelayTimeByMemc;
typedef struct
{
    SCALER_WIN eWindow;
    MS_U16 u16Ret;
} stPQ_GetDelayTime,*pstPQ_GetDelayTime;
typedef struct
{
    PQ_WIN eWindow;
    MS_U8 eXRuleType;
    MS_BOOL bPreV_ScalingDown;
    MS_BOOL bInterlace;
    MS_BOOL bColorSpaceYUV;
    MS_U16 u16InputSize;
    MS_U16 u16SizeAfterScaling;
    MS_BOOL bReturnValue;
} stPQ_LoadScalingTable,*pstPQ_LoadScalingTable;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bColorRange0_255;
} stPQ_SetColorRange,*pstPQ_SetColorRange;
typedef struct DLL_PACKED
{
    MS_BOOL *pbInputColorSpaceRGB;
    MS_U16 * pu16DoCSC;
    PQ_WIN eWindow;
    PQ_FOURCE_COLOR_FMT enForceColor;
    MS_BOOL bLoadPQTable;
    MS_BOOL bReturnValue;
} stPQ_Get_CSC_XRuleIdx,*pstPQ_Get_CSC_XRuleIdx;
typedef struct
{
    PQ_WIN eWindow;
    PQ_FOURCE_COLOR_FMT enForceColor;
    MS_BOOL bReturnValue;
} stPQ_Get_MemYUVFmt,*pstPQ_Get_MemYUVFmt;
typedef struct
{
    PQ_WIN eWindow;
    PQ_FOURCE_COLOR_FMT enForceColor;
    MS_BOOL bReturnValue;
} stPQ_SetCSC,*pstPQ_SetCSC;
#if (PQ_XRULE_DB_ENABLE == 1)
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16MADi_Idx;
} stPQ_Set_DBRule,*pstPQ_Set_DBRule;
#endif
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bFullMotion;
} stPQ_EnableMADIForce,*pstPQ_EnableMADIForce;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bPVR_On;
} stPQ_ReduceBW_ForPVR,*pstPQ_ReduceBW_ForPVR;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bFBL;
    MS_BOOL bPreV_ScalingDown;
    MS_U16 u16V_CropStart;
} stPQ_Set420upsampling,*pstPQ_Set420upsampling;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
} stPQ_SetFilmMode,*pstPQ_SetFilmMode;
typedef struct
{
    PQ_WIN eWindow;
    MS_U8 u8Level;
    MS_BOOL bEnable;
} stPQ_SetNonLinearScaling,*pstPQ_SetNonLinearScaling;
typedef struct
{
    MS_BOOL bIpSync;
} stPQ_3DCloneforPIP,*pstPQ_3DCloneforPIP;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bOn;
} stPQ_DisableFilmMode,*pstPQ_DisableFilmMode;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_DisableUCFeature,*pstPQ_DisableUCFeature;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U16 *pu16TabIdx_MADi;
    MS_U16 *pu16TabIdx_MADi_Mot;
    MS_U16 *pu16TabIdx_MADi_DFK;
    MS_U16 *pu16TabIdx_MADi_SST;
    MS_U16 *pu16TabIdx_MADi_EODiW;
    MS_U16 *pu16TabIdx_UCNR;
    MS_U16 *pu16TabIdx_UCDi;
    MS_U16 *pu16TabIdx_UCCTL;
    MS_U16 *pu16TabIdx_SwDriver;
    MS_U16 *pu16TabIdx_PostCCS;
    MS_U16 *pu16TabIdx_DHD;
    MS_BOOL bReturnValue;
} stPQ_GetCurrentUCFeature,*pstPQ_GetCurrentUCFeature;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U16 *pu16TabIdx_MADi_DFK;
    MS_U16 *pu16TabIdx_MADi_SST;
    MS_U16 *pu16TabIdx_MADi_EODiW;
    MS_U16 *pu16TabIdx_UCNR;
    MS_U16 *pu16TabIdx_UCDi;
    MS_U16 *pu16TabIdx_UCCTL;
    MS_U16 *pu16TabIdx_SwDriver;
    MS_U16 *pu16TabIdx_PostCCS;
    MS_U16 *pu16TabIdx_DHD;
    MS_BOOL bReturnValue;
} stPQ_GetUCFeatureFromPQ,*pstPQ_GetUCFeatureFromPQ;
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_EnableUCFeature,*pstPQ_EnableUCFeature;
#endif
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 pu16TabIdx_MADi;
    MS_U16 pu16TabIdx_MADi_Mot;
    MS_U16 pu16TabIdx_MADi_DFK;
    MS_U16 pu16TabIdx_MADi_SST;
    MS_U16 pu16TabIdx_MADi_EODiW;
    MS_U16 pu16TabIdx_UCNR;
    MS_U16 pu16TabIdx_UCDi;
    MS_U16 pu16TabIdx_UCCTL;
    MS_U16 pu16TabIdx_SwDriver;
    MS_U16 pu16TabIdx_PostCCS;
    MS_U16 pu16TabIdx_DHD;
    MS_BOOL bReturnValue;
} stPQ_SetUCFeature,*pstPQ_SetUCFeature;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetDMSV12L,*pstPQ_SetDMSV12L;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetDMSV12LFromXRuleTable,*pstPQ_GetDMSV12LFromXRuleTable;
typedef struct DLL_PACKED
{
    const MSIF_Version **ppVersion;
    E_DRVPQ_ReturnValue bReturnValue;
} stPQ_GetLibVer,*pstPQ_GetLibVer;
typedef struct DLL_PACKED
{
    MS_PQ_INFO *pInfo;
    PQ_ReturnValue bReturnValue;
} stPQ_GetInfo,*pstPQ_GetInfo;
typedef struct DLL_PACKED
{
    MS_PQ_Status *pStatus;
    MS_BOOL bReturnValue;
} stPQ_GetStatus,*pstPQ_GetStatus;
typedef struct
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturnValue;
} stPQ_SetDbgLevel,*pstPQ_SetDbgLevel;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
} stPQ_MADiForceMotionC,*pstPQ_MADiForceMotionC;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
} stPQ_MADiForceMotionY,*pstPQ_MADiForceMotionY;

#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnableY;
    MS_U16 u16DataY;
    MS_BOOL bEnableC;
    MS_U16 u16DataC;
} stPQ_MADiForceMotion,*pstPQ_MADiForceMotion;
#endif

#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEn;
} stPQ_SetColorThroughMode,*pstPQ_SetColorThroughMode;
#endif

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bFilm;
} stPQ_SetRFblMode,*pstPQ_SetRFblMode;
typedef struct DLL_PACKED
{
    MS_PQ_RFBL_INFO * pstInfo;
} stPQ_GetRFblMode,*pstPQ_GetRFblMode;
#if (PQ_ENABLE_VD_SAMPLING)
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    PQ_INPUT_SOURCE_TYPE enInputSourceType;
    PQ_VIDEOSTANDARD_TYPE eStandard;
    MS_PQ_VD_Sampling_Info *pInfo;
    MS_BOOL bReturnValue;
} stPQ_Get_VDSampling_Info,*pstPQ_Get_VDSampling_Info;
#endif
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U32 u32Flag;
    void *pBuf;
    MS_U32 u32BufSize;
    MS_BOOL bReturnValue;
} stPQ_INTER_IOCTL,*pstPQ_INTER_IOCTL;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    PQ_INPUT_SOURCE_TYPE enInputSourceType;
    MS_PQ_Mode_Info *pstPQModeInfo;
    MS_PQ_ADC_SAMPLING_TYPE bReturnValue;
} stPQ_Get_ADCSampling_Info,*pstPQ_Get_ADCSampling_Info;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_Set3D_OnOff,*pstPQ_Set3D_OnOff;
typedef struct DLL_PACKED
{
    MS_PQ_CAP_INFO *pInfo;
    MS_BOOL bReturnValue;
} stPQ_GetCaps,*pstPQ_GetCaps;
typedef struct
{
    MS_PQ_PQLOADTABLE_WIN enPQWin;
} stPQ_PrintLoadTableInfo,*pstPQ_PrintLoadTableInfo;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetDS_OnOFF,*pstPQ_SetDS_OnOFF;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Get_RFBL_Info,*pstPQ_Get_RFBL_Info;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_CheckHDMode,*pstPQ_CheckHDMode;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_Get_Interlace_SD_mode,*pstPQ_Get_Interlace_SD_mode;
typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetDualViewState,*pstPQ_SetDualViewState;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_GetVGASubCaseForceTo422Mode,*pstPQ_GetVGASubCaseForceTo422Mode;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_GetDualViewState,*pstPQ_GetDualViewState;
typedef struct
{
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_ForceVideoInputMode,*pstPQ_ForceVideoInputMode;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_IsForceVideoInputMode,*pstPQ_IsForceVideoInputMode;
typedef struct DLL_PACKED
{
    XC_SETWIN_INFO *pstXC_SetWin_Info;
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Set_PointToPoint,*pstPQ_Set_PointToPoint;
typedef struct DLL_PACKED
{
    XC_SETWIN_INFO *pstXC_SetWin_Info;
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Set_PqBypass,*pstPQ_Set_PqBypass;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Get_PointToPoint,*pstPQ_Get_PointToPoint;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Get_PqBypass,*pstPQ_Get_PqBypass;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_Update_MemFormat,*pstPQ_Update_MemFormat;
typedef struct
{
    PQ_WIN enPQWin;
    MS_U16 u16Ret;
} stPQ_GetHsize,*pstPQ_GetHsize;
typedef struct
{
    PQ_WIN enPQWin;
    MS_U16 u16Ret;
} stPQ_GetVsize,*pstPQ_GetVsize;
typedef struct
{
    PQ_WIN enPQWin;
    MS_BOOL bReturnValue;
} stPQ_IsInterlace,*pstPQ_IsInterlace;
typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_LOW_3dQuality,*pstPQ_LOW_3dQuality;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_MWE_SetEnhanceQuality,*pstPQ_MWE_SetEnhanceQuality;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_MWE_RestoreEnhanceQuality,*pstPQ_MWE_RestoreEnhanceQuality;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_MWE_RestoreOffQuality,*pstPQ_MWE_RestoreOffQuality;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_MWE_SetOffQuality,*pstPQ_MWE_SetOffQuality;
typedef struct
{
    PQ_WIN eWindow;
} stPQ_MWE_CloneWindow,*pstPQ_MWE_CloneWindow;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_3D_SettingForLBL,*pstPQ_3D_SettingForLBL;
typedef struct
{
    PQ_3DTYPE enPQ3DType;
    PQ_WIN eWindow;
} stPQ_3D_CloneWindow,*pstPQ_3D_CloneWindow;
typedef struct
{
    MS_BOOL bReturnValue;
} stPQ_Get_SensioMode,*pstPQ_Get_SensioMode;
typedef struct
{
    MS_BOOL  bEnable;
} stPQ_Set_SensioMode,*pstPQ_Set_SensioMode;
typedef struct
{
    MS_U16 u16selection;
    PQ_WIN eWindow;
} stPQ_SetSelectCSC,*pstPQ_SetSelectCSC;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetR2YEqSelect,*pstPQ_GetR2YEqSelect;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bMemFmt422;
    MS_U16 *pu16TabIdx_MemFormat;
    MS_U16 *pu16TabIdx_444To422;
    MS_U16 *pu16TabIdx_422To444;
    MS_BOOL bReturnValue;
} stPQ_GetMemFmtInGeneral,*pstPQ_GetMemFmtInGeneral;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bFBL;
    MS_U16 *pu16TabIdx_MADi_Motion;
    MS_U16 *pu16TabIdx_MADi;
    MS_BOOL bReturnValue;
} stPQ_GetMADiForRFBL,*pstPQ_GetMADiForRFBL;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx_MADi;
    MS_BOOL bReturnValue;
} stPQ_Patch2Rto4RForFieldPackingMode,*pstPQ_Patch2Rto4RForFieldPackingMode;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bMemFmt422;
    MS_U16 u16TabIdx_MADi;
    MS_U16 u16TabIdx_MemFormat;
    MS_U8 *pu8BitsPerPixel;
    PQ_DEINTERLACE_MODE *peDeInterlaceMode;
    MS_U8 *pu8FrameCount;
    MS_BOOL bReturnValue;
} stPQ_GetBPPInfoFromMADi,*pstPQ_GetBPPInfoFromMADi;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bMemFmt422;
    MS_U16 *pu16TabIdx_MemFormat;
    MS_U16 *pu16TabIdx_444To422;
    MS_U16 *pu16TabIdx_422To444;
    MS_U8 *pu8BitsPerPixel;
    MS_U16 *pu16TabIdx_MADi_Motion;
    MS_U16 *pu16TabIdx_MADi;
    MS_BOOL bReturnValue;
} stPQ_ChangeMemConfigFor3D,*pstPQ_ChangeMemConfigFor3D;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bMemFmt422;
    MS_U8 *pu8BitsPerPixel;
    MS_BOOL bReturnValue;
} stPQ_ForceBPPForDynamicMemFmt,*pstPQ_ForceBPPForDynamicMemFmt;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx_MADi;
    MS_U8 u8FrameCount;
    MS_BOOL bReturnValue;
} stPQ_SuggestFrameNum,*pstPQ_SuggestFrameNum;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetGameMode,*pstPQ_SetGameMode;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetGameMode_Status,*pstPQ_GetGameMode_Status;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetBypassMode,*pstPQ_SetBypassMode;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_GetBypassModeStatus,*pstPQ_GetBypassModeStatus;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    char *u8Ret;
} stPQ_GetVersion,*pstPQ_GetVersion;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    char *u8Ret;
} stPQ_Cus_GetVersion,*pstPQ_Cus_GetVersion;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GruleType;
    MS_U16 u16GruleStatus;
    MS_BOOL bReturnValue;
} stPQ_SetGRuleStatus,*pstPQ_SetGRuleStatus;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16Ret;
} stPQ_GetGRule_LvlNum,*pstPQ_GetGRule_LvlNum;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16Ret;
} stPQ_GetGRule_GRuleNum,*pstPQ_GetGRule_GRuleNum;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16GRuleLevelIndex;
    MS_U16 u16Ret;
} stPQ_GetGRule_LevelIndex,*pstPQ_GetGRule_LevelIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16GRuleIPIndex;
    MS_U16 u16Ret;
} stPQ_GetGRule_IPIndex,*pstPQ_GetGRule_IPIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16PQ_NRIdx;
    MS_U16 u16GRuleIPIndex;
    MS_U16 u16Ret;
} stPQ_GetGRule_TableIndex,*pstPQ_GetGRule_TableIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16GRuleIPIndex;
    MS_U16 u16Ret;
} stPQ_GetCustomerGRule_IPIndex,*pstPQ_GetCustomerGRule_IPIndex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16PQ_NRIdx;
    MS_U16 u16GRuleIPIndex;
    MS_U16 u16Ret;
} stPQ_GetGRule_CustomerTableIndex,*pstPQ_GetGRule_CustomerTableIndex;
typedef struct
{
    MS_U64 u64Red_x;
    MS_U64 u64Red_y;
    MS_U64 u64Green_x;
    MS_U64 u64Green_y;
    MS_U64 u64Blue_x;
    MS_U64 u64Blue_y;
    MS_U64 u64White_x;
    MS_U64 u64White_y;
    PQ_WIN eWindow;
    MS_U16 bReturnValue;
} stPQ_Set_xvYCC_MatrixCoefficient,*pstPQ_Set_xvYCC_MatrixCoefficient;
typedef struct
{
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_U16 bReturnValue;
} stPQ_Set_xvYCC_MatrixEnable,*pstPQ_Set_xvYCC_MatrixEnable;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16Ret;
} stPQ_GetDnrWholeReg,*pstPQ_GetDnrWholeReg;
typedef struct
{
    MS_U16 uplayerSetting;
    PQ_MISC_GROUP group;
    MS_BOOL bReturnValue;
} stPQ_Up_Layer_Set_Config,*pstPQ_Up_Layer_Set_Config;
typedef struct
{
    MS_PQ_FUNCTION_TYPE eFunctionType;
    MS_PQ_EXECUTOR bReturnValue;
} stPQ_GetQmapExecutor,*pstPQ_GetQmapExecutor;
typedef struct
{
    PQ_WIN eWindow;
    PQ_YUV_STD enStd;
} stPQ_SetPhotoYUVStandard,*pstPQ_SetPhotoYUVStandard;
typedef struct
{
    PQ_WIN eWindow;
    char *bReturnValue;
} stPQ_Ex_GetVersion,*pstPQ_Ex_GetVersion;
typedef struct
{
    PQ_WIN eWindow;
    MS_U32 bReturnValue;
} stPQ_GetAdaptiveVersion,*pstPQ_GetAdaptiveVersion;
typedef struct
{
    PQ_WIN eWindow;
    MS_U32 bReturnValue;
} stPQ_LoadUFSCSettings,*pstPQ_LoadUFSCSettings;
#if (ENABLE_PQ_BIN_CF)
typedef struct
{
    PQ_WIN eWindow;
    MS_U32 bReturnValue;
} stPQ_LoadCFSettings,*pstPQ_LoadCFSettings;
#endif
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16IPIdx;
    PQ_ENGINE_TYPE ePQEngineType;
    MS_U16 u16Ret;
} stPQ_GetTableIndex_Ex,*pstPQ_GetTableIndex_Ex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U8 u8Mode;
    MS_BOOL bRet;
} stPQ_Demo_CloneWindow,*pstPQ_Demo_CloneWindow;
typedef struct
{
    PQ_WIN eWindow;
    MS_U8 bEnable;
} stPQ_EnableScalerGamma,*pstPQ_EnableScalerGamma;
typedef struct
{
    APIPNL_GAMMA_TYPE enGammType;
    MS_U8 **pu8GammTable;
    APIPNL_GAMMA_MAPPEING_MODE enGammaMappingMode;
} stPQ_SetGammaTbl,*pstPQ_SetGammaTbl;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    PQ_ENGINE_TYPE ePQEngineType;
    MS_U16 u16Ret;
} stPQ_GetGRule_LvlNum_Ex,*pstPQ_GetGRule_LvlNum_Ex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16GRuleLevelIndex;
    PQ_ENGINE_TYPE ePQEngineType;
    MS_U16 u16Ret;
} stPQ_GetGRule_LevelIndex_Ex,*pstPQ_GetGRule_LevelIndex_Ex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16GRuleIPIndex;
    PQ_ENGINE_TYPE ePQEngineType;
    MS_U16 u16Ret;
} stPQ_GetGRule_IPIndex_Ex,*pstPQ_GetGRule_IPIndex_Ex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16GRuleType;
    MS_U16 u16PQ_NRIdx;
    MS_U16 u16GRuleIPIndex;
    PQ_ENGINE_TYPE ePQEngineType;
    MS_U16 u16Ret;
} stPQ_GetGRule_TableIndex_Ex,*pstPQ_GetGRule_TableIndex_Ex;
typedef struct
{
    PQ_WIN eWindow;
    MS_U16 u16TabIdx;
    MS_U16 u16PQIPIdx;
    PQ_ENGINE_TYPE ePQEngineType;
} stPQ_LoadTable_Ex,*pstPQ_LoadTable_Ex;
typedef struct DLL_PACKED
{
    PQ_BIN_PATH ePqBinPath;
    MS_U8 u8size;
    char* b_PQBinFilePath;
} stPQ_SetPQBinPath,*pstPQ_SetPQBinPath;
//drvPQ_cus.c
typedef struct
{
    PQ_WIN eWindow;
    PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType;
} stPQ_LoadFilmModeTable,*pstPQ_LoadFilmModeTable;
typedef struct
{
    PQ_WIN eWindow;
    PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType;
} stPQ_LoadDynamicContrastTable,*pstPQ_LoadDynamicContrastTable;
typedef struct
{
    PQ_WIN eWindow;
    PQ_3D_NR_FUNCTION_TYPE en3DNRType;
} stPQ_LoadNRTable,*pstPQ_LoadNRTable;
typedef struct
{
    PQ_WIN eWindow;
    PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType;
} stPQ_LoadMPEGNRTable,*pstPQ_LoadMPEGNRTable;
typedef struct
{
    PQ_WIN eWindow;
    PQ_FEATURE_SWITCH_TYPE enCtrlType;
} stPQ_Load_ULTRACLEAR_Table,*pstPQ_Load_ULTRACLEAR_Table;
typedef struct
{
    PQ_WIN eWindow;
    PQ_XVYCC_TYPE enXvyccType;
} stPQ_Load_XVYCC_Table,*pstPQ_Load_XVYCC_Table;
typedef struct
{
    PQ_WIN eWindow;
    PQ_HDR_MODE enHdrMode;
} stPQ_Load_HDR_Table,*pstPQ_Load_HDR_Table;
typedef struct
{
    PQ_SUPPORTED_TYPE enType;
    MS_BOOL bReturnValue;
} stPQ_GRULE_Get_Support_Status,*pstPQ_GRULE_Get_Support_Status;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_BOOL bMemFmt422;
    MS_BOOL bFBL;
    MS_U8 *pu8BitsPerPixel;
    PQ_DEINTERLACE_MODE bReturnValue;
} stPQ_SetMemFormat,*pstPQ_SetMemFormat;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bOSD_On;
} stPQ_ReduceBW_ForOSD,*pstPQ_ReduceBW_ForOSD;
typedef struct
{
    PQ_WIN eWindow;
    PQ_PTP_FUNCTION_TYPE enPTP_Type;
} stPQ_LoadPTPTable,*pstPQ_LoadPTPTable;
typedef struct
{
    MS_U16 u16DisplayWidth;
    PQ_DISPLAY_TYPE enDisplaType;
} stPQ_Init_DisplayType,*pstPQ_Init_DisplayType;
typedef struct
{
    MS_U16 u16DisplayWidth;
    PQ_DISPLAY_TYPE enDisplaType;
} stPQ_Set_DisplayType,*pstPQ_Set_DisplayType;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U16 *pu16TabIdx_MADi;
    MS_U16 *pu16TabIdx_MADi_Motion;
    MS_BOOL bReturnValue;
} stPQ_GetMADiFromOSDBWGrule,*pstPQ_GetMADiFromOSDBWGrule;
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    MS_U16 *pu16TabIdx_MADi;
    MS_U16 *pu16TabIdx_MADi_Motion;
    MS_BOOL bReturnValue;
} stPQ_GetMADiInGeneral,*pstPQ_GetMADiInGeneral;
typedef struct DLL_PACKED
{
    XC_SETWIN_INFO *pstXC_SetWin_Info;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_Check_PointToPoint_Condition,*pstPQ_Check_PointToPoint_Condition;
typedef struct
{
    PQ_WIN eWindow;
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetPostCCSOnOff,*pstPQ_SetPostCCSOnOff;
typedef struct
{
    MS_BOOL bEnable;
} stPQ_FilmMode_AnyCandence_Enable,*pstPQ_FilmMode_AnyCandence_Enable;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetH264_OnOff,*pstPQ_SetH264_OnOff;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetG3D_OnOff,*pstPQ_SetG3D_OnOff;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetMVC4kx1k_OnOff,*pstPQ_SetMVC4kx1k_OnOff;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetNetworkMM_OnOff,*pstPQ_SetNetworkMM_OnOff;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_SetMM_OnOff,*pstPQ_SetMM_OnOff;
typedef struct
{
    MS_BOOL bEn;
    PQ_WIN ePQWindow;
    MS_BOOL bReturnValue;
} stPQ_SetRmvb_OnOff,*pstPQ_SetRmvb_OnOff;
typedef struct
{
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_SetVIPBypass_OnOff,*pstPQ_SetVIPBypass_OnOff;
typedef struct
{
    MS_BOOL bEnable;
    PQ_WIN eWindow;
    MS_BOOL bReturnValue;
} stPQ_SetPeaking_OnOff,*pstPQ_SetPeaking_OnOff;
typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bReturnValue;
} stPQ_SetFakeOutEnable,*pstPQ_SetFakeOutEnable;
typedef struct
{
    MS_BOOL bEn;
    MS_BOOL bReturnValue;
} stPQ_Set1KTravelingEnable,*pstPQ_Set1KTravelingEnable;
//drvPQ_Text.c
typedef struct DLL_PACKED
{
    PQ_WIN eWindow;
    char *u8Ret;
} stPQ_GetSrcTypeName,*pstPQ_GetSrcTypeName;
typedef struct DLL_PACKED
{
    MS_U8 u8PQIPIdx;
    char *u8Ret;
} stPQ_GetIPName,*pstPQ_GetIPName;
typedef struct DLL_PACKED
{
    MS_U8 u8PQIPIdx;
    MS_U8 u8TabIdx;
    char *u8Ret;
} stPQ_GetTableName,*pstPQ_GetTableName;
typedef struct DLL_PACKED
{
    MS_U8 u8TabIdx;
} stPQ_BW_CusMode_LoadTable,*pstPQ_BW_CusMode_LoadTable;
//drvbw.c
typedef struct
{
    PQ_WIN eWindow;
} stPQ_BW_LoadTableByContext,*pstPQ_BW_LoadTableByContext;

typedef struct
{
    MS_BOOL bFrameBuffLess;
} stPQ_SetFrameBuffMode,*pstPQ_SetFrameBuffMode;

//#ifdef _XC_C_
//#define INTERFACE
//#else
//#define INTERFACE extern
//#endif
//#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif //_XC_H_
