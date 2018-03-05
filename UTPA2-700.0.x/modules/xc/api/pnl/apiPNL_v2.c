#ifndef _API_XC_PANEL_V2_C_
#define _API_XC_PANEL_V2_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "PNL_private.h"
#include "utopia.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
void *pInstancePNLStr = NULL;
MS_U32 PNLStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stPNL_SetPowerState PNLArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstancePNLStr == NULL)
            {
                if(UtopiaOpen(MODULE_PNL, &pInstancePNLStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen PNL failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            PNLArgs.ePowerState = u32PowerState;
            PNLArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstancePNLStr, E_PNL_CMD_SETPOWERSTATE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain PNL engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=PNLArgs.u32Return;
            break;
        case E_POWER_RESUME:
            if (pInstancePNLStr == NULL)
            {
                if(UtopiaOpen(MODULE_PNL, &pInstancePNLStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen PNL failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            PNLArgs.ePowerState = u32PowerState;
            PNLArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstancePNLStr, E_PNL_CMD_SETPOWERSTATE, (void*)&PNLArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain PNL engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=PNLArgs.u32Return;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif
// ========== Global definition ==========
void* pModulePNL = NULL;

void PNLRegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 u32ResourceStatusCheck[E_PNL_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    UtopiaModuleCreate(MODULE_PNL, 0, &pModulePNL);
    UtopiaModuleRegister(pModulePNL);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModulePNL, (FUtopiaOpen)PNLOpen, (FUtopiaClose)PNLClose, (FUtopiaIOctl)PNLIoctl);

    // 2. deal with resource  , PNL may need 2 resources, one is readonly, the other is XC resource
    void* psResource = NULL;
    void* psResource1 = NULL;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModulePNL, (FUtopiaSTR)PNLStr);
    UtopiaModuleSetSTRPrivate(pModulePNL, sizeof(PNL_REGS_SAVE_AREA));
#endif
    // 3. resource can allocate private for internal use
    MS_U32 u32MaxDeviceNum = _XC_PNL_GET_DEVICE_NUM();
    MS_U32 i = 0;
    for(i = 0 ; i < u32MaxDeviceNum ;i++)
    {
        // need refine
        if(i == 0)
        {
            UtopiaModuleAddResourceStart(pModulePNL, E_PNL_POOL_ID_INTERNAL);
            u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL] = UtopiaResourceCreate("PNL_VARIABLE", sizeof(PNL_RESOURCE_PRIVATE), &psResource);
            UtopiaResourceRegister(pModulePNL, psResource, E_PNL_POOL_ID_INTERNAL);
            UtopiaModuleAddResourceEnd(pModulePNL, E_PNL_POOL_ID_INTERNAL);
        }
        else
        {
            UtopiaModuleAddResourceStart(pModulePNL, E_PNL_POOL_ID_INTERNAL1);
            u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL1] = UtopiaResourceCreate("PNL_VARIABLE1", sizeof(PNL_RESOURCE_PRIVATE), &psResource1);
            UtopiaResourceRegister(pModulePNL, psResource1, E_PNL_POOL_ID_INTERNAL1);
            UtopiaModuleAddResourceEnd(pModulePNL, E_PNL_POOL_ID_INTERNAL1);
        }
    }

    //4. init resource private members here   (aka, global variable)
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pPNLResourcePrivate));

    if (u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pPNLResourcePrivate->bResourceRegistered = TRUE;

        // apiPNL.c
        pPNLResourcePrivate->stapiPNL._bSkipTimingChange = FALSE;
        pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_ALL;
        memset(pPNLResourcePrivate->stapiPNL._pu8GammaTab[0], 0 ,sizeof(MS_U8)*GammaTblSize);
        memset(pPNLResourcePrivate->stapiPNL._pu8GammaTab[1], 0 ,sizeof(MS_U8)*GammaTblSize);
        memset(pPNLResourcePrivate->stapiPNL._pu8GammaTab[2], 0 ,sizeof(MS_U8)*GammaTblSize);
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized = FALSE;
        pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled = FALSE;
        pPNLResourcePrivate->stapiPNL._u16PanelDCLK = 0;
        pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = DISABLE;
        pPNLResourcePrivate->stapiPNL._u16PanelHStart = 0;
        pPNLResourcePrivate->stapiPNL._bForceSetHStartEnable = DISABLE;
        pPNLResourcePrivate->stapiPNL.ePrevPowerState = E_POWER_MECHANICAL;

        // halPNL.c
        pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = 250;
        pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = E_PNL_OUTPUT_CLK_DATA;
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 1; // 00:250mv, 01:350mv, 10:300mv, 11:200mv
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01;
        pPNLResourcePrivate->sthalPNL._usMOD_CALI_OFFSET = 0;
        pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE  = 0x1F; // 250mv:2'h15, 350mv:2'h1F, 300mv:2'h1A, 200mv:2'h10
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = 0;
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 0;
        pPNLResourcePrivate->sthalPNL._u8PnlDiffSwingLevel = 0x63;
        pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = FALSE;

        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType   = 0; // default
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3   = 0x5410;
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7   = 0x5410;
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11  = 0x0000;
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13 = 0x0000;
    }

    if (psResource1 != NULL)
    {
        UtopiaResourceGetPrivate(psResource1,(void**)(&pPNLResourcePrivate));

        if (u32ResourceStatusCheck[E_PNL_POOL_ID_INTERNAL1] == UTOPIA_STATUS_SHM_EXIST)
        {
            // do nothing, since it is already inited
        }
        else
        {
            // Init flow control related variables here. Other global variable should be
            // inited in each of its init function relatively.
            pPNLResourcePrivate->bResourceRegistered = TRUE;

            // apiPNL.c
            pPNLResourcePrivate->stapiPNL._bSkipTimingChange = FALSE;
            pPNLResourcePrivate->stapiPNL._cstPnlInfo.eSupportGammaType = E_APIPNL_GAMMA_ALL;
            pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized = FALSE;
            pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Enabled = FALSE;
            pPNLResourcePrivate->stapiPNL._u16PanelDCLK = 0;
            pPNLResourcePrivate->stapiPNL._bForceSetDCLKEnable = DISABLE;
            pPNLResourcePrivate->stapiPNL._u16PanelHStart = 0;
            pPNLResourcePrivate->stapiPNL._bForceSetHStartEnable = DISABLE;
            pPNLResourcePrivate->stapiPNL.ePrevPowerState = E_POWER_MECHANICAL;

            // halPNL.c
            pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = 250;
            pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = E_PNL_OUTPUT_CLK_DATA;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01;
            pPNLResourcePrivate->sthalPNL._usMOD_CALI_OFFSET = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE  = 0x15;
            pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = 0;
            pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 0;
            pPNLResourcePrivate->sthalPNL._u8PnlDiffSwingLevel = 0x63;
            pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = FALSE;

            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType   = 0; // default
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3   = 0x5410;
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7   = 0x5410;
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11  = 0x0000;
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13 = 0x0000;
        }
    }
}

MS_U32 PNLOpen(void** ppInstance, const void* const pAttribute)
{
    PNL_INSTANCE_PRIVATE *psPNLInstPri = NULL;
    UtopiaInstanceCreate(sizeof(PNL_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psPNLInstPri);

    psPNLInstPri->u32DeviceID             = 0;
    if(pAttribute != NULL)
    {
        MS_U32* pID = (MS_U32*)pAttribute;
        psPNLInstPri->u32DeviceID = *pID;
    }
#ifdef UTOPIA_20
    psPNLInstPri->fpPNL_GetLibVer         = MApi_PNL_GetLibVer_U2;
    psPNLInstPri->fpPNL_GetInfo           = MApi_PNL_GetInfo_U2;
    psPNLInstPri->fpPNL_GetStatus         = MApi_PNL_GetStatus_U2;
    psPNLInstPri->fpPNL_GetStatusEx       = MApi_PNL_GetStatusEx_U2;
    psPNLInstPri->fpPNL_SetDbgLevel       = MApi_PNL_SetDbgLevel_U2;
    psPNLInstPri->fpPNL_IOMapBaseInit     = MApi_PNL_IOMapBaseInit_U2;
    psPNLInstPri->fpPNL_PreInit           = MApi_PNL_PreInit_U2;
    psPNLInstPri->fpPNL_Init              = MApi_PNL_Init_U2;
    psPNLInstPri->fpPNL_Init_Ex           = MApi_PNL_Init_Ex_U2;
    psPNLInstPri->fpPNL_GetConfig         = MApi_PNL_GetConfig_U2;
    psPNLInstPri->fpPNL_SetOutput         = MApi_PNL_SetOutput_U2;
    psPNLInstPri->fpPNL_ChangePanelType   = MApi_PNL_ChangePanelType_U2;
    psPNLInstPri->fpPNL_TCONMAP_DumpTable = MApi_PNL_TCONMAP_DumpTable_U2;
    psPNLInstPri->fpPNL_TCONMAP_Power_Sequence = MApi_PNL_TCONMAP_Power_Sequence_U2;
    psPNLInstPri->fpPNL_TCON_Count_Reset       = MApi_PNL_TCON_Count_Reset_U2;
    psPNLInstPri->fpPNL_TCON_Init              = MApi_PNL_TCON_Init_U2;
    psPNLInstPri->fpPNL_GetDstInfo             = MApi_PNL_GetDstInfo_U2;
    psPNLInstPri->fpPNL_Control_Out_Swing      = MApi_PNL_Control_Out_Swing_U2;
    psPNLInstPri->fpPNL_SetOutputPattern       = MApi_PNL_SetOutputPattern_U2;
    psPNLInstPri->fpPNL_Mod_Calibration_Setting = MApi_Mod_Calibration_Setting_U2;
    psPNLInstPri->fpPNL_Mod_Do_Calibration      = MApi_Mod_Do_Calibration_U2;
    psPNLInstPri->fpPNL_BD_LVDS_Output_Type     = MApi_BD_LVDS_Output_Type_U2;
    psPNLInstPri->fpPNL_SetLPLLTypeExt          = MApi_PNL_SetLPLLTypeExt_U2;
    psPNLInstPri->fpPNL_Init_MISC               = MApi_PNL_Init_MISC_U2;
    psPNLInstPri->fpPNL_Get_MISC_Status         = MApi_PNL_GetMiscStatus_U2;
    psPNLInstPri->fpPNL_MOD_OutputConfig_User   = MApi_PNL_MOD_OutputConfig_User_U2;
    psPNLInstPri->fpPNL_MOD_OutputChannelOrder  = MApi_PNL_MOD_OutputChannelOrder_U2;
    psPNLInstPri->fpPNL_HWLVDSReservedtoLRFlag = MApi_PNL_HWLVDSReservedtoLRFlag_U2;
    psPNLInstPri->fpPNL_MOD_PVDD_Power_Setting = MApi_MOD_PVDD_Power_Setting_U2;
    psPNLInstPri->fpPNL_SetSSC_En              = MApi_PNL_SetSSC_En_U2;
    psPNLInstPri->fpPNL_SetSSC_Fmodulation     = MApi_PNL_SetSSC_Fmodulation_U2;
    psPNLInstPri->fpPNL_SetSSC_Rdeviation      = MApi_PNL_SetSSC_Rdeviation_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_En           = MApi_PNL_SetOSDSSC_En_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_Fmodulation  = MApi_PNL_SetOSDSSC_Fmodulation_U2;
    psPNLInstPri->fpPNL_SetOSDSSC_Rdeviation   = MApi_PNL_SetOSDSSC_Rdeviation_U2;
    psPNLInstPri->fpPNL_SkipTimingChange       = MApi_PNL_SkipTimingChange_U2;
    psPNLInstPri->fpPNL_PreSetModeOn           = MApi_PNL_PreSetModeOn_U2;
    psPNLInstPri->fpPNL_OverDriver_Init        = MApi_PNL_OverDriver_Init_U2;
    psPNLInstPri->fpPNL_OverDriver_Enable      = MApi_PNL_OverDriver_Enable_U2;
    psPNLInstPri->fpPNL_Get_LPLL_Type          = MApi_Pnl_Get_LPLL_Type_U2;
    psPNLInstPri->fpPNL_Get_TCON_Capability    = MApi_PNL_Get_TCON_Capability_U2;
    psPNLInstPri->fpPNL_SetPairSwap            = MApi_PNL_SetPairSwap_U2;
    psPNLInstPri->fpPNL_SetExt_LPLL_Type       = MApi_PNL_SetExt_LPLL_Type_U2;
    psPNLInstPri->fpPNL_CalExtLPLLSETbyDClk    = MApi_PNL_CalExtLPLLSETbyDClk_U2;
    psPNLInstPri->fpPNL_SetDiffSwingLevel      = MApi_PNL_SetDiffSwingLevel_U2;
    psPNLInstPri->fpPNL_SetPowerState          = MApi_PNL_SetPowerState_U2;
    psPNLInstPri->fpPNL_ForceSetPanelDCLK      = MApi_PNL_ForceSetPanelDCLK_U2;
    psPNLInstPri->fpPNL_ForceSetPanelHStart    = MApi_PNL_ForceSetPanelHStart_U2;
    psPNLInstPri->fpPNL_EnableInternalTermination = MApi_PNL_EnableInternalTermination_U2;
    psPNLInstPri->fpPNL_OutputDeviceHandshake     = MApi_PNL_OutputDeviceHandshake_U2;
    psPNLInstPri->fpPNL_OutputDeviceOCHandshake   = MApi_PNL_OutputDeviceOCHandshake_U2;
    psPNLInstPri->fpPNL_SetOutputInterlaceTiming  = MApi_PNL_SetOutputInterlaceTiming_U2;
    psPNLInstPri->fpPNL_GetOutputInterlaceTiming  = MApi_PNL_GetOutputInterlaceTiming_U2;
    psPNLInstPri->fpPNL_GetPanelData              = MApi_PNL_GetPanelData_U2;
    psPNLInstPri->fpPNL_DumpPanelData     = MApi_PNL_DumpPanelData_U2;
    psPNLInstPri->fpPNL_SetSSC            = MApi_PNL_SetSSC_U2;
    psPNLInstPri->fpPNL_GetPanelOnTiming  = MApi_PNL_GetPanelOnTiming_U2;
    psPNLInstPri->fpPNL_GetPanelOffTiming = MApi_PNL_GetPanelOffTiming_U2;
    psPNLInstPri->fpPNL_GetPanelDimCtrl   = MApi_PNL_GetPanelDimCtrl_U2;
    psPNLInstPri->fpPNL_GetAllGammaTbl    = MApi_PNL_GetAllGammaTbl_U2;
    psPNLInstPri->fpPNL_EnablePanel       = MApi_PNL_EnablePanel_U2;
    psPNLInstPri->fpPNL_SetGammaTbl       = MApi_PNL_SetGammaTbl_U2;
    psPNLInstPri->fpPNL_GetGammaTbl       = MApi_PNL_GetGammaTbl_U2;
    psPNLInstPri->fpPNL_SetGammaValue     = MApi_PNL_SetGammaValue_U2;
    psPNLInstPri->fpPNL_GetLPLLMode       = MApi_PNL_GetLPLLMode_U2;
    psPNLInstPri->fpPNL_GetDefVFreq       = MApi_PNL_GetDefVFreq_U2;
    psPNLInstPri->fpPNL_GetPanelName      = MApi_PNL_GetName_U2;
    psPNLInstPri->fpPNL_GetHStart         = MAPI_PNL_GetPNLHstart_U2;
    psPNLInstPri->fpPNL_GetVStart         = MAPI_PNL_GetPNLVstart_U2;
    psPNLInstPri->fpPNL_GetWidth          = MAPI_PNL_GetPNLWidth_U2;
    psPNLInstPri->fpPNL_GetHeight         = MAPI_PNL_GetPNLHeight_U2;
    psPNLInstPri->fpPNL_GetHtotal         = MAPI_PNL_GetPNLHtotal_U2;
    psPNLInstPri->fpPNL_GetVtotal         = MAPI_PNL_GetPNLVtotal_U2;
    psPNLInstPri->fpPNL_GetHsyncwidth     = MAPI_PNL_GetPNLHsyncWidth_U2;
    psPNLInstPri->fpPNL_GetHsyncBackPorch = MAPI_PNL_GetPNLHsyncBackPorch_U2;
    psPNLInstPri->fpPNL_GetVsyncBackPorch = MAPI_PNL_GetPNLVsyncBackPorch_U2;
    psPNLInstPri->fpPNL_GetLPLLType       = MApi_PNL_GetLPLLType_U2;
    psPNLInstPri->fpPNL_GetARC            = MApi_PNL_GetARC_U2;
    psPNLInstPri->fpPNL_GetMinSET         = MApi_PNL_GetMinSET_U2;
    psPNLInstPri->fpPNL_GetMaxSET         = MApi_PNL_GetMaxSET_U2;
    psPNLInstPri->fpPNL_GetOutTimingMode  = MApi_PNL_GetOutTimingMode_U2;
    psPNLInstPri->fpPNL_CheckVBY1HandshakeStatus  = MApi_PNL_Check_VBY1_Handshake_Status_U2;
    psPNLInstPri->fpPNL_SetVideoHWTraining          = MApi_PNL_SetVideoHWTraining_U2;
    psPNLInstPri->fpPNL_SetOSDHWTraining            = MApi_PNL_SetOSDHWTraining_U2;
    psPNLInstPri->fpPNL_GetVideoHWTraining_Status   = MApi_PNL_GetVideoHWTraining_Status_U2;
    psPNLInstPri->fpPNL_GetOSDHWTraining_Status     = MApi_PNL_GetOSDHWTraining_Status_U2;
    psPNLInstPri->fpPNL_Setting                     = MApi_PNL_Setting_U2;
#endif
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 PNLIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
#ifdef UTOPIA_20
    void* pModule = NULL;
    PNL_INSTANCE_PRIVATE* psPNLInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psPNLInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);
    pu32PNLInst_private = pInstance;
    switch(u32Cmd)
    {
        case E_PNL_CMD_GETLIBVER:
        {
            pstPNL_GetLibVer pPNLArgs = (pstPNL_GetLibVer)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_GetLibVer(pInstance, pPNLArgs->ppVersion);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETINFO:
        {
            pstPNL_GetInfo pPNLArgs = (pstPNL_GetInfo)pArgs;
            pPNLArgs->pApiInfo = psPNLInstPri->fpPNL_GetInfo(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETSTATUS:
        {
            pstPNL_GetStatus pPNLArgs = (pstPNL_GetStatus)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetStatus(pInstance, pPNLArgs->pPnlStatus);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETSTATUSEX:
        {
            pstPNL_GetStatusEx pPNLArgs = (pstPNL_GetStatusEx)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetStatusEx(pInstance, pPNLArgs->pPnlExtStatus);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETDBGLEVEL:
        {
            pstPNL_SetDbgLevel pPNLArgs = (pstPNL_SetDbgLevel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetDbgLevel(pInstance, pPNLArgs->u16DbgSwitch);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_IOMAPBASEINIT:
        {
            pstPNL_IOMapBaseInit pPNLArgs = (pstPNL_IOMapBaseInit)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_IOMapBaseInit(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_PREINIT:
        {
            pstPNL_PreInit pPNLArgs = (pstPNL_PreInit)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_PreInit(pInstance, pPNLArgs->eInitParam);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT:
        {
            pstPNL_Init pPNLArgs = (pstPNL_Init)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Init(pInstance, pPNLArgs->pSelPanelType);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT_EX:
        {
            pstPNL_Init_Ex pPNLArgs = (pstPNL_Init_Ex)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Init_Ex(pInstance, pPNLArgs->pSelPanelType, pPNLArgs->LIBVER);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_CONFIG:
        {
            pstPNL_GetConfig pPNLArgs = (pstPNL_GetConfig) pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_GetConfig(pInstance,pPNLArgs->pSelPanelType);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOUTPUT:
        {
            pstPNL_SetOutput pPNLArgs = (pstPNL_SetOutput)pArgs;
            psPNLInstPri->fpPNL_SetOutput(pInstance, pPNLArgs->eOutputMode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CHANGEPANELTYPE:
        {
            pstPNL_ChangePanelType pPNLArgs = (pstPNL_ChangePanelType)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_ChangePanelType(pInstance, pPNLArgs->pSelPanelType);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCONMAP_DUMPTABLE:
        {
            pstPNL_TCONMAP_DumpTable pPNLArgs = (pstPNL_TCONMAP_DumpTable)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_TCONMAP_DumpTable(pInstance, pPNLArgs->pTCONTable, pPNLArgs->u8Tcontype);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCONMAP_POWER_SEQUENCE:
        {
            pstPNL_TCONMAP_Power_Sequence pPNLArgs = (pstPNL_TCONMAP_Power_Sequence)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_TCONMAP_Power_Sequence(pInstance, pPNLArgs->pTCONTable, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCON_COUNT_RESET:
        {
            pstPNL_TCON_Count_Reset pPNLArgs = (pstPNL_TCON_Count_Reset)pArgs;
            psPNLInstPri->fpPNL_TCON_Count_Reset(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_TCON_INIT:
        {
            psPNLInstPri->fpPNL_TCON_Init(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETDSTINFO:
        {
            pstPNL_GetDstInfo pPNLArgs = (pstPNL_GetDstInfo)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetDstInfo(pInstance, pPNLArgs->pDstInfo, pPNLArgs->u32SizeofDstInfo);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CONTROL_OUT_SWING:
        {
            pstPNL_Control_Out_Swing pPNLArgs = (pstPNL_Control_Out_Swing)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Control_Out_Swing(pInstance, pPNLArgs->u16Swing_Level);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_OUTPUT_PATTERN:
        {
            pstPNL_SetOutputPattern pPNLArgs = (pstPNL_SetOutputPattern)pArgs;
            psPNLInstPri->fpPNL_SetOutputPattern(pInstance, pPNLArgs->bEnable, pPNLArgs->u16Red, pPNLArgs->u16Green, pPNLArgs->u16Blue);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_CALIBRATION_SETTING:
        {
            pstPNL_Mod_Calibration_Setting pPNLArgs = (pstPNL_Mod_Calibration_Setting)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Mod_Calibration_Setting(pInstance, pPNLArgs->pstModCaliInfo);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_DO_CALIBRATION:
        {
            pstPNL_Mod_Do_Calibration pPNLArgs = (pstPNL_Mod_Do_Calibration)pArgs;
            pPNLArgs->bReturn= psPNLInstPri->fpPNL_Mod_Do_Calibration(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_BD_LVDS_OUTPUT_TYPE:
        {
            pstPNL_BD_LVDS_Output_Type pPNLArgs = (pstPNL_BD_LVDS_Output_Type)pArgs;
            psPNLInstPri->fpPNL_BD_LVDS_Output_Type(pInstance, pPNLArgs->Type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETLPLLTYPEEXT:
        {
            pstPNL_SetLPLLTypeExt pPNLArgs = (pstPNL_SetLPLLTypeExt)pArgs;
            psPNLInstPri->fpPNL_SetLPLLTypeExt(pInstance, pPNLArgs->eLPLL_TypeExt);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_INIT_MISC:
        {
            pstPNL_Init_MISC pPNLArgs = (pstPNL_Init_MISC)pArgs;
            psPNLInstPri->fpPNL_Init_MISC(pInstance, pPNLArgs->ePNL_MISC);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_MISC_STATUS:
        {
            pstPNL_Get_MISC_Status pPNLArgs = (pstPNL_Get_MISC_Status)pArgs;
            pPNLArgs->u32Status = psPNLInstPri->fpPNL_Get_MISC_Status(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_OUTPUTCONFIG_USER:
        {
            pstPNL_MOD_OutputConfig_User pPNLArgs = (pstPNL_MOD_OutputConfig_User)pArgs;
            psPNLInstPri->fpPNL_MOD_OutputConfig_User(pInstance, pPNLArgs->u32OutputCFG0_7, pPNLArgs->u32OutputCFG8_15, pPNLArgs->u32OutputCFG16_21);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_OUTPUT_CHANNEL_ORDER:
        {
            pstPNL_MOD_OutputChannelOrder pPNLArgs = (pstPNL_MOD_OutputChannelOrder)pArgs;
            psPNLInstPri->fpPNL_MOD_OutputChannelOrder(pInstance,
                                                       pPNLArgs->u8OutputOrderType,
                                                       pPNLArgs->u16OutputOrder0_3,
                                                       pPNLArgs->u16OutputOrder4_7,
                                                       pPNLArgs->u16OutputOrder8_11,
                                                       pPNLArgs->u16OutputOrder12_13);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG:
        {
            pstPNL_HWLVDSReservedtoLRFlag pPNLArgs = (pstPNL_HWLVDSReservedtoLRFlag)pArgs;
            psPNLInstPri->fpPNL_HWLVDSReservedtoLRFlag(pInstance, pPNLArgs->lvdsresinfo);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_MOD_PVDD_POWER_SETTING:
        {
            pstPNL_MOD_PVDD_Power_Setting pPNLArgs = (pstPNL_MOD_PVDD_Power_Setting)pArgs;
            psPNLInstPri->fpPNL_MOD_PVDD_Power_Setting(pInstance, pPNLArgs->bIs2p5);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_EN:
        {
            pstPNL_SetSSC_En pPNLArgs = (pstPNL_SetSSC_En)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_En(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_FMODULATION:
        {
            pstPNL_SetSSC_Fmodulation pPNLArgs = (pstPNL_SetSSC_Fmodulation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_Fmodulation(pInstance, pPNLArgs->u16Fmodulation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC_RDEVIATION:
        {
            pstPNL_SetSSC_Rdeviation pPNLArgs = (pstPNL_SetSSC_Rdeviation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetSSC_Rdeviation(pInstance, pPNLArgs->u16Rdeviation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_EN:
        {
            pstPNL_SetOSDSSC_En pPNLArgs = (pstPNL_SetOSDSSC_En)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_En(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_FMODULATION:
        {
            pstPNL_SetOSDSSC_Fmodulation pPNLArgs = (pstPNL_SetOSDSSC_Fmodulation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_Fmodulation(pInstance, pPNLArgs->u16Fmodulation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOSDSSC_RDEVIATION:
        {
            pstPNL_SetOSDSSC_Rdeviation pPNLArgs = (pstPNL_SetOSDSSC_Rdeviation)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOSDSSC_Rdeviation(pInstance, pPNLArgs->u16Rdeviation);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SKIPTIMINGCHANGE:
        {
            pstPNL_SkipTimingChange pPNLArgs = (pstPNL_SkipTimingChange)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_SkipTimingChange(pInstance, pPNLArgs->bFlag);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_PRE_SET_MODE_ON:
        {
            pstPNL_PreSetModeOn pPNLArgs = (pstPNL_PreSetModeOn)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_PreSetModeOn(pInstance, pPNLArgs->bSetMode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OVERDRIVER_INIT:
        {
            pstPNL_OverDriver_Init pPNLArgs = (pstPNL_OverDriver_Init)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_OverDriver_Init(pInstance, pPNLArgs->pPNL_ODInitData, pPNLArgs->u32ODInitDataLen);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OVERDRIVER_ENABLE:
        {
            pstPNL_OverDriver_Enable pPNLArgs = (pstPNL_OverDriver_Enable)pArgs;
            pPNLArgs->eResult = psPNLInstPri->fpPNL_OverDriver_Enable(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_LPLL_TYPE:
        {
            pstPNL_Get_LPLL_Type pPNLArgs = (pstPNL_Get_LPLL_Type)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_Get_LPLL_Type(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_TCON_CAPABILITY:
        {
            pstPNL_Get_TCON_Capability pPNLArgs = (pstPNL_Get_TCON_Capability)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_Get_TCON_Capability(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETPAIRSWAP:
        {
            pstPNL_SetPairSwap pPNLArgs = (pstPNL_SetPairSwap)pArgs;
            psPNLInstPri->fpPNL_SetPairSwap(pInstance, pPNLArgs->u32Polarity);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_EXT_LPLL_TYPE:
        {
            pstPNL_SetExt_LPLL_Type pPNLArgs = (pstPNL_SetExt_LPLL_Type)pArgs;
            psPNLInstPri->fpPNL_SetExt_LPLL_Type(pInstance, pPNLArgs->u16Ext_lpll_type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CALEXTLPLLSETBYDCLK:
        {
            pstPNL_CalExtLPLLSETbyDClk pPNLArgs = (pstPNL_CalExtLPLLSETbyDClk)pArgs;
            psPNLInstPri->fpPNL_CalExtLPLLSETbyDClk(pInstance, pPNLArgs->ldHz);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETDIFFSWINGLEVEL:
        {
            pstPNL_SetDiffSwingLevel pPNLArgs = (pstPNL_SetDiffSwingLevel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetDiffSwingLevel(pInstance, pPNLArgs->u8Swing_Level);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETPOWERSTATE:
        {
            pstPNL_SetPowerState pPNLArgs = (pstPNL_SetPowerState)pArgs;
            pPNLArgs->u32Return = psPNLInstPri->fpPNL_SetPowerState(pInstance, pPNLArgs->ePowerState);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_FORCESETPANELDCLK:
        {
            pstPNL_ForceSetPanelDCLK pPNLArgs = (pstPNL_ForceSetPanelDCLK)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_ForceSetPanelDCLK(pInstance, pPNLArgs->u16PanelDCLK, pPNLArgs->bSetDCLKEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_FORCESETPANELHSTART:
        {
            pstPNL_ForceSetPanelHStart pPNLArgs = (pstPNL_ForceSetPanelHStart)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_ForceSetPanelHStart(pInstance, pPNLArgs->u16PanelHStart, pPNLArgs->bSetHStartEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_ENABLEINTERNALTERMINATION:
        {
            pstPNL_EnableInternalTermination pPNLArgs = (pstPNL_EnableInternalTermination)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_EnableInternalTermination(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OUTPUTDEVICEHANDSHAKE:
        {
            pstPNL_OutputDeviceHandshake pPNLArgs = (pstPNL_OutputDeviceHandshake)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_OutputDeviceHandshake(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_OUTPUTDEVICEOCHANDSHAKE:
        {
            pstPNL_OutputDeviceOCHandshake pPNLArgs = (pstPNL_OutputDeviceOCHandshake)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_OutputDeviceOCHandshake(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETOUTPUTINTERLACETIMING:
        {
            pstPNL_SetOutputInterlaceTiming pPNLArgs = (pstPNL_SetOutputInterlaceTiming)pArgs;
            pPNLArgs->eReturn = psPNLInstPri->fpPNL_SetOutputInterlaceTiming(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETOUTPUTINTERLACETIMING:
        {
            pstPNL_GetOutputInterlaceTiming pPNLArgs = (pstPNL_GetOutputInterlaceTiming)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetOutputInterlaceTiming(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELDATA:
        {
            pstPNL_GetPanelData pPNLArgs = (pstPNL_GetPanelData)pArgs;
            psPNLInstPri->fpPNL_GetPanelData(pInstance, pPNLArgs->pstPNLData);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_DUMPPANELDATA:
        {
            psPNLInstPri->fpPNL_DumpPanelData(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETSSC:
        {
            pstPNL_SetSSC pPNLArgs = (pstPNL_SetSSC)pArgs;
            psPNLInstPri->fpPNL_SetSSC(pInstance, pPNLArgs->u16Fmodulation, pPNLArgs->u16Rdeviation, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELONTIMING:
        {
            pstPNL_GetPanelOnTiming pPNLArgs = (pstPNL_GetPanelOnTiming)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetPanelOnTiming(pInstance, pPNLArgs->seq);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELOFFTIMING:
        {
            pstPNL_GetPanelOffTiming pPNLArgs = (pstPNL_GetPanelOffTiming)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetPanelOffTiming(pInstance, pPNLArgs->seq);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELDIMCTRL:
        {
            pstPNL_GetPanelDimCtrl pPNLArgs = (pstPNL_GetPanelDimCtrl)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetPanelDimCtrl(pInstance, pPNLArgs->dim_type);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETALLGAMMATBL:
        {
            pstPNL_GetAllGammaTbl pPNLArgs = (pstPNL_GetAllGammaTbl)pArgs;
            pPNLArgs->ppu8Return = psPNLInstPri->fpPNL_GetAllGammaTbl(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_ENABLEPANEL:
        {
            pstPNL_EnablePanel pPNLArgs = (pstPNL_EnablePanel)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_EnablePanel(pInstance, pPNLArgs->bPanelOn);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETGAMMATBL:
        {
            pstPNL_SetGammaTbl pPNLArgs = (pstPNL_SetGammaTbl)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetGammaTbl(pInstance, pPNLArgs->eGammaType, pPNLArgs->pu8GammaTab, pPNLArgs->Gamma_Map_Mode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETGAMMATBL:
        {
            pstPNL_GetGammaTbl pPNLArgs = (pstPNL_GetGammaTbl)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_GetGammaTbl(pInstance, pPNLArgs->eGammaType, pPNLArgs->pu8GammaTab, pPNLArgs->Gamma_Map_Mode);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SETGAMMAVALUE:
        {
            pstPNL_SetGammaValue pPNLArgs = (pstPNL_SetGammaValue)pArgs;
            pPNLArgs->bReturn = psPNLInstPri->fpPNL_SetGammaValue(pInstance, pPNLArgs->u8Channel, pPNLArgs->u16Offset, pPNLArgs->u16GammaValue);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETLPLLMODE:
        {
            pstPNL_GetLPLLMode pPNLArgs = (pstPNL_GetLPLLMode)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetLPLLMode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETDEFVFREQ:
        {
            pstPNL_GetDefVFreq pPNLArgs = (pstPNL_GetDefVFreq)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetDefVFreq(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETPANELNAME:
        {
            pstPNL_GetName pPNLArgs = (pstPNL_GetName)pArgs;
            pPNLArgs->pPNLName = psPNLInstPri->fpPNL_GetPanelName(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETHSTART:
        {
            pstPNL_GetHstart pPNLArgs = (pstPNL_GetHstart)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetHStart(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETVSTART:
        {
            pstPNL_GetVstart pPNLArgs = (pstPNL_GetVstart)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetVStart(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETWIDTH:
        {
            pstPNL_GetWidth pPNLArgs = (pstPNL_GetWidth)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetWidth(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETHEIGHT:
        {
            pstPNL_GetWidth pPNLArgs = (pstPNL_GetWidth)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetHeight(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETHTOTAL:
        {
            pstPNL_GetHtotal pPNLArgs = (pstPNL_GetHtotal)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetHtotal(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETVTOTAL:
        {
            pstPNL_GetVtotal pPNLArgs = (pstPNL_GetVtotal)pArgs;
            pPNLArgs->u16Return = psPNLInstPri->fpPNL_GetVtotal(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETHSYNCWIDTH:
        {
            pstPNL_GetHsyncWidth pPNLArgs = (pstPNL_GetHsyncWidth)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetHsyncwidth(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETHSYNCBACKPORCH:
        {
            pstPNL_GetHsyncBackPorch pPNLArgs = (pstPNL_GetHsyncBackPorch)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetHsyncBackPorch(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETVSYNCBACKPORCH:
        {
            pstPNL_GetVsyncBackPorch pPNLArgs = (pstPNL_GetVsyncBackPorch)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetVsyncBackPorch(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETLPLLTYPE:
        {
            pstPNL_GetLPLLType pPNLArgs = (pstPNL_GetLPLLType)pArgs;
            pPNLArgs->u8Return = psPNLInstPri->fpPNL_GetLPLLType(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETARC:
        {
            pstPNL_GetARC pPNLArgs = (pstPNL_GetARC)pArgs;
            pPNLArgs->ePanelAspectRatio= psPNLInstPri->fpPNL_GetARC(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETMINSET:
        {
            pstPNL_GetMinSET pPNLArgs = (pstPNL_GetMinSET)pArgs;
            pPNLArgs->u32Return = psPNLInstPri->fpPNL_GetMinSET(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETMAXSET:
        {
            pstPNL_GetMaxSET pPNLArgs = (pstPNL_GetMaxSET)pArgs;
            pPNLArgs->u32Return = psPNLInstPri->fpPNL_GetMaxSET(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GETOUTTIMINGMODE:
        {
            pstPNL_GetOutTimingMode pPNLArgs = (pstPNL_GetOutTimingMode)pArgs;
            pPNLArgs->ePanelOutTimingMode= psPNLInstPri->fpPNL_GetOutTimingMode(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_CHECKVBY1HANDSHAKESTATUS:
        {
            pstPNL_CheckVBY1HandshakeStatus pPNLArgs = (pstPNL_CheckVBY1HandshakeStatus)pArgs;
            pPNLArgs->bResult = psPNLInstPri->fpPNL_CheckVBY1HandshakeStatus(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_VIDEOHWTRAINING_MODE:
        {
            pstPNL_MOD_Video_HWTrainingMode pPNLArgs = (pstPNL_MOD_Video_HWTrainingMode)pArgs;
            psPNLInstPri->fpPNL_SetVideoHWTraining(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_SET_OSDHWTRAINING_MODE:
        {
            pstPNL_MOD_OSD_HWTrainingMode pPNLArgs = (pstPNL_MOD_OSD_HWTrainingMode)pArgs;
            psPNLInstPri->fpPNL_SetOSDHWTraining(pInstance, pPNLArgs->bEnable);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_VIDEOHWTRAINING_MODE:
        {
            pstPNL_GetVideoHWTraining_status pPNLArgs = (pstPNL_GetVideoHWTraining_status)pArgs;
            pPNLArgs->bStatus= psPNLInstPri->fpPNL_GetVideoHWTraining_Status(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }

        case E_PNL_CMD_GET_OSDHWTRAINING_MODE:
        {
            pstPNL_GetOSDHWTraining_status pPNLArgs = (pstPNL_GetOSDHWTraining_status)pArgs;
            pPNLArgs->bStatus = psPNLInstPri->fpPNL_GetOSDHWTraining_Status(pInstance);

            return UTOPIA_STATUS_SUCCESS;
        }
        case E_PNL_CMD_Setting:
        {
            pstPNL_Setting pPNLArgs = (pstPNL_Setting)pArgs;
            pPNLArgs->eReturnValue = psPNLInstPri->fpPNL_Setting(pInstance, pPNLArgs->u32Cmd, pPNLArgs->pCmdArgs, pPNLArgs->u32CmdArgsSize);

            return UTOPIA_STATUS_SUCCESS;
        }
        default:
            printf("Error Command = %td\n", (ptrdiff_t)u32Cmd);
            break;
    }

    return UTOPIA_STATUS_FAIL;
#else
    return UTOPIA_STATUS_SUCCESS;
#endif
}

MS_U32 PNLClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

#endif
