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
#include "ULog.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "regCHIP.h"
#include "drvAUDIO_if.h"
#include "../../../drv/audio/internal/drvAUDIO_internal.h"
#include "halAUDIO.h"

// Internal Definition
#include "halSOUND.h"
#include "regAUDIO.h"
#include "halMAD2.h"
#include "halADVAUDIO.h"
#include "audio_comm2.h"

extern MS_S32  _s32AUDIOMutexIDMA;

#define  SrsDmAddr  (SRS_TSXT_base_addr>>8)
#define  TshdDmAddr  0x1C

//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
#define OFFSETOF(type,member) ((size_t)(&((type *)0)->member))
#define SIZEOF(type,member) ((size_t) sizeof(((type *)0)->member))



/** Total Sonics presets */
DBXTV_DM TotSonDM[1];
DBXTV_PM TotSonPM[1];
DBXTV_DM TotVolDM[2];
DBXTV_PM TotVolPM[2];
DBXTV_DM TotSurDM[1];
DBXTV_PM TotSurPM[1];
static AUDIO_OUTPUT_TYPE DBX_PATH = AUDIO_I2S_OUTPUT;

#endif

const MS_U8 Gain_TBL[][3]=
{
    {0x7F, 0xFF, 0xFF},  //0dB
    {0x72, 0x14, 0x81},  //-1dB
    {0x65, 0xAC, 0x8B},  //-2dB
    {0x5A, 0x9D, 0xF6},  //-3dB
    {0x50, 0xC3, 0x35},  //-4dB
    {0x47, 0xFA, 0xCC},  //-5dB
    {0x40, 0x26, 0xE6},  //-6dB
    {0x39, 0x2C, 0xED},  //-7dB
    {0x32, 0xF5, 0x2C},  //-8dB
    {0x2D, 0x6A, 0x86},  //-9dB
    {0x28, 0x7A, 0x26},  //-10dB
    {0x24, 0x13, 0x46},  //-11dB
    {0x20, 0x26, 0xF2},  //-12dB
    {0x1C, 0xA7, 0xD7},  //-13dB
    {0x19, 0x8A, 0x13},  //-14dB
    {0x16, 0xC3, 0x10},  //-15dB
    {0x14, 0x49, 0x60},  //-16dB
};

const MS_U8 Gain_TBL_1[][3]=
{
    {0x40, 0x00, 0x00},  //0dB
    {0x47, 0xCF, 0x25},  //1dB
    {0x50, 0x92, 0x3B},  //2dB
    {0x5A, 0x67, 0x03},  //3dB
    {0x65, 0x6E, 0xE3},  //4dB
    {0x72, 0xCF, 0x53},  //5dB
    {0x7F, 0xFF, 0xFF},  //6dB
};

const MS_U8 Gain_HDTBL [][3]=
{
    {0x7F, 0xFF, 0xFF},  //1.0
    {0x73, 0x33, 0x32},  //0.9
    {0x66, 0x66, 0x65},  //0.8
    {0x59, 0x99, 0x98},  //0.7
    {0x4C, 0xCC, 0xCC},  //0.6
    {0x3F, 0xFF, 0xFF},  //0.5
    {0x33, 0x33, 0x32},  //0.4
    {0x26, 0x66, 0x66},  //0.3
    {0x19, 0x99, 0x99},  //0.2
    {0x0C, 0xCC, 0xCC},  //0.1
    {0x0B, 0x85, 0x1E},  //0.09
    {0x0A, 0x3D, 0x70},  //0.08
    {0x08, 0xF5, 0xC2},  //0.07
    {0x07, 0xAE, 0x14},  //0.06
    {0x06, 0x66, 0x66},  //0.05
    {0x05, 0x1E, 0xB8},  //0.04
    {0x03, 0xD7, 0x0A},  //0.03
    {0x02, 0x8F, 0x5C},  //0.02
    {0x01, 0x47, 0xAE},  //0.01
    {0x00, 0x00, 0x00},  //0
    {0x00, 0x00, 0x00},  //0
};


const MS_U32 AEQ_CONFIG_1_IIRCoef [4][6]=
{
  {0x000001,  0x7e5896,  0x81a76a,  0x000000,  0x7cb12c,  0x000000}, //IIR1_iwl, b0, b1, b2, a1, a2
  {0x000001,  0x486cff,  0x486cff,  0x000000,  0xef2603,  0x000000}, //IIR2_iwl, b0, b1, b2, a1, a2
  {0x000002,  0x436dc0,  0x828e12,  0x3a4917,  0x7d71ee,  0xc2492a}, //IIR3_iwl, b0, b1, b2, a1, a2
  {0x000002,  0x400000,  0x000000,  0x000000,  0x000000,  0x000000}, //IIR_Gain_iwl, Iir_Gain
};

const MS_U32 AEQ_CONFIG_2_IIRCoef [5][6]=
{
  {0x000002,  0x0d5a92,  0xe562a0,  0x0d42e6,  0x7fe2fb,  0xc01c92}, //IIR1_iwl, b0, b1, b2, a1, a2
  {0x000002,  0x3ed201,  0x837d2e,  0x3dbbbe,  0x76b8de,  0xc93cb7},
  {0x000002,  0x345006,  0x9c9656,  0x2ff53f,  0x7d8917,  0xc1619a},
  {0x000002,  0x27da9a,  0xc50eba,  0x227103,  0x3abfef,  0xf5f294},
  {0x000004,  0x5c447d,  0x000000,  0x000000,  0x000000,  0x000000}, //IIR_Gain_iwl, Iir_Gain
};

const MS_U32 AEQ_CONFIG_2_FIRCoef [60]=
{
   0x000000,  0x383465,  0xd23472,  0x2667ae,
   0x007bc1,  0x084263,  0x076a3a,  0x0c00c3,
   0x077e09,  0x06f814,  0x068613,  0x0708f6,
   0x050e64,  0x04c12e,  0x031c35,  0x040614,
   0x033057,  0x0235f0,  0x028256,  0x02df74,
   0x01b863,  0x029d89,  0x02a018,  0x019c99,
   0x0221f7,  0x02e109,  0x01262b,  0x018863,
   0x02398d,  0x00edae,  0x00c384,  0x01a60a,
   0x006622,  0x00607d,  0x013110,  0x0051dc,
   0xfff183,  0x01075a,  0x003eb7,  0xffe4e9,
   0x00a85d,  0x002c4e,  0xffb223,  0x007346,
   0xffe1b4,  0xff83b8,  0x001dba,  0xffd721,
   0xff4ef1,  0x00009c,  0xffc45d,  0xff7179,
   0xfff51b,  0xffe738,  0xff812a,  0x001810,
   0xfff7c6,  0xfe9391,  0x000002,  0x6aafd7,
};

const MS_U32 AEQ_CONFIG_3_FIRCoef [53]=
{
   0x000002,  0x5dce15,  0xfe08e9,  0xddb769,
   0x012239,  0xfb6c99,  0x06006c,  0x01bcfb,
   0xf95c41,  0x0a29a5,  0x00ae3d,  0xf88b62,
   0xfc4f53,  0x06e89e,  0x05e41c,  0x009fe6,
   0x044f03,  0xf91550,  0xfc337c,  0xfcea30,
   0xf8223f,  0x03536d,  0x043ba4,  0x063dd8,
   0x01a51f,  0xfb7954,  0xfedcc6,  0xff9f61,
   0x0553d7,  0x029a62,  0x00523e,  0xfeb10e,
   0xf988d8,  0xfe21c2,  0xfe8bc1,  0x01455d,
   0x02032b,  0xff90c6,  0x023bab,  0xfffd1c,
   0x014cf2,  0x00a0b3,  0xff2edc,  0x002839,
   0xfe1caa,  0xffef2b,  0xfe4a32,  0xfe4b2f,
   0x006c5d,  0x01378f,  0x03da55,  0x000002,
   0x400000,
};

const MS_U32 AEQ_CONFIG_3_IIRCoef [2][6]=
{
  {0x000002,  0x400000,  0x000000,  0x000000,  0x000000,  0x000000}, //IIR1_iwl, b0, b1, b2, a1, a2        // 48k
  {0x000002,  0x400000,  0x000000,  0x000000,  0x000000,  0x000000}, //IIR_Gain_iwl, Iir_Gain
};
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_AseSetBinAddress()
/// @brief \b Function \b Description:  This function is used to assign Bin address of advanced sound effect
/// @param <IN>        \b index    : type index
/// @param <IN>        \b BinAddr    : address of Bin file
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    : AseBinAddress[index], address table
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr)
{
    u8Index = u8Index;
    BinAddr = BinAddr;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VDS_Enable()
/// @brief \b Function \b Description:  This function is used to set VDS
/// @param <IN>        \b u8enVDS    : 1: enable, 0: disable
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_VDS_Enable(MS_U8 u8enVDS)
{
#if(AUDIO_USE_SOUND_EFFECT_VDS==1)

    MS_U8 u8Volume_backup;

    u8Volume_backup = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);

    HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME,0x80,0x80);     // Mute

    if (u8enVDS)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x80,0x80);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x80,0x00);
    }
    AUDIO_DELAY1MS(5);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME,0x80,u8Volume_backup);      // un_Mute

#else
    u8enVDS = u8enVDS;

#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VSPK_Enable()
/// @brief \b Function \b Description:  This function is used to set VSPK
/// @param <IN>        \b u8enVSPK    : 1: enable, 0: disable
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_VSPK_Enable(MS_U8 u8enVSPK)
{

#if(AUDIO_USE_SOUND_EFFECT_VSPK==1)

    MS_U8 u8Volume_backup;

    u8Volume_backup = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME,0x80,0x80);     // Mute

    if (u8enVSPK)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x80,0x80);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x80,0x00);
    }

    AUDIO_DELAY1MS(100);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME,0xFF,u8Volume_backup);     // un_Mute

#else
    u8enVSPK = u8enVSPK;
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_SetTsxtPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-XT
/// @param <IN>        \b u8mode:  parameter mode
/// @param <IN>        \b u8value: parameter value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b u8SRSControl    : store enable bits
////////////////////////////////////////////////////////////////////////////////
MS_U8 u8SRSControl = 0xE0;
void    HAL_SOUND_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{
#if(AUDIO_USE_SOUND_EFFECT_SRS==1)
    switch (u8mode)
    {
        case 0: //TS Gain
              if(u8value > 12) u8value = 12;
              HAL_SOUND_SetPara(SrsDmAddr,0x00,Gain_TBL[u8value][0],Gain_TBL[u8value][1],Gain_TBL[u8value][2]);
              break;
          case 1: //FocusElevation
              if(u8value > 12) u8value = 12;
              HAL_SOUND_SetPara(SrsDmAddr,0x01,Gain_TBL[u8value][0],Gain_TBL[u8value][1],Gain_TBL[u8value][2]);
              break;
          case 2: //TruBassGain
              if(u8value > 12) u8value = 12;
              HAL_SOUND_SetPara(SrsDmAddr,0x02,Gain_TBL[u8value][0],Gain_TBL[u8value][1],Gain_TBL[u8value][2]);
              break;
          case 3: //SpeakSize
              if(u8value > 3) u8value = 3;
              HAL_SOUND_SetPara(SrsDmAddr,0x03,0,0,u8value*12);
              break;
          case 4: //Input mode
              if(u8value > 2) u8value = 2;
              HAL_SOUND_SetPara(SrsDmAddr,0x04,0,0,u8value);
              break;
          case 7: //Output Gain
              if(u8value > 6) u8value = 6;
              HAL_SOUND_SetPara(SrsDmAddr,0x07,Gain_TBL_1[u8value][0],Gain_TBL_1[u8value][1],Gain_TBL_1[u8value][2]);
              break;
      }
#else
    u8mode = u8mode;
    u8value = u8value;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_SetTshdPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-HD
/// @param <IN>        \b u8mode:  parameter mode
/// @param <IN>        \b u8value: parameter value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b u8SRSControl    : store enable bits
////////////////////////////////////////////////////////////////////////////////
#define kSrsTruSurroundHDOutput2_0_0       0x03
#define kSrsTruSurroundHDOutputHeadphone 0x43
#define kSrsTruSurroundHDOutput2_0_1     0x23
#define kSrsTruSurroundHDOutput3_0_0     0x13
#define kSrsTruSurroundHDOutput3_0_1     0x33

MS_U8  mOutputMode_TBL[5]={
    kSrsTruSurroundHDOutput2_0_0,
    kSrsTruSurroundHDOutputHeadphone,
    kSrsTruSurroundHDOutput2_0_1,
    kSrsTruSurroundHDOutput3_0_0,
    kSrsTruSurroundHDOutput3_0_1,
};

MS_U8 u8TSHDontrol = 0xF0;
void  HAL_SOUND_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1 || AUDIO_USE_SOUND_EFFECT_TSHDVIQ==1)
    switch (u8mode)
    {
        case 1: //SRS_EN_BITS
            HAL_SOUND_SetPara(TshdDmAddr,0x01,0,u8value,0);
            break;
        case 2: //mInputMode
            if(u8value > 11) u8value = 11;
            HAL_SOUND_SetPara(TshdDmAddr,0x02,0,0,u8value);
            break;
        case 3: //mOutputMode
            if(u8value > 4) u8value = 4;
            HAL_SOUND_SetPara(TshdDmAddr,0x03,0,0,mOutputMode_TBL[u8value]);
            break;
        case 4: //SpeakerSize
            if(u8value > 3) u8value = 3;
            HAL_SOUND_SetPara(TshdDmAddr,0x04,0,0,u8value);
            break;
        case 5: //TruBassControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x05,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 6: //DefinitionControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x06,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 7: //FOCUSControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x07,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 8: //SurroundLevel
            if(u8value > 10) u8value = 10;
            HAL_SOUND_SetPara(TshdDmAddr,0x08,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 9: //Crossover
            if(u8value > 2) u8value = 2;
            HAL_SOUND_SetPara(TshdDmAddr,0x09,0,0,u8value);
            break;
        case 10: //TruBassSubwooferEnable
            HAL_SOUND_SetPara(TshdDmAddr,0x0A,0,0,u8value);
            break;
        case 11: //TshdCertifyChannel
            HAL_SOUND_SetPara(TshdDmAddr,0x0B,0,0,u8value);
            break;
        case 12: //mInputGain
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x0C,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 13: //WowSpaceControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x0D,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 14: //WowCenterControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x0E,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 15: //WOWHDSRS3DMode
            HAL_SOUND_SetPara(TshdDmAddr,0x0F,0,0,u8value);
            break;
        case 16: //mLimiterControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x10,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 17: //mOutputGain
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x11,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 18: //mBypassGain
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x12,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 19: //mHeadroomGain
            if(u8value > 16) u8value = 16;
            HAL_SOUND_SetPara(TshdDmAddr,0x13,Gain_TBL[u8value][0],Gain_TBL[u8value][1],Gain_TBL[u8value][2]);
            break;
        case 20: //VIQ ReferenceLevel
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x19,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 21: //VIQ mLimiterControl
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x1A,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 22: //HL mLimiterBoost
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x19,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
        case 23: //HL mHardLimit
            if(u8value > 20) u8value = 20;
            HAL_SOUND_SetPara(TshdDmAddr,0x1A,Gain_HDTBL[u8value][0],Gain_HDTBL[u8value][1],Gain_HDTBL[u8value][2]);
            break;
      }
#else
    u8mode = u8mode;
    u8value = u8value;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_Enable()
/// @brief \b Function \b Description:  This function is used to set SRS-XT
/// @param <IN>        \b u8enSRS:  1: enable, 0: disable
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_SRS_Enable(MS_U8 u8enSRS)
{
#if(AUDIO_USE_SOUND_EFFECT_SRS==1)
     if (u8enSRS)
     {
         HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
         HAL_SOUND_SRS_TruBass(1);
         HAL_SOUND_SRS_DC(1);
         HAL_SOUND_SRS_SetTsxtPara(4,2);//PassiveMatrix Mode
         HAL_SOUND_SRS_SetTsxtPara(0,6); //InputGain -6dB
         HAL_SOUND_SRS_SetTsxtPara(1,4); //DC -4dB
         HAL_SOUND_SRS_SetTsxtPara(2,6); //Trubass Gain -6dB
         HAL_SOUND_SRS_SetTsxtPara(3,1); //SpeakSize 150Hz
     }
     else
     {
         HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
     }
#else
    u8enSRS = u8enSRS;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_Enable()
/// @brief \b Function \b Description:  This function is used to set SRS-HD
/// @param <IN>        \b u8enSRS:  1: enable, 0: disable
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_Enable(MS_U8 u8enSRS)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)
    //Set TruSurroundHD
    if (u8enSRS)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
        HAL_SOUND_TSHD_TruBass(1);
        HAL_SOUND_TSHD_Clarity(1);
        HAL_SOUND_TSHD_Definition(1);
        HAL_SOUND_SRS_SetTshdPara(12,7); //TS InputGain       0.3
        HAL_SOUND_SRS_SetTshdPara(7,5); //FocusElevation      0.5
        HAL_SOUND_SRS_SetTshdPara(8,2); //SurroundLevel       0.8
        HAL_SOUND_SRS_SetTshdPara(6,7); //Definition Control  0.3
        HAL_SOUND_SRS_SetTshdPara(5,7); //TruBass Control     0.3
        HAL_SOUND_SRS_SetTshdPara(4,2); //200Hz
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
    }

#else
    u8enSRS = u8enSRS;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_VSPK_SetWMod()
/// @brief \b Function \b Description:  This function is used to set VSPK W Mode
/// @param <IN>        \b u8VSPK_WMode:
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_VSPK_SetWMod(MS_U8 u8VSPK_WMode)
{

#if(AUDIO_USE_SOUND_EFFECT_VSPK==1)
    if (u8VSPK_WMode)
    {   // Wide mod
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x40,0x40);     //  [6]=1, wide mode
    }
    else
    {   // Ref mod
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x40,0x00);     //  [6]=0, reference mode
    }
#else
    u8VSPK_WMode = u8VSPK_WMode;
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_VSPK_SetSMod()
/// @brief \b Function \b Description:  This function is used to set VSPK S Mode
/// @param <IN>        \b u8VSPK_SMode:
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_VSPK_SetSMod(MS_U8 u8VSPK_SMode)
{
#if(AUDIO_USE_SOUND_EFFECT_VSPK==1)

    if (u8VSPK_SMode)
    {   // Music mod
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x20,0x20);     //  [0]=1, music mode
    }
    else
    {   // Movie mod
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0x20,0x00);     //  [0]=0, movie mode
    }
#else
    u8VSPK_SMode = u8VSPK_SMode;
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_BBE_SetBbeMode()
/// @brief \b Function \b Description:  This function is used to set BBE Mode
/// @param <IN>        \b u8EnBBE:
/// @param <OUT>       \b u8BBEMode    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_BBE_SetBbeMode(MS_U8 u8EnBBE, MS_U8 u8BBEMode)
{
#if(AUDIO_USE_SOUND_EFFECT_BBE==1)
    u8EnBBE = u8EnBBE;
    u8BBEMode = u8BBEMode;
#else
    u8EnBBE = u8EnBBE;
    u8BBEMode = u8BBEMode;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetPara()
/// @brief \b Function \b Description:  This function is used to write paramters value into SRAM
/// @param <IN>        \b addr1:  middle byte of address
/// @param <IN>        \b addr2:  low byte of address
/// @param <IN>        \b value1: high byte of value
/// @param <IN>        \b value2: middle byte of value
/// @param <IN>        \b value3: low byte of value
/// @param <OUT>        \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_SetPara(MS_U8 u8Addr1 , MS_U8 u8Addr2, MS_U8 u8Value1, MS_U8 u8Value2, MS_U8 u8Value3)
{
    MS_U16 addr = (((MS_U8)u8Addr1 << 8) | (MS_U8)u8Addr2);
    MS_U32 value = (((MS_U8)u8Value1 << 16) | ((MS_U8)u8Value2 << 8) | (MS_U8)u8Value3);
    HAL_MAD2_Write_DSP_sram(addr, value, DSP_MEM_TYPE_DM);//MDrv_MAD2_Write_DSP_sram(addr, value, DSP_MEM_TYPE_DM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_DC()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-XT Dialog Clarity
/// @param <IN>        \b u8SRSenDC:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_SRS_DC(MS_U8 u8SRSenDC)
{
#if(AUDIO_USE_SOUND_EFFECT_SRS==1)

    if (u8SRSenDC)
        u8SRSControl |= 0x20;
    else
        u8SRSControl &= 0xDF;
    HAL_SOUND_SetPara(SrsDmAddr,0x06,0,u8SRSControl,0);  // [5] = 0, Dialog Clarity disable
#else
    u8SRSenDC = u8SRSenDC;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-XT Trubass
/// @param <IN>        \b u8SRSenTruBass:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_SRS_TruBass(MS_U8 u8SRSenTruBass)
{
#if(AUDIO_USE_SOUND_EFFECT_SRS==1)

    if (u8SRSenTruBass)
        u8SRSControl |= 0x80;
    else
        u8SRSControl &= 0x7F;
    HAL_SOUND_SetPara(SrsDmAddr,0x06,0,u8SRSControl,0);  // [7] = 0, TruBass disable

#else
    u8SRSenTruBass = u8SRSenTruBass;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Trubass
/// @param <IN>        \b u8SRSenTruBass:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenTruBass)
        u8TSHDontrol |= 0x80;
    else
        u8TSHDontrol &= 0x7F;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);
#else
    u8TSHDenTruBass = u8TSHDenTruBass;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param <IN>        \b u8TSHDenDefinition:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenDefinition)
        u8TSHDontrol |= 0x40;
    else
        u8TSHDontrol &= 0xBF;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);

#else
    u8TSHDenDefinition = u8TSHDenDefinition;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_Clarity()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD DC
/// @param <IN>        \b u8TSHDenClarity:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_Clarity(MS_U8 u8TSHDenClarity)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenClarity)
        u8TSHDontrol |= 0x20;
    else
        u8TSHDontrol &= 0xDF;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);

#else
    u8TSHDenClarity = u8TSHDenClarity;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_TruSurr()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD
/// @param <IN>        \b u8TSHDenTruSurr:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_TruSurr(MS_U8 u8TSHDenTruSurr)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenTruSurr)
        u8TSHDontrol |= 0x10;
    else
        u8TSHDontrol &= 0xEF;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);

#else
    u8TSHDenTruSurr = u8TSHDenTruSurr;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_Limiter()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Limiter
/// @param <IN>        \b u8TSHDenLimiter:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_Limiter(MS_U8 u8TSHDenLimiter)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenLimiter)
        u8TSHDontrol |= 0x08;
    else
        u8TSHDontrol &= 0xF7;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);

#else
    u8TSHDenLimiter = u8TSHDenLimiter;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TSHD_SRS3D()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param <IN>        \b u8TSHDenSRS3D:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_TSHD_SRS3D(MS_U8 u8TSHDenSRS3D)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHD==1)

    if (u8TSHDenSRS3D)
        u8TSHDontrol |= 0x04;
    else
        u8TSHDontrol &= 0xFB;
    HAL_SOUND_SetPara(TshdDmAddr,0x81,0,u8TSHDontrol,0);

#else
    u8TSHDenSRS3D = u8TSHDenSRS3D;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_TruVolume_Enable()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param <IN>        \b u8TSHDenTVol:  1: enable, 0: disable
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_TruVolume_Enable(MS_U8 u8TSHDenTVol)
{
#if(AUDIO_USE_SOUND_EFFECT_TSHDVIQ==1)

    if (u8TSHDenTVol)
       HAL_SOUND_SetPara(TshdDmAddr,0x14,0,0,1);
    else
       HAL_SOUND_SetPara(TshdDmAddr,0x14,0,0,0);
#else
   u8TSHDenTVol = u8TSHDenTVol;
#endif
}

MS_U8 Gain_GCF[][3]=
{
    {0x01, 0x00, 0x00},  //high
    {0x01, 0xCC, 0xCC},  //medium
    {0x02, 0x66, 0x66},  //Low
};

MS_U8 ABX_Gain_TBL[][3]=
{
    {0x7F, 0xB2, 0x60},  //  6db
    {0x78, 0x8D, 0xB4},  //5.5db
    {0x71, 0xCF, 0x54},  //  5db
    {0x6B, 0x71, 0x86},  //4.5db
    {0x65, 0x6E, 0xE3},  //  4db
    {0x5F, 0xC2, 0x53},  //3.5db
    {0x5A, 0x67, 0x03},  //  3db
    {0x55, 0x58, 0x6A},  //2.5db
    {0x50, 0x92, 0x3B},  //  2db
    {0x4C, 0x10, 0x6B},  //1.5db
    {0x47, 0xCF, 0x26},  //  1db
    {0x43, 0xCA, 0xD0},  //0.5db
    {0x40, 0x00, 0x00},  //  0db
    {0x3C, 0x6B, 0x7E},  //-0.5db
    {0x39, 0x0A, 0x41},  //  -1db
    {0x35, 0xD9, 0x6B},  //-1.5db
    {0x32, 0xD6, 0x46},  //  -2db
    {0x2F, 0xFE, 0x44},  //-2.5db
    {0x2D, 0x4E, 0xFB},  //  -3db
    {0x2A, 0xC6, 0x25},  //-3.5db
    {0x28, 0x61, 0x9A},  //  -4db
    {0x26, 0x1F, 0x54},  //-4.5db
    {0x23, 0xFD, 0x66},  //  -5db
    {0x21, 0xFA, 0x02},  //-5.5db
    {0x20, 0x13, 0x73},  //  -6db
};

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_ADV_SetPara()
/// @brief \b Function \b Description:  This function is used to set CV level
/// @param <IN>        \b level:
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_ADV_SetPara(MS_U8 u8mode, MS_U8 u8value)
{
#if(AUDIO_USE_SOUND_EFFECT_ADV==1)
    MS_U32 u32value;
    switch (u8mode)
    {
        case 1: //ADV
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable ADV\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x80);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable ADV\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x00);
            }
            break;
        case 2: //Dyn Vol
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable Dynamic Volume\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x40);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable Dynamic Volume\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x00);
            }
            break;
        case 3: //Dyn EQ
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable Dynamic EQ\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x20);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable Dynamic EQ\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x00);
            }
            break;
        case 4: //sampling rate
            break;
        case 5: //compression mode
         /*   if(u8value == 0)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Heavy Mode\r\n");
            else if(u8value == 1)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Medium Mode\r\n");
            else if(u8value == 2)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Low Mode\r\n"); */
            HAL_SOUND_SetPara(0x28,0x81,Gain_GCF[u8value][0],Gain_GCF[u8value][1],Gain_GCF[u8value][2]);
            break;
        case 6: //GC
            u32value = 0x010000 + ((MS_U32)u8value)*6553;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "GC:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x81,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 7: //EQ Offset
            u32value = 40*65536 - ((MS_U32)u8value)*65536;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "EQ_Offset:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x82,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 8: //VolSetting
            u32value = 10*65536 - ((MS_U32)u8value)*65536;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "VolSetting:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x80,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 9: //Main Vol
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Main Vol -%1ddB\r\n",u8value);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0xff, 0x0C + u8value);
            break;

        case 10: //ABX
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable ABX\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x10);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable ABX\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x00);
            }
            break;
        case 11: //ABX gWet
            if(u8value > 24) u8value = 24;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "ABX gWet %1d\r\n",u8value);
            HAL_SOUND_SetPara(0x28,0x83,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 12: //ABX gDry
            if(u8value > 24) u8value = 24;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "ABX gDry %1d\r\n",u8value);
            HAL_SOUND_SetPara(0x28,0x84,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 13: //ABX FILSET
            if(u8value > 1) u8value = 1;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "FILSET %1d\r\n",u8value+1);
            HAL_SOUND_SetPara(0x28,0x85,0,0,u8value);
            break;
    }
#else
    u8mode = u8mode;
    u8value = u8value;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SRS_APOLLO_SetPara()
/// @brief \b Function \b Description:  This function is used to set SRS_APOLLO Para
/// @param <IN>        \b level:
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
/// SRS_EN_BITS[7]<15> All
/// SRS_EN_BITS[6]<14> TSHD
/// SRS_EN_BITS[5]<13> dialog clarity
/// SRS_EN_BITS[4]<12> Definition
/// SRS_EN_BITS[3]<11> Trubass
/// SRS_EN_BITS[2]<10> TruVolume
/// SRS_EN_BITS[1]<9> HardLimiter
/// SRS_EN_BITS[0]<8> HPF
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_SRS_APOLLO_SetPara(MS_U8 u8mode, MS_U32 u32param, MS_U16 u16value2)
{
    MS_U64 FXP24Value;
    MS_U32 FIRCoef_index;
    MS_U32 u32value,u32value2, u32value3, u32value4, u32value5, u32value8;
    MS_U16 Q24H, Q24L, Q24H2, Q24L2, Q24H3, Q24L3, Q24H4, Q24L4, Q24H5, Q24L5, Q24H8, Q24L8, FIRCoef_addr;
    MS_BOOL bEnable=FALSE;

    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO,  "HAL_SOUND_APOLLO_SetPara: %x %x %x\r\n",(int)u8mode,(int)u32param, (int)u16value2);
    bEnable=(MS_BOOL)u32param;

    if(u16value2 == 0)//for previous user, u16 parameter
        FXP24Value = (8388607*(MS_U64)u32param)/1000;
    else//for u32 user, fix point 24 bit format
        FXP24Value = u32param;

    u32param_normalize(u32value, FXP24Value, 0, Q24H, Q24L);//no normalize
    u32param_normalize(u32value2, FXP24Value, 2, Q24H2, Q24L2);//normalize 4
    u32param_normalize(u32value3, FXP24Value, 3, Q24H3, Q24L3);//normalize 8
    u32param_normalize(u32value4, FXP24Value, 4, Q24H4, Q24L4);//normalize 16
    u32param_normalize(u32value5, FXP24Value, 5, Q24H5, Q24L5);//normalize 32
    u32param_normalize(u32value8, FXP24Value, 8, Q24H8, Q24L8);//normalize 256

    switch (u8mode)
    {
        /////////////////////////////////////////////////PureSound Control Start///////////////////////////////////////////////////////////
        case AU_SRS_PURESOUND_HL_EN:
            if(u32param)
            {
                HAL_AUDIO_WriteMaskByte(0x2D43,0x02,0x02);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2D43,0x02,0x00);
            }
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_EN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_HL_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlInputGain_addr, u32value2, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_INPUT_GAIN %u===@@@\r\n",u8mode,u32value2);
            break;

        case AU_SRS_PURESOUND_HL_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlOutputGain_addr, u32value2, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value2);
            break;

        case AU_SRS_PURESOUND_HL_BYPASS_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlBypassGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_BYPASS_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_HL_LIMITERBOOST:
            if( u16value2 == 0)
            {
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlLimiterboost_addr, u32value5, DSP_MEM_TYPE_DM);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_LIMITERBOOST %u===@@@\r\n",u8mode,u32value5);
            }
            else
            {
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlLimiterboost_addr, FXP24Value, DSP_MEM_TYPE_DM);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_LIMITERBOOST %llu===@@@\r\n",u8mode,FXP24Value);
            }
            break;

        case AU_SRS_PURESOUND_HL_HARDLIMIT:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlHardLimit_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_HARDLIMIT %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_HL_DELAYLEN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlDelaylen_addr, u32param, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_DELAYLEN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_AEQ_EN:
            if(u32param)
            {
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x10);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x00);
            }
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_EN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_AEQ_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqInputGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_INPUT_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqOutputGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_AEQ_BYPASS_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqBypassGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_BYPASS_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_FILTER_CONFIG:
            if(u32param == 1)
            {
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x00);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1_iwl_addr, AEQ_CONFIG_1_IIRCoef[0][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2_iwl_addr, AEQ_CONFIG_1_IIRCoef[1][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3_iwl_addr, AEQ_CONFIG_1_IIRCoef[2][0], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr, AEQ_CONFIG_1_IIRCoef[0][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+1, AEQ_CONFIG_1_IIRCoef[0][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+2, AEQ_CONFIG_1_IIRCoef[0][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+3, AEQ_CONFIG_1_IIRCoef[0][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+4, AEQ_CONFIG_1_IIRCoef[0][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr, AEQ_CONFIG_1_IIRCoef[1][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+1, AEQ_CONFIG_1_IIRCoef[1][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+2, AEQ_CONFIG_1_IIRCoef[1][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+3, AEQ_CONFIG_1_IIRCoef[1][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+4, AEQ_CONFIG_1_IIRCoef[1][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr, AEQ_CONFIG_1_IIRCoef[2][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+1, AEQ_CONFIG_1_IIRCoef[2][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+2, AEQ_CONFIG_1_IIRCoef[2][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+3, AEQ_CONFIG_1_IIRCoef[2][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+4, AEQ_CONFIG_1_IIRCoef[2][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_iwl_addr, AEQ_CONFIG_1_IIRCoef[3][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_addr, AEQ_CONFIG_1_IIRCoef[3][1], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_NumOfTaps_addr, 0, DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_NumOfSections_addr, 3, DSP_MEM_TYPE_PM);
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x10);
            }
            else if(u32param == 2 || u32param == 0 )
            {
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x00);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1_iwl_addr, AEQ_CONFIG_2_IIRCoef[0][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2_iwl_addr, AEQ_CONFIG_2_IIRCoef[1][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3_iwl_addr, AEQ_CONFIG_2_IIRCoef[2][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4_iwl_addr, AEQ_CONFIG_2_IIRCoef[3][0], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr, AEQ_CONFIG_2_IIRCoef[0][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+1, AEQ_CONFIG_2_IIRCoef[0][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+2, AEQ_CONFIG_2_IIRCoef[0][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+3, AEQ_CONFIG_2_IIRCoef[0][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+4, AEQ_CONFIG_2_IIRCoef[0][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr, AEQ_CONFIG_2_IIRCoef[1][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+1, AEQ_CONFIG_2_IIRCoef[1][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+2, AEQ_CONFIG_2_IIRCoef[1][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+3, AEQ_CONFIG_2_IIRCoef[1][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir2Coefs_addr+4, AEQ_CONFIG_2_IIRCoef[1][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr, AEQ_CONFIG_2_IIRCoef[2][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+1, AEQ_CONFIG_2_IIRCoef[2][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+2, AEQ_CONFIG_2_IIRCoef[2][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+3, AEQ_CONFIG_2_IIRCoef[2][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir3Coefs_addr+4, AEQ_CONFIG_2_IIRCoef[2][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4Coefs_addr, AEQ_CONFIG_2_IIRCoef[3][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4Coefs_addr+1, AEQ_CONFIG_2_IIRCoef[3][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4Coefs_addr+2, AEQ_CONFIG_2_IIRCoef[3][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4Coefs_addr+3, AEQ_CONFIG_2_IIRCoef[3][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir4Coefs_addr+4, AEQ_CONFIG_2_IIRCoef[3][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_iwl_addr, AEQ_CONFIG_2_IIRCoef[4][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_addr, AEQ_CONFIG_2_IIRCoef[4][1], DSP_MEM_TYPE_PM);


                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_iwl_addr, AEQ_CONFIG_2_FIRCoef[0], DSP_MEM_TYPE_PM);

                FIRCoef_addr = SRS_PURESOUND_AeqFirCoefs_addr;
                for(FIRCoef_index = 1 ; FIRCoef_index <= 57 ; FIRCoef_index++ , FIRCoef_addr++)
                {
                    HAL_MAD2_Write_DSP_sram(FIRCoef_addr, AEQ_CONFIG_2_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);
                }
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_Gain_iwl_addr, AEQ_CONFIG_2_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);
                FIRCoef_index++ ;
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_Gain_addr, AEQ_CONFIG_2_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);


                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_NumOfTaps_addr, 57, DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_NumOfSections_addr, 4, DSP_MEM_TYPE_PM);
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x10);
            }
            else if(u32param == 3)
            {
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x00);


                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1_iwl_addr, AEQ_CONFIG_3_IIRCoef[0 ][0], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr, AEQ_CONFIG_3_IIRCoef[0 ][1], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+1, AEQ_CONFIG_3_IIRCoef[0 ][2], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+2, AEQ_CONFIG_3_IIRCoef[0 ][4], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+3, AEQ_CONFIG_3_IIRCoef[0][5], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir1Coefs_addr+4, AEQ_CONFIG_3_IIRCoef[0 ][3], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_iwl_addr, AEQ_CONFIG_3_IIRCoef[1][0], DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_Gain_addr, AEQ_CONFIG_3_IIRCoef[1 ][1], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_iwl_addr, AEQ_CONFIG_3_FIRCoef[0], DSP_MEM_TYPE_PM);

                FIRCoef_addr = SRS_PURESOUND_AeqFirCoefs_addr;
                for(FIRCoef_index = 1 ; FIRCoef_index <= 50 ; FIRCoef_index++ , FIRCoef_addr++)
                {
                    HAL_MAD2_Write_DSP_sram(FIRCoef_addr, AEQ_CONFIG_3_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);
                }
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_Gain_iwl_addr, AEQ_CONFIG_3_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);
                FIRCoef_index++ ;
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_Gain_addr, AEQ_CONFIG_3_FIRCoef[FIRCoef_index], DSP_MEM_TYPE_PM);

                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqFir_NumOfTaps_addr, 50, DSP_MEM_TYPE_PM);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqIir_NumOfSections_addr, 1, DSP_MEM_TYPE_PM);
                HAL_AUDIO_WriteMaskByte(0x2D42,0x10,0x10);
            }
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_FILTER_CONFIG %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_HPF_EN:
            if(u32param)
            {
                HAL_AUDIO_WriteMaskByte(0x2D43,0x01,0x01);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2D43,0x01,0x00);
            }
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HPF_EN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_HPF_FREQUENCY:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mHPFfc_addr, u32param, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HPF_FREQUENCY %d===@@@\r\n",u8mode,u32param);
            break;
#if 0 // Need Kristy to modify puresound
        case AU_SRS_PURESOUND_TBHD_EN:
            if(u32param)
            {
                HAL_AUDIO_WriteMaskByte(0x2D33,0x08,0x08);
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_TBHD_init_flag_addr,1, DSP_MEM_TYPE_DM);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2D33,0x08,0x00);
            }
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_EN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_TBHD_TRUBASS_LEVEL:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_TruBassControl_addr, u32value, DSP_MEM_TYPE_DM);;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_TRUBASS_LEVEL %lu===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_TBHD_SPEAKER_SIZE:
            if(u32param > 2) u32param = 2;
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_SpeakerSize_addr, u32param, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_SPEAKER_SIZE %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_tbc_mLevelIndependent_addr, u32param, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_tbc_mCompressorControl_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL %lu===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_TBHD_MODE:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_tbc_mMode_addr, u32param, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_MODE %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_TBHD_SPEAKER_AUDIO:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr,  TruBassLowPassFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+1,TruBassLowPassFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+2,TruBassLowPassFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+6,  TruBassLowPassFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+7,TruBassLowPassFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+8,TruBassLowPassFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+12,  TruBassLowPassFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+13,TruBassLowPassFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr+14,TruBassLowPassFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);

            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr,  TruBassLowFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+1,TruBassLowFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+2,TruBassLowFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+3,TruBassLowFilterCoef_32k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+8,  TruBassLowFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+9,TruBassLowFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+10,TruBassLowFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+11,TruBassLowFilterCoef_44k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+16,  TruBassLowFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+17,TruBassLowFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+18,TruBassLowFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Audio_addr+19,TruBassLowFilterCoef_48k[u32param][3], DSP_MEM_TYPE_PM);

            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr,  TruBassMidFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+1,TruBassMidFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+2,TruBassMidFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+3,TruBassMidFilterCoef_32k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+8, TruBassMidFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+9,TruBassMidFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+10,TruBassMidFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+11,TruBassMidFilterCoef_44k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+16,  TruBassMidFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+17,TruBassMidFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+18,TruBassMidFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Audio_addr+19,TruBassMidFilterCoef_48k[u8value][3], DSP_MEM_TYPE_PM);


            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_TBHD_init_flag_addr,1, DSP_MEM_TYPE_DM);

            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_SPEAKER_AUDIO %d===@@@\r\n",u8mode,u32param);
            break;

        case AU_SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr,  TruBassLowPassFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+1,TruBassLowPassFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+2,TruBassLowPassFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+6,  TruBassLowPassFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+7,TruBassLowPassFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+8,TruBassLowPassFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+12,  TruBassLowPassFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+13,TruBassLowPassFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr+14,TruBassLowPassFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);

            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr,  TruBassLowFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+1,TruBassLowFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+2,TruBassLowFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+3,TruBassLowFilterCoef_32k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+8,  TruBassLowFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+9,TruBassLowFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+10,TruBassLowFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+11,TruBassLowFilterCoef_44k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+16,  TruBassLowFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+17,TruBassLowFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+18,TruBassLowFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr+19,TruBassLowFilterCoef_48k[u32param][3], DSP_MEM_TYPE_PM);

            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr,  TruBassMidFilterCoef_32k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+1,TruBassMidFilterCoef_32k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+2,TruBassMidFilterCoef_32k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+3,TruBassMidFilterCoef_32k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+8, TruBassMidFilterCoef_44k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+9,TruBassMidFilterCoef_44k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+10,TruBassMidFilterCoef_44k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+11,TruBassMidFilterCoef_44k[u32param][3], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+16,  TruBassMidFilterCoef_48k[u32param][0], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+17,TruBassMidFilterCoef_48k[u32param][1], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+18,TruBassMidFilterCoef_48k[u32param][2], DSP_MEM_TYPE_PM);
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr+19,TruBassMidFilterCoef_48k[u32param][3], DSP_MEM_TYPE_PM);


            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_TBHD_init_flag_addr,1, DSP_MEM_TYPE_DM);

            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS %d===@@@\r\n",u8mode,u32param);
            break;
#endif
        case AU_SRS_PURESOUND_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mInputGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_INPUT_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        case AU_SRS_PURESOUND_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mOutputGain_addr, u32value, DSP_MEM_TYPE_DM);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\r\n@@@== DSP [%d]AU_SRS_PURESOUND_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value);
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO,  "-----HAL_SOUND_SRS_APOLLO_SetPara: Unknown command u8mode=%d u16value=0x%x-----\r\n", u8mode, (unsigned int)(MS_U32)u32param);
            break;

    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_ADSY_SetPara()
/// @brief \b Function \b Description:  This function is used to set CV level
/// @param <IN>        \b level:
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void  HAL_SOUND_ADSY_SetPara(MS_U8 u8mode, MS_U8 u8value)
{
#if(AUDIO_USE_SOUND_EFFECT_ADV==1)
    MS_U32 u32value;
    switch (u8mode)
    {
        case 1: //ADV
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable ADV\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x80);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable ADV\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x00);
            }
            break;
        case 2: //Dyn Vol
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable Dynamic Volume\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x40);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable Dynamic Volume\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x00);
            }
            break;
        case 3: //Dyn EQ
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable Dynamic EQ\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x20);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable Dynamic EQ\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x00);
            }
            break;
        case 4: //sampling rate
            break;
        case 5: //compression mode
            if(u8value == 0)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Heavy Mode\r\n");
            else if(u8value == 1)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Medium Mode\r\n");
            else if(u8value == 2)
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Low Mode\r\n");
            HAL_SOUND_SetPara(0x28,0x81,Gain_GCF[u8value][0],Gain_GCF[u8value][1],Gain_GCF[u8value][2]);
            break;
        case 6: //GC
            u32value = 0x010000 + ((MS_U32)u8value)*6553;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "GC:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x81,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 7: //EQ Offset
            u32value = 40*65536 - ((MS_U32)u8value)*65536;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "EQ_Offset:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x82,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 8: //VolSetting
            u32value = 10*65536 - ((MS_U32)u8value)*65536;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "VolSetting:  %x",(MS_U8)(u32value>>16 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x",(MS_U8)(u32value>>8 & 0xff));
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, " %x\r\n",(MS_U8)(u32value & 0xff));
            HAL_SOUND_SetPara(0x28,0x80,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 9: //Main Vol
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Main Vol -%1ddB\r\n",u8value);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0xff, 0x0C + u8value);
            break;

        case 10: //ABX
            if(u8value)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Enable ABX\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x10);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Disable ABX\r\n");
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x00);
            }
            break;
        case 11: //ABX gWet
            if(u8value > 24) u8value = 24;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "ABX gWet %1d\r\n",u8value);
            HAL_SOUND_SetPara(0x28,0x83,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 12: //ABX gDry
            if(u8value > 24) u8value = 24;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "ABX gDry %1d\r\n",u8value);
            HAL_SOUND_SetPara(0x28,0x84,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 13: //ABX FILSET
            if(u8value > 1) u8value = 1;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "FILSET %1d\r\n",u8value+1);
            HAL_SOUND_SetPara(0x28,0x85,0,0,u8value);
            break;
    }
#else
    u8mode = u8mode;
    u8value = u8value;
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_ADVSOUND_ProcessEnable
/// @brief \b Function \b Description: This routine is used to enable process of advanced sound effect.
/// @param <IN>        \b ADVSND_TYPE type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_ADVSOUND_ProcessEnable(AUDIO_DSP_CODE_TYPE type)
{
    MS_BOOL status = FALSE;
    MS_BOOL ret_status = FALSE;

    if (type == AU_DVB2_ADVSND_SRS)      //TSXT
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
    }
    else if (type == AU_DVB2_ADVSND_TSHD)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
    }
    else if (type == AU_DVB2_ADVSND_PURESND)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
    }
    else if (type == AU_DVB2_ADVSND_DBX)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_SOUND_DBX_Enable(TRUE);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
    }
    else if (type == AU_DVB2_ADVSND_BONGIOVI_DPS)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);

    }
    else if (type == AU_DVB2_ADVSND_SONICMOTION_ABS3D)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
        ret_status = HAL_AUDIO_Alg2ReloadCode(type);
        if (ret_status == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            status = TRUE;
        }
        HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
    }
    else if (type == AU_DVB2_ADVSND_MAX)
    {
        HAL_ADVSOUND_SubProcessEnable(SRS_THEATERSOUND_TOTAL, 0);    //Disable all SRS function
        if (HAL_AUDIO_GetAdvSndEffDspCodeType() == AU_DVB2_ADVSND_DBX)
        {
            HAL_SOUND_DBX_Enable(FALSE);
        }
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,0x00);
        status = TRUE;
    }
    else
    {
        if(HAL_AUDIO_GetAdvSndEffDspCodeType() == AU_DVB2_ADVSND_DBX)
        {
            HAL_SOUND_DBX_Enable(FALSE);
        }
        HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,0x00);
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;34m-----------Error : IP Security Checking Fail !!! type=%d------------\033[0m \n", type);
    }
    return(status);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_ADVSOUND_SubProcessEnable()
/// @brief \b Function \b Description:  This function is used to set the Enable Process of advanced sound effect.
/// @param <IN>        \b level:
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{
    MS_BOOL status = FALSE;
    switch (proc)
    {

        //SRS_TSXT
        case SRS_TSXT_TRUBASS:
            HAL_SOUND_SRS_TruBass((MS_U8)enable);
            status = TRUE;
            break;

        case SRS_TSXT_DC:
            HAL_SOUND_SRS_DC((MS_U8)enable);
            status = TRUE;
            break;

        //SRS_TSHD
        case SRS_TSHD_TRUBASS:
            HAL_SOUND_TSHD_TruBass((MS_U8)enable);
            status = TRUE;
            break;

        case SRS_TSHD_DC:
            HAL_SOUND_TSHD_Clarity((MS_U8)enable);
            status = TRUE;
            break;

        case SRS_TSHD_DEFINITION:
            HAL_SOUND_TSHD_Definition((MS_U8)enable);
            status = TRUE;
            break;

        case SRS_TSHD_SRS3D:
            HAL_SOUND_TSHD_SRS3D((MS_U8)enable);
            status = TRUE;
            break;

        //SRS_THEATERSOUND
        case SRS_THEATERSOUND_3D_EN:
        case SRS_THEATERSOUND_TOTAL:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TSHD:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TSHD_SURR:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_SURR_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        //SRS_THEATERSOUND_CC3D
        case SRS_THEATERSOUND_CC3D_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_DEPTH_PROCESS_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_3D_SURR_BOOST_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CC3D_FADE_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_FADE_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CC3D_TSHD_MIX_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TSHD_MIX_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CC3D_TBHDX_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_GEQ_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUBASS:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_DC:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_DEFINITION:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DEFINITION_FRONT_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TSHD_LEVEL_INDPT:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUVOLUME:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_HARDLIMITER:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HL_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_HPF:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HPF_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_HPF_END:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HPF_END_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUEQ:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_AEQ_PEQ_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_VIQ_NOISE_MNGR_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_CS:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUDIALOG:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUVOLUME_NORMALIZER:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_NORMALIZER_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_THEATERSOUND_TRUVOLUME_SMOOTH:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_SMOOTH_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        //AUDYSSEY
        case AUDYSSEY_DYNAMICVOL:
            HAL_SOUND_ADV_SetPara(2,(MS_U8)enable);
            status = TRUE;
            break;

        case AUDYSSEY_DYNAMICEQ:
            HAL_SOUND_ADV_SetPara(3,(MS_U8)enable);
            status = TRUE;
            break;

        case AUDYSSEY_EQ:
            break;

        case AUDYSSEY_ABX:
            HAL_SOUND_ADV_SetPara(10,(MS_U8)enable);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_PURESOUND_AEQ:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        case SRS_PURESOUND_HPF:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HPF_EN, (MS_U16)enable, 0);
            status = TRUE;
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;34m-----------Error : HAL_ADVSOUND_SubProcessEnable!!! Unknown type=%d, enable=%d------------\033[0m \n", proc, enable);
            break;
    }
    return (status);
}



////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_SetParam()
/// @brief \b Function \b Description:  This function is used to set Advanced Sound paramters
/// @param <IN>        \b level:
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{

    MS_BOOL status = FALSE;
    MS_U8   u8value1;

    u8value1 = (MS_U8)u32value1;

    switch (param)
    {
        //BBE
        case BBE_MODE_SET:
            HAL_SOUND_BBE_SetBbeMode(u8value1, 0);
            status = TRUE;
            break;

        //DOLBY
        case DOLBY_PL2VDPK_SMOD:
            HAL_SOUND_VSPK_SetSMod(u8value1);
            status = TRUE;
            break;

        case DOLBY_PL2VDPK_WMOD:
            HAL_SOUND_VSPK_SetWMod(u8value1);
            status = TRUE;
            break;

        //SRS TSXT
        case SRS_TSXT_SET_INPUT_GAIN:
            HAL_SOUND_SRS_SetTsxtPara(0,u8value1);
            status = TRUE;
            break;

        case SRS_TSXT_SET_DC_GAIN:
            HAL_SOUND_SRS_SetTsxtPara(1,u8value1);
            status = TRUE;
            break;

        case SRS_TSXT_SET_TRUBASS_GAIN:
            HAL_SOUND_SRS_SetTsxtPara(2,u8value1);
            status = TRUE;
            break;

        case SRS_TSXT_SET_SPEAKERSIZE:
            HAL_SOUND_SRS_SetTsxtPara(3,u8value1);
            status = TRUE;
            break;

        case SRS_TSXT_SET_INPUT_MODE:
            HAL_SOUND_SRS_SetTsxtPara(4,u8value1);
            status = TRUE;
            break;

        case SRS_TSXT_SET_OUTPUT_GAIN:
            HAL_SOUND_SRS_SetTsxtPara(7,u8value1);
            status = TRUE;
            break;

        //SRS TSHD
        case SRS_TSHD_SET_INPUT_MODE:
            HAL_SOUND_SRS_SetTshdPara(2,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_OUTPUT_MODE:
            HAL_SOUND_SRS_SetTshdPara(3,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_SPEAKERSIZE:
            HAL_SOUND_SRS_SetTshdPara(4,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_TRUBASS_CONTROL:
            HAL_SOUND_SRS_SetTshdPara(5,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_DEFINITION_CONTROL:
            HAL_SOUND_SRS_SetTshdPara(6,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_DC_CONTROL:
            HAL_SOUND_SRS_SetTshdPara(7,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_SURROUND_LEVEL:
            HAL_SOUND_SRS_SetTshdPara(8,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_INPUT_GAIN:
            HAL_SOUND_SRS_SetTshdPara(12,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_WOWSPACE_CONTROL:
            HAL_SOUND_SRS_SetTshdPara(13,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_WOWCENTER_CONTROL:
            HAL_SOUND_SRS_SetTshdPara(14,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_WOWHDSRS3DMODE:
            HAL_SOUND_SRS_SetTshdPara(15,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_LIMITERCONTROL:
            HAL_SOUND_SRS_SetTshdPara(16,u8value1);
            status = TRUE;
            break;

        case SRS_TSHD_SET_OUTPUT_GAIN:
            HAL_SOUND_SRS_SetTshdPara(17,u8value1);
            status = TRUE;
            break;

        //AUDYSSEY
        case AUDYSSEY_DYNAMICVOL_COMPRESS_MODE:
            HAL_SOUND_ADV_SetPara(5,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_DYNAMICVOL_GC:
            HAL_SOUND_ADV_SetPara(6,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_DYNAMICVOL_VOLSETTING:
            HAL_SOUND_ADV_SetPara(8,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_DYNAMICEQ_EQOFFSET:
            HAL_SOUND_ADV_SetPara(7,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_ABX_GWET:
            HAL_SOUND_ADV_SetPara(11,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_ABX_GDRY:
            HAL_SOUND_ADV_SetPara(12,u8value1);
            status = TRUE;
            break;

        case AUDYSSEY_ABX_FILSET:
            HAL_SOUND_ADV_SetPara(13,u8value1);
            status = TRUE;
            break;

        case DBX_MODE_SET:
            HAL_SET_DBX_MODE(u8value1);
            status = FALSE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_INPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_INPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_OUTPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_OUTPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_BYPASS_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_BYPASS_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_LIMITERBOOST:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_LIMITERBOOST,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_HARDLIMIT:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_HARDLIMIT,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HL_DELAYLEN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_DELAYLEN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_AEQ_INPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_INPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_AEQ_OUTPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_AEQ_BYPASS_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_BYPASS_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_HPF_FREQUENCY:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HPF_FREQUENCY,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_FILTER_CONFIG:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_FILTER_CONFIG,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_TRUBASS_LEVEL:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_TRUBASS_LEVEL,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_SPEAKER_SIZE:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_SPEAKER_SIZE,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_LEVEL_INDEPENDENT_EN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_COMPRESSOR_LEVEL,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_MODE:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_MODE,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_SPEAKER_AUDIO:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_SPEAKER_AUDIO,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_TBHD_SPEAKER_ANALYSIS,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_INPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_INPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        //SRS PURESOUND
        case SRS_PURESOUND_OUTPUT_GAIN:
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_OUTPUT_GAIN,u32value1, u16value2);
            status = TRUE;
            break;

        case BONGIOVI_DPS_SET_GLOBAL_INPUT_GAIN:
            {
                MS_S16 s16dBGainValue;

                // Input gain (global input gain) -range -20dB to +12dB in 1dB increments
                s16dBGainValue = (MS_S16)u32value1;   // cast to signed type for below range check (-20~12).
                if (s16dBGainValue > 12)
                    s16dBGainValue = 12;
                if (s16dBGainValue < -20)
                    s16dBGainValue = -20;
                u32value1 = s16dBGainValue;           // cast to unsigned type again.

                u32value1 = u32value1 - 12;
                u32value1 = -u32value1;
                HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_GLOBAL_INPUT_GAIN_CONTROL_ADDR, u32value1, DSP_MEM_TYPE_PM);

                status = TRUE;
                break;
            }

        case BONGIOVI_DPS_SET_GLOBAL_OUTPUT_GAIN:
            {
                MS_S16 s16dBGainValue;

                // Output gain (global output gain) - range -20dB to +12dB in 1dB increments.
                s16dBGainValue = (MS_S16)u32value1;   // cast to signed type for below range check (-20~12).
                if (s16dBGainValue > 12)
                    s16dBGainValue = 12;
                if (s16dBGainValue < -20)
                    s16dBGainValue = -20;
                u32value1 = s16dBGainValue;           // cast to unsigned type again.

                u32value1 = u32value1 - 12;
                u32value1 = -u32value1;
                HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_GLOBAL_OUTPUT_GAIN_CONTROL_ADDR, u32value1, DSP_MEM_TYPE_PM);

                status = TRUE;
                break;
            }

        case BONGIOVI_DPS_SET_STEREO_WIDENER_MGAIN:
            {
                MS_S16 s16dBGainValue;

                s16dBGainValue = (MS_S16)u32value1;   // cast to signed type for below range check (0~12).
                if (s16dBGainValue > 12)
                    s16dBGainValue = 12;
                if (s16dBGainValue < 0)
                    s16dBGainValue = 0;
                u32value1 = s16dBGainValue;           // cast to unsigned type again.

                u32value1 = u32value1 - 12;
                u32value1 = -u32value1;
                HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_STEREO_WIDENER_MGAIN_CONTROL_ADDR, u32value1, DSP_MEM_TYPE_PM);

                status = TRUE;
                break;
            }

        case BONGIOVI_DPS_SET_STEREO_WIDENER_SGAIN:
            {
                MS_S16 s16dBGainValue;

                // Stereo Widening Amount - sGain (sides variable) - range 0dB to +12dB in 1dB increments.
                s16dBGainValue = (MS_S16)u32value1;   // cast to signed type for below range check (0~12).
                if (s16dBGainValue > 12)
                    s16dBGainValue = 12;
                if (s16dBGainValue < 0)
                    s16dBGainValue = 0;
                u32value1 = s16dBGainValue;           // cast to unsigned type again.

                u32value1 = u32value1 - 12;
                u32value1 = -u32value1;
                HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_STEREO_WIDENER_SGAIN_CONTROL_ADDR, u32value1, DSP_MEM_TYPE_PM);

                status = TRUE;
                break;
            }

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;34m-----------Error : HAL_ADVSOUND_SetParam!!! Unknown param=%d, u32value1=0x%x------------\033[0m \n", param, (int)u32value1);
            status = FALSE;
            break;
    }
    return (status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description:  This function is used to copy user parameters to hal
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size)
{
    switch (type)
    {
        case DBX_LOAD_DM:
        case DBX_LOAD_PM:
        case DBX_COPY_TotSonDM:
        case DBX_COPY_TotSonPM:
        case DBX_COPY_TotVolDM:
        case DBX_COPY_TotVolPM:
        case DBX_COPY_TotSurDM:
        case DBX_COPY_TotSurPM:
            // The input argument "size" for MApi_AUDIO_COPY_Parameter is byte size, but the "size" for HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP for DBX is integer size. So size needs to be divided by sizeof(MS_U32).
            HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP(type, (MS_U32 *)Parameter_ptr, size/sizeof(MS_U32));
            break;

        case BONGIOVI_DPS_SET_PARAMS:
            {
                MS_U32 dps_param_count, dps_param_update_count;
                MS_U32 i;

                dps_param_count = HAL_MAD2_Read_DSP_sram(BONGIOVI_DPS_PARAM_COUNT_ADDR, DSP_MEM_TYPE_PM);
                for (i=0; i<dps_param_count; i++)
                {
                    HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_STEREO_WIDENER_ADDR+i, ((MS_U32 *)Parameter_ptr)[i]>>8, DSP_MEM_TYPE_PM);
                }
                dps_param_update_count = HAL_MAD2_Read_DSP_sram(BONGIOVI_DPS_UPDATE_COUNT_ADDR, DSP_MEM_TYPE_PM);
                dps_param_update_count = dps_param_update_count+1;
                HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_UPDATE_COUNT_ADDR, dps_param_update_count, DSP_MEM_TYPE_PM);
            }
            break;

        default:
            break;
    }
}

#define H2BYTE_1(value)             ((MS_U8)((value) / 0x10000))
#define HIBYTE_1(value)             ((MS_U8)((value >>8) & 0x0000FF))
#define LOBYTE_1(value)             ((MS_U8)(value&0x0000FF))

MS_BOOL  HAL_SOUND_DBX_Enable(MS_BOOL u8enDBX)
{
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
MS_U8 tmp_volume;
    if (u8enDBX)
    {
        if(DBX_PATH == AUDIO_I2S_OUTPUT)
        {
           tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_I2S_VOLUME);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x80);          //mute
           MsOS_DelayTask(20);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x1F);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x80);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteByte(REG_SOUND_I2S_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT0_OUTPUT)
        {
           tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x80);          //mute
           MsOS_DelayTask(20);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x1F);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x40);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteByte(REG_SOUND_AUOUT0_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT1_OUTPUT)
        {
           tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT1_VOLUME);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x80);          //mute
           MsOS_DelayTask(20);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x1F);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x20);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteByte(REG_SOUND_AUOUT1_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT2_OUTPUT)
        {
           tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT2_VOLUME);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x80);          //mute
           MsOS_DelayTask(20);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x1F);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x10);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteByte(REG_SOUND_AUOUT2_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT3_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT3_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x1F);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x08);         //set dbx refrence to path main
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT3_VOLUME, tmp_volume);          //un-mute
        }
    }
    else
    {
        if(DBX_PATH == AUDIO_I2S_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_I2S_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_I2S_VOLUME, tmp_volume);          //un-mute

        }
        else if(DBX_PATH == AUDIO_AUOUT0_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT0_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT1_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT1_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT1_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT2_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT2_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT2_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT3_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT3_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG5, 0xFF, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT3_VOLUME, tmp_volume);          //un-mute
        }

    }

        return TRUE;
#else
        return TRUE;
    #endif
}

MS_BOOL  HAL_SET_DBX_MODE(MS_U8 u8enMODE)
{
         u8enMODE = u8enMODE;
         return TRUE;
}

void HAL_DBXTV_SetMode(EN_DBXTV_TotSonMode enTotSonMode, EN_DBXTV_TotVolMode enTotVolMode, EN_DBXTV_TotSurMode enTotSurMode,MS_U32 enable)
{
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
MS_U8 tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_I2S_VOLUME);

    if(DBX_PATH == AUDIO_I2S_OUTPUT)
    {
       tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_I2S_VOLUME);
       HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x80);          //mute
    }
    else if(DBX_PATH == AUDIO_AUOUT0_OUTPUT)
    {
       tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);
       HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x80);          //mute
    }
    else if(DBX_PATH == AUDIO_AUOUT1_OUTPUT)
    {
        tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT1_VOLUME);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x80);          //mute
    }
    else if(DBX_PATH == AUDIO_AUOUT2_OUTPUT)
    {
        tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT2_VOLUME);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x80);          //mute
    }
    else if(DBX_PATH == AUDIO_AUOUT3_OUTPUT)
    {
        tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT3_VOLUME);
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x80, 0x80);          //mute
    }

//    HAL_AUDIO_WriteMaskByte(0x2D01, 0x80, 0x80);          //mute
    MsOS_DelayTask(20);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2, 0x1F, 0x00);          //disable DBX

    // Turn off dbx-tv
    HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_DVCOnOff, 0x000000,DSP_MEM_TYPE_DM);
    HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_DPPOnOff, 0x000000,DSP_MEM_TYPE_DM);
    HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_IWBypass, 0x000001,DSP_MEM_TYPE_DM);
    HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_EQBypass, 0x000001,DSP_MEM_TYPE_DM);
    HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_BEOnOff, 0x000000,DSP_MEM_TYPE_DM);


    if ( (enTotSonMode != E_TOTSON_OFF) )
    {
        // TotVol is off and TotSur is off
        // Copy the Total Sonics preset to memory
        HAL_Copy((void *)&TotSonDM[enTotSonMode], DBX_DM_ABS_ADDRESS|0x8000, TOTSON_LENGTH_DM);
        HAL_Copy((void *)&TotSonPM[enTotSonMode], DBX_PM_ABS_ADDRESS, TOTSON_LENGTH_PM);
    }
    if ( (enTotVolMode != E_TOTVOL_OFF) )
    {
        // Copy the Total Volume DVC preset to memory
        HAL_Copy((void *)&TotVolDM[enTotVolMode].Data[DVC_OFFSET_DM], (DBX_DM_ABS_ADDRESS+DVC_OFFSET_DM)|0x8000, DVC_LENGTH_DM);
        HAL_Copy((void *)&TotVolPM[enTotVolMode].Data[DVC_OFFSET_PM], DBX_PM_ABS_ADDRESS+DVC_OFFSET_PM, DVC_LENGTH_PM);
        // Copy the Total Volume DPP preset to memory
        HAL_Copy((void *)&TotVolDM[enTotVolMode].Data[DPP_OFFSET_DM], (DBX_DM_ABS_ADDRESS+DPP_OFFSET_DM)|0x8000, DPP_LENGTH_DM);
        HAL_Copy((void *)&TotVolPM[enTotVolMode].Data[DPP_OFFSET_PM], DBX_PM_ABS_ADDRESS+DPP_OFFSET_PM, DPP_LENGTH_PM);
    }
    if ( (enTotSurMode != E_TOTSUR_OFF) )
    {

        // Copy the Total Surround level parameters to memory
        if (enTotVolMode == E_TOTVOL_OFF)   //195_2
        {
            HAL_Copy((void *)&TotSurDM[enTotSurMode].Data[DM_OFFSET_DVCHiLevel], (DBX_DM_ABS_ADDRESS+DM_OFFSET_DVCHiLevel)|0x8000, 1);
            HAL_Copy((void *)&TotSurDM[enTotSurMode].Data[DM_OFFSET_DVCLoLevel], (DBX_DM_ABS_ADDRESS+DM_OFFSET_DVCLoLevel)|0x8000, 1);
        }

        if (enTotVolMode != E_TOTVOL_NIGHT)
        {
            // Copy the Total Surround IW preset to memory
            HAL_Copy((void *)&TotSurDM[enTotSurMode].Data[IW_OFFSET_DM], (DBX_DM_ABS_ADDRESS+IW_OFFSET_DM)|0x8000, IW_LENGTH_DM);
            HAL_Copy((void *)&TotSurPM[enTotSurMode].Data[IW_OFFSET_PM], DBX_PM_ABS_ADDRESS+IW_OFFSET_PM, IW_LENGTH_PM);
            // Copy the Total Surround DPP preset to memory
            HAL_Copy((void *)&TotSurDM[enTotSurMode].Data[DPP_OFFSET_DM], (DBX_DM_ABS_ADDRESS+DPP_OFFSET_DM)|0x8000, DPP_LENGTH_DM);
            HAL_Copy((void *)&TotSurPM[enTotSurMode].Data[DPP_OFFSET_PM], DBX_PM_ABS_ADDRESS+DPP_OFFSET_PM, DPP_LENGTH_PM);
        }
    }

    if (enTotVolMode == E_TOTVOL_NIGHT)
    {
        // Turn off IW
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_IWBypass, 0x000001, DSP_MEM_TYPE_DM);

    }

    if ((enable&Enable_DVC) == 0)
    {
        // Bypass DVC
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_DVCOnOff, 0x000000,DSP_MEM_TYPE_DM);
    }
    if ((enable&Enable_DPP) == 0)
    {
        // Bypass DPP
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_DPPOnOff, 0x000000,DSP_MEM_TYPE_DM);
    }
    if ((enable&Enable_IW) == 0)
    {
        // Bypass IW
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_IWBypass, 0x000001,DSP_MEM_TYPE_DM);
    }
    if ((enable&Enable_EQ) == 0)
    {
        // Bypass EQ
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_EQBypass, 0x000001,DSP_MEM_TYPE_DM);
    }
    if ((enable&Enable_BE) == 0)
    {
        // Bypass BE
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_BEOnOff, 0x000000,DSP_MEM_TYPE_DM);
    }

    HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2, 0x1F, 0x1F);          //enable DBX

    MsOS_DelayTask(20);

    if(DBX_PATH == AUDIO_I2S_OUTPUT)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0xFF, tmp_volume);          //unmute
    }
    else if(DBX_PATH == AUDIO_AUOUT0_OUTPUT)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0xFF, tmp_volume);          //unmute
    }
    else if(DBX_PATH == AUDIO_AUOUT1_OUTPUT)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0xFF, tmp_volume);          //unmute
    }
    else if(DBX_PATH == AUDIO_AUOUT2_OUTPUT)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0xFF, tmp_volume);          //unmute
    }
    else if(DBX_PATH == AUDIO_AUOUT3_OUTPUT)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0xFF, tmp_volume);          //unmute
    }

#else
    {
         enTotSonMode = enTotSonMode;
}

#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_DBXTV_SetVolPath(AUDIO_PATH_TYPE path)
/// @brief \b Function \b Description: This routine is used to set dbx TV volume path
/// @param <IN>        \b param, AUDIO_PATH_TYPE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void Hal_DBXTV_SetVolPath(AUDIO_OUTPUT_TYPE path){
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
  DBX_PATH = path;
#else
  path = path;
#endif
}

void HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP(ADVSND_PARAM type, MS_U32 *Parameter_ptr, MS_U32 size)
{
      MS_U32 i=0;
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)

      MS_U32* tmp_ptr=(void *)&TotSonDM[0];

        switch (type)
        {
            case DBX_LOAD_PM:
                break;

            case DBX_LOAD_DM:
                break;

            case DBX_COPY_TotSonDM:
                tmp_ptr = (void *)&TotSonDM[0];
                break;

            case DBX_COPY_TotSonPM:
                tmp_ptr = (void *)&TotSonPM[0];
                break;


            case DBX_COPY_TotVolPM:
                tmp_ptr = (void *)&TotVolPM[0];
                break;

            case DBX_COPY_TotVolDM:
                tmp_ptr = (void *)&TotVolDM[0];
                break;


            case DBX_COPY_TotSurDM:
                tmp_ptr = (void *)&TotSurDM[0];
                break;


            case DBX_COPY_TotSurPM:
                tmp_ptr = (void *)&TotSurPM[0];
                break;

            default:

                break;
        }

        for (i=0; i<size; i++)
            *tmp_ptr++ = *Parameter_ptr++;

#else
      i = i;
#endif


}

/**
    Function to copy from preset memory to DSP memory.
    @param ptr pointer to memory
    @param Address DSP address
    @param Length in words
    @return TRUE if success, FALSE if error occurred
    @note
*/
int HAL_Copy(MS_U32 *ptr_data, unsigned int Address, unsigned int Length)
{
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
    MS_U8   dat[3],i;

    if(HAL_AUDIO_GetDSPalive() == 0)
        return 0;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    HAL_AUDIO_WriteMaskReg(REG_SE_BDMA_CFG, 0xFFF0, 0x0000); //to set IDMA dta from MCU not from SE BDMA
    HAL_AUDIO_WriteMaskByte(REG_FD230_SELECT, 0x01, 0x01); // IDMA is used by SE-DSP

    if ((HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_READ_RDY) == FALSE)||(HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_WRITE_RDY) == FALSE))
    {
        printf("ERROR: DEC DSP IDMA Busy \n");
        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
        return FALSE;
    }

    HAL_AUDIO_WriteByte( REG_SE_IDMA_WRBASE_ADDR_L, (MS_U8)(Address&0xFF));
    HAL_AUDIO_WriteByte( REG_SE_IDMA_WRBASE_ADDR_H, (MS_U8)(Address>>8));

    for (i = 0; i<Length; i++)
    {
        dat[2] = H2BYTE_1(ptr_data[i]);
        dat[1] = HIBYTE_1(ptr_data[i]);
        dat[0] = LOBYTE_1(ptr_data[i]);

        HAL_AUDIO_WriteByte(REG_SE_DSP_BRG_DATA_L,dat[1]);
        HAL_AUDIO_WriteByte(REG_SE_DSP_BRG_DATA_H,dat[2]);
        MsOS_DelayTask(2);
        if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
        {
            printf("ERROR: DEC DSP IDMA write data time out1 \n");
            OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
            return FALSE;
        }

        HAL_AUDIO_WriteByte(REG_SE_DSP_BRG_DATA_L,dat[0]);
        HAL_AUDIO_WriteByte(REG_SE_DSP_BRG_DATA_H,0x00);
        if (HAL_AUDSP_CheckSeIdmaReady(AUD_CHK_DSP_WRITE_RDY)==FALSE)
        {
            printf("ERROR: DEC DSP IDMA write data time out2 \n");
            OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
            return FALSE;
        }
    }

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
#endif
    return TRUE;
}

