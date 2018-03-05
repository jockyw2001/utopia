

#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"

#include "nsk_dbg.h"

#include "nsk_282.h"
#include "nsk_dbg.h"
#include "nsk_3014.h"
#include "hdi_121.h"
#include "drvCIPHER.h"
#include "drvDSCMB.h"
#include "drvNSK2.h"
#include "nsk2hdi_header.h"



/*****************************************************************************\
  \Definitions
\*****************************************************************************/
#define NSK2_M2MCHANNEL_NUM             16
#define M2MChannelCookieSize            0x1000
#define NSK2_M2MChannelGroup_Number     2
#define NSK2_M2MChannel_Number          2

#define ChangeGetM2MProp

/*****************************************************************************\
  \ typedef struct
\*****************************************************************************/

typedef struct
{
    MS_U32          u32CMHandle;
    MS_U8           Algorithm;
    MS_U8           Operation;
    MS_U8           clearkey[0x100];
    MS_U8           clearkey_length;
    MS_U8           en_swKeys;
} M2MChannelDevice_Cookie_t;

typedef struct _NSK2_M2MTable
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
    MS_U32                          handle_id;
} NSK2_M2MTable_Info;

typedef struct _DMA_iv1_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 iv_payload[18];
}DMA_IV1_Desc_t;


typedef struct _DMA_iv2_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 iv_payload[18];
}DMA_IV2_Desc_t;

typedef struct _DMA_operation_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 operation;
}DMA_Operation_Desc_t;


typedef struct _DMA_memory_descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 type;
}DMA_Memory_Desc_t;

typedef struct _NSK2_DMAInfo
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
    MS_U32                          handle_id;
} NSK2_DMA_Info;


typedef struct {
    MS_U8   Algorithm;
    MS_U8   Operation;
    MS_U32  size;
    MS_U8   *pInput;
    MS_U8   *pOutput;
    MS_U8   IV1[0x10];
    MS_U32  u32IV[0x4];
    MS_U8   IV2[0x10];
    MS_U8   clearkey[0x100];
    MS_U8   clearkey_length;
    MS_U8   en_swKeys;
} M2MConfig_t;


typedef struct _M2MChannel_ClearSWKey_Descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 *pClearSWKey;
} M2MChannel_ClearSWKey_Desc_t;

typedef struct _NSK2_M2MChannelGroup
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
    MS_U32                          handle_id;
} NSK2_M2MChannelGroup_Info;

typedef struct _M2MChannel_Operation_Descriptor{
    MS_U8 descriptor_tag;
    MS_U8 descriptor_length;
    MS_U8 operation;
} M2MCh_Operation_Desc_t;

typedef struct _NSK2_M2MChannel
{
    MS_BOOL                         alloc;
    MS_BOOL                         enable;
    MS_U32                          nsk_sw;
    MS_U32                          handle_id;
} NSK2_M2MChannel_Info;


/*****************************************************************************\
  \ local variables
\*****************************************************************************/


static DMA_IV1_Desc_t dma_iv1_desc = {
    .descriptor_tag = NSK2HDI_DMA_IV1_DESC_TAG,
    .descriptor_length = sizeof(DMA_IV1_Desc_t) - 2,
};

static DMA_IV2_Desc_t dma_iv2_desc = {
    .descriptor_tag = NSK2HDI_DMA_IV2_DESC_TAG,
    .descriptor_length = sizeof(DMA_IV2_Desc_t) - 2,
};



/*****************************************************************************\
  \ global variables
\*****************************************************************************/
extern void *pM2MChannelcookie[NSK2_M2MCHANNEL_NUM];

M2MChannelDevice_Cookie_t *gpM2MChCookie;
NSK2_M2MTable_Info nsk_m2mTable;
NSK2_DMA_Info nsk2_dma;
NSK2_M2MChannel_Info nsk_m2m[NSK2_M2MChannel_Number];
NSK2_M2MChannelGroup_Info nsk_m2mgrp[NSK2_M2MChannelGroup_Number];

/*****************************************************************************\
  \ NSK2HDI m2m functions
\*****************************************************************************/

/*****************************************************************************\
  \ M2M devices - Table device
\*****************************************************************************/


NSK2HDI_STATUS NSKHDI_M2MTable_Open (NSK2HDI_DEVICE_ID M2MTable_ID,
                                     NSK2HDI_HANDLE    *M2MTable_handle)
{
    NSK_TRACE(("Enter\n"));
    NSK_TRACE(("M2MTable_ID = %x\n",M2MTable_ID));

    nsk_m2mTable.alloc = TRUE;
    nsk_m2mTable.handle_id = 0;

    *M2MTable_handle = (NSK2HDI_HANDLE *)&nsk_m2mTable.handle_id;

    NSK_TRACE(("M2MTable_handle = 0x%x\n",nsk_m2mTable.handle_id));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_M2MTable_Close (NSK2HDI_HANDLE M2MTable_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(M2MTable_handle);
    NSK_TRACE(("u32Handle = 0x%x\n",u32Handle));

    nsk_m2mTable.alloc = FALSE;
    nsk_m2mTable.handle_id = 0;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_M2MTable_EnumerateChildren( NSK2HDI_HANDLE       M2MTable_handle,
                                                  NDS_ULONG            *child_device_num,
                                                  NSK2HDI_CHILD_DEVICE *child_device)
{
    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("child_device_num = %x\n",*child_device_num));

    if(child_device == NULL)
    {
        //should fill the number of child device...
        *child_device_num = 2;
        return NSK2HDI_STATUS_OK;
    }

    child_device[0].device_id = 0x1234;
    child_device[0].device_type = NSK2HDI_DMA_DEVICE_TYPE;
    child_device[1].device_id = 0x5678;
    child_device[1].device_type = NSK2HDI_M2MCHANNEL_GROUP_DEVICE_TYPE;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ M2M devices - DMA device
\*****************************************************************************/

NSK2HDI_STATUS NSKHDI_DMADevice_Open (NSK2HDI_DEVICE_ID DMADevice_ID,
                                      NSK2HDI_HANDLE    *DMADevice_handle)
{

    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("DMADevice_ID = %x\n",DMADevice_ID));

    if(*DMADevice_handle != NULL)
    {
        //NSK_ERROR(("CmChannele_handle no NULL\n"));
    }

    if(nsk2_dma.alloc == TRUE)
    {
        //NSK_ERROR(("DMA is used\n"));
    }

    nsk2_dma.alloc = TRUE;
    *DMADevice_handle = (NSK2HDI_HANDLE *)&nsk2_dma.handle_id;

    NSK_TRACE(("DMADevice_handle = 0x%x\n",nsk2_dma.handle_id));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_DMADevice_Close (NSK2HDI_HANDLE DMADevice_handle)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 u32Handle = NSKHDI_GetHandle(DMADevice_handle);
    //NSK_TRACE(("DMADevice_handle = %x\n",u32Handle));
    NSK_TRACE(("DMADevice_handle = 0x%x\n",u32Handle));

    nsk2_dma.alloc = FALSE;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}



NSK2HDI_STATUS NSKHDI_DMADevice_GetProperties( NSK2HDI_HANDLE  DMADevice_handle,
                                               NDS_ULONG       request_id,
                                               NDS_ULONG       *desc_size,
                                               NDS_UBYTE       *desc)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(DMADevice_handle);
    NSK_TRACE(("DMADevice_handle = 0x%x\n",u32Handle));

    if(request_id != NSK2HDI_DMA_READ_ALL_DESCRIPTORS_REQUEST)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    if(desc == NULL)
    {
        MDrv_NSK2_GetDMAProperties((MS_U32*)desc_size, (MS_U8*)desc);
        NSK_TRACE(("return desc_size = 0x%x\n",(MS_U32)*desc_size));
        return NSK2HDI_STATUS_OK;
    }

    if(*desc_size == 0)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    MDrv_NSK2_GetDMAProperties((MS_U32*)desc_size, (MS_U8*)desc);
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

void NSKHDI_M2MAlgoTrans(MS_U8 NSK2HDI_Algo, DRV_CIPHER_ALGO *pCipherAlgo)
{

    NSK_TRACE(("NSK2HDI_Algo = %x\n",NSK2HDI_Algo));

    if(NSK2HDI_SPROFILE_M2M_DES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_DES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_ECB;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_DES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV1;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV2 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_DES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV2;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_DES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_TDES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_ECB;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_TDES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV1;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV2 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_TDES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV2;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_TDES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_ECB;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_IV1 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CTS;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV1;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CTS;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_IV1;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_SCTE52;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CBC;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_RC4_64 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_RC4;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_NONE;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CTR == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CTR;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CTR64 == NSK2HDI_Algo)
    {
        pCipherAlgo->eMainAlgo = E_CIPHER_MAIN_AES;
        pCipherAlgo->eSubAlgo  = E_CIPHER_SUB_CTR;
        pCipherAlgo->eResAlgo  = E_CIPHER_RES_CLR;
        pCipherAlgo->eSBAlgo   = E_CIPHER_SB_CLR;
    }
    else
    {
        NSK_ERROR(("Not Implement\n"));
    }
}


void NSKHDI_M2MGetNSKSubAlgo(MS_U8 NSK2HDI_Algo, CIPHER_SUB_ALGO *pSubAlgo)
{
#define SUBALGO_ECB  0
#define SUBALGO_CBC  1
#define SUBALGO_CTR  2
#define SUBALGO_CBCMAC 3
#define SUBALGO_CTR_64 4
#define SUBALGO_CMAC_KEY 5
#define SUBALGO_CMAC_Algo 6
#define SUBALGO_PCBC_ADD 7
#define SUBALGO_PCBC_XOR 8

    NSK_TRACE(("NSK2HDI_Algo = 0x%02x\n",NSK2HDI_Algo));

    if(NSK2HDI_SPROFILE_M2M_DES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_ECB;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_SCTE52_IV2 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_DES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_ECB;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_SCTE52_IV2 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_TDES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_ECB_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_ECB;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_IV1 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CTS_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_IV1 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_SCTE52_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CBC_CLR_CLR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CBC;
    }
    else if(NSK2HDI_SPROFILE_M2M_RC4_64 == NSK2HDI_Algo)
    {
        *pSubAlgo = 0;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CTR == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CTR;
    }
    else if(NSK2HDI_SPROFILE_M2M_AES_CTR64 == NSK2HDI_Algo)
    {
        *pSubAlgo = SUBALGO_CTR_64;
    }
    else
    {
        NSK_ERROR(("Not Implement\n"));
    }

}


//#define IV_FROM_MAGIC

void NSKHDI_WriteM2MTest(M2MConfig_t *pM2MConfig)
{

	MS_BOOL bDecrpt = FALSE;
	DRV_CIPHER_RET    ret;
	MS_U32            u32CmdId;
	DRV_CIPHER_DMACFG stCfg;
    MS_U32 u32DelayTime;

    memset(&stCfg,0x0,sizeof(DRV_CIPHER_DMACFG));

    if (pM2MConfig->Operation == NSK2HDI_M2MCHANNEL_DECRYPT_OPERATION)
    {
        NSK_TRACE(("DECRYPT_OPERATION\n"));
        bDecrpt = TRUE;
    }
    else if (pM2MConfig->Operation == NSK2HDI_M2MCHANNEL_ENCRYPT_OPERATION)
    {
        NSK_TRACE(("ENCRYPT_OPERATION\n"));
        bDecrpt = FALSE;
    }
    else
    {
        NSK_ERROR(("Not support operation\n"));
    }


    NSKHDI_M2MAlgoTrans(pM2MConfig->Algorithm,&stCfg.stAlgo);

    MS_U8 u8IVData[0x10];
    MS_U8 i;

    memset(u8IVData,0x0,0x10);
#ifdef IV_FROM_MAGIC
    MS_U8 u8IVData1[0x10] = {0xbd, 0x98, 0xa2, 0x06, 0x1b, 0x76, 0x1b, 0xa4, 0x15, 0x10, 0x1f, 0x26, 0xf6, 0x66, 0x6, 0x45};
    MS_U8 u8IVData2[0x10] = {0x91, 0x0c, 0xc0, 0x9f, 0x9a, 0x04, 0x7b, 0x6a, 0xa8, 0xde, 0xfe, 0xe2, 0xfc, 0xae, 0xad, 0xb3};
    memcpy(pM2MConfig->IV1,u8IVData1,0x10);
    memcpy(pM2MConfig->IV2,u8IVData2,0x10);
#endif

    NSKHDI_IVDataTrans(pM2MConfig->Algorithm,pM2MConfig->IV1,pM2MConfig->IV2,pM2MConfig->u32IV);

    if(pM2MConfig->en_swKeys == TRUE)
    {
        NSK_TRACE(("Write SW key\n"));
        stCfg.u32CAVid = 2;
        stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CPU;
        stCfg.stKey.u8KeyIdx = 0;
        stCfg.stKey.u8KeyLen = pM2MConfig->clearkey_length;
        stCfg.stKey.pu8KeyData = (MS_U8 *)pM2MConfig->clearkey;
        stCfg.stKey.u8IVLen = 16;

        for(i=0;i<4;i++)
        {
            u8IVData[i*4+0] = (MS_U8)((pM2MConfig->u32IV[i]>>24)&0xff);
            u8IVData[i*4+1] = (MS_U8)((pM2MConfig->u32IV[i]>>16)&0xff);
            u8IVData[i*4+2] = (MS_U8)((pM2MConfig->u32IV[i]>>8)&0xff);
            u8IVData[i*4+3] = (MS_U8)((pM2MConfig->u32IV[i]>>0)&0xff);
        }

        stCfg.stKey.pu8IVData = (MS_U8 *)u8IVData;
    }
    else
    {
        stCfg.u32CAVid = 1;
        NSK_TRACE(("WriteM2MKey with NSK write key\n"));

        //for nds usage....
        stCfg.stAlgo.eSubAlgo = E_CIPHER_SUB_NONE;
        stCfg.stAlgo.eMainAlgo = E_CIPHER_MAIN_NONE;
    	stCfg.stAlgo.eSBAlgo   = E_CIPHER_SB_NONE;
        stCfg.stAlgo.eResAlgo  = E_CIPHER_RES_NONE;

    	//Key, NDS
    	stCfg.stKey.eKeySrc = E_CIPHER_KSRC_CAIP;
    	stCfg.stKey.u8KeyIdx = 0;
    	stCfg.stKey.u8KeyLen = 0;
    	stCfg.stKey.pu8KeyData = 0;

        stCfg.stKey.u8IVLen = 16;


        //(MS_U8*)pM2MConfig->u32IV;
        //pu32IV[3] = 0xf6660645;
        //pu32IV[2] = 0x15101f26;
        //pu32IV[1] = 0x1b761ba4;
        //pu32IV[0] = 0xbd98a206;
        for(i=0;i<4;i++)
        {
            u8IVData[i*4+0] = (MS_U8)((pM2MConfig->u32IV[i]>>24)&0xff);
            u8IVData[i*4+1] = (MS_U8)((pM2MConfig->u32IV[i]>>16)&0xff);
            u8IVData[i*4+2] = (MS_U8)((pM2MConfig->u32IV[i]>>8)&0xff);
            u8IVData[i*4+3] = (MS_U8)((pM2MConfig->u32IV[i]>>0)&0xff);
        }
        stCfg.stKey.pu8IVData = u8IVData;

    }

    u32DelayTime = pM2MConfig->size >> 10 ; //100k byte, 1ms

    if(u32DelayTime < 100)
        u32DelayTime = 1;
    else
        u32DelayTime /= 100;

    //Decrpt or Encrypt
    stCfg.bDecrypt = bDecrpt;

    if( MApi_NSK2_IsHarmonizer() == TRUE )//running harmonizer test....
    {
        MS_U8 *pTestInput, *pTestOutput;
        pTestInput = MApi_NSK2_AllocateMemory(pM2MConfig->size, FALSE);
        pTestOutput = MApi_NSK2_AllocateMemory(pM2MConfig->size, FALSE);

        memcpy(pTestInput, pM2MConfig->pInput,pM2MConfig->size);
        //InData
    	stCfg.stInput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestInput);
    	stCfg.stInput.u32Size = pM2MConfig->size;

        NSK_TRACE(("Input Addr = 0x%llx, u32Size = 0x%x\n", stCfg.stInput.u32Addr, stCfg.stInput.u32Size));

        //OutData
        stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestOutput);
        stCfg.stOutput.u32Size = pM2MConfig->size;

        NSK_TRACE(("Output Addr = 0x%llx, u32Size = 0x%x\n",stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size));

        MsOS_FlushMemory();

        MS_U32   u32Exception=0;

        ret = MDrv_CIPHER_DMACalc(stCfg, &u32CmdId); //use hal code?
        if(ret != DRV_CIPHER_OK)
        {
    	    NSK_ERROR(("Fail to M2M Test\n"));
            return;
        }

        do
        {
            MsOS_DelayTask(u32DelayTime);
            ret = MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception);
        }while(ret == FALSE);


        NSK_TRACE(("DMA done\n"));
        MsOS_ReadMemory();

        if(u32Exception)
        {
            NSK_TRACE(("DMA Exception: [0x%x]\n", u32Exception));
        }

        memcpy(pM2MConfig->pOutput, pTestOutput,pM2MConfig->size);

        MApi_NSK2_FreeMemory(pTestInput, FALSE);
        MApi_NSK2_FreeMemory(pTestOutput, FALSE);
    }
    else  //normal case..., real project, not harmonizer one....
    {
        MS_U8 *pTestInput, *pTestOutput;
        pTestInput = pM2MConfig->pInput;
        pTestOutput = pM2MConfig->pOutput;

        //InData
        stCfg.stInput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestInput);
        stCfg.stInput.u32Size = pM2MConfig->size;

        NSK_TRACE(("Input Addr = 0x%llx, u32Size = 0x%x\n",stCfg.stInput.u32Addr, stCfg.stInput.u32Size));

        //OutData
        stCfg.stOutput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestOutput);
        stCfg.stOutput.u32Size = pM2MConfig->size;

        NSK_TRACE(("Output Addr = 0x%llx, u32Size = 0x%x\n",stCfg.stOutput.u32Addr, stCfg.stOutput.u32Size));

        MsOS_FlushMemory();

        MS_U32           u32Exception=0;

        ret = MDrv_CIPHER_DMACalc(stCfg, &u32CmdId); //use hal code?
        if(ret != DRV_CIPHER_OK)
        {
            NSK_ERROR(("Fail to M2M Test\n"));
            return;
        }

        do
        {
            MsOS_DelayTask(u32DelayTime);
            ret = MDrv_CIPHER_IsDMADone(u32CmdId, &u32Exception);
        }while(ret == FALSE);

        NSK_TRACE(("DMA done\n"));
        MsOS_ReadMemory();

        if(u32Exception)
        {
            NSK_TRACE(("DMA Exception: [0x%x]\n", u32Exception));
        }
    }
}

NSK2HDI_STATUS NSKHDI_DMADevice_WriteData( NSK2HDI_HANDLE  DMADevice_handle,
                                           void            *M2MChannel_cookie,
                                           NDS_ULONG       desc_size,
                                           const NDS_UBYTE *desc,
                                           NDS_ULONG       data_size,
                                           NDS_UBYTE       *in_data,
                                           NDS_UBYTE       *out_data)
{

    MS_U8 *pDesc = (MS_U8 *)desc;
    MS_U8 desc_tag;
    NDS_ULONG   size;
    MS_U8 TagSwitch;
    MS_U8 descriptor_length;
    MS_U8 operation;
    MS_U8 type;
    M2MConfig_t tM2MConfig;
    MS_U8 M2MAlgorithm, M2MOpration;

    M2MChannelDevice_Cookie_t *pM2MChCookie = (M2MChannelDevice_Cookie_t *)M2MChannel_cookie;

    memset(&tM2MConfig,0x0,sizeof(M2MConfig_t));

    M2MAlgorithm = tM2MConfig.Algorithm = pM2MChCookie->Algorithm;
    M2MOpration  = tM2MConfig.Operation = pM2MChCookie->Operation;
    tM2MConfig.en_swKeys = pM2MChCookie->en_swKeys;
    tM2MConfig.pInput = in_data;
    tM2MConfig.pOutput = out_data;
    tM2MConfig.size  = data_size;
    tM2MConfig.clearkey_length = pM2MChCookie->clearkey_length;
    memcpy(tM2MConfig.clearkey, pM2MChCookie->clearkey, tM2MConfig.clearkey_length);

    NSK_TRACE(("Enter , M2MChannel_cookie = 0x%p\n", M2MChannel_cookie));

    NSK_TRACE(("desc_size = %x,data_size=%x\n",desc_size,data_size));
    NSK_TRACE(("in_data = 0x%p,out_data=0x%p\n",in_data,out_data));
    NSK_TRACE(("M2MAlgorithm = 0x%x, M2MOpration = 0x%x\n",M2MAlgorithm,M2MOpration));

    desc_tag = *pDesc;
    size = desc_size;
    pDesc = (MS_U8*)desc;

    while(size>=3)
    {
        TagSwitch = pDesc[0] ;
        descriptor_length = pDesc[1];

        switch(TagSwitch)
        {
            case NSK2HDI_DMA_IV1_DESC_TAG:
                NSK_TRACE(("NSK2HDI_DMA_IV1_DESC_TAG, descriptor_length = 0x%02x\n",descriptor_length));
                memcpy(dma_iv1_desc.iv_payload,&pDesc[2],descriptor_length);
                memcpy(tM2MConfig.IV1,&pDesc[2],descriptor_length);
            #if 0
                printf("NSK2HDI_DMA_IV1, ");
                for(i=0;i<descriptor_length;i++)
                {
                    printf("%x, ",tM2MConfig.IV1[i]);
                }
                printf("\n");
            #endif
                break;

            case NSK2HDI_DMA_IV2_DESC_TAG:
                NSK_TRACE(("NSK2HDI_DMA_IV2_DESC_TAG, descriptor_length = 0x%02x\n",descriptor_length));
                memcpy(dma_iv2_desc.iv_payload,&pDesc[2],descriptor_length);
                memcpy(tM2MConfig.IV2,&pDesc[2],descriptor_length);
            #if 0
                printf("NSK2HDI_DMA_IV2, ");
                for(i=0;i<descriptor_length;i++)
                {
                    printf("%x, ",tM2MConfig.IV2[i]);
                }
                printf("\n");
            #endif
                break;

            case NSK2HDI_DMA_OPERATION_DESC_TAG:
                operation = pDesc[2];
                NSK_TRACE(("NSK2HDI_DMA_OPERATION_DESC_TAG, operation = 0x%02x\n",operation));
                break;

            case NSK2HDI_DMA_SRC_MEMORY_TYPE_DESC_TAG:
                type = pDesc[2];
                NSK_TRACE(("NSK2HDI_DMA_SRC_MEMORY_TYPE_DESC_TAG, type = 0x%02x\n",type));
                if(NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE == type)
                {
                    NSK_TRACE(("DMA_SRC_MEMORY NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE\n"));
                }
                break;

            case NSK2HDI_DMA_DST_MEMORY_TYPE_DESC_TAG:
                type = pDesc[2];
                NSK_TRACE(("NSK2HDI_DMA_DST_MEMORY_TYPE_DESC_TAG, type = 0x%02x\n",type));
                if(NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE == type)
                {
                    NSK_TRACE(("DMA_DST_MEMORY NSK2HDI_DMA_CONTIGUOUS_MEMORY_TYPE\n"));
                }
                break;

            default:
                NSK_ERROR(("unknown desc tag\n"));
                break;
        }
        size -= (descriptor_length+2);
        pDesc += (descriptor_length+2);
    }


    //memcpy(out_data,in_data,data_size);

    NSKHDI_WriteM2MTest(&tM2MConfig);

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ M2M devices - Channle Group device
\*****************************************************************************/

NSK2HDI_STATUS NSKHDI_M2MChannelGroup_Open (NSK2HDI_DEVICE_ID M2MChannelGroup_ID,
                                            NSK2HDI_HANDLE    *M2MChannelGroup_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 i;

    NSK_TRACE(("M2MChannelGroup_ID = %x\n",M2MChannelGroup_ID));

    if(*M2MChannelGroup_handle != NULL)
    {
        NSK_ERROR(("M2MChannelGroup_handle no NULL\n"));
    }

    for(i = 0; i < NSK2_M2MChannelGroup_Number; i++)
    {
        if (nsk_m2mgrp[i].alloc == FALSE)
        {
            break;
        }
    }
    if (i == NSK2_M2MChannelGroup_Number)
    {
        return (NSK2HDI_STATUS_FAILED);
    }

    nsk_m2mgrp[i].alloc = TRUE;
    nsk_m2mgrp[i].handle_id = i;

    *M2MChannelGroup_handle = (NSK2HDI_HANDLE *)&nsk_m2mgrp[i].handle_id;

    NSK_TRACE(("M2MChannelGroup_handle = 0x%x\n",nsk_m2mgrp[i].handle_id));

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_M2MChannelGroup_Close (NSK2HDI_HANDLE M2MChannelGroup_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannelGroup_handle);

    nsk_m2mgrp[u32Handle].alloc = FALSE;
    nsk_m2mgrp[u32Handle].handle_id = 0;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_M2MChannelGroup_GetProperties( NSK2HDI_HANDLE  M2MChannelGroup_handle,
                                                     NDS_ULONG       request_id,
                                                     NDS_ULONG       *desc_size,
                                                     NDS_UBYTE       *desc)
{
    NSK_TRACE(("Enter\n"));

    if(request_id != NSK2HDI_M2MCHANNELGROUP_READ_ALL_DESCRIPTORS_REQUEST)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }

    if(desc == NULL)
    {
        MDrv_NSK2_GetM2MProperties((MS_U32*)desc_size, (MS_U8*)desc);
        return NSK2HDI_STATUS_OK;
    }

    if(*desc_size == 0)
    {
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }


    MDrv_NSK2_GetM2MProperties((MS_U32*)desc_size, (MS_U8*)desc);
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

/*****************************************************************************\
  \ M2M devices - Channle device
\*****************************************************************************/

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Open (NSK2HDI_DEVICE_ID M2MChannelGroup_ID,
                                             NSK2HDI_HANDLE    *M2MChannel_handle)
{
    NSK_TRACE(("Enter\n"));

    NSK_TRACE(("M2MChannelGroup_ID = %x\n",M2MChannelGroup_ID));

    MS_U32 i;

    if(*M2MChannel_handle != NULL)
    {
        NSK_ERROR(("CmChannele_handle no NULL\n"));
    }

    for(i = 0; i < NSK2_M2MChannel_Number; i++)
    {
        if (nsk_m2m[i].alloc == FALSE)
        {
            break;
        }
    }
    if (i == NSK2_M2MChannel_Number)
    {
        return (NSK2HDI_STATUS_FAILED);
    }

    nsk_m2m[i].alloc = TRUE;
    nsk_m2m[i].handle_id = i;

    *M2MChannel_handle = (NSK2HDI_HANDLE *)&nsk_m2m[i].handle_id;

    NSK_TRACE(("M2MChannel_handle = 0x%x\n",nsk_m2m[i].handle_id));
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Close (NSK2HDI_HANDLE M2MChannel_handle)
{
    NSK_TRACE(("Enter\n"));
    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannel_handle);

    nsk_m2m[u32Handle].alloc = FALSE;
    nsk_m2m[u32Handle].handle_id = 0;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSKHDI_M2MChannelDevice_GetCookie(NSK2HDI_HANDLE M2MChannele_handle,
                                                 void **cookie)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannele_handle);

    *cookie = pM2MChannelcookie[u32Handle];

    NSK_TRACE(("Exit, cookie = %p\n",*cookie));
    return NSK2HDI_STATUS_OK;
}


/*	configure cookie according to given descriptor.
	later cookie will be used to set m2m params @ WriteData		*/
NSK2HDI_STATUS NSKHDI_M2MChannelDevice_Configure (NSK2HDI_HANDLE  M2MChannele_handle,
                                                  NDS_ULONG       desc_size,
                                                  const NDS_UBYTE *desc)
{
    NSK_TRACE(("Enter\n"));

    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannele_handle);

    MS_U8 *pDesc = (MS_U8 *)desc;
    NDS_ULONG   size;
    MS_U8 TagSwitch;
    MS_U8 descriptor_length;
    MS_U8 operation;
    MS_U8 algorithm;

    gpM2MChCookie = (M2MChannelDevice_Cookie_t *)pM2MChannelcookie[u32Handle];

    NSK_TRACE(("gpM2MChCookie = 0x%p\n", gpM2MChCookie));

    size = desc_size;
    pDesc = (MS_U8*)desc;

    while(size>=3)
    {
        TagSwitch = pDesc[0] ;
        descriptor_length = pDesc[1];

        switch(TagSwitch)
        {
            case NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG:
                algorithm = pDesc[2];
                NSK_TRACE(("NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG, algorithm = 0x%02x\n",algorithm));
                gpM2MChCookie->Algorithm = algorithm;
                break;

            case NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG:
                operation = pDesc[2];
                NSK_TRACE(("NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG, operation = 0x%02x\n",operation));
                gpM2MChCookie->Operation = operation;
                break;

            default:
                NSK_ERROR(("unknown desc tag\n"));
                break;
        }
        size -= (descriptor_length+2);
        pDesc += (descriptor_length+2);
    }

#if 0
    if(desc != NULL)
    {
        M2MCh_Operation_Desc_t *pM2MChOp = (M2MCh_Operation_Desc_t *)desc;

        NSK_TRACE(("desc_size = %x, tag = %x , operation = %x\n",desc_size, pM2MChOp->descriptor_tag, pM2MChOp->operation));

    #if 0
        if(pM2MChOp->descriptor_tag == NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG)
        {
            NSK_TRACE(("set algorithm = %d\n",pM2MChOp->operation));
        }
        else if (pM2MChOp->descriptor_tag == NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG)
        {
            NSK_TRACE(("set operation = %d\n",pM2MChOp->operation));
        }

        //#define NSK2HDI_M2MCHANNEL_DECRYPT_OPERATION        0x01U /*!< @brief Data chunk descrambling operation */
        //#define NSK2HDI_M2MCHANNEL_ENCRYPT_OPERATION        0x02U /*!< @brief Data chunk scrambling operation */
    #endif
        gpM2MChCookie->u32Algorithm = pM2MChOp->operation;
    }
    else
    {
        NSK_TRACE(("desc == NULL\n"));
    }
#endif
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}



NSK2HDI_STATUS NSKHDI_M2MChannelDevice_WriteM2MKey(NSK2HDI_HANDLE  M2MChannele_handle,
                                                   NDS_ULONG       desc_size,
                                                   const NDS_UBYTE *desc)
{
    NSK_TRACE(("Enter\n"));
//    MS_U32 i;
    MS_U8 *pDesc = (MS_U8 *)desc;
    NDS_ULONG   size;
    MS_U8 TagSwitch;
    MS_U8 descriptor_length;
    MS_U8 operation;
    MS_U8 algorithm;

    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannele_handle);
    M2MChannelDevice_Cookie_t *pM2MChCookie = (M2MChannelDevice_Cookie_t *)pM2MChannelcookie[u32Handle];

    if(desc != NULL)
    {
        size = desc_size;
        pDesc = (MS_U8*)desc;

        while(size>=3)
        {
            TagSwitch = pDesc[0] ;
            descriptor_length = pDesc[1];

            switch(TagSwitch)
            {
                case NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG:
                    algorithm = pDesc[2];
                    NSK_TRACE(("NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG, algorithm = %x\n",algorithm));
                    break;

                case NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG:
                    operation = pDesc[2];
                    NSK_TRACE(("NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG, operation = %x\n",operation));
                    break;


                case NSK2HDI_M2MCHANNEL_CLEAR_SWKEY_DESC_TAG:
                    NSK_TRACE(("set clear key\n"));
                    pM2MChCookie->clearkey_length = descriptor_length;
                    memcpy(pM2MChCookie->clearkey, &pDesc[2], descriptor_length);
            #if 0
                    for(i=0;i<pM2MChCookie->clearkey_length;i++)
                    {
                        printf("%x, ",pM2MChCookie->clearkey[i]);
                    }
                    printf("\n");
            #endif
                    pM2MChCookie->en_swKeys = TRUE;
                    break;

                default:
                    NSK_ERROR(("unknown desc tag\n"));
                    break;
            }
            size -= (descriptor_length+2);
            pDesc += (descriptor_length+2);
        }
    }
    else
    {
        NSK_TRACE(("desc == NULL\n"));
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

NSK2HDI_STATUS NSKHDI_M2MChannelDevice_NSKWriteM2MKey(NSK2HDI_HANDLE  M2MChannele_handle,
                                                      NDS_ULONG       desc_size,
                                                      const NDS_UBYTE *desc)
{
    NSK_TRACE(("Enter\n"));

    MS_U8 *pDesc = (MS_U8 *)desc;
    NDS_ULONG   size;
    MS_U8 TagSwitch;
    MS_U8 descriptor_length;
    MS_U8 operation;
    MS_U8 algorithm = 1;

    MS_U32 u32Handle = NSKHDI_GetHandle(M2MChannele_handle);
    M2MChannelDevice_Cookie_t *pM2MChCookie = (M2MChannelDevice_Cookie_t *)pM2MChannelcookie[u32Handle];

    if(desc != NULL)
    {
        M2MCh_Operation_Desc_t *pM2MOp = (M2MCh_Operation_Desc_t *)desc;

        NSK_TRACE(("desc_size = %x, length = %x, operation = %x\n",desc_size,pM2MOp->descriptor_length,pM2MOp->operation));

        size = desc_size;
        pDesc = (MS_U8*)desc;

        while(size>=3)
        {
            TagSwitch = pDesc[0] ;
            descriptor_length = pDesc[1];

            switch(TagSwitch)
            {
                case NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG:
                    algorithm = pDesc[2];
                    NSK_TRACE(("NSK2HDI_M2MCHANNEL_ALGORITHM_DESC_TAG, algorithm = %x\n",algorithm));
                    break;

                case NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG:
                    operation = pDesc[2];
                    NSK_TRACE(("NSK2HDI_M2MCHANNEL_OPERATION_DESC_TAG, operation = %x\n",operation));
                    break;

                default:
                    NSK_ERROR(("unknown desc tag\n"));
                    break;
            }
            size -= (descriptor_length+2);
            pDesc += (descriptor_length+2);
        }

        pM2MChCookie->en_swKeys = FALSE;
        MDrv_NSK21_WriteM2MKey(algorithm);
    }
    else
    {
        NSK_TRACE(("desc == NULL\n"));
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;

}

