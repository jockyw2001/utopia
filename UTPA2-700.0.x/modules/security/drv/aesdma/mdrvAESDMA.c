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
#include "drvAESDMA.h"
#include "drvAESDMA_private.h"
#include "MsOS.h"
#include "halAESDMA.h"
#include "ULog.h"

#define TAG_AESDMA "AESDMA"

enum
{
    AESDMA_POOL_ID_AES = 0,
    AESDMA_POOL_ID_SHA,
    AESDMA_POOL_ID_RSA,
} eAesdmaPoolID;

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_AESDMA, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)AESDMAOpen, (FUtopiaClose)AESDMAClose, (FUtopiaIOctl)AESDMAIoctl);

    // 2. deal with resource
    void* psResource = NULL;
	UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_AES);
    UtopiaResourceCreate("AES", sizeof(AESDMA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_AES);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_AES);

    UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_SHA);
    psResource = NULL;
    UtopiaResourceCreate("SHA", sizeof(SHA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_SHA);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_SHA);

    UtopiaModuleAddResourceStart(pUtopiaModule, AESDMA_POOL_ID_RSA);
    psResource = NULL;
    UtopiaResourceCreate("RSA", sizeof(RSA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AESDMA_POOL_ID_RSA);
	UtopiaModuleAddResourceEnd(pUtopiaModule, AESDMA_POOL_ID_RSA);

}

MS_U32 AESDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
	MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[AESDMA INFO]AESDMA open \n"));

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
    pAesdmaPri->fpAESDMASetClk = _MDrv_AESDMA_Set_Clk;
    pAesdmaPri->fpAESDMASetSecureKey = _MDrv_AESDMA_SetSecureKey;
    pAesdmaPri->fpAESDMASetPS = _MDrv_AESDMA_SetPS;
    pAesdmaPri->fpAESDMAPSRelease = _MDrv_AESDMA_PSRelease;
    pAesdmaPri->fpAESDMAGetPSMatchedByteCNT = _MDrv_AESDMA_GetPSMatchedByteCNT;
    pAesdmaPri->fpAESDMAGetPSMatchedPTN = _MDrv_AESDMA_GetPSMatchedPTN;
    pAesdmaPri->fpAESDMANotify = _MDrv_AESDMA_Notify;
    pAesdmaPri->fpAESDMARand = _MDrv_AESDMA_Rand;
    pAesdmaPri->fpAESDMAGetRandNum = _MDrv_AESDMA_Get_Rand_Num;
    pAesdmaPri->fpRSACalculate = _MDrv_RSA_Calculate;
    pAesdmaPri->fpRSAIsFinished = _MDrv_RSA_IsFinished;
    pAesdmaPri->fpRSAOutput = _MDrv_RSA_Output;
    pAesdmaPri->fpSHACalculate = _MDrv_SHA_Calculate;
    pAesdmaPri->fpSHACalculateManual = MDrv_SHA_CalculateManual;
    pAesdmaPri->fpAESDMAParserMaskscrmb = _MDrv_AESDMA_Parser_MaskScrmb;
    pAesdmaPri->fpAESDMAParserSetScrmbPatten = _MDrv_AESDMA_Parser_SetScrmbPattern;
    pAesdmaPri->fpAESDMAParserBypassPid = _MDrv_AESDMA_Parser_BypassPid;
    pAesdmaPri->fpAESDMAParserSetPid = _MDrv_AESDMA_Parser_SetPid;
    pAesdmaPri->fpAESDMAParserEncrypt = _MDrv_AESDMA_Parser_Encrypt;
    pAesdmaPri->fpAESDMAParserDecrypt = _MDrv_AESDMA_Parser_Decrypt;
    pAesdmaPri->fpAESDMAParserQueryPidCount = _MDrv_AESDMA_Parser_QueryPidCount;
    pAesdmaPri->fpAESDMAParserSetAddedScrmbPattern = _MDrv_AESDMA_Parser_SetAddedScrmbPattern;
    pAesdmaPri->fpAESDMASetKeySel = _MDrv_AESDMA_SetKeySel;
    pAesdmaPri->fpAESDMASetOddIv = _MDrv_AESDMA_SetOddIV;
    pAesdmaPri->fpAESDMAIsSecretKeyInNormalBlank = _MDrv_AESDMA_IsSecretKeyInNormalBank;
    pAesdmaPri->fpHDCPHDCPProcessCipher = MDrv_HDCP_ProcessCipher;
    pAesdmaPri->fpAESDMANormalReset = _MDrv_AESDMA_NormalReset;
    pAesdmaPri->fpGetHDCPCipherState = MDrv_HDCP_GetHdcpCipherState;
    pAesdmaPri->bEvenScrmbPatternEn = FALSE;
    pAesdmaPri->bOddScrmbPatternEn = FALSE;
    pAesdmaPri->bSetKey = FALSE;
    pAesdmaPri->bSetEng = FALSE;
    pAesdmaPri->bSetIV = FALSE;
    pAesdmaPri->bHwParserEn = FALSE;
    pAesdmaPri->bPid0En = FALSE;
    pAesdmaPri->bPid1En = FALSE;
    pAesdmaPri->bMaskScrmbEn = FALSE;
    pAesdmaPri->fpAESDMAKeyIndex = _MDrv_AESDMA_SetKeyIndex;
    pAesdmaPri->u32KeyIndexEven = 0x0;
    pAesdmaPri->u32KeyIndexOdd = 0x0;
    pAesdmaPri->bEnableKeyIndexEvenKey = FALSE;
    pAesdmaPri->bEnableKeyIndexOddKey = FALSE;
    pAesdmaPri->fpAesCtr64 = _MDrv_AESDMA_SetAesCtr64;
    pAesdmaPri->bEnableAesCtr64 = FALSE;
    pAesdmaPri->fpAESDMASecureKeyIdx = _MDrv_AESDMA_SetSecuredKeyIndex;
    pAesdmaPri->u8SecureKeyIndex = 1;
    pAesdmaPri->u32miunum = 0;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AESDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    PAESDMA_INIT pInitParam = NULL;
    PAESDMA_FILE_INOUT pFileInOutParam = NULL;
    PAESDMA_SEL_ENG pSelEngParam = NULL;
    PAESDMA_SET_PS pSetPSParam = NULL;
    PAESDMA_NOTIFY pNotifyParam = NULL;
    PAESDMA_RAND pRandParam = NULL;
    PRSA_CALCULATE pRSACalParam = NULL;
    PRSA_OUTPUT pRSAOutParam = NULL;
    PSHA_CALCULATE pSHACalParam = NULL;
    PSHA_CALCULATE_MANUAL pSHACalManualParam = NULL;
    PAESDMA_PARSER_CONFIGURATIONS pParserConfigurations = NULL;
    PAESDMA_PARSER_PID_CONFIGURATIONS pParserPidConfigurations = NULL;

    MS_U32 u32Ret = 0;
    MS_BOOL bEnable = 0;
    AESDMA_INSTANT_PRIVATE* psAesInstPri = NULL;
    AESDMA_RESOURCE_SHARED* psAesResourceShared = NULL;
    RSA_RESOURCE_SHARED* psRSAResourceShared = NULL;
    PHDCP_CIPHER psHDCPData = NULL;
    PHDCP_CIPHER_STATE psHDCPCipherState = NULL;
    PAESDMA_KEY_INDEX psAESDMAKeyIndex = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void*)&psAesInstPri);
    void* pResource = NULL;
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    MS_VIRT u32Addr = 0;
    MS_U32 u32ShmId = 0;
    MS_U32 u32BufSize = 0;
    MS_VIRT u32IVarrayAddr = 0;
    MS_VIRT u32KeyarryAddr = 0;
    MS_U32 u32timeout_count = 0;
    MS_U32 u32timeout_value = 0;

    switch(u32Cmd)
    {
        case MDrv_CMD_AESDMA_Init:
            {
                pInitParam = (PAESDMA_INIT)pArgs;
                u32Ret = (psAesInstPri->fpAESDMAInit)(pInitParam->u32miu0addr,pInitParam->u32miu1addr,pInitParam->u32miunum);

                psAesInstPri->u32miu0addr = pInitParam->u32miu0addr;
                psAesInstPri->u32miu1addr = pInitParam->u32miu1addr;
                psAesInstPri->u32miunum = pInitParam->u32miunum;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]u32miu0addr = %lx\n",psAesInstPri->u32miu0addr));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]u32miu1addr = %lx\n",psAesInstPri->u32miu1addr));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]u32miunum = %lx\n",psAesInstPri->u32miunum));

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetIV:
            {
                memcpy((void *)psAesInstPri->InitVector,(void *)pArgs,16);
                psAesInstPri->bSetIV = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_Reset:
            {
                u32Ret = (psAesInstPri->fpAESDMAReset)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetFileInOut:
            {
                pFileInOutParam = (PAESDMA_FILE_INOUT)pArgs;
                psAesInstPri->u32FileinAddr = pFileInOutParam->u32FileinAddr;
                psAesInstPri->u32FileInNum = pFileInOutParam->u32FileInNum;
                psAesInstPri->u32FileOutSAddr = pFileInOutParam->u32FileOutSAddr;
                psAesInstPri->u32FileOutEAddr = pFileInOutParam->u32FileOutEAddr;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]FileinAddr = %lx\n",psAesInstPri->u32FileinAddr));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]FileInNum = %lx\n",psAesInstPri->u32FileInNum));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]FileOutSAddr = %lx\n",psAesInstPri->u32FileOutSAddr));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]FileOutEAddr = %lx\n",psAesInstPri->u32FileOutEAddr));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetKey:
            {
                memcpy((void *)psAesInstPri->CipherKey,(void *)pArgs,16);
                psAesInstPri->bSetKey = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SelEng:
            {
                pSelEngParam = (PAESDMA_SEL_ENG)pArgs;
                psAesInstPri->eMode = pSelEngParam->eMode;
                psAesInstPri->bDescrypt = pSelEngParam->bDescrypt;
				psAesInstPri->bSetEng = TRUE;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]mode = %x\n",psAesInstPri->eMode));
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]Descrypt = %x\n",psAesInstPri->bDescrypt));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_Start:
            {
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_AES, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        ULOGE(TAG_AESDMA, "[AESDMA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(AESDMA_RESOURCE_SHARED));
                }

                psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;

                psAesResourceShared->pAESResource = pResource;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[AESDMA INFO]pResource = %x\n",(MS_U32)psAesResourceShared->pAESResource));

                //AESDMA reset
                if ((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_DES_CTR)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_TDES_CTR))
                {
                    (psAesInstPri->fpAESDMANormalReset)();
                }
                else
                {
                    (psAesInstPri->fpAESDMAReset)();
                }

                //AESDMA set mode and select encrypt or decrypt
                if (psAesInstPri->bSetEng == TRUE)
                {
                    //AESDMA set mode and select encrypt or decrypt
                    (psAesInstPri->fpAESDMASelEng)(psAesInstPri->eMode,psAesInstPri->bDescrypt);
                    psAesInstPri->bSetEng = FALSE;
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bEnableTwoKey==1)
                {
                    u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherOddKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_ODD_KEY);

                    u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_EVEN_KEY);
                }
                else
                {
                    if(psAesInstPri->bSecretKey==1)
                    {
                        (psAesInstPri->fpAESDMASecureKeyIdx)(psAesInstPri->u8SecureKeyIndex);
                    }
                    else
                    {
                        u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherKey[0];
                        (psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                    }
                }

                //AESDMA set IV
                if((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_TDES_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    u32IVarrayAddr = (MS_VIRT)&psAesInstPri->InitVector[0];
                    (psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);
                    psAesInstPri->bSetIV = FALSE;


                    if(psAesInstPri->bEnableTwoKey==1)
                    {
                        u32IVarrayAddr = (MS_VIRT)&psAesInstPri->InitVectorOdd[0];
                        (psAesInstPri->fpAESDMASetOddIv)((MS_U32*)u32IVarrayAddr);
                    }
                }

                // HW Parser configurations
                if (psAesInstPri->bHwParserEn == TRUE)
                {
                    psAesInstPri->bHwParserEn = FALSE;
                    if (psAesInstPri->bParserDecrypt == TRUE)
                    {
                        (psAesInstPri->fpAESDMAParserDecrypt)(psAesInstPri->eParserMode);
                    }
                    else
                    {
                        (psAesInstPri->fpAESDMAParserEncrypt)(psAesInstPri->eParserMode);
                    }

                    if (psAesInstPri->bEvenScrmbPatternEn == TRUE)
                    {
                        psAesInstPri->bEvenScrmbPatternEn = FALSE;
                        (psAesInstPri->fpAESDMAParserSetScrmbPatten)(psAesInstPri->eScrmbPattern);
                    }

                    if (psAesInstPri->bOddScrmbPatternEn == TRUE)
                    {
                        psAesInstPri->bOddScrmbPatternEn = FALSE;
                        (psAesInstPri->fpAESDMAParserSetAddedScrmbPattern)(psAesInstPri->eAddedScrmbPattern);
                    }

                    if (psAesInstPri->bMaskScrmbEn == TRUE)
                    {
                        psAesInstPri->bMaskScrmbEn = FALSE;
                        (psAesInstPri->fpAESDMAParserMaskscrmb)(psAesInstPri->bMaskScrmb);
                    }

                    // Set PID
                    if (psAesInstPri->bBypassPid == TRUE)
                    {
                        psAesInstPri->bBypassPid = FALSE;
                        (psAesInstPri->fpAESDMAParserBypassPid)(psAesInstPri->bBypassPid);
                    }

                    if (psAesInstPri->bPid0En == TRUE)
                    {
                        psAesInstPri->bPid0En = FALSE;
                        (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex, psAesInstPri->u16Pid);
                    }

                    if (psAesInstPri->bPid1En == TRUE)
                    {
                        psAesInstPri->bPid1En = FALSE;
                        (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex1, psAesInstPri->u16Pid1);
                    }
                }
                //CBC segmental decryption case
                //if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                //    && psAesInstPri->bDescrypt == TRUE)
                if((((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))&& psAesInstPri->bDescrypt == TRUE)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR))
                {
                    MS_U32 i;
                    MS_VIRT FileinVirtAddr;

                    FileinVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileinAddr);

                    if(psAesInstPri->eMode != E_DRVAESDMA_CIPHER_CTR)
                    {
                        for(i = 0; i < 16; i++)
                        {
                            psAesInstPri->InitVector[i] = *(MS_U8 *)(FileinVirtAddr + psAesInstPri->u32FileInNum-1-i);
                            MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]Init Vector[%d] = 0x%x\n",(MS_U32)i, psAesInstPri->InitVector[i]));
                        }
                    }
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bSecretKey==1)
                {
                    (psAesInstPri->fpAESDMASecureKeyIdx)(psAesInstPri->u8SecureKeyIndex);
                }
                else
                {
                    u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherKey[0];
                    (psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                }

                //Set Key Index
                if(psAesInstPri->bEnableKeyIndexEvenKey == TRUE)
                {
                    psAesInstPri->bEnableKeyIndexEvenKey = FALSE;
                    (psAesInstPri->fpAESDMAKeyIndex) (psAesInstPri->u32KeyIndexEven, FALSE);
                }

                if(psAesInstPri->bEnableKeyIndexOddKey == TRUE)
                {
                    psAesInstPri->bEnableKeyIndexOddKey = FALSE;
                    (psAesInstPri->fpAESDMAKeyIndex) (psAesInstPri->u32KeyIndexOdd, TRUE);
                }

                //Set AESDMA CTR 64 Enable
                psAesInstPri->fpAesCtr64 (psAesInstPri->bEnableAesCtr64);

                //AESDMA trigger start
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_GetStatus:
            {
                if ((psAesInstPri->fpAESDMAGetStatus)(&u32Ret) == UTOPIA_STATUS_SUCCESS)
                {
                    *(MS_U32 *)pArgs = u32Ret;
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }
        case MDrv_CMD_AESDMA_IsFinished:
            {
                if(pArgs)
                {
                    u32timeout_value = *(MS_U32 *)pArgs;
                }
                else
                {
                    ULOGE(TAG_AESDMA, "[AESDMA INFO] Please pass timeout value to driver\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

                if ((u32timeout_value < 1) | (u32timeout_value > AES_MAX_TIMEOUT_VALUE))
                {
                    ULOGE(TAG_AESDMA, "[AESDMA INFO] Invalid timeout value\n");
                    return UTOPIA_STATUS_ERR_INV;
                }


                if (FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    ULOGE(TAG_AESDMA, "[AESDMA INFO]query Resouce shared memory fail\n");
                    return UTOPIA_STATUS_ERR_NOMEM;
                }
                else
                {
                    psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;
                    pResource = psAesResourceShared->pAESResource;
                    MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[AESDMA INFO]pResource = %x\n",(MS_U32)psAesResourceShared->pAESResource));
                }

                psAesInstPri->fpAESDMAIsFinished(&u32Ret);
                while (u32Ret != DRVAESDMA_OK)
				{
					MsOS_DelayTaskUs(10);
					u32timeout_count++;

					//timeout
					if(u32timeout_count>u32timeout_value)
					{
						(psAesInstPri->fpAESDMAReset)();
                        UtopiaResourceRelease(pResource);
                        ULOGE(TAG_AESDMA, "[AESDMA ERROR] AESDMA timeout happened\n");
						return UTOPIA_STATUS_FAIL;
					}
                    psAesInstPri->fpAESDMAIsFinished(&u32Ret);
				}

                //CBC segmental encryption case
                if(((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||(psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                    && psAesInstPri->bDescrypt == FALSE)
                {
                    //printf("[AESDMA]FileOutStartAddr PA = %x\n",psAesInstPri->u32FileOutSAddr);
                    //printf("[AESDMA]Descrypt = %x\n",psAesInstPri->bDescrypt);
                    MS_U32 i;
                    MS_VIRT FileOutStartVirtAddr;

                    FileOutStartVirtAddr = MsOS_PA2KSEG1(psAesInstPri->u32FileOutSAddr);

                    for(i = 0; i < 16; i++)
                    {
                        psAesInstPri->InitVector[i] = *(MS_U8 *)(FileOutStartVirtAddr+psAesInstPri->u32FileInNum-1-i);
                        MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]Init Vector[%d] = 0x%x\n",(MS_U32)i, psAesInstPri->InitVector[i]));
                    }
                }

                //clear secret key setting
                psAesInstPri->bSecretKey = 0;

                UtopiaResourceRelease(pResource);
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]MDrv_AESDMA_IsFinished\n"));
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetClk:
            {
                bEnable = *(MS_BOOL *)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetClk)(bEnable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_SetSecureKey:
            {
                psAesInstPri->bSecretKey = 1;
                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_SetPS:
            {
                pSetPSParam = (PAESDMA_SET_PS)pArgs;
                u32Ret = (psAesInstPri->fpAESDMASetPS)(pSetPSParam->u32PTN,pSetPSParam->u32Mask,pSetPSParam->bPSin_Enable,pSetPSParam->bPSout_Enable);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_PSRelease:
            {
                u32Ret = (psAesInstPri->fpAESDMAPSRelease)();
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedByteCNT:
            {
                (psAesInstPri->fpAESDMAGetPSMatchedByteCNT)(&u32Ret);
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_GetPSMatchedPTN:
            {
                u32Ret = (psAesInstPri->fpAESDMAGetPSMatchedPTN)();
                return u32Ret;
            }
        case MDrv_CMD_AESDMA_Notify:
            {
                pNotifyParam = (PAESDMA_NOTIFY)pArgs;
                u32Ret = (psAesInstPri->fpAESDMANotify)(pNotifyParam->eEvents,pNotifyParam->pfCallback);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_Rand:
            {
                pRandParam = (PAESDMA_RAND)pArgs;
                u32Ret = (psAesInstPri->fpAESDMARand)(pRandParam->u32PABuf,pRandParam->u32Size);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_Calculate:
            {
                pRSACalParam = (PRSA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_RSA, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        ULOGE(TAG_AESDMA, "[RSA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(RSA_RESOURCE_SHARED));
                }

                psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;

                psRSAResourceShared->pRSAResource = pResource;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[RSA INFO]pResource = %x\n",(MS_U32)psRSAResourceShared->pRSAResource));

                u32Ret = (psAesInstPri->fpRSACalculate)(pRSACalParam->pstSign,pRSACalParam->pstKey,pRSACalParam->eMode);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_RSA_IsFinished:
            {
                if(pArgs)
                {
                    u32timeout_value = *(MS_U32 *)pArgs;
                }
                else
                {
                    ULOGE(TAG_AESDMA, "[RSA INFO] Please pass timeout value to driver\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

                if (u32timeout_value > RSA_MAX_TIMEOUT_VALUE)
                {
                    ULOGE(TAG_AESDMA, "[RSA INFO] Invalid timeout value\n");
                    return UTOPIA_STATUS_ERR_INV;
                }

                if (FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    ULOGE(TAG_AESDMA, "[RSA INFO]query Resouce shared memory fail\n");
                    return UTOPIA_STATUS_ERR_NOMEM;
                }
                else
                {
                    psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
                    pResource = psRSAResourceShared->pRSAResource;
                    MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[RSA INFO]pResource = %x\n",(MS_U32)psRSAResourceShared->pRSAResource));
                }

                while ((psAesInstPri->fpRSAIsFinished)() != DRVAESDMA_OK)
				{
					MsOS_DelayTaskUs(10);
					u32timeout_count++;

					//timeout
					if(u32timeout_count>u32timeout_value)
					{
                        UtopiaResourceRelease(pResource);
                        ULOGE(TAG_AESDMA, "[RSA ERROR] RSA timeout happened\n");
						return UTOPIA_STATUS_FAIL;
					}

				}

                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[RSA INFO]MDrv_RSA_IsFinished\n"));
                return UTOPIA_STATUS_SUCCESS;

            }
        case MDrv_CMD_RSA_Output:
            {
                if (FALSE == MsOS_SHM_GetId((MS_U8*)"RSA driver", sizeof(RSA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    ULOGE(TAG_AESDMA, "[RSA INFO]query Resouce shared memory fail\n");
                    return UTOPIA_STATUS_ERR_NOMEM;
                }
                else
                {
                    psRSAResourceShared = (RSA_RESOURCE_SHARED*)u32Addr;
                    pResource = psRSAResourceShared->pRSAResource;
                    MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[RSA INFO]pResource = %x\n",(MS_U32)psRSAResourceShared->pRSAResource));
                }

                pRSAOutParam = (PRSA_OUTPUT)pArgs;
                while((psAesInstPri->fpRSAIsFinished)()==1);
                u32Ret = (psAesInstPri->fpRSAOutput)(pRSAOutParam->eMode,pRSAOutParam->pstRSAOut);

                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_Calculate:
            {
                pSHACalParam = (PSHA_CALCULATE)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_SHA, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculate)(pSHACalParam->eMode,pSHACalParam->u32PAInBuf,pSHACalParam->u32Size,pSHACalParam->u32PAOutBuf);
                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_AESDMA_Memcpy:
            {
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_AES, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        ULOGE(TAG_AESDMA, "[AESDMA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(AESDMA_RESOURCE_SHARED));
                }

                psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;

                psAesResourceShared->pAESResource = pResource;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[AESDMA INFO]pResource = %x\n",(MS_U32)psAesResourceShared->pAESResource));

                //AESDMA reset
                (psAesInstPri->fpAESDMAReset)();

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA trigger start for using AESDMA replace BDMA case
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);

                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }
        case MDrv_CMD_SHA_CalculateManual:
            {
                pSHACalManualParam = (PSHA_CALCULATE_MANUAL)pArgs;
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_SHA, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }
                u32Ret = (psAesInstPri->fpSHACalculateManual)(pSHACalManualParam->stCfg,pSHACalManualParam->eStage,pSHACalManualParam->u32DoneBytes,pSHACalManualParam->pu8SetIV);
                UtopiaResourceRelease(pResource);
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }


        case MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserConfigurations = (PAESDMA_PARSER_CONFIGURATIONS)pArgs;
                psAesInstPri->eAddedScrmbPattern = pParserConfigurations->eAddedScrmbPattern;
                psAesInstPri->bOddScrmbPatternEn = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAesInstPri->fpAESDMAParserQueryPidCount(pArgs);

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_MASKSCRMB:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                bEnable = *(MS_BOOL *)pArgs;
                psAesInstPri->bMaskScrmb = bEnable;
                psAesInstPri->bMaskScrmbEn = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserConfigurations = (PAESDMA_PARSER_CONFIGURATIONS)pArgs;
                psAesInstPri->eScrmbPattern = pParserConfigurations->eScrmbPattern;
                psAesInstPri->bEvenScrmbPatternEn = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_BYPASSPID:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserPidConfigurations = (PAESDMA_PARSER_PID_CONFIGURATIONS)pArgs;
                psAesInstPri->bBypassPid = pParserPidConfigurations->bBypassPid;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_SETPID:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserPidConfigurations = (PAESDMA_PARSER_PID_CONFIGURATIONS)pArgs;

                if (pParserPidConfigurations->u8PidIndex == 0)
                {
                    psAesInstPri->u8PidIndex = pParserPidConfigurations->u8PidIndex;
                    psAesInstPri->u16Pid = pParserPidConfigurations->u16Pid;
                    psAesInstPri->bPid0En = TRUE;
                }
                else if (pParserPidConfigurations->u8PidIndex == 1)
                {
                    psAesInstPri->u8PidIndex1 = pParserPidConfigurations->u8PidIndex;
                    psAesInstPri->u16Pid1 = pParserPidConfigurations->u16Pid;
                    psAesInstPri->bPid1En = TRUE;
                }

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_ENCRYPT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserConfigurations = (PAESDMA_PARSER_CONFIGURATIONS)pArgs;
                psAesInstPri->eParserMode = pParserConfigurations->eParserMode;
                psAesInstPri->bParserDecrypt = FALSE;
                psAesInstPri->bHwParserEn = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_DECRYPT:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                pParserConfigurations = (PAESDMA_PARSER_CONFIGURATIONS)pArgs;
                psAesInstPri->eParserMode = pParserConfigurations->eParserMode;
                psAesInstPri->bParserDecrypt = TRUE;
                psAesInstPri->bHwParserEn = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_PARSER_Start:
            {
                if(UtopiaResourceObtain(pModule, AESDMA_POOL_ID_AES, &pResource) != 0)
                {
                    ULOGE(TAG_AESDMA, "UtopiaResourceObtainToInstant fail\n");
                    return UTOPIA_STATUS_ERR_NOT_AVAIL;
                }

                if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY))
                {
                    if(FALSE == MsOS_SHM_GetId((MS_U8*)"AESDMA driver", sizeof(AESDMA_RESOURCE_SHARED), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE))
                    {
                        ULOGE(TAG_AESDMA, "[AESDMA INFO]create Resouce shared memory fail\n");
                        return UTOPIA_STATUS_ERR_NOMEM;
                    }
                    memset( (MS_U8*)u32Addr, 0, sizeof(AESDMA_RESOURCE_SHARED));
                }

                psAesResourceShared = (AESDMA_RESOURCE_SHARED*)u32Addr;

                psAesResourceShared->pAESResource = pResource;
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "\n[AESDMA INFO]pResource = %x\n",(MS_U32)psAesResourceShared->pAESResource));

                if ((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_DES_CTR)||
                    (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_TDES_CTR))
                {
                    (psAesInstPri->fpAESDMANormalReset)();
                }
                else
                {
                    //AESDMA reset
                    (psAesInstPri->fpAESDMAReset)();
                }

                if (psAesInstPri->bSetEng == TRUE)
                {
                    //AESDMA set mode and select encrypt or decrypt
                    (psAesInstPri->fpAESDMASelEng)(psAesInstPri->eMode,psAesInstPri->bDescrypt);
                    psAesInstPri->bSetEng = FALSE;
                }

                //AESDMA set FileInOut info
                (psAesInstPri->fpAESDMASetFileInOut)(psAesInstPri->u32FileinAddr,psAesInstPri->u32FileInNum,psAesInstPri->u32FileOutSAddr,psAesInstPri->u32FileOutEAddr);

                //AESDMA set key
                if(psAesInstPri->bEnableTwoKey==1)
                {
                    u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherOddKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_ODD_KEY);

                    u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherKey[0];
                    (psAesInstPri->fpAESDMASetKeySel)((MS_U32*)u32KeyarryAddr, E_DRVAESDMA_EVEN_KEY);
                }
                else
                {
                    if(psAesInstPri->bSecretKey==1)
                    {
                        (psAesInstPri->fpAESDMASecureKeyIdx)(psAesInstPri->u8SecureKeyIndex);
                    }
                    else
                    {
                        u32KeyarryAddr = (MS_VIRT)&psAesInstPri->CipherKey[0];
                        (psAesInstPri->fpAESDMASetKey)((MS_U32*)u32KeyarryAddr);
                    }
                }

                //AESDMA set IV
                if((psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CBC)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTR)||
                   (psAesInstPri->eMode == E_DRVAESDMA_CIPHER_CTS_CBC))
                {
                    u32IVarrayAddr = (MS_VIRT)&psAesInstPri->InitVector[0];
                    if (psAesInstPri->bSetIV == TRUE)
                    {
                        (psAesInstPri->fpAESDMASetIV)((MS_U32*)u32IVarrayAddr);
                        psAesInstPri->bSetIV = FALSE;
                    }

                    if(psAesInstPri->bEnableTwoKey==1)
                    {
                        u32IVarrayAddr = (MS_VIRT)&psAesInstPri->InitVectorOdd[0];
                        (psAesInstPri->fpAESDMASetOddIv)((MS_U32*)u32IVarrayAddr);
                    }

                }

                // HW Parser configurations
                if (psAesInstPri->bHwParserEn == TRUE)
                {
                    psAesInstPri->bHwParserEn = FALSE;
                }
                if (psAesInstPri->bParserDecrypt == TRUE)
                {
                    (psAesInstPri->fpAESDMAParserDecrypt)(psAesInstPri->eParserMode);
                }
                else
                {
                    (psAesInstPri->fpAESDMAParserEncrypt)(psAesInstPri->eParserMode);
                }

                if (psAesInstPri->bEvenScrmbPatternEn == TRUE)
                {
                    psAesInstPri->bEvenScrmbPatternEn = FALSE;
                    (psAesInstPri->fpAESDMAParserSetScrmbPatten)(psAesInstPri->eScrmbPattern);
                }

                if (psAesInstPri->bOddScrmbPatternEn == TRUE)
                {
                    psAesInstPri->bOddScrmbPatternEn = FALSE;
                    (psAesInstPri->fpAESDMAParserSetAddedScrmbPattern)(psAesInstPri->eAddedScrmbPattern);
                }

                (psAesInstPri->fpAESDMAParserMaskscrmb)(psAesInstPri->bMaskScrmb);

                // Set PID
                if (psAesInstPri->bBypassPid == TRUE)
                {
                    psAesInstPri->bBypassPid = FALSE;
                    (psAesInstPri->fpAESDMAParserBypassPid)(psAesInstPri->bBypassPid);
                }

                if (psAesInstPri->bPid0En == TRUE)
                {
                    psAesInstPri->bPid0En = FALSE;
                    (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex, psAesInstPri->u16Pid);
                }
                if (psAesInstPri->bPid1En == TRUE)
                {
                    psAesInstPri->bPid1En = FALSE;
                    (psAesInstPri->fpAESDMAParserSetPid)(psAesInstPri->u8PidIndex1, psAesInstPri->u16Pid1);
                }

                // AESDMA trigger start
                u32Ret = (psAesInstPri->fpAESDMAStart)(1);
                MS_DEBUG_MSG(ULOGD(TAG_AESDMA, "[AESDMA INFO]MDrv_CMD_AESDMA_PARSER_Start\n"));
                if (u32Ret == DRVAESDMA_OK)
                    return UTOPIA_STATUS_SUCCESS;
                else
                    return UTOPIA_STATUS_FAIL;
            }

        case MDrv_CMD_AESDMA_SetOddKey:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                memcpy((void *)psAesInstPri->CipherOddKey,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_ENABLE_Two_Key:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                bEnable = *(MS_BOOL *)pArgs;
                psAesInstPri->bEnableTwoKey = bEnable;

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_SetOddIV:
            {
                memcpy((void *)psAesInstPri->InitVectorOdd,(void *)pArgs,16);
                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                if ( psAesInstPri->fpAESDMAIsSecretKeyInNormalBlank(pArgs) == DRVAESDMA_OK )
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }


        case MDrv_CMD_HDCP_MDrv_PROCESSCIPHER:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psHDCPData = (HDCP_CIPHER *)pArgs;

                if ( psAesInstPri->fpHDCPHDCPProcessCipher(psHDCPData->u8Idx, (void*)psHDCPData->pu8Riv, (void*)psHDCPData->pu8ContentKey) == DRVAESDMA_OK )
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }

        case MDrv_CMD_HDCP_GetHdcpCipherState:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psHDCPCipherState = (HDCP_CIPHER_STATE *)pArgs;

                if ( psAesInstPri->fpGetHDCPCipherState(psHDCPCipherState->u8Idx, psHDCPCipherState->pu8State) == DRVAESDMA_OK )
                {
                    return UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    return UTOPIA_STATUS_FAIL;
                }
            }

        case MDrv_CMD_AESDMA_Set_Key_Index:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAESDMAKeyIndex = (AESDMA_KEY_INDEX *)pArgs;

                if (psAESDMAKeyIndex-> bOddKeyEn == TRUE)
                {
                    psAesInstPri->bEnableKeyIndexOddKey = TRUE;
                    psAesInstPri->u32KeyIndexOdd = psAESDMAKeyIndex->u32KeyIndex;
                }
                else
                {
                    psAesInstPri->bEnableKeyIndexEvenKey = TRUE;
                    psAesInstPri->u32KeyIndexEven = psAESDMAKeyIndex->u32KeyIndex;
                }

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_Set_AES_CTR64_En:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                bEnable = *(MS_BOOL *)pArgs;
                psAesInstPri->bEnableAesCtr64 = bEnable;

                return UTOPIA_STATUS_SUCCESS;
            }

        case MDrv_CMD_AESDMA_Set_Efuse_Key_Index:
            {
                if (pArgs == NULL)
                    return UTOPIA_STATUS_FAIL;

                psAesInstPri->bSecretKey = 1;
                psAesInstPri->u8SecureKeyIndex = *(MS_U8 *)pArgs;

                return UTOPIA_STATUS_SUCCESS;
            }
        case MDrv_CMD_AESDMA_GetConfig:
            {
                if (psAesInstPri->u32miunum == 0){
                    return UTOPIA_STATUS_FAIL;
                }
                pInitParam = (PAESDMA_INIT) pArgs;

                pInitParam->u32miu0addr = psAesInstPri->u32miu0addr;
                pInitParam->u32miu1addr = psAesInstPri->u32miu1addr;
                pInitParam->u32miunum = psAesInstPri->u32miunum;

                return UTOPIA_STATUS_SUCCESS;
            }
        default:
            break;
    };

	return 0;
}

MS_U32 AESDMAClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return 0;
}

