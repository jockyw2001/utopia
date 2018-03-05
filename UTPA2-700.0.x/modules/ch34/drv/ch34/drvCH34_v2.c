#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "drvCH34.h"
#include "drvCH34_private.h"
#include "drvCH34_v2.h"
#include "MsOS.h"
#include "utopia.h"

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL

#include <stdio.h>
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// this func will be call to init by utopia20 framework
void CH34RegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_CH34, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)CH34Open, (FUtopiaClose)CH34Close, (FUtopiaIOctl)CH34Ioctl);

//No globle veriable, no need resource?
#if 0
    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, GOPSCD_POOL);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("ch34", sizeof(GOPSCD_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, GOPSCD_POOL);

	UtopiaModuleAddResourceEnd(pUtopiaModule, GOPSCD_POOL);
#endif

}

MS_U32 CH34Open(void** ppInstance, const void* const pAttribute)
{
    CH34_INSTANT_PRIVATE *pCH34Pri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(CH34_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pCH34Pri);

    pCH34Pri->fpCH34Init            = (IOCTL_CH34_INIT)MDrv_Ch34_Init_U2;
    pCH34Pri->fpCH34SetMode         = (IOCTL_CH34_SET_MODE)MDrv_Ch34_SetMode_U2;
    pCH34Pri->fpCH34WriteTable      = (IOCTL_CH34_WRITE_TABLE)MDrv_Ch34_WriteCH34Table_U2;
    pCH34Pri->fpCH34TurnOnOff       = (IOCTL_CH34_TURN_ONOFF)MDrv_Ch34_TurnOnOff_U2;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 CH34Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    MS_U32 u32Ret = En_Drv_CH34_OK;
    CH34_INSTANT_PRIVATE* psCH34InstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psCH34InstPri);

    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    switch(u32Cmd)
    {
        case MAPI_CMD_CH34_INIT:
        {
            PCH34_INIT_INFO ptr = (PCH34_INIT_INFO)pArgs;
            ptr->bReturnValue = psCH34InstPri->fpCH34Init();
            break;
        }
        case MAPI_CMD_CH34_SETMODE:
        {
            PCH34_SET_MODE_INFO ptr = (PCH34_SET_MODE_INFO)pArgs;
            ptr->bReturnValue = psCH34InstPri->fpCH34SetMode(ptr->channel,ptr->videosys,ptr->audiosys);

            break;

        }
        case MAPI_CMD_CH34_WRITETABLE:
        {
            PCH34_WRITE_TABLE_INFO ptr = (PCH34_WRITE_TABLE_INFO)pArgs;
            ptr->bReturnValue = psCH34InstPri->fpCH34WriteTable(ptr->pTable);

            break;

        }
        case MAPI_CMD_CH34_TURNONOFF:
        {
            PCH34_TURN_ONOFF_INFO ptr = (PCH34_TURN_ONOFF_INFO)pArgs;
            ptr->bReturnValue = psCH34InstPri->fpCH34TurnOnOff(ptr->bOn);

            break;

        }
        default:
            break;
    };

    if(u32Ret != En_Drv_CH34_OK)
    {
        printf("[%s] ERROR on cmd:%d\n",__FUNCTION__,u32Cmd);
    }

    //printf("(%s) Done\n\n",__FUNCTION__);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 CH34Close(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


