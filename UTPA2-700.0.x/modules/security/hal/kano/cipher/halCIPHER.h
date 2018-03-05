////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halCIPHER.h
// @brief  CIPHER HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_CIPHER_H__
#define __HAL_CIPHER_H__

#include "regCIPHER.h"
#include "drvCIPHER.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  CIPHER Software Define
//--------------------------------------------------------------------------------------------------
#define HAL_CIPHER_RESETKEY_TIMEOUT_VALUE (1000UL)

//--------------------------------------------------------------------------------------------------
//  CIPHER Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

#define HAL_CRYPTODMA_KEYLEN_MAX    16UL
#define HAL_CRYPTODMA_DIRDATA_MAX   16UL
#define HAL_CRYPTODMA_OTPHASH_UNIT  16UL
#define HAL_CRYPTODMA_OTPHASH_SIZE_MIN  32UL
#define HAL_CRYPTODMA_THREAD_ID_MAX 0xFFFFUL
#define HAL_CRYPTODMA_DMA_KEY_SLOT  4UL
#define HAL_CRYPTODMA_OTP_SCK_NUM   4UL
#define HAL_CRYPTODMA_CAVID_MAX     0x1FUL
//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

////////////////////////////////////////////////////////////////////////////////////////////////
// QMEM base address
////////////////////////////////////////////////////////////////////////////////////////////////
#define HAL_CIPHER_BASE_IQMEM            0x00000000UL
#define HAL_CIPHER_BASE_DQMEM            0x80000000UL
#define HAL_CIPHER_BASE_LUT              0x00000010
#define HAL_CIPHER_SIZE_LUT              256
#define HAL_CIPHER_BASE_M                0x00000110
#define HAL_CIPHER_SIZE_M                128
#define HAL_CIPHER_BASE_BC               0x00000190
#define HAL_CIPHER_SIZE_BC               16

////////////////////////////////////////////////////////////////////////////////////////////////
// IRQ
////////////////////////////////////////////////////////////////////////////////////////////////
#define CRYPTODMA_IRQ       E_INT_FIQ_CA_CRYPTO_DMA  //halIRQTBL.h CryptoDMA FIQ 56

////////////////////////////////////////////////////////////////////////////////////////////////
// Except
////////////////////////////////////////////////////////////////////////////////////////////////
#define HAL_CIPHER_EXCEPT_CAVID     0x0001UL
#define HAL_CIPHER_EXCEPT_DATA      0x0002UL
#define HAL_CIPHER_EXCEPT_ALGO      0x0004UL
#define HAL_CIPHER_EXCEPT_DMA_KEY   0x0008UL
#define HAL_CIPHER_EXCEPT_HMAC_KEY  0x0010UL

////////////////////////////////////////////////////////////////////////////////////////////////
// AESDMA Compatible
////////////////////////////////////////////////////////////////////////////////////////////////
#define HAL_CIPHER_KEYSLOT_BASE     0x10UL

typedef enum
{
    E_CIPHER_CAVID1 = 0x0001,
    E_CIPHER_CAVID2 = 0x0002,
    E_CIPHER_CAVID3 = 0x0003,
    E_CIPHER_CAVID4 = 0x0004,
    E_CIPHER_CAVID5 = 0x0005,
    E_CIPHER_CAVID6 = 0x0006,
}HAL_CIPHER_CAVID;

/// SHA Mode
typedef enum
{
    E_HASH_SHA1 = 0,
    E_HASH_SHA256,
    E_HASH_MD5,
}HAL_CIPHER_HASHMODE;

typedef enum
{
    E_DMA_ALGO_NONE = 0,
    E_DMA_ALGO_AES = 1,
    E_DMA_ALGO_DES = 2,
    E_DMA_ALGO_TDES = 3,
    E_DMA_ALGO_M6_S56_CCBC = 4 ,
    E_DMA_ALGO_M6_S56 =5 ,
    E_DMA_ALGO_M6_KE56 = 7 ,
    E_DMA_ALGO_RC4 = 8,
}HAL_CIPHER_ALGO;

typedef enum
{
    E_DMA_MODE_NONE = 0,
    E_DMA_MODE_ECB = 0,
    E_DMA_MODE_CBC,
    E_DMA_MODE_CTR,
    E_DMA_MODE_CBC_MAC,
    E_DMA_MODE_CTR_64,
	E_DMA_MODE_CMAC_Key,
    E_DMA_MODE_CMAC_Algo,
    E_DMA_MODE_PCBC_ADD,
    E_DMA_MODE_PCBC_XOR,
    E_DMA_MODE_OTPHASH,
    E_DMA_MODE_NUM,

}HAL_CIPHER_MODE;

typedef enum
{
    // From KL
    E_DMA_KSEL_SK0       = 0 ,
    E_DMA_KSEL_SK1       = 1 ,
    E_DMA_KSEL_SK2       = 2 ,
    E_DMA_KSEL_SK3       = 3 ,

    // From OTP
    E_DMA_KSEL_MK0       = 4 ,
    E_DMA_KSEL_MK1       = 5 ,
    E_DMA_KSEL_CCCK      = 6 ,
    E_DMA_KSEL_STRN      = 7 ,

    // From CPU
    E_DMA_KSEL_REGKEY,

	//From NSK
    E_DMA_KSEL_CAIP,

}HAL_CIPHER_KEYSRC;

typedef enum
{
    E_DMA_SRC_DIRECT   = 0 ,
    E_DMA_SRC_DRAM     = 1 ,
    E_DMA_SRC_IQMEM    = 2 ,
    E_DMA_SRC_DQMEM    = 2 ,
    E_DMA_SRC_HW_INPUT = 3
}HAL_CIPHER_DATASRC;

typedef enum
{
    E_DMA_DST_DRAM     = 0 ,
    E_DMA_DST_REGFILE  = 1 ,
    E_DMA_DST_IQMEM    = 1 ,
    E_DMA_DST_DQMEM    = 1 ,
}HAL_CIPHER_DATADST;

typedef enum
{
    E_DMA_RESIDUE_NONE   = 0 ,
    E_DMA_RESIDUE_CLR    = 0 ,
    E_DMA_RESIDUE_CTS    = 1 ,
    E_DMA_RESIDUE_SCTE52 = 2 ,
    E_DMA_RESIDUE_NUM ,

}HAL_CIPHER_RESIDUE;

typedef enum
{
    E_DMA_SB_NONE   = 0 ,
    E_DMA_SB_CLR    = 0 ,
    E_DMA_SB_IV1   ,
    E_DMA_SB_IV2  ,
    E_DMA_SB_NUM  ,

}HAL_CIPHER_SHORTBLOCK;

typedef enum
{
    E_DMA_INT_NONE    = 0 ,
    E_DMA_INT_ENABLE  = 1 ,
    E_DMA_INT_EN_WAIT = 2 ,

}HAL_CIPHER_INTMODE;

typedef enum
{
    E_CIPHER_HASH_IWC_PRV = 0,
    E_CIPHER_HASH_IWC_MANUAL,
}HAL_CIPHER_IWCTYPE;

typedef enum
{
    E_CIPHER_TYPE_DMA    = 0 ,
    E_CIPHER_TYPE_SHA        ,
    E_CIPHER_TYPE_OTPHASH    ,

}HAL_CIPHER_CMDTYPE;

typedef enum
{
    E_PARSER_HDCPMODE_NONE = 0,
    E_PARSER_HDCPMODE_HDCP20,

}HAL_CIPHER_PARSER_HDCPMODE;

typedef enum
{
    E_PARSER_TSMODE_PES = 0,
    E_PARSER_TSMODE_TS,

}HAL_CIPHER_PARSER_TSMODE;

typedef enum
{
    E_PARSER_PKTMODE_188 = 0,
    E_PARSER_PKTMODE_192,

}HAL_CIPHER_PARSER_PKTMODE;

typedef enum
{
    E_PARSER_AUTOMODE_NONE = 0,
    E_PARSER_AUTOMODE_EN,

}HAL_CIPHER_PARSER_AUTOMODE;

typedef enum
{
    E_PARSER_ITMODE_NONE = 0,
    E_PARSER_ITMODE_EN,

}HAL_CIPHER_PARSER_ITMODE;  //Init trust

typedef enum
{
    E_PARSER_CLEARMODE_NONE = 0,
    E_PARSER_CLEARMODE_EN,

}HAL_CIPHER_PARSER_CLEARMODE;

//Tmp area, open to drv level latter
typedef struct
{
	MS_U32 u32ObfIdxR;
	MS_U32 u32ObfIdxW;
}DRV_CIPHER_OBF;

typedef enum
{
    E_CIPHER_PARSER_TS_PKT192 = 0,
    E_CIPHER_PARSER_TS_PKT192_CLEAR,
    E_CIPHER_PARSER_TS_PKT188,
    E_CIPHER_PARSER_TS_PKT188_CLEAR,
    E_CIPHER_PARSER_HDCP20_PKT192,
    E_CIPHER_PARSER_HDCP20_PKT192_CLEAR,
    E_CIPHER_PARSER_HDCP20_PKT188,
    E_CIPHER_PARSER_HDCP20_PKT188_CLEAR,
} CIPHER_PARSER_MODE;

typedef struct
{
    MS_U8           *pu8PID0;
    MS_U8           *pu8PID1;
}CIPHER_PARSER_PID;

typedef enum
{
    E_CIPHER_PARSER_SCB_NONE = 0,
    E_CIPHER_PARSER_SCB_10,
    E_CIPHER_PARSER_SCB_11,
} CIPHER_PARSER_SCB;

typedef struct
{
    CIPHER_PARSER_SCB  eSCB; //Transport Stream Scramble Pattern, decide 10 or 11 or 1x need to scrambled (TS layer)
    CIPHER_PARSER_SCB  eFSCB;
    MS_BOOL bTsScrbMask; //Transport Stream Mask
    MS_BOOL bRmvScrb; //Remove Scramble
    MS_BOOL bInScrb; //Insert Scramble
} CIPHER_PARSER_TSCFG;


typedef struct
{
    DRV_CIPHER_ALGO stAlgo;
    DRV_CIPHER_KEY  stKey;
    DRV_CIPHER_DATA stInput;
    DRV_CIPHER_DATA stOutput;
    MS_BOOL         bDecrypt;
    CIPHER_PARSER_MODE  eParserMode;
    CIPHER_PARSER_TSCFG stTSCfg;
    CIPHER_PARSER_PID   stPID;
    DRV_CIPHER_KEY  stKey2;
    MS_U32 u32CAVid;
    MS_BOOL bClearHead;
    P_DrvCIPHER_EvtCallback pfCallback;
}CIPHER_PARSERCFG;

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_CIPHER_SetBank(MS_VIRT u32BankAddr) ;
void HAL_CIPHER_ResetStatus(MS_BOOL RstDma , MS_BOOL RstSha);
void HAL_CIPHER_ResetException(void);
MS_BOOL HAL_CIPHER_ResetKey(MS_U32 u32KeyIdx);
void HAL_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel);

void HAL_CIPHER_SWReset(void);

void HAL_CIPHER_DMA_Set_InputSrcFrom(CIPHER_MEM_TYPE InputSrcFrom, MS_U8* pu8Data, MS_U32 u32Size);
MS_BOOL HAL_CIPHER_DMA_Set_OutputDstTo(CIPHER_MEM_TYPE OutputDstTo, MS_U8* pu8Data, MS_U32 u32Size);
void HAL_CIPHER_DMA_Set_OutputDstKL(MS_BOOL bDstKL);


void HAL_CIPHER_DMA_Set_FileinDesc(MS_PHY FileinAddr, MS_U32 u32FileinNum);
void HAL_CIPHER_DMA_Set_FileoutDesc(MS_PHY FileoutSAddr, MS_PHY phyFileoutEAddr);
void HAL_CIPHER_OTPHash_Set_FileinDesc(MS_PHY u32FileinAddr, MS_U32 u32FileinNum, MS_U32 u32CurrentRound, CIPHER_MEM_TYPE eInputSrcFrom);

MS_BOOL HAL_CIPHER_DMA_Set_Key(DRV_CIPHER_KEY stKey);
void HAL_CIPHER_DMA_Set_IV(MS_U8* pu8IV, MS_U32 u32Size);

MS_BOOL HAL_CIPHER_DMA_Set_Data(MS_U8* pu8Data, MS_U32 u32Size);
void HAL_CIPHER_DMA_Set_Config(MS_BOOL OutputReg);
void HAL_CIPHER_DMA_Set_ReportMode(MS_BOOL RptInDram, MS_PHY u32DramAddr);

void HAL_CIPHER_DMA_Set_DataSwap(MS_BOOL InDataSwap , MS_BOOL OutDataSwap,
                                MS_BOOL DInByteSwap, MS_BOOL DOutByteSwap );

void HAL_CIPHER_DMA_Set_Algo(DRV_CIPHER_ALGO stAlgo);

void HAL_CIPHER_DMA_Set_OTPHash(MS_U32 u32CurrentRound, MS_U32 u32OTPHashRound);

MS_BOOL HAL_CIPHER_DMA_Start(MS_BOOL Decrypt , HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ, MS_U16 u16CmdID);
MS_BOOL HAL_CIPHER_OTPHash_Start(HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ, MS_U16 u16CmdID);
MS_BOOL HAL_CIPHER_DMA_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret);

void HAL_CIPHER_DMA_WriteCMDQ(MS_U32 u32Cmd);

MS_BOOL HAL_CIPHER_DMA_Set_CaVid(MS_U32 u32CAVid);

void HAL_CIPHER_DMA_GetRpt(MS_U32 *DmaRpt);
void HAL_CIPHER_DMA_GetData(MS_U8 *u8Data) ;
void HAL_CIPHER_DMA_AlgoTable_Init(void);
MS_BOOL HAL_CIPHER_DMA_CheckAlgo(HAL_CIPHER_ALGO eAlgo, HAL_CIPHER_MODE eMode, HAL_CIPHER_RESIDUE eRes, HAL_CIPHER_SHORTBLOCK eSB);
void HAL_CIPHER_Hash_SetMsgLength( MS_U32 u32Size );
MS_BOOL HAL_CIPHER_Hash_SetMsg(MS_PHY u32MsgPAddr, MS_U32 u32Size ,MS_U32 u32SrcSel );
void HAL_CIPHER_Hash_SetHOS(MS_BOOL bHos);
MS_BOOL HAL_CIPHER_Hash_Start(HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ , MS_BOOL bRst, MS_U16 u16CmdID);
MS_BOOL HAL_CIPHER_Hash_Set_OuputAddr(MS_PHY u32OutputPAddr, MS_U32 u32DstSel);
MS_BOOL HAL_CIPHER_Hash_Set_InitWordCnt(HAL_CIPHER_IWCTYPE eIWCType, MS_U32 u32StartBytes);
MS_BOOL HAL_CIPHER_Hash_Set_IV(MS_U8* pu8IV, MS_U32 u32IVSize ,MS_U32 u32IVSel);
MS_BOOL HAL_CIPHER_Hash_Set_CaVid(MS_U32 u32CAVid);
void HAL_CIPHER_Hash_Set_Config(CIPHER_HASH_ALGO algo , MS_BOOL bAutoPad , MS_BOOL bInv16);
void HAL_CIPHER_Hash_GetRpt(MS_U32 *HashRpt, MS_U32 u32Size);
MS_BOOL HAL_CIPHER_Hash_SetHMACKey(DRV_CIPHER_HMAC_KEY stHMACKey, CIPHER_HMAC_KPAD eKpad, MS_BOOL bClear);
MS_BOOL HAL_CIPHER_Hash_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret);
MS_BOOL HAL_CIPHER_Hash_Set_MsgSrcFrom(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashSrc);
MS_BOOL HAL_CIPHER_Hash_Set_OutputDstTo(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashDst);
void HAL_CIPHER_Hash_Set_ReportMode(MS_BOOL RptInDram, MS_U32 u32DramAddr);
void HAL_CIPHER_Hash_ExceptFilter(MS_U32 *pu32Exception, CIPHER_KEY_SRC eKeySrc, MS_U8 u8KeyIdx);

MS_U32 HAL_CIPHER_ReadException(MS_U32 u32ExcTmp);
void HAL_CIPHER_GetException(MS_U32 *pu32ExcFlag);

void HAL_CIPHER_IntEnable(void);
void HAL_CIPHER_IntClear(void);

//=============PARSER=================================
void HAL_CIPHER_PARSER_Set_Mode(CIPHER_PARSER_MODE eMode);
void HAL_CIPHER_PARSER_Set_PID(CIPHER_PARSER_PID stPID);
void HAL_CIPHER_PARSER_Set_SCB(CIPHER_PARSER_SCB eSCB);
void HAL_CIPHER_PARSER_Set_ForceSCB(MS_BOOL bInsert, CIPHER_PARSER_SCB eSCB);
MS_BOOL HAL_CIPHER_PARSER_Set_IV2(MS_U8 *pu8IV2, MS_U8 u8IVLen);
MS_BOOL HAL_CIPHER_PARSER_Set_Key2(DRV_CIPHER_KEY stKey);
void HAL_CIPHER_PARSER_Set_MaskSCB(MS_BOOL bEnable);
void HAL_CIPHER_PARSER_Rmv_SCB(MS_BOOL bRemove);
void HAL_CIPHER_PARSER_BypassPid(MS_BOOL bEnable);
void HAL_CIPHER_PARSER_Set_ClearStartMode(MS_BOOL bEnable);
//MISC Function
MS_BOOL HAL_CIPHER_Misc_Random(MS_U8 *pu8Buf, MS_U32 u32Size);
MS_BOOL HAL_CIPHER_Set_OBFIdx(MS_BOOL bDMA, MS_U8 u8ReadIdx, MS_U8 u8WriteIdx);

#endif // #ifndef __HAL_CIPHER_H__

