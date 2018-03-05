
#include <stdlib.h>
#include <string.h>
#include "MsCommon.h"

#include "nsk2hdi_bl.h"
#include "nsk_dbg.h"
#include "drvCIPHER.h"
#include "nsk2hdi_header.h"

/*****************************************************************************\
  \Definitions
\*****************************************************************************/
#define SHA1_SIZE   (20)
#define SHA256_SIZE (32)
#define MD5_SIZE    (16)

#define MaximumHashSize  SHA256_SIZE



/*****************************************************************************\
  \ typedef struct
\*****************************************************************************/


typedef struct _bl_capability_descriptor {
    NDS_UBYTE       descriptor_tag;         // NSK2HDI_BL_CAPABILITY_DESC_TAG
    NDS_UBYTE       len;
    NDS_UBYTE       max_data_size[4];
    NDS_UBYTE       min_data_size[4];
    NDS_UBYTE       data_size_granularity[4];
    NDS_UBYTE       data_alignment[4];
} BL_CAPABILITY_DESCRIPTOR;

typedef struct _bl_hashing_algorithm_descriptor {
    NDS_UBYTE       descriptor_tag;         // NSK2HDI_BL_ALGORITHM_DESC_TAG
    NDS_UBYTE       len;
    NDS_UBYTE       algorithm;
    //NDS_UBYTE       hash_block_size[2];
} BL_HASHING_ALGORITHM_DESCRIPTOR;

typedef struct _bl_desc {
    BL_CAPABILITY_DESCRIPTOR            m_cap;
    BL_HASHING_ALGORITHM_DESCRIPTOR     m_algo;
} BL_DESC;



/*****************************************************************************\
  \ local variables
\*****************************************************************************/


static BL_CAPABILITY_DESCRIPTOR m_cap_des = {
    .descriptor_tag       = NSK2HDI_BL_CAPABILITY_DESC_TAG,
    .len                  = sizeof(BL_CAPABILITY_DESCRIPTOR) - 2,
    .max_data_size[0] = 0,
    .max_data_size[1] = 0,
    .max_data_size[2] = 0x80,
    .max_data_size[3] = 0,
    .min_data_size[0] = 0x01,
    .data_size_granularity[0] = 0x10,
    .data_alignment[0] = 0x10,
};


static BL_HASHING_ALGORITHM_DESCRIPTOR m_algo_des[3] = {
    {
        .descriptor_tag = NSK2HDI_BL_ALGORITHM_DESC_TAG,
        .len            = sizeof(BL_HASHING_ALGORITHM_DESCRIPTOR) - 2,
        .algorithm      = NSK2HDI_BL_SHA1_HASH_ALG_TYPE,
        //.hash_block_size[0] = 0x0,
        //.hash_block_size[1] = 0x1,

    },

    {
        .descriptor_tag = NSK2HDI_BL_ALGORITHM_DESC_TAG,
        .len            = sizeof(BL_HASHING_ALGORITHM_DESCRIPTOR) - 2,
        .algorithm      = NSK2HDI_BL_SHA256_HASH_ALG_TYPE,
        //.hash_block_size[0] = 0x0,
        //.hash_block_size[1] = 0x1,

    },

    {
        .descriptor_tag = NSK2HDI_BL_ALGORITHM_DESC_TAG,
        .len            = sizeof(BL_HASHING_ALGORITHM_DESCRIPTOR) - 2,
        .algorithm      = NSK2HDI_BL_MD5_HASH_ALG_TYPE,
        //.hash_block_size[0] = 0x0,
        //.hash_block_size[1] = 0x1,

    },

};


static BL_HASHING_ALGORITHM_DESCRIPTOR ghash_algo;
static NDS_ULONG ghash_data_size;  // IN
static NDS_UBYTE *gphash_data;   // IN

static MS_U8 HASH_OUTPUT[MaximumHashSize];
static MS_U8 HASH_Middle_OUTPUT[MaximumHashSize];
static MS_U32 HASH_ACCU = 0;
static NDS_UBYTE manual_mode = FALSE;

static CIPHER_HASH_STAGE operation_stage = E_CIPHER_HASH_STAGE_FIRST;

/*****************************************************************************\
  \ global variables
\*****************************************************************************/


/*****************************************************************************\
  \ NSK2HDI bootloader related functions
\*****************************************************************************/


static void _DumpBuf(const MS_U8 *buf, MS_U32 len)
{
    MS_U32 i = 0;
    for (i = 0; i < len; i++) {
        printf("0x%02x%c", buf[i], ((i % 16) == 15) ? '\n' : ' ');
    }
    printf("\n");
}

NSK2HDI_STATUS NSK2HDI_CalHASH_Auto(NDS_ULONG           data_size,      // IN
                                    const NDS_UBYTE     *data,          // IN
                                    NDS_ULONG           *digest_size,   // IN/OUT
                                    NDS_UBYTE           *digest,
                                    NDS_ULONG           algorithm )
{
    DRV_CIPHER_HASHCFG cipher_cfg;
    DRV_CIPHER_RET ret;
    NDS_ULONG OutputSize = SHA1_SIZE;
    MS_U32 u32CmdId,u32Exception;

    NSK_TRACE(("Enter\n"));

    _DumpBuf(data,data_size);

    memset(&cipher_cfg, 0x0, sizeof(DRV_CIPHER_HASHCFG));

    cipher_cfg.u32CAVid = 0xf;
    if(NSK2HDI_BL_SHA1_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = SHA1_SIZE;
    }
    else if(NSK2HDI_BL_SHA256_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = SHA256_SIZE;
    }
    else if(NSK2HDI_BL_MD5_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_MD5;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = MD5_SIZE;
    }

    NSK_TRACE(("eAlgo = %d, OutputSize = %x\n",cipher_cfg.eAlgo,OutputSize));

    MS_U8 *pTestInput, *pTestOutput;
    pTestInput = MApi_NSK2_AllocateMemory(data_size, FALSE);
    pTestOutput = MApi_NSK2_AllocateMemory(OutputSize, FALSE);

    memcpy(pTestInput,data,data_size);

    cipher_cfg.stInput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestInput);
    cipher_cfg.stInput.u32Size = data_size;
    cipher_cfg.pu8Digest_Buf = (MS_U8 *)pTestOutput;


    ret = MDrv_CIPHER_HASH(cipher_cfg, &u32CmdId);

    if(ret != 0)
    {
        NSK_ERROR(("HASH FAIL!!!\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
    {
        MsOS_DelayTask(20);
    }

    memcpy(HASH_OUTPUT,pTestOutput,OutputSize);

    _DumpBuf(HASH_OUTPUT, OutputSize);

    memcpy(digest, HASH_OUTPUT, OutputSize);
    *digest_size = OutputSize;

    MApi_NSK2_FreeMemory(pTestInput, FALSE);
    MApi_NSK2_FreeMemory(pTestOutput, FALSE);


    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}


NSK2HDI_STATUS NSK2HDI_CalHASH_Manual( NDS_ULONG           data_size,      // IN
                                       const NDS_UBYTE     *data,          // IN
                                       NDS_ULONG           *digest_size,   // IN/OUT
                                       NDS_UBYTE           *digest,
                                       NDS_ULONG           algorithm,
                                       CIPHER_HASH_STAGE   stage)
{

    NSK_TRACE(("Enter\n"));

    DRV_CIPHER_HASHCFG_MANUAL cipher_cfg;
    DRV_CIPHER_RET     ret = DRV_CIPHER_OK;
    NDS_ULONG          OutputSize = SHA1_SIZE;
    MS_U32             u32CmdId,u32Exception;

    memset(&cipher_cfg, 0, sizeof(cipher_cfg));

    if(NSK2HDI_BL_SHA1_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA1;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = SHA1_SIZE;
    }
    else if(NSK2HDI_BL_SHA256_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_SHA256;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = SHA256_SIZE;
    }
    else if(NSK2HDI_BL_MD5_HASH_ALG_TYPE == algorithm)
    {
        cipher_cfg.eAlgo = E_CIPHER_HASH_ALGO_MD5;
        cipher_cfg.u32Digest_Buf_Size = OutputSize = MD5_SIZE;
    }

    MS_U8 *pTestInput, *pTestOutput, *pTestMidOutput;
    pTestInput = MApi_NSK2_AllocateMemory(data_size, FALSE);
    pTestOutput = MApi_NSK2_AllocateMemory(OutputSize, FALSE);
    pTestMidOutput = MApi_NSK2_AllocateMemory(OutputSize, FALSE);

    memcpy(pTestMidOutput,HASH_Middle_OUTPUT,OutputSize);
    memcpy(pTestInput,data,data_size);

    cipher_cfg.u32CAVid = 0xf;

    cipher_cfg.stInput.u32Addr = MsOS_VA2PA((MS_VIRT)pTestInput);
    cipher_cfg.stInput.u32Size = data_size;
    cipher_cfg.pu8Digest_Buf = (MS_U8 *)pTestOutput;

    NSK_TRACE(("stage = %d, data_size = %x\n",stage, data_size));
    NSK_TRACE((" HASH_ACCU = %d\n",HASH_ACCU));

#if 0
    if(data_size == 1)
    {
        printf("final data = %x\n",*data);
    }
#endif

    if(stage == E_CIPHER_HASH_STAGE_FIRST)
    {
        NSK_TRACE(("E_CIPHER_HASH_STAGE_FIRST\n"));
        cipher_cfg.eStage = E_CIPHER_HASH_STAGE_FIRST;
        cipher_cfg.u32StartByte = 0;
        cipher_cfg.pu8SetIV = NULL;
        cipher_cfg.eIVMode = E_CIPHER_HASH_IV_FIPS;
        ret = MDrv_CIPHER_HASHManual(cipher_cfg, &u32CmdId);

        while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
        {
            MsOS_DelayTask(20);
        }

        HASH_ACCU += data_size;
    }
    else if (stage == E_CIPHER_HASH_STAGE_UPDATE)
    {
        NSK_TRACE(("E_CIPHER_HASH_STAGE_UPDATE\n"));
        cipher_cfg.eStage = E_CIPHER_HASH_STAGE_UPDATE;
        cipher_cfg.u32StartByte = HASH_ACCU;
        cipher_cfg.pu8SetIV = (MS_U8 *)pTestMidOutput;
        cipher_cfg.eIVMode = E_CIPHER_HASH_IV_CMD;

        ret = MDrv_CIPHER_HASHManual(cipher_cfg, &u32CmdId);

        while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
        {
            MsOS_DelayTask(20);
        }

        HASH_ACCU += data_size;
    }
    else if (stage == E_CIPHER_HASH_STAGE_LAST)
    {
        NSK_TRACE(("E_CIPHER_HASH_STAGE_LAST\n"));
        cipher_cfg.eStage = E_CIPHER_HASH_STAGE_LAST;
        cipher_cfg.u32StartByte = HASH_ACCU;
        cipher_cfg.pu8SetIV = (MS_U8 *)pTestMidOutput;
        cipher_cfg.eIVMode = E_CIPHER_HASH_IV_CMD;

        ret = MDrv_CIPHER_HASHManual(cipher_cfg, &u32CmdId);

        while(FALSE == MDrv_CIPHER_IsHASHDone(u32CmdId,&u32Exception))
        {
            MsOS_DelayTask(20);
        }

        if(digest != NULL)
        {
            memcpy(HASH_OUTPUT,pTestOutput,OutputSize);
            memcpy(digest, HASH_OUTPUT, OutputSize);
        }
        else
        {
            NSK_ERROR(("the return buffer is null!!!\n"));
        }

        if(digest_size != NULL)
        {
            *digest_size = OutputSize;
        }
        else
        {
            NSK_ERROR(("the return buffer size is null!!!\n"));
        }
    }

    NSK_TRACE(("ret = %d\n",ret));

    if(ret != 0)
    {
        NSK_ERROR(("HASH FAIL!!!\n"));
        return NSK2HDI_STATUS_FAILED;
    }

    memcpy(HASH_Middle_OUTPUT, pTestOutput, OutputSize);

    MApi_NSK2_FreeMemory(pTestInput, FALSE);
    MApi_NSK2_FreeMemory(pTestOutput, FALSE);
    MApi_NSK2_FreeMemory(pTestMidOutput, FALSE);

    NSK_TRACE(("Exit\n"));

    return NSK2HDI_STATUS_OK;
}


//this function is used to query the capabilities of the provided hashing engine.
//out--> BL Capability descriptor, BL Algorithm descriptor
NSK2HDI_STATUS NSK2HDI_BL_GetHashCapabilities(
                                        NDS_ULONG   request_id,     // IN
                                        NDS_ULONG  *desc_size,      // IN/OUT
                                        NDS_UBYTE  *desc            // OUT
                                        )
{
    NSK_TRACE(("Enter\n"));

    if (request_id != NSK2HDI_BL_ALL_DESCRIPTORS_REQUEST)
    {
        NSK_ERROR(("Invalid TAG:%x", request_id));
        return NSK2HDI_STATUS_INVALID_REQUEST;
    }
    NSK_PRINT(("desc_size:%d \n", *desc_size ));

    if(desc == NULL)
    {
        *desc_size = sizeof(BL_CAPABILITY_DESCRIPTOR) + 3 * sizeof(BL_HASHING_ALGORITHM_DESCRIPTOR) ;
        NSK_TRACE(("return desc_size = %x\n",*desc_size));
        return NSK2HDI_STATUS_OK;
    }


    memcpy(desc,&m_cap_des, sizeof(BL_CAPABILITY_DESCRIPTOR));

    memcpy( (desc + sizeof(BL_CAPABILITY_DESCRIPTOR)) ,&m_algo_des[0], 3 * sizeof(BL_HASHING_ALGORITHM_DESCRIPTOR) );

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

//this function shall initialize a hashing operation.
NSK2HDI_STATUS NSK2HDI_BL_InitializeHashOperation(
                                                  NDS_ULONG desc_size,  // IN
                                                  NDS_UBYTE *desc       // IN
                                                  )
{
    NSK_TRACE(("Enter\n"));
    NSK_TRACE(("%s desc_size:%d\n", __FUNCTION__, desc_size));

    MS_U32 size = desc_size;
    MS_U8  *pBL_buf = (MS_U8*)desc;
    MS_U8 u8size;
    MS_U8 TagSwitch;
    MS_U8 algorithm;

    while(size>=3)
    {
        TagSwitch = pBL_buf[0];
        u8size = pBL_buf[1];

        switch(TagSwitch)
        {
            case NSK2HDI_BL_ALGORITHM_DESC_TAG:
                NSK_TRACE(("NSK2HDI_BL_ALGORITHM_DESC_TAG\n"));
                algorithm = pBL_buf[2];
                NSK_TRACE(("algorithm = %x\n",algorithm));
                ghash_algo.algorithm = algorithm;
                //memcpy(&ghash_algo, pBL_buf, u8size);
                break;

            case NSK2HDI_BL_CAPABILITY_DESC_TAG:
                NSK_TRACE(("NSK2HDI_BL_CAPABILITY_DESC_TAG\n"));
                break;

             default:
                NSK_ERROR(("unknown desc tag\n"));
                break;
        }

        pBL_buf += 3;
        size -= 3;
    }

    memset(HASH_OUTPUT, 0x0, MaximumHashSize);
    memset(HASH_Middle_OUTPUT, 0x0, MaximumHashSize);
    operation_stage = E_CIPHER_HASH_STAGE_FIRST;

    HASH_ACCU = 0;
    manual_mode = FALSE;
    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}

//this fcuntion shall performa a hash operation on a chunk of data, or continue a
//previous operation. This function shall return only upon completion of the operation.
//Other threads may not call any other API from this document during this opration.
NSK2HDI_STATUS NSK2HDI_BL_PerformHashOperation(
                                        NDS_ULONG           data_size,  // IN
                                        const NDS_UBYTE     *data       // IN
                                        )
{
    NSK_TRACE(("Enter\n"));
    printf ("%s data_size:%d\n", __FUNCTION__, data_size);
    MS_U8 algorithm;

    algorithm = ghash_algo.algorithm;

    NSK_TRACE(("algorithm = %x\n",algorithm));

    ghash_data_size = data_size;
    gphash_data = (NDS_UBYTE *)data;

    if( (NSK2HDI_BL_SHA1_HASH_ALG_TYPE != algorithm) &&
        (NSK2HDI_BL_SHA256_HASH_ALG_TYPE != algorithm) &&
        (NSK2HDI_BL_MD5_HASH_ALG_TYPE == algorithm) )
    {
        NSK_TRACE(("algorithm not support\n"));
    }

    NSK2HDI_CalHASH_Manual(data_size, data, NULL, NULL, algorithm, operation_stage);
    manual_mode = TRUE;
    if(operation_stage == E_CIPHER_HASH_STAGE_FIRST)
    {
        operation_stage = E_CIPHER_HASH_STAGE_UPDATE;
    }

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}




//this function completes a hash operation.
//out-->digest_size, actual size in bytes of the hash digest written to buffer.
//out-->digest, Buufer with the resulting hash digest.
NSK2HDI_STATUS NSK2HDI_BL_CompleteHashOperation(
                                        NDS_ULONG           data_size,      // IN
                                        const NDS_UBYTE     *data,          // IN
                                        NDS_ULONG           *digest_size,   // IN/OUT
                                        NDS_UBYTE           *digest         // OUT
                                        )
{
    NSK_TRACE(("Enter\n"));
    NDS_ULONG algorithm;

    algorithm = ghash_algo.algorithm;

    NSK_TRACE(("%s data_size:0x%x\n", __FUNCTION__, data_size));
    NSK_TRACE(("algorithm = %x\n",algorithm));

    if(manual_mode == TRUE)
    {
        NSK2HDI_CalHASH_Manual(data_size, data, digest_size, digest, algorithm, E_CIPHER_HASH_STAGE_LAST);
    }
    else
    {
        NSK2HDI_CalHASH_Auto(data_size, data, digest_size, digest, algorithm);
    }

    //*digest_size = output_size;

    NSK_TRACE(("Exit\n"));
    return NSK2HDI_STATUS_OK;
}




