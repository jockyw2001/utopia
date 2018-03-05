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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD_ISDBT.h
/// @brief  ISDBT Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Demod Demod module

 *! \defgroup ISDBT ISDBT interface (drvDMD_ISDBT.h)
 *  \ingroup Demod
    
 *! \defgroup ISDBT_BASIC ISDBT basic control
 *  \ingroup ISDBT
     
 *! \defgroup ISDBT_INFO ISDBT information
 *  \ingroup ISDBT
    
 *! \defgroup ISDBT_STR ISDBT STR
 *  \ingroup ISDBT
      
 *! \defgroup ISDBT_ToBeRemoved ISDBT API to be removed
 *  \ingroup ISDBT
 */

#ifndef _DRV_ISDBT_H_
#define _DRV_ISDBT_H_

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#define DMD_ISDBT_UTOPIA_EN                  1
#define DMD_ISDBT_UTOPIA2_EN                 0

#define DMD_ISDBT_STR_EN                     1
#define DMD_ISDBT_MULTI_THREAD_SAFE          0
#define DMD_ISDBT_MULTI_DMD_EN               1

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsTypes.h"
#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#include "MsCommon.h"
#endif
#if DMD_ISDBT_UTOPIA2_EN || DMD_ISDBT_STR_EN
#include "utopia.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef DLL_PUBLIC 
#define DLL_PUBLIC 
#endif

#if DMD_ISDBT_MULTI_DMD_EN
#define DMD_ISDBT_MAX_DEMOD_NUM          2
#else
#define DMD_ISDBT_MAX_DEMOD_NUM          1
#endif

#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#define MSIF_DMD_ISDBT_LIB_CODE          {'D','M','D','_','I','S','D','B','T','_'} //Lib code
#define MSIF_DMD_ISDBT_LIBVER            {'0','0'}                                 //LIB version
#define MSIF_DMD_ISDBT_BUILDNUM          {'0','0'}                                 //Build Number
#define MSIF_DMD_ISDBT_CHANGELIST        {'0','0','0','0','0','0','0','0'}         //P4 ChangeList Number

#define DMD_ISDBT_VER                    /* Character String for DRV/API version             */  \
    MSIF_TAG,                            /* 'MSIF'                                           */  \
    MSIF_CLASS,                          /* '00'                                             */  \
    MSIF_CUS,                            /* 0x0000                                           */  \
    MSIF_MOD,                            /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                   \
    MSIF_CPU,                                                                                    \
    MSIF_DMD_ISDBT_LIB_CODE,             /* IP__                                             */  \
    MSIF_DMD_ISDBT_LIBVER,               /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_ISDBT_BUILDNUM,             /* 00 ~ 99                                          */  \
    MSIF_DMD_ISDBT_CHANGELIST,           /* CL#                                              */  \
    MSIF_OS
#endif // #if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN

#ifndef BIT_
#define BIT_(n)                             (1 << (n))
#endif

#define DMD_ISDBT_LOCK_FSA_TRACK_LOCK       BIT_(0)
#define DMD_ISDBT_LOCK_PSYNC_LOCK           BIT_(1)
#define DMD_ISDBT_LOCK_ICFO_CH_EXIST_LOCK   BIT_(2)
#define DMD_ISDBT_LOCK_FEC_LOCK             BIT_(3)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    DMD_ISDBT_HAL_CMD_Exit = 0,
    DMD_ISDBT_HAL_CMD_InitClk,
    DMD_ISDBT_HAL_CMD_Download,
    DMD_ISDBT_HAL_CMD_FWVERSION,
    DMD_ISDBT_HAL_CMD_SoftReset,
    DMD_ISDBT_HAL_CMD_SetACICoef,
    DMD_ISDBT_HAL_CMD_SetISDBTMode,
    DMD_ISDBT_HAL_CMD_SetModeClean,
    DMD_ISDBT_HAL_CMD_Active,
    DMD_ISDBT_HAL_CMD_Check_FEC_Lock,
    DMD_ISDBT_HAL_CMD_Check_FSA_TRACK_Lock,
    DMD_ISDBT_HAL_CMD_Check_PSYNC_Lock,
    DMD_ISDBT_HAL_CMD_Check_ICFO_CH_EXIST_Lock,
    DMD_ISDBT_HAL_CMD_GetSignalCodeRate,
    DMD_ISDBT_HAL_CMD_GetSignalGuardInterval,
    DMD_ISDBT_HAL_CMD_GetSignalTimeInterleaving,
    DMD_ISDBT_HAL_CMD_GetSignalFFTValue,
    DMD_ISDBT_HAL_CMD_GetSignalModulation,
    DMD_ISDBT_HAL_CMD_ReadIFAGC,
    DMD_ISDBT_HAL_CMD_GetFreqOffset,
    DMD_ISDBT_HAL_CMD_GetSignalQuality,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerA,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerB,
    DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerC,
    DMD_ISDBT_HAL_CMD_GetSignalQualityCombine,
    DMD_ISDBT_HAL_CMD_GetSNR,
    DMD_ISDBT_HAL_CMD_GetPreViterbiBer,
    DMD_ISDBT_HAL_CMD_GetPostViterbiBer,
    DMD_ISDBT_HAL_CMD_Read_PKT_ERR,
    DMD_ISDBT_HAL_CMD_TS_INTERFACE_CONFIG,
    DMD_ISDBT_HAL_CMD_IIC_Bypass_Mode,
    DMD_ISDBT_HAL_CMD_SSPI_TO_GPIO,
    DMD_ISDBT_HAL_CMD_GPIO_GET_LEVEL,
    DMD_ISDBT_HAL_CMD_GPIO_SET_LEVEL,
    DMD_ISDBT_HAL_CMD_GPIO_OUT_ENABLE,
    DMD_ISDBT_HAL_CMD_GET_REG,
    DMD_ISDBT_HAL_CMD_SET_REG
} DMD_ISDBT_HAL_COMMAND;

typedef enum
{
    E_ISDBT_Layer_A = 0x00,
    E_ISDBT_Layer_B = 0x01,
    E_ISDBT_Layer_C = 0x02,
    E_ISDBT_Layer_INVALID,
} EN_ISDBT_Layer;

typedef enum
{      
    E_ISDBT_FFT_2K = 0x00,  /// 2K     
    E_ISDBT_FFT_4K = 0x01,  /// 4k      
    E_ISDBT_FFT_8K = 0x02,  /// 8k     
    E_ISDBT_FFT_INVALID,    /// invalid indicator
} EN_ISDBT_FFT_VAL;

typedef enum
{
    E_ISDBT_DQPSK   = 0,   /// DQPSK  
    E_ISDBT_QPSK    = 1,   /// QPSK
    E_ISDBT_16QAM   = 2,   /// 16QAM
    E_ISDBT_64QAM   = 3,   /// 64QAM
    E_ISDBT_QAM_INVALID,   /// invalid indicator
} EN_ISDBT_CONSTEL_TYPE;

typedef enum
{
    E_ISDBT_CODERATE_1_2 = 0,   /// 1/2
    E_ISDBT_CODERATE_2_3 = 1,   /// 2/3
    E_ISDBT_CODERATE_3_4 = 2,   /// 3/4
    E_ISDBT_CODERATE_5_6 = 3,   /// 5/6
    E_ISDBT_CODERATE_7_8 = 4,   /// 7/8
    E_ISDBT_CODERATE_INVALID,   /// invalid indicator
} EN_ISDBT_CODE_RATE;

typedef enum
{
    E_ISDBT_GUARD_INTERVAL_1_4  = 0,   /// 1/4
    E_ISDBT_GUARD_INTERVAL_1_8  = 1,   /// 1/8
    E_ISDBT_GUARD_INTERVAL_1_16 = 2,   /// 1/16
    E_ISDBT_GUARD_INTERVAL_1_32 = 3,   /// 1/32
    E_ISDBT_GUARD_INTERVAL_INVALID,    /// invalid indicator
} EN_ISDBT_GUARD_INTERVAL;

typedef enum
{ 
    // 2K mode
    E_ISDBT_2K_TDI_0 = 0,   /// Tdi = 0
    E_ISDBT_2K_TDI_4 = 1,   /// Tdi = 4
    E_ISDBT_2K_TDI_8 = 2,   /// Tdi = 8
    E_ISDBT_2K_TDI_16 = 3,  /// Tdi = 16
    // 4K mode
    E_ISDBT_4K_TDI_0 = 4,   /// Tdi = 0
    E_ISDBT_4K_TDI_2 = 5,   /// Tdi = 2
    E_ISDBT_4K_TDI_4 = 6,   /// Tdi = 4
    E_ISDBT_4K_TDI_8 = 7,   /// Tdi = 8
    // 8K mode   
    E_ISDBT_8K_TDI_0 = 8,   /// Tdi = 0
    E_ISDBT_8K_TDI_1 = 9,   /// Tdi = 1
    E_ISDBT_8K_TDI_2 = 10,  /// Tdi = 2
    E_ISDBT_8K_TDI_4 = 11,  /// Tdi = 4
    E_ISDBT_TDI_INVALID,    /// invalid indicator
} EN_ISDBT_TIME_INTERLEAVING;

typedef struct _ISDBT_MODULATION_MODE
{
    EN_ISDBT_CODE_RATE         eIsdbtCodeRate;
    EN_ISDBT_GUARD_INTERVAL    eIsdbtGI;
    EN_ISDBT_TIME_INTERLEAVING eIsdbtTDI;
    EN_ISDBT_FFT_VAL           eIsdbtFFT;
    EN_ISDBT_CONSTEL_TYPE      eIsdbtConstellation;
} sISDBT_MODULATION_MODE;

typedef struct _DMD_ISDBT_GET_MODULATION
{
    EN_ISDBT_Layer        eIsdbtLayer;
    EN_ISDBT_CONSTEL_TYPE eConstellation;
} DMD_ISDBT_GET_MODULATION;

typedef struct _DMD_ISDBT_GET_CodeRate
{
    EN_ISDBT_Layer     eIsdbtLayer;
    EN_ISDBT_CODE_RATE eCodeRate;
} DMD_ISDBT_GET_CodeRate;

typedef struct _DMD_ISDBT_GET_TimeInterleaving
{
    EN_ISDBT_Layer             eIsdbtLayer;
    EN_ISDBT_TIME_INTERLEAVING eTimeInterleaving;
} DMD_ISDBT_GET_TimeInterleaving;

typedef struct _DMD_ISDBT_GET_BER_VALUE
{
    EN_ISDBT_Layer eIsdbtLayer;
    float          fBerValue;
} DMD_ISDBT_GET_BER_VALUE;

typedef struct _DMD_ISDBT_GET_PKT_ERR
{
    EN_ISDBT_Layer eIsdbtLayer;
    MS_U16         u16PacketErr;
} DMD_ISDBT_GET_PKT_ERR;

typedef struct
{
    MS_U8 u8Pin;
    union
    {
        MS_BOOL bLevel;
        MS_BOOL bIsOut;
    };
} DMD_ISDBT_GPIO_PIN_DATA;

typedef struct
{
    MS_U16 u16Addr;
    MS_U8  u8Data;
} DMD_ISDBT_REG_DATA;

typedef enum
{
    DMD_ISDBT_DBGLV_NONE,    // disable all the debug message
    DMD_ISDBT_DBGLV_INFO,    // information
    DMD_ISDBT_DBGLV_NOTICE,  // normal but significant condition
    DMD_ISDBT_DBGLV_WARNING, // warning conditions
    DMD_ISDBT_DBGLV_ERR,     // error conditions
    DMD_ISDBT_DBGLV_CRIT,    // critical conditions
    DMD_ISDBT_DBGLV_ALERT,   // action must be taken immediately
    DMD_ISDBT_DBGLV_EMERG,   // system is unusable
    DMD_ISDBT_DBGLV_DEBUG,   // debug-level messages
} DMD_ISDBT_DbgLv;

typedef enum
{   DMD_ISDBT_DEMOD,
    DMD_ISDBT_DEMOD_6M = DMD_ISDBT_DEMOD,
    DMD_ISDBT_DEMOD_7M,
    DMD_ISDBT_DEMOD_8M,
    DMD_ISDBT_DEMOD_MAX,
    DMD_ISDBT_DEMOD_NULL = DMD_ISDBT_DEMOD_MAX,
} DMD_ISDBT_DEMOD_TYPE;

typedef enum
{
    DMD_ISDBT_GETLOCK,
    DMD_ISDBT_GETLOCK_FSA_TRACK_LOCK,
    DMD_ISDBT_GETLOCK_PSYNC_LOCK,
    DMD_ISDBT_GETLOCK_ICFO_CH_EXIST_LOCK,
    DMD_ISDBT_GETLOCK_FEC_LOCK
} DMD_ISDBT_GETLOCK_TYPE;

typedef enum
{
    DMD_ISDBT_LOCK,
    DMD_ISDBT_CHECKING,
    DMD_ISDBT_CHECKEND,
    DMD_ISDBT_UNLOCK,
    DMD_ISDBT_NULL,
} DMD_ISDBT_LOCK_STATUS;

/// For demod init
typedef struct
{
    // init
    MS_U16 u16ISDBTIcfoChExistCheckTime;
    MS_U16 u16ISDBTFECLockCheckTime;

    // register init
    MS_U8 *u8DMD_ISDBT_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_ISDBT_DSPRegInitSize;
    MS_U8 *u8DMD_ISDBT_InitExt; // TODO use system variable type

    //By Tuners:
    MS_U16  u16IF_KHZ;//By Tuners
    MS_BOOL bIQSwap;//0
    MS_U16  u16AgcReferenceValue;//0
    MS_BOOL bTunerGainInvert;//0

    //By IC:
    MS_BOOL bIsExtDemod;//0

    //By TS (Only for MCP or ext demod):
    MS_U8 u1TsConfigByte_SerialMode : 1;
    MS_U8 u1TsConfigByte_DataSwap   : 1;
    MS_U8 u1TsConfigByte_ClockInv   : 1;
    MS_U8 u5TsConfigByte_DivNum     : 5;

    //By SYS I2C (Only for MCP or ext demod):
    MS_U8 u8I2CSlaveAddr;
    MS_U8 u8I2CSlaveBus;
    MS_BOOL (*I2C_WriteBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
    MS_BOOL (*I2C_ReadBytes)(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data); 
    
    //By SYS MSPI (Only for MCP or ext demod):
    MS_BOOL bIsUseSspiLoadCode;
    MS_BOOL bIsSspiUseTsPin;
    
    //By SYS memory mapping (Only for int demod):
    MS_U32 u32TdiStartAddr;
    
    #if !DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
    MS_U32  (*GetSystemTimeMS)(void);      // Get sys time (unit: ms)
    void    (*DelayMS)(MS_U32 ms);         // Delay time (unit: ms)
    MS_BOOL (*CreateMutex)(MS_BOOL enable); // Create&Delete mutex
    void    (*LockDMD)(MS_BOOL enable);    // Enter&Leave mutex
    #endif
} DMD_ISDBT_InitData;

typedef struct
{
    MS_BOOL bInit;
    MS_BOOL bDownloaded;
    
    #if DMD_ISDBT_STR_EN
    MS_BOOL              bIsDTV;
    EN_POWER_MODE        eLastState;
    #endif
    DMD_ISDBT_DEMOD_TYPE eLastType;
    
    MS_BOOL (*HAL_DMD_ISDBT_IOCTL_CMD)(DMD_ISDBT_HAL_COMMAND eCmd, void *pPara);
} DMD_ISDBT_PriData;

typedef struct
{
    MS_U8  u8Version;
    MS_U32 u32ISDBTScanTimeStart;
    MS_U32 u32ISDBTFECLockTime;
    MS_U32 u32ISDBTLockStatus;
} DMD_ISDBT_Info;

typedef struct
{
    DMD_ISDBT_InitData  sDMD_ISDBT_InitData;
    DMD_ISDBT_PriData   sDMD_ISDBT_PriData;
    DMD_ISDBT_Info      sDMD_ISDBT_Info;
} DMD_ISDBT_ResData;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
/// Set detailed level of ISDBT driver debug message
/// @ingroup ISDBT_BASIC
/// @param u8DbgLevel    \b IN: debug level for Parallel Flash driver 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetDbgLevel(DMD_ISDBT_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of ISDBT driver 
/// @ingroup ISDBT_INFO
/// @return : the pointer to the driver information
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern DMD_ISDBT_Info* MDrv_DMD_ISDBT_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// Get ISDBT driver version
/// @ingroup ISDBT_INFO
/// @param ppVersion    \b OUT: the pointer to the driver version
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// Initialize HAL interface
/// @ingroup ISDBT_BASIC
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Initial_Hal_Interface(void);  

////////////////////////////////////////////////////////////////////////////////
///                            SINGLE DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/// Initialize ISDBT Demod
/// @ingroup ISDBT_BASIC
/// @param pDMD_ISDBT_InitData    \b IN: ISDBT initial parameters 
/// @param u32InitDataLen        \b IN: size of ISDBT initial parameters
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Init(DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen);
//-------------------------------------------------------------------------------------------------
/// Exit ISDBT DTV mode
/// @ingroup ISDBT_STR
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Exit(void);
//-------------------------------------------------------------------------------------------------
/// Get FW version
/// @ingroup ISDBT_ToBeRemoved
/// @param pu16FwVer    \b OUT: the pointer to FW version
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetFwVer(MS_U16 *pu16FwVer);
//-------------------------------------------------------------------------------------------------
/// Reset FW state machine
/// @ingroup ISDBT_ToBeRemoved
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern void MDrv_DMD_ISDBT_SetReset(void);
//-------------------------------------------------------------------------------------------------
/// Set Demod mode and restart Demod
/// @ingroup ISDBT_BASIC
/// @param eType    \b IN: Select ISDBT modulation type
/// @param bEnable  \b IN: Enable SetConfig function
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_AdvSetConfig(DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set Demod mode and restart Demod
/// @ingroup ISDBT_BASIC
/// @param bEnable  \b IN: Enable SetConfig function
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetConfig(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Active Demod
/// @ingroup ISDBT_ToBeRemoved
/// @param bEnable    \b IN: Active Demod if TRUE
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetActive(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Support ISDBT STR function
/// @ingroup ISDBT_STR
/// @param u16PowerState    \b IN: Set STR status
/// @return : STR status
//-------------------------------------------------------------------------------------------------
#if DMD_ISDBT_STR_EN
DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_SetPowerState(EN_POWER_MODE u16PowerState);
#endif
//-------------------------------------------------------------------------------------------------
/// Get lock status
/// @ingroup ISDBT_INFO
/// @param eType    \b IN: select lock type
/// @return : lock status
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_GetLock(DMD_ISDBT_GETLOCK_TYPE eType);
//-------------------------------------------------------------------------------------------------
/// Get modulation mode
/// @ingroup ISDBT_INFO
/// @param eLayerIndex    \b IN: layer index
/// @param sIsdbtModulationMode    \b OUT: the pointer to ISDBT modulation mode
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetModulationMode(EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode);
//-------------------------------------------------------------------------------------------------
/// Get Demod signal strength (IF AGC gain)
/// @ingroup ISDBT_INFO
/// @param u16Strength    \b OUT: the pointer to signal strength
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetSignalStrength(MS_U16 *u16Strength);
//-------------------------------------------------------------------------------------------------
/// Get Demod frequency offset
/// @ingroup ISDBT_INFO
/// @param pFreqOff    \b OUT: the pointer to CFO
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetFreqOffset(float *pFreqOff);
//-------------------------------------------------------------------------------------------------
/// Get Demod signal quality
/// @ingroup ISDBT_INFO
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQuality(void);
//-------------------------------------------------------------------------------------------------
/// Get Demod layerA signal quality
/// @ingroup ISDBT_INFO
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerA(void);
//-------------------------------------------------------------------------------------------------
/// Get Demod layerB signal quality
/// @ingroup ISDBT_INFO
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerB(void);
//-------------------------------------------------------------------------------------------------
/// Get Demod layerC signal quality
/// @ingroup ISDBT_INFO
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityOfLayerC(void);
//-------------------------------------------------------------------------------------------------
/// Get Demod Combine signal quality
/// @ingroup ISDBT_INFO
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_GetSignalQualityCombine(void);
//-------------------------------------------------------------------------------------------------
/// Get SNR
/// @ingroup ISDBT_INFO
/// @param pf_snr    \b OUT: the pointer to SNR
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetSNR(float *pf_snr);
//-------------------------------------------------------------------------------------------------
/// Get Demod pre Viterbi number
/// @ingroup ISDBT_INFO
/// @param eLayerIndex    \b IN: layer index
/// @param fber    \b OUT: the pointer to pre Viterbi BER 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber);
//-------------------------------------------------------------------------------------------------
/// Get Demod post Viterbi number
/// @ingroup ISDBT_INFO
/// @param eLayerIndex    \b IN: layer index
/// @param fber    \b OUT: the pointer to post Viterbi BER 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, float *fber);
//-------------------------------------------------------------------------------------------------
/// Get Demod packet error number
/// @ingroup ISDBT_INFO
/// @param eLayerIndex    \b IN: layer index
/// @param u16PacketErr    \b OUT: the pointer to packet error number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_Read_PKT_ERR(EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr);
//-------------------------------------------------------------------------------------------------
/// Set TS output mode
/// @ingroup ISDBT_BASIC
/// @param u8TsConfigData    \b IN: TS configuration
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetSerialControl(MS_U8 u8TsConfigData);
//-------------------------------------------------------------------------------------------------
/// Enable I2C bypass mode
/// @ingroup ISDBT_BASIC
/// @param bEnable    \b IN: Enable bypass mode
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_IIC_BYPASS_MODE(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set SSPI pin as GPIO function
/// @ingroup ISDBT_BASIC
/// @param bEnable    \b IN: Switch to GPIO if TRUE, otherwise SSPI 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SWITCH_SSPI_GPIO(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Get GPIO level
/// @ingroup ISDBT_BASIC
/// @param u8Pin    \b IN: Select pin number 
/// @param bLevel    \b OUT: the pointer to GPIO level
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_GET_LEVEL(MS_U8 u8Pin, MS_BOOL *bLevel);
//-------------------------------------------------------------------------------------------------
/// Set GPIO level
/// @ingroup ISDBT_BASIC
/// @param u8Pin    \b IN: Select pin number 
/// @param bLevel    \b IN: Set GPIO level
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_SET_LEVEL(MS_U8 u8Pin, MS_BOOL bLevel);
//-------------------------------------------------------------------------------------------------
/// Set GPIO as output or input
/// @ingroup ISDBT_BASIC
/// @param u8Pin    \b IN: Select pin number 
/// @param bEnableOut    \b IN: output if TRUE, otherwise input
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GPIO_OUT_ENABLE(MS_U8 u8Pin, MS_BOOL bEnableOut);
//-------------------------------------------------------------------------------------------------
/// Get Demod register value
/// @ingroup ISDBT_BASIC
/// @param u16Addr    \b IN: register address 
/// @param pu8Data    \b OUT: the pointer to register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// Set Demod register value
/// @ingroup ISDBT_BASIC
/// @param u16Addr    \b IN: register address 
/// @param u8Data    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data);

////////////////////////////////////////////////////////////////////////////////
///                            MULTI DEMOD API                              ///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
/// Initialize multiple ISDBT Demod
/// @ingroup ISDBT_BASIC
/// @param id                    \b IN: Select Demod number
/// @param pDMD_ISDBT_InitData    \b IN: ISDBT initial parameters 
/// @param u32InitDataLen        \b IN: size of ISDBT initial parameters
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Init(MS_U8 id, DMD_ISDBT_InitData *pDMD_ISDBT_InitData, MS_U32 u32InitDataLen);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of exiting ISDBT DTV mode 
/// @ingroup ISDBT_STR
/// @param id    \b IN: Select Demod number
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Exit(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting FW version
/// @ingroup ISDBT_ToBeRemoved
/// @param id    \b IN: Select Demod number
/// @param pu16FwVer    \b OUT: the pointer to FW version
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetFwVer(MS_U8 id, MS_U16 *pu16FwVer);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of resetting FW state machine
/// @ingroup ISDBT_ToBeRemoved
/// @param id    \b IN: Select Demod number
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern void MDrv_DMD_ISDBT_MD_SetReset(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting and restarting Demod
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param eType    \b IN: Select ISDBT modulation type
/// @param bEnable  \b IN: Enable SetConfig function
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_AdvSetConfig(MS_U8 id, DMD_ISDBT_DEMOD_TYPE eType, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting and restarting Demod
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param bEnable  \b IN: Enable SetConfig function
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetConfig(MS_U8 id, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Active Demod
/// @ingroup ISDBT_ToBeRemoved
/// @param id    \b IN: Select Demod number
/// @param bEnable    \b IN: Active Demod if TRUE
/// @return TRUE : succeed 
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetActive(MS_U8 id, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of ISDBT STR function
/// @ingroup ISDBT_STR
/// @param id    \b IN: Select Demod number
/// @param u16PowerState    \b IN: Set STR status
/// @return : STR status
//-------------------------------------------------------------------------------------------------
#if DMD_ISDBT_STR_EN
// DLL_PUBLIC extern MS_U32 MDrv_DMD_ISDBT_MD_SetPowerState(MS_U8 id, EN_POWER_MODE u16PowerState);
#endif
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting lock status
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param eType    \b IN: select lock type
/// @return : lock status
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern DMD_ISDBT_LOCK_STATUS MDrv_DMD_ISDBT_MD_GetLock(MS_U8 id, DMD_ISDBT_GETLOCK_TYPE eType);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting modulation mode
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param eLayerIndex    \b IN: layer index
/// @param sIsdbtModulationMode    \b OUT: the pointer to ISDBT modulation mode
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetModulationMode(MS_U8 id, EN_ISDBT_Layer eLayerIndex, sISDBT_MODULATION_MODE *sIsdbtModulationMode);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod signal strength (IF AGC gain)
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param u16Strength    \b OUT: the pointer to signal strength
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetSignalStrength(MS_U8 id, MS_U16 *u16Strength);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod fequency offset
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param pFreqOff    \b OUT: the pointer to CFO
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetFreqOffset(MS_U8 id, float *pFreqOff);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod signal quality
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQuality(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod layerA signal quality
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod layerB signal quality
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod layerC signal quality
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod combine signal quality
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @return : signal quality
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_U16 MDrv_DMD_ISDBT_MD_GetSignalQualityCombine(MS_U8 id);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting SNR
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param pf_snr    \b OUT: the pointer to SNR 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetSNR(MS_U8 id, float *pf_snr);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod pre Viterbi number
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param eLayerIndex    \b IN: layer index
/// @param fber    \b OUT: the pointer to BER 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPreViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod post Viterbi number
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param eLayerIndex    \b IN: layer index
/// @param fber    \b OUT: the pointer to BER 
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetPostViterbiBer(MS_U8 id, EN_ISDBT_Layer eLayerIndex, float *fber);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod packet error number
/// @ingroup ISDBT_INFO
/// @param id    \b IN: Select Demod number
/// @param eLayerIndex    \b IN: layer index
/// @param u16PacketErr    \b OUT: the pointer to packet error number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_Read_PKT_ERR(MS_U8 id, EN_ISDBT_Layer eLayerIndex, MS_U16 *u16PacketErr);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting TS output mode
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u8TsConfigData    \b IN: TS configuration
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetSerialControl(MS_U8 id, MS_U8 u8TsConfigData);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of enabling I2C bypass mode
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param bEnable    \b IN: Enable bypass mode
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE(MS_U8 id, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting SSPI pin as GPIO function
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param bEnable    \b IN: Switch to GPIO if TRUE, otherwise SSPI
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO(MS_U8 id, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting GPIO level
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u8Pin    \b IN: Select pin number 
/// @param bLevel    \b OUT: the pointer to GPIO level
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL *bLevel);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting GPIO level
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u8Pin    \b IN: Select pin number 
/// @param bLevel    \b IN: Set GPIO level
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL(MS_U8 id, MS_U8 u8Pin, MS_BOOL bLevel);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting GPIO as output or input
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u8Pin    \b IN: Select pin number 
/// @param bEnableOut    \b IN: output if TRUE, otherwise input
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE(MS_U8 id, MS_U8 u8Pin, MS_BOOL bEnableOut);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of getting Demod register value
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u16Addr    \b IN: register address 
/// @param pu8Data    \b OUT: the pointer to get register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_GetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 *pu8Data);
//-------------------------------------------------------------------------------------------------
/// Support multiple Demod of setting Demod register value
/// @ingroup ISDBT_BASIC
/// @param id    \b IN: Select Demod number
/// @param u16Addr    \b IN: register address 
/// @param u8Data    \b IN: register data
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// DLL_PUBLIC extern MS_BOOL MDrv_DMD_ISDBT_MD_SetReg(MS_U8 id, MS_U16 u16Addr, MS_U8 u8Data);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ISDBT_H_
