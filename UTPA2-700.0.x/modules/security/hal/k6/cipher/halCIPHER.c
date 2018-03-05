////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2013 MStar Semiconductor, Inc.
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
// file   halCIPHER.c
// @brief  CIPHER HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <stdio.h>
#include <string.h>
#endif


#include "MsCommon.h"
#include "regCIPHER.h"
#include "halCIPHER.h"
#include "drvCIPHER.h"
#include "halCHIP.h"

#ifndef MSOS_TYPE_LINUX_KERNEL
extern void * memset ( void * ptr, int value, size_t num );
extern void * memcpy ( void * destination, const void * source, size_t num );
extern int printf ( const char * format, ... );
#endif
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define     DELAY_WAITING           1UL
#define     DELAY_RESCHEDULE        10UL
#define     COMQ_AVALIABLE          0x10UL
#define     CIPHER_KEY_SIZE         16UL
#define     CRYPTODMA_DEBUG_SIZE    32UL
#define     HASH_MANUAL_BLOCK_SIZE  64UL
#define     PID_SIZE                2UL
#define     MIU_SELECT_BIT          0x80000000UL

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT               _u32RegBase = 0;
static REG_AESDMACtrl        *_AESDMACtrl  = 0 ;
static CIPHER_DBGMSG_LEVEL _HAL_CIPHERDBGLevel = CIPHER_DBG_ERR;

// Algorithm Table for AES/TDES/DES
static MS_BOOL _CryptodMA_AlgoTable[4][E_DMA_MODE_NUM][E_DMA_RESIDUE_NUM][E_DMA_SB_NUM];


#define REG32_W(reg, value)     do {    \
                                    (reg)->H = ((value) >> 16); \
                                    (reg)->L = ((value) & 0x0000FFFF);  \
                                } while(0)

#define REG32(addr) (*(volatile MS_U32 *) (addr))
#define REG16(add) *((volatile MS_U16 *)(add))

#ifdef CIPHER_NO_PRINTF
#define HAL_DEBUG_CIPHER(debug_level, x)
#else
#define HAL_DEBUG_CIPHER(debug_level, x)     do { if (_HAL_CIPHERDBGLevel >= (debug_level)) (x); } while(0)
#endif
//--------------------------------------------------------------------------------------------------
//   Local Variable
//--------------------------------------------------------------------------------------------------

MS_PHY _u32DmaInAddr;
MS_U32 _u32DmaInNum;
MS_PHY _u32DmaOut_StartAddr;
MS_PHY _u32DmaOut_EndAddr;
MS_PHY _u32ReportAddr;
MS_U32 _u32OTPHashLengthPadding;

MS_U8 _pu8CipherKey[CIPHER_KEY_SIZE];
MS_U8 _pu8CipherKey2[CIPHER_KEY_SIZE];
MS_U8 _pu8IV[CIPHER_KEY_SIZE];
MS_U8 _pu8IV2[CIPHER_KEY_SIZE];
MS_U8 _pu8Data[CIPHER_KEY_SIZE];
MS_U8 _pu8PID0[PID_SIZE];
MS_U8 _pu8PID1[PID_SIZE];

MS_U32 _u32CmdLen = 0 ;

// [NOTE] Solve Strict-Aliasing //
union {
    CMD_DmaFormat0  DmaCtrl0;
    MS_U32 u32DmaCtrl0;
}_uDmaCtrl0;

union{
    CMD_DmaFormat1  DmaCtrl1;
    MS_U32 u32DmaCtrl1;
}_uDmaCtrl1;

union{
    CMD_DmaFormat2  DmaCtrl2;
    MS_U32 u32DmaCtrl2;
}_uDmaCtrl2;

union{
    CMD_DmaFormat3  DmaCtrl3;
    MS_U32 u32DmaCtrl3;
}_uDmaCtrl3;

union{
    Hash_DmaFormat0  HashCtrl0;
    MS_U32 u32HashCtrl0;
}_uHashCtrl0;

union{
    Hash_DmaFormat1  HashCtrl1;
    MS_U32 u32HashCtrl1;
}_uHashCtrl1;

union{
    CMD_HashFormat2  HashCtrl2;
    MS_U32 u32HashCtrl2;
}_uHashCtrl2;

MS_PHY _u32HashSrcAddr;
MS_PHY _u32HashDestAddr;
MS_U32 _u32InitWordCnt;
MS_U32 _u32HashMsgLen;

MS_U8  _pu8HashIV[HASH_CMD_IV_SIZE];
MS_U8  _pu8HashMsg[HASH_CMD_MSG_SIZE];
MS_U8  _pu8HashHMACHostKey[HASH_CMD_HMAC_HOSTKEY_SIZE];

static MS_U32 _CIPHER_CAVid = 0x0F;

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void WRITE_CMDQ(MS_U32 u32Value)
{
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s] = 0x%08X \n",__func__, (unsigned int)(u32Value)));
    REG32(&_AESDMACtrl->Cmd_Queue) = (u32Value) ;
}

MS_BOOL _HAL_CIPHER_TransMIUAddr(MS_PHY* phyAddr)
{
    CHIP_MIU_ID eMIUSel;
    MS_PHY phyOffset;

    if(NULL == phyAddr)
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]NULL Address!\n",__func__, __LINE__));
        return FALSE;
    }

    _phy_to_miu_offset(eMIUSel,phyOffset,*phyAddr);

    if(E_CHIP_MIU_0 == eMIUSel)
    {
        *phyAddr = phyOffset;
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Physical address trans to MIU0\n",__func__, __LINE__));
    }
    else
    {
        *phyAddr = (phyOffset | MIU_SELECT_BIT);
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Physical address trans to MIU1\n",__func__, __LINE__));
    }

    return TRUE;
}

MS_U32 _HAL_CIPHER_GetCmdLen(HAL_CIPHER_CMDTYPE CmdType)
{

    MS_U32 u32CmdLen = 0 ;


    if ( E_CIPHER_TYPE_DMA == CmdType )
    {
        u32CmdLen = 7;

        if ( _uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl0.DmaCtrl0.DIV == TRUE )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl1.DmaCtrl1.SD == E_DMA_SRC_DIRECT )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl0.DmaCtrl0.DK == TRUE )
        {
            u32CmdLen += 4 ;
        }
		if ( _uDmaCtrl2.DmaCtrl2.HP == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.DK2 == TRUE )
        {
            u32CmdLen += 4 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.DIV2 == TRUE )
        {
            u32CmdLen += 4 ;
        }

        return u32CmdLen ;
    }


    if ( E_CIPHER_TYPE_SHA == CmdType )
    {

        u32CmdLen = 3 ;

	    if (_uHashCtrl0.HashCtrl0.CMD == CMD_HASH_START_RR )
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG )
        {
            u32CmdLen += 1 ;
        }

		if(_uHashCtrl2.HashCtrl2.RR == TRUE)
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.IWC_Sel == CMD_HASH_IWC_CMDQ )
        {
            u32CmdLen += 1 ;
        }

        if ((_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_DRAM ) || (_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_R2 ))
        {
            u32CmdLen += 1 ;
        }

        if (_uHashCtrl0.HashCtrl0.InitHashSel == CMD_HASH_IV_CMD )
        {
            u32CmdLen += 8 ;
        }

        if (_uHashCtrl0.HashCtrl0.Src == CMD_HASH_SRC_REG )
        {
            u32CmdLen += 16 ;
        }

		if((_uHashCtrl0.HashCtrl0.HMAC_KeySel == CMD_HMAC_KSEL_HK) && (_uHashCtrl0.HashCtrl0.DK == CMD_HMAC_KSEL_DIR_HK))
        {
            u32CmdLen += 4 ;
        }

        return u32CmdLen ;

    }

    if ( E_CIPHER_TYPE_OTPHASH == CmdType )
    {
        u32CmdLen = 7 ;

        if ( _uDmaCtrl1.DmaCtrl1.NL == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
            u32CmdLen += 1 ;
        }
        if ( _uDmaCtrl3.DmaCtrl3.AP == TRUE && _uDmaCtrl3.DmaCtrl3.IK == FALSE)
        {
            u32CmdLen += 1 ;
        }

        return u32CmdLen ;
    }


    return 0 ;

}

HAL_CIPHER_KEYSRC _HAL_CIPHER_TransKey(CIPHER_KEY_SRC src, MS_U32 idx)
{
    HAL_CIPHER_KEYSRC KeySrc = E_DMA_KSEL_REGKEY;

    if(src == E_CIPHER_KSRC_KL)
    {
        KeySrc = E_DMA_KSEL_SK0 + idx;
    }
    else if(src == E_CIPHER_KSRC_OTP)
    {
        KeySrc = E_DMA_KSEL_MK0 + idx;
    }
    else if(src == E_CIPHER_KSRC_CPU)
    {
        KeySrc = E_DMA_KSEL_REGKEY;
    }
	else if(src == E_CIPHER_KSRC_CAIP)
    {
        KeySrc = E_DMA_KSEL_CAIP;
    }

    return KeySrc;
}

HAL_CIPHER_ALGO _HAL_CIPHER_TransMainAlgo(CIPHER_MAIN_ALGO algo)
{
    HAL_CIPHER_ALGO hal_algo;
    switch(algo)
    {
    case E_CIPHER_MAIN_AES:
        hal_algo = E_DMA_ALGO_AES;
        break;
    case E_CIPHER_MAIN_DES:
        hal_algo = E_DMA_ALGO_DES;
        break;
    case E_CIPHER_MAIN_TDES:
        hal_algo = E_DMA_ALGO_TDES;
        break;
    case E_CIPHER_MAIN_M6_S56_CCBC:
        hal_algo = E_DMA_ALGO_M6_S56_CCBC;
        break;
    case E_CIPHER_MAIN_M6_S56:
        hal_algo = E_DMA_ALGO_M6_S56;
        break;
    case E_CIPHER_MAIN_M6_KE56:
        hal_algo = E_DMA_ALGO_M6_KE56;
        break;
    case E_CIPHER_MAIN_RC4:
        hal_algo = E_DMA_ALGO_RC4;
        break;
    case E_CIPHER_MAIN_RC4_128:
        hal_algo = E_DMA_ALGO_RC4_128;
        break;
    default:
        hal_algo = E_DMA_ALGO_NONE;
    }

    return hal_algo;
}

HAL_CIPHER_MODE _HAL_CIPHER_TransSubAlgo(CIPHER_SUB_ALGO algo)
{
    HAL_CIPHER_MODE hal_algo;
    switch(algo)
    {
    case E_CIPHER_SUB_ECB:
        hal_algo = E_DMA_MODE_ECB;
        break;
    case E_CIPHER_SUB_CBC:
        hal_algo = E_DMA_MODE_CBC;
        break;
    case E_CIPHER_SUB_CTR:
        hal_algo = E_DMA_MODE_CTR;
        break;
    case E_CIPHER_SUB_CBC_MAC:
        hal_algo = E_DMA_MODE_CBC_MAC;
        break;
    case E_CIPHER_SUB_CTR_64:
        hal_algo = E_DMA_MODE_CTR_64;
        break;
	case E_CIPHER_SUB_CMAC_Key:
        hal_algo = E_DMA_MODE_CMAC_Key;
        break;
    case E_CIPHER_SUB_CMAC_Algo:
        hal_algo = E_DMA_MODE_CMAC_Algo;
        break;
    case E_CIPHER_SUB_PCBC_ADD:
        hal_algo = E_DMA_MODE_PCBC_ADD;
        break;
    case E_CIPHER_SUB_PCBC_XOR:
        hal_algo = E_DMA_MODE_PCBC_XOR;
        break;
    case E_CIPHER_SUB_OTPHASH:
        hal_algo = E_DMA_MODE_OTPHASH;
        break;
    default:
        hal_algo = E_DMA_MODE_NONE;
    }

    return hal_algo;

}

HAL_CIPHER_RESIDUE _HAL_CIPHER_TransResAlgo(CIPHER_RES_ALGO algo)
{
    HAL_CIPHER_RESIDUE hal_algo;
    switch(algo)
    {
    case E_CIPHER_RES_CLR:
        hal_algo = E_DMA_RESIDUE_CLR;
        break;
    case E_CIPHER_RES_CTS:
        hal_algo = E_DMA_RESIDUE_CTS;
        break;
    case E_CIPHER_RES_SCTE52:
        hal_algo = E_DMA_RESIDUE_SCTE52;
        break;
    default:
        hal_algo = E_DMA_RESIDUE_NONE;
    }

    return hal_algo;
}

HAL_CIPHER_SHORTBLOCK _HAL_CIPHER_TransSBAlgo(CIPHER_SB_ALGO algo)
{
    HAL_CIPHER_SHORTBLOCK hal_algo;
    switch(algo)
    {
    case E_CIPHER_SB_CLR:
        hal_algo = E_DMA_SB_CLR;
        break;
    case E_CIPHER_SB_IV1:
        hal_algo = E_DMA_SB_IV1;
        break;
    case E_CIPHER_SB_IV2:
        hal_algo = E_DMA_SB_IV2;
        break;
    default:
        hal_algo = E_DMA_SB_NONE;
    }

    return hal_algo;

}

HAL_CIPHER_HASHMODE _HAL_CIPHER_TransHASHAlgo(CIPHER_HASH_ALGO algo)
{
    HAL_CIPHER_HASHMODE hal_algo;
    switch(algo)
    {
    case E_CIPHER_HASH_ALGO_SHA1:
        hal_algo = E_HASH_SHA1;
        break;
    case E_CIPHER_HASH_ALGO_SHA256:
        hal_algo = E_HASH_SHA256;
        break;
    case E_CIPHER_HASH_ALGO_MD5:
        hal_algo = E_HASH_MD5;
        break;
    default:
        hal_algo = E_HASH_SHA256;
    }

    return hal_algo;
}

void HAL_CIPHER_DMA_AlgoTable_Init(void)
{

    memset(_CryptodMA_AlgoTable , 0x0 , sizeof(_CryptodMA_AlgoTable));

    // AES algorithms //
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CBC_MAC][E_DMA_RESIDUE_CLR][E_DMA_RESIDUE_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_AES][E_DMA_MODE_CTR][E_DMA_RESIDUE_NONE][E_DMA_SB_NONE] = TRUE ; // residue & sb don't care

    // DES Algorithms //
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV2] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_DES][E_DMA_MODE_CTR][E_DMA_RESIDUE_NONE][E_DMA_SB_NONE] = TRUE ; // residue & sb don't care

    // TDES Algorithm //
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_ECB][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CTR][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CTS][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_CLR][E_DMA_SB_CLR] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV1] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_IV2] = TRUE ;
    _CryptodMA_AlgoTable[E_DMA_ALGO_TDES][E_DMA_MODE_CBC][E_DMA_RESIDUE_SCTE52][E_DMA_SB_CLR] = TRUE ;

}


MS_BOOL HAL_CIPHER_DMA_CheckAlgo(HAL_CIPHER_ALGO eAlgo, HAL_CIPHER_MODE eMode, HAL_CIPHER_RESIDUE eRes, HAL_CIPHER_SHORTBLOCK eSB)
{
    // Special Cases //
    if (eAlgo > E_DMA_ALGO_TDES )
    {
        if ((E_DMA_ALGO_RC4==eAlgo)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR == eSB)&&(E_DMA_MODE_NONE==eMode))
        {   //RC4/none/none/none
            return TRUE ;
        }
        if ((E_DMA_ALGO_M6_KE56==eAlgo)&&(E_DMA_MODE_ECB==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_KE56/ecb/none/clr
            return TRUE;
        }
        if ((E_DMA_ALGO_M6_S56==eAlgo)&&(E_DMA_MODE_ECB==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_S56/ecb/none/clr
            return TRUE;
        }
        if ((E_DMA_ALGO_M6_S56_CCBC==eAlgo)&&(E_DMA_MODE_NONE==eMode)&&(E_DMA_RESIDUE_NONE==eRes)&&(E_DMA_SB_CLR==eSB))
        {   //M6_S56_CCBC/none/none/clr
            return TRUE;
        }
        return FALSE ;
    }

    // Reference table for Algorithm AES/TDES/DES //
    return _CryptodMA_AlgoTable[eAlgo][eMode][eRes][eSB] ;
}

void HAL_CIPHER_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase = u32BankAddr;
    _AESDMACtrl = (REG_AESDMACtrl*)(_u32RegBase + REG_CIPHERCTRL_BASE);

}

void HAL_CIPHER_SetDbgLevel(CIPHER_DBGMSG_LEVEL eDBGMsgLevel)
{
    _HAL_CIPHERDBGLevel = eDBGMsgLevel;
}

void HAL_CIPHER_ResetStatus(MS_BOOL RstDma , MS_BOOL RstHash)
{
    if (RstDma)
    {
        _uDmaCtrl0.u32DmaCtrl0 = 0 ;
        _uDmaCtrl1.u32DmaCtrl1 = 0 ;
        _uDmaCtrl2.u32DmaCtrl2 = 0 ;
        _uDmaCtrl3.u32DmaCtrl3 = 0 ;

        _u32DmaInAddr = 0;
        _u32DmaInNum  = 0 ;
        _u32OTPHashLengthPadding = 0;
        _u32DmaOut_StartAddr = 0;
        _u32DmaOut_EndAddr   = 0;
        _u32ReportAddr = 0 ;
        memset(&_pu8CipherKey ,   0x0, CIPHER_KEY_SIZE );
        memset(&_pu8IV        ,   0x0, CIPHER_KEY_SIZE );
		memset(&_pu8CipherKey2 ,   0x0, CIPHER_KEY_SIZE );
        memset(&_pu8IV2        ,   0x0, CIPHER_KEY_SIZE );
        memset(&_pu8Data      ,   0x0, CIPHER_KEY_SIZE );
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Reset DMA value\n", __FUNCTION__, __LINE__));

    }

    if (RstHash)
    {
        (_uHashCtrl0.u32HashCtrl0) = 0 ;
		(_uHashCtrl1.u32HashCtrl1) = 0 ;
		(_uHashCtrl2.u32HashCtrl2) = 0 ;

        _u32HashSrcAddr  = 0 ;
        _u32InitWordCnt  = 0 ;
        _u32HashDestAddr = 0 ;
        _u32HashMsgLen   = 0 ;

        memset(_pu8HashIV, 0x0 ,HASH_CMD_IV_SIZE);
        memset(_pu8HashMsg, 0x0 , HASH_CMD_MSG_SIZE);
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Reset HASH value\n", __FUNCTION__, __LINE__));
    }

}

void HAL_CIPHER_ResetException(void)
{
    MS_U32 u32DMACtrl = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = u32DMACtrl | REG_EXCEPT_FLAG_CLEAR;  //clear all except flag
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = u32DMACtrl & ~REG_EXCEPT_FLAG_CLEAR;  //enable acpu except flag
}

MS_BOOL HAL_CIPHER_ResetKey(MS_U32 u32KeyIdx)
{
    HAL_CIPHER_KEYSRC KeySrc;

    //Check Key Index
    if(u32KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid Key Index[%u]\n",
            __func__, __LINE__, u32KeyIdx));
        return FALSE;
    }

    KeySrc = _HAL_CIPHER_TransKey(E_CIPHER_KSRC_KL, u32KeyIdx);

    _uDmaCtrl0.DmaCtrl0.DK = FALSE;
    _uDmaCtrl0.DmaCtrl0.KeySel = KeySrc|CMD_DMA_KSEL_CLR_SK;

    return TRUE;

}
void HAL_CIPHER_SWReset(void)
{
    MS_U32 DMA_CTRL = REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]);
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = DMA_CTRL | REG_DMA_SW_RESET;  //reset CryptoDMA
    REG32(&_AESDMACtrl->Dma_Ctrl[REG_DMA_CTRL]) = DMA_CTRL & ~REG_DMA_SW_RESET; //enable CryptoDMA

}

MS_BOOL HAL_CIPHER_Set_OBFIdx(MS_BOOL bDMA, MS_U8 u8ReadIdx, MS_U8 u8WriteIdx)
{
    if(bDMA)
    {
        _uDmaCtrl2.DmaCtrl2.OBF_IDX_READ = u8ReadIdx;
        _uDmaCtrl2.DmaCtrl2.OBF_IDX_WRITE = u8WriteIdx;
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Set DMA Obf Index Read:[%u], Write:[%u]\n",
            __func__, __LINE__, u8ReadIdx, u8WriteIdx));
    }
    else
    {
        _uHashCtrl1.HashCtrl1.OBF_IDX_READ  =  u8ReadIdx;
        _uHashCtrl1.HashCtrl1.OBF_IDX_WRITE =  u8WriteIdx;
        HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Set HASH Obf Index Read:[%u], Write:[%u]\n",
            __func__, __LINE__, u8ReadIdx, u8WriteIdx));
    }

    return TRUE;
}

void HAL_CIPHER_DMA_Set_InputSrcFrom(CIPHER_MEM_TYPE InputSrcFrom, MS_U8* pu8Data, MS_U32 u32Size)
{
	HAL_CIPHER_DATASRC src = E_DMA_SRC_DRAM;

	switch(InputSrcFrom)
	{
		case E_CIPHER_DRAM:
            _u32DmaInAddr = 0;
			src = E_DMA_SRC_DRAM;
			break;

		case E_CIPHER_DQMEM://only for R2
			_u32DmaInAddr = HAL_CIPHER_BASE_DQMEM;
			src = E_DMA_SRC_DQMEM;
			break;

		case E_CIPHER_IQMEM://only for R2
			_u32DmaInAddr = HAL_CIPHER_BASE_IQMEM;
			src = E_DMA_SRC_IQMEM;
			break;

		// TODO: future work, DIRECT data from pu8Data
		#if 0
		case E_CIPHER_DIRECT:
			HAL_CIPHER_DMA_Set_Data(pu8Data, u32Size);
			src = E_DMA_SRC_DIRECT;
			break;
		#endif

        case E_CIPHER_OTP: //only for OTPHASH
			_u32DmaInAddr = 0;
			src = E_DMA_SRC_HW_INPUT;
			break;

		default:
    		_u32DmaInAddr = 0;
			src = E_DMA_SRC_DRAM;
			break;
	}

    _uDmaCtrl1.DmaCtrl1.SD = (MS_U32)src;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set input source from [%d]\n", __func__, __LINE__, src));
}

MS_BOOL HAL_CIPHER_DMA_Set_OutputDstTo(CIPHER_MEM_TYPE OutputDstTo, MS_U8* pu8Data, MS_U32 u32Size)
{
	HAL_CIPHER_DATADST dst = E_DMA_DST_DRAM;
    MS_BOOL bDstKL = FALSE;

	switch(OutputDstTo)
	{
		case E_CIPHER_DRAM:
            _u32DmaOut_StartAddr = 0;
            _u32DmaOut_EndAddr 	 = 0;
			dst = E_DMA_DST_DRAM;
			break;

		case E_CIPHER_DQMEM: //only for R2
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_DQMEM;
			_u32DmaOut_EndAddr   = HAL_CIPHER_BASE_DQMEM;
			dst = E_DMA_DST_DQMEM;
			break;

		case E_CIPHER_IQMEM: //only for R2
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_IQMEM;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_IQMEM;
			dst = E_DMA_DST_IQMEM;
			break;

		case E_CIPHER_DIRECT: //for ACPU, OTPHASH
			dst = E_DMA_DST_REGFILE;
			break;

		case E_CIPHER_KL_LUT:     //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_LUT;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_LUT;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		case E_CIPHER_KL_M:       //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_M;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_M;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		case E_CIPHER_KL_BC:     //for KL transform algorithm
			_u32DmaOut_StartAddr = HAL_CIPHER_BASE_BC;
			_u32DmaOut_EndAddr	 = HAL_CIPHER_BASE_BC;
			dst = E_DMA_DST_DRAM;
            bDstKL = TRUE;
			break;

		default:
		    _u32DmaOut_StartAddr = 0;
            _u32DmaOut_EndAddr   = 0;
			dst = E_DMA_DST_DRAM;
			break;
	}

    _uDmaCtrl1.DmaCtrl1.Dest= (MS_U32)dst;

    if(bDstKL)
    {
        HAL_CIPHER_DMA_Set_OutputDstKL(TRUE);
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set output destination to KL [0x%4x]\n", __func__, __LINE__, (unsigned int)_u32DmaOut_StartAddr));
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set output destination to [%d]\n", __func__, __LINE__, dst));
    }

    return TRUE;
}

void HAL_CIPHER_DMA_Set_OutputDstKL(MS_BOOL bDstKL)
{
    if(bDstKL)
    {
        _uDmaCtrl1.DmaCtrl1.DestKL = CMD_DMA_OUTPUT_SRAM_KL;
        _uDmaCtrl0.DmaCtrl0.DK = TRUE;
    }
    else
    {
        _uDmaCtrl1.DmaCtrl1.DestKL = CMD_DMA_OUTPUT_DEST;
    }
}

void HAL_CIPHER_DMA_Set_FileinDesc(MS_PHY u32FileinAddr, MS_U32 u32FileinNum)
{

     _u32DmaInAddr += u32FileinAddr;
     _u32DmaInNum = u32FileinNum ;
}

void HAL_CIPHER_DMA_Set_FileoutDesc(MS_PHY u32FileoutSAddr, MS_PHY u32phyFileoutEAddr)
{
    _u32DmaOut_StartAddr += u32FileoutSAddr;
    _u32DmaOut_EndAddr   += u32phyFileoutEAddr;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d] _u32DmaOut_StartAddr = %x _u32DmaOut_EndAddr = %x\n", __func__, __LINE__, (unsigned int)_u32DmaOut_StartAddr, (unsigned int)_u32DmaOut_EndAddr));
}

void HAL_CIPHER_OTPHash_Set_FileinDesc(MS_PHY u32FileinAddr, MS_U32 u32FileinNum, MS_U32 u32CurrentRound, CIPHER_MEM_TYPE eInputSrcFrom)
{
    if(E_CIPHER_OTP == eInputSrcFrom)
    {
        _u32DmaInAddr = u32CurrentRound;
    }
    else
    {
    _u32DmaInAddr = u32FileinAddr + u32CurrentRound*HAL_CRYPTODMA_OTPHASH_UNIT;
    }

    _u32OTPHashLengthPadding = u32FileinNum;
    if((u32FileinNum - u32CurrentRound*HAL_CRYPTODMA_OTPHASH_UNIT) >= HAL_CRYPTODMA_OTPHASH_UNIT)
    {
        _u32DmaInNum = HAL_CRYPTODMA_OTPHASH_UNIT;
    }
     else
    {
        _u32DmaInNum = u32FileinNum - u32CurrentRound*HAL_CRYPTODMA_OTPHASH_UNIT;
    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d] _u32DmaInAddr = %x _u32DmaInNum = %x _u32OTPHashLengthPadding = %x\n", __func__, __LINE__, (unsigned int)_u32DmaInAddr, (unsigned int)_u32DmaInNum, (unsigned int)_u32OTPHashLengthPadding));
}

MS_BOOL HAL_CIPHER_DMA_Set_Key(DRV_CIPHER_KEY stKey)
{
    HAL_CIPHER_KEYSRC KeySrc = 0;
    HAL_CIPHER_ALGO algo = 0;
    MS_BOOL bDK = FALSE;

	algo = _uDmaCtrl0.DmaCtrl0.Algo;

    if(E_DMA_ALGO_NONE == algo)
    {
        //No key
        bDK = TRUE;
        memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        goto SET_KEY_DONE;
    }
    //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid KeySrc[%d], Index[%d]\n",
           __func__, __LINE__, stKey.eKeySrc, stKey.u8KeyIdx));
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stKey.eKeySrc, stKey.u8KeyIdx);

    // key from CPU
    if(E_DMA_KSEL_REGKEY == KeySrc)
    {
        if ((NULL != stKey.pu8KeyData) && (stKey.u8KeyLen != 0) && (stKey.u8KeyLen <= HAL_CRYPTODMA_KEYLEN_MAX) )
        {
            memset(_pu8CipherKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8CipherKey, (MS_U8 *)stKey.pu8KeyData , stKey.u8KeyLen);
            bDK = TRUE ;
			KeySrc = 0;
        }
        else
        {
			return FALSE ;
        }

    }
    else if(E_DMA_KSEL_CAIP == KeySrc)
    {
        bDK = FALSE ;
        KeySrc = 0 ;
    }
    else// key from KL or OTP
    {
        bDK = FALSE;
    }

SET_KEY_DONE:

	_uDmaCtrl0.DmaCtrl0.DK = bDK;
    _uDmaCtrl0.DmaCtrl0.KeySel = KeySrc;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set DK[%d], KeySel[%d]\n",
        __func__, __LINE__, bDK, KeySrc));

    return TRUE ;
}

void HAL_CIPHER_DMA_Set_IV(MS_U8* pu8IV, MS_U32 u32Size)
{
    MS_BOOL bDIV = FALSE;

    if ((NULL != pu8IV) && (u32Size != 0) && (u32Size <= HAL_CRYPTODMA_KEYLEN_MAX) )
    {
        memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        memcpy(_pu8IV, (MS_U8 *)pu8IV, u32Size);
        bDIV = TRUE;
    }
    else // Data from Dram
    {
        memset(_pu8IV, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        bDIV = FALSE;
    }

    _uDmaCtrl0.DmaCtrl0.DIV = bDIV ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set DIV[%d]\n",
        __func__, __LINE__, bDIV));
}


// Set Direct Data to Register Input, if Dram input, set pu8Data = NULL ;
MS_BOOL HAL_CIPHER_DMA_Set_Data(MS_U8* pu8Data, MS_U32 u32Size)
{
    HAL_CIPHER_DATASRC eDataSrc = E_DMA_SRC_DRAM;

    // Data size can't be zero //
    if (( 0 == u32Size ) && (NULL != pu8Data ))
    {
        return FALSE ;
    }

    if (NULL != pu8Data )  // Data from CMDQ
    {
        _u32DmaInNum = u32Size ;
        memset(_pu8Data, 0x0, HAL_CRYPTODMA_DIRDATA_MAX);
        memcpy(_pu8Data, (MS_U8 *)pu8Data, u32Size);
        eDataSrc = E_DMA_SRC_DIRECT ;
    }
    else // Data from Dram
    {
        memset(_pu8Data, 0x0 , HAL_CRYPTODMA_DIRDATA_MAX );
        eDataSrc = E_DMA_SRC_DRAM ;
    }

    _uDmaCtrl1.DmaCtrl1.SD = (MS_U32)eDataSrc ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set SD[%d]\n",
        __func__, __LINE__, eDataSrc));

    return TRUE ;
}

MS_BOOL HAL_CIPHER_DMA_Set_CaVid(MS_U32 u32CAVid)
{
    MS_U32 u32Tmp = u32CAVid;

    if(u32Tmp > HAL_CRYPTODMA_CAVID_MAX)
    {
        return FALSE;
    }

    if(u32Tmp == 0)
    {
        u32Tmp = _CIPHER_CAVid ;
    }
    else
    {
        u32Tmp = u32CAVid ;
    }

    _uDmaCtrl1.DmaCtrl1.CaVid = u32Tmp ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n",
        __func__, __LINE__, (unsigned int)u32Tmp));

    return TRUE;
}

void HAL_CIPHER_DMA_Set_Config(MS_BOOL OutputReg)
{
    _uDmaCtrl1.DmaCtrl1.Dest= OutputReg ;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set dest[%d]\n",
        __func__, __LINE__, OutputReg));
}


void HAL_CIPHER_DMA_Set_ReportMode(MS_BOOL RptInDram, MS_PHY u32DramAddr)
{
    MS_BOOL bRR = FALSE;

    if (RptInDram)
    {
        bRR = TRUE ;
        _u32ReportAddr = u32DramAddr;

    }
    else
    {
        bRR = FALSE ;
        _u32ReportAddr = 0;

    }

    _uDmaCtrl1.DmaCtrl1.RR = bRR;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set report mode[%d], report address[0x%08x]\n",
        __func__, __LINE__, bRR, (unsigned int)_u32ReportAddr));
}

// [NOTE] HW Bug, the DMA can't support DRAM Data Swap, Data Error for input and output
void HAL_CIPHER_DMA_Set_DataSwap(MS_BOOL InDataSwap , MS_BOOL OutDataSwap,
                                MS_BOOL DInByteSwap, MS_BOOL DOutByteSwap )
{
    _uDmaCtrl1.DmaCtrl1.CIS = InDataSwap ;
    _uDmaCtrl1.DmaCtrl1.COS = OutDataSwap ;
    //_uDmaCtrl1.DmaCtrl1.DODS = DOutBitSwap ;
    _uDmaCtrl1.DmaCtrl1.DOBS = DOutByteSwap ;
    _uDmaCtrl1.DmaCtrl1.DIBS = DInByteSwap;

}


//void HAL_CIPHER_DMA_Set_Algo(CipherAlgo algo , CipherMode subalgo , Cipher_Residue eRes , Cipher_ShortBlock eSB )
void HAL_CIPHER_DMA_Set_Algo(DRV_CIPHER_ALGO stAlgo)
{
    HAL_CIPHER_ALGO algo;
    HAL_CIPHER_MODE subalgo;
    HAL_CIPHER_RESIDUE eRes;
    HAL_CIPHER_SHORTBLOCK eSB;

    if(_uDmaCtrl1.DmaCtrl1.CaVid == E_CIPHER_CAVID1)
    {
        algo    = (HAL_CIPHER_ALGO)stAlgo.eMainAlgo;
        subalgo = (HAL_CIPHER_MODE)stAlgo.eSubAlgo;

        // For OTPHASH in E_CIPHER_CAVID1
        if(stAlgo.eSubAlgo == E_CIPHER_SUB_OTPHASH)
            subalgo = E_DMA_MODE_OTPHASH;
    }
    else
    {
        algo    = _HAL_CIPHER_TransMainAlgo(stAlgo.eMainAlgo);
        subalgo = _HAL_CIPHER_TransSubAlgo(stAlgo.eSubAlgo);
    }

    eRes    = _HAL_CIPHER_TransResAlgo(stAlgo.eResAlgo);
    eSB     = _HAL_CIPHER_TransSBAlgo(stAlgo.eSBAlgo);

    _uDmaCtrl0.DmaCtrl0.Algo= algo ;
    _uDmaCtrl0.DmaCtrl0.SubAlgo= subalgo ;
    _uDmaCtrl0.DmaCtrl0.Residue = eRes ;
    _uDmaCtrl0.DmaCtrl0.SB = eSB ;

     HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set algorithm, MAIN[%d], SUB[%d], RES[%d], SB[%d]\n",
        __func__, __LINE__, algo, subalgo, eRes, eSB));
}

void HAL_CIPHER_DMA_Set_OTPHash(MS_U32 u32CurrentRound, MS_U32 u32OTPHashRound)
{
    _uDmaCtrl3.DmaCtrl3.AP = 0;
    _uDmaCtrl3.DmaCtrl3.IK = 0;
    // First OTPHASH enable IK, other disable IK
    if(u32CurrentRound == 0)
    {
        _uDmaCtrl3.DmaCtrl3.IK = 1;
    }
    // Final OTPHASH enable AP, other disable AP
    if(u32CurrentRound == u32OTPHashRound)
    {
        _uDmaCtrl3.DmaCtrl3.AP = 1;
    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set OTPHASH, AP[%d], IK[%d]\n",
        __func__, __LINE__, _uDmaCtrl3.DmaCtrl3.AP, _uDmaCtrl3.DmaCtrl3.IK));
}

void HAL_CIPHER_DMA_WriteCMDQ(MS_U32 u32Cmd)
{
    WRITE_CMDQ(u32Cmd) ;
}

void HAL_CIPHER_DMA_GetData(MS_U8 *u8Data)
{

    MS_U32 tmp ;

    // [Note] for u8Data is not always 4 bytes alignment.
    tmp = REG32(&_AESDMACtrl->Dma_Out[0]);
    u8Data[0] = (tmp >> 0)  & 0xFF ;
    u8Data[1] = (tmp >> 8)  & 0xFF ;
    u8Data[2] = (tmp >> 16) & 0xFF ;
    u8Data[3] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[1]);
    u8Data[4] = (tmp >> 0)  & 0xFF ;
    u8Data[5] = (tmp >> 8)  & 0xFF ;
    u8Data[6] = (tmp >> 16) & 0xFF ;
    u8Data[7] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[2]);
    u8Data[8] = (tmp >> 0)  & 0xFF ;
    u8Data[9] = (tmp >> 8)  & 0xFF ;
    u8Data[10] = (tmp >> 16) & 0xFF ;
    u8Data[11] = (tmp >> 24) & 0xFF ;

    tmp = REG32(&_AESDMACtrl->Dma_Out[3]);
    u8Data[12] = (tmp >> 0)  & 0xFF ;
    u8Data[13] = (tmp >> 8)  & 0xFF ;
    u8Data[14] = (tmp >> 16) & 0xFF ;
    u8Data[15] = (tmp >> 24) & 0xFF ;

}

void HAL_CIPHER_DMA_GetRpt(MS_U32 *DmaRpt)
{
    MS_U32 i ;

    for (i = 0 ; i < DMA_RPT_REG_SIZE ; i++)
    {
        DmaRpt[i] = REG32(&_AESDMACtrl->Dma_Reportp[i]);
    }
}

MS_BOOL HAL_CIPHER_DMA_Start(MS_BOOL Decrypt, HAL_CIPHER_INTMODE IntMode, MS_BOOL bWaitCmdQ, MS_U16 u16CmdID)
{
    MS_U32 u32SpareCnt = 0 , CmdLen = 0 ;

    _uDmaCtrl0.DmaCtrl0.OVT = FALSE ;

    //Set Thread Id
    _uDmaCtrl1.DmaCtrl1.TID  = u16CmdID ;

    _uDmaCtrl1.DmaCtrl1.IntM = IntMode ;

    CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_DMA);

    _uDmaCtrl0.DmaCtrl0.CL  = CmdLen & CMD_LEN_MASK ;
    _uDmaCtrl0.DmaCtrl0.CMD = ((Decrypt?0x3:0x2) & 0xF);

	_uDmaCtrl2.DmaCtrl2.NS = 0;
    _uDmaCtrl1.DmaCtrl1.NL = CMD_NEW_LINE;

    //Parser patch
    if(TRUE == _uDmaCtrl2.DmaCtrl2.HP)
    {
        MS_U32 u32Tmp = 0;
        u32Tmp = _AESDMACtrl->Dma_Ctrl[1];
        u32Tmp |= REG_PARSER_LG_PATH_EN;
        _AESDMACtrl->Dma_Ctrl[1] = u32Tmp;
    }

    MS_U32 MIUCrossBar = REG32(_u32RegBase + REG_MIU_CROSSBAR);  //MIU CrossBar Bank 0x1613
    REG32(_u32RegBase + REG_MIU_CROSSBAR) = MIUCrossBar | 0xf;  //enable MIU crossbar


    if(_uDmaCtrl1.DmaCtrl1.SD == CMD_DMA_DATA_DRAM)
    {
        if(_HAL_CIPHER_TransMIUAddr(&_u32DmaInAddr) == FALSE)
        {
            return FALSE;
        }
    }

    if(_uDmaCtrl1.DmaCtrl1.Dest == CMD_DMA_OUTPUT_DRAM)
    {
        if(_HAL_CIPHER_TransMIUAddr(&_u32DmaOut_StartAddr) == FALSE)
        {
            return FALSE;
        }
        if(_HAL_CIPHER_TransMIUAddr(&_u32DmaOut_EndAddr) == FALSE)
        {
            return FALSE;
        }
    }

DMA_RETRY:
    // [HW] Hardware will check Current spare count first, and others read spare count will be zero!
    u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

    if (u32SpareCnt > CmdLen)
    {
        u32SpareCnt -= CmdLen ;

        WRITE_CMDQ( (_uDmaCtrl0.u32DmaCtrl0)) ;
        WRITE_CMDQ( (_uDmaCtrl1.u32DmaCtrl1)) ;
		WRITE_CMDQ( (_uDmaCtrl2.u32DmaCtrl2)) ;
        WRITE_CMDQ( (_uDmaCtrl3.u32DmaCtrl3)) ;

        WRITE_CMDQ((MS_U32)_u32DmaInAddr) ;
        WRITE_CMDQ(_u32DmaInNum) ;
        WRITE_CMDQ((MS_U32)_u32DmaOut_StartAddr);
        WRITE_CMDQ((MS_U32)_u32DmaOut_EndAddr);

        if (_uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
           WRITE_CMDQ((MS_U32)_u32ReportAddr);
        }

        // DMA Initial Vector from CmdQ
        if (_uDmaCtrl0.DmaCtrl0.DIV == TRUE )
        {
            #if 0
            WRITE_CMDQ(_pu8IV[15]<<24 |_pu8IV[14]<<16 | _pu8IV[13]<<8 | _pu8IV[12]);
            WRITE_CMDQ(_pu8IV[11]<<24 |_pu8IV[10]<<16 | _pu8IV[ 9]<<8 | _pu8IV[ 8]);
            WRITE_CMDQ(_pu8IV[ 3]<<24 |_pu8IV[ 2]<<16 | _pu8IV[ 1]<<8 | _pu8IV[ 0]);
            WRITE_CMDQ(_pu8IV[ 7]<<24 |_pu8IV[ 6]<<16 | _pu8IV[ 5]<<8 | _pu8IV[ 4]);
            #else
            WRITE_CMDQ(_pu8IV[ 0]<<24 |_pu8IV[ 1]<<16 | _pu8IV[ 2]<<8 | _pu8IV[ 3]);
            WRITE_CMDQ(_pu8IV[ 4]<<24 |_pu8IV[ 5]<<16 | _pu8IV[ 6]<<8 | _pu8IV[ 7]);
            WRITE_CMDQ(_pu8IV[ 8]<<24 |_pu8IV[ 9]<<16 | _pu8IV[10]<<8 | _pu8IV[11]);
            WRITE_CMDQ(_pu8IV[12]<<24 |_pu8IV[13]<<16 | _pu8IV[14]<<8 | _pu8IV[15]);
            #endif
        }

        // DMA Data from CmdQ
        if (_uDmaCtrl1.DmaCtrl1.SD == CMD_DMA_DATA_REG )
        {
			#if 0
            WRITE_CMDQ(_pu8Data[15]<<24 |_pu8Data[14]<<16 | _pu8Data[13]<<8 | _pu8Data[12]);
            WRITE_CMDQ(_pu8Data[11]<<24 |_pu8Data[10]<<16 | _pu8Data[ 9]<<8 | _pu8Data[ 8]);
            WRITE_CMDQ(_pu8Data[ 7]<<24 |_pu8Data[ 6]<<16 | _pu8Data[ 5]<<8 | _pu8Data[ 4]);
            WRITE_CMDQ(_pu8Data[ 3]<<24 |_pu8Data[ 2]<<16 | _pu8Data[ 1]<<8 | _pu8Data[ 0]);
            #else
            WRITE_CMDQ(_pu8Data[ 0]<<24 |_pu8Data[ 1]<<16 | _pu8Data[ 2]<<8 | _pu8Data[ 3]);
            WRITE_CMDQ(_pu8Data[ 4]<<24 |_pu8Data[ 5]<<16 | _pu8Data[ 6]<<8 | _pu8Data[ 7]);
            WRITE_CMDQ(_pu8Data[ 8]<<24 |_pu8Data[ 9]<<16 | _pu8Data[10]<<8 | _pu8Data[11]);
            WRITE_CMDQ(_pu8Data[12]<<24 |_pu8Data[13]<<16 | _pu8Data[14]<<8 | _pu8Data[15]);
            #endif
        }

        // DMA Key from CmdQ
        if (_uDmaCtrl0.DmaCtrl0.DK == TRUE )
        {
            #if 0
            WRITE_CMDQ(_pu8CipherKey[15]<<24 |_pu8CipherKey[14]<<16 | _pu8CipherKey[13]<<8 | _pu8CipherKey[12]);
            WRITE_CMDQ(_pu8CipherKey[11]<<24 |_pu8CipherKey[10]<<16 | _pu8CipherKey[ 9]<<8 | _pu8CipherKey[ 8]);
            WRITE_CMDQ(_pu8CipherKey[ 7]<<24 |_pu8CipherKey[ 6]<<16 | _pu8CipherKey[ 5]<<8 | _pu8CipherKey[ 4]);
            WRITE_CMDQ(_pu8CipherKey[ 3]<<24 |_pu8CipherKey[ 2]<<16 | _pu8CipherKey[ 1]<<8 | _pu8CipherKey[ 0]);
            #else
            WRITE_CMDQ(_pu8CipherKey[ 0]<<24 |_pu8CipherKey[ 1]<<16 | _pu8CipherKey[ 2]<<8 | _pu8CipherKey[ 3]);
            WRITE_CMDQ(_pu8CipherKey[ 4]<<24 |_pu8CipherKey[ 5]<<16 | _pu8CipherKey[ 6]<<8 | _pu8CipherKey[ 7]);
            WRITE_CMDQ(_pu8CipherKey[ 8]<<24 |_pu8CipherKey[ 9]<<16 | _pu8CipherKey[10]<<8 | _pu8CipherKey[11]);
            WRITE_CMDQ(_pu8CipherKey[12]<<24 |_pu8CipherKey[13]<<16 | _pu8CipherKey[14]<<8 | _pu8CipherKey[15]);
            #endif
        }

		// PID for HW parser
        if (_uDmaCtrl2.DmaCtrl2.HP == TRUE )
        {
            WRITE_CMDQ(_pu8PID0[ 0]<<27 |_pu8PID0[ 1]<<19 | _pu8PID1[ 0]<<14 | _pu8PID1[ 1]<<6);

        }

        if (_uDmaCtrl3.DmaCtrl3.DIV2 == TRUE )
        {
            WRITE_CMDQ(_pu8IV2[ 0]<<24 |_pu8IV2[ 1]<<16 | _pu8IV2[ 2]<<8 | _pu8IV2[ 3]);
            WRITE_CMDQ(_pu8IV2[ 4]<<24 |_pu8IV2[ 5]<<16 | _pu8IV2[ 6]<<8 | _pu8IV2[ 7]);
            WRITE_CMDQ(_pu8IV2[ 8]<<24 |_pu8IV2[ 9]<<16 | _pu8IV2[10]<<8 | _pu8IV2[11]);
            WRITE_CMDQ(_pu8IV2[12]<<24 |_pu8IV2[13]<<16 | _pu8IV2[14]<<8 | _pu8IV2[15]);
        }

        if (_uDmaCtrl3.DmaCtrl3.DK2 == TRUE )
        {
            WRITE_CMDQ(_pu8CipherKey2[ 0]<<24 |_pu8CipherKey2[ 1]<<16 | _pu8CipherKey2[ 2]<<8 | _pu8CipherKey2[ 3]);
            WRITE_CMDQ(_pu8CipherKey2[ 4]<<24 |_pu8CipherKey2[ 5]<<16 | _pu8CipherKey2[ 6]<<8 | _pu8CipherKey2[ 7]);
            WRITE_CMDQ(_pu8CipherKey2[ 8]<<24 |_pu8CipherKey2[ 9]<<16 | _pu8CipherKey2[10]<<8 | _pu8CipherKey2[11]);
            WRITE_CMDQ(_pu8CipherKey2[12]<<24 |_pu8CipherKey2[13]<<16 | _pu8CipherKey2[14]<<8 | _pu8CipherKey2[15]);
        }

        // [HW] Hardware will check Current spare count != Write back Spare count to start operation.
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        return TRUE ;

    }
    else
    {
        // [HW] Check Current spare count == Write back Spare count to start wait cmd
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        if (bWaitCmdQ)
        {
            //  Retry until CMDQ avaliable
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA retry!\n",__func__, __LINE__));
            goto DMA_RETRY;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA Fail!\n",__func__, __LINE__));
            //  Return Fail, Command abort
            return FALSE ;
        }
    }
//DMA_FAIL:

}

MS_BOOL HAL_CIPHER_OTPHash_Start(HAL_CIPHER_INTMODE IntMode, MS_BOOL bWaitCmdQ, MS_U16 u16CmdID)
{
    MS_U32 u32SpareCnt = 0 , CmdLen = 0 ;

    _uDmaCtrl0.DmaCtrl0.OVT = FALSE ;

    //Set Thread Id
    _uDmaCtrl1.DmaCtrl1.TID  = u16CmdID ;

    _uDmaCtrl1.DmaCtrl1.IntM = IntMode ;

    _uDmaCtrl1.DmaCtrl1.NL = CMD_NEW_LINE;

    CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_OTPHASH);

    _uDmaCtrl0.DmaCtrl0.CL  = CmdLen & CMD_LEN_MASK ;
    _uDmaCtrl0.DmaCtrl0.CMD = CMD_DMA_ENCRYPT;

	_uDmaCtrl2.DmaCtrl2.NS = 0;

    MS_U32 MIUCrossBar = REG32(_u32RegBase + REG_MIU_CROSSBAR);  //MIU CrossBar Bank 0x1613
    REG32(_u32RegBase + REG_MIU_CROSSBAR) = MIUCrossBar | 0xf;  //enable MIU crossbar

DMA_RETRY:
    // [HW] Hardware will check Current spare count first, and others read spare count will be zero!
    u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

    if (u32SpareCnt > CmdLen)
    {
        u32SpareCnt -= CmdLen ;

        WRITE_CMDQ( (_uDmaCtrl0.u32DmaCtrl0)) ;
        WRITE_CMDQ( (_uDmaCtrl1.u32DmaCtrl1)) ;
		WRITE_CMDQ( (_uDmaCtrl2.u32DmaCtrl2)) ;
        WRITE_CMDQ( (_uDmaCtrl3.u32DmaCtrl3)) ;

        WRITE_CMDQ((MS_U32)_u32DmaInAddr) ;
        WRITE_CMDQ(_u32DmaInNum) ;
        WRITE_CMDQ((MS_U32)_u32DmaOut_StartAddr);
        WRITE_CMDQ((MS_U32)_u32DmaOut_EndAddr);

        if(_uDmaCtrl3.DmaCtrl3.AP == TRUE && _uDmaCtrl3.DmaCtrl3.IK == FALSE)
        {
            WRITE_CMDQ(_u32OTPHashLengthPadding);
        }

        if (_uDmaCtrl1.DmaCtrl1.RR == TRUE )
        {
           WRITE_CMDQ((MS_U32)_u32ReportAddr);
        }

        // [HW] Hardware will check Current spare count != Write back Spare count to start operation.
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        return TRUE ;
    }
    else
    {
        // [HW] Check Current spare count == Write back Spare count to start wait cmd
        REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
        if (bWaitCmdQ)
        {
            //  Retry until CMDQ avaliable
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA retry!\n",__func__, __LINE__));
            goto DMA_RETRY;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA Fail!\n",__func__, __LINE__));
            //  Return Fail, Command abort
            return FALSE ;
        }
    }

}

MS_BOOL HAL_CIPHER_DMA_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret)
{
	MS_U32 u32ExcTmp = 0;
    MS_U32 u32Rpt   = REG32(&_AESDMACtrl->Dma_Reportp[0]) ;
    MS_U32 u32RptId = u32Rpt & REG_CIPHER_RPT_THREAD_MSK;

    if(u32RptId == u32CmdID)
    {
        u32ExcTmp = (MS_U32) REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
        *u32Ret = u32ExcTmp;
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]DMA Cmd[%u] Not done!ReportId[%u]\n",
            __func__, __LINE__, u32CmdID, u32RptId));
        return FALSE;
    }

    return TRUE;

}

void HAL_CIPHER_GetException(MS_U32 *pu32ExcFlag)
{
    MS_U32 u32ExcTmp = (MS_U32) REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
    *pu32ExcFlag = u32ExcTmp;
}

MS_U32 HAL_CIPHER_ReadException(MS_U32 u32ExcTmp)
{
    MS_U32 i = 0;
    MS_U32 u32ExcOut = 0;

    for(i = 0;i<CRYPTODMA_DEBUG_SIZE;i++) //check each error flag
    {
        if((u32ExcTmp & (1 << i)) >> i)
        {
            switch(i)
            {
                case E_CIPHER_NDS_Region_fail:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_NDS_Region_fail\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_NDS_keyoff:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_NDS_keyoff\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_sha_write_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_write_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_read_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_read_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_error_flag:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_error_flag\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_read_scr:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_read_scr\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_key:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_CAVid:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_CAVid\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_CAVID;
                    break;
                case E_CIPHER_except_des:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_des\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_NDS:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_NDS\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_chain:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_chain\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_algo:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_algo\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_ALGO;
                    break;
                case E_CIPHER_except_key_HID:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key_HID\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_key_cbc_mac:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_key_cbc_mac\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_except_file_dqmem:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_file_dqmem\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_except_hash_dqmem:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_hash_dqmem\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
				case E_CIPHER_disable_masterkey0:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_masterkey0\n", __func__, __LINE__));
	                u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_masterkey1:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_masterkey1\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_cck:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_cck\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_0:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_reg_key_0\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_1:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_reg_key_1\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_2:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_reg_key_2\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
					break;
                case E_CIPHER_disable_reg_key_3:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_disable_reg_key_3\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_HMAC_KEY;
                    break;
                case E_CIPHER_dma_forbid_qmem2dram:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_dma_forbid_qmem2dram\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_dma_forbid_cryptodma_keyslot_qmem:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_dma_forbid_cryptodma_keyslot_qmem\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                case E_CIPHER_sha_forbid_qmem2dram:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_forbid_qmem2dram\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
                case E_CIPHER_sha_forbid_cryptodma_keyslot_qmem:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_sha_forbid_cryptodma_keyslot_qmem\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_tdes_key_error:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_tdes_key_error\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_write_address_error:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_write_address_error\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DATA;
                    break;
				case E_CIPHER_except_str:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_str\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
				case E_CIPHER_except_sha_str:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] E_CIPHER_except_sha_str\n", __func__, __LINE__));
                    u32ExcOut |= HAL_CIPHER_EXCEPT_DMA_KEY;
                    break;
                default:
                    HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d] Unknown error \n", __func__, __LINE__));
                    break;

            }

        }

    }

    HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]Exception out [0x%08x]\n", __func__, __LINE__, (unsigned int)u32ExcOut));

    return u32ExcOut;

}

void HAL_CIPHER_IntEnable(void)
{
    MS_U16 u16MaskTmp = REG16(_u32RegBase + REG_HST0_FIQ_MASK_63_48);
    u16MaskTmp &= REG_HTS0_FIQ_MASK_CRYPTODMA;  //FIQ 56 in reg_hst0_fiq_mask_63_48_
    REG16(_u32RegBase + REG_HST0_FIQ_MASK_63_48) = u16MaskTmp;
}

void HAL_CIPHER_IntClear(void)
{
    REG16(_u32RegBase + REG_HST0_FIQ_STATUS_63_48) = REG_HTS0_FIQ_CRYPTODMA; //set 1 to clear interrupt
}

//======================================================================

void HAL_CIPHER_Hash_GetRpt(MS_U32 *HashRpt, MS_U32 u32Size)
{
    MS_U32 i , tmp;

    if (u32Size < HASH_RPT_REG_SIZE )
    {
        tmp = u32Size ;
    }
    else
    {
        tmp = HASH_RPT_REG_SIZE ;
    }

    for (i = 0 ; i < tmp ; i++)
    {
        HashRpt[i] = REG32(&_AESDMACtrl->Hash_Reportp[i]);
    }
}

void HAL_CIPHER_Hash_Set_Config(CIPHER_HASH_ALGO algo , MS_BOOL bAutoPad , MS_BOOL bInv16)
{
    HAL_CIPHER_HASHMODE hash_mode = E_HASH_SHA1;
    hash_mode = _HAL_CIPHER_TransHASHAlgo(algo);

    _uHashCtrl0.HashCtrl0.HashSel = hash_mode ;
    _uHashCtrl0.HashCtrl0.AutoPad = bAutoPad ;
    _uHashCtrl0.HashCtrl0.Inv16 = bInv16; //Inverse the byte order per 16 bytes

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH Mode[%d], AutoPad[%d], Inverse16[%d]!\n",
        __func__, __LINE__, hash_mode, bAutoPad, bInv16));
}

void HAL_CIPHER_Hash_Set_ReportMode(MS_BOOL RptInDram, MS_U32 u32DramAddr)
{
    if (RptInDram)
    {
        _u32ReportAddr = u32DramAddr;

    }
    else
    {
        _u32ReportAddr = 0;
    }

    _uHashCtrl2.HashCtrl2.RR = RptInDram ;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set report mode[%d], report address[0x%08x]\n",
        __func__, __LINE__, RptInDram, (unsigned int)_u32ReportAddr));
}

MS_BOOL HAL_CIPHER_Hash_SetHMACKey(DRV_CIPHER_HMAC_KEY stHMACKey, CIPHER_HMAC_KPAD eKpad, MS_BOOL bClear)
{
	MS_BOOL bHMAC = TRUE;
    MS_U32 u32HMACKeySel = 0;
    MS_U32 u32Kpad = CMD_HASH_IKP;
    HAL_CIPHER_KEYSRC KeySrc;

	 //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stHMACKey.eKeySrc) && (stHMACKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stHMACKey.eKeySrc) && (stHMACKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid KeySrc[%d], Index[%d]\n",
           __func__, __LINE__, stHMACKey.eKeySrc, stHMACKey.u8KeyIdx));
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stHMACKey.eKeySrc, stHMACKey.u8KeyIdx);

    switch(KeySrc)
    {
        case E_DMA_KSEL_REGKEY:
            u32HMACKeySel = CMD_HMAC_KSEL_HK;
            break;
        case E_DMA_KSEL_STRN:
            u32HMACKeySel = CMD_HMAC_KSEL_STRN;
            break;
        case E_DMA_KSEL_MK0:
            u32HMACKeySel = CMD_HMAC_KSEL_MK0;
            break;
        case E_DMA_KSEL_MK1:
            u32HMACKeySel = CMD_HMAC_KSEL_MK1;
            break;
        case E_DMA_KSEL_CCCK:
            u32HMACKeySel = CMD_HMAC_KSEL_CCCK;
            break;
        case E_DMA_KSEL_SK0:
            u32HMACKeySel = CMD_HMAC_KSEL_SK0;
            break;
        case E_DMA_KSEL_SK1:
            u32HMACKeySel = CMD_HMAC_KSEL_SK1;
            break;
        case E_DMA_KSEL_SK2:
            u32HMACKeySel = CMD_HMAC_KSEL_SK2;
            break;
        case E_DMA_KSEL_SK3:
            u32HMACKeySel = CMD_HMAC_KSEL_SK3;
            break;
        default:
            u32HMACKeySel = 0;
            break;
    }

    if(eKpad == E_CIPHER_HMAC_OKPAD)
    {
        u32Kpad = CMD_HASH_OKP;
    }

    if(bClear)
    {
        u32HMACKeySel += (MS_U32)CMD_HMAC_KSEL_CLR_SK;
    }

    if(KeySrc == E_DMA_KSEL_REGKEY)
    {
		//command mode
        if (0 != stHMACKey.pu8KeyData)
        {
            memset(_pu8HashHMACHostKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8HashHMACHostKey, (MS_U8 *)stHMACKey.pu8KeyData , stHMACKey.u8KeyLen);
            _uHashCtrl0.HashCtrl0.DK = CMD_HMAC_KSEL_DIR_HK;
        }
        else
        {
            memset(_pu8HashHMACHostKey, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            bHMAC = FALSE;
        }
    }

    _uHashCtrl0.HashCtrl0.HMAC = bHMAC;
    _uHashCtrl0.HashCtrl0.KPAD = u32Kpad;
    _uHashCtrl0.HashCtrl0.HMAC_KeySel = u32HMACKeySel;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HMAC[%d], HMAC KPAD[%u] HMAC KeySel[%u]!\n",
        __func__, __LINE__, bHMAC, u32Kpad, u32HMACKeySel));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_CaVid(MS_U32 u32CAVid)
{
    MS_U32 u32Tmp = u32CAVid;

    if(u32Tmp > HAL_CRYPTODMA_CAVID_MAX)
    {
        return FALSE;
    }

    if(u32Tmp == 0)
    {
        u32Tmp = _CIPHER_CAVid ;
    }
    else
    {
        u32Tmp = u32CAVid;
    }

    _uHashCtrl1.HashCtrl1.CaVid = u32Tmp;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set CAVid [0x%x]\n",
        __func__, __LINE__, (unsigned int)u32Tmp));

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_IV(MS_U8* pu8IV, MS_U32 u32IVSize ,MS_U32 u32IVSel)
{
    // 256 bits for sha256 , 160 bits for sha1
    _uHashCtrl0.HashCtrl0.InitHashSel = u32IVSel ;

	HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH IVSel [%u]\n",
        __func__, __LINE__, u32IVSel));

    if(u32IVSel == CMD_HASH_IV_CMD)
    {
        if((pu8IV != NULL) && (u32IVSize > 0))
        {
            memset(_pu8HashIV, 0x0, HASH_CMD_IV_SIZE);
            memcpy(_pu8HashIV, (MS_U8 *)pu8IV , u32IVSize);

            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}


MS_BOOL HAL_CIPHER_Hash_Set_InitWordCnt(HAL_CIPHER_IWCTYPE eIWCType, MS_U32 u32StartBytes)
{
	MS_U32 u32IWCType = 0;
    MS_U32 u32IWC = 0;
    MS_U32 Rpt[2] = {0};

    if(eIWCType == E_CIPHER_HASH_IWC_PRV) //Init Word Count from report
    {
        u32IWCType = CMD_HASH_IWC_RPT;
        HAL_CIPHER_Hash_GetRpt(Rpt, 2); //For debug
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IWC from report is [%u]\n",
        __func__, __LINE__, Rpt[1]>>2));
	}
    else
    {
        u32IWCType = CMD_HASH_IWC_CMDQ;
        if((u32StartBytes % HASH_MANUAL_BLOCK_SIZE) != 0)
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Init word count is 64 bytes alignment!\n",
                __func__, __LINE__));
            return FALSE;
        }

        if(u32StartBytes != 0)
        {
            u32IWC = u32StartBytes/4; //Init Word Count
        }
        else
        {
            u32IWC = 0;
        }
    }

    _u32InitWordCnt = u32IWC;
    _uHashCtrl0.HashCtrl0.IWC_Sel = u32IWCType;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IWC type[%u], IWC[%u]\n",
        __func__, __LINE__, u32IWCType, _u32InitWordCnt));

    return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_MsgSrcFrom(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashSrc)
{
    MS_U32 u32SrcTmp = 0;
    switch(eMemType)
    {
    	case E_CIPHER_DRAM:
    		u32SrcTmp = CMD_HASH_SRC_DRAM;
            _u32HashSrcAddr = 0;
    		break;
        case E_CIPHER_DQMEM:
            u32SrcTmp = CMD_HASH_SRC_R2;
            _u32HashSrcAddr = HAL_CIPHER_BASE_DQMEM;
            break;
        case E_CIPHER_IQMEM:
            u32SrcTmp = CMD_HASH_SRC_R2;
            _u32HashSrcAddr = HAL_CIPHER_BASE_IQMEM;
            break;
        case E_CIPHER_DIRECT:
            u32SrcTmp = CMD_HASH_SRC_REG;
            _u32HashSrcAddr = 0;
            break;
        default:
            u32SrcTmp = CMD_HASH_SRC_DRAM;
            _u32HashSrcAddr = 0;
            break;
    }

    *u32HashSrc = u32SrcTmp;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH source from [%u]\n",
        __func__, __LINE__, u32SrcTmp));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_OutputDstTo(CIPHER_MEM_TYPE eMemType, MS_U32 *u32HashDst)
{
    MS_U32 u32DstTmp = 0;
    switch(eMemType)
    {
        case E_CIPHER_DRAM:
    		u32DstTmp = CMD_HASH_OUTPUT_DRAM;
            _u32HashDestAddr = 0;
    		break;
        case E_CIPHER_DQMEM:
            u32DstTmp = CMD_HASH_OUTPUT_R2;
            _u32HashDestAddr = HAL_CIPHER_BASE_DQMEM;
            break;
        case E_CIPHER_IQMEM:
            u32DstTmp = CMD_HASH_OUTPUT_R2;
            _u32HashDestAddr = HAL_CIPHER_BASE_IQMEM;
            break;
        case E_CIPHER_DIRECT:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;
            _u32HashDestAddr = 0;
            break;
        default:
            u32DstTmp = CMD_HASH_OUTPUT_RPT;
            _u32HashDestAddr = 0;
            break;
    }

    *u32HashDst = u32DstTmp;
    _uHashCtrl0.HashCtrl0.Dest = u32DstTmp;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH destination[%u]\n",
        __func__, __LINE__, u32DstTmp));

	return TRUE;
}

MS_BOOL HAL_CIPHER_Hash_Set_OuputAddr(MS_PHY u32OutputPAddr, MS_U32 u32DstSel)
{
    if ((CMD_HASH_OUTPUT_DRAM == u32DstSel) || (CMD_HASH_OUTPUT_R2 == u32DstSel))
    {
        if (0 != u32OutputPAddr)
        {
            _u32HashDestAddr += u32OutputPAddr;
        }
        else
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Hash output address is NULL\n",__func__, __LINE__));
            return FALSE;
        }
    }
    return TRUE;
}

void HAL_CIPHER_Hash_SetMsgLength( MS_U32 u32Size )
{
	_u32HashMsgLen = u32Size;

	HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH message length[%u]\n",
        __func__, __LINE__, u32Size));

	return ;
}

void HAL_CIPHER_Hash_SetHOS(MS_BOOL bHos)
{
	_uHashCtrl0.HashCtrl0.HOS = bHos;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HOS[%d]\n",
        __func__, __LINE__, bHos));

	return ;
}


MS_BOOL HAL_CIPHER_Hash_SetMsg(MS_PHY u32MsgPAddr, MS_U32 u32Size ,MS_U32 u32SrcSel )
{
    _uHashCtrl0.HashCtrl0.Src = u32SrcSel;
    _u32HashMsgLen = u32Size;

    if ((CMD_HASH_SRC_DRAM == u32SrcSel) || (CMD_HASH_SRC_R2 == u32SrcSel))
    {
        if (0 != u32MsgPAddr)
        {
            _u32HashSrcAddr += u32MsgPAddr;
        }
        else
        {
            return FALSE;
        }
    }
    else if (CMD_HASH_SRC_REG == u32SrcSel)
    {
        if((u32MsgPAddr == 0) || (u32Size > HASH_CMD_MSG_SIZE))
        {
            HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Error:Set HASH direct message size[%u] over 64 bytes\n",
                __func__, __LINE__, u32Size));
            return FALSE;
        }
        _u32HashSrcAddr = 0;
        memset(_pu8HashMsg , 0x0 , HASH_CMD_MSG_SIZE);
        memcpy(_pu8HashMsg , (MS_U8 *)(MsOS_PA2KSEG1(u32MsgPAddr)) , u32Size);
    }

	HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set HASH message source[%u], size[%u]\n",
        __func__, __LINE__, u32SrcSel, _u32HashMsgLen));

    return TRUE;

}


MS_BOOL HAL_CIPHER_Hash_Start(HAL_CIPHER_INTMODE IntMode , MS_BOOL bWaitCmdQ , MS_BOOL bRst, MS_U16 u16CmdID)
{
        MS_U32 u32SpareCnt = 0 , CmdLen = 0 , i = 0 ;
        MS_U32 u32Tmp;

        _uHashCtrl1.HashCtrl1.TID = u16CmdID;
		_uHashCtrl0.HashCtrl0.CMD =  CMD_HASH_START_RR ;

        _uHashCtrl0.HashCtrl0.OVT = FALSE ;
        _uHashCtrl0.HashCtrl0.IntM = IntMode ;

        CmdLen = _HAL_CIPHER_GetCmdLen(E_CIPHER_TYPE_SHA);

        _uHashCtrl0.HashCtrl0.CL  = CmdLen & CMD_LEN_MASK ;

		_uHashCtrl1.HashCtrl1.NS =  0;

        MS_U32 MIUCrossBar = REG32(_u32RegBase + REG_MIU_CROSSBAR);  //MIU CrossBar Bank 0x1613
        REG32(_u32RegBase + REG_MIU_CROSSBAR) = MIUCrossBar | 0xf;  //enable MIU crossbar


SHA_RETRY:
        u32SpareCnt = REG32(&_AESDMACtrl->SpareCnt) ;

        if (u32SpareCnt > CmdLen)
        {
            u32SpareCnt -= CmdLen ;

            WRITE_CMDQ( (_uHashCtrl0.u32HashCtrl0)) ;

            WRITE_CMDQ( (_uHashCtrl1.u32HashCtrl1)) ;

			WRITE_CMDQ( (_uHashCtrl2.u32HashCtrl2)) ;

            WRITE_CMDQ(_u32HashMsgLen) ;

			//SRC
            if (_uHashCtrl0.HashCtrl0.Src != CMD_HASH_SRC_REG)
            {
                WRITE_CMDQ(_u32HashSrcAddr) ;
            }

			//RR address
            if (_uHashCtrl2.HashCtrl2.RR == TRUE)
            {
                WRITE_CMDQ(_u32ReportAddr) ;
            }

			// IWC
            if (CMD_HASH_IWC_CMDQ == _uHashCtrl0.HashCtrl0.IWC_Sel)
            {
               WRITE_CMDQ(_u32InitWordCnt);
            }
            else
            {
                REG32(&_AESDMACtrl->Hash_Reportp[1]) = _u32InitWordCnt;
            }

			// DST
            if ((CMD_HASH_OUTPUT_DRAM == _uHashCtrl0.HashCtrl0.Dest) || (_uHashCtrl0.HashCtrl0.Dest == CMD_HASH_OUTPUT_R2 ))
            {
                WRITE_CMDQ(_u32HashDestAddr);
            }

            // HASH Initial Vector from CmdQ
            if (CMD_HASH_IV_CMD == _uHashCtrl0.HashCtrl0.InitHashSel )
            {
                for ( i = 0 ; i < (HASH_CMD_IV_SIZE/4) ; i ++  )
                {
                    u32Tmp = ((_pu8HashIV[4*i+0]<<24)|(_pu8HashIV[4*i+1]<<16)|(_pu8HashIV[4*i+2]<<8)|(_pu8HashIV[4*i+3]) );

                    WRITE_CMDQ(u32Tmp);
                }
            }

            // HASH Data from CmdQ
            if ( CMD_HASH_SRC_REG == _uHashCtrl0.HashCtrl0.Src )
            {
                for ( i = 0 ; i < (HASH_CMD_MSG_SIZE/4) ; i ++  )
                {
					 u32Tmp = (  (_pu8HashMsg[i*4+0]<<24)
					 			|(_pu8HashMsg[i*4+1]<<16)
					 			|(_pu8HashMsg[i*4+2]<<8)
					 			|(_pu8HashMsg[i*4+3]<<0)
					 		  );

                     WRITE_CMDQ(u32Tmp);
				}
            }

			if((CMD_HMAC_KSEL_HK == _uHashCtrl0.HashCtrl0.HMAC_KeySel) && (CMD_HMAC_KSEL_DIR_HK == _uHashCtrl0.HashCtrl0.DK))
            {
                for ( i = 0 ; i < (HASH_CMD_HMAC_HOSTKEY_SIZE/4) ; i ++  )
                {
                    u32Tmp = ((_pu8HashHMACHostKey[4*i+0]<<24)|(_pu8HashHMACHostKey[4*i+1]<<16)|(_pu8HashHMACHostKey[4*i+2]<<8)|(_pu8HashHMACHostKey[4*i+3]) );

                    WRITE_CMDQ(u32Tmp);
                }
            }
            // [HW] Hardware will check Current spare count != Write back Spare count to start operation.
            REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;
            return TRUE ;

        }
        else
        {
            // [HW] Check Current spare count == Write back Spare count to start wait cmd
            REG32(&_AESDMACtrl->SpareCnt) = u32SpareCnt ;

            if (bWaitCmdQ)
            {
                //  Retry until CMDQ avaliable
                HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]HASH cmd retry!\n",
                    __func__, __LINE__));
                goto SHA_RETRY;
            }
            else
            {
                //  Return Fail, Command abort
                return FALSE ;
            }
        }
}

MS_BOOL HAL_CIPHER_Hash_CmdDone(MS_U32 u32CmdID, MS_U32 *u32Ret)
{
    MS_U32 u32ExcTmp = 0;

    MS_U32 u32Tmp = REG32(&_AESDMACtrl->Hash_Reportp[0]) ;
    MS_U32 u32RptId = u32Tmp & REG_CIPHER_RPT_THREAD_MSK;

    if(u32RptId == u32CmdID)
    {
    	u32ExcTmp = (MS_U32)REG32(&_AESDMACtrl->Dma_Ctrl[REG_EXCEPT_FLAG]);
        *u32Ret = u32ExcTmp;
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_WARN, printf("[%s][%d]HASH Cmd[%u] Not done!ReportId[%u]\n",
                    __func__, __LINE__, u32CmdID, u32RptId));
        return FALSE;
    }

    return TRUE;
}

void HAL_CIPHER_Hash_ExceptFilter(MS_U32 *pu32Exception, CIPHER_KEY_SRC eKeySrc, MS_U8 u8KeyIdx)
{
    //Because CryptoDMA will check every key slot usage for HMAC
    //We need to filter out the correct key exception
    MS_U32 u32ExcTmp = 0;
    HAL_CIPHER_KEYSRC KeySrc = 0;
    MS_U32 i = 0;

    u32ExcTmp = *pu32Exception;

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(eKeySrc, u8KeyIdx);

    //Only check HMAC key exception
    for(i = E_CIPHER_disable_masterkey0;i<=E_CIPHER_disable_reg_key_3;i++) //check each error flag
    {
        if((u32ExcTmp & (1 << i)) >> i)
        {
            switch(i)
            {
                case E_CIPHER_disable_masterkey0:
                    if(E_DMA_KSEL_MK0 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey0));
					break;

                case E_CIPHER_disable_masterkey1:
                    if(E_DMA_KSEL_MK1 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_masterkey1));
					break;

                case E_CIPHER_disable_cck:
                    if(E_DMA_KSEL_CCCK != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_cck));
					break;

                 case E_CIPHER_disable_reg_key_0:
                    if(E_DMA_KSEL_SK0 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_0));
					break;

                case E_CIPHER_disable_reg_key_1:
                    if(E_DMA_KSEL_SK1 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_1));
					break;

                case E_CIPHER_disable_reg_key_2:
                    if(E_DMA_KSEL_SK2 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_2));
					break;

                case E_CIPHER_disable_reg_key_3:
                    if(E_DMA_KSEL_SK3 != KeySrc)
	                    u32ExcTmp &= (~(1 << E_CIPHER_disable_reg_key_3));
                    break;

                default:
                    break;
            }
        }
    }

    *pu32Exception = u32ExcTmp;

    return;

}
// =====================================
//  Parser function implementation
// =====================================
void HAL_CIPHER_PARSER_Set_Mode(CIPHER_PARSER_MODE eMode)

{
    HAL_CIPHER_PARSER_HDCPMODE eHDCPMode = 0;
    HAL_CIPHER_PARSER_TSMODE eTSMode = 0;
    HAL_CIPHER_PARSER_PKTMODE ePKTMode = 0;
    HAL_CIPHER_PARSER_AUTOMODE eAutoMode = 0;
    HAL_CIPHER_PARSER_ITMODE eITMode = 0;
    HAL_CIPHER_PARSER_CLEARMODE eClearMode = 0;

    switch(eMode)
    {
        case E_CIPHER_PARSER_TS_PKT192:
            eHDCPMode = E_PARSER_HDCPMODE_NONE;
            eTSMode = E_PARSER_TSMODE_TS;
            ePKTMode = E_PARSER_PKTMODE_192;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_NONE;
            break;
        case E_CIPHER_PARSER_TS_PKT192_CLEAR:
            eHDCPMode = E_PARSER_HDCPMODE_NONE;
            eTSMode = E_PARSER_TSMODE_TS;
            ePKTMode = E_PARSER_PKTMODE_192;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_EN;
            break;
        case E_CIPHER_PARSER_TS_PKT188:
            eHDCPMode = E_PARSER_HDCPMODE_NONE;
            eTSMode = E_PARSER_TSMODE_TS;
            ePKTMode = E_PARSER_PKTMODE_188;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_NONE;
            break;
        case E_CIPHER_PARSER_TS_PKT188_CLEAR:
            eHDCPMode = E_PARSER_HDCPMODE_NONE;
            eTSMode = E_PARSER_TSMODE_TS;
            ePKTMode = E_PARSER_PKTMODE_188;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_EN;
            break;
        case E_CIPHER_PARSER_HDCP20_PKT192:
            eHDCPMode = E_PARSER_HDCPMODE_HDCP20;
            eTSMode = E_PARSER_TSMODE_PES;
            ePKTMode = E_PARSER_PKTMODE_192;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_NONE;
            break;
        case E_CIPHER_PARSER_HDCP20_PKT192_CLEAR:
            eHDCPMode = E_PARSER_HDCPMODE_HDCP20;
            eTSMode = E_PARSER_TSMODE_PES;
            ePKTMode = E_PARSER_PKTMODE_192;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_NONE;
            eClearMode = E_PARSER_CLEARMODE_EN;
            break;
        case E_CIPHER_PARSER_HDCP20_PKT188:
            eHDCPMode = E_PARSER_HDCPMODE_HDCP20;
            eTSMode = E_PARSER_TSMODE_PES;
            ePKTMode = E_PARSER_PKTMODE_188;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_EN;
            eClearMode = E_PARSER_CLEARMODE_NONE;
            break;
        case E_CIPHER_PARSER_HDCP20_PKT188_CLEAR:
            eHDCPMode = E_PARSER_HDCPMODE_HDCP20;
            eTSMode = E_PARSER_TSMODE_PES;
            ePKTMode = E_PARSER_PKTMODE_188;
            eAutoMode = E_PARSER_AUTOMODE_EN;
            eITMode = E_PARSER_ITMODE_NONE;
            eClearMode = E_PARSER_CLEARMODE_EN;
            break;
        default:
            printf("Error: Not suport parser mode!\n");
            break;
    }

    _uDmaCtrl2.DmaCtrl2.HP   = TRUE;
    _uDmaCtrl2.DmaCtrl2.HDCP = eHDCPMode;
    _uDmaCtrl2.DmaCtrl2.TS   = eTSMode;
    _uDmaCtrl2.DmaCtrl2.P192 = ePKTMode;
    _uDmaCtrl2.DmaCtrl2.AT   = eAutoMode;
    _uDmaCtrl2.DmaCtrl2.IT   = eITMode;
    _uDmaCtrl2.DmaCtrl2.CR   = eClearMode;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Parser config:HDCP[%d], TS[%d], P192[%d], AT[%d], IT[%d], ClearMode[%d]\n",
        __func__, __LINE__, eHDCPMode, eTSMode, ePKTMode, eAutoMode, eITMode, eClearMode));


}

void HAL_CIPHER_PARSER_Set_MaskSCB(MS_BOOL bEnable)
{
    _uDmaCtrl2.DmaCtrl2.TsSbMk = bEnable;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Enable MaskSCB[%d]!\n",
                __func__, __LINE__, bEnable));
}

void HAL_CIPHER_PARSER_Set_SCB(CIPHER_PARSER_SCB eSCB)
{
    MS_U8 u8SCB = 0;

    if(eSCB == E_CIPHER_PARSER_SCB_10)
    {
        u8SCB = CMD_PARSER_SCB10;
    }
    else if(eSCB == E_CIPHER_PARSER_SCB_11)
    {
        u8SCB = CMD_PARSER_SCB11;
    }
    else
    {
        u8SCB = 0;
    }
    _uDmaCtrl2.DmaCtrl2.TsSbPn = u8SCB;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set SCB[%u]!\n",
                __func__, __LINE__, u8SCB));
}

void HAL_CIPHER_PARSER_Set_ForceSCB(MS_BOOL bInsert, CIPHER_PARSER_SCB eSCB)
{
    MS_U8 u8SCB = 0;

    if(bInsert)
    {
        if(eSCB == E_CIPHER_PARSER_SCB_10)
        {
            u8SCB = CMD_PARSER_SCB10;
        }
        else if(eSCB == E_CIPHER_PARSER_SCB_11)
        {
            u8SCB = CMD_PARSER_SCB11;
        }
        else
        {
            u8SCB = 0;
        }
    }

    _uDmaCtrl2.DmaCtrl2.InSb = bInsert;
    _uDmaCtrl2.DmaCtrl2.TsInSb = u8SCB;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set Insert Enable[%d], SCB[%u]!\n",
                __func__, __LINE__, bInsert, u8SCB));

}

void HAL_CIPHER_PARSER_Rmv_SCB(MS_BOOL bRemove)
{
    _uDmaCtrl2.DmaCtrl2.RmSb = bRemove;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Remove SCB[%d]!\n",
                __func__, __LINE__, bRemove));
}

void HAL_CIPHER_PARSER_BypassPid(MS_BOOL bEnable)
{
    _uDmaCtrl2.DmaCtrl2.BPS = bEnable;
    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Bypass PID[%d]!\n",
                __func__, __LINE__, bEnable));
}

void HAL_CIPHER_PARSER_Set_PID(CIPHER_PARSER_PID stPID)
{
    if(stPID.pu8PID0 != NULL)
    {
        memset(_pu8PID0, 0x0 , PID_SIZE );
        memcpy(_pu8PID0, stPID.pu8PID0, PID_SIZE);
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set PID0 NULL!\n",
                    __func__, __LINE__));
    }

    if(stPID.pu8PID1 != NULL)
    {
        memset(_pu8PID1, 0x0 , PID_SIZE );
        memcpy(_pu8PID1, stPID.pu8PID1 , PID_SIZE);
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set PID1 NULL!\n",
                    __func__, __LINE__));
    }
}

MS_BOOL HAL_CIPHER_PARSER_Set_Key2(DRV_CIPHER_KEY stKey)
{
    HAL_CIPHER_KEYSRC KeySrc = 0;
    HAL_CIPHER_ALGO algo = 0;
    MS_BOOL bDK2 = FALSE;
    MS_BOOL bTwoKey = FALSE;

    algo = _uDmaCtrl0.DmaCtrl0.Algo;

    if(E_DMA_ALGO_NONE == algo)
    {
        //No key
        bDK2 = FALSE;
        bTwoKey = FALSE;
        memset(_pu8CipherKey2, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX);
        goto SET_KEY_DONE;
    }

    //Check Key Source, Index
    if(((E_CIPHER_KSRC_KL == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_DMA_KEY_SLOT-1)) ||
       ((E_CIPHER_KSRC_OTP == stKey.eKeySrc) && (stKey.u8KeyIdx > HAL_CRYPTODMA_OTP_SCK_NUM-1)))
    {
       HAL_DEBUG_CIPHER(CIPHER_DBG_ERR, printf("[%s][%d]Invalid KeySrc[%d], Index[%d]\n",
           __func__, __LINE__, stKey.eKeySrc, stKey.u8KeyIdx));
       return FALSE;
    }

    //DRV to HAL Type transfer
    KeySrc = _HAL_CIPHER_TransKey(stKey.eKeySrc, stKey.u8KeyIdx);

    // key from CPU
    if(E_DMA_KSEL_REGKEY == KeySrc)
    {
        if ((0 != stKey.pu8KeyData) && (stKey.u8KeyLen != 0) && (stKey.u8KeyLen <= HAL_CRYPTODMA_KEYLEN_MAX) )
        {
            memset(_pu8CipherKey2, 0x0 , HAL_CRYPTODMA_KEYLEN_MAX );
            memcpy(_pu8CipherKey2, (MS_U8 *)stKey.pu8KeyData , stKey.u8KeyLen);
            bDK2 = TRUE;
            KeySrc = 0;
        }
        else
        {
            return FALSE ;
        }
    }
    else if(E_DMA_KSEL_CAIP == KeySrc)
    {
        bDK2 = FALSE ;
        KeySrc = 0 ;
    }
    else// key from KL or OTP
    {
        bDK2 = FALSE;
    }

    bTwoKey = TRUE;

SET_KEY_DONE:

	_uDmaCtrl3.DmaCtrl3.DK2 = bDK2;
    _uDmaCtrl3.DmaCtrl3.KeySel2 = KeySrc;
    _uDmaCtrl2.DmaCtrl2.TK = bTwoKey;

    HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set DK2[%d], KeySel2[%d]\n",
        __func__, __LINE__, bDK2, KeySrc));

    return TRUE ;

}

MS_BOOL HAL_CIPHER_PARSER_Set_IV2(MS_U8 *pu8IV2, MS_U8 u8IVLen)
{
    if((pu8IV2 != NULL) && (u8IVLen != 0))
    {
        memset(_pu8IV2, 0, CIPHER_KEY_SIZE);
        memcpy(_pu8IV2, pu8IV2, u8IVLen);
        _uDmaCtrl3.DmaCtrl3.DIV2 = TRUE;

        return TRUE;
    }
    else
    {
        HAL_DEBUG_CIPHER(CIPHER_DBG_INFO, printf("[%s][%d]Set IV2 NULL!\n",
                        __func__, __LINE__));
        return FALSE;
    }

}

void HAL_CIPHER_PARSER_Set_ClearStartMode(MS_BOOL bEnable)
{
    _uDmaCtrl3.DmaCtrl3.CRS = bEnable;
}

// =====================================
//  MISC function implementation
// =====================================
MS_BOOL HAL_CIPHER_Misc_Random(MS_U8 *pu8Buf, MS_U32 u32Size)
{
    MS_U16 u16TRN = 0;
    MS_U16 u16PreTRN = 0;
    MS_U32 u32Count = 0;

    if(NULL == pu8Buf || 0 == u32Size)
        return FALSE;

#if defined (MCU_AEON)

    while(u32Count < u32Size)
    {
        do{
            do{
                //Toggle random number bit
                REG32(_u32RegBase+REG_RNG_TRNG_SCPU) ^= REG_RNG_TRNG_ACK_SCPU;
            }//Read random number valid
            while( !(REG32(_u32RegBase+REG_RNG_TRNG_SCPU) & REG_RNG_TRNG_VALID_SCPU_MASK) );
        }
        while( (u16TRN = (REG32(_u32RegBase+REG_RNG_TRNG_SCPU) & REG_RNG_TRNG_OUT_SCPU_MASK))==u16PreTRN );
        u16PreTRN = u16TRN;

        if(0 == ((u32Size - u32Count)%2UL))
        {
            *(pu8Buf+u32Count)   = u16TRN & 0xFFUL;
            *(pu8Buf+u32Count+1) = (u16TRN & 0xFF00UL) >> 8;
            u32Count += 2;
        }
        else
        {
            *(pu8Buf+u32Count) = u16TRN & 0xFFUL;
            u32Count += 1;
        }
    }

#else

    while(u32Count < u32Size)
    {
        do{
            do{
                //Toggle random number bit
                REG32(_u32RegBase+REG_RNG_TRNG_ACPU) ^= REG_RNG_TRNG_ACK_ACPU;
            }//Read random number valid
            while( !(REG32(_u32RegBase+REG_RNG_TRNG_ACPU) & REG_RNG_TRNG_VALID_ACPU_MASK) );
        }
        while( (u16TRN = (REG32(_u32RegBase+REG_RNG_TRNG_ACPU) & REG_RNG_TRNG_OUT_ACPU_MASK))==u16PreTRN );
        u16PreTRN = u16TRN;

        if(0 == ((u32Size - u32Count)%2UL))
        {
            *(pu8Buf+u32Count)   = u16TRN & 0xFFUL;
            *(pu8Buf+u32Count+1) = (u16TRN & 0xFF00UL) >> 8;
            u32Count += 2;
        }
        else
        {
            *(pu8Buf+u32Count) = u16TRN & 0xFFUL;
            u32Count += 1;
        }
    }
#endif

    return TRUE;
}


