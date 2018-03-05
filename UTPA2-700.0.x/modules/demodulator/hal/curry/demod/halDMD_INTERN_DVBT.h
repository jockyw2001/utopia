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

#ifndef _INTERN_DVBT_H_
#define _INTERN_DVBT_H_

#ifdef _INTERN_DVBT_C_
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




#define INTERN_DVBT_TS_SERIAL_INVERSION       0
#define INTERN_DVBT_TS_PARALLEL_INVERSION     1
#define INTERN_DVBT_DTV_DRIVING_LEVEL          1
#define INTERN_DVBT_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  1
#endif


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

#if 1
typedef enum
{
	T_OPMODE_RFAGC_EN = 0x00, 	// 0X00
	T_OPMODE_HUMDET_EN,			// 0X01
	T_OPMODE_DCR_EN,				// 0X02
	T_OPMODE_IIS_EN,				// 0X03
	T_OPMODE_CCI_EN,				// 0X04
	T_OPMODE_ACI_EN,				// 0X05
	T_OPMODE_IQB_EN,				// 0X06
	T_OPMODE_AUTO_IQ,				// 0X07
	T_OPMODE_AUTO_RFMAX,			// 0X08
	T_OPMODE_AUTO_ACI,			// 0X09
	T_OPMODE_FIX_MODE_CP,		// 0x0A
	T_OPMODE_FIX_TPS,				// 0X0B
	T_OPMODE_AUTO_SCAN,			// 0X0C
	T_OPMODE_RSV_0X0D,			// 0X0D
	T_OPMODE_RSV_0X0E,			// 0X0E
	T_OPMODE_RSV_0X0F,			// 0X0F
	T_CONFIG_RSSI,					// 0x10
	T_CONFIG_ZIF,					// 0X11
	T_CONFIG_FREQ,					// 0X12
	T_CONFIG_FC_L,					// 0X13
	T_CONFIG_FC_H,					// 0x14
	T_CONFIG_FS_L, 					// 0x15
	T_CONFIG_FS_H,		 			// 0x16
	T_CONFIG_BW,					// 0x17
	T_CONFIG_MODE,	 			// 0x18
	T_CONFIG_CP,   		  			// 0x19
	T_CONFIG_LP_SEL, 	 			// 0x1A
	T_CONFIG_CSTL,					// 0x1B
	T_CONFIG_HIER,					// 0x1C
	T_CONFIG_HPCR,					// 0x1D
	T_CONFIG_LPCR,					// 0x1E
	T_CONFIG_IQ_SWAP,				// 0x1F
	T_CONFIG_RFMAX,				// 0X20
	T_CONFIG_CCI,					// 0X21
	T_CONFIG_ICFO_RANGE,			// 0x22
	T_CONFIG_RFAGC_REF,			// 0X23
	T_CONFIG_IFAGC_REF_2K,		// 0X24
	T_CONFIG_IFAGC_REF_8K,		// 0X25
	T_CONFIG_IFAGC_REF_ACI,		// 0X26
	T_CONFIG_IFAGC_REF_IIS,		// 0X27
	T_CONFIG_ACI_DET_TH_L,		// 0X28
	T_CONFIG_ACI_DET_TH_H,		// 0X29
	T_CONFIG_TS_SERIAL,			// 0X2A
	T_CONFIG_TS_CLK_RATE,			// 0X2B
	T_CONFIG_TS_OUT_INV,			// 0X2C
	T_CONFIG_TS_DATA_SWAP,		// 0X2D
	T_CONFIG_2K_SFO_H,			// 0X2E
	T_CONFIG_2K_SFO_L,			// 0X2F
	T_CONFIG_8K_SFO_H,			// 0X30
	T_CONFIG_8K_SFO_L,			// 0X31
	T_CONFIG_CHECK_CHANNEL,		// 0X32
	T_CONFIG_SLICER_SNR_POS,		// 0X33
    T_CONFIG_TDFE_CCI_KP,			// 0X34
    T_CONFIG_CCI_FSWEEP,			// 0X35
    T_CONFIG_TS_CLK_RATE_AUTO,	// 0X36
    T_CONFIG_IF_INV_PWM_OUT_EN, // 0x37
    T_CONFIG_TUNER_LOWIF,           // 0x38
    T_CONFIG_FIF_L,                 // 0x39
    T_CONFIG_FIF_H,                 // 0x3A
    T_CONFIG_TUNER_SAWLESS,         // 0x3B
    T_CONFIG_IFAGC_REF_2K_H,        // 0X3C
    T_CONFIG_IFAGC_REF_8K_H,        // 0X3D
    T_CONFIG_IFAGC_REF_IIS_H,       // 0x3E
    T_CONFIG_IFAGC_REF_ACI_H,       // 0x3F
    T_DVBT_PARAM_LEN,
} DVBT_fake_Param;
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

//--------------------------------------------------------------------
MS_BOOL INTERN_DVBT_Reset ( void );
MS_BOOL INTERN_DVBT_Active(MS_BOOL bEnable);
MS_BOOL INTERN_DVBT_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt);
MS_BOOL INTERN_DVBT_Exit ( void );
MS_BOOL INTERN_DVBT_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBT_DSPRegInitExt, MS_U8 u8DMD_DVBT_DSPRegInitSize);
MS_BOOL INTERN_DVBT_DSPReg_Init(const MS_U8 *u8DVBT_DSPReg,  MS_U8 u8Size);
MS_BOOL INTERN_DVBT_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk);
MS_BOOL INTERN_DVBT_Config ( DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U8 u8TSClk, MS_U32 u32IFFreq, MS_U32 u32FSFreq, MS_U8 u8IQSwap);
DMD_LOCK_STATUS INTERN_DVBT_Lock(MS_U16 u16DMD_DVBT_TPS_Timeout, MS_U16 u16DMD_DVBT_FEC_Timeout);
MS_BOOL INTERN_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE eStatus);
// 20160303 for kernel mode by oga.
// MS_BOOL INTERN_DVBT_GetSignalStrength(MS_U16 *strength, const DMD_DVBT_InitData *sDMD_DVBT_InitData, MS_U8 u8SarValue, float fRFPowerDbm);
// MS_BOOL INTERN_DVBT_GetSignalQuality(MS_U16 *quality, const DMD_DVBT_InitData *sDMD_DVBT_InitData, MS_U8 u8SarValue, float fRFPowerDbm);
MS_BOOL INTERN_DVBT_GetPostViterbiBer(MS_U16 *BitErrPeriod_reg, MS_U32 *BitErr_reg, MS_U16 *PktErr);
MS_BOOL INTERN_DVBT_GetPreViterbiBer(float *ber);
MS_BOOL INTERN_DVBT_GetPacketErr(MS_U16 *u16PktErr);
MS_BOOL INTERN_DVBT_Get_CELL_ID(MS_U16 *cell_id);
MS_BOOL INTERN_DVBT_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW);
MS_BOOL INTERN_DVBT_Get_TPS_Parameter_Const( MS_U16 * TPS_parameter, E_SIGNAL_TYPE eSignalType);
MS_BOOL INTERN_DVBT_Get_TPS_Info( MS_U16 * TPS_parameter);
void INTERN_DVBT_GetSNR (MS_U32 *noise_power_reg);
MS_BOOL INTERN_DVBT_Version(MS_U16 *ver);
MS_BOOL INTERN_DVBT_Show_Modulation_info(void);
MS_BOOL INTERN_DVBT_Show_Demod_Info(void);
MS_BOOL INTERN_DVBT_Show_Lock_Info(void);
MS_BOOL INTERN_DVBT_Show_PRESFO_Info(void);
MS_BOOL INTERN_DVBT_Show_Lock_Time_Info(void);
MS_BOOL INTERN_DVBT_Show_BER_Info(void);
MS_BOOL INTERN_DVBT_Show_AGC_Info(void);
MS_BOOL INTERN_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel, DMD_CODERATE code_rate, float write_value);
MS_BOOL INTERN_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL constel, DMD_CODERATE code_rate, float *read_value);
//arthur
MS_BOOL INTERN_DVBT_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);

//--------------------------------------------------------------------

#define INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY

#undef EXTSEL
#endif

