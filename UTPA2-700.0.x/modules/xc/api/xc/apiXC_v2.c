
#define  _XC_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "mvideo_context.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mhal_xc_chip_config.h"
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
#include "apiXC_Adc_v2.h"
#include "apiXC_Auto_v2.h"
#include "apiXC_ModeParse_v2.h"
#include "apiXC_PCMonitor_v2.h"
#include "drvXC_HDMI_if_v2.h"
#include "xc_hwreg_utility2.h"

#include "utopia_api_database.h"
#include "halCHIP.h"

#define XC_RES_NAME_MAX_LEN  64
#define atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0)
// ========== Global definition ==========
void* pModuleXC = NULL;
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
extern MS_U32 u32SetWindowTime;
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
static MS_BOOL bIsXCRegistered = FALSE;
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
void* pInstanceXCStr = NULL;
MS_U32 XCStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stXC_SET_POWERSTATE XCArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstanceXCStr == NULL)
            {
                if(UtopiaOpen(MODULE_XC, &pInstanceXCStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen XC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            XCArgs.enPowerState = u32PowerState;
            XCArgs.u32ReturnValue = UTOPIA_STATUS_FAIL;

            if(UtopiaIoctl(pInstanceXCStr, E_XC_CMD_SET_POWERSTATE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain XC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=XCArgs.u32ReturnValue;
            break;
        case E_POWER_RESUME:
            if (pInstanceXCStr == NULL)
            {
                if(UtopiaOpen(MODULE_XC, &pInstanceXCStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen XC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            XCArgs.enPowerState = u32PowerState;
            XCArgs.u32ReturnValue = UTOPIA_STATUS_FAIL;

            if(UtopiaIoctl(pInstanceXCStr, E_XC_CMD_SET_POWERSTATE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain XC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=XCArgs.u32ReturnValue;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT

static void _XCMdbloctl_Cat_OPTEE(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_RESOURCE_PRIVATE_FOR_TEE* pXCResourcePrivateForTee = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivateForTee));

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler OPTEE Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"OPTEE REE enable                   : %s\n", pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable==FALSE?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"OPTEE TEE enable                   : %s\n", pXCResourcePrivateForTee->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable==TRUE?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"OPTEE Buffer Address               : 0x%lx\n", pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].drams_data[0].address);
    MdbPrint(paraGetInfo->u64ReqHdl,"OPTEE Buffer Length                : %x\n", pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].drams_data[0].length);
}

static void _XCMdbloctl_Cat_3D(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler 3D Info -----------------\n");
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_FRAME_PACKING)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_FRAME_PACKING\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_FIELD_ALTERNATIVE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_LINE_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_LINE_ALTERNATIVE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_SIDE_BY_SIDE_FULL\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_L_DEPTH)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_L_DEPTH\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_TOP_BOTTOM)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_TOP_BOTTOM\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_SIDE_BY_SIDE_HALF\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_CHECK_BORAD)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_CHECK_BORAD\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_USER)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_MODE_USER\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_FRAME_ALTERNATIVE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_FRAME_PACKING_OPT)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_FRAME_PACKING_OPT\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_TOP_BOTTOM_OPT)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_TOP_BOTTOM_OPT\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_NORMAL_2D\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D_INTERLACE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_NORMAL_2D_INTERLACE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D_INTERLACE_PTP)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_NORMAL_2D_INTERLACE_PTP\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_NORMAL_2D_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D input format                    : E_XC_3D_INPUT_PIXEL_ALTERNATIVE\n");
    }

    if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_MODE_NONE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_MODE_NONE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_LINE_ALTERNATIVE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_TOP_BOTTOM\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_ALTERNATIVE\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_L)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_L\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_R)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_R\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_CHECKBOARD_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_CHECKBOARD_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_L_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_L_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_R_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_R_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_TOP_BOTTOM_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_PACKING)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_PACKING\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_TOP)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_TOP_TOP\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_BOTTOM_BOTTOM)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_BOTTOM_BOTTOM\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LEFT_LEFT)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_LEFT_LEFT\n");
    }
    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_RIGHT_RIGHT)
    {
         MdbPrint(paraGetInfo->u64ReqHdl,"3D output format                   : E_XC_3D_OUTPUT_RIGHT_RIGHT\n");
    }

    if(((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D)||(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_NORMAL_2D_HW))&&(E_XC_3D_OUTPUT_MODE_NONE != pXCResourcePrivate->stdrvXC_3D._eOutput3DMode))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"2D to 3D status                    : TRUE\n");
    }
    else
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"2D to 3D status                    : FALSE\n");
    }
    MdbPrint(paraGetInfo->u64ReqHdl,"Output timing                      : %d_%d_%d\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width,
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq);
}

static void _XCMdbloctl_Cat_TGEN(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler TGEN Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Hsync width                        : %d\n", MApi_XC_R2BYTEMSK(REG_SC_BK10_01_L, 0xFF));
    MdbPrint(paraGetInfo->u64ReqHdl,"Vsync position                     : (%d,%d)\n", MApi_XC_R2BYTEMSK(REG_SC_BK10_02_L, 0x7FF)+(MApi_XC_R2BYTEMSK(REG_SC_BK10_02_L, 0x1000)>>1), MApi_XC_R2BYTEMSK(REG_SC_BK10_03_L, 0x1FFF));
    MdbPrint(paraGetInfo->u64ReqHdl,"HDE position                       : (%d,%d)\n", MApi_XC_R2BYTEMSK(REG_SC_BK10_04_L, 0x3FFF), MApi_XC_R2BYTEMSK(REG_SC_BK10_05_L, 0x3FFF));
    MdbPrint(paraGetInfo->u64ReqHdl,"VDE position                       : (%d,%d)\n", MApi_XC_R2BYTEMSK(REG_SC_BK10_06_L, 0x1FFF), MApi_XC_R2BYTEMSK(REG_SC_BK10_07_L, 0x1FFF));
    MdbPrint(paraGetInfo->u64ReqHdl,"Vsync control mode                 : %s\n", MApi_XC_R2BYTEMSK(REG_SC_BK10_05_L, 0x01)==0x00?"Auto":"Manual");
}

static void _XCMdbloctl_Cat_HDR(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler HDR Info -----------------\n");
    //MdbPrint(paraGetInfo->u64ReqHdl,"HDR type                           : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Gamma/Degamma autodownload status  : %s\n", MApi_XC_R2BYTEMSK(REG_SC_BK7A_01_L, BIT(0))==0?"On":"Off");
    MdbPrint(paraGetInfo->u64ReqHdl,"TMO autodownload status            : %s\n", MApi_XC_R2BYTEMSK(REG_SC_BK7A_02_L, BIT(2))==0?"On":"Off");
    MdbPrint(paraGetInfo->u64ReqHdl,"3D LUT autodownload status         : %s\n", MApi_XC_R2BYTEMSK(REG_SC_BK7A_02_L, BIT(4))==0?"On":"Off");
    MdbPrint(paraGetInfo->u64ReqHdl,"XC handshake status                : %s\n", MApi_XC_R2BYTE(REG_SC_BK02_10_L)==0x25?"Off":"On");
    /*MdbPrint(paraGetInfo->u64ReqHdl,"CRC status                         : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"R2Y status                         : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Y2R status                         : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"MM HDR vdec version number         : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Autodownload fail bit              : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Dolby Global dimming value         : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Dolby Global dimming delay count   : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"set_HDRDS_info average time        : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"set_HDRDS_info peak time           : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"HDR HSize                          : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"HDR VSize                          : \n");*/
}

static void _XCMdbloctl_Cat_MEMC(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    XC_REGS_SAVE_AREA *pXCResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pXCResourceStr);
    MS_U8 u8sel, u8temp = 0;

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler MEMC Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"FRC enable                         : %s\n", MApi_XC_R2BYTEMSK(REG_SC_BK68_40_L, BIT(2))==BIT(2)?"On":"Off");
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC level                         : %d\n", R2BYTE(0x422c44UL));
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC status                        : %s\n", R2BYTEMSK(0x422c80UL, BIT(0))==BIT(0)?"On":"Off");
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC memory size(L)                : %d\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_MEMC_MemSize);
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC memory size(R)                : %d\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_R2_MemSize);
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC memory size(PQ)               : %d\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_ME1_MemSize);
    _phy_to_miu_offset(u8sel, u8temp, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_MEMC_MemAddr);
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC MIU number(L)                 : MIU%d\n", u8sel);
    _phy_to_miu_offset(u8sel, u8temp, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_R2_MemAddr);
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC MIU number(R)                 : MIU%d\n", u8sel);
    _phy_to_miu_offset(u8sel, u8temp, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_ME1_MemAddr);
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC MIU number(PQ)                : MIU%d\n", u8sel);
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_DUAL_MIU)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"MEMC dual miu                      : TRUE \n");
    }
    else
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"MEMC dual miu                      : FALSE \n");
    }
    MdbPrint(paraGetInfo->u64ReqHdl,"MEMC in/out frame rate             : %d:%d \n", pXCResourceStr->pTimingInfo.u16InputVFreq, gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC);
}

static void _XCMdbloctl_Cat_FrameLock(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    XC_REGS_SAVE_AREA *pXCResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pXCResourceStr);

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler Frame Lock Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"H total                            : %d\n", MApi_XC_R2BYTE(REG_SC_BK10_0C_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"V total                            : %d\n", MApi_XC_R2BYTE(REG_SC_BK10_0D_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"Input frame rate                   : %d\n", pXCResourceStr->pTimingInfo.u16InputVFreq);
    MdbPrint(paraGetInfo->u64ReqHdl,"Output frame rate                  : %d\n", gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC);
    MdbPrint(paraGetInfo->u64ReqHdl,"Lock ivs:ovs                       : %d:%d\n", R2BYTEMSK(0x103118UL,0x0F00)>>8, R2BYTEMSK(0x103118UL,0xF000)>>11);
    if(R2BYTEMSK(0x1031E0UL,BIT(15))==BIT(15))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Lock mode                          : lockvfreq\n");
    }
    else if(R2BYTEMSK(0x1031F0UL,BIT(12))==BIT(12))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Lock mode                          : ivs_ovs\n");
    }
    else if(R2BYTEMSK(0x1031FCUL,BIT(15))==BIT(15))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Lock mode                          : spread\n");
    }
    else
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Lock mode                          : lockphase\n");
    }
}
static void _XCMdbloctl_Cat_SetWindow(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U32 u32address;
    MS_U8 u8temp = 0, u8sel;

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler Setwindow Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Capture window                     : (%d,%d) %dx%d\n", gSrcInfo[MAIN_WINDOW].stCapWin.x, gSrcInfo[MAIN_WINDOW].stCapWin.y, gSrcInfo[MAIN_WINDOW].stCapWin.width, gSrcInfo[MAIN_WINDOW].stCapWin.height);
    MdbPrint(paraGetInfo->u64ReqHdl,"Crop window                        : (%d,%d) %dx%d\n", gSrcInfo[MAIN_WINDOW].stCropWin.x, gSrcInfo[MAIN_WINDOW].stCropWin.y, gSrcInfo[MAIN_WINDOW].stCropWin.width, gSrcInfo[MAIN_WINDOW].stCropWin.height);
    MdbPrint(paraGetInfo->u64ReqHdl,"Display window                     : (%d,%d) %dx%d\n", gSrcInfo[MAIN_WINDOW].stDispWin.x, gSrcInfo[MAIN_WINDOW].stDispWin.y, gSrcInfo[MAIN_WINDOW].stDispWin.width, gSrcInfo[MAIN_WINDOW].stDispWin.height);
    MdbPrint(paraGetInfo->u64ReqHdl,"Pre-H cus scaling                  : %s (%d->%d)\n", (gSrcInfo[MAIN_WINDOW].Status2.bPreHCusScaling==TRUE)?"TRUE":"FALSE", gSrcInfo[MAIN_WINDOW].Status2.u16PreHCusScalingSrc, gSrcInfo[MAIN_WINDOW].Status2.u16PreHCusScalingDst);
    MdbPrint(paraGetInfo->u64ReqHdl,"Pre-V cus scaling                  : %s (%d->%d)\n", (gSrcInfo[MAIN_WINDOW].Status2.bPreVCusScaling==TRUE)?"TRUE":"FALSE", gSrcInfo[MAIN_WINDOW].Status2.u16PreVCusScalingSrc, gSrcInfo[MAIN_WINDOW].Status2.u16PreVCusScalingDst);
    MdbPrint(paraGetInfo->u64ReqHdl,"Post-H cus scaling                 : %s (%d->%d)\n", (gSrcInfo[MAIN_WINDOW].bHCusScaling==TRUE)?"TRUE":"FALSE", gSrcInfo[MAIN_WINDOW].u16HCusScalingSrc, gSrcInfo[MAIN_WINDOW].u16HCusScalingDst);
    MdbPrint(paraGetInfo->u64ReqHdl,"Post-V cus scaling                 : %s (%d->%d)\n", (gSrcInfo[MAIN_WINDOW].bVCusScaling==TRUE)?"TRUE":"FALSE", gSrcInfo[MAIN_WINDOW].u16VCusScalingSrc, gSrcInfo[MAIN_WINDOW].u16VCusScalingDst);
#if defined (__aarch64__)
    MdbPrint(paraGetInfo->u64ReqHdl,"Memory size                        : %x\n", pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size);
#else
    MdbPrint(paraGetInfo->u64ReqHdl,"Memory size                        : %tx\n", (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size);
#endif
    MdbPrint(paraGetInfo->u64ReqHdl,"Frame count                        : %d\n", MApi_XC_R2BYTEMSK(REG_SC_BK12_19_L, 0x000F));
    MdbPrint(paraGetInfo->u64ReqHdl,"RW diff count                      : %d\n", (MApi_XC_R2BYTEMSK(REG_SC_BK12_07_L, 0xE000)>>13));
    MdbPrint(paraGetInfo->u64ReqHdl,"2-steps scaling status             : %s\n", (MApi_XC_IsSupport2StepScaling()==TRUE)?"TRUE":"FALSE");
    u32address = (MS_U32)((MApi_XC_R2BYTE(REG_SC_BK12_09_L))<<16) | (MS_U32)(MApi_XC_R2BYTE(REG_SC_BK12_08_L));
    _phy_to_miu_offset(u8sel, u8temp, u32address);
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler MIU number                  : MIU%d\n", u8sel);
    MdbPrint(paraGetInfo->u64ReqHdl,"Memory IPM address                 : 0x%lx\n", u32address);
    u32address = (MS_U32)((MApi_XC_R2BYTE(REG_SC_BK12_11_L))<<16) | (MS_U32)(MApi_XC_R2BYTE(REG_SC_BK12_10_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"Memory OPM address                 : 0x%lx\n", u32address);
    MdbPrint(paraGetInfo->u64ReqHdl,"IPM Offset                         : %d\n", MApi_XC_R2BYTE(REG_SC_BK12_0E_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"IPM Fetch                          : %d\n", MApi_XC_R2BYTE(REG_SC_BK12_0F_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"OPM Offset                         : %d\n", MApi_XC_R2BYTE(REG_SC_BK12_16_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"OPM Fetch                          : %d\n", MApi_XC_R2BYTE(REG_SC_BK12_17_L));
    MdbPrint(paraGetInfo->u64ReqHdl,"Input DE H size                    : %d\n", Hal_SC_ip_de_hend_info(pInstance, MAIN_WINDOW)-Hal_SC_ip_de_hstart_info(pInstance, MAIN_WINDOW)+1);
    MdbPrint(paraGetInfo->u64ReqHdl,"Input DE V size                    : %d\n", Hal_SC_ip_de_vend_info(pInstance, MAIN_WINDOW)-Hal_SC_ip_de_vstart_info(pInstance, MAIN_WINDOW)+1);
}

static void _XCMdbloctl_Cat_PQ(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{/*
    void *pInstance, *pModule;
    pInstance = UtopiaModuleGetLocalInstantList(MODULE_ACE, pInstance);
    UtopiaInstanceGetModule(pInstance, &pModule);
    ACE_REGS_SAVE_AREA *pACEResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pACEResourceStr);*/

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar Scaler PQ Info -----------------\n");
    //MdbPrint(paraGetInfo->u64ReqHdl,"Brightness_R : %d, \n", pACEResourceStr->u8Brightness_R);
    //MdbPrint(paraGetInfo->u64ReqHdl,"Brightness_G : %d, \n", pACEResourceStr->u8Brightness_G);
    //MdbPrint(paraGetInfo->u64ReqHdl,"Brightness_B : %d, \n", pACEResourceStr->u8Brightness_B);
    //MdbPrint(paraGetInfo->u64ReqHdl,"Contrast : %d\n", pACEResourceStr->u8Contrast);
    //MdbPrint(paraGetInfo->u64ReqHdl,"Saturation  : %d\n", pACEResourceStr->u8Saturation);
    //MdbPrint(paraGetInfo->u64ReqHdl,"Hue : %d\n", pACEResourceStr->u8Hue);
    MdbPrint(paraGetInfo->u64ReqHdl,"UCNR status                        : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK2A_02_L,BIT(15))==BIT(15))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Compress mode                      : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK08_01_L,BIT(0))==0)?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"DNR status                         : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK12_02_L,BIT(14))==BIT(14))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"DLC status                         : %s\n", (R2BYTEMSK(0x141A08UL,BIT(7))==BIT(7))?"TRUE":"FALSE");
    //MdbPrint(paraGetInfo->u64ReqHdl,"DLC window                         : \n");
    //MdbPrint(paraGetInfo->u64ReqHdl,"PQ Source type                     : \n");
    //MdbPrint(paraGetInfo->u64ReqHdl,"Gamma index                        : \n");
    MdbPrint(paraGetInfo->u64ReqHdl,"PQ Gamma Enable                    : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK10_50_L,BIT(0))==BIT(0))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Panel Gamma Enable                 : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK24_26_L,BIT(3))==BIT(3))?"TRUE":"FALSE");
}

static void _XCMdbloctl_Cat_DISP(void *pInstance, MDBCMD_GETINFO_PARAMETER *paraGetInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    XC_REGS_SAVE_AREA *pXCResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pXCResourceStr);

    MdbPrint(paraGetInfo->u64ReqHdl,"----------------  Mstar DISP In Out Info -----------------\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Window enable                      : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK20_10_L,BIT(0))==BIT(0))?"TRUE":"FALSE"); //main\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Mute enable                        : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK10_19_L, BIT(1))==BIT(1))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Output resolution                  : %d_%d_%d_P\n", gSrcInfo[MAIN_WINDOW].stDispWin.width, gSrcInfo[MAIN_WINDOW].stDispWin.height, gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC);
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_10BIT_4LANE ||
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_8BIT_4LANE)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Output type                        : VB1_4lane\n");
    }
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_10BIT_8LANE ||
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_8BIT_8LANE)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Output type                        : VB1_8lane\n");
    }
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_10BIT_2LANE ||
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_VBY1_8BIT_2LANE)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Output type                        : VB1_2lane\n");
    }
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_LVDS)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Output type                        : LVDS\n");
    }

    if(IsSrcTypeHDMI(gSrcInfo[0].enInputSourceType))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Input source type                  : HDMI\n");
    }
    else if(IsSrcTypeATV(gSrcInfo[0].enInputSourceType))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Input source type                  : ATV\n");
    }
    else if(IsSrcTypeDTV(gSrcInfo[0].enInputSourceType))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Input source type                  : DTV\n");
    }
    else if(IsSrcTypeStorage(gSrcInfo[0].enInputSourceType))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Input source type                  : MM\n");
    }
    else if(IsSrcTypeVga(gSrcInfo[0].enInputSourceType))
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Input source type                  : VGA\n");
    }
    MdbPrint(paraGetInfo->u64ReqHdl,"Source resolution                  : %d_%d_%d_%c\n", gSrcInfo[MAIN_WINDOW].stCapWin.width, gSrcInfo[MAIN_WINDOW].stCapWin.height
        , pXCResourceStr->pTimingInfo.u16InputVFreq, (gSrcInfo[MAIN_WINDOW].bInterlace==TRUE)?'I':'P');
    MdbPrint(paraGetInfo->u64ReqHdl,"Input I/P mode status              : %s\n", (gSrcInfo[MAIN_WINDOW].bInterlace==TRUE)?"Interface":"Progressive");
    MdbPrint(paraGetInfo->u64ReqHdl,"Memory color space                 : %s\n", (gSrcInfo[MAIN_WINDOW].bUseYUVSpace==TRUE)?"YUV":"RGB");
    MdbPrint(paraGetInfo->u64ReqHdl,"Display size                       : %d_%d\n", MApi_XC_R2BYTE(REG_SC_BK10_09_L)-MApi_XC_R2BYTE(REG_SC_BK10_08_L)+1,
        MApi_XC_R2BYTE(REG_SC_BK10_0B_L)-MApi_XC_R2BYTE(REG_SC_BK10_0A_L)+1);
    MdbPrint(paraGetInfo->u64ReqHdl,"Mirror mode                        : %s\n", (gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode == TRUE)?"TRUE":"FALSE");
    if(R2BYTE(0x103154UL) == 0xF4 || R2BYTE(0x103154UL) == 0x1F4)
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Frame lock status                  : TURE\n");
    }
    else
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"Frame lock status                  : FALSE\n");
    }
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler dual miu                    : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK11_29_L,BIT(15))==BIT(15))?"TRUE":"FALSE");
#if (XC_SUPPORT_CMA == TRUE)
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler CMA                         : TRUE\n");
#else
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler CMA                         : FALSE\n");
#endif
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler auto no signal status       : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK01_02_L,BIT(7))==BIT(7))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"DS status                          : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK1F_11_L,BIT(9))==BIT(9) && MApi_XC_R2BYTEMSK(REG_SC_BK1F_11_L,BIT(11))==BIT(11))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Menuload status                    : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK1F_02_L,BIT(15))==BIT(15))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"2P mode status                     : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK01_07_L,BIT(15))==BIT(15))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"HSK mode status                    : %s\n", (gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode==TRUE)?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Window type                        : Main Window\n");
    MdbPrint(paraGetInfo->u64ReqHdl,"Freeze                             : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK12_01_L,BIT(11))==BIT(11))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Offline detect                     : %s\n", (MApi_XC_R2BYTEMSK(0x257EUL,BIT(1))==0)?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Setwindow time                     : %dus\n", u32SetWindowTime);
#ifdef UFO_XC_FB_LEVEL
    if(gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL || gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI)
#else
    if(gSrcInfo[MAIN_WINDOW].bFBL == TRUE || gSrcInfo[MAIN_WINDOW].bR_FBL == TRUE)
#endif
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"RFBL/FBL                           : TRUE\n");
    }
    else
    {
        MdbPrint(paraGetInfo->u64ReqHdl,"RFBL/FBL                           : FALSE\n");
    }
    MdbPrint(paraGetInfo->u64ReqHdl,"Local dimming                      : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK2E_01_L,BIT(0))==BIT(0))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Disable input source               : %s\n", (MApi_XC_R2BYTEMSK(REG_SC_BK01_02_L,BIT(7))==BIT(7))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler IRQ status (Bit 4)          : %s (OP Vsync)\n", (MApi_XC_R2BYTEMSK(REG_SC_BK00_14_L,BIT(4))==BIT(4))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler IRQ status (Bit 11)         : %s (IP1 Vsync)\n", (MApi_XC_R2BYTEMSK(REG_SC_BK00_14_L,BIT(11))==BIT(11))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler IRQ status (Bit 21)         : %s (HDR DMA)\n", (MApi_XC_R2BYTEMSK(REG_SC_BK00_15_L,BIT(5))==BIT(5))?"TRUE":"FALSE");
    MdbPrint(paraGetInfo->u64ReqHdl,"Scaler IRQ status (Bit 22)         : %s (HDR Descramble)\n", (MApi_XC_R2BYTEMSK(REG_SC_BK00_15_L,BIT(6))==BIT(6))?"TRUE":"FALSE");

}

static void _XCMdbloctl_TestPattern(MS_U8 u8IP, MS_BOOL bEnable, MS_U8 u8window)
{
    if(u8IP == 0)
    {
        XC_SET_IPMUX_TESTPATTERN_t ipmux_test_pattern;
        ipmux_test_pattern.bEnable = bEnable;
        ipmux_test_pattern.u16R_CR_Data = 32;
        ipmux_test_pattern.u16G_Y_Data  = 32;
        ipmux_test_pattern.u16B_CB_Data = 32;
        MApi_XC_GenerateTestPattern(E_XC_IPMUX_PATTERN_MODE,(void *)&ipmux_test_pattern,sizeof(XC_SET_IPMUX_TESTPATTERN_t));
    }
    else if(u8IP == 1)
    {
        XC_SET_IP1_TESTPATTERN_t ip1_test_pattern;
        ip1_test_pattern.u16Enable = bEnable;
        ip1_test_pattern.u32Pattern_type = 32;
        ip1_test_pattern.eWindow = (u8window==0)?MAIN_WINDOW:SUB_WINDOW;
        MApi_XC_GenerateTestPattern(E_XC_IP1_PATTERN_MODE,(void*)&ip1_test_pattern,sizeof(XC_SET_IP1_TESTPATTERN_t));
    }
    else if(u8IP == 2)
    {
        XC_SET_OP_TESTPATTERN_t op_test_pattern;
        op_test_pattern.bMiuLineBuff = bEnable;
        op_test_pattern.bLineBuffHVSP = FALSE;
        MApi_XC_GenerateTestPattern(E_XC_OP_PATTERN_MODE,(void*)&op_test_pattern,sizeof(XC_SET_OP_TESTPATTERN_t));
    }
    else if(u8IP == 3)
    {
        XC_SET_OP_TESTPATTERN_t op_test_pattern;
        op_test_pattern.bMiuLineBuff = FALSE;
        op_test_pattern.bLineBuffHVSP = bEnable;
        MApi_XC_GenerateTestPattern(E_XC_OP_PATTERN_MODE,(void*)&op_test_pattern,sizeof(XC_SET_OP_TESTPATTERN_t));
    }
    else if(u8IP == 5)
    {
        XC_SET_VOP_TESTPATTERN_t vop_test_pattern;
        vop_test_pattern.bEnable = bEnable;
        MApi_XC_GenerateTestPattern(E_XC_VOP_PATTERN_MODE,(void*)&vop_test_pattern,sizeof(XC_SET_VOP_TESTPATTERN_t));
    }
    else if(u8IP == 4)
    {
        XC_SET_VOP2_TESTPATTERN_t vop2_test_pattern;
        vop2_test_pattern.bEnable = bEnable;
        vop2_test_pattern.u16B_CB_Data = 32;
        vop2_test_pattern.u16G_Y_Data= 32;
        vop2_test_pattern.u16R_CR_Data= 32;
        MApi_XC_GenerateTestPattern(E_XC_VOP2_PATTERN_MODE,(void*)&vop2_test_pattern,sizeof(XC_SET_VOP2_TESTPATTERN_t));
    }
    else if(u8IP == 6)
    {
        XC_SET_MOD_TESTPATTERN_t mod_test_pattern;
        mod_test_pattern.bEnable = bEnable;
        mod_test_pattern.u16B_CB_Data = 32;
        mod_test_pattern.u16G_Y_Data= 32;
        mod_test_pattern.u16R_CR_Data= 32;
        MApi_XC_GenerateTestPattern(E_XC_MOD_PATTERN_MODE,(void*)&mod_test_pattern,sizeof(XC_SET_MOD_TESTPATTERN_t));
    }
}

static void _XCMdbIoctl_EchoCmd(char *pcCmdLine)
{
    char pch[] = " =";
            char* psep;
            psep = strsep(&pcCmdLine,pch);

            if(strcmp("pattern", psep) == 0)
            {
                MS_U8 u8IP, u8window;
                MS_BOOL bEnable;
                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                u8IP = (MS_U8)atoi(psep);

                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                bEnable = (MS_BOOL)atoi(psep);

                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                psep = strsep(&pcCmdLine,pch);
                if(psep == NULL)
                {
                    return;
                }
                u8window = (MS_U8)atoi(psep);

                _XCMdbloctl_TestPattern(u8IP, bEnable, u8window);
            }
}

MS_U32 XCMdbIoctl(MS_U32 cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;
    switch(cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            /*MdbPrint(paraCmdLine->u64ReqHdl,"LINE:%d, MDBCMD_CMDLINE\n", __LINE__);
            MdbPrint(paraCmdLine->u64ReqHdl,"u32CmdSize: %d\n", paraCmdLine->u32CmdSize);
            MdbPrint(paraCmdLine->u64ReqHdl,"pcCmdLine: %s\n", paraCmdLine->pcCmdLine);*/
            _XCMdbIoctl_EchoCmd(paraCmdLine->pcCmdLine);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            break;
        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            //MdbPrint(paraGetInfo->u64ReqHdl,"LINE:%d, MDBCMD_GETINFO\n", __LINE__);
            void *pInstance;
            pInstance = UtopiaModuleGetLocalInstantList(MODULE_XC, pInstance);
            _XCMdbloctl_Cat_DISP(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_PQ(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_SetWindow(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_FrameLock(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_MEMC(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_HDR(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_TGEN(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_3D(pInstance, paraGetInfo);
            _XCMdbloctl_Cat_OPTEE(pInstance, paraGetInfo);
            paraGetInfo->result = MDBRESULT_SUCCESS_FIN;
            break;
        default:
            MdbPrint(paraGetInfo->u64ReqHdl,"unknown cmd\n", __LINE__);
            break;
    }
    return 0;
}
#endif


void XCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 u32ResourceStatusCheck[E_XC_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};

    // 1. deal with module
    UtopiaModuleCreate(MODULE_XC, 0, &pModuleXC);
    UtopiaModuleRegister(pModuleXC);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleXC, (FUtopiaOpen)ModuleType, (FUtopiaClose)XCClose, (FUtopiaIOctl)XCIoctl);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("xc", (FUtopiaMdbIoctl)XCMdbIoctl);
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleXC, (FUtopiaSTR)XCStr);
    UtopiaModuleSetSTRPrivate(pModuleXC, sizeof(XC_REGS_SAVE_AREA));
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if (!bIsXCRegistered)
    {
        g_pDevice0Instance = NULL;
        g_pDevice1Instance = NULL;
        bIsXCRegistered = TRUE;
    }
#endif
    // 3. resource can allocate private for internal use
    MS_U8 u8DeviceIndex = 0;
    for(u8DeviceIndex = 0; u8DeviceIndex < MAX_XC_DEVICE_NUM ; u8DeviceIndex++)
    {
        MS_U8 u8DeviceOffset = (E_XC_POOL_ID_NUM*u8DeviceIndex);
        void* psResource = NULL;

        UtopiaModuleAddResourceStart(pModuleXC, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_MENULOAD);
        if(u8DeviceIndex == 0)
        {
            u32ResourceStatusCheck[u8DeviceOffset + E_XC_POOL_ID_INTERNAL_MENULOAD] = UtopiaResourceCreate("MENULOAD", sizeof(MLOAD_RESOURCE_PRIVATE), &psResource);
        }
        else
        {
            char u8ResName[XC_RES_NAME_MAX_LEN];
            memset(&u8ResName, 0x0, sizeof(u8ResName));
            snprintf(u8ResName, XC_RES_NAME_MAX_LEN, "MENULOAD_%d", u8DeviceIndex);
            u32ResourceStatusCheck[u8DeviceOffset + E_XC_POOL_ID_INTERNAL_MENULOAD] = UtopiaResourceCreate(u8ResName, sizeof(MLOAD_RESOURCE_PRIVATE), &psResource);
        }
        UtopiaResourceRegister(pModuleXC, psResource, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_MENULOAD);
        UtopiaModuleAddResourceEnd(pModuleXC, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_MENULOAD);

        UtopiaModuleAddResourceStart(pModuleXC, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_REGISTER);
        if(u8DeviceIndex == 0)
        {
            u32ResourceStatusCheck[u8DeviceOffset + E_XC_POOL_ID_INTERNAL_REGISTER] = UtopiaResourceCreate("XC_REGISTER", 0, &psResource);
        }
        else
        {
            char u8ResName[XC_RES_NAME_MAX_LEN];
            memset(&u8ResName, 0x0, sizeof(u8ResName));
            snprintf(u8ResName, XC_RES_NAME_MAX_LEN, "XC_REGISTER_%d", u8DeviceIndex);
            u32ResourceStatusCheck[u8DeviceOffset + E_XC_POOL_ID_INTERNAL_REGISTER] = UtopiaResourceCreate(u8ResName, 0, &psResource);
        }
        UtopiaResourceRegister(pModuleXC, psResource, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_REGISTER);
        UtopiaModuleAddResourceEnd(pModuleXC, u8DeviceOffset + E_XC_POOL_ID_INTERNAL_REGISTER);

        UtopiaModuleAddResourceStart(pModuleXC, _XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex));
        if(u8DeviceIndex == 0)
        {
#ifdef MSOS_TYPE_OPTEE
            u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] = UtopiaResourceCreate("XC_VARIABLE", sizeof(XC_RESOURCE_PRIVATE_FOR_TEE), &psResource);
#else
            u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] = UtopiaResourceCreate("XC_VARIABLE", sizeof(XC_RESOURCE_PRIVATE), &psResource);
#endif
        }
        else
        {
            char u8ResName[XC_RES_NAME_MAX_LEN];
            memset(&u8ResName, 0x0, sizeof(u8ResName));
            snprintf(u8ResName, XC_RES_NAME_MAX_LEN, "XC_VARIABLE_%d", u8DeviceIndex);
#ifdef MSOS_TYPE_OPTEE
            u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] = UtopiaResourceCreate(u8ResName, sizeof(XC_RESOURCE_PRIVATE_FOR_TEE), &psResource);
#else
            u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] = UtopiaResourceCreate(u8ResName, sizeof(XC_RESOURCE_PRIVATE), &psResource);
#endif
        }
        UtopiaResourceRegister(pModuleXC, psResource, _XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex));
        UtopiaModuleAddResourceEnd(pModuleXC, _XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex));

        //4. init resource private members here   (aka, global variable)
#ifdef MSOS_TYPE_OPTEE
        XC_RESOURCE_PRIVATE_FOR_TEE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(psResource,(void**)(&pXCResourcePrivate));
        //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
        if (u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] == UTOPIA_STATUS_SHM_EXIST)
        {
            // do nothing, since it is already inited
        }
        else
        {
            memset(&(pXCResourcePrivate->sthal_Optee),0,sizeof(ST_HAL_OPTEE));
        }
#else
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(psResource,(void**)(&pXCResourcePrivate));

        //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
        if (u32ResourceStatusCheck[_XC_SELECT_INTERNAL_VARIABLE(u8DeviceIndex)] == UTOPIA_STATUS_SHM_EXIST)
        {
            // do nothing, since it is already inited
        }
        else
        {
            // Init flow control related variables here. Other global variable should be
            // inited in each of its init function relatively.
            pXCResourcePrivate->bResourceRegistered = TRUE;
            pXCResourcePrivate->bXCMWEStatus = FALSE;

            // apiXC_Auto.c
            pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = FALSE;
            pXCResourcePrivate->stapiXC_Auto._u8ThresholdValue = 0x05;
            pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode = E_XC_SW_CALIBRATION;

            // apiXC_PCMonitor.c
            pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[SUB_WINDOW] = 0;
            pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[SUB_WINDOW] = 0;
            pXCResourcePrivate->stapiXC_PCMonitor._bNewModeInterlacedDetect = FALSE;

            // mdrv_hdmi.c
            pXCResourcePrivate->stdrv_HDMI._ResetDone = FALSE;
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done = FALSE;
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count = FALSE;
            pXCResourcePrivate->stdrv_HDMI.u32VsyncTimer = 0;
            pXCResourcePrivate->stdrv_HDMI._prev_enPowerState = E_POWER_MECHANICAL;

#if (LD_ENABLE==1)
            // mdrv_ld.c
            pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished = FALSE;
            pXCResourcePrivate->stdrv_LD._u16LedPos = 0;

            // mdrv_ldalgo.c
            pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio_3D = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Test = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Iluma_Pre = 0;
            pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero = 0;
            pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = 0;
            pXCResourcePrivate->stdrv_LDALGO._u8OSDEnable = 1;
            pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num = LD_BLOCK_DEFALUT;
            pXCResourcePrivate->stdrv_LDALGO._bLDoff = TRUE;
#endif

            // mdrv_sc_3d.c
#if HW_2DTO3D_SUPPORT
            pXCResourcePrivate->stdrvXC_3D.bAdjustDNRSize = FALSE;
            pXCResourcePrivate->stdrvXC_3D.u32OriginalDNRSize = 0;
#endif
            pXCResourcePrivate->stdrvXC_3D.s_u8flg = 0;
            pXCResourcePrivate->stdrvXC_3D.s_u8Ptr = 0;
            pXCResourcePrivate->stdrvXC_3D.u8Ptr = 0;
            pXCResourcePrivate->stdrvXC_3D.bFirstFlag = TRUE;
            pXCResourcePrivate->stdrvXC_3D.ePreInputMode = E_XC_3D_INPUT_MODE_NONE;
            pXCResourcePrivate->stdrvXC_3D.ePreOutputMode = E_XC_3D_OUTPUT_MODE_NONE;

#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 3) //new 2d to 3d architecture
            pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei = 0;
            pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei = 0;//instead of TpBGCOLOR_Valid
            pXCResourcePrivate->stdrvXC_3D._s16DnBGCOLOR_ValidWei = 0;//instead of DnBGCOLOR_Valid
            pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei = 0;// for blue sky ratio
            pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei_Bottom = 0;// for blue sky ratio
            pXCResourcePrivate->stdrvXC_3D._s16TBA_BGColorWei = 0;
            pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorWei = 0;
            pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt = 0;
            pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt = 0;
            pXCResourcePrivate->stdrvXC_3D._s16BlueSkyRatio = 0;

            pXCResourcePrivate->stdrvXC_3D._u16Y_Sep = 0;        // horizontal detection
            pXCResourcePrivate->stdrvXC_3D._u16Y_Sep_OUT = 0;    // horizontal detection for output frame position

            pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueAndFlatBlkCnt = 0;
            pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueAndFlatBlkCnt_Dn = 0;

            //keeps blueSkyRatio stable
            pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1 = 0;
            pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2 = 0;

            //for blkbased Depth
            pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En = 1; // 1: 3x3 2: 5x5  3:7x7  4:disable bilinear
            pXCResourcePrivate->stdrvXC_3D._u8Bilateral_En = 0;
            pXCResourcePrivate->stdrvXC_3D._u8DepthAB = 0; //1:TableA 2:TableB 3:ABweight
            pXCResourcePrivate->stdrvXC_3D._u8Bilinear_En = 1;
            pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3 = 0;

            //multiview adjust dummy
            pXCResourcePrivate->stdrvXC_3D._s8Shift_Value = 40;
            pXCResourcePrivate->stdrvXC_3D._s8Clamp_Value = 28;
            pXCResourcePrivate->stdrvXC_3D._s8Multi_Gain = 16;
            pXCResourcePrivate->stdrvXC_3D._s32Depth_Control_Var_IIR = 16;
            pXCResourcePrivate->stdrvXC_3D._s32Mean_Depth_IIR = 128;
            pXCResourcePrivate->stdrvXC_3D._u8Horizontal_Step = 32;
            pXCResourcePrivate->stdrvXC_3D._u8Vertical_Step = 32;

            //for IIR
            pXCResourcePrivate->stdrvXC_3D.u8IIR_En = 1;
            pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight = 96;
            pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1 = 0;
            pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2 = 0;
            pXCResourcePrivate->stdrvXC_3D.u16Pre_YSep = 0;

            pXCResourcePrivate->stdrvXC_3D.s_bFirst = TRUE;

            //for DepthDectionBLK
            //pXCResourcePrivate->stdrvXC_3D.u8BLKblueSkyRatio1 = 0;
            //pXCResourcePrivate->stdrvXC_3D.u8BLKblueSkyRatio2 = 0;
            //for Trend Detection
            pXCResourcePrivate->stdrvXC_3D.u8VerGLB_Wei =0;
            //for DEBUGFn
            pXCResourcePrivate->stdrvXC_3D.u8DBG_En = 0;
            pXCResourcePrivate->stdrvXC_3D.u8Round = 0;
            //for job-division of LUT update
            pXCResourcePrivate->stdrvXC_3D.job_id = 0;
            pXCResourcePrivate->stdrvXC_3D.job_freq = 0;
            pXCResourcePrivate->stdrvXC_3D.updateSRAM_state = 1;
            pXCResourcePrivate->stdrvXC_3D.updateSRAM_PreState = 3;
            pXCResourcePrivate->stdrvXC_3D.NoMot_Cnt = 0;
            pXCResourcePrivate->stdrvXC_3D.NoMot_Flg = 0;
            //BGColorDetection
            pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp = 0;
            pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn = 0;
            pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp = 128;
            pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp = 128;
            pXCResourcePrivate->stdrvXC_3D.BGColor_CbBn = 128;
            pXCResourcePrivate->stdrvXC_3D.BGColor_CrBn = 128;
            pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforTp = 0;
            pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforBn = 0;
            pXCResourcePrivate->stdrvXC_3D._u8TBABGColorWei = 0;
            //for MainObj Detection
            pXCResourcePrivate->stdrvXC_3D._u8MainObjWei = 0;
            //for LGE ver GLOBAL depth
            pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1 = 0;
            pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2 = 0;

            //for TrendDetection
            pXCResourcePrivate->stdrvXC_3D.u8VerGLB_Wei = 0;
            //for DEBUGFn
            pXCResourcePrivate->stdrvXC_3D.u8DBG_En = 0;

            //for Size related settings
            pXCResourcePrivate->stdrvXC_3D._u16PanelWidth = 0;
            pXCResourcePrivate->stdrvXC_3D._u16PanelHeight = 0;
            pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_H = 0;
            pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_V = 0;
#endif
#endif

#if HW_2DTO3D_SUPPORT
            pXCResourcePrivate->stdrvXC_3D._u32HisWeight = 0;
            pXCResourcePrivate->stdrvXC_3D._u32ArtWeight = 0;
            pXCResourcePrivate->stdrvXC_3D._u32ReturnWeightCur = 0;
            pXCResourcePrivate->stdrvXC_3D._u32ReturnWeight = 0;
            pXCResourcePrivate->stdrvXC_3D._u32X = 0xe8c9af37;
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurSum = 0;
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseSum = 0;
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur = 0;
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse = 0;
            pXCResourcePrivate->stdrvXC_3D._u8RemapAvgBias = 0;
            pXCResourcePrivate->stdrvXC_3D._u16SBSCount = 0;
            pXCResourcePrivate->stdrvXC_3D._u16TBCount = 0;
            pXCResourcePrivate->stdrvXC_3D._u162DCount = 0;
            pXCResourcePrivate->stdrvXC_3D._u16TotalCount = 0;
#endif

            // mdrv_sc_display.c
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
            pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_IDLE_STATE;
            pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bForceFreerun = FALSE;
            pXCResourcePrivate->stdrvXC_Display._ForceVFreq = VFREQ_FROM_PANEL;
            pXCResourcePrivate->stdrvXC_Display._bHSyncChanged = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bVSyncChanged = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR = FALSE;
            pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = 0;
            pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM = FALSE;
            pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce=0;
            pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce=0;
            pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable  = FALSE;
            pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
            pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Display.s_u32DelayT = 0;
            pXCResourcePrivate->stdrvXC_Display.s_u32InputVsyncStart = 0;
            pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd = 0xFFFF;         // max PRD error
            pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase = 0xFFFF;             // max phase error
            pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = 0;
            pXCResourcePrivate->stdrvXC_Display.g_u8FrcIn = 0;
            pXCResourcePrivate->stdrvXC_Display.g_u8FrcOut = 0;

            // mdrv_sc_dynamicscaling.c
            pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[SUB_WINDOW] = FALSE;

            // mdrv_sc_ip.c
            pXCResourcePrivate->stdrvXC_IP.u8Count = 1;
#if ENABLE_DEBUG_CAPTURE_N_FRAMES
            pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR = FALSE;
            pXCResourcePrivate->stdrvXC_IP.u8Cnt = 0;
#endif

            // mdrv_sc_scaling.c
            pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna = FALSE;
            pXCResourcePrivate->stdrvXC_Scaling._bVAutoFitPrescalingEna = FALSE;
            pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = FALSE;
            pXCResourcePrivate->stdrvXC_Scaling.u32DNRBaseOffset[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling.u32DNRBaseOffset[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._au8StoreFrameNumFactor[0] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._au8StoreFrameNumFactor[1] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._bMainWinSkipWaitOutputVsync = FALSE;
            pXCResourcePrivate->stdrvXC_Scaling._bSubWinSkipWaitOutputVsync = FALSE;
            pXCResourcePrivate->stdrvXC_Scaling._bPQSetHPreScalingratio = FALSE;
#ifdef ENABLE_SCALING_WO_MUTE
            pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[MAIN_WINDOW] = 1;
            pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[SUB_WINDOW] = 1;
#endif
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16HScaleSrc[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16HScaleSrc[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16HScaleDst[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16HScaleDst[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16VScaleSrc[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16VScaleSrc[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16VScaleDst[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u16VScaleDst[SUB_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 0;

            // mvideo.c
            pXCResourcePrivate->stdrvXC_MVideo.bNeedInitShared = TRUE;
            memset(&(pXCResourcePrivate->stdrvXC_MVideo._SContext), 0, sizeof(XC_Context));
            memset(&(pXCResourcePrivate->stdrvXC_MVideo._SCShared), 0, sizeof(XC_Context_Shared));
            pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[MAIN_WINDOW] = E_MS_XC_MEM_FMT_AUTO;
            pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[SUB_WINDOW] = E_MS_XC_MEM_FMT_AUTO;
            gSrcInfo[MAIN_WINDOW].Status2.stXCConfigCMA[CMA_XC_SELF_MEM].u32HeapID = XC_INVALID_HEAP_ID;
            gSrcInfo[SUB_WINDOW].Status2.stXCConfigCMA[CMA_XC_SELF_MEM].u32HeapID = XC_INVALID_HEAP_ID;
            gSrcInfo[MAIN_WINDOW].Status2.stXCConfigCMA[CMA_XC_COBUFF_MEM].u32HeapID = XC_INVALID_HEAP_ID;
            gSrcInfo[SUB_WINDOW].Status2.stXCConfigCMA[CMA_XC_COBUFF_MEM].u32HeapID = XC_INVALID_HEAP_ID;
            pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[MAIN_WINDOW] = INPUT_SOURCE_NONE;
            pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[SUB_WINDOW] = INPUT_SOURCE_NONE;
            pXCResourcePrivate->stdrvXC_MVideo._bSkipSWReset = FALSE;   ///< Flag for: If the system needs to show Logo from MBoot layer to APP layer without flashing. \n We bypass SW reset in MApi_XC_Init.
            pXCResourcePrivate->stdrvXC_MVideo._bDFBInit = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift = 0;
            pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift = 0;
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualMainWinDone = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo._bSetDualSubWinDone = FALSE;

#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
            pXCResourcePrivate->stdrvXC_MVideo.bIsHWPatternEnabled = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bIsMute = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bIsInputSrcDisabled = FALSE;
#endif
#endif

#if SUPPORT_OP2_TEST_PATTERN
            pXCResourcePrivate->stdrvXC_MVideo.u16OP2_color_3x3 = 0;
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2TestPattern= FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2InputSrcDisabled = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bIsOP2Mute= FALSE;
#endif
#if FRC_INSIDE
            pXCResourcePrivate->stdrvXC_MVideo.bEnableUserMode = FALSE;
#endif

            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableLockFreqOnly = TRUE;

#if (ENABLE_NONSTD_INPUT_MCNR==1)
            pXCResourcePrivate->stdrvXC_MVideo.bIsUnStdSignal = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.u8StdSignalStbCnt = 0;
            pXCResourcePrivate->stdrvXC_MVideo.u8UnStdOutput[MAIN_WINDOW] = 0;
            pXCResourcePrivate->stdrvXC_MVideo.u8UnStdOutput[SUB_WINDOW] = 1;
#endif

            pXCResourcePrivate->stdrvXC_MVideo.OSDC_FREQ = 600;  // default 60Hz
            pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState = E_POWER_MECHANICAL;
            pXCResourcePrivate->stdrvXC_MVideo.bSignalStableForStr = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bModuleInited = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.bTimingUnstableForStr = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.u8DebounceForStr = 0;

            // mvideo_context.c
            pXCResourcePrivate->stdrvXC_MVideo_Context.s_HDMISYNC_type = HDMI_SYNC_DE;
            pXCResourcePrivate->stdrvXC_MVideo_Context._bSupportHVMode = FALSE;
#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
            // For HW internal test pattern using
            pXCResourcePrivate->stdrvXC_MVideo_Context.bOP1_PatGen_Init = FALSE;
#endif
#endif

#if FRC_INSIDE
            pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCInited = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming = E_XC_FRC_InputTiming_4K2K;
#if (HW_DESIGN_4K2K_VER == 6) // For Manhattan, FSC+FRC
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetH = 0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetV = 0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16MEMCLevel = MEMC_LEVEL_MID;
            pXCResourcePrivate->stdrvXC_MVideo_Context.bIsFRCMute = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsAddrUpdated = FALSE;
#endif
#endif
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable = FALSE;
#if (HW_DESIGN_HDMITX_VER == 2)
            pXCResourcePrivate->stdrvXC_MVideo_Context.enHDMITXColorDepth = E_XC_HDMITX_COLORDEPTH_8BIT;
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
            // xc_hwreg_utility2.h
            pXCResourcePrivate->stXC_Utility._XC_EventFlag = 0;
            pXCResourcePrivate->stXC_Utility._XC_VSyncRun = FALSE;
            pXCResourcePrivate->stXC_Utility._XC_VSyncCount = 0;
            pXCResourcePrivate->stXC_Utility._XC_VSyncMax = 0;
#endif

            // mhal_adc.c
            pXCResourcePrivate->sthal_ADC.bIsYPbPrLooseLPF = FALSE;
            pXCResourcePrivate->sthal_ADC._bSourceSwitched = FALSE;
            pXCResourcePrivate->sthal_ADC._bEnableHWCalibration = FALSE;
            pXCResourcePrivate->sthal_ADC._bUseBandgap = FALSE;
            pXCResourcePrivate->sthal_ADC._eCvbsOutType = ADC_CVBSOUT_DISABLE_1;
            pXCResourcePrivate->sthal_ADC.enLastSourceType = ADC_INPUTSOURCE_UNKNOW;
            pXCResourcePrivate->sthal_ADC.u16LastHtotal = 0;
            pXCResourcePrivate->sthal_ADC.u16LastPixelClk = 0;
            pXCResourcePrivate->sthal_ADC.u16LastSamplingRatio = 1;

            // mhal_ip.c
            pXCResourcePrivate->sthal_IP._bSetVGASogEn = FALSE;
            pXCResourcePrivate->sthal_IP._u8SetSogDetectStatus = 0;

            // mhal_sc.c
            pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_fclk_gate_en = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_reg_ip2_clk_gate_en = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_ficlk_status = 0;
            pXCResourcePrivate->sthal_SC.u32OP1_reg_pre_down_f2 = 0;
            pXCResourcePrivate->sthal_SC.u32OP1_reg_pre_down_f1 = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_color_3x3 = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_color_vip = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_reg_di1lb_en = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_bypass_miu = 0;
            pXCResourcePrivate->sthal_SC.u16OP1_vop_dualpixel = 0;
            pXCResourcePrivate->sthal_SC.g_u16TmpValue = 0;
            pXCResourcePrivate->sthal_SC.g_bCntFlg = 1;
            pXCResourcePrivate->sthal_SC.bVSDshift[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->sthal_SC.bVSDshift[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->sthal_SC.su16OldValue = 0;
            pXCResourcePrivate->sthal_SC.su16OldValue_BWD_Status = 0;
            pXCResourcePrivate->sthal_SC.sbOldValueReaded = FALSE;
            // init as hw default value
            pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = 0x08;
            pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_DISP_AREA_TRIG] = 0x0A;
            pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = 0;
            pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG] = 0;
            pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = FALSE;
            pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = FALSE;
#if (HW_DESIGN_4K2K_VER == 4)
            pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_IP_AUTO_TUNE_AREA_TRIG] = 0x03;
            pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_IP_DISP_AREA_TRIG] = 0x05;
#endif
            pXCResourcePrivate->sthal_SC.u16OldValue[E_STORE_VALUE_INIT_CMD_NUM] = 0;

            pXCResourcePrivate->sthal_SC.bTrig_pStored = FALSE;
            pXCResourcePrivate->sthal_SC.u8Time_count = 0;
#if SUPPORT_SEAMLESS_ZAPPING
            pXCResourcePrivate->sthal_SC.u32DynamicScalingBaseAddr = 0;
            pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth = 0;
            pXCResourcePrivate->sthal_SC.bSeamlessZappingEnable[MAIN_WINDOW] = FALSE;
            pXCResourcePrivate->sthal_SC.bSeamlessZappingEnable[SUB_WINDOW] = FALSE;
            pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[MAIN_WINDOW] = 0;
            pXCResourcePrivate->sthal_SC.u8SeamlessZappingCurIndex[SUB_WINDOW] = 0;
            pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[MAIN_WINDOW] = 0;
            pXCResourcePrivate->sthal_SC.u8SeamlessZappingNextIndex[SUB_WINDOW] = 0;
#endif

#if (HW_DESIGN_4K2K_VER == 7)
            pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet = FALSE;
#endif

        }
#endif
    }
}


void XC_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpXC_GetLibVer = MApi_XC_GetLibVer_U2;
    psXCInstPri->fpXC_GetInfo = MApi_XC_GetInfo_U2;
    psXCInstPri->fpXC_GetStatus = MApi_XC_GetStatus_U2;
    psXCInstPri->fpXC_GetStatus_nodelay = MApi_XC_GetStatusNodelay_U2;
    psXCInstPri->fpXC_GetStatusEx = MApi_XC_GetStatusEx_U2;
    psXCInstPri->fpXC_SetDbgLevel = MApi_XC_SetDbgLevel_U2;
    psXCInstPri->fpXC_FPLLCustomerMode = MApi_XC_FPLLCustomerMode_U2;
    psXCInstPri->fpXC_SetIOMapBase = MDrv_XC_SetIOMapBase_U2;
    psXCInstPri->fpXC_Init = MApi_XC_Init_U2;
    psXCInstPri->fpXC_GetConfig = MApi_XC_GetConfig_U2;
    psXCInstPri->fpXC_Init_MISC = MApi_XC_Init_MISC_U2;
    psXCInstPri->fpXC_GetMISCStatus = MApi_XC_GetMISCStatus_U2;
    psXCInstPri->fpXC_GetCapability = MApi_XC_GetCapability_U2;
    psXCInstPri->fpXC_GetChipCaps = MApi_XC_GetChipCaps_U2;
    psXCInstPri->fpXC_Exit = MApi_XC_Exit_U2;
    psXCInstPri->fpXC_SetDynamicScaling = MApi_XC_SetDynamicScaling_U2;
    psXCInstPri->fpXC_SetDynamicScalingFlag = MApi_XC_Set_DynamicScalingFlag_U2;
    psXCInstPri->fpXC_Get_DNRBaseOffset = MApi_XC_Get_DNRBaseOffset_U2;
    psXCInstPri->fpXC_Get_FrameNumFactor = MApi_XC_Get_FrameNumFactor_U2;
    psXCInstPri->fpXC_SetWindow = MApi_XC_SetWindow_U2;
    psXCInstPri->fpXC_SetDualWindow = MApi_XC_SetDualWindow_U2;
    psXCInstPri->fpXC_SetTravelingWindow = MApi_XC_SetTravelingWindow_U2;
    psXCInstPri->fpXC_SetInputSource = MApi_XC_SetInputSource_U2;
    psXCInstPri->fpXC_IsYUVSpace =    MApi_XC_IsYUVSpace_U2;
    psXCInstPri->fpXC_IsMemoryFormat422 =    MApi_XC_IsMemoryFormat422_U2;
    psXCInstPri->fpXC_EnableForceRGBin =    MApi_XC_EnableForceRGBin_U2;
    psXCInstPri->fpXC_EnableMirrorModeEx =    MApi_XC_EnableMirrorModeEx_U2;
    psXCInstPri->fpXC_GetMirrorModeTypeEx =    MApi_XC_GetMirrorModeTypeEx_U2;
    psXCInstPri->fpXC_GetSyncStatus =    MApi_XC_GetSyncStatus_U2;
    psXCInstPri->fpXC_ConfigCMA =    MApi_XC_ConfigCMA_U2;
    psXCInstPri->fpXC_WaitOutputVSync =    MApi_XC_WaitOutputVSync_U2;
    psXCInstPri->fpXC_WaitInputVSync =    MApi_XC_WaitInputVSync_U2;
    psXCInstPri->fpXC_SetHdmiSyncMode =    MApi_XC_SetHdmiSyncMode_U2;
    psXCInstPri->fpXC_GetHdmiSyncMode =    MApi_XC_GetHdmiSyncMode_U2;
    psXCInstPri->fpXC_SetRepWindow =    MApi_XC_SetRepWindow_U2;
    psXCInstPri->fpXC_SkipOPWriteOffInSetWindow = MApi_XC_SkipOPWriteOffInSetWindow_U2;
    psXCInstPri->fpXC_GetSkipOPWriteOffInSetWindow = MApi_XC_GetSkipOPWriteOffInSetWindow_U2;
    psXCInstPri->fpXC_SetSkipDisableOPWriteOffInFPLL = MApi_XC_SetSkipDisableOPWriteOffInFPLL_U2;
    psXCInstPri->fpXC_Set_OPWriteOffEnable =    MApi_XC_Set_OPWriteOffEnable_U2;
    psXCInstPri->fpXC_Set_OPWriteOffEnableToReg = MApi_XC_Set_OPWriteOffEnableToReg_U2;
    psXCInstPri->fpXC_ForceSet_OPWriteOffEnable = MApi_XC_ForceSet_OPWriteOffEnable_U2;
    psXCInstPri->fpXC_Get_OPWriteOffEnable = MApi_XC_Get_OPWriteOffEnable_U2;
    psXCInstPri->fpXC_SetDispWinToReg =    MApi_XC_SetDispWinToReg_U2;
    psXCInstPri->fpXC_GetDispWinFromReg =    MApi_XC_GetDispWinFromReg_U2;
    psXCInstPri->fpXC_FreezeImg =    MApi_XC_FreezeImg_U2;
    psXCInstPri->fpXC_IsFreezeImg =    MApi_XC_IsFreezeImg_U2;
    psXCInstPri->fpXC_GenerateBlackVideoForBothWin =    MApi_XC_GenerateBlackVideoForBothWin_U2;
    psXCInstPri->fpXC_Set_BLSK =    MApi_XC_Set_BLSK_U2;
    psXCInstPri->fpXC_GenerateBlackVideo =    MApi_XC_GenerateBlackVideo_U2;
    psXCInstPri->fpXC_IsBlackVideoEnable =    MApi_XC_IsBlackVideoEnable_U2;
    psXCInstPri->fpXC_EnableFrameBufferLess =    MApi_XC_EnableFrameBufferLess_U2;
    psXCInstPri->fpXC_IsCurrentFrameBufferLessMode =    MApi_XC_IsCurrentFrameBufferLessMode_U2;
    psXCInstPri->fpXC_EnableRequest_FrameBufferLess =    MApi_XC_EnableRequest_FrameBufferLess_U2;
    psXCInstPri->fpXC_IsCurrentRequest_FrameBufferLessMode =    MApi_XC_IsCurrentRequest_FrameBufferLessMode_U2;
    psXCInstPri->fpXC_Get_3D_HW_Version =    MApi_XC_Get_3D_HW_Version_U2;
    psXCInstPri->fpXC_Get_3D_IsSupportedHW2DTo3D =    MApi_XC_Get_3D_IsSupportedHW2DTo3D_U2;
    psXCInstPri->fpXC_Set_3D_Mode =    MApi_XC_Set_3D_Mode_U2;
    psXCInstPri->fpXC_Set_3D_MainWin_FirstMode =    MApi_XC_Set_3D_MainWin_FirstMode_U2;
    psXCInstPri->fpXC_Set_3D_LR_Frame_Exchg =    MApi_XC_Set_3D_LR_Frame_Exchg_U2;
    psXCInstPri->fpXC_3D_Is_LR_Frame_Exchged =    MApi_XC_3D_Is_LR_Frame_Exchged_U2;
    psXCInstPri->fpXC_Get_3D_Input_Mode =    MApi_XC_Get_3D_Input_Mode_U2;
    psXCInstPri->fpXC_Get_3D_Output_Mode =    MApi_XC_Get_3D_Output_Mode_U2;
    psXCInstPri->fpXC_Get_3D_Panel_Type =    MApi_XC_Get_3D_Panel_Type_U2;
    psXCInstPri->fpXC_Get_3D_MainWin_First =    MApi_XC_Get_3D_MainWin_First_U2;
    psXCInstPri->fpXC_3DMainSub_IPSync =    MApi_XC_3DMainSub_IPSync_U2;
    psXCInstPri->fpXC_Set_3D_VerVideoOffset =    MApi_XC_Set_3D_VerVideoOffset_U2;
    psXCInstPri->fpXC_Get_3D_VerVideoOffset =    MApi_XC_Get_3D_VerVideoOffset_U2;
    psXCInstPri->fpXC_Is3DFormatSupported =    MApi_XC_Is3DFormatSupported_U2;
    psXCInstPri->fpXC_Set_3D_HShift =    MApi_XC_Set_3D_HShift_U2;
    psXCInstPri->fpXC_Enable_3D_LR_Sbs2Line =    MApi_XC_Enable_3D_LR_Sbs2Line_U2;
    psXCInstPri->fpXC_Get_3D_HShift =    MApi_XC_Get_3D_HShift_U2;
    psXCInstPri->fpXC_Set_3D_HW2DTo3D_Buffer =    MApi_XC_Set_3D_HW2DTo3D_Buffer_U2;
    psXCInstPri->fpXC_Set_3D_HW2DTo3D_Parameters =    MApi_XC_Set_3D_HW2DTo3D_Parameters_U2;
    psXCInstPri->fpXC_Get_3D_HW2DTo3D_Parameters =    MApi_XC_Get_3D_HW2DTo3D_Parameters_U2;
    psXCInstPri->fpXC_Set_3D_Detect3DFormat_Parameters =    MApi_XC_Set_3D_Detect3DFormat_Parameters_U2;
    psXCInstPri->fpXC_Get_3D_Detect3DFormat_Parameters =    MApi_XC_Get_3D_Detect3DFormat_Parameters_U2;
    psXCInstPri->fpXC_Detect3DFormatByContent =    MApi_XC_Detect3DFormatByContent_U2;
    psXCInstPri->fpXC_DetectNL =    MApi_XC_DetectNL_U2;
    psXCInstPri->fpXC_3D_PostPQSetting =    MApi_XC_3D_PostPQSetting_U2;
    psXCInstPri->fpXC_Set_3D_FPInfo =    MApi_XC_Set_3D_FPInfo_U2;
    psXCInstPri->fpXC_EnableAutoDetect3D =    MApi_XC_EnableAutoDetect3D_U2;
    psXCInstPri->fpXC_GetAutoDetect3DFlag =    MApi_XC_GetAutoDetect3DFlag_U2;
    psXCInstPri->fpXC_Set_3D_SubWinClk =    MApi_XC_Set_3D_SubWinClk_U2;
    psXCInstPri->fpXC_3D_Is_LR_Sbs2Line =    MApi_XC_3D_Is_LR_Sbs2Line_U2;
    psXCInstPri->fpSC_3D_Is_Skip_Default_LR_Flag =    MApi_SC_3D_Is_Skip_Default_LR_Flag_U2;
    psXCInstPri->fpXC_3D_Enable_Skip_Default_LR_Flag =    MApi_XC_3D_Enable_Skip_Default_LR_Flag_U2;
    psXCInstPri->fpXC_Mux_Init =    MApi_XC_Mux_Init_U2;
    psXCInstPri->fpXC_Mux_SourceMonitor =    MApi_XC_Mux_SourceMonitor_U2;
    psXCInstPri->fpXC_Mux_CreatePath =    MApi_XC_Mux_CreatePath_U2;
    psXCInstPri->fpXC_Mux_DeletePath =    MApi_XC_Mux_DeletePath_U2;
    psXCInstPri->fpXC_Mux_EnablePath =    MApi_XC_Mux_EnablePath_U2;
    psXCInstPri->fpXC_Mux_TriggerPathSyncEvent =    MApi_XC_Mux_TriggerPathSyncEvent_U2;
    psXCInstPri->fpXC_Mux_TriggerDestOnOffEvent =    MApi_XC_Mux_TriggerDestOnOffEvent_U2;
    psXCInstPri->fpXC_Mux_OnOffPeriodicHandler =    MApi_XC_Mux_OnOffPeriodicHandler_U2;
    psXCInstPri->fpXC_Mux_GetPathInfo =    MApi_XC_Mux_GetPathInfo_U2;
    psXCInstPri->fpXC_Mux_SetSupportMhlPathInfo =    MApi_XC_Mux_SetSupportMhlPathInfo_U2;
    psXCInstPri->fpXC_Mux_SetMhlHotPlugInverseInfo =    MApi_XC_Mux_SetMhlHotPlugInverseInfo_U2;
    psXCInstPri->fpXC_Mux_GetHDMIPort =    MApi_XC_Mux_GetHDMIPort_U2;
    psXCInstPri->fpXC_Mux_GetMappingTab =    MApi_XC_Mux_GetPortMappingMatrix_U2;
    psXCInstPri->fpXC_MUX_MApinputSourceToVDYMuxPORT =    MApi_XC_MUX_MapInputSourceToVDYMuxPORT_U2;
    psXCInstPri->fpXC_Set_NR =    MApi_XC_Set_NR_U2;
    psXCInstPri->fpXC_FilmMode_P =    MApi_XC_FilmMode_P_U2;
    psXCInstPri->fpXC_GetUCEnabled =    MApi_XC_GetUCEnabled_U2;
    psXCInstPri->fpXC_GenSpecificTiming =    MApi_XC_GenSpecificTiming_U2;
    psXCInstPri->fpXC_GetDEBypassMode =    MApi_XC_GetDEBypassMode_U2;
    psXCInstPri->fpXC_GetDEWindow =    MApi_XC_GetDEWindow_U2;
    psXCInstPri->fpXC_GetDEWidthHeightInDEByPassMode =    MApi_XC_GetDEWidthHeightInDEByPassMode_U2;
    psXCInstPri->fpXC_GetCaptureWindow =    MApi_XC_GetCaptureWindow_U2;
    psXCInstPri->fpXC_SetCaptureWindowVstart =    MApi_XC_SetCaptureWindowVstart_U2;
    psXCInstPri->fpXC_SetCaptureWindowHstart =    MApi_XC_SetCaptureWindowHstart_U2;
    psXCInstPri->fpXC_SetCaptureWindowVsize =    MApi_XC_SetCaptureWindowVsize_U2;
    psXCInstPri->fpXC_SetCaptureWindowHsize =    MApi_XC_SetCaptureWindowHsize_U2;
    psXCInstPri->fpXC_SoftwareReset =    MApi_XC_SoftwareReset_U2;
    psXCInstPri->fpXC_CalculateHFreqx10 =    MApi_XC_CalculateHFreqx10_U2;
    psXCInstPri->fpXC_CalculateHFreqx1K =    MApi_XC_CalculateHFreqx1K_U2;
    psXCInstPri->fpXC_CalculateVFreqx10 =    MApi_XC_CalculateVFreqx10_U2;
    psXCInstPri->fpXC_CalculateVFreqx1K =    MApi_XC_CalculateVFreqx1K_U2;
    psXCInstPri->fpXC_GetAccurateVFreqx1K =    MApi_XC_GetAccurateVFreqx1K_U2;
    psXCInstPri->fpXC_InterruptAttach =    MApi_XC_InterruptAttach_U2;
    psXCInstPri->fpXC_InterruptDeAttach =    MApi_XC_InterruptDeAttach_U2;
    psXCInstPri->fpXC_DisableInputSource =    MApi_XC_DisableInputSource_U2;
    psXCInstPri->fpXC_IsInputSourceDisabled =    MApi_XC_IsInputSourceDisabled_U2;
    psXCInstPri->fpXC_ChangePanelType =    MApi_XC_ChangePanelType_U2;
    psXCInstPri->fpXC_GetCurrentReadBank =    MApi_XC_GetCurrentReadBank_U2;
    psXCInstPri->fpXC_GetCurrentWriteBank =    MApi_XC_GetCurrentWriteBank_U2;
    psXCInstPri->fpXC_SetAutoPreScaling =    MApi_XC_SetAutoPreScaling_U2;
    psXCInstPri->fpXC_GetVSyncWidth =    MApi_XC_GetVSyncWidth_U2;
    psXCInstPri->fpXC_set_GOP_Enable =    MApi_XC_set_GOP_Enable_U2;
    psXCInstPri->fpXC_ip_sel_for_gop =    MApi_XC_ip_sel_for_gop_U2;
    psXCInstPri->fpXC_SetVOPNBL =    MApi_XC_SetVOPNBL_U2;
    psXCInstPri->fpXC_GetDstInfo =    MApi_XC_GetDstInfo_U2;
    psXCInstPri->fpXC_Set_FD_Mask_ByWin = MApi_XC_Set_FD_Mask_ByWin_U2;
    psXCInstPri->fpXC_Get_FD_Mask_ByWin = MApi_XC_Get_FD_Mask_ByWin_U2;
    psXCInstPri->fpXC_Get_FD_Mask_Status = MApi_XC_Get_FD_Mask_Status_U2;
    psXCInstPri->fpXC_SetIP1TestPattern =    MApi_XC_SetIP1TestPattern_U2;
#ifdef UFO_XC_TEST_PATTERN
    psXCInstPri->fpXC_GenerateTestPattern =    MApi_XC_GenerateTestPattern_U2;
#endif
    psXCInstPri->fpXC_InitIPForInternalTiming =    MApi_XC_InitIPForInternalTiming_U2;
    psXCInstPri->fpXC_SetIPMux =    MApi_XC_SetIPMux_U2;
    psXCInstPri->fpXC_Is_H_Sync_Active =    MApi_XC_Is_H_Sync_Active_U2;
    psXCInstPri->fpXC_Is_V_Sync_Active =    MApi_XC_Is_V_Sync_Active_U2;
    psXCInstPri->fpXC_GetAutoPositionWindow =    MApi_XC_GetAutoPositionWindow_U2;
    psXCInstPri->fpXC_SetFrameBufferAddress =    MApi_XC_SetFrameBufferAddress_U2;
    psXCInstPri->fpXC_SetFRCMFrameBufferAddress =    MApi_XC_SetFRCMFrameBufferAddress_U2;
    psXCInstPri->fpXC_IsFrameBufferEnoughForCusScaling =    MApi_XC_IsFrameBufferEnoughForCusScaling_U2;
    psXCInstPri->fpXC_SetScalerMemoryRequest =    MApi_XC_SetScalerMemoryRequest_U2;
    psXCInstPri->fpXC_Get_PixelData =    MApi_XC_Get_PixelData_U2;
    psXCInstPri->fpXC_GetAvailableSize =    MApi_XC_GetAvailableSize_U2;
    psXCInstPri->fpXC_SetFrameColor =    MApi_XC_SetFrameColor_U2;
    psXCInstPri->fpXC_SetDispWindowColor =    MApi_XC_SetDispWindowColor_U2;
    psXCInstPri->fpXC_SupportSourceToVE =    MApi_XC_SupportSourceToVE_U2;
    psXCInstPri->fpXC_SetOutputCapture =    MApi_XC_SetOutputCapture_U2;
    psXCInstPri->fpXC_SetGammaOnOff  =    MApi_XC_SetGammaOnOff_U2;
    psXCInstPri->fpXC_SetPreGammaGain =    MApi_XC_SetPreGammaGain_U2;
    psXCInstPri->fpXC_SetPreGammaOffset =    MApi_XC_SetPreGammaOffset_U2;
    psXCInstPri->fpXC_SetPanelTiming =    MApi_XC_SetPanelTiming_U2;
    psXCInstPri->fpXC_SetOutTimingMode =    MApi_XC_SetOutTimingMode_U2;
    psXCInstPri->fpXC_SetFreeRunTiming =    MApi_XC_SetFreeRunTiming_U2;
    psXCInstPri->fpXC_Set_CustomerSyncInfo =    MApi_XC_Set_CustomerSyncInfo_U2;
    psXCInstPri->fpXC_WaitFPLLDone =    MApi_XC_WaitFPLLDone_U2;
    psXCInstPri->fpXC_GetOutputVFreqX100 =    MApi_XC_GetOutputVFreqX100_U2;
    psXCInstPri->fpXC_GetOP1OutputVFreqX100 =    MApi_XC_GetOP1OutputVFreqX100_U2;
    psXCInstPri->fpXC_FrameLockCheck =    MApi_XC_FrameLockCheck_U2;
    psXCInstPri->fpXC_CustomizeFRCTable =    MApi_XC_CustomizeFRCTable_U2;
    psXCInstPri->fpXC_OutputFrameCtrl =    MApi_XC_OutputFrameCtrl_U2;
    psXCInstPri->fpSC_Enable_FPLL_FSM =    MApi_SC_Enable_FPLL_FSM_U2;
    psXCInstPri->fpSC_ForceFreerun =    MApi_SC_ForceFreerun_U2;
    psXCInstPri->fpSC_IsForceFreerun =    MApi_SC_IsForceFreerun_U2;
    psXCInstPri->fpSC_SetFreerunVFreq =    MApi_SC_SetFreerunVFreq_U2;
    psXCInstPri->fpXC_SetExPanelInfo =    MApi_XC_SetExPanelInfo_U2;
    psXCInstPri->fpXC_Enable_FPLL_Thresh_Mode =    MApi_XC_Enable_FPLL_Thresh_Mode_U2;
    psXCInstPri->fpXC_Get_FPLL_Thresh_Mode = MApi_XC_Get_FPLL_Thresh_Mode_U2;
    psXCInstPri->fpXC_EnableIPAutoNoSignal =    MApi_XC_EnableIPAutoNoSignal_U2;
    psXCInstPri->fpXC_GetIPAutoNoSignal =    MApi_XC_GetIPAutoNoSignal_U2;
    psXCInstPri->fpXC_EnableIPAutoCoast =    MApi_XC_EnableIPAutoCoast_U2;
    psXCInstPri->fpXC_EnableIPCoastDebounce =    MApi_XC_EnableIPCoastDebounce_U2;
    psXCInstPri->fpXC_ClearIPCoastStatus =    MApi_XC_ClearIPCoastStatus_U2;
    psXCInstPri->fpXC_EnableFpllManualSetting =    MApi_XC_EnableFpllManualSetting_U2;
    psXCInstPri->fpXC_FpllBoundaryTest =    MApi_XC_FpllBoundaryTest_U2;
    psXCInstPri->fpXC_SetOffLineDetection  =    MApi_XC_SetOffLineDetection_U2;
    psXCInstPri->fpXC_GetOffLineDetection =    MApi_XC_GetOffLineDetection_U2;
    psXCInstPri->fpXC_SetOffLineSogThreshold =    MApi_XC_SetOffLineSogThreshold_U2;
    psXCInstPri->fpXC_SetOffLineSogBW =    MApi_XC_SetOffLineSogBW_U2;
    psXCInstPri->fpXC_OffLineInit =    MApi_XC_OffLineInit_U2;
    psXCInstPri->fpXC_Set_Extra_fetch_adv_line =    MApi_XC_Set_Extra_fetch_adv_line_U2;
    psXCInstPri->fpXC_SetVGASogEn =    MApi_XC_SetVGASogEn_U2;
    psXCInstPri->fpXC_EnableWindow =    MApi_XC_EnableWindow_U2;
    psXCInstPri->fpXC_Is_SubWindowEanble =    MApi_XC_Is_SubWindowEanble_U2;
    psXCInstPri->fpXC_SetBorderFormat =    MApi_XC_SetBorderFormat_U2;
    psXCInstPri->fpXC_EnableBorder =    MApi_XC_EnableBorder_U2;
    psXCInstPri->fpXC_ZorderMainWindowFirst =    MApi_XC_ZorderMainWindowFirst_U2;
    psXCInstPri->fpXC_PQ_LoadFunction =    MApi_XC_PQ_LoadFunction_U2;
    psXCInstPri->fpXC_Check_HNonLinearScaling =    MApi_XC_Check_HNonLinearScaling_U2;
    psXCInstPri->fpXC_EnableEuroHdtvSupport =    MApi_XC_EnableEuroHdtvSupport_U2;
    psXCInstPri->fpXC_EnableEuroHdtvDetection =    MApi_XC_EnableEuroHdtvDetection_U2;
    psXCInstPri->fpXC_ReadByte =    MApi_XC_ReadByte_U2;
    psXCInstPri->fpXC_WriteByte =    MApi_XC_WriteByte_U2;
    psXCInstPri->fpXC_WriteByteMask =    MApi_XC_WriteByteMask_U2;
    psXCInstPri->fpXC_Write2ByteMask =    MApi_XC_Write2ByteMask_U2;
    psXCInstPri->fpXC_W2BYTE =    MApi_XC_W2BYTE_U2;
    psXCInstPri->fpXC_R2BYTE =    MApi_XC_R2BYTE_U2;
    psXCInstPri->fpXC_W4BYTE =    MApi_XC_W4BYTE_U2;
    psXCInstPri->fpXC_R4BYTE =    MApi_XC_R4BYTE_U2;
    psXCInstPri->fpXC_R2BYTEMSK =    MApi_XC_R2BYTEMSK_U2;
    psXCInstPri->fpXC_W2BYTEMSK =    MApi_XC_W2BYTEMSK_U2;
    psXCInstPri->fpXC_MLoad_Init =    MApi_XC_MLoad_Init_U2;
    psXCInstPri->fpXC_MLoad_Enable =    MApi_XC_MLoad_Enable_U2;
    psXCInstPri->fpXC_MLoad_Cus_Init =    MApi_XC_MLoad_Cus_Init_U2;
    psXCInstPri->fpXC_MLoad_Cus_Enable =    MApi_XC_MLoad_Cus_Enable_U2;
    psXCInstPri->fpXC_MLoad_GetStatus =    MApi_XC_MLoad_GetStatus_U2;
    psXCInstPri->fpXC_MLoad_WriteCmd_And_Fire =    MApi_XC_MLoad_WriteCmd_And_Fire_U2;
    psXCInstPri->fpXC_MLoad_WriteCmds_And_Fire =    MApi_XC_MLoad_WriteCmds_And_Fire_U2;
    psXCInstPri->fpXC_MLoad_Combine_Enable = MApi_XC_Combine_MLoadEn_U2;
    psXCInstPri->fpXC_MLG_Init =    MApi_XC_MLG_Init_U2;
    psXCInstPri->fpXC_MLG_Enable =    MApi_XC_MLG_Enable_U2;
    psXCInstPri->fpXC_MLG_GetCaps =    MApi_XC_MLG_GetCaps_U2;
    psXCInstPri->fpXC_MLG_GetStatus =    MApi_XC_MLG_GetStatus_U2;
    psXCInstPri->fpXC_SetOSD2VEMode =    MApi_XC_SetOSD2VEMode_U2;
    psXCInstPri->fpXC_IP2_PreFilter_Enable =    MApi_XC_IP2_PreFilter_Enable_U2;
    psXCInstPri->fpXC_Get_Pixel_RGB =    MApi_XC_Get_Pixel_RGB_U2;
    psXCInstPri->fpXC_KeepPixelPointerAppear =    MApi_XC_KeepPixelPointerAppear_U2;
    psXCInstPri->fpXC_Set_MemFmtEx =    MApi_XC_Set_MemFmtEx_U2;
    psXCInstPri->fpXC_IsRequestFrameBufferLessMode =    MApi_XC_IsRequestFrameBufferLessMode_U2;
    psXCInstPri->fpXC_SkipSWReset =    MApi_XC_SkipSWReset_U2;
    psXCInstPri->fpXC_EnableRepWindowForFrameColor =    MApi_XC_EnableRepWindowForFrameColor_U2;
    psXCInstPri->fpXC_SetOSDLayer =    MApi_XC_SetOSDLayer_U2;
    psXCInstPri->fpXC_GetOSDLayer =    MApi_XC_GetOSDLayer_U2;
    psXCInstPri->fpXC_SetVideoAlpha =    MApi_XC_SetVideoAlpha_U2;
    psXCInstPri->fpXC_GetVideoAlpha =    MApi_XC_GetVideoAlpha_U2;
    psXCInstPri->fpXC_SkipWaitVsync =    MApi_XC_SkipWaitVsync_U2;
    psXCInstPri->fpXC_SetCMAHeapID =    MApi_XC_SetCMAHeapID_U2;
    psXCInstPri->fpXC_OP2VOPDESel =    MApi_XC_OP2VOPDESel_U2;
    psXCInstPri->fpXC_FRC_SetWindow =    MApi_XC_FRC_SetWindow_U2;
    psXCInstPri->fpXC_Enable_TwoInitFactor =    MApi_XC_Enable_TwoInitFactor_U2;
    psXCInstPri->fpXC_IsFieldPackingModeSupported =    MApi_XC_IsFieldPackingModeSupported_U2;
    psXCInstPri->fpXC_PreInit =    MApi_XC_PreInit_U2;
    psXCInstPri->fpXC_Get_BufferData =    MApi_XC_Get_BufferData_U2;
    psXCInstPri->fpXC_Set_BufferData =    MApi_XC_Set_BufferData_U2;
    psXCInstPri->fpXC_EnableMainWindow =    MApi_XC_EnableMainWindow_U2;
    psXCInstPri->fpXC_EnableSubWindow =    MApi_XC_EnableSubWindow_U2;
    psXCInstPri->fpXC_DisableSubWindow =    MApi_XC_DisableSubWindow_U2;
    psXCInstPri->fpXC_SetPixelShift =    MApi_XC_SetPixelShift_U2;
    psXCInstPri->fpXC_SetVideoOnOSD = MApi_XC_SetVideoOnOSD_U2;
    psXCInstPri->fpXC_SetOSDLayerBlending = MApi_XC_SetOSDLayerBlending_U2;
    psXCInstPri->fpXC_SetOSDLayerAlpha = MApi_XC_SetOSDLayerAlpha_U2;
    psXCInstPri->fpXC_SetOSDBlendingFormula =    MApi_XC_SetOSDBlendingFormula_U2;
    psXCInstPri->fpXC_ReportPixelInfo =    MApi_XC_ReportPixelInfo_U2;
    psXCInstPri->fpXC_SetScaling =    MApi_XC_SetScaling_U2;
    psXCInstPri->fpXC_SetMCDIBufferAddress =    MApi_XC_SetMCDIBufferAddress_U2;
    psXCInstPri->fpXC_EnableMCDI =    MApi_XC_EnableMCDI_U2;
    psXCInstPri->fpXC_SendCmdToFRC =    MApi_XC_SendCmdToFRC_U2;
    psXCInstPri->fpXC_GetMsgFromFRC =    MApi_XC_GetMsgFromFRC_U2;
    psXCInstPri->fpXC_EnableRWBankAuto =    MApi_XC_EnableRWBankAuto_U2;
    psXCInstPri->fpXC_SetWRBankMappingNum =    MApi_XC_SetWRBankMappingNum_U2;
    psXCInstPri->fpXC_GetWRBankMappingNum =    MApi_XC_GetWRBankMappingNum_U2;
    psXCInstPri->fpXC_GetWRBankMappingNumForZap = MApi_XC_GetWRBankMappingNumForZap_U2;
    psXCInstPri->fpXC_SetBOBMode = MApi_XC_SetBOBMode_U2;
    psXCInstPri->fpXC_SetForceReadBank = MApi_XC_SetForceReadBank_U2;
    psXCInstPri->fpXC_SetLockPoint = MApi_XC_SetLockPoint_U2;
    psXCInstPri->fpXC_LD_Init =    MApi_XC_LD_Init_U2;
    psXCInstPri->fpXC_LD_SetMemoryAddress =    MApi_XC_LD_SetMemoryAddress_U2;
    psXCInstPri->fpXC_LD_Get_Value = MApi_XC_LD_Get_Value_U2;
    psXCInstPri->fpXC_LD_SetLevel =    MApi_XC_LD_SetLevel_U2;
    psXCInstPri->fpSet_TurnoffLDBL =    MApi_Set_TurnoffLDBL_U2;
    psXCInstPri->fpSet_notUpdateSPIDataFlags =    MApi_Set_notUpdateSPIDataFlags_U2;
    psXCInstPri->fpSet_UsermodeLDFlags =    MApi_Set_UsermodeLDFlags_U2;
    psXCInstPri->fpSet_BLLevel =    MApi_Set_BLLevel_U2;
    psXCInstPri->fpXC_Set_BWS_Mode =    MApi_XC_Set_BWS_Mode_U2;
    psXCInstPri->fpXC_FRC_ColorPathCtrl =    MApi_XC_FRC_ColorPathCtrl_U2;
    psXCInstPri->fpXC_FRC_OP2_SetRGBGain =    MApi_XC_FRC_OP2_SetRGBGain_U2;
    psXCInstPri->fpXC_FRC_OP2_SetRGBOffset =    MApi_XC_FRC_OP2_SetRGBOffset_U2;
    psXCInstPri->fpXC_FRC_OP2_SetDither =    MApi_XC_FRC_OP2_SetDither_U2;
    psXCInstPri->fpXC_FRC_BypassMFC =    MApi_XC_FRC_BypassMFC_U2;
    psXCInstPri->fpXC_FRC_Mute =    MApi_XC_FRC_Mute_U2;
    psXCInstPri->fpXC_ForceReadFrame =    MApi_XC_ForceReadFrame_U2;
    psXCInstPri->fpXC_SetCsc =    MApi_XC_SetCsc_U2;
    psXCInstPri->fpXC_RegisterPQSetFPLLThreshMode =    MApi_XC_RegisterPQSetFPLLThreshMode_U2;
    psXCInstPri->fpXC_GetFreeRunStatus =    MApi_XC_GetFreeRunStatus_U2;
    psXCInstPri->fpXC_BYPASS_SetCSC = MApi_XC_BYPASS_SetCSC_U2;
    psXCInstPri->fpXC_Get_DSForceIndexSupported =    MApi_XC_Get_DSForceIndexSupported_U2;
    psXCInstPri->fpXC_Set_DSForceIndex =    MApi_XC_Set_DSForceIndex_U2;
    psXCInstPri->fpXC_Set_DSIndexSourceSelect =    MApi_XC_Set_DSIndexSourceSelect_U2;
    psXCInstPri->fpXC_GetDynamicScalingStatus = MApi_XC_GetDynamicScalingStatus_U2;
    psXCInstPri->fpXC_OSDC_InitSetting =    MApi_XC_OSDC_InitSetting_U2;
    psXCInstPri->fpXC_OSDC_SetOutVfreqx10 = MApi_XC_OSDC_SetOutVfreqx10_U2;
    psXCInstPri->fpXC_OSDC_Control =    MApi_XC_OSDC_Control_U2;
    psXCInstPri->fpXC_OSDC_GetDstInfo =    MApi_XC_OSDC_GetDstInfo_U2;
    psXCInstPri->fpXC_Set_PowerState = MApi_XC_SetPowerState_U2;
    psXCInstPri->fpXC_BYPASS_SetOSDVsyncPos = MApi_XC_BYPASS_SetOSDVsyncPos_U2;
    psXCInstPri->fpXC_BYPASS_SetInputSrc = MApi_XC_BYPASS_SetInputSrc_U2;
    psXCInstPri->fpXC_SetSeamlessZapping = MApi_XC_SetSeamlessZapping_U2;
    psXCInstPri->fpXC_GetSeamlessZappingStatus = MApi_XC_GetSeamlessZappingStatus_U2;
    psXCInstPri->fpXC_Vtrack_SetPayloadData = MApi_XC_Vtrack_SetPayloadData_U2;
    psXCInstPri->fpXC_Vtrack_SetUserDefindedSetting = MApi_XC_Vtrack_SetUserDefindedSetting_U2;
    psXCInstPri->fpXC_Vtrack_Enable = MApi_XC_Vtrack_Enable_U2;
    psXCInstPri->fpXC_PreSetPQInfo = MApi_XC_PreSetPQInfo_U2;
    psXCInstPri->fpXC_Is_OP1_TestPattern_Enabled = MApi_XC_Is_OP1_TestPattern_Enabled_U2;
    psXCInstPri->fpXC_Set_OP1_TestPattern = MApi_XC_Set_OP1_TestPattern_U2;
    psXCInstPri->fpXC_Set_OP2_TestPattern = MApi_XC_Set_OP2_Pattern_U2;
    psXCInstPri->fpXC_Check_WhiteBalance_Pattern_Mode = MApi_XC_CheckWhiteBalancePatternModeSupport_U2;
    psXCInstPri->fpXC_Set_HLinearScaling = MApi_XC_SetHLinearScaling_U2;
    psXCInstPri->fpXC_EnableT3D = MApi_XC_EnableT3D_U2;
    psXCInstPri->fpXC_Set_FRC_InputTiming = MApi_XC_Set_FRC_InputTiming_U2;
    psXCInstPri->fpXC_Get_FRC_InputTiming = MApi_XC_Get_FRC_InputTiming_U2;
    psXCInstPri->fpXC_Get_VirtualBox_Info = MApi_XC_Get_VirtualBox_Info_U2;
    psXCInstPri->fpXC_Set_OSD_DETECT = MApi_XC_Set_OSD_Detect_U2;
    psXCInstPri->fpXC_Get_OSD_DETECT = MApi_XC_Get_OSD_Detect_U2;
    psXCInstPri->fpXC_Enable_LockFreqOnly = MApi_XC_Enable_LockFreqOnly_U2;
    psXCInstPri->fpXC_Is2K2KToFrcMode = MApi_XC_Is2K2KToFrcMode_U2;
    psXCInstPri->fpXC_SetXCVOPdata = MApi_XC_Set_XC_VOP_U2;
    psXCInstPri->fpXC_GetSWDSIndex =  MApi_XC_GetSWDSIndex_U2;
    psXCInstPri->fpXC_Get_Is_SupportSWDS = MApi_XC_Is_SupportSWDS_U2;
    psXCInstPri->fpXC_Get_FRCM_FrameNum = MApi_XC_Get_FRCM_FrameNum_U2;
    psXCInstPri->fpXC_SetPixelShiftFeatures = MApi_XC_SetPixelShiftFeatures_U2;
    psXCInstPri->fpXC_SetForceWrite =   MApi_XC_SetForceWrite_U2;
    psXCInstPri->fpXC_GetForceWrite =   MApi_XC_GetForceWrite_U2;
#ifdef UFO_XC_PQ_PATH
    psXCInstPri->fpXC_GetPQPathStatus = MApi_XC_GetPQPathStatus_U2;
#endif
#ifdef UFO_XC_AUTO_DOWNLOAD
    psXCInstPri->fpXC_AutoDownloadConfig = MApi_XC_AutoDownload_Config_U2;
    psXCInstPri->fpXC_AutoDownloadWrite = MApi_XC_AutoDownload_Write_U2;
    psXCInstPri->fpXC_AutoDownloadFire = MApi_XC_AutoDownload_Fire_U2;
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
    psXCInstPri->fpXC_HDRControl = MApi_XC_HDR_Control_U2;
#endif
#endif
    psXCInstPri->fpXC_OPTEE_Ctrl = MApi_XC_OPTEE_Control_U2;
    psXCInstPri->fpXC_OPTEE_CheckRBase = MApi_XC_OPTEE_CheckRBase_U2;
    psXCInstPri->fpXC_OPTEE_Mux = MApi_XC_OPTEE_Mux_U2;
    psXCInstPri->fpXC_GetResourceByPipeID = MApi_XC_GetResourceByPipeID_U2;
    psXCInstPri->fpXC_ConfigPipe = MApi_XC_ConfigPipe_U2;
    psXCInstPri->fpXC_CheckPipe = MApi_XC_CheckPipe_U2;

#ifdef UFO_XC_SET_DSINFO_V0
    psXCInstPri->fpXC_SetDSInfo = MApi_XC_SetDSInfo_U2;
#endif
//** OBSELETE FUNCTIONS **//
    psXCInstPri->fpsc_get_output_vfreq =    mvideo_sc_get_output_vfreq_U2;
    psXCInstPri->fpXC_Get_Current_OutputVFreqX100 =    MApi_XC_Get_Current_OutputVFreqX100_U2;
    psXCInstPri->fpXC_SetMemoryWriteRequest =    MApi_XC_SetMemoryWriteRequest_U2;
    psXCInstPri->fpXC_Set_MemFmt =    MApi_XC_Set_MemFmt_U2;
    psXCInstPri->fpXC_SetOutputAdjustSetting =    MApi_XC_SetOutputAdjustSetting_U2;
    psXCInstPri->fsScaler_FPLL_FSM =    msAPI_Scaler_FPLL_FSM_U2;
    psXCInstPri->fpXC_EnableMirrorMode =    MApi_XC_EnableMirrorMode_U2;
    psXCInstPri->fpXC_EnableMirrorMode2 =    MApi_XC_EnableMirrorMode2_U2;
#ifdef SUPPORT_BWD
    psXCInstPri->fpXC_SetBwdConfig = MApi_XC_SetBwdConfig_U2;
#endif
//** OBSELETE FUNCTIONS**//
    psXCInstPri->fpXC_dump_shm = MApi_XC_Dump_SHM_U2;
    psXCInstPri->fpXC_IsSupport2StepScaling = MApi_XC_IsSupport2StepScaling_U2;
    psXCInstPri->fpXC_VideoPlaybackCtrl = MApi_XC_VideoPlaybackCtrl_U2;

#ifdef UFO_XC_SETBLACKVIDEOBYMODE
    psXCInstPri->fpXC_GenerateBlackVideoByMode =    MApi_XC_GenerateBlackVideoByMode_U2;
#endif
    psXCInstPri->fpXC_swds_fire = MApi_SWDS_Fire_U2;

#ifdef UFO_XC_SUPPORT_DUAL_MIU
    psXCInstPri->fpXC_enableDualMode = MApi_XC_EnableMiuDualMode_U2;
#endif

#ifdef UFO_XC_GET_3D_FORMAT
    psXCInstPri->fpXC_Get3DFormat = MApi_XC_Get3DFormat_U2;
#endif

#ifdef UFO_XC_FB_LEVEL
    psXCInstPri->fpXC_SetFBLevel =    MApi_XC_Set_FB_Level_U2;
    psXCInstPri->fpXC_GetFBLevel =    MApi_XC_Get_FB_Level_U2;
#endif
    psXCInstPri->fpXC_Set_SWDR_Info = MApi_XC_SetSWDRInfo_U2;
    psXCInstPri->fpXC_Get_SWDR_Info = MApi_XC_GetSWDRInfo_U2;
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    psXCInstPri->fpXC_GetHDMIPolicy = MApi_XC_GetHDMIPolicy_U2;
#endif
}

void XC_U1_U2_RELATION(void** ppInstance, const void* const pAttribute)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psXCInstPri);
    psXCInstPri->u32DeviceID = 0;

    #define URELATION(a,b,c)   psXCInstPri->_URELATION_ASSIGN(a,b,c)
        URELATION(fpXC_GetLibVer,(IOCTL_XC_GETLIBVER)MApi_XC_GetLibVer_U2,MApi_XC_GetLibVer)
        URELATION(fpXC_GetInfo,(IOCTL_XC_GETINFO)MApi_XC_GetInfo_U2,MApi_XC_GetInfo)
        URELATION(fpXC_GetStatus,(IOCTL_XC_GETSTATUS)MApi_XC_GetStatus_U2,MApi_XC_GetStatus)
        URELATION(fpXC_GetStatus_nodelay,(IOCTL_XC_GETSTATUS_NODELAY)MApi_XC_GetStatusNodelay_U2,MApi_XC_GetStatusNodelay)
        URELATION(fpXC_GetStatusEx,(IOCTL_XC_GETSTATUSEX)MApi_XC_GetStatusEx_U2,MApi_XC_GetStatusEx)
        URELATION(fpXC_SetDbgLevel,(IOCTL_XC_SETDBGLEVEL)MApi_XC_SetDbgLevel_U2,MApi_XC_SetDbgLevel)
        URELATION(fpXC_FPLLCustomerMode,(IOCTL_XC_FPLLCUSTOMERMODE)MApi_XC_FPLLCustomerMode_U2,MApi_XC_FPLLCustomerMode)
        URELATION(fpXC_SetIOMapBase,(IOCTL_XC_SETIOMAPBASE)MDrv_XC_SetIOMapBase_U2,MDrv_XC_SetIOMapBase)
        URELATION(fpXC_Init,(IOCTL_XC_INIT)MApi_XC_Init_U2,MApi_XC_Init)
        URELATION(fpXC_GetConfig,(IOCTL_XC_GETCONFIG)MApi_XC_GetConfig_U2,MApi_XC_GetConfig)
        URELATION(fpXC_Init_MISC,(IOCTL_XC_INIT_MISC)MApi_XC_Init_MISC_U2,MApi_XC_Init_MISC)
        URELATION(fpXC_GetMISCStatus,(IOCTL_XC_GETMISCSTATUS)MApi_XC_GetMISCStatus_U2,MApi_XC_GetMISCStatus)
        URELATION(fpXC_GetCapability,(IOCTL_XC_GETCAPABILITY)MApi_XC_GetCapability_U2,MApi_XC_GetCapability)
        URELATION(fpXC_GetChipCaps,(IOCTL_XC_GETCHIPCAPS)MApi_XC_GetChipCaps_U2,MApi_XC_GetChipCaps)
        URELATION(fpXC_Exit,(IOCTL_XC_EXIT)MApi_XC_Exit_U2,MApi_XC_Exit)
        URELATION(fpXC_SetDynamicScaling,(IOCTL_XC_SETDYNAMICSCALING)MApi_XC_SetDynamicScaling_U2,MApi_XC_SetDynamicScaling)
        URELATION(fpXC_SetDynamicScalingFlag,(IOCTL_XC_SETDYNAMICSCALING_FLAG)MApi_XC_Set_DynamicScalingFlag_U2,MApi_XC_Set_DynamicScalingFlag)
        URELATION(fpXC_Get_DNRBaseOffset,(IOCTL_XC_GET_DNRBASEOFFSET)MApi_XC_Get_DNRBaseOffset_U2,MApi_XC_Get_DNRBaseOffset)
        URELATION(fpXC_Get_FrameNumFactor,(IOCTL_XC_GET_FRAMENUMFACTOR)MApi_XC_Get_FrameNumFactor_U2,MApi_XC_Get_FrameNumFactor)
        URELATION(fpXC_SetWindow,(IOCTL_XC_SETWINDOW)MApi_XC_SetWindow_U2,MApi_XC_SetWindow)
        URELATION(fpXC_SetDualWindow,(IOCTL_XC_SETDUALWINDOW)MApi_XC_SetDualWindow_U2,MApi_XC_SetDualWindow)
        URELATION(fpXC_SetTravelingWindow,(IOCTL_XC_SETTRAVELINGWINDOW)MApi_XC_SetTravelingWindow_U2,MApi_XC_SetTravelingWindow)
        URELATION(fpXC_SetInputSource,(IOCTL_XC_SETINPUTSOURCE)MApi_XC_SetInputSource_U2,MApi_XC_SetInputSource)
        URELATION(fpXC_IsYUVSpace,(IOCTL_XC_ISYUVSPACE)MApi_XC_IsYUVSpace_U2,MApi_XC_IsYUVSpace)
        URELATION(fpXC_IsMemoryFormat422,(IOCTL_XC_ISMEMORYFORMAT422)MApi_XC_IsMemoryFormat422_U2,MApi_XC_IsMemoryFormat422)
        URELATION(fpXC_EnableForceRGBin,(IOCTL_XC_ENABLEFORCERGBIN)MApi_XC_EnableForceRGBin_U2,MApi_XC_EnableForceRGBin)
        URELATION(fpXC_EnableMirrorModeEx,(IOCTL_XC_ENABLEMIRRORMODEEX)MApi_XC_EnableMirrorModeEx_U2,MApi_XC_EnableMirrorModeEx)
        URELATION(fpXC_GetMirrorModeTypeEx,(IOCTL_XC_GETMIRRORMODETYPEEX)MApi_XC_GetMirrorModeTypeEx_U2,MApi_XC_GetMirrorModeTypeEx)
        URELATION(fpXC_GetSyncStatus,(IOCTL_XC_GETSYNCSTATUS)MApi_XC_GetSyncStatus_U2,MApi_XC_GetSyncStatus)
        URELATION(fpXC_ConfigCMA,(IOCTL_XC_CONFIGCMA)MApi_XC_ConfigCMA_U2,MApi_XC_ConfigCMA)
        URELATION(fpXC_WaitOutputVSync,(IOCTL_XC_WAITOUTPUTVSYNC)MApi_XC_WaitOutputVSync_U2,MApi_XC_WaitOutputVSync)
        URELATION(fpXC_WaitInputVSync,(IOCTL_XC_WAITINPUTVSYNC)MApi_XC_WaitInputVSync_U2,MApi_XC_WaitInputVSync)
        URELATION(fpXC_SetHdmiSyncMode,(IOCTL_XC_SETHDMISYNCMODE)MApi_XC_SetHdmiSyncMode_U2,MApi_XC_SetHdmiSyncMode)
        URELATION(fpXC_GetHdmiSyncMode,(IOCTL_XC_GETHDMISYNCMODE)MApi_XC_GetHdmiSyncMode_U2,MApi_XC_GetHdmiSyncMode)
        URELATION(fpXC_SetRepWindow,(IOCTL_XC_SETREPWINDOW)MApi_XC_SetRepWindow_U2,MApi_XC_SetRepWindow)
        URELATION(fpXC_SkipOPWriteOffInSetWindow,(IOCTL_XC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW)MApi_XC_SkipOPWriteOffInSetWindow_U2,MApi_XC_SkipOPWriteOffInSetWindow)
        URELATION(fpXC_GetSkipOPWriteOffInSetWindow,(IOCTL_XC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW)MApi_XC_GetSkipOPWriteOffInSetWindow_U2,MApi_XC_GetSkipOPWriteOffInSetWindow)
        URELATION(fpXC_SetSkipDisableOPWriteOffInFPLL,(IOCTL_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL)MApi_XC_SetSkipDisableOPWriteOffInFPLL_U2,MApi_XC_SetSkipDisableOPWriteOffInFPLL)
        URELATION(fpXC_Set_OPWriteOffEnable,(IOCTL_XC_SET_OPWRITEOFFENABLE)MApi_XC_Set_OPWriteOffEnable_U2,MApi_XC_Set_OPWriteOffEnable)
        URELATION(fpXC_Set_OPWriteOffEnableToReg,(IOCTL_XC_SET_OPWRITEOFF_ENABLE_TO_REG)MApi_XC_Set_OPWriteOffEnableToReg_U2,MApi_XC_Set_OPWriteOffEnableToReg)
        URELATION(fpXC_ForceSet_OPWriteOffEnable,(IOCTL_XC_FORCESET_OPWRITEOFF_ENABLE)MApi_XC_ForceSet_OPWriteOffEnable_U2,MApi_XC_ForceSet_OPWriteOffEnable)
        URELATION(fpXC_Get_OPWriteOffEnable,(IOCTL_XC_GET_OPWRITEOFFENABLE)MApi_XC_Get_OPWriteOffEnable_U2,MApi_XC_Get_OPWriteOffEnable)
        URELATION(fpXC_SetDispWinToReg,(IOCTL_XC_SETDISPWINTOREG)MApi_XC_SetDispWinToReg_U2,MApi_XC_SetDispWinToReg)
        URELATION(fpXC_GetDispWinFromReg,(IOCTL_XC_GETDISPWINFROMREG)MApi_XC_GetDispWinFromReg_U2,MApi_XC_GetDispWinFromReg)
        URELATION(fpXC_FreezeImg,(IOCTL_XC_FREEZEIMG)MApi_XC_FreezeImg_U2,MApi_XC_FreezeImg)
        URELATION(fpXC_IsFreezeImg,(IOCTL_XC_ISFREEZEIMG)MApi_XC_IsFreezeImg_U2,MApi_XC_IsFreezeImg)
        URELATION(fpXC_GenerateBlackVideoForBothWin,(IOCTL_XC_GENERATEBLACKVIDEOFORBOTHWIN)MApi_XC_GenerateBlackVideoForBothWin_U2,MApi_XC_GenerateBlackVideoForBothWin)
        URELATION(fpXC_Set_BLSK,(IOCTL_XC_SET_BLSK)MApi_XC_Set_BLSK_U2,MApi_XC_Set_BLSK)
        URELATION(fpXC_GenerateBlackVideo,(IOCTL_XC_GENERATEBLACKVIDEO)MApi_XC_GenerateBlackVideo_U2,MApi_XC_GenerateBlackVideo)
        URELATION(fpXC_IsBlackVideoEnable,(IOCTL_XC_ISBLACKVIDEOENABLE)MApi_XC_IsBlackVideoEnable_U2,MApi_XC_IsBlackVideoEnable)
        URELATION(fpXC_EnableFrameBufferLess,(IOCTL_XC_ENABLEFRAMEBUFFERLESS)MApi_XC_EnableFrameBufferLess_U2,MApi_XC_EnableFrameBufferLess)
        URELATION(fpXC_IsCurrentFrameBufferLessMode,(IOCTL_XC_ISCURRENTFRAMEBUFFERLESSMODE)MApi_XC_IsCurrentFrameBufferLessMode_U2,MApi_XC_IsCurrentFrameBufferLessMode)
        URELATION(fpXC_EnableRequest_FrameBufferLess,(IOCTL_XC_ENABLEREQUEST_FRAMEBUFFERLESS)MApi_XC_EnableRequest_FrameBufferLess_U2,MApi_XC_EnableRequest_FrameBufferLess)
        URELATION(fpXC_IsCurrentRequest_FrameBufferLessMode,(IOCTL_XC_ISCURRENTREQUEST_FRAMEBUFFERLESSMODE)MApi_XC_IsCurrentRequest_FrameBufferLessMode_U2,MApi_XC_IsCurrentRequest_FrameBufferLessMode)
        URELATION(fpXC_Get_3D_HW_Version,(IOCTL_XC_GET_3D_HW_VERSION)MApi_XC_Get_3D_HW_Version_U2,MApi_XC_Get_3D_HW_Version)
        URELATION(fpXC_Get_3D_IsSupportedHW2DTo3D,(IOCTL_XC_GET_3D_ISSUPPORTEDHW2DTO3D)MApi_XC_Get_3D_IsSupportedHW2DTo3D_U2,MApi_XC_Get_3D_IsSupportedHW2DTo3D)
        URELATION(fpXC_Set_3D_Mode,(IOCTL_XC_SET_3D_MODE)MApi_XC_Set_3D_Mode_U2,MApi_XC_Set_3D_Mode)
        URELATION(fpXC_Set_3D_MainWin_FirstMode,(IOCTL_XC_SET_3D_MAINWIN_FIRSTMODE)MApi_XC_Set_3D_MainWin_FirstMode_U2,MApi_XC_Set_3D_MainWin_FirstMode)
        URELATION(fpXC_Set_3D_LR_Frame_Exchg,(IOCTL_XC_SET_3D_LR_FRAME_EXCHG)MApi_XC_Set_3D_LR_Frame_Exchg_U2,MApi_XC_Set_3D_LR_Frame_Exchg)
        URELATION(fpXC_3D_Is_LR_Frame_Exchged,(IOCTL_XC_3D_IS_LR_FRAME_EXCHGED)MApi_XC_3D_Is_LR_Frame_Exchged_U2,MApi_XC_3D_Is_LR_Frame_Exchged)
        URELATION(fpXC_Get_3D_Input_Mode,(IOCTL_XC_GET_3D_INPUT_MODE)MApi_XC_Get_3D_Input_Mode_U2,MApi_XC_Get_3D_Input_Mode)
        URELATION(fpXC_Get_3D_Output_Mode,(IOCTL_XC_GET_3D_OUTPUT_MODE)MApi_XC_Get_3D_Output_Mode_U2,MApi_XC_Get_3D_Output_Mode)
        URELATION(fpXC_Get_3D_Panel_Type,(IOCTL_XC_GET_3D_PANEL_TYPE)MApi_XC_Get_3D_Panel_Type_U2,MApi_XC_Get_3D_Panel_Type)
        URELATION(fpXC_Get_3D_MainWin_First,(IOCTL_XC_GET_3D_MAINWIN_FIRST)MApi_XC_Get_3D_MainWin_First_U2,MApi_XC_Get_3D_MainWin_First)
        URELATION(fpXC_3DMainSub_IPSync,(IOCTL_XC_3DMAINSUB_IPSYNC)MApi_XC_3DMainSub_IPSync_U2,MApi_XC_3DMainSub_IPSync)
        URELATION(fpXC_Set_3D_VerVideoOffset,(IOCTL_XC_SET_3D_VERVIDEOOFFSET)MApi_XC_Set_3D_VerVideoOffset_U2,MApi_XC_Set_3D_VerVideoOffset)
        URELATION(fpXC_Get_3D_VerVideoOffset,(IOCTL_XC_GET_3D_VERVIDEOOFFSET)MApi_XC_Get_3D_VerVideoOffset_U2,MApi_XC_Get_3D_VerVideoOffset)
        URELATION(fpXC_Is3DFormatSupported,(IOCTL_XC_IS3DFORMATSUPPORTED)MApi_XC_Is3DFormatSupported_U2,MApi_XC_Is3DFormatSupported)
        URELATION(fpXC_Set_3D_HShift,(IOCTL_XC_SET_3D_HSHIFT)MApi_XC_Set_3D_HShift_U2,MApi_XC_Set_3D_HShift)
        URELATION(fpXC_Enable_3D_LR_Sbs2Line,(IOCTL_XC_ENABLE_3D_LR_SBS2LINE)MApi_XC_Enable_3D_LR_Sbs2Line_U2,MApi_XC_Enable_3D_LR_Sbs2Line)
        URELATION(fpXC_Get_3D_HShift,(IOCTL_XC_GET_3D_HSHIFT)MApi_XC_Get_3D_HShift_U2,MApi_XC_Get_3D_HShift)
        URELATION(fpXC_Set_3D_HW2DTo3D_Buffer,(IOCTL_XC_SET_3D_HW2DTO3D_BUFFER)MApi_XC_Set_3D_HW2DTo3D_Buffer_U2,MApi_XC_Set_3D_HW2DTo3D_Buffer)
        URELATION(fpXC_Set_3D_HW2DTo3D_Parameters,(IOCTL_XC_SET_3D_HW2DTO3D_PARAMETERS)MApi_XC_Set_3D_HW2DTo3D_Parameters_U2,MApi_XC_Set_3D_HW2DTo3D_Parameters)
        URELATION(fpXC_Get_3D_HW2DTo3D_Parameters,(IOCTL_XC_GET_3D_HW2DTO3D_PARAMETERS)MApi_XC_Get_3D_HW2DTo3D_Parameters_U2,MApi_XC_Get_3D_HW2DTo3D_Parameters)
        URELATION(fpXC_Set_3D_Detect3DFormat_Parameters,(IOCTL_XC_SET_3D_DETECT3DFORMAT_PARAMETERS)MApi_XC_Set_3D_Detect3DFormat_Parameters_U2,MApi_XC_Set_3D_Detect3DFormat_Parameters)
        URELATION(fpXC_Get_3D_Detect3DFormat_Parameters,(IOCTL_XC_GET_3D_DETECT3DFORMAT_PARAMETERS)MApi_XC_Get_3D_Detect3DFormat_Parameters_U2,MApi_XC_Get_3D_Detect3DFormat_Parameters)
        URELATION(fpXC_Detect3DFormatByContent,(IOCTL_XC_DETECT3DFORMATBYCONTENT)MApi_XC_Detect3DFormatByContent_U2,MApi_XC_Detect3DFormatByContent)
        URELATION(fpXC_DetectNL,(IOCTL_XC_DETECTNL)MApi_XC_DetectNL_U2,MApi_XC_DetectNL)
        URELATION(fpXC_3D_PostPQSetting,(IOCTL_XC_3D_POSTPQSETTING)MApi_XC_3D_PostPQSetting_U2,MApi_XC_3D_PostPQSetting)
        URELATION(fpXC_Set_3D_FPInfo,(IOCTL_XC_SET_3D_FPINFO)MApi_XC_Set_3D_FPInfo_U2,MApi_XC_Set_3D_FPInfo)
        URELATION(fpXC_EnableAutoDetect3D,(IOCTL_XC_ENABLEAUTODETECT3D)MApi_XC_EnableAutoDetect3D_U2,MApi_XC_EnableAutoDetect3D)
        URELATION(fpXC_GetAutoDetect3DFlag,(IOCTL_XC_GETAUTODETECT3DFLAG)MApi_XC_GetAutoDetect3DFlag_U2,MApi_XC_GetAutoDetect3DFlag)
        URELATION(fpXC_Set_3D_SubWinClk,(IOCTL_XC_SET_3D_SUBWINCLK)MApi_XC_Set_3D_SubWinClk_U2,MApi_XC_Set_3D_SubWinClk)
        URELATION(fpXC_3D_Is_LR_Sbs2Line,(IOCTL_XC_3D_IS_LR_SBS2LINE)MApi_XC_3D_Is_LR_Sbs2Line_U2,MApi_XC_3D_Is_LR_Sbs2Line)
        URELATION(fpSC_3D_Is_Skip_Default_LR_Flag,(IOCTL_SC_3D_IS_SKIP_DEFAULT_LR_FLAG)MApi_SC_3D_Is_Skip_Default_LR_Flag_U2,MApi_SC_3D_Is_Skip_Default_LR_Flag)
        URELATION(fpXC_3D_Enable_Skip_Default_LR_Flag,(IOCTL_XC_3D_ENABLE_SKIP_DEFAULT_LR_FLAG)MApi_XC_3D_Enable_Skip_Default_LR_Flag_U2,MApi_XC_3D_Enable_Skip_Default_LR_Flag)
        URELATION(fpXC_Mux_Init,(IOCTL_XC_MUX_INIT)MApi_XC_Mux_Init_U2,MApi_XC_Mux_Init)
        URELATION(fpXC_Mux_SourceMonitor,(IOCTL_XC_MUX_SOURCEMONITOR)MApi_XC_Mux_SourceMonitor_U2,MApi_XC_Mux_SourceMonitor)
        URELATION(fpXC_Mux_CreatePath,(IOCTL_XC_MUX_CREATEPATH)MApi_XC_Mux_CreatePath_U2,MApi_XC_Mux_CreatePath)
        URELATION(fpXC_Mux_DeletePath,(IOCTL_XC_MUX_DELETEPATH)MApi_XC_Mux_DeletePath_U2,MApi_XC_Mux_DeletePath)
        URELATION(fpXC_Mux_EnablePath,(IOCTL_XC_MUX_ENABLEPATH)MApi_XC_Mux_EnablePath_U2,MApi_XC_Mux_EnablePath)
        URELATION(fpXC_Mux_TriggerPathSyncEvent,(IOCTL_XC_MUX_TRIGGERPATHSYNCEVENT)MApi_XC_Mux_TriggerPathSyncEvent_U2,MApi_XC_Mux_TriggerPathSyncEvent)
        URELATION(fpXC_Mux_TriggerDestOnOffEvent,(IOCTL_XC_MUX_TRIGGERDESTONOFFEVENT)MApi_XC_Mux_TriggerDestOnOffEvent_U2,MApi_XC_Mux_TriggerDestOnOffEvent)
        URELATION(fpXC_Mux_OnOffPeriodicHandler,(IOCTL_XC_MUX_ONOFFPERIODICHANDLER)MApi_XC_Mux_OnOffPeriodicHandler_U2,MApi_XC_Mux_OnOffPeriodicHandler)
        URELATION(fpXC_Mux_GetPathInfo,(IOCTL_XC_MUX_GETPATHINFO)MApi_XC_Mux_GetPathInfo_U2,MApi_XC_Mux_GetPathInfo)
        URELATION(fpXC_Mux_SetSupportMhlPathInfo,(IOCTL_XC_MUX_SETSUPPORTMHLPATHINFO)MApi_XC_Mux_SetSupportMhlPathInfo_U2,MApi_XC_Mux_SetSupportMhlPathInfo)
        URELATION(fpXC_Mux_SetMhlHotPlugInverseInfo,(IOCTL_XC_MUX_SETMHLHOTPLUGINVERSEINFO)MApi_XC_Mux_SetMhlHotPlugInverseInfo_U2,MApi_XC_Mux_SetMhlHotPlugInverseInfo)
        URELATION(fpXC_Mux_GetHDMIPort,(IOCTL_XC_MUX_GETHDMIPORT)MApi_XC_Mux_GetHDMIPort_U2,MApi_XC_Mux_GetHDMIPort)
        URELATION(fpXC_Mux_GetMappingTab,(IOCTL_XC_MUX_GETMAPPINGTAB)MApi_XC_Mux_GetPortMappingMatrix_U2,MApi_XC_Mux_GetPortMappingMatrix)
        URELATION(fpXC_MUX_MApinputSourceToVDYMuxPORT,(IOCTL_XC_MUX_MAPINPUTSOURCETOVDYMUXPORT)MApi_XC_MUX_MapInputSourceToVDYMuxPORT_U2,MApi_XC_MUX_MapInputSourceToVDYMuxPORT)
        URELATION(fpXC_Set_NR,(IOCTL_XC_SET_NR)MApi_XC_Set_NR_U2,MApi_XC_Set_NR)
        URELATION(fpXC_FilmMode_P,(IOCTL_XC_FILMMODE_P)MApi_XC_FilmMode_P_U2,MApi_XC_FilmMode_P)
        URELATION(fpXC_GetUCEnabled,(IOCTL_XC_GETUCENABLED)MApi_XC_GetUCEnabled_U2,MApi_XC_GetUCEnabled)
        URELATION(fpXC_GenSpecificTiming,(IOCTL_XC_GENSPECIFICTIMING)MApi_XC_GenSpecificTiming_U2,MApi_XC_GenSpecificTiming)
        URELATION(fpXC_GetDEBypassMode,(IOCTL_XC_GETDEBYPASSMODE)MApi_XC_GetDEBypassMode_U2,MApi_XC_GetDEBypassMode)
        URELATION(fpXC_GetDEWindow,(IOCTL_XC_GETDEWINDOW)MApi_XC_GetDEWindow_U2,MApi_XC_GetDEWindow)
        URELATION(fpXC_GetDEWidthHeightInDEByPassMode,(IOCTL_XC_GETDEWIDTHHEIGHTINDEBYPASSMODE)MApi_XC_GetDEWidthHeightInDEByPassMode_U2,MApi_XC_GetDEWidthHeightInDEByPassMode)
        URELATION(fpXC_GetCaptureWindow,(IOCTL_XC_GETCAPTUREWINDOW)MApi_XC_GetCaptureWindow_U2,MApi_XC_GetCaptureWindow)
        URELATION(fpXC_SetCaptureWindowVstart,(IOCTL_XC_SETCAPTUREWINDOWVSTART)MApi_XC_SetCaptureWindowVstart_U2,MApi_XC_SetCaptureWindowVstart)
        URELATION(fpXC_SetCaptureWindowHstart,(IOCTL_XC_SETCAPTUREWINDOWHSTART)MApi_XC_SetCaptureWindowHstart_U2,MApi_XC_SetCaptureWindowHstart)
        URELATION(fpXC_SetCaptureWindowVsize,(IOCTL_XC_SETCAPTUREWINDOWVSIZE)MApi_XC_SetCaptureWindowVsize_U2,MApi_XC_SetCaptureWindowVsize)
        URELATION(fpXC_SetCaptureWindowHsize,(IOCTL_XC_SETCAPTUREWINDOWHSIZE)MApi_XC_SetCaptureWindowHsize_U2,MApi_XC_SetCaptureWindowHsize)
        URELATION(fpXC_SoftwareReset,(IOCTL_XC_SOFTWARERESET)MApi_XC_SoftwareReset_U2,MApi_XC_SoftwareReset)
        URELATION(fpXC_CalculateHFreqx10,(IOCTL_XC_CALCULATEHFREQX10)MApi_XC_CalculateHFreqx10_U2,MApi_XC_CalculateHFreqx10)
        URELATION(fpXC_CalculateHFreqx1K,(IOCTL_XC_CALCULATEHFREQX1K)MApi_XC_CalculateHFreqx1K_U2,MApi_XC_CalculateHFreqx1K)
        URELATION(fpXC_CalculateVFreqx10,(IOCTL_XC_CALCULATEVFREQX10)MApi_XC_CalculateVFreqx10_U2,MApi_XC_CalculateVFreqx10)
        URELATION(fpXC_CalculateVFreqx1K,(IOCTL_XC_CALCULATEVFREQX1K)MApi_XC_CalculateVFreqx1K_U2,MApi_XC_CalculateVFreqx1K)
        URELATION(fpXC_GetAccurateVFreqx1K,(IOCTL_XC_GETACCURATEVFREQX1K)MApi_XC_GetAccurateVFreqx1K_U2,MApi_XC_GetAccurateVFreqx1K)
        URELATION(fpXC_InterruptAttach,(IOCTL_XC_INTERRUPTATTACH)MApi_XC_InterruptAttach_U2,MApi_XC_InterruptAttach)
        URELATION(fpXC_InterruptDeAttach,(IOCTL_XC_INTERRUPTDEATTACH)MApi_XC_InterruptDeAttach_U2,MApi_XC_InterruptDeAttach)
        URELATION(fpXC_DisableInputSource,(IOCTL_XC_DISABLEINPUTSOURCE)MApi_XC_DisableInputSource_U2,MApi_XC_DisableInputSource)
        URELATION(fpXC_IsInputSourceDisabled,(IOCTL_XC_ISINPUTSOURCEDISABLED)MApi_XC_IsInputSourceDisabled_U2,MApi_XC_IsInputSourceDisabled)
        URELATION(fpXC_ChangePanelType,(IOCTL_XC_CHANGEPANELTYPE)MApi_XC_ChangePanelType_U2,MApi_XC_ChangePanelType)
        URELATION(fpXC_GetCurrentReadBank,(IOCTL_XC_GETCURRENTREADBANK)MApi_XC_GetCurrentReadBank_U2,MApi_XC_GetCurrentReadBank)
        URELATION(fpXC_GetCurrentWriteBank,(IOCTL_XC_GETCURRENTWRITEBANK)MApi_XC_GetCurrentWriteBank_U2,MApi_XC_GetCurrentWriteBank)
        URELATION(fpXC_SetAutoPreScaling,(IOCTL_XC_SETAUTOPRESCALING)MApi_XC_SetAutoPreScaling_U2,MApi_XC_SetAutoPreScaling)
        URELATION(fpXC_GetVSyncWidth,(IOCTL_XC_GETVSYNCWIDTH)MApi_XC_GetVSyncWidth_U2,MApi_XC_GetVSyncWidth)
        URELATION(fpXC_set_GOP_Enable,(IOCTL_XC_SET_GOP_ENABLE)MApi_XC_set_GOP_Enable_U2,MApi_XC_set_GOP_Enable)
        URELATION(fpXC_ip_sel_for_gop,(IOCTL_XC_IP_SEL_FOR_GOP)MApi_XC_ip_sel_for_gop_U2,MApi_XC_ip_sel_for_gop)
        URELATION(fpXC_SetVOPNBL,(IOCTL_XC_SETVOPNBL)MApi_XC_SetVOPNBL_U2,MApi_XC_SetVOPNBL)
        URELATION(fpXC_GetDstInfo,(IOCTL_XC_GETDSTINFO)MApi_XC_GetDstInfo_U2,MApi_XC_GetDstInfo)
        URELATION(fpXC_Set_FD_Mask_ByWin,(IOCTL_XC_SET_FD_MASK_BYWIN)MApi_XC_Set_FD_Mask_ByWin_U2,MApi_XC_set_FD_Mask_byWin)
        URELATION(fpXC_Get_FD_Mask_ByWin,(IOCTL_XC_GET_FD_MASK_BYWIN)MApi_XC_Get_FD_Mask_ByWin_U2,MApi_XC_Get_FD_Mask_byWin)
        URELATION(fpXC_Get_FD_Mask_Status,(IOCTL_XC_GET_FD_MASK_STATUS)MApi_XC_Get_FD_Mask_Status_U2,MApi_XC_Get_FD_Mask_Status)
        URELATION(fpXC_SetIP1TestPattern,(IOCTL_XC_SETIP1TESTPATTERN)MApi_XC_SetIP1TestPattern_U2,MApi_XC_SetIP1TestPattern)
        URELATION(fpXC_InitIPForInternalTiming,(IOCTL_XC_INITIPFORINTERNALTIMING)MApi_XC_InitIPForInternalTiming_U2,MApi_XC_InitIPForInternalTiming)
        URELATION(fpXC_SetIPMux,(IOCTL_XC_SETIPMUX)MApi_XC_SetIPMux_U2,MApi_XC_SetIPMux)
        URELATION(fpXC_Is_H_Sync_Active,(IOCTL_XC_IS_H_SYNC_ACTIVE)MApi_XC_Is_H_Sync_Active_U2,MApi_XC_Is_H_Sync_Active)
        URELATION(fpXC_Is_V_Sync_Active,(IOCTL_XC_IS_V_SYNC_ACTIVE)MApi_XC_Is_V_Sync_Active_U2,MApi_XC_Is_V_Sync_Active)
        URELATION(fpXC_GetAutoPositionWindow,(IOCTL_XC_GETAUTOPOSITIONWINDOW)MApi_XC_GetAutoPositionWindow_U2,MApi_XC_GetAutoPositionWindow)
        URELATION(fpXC_SetFrameBufferAddress,(IOCTL_XC_SETFRAMEBUFFERADDRESS)MApi_XC_SetFrameBufferAddress_U2,MApi_XC_SetFrameBufferAddress)
        URELATION(fpXC_SetFRCMFrameBufferAddress,(IOCTL_XC_SETFRCMFRAMEBUFFERADDRESS)MApi_XC_SetFRCMFrameBufferAddress_U2,MApi_XC_SetFRCMFrameBufferAddress)
        URELATION(fpXC_IsFrameBufferEnoughForCusScaling,(IOCTL_XC_ISFRAMEBUFFERENOUGHFORCUSSCALING)MApi_XC_IsFrameBufferEnoughForCusScaling_U2,MApi_XC_IsFrameBufferEnoughForCusScaling)
        URELATION(fpXC_SetScalerMemoryRequest,(IOCTL_XC_SETSCALERMEMORYREQUEST)MApi_XC_SetScalerMemoryRequest_U2,MApi_XC_SetScalerMemoryRequest)
        URELATION(fpXC_Get_PixelData,(IOCTL_XC_GET_PIXELDATA)MApi_XC_Get_PixelData_U2,MApi_XC_Get_PixelData)
        URELATION(fpXC_GetAvailableSize,(IOCTL_XC_GETAVAILABLESIZE)MApi_XC_GetAvailableSize_U2,MApi_XC_GetAvailableSize)
        URELATION(fpXC_SetFrameColor,(IOCTL_XC_SETFRAMECOLOR)MApi_XC_SetFrameColor_U2,MApi_XC_SetFrameColor)
        URELATION(fpXC_SetDispWindowColor,(IOCTL_XC_SETDISPWINDOWCOLOR)MApi_XC_SetDispWindowColor_U2,MApi_XC_SetDispWindowColor)
        URELATION(fpXC_SupportSourceToVE,(IOCTL_XC_SUPPORTSOURCETOVE)MApi_XC_SupportSourceToVE_U2,MApi_XC_SupportSourceToVE)
        URELATION(fpXC_SetOutputCapture,(IOCTL_XC_SETOUTPUTCAPTURE)MApi_XC_SetOutputCapture_U2,MApi_XC_SetOutputCapture)
        URELATION(fpXC_SetGammaOnOff,(IOCTL_XC_SETGAMMAONOFF)MApi_XC_SetGammaOnOff_U2,MApi_XC_SetGammaOnOff)
        URELATION(fpXC_SetPreGammaGain,(IOCTL_XC_SETPREGAMMAGAIN)MApi_XC_SetPreGammaGain_U2,MApi_XC_SetPreGammaGain)
        URELATION(fpXC_SetPreGammaOffset,(IOCTL_XC_SETPREGAMMAOFFSET)MApi_XC_SetPreGammaOffset_U2,MApi_XC_SetPreGammaOffset)
        URELATION(fpXC_SetPanelTiming,(IOCTL_XC_SETPANELTIMING)MApi_XC_SetPanelTiming_U2,MApi_XC_SetPanelTiming)
        URELATION(fpXC_SetOutTimingMode,(IOCTL_XC_SETOUTTIMINGMODE)MApi_XC_SetOutTimingMode_U2,MApi_XC_SetOutTimingMode)
        URELATION(fpXC_SetFreeRunTiming,(IOCTL_XC_SETFREERUNTIMING)MApi_XC_SetFreeRunTiming_U2,MApi_XC_SetFreeRunTiming)
        URELATION(fpXC_Set_CustomerSyncInfo,(IOCTL_XC_SET_CUSTOMERSYNCINFO)MApi_XC_Set_CustomerSyncInfo_U2,MApi_XC_Set_CustomerSyncInfo)
        URELATION(fpXC_WaitFPLLDone,(IOCTL_XC_WAITFPLLDONE)MApi_XC_WaitFPLLDone_U2,MApi_XC_WaitFPLLDone)
        URELATION(fpXC_GetOutputVFreqX100,(IOCTL_XC_GETOUTPUTVFREQX100)MApi_XC_GetOutputVFreqX100_U2,MApi_XC_GetOutputVFreqX100)
        URELATION(fpXC_GetOP1OutputVFreqX100,(IOCTL_XC_GETOP1OUTPUTVFREQX100)MApi_XC_GetOP1OutputVFreqX100_U2,MApi_XC_GetOP1OutputVFreqX100)
        URELATION(fpXC_FrameLockCheck,(IOCTL_XC_FRAMELOCKCHECK)MApi_XC_FrameLockCheck_U2,MApi_XC_FrameLockCheck)
        URELATION(fpXC_CustomizeFRCTable,(IOCTL_XC_CUSTOMIZEFRCTABLE)MApi_XC_CustomizeFRCTable_U2,MApi_XC_CustomizeFRCTable)
        URELATION(fpXC_OutputFrameCtrl,(IOCTL_XC_OUTPUTFRAMECTRL)MApi_XC_OutputFrameCtrl_U2,MApi_XC_OutputFrameCtrl)
        URELATION(fpSC_Enable_FPLL_FSM,(IOCTL_SC_ENABLE_FPLL_FSM)MApi_SC_Enable_FPLL_FSM_U2,MApi_SC_Enable_FPLL_FSM)
        URELATION(fpSC_ForceFreerun,(IOCTL_SC_FORCEFREERUN)MApi_SC_ForceFreerun_U2,MApi_SC_ForceFreerun)
        URELATION(fpSC_IsForceFreerun,(IOCTL_SC_ISFORCEFREERUN)MApi_SC_IsForceFreerun_U2,MApi_SC_IsForceFreerun)
        URELATION(fpSC_SetFreerunVFreq,(IOCTL_SC_SETFREERUNVFREQ)MApi_SC_SetFreerunVFreq_U2,MApi_SC_SetFreerunVFreq)
        URELATION(fpXC_SetExPanelInfo,(IOCTL_XC_SETEXPANELINFO)MApi_XC_SetExPanelInfo_U2,MApi_XC_SetExPanelInfo)
        URELATION(fpXC_Enable_FPLL_Thresh_Mode,(IOCTL_XC_ENABLE_FPLL_THRESH_MODE)MApi_XC_Enable_FPLL_Thresh_Mode_U2,MApi_XC_Enable_FPLL_Thresh_Mode)
        URELATION(fpXC_Get_FPLL_Thresh_Mode,(IOCTL_XC_GET_FPLL_THRESH_MODE)MApi_XC_Get_FPLL_Thresh_Mode_U2,MApi_XC_Get_FPLL_Thresh_Mode)
        URELATION(fpXC_EnableIPAutoNoSignal,(IOCTL_XC_ENABLEIPAUTONOSIGNAL)MApi_XC_EnableIPAutoNoSignal_U2,MApi_XC_EnableIPAutoNoSignal)
        URELATION(fpXC_GetIPAutoNoSignal,(IOCTL_XC_GETIPAUTONOSIGNAL)MApi_XC_GetIPAutoNoSignal_U2,MApi_XC_GetIPAutoNoSignal)
        URELATION(fpXC_EnableIPAutoCoast,(IOCTL_XC_ENABLEIPAUTOCOAST)MApi_XC_EnableIPAutoCoast_U2,MApi_XC_EnableIPAutoCoast)
        URELATION(fpXC_EnableIPCoastDebounce,(IOCTL_XC_ENABLEIPCOASTDEBOUNCE)MApi_XC_EnableIPCoastDebounce_U2,MApi_XC_EnableIPCoastDebounce)
        URELATION(fpXC_ClearIPCoastStatus,(IOCTL_XC_CLEARIPCOASTSTATUS)MApi_XC_ClearIPCoastStatus_U2,MApi_XC_ClearIPCoastStatus)
        URELATION(fpXC_EnableFpllManualSetting,(IOCTL_XC_ENABLEFPLLMANUALSETTING)MApi_XC_EnableFpllManualSetting_U2,MApi_XC_EnableFpllManualSetting)
        URELATION(fpXC_FpllBoundaryTest,(IOCTL_XC_FPLLBOUNDARYTEST)MApi_XC_FpllBoundaryTest_U2,MApi_XC_FpllBoundaryTest)
        URELATION(fpXC_SetOffLineDetection,(IOCTL_XC_SETOFFLINEDETECTION)MApi_XC_SetOffLineDetection_U2,MApi_XC_SetOffLineDetection)
        URELATION(fpXC_GetOffLineDetection,(IOCTL_XC_GETOFFLINEDETECTION)MApi_XC_GetOffLineDetection_U2,MApi_XC_GetOffLineDetection)
        URELATION(fpXC_SetOffLineSogThreshold,(IOCTL_XC_SETOFFLINESOGTHRESHOLD)MApi_XC_SetOffLineSogThreshold_U2,MApi_XC_SetOffLineSogThreshold)
        URELATION(fpXC_SetOffLineSogBW,(IOCTL_XC_SETOFFLINESOGBW)MApi_XC_SetOffLineSogBW_U2,MApi_XC_SetOffLineSogBW)
        URELATION(fpXC_OffLineInit,(IOCTL_XC_OFFLINEINIT)MApi_XC_OffLineInit_U2,MApi_XC_OffLineInit)
        URELATION(fpXC_Set_Extra_fetch_adv_line,(IOCTL_XC_SET_EXTRA_FETCH_ADV_LINE)MApi_XC_Set_Extra_fetch_adv_line_U2,MApi_XC_Set_Extra_fetch_adv_line)
        URELATION(fpXC_SetVGASogEn,(IOCTL_XC_SETVGASOGEN)MApi_XC_SetVGASogEn_U2,MApi_XC_SetVGASogEn)
        URELATION(fpXC_EnableWindow,(IOCTL_XC_ENABLEWINDOW)MApi_XC_EnableWindow_U2,MApi_XC_EnableWindow)
        URELATION(fpXC_Is_SubWindowEanble,(IOCTL_XC_IS_SUBWINDOWEANBLE)MApi_XC_Is_SubWindowEanble_U2,MApi_XC_Is_SubWindowEanble)
        URELATION(fpXC_SetBorderFormat,(IOCTL_XC_SETBORDERFORMAT)MApi_XC_SetBorderFormat_U2,MApi_XC_SetBorderFormat)
        URELATION(fpXC_EnableBorder,(IOCTL_XC_ENABLEBORDER)MApi_XC_EnableBorder_U2,MApi_XC_EnableBorder)
        URELATION(fpXC_ZorderMainWindowFirst,(IOCTL_XC_ZORDERMAINWINDOWFIRST)MApi_XC_ZorderMainWindowFirst_U2,MApi_XC_ZorderMainWindowFirst)
        URELATION(fpXC_PQ_LoadFunction,(IOCTL_XC_PQ_LOADFUNCTION)MApi_XC_PQ_LoadFunction_U2,MApi_XC_PQ_LoadFunction)
        URELATION(fpXC_Check_HNonLinearScaling,(IOCTL_XC_CHECK_HNONLINEARSCALING)MApi_XC_Check_HNonLinearScaling_U2,MApi_XC_Check_HNonLinearScaling)
        URELATION(fpXC_EnableEuroHdtvSupport,(IOCTL_XC_ENABLEEUROHDTVSUPPORT)MApi_XC_EnableEuroHdtvSupport_U2,MApi_XC_EnableEuroHdtvSupport)
        URELATION(fpXC_EnableEuroHdtvDetection,(IOCTL_XC_ENABLEEUROHDTVDETECTION)MApi_XC_EnableEuroHdtvDetection_U2,MApi_XC_EnableEuroHdtvDetection)
        URELATION(fpXC_ReadByte,(IOCTL_XC_READBYTE)MApi_XC_ReadByte_U2,MApi_XC_ReadByte)
        URELATION(fpXC_WriteByte,(IOCTL_XC_WRITEBYTE)MApi_XC_WriteByte_U2,MApi_XC_WriteByte)
        URELATION(fpXC_WriteByteMask,(IOCTL_XC_WRITEBYTEMASK)MApi_XC_WriteByteMask_U2,MApi_XC_WriteByteMask)
        URELATION(fpXC_Write2ByteMask,(IOCTL_XC_WRITE2BYTEMASK)MApi_XC_Write2ByteMask_U2,MApi_XC_Write2ByteMask)
        URELATION(fpXC_W2BYTE,(IOCTL_XC_W2BYTE)MApi_XC_W2BYTE_U2,MApi_XC_W2BYTE)
        URELATION(fpXC_R2BYTE,(IOCTL_XC_R2BYTE)MApi_XC_R2BYTE_U2,MApi_XC_R2BYTE)
        URELATION(fpXC_W4BYTE,(IOCTL_XC_W4BYTE)MApi_XC_W4BYTE_U2,MApi_XC_W4BYTE)
        URELATION(fpXC_R4BYTE,(IOCTL_XC_R4BYTE)MApi_XC_R4BYTE_U2,MApi_XC_R4BYTE)
        URELATION(fpXC_R2BYTEMSK,(IOCTL_XC_R2BYTEMSK)MApi_XC_R2BYTEMSK_U2,MApi_XC_R2BYTEMSK)
        URELATION(fpXC_W2BYTEMSK,(IOCTL_XC_W2BYTEMSK)MApi_XC_W2BYTEMSK_U2,MApi_XC_W2BYTEMSK)
        URELATION(fpXC_MLoad_Init,(IOCTL_XC_MLOAD_INIT)MApi_XC_MLoad_Init_U2,MApi_XC_MLoad_Init)
        URELATION(fpXC_MLoad_Enable,(IOCTL_XC_MLOAD_ENABLE)MApi_XC_MLoad_Enable_U2,MApi_XC_MLoad_Enable)
        URELATION(fpXC_MLoad_Cus_Init,(IOCTL_XC_MLOAD_CUS_INIT)MApi_XC_MLoad_Cus_Init_U2,MApi_XC_MLoad_Cus_Init)
        URELATION(fpXC_MLoad_Cus_Enable,(IOCTL_XC_MLOAD_CUS_ENABLE)MApi_XC_MLoad_Cus_Enable_U2,MApi_XC_MLoad_Cus_Enable)
        URELATION(fpXC_MLoad_GetStatus,(IOCTL_XC_MLOAD_GETSTATUS)MApi_XC_MLoad_GetStatus_U2,MApi_XC_MLoad_GetStatus)
        URELATION(fpXC_MLoad_WriteCmd_And_Fire,(IOCTL_XC_MLOAD_WRITECMD_AND_FIRE)MApi_XC_MLoad_WriteCmd_And_Fire_U2,MApi_XC_MLoad_WriteCmd_And_Fire)
        URELATION(fpXC_MLoad_WriteCmds_And_Fire,(IOCTL_XC_MLOAD_WRITECMDS_AND_FIRE)MApi_XC_MLoad_WriteCmds_And_Fire_U2,MApi_XC_MLoad_WriteCmds_And_Fire)
        URELATION(fpXC_MLoad_Combine_Enable,(IOCTL_XC_MLOAD_COMBINE)MApi_XC_Combine_MLoadEn_U2,MApi_XC_Combine_MLoadEn)
        URELATION(fpXC_MLG_Init,(IOCTL_XC_MLG_INIT)MApi_XC_MLG_Init_U2,MApi_XC_MLG_Init)
        URELATION(fpXC_MLG_Enable,(IOCTL_XC_MLG_ENABLE)MApi_XC_MLG_Enable_U2,MApi_XC_MLG_Enable)
        URELATION(fpXC_MLG_GetCaps,(IOCTL_XC_MLG_GETCAPS)MApi_XC_MLG_GetCaps_U2,MApi_XC_MLG_GetCaps)
        URELATION(fpXC_MLG_GetStatus,(IOCTL_XC_MLG_GETSTATUS)MApi_XC_MLG_GetStatus_U2,MApi_XC_MLG_GetStatus)
        URELATION(fpXC_SetOSD2VEMode,(IOCTL_XC_SETOSD2VEMODE)MApi_XC_SetOSD2VEMode_U2,MApi_XC_SetOSD2VEMode)
        URELATION(fpXC_IP2_PreFilter_Enable,(IOCTL_XC_IP2_PREFILTER_ENABLE)MApi_XC_IP2_PreFilter_Enable_U2,MApi_XC_IP2_PreFilter_Enable)
        URELATION(fpXC_Get_Pixel_RGB,(IOCTL_XC_GET_PIXEL_RGB)MApi_XC_Get_Pixel_RGB_U2,MApi_XC_Get_Pixel_RGB)
        URELATION(fpXC_KeepPixelPointerAppear,(IOCTL_XC_KEEPPIXELPOINTERAPPEAR)MApi_XC_KeepPixelPointerAppear_U2,MApi_XC_KeepPixelPointerAppear)
        URELATION(fpXC_Set_MemFmtEx,(IOCTL_XC_SET_MEMFMTEX)MApi_XC_Set_MemFmtEx_U2,MApi_XC_Set_MemFmtEx)
        URELATION(fpXC_IsRequestFrameBufferLessMode,(IOCTL_XC_ISREQUESTFRAMEBUFFERLESSMODE)MApi_XC_IsRequestFrameBufferLessMode_U2,MApi_XC_IsRequestFrameBufferLessMode)
        URELATION(fpXC_SkipSWReset,(IOCTL_XC_SKIPSWRESET)MApi_XC_SkipSWReset_U2,MApi_XC_SkipSWReset)
        URELATION(fpXC_EnableRepWindowForFrameColor,(IOCTL_XC_ENABLEREPWINDOWFORFRAMECOLOR)MApi_XC_EnableRepWindowForFrameColor_U2,MApi_XC_EnableRepWindowForFrameColor)
        URELATION(fpXC_SetOSDLayer,(IOCTL_XC_SETOSDLAYER)MApi_XC_SetOSDLayer_U2,MApi_XC_SetOSDLayer)
        URELATION(fpXC_GetOSDLayer,(IOCTL_XC_GETOSDLAYER)MApi_XC_GetOSDLayer_U2,MApi_XC_GetOSDLayer)
        URELATION(fpXC_SetVideoAlpha,(IOCTL_XC_SETVIDEOALPHA)MApi_XC_SetVideoAlpha_U2,MApi_XC_SetVideoAlpha)
        URELATION(fpXC_GetVideoAlpha,(IOCTL_XC_GETVIDEOALPHA)MApi_XC_GetVideoAlpha_U2,MApi_XC_GetVideoAlpha)
        URELATION(fpXC_SkipWaitVsync,(IOCTL_XC_SKIPWAITVSYNC)MApi_XC_SkipWaitVsync_U2,MApi_XC_SkipWaitVsync)
        URELATION(fpXC_SetCMAHeapID,(IOCTL_XC_SETCMAHEAPID)MApi_XC_SetCMAHeapID_U2,MApi_XC_SetCMAHeapID)
        URELATION(fpXC_OP2VOPDESel,(IOCTL_XC_OP2VOPDESEL)MApi_XC_OP2VOPDESel_U2,MApi_XC_OP2VOPDESel)
        URELATION(fpXC_FRC_SetWindow,(IOCTL_XC_FRC_SETWINDOW)MApi_XC_FRC_SetWindow_U2,MApi_XC_FRC_SetWindow)
        URELATION(fpXC_Enable_TwoInitFactor,(IOCTL_XC_ENABLE_TWOINITFACTOR)MApi_XC_Enable_TwoInitFactor_U2,MApi_XC_Enable_TwoInitFactor)
        URELATION(fpXC_IsFieldPackingModeSupported,(IOCTL_XC_ISFIELDPACKINGMODESUPPORTED)MApi_XC_IsFieldPackingModeSupported_U2,MApi_XC_IsFieldPackingModeSupported)
        URELATION(fpXC_PreInit,(IOCTL_XC_PREINIT)MApi_XC_PreInit_U2,MApi_XC_PreInit)
        URELATION(fpXC_Get_BufferData,(IOCTL_XC_GET_BUFFERDATA)MApi_XC_Get_BufferData_U2,MApi_XC_Get_BufferData)
        URELATION(fpXC_Set_BufferData,(IOCTL_XC_SET_BUFFERDATA)MApi_XC_Set_BufferData_U2,MApi_XC_Set_BufferData)
        URELATION(fpXC_EnableMainWindow,(IOCTL_XC_ENABLEMAINWINDOW)MApi_XC_EnableMainWindow_U2,MApi_XC_EnableMainWindow)
        URELATION(fpXC_EnableSubWindow,(IOCTL_XC_ENABLESUBWINDOW)MApi_XC_EnableSubWindow_U2,MApi_XC_EnableSubWindow)
        URELATION(fpXC_DisableSubWindow,(IOCTL_XC_DISABLESUBWINDOW)MApi_XC_DisableSubWindow_U2,MApi_XC_DisableSubWindow)
        URELATION(fpXC_SetPixelShift,(IOCTL_XC_SETPIXELSHIFT)MApi_XC_SetPixelShift_U2,MApi_XC_SetPixelShift)
        URELATION(fpXC_SetVideoOnOSD,(IOCTL_XC_SETVIDEOONOSD)MApi_XC_SetVideoOnOSD_U2,MApi_XC_SetVideoOnOSD)
        URELATION(fpXC_SetOSDLayerBlending,(IOCTL_XC_SETOSDLAYERBLENDING)MApi_XC_SetOSDLayerBlending_U2,MApi_XC_SetOSDLayerBlending)
        URELATION(fpXC_SetOSDLayerAlpha,(IOCTL_XC_SETOSDLAYERALPHA)MApi_XC_SetOSDLayerAlpha_U2,MApi_XC_SetOSDLayerAlpha)
        URELATION(fpXC_SetOSDBlendingFormula,(IOCTL_XC_SETOSDBLENDINGFORMULA)MApi_XC_SetOSDBlendingFormula_U2,MApi_XC_SetOSDBlendingFormula)
        URELATION(fpXC_ReportPixelInfo,(IOCTL_XC_REPORTPIXELINFO)MApi_XC_ReportPixelInfo_U2,MApi_XC_ReportPixelInfo)
        URELATION(fpXC_SetScaling,(IOCTL_XC_SETSCALING)MApi_XC_SetScaling_U2,MApi_XC_SetScaling)
        URELATION(fpXC_SetMCDIBufferAddress,(IOCTL_XC_SETMCDIBUFFERADDRESS)MApi_XC_SetMCDIBufferAddress_U2,MApi_XC_SetMCDIBufferAddress)
        URELATION(fpXC_EnableMCDI,(IOCTL_XC_ENABLEMCDI)MApi_XC_EnableMCDI_U2,MApi_XC_EnableMCDI)
        URELATION(fpXC_SendCmdToFRC,(IOCTL_XC_SENDCMDTOFRC)MApi_XC_SendCmdToFRC_U2,MApi_XC_SendCmdToFRC)
        URELATION(fpXC_GetMsgFromFRC,(IOCTL_XC_GETMSGFROMFRC)MApi_XC_GetMsgFromFRC_U2,MApi_XC_GetMsgFromFRC)
        URELATION(fpXC_EnableRWBankAuto,(IOCTL_XC_ENABLERWBANKAUTO)MApi_XC_EnableRWBankAuto_U2,MApi_XC_EnableRWBankAuto)
        URELATION(fpXC_SetWRBankMappingNum,(IOCTL_XC_SETWRBANKMAPPINGNUM)MApi_XC_SetWRBankMappingNum_U2,MApi_XC_SetWRBankMappingNum)
        URELATION(fpXC_GetWRBankMappingNum,(IOCTL_XC_GETWRBANKMAPPINGNUM)MApi_XC_GetWRBankMappingNum_U2,MApi_XC_GetWRBankMappingNum)
        URELATION(fpXC_GetWRBankMappingNumForZap,(IOCTL_XC_GETWRBANKMAPPINGNUMFORZAP)MApi_XC_GetWRBankMappingNumForZap_U2,MApi_XC_GetWRBankMappingNumForZap)
        URELATION(fpXC_SetBOBMode,(IOCTL_XC_SET_BOBMODE)MApi_XC_SetBOBMode_U2,MApi_XC_SetBOBMode)
        URELATION(fpXC_SetForceReadBank,(IOCTL_XC_SET_FORCE_READBANK)MApi_XC_SetForceReadBank_U2,MApi_XC_SetForceReadBank)
        URELATION(fpXC_SetLockPoint,(IOCTL_XC_SET_LOCK_POINT)MApi_XC_SetLockPoint_U2,MApi_XC_SetLockPoint)
        URELATION(fpXC_LD_Init,(IOCTL_XC_LD_INIT)MApi_XC_LD_Init_U2,MApi_XC_LD_Init)
        URELATION(fpXC_LD_SetMemoryAddress,(IOCTL_XC_LD_SETMEMORYADDRESS)MApi_XC_LD_SetMemoryAddress_U2,MApi_XC_LD_SetMemoryAddress)
        URELATION(fpXC_LD_Get_Value,(IOCTL_XC_LD_GET_VALUE)MApi_XC_LD_Get_Value_U2,MApi_XC_LD_Get_Value)
        URELATION(fpXC_LD_SetLevel,(IOCTL_XC_LD_SETLEVEL)MApi_XC_LD_SetLevel_U2,MApi_XC_LD_SetLevel)
        URELATION(fpSet_TurnoffLDBL,(IOCTL_SET_TURNOFFLDBL)MApi_Set_TurnoffLDBL_U2,MApi_Set_TurnoffLDBL)
        URELATION(fpSet_notUpdateSPIDataFlags,(IOCTL_SET_NOTUPDATESPIDATAFLAGS)MApi_Set_notUpdateSPIDataFlags_U2,MApi_Set_notUpdateSPIDataFlags)
        URELATION(fpSet_UsermodeLDFlags,(IOCTL_SET_USERMODELDFLAGS)MApi_Set_UsermodeLDFlags_U2,MApi_Set_UsermodeLDFlags)
        URELATION(fpSet_BLLevel,(IOCTL_SET_BLLEVEL)MApi_Set_BLLevel_U2,MApi_Set_BLLevel)
        URELATION(fpXC_Set_BWS_Mode,(IOCTL_XC_SET_BWS_MODE)MApi_XC_Set_BWS_Mode_U2,MApi_XC_Set_BWS_Mode)
        URELATION(fpXC_FRC_ColorPathCtrl,(IOCTL_XC_FRC_COLORPATHCTRL)MApi_XC_FRC_ColorPathCtrl_U2,MApi_XC_FRC_ColorPathCtrl)
        URELATION(fpXC_FRC_OP2_SetRGBGain,(IOCTL_XC_FRC_OP2_SETRGBGAIN)MApi_XC_FRC_OP2_SetRGBGain_U2,MApi_XC_FRC_OP2_SetRGBGain)
        URELATION(fpXC_FRC_OP2_SetRGBOffset,(IOCTL_XC_FRC_OP2_SETRGBOFFSET)MApi_XC_FRC_OP2_SetRGBOffset_U2,MApi_XC_FRC_OP2_SetRGBOffset)
        URELATION(fpXC_FRC_OP2_SetDither,(IOCTL_XC_FRC_OP2_SETDITHER)MApi_XC_FRC_OP2_SetDither_U2,MApi_XC_FRC_OP2_SetDither)
        URELATION(fpXC_FRC_BypassMFC,(IOCTL_XC_FRC_BYPASSMFC)MApi_XC_FRC_BypassMFC_U2,MApi_XC_FRC_BypassMFC)
        URELATION(fpXC_FRC_Mute,(IOCTL_XC_FRC_MUTE)MApi_XC_FRC_Mute_U2,MApi_XC_FRC_Mute)
        URELATION(fpXC_ForceReadFrame,(IOCTL_XC_FORCEREADFRAME)MApi_XC_ForceReadFrame_U2,MApi_XC_ForceReadFrame)
        URELATION(fpXC_SetCsc,(IOCTL_XC_SETCSC)MApi_XC_SetCsc_U2,MApi_XC_SetCsc)
        URELATION(fpXC_RegisterPQSetFPLLThreshMode,(IOCTL_XC_REGISTERPQSETFPLLTHRESHMODE)MApi_XC_RegisterPQSetFPLLThreshMode_U2,MApi_XC_RegisterPQSetFPLLThreshMode)
        URELATION(fpXC_GetFreeRunStatus,(IOCTL_XC_GETFREERUNSTATUS)MApi_XC_GetFreeRunStatus_U2,MApi_XC_GetFreeRunStatus)
        URELATION(fpXC_BYPASS_SetCSC,(IOCTL_XC_SET_BYPASS_CSC)MApi_XC_BYPASS_SetCSC_U2,MApi_XC_BYPASS_SetCSC)
        URELATION(fpXC_Get_DSForceIndexSupported,(IOCTL_XC_GET_DSFORCEINDEXSUPPORTED)MApi_XC_Get_DSForceIndexSupported_U2,MApi_XC_Get_DSForceIndexSupported)
        URELATION(fpXC_Set_DSForceIndex,(IOCTL_XC_SET_DSFORCEINDEX)MApi_XC_Set_DSForceIndex_U2,MApi_XC_Set_DSForceIndex)
        URELATION(fpXC_Set_DSIndexSourceSelect,(IOCTL_XC_SET_DSINDEXSOURCESELECT)MApi_XC_Set_DSIndexSourceSelect_U2,MApi_XC_Set_DSIndexSourceSelect)
        URELATION(fpXC_GetDynamicScalingStatus,(IOCTL_XC_GET_DYNAMICSCALINGSTATUS)MApi_XC_GetDynamicScalingStatus_U2,MApi_XC_GetDynamicScalingStatus)
        URELATION(fpXC_OSDC_InitSetting,(IOCTL_XC_OSDC_INITSETTING)MApi_XC_OSDC_InitSetting_U2,MApi_XC_OSDC_InitSetting)
        URELATION(fpXC_OSDC_SetOutVfreqx10,(IOCTL_XC_OSDC_SET_OUTPUTVFREQX10)MApi_XC_OSDC_SetOutVfreqx10_U2,MApi_XC_OSDC_SetOutVfreqx10)
        URELATION(fpXC_OSDC_Control,(IOCTL_XC_OSDC_CONTROL)MApi_XC_OSDC_Control_U2,MApi_XC_OSDC_Control)
        URELATION(fpXC_OSDC_GetDstInfo,(IOCTL_XC_OSDC_GETDSTINFO)MApi_XC_OSDC_GetDstInfo_U2,MApi_XC_OSDC_GetDstInfo)
        URELATION(fpXC_Set_PowerState,(IOCTL_XC_SET_POWERSTATE)MApi_XC_SetPowerState_U2,MApi_XC_SetPowerState)
        URELATION(fpXC_BYPASS_SetOSDVsyncPos,(IOCTL_XC_SET_BYPASS_OSDVSYNC_POS)MApi_XC_BYPASS_SetOSDVsyncPos_U2,MApi_XC_BYPASS_SetOSDVsyncPos)
        URELATION(fpXC_BYPASS_SetInputSrc,(IOCTL_XC_SET_BYPASS_INPUTSRC)MApi_XC_BYPASS_SetInputSrc_U2,MApi_XC_BYPASS_SetInputSrc)
        URELATION(fpXC_SetSeamlessZapping,(IOCTL_XC_SET_SEAMELESS_ZAPPING)MApi_XC_SetSeamlessZapping_U2,MApi_XC_SetSeamlessZapping)
        URELATION(fpXC_GetSeamlessZappingStatus,(IOCTL_XC_GET_SEAMELESS_ZAPPING_STATUS)MApi_XC_GetSeamlessZappingStatus_U2,MApi_XC_GetSeamlessZappingStatus)
        URELATION(fpXC_Vtrack_SetPayloadData,(IOCTL_XC_SET_VTRACK_PAYLOADDATA)MApi_XC_Vtrack_SetPayloadData_U2,MApi_XC_Vtrack_SetPayloadData)
        URELATION(fpXC_Vtrack_SetUserDefindedSetting,(IOCTL_XC_SET_VTRACK_USERDEFINED_SETTING)MApi_XC_Vtrack_SetUserDefindedSetting_U2,MApi_XC_Vtrack_SetUserDefindedSetting)
        URELATION(fpXC_Vtrack_Enable,(IOCTL_XC_SET_VTRACK_ENABLE)MApi_XC_Vtrack_Enable_U2,MApi_XC_Vtrack_Enable)
        URELATION(fpXC_PreSetPQInfo,(IOCTL_XC_PRESET_PQINFO)MApi_XC_PreSetPQInfo_U2,MApi_XC_PreSetPQInfo)
        URELATION(fpXC_Is_OP1_TestPattern_Enabled,(IOCTL_XC_GET_OP1_TESTPATTERN_ENABLED)MApi_XC_Is_OP1_TestPattern_Enabled_U2,MApi_XC_Is_OP1_TestPattern_Enabled)
        URELATION(fpXC_Set_OP1_TestPattern,(IOCTL_XC_SET_OP1_TESTPATTERN)MApi_XC_Set_OP1_TestPattern_U2,MApi_XC_Set_OP1_TestPattern)
        URELATION(fpXC_Set_OP2_TestPattern,(IOCTL_XC_SET_OP2_TESTPATTERN)MApi_XC_Set_OP2_Pattern_U2,MApi_XC_Set_OP2_Pattern)
        URELATION(fpXC_Check_WhiteBalance_Pattern_Mode,(IOCTL_XC_CHECK_WHITEBALANCE_PATTERN_MODE)MApi_XC_CheckWhiteBalancePatternModeSupport_U2,MApi_XC_CheckWhiteBalancePatternModeSupport)
        URELATION(fpXC_Set_HLinearScaling,(IOCTL_XC_SET_HLINEARSCALING)MApi_XC_SetHLinearScaling_U2,MApi_XC_SetHLinearScaling)
        URELATION(fpXC_EnableT3D,(IOCTL_XC_ENABLE_T3D)MApi_XC_EnableT3D_U2,MApi_XC_EnableT3D)
        URELATION(fpXC_Set_FRC_InputTiming,(IOCTL_XC_SET_FRC_INPUTTIMING)MApi_XC_Set_FRC_InputTiming_U2,MApi_XC_Set_FRC_InputTiming)
        URELATION(fpXC_Get_FRC_InputTiming,(IOCTL_XC_GET_FRC_INPUTTIMING)MApi_XC_Get_FRC_InputTiming_U2,MApi_XC_Get_FRC_InputTiming)
        URELATION(fpXC_Get_VirtualBox_Info,(IOCTL_XC_GET_VBOX_INFO)MApi_XC_Get_VirtualBox_Info_U2,MApi_XC_Get_VirtualBox_Info)
        URELATION(fpXC_Set_OSD_DETECT,(IOCTL_XC_SET_OSD_DETECT)MApi_XC_Set_OSD_Detect_U2,MApi_XC_Set_OSD_Detect)
        URELATION(fpXC_Get_OSD_DETECT,(IOCTL_XC_GET_OSD_DETECT)MApi_XC_Get_OSD_Detect_U2,MApi_XC_Get_OSD_Detect)
        URELATION(fpXC_Enable_LockFreqOnly,(IOCTL_XC_ENABLE_LOCKFREQONLY)MApi_XC_Enable_LockFreqOnly_U2,MApi_XC_Enable_LockFreqOnly)
        URELATION(fpXC_Is2K2KToFrcMode,(IOCTL_XC_IS2K2KTOFRCMODE)MApi_XC_Is2K2KToFrcMode_U2,MApi_XC_Is2K2KToFrcMode)
        URELATION(fpXC_SetXCVOPdata,(IOCTL_XC_SET_XC_VOP_DATA)MApi_XC_Set_XC_VOP_U2,MApi_XC_Set_XC_VOP)
        URELATION(fpXC_GetSWDSIndex,(IOCTL_XC_GETSWDSINDEX)MApi_XC_GetSWDSIndex_U2,MApi_XC_GetSWDSIndex)
        URELATION(fpXC_Get_Is_SupportSWDS,(IOCTL_XC_GET_Is_SupportSWDS)MApi_XC_Is_SupportSWDS_U2,MApi_XC_Is_SupportSWDS)
        URELATION(fpXC_Get_FRCM_FrameNum,(IOCTL_XC_GET_FRCM_FRAMENUM)MApi_XC_Get_FRCM_FrameNum_U2,MApi_XC_Get_FRCMFrameNum)
        URELATION(fpXC_SetPixelShiftFeatures,(IOCTL_XC_SET_PIXELSHIFT_FEATURES)MApi_XC_SetPixelShiftFeatures_U2,MApi_XC_SetPixelShiftFeatures)
        URELATION(fpXC_SetForceWrite,(IOCTL_XC_SETFORCEWRITE)MApi_XC_SetForceWrite_U2,MApi_XC_SetForceWrite)
        URELATION(fpXC_GetForceWrite,(IOCTL_XC_GETFORCEWRITE)MApi_XC_GetForceWrite_U2,MApi_XC_GetForceWrite)
#ifdef UFO_XC_PQ_PATH
        URELATION(fpXC_GetPQPathStatus,(IOCTL_XC_CMD_GET_PQ_PATH_STATUS)MApi_XC_GetPQPathStatus_U2,MApi_XC_GetPQPathStatus)
#endif
        URELATION(fpXC_AutoDownloadConfig,(IOCTL_XC_AUTODOWNLOAD_CONFIG_CTRL)MApi_XC_AutoDownload_Config_U2,MApi_XC_AutoDownload_Config)
        URELATION(fpXC_AutoDownloadWrite,(IOCTL_XC_AUTODOWNLOAD_WRITE_CTRL)MApi_XC_AutoDownload_Write_U2,MApi_XC_AutoDownload_Write)
        URELATION(fpXC_AutoDownloadFire,(IOCTL_XC_AUTODOWNLOAD_FIRE_CTRL)MApi_XC_AutoDownload_Fire_U2,MApi_XC_AutoDownload_Fire)
        URELATION(fpXC_HDRControl,(IOCTL_XC_HDR_CTRL)MApi_XC_HDR_Control_U2,MApi_XC_HDR_Control)
        URELATION(fpXC_OPTEE_Ctrl,(IOCTL_XC_OPTEE_CTRL)MApi_XC_OPTEE_Control_U2,MApi_XC_OPTEE_Control)
        URELATION(fpXC_OPTEE_Mux,(IOCTL_XC_OPTEE_MUX)MApi_XC_OPTEE_Mux_U2,MApi_XC_OPTEE_Mux)
        URELATION(fpXC_GetResourceByPipeID,(IOCTL_XC_GET_RES_BY_PIPE)MApi_XC_GetResourceByPipeID_U2,MApi_XC_GetResourceByPipeID)
        URELATION(fpXC_ConfigPipe,(IOCTL_XC_CONFIG_PIPE)MApi_XC_ConfigPipe_U2,MApi_XC_ConfigPipe)
        URELATION(fpXC_CheckPipe,(IOCTL_XC_CHECK_PIPE)MApi_XC_CheckPipe_U2,MApi_XC_CheckPipe)
        URELATION(fpXC_SetDSInfo,(IOCTL_XC_SETDSINFO)MApi_XC_SetDSInfo_U2,MApi_XC_SetDSInfo)
        URELATION(fpsc_get_output_vfreq,(IOCTL_SC_GET_OUTPUT_VFREQ)mvideo_sc_get_output_vfreq_U2,mvideo_sc_get_output_vfreq)
        URELATION(fpXC_Get_Current_OutputVFreqX100,(IOCTL_XC_GET_CURRENT_OUTPUTVFREQX100)MApi_XC_Get_Current_OutputVFreqX100_U2,MApi_XC_Get_Current_OutputVFreqX100)
        URELATION(fpXC_SetMemoryWriteRequest,(IOCTL_XC_SETMEMORYWRITEREQUEST)MApi_XC_SetMemoryWriteRequest_U2,MApi_XC_SetMemoryWriteRequest)
        URELATION(fpXC_Set_MemFmt,(IOCTL_XC_SET_MEMFMT)MApi_XC_Set_MemFmt_U2,MApi_XC_Set_MemFmt)
        URELATION(fpXC_SetOutputAdjustSetting,(IOCTL_XC_SETOUTPUTADJUSTSETTING)MApi_XC_SetOutputAdjustSetting_U2,MApi_XC_SetOutputAdjustSetting)
        URELATION(fsScaler_FPLL_FSM,(IOCTL_SCALER_FPLL_FSM)msAPI_Scaler_FPLL_FSM_U2,msAPI_Scaler_FPLL_FSM)
        URELATION(fpXC_EnableMirrorMode,(IOCTL_XC_ENABLEMIRRORMODE)MApi_XC_EnableMirrorMode_U2,MApi_XC_EnableMirrorMode)
        URELATION(fpXC_EnableMirrorMode2,(IOCTL_XC_ENABLEMIRRORMODE2)MApi_XC_EnableMirrorMode2_U2,MApi_XC_EnableMirrorMode2)
#ifdef SUPPORT_BWD
        URELATION(fpXC_SetBwdConfig,(IOCTL_XC_SET_BWD_CONFIG)MApi_XC_SetBwdConfig_U2,MApi_XC_SetBwdConfig)
#endif
        URELATION(fpXC_dump_shm,(IOCTL_DUMP_SHM)MApi_XC_Dump_SHM_U2,MApi_XC_Dump_SHM)
        URELATION(fpXC_IsSupport2StepScaling,(IOCTL_XC_CMD_IS_SUPPORT_2_STEP_SCALING)MApi_XC_IsSupport2StepScaling_U2,MApi_XC_IsSupport2StepScaling)
        URELATION(fpXC_VideoPlaybackCtrl,(IOCTL_XC_VIDEO_PLAYBACK_CTRL)MApi_XC_VideoPlaybackCtrl_U2,MApi_XC_VideoPlaybackCtrl)
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
        URELATION(fpXC_GenerateBlackVideoByMode,(IOCTL_XC_GENERATEBLACKVIDEOBYMODE)MApi_XC_GenerateBlackVideoByMode_U2,MApi_XC_GenerateBlackVideoByMode)
#endif
        URELATION(fpXC_swds_fire,(IOCTL_SWDS_Fire)MApi_SWDS_Fire_U2,MApi_SWDS_Fire)
        URELATION(fpXC_ADC_SetCVBSOut,(IOCTL_XC_ADC_SETCVBSOUT)MApi_XC_ADC_SetCVBSOut_U2,MApi_XC_ADC_SetCVBSOut)
        URELATION(fpXC_ADC_IsCVBSOutEnabled,(IOCTL_XC_ADC_ISCVBSOUTENABLED)MApi_XC_ADC_IsCVBSOutEnabled_U2,MApi_XC_ADC_IsCVBSOutEnabled)
        URELATION(fpXC_ADC_SetPcClock,(IOCTL_XC_ADC_SETPCCLOCK)MApi_XC_ADC_SetPcClock_U2,MApi_XC_ADC_SetPcClock)
        URELATION(fpXC_ADC_SetPhase,(IOCTL_XC_ADC_SETPHASE)MApi_XC_ADC_SetPhase_U2,MApi_XC_ADC_SetPhase)
        URELATION(fpXC_ADC_SetPhaseEx,(IOCTL_XC_ADC_SETPHASEEX)MApi_XC_ADC_SetPhaseEx_U2,MApi_XC_ADC_SetPhaseEx)
        URELATION(fpXC_ADC_GetPhaseRange,(IOCTL_XC_ADC_GETPHASERANGE)MApi_XC_ADC_GetPhaseRange_U2,MApi_XC_ADC_GetPhaseRange)
        URELATION(fpXC_ADC_GetPhase,(IOCTL_XC_ADC_GETPHASE)MApi_XC_ADC_GetPhase_U2,MApi_XC_ADC_GetPhase)
        URELATION(fpXC_ADC_GetPhaseEx,(IOCTL_XC_ADC_GETPHASEEX)MApi_XC_ADC_GetPhaseEx_U2,MApi_XC_ADC_GetPhaseEx)
        URELATION(fpXC_ADC_IsScartRGB,(IOCTL_XC_ADC_ISSCARTRGB)MApi_XC_ADC_IsScartRGB_U2,MApi_XC_ADC_IsScartRGB)
        URELATION(fpXC_ADC_GetPcClock,(IOCTL_XC_ADC_GETPCCLOCK)MApi_XC_ADC_GetPcClock_U2,MApi_XC_ADC_GetPcClock)
        URELATION(fpXC_ADC_GetSoGLevelRange,(IOCTL_XC_ADC_GETSOGLEVELRANGE)MApi_XC_ADC_GetSoGLevelRange_U2,MApi_XC_ADC_GetSoGLevelRange)
        URELATION(fpXC_ADC_SetSoGLevel,(IOCTL_XC_ADC_SETSOGLEVEL)MApi_XC_ADC_SetSoGLevel_U2,MApi_XC_ADC_SetSoGLevel)
        URELATION(fpXC_ADC_PowerOff,(IOCTL_XC_ADC_POWEROFF)MApi_XC_ADC_PowerOff_U2,MApi_XC_ADC_PowerOff)
        URELATION(fpXC_ADC_GetDefaultGainOffset,(IOCTL_XC_ADC_GETDEFAULTGAINOFFSET)MApi_XC_ADC_GetDefaultGainOffset_U2,MApi_XC_ADC_GetDefaultGainOffset)
        URELATION(fpXC_ADC_GetMaximalOffsetValue,(IOCTL_XC_ADC_GETMAXIMALOFFSETVALUE)MApi_XC_ADC_GetMaximalOffsetValue_U2,MApi_XC_ADC_GetMaximalOffsetValue)
        URELATION(fpXC_ADC_GetMaximalGainValue,(IOCTL_XC_ADC_GETMAXIMALGAINVALUE)MApi_XC_ADC_GetMaximalGainValue_U2,MApi_XC_ADC_GetMaximalGainValue)
        URELATION(fpXC_ADC_GetCenterGain,(IOCTL_XC_ADC_GETCENTERGAIN)MApi_XC_ADC_GetCenterGain_U2,MApi_XC_ADC_GetCenterGain)
        URELATION(fpXC_ADC_GetCenterOffset,(IOCTL_XC_ADC_GETCENTEROFFSET)MApi_XC_ADC_GetCenterOffset_U2,MApi_XC_ADC_GetCenterOffset)
        URELATION(fpXC_ADC_SetGain,(IOCTL_XC_ADC_SETGAIN)MApi_XC_ADC_SetGain_U2,MApi_XC_ADC_SetGain)
        URELATION(fpXC_ADC_SetOffset,(IOCTL_XC_ADC_SETOFFSET)MApi_XC_ADC_SetOffset_U2,MApi_XC_ADC_SetOffset)
        URELATION(fpXC_ADC_AdjustGainOffset,(IOCTL_XC_ADC_ADJUSTGAINOFFSET)MApi_XC_ADC_AdjustGainOffset_U2,MApi_XC_ADC_AdjustGainOffset)
        URELATION(fpXC_ADC_Source_Calibrate,(IOCTL_XC_ADC_SOURCE_CALIBRATE)MApi_XC_ADC_Source_Calibrate_U2,MApi_XC_ADC_Source_Calibrate)
        URELATION(fpXC_ADC_SetSoGCal,(IOCTL_XC_ADC_SETSOGCAL)MApi_XC_ADC_SetSoGCal_U2,MApi_XC_ADC_SetSoGCal)
        URELATION(fpXC_ADC_SetRGB_PIPE_Delay,(IOCTL_XC_ADC_SETRGB_PIPE_DELAY)MApi_XC_ADC_SetRGB_PIPE_Delay_U2,MApi_XC_ADC_SetRGB_PIPE_Delay)
        URELATION(fpXC_ADC_ScartRGB_SOG_ClampDelay,(IOCTL_XC_ADC_SCARTRGB_SOG_CLAMPDELAY)MApi_XC_ADC_ScartRGB_SOG_ClampDelay_U2,MApi_XC_ADC_ScartRGB_SOG_ClampDelay)
        URELATION(fpXC_ADC_Set_YPbPrLooseLPF,(IOCTL_XC_ADC_SET_YPBPRLOOSELPF)MApi_XC_ADC_Set_YPbPrLooseLPF_U2,MApi_XC_ADC_Set_YPbPrLooseLPF)
        URELATION(fpXC_ADC_Set_SOGBW,(IOCTL_XC_ADC_SET_SOGBW)MApi_XC_ADC_Set_SOGBW_U2,MApi_XC_ADC_Set_SOGBW)
        URELATION(fpXC_ADC_SetClampDuration,(IOCTL_XC_ADC_SETCLAMPDURATION)MApi_XC_ADC_SetClampDuration_U2,MApi_XC_ADC_SetClampDuration)
        URELATION(fpXC_ADC_EnableHWCalibration,(IOCTL_XC_ADC_ENABLEHWCALIBRATION)MApi_XC_ADC_EnableHWCalibration_U2,MApi_XC_ADC_EnableHWCalibration)
        URELATION(fpXC_ADC_SetIdacCurrentMode,(IOCTL_XC_ADC_SETIDACCURRENTMODE)MApi_XC_ADC_SetIdacCurrentMode_U2,MApi_XC_ADC_SetIdacCurrentMode)
        URELATION(fpXC_ADC_GetIdacCurrentMode,(IOCTL_XC_ADC_GETIDACCURRENTMODE)MApi_XC_ADC_GetIdacCurrentMode_U2,MApi_XC_ADC_GetIdacCurrentMode)
        URELATION(fpXC_Auto_Geometry,(IOCTL_XC_AUTO_GEOMETRY) MApi_XC_Auto_Geometry_U2, MApi_XC_Auto_Geometry)
        URELATION(fpXC_Auto_Geometry_Ex,(IOCTL_XC_AUTO_GEOMETRY_EX) MApi_XC_Auto_Geometry_Ex_U2,  MApi_XC_Auto_Geometry_Ex)
        URELATION(fpXC_Auto_StopAutoGeometry,(IOCTL_XC_AUTO_STOPAUTOGEOMETRY) MApi_XC_Auto_StopAutoGeometry_U2, MApi_XC_Auto_StopAutoGeometry)
        URELATION(fpXC_Auto_GainOffset,(IOCTL_XC_AUTO_GAINOFFSET) MApi_XC_Auto_GainOffset_U2, MApi_XC_Auto_GainOffset)
        URELATION(fpXC_Auto_GetHWFixedGainOffset,(IOCTL_XC_AUTO_GETHWFIXEDGAINOFFSET) MApi_XC_Auto_GetHWFixedGainOffset_U2, MApi_XC_Auto_GetHWFixedGainOffset)
        URELATION(fpXC_Auto_SetValidData,(IOCTL_XC_AUTO_SETVALIDDATA) MApi_XC_Auto_SetValidData_U2, MApi_XC_Auto_SetValidData)
        URELATION(fpXC_Auto_AutoOffset,(IOCTL_XC_AUTO_AUTOOFFSET) MApi_XC_Auto_AutoOffset_U2, MApi_XC_Auto_AutoOffset)
        URELATION(fpXC_Auto_DetectWidth,(IOCTL_XC_AUTO_DETECTWIDTH) MApi_XC_Auto_DetectWidth_U2,  MApi_XC_Auto_DetectWidth)
        URELATION(fpXC_Auto_SetCalibrationMode,(IOCTL_XC_AUTO_SETCALIBRATIONMODE) MApi_XC_Auto_SetCalibrationMode_U2, MApi_XC_Auto_SetCalibrationMode)
        URELATION(fpXC_Auto_GetCalibrationMode,(IOCTL_XC_AUTO_GETCALIBRATIONMODE) MApi_XC_Auto_GetCalibrationMode_U2, MApi_XC_Auto_GetCalibrationMode)
        URELATION(fpXC_Auto_GetSyncInfo,(IOCTL_XC_AUTO_GETSYNCINFO) MApi_XC_AUTO_GetSyncInfo_U2,  MApi_XC_AUTO_GetSyncInfo)
        URELATION(fpXC_ModeParse_Init,(IOCTL_XC_MODEPARSE_INIT)MApi_XC_ModeParse_Init_U2,MApi_XC_ModeParse_Init)
        URELATION(fpXC_ModeParse_MatchMode,(IOCTL_XC_MODEPARSE_MATCHMODE)MApi_XC_ModeParse_MatchMode_U2,MApi_XC_ModeParse_MatchMode)
        URELATION(fpXC_ModeParse_MatchModeEx,(IOCTL_XC_MODEPARSE_MATCHMODEEX)MApi_XC_ModeParse_MatchModeEx_U2,MApi_XC_ModeParse_MatchModeEx)
        URELATION(fpXC_PCMonitor_Init,(IOCTL_XC_PCMONITOR_INIT) MApi_XC_PCMonitor_Init_U2,  MApi_XC_PCMonitor_Init)
        URELATION(fpXC_PCMonitor_Restart,(IOCTL_XC_PCMONITOR_RESTART) MApi_XC_PCMonitor_Restart_U2, MApi_XC_PCMonitor_Restart)
        URELATION(fpXC_PCMonitor_SetTimingCount,(IOCTL_XC_PCMONITOR_SETTIMINGCOUNT) MApi_XC_PCMonitor_SetTimingCount_U2,  MApi_XC_PCMonitor_SetTimingCount)
        URELATION(fpXC_PCMonitor,(IOCTL_XC_PCMONITOR) MApi_XC_PCMonitor_U2, MApi_XC_PCMonitor)
        URELATION(fpXC_PCMonitor_GetCurrentState,(IOCTL_XC_PCMONITOR_GETCURRENTSTATE) MApi_XC_PCMonitor_GetCurrentState_U2, MApi_XC_PCMonitor_GetCurrentState)
        URELATION(fpXC_PCMonitor_GetSyncStatus,(IOCTL_XC_PCMONITOR_GETSYNCSTATUS) MApi_XC_PCMonitor_GetSyncStatus_U2, MApi_XC_PCMonitor_GetSyncStatus)
        URELATION(fpXC_PCMonitor_Get_HFreqx10,(IOCTL_XC_PCMONITOR_GET_HFREQX10) MApi_XC_PCMonitor_Get_HFreqx10_U2,  MApi_XC_PCMonitor_Get_HFreqx10)
        URELATION(fpXC_PCMonitor_Get_HFreqx1K,(IOCTL_XC_PCMONITOR_GET_HFREQX1K) MApi_XC_PCMonitor_Get_HFreqx1K_U2,  MApi_XC_PCMonitor_Get_HFreqx1K)
        URELATION(fpXC_PCMonitor_Get_VFreqx10,(IOCTL_XC_PCMONITOR_GET_VFREQX10) MApi_XC_PCMonitor_Get_VFreqx10_U2,  MApi_XC_PCMonitor_Get_VFreqx10)
        URELATION(fpXC_PCMonitor_Get_VFreqx1K,(IOCTL_XC_PCMONITOR_GET_VFREQX1K) MApi_XC_PCMonitor_Get_VFreqx1K_U2,  MApi_XC_PCMonitor_Get_VFreqx1K)
        URELATION(fpXC_PCMonitor_Get_Vtotal,(IOCTL_XC_PCMONITOR_GET_VTOTAL) MApi_XC_PCMonitor_Get_Vtotal_U2,  MApi_XC_PCMonitor_Get_Vtotal)
        URELATION(fpXC_PCMonitor_Get_Dvi_Hdmi_De_Info,(IOCTL_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO) MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info_U2,  MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info)
        URELATION(fpXC_PCMonitor_SyncLoss,(IOCTL_XC_PCMONITOR_SYNCLOSS) MApi_XC_PCMonitor_SyncLoss_U2,  MApi_XC_PCMonitor_SyncLoss)
        URELATION(fpXC_PCMonitor_InvalidTimingDetect,(IOCTL_XC_PCMONITOR_INVALIDTIMINGDETECT) MApi_XC_PCMonitor_InvalidTimingDetect_U2, MApi_XC_PCMonitor_InvalidTimingDetect)
        URELATION(fpXC_PCMonitor_SetTimingCountEx,(IOCTL_XC_PCMONITOR_SETTIMINGCOUNTEX) MApi_XC_PCMonitor_SetTimingCountEx_U2,  MApi_XC_PCMonitor_SetTimingCountEx)
        URELATION(fpHDMI_Func_Caps,(IOCTL_HDMI_FUNC_CAPS)MDrv_HDMI_Func_Caps_U2,MDrv_HDMI_Func_Caps)
        URELATION(fpHDMI_init,(IOCTL_HDMI_INIT)MDrv_HDMI_init_U2,MDrv_HDMI_init)
        URELATION(fpHDMI_Exit,(IOCTL_HDMI_EXIT)MDrv_HDMI_Exit_U2,MDrv_HDMI_Exit)
        URELATION(fpHDMI_pkt_reset,(IOCTL_HDMI_PKT_RESET)MDrv_HDMI_pkt_reset_U2,MDrv_HDMI_pkt_reset)
        URELATION(fpHDMI_pullhpd,(IOCTL_HDMI_PULLHPD)MDrv_HDMI_pullhpd_U2,MDrv_HDMI_pullhpd)
        URELATION(fpHDMI_GC_Info,(IOCTL_HDMI_GC_INFO)MDrv_HDMI_GC_Info_U2,MDrv_HDMI_GC_Info)
        URELATION(fpHDMI_Packet_Received,(IOCTL_HDMI_PACKET_RECEIVED)MDrv_HDMI_Packet_Received_U2,MDrv_HDMI_Packet_Received)
        URELATION(fpHDMI_Get_ColorFormat,(IOCTL_HDMI_GET_COLORFORMAT)MDrv_HDMI_Get_ColorFormat_U2,MDrv_HDMI_Get_ColorFormat)
        URELATION(fpHDMI_Get_ColorRange,(IOCTL_HDMI_GET_COLORRANGE)MDrv_HDMI_Get_ColorRange_U2,MDrv_HDMI_Get_ColorRange)
        URELATION(fpHDMI_Set_EQ,(IOCTL_HDMI_SET_EQ)MDrv_HDMI_Set_EQ_U2,MDrv_HDMI_Set_EQ)
        URELATION(fpHDMI_Set_EQ_To_Port,(IOCTL_HDMI_SET_EQ_TO_PORT)MDrv_HDMI_Set_EQ_To_Port_U2,MDrv_HDMI_Set_EQ_To_Port)
        URELATION(fpHDMI_Audio_MUTE_Enable,(IOCTL_HDMI_AUDIO_MUTE_ENABLE)MDrv_HDMI_Audio_MUTE_Enable_U2,MDrv_HDMI_Audio_MUTE_Enable)
        URELATION(fpHDMI_Audio_Status_Clear,(IOCTL_HDMI_AUDIO_STATUS_CLEAR)MDrv_HDMI_Audio_Status_Clear_U2,MDrv_HDMI_Audio_Status_Clear)
        URELATION(fpHDMI_Get_AspectRatio,(IOCTL_HDMI_GET_ASPECTRATIO)MDrv_HDMI_Get_AspectRatio_U2,MDrv_HDMI_Get_AspectRatio)
        URELATION(fpHDMI_Get_ActiveFormat_AspectRatio,(IOCTL_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO)MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2,MDrv_HDMI_Get_ActiveFormat_AspectRatio)
        URELATION(fpHDMI_Get_AVIInfoFrameVer,(IOCTL_HDMI_GET_AVIINFOFRAMEVER)MDrv_HDMI_Get_AVIInfoFrameVer_U2,MDrv_HDMI_Get_AVIInfoFrameVer)
        URELATION(fpHDMI_err_status_update,(IOCTL_HDMI_ERR_STATUS_UPDATE)MDrv_HDMI_err_status_update_U2,MDrv_HDMI_err_status_update)
        URELATION(fpHDMI_Get_TMDS_freq,(IOCTL_HDMI_GET_TMDS_FREQ)MDrv_HDMI_GetTMDSFreq_U2,MDrv_HDMI_GetTMDSFreq)
        URELATION(fpHDMI_Get_PollingStatus,(IOCTL_HDMI_GET_POLLINGSTATUS)MDrv_HDMI_Get_PollingStatus_U2,MDrv_HDMI_Get_PollingStatus)
        URELATION(fpHDMI_IsHDMI_Mode,(IOCTL_HDMI_ISHDMI_MODE)MDrv_HDMI_IsHDMI_Mode_U2,MDrv_HDMI_IsHDMI_Mode)
        URELATION(fpHDMI_Get_MID_info,(IOCTL_HDMI_GET_MID_INFO)MDrv_HDMI_Get_MID_info_U2,MDrv_HDMI_Get_MID_info)
        URELATION(fpHDMI_get_packet_value,(IOCTL_HDMI_GET_PACKET_VALUE)MDrv_HDMI_get_packet_value_U2,MDrv_HDMI_get_packet_value)
        URELATION(fpDVI_ChannelPhaseStatus,(IOCTL_DVI_CHANNELPHASESTATUS)MDrv_DVI_ChannelPhaseStatus_U2,MDrv_DVI_ChannelPhaseStatus)
        URELATION(fpDVI_SoftwareReset,(IOCTL_DVI_SOFTWARERESET)MDrv_DVI_SoftwareReset_U2,MDrv_DVI_SoftwareReset)
        URELATION(fpdvi_reset,(IOCTL_DVI_RESET)mdrv_dvi_reset_U2,mdrv_dvi_reset)
        URELATION(fpHDMI_Get_AVIInfoActiveInfoPresent,(IOCTL_HDMI_AVIINFO_ACTIVEINFOPRESENT)MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2,MDrv_HDMI_Get_AVIInfoActiveInfoPresent)
        URELATION(fpDVI_ClkPullLow,(IOCTL_DVI_CLKPULLLOW)MDrv_DVI_ClkPullLow_U2,MDrv_DVI_ClkPullLow)
        URELATION(fpDVI_SwitchSrc,(IOCTL_DVI_SWITCHSRC)MDrv_DVI_SwitchSrc_U2,MDrv_DVI_SwitchSrc)
        URELATION(fpHDMI_SetForClockLessThan70Mhz,(IOCTL_HDMI_SETFORCLOCKLESSTHAN70MHZ)MDrv_HDMI_SetForClockLessThan70Mhz_U2,MDrv_HDMI_SetForClockLessThan70Mhz)
        URELATION(fpHDMI_dvi_adjust,(IOCTL_HDMI_DVI_ADJUST)MDrv_HDMI_dvi_adjust_U2,MDrv_HDMI_dvi_adjust)
        URELATION(fpHDMI_SetUpdatePhaseLineCount,(IOCTL_HDMI_SETUPDATEPHASELINECOUNT)MDrv_HDMI_SetUpdatePhaseLineCount_U2,MDrv_HDMI_SetUpdatePhaseLineCount)
        URELATION(fpHDCP_Enable,(IOCTL_HDCP_ENABLE)MDrv_HDCP_Enable_U2,MDrv_HDCP_Enable)
        URELATION(fpHDMI_SetHdcpEnable,(IOCTL_HDMI_SETHDCPENABLE)MDrv_HDMI_SetHdcpEnable_U2,MDrv_HDMI_SetHdcpEnable)
        URELATION(fpHDCP_ClearStatus,(IOCTL_HDCP_CLEARSTATUS)MDrv_HDCP_ClearStatus_U2,MDrv_HDCP_ClearStatus)
        URELATION(fpHDCP_initproductionkey,(IOCTL_HDCP_INITPRODUCTIONKEY)MDrv_HDCP_initproductionkey_U2,MDrv_HDCP_initproductionkey)
        URELATION(fpHDCP_GetStatus,(IOCTL_HDCP_GETSTATUS)MDrv_HDCP_GetStatus_U2,MDrv_HDCP_GetStatus)
        URELATION(fpHDCP_Vsync_end_en,(IOCTL_HDCP_VSYNC_END_EN)MDrv_HDCP_Vsync_end_en_U2,MDrv_HDCP_Vsync_end_en)
        URELATION(fpHDMI_audio_output,(IOCTL_HDMI_AUDIO_OUTPUT)MDrv_HDMI_audio_output_U2,MDrv_HDMI_audio_output)
        URELATION(fpHDMI_audio_cp_hdr_info,(IOCTL_HDMI_AUDIO_CP_HDR_INFO)MDrv_HDMI_audio_cp_hdr_info_U2,MDrv_HDMI_audio_cp_hdr_info)
        URELATION(fpHDMI_audio_channel_status,(IOCTL_HDMI_AUDIO_CHANNEL_STATUS)MDrv_HDMI_audio_channel_status_U2,MDrv_HDMI_audio_channel_status)
        URELATION(fpHDMI_GetLibVer,(IOCTL_HDMI_GETLIBVER)MDrv_HDMI_GetLibVer_U2,MDrv_HDMI_GetLibVer)
        URELATION(fpHDMI_GetInfo,(IOCTL_HDMI_GETINFO)MDrv_HDMI_GetInfo_U2,MDrv_HDMI_GetInfo)
        URELATION(fpHDMI_GetStatus,(IOCTL_HDMI_GETSTATUS)MDrv_HDMI_GetStatus_U2,MDrv_HDMI_GetStatus)
        URELATION(fpHDMI_READ_DDCRAM,(IOCTL_HDMI_READ_DDCRAM)MDrv_HDMI_READ_DDCRAM_U2,MDrv_HDMI_READ_DDCRAM)
        URELATION(fpHDMI_PROG_DDCRAM,(IOCTL_HDMI_PROG_DDCRAM)MDrv_HDMI_PROG_DDCRAM_U2,MDrv_HDMI_PROG_DDCRAM)
        URELATION(fpHDMI_Get_Ext_Colorimetry,(IOCTL_HDMI_GET_EXT_COLORIMETRY)MDrv_HDMI_Get_ExtColorimetry_U2,MDrv_HDMI_Get_ExtColorimetry)
        URELATION(fpHDMI_Get_Content_Type,(IOCTL_HDMI_GET_CONTENT_TYPE)MDrv_HDMI_Get_Content_Type_U2,MDrv_HDMI_Get_Content_Type)
        URELATION(fpHDMI_Get_Pixel_Repetition,(IOCTL_HDMI_GET_PIXEL_REPETITION)MDrv_HDMI_Get_Pixel_Repetition_U2,MDrv_HDMI_Get_Pixel_Repetition)
        URELATION(fpHDMI_Check4K2K,(IOCTL_HDMI_CHECK_4K2K)MDrv_HDMI_Check4K2K_U2,MDrv_HDMI_Check4K2K)
        URELATION(fpHDMI_3D_4Kx2K_Process,(IOCTL_HDMI_3D_4KX2K_PROCESS)MDrv_HDMI_3D_4Kx2K_Process_U2,MDrv_HDMI_3D_4Kx2K_Process)
        URELATION(fpHDMI_AVG_ScaleringDown,(IOCTL_HDMI_AVG_SCALERINGDOWN)MDrv_HDMI_AVG_ScaleringDown_U2,MDrv_HDMI_AVG_ScaleringDown)
        URELATION(fpHDMI_Check_Additional_Format,(IOCTL_HDMI_CHECK_ADDITIONAL_FORMAT)MDrv_HDMI_Check_Additional_Format_U2,MDrv_HDMI_Check_Additional_Format)
        URELATION(fpHDMI_Get_3D_Structure,(IOCTL_HDMI_GET_3D_STRUCTURE)MDrv_HDMI_Get_3D_Structure_U2,MDrv_HDMI_Get_3D_Structure)
        URELATION(fpHDMI_Get_3D_Ext_Data,(IOCTL_HDMI_GET_3D_EXT_DATA)MDrv_HDMI_Get_3D_Ext_Data_U2,MDrv_HDMI_Get_3D_Ext_Data)
        URELATION(fpHDMI_Get_3D_Meta_Field,(IOCTL_HDMI_GET_3D_META_FIELD)MDrv_HDMI_Get_3D_Meta_Field_U2,MDrv_HDMI_Get_3D_Meta_Field)
        URELATION(fpHDMI_Get_VIC_Code,(IOCTL_HDMI_GET_VIC_CODE)MDrv_HDMI_Get_VIC_Code_U2,MDrv_HDMI_Get_VIC_Code)
        URELATION(fpHDMI_Get_4Kx2K_VIC_Code,(IOCTL_HDMI_GET_4KX2K_VIC_CODE)MDrv_HDMI_Get_4Kx2K_VIC_Code_U2,MDrv_HDMI_Get_4Kx2K_VIC_Code)
        URELATION(fpHDMI_ARC_PINControl,(IOCTL_HDMI_ARC_PINCONTROL)MDrv_HDMI_ARC_PINControl_U2,MDrv_HDMI_ARC_PINControl)
        URELATION(fpDVI_Software_Reset,(IOCTL_DVI_SOFTWARE_RESET)MDrv_DVI_Software_Reset_U2,MDrv_DVI_Software_Reset)
        URELATION(fpHDMI_Set_PowerState,(IOCTL_HDMI_SET_POWERSTATE)MDrv_HDMI_SetPowerState_U2,MDrv_HDMI_SetPowerState)
        URELATION(fpHDMI_dvi_hf_adjust,(IOCTL_HDMI_DVI_HF_ADJUST)MDrv_DVI_HF_adjust_U2,MDrv_DVI_HF_adjust)
        URELATION(fpHDMI_checkhdmi20_setting,(IOCTL_HDMI_CHECKHDMI20_SETTING)MDrv_HDMI_CheckHDMI20_Setting_U2,MDrv_HDMI_CheckHDMI20_Setting)
        URELATION(fpHDMI_Set5VDetectGPIOSelect,(IOCTL_HDMI_SET_5V_DETECT_GPIO_SELECT)MDrv_HDMI_Set5VDetectGPIOSelect_U2,MDrv_HDMI_Set5VDetectGPIOSelect)
        URELATION(fpHDMI_Ctrl,(IOCTL_HDMI_CTRL)MDrv_HDMI_Ctrl_U2,MDrv_HDMI_Ctrl)
        URELATION(fpHDMI_GetDataInfo,(IOCTL_HDMI_GET_DATA_INFO)MDrv_HDMI_GetDataInfo_U2,MDrv_HDMI_GetDataInfo)
#ifdef UFO_XC_VMARK
        URELATION(fpXC_VMark_SetParameters,(IOCTL_XC_VMARK_SET_PARAMETERS)MApi_XC_VMark_SetParameters_U2,MApi_XC_VMark_SetParameters)
#endif
        URELATION(fpXC_Set_SWDR_Info,(IOCTL_XC_SET_SWDR_INFO)MApi_XC_SetSWDRInfo_U2,MApi_XC_SetSWDRInfo)
        URELATION(fpXC_Get_SWDR_Info,(IOCTL_XC_GET_SWDR_INFO)MApi_XC_GetSWDRInfo_U2,MApi_XC_GetSWDRInfo)
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
        URELATION(fpXC_GetHDMIPolicy,(IOCTL_XC_GET_HDMI_POLICY)MApi_XC_GetHDMIPolicy_U2,MApi_XC_GetHDMIPolicy)
#endif
    #undef URELEATION
}

MS_U32 XCOpenFull(void** ppInstance, const void* const pAttribute)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    XC_INSTANT_ATTRIBUTE *pstXCInstAttr = (XC_INSTANT_ATTRIBUTE*)pAttribute;

    if (UtopiaInstanceCreate(sizeof(XC_INSTANCE_PRIVATE), ppInstance) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Create Instance Failed\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_FAIL;
    }

    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psXCInstPri);
    psXCInstPri->u32DeviceID = 0;
    if(pAttribute != NULL)
    {
        psXCInstPri->u32DeviceID = pstXCInstAttr->u32DeviceID;
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if((pstXCInstAttr == NULL) // Default: eXCType = E_XC_INSTANT_TYPE_XC
     ||(pstXCInstAttr->eXCType == E_XC_INSTANT_TYPE_XC)
    )
    {
        if (psXCInstPri->u32DeviceID == 1)
        {
            g_pDevice1Instance = *ppInstance;
        }
        else
        {
            g_pDevice0Instance = *ppInstance;
        }
    }
#endif

#ifdef CONFIG_UTOPIA_SIZE_CUSTOMER
    XC_U1_U2_RELATION(ppInstance,pAttribute);
#else
    // init Instance private members here
    XC_Initialize(psXCInstPri, pAttribute);
    ADC_Initialize(psXCInstPri, pAttribute);
    AUTO_Initialize(psXCInstPri, pAttribute);

    MODEPARSE_Initialize(psXCInstPri, pAttribute);
    PCMONITOR_Initialize(psXCInstPri, pAttribute);
    HDMI_IF_Initialize(psXCInstPri, pAttribute);
#endif

#ifdef UFO_XC_VMARK
    VMARK_Initialize(psXCInstPri, pAttribute);
#endif
    return UTOPIA_STATUS_SUCCESS;
}


// this is for those who has code size problem and needs the minimum set of functions.
MS_U32 XCOpenMinimum(void** ppInstance, const void* const pAttribute)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

    if (UtopiaInstanceCreate(sizeof(XC_INSTANCE_PRIVATE), ppInstance) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Create Instance Failed\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_FAIL;
    }

    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psXCInstPri);

    if(pAttribute != NULL)
    {
        MS_U32* u32Attr = (MS_U32*)pAttribute;
        psXCInstPri->u32DeviceID = *u32Attr;
    }
    else
    {
        psXCInstPri->u32DeviceID = 0;
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(psXCInstPri->u32DeviceID == 1)
    {
        g_pDevice1Instance = *ppInstance;
    }
    else
    {
        g_pDevice0Instance = *ppInstance;
    }
#endif
    psXCInstPri->fpXC_GetCapability = MApi_XC_GetCapability_U2;
    psXCInstPri->fpXC_PreInit =    MApi_XC_PreInit_U2;
    psXCInstPri->fpXC_Init_MISC = MApi_XC_Init_MISC_U2;
    psXCInstPri->fpXC_Init = MApi_XC_Init_U2;
    psXCInstPri->fpXC_SetExPanelInfo =    MApi_XC_SetExPanelInfo_U2;
    psXCInstPri->fpXC_FRC_Mute = MApi_XC_FRC_Mute_U2;
    psXCInstPri->fpXC_ADC_SetCVBSOut =  MApi_XC_ADC_SetCVBSOut_U2;
    psXCInstPri->fpXC_SetOutputCapture =    MApi_XC_SetOutputCapture_U2;
    psXCInstPri->fpXC_SetDispWindowColor =    MApi_XC_SetDispWindowColor_U2;
    psXCInstPri->fpXC_OSDC_InitSetting =    MApi_XC_OSDC_InitSetting_U2;
    psXCInstPri->fpXC_OSDC_SetOutVfreqx10 = MApi_XC_OSDC_SetOutVfreqx10_U2;
    psXCInstPri->fpXC_OSDC_Control =    MApi_XC_OSDC_Control_U2;
    psXCInstPri->fpXC_SetFrameColor =    MApi_XC_SetFrameColor_U2;
    psXCInstPri->fpXC_WriteByteMask =    MApi_XC_WriteByteMask_U2;
    psXCInstPri->fpXC_Write2ByteMask =    MApi_XC_Write2ByteMask_U2;
    psXCInstPri->fpXC_W2BYTE =    MApi_XC_W2BYTE_U2;
    psXCInstPri->fpXC_W4BYTE =    MApi_XC_W4BYTE_U2;
    psXCInstPri->fpXC_W2BYTEMSK =    MApi_XC_W2BYTEMSK_U2;
    psXCInstPri->fpXC_MLG_GetStatus =    MApi_XC_MLG_GetStatus_U2;
    psXCInstPri->fpXC_SkipSWReset =    MApi_XC_SkipSWReset_U2;
    psXCInstPri->fpXC_GenerateBlackVideo =    MApi_XC_GenerateBlackVideo_U2;
    psXCInstPri->fpXC_Mux_Init =    MApi_XC_Mux_Init_U2;
    psXCInstPri->fpXC_Mux_CreatePath =    MApi_XC_Mux_CreatePath_U2;
    psXCInstPri->fpXC_Mux_EnablePath =    MApi_XC_Mux_EnablePath_U2;
    psXCInstPri->fpXC_SetInputSource = MApi_XC_SetInputSource_U2;
    psXCInstPri->fpXC_DisableInputSource =    MApi_XC_DisableInputSource_U2;
    psXCInstPri->fpXC_SkipWaitVsync =    MApi_XC_SkipWaitVsync_U2;
    psXCInstPri->fpXC_SetWindow = MApi_XC_SetWindow_U2;
    psXCInstPri->fpXC_GetMISCStatus = MApi_XC_GetMISCStatus_U2;
    psXCInstPri->fpXC_SetOSD2VEMode =    MApi_XC_SetOSD2VEMode_U2;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 XCOpenForTEE(void** ppInstance, const void* const pAttribute)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

    if (UtopiaInstanceCreate(sizeof(XC_INSTANCE_PRIVATE), ppInstance) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Create Instance Failed\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_FAIL;
    }

    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psXCInstPri);

    if(pAttribute != NULL)
    {
        MS_U32* u32Attr = (MS_U32*)pAttribute;
        psXCInstPri->u32DeviceID = *u32Attr;
    }
    else
    {
        psXCInstPri->u32DeviceID = 0;
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(psXCInstPri->u32DeviceID == 1)
    {
        g_pDevice1Instance = *ppInstance;
    }
    else
    {
        g_pDevice0Instance = *ppInstance;
    }
#endif
    psXCInstPri->fpXC_Init = MApi_XC_Init_U2;
    psXCInstPri->fpXC_OPTEE_Ctrl = MApi_XC_OPTEE_Control_U2;
    psXCInstPri->fpXC_OPTEE_CheckRBase = MApi_XC_OPTEE_CheckRBase_U2;
    psXCInstPri->fpXC_OPTEE_Mux = MApi_XC_OPTEE_Mux_U2;
    psXCInstPri->fpXC_GetResourceByPipeID = MApi_XC_GetResourceByPipeID_U2;
    psXCInstPri->fpXC_ConfigPipe = MApi_XC_ConfigPipe_U2;
    psXCInstPri->fpXC_CheckPipe = MApi_XC_CheckPipe_U2;
    psXCInstPri->fpXC_SetDispWinToReg =    MApi_XC_SetDispWinToReg_U2;
    psXCInstPri->fpXC_GetDispWinFromReg = MApi_XC_GetDispWinFromReg_U2;
    psXCInstPri->fpXC_Vtrack_SetPayloadData = MApi_XC_Vtrack_SetPayloadData_U2;
    psXCInstPri->fpXC_Vtrack_SetUserDefindedSetting = MApi_XC_Vtrack_SetUserDefindedSetting_U2;
    psXCInstPri->fpXC_Vtrack_Enable = MApi_XC_Vtrack_Enable_U2;
    psXCInstPri->fpXC_GetStatus = MApi_XC_GetStatus_U2;
    psXCInstPri->fpXC_GenerateBlackVideo = MApi_XC_GenerateBlackVideo_U2;
#ifdef UFO_XC_VMARK
    VMARK_Initialize(psXCInstPri, pAttribute);
#endif
    psXCInstPri->fpXC_Set_PowerState = MApi_XC_SetPowerState_U2;

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 XCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    E_XC_IOCTL_CMDS eCmds = (E_XC_IOCTL_CMDS) u32Cmd;

    if( eCmds >= E_XC_CMD_NUM )
    {
        printf(" IOCTL command out of bounds, please check!\n");
        return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    // APIXC
    // E_XC_CMD_NONE = 0 so eCmds can not compare with E_XC_CMD_NONE
    // eCmds is unsigned value, can not compare with 0
    if (eCmds <= E_XC_CMD_MAX)
    {
        u32Return = _XCIoctl(pInstance, u32Cmd, pArgs);
    }
    // APIXC_ADC
    else if((eCmds >= E_XC_ADC_CMD_NONE)
            && (eCmds <= E_XC_ADC_CMD_MAX))
    {
        u32Return = _ADCIoctl(pInstance, u32Cmd, pArgs);
    }
    // APIXC_AUTO
    else if((eCmds >= E_XC_AUTO_CMD_NONE)
            && (eCmds <= E_XC_AUTO_CMD_MAX))
    {
        u32Return = _AUTOIoctl(pInstance, u32Cmd, pArgs);
    }

    // APIXC_MODEPARSE
    else if((eCmds >= E_XC_MODEPARSE_CMD_NONE)
            && (eCmds <= E_XC_MODEPARSE_CMD_MAX))
    {
        u32Return = _MODEPARSEIoctl(pInstance, u32Cmd, pArgs);
    }
    // APIXC_PCMONITOR
    else if((eCmds >= E_XC_PCMONITOR_CMD_NONE)
            && (eCmds <= E_XC_PCMONITOR_CMD_MAX))
    {
        u32Return = _PCMONITORIoctl(pInstance, u32Cmd, pArgs);
    }
    // DRVXC_HDMI_IF
    else if((eCmds >= E_XC_HDMI_IF_CMD_NONE)
            && (eCmds <= E_XC_HDMI_IF_CMD_MAX))
    {
        u32Return = _HDMI_IF_Ioctl(pInstance, u32Cmd, pArgs);
    }
#ifdef UFO_XC_VMARK
    else if((eCmds >= E_XC_VMARK_CMD_NONE)
            && (eCmds <= E_XC_VMARK_CMD_MAX))
    {
        u32Return = _VMARKIoctl(pInstance, u32Cmd, pArgs);
    }
#endif
    return u32Return;
}

MS_U32 _XCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //void* pModule = NULL;
    //void* pResource = NULL;
    XC_INSTANCE_PRIVATE* psXCInstPri = NULL;

    //UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psXCInstPri)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
    }

    if(psXCInstPri->u32DeviceID == 0)
    {
        pu32XCInst_private = pInstance;
    }
    else if(psXCInstPri->u32DeviceID == 1)
    {
        pu32XCInst_1_private = pInstance;
    }

    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    switch(u32Cmd)
    {
        case E_XC_CMD_GET_LIB_VERSION:
        {
            pstXC_GET_LIB_VER pXCArgs = (pstXC_GET_LIB_VER) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetLibVer(pInstance, pXCArgs->ppVersion);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_INFO:
        {

            pstXC_CMD_GET_INFO  pXCArgs = (pstXC_CMD_GET_INFO) pArgs;
#ifndef MSOS_TYPE_LINUX_KERNEL
            pXCArgs->stReturnValue = psXCInstPri->fpXC_GetInfo(pInstance);
#else
            memcpy(pXCArgs->stReturnValue,psXCInstPri->fpXC_GetInfo(pInstance),sizeof(XC_ApiInfo));
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_STATUS:
        {

            pstXC_GET_STATUS pXCArgs = (pstXC_GET_STATUS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetStatus(pInstance,pXCArgs->pDrvStatus, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_STATUS_NODELAY:
        {

            PST_XC_GET_STATUS_NODELAY pXCArgs = (PST_XC_GET_STATUS_NODELAY) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetStatus_nodelay(pInstance,pXCArgs->pDrvStatus, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_STATUS_EX:
        {

            pstXC_GET_STATUS_EX  pXCArgs = (pstXC_GET_STATUS_EX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetStatusEx(pInstance,pXCArgs->pDrvStatusEx, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DBG_LEVEL:
        {

            pstXC_SET_DBG_LEVEL pXCArgs = (pstXC_SET_DBG_LEVEL) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetDbgLevel(pInstance,pXCArgs->u16DbgSwitch);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_FPLL_CUSTOMERMODE:
        {

            pstXC_SET_FPLL_CUSTOMERMODE pXCArgs = (pstXC_SET_FPLL_CUSTOMERMODE) pArgs;
            psXCInstPri->fpXC_FPLLCustomerMode(pInstance,pXCArgs->eMode, pXCArgs->eFlag, pXCArgs->u32Data);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IOMAPBASE:

        {

            pstXC_CMD_SET_IOMAPBASE pXCArgs = (pstXC_CMD_SET_IOMAPBASE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetIOMapBase(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_INIT:
        {

            pstXC_INIT pXCArgs = (pstXC_INIT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Init(pInstance,pXCArgs->pXC_InitData, pXCArgs->u32InitDataLen);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            //because use sc1 main as sc0 sub, sc1 mian use sc0 sub frame buffer
            if (pXCArgs->pXC_InitData != NULL)
            {
                pXCArgs->pXC_InitData->u32Main_FB_Start_Addr = pXCArgs->pXC_InitData->u32Sub_FB_Start_Addr;
                pXCArgs->pXC_InitData->u32Main_FB_Size = pXCArgs->pXC_InitData->u32Sub_FB_Size;
            }
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Init(g_pDevice1Instance,pXCArgs->pXC_InitData, pXCArgs->u32InitDataLen);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_INIT_MISC:
        {

            pstXC_INIT_MISC pXCArgs = (pstXC_INIT_MISC) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Init_MISC(pInstance,pXCArgs->pXC_Init_Misc, pXCArgs->u32InitMiscDataLen);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_MISC_STATUS:
        {

            pstXC_GET_MISC_STATUS pXCArgs = (pstXC_GET_MISC_STATUS) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetMISCStatus(pInstance,pXCArgs->pXC_Init_Misc);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CAPABILITY:
        {

            pstXC_GET_CAPABILITY pXCArgs = (pstXC_GET_CAPABILITY) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_GetCapability(pInstance,pXCArgs->u32Id);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CHIPCAPS:
        {


            pstXC_GET_CHIP_CAPS pXCArgs = (pstXC_GET_CHIP_CAPS) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetChipCaps(pInstance,pXCArgs->eCapType, pXCArgs->pRet, pXCArgs->ret_size);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_EXIT:
        {

            pstXC_EXIT pXCArgs = (pstXC_EXIT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Exit(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Exit(g_pDevice1Instance);
#endif
            printf("XC Exited\n");

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DYNAMIC_SCALING:
        {

            pstXC_SET_DYNAMIC_SCALING pXCArgs = (pstXC_SET_DYNAMIC_SCALING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetDynamicScaling(pInstance,pXCArgs->pstDSInfo, pXCArgs->u32DSInfoLen, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_DYNAMIC_SCALING_FLAG:
        {

            pstXC_SET_DYNAMIC_SCALING_FLAG pXCArgs = (pstXC_SET_DYNAMIC_SCALING_FLAG) pArgs;
            psXCInstPri->fpXC_SetDynamicScalingFlag(pInstance,pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetDynamicScalingFlag(g_pDevice1Instance,pXCArgs->bEnable);
#endif

            break;
        }

        case E_XC_CMD_GET_DNR_BASEOFFSET:
        {

            pstXC_GET_DNR_BASEOFFSET pXCArgs = (pstXC_GET_DNR_BASEOFFSET) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_Get_DNRBaseOffset(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_STORE_FRAMENUM:
        case E_XC_CMD_GET_FRAMENUM_FACTOR:
        {

            pstXC_GET_FRAMENUM_FACTOR pXCArgs = (pstXC_GET_FRAMENUM_FACTOR) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_Get_FrameNumFactor(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_WINDOW:
        {
            pstXC_SET_WINDOW pXCArgs = (pstXC_SET_WINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetWindow(pInstance, pXCArgs->pstXC_SetWin_Info, pXCArgs->u32InitDataLen, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_DUALWINDOW:
        {
            pstXC_SET_DUALWINDOW pXCArgs = (pstXC_SET_DUALWINDOW) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetDualWindow(pInstance, pXCArgs->pstXC_SetWin_Info_Main, pXCArgs->pstXC_SetWin_Info_Sub);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_TRAVELING_WINDOW:
        {


            pstXC_SET_TRAVELING_WINDOW pXCArgs = (pstXC_SET_TRAVELING_WINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetTravelingWindow(pInstance, pXCArgs->pstXC_SetWin_Info, pXCArgs->u32InitDataLen, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_INPUTSOURCE:
        {

            pstXC_SET_INPUTSOURCE pXCArgs = (pstXC_SET_INPUTSOURCE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetInputSource(pInstance, pXCArgs->enInputSourceType, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;

            break;
        }


        case E_XC_CMD_CHECK_YUVSPACE:
        {


            pstXC_CHECK_YUVSPACE pXCArgs = (pstXC_CHECK_YUVSPACE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsYUVSpace(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_MEMORYFORMAT422:
        {


            pstXC_CHECK_MEMORYFORMAT422 pXCArgs = (pstXC_CHECK_MEMORYFORMAT422) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsMemoryFormat422(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FORCE_RGBIN:
        {


            pstXC_SET_FORCE_RGBIN pXCArgs = (pstXC_SET_FORCE_RGBIN) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_EnableForceRGBin(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MIRRORMODE_EX:
        {


            pstXC_SET_MIRRORMODE_EX pXCArgs = (pstXC_SET_MIRRORMODE_EX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_EnableMirrorModeEx(pInstance, pXCArgs->eMirrorMode, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_MIRRORMODE_TYPEEX:
        {


            pstXC_GET_MIRRORMODE_TYPEEX pXCArgs = (pstXC_GET_MIRRORMODE_TYPEEX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetMirrorModeTypeEx(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_SYNC_STATUS:
        {


            pstXC_GET_SYNC_STATUS pXCArgs = (pstXC_GET_SYNC_STATUS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GetSyncStatus(pInstance, pXCArgs->eCurrentSrc, pXCArgs->sXC_Sync_Status, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_CONFIG_CMA:
        {


            pstXC_CONFIG_CMA pXCArgs = (pstXC_CONFIG_CMA) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_ConfigCMA(pInstance, pXCArgs->pstXC_CMA_Config, pXCArgs->enCmaClient, pXCArgs->u32InitDataLen, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_WAIT_OUTPUT_VSYNC:
        {


            pstXC_SET_WAIT_OUTPUT_VSYNC pXCArgs = (pstXC_SET_WAIT_OUTPUT_VSYNC) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_WaitOutputVSync(pInstance, pXCArgs->u8NumVSyncs, pXCArgs->u16Timeout, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_WAIT_INPUT_VSYNC:
        {


            pstXC_SET_WAIT_INPUT_VSYNC pXCArgs = (pstXC_SET_WAIT_INPUT_VSYNC) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_WaitInputVSync(pInstance, pXCArgs->u8NumVSyncs, pXCArgs->u16Timeout, pXCArgs->eWindow);



            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_HDMI_SYNCMODE:
        {


            pstXC_SET_HDMI_SYNCMODE pXCArgs = (pstXC_SET_HDMI_SYNCMODE) pArgs;
            psXCInstPri->fpXC_SetHdmiSyncMode(pInstance, pXCArgs->eSynctype);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetHdmiSyncMode(g_pDevice1Instance, pXCArgs->eSynctype);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_HDMI_SYNCMODE:
        {


            pstXC_GET_HDMI_SYNCMODE pXCArgs = (pstXC_GET_HDMI_SYNCMODE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetHdmiSyncMode(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_REPORT_WINDOW:
        {
            pstXC_SET_REPORT_WINDOW pXCArgs = (pstXC_SET_REPORT_WINDOW) pArgs;
            psXCInstPri->fpXC_SetRepWindow(pInstance, pXCArgs->bEnable, pXCArgs->Window, pXCArgs->u8Color);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetRepWindow(g_pDevice1Instance, pXCArgs->bEnable, pXCArgs->Window, pXCArgs->u8Color);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_SKIP_OPWRITEOFF_IN_SETWINDOW:
        {
            pstXC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW pXCArgs = (pstXC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SkipOPWriteOffInSetWindow(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_SKIP_OPWRITEOFF_IN_SETWINDOW:
        {
            pstXC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW pXCArgs = (pstXC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetSkipOPWriteOffInSetWindow(pInstance,pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL:
        {
            pstXC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL pXCArgs = (pstXC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetSkipDisableOPWriteOffInFPLL(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_OPWRITEOFF_ENABLE:
        {
            pstXC_SET_OPWRITEOFF_ENABLE pXCArgs = (pstXC_SET_OPWRITEOFF_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_OPWriteOffEnable(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_OPWRITEOFF_ENABLE_TO_REG:
        {
            pstXC_SET_OPWRITEOFF_ENABLE_TO_REG pXCArgs = (pstXC_SET_OPWRITEOFF_ENABLE_TO_REG) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_OPWriteOffEnableToReg(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_FORCESET_OPWRITEOFF_ENABLE:
        {
            pstXC_FORCESET_OPWRITEOFF_ENABLE pXCArgs = (pstXC_FORCESET_OPWRITEOFF_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_ForceSet_OPWriteOffEnable(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_OPWRITEOFF_ENABLE:
        {
            pstXC_GET_OPWRITEOFF_ENABLE pXCArgs = (pstXC_GET_OPWRITEOFF_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_OPWriteOffEnable(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_DISPLAY_WIN_TO_REG:
        {
            pstXC_SET_DISPLAY_WIN_TO_REG pXCArgs = (pstXC_SET_DISPLAY_WIN_TO_REG) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetDispWinToReg(pInstance, pXCArgs->pstDspwin, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_DISPLAY_WIN_FROM_REG:
        {


            pstXC_GET_DISPLAY_WIN_FROM_REG pXCArgs = (pstXC_GET_DISPLAY_WIN_FROM_REG) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GetDispWinFromReg(pInstance, pXCArgs->pstDspwin, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FREEZEIMG:
        {

            pstXC_SET_FREEZEIMG pXCArgs = (pstXC_SET_FREEZEIMG) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_FreezeImg(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_CHECK_FREEZEIMG:
        {


            pstXC_CHECK_FREEZEIMG pXCArgs = (pstXC_CHECK_FREEZEIMG) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsFreezeImg(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_BOTHWINDOW_BLACKVIDEO:
        {


            pstXC_SET_BOTHWINDOW_BLACKVIDEO pXCArgs = (pstXC_SET_BOTHWINDOW_BLACKVIDEO) pArgs;
            psXCInstPri->fpXC_GenerateBlackVideoForBothWin(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BLACKSCREEN:
        {


            pstXC_SET_BLACKSCREEN pXCArgs = (pstXC_SET_BLACKSCREEN) pArgs;
            psXCInstPri->fpXC_Set_BLSK(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Set_BLSK(g_pDevice1Instance, pXCArgs->bEnable);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BLACKVIDEO:
        {


            pstXC_SET_BLACKVIDEO pXCArgs = (pstXC_SET_BLACKVIDEO) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GenerateBlackVideo(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_BLACKVIDEO_ENABLE:
        {

            pstXC_CHECK_BLACKVIDEO_ENABLE pXCArgs = (pstXC_CHECK_BLACKVIDEO_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsBlackVideoEnable(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRAMEBUFFERLESS:
        {


            pstXC_SET_FRAMEBUFFERLESS pXCArgs = (pstXC_SET_FRAMEBUFFERLESS) pArgs;
            psXCInstPri->fpXC_EnableFrameBufferLess(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableFrameBufferLess(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_FRAMEBUFFERLESS:
        {


            pstXC_CHECK_FRAMEBUFFERLESS pXCArgs = (pstXC_CHECK_FRAMEBUFFERLESS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsCurrentFrameBufferLessMode(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_REQUEST_FRAMEBUFFERLESS:
        {


            pstXC_SET_REQUEST_FRAMEBUFFERLESS pXCArgs = (pstXC_SET_REQUEST_FRAMEBUFFERLESS) pArgs;
            psXCInstPri->fpXC_EnableRequest_FrameBufferLess(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableRequest_FrameBufferLess(g_pDevice1Instance, pXCArgs->bEnable);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS:
        {


            pstXC_CHECK_REQUEST_FRAMEBUFFERLESS pXCArgs = (pstXC_CHECK_REQUEST_FRAMEBUFFERLESS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsCurrentRequest_FrameBufferLessMode(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_HWVERSION:
        {


            pstXC_GET_3D_HWVERSION pXCArgs = (pstXC_GET_3D_HWVERSION) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_Get_3D_HW_Version(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_SUPPORT_HW2DTO3D:
        {


            pstXC_CHECK_3D_SUPPORT_HW2DTO3D pXCArgs = (pstXC_CHECK_3D_SUPPORT_HW2DTO3D) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_3D_IsSupportedHW2DTo3D(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_MODE:
        {


            pstXC_SET_3D_MODE pXCArgs = (pstXC_SET_3D_MODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_Mode(pInstance,
                                    pXCArgs->e3dInputMode,
                                    pXCArgs->e3dOutputMode,
                                    pXCArgs->e3dPanelType,
                                    pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_MAINWIN_FIRST:
        {


            pstXC_SET_3D_MAINWIN_FIRST pXCArgs = (pstXC_SET_3D_MAINWIN_FIRST) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_MainWin_FirstMode(pInstance, pXCArgs->bMainFirst);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_LR_EXCHANGE:
        {


            pstXC_SET_3D_LR_EXCHANGE pXCArgs = (pstXC_SET_3D_LR_EXCHANGE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_LR_Frame_Exchg(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_LR_EXCHANGED:
        {


            pstXC_CHECK_3D_LR_EXCHANGED pXCArgs = (pstXC_CHECK_3D_LR_EXCHANGED) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_3D_Is_LR_Frame_Exchged(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_INPUT_MODE:
        {


            pstXC_GET_3D_INPUT_MODE pXCArgs = (pstXC_GET_3D_INPUT_MODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_3D_Input_Mode(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_OUTPUT_MODE:
        {


            pstXC_GET_3D_OUTPUT_MODE pXCArgs = (pstXC_GET_3D_OUTPUT_MODE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_3D_Output_Mode(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


#ifdef UFO_XC_GET_3D_FORMAT
        case E_XC_CMD_GET3DFORMAT:
        {


            pstXC_GET3DFORMAT pXCArgs = (pstXC_GET3DFORMAT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get3DFormat(pInstance, pXCArgs->e3DAttrType, pXCArgs->para, pXCArgs->p3DFormat);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif


        case E_XC_CMD_GET_3D_PANELTYPE:
        {


            pstXC_GET_3D_PANELTYPE pXCArgs = (pstXC_GET_3D_PANELTYPE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_3D_Panel_Type(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_MAINWIN_FIRST:
        {


            pstXC_GET_3D_MAINWIN_FIRST pXCArgs = (pstXC_GET_3D_MAINWIN_FIRST) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_3D_MainWin_First(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_MAINSUB_IPSYNC:
        {


            pstXC_CHECK_3D_MAINSUB_IPSYNC pXCArgs = (pstXC_CHECK_3D_MAINSUB_IPSYNC) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_3DMainSub_IPSync(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_VERTICALVIDEO_OFFSET:
        {


            pstXC_SET_3D_VERTICALVIDEO_OFFSET pXCArgs = (pstXC_SET_3D_VERTICALVIDEO_OFFSET) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_VerVideoOffset(pInstance, pXCArgs->u163DVerVideoOffset);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_VERTICALVIDEO_OFFSET:
        {


            pstXC_GET_3D_VERTICALVIDEO_OFFSET pXCArgs = (pstXC_GET_3D_VERTICALVIDEO_OFFSET) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_Get_3D_VerVideoOffset(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_FORMAT_SUPPORTED:
        {


            pstXC_CHECK_3D_FORMAT_SUPPORTED pXCArgs = (pstXC_CHECK_3D_FORMAT_SUPPORTED) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Is3DFormatSupported(pInstance, pXCArgs->e3dInputMode, pXCArgs->e3dOutputMode);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_HORIZONTAL_SHIFT:
        {


            pstXC_SET_3D_HORIZONTAL_SHIFT pXCArgs = (pstXC_SET_3D_HORIZONTAL_SHIFT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_HShift(pInstance, pXCArgs->u16HShift);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_LR_SBS2LINE:
        {


            pstXC_SET_3D_LR_SBS2LINE pXCArgs = (pstXC_SET_3D_LR_SBS2LINE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Enable_3D_LR_Sbs2Line(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_HORIZONTAL_SHIFT:
        {


            pstXC_GET_3D_HORIZONTAL_SHIFT pXCArgs = (pstXC_GET_3D_HORIZONTAL_SHIFT) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_Get_3D_HShift(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_HW2DTO3D_BUFFER:
        {


            pstXC_SET_3D_HW2DTO3D_BUFFER pXCArgs = (pstXC_SET_3D_HW2DTO3D_BUFFER) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_HW2DTo3D_Buffer(pInstance, pXCArgs->u32HW2DTO3D_DD_Buf, pXCArgs->u32HW2DTO3D_DR_Buf);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_HW2DTO3D_PARAMETERS:
        {


            pstXC_SET_3D_HW2DTO3D_PARAMETERS pXCArgs = (pstXC_SET_3D_HW2DTO3D_PARAMETERS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_HW2DTo3D_Parameters(pInstance, pXCArgs->st3DHw2DTo3DPara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_HW2DTO3D_PARAMETERS:
        {


            pstXC_GET_3D_HW2DTO3D_PARAMETERS pXCArgs = (pstXC_GET_3D_HW2DTO3D_PARAMETERS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_3D_HW2DTo3D_Parameters(pInstance, pXCArgs->pst3DHw2DTo3DPara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_DETECT_3DFORMAT_PARAMETERS:
        {


            pstXC_SET_3D_DETECT_3DFORMAT_PARAMETERS pXCArgs = (pstXC_SET_3D_DETECT_3DFORMAT_PARAMETERS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_Detect3DFormat_Parameters(pInstance, pXCArgs->pstDetect3DFormatPara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_DETECT_3DFORMAT_PARAMETERS:
        {


            pstXC_GET_3D_DETECT_3DFORMAT_PARAMETERS pXCArgs = (pstXC_GET_3D_DETECT_3DFORMAT_PARAMETERS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_3D_Detect3DFormat_Parameters(pInstance, pXCArgs->pstDetect3DFormatPara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_FORMAT_DETECTED_BY_CONTENT:
        {


            pstXC_GET_3D_FORMAT_DETECTED_BY_CONTENT pXCArgs = (pstXC_GET_3D_FORMAT_DETECTED_BY_CONTENT) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Detect3DFormatByContent(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_NINELATTICE:
        {


            pstXC_SET_NINELATTICE pXCArgs = (pstXC_SET_NINELATTICE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_DetectNL(pInstance, pXCArgs->eWindow, pXCArgs->pstDetectNLatticePara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_POST_PQSETTING:
        {


            pstXC_SET_3D_POST_PQSETTING pXCArgs = (pstXC_SET_3D_POST_PQSETTING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_3D_PostPQSetting(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_FRAMEPACKING_INFO:
        {


            pstXC_SET_3D_FRAMEPACKING_INFO pXCArgs = (pstXC_SET_3D_FRAMEPACKING_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_FPInfo(pInstance, pXCArgs->pstFPInfoPara);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_AUTODETECT:
        {


            pstXC_SET_3D_AUTODETECT pXCArgs = (pstXC_SET_3D_AUTODETECT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_EnableAutoDetect3D(pInstance, pXCArgs->bEnable, pXCArgs->enDetectMethod);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_3D_AUTODETECT_3DFLAG:
        {


            pstXC_GET_3D_AUTODETECT_3DFLAG pXCArgs = (pstXC_GET_3D_AUTODETECT_3DFLAG) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetAutoDetect3DFlag(pInstance, pXCArgs->penDetectMethod, pXCArgs->pbEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_SUBWINCLOCK:
        {


            pstXC_SET_3D_SUBWINCLOCK pXCArgs = (pstXC_SET_3D_SUBWINCLOCK) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_3D_SubWinClk(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_LR_SBS2LINE:
        {


            pstXC_CHECK_3D_LR_SBS2LINE pXCArgs = (pstXC_CHECK_3D_LR_SBS2LINE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_3D_Is_LR_Sbs2Line(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_3D_SKIP_DEFAULT_LR_FLAG:
        {


            pstXC_CHECK_3D_SKIP_DEFAULT_LR_FLAG pXCArgs = (pstXC_CHECK_3D_SKIP_DEFAULT_LR_FLAG) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSC_3D_Is_Skip_Default_LR_Flag(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_3D_SKIP_DEFAULT_LR_FLAG:
        {


            pstXC_SET_3D_SKIP_DEFAULT_LR_FLAG pXCArgs = (pstXC_SET_3D_SKIP_DEFAULT_LR_FLAG) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_3D_Enable_Skip_Default_LR_Flag(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_INIT:
        {


            pstXC_SET_MUX_INIT pXCArgs = (pstXC_SET_MUX_INIT) pArgs;
            psXCInstPri->fpXC_Mux_Init(pInstance, pXCArgs->input_source_to_input_port);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_Init(g_pDevice1Instance, pXCArgs->input_source_to_input_port);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_SOURCE_MONITOR:
        {


            pstXC_SET_MUX_SOURCE_MONITOR pXCArgs = (pstXC_SET_MUX_SOURCE_MONITOR) pArgs;
            psXCInstPri->fpXC_Mux_SourceMonitor(pInstance, pXCArgs->bRealTimeMonitorOnly);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_SourceMonitor(g_pDevice1Instance, pXCArgs->bRealTimeMonitorOnly);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_CREATE_PATH:
        {


            pstXC_SET_MUX_CREATE_PATH pXCArgs = (pstXC_SET_MUX_CREATE_PATH) pArgs;
            pXCArgs->s16ReturnValue = psXCInstPri->fpXC_Mux_CreatePath(pInstance, pXCArgs->Path_Info, pXCArgs->u32InitDataLen);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_CreatePath(g_pDevice1Instance, pXCArgs->Path_Info, pXCArgs->u32InitDataLen);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_DELETE_PATH:
        {

            pstXC_SET_MUX_DELETE_PATH pXCArgs = (pstXC_SET_MUX_DELETE_PATH) pArgs;
            pXCArgs->s16ReturnValue = psXCInstPri->fpXC_Mux_DeletePath(pInstance, pXCArgs->src, pXCArgs->dest);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_ENABLE_PATH:
        {


            pstXC_SET_MUX_ENABLE_PATH pXCArgs = (pstXC_SET_MUX_ENABLE_PATH) pArgs;
            pXCArgs->s16ReturnValue = psXCInstPri->fpXC_Mux_EnablePath(pInstance, pXCArgs->PathId);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_TRIGGER_PATH_SYNC_EVENT:
        {


            pstXC_SET_MUX_TRIGGER_PATH_SYNC_EVENT pXCArgs = (pstXC_SET_MUX_TRIGGER_PATH_SYNC_EVENT) pArgs;
            psXCInstPri->fpXC_Mux_TriggerPathSyncEvent(pInstance, pXCArgs->src, pXCArgs->para);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_TriggerPathSyncEvent(g_pDevice1Instance, pXCArgs->src, pXCArgs->para);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_TRIGGER_DEST_ONOFF_EVENT:
        {


            pstXC_SET_MUX_TRIGGER_DEST_ONOFF_EVENT pXCArgs = (pstXC_SET_MUX_TRIGGER_DEST_ONOFF_EVENT) pArgs;
            psXCInstPri->fpXC_Mux_TriggerDestOnOffEvent(pInstance, pXCArgs->src, pXCArgs->para);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_TriggerDestOnOffEvent(g_pDevice1Instance, pXCArgs->src, pXCArgs->para);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_ONOFF_PERIODIC_HANDLER:
        {


            pstXC_SET_MUX_ONOFF_PERIODIC_HANDLER pXCArgs = (pstXC_SET_MUX_ONOFF_PERIODIC_HANDLER) pArgs;
            pXCArgs->s16ReturnValue = psXCInstPri->fpXC_Mux_OnOffPeriodicHandler(pInstance, pXCArgs->src, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_OnOffPeriodicHandler(g_pDevice1Instance, pXCArgs->src, pXCArgs->bEnable);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_MUX_PATHINFO:
        {


            pstXC_GET_MUX_PATHINFO pXCArgs = (pstXC_GET_MUX_PATHINFO) pArgs;
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_Mux_GetPathInfo(pInstance, pXCArgs->Paths);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_GetPathInfo(g_pDevice1Instance, pXCArgs->Paths);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_SUPPORT_MHL_PATHINFO:
        {


            pstXC_SET_MUX_SUPPORT_MHL_PATHINFO pXCArgs = (pstXC_SET_MUX_SUPPORT_MHL_PATHINFO) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Mux_SetSupportMhlPathInfo(pInstance, pXCArgs->u8MhlSupportInfo);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_SetSupportMhlPathInfo(g_pDevice1Instance, pXCArgs->u8MhlSupportInfo);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MUX_MHL_HOTPLUG_INVERSE_INFO:
        {


            pstXC_SET_MUX_MHL_HOTPLUG_INVERSE_INFO pXCArgs = (pstXC_SET_MUX_MHL_HOTPLUG_INVERSE_INFO) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Mux_SetMhlHotPlugInverseInfo(pInstance, pXCArgs->bIsMhlHotPlugInverse);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_SetMhlHotPlugInverseInfo(g_pDevice1Instance, pXCArgs->bIsMhlHotPlugInverse);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_MUX_HDMIPORT:
        {


            pstXC_GET_MUX_HDMIPORT pXCArgs = (pstXC_GET_MUX_HDMIPORT) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Mux_GetHDMIPort(pInstance, pXCArgs->src);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_GetHDMIPort(g_pDevice1Instance, pXCArgs->src);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_MUX_MAPPINGTAB:
        {
            pstXC_GET_MUX_MAPPINGTAB pXCArgs = (pstXC_GET_MUX_MAPPINGTAB) pArgs;
            psXCInstPri->fpXC_Mux_GetMappingTab(pInstance, pXCArgs->mapping_tab,pXCArgs->length);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Mux_GetMappingTab(g_pDevice1Instance, pXCArgs->mapping_tab,pXCArgs->length);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_MUX_INPUTSOURCE2VDYMUXPORT:
        {


            pstXC_GET_MUX_INPUTSOURCE2VDYMUXPORT pXCArgs = (pstXC_GET_MUX_INPUTSOURCE2VDYMUXPORT) pArgs;
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_MUX_MApinputSourceToVDYMuxPORT(pInstance, pXCArgs->u8InputSourceType);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_NR:
        {


            pstXC_SET_NR pXCArgs = (pstXC_SET_NR) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_NR(pInstance, pXCArgs->bEn, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FILM_MODE_PATCH:
        {


            psXCInstPri->fpXC_FilmMode_P(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_UC_ENABLED:
        {


            pstXC_CHECK_UC_ENABLED pXCArgs = (pstXC_CHECK_UC_ENABLED) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetUCEnabled(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_GENERATE_SPECIFIC_TIMING:
        {


            pstXC_SET_GENERATE_SPECIFIC_TIMING pXCArgs = (pstXC_SET_GENERATE_SPECIFIC_TIMING) pArgs;
            psXCInstPri->fpXC_GenSpecificTiming(pInstance, pXCArgs->timingtype);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_DE_BYPASS_MODE:
        {


            pstXC_GET_DE_BYPASS_MODE pXCArgs = (pstXC_GET_DE_BYPASS_MODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetDEBypassMode(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_DE_WINDOW:
        {


            pstXC_GET_DE_WINDOW pXCArgs = (pstXC_GET_DE_WINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GetDEWindow(pInstance, pXCArgs->psWin, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE:
        {


            pstXC_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE pXCArgs = (pstXC_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GetDEWidthHeightInDEByPassMode(pInstance, pXCArgs->pu16Width, pXCArgs->pu16Height, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CAPTURE_WINDOW:
        {


            pstXC_GET_CAPTURE_WINDOW pXCArgs = (pstXC_GET_CAPTURE_WINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_GetCaptureWindow(pInstance, pXCArgs->capture_win, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CAPTURE_WINDOW_VSTART:
        {


            pstXC_SET_CAPTURE_WINDOW_VSTART pXCArgs = (pstXC_SET_CAPTURE_WINDOW_VSTART) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetCaptureWindowVstart(pInstance, pXCArgs->u16Vstart, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CAPTURE_WINDOW_HSTART:
        {


            pstXC_SET_CAPTURE_WINDOW_HSTART pXCArgs = (pstXC_SET_CAPTURE_WINDOW_HSTART) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetCaptureWindowHstart(pInstance, pXCArgs->u16Hstart, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CAPTURE_WINDOW_VSIZE:
        {


            pstXC_SET_CAPTURE_WINDOW_VSIZE pXCArgs = (pstXC_SET_CAPTURE_WINDOW_VSIZE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetCaptureWindowVsize(pInstance, pXCArgs->u16Vsize, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CAPTURE_WINDOW_HSIZE:
        {


            pstXC_SET_CAPTURE_WINDOW_HSIZE pXCArgs = (pstXC_SET_CAPTURE_WINDOW_HSIZE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetCaptureWindowHsize(pInstance, pXCArgs->u16Hsize, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SOFTWARE_RESET:
        {


            pstXC_SET_SOFTWARE_RESET pXCArgs = (pstXC_SET_SOFTWARE_RESET) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SoftwareReset(pInstance, pXCArgs->u8Reset, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_HFREQX10:
        {


            pstXC_GET_HFREQX10 pXCArgs = (pstXC_GET_HFREQX10) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_CalculateHFreqx10(pInstance, pXCArgs->u16HPeriod);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_HFREQX1K:
        {


            pstXC_GET_HFREQX1K pXCArgs = (pstXC_GET_HFREQX1K) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_CalculateHFreqx1K(pInstance, pXCArgs->u16HPeriod);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_VFREQX10:
        {


            pstXC_GET_VFREQX10 pXCArgs = (pstXC_GET_VFREQX10) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_CalculateVFreqx10(pInstance, pXCArgs->u16HFreq, pXCArgs->u16VTotal);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_VFREQX1K:
        {


            pstXC_GET_VFREQX1K pXCArgs = (pstXC_GET_VFREQX1K) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_CalculateVFreqx1K(pInstance, pXCArgs->u32HFreqx1K, pXCArgs->u16VTotal);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_ACCURATE_VFREQX1k:
        {


            pstXC_GET_ACCURATE_VFREQX1k pXCArgs = (pstXC_GET_ACCURATE_VFREQX1k) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_GetAccurateVFreqx1K(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_INTERRUPT_ATTACH:
        {


            pstXC_SET_INTERRUPT_ATTACH pXCArgs = (pstXC_SET_INTERRUPT_ATTACH) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_InterruptAttach(pInstance, pXCArgs->eIntNum, pXCArgs->pIntCb, pXCArgs->pParam);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_INTERRUPT_DEATTACH:
        {


            pstXC_SET_INTERRUPT_DEATTACH pXCArgs = (pstXC_SET_INTERRUPT_DEATTACH) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_InterruptDeAttach(pInstance, pXCArgs->eIntNum, pXCArgs->pIntCb, pXCArgs->pParam);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DISABLE_INPUTSOURCE:
        {


            pstXC_SET_DISABLE_INPUTSOURCE pXCArgs = (pstXC_SET_DISABLE_INPUTSOURCE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(!MDrv_XC_GetMWEStatus(pInstance))
            {
                USE_OTHER_SC_AS_SUB_WINDOW();
            }
#endif
            psXCInstPri->fpXC_DisableInputSource(pInstance, pXCArgs->bDisable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_INPUTSOURCE_DISABLED:
        {


            pstXC_CHECK_INPUTSOURCE_DISABLED pXCArgs = (pstXC_CHECK_INPUTSOURCE_DISABLED) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsInputSourceDisabled(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CHANGE_PANELTYPE:
        {


            pstXC_SET_CHANGE_PANELTYPE pXCArgs = (pstXC_SET_CHANGE_PANELTYPE) pArgs;
            psXCInstPri->fpXC_ChangePanelType(pInstance, pXCArgs->pstPanelInfo);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CURRENT_READBANK:
        {


            pstXC_GET_CURRENT_READBANK pXCArgs = (pstXC_GET_CURRENT_READBANK) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetCurrentReadBank(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CURRENT_WRITEBANK:
        {


            pstXC_GET_CURRENT_WRITEBANK pXCArgs = (pstXC_GET_CURRENT_WRITEBANK) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetCurrentWriteBank(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_AUTO_PRESCALING:
        {


            pstXC_SET_AUTO_PRESCALING pXCArgs = (pstXC_SET_AUTO_PRESCALING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetAutoPreScaling(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_VSYNC_WIDTH:
        {


            pstXC_GET_VSYNC_WIDTH pXCArgs = (pstXC_GET_VSYNC_WIDTH) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_GetVSyncWidth(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_GOP_ENABLE:
        {


            pstXC_SET_GOP_ENABLE pXCArgs = (pstXC_SET_GOP_ENABLE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_set_GOP_Enable(pInstance, pXCArgs->MaxGOP, pXCArgs->UseNum, pXCArgs->u8MuxNum, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SELECT_IP_FOR_GOP:
        {


            pstXC_SET_SELECT_IP_FOR_GOP pXCArgs = (pstXC_SET_SELECT_IP_FOR_GOP) pArgs;
            psXCInstPri->fpXC_ip_sel_for_gop(pInstance, pXCArgs->u8MuxNum, pXCArgs->ipSelGop);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_VOP_NEW_BLENDING_LEVEL:
        {


            psXCInstPri->fpXC_SetVOPNBL(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_DESTINATION_INFO:
        {


            pstXC_GET_DESTINATION_INFO pXCArgs = (pstXC_GET_DESTINATION_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetDstInfo(pInstance, pXCArgs->pDstInfo, pXCArgs->u32SizeofDstInfo, pXCArgs->XCDstType);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FDMASK_BYWIN:
        {


            pstXC_SET_FDMASK_BYWIN pXCArgs = (pstXC_SET_FDMASK_BYWIN) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_FD_Mask_ByWin(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_FDMASK_BYWIN:
        {


            pstXC_GET_FDMASK_BYWIN pXCArgs = (pstXC_GET_FDMASK_BYWIN) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_FD_Mask_ByWin(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IP1_TESTPATTERN:
        {


            pstXC_SET_IP1_TESTPATTERN pXCArgs = (pstXC_SET_IP1_TESTPATTERN) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetIP1TestPattern(pInstance, pXCArgs->u8Enable, pXCArgs->u6Pattern_type, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#ifdef UFO_XC_TEST_PATTERN
        case E_XC_CMD_SET_TESTPATTERN:
        {


            pstXC_SET_TESTPATTERN pXCArgs = (pstXC_SET_TESTPATTERN) pArgs;
            psXCInstPri->fpXC_GenerateTestPattern(pInstance, pXCArgs->ePatternMode,pXCArgs->para, pXCArgs->u16Length);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
        case E_XC_CMD_SET_INIT_IP_FOR_INTERNAL_TIMING:
        {


            pstXC_SET_INIT_IP_FOR_INTERNAL_TIMING pXCArgs = (pstXC_SET_INIT_IP_FOR_INTERNAL_TIMING) pArgs;
            psXCInstPri->fpXC_InitIPForInternalTiming(pInstance, pXCArgs->timingtype);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IPMUX:
        {


            pstXC_SET_IPMUX pXCArgs = (pstXC_SET_IPMUX) pArgs;
            psXCInstPri->fpXC_SetIPMux(pInstance, pXCArgs->u8Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_HSYNC_ACTIVE:
        {


            pstXC_CHECK_HSYNC_ACTIVE pXCArgs = (pstXC_CHECK_HSYNC_ACTIVE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Is_H_Sync_Active(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_VSYNC_ACTIVE:
        {


            pstXC_CHECK_VSYNC_ACTIVE pXCArgs = (pstXC_CHECK_VSYNC_ACTIVE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Is_V_Sync_Active(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_AUTO_POSITION_WINDOW:
        {


            pstXC_GET_AUTO_POSITION_WINDOW pXCArgs = (pstXC_GET_AUTO_POSITION_WINDOW) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetAutoPositionWindow(pInstance, pXCArgs->u8ValidData, pXCArgs->eWindow, pXCArgs->pstAutoPositionWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRAMEBUFFER_ADDRESS:
        {


            pstXC_SET_FRAMEBUFFER_ADDRESS pXCArgs = (pstXC_SET_FRAMEBUFFER_ADDRESS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetFrameBufferAddress(pInstance, pXCArgs->u32FBAddress, pXCArgs->u32FBSize, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_FRCM_FRAMEBUFFER_ADDRESS:
        {

            pstXC_SET_FRAMEBUFFER_ADDRESS pXCArgs = (pstXC_SET_FRAMEBUFFER_ADDRESS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetFRCMFrameBufferAddress(pInstance, pXCArgs->u32FBAddress, pXCArgs->u32FBSize, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING:
        {


            pstXC_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING pXCArgs = (pstXC_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsFrameBufferEnoughForCusScaling(pInstance, pXCArgs->pstXC_SetWin_Info, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SCALER_MEMORY_REQUEST:
        {


            pstXC_SET_SCALER_MEMORY_REQUEST pXCArgs = (pstXC_SET_SCALER_MEMORY_REQUEST) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetScalerMemoryRequest(pInstance, pXCArgs->bEnable, pXCArgs->eRequestType, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_PIXEL_DATA:
        {


            pstXC_GET_PIXEL_DATA pXCArgs = (pstXC_GET_PIXEL_DATA) pArgs;
            psXCInstPri->fpXC_Get_PixelData(pInstance, pXCArgs->u16CorX, pXCArgs->u16CorY, pXCArgs->pixel);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_AVAILABLE_SIZE:
        {


            pstXC_GET_AVAILABLE_SIZE pXCArgs = (pstXC_GET_AVAILABLE_SIZE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_GetAvailableSize(pInstance, pXCArgs->eWindow, pXCArgs->u8FBNum, pXCArgs->u32InputSize);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRAME_COLOR:
        {


            pstXC_SET_FRAME_COLOR pXCArgs = (pstXC_SET_FRAME_COLOR) pArgs;
            psXCInstPri->fpXC_SetFrameColor(pInstance, pXCArgs->u32aRGB);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DISPLAY_WINDOW_COLOR:
        {


            pstXC_SET_DISPLAY_WINDOW_COLOR pXCArgs = (pstXC_SET_DISPLAY_WINDOW_COLOR) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetDispWindowColor(pInstance, pXCArgs->u8Color, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_SUPPORT_SOURCE_TO_VE:
        {


            pstXC_GET_SUPPORT_SOURCE_TO_VE pXCArgs = (pstXC_GET_SUPPORT_SOURCE_TO_VE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SupportSourceToVE(pInstance, pXCArgs->pOutputCapability);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OUTPUT_CAPTURE:
        {


            pstXC_SET_OUTPUT_CAPTURE pXCArgs = (pstXC_SET_OUTPUT_CAPTURE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOutputCapture(pInstance, pXCArgs->bEnable, pXCArgs->eSourceToVE);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_GAMMA_ONOFF:
        {


            pstXC_SET_GAMMA_ONOFF pXCArgs = (pstXC_SET_GAMMA_ONOFF) pArgs;
            psXCInstPri->fpXC_SetGammaOnOff(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetGammaOnOff(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_PREGAMMA_GAIN:
        {


            pstXC_SET_PREGAMMA_GAIN pXCArgs = (pstXC_SET_PREGAMMA_GAIN) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetPreGammaGain(pInstance, pXCArgs->eWindow, pXCArgs->eVop_Channel, pXCArgs->u16Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_PREGAMMA_OFFSET:
        {


            pstXC_SET_PREGAMMA_OFFSET pXCArgs = (pstXC_SET_PREGAMMA_OFFSET) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetPreGammaOffset(pInstance, pXCArgs->eWindow, pXCArgs->eVop_Channel, pXCArgs->u16Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_PANEL_TIMING:
        {


            pstXC_SET_PANEL_TIMING pXCArgs = (pstXC_SET_PANEL_TIMING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetPanelTiming(pInstance, pXCArgs->pTimingInfo, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OUTPUT_TIMING_MODE:
        {


            pstXC_SET_OUTPUT_TIMING_MODE pXCArgs = (pstXC_SET_OUTPUT_TIMING_MODE) pArgs;
            psXCInstPri->fpXC_SetOutTimingMode(pInstance, pXCArgs->enPnl_Out_Timing_Mode);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetOutTimingMode(g_pDevice1Instance, pXCArgs->enPnl_Out_Timing_Mode);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FREERUN_TIMING:
        {


            psXCInstPri->fpXC_SetFreeRunTiming(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CUSTOMER_SYNC_INFO:
        {


            pstXC_SET_CUSTOMER_SYNC_INFO pXCArgs = (pstXC_SET_CUSTOMER_SYNC_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_CustomerSyncInfo(pInstance,
                                    pXCArgs->u16FrontPorch,
                                    pXCArgs->u16SyncWidth,
                                    pXCArgs->u16OutputVfreq);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_WAIT_FPLL_DONE:
        {


            pstXC_GET_WAIT_FPLL_DONE pXCArgs = (pstXC_GET_WAIT_FPLL_DONE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_WaitFPLLDone(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_OUTPUT_VFREQX100:
        {


            pstXC_GET_OUTPUT_VFREQX100 pXCArgs = (pstXC_GET_OUTPUT_VFREQX100) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetOutputVFreqX100(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_OP1_OUTPUT_VFREQX100:
        {


            pstXC_GET_OP1_OUTPUT_VFREQX100 pXCArgs = (pstXC_GET_OP1_OUTPUT_VFREQX100) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_GetOP1OutputVFreqX100(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_FRAMELOCK:
        {


            pstXC_CHECK_FRAMELOCK pXCArgs = (pstXC_CHECK_FRAMELOCK) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_FrameLockCheck(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CUSTOMIZE_FRC_TABLE:
        {


            pstXC_SET_CUSTOMIZE_FRC_TABLE pXCArgs = (pstXC_SET_CUSTOMIZE_FRC_TABLE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_CustomizeFRCTable(pInstance, pXCArgs->stFRCTable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_CustomizeFRCTable(g_pDevice1Instance, pXCArgs->stFRCTable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OUTPUT_FRAME_CONTROL:
        {


            pstXC_SET_OUTPUT_FRAME_CONTROL pXCArgs = (pstXC_SET_OUTPUT_FRAME_CONTROL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OutputFrameCtrl(pInstance, pXCArgs->bEnable, pXCArgs->pstOutFrameInfo, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FPLL_FSM_ENABLE:
        {


            pstXC_SET_FPLL_FSM_ENABLE pXCArgs = (pstXC_SET_FPLL_FSM_ENABLE) pArgs;
            psXCInstPri->fpSC_Enable_FPLL_FSM(pInstance, pXCArgs->bTrue);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpSC_Enable_FPLL_FSM(g_pDevice1Instance, pXCArgs->bTrue);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FORCE_FREERUN:
        {


            pstXC_SET_FORCE_FREERUN pXCArgs = (pstXC_SET_FORCE_FREERUN) pArgs;
            psXCInstPri->fpSC_ForceFreerun(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpSC_ForceFreerun(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_FORCE_FREERUN:
        {


            pstXC_CHECK_FORCE_FREERUN pXCArgs = (pstXC_CHECK_FORCE_FREERUN) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSC_IsForceFreerun(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FREERUN_FREQ:
        {


            pstXC_SET_FREERUN_FREQ pXCArgs = (pstXC_SET_FREERUN_FREQ) pArgs;
            psXCInstPri->fpSC_SetFreerunVFreq(pInstance, pXCArgs->VFreq);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpSC_SetFreerunVFreq(g_pDevice1Instance, pXCArgs->VFreq);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_EXPANEL_INFO:
        {


            pstXC_SET_EXPANEL_INFO pXCArgs = (pstXC_SET_EXPANEL_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetExPanelInfo(pInstance, pXCArgs->bEnable, pXCArgs->pPanelInfoEx);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FPLL_THRESHMODE:
        {

            pstXC_SET_FPLL_THRESHMODE pXCArgs = (pstXC_SET_FPLL_THRESHMODE) pArgs;
            psXCInstPri->fpXC_Enable_FPLL_Thresh_Mode(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Enable_FPLL_Thresh_Mode(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_FPLL_THRESHMODE:
        {
            pstXC_GET_FPLL_THRESHMODE pXCArgs = (pstXC_GET_FPLL_THRESHMODE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_FPLL_Thresh_Mode(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_IPAUTONOSIGNAL:
        {


            pstXC_SET_IPAUTONOSIGNAL pXCArgs = (pstXC_SET_IPAUTONOSIGNAL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_EnableIPAutoNoSignal(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_IPAUTONOSIGNAL:
        {

            pstXC_GET_IPAUTONOSIGNAL pXCArgs = (pstXC_GET_IPAUTONOSIGNAL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetIPAutoNoSignal(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IPAUTOCOAST:
        {


            pstXC_SET_IPAUTOCOAST pXCArgs = (pstXC_SET_IPAUTOCOAST) pArgs;
            psXCInstPri->fpXC_EnableIPAutoCoast(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableIPAutoCoast(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IPCOAST_DEBOUNCE_ENABLE:
        {


            psXCInstPri->fpXC_EnableIPCoastDebounce(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableIPCoastDebounce(g_pDevice1Instance);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CLEAR_IPCOAST_STATUS:
        {


            psXCInstPri->fpXC_ClearIPCoastStatus(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_ClearIPCoastStatus(g_pDevice1Instance);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FPLL_MANUAL_SETTING_ENABLE:
        {


            pstXC_SET_FPLL_MANUAL_SETTING_ENABLE pXCArgs = (pstXC_SET_FPLL_MANUAL_SETTING_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableFpllManualSetting(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableFpllManualSetting(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FPLL_BOUNDARY_TEST:
        {


            pstXC_SET_FPLL_BOUNDARY_TEST pXCArgs = (pstXC_SET_FPLL_BOUNDARY_TEST) pArgs;
            psXCInstPri->fpXC_FpllBoundaryTest(pInstance, pXCArgs->u32FpllSetOffset, pXCArgs->u16TestCnt);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OFFLINE_DETECTION:
        {


            pstXC_SET_OFFLINE_DETECTION pXCArgs = (pstXC_SET_OFFLINE_DETECTION) pArgs;
            psXCInstPri->fpXC_SetOffLineDetection(pInstance, pXCArgs->u8InputSourceType);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_OFFLINE_DETECTION:
        {


            pstXC_GET_OFFLINE_DETECTION pXCArgs = (pstXC_GET_OFFLINE_DETECTION) pArgs;
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_GetOffLineDetection(pInstance, pXCArgs->u8InputSourceType);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OFFLINE_SOG_THRESHOLD:
        {


            pstXC_SET_OFFLINE_SOG_THRESHOLD pXCArgs = (pstXC_SET_OFFLINE_SOG_THRESHOLD) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetOffLineSogThreshold(pInstance, pXCArgs->u8Threshold);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OFFLINE_SOG_BW:
        {


            pstXC_SET_OFFLINE_SOG_BW pXCArgs = (pstXC_SET_OFFLINE_SOG_BW) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetOffLineSogBW(pInstance, pXCArgs->u8BW);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OFFLINE_INIT:
        {


            pstXC_SET_OFFLINE_INIT pXCArgs = (pstXC_SET_OFFLINE_INIT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_OffLineInit(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_EXTRA_FETCH_ADV_LINE:
        {


            pstXC_SET_EXTRA_FETCH_ADV_LINE pXCArgs = (pstXC_SET_EXTRA_FETCH_ADV_LINE) pArgs;
            psXCInstPri->fpXC_Set_Extra_fetch_adv_line(pInstance, pXCArgs->u8val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_VGA_SOG_ENABLE:
        {


            pstXC_SET_VGA_SOG_ENABLE pXCArgs = (pstXC_SET_VGA_SOG_ENABLE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetVGASogEn(pInstance, pXCArgs->bVGASogEn);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_WINDOW_ENABLE:
        {


            pstXC_SET_WINDOW_ENABLE pXCArgs = (pstXC_SET_WINDOW_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(!MDrv_XC_GetMWEStatus(pInstance))
            {
                USE_OTHER_SC_AS_SUB_WINDOW();
            }
#endif
            psXCInstPri->fpXC_EnableWindow(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_SUBWINDOW_ENABLE:
        {


            pstXC_CHECK_SUBWINDOW_ENABLE pXCArgs = (pstXC_CHECK_SUBWINDOW_ENABLE) pArgs;
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Is_SubWindowEanble(pInstance);
#else
            if(!MDrv_XC_GetMWEStatus(pInstance))
            {
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Is_SubWindowEanble(g_pDevice1Instance);
            }
            else
            {
                pXCArgs->bReturnValue = psXCInstPri->fpXC_Is_SubWindowEanble(pInstance);
            }
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BORDER_FORMAT:
        {


            pstXC_SET_BORDER_FORMAT pXCArgs = (pstXC_SET_BORDER_FORMAT) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetBorderFormat(pInstance,
                                              pXCArgs->u8Left,
                                              pXCArgs->u8Right,
                                              pXCArgs->u8Up,
                                              pXCArgs->u8Down,
                                              pXCArgs->u8color,
                                              pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BORDER_ENABLE:
        {


            pstXC_SET_BORDER_ENABLE pXCArgs = (pstXC_SET_BORDER_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_EnableBorder(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_ZORDER_MAINWINDOW_FIRST:
        {
            pstXC_SET_ZORDER_MAINWINDOW_FIRST pXCArgs = (pstXC_SET_ZORDER_MAINWINDOW_FIRST) pArgs;
            psXCInstPri->fpXC_ZorderMainWindowFirst(pInstance, pXCArgs->bMainFirst);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_PQ_LOADFUNCTION:
        {


            pstXC_SET_PQ_LOADFUNCTION pXCArgs = (pstXC_SET_PQ_LOADFUNCTION) pArgs;
            psXCInstPri->fpXC_PQ_LoadFunction(pInstance, pXCArgs->function_Info, pXCArgs->u32InitDataLen);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_PQ_LoadFunction(g_pDevice1Instance, pXCArgs->function_Info, pXCArgs->u32InitDataLen);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_H_NONLINEAR_SCALING:
        {


            pstXC_CHECK_H_NONLINEAR_SCALING pXCArgs = (pstXC_CHECK_H_NONLINEAR_SCALING) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Check_HNonLinearScaling(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_EUROHDTV_SUPPORT_ENABLE:
        {


            pstXC_SET_EUROHDTV_SUPPORT_ENABLE pXCArgs = (pstXC_SET_EUROHDTV_SUPPORT_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableEuroHdtvSupport(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableEuroHdtvSupport(g_pDevice1Instance, pXCArgs->bEnable);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_EUROHDTV_DETECTION_ENABLE:
        {


            pstXC_SET_EUROHDTV_DETECTION_ENABLE pXCArgs = (pstXC_SET_EUROHDTV_DETECTION_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_EnableEuroHdtvDetection(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_READBYTE:
        {


            pstXC_READBYTE pXCArgs = (pstXC_READBYTE) pArgs;
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_ReadByte(pInstance, pXCArgs->u32Reg);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_WRITEBYTE:
        {


            pstXC_WRITEBYTE pXCArgs = (pstXC_WRITEBYTE) pArgs;
            psXCInstPri->fpXC_WriteByte(pInstance, pXCArgs->u32Reg, pXCArgs->u8Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_WRITEBYTEMSK:
        {

            pstXC_WRITEBYTEMSK pXCArgs = (pstXC_WRITEBYTEMSK) pArgs;
            psXCInstPri->fpXC_WriteByteMask(pInstance, pXCArgs->u32Reg, pXCArgs->u8Val, pXCArgs->u8Msk);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_WRITE2BYTEMSK:
        {


            pstXC_WRITE2BYTEMSK pXCArgs = (pstXC_WRITE2BYTEMSK) pArgs;
            psXCInstPri->fpXC_Write2ByteMask(pInstance, pXCArgs->u32Reg, pXCArgs->u16val, pXCArgs->u16Mask);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_W2BYTE:
        {


            pstXC_W2BYTE pXCArgs = (pstXC_W2BYTE) pArgs;
            psXCInstPri->fpXC_W2BYTE(pInstance, pXCArgs->u32Reg, pXCArgs->u16Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_R2BYTE:
        {


            pstXC_R2BYTE pXCArgs = (pstXC_R2BYTE) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_R2BYTE(pInstance, pXCArgs->u32Reg);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_W4BYTE:
        {


            pstXC_W4BYTE pXCArgs = (pstXC_W4BYTE) pArgs;
            psXCInstPri->fpXC_W4BYTE(pInstance, pXCArgs->u32Reg, pXCArgs->u32Val);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_R4BYTE:
        {


            pstXC_R4BYTE pXCArgs = (pstXC_R4BYTE) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_R4BYTE(pInstance, pXCArgs->u32Reg);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_R2BYTEMSK:
        {


            pstXC_R2BYTEMSK pXCArgs = (pstXC_R2BYTEMSK) pArgs;
            pXCArgs->u16ReturnValue = psXCInstPri->fpXC_R2BYTEMSK(pInstance, pXCArgs->u32Reg, pXCArgs->u16mask);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_W2BYTEMSK:
        {


            pstXC_W2BYTEMSK pXCArgs = (pstXC_W2BYTEMSK) pArgs;
            psXCInstPri->fpXC_W2BYTEMSK(pInstance, pXCArgs->u32Reg, pXCArgs->u16Val, pXCArgs->u16mask);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLOAD_INIT:
        {
            pstXC_MLOAD_INIT pXCArgs = (pstXC_MLOAD_INIT) pArgs;
            MS_U32 u32MloadBufLen = pXCArgs->u32BufByteLen;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            u32MloadBufLen = pXCArgs->u32BufByteLen/2;
#endif
            psXCInstPri->fpXC_MLoad_Init(pInstance, pXCArgs->PhyAddr, u32MloadBufLen);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_MLoad_Init(g_pDevice1Instance, pXCArgs->PhyAddr + u32MloadBufLen, u32MloadBufLen);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLOAD_ENABLE:
        {


            pstXC_MLOAD_ENABLE pXCArgs = (pstXC_MLOAD_ENABLE) pArgs;
            psXCInstPri->fpXC_MLoad_Enable(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_MLoad_Enable(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_MLOAD_CUS_INIT:
        {
            pstXC_MLOAD_CUS_INIT pXCArgs = (pstXC_MLOAD_CUS_INIT) pArgs;

            psXCInstPri->fpXC_MLoad_Cus_Init(pInstance, pXCArgs->eMloadSelect, pXCArgs->PhyAddr, pXCArgs->u32BufByteLen);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_MLOAD_CUS_ENABLE:
        {
            pstXC_MLOAD_CUS_ENABLE pXCArgs = (pstXC_MLOAD_CUS_ENABLE) pArgs;
            psXCInstPri->fpXC_MLoad_Cus_Enable(pInstance, pXCArgs->eMloadSelect, pXCArgs->bEnable);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_MLOAD_GETSTATUS:
        {


            pstXC_MLOAD_GETSTATUS pXCArgs = (pstXC_MLOAD_GETSTATUS) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_MLoad_GetStatus(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLOAD_WRITE_CMD_AND_FIRE:
        {


            pstXC_MLOAD_WRITE_CMD_AND_FIRE pXCArgs = (pstXC_MLOAD_WRITE_CMD_AND_FIRE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_MLoad_WriteCmd_And_Fire(pInstance, pXCArgs->u32Addr, pXCArgs->u16Data, pXCArgs->u16Mask);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLOAD_WRITE_CMDS_AND_FIRE:
        {


            pstXC_MLOAD_WRITE_CMDS_AND_FIRE pXCArgs = (pstXC_MLOAD_WRITE_CMDS_AND_FIRE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_MLoad_WriteCmds_And_Fire(pInstance, pXCArgs->pu32Addr, pXCArgs->pu16Data, pXCArgs->pu16Mask, pXCArgs->u16CmdCnt);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_MLOAD_COMBINE:
        {
            pstXC_MLOAD_COMBINE pXCArgs = (pstXC_MLOAD_COMBINE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_MLoad_Combine_Enable(pInstance, pXCArgs->bEn);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_MLG_INIT:
        {


            pstXC_MLG_INIT pXCArgs = (pstXC_MLG_INIT) pArgs;
            psXCInstPri->fpXC_MLG_Init(pInstance, pXCArgs->PhyAddr, pXCArgs->u32BufByteLen);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLG_ENABLE:
        {


            pstXC_MLG_ENABLE pXCArgs = (pstXC_MLG_ENABLE) pArgs;
            psXCInstPri->fpXC_MLG_Enable(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLG_GET_CAPS:
        {


            pstXC_MLG_GET_CAPS pXCArgs = (pstXC_MLG_GET_CAPS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_MLG_GetCaps(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_MLG_GETSTATUS:
        {


            pstXC_MLG_GETSTATUS pXCArgs = (pstXC_MLG_GETSTATUS) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_MLG_GetStatus(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OSD2VEMODE:
        {


            pstXC_SET_OSD2VEMODE pXCArgs = (pstXC_SET_OSD2VEMODE) pArgs;
            psXCInstPri->fpXC_SetOSD2VEMode(pInstance, pXCArgs->eVOPSelOSD_MUX);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_IP2_PREFILTER_ENABLE:
        {


            pstXC_SET_IP2_PREFILTER_ENABLE pXCArgs = (pstXC_SET_IP2_PREFILTER_ENABLE) pArgs;
            psXCInstPri->fpXC_IP2_PreFilter_Enable(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_IP2_PreFilter_Enable(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_PIXELRGB:
        {


            pstXC_GET_PIXELRGB pXCArgs = (pstXC_GET_PIXELRGB) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_Pixel_RGB(pInstance, pXCArgs->pData, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_KEEP_PIXELPOINTER_APPEAR:
        {


            pstXC_SET_KEEP_PIXELPOINTER_APPEAR pXCArgs = (pstXC_SET_KEEP_PIXELPOINTER_APPEAR) pArgs;
            psXCInstPri->fpXC_KeepPixelPointerAppear(pInstance, pXCArgs->bEnable);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_KeepPixelPointerAppear(g_pDevice1Instance, pXCArgs->bEnable);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MEMORY_FORMAT_EX:
        {


            pstXC_SET_MEMORY_FORMAT_EX pXCArgs = (pstXC_SET_MEMORY_FORMAT_EX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_MemFmtEx(pInstance, pXCArgs->eMemFmt, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS_MODE:
        {


            pstXC_CHECK_REQUEST_FRAMEBUFFERLESS_MODE pXCArgs = (pstXC_CHECK_REQUEST_FRAMEBUFFERLESS_MODE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsRequestFrameBufferLessMode(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SKIP_SWRESET:
        {


            pstXC_SET_SKIP_SWRESET pXCArgs = (pstXC_SET_SKIP_SWRESET) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SkipSWReset(pInstance, pXCArgs->bFlag);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SkipSWReset(g_pDevice1Instance, pXCArgs->bFlag);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_REPORTWINDOW_FOR_FRAMECOLOR:
        {


            pstXC_SET_REPORTWINDOW_FOR_FRAMECOLOR pXCArgs = (pstXC_SET_REPORTWINDOW_FOR_FRAMECOLOR) pArgs;
            psXCInstPri->fpXC_EnableRepWindowForFrameColor(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OSDLAYER:
        {


            pstXC_SET_OSDLAYER pXCArgs = (pstXC_SET_OSDLAYER) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOSDLayer(pInstance, pXCArgs->eVOPOSDLayer, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_OSDLAYER:
        {


            pstXC_GET_OSDLAYER pXCArgs = (pstXC_GET_OSDLAYER) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetOSDLayer(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_VIDEO_ALPHA:
        {


            pstXC_SET_VIDEO_ALPHA pXCArgs = (pstXC_SET_VIDEO_ALPHA) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetVideoAlpha(pInstance, pXCArgs->u8Val, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_VIDEO_ALPHA:
        {


            pstXC_GET_VIDEO_ALPHA pXCArgs = (pstXC_GET_VIDEO_ALPHA) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetVideoAlpha(pInstance, pXCArgs->pu8Val, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SKIP_WAIT_VSYNC:
        {


            pstXC_SKIP_WAIT_VSYNC pXCArgs = (pstXC_SKIP_WAIT_VSYNC) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SkipWaitVsync(pInstance, pXCArgs->eWindow, pXCArgs->bIsSkipWaitVsyn);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CMA_HEAP_ID:
        {


            pstXC_SET_CMA_HEAP_ID pXCArgs = (pstXC_SET_CMA_HEAP_ID) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetCMAHeapID(pInstance, pXCArgs->u8CMAHeapID, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OP2VOP_DE_SELECTION:
        {


            pstXC_SET_OP2VOP_DE_SELECTION pXCArgs = (pstXC_SET_OP2VOP_DE_SELECTION) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OP2VOPDESel(pInstance, pXCArgs->eVopDESel);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OP2VOPDESel(g_pDevice1Instance, pXCArgs->eVopDESel);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRC_WINDOW:
        {


            pstXC_SET_FRC_WINDOW pXCArgs = (pstXC_SET_FRC_WINDOW) pArgs;
            psXCInstPri->fpXC_FRC_SetWindow(pInstance, pXCArgs->e3dInputMode, pXCArgs->e3dOutputMode, pXCArgs->e3dPanelType);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_2INITFACTOR_ENABLE:
        {


            pstXC_SET_2INITFACTOR_ENABLE pXCArgs = (pstXC_SET_2INITFACTOR_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Enable_TwoInitFactor(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_CHECK_FIELDPACKINGMODE_SUPPORTED:
        {


            pstXC_CHECK_FIELDPACKINGMODE_SUPPORTED pXCArgs = (pstXC_CHECK_FIELDPACKINGMODE_SUPPORTED) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsFieldPackingModeSupported(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_PREINIT:
        {


            pstXC_PREINIT pXCArgs = (pstXC_PREINIT) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_PreInit(pInstance, pXCArgs->eType, pXCArgs->para, pXCArgs->u32Length);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_PreInit(g_pDevice1Instance, pXCArgs->eType, pXCArgs->para, pXCArgs->u32Length);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_BUFFERDATA:
        {


            pstXC_GET_BUFFERDATA pXCArgs = (pstXC_GET_BUFFERDATA) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Get_BufferData(pInstance, pXCArgs->eBufType, pXCArgs->pRect, pXCArgs->pRectBuf, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BUFFERDATA:
        {


            pstXC_SET_BUFFERDATA pXCArgs = (pstXC_SET_BUFFERDATA) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_BufferData(pInstance, pXCArgs->eBufType, pXCArgs->pDstRect, pXCArgs->pSrcBuf, pXCArgs->pSrcRect, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MAINWINDOW_ENABLE:
        {


            pstXC_SET_MAINWINDOW_ENABLE pXCArgs = (pstXC_SET_MAINWINDOW_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableMainWindow(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SUBWINDOW_ENABLE:
        {


            pstXC_SET_SUBWINDOW_ENABLE pXCArgs = (pstXC_SET_SUBWINDOW_ENABLE) pArgs;
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
            psXCInstPri->fpXC_EnableSubWindow(pInstance, pXCArgs->pstDispWin);
#else
            if(!MDrv_XC_GetMWEStatus(pInstance))
            {
                CHECK_DEVICE1_INSTANCE();
                psXCInstPri->fpXC_EnableSubWindow(g_pDevice1Instance, pXCArgs->pstDispWin);
            }
            else
            {
                psXCInstPri->fpXC_EnableSubWindow(pInstance, pXCArgs->pstDispWin);
            }
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SUBWINDOW_DISABLE:
        {


#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
            psXCInstPri->fpXC_DisableSubWindow(pInstance);
#else
            if(!MDrv_XC_GetMWEStatus(pInstance))
            {
                CHECK_DEVICE1_INSTANCE();
                psXCInstPri->fpXC_DisableSubWindow(g_pDevice1Instance);
            }
            else
            {
                psXCInstPri->fpXC_DisableSubWindow(pInstance);
            }
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_PIXEL_SHIFT:
        {


            pstXC_SET_PIXEL_SHIFT pXCArgs = (pstXC_SET_PIXEL_SHIFT) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetPixelShift(pInstance, pXCArgs->s8H, pXCArgs->s8V);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_VIDEO_ON_OSD:
        {
            pstXC_SET_VIDEO_ON_OSD pXCArgs = (pstXC_SET_VIDEO_ON_OSD) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetVideoOnOSD(pInstance, pXCArgs->enlayer, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_SET_OSD_LAYER_BLEANDING:
        {
            pstXC_SET_OSD_LAYER_BLENDING pXCArgs = (pstXC_SET_OSD_LAYER_BLENDING) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOSDLayerBlending(pInstance, pXCArgs->u8Layer, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_SET_OSD_LAYER_ALPHA:
        {
            pstXC_SET_OSD_LAYER_ALPHA pXCArgs = (pstXC_SET_OSD_LAYER_ALPHA) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOSDLayerAlpha(pInstance, pXCArgs->u8Layer, pXCArgs->u8Alpha);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_SET_OSD_BLENDING_FORMULA:
        {
            pstXC_SET_OSD_BLENDING_FORMULA pXCArgs = (pstXC_SET_OSD_BLENDING_FORMULA) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOSDBlendingFormula(pInstance, pXCArgs->enOsdIndex, pXCArgs->enType, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_REPORT_PIXEL_INFO:
        {


            pstXC_GET_REPORT_PIXEL_INFO pXCArgs = (pstXC_GET_REPORT_PIXEL_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_ReportPixelInfo(pInstance, pXCArgs->pstRepPixInfo);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_SCALING:
        {


            pstXC_SET_SCALING pXCArgs = (pstXC_SET_SCALING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetScaling(pInstance, pXCArgs->bEnable, pXCArgs->eScaling_type, pXCArgs->eVector_type, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MCDI_BUFFERADDRESS:
        {


            pstXC_SET_MCDI_BUFFERADDRESS pXCArgs = (pstXC_SET_MCDI_BUFFERADDRESS) pArgs;
            psXCInstPri->fpXC_SetMCDIBufferAddress(pInstance, pXCArgs->u32FBAddress, pXCArgs->u32FBSize, pXCArgs->eType);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_SetMCDIBufferAddress(g_pDevice1Instance, pXCArgs->u32FBAddress, pXCArgs->u32FBSize, pXCArgs->eType);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MCDI_ENABLE:
        {


            pstXC_SET_MCDI_ENABLE pXCArgs = (pstXC_SET_MCDI_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableMCDI(pInstance, pXCArgs->bEnable, pXCArgs->eType);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_EnableMCDI(g_pDevice1Instance, pXCArgs->bEnable, pXCArgs->eType);
#endif


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SEND_CMD2FRC:
        {


            pstXC_SEND_CMD2FRC pXCArgs = (pstXC_SEND_CMD2FRC) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SendCmdToFRC(pInstance, pXCArgs->u8Cmd, pXCArgs->count, pXCArgs->pFRC_R2_Para);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_MSG_FROM_FRC:
        {


            pstXC_GET_MSG_FROM_FRC pXCArgs = (pstXC_GET_MSG_FROM_FRC) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetMsgFromFRC(pInstance, pXCArgs->pu8Cmd, pXCArgs->pu8ParaCount, pXCArgs->pu8Para);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_AUTO_RWBANK_ENABLE:
        {


            pstXC_SET_AUTO_RWBANK_ENABLE pXCArgs = (pstXC_SET_AUTO_RWBANK_ENABLE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_EnableRWBankAuto(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_RWBANK_MAPPING_NUM:
        {


            pstXC_SET_RWBANK_MAPPING_NUM pXCArgs = (pstXC_SET_RWBANK_MAPPING_NUM) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetWRBankMappingNum(pInstance, pXCArgs->u8Val, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_RWBANK_MAPPING_NUM:
        {
            pstXC_GET_RWBANK_MAPPING_NUM pXCArgs = (pstXC_GET_RWBANK_MAPPING_NUM) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_GetWRBankMappingNum(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_RWBANK_MAPPING_NUM_FOR_ZAP:
        {
            pstXC_GET_RWBANK_MAPPING_NUM_FOR_ZAP pXCArgs = (pstXC_GET_RWBANK_MAPPING_NUM_FOR_ZAP) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_GetWRBankMappingNumForZap(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_BOBMODE:
        {
            pstXC_SET_BOBMODE pXCArgs = (pstXC_SET_BOBMODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetBOBMode(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_FORCE_READBANK:
        {
            pstXC_SET_FORCE_READBANK pXCArgs = (pstXC_SET_FORCE_READBANK) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetForceReadBank(pInstance, pXCArgs->bEnable,pXCArgs->u8Bank, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_LOCK_POINT:
        {
            pstXC_SET_LOCK_POINT pXCArgs = (pstXC_SET_LOCK_POINT) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetLockPoint(pInstance, pXCArgs->u16LockPoint, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_LD_INIT:
        {
            pstXC_LD_INIT pXCArgs = (pstXC_LD_INIT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_LD_Init(pInstance, pXCArgs->enLDPanelType);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_LD_MEMORYADDRESS:
        {
            pstXC_SET_LD_MEMORYADDRESS pXCArgs = (pstXC_SET_LD_MEMORYADDRESS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_LD_SetMemoryAddress(pInstance,
                                    pXCArgs->u8MIU,
                                    pXCArgs->u32LDFBase0,
                                    pXCArgs->u32LDFBase1,
                                    pXCArgs->u32LDBBase0,
                                    pXCArgs->u32LDBBase1,
                                    pXCArgs->u32EDGE2DBase,
                                    pXCArgs->u32LEDOffset);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_LD_VALUE:
        {
            pstXC_GET_LD_VALUE pXCArgs = (pstXC_GET_LD_VALUE) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_LD_Get_Value(pInstance,
                                    pXCArgs->pu8LDValue,
                                    pXCArgs->u8WSize,
                                    pXCArgs->u8HSize);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_LD_LEVEL:
        {


            pstXC_SET_LD_LEVEL pXCArgs = (pstXC_SET_LD_LEVEL) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_LD_SetLevel(pInstance, pXCArgs->enMode);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_LD_TURNOFF_LDBL:
        {


            pstXC_SET_LD_TURNOFF_LDBL pXCArgs = (pstXC_SET_LD_TURNOFF_LDBL) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSet_TurnoffLDBL(pInstance, pXCArgs->bTurnoffBL);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS:
        {


            pstXC_SET_NOTUPDATE_SPI_DATA_FLAGS pXCArgs = (pstXC_SET_NOTUPDATE_SPI_DATA_FLAGS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSet_notUpdateSPIDataFlags(pInstance, pXCArgs->bRefreshSPIData);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_LD_USERMODEFLAGS:
        {


            pstXC_SET_LD_USERMODEFLAGS pXCArgs = (pstXC_SET_LD_USERMODEFLAGS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSet_UsermodeLDFlags(pInstance, pXCArgs->bUserLDFlags);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BL_LEVEL:
        {


            pstXC_SET_BL_LEVEL pXCArgs = (pstXC_SET_BL_LEVEL) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpSet_BLLevel(pInstance, pXCArgs->u8BLLevel);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_BWS_MODE:
        {


            pstXC_SET_BWS_MODE pXCArgs = (pstXC_SET_BWS_MODE) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_Set_BWS_Mode(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRC_COLOR_PATH_CONTROL:
        {


            pstXC_SET_FRC_COLOR_PATH_CONTROL pXCArgs = (pstXC_SET_FRC_COLOR_PATH_CONTROL) pArgs;
            psXCInstPri->fpXC_FRC_ColorPathCtrl(pInstance, pXCArgs->u16Path_sel, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRC_OP2_RGBGAIN:
        {


            pstXC_SET_FRC_OP2_RGBGAIN pXCArgs = (pstXC_SET_FRC_OP2_RGBGAIN) pArgs;
            psXCInstPri->fpXC_FRC_OP2_SetRGBGain(pInstance, pXCArgs->u16RedGain, pXCArgs->u16GreenGain, pXCArgs->u16BlueGain);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRC_OP2_RGBOFFSET:
        {


            pstXC_SET_FRC_OP2_RGBOFFSET pXCArgs = (pstXC_SET_FRC_OP2_RGBOFFSET) pArgs;
            psXCInstPri->fpXC_FRC_OP2_SetRGBOffset(pInstance, pXCArgs->u16RedOffset, pXCArgs->u16GreenOffset, pXCArgs->u16BlueOffset);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FRC_OP2_DITHER:
        {


            pstXC_SET_FRC_OP2_DITHER pXCArgs = (pstXC_SET_FRC_OP2_DITHER) pArgs;
            psXCInstPri->fpXC_FRC_OP2_SetDither(pInstance, pXCArgs->u16dither);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_FRC_BYPASS:
        {


            pstXC_FRC_BYPASS pXCArgs = (pstXC_FRC_BYPASS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_FRC_BypassMFC(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_FRC_MUTE:
        {


            pstXC_FRC_MUTE pXCArgs = (pstXC_FRC_MUTE) pArgs;
            psXCInstPri->fpXC_FRC_Mute(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_FORCE_READ_FRAME:
        {


            pstXC_SET_FORCE_READ_FRAME pXCArgs = (pstXC_SET_FORCE_READ_FRAME) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_ForceReadFrame(pInstance, pXCArgs->bEnable, pXCArgs->u16FrameIndex);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_ForceReadFrame(g_pDevice1Instance, pXCArgs->bEnable, pXCArgs->u16FrameIndex);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_CSC:
        {


            pstXC_SET_CSC pXCArgs = (pstXC_SET_CSC) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_SetCsc(pInstance, pXCArgs->bEnable, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_REGISTER_PQ_FPLL_THRESH_MODE:
        {


            pstXC_SET_REGISTER_PQ_FPLL_THRESH_MODE pXCArgs = (pstXC_SET_REGISTER_PQ_FPLL_THRESH_MODE) pArgs;
            psXCInstPri->fpXC_RegisterPQSetFPLLThreshMode(pInstance, pXCArgs->fpPQCB);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_FREERUN_STATUS:
        {
            pstXC_GET_FREERUN_STATUS pXCArgs = (pstXC_GET_FREERUN_STATUS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetFreeRunStatus(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_BYPASS_CSC:
        {
            pstXC_SET_BYPASS_CSC pXCArgs = (pstXC_SET_BYPASS_CSC) pArgs;
            psXCInstPri->fpXC_BYPASS_SetCSC(pInstance, pXCArgs->bEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_DS_FORCE_INDEX_SUPPORTED:
        {
            pstXC_GET_DS_FORCE_INDEX_SUPPORTED pXCArgs = (pstXC_GET_DS_FORCE_INDEX_SUPPORTED) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_DSForceIndexSupported(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DS_FORCE_INDEX:
        {


            pstXC_SET_DS_FORCE_INDEX pXCArgs = (pstXC_SET_DS_FORCE_INDEX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_DSForceIndex(pInstance, pXCArgs->bEnable, pXCArgs->u8Index, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_DS_INDEX_SOURCE_SELECT:
        {
            pstXC_SET_DS_INDEX_SOURCE_SELECT pXCArgs = (pstXC_SET_DS_INDEX_SOURCE_SELECT) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_DSIndexSourceSelect(pInstance, pXCArgs->eDSIdxSrc, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_DS_STATUS:
        {
            pstXC_GET_DS_STATUS pXCArgs = (pstXC_GET_DS_STATUS) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_GetDynamicScalingStatus(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_OSDC_INITSETTING:
        {
            pstXC_OSDC_INITSETTING pXCArgs = (pstXC_OSDC_INITSETTING) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OSDC_InitSetting(pInstance, pXCArgs->e_osdctgen_type, pXCArgs->pstOC_Tgen_Cus, pXCArgs->pstOC_Ctrl);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_OSDC_SET_OUTPUTVFREQX10:
        {
            pstXC_OSDC_SET_OUTPUTVFREQX10 pXCArgs = (pstXC_OSDC_SET_OUTPUTVFREQX10) pArgs;
            psXCInstPri->fpXC_OSDC_SetOutVfreqx10(pInstance, pXCArgs->u16Vfreq);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_OSDC_CONTROL:
        {


            pstXC_OSDC_CONTROL pXCArgs = (pstXC_OSDC_CONTROL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OSDC_Control(pInstance, pXCArgs->eCtrl_type);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_OSDC_GET_DESTINATION_INFO:
        {
            pstXC_OSDC_GET_DESTINATION_INFO pXCArgs = (pstXC_OSDC_GET_DESTINATION_INFO) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_OSDC_GetDstInfo(pInstance, pXCArgs->pDstInfo, pXCArgs->u32SizeofDstInfo);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_POWERSTATE:
        {
            pstXC_SET_POWERSTATE pXCArgs = (pstXC_SET_POWERSTATE) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_Set_PowerState(pInstance, pXCArgs->enPowerState);

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            psXCInstPri->fpXC_Set_PowerState(g_pDevice1Instance, pXCArgs->enPowerState);
#endif

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_BYPASS_OSDVSYNC_POS:
        {
            pstXC_SET_BYPASS_OSDVSYNC_POS pXCArgs = (pstXC_SET_BYPASS_OSDVSYNC_POS) pArgs;
            psXCInstPri->fpXC_BYPASS_SetOSDVsyncPos(pInstance, pXCArgs->u16VsyncStart, pXCArgs->u16VsyncEnd);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_BYPASS_INPUTSRC:
        {
            pstXC_SET_BYPASS_INPUTSRC pXCArgs = (pstXC_SET_BYPASS_INPUTSRC) pArgs;
            psXCInstPri->fpXC_BYPASS_SetInputSrc(pInstance, pXCArgs->bEnable, pXCArgs->input);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_SEAMLESS_ZAPPING:
        {
            pstXC_SET_SEAMLESS_ZAPPING pXCArgs = (pstXC_SET_SEAMLESS_ZAPPING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetSeamlessZapping(pInstance, pXCArgs->eWindow, pXCArgs->bEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_SEAMLESS_ZAPPING_STATUS:
        {
            pstXC_GET_SEAMLESS_ZAPPING_STATUS pXCArgs = (pstXC_GET_SEAMLESS_ZAPPING_STATUS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetSeamlessZappingStatus(pInstance, pXCArgs->eWindow, pXCArgs->pbEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_VTRACK_PAYLOADDATA:
        {
            pstXC_SET_VTRACK_PAYLOADDATA pXCArgs = (pstXC_SET_VTRACK_PAYLOADDATA) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Vtrack_SetPayloadData(pInstance, pXCArgs->u16Timecode, pXCArgs->u8OperatorID);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_VTRACK_USERDEFINED_SETTING:
        {
            pstXC_SET_VTRACK_USERDEFINED_SETTING pXCArgs = (pstXC_SET_VTRACK_USERDEFINED_SETTING) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Vtrack_SetUserDefindedSetting(pInstance, pXCArgs->bUserDefinded, pXCArgs->pu8Setting);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_VTRACK_ENABLE:
        {
            pstXC_SET_VTRACK_ENABLE pXCArgs = (pstXC_SET_VTRACK_ENABLE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Vtrack_Enable(pInstance, pXCArgs->u8FrameRate, pXCArgs->bEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_PRESET_PQINFO:
        {
            pstXC_PRESET_PQINFO pXCArgs = (pstXC_PRESET_PQINFO) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fpXC_PreSetPQInfo(pInstance, pXCArgs->pstXC_SetWin_Info, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_GET_OP1_TESTPATTERN_ENABLED:
        {
            pstXC_GET_OP1_TESTPATTERN_ENABLED pXCArgs = (pstXC_GET_OP1_TESTPATTERN_ENABLED) pArgs;
            pXCArgs->bReturnValue= psXCInstPri->fpXC_Is_OP1_TestPattern_Enabled(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_SET_OP1_TESTPATTERN:
        {
            pstXC_SET_OP1_TESTPATTERN pXCArgs = (pstXC_SET_OP1_TESTPATTERN) pArgs;
            pXCArgs->eReturnValue= psXCInstPri->fpXC_Set_OP1_TestPattern(pInstance,pXCArgs->ePattern,pXCArgs->eMode);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_SET_OP2_TESTPATTERN:
        {
            pstXC_SET_OP2_TESTPATTERN pXCArgs = (pstXC_SET_OP2_TESTPATTERN) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_OP2_TestPattern(pInstance
                                    ,pXCArgs->bEnable
                                    ,pXCArgs->u16R_Data
                                    ,pXCArgs->u16G_Data
                                    ,pXCArgs->u16B_Data);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CHECK_WHITEBALANCE_PATTERN_MODE:
        {
            pstXC_CHECK_WB_TESTPATTERN pXCArgs = (pstXC_CHECK_WB_TESTPATTERN) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Check_WhiteBalance_Pattern_Mode(pInstance
                                    ,pXCArgs->eWBPattern
                                                                                     );

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_SET_HLINEARSCALING:
        {
            pstXC_SET_HLINEARSCALING pXCArgs = (pstXC_SET_HLINEARSCALING) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_HLinearScaling(pInstance
                                    ,pXCArgs->bEnable
                                    ,pXCArgs->bSign
                                    ,pXCArgs->u16Delta
                                    ,pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_SET_ENABLE_T3D:
        {
            pstXC_EnableT3D pXCArgs = (pstXC_EnableT3D) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_EnableT3D(pInstance
                                    ,pXCArgs->bEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_SET_FRC_INPUTTIMING:
        {
            pstXC_Set_FRC_InputTiming pXCArgs = (pstXC_Set_FRC_InputTiming) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_FRC_InputTiming(pInstance
                                    ,pXCArgs->enFRC_InputTiming);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_GET_FRC_INPUTTIMING:
        {
            pstXC_Get_FRC_InputTiming pXCArgs = (pstXC_Get_FRC_InputTiming) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_FRC_InputTiming(pInstance, pXCArgs->penFRC_InputTiming);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_GET_VBOX_INFO:
        {
            pstXC_Get_VBOX_Info pXCArgs = (pstXC_Get_VBOX_Info) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_VirtualBox_Info(pInstance, pXCArgs->pstXC_VboxInfo);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_SET_OSD_DETECT:
        {
            pstXC_Set_OSD_DETECT pXCArgs = (pstXC_Set_OSD_DETECT) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_OSD_DETECT(pInstance, pXCArgs->bEnable,pXCArgs->Threhold);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Set_OSD_DETECT(g_pDevice1Instance, pXCArgs->bEnable,pXCArgs->Threhold);
#endif
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_GET_OSD_DETECT:
        {
            pstXC_Get_OSD_DETECT pXCArgs = (pstXC_Get_OSD_DETECT) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_OSD_DETECT(pInstance, pXCArgs->pbOSD);
            /*
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            CHECK_DEVICE1_INSTANCE();
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Get_OSD_DETECT(g_pDevice1Instance, pXCArgs->pbOSD);
#endif
            */
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_ENABLE_LOCKFREQONLY:
        {
            pstXC_Enable_LockFreqOnly pXCArgs = (pstXC_Enable_LockFreqOnly) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_Enable_LockFreqOnly(pInstance, pXCArgs->bEnable);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_IS2K2KTOFRCMODE:
        {
            pstXC_Is2K2KToFrcMode pXCArgs = (pstXC_Is2K2KToFrcMode) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Is2K2KToFrcMode(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_SET_XC_VOP:
        {
            pstXC_SetXC_VOP pXCArgs = (pstXC_SetXC_VOP) pArgs;
            pXCArgs->bReturn = psXCInstPri->fpXC_SetXCVOPdata(pInstance, pXCArgs->stXC_VOPData);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

// OBSOLETE command **START**, please avoid using these
        case E_XC_CMD_GET_OUTPUT_VFREQ:
        {


            pstXC_SetXC_VOP pXCArgs = (pstXC_SetXC_VOP) pArgs;
            pXCArgs->bReturn = psXCInstPri->fpsc_get_output_vfreq(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_CURRENT_OUTPUTVFREQX100:
        {


            pstXC_GET_CURRENT_OUTPUTVFREQX100 pXCArgs = (pstXC_GET_CURRENT_OUTPUTVFREQX100) pArgs;
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_Get_Current_OutputVFreqX100(pInstance);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MEMORY_WRITE_REQUEST:
        {


            pstXC_SET_MEMORY_WRITE_REQUEST pXCArgs = (pstXC_SET_MEMORY_WRITE_REQUEST) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetMemoryWriteRequest(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MEMORY_FORMAT:
        {


            pstXC_SET_MEMORY_FORMAT pXCArgs = (pstXC_SET_MEMORY_FORMAT) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_MemFmt(pInstance, pXCArgs->eMemFmt);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_OUTPUT_ADJUST_SETTING:
        {


            pstXC_SET_OUTPUT_ADJUST_SETTING pXCArgs = (pstXC_SET_OUTPUT_ADJUST_SETTING) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetOutputAdjustSetting(pInstance, pXCArgs->stAdjSetting);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_FPLL_FSM:
        {


            pstXC_SET_FPLL_FSM pXCArgs = (pstXC_SET_FPLL_FSM) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            psXCInstPri->fsScaler_FPLL_FSM(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MIRROR_MODE_ENABLE:
        {


            pstXC_SET_MIRROR_MODE_ENABLE pXCArgs = (pstXC_SET_MIRROR_MODE_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableMirrorMode(pInstance, pXCArgs->bEnable);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_SET_MIRROR_MODE2_ENABLE:
        {


            pstXC_SET_MIRROR_MODE2_ENABLE pXCArgs = (pstXC_SET_MIRROR_MODE2_ENABLE) pArgs;
            psXCInstPri->fpXC_EnableMirrorMode2(pInstance, pXCArgs->eMirrorMode);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_SWDSIndex:
        {
            pstXC_GET_SWDSIndex pXCArgs = (pstXC_GET_SWDSIndex) pArgs;
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_GetSWDSIndex(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_IS_SupportSWDS:
        {
            pstXC_GET_Is_SupportSWDS pXCArgs = (pstXC_GET_Is_SupportSWDS) pArgs;
            pXCArgs->bEnable = psXCInstPri->fpXC_Get_Is_SupportSWDS(pInstance);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#ifdef UFO_XC_SET_DSINFO_V0
        case E_XC_CMD_SET_DSINFO:
        {
            pstXC_SET_DSINFO pXCArgs = (pstXC_SET_DSINFO) pArgs;
            psXCInstPri->fpXC_SetDSInfo(pInstance,pXCArgs->pstSetDSInfo, pXCArgs->u32DSInfoLen, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
        case E_XC_CMD_GET_FRCM_FRAMENUM:
        {

            pstXC_GET_FRCM_FRAMENUM pXCArgs = (pstXC_GET_FRCM_FRAMENUM) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->u8ReturnValue = psXCInstPri->fpXC_Get_FRCM_FrameNum(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_PIXEL_SHIFT_FEATURES:
        {
            pstXC_SET_PIXELSHIFT_FEATURES pXCArgs = (pstXC_SET_PIXELSHIFT_FEATURES) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetPixelShiftFeatures(pInstance,
                                    pXCArgs->u16HPixelShiftRange,
                                    pXCArgs->u16VPixelShiftRange,
                                    pXCArgs->ePixelShiftFeature);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_DUMP_SHM:
        {
            psXCInstPri->fpXC_dump_shm(pInstance);
            break;
        }
        case E_XC_CMD_SET_BWD_CONFIG:
        {
            pstXC_SetBwdConfig pXCArgs = (pstXC_SetBwdConfig)pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_SetBwdConfig(pInstance,pXCArgs->eType,pXCArgs->pstParam, pXCArgs->eWindow);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        case E_XC_CMD_ENABLE_DUAL_MODE:
        {
            pstXC_EnableDualMode pXCArgs = (pstXC_EnableDualMode)pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_enableDualMode(pInstance,pXCArgs->bEnable,pXCArgs->u32LeftFactor, pXCArgs->u32RightFactor,pXCArgs->eWindow);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif

        case E_XC_CMD_IS_SUPPORT_2_STEP_SCALING:
        {
            pstXC_IsSupport2StepScaling pXCArgs = (pstXC_IsSupport2StepScaling)pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_IsSupport2StepScaling(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_FORCE_WRITE:
        {


            pstXC_SET_FORCE_WRITE pXCArgs = (pstXC_SET_FORCE_WRITE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetForceWrite(pInstance, pXCArgs->enMuteType, pXCArgs->bIsForceWrite);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_GET_FORCE_WRITE:
        {
            pstXC_GET_FORCE_WRITE pXCArgs = (pstXC_GET_FORCE_WRITE) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetForceWrite(pInstance, pXCArgs->enMuteType);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_VIDEO_PLAYBACK_CTRL:
        {
            pstXC_VIDEO_PLAYBACK_CTRL pXCArgs = (pstXC_VIDEO_PLAYBACK_CTRL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_VideoPlaybackCtrl(pInstance,pXCArgs->u32Cmd,pXCArgs->pCmdArgs,pXCArgs->u32CmdArgsSize);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

#ifdef UFO_XC_PQ_PATH
            case E_XC_CMD_GET_PQ_PATH:
            {
                pstXC_GetPqPathStatus pXCArgs = (pstXC_GetPqPathStatus)pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpXC_GetPQPathStatus(pInstance, pXCArgs->ePqPathType, pXCArgs->u16Width, pXCArgs->u16Height);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
#endif

#ifdef UFO_XC_AUTO_DOWNLOAD
        case E_XC_CMD_AUTODOWNLOAD_CONFIG_CTRL:
        {
            pstXC_AUTODOWNLOAD_CONFIG_CTRL pXCArgs = (pstXC_AUTODOWNLOAD_CONFIG_CTRL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_AutoDownloadConfig(pInstance,pXCArgs->pstConfigInfo);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_AUTODOWNLOAD_WRITE_CTRL:
        {
            pstXC_AUTODOWNLOAD_WRITE_CTRL pXCArgs = (pstXC_AUTODOWNLOAD_WRITE_CTRL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_AutoDownloadWrite(pInstance,pXCArgs->pstDataInfo);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_AUTODOWNLOAD_FIRE_CTRL:
        {
            pstXC_AUTODOWNLOAD_FIRE_CTRL pXCArgs = (pstXC_AUTODOWNLOAD_FIRE_CTRL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_AutoDownloadFire(pInstance,pXCArgs->enClient);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
        case E_XC_CMD_HDR_CTRL:
        {
            pstXC_HDR_CTRL pXCArgs = (pstXC_HDR_CTRL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_HDRControl(pInstance,pXCArgs->enCtrlType,pXCArgs->pParam);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
#endif
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
        case E_XC_CMD_SET_BLACKVIDEO_BYMODE:
            {
                pstXC_SET_BLACKVIDEO_BYMODE pXCArgs = (pstXC_SET_BLACKVIDEO_BYMODE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpXC_GenerateBlackVideoByMode(pInstance, pXCArgs->bEnable, pXCArgs->eWindow, pXCArgs->eMode);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
#endif

        case E_XC_CMD_SWDS_FIRE:
            {
                pstXC_SWDS_FIRE pXCArgs = (pstXC_SWDS_FIRE) pArgs;
                psXCInstPri->fpXC_swds_fire(pInstance, pXCArgs->eWindow);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_CMD_OPTEE_CTL:
        {
            pstXC_OPTEE_CTRL pXCArgs = (pstXC_OPTEE_CTRL) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->xc_handler->eWindow == SUB_WINDOW)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
                pXCArgs->xc_handler->eWindow = MAIN_WINDOW;
            }
#endif
            pXCArgs->bReturn = psXCInstPri->fpXC_OPTEE_Ctrl(pInstance,pXCArgs->action, pXCArgs->xc_handler);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_OPTEE_CHECKRBASE:
        {
            pstXC_OPTEE_CheckRBase pXCArgs = (pstXC_OPTEE_CheckRBase) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->eWindow == SUB_WINDOW)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
                pXCArgs->eWindow = MAIN_WINDOW;
            }
#endif
            pXCArgs->bReturn = psXCInstPri->fpXC_OPTEE_CheckRBase(pInstance,pXCArgs->eWindow);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_OPTEE_MUX:
        {
            pstXC_OPTEE_MUX pXCArgs = (pstXC_OPTEE_MUX) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->xc_mux_data.eWindow == SUB_WINDOW)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
                pXCArgs->xc_mux_data.eWindow = MAIN_WINDOW;
            }
#endif
            pXCArgs->bReturn = psXCInstPri->fpXC_OPTEE_Mux(pInstance,pXCArgs->action, pXCArgs->xc_mux_data);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_GET_RES_BY_PIPE:
        {
            pstXC_GetResourceByPipeID pXCArgs = (pstXC_GetResourceByPipeID) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->u32PipeID == pipeID_SC1_main)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
            }
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_GetResourceByPipeID(pInstance,pXCArgs->u32PipeID,pXCArgs->U32RetResNum,pXCArgs->pResource);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_CONFIG_PIPE:
        {
            pstXC_ConfigPipe pXCArgs = (pstXC_ConfigPipe) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->u32PipeID == pipeID_SC1_main)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
            }
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_ConfigPipe(pInstance,pXCArgs->u32PipeID,pXCArgs->u32SecureDMA,pXCArgs->u32OperationMode);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_CHECK_PIPE:
        {
            pstXC_CheckPipe pXCArgs = (pstXC_CheckPipe) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(pXCArgs->u32PipeID == pipeID_SC1_main)
            {
                CHECK_DEVICE1_INSTANCE();
                pInstance = g_pDevice1Instance;
            }
#endif
            pXCArgs->u32ReturnValue = psXCInstPri->fpXC_CheckPipe(pInstance,pXCArgs->u32PipeID,pXCArgs->u32SecureDMA,pXCArgs->u32OperationMode);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#ifdef UFO_XC_FB_LEVEL
        case E_XC_CMD_SET_FB_LEVEL:
        {
            pstXC_SET_FB_LEVEL pXCArgs = (pstXC_SET_FB_LEVEL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_SetFBLevel(pInstance, pXCArgs->eFBLevel, pXCArgs->eWindow);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_GET_FB_LEVEL:
        {
            pstXC_GET_FB_LEVEL pXCArgs = (pstXC_GET_FB_LEVEL) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetFBLevel(pInstance, pXCArgs->eWindow);

            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
        case E_XC_CMD_GET_CONFIG:
        {
            pstXC_GET_CONFIG pXCArgs = (pstXC_GET_CONFIG) pArgs;
            pXCArgs->eReturnValue = psXCInstPri->fpXC_GetConfig(pInstance,pXCArgs->pXC_InitData);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }


        case E_XC_CMD_GET_FDMASK_STATUS:
        {


            pstXC_GET_FDMASK_STATUS pXCArgs = (pstXC_GET_FDMASK_STATUS) pArgs;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_FD_Mask_Status(pInstance, pXCArgs->eWindow);


            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_XC_CMD_SET_SWDR_INFO:
        {
            pstXC_SWDR_INFO pXCArgs = (pstXC_SWDR_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Set_SWDR_Info(pInstance,pXCArgs->pSWDR_INFO);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_XC_CMD_GET_SWDR_INFO:
        {
            pstXC_SWDR_INFO pXCArgs = (pstXC_SWDR_INFO) pArgs;
            pXCArgs->bReturnValue = psXCInstPri->fpXC_Get_SWDR_Info(pInstance,pXCArgs->pSWDR_INFO);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
        case E_XC_CMD_GET_HDMIPOLICY:
        {

            pstXC_GET_HDMIPOLICY pXCArgs = (pstXC_GET_HDMIPOLICY) pArgs;
            pXCArgs-> u32HDMIPolicyInfo = psXCInstPri->fpXC_GetHDMIPolicy(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
// OBSOLETE command **END**, please avoid using these
        case E_XC_CMD_NONE:
        case E_XC_CMD_MAX:
        default:
        {
            printf("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
            u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
            break;
        }
    }
    return u32Return;
}

MS_U32 XCClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}


#undef _XC_C_
