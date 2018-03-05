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

#ifndef _INTERN_DVBC_H_
#define _INTERN_DVBC_H_

#ifdef _INTERN_DVBT_C_
#define EXTSEL
#else
#define EXTSEL extern
#endif

#define NEW_TR_MODULE
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
    //INTERN_DVBT_ Capture Range fix to 500K
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




#define INTERN_DVBC_TS_SERIAL_INVERSION       0
#define INTERN_DVBC_TS_PARALLEL_INVERSION     1
#define INTERN_DVBC_DTV_DRIVING_LEVEL          1
#define INTERN_DVBC_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  1
#endif

typedef enum
{
    E_RF_QAM_MODE_16QAM  = 0x00,     ///< 16QAM
    E_RF_QAM_MODE_32QAM  = 0x01,     ///< 32QAM
    E_RF_QAM_MODE_64QAM  = 0x02,     ///< 64QAM
    E_RF_QAM_MODE_128QAM = 0x03,     ///< 128QAM
    E_RF_QAM_MODE_256QAM = 0x04,     ///< 256QAM
    E_RF_QAM_MODE_INVALID            ///< Invalid
} RF_CHANNEL_QAM_MODE;

#if 0
typedef enum
{
    COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    COFDM_TPS_LOCK,
    COFDM_DCR_LOCK,
    COFDM_AGC_LOCK,
    COFDM_MODE_DET,

} COFDM_LOCK_STATUS;

//--------------------------------------------------------------------
typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,

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

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    pc_op_code = 0,
    pc_if_freq,
    pc_sound_sys,
    pc_vif_vga_maximum_l,
    pc_vif_vga_maximum_h,
    pc_scan_mode,
    pc_vif_top,
    pc_gain_distribution_thr_l,
    pc_gain_distribution_thr_h,

    VIF_PARAM_MAX_NUM
}E_VIF_PARAM;

typedef enum
{
    pc_system = 0,

    SYS_PARAM_MAX_NUM
}E_SYS_PARAM;

typedef enum
{
    SET_IF_FREQ = 0,
    SET_SOUND_SYS,
    VIF_INIT,
    SET_VIF_HANDLER,
    VIF_TOP_ADJUST,

    VIF_CMD_MAX_NUM
}E_VIF_CMD;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;

typedef enum
{
    dac_op_code = 0,
    dac_idac_ch0,
    dac_idac_ch1,

    DAC_PARAM_MAX_NUM
}
E_DAC_PARAM;

typedef enum
{
    DAC_RUN_CALI = 0,
    DAC_IDAC_ASSIGN,

    DAC_CMD_MAX_NUM
}
E_DAC_CMD;

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

typedef enum
{
    E_RF_QAM_MODE_16QAM  = 0x00,     ///< 16QAM
    E_RF_QAM_MODE_32QAM  = 0x01,     ///< 32QAM
    E_RF_QAM_MODE_64QAM  = 0x02,     ///< 64QAM
    E_RF_QAM_MODE_128QAM = 0x03,     ///< 128QAM
    E_RF_QAM_MODE_256QAM = 0x04,     ///< 256QAM
    E_RF_QAM_MODE_INVALID            ///< Invalid
} RF_CHANNEL_QAM_MODE;

typedef enum
{
	S0_entry_num,
	S10_PSYNC_fail_num,
	S10_TPS_invalid_num,
	S8_TPS_invalid_num,
	S6_TPS_unlock_num,
	S4_Mode_CP_unlock_num,
	CCI_Tracking_lock_num,
	CCI_Tracking_lock_p1_num,
	CCI_Tracking_lock_p2_num,
	S11_PSYNC_FAIL_LOCKED_num,
	S11_PSYNC_FAIL_SEARCH_num,
	lock_time_l,
	lock_time_h,
	hw_channel_length_l,
	hw_channel_length_h,
	sw_channel_length_l,
	sw_channel_length_h,
	sw_offset_SA_l,
	sw_offset_SA_h,
	sw_oneshot_peak_num,
	CI_Indicator,
	ACI_Indicator,
	FD_coeff,
	TD_coeff,
	SNR_Select,
	FsaMode,
	InGI,
	Fsa_Stop_Track,
	short_echo_Det,

	DBG_LIST_NUM
}DBG_table_type;
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
}S_SQI_CN_NORDIGP1_INTERN_DVBT;

typedef struct
{
    float    power_db;
    U8        sar3_val;
}S_INTERN_DVBT_RFAGC_SSI;

typedef struct
{
    float    power_db;
    U8        agc_val;
}S_INTERN_DVBT_IFAGC_SSI;

typedef struct
{
    U8        constel;
    U8        code_rate;
    float    p_ref;
}S_INTERN_DVBT_SSI_PREF;

typedef struct
{
    float    attn_db;
    U8        agc_err;
}S_INTERN_DVBT_IFAGC_ERR;
*/
//--------------------------------------------------------------------
typedef struct
{
    MS_U8        cmd_code;
    MS_U8        param[64];
} S_CMDPKTREG;

typedef enum
{
    TS_MODUL_MODE,
    TS_FFX_VALUE,
    TS_GUARD_INTERVAL,
    TS_CODE_RATE,

    TS_PARAM_MAX_NUM
}E_SIGNAL_TYPE;

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

    CMD_MAX_NUM
}E_CMD_CODE;

typedef enum
{
    TS_PARALLEL = 0,
    TS_SERIAL = 1,

    TS_MODE_MAX_NUM
}E_TS_MODE;

typedef enum
{
    E_SYS_UNKOWN = -1,
    E_SYS_DVBT,
    E_SYS_DVBC,
    E_SYS_ATSC,
    E_SYS_VIF,

    E_SYS_NUM
}E_SYSTEM;
//--------------------------------------------------------------------
MS_BOOL INTERN_DVBC_Reset ( void );
MS_BOOL INTERN_DVBC_Active(MS_BOOL bEnable);
MS_BOOL INTERN_DVBC_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt);
MS_BOOL INTERN_DVBC_Exit ( void );
MS_BOOL INTERN_DVBC_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBC_DSPRegInitExt, MS_U8 u8DMD_DVBC_DSPRegInitSize);
MS_U16 INTERN_DVBC_DSPReg_Init(const MS_U8 *u8DVBC_DSPReg,  MS_U8 u8Size);
MS_BOOL INTERN_DVBC_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk);
MS_BOOL INTERN_DVBC_Config(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
MS_BOOL INTERN_DVBC_Active(MS_BOOL bEnable);

MS_BOOL INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, MS_U32 u32CurrRFPowerDbm, MS_U32 u32NoChannelRFPowerDbm, MS_U32 u32TimeInterval);
//waiting add
MS_BOOL INTERN_DVBC_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);
//MS_BOOL INTERN_DVBC_GetSignalStrength(MS_U16 *strength, const DMD_DVBC_InitData *sDMD_DVBC_InitData, MS_U8 u8SarValue);
//MS_BOOL INTERN_DVBC_GetSignalQuality(MS_U16 *quality, const DMD_DVBC_InitData *sDMD_DVBC_InitData, MS_U8 u8SarValue);
MS_BOOL INTERN_DVBC_GetPostViterbiBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg);

MS_BOOL INTERN_DVBC_GetPacketErr(MS_U16 *pktErr);

MS_BOOL INTERN_DVBC_GetSNR(MS_U16 *snr_reg);

MS_BOOL INTERN_DVBC_Get_CELL_ID(MS_U16 *cell_id);
// MS_BOOL INTERN_DVBC_Get_TPS_Parameter_Const( MS_U16 * TPS_parameter, E_SIGNAL_TYPE eSignalType);
MS_BOOL INTERN_DVBC_Version(MS_U16 *ver);
MS_BOOL INTERN_DVBC_GetCurrentSymbolRate(MS_U16 *u16SymbolRate);
MS_BOOL INTERN_DVBC_Get_FreqOffset(MS_U32 *config_Fc_reg, MS_U32 *Fc_over_Fs_reg, MS_U16 *Cfo_offset_reg, MS_U8 u8BW);
MS_BOOL INTERN_DVBC_GetCurrentModulationType(DMD_DVBC_MODULATION_TYPE *pQAMMode);
//--------------------------------------------------------------------

#define INTERN_DVBC_LOAD_FW_FROM_CODE_MEMORY

#undef EXTSEL
#endif

