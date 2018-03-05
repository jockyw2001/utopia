#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/compat.h>

#include "PQ_adp.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"

#define CMP_CPY_FROM_USER(a,b,c)                                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }

#ifdef CONFIG_COMPAT
typedef struct DLL_PACKED
{
    PQ_BIN_PATH ePqBinPath;
    MS_U8 u8size;
    compat_uptr_t b_PQBinFilePath;
} _compat_stPQ_SetPQBinPath;
#endif

//Base MS_TYPE DEF
UADP_SPT_NAMEnNXT_DEF(PQ_MS_U8,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MS_U16,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MS_U32,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MS_BOOL,0);
//****************
//drvPQ.c
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_Init_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_INIT,1);
UADP_SPT_NAMEnNXT_DEF(PQ_GetConfig,1);
UADP_SPT_NAMEnNXT_DEF(PQ_SetPowerState,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetIPNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetTableNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetCurrentTableIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetTableIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetCustomerTableIndex,0);
//UADP_SPT_NAMEnNXT_DEF(PQ_GetXRuleIPIndex,0);
//UADP_SPT_NAMEnNXT_DEF(PQ_GetXRuleTableIndex,0);
//UADP_SPT_NAMEnNXT_DEF(PQ_GetXRuleIPNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadTableData,1);
UADP_SPT_NAMEnNXT_DEF(PQ_CloneTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadCustomerTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_CheckSettings,0);
#if (ENABLE_PQ_MLOAD)
UADP_SPT_NAMEnNXT_DEF(PQ_Set_MLoadEn,0);
#endif
UADP_SPT_NAMEnNXT_DEF(PQ_SkipDuplicatedSetting,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetSkipDuplicatedSettingStatus,0);
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
UADP_SPT_NAMEnNXT_DEF(PQ_SetFrcmCbCrSwap,0);
#endif
UADP_SPT_NAMEnNXT_DEF(PQ_LoadSettings,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadCustomerSettings,0);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_Dtv_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_DTVInfo,1);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_MuliMedia_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_MultiMediaInfo,1);
UADP_SPT_NAMEnNXT_DEF(ADP_MS_PQ_Vd_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_VDInfo,1);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_Mode_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_ModeInfo,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetHDMIInfo,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetHDMI_PC,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetHDMI_PC_Status,0);
UADP_SPT_NAMEnNXT_DEF(PQ_DesideSrcType,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetInputSourceType,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetSrcType,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetDelayTimeByMemc,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetDelayTime,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadScalingTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetColorRange,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_CSC_XRuleIdx,2);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_MemYUVFmt,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetCSC,0);
#if (PQ_XRULE_DB_ENABLE == 1)
UADP_SPT_NAMEnNXT_DEF(PQ_Set_DBRule,0);
#endif
UADP_SPT_NAMEnNXT_DEF(PQ_EnableMADIForce,0);
UADP_SPT_NAMEnNXT_DEF(PQ_ReduceBW_ForPVR,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set420upsampling,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetFilmMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetNonLinearScaling,0);
UADP_SPT_NAMEnNXT_DEF(PQ_3DCloneforPIP,0);
UADP_SPT_NAMEnNXT_DEF(PQ_DisableFilmMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_DisableUCFeature,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetCurrentUCFeature,11);
UADP_SPT_NAMEnNXT_DEF(PQ_GetUCFeatureFromPQ,9);
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
UADP_SPT_NAMEnNXT_DEF(PQ_EnableUCFeature,0);
#endif
UADP_SPT_NAMEnNXT_DEF(PQ_SetUCFeature,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetDMSV12L,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetDMSV12LFromXRuleTable,0);
UADP_SPT_NAMEnNXT_DEF(ADP_MSIF_Version,0);
UADP_SPT_NAMEnNXT_DEF(ADP_P_MSIF_Version,1);
UADP_SPT_NAMEnNXT_DEF(PQ_GetLibVer,1);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_INFO,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetInfo,1);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_Status,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetStatus,1);
UADP_SPT_NAMEnNXT_DEF(PQ_SetDbgLevel,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MADiForceMotionC,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MADiForceMotionY,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetRFblMode,0);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_RFBL_INFO,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetRFblMode,1);
#if (PQ_ENABLE_VD_SAMPLING)
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_VD_Sampling_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_VDSampling_Info,1);
#endif
//PQ_IOCTL
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_HSD_SAMPLING_INFO,0);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_ADC_SAMPLING_INFO,0);

UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_HSD_SAMPLING,1);
UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_PREVSD_BILINEAR,1);
UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_ADC_SAMPLING,1);
UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_RFBL_CTRL,1);
UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_SUGGESTED_FRAMENUM,1);
UADP_SPT_NAMEnNXT_DEF(PQ_INTER_IOCTL_SET_UCFEATURE,1);
//endof PQ_IOCTL
UADP_SPT_NAMEnNXT_DEF(PQ_Get_ADCSampling_Info,1);
UADP_SPT_NAMEnNXT_DEF(PQ_Set3D_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_CAP_INFO,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetCaps,1);
UADP_SPT_NAMEnNXT_DEF(PQ_PrintLoadTableInfo,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetDS_OnOFF,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_RFBL_Info,0);
UADP_SPT_NAMEnNXT_DEF(PQ_CheckHDMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_Interlace_SD_mode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetDualViewState,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetVGASubCaseForceTo422Mode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetDualViewState,0);
UADP_SPT_NAMEnNXT_DEF(PQ_ForceVideoInputMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_IsForceVideoInputMode,0);
UADP_SPT_NAMEnNXT_DEF(ADP_XC_SETWIN_INFO,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_PointToPoint,1);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_PointToPoint,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Update_MemFormat,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetHsize,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetVsize,0);
UADP_SPT_NAMEnNXT_DEF(PQ_IsInterlace,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LOW_3dQuality,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MWE_SetEnhanceQuality,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MWE_RestoreEnhanceQuality,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MWE_RestoreOffQuality,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MWE_SetOffQuality,0);
UADP_SPT_NAMEnNXT_DEF(PQ_MWE_CloneWindow,0);
UADP_SPT_NAMEnNXT_DEF(PQ_3D_SettingForLBL,0);
UADP_SPT_NAMEnNXT_DEF(PQ_3D_CloneWindow,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_SensioMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_SensioMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetSelectCSC,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetR2YEqSelect,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetMemFmtInGeneral,3);
UADP_SPT_NAMEnNXT_DEF(PQ_GetMADiForRFBL,2);
UADP_SPT_NAMEnNXT_DEF(PQ_Patch2Rto4RForFieldPackingMode,0);
UADP_SPT_NAMEnNXT_DEF(ADP_PQ_DEINTERLACE_MODE,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetBPPInfoFromMADi,3);
UADP_SPT_NAMEnNXT_DEF(PQ_ChangeMemConfigFor3D,6);
UADP_SPT_NAMEnNXT_DEF(PQ_ForceBPPForDynamicMemFmt,1);
UADP_SPT_NAMEnNXT_DEF(PQ_SuggestFrameNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetGameMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGameMode_Status,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetBypassMode,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetBypassModeStatus,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Demo_CloneWindow,0);
UADP_SPT_NAMEnNXT_DEF(PQ_EnableScalerGamma,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetGammaTbl,1);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_LvlNum_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_LevelIndex_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_IPIndex_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_TableIndex_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadTable_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetTableIndex_Ex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_PqBypass,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_PqBypass,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadUFSCSettings,0);
#if (ENABLE_PQ_BIN_CF)
UADP_SPT_NAMEnNXT_DEF(PQ_LoadCFSettings,0);
#endif
//E_PQ_CMD_GET_VERSION
UADP_SPT_NAMEnNXT_DEF(PQ_SetGRuleStatus,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_LvlNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_GRuleNum,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_LevelIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_IPIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_TableIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetCustomerGRule_IPIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetGRule_CustomerTableIndex,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_xvYCC_MatrixCoefficient,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_xvYCC_MatrixEnable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Get_DNR_Whole_Reg,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Up_Layer_Set_Config,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetQmapExecutor,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetPhotoYUVStandard,0);
//drvPQ_cus.c
UADP_SPT_NAMEnNXT_DEF(PQ_LoadFilmModeTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadDynamicContrastTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadNRTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadMPEGNRTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Load_ULTRACLEAR_Table,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Load_XVYCC_Table,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Load_HDR_Table,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GRULE_Get_Support_Status,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetMemFormat,1);
UADP_SPT_NAMEnNXT_DEF(PQ_ReduceBW_ForOSD,0);
UADP_SPT_NAMEnNXT_DEF(PQ_LoadPTPTable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Init_DisplayType,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set_DisplayType,0);
UADP_SPT_NAMEnNXT_DEF(PQ_GetMADiFromOSDBWGrule,2);
UADP_SPT_NAMEnNXT_DEF(PQ_GetMADiInGeneral,2);
UADP_SPT_NAMEnNXT_DEF(PQ_Check_PointToPoint_Condition,1);
UADP_SPT_NAMEnNXT_DEF(PQ_SetPostCCSOnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_FilmMode_AnyCandence_Enable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetH264_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetG3D_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetMVC4kx1k_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetNetworkMM_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetMM_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetRmvb_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetVIPBypass_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetPeaking_OnOff,0);
UADP_SPT_NAMEnNXT_DEF(PQ_SetFakeOutEnable,0);
UADP_SPT_NAMEnNXT_DEF(PQ_Set1KTravelingEnable,0);
//drvPQ_Text.c
//E_PQ_CMD_GET_SRC_TYPE_NAME:
//E_PQ_CMD_GET_IP_NAME:
//E_PQ_CMD_GET_TABLE_NAME:
//drvbw.c
UADP_SPT_NAMEnNXT_DEF(PQ_BW_LoadTableByContext,0);
UADP_SPT_NAMEnNXT_DEF(PQ_BW_CusMode_LoadTable,0);


#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
UADP_SPT_NAMEnNXT_DEF(PQ_MADiForceMotion,0);
#endif

#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
UADP_SPT_NAMEnNXT_DEF(PQ_SetColorThroughMode,0);
#endif





MS_U32 PQ_adp_Init(FUtopiaIOctl* pIoctl)
{
//Base MS_TYPE DEF
    UADP_SPT_NAME0NXT(PQ_MS_U8,MS_U8);
    UADP_SPT_NAME0NXT(PQ_MS_U16,MS_U16);
    UADP_SPT_NAME0NXT(PQ_MS_U32,MS_U32);
    UADP_SPT_NAME0NXT(PQ_MS_BOOL,MS_BOOL);
//****************
//drvPQ.c
    UADP_SPT_NAME0NXT(ADP_PQ_Init_Info,MS_PQ_Init_Info);
    UADP_SPT_NAME1NXT(PQ_INIT,stPQ_INIT,pstPQInitInfo,ADP_PQ_Init_Info);
    UADP_SPT_NAME1NXT(PQ_GetConfig,stPQ_GetConfig,pstPQInitInfo,ADP_PQ_Init_Info);
    UADP_SPT_NAME0NXT(PQ_SetPowerState,stPQ_SetPowerState);
    UADP_SPT_NAME0NXT(PQ_GetIPNum,stPQ_GetIPNum);
    UADP_SPT_NAME0NXT(PQ_GetTableNum,stPQ_GetTableNum);
    UADP_SPT_NAME0NXT(PQ_GetCurrentTableIndex,stPQ_GetCurrentTableIndex);
    UADP_SPT_NAME0NXT(PQ_GetTableIndex,stPQ_GetTableIndex);
    UADP_SPT_NAME0NXT(PQ_GetCustomerTableIndex,stPQ_GetCustomerTableIndex);
    //UADP_SPT_NAME0NXT(PQ_GetXRuleIPIndex,stPQ_GetXRuleIPIndex);
    //UADP_SPT_NAME0NXT(PQ_GetXRuleTableIndex,stPQ_GetXRuleTableIndex);
    //UADP_SPT_NAME0NXT(PQ_GetXRuleIPNum,stPQ_GetXRuleIPNum);
    UADP_SPT_NAME1NXT(PQ_LoadTableData,stPQ_LoadTableData,pTable,PQ_MS_U8*10);
    UADP_SPT_NAME0NXT(PQ_CloneTable,stPQ_CloneTable);
    UADP_SPT_NAME0NXT(PQ_LoadTable,stPQ_LoadTable);
    UADP_SPT_NAME0NXT(PQ_LoadCustomerTable,stPQ_LoadCustomerTable);
    UADP_SPT_NAME0NXT(PQ_CheckSettings,stPQ_CheckSettings);
#if (ENABLE_PQ_MLOAD)
    UADP_SPT_NAME0NXT(PQ_Set_MLoadEn,stPQ_Set_MLoadEn);
#endif
    UADP_SPT_NAME0NXT(PQ_SkipDuplicatedSetting,stPQ_SkipDuplicatedSetting);
    UADP_SPT_NAME0NXT(PQ_GetSkipDuplicatedSettingStatus,stPQ_GetSkipDuplicatedSettingStatus);
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
    UADP_SPT_NAME0NXT(PQ_SetFrcmCbCrSwap,stPQ_SetFrcmCbCrSwap);
#endif
    UADP_SPT_NAME0NXT(PQ_LoadSettings,stPQ_LoadSettings);
    UADP_SPT_NAME0NXT(PQ_LoadCustomerSettings,stPQ_LoadCustomerSettings);
    UADP_SPT_NAME0NXT(ADP_PQ_Dtv_Info,MS_PQ_Dtv_Info);
    UADP_SPT_NAME1NXT(PQ_Set_DTVInfo,stPQ_Set_DTVInfo,pstPQDTVInfo,ADP_PQ_Dtv_Info);
    UADP_SPT_NAME0NXT(ADP_PQ_MuliMedia_Info,MS_PQ_MuliMedia_Info);
    UADP_SPT_NAME1NXT(PQ_Set_MultiMediaInfo,stPQ_Set_MultiMediaInfo,pstPQMMInfo,ADP_PQ_MuliMedia_Info);
    UADP_SPT_NAME0NXT(ADP_MS_PQ_Vd_Info,MS_PQ_Vd_Info);
    UADP_SPT_NAME1NXT(PQ_Set_VDInfo,stPQ_Set_VDInfo,pstPQVDInfo,ADP_MS_PQ_Vd_Info);
    UADP_SPT_NAME0NXT(ADP_PQ_Mode_Info,MS_PQ_Mode_Info);
    UADP_SPT_NAME1NXT(PQ_Set_ModeInfo,stPQ_Set_ModeInfo,pstPQModeInfo,ADP_PQ_Mode_Info);
    UADP_SPT_NAME0NXT(PQ_SetHDMIInfo,stPQ_SetHDMIInfo);
    UADP_SPT_NAME0NXT(PQ_SetHDMI_PC,stPQ_SetHDMI_PC);
    UADP_SPT_NAME0NXT(PQ_GetHDMI_PC_Status,stPQ_GetHDMI_PC_Status);
    UADP_SPT_NAME0NXT(PQ_DesideSrcType,stPQ_DesideSrcType);
    UADP_SPT_NAME0NXT(PQ_GetInputSourceType,stPQ_GetInputSourceType);
    UADP_SPT_NAME0NXT(PQ_GetSrcType,stPQ_GetSrcType);
    UADP_SPT_NAME0NXT(PQ_GetDelayTimeByMemc,stPQ_GetDelayTimeByMemc);
    UADP_SPT_NAME0NXT(PQ_GetDelayTime,stPQ_GetDelayTime);
    UADP_SPT_NAME0NXT(PQ_LoadScalingTable,stPQ_LoadScalingTable);
    UADP_SPT_NAME0NXT(PQ_SetColorRange,stPQ_SetColorRange);
    UADP_SPT_NAME2NXT(PQ_Get_CSC_XRuleIdx,stPQ_Get_CSC_XRuleIdx,pbInputColorSpaceRGB,PQ_MS_BOOL,pu16DoCSC,PQ_MS_U16);
    UADP_SPT_NAME0NXT(PQ_Get_MemYUVFmt,stPQ_Get_MemYUVFmt);
    UADP_SPT_NAME0NXT(PQ_SetCSC,stPQ_SetCSC);
#if (PQ_XRULE_DB_ENABLE == 1)
    UADP_SPT_NAME0NXT(PQ_Set_DBRule,stPQ_Set_DBRule);
#endif
    UADP_SPT_NAME0NXT(PQ_EnableMADIForce,stPQ_EnableMADIForce);
    UADP_SPT_NAME0NXT(PQ_ReduceBW_ForPVR,stPQ_ReduceBW_ForPVR);
    UADP_SPT_NAME0NXT(PQ_Set420upsampling,stPQ_Set420upsampling);
    UADP_SPT_NAME0NXT(PQ_SetFilmMode,stPQ_SetFilmMode);
    UADP_SPT_NAME0NXT(PQ_SetNonLinearScaling,stPQ_SetNonLinearScaling);
    UADP_SPT_NAME0NXT(PQ_3DCloneforPIP,stPQ_3DCloneforPIP);
    UADP_SPT_NAME0NXT(PQ_DisableFilmMode,stPQ_DisableFilmMode);
    UADP_SPT_NAME0NXT(PQ_DisableUCFeature,stPQ_DisableUCFeature);
    UADP_SPT_NAME11NXT(PQ_GetCurrentUCFeature,stPQ_GetCurrentUCFeature,
                        pu16TabIdx_MADi,PQ_MS_U16,
                        pu16TabIdx_MADi_Mot,PQ_MS_U16,
                        pu16TabIdx_MADi_DFK,PQ_MS_U16,
                        pu16TabIdx_MADi_SST,PQ_MS_U16,
                        pu16TabIdx_MADi_EODiW,PQ_MS_U16,
                        pu16TabIdx_UCNR,PQ_MS_U16,
                        pu16TabIdx_UCDi,PQ_MS_U16,
                        pu16TabIdx_UCCTL,PQ_MS_U16,
                        pu16TabIdx_SwDriver,PQ_MS_U16,
                        pu16TabIdx_PostCCS,PQ_MS_U16,
                        pu16TabIdx_DHD,PQ_MS_U16);
    UADP_SPT_NAME9NXT(PQ_GetUCFeatureFromPQ,stPQ_GetUCFeatureFromPQ,
                        pu16TabIdx_MADi_DFK,PQ_MS_U16,
                        pu16TabIdx_MADi_SST,PQ_MS_U16,
                        pu16TabIdx_MADi_EODiW,PQ_MS_U16,
                        pu16TabIdx_UCNR,PQ_MS_U16,
                        pu16TabIdx_UCDi,PQ_MS_U16,
                        pu16TabIdx_UCCTL,PQ_MS_U16,
                        pu16TabIdx_SwDriver,PQ_MS_U16,
                        pu16TabIdx_PostCCS,PQ_MS_U16,
                        pu16TabIdx_DHD,PQ_MS_U16);
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    UADP_SPT_NAME0NXT(PQ_EnableUCFeature,stPQ_EnableUCFeature);
#endif
    UADP_SPT_NAME0NXT(PQ_SetUCFeature,stPQ_SetUCFeature);
    UADP_SPT_NAME0NXT(PQ_SetDMSV12L,stPQ_SetDMSV12L);
    UADP_SPT_NAME0NXT(PQ_GetDMSV12LFromXRuleTable,stPQ_GetDMSV12LFromXRuleTable);
    UADP_SPT_NAME0NXT(ADP_MSIF_Version,MSIF_Version);
    UADP_SPT_NAME1NXT(ADP_P_MSIF_Version,P_MSIF_Version,__msif_version,ADP_MSIF_Version);
    UADP_SPT_NAME1NXT(PQ_GetLibVer,stPQ_GetLibVer,ppVersion,ADP_P_MSIF_Version);    //to do: need to check it works or not
    UADP_SPT_NAME0NXT(ADP_PQ_INFO,MS_PQ_INFO);
    UADP_SPT_NAME1NXT(PQ_GetInfo,stPQ_GetInfo,pInfo,ADP_PQ_INFO);
    UADP_SPT_NAME0NXT(ADP_PQ_Status,MS_PQ_Status);
    UADP_SPT_NAME1NXT(PQ_GetStatus,stPQ_GetStatus,pStatus,ADP_PQ_Status);
    UADP_SPT_NAME0NXT(PQ_SetDbgLevel,stPQ_SetDbgLevel);
    UADP_SPT_NAME0NXT(PQ_MADiForceMotionC,stPQ_MADiForceMotionC);
    UADP_SPT_NAME0NXT(PQ_MADiForceMotionY,stPQ_MADiForceMotionY);
    UADP_SPT_NAME0NXT(PQ_SetRFblMode,stPQ_SetRFblMode);
    UADP_SPT_NAME0NXT(ADP_PQ_RFBL_INFO,MS_PQ_RFBL_INFO);
    UADP_SPT_NAME1NXT(PQ_GetRFblMode,stPQ_GetRFblMode,pstInfo,ADP_PQ_RFBL_INFO);
#if (PQ_ENABLE_VD_SAMPLING)
    UADP_SPT_NAME0NXT(ADP_PQ_VD_Sampling_Info,MS_PQ_VD_Sampling_Info);
    UADP_SPT_NAME1NXT(PQ_Get_VDSampling_Info,stPQ_Get_VDSampling_Info,pInfo,ADP_PQ_VD_Sampling_Info);
#endif
//PQ_IOCTL
    UADP_SPT_NAME0NXT(ADP_PQ_HSD_SAMPLING_INFO,PQ_HSD_SAMPLING_INFO);
    UADP_SPT_NAME0NXT(ADP_PQ_ADC_SAMPLING_INFO,PQ_ADC_SAMPLING_INFO);

    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_HSD_SAMPLING,stPQ_INTER_IOCTL,pBuf,ADP_PQ_HSD_SAMPLING_INFO);
    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_PREVSD_BILINEAR,stPQ_INTER_IOCTL,pBuf,PQ_MS_BOOL);
    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_ADC_SAMPLING,stPQ_INTER_IOCTL,pBuf,ADP_PQ_ADC_SAMPLING_INFO);
    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_RFBL_CTRL,stPQ_INTER_IOCTL,pBuf,PQ_MS_U32);
    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_SUGGESTED_FRAMENUM,stPQ_INTER_IOCTL,pBuf,PQ_MS_U16);
    UADP_SPT_NAME1NXT(PQ_INTER_IOCTL_SET_UCFEATURE,stPQ_INTER_IOCTL,pBuf,PQ_MS_BOOL);
//endof PQ_IOCTL
    UADP_SPT_NAME1NXT(PQ_Get_ADCSampling_Info,stPQ_Get_ADCSampling_Info,pstPQModeInfo,ADP_PQ_Mode_Info);
    UADP_SPT_NAME0NXT(PQ_Set3D_OnOff,stPQ_Set3D_OnOff);
    UADP_SPT_NAME0NXT(ADP_PQ_CAP_INFO,MS_PQ_CAP_INFO);
    UADP_SPT_NAME1NXT(PQ_GetCaps,stPQ_GetCaps,pInfo,ADP_PQ_CAP_INFO);
    UADP_SPT_NAME0NXT(PQ_PrintLoadTableInfo,stPQ_PrintLoadTableInfo);
    UADP_SPT_NAME0NXT(PQ_SetDS_OnOFF,stPQ_SetDS_OnOFF);
    UADP_SPT_NAME0NXT(PQ_Get_RFBL_Info,stPQ_Get_RFBL_Info);
    UADP_SPT_NAME0NXT(PQ_CheckHDMode,stPQ_CheckHDMode);
    UADP_SPT_NAME0NXT(PQ_Get_Interlace_SD_mode,stPQ_Get_Interlace_SD_mode);
    UADP_SPT_NAME0NXT(PQ_SetDualViewState,stPQ_SetDualViewState);
    UADP_SPT_NAME0NXT(PQ_GetVGASubCaseForceTo422Mode,stPQ_GetVGASubCaseForceTo422Mode);
    UADP_SPT_NAME0NXT(PQ_GetDualViewState,stPQ_GetDualViewState);
    UADP_SPT_NAME0NXT(PQ_ForceVideoInputMode,stPQ_ForceVideoInputMode);
    UADP_SPT_NAME0NXT(PQ_IsForceVideoInputMode,stPQ_IsForceVideoInputMode);
    UADP_SPT_NAME0NXT(ADP_XC_SETWIN_INFO,XC_SETWIN_INFO);
    UADP_SPT_NAME1NXT(PQ_Set_PointToPoint,stPQ_Set_PointToPoint,pstXC_SetWin_Info,ADP_XC_SETWIN_INFO);
    UADP_SPT_NAME0NXT(PQ_Get_PointToPoint,stPQ_Get_PointToPoint);
    UADP_SPT_NAME0NXT(PQ_Update_MemFormat,stPQ_Update_MemFormat);
    UADP_SPT_NAME0NXT(PQ_GetHsize,stPQ_GetHsize);
    UADP_SPT_NAME0NXT(PQ_GetVsize,stPQ_GetVsize);
    UADP_SPT_NAME0NXT(PQ_IsInterlace,stPQ_IsInterlace);
    UADP_SPT_NAME0NXT(PQ_LOW_3dQuality,stPQ_LOW_3dQuality);
    UADP_SPT_NAME0NXT(PQ_MWE_SetEnhanceQuality,stPQ_MWE_SetEnhanceQuality);
    UADP_SPT_NAME0NXT(PQ_MWE_RestoreEnhanceQuality,stPQ_MWE_RestoreEnhanceQuality);
    UADP_SPT_NAME0NXT(PQ_MWE_RestoreOffQuality,stPQ_MWE_RestoreOffQuality);
    UADP_SPT_NAME0NXT(PQ_MWE_SetOffQuality,stPQ_MWE_SetOffQuality);
    UADP_SPT_NAME0NXT(PQ_MWE_CloneWindow,stPQ_MWE_CloneWindow);
    UADP_SPT_NAME0NXT(PQ_3D_SettingForLBL,stPQ_3D_SettingForLBL);
    UADP_SPT_NAME0NXT(PQ_3D_CloneWindow,stPQ_3D_CloneWindow);
    UADP_SPT_NAME0NXT(PQ_Get_SensioMode,stPQ_Get_SensioMode);
    UADP_SPT_NAME0NXT(PQ_Set_SensioMode,stPQ_Set_SensioMode);
    UADP_SPT_NAME0NXT(PQ_SetSelectCSC,stPQ_SetSelectCSC);
    UADP_SPT_NAME0NXT(PQ_GetR2YEqSelect,stPQ_GetR2YEqSelect);
    UADP_SPT_NAME3NXT(PQ_GetMemFmtInGeneral,stPQ_GetMemFmtInGeneral,pu16TabIdx_MemFormat,PQ_MS_U16
                                                                    ,pu16TabIdx_444To422,PQ_MS_U16
                                                                    ,pu16TabIdx_422To444,PQ_MS_U16);
    UADP_SPT_NAME2NXT(PQ_GetMADiForRFBL,stPQ_GetMADiForRFBL,pu16TabIdx_MADi_Motion,PQ_MS_U16
                                                            ,pu16TabIdx_MADi,PQ_MS_U16);
    UADP_SPT_NAME0NXT(PQ_Patch2Rto4RForFieldPackingMode,stPQ_Patch2Rto4RForFieldPackingMode);
    UADP_SPT_NAME0NXT(ADP_PQ_DEINTERLACE_MODE,PQ_DEINTERLACE_MODE);
    UADP_SPT_NAME3NXT(PQ_GetBPPInfoFromMADi,stPQ_GetBPPInfoFromMADi,pu8BitsPerPixel,PQ_MS_U8
                                                                    ,peDeInterlaceMode,ADP_PQ_DEINTERLACE_MODE
                                                                    ,pu8FrameCount,PQ_MS_U8);
    UADP_SPT_NAME6NXT(PQ_ChangeMemConfigFor3D,stPQ_ChangeMemConfigFor3D,
                                            pu16TabIdx_MemFormat,PQ_MS_U16,
                                            pu16TabIdx_444To422,PQ_MS_U16,
                                            pu16TabIdx_422To444,PQ_MS_U16,
                                            pu8BitsPerPixel,PQ_MS_U8,
                                            pu16TabIdx_MADi_Motion,PQ_MS_U16,
                                            pu16TabIdx_MADi,PQ_MS_U16);
    UADP_SPT_NAME1NXT(PQ_ForceBPPForDynamicMemFmt,stPQ_ForceBPPForDynamicMemFmt,pu8BitsPerPixel,PQ_MS_U8);
    UADP_SPT_NAME0NXT(PQ_SuggestFrameNum,stPQ_SuggestFrameNum);
    UADP_SPT_NAME0NXT(PQ_SetGameMode,stPQ_SetGameMode);
    UADP_SPT_NAME0NXT(PQ_GetGameMode_Status,stPQ_GetGameMode_Status);
    UADP_SPT_NAME0NXT(PQ_SetBypassMode,stPQ_SetBypassMode);
    UADP_SPT_NAME0NXT(PQ_GetBypassModeStatus,stPQ_GetBypassModeStatus);
    UADP_SPT_NAME0NXT(PQ_GetTableIndex_Ex,stPQ_GetTableIndex_Ex);
    UADP_SPT_NAME0NXT(PQ_Demo_CloneWindow,stPQ_Demo_CloneWindow);
    UADP_SPT_NAME0NXT(PQ_EnableScalerGamma,stPQ_EnableScalerGamma);
    UADP_SPT_NAME1NXT(PQ_SetGammaTbl,stPQ_SetGammaTbl,pu8GammTable,ADP_P_MSIF_Version);
    UADP_SPT_NAME0NXT(PQ_GetGRule_LvlNum_Ex,stPQ_GetGRule_LvlNum_Ex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_LevelIndex_Ex,stPQ_GetGRule_LevelIndex_Ex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_IPIndex_Ex,stPQ_GetGRule_IPIndex_Ex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_TableIndex_Ex,stPQ_GetGRule_TableIndex_Ex);
    UADP_SPT_NAME0NXT(PQ_LoadTable_Ex,stPQ_LoadTable_Ex);
    UADP_SPT_NAME0NXT(PQ_Get_PqBypass,pstPQ_Get_PqBypass);
    UADP_SPT_NAME0NXT(PQ_Set_PqBypass,pstPQ_Set_PqBypass);
    UADP_SPT_NAME0NXT(PQ_LoadUFSCSettings,stPQ_LoadUFSCSettings);
#if (ENABLE_PQ_BIN_CF)
    UADP_SPT_NAME0NXT(PQ_LoadCFSettings,stPQ_LoadCFSettings);
#endif
//E_PQ_CMD_GET_VERSION
    UADP_SPT_NAME0NXT(PQ_SetGRuleStatus,stPQ_SetGRuleStatus);
    UADP_SPT_NAME0NXT(PQ_GetGRule_LvlNum,stPQ_GetGRule_LvlNum);
    UADP_SPT_NAME0NXT(PQ_GetGRule_GRuleNum,stPQ_GetGRule_GRuleNum);
    UADP_SPT_NAME0NXT(PQ_GetGRule_LevelIndex,stPQ_GetGRule_LevelIndex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_IPIndex,stPQ_GetGRule_IPIndex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_TableIndex,stPQ_GetGRule_TableIndex);
    UADP_SPT_NAME0NXT(PQ_GetCustomerGRule_IPIndex,stPQ_GetCustomerGRule_IPIndex);
    UADP_SPT_NAME0NXT(PQ_GetGRule_CustomerTableIndex,stPQ_GetGRule_CustomerTableIndex);
    UADP_SPT_NAME0NXT(PQ_Set_xvYCC_MatrixCoefficient,stPQ_Set_xvYCC_MatrixCoefficient);
    UADP_SPT_NAME0NXT(PQ_Set_xvYCC_MatrixEnable,stPQ_Set_xvYCC_MatrixEnable);
    UADP_SPT_NAME0NXT(PQ_Get_DNR_Whole_Reg,stPQ_GetDnrWholeReg);
    UADP_SPT_NAME0NXT(PQ_Up_Layer_Set_Config,stPQ_Up_Layer_Set_Config);
    UADP_SPT_NAME0NXT(PQ_GetQmapExecutor,stPQ_GetQmapExecutor);
    UADP_SPT_NAME0NXT(PQ_SetPhotoYUVStandard,stPQ_SetPhotoYUVStandard);
//drvPQ_cus.c
    UADP_SPT_NAME0NXT(PQ_LoadFilmModeTable,stPQ_LoadFilmModeTable);
    UADP_SPT_NAME0NXT(PQ_LoadDynamicContrastTable,stPQ_LoadDynamicContrastTable);
    UADP_SPT_NAME0NXT(PQ_LoadNRTable,stPQ_LoadNRTable);
    UADP_SPT_NAME0NXT(PQ_LoadMPEGNRTable,stPQ_LoadMPEGNRTable);
    UADP_SPT_NAME0NXT(PQ_Load_ULTRACLEAR_Table,stPQ_Load_ULTRACLEAR_Table);
    UADP_SPT_NAME0NXT(PQ_Load_XVYCC_Table,stPQ_Load_XVYCC_Table);
    UADP_SPT_NAME0NXT(PQ_Load_HDR_Table,stPQ_Load_HDR_Table);
    UADP_SPT_NAME0NXT(PQ_GRULE_Get_Support_Status,stPQ_GRULE_Get_Support_Status);
    UADP_SPT_NAME1NXT(PQ_SetMemFormat,stPQ_SetMemFormat,pu8BitsPerPixel,PQ_MS_U8);
    UADP_SPT_NAME0NXT(PQ_ReduceBW_ForOSD,stPQ_ReduceBW_ForOSD);
    UADP_SPT_NAME0NXT(PQ_LoadPTPTable,stPQ_LoadPTPTable);
    UADP_SPT_NAME0NXT(PQ_Init_DisplayType,stPQ_Init_DisplayType);
    UADP_SPT_NAME0NXT(PQ_Set_DisplayType,stPQ_Set_DisplayType);
    UADP_SPT_NAME2NXT(PQ_GetMADiFromOSDBWGrule,stPQ_GetMADiFromOSDBWGrule
                                                ,pu16TabIdx_MADi,PQ_MS_U16
                                                ,pu16TabIdx_MADi_Motion,PQ_MS_U16);
    UADP_SPT_NAME2NXT(PQ_GetMADiInGeneral,stPQ_GetMADiInGeneral
                                                ,pu16TabIdx_MADi,PQ_MS_U16
                                                ,pu16TabIdx_MADi_Motion,PQ_MS_U16);
    UADP_SPT_NAME1NXT(PQ_Check_PointToPoint_Condition,stPQ_Check_PointToPoint_Condition
                                                ,pstXC_SetWin_Info,ADP_XC_SETWIN_INFO);
    UADP_SPT_NAME0NXT(PQ_SetPostCCSOnOff,stPQ_SetPostCCSOnOff);
    UADP_SPT_NAME0NXT(PQ_FilmMode_AnyCandence_Enable,stPQ_FilmMode_AnyCandence_Enable);
    UADP_SPT_NAME0NXT(PQ_SetH264_OnOff,stPQ_SetH264_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetG3D_OnOff,stPQ_SetG3D_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetMVC4kx1k_OnOff,stPQ_SetMVC4kx1k_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetNetworkMM_OnOff,stPQ_SetNetworkMM_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetMM_OnOff,stPQ_SetMM_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetRmvb_OnOff,stPQ_SetRmvb_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetVIPBypass_OnOff,stPQ_SetVIPBypass_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetPeaking_OnOff,stPQ_SetPeaking_OnOff);
    UADP_SPT_NAME0NXT(PQ_SetFakeOutEnable,stPQ_SetFakeOutEnable);
    UADP_SPT_NAME0NXT(PQ_Set1KTravelingEnable,stPQ_Set1KTravelingEnable);
//drvPQ_Text.c
//E_PQ_CMD_GET_SRC_TYPE_NAME:
//E_PQ_CMD_GET_IP_NAME:
//E_PQ_CMD_GET_TABLE_NAME:
//drvbw.c
    UADP_SPT_NAME0NXT(PQ_BW_LoadTableByContext,stPQ_BW_LoadTableByContext);
    UADP_SPT_NAME0NXT(PQ_BW_CusMode_LoadTable,stPQ_BW_CusMode_LoadTable);

#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
    UADP_SPT_NAME0NXT(PQ_MADiForceMotion,stPQ_MADiForceMotion);
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
    UADP_SPT_NAME0NXT(PQ_SetColorThroughMode,stPQ_SetColorThroughMode);
#endif

    *pIoctl= (FUtopiaIOctl)PQ_adp_Ioctl;

    return 0;
}


MS_U32 PQ_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=UTOPIA_STATUS_SUCCESS;
    char buffer_arg[2];

    //printk("\033[1;33m[%s:%d]PQ_CMD=%ld\033[m\n",__FUNCTION__,__LINE__,u32Cmd);

    switch(u32Cmd)
    {
        case E_PQ_CMD_INIT:
        {
            //pstPQ_INIT pPQArgs = (pstPQ_INIT)pArgs;
            //psPQInstPri->fpPQ_Init(pInstance, pPQArgs->pstPQInitInfo);
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INIT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_EXIT:
        {
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }
        case E_PQ_CMD_SET_POWER_STATE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetPowerState, spt_PQ_SetPowerState,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_IP_NUM:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetIPNum, spt_PQ_GetIPNum,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_TABLE_NUM:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetTableNum, spt_PQ_GetTableNum,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CURRENT_TABLE_INDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetCurrentTableIndex, spt_PQ_GetCurrentTableIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_TABLE_INDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetTableIndex, spt_PQ_GetTableIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CUSTOMER_TABLE_INDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetCustomerTableIndex, spt_PQ_GetCustomerTableIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        //case E_PQ_CMD_GET_XRULE_IP_INDEX:
        //{
        //    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetXRuleIPIndex, spt_PQ_GetXRuleIPIndex,buffer_arg,sizeof(buffer_arg));
        //    break;
        //}
        //case E_PQ_CMD_GET_XRULE_TABLE_INDEX:
        //{
        //    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetXRuleTableIndex, spt_PQ_GetXRuleTableIndex,buffer_arg,sizeof(buffer_arg));
        //    break;
        //}
        //case E_PQ_CMD_GET_XRULE_IP_NUM:
        //{
        //    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetXRuleIPNum, spt_PQ_GetXRuleIPNum,buffer_arg,sizeof(buffer_arg));
        //    break;
        //}
        case E_PQ_CMD_LOAD_TABLE_DATA:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadTableData, spt_PQ_LoadTableData,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_CLONE_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_CloneTable, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadTable, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_CUSTOMER_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadCustomerTable, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_CHECK_SETTINGS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_CheckSettings, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (ENABLE_PQ_MLOAD)
        case E_PQ_CMD_SET_MLOAD_EN:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_MLoadEn, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_SKIP_DUPLICATED_SETTING:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SkipDuplicatedSetting, spt_PQ_SkipDuplicatedSetting,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_SKIP_DUPLICATED_SETTING_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetSkipDuplicatedSettingStatus, spt_PQ_GetSkipDuplicatedSettingStatus,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
        case E_PQ_CMD_SET_FRCM_CB_CR_SWAP:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetFrcmCbCrSwap, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_LOAD_SETTINGS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadSettings, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_CUSTONER_SETTINGS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadCustomerSettings, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DTVINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_DTVInfo, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_MULTIMEDIAINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_MultiMediaInfo, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_VDINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_VDInfo, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_MODEINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_ModeInfo, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_HDMIINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetHDMIInfo, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_HDMI_PC:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetHDMI_PC, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_HDMI_PC_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetHDMI_PC_Status, spt_PQ_GetHDMI_PC_Status,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_DESIDE_SRC_TYPE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_DesideSrcType, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_INPUTSOURCE_TYPE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetInputSourceType, spt_PQ_GetInputSourceType,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_SRC_TYPE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetSrcType,spt_PQ_GetSrcType,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_DELAY_TIME_BY_MEMC:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetDelayTimeByMemc,spt_PQ_GetDelayTimeByMemc,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_DELAY_TIME:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetDelayTime,spt_PQ_GetDelayTime,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_SCALING_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadScalingTable,spt_PQ_LoadScalingTable,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_COLOR_RANGE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetColorRange,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CSC_XRULEIDX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_CSC_XRuleIdx,spt_PQ_Get_CSC_XRuleIdx,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_MEM_YUVFMT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_MemYUVFmt,spt_PQ_Get_MemYUVFmt,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_CSC:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetCSC,spt_PQ_SetCSC,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (PQ_XRULE_DB_ENABLE == 1)
        case E_PQ_CMD_SET_DBRULE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_DBRule,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_ENABLE_MADI_FORCE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_EnableMADIForce,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_REDUCE_BW_FOR_PVR:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_ReduceBW_ForPVR,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_420UP_SAMPLEING:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set420upsampling,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_FILEMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetFilmMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_NONLINEAR_SCALING:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetNonLinearScaling,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_3D_CLONE_FOR_PIP:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_3DCloneforPIP,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_DISABLE_FILEMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_DisableFilmMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_DISABLE_UC_FEATURE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_DisableUCFeature,spt_PQ_DisableUCFeature,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CURRENT_UC_FEATURE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetCurrentUCFeature,spt_PQ_GetCurrentUCFeature,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_UC_FEATURE_FROM_PQ:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetUCFeatureFromPQ,spt_PQ_GetUCFeatureFromPQ,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
        case E_PQ_CMD_ENABLE_UC_FEATURE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_EnableUCFeature,spt_PQ_EnableUCFeature,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_SET_UC_FEATURE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetUCFeature,spt_PQ_SetUCFeature,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DMSV12L:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetDMSV12L,spt_PQ_SetDMSV12L,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_DMSV12L_FROM_XRULE_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetDMSV12LFromXRuleTable,spt_PQ_GetDMSV12LFromXRuleTable,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_LIB_VER:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetLibVer,spt_PQ_GetLibVer,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_INFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetInfo,spt_PQ_GetInfo,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetStatus,spt_PQ_GetStatus,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DBGLEVEL:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetDbgLevel,spt_PQ_SetDbgLevel,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MADI_FORCE_MOTIONC:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MADiForceMotionC,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MADI_FORCE_MOTIONY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MADiForceMotionY,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
        case E_PQ_CMD_MADI_FORCE_MOTION:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MADiForceMotion,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
        case E_PQ_CMD_SET_COLOR_THROUGH_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetColorThroughMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_SET_RFBL_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetRFblMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_RFBL_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetRFblMode,spt_PQ_GetRFblMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (PQ_ENABLE_VD_SAMPLING)
        case E_PQ_CMD_GET_VDSAMPLING_INFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_VDSampling_Info,spt_PQ_Get_VDSampling_Info,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_INTER_IOCTL:
        {
            stPQ_INTER_IOCTL ptr;
            if(copy_from_user(&ptr, (stPQ_INTER_IOCTL __user *)pArgs, sizeof(stPQ_INTER_IOCTL)))
            {
                return UTOPIA_STATUS_FAIL;
            }
            switch(ptr.u32Flag)
            {
                case E_IOCTL_HSD_SAMPLING:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_HSD_SAMPLING,spt_PQ_INTER_IOCTL_HSD_SAMPLING,buffer_arg,sizeof(buffer_arg));
                break;
                case E_IOCTL_PREVSD_BILINEAR:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_PREVSD_BILINEAR,spt_PQ_INTER_IOCTL_PREVSD_BILINEAR,buffer_arg,sizeof(buffer_arg));
                break;
                case E_IOCTL_ADC_SAMPLING:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_ADC_SAMPLING,spt_PQ_INTER_IOCTL_ADC_SAMPLING,buffer_arg,sizeof(buffer_arg));
                break;
                case E_IOCTL_RFBL_CTRL:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_RFBL_CTRL,spt_PQ_INTER_IOCTL_RFBL_CTRL,buffer_arg,sizeof(buffer_arg));
                break;
                case E_IOCTL_PQ_SUGGESTED_FRAMENUM:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_SUGGESTED_FRAMENUM,spt_PQ_INTER_IOCTL_SUGGESTED_FRAMENUM,buffer_arg,sizeof(buffer_arg));
                break;
                case E_IOCTL_SET_UCFEATURE:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_INTER_IOCTL_SET_UCFEATURE,spt_PQ_INTER_IOCTL_SET_UCFEATURE,buffer_arg,sizeof(buffer_arg));
                break;
            }
            break;
        }
        case E_PQ_CMD_GET_ADC_SAMPLING_INTO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_ADCSampling_Info,spt_PQ_Get_ADCSampling_Info,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_3D_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set3D_OnOff,spt_PQ_Set3D_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CAPS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetCaps,spt_PQ_GetCaps,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_PRINT_LOADTABLE_INFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_PrintLoadTableInfo,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SETDS_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetDS_OnOFF,spt_PQ_SetDS_OnOFF,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_RFBL_INFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_RFBL_Info,spt_PQ_Get_RFBL_Info,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_CHECK_HD_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_CheckHDMode,spt_PQ_CheckHDMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_INTERLACE_SD_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_Interlace_SD_mode,spt_PQ_Get_Interlace_SD_mode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DUAL_VIEW_STATE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetDualViewState,spt_PQ_SetDualViewState,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_VGASUBCASE_FORCETO_422MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetVGASubCaseForceTo422Mode,spt_PQ_GetVGASubCaseForceTo422Mode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_DUAL_VIEW_STATE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetDualViewState,spt_PQ_GetDualViewState,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_FORCE_VIDEO_INPUT_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_ForceVideoInputMode,spt_PQ_ForceVideoInputMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_IS_FORCE_VIDEO_INPUTMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_IsForceVideoInputMode,spt_PQ_IsForceVideoInputMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_POINT2POINT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_PointToPoint,spt_PQ_Set_PointToPoint,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_POINT2POINT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_PointToPoint,spt_PQ_Get_PointToPoint,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_UPDATE_MEMFORMAT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Update_MemFormat,spt_PQ_Update_MemFormat,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_HSIZE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetHsize,spt_PQ_GetHsize,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_VSIZE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetVsize,spt_PQ_GetVsize,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_IS_INTERLACE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_IsInterlace,spt_PQ_IsInterlace,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOW_3D_QUALITY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LOW_3dQuality,spt_PQ_LOW_3dQuality,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MWE_SET_ENHANCE_QUALITY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MWE_SetEnhanceQuality,spt_PQ_MWE_SetEnhanceQuality,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MWE_RESTORE_ENHANCE_QUALITY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MWE_RestoreEnhanceQuality,spt_PQ_MWE_RestoreEnhanceQuality,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MWE_RESTORE_OFF_QUALITY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MWE_RestoreOffQuality,spt_PQ_MWE_RestoreOffQuality,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MWE_SET_OFF_QUALITY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MWE_SetOffQuality,spt_PQ_MWE_SetOffQuality,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_MWE_CLONE_WINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_MWE_CloneWindow,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_3D_SETTING_FOR_LBL:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_3D_SettingForLBL,spt_PQ_3D_SettingForLBL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_3D_CLONE_WINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_3D_CloneWindow,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_SENSIOMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_SensioMode,spt_PQ_Get_SensioMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_SENSIOMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_SensioMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DIPF:
        {
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_SELECT_CSC:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetSelectCSC,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_R2YEQ_SELECT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetR2YEqSelect,spt_PQ_GetR2YEqSelect,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_MEMFMT_IN_GENERAL:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetMemFmtInGeneral,spt_PQ_GetMemFmtInGeneral,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_MADI_FOR_RFBL:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetMADiForRFBL,spt_PQ_GetMADiForRFBL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_PATCH2RTO4R_FOR_FIELDPACKINGMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Patch2Rto4RForFieldPackingMode,spt_PQ_Patch2Rto4RForFieldPackingMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_BPPINFO_FROM_MADI:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetBPPInfoFromMADi,spt_PQ_GetBPPInfoFromMADi,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_CHANGE_MEMCONFIG_FOR3D:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_ChangeMemConfigFor3D,spt_PQ_ChangeMemConfigFor3D,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_FORCE_BPP_FOR_DYNAMIC_MEMFMT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_ForceBPPForDynamicMemFmt,spt_PQ_ForceBPPForDynamicMemFmt,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SUGGEST_FRAMENUM:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SuggestFrameNum,spt_PQ_SuggestFrameNum,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_GAMEMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetGameMode,spt_PQ_SetGameMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GAMEMODE_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGameMode_Status,spt_PQ_GetGameMode_Status,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_BYPASS_MODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetBypassMode,spt_PQ_SetBypassMode,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_BYPASS_MODE_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetBypassModeStatus,spt_PQ_GetBypassModeStatus,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_VERSION:
        {
            break;
        }
        case E_PQ_CMD_SET_GRULE_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetGRuleStatus,spt_PQ_SetGRuleStatus,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_LV1NUM:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_LvlNum,spt_PQ_GetGRule_LvlNum,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_GRULENUM:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_GRuleNum,spt_PQ_GetGRule_GRuleNum,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_LEVELINDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_LevelIndex,spt_PQ_GetGRule_LevelIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_IPINDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_IPIndex,spt_PQ_GetGRule_IPIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_TABLEINDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_TableIndex,spt_PQ_GetGRule_TableIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CUSTOMERGRULE_IPINDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetCustomerGRule_IPIndex,spt_PQ_GetCustomerGRule_IPIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_CUSTOMERGRULE_TABLEINDEX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_CustomerTableIndex,spt_PQ_GetGRule_CustomerTableIndex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_XVYCC_MATRIXCOEFFICIENT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_xvYCC_MatrixCoefficient,spt_PQ_Set_xvYCC_MatrixCoefficient,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_XVYCC_MATRIXENABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_xvYCC_MatrixEnable,spt_PQ_Set_xvYCC_MatrixEnable,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_DNR_WHOLE_REG:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_DNR_Whole_Reg,spt_PQ_Get_DNR_Whole_Reg,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_UP_LAYER_SET_CONFIG:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Up_Layer_Set_Config,spt_PQ_Up_Layer_Set_Config,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_QMAP_EXECUTOR:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetQmapExecutor,spt_PQ_GetQmapExecutor,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_PHOTO_YUV_STD:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetPhotoYUVStandard,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_TABLE_INDEX_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetTableIndex_Ex,spt_PQ_GetTableIndex_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_DEMO_CLONE_WINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Demo_CloneWindow,spt_PQ_Demo_CloneWindow,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_ENABLE_SCALER_GAMMA:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_EnableScalerGamma,spt_PQ_EnableScalerGamma,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_GAMMA_TBl:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetGammaTbl,spt_PQ_SetGammaTbl,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_LV1NUM_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_LvlNum_Ex,spt_PQ_GetGRule_LvlNum_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_LEVELINDEX_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_LevelIndex_Ex,spt_PQ_GetGRule_LevelIndex_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_IPINDEX_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_IPIndex_Ex,spt_PQ_GetGRule_IPIndex_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_GRULE_TABLEINDEX_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetGRule_TableIndex_Ex,spt_PQ_GetGRule_TableIndex_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_TABLE_EX:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadTable_Ex,spt_PQ_LoadTable_Ex,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_PQBYPASS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_PqBypass,spt_PQ_Set_PqBypass,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_PQBYPASS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Get_PqBypass,spt_PQ_Get_PqBypass,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_PQBIN_PATH:
        {
            stPQ_SetPQBinPath tmp;
#ifdef CONFIG_COMPAT
            _compat_stPQ_SetPQBinPath tmp_compat;
            CMP_CPY_FROM_USER(&tmp_compat, pArgs, sizeof(_compat_stPQ_SetPQBinPath));
            tmp.b_PQBinFilePath = malloc(tmp_compat.u8size);
            CMP_CPY_FROM_USER(tmp.b_PQBinFilePath, tmp_compat.b_PQBinFilePath, tmp_compat.u8size);
            tmp.ePqBinPath = tmp_compat.ePqBinPath;
            tmp.u8size = tmp_compat.u8size;
#else
            stPQ_SetPQBinPath tmp1;
            copy_from_user(&tmp1,pArgs,sizeof(stPQ_SetPQBinPath));
            tmp.b_PQBinFilePath = malloc(tmp1.u8size);
            copy_from_user(tmp.b_PQBinFilePath,tmp1.b_PQBinFilePath,tmp1.u8size);
            tmp.ePqBinPath = tmp1.ePqBinPath;
            tmp.u8size = tmp1.u8size;
#endif
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&tmp);
            free(tmp.b_PQBinFilePath);
            break;
        }

//drvPQ_cus.c
        case E_PQ_CMD_LOAD_FILMMODE_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadFilmModeTable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_DYNAMIC_CONTRAST_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadDynamicContrastTable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_NR_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadNRTable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_MPEG_NR_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadMPEGNRTable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_ULTRACLEAR_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Load_ULTRACLEAR_Table,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_XVYCC_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Load_XVYCC_Table,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_HDR_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Load_HDR_Table,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GRULE_GET_SUPPORT_STATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GRULE_Get_Support_Status,spt_PQ_GRULE_Get_Support_Status,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_MEMFORMAT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetMemFormat,spt_PQ_SetMemFormat,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_REDUCEBW_ForOSD:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_ReduceBW_ForOSD,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_PTP_TABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadPTPTable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_INIT_DISPLAY_TYPE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Init_DisplayType,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_DISPLAY_TYPE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set_DisplayType,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_MADI_FROM_OSDBW_GRULE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetMADiFromOSDBWGrule,spt_PQ_GetMADiFromOSDBWGrule,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_GET_MADI_IN_GENERAL:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetMADiInGeneral,spt_PQ_GetMADiInGeneral,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_CHECK_POINT2POINT_CONDITION:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Check_PointToPoint_Condition,spt_PQ_Check_PointToPoint_Condition,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_POSTCCS_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetPostCCSOnOff,spt_PQ_SetPostCCSOnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_ADAPTIVE_TUNING:
        {
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }
        case E_PQ_CMD_FILMMODE_ANYCANDENCE_ENABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_FilmMode_AnyCandence_Enable,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SETH264_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetH264_OnOff,spt_PQ_SetH264_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SETG3D_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetG3D_OnOff,spt_PQ_SetG3D_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SETMVC4KX1K_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetMVC4kx1k_OnOff,spt_PQ_SetMVC4kx1k_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_NETWORKMM_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetNetworkMM_OnOff,spt_PQ_SetNetworkMM_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SETMM_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetMM_OnOff,spt_PQ_SetMM_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_RMVB_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetRmvb_OnOff,spt_PQ_SetRmvb_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_VIPBYPASS_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetVIPBypass_OnOff,spt_PQ_SetVIPBypass_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_PEAKING_ONOFF:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetPeaking_OnOff,spt_PQ_SetPeaking_OnOff,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_FAKE_OUT_ENABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_SetFakeOutEnable,spt_PQ_SetFakeOutEnable,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_SET_1KTRAVELINGENABLE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_Set1KTravelingEnable,spt_PQ_Set1KTravelingEnable,buffer_arg,sizeof(buffer_arg));
            break;
        }
//drvPQ_Text.c
        case E_PQ_CMD_GET_SRC_TYPE_NAME:
        {
            break;
        }
        case E_PQ_CMD_GET_IP_NAME:
        {
            break;
        }
        case E_PQ_CMD_GET_TABLE_NAME:
        {
            break;
        }
//drvbw.c
        case E_PQ_CMD_BW_LOAD_INIT_TABLE:
        {
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }
        case E_PQ_CMD_BW_LOAD_TABLE_BY_CONTEXT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_BW_LoadTableByContext,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_BW_RESET_TO_DEFAULT:
        {
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }
        case E_PQ_CMD_BW_CUSMODE_LOADTABLE:
        {
            u32Ret= UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_BW_CusMode_LoadTable,spt_PQ_BW_CusMode_LoadTable,buffer_arg,sizeof(buffer_arg));
            break;
        }
        case E_PQ_CMD_LOAD_UFSC_SETTING:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadUFSCSettings,spt_PQ_LoadUFSCSettings,buffer_arg,sizeof(buffer_arg));
            break;
        }
#if (ENABLE_PQ_BIN_CF)
        case E_PQ_CMD_LOAD_CF_SETTING:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_LoadCFSettings,spt_PQ_LoadCFSettings,buffer_arg,sizeof(buffer_arg));
            break;
        }
#endif
        case E_PQ_CMD_GETCONFIG:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_PQ_GetConfig, spt_PQ_GetConfig,buffer_arg,sizeof(buffer_arg));
            break;
        }
        default:
            printk("Error Command = %u\n", u32Cmd);
        break;
    }
    return u32Ret;
}

