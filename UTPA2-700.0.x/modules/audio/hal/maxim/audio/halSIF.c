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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

// Internal Definition
#include "regCHIP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_SIF(msg) //msg

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HINIBBLE(u8Data)    ((u8Data) >> 4)
#define LONIBBLE(u8Data)    ((u8Data) << 4)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
static MS_U16 SIF_MailBoxArray[4];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 *g_AudioVars2;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bHiDev_Flag = FALSE;
static MS_BOOL bDetect_Main_Std_Only_Flag = FALSE;
static MS_U8   hidev_filter_bandwidth_level = 0x10;
static MS_BOOL gAutoMute = FALSE;
static MS_BOOL gAutoSoundmode = TRUE;
static MS_BOOL bADCFromVifPathSupported = TRUE, bADCFromVifPathEnabled = TRUE;   // for T3, default enable vif path.
static MS_BOOL beeper_enable = FALSE;
static MS_BOOL vif_patch_flag = TRUE; // If it's ture, chips need to be added vif patches for palsum vif 42M & 44M.
static AUDIO_DSP_CODE_TYPE   u8sifSystemType = AU_SIF_NONE;
static AU_SIF_CLOCK_Type  sif_clock = AUD_SIF_43M;

const MS_U16  PRESCALE_STEP_TBL[]=
{
    0x7E2B,       // -0.125dB
    0x7C5E,       // -0.25dB
    0x78D6,       // -0.5dB
    0x7214,       // -1dB
    0x65AC,       // -2dB
    0x50C3,       // -4dB
};

// for DVB project threshold function..
const MS_U16 au_pal_sys_threshold_SE[12]=
{
    M_A2_THRESHOLD_ADDR_SE,
    BG_A2_THRESHOLD_ADDR_SE,
    DK_A2_THRESHOLD_ADDR_SE,
    I_FM_THRESHOLD_ADDR_SE,
    L_AM_THRESHOLD_ADDR_SE,
    NICAM_BGDKL_THRESHOLD_ADDR_SE,
    NICAM_I_THRESHOLD_ADDR_SE,
    HIDEV_M_THRESHOLD_ADDR_SE,
    HIDEV_BG_THRESHOLD_ADDR_SE,
    HIDEV_DK_THRESHOLD_ADDR_SE,
    HIDEV_I_THRESHOLD_ADDR_SE,
    BTSC_THRESHOLD_ADDR_SE,
};

const MS_U16  SIF_PM_GAIN_TBL_PAL_SE[6][2]=
{
   { A2_OUTPUT_GAIN_X_ADDR_SE,   A2_OUTPUT_GAIN_X_ADDR_SE+1},  //A2 / FM-MONO
   { NICAM_OUTPUT_GAIN_ADDR_SE,  NICAM_OUTPUT_GAIN_ADDR_SE+1}, //NICAM
   { AM_OUTPUT_GAIN_ADDR_SE,     AM_OUTPUT_GAIN_ADDR_SE+1},    //AM
   { A2_OUTPUT_GAIN_X_ADDR_SE+2, A2_OUTPUT_GAIN_X_ADDR_SE+3},  //HIDEV
   { A2_OUTPUT_GAIN_M_ADDR_SE,   A2_OUTPUT_GAIN_M_ADDR_SE+1},  //FM-MONO M
   { A2_OUTPUT_GAIN_M_ADDR_SE+2, A2_OUTPUT_GAIN_M_ADDR_SE+3},  //FM-MONO M HIDEV
};

const MS_U16  SIF_PM_GAIN_TBL_BTSC_SE[4][2]=
{
   { BTSC_OUTPUT_GAIN_ADDR_SE,   BTSC_OUTPUT_GAIN_ADDR_SE+1},  //BTSC Total Gain
   { BTSC_MTS_OUTPUT_GAIN_SE,    BTSC_MTS_OUTPUT_GAIN_SE+1},   //BTSC Mono
   { BTSC_MTS_OUTPUT_GAIN_SE+2,  BTSC_MTS_OUTPUT_GAIN_SE+3},   //BTSC Stereo
   { BTSC_MTS_OUTPUT_GAIN_SE+4,  BTSC_MTS_OUTPUT_GAIN_SE+5},   //BTSC SAP
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U32  _Convert_SIF_Reg(MS_U32 u32RegAddr)
{
    if (g_AudioVars2->g_u8SifDspType == DSP_DEC)
    {
        return u32RegAddr;
    }

    MS_U32 resultReg = u32RegAddr;
    switch(u32RegAddr)
    {
        case AU_CMD_STANDARD:
            resultReg = AU_CMD_STANDARD_SE;
            break;

        case AU_CMD_PFIRBANDWIDTH:
            resultReg = AU_CMD_PFIRBANDWIDTH_SE;
            break;

        case AU_CMD_AGC:
            resultReg = AU_CMD_AGC_SE;
            break;

        case AU_CMD_MODE1:
            resultReg = AU_CMD_MODE1_SE;
            break;

        case AU_CMD_MODE2:
            resultReg = AU_CMD_MODE2_SE;
            break;

        //case AU_CMD_DEBUG:
        case AU_CMD_DBG_CMD:
            resultReg = AU_CMD_DBG_CMD_SE;
            break;

        case AU_CMD_DBG_DATA_H:
            resultReg = AU_CMD_DBG_DATA_H_SE;
            break;

        case AU_CMD_DBG_DATA_M:
            resultReg = AU_CMD_DBG_DATA_M_SE;
            break;

        case AU_CMD_DBG_DATA_L:
            resultReg = AU_CMD_DBG_DATA_L_SE;
            break;

        case AU_CMD_TONE:
            resultReg = AU_CMD_TONE_SE;
            break;

        case AU_STATUS_STANDARD:
            resultReg = AU_STATUS_STANDARD_SE;
            break;

        case AU_STATUS_MODE1:
            resultReg = AU_STATUS_MODE1_SE;
            break;

        case AU_STATUS_MODE2:
            resultReg = AU_STATUS_MODE2_SE;
            break;

        //case AU_STATUS_DEBUG1:
        //    resultReg = AU_STATUS_DEBUG1_SE;
       //     break;

        //case AU_STATUS_DEBUG2:
        //    resultReg = AU_STATUS_DEBUG2_SE;
        //    break;

        case AU_STATUS_DBG_H:
            resultReg = AU_STATUS_DBG_H_SE;
            break;

        case AU_STATUS_DBG_M:
            resultReg = AU_STATUS_DBG_M_SE;
            break;

        case AU_STATUS_DBG_L:
            resultReg = AU_STATUS_DBG_L_SE;
            break;

        default:
            break;
    }
    return resultReg;
}

static void HAL_SIF_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    HAL_AUDIO_AbsWriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

static MS_U8 HAL_SIF_ReadByte(MS_U32 u32RegAddr)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    return (HAL_AUDIO_ReadByte(u32RegAddr));
}

static void HAL_SIF_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    HAL_AUDIO_WriteByte(u32RegAddr, u8Val);
}

static void HAL_SIF_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    HAL_AUDIO_WriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

static MS_U16 HAL_SIF_ReadReg(MS_U32 u32RegAddr)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    return (HAL_AUDIO_ReadReg(u32RegAddr));
}

static void HAL_SIF_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    u32RegAddr = _Convert_SIF_Reg(u32RegAddr);
    HAL_AUDIO_WriteReg(u32RegAddr, u16Val);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_Init()
/// @brief \b Function \b Description: This routine is the initialization for SIF
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_SIF_Init(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_TriggerSifPLL()
/// @brief \b Function \b Description:  This function is used to initialize SIF analog part .
/// @param <IN>        \b u8Index   :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b MS_U32    :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_SIF_TriggerSifPLL(void)
{
    MS_U8 u8MuxSelect;
    MS_U8 uStandard;

    if (bADCFromVifPathEnabled == TRUE) //enable VIF
    {
        HAL_SIF_AbsWriteMaskByte(0x112c02, 0x04, 0x04);
        HAL_SIF_AbsWriteMaskByte(0x112cb0, 0x80, 0x80);
        HAL_SIF_AbsWriteMaskByte(0x112cb0, 0x60, 0x60);
        HAL_SIF_AbsWriteMaskByte(0x112CC6, 0x01, 0x01); // SIF AGC control by DSP
        HAL_SIF_AbsWriteMaskByte(0x112CC8, 0xFF, 0xe0);
        HAL_SIF_AbsWriteMaskByte(0x112CC9, 0xFF, 0x3E); // Enable VIF
    }
    else    //enable SIF
    {
        u8MuxSelect=HAL_AUDIO_AbsReadByte(0x101E39);         // Back up MUX setting
        HAL_SIF_AbsWriteMaskByte(0x101E39, 0x03, 0x00);  // Mux switch for HK MCU access bank 0x1120 & 0x1128

        HAL_SIF_AbsWriteMaskByte(0x112090, 0xFF, 0x00);
        HAL_SIF_AbsWriteMaskByte(0x112091, 0xFF, 0xEF);

        HAL_SIF_AbsWriteMaskByte(0x103314, 0xFF, 0x00);  // [3:0]  reg_ckg_dvbtc_adc
        HAL_SIF_AbsWriteMaskByte(0x103315, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x112002, 0xFF, 0x74);
        HAL_SIF_AbsWriteMaskByte(0x112003, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x1128AE, 0x80, 0x80);  // enable LDOs
        HAL_SIF_AbsWriteMaskByte(0x1128AE, 0x18, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x1128B1, 0x01, 0x00);  // enable LDO25
        HAL_SIF_AbsWriteMaskByte(0x112840, 0x10, 0x00);  // ref enable

        HAL_SIF_AbsWriteMaskByte(0x112802, 0xFF, 0x20);  // mux selection
        HAL_SIF_AbsWriteMaskByte(0x112803, 0xFF, 0x02);

        HAL_SIF_AbsWriteMaskByte(0x112816, 0xFF, 0x05);  // adc gain
        HAL_SIF_AbsWriteMaskByte(0x112817, 0xFF, 0x05);

        HAL_SIF_AbsWriteMaskByte(0x11281E, 0xFF, 0x80);  // calibration buffer
        HAL_SIF_AbsWriteMaskByte(0x11281F, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x112860, 0xFF, 0x00);  // dmpll post dividre
        HAL_SIF_AbsWriteMaskByte(0x112861, 0xFF, 0x10);

        HAL_SIF_AbsWriteMaskByte(0x112864, 0xFF, 0x00);  // MPLL_output_div_second
        HAL_SIF_AbsWriteMaskByte(0x112865, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x11287E, 0xFF, 0x00);  // SIF CLK select
        HAL_SIF_AbsWriteMaskByte(0x11287F, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x112866, 0xFF, 0x01);  // loop divider
        HAL_SIF_AbsWriteMaskByte(0x112867, 0xFF, 0x12);

        HAL_SIF_AbsWriteMaskByte(0x11286A, 0xFF, 0x83);  // pll power down
        HAL_SIF_AbsWriteMaskByte(0x11286B, 0xFF, 0x00);
        AUDIO_DELAY1MS(1);
        HAL_SIF_AbsWriteMaskByte(0x11286A, 0xFF, 0x03);  // pll power up
        HAL_SIF_AbsWriteMaskByte(0x11286B, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x11286C, 0xFF, 0x00);  // agc enable
        HAL_SIF_AbsWriteMaskByte(0x11286D, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x112818, 0xFF, 0x02);  // adc power down
        HAL_SIF_AbsWriteMaskByte(0x112819, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x112830, 0xFF, 0x01);
        HAL_SIF_AbsWriteMaskByte(0x112831, 0xFF, 0x00);

        HAL_SIF_AbsWriteMaskByte(0x101E39, 0x03, u8MuxSelect);  // Revert MUX setting

        HAL_SIF_AbsWriteMaskByte(0x112c02, 0x04, 0x04);
        HAL_SIF_AbsWriteMaskByte(0x112cb0, 0x80, 0x80);
        HAL_SIF_AbsWriteMaskByte(0x112cb0, 0x60, 0x60);
        HAL_SIF_AbsWriteMaskByte(0x112CC6, 0x01, 0x01); // SIF AGC control by DSP
        HAL_SIF_AbsWriteMaskByte(0x112CC8, 0xFF, 0xe0);
        HAL_SIF_AbsWriteMaskByte(0x112CC9, 0xFF, 0x2E); // Enable SIF

        HAL_SIF_AbsWriteMaskByte(0x1128C0, 0x02, 0x02); // IQ switch (demod setting)
    }

    uStandard = HAL_SIF_ReadByte(AU_CMD_STANDARD);
    HAL_SIF_WriteByte(AU_CMD_STANDARD, 0x00);
    AUDIO_DELAY1MS(10);
    HAL_SIF_WriteByte(AU_CMD_STANDARD, 0x20);
    AUDIO_DELAY1MS(10);
    HAL_SIF_WriteByte(AU_CMD_STANDARD, uStandard);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_GetDspType()
/// @brief \b Function \b Description:  This function is used to get the DSP(DSP_DEC or DSP_SE) which SIF module used.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8   : DSP which SIF modules (DSP_DEC or DSP_SE)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SIF_GetDspType(void)
{
    if (g_AudioVars2 == NULL)
    {
        DBG_SIF(printf("%s() : invalid g_AudioVars2!\n", __FUNCTION__));
        return DSP_SE;
    }

    return g_AudioVars2->g_u8SifDspType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_SetDspCodeType()
/// @brief \b Function \b Description:  This function is used to set the DSP code type SIF module used.
/// @param <IN>        \b MS_U8: SIF DSP code type.
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SIF_SetDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    if (g_AudioVars2 == NULL)
    {
        DBG_SIF(printf("%s() : invalid g_AudioVars2!\n", __FUNCTION__));
        return;
    }

    if (g_AudioVars2->g_u8SifDspType == DSP_DEC)
    {
        g_AudioVars2->g_DspCodeType = dspCodeType;
    }
    else
    {
        g_AudioVars2->g_Dsp2CodeType = dspCodeType;
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_GetDspCodeType()
/// @brief \b Function \b Description:  This function is used to set the DSP code type SIF module used.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: SIF DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_SIF_GetDspCodeType(void)
{
    if (g_AudioVars2 == NULL)
    {
        DBG_SIF(printf("%s() : invalid g_AudioVars2!\n", __FUNCTION__));
        return AU_DVB_STANDARD_INVALID;
    }

    if (g_AudioVars2->g_u8SifDspType == DSP_DEC)
    {
        return g_AudioVars2->g_DspCodeType;
    }
    else
    {
        return g_AudioVars2->g_Dsp2CodeType;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : HAL_SIF_SetSoundMode
/// @brief \b Function \b Description : This routine is to set the sound mode for SIF module.
/// @param <IN>        \b u8Type  : u8Mode_u8Type
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE u8Mode_Type)
{
    MS_U8 is_auto, is_auto_mute;
    DBG_SIF(printf("HAL_SIF_SetSoundMode(mode=%bx)\r\n", u8Mode_Type));

    if (gAutoMute==TRUE)
        is_auto_mute = 0x80;
    else
        is_auto_mute = 0x00;

    if (gAutoSoundmode==TRUE)
        is_auto = 0x80;
    else
        is_auto = 0x00;

    switch (u8Mode_Type)
    {
        case AU_MODE_MONO:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00|is_auto_mute);
            }
            else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_MONO)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00|is_auto_mute);
                }
                else if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00|is_auto_mute);
                }
                else if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE2, 0x01);
                    HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00|is_auto_mute);
                }
            }
            else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_EIAJ)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1, 0x00|is_auto_mute);
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error1!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_STEREO:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1,  0x01|is_auto);
            }
            else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1,  0x01|is_auto);
                }
                else if((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE2,  0x02);
                }
            }
            else if((HAL_SIF_GetDspCodeType()) == AU_SIF_EIAJ)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1,  0x01|is_auto);
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error2!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_SAP:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1,  0x02|is_auto);
            }
            else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2)
                    HAL_SIF_WriteByte(AU_CMD_MODE1,  0x02|is_auto);
            }
            else if((HAL_SIF_GetDspCodeType()) == AU_SIF_EIAJ)
            {
                HAL_SIF_WriteByte(AU_CMD_MODE1,  0x02|is_auto);
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error3!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_LANG_A:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1,  0x00|is_auto);
                }
                else if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM)
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE2,  0x06);
                }
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error4!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_LANG_B:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2 )
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1,  0x02|is_auto);
                }
                else if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM )
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE2,  0x07);
                }
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error5!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_LANG_AB:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                if ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_A2 )
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE1,  0x03|is_auto);
                }
                else if((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM )
                {
                    HAL_SIF_WriteByte(AU_CMD_MODE2,  0x05);
                }
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error6!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_NICAM_MONO:
            if (((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM) && ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM))
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2, 0x08);
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error7!! \r\n"));
                return FALSE;
            }
            break;

        case AU_MODE_FORCE_NICAM:
            if (((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM) && ((HAL_SIF_ReadByte(AU_CMD_STANDARD)&0xF0) == SIF_MODE_NICAM))
            {
                HAL_SIF_WriteByte(AU_CMD_MODE2, 0x80);
            }
            else
            {
                DBG_SIF(printf("\r\n set sound mode error8!! \r\n"));
                return FALSE;
            }
            break;

        default:
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : HAL_SIF_StartAutoDetect
/// @brief \b Function \b Description : This routine is to start to detect the sound system for SIF Palsum module.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SIF_StartAutoDetect(void)
{
    if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
        return FALSE;

    HAL_SIF_WriteMaskByte(AU_CMD_STANDARD,0xFF,0x00);
    AUDIO_DELAY1MS(1);
    HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
    if(bDetect_Main_Std_Only_Flag == 0)
        HAL_SIF_WriteByte(AU_CMD_STANDARD, 0xE0);
    else
        HAL_SIF_WriteByte(AU_CMD_STANDARD, 0xC0);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : HAL_SIF_SendCmd
/// @brief \b Function \b Description : This routine is to send command to SIF module to do corresponding action.
/// @param <IN>        \b AU_SIF_CMD : u8DecCmd, MS_U8 : comm_arg1, MS_U8 : comm_arg2
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b MS_U8 :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2)
{
    MS_U8 u8Return = 0;

    switch( u8DecCmd )
    {
        case AU_SIF_CMD_SET_STOP:
                HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x01, 0x00);
                break;

        case AU_SIF_CMD_SET_PLAY:
                HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x01, 0x01);
            break;

        case AU_SIF_CMD_ENABLE_HIDEV:
            bHiDev_Flag = (MS_BOOL)comm_arg1;

            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
                break;

            u8Return = HAL_SIF_ReadReg(AU_CMD_STANDARD);
            if((u8Return&0xF0)<=SIF_MODE_HIDEV)
            {
                u8Return = u8Return & 0x0F;
                if(bHiDev_Flag)
                {
                    HAL_SIF_WriteByte(AU_CMD_STANDARD, u8Return|SIF_MODE_HIDEV);
                    HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x01, 0x01);  //Reset FC tracking
                    AUDIO_DELAY1MS(5);
                    HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x01, 0x00);
                    HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x02, 0x02);  //Enable FC tracking
                    AUDIO_DELAY1MS(1);
                }
                else
                {
                    HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x02, 0x00);  //Disable FC tracking
                    AUDIO_DELAY1MS(1);
                    HAL_SIF_WriteByte(AU_CMD_STANDARD, u8Return|SIF_MODE_MONO);
                }
            }
            break;

        case AU_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL:
            hidev_filter_bandwidth_level= comm_arg1;
            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, hidev_filter_bandwidth_level);
            break;

        case AU_SIF_CMD_RESET_FC_TRACKING:
            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
                break;

            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x01, 0x01);
            AUDIO_DELAY1MS(5);
            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x01, 0x00);
            break;

        case AU_SIF_CMD_ENABLE_FC_TRACKING:
            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
                break;

            if ((MS_BOOL)comm_arg1 == TRUE) {
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x02, 0x02);
                AUDIO_DELAY1MS(1);
            }
            else {
                HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x02, 0x00);
                AUDIO_DELAY1MS(1);
            }
            break;

        case AU_SIF_CMD_SET_ADC_FROM_VIF_PATH:
            if (bADCFromVifPathSupported == TRUE) {
                if ((MS_BOOL)comm_arg1 == TRUE) {
                    if (bADCFromVifPathEnabled == FALSE)
                    {
                        bADCFromVifPathEnabled = TRUE;
                        sif_clock = AUD_SIF_43M;
                    }
                }
                else {
                    if (bADCFromVifPathEnabled == TRUE)
                    {
                        bADCFromVifPathEnabled = FALSE;
                        sif_clock = AUD_SIF_48M;
                    }
                }
            }
            break;

        case AU_SIF_CMD_SET_SIFFIFO_MODE:
            // 2-stage or 4-stage SIF FIFO Mode.
            break;

        case AU_SIF_CMD_ENABLE_CHANNEL:
            if ((MS_BOOL)comm_arg1 == TRUE)
            {
                if (HAL_SIF_SendCmd(AU_SIF_CMD_GET_ADC_FROM_VIF_PATH, 0, 0) == TRUE) // VIF mode
                {
                    HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x70, 0x00);

                    if (sif_clock == AUD_SIF_42M)
                    {
                        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x20); // SIF CLK: 42MHZ
                    }
                    else if (sif_clock == AUD_SIF_43M)
                    {
                        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x00); // SIF CLK: 43MHZ
                    }
                    else if (sif_clock == AUD_SIF_44M)
                    {
                        HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x30); // SIF CLK: 44MHZ
                    }
                }
                else // SIF mode
                {
                    HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x70, 0x40); // SIF CLK: 48MHZ
                }
            }
            break;

        case AU_SIF_CMD_ENABLE_AUTO_MUTE:
            gAutoMute= (MS_BOOL)comm_arg1;
            HAL_SIF_WriteMaskByte(AU_CMD_MODE1, 0x80, ((MS_U8)gAutoMute<<7));
            break;

        case AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE:
            gAutoSoundmode= (MS_BOOL)comm_arg1;
            break;

        case AU_SIF_CMD_ENABLE_BEEPER_FUNCTION:
            beeper_enable = (MS_BOOL)comm_arg1;
            if(beeper_enable)
                HAL_SIF_WriteMaskByte(AU_CMD_TONE, 0x80, 0x80);
            else
                HAL_SIF_WriteMaskByte(AU_CMD_TONE, 0x80, 0x00);

            break;

        case AU_SIF_CMD_SET_BEEPER_TONE:
            // tone = comm_arg1;
            // volume = comm_arg2;
            ///////////////////////////////////////////////////////////////////////////////////
            //Volume control.
            //Gain setting = 0db _ VOL*L0 dB (0db ~ -114db)
            //VOL = 0 ~ 11 (+12~+1db)
            //VOL = 12 (0db)
            //VOL = 13 ~ 126 (-1 ~ -114db)
            //VOL = 127, mute
            ///////////////////////////////////////////////////////////////////////////////////
            //REG_2D2E[6:0]: beeper tone frequency (maximum 32 step(4KHz))
            //0x01: 125Hz
            //0x02: 250Hz
            //...
            //0x08: 1kHz
            //...
            //0x11: 2kHz
            //...
            //0x19: 3kHz
            //...
            //0x21: 4kHz
            ///////////////////////////////////////////////////////////////////////////////////
            HAL_SIF_WriteMaskByte(AU_CMD_TONE, 0x7F, comm_arg1);
            break;

        case AU_SIF_CMD_SET_STD:
            HAL_SIF_WriteMaskByte(AU_CMD_STANDARD, 0x0f, comm_arg1);
            break;

        case AU_SIF_CMD_SET_SUB_CARRIER_STD:
            HAL_SIF_WriteByte(AU_CMD_STANDARD, comm_arg1);
            break;

        case AU_SIF_CMD_SET_CARRIER_DEBOUNCE:
            HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x80, comm_arg1);
            break;

        case AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER:
            HAL_SIF_WriteMaskByte(REG_AUDIO_INPUT_CFG , 0x04, comm_arg2);
            break;
#if 1
        case AU_SIF_CMD_ENABLE_AGC:
            if ((MS_BOOL)comm_arg1 == TRUE) {
                HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x01);
                AUDIO_DELAY1MS(3);
            }
            else {
                HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x01, 0x00);
                AUDIO_DELAY1MS(3);
            }
            break;

        case AU_SIF_CMD_RESET_AGC:
            HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x02);
            AUDIO_DELAY1MS(3);
            HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x02, 0x00);
            AUDIO_DELAY1MS(3);
            break;

        case AU_SIF_CMD_STANDARD_RESET:
            HAL_SIF_WriteByte(AU_CMD_STANDARD, 0x00);  //Clear SIF Carrier1 bit
            HAL_SIF_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x02, 0x00);  //Disable FC tracking
            AUDIO_DELAY1MS(10);
            break;
#endif
        case AU_SIF_CMD_DK123_MONITOR:
            if ((MS_BOOL)comm_arg1 == TRUE) {
                HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x08, 0x08);
                AUDIO_DELAY1MS(3);
            }
            else {
                HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x08, 0x00);
                AUDIO_DELAY1MS(3);
            }
            break;

        case AU_SIF_CMD_DETECT_MAIN_STD_ONLY:
            bDetect_Main_Std_Only_Flag = (MS_BOOL)comm_arg1;
            break;

        // GET INFO
        case AU_SIF_CMD_GET_HIDEV_ENABLE:
            u8Return = (MS_U8)bHiDev_Flag;
            break;

        case AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL:
            u8Return = (MS_U8)hidev_filter_bandwidth_level;
            break;

        case AU_SIF_CMD_GET_ADC_FROM_VIF_PATH:
            u8Return = (MS_U8)bADCFromVifPathEnabled;
            break;

        case AU_SIF_CMD_GET_FM_A2_MODE_STATUS:
            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
            {
                DBG_SIF(printf("DSP type not palsum\n"));
                break;
            }
            u8Return = HAL_SIF_ReadReg(AU_STATUS_MODE1);
            break;

        case AU_SIF_CMD_GET_BTSC_MODE_STATUS:
            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_BTSC) {
                DBG_SIF(printf("DSP type not btsc\n"));
                break;
            }
            u8Return = HAL_SIF_ReadReg(AU_STATUS_MODE1);
            break;

        case AU_SIF_CMD_GET_NICAM_MODE_STATUS:
            if ((HAL_SIF_GetDspCodeType()) != AU_SIF_PALSUM)
            {
                DBG_SIF(printf("DSP type not palsum\n"));
                break;
            }
            u8Return = HAL_SIF_ReadReg(AU_STATUS_MODE2);
            break;

        case AU_SIF_CMD_GET_STANDARD_STATUS:
            if ((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM)
            {
                u8Return = HAL_SIF_ReadReg(AU_STATUS_STANDARD);
            }
            else if ((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC)
            {
                u8Return = AU_SYS_M_BTSC;
            }
            else
            {
                DBG_SIF(printf("DSP type not SIF \n"));
                return FALSE;
            }
            break;
        case AU_SIF_CMD_GET_STANDARD_CMD:
            u8Return = HAL_SIF_ReadReg(AU_CMD_STANDARD);
            break;

        case AU_SIF_CMD_GET_NSR_CMD:
            HAL_SIF_WriteMaskByte(AU_CMD_DBG_CMD, 0xFF, 0x3C);
            HAL_SIF_WriteMaskByte(AU_CMD_DBG_DATA_H, 0xFF, 0x00);
            HAL_SIF_WriteMaskByte(AU_CMD_DBG_DATA_M, 0xFF, 0x00);
            HAL_SIF_WriteMaskByte(AU_CMD_DBG_DATA_L, 0xFF, 0x00);
            u8Return = HAL_SIF_ReadReg(AU_STATUS_DBG_H);
            break;

        case AU_SIF_CMD_GET_AMP_CMD:
            HAL_SIF_WriteMaskByte(AU_CMD_DBG_CMD, 0xFF, 0x30);
            u8Return = HAL_SIF_ReadReg(AU_STATUS_DBG_H);
            u8Return = HAL_SIF_ReadReg(AU_STATUS_DBG_M);
            break;

        case AU_SIF_CMD_GET_VIF_PATCH_ENABLE:
            u8Return = (MS_U8)vif_patch_flag;
            break;

        case AU_SIF_CMD_SET_SIF_SHIFT_CLK:
            if (HAL_SIF_SendCmd(AU_SIF_CMD_GET_ADC_FROM_VIF_PATH, 0, 0) == TRUE) // VIF mode
            {
                HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x70, 0x00);

                if (comm_arg1 == 0x00)
                {
                    sif_clock = AUD_SIF_42M;
                    HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x20); // SIF CLK: 42MHZ
                }
                else if (comm_arg1 == 0x01)
                {
                    sif_clock = AUD_SIF_43M;
                    HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x00); // SIF CLK: 43MHZ
                }
                else if (comm_arg1 == 0x02)
                {
                    sif_clock = AUD_SIF_44M;
                    HAL_SIF_WriteMaskByte(AU_CMD_AGC, 0x30, 0x30); // SIF CLK: 44MHZ
                }
                HAL_SIF_SetSystem(u8sifSystemType);
            }
            break;

        default:
            break;
    }
    return u8Return;
}

MS_U32 HAL_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value)
{
    MS_BOOL  writethd;
    MS_U16 standard_type;

    writethd = HINIBBLE(rw_standard_type) ;
    standard_type = LONIBBLE(rw_standard_type) ;

    if (((HAL_SIF_GetDspCodeType()) == AU_SIF_PALSUM) &&( standard_type >= BTSC_STANDARD))
        return FALSE;
    else if(((HAL_SIF_GetDspCodeType()) == AU_SIF_BTSC) &&( standard_type != BTSC_STANDARD))
        return FALSE;

    if (u16Value > 0x7fff)
    {
        DBG_SIF(printf("Threshold value is too large\n"));
        return FALSE;
    }

    if (writethd)
    {
        HAL_MAD2_Write_DSP_sram(au_pal_sys_threshold_SE[standard_type]+u8Threshold_type,((MS_U32)u16Value)<<8, DSP_MEM_TYPE_PM);
    }
    else
    {
        return (HAL_MAD2_Read_DSP_sram((au_pal_sys_threshold_SE[standard_type]+u8Threshold_type),DSP_MEM_TYPE_PM));
    }

    return TRUE;
}

MS_BOOL HAL_SIF_GetOrginalGain(void)
{
    MS_U8  i;

    if (g_AudioVars2 == NULL)
    {
        DBG_SIF(printf("%s() : invalid g_AudioVars2!\n", __FUNCTION__));
        return FALSE;
    }

    if ((HAL_SIF_GetDspCodeType())== AU_SIF_BTSC)
    {
        for(i = 0; i< sizeof(SIF_PM_GAIN_TBL_BTSC_SE)/(sizeof(MS_U16)*2); i++)
        {
                g_AudioVars2->sif_gain_0[i] = (HAL_MAD2_Read_DSP_sram (SIF_PM_GAIN_TBL_BTSC_SE[i][0],DSP_MEM_TYPE_PM) >> 8);
                g_AudioVars2->sif_shift_0[i] = HAL_MAD2_Read_DSP_sram (SIF_PM_GAIN_TBL_BTSC_SE[i][1],DSP_MEM_TYPE_PM);
        }
    }
    else if ((HAL_SIF_GetDspCodeType())== AU_SIF_PALSUM)
    {
        for(i = 0; i< sizeof(SIF_PM_GAIN_TBL_PAL_SE)/(sizeof(MS_U16)*2); i++)
        {
            g_AudioVars2->sif_gain_0[i] = (HAL_MAD2_Read_DSP_sram (SIF_PM_GAIN_TBL_PAL_SE[i][0],DSP_MEM_TYPE_PM) >> 8);
            g_AudioVars2->sif_shift_0[i] = HAL_MAD2_Read_DSP_sram (SIF_PM_GAIN_TBL_PAL_SE[i][1],DSP_MEM_TYPE_PM);
        }
    }
    else
    {
        DBG_SIF(printf("\r\n get original gain DSP type error!! \r\n"));
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_SIF_SetPrescale(MS_U8 u8Type, MS_S32 db_value)
{
    MS_U8  i, i_min, i_max;
    MS_U8  u8DspCodeType;
    MS_S32 normalized_db;
    MS_U16 sif_shift, sif_gain, shift_value;
    MS_U32 gain;

    u8DspCodeType=HAL_SIF_GetDspCodeType();
    if (db_value == 99)
    {
        if (u8DspCodeType == AU_SIF_BTSC)
        {
            HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_BTSC_SE[u8Type][0],0, DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_BTSC_SE[u8Type][1],0, DSP_MEM_TYPE_PM);
        }
        else if (u8DspCodeType == AU_SIF_PALSUM)
        {
            HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_PAL_SE[u8Type][0],0, DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_PAL_SE[u8Type][1],0, DSP_MEM_TYPE_PM);
        }
        else
        {
            DBG_SIF(printf("\r\n set prescale DSP type error!! %x \r\n",u8DspCodeType));
            return FALSE;
        }
        return TRUE;
    }

    if (g_AudioVars2 != NULL)
    {
        sif_gain = g_AudioVars2->sif_gain_0[u8Type];
        sif_shift = g_AudioVars2->sif_shift_0[u8Type];
    }
    else
    {
        sif_gain = 0x7FFF;
        sif_shift = 0x0000;
    }
    //printf("\r\n dB_value *4 is %d  \r\n ",(MS_U16)db_value);
    //printf("\r\n original_sif_gain is %x",(MS_U16)g_AudioVars2->sif_gain);
    //printf("\r\n original_sif_shift is %x",(MS_U16)g_AudioVars2->sif_shift);

    //read user DB setting
    i_min = 8/PRESCALE_STEP_ONE_DB-1;
    i_max = sizeof(PRESCALE_STEP_TBL)/sizeof(MS_U16);
    //caculate new gain & shift
    if(db_value>(18*PRESCALE_STEP_ONE_DB))
    {
        shift_value = 5;// 3;
        db_value = 0;
    }
    else if (db_value > (12*PRESCALE_STEP_ONE_DB))
    {
        shift_value = 5;// 3;
        db_value = db_value - (18*PRESCALE_STEP_ONE_DB);
    }
    else if (db_value > (6*PRESCALE_STEP_ONE_DB))
    {
        shift_value = 4;// 2;
        db_value = db_value - (12*PRESCALE_STEP_ONE_DB);
    }
    else if (db_value > 0 )
    {
        shift_value = 3;// 1;
        db_value = db_value - (6*PRESCALE_STEP_ONE_DB);
    }
    else if (db_value > (-6*PRESCALE_STEP_ONE_DB) )
    {
        shift_value = 2;//0;
    }
    else if (db_value > (-12*PRESCALE_STEP_ONE_DB) )
    {
        shift_value = 1;//-1;
        db_value = db_value + (6*PRESCALE_STEP_ONE_DB);
    }
    else
    {
        shift_value = 0;//-2;
        db_value = 0;
    }
    gain = 0x7FFF;
    normalized_db = -db_value;

    for(i=i_min;i<i_max;i++)
    {
        if(normalized_db & (1<<(i-i_min)))
        {
            gain = ((MS_U32)PRESCALE_STEP_TBL[i])*((MS_U32)gain);
            gain = gain>>15;
        }
    }
    // check for
    while ((sif_shift+shift_value) <2)
    {
        gain = (0x4000*gain);
        gain = gain>>15;
        shift_value++;
    }

    gain = (gain*((MS_U32)sif_gain))>>15;
    sif_gain = (MS_U16) gain;
    sif_shift = sif_shift+shift_value-2;
    //set new gain & shift to PM
    if (u8DspCodeType   == AU_SIF_BTSC)
    {
        HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_BTSC_SE[u8Type][0], (((MS_U32)sif_gain)<<8), DSP_MEM_TYPE_PM);
        HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_BTSC_SE[u8Type][1], (MS_U32)sif_shift, DSP_MEM_TYPE_PM);
    }
    else if (u8DspCodeType == AU_SIF_PALSUM)
    {
        HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_PAL_SE[u8Type][0], (((MS_U32)sif_gain)<<8), DSP_MEM_TYPE_PM);
        HAL_MAD2_Write_DSP_sram(SIF_PM_GAIN_TBL_PAL_SE[u8Type][1], (MS_U32)sif_shift, DSP_MEM_TYPE_PM);
    }
    else
    {
         DBG_SIF(printf("\r\n set prescale DSP type error!! %x \r\n",u8DspCodeType));
         return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_SIF_ReLoadCode(AUDIO_DSP_CODE_TYPE u8Type)
{
    if ((HAL_SIF_GetDspCodeType())== u8Type)
            return TRUE;

    DBG_SIF(printf("MDrv_SIF_ReLoadCode(u8Type= SIF(%x))\r\n",u8Type));

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, FALSE, NULL); // REset SIF
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER, FALSE, 0x00); // disable SIF Audio sythesizer & disable DVB fix-sync mode

    if (HAL_SIF_GetDspType() == DSP_SE)
    {
        HAL_SIF_AbsWriteMaskByte(0x112CC9, 0x20, 0x20);   //0x112CC8[13] = 1 to IDMA port selection to SE DSP

        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);
        // Reset MAD module
        HAL_MAD2_DisEn_MIUREQ();
        if(HAL_AUDIO_Alg2ReloadCode(u8Type) == TRUE)
        {
            HAL_SIF_SetDspCodeType(u8Type);
        }
        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    }

    HAL_SIF_GetOrginalGain();

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, NULL);
    // Enable SIF Audio synthesizer here to prevent reload unstable noise.
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_SIF_SYNTHESIZER, TRUE, 0x04); // enable SIF Audio sythesizer

    if (u8Type == AU_SIF_PALSUM)
    {
        HAL_SIF_SendCmd(AU_SIF_CMD_SET_CARRIER_DEBOUNCE, 0x80, NULL);
        HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, TRUE, NULL);
        HAL_SIF_SendCmd(AU_SIF_CMD_DK123_MONITOR, TRUE, NULL);
    }

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_MUTE, FALSE, NULL);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE, TRUE, NULL);

    return TRUE;
}

MS_BOOL HAL_SIF_SetSystem(AUDIO_DSP_CODE_TYPE u8SystemType)
{
    u8sifSystemType = u8SystemType;
    HAL_SIF_ReLoadCode(u8sifSystemType);
    HAL_SIF_TriggerSifPLL();
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, NULL);
    return TRUE;
}

void HAL_SIF_BackupMailbox(void)
{
    MS_U8 i = 0;

    for(i=0;i<sizeof(SIF_MailBoxArray)/sizeof(MS_U16);i++)
    {
        SIF_MailBoxArray[i]=HAL_SIF_ReadReg(AU_CMD_STANDARD+2*i);
    }
}

void HAL_SIF_RestoreMailbox(void)
{
    MS_U8 i = 0;

    for(i=0;i<sizeof(SIF_MailBoxArray)/sizeof(MS_U16);i++)
    {
        HAL_SIF_WriteReg(AU_CMD_STANDARD+2*i, SIF_MailBoxArray[i]);
    }
}

MS_BOOL HAL_SIF_CheckDecSys(En_DVB_decSystemType enDecSystem)
{
    AUDIO_DSP_CODE_TYPE dspCodeType1, dspCodeType2;
    dspCodeType1 = HAL_SIF_GetDspCodeType();
    dspCodeType2 = HAL_AUDIO_Convert_EnDVBdecSystemType_to_AudioDspCodeType(enDecSystem);

    if (dspCodeType1 == dspCodeType2)
        return TRUE;
    else
        return FALSE;
}