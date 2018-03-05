#include "MsTypes.h"
#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif

#include "utopia_dapi.h"
#include "apiXC_Ace.h"
#include "drvACE.h"
#include "ACE_private.h"
#include "apiXC_Ace_v2.h"
#include "utopia.h"

#define MWE_TIMER           60
#define MWE_H_STEP          6
#define MWE_MOVE_RIGHT      1
#define MWE_MOVE_LEFT       0

// ========== Global definition ==========
void* pModuleACE = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
void* pInstanceACEStr = NULL;
MS_U32 ACEStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stACE_Set_PowerState ACEArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstanceACEStr == NULL)
            {
                if(UtopiaOpen(MODULE_ACE, &pInstanceACEStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen ACE failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            ACEArgs.enPowerState = u32PowerState;
            ACEArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceACEStr, E_ACE_CMD_SET_POWERSTATE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain ACE engine fail\n");
            }
            u32Return=ACEArgs.u32Return;
            break;
        case E_POWER_RESUME:
            if (pInstanceACEStr == NULL)
            {
                if(UtopiaOpen(MODULE_ACE, &pInstanceACEStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen ACE failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            ACEArgs.enPowerState = u32PowerState;
            ACEArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceACEStr, E_ACE_CMD_SET_POWERSTATE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain ACE engine fail\n");
            }
            u32Return=ACEArgs.u32Return;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif

void ACERegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 u32ResourceStatusCheck[E_ACE_POOL_ID_MAX] = {0};
    // 1. deal with module
    UtopiaModuleCreate(MODULE_ACE, 0, &pModuleACE);
    UtopiaModuleRegister(pModuleACE);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleACE, (FUtopiaOpen)ACEOpen, (FUtopiaClose)ACEClose, (FUtopiaIOctl)ACEIoctl);

    // 2. deal with resource  , ACE
    void* psResource = NULL;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleACE, (FUtopiaSTR)ACEStr);
    UtopiaModuleSetSTRPrivate(pModuleACE, sizeof(ACE_REGS_SAVE_AREA));
#endif
    // 3. resource can allocate private for internal use
    MS_U32 u32MaxDeviceNum = _XC_ACE_GET_DEVICE_NUM();
    MS_U32 i = 0;
    for(i = 0 ; i < u32MaxDeviceNum ; i++)
    {
        //Need refine
        if(i == 0)
        {
            UtopiaModuleAddResourceStart(pModuleACE, E_ACE_POOL_ID_INTERNAL);
            u32ResourceStatusCheck[E_ACE_POOL_ID_INTERNAL] = UtopiaResourceCreate("ACE0", sizeof(ACE_RESOURCE_PRIVATE), &psResource);
            UtopiaResourceRegister(pModuleACE, psResource, E_ACE_POOL_ID_INTERNAL);
            UtopiaModuleAddResourceEnd(pModuleACE, E_ACE_POOL_ID_INTERNAL);
        }
        else
        {
            UtopiaModuleAddResourceStart(pModuleACE, E_ACE_POOL_ID_INTERNAL_1);
            u32ResourceStatusCheck[E_ACE_POOL_ID_INTERNAL_1] = UtopiaResourceCreate("ACE1", sizeof(ACE_RESOURCE_PRIVATE), &psResource);
            UtopiaResourceRegister(pModuleACE, psResource, E_ACE_POOL_ID_INTERNAL_1);
        	UtopiaModuleAddResourceEnd(pModuleACE, E_ACE_POOL_ID_INTERNAL_1);
        }
    }


    //4. init resource private members here   (aka, global variable)
    ACE_RESOURCE_PRIVATE* pACEResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pACEResourcePrivate));

    // shall we init first?
    if (u32ResourceStatusCheck[E_ACE_POOL_ID_INTERNAL] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pACEResourcePrivate->bResourceRegistered = TRUE;
        pACEResourcePrivate->stapiACE.g_bACEModuleInited[0] = FALSE;
        pACEResourcePrivate->stapiACE.g_bACEModuleInited[1] = FALSE;

        // apiPNL.c
        pACEResourcePrivate->stapiACE._MWEFunc = E_XC_ACE_MWE_MODE_OFF;
        pACEResourcePrivate->stapiACE._MWETimer = 0;
        pACEResourcePrivate->stapiACE._MWEFlag = MWE_MOVE_LEFT;
        //pACEResourcePrivate->stapiACE.bMWEStatus = FALSE;
        //pACEResourcePrivate->stapiACE.gMWERect;
        //pACEResourcePrivate->stapiACE.g_XC_ACE_InitData = DISABLE;
        //pACEResourcePrivate->stapiACE._bMWESkipDriverSetting = FALSE;
        //pACEResourcePrivate->stapiACE._enMirrorMode = E_XC_ACE_MWE_MIRROR_NORMAL;
        //pACEResourcePrivate->stapiACE.g_u8MotionStableCnt = 0;
        //pACEResourcePrivate->stapiACE.g_u8LumaStableCnt = 0;
        //pACEResourcePrivate->stapiACE.g_u8FlickStableCnt = 0;
    }



}

MS_U32 ACEOpen(void** ppInstance, const void* const pAttribute)
{
    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
    UtopiaInstanceCreate(sizeof(ACE_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psACEInstPri);
    if(pAttribute != NULL)
    {
        MS_U32* u32Attr = (MS_U32*)pAttribute;
        psACEInstPri->u32DeviceID = *u32Attr;
    }
    else
    {
        psACEInstPri->u32DeviceID = 0;
    }
    psACEInstPri->fpACE_Init = MApi_XC_ACE_Init_U2;
    psACEInstPri->fpACE_Exit = MApi_XC_ACE_Exit_U2;
    psACEInstPri->fpACE_DMS = MApi_XC_ACE_DMS_U2;
    psACEInstPri->fpACE_SetPCYUV2RGB = MApi_XC_ACE_SetPCYUV2RGB_U2;
    psACEInstPri->fpACE_SelectYUVtoRGBMatrix = MApi_XC_ACE_SelectYUVtoRGBMatrix_U2;
    psACEInstPri->fpACE_SetColorCorrectionTable = MApi_XC_ACE_SetColorCorrectionTable_U2;
    psACEInstPri->fpACE_SetPCsRGBTable = MApi_XC_ACE_SetPCsRGBTable_U2;
    psACEInstPri->fpACE_GetColorMatrix = MApi_XC_ACE_GetColorMatrix_U2;
    psACEInstPri->fpACE_SetColorMatrix = MApi_XC_ACE_SetColorMatrix_U2;
    psACEInstPri->fpACE_SetBypassColorMatrix = MApi_XC_ACE_SetBypassColorMatrix_U2;
    psACEInstPri->fpACE_SetIHC = MApi_XC_ACE_SetIHC_U2;
    psACEInstPri->fpACE_SetICC = MApi_XC_ACE_SetICC_U2;
    psACEInstPri->fpACE_SetIBC = MApi_XC_ACE_SetIBC_U2;
    psACEInstPri->fpACE_PatchDTGColorChecker = MApi_XC_ACE_PatchDTGColorChecker_U2;
    psACEInstPri->fpACE_GetACEInfo = MApi_XC_ACE_GetACEInfo_U2;
    psACEInstPri->fpACE_GetACEInfoEx = MApi_XC_ACE_GetACEInfoEx_U2;
    psACEInstPri->fpACE_PicSetContrast = MApi_XC_ACE_PicSetContrast_U2;
    psACEInstPri->fpACE_PicSetBrightness = MApi_XC_ACE_PicSetBrightness_U2;
    psACEInstPri->fpACE_PicSetBrightnessPrecise = MApi_XC_ACE_PicSetBrightnessPrecise_U2;
    psACEInstPri->fpACE_PicSetPreYOffset = MApi_XC_ACE_PicSetPreYOffset_U2;
    psACEInstPri->fpACE_PicGetPreYOffset = MApi_XC_ACE_PicGetPreYOffset_U2;
    psACEInstPri->fpACE_PicSetHue = MApi_XC_ACE_PicSetHue_U2;
    psACEInstPri->fpACE_SkipWaitVsync = MApi_XC_ACE_SkipWaitVsync_U2;
    psACEInstPri->fpACE_PicSetSaturation = MApi_XC_ACE_PicSetSaturation_U2;
    psACEInstPri->fpACE_PicSetSharpness = MApi_XC_ACE_PicSetSharpness_U2;
    psACEInstPri->fpACE_PicSetColorTemp = MApi_XC_ACE_PicSetColorTemp_U2;
    psACEInstPri->fpACE_PicSetColorTempEx = MApi_XC_ACE_PicSetColorTempEx_U2;
    psACEInstPri->fpACE_PicSetColorTemp_10Bits = MApi_XC_ACE_PicSetColorTemp_10Bits_U2;
    psACEInstPri->fpACE_PicSetPostColorTemp_V02 = MApi_XC_ACE_PicSetPostColorTemp_V02_U2;
    psACEInstPri->fpACE_SetFleshTone = MApi_XC_ACE_SetFleshTone_U2;
    psACEInstPri->fpACE_SetBlackAdjust = MApi_XC_ACE_SetBlackAdjust_U2;
    psACEInstPri->fpACE_Set_IHC_SRAM = MApi_XC_ACE_Set_IHC_SRAM_U2;
    psACEInstPri->fpACE_Set_ICC_SRAM = MApi_XC_ACE_Set_ICC_SRAM_U2;
    psACEInstPri->fpACE_EnableMWE = MApi_XC_ACE_EnableMWE_U2;
    psACEInstPri->fpACE_MWECloneVisualEffect = MApi_XC_ACE_MWECloneVisualEffect_U2;
    psACEInstPri->fpACE_MWESetRegTable = MApi_XC_ACE_MWESetRegTable_U2;
    psACEInstPri->fpACE_SetMWELoadFromPQ = MApi_XC_ACE_SetMWELoadFromPQ_U2;
    psACEInstPri->fpACE_MWEStatus = MApi_XC_ACE_MWEStatus_U2;
    psACEInstPri->fpACE_MWEHandle = MApi_XC_ACE_MWEHandle_U2;
    psACEInstPri->fpACE_MWEFuncSel = MApi_XC_ACE_MWEFuncSel_U2;
    psACEInstPri->fpACE_MWESetDispWin = MApi_XC_ACE_MWESetDispWin_U2;
    psACEInstPri->fpACE_3DClonePQMap = MApi_XC_ACE_3DClonePQMap_U2;
    psACEInstPri->fpACE_DNR_Blending_NRTbl = MApi_XC_ACE_DNR_Blending_NRTbl_U2;
    psACEInstPri->fpACE_DNR_Blending_MISC = MApi_XC_ACE_DNR_Blending_MISC_U2;
    psACEInstPri->fpACE_DNR_Blending = MApi_XC_ACE_DNR_Blending_U2;
    psACEInstPri->fpACE_DNR_Get_PrecisionShift = MApi_XC_ACE_DNR_Get_PrecisionShift_U2;
    psACEInstPri->fpACE_DNR_GetMotion_Weight = MApi_XC_ACE_DNR_GetMotion_Weight_U2;
    psACEInstPri->fpACE_DNR_GetLuma_Weight = MApi_XC_ACE_DNR_GetLuma_Weight_U2;
    psACEInstPri->fpACE_DNR_GetNoise_Weight = MApi_XC_ACE_DNR_GetNoise_Weight_U2;
    psACEInstPri->fpACE_DNR_GetMotion = MApi_XC_ACE_DNR_GetMotion_U2;
    psACEInstPri->fpACE_DNR_Init_Motion = MApi_XC_ACE_DNR_Init_Motion_U2;
    psACEInstPri->fpACE_DNR_Init_Luma = MApi_XC_ACE_DNR_Init_Luma_U2;
    psACEInstPri->fpACE_DNR_Init_Noise = MApi_XC_ACE_DNR_Init_Noise_U2;
    psACEInstPri->fpACE_DNR_SetParam = MApi_XC_ACE_DNR_SetParam_U2;
    psACEInstPri->fpACE_DNR_GetParam = MApi_XC_ACE_DNR_GetParam_U2;
    psACEInstPri->fpACE_DNR_SetNRTbl = MApi_XC_ACE_DNR_SetNRTbl_U2;
    psACEInstPri->fpACE_GetLibVer = MApi_XC_ACE_GetLibVer_U2;
    psACEInstPri->fpACE_GetInfo = MApi_XC_ACE_GetInfo_U2;
    psACEInstPri->fpACE_GetStatus = MApi_XC_ACE_GetStatus_U2;
    psACEInstPri->fpACE_SetDbgLevel = MApi_XC_ACE_SetDbgLevel_U2;
    psACEInstPri->fpACE_ColorCorrectionTable = MApi_XC_ACE_ColorCorrectionTable_U2;
    psACEInstPri->fpACE_SetColorMatrixControl = MApi_XC_ACE_SetColorMatrixControl_U2;
    psACEInstPri->fpACE_SetRBChannelRange = MApi_XC_ACE_SetRBChannelRange_U2;
    psACEInstPri->fpACE_PicSetPostColorTemp = MApi_XC_ACE_PicSetPostColorTemp_U2;
    psACEInstPri->fpACE_PicSetPostColorTempEx = MApi_XC_ACE_PicSetPostColorTempEx_U2;
    psACEInstPri->fpACE_PicSetPostColorTemp2 = MApi_XC_ACE_PicSetPostColorTemp2_U2;
    psACEInstPri->fpACE_PicSetPostColorTemp2Ex = MApi_XC_ACE_PicSetPostColorTemp2Ex_U2;
    psACEInstPri->fpACE_SetPowerState = MApi_XC_ACE_SetPowerState_U2;
    psACEInstPri->fpACE_SetHDRInit = MApi_XC_ACE_SetHDRInit_U2;
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 ACEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    ACE_INSTANCE_PRIVATE* psACEInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psACEInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);

    switch(u32Cmd)
    {
        case E_ACE_CMD_INIT:
            {
                pstACE_Init pACEArgs = (pstACE_Init)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_Init(pInstance, pACEArgs->pstXC_ACE_InitData, pACEArgs->u32InitDataLen);
                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_EXIT:
            {
                pstACE_Exit pACEArgs = (pstACE_Exit)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_Exit(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DMS:
            {
                pstACE_DMS pACEArgs = (pstACE_DMS)pArgs;
                psACEInstPri->fpACE_DMS(pInstance, pACEArgs->eWindow, pACEArgs->bisATV);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETPCYUV2RGB:
            {
                pstACE_SetPCYUV2RGB pACEArgs = (pstACE_SetPCYUV2RGB)pArgs;
                psACEInstPri->fpACE_SetPCYUV2RGB(pInstance, pACEArgs->eWindow, pACEArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SELECTYUVTORGBMATRIX:
            {
                pstACE_SelectYUVtoRGBMatrix pACEArgs = (pstACE_SelectYUVtoRGBMatrix)pArgs;
                psACEInstPri->fpACE_SelectYUVtoRGBMatrix(pInstance, pACEArgs->eWindow, pACEArgs->ucMatrix, pACEArgs->psUserYUVtoRGBMatrix);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETCOLORCORRECTIONTABLE:
            {
                pstACE_SetColorCorrectionTable pACEArgs = (pstACE_SetColorCorrectionTable)pArgs;
                psACEInstPri->fpACE_SetColorCorrectionTable(pInstance, pACEArgs->bScalerWin);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETPCSRGBTABLE:
            {
                pstACE_SetPCsRGBTable pACEArgs = (pstACE_SetPCsRGBTable)pArgs;
                psACEInstPri->fpACE_SetPCsRGBTable(pInstance, pACEArgs->bScalerWin);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETCOLORMATRIX:
            {
                pstACE_ColorMatrix pACEArgs = (pstACE_ColorMatrix)pArgs;
                psACEInstPri->fpACE_GetColorMatrix(pInstance, pACEArgs->eWindow, pACEArgs->pu16Matrix);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETCOLORMATRIX:
            {
                pstACE_ColorMatrix pACEArgs = (pstACE_ColorMatrix)pArgs;
                psACEInstPri->fpACE_SetColorMatrix(pInstance, pACEArgs->eWindow, pACEArgs->pu16Matrix);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETBYPASSCOLORMATRIX:
            {
                pstACE_SetBypassColorMatrix pACEArgs = (pstACE_SetBypassColorMatrix)pArgs;
                pACEArgs->eResult = psACEInstPri->fpACE_SetBypassColorMatrix(pInstance, pACEArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETIHC:
            {
                pstACE_SetIHC pACEArgs = (pstACE_SetIHC)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetIHC(pInstance, pACEArgs->bScalerWin, pACEArgs->eIHC, pACEArgs->u8Val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETICC:
            {
                pstACE_SetICC pACEArgs = (pstACE_SetICC)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetICC(pInstance, pACEArgs->bScalerWin, pACEArgs->eICC, pACEArgs->u8Val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETIBC:
            {
                pstACE_SetIBC pACEArgs = (pstACE_SetIBC)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetIBC(pInstance, pACEArgs->bScalerWin, pACEArgs->eIBC, pACEArgs->u8Val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PATCHDTGCOLORCHECKER:
            {
                pstACE_PatchDTGColorChecker pACEArgs = (pstACE_PatchDTGColorChecker)pArgs;
                psACEInstPri->fpACE_PatchDTGColorChecker(pInstance, pACEArgs->u8Mode);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETACEINFO:
            {
                pstACE_GetACEInfo pACEArgs = (pstACE_GetACEInfo)pArgs;
                pACEArgs->u16Return = psACEInstPri->fpACE_GetACEInfo(pInstance, pACEArgs->bWindow, pACEArgs->eXCACEInfoType);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETACEINFOEX:
            {
                pstACE_GetACEInfoEx pACEArgs = (pstACE_GetACEInfoEx)pArgs;
                psACEInstPri->fpACE_GetACEInfoEx(pInstance, pACEArgs->bWindow, pACEArgs->pInfoEx);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETCONTRAST:
            {
                pstACE_PicSetContrast pACEArgs = (pstACE_PicSetContrast)pArgs;
                psACEInstPri->fpACE_PicSetContrast(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->u8Contrast);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETBRIGHTNESS:
            {
                pstACE_PicSetBrightness pACEArgs = (pstACE_PicSetBrightness)pArgs;
                psACEInstPri->fpACE_PicSetBrightness(pInstance, pACEArgs->eWindow, pACEArgs->u8Brightness_R, pACEArgs->u8Brightness_G, pACEArgs->u8Brightness_B);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETBRIGHTNESSPRECISE:
            {
                pstACE_PicSetBrightnessPrecise pACEArgs = (pstACE_PicSetBrightnessPrecise)pArgs;
                psACEInstPri->fpACE_PicSetBrightnessPrecise(pInstance, pACEArgs->eWindow, pACEArgs->u16Brightness_R, pACEArgs->u16Brightness_G, pACEArgs->u16Brightness_B);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSET_PRE_Y_OFFSET:
            {
                pstACE_PreYOffset pstACEArgs = (pstACE_PreYOffset)pArgs;
                MS_BOOL bResult = psACEInstPri->fpACE_PicSetPreYOffset(pInstance, pstACEArgs->eWindow, pstACEArgs->u8PreYOffset);

                if (bResult)
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
        case E_ACE_CMD_PICGET_PRE_Y_OFFSET:
            {
                pstACE_PreYOffset pstACEArgs = (pstACE_PreYOffset)pArgs;
                pstACEArgs->u8PreYOffset = psACEInstPri->fpACE_PicGetPreYOffset(pInstance, pstACEArgs->eWindow);

                return UTOPIA_STATUS_SUCCESS;
            }
        case E_ACE_CMD_PICSETHUE:
            {
                pstACE_PicSetHue pACEArgs = (pstACE_PicSetHue)pArgs;
                psACEInstPri->fpACE_PicSetHue(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->u8Hue);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SKIPWAITVSYNC:
            {
                pstACE_SkipWaitVsync pACEArgs = (pstACE_SkipWaitVsync)pArgs;
                pACEArgs->eResult = psACEInstPri->fpACE_SkipWaitVsync(pInstance, pACEArgs->eWindow, pACEArgs->bIsSkipWaitVsyn);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETSATURATION:
            {
                pstACE_PicSetSaturation pACEArgs = (pstACE_PicSetSaturation)pArgs;
                psACEInstPri->fpACE_PicSetSaturation(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->u8Saturation);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETSHARPNESS:
            {
                pstACE_PicSetSharpness pACEArgs = (pstACE_PicSetSharpness)pArgs;
                psACEInstPri->fpACE_PicSetSharpness(pInstance, pACEArgs->eWindow, pACEArgs->u8Sharpness );

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETCOLORTEMP:
            {
                pstACE_PicSetColorTemp pACEArgs = (pstACE_PicSetColorTemp)pArgs;
                psACEInstPri->fpACE_PicSetColorTemp(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETCOLORTEMPEX:
            {
                pstACE_PicSetColorTempEx pACEArgs = (pstACE_PicSetColorTempEx)pArgs;
                psACEInstPri->fpACE_PicSetColorTempEx(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETCOLORTEMP_10BITS:
            {
                pstACE_PicSetColorTempEx pACEArgs = (pstACE_PicSetColorTempEx)pArgs;
                psACEInstPri->fpACE_PicSetColorTemp_10Bits(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETPOSTCOLORTEMP_V02:
            {
                pstACE_PicSetPostColorTemp_V02 pACEArgs = (pstACE_PicSetPostColorTemp_V02)pArgs;
                psACEInstPri->fpACE_PicSetPostColorTemp_V02(pInstance, pACEArgs->eWindow, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETFLESHTONE:
            {
                pstACE_SetFleshTone pACEArgs = (pstACE_SetFleshTone)pArgs;
                psACEInstPri->fpACE_SetFleshTone(pInstance, pACEArgs->eWindow, pACEArgs->bEn, pACEArgs->u8FleshTone);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETBLACKADJUST:
            {
                pstACE_SetBlackAdjust pACEArgs = (pstACE_SetBlackAdjust)pArgs;
                psACEInstPri->fpACE_SetBlackAdjust(pInstance, pACEArgs->eWindow, pACEArgs->u8BlackAdjust);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SET_IHC_SRAM:
            {
                pstACE_Set_IHC_SRAM pACEArgs = (pstACE_Set_IHC_SRAM)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_Set_IHC_SRAM(pInstance, pACEArgs->pBuf, pACEArgs->u16ByteSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SET_ICC_SRAM:
            {
                pstACE_Set_ICC_SRAM pACEArgs = (pstACE_Set_ICC_SRAM)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_Set_ICC_SRAM(pInstance, pACEArgs->pBuf, pACEArgs->u16ByteSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_ENABLEMWE:
            {
                pstACE_EnableMWE pACEArgs = (pstACE_EnableMWE)pArgs;
                psACEInstPri->fpACE_EnableMWE(pInstance, pACEArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWECLONEVISUALEFFECT:
            {
                psACEInstPri->fpACE_MWECloneVisualEffect(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWESETREGTABLE:
            {
                pstACE_MWESetRegTable pACEArgs = (pstACE_MWESetRegTable)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_MWESetRegTable(pInstance, pACEArgs->pMWETable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETMWELOADFROMPQ:
            {
                pstACE_SetMWELoadFromPQ pACEArgs = (pstACE_SetMWELoadFromPQ)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetMWELoadFromPQ(pInstance, pACEArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWESTATUS:
            {
                pstACE_MWEStatus pACEArgs = (pstACE_MWEStatus)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_MWEStatus(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWEHANDLE:
            {
                psACEInstPri->fpACE_MWEHandle(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWEFUNCSEL:
            {
                pstACE_MWEFuncSel pACEArgs = (pstACE_MWEFuncSel)pArgs;
                psACEInstPri->fpACE_MWEFuncSel(pInstance, pACEArgs->eWindow, pACEArgs->mwe_func);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_MWESETDISPWIN:
            {
                pstACE_MWESetDispWin pACEArgs = (pstACE_MWESetDispWin)pArgs;
                psACEInstPri->fpACE_MWESetDispWin(pInstance, pACEArgs->u16MWE_Disp_Hstart, pACEArgs->u16MWE_Disp_Vstart, pACEArgs->u16MWE_Disp_Width, pACEArgs->u16MWE_Disp_Height);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_3DCLONEPQMAP:
            {
                pstACE_3DClonePQMap pACEArgs = (pstACE_3DClonePQMap)pArgs;
                psACEInstPri->fpACE_3DClonePQMap(pInstance, pACEArgs->enWeaveType);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_BLENDING_NRTBL:
            {
                pstACE_DNR_Blending_NRTbl pACEArgs = (pstACE_DNR_Blending_NRTbl)pArgs;
                psACEInstPri->fpACE_DNR_Blending_NRTbl(pInstance,
                        pACEArgs->pu8NewNR,
                        pACEArgs->u8Weight,
                        pACEArgs->u8Step,
                        pACEArgs->pu16PreY0,
                        pACEArgs->pu16PreY1,
                        pACEArgs->pu8NRTBL);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_BLENDING_MISC:
            {
                pstACE_DNR_Blending_MISC pACEArgs = (pstACE_DNR_Blending_MISC)pArgs;
                pACEArgs->u16Return = psACEInstPri->fpACE_DNR_Blending_MISC(pInstance, pACEArgs->u8NewItem, pACEArgs->u16PreItem, pACEArgs->u8Weight, pACEArgs->u8Step);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_BLENDING:
            {
                pstACE_DNR_Blending pACEArgs = (pstACE_DNR_Blending)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_Blending(pInstance, pACEArgs->u8NewItem, pACEArgs->u16PreItem, pACEArgs->u8Weight);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GET_PRECISIONSHIFT:
            {
                pstACE_DNR_Get_PrecisionShift pACEArgs = (pstACE_DNR_Get_PrecisionShift)pArgs;
                pACEArgs->u16Return = psACEInstPri->fpACE_DNR_Get_PrecisionShift(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GETMOTION_WEIGHT:
            {
                pstACE_DNR_GetMotion_Weight pACEArgs = (pstACE_DNR_GetMotion_Weight)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_GetMotion_Weight(pInstance,
                        pACEArgs->u8CurMotion,
                        pACEArgs->u8PreMotion,
                        pACEArgs->u8CurMotionLvl,
                        pACEArgs->u8PreMotionLvl,
                        pACEArgs->u8DeFlick_Thre,
                        pACEArgs->u8DeFilick_Step);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GETLUMA_WEIGHT:
            {
                pstACE_DNR_GetLuma_Weight pACEArgs = (pstACE_DNR_GetLuma_Weight)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_GetLuma_Weight(pInstance,
                        pACEArgs->u8CurAvgLuma,
                        pACEArgs->u8PreAvgLuam,
                        pACEArgs->u8CurLumaLvl,
                        pACEArgs->u8PreLumaLvl,
                        pACEArgs->u8DeFlick_Th,
                        pACEArgs->u8DeFlick_Step);


                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GETNOISE_WEIGHT:
            {
                pstACE_DNR_GetNoise_Weight pACEArgs = (pstACE_DNR_GetNoise_Weight)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_GetNoise_Weight(pInstance,
                        pACEArgs->u8CurNoise,
                        pACEArgs->u8PreNoise,
                        pACEArgs->u8Range,
                        pACEArgs->u8DeFlick_Th,
                        pACEArgs->u8DeFlick_Step);


                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GETMOTION:
            {
                pstACE_DNR_GetMotion pACEArgs = (pstACE_DNR_GetMotion)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_GetMotion(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_INIT_MOTION:
            {
                psACEInstPri->fpACE_DNR_Init_Motion(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_INIT_LUMA:
            {
                psACEInstPri->fpACE_DNR_Init_Luma(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_INIT_NOISE:
            {
                psACEInstPri->fpACE_DNR_Init_Noise(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_SETPARAM:
            {
                pstACE_DNR_SetParam pACEArgs = (pstACE_DNR_SetParam)pArgs;
                psACEInstPri->fpACE_DNR_SetParam(pInstance, pACEArgs->eWindow, pACEArgs->eParam, pACEArgs->u16val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_GETPARAM:
            {
                pstACE_DNR_GetParam pACEArgs = (pstACE_DNR_GetParam)pArgs;
                pACEArgs->u8Return = psACEInstPri->fpACE_DNR_GetParam(pInstance, pACEArgs->eWindow, pACEArgs->eParam);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_DNR_SETNRTBL:
            {
                pstACE_DNR_SetNRTbl pACEArgs = (pstACE_DNR_SetNRTbl)pArgs;
                psACEInstPri->fpACE_DNR_SetNRTbl(pInstance, pACEArgs->pu8Tbl);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETLIBVER:
            {
                pstACE_GetLibVer pACEArgs = (pstACE_GetLibVer)pArgs;
                pACEArgs->eResult = psACEInstPri->fpACE_GetLibVer(pInstance, pACEArgs->ppVersion);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETINFO:
            {
                pstACE_GetInfo pACEArgs = (pstACE_GetInfo)pArgs;
                pACEArgs->pstApiInfo = psACEInstPri->fpACE_GetInfo(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_GETSTATUS:
            {
                pstACE_GetStatus pACEArgs = (pstACE_GetStatus)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_GetStatus(pInstance, pACEArgs->pDrvStatus, pACEArgs->bWindow);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETDBGLEVEL:
            {
                pstACE_SetDbgLevel pACEArgs = (pstACE_SetDbgLevel)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetDbgLevel(pInstance, pACEArgs->u16DbgSwitch);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_COLORCORRECTIONTABLE:
            {
                pstACE_ColorCorrectionTable pACEArgs = (pstACE_ColorCorrectionTable)pArgs;
                psACEInstPri->fpACE_ColorCorrectionTable(pInstance, pACEArgs->bScalerWin, pACEArgs->psColorCorrectionTable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETCOLORMATRIXCONTROL:
            {
                pstACE_SetColorMatrixControl pACEArgs = (pstACE_SetColorMatrixControl)pArgs;
                psACEInstPri->fpACE_SetColorMatrixControl(pInstance, pACEArgs->bScalerWin, pACEArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SETRBCHANNELRANGE:
            {
                pstACE_SetRBChannelRange pACEArgs = (pstACE_SetRBChannelRange)pArgs;
                psACEInstPri->fpACE_SetRBChannelRange(pInstance, pACEArgs->bScalerWin, pACEArgs->bRange);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETPOSTCOLORTEMP:
            {
                pstACE_PicSetPostColorTemp pACEArgs = (pstACE_PicSetPostColorTemp)pArgs;
                psACEInstPri->fpACE_PicSetPostColorTemp(pInstance, pACEArgs->eWindow, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETPOSTCOLORTEMPEX:
            {
                pstACE_PicSetPostColorTempEx pACEArgs = (pstACE_PicSetPostColorTempEx)pArgs;
                psACEInstPri->fpACE_PicSetPostColorTempEx(pInstance, pACEArgs->eWindow, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETPOSTCOLORTEMP2:
            {
                pstACE_PicSetColorTemp pACEArgs = (pstACE_PicSetColorTemp)pArgs;
                psACEInstPri->fpACE_PicSetPostColorTemp2(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_PICSETPOSTCOLORTEMP2EX:
            {
                pstACE_PicSetPostColorTempEx pACEArgs = (pstACE_PicSetPostColorTempEx)pArgs;
                psACEInstPri->fpACE_PicSetPostColorTemp2Ex(pInstance, pACEArgs->eWindow, pACEArgs->bUseYUVSpace, pACEArgs->pstColorTemp);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SET_POWERSTATE:
            {
                pstACE_Set_PowerState pACEArgs = (pstACE_Set_PowerState)pArgs;
                pACEArgs->u32Return = psACEInstPri->fpACE_SetPowerState(pInstance, pACEArgs->enPowerState);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_ACE_CMD_SET_HDR_INFO:
            {
                pstACE_HDRInitData pACEArgs = (pstACE_HDRInitData)pArgs;
                pACEArgs->bReturn = psACEInstPri->fpACE_SetHDRInit(pInstance, pACEArgs->pstHDRInitData);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        default:
            printf("Error Command = %td\n",(ptrdiff_t)u32Cmd);
            break;

    }
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 ACEClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

