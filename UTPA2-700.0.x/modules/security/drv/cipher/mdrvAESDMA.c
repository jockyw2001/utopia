#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#include "MsTypes.h"
#include "utopia_dapi.h"
#include "utopia.h"
//#include "drvAESDMA.h"
#include "drvCIPHER.h"
#include "drvAESDMA_private.h"
#include "MsOS.h"

#define AESDMA_KEY_SLOT_BASE (0x10)

enum
{
    AESDMA_POOL_ID_AES = 0,
    AESDMA_POOL_ID_SHA,
    AESDMA_POOL_ID_RSA,
    AESDMA_POOL_ID_DRVAES,
} eAesdmaPoolID;

void* pModuleAESDMA = NULL;

MS_U32 _AESDMA_MSB2LSB(MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* psResource = NULL;

    // 1. deal with module
    //void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_AESDMA, 8, &pModuleAESDMA);
    UtopiaModuleRegister(pModuleAESDMA);
    UtopiaModuleSetupFunctionPtr(pModuleAESDMA, (FUtopiaOpen)AESDMAOpen, (FUtopiaClose)AESDMAClose, (FUtopiaIOctl)AESDMAIoctl);

    // 2. deal with resource
    //Resource AES
	UtopiaModuleAddResourceStart(pModuleAESDMA, AESDMA_POOL_ID_AES);
    UtopiaResourceCreate("AES", sizeof(AESDMA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleAESDMA, psResource, AESDMA_POOL_ID_AES);
	UtopiaModuleAddResourceEnd(pModuleAESDMA, AESDMA_POOL_ID_AES);

    //Resource SHA
    UtopiaModuleAddResourceStart(pModuleAESDMA, AESDMA_POOL_ID_SHA);
    psResource = NULL;
    UtopiaResourceCreate("SHA", sizeof(SHA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleAESDMA, psResource, AESDMA_POOL_ID_SHA);
	UtopiaModuleAddResourceEnd(pModuleAESDMA, AESDMA_POOL_ID_SHA);

    //Resource RSA
    UtopiaModuleAddResourceStart(pModuleAESDMA, AESDMA_POOL_ID_RSA);
    psResource = NULL;
    UtopiaResourceCreate("RSA", sizeof(RSA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleAESDMA, psResource, AESDMA_POOL_ID_RSA);
	UtopiaModuleAddResourceEnd(pModuleAESDMA, AESDMA_POOL_ID_RSA);

    //Resource DRVAES
    UtopiaModuleAddResourceStart(pModuleAESDMA, AESDMA_POOL_ID_DRVAES);
    psResource = NULL;
    UtopiaResourceCreate("DRVAES", sizeof(DRVAESDMA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleAESDMA, psResource, AESDMA_POOL_ID_DRVAES);
	UtopiaModuleAddResourceEnd(pModuleAESDMA, AESDMA_POOL_ID_DRVAES);

}

MS_U32 AESDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	MS_DEBUG_MSG(printf("\n[AESDMA INFO]AESDMA open \n"));

    AESDMA_INSTANT_PRIVATE *pAesdmaPri = NULL;
    void *pAesdmaPriVoid = NULL;
    UtopiaInstanceCreate(sizeof(AESDMA_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pAesdmaPriVoid);

    pAesdmaPri = (AESDMA_INSTANT_PRIVATE *)pAesdmaPriVoid;

    pAesdmaPri->fpAESDMAInit = _MDrv_AESDMA_Init;
    pAesdmaPri->fpAESDMASetIV = _MDrv_AESDMA_SetIV;
    pAesdmaPri->fpAESDMAReset = _MDrv_AESDMA_Reset;
    pAesdmaPri->fpAESDMASetFileInOut = _MDrv_AESDMA_SetFileInOut;
    pAesdmaPri->fpAESDMASetKey = _MDrv_AESDMA_SetKey;
    pAesdmaPri->fpAESDMASelEng = _MDrv_AESDMA_SelEng;
    pAesdmaPri->fpAESDMAStart = _MDrv_AESDMA_Start;
    pAesdmaPri->fpAESDMAGetStatus = _MDrv_AESDMA_GetStatus;
    pAesdmaPri->fpAESDMAIsFinished = _MDrv_AESDMA_IsFinished;
    pAesdmaPri->fpAESDMASetSecureKey = _MDrv_AESDMA_SetSecureKey;
    pAesdmaPri->fpAESDMASetPS = _MDrv_AESDMA_SetPS;
    pAesdmaPri->fpAESDMAPSRelease = _MDrv_AESDMA_PSRelease;
    pAesdmaPri->fpAESDMAGetPSMatchedByteCNT = _MDrv_AESDMA_GetPSMatchedByteCNT;
    pAesdmaPri->fpAESDMAGetPSMatchedPTN = _MDrv_AESDMA_GetPSMatchedPTN;
    pAesdmaPri->fpAESDMANotify = _MDrv_AESDMA_Notify;
    pAesdmaPri->fpAESDMARand = _MDrv_AESDMA_Rand;
    pAesdmaPri->fpRSACalculate = _MDrv_RSA_Calculate;
    pAesdmaPri->fpRSAIsFinished = _MDrv_RSA_IsFinished;
    pAesdmaPri->fpRSAOutput = _MDrv_RSA_Output;
    pAesdmaPri->fpSHACalculate = _MDrv_SHA_Calculate;
    pAesdmaPri->fpSHACalculateManual = _MDrv_SHA_CalculateManual;
    pAesdmaPri->fpAESDMASetKey_Ex = _MDrv_AESDMA_SetKey_Ex;
    pAesdmaPri->fpAESDMASetIV_Ex = _MDrv_AESDMA_SetIV_Ex;
    pAesdmaPri->fpAESDMASetDefaultCAVid = _MDrv_AESDMA_SetDefaultCAVid;
    pAesdmaPri->fpAESDMASetKeyIndex = _MDrv_AESDMA_SetKeyIndex;
    pAesdmaPri->fpAESDMAAllocKeySlot = _MDrv_AESDMA_AllocKeySlot;
    pAesdmaPri->fpAESDMAFreeKeySlot = _MDrv_AESDMA_FreeKeySlot;
    pAesdmaPri->fpAESDMAInitBySWFlag = _MDrv_AESDMA_InitBySWFlag;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AESDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    PAESDMA_INIT pInitParam = NULL;
    PAESDMA_FILE_INOUT pFileInOutParam = NULL;
    PAESDMA_SEL_ENG pSelEngParam = NULL;
    PAESDMA_SET_KEY_EX pSetKeyEx = NULL;
    PAESDMA_SET_IV_EX pSetIVEx = NULL;
    PAESDMA_SET_PS pSetPSParam = NULL;
    PAESDMA_NOTIFY pNotifyParam = NULL;
    PAESDMA_RAND pRandParam = NULL;
    PAESDMA_CAVID pCAVid = NULL;
    PRSA_CALCULATE pRSACalParam = NULL;
    PRSA_OUTPUT pRSAOutParam = NULL;
    PSHA_CALCULATE pSHACalParam = NULL;
    PSHA_CALCULATE_MANUAL pSHACalManuParam = NULL;
    PAESDMA_KEY_INDEX pKeyIndex = NULL;
    PAESDMA_ALLOC_KEY_SLOT pAllocKeySlot = NULL;
    MS_U32 u32Ret = 0;
	AESDMA_INSTANT_PRIVATE* psAesInstPri = NULL;
    AESDMA_RESOURCE_SHARED* psAesResourceShared = NULL;
    RSA_RESOURCE_SHARED* psRSAResourceShared = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psAesInstPri);
	void* pTmpResource = NULL;
    void* pDRVAESResource = NULL;
    MS_U32 u32ShmId = 0;
    MS_VIRT u32Addr = 0;
    MS_U32 u32BufSize = 0;
    MS_U32 u32IVarrayAddr[4] = {0};
    MS_U32 u32KeyarryAddr[4] = {0};
    MS_U32 u32timeout_count = 0;
    MS_U32 u32timeout_value = 0;
    DRVAESDMA_RESOURCE_PRIVATE* pstResPri  = NULL;

    if(UtopiaResourceObtain(pModuleAESDMA, AESDMA_POOL_ID_DRVAES, &pDRVAESResource) != UTOPIA_STATUS_SUCCESS)
    {
         printf("UtopiaResourceObtain fail\n");
         return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceGetPrivate(pDRVAESResource, (void**)&pstResPri);

    if((FALSE == pstResPri->bInited_Drv)  &&
        (u32Cmd != MDrv_CMD_AESDMA_Init) )  // which command don't need do it after init?
    {
        UtopiaResourceRelease(pDRVAESResource);
        return UTOPIA_STATUS_FAIL;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_AESDMA_Init:
            {
                if(pstResPri->bInited_Drv == FALSE)
                {
                    pInitParam = (PAESDMA_INIT)pArgs;
                    u32Ret = (psAesInstPri->fpAESDMAInit)(pInitParam->u32miu0addr,pInitParam->u32miu1addr,pInitParam->u32miunum);
                    psAesInstPri->u32IVLen = 0;
                    psAesInstPri->u32FileinAddr = 0;
                    psAesInstPri->u32FileInNum = 0;
                    psAesInstPri->u32FileOutSAddr = 0;
                    psAesInstPri->u32FileOutEAddr = 0;
                    psAesInstPri->ptrKey = 0;
                    psAesInstPri->u32KeyLen = 0;
                    psAesInstPri->u32Mode = 0xFF; //Default simple DMA
                    psAesInstPri->u32KeyIndex = 0;
                    psAesInstPri->bOddKey = FALSE;
                    psAesInstPri->u32CAVid = 0;

                    memset(psAesInstPri->CipherKey, 0, sizeof(psAesInstPri->CipherKey));
                    memset(psAesInstPri->InitVector, 0, sizeof(psAesInstPri->InitVector));
                    memset(psAesInstPri->u8KeySlotUsage, 0, sizeof(psAesInstPri->u8KeySlotUsage));

                    if (u32Ret == DRVAESDMA_OK)
                    {
                        pstResPri->bInited_Drv = TRUE;
                        u32Ret = UTOPIA_STATUS_SUCCESS;
                    }
                    else
                    {
                        u32Ret = UTOPIA_STATUS_FAIL;
                    }
                }
                else
                {
                    u32Ret = (psAesInstPri->fpAESDMAInitBySWFlag)(TRUE);
                    if (u32Ret == DRVAESDMA_OK)
                    {
                        u32Ret = UTOPIA_STATUS_SUCCESS;
                    }
                    else
                    {
                        u32Ret = UTOPIA_STATUS_FAIL;
                    }
                }
            }
            break;

        case MDrv_CMD_AESDMA_SetIV:
            {
                if(NULL != pArgs)
                {
                    u32IVarrayAddr[3] = _AESDMA_MSB2LSB((MS_U8 *)pArgs);
                    u32IVarrayAddr[2] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 4);
                    u32IVarrayAddr[1] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 8);
                    u32IVarrayAddr[0] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 12);
                }
                else
                {
                    printf("[AESDMA INFO]Set IV fail, IV is NULL\n");
                    u32Ret = UTOPIA_STATUS_FAIL;
                    break;
                }

                memcpy((void *)psAesInstPri->InitVector,(MS_U8 *)u32IVarrayAddr,16);
                psAesInstPri->u32IVLen = 16;
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_Reset:
            {
                u32Ret = (psAesInstPri->fpAESDMAReset)();
                //Reset private data
                psAesInstPri->u32IVLen = 0;
                psAesInstPri->u32FileinAddr = 0;
                psAesInstPri->u32FileInNum = 0;
                psAesInstPri->u32FileOutSAddr = 0;
                psAesInstPri->u32FileOutEAddr = 0;
                psAesInstPri->ptrKey = 0;
                psAesInstPri->u32KeyLen = 0;
                psAesInstPri->u32Mode = 0xFF;
                psAesInstPri->u32KeyIndex = 0;
                psAesInstPri->bOddKey = FALSE;
                psAesInstPri->u32CAVid = 0;

                memset(psAesInstPri->CipherKey, 0, sizeof(psAesInstPri->CipherKey));
                memset(psAesInstPri->InitVector, 0, sizeof(psAesInstPri->InitVector));
                memset(psAesInstPri->u8KeySlotUsage, 0, sizeof(psAesInstPri->u8KeySlotUsage));

                if (u32Ret == DRVAESDMA_OK)
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;

        case MDrv_CMD_AESDMA_SetFileInOut:
            {
                pFileInOutParam = (PAESDMA_FILE_INOUT)pArgs;
                psAesInstPri->u32FileinAddr = pFileInOutParam->u32FileinAddr;
                psAesInstPri->u32FileInNum = pFileInOutParam->u32FileInNum;
                psAesInstPri->u32FileOutSAddr = pFileInOutParam->u32FileOutSAddr;
                psAesInstPri->u32FileOutEAddr = pFileInOutParam->u32FileOutEAddr;
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileinAddr = %lx\n",psAesInstPri->u32FileinAddr));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileInNum = %lx\n",psAesInstPri->u32FileInNum));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileOutSAddr = %lx\n",psAesInstPri->u32FileOutSAddr));
                MS_DEBUG_MSG(printf("[AESDMA INFO]FileOutEAddr = %lx\n",psAesInstPri->u32FileOutEAddr));
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_SetKey:
            {
                if(NULL != pArgs)
                {
                    u32KeyarryAddr[3] = _AESDMA_MSB2LSB((MS_U8 *)pArgs);
                    u32KeyarryAddr[2] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 4);
                    u32KeyarryAddr[1] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 8);
                    u32KeyarryAddr[0] = _AESDMA_MSB2LSB((MS_U8 *)pArgs + 12);
                    memcpy((void *)psAesInstPri->CipherKey,(MS_U8 *)u32KeyarryAddr,16);
                    psAesInstPri->ptrKey = (MS_VIRT)psAesInstPri->CipherKey;
                }
                else
                {
                    psAesInstPri->ptrKey = 0;
                }

                psAesInstPri->u32KeyLen = 16;
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_SelEng:
            {
                pSelEngParam = (PAESDMA_SEL_ENG)pArgs;
                psAesInstPri->u32Mode = pSelEngParam->eMode;
                psAesInstPri->bDescrypt = pSelEngParam->bDescrypt;
                MS_DEBUG_MSG(printf("[AESDMA INFO]mode = %lu\n",(long unsigned int)psAesInstPri->u32Mode));
                MS_DEBUG_MSG(printf("[AESDMA INFO]Descrypt = %x\n",psAesInstPri->bDescrypt));
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_Start:
            {
                if(UtopiaResourceObtain(pModuleAESDMA, AESDMA_POOL_ID_AES, &pTmpResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            u32Ret = UTOPIA_STATUS_ERR_NOT_AVAIL;
                    break;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        printf("[AESDMA INFO]create Resouce shared memory fail\n");
                        u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                        break;
                    }
                    memset((MS_U8 *)u32Addr, 0, sizeof(AESDMA_RESOURCE_SHARED));
                }

                psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;

                psAesResourceShared->pAESResource = pTmpResource;
                MS_DEBUG_MSG(printf("\n[AESDMA INFO]pResource = %x\n",(int)psAesResourceShared->pAESResource));

                //AESDMA reset
//                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set mode and select encrypt or decrypt
                (psAesInstPri->fpAESDMASelEng)(psAesInstPri->u32Mode,psAesInstPri->bDescrypt);

                //AESDMA set IV
                if((psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    //u32IVarrayAddr = (MS_U32)&psAesInstPri->InitVector[0];
                    //(psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);
                    (psAesInstPri->fpAESDMASetIV_Ex)(&psAesInstPri->InitVector[0], psAesInstPri->u32IVLen);
                }

                //CBC segmental decryption case
                //if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                //    && psAesInstPri->bDescrypt == TRUE)
                if((((psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CTS_CBC))&& psAesInstPri->bDescrypt == TRUE)||
                    (psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CTR))
                {
                    //printf("[AESDMA]FileInAddr PA = %x\n",psAesInstPri->u32FileinAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 i;
                    MS_VIRT FileinVirtAddr;

                    FileinVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileinAddr);

                    if(psAesInstPri->u32Mode != E_DRVAESDMA_CIPHER_CTR)
                    {
                        for(i = 0; i < 16; i++)
                        {
                            psAesInstPri->InitVector[16-i-1] = *(MS_U8 *)(FileinVirtAddr + psAesInstPri->u32FileInNum-1-i);
                            MS_DEBUG_MSG(printf("[AESDMA INFO]Init Vector[%d] = 0x%x\n",(int)i, psAesInstPri->InitVector[i]));
                        }
                    }
                    psAesInstPri->u32IVLen = 16;
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bSecretKey==1)
                {
                    (psAesInstPri->fpAESDMASetSecureKey)();
                }
                else
                {
                    //u32KeyarryAddr = (MS_U32)&psAesInstPri->CipherKey[0];
                    //(psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                    (psAesInstPri->fpAESDMASetKey_Ex)((MS_U8*)psAesInstPri->ptrKey, psAesInstPri->u32KeyLen);

                    if(0 != psAesInstPri->u32KeyIndex)
                    {
                        (psAesInstPri->fpAESDMASetKeyIndex)(psAesInstPri->u32KeyIndex, psAesInstPri->bOddKey);
                    }
                }

                //AESDMA trigger start
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_CMD_AESDMA_Start\n"));
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_GetStatus:
            {
                if ((psAesInstPri->fpAESDMAGetStatus)(&u32Ret) == UTOPIA_STATUS_SUCCESS)
                {
                    *(MS_U32 *)pArgs = u32Ret;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;

        case MDrv_CMD_AESDMA_IsFinished:
            {
                if (FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    printf("[AESDMA INFO]query Resouce shared memory fail\n");
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                    break;
                }
                else
                {

#if defined (__aarch64__)

                    psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;
#else
                    psAesResourceShared = (AESDMA_RESOURCE_SHARED*)(MS_U32)u32Addr;
#endif
                    pTmpResource = psAesResourceShared->pAESResource;
                    MS_DEBUG_MSG(printf("\n[AESDMA INFO]pResource = %p\n",(int)psAesResourceShared->pAESResource));
                }

                while ((psAesInstPri->fpAESDMAIsFinished)(&u32Ret) != DRVAESDMA_OK);

                //CBC segmental encryption case
                if(((psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->u32Mode == E_DRVAESDMA_CIPHER_CTS_CBC))
                    && psAesInstPri->bDescrypt == FALSE)
                {
                    //printf("[AESDMA]FileOutStartAddr PA = %x\n",psAesInstPri->u32FileOutSAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 i;
                    MS_VIRT FileOutStartVirtAddr;

                    FileOutStartVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileOutSAddr);

                    for(i = 0; i < 16; i++)
                    {
                        psAesInstPri->InitVector[16-i-1] = *(MS_U8 *)(FileOutStartVirtAddr+psAesInstPri->u32FileInNum-1-i);
                        MS_DEBUG_MSG(printf("[AESDMA INFO]Init Vector[%d] = 0x%x\n",(int)i, psAesInstPri->InitVector[i]));
                    }
                    psAesInstPri->u32IVLen = 16;

                }

                //clear secret key setting
                psAesInstPri->bSecretKey = 0;
                UtopiaResourceRelease(pTmpResource);
                MS_DEBUG_MSG(printf("[AESDMA INFO]MDrv_AESDMA_IsFinished\n"));
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_SetSecureKey:
            {
                psAesInstPri->bSecretKey = 1;
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_SetPS:
            {
                pSetPSParam = (PAESDMA_SET_PS)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetPS)(pSetPSParam->u32PTN,pSetPSParam->u32Mask,pSetPSParam->bPSin_Enable,pSetPSParam->bPSout_Enable);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_PSRelease:
            {
                u32Ret = (psAesInstPri->fpAESDMAPSRelease)();
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_GetPSMatchedByteCNT:
            {
                if ((psAesInstPri->fpAESDMAGetPSMatchedByteCNT)(&u32Ret) == UTOPIA_STATUS_SUCCESS)
                {
                    *(MS_U32 *)pArgs = u32Ret;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;

        case MDrv_CMD_AESDMA_GetPSMatchedPTN:
            {
                u32Ret = (psAesInstPri->fpAESDMAGetPSMatchedPTN)();
            }
            break;

        case MDrv_CMD_AESDMA_Notify:
            {
                pNotifyParam = (PAESDMA_NOTIFY)pArgs;
                u32Ret = (psAesInstPri->fpAESDMANotify)(pNotifyParam->eEvents,pNotifyParam->pfCallback);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_Rand:
            {
                pRandParam = (PAESDMA_RAND)pArgs;
                u32Ret = (psAesInstPri->fpAESDMARand)(pRandParam->u32PABuf,pRandParam->u32Size);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_RSA_Calculate:
            {
                pRSACalParam = (PRSA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModuleAESDMA, AESDMA_POOL_ID_RSA, &pTmpResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            u32Ret = UTOPIA_STATUS_ERR_NOT_AVAIL;
                    break;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        printf("[RSA INFO]create Resouce shared memory fail\n");
                        u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                        break;
                    }
                    memset((MS_U8 *)u32Addr, 0, sizeof(RSA_RESOURCE_SHARED));
                }

#if defined (__aarch64__)

                psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
#else
                psRSAResourceShared = (RSA_RESOURCE_SHARED*)(MS_U32)u32Addr;
#endif

                psRSAResourceShared->pRSAResource = pTmpResource;
                MS_DEBUG_MSG(printf("\n[RSA INFO]pResource = %x\n",(int)psRSAResourceShared->pRSAResource));

                u32Ret = (psAesInstPri->fpRSACalculate)(pRSACalParam->pstSign,pRSACalParam->pstKey,pRSACalParam->eMode);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_RSA_IsFinished:
            {
                if(pArgs)
                {
                    u32timeout_value = *(MS_U32 *)pArgs;
                }
                else
                {
                    printf("[RSA INFO] Please pass timeout value to driver\n");
                    u32Ret = UTOPIA_STATUS_ERR_INV;
                    break;
                }

                if (u32timeout_value > RSA_MAX_TIMEOUT_VALUE)
                {
                    printf("[RSA INFO] Invalid timeout value\n");
                    u32Ret = UTOPIA_STATUS_ERR_INV;
                    break;
                }

                if (FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    printf("[RSA INFO]query Resouce shared memory fail\n");
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                    break;
                }
                else
                {
                    psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
                    pTmpResource = psRSAResourceShared->pRSAResource;
                    MS_DEBUG_MSG(printf("\n[RSA INFO]pResource = %x\n",(int)psRSAResourceShared->pRSAResource));
                }

                while ((psAesInstPri->fpRSAIsFinished)() != DRVAESDMA_OK)
				{
                    #ifdef MSOS_TYPE_OPTEE
                    #else
					MsOS_DelayTaskUs(10);
                    #endif
					u32timeout_count++;

					//timeout
					if(u32timeout_count>u32timeout_value)
					{
                        UtopiaResourceRelease(pTmpResource);
                        printf("[RSA ERROR] RSA timeout happened\n");
						u32Ret = UTOPIA_STATUS_FAIL;
                        break;
					}

				}

                MS_DEBUG_MSG(printf("[RSA INFO]MDrv_RSA_IsFinished\n"));
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_RSA_Output:
            {
                if (FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    printf("[RSA INFO]query Resouce shared memory fail\n");
                    u32Ret = UTOPIA_STATUS_ERR_NOMEM;
                    break;
                }
                else
                {
                    psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
                    pTmpResource = psRSAResourceShared->pRSAResource;
                    MS_DEBUG_MSG(printf("\n[RSA INFO]pResource = %x\n",(int)psRSAResourceShared->pRSAResource));
                }

                pRSAOutParam = (PRSA_OUTPUT)pArgs;
                while((psAesInstPri->fpRSAIsFinished)()==1);
                u32Ret = (psAesInstPri->fpRSAOutput)(pRSAOutParam->eMode,pRSAOutParam->pstRSAOut);

                UtopiaResourceRelease(pTmpResource);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_SHA_Calculate:
            {
                pSHACalParam = (PSHA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModuleAESDMA, AESDMA_POOL_ID_SHA, &pTmpResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            u32Ret = UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculate)(pSHACalParam->eMode,pSHACalParam->u32PAInBuf,pSHACalParam->u32Size,pSHACalParam->u32PAOutBuf);
                UtopiaResourceRelease(pTmpResource);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_Memcpy:
            {
                //AESDMA trigger start for using AESDMA replace BDMA case
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);

                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_SHA_CalculateManual:
            {
                pSHACalManuParam = (PSHA_CALCULATE_MANUAL)pArgs;
                if(UtopiaResourceObtain(pModuleAESDMA, AESDMA_POOL_ID_SHA, &pTmpResource) != 0)
                {
                    printf("UtopiaResourceObtainToInstant fail\n");
		            u32Ret = UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculateManual)(pSHACalManuParam->stCfg,pSHACalManuParam->eStage,pSHACalManuParam->u32DoneBytes,pSHACalManuParam->pu8SetIV);
                UtopiaResourceRelease(pTmpResource);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_SetKey_Ex:
            {
                pSetKeyEx = (PAESDMA_SET_KEY_EX)pArgs;
                if(pSetKeyEx->pu8Key == NULL)
                {
                    psAesInstPri->ptrKey = 0;
                }
                else
                {
                    psAesInstPri->u32KeyIndex = 0;
                    memcpy((void *)psAesInstPri->CipherKey, pSetKeyEx->pu8Key, pSetKeyEx->u32Len);
                    psAesInstPri->ptrKey = (MS_VIRT)psAesInstPri->CipherKey;
                }

                psAesInstPri->u32KeyLen = pSetKeyEx->u32Len;
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_SetIV_Ex:
            {
                pSetIVEx = (PAESDMA_SET_IV_EX)pArgs;
                memcpy((void *)psAesInstPri->InitVector, pSetIVEx->pu8IV, pSetIVEx->u32Len);
                psAesInstPri->u32IVLen = pSetIVEx->u32Len;
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case MDrv_CMD_AESDMA_Set_Key_Index:
            {
                pKeyIndex = (PAESDMA_KEY_INDEX)pArgs;
                psAesInstPri->u32KeyIndex = pKeyIndex->u32KeyIndex;
                psAesInstPri->bOddKey = pKeyIndex->bOddKeyEn;
                psAesInstPri->ptrKey = 0;
                u32Ret = (psAesInstPri->fpAESDMASetKeyIndex)(pKeyIndex->u32KeyIndex, pKeyIndex->bOddKeyEn);
                if (u32Ret == DRVAESDMA_OK)
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                else
                    u32Ret = UTOPIA_STATUS_FAIL;
            }
            break;

        case MDrv_CMD_AESDMA_SetDefaultCAVid:
            {
                pCAVid = (PAESDMA_CAVID)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetDefaultCAVid)(pCAVid->u32CAVid);
                if (u32Ret == DRVAESDMA_OK)
                {
                    psAesInstPri->u32CAVid = pCAVid->u32CAVid;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;

        case MDrv_CMD_AESDMA_AllocKeySlot:
            {
                MS_U32 i = 0;

                pAllocKeySlot = (PAESDMA_ALLOC_KEY_SLOT)pArgs;

                for(i = 0;i< sizeof(psAesInstPri->u8KeySlotUsage);i++)
                {
                    if(0 == psAesInstPri->u8KeySlotUsage[i])
                    {
                        break;
                    }
                }

                if(sizeof(psAesInstPri->u8KeySlotUsage) == i)
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
                else
                {
                    psAesInstPri->u8KeySlotUsage[i] = 1;
                    *pAllocKeySlot->pu32KeyIndex = i + AESDMA_KEY_SLOT_BASE;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;

        case MDrv_CMD_AESDMA_FreeKeySlot:
            {
                pKeyIndex = (PAESDMA_KEY_INDEX)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetDefaultCAVid)(psAesInstPri->u32CAVid);
                if(u32Ret != DRVAESDMA_OK)
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                    break;
                }

                u32Ret = (psAesInstPri->fpAESDMAFreeKeySlot)(pKeyIndex->u32KeyIndex);

                if ((u32Ret == DRVAESDMA_OK) && (pKeyIndex->u32KeyIndex >= AESDMA_KEY_SLOT_BASE))
                {
                    psAesInstPri->u8KeySlotUsage[(pKeyIndex->u32KeyIndex - AESDMA_KEY_SLOT_BASE)] = 0;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;

        default:
            break;
    };

    UtopiaResourceRelease(pDRVAESResource);

	return u32Ret;
}

MS_U32 AESDMAClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}

