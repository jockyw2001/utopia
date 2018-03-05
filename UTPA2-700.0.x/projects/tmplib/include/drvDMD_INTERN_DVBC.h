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
/// @file   drvDMD_INTERN_DVBC.h
/// @brief  DVBC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Demod Demod modules

 *! \defgroup DVBC DVBC interface (drvDMD_INTERN_DVBC.h)
 *  \ingroup Demod
 
    \brief 
    
    DVBC driver is a driver for control the operation of the DVB-C system. Some important 
    information can be shown by calling the relative information function. 

    <b>Features</b>
   - Init
   - Exit
   - Set Dbg Level
   - Get Info
   - Get DSP Reg
   - Set DSP Reg
   - Get Lib Ver
   - Get Reg
   - Get FW Ver
   - Set Reg
   - Set Serial Control
   - Set Config
   - Set Config symbol rate list
   - Set Active
   - Get Lock
   - Get Lock With RF Power
   - Get Signal Strength
   - Get Signal Strength With RFPower
   - Get Signal Quality
   - Get Signal Quality With RFPower
   - Get SNR
   - Get Post Viterbi Ber
   - Get Packet Err
   - Get Cell ID
   - Get Status
   - Active Dmd Switch
   - Dual Individual Init
   - Dual Public _Init
   - Set Power State             
    

    <b> DVBC Block Diagram: </b> \n
    \image html DVBC.png

    <b> Operation Code Flow: </b> \n
    -# Init
    -# SetConfig
    -# Get lock
    -# Get relative information




 *! \defgroup DVBC DVBC interface (drvDMD_INTERN_DVBC.h)
 *  \ingroup Demod
    
 * *! \defgroup DVBC_BASIC DMD Module basic
 *  \ingroup DVBC
     
 *! \defgroup DVBC_To_Be_Removed DVBC to be removed
 *  \ingroup DVBC
    
 *! \defgroup DVBC_To_Be_Modified DVBC to be modified
 *  \ingroup DVBC
    
 *! \defgroup DVBC_REG To set/get register
 *  \ingroup DVBC
    
 *! \defgroup DVBC_CONFIG DVBC config
 *  \ingroup DVBC

 *! \defgroup DVBC_LOCK DVBC get lock
 *  \ingroup DVBC
   
 *! \defgroup DVBC_INFO To get DVBC information
 *  \ingroup DVBC
 
  *! \defgroup DVBC_DUAL DVBC dual demod
 *  \ingroup DVBC
 */

#ifndef _DRV_DVBC_H_
#define _DRV_DVBC_H_

#include "MsTypes.h"

#include "MsCommon.h"
#include "drvDMD_common.h" 
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
#define MSIF_DMD_DVBC_INTERN_LIB_CODE           {'D','V', 'B','C'} //Lib code
#define MSIF_DMD_DVBC_INTERN_LIBVER             {'0','1'}      //LIB version
#define MSIF_DMD_DVBC_INTERN_BUILDNUM           {'2','2' }    //Build Number
#define MSIF_DMD_DVBC_INTERN_CHANGELIST         {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number
// oga    
#define    INDEX_C_TS_PHASE_EN            23
#define    INDEX_C_TS_PHASE_NUM        24

#define DMD_DVBC_INTERN_VER                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMD_DVBC_INTERN_LIB_CODE,      /* IP__                                             */  \
    MSIF_DMD_DVBC_INTERN_LIBVER,        /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMD_DVBC_INTERN_BUILDNUM,      /* 00 ~ 99                                          */  \
    MSIF_DMD_DVBC_INTERN_CHANGELIST,    /* CL#                                              */  \
    MSIF_OS

#define IS_BITS_SET(val, bits)                  (((val)&(bits)) == (bits))

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DMD_DVBC_DBGLV_NONE,    // disable all the debug message
    DMD_DVBC_DBGLV_INFO,    // information
    DMD_DVBC_DBGLV_NOTICE,  // normal but significant condition
    DMD_DVBC_DBGLV_WARNING, // warning conditions
    DMD_DVBC_DBGLV_ERR,     // error conditions
    DMD_DVBC_DBGLV_CRIT,    // critical conditions
    DMD_DVBC_DBGLV_ALERT,   // action must be taken immediately
    DMD_DVBC_DBGLV_EMERG,   // system is unusable
    DMD_DVBC_DBGLV_DEBUG,   // debug-level messages
} DMD_DVBC_DbgLv;

typedef enum
{
    DMD_DVBC_LOCK,
    DMD_DVBC_CHECKING,
    DMD_DVBC_CHECKEND,
    DMD_DVBC_UNLOCK,
    DMD_DVBC_NULL,
} DMD_DVBC_LOCK_STATUS;

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
//    DMD_DVBC_GETLOCK_FEC_STABLE_LOCK,
} DMD_DVBC_GETLOCK_TYPE;

typedef enum
{
    DMD_DVBC_QAM16 = 0,
    DMD_DVBC_QAM32 = 1,
    DMD_DVBC_QAM64 = 2,
    DMD_DVBC_QAM128 = 3,
    DMD_DVBC_QAM256 = 4,
    DMD_DVBC_QAMAUTO = 128,
} DMD_DVBC_MODULATION_TYPE;

typedef enum
{
    DMD_DVBC_RF_CH_BAND_6MHz = 0x01,
    DMD_DVBC_RF_CH_BAND_7MHz = 0x02,
    DMD_DVBC_RF_CH_BAND_8MHz = 0x03,
    DMD_DVBC_RF_CH_BAND_INVALID
} DMD_DVBC_RF_CHANNEL_BANDWIDTH;

typedef enum
{
    E_DMD_DVBC_PARAM_VERSION,           //0x00
    E_DMD_DVBC_OP_RFAGC_EN,
    E_DMD_DVBC_OP_HUMDET_EN,
    E_DMD_DVBC_OP_DCR_EN,
    E_DMD_DVBC_OP_IQB_EN,
    E_DMD_DVBC_OP_AUTO_IQ,
    E_DMD_DVBC_OP_AUTO_RFMAX,
    E_DMD_DVBC_OP_AUTO_ACI,
    E_DMD_DVBC_OP_AUTO_SCAN,
    E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE,
    E_DMD_DVBC_OP_AUTO_SCAN_QAM,
    E_DMD_DVBC_OP_ATV_DETECTOR_EN,
    E_DMD_DVBC_OP_ZIF_EN,                       //ZIF
    E_DMD_DVBC_OP_NORMALIF_EN,            //NIF
    E_DMD_DVBC_OP_LIF_EN,                       //LIF
    E_DMD_DVBC_OP_SAWLESS_EN,             //SAWLESS
    E_DMD_DVBC_IF_INV_PWM_OUT_EN,     //Sony Tuner , 0x10
    E_DMD_DVBC_CFG_RSSI,
    E_DMD_DVBC_CFG_ZIF,
    E_DMD_DVBC_CFG_FS_L,                        //FS
    E_DMD_DVBC_CFG_FS_H,                        //FS
    E_DMD_DVBC_CFG_FIF_L,                       //IF
    E_DMD_DVBC_CFG_FIF_H,                       //IF
    E_DMD_DVBC_CFG_FC_L,                //FC
    E_DMD_DVBC_CFG_FC_H,                //FC
    E_DMD_DVBC_CFG_BW0_L,
    E_DMD_DVBC_CFG_BW0_H,
    E_DMD_DVBC_CFG_BW1_L,
    E_DMD_DVBC_CFG_BW1_H,
    E_DMD_DVBC_CFG_BW2_L,
    E_DMD_DVBC_CFG_BW2_H,
    E_DMD_DVBC_CFG_BW3_L,
    E_DMD_DVBC_CFG_BW3_H,               //0x20
    E_DMD_DVBC_CFG_BW4_L,
    E_DMD_DVBC_CFG_BW4_H,
    E_DMD_DVBC_CFG_BW5_L,
    E_DMD_DVBC_CFG_BW5_H,
    E_DMD_DVBC_CFG_BW6_L,
    E_DMD_DVBC_CFG_BW6_H,
    E_DMD_DVBC_CFG_BW7_L,
    E_DMD_DVBC_CFG_BW7_H,
    E_DMD_DVBC_CFG_BW8_L,
    E_DMD_DVBC_CFG_BW8_H,
    E_DMD_DVBC_CFG_BW9_L,
    E_DMD_DVBC_CFG_BW9_H,
    E_DMD_DVBC_CFG_BW10_L,
    E_DMD_DVBC_CFG_BW10_H,
    E_DMD_DVBC_CFG_BW11_L,
    E_DMD_DVBC_CFG_BW11_H,              //0x30
    E_DMD_DVBC_CFG_RFMAX,
    E_DMD_DVBC_CFG_QAM,
    E_DMD_DVBC_CFG_IQ_SWAP,
    E_DMD_DVBC_CFG_CCI,
    E_DMD_DVBC_CFG_TS_SERIAL,
    E_DMD_DVBC_CFG_TS_CLK_RATE,
    E_DMD_DVBC_CFG_TS_CLK_INV,
    E_DMD_DVBC_CFG_TS_DATA_SWAP,
    E_DMD_DVBC_CFG_TS_EN,
    E_DMD_DVBC_AGC_REF_L,
    E_DMD_DVBC_AGC_REF_H,
    E_DMD_DVBC_AGC_K,
    E_DMD_DVBC_AGC_LOCK_TH,
    E_DMD_DVBC_AGC_LOCK_NUM,
    E_DMD_DVBC_ADC_PGA_GAIN_I,
    E_DMD_DVBC_ADC_PGA_GAIN_Q,                  //0x40
    E_DMD_DVBC_ADC_PWDN_I,
    E_DMD_DVBC_ADC_PWDN_Q,
    E_DMD_DVBC_ADC_MPLL_DIV_SEL,
    E_DMD_DVBC_CCI_BYPASS,
    E_DMD_DVBC_CCI_LOCK_DET,
    E_DMD_DVBC_CCI_FREQN_OUT_0,
    E_DMD_DVBC_CCI_FREQN_OUT_1,
    E_DMD_DVBC_RF_GAIN_MIN,
    E_DMD_DVBC_RF_GAIN_MAX,
    E_DMD_DVBC_IF_GAIN_MIN,
    E_DMD_DVBC_IF_GAIN_MAX,
    E_DMD_DVBC_NO_SIGNAL_NUM_TH,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_L,
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_H,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_L,
    E_DMD_DVBC_NO_SIGNAL_ERR_TH_H,          //0x50
    E_DMD_DVBC_TUNER_NUM,
    E_DMD_DVBC_UNCRT_PKT_NUM_7_0,
    E_DMD_DVBC_UNCRT_PKT_NUM_8_15,
    E_DMD_DVBC_STATE,
    E_DMD_DVBC_ILL_LOCK,
    E_DMD_DVBC_DAGC1_REF,
    E_DMD_DVBC_DAGC2_REF,
    E_DMD_DVBC_EQ_KP3_16QAM,
    E_DMD_DVBC_EQ_KP3_32QAM,
    E_DMD_DVBC_EQ_KP3_64QAM,
    E_DMD_DVBC_EQ_KP3_128QAM,
    E_DMD_DVBC_EQ_KP3_256QAM,
    E_DMD_DVBC_EQ_KP4_16QAM,
    E_DMD_DVBC_EQ_KP4_32QAM,
    E_DMD_DVBC_EQ_KP4_64QAM,
    E_DMD_DVBC_EQ_KP4_128QAM,                   //0x60
    E_DMD_DVBC_EQ_KP4_256QAM,
    E_DMD_DVBC_SNR100_L,
    E_DMD_DVBC_SNR100_H,
    E_DMD_DVBC_CFO10_L,
    E_DMD_DVBC_CFO10_H,
    E_DMD_DVBC_TR_LOCK,
    E_DMD_DVBC_CR_LOCK,
    E_DMD_DVBC_EQ_DMA1_LOCK,
    E_DMD_DVBC_EQ_DMA_LOCK,
    E_DMD_DVBC_EQ_DD1_LOCK,
    E_DMD_DVBC_EQ_DD_LOCK,
    E_DMD_DVBC_FEC_LOCK,                //0x6C
    E_DMD_DVBC_CHIP_VERSION,
} DVBC_Param_2;

typedef enum
{
    // Operation Mode Settings
    e_opmode_rfagc_en, //0
    e_opmode_humdet_en,
    e_opmode_dcr_en,
    e_opmode_iqb_en,
    e_opmode_auto_iq,
    e_opmode_auto_rfmax,
    e_opmode_auto_aci,
    e_opmode_auto_scan,
    e_opmode_auto_scan_sym_rate,
    e_opmode_auto_scan_qam,
    e_opmode_atv_detector_en,
    e_opmode_no_sig_if_gain_th_l,
    e_opmode_no_sig_if_gain_th_h,
    e_opmode_no_sig_if_err_th_l,
    e_opmode_no_sig_if_err_th_h,
    e_opmode_rsv_0x0F, //15
    // Config Params
    e_config_rssi,
    e_config_zif,
    e_config_freq,
    e_config_fc_l,
    e_config_fc_h,//20
    e_config_fs_l,
    e_config_fs_h,
    e_config_bw_l,
    e_config_bw_h,
    e_config_bw1_l,//25
    e_config_bw1_h,
    e_config_bw2_l,
    e_config_bw2_h,
    e_config_bw3_l,
    e_config_bw3_h,//30
    e_config_rsv_0x1F,
    e_config_rfmax,
    e_config_qam,
    e_config_iq_swap,
    e_config_cci,
    e_config_ts_serial,
    e_config_ts_clk_rate,
    e_config_ts_out_inv,
    e_config_ts_data_swap,
    e_config_lif,
    e_config_fif_l,
    e_config_fif_h,
    e_config_tuner_sawless,
    DVBC_PARAM_LEN,
} DVBC_Param;

/// For demod init
typedef struct
{
    // tuner parameter
    MS_U8 u8SarChannel;
    DMD_RFAGC_SSI *pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;

    // register init
    MS_U8 *u8DMD_DVBC_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBC_DSPRegInitSize;
    MS_U8 *u8DMD_DVBC_InitExt; // TODO use system variable type
} DMD_DVBC_InitData;

typedef enum
{
    E_DMD_DVBC_FAIL=0,
    E_DMD_DVBC_OK=1
} DMD_DVBC_Result;


typedef struct
{
    MS_U16 u16Version;
    MS_U16 u16SymbolRate;
    DMD_DVBC_MODULATION_TYPE eQamMode;
    MS_U32 u32IFFreq;
    MS_BOOL bSpecInv;
    MS_BOOL bSerialTS;
    MS_U8 u8SarValue;
    MS_U32 u32ChkScanTimeStart;
    DMD_DVBC_LOCK_STATUS eLockStatus;
    MS_U16 u16Strength;
    MS_U16 u16Quality;
    MS_U32 u32Intp; //    
    MS_U32 u32FcFs; //    
    MS_U8 u8Qam; //
    MS_U16 u16SymbolRateHal; //        
} DMD_DVBC_Info;

//typedef void(*P_DMD_ISR_Proc)(MS_U8 u8DMDID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_Init
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Initialize DVBC 
/// @ingroup DVBC_BASIC   
/// @param  pDMD_DVBC_InitData     \b IN: Initial data
/// @param  u32InitDataLen                \b IN: Initial data length
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);

////////////////////////////////////////////////////////////////////////////////
/// Should be called when exit VD input source
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Exit DVBC 
/// @ingroup DVBC_BASIC   
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Exit(void);

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

//-------------------------------------------------------------------------------------------------
/// Set detailed level of DVBC driver debug message 
/// @ingroup DVBC_BASIC   
/// @param  u8DbgLevel                \b IN: debug level for Parallel Flash driver
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetDbgLevel(DMD_DVBC_DbgLv u8DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Get the information of DVBC driver\n
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Get the information of DVBC driver
/// @ingroup DVBC_INFO  
/// @return : the pointer to the driver information 
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC const DMD_DVBC_Info* MDrv_DMD_DVBC_GetInfo(void);

//-------------------------------------------------------------------------------------------------
/// Get DVBC driver version
/// when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
///To get/set the DSP parameter table from demod MCU
//u16Addr     :the address of the demod MCU DSP parameter table
//////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// Get the DSP parameter table from demod MCU
/// @ingroup DVBC_REG  
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data);

//-------------------------------------------------------------------------------------------------
/// Set the DSP parameter table from demod MCU
/// @ingroup DVBC_REG  
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data);

//-------------------------------------------------------------------------------------------------
/// Get DVBC intern version
/// @ingroup DVBC_INFO  
/// @param  ppVersion                \b IN: Version
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLibVer(const MSIF_Version **ppVersion);

////////////////////////////////////////////////////////////////////////////////
/// To get DVBC's register  value, only for special purpose.\n
/// u16Addr       : the address of DVBT's register\n
/// return the value of AFEC's register\n
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get DVBC's register value, only for special purpose
/// @ingroup DVBC_REG  
/// @param  u16Addr                \b IN: register address
/// @param  pu8Data                \b IN: register value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data);

////////////////////////////////////////////////////////////////////////////////
/// Get DVBC FW version
/// u16Addr       : the address of DVBC's register\n
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get DVBC FW version
/// @ingroup DVBC_INFO  
/// @param  ver                \b IN: FW version
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetFWVer(MS_U16 *ver);

//-------------------------------------------------------------------------------------------------
/// To set DVBC's register value, only for special purpose
/// @ingroup DVBC_REG 
/// @param  u16Addr                \b IN: register address
/// @param  pu8Data                \b IN: register value
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_SetSerialControl
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To set TS serial/parallel mode
/// @ingroup DVBC_CONFIG  
/// @param  bEnable                \b IN: 1=>serial, 0=>parallel
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetSerialControl(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_SetConfig
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To set config data
/// @ingroup DVBC_CONFIG  
/// @param  u16SymbolRate                                    \b IN: set symbol rate
/// @param  eQamMode                                                \b IN: set QAM mode auto/manual
/// @param  u32IFFreq                                            \b IN: set IF frequency
/// @param  bSpecInv                                                \b IN: 1=>spectrum invert, 0=>spectrum normal
/// @param  bSerialTS                                            \b IN: 1=>serial, 0=>parallel
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetConfig(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_SetConfig_symbol_rate_list
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To set config data with symbol rate list
/// @ingroup DVBC_CONFIG  
/// @param  u16SymbolRate                                    \b IN: set symbol rate
/// @param  eQamMode                                                \b IN: set QAM mode auto/manual
/// @param  u32IFFreq                                            \b IN: set IF frequency
/// @param  bSpecInv                                                \b IN: 1=>spectrum invert, 0=>spectrum normal
/// @param  bSerialTS                                            \b IN: 1=>serial, 0=>parallel
/// @param  pu16_symbol_rate_list                 \b IN: symbol rate list
/// @param  u8_symbol_rate_list_num               \b IN: symbol rate list number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetConfig_symbol_rate_list(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_SetActive
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To active DVBC FSM
/// @ingroup DVBC_BASIC  
/// @param  bEnable                \b IN: 1=>Enable
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern  DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_SetActive(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_Get_Lock
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// DVBC get lock flow
/// @ingroup DVBC_LOCK  
/// @param  eType                             \b IN: DVBC get lock
/// @param  eLockStatus                \b IN: DVBC lock status
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetLockWithRFPower
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// DVBC get lock with RF power
/// @ingroup DVBC_LOCK  
/// @param  eType                                             \b IN: DVBC get lock
/// @param  eLockStatus                                \b IN: DVBC lock status
/// @param  fCurrRFPowerDbm                        \b IN: Current RF power in dBm
/// @param  fNoChannelRFPowerDbm               \b IN: No channel RF power in dBm
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetLockWithRFPower(DMD_DVBC_GETLOCK_TYPE eType, DMD_DVBC_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBT_GetSignalStrength
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get signal strength
/// @ingroup DVBC_INFO  
/// @param  u16Strength                                 \b IN: Signal strength
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalStrengthWithRFPower
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get signal strength with RF power
/// @ingroup DVBC_INFO  
/// @param  u16Strength                                 \b IN: Signal strength
/// @param  fRFPowerDbm                                 \b IN: RF power
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalQuality
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get signal quality
/// @ingroup DVBC_INFO  
/// @param  u16Quality                                 \b IN: Signal quality
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSignalQualityWithRFPower
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get signal quality with RF power
/// @ingroup DVBC_INFO  
/// @param  u16Quality                                 \b IN: Signal quality
/// @param  fRFPowerDbm                            \b IN: RF power
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetSNR
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get SNR
/// @ingroup DVBC_INFO  
/// @param  fSNR                                 \b IN: SNR
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetSNR(float *fSNR);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetPostViterbiBer
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get post viterbi BER
/// @ingroup DVBC_INFO  
/// @param  ber                                 \b IN: BER
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPostViterbiBer(float *ber);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetPacketErr
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get packet error
/// @ingroup DVBC_INFO  
/// @param  pktErr                             \b IN: Packet error
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetPacketErr(MS_U16 *pktErr);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetCellID
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get cell ID
/// @ingroup DVBC_INFO  
/// @param  u16CellID                        \b IN: Cell ID
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetCellID(MS_U16 *u16CellID);

////////////////////////////////////////////////////////////////////////////////
/// MDrv_DMD_DVBC_GetStatus
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------
/// To get DVBC status such as QAM type, symbol rate and frequency offset
/// @ingroup DVBC_INFO  
/// @param  pQAMMode                                 \b IN: QAM type
/// @param  u16SymbolRate                    \b IN: Symbol rate
/// @param  pFreqOff                                 \b IN: Frequency offset
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff);

//-------------------------------------------------------------------------------------------------
/// To switch demod No.0 and No.1, update demod information current used
/// For Kaiser only
/// @ingroup DVBC_DUAL  
/// @param  demod_no                                 \b IN: Demod number
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no);

//-------------------------------------------------------------------------------------------------
/// Dual init DVBC demod
/// For Kaiser only
/// @ingroup DVBC_DUAL  
/// @param  pDMD_DVBC_InitData           \b IN: DVBC init data
/// @param  u32InitDataLen                   \b IN: DVBC init data length
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern     DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);

//-------------------------------------------------------------------------------------------------
/// Public dual init DVBC demod
/// For Kaiser only
/// @ingroup DVBC_DUAL  
/// @param  u8AGC_Tristate_Ctrl       \b IN: Tristate control for RFAGC and IFAGC
/// @param  u8Sar_Channel                   \b IN: SAR channel for ADC config
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
extern  DLL_PUBLIC MS_BOOL MDrv_DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel);

//-------------------------------------------------------------------------------------------------
/// To set DVBC power state
/// @ingroup DVBC_BASIC  
/// @param  u16PowerState               \b IN: Power mode
/// @return UTOPIA_STATUS_TRUE : succeed 
/// @return UTOPIA_STATUS_FAIL : fail 
//-------------------------------------------------------------------------------------------------
// extern DLL_PUBLIC MS_U32 MDrv_DMD_DVBC_SetPowerState(EN_POWER_MODE u16PowerState);
#ifdef __cplusplus
}
#endif


#endif // _DRV_DVBC_H_

