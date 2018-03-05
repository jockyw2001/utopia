#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "drvXC_HDMI_if.h"
#include "mhal_xc_chip_config.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "apiXC_Auto_v2.h"

void AUTO_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpXC_Auto_Geometry = 	MApi_XC_Auto_Geometry_U2;
    psXCInstPri->fpXC_Auto_Geometry_Ex = 	MApi_XC_Auto_Geometry_Ex_U2;
    psXCInstPri->fpXC_Auto_StopAutoGeometry = 	MApi_XC_Auto_StopAutoGeometry_U2;
    psXCInstPri->fpXC_Auto_GainOffset = 	MApi_XC_Auto_GainOffset_U2;
    psXCInstPri->fpXC_Auto_GetHWFixedGainOffset = 	MApi_XC_Auto_GetHWFixedGainOffset_U2;
    psXCInstPri->fpXC_Auto_SetValidData = 	MApi_XC_Auto_SetValidData_U2;
    psXCInstPri->fpXC_Auto_AutoOffset = 	MApi_XC_Auto_AutoOffset_U2;
    psXCInstPri->fpXC_Auto_DetectWidth = 	MApi_XC_Auto_DetectWidth_U2;
    psXCInstPri->fpXC_Auto_SetCalibrationMode = 	MApi_XC_Auto_SetCalibrationMode_U2;
    psXCInstPri->fpXC_Auto_GetCalibrationMode =	MApi_XC_Auto_GetCalibrationMode_U2;
    psXCInstPri->fpXC_Auto_GetSyncInfo =	MApi_XC_AUTO_GetSyncInfo_U2;

    return;
}

MS_U32 _AUTOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
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
        case E_XC_AUTO_CMD_GEOMETRY:
            {

                pstXC_AUTO_GEOMETRY pXCArgs = (pstXC_AUTO_GEOMETRY) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_Geometry(pInstance,
                                                                    pXCArgs->enAutoTuneType,
                                                                    pXCArgs->ActiveInfo,
                                                                    pXCArgs->StandardInfo,
                                                                    pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_GEOMETRY_EX:
            {

                pstXC_AUTO_GEOMETRY_EX pXCArgs = (pstXC_AUTO_GEOMETRY_EX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_Geometry_Ex(pInstance,
                                                                    pXCArgs->enAutoTuneType,
                                                                    pXCArgs->ActiveInfo,
                                                                    pXCArgs->StandardInfo,
                                                                    pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_STOPAUTOGEOMETRY:
            {

                pstXC_AUTO_STOPAUTOGEOMETRY pXCArgs = (pstXC_AUTO_STOPAUTOGEOMETRY) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_StopAutoGeometry(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_GAINOFFSET:
            {

                pstXC_AUTO_GAINOFFSET pXCArgs = (pstXC_AUTO_GAINOFFSET) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_GainOffset(pInstance,
                                                                    pXCArgs->type,
                                                                    pXCArgs->enAutoTuneType,
                                                                    pXCArgs->pstADCSetting,
                                                                    pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_GETHWFIXEDGAINOFFSET:
            {

                pstXC_AUTO_GETHWFIXEDGAINOFFSET pXCArgs = (pstXC_AUTO_GETHWFIXEDGAINOFFSET) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_GetHWFixedGainOffset(pInstance,
                                                                    pXCArgs->enAutoTuneType,
                                                                    pXCArgs->pstADCSetting);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_SETVALIDDATA:
            {

                pstXC_AUTO_SETVALIDDATA pXCArgs = (pstXC_AUTO_SETVALIDDATA) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_SetValidData(pInstance, pXCArgs->Value);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_AUTOOFFSET:
            {

                pstXC_AUTO_AUTOOFFSET pXCArgs = (pstXC_AUTO_AUTOOFFSET) pArgs;
                psXCInstPri->fpXC_Auto_AutoOffset(pInstance, pXCArgs->bEnable, pXCArgs->bIsYPbPrFlag);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_DETECTWIDTH:
            {

                pstXC_AUTO_DETECTWIDTH pXCArgs = (pstXC_AUTO_DETECTWIDTH) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_DetectWidth(pInstance,
                                                                    pXCArgs->pSignalInfo,
                                                                    pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_SETCALIBRATIONMODE:
            {

                pstXC_AUTO_SETCALIBRATIONMODE pXCArgs = (pstXC_AUTO_SETCALIBRATIONMODE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Auto_SetCalibrationMode(pInstance, pXCArgs->eMode);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_GETCALIBRATIONMODE:
            {

                pstXC_AUTO_GETCALIBRATIONMODE pXCArgs = (pstXC_AUTO_GETCALIBRATIONMODE) pArgs;
                psXCInstPri->fpXC_Auto_GetCalibrationMode(pInstance, pXCArgs->eMode);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_AUTO_CMD_GETSYNCINFO:
            {

                pstXC_AUTO_GETSYNCINFO pXCArgs = (pstXC_AUTO_GETSYNCINFO) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                if(pXCArgs->pSyncInfo->eWindow == SUB_WINDOW)
                {
                    CHECK_DEVICE1_INSTANCE();
                    pInstance = g_pDevice1Instance;
                    pXCArgs->pSyncInfo->eWindow = MAIN_WINDOW;


                }
#endif
                psXCInstPri->fpXC_Auto_GetSyncInfo(pInstance,pXCArgs->pSyncInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_AUTO_CMD_NONE:
        case E_XC_AUTO_CMD_MAX:
        default:
            {
                printf("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}
