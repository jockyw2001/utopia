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
//  [for Doxygen]
/// file drvADVSOUND.c
/// @brief Subroutine for advance sound effect
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvBDMA.h"

#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvADVSOUND.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "regAUDIO.h"
#include "drvBDMA.h"
#include "halAUDIO.h"
#include "halADVAUDIO.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AseSetBinAddress()
/// @brief \b Function \b Description:  This function is used to assign Bin address of advanced sound effect
/// @param u8Index     \b : type index
/// @param BinAddr     \b : address of Bin file
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr)
{
    HAL_SOUND_AseSetBinAddress(u8Index, BinAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetPara()
/// @brief \b Function \b Description:  This function is used to write paramters value into SRAM
/// @param u8Addr1     \b : middle byte of address
/// @param u8Addr2     \b : low byte of address
/// @param u8Value1    \b : high byte of value
/// @param u8Value2    \b : middle byte of value
/// @param u8Value3    \b : low byte of value
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SetPara(MS_U8 u8Addr1 , MS_U8 u8Addr2, MS_U8 u8Value1, MS_U8 u8Value2, MS_U8 u8Value3)
{
    HAL_SOUND_SetPara(u8Addr1 , u8Addr2, u8Value1, u8Value2, u8Value3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_TruSurr()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD
/// @param u8TSHDenTruSurr \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_TruSurr(MS_U8 u8TSHDenTruSurr)
{
    HAL_SOUND_TSHD_TruSurr(u8TSHDenTruSurr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_Limiter()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Limiter
/// @param u8TSHDenLimiter \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_Limiter(MS_U8 u8TSHDenLimiter)
{
    HAL_SOUND_TSHD_Limiter(u8TSHDenLimiter);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_SRS3D()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param u8TSHDenSRS3D \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_SRS3D(MS_U8 u8TSHDenSRS3D)
{
    HAL_SOUND_TSHD_SRS3D(u8TSHDenSRS3D);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TruVolume_Enable()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param u8TSHDenTVol \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TruVolume_Enable(MS_U8 u8TSHDenTVol)
{
    HAL_SOUND_TruVolume_Enable(u8TSHDenTVol);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_ADSY_SetPara()
/// @brief \b Function \b Description:  This function is used to set parameter
/// @param u8mode      \b :
/// @param u8value     \b :
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_ADSY_SetPara(MS_U8 u8mode, MS_U8 u8value)
{
    HAL_SOUND_ADSY_SetPara(u8mode, u8value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, u32value1, u16value2
/// @param <OUT>       \b NONE    :
/// @return <OUT>       \b FALSE : Setting Fail
///                               \b TRUE  : Setting OK
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
    return(HAL_ADVSOUND_SetParam(param, u32value1, u16value2));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description:  This function is used to copy user parameters to hal
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_COPY_Parameter(ADVSND_PARAM type, void * Parameter_ptr, MS_U32 size)
{
    HAL_AUDIO_COPY_Parameter(type, Parameter_ptr, size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DBXTV_SetVolPath(AUDIO_PATH_TYPE path)
/// @brief \b Function \b Description: This routine is used to set dbx TV volume path
/// @param <IN>        \b param, AUDIO_PATH_TYPE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MDrv_DBXTV_SetVolPath(AUDIO_OUTPUT_TYPE path){
        Hal_DBXTV_SetVolPath(path);
}
