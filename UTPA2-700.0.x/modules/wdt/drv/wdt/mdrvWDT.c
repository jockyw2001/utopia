#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include <stdio.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "drvWDT.h"
#include "drvWDT_private.h"
#include "MsOS.h"
#include "utopia.h"


enum
{
    WDT_POOL_ID_WDT0=0
} eWDTPoolID;

void* psWdtResource = NULL;
// this func will be call to init by utopia20 framework
void WDTRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;
    WDT_RESOURCE_PRIVATE * pResPri = NULL;
    UtopiaModuleCreate(MODULE_WDT, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)WDTOpen, (FUtopiaClose)WDTClose, (FUtopiaIOctl)WDTIoctl);

    // 2. Resource register
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, WDT_POOL_ID_WDT0);
    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("wdt0", sizeof(WDT_RESOURCE_PRIVATE), &psWdtResource);
    UtopiaResourceRegister(pUtopiaModule, psWdtResource, WDT_POOL_ID_WDT0);

    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, WDT_POOL_ID_WDT0);
    if(UtopiaResourceObtain(pUtopiaModule, WDT_POOL_ID_WDT0, &psWdtResource) != 0)
    {
      printf("UtopiaResourceObtainToInstant fail\n");
    }
    UtopiaResourceGetPrivate(psWdtResource, (void**)&pResPri);
    UtopiaResourceRelease(psWdtResource);
}

MS_U32 WDTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    WDT_INSTANT_PRIVATE *pWdtPri = NULL;
    void *pWdtPriVoid = NULL;
    //printf("\n[BDMA INFO] bdma open");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(WDT_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pWdtPriVoid);
    pWdtPri = (WDT_INSTANT_PRIVATE*)pWdtPriVoid;
    pWdtPri->fpWdtIsEn = (IOCTL_WDT_IS_ENABLE)MDrv_WDT_IsEnable_U2K;
    pWdtPri->fpWdtInit = (IOCTL_WDT_INIT)MDrv_WDT_Init_U2K;
    pWdtPri->fpWdtStop = (IOCTL_WDT_STOP)MDrv_WDT_Stop_U2K;
    pWdtPri->fpWdtClear = (IOCTL_WDT_CLEAR)MDrv_WDT_Clear_U2K;
    pWdtPri->fpWdtClrRstFlg = (IOCTL_WDT_CLEAR_RST_FLAG)MDrv_WDT_ClearRstFlag_U2K;
    pWdtPri->fpWdtIsRst = (IOCTL_WDT_IS_RST)MDrv_WDT_IsReset_U2K;
    pWdtPri->fpWdtSetTimer = (IOCTL_WDT_SET_TIMER)MDrv_WDT_SetTimer_U2K;
    pWdtPri->fpWdtSetMs = (IOCTL_WDT_SET_MS)MDrv_WDT_SetTimer_ms_U2K;
    pWdtPri->fpWdtSetUs = (IOCTL_WDT_SET_US)MDrv_WDT_SetTimer_us_U2K;
    pWdtPri->fpWdtSetIntTimer = (IOCTL_WDT_SET_INT_TIMER)MDrv_WDT_SetIntTimer_U2K;
    pWdtPri->fpTimerCount= (IOCTL_TIMER_COUNT)MDrv_TIMER_Count_U2K;
    pWdtPri->fpTimerInt = (IOCTL_TIMER_INT)MDrv_TIMER_Init_U2K;
    pWdtPri->fpTimerMaxMatch = (IOCTL_TIMER_MAX_MATCH)MDrv_TIMER_HitMaxMatch_U2K;
    pWdtPri->fpTimerRST = (IOCTL_TIMER_RST)MDrv_TIMER_Rst_U2K;
    pWdtPri->fpTimerSetMaxMatch = (IOCTL_TIMER_SET_MAX_MATCH)MDrv_TIMER_SetMaxMatch_U2K;
    pWdtPri->fpTimerGetMaxMatch = (IOCTL_TIMER_GET_MAX_MATCH)MDrv_TIMER_GetMaxMatch_U2K;
    pWdtPri->fpTimerGetCount = (IOCTL_TIMER_GET_COUNT)MDrv_TIMER_GetCounter_U2K;
    pWdtPri->fpTimerGetSec = (IOCTL_TIMER_GET_SEC)MDrv_TIMER_GetSecond_U2K;
    pWdtPri->fpTimerGetMs = (IOCTL_TIMER_GET_MS)MDrv_TIMER_GetMs_U2K;
    pWdtPri->fpTimerGetUs = (IOCTL_TIMER_GET_US)MDrv_TIMER_GetUs_U2K;
    pWdtPri->fpTimerDelay = (IOCTL_TIMER_DELAY)MDrv_TIMER_Delay_U2K;
    pWdtPri->fpTimerDelayMs = (IOCTL_TIMER_DELAY_MS)MDrv_TIMER_DelayMs_U2K;
    pWdtPri->fpTimerDelayUs = (IOCTL_TIMER_DELAY_US	)MDrv_TIMER_DelayUs_U2K;
    pWdtPri->fpTimerInitial = (IOCTL_TIMER_INITIAL	)MDrv_TIMER_Init_U2K;
    pWdtPri->fpTimerExit = (IOCTL_TIMER_EXIT	)MDrv_TIMER_Exit_U2K;
	return UTOPIA_STATUS_SUCCESS;
}


// FIXME: why static?
MS_U32 WDTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	WDT_RESOURCE_PRIVATE * pResPri = NULL;
	WDT_INSTANT_PRIVATE* psWdtInstPri = NULL;
	MS_U32 u32Ret = TRUE;
	MS_U32 u32MatchVal;
	MS_U32 u32CntVal;
	MS_U32 u32SecVal;
	MS_U32 u32MsecVal;
	MS_U32 u32UsecVal;
	WDT_Result statustmp;
	UtopiaInstanceGetModule(pInstance, &pModule);
	UtopiaInstanceGetPrivate(pInstance, (void**)&psWdtInstPri);
	UtopiaResourceGetPrivate(psWdtResource, (void**)&pResPri);
	WDT_PRIVATE_PARAM *param = NULL;
	if (pArgs != NULL)
		param= (WDT_PRIVATE_PARAM *) pArgs;

    switch(u32Cmd)
    {
        case MDrv_CMD_WDT_Init:
            pResPri->eLevel=param->privateWdtInit.eLevel;
            u32Ret = psWdtInstPri->fpWdtInit(param->privateWdtInit.eLevel);
            break;
        case MDrv_CMD_WDT_Stop:
            u32Ret = psWdtInstPri->fpWdtStop(param->privateWdtStop.eLevel);
            break;
        case MDrv_CMD_WDT_Clear:
            u32Ret = psWdtInstPri->fpWdtClear();
            break;
        case MDrv_CMD_WDT_ClearRstFlag:
            u32Ret = psWdtInstPri->fpWdtClrRstFlg();
            break;
        case  MDrv_CMD_WDT_IsReset:
             statustmp = psWdtInstPri->fpWdtIsRst();
             param->privateWdtIsRst.status=statustmp;
            break;
        case MDrv_CMD_WDT_IsEnable:
	       statustmp =  psWdtInstPri->fpWdtIsEn();
             param->privateWdtIsEn.status=statustmp;
            break;
        case MDrv_CMD_WDT_SetTimer:
             u32Ret = psWdtInstPri->fpWdtSetTimer(param->privateWdtSetTimer.eLevel,param->privateWdtSetTimer.sec);
            break;
        case MDrv_CMD_WDT_SetTimer_ms:
            u32Ret = psWdtInstPri->fpWdtSetMs(param->privateWdtSetMSec.eLevel,param->privateWdtSetMSec.msec);
            break;
        case MDrv_CMD_WDT_SetTimer_us:
            u32Ret = psWdtInstPri->fpWdtSetUs(param->privateWdtSetUSec.eLevel,param->privateWdtSetUSec.usec);
            break;
        case MDrv_CMD_WDT_SetIntTimer:
	     u32Ret = psWdtInstPri->fpWdtSetIntTimer(param->privateWdtSetSec.eLevel,param->privateWdtSetSec.sec);
        case MDrv_CMD_TIMER_Init:
	     psWdtInstPri->fpTimerInt(param->privateTimerInit.eTimer,param->privateTimerInit.bEnable);
            break;
        case MDrv_CMD_TIMER_Count:
	     psWdtInstPri->fpTimerCount(param->privateTimerCnt.eTimer,param->privateTimerCnt.bEnable);
            break;
        case MDrv_CMD_TIMER_Rst:
	     psWdtInstPri->fpTimerRST(param->privateTimerRst.eTimer);
            break;
        case MDrv_CMD_TIMER_SetMaxMatch:
	     psWdtInstPri->fpTimerSetMaxMatch(param->privateTimerSetMaxMatch.eTimer,param->privateTimerSetMaxMatch.u32MaxTimer);
	     break;
        case MDrv_CMD_TIMER_HitMaxMatch:
	     u32Ret =psWdtInstPri->fpTimerMaxMatch(param->privateTimerHitMaxMatch.eTimer);
            break;
        case MDrv_CMD_TIMER_GetMaxMatch:
	       u32MatchVal=psWdtInstPri->fpTimerGetMaxMatch(param->privateTimerGetMaxMatch.eTimer);
		param->privateTimerGetMaxMatch.u32MaxMatchVal=u32MatchVal;
		if (0xFFFFFFFF != u32MatchVal)
                u32Ret= TRUE;
		else
                u32Ret= FALSE;
            break;
        case MDrv_CMD_TIMER_GetCounter:
		u32CntVal=psWdtInstPri->fpTimerGetCount(param->privateTimerGetCunter.eTimer);
		param->privateTimerGetCunter.u32Cnt=u32CntVal;
		if (0xFFFFFFFF != u32CntVal)
                u32Ret= TRUE;
		else
                u32Ret= FALSE;
            break;
        case MDrv_CMD_TIMER_GetSecond:
		u32SecVal=psWdtInstPri->fpTimerGetSec(param->privateTimerGetSec.eTimer);
		param->privateTimerGetSec.u32Sec=u32SecVal;
		if (0xFFFFFFFF != u32SecVal)
                u32Ret= TRUE;
		else
                u32Ret= FALSE;
            break;
        case MDrv_CMD_TIMER_GetMs:
		u32MsecVal=psWdtInstPri->fpTimerGetMs(param->privateTimerGetMs.eTimer);
		param->privateTimerGetMs.u32Msec=u32MsecVal;
		if (0xFFFFFFFF != u32MsecVal)
                u32Ret= TRUE;
		else
                u32Ret= FALSE;
		break;
        case MDrv_CMD_TIMER_GetUs:
		u32UsecVal=psWdtInstPri->fpTimerGetUs(param->privateTimerGetUs.eTimer);
		param->privateTimerGetUs.u32Usec=u32UsecVal;
		if (0xFFFFFFFF != u32UsecVal)
                u32Ret= TRUE;
		else
                u32Ret= FALSE;
		break;
        case MDrv_CMD_TIMER_Delay:
		psWdtInstPri->fpTimerDelay(param->privateTimerDelay.eTimer,param->privateTimerDelay.u32Second);
            break;
       case MDrv_CMD_TIMER_DelayMs:
	   	psWdtInstPri->fpTimerDelayMs(param->privateTimerDelayMs.eTimer,param->privateTimerDelayMs.u32ms);
            break;
        case MDrv_CMD_TIMER_DelayUs:
		psWdtInstPri->fpTimerDelayUs(param->privateTimerDelayUs.eTimer,param->privateTimerDelayUs.u32us);
            break;
	 case MDrv_CMD_TIMER_Initial:
	 	psWdtInstPri->fpTimerInitial();
	     break;
	 case MDrv_CMD_TIMER_Exit:
	 	psWdtInstPri->fpTimerExit();
	     break;
	 case MDrv_CMD_WDT_GetConfig:
             *(WDT_DbgLv*)pArgs= pResPri->eLevel;
	     break;
        default:
            break;
    }
	return  (u32Ret==TRUE?UTOPIA_STATUS_SUCCESS:UTOPIA_STATUS_FAIL); // FIXME: error code
}

MS_U32 WDTClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}


