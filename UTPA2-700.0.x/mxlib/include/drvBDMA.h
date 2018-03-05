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
////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_BDMA BDMA interface
    \ingroup  G_PERIPHERAL
   
    \brief 
 
    BDMA is Byte-aligned data transfer DMA engine. It can execute data transmission between MIU to MIU, 
    flash to MIU, SRAM to MIU?�etc.

    <b>Features</b>

    - Byte-aligned data transfer DMA engine
    - Address increase or decrease while DMA
    - Two command channels (optional). Note that when 2 command channels exist, they are processed as FIFO (First trigger, first service)
    - Several data width for device (1/2/4/8/16 bytes)
    - Bundled source device: MIU/Flash
    - Bundled destination device: MIU/VDMCU/DSP/TSP/HK51 1K SRAM

    <b> Address decreasing mode:</b> \n
    Avoid data overlapped on the same storage. Ex: Copy data A to B.
    \image html drvBDMA_pic.png

    <b> BDMA Block Diagram: </b> \n
    \image html drvBDMA_pic2.png

    <b> Operation Code Flow: </b> \n
    -# Prepare BDMA setting for each operation
    -# Set and start BDMA in command handle
    -# Get BDMA free channel
    -# Set setting
    -# Trigger BDMA
    -# BDMA done by polling
    \image html drvBDMA_pic3.png


    \defgroup G_BDMA_INIT Initialization Task relative
    \ingroup  G_BDMA
    \defgroup G_BDMA_COMMON Common Task relative
    \ingroup  G_BDMA
    \defgroup G_BDMA_PS Pattern search relative
    \ingroup  G_BDMA
    \defgroup G_BDMA_MOBF  MOBF relative
    \ingroup  G_BDMA
    \defgroup G_BDMA_ToBeModified BDMA api to be modified
    \ingroup  G_BDMA
    \defgroup G_BDMA_ToBeRemove BDMA api to be removed
    \ingroup  G_BDMA
*/

#ifndef _DRVBDMA_H_
#define _DRVBDMA_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @author MStar Semiconductor Inc.
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"
#include "UFO.h"

#define BDMA_UTOPIA20       (1)

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_BDMA_LIB_CODE	{'B','D','M','A'}    //Lib code
#define MSIF_BDMA_LIBVER		{'0','3'}            //LIB version
#define MSIF_BDMA_BUILDNUM      {'0','1'}            //Build Number
#define MSIF_BDMA_CHANGELIST   {'0','0','3','4','8','0','3','3'} //P4 ChangeList Number

#define BDMA_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_BDMA_LIB_CODE,                  /* IP__                                             */  \
    MSIF_BDMA_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_BDMA_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_BDMA_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//v: value n: shift n bits
#define _LShift(v, n)           ((v) << (n))
#define _RShift(v, n)           ((v) >> (n))

#define BDMA_SEARCH_ALL_MATCHED (0)
#define BDMA_CRC32_POLY         (0x04C11DB7)
#define BDMA_CRC16_POLY         (0x8005)
#define BDMA_CRC_SEED_0         (0)
#define BDMA_CRC_SEED_F         (0xFFFFFFFF)

/// Operation cfg
#define BDMA_OPCFG_DEF          		(0)
#define BDMA_OPCFG_INV_COPY     	(0x01)
#define BDMA_OPCFG_CRC_REFLECT  	(0x02)      //bit reflection of each input byte
#define BDMA_OPCFG_CRC_COPY     	(0x04)      //copy then crc check
#define BDMA_OPCFG_NOWAIT_COPY  	(0x08)      //copy then quit
#define BDMA_OPCFG_MOBF_PS  		(0x10)      //copy then quit

typedef enum _BDMA_DbgLv
{
    E_BDMA_DBGLV_NONE           //no debug message
    ,E_BDMA_DBGLV_PERFORMANCE   //show performance only
    ,E_BDMA_DBGLV_ERR_ONLY      //show error only
    ,E_BDMA_DBGLV_REG_DUMP      //show error & reg dump
    ,E_BDMA_DBGLV_INFO          //show error & informaiton
    ,E_BDMA_DBGLV_ALL           //show error, information & funciton name
}BDMA_DbgLv;

typedef enum _BDMA_Dev
{
    E_BDMA_DEV_MIU0
    ,E_BDMA_DEV_MIU1
    ,E_BDMA_DEV_SEARCH
    ,E_BDMA_DEV_CRC32
    ,E_BDMA_DEV_MEM_FILL
    ,E_BDMA_DEV_FLASH
    ,E_BDMA_DEV_DMDMCU
    ,E_BDMA_DEV_VDMCU
    ,E_BDMA_DEV_DSP
    ,E_BDMA_DEV_TSP
    ,E_BDMA_DEV_1KSRAM_HK51
    ,E_BDMA_DEV_MIU2
    ,E_BDMA_DEV_MIU3
    ,E_BDMA_DEV_NOT_SUPPORT
}BDMA_Dev;

typedef enum _BDMA_SrcDev
{
    E_BDMA_SRCDEV_MIU0          = E_BDMA_DEV_MIU0
    ,E_BDMA_SRCDEV_MIU1         = E_BDMA_DEV_MIU1
    ,E_BDMA_SRCDEV_MEM_FILL     = E_BDMA_DEV_MEM_FILL
    ,E_BDMA_SRCDEV_FLASH        = E_BDMA_DEV_FLASH
    ,E_BDMA_SRCDEV_MIU2         = E_BDMA_DEV_MIU2
    ,E_BDMA_SRCDEV_MIU3         = E_BDMA_DEV_MIU3
    ,E_BDMA_SRCDEV_NOT_SUPPORT  = E_BDMA_DEV_NOT_SUPPORT
}BDMA_SrcDev;

typedef enum _BDMA_DstDev
{
    E_BDMA_DSTDEV_MIU0          = E_BDMA_DEV_MIU0
    ,E_BDMA_DSTDEV_MIU1         = E_BDMA_DEV_MIU1
    ,E_BDMA_DSTDEV_SEARCH       = E_BDMA_DEV_SEARCH
    ,E_BDMA_DSTDEV_CRC32        = E_BDMA_DEV_CRC32
    ,E_BDMA_DSTDEV_DMDMCU       = E_BDMA_DEV_DMDMCU         //Demod
    ,E_BDMA_DSTDEV_VDMCU        = E_BDMA_DEV_VDMCU          //VD
    ,E_BDMA_DSTDEV_DSP          = E_BDMA_DEV_DSP
    ,E_BDMA_DSTDEV_TSP          = E_BDMA_DEV_TSP
    ,E_BDMA_DSTDEV_HK51_1KSRAM  = E_BDMA_DEV_1KSRAM_HK51
    ,E_BDMA_DSTDEV_MIU2         = E_BDMA_DEV_MIU2
    ,E_BDMA_DSTDEV_MIU3         = E_BDMA_DEV_MIU3
    ,E_BDMA_DSTDEV_NOT_SUPPORT  = E_BDMA_DEV_NOT_SUPPORT
}BDMA_DstDev;

#define BDMA_SET_CPYTYPE(src, dst) ((src & 0x0F) | _LShift((dst &0x0F), 8))

typedef enum _BDMA_CpyType
{
    E_BDMA_SDRAM2SDRAM          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_MIU0)
    ,E_BDMA_SDRAM2SDRAM1        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_MIU1)
    ,E_BDMA_SDRAM2SDRAM2        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_MIU2)
    ,E_BDMA_SDRAM2SDRAM3        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_MIU3)
    ,E_BDMA_SDRAM2DMDMCU        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_DMDMCU)
    ,E_BDMA_SDRAM2VDMCU         = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_VDMCU)
    ,E_BDMA_SDRAM2DSP           = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_DSP)
    ,E_BDMA_SDRAM2TSP           = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_TSP)
    ,E_BDMA_SDRAM2SRAM1K_HK51   = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU0, E_BDMA_DEV_1KSRAM_HK51)
    ,E_BDMA_SDRAM12SDRAM        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_MIU0)
    ,E_BDMA_SDRAM12SDRAM1       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_MIU1)
    ,E_BDMA_SDRAM12SDRAM2       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_MIU2)
    ,E_BDMA_SDRAM12SDRAM3       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_MIU3)
    ,E_BDMA_SDRAM12DMDMCU       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_DMDMCU)
    ,E_BDMA_SDRAM12VDMCU        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_VDMCU)
    ,E_BDMA_SDRAM12DSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_DSP)
    ,E_BDMA_SDRAM12TSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_TSP)
    ,E_BDMA_SDRAM12SRAM1K_HK51  = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU1, E_BDMA_DEV_1KSRAM_HK51)
    ,E_BDMA_SDRAM22SDRAM        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_MIU0)
    ,E_BDMA_SDRAM22SDRAM1       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_MIU1)
    ,E_BDMA_SDRAM22SDRAM2       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_MIU2)
    ,E_BDMA_SDRAM22SDRAM3       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_MIU3)
    ,E_BDMA_SDRAM22DMDMCU       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_DMDMCU)
    ,E_BDMA_SDRAM22VDMCU        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_VDMCU)
    ,E_BDMA_SDRAM22DSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_DSP)
    ,E_BDMA_SDRAM22TSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_TSP)
    ,E_BDMA_SDRAM22SRAM1K_HK51  = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU2, E_BDMA_DEV_1KSRAM_HK51)
    ,E_BDMA_SDRAM32SDRAM        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_MIU0)
    ,E_BDMA_SDRAM32SDRAM1       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_MIU1)
    ,E_BDMA_SDRAM32SDRAM2       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_MIU2)
    ,E_BDMA_SDRAM32SDRAM3       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_MIU3)
    ,E_BDMA_SDRAM32DMDMCU       = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_DMDMCU)
    ,E_BDMA_SDRAM32VDMCU        = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_VDMCU)
    ,E_BDMA_SDRAM32DSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_DSP)
    ,E_BDMA_SDRAM32TSP          = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_TSP)
    ,E_BDMA_SDRAM32SRAM1K_HK51  = BDMA_SET_CPYTYPE(E_BDMA_DEV_MIU3, E_BDMA_DEV_1KSRAM_HK51)
    ,E_BDMA_FLASH2SDRAM         = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_MIU0)
    ,E_BDMA_FLASH2SDRAM1        = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_MIU1)
    ,E_BDMA_FLASH2DMDMCU        = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_DMDMCU)
    ,E_BDMA_FLASH2VDMCU         = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_VDMCU)
    ,E_BDMA_FLASH2DSP           = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_DSP)
    ,E_BDMA_FLASH2TSP           = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_TSP)
    ,E_BDMA_FLASH2SRAMHK51      = BDMA_SET_CPYTYPE(E_BDMA_DEV_FLASH, E_BDMA_DEV_1KSRAM_HK51)
    ,E_BDMA_CPYTYPE_MAX
}BDMA_CpyType;

typedef enum _BDMA_Result
{
    E_BDMA_NOT_SUPPORT = -1
    ,E_BDMA_FAIL = 0
    ,E_BDMA_OK = 1
    ,E_BDMA_TIMEOUT
    ,E_BDMA_QUEUE_FULL
    ,E_BDMA_BUSY
}BDMA_Result;

typedef struct _BDMA_HwInfo
{
    MS_BOOL bEnMIU1;        //MIU1
    MS_BOOL bEnHost;        //bdma host
    MS_BOOL bEnMemFill;     //memory fill
    MS_BOOL bEnFlsCpy;      //flash copy
    MS_BOOL bEnDevDw;       //bdma device data width
    MS_BOOL bEnDmyWrCnt;    //bdma dummy wr count
    MS_BOOL bEnDMDMCU;      //bdma to DeMod MCU
    MS_BOOL bEnTSP;         //bdma to TSP
    MS_BOOL bEnDSP;         //bdma to DSP
    MS_BOOL bEnHK51_1KSRAM; //bdma to HK51_1KSRAM
}BDMA_HwInfo;

typedef struct _BDMA_Info
{
    MS_U8       u8ChNum;
    MS_U16      u16ChipVer;
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
    MS_U32      u32IOMap;
    MS_U32      u32MIU1Base;
#else
    MS_VIRT     u32IOMap;
    MS_PHY      phy64MIU1Base;
#endif
    MS_S32      s32Mutex;
    MS_BOOL     bInit;
    BDMA_DbgLv  eDbgLv;
    BDMA_HwInfo sHwCap;
}BDMA_Info;

typedef struct _BDMA_ChStatus
{
    MS_BOOL bIsBusy;
    MS_BOOL bIsInt;
    MS_BOOL bIsFound;
}BDMA_ChStatus;

typedef struct DLL_PACKED _BDMA_Status
{
    MS_BOOL         bInit;
    BDMA_DbgLv      eDbgLv;
    BDMA_ChStatus   sChSta[2];
}BDMA_Status;

#ifdef MOBF_ENABLE

typedef struct DLL_PACKED _BDMA_MOBF_PS
{
    MS_U32 u32Pattern;
    MS_U32 u32ExcluBit;
    MS_U32 u32MobfKey;
}BDMA_MOBF_PS;

#endif

typedef void (*BDMA_ISR_CBF)(BDMA_Result eRet);
////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#include "drvBDMA_v2.h"


// status check & dbg level
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_BDMA_GetStatus(BDMA_Status *pStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const BDMA_Info* MDrv_BDMA_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_BDMA_GetMinSize(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_SetDbgLevel(BDMA_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
BDMA_Result MDrv_BDMA_Init(MS_U32 u32Miu1Base);
#else
BDMA_Result MDrv_BDMA_Init(MS_PHY phy64Miu1Base);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_U32 MDrv_BDMA_Search(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev);
#else
MS_U32 MDrv_BDMA_Search(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev);
#endif
#ifdef MOBF_ENABLE
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_PS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_U32 MDrv_BDMA_MOBFSearch(MS_PHYADDR u32Addr, MS_U32 u32Len, BDMA_MOBF_PS *pMobfPsCfg, BDMA_SrcDev eDev);
#else
MS_PHY MDrv_BDMA_MOBFSearch(MS_PHY phy64Addr, MS_U32 u32Len, BDMA_MOBF_PS *pMobfPsCfg, BDMA_SrcDev eDev);
#endif
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_MOBF
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
MS_U32 MDrv_BDMA_CRC32(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect);
#else
MS_U32 MDrv_BDMA_CRC32(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
BDMA_Result MDrv_BDMA_PatternFill(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev);
#else
BDMA_Result MDrv_BDMA_PatternFill(MS_PHY u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_PS
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
BDMA_Result MDrv_BDMA_MemCopy(MS_PHYADDR u32SrcAddr, MS_PHYADDR u32DstAddr, MS_U32 u32Len);
#else
BDMA_Result MDrv_BDMA_MemCopy(MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
BDMA_Result MDrv_BDMA_FlashCopy2Dram(MS_PHYADDR u32FlashAddr, MS_PHYADDR u32DramAddr, MS_U32 u32Len);
#else
BDMA_Result MDrv_BDMA_FlashCopy2Dram(MS_PHY phy64FlashAddr, MS_PHY phy64DramAddr, MS_U32 u32Len);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
BDMA_Result MDrv_BDMA_CopyHnd(MS_PHYADDR u32SrcAddr, MS_PHYADDR u32DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg);
#else
BDMA_Result MDrv_BDMA_CopyHnd(MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_BDMA_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_WaitFlashDone(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_ToBeModified
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_BDMA_SetSPIOffsetForMCU(void);

// status
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_BDMA_DumpCB(void *pvOpCB);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_ToBeModified
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_Stop_All(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_ToBeModified
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_Stop(MS_U8 u8Ch);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_BDMA_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
BDMA_Result MDrv_BDMA_Exit(void);


#ifdef __cplusplus
}
#endif
#endif
