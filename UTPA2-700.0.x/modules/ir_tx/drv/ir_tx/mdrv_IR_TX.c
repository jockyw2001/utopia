#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include <stdio.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsTypes.h"
#include "utopia_dapi.h"
#include "drv_IR_TX.h"
#include "drvIR_TX_private.h"
#include "MsOS.h"
#include "utopia.h"

#include "ULog.h"

#define TAG_IR_TX "IR_TX"

enum
{
    IRTX_POOL_ID_IRTX0=0
} eIR_TXPoolID;


void IR_TXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_IR_TX, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)IR_TXOpen, (FUtopiaClose)IR_TXClose, (FUtopiaIOctl)IR_TXIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    //UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)BDMAStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, IRTX_POOL_ID_IRTX0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("ir_tx", sizeof(IR_TX_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, IRTX_POOL_ID_IRTX0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, IRTX_POOL_ID_IRTX0);
}



MS_U32 IR_TXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    IR_TX_INSTANT_PRIVATE *pBdmaPri = NULL;

    UtopiaInstanceCreate(sizeof(IR_TX_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pBdmaPri);

    pBdmaPri->fpIRTXSetStatus         = (IOCTL_IRTX_SETSTATUS)_MDrv_IR_TX_SetStatus;
    pBdmaPri->fpIRTXSetMemStatus      = (IOCTL_IRTX_SETMEMSTATUS)_MDrv_IR_TX_SetMemStatus;
    pBdmaPri->fpIRTXSetClkDiv         = (IOCTL_IRTX_SETCLKDIV)_MDrv_IR_TX_SetClkDiv;
    pBdmaPri->fpIRTXSetDelayCycleTime = (IOCTL_IRTX_SETDELAYCLKTIME)_MDrv_IR_TX_SetDelayCycleTime;
    pBdmaPri->fpIRTXSetMemAddr        = (IOCTL_IRTX_SETMEMADDR)_MDrv_IR_TX_SetMemAddr;

    pBdmaPri->fpIRTXSetMemData        = (IOCTL_IRTX_SETMEMDATA)_MDrv_IR_TX_SetMemData;
    pBdmaPri->fpIRTXSetUnitValue      = (IOCTL_IRTX_SETUNITVALUE)_MDrv_IR_TX_SetUnitValue;
    pBdmaPri->fpIRTXSetShotCount      = (IOCTL_IRTX_SETSHOTCOUNT)_MDrv_IR_TX_SetShotCount;
    pBdmaPri->fpIRTX_Init             = (IOCTL_IRTX_INIT)_MDrv_IR_TX_Init;
    pBdmaPri->fpIRTX_Trigger          = (IOCTL_IRTX_TRIGGER)_MDrv_IR_TX_Trigger;
    pBdmaPri->fpIRTX_SetCarrierCount  = (IOCTL_IRTX_SETCARRIERCOUNT)_MDrv_IR_TX_SetCarrierCount;

	return 0;
}

MS_U32 IR_TXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	//void* pResource = NULL;

    //PIRTX_SETSTATUS_PARAM pSetStatusParam = NULL;
    //PIRTX_SETMEMSTATUS_PARAM pMemStatusParam = NULL;
    //PIRTX_SETCLKDIV_PARAM pSetClkDivParam = NULL;
    //PIRTX_SETDELAYCLKTIME_PARAM pSetDelayClkTimeParam = NULL;
    //PIRTX_SETMEMADDR_PARAM pSetMemAddrParam = NULL;
    //PIRTX_SETMEMDATA_PARAM pSetMemDataParam = NULL;
    //PIRTX_SETUNITVALUE_PARAM pSetUnitValueParam = NULL;
    //PIRTX_SETSHOTCOUNT_PARAM pShotCountParam = NULL;
    //PIRTX_INIT_PARAM pInit = NULL;
    //PIRTX_TRIGGER_PARAM pTriggerParam = NULL;
    //PIRTX_SETCARRIERCOUNT_PARAM pSetCarrierCountParam = NULL;

    IR_TX_INSTANT_PRIVATE* psIRTXInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psIRTXInstPri);

    MS_U32 u32Ret;

    switch(u32Cmd)
    {

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_Init:
        {
            //PIRTX_INIT_PARAM pParam = (PIRTX_INIT_PARAM)pArgs;
            u32Ret = psIRTXInstPri->fpIRTX_Init();
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetStatus:
		{

			PIRTX_SETSTATUS_PARAM pParam = (PIRTX_SETSTATUS_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetStatus(pParam->Status);
            return u32Ret;
		}

////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetMemStatus:
		{
			PIRTX_SETMEMSTATUS_PARAM pParam = (PIRTX_SETMEMSTATUS_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetMemStatus(pParam->Status);
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetClkDiv:
		{
			PIRTX_SETCLKDIV_PARAM pParam = (PIRTX_SETCLKDIV_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetClkDiv(pParam->Div);
            return u32Ret;
		}

////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetDelayCycleTime:
		{
			PIRTX_SETDELAYCLKTIME_PARAM pParam = (PIRTX_SETDELAYCLKTIME_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetDelayCycleTime(pParam->CycleTime_H,pParam->CycleTime_L);
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetMemAddr:
		{
			PIRTX_SETMEMADDR_PARAM pParam = (PIRTX_SETMEMADDR_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetMemAddr(pParam->MemAddr);
            return u32Ret;
		}

////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetMemData:
		{
			PIRTX_SETMEMDATA_PARAM pParam = (PIRTX_SETMEMDATA_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetMemData(pParam->MemData);
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetUnitValue:
		{
			PIRTX_SETUNITVALUE_PARAM pParam = (PIRTX_SETUNITVALUE_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetUnitValue(pParam->Unit_Value,pParam->Unit_Number);
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetShotCount:
		{
			PIRTX_SETSHOTCOUNT_PARAM pParam = (PIRTX_SETSHOTCOUNT_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTXSetShotCount(pParam->H_ShotCount, pParam->L_ShotCount, pParam->Unit_Number);
            return u32Ret;
		}

/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_Trigger:
		{
			//PIRTX_TRIGGER_PARAM pParam = (PIRTX_TRIGGER_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTX_Trigger();
            return u32Ret;
		}
/////////////////////////////////////////////////////////////////

		case MDrv_CMD_IR_TX_SetCarrierCount:
		{
			PIRTX_SETCARRIERCOUNT_PARAM pParam = (PIRTX_SETCARRIERCOUNT_PARAM)pArgs;
			u32Ret = psIRTXInstPri->fpIRTX_SetCarrierCount(pParam->Count);
            return u32Ret;
		}
	}//end switch

	return 1; // FIXME: error code
}


MS_U32 IR_TXClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

