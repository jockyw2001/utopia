#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif


#include "MsTypes.h"
#include "MsVersion.h"
#include "MsDevice.h"
#include "drvMMIO.h"
#include "hal_IR_TX.h"
#include "drv_IR_TX.h"
#include "reg_IR_TX.h"
#include "utopia.h"
#include "MsOS.h"


#if (IRTX_UTOPIA20)

#include "drvIR_TX_private.h"

void* pInstantIRTX = NULL;
void* pAttributeIRTX = NULL;

MS_BOOL bU20IRTXOpened = FALSE;
MS_BOOL _MDrv_IR_TX_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	if (FALSE == bU20IRTXOpened)
	{
    #if defined(MSOS_TYPE_LINUX_KERNEL)
		if(UtopiaOpen(MODULE_IR_TX|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
    #else
        if(UtopiaOpen(MODULE_IR_TX, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
    #endif
		{
			return FALSE;
		}
		bU20IRTXOpened= TRUE;
	}
	return TRUE;
}
#endif

/****************************************************************/

MS_BOOL _MDrv_IR_TX_Trigger(void)
{
    MS_BOOL bRet = TRUE;

    bRet &= HAL_IR_TX_Trigger();

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}


MS_BOOL MDrv_IR_TX_Trigger(void)
{
#if (IRTX_UTOPIA20)

	PIRTX_TRIGGER_PARAM pParam = NULL;

	if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
		return 0;

    pParam = (PIRTX_TRIGGER_PARAM)malloc(sizeof(IRTX_TRIGGER_PARAM));


    UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_Trigger,(MS_U32*)pParam);

    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_Trigger();
#endif

}

/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetStatus(MS_U16 Status)
{
    MS_BOOL bRet = TRUE;

    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_STATUS, (MS_U16)Status);

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}


MS_BOOL MDrv_IR_TX_SetStatus(MS_U16 Status)
{
#if IRTX_UTOPIA20

    PIRTX_SETSTATUS_PARAM pParam = NULL;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;


    pParam = (PIRTX_SETSTATUS_PARAM)malloc(sizeof(IRTX_SETSTATUS_PARAM));

	pParam->Status = Status;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetStatus,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetStatus fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetStatus(Status);
#endif

}

/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetMemStatus(MS_U16 Status)
{
    MS_BOOL bRet = TRUE;

    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_MEM_REG_STATUS, (MS_U16)Status);

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetMemStatus(MS_U16 Status)
{
#if IRTX_UTOPIA20

    PIRTX_SETMEMSTATUS_PARAM pParam = NULL;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETMEMSTATUS_PARAM)malloc(sizeof(IRTX_SETMEMSTATUS_PARAM));
	pParam->Status = Status;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetMemStatus,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetMemStatus fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetMemStatus(Status);
#endif

}


/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetClkDiv(MS_U8 Div)
{
    MS_BOOL bRet = TRUE;
    //bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_ClkDiv, (MS_U16)(u16Div << 8));
    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_ClkDiv, (MS_U16)(Div << 8));

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetClkDiv(MS_U8 Div)
{
#if IRTX_UTOPIA20

    PIRTX_SETCLKDIV_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETCLKDIV_PARAM)malloc(sizeof(IRTX_SETCLKDIV_PARAM));
	pParam->Div = Div;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetClkDiv,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
       free(pParam);
       return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetClkDiv(Div);
#endif

}


/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetDelayCycleTime(MS_U16 CycleTime_H, MS_U16 CycleTime_L)
{
    MS_BOOL bRet = TRUE;
    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_CycleTime_H, (MS_U16)CycleTime_H);
    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_CycleTime_L, (MS_U16)CycleTime_L);

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetDelayCycleTime(MS_U16 CycleTime_H, MS_U16 CycleTime_L)
{
#if IRTX_UTOPIA20

	PIRTX_SETDELAYCLKTIME_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETDELAYCLKTIME_PARAM)malloc(sizeof(IRTX_SETDELAYCLKTIME_PARAM));

	pParam->CycleTime_H = CycleTime_H;
	pParam->CycleTime_L = CycleTime_L;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetDelayCycleTime,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetDelayCycleTime fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetDelayCycleTime(CycleTime_H, CycleTime_L);
#endif

}

/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetMemAddr(MS_U16 MemAddr)
{
    MS_BOOL bRet = TRUE;
        
            bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_MEM_REG_ADDR, (MS_U16)MemAddr);

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}


MS_BOOL MDrv_IR_TX_SetMemAddr(MS_U16 MemAddr)
{

#if IRTX_UTOPIA20

	PIRTX_SETMEMADDR_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETMEMADDR_PARAM)malloc(sizeof(IRTX_SETMEMADDR_PARAM));

	pParam->MemAddr = MemAddr;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetMemAddr,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetMemAddr fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetMemAddr(MemAddr);
#endif

}



/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetMemData(MS_U16 MemData)
{
    MS_BOOL bRet = TRUE;

    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_MEM_REG_DATA, (MS_U16) MemData);

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetMemData(MS_U16 MemData)
{

#if IRTX_UTOPIA20

	PIRTX_SETMEMDATA_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETMEMDATA_PARAM)malloc(sizeof(IRTX_SETMEMDATA_PARAM));

	pParam->MemData = MemData;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetMemData,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetMemData fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetMemData(MemData);
#endif

}


/****************************************************************/
MS_BOOL _MDrv_IR_TX_SetUnitValue(MS_U16 Unit_Value, MS_U8 Unit_Number)
{
    MS_BOOL bRet = TRUE;
    switch(Unit_Number)
    {
    	case 0:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)Unit_Value);
		    break;
    	case 1:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 2));
		    break;
	    case 2:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 4));
		    break;
	    case 3:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 6));
		    break;
	    case 4:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 8));
	    	break;
		case 5:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 10));
		    break;
    	case 6:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 12));
		    break;
	    case 7:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_H, (MS_U16)(Unit_Value << 14));
		    break;
	    case 8:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_L, (MS_U16)(Unit_Value));
		    break;
	    case 10:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_L, (MS_U16)(Unit_Value << 2));
	    	break;
    	case 11:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_L, (MS_U16)(Unit_Value << 4));
		    break;
    	case 12:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_L, (MS_U16)(Unit_Value << 6));
		    break;
	    case 13:
	        bRet &= HAL_IR_TX_WriteRegBit((MS_PHY)IR_TX_REG_Unit_Value_L, (MS_U16)(Unit_Value << 8));
		    break;

	    default:
		    printf("Wrong Unit Number\n");
            bRet = FALSE;
    }
#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetUnitValue(MS_U16 Unit_Value, MS_U8 Unit_Number)
{

#if IRTX_UTOPIA20

	PIRTX_SETUNITVALUE_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETUNITVALUE_PARAM)malloc(sizeof(IRTX_SETUNITVALUE_PARAM));

	pParam->Unit_Value = Unit_Value;
	pParam->Unit_Number = Unit_Number;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetUnitValue,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetUnitValue fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetUnitValue(Unit_Value, Unit_Number);
#endif

}

/****************************************************************/
MS_BOOL _MDrv_IR_TX_SetShotCount(MS_U16 H_ShotCount, MS_U16 L_ShotCount, MS_U8 Unit_Number)
{
    MS_BOOL bRet = TRUE;
    switch(Unit_Number)
    {
        case  0:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit00_ShotCount_H, (MS_U16)H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit00_ShotCount_L, (MS_U16)L_ShotCount);
		break;
	case  1:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit01_ShotCount_H, (MS_U16)H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit01_ShotCount_L, (MS_U16)L_ShotCount);
		break;
	case  2:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit02_ShotCount_H, (MS_U16)H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit02_ShotCount_L, (MS_U16)L_ShotCount);
		break;
	case  3:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit03_ShotCount_H, (MS_U16)H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit03_ShotCount_L, (MS_U16)L_ShotCount);
		break;
	case  4:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit04_ShotCount_H, (MS_U16)H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit04_ShotCount_L, (MS_U16)L_ShotCount);
		break;
	case  5:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit05_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit05_ShotCount_L, L_ShotCount);
		break;
	case  6:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit06_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit06_ShotCount_L, L_ShotCount);
		break;
	case  7:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit07_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit07_ShotCount_L, L_ShotCount);
		break;
	case  8:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit08_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit08_ShotCount_L, L_ShotCount);
		break;
	case  9:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit09_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit09_ShotCount_L, L_ShotCount);
		break;
	case 10:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit10_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit10_ShotCount_L, L_ShotCount);
		break;
	case 11:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit11_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit11_ShotCount_L, L_ShotCount);
		break;
	case 12:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit12_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit12_ShotCount_L, L_ShotCount);
		break;
	case 13:
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit13_ShotCount_H, H_ShotCount);
		bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_REG_Unit13_ShotCount_L, L_ShotCount);
		break;
	default:
		printf("Wrong Unit Number\n");
        bRet = FALSE;
    }
#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetShotCount(MS_U16 H_ShotCount, MS_U16 L_ShotCount, MS_U8 Unit_Number)
{

#if IRTX_UTOPIA20

	PIRTX_SETSHOTCOUNT_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETSHOTCOUNT_PARAM)malloc(sizeof(IRTX_SETSHOTCOUNT_PARAM));

	pParam->H_ShotCount = H_ShotCount;
	pParam->L_ShotCount = L_ShotCount;
	pParam->Unit_Number = Unit_Number;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetShotCount,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_IR_TX_SetShotCount fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetShotCount(H_ShotCount, L_ShotCount, Unit_Number);
#endif

}


/****************************************************************/

MS_BOOL _MDrv_IR_TX_Init(void)
{
    MS_BOOL bRet = TRUE;
    MS_VIRT virtBaseAddr = 0;

    MS_PHY u32BaseSize = 0;

    MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_IR_TX);

    HAL_IR_TX_SetIOMapBase(virtBaseAddr);

    bRet &= HAL_IR_TX_Reset();
    bRet &= HAL_IR_TX_SetIRTXMode();

#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}


MS_BOOL MDrv_IR_TX_Init(void)
{

#if IRTX_UTOPIA20

	PIRTX_INIT_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	{
       	printf("Open fail \n");
		return 0;
    }
    else
	{
		printf("Open success \n");
	}

    pParam = (PIRTX_INIT_PARAM)malloc(sizeof(IRTX_INIT_PARAM));

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_Init,(void*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_IR_TX_Init fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_Init();
#endif

}

/****************************************************************/

MS_BOOL _MDrv_IR_TX_SetCarrierCount(MS_U16 Count)
{
    MS_BOOL bRet = TRUE;
    bRet &= HAL_IR_TX_Write2Byte((MS_PHY)IR_TX_CARRIER_CNT,(MS_U16)Count);
#if IRTX_UTOPIA20
    return (!bRet);
#else
    return bRet;
#endif
}

MS_BOOL MDrv_IR_TX_SetCarrierCount(MS_U16 Count)
{

#if IRTX_UTOPIA20

	PIRTX_SETCARRIERCOUNT_PARAM pParam;

    if (FALSE == _MDrv_IR_TX_CheckUtopia20Open(&pInstantIRTX, 0, pAttributeIRTX))
	    return 0;

    pParam = (PIRTX_SETCARRIERCOUNT_PARAM)malloc(sizeof(IRTX_SETCARRIERCOUNT_PARAM));

	pParam->Count = Count;

    if(UtopiaIoctl(pInstantIRTX,MDrv_CMD_IR_TX_SetCarrierCount,(MS_U32*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_IR_TX_SetCarrierCount fail\n");
        free(pParam);
        return IRTX_FAIL;
    }
    free(pParam);
    return IRTX_OK;
#else
    return _MDrv_IR_TX_SetCarrierCount(Count);
#endif

}

