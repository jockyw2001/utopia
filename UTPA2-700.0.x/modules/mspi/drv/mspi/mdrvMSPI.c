////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrvMSPI.c
/// @brief  Master SPI Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvMSPI.h"
#include "drvMSPI_private.h"
#include "MsOS.h"

#include "ULog.h"

#define TAG_MSPI "MSPI"

enum
{
    MSPI_POOL_ID=0
} eMspiPoolID;

void MSPIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_MSPI, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)MSPIOpen, (FUtopiaClose)MSPIClose, (FUtopiaIOctl)MSPIIoctl);

    // 2. deal with resource
    void* psResource = NULL;
	UtopiaModuleAddResourceStart(pUtopiaModule, MSPI_POOL_ID);
    UtopiaResourceCreate("MSPI", sizeof(MSPI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, MSPI_POOL_ID);
	UtopiaModuleAddResourceEnd(pUtopiaModule, MSPI_POOL_ID);

}

MS_U32 MSPIOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    ULOGD(TAG_MSPI, "\n[MSPI INFO] master spi open \n");

    MSPI_INSTANT_PRIVATE *pMSPIPri = NULL;
    UtopiaInstanceCreate(sizeof(MSPI_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pMSPIPri);
#if (MSPI_UTOPIA20)
    pMSPIPri->fpMSPI_Init_Ext = _MDrv_MPSI_Init_Ext;
    pMSPIPri->fpMSPI_Init = _MDrv_MSPI_Init;
    pMSPIPri->fpMSPI_Read = _MDrv_MSPI_Read;
    pMSPIPri->fpMSPI_Write = _MDrv_MSPI_Write;
    pMSPIPri->fpMSPI_DCConfig = _MDrv_MSPI_DCConfig;
    pMSPIPri->fpMSPI_CLKConfig = _MDrv_MSPI_CLKConfig;
    pMSPIPri->fpMSPI_FRAMEConfig = _MDrv_MSPI_FRAMEConfig;
    pMSPIPri->fpMSPI_SlaveEnable = _MDrv_MSPI_SlaveEnable;
    pMSPIPri->fpMSPI_SetPowerState = _MDrv_MSPI_SetPowerState;
	pMSPIPri->fpMSPI_SetDbgLevel = _MDrv_MSPI_SetDbgLevel;
    pMSPIPri->fpMasterSPI_Init_Ext = _MDrv_MasterSPI_Init_Ext;
    pMSPIPri->fpMasterSPI_Init = _MDrv_MasterSPI_Init;
    pMSPIPri->fpMasterSPI_Read = _MDrv_MasterSPI_Read;
    pMSPIPri->fpMasterSPI_Write = _MDrv_MasterSPI_Write;
    pMSPIPri->fpMasterSPI_DCConfig = _MDrv_MasterSPI_DCConfig;
    pMSPIPri->fpMasterSPI_CLKConfig = _MDrv_MasterSPI_CLKConfig;
    pMSPIPri->fpMasterSPI_FrameConfig = _MDrv_MasterSPI_FRAMEConfig;
    pMSPIPri->fpMasterSPI_SlaveEnable = _MDrv_MasterSPI_SlaveEnable;
    pMSPIPri->fpMasterSPI_CsPadConfig = _MDrv_MasterSPI_CsPadConfig;
    pMSPIPri->fpMasterSPI_MaxClkConfig = _MDrv_MasterSPI_MaxClkConfig;
#else
    pMSPIPri->fpMSPI_Init_Ext = MDrv_MSPI_Init_Ext;
    pMSPIPri->fpMSPI_Init = MDrv_MSPI_Init;
    pMSPIPri->fpMSPI_Read = MDrv_MSPI_Read;
    pMSPIPri->fpMSPI_Write = MDrv_MSPI_Write;
    pMSPIPri->fpMSPI_DCConfig = MDrv_MSPI_DCConfig;
    pMSPIPri->fpMSPI_CLKConfig = MDrv_MSPI_CLKConfig;
    pMSPIPri->fpMSPI_FRAMEConfig = MDrv_MSPI_FRAMEConfig;
    pMSPIPri->fpMSPI_SlaveEnable = MDrv_MSPI_SlaveEnable;
    pMSPIPri->fpMSPI_SetPowerState = MDrv_MSPI_SetPowerState;
    pMSPIPri->fpMSPI_SetDbgLevel = MDrv_MSPI_SetDbgLevel;
    pMSPIPri->fpMasterSPI_Init = MDrv_MasterSPI_Init;
    pMSPIPri->fpMasterSPI_Read = MDrv_MasterSPI_Read;
    pMSPIPri->fpMasterSPI_Write = MDrv_MasterSPI_Write;
    pMSPIPri->fpMasterSPI_DCConfig = MDrv_MasterSPI_DCConfig;
    pMSPIPri->fpMasterSPI_CLKConfig = MDrv_MasterSPI_CLKConfig;
    pMSPIPri->fpMasterSPI_FrameConfig = MDrv_MasterSPI_FRAMEConfig;
    pMSPIPri->fpMasterSPI_SlaveEnable = MDrv_MasterSPI_SlaveEnable;
    pMSPIPri->fpMasterSPI_CsPadConfig = MDrv_MasterSPI_CsPadConfig;
    pMSPIPri->fpMasterSPI_MaxClkConfig = MDrv_MasterSPI_MaxClkConfig;
#endif

	return TRUE;
}

// FIXME: why static?
MS_U32 MSPIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    PMSPI_INIT_EXT pInitExtParam = NULL;
    PMSPI_INIT pInitParam = NULL;
    PMSPI_READ pReadParam = NULL;
    PMSPI_WRITE pWriteParam = NULL;
    PMSPI_DCCONFIG pDcconfigParam = NULL;
    PMSPI_CLKCONFIG pCLKConfigParam = NULL;
    PMSPI_FRAMECONFIG pFrameConfigParam = NULL;
    PMSPI_SLAVE_ENABLE pSlaveEnableParam = NULL;
    PMSPISETPOWERSTATE pSetPowerStateParam = NULL;
    PMSPI_SETDBGLEVEL pSetDbgLevelParam = NULL;
    PMASTERSPI_READ pMSPIReadParam = NULL;
    PMASTERSPI_WRITE pMSPIWriteParam = NULL;
    PMASTERSPI_DCCONFIG pMSPIDcconfigParam = NULL;
    PMASTERSPI_CLKCONFIG pMSPICLKConfigParam = NULL;
    PMASTERSPI_FRAMECONFIG pMSPIFrameConfigParam = NULL;
    PMASTERSPI_SLAVE_ENABLE pMSPISlaveEnableParam = NULL;
    PMASTERSPI_CSPADCONFIG pMSPISetCsPadConfigParam = NULL;
    PMASTERSPI_MAXCLKCONFIG pMSPIMaxClkConfigParam = NULL;

    MS_U32 u32Ret;
    MSPI_INSTANT_PRIVATE* psMSPIInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psMSPIInstPri);

    switch(u32Cmd)
    {
        case MDrv_CMD_MSPI_Init_Ext:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_Init_Ext\n");
            pInitExtParam = (PMSPI_INIT_EXT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Init_Ext(pInitExtParam->u8HWNum);
            return 0;
        case MDrv_CMD_MSPI_Init:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_Init\n");
            pInitParam = (PMSPI_INIT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
                    return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Init(pInitParam->ptMSPIConfig, pInitParam->u8HWNum);
            return 0;
        case MDrv_CMD_MSPI_Read:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_Read\n");
            pReadParam = (PMSPI_READ)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
                    return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Read(pReadParam->pu8Data, pReadParam->u16Size);
            return 0;
        case MDrv_CMD_MSPI_Write:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_Write\n");
            pWriteParam = (PMSPI_WRITE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_Write(pWriteParam->pu8Data, pWriteParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_DCConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_DCConfig\n");
            pDcconfigParam = (PMSPI_DCCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            psMSPIInstPri->fpMSPI_DCConfig(pDcconfigParam->ptDCConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_CLKConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_CLKConfig\n");
            pCLKConfigParam = (PMSPI_CLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_CLKConfig(pCLKConfigParam->ptCLKConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_FRAMEConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_FRAMEConfig\n");
            pFrameConfigParam = (PMSPI_FRAMECONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_FRAMEConfig(pFrameConfigParam->ptFrameConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_SlaveEnable:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_SlaveEnable\n");
            pSlaveEnableParam = (PMSPI_SLAVE_ENABLE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            psMSPIInstPri->fpMSPI_SlaveEnable(pSlaveEnableParam->bEnable);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_SetPowerState:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_SetPowerState\n");
            pSetPowerStateParam = (PMSPISETPOWERSTATE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
			u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_SetPowerState(pSetPowerStateParam->enPowerState);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MSPI_DbgLEvel:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MSPI_DbgLEvel\n");
            pSetDbgLevelParam = (PMSPI_SETDBGLEVEL)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMSPI_SetDbgLevel(pSetDbgLevelParam->u8DbgLevel);
            UtopiaResourceRelease(pResource);
            return 0;

        //===================================================================
        //              MSPI  NEW API of support slave device id
        //===================================================================
        case MDrv_CMD_MasterSPI_Init:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_Init\n");
            pInitParam = (PMSPI_INIT)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Init(pInitParam->ptMSPIConfig, pInitParam->u8HWNum);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_Read:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_Read\n");
            pMSPIReadParam = (PMASTERSPI_READ)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Read(pMSPIReadParam->u32DevID, pMSPIReadParam->pu8Data, pMSPIReadParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_Write:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_Write\n");
            pMSPIWriteParam = (PMASTERSPI_WRITE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_Write(pMSPIWriteParam->u32DevID, pMSPIWriteParam->pu8Data, pMSPIWriteParam->u16Size);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_DCConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_DCConfig\n");
            pMSPIDcconfigParam = (PMASTERSPI_DCCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_DCConfig(pMSPIDcconfigParam->u32DevID, pMSPIDcconfigParam->ptDCConfig);
            UtopiaResourceRelease(pResource);
            return 0;

        case MDrv_CMD_MasterSPI_CLKConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_CLKConfig\n");
            pMSPICLKConfigParam = (PMASTERSPI_CLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_CLKConfig(pMSPICLKConfigParam->u32DevID, pMSPICLKConfigParam->ptCLKConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_FRAMEConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_FRAMEConfig\n");
            pMSPIFrameConfigParam = (PMASTERSPI_FRAMECONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_FrameConfig(pMSPIFrameConfigParam->u32DevID, pMSPIFrameConfigParam->ptFrameConfig);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_SlaveEnable:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_SlaveEnable\n");
            pMSPISlaveEnableParam = (PMASTERSPI_SLAVE_ENABLE)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_SlaveEnable(pMSPISlaveEnableParam->u32DevID, pMSPISlaveEnableParam->bEnable);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_CSPadConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_CSPadConfig\n");
            pMSPISetCsPadConfigParam = (PMASTERSPI_CSPADCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_CsPadConfig(pMSPISetCsPadConfigParam->u32DevID, pMSPISetCsPadConfigParam->u32CsPad);
            UtopiaResourceRelease(pResource);
            return 0;
        case MDrv_CMD_MasterSPI_MaxClkConfig:
            ULOGD(TAG_MSPI, "MSPIIoctl - MDrv_CMD_MasterSPI_MaxClkConfig\n");
			pMSPIMaxClkConfigParam = (PMASTERSPI_MAXCLKCONFIG)pArgs;
            if(UtopiaResourceObtain(pModule, MSPI_POOL_ID, &pResource) != 0)
            {
                ULOGE(TAG_MSPI, "UtopiaResourceObtainToInstant fail\n");
	            return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psMSPIInstPri->fpMasterSPI_MaxClkConfig(pMSPIMaxClkConfigParam->u32DevID, pMSPIMaxClkConfigParam->u32MaxClk);
            UtopiaResourceRelease(pResource);
            return 0;
        default:
            break;
    };

	return 0; // FIXME: error code
}

MS_U32 MSPIClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}
