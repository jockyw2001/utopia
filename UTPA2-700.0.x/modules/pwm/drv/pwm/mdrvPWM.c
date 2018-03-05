#include "MsTypes.h"
#include "string.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvPWM.h"
#include "drvPWM_private.h"
#include "MsOS.h"

#include "ULog.h"

#define TAG_PWM "PWM"

enum
{
    PWM_POOL_ID_PWM0=0
} ePWMPoolID;

void PWMRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_PWM, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)PWMOpen, (FUtopiaClose)PWMClose, (FUtopiaIOctl)PWMIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, PWM_POOL_ID_PWM0);
    UtopiaResourceCreate("PWM", sizeof(PWM_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, PWM_POOL_ID_PWM0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, PWM_POOL_ID_PWM0);

   // FLASHRegisterToUtopia only call once, so driver init code can put here,v
}

MS_U32 PWMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    PWM_INSTANT_PRIVATE *pPwmPri = NULL;
    ULOGD(TAG_PWM, "\n[PWM INFO] pwm open");    
    
    UtopiaInstanceCreate(sizeof(PWM_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pPwmPri);

    pPwmPri->fpPWMOen = MDrv_PWM_Oen;
    pPwmPri->fpPWMPeriod = MDrv_PWM_Period;
    pPwmPri->fpPWMDutyCycle = MDrv_PWM_DutyCycle;
    pPwmPri->fpPWMUnitDiv = MDrv_PWM_UnitDiv;
    pPwmPri->fpPWMDiv = MDrv_PWM_Div;
    pPwmPri->fpPWMPolarity = MDrv_PWM_Polarity;
    pPwmPri->fpPWMVdben = MDrv_PWM_Vdben;
    pPwmPri->fpPWMResetEn = MDrv_PWM_ResetEn;
    pPwmPri->fpPWMDben = MDrv_PWM_Dben;
    pPwmPri->fpPWMShift = MDrv_PWM_Shift;
    pPwmPri->fpPWMNvsync = MDrv_PWM_Nvsync;
    
    MDrv_PWM_Init(E_PWM_DBGLV_ALL);
    return TRUE;
}

// FIXME: why static?
MS_U32 PWMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    
    PPWM_OEN_PARAM pOenParam = NULL;
    PPWM_PERIOD_PARAM pPeriodParam = NULL;
    PPWM_DUTYCYCLE_PARAM pDutyCycleParam = NULL;
    PPWM_UNITDIV_PARAM pUnitDivParam = NULL;
    PPWM_DIV_PARAM pDivParam = NULL;
    PPWM_POLARITY_PARAM pPolarityParam = NULL;
    PPWM_VDBEN_PARAM pVdbenParam = NULL;
    PPWM_RESETEN_PARAM pResetEnParam = NULL;
    PPWM_DBEN_PARAM pDbenParam = NULL;
    PPWM_SHIFT_PARAM pShiftParam = NULL;
    PPWM_NVSYNC_PARAM pNvsyncParam = NULL;

    MS_U32 u32Ret;
    PWM_INSTANT_PRIVATE* psPWMInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPWMInstPri);

    switch(u32Cmd)
    {
        case MDrv_CMD_PWM_Oen:
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Oen\n");
            pOenParam = (PPWM_OEN_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWMInstPri->fpPWMOen(pOenParam->u8IndexPWM,pOenParam->bOenPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Period:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Period\n");
            pPeriodParam = (PPWM_PERIOD_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWMInstPri->fpPWMPeriod(pPeriodParam->u8IndexPWM,pPeriodParam->u32PeriodPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_DutyCycle:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_DutyCycle\n");
            pDutyCycleParam = (PPWM_DUTYCYCLE_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWMInstPri->fpPWMDutyCycle(pDutyCycleParam->u8IndexPWM,pDutyCycleParam->u32DutyPWM); 
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_UnitDiv:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_UnitDiv\n");
            pUnitDivParam = (PPWM_UNITDIV_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWMInstPri->fpPWMUnitDiv(pUnitDivParam->u16UnitDivPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Div:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Div\n");
            pDivParam = (PPWM_DIV_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMDiv(pDivParam->u8IndexPWM,pDivParam->u16DivPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Polarity:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Polarity\n");
            pPolarityParam = (PPWM_POLARITY_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMPolarity(pPolarityParam->u8IndexPWM,pPolarityParam->bPolPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Vdben:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Vdben\n");
            pVdbenParam = (PPWM_VDBEN_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMVdben(pVdbenParam->u8IndexPWM,pVdbenParam->bVdbenPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_ResetEn:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_ResetEn\n");
            pResetEnParam = (PPWM_RESETEN_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMResetEn(pResetEnParam->u8IndexPWM,pResetEnParam->bRstPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Dben:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Dben\n");
            pDbenParam = (PPWM_DBEN_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMDben(pDbenParam->u8IndexPWM,pDbenParam->bDbenPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Shift:	
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Shift\n");
            pShiftParam = (PPWM_SHIFT_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWMInstPri->fpPWMShift(pShiftParam->u8IndexPWM,pShiftParam->u32DutyPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_PWM_Nvsync:
            ULOGD(TAG_PWM, "PWMIoctl - MDrv_CMD_PWM_Nvsync\n");
            pNvsyncParam = (PPWM_NVSYNC_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, PWM_POOL_ID_PWM0, &pResource) != 0)
            {
                ULOGE(TAG_PWM, "UtopiaResourceObtainToInstant fail\n");
	        return 0xFFFFFFFF;
            }
            psPWMInstPri->fpPWMNvsync(pNvsyncParam->u8IndexPWM,pNvsyncParam->bNvsPWM);
            UtopiaResourceRelease(pResource);
            return 0;
        default:
            break;
    }

    return 0; // FIXME: error code

}

MS_U32 PWMClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}
