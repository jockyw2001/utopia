#ifndef _API_PQ_PANEL_V2_C_
#define _API_PQ_PANEL_V2_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"
#include "utopia_api_database.h"

// ========== Global definition ==========
void* pModulePQ = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
void *pInstancePQStr = NULL;
MS_U32 PQStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stPQ_SetPowerState PQArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstancePQStr == NULL)
            {
                if(UtopiaOpen(MODULE_PQ, &pInstancePQStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen PQ failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            PQArgs.enPowerState = u32PowerState;
            PQArgs.u32Ret = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstancePQStr, E_PQ_CMD_SET_POWER_STATE, (void*)&PQArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain PQ engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=PQArgs.u32Ret;
            break;
        case E_POWER_RESUME:
            if (pInstancePQStr == NULL)
            {
                if(UtopiaOpen(MODULE_PQ, &pInstancePQStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen PQ failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            PQArgs.enPowerState = u32PowerState;
            PQArgs.u32Ret = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstancePQStr, E_PQ_CMD_SET_POWER_STATE, (void*)&PQArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain PQ engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=PQArgs.u32Ret;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif
void PQRegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 u32ResourceStatusCheck[E_PQ_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    UtopiaModuleCreate(MODULE_PQ, 0, &pModulePQ);
    UtopiaModuleRegister(pModulePQ);
#if 1
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModulePQ, (FUtopiaOpen)PQOpen, (FUtopiaClose)PQClose, (FUtopiaIOctl)PQIoctl);

    // 2. deal with resource  , PQ may need 2 resources, one is readonly, the other is XC resource
    void* psResource = NULL;
    void* psResource1 = NULL;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModulePQ, (FUtopiaSTR)PQStr);
    UtopiaModuleSetSTRPrivate(pModulePQ, sizeof(PQ_REGS_SAVE_AREA));
#endif

    // 3. resource can allocate private for internal use
    MS_U32 u32MaxDeviceNum = _MDrv_PQ_GET_DEVICE_NUM();
    MS_U32 i = 0;
    for(i = 0 ; i < u32MaxDeviceNum ;i++)
    {
        // need refine
        if(i == 0)
        {
            UtopiaModuleAddResourceStart(pModulePQ, E_PQ_POOL_ID_INTERNAL);
            u32ResourceStatusCheck[E_PQ_POOL_ID_INTERNAL] = UtopiaResourceCreate("PQ_VARIABLE", sizeof(PQ_RESOURCE_PRIVATE), &psResource);
            UtopiaResourceRegister(pModulePQ, psResource, E_PQ_POOL_ID_INTERNAL);
            UtopiaModuleAddResourceEnd(pModulePQ, E_PQ_POOL_ID_INTERNAL);
        }
        else
        {
            UtopiaModuleAddResourceStart(pModulePQ, E_PQ_POOL_ID_INTERNAL1);
            u32ResourceStatusCheck[E_PQ_POOL_ID_INTERNAL1] = UtopiaResourceCreate("PQ_VARIABLE1", sizeof(PQ_RESOURCE_PRIVATE), &psResource1);
            UtopiaResourceRegister(pModulePQ, psResource1, E_PQ_POOL_ID_INTERNAL1);
            UtopiaModuleAddResourceEnd(pModulePQ, E_PQ_POOL_ID_INTERNAL1);
        }
    }

    //4. init resource private members here   (aka, global variable)
    PQ_RESOURCE_PRIVATE* pPQResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pPQResourcePrivate));
    char ptmp[PQ_FILE_PATH_LENGTH]={0};

    if (u32ResourceStatusCheck[E_PQ_POOL_ID_INTERNAL] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pPQResourcePrivate->bResourceRegistered = TRUE;
        strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_CUSTOMER_PATH,ptmp,PQ_FILE_PATH_LENGTH);
        strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_DEFAULT_PATH,ptmp,PQ_FILE_PATH_LENGTH);
        strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_INI_PATH,ptmp,PQ_FILE_PATH_LENGTH);
        strncpy(pPQResourcePrivate->stPQBin_Path.pm_PQ_BANDWIDTH_PATH,ptmp,PQ_FILE_PATH_LENGTH);
#ifdef UFO_XC_PQ_SUPPORT_INITVAR_INTO_SHAREMEMORY
        memset(&pPQResourcePrivate->PQ_InitInfo, 0x00, sizeof(MS_PQ_Init_Info));
#endif

        // apiPQ.c
        //pPQResourcePrivate->stapiPQ._bSkipTimingChange = FALSE;
        //pPQResourcePrivate->stapiPQ._cstPQInfo.eSupportGammaType = E_APIPQ_GAMMA_ALL;
        //pPQResourcePrivate->stapiPQ._pu8GammaTab[0]=NULL;
        //pPQResourcePrivate->stapiPQ._pu8GammaTab[1]=NULL;
        //pPQResourcePrivate->stapiPQ._pu8GammaTab[2]=NULL;
        //pPQResourcePrivate->stapiPQ._stPQStatus.bPanel_Initialized = FALSE;
        //pPQResourcePrivate->stapiPQ._stPQStatus.bPanel_Enabled = FALSE;
        //pPQResourcePrivate->stapiPQ._u16PanelDCLK = 0;
        //pPQResourcePrivate->stapiPQ._bForceSetDCLKEnable = DISABLE;
        //pPQResourcePrivate->stapiPQ._u16PanelHStart = 0;
        //pPQResourcePrivate->stapiPQ._bForceSetHStartEnable = DISABLE;
        //pPQResourcePrivate->stapiPQ.ePrevPowerState = E_POWER_MECHANICAL;

        // halPQ.c
        //pPQResourcePrivate->sthalPQ._u16PQDefault_SwingLevel = 250;
        //pPQResourcePrivate->sthalPQ._eDrvPQInitOptions = E_PQ_OUTPUT_CLK_DATA;
        //pPQResourcePrivate->sthalPQ._u8MOD_CALI_TARGET = 1; // 00:250mv, 01:350mv, 10:300mv, 11:200mv
        //pPQResourcePrivate->sthalPQ._u8MOD_CALI_PAIR_SEL = 0x01;
        //pPQResourcePrivate->sthalPQ._usMOD_CALI_OFFSET = 0;
        //pPQResourcePrivate->sthalPQ._u8MOD_CALI_VALUE  = 0x1F; // 250mv:2'h15, 350mv:2'h1F, 300mv:2'h1A, 200mv:2'h10
        //pPQResourcePrivate->sthalPQ._u8MOD_LVDS_Pair_Shift = 0;
        //pPQResourcePrivate->sthalPQ._u8MOD_LVDS_Pair_Type = 0;
        //pPQResourcePrivate->sthalPQ._u8PQDiffSwingLevel = 0x63;
        //pPQResourcePrivate->sthalPQ._bPVDD_2V5 = FALSE;
    }

    if (psResource1 != NULL)
    {
        UtopiaResourceGetPrivate(psResource1,(void**)(&pPQResourcePrivate));

        if (u32ResourceStatusCheck[E_PQ_POOL_ID_INTERNAL1] == UTOPIA_STATUS_SHM_EXIST)
        {
            // do nothing, since it is already inited
        }
        else
        {
            // Init flow control related variables here. Other global variable should be
            // inited in each of its init function relatively.
            pPQResourcePrivate->bResourceRegistered = TRUE;
#ifdef UFO_XC_PQ_SUPPORT_INITVAR_INTO_SHAREMEMORY
            memset(&pPQResourcePrivate->PQ_InitInfo, 0x00, sizeof(MS_PQ_Init_Info));
#endif

            // apiPQ.c
            //pPQResourcePrivate->stapiPQ._bSkipTimingChange = FALSE;
            //pPQResourcePrivate->stapiPQ._cstPQInfo.eSupportGammaType = E_APIPQ_GAMMA_ALL;
            //pPQResourcePrivate->stapiPQ._stPQStatus.bPanel_Initialized = FALSE;
            //pPQResourcePrivate->stapiPQ._stPQStatus.bPanel_Enabled = FALSE;
            //pPQResourcePrivate->stapiPQ._u16PanelDCLK = 0;
            //pPQResourcePrivate->stapiPQ._bForceSetDCLKEnable = DISABLE;
            //pPQResourcePrivate->stapiPQ._u16PanelHStart = 0;
            //pPQResourcePrivate->stapiPQ._bForceSetHStartEnable = DISABLE;
            //pPQResourcePrivate->stapiPQ.ePrevPowerState = E_POWER_MECHANICAL;

            // halPQ.c
            //pPQResourcePrivate->sthalPQ._u16PQDefault_SwingLevel = 250;
            //pPQResourcePrivate->sthalPQ._eDrvPQInitOptions = E_PQ_OUTPUT_CLK_DATA;
            //pPQResourcePrivate->sthalPQ._u8MOD_CALI_TARGET = 0;
            //pPQResourcePrivate->sthalPQ._u8MOD_CALI_PAIR_SEL = 0x01;
            //pPQResourcePrivate->sthalPQ._usMOD_CALI_OFFSET = 0;
            //pPQResourcePrivate->sthalPQ._u8MOD_CALI_VALUE  = 0x15;
            //pPQResourcePrivate->sthalPQ._u8MOD_LVDS_Pair_Shift = 0;
            //pPQResourcePrivate->sthalPQ._u8MOD_LVDS_Pair_Type = 0;
            //pPQResourcePrivate->sthalPQ._u8PQDiffSwingLevel = 0x63;
            //pPQResourcePrivate->sthalPQ._bPVDD_2V5 = FALSE;
        }
    }
#endif
}

MS_U32 PQOpen(void** ppInstance,MS_U32 u32ModuleVersion, void* pAttribute)
{
#if 1
    PQ_INSTANCE_PRIVATE *psPQInstPri = NULL;
    UtopiaInstanceCreate(sizeof(PQ_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psPQInstPri);

    psPQInstPri->u32DeviceID             = 0;
    //todo:  have to check later why AN use 32bits libutopia.so casue error
    /*if(pAttribute != NULL)
    {
        MS_U32* pID = (MS_U32*)pAttribute;
        psPQInstPri->u32DeviceID = *pID;
    }*/
#ifdef CONFIG_UTOPIA_SIZE_CUSTOMER
    #define URELATION(a,b,c)   psPQInstPri->_URELATION_ASSIGN(a,b,c)
            _MODULE_PQ_U1_U2_RELATION
    #undef URELEATION
#else
    psPQInstPri->fpPQ_Init                                = MDrv_PQ_Init_U2;
    psPQInstPri->fpPQ_Exit                                = MDrv_PQ_Exit_U2;
    psPQInstPri->fpPQ_SetPowerState                        = MDrv_PQ_SetPowerState_U2;
    psPQInstPri->fpPQ_GetIPNum                            = MDrv_PQ_GetIPNum_U2;
    psPQInstPri->fpPQ_GetTableNum                        = MDrv_PQ_GetTableNum_U2;
    psPQInstPri->fpPQ_GetCurrentTableIndex                = MDrv_PQ_GetCurrentTableIndex_U2;
    psPQInstPri->fpPQ_GetTableIndex                        = MDrv_PQ_GetTableIndex_U2;
    psPQInstPri->fpPQ_GetCustomerTableIndex                = MDrv_PQ_GetCustomerTableIndex_U2;
    //psPQInstPri->fpPQ_GetXRuleIPIndex                    = MDrv_PQ_GetXRuleIPIndex_U2;
    //psPQInstPri->fpPQ_GetXRuleTableIndex                = MDrv_PQ_GetXRuleTableIndex_U2;
    //psPQInstPri->fpPQ_GetXRuleIPNum                        = MDrv_PQ_GetXRuleIPNum_U2;
    psPQInstPri->fpPQ_LoadTableData                        = MDrv_PQ_LoadTableData_U2;
    psPQInstPri->fpPQ_CloneTable                        = MDrv_PQ_CloneTable_U2;
    psPQInstPri->fpPQ_LoadTable                            = MDrv_PQ_LoadTable_U2;
    psPQInstPri->fpPQ_LoadCustomerTable                    = MDrv_PQ_LoadCustomerTable_U2;
    psPQInstPri->fpPQ_CheckSettings                        = MDrv_PQ_CheckSettings_U2;
#if    (ENABLE_PQ_MLOAD)
    psPQInstPri->fpPQ_MLoadEn                            = MDrv_PQ_Set_MLoadEn_U2;
#endif
    psPQInstPri->fpPQ_SkipDuplicatedSetting                = MDrv_PQ_SkipDuplicatedSetting_U2;
    psPQInstPri->fpPQ_GetSkipDuplicatedSettingStatus    = MDrv_PQ_GetSkipDuplicatedSettingStatus_U2;
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
    psPQInstPri->fpPQ_SetFrcmCbCrSwap                    = MDrv_PQ_SetFrcmCbCrSwap_U2;
#endif
    psPQInstPri->fpPQ_LoadSettings                        = MDrv_PQ_LoadSettings_U2;
    psPQInstPri->fpPQ_LoadCustomerSettings                = MDrv_PQ_LoadCustomerSettings_U2;
    psPQInstPri->fpPQ_Set_DTVInfo                        = MDrv_PQ_Set_DTVInfo_U2;
    psPQInstPri->fpPQ_Set_MultiMediaInfo                = MDrv_PQ_Set_MultiMediaInfo_U2;
    psPQInstPri->fpPQ_Set_VDInfo                        = MDrv_PQ_Set_VDInfo_U2;
    psPQInstPri->fpPQ_Set_ModeInfo                        = MDrv_PQ_Set_ModeInfo_U2;
    psPQInstPri->fpPQ_SetHDMIInfo                        = MDrv_PQ_SetHDMIInfo_U2;
    psPQInstPri->fpPQ_SetHDMI_PC                        = MDrv_PQ_SetHDMI_PC_U2;
    psPQInstPri->fpPQ_GetHDMI_PC_Status                    = MDrv_PQ_GetHDMI_PC_Status_U2;
    psPQInstPri->fpPQ_DesideSrcType                        = MDrv_PQ_DesideSrcType_U2;
    psPQInstPri->fpPQ_GetInputSourceType                = MDrv_PQ_GetInputSourceType_U2;
    psPQInstPri->fpPQ_GetSrcType                        = MDrv_PQ_GetSrcType_U2;
    psPQInstPri->fpPQ_GetDelayTimeByMemc                = MDrv_PQ_GetDelayTimeByMemc_U2;
    psPQInstPri->fpPQ_GetDelayTime                        = MDrv_PQ_GetDelayTime_U2;
    psPQInstPri->fpPQ_LoadScalingTable                    = MDrv_PQ_LoadScalingTable_U2;
    psPQInstPri->fpPQ_SetColorRange                        = MDrv_PQ_SetColorRange_U2;
    psPQInstPri->fpPQ_Get_CSC_XRuleIdx                    = MDrv_PQ_Get_CSC_XRuleIdx_U2;
    psPQInstPri->fpPQ_Get_MemYUVFmt                        = MDrv_PQ_Get_MemYUVFmt_U2;
    psPQInstPri->fpPQ_SetCSC                            = MDrv_PQ_SetCSC_U2;
#if (PQ_XRULE_DB_ENABLE == 1)
    psPQInstPri->fpPQ_Set_DBRule                        = MDrv_PQ_Set_DBRule_U2;
#endif
    psPQInstPri->fpPQ_EnableMADIForce                    = MDrv_PQ_EnableMADIForce_U2;
    psPQInstPri->fpPQ_ReduceBW_ForPVR                    = MDrv_PQ_ReduceBW_ForPVR_U2;
    psPQInstPri->fpPQ_Set420upsampling                    = MDrv_PQ_Set420upsampling_U2;
    psPQInstPri->fpPQ_SetFilmMode                        = MDrv_PQ_SetFilmMode_U2;
    psPQInstPri->fpPQ_SetNonLinearScaling                = MDrv_PQ_SetNonLinearScaling_U2;
    psPQInstPri->fpPQ_3DCloneforPIP                        = MDrv_PQ_3DCloneforPIP_U2;
    psPQInstPri->fpPQ_DisableFilmMode                    = MDrv_PQ_DisableFilmMode_U2;
    psPQInstPri->fpPQ_DisableUCFeature                    = MDrv_PQ_DisableUCFeature_U2;
    psPQInstPri->fpPQ_GetCurrentUCFeature                = MDrv_PQ_GetCurrentUCFeature_U2;
    psPQInstPri->fpPQ_GetUCFeatureFromPQ                = MDrv_PQ_GetUCFeatureFromPQ_U2;
#if    (PQ_GRULE_GAME_MODE_ENABLE == 1)
    psPQInstPri->fpPQ_EnableUCFeature                    = MDrv_PQ_EnableUCFeature_U2;
#endif
    psPQInstPri->fpPQ_SetUCFeature                        = MDrv_PQ_SetUCFeature_U2;
    psPQInstPri->fpPQ_SetDMSV12L                        = MDrv_PQ_SetDMSV12L_U2;
    psPQInstPri->fpPQ_GetDMSV12LFromXRuleTable            = MDrv_PQ_GetDMSV12LFromXRuleTable_U2;
    psPQInstPri->fpPQ_GetLibVer                            = MDrv_PQ_GetLibVer_U2;
    psPQInstPri->fpPQ_GetInfo                            = MDrv_PQ_GetInfo_U2;
    psPQInstPri->fpPQ_GetStatus                            = MDrv_PQ_GetStatus_U2;
    psPQInstPri->fpPQ_SetDbgLevel                        = MDrv_PQ_SetDbgLevel_U2;
    psPQInstPri->fpPQ_MADiForceMotionC                    = MDrv_PQ_MADiForceMotionC_U2;
    psPQInstPri->fpPQ_MADiForceMotionY                    = MDrv_PQ_MADiForceMotionY_U2;
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
    psPQInstPri->fpPQ_MADiForceMotion                    = MDrv_PQ_MADiForceMotion_U2;
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
    psPQInstPri->fpPQ_SetColorThroughMode                = MDrv_PQ_SetColorThroughMode_U2;
#endif

    psPQInstPri->fpPQ_SetRFblMode                        = MDrv_PQ_SetRFblMode_U2;
    psPQInstPri->fpPQ_GetRFblMode                        = MDrv_PQ_GetRFblMode_U2;
#if (PQ_ENABLE_VD_SAMPLING)
    psPQInstPri->fpPQ_Get_VDSampling_Info                = MDrv_PQ_Get_VDSampling_Info_U2;
#endif
    psPQInstPri->fpPQ_INTER_IOCTL                        = MDrv_PQ_IOCTL_U2;
    psPQInstPri->fpPQ_Get_ADCSampling_Info                = MDrv_PQ_Get_ADCSampling_Info_U2;
    psPQInstPri->fpPQ_Set3D_OnOff                        = MDrv_PQ_Set3D_OnOff_U2;
    psPQInstPri->fpPQ_GetCaps                            = MDrv_PQ_GetCaps_U2;
    psPQInstPri->fpPQ_PrintLoadTableInfo                = MDRV_PQ_PrintLoadTableInfo_U2;
    psPQInstPri->fpPQ_SetDS_OnOFF                        = MDrv_PQ_SetDS_OnOFF_U2;
    psPQInstPri->fpPQ_Get_RFBL_Info                        = MDrv_PQ_Get_RFBL_Info_U2;
    psPQInstPri->fpPQ_CheckHDMode                        = MDrv_PQ_CheckHDMode_U2;
    psPQInstPri->fpPQ_Interlace_SD_mode                    = MDrv_PQ_Get_Interlace_SD_mode_U2;
    psPQInstPri->fpPQ_SetDualViewState                    = MDrv_PQ_SetDualViewState_U2;
    psPQInstPri->fpPQ_GetVGASubCaseForceTo422Mode        = MDrv_PQ_GetVGASubCaseForceTo422Mode_U2;
    psPQInstPri->fpPQ_GetDualViewState                    = MDrv_PQ_GetDualViewState_U2;
    psPQInstPri->fpPQ_ForceVideoInputMode                = MDrv_PQ_ForceVideoInputMode_U2;
    psPQInstPri->fpPQ_IsForceVideoInputMode                = MDrv_PQ_IsForceVideoInputMode_U2;
    psPQInstPri->fpPQ_Set_PointToPoint                    = MDrv_PQ_Set_PointToPoint_U2;
    psPQInstPri->fpPQ_Get_PointToPoint                    = MDrv_PQ_Get_PointToPoint_U2;
    psPQInstPri->fpPQ_Update_MemFormat                    = MDrv_PQ_Update_MemFormat_U2;
    psPQInstPri->fpPQ_GetHsize                            = MDrv_PQ_GetHsize_U2;
    psPQInstPri->fpPQ_GetVsize                            = MDrv_PQ_GetVsize_U2;
    psPQInstPri->fpPQ_IsInterlace                        = MDrv_PQ_IsInterlace_U2;
    psPQInstPri->fpPQ_LOW_3dQuality                        = MDrv_PQ_LOW_3dQuality_U2;
    psPQInstPri->fpPQ_MWE_SetEnhanceQuality                = MDrv_PQ_MWE_SetEnhanceQuality_U2;
    psPQInstPri->fpPQ_MWE_RestoreEnhanceQuality            = MDrv_PQ_MWE_RestoreEnhanceQuality_U2;
    psPQInstPri->fpPQ_MWE_RestoreOffQuality                = MDrv_PQ_MWE_RestoreOffQuality_U2;
    psPQInstPri->fpPQ_MWE_SetOffQuality                    = MDrv_PQ_MWE_SetOffQuality_U2;
    psPQInstPri->fpPQ_MWE_CloneWindow                    = MDrv_PQ_MWE_CloneWindow_U2;
    psPQInstPri->fpPQ_3D_SettingForLBL                    = MDrv_PQ_3D_SettingForLBL_U2;
    psPQInstPri->fpPQ_3D_CloneWindow                    = MDrv_PQ_3D_CloneWindow_U2;
    psPQInstPri->fpPQ_Get_SensioMode                     = MDrv_PQ_Get_SensioMode_U2;
    psPQInstPri->fpPQ_Set_SensioMode                    = MDrv_PQ_Set_SensioMode_U2;
    psPQInstPri->fpPQ_Set_DIPF                            = MDrv_PQ_Set_DIPF_U2;
    psPQInstPri->fpPQ_SetSelectCSC                        = MDrv_PQ_SetSelectCSC_U2;
    psPQInstPri->fpPQ_GetR2YEqSelect                    = MDrv_PQ_GetR2YEqSelect_U2;
    psPQInstPri->fpPQ_GetMemFmtInGeneral                = MDrv_PQ_GetMemFmtInGeneral_U2;
    psPQInstPri->fpPQ_GetMADiForRFBL                    = MDrv_PQ_GetMADiForRFBL_U2;
    psPQInstPri->fpPQ_Patch2Rto4RForFieldPackingMode    = MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2;
    psPQInstPri->fpPQ_GetBPPInfoFromMADi                = MDrv_PQ_GetBPPInfoFromMADi_U2;
    psPQInstPri->fpPQ_ChangeMemConfigFor3D                = MDrv_PQ_ChangeMemConfigFor3D_U2;
    psPQInstPri->fpPQ_ForceBPPForDynamicMemFmt            = MDrv_PQ_ForceBPPForDynamicMemFmt_U2;
    psPQInstPri->fpPQ_SuggestFrameNum                    = MDrv_PQ_SuggestFrameNum_U2;
    psPQInstPri->fpPQ_SetGameMode                        = MDrv_PQ_SetGameMode_U2;
    psPQInstPri->fpPQ_GetGameMode_Status                = MDrv_PQ_GetGameMode_Status_U2;
    psPQInstPri->fpPQ_SetBypassMode                        = MDrv_PQ_SetBypassMode_U2;
    psPQInstPri->fpPQ_GetBypassModeStatus                = MDrv_PQ_GetBypassModeStatus_U2;
    psPQInstPri->fpPQ_GetVersion                        = MDrv_PQ_GetVersion_U2;
    psPQInstPri->fpPQ_Cus_GetVersion                    = MDrv_PQ_Cus_GetVersion_U2;
    psPQInstPri->fpPQ_SetGRuleStatus                    = MDrv_PQ_SetGRuleStatus_U2;
    psPQInstPri->fpPQ_GetGRule_LvlNum                    = MDrv_PQ_GetGRule_LvlNum_U2;
    psPQInstPri->fpPQ_GetGRule_GRuleNum                    = MDrv_PQ_GetGRule_GRuleNum_U2;
    psPQInstPri->fpPQ_GetGRule_LevelIndex                = MDrv_PQ_GetGRule_LevelIndex_U2;
    psPQInstPri->fpPQ_GetGRule_IPIndex                    = MDrv_PQ_GetGRule_IPIndex_U2;
    psPQInstPri->fpPQ_GetGRule_TableIndex                = MDrv_PQ_GetGRule_TableIndex_U2;
    psPQInstPri->fpPQ_GetCustomerGRule_IPIndex            = MDrv_PQ_GetCustomerGRule_IPIndex_U2;
    psPQInstPri->fpPQ_GetGRule_CustomerTableIndex        = MDrv_PQ_GetGRule_CustomerTableIndex_U2;
    psPQInstPri->fpPQ_Set_xvYCC_MatrixCoefficient        = MDrv_PQ_Set_xvYCC_MatrixCoefficient_U2;
    psPQInstPri->fpPQ_Get_DNR_Whole_reg                  = MDrv_PQ_Get_DNR_Whole_Reg_U2;
    psPQInstPri->fpPQ_Up_Layer_Set_Config                = MDrv_PQ_Up_Layer_Set_Config_U2;
    psPQInstPri->fpPQ_GetQmapExecutor                    = MDrv_PQ_GetQmapExecutor_U2;
    psPQInstPri->fpPQ_SetPhotoYUVStandard                = MDrv_PQ_SetPhotoYUVStandard_U2;
    psPQInstPri->fpPQ_Ex_GetVersion                        = MDrv_PQ_Ex_GetVersion_U2;
    psPQInstPri->fpPQ_GetAdaptiveVersion                = MDrv_PQ_GetAdaptiveVersion_U2;
    psPQInstPri->fpPQ_LoadUFSCSettings                    = MDrv_PQ_LoadUFSCSettings_U2;
#if (ENABLE_PQ_BIN_CF)
    psPQInstPri->fpPQ_LoadCFSettings                    = MDrv_PQ_LoadCFSettings_U2;
#endif
    psPQInstPri->fpPQ_Set_PqBypass                                 = MDrv_PQ_Set_PqBypass_U2;
    psPQInstPri->fpPQ_Get_PqBypass                                 = MDrv_PQ_Get_PqBypass_U2;
    psPQInstPri->fpPQ_Set_PqBin_Path                                 = MDrv_PQ_SetPQBinPath_U2;
//drvPQ_cus.c
    psPQInstPri->fpPQ_LoadFilmModeTable                    = MDrv_PQ_LoadFilmModeTable_U2;
    psPQInstPri->fpPQ_LoadDynamicContrastTable            = MDrv_PQ_LoadDynamicContrastTable_U2;
    psPQInstPri->fpPQ_LoadNRTable                        = MDrv_PQ_LoadNRTable_U2;
    psPQInstPri->fpPQ_LoadMPEGNRTable                    = MDrv_PQ_LoadMPEGNRTable_U2;
    psPQInstPri->fpPQ_Load_ULTRACLEAR_Table                = MDrv_PQ_Load_ULTRACLEAR_Table_U2;
    psPQInstPri->fpPQ_Load_XVYCC_Table                    = MDrv_PQ_Load_XVYCC_Table_U2;
    psPQInstPri->fpPQ_Load_HDR_Table                    = MDrv_PQ_Load_HDR_Table_U2;
    psPQInstPri->fpPQ_GRULE_Get_Support_Status            = MDrv_PQ_GRULE_Get_Support_Status_U2;
    psPQInstPri->fpPQ_SetMemFormat                        = MDrv_PQ_SetMemFormat_U2;
    psPQInstPri->fpPQ_ReduceBW_ForOSD                    = MDrv_PQ_ReduceBW_ForOSD_U2;
    psPQInstPri->fpPQ_LoadPTPTable                        = MDrv_PQ_LoadPTPTable_U2;
    psPQInstPri->fpPQ_Init_DisplayType                    = MDrv_PQ_Init_DisplayType_U2;
    psPQInstPri->fpPQ_Set_DisplayType                    = MDrv_PQ_Set_DisplayType_U2;
    psPQInstPri->fpPQ_GetMADiFromOSDBWGrule                = MDrv_PQ_GetMADiFromOSDBWGrule_U2;
    psPQInstPri->fpPQ_GetMADiInGeneral                    = MDrv_PQ_GetMADiInGeneral_U2;
    psPQInstPri->fpPQ_Check_PointToPoint_Condition        = MDrv_PQ_Check_PointToPoint_Condition_U2;
    psPQInstPri->fpPQ_SetPostCCSOnOff                    = MDrv_PQ_SetPostCCSOnOff_U2;
    psPQInstPri->fpPQ_AdaptiveTuning                    = MDrv_PQ_AdaptiveTuning_U2;
    psPQInstPri->fpPQ_FilmMode_AnyCandence_Enable        = MDrv_PQ_FilmMode_AnyCandence_Enable_U2;
    psPQInstPri->fpPQ_SetH264_OnOff                        = MDrv_PQ_SetH264_OnOff_U2;
    psPQInstPri->fpPQ_SetG3D_OnOff                        = MDrv_PQ_SetG3D_OnOff_U2;
    psPQInstPri->fpPQ_SetMVC4kx1k_OnOff                    = MDrv_PQ_SetMVC4kx1k_OnOff_U2;
    psPQInstPri->fpPQ_SetNetworkMM_OnOff                = MDrv_PQ_SetNetworkMM_OnOff_U2;
    psPQInstPri->fpPQ_SetMM_OnOff                        = MDrv_PQ_SetMM_OnOff_U2;
    psPQInstPri->fpPQ_SetRmvb_OnOff                        = MDrv_PQ_SetRmvb_OnOff_U2;
    psPQInstPri->fpPQ_SetVIPBypass_OnOff                = MDrv_PQ_SetVIPBypass_OnOff_U2;
    psPQInstPri->fpPQ_SetPeaking_OnOff                    = MDrv_PQ_SetPeaking_OnOff_U2;
    psPQInstPri->fpPQ_SetFakeOutEnable                     = MDrv_PQ_SetFakeOutEnable_U2;
    psPQInstPri->fpPQ_GetTableIndex_Ex                                      = MDrv_PQ_GetTableIndex_Ex_U2;
    psPQInstPri->fpPQ_LoadTable_Ex                                          = MDrv_PQ_LoadTable_Ex_U2;
    psPQInstPri->fpPQ_Demo_CloneWindow                                      = MDrv_PQ_Demo_CloneWindow_U2;
    psPQInstPri->fpPQ_GetGRule_LvlNum_Ex                                    = MDrv_PQ_GetGRule_LvlNum_Ex_U2;
    psPQInstPri->fpPQ_GetGRule_LevelIndex_Ex                                = MDrv_PQ_GetGRule_LevelIndex_Ex_U2;
    psPQInstPri->fpPQ_GetGRule_IPIndex_Ex                                   = MDrv_PQ_GetGRule_IPIndex_Ex_U2;
    psPQInstPri->fpPQ_GetGRule_TableIndex_Ex                                = MDrv_PQ_GetGRule_TableIndex_Ex_U2;
    psPQInstPri->fpPQ_EnableScalerGamma                                     = MDrv_PQ_EnableScalerGamma_U2;
    psPQInstPri->fpPQ_SetGammaTbl                                           = MDrv_PQ_SetGammaTbl_U2;
    psPQInstPri->fpPQ_Set1KTravelingEnable                                 = MDrv_PQ_Set1KTravelingEnable_U2;
//drvPQ.c
    psPQInstPri->fpPQ_GetSrcTypeName                     = MDrv_PQ_GetSrcTypeName_U2;
    psPQInstPri->fpPQ_GetIPName                             = MDrv_PQ_GetIPName_U2;
    psPQInstPri->fpPQ_GetTableName                         = MDrv_PQ_GetTableName_U2;
//drvbw.c
    psPQInstPri->fpPQ_BW_LoadInitTable                     = MDrv_BW_LoadInitTable_U2;
    psPQInstPri->fpPQ_BW_LoadTableByContext                = MDrv_BW_LoadTableByContext_U2;
    psPQInstPri->fpPQ_BW_ResetToDefault                    = MDrv_BW_ResetToDefault_U2;
       psPQInstPri->fpPQ_BW_CusMode_LoadTable                            = MDrv_BW_CusMode_LoadTable_U2;
    psPQInstPri->fpPQ_GetConfig                            = MDrv_PQ_GetConfig_U2;
    psPQInstPri->fpPQ_SET_FrameBuff_Mode                   = MDrv_PQ_SetFrameBuffMode_U2;
#endif
#endif
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 PQIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
#if 1
    //void* pModule = NULL;
    PQ_INSTANCE_PRIVATE* psPQInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psPQInstPri);
    //UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__);
    //pu32PQInst_private = pInstance;
    switch(u32Cmd)
    {
        case E_PQ_CMD_INIT:
        {
            pstPQ_INIT pPQArgs = (pstPQ_INIT)pArgs;
            psPQInstPri->fpPQ_Init(pInstance, pPQArgs->pstPQInitInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_EXIT:
        {
            psPQInstPri->fpPQ_Exit(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_POWER_STATE:
        {
            pstPQ_SetPowerState pPQArgs = (pstPQ_SetPowerState)pArgs;
            pPQArgs->u32Ret = psPQInstPri->fpPQ_SetPowerState(pInstance, pPQArgs->enPowerState);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_IP_NUM:
        {
            pstPQ_GetIPNum pPQArgs = (pstPQ_GetIPNum)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_GetIPNum(pInstance, pPQArgs->eWindow);
            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_TABLE_NUM:
        {
            pstPQ_GetTableNum pPQArgs = (pstPQ_GetTableNum)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_GetTableNum(pInstance, pPQArgs->eWindow, pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CURRENT_TABLE_INDEX:
        {
            pstPQ_GetCurrentTableIndex pPQArgs = (pstPQ_GetCurrentTableIndex)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_GetCurrentTableIndex(pInstance, pPQArgs->eWindow, pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_TABLE_INDEX:
        {
            pstPQ_GetTableIndex pPQArgs = (pstPQ_GetTableIndex)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_GetTableIndex(pInstance, pPQArgs->eWindow, pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CUSTOMER_TABLE_INDEX:
        {
            pstPQ_GetCustomerTableIndex pPQArgs = (pstPQ_GetCustomerTableIndex)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_GetCustomerTableIndex(pInstance, pPQArgs->eWindow, pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        //case E_PQ_CMD_GET_XRULE_IP_INDEX:
        //{
        //    pstPQ_GetXRuleIPIndex pPQArgs = (pstPQ_GetXRuleIPIndex)pArgs;
        //    pPQArgs->u16Ret = psPQInstPri->fpPQ_GetXRuleIPIndex(pInstance, pPQArgs->eWindow, pPQArgs->u16XRuleType,pPQArgs->u16XRuleIP);

        //    return UTOPIA_STATUS_SUCCESS;
        //}
        //case E_PQ_CMD_GET_XRULE_TABLE_INDEX:
        //{
        //    pstPQ_GetXRuleTableIndex pPQArgs = (pstPQ_GetXRuleTableIndex)pArgs;
        //    pPQArgs->u16Ret = psPQInstPri->fpPQ_GetXRuleTableIndex(pInstance, pPQArgs->eWindow,
        //                                                                        pPQArgs->u16XRuleType,
        //                                                                        pPQArgs->u16XRuleIdx,
        //                                                                        pPQArgs->u16XRuleIP);

        //    return UTOPIA_STATUS_SUCCESS;
        //}
        //case E_PQ_CMD_GET_XRULE_IP_NUM:
        //{
        //    pstPQ_GetXRuleIPNum pPQArgs = (pstPQ_GetXRuleIPNum)pArgs;
        //    pPQArgs->u16Ret = psPQInstPri->fpPQ_GetXRuleIPNum(pInstance, pPQArgs->eWindow, pPQArgs->u16XRuleType);

        //    return UTOPIA_STATUS_SUCCESS;
        //}
        case E_PQ_CMD_LOAD_TABLE_DATA:
        {
            pstPQ_LoadTableData pPQArgs = (pstPQ_LoadTableData)pArgs;
            psPQInstPri->fpPQ_LoadTableData(pInstance, pPQArgs->eWindow ,
                                                        pPQArgs->u16TabIdx ,
                                                        pPQArgs->u16PQIPIdx ,
                                                        pPQArgs->pTable ,
                                                        pPQArgs->u16TableSize);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CLONE_TABLE:
        {
            pstPQ_CloneTable pPQArgs = (pstPQ_CloneTable)pArgs;
            psPQInstPri->fpPQ_CloneTable(pInstance, pPQArgs->eWindow ,
                                                        pPQArgs->u16TabIdx ,
                                                        pPQArgs->u16PQMainIPIdx ,
                                                        pPQArgs->u16PQSubIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_TABLE:
        {
            pstPQ_LoadTable pPQArgs = (pstPQ_LoadTable)pArgs;
            psPQInstPri->fpPQ_LoadTable(pInstance, pPQArgs->eWindow ,
                                                        pPQArgs->u16TabIdx ,
                                                        pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_CUSTOMER_TABLE:
        {
            pstPQ_LoadCustomerTable pPQArgs = (pstPQ_LoadCustomerTable)pArgs;
            psPQInstPri->fpPQ_LoadCustomerTable(pInstance, pPQArgs->eWindow ,
                                                            pPQArgs->u16TabIdx ,
                                                            pPQArgs->u16PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CHECK_SETTINGS:
        {
            pstPQ_CheckSettings pPQArgs = (pstPQ_CheckSettings)pArgs;
            psPQInstPri->fpPQ_CheckSettings(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (ENABLE_PQ_MLOAD)
        case E_PQ_CMD_SET_MLOAD_EN:
        {
            pstPQ_Set_MLoadEn pPQArgs = (pstPQ_Set_MLoadEn)pArgs;
            psPQInstPri->fpPQ_MLoadEn(pInstance, pPQArgs->eWindow, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
        case E_PQ_CMD_SKIP_DUPLICATED_SETTING:
        {
            pstPQ_SkipDuplicatedSetting pPQArgs = (pstPQ_SkipDuplicatedSetting)pArgs;
            pPQArgs->bReturnValue = psPQInstPri->fpPQ_SkipDuplicatedSetting(pInstance, pPQArgs->eWindow, pPQArgs->bSkip);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_SKIP_DUPLICATED_SETTING_STATUS:
        {
            pstPQ_GetSkipDuplicatedSettingStatus pPQArgs = (pstPQ_GetSkipDuplicatedSettingStatus)pArgs;
            pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetSkipDuplicatedSettingStatus(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (PQ_FRCM_CBCR_SWAP_BY_SW == 1)
        case E_PQ_CMD_SET_FRCM_CB_CR_SWAP:
        {
            pstPQ_SetFrcmCbCrSwap pPQArgs = (pstPQ_SetFrcmCbCrSwap)pArgs;
            psPQInstPri->fpPQ_SetFrcmCbCrSwap(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
        case E_PQ_CMD_LOAD_SETTINGS:
        {
            pstPQ_LoadSettings pPQArgs = (pstPQ_LoadSettings)pArgs;
            psPQInstPri->fpPQ_LoadSettings(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_CUSTONER_SETTINGS:
        {
            pstPQ_LoadCustomerSettings pPQArgs = (pstPQ_LoadCustomerSettings)pArgs;
            psPQInstPri->fpPQ_LoadCustomerSettings(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DTVINFO:
        {
            pstPQ_Set_DTVInfo pPQArgs = (pstPQ_Set_DTVInfo)pArgs;
            psPQInstPri->fpPQ_Set_DTVInfo(pInstance, pPQArgs->eWindow, pPQArgs->pstPQDTVInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_MULTIMEDIAINFO:
        {
            pstPQ_Set_MultiMediaInfo pPQArgs = (pstPQ_Set_MultiMediaInfo)pArgs;
            psPQInstPri->fpPQ_Set_MultiMediaInfo(pInstance, pPQArgs->eWindow, pPQArgs->pstPQMMInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_VDINFO:
        {
            pstPQ_Set_VDInfo pPQArgs = (pstPQ_Set_VDInfo)pArgs;
            psPQInstPri->fpPQ_Set_VDInfo(pInstance, pPQArgs->eWindow, pPQArgs->pstPQVDInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_MODEINFO:
        {
            pstPQ_Set_ModeInfo pPQArgs = (pstPQ_Set_ModeInfo)pArgs;
            psPQInstPri->fpPQ_Set_ModeInfo(pInstance, pPQArgs->eWindow, pPQArgs->enInputSourceType ,pPQArgs->pstPQModeInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_HDMIINFO:
        {
            pstPQ_SetHDMIInfo pPQArgs = (pstPQ_SetHDMIInfo)pArgs;
            psPQInstPri->fpPQ_SetHDMIInfo(pInstance, pPQArgs->eWindow, &(pPQArgs->pstPQHDMIInfo));

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_HDMI_PC:
        {
            pstPQ_SetHDMI_PC pPQArgs = (pstPQ_SetHDMI_PC)pArgs;
            psPQInstPri->fpPQ_SetHDMI_PC(pInstance, pPQArgs->eWindow, pPQArgs->bIsTrue);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_HDMI_PC_STATUS:
        {
            pstPQ_GetHDMI_PC_Status pPQArgs = (pstPQ_GetHDMI_PC_Status)pArgs;
            pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetHDMI_PC_Status(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_DESIDE_SRC_TYPE:
        {
            pstPQ_DesideSrcType pPQArgs = (pstPQ_DesideSrcType)pArgs;
               psPQInstPri->fpPQ_DesideSrcType(pInstance, pPQArgs->eWindow, pPQArgs->enInputSourceType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_INPUTSOURCE_TYPE:
        {
            pstPQ_GetInputSourceType pPQArgs = (pstPQ_GetInputSourceType)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetInputSourceType(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_SRC_TYPE:
        {
            pstPQ_GetSrcType pPQArgs = (pstPQ_GetSrcType)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetSrcType(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_DELAY_TIME_BY_MEMC:
        {
            pstPQ_GetDelayTimeByMemc pPQArgs = (pstPQ_GetDelayTimeByMemc)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetDelayTimeByMemc(pInstance,pPQArgs->eWindow,pPQArgs->u32MemcFrameDelay);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_DELAY_TIME:
        {
            pstPQ_GetDelayTime pPQArgs = (pstPQ_GetDelayTime)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetDelayTime(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_SCALING_TABLE:
        {
            pstPQ_LoadScalingTable pPQArgs = (pstPQ_LoadScalingTable)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_LoadScalingTable(pInstance ,pPQArgs->eWindow,pPQArgs->eXRuleType ,
                                                                                    pPQArgs->bPreV_ScalingDown,
                                                                                    pPQArgs->bInterlace,
                                                                                    pPQArgs->bColorSpaceYUV,
                                                                                    pPQArgs->u16InputSize,
                                                                                    pPQArgs->u16SizeAfterScaling);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_COLOR_RANGE:
        {
            pstPQ_SetColorRange pPQArgs = (pstPQ_SetColorRange)pArgs;
               psPQInstPri->fpPQ_SetColorRange(pInstance ,pPQArgs->eWindow,pPQArgs->bColorRange0_255);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CSC_XRULEIDX:
        {
            pstPQ_Get_CSC_XRuleIdx pPQArgs = (pstPQ_Get_CSC_XRuleIdx)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_CSC_XRuleIdx(pInstance ,pPQArgs->pbInputColorSpaceRGB ,
                                                                                    pPQArgs->pu16DoCSC,
                                                                                    pPQArgs->eWindow,
                                                                                    pPQArgs->enForceColor,
                                                                                    pPQArgs->bLoadPQTable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_MEM_YUVFMT:
        {
            pstPQ_Get_MemYUVFmt pPQArgs = (pstPQ_Get_MemYUVFmt)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_MemYUVFmt(pInstance ,pPQArgs->eWindow, pPQArgs->enForceColor);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_CSC:
        {
            pstPQ_SetCSC pPQArgs = (pstPQ_SetCSC)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetCSC(pInstance ,pPQArgs->eWindow, pPQArgs->enForceColor);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (PQ_XRULE_DB_ENABLE == 1)
        case E_PQ_CMD_SET_DBRULE:
        {
            pstPQ_Set_DBRule pPQArgs = (pstPQ_Set_DBRule)pArgs;
               psPQInstPri->fpPQ_Set_DBRule(pInstance ,pPQArgs->eWindow, pPQArgs->u16MADi_Idx);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
        case E_PQ_CMD_ENABLE_MADI_FORCE:
        {
            pstPQ_EnableMADIForce pPQArgs = (pstPQ_EnableMADIForce)pArgs;
               psPQInstPri->fpPQ_EnableMADIForce(pInstance ,pPQArgs->eWindow, pPQArgs->bFullMotion);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_REDUCE_BW_FOR_PVR:
        {
            pstPQ_ReduceBW_ForPVR pPQArgs = (pstPQ_ReduceBW_ForPVR)pArgs;
               psPQInstPri->fpPQ_ReduceBW_ForPVR(pInstance ,pPQArgs->eWindow, pPQArgs->bPVR_On);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_420UP_SAMPLEING:
        {
            pstPQ_Set420upsampling pPQArgs = (pstPQ_Set420upsampling)pArgs;
               psPQInstPri->fpPQ_Set420upsampling(pInstance ,pPQArgs->eWindow, pPQArgs->bFBL , pPQArgs->bPreV_ScalingDown, pPQArgs->u16V_CropStart);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_FILEMODE:
        {
            pstPQ_SetFilmMode pPQArgs = (pstPQ_SetFilmMode)pArgs;
               psPQInstPri->fpPQ_SetFilmMode(pInstance ,pPQArgs->eWindow, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_NONLINEAR_SCALING:
        {
            pstPQ_SetNonLinearScaling pPQArgs = (pstPQ_SetNonLinearScaling)pArgs;
               psPQInstPri->fpPQ_SetNonLinearScaling(pInstance ,pPQArgs->eWindow, pPQArgs->u8Level,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_3D_CLONE_FOR_PIP:
        {
            pstPQ_3DCloneforPIP pPQArgs = (pstPQ_3DCloneforPIP)pArgs;
               psPQInstPri->fpPQ_3DCloneforPIP(pInstance ,pPQArgs->bIpSync);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_DISABLE_FILEMODE:
        {
            pstPQ_DisableFilmMode pPQArgs = (pstPQ_DisableFilmMode)pArgs;
               psPQInstPri->fpPQ_DisableFilmMode(pInstance ,pPQArgs->eWindow,pPQArgs->bOn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_DISABLE_UC_FEATURE:
        {
            pstPQ_DisableUCFeature pPQArgs = (pstPQ_DisableUCFeature)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_DisableUCFeature(pInstance ,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CURRENT_UC_FEATURE:
        {
            pstPQ_GetCurrentUCFeature pPQArgs = (pstPQ_GetCurrentUCFeature)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetCurrentUCFeature(pInstance ,pPQArgs->eWindow,
                                                                                        pPQArgs->pu16TabIdx_MADi,
                                                                                        pPQArgs->pu16TabIdx_MADi_Mot,
                                                                                        pPQArgs->pu16TabIdx_MADi_DFK,
                                                                                        pPQArgs->pu16TabIdx_MADi_SST,
                                                                                        pPQArgs->pu16TabIdx_MADi_EODiW,
                                                                                        pPQArgs->pu16TabIdx_UCNR,
                                                                                        pPQArgs->pu16TabIdx_UCDi,
                                                                                        pPQArgs->pu16TabIdx_UCCTL,
                                                                                        pPQArgs->pu16TabIdx_SwDriver,
                                                                                        pPQArgs->pu16TabIdx_PostCCS,
                                                                                        pPQArgs->pu16TabIdx_DHD);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_UC_FEATURE_FROM_PQ:
        {
            pstPQ_GetUCFeatureFromPQ pPQArgs = (pstPQ_GetUCFeatureFromPQ)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetUCFeatureFromPQ(pInstance ,pPQArgs->eWindow,
                                                                                        pPQArgs->pu16TabIdx_MADi_DFK,
                                                                                        pPQArgs->pu16TabIdx_MADi_SST,
                                                                                        pPQArgs->pu16TabIdx_MADi_EODiW,
                                                                                        pPQArgs->pu16TabIdx_UCNR,
                                                                                        pPQArgs->pu16TabIdx_UCDi,
                                                                                        pPQArgs->pu16TabIdx_UCCTL,
                                                                                        pPQArgs->pu16TabIdx_SwDriver,
                                                                                        pPQArgs->pu16TabIdx_PostCCS,
                                                                                        pPQArgs->pu16TabIdx_DHD);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
        case E_PQ_CMD_ENABLE_UC_FEATURE:
        {
            pstPQ_EnableUCFeature pPQArgs = (pstPQ_EnableUCFeature)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_EnableUCFeature(pInstance ,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
        case E_PQ_CMD_SET_UC_FEATURE:
        {
            pstPQ_SetUCFeature pPQArgs = (pstPQ_SetUCFeature)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetUCFeature(pInstance ,pPQArgs->eWindow,
                                                                                   pPQArgs->pu16TabIdx_MADi,
                                                                                   pPQArgs->pu16TabIdx_MADi_Mot,
                                                                                   pPQArgs->pu16TabIdx_MADi_DFK,
                                                                                   pPQArgs->pu16TabIdx_MADi_SST,
                                                                                   pPQArgs->pu16TabIdx_MADi_EODiW,
                                                                                   pPQArgs->pu16TabIdx_UCNR,
                                                                                   pPQArgs->pu16TabIdx_UCDi,
                                                                                   pPQArgs->pu16TabIdx_UCCTL,
                                                                                   pPQArgs->pu16TabIdx_SwDriver,
                                                                                   pPQArgs->pu16TabIdx_PostCCS,
                                                                                   pPQArgs->pu16TabIdx_DHD);


            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DMSV12L:
        {
            pstPQ_SetDMSV12L pPQArgs = (pstPQ_SetDMSV12L)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetDMSV12L(pInstance ,pPQArgs->eWindow,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_DMSV12L_FROM_XRULE_TABLE:
        {
            pstPQ_GetDMSV12LFromXRuleTable pPQArgs = (pstPQ_GetDMSV12LFromXRuleTable)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetDMSV12LFromXRuleTable(pInstance ,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_LIB_VER:
        {
            pstPQ_GetLibVer pPQArgs = (pstPQ_GetLibVer)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetLibVer(pInstance ,pPQArgs->ppVersion);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_INFO:
        {
            pstPQ_GetInfo pPQArgs = (pstPQ_GetInfo)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetInfo(pInstance ,pPQArgs->pInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_STATUS:
        {
            pstPQ_GetStatus pPQArgs = (pstPQ_GetStatus)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetStatus(pInstance ,pPQArgs->pStatus);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DBGLEVEL:
        {
            pstPQ_SetDbgLevel pPQArgs = (pstPQ_SetDbgLevel)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetDbgLevel(pInstance ,pPQArgs->u16DbgSwitch);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MADI_FORCE_MOTIONC:
        {
            pstPQ_MADiForceMotionC pPQArgs = (pstPQ_MADiForceMotionC)pArgs;
               psPQInstPri->fpPQ_MADiForceMotionC(pInstance ,pPQArgs->eWindow, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MADI_FORCE_MOTIONY:
        {
            pstPQ_MADiForceMotionY pPQArgs = (pstPQ_MADiForceMotionY)pArgs;
               psPQInstPri->fpPQ_MADiForceMotionY(pInstance ,pPQArgs->eWindow, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
#ifdef UFO_XC_PQ_SUPPORT_SWMOTION
        case E_PQ_CMD_MADI_FORCE_MOTION:
        {
            pstPQ_MADiForceMotion pPQArgs = (pstPQ_MADiForceMotion)pArgs;
               psPQInstPri->fpPQ_MADiForceMotion(pInstance ,pPQArgs->eWindow, pPQArgs->bEnableY, pPQArgs->u16DataY
                , pPQArgs->bEnableC, pPQArgs->u16DataC);
            return UTOPIA_STATUS_SUCCESS;
        }
#endif
#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
        case E_PQ_CMD_SET_COLOR_THROUGH_MODE:
        {
            pstPQ_SetColorThroughMode pPQArgs = (pstPQ_SetColorThroughMode)pArgs;
               psPQInstPri->fpPQ_SetColorThroughMode(pInstance ,pPQArgs->eWindow, pPQArgs->bEn);
            return UTOPIA_STATUS_SUCCESS;
        }
#endif

        case E_PQ_CMD_SET_RFBL_MODE:
        {
            pstPQ_SetRFblMode pPQArgs = (pstPQ_SetRFblMode)pArgs;
               psPQInstPri->fpPQ_SetRFblMode(pInstance ,pPQArgs->bEnable, pPQArgs->bFilm);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_RFBL_MODE:
        {
            pstPQ_GetRFblMode pPQArgs = (pstPQ_GetRFblMode)pArgs;
               psPQInstPri->fpPQ_GetRFblMode(pInstance ,pPQArgs->pstInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (PQ_ENABLE_VD_SAMPLING)
        case E_PQ_CMD_GET_VDSAMPLING_INFO:
        {
            pstPQ_Get_VDSampling_Info pPQArgs = (pstPQ_Get_VDSampling_Info)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_VDSampling_Info(pInstance ,pPQArgs->eWindow,
                                                                                        pPQArgs->enInputSourceType,
                                                                                        pPQArgs->eStandard,
                                                                                        pPQArgs->pInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
        case E_PQ_CMD_INTER_IOCTL:
        {
            pstPQ_INTER_IOCTL pPQArgs = (pstPQ_INTER_IOCTL)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_INTER_IOCTL(pInstance ,pPQArgs->eWindow,
                                                                                pPQArgs->u32Flag,
                                                                                pPQArgs->pBuf,
                                                                                pPQArgs->u32BufSize);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_ADC_SAMPLING_INTO:
        {
            pstPQ_Get_ADCSampling_Info pPQArgs = (pstPQ_Get_ADCSampling_Info)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_ADCSampling_Info(pInstance ,pPQArgs->eWindow,
                                                                                        pPQArgs->enInputSourceType,
                                                                                        pPQArgs->pstPQModeInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_3D_ONOFF:
        {
            pstPQ_Set3D_OnOff pPQArgs = (pstPQ_Set3D_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set3D_OnOff(pInstance ,pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CAPS:
        {
            pstPQ_GetCaps pPQArgs = (pstPQ_GetCaps)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetCaps(pInstance ,pPQArgs->pInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_PRINT_LOADTABLE_INFO:
        {
            pstPQ_PrintLoadTableInfo pPQArgs = (pstPQ_PrintLoadTableInfo)pArgs;
               psPQInstPri->fpPQ_PrintLoadTableInfo(pInstance ,pPQArgs->enPQWin);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SETDS_ONOFF:
        {
            pstPQ_SetDS_OnOFF pPQArgs = (pstPQ_SetDS_OnOFF)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetDS_OnOFF(pInstance ,pPQArgs->eWindow ,pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_RFBL_INFO:
        {
            pstPQ_Get_RFBL_Info pPQArgs = (pstPQ_Get_RFBL_Info)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_RFBL_Info(pInstance ,pPQArgs->eWindow );

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CHECK_HD_MODE:
        {
            pstPQ_CheckHDMode pPQArgs = (pstPQ_CheckHDMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_CheckHDMode(pInstance ,pPQArgs->eWindow );

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_INTERLACE_SD_MODE:
        {
            pstPQ_Get_Interlace_SD_mode pPQArgs = (pstPQ_Get_Interlace_SD_mode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Interlace_SD_mode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DUAL_VIEW_STATE:
        {
            pstPQ_SetDualViewState pPQArgs = (pstPQ_SetDualViewState)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetDualViewState(pInstance, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_VGASUBCASE_FORCETO_422MODE:
        {
            pstPQ_GetVGASubCaseForceTo422Mode pPQArgs = (pstPQ_GetVGASubCaseForceTo422Mode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetVGASubCaseForceTo422Mode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_DUAL_VIEW_STATE:
        {
            pstPQ_GetDualViewState pPQArgs = (pstPQ_GetDualViewState)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetDualViewState(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_FORCE_VIDEO_INPUT_MODE:
        {
            pstPQ_ForceVideoInputMode pPQArgs = (pstPQ_ForceVideoInputMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_ForceVideoInputMode(pInstance, pPQArgs->bEnable,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_IS_FORCE_VIDEO_INPUTMODE:
        {
            pstPQ_IsForceVideoInputMode pPQArgs = (pstPQ_IsForceVideoInputMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_IsForceVideoInputMode(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_POINT2POINT:
        {
            pstPQ_Set_PointToPoint pPQArgs = (pstPQ_Set_PointToPoint)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set_PointToPoint(pInstance, pPQArgs->pstXC_SetWin_Info,pPQArgs->bEnable,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_POINT2POINT:
        {
            pstPQ_Get_PointToPoint pPQArgs = (pstPQ_Get_PointToPoint)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_PointToPoint(pInstance, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_UPDATE_MEMFORMAT:
        {
            pstPQ_Update_MemFormat pPQArgs = (pstPQ_Update_MemFormat)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Update_MemFormat(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_HSIZE:
        {
            pstPQ_GetHsize pPQArgs = (pstPQ_GetHsize)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetHsize(pInstance, pPQArgs->enPQWin);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_VSIZE:
        {
            pstPQ_GetVsize pPQArgs = (pstPQ_GetVsize)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetVsize(pInstance, pPQArgs->enPQWin);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_IS_INTERLACE:
        {
            pstPQ_IsInterlace pPQArgs = (pstPQ_IsInterlace)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_IsInterlace(pInstance, pPQArgs->enPQWin);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOW_3D_QUALITY:
        {
            pstPQ_LOW_3dQuality pPQArgs = (pstPQ_LOW_3dQuality)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_LOW_3dQuality(pInstance, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MWE_SET_ENHANCE_QUALITY:
        {
            pstPQ_MWE_SetEnhanceQuality pPQArgs = (pstPQ_MWE_SetEnhanceQuality)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_MWE_SetEnhanceQuality(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MWE_RESTORE_ENHANCE_QUALITY:
        {
            pstPQ_MWE_RestoreEnhanceQuality pPQArgs = (pstPQ_MWE_RestoreEnhanceQuality)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_MWE_RestoreEnhanceQuality(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MWE_RESTORE_OFF_QUALITY:
        {
            pstPQ_MWE_RestoreOffQuality pPQArgs = (pstPQ_MWE_RestoreOffQuality)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_MWE_RestoreOffQuality(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MWE_SET_OFF_QUALITY:
        {
            pstPQ_MWE_SetOffQuality pPQArgs = (pstPQ_MWE_SetOffQuality)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_MWE_SetOffQuality(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_MWE_CLONE_WINDOW:
        {
            pstPQ_MWE_CloneWindow pPQArgs = (pstPQ_MWE_CloneWindow)pArgs;
               psPQInstPri->fpPQ_MWE_CloneWindow(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_3D_SETTING_FOR_LBL:
        {
            pstPQ_3D_SettingForLBL pPQArgs = (pstPQ_3D_SettingForLBL)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_3D_SettingForLBL(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_3D_CLONE_WINDOW:
        {
            pstPQ_3D_CloneWindow pPQArgs = (pstPQ_3D_CloneWindow)pArgs;
               psPQInstPri->fpPQ_3D_CloneWindow(pInstance,pPQArgs->enPQ3DType,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_SENSIOMODE:
        {
            pstPQ_Get_SensioMode pPQArgs = (pstPQ_Get_SensioMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_SensioMode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_SENSIOMODE:
        {
            pstPQ_Set_SensioMode pPQArgs = (pstPQ_Set_SensioMode)pArgs;
               psPQInstPri->fpPQ_Set_SensioMode(pInstance,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DIPF:
        {
               psPQInstPri->fpPQ_Set_DIPF(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_SELECT_CSC:
        {
            pstPQ_SetSelectCSC pPQArgs = (pstPQ_SetSelectCSC)pArgs;
               psPQInstPri->fpPQ_SetSelectCSC(pInstance,pPQArgs->u16selection,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_R2YEQ_SELECT:
        {
            pstPQ_GetR2YEqSelect pPQArgs = (pstPQ_GetR2YEqSelect)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetR2YEqSelect(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_MEMFMT_IN_GENERAL:
        {
            pstPQ_GetMemFmtInGeneral pPQArgs = (pstPQ_GetMemFmtInGeneral)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetMemFmtInGeneral(pInstance,pPQArgs->eWindow,
                                                                                    pPQArgs->bMemFmt422,
                                                                                    pPQArgs->pu16TabIdx_MemFormat,
                                                                                    pPQArgs->pu16TabIdx_444To422,
                                                                                    pPQArgs->pu16TabIdx_422To444);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_MADI_FOR_RFBL:
        {
            pstPQ_GetMADiForRFBL pPQArgs = (pstPQ_GetMADiForRFBL)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetMADiForRFBL(pInstance,pPQArgs->eWindow,
                                                                                    pPQArgs->bFBL,
                                                                                    pPQArgs->pu16TabIdx_MADi_Motion,
                                                                                    pPQArgs->pu16TabIdx_MADi);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_PATCH2RTO4R_FOR_FIELDPACKINGMODE:
        {
            pstPQ_Patch2Rto4RForFieldPackingMode pPQArgs = (pstPQ_Patch2Rto4RForFieldPackingMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Patch2Rto4RForFieldPackingMode(pInstance,pPQArgs->eWindow,pPQArgs->u16TabIdx_MADi);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_BPPINFO_FROM_MADI:
        {
            pstPQ_GetBPPInfoFromMADi pPQArgs = (pstPQ_GetBPPInfoFromMADi)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetBPPInfoFromMADi(pInstance,pPQArgs->eWindow,
                                                                                        pPQArgs->bMemFmt422,
                                                                                        pPQArgs->u16TabIdx_MADi,
                                                                                        pPQArgs->u16TabIdx_MemFormat,
                                                                                        pPQArgs->pu8BitsPerPixel,
                                                                                        pPQArgs->peDeInterlaceMode,
                                                                                        pPQArgs->pu8FrameCount);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CHANGE_MEMCONFIG_FOR3D:
        {
            pstPQ_ChangeMemConfigFor3D pPQArgs = (pstPQ_ChangeMemConfigFor3D)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_ChangeMemConfigFor3D(pInstance,pPQArgs->eWindow,
                                                                                        pPQArgs->bMemFmt422,
                                                                                        pPQArgs->pu16TabIdx_MemFormat,
                                                                                        pPQArgs->pu16TabIdx_444To422,
                                                                                        pPQArgs->pu16TabIdx_422To444,
                                                                                        pPQArgs->pu8BitsPerPixel,
                                                                                        pPQArgs->pu16TabIdx_MADi_Motion,
                                                                                        pPQArgs->pu16TabIdx_MADi);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_FORCE_BPP_FOR_DYNAMIC_MEMFMT:
        {
            pstPQ_ForceBPPForDynamicMemFmt pPQArgs = (pstPQ_ForceBPPForDynamicMemFmt)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_ForceBPPForDynamicMemFmt(pInstance,pPQArgs->eWindow,pPQArgs->bMemFmt422,pPQArgs->pu8BitsPerPixel);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SUGGEST_FRAMENUM:
        {
            pstPQ_SuggestFrameNum pPQArgs = (pstPQ_SuggestFrameNum)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SuggestFrameNum(pInstance,pPQArgs->eWindow,pPQArgs->u16TabIdx_MADi,pPQArgs->u8FrameCount);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_GAMEMODE:
        {
            pstPQ_SetGameMode pPQArgs = (pstPQ_SetGameMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetGameMode(pInstance,pPQArgs->eWindow,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GAMEMODE_STATUS:
        {
            pstPQ_GetGameMode_Status pPQArgs = (pstPQ_GetGameMode_Status)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetGameMode_Status(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_BYPASS_MODE:
        {
            pstPQ_SetBypassMode pPQArgs = (pstPQ_SetBypassMode)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetBypassMode(pInstance,pPQArgs->eWindow,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_BYPASS_MODE_STATUS:
        {
            pstPQ_GetBypassModeStatus pPQArgs = (pstPQ_GetBypassModeStatus)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetBypassModeStatus(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_VERSION:
        {
            pstPQ_GetVersion pPQArgs = (pstPQ_GetVersion)pArgs;
               pPQArgs->u8Ret = psPQInstPri->fpPQ_GetVersion(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CUS_GET_VERSION:
        {
            pstPQ_Cus_GetVersion pPQArgs = (pstPQ_Cus_GetVersion)pArgs;
               pPQArgs->u8Ret = psPQInstPri->fpPQ_Cus_GetVersion(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_GRULE_STATUS:
        {
            pstPQ_SetGRuleStatus pPQArgs = (pstPQ_SetGRuleStatus)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetGRuleStatus(pInstance,pPQArgs->eWindow,pPQArgs->u16GruleType,pPQArgs->u16GruleStatus);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GRULE_LV1NUM:
        {
            pstPQ_GetGRule_LvlNum pPQArgs = (pstPQ_GetGRule_LvlNum)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_LvlNum(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GRULE_GRULENUM:
        {
            pstPQ_GetGRule_GRuleNum pPQArgs = (pstPQ_GetGRule_GRuleNum)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_GRuleNum(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GRULE_LEVELINDEX:
        {
            pstPQ_GetGRule_LevelIndex pPQArgs = (pstPQ_GetGRule_LevelIndex)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_LevelIndex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->u16GRuleLevelIndex);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GRULE_IPINDEX:
        {
            pstPQ_GetGRule_IPIndex pPQArgs = (pstPQ_GetGRule_IPIndex)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_IPIndex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->u16GRuleIPIndex);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_GRULE_TABLEINDEX:
        {
            pstPQ_GetGRule_TableIndex pPQArgs = (pstPQ_GetGRule_TableIndex)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_TableIndex(pInstance,pPQArgs->eWindow,
                                                                                pPQArgs->u16GRuleType,
                                                                                pPQArgs->u16PQ_NRIdx,
                                                                                pPQArgs->u16GRuleIPIndex);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CUSTOMERGRULE_IPINDEX:
        {
            pstPQ_GetCustomerGRule_IPIndex pPQArgs = (pstPQ_GetCustomerGRule_IPIndex)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetCustomerGRule_IPIndex(pInstance,pPQArgs->eWindow,
                                                                                pPQArgs->u16GRuleType,
                                                                                pPQArgs->u16GRuleIPIndex);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_CUSTOMERGRULE_TABLEINDEX:
        {
            pstPQ_GetGRule_CustomerTableIndex pPQArgs = (pstPQ_GetGRule_CustomerTableIndex)pArgs;
               pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_CustomerTableIndex(pInstance,pPQArgs->eWindow,
                                                                                pPQArgs->u16GRuleType,
                                                                                pPQArgs->u16PQ_NRIdx,
                                                                                pPQArgs->u16GRuleIPIndex);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_XVYCC_MATRIXCOEFFICIENT:
        {
            pstPQ_Set_xvYCC_MatrixCoefficient pPQArgs = (pstPQ_Set_xvYCC_MatrixCoefficient)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set_xvYCC_MatrixCoefficient(pInstance,pPQArgs->u64Red_x,
                                                                                                pPQArgs->u64Red_y,
                                                                                                pPQArgs->u64Green_x,
                                                                                                pPQArgs->u64Green_y,
                                                                                                pPQArgs->u64Blue_x,
                                                                                                pPQArgs->u64Blue_y,
                                                                                                pPQArgs->u64White_x,
                                                                                                pPQArgs->u64White_y,
                                                                                                pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_XVYCC_MATRIXENABLE:
        {
            pstPQ_Set_xvYCC_MatrixEnable pPQArgs = (pstPQ_Set_xvYCC_MatrixEnable)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set_xvYCC_MatrixEnable(pInstance,pPQArgs->bEnable,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_DNR_WHOLE_REG:
        {
            pstPQ_GetDnrWholeReg pPQArgs = (pstPQ_GetDnrWholeReg)pArgs;
            pPQArgs->u16Ret = psPQInstPri->fpPQ_Get_DNR_Whole_reg(pInstance, pPQArgs->eWindow);
            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_UP_LAYER_SET_CONFIG:
        {
            pstPQ_Up_Layer_Set_Config pPQArgs = (pstPQ_Up_Layer_Set_Config)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Up_Layer_Set_Config(pInstance,pPQArgs->uplayerSetting,pPQArgs->group);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_QMAP_EXECUTOR:
        {
            pstPQ_GetQmapExecutor pPQArgs = (pstPQ_GetQmapExecutor)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetQmapExecutor(pInstance,pPQArgs->eFunctionType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_PHOTO_YUV_STD:
        {
            pstPQ_SetPhotoYUVStandard pPQArgs = (pstPQ_SetPhotoYUVStandard)pArgs;
               psPQInstPri->fpPQ_SetPhotoYUVStandard(pInstance,pPQArgs->eWindow, pPQArgs->enStd);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_EX_GET_VERSION:
        {
            pstPQ_Ex_GetVersion pPQArgs = (pstPQ_Ex_GetVersion)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Ex_GetVersion(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_ADAPTVIE_VERSION:
        {
            pstPQ_GetAdaptiveVersion pPQArgs = (pstPQ_GetAdaptiveVersion)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetAdaptiveVersion(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_UFSC_SETTING:
        {
            pstPQ_LoadUFSCSettings pPQArgs = (pstPQ_LoadUFSCSettings)pArgs;
               psPQInstPri->fpPQ_LoadUFSCSettings(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#if (ENABLE_PQ_BIN_CF)
        case E_PQ_CMD_LOAD_CF_SETTING:
        {
            pstPQ_LoadCFSettings pPQArgs = (pstPQ_LoadCFSettings)pArgs;
            psPQInstPri->fpPQ_LoadCFSettings(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
#endif
              case E_PQ_CMD_LOAD_TABLE_EX:
              {
                  pstPQ_LoadTable_Ex pPQArgs = (pstPQ_LoadTable_Ex)pArgs;
                     psPQInstPri->fpPQ_LoadTable_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16TabIdx,pPQArgs->u16PQIPIdx,pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_TABLE_INDEX_EX:
              {
                  pstPQ_GetTableIndex_Ex pPQArgs = (pstPQ_GetTableIndex_Ex)pArgs;
                     pPQArgs->u16Ret = psPQInstPri->fpPQ_GetTableIndex_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16IPIdx,pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_DEMO_CLONE_WINDOW:
              {
                  pstPQ_Demo_CloneWindow pPQArgs = (pstPQ_Demo_CloneWindow)pArgs;
                     pPQArgs->bRet = psPQInstPri->fpPQ_Demo_CloneWindow(pInstance, pPQArgs->eWindow,pPQArgs->u8Mode);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_GRULE_LV1NUM_EX:
              {
                  pstPQ_GetGRule_LvlNum_Ex pPQArgs = (pstPQ_GetGRule_LvlNum_Ex)pArgs;
                     pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_LvlNum_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_GRULE_LEVELINDEX_EX:
              {
                  pstPQ_GetGRule_LevelIndex_Ex pPQArgs = (pstPQ_GetGRule_LevelIndex_Ex)pArgs;
                     pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_LevelIndex_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->u16GRuleLevelIndex, pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_GRULE_IPINDEX_EX:
              {
                  pstPQ_GetGRule_IPIndex_Ex pPQArgs = (pstPQ_GetGRule_IPIndex_Ex)pArgs;
                     pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_IPIndex_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->u16GRuleIPIndex, pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_GRULE_TABLEINDEX_EX:
              {
                  pstPQ_GetGRule_TableIndex_Ex pPQArgs = (pstPQ_GetGRule_TableIndex_Ex)pArgs;
                     pPQArgs->u16Ret = psPQInstPri->fpPQ_GetGRule_TableIndex_Ex(pInstance,pPQArgs->eWindow,pPQArgs->u16GRuleType,pPQArgs->u16PQ_NRIdx,pPQArgs->u16GRuleIPIndex, pPQArgs->ePQEngineType);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_ENABLE_SCALER_GAMMA:
              {
                  pstPQ_EnableScalerGamma pPQArgs = (pstPQ_EnableScalerGamma)pArgs;
                     psPQInstPri->fpPQ_EnableScalerGamma(pInstance, pPQArgs->eWindow,pPQArgs->bEnable);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_SET_GAMMA_TBl:
              {
                  pstPQ_SetGammaTbl pPQArgs = (pstPQ_SetGammaTbl)pArgs;
                     psPQInstPri->fpPQ_SetGammaTbl(pInstance, pPQArgs->enGammType,pPQArgs->pu8GammTable,pPQArgs->enGammaMappingMode);

                  return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_SET_PQBYPASS:
              {
                  pstPQ_Set_PqBypass pPQArgs = (pstPQ_Set_PqBypass)pArgs;
                    pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set_PqBypass(pInstance, pPQArgs->pstXC_SetWin_Info,pPQArgs->bEnable,pPQArgs->eWindow);

                    return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_GET_PQBYPASS:
              {
                  pstPQ_Get_PqBypass pPQArgs = (pstPQ_Get_PqBypass)pArgs;
                    pPQArgs->bReturnValue = psPQInstPri->fpPQ_Get_PqBypass(pInstance, pPQArgs->eWindow);

                    return UTOPIA_STATUS_SUCCESS;
              }
              case E_PQ_CMD_SET_PQBIN_PATH:
              {
                  pstPQ_SetPQBinPath pPQArgs = (pstPQ_SetPQBinPath)pArgs;
                   psPQInstPri->fpPQ_Set_PqBin_Path(pInstance, pPQArgs->ePqBinPath, pPQArgs->u8size, pPQArgs->b_PQBinFilePath);

                    return UTOPIA_STATUS_SUCCESS;
              }
//drvPQ_cus.c
        case E_PQ_CMD_LOAD_FILMMODE_TABLE:
        {
            pstPQ_LoadFilmModeTable pPQArgs = (pstPQ_LoadFilmModeTable)pArgs;
               psPQInstPri->fpPQ_LoadFilmModeTable(pInstance,pPQArgs->eWindow,pPQArgs->enFilmModeType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_DYNAMIC_CONTRAST_TABLE:
        {
            pstPQ_LoadDynamicContrastTable pPQArgs = (pstPQ_LoadDynamicContrastTable)pArgs;
               psPQInstPri->fpPQ_LoadDynamicContrastTable(pInstance,pPQArgs->eWindow,pPQArgs->enDynamicContrastType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_NR_TABLE:
        {
            pstPQ_LoadNRTable pPQArgs = (pstPQ_LoadNRTable)pArgs;
               psPQInstPri->fpPQ_LoadNRTable(pInstance,pPQArgs->eWindow,pPQArgs->en3DNRType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_MPEG_NR_TABLE:
        {
            pstPQ_LoadMPEGNRTable pPQArgs = (pstPQ_LoadMPEGNRTable)pArgs;
               psPQInstPri->fpPQ_LoadMPEGNRTable(pInstance,pPQArgs->eWindow,pPQArgs->enMPEGNRType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_ULTRACLEAR_TABLE:
        {
            pstPQ_Load_ULTRACLEAR_Table pPQArgs = (pstPQ_Load_ULTRACLEAR_Table)pArgs;
               psPQInstPri->fpPQ_Load_ULTRACLEAR_Table(pInstance,pPQArgs->eWindow,pPQArgs->enCtrlType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_XVYCC_TABLE:
        {
            pstPQ_Load_XVYCC_Table pPQArgs = (pstPQ_Load_XVYCC_Table)pArgs;
               psPQInstPri->fpPQ_Load_XVYCC_Table(pInstance,pPQArgs->eWindow,pPQArgs->enXvyccType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_HDR_TABLE:
        {
            pstPQ_Load_HDR_Table pPQArgs = (pstPQ_Load_HDR_Table)pArgs;
               psPQInstPri->fpPQ_Load_HDR_Table(pInstance,pPQArgs->eWindow,pPQArgs->enHdrMode);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GRULE_GET_SUPPORT_STATUS:
        {
            pstPQ_GRULE_Get_Support_Status pPQArgs = (pstPQ_GRULE_Get_Support_Status)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GRULE_Get_Support_Status(pInstance,pPQArgs->enType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_MEMFORMAT:
        {
            pstPQ_SetMemFormat pPQArgs = (pstPQ_SetMemFormat)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetMemFormat(pInstance,pPQArgs->eWindow,
                                                                                pPQArgs->bMemFmt422,
                                                                                pPQArgs->bFBL,
                                                                                pPQArgs->pu8BitsPerPixel);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_REDUCEBW_ForOSD:
        {
            pstPQ_ReduceBW_ForOSD pPQArgs = (pstPQ_ReduceBW_ForOSD)pArgs;
               psPQInstPri->fpPQ_ReduceBW_ForOSD(pInstance,pPQArgs->eWindow, pPQArgs->bOSD_On);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_LOAD_PTP_TABLE:
        {
            pstPQ_LoadPTPTable pPQArgs = (pstPQ_LoadPTPTable)pArgs;
               psPQInstPri->fpPQ_LoadPTPTable(pInstance,pPQArgs->eWindow, pPQArgs->enPTP_Type);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_INIT_DISPLAY_TYPE:
        {
            pstPQ_Init_DisplayType pPQArgs = (pstPQ_Init_DisplayType)pArgs;
               psPQInstPri->fpPQ_Init_DisplayType(pInstance,pPQArgs->u16DisplayWidth, pPQArgs->enDisplaType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_DISPLAY_TYPE:
        {
            pstPQ_Set_DisplayType pPQArgs = (pstPQ_Set_DisplayType)pArgs;
               psPQInstPri->fpPQ_Set_DisplayType(pInstance,pPQArgs->u16DisplayWidth, pPQArgs->enDisplaType);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_MADI_FROM_OSDBW_GRULE:
        {
            pstPQ_GetMADiFromOSDBWGrule pPQArgs = (pstPQ_GetMADiFromOSDBWGrule)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetMADiFromOSDBWGrule(pInstance,pPQArgs->eWindow,
                                                                                        pPQArgs->pu16TabIdx_MADi,
                                                                                        pPQArgs->pu16TabIdx_MADi_Motion);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_MADI_IN_GENERAL:
        {
            pstPQ_GetMADiInGeneral pPQArgs = (pstPQ_GetMADiInGeneral)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_GetMADiInGeneral(pInstance,pPQArgs->eWindow,
                                                                                        pPQArgs->pu16TabIdx_MADi,
                                                                                        pPQArgs->pu16TabIdx_MADi_Motion);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_CHECK_POINT2POINT_CONDITION:
        {
            pstPQ_Check_PointToPoint_Condition pPQArgs = (pstPQ_Check_PointToPoint_Condition)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Check_PointToPoint_Condition(pInstance,pPQArgs->pstXC_SetWin_Info, pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_POSTCCS_ONOFF:
        {
            pstPQ_SetPostCCSOnOff pPQArgs = (pstPQ_SetPostCCSOnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetPostCCSOnOff(pInstance, pPQArgs->eWindow ,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_ADAPTIVE_TUNING:
        {
               psPQInstPri->fpPQ_AdaptiveTuning(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_FILMMODE_ANYCANDENCE_ENABLE:
        {
            pstPQ_FilmMode_AnyCandence_Enable pPQArgs = (pstPQ_FilmMode_AnyCandence_Enable)pArgs;
               psPQInstPri->fpPQ_FilmMode_AnyCandence_Enable(pInstance,pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SETH264_ONOFF:
        {
            pstPQ_SetH264_OnOff pPQArgs = (pstPQ_SetH264_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetH264_OnOff(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SETG3D_ONOFF:
        {
            pstPQ_SetG3D_OnOff pPQArgs = (pstPQ_SetG3D_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetG3D_OnOff(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SETMVC4KX1K_ONOFF:
        {
            pstPQ_SetMVC4kx1k_OnOff pPQArgs = (pstPQ_SetMVC4kx1k_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetMVC4kx1k_OnOff(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_NETWORKMM_ONOFF:
        {
            pstPQ_SetNetworkMM_OnOff pPQArgs = (pstPQ_SetNetworkMM_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetNetworkMM_OnOff(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SETMM_ONOFF:
        {
            pstPQ_SetMM_OnOff pPQArgs = (pstPQ_SetMM_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetMM_OnOff(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_RMVB_ONOFF:
        {
            pstPQ_SetRmvb_OnOff pPQArgs = (pstPQ_SetRmvb_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetRmvb_OnOff(pInstance, pPQArgs->bEn ,pPQArgs->ePQWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_VIPBYPASS_ONOFF:
        {
            pstPQ_SetVIPBypass_OnOff pPQArgs = (pstPQ_SetVIPBypass_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetVIPBypass_OnOff(pInstance, pPQArgs->bEnable ,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_PEAKING_ONOFF:
        {
            pstPQ_SetPeaking_OnOff pPQArgs = (pstPQ_SetPeaking_OnOff)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetPeaking_OnOff(pInstance, pPQArgs->bEnable ,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_FAKE_OUT_ENABLE:
        {
            pstPQ_SetFakeOutEnable pPQArgs = (pstPQ_SetFakeOutEnable)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_SetFakeOutEnable(pInstance, pPQArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }
//drvPQ_Text.c
        case E_PQ_CMD_GET_SRC_TYPE_NAME:
        {
            pstPQ_GetSrcTypeName pPQArgs = (pstPQ_GetSrcTypeName)pArgs;
               pPQArgs->u8Ret = psPQInstPri->fpPQ_GetSrcTypeName(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_IP_NAME:
        {
            pstPQ_GetIPName pPQArgs = (pstPQ_GetIPName)pArgs;
               pPQArgs->u8Ret = psPQInstPri->fpPQ_GetIPName(pInstance,pPQArgs->u8PQIPIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GET_TABLE_NAME:
        {
            pstPQ_GetTableName pPQArgs = (pstPQ_GetTableName)pArgs;
               pPQArgs->u8Ret = psPQInstPri->fpPQ_GetTableName(pInstance,pPQArgs->u8PQIPIdx,pPQArgs->u8TabIdx);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_1KTRAVELINGENABLE:
        {
            pstPQ_Set1KTravelingEnable pPQArgs = (pstPQ_Set1KTravelingEnable)pArgs;
               pPQArgs->bReturnValue = psPQInstPri->fpPQ_Set1KTravelingEnable(pInstance, pPQArgs->bEn);

            return UTOPIA_STATUS_SUCCESS;
        }
//drvbw.c
        case E_PQ_CMD_BW_LOAD_INIT_TABLE:
        {
            psPQInstPri->fpPQ_BW_LoadInitTable(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_BW_LOAD_TABLE_BY_CONTEXT:
        {
            pstPQ_BW_LoadTableByContext pPQArgs = (pstPQ_BW_LoadTableByContext)pArgs;
               psPQInstPri->fpPQ_BW_LoadTableByContext(pInstance,pPQArgs->eWindow);

            return UTOPIA_STATUS_SUCCESS;
        }
             case E_PQ_CMD_BW_CUSMODE_LOADTABLE:
             {
                 pstPQ_BW_CusMode_LoadTable pPQArgs = (pstPQ_BW_CusMode_LoadTable)pArgs;
                    psPQInstPri->fpPQ_BW_CusMode_LoadTable(pInstance,pPQArgs->u8TabIdx);

                 return UTOPIA_STATUS_SUCCESS;
             }
        case E_PQ_CMD_BW_RESET_TO_DEFAULT:
        {
               psPQInstPri->fpPQ_BW_ResetToDefault(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_GETCONFIG:
        {
            pstPQ_GetConfig pPQArgs = (pstPQ_GetConfig)pArgs;
            pPQArgs->eRet = psPQInstPri->fpPQ_GetConfig(pInstance, pPQArgs->pstPQInitInfo);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PQ_CMD_SET_FRAMEBUFF_MODE:
        {
            pstPQ_SetFrameBuffMode pPQArgs = (pstPQ_SetFrameBuffMode)pArgs;
            psPQInstPri->fpPQ_SET_FrameBuff_Mode(pInstance, pPQArgs->bFrameBuffLess);
            return UTOPIA_STATUS_SUCCESS;
        }
        default:
        {
            printf("Error Command = %td\n", (ptrdiff_t) u32Cmd);
            break;
        }
    }
#endif
    return UTOPIA_STATUS_FAIL;
}

MS_U32 PQClose(void* pInstance)
{
#if 1
    UtopiaInstanceDelete(pInstance);
#endif
    return UTOPIA_STATUS_SUCCESS;
}

#endif
