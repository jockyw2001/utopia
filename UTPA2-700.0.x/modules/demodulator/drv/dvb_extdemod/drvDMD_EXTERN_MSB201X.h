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
/// @file   drvDMD_EXTERN_MSB201X.h
/// @brief  MSB201X Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBC_H_
#define _DRV_DVBC_H_

#include "MsCommon.h"
//#include "drvDMD_common.h"
#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define SUPPORT_MULTI_DEMOD      1  //0

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_DMD_MSB201X_INTERN_LIB_CODE           {'1','2', '4','x'} //Lib code
#define MSIF_DMD_MSB201X_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_MSB201X_INTERN_BUILDNUM           {'0','0' }    //Build Number
#define MSIF_DMD_MSB201X_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define DEMOD_MAX_INSTANCE 2
#define DEMOD_MAX_CHANNEL 2

#define DMD_MSB201X_EXTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_MSB201X_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_MSB201X_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_MSB201X_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_MSB201X_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_DMD_MSB201X_DBGLV_NONE,    // disable all the debug message
    E_DMD_MSB201X_DBGLV_INFO,    // information
    E_DMD_MSB201X_DBGLV_NOTICE,  // normal but significant condition
    E_DMD_MSB201X_DBGLV_WARNING, // warning conditions
    E_DMD_MSB201X_DBGLV_ERR,     // error conditions
    E_DMD_MSB201X_DBGLV_CRIT,    // critical conditions
    E_DMD_MSB201X_DBGLV_ALERT,   // action must be taken immediately
    E_DMD_MSB201X_DBGLV_EMERG,   // system is unusable
    E_DMD_MSB201X_DBGLV_DEBUG,   // debug-level messages
} eDMD_MSB201X_DbgLv;

typedef enum
{
    E_DMD_DVBC_PARAM_VERSION,           //0x00
    E_DMD_DVBC_OP_AUTO_IQ,
    E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE,
    E_DMD_DVBC_OP_AUTO_SCAN_QAM,
    E_DMD_DVBC_OP_ZIF_EN,                        
    E_DMD_DVBC_OP_LIF_EN,                        
    E_DMD_DVBC_IF_INV_PWM_OUT_EN,     
    E_DMD_DVBC_CFG_FIF_L,                   
    E_DMD_DVBC_CFG_FIF_H, 
    E_DMD_DVBC_CFG_BW0_L,
    E_DMD_DVBC_CFG_BW0_H,
    E_DMD_DVBC_CFG_QAM,
    E_DMD_DVBC_CFG_IQ_SWAP,	
    E_DMD_DVBC_CFG_CCI,
    E_DMD_DVBC_AGC_REF_L,
    E_DMD_DVBC_AGC_REF_H,   
    E_DMD_DVBC_NO_SIGNAL_NUM_TH,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_L,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_H,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_L,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_H,
    E_DMD_DVBC_CHIP_VERSION,	//0x10
}  eDMD_MSB201X_Param;

typedef enum
{
    DMD_DVBC_QAM16 = 0,
    DMD_DVBC_QAM32 = 1,
    DMD_DVBC_QAM64 = 2,
    DMD_DVBC_QAM128 = 3,
    DMD_DVBC_QAM256 = 4,
    DMD_DVBC_QAMAUTO = 128,
} eDMD_MSB201X_MODULATION_TYPE;

typedef enum
{
    DMD_DVBC_GETLOCK,
    DMD_DVBC_GETLOCK_FEC_LOCK,
    DMD_DVBC_GETLOCK_PSYNC_LOCK,
    DMD_DVBC_GETLOCK_TPS_LOCK,
    DMD_DVBC_GETLOCK_DCR_LOCK,
    DMD_DVBC_GETLOCK_AGC_LOCK,
    DMD_DVBC_GETLOCK_MODE_DET,
    DMD_DVBC_GETLOCK_NO_CHANNEL,
    DMD_DVBC_GETLOCK_ATV_DETECT,
    DMD_DVBC_GETLOCK_TR_LOCK,
    DMD_DVBC_GETLOCK_TR_EVER_LOCK,
} eDMD_MSB201X_GETLOCK_TYPE;

typedef enum
{
    DMD_DVBC_LOCK,
    DMD_DVBC_CHECKING,
    DMD_DVBC_CHECKEND,
    DMD_DVBC_UNLOCK,
    DMD_DVBC_NULL,
} eDMD_MSB201X_LOCK_STATUS;

typedef enum
{
    E_DMD_MSB201X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1,
    E_DMD_MSB201X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2,
    E_DMD_MSB201X_DEMOD_I2C_DYNAMIC_SLAVE_ID_3,
    E_DMD_MSB201X_DEMOD_I2C_DYNAMIC_SLAVE_ID_4
} eDMD_MSB201X_DemodI2CSlaveID;

typedef enum
{
    E_DMD_MSB201X_DEMOD_I2C_READ_BYTES,
    E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES
} eDMD_MSB201X_DemodI2CMethod;


typedef enum
{
    DEMOD0,
    DEMOD1,
    ALL_DEMOD
} eDMD_MSB201X_Demod_Index;

typedef struct
{
    float   power_db;
    MS_U8   agc_val;
}sDMD_MSB201X_IFAGC_SSI;

typedef struct
{
    float   attn_db;
    MS_U8   agc_err;
}sDMD_MSB201X_IFAGC_ERR;

typedef struct
{
    MS_U16 u16Version;
    MS_U16 u16SymbolRate;
    eDMD_MSB201X_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp; //	
    MS_U32 u32FcFs; //	
    MS_U8 u8Qam; //
    MS_U16 u16SymbolRateHal; //		
} sDMD_MSB201X_Info;

typedef struct
{
    sDMD_MSB201X_IFAGC_SSI *pIFAGC_SSI;
    sDMD_MSB201X_IFAGC_ERR *pIFAGC_ERR;
    MS_U16 IFAGC_SSI_Length;
    MS_U16 IFAGC_ERR_Length;
} sDMD_MSB201X_IFAGC;

/// For demod init
typedef struct
{
    MS_U8 u8WO_SPI_Flash;
    MS_BOOL bPreloadDSPCodeFromMainCHIPI2C;
    MS_BOOL bFlashWPEnable;
    void (*fpGPIOReset)(MS_BOOL bOnOff);
    MS_BOOL (*fpMSB201X_I2C_Access)(MS_U8 u8DevID, eDMD_MSB201X_DemodI2CMethod eMethod, MS_U8 u8AddrSize, MS_U8 *pu8Addr, MS_U16 u16Size, MS_U8 *pu8Data);
    MS_U8* pDVBC_DSP_REG;
    MS_U8 DVBC_DSP_REG_Length;
    sDMD_MSB201X_IFAGC IFAGC_Data[DEMOD_MAX_CHANNEL];
    MS_BOOL bEnableSPILoadCode;
    void (*fpMSB201X_SPIPAD_En)(MS_BOOL bOnOff);
    MS_U8 u8WO_Sdram;// 1 means no sdram on board
} sDMD_MSB201X_InitData;


typedef enum
{
    E_DMD_MSB201X_DEMOD_NONE,
    E_DMD_MSB201X_DEMOD_DVBC,
} eDMD_MSB201X_DemodulatorType;


//TS parameter area
typedef enum
{
    E_DMD_MSB201X_PARALLEL,
    E_DMD_MSB201X_3_WIRE_REMUX2TS0,
    E_DMD_MSB201X_3_WIRE_REMUX2TS1,
    E_DMD_MSB201X_3_WIRE_REMUX2TS2,
    E_DMD_MSB201X_3_WIRE_REMUX2TS3,
    E_DMD_MSB201X_3_WIRE_REMUX2TS4,
    E_DMD_MSB201X_3_WIRE_REMUX2TS5,
    E_DMD_MSB201X_3_WIRE_REMUX2TS6,
    E_DMD_MSB201X_3_WIRE_REMUX2TS7,
    E_DMD_MSB201X_3_WIRE_DMD0_TS0_DMD1_TS1,
    E_DMD_MSB201X_3_WIRE_DMD0_TS1_DMD1_TS0,
    E_DMD_MSB201X_4_WIRE_REMUX2TS0,
    E_DMD_MSB201X_4_WIRE_REMUX2TS1,
    E_DMD_MSB201X_4_WIRE_REMUX2TS2,
    E_DMD_MSB201X_4_WIRE_REMUX2TS3,
    E_DMD_MSB201X_4_WIRE_REMUX2TS4,
    E_DMD_MSB201X_4_WIRE_REMUX2TS5,
    E_DMD_MSB201X_4_WIRE_DMD0_TS0_DMD1_TS1,
    E_DMD_MSB201X_4_WIRE_DMD0_TS1_DMD1_TS0,
    E_DMD_MSB201X_TS_MODE_MAX
}eDMD_MSB201X_TS_MODE;

typedef enum
{
    E_DMD_MSB201X_TS_CLK_AUTO   ,
    E_DMD_MSB201X_TS_CLK_216MHz ,
    E_DMD_MSB201X_TS_CLK_108MHz ,
    E_DMD_MSB201X_TS_CLK_72MHz  ,
    E_DMD_MSB201X_TS_CLK_54MHz  ,
    E_DMD_MSB201X_TS_CLK_43p2MHz,
    E_DMD_MSB201X_TS_CLK_36MHz  ,
    E_DMD_MSB201X_TS_CLK_30p8MHz,
    E_DMD_MSB201X_TS_CLK_27MHz  ,
    E_DMD_MSB201X_TS_CLK_24MHz  ,
    E_DMD_MSB201X_TS_CLK_21p6MHz,
    E_DMD_MSB201X_TS_CLK_19p6MHz,
    E_DMD_MSB201X_TS_CLK_18MHz  ,
    E_DMD_MSB201X_TS_CLK_16p6MHz,
		E_DMD_MSB201X_TS_CLK_15p4MHz,
		E_DMD_MSB201X_TS_CLK_14p4MHz,
		E_DMD_MSB201X_TS_CLK_13p5MHz,
		E_DMD_MSB201X_TS_CLK_MAX    ,
} eDMD_MSB201X_TS_CLK;   
                                
typedef enum                   
{
		E_DMD_MSB201X_TS_DRVING_LO,    //4mA 
		E_DMD_MSB201X_TS_DRVING_HI,    //8mA 
    E_DMD_MSB201X_TS_DRVING_MAX    //8mA
} eDMD_MSB201X_TS_DRVING;

typedef enum
{
    E_MSB201X_I2C_READ_WRITE,
    E_MSB201X_SPI_READ_WRITE,
} e_MSB201X_DSP_ReadWrite;

typedef struct
{
  MS_BOOL  bEnable[DEMOD_MAX_CHANNEL];
  eDMD_MSB201X_TS_MODE eTSMode;
  eDMD_MSB201X_TS_CLK eTSClk;
  eDMD_MSB201X_TS_DRVING eTSDrv;
  MS_BOOL bLSB1st[DEMOD_MAX_CHANNEL];
  MS_BOOL bRemuxLSB1st;
  MS_BOOL bCLKInverse;
} sDMD_MSB201X_TS_Param;

typedef struct
{
    MS_U8 *pHeaderPtr;
    MS_U8 u8HeaderSize;
    MS_BOOL bEnable;
} sDMD_MSB201X_extHeader;

typedef struct
{
    MS_U8 *pHeaderPtr;
    MS_BOOL bEnable;
} sDMD_MSB201X_CIHeader;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// Set detailed level of DVBT driver debug message
/// u8DbgLevel : debug level for Parallel Flash driver\n
/// AVD_DBGLV_NONE,    ///< disable all the debug message\n
/// AVD_DBGLV_INFO,    ///< information\n
/// AVD_DBGLV_NOTICE,  ///< normal but significant condition\n
/// AVD_DBGLV_WARNING, ///< warning conditions\n
/// AVD_DBGLV_ERR,     ///< error conditions\n
/// AVD_DBGLV_CRIT,    ///< critical conditions\n
/// AVD_DBGLV_ALERT,   ///< action must be taken immediately\n
/// AVD_DBGLV_EMERG,   ///< system is unusable\n
/// AVD_DBGLV_DEBUG,   ///< debug-level messages\n
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
//extern MS_BOOL MDrv_DMD_MSB201X_SetDbgLevel(eDMD_MSB201X_DbgLv u8DbgLevel);
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBT driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
//extern DMD_DVBT_Info* MDrv_DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE eInfoType);
//-------------------------------------------------------------------------------------------------
/// Get DVBT driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
//extern MS_BOOL MDrv_DMD_MSB201X_GetLibVer(const MSIF_Version **ppVersion);
////////////////////////////////////////////////////////////////////////////////
/// Get DVBT FW version
/// u16Addr       : the address of DVBT's register\n
////////////////////////////////////////////////////////////////////////////////
//extern MS_BOOL MDrv_DMD_MSB201X_GetFWVer(MS_U16 *ver);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_GetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_SetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 u8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_SetRegs(MS_U8 devID, MS_U32 u32Addr, MS_U8* u8pData, MS_U16 data_size);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_SetReg2Bytes(MS_U8 devID, MS_U32 u32Addr, MS_U16 u16Data);
////////////////////////////////////////////////////////////////////////////////
/// To get DVBT's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_GetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data);
////////////////////////////////////////////////////////////////////////////////
/// To set DVBT's register value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// u8Value        : the value to be set\n
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_SetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data);

/// MDrv_DMD_MSB201X_LoadDSPCode
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_LoadDSPCode(MS_U8 devID);
////////////////////////////////////////////////////////////////////////////////
/// Get lock
extern MS_BOOL MDrv_DMD_MSB201X_Demod_GetLock(MS_U8 devID, MS_U8 u8DemodIndex);
////////////////////////////////////////////////////////////////////////////////////
/// Demod init
extern MS_BOOL MDrv_DMD_MSB201X_Init(MS_U8 devID, MS_U8 u8DemodIndex, sDMD_MSB201X_InitData *pDMD_MSB201X_InitData, MS_U32 u32InitDataLen);
////////////////////////////////////////////////////////////////////////////////////
/// power on init
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_Power_On_Initialization(MS_U8 devID, MS_U8 u8DemodIndex);
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB201X_GetPacketErr(MS_U8 devID,MS_U8 u8DemodIndex, MS_U16 *pktErr);
extern MS_BOOL MDrv_DMD_MSB201X_Restart(MS_U8 devID, MS_U8 u8DemodIndex);
//extern MS_BOOL MDrv_DMD_MSB201X_CfgExtHeader(MS_U8 devID, MS_U8 u8DemodIndex,  MS_U8 u8HeaderSize, MS_U8* pHeaderPtr, MS_U8 u8HeaderEnable);
extern MS_BOOL MDrv_DMD_MSB201X_Exit(MS_U8 devID);
extern MS_BOOL MDrv_DMD_MSB201X_SetActive(MS_U8 devID, MS_U8 u8DemodIndex, MS_BOOL bEnable);
extern MS_BOOL MDrv_DMD_MSB201X_GetFWVer(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 *ver);
extern MS_BOOL MDrv_DMD_MSB201X_GetPacketErr(MS_U8 devID,MS_U8 u8DemodIndex, MS_U16 *pktErr);
extern MS_BOOL MDrv_DMD_MSB201X_GetPostViterbiBer(MS_U8 devID,MS_U8 u8DemodIndex, float *ber);
extern MS_BOOL MDrv_DMD_MSB201X_GetSNR(MS_U8 devID,MS_U8 u8DemodIndex, float *fSNR);
extern MS_BOOL MDrv_DMD_MSB201X_GetLock(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_GETLOCK_TYPE eType, eDMD_MSB201X_LOCK_STATUS *eLockStatus);
extern MS_BOOL MDrv_DMD_MSB201X_GetLockWithRFPower(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_GETLOCK_TYPE eType, eDMD_MSB201X_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm);
extern MS_BOOL MDrv_DMD_MSB201X_GetRFLevel(MS_U8 devID, MS_U8 u8DemodIndex, float *fRFPowerDbmResult, float fRFPowerDbm);
extern MS_BOOL MDrv_DMD_MSB201X_GetStatus(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff);
extern MS_BOOL MDrv_DMD_MSB201X_I2C_BYPASS(MS_U8 devID,MS_U8 bypass_en);
extern MS_BOOL MDrv_DMD_MSB201X_SetConfig(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16SymbolRate, eDMD_MSB201X_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv);

extern MS_BOOL MDrv_DMD_MSB201X_CfgExtHeader(MS_U8 devID, MS_U8 u8DemodIndex,  sDMD_MSB201X_extHeader *pDMD_MSB201X_extHeader_Param);
extern MS_BOOL MDrv_DMD_MSB201X_Set_TSOut(MS_U8 devID, sDMD_MSB201X_TS_Param *pDMD_MSB201X_TS_Param);
#ifdef __cplusplus
}
#endif


#endif

