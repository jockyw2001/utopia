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
#include "apiXC_ModeParse_v2.h"

void MODEPARSE_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpXC_ModeParse_Init = MApi_XC_ModeParse_Init_U2;
    psXCInstPri->fpXC_ModeParse_MatchMode = MApi_XC_ModeParse_MatchMode_U2;
    psXCInstPri->fpXC_ModeParse_MatchModeEx = MApi_XC_ModeParse_MatchModeEx_U2;

    return;
}

MS_U32 _MODEPARSEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
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
        case E_XC_MODEPARSE_CMD_INIT:
            {

                pstXC_MODEPARSE_INIT pXCArgs = (pstXC_MODEPARSE_INIT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_ModeParse_Init(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_MODEPARSE_CMD_MATCHMODE:
            {

                pstXC_MODEPARSE_MATCHMODE pXCArgs = (pstXC_MODEPARSE_MATCHMODE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpXC_ModeParse_MatchMode(pInstance, pXCArgs->ModeDB,pXCArgs->u8NumberOfItems,pXCArgs->psInputInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_MODEPARSE_CMD_MATCHMODEEX:
            {

                pstXC_MODEPARSE_MATCHMODEEX pXCArgs = (pstXC_MODEPARSE_MATCHMODEEX) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpXC_ModeParse_MatchModeEx(pInstance, pXCArgs->ModeDB,pXCArgs->u8NumberOfItems,pXCArgs->psInputInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_MODEPARSE_CMD_NONE:
        case E_XC_MODEPARSE_CMD_MAX:
        default:
            {
                printf("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}
