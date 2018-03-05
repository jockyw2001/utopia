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

#ifndef _DEV_SIF_H_
#define _DEV_SIF_H_

#include "MsTypes.h"

#define SIF_DSP_TYPE 	1	//0:DSP_DEC, 1:DSP_SE 

// for standard status
// [3:2]: 00:NTSC 01:A2 10:NICAM 11:OTHER
// BIT7: AUTO MODE
enum
{
    IS_MAIN_CARRIER= 0x01,
    IS_A2= 0x02,
    IS_NICAM= 0x04
};

enum
{
    AU_CARRIER1_IGNORE = 0,
    AU_CARRIER1_KOREA  = 1,
    AU_CARRIER1_BTSC   = 2,
    AU_CARRIER1_JAPAN  = 3
};
// 6.5 MHz
enum
{
    AU_CARRIER2_L  = 0x10,
    AU_CARRIER2_DK = 0x20
};

enum
{
    AU_INFO_CARRIER_1_AMP,
    AU_INFO_CARRIER_1_VAR,
    AU_INFO_CARRIER_2_AMP,
    AU_INFO_CARRIER_2_VAR,
    AU_INFO_CARRIER_STATE,
    AU_INFO_ERROR_COUNT,
    AU_INFO_BTSC_MAIN_AMP,
    AU_INFO_BTSC_MAIN_VAR,
    AU_INFO_EIAJ_MAIN_AMP,
    AU_INFO_EIAJ_MAIN_VAR,
    AU_INFO_NICAM_BIT_ERROR,
    AU_INFO_AM_MAIN_AMP,
};

typedef enum
{
   AUD_SIF_42M,
   AUD_SIF_43M,
   AUD_SIF_44M,
   AUD_SIF_48M   
} AU_SIF_CLOCK_Type;

//sif threshold address
#if (SIF_DSP_TYPE == 0)
#define BTSC_THRESHOLD_ADDR         BTSC_THRESHOLD_Addr
#define M_A2_THRESHOLD_ADDR         ADDR_a2_stdM_thr
#define BG_A2_THRESHOLD_ADDR        ADDR_a2_stdBG_thr
#define DK_A2_THRESHOLD_ADDR        ADDR_a2_stdDK_thr
#define I_FM_THRESHOLD_ADDR         ADDR_a2_stdI_thr
#define L_AM_THRESHOLD_ADDR         ADDR_am_thr
#define NICAM_BGDKL_THRESHOLD_ADDR  ADDR_nicam_stdBG_pherr_thr
#define NICAM_I_THRESHOLD_ADDR      ADDR_nicam_stdI_pherr_thr

#define HIDEV_M_THRESHOLD_ADDR      ADDR_hidev_stdM_thr
#define HIDEV_BG_THRESHOLD_ADDR     ADDR_hidev_stdBG_thr
#define HIDEV_DK_THRESHOLD_ADDR     ADDR_hidev_stdDK_thr
#define HIDEV_I_THRESHOLD_ADDR      ADDR_hidev_stdI_thr

//sif gain address
#define A2_OUTPUT_GAIN_X_ADDR   ADDR_fm_stdX_gain
#define NICAM_OUTPUT_GAIN_ADDR  ADDR_nicam_gain
#define AM_OUTPUT_GAIN_ADDR     ADDR_am_gain
#define A2_OUTPUT_GAIN_M_ADDR   ADDR_fm_stdM_gain
#define BTSC_OUTPUT_GAIN_ADDR   BTSC_OUTPUT_GAIN_Addr
#define BTSC_MTS_OUTPUT_GAIN    MTS_OUTPUT_GAIN_Addr
#else
#define BTSC_THRESHOLD_ADDR         BTSC_THRESHOLD_Addr_2
#define M_A2_THRESHOLD_ADDR         ADDR_a2_stdM_thr_2
#define BG_A2_THRESHOLD_ADDR        ADDR_a2_stdBG_thr_2
#define DK_A2_THRESHOLD_ADDR        ADDR_a2_stdDK_thr_2
#define I_FM_THRESHOLD_ADDR         ADDR_a2_stdI_thr_2
#define L_AM_THRESHOLD_ADDR         ADDR_am_thr_2
#define NICAM_BGDKL_THRESHOLD_ADDR  ADDR_nicam_stdBG_pherr_thr_2
#define NICAM_I_THRESHOLD_ADDR      ADDR_nicam_stdI_pherr_thr_2

#define HIDEV_M_THRESHOLD_ADDR      ADDR_hidev_stdM_thr_2
#define HIDEV_BG_THRESHOLD_ADDR     ADDR_hidev_stdBG_thr_2
#define HIDEV_DK_THRESHOLD_ADDR     ADDR_hidev_stdDK_thr_2
#define HIDEV_I_THRESHOLD_ADDR      ADDR_hidev_stdI_thr

//sif gain address
#define A2_OUTPUT_GAIN_X_ADDR   ADDR_fm_stdX_gain_2
#define NICAM_OUTPUT_GAIN_ADDR  ADDR_nicam_gain_2
#define AM_OUTPUT_GAIN_ADDR     ADDR_am_gain_2
#define A2_OUTPUT_GAIN_M_ADDR   ADDR_fm_stdM_gain_2
#define BTSC_OUTPUT_GAIN_ADDR   BTSC_OUTPUT_GAIN_Addr_2
#define BTSC_MTS_OUTPUT_GAIN    MTS_OUTPUT_GAIN_Addr_2
#endif

//sif sub-carrier set
#define SIF_MODE_NICAM          0x40
#define SIF_MODE_A2             0x20
#define SIF_MODE_HIDEV          0x10
#define SIF_MODE_MONO           0x00

//sif hidev bandwidth type
#define HIDEV_PFIR_BW_NARROW    0x10
#define HIDEV_PFIR_BW_MIDDLE    0x20
#define HIDEV_PFIR_BW_WIDE      0x30

//Mailbox definition
#define AU_CMD_TONE             M2S_MBOX_SINE_GEN//0x2D28
#if (SIF_DSP_TYPE == 0)
#define AU_CMD_STANDARD         M2D_MBOX_SIF_CMD_STANDARD
#define AU_CMD_PFIRBANDWIDTH    M2D_MBOX_SIF_CMD_PFIRBANDWIDTH     //HiDEV[5:4], A2 carrier2 pfir[1:0]
#define AU_CMD_AGC              M2D_MBOX_SIF_CMD_PFIRBANDWIDTH+1  //[0] enable/disable AGC, [1]reset AGC
#define AU_CMD_MODE1            M2D_MBOX_SIF_CMD_MODE1
#define AU_CMD_MODE2            M2D_MBOX_SIF_CMD_MODE2
#define AU_CMD_DEBUG            M2D_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_CMD          M2D_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_DATA_H       M2D_MBOX_DBG_CMD1
#define AU_CMD_DBG_DATA_M       M2D_MBOX_DBG_CMD2+1
#define AU_CMD_DBG_DATA_L       M2D_MBOX_DBG_CMD2
#define AU_CMD_TONE             M2S_MBOX_SINE_GEN

#define AU_STATUS_STANDARD      D2M_MBOX_SIF_DETECTION_RESULT
#define AU_STATUS_MODE1         D2M_MBOX_SIF_STATUS_MODE1
#define AU_STATUS_MODE2         D2M_MBOX_SIF_STATUS_MODE2
#define AU_STATUS_DEBUG1        D2M_MBOX_DBG_RESULT1
#define AU_STATUS_DEBUG2        D2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_H         D2M_MBOX_DBG_RESULT1
#define AU_STATUS_DBG_M         D2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_L         D2M_MBOX_DBG_RESULT2
#else
#define AU_CMD_STANDARD         M2S_MBOX_SIF_CMD_STANDARD
#define AU_CMD_PFIRBANDWIDTH    M2S_MBOX_SIF_CMD_PFIRBANDWIDTH     //HiDEV[5:4], A2 carrier2 pfir[1:0]
#define AU_CMD_AGC              M2S_MBOX_SIF_CMD_PFIRBANDWIDTH+1  //[0] enable/disable AGC, [1]reset AGC
#define AU_CMD_MODE1            M2S_MBOX_SIF_CMD_MODE1
#define AU_CMD_MODE2            M2S_MBOX_SIF_CMD_MODE2
#define AU_CMD_DEBUG            M2S_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_CMD          M2S_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_DATA_H       M2S_MBOX_DBG_CMD1
#define AU_CMD_DBG_DATA_M       M2S_MBOX_DBG_CMD2+1
#define AU_CMD_DBG_DATA_L       M2S_MBOX_DBG_CMD2
#define AU_CMD_TONE             M2S_MBOX_SINE_GEN

#define AU_STATUS_STANDARD      S2M_MBOX_SIF_DETECTION_RESULT
#define AU_STATUS_MODE1         S2M_MBOX_SIF_STATUS_MODE1
#define AU_STATUS_MODE2         S2M_MBOX_SIF_STATUS_MODE2
#define AU_STATUS_DEBUG1        S2M_MBOX_DBG_RESULT1
#define AU_STATUS_DEBUG2        S2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_H         S2M_MBOX_DBG_RESULT1
#define AU_STATUS_DBG_M         S2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_L         S2M_MBOX_DBG_RESULT2
#endif

#define SIF_ADC_FROM_VIF_PATH    0x08

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_SIF_Init(void);
void    HAL_SIF_TriggerSifPLL(void);

MS_U8   HAL_SIF_GetDspType(void);
void    HAL_SIF_SetDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType);
AUDIO_DSP_CODE_TYPE HAL_SIF_GetDspCodeType(void);

MS_BOOL HAL_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE u8Mode_type);
MS_BOOL HAL_SIF_StartAutoDetect(void);

MS_U32  HAL_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value);
MS_BOOL HAL_SIF_GetOrginalGain(void);
MS_BOOL HAL_SIF_SetPrescale(MS_U8 u8Type,int db_value);

MS_U8   HAL_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2);
MS_BOOL HAL_SIF_ReLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
MS_BOOL HAL_SIF_SetSystem(AUDIO_DSP_CODE_TYPE dspCodeType);

MS_BOOL HAL_SIF_CheckDecSys(En_DVB_decSystemType enDecSystem);
#endif // _DEV_SIF_H_
