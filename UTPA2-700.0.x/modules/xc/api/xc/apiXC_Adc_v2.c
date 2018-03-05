#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_ip.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "mhal_xc_chip_config.h"
#include "drvXC_ADC_Internal.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "apiXC_Adc_v2.h"


void ADC_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpXC_ADC_SetCVBSOut =     MApi_XC_ADC_SetCVBSOut_U2;
    psXCInstPri->fpXC_ADC_IsCVBSOutEnabled =     MApi_XC_ADC_IsCVBSOutEnabled_U2;
    psXCInstPri->fpXC_ADC_SetPcClock =     MApi_XC_ADC_SetPcClock_U2;
    psXCInstPri->fpXC_ADC_SetPhase =     MApi_XC_ADC_SetPhase_U2;
    psXCInstPri->fpXC_ADC_SetPhaseEx =     MApi_XC_ADC_SetPhaseEx_U2;
    psXCInstPri->fpXC_ADC_GetPhaseRange =     MApi_XC_ADC_GetPhaseRange_U2;
    psXCInstPri->fpXC_ADC_GetPhase =     MApi_XC_ADC_GetPhase_U2;
    psXCInstPri->fpXC_ADC_GetPhaseEx =     MApi_XC_ADC_GetPhaseEx_U2;
    psXCInstPri->fpXC_ADC_IsScartRGB =     MApi_XC_ADC_IsScartRGB_U2;
    psXCInstPri->fpXC_ADC_GetPcClock =     MApi_XC_ADC_GetPcClock_U2;
    psXCInstPri->fpXC_ADC_GetSoGLevelRange =     MApi_XC_ADC_GetSoGLevelRange_U2;
    psXCInstPri->fpXC_ADC_SetSoGLevel =     MApi_XC_ADC_SetSoGLevel_U2;
    psXCInstPri->fpXC_ADC_PowerOff =     MApi_XC_ADC_PowerOff_U2;
    psXCInstPri->fpXC_ADC_GetDefaultGainOffset =     MApi_XC_ADC_GetDefaultGainOffset_U2;
    psXCInstPri->fpXC_ADC_GetMaximalOffsetValue =     MApi_XC_ADC_GetMaximalOffsetValue_U2;
    psXCInstPri->fpXC_ADC_GetMaximalGainValue =     MApi_XC_ADC_GetMaximalGainValue_U2;
    psXCInstPri->fpXC_ADC_GetCenterGain =     MApi_XC_ADC_GetCenterGain_U2;
    psXCInstPri->fpXC_ADC_GetCenterOffset =     MApi_XC_ADC_GetCenterOffset_U2;
    psXCInstPri->fpXC_ADC_SetGain =     MApi_XC_ADC_SetGain_U2;
    psXCInstPri->fpXC_ADC_SetOffset =     MApi_XC_ADC_SetOffset_U2;
    psXCInstPri->fpXC_ADC_AdjustGainOffset =     MApi_XC_ADC_AdjustGainOffset_U2;
    psXCInstPri->fpXC_ADC_Source_Calibrate =     MApi_XC_ADC_Source_Calibrate_U2;
    psXCInstPri->fpXC_ADC_SetSoGCal =     MApi_XC_ADC_SetSoGCal_U2;
    psXCInstPri->fpXC_ADC_SetRGB_PIPE_Delay =     MApi_XC_ADC_SetRGB_PIPE_Delay_U2;
    psXCInstPri->fpXC_ADC_ScartRGB_SOG_ClampDelay =     MApi_XC_ADC_ScartRGB_SOG_ClampDelay_U2;
    psXCInstPri->fpXC_ADC_Set_YPbPrLooseLPF =     MApi_XC_ADC_Set_YPbPrLooseLPF_U2;
    psXCInstPri->fpXC_ADC_Set_SOGBW =     MApi_XC_ADC_Set_SOGBW_U2;
    psXCInstPri->fpXC_ADC_SetClampDuration =     MApi_XC_ADC_SetClampDuration_U2;
    psXCInstPri->fpXC_ADC_EnableHWCalibration =     MApi_XC_ADC_EnableHWCalibration_U2;
    psXCInstPri->fpXC_ADC_SetIdacCurrentMode =     MApi_XC_ADC_SetIdacCurrentMode_U2;
    psXCInstPri->fpXC_ADC_GetIdacCurrentMode =     MApi_XC_ADC_GetIdacCurrentMode_U2;

    return;
}

MS_U32 _ADCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //void* pModule = NULL;
    //void* pResource = NULL;
    XC_INSTANCE_PRIVATE* psXCInstPri = NULL;

    //UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psXCInstPri)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
    }

    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    switch(u32Cmd)
    {
        case E_XC_ADC_CMD_SETCVBSOUT:
            {

                pstXC_ADC_SETCVBSOUT  pXCArgs = (pstXC_ADC_SETCVBSOUT) pArgs;
                psXCInstPri->fpXC_ADC_SetCVBSOut(pInstance, pXCArgs->bEnable, pXCArgs->enOutputType, pXCArgs->enInputPortType, pXCArgs->isDigitalSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_ISCVBSOUTENABLED:
            {

                pstXC_ADC_ISCVBSOUTENABLED  pXCArgs = (pstXC_ADC_ISCVBSOUTENABLED) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_IsCVBSOutEnabled(pInstance, pXCArgs->enOutputType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETPCCLOCK:
            {

                pstXC_ADC_SETPCCLOCK  pXCArgs = (pstXC_ADC_SETPCCLOCK) pArgs;
                psXCInstPri->fpXC_ADC_SetPcClock(pInstance, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETPHASE:
            {

                pstXC_ADC_SETPHASE  pXCArgs = (pstXC_ADC_SETPHASE) pArgs;
                psXCInstPri->fpXC_ADC_SetPhase(pInstance, pXCArgs->u8Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETPHASEEX:
            {

                pstXC_ADC_SETPHASEEX  pXCArgs = (pstXC_ADC_SETPHASEEX) pArgs;
                psXCInstPri->fpXC_ADC_SetPhaseEx(pInstance, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETPHASERANGE:
            {

                pstXC_ADC_GETPHASERANGE  pXCArgs = (pstXC_ADC_GETPHASERANGE) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetPhaseRange(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETPHASE:
            {

                pstXC_ADC_GETPHASE  pXCArgs = (pstXC_ADC_GETPHASE) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpXC_ADC_GetPhase(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETPHASEEX:
            {

                pstXC_ADC_GETPHASEEX  pXCArgs = (pstXC_ADC_GETPHASEEX) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetPhaseEx(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_ISSCARTRGB:
            {

                pstXC_ADC_ISSCARTRGB  pXCArgs = (pstXC_ADC_ISSCARTRGB) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_IsScartRGB(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETPCCLOCK:
            {

                pstXC_ADC_GETPCCLOCK  pXCArgs = (pstXC_ADC_GETPCCLOCK) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetPcClock(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETSOGLEVELRANGE:
            {

                pstXC_ADC_GETSOGLEVELRANGE  pXCArgs = (pstXC_ADC_GETSOGLEVELRANGE) pArgs;
                psXCInstPri->fpXC_ADC_GetSoGLevelRange(pInstance, pXCArgs->u32min,pXCArgs->u32max, pXCArgs->u32Recommend_value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETSOGLEVEL:
            {

                pstXC_ADC_SETSOGLEVEL  pXCArgs = (pstXC_ADC_SETSOGLEVEL) pArgs;
                psXCInstPri->fpXC_ADC_SetSoGLevel(pInstance, pXCArgs->u32Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_POWEROFF:
            {


                psXCInstPri->fpXC_ADC_PowerOff(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETDEFAULTGAINOFFSET:
            {

                pstXC_ADC_GETDEFAULTGAINOFFSET  pXCArgs = (pstXC_ADC_GETDEFAULTGAINOFFSET) pArgs;
                psXCInstPri->fpXC_ADC_GetDefaultGainOffset(pInstance, pXCArgs->enSource, pXCArgs->pstADCSetting);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETMAXIMALOFFSETVALUE:
            {

                pstXC_ADC_GETMAXIMALOFFSETVALUE  pXCArgs = (pstXC_ADC_GETMAXIMALOFFSETVALUE) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetMaximalOffsetValue(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETMAXIMALGAINVALUE:
            {

                pstXC_ADC_GETMAXIMALGAINVALUE  pXCArgs = (pstXC_ADC_GETMAXIMALGAINVALUE) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetMaximalGainValue(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETCENTERGAIN:
            {

                pstXC_ADC_GETCENTERGAIN  pXCArgs = (pstXC_ADC_GETCENTERGAIN) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetCenterGain(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GETCENTEROFFSET:
            {

                pstXC_ADC_GETCENTEROFFSET  pXCArgs = (pstXC_ADC_GETCENTEROFFSET) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_ADC_GetCenterOffset(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETGAIN:
            {

                pstXC_ADC_SETGAIN  pXCArgs = (pstXC_ADC_SETGAIN) pArgs;
                psXCInstPri->fpXC_ADC_SetGain(pInstance, pXCArgs->u8Color, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETOFFSET:
            {

                pstXC_ADC_SETOFFSET  pXCArgs = (pstXC_ADC_SETOFFSET) pArgs;
                psXCInstPri->fpXC_ADC_SetOffset(pInstance, pXCArgs->u8Color, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_ADJUSTGAINOFFSET:
            {

                pstXC_ADC_ADJUSTGAINOFFSET  pXCArgs = (pstXC_ADC_ADJUSTGAINOFFSET) pArgs;
                psXCInstPri->fpXC_ADC_AdjustGainOffset(pInstance, pXCArgs->pstADCSetting);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SOURCE_CALIBRATE:
            {

                pstXC_ADC_SOURCE_CALIBRATE  pXCArgs = (pstXC_ADC_SOURCE_CALIBRATE) pArgs;
                psXCInstPri->fpXC_ADC_Source_Calibrate(pInstance, pXCArgs->enInputSourceType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETSOGCAL:
            {

                psXCInstPri->fpXC_ADC_SetSoGCal(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETRGB_PIPE_DELAY:
            {

                pstXC_ADC_SETRGB_PIPE_DELAY  pXCArgs = (pstXC_ADC_SETRGB_PIPE_DELAY) pArgs;
                psXCInstPri->fpXC_ADC_SetRGB_PIPE_Delay(pInstance, pXCArgs->u8Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SCARTRGB_SOG_CLAMPDELAY:
            {

                pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY  pXCArgs = (pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY) pArgs;
                psXCInstPri->fpXC_ADC_ScartRGB_SOG_ClampDelay(pInstance, pXCArgs->u16Clpdly, pXCArgs->u16Caldur);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SET_YPBPRLOOSELPF:
            {

                pstXC_ADC_SET_YPBPRLOOSELPF  pXCArgs = (pstXC_ADC_SET_YPBPRLOOSELPF) pArgs;
                psXCInstPri->fpXC_ADC_Set_YPbPrLooseLPF(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                psXCInstPri->fpXC_ADC_Set_YPbPrLooseLPF(g_pDevice1Instance, pXCArgs->bEnable);
#endif
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SET_SOGBW:
            {

                pstXC_ADC_SET_SOGBW  pXCArgs = (pstXC_ADC_SET_SOGBW) pArgs;
                psXCInstPri->fpXC_ADC_Set_SOGBW(pInstance, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SETCLAMPDURATION:
            {

                pstXC_ADC_SETCLAMPDURATION  pXCArgs = (pstXC_ADC_SETCLAMPDURATION) pArgs;
                psXCInstPri->fpXC_ADC_SetClampDuration(pInstance, pXCArgs->u16Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_ENABLEHWCALIBRATION:
            {

                pstXC_ADC_ENABLEHWCALIBRATION  pXCArgs = (pstXC_ADC_ENABLEHWCALIBRATION) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_EnableHWCalibration(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_EnableHWCalibration(g_pDevice1Instance, pXCArgs->bEnable);
#endif
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_SET_IDACCURRENTMODE:
            {
                pstXC_ADC_SETIDACCURRENTMODE  pXCArgs = (pstXC_ADC_SETIDACCURRENTMODE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_SetIdacCurrentMode(pInstance, pXCArgs->eMode);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ADC_SetIdacCurrentMode(g_pDevice1Instance, pXCArgs->eMode);
#endif
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_GET_IDACCURRENTMODE:
            {
                pstXC_ADC_GETIDACCURRENTMODE  pXCArgs = (pstXC_ADC_GETIDACCURRENTMODE) pArgs;
                pXCArgs->eReturnMode = psXCInstPri->fpXC_ADC_GetIdacCurrentMode(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_ADC_CMD_NONE:
        case E_XC_ADC_CMD_MAX:
        default:
            {
                printf("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}

