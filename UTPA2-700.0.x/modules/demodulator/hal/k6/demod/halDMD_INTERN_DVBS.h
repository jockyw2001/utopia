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

#ifndef _INTERN_DVBS_H_
#define _INTERN_DVBS_H_

#ifdef _INTERN_DVBS_C_
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
    #define DEMOD_CAPTURE_RANGE_SIZE			 DEMOD_CAPTURE_RANGE_500_K
#endif

#define MDrv_ReadByte(x)  HAL_DMD_RIU_ReadByte(x)
#define MDrv_WriteByte(x,y)  HAL_DMD_RIU_WriteByte(x,y)

#if 1
#define U8      unsigned char
#define U16     unsigned short
#define U32     unsigned long
#define BOOL    unsigned char
#define BOOLEAN  unsigned char
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


#define INTERN_DVBS_TS_SERIAL_INVERSION       			0
#define INTERN_DVBS_TS_PARALLEL_INVERSION     			1
#define INTERN_DVBS_DTV_DRIVING_LEVEL          			1
#define INTERN_DVBS_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE  	1
#endif

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
    E_SYS_DVBS,

    E_SYS_NUM
}E_SYSTEM;

//--------------------------------------------------------------------
MS_BOOL INTERN_DVBS_Reset ( void );
MS_U16 	INTERN_DVBS_DSPReg_Init(const MS_U8 *u8DVBS_DSPReg,  MS_U8 u8Size);
MS_BOOL INTERN_DVBS_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt);
MS_BOOL INTERN_DVBS_Exit ( void );
MS_BOOL INTERN_DVBS_Active(MS_BOOL bEnable);

MS_BOOL INTERN_DVBS_GetPostViterbiBer(MS_U32 *BitErr, MS_U16 *BitErrPeriod);

MS_BOOL INTERN_DVBS_GetPerViterbiBer(float *perber);
MS_BOOL INTERN_DVBS_GetPacketErr(MS_U16 *pktErr);

MS_BOOL INTERN_DVBS_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBS_DSPRegInitExt, MS_U8 u8DMD_DVBS_DSPRegInitSize);

MS_BOOL INTERN_DVBS_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk);
MS_BOOL INTERN_DVBS_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
MS_BOOL INTERN_DVBS_BlindScan_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
MS_BOOL INTERN_DVBS_Active(MS_BOOL bEnable);
MS_BOOL INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK_TYPE eType, MS_U16 fCurrRFPowerDbm, MS_U16 fNoChannelRFPowerDbm, MS_U32 u32TimeInterval);
MS_BOOL INTERN_DVBS_GetSignalStrength(MS_U16 fRFPowerDbm, DMD_DVBS_DEMOD_TYPE *pDemodType, MS_U8  *u8_DVBS2_CurrentCodeRateLocal,  MS_U8   *u8_DVBS2_CurrentConstellationLocal);
//MS_BOOL INTERN_DVBS_GetSignalQuality(MS_U16 *quality, const DMD_DVBS_InitData *sDMD_DVBS_InitData, MS_U8 u8SarValue, float fRFPowerDbm);

MS_BOOL INTERN_DVBS_GetSNR(MS_U32 *u32NDA_SNR_A, MS_U32 *u32NDA_SNR_AB);

MS_BOOL INTERN_DVBS_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);
MS_BOOL INTERN_DVBS_Get_CELL_ID(MS_U16 *cell_id);
// MS_BOOL INTERN_DVBC_Get_TPS_Parameter_Const( MS_U16 * TPS_parameter, E_SIGNAL_TYPE eSignalType);
MS_BOOL INTERN_DVBS_Version(MS_U16 *ver);
MS_BOOL INTERN_DVBS_GetCurrentSymbolRate(MS_U32 *u32SymbolRate);
MS_BOOL INTERN_DVBS_Get_FreqOffset(MS_S16 *s16CFO);

void INTERN_DVBS_info(void);
MS_BOOL INTERN_DVBS_Show_AGC_Info(void);
MS_BOOL INTERN_DVBS_GetTsDivNum(MS_U32 *u32SymbolRate, MS_U8* system_type_reg, MS_U8 *code_rate_idx, MS_U8 *fec_type_idx, MS_U8 *pilot_flag, MS_U32 *u32temp, MS_U8 *code_rate_reg);
MS_BOOL INTERN_DVBS_GetCurrentModulationType(DMD_DVBS_MODULATION_TYPE *pQAMMode);
MS_BOOL INTERN_DVBS_GetCurrentDemodType(DMD_DVBS_DEMOD_TYPE *pDemodType);
MS_BOOL INTERN_DVBS_GetCurrentDemodCodeRate(DMD_DVBS_CODE_RATE_TYPE *pCodeRate);
MS_BOOL INTERN_DVBS_GetTunrSignalLevel_PWR(MS_U16 *u16Data);
//--------------------------------------------------------------------
MS_BOOL INTERN_DVBS_GetRollOff(MS_U8 *pRollOff);
MS_BOOL INTERN_DVBS_BlindScan_GetTunerFreq(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq);

MS_BOOL INTERN_DVBS_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq);
MS_BOOL INTERN_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd);
MS_BOOL INTERN_DVBS_BlindScan_Cancel(void);
MS_BOOL INTERN_DVBS_BlindScan_End(void);
MS_BOOL INTERN_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable);
MS_BOOL INTERN_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq);
MS_BOOL INTERN_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum, MS_U8 *substate_reg, MS_U32  *u32Data, MS_U16 *symbolrate_reg, MS_U16 *CFO_reg);

MS_BOOL INTERN_DVBS_DiSEqC_Init(void);
MS_BOOL INTERN_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow);
MS_BOOL INTERN_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow);
MS_BOOL INTERN_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn);
MS_BOOL INTERN_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn);
MS_BOOL INTERN_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize);
MS_BOOL INTERN_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff);
MS_BOOL INTERN_DVBS_DiSEqC_SetTone(MS_BOOL bTone1);
MS_BOOL INTERN_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower);

//MS_BOOL INTERN_DVBS_Demod_Restart(MS_U32 u32SymbolRate, MS_U32 u32Frequency);
MS_BOOL INTERN_DVBS_Get_DTV_SQuality_BAR(MS_U8 *u8_gSQValue);
MS_BOOL INTERN_DVBS_Demod_Get_Debug_Info_get_once(void);
MS_BOOL INTERN_DVBS_Demod_Get_Debug_Info_polling(void);
MS_BOOL INTERN_DVBS_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data);
MS_BOOL INTERN_DVBS_ReadReg2bytes(MS_U16 u16Addr, MS_U16 *pu16Data);
MS_BOOL INTERN_DVBS_DTV_FrontendSetFreeze(void);
MS_BOOL INTERN_DVBS_DTV_FrontendUnFreeze(void);
MS_BOOL INTERN_DVBS_DTV_InnerSetFreeze(void);
MS_BOOL INTERN_DVBS_DTV_InnerUnFreeze(void);

#define INTERN_DVBS_LOAD_FW_FROM_CODE_MEMORY

#undef EXTSEL
#endif

