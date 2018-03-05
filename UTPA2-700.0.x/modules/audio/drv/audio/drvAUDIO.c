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
///////////////////////////////////////////////////////////////////////////////
#define _DRVAUDIO_C

///////////////////////////////////////////////////////////////////////////////
/// file   drvAUDIO.c
/// @author MStar Semiconductor Inc.
/// @brief   Audio common driver
///////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#else
#include <string.h>
#endif

#include "ULog.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "drvSYS.h"
#include "drvBDMA.h"
// Internal Definition
#include "drvMMIO.h"

#include "drvAUDIO.h"
#include "drvAUDIO_if.h"
#include "./internal/drvAUDIO_internal.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvSOUND.h"
#include "./internal/drvSIF.h"
#include "./internal/drvAudio_Common.h"

#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halAUDIO.h"
#include "regAUDIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DRVAUDIO_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if ( MDrv_AUDIO_SHM_Init() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

static MSIF_Version _drv_aud_version = {
    .DDI = { AUD_DRV_VERSION, },
};

static MS_BOOL _bMMIO_Init = FALSE;
static MS_BOOL _PreInit = FALSE;
static MS_VIRT _gu32AudioRiuBaseAddr;

// Global variables for VoIP applications to register the CallBack.
static MS_VIRT (*audio_dec_app_callbk)(MS_U8) = NULL;
static MS_VIRT (*audio_se_app_callbk)(MS_U8) = NULL;

AUDIO_DEC_ID (* gOpenDecodeSystemFuncPtr)(AudioDecStatus_t * p_AudioDecStatus) = NULL;
MS_BOOL (* gDigitalOutChannelStatusFuncPtr)(DIGITAL_OUTPUT_TYPE , Digital_Out_Channel_Status_t *) = NULL;
MS_PHY (* gGetDDRInfoFuncPtr)(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo) = NULL;

#define AUDIO_ASSERT(_cnd, _fmt, _args...)                                      \
				do {                                                            \
                    if (!(_cnd)) {                                              \
                        MS_ASSERT(0);                                           \
                    }                                                           \
                } while(0)

#if defined (MSOS_TYPE_NOS)
    #define MDRV_FIQ_BEGIN          0x00
    #define FIQ_DEC_DSP2UP          (MDRV_FIQ_BEGIN + 29)
    #define FIQ_SE_DSP2UP           (MDRV_FIQ_BEGIN + 14)
    void MDrv_AUDIO_DecDspISR(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data);
    void MDrv_AUDIO_SeDspISR(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data);
#else // Ecos and Linux
    void MDrv_AUDIO_DecDspISR( InterruptNum eIntNum );
    void MDrv_AUDIO_SeDspISR( InterruptNum eIntNum );
#endif

MS_U8  g_Dsp2CodeType;
AU_CHIP_TYPE gAudioChipType;
MS_BOOL bIsNonPCMInDec2 = TRUE;
AUDIO_SHARED_VARS2 * g_AudioVars2 = NULL;

extern MS_S32  _s32AUDIOMutexReboot;
extern MS_BOOL g_bDSPLoadCode;
extern MS_U8 g_Common_PCM_IO_Init;
extern AUDIO_PCM_t *g_PCM[AUDIO_PCM_MAXNUM];

extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO Basic Read/Write Drv Function
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    return(HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus(p_hdmiAudioSts));
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUD_GetLibVer()
/// @brief \b Function \b Description:  This function is used to get version information for audio module
/// @return   MS_BOOL  \b : TRUE or FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUD_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_aud_version;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ReadReg()
/// @brief \b Function \b Description:  This function is used to read the general register value
/// @param  u32RegAddr \b : Register address
/// @return MS_U16      \b : Register value
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_ReadReg(MS_U32 u32RegAddr)
{
    return (HAL_AUDIO_ReadReg(u32RegAddr));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteReg()
/// @brief \b Function \b Description:  This function is used to write value to general register
/// @param u32RegAddr  \b : Register address
/// @param u16Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16  u16Val)
{
    HAL_AUDIO_WriteReg(u32RegAddr, u16Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteMaskReg()
/// @brief \b Function \b Description:  This function is used to write mask value to general register
/// @param u32RegAddr  \b : Register address
/// @param u16Mask      \b : Mask value
/// @param u16Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16  u16Mask, MS_U16  u16Val)
{
    HAL_AUDIO_WriteMaskReg(u32RegAddr, u16Mask, u16Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ReadByte()
/// @brief \b Function \b Description:  This function is used to read the general register value
/// @param  u32RegAddr \b : Register address
/// @return MS_U8      \b : Register value
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_ReadByte(MS_U32 u32RegAddr)
{
    return (HAL_AUDIO_ReadByte(u32RegAddr));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteByte()
/// @brief \b Function \b Description:  This function is used to write value to general register
/// @param u32RegAddr  \b : Register address
/// @param u8Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8  u8Val)
{
    HAL_AUDIO_WriteByte(u32RegAddr, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteMaskByte()
/// @brief \b Function \b Description:  This function is used to write mask value to general register
/// @param u32RegAddr  \b : Register address
/// @param u8Mask      \b : Mask value
/// @param u8Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val)
{
    HAL_AUDIO_WriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecReadByte()
/// @brief \b Function \b Description:  This function is used to read the Dec-DSP register value
/// @param u32RegAddr  \b : Register address
/// @return MS_U8      \b : Register value
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_DecReadByte(MS_U32 u32RegAddr)
{
    return (HAL_AUDIO_DecReadByte(u32RegAddr));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecWriteByte()
/// @brief \b Function \b Description:  This function is used to write value to Dec_DSP register
/// @param u32RegAddr  \b : Register address
/// @param u8Val       \b : Register value
///////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DecWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    HAL_AUDIO_DecWriteByte(u32RegAddr, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SeReadByte()
/// @brief \b Function \b Description:  This function is used to read the Se-DSP register value
/// @param u32RegAddr  \b : Register address
/// @return MS_U8      \b : Register value
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_SeReadByte(MS_U32 u32RegAddr)
{
    return (HAL_AUDIO_SeReadByte(u32RegAddr));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SeWriteByte()
/// @brief \b Function \b Description:  This function is used to write value to Se_DSP register
/// @param u32RegAddr  \b : Register address
/// @param u8Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SeWriteByte(MS_U32 u32RegAddr, MS_U8  u8Val)
{
    HAL_AUDIO_SeWriteByte(u32RegAddr, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecWriteMaskByte()
/// @brief \b Function \b Description:  This function is used to write mask value to Dec_DSP register
/// @param u32RegAddr  \b : Register address
/// @param u8Mask      \b : Mask value
/// @param u8Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DecWriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val)
{
    HAL_AUDIO_DecWriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SeWriteMaskByte()
/// @brief \b Function \b Description:  This function is used to write mask value to Se_DSP register
/// @param u32RegAddr  \b : Register address
/// @param u8Mask      \b : Mask value
/// @param u8Val       \b : Register value
////////////////////////////////////////////////////////////////////////////////

void MDrv_AUDIO_SeWriteMaskByte(MS_U32 u32RegAddr, MS_U8  u8Mask, MS_U8  u8Val)
{
    HAL_AUDIO_SeWriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_CheckBootOnInitState()
/// @brief \b Function \b Description:  This routine is used to check whether mboot load init script
/// @param void
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_CheckBootOnInitState(void)
{
    return HAL_AUDIO_CheckBootOnInitState();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to read the Dec-DSP mail box value
/// @param u8ParamNum  \b : Mail box address
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_ReadDecMailBox(MS_U8 u8ParamNum)
{
    return(HAL_AUDIO_ReadMailBox(DSP_DEC, u8ParamNum));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to write Dec-DSP mail box
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
void _MDrv_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{
    HAL_AUDIO_WriteMailBox(DSP_DEC, u8ParamNum, u16Data);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteSeMailBox()
/// @brief \b Function \b Description:  This routine is used to read the Dec-DSP mail box value
/// @param u8ParamNum  \b : Mail box address
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_ReadSeMailBox(MS_U8 u8ParamNum)
{
    return(HAL_AUDIO_ReadMailBox(DSP_SE, u8ParamNum));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteSeMailBox()
/// @brief \b Function \b Description:  This routine is used to write Dec-DSP mail box
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteSeMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{
    HAL_AUDIO_WriteMailBox(DSP_SE, u8ParamNum, u16Data);
}



//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO Initialize Relational Drv Function
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_InitMMIO()
/// @brief \b Function \b Description:  This routine is used to init audio MMIO
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_InitMMIO(void)
{
    MS_PHY u32NonPMBankSize;

#ifndef CONFIG_MBOOT
    if(_bMMIO_Init==TRUE)
        return;
#endif

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &_gu32AudioRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_AUDIO ) != TRUE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s\n", "Audio get base address failed\n");
        return;
    }
    else
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG,  "MDrv_AUDIO_Init u32PnlRiuBaseAddr = %x\n", (unsigned int)_gu32AudioRiuBaseAddr);
    }
    HAL_AUDIO_Set_MMIO_BASE(_gu32AudioRiuBaseAddr);

    _bMMIO_Init = TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_AttachInterrupt()
/// @brief \b Function \b Description:  This routine is used to attach/detach audio interrupts
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_AttachInterrupt(MS_BOOL bEnable)
{
    /*
     * Note!
     * HAL_AUDIO_Init() currently is designed to check if SE DSP is supported or not,
     * so we need to call this function here!
     */
    MS_BOOL has_SE_DSP = HAL_AUDIO_Init(TRUE);

    /*
     * Although MsOS_XXXInterrupt() can check if it's "Success" or "NOT Success",
     * but we don't handle the failed case here!
     */
    if (bEnable)
    {
        MsOS_AttachInterrupt(E_INT_FIQ_DEC_DSP2UP, (InterruptCb)MDrv_AUDIO_DecDspISR);
        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
        if (has_SE_DSP)
        {
            MsOS_AttachInterrupt(E_INT_FIQ_SE_DSP2UP, (InterruptCb)MDrv_AUDIO_SeDspISR);
            MsOS_EnableInterrupt(E_INT_FIQ_SE_DSP2UP);
        }
    }
    else
    {
        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);
        MsOS_DetachInterrupt(E_INT_FIQ_DEC_DSP2UP);
        if (has_SE_DSP)
        {
            MsOS_DisableInterrupt(E_INT_FIQ_SE_DSP2UP);
            MsOS_DetachInterrupt(E_INT_FIQ_SE_DSP2UP);
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_Mutex_Init()
/// @brief \b Function \b Description: This routine is the initialization of Mutex for Audio module.
/// @param u8Standard  \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_Mutex_Init(void)
{
    MS_BOOL ret = TRUE;

    ret = HAL_AUDIO_Mutex_Init();

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SHM_Init()
/// @brief \b Function \b Description: This routine is the initialization of SHM for Audio module.
/// @param u8Standard  \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SHM_Init(void)
{
    MS_BOOL ret = TRUE;

    ret = MDrv_AUDIO_InitialVars();

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_Init()
/// @brief \b Function \b Description: This routine is the initialization for Audio module.
/// @param u8Standard  \b : Load Decoder type in audio initialize
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_Init(void)
{
    gAudioChipType = HAL_AUDIO_GetChipType();

    MDrv_AUDIO_InitMMIO();

    HAL_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
    HAL_AUDIO_SetPlayFileFlag(DSP_SE, 0);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SET_INIT_FLAG()
/// @brief \b Function \b Description: This routine set the initial status of Aduio module.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SET_INIT_FLAG(MS_BOOL bSet)
{
    HAL_AUDIO_SET_INIT_FLAG(bSet);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GET_INIT_FLAG()
/// @brief \b Function \b Description: This routine get the initial status of Aduio module.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_GET_INIT_FLAG(void)
{
    return HAL_AUDIO_GET_INIT_FLAG();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_InitialVars()
/// @brief \b Function \b Description:  Initial variables in share memory
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_InitialVars(void)
{
    return HAL_AUDIO_InitialVars();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_Close()
/// @brief \b Function \b Description: This routine is the close function for Audio module.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_Close(void)
{

    MDrv_AUDIO_AttachInterrupt(FALSE);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_Open()
/// @brief \b Function \b Description: This routine is the open function for Audio module.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_Open(void)
{

    MDrv_AUDIO_AttachInterrupt(TRUE);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteInitTable()
/// @brief \b Function \b Description: This routine is used to write the audio initial table.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_WriteInitTable(void)
{
    DRVAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->ChipRevision = MDrv_SYS_GetChipRev();

    HAL_AUDIO_WriteInitTable();

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description: This routine is used to set power on DAC sequence before setting init table.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void MDrv_AUDIO_WritePreInitTable(void)
{
    _PreInit = TRUE; // If open in Api layer, _PreInit will be true, to prevent do HAL_AUDIO_InitialVars() once

    HAL_AUDIO_WritePreInitTable();

   return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description: This routine is used to enable earphone low power stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void MDrv_AUDIO_EnaEarphone_LowPower_Stage(void)
{
   HAL_AUDIO_EnaEarphone_LowPower_Stage();
   return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description: This routine is used to enable earphone high driving stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void MDrv_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
   HAL_AUDIO_EnaEarphone_HighDriving_Stage();
   return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SwResetMAD()
/// @brief \b Function \b Description: This function is used to software reset MAD
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SwResetMAD(void)
{
    /* To prevent unexpected condition, set stop command to all R2/DSP */
    MDrv_AUDIO_WriteStopDecTable();

    if (MDrv_AUDIO_CheckBootOnInitState() == TRUE)
    {
        HAL_AUDIO_SwResetMAD();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param bPower_on   \b : TRUE --power on,
///                         FALSE--power off
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetPowerOn(MS_BOOL bPower_on)
{
    HAL_AUDIO_SetPowerOn(bPower_on);

    if (bPower_on == FALSE)
    {
        MDrv_AUDIO_AttachInterrupt(FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetDspBaseAddr()
/// @brief \b Function \b Description:  This function is used to set bin file base address in flash for Audio module.
/// @param u8Index     \b :
/// @param u32Bin_Base_Address    \b :
/// @param u32Mad_Base_Buffer_Adr \b :
////////////////////////////////////////////////////////////////////////////////
void _MDrv_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_PHY phyBin_Base_Address, MS_PHY phyMad_Base_Buffer_Adr)
{
    HAL_AUDIO_SetDspBaseAddr(u8Index, phyBin_Base_Address, phyMad_Base_Buffer_Adr);
}

///////////////////////////////////////////////////////////////////////////////
///
/// @brief \b Function \b Name: MDrv_AUDIO_GetDspBinBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the Bin file base address.
/// @param u8Index     \b     :
/// @return MS_U32     \b     :
////////////////////////////////////////////////////////////////////////////////
MS_PHY MDrv_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index)
{
    return  (HAL_AUDIO_GetDspBinBaseAddr(u8Index));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetDspMadBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param u8Index     \b     :
/// @return MS_U32     \b     :
////////////////////////////////////////////////////////////////////////////////
MS_PHY _MDrv_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{
    return (HAL_AUDIO_GetDspMadBaseAddr(u8Index));
}

///////////////////////////////////////////////////////////////////////////////
///
/// @brief \b Function \b Name: MDrv_AUDIO_DspBootOnDDR()
/// @brief \b Function \b Description:  Enable DSP load / reload code from DDR
/// @param bEnable     \b : 0 -- load code from FLASH,
///                         1 -- load code from DDR
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DspBootOnDDR(MS_BOOL bEnable)
{
    HAL_AUDIO_DspBootOnDDR(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecoderLoadCode()
/// @brief \b Function \b Description: This routine is used to load Dec-DSP code
/// @return MS_BOOL    \b : TRUE --DEC-DSP load code okay,
///                         FALSE--DEC-DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_DecoderLoadCode(void)
{
    return HAL_AUDIO_DecoderLoadCode();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SeSystemLoadCode()
/// @brief \b Function \b Description: This routine is used to load DSP code for SE-DSP .
/// @return MS_BOOL    \b : TRUE --DEC-DSP load code okay,
///                         FALSE--DEC-DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SeSystemLoadCode(void)
{
    return HAL_AUDIO_SeSystemLoadCode();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_RebootDecDSP()
/// @brief \b Function \b Description:  This routine reboot Decoder DSP.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_RebootDSP(MS_BOOL bDspType)
{
    if (bDspType==DSP_SE)
    {
        MDrv_MAD2_RebootDsp();  //reboot sndEff DSP
    }
    else
    {
        MDrv_AUDIO_RebootDecDSP();  //reboot DEC DSP
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_TriggerSifPLL()
/// @brief \b Function \b Description:  This function is used to initialize SIF analog part .
////////////////////////////////////////////////////////////////////////////////
void _MDrv_AUDIO_TriggerSifPLL(void)
{
    HAL_SIF_TriggerSifPLL();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecDspISR()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
#if defined (MSOS_TYPE_NOS)
void MDrv_AUDIO_DecDspISR(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data)
#else // Ecos and Linux
void MDrv_AUDIO_DecDspISR( InterruptNum eIntNum )
#endif
{
    #if defined (MSOS_TYPE_NOS)
        UNUSED(pHalReg);
        UNUSED(u32Data);
    #else // Ecos and Linux
        UNUSED(eIntNum);
    #endif

    HAL_AUDIO_DecDspISR();

    /* perfrom DEC CallBack for VoIP applications */
    if(audio_dec_app_callbk != NULL)
    {
        (*audio_dec_app_callbk)(HAL_AUDIO_ReadByte(REG_D2M_MAILBOX_DEC_ISRCMD/*REG_D2M_MAILBOX_DEC_ISRCMD*/));
    }

    MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DecDspISR()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
#if defined (MSOS_TYPE_NOS)
    void MDrv_AUDIO_SeDspISR(MHAL_SavedRegisters *pHalReg, MS_U32 u32Data)
#else // Ecos and Linux
    void MDrv_AUDIO_SeDspISR( InterruptNum eIntNum )
#endif
{
    #if defined (MSOS_TYPE_NOS)
        UNUSED(pHalReg);
        UNUSED(u32Data);
    #else // Ecos and Linux
        UNUSED(eIntNum);
    #endif

    HAL_AUDIO_SeDspISR();

    /* perfrom SE CallBack for VoIP applications */
    if(audio_se_app_callbk != NULL)
    {
        (*audio_se_app_callbk)(HAL_AUDIO_ReadByte(REG_D2M_MAILBOX_SE_ISRCMD/*REG_D2M_MAILBOX_SE_ISRCMD*/));
    }

    MsOS_EnableInterrupt(E_INT_FIQ_SE_DSP2UP);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetPlayFileFlag()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetPlayFileFlag(MS_BOOL bDspType, MS_BOOL bSet)
{
    HAL_AUDIO_SetPlayFileFlag(bDspType, bSet);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetPlayFileFlag()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_GetPlayFileFlag(MS_BOOL bDspType)
{
    return (HAL_AUDIO_GetPlayFileFlag(bDspType));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetEncodeDoneFlag()
/// @brief \b Function \b Description:  This function is used to get the Encoder flag status
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_GetEncodeDoneFlag(void)
{
    return (HAL_AUDIO_GetEncodeDoneFlag());
}

//======================================================================

////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                 ///
//        AUDIO I/O Config Relational Drv Function                                                             ///
//                                                                                                                                 ///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetPathGroup()
/// @brief \b Function \b Description: This routine is used to get Audio path group type .
/// @param output      \b : Audio output path-group type
////////////////////////////////////////////////////////////////////////////////
AUDIO_PATH_GROUP_TYPE MDrv_AUDIO_GetPathGroup(void)
{
    return(HAL_AUDIO_GetPathGroup());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetNormalPath()
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input .
/// @param path        \b : Audio DSP channel
/// @param input       \b : Audio input type
/// @param output      \b : Audio output type
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetNormalPath(AUDIO_PATH_TYPE path, AUDIO_INPUT_TYPE input, AUDIO_OUTPUT_TYPE output)
{
    HAL_AUDIO_SetNormalPath(path, input, output);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetInputPath()
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input and Output.
/// @param u8Path      \b : Audio DSP channel
/// @param u8Input     \b : Audio input type
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetInputPath( AUDIO_INPUT_TYPE u8Input, AUDIO_PATH_TYPE u8Path)
{
    MS_U8 path;

    path=(MS_U8)u8Path;


    if(u8Path == AUDIO_PATH_NULL)
    {
        return;    // path0~3 are used for multi-channel ; we've fixed the setting in audio init table.
    }
    HAL_AUDIO_SetInputPath(u8Input ,u8Path);

    if((u8Input != AUDIO_HDMI_INPUT) && (u8Input != AUDIO_DSP1_HDMI_INPUT) && (u8Input != AUDIO_DSP3_HDMI_INPUT))
    {
            HAL_AUDIO_WriteMaskByte((REG_AUDIO_MUTE_CTRL1+1) + (path-4)*2, 0x78, 0x00);
    }
    else if(u8Input == AUDIO_HDMI_INPUT)  // HDMI pcm in
    {
            HAL_AUDIO_WriteMaskByte((REG_AUDIO_MUTE_CTRL1+1) + (path-4)*2, 0x78, 0x78);
    }
    else if((u8Input == AUDIO_DSP1_HDMI_INPUT) ||(u8Input == AUDIO_DSP3_HDMI_INPUT))  // HDMI non-PCM in
    {
        HAL_AUDIO_WriteMaskByte((REG_AUDIO_MUTE_CTRL1+1) + (path-4)*2, 0x78, 0x70);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetOutPath()
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param u8Path      \b : Audio DSP channel
/// @param u8Output    \b : Audio output type
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetOutputPath(AUDIO_PATH_TYPE u8Path, AUDIO_OUTPUT_TYPE u8Output)
{
    HAL_AUDIO_SetOutputPath(u8Path, u8Output);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetInternalPath()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio internal path
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output)
{
    HAL_AUDIO_SetInternalPath(u8Path, u8Output);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_FwTriggerDSP()
/// @brief \b Function \b Description:  This routine send a PIO11 interrupt to DSP with a command on 0x2D34.
/// @param u16Cmd      \b : 0xF0-- for MHEG5 file protocol
///////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
{
    HAL_AUDIO_FwTriggerDSP((MS_U16)u16Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_I2S_SetMode()
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param u8Mode      \b : MCLK , MS_U16_WIDTH , SOURCE_CH , FORMAT
/// @param u8Val      \b : Please reference the register table 0x2C8C & 0x2C8D .
///////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8  u8Val)
{
   _HAL_AUDIO_I2S_SetMode(u8Mode, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ReadDspCodeType()
/// @brief \b Function \b Description:  This routine is used to read the DSP code type.
/// @return MS_U8      \b : DSP code type
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_ReadDspCodeType(void)
{
    return HAL_AUDIO_GetDspCodeType();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_BT_SetUploadRate()
/// @brief \b Function \b Description:  According Blue tooth upload path, for different sampling rate setting the Synthesizer.
/// @param bEnable     \b : 0 -- Disable Blue Tooth upload,
///                         1 -- Enable Blue Tooth upload
/// @param u8Samprate  \b : Sampling Rate
///                         0--no change,
///                         1--48KHz,
///                         2--44KHz,
///                         3--32KHz
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_BT_SetUploadRate(MS_BOOL bEnable,MS_U8 u8Samprate)
{
    HAL_AUDIO_BT_SetUploadRate(bEnable,u8Samprate);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_BT_SetBufferCounter()
/// @brief \b Function \b Description:  Set the DDR buffer according the sampling rate and the frame time
/// @param u32Counter  \b : if the sampling rate is 48KHz, the frame time is 40ms.
///                         ==> the frame buffer size is 48000*0x04*2 (L/R) *2(Bytes/sample) = 0x1E00,
///                             the counter is 0x1E00/8 = 960 (For 1*Burst DMA)
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_BT_SetBufferCounter(MS_U32 u32Counter)
{
    HAL_AUDIO_BT_SetBufferCounter(u32Counter);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    HAL_AUDIO_USBPCM_Enable(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    HAL_AUDIO_USBPCM_SetFlag(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_USBPCM_GetFlag(void)
{
    return HAL_AUDIO_USBPCM_GetFlag();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{

    HAL_AUDIO_USBPCM_GetMemInfo(uploadInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ReadDspCounter()
/// @brief \b Function \b Description:  Read the DSP running counter
/// @param CounterType \b  :
///                 - DSP_DEC_MAIN_COUNTER ==> Decoder DSP Main loop counter
///                 - DSP_DEC_TIMER_COUNTER ==> Decoder Timer counter
///                 - DSP_SE_MAIN_COUNTER ==> SE DSP Main loop counter
///                 - DSP_SE_TIMER_COUNTER ==> SE Timer counter
///                 - DSP_SE_ISR_COUNTER ==> SE Interrupt ISR counter
///                 - DEC_R2_MAIN_COUNTER   ==> DEC R2 Main loop counter
///                 - DEC_R2_TIMER_COUNTER  ==> DEC R2 Timer counter
///                 - SND_R2_MAIN_COUNTER   ==> SND R2 Main loop counter
///                 - SND_R2_TIMER_COUNTER  ==> SND R2 Timer counter
/// @return MS_U8      \b  : Running counter value
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_ReadDspCounter(AUDIO_DSP_COUNTER_TYPE  CounterType)
{
       return HAL_AUDIO_ReadDspCounter(CounterType);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AuProcessPowerDown_Wait()
/// @brief \b Function \b Description: This routine is used to DSP power-down wait function.
/// @param bEnable     \b : TRUE --Not wait,
///                         FALSE--wait
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetPowerDownWait(MS_BOOL bEnable)
{
    HAL_AUDIO_SetPowerDownWait(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_IsDTV()
/// @brief \b Function \b Description:  Report the decoder type is ATV or DTV
/// @return MS_BOOL    \b : 0 -> ATV , 1 -> DTV
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_IsDTV(void)
{
    return HAL_AUDIO_GetIsDtvFlag();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetFading()
/// @brief \b Function \b Description:  This routine is used to set the Fading response time
/// @param u32VolFading \b : Fading response time parameter
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetFading(MS_U32 u32VolFading)
{
    HAL_AUDIO_Set_Fading(u32VolFading);
}

//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO SPDIF Relational Drv Function
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_HWEN()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param bEnable     \b : 0--Disable S/PDIF out,
///                         1--Enable S/PDIF out
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_HWEN(MS_U8 bEnable)
{
    _HAL_AUDIO_SPDIF_HWEN((MS_BOOL)bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param bEnMute     \b : 0--Disable mute,
///                         1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_SetMute(MS_U8 bEnMute)
{
    DRVAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_SPDIF_MuteStatus = bEnMute;

    _HAL_AUDIO_SPDIF_SetMute((MS_BOOL)bEnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param u8Spdif_mode \b : Reg_2D2E
///                         - bit[0] = 0: spdif enable, 1: spdif disable (Se-DSP)
///                         - bit[1] = 0: PCM mode,     1: non-PCM mode
///                         - bit[2] = 1: non-PCM NULL Payload
/// @param u8Input_src \b : DTV
///                         1 : ATV,
///                         2 : HDMI,
///                         3 : ADC,
///                         4 : CardReader,
///                         5 : SPDIF
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_SetMode(MS_U8 u8Spdif_mode, MS_U8 u8Input_src)
{
    HAL_AUDIO_SPDIF_SetMode(u8Spdif_mode, u8Input_src);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_GetMode()
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @return MS_U8      \b :  Se-DSP 0x2D2E value
///                        - u8Spdif_mode = 0x0 : SPDIF enable, PCM mode
///                        - u8Spdif_mode = 0x1 : SPDIF OFF
///                        - u8Spdif_mode = 0x2 : SPDIF enable, non-PCM mode
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_SPDIF_GetMode(void)
{
    return _HAL_AUDIO_SPDIF_GetMode();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_ByPassChannel()
/// @brief \b Function \b Description:  ByPass the SPDIF channel (CH4)
/// @param bEnable     \b :  TRUE --BYPASS CH4,
///                          FALSE--OPEN CH4
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_ByPassChannel(MS_BOOL bEnable)
{
    _HAL_AUDIO_SPDIF_ByPassChannel(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit
/// @param L_bit_en  \b : generation bit
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    HAL_AUDIO_SPDIF_SetSCMS(C_bit_en, L_bit_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_SPDIF_GetSCMS(void)
{
    return(HAL_AUDIO_SPDIF_GetSCMS());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_SetChannelStatus()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF CS mode.
/// @return MS_U8      \b :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)
{
    HAL_AUDIO_SetChannelStatus(eType, eValue);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:  This routine is used to control SPDIF CS in detail.
/// @return MS_U8      \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{
    return(HAL_AUDIO_SPDIF_ChannelStatus_CTRL(cs_mode, status));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_SetOutputType()
/// @brief \b Function \b Description:  New create to set S/PDIF output format
/// @param eType \b :
///    SPDIF_OUT_PCM ==> S/PDIF always output PCM
///    SPDIF_OUT_AUTO ==> S/PDIF OUTPUT PCM or non-PCM automatic
///    SPDIF_OUT_BYPASS ==> S/PDIF OUTPUT the by-pass data from HDMI
///    SPDIF_OUT_NONE ==> S/PDIF OUTPUT nothing
/// @param eSource \b : DTV
///    E_AUDIO_INFO_DTV_IN==> DTV input
///    E_AUDIO_INFO_ATV_IN==> ATV input
///    E_AUDIO_INFO_HDMI_IN==> HDMI Input
///    E_AUDIO_INFO_ADC_IN==> ADC input mode
///    E_AUDIO_INFO_MM_IN==> MM mode
///    E_AUDIO_INFO_SPDIF_IN==> SPDIF input mode
///    E_AUDIO_INFO_KTV_IN==> KTV input mode
///    E_AUDIO_INFO_GAME_IN==> Game mode
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_SetOutputType (AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType)
{
     HAL_AUDIO_SPDIF_SetOutputType(eSource, eType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPDIF/HDMI/ARC channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Mdrv_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    MS_BOOL RET = FALSE;
    if (gDigitalOutChannelStatusFuncPtr != NULL)
    {
        RET = gDigitalOutChannelStatusFuncPtr(ePath, stChannelStatus);
    }

    return RET;
}

//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO HDMI Relational Drv Function
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable HDMI output (Software)
/// @param bEnMute     \b : 0--Disable mute,
///                         1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_HDMI_Tx_SetMute(MS_U8 bEnMute)
{
    _HAL_AUDIO_HDMI_Tx_SetMute((MS_BOOL)bEnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
    HAL_AUDIO_HDMI_Tx_GetStatus(onOff, hdmi_SmpFreq, outType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Monitor()
/// @brief \b Function \b Description:  Report PCM/HDMI non-PCM status
/// @return MS_U8      \b : return 0x2C0C value
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_HDMI_NonpcmMonitor(void)
{
    return (HAL_AUDIO_HDMI_NonpcmMonitor());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_SetOutputType()
/// @brief \b Function \b Description:  To set HDMI output type independently
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType, AUDIO_SOURCE_INFO_TYPE eSource)
{
     HAL_AUDIO_HDMI_TX_SetMode(outType, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_DolbyMonitor()
/// @brief \b Function \b Description:  Report HDMI non-PCM Dolby mod status
/// @return MS_U8      \b : 0-- Other mode  ; 1-- Dolby mode
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_HDMI_DolbyMonitor(void)
{
    return HAL_AUDIO_HDMI_DolbyMonitor();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SPDIF_Monitor_SamplingRate()
/// @brief \b Function \b Description:  This routine is to set ADC clock for SPDIF raw out
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SPDIF_Monitor_SamplingRate(void)
{
    HAL_AUDIO_SPDIF_Monitor_SamplingRate();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_SetNonpcm()
/// @brief \b Function \b Description:  Set HDMI PCM/non-PCM Mode
/// @param bEnNonPCM   \b : FALSE==> PCM mode
///                       : TRUE ==> non-PCM mode
////////////////////////////////////////////////////////////////////////////////
AUDIO_HDMI_RX_TYPE MDrv_AUDIO_HDMI_SetNonpcm(MS_U8 bEnNonPCM)
{
    AUDIO_HDMI_RX_TYPE ret_type = HDMI_RX_PCM;
    MS_U8 HDMI_PC_Status;

    HDMI_PC_Status = MDrv_AUDIO_HDMI_DolbyMonitor();

    switch(HDMI_PC_Status)
    {
        case AU_HDMI_DTS_TYPE1:
        case AU_HDMI_DTS_TYPE2:
        case AU_HDMI_DTS_TYPE3:
            ret_type = HDMI_RX_DTS;
            break;

        case AU_HDMI_AC3:
            ret_type = HDMI_RX_DD;
            break;

        case AU_HDMI_AC3P:
            ret_type = HDMI_RX_DDP;
            break;

         default:
             ret_type = HDMI_RX_PCM;
            break;
    };

    HAL_AUDIO_HDMI_SetNonpcm(bEnNonPCM);

    return ret_type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_AC3_PathCFG()
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path Control
/// @param u8Ctrl      \b :
///                     - 0==> DVB2_AD path
///                     - 1==> HDMI path
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_HDMI_AC3_PathCFG(MS_U8 u8Ctrl)
{
    HAL_AUDIO_HDMI_AC3_PathCFG(u8Ctrl);
}

//-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO Internal Use Drv Function
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_AdcInit()
/// @brief \b Function \b Description:  This routine is ADC relational register Init.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_AdcInit(void)
{
    HAL_AUDIO_ADCInit();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetDecoderStatus()
/// @brief \b Function \b Description:  This routine is used to read the Decoder status.
/// @return MS_U8      \b : Decoder status
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_AUDIO_GetDecoderStatus(void)
{
    return HAL_AUDIO_Dec_Status();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_GetSynthFreq()
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @return MS_U16     \b : return freq. If no signal, return 0
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_HDMI_GetSynthFreq(void)
{
    return HAL_AUDIO_HDMI_Get_SYNTH_FREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_HDMI_SetDownSample()
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param ratio       \b : donwsample ratio. 00: Normal (from 1x to 1x),
///                         01: Down sample from 2x to 1x.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
{
    HAL_AUDIO_HDMI_Set_DwonSample(ratio);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SendIntrupt()
/// @brief \b Function \b Description:  send a PIO8 interrupt to DSP
/// @param bDspType    \b :
/// @param u8Cmd       \b :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd)
{
    HAL_AUDIO_SendIntrupt(bDspType, u8Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate)
{
    HAL_AUDIO_DmaReader_Init(sampleRate);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description: Used DMA reader in all audio source application
///                    \b              (ex: Key sound) (CHIP_T8/T12)
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DmaReader_AllInput_Init(void)
{
    HAL_AUDIO_DmaReader_AllInput_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer bytes
/// @param <OUT>       \b NONE    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
{
    return(HAL_AUDIO_DmaReader_WritePCM(buffer,bytes));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DmaWriter_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DmaWriter_Init(void)
{
    HAL_AUDIO_DmaWriter_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_AbsReadReg()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_AbsReadReg(MS_U32 u32RegAddr)
{
    return HAL_AUDIO_AbsReadReg(u32RegAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_AbsWriteReg()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    HAL_AUDIO_AbsWriteReg(u32RegAddr, u16Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetEntertainmentMode()
/// @brief \b Function \b Description: This routine is used to switch the DTV/KTV/GAME mode
/// @param <IN>        \b   eType   : eMode
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE eMode)
{
     HAL_AUDIO_SetEntertainmentMode(eMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
    return(HAL_AUDIO_GetDecSysSupportStatus(enDecSystem));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DspReboot()
/// @brief \b Function \b Description: This routine is used to do dsp reboot and some specific setting
/// @param <IN>        \b   eType   : param
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DspReboot(MS_U8 param)
{
    DRVAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_bHashkeyFlag = FALSE;

    MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    MsOS_DisableInterrupt(E_INT_FIQ_SE_DSP2UP);

    OS_OBTAIN_MUTEX(_s32AUDIOMutexReboot, MSOS_WAIT_FOREVER);

     HAL_AUDIO_DspReboot(0);

     MsOS_DelayTask(2);
    HAL_AUDIO_WriteMaskByte(0x2c02, 0x04, 0x00);//disable SIF clock
    MsOS_DelayTask(1);

    HAL_AUDIO_BackupMailbox();

    if ( param == 0 )
        HAL_AUDIO_SwResetMAD();

    HAL_AUDIO_RebootDecDSP();

    if ( param == 0 )
        MDrv_MAD2_RebootDsp();

    //restore hash key after reboot DSP
    MDrv_AUDIO_CheckHashkey();

    HAL_AUDIO_RestoreMailbox();

    HAL_AUDIO_WriteMaskByte(0x2c02, 0x04, 0x04);//enable SIF clock

     HAL_AUDIO_DspReboot(1);

     OS_RELEASE_MUTEX(_s32AUDIOMutexReboot);

    MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    MsOS_EnableInterrupt(E_INT_FIQ_SE_DSP2UP);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetUniDecodeDoneFlag()
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_AUDIO_GetUniDecodeDoneTag(void)
{
    return (HAL_AUDIO_GetUniDecodeDoneTag());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_RebootDecDSP()
/// @brief \b Function \b Description:  This routine reboot Decoder DSP.
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_RebootDecDSP(void)
{
    HAL_AUDIO_RebootDecDSP();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_VoIP_Config()
/// @brief \b Function \b Description:  This routine is used for VoIP applications
/// @param <IN>        \b AUDIO_VoIP_CONFIG_TYPE : configType
/// @param <IN>        \b MS_U32 : param1
/// @param <IN>        \b MS_U32 : param2
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_VIRT param1, MS_VIRT param2)
{
    MS_BOOL ret = FALSE;

    /*
     * For the moment, param2 is not in used, it's just reserved for any advanced requirement in the future.
     * Please be remembered to remove below dummy once param2 is used, because this dummy is just to pass coverity check.
     */
    UNUSED(param2);

    switch(configType)
    {
        case AUDIO_VoIP_CONFIG_REGISTER_DEC_CB:
        {
            audio_dec_app_callbk = (void *)param1;
            ret = TRUE;
            break;
        }

        case AUDIO_VoIP_CONFIG_REGISTER_SE_CB:
        {
            audio_se_app_callbk = (void *)param1;
            ret = TRUE;
            break;
        }

        case AUDIO_VoIP_CONFIG_ALSA_PLAYBACK:
        {
            ret = MDrv_AUDIO_ALSA_Enable((MS_BOOL)param1);
            break;
        }

        case AUDIO_VOIP_CONFIG_PCM_UPLOAD:
        {
            MS_BOOL bVoipEnable = (MS_BOOL)param1;
            HAL_AUDIO_USBPCM_Enable(bVoipEnable);
            HAL_AUDIO_SetEntertainmentMode(bVoipEnable == 1 ? AUDIO_ETMODE_VOIP_ON : AUDIO_ETMODE_VOIP_OFF);
            ret = TRUE;
            break;
        }

        default:
        {
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Invalid VoIP Config Type (%d) !!\n", __func__, (int)configType);
            break;
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_ALSA_Check(void)
{
    return HAL_AUDIO_ALSA_Check();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    return HAL_AUDIO_ALSA_Enable(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    return HAL_AUDIO_SetDataCaptureSource(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCMCapture_Init()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    return HAL_AUDIO_PCMCapture_Init(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
   return HAL_AUDIO_PCMCapture_Start(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
     return HAL_AUDIO_PCMCapture_Stop(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID,  void* buffer, const MS_U32 bytes)
{
    return HAL_AUDIO_PCMCapture_Read(eID, buffer, bytes);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_DumpDspInfo()
/// @brief \b Function \b Description:  Dump DSP infomation
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_DumpDspInfo(void)
{
    HAL_AUDIO_DumpDspInfo();
}

AUDIO_DEC_ID MDrv_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    if (gOpenDecodeSystemFuncPtr == NULL)
    {
        return AU_DEC_INVALID;
    }

    return gOpenDecodeSystemFuncPtr(p_AudioDecStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_PHY MDrv_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    MS_PHY DDRValue = 0;

    if (gGetDDRInfoFuncPtr == NULL)
    {
        return DDRValue;
    }

    return gGetDDRInfoFuncPtr(DecId, DDRInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_update_DspUsage()
/// @brief \b Function \b Description:  update DSP resource status when select input source
/// @param <IN>        \b AUDIO_SOURCE_INFO_TYPE : select input source type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_update_DspUsage(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    HAL_AUDIO_update_DspUsage(eSourceType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetCaps()
/// @brief \b Function \b Description: Get Audio Capabilities
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b MS_U32 * : Audio Capabilites pointer
/// @param <OUT>        \b MS_BOOL : return TRUE if success, else return FALSE
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps)
{
    MS_U32 ret = FALSE;

    ret = HAL_AUDIO_GetCaps(DecId, pCaps);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_BOOL : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_CheckPlayDone(void)
{
    return HAL_AUDIO_CheckPlayDone();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Xe8_16_24_32Bit_To_Le16Bit()
/// @brief \b Function \b Description : Convert PCM date to 16bits/LE.
/// @param <IN>        \b MS_U16 : Pointer to a block of memory of destination PCM date.
/// @param <IN>        \b MS_U8 : Pointer to a block of memory of source PCM date.
/// @param <IN>        \b MS_U32 : BitWidth of source PCM date.
/// @param <IN>        \b MS_U32 : Endian type of source PCM date.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Xe8_16_24_32Bit_To_Le16Bit(MS_U16 *pU16Dst, MS_U8 *pU8Src, MS_U32 u32BitWidth, MS_U32 u32BigEndian)
{
    MS_S32 s32Ret = 0;
    MS_U8 *pU8Dst = NULL;
    MS_U8 u8SrcPcmHigh = 0;
    MS_U8 u8SrcPcmLow = 0;

    pU8Dst = (MS_U8 *)pU16Dst;

    if (u32BitWidth == 8)
    {
        MS_U16 u16PcmTmp = 0;

        u16PcmTmp = ((MS_U16)(*pU8Src - 128) << 8);
        u8SrcPcmLow = (MS_U8)(u16PcmTmp & 0xFF);
        u8SrcPcmHigh = (MS_U8)(u16PcmTmp >> 8);
    }
    else
    {
        MS_U8 u8PcmHighIndex = 0;
        MS_U8 u8PcmLowIndex = 0;

        if (u32BigEndian == TRUE)
        {
            u8PcmLowIndex = 1;
            u8PcmHighIndex = 0;
        }
        else
        {
            u8PcmLowIndex = (u32BitWidth / 8) - 2;
            u8PcmHighIndex = (u32BitWidth / 8) - 1;
        }

        u8SrcPcmLow = pU8Src[u8PcmLowIndex];
        u8SrcPcmHigh = pU8Src[u8PcmHighIndex];
    }

    pU8Dst[0] = u8SrcPcmLow;
    pU8Dst[1] = u8SrcPcmHigh;

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_converter()
/// @brief \b Function \b Description : Convert PCM date to 16bits/LE, and specific channel number.
/// @param <IN>        \b void : Pointer to a block of memory for storing converted PCM data.
/// @param <IN>        \b MS_U32 : The requested PCM channel number.
/// @param <IN>        \b MS_U32 : The requested PCM size.
/// @param <IN>        \b void : Pointer to a block of memory with original PCM data.
/// @param <IN>        \b AUDIO_PCM_INFO_t : The using PCM capabilities.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_converter(void *pDstData, MS_U32 u32DstChannel, MS_U32 u32DstSize, const void *pSrcData, AUDIO_PCM_INFO_t *pSrcPCMInfo)
{
    MS_S32 s32Ret = 0;
    MS_U32 u32Channel = 0;
    MS_U32 u32BitWidth = 0;
    MS_U32 u32BigEndian = 0;
    MS_U32 u32DstBitWidth = 16;

    if (pDstData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pDstData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pSrcData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pSrcData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pSrcPCMInfo == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pSrcPCMInfo should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if ((u32DstChannel != 2) && (u32DstChannel != 10))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32DstChannel(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32DstChannel);
        return -A_EINVAL;
    }

    if (u32DstSize == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32DstSize);
        return -A_EINVAL;
    }

    u32Channel = pSrcPCMInfo->u32Channel;
    if ((u32Channel < 1) || (u32Channel > 10))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Channel(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Channel);
        return -A_EINVAL;
    }

    u32BitWidth = pSrcPCMInfo->u32BitWidth;
    if ((u32BitWidth != 8) && (u32BitWidth != 16) && (u32BitWidth != 24) && (u32BitWidth != 32))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32BitWidth(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32BitWidth);
        return -A_EINVAL;
    }

    u32BigEndian = pSrcPCMInfo->u32BigEndian;
    if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32BigEndian(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32BigEndian);
        return -A_EINVAL;
    }

    if ((u32Channel == u32DstChannel) && (u32BitWidth == u32DstBitWidth) && (u32BigEndian == FALSE))
    {
        /* PCM format is exactly same, just copy PCM data from source to destination */
        memcpy(pDstData, pSrcData, u32DstSize);
    }
    else
    {
        MS_U8 *pU8SrcData = NULL;
        MS_U16 *pU16SrcData = NULL;
        MS_U16 *pU16DstData = NULL;
        MS_U16 u16PcmArray[10] = {0};
        MS_U32 u32PcmSample = 0;
        MS_U32 u32PcmSampleIndex = 0;
        MS_U32 u32ChannelIndex = 0;

        pU8SrcData = (MS_U8 *)pSrcData;
        pU16SrcData = (MS_U16 *)pSrcData;
        pU16DstData = (MS_U16 *)pDstData;
        u32PcmSample = (u32DstSize / u32DstChannel) / (u32DstBitWidth / 8);

        for (u32PcmSampleIndex = 0; u32PcmSampleIndex < u32PcmSample; u32PcmSampleIndex++)
        {
            for (u32ChannelIndex = 0; u32ChannelIndex < u32Channel; u32ChannelIndex++)
            {
                MDrv_AUDIO_PCM_Xe8_16_24_32Bit_To_Le16Bit(&u16PcmArray[u32ChannelIndex], pU8SrcData, u32BitWidth, u32BigEndian);
                pU8SrcData += (pSrcPCMInfo->u32BitWidth / 8);
            }

            if (u32Channel == 1)
            {
                u16PcmArray[AUDIO_MULTICH_PCM_DMXR] = u16PcmArray[AUDIO_MULTICH_PCM_DMXL];
                u16PcmArray[AUDIO_MULTICH_PCM_C] = u16PcmArray[AUDIO_MULTICH_PCM_DMXL];
            }
            else if (u32Channel == 2)
            {
                u16PcmArray[AUDIO_MULTICH_PCM_L] = u16PcmArray[AUDIO_MULTICH_PCM_DMXL];
                u16PcmArray[AUDIO_MULTICH_PCM_R] = u16PcmArray[AUDIO_MULTICH_PCM_DMXR];
            }

            *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_DMXL];
            *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_DMXR];
            if (u32DstChannel == 10)
            {
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_L];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_C];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_R];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_LS];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_RS];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_LFE];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_LRS];
                *pU16DstData++ = u16PcmArray[AUDIO_MULTICH_PCM_RRS];
            }
        }
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Init()
/// @brief \b Function \b Description : Init common PCM I/O.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_PCM_Init(void)
{
    MS_S32 s32DeviceId = 0;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Init PCM device\n", __FUNCTION__);

    DRVAUDIO_CHECK_SHM_INIT;

    if (g_Common_PCM_IO_Init == TRUE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: already initialized\n", __FUNCTION__);
        return;
    }

    for (s32DeviceId = 0; s32DeviceId < AUDIO_PCM_MAXNUM; s32DeviceId++)
    {
        if (g_PCM[s32DeviceId] == NULL)
        {
            continue;
        }

        g_PCM[s32DeviceId]->pPcmInfo = &g_AudioVars2->g_PcmInfo[s32DeviceId];
    }

    g_Common_PCM_IO_Init = TRUE;

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_DeInit()
/// @brief \b Function \b Description : DeInit common PCM I/O.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_PCM_DeInit(void)
{
    MS_S32 s32DeviceId = 0;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DeInit PCM device\n", __FUNCTION__);

    DRVAUDIO_CHECK_SHM_INIT;

    if (g_Common_PCM_IO_Init == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: no need to de-initialize\n", __FUNCTION__);
        return;
    }

    for (s32DeviceId = 0; s32DeviceId < AUDIO_PCM_MAXNUM; s32DeviceId++)
    {
        if (g_PCM[s32DeviceId] == NULL)
        {
            continue;
        }

        g_PCM[s32DeviceId]->pPcmInfo = NULL;
    }

    g_Common_PCM_IO_Init = FALSE;

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Open()
/// @brief \b Function \b Description : Open a PCM interface according to specific capabilities.
/// @param <IN>        \b void : Pointer to a block of memory with PCM information.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = NULL;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_S32 s32DeviceId = 0;
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Open PCM device\n", __FUNCTION__);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (unsigned int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    for (s32DeviceId = 0; s32DeviceId < AUDIO_PCM_MAXNUM; s32DeviceId++)
    {
        pPCM = g_PCM[s32DeviceId];

        if (pPCM == NULL)
        {
            continue;
        }

        if (pPCM->tPcmOps.open == NULL)
        {
            continue;
        }

        if (pUserPcmInfo->u8Name[0] != 0x00)
        {
            if (strcmp((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name) != 0)
            {
                continue;
            }
        }

        if (pPCM->pPcmInfo->u8ConnectFlag == TRUE)
        {
            continue;
        }

        s32Ret = pPCM->tPcmOps.open(pUserPcmInfo);
        if (s32Ret < 0)
        {
            continue;
        }

        break;
    }

    s32Ret = (s32DeviceId == AUDIO_PCM_MAXNUM) ? (-A_EBUSY) : s32DeviceId;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: return device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Close()
/// @brief \b Function \b Description : Close an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Close(MS_S32 s32DeviceId)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Close PCM device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.close == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register close()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    s32Ret = pPCM->tPcmOps.close();

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Start()
/// @brief \b Function \b Description : Start an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Start(MS_S32 s32DeviceId)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Start PCM device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.start == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register start()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8StartFlag == FALSE)
    {
        s32Ret = pPCM->tPcmOps.start();
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Stop()
/// @brief \b Function \b Description : Stop a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Stop(MS_S32 s32DeviceId)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Stop PCM device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.stop == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register stop()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8StartFlag != FALSE)
    {
        s32Ret = pPCM->tPcmOps.stop();
    }

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Set()
/// @brief \b Function \b Description : Set configuration to an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory with specific configuration, and this memory is not allowed to be updated by this API.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Set command %u to PCM device-id %d\n", __FUNCTION__, u32Cmd, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.set == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register set()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8StartFlag != FALSE)
    {
        switch (u32Cmd)
        {
            case AUDIO_PCM_CMD_MIXINGGROUP:
            case AUDIO_PCM_CMD_TIMESTAMP:
            case AUDIO_PCM_CMD_WEIGHTING:
            case AUDIO_PCM_CMD_VOLUME:
            case AUDIO_PCM_CMD_MUTE:
            {
                break;
            }

            default:
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is running, can not be configured now!\n", __FUNCTION__, (int)s32DeviceId);
                return -A_EBUSY;
            }
        }
    }

    s32Ret = pPCM->tPcmOps.set(u32Cmd, pData);

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Get()
/// @brief \b Function \b Description : Get configuration from an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory for receiving specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Get command %u from PCM device-id %d\n", __FUNCTION__, u32Cmd, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.get == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register get()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    s32Ret = pPCM->tPcmOps.get(u32Cmd, pData);

    return s32Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Read()
/// @brief \b Function \b Description : Read a specific amount of PCM data from a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_U8 *pBufTmp = NULL;
    MS_U32 u32SizeToRead = 0;
    MS_U32 u32SizeToReadRemain = 0;
    MS_U32 u32BufferLevel = 0;
    MS_U32 u32ReadSize = 0;
    MS_U32 u32Loop = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Read %u bytes from PCM device-id %d\n", __FUNCTION__, u32Size, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pBuf == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return u32Size;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (u32Size == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return u32Size;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->tPcmOps.read == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register read()!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->pPcmInfo->u8StartFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not started yet!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->tPcmOps.get == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register get()!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    pBufTmp = (MS_U8 *)pBuf;
    u32SizeToRead = u32Size;
    u32SizeToReadRemain = u32SizeToRead;

    do
    {
        pPCM->tPcmOps.get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32BufferLevel);

        if (pPCM->pPcmInfo->u8NonBlockingFlag == TRUE)
        {
            u32SizeToRead = (u32SizeToRead > u32BufferLevel) ? u32BufferLevel : u32SizeToRead;
            if (u32SizeToRead == 0)
            {
                break;
            }

            u32SizeToReadRemain = u32SizeToRead;
            u32ReadSize = u32SizeToReadRemain;

        }
        else
        {
            u32ReadSize = u32SizeToReadRemain;
            u32ReadSize = (u32ReadSize > u32BufferLevel) ? u32BufferLevel : u32ReadSize;
        }

        if (u32ReadSize == 0)
        {
            u32Loop++;
            if (u32Loop > 250)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: s32DeviceId(%d) read PCM timeout!\n", __FUNCTION__, (int)s32DeviceId);
                break;
            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            msleep(2);
#else
            AUDIO_DELAY1MS(2);
#endif
        }
        else
        {
            u32ReadSize = pPCM->tPcmOps.read((void *)pBufTmp, u32ReadSize);
            u32Loop = 0;
        }

        pBufTmp += u32ReadSize;
        u32SizeToReadRemain -= u32ReadSize;
    } while (u32SizeToReadRemain > 0);

    return u32SizeToRead;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Write()
/// @brief \b Function \b Description : Write a specific amount of PCM data to a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data, and this memory is not allowed to be updated by this API.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_U8 *pConvertedPcmBuf = NULL;
    MS_U8 *pConvertedPcmBufTmp = NULL;
    MS_U32 u32SampleToWrite = 0;
    MS_U32 u32TargetSizeToWrite = 0;
    MS_U32 u32TargetSizeToWriteRemain = 0;
    MS_U32 u32BufferLevel = 0;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32AvailSize = 0;
    MS_U32 u32WriteSize = 0;
    MS_U32 u32TotalWriteSize = 0;
    MS_U32 u32TotalWriteSample = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_U32 u32Loop = 0;
    MS_S32 s32Ret = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Write %u bytes to PCM device-id %d\n", __FUNCTION__, u32Size, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pBuf == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return u32Size;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (u32Size == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (int)u32Size);
        return u32Size;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->tPcmOps.write == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register write()!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->pPcmInfo->u8StartFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not started yet!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    if (pPCM->tPcmOps.get == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register get()!\n", __FUNCTION__, (int)s32DeviceId);
        return u32Size;
    }

    u32SampleToWrite = (u32Size / (pPCM->pPcmInfo->u32BitWidth / 8)) / pPCM->pPcmInfo->u32Channel;
    if (u32SampleToWrite == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Size(%u) is not a sample base size at %u channels!\n", __FUNCTION__, (unsigned int)u32Size, (unsigned int)pPCM->pPcmInfo->u32Channel);
        return u32Size;
    }

    pPCM->tPcmOps.get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    if (pPCM->pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* total is 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* total is stereo = 2 channels */
        u32TargetChannel = 2;
    }

    u32TargetBitWidth = 16; /* target is always 16 bits mode */
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8);

    u32TargetSizeToWrite = (u32SampleToWrite * (u32TargetBitWidth / 8)) * u32TargetChannel;
    u32TargetSizeToWriteRemain = u32TargetSizeToWrite;

    do
    {
        pPCM->tPcmOps.get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32BufferLevel);
        u32AvailSize = u32BufferSize - u32BufferLevel;

        /* find the maxium size of available size that can be aligned to both channel and bitwidth */
        AUDIO_DO_ALIGNMENT(u32AvailSize, u32TargetAlignmentSize);

        if (pPCM->pPcmInfo->u8NonBlockingFlag == TRUE)
        {
            u32TargetSizeToWrite = (u32TargetSizeToWrite > u32AvailSize) ? u32AvailSize : u32TargetSizeToWrite;
            if (u32TargetSizeToWrite == 0)
            {
                break;
            }

            u32TargetSizeToWriteRemain = u32TargetSizeToWrite;
            u32WriteSize = u32TargetSizeToWriteRemain;
        }
        else
        {
            u32WriteSize = u32TargetSizeToWriteRemain;
            u32WriteSize = (u32WriteSize > u32AvailSize) ? u32AvailSize : u32WriteSize;
        }

        if (u32WriteSize == 0)
        {
            u32Loop++;
            if (u32Loop > 250)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) write PCM timeout!\n", __FUNCTION__, (int)s32DeviceId);
                break;
            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            msleep(2);
#else
            AUDIO_DELAY1MS(2);
#endif
        }
        else
        {
            if (pConvertedPcmBuf == NULL)
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                pConvertedPcmBuf = (MS_U8 *)vmalloc(u32TargetSizeToWrite);
#else
                pConvertedPcmBuf = (MS_U8 *)malloc(u32TargetSizeToWrite);
#endif
                if (pConvertedPcmBuf == NULL)
                {
                    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) can't allocate %u bytes for PCM buffer!\n", __FUNCTION__, (int)s32DeviceId, (unsigned int)u32TargetSizeToWrite);
                    return u32Size;
                }

                memset((void *)pConvertedPcmBuf, 0x00, u32TargetSizeToWrite);
                s32Ret = MDrv_AUDIO_PCM_converter((void *)pConvertedPcmBuf, u32TargetChannel, u32TargetSizeToWrite, pBuf, pPCM->pPcmInfo);
                if (s32Ret < 0)
                {
                    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) fail to convert PCM data!\n", __FUNCTION__, (unsigned int)s32DeviceId);
                }

                pConvertedPcmBufTmp = pConvertedPcmBuf;
            }

            u32WriteSize = pPCM->tPcmOps.write((void *)pConvertedPcmBufTmp, u32WriteSize);
            u32Loop = 0;
        }

        pConvertedPcmBufTmp += u32WriteSize;
        u32TargetSizeToWriteRemain -= u32WriteSize;
        u32TotalWriteSample += ((u32WriteSize / (u32TargetBitWidth / 8)) / u32TargetChannel);
    } while (u32TargetSizeToWriteRemain > 0);

    if (pConvertedPcmBuf != NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        vfree(pConvertedPcmBuf);
#else
        free(pConvertedPcmBuf);
#endif
        pConvertedPcmBuf = NULL;
    }

    u32TotalWriteSize = (u32TotalWriteSample * (pPCM->pPcmInfo->u32BitWidth / 8)) * pPCM->pPcmInfo->u32Channel;

    return u32TotalWriteSize;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_PCM_Flush()
/// @brief \b Function \b Description : Flush an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 MDrv_AUDIO_PCM_Flush(MS_S32 s32DeviceId)
{
    AUDIO_PCM_t *pPCM = NULL;
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Flush PCM device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_PCM_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    pPCM = g_PCM[s32DeviceId];

    if (pPCM == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't support PCM I/O!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->tPcmOps.flush == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) doesn't register flush()!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8ConnectFlag == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is not opened yet!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EPERM;
    }

    if (pPCM->pPcmInfo->u8StartFlag != FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is running, can not do flush now!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EBUSY;
    }

    s32Ret = pPCM->tPcmOps.flush();

    return s32Ret;
}

void MDrv_AUDIO_ResetDspCodeType(void)
{
    HAL_AUDIO_ResetDspCodeType();
}

MS_BOOL MDrv_AUDIO_Check_AsndR2_SupportStatus(void)
{
    return ASND_R2_SUPPORT;
}

void MDrv_AUDIO_WriteStopDecTable(void)
{
    HAL_AUDIO_WriteStopDecTable();

    return;
}

MS_S32 MDrv_AUDIO_InputSwitch(AUDIO_SWITCH_GROUP enGroup)
{
    HAL_AUDIO_InputSwitch(enGroup);

    return 0;
}

MS_S32 MDrv_AUDIO_ENCODER_StrCmd_To_U32Cmd(const char *pCmd, MS_U32 *pU32Cmd)
{
    MS_S32 s32Ret = 0;

    if (strcmp("GetFrameSize", pCmd) == 0)
    {
       *pU32Cmd = AUDIO_ENCODER_IOCTL_GET_FRAME_SIZE;
    }
    else if (strcmp("GetAvailableSize", pCmd) == 0)
    {
       *pU32Cmd = AUDIO_ENCODER_IOCTL_GET_AVAILABLE_SIZE;
    }
    else if (strcmp("GetTimestampHigh32Bit", pCmd) == 0)
    {
       *pU32Cmd = AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_HIGH32BIT;
    }
    else if (strcmp("GetTimestampLow32Bit", pCmd) == 0)
    {
       *pU32Cmd = AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_LOW32BIT;
    }
    else
    {
        s32Ret = -A_EINVAL;
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Open(const char *pName)
{
    MS_S32 s32DeviceId = 0;
    MS_S32 s32Ret = 0;


    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Open Encoder device\n", __FUNCTION__);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pName == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pName should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (strcmp("MP3_ENCODER1", pName) == 0)
    {
        s32Ret = HAL_AUDIO_MP3_ENCODER1_Open();
        s32DeviceId = 0;
    }
    else if (strcmp("MP3_ENCODER2", pName) == 0)
    {
        s32Ret = HAL_AUDIO_MP3_ENCODER2_Open();
        s32DeviceId = 1;
    }
    else
    {
        s32Ret = -A_ENODEV;
    }

    if (s32Ret == 0)
    {
        s32Ret = s32DeviceId;
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: return device-id %d\n", __FUNCTION__, (int)s32DeviceId);
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Close(MS_S32 s32DeviceId)
{
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Close Encoder device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Close();
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Close();
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Start(MS_S32 s32DeviceId)
{
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Start Encoder device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Start();
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Start();
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId)
{
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Stop Encoder device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Stop();
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Stop();
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData)
{
    MS_U32 u32Cmd = 0;
    MS_S32 s32Ret = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Ioctl command '%s' to Encoder device-id %d\n", __FUNCTION__, pCmd, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pCmd == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pCmd should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pData == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    if (MDrv_AUDIO_ENCODER_StrCmd_To_U32Cmd(pCmd, &u32Cmd) < 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Warning! pCmd '%s' is not supported!\n", __FUNCTION__, pCmd);
        return -A_EINVAL;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Ioctl(u32Cmd, pData);
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Ioctl(u32Cmd, pData);
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}

MS_U32 MDrv_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    MS_S32 s32Ret = 0;
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Read %u bytes from Encoder device-id %d\n", __FUNCTION__, u32Size, s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if (pBuf == NULL)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    if (u32Size == 0)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return u32Size;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Read(pBuf, u32Size);
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Read(pBuf, u32Size);
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}

MS_S32 MDrv_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId)
{
    MS_S32 s32Ret = 0;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Flush Encoder device-id %d\n", __FUNCTION__, (int)s32DeviceId);

    DRVAUDIO_CHECK_SHM_INIT;

    if ((s32DeviceId < 0) || (s32DeviceId >= AUDIO_ENCODER_MAXNUM))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: Error! s32DeviceId(%d) is invalid!\n", __FUNCTION__, (int)s32DeviceId);
        return -A_EINVAL;
    }

    switch (s32DeviceId)
    {
        case 0:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER1_Flush();
            break;
        }

        case 1:
        {
            s32Ret = HAL_AUDIO_MP3_ENCODER2_Flush();
            break;
        }

        default:
        {
            s32Ret = -A_ENODEV;
            break;
        }
    }

    return s32Ret;
}