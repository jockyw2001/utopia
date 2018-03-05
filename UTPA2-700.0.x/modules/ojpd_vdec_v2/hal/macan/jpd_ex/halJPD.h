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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   halJPD.h
/// @brief  JPD hal interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_JPD_H_
#define _HAL_JPD_H_


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//Version
#define JPD_HAL_VERSION     "JPD.HAL.00117630"

// VPU MailBox
#define VPU_HI_MBOX0        0
#define VPU_HI_MBOX1        1
#define VPU_RISC_MBOX0      2
#define VPU_RISC_MBOX1      3

//#ifndef HAL_MIU1_BASE
//#define HAL_MIU1_BASE       0x10000000 // 256M
//#endif
#define JPD_MIU_ADDR_MASK   (HAL_MIU1_BASE - 1)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// JPD debug level enum
typedef enum
{
    E_JPD_DEBUG_HAL_NONE = 0x0
  , E_JPD_DEBUG_HAL_MSG  = 0x04
  , E_JPD_DEBUG_HAL_ERR  = 0x08
} JPD_HalDbgLevel;

// Dual Decode
typedef enum
{
    E_HAL_MJPEG_MAIN_STREAM,
    E_HAL_MJPEG_SUB_STREAM
} HAL_MJPEG_StreamType;

typedef enum
{
    E_HAL_MJPEG_STREAM_NONE = 0x0,

    E_HAL_MJPEG_MAIN_STREAM_BASE = 0x10,
    E_HAL_MJPEG_MAIN_STREAM0 = E_HAL_MJPEG_MAIN_STREAM_BASE,
    E_HAL_MJPEG_MAIN_STREAM_MAX,

    E_HAL_MJPEG_SUB_STREAM_BASE = 0x20,
    E_HAL_MJPEG_SUB_STREAM0 = E_HAL_MJPEG_SUB_STREAM_BASE,
    E_HAL_MJPEG_SUB_STREAM_MAX,

} HAL_MJPEG_StreamId;

typedef struct
{
    MS_U8* pu8HalVer;
    MS_U8* pu8FwVer;
}JPD_HAL_Version;

// Dual Decode
typedef struct
{
    HAL_MJPEG_StreamId eStreamId;
    MS_BOOL bUsed;
} HAL_MJPEG_Stream;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void HAL_JPD_SelectOJPD(void);
void HAL_JPD_SetMIU(MS_U8 u8Idx);
void HAL_JPD_Set_S_Config(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_S_Config(void);
void HAL_JPD_Set_M_Config(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_M_Config(void);
void HAL_JPD_SetIntEn(MS_U16 u16Value);
void HAL_JPD_SetMRCBufFloorLow(MS_U16 u16Value);
void HAL_JPD_SetMRCBufFloorHigh(MS_U16 u16Value);
void HAL_JPD_SetMRCBufCeilLow(MS_U16 u16Value);
void HAL_JPD_SetMRCBufCeilHigh(MS_U16 u16Value);
void HAL_JPD_SetRCSMAddrLow(MS_U16 u16Value);
void HAL_JPD_SetRCSMAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetMWCBufStAddrLow(MS_U16 u16Value);
void HAL_JPD_SetMWCBufStAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetPic_H(MS_U16 u16Value);
void HAL_JPD_SetPic_V(MS_U16 u16Value);
void HAL_JPD_SetEventFlag(MS_U16 u16Value);
MS_U16 HAL_JPD_GetEventFlag(void);
void HAL_JPD_SetROI_H(MS_U16 u16Value);
void HAL_JPD_SetROI_V(MS_U16 u16Value);
void HAL_JPD_SetROIWidth(MS_U16 u16Value);
void HAL_JPD_SetROIHeight(MS_U16 u16Value);
#if 0 // Unused function
MS_U16 HAL_JPD_GetCurMadrLow(void);
MS_U16 HAL_JPD_GetCurMadrHigh(void);
#endif
void HAL_JPD_SetClock(MS_U16 u16Value);
MS_U16 HAL_JPD_GetClock(void);
void HAL_JPD_PowerOn(void);
void HAL_JPD_PowerOff(void);
void HAL_JPD_InitRegBase(void);
void HAL_JPD_CreateMutex(void);
void HAL_JPD_DeleteMutex(void);
void HAL_JPD_SetRSTIntv(MS_U16 u16Value);
MS_U16 HAL_JPD_GetCurVidx(void);
void HAL_JPD_Set_TID_Addr(MS_U16 u16Value);
void HAL_JPD_Set_TID_Dat(MS_U16 u16Value);
MS_U16 HAL_JPD_Get_TID_Dat(void);
void HAL_JPD_SetDbgLevel(MS_U8 u8DbgLevel);
void HAL_JPD_GetLibVer(JPD_HAL_Version *pHalVer);
void HAL_JPD_Rst(void);
void HAL_JPD_SetMWBuffLineNum(MS_U16 u16Value);
MS_U32 HAL_JPD_GetCurMRCAddr(void);
MS_U16 HAL_JPD_GetCurRow(void);
MS_U16 HAL_JPD_GetCurCol(void);
void HAL_JPD_SetWriteProtect(MS_BOOL enable);
#if (JPD_SUPPORT_AUTO_PROTECT==true)
void HAL_JPD_SetAutoProtect(MS_BOOL enable);
void HAL_JPD_SetWPENAddrHigh(MS_U16 u16Value);
void HAL_JPD_SetWPENAddrLow(MS_U16 u16Value);
#endif
void HAL_JPD_SetSpare(MS_U16 u16Value);
MS_U16 HAL_JPD_GetSpare(void);


#endif // _HAL_JPD_H_

