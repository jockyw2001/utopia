//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDMX_tsio.h
/// @brief  TSIO API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_DMX_TSIO_H__
#define __API_DMX_TSIO_H__

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum
{
    TSIO_CC_RESULT_OK,
    TSIO_CC_RESULT_NORESULT,
    TSIO_CC_RESULT_TXCMD_TOOSMALL,
    TSIO_CC_RESULT_TXCMD_TOOBIG,
    TSIO_CC_RESULT_TXCMD_TIMEOUT,
    TSIO_CC_RESULT_RXCMD_TOOSMALL,
    TSIO_CC_RESULT_RXCMD_TOOBIG,
    TSIO_CC_RESULT_RXCMD_TIMEOUT,
    TSIO_CC_RESULT_ERR_FLAG,
    TSIO_CC_RESULT_ERROR,
}TSIO_CC_RESULT;

typedef enum
{
    TSIO_CLK_200M = 0,
    TSIO_CLK_100M,
    TSIO_CLK_50M,
    TSIO_CLK_25M,
}TSIO_CLK;

typedef enum
{
    TSIO_CMD_BYPASSSMC,         //bypass SMC setting, 0: disable, 1: direct8, 2: direct 16, 3: loopback.
    TSIO_CMD_STUFFINGSID,       //stuffing id (from 0 ~ 0x3F)
    TSIO_CMD_LOCDECRYPT,        //1: enable loc decrypt, 0: disable
    TSIO_CMD_SYNCTHRESHOLD,     //sync threshold (3 ~ 8)
    TSIO_CMD_DBGLEVEL,          //3: trace, 2: info, 1: error, 0: release


    TSIO_CMD_PHASE_ADJUST,      //kastor U01 use
    TSIO_CMD_BITTRAINING_ADJUST,//kastor U01 use
}TSIO_CMD;

typedef enum
{
    TSIO_INT_TYPE_NONE = 0,
    TSIO_INT_TYPE_SGDMAOUT_LASTNODE,
    TSIO_INT_TYPE_SGDMAOUT_NODEINT,
    TSIO_INT_TYPE_SGDMAOUT_DROP,
    TSIO_INT_TYPE_SGDMAOUT_DMAEND,
    TSIO_INT_TYPE_SGDMAIN_DONE,
    TSIO_INT_TYPE_CC_TX_DONE,
    TSIO_INT_TYPE_CC_RX_DONE,
    TSIO_INT_TYPE_LIVE_TSO_OVERFLOW,
    TSIO_INT_TYPE_OPERATION_START,
}TSIO_INT_TYPE;

typedef enum
{
    TSIO_INFO_LOSELOCKCNT = 0,      //lose locked count
    TSIO_INFO_USEBUFLEN = 1,        //get use buffer size, arg is service handleid from 0 to 63
}TSIO_INFO;

typedef struct
{
    MS_PHY phyLastpktBufAddr; // 192 bytes (pkt size) x 64 (service num)
    MS_U32 u32LastpktBufSize;
    MS_PHY phyDMAVQBufAddr;
    MS_U32 u32DMAVQBufSize;
    MS_PHY phyDMAinNodeAddr; // 1 node need 16 bytes.
    MS_U32 u32DMAinNodeSize;
    MS_PHY phyDMAoutNodeAddr; // 1 node need 16 bytes.
    MS_U32 u32DMAoutNodeSize;
}TSIO_Init_Set;

typedef struct
{
    MS_U8* pIn;
    MS_U16 u16InLen;
    MS_U8* pOut;
    MS_U16* pOutLen;
    MS_U32 u32Timeout;
}TSIO_CC_Set;

typedef enum
{
    E_TSIO_SERVICEDEST_LIVE = 0,
    E_TSIO_SERVICEDEST_DMA,
}TSIO_SERVICEDEST;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bTSmode;
    MS_BOOL bMidPriority;
    MS_BOOL bLastNodeFlush;
    MS_BOOL bNewPkt;
    MS_BOOL bBypassAll;
}TSIO_DMAIN_SET;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bSecurity;
}TSIO_DMAOUT_SET;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bEvenkeyenable;
    MS_U8 *u8Evenkey;
    MS_U16 u16Evenkeylen;
    MS_BOOL bOddkeyenable;
    MS_U8 *u8Oddkey;
    MS_U16 u16Oddkeylen;
}TSIO_LOCKEY_SET;

typedef void(*TSIOIntTypeCB)(TSIO_INT_TYPE eType, MS_U32 u32Info, void *ext1, void *ext2);

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#ifdef DMX_UTOPIA_20
#include "apiDMX_tsio_v2.h"
#endif

//-------------------------------------------------------------------------------------------------
/// Register TSIO callback function
/// @param TSIOIntTypeCB                                  \b IN: Callback function pointer
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_RegisterIntCb(TSIOIntTypeCB efn);

//-------------------------------------------------------------------------------------------------
/// Init TSIO
/// @param eParm                         \b IN: init param
/// @return DMX_FILTER_STATUS
/// @note
/// It should be called before calling any TSIO API.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Init(TSIO_Init_Set *eParm);

//-------------------------------------------------------------------------------------------------
/// Open TSIO
/// @param eClk                         \b IN: smc clk speed
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Open(TSIO_CLK eClk);

//-------------------------------------------------------------------------------------------------
/// Exit TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Close TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Close(void);

//-------------------------------------------------------------------------------------------------
/// Send ctrl command to TSIO
/// @param eCmd                                     \b IN: Command
/// @param u32Value                                \b IN: Commnad's param
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Cmd(TSIO_CMD eCmd, MS_U32 u32Value);

//-------------------------------------------------------------------------------------------------
/// Send and Recv C&C haed-based cmd
/// @param Parm                                        \b IN: Pointer to store C&C cmd address
/// @param pResult                                     \b IN: Pointer to store result status
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_CC(TSIO_CC_Set *Parm, TSIO_CC_RESULT* pResult);

//-------------------------------------------------------------------------------------------------
/// Get TSIO informaiton
/// @param eInfo                                        \b IN: Info type
/// @param u32Arg                                      \b IN: detail value
/// @param u32Retinfo                                 \b OUT: Pointer to store Info address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_GetInfo(TSIO_INFO eInfo , MS_U32 u32Arg, MS_U32 *u32Retinfo);

//-------------------------------------------------------------------------------------------------
/// Set DMAOUT Virture channel to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDMAoutVC(MS_U16 u16Handle, TSIO_DMAOUT_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// Get DMAOUT VC  write address
/// @param u16Handle                                    \b IN: Service handle ID
/// @param pphyWrite                                    \b OUT: dma out write address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAout_WriteGet(MS_U16 u16Handle, MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set destination to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDestination(MS_U16 u16Handle, TSIO_SERVICEDEST eDest);

//-------------------------------------------------------------------------------------------------
/// open DMAIN pid filter for TSIO service
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param u16Pid                                       \b IN:  PID value of DMAIN PID filter
/// @param pu16FltId                                    \b OUT: Pointer to DMAIN PID filter ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAin_PidOpen(MS_U16 u16Handle, MS_U16 u16Pid, MS_U16 *pu16FltId);

//-------------------------------------------------------------------------------------------------
/// close DMAIN pid filter for TSIO service
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param u16FltId                                     \b IN:  DMAIN PID filter ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAin_PidClose(MS_U16 u16Handle, MS_U16 u16FltId);

//-------------------------------------------------------------------------------------------------
/// Set DMAIN Virture channel to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma in param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetDMAinVC(MS_U16 u16Handle, TSIO_DMAIN_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// start DMAIN Virture channel
/// @param u16Handle                                    \b IN: Service handle ID
/// @param u32Pacing                                     \b IN: Sgdma in pacing rate
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_DMAinVC_Start(MS_U16 u16Handle, MS_U32 u32Pacing);

//-------------------------------------------------------------------------------------------------
/// Set Locdecrypt key to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Pointer to store loc decrypt key address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_SetLocdecKey(MS_U16 u16Handle, TSIO_LOCKEY_SET *eSet);

//-------------------------------------------------------------------------------------------------
/// Enable/disable locdec key
/// @param u16Handle                                    \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable locdec key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_LocdecKeyEnable(MS_U16 u16Handle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO service
/// @param u8Sid                                            \b IN: Service ID
/// @param u16Handle                                    \b OUT: pointer of service handle for return
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_Alloc(MS_U8 u8Sid, MS_U16 *u16Handle);

//-------------------------------------------------------------------------------------------------
/// Free TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_Free(MS_U16 u16Handle);

//-------------------------------------------------------------------------------------------------
/// open input pid filter for TSIO service
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param eTSOInSrc                                    \b IN: TSIO PID filter source
/// @param u16Pid                                          \b IN: PID value of TSO PID filter
/// @param pu16DmxId                                    \b OUT: Pointer to store PID filter ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_PidOpen(MS_U16 u16Handle, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId);

//-------------------------------------------------------------------------------------------------
/// close input pid filter for TSIO service
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param u16DmxId                                     \b IN: TSO PID filter source
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSIO_Service_PidClose(MS_U16 u16Handle, MS_U16 u16DmxId);


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_H__
