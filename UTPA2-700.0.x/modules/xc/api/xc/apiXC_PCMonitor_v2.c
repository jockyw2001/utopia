#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mhal_xc_chip_config.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
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
#include "apiXC_PCMonitor_v2.h"

void PCMONITOR_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpXC_PCMonitor_Init = 	MApi_XC_PCMonitor_Init_U2;
    psXCInstPri->fpXC_PCMonitor_Restart = 	MApi_XC_PCMonitor_Restart_U2;
    psXCInstPri->fpXC_PCMonitor_SetTimingCount = 	MApi_XC_PCMonitor_SetTimingCount_U2;
    psXCInstPri->fpXC_PCMonitor = 	MApi_XC_PCMonitor_U2;
    psXCInstPri->fpXC_PCMonitor_GetCurrentState = 	MApi_XC_PCMonitor_GetCurrentState_U2;
    psXCInstPri->fpXC_PCMonitor_GetSyncStatus = 	MApi_XC_PCMonitor_GetSyncStatus_U2;
    psXCInstPri->fpXC_PCMonitor_Get_HFreqx10 = 	MApi_XC_PCMonitor_Get_HFreqx10_U2;
    psXCInstPri->fpXC_PCMonitor_Get_HFreqx1K = 	MApi_XC_PCMonitor_Get_HFreqx1K_U2;
    psXCInstPri->fpXC_PCMonitor_Get_VFreqx10 = 	MApi_XC_PCMonitor_Get_VFreqx10_U2;
    psXCInstPri->fpXC_PCMonitor_Get_VFreqx1K = 	MApi_XC_PCMonitor_Get_VFreqx1K_U2;
    psXCInstPri->fpXC_PCMonitor_Get_Vtotal = 	MApi_XC_PCMonitor_Get_Vtotal_U2;
    psXCInstPri->fpXC_PCMonitor_Get_Dvi_Hdmi_De_Info = 	MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info_U2;
    psXCInstPri->fpXC_PCMonitor_SyncLoss = 	MApi_XC_PCMonitor_SyncLoss_U2;
    psXCInstPri->fpXC_PCMonitor_InvalidTimingDetect = 	MApi_XC_PCMonitor_InvalidTimingDetect_U2;
    psXCInstPri->fpXC_PCMonitor_SetTimingCountEx = 	MApi_XC_PCMonitor_SetTimingCountEx_U2;

    return;
}

MS_U32 _PCMONITORIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
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
        case E_XC_PCMONITOR_CMD_INIT:
            {

                pstXC_PCMONITOR_INIT pXCArgs = (pstXC_PCMONITOR_INIT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_PCMonitor_Init(pInstance, pXCArgs->u8MaxWindowNum);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->bReturnValue = psXCInstPri->fpXC_PCMonitor_Init(g_pDevice1Instance, pXCArgs->u8MaxWindowNum);
#endif
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_RESTART:
            {

                pstXC_PCMONITOR_RESTART pXCArgs = (pstXC_PCMONITOR_RESTART) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                psXCInstPri->fpXC_PCMonitor_Restart(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNT:
            {

                pstXC_PCMONITOR_SETTIMINGCOUNT pXCArgs = (pstXC_PCMONITOR_SETTIMINGCOUNT) pArgs;
                psXCInstPri->fpXC_PCMonitor_SetTimingCount(pInstance, pXCArgs->u16TimingStableCounter, pXCArgs->u16TimingNosyncCounter);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                psXCInstPri->fpXC_PCMonitor_SetTimingCount(g_pDevice1Instance, pXCArgs->u16TimingStableCounter, pXCArgs->u16TimingNosyncCounter);
#endif

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD:
            {

                pstXC_PCMONITOR pXCArgs = (pstXC_PCMONITOR) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->eReturnValue = psXCInstPri->fpXC_PCMonitor(pInstance, pXCArgs->eCurrentSrc, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GETCURRENTSTATE:
            {

                pstXC_PCMONITOR_GETCURRENTSTATE pXCArgs = (pstXC_PCMONITOR_GETCURRENTSTATE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->eReturnValue = psXCInstPri->fpXC_PCMonitor_GetCurrentState(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GETSYNCSTATUS:
            {

                pstXC_PCMONITOR_GETSYNCSTATUS pXCArgs = (pstXC_PCMONITOR_GETSYNCSTATUS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u8ReturnValue = psXCInstPri->fpXC_PCMonitor_GetSyncStatus(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_HFREQX10:
            {

                pstXC_PCMONITOR_GET_HFREQX10 pXCArgs = (pstXC_PCMONITOR_GET_HFREQX10) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_PCMonitor_Get_HFreqx10(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_HFREQX1K:
            {

                pstXC_PCMONITOR_GET_HFREQX1K pXCArgs = (pstXC_PCMONITOR_GET_HFREQX1K) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u32ReturnValue = psXCInstPri->fpXC_PCMonitor_Get_HFreqx1K(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_VFREQX10:
            {

                pstXC_PCMONITOR_GET_VFREQX10 pXCArgs = (pstXC_PCMONITOR_GET_VFREQX10) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_PCMonitor_Get_VFreqx10(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_VFREQX1K:
            {

                pstXC_PCMONITOR_GET_VFREQX1K pXCArgs = (pstXC_PCMONITOR_GET_VFREQX1K) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u32ReturnValue = psXCInstPri->fpXC_PCMonitor_Get_VFreqx1K(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_VTOTAL:
            {

                pstXC_PCMONITOR_GET_VTOTAL pXCArgs = (pstXC_PCMONITOR_GET_VTOTAL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->u16ReturnValue = psXCInstPri->fpXC_PCMonitor_Get_Vtotal(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_GET_DVI_HDMI_DE_INFO:
            {

                pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO pXCArgs = (pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                psXCInstPri->fpXC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, pXCArgs->eWindow, pXCArgs->msWin);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_SYNCLOSS:
            {

                pstXC_PCMONITOR_SYNCLOSS pXCArgs = (pstXC_PCMONITOR_SYNCLOSS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_PCMonitor_SyncLoss(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_INVALIDTIMINGDETECT:
            {

                pstXC_PCMONITOR_INVALIDTIMINGDETECT pXCArgs = (pstXC_PCMONITOR_INVALIDTIMINGDETECT) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                USE_OTHER_SC_AS_SUB_WINDOW();
#endif
                pXCArgs->bReturnValue = psXCInstPri->fpXC_PCMonitor_InvalidTimingDetect(pInstance, pXCArgs->bPollingOnly, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNTEX:
            {

                pstXC_PCMONITOR_SETTIMINGCOUNTEX pXCArgs = (pstXC_PCMONITOR_SETTIMINGCOUNTEX) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpXC_PCMonitor_SetTimingCountEx(pInstance, pXCArgs->eCurrentSrc, pXCArgs->u16TimingStableCounter, pXCArgs->u16TimingNosyncCounter);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->eReturnValue = psXCInstPri->fpXC_PCMonitor_SetTimingCountEx(g_pDevice1Instance, pXCArgs->eCurrentSrc, pXCArgs->u16TimingStableCounter, pXCArgs->u16TimingNosyncCounter);
#endif

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_PCMONITOR_CMD_NONE:
        case E_XC_PCMONITOR_CMD_MAX:
        default:
            {
                printf("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}

