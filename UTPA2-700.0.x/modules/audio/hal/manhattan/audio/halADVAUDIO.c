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
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/io.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "regCHIP.h"
#include "drvAUDIO_if.h"
#include "halAUDIO.h"

// Internal Definition
#include "halSOUND.h"
#include "regAUDIO.h"
#include "halMAD2.h"
#include "halADVAUDIO.h"
#include "halAUR2.h"
#include "audio_comm2.h"

extern MS_S32  _s32AUDIOMutexIDMA;

#define  SrsDmAddr  0x1C
#define  TshdDmAddr  0x1C

#define DBG_PRINT_P1(msg) //msg
#define DBG_PRINT_P2(msg) //msg
#define DBG_PRINT_P3(msg) //msg

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

extern AUDIO_SHARED_VARS2    * g_AudioVars2;

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
                DBG_PRINT_P1(printf("Enable ADV\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x80);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable ADV\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x00);
            }
            break;
        case 2: //Dyn Vol
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable Dynamic Volume\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x40);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable Dynamic Volume\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x00);
            }
            break;
        case 3: //Dyn EQ
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable Dynamic EQ\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x20);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable Dynamic EQ\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x00);
            }
            break;
        case 4: //sampling rate
            break;
        case 5: //compression mode
         /*   if(u8value == 0)
                DBG_PRINT_P1(printf("Heavy Mode\r\n"));
            else if(u8value == 1)
                DBG_PRINT_P1(printf("Medium Mode\r\n"));
            else if(u8value == 2)
                DBG_PRINT_P1(printf("Low Mode\r\n")); */
            HAL_SOUND_SetPara(0x28,0x81,Gain_GCF[u8value][0],Gain_GCF[u8value][1],Gain_GCF[u8value][2]);
            break;
        case 6: //GC
            u32value = 0x010000 + ((MS_U32)u8value)*6553;
            DBG_PRINT_P1(printf("GC:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x81,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 7: //EQ Offset
            u32value = 40*65536 - ((MS_U32)u8value)*65536;
            DBG_PRINT_P1(printf("EQ_Offset:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x82,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 8: //VolSetting
            u32value = 10*65536 - ((MS_U32)u8value)*65536;
            DBG_PRINT_P1(printf("VolSetting:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x80,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 9: //Main Vol
            DBG_PRINT_P1(printf("Main Vol -%1ddB\r\n",u8value));
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0xff, 0x0C + u8value);
            break;

        case 10: //ABX
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable ABX\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x10);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable ABX\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x00);
            }
            break;
        case 11: //ABX gWet
            if(u8value > 24) u8value = 24;
            DBG_PRINT_P1(printf("ABX gWet %1d\r\n",u8value));
            HAL_SOUND_SetPara(0x28,0x83,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 12: //ABX gDry
            if(u8value > 24) u8value = 24;
            DBG_PRINT_P1(printf("ABX gDry %1d\r\n",u8value));
            HAL_SOUND_SetPara(0x28,0x84,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 13: //ABX FILSET
            if(u8value > 1) u8value = 1;
            DBG_PRINT_P1(printf("FILSET %1d\r\n",u8value+1));
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
    char *st=NULL;

    //printf("HAL_SOUND_APOLLO_SetPara: %x %x %x\r\n",(int)u8mode,(int)u32param, (int)u16value2);
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
        case AU_SRS_THEATER_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_processEn, AASE1,	0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("\n\033[0;34m THEATERSOUND3D_EN %d \033[0m \n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TSHD_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_SURR_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_Sur_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TSHD_SURR_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_TrubassFront_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_FRONT_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_DEFINITION_FRONT_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_Definition_En, AASE1, 0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TSHD_DEFINITION_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_DialogClarity_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TSHD_DIALOG_CLARITY_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_TSHD_Trubass_level_indep_En, AASE1,  0,  (MS_U16)bEnable);//morris modify 20111013
            DBG_PRINT_P1(printf("TSHD_TRUBASS_LEVEL_INDEPENDENT_EN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_DEPTH_PROCESS_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_DepthProcessing_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_DEPTH_PROCESS_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_3D_SURR_BOOST_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_3DSurroundBoost_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_3D_SURR_BOOST_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_FADE_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_Fade_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_FADE_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_TSHD_MIX_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TSHD_Mix_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_TSHD_MIX_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDx_En, AASE1, 0,   (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_TBHDX_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_GEQ_5B_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CC3D_GEQ_EN %d\n",bEnable));
            break;
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        case AU_SRS_THEATER_CC3D_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_InputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_OutputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_BYPASS_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_BypassGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_BYPASS_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_APERTURE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_Aperture, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_APERTURE %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GAINLIMIT:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GainLimit, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_GAINLIMIT %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_FF_DEPTH:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_FFDepth, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_FF_DEPTH %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_NF_DEPTH:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_NFDepth, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_NF_DEPTH %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TSHD_MIX_FADE_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TSHD_MixFadeCtrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_TSHD_MIX_FADE_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxInputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_TBHDX_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_BASSLEVEL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxBassLevel, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_TBHDX_BASSLEVEL %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_SPEAKERSIZE:
            st = st;
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxSpeakerSize, AASE1,  0, u32param);
            switch(u32param)
            {
                case SRS_TBHDX_SPEAKERSIZE_40HZ:
                    st="40Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_50HZ:
                    st="50Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_60HZ:
                    st="60Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_70HZ:
                    st="70Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_80HZ:
                    st="80Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_90HZ:
                    st="90Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_100HZ:
                    st="100Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_110HZ:
                    st="110Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_120HZ:
                    st="120Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_130HZ:
                    st="130Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_140HZ:
                    st="140Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_150HZ:
                    st="150Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_160HZ:
                    st="160Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_170HZ:
                    st="170Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_180HZ:
                    st="180Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_190HZ:
                    st="190Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_200HZ:
                    st="200Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_210HZ:
                    st="210Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_220HZ:
                    st="220Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_230HZ:
                    st="230Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_240HZ:
                    st="240Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_250HZ:
                    st="250Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_300HZ:
                    st="300Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_400HZ:
                    st="400Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_500HZ:
                    st="500Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_600HZ:
                    st="600Hz";
                    break;
                case SRS_TBHDX_SPEAKERSIZE_CUSTOM:
                    st="CUSTOM";
                    break;
                default:
                    st="UNKNOWN TYPE";
                    break;
            }
            DBG_PRINT_P1(printf("CC3D_TBHDX_SPEAKERSIZE %s (%d)\n",st,u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_MODE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxMode, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_TBHDX_MODE %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_DYNAMICS:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxDynamics, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_TBHDX_DYNAMICS %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_HP_ORDER:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxHPOrder, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_TBHDX_HP_ORDER %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_TBHDX_CUSTOM_FILTER:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxCustomFilter, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_TBHDX_CUSTOM_FILTER %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_input_gain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CC3D_GEQ_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_BAND0_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band0, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("CC3D_GEQ_BAND0_GAIN index =%d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_BAND1_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band1, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_GEQ_BAND1_GAIN index =%d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_BAND2_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band2, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_GEQ_BAND2_GAIN index =%d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_BAND3_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band3, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_GEQ_BAND3_GAIN index =%d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_GEQ_BAND4_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band4, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_GEQ_BAND4_GAIN index =%d\n",u32param));
            break;

        case AU_SRS_THEATER_CC3D_PROCESS_PATH:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CC3D_Process_Path, AASE1,  0, u32param);
            DBG_PRINT_P1(printf("CC3D_PROCESS_PATH =%d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_SURROUND_LEVEL_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_surround_level_ctrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_SURR_LEVEL_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_input_gain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_output_gain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_DEFINITION_FRONT_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_definition_ctrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_DEFINITION_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_DIALOG_CLARITY_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_dialog_clarity_ctrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_DIALOG_CLARITY_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_FRONT_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_front_ctrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_FRONT_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_SIZE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_size, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_SPEAKER_SIZE %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_COMPRESSOR_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_compressor_ctrl, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_COMPRESSOR_CTRL %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_PROCESS_MODE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_process_mode, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_PROCESS_MODE %d\n",u32param));
            break;
            /*   kSrsTruBassCustomSpeakerLFResponse20Hz = 0,         ///////For case AU_SRS_FTB_SPEAKER_AUDIO & case AU_SRS_FTB_SPEAKER_ANALYSIS setting //////
            kSrsTruBassCustomSpeakerLFResponse30Hz,
            kSrsTruBassCustomSpeakerLFResponse40Hz,
            kSrsTruBassCustomSpeakerLFResponse50Hz,
            kSrsTruBassCustomSpeakerLFResponse60Hz,
            kSrsTruBassCustomSpeakerLFResponse70Hz,
            kSrsTruBassCustomSpeakerLFResponse80Hz,
            kSrsTruBassCustomSpeakerLFResponse90Hz,
            kSrsTruBassCustomSpeakerLFResponse100Hz,
            kSrsTruBassCustomSpeakerLFResponse110Hz,
            kSrsTruBassCustomSpeakerLFResponse120Hz,
            kSrsTruBassCustomSpeakerLFResponse130Hz,
            kSrsTruBassCustomSpeakerLFResponse140Hz,
            kSrsTruBassCustomSpeakerLFResponse150Hz,
            kSrsTruBassCustomSpeakerLFResponse160Hz,
            kSrsTruBassCustomSpeakerLFResponse170Hz,
            kSrsTruBassCustomSpeakerLFResponse180Hz,
            kSrsTruBassCustomSpeakerLFResponse190Hz,
            kSrsTruBassCustomSpeakerLFResponse200Hz,
            kSrsTruBassCustomSpeakerLFResponse210Hz,
            kSrsTruBassCustomSpeakerLFResponse220Hz,
            kSrsTruBassCustomSpeakerLFResponse230Hz,
            kSrsTruBassCustomSpeakerLFResponse240Hz,
            kSrsTruBassCustomSpeakerLFResponse250Hz,
            kSrsTruBassCustomSpeakerLFResponse260Hz,
            kSrsTruBassCustomSpeakerLFResponse270Hz,
            kSrsTruBassCustomSpeakerLFResponse280Hz,
            kSrsTruBassCustomSpeakerLFResponse290Hz,
            kSrsTruBassCustomSpeakerLFResponse300Hz,
            kSrsTruBassCustomSpeakerLFResponse310Hz,
            kSrsTruBassCustomSpeakerLFResponse320Hz,
            kSrsTruBassCustomSpeakerLFResponse330Hz,
            kSrsTruBassCustomSpeakerLFResponse340Hz,
            kSrsTruBassCustomSpeakerLFResponse350Hz,
            kSrsTruBassCustomSpeakerLFResponse360Hz,
            kSrsTruBassCustomSpeakerLFResponse370Hz,
            kSrsTruBassCustomSpeakerLFResponse380Hz,
            kSrsTruBassCustomSpeakerLFResponse390Hz,
            kSrsTruBassCustomSpeakerLFResponse400Hz,
            */
        case AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_AUDIO:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_audio, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_SPEAKER_AUDIO %d\n",u32param));
            break;

        case AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_ANALYSIS:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_analysis, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("TSHD_TRUBASS_SPEAKER_ANALYSIS %d\n",u32param));
            break;

        case AU_SRS_THEATER_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_InputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_OutputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_BYPASS_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_BypassGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("BYPASS_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_HEADROOM_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_HeadroomGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("HEADROOM_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_INPUT_MODE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_InputMode, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("INPUT_MODE %d\n",u32param));
            break;

        // HL
        case AU_SRS_THEATER_HL_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_HL_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("HL_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_HL_LIMIT_LEVEL_CTRL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_HL_limit_ctrl, AASE1,  Q24H, Q24L);
            DBG_PRINT_P1(printf("HL_LIMIT_LEVEL_CTRL %d\n",bEnable));
            break;

        case AU_SRS_THEATER_HL_BOOST_GAIN:
            HAL_SND_R2_SetCommInfo(AU_SRS_THEATER_HL_BOOST_GAIN, AASE1,   Q24H8, Q24L8);
            DBG_PRINT_P1(printf("HL_BOOST_GAIN %d\n",bEnable));
            break;

        // HPF
        case AU_SRS_THEATER_HPF_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_HPFEn, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("HPF_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_HPF_END_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_HPF_End_En, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("HPF_END_EN %d\n",bEnable));
            break;

        // VOLHD
        case AU_SRS_THEATER_VIQ_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDEn, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("VOLHD_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_VIQ_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDInputGain, AASE1, Q24H5, Q24L5);
            DBG_PRINT_P1(printf("VOLHD_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_VIQ_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDOutGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("VOLHD_OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_VIQ_BYPASS_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDBypassGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("VOLHD_BYPASS_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_VIQ_VOLUME_MODE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDMode, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("VOLHD_VOLUME_MODE %d\n",u32param));
            break;

        case AU_SRS_THEATER_VOLHD_MAXGAIN_CONTROL:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDMaxGainControl, AASE1, Q24H5, Q24L5);
            DBG_PRINT_P1(printf("VOLHD_MAXGAIN_CTRL %d\n",u32param));
            break;

        // VOLHD_NORMALIZER
        case AU_SRS_THEATER_VOLHD_NORMALIZER_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDNormalizerEnable, AASE1, 0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("VOLHD_NORMALIZER_EN %d\n",u32param));
            break;

        //UNKNOWN
        case AU_SRS_THEATER_VOLHD_NORMTHRESH:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDNormThresh, AASE1,  Q24H, Q24L);
            DBG_PRINT_P1(printf("VOLHD_NORMTHRESH %d\n",u32param));
            break;

        // VOLHD_SMOOTH
        case AU_SRS_THEATER_VOLHD_SMOOTH_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDSmoothEnable, AASE1, 0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("VOLHD_SMOOTH_EN %d\n",u32param));
            break;

        // VOLHD_LKFS UNKNOWN
        case AU_SRS_THEATER_VOLHD_LKFS:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_truvolHDLKFS, AASE1,  Q24H, Q24L);
            DBG_PRINT_P1(printf("VOLHD_LKFS %d\n",u32param));
            break;

        // CS
        case AU_SRS_THEATER_CS_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSEn, AASE1, 0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("CS_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_CS_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSInputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("CS_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CS_PROCESSING_MODE:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSProcessingMode, AASE1, 0, u32param);
            DBG_PRINT_P1(printf("CS_PROCESSING_MODE %d\n",u32param));
            break;

        case AU_SRS_THEATER_CS_LR_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSLROutputGain, AASE1, Q24H3, Q24L3);
            DBG_PRINT_P1(printf("CS_LR_OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CS_LSRS_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSLsRsOutputGain, AASE1, Q24H3, Q24L3);
            DBG_PRINT_P1(printf("CS_LSRS_OUTPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_CS_CENTER_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_CSCenterOutputGain, AASE1, Q24H3, Q24L3);
            DBG_PRINT_P1(printf("CS_CENTER_OUT_GAIN %d\n",u32param));
            break;

        // TRUDIALOG
        case AU_SRS_THEATER_TRUDIALOG_EN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogEn, AASE1,  0,  (MS_U16)bEnable);
            DBG_PRINT_P1(printf("TRUDIALOG_EN %d\n",bEnable));
            break;

        case AU_SRS_THEATER_TRUDIALOG_INPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogInputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TRUDIALOG_INPUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TRUDIALOG_OUTPUT_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogOutputGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TRUDIALOG_OUT_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TRUDIALOG_BYPASS_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogBypassGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TRUDIALOG_BYPASS_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TRUDIALOG_PROCESS_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogProcessGain, AASE1, Q24H4, Q24L4);
            DBG_PRINT_P1(printf("TRUDIALOG_PROCESS_GAIN %d\n",u32param));
            break;

        case AU_SRS_THEATER_TRUDIALOG_CLARITY_GAIN:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SRS_Theater_trudialogClarityGain, AASE1, Q24H, Q24L);
            DBG_PRINT_P1(printf("TRUDIALOG_CLARITY_GAIN %d\n",u32param));
            break;

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
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_EN %d===@@@\r\n",u8mode,u32param));
            break;

        case AU_SRS_PURESOUND_HL_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlInputGain_addr, u32value2, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_INPUT_GAIN %u===@@@\r\n",u8mode,u32value2));
            break;

        case AU_SRS_PURESOUND_HL_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlOutputGain_addr, u32value2, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value2));
            break;

        case AU_SRS_PURESOUND_HL_BYPASS_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlBypassGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_BYPASS_GAIN %u===@@@\r\n",u8mode,u32value));
            break;

        case AU_SRS_PURESOUND_HL_LIMITERBOOST:
            if( u16value2 == 0)
            {
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlLimiterboost_addr, u32value5, DSP_MEM_TYPE_DM);
                DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_LIMITERBOOST %lu===@@@\r\n",u8mode,u32value5));
            }
            else
            {
                HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlLimiterboost_addr, FXP24Value, DSP_MEM_TYPE_DM);
                DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_LIMITERBOOST %lu===@@@\r\n",u8mode,FXP24Value));
            }
            break;

        case AU_SRS_PURESOUND_HL_HARDLIMIT:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlHardLimit_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_HARDLIMIT %u===@@@\r\n",u8mode,u32value));
            break;

        case AU_SRS_PURESOUND_HL_DELAYLEN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_hlDelaylen_addr, u32param, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HL_DELAYLEN %d===@@@\r\n",u8mode,u32param));
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
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_EN %d===@@@\r\n",u8mode,u32param));
            break;

        case AU_SRS_PURESOUND_AEQ_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqInputGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_INPUT_GAIN %u===@@@\r\n",u8mode,u32value));
            break;

        case AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqOutputGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value));
            break;

        case AU_SRS_PURESOUND_AEQ_BYPASS_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_AeqBypassGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_AEQ_BYPASS_GAIN %u===@@@\r\n",u8mode,u32value));
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
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_FILTER_CONFIG %d===@@@\r\n",u8mode,u32param));
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
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HPF_EN %d===@@@\r\n",u8mode,u32param));
            break;

        case AU_SRS_PURESOUND_HPF_FREQUENCY:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mHPFfc_addr, u32param, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_HPF_FREQUENCY %d===@@@\r\n",u8mode,u32param));
            break;

        case AU_SRS_PURESOUND_INPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mInputGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_INPUT_GAIN %u===@@@\r\n",u8mode,u32value));
            break;

        case AU_SRS_PURESOUND_OUTPUT_GAIN:
            HAL_MAD2_Write_DSP_sram(SRS_PURESOUND_mOutputGain_addr, u32value, DSP_MEM_TYPE_DM);
            DBG_PRINT_P1(printf("\r\n@@@== DSP [%d]AU_SRS_PURESOUND_OUTPUT_GAIN %u===@@@\r\n",u8mode,u32value));
            break;

        default:
            //printf("-----HAL_SOUND_SRS_APOLLO_SetPara: Unknown command u8mode=%d u16value=0x%lx-----\r\n", u8mode, (MS_U32)u32param);
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
                DBG_PRINT_P1(printf("Enable ADV\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x80);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable ADV\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x80, 0x00);
            }
            break;
        case 2: //Dyn Vol
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable Dynamic Volume\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x40);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable Dynamic Volume\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x40, 0x00);
            }
            break;
        case 3: //Dyn EQ
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable Dynamic EQ\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x20);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable Dynamic EQ\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x20, 0x00);
            }
            break;
        case 4: //sampling rate
            break;
        case 5: //compression mode
            if(u8value == 0)
                DBG_PRINT_P1(printf("Heavy Mode\r\n"));
            else if(u8value == 1)
                DBG_PRINT_P1(printf("Medium Mode\r\n"));
            else if(u8value == 2)
                DBG_PRINT_P1(printf("Low Mode\r\n"));
            HAL_SOUND_SetPara(0x28,0x81,Gain_GCF[u8value][0],Gain_GCF[u8value][1],Gain_GCF[u8value][2]);
            break;
        case 6: //GC
            u32value = 0x010000 + ((MS_U32)u8value)*6553;
            DBG_PRINT_P1(printf("GC:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x81,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 7: //EQ Offset
            u32value = 40*65536 - ((MS_U32)u8value)*65536;
            DBG_PRINT_P1(printf("EQ_Offset:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x82,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 8: //VolSetting
            u32value = 10*65536 - ((MS_U32)u8value)*65536;
            DBG_PRINT_P1(printf("VolSetting:  %x",(MS_U8)(u32value>>16 & 0xff)));
            DBG_PRINT_P1(printf(" %x",(MS_U8)(u32value>>8 & 0xff)));
            DBG_PRINT_P1(printf(" %x\r\n",(MS_U8)(u32value & 0xff)));
            HAL_SOUND_SetPara(0x28,0x80,(MS_U8)(u32value>>16 & 0xff),(MS_U8)(u32value>>8 & 0xff),(MS_U8)(u32value & 0xff));
            break;
        case 9: //Main Vol
            DBG_PRINT_P1(printf("Main Vol -%1ddB\r\n",u8value));
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0xff, 0x0C + u8value);
            break;

        case 10: //ABX
            if(u8value)
            {
                DBG_PRINT_P1(printf("Enable ABX\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x10);
            }
            else
            {
                DBG_PRINT_P1(printf("Disable ABX\r\n"));
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1, 0x10, 0x00);
            }
            break;
        case 11: //ABX gWet
            if(u8value > 24) u8value = 24;
            DBG_PRINT_P1(printf("ABX gWet %1d\r\n",u8value));
            HAL_SOUND_SetPara(0x28,0x83,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 12: //ABX gDry
            if(u8value > 24) u8value = 24;
            DBG_PRINT_P1(printf("ABX gDry %1d\r\n",u8value));
            HAL_SOUND_SetPara(0x28,0x84,ABX_Gain_TBL[u8value][0],ABX_Gain_TBL[u8value][1],ABX_Gain_TBL[u8value][2]);
            break;
        case 13: //ABX FILSET
            if(u8value > 1) u8value = 1;
            DBG_PRINT_P1(printf("FILSET %1d\r\n",u8value+1));
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


        if(type == AU_DVB2_ADVSND_SRS)      //TSXT
        {
            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_SRS_SEL, 0x01, 0x01);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_EN, TRUE, 0);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_EN, TRUE, 0);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_SURR_EN, TRUE, 0);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN, TRUE, 0);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN, TRUE, 0);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN, TRUE, 0);
            status = TRUE;
        }
        else if((type == AU_DVB2_ADVSND_TSHD) ||(type == AU_DVB2_ADVSND_THEATERSOUND) ||(type == AU_DVB2_ADVSND_STUDIOSOUND_3D))
        {
            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_SRS_SEL, 0x01, 0x01);
            HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_EN, TRUE, 0);
            if(type == AU_DVB2_ADVSND_TSHD)
            {
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_EN, TRUE, 0);
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_SURR_EN, TRUE, 0);
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN, TRUE, 0);
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DEFINITION_FRONT_EN, TRUE, 0);
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN, TRUE, 0);
                HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN, TRUE, 0);
            }
            if(type == AU_DVB2_ADVSND_STUDIOSOUND_3D)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
                HAL_AUDIO_Alg2ReloadCode(AU_DVB2_ADVSND_PURESND);
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            }
            status = TRUE;
        }
        else if(type == AU_DVB2_ADVSND_PURESND)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
            ret_status = HAL_AUDIO_Alg2ReloadCode(type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
            status = TRUE;
        }
        else if(type == AU_DVB2_ADVSND_DBX)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
            ret_status = HAL_AUDIO_Alg2ReloadCode(type);
            if( ret_status == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
                HAL_SOUND_DBX_Enable(TRUE);
            }
            HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
            status = TRUE;
        }
        else if(type == AU_DVB2_ADVSND_BONGIOVI_DPS)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,0x00);         //Reset the advsnd mode setting
            ret_status = HAL_AUDIO_Alg2ReloadCode(type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,type);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x80);
            HAL_AUDIO_SetAdvSndEffDspCodeType(type, ret_status);
            status = TRUE;
        }
        else if (type == AU_DVB2_ADVSND_SONICMOTION_ABS3D)
        {
            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_ABS3D_SEL, 0x01, 0x01);
            if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT0_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC0_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT1_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC1_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT2_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC2_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT3_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC3_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_I2S_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_I2S0_EN_BIT), 0);
            }
            status = TRUE;
        }
        else if(type == AU_DVB2_ADVSND_MAX)
        {
           // HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_SRS_SEL, 0x01, 0x00);  //This setting will reset all SRS parameter
           HAL_ADVSOUND_SubProcessEnable(SRS_THEATERSOUND_TOTAL, 0);    //Disable all SRS function
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0xFF,0x00);
            status = TRUE;
        }
        else
        {
            DBG_PRINT_P1(printf("\n\033[0;34m-----------Error : IP Security Checking Fail !!! type=%d------------\033[0m \n", type);)
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

    //SRS_THEATERSOUND
    if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_3D_EN) || ADVFUNC_Compare(proc, SRS_THEATERSOUND_TOTAL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_EN, (MS_U16)enable, 0);
        status = true;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_SURR) || ADVFUNC_Compare(proc, SRS_TSHD_SRS3D))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_SURR_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_DEPTH_PROCESS_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_3D_SURR_BOOST_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_FADE_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_FADE_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_TSHD_MIX_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TSHD_MIX_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_TBHDX_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_GEQ_EN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_TSXT_TRUBASS) || ADVFUNC_Compare(proc, SRS_TSHD_TRUBASS) || ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUBASS))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_FRONT_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_TSXT_DC) || ADVFUNC_Compare(proc, SRS_TSHD_DC) || ADVFUNC_Compare(proc, SRS_THEATERSOUND_DC))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DIALOG_CLARITY_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_TSHD_DEFINITION) || ADVFUNC_Compare(proc, SRS_THEATERSOUND_DEFINITION))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DEFINITION_FRONT_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_LEVEL_INDPT))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_LEVEL_INDEPENDENT_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_HARDLIMITER))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HL_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_HPF))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HPF_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_HPF_END))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HPF_END_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUEQ))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_AEQ_PEQ_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_VIQ_NOISE_MNGR_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_CS))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUDIALOG))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_NORMALIZER))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_NORMALIZER_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_SMOOTH))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_SMOOTH_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    //AUDYSSEY
    else if (ADVFUNC_Compare(proc, AUDYSSEY_DYNAMICVOL))
    {
        HAL_SOUND_ADV_SetPara(2,(MS_U8)enable);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, AUDYSSEY_DYNAMICEQ))
    {
        HAL_SOUND_ADV_SetPara(3,(MS_U8)enable);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, AUDYSSEY_ABX))
    {
        HAL_SOUND_ADV_SetPara(10,(MS_U8)enable);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVFUNC_Compare(proc, SRS_PURESOUND_HL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_PURESOUND_AEQ))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SRS_PURESOUND_HPF))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HPF_EN, (MS_U16)enable, 0);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, BONGIOVI_DPS_SURR_EN))
    {
        MS_U32 dps_param_update_count;
        MS_BOOL stereoWidenBypass;

        if (enable)
            stereoWidenBypass = FALSE;
        else
            stereoWidenBypass = TRUE;
        HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_STEREO_WIDENER_ADDR, (MS_U32)stereoWidenBypass, DSP_MEM_TYPE_PM);
        dps_param_update_count = HAL_MAD2_Read_DSP_sram(BONGIOVI_DPS_UPDATE_COUNT_ADDR, DSP_MEM_TYPE_PM);
        dps_param_update_count = dps_param_update_count+1;
        HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_UPDATE_COUNT_ADDR, dps_param_update_count, DSP_MEM_TYPE_PM);
        status = TRUE;
    }
    else if (ADVFUNC_Compare(proc, SONICEMOTION_ABS3D_EN))
    {
        if(enable)
        {
            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_ABS3D_SEL, 0x01, 0x01);
            if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT0_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC0_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT1_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC1_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT2_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC2_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT3_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC3_EN_BIT), 0);
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_I2S_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_I2S0_EN_BIT), 0);
            }
        }
        else
        {
            HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_ABS3D_SEL, 0x01, 0x00);
            if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT0_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC0_EN_BIT), 1<< (M2S_MBOX_VOL_DAC0_EN_BIT));
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT1_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC1_EN_BIT), 1<< (M2S_MBOX_VOL_DAC1_EN_BIT));
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT2_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC2_EN_BIT), 1<< (M2S_MBOX_VOL_DAC2_EN_BIT));
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_AUOUT3_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_DAC3_EN_BIT), 1<< (M2S_MBOX_VOL_DAC3_EN_BIT));
            }
            else if (g_AudioVars2->AudioOutputInfo.SpeakerOut == AUDIO_I2S_OUTPUT)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 1<< (M2S_MBOX_VOL_I2S0_EN_BIT), 1<< (M2S_MBOX_VOL_I2S0_EN_BIT));
            }
        }
        status = TRUE;
    }
    else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\n\033[0;34m-----------Error : HAL_ADVSOUND_SubProcessEnable!!! Unknown type=%s, enable=%d------------\033[0m \n", proc, enable);

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

    //BBE
    if (ADVSND_PARAM_Compare(param, BBE_MODE_SET))
    {
        HAL_SOUND_BBE_SetBbeMode(u8value1, 0);
        status = TRUE;
    }
    //DOLBY
    else if (ADVSND_PARAM_Compare(param, DOLBY_PL2VDPK_SMOD))
    {
        HAL_SOUND_VSPK_SetSMod(u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, DOLBY_PL2VDPK_WMOD))
    {
        HAL_SOUND_VSPK_SetWMod(u8value1);
        status = TRUE;
    }
    //SRS_THEATERSOUND_HL
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_HARDLIMITER_LEVEL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HL_LIMIT_LEVEL_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_HL
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_HARDLIMITER_BOOST_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_HL_BOOST_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_MODE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_VOLUME_MODE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_MAX_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_MAXGAIN_CONTROL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD NOT USED
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR_THLD))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_VIQ_NOISE_MNGR_THLD,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD NOT USED
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_CALIBRATE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_VIQ_CALIBRATE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VIQ_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_REF_LEVEL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_LKFS,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TOLHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUVOLUME_NORMALIZE_THRESH))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_VOLHD_NORMTHRESH,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TSHD_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TSHD_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_SURR_LEVEL_CONTROL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_SURROUND_LEVEL_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_DEFINITION_CONTROL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DEFINITION_FRONT_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_DC_CONTROL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_DIALOG_CLARITY_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUBASS_CONTROL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_FRONT_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_SPEAKERSIZE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_SIZE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUBASS_COMPRESSOR_CONTROL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_COMPRESSOR_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUBASS_PROCESS_MODE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_PROCESS_MODE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUBASS_SPEAKER_AUDIO))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_AUDIO,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TSHD
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUBASS_SPEAKER_ANALYSIS))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TSHD_TRUBASS_SPEAKER_ANALYSIS,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_APERTURE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_APERTURE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_GAINLIMIT))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GAINLIMIT,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_FF_DEPTH))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_FF_DEPTH,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_NF_DEPTH))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_NF_DEPTH,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TSHD_MIX_FADE_CTRL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TSHD_MIX_FADE_CTRL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_BASSLEVEL))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_BASSLEVEL,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_SPEAKERSIZE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_SPEAKERSIZE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_MODE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_MODE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_DYNAMICS))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_DYNAMICS,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_HP_ORDER))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_HP_ORDER,u32value1, u16value2);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_TBHDX_CUSTOM_FILTER))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_TBHDX_CUSTOM_FILTER,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_BAND0_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_BAND0_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_BAND1_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_BAND1_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_BAND2_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_BAND2_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_BAND3_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_BAND3_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CC3D
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_GEQ_BAND4_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_GEQ_BAND4_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CC3D_PROCESS_PATH))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CC3D_PROCESS_PATH,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND NOT USED
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_HPF_FC))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_AEQ_HPF_FC,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_HEADROOM_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_HEADROOM_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_INPUT_MODE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_INPUT_MODE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CS
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CS_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CS
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CS_PROCESS_MODE))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_PROCESSING_MODE,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CS
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CS_LR_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_LR_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CS
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CS_LsRs_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_LSRS_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_CS
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_CS_Center_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_CS_CENTER_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TRUDIALOG
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUDIALOG_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TRUDIALOG
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUDIALOG_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TRUDIALOG
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUDIALOG_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TRUDIALOG
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUDIALOG_PROCESS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_PROCESS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS_THEATERSOUND_TRUDIALOG
    else if (ADVSND_PARAM_Compare(param, SRS_THEATERSOUND_TRUDIALOG_CLARITY_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_THEATER_TRUDIALOG_CLARITY_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //AUDYSSEY
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_DYNAMICVOL_COMPRESS_MODE))
    {
        HAL_SOUND_ADV_SetPara(5,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_DYNAMICVOL_GC))
    {
        HAL_SOUND_ADV_SetPara(6,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_DYNAMICVOL_VOLSETTING))
    {
        HAL_SOUND_ADV_SetPara(8,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_DYNAMICEQ_EQOFFSET))
    {
        HAL_SOUND_ADV_SetPara(7,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_ABX_GWET))
    {
        HAL_SOUND_ADV_SetPara(11,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_ABX_GDRY))
    {
        HAL_SOUND_ADV_SetPara(12,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, AUDYSSEY_ABX_FILSET))
    {
        HAL_SOUND_ADV_SetPara(13,u8value1);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, DBX_MODE_SET))
    {
        HAL_SET_DBX_MODE(u8value1);
        status = FALSE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_LIMITERBOOST))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_LIMITERBOOST,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_HARDLIMIT))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_HARDLIMIT,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HL_DELAYLEN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HL_DELAYLEN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_AEQ_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_AEQ_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_AEQ_BYPASS_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_AEQ_BYPASS_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_FILTER_CONFIG))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_FILTER_CONFIG,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_HPF_FREQUENCY))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_HPF_FREQUENCY,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_INPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_INPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    //SRS PURESOUND
    else if (ADVSND_PARAM_Compare(param, SRS_PURESOUND_OUTPUT_GAIN))
    {
        HAL_SOUND_SRS_APOLLO_SetPara(AU_SRS_PURESOUND_OUTPUT_GAIN,u32value1, u16value2);
        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, BONGIOVI_DPS_SET_GLOBAL_INPUT_GAIN))
    {
        MS_S16 s16dBGainValue;

        // Input gain (global input gain) -range -20dB to +12dB in 1dB increments
        s16dBGainValue = (MS_S16)u32value1;   // cast to signed type for below range check (-20~12).
        if (s16dBGainValue > 12)
            s16dBGainValue = 12;
        else if (s16dBGainValue < -20)
            s16dBGainValue = -20;
        u32value1 = s16dBGainValue;           // cast to unsigned type again.

        u32value1 = u32value1 - 12;
        u32value1 = -u32value1;
        HAL_MAD2_Write_DSP_sram(BONGIOVI_DPS_GLOBAL_INPUT_GAIN_CONTROL_ADDR, u32value1, DSP_MEM_TYPE_PM);

        status = TRUE;
    }
    else if (ADVSND_PARAM_Compare(param, BONGIOVI_DPS_SET_GLOBAL_OUTPUT_GAIN))
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
    }
    else if (ADVSND_PARAM_Compare(param, BONGIOVI_DPS_SET_STEREO_WIDENER_MGAIN))
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
    }
    else if (ADVSND_PARAM_Compare(param, BONGIOVI_DPS_SET_STEREO_WIDENER_SGAIN))
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
    }
    else
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "\n\033[0;34m-----------Error : HAL_ADVSOUND_SetParam!!! Unknown param=%s, u32value1=0x%x------------\033[0m \n", param, (int)u32value1);
        status = FALSE;
    }
    return (status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description:  This function is used to copy user parameters to hal
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size)
{
    if (ADVSND_PARAM_Compare(type, DBX_LOAD_DM) ||
        ADVSND_PARAM_Compare(type, DBX_LOAD_PM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotSonDM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotSonPM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotVolDM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotVolPM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotSurDM) ||
        ADVSND_PARAM_Compare(type, DBX_COPY_TotSurPM))
    {
        if (HAL_AUDIO_GetAdvSndEffDspCodeType() == AU_DVB2_ADVSND_DBX)
            // The input argument "size" for MApi_AUDIO_COPY_Parameter is byte size, but the "size" for HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP for DBX is integer size. So size needs to be divided by sizeof(MS_U32).
            HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP(type, (MS_U32 *)Parameter_ptr, size/sizeof(MS_U32));
    }
    else if (ADVSND_PARAM_Compare(type, BONGIOVI_DPS_SET_PARAMS))
    {
        if (HAL_AUDIO_GetAdvSndEffDspCodeType() == AU_DVB2_ADVSND_BONGIOVI_DPS)
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
    }
    else if (ADVSND_PARAM_Compare(type, SONICEMOTION_ABS3D_SET_PARAMS))
    {
        if (HAL_AUDIO_GetAdvSndEffDspCodeType() == AU_DVB2_ADVSND_SONICMOTION_ABS3D)
        {
            MS_U32 offset, size;
            MS_U16 param1,param2;

            size = HAL_SND_R2_GetCommInfo(AUR2_CommParam_AASE_SONICEMOTION_ABS3D_GET_PARAMS_SIZE, AASE1, 0, 0);
            offset = HAL_SND_R2_GetCommInfo(AUR2_CommParam_AASE_SONICEMOTION_ABS3D_GET_PARAMS_R2_ADDR, AASE1, 0, 0);
            param1 = (offset >> 16) & 0xffff;
            param2 = offset & 0xffff;
#ifdef MSOS_TYPE_LINUX_KERNEL
            memcpy_toio((void *)MsOS_PA2KSEG1(HAL_AUDIO_GetDspMadBaseAddr(2)+ ADEC__R2_DDR_SIZE + offset), Parameter_ptr, size);
#else
            memcpy((void *)MsOS_PA2KSEG1(HAL_AUDIO_GetDspMadBaseAddr(2)+ ADEC__R2_DDR_SIZE + offset), Parameter_ptr, size);
#endif
            MsOS_FlushMemory();
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_SONICEMOTION_ABS3D_SET_PARAMS, AASE1, param1, param2);
        }
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
           HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x80);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x10, 0x00);      //Bypass  I2S volumne
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
           HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x40);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x01, 0x00);      //Bypass  volumne
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
           HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x20);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x02, 0x00);      //Bypass  volumne
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
           HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x10);         //set dbx refrence to path main
           MsOS_DelayTask(5);
           HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x04, 0x00);      //Bypass  volumne
           HAL_AUDIO_WriteByte(REG_SOUND_AUOUT2_VOLUME, tmp_volume);          //un-mute
        }
    }
    else
    {
        if(DBX_PATH == AUDIO_I2S_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_I2S_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x10, 0x10);          //enable I2S volumne
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_I2S_VOLUME, tmp_volume);          //un-mute

        }
        else if(DBX_PATH == AUDIO_AUOUT0_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT0_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x01, 0x01);          //enable DAC0 volumne
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT0_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT1_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT1_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x02, 0x02);          //enable DAC1 volumne
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT1_VOLUME, tmp_volume);          //un-mute
        }
        else if(DBX_PATH == AUDIO_AUOUT2_OUTPUT)
        {
            tmp_volume = HAL_AUDIO_ReadByte(REG_SOUND_AUOUT2_VOLUME);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x80);          //mute
            MsOS_DelayTask(20);
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x04, 0x04);          //enable DAC2 volumne
            MsOS_DelayTask(5);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2,0xFF,00);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG1,0x80,0x00);
            MsOS_DelayTask(1);
            HAL_AUDIO_WriteMaskByte(MB_2D44+1, 0xF0, 0x00);         //bypass volume in DBX
            HAL_AUDIO_WriteByte(REG_SOUND_AUOUT2_VOLUME, tmp_volume);          //un-mute
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

//    HAL_AUDIO_WriteMaskByte(0x2D01, 0x80, 0x80);          //mute
    MsOS_DelayTask(20);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2, 0x1F, 0x00);          //disable DBX
    MsOS_DelayTask(5);

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
        HAL_MAD2_Write_DSP_sram(DBX_DM_ABS_ADDRESS + DM_OFFSET_IWBypass, 0x000001, DPP_LENGTH_DM);
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

//    HAL_AUDIO_WriteMaskByte(0x2D01, 0x80, 0x00);          //unmute
    MsOS_DelayTask(5);
    HAL_AUDIO_WriteMaskByte(REG_SOUND_ADV_CFG2, 0x1F, 0x1F);          //enable DBX

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



void HAL_SOUND_DBX_COPY_PM(MS_U32 *PM_ptr,MS_U32 size)
{
    PM_ptr = PM_ptr;
}


void HAL_SOUND_DBX_COPY_DM(MS_U32 *DM_ptr,MS_U32 size)
{
    DM_ptr = DM_ptr;
}

void HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP(ADVSND_PARAM type, MS_U32 *Parameter_ptr,MS_U32 size)
{
    MS_U32 i=0;
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
    MS_U32* tmp_ptr=(void *)&TotSonDM[0];

    if (ADVSND_PARAM_Compare(type, DBX_COPY_TotSonDM))
        tmp_ptr = (void *)&TotSonDM[0];
    else if (ADVSND_PARAM_Compare(type, DBX_COPY_TotSonPM))
        tmp_ptr = (void *)&TotSonPM[0];
    else if (ADVSND_PARAM_Compare(type, DBX_COPY_TotVolPM))
        tmp_ptr = (void *)&TotVolPM[0];
    else if (ADVSND_PARAM_Compare(type, DBX_COPY_TotVolDM))
        tmp_ptr = (void *)&TotVolDM[0];
    else if (ADVSND_PARAM_Compare(type, DBX_COPY_TotSurDM))
        tmp_ptr = (void *)&TotSurDM[0];
    else if (ADVSND_PARAM_Compare(type, DBX_COPY_TotSurPM))
        tmp_ptr = (void *)&TotSurPM[0];

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
int HAL_Copy(MS_U32 *ptr_data, MS_U32 Address, MS_U32 Length)
{
#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
    MS_U8   dat[3],i;

    if(HAL_AUDIO_GetDSPalive() == 0)
        return 0;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    HAL_AUDIO_AbsWriteMaskByte(0x103c14, 0x01, 0x00);//temp:need to replace function.
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

