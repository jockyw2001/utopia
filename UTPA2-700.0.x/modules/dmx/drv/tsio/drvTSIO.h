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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSO.h
/// @brief  TS I/O Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSO DDI are not allowed to use in any interrupt context other than TSO ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TSIO_H_
#define _DRV_TSIO_H_



#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Transport stream null PID
#define MSIF_TSIO_LIB_CODE           {'T','S','I','O'}                                           // Lib code
#define MSIF_TSIO_LIBVER             {'0','1'}                                                   // LIB version
#define MSIF_TSIO_BUILDNUM           {'0','1'}                                                   // Build Number
#define MSIF_TSIO_CHANGELIST         {'0','0','0','0','0','0','0','0'}                           // P4 ChangeList Number

#define TSIO_DRV_VERSION             /* Character String for DRV/API version             */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* 0x0000                                           */  \
    MSIF_MOD,                       /* 0x0000                                           */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_TSIO_LIB_CODE,              /* IP__                                             */  \
    MSIF_TSIO_LIBVER,                /* 0.0 ~ Z.Z                                        */  \
    MSIF_TSIO_BUILDNUM,              /* 00 ~ 99                                          */  \
    MSIF_TSIO_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS


#define TSIO_CC_CMD_MIN_LEN                             2
#define TSIO_CC_CMD_MAX_LEN                             258
#define TSIO_CC_CMD_TIMEOUT                             200 //200 ms
#define TSIO_CC_CMD_ERR_STATUS_MASK                     0x0770

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DRVTSIO_FAIL                      = 0,
    DRVTSIO_OK                        = 1,
    DRVTSIO_INVALID_PARAM             = 2,
    DRVTSIO_FUNC_ERROR                = 3,
}TSIO_Result;

typedef enum
{
    E_DRVTSIO_DBG_Release = 0,
    E_DRVTSIO_DBG_ERR,
    E_DRVTSIO_DBG_INFO,
    E_DRVTSIO_DBG_TRACE,
}DRVTSIO_DBGLEVEL;

typedef enum
{
    E_DRVTSIO_SERVICEDEST_LIVE = 0,
    E_DRVTSIO_SERVICEDEST_DMA,
}DRVTSIO_SERVICEDEST;

typedef enum
{
    DRVTSIO_INFO_LOSELOCKCNT = 0,
    DRVTSIO_INFO_USEBUFLEN = 1,
    DRVTSIO_INFO_MAX = 0xFFFF,
}DRVTSIO_INFO;

typedef enum
{
    DRVTSIO_CLK_200M = 0,
    DRVTSIO_CLK_100M = 1,
    DRVTSIO_CLK_50M = 2,
    DRVTSIO_CLK_25M = 3,
}DRVTSIO_CLK;

typedef enum
{
    DRVTSIO_CAP_RESOURCE_SIZE          = 0,                     // Get the data structure size of private resource (share resource)
    DRVTSIO_CAP_CAP_NULL,
} DRVTSIO_CAP;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bTSmode;
    MS_BOOL bMidPriority;
    MS_BOOL bLastNodeFlush;
    MS_BOOL bNewPkt;
}DRVTSIO_DMAIN_SET;

typedef struct
{
    MS_PHY phyBufAddr;
    MS_U32 u32BufSize;
    MS_BOOL bSecurity;
}DRVTSIO_DMAOUT_SET;

typedef struct
{
    MS_U32 u32Endnodeflag; //[0]: last node, [1]: node int
    MS_U32 u32Buffersize;
    MS_U32 u32Bufferaddr;
    MS_U32 u32Nextnodeaddr;
}DRVTSIO_SGDMA_NODE;

typedef enum
{
    E_TSIO_INT_NONE = 0,
    E_TSIO_SGDMAOUT_LASTNODE = 1,
    E_TSIO_SGDMAOUT_NODEINT = 2,
    E_TSIO_SGDMAOUT_DROP = 3,
    E_TSIO_SGDMAOUT_DMAEND = 4,
    E_TSIO_SGDMAIN_DONE,
    E_TSIO_CC_TX_DONE,
    E_TSIO_CC_RX_DONE,
    E_TSIO_LIVE_TSO_OVERFLOW,
    E_TSIO_OPERATION_START,
}E_TSIO_INT_TYPE;

typedef struct
{
    MS_U32 u32Event;
    MS_U64 u64DmainInt;
    MS_U64 u64DmaoutInt[4];
}DRVTSIO_IOSIGNAL;


typedef void(*TSIOIntCB)(E_TSIO_INT_TYPE eType, MS_U32 u32Info, void *ext1, void *ext2);

//-------------------------------------------------------------------------------------------------
/// Get TSIO IO signal
/// @param stIosig                                   \b OUT: TSIO IO signal
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Wait_IoSignal(DRVTSIO_IOSIGNAL *stIosig);

//-------------------------------------------------------------------------------------------------
/// Register interrupt callback function
/// @param TSIOIntCB                                  \b IN: Callback function pointer
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_RegisterIntCb(TSIOIntCB efn);

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_InitLibResource(void* pResMemAddr);

//-------------------------------------------------------------------------------------------------
/// Get TSIO Hardware capability
/// @param  eCap                                 \b IN: Capability item to get
/// @param  pOutput                             \b IN: Pointer to store getting result
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_GetCap(DRVTSIO_CAP eCap, void* pOutput);

//-------------------------------------------------------------------------------------------------
/// Initialize TSIO driver
/// @return TSIO_Result
/// @note
/// It should be called before calling any other TSIO functions.
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Init(void);

//-------------------------------------------------------------------------------------------------
/// Open TSIO
/// @param eClk                         \b IN: clk speed
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Open(DRVTSIO_CLK eClk);

//-------------------------------------------------------------------------------------------------
/// Exit TSIO driver
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Close TSIO
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Close(void);

//-------------------------------------------------------------------------------------------------
/// Search and setting Analog phase
/// @param u8offset                         \b IN: shift phase
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Analogphase_adj_bittraining(MS_U8 u8offset);

//-------------------------------------------------------------------------------------------------
/// Search and setting Analog phase
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Analogphase_adj(void);

//-------------------------------------------------------------------------------------------------
/// Set debug level of TSIO
/// @param  DbgLevel                \b IN: Debug Level Value
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SetDbgLevel(DRVTSIO_DBGLEVEL DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Reset TSIO driver
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Enable bypass SMC setting on TSIO module
/// @param u8Mode                  \b IN: 0: disable, 1: direct8, 2: direct 16, 3: loopback , bypass SMC setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_BypassSMC_Set(MS_U8 u8Mode);

//-------------------------------------------------------------------------------------------------
/// Set Stuffing ServiceID to TSIO module
/// @param u32StuffingSID           \b IN: stuffing service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_StuffingSID_Set(MS_U32 u32StuffingSID);

//-------------------------------------------------------------------------------------------------
/// Enable Loc decrypt engine on TSIO module
/// @param bEnable           \b IN: enable/disable loc decrypt engine
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_LocDecrypt_Set(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set Sync threshold value to TSIO module
/// @param u8Value           \b IN: threshold value
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Syncthreshold_Set(MS_U8 u8Value);

//-------------------------------------------------------------------------------------------------
/// Enable Bypass packet merge on TSIO module
/// @param bEnable           \b IN: enable/disable bypass packet merge
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_BypassMerge_Set(MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Reset SGDMAIN
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO SGDMA_in PID filter
/// @param  pu16PidFltId                         \b OUT: pointer of PID filter ID for return
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_Alloc(MS_U16 *pu16PidFltId);

//-------------------------------------------------------------------------------------------------
/// Free TSIO SGDMA_in PID filter
/// @param u16PidFltId                           \b IN: PID filter ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_Free(MS_U16 u16PidFltId);

//-------------------------------------------------------------------------------------------------
/// Set PID to SGDMA_in PID filter
/// @param  u16PidFltId                       \b IN: PID filter ID
/// @param  u16Pid                             \b IN: PID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_SetPid(MS_U16 u16PidFltId, MS_U16 u16Pid);

//-------------------------------------------------------------------------------------------------
/// Set Service ID to PID filter
/// @param  u16PidFltId                         \b IN: PID filter ID
/// @param  u8Sid                                \b IN: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_SetSid(MS_U16 u16PidFltId, MS_U8 u8Sid);

//=======================================================
// C&C setting API
//=======================================================

//-------------------------------------------------------------------------------------------------
/// Send C&C cmd
/// @param pu8Cmd                                        \b IN: Pointer to store C&C cmd address
/// @param u16CmdLen                                   \b IN: C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_SendCmd(MS_U8 *pu8Cmd, MS_U16 u16CmdLen);

//-------------------------------------------------------------------------------------------------
/// Send C&C cmd in speed-up mode
/// @param pu8Cmd                                        \b IN: Pointer to store C&C cmd address
/// @param u16CmdLen                                   \b IN: C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_SendCmd_Fast(MS_U8 *pu8Cmd, MS_U16 u16CmdLen);

//-------------------------------------------------------------------------------------------------
/// Receive C&C cmd
/// @param pu8Cmd                                        \b OUT: Pointer to store C&C cmd address
/// @param pu16CmdLen                                  \b OUT: Pointer to store C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveCmd(MS_U8 *pu8Cmd, MS_U16 *pu16CmdLen);

//-------------------------------------------------------------------------------------------------
/// Receive C&C cmd in speed-up mode
/// @param pu8Cmd                                        \b OUT: Pointer to store C&C cmd address
/// @param pu16CmdLen                                  \b OUT: Pointer to store C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveCmd_Fast(MS_U8 *pu8Cmd, MS_U16 *pu16CmdLen);

//-------------------------------------------------------------------------------------------------
/// Receive interupt status
/// @param u32Timeout                                  \b IN: Timeout value
/// @param pu16Status                                  \b OUT: Pointer to store C&C receiver status address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveIntStatus(MS_U32 u32Timeout, MS_U16 *pu16Status);

//=======================================================
// Physic buffer setting API
//=======================================================

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN Last packet buffer
/// @param u32LastpktBufAddr                          \b IN: Last packet buffer physic address
/// @param u32LastpktBufSize                           \b IN: Last packet buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_LastPktBuf_Set(MS_U32 u32LastpktBufAddr, MS_U32 u32LastpktBufSize);

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN VQ buffer
/// @param u32VQBufAddr                          \b IN: SGDMAIN VQ buffer physic address
/// @param u32VQBufSize                           \b IN: SGDMAIN VQ buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_SGVQBuf_Set(MS_U32 u32VQBufAddr, MS_U32 u32VQBufSize);

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN Node buffer
/// @param u32NodeAddr                          \b IN: SGDMAIN Node buffer physic address
/// @param u32NodeSize                           \b IN: SGDMAIN Node buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_NodeBuf_Set(MS_U32 u32NodeAddr, MS_U32 u32NodeSize);

//-------------------------------------------------------------------------------------------------
/// Set SGDMAOUT Node buffer
/// @param u32NodeAddr                          \b IN: SGDMAOUT Node buffer physic address
/// @param u32NodeSize                           \b IN: SGDMAOUT Node buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAOUT_NodeBuf_Set(MS_U32 u32NodeAddr, MS_U32 u32NodeSize);

//=======================================================
// Service relative setting API
//=======================================================

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO service
/// @param  pu16Servicehandle                         \b OUT: pointer of service handle for return
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_Alloc(MS_U16 *pu16Servicehandle);

//-------------------------------------------------------------------------------------------------
/// Free TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_Free(MS_U16 u16Servicehandle);

//-------------------------------------------------------------------------------------------------
/// Set Service ID to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u8Sid                                            \b IN: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetSid(MS_U16 u16Servicehandle, MS_U8 u8Sid);

//-------------------------------------------------------------------------------------------------
/// Get Service ID with target Service handle
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u8Sid                                            \b OUT: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_GetSid(MS_U16 u16Servicehandle, MS_U8 *u8Sid);

//-------------------------------------------------------------------------------------------------
/// Set DMAIN Virture channel to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma in param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDMAinVC(MS_U16 u16Servicehandle, DRVTSIO_DMAIN_SET *peSet);

//-------------------------------------------------------------------------------------------------
/// start DMAIN Virture channel
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u32Pacing                                     \b IN: Sgdma in pacing rate, unit: bps
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_DMAinVC_Start(MS_U16 u16Servicehandle, MS_U32 u32Pacing);

//-------------------------------------------------------------------------------------------------
/// Set DMAOUT VC to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDMAoutVC(MS_U16 u16Servicehandle, DRVTSIO_DMAOUT_SET* peSet);

//-------------------------------------------------------------------------------------------------
/// Get DMAOUT VC  write address
/// @param u16Servicehandle                          \b IN: Service handle ID
/// @param pphyWrite                                    \b OUT: dma out write address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_DMAout_WriteGet(MS_U16 u16Servicehandle, MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set destination to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDestination(MS_U16 u16Servicehandle, DRVTSIO_SERVICEDEST eDest);

//-------------------------------------------------------------------------------------------------
/// Enable bypass SMC merge setting to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                  \b IN: enable/disable bypass SMC setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_BypassSMCMergeEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Set Locdecrypt key to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param pKey                                            \b IN: Pointer to store loc decrypt key address
/// @param u16KeySize                                   \b IN: Decrypt key size
/// @param bOddKey                                       \b IN: 1: odd key, 0: even key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecKey(MS_U16 u16Servicehandle, MS_U8 *pKey, MS_U16 u16KeySize, MS_BOOL bOddKey);

//-------------------------------------------------------------------------------------------------
/// Enable/disable locdec key
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable locdec key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_LocdecKeyEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable secure keyslot on Locdecrypt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable / disable secure keyslot
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecSecureKeyslot(MS_U16 u16Servicehandle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable privilege keyslot on Locdecrypt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable / disable secure flag
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecPrivilegeKeyslot(MS_U16 u16Servicehandle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Enable SGDMAIN PidFlt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SgdmaInPidFltEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// Get TSIO informaiton
/// @param eInfo                                        \b IN: Info type
/// @param u32Arg                                      \b IN: detail value
/// @param u32Retinfo                                 \b OUT: Pointer to store Info address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_GetInfo(DRVTSIO_INFO eInfo, MS_U32 u32Arg, MS_U32 *u32Retinfo);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_TSIO_H_


