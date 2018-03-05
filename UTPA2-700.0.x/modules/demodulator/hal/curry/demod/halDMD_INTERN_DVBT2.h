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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _INTERN_DVBT2_H_
#define _INTERN_DVBT2_H_

#ifdef _INTERN_DVBT2_C_
#define EXTSEL
#else
#define EXTSEL extern
#endif


//--------------------------------------------------------------------

// #define     DEMOD_DYNAMIC_SLAVE_ID_1          0x32
// #define     DEMOD_DYNAMIC_SLAVE_ID_2          0x72
// #define     DEMOD_DYNAMIC_SLAVE_ID_3          0xB2
// #define     DEMOD_DYNAMIC_SLAVE_ID_4          0xF2

#define     DEMOD_ADDR_H            0x00
#define     DEMOD_ADDR_L            0x01
#define     DEMOD_WRITE_REG         0x02
#define     DEMOD_WRITE_REG_EX      0x03
#define     DEMOD_READ_REG          0x04
#define     DEMOD_RAM_CONTROL       0x05

#if  0//DTV_SCAN_AUTO_FINE_TUNE_ENABLE
    //INTERN_DVBT2_ Capture Range fix to 500K
    #define DEMOD_CAPTURE_RANGE_500_K            500
        #define DEMOD_CAPTURE_RANGE_SIZE                                      DEMOD_CAPTURE_RANGE_500_K
#endif

#define MDrv_ReadByte(x)  HAL_DMD_RIU_ReadByte(x)
#define MDrv_WriteByte(x,y)  HAL_DMD_RIU_WriteByte(x,y)

#if 1
#define U8      MAPI_U8
#define U16     MAPI_U16
#define U32     MAPI_U32
#define BOOL    MAPI_BOOL
#define BOOLEAN    MAPI_BOOL
#if 0
#define BIT0     0x01
#define BIT1     0x02
#define BIT2     0x04
#define BIT3     0x08
#define BIT4     0x10
#define BIT5     0x20
#define BIT6     0x40
#define BIT7     0x80
#endif
#define BYTE     MAPI_U8

#define WORD     MAPI_WORD
#define E_RESULT_SUCCESS     MAPI_TRUE
#define E_RESULT_FAILURE     MAPI_FALSE
#define FUNCTION_RESULT      MAPI_BOOL




#define INTERN_DVBT2_TS_SERIAL_INVERSION       0
#define INTERN_DVBT2_TS_PARALLEL_INVERSION     1
#define INTERN_DVBT2_DTV_DRIVING_LEVEL          1
#define INTERN_DVBT2_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  1
#endif


//--------------------------------------------------------------------
typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,
    E_SYS_DVBT2,

    E_SYS_NUM
}E_SYSTEM;

typedef enum
{
    CMD_SYSTEM_INIT = 0,
    CMD_DAC_CALI,
    CMD_DVBT_CONFIG,
    CMD_DVBC_CONFIG,
    CMD_VIF_CTRL,
    CMD_FSM_CTRL,
    CMD_INDIR_RREG,
    CMD_INDIR_WREG,
    CMD_GET_INFO,
    CMD_TS_CTRL,
    CMD_TUNED_VALUE,
    CMD_DVBT2_CONFIG,

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    pc_system = 0,

    SYS_PARAM_MAX_NUM
}E_SYS_PARAM;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;

typedef enum
{
    agc_ref_small,
    agc_ref_large,
    agc_ref_aci,
    ripple_switch_th_l,
    ripple_switch_th_h,

    TUNED_PARAM_MAX_NUM
}E_TUNED_PARAM;

//@@++ Arki 20100125
typedef enum
{
    TS_MODUL_MODE,
    TS_FFX_VALUE,
    TS_GUARD_INTERVAL,
    TS_CODE_RATE,

    TS_PARAM_MAX_NUM
}E_SIGNAL_TYPE;
//@@-- Arki 20100125

typedef struct
{
    MS_U8        cmd_code;
    MS_U8        param[64];
} S_CMDPKTREG;
#if 0
typedef enum
{
    // fw version, check sum
    T2_CHECK_SUM_L      = 0x00,
    E_T2_CHECK_SUM_H,
    E_T2_FW_VER_0,
    E_T2_FW_VER_1,
    E_T2_FW_VER_2,

    // operation mode
    E_T2_ZIF_EN           = 0x20,
    E_T2_RF_AGC_EN,
    E_T2_HUM_DET_EN,
    E_T2_DCR_EN,
    E_T2_IQB_EN,
    E_T2_IIS_EN,
    E_T2_CCI_EN,
    E_T2_LOW_PWR_DET_EN,
    E_T2_ACI_DET_EN,
    E_T2_TD_MOTION_EN,
    E_T2_FD_MOTION_EN,

    // channel tuning param
    E_T2_BW               = 0x40,
    E_T2_FC_L             = 0x41,
    E_T2_FC_H             = 0x42,
    E_T2_FS_L,
    E_T2_FS_H,
    E_T2_ZIF,
    E_T2_GI,
    E_T2_ACI_DET_TYPE,
    E_T2_AGC_REF,         //0x48
    E_T2_RSSI_REF,
    E_T2_SNR_TIME_L,
    E_T2_SNR_TIME_H,
    E_T2_BER_CMP_TIME_L,
    E_T2_BER_CMP_TIME_H,
    E_T2_SFO_CFO_NUM,
    E_T2_CCI,
    E_T2_ACI_DET_TH_L,    //0x50
    E_T2_ACI_DET_TH_H,
    E_T2_TS_SERIAL     = 0x52,
    E_T2_TS_CLK_RATE   = 0x53,
    E_T2_TS_OUT_INV    = 0x54,
    E_T2_TS_DATA_SWAP  = 0x55,
    E_T2_TDP_CCI_KP,
    E_T2_CCI_FSWEEP,      //0x57
    E_T2_TS_ERR_POL,      //0x58
    E_T2_IF_AGC_INV_PWM_EN, // 0x59

    E_T2_TOTAL_CFO_0      = 0x85,
    E_T2_TOTAL_CFO_1,

    // dvbt2 lock history
    E_T2_DVBT2_LOCK_HIS   = 0xF0,
    E_T2_FEF_DET_IND,
    E_T2_MPLP_NO_COMMON_IND,
    E_T2_SNR_L,             // 0xf3
    E_T2_SNR_H,             // 0xf4
    E_T2_DOPPLER_DET_FLAG,  // 0xf5
    E_T2_DOPPLER_DET_TH_L,  // 0xf6
    E_T2_DOPPLER_DET_TH_H,  // 0xf7

    // splp, mplp releted
    E_T2_PLP_ID_ARR       = 0x100,
    E_T2_L1_FLAG          = 0x120,
    E_T2_PLP_ID,
    E_T2_GROUP_ID,
    E_T2_PARAM_NUM,
} E_DVBT2_PARAM;
#endif
/*
// Move to Tuner_SSI.h
typedef enum
{
    _QPSK        = 0x0,
    _16QAM        = 0x1,
    _64QAM        = 0x2,
}E_CONSTEL;

typedef enum
{
    _CR1Y2        = 0x0,
    _CR2Y3        = 0x1,
    _CR3Y4        = 0x2,
    _CR5Y6        = 0x3,
    _CR7Y8        = 0x4,
}E_CODERATE;


typedef struct
{
    U8        constel;
    U8        code_rate;
    float    cn_ref;
}S_SQI_CN_NORDIGP1_INTERN_DVBT2;

typedef struct
{
    float    power_db;
    U8        sar3_val;
}S_INTERN_DVBT2_RFAGC_SSI;

typedef struct
{
    float    power_db;
    U8        agc_val;
}S_INTERN_DVBT2_IFAGC_SSI;

typedef struct
{
    U8        constel;
    U8        code_rate;
    float    p_ref;
}S_INTERN_DVBT2_SSI_PREF;

typedef struct
{
    float    attn_db;
    U8        agc_err;
}S_INTERN_DVBT2_IFAGC_ERR;
*/
//--------------------------------------------------------------------

//--------------------------------------------------------------------
MS_BOOL INTERN_DVBT2_Reset ( void );
MS_BOOL INTERN_DVBT2_SoftReset ( void );
MS_BOOL INTERN_DVBT2_Active(MS_BOOL bEnable);
MS_BOOL INTERN_DVBT2_Exit ( void );
MS_BOOL INTERN_DVBT2_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBT_DSPRegInitExt, MS_U8 u8DMD_DVBT_DSPRegInitSize);
MS_BOOL INTERN_DVBT2_DSPReg_Init(const MS_U8 *u8DVBT_DSPReg,  MS_U8 u8Size);
MS_BOOL INTERN_DVBT2_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk);
MS_BOOL INTERN_DVBT2_Config(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U32 u32IFFreq, MS_U8 u8PlpID);
DMD_T2_LOCK_STATUS INTERN_DVBT2_Lock(MS_U16 u16DMD_DVBT2_P1_Timeout, MS_U16 u16DMD_DVBT2_FEC_Timeout);
MS_BOOL INTERN_DVBT2_GetLock(DMD_DVBT2_GETLOCK_TYPE eStatus);
//MS_BOOL INTERN_DVBT2_GetSignalStrength(MS_U16 *strength, const DMD_DVBT2_InitData *sDMD_DVBT2_InitData, MS_U8 u8SarValue, float fRFPowerDbm);
//MS_BOOL INTERN_DVBT2_GetSignalQuality(MS_U16 *quality, const DMD_DVBT2_InitData *sDMD_DVBT2_InitData, MS_U8 u8SarValue, float fRFPowerDbm);
//MS_BOOL INTERN_DVBT2_GetPostLdpcBer(float *ber);
MS_BOOL INTERN_DVBT2_GetPostLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType);

//MS_BOOL INTERN_DVBT2_GetPreLdpcBer(float *ber);
MS_BOOL INTERN_DVBT2_GetPreLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType);

MS_BOOL INTERN_DVBT2_GetPacketErr(MS_U16 *u16PktErr);
//MS_BOOL INTERN_DVBT2_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW);
MS_BOOL INTERN_DVBT2_Get_FreqOffset(MS_U32 *CfoTd_reg, MS_U32 *CfoFd_reg, MS_U32 *Icfo_reg, MS_U8 *fft_reg);

MS_BOOL INTERN_DVBT2_Get_L1_Parameter( MS_U16 * pu16L1_parameter, DMD_DVBT2_SIGNAL_INFO eSignalType);
//float INTERN_DVBT2_GetSNR (void);
MS_BOOL INTERN_DVBT2_GetSNR (MS_U16 *u16_snr100,  MS_U8 *snr_cali, MS_U8 *u8_gi);

MS_BOOL INTERN_DVBT2_Version(MS_U16 *ver);
MS_BOOL INTERN_DVBT2_Show_Modulation_info(void);
MS_BOOL INTERN_DVBT2_Show_Demod_Info(void);
MS_BOOL INTERN_DVBT2_Show_Lock_Info(void);
MS_BOOL INTERN_DVBT2_Show_PRESFO_Info(void);
MS_BOOL INTERN_DVBT2_Show_Lock_Time_Info(void);
MS_BOOL INTERN_DVBT2_Show_BER_Info(void);
MS_BOOL INTERN_DVBT2_Show_AGC_Info(void);
//MS_BOOL INTERN_DVBT2_NORDIG_SSI_Table_Write(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value);
//MS_BOOL INTERN_DVBT2_NORDIG_SSI_Table_Read(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value);
MS_BOOL INTERN_DVBT2_GetPlpBitMap(MS_U8* u8PlpBitMap);
MS_BOOL INTERN_DVBT2_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID);
MS_BOOL INTERN_DVBT2_SetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID);

//--------------------------------------------------------------------

#define INTERN_DVBT2_LOAD_FW_FROM_CODE_MEMORY

#undef EXTSEL
#endif