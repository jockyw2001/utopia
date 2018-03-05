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
//******************************************************************************
//  Copyright (c) 2008-2009 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      drvMAD2.c
//  [Abstract]:
//      This module contains code for Audio 2nd DSP driver
//      procedure and subroutin
//  [Reversion History]:
//      Initial release:    15 July, 2005
//
//  [Doxygen]
/// file drvMAD2.c
/// @brief Subroutine for Audio 2nd DSP
/// @author MStarSemi Inc.
//*******************************************************************************

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

// Internal
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvSOUND.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "regAUDIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define H2BYTE(value)             ((MS_U8)((value) / 0x10000))

extern MS_U8  g_Dsp2CodeType;
extern MS_U32 MadBaseBufferAdr;

//Temp mask by Cathy extern MS_U8 CheckIPControlBit(MS_U8  u8Bit);


MS_U8  DVB2_IP_CONTROL_BIT[] =
{
    255,                         //MPEG_AD
    11,                          //AC3_AD
    12,                          //AC3P_AD
    255,                         //AAC_AD
    13,                          //DDE
    255,                         //SBC
    255,                         //MP3
    255,                         //MPEG_EN
    255,                         //DTSE
    255,                         //XPCM
    255,                         //KTV
    255,                         //KTV2
    255,                         //NONE
};

//======================================================================
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the SE-DSP memory information
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetMemInfo(void)
{
    HAL_MAD2_SetMemInfo();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder command.
/// @param u8DecCmd    \b system command    :
///                                         AU_DVB2_DECCMD_STOP,           //0
///                                         AU_DVB2_DECCMD_PLAY,           //1
///                                         AU_DVB2_DECCMD_PLAYFILETSP = 2,
///                                         AU_DVB2_DECCMD_RESYNC,
///                                         AU_DVB2_DECCMD_PLAYFILE = 4,
///                                         AU_DVB2_DECCMD_PAUSE = 6
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD u8DecCmd)
{
    HAL_MAD2_SetDecCmd((AU_DVB_DECCMD)u8DecCmd);
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_GetDecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB2 decode command.
/// @return AU_DVB_DECCMD \b command type:
///                                 AU_DVB_DECCMD_STOP,      //0
///                                 AU_DVB_DECCMD_PLAY,      //1
///                                 AU_DVB_DECCMD_PLAYFILETSP = 2,
///                                 AU_DVB_DECCMD_RESYNC,
///                                          ....etc
///////////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD MDrv_MAD2_GetDecCmd(void)
{
    return (AU_DVB_DECCMD) (HAL_MAD2_GetDecCmd()&0xF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetFreeRun()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder in free run or AV-sync mode.
/// @param u8FreeRun   \b AU_DVB2_FreeRunMode_AVsync  0--normal AV sync
///                       AU_DVB2_FreeRunMode_FreeRun 1--free-run mode
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode u8FreeRun)
{
    HAL_MAD2_SetFreeRun((MS_U8)u8FreeRun);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_RebootDsp(void)
/// @brief \b Function \b Description:  This routine reboot Sound Effect DSP.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_RebootDsp(void)
{
    HAL_MAD2_RebootDsp();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_DisEn_MIUREQ(void)
{
    HAL_MAD2_DisEn_MIUREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_DisMiuReq(void)
{
    HAL_MAD2_DisMiuReq();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_RSTMAD_DisEn_MIUREQ(void)
{
    HAL_MAD2_RSTMAD_DisEn_MIUREQ();

}



////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value for int status by IDMA
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value for interrupt status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_MAD2_XPCM2_CheckIntStatus(void)
{
    return (MS_U16)HAL_MAD2_Read_DSP_sram(0x1494, DSP_MEM_TYPE_DM);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_XPCM_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType   \b type: 1--lpcm
///                             2--MS-ADPCM
///                             17--IMA-ADPCM
/// @param Channels    \b channel num: 0-- mono
///                                    1-- stereo
/// @param sampleRate  \b sample rate: 8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param bitsPerSample \b bit per sample:  8--8 bits
///                                         16--16 bits
/// @param blockSize \b block size
/// @param samplePerBlock \b samples per block
/// @return MS_U8    \b TRUE--parameters are correct
///                     FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_XPCM_setParam (XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
   HAL_MAD2_XPCM_setParam (audioType,channels, sampleRate,
                                                     bitsPerSample, blockSize, samplePerBlock);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_Read_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value by IDMA
/// @param <IN>        \b u16Dsp_addr    : DSP internal sram address
/// @param <IN>        \b dm    :    0--DSP_MEM_TYPE_PM
///                                1--DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_MAD2_Read_DSP_sram(MS_U16 u16Dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return (HAL_MAD2_Read_DSP_sram(u16Dsp_addr, dsp_memory_type));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_Write_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Write DSP internal sram value by IDMA
/// @param <IN>        \b u16Dsp_addr    : DSP internal sram address
/// @param <IN>        \b u32Value     : data want to write
/// @param <IN>        \b dm        :    0-- write to DSP_MEM_TYPE_PM
///                                        1-- write to DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MAD2_Write_DSP_sram(MS_U16 u16Dsp_addr, MS_U32 u32Value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return(HAL_MAD2_Write_DSP_sram(u16Dsp_addr, u32Value, dsp_memory_type));
}


