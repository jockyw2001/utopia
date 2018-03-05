#define _DAC_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDAC.h"
#include "apiDAC_private.h"
#include "apiDAC_v2.h"
#include "UFO.h"
#include "utopia_api_database.h"
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
// for semaphore POOL


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
// ========== Global definition ==========
void* pModuleDAC = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
void *pInstanceDACStr = NULL;
MS_U32 DACStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stDAC_POWER_MODE DACArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstanceDACStr == NULL)
            {
                if(UtopiaOpen(MODULE_DAC, &pInstanceDACStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen DAC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            DACArgs.ePowerState = u32PowerState;
            DACArgs.stReturnValue = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceDACStr, E_DAC_SET_POWER_STATE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain DAC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=DACArgs.stReturnValue;
            break;
        case E_POWER_RESUME:
            if (pInstanceDACStr == NULL)
            {
                if(UtopiaOpen(MODULE_DAC, &pInstanceDACStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen DAC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            DACArgs.ePowerState = u32PowerState;
            DACArgs.stReturnValue = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceDACStr, E_DAC_SET_POWER_STATE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain DAC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=DACArgs.stReturnValue;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif

// this func will be call to init by utopia20 framework
void DACRegisterToUtopia(void)
{
    MS_U32 u32ResourceStatusCheck[E_DAC_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    UtopiaModuleCreate(MODULE_DAC, 8, &pModuleDAC);
    UtopiaModuleRegister(pModuleDAC);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleDAC, (FUtopiaOpen)DACOpen, (FUtopiaClose)DACClose, (FUtopiaIOctl)DACIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleDAC, (FUtopiaSTR)DACStr);
    UtopiaModuleSetSTRPrivate(pModuleDAC, sizeof(DAC_REGS_SAVE_AREA));
#endif

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pModuleDAC, E_DAC_POOL_ID_INTERNAL_VARIABLE);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    u32ResourceStatusCheck[E_DAC_POOL_ID_INTERNAL_VARIABLE] = UtopiaResourceCreate("dac", sizeof(DAC_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pModuleDAC, psResource, E_DAC_POOL_ID_INTERNAL_VARIABLE);

	UtopiaModuleAddResourceEnd(pModuleDAC, E_DAC_POOL_ID_INTERNAL_VARIABLE);

    //4. init resource private members here   (aka, global variable)
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pDACResourcePrivate));

    //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
    if (u32ResourceStatusCheck[E_DAC_POOL_ID_INTERNAL_VARIABLE] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pDACResourcePrivate->bResourceRegistered = TRUE;
        memset(&(pDACResourcePrivate->stapiDAC),0,sizeof(pDACResourcePrivate->stapiDAC));
        memset(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.DACTable, 0, sizeof(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.DACTable));
        memset(&pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u8DACtype, 0, sizeof(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u8DACtype));
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_now = E_RES_720x480I_60Hz;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC0 = E_RES_720x480I_60Hz;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC1 = E_RES_720x480I_60Hz;
    }
}

MS_U32 DACOpen(void** ppInstance, const void* const pAttribute)
{
    DAC_INSTANT_PRIVATE *pDACPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(DAC_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pDACPri);

#ifdef CONFIG_UTOPIA_SIZE_CUSTOMER
    #define URELATION(a,b,c)   pDACPri->_URELATION_ASSIGN(a,b,c)
            _MODULE_DAC_U1_U2_RELATION
    #undef URELEATION
#else
    pDACPri->fpDACGetLibVer             = MApi_DAC_GetLibVer_U2;
    pDACPri->fpDACGetInfo               = MApi_DAC_GetInfo_U2;
    pDACPri->fpDACGetStatus             = MApi_DAC_GetStatus_U2;
    pDACPri->fpDACSetDbgLevel           = MApi_DAC_SetDbgLevel_U2;
    pDACPri->fpDACInit                  = MApi_DAC_Init_U2;
    pDACPri->fpDACEnable                = MApi_DAC_Enable_U2;
    pDACPri->fpDACSetClkInv             = MApi_DAC_SetClkInv_U2;
    pDACPri->fpDACSetYPbPrOutputTiming  = MApi_DAC_SetYPbPrOutputTiming_U2;
    pDACPri->fpDACSetOutputSource       = MApi_DAC_SetOutputSource_U2;
    pDACPri->fpDACSetOutputLevel        = MApi_DAC_SetOutputLevel_U2;
    pDACPri->fpDACSetOutputSwapSel      = MApi_DAC_SetOutputSwapSel_U2;
    pDACPri->fpDACOnOffSD               = MApi_DAC_OnOffSD_U2;
    pDACPri->fpDACGetSDStatus           = MApi_DAC_GetSDStatus_U2;
    pDACPri->fpDACOnOffHD               = MApi_DAC_OnOffHD_U2;
    pDACPri->fpDACGetHDStatus           = MApi_DAC_GetHDStatus_U2;
    pDACPri->fpDACClkSel                = MApi_DAC_ClkSel_U2;
    pDACPri->fpDACDumpTable             = MApi_DAC_DumpTable_U2;
    pDACPri->fpDACExit                  = MApi_DAC_Exit_U2;
    pDACPri->fpDACSetIHalfOutput        = MApi_DAC_SetIHalfOutput_U2;
    pDACPri->fpDACSetQuartOutput        = MApi_DAC_SetQuartOutput_U2;
    pDACPri->fpDACSetDacState           = MApi_DAC_SetDacState_U2;
    pDACPri->fpDACHotPlugDetect         = MApi_DAC_HotPlugDetect_U2;
    pDACPri->fpDACSetPowerState         = Mapi_DAC_SetPowerState_U2;
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
    pDACPri->fpDACGetOutputInterlaceTiming   = MApi_DAC_GetOutputInterlaceTiming_U2;
#endif
    pDACPri->fpDACSetWSSOnOff           = MApi_DAC_SetWSSOnOff_U2;
    pDACPri->fpDACGetWSSStatus          = MApi_DAC_GetWSSStatus_U2;
    pDACPri->fpDACResetWSSData          = MApi_DAC_ResetWSSData_U2;
    pDACPri->fpDACSetWSSOutput          = MApi_DAC_SetWSSOutput_U2;
    pDACPri->fpDACEnableICT             = MApi_DAC_EnableICT_U2;
    pDACPri->fpDACSetVGAHsyncVsync        = MApi_DAC_SetVGAHsyncVsync_U2;
#endif
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DACIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    if(pInstance == NULL)
    {
        printf("[%s] pInstance is NULL\n",__FUNCTION__);
        return UTOPIA_STATUS_FAIL;
    }
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    DAC_INSTANT_PRIVATE* psDACInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psDACInstPri);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    switch(u32Cmd)
    {
        case E_DAC_GET_LIBVER:
        {
            pstDAC_GET_LIBVER ptr = (pstDAC_GET_LIBVER)pArgs;
            ptr->eReturnValue = psDACInstPri->fpDACGetLibVer(pInstance, ptr->ppVersion);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_INFO:
        {
            pstDAC_GET_INFO ptr = (pstDAC_GET_INFO)pArgs;
            ptr->stReturnValue = psDACInstPri->fpDACGetInfo(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
        case E_DAC_GET_GETOUTPUTINTERLACETIMING:
        {
            pstDAC_GET_OUTPUTINTERLACETIMING ptr = (pstDAC_GET_OUTPUTINTERLACETIMING)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACGetOutputInterlaceTiming(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
#endif
        case E_DAC_GET_STATUS:
        {
            pstDAC_GET_STATUS ptr = (pstDAC_GET_STATUS)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACGetStatus(pInstance, ptr->pDacStatus);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_DBG_LEVEL:
        {
            pstDAC_SET_DBG_LEVEL ptr = (pstDAC_SET_DBG_LEVEL)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACSetDbgLevel(pInstance, ptr->u16DbgSwitch);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_INIT:
        {
            pstDAC_INIT ptr = (pstDAC_INIT)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACInit(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ENABLE:
        {
            pstDAC_ENABLE ptr = (pstDAC_ENABLE)pArgs;
            psDACInstPri->fpDACEnable(pInstance, ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_CLKINV:
        {
            pstDAC_SET_CLKINV ptr = (pstDAC_SET_CLKINV)pArgs;
            psDACInstPri->fpDACSetClkInv(pInstance, ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;

        }
        case E_DAC_SET_YPBPR_OUTPUTTIMING:
        {
            pstDAC_SET_YPBPR_OUTPUTTIMING ptr = (pstDAC_SET_YPBPR_OUTPUTTIMING)pArgs;
            psDACInstPri->fpDACSetYPbPrOutputTiming(pInstance, ptr->eTiming);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_SOURCE:
        {
            pstDAC_SET_OUTPUT_SOURCE ptr = (pstDAC_SET_OUTPUT_SOURCE)pArgs;
            psDACInstPri->fpDACSetOutputSource(pInstance, ptr->enOutputType,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_LEVEL:
        {
            pstDAC_SET_OUTPUT_LEVEL ptr = (pstDAC_SET_OUTPUT_LEVEL)pArgs;
            psDACInstPri->fpDACSetOutputLevel(pInstance, ptr->enLevel,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_OUTPUT_SWAPSEL:
        {
            pstDAC_SET_OUTPUT_SWAPSEL ptr = (pstDAC_SET_OUTPUT_SWAPSEL)pArgs;
            psDACInstPri->fpDACSetOutputSwapSel(pInstance, ptr->enSwap,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ONOFF_SD:
        {
            pstDAC_ONOFF_SD ptr = (pstDAC_ONOFF_SD)pArgs;
            psDACInstPri->fpDACOnOffSD(pInstance, ptr->enBit);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_SD_STATUS:
        {
            pstDAC_GET_SD_STATUS ptr = (pstDAC_GET_SD_STATUS)pArgs;
            ptr->enBit = psDACInstPri->fpDACGetSDStatus(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ONOFF_HD:
        {
            pstDAC_ONOFF_HD ptr = (pstDAC_ONOFF_HD)pArgs;
            psDACInstPri->fpDACOnOffHD(pInstance, ptr->enBit);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_HD_STATUS:
        {
            pstDAC_GET_HD_STATUS ptr = (pstDAC_GET_HD_STATUS)pArgs;
            ptr->enBit = psDACInstPri->fpDACGetHDStatus(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_CLKSEL:
        {
            pstDAC_CLKSEL ptr = (pstDAC_CLKSEL)pArgs;
            psDACInstPri->fpDACClkSel(pInstance, ptr->eTiming,ptr->ebits);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_DUMP_TABLE:
        {
            pstDAC_DUMP_TABLE ptr = (pstDAC_DUMP_TABLE)pArgs;
            psDACInstPri->fpDACDumpTable(pInstance, ptr->pDACTable,ptr->u8DACtype);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_EXIT:
        {
            psDACInstPri->fpDACExit(pInstance);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_IHALF_OUTPUT:
        {
            pstDAC_SET_IHALF_OUTPUT ptr = (pstDAC_SET_IHALF_OUTPUT)pArgs;
            psDACInstPri->fpDACSetIHalfOutput(pInstance, ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_QUART_OUTPUT:
        {
            pstDAC_SET_QUART_OUTPUT ptr = (pstDAC_SET_QUART_OUTPUT)pArgs;
            psDACInstPri->fpDACSetQuartOutput(pInstance, ptr->bEnable,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_DAC_STATE:
        {
            pstDAC_SET_DAC_STATE ptr = (pstDAC_SET_DAC_STATE)pArgs;
            psDACInstPri->fpDACSetDacState(pInstance, ptr->bEnabled,ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_HOTPLUG_DETECT:
        {
            pstDAC_HOTPLUG_DETECT ptr = (pstDAC_HOTPLUG_DETECT)pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACHotPlugDetect(pInstance, ptr->SelDAC,ptr->DetectType,ptr->State);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_POWER_STATE:
        {
            pstDAC_POWER_MODE ptr = (pstDAC_POWER_MODE) pArgs;
            ptr->stReturnValue = psDACInstPri->fpDACSetPowerState(pInstance, ptr->ePowerState);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_WSSONOFF:
        {
            pstDAC_SET_WSSONOFF ptr = (pstDAC_SET_WSSONOFF) pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACSetWSSOnOff(pInstance, ptr->bEnable, ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_GET_WSSSTATUS:
        {
            pstDAC_GET_WSSSTATUS ptr = (pstDAC_GET_WSSSTATUS) pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACGetWSSStatus(pInstance, ptr->bIsYPbPr, ptr->pEnable, ptr->pSetWSS_Data);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_RESET_WSSDATA:
        {
            pstDAC_RESET_WSSDATA ptr = (pstDAC_RESET_WSSDATA) pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACResetWSSData(pInstance, ptr->bIsYPbPr);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_WSSOUTPUT:
        {
            pstDAC_SET_WSSOUTPUT ptr = (pstDAC_SET_WSSOUTPUT) pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACSetWSSOutput(pInstance, ptr->bEnable, ptr->bIsYPbPr, ptr->SetWSS_Data);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_ENABLE_ICT:
        {
            pstDAC_ENABLE_ICT ptr = (pstDAC_ENABLE_ICT) pArgs;
            ptr->bReturnValue = psDACInstPri->fpDACEnableICT(pInstance, ptr->bEnable);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DAC_SET_VGA_HSYNCVSYNC:
        {
            pstDAC_SET_VGA_HSYNCVSYNC ptr = (pstDAC_SET_VGA_HSYNCVSYNC)pArgs;
            psDACInstPri->fpDACSetVGAHsyncVsync(pInstance, ptr->bEnable);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        default:
            printf("[%s] out of  DAC cmd:%td\n",__FUNCTION__,(ptrdiff_t)u32Cmd);
            break;
    };

    //printf("(%s) Done\n\n",__FUNCTION__);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DACClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


