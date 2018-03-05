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

#ifndef _DEV_AUDSP_H_
#define _DEV_AUDSP_H_
#include "MsCommon.h"
#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "../../drv/audio/internal/drvAUDIO_internal.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define code
#define BYTE MS_U8

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    MS_U32            cm_addr;
    MS_U32            cm_len;
    MS_U8            *cm_buf;
    
    MS_U32            pm_addr;
    MS_U32            pm_len;
    MS_U8            *pm_buf;
    
    MS_U32            cache_addr;
    MS_U32            cache_len;
    MS_U8            *cache_buf;
    
    MS_U32            prefetch_addr;
    MS_U32            prefetch_len;
    MS_U8            *prefetch_buf;

    char            *AlgName;
    AUDIO_DSP_CODE_TYPE    dspCodeType;
} AUDIO_ALG_INFO, *PAUDIO_ALG_INFO;

typedef struct
{
    AUDIO_ALG_INFO* pau_info;
    MS_U8 DSP_select;
}LOAD_CODE_INFO;
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//================================================================
//  Structure
//================================================================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//void    HAL_AUDIO_Set_MMIO_BASE(MS_U32 u32_MMIO_MapBase);

//================================================================
//  Basic Register read/write function
//================================================================
extern MS_U8   HAL_AUDIO_ReadByte(MS_U32 u32RegAddr);
extern MS_U8   HAL_AUDIO_DecReadByte(MS_U32 u32RegAddr);
extern MS_U8   HAL_AUDIO_SeReadByte(MS_U32 u32RegAddr);

extern MS_U16  HAL_AUDIO_ReadReg(MS_U32 u32RegAddr);
extern MS_U16  HAL_AUDIO_DecReadReg(MS_U32 u32RegAddr);
extern MS_U16  HAL_AUDIO_SeReadReg(MS_U32 u32RegAddr);

extern void    HAL_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
extern void    HAL_AUDIO_DecWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
extern void    HAL_AUDIO_SeWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);

extern void    HAL_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
extern void    HAL_AUDIO_DecWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
extern void    HAL_AUDIO_SeWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);

extern void    HAL_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
extern void    HAL_AUDIO_DecWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
extern void    HAL_AUDIO_SeWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);

extern void    HAL_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
extern void    HAL_AUDIO_DecWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
extern void    HAL_AUDIO_SeWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);

extern void    _HAL_AUDIO_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);

extern void    HAL_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);
extern MS_U16  HAL_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);
extern MS_PHY HAL_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);
extern MS_PHY HAL_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index);
extern MS_BOOL MDrv_FLASH_CheckWriteDone(void);
extern void    HAL_AUDIO_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);

MS_BOOL HAL_AUDSP_DspLoadCodeSegment(MS_U32 dsp_addr, MS_U8  *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select);
MS_BOOL HAL_AUDSP_DspVerifySegmentCode(MS_U32 dsp_addr, MS_U8 *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select);

MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type);
MS_BOOL HAL_AUDSP_CheckSeIdmaReady(MS_U8 u8IdmaChk_type);
MS_U32 HAL_AUDSP_GetDspBinBaseAddr(MS_U8 u8Index);
void HAL_AUDSP_SetDspCodeTypeLoaded(AUDIO_DSP_CODE_TYPE dspCodeType);
AUDIO_DSP_CODE_TYPE HAL_AUDSP_GetDspCodeTypeLoaded(void);
MS_BOOL HAL_AUDSP_DspLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
void HAL_AUDSP_SetDspLoadCodeInfo(AUDIO_ALG_INFO *pau_info, MS_U8 DSP_select);
LOAD_CODE_INFO* HAL_AUDSP_GetDspLoadCodeInfo(void);
MS_BOOL HAL_AUDSP_DspLoadCode2(MS_U8 DSP_select, void * info);
void HAL_AUDSP_DECR2LoadCode(void);
MS_BOOL HAL_AUDSP_Check_XBox_TransData_Ready(void);
#endif // _DEV_AUDSP_H_

