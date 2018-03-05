#include "utopia_private.h"
#include <stdio.h>
#include "utopia.h"
#include "utopia_module.h"
#include "utopia_dapi.h"
#include "string.h"
#include "MsOS.h"
#include "MsCommon.h"

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

void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
#if defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KANO)
void CPURegisterToUtopia(FUtopiaOpen ModuleType);
void FLASHRegisterToUtopia(FUtopiaOpen ModuleType);
void IRRegisterToUtopia(FUtopiaOpen ModuleType);
void MBXRegisterToUtopia(FUtopiaOpen ModuleType);
void MVOPRegisterToUtopia(FUtopiaOpen ModuleType);
void MSPIRegisterToUtopia(FUtopiaOpen ModuleType);
void PWMRegisterToUtopia(FUtopiaOpen ModuleType);
void PWSRegisterToUtopia(FUtopiaOpen ModuleType);
void RTCRegisterToUtopia(FUtopiaOpen ModuleType);
void SARRegisterToUtopia(FUtopiaOpen ModuleType);

#if defined(CHIP_KERES) || defined(CHIP_KIRIN)
void CH34RegisterToUtopia(FUtopiaOpen ModuleType);
void VBIRegisterToUtopia(FUtopiaOpen ModuleType);
#endif

void ACERegisterToUtopia(FUtopiaOpen ModuleType);
void DLCRegisterToUtopia(FUtopiaOpen ModuleType);
void DMXRegisterToUtopia(FUtopiaOpen ModuleType);
void GFXRegisterToUtopia(FUtopiaOpen ModuleType);
void GOPRegisterToUtopia(FUtopiaOpen ModuleType);
void PNLRegisterToUtopia(FUtopiaOpen ModuleType);
void XCRegisterToUtopia(FUtopiaOpen ModuleType);

void VDEC_EXRegisterToUtopia(FUtopiaOpen ModuleType);
void SYSRegisterToUtopia(FUtopiaOpen ModuleType);
void UARTRegisterToUtopia(FUtopiaOpen ModuleType);
void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType);
#ifdef CONFIG_API_JPEG_EX
void JPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
#ifdef CONFIG_API_NJPEG_EX
void NJPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType);
#endif
void GPDRegisterToUtopia(FUtopiaOpen ModuleType);

void DACRegisterToUtopia(void);
void SEMRegisterToUtopia(void);
void DSCMBRegisterToUtopia(void);
void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
void ACPRegisterToUtopia(FUtopiaOpen ModuleType);
void HDMITXRegisterToUtopia(FUtopiaOpen ModuleType);
void CECRegisterToUtopia(FUtopiaOpen ModuleType);

#endif

MS_U32 UtopiaInit()
{
	MS_U32 u32Ret = 0;
	printf("\033[35m[eCos] UtopiaInit\033[m\n\n");

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

#if defined(CHIP_KERES) || defined(CHIP_KIRIN) || defined(CHIP_KANO)
    CPURegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CPU_FULL);
    MBXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MBX_FULL);
    SYSRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SYS_FULL);
    UARTRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_UART_FULL);
#if 0 // Below is not necessary for ucos
    FLASHRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_FLASH_FULL);
    IRRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_IR_FULL);
    MVOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MVOP_FULL);
    MSPIRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_MSPI_FULL);
    PWMRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PWM_FULL);
    PWSRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PWS_FULL);
    RTCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_RTC_FULL);
    SARRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_SAR_FULL);
    CECRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CEC_FULL);
    ACERegisterToUtopia((FUtopiaOpen)MODULE_TYPE_ACE_FULL);
    DLCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DLC_FULL);
    DMXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_DMX_FULL);
    GFXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GFX_FULL);
    GOPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GOP_FULL);
    PNLRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_PNL_FULL);
    XCRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_XC_FULL);

#if defined(CHIP_KERES) || defined(CHIP_KIRIN)
    CH34RegisterToUtopia((FUtopiaOpen)MODULE_TYPE_CH34_FULL);
    VBIRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VBI_FULL);
#endif
    VDEC_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_VDEC_EX_FULL);
    TVENCODERRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_TVENCODER_FULL);
#ifdef CONFIG_API_JPEG_EX
    JPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
#endif
#ifdef CONFIG_API_NJPEG_EX
    NJPEG_EXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_NJPEG_EX_FULL);
#endif
    GPDRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_GPD_FULL);

    DACRegisterToUtopia();
    SEMRegisterToUtopia();
    DSCMBRegisterToUtopia();
    AESDMARegisterToUtopia((FUtopiaOpen)MODULE_TYPE_AESDMA_FULL);
    ACPRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_ACP_FULL);
    HDMITXRegisterToUtopia((FUtopiaOpen)MODULE_TYPE_HDMITX_FULL);
#endif
#endif

    MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);

	return u32Ret;
}

MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** ppInstanceTmp
		, MS_U32 u32ModuleVersion, const void* const pAttribute)
{
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    UTOPIA_INSTANCE** ppInstance = (UTOPIA_INSTANCE**)ppInstanceTmp;

    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == u32ModuleID)
        {
            int ret = psUtopiaModule->fpOpen((void**)ppInstance, pAttribute);

            if(ret)
            {
                printf("[utopia error] fail to create instance\n");
                return ret;
            }

            (*ppInstance)->psModule = psUtopiaModule;
			(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
            return ret;
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }

	printf("\033[35mFunction = %s, Line = %d, UtopiaOpen strange failed\033[m\n", __FUNCTION__, __LINE__);

	return UTOPIA_STATUS_FAIL;
}

MS_U32 UtopiaIoctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
	UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstanceTmp;
	/* check param. */
	if (pInstance == NULL)
	{
		printf("[utopia error] instance pointer should not be null\n");
		return UTOPIA_STATUS_FAIL;
	}

	return TO_INSTANCE_PTR(pInstance)->psModule->fpIoctl(pInstance, u32Cmd, pArgs);
}

MS_U32 UtopiaClose(void* pInstantTmp)
{
    UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

	printf("\n[utopia info] close module id: %s\n",
			moduleNames[pInstant->psModule->u32ModuleID]);

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
