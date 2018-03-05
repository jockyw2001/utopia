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
// ********************************************************
//

///////////////////////////////////////////////////////////////////////////////
///
/// file    pcmcia_private.h
/// @brief  PCMCIA Driver Utopia2.0 private
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////

#ifndef __PCMCIA_PRIVATE__
#define __PCMCIA_PRIVATE__
#include "drvPCMCIA.h"

#define MAGIC_NUM 0x87654321

enum {
    E_PCMCIA_RESOURCE,
} ePcmResourceId;

typedef struct _PCMCIA_RESOURCE_PRIVATE
{
//    MS_BOOL         bPCMCIA_Irq[E_PCMCIA_MODULE_MAX];
//    MS_BOOL         bPCMCIA_IrqStatus[E_PCMCIA_MODULE_MAX];
//    MS_U8           u8PCMCIA_Command[E_PCMCIA_MODULE_MAX];
//    MS_U8           u8HW_ResetDuration;

//    MS_BOOL         bHighActive;
//    MS_BOOL         bCardInside[E_PCMCIA_MODULE_MAX];
//    MS_BOOL         bPCMCIA_Detect_Enable;
//    MS_U32          u32PCMCIA_CD_To_HWRST_Timer[E_PCMCIA_MODULE_MAX];
    MS_U8           u8PCMCIACurModule;
    
    // APP callback function
//    IsrCallback     fnIsrCallback[E_PCMCIA_MODULE_MAX];
    
    MS_BOOL         bInited_Drv;   
    MS_U32          u32Magic;
    MS_BOOL         bCD_Reverse;
} PCMCIA_RESOURCE_PRIVATE;

typedef struct _PCMCIA_INSTANT_PRIVATE
{
} PCMCIA_INSTANT_PRIVATE;


void        _MDrv_PCMCIA_InitSW( MS_BOOL bCD_Reverse );
void        _MDrv_PCMCIA_InitHW( MS_BOOL bResume );
MS_U32      _MDrv_PCMCIA_SetPowerState(EN_POWER_MODE u16PowerState);
MS_BOOL     _MDrv_PCMCIA_DetectV2( PCMCIA_MODULE eModule );
MS_BOOL     _MDrv_PCMCIA_PollingV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_Set_HW_ResetDuration( MS_U8 u8HW_ResetDuration );
void        _MDrv_PCMCIA_ResetHW_V2( PCMCIA_MODULE eModule);
void        _MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value);
void        _MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest );
void        _MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value);
void        _MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer);
MS_U8       _MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr );
MS_BOOL     _MDrv_PCMCIA_IsModuleStillPluggedV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_Set_Detect_Trigger( MS_BOOL bHighActive );
void        _MDrv_PCMCIA_Set_Detect_Enable( MS_BOOL bEnable );
MS_U32      _MDrv_PCMCIA_Get_CD_IntervalV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_ParseAttribMem( MS_U8 *pAttribMem, MS_U16 dwLen, PCMCIA_INFO *pInfo );
void        _MDrv_PCMCIA_SetCommandBitV2( PCMCIA_MODULE eModule, MS_U8 u8CommandBit, MS_BOOL bValue );
MS_BOOL     _MDrv_PCMCIA_ResetInterfaceV2( PCMCIA_MODULE eModule );
MS_BOOL     _MDrv_PCMCIA_IsDataAvailableV2( PCMCIA_MODULE eModule );
MS_U16      _MDrv_PCMCIA_ReadDataV2( PCMCIA_MODULE eModule, MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize );
MS_BOOL     _MDrv_PCMCIA_WriteDataV2( PCMCIA_MODULE eModule, MS_U8* u8pWriteBuffer, MS_U16 u16DataLen );
MS_BOOL     _MDrv_PCMCIA_SwitchToIOmodeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
MS_U16      _MDrv_PCMCIA_NegotiateBufferSizeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
void        _MDrv_PCMCIA_WriteBufferSizeV2( PCMCIA_MODULE eModule, MS_U16 u16BufferSize );
MS_BOOL     _MDrv_PCMCIA_GetLibVer( const MSIF_Version **ppVersion );
void        _MDrv_PCMCIA_Enable_InterruptV2( PCMCIA_MODULE eModule, MS_BOOL bEnable );
void        _MDrv_PCMCIA_Set_InterruptStatusV2( PCMCIA_MODULE eModule, MS_BOOL Status );
MS_BOOL     _MDrv_PCMCIA_Get_InterruptStatusV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_InstarllIsrCallbackV2( PCMCIA_MODULE eModule, IsrCallback fnIsrCallback );
MS_BOOL     _MDrv_PCMCIA_WaitForStatusBitV2( PCMCIA_MODULE eModule, MS_U8 u8StatusBit );
void        _MDrv_PCMCIA_Exit( MS_BOOL bSuspend );
#endif //__PCMCIA_PRIVATE__
