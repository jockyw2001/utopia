#if 1
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsTypes.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvHWI2C.h"
#include "drvHWI2C_v2.h"
#include "drvHWI2C_private.h"
#include "MsOS.h"
#endif



enum
{
    HWI2C_POOL_ID0=0
} eHwi2cPoolID;

#ifdef CONFIG_MSTAR_UTPA2K_WDT_ENABLE
extern MS_BOOL _MDrv_HWI2C_Init(HWI2C_UnitCfg *psCfg);
extern MS_BOOL _MDrv_HWI2C_WriteBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
extern MS_BOOL _MDrv_HWI2C_ReadBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
extern MS_BOOL _MDrv_HWI2C_Start(void);
extern MS_BOOL _MDrv_HWI2C_Stop(void);
extern MS_BOOL _MDrv_HWI2C_SelectPort(HWI2C_PORT ePort);
extern MS_BOOL _MDrv_HWI2C_SetClk(HWI2C_CLKSEL eClk);
//extern MS_BOOL _MDrv_HWI2C_WriteBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
//extern MS_BOOL _MDrv_HWI2C_ReadBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
extern MS_BOOL _MDrv_HWI2C_GetPortIndex(HWI2C_PORT ePort, MS_U8* pu8Port);
void* psI2cResource = NULL;
MS_U32 HWIICStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
    }
    return u32Return;// for success
}

// this func will be call to init by utopia20 framework
void HWI2CRegisterToUtopia(void)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    HWI2C_RESOURCE_PRIVATE * pResPri = NULL;
    UtopiaModuleCreate(MODULE_HWI2C, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)HWI2COpen, (FUtopiaClose)HWI2CClose, (FUtopiaIOctl)HWI2CIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)HWIICStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif	

    // 2. deal with resource
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
    UtopiaModuleAddResourceStart(pUtopiaModule, HWI2C_POOL_ID0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("hwi2c_0", sizeof(HWI2C_RESOURCE_PRIVATE), &psI2cResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psI2cResource, HWI2C_POOL_ID0);
    // end function to add res
	  UtopiaModuleAddResourceEnd(pUtopiaModule, HWI2C_POOL_ID0);
    if(UtopiaResourceObtain(pUtopiaModule, HWI2C_POOL_ID0, &psI2cResource) != 0)
    {
      printf("UtopiaResourceObtainToInstant fail\n");
    }
    UtopiaResourceGetPrivate(psI2cResource, (void**)&pResPri);
    UtopiaResourceRelease(psI2cResource);
}


//MS_U32 HWI2COpen(void** ppInstance, void* pAttribute)
MS_U32 HWI2COpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    HWI2C_INSTANT_PRIVATE *pHWI2CPri = NULL;
    printf("\n[HWI2C INFO] hwi2c open");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(HWI2C_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pHWI2CPri);
	
    #if 0	
    //pHWI2CPri->fpHWI2CInit = MDrv_HWI2C_Init;
    pHWI2CPri->fpHWI2CStart = MDrv_HWI2C_Start;
    pHWI2CPri->fpHWI2CStop = MDrv_HWI2C_Stop;
    //pHWI2CPri->fpHWI2CGetPortIndex = MDrv_HWI2C_GetPortIndex;
    pHWI2CPri->fpHWI2CSelectPort = MDrv_HWI2C_SelectPort;
    pHWI2CPri->fpHWI2CSetClk = MDrv_HWI2C_SetClk;
    //pHWI2CPri->fpHWI2CSetReadMode = MDrv_HWI2C_SetReadMode;
    //pHWI2CPri->fpHWI2CWriteByte = MDrv_HWI2C_WriteByte; 
    pHWI2CPri->fpHWI2CWriteBytes = MDrv_HWI2C_WriteBytes;
    //pHWI2CPri->fpHWI2CReadByte = MDrv_HWI2C_ReadByte;
    pHWI2CPri->fpHWI2CReadBytes = MDrv_HWI2C_ReadBytes;
    //pHWI2CPri->fpHWI2CSelectPort1 = MDrv_HWI2C_SelectPort1;
    //pHWI2CPri->fpHWI2CSetClkP1 = MDrv_HWI2C_SetClkP1;
    //pHWI2CPri->fpHWI2CSetReadModeP1 = MDrv_HWI2C_SetReadModeP1;
    //pHWI2CPri->fpHWI2CWriteByteP1 = MDrv_HWI2C_WriteByteP1;
    //pHWI2CPri->fpHWI2CWriteBytesP1 = MDrv_HWI2C_WriteBytesP1;
    //pHWI2CPri->fpHWI2CReadByteP1 = MDrv_HWI2C_ReadByteP1;
    //pHWI2CPri->fpHWI2CReadBytesP1 = MDrv_HWI2C_ReadBytesP1;
    //pHWI2CPri->fpHWI2CSetDbgLevel = MDrv_HWI2C_SetDbgLevel;
    //pHWI2CPri->fpHWI2CGetLibVer = MDrv_HWI2C_GetLibVer;
    //pHWI2CPri->fpHWI2CGetStatus = MDrv_HWI2C_GetStatus;
    //pHWI2CPri->fpHWI2CGetInfo = MDrv_HWI2C_GetInfo;
    //pHWI2CPri->fpHWI2CSetPowerState = MDrv_HWI2C_SetPowerState;
    //_MDrv_HWI2C_Init_((HWI2C_UnitCfg *) pAttribute);
    #else
    pHWI2CPri->fpHWI2CInit = MDrv_HWI2C_Init_U2K;
    pHWI2CPri->fpHWI2CStart = MDrv_HWI2C_Start_U2K;
    pHWI2CPri->fpHWI2CStop = MDrv_HWI2C_Stop_U2K;
    pHWI2CPri->fpHWI2CSelectPort = MDrv_HWI2C_SelectPort_U2K;
    pHWI2CPri->fpHWI2CSetClk = MDrv_HWI2C_SetClk_U2K;
    pHWI2CPri->fpHWI2CWriteBytes = MDrv_HWI2C_WriteBytes_U2K;
    pHWI2CPri->fpHWI2CReadBytes = MDrv_HWI2C_ReadBytes_U2K;
  //  pHWI2CPri->fpHWI2CWriteBytesP1 = MDrv_HWI2C_WriteBytesP1_U2K;
  //  pHWI2CPri->fpHWI2CReadBytesP1 = MDrv_HWI2C_ReadBytesP1_U2K;
    #endif
    
    return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 HWI2CIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    HWI2C_RESOURCE_PRIVATE * pResPri = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    MS_U32 u32Ret = 0;
    //MS_U32 u32SearchAddr;
    //MS_U32 u32CRC32;

    HWI2C_PRIVATE_PARAM *param = NULL;
    if (pArgs != NULL)
        param= (HWI2C_PRIVATE_PARAM *) pArgs;

    if (param == NULL)
        return UTOPIA_STATUS_ERR_RESOURCE;

    HWI2C_INSTANT_PRIVATE* psHWI2CInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psHWI2CInstPri);
    UtopiaResourceGetPrivate(psI2cResource, (void**)&pResPri);
    switch(u32Cmd)
    {	
        case MDrv_CMD_HWI2C_Init:
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            pResPri->privateInit.psCfg = param->privateInit.psCfg;
            u32Ret = psHWI2CInstPri->fpHWI2CInit(param->privateInit.psCfg);
            UtopiaResourceRelease(pResource);
            break;
	
        case MDrv_CMD_HWI2C_Start:
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CStart();
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_HWI2C_Stop:
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CStop();
            UtopiaResourceRelease(pResource);
            break;
        
        case MDrv_CMD_HWI2C_WriteBytes:
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	        return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret = psHWI2CInstPri->fpHWI2CWriteBytes(param->privateWriteBytes.u16SlaveCfg, 
                                                       param->privateWriteBytes.uAddrCnt,
                                                       param->privateWriteBytes.pRegAddr,
                                                       param->privateWriteBytes.uSize,
                                                       param->privateWriteBytes.pData);

	    param->privateWriteBytes.bReturn = (MS_BOOL) u32Ret;
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_HWI2C_ReadBytes:	
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CReadBytes(param->privateReadBytes.u16SlaveCfg, 
                                                      param->privateReadBytes.uAddrCnt, 
                                                      param->privateReadBytes.pRegAddr,
                                                      param->privateReadBytes.uSize,
                                                      param->privateReadBytes.pData);
			
	    param->privateReadBytes.bReturn = (MS_BOOL) u32Ret;	
            UtopiaResourceRelease(pResource);
            break;
	    
	case MDrv_CMD_HWI2C_SelectPort:
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CSelectPort(param->privateSelectPort.ePort);
            UtopiaResourceRelease(pResource);
            break;

        case MDrv_CMD_HWI2C_SetClk:	
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
	            return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CSetClk(param->privateSetClk.eClk); 
			
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_HWI2C_GetConfig:
            param->privateInit.psCfg = pResPri->privateInit.psCfg;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
	#if 0	  
        case MDrv_CMD_HWI2C_WriteBytesP1:	
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CWriteBytesP1(param->privateWriteBytesP1.u16SlaveCfg, 
                                                         param->privateWriteBytesP1.uAddrCnt,
                                                         param->privateWriteBytesP1.pRegAddr,
                                                         param->privateWriteBytesP1.uSize,
                                                         param->privateWriteBytesP1.pData);
			
            UtopiaResourceRelease(pResource);
            break;
		
         case MDrv_CMD_HWI2C_ReadBytesP1:	
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, HWI2C_POOL_ID0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            u32Ret = psHWI2CInstPri->fpHWI2CReadBytesP1(param->privateReadBytesP1.u16SlaveCfg, 
                                                        param->privateReadBytesP1.uAddrCnt,
                                                        param->privateReadBytesP1.pRegAddr,
                                                        param->privateReadBytesP1.uSize,
                                                        param->privateReadBytesP1.pData);

            UtopiaResourceRelease(pResource);
            break;
        #endif    
        default:
            break;
    };

    return (u32Ret==TRUE?UTOPIA_STATUS_SUCCESS:UTOPIA_STATUS_FAIL); // FIXME: error code
}

MS_U32 HWI2CClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);
    return UTOPIA_STATUS_SUCCESS;
}
#endif
