#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include <stdio.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "drvGPIO.h"
#include "drvGPIO_private.h"
#include "MsOS.h"
#include "utopia.h"


enum
{
    GPIO_POOL_ID_GPIO0=0
} eGPIOPoolID;


// this func will be call to init by utopia20 framework
void GPIORegisterToUtopia(FUtopiaOpen ModuleType)
{
     void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_GPIO, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)GPIOOpen, (FUtopiaClose)GPIOClose, (FUtopiaIOctl)GPIOIoctl);
    // 2. Resource register
    void* psResource = NULL;
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, GPIO_POOL_ID_GPIO0);
    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("gpio0", sizeof(GPIO_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, GPIO_POOL_ID_GPIO0);

    //UtopiaResourceCreate("cmdq1", sizeof(CMDQ_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, CMDQ_POOL_ID_CMDQ0);
    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, GPIO_POOL_ID_GPIO0);
}

MS_U32 GPIOOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	GPIO_INSTANT_PRIVATE *pGpioPri = NULL;
	void *pGpioPriVoid = NULL;

	//UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
	// instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
	UtopiaInstanceCreate(sizeof(GPIO_INSTANT_PRIVATE), ppInstance);
	// setup func in private and assign the calling func in func ptr in instance private
	UtopiaInstanceGetPrivate(*ppInstance, &pGpioPriVoid);
	pGpioPri= (GPIO_INSTANT_PRIVATE*)pGpioPriVoid;
	pGpioPri->fpGpioInit= (IOCTL_GPIO_INIT)mdrv_gpio_init_U2K;
	pGpioPri->fpGpioGetSts= (IOCTL_GPIO_GET_STS)MDrv_GPIO_GetStatus_U2K;
	pGpioPri->fpGpioSetH= (IOCTL_GPIO_SET_H)mdrv_gpio_set_high_U2K;
	pGpioPri->fpGpioSetL= (IOCTL_GPIO_SET_L)mdrv_gpio_set_low_U2K;
	pGpioPri->fpGpioSetInput= (IOCTL_GPIO_SET_INPUT)mdrv_gpio_set_input_U2K;
	pGpioPri->fpGpioGetInout= (IOCTL_GPIO_GET_INOUT)mdrv_gpio_get_inout_U2K;
	pGpioPri->fpGpioGetLevel= (IOCTL_GPIO_GET_LEVEL)mdrv_gpio_get_level_U2K;
	pGpioPri->fpGpioAttachInterrupt= (IOCTL_GPIO_ATTACH_INTERRUPT)mdrv_gpio_attach_interrupt_U2K;
	pGpioPri->fpGpioDetachInterrupt= (IOCTL_GPIO_DETACH_INTERRUPT)mdrv_gpio_detach_interrupt_U2K;
	pGpioPri->fpGpioDisableInterrupt = (IOCTL_GPIO_DISABLE_INTERRUPT)mdrv_gpio_disable_interrupt_U2K;
	pGpioPri->fpGpioEnableInterrupt = (IOCTL_GPIO_ENALBE_INTERRUPT)mdrv_gpio_enable_interrupt_U2K;
	pGpioPri->fpGpioDisableInterruptAll= (IOCTL_GPIO_DISABLE_INTERRUPT_ALL)mdrv_gpio_disable_interrupt_all_U2K;
	pGpioPri->fpGpioEnableInterruptAll = (IOCTL_GPIO_ENABLE_INTERRUPT_ALL)mdrv_gpio_enable_interrupt_all_U2K;
	pGpioPri->fpGpioInterruptAction = (IOCTL_GPIO_INTERRUPT_ACTION) mdrv_gpio_interrupt_action_U2K;
	return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 GPIOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	GPIO_INSTANT_PRIVATE* psGpioInstPri = NULL;
	MS_U32 u32Ret = TRUE;
	MS_U32 u32InOutVal;
	MS_U32 u32LevelVal;
	
	UtopiaInstanceGetModule(pInstance, &pModule);
	UtopiaInstanceGetPrivate(pInstance, (void**)&psGpioInstPri);
	GPIO_PRIVATE_PARAM *param = NULL;
	if (pArgs != NULL)
		param= (GPIO_PRIVATE_PARAM *) pArgs;

    switch(u32Cmd)
    {
        case MDrv_CMD_GPIO_Init:
            psGpioInstPri->fpGpioInit();
            break;
        case MDrv_CMD_GPIO_GetStauts:
            psGpioInstPri->fpGpioGetSts(param->privatGpioSts.pSts);
            break;
        case MDrv_CMD_GPIO_Set_High:
            psGpioInstPri->fpGpioSetH(param->privateGpioSetHigh.gpio_num);
            break;
        case MDrv_CMD_GPIO_Set_Low:
            psGpioInstPri->fpGpioSetL(param->privateGpioSetLow.gpio_num);
            break;
        case  MDrv_CMD_GPIO_Set_Input:
            psGpioInstPri->fpGpioSetInput(param->privateGpioInput.gpio_num);
            break;
        case MDrv_CMD_GPIO_Get_Inout:
		u32InOutVal = psGpioInstPri->fpGpioGetInout(param->privateGpioGetInout.gpio_num);
		param->privateGpioGetInout.u32InOut=u32InOutVal;
            break;
        case  MDrv_CMD_GPIO_Get_Level:
             u32LevelVal = psGpioInstPri->fpGpioGetLevel(param->privateGpioGetLevel.gpio_num);
		param->privateGpioGetLevel.u32Level=u32LevelVal;
            break;
        case MDrv_CMD_GPIO_Attach_Interrupt:
		u32Ret = psGpioInstPri->fpGpioAttachInterrupt(param->privateGpioAttachInterrupt.gpio_num,param->privateGpioAttachInterrupt.gpio_edge_type,param->privateGpioAttachInterrupt.GPIO_Callback);
            break;
        case MDrv_CMD_GPIO_Detach_Interrupt:
            u32Ret = psGpioInstPri->fpGpioDetachInterrupt(param->privateGpioDetachInterrupt.gpio_num);
            break;
        case  MDrv_CMD_GPIO_En_Interrupt:
	     u32Ret = psGpioInstPri->fpGpioEnableInterrupt(param->privateGpioEnableInterrupt.gpio_num);
        case MDrv_CMD_GPIO_Dis_Interrupt:
	     u32Ret = psGpioInstPri->fpGpioDisableInterrupt(param->privateGpioDisableInterrupt.gpio_num);
            break;
        case MDrv_CMD_GPIO_Dis_Interrupt_All:
	     psGpioInstPri->fpGpioDisableInterruptAll();
            break;
        case MDrv_CMD_GPIO_En_Interrupt_All:
	     psGpioInstPri->fpGpioEnableInterruptAll();
            break;
        case MDrv_CMD_GPIO_Interrupt_Action:
	     psGpioInstPri->fpGpioInterruptAction();
	     break;
        default:
            break;
    }
	return  (u32Ret==TRUE?UTOPIA_STATUS_SUCCESS:UTOPIA_STATUS_FAIL); // FIXME: error code
}

MS_U32 GPIOClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}


