#include "utopia_private.h"
#include <stdio.h>
#include "utopia.h"
#include "utopia_module.h"
#include "utopia_dapi.h"
#include "string.h"
#include "MsOS.h"
#include "MsCommon.h"
#include "utopia_driver_id.h"
#include "MsTypes.h"

//Global Variable===================================================
UTOPIA_PRIVATE* psUtopiaPrivate;

char moduleNames[][40] = {
#define PREFIX(MODULE) "MODULE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char rpoolNames[][40] = {
#define PREFIX(MODULE) "RPOOL_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char ResourceNames[][40] = {
#define PREFIX(MODULE) "RESOURCE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

// NOS use CONFIG_MBOOT/CONFIG_CHAKRA2/CONFIG_PUMABOOT to define the XXXXRegisterToUtopia

void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
void SYSRegisterToUtopia(FUtopiaOpen ModuleType);

#ifdef CONFIG_MBOOT
void CPURegisterToUtopia(FUtopiaOpen ModuleType);
void PNLRegisterToUtopia(FUtopiaOpen ModuleType);
void GOPRegisterToUtopia(FUtopiaOpen ModuleType);
void GFXRegisterToUtopia(FUtopiaOpen ModuleType);
void MIURegisterToUtopia(FUtopiaOpen ModuleType);
void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
void HDMITXRegisterToUtopia(FUtopiaOpen ModuleType);
void CECRegisterToUtopia(FUtopiaOpen ModuleType);
void DSCMBRegisterToUtopia(void);
void CIPHERRegisterToUtopia(void);
#ifdef CONFIG_API_JPEG_EX
void JPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
#ifdef CONFIG_API_NJPEG_EX
void NJPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
void XCRegisterToUtopia(FUtopiaOpen ModuleType);
#ifdef CONFIG_API_DAC
void DACRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
#ifdef CONFIG_API_LOCALDIMM
void LDRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
#endif

#ifdef CONFIG_CHAKRA2
void PNLRegisterToUtopia(FUtopiaOpen ModuleType);
void GOPRegisterToUtopia(FUtopiaOpen ModuleType);
void GFXRegisterToUtopia(FUtopiaOpen ModuleType);
void NJPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
void XCRegisterToUtopia(FUtopiaOpen ModuleType);
void DMXRegisterToUtopia(FUtopiaOpen ModuleType);
void VDEC_EXRegisterToUtopia(FUtopiaOpen ModuleType);
void ACERegisterToUtopia(FUtopiaOpen ModuleType);
void DBCRegisterToUtopia(FUtopiaOpen ModuleType);
void DLCRegisterToUtopia(FUtopiaOpen ModuleType);
void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
void CPURegisterToUtopia(FUtopiaOpen ModuleType);
void IRRegisterToUtopia(FUtopiaOpen ModuleType);
void MVOPRegisterToUtopia(FUtopiaOpen ModuleType);
void PWMRegisterToUtopia(FUtopiaOpen ModuleType);
void PWSRegisterToUtopia(FUtopiaOpen ModuleType);
void SARRegisterToUtopia(FUtopiaOpen ModuleType);
void SEMRegisterToUtopia(FUtopiaOpen ModuleType);
void FLASHRegisterToUtopia(FUtopiaOpen ModuleType);
void SYSRegisterToUtopia(FUtopiaOpen ModuleType);
void UARTRegisterToUtopia(FUtopiaOpen ModuleType);
void VBIRegisterToUtopia(FUtopiaOpen ModuleType);
void MBXRegisterToUtopia(FUtopiaOpen ModuleType);
void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType);
void DIPRegisterToUtopia(FUtopiaOpen ModuleType);
#endif

#ifdef CONFIG_PUMABOOT
void PNLRegisterToUtopia(FUtopiaOpen ModuleType);
void GOPRegisterToUtopia(FUtopiaOpen ModuleType);
void GFXRegisterToUtopia(FUtopiaOpen ModuleType);
void NJPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
void XCRegisterToUtopia(FUtopiaOpen ModuleType);
void DMXRegisterToUtopia(FUtopiaOpen ModuleType);
void VDEC_EXRegisterToUtopia(FUtopiaOpen ModuleType);
void ACERegisterToUtopia(FUtopiaOpen ModuleType);
void DBCRegisterToUtopia(FUtopiaOpen ModuleType);
void DLCRegisterToUtopia(FUtopiaOpen ModuleType);
void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
void MVOPRegisterToUtopia(FUtopiaOpen ModuleType);
void PWMRegisterToUtopia(FUtopiaOpen ModuleType);
void SEMRegisterToUtopia(FUtopiaOpen ModuleType);
void SYSRegisterToUtopia(FUtopiaOpen ModuleType);
void UARTRegisterToUtopia(FUtopiaOpen ModuleType);
void VBIRegisterToUtopia(FUtopiaOpen ModuleType);
void MBXRegisterToUtopia(FUtopiaOpen ModuleType);
void MIURegisterToUtopia(FUtopiaOpen ModuleType);
#endif

MS_U32 UtopiaInit()
{
    MsOS_SHM_Init();

    psUtopiaPrivate = (UTOPIA_PRIVATE*)malloc(sizeof(UTOPIA_PRIVATE));
    memset(psUtopiaPrivate, 0, sizeof(UTOPIA_PRIVATE));
    psUtopiaPrivate->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO, "UtopiaPrivateMutex", MSOS_PROCESS_SHARED);

    MsOS_ObtainMutex(psUtopiaPrivate->u32MutexID, MSOS_WAIT_FOREVER);

    /*
     * to avoid weak attribute malfunction in static lib,
     * we explicitly call needed functions
     */
    UTOPIARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UTOPIA_FULL);
    BDMARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_BDMA_FULL);
	SYSRegisterToUtopia(NULL);

#ifdef CONFIG_PUMABOOT
    VBIRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VBI_FULL);
    XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_FULL);
    PNLRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PNL_FULL);
    ACERegisterToUtopia((FUtopiaOpen)MODULE_TYPE_ACE_FULL);
    NJPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
    MVOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MVOP_FULL);
    PWMRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PWM_FULL);
    SYSRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SYS_FULL);
    UARTRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UART_FULL);
    MBXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MBX_FULL);
    SEMRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SEM_FULL);
    DMXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SEM_FULL);
    VDEC_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VDEC_EX_FULL);
    GOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GOP_FULL);
    GFXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GFX_FULL);
    DLCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DLC_FULL);
    MIURegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MIU_FULL);
    #endif

#ifdef CONFIG_CHAKRA2
    printf("\033[35m[Chakra2] Driver Register to Utopia\033[m\n");

    VBIRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VBI_FULL);
    XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_FULL);
    PNLRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PNL_FULL);
    ACERegisterToUtopia((FUtopiaOpen)MODULE_TYPE_ACE_FULL);
    AESDMARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_AESDMA_FULL);
    IRRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_IR_FULL);
    NJPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
    MVOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MVOP_FULL);
    PWMRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PWM_FULL);
    PWSRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PWS_FULL);
    FLASHRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_FLASH_FULL);
    SYSRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SYS_FULL);
    UARTRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UART_FULL);
    MBXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MBX_FULL);
    TVENCODERRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_TVENCODER_FULL);
    DIPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DIP_FULL);
    SARRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SAR_FULL);
    CECRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CEC_FULL);
    SEMRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SEM_FULL);
    DMXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SEM_FULL);
    VDEC_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VDEC_EX_FULL);
    GOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GOP_FULL);
    GFXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GFX_FULL);
    DLCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DLC_FULL);
#endif

#ifdef CONFIG_MBOOT
    CPURegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CPU_FULL);
    PNLRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PNL_FULL);
    GOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GOP_FULL);
    GFXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GFX_FULL);
	MIURegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MIU_FULL);
    //DSCMBRegisterToUtopia();
    //CIPHERRegisterToUtopia();
    #ifdef CONFIG_API_JPEG_EX
    JPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
    #endif

    #ifdef CONFIG_API_NJPEG_EX
    NJPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
    #endif
    XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_MINIMUM);
    #ifdef CONFIG_API_DAC
    DACRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DAC_FULL);
    #endif
    #ifdef CONFIG_API_LOCALDIMM
    LDRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_LD_FULL);
    #endif
	AESDMARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_AESDMA_FULL);
    HDMITXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_HDMITX_FULL);
#endif

    MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);
    return 0;
}
#ifdef CONFIG_MBOOT
MS_U32 UtopiaInit_str()
{
    printf("\033[35m[NOS] UtopiaInit_str\033[m\n\n");
    MsOS_SHM_Init();

    psUtopiaPrivate = (UTOPIA_PRIVATE*)malloc(sizeof(UTOPIA_PRIVATE));
    memset(psUtopiaPrivate, 0, sizeof(UTOPIA_PRIVATE));
    psUtopiaPrivate->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO, "UtopiaPrivateMutex", MSOS_PROCESS_SHARED);

    MsOS_ObtainMutex(psUtopiaPrivate->u32MutexID, MSOS_WAIT_FOREVER);

    /*
     * to avoid weak attribute malfunction in static lib,
     * we explicitly call needed functions
     */
    UTOPIARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UTOPIA_FULL);
    BDMARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_BDMA_FULL);

    printf("\033[35m[MBoot] Driver Register to Utopia\033[m\n");
    CPURegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CPU_FULL);
    PNLRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PNL_FULL);
    XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_MINIMUM);

    MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);
    return 0;
}
#endif

MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** ppInstanceTmp
		, MS_U32 u32ModuleVersion, const void* const pAttribute)
{
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    UTOPIA_INSTANCE** ppInstance = (UTOPIA_INSTANCE**)ppInstanceTmp;

    //printf("application is going to open %s\n\n", moduleNames[u32ModuleID]);

    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == u32ModuleID)//need modify to link list
        {
            int ret = psUtopiaModule->fpOpen((void**)ppInstance, pAttribute);

            if(ret)
            {
                printf("\033[35mFunction = %s, Line = %d, UtopiaOpen: create Instance failed\033[m\n", __PRETTY_FUNCTION__, __LINE__);
                RET_OR_BLOCK(ret);
            }

            (*ppInstance)->psModule = psUtopiaModule;
			(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
            return ret;
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }

    printf("\033[35mFunction = %s, Line = %d, UtopiaOpen[%s] strange failed\033[m\n", __PRETTY_FUNCTION__, __LINE__, moduleNames[u32ModuleID]);
    psUtopiaModule = psUtopiaPrivate->psModuleHead;
    while(psUtopiaModule != NULL)
    {
        printf("having %s\n", moduleNames[psUtopiaModule->u32ModuleID]);
        psUtopiaModule = psUtopiaModule->psNext;
    }

    return UTOPIA_STATUS_FAIL;
}

MS_U32 UtopiaIoctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstanceTmp;
    return ((UTOPIA_INSTANCE*)pInstance)->psModule->fpIoctl(pInstance, u32Cmd, pArgs);
}

MS_U32 UtopiaClose(void* pInstantTmp)
{
    UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

    printf("\n[UTOPIA INFO] close moduleNames[%tu]: %s\n\n", (ptrdiff_t)pInstant->psModule->u32ModuleID, moduleNames[pInstant->psModule->u32ModuleID]);

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == pInstant->psModule->u32ModuleID)
        {
            return psUtopiaModule->fpClose(pInstant);
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return 0;
}

