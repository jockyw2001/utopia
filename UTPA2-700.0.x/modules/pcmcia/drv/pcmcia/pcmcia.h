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
/// file    pcmcia.h
/// @brief  PCMCIA Driver Utopia2.0 Interface header
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////
#ifndef __PCMCIA_H__
#define __PCMCIA_H__

/*****************************************************************************/
/*                       Header Files                                        */
/*****************************************************************************/
// Common Definition
#include "MsTypes.h"
#include "drvPCMCIA.h"

typedef enum {
    E_MDRV_CMD_PCM_Init,
    E_MDRV_CMD_PCM_Detect,
    E_MDRV_CMD_PCM_Enable_Interrupt,
    E_MDRV_CMD_PCM_Exit,
    E_MDRV_CMD_PCM_Get_CD_Interval,
    E_MDRV_CMD_PCM_Get_InterruptStatus,
    E_MDRV_CMD_PCM_GetLibVer,
    E_MDRV_CMD_PCM_InstallIsrCallback,
    E_MDRV_CMD_PCM_IsDataAvailable,
    E_MDRV_CMD_PCM_IsModuleStillPlugged,
    E_MDRV_CMD_PCM_NegotiateBufferSize,
    E_MDRV_CMD_PCM_ParseAttribMem,
    E_MDRV_CMD_PCM_Polling,
    E_MDRV_CMD_PCM_ReadAttribMem, 
    E_MDRV_CMD_PCM_ReadData,
    E_MDRV_CMD_PCM_ReadIOMem,
    E_MDRV_CMD_PCM_ResetHW,
    E_MDRV_CMD_PCM_ResetInterface,
    E_MDRV_CMD_PCM_Set_Detect_Enable,
    E_MDRV_CMD_PCM_Set_Detect_Trigger,
    E_MDRV_CMD_PCM_Set_HW_ResetDuration,
    E_MDRV_CMD_PCM_Set_InterruptStatus,
    E_MDRV_CMD_PCM_SetCommandBit,
    E_MDRV_CMD_PCM_SwitchToIOmode,
    E_MDRV_CMD_PCM_WaitForStatusBit,
    E_MDRV_CMD_PCM_WriteAttribMem,
    E_MDRV_CMD_PCM_WriteBufferSize,
    E_MDRV_CMD_PCM_WriteData,
    E_MDRV_CMD_PCM_WriteIOMem,
    E_MDRV_CMD_PCM_WriteIOMemLong,
    E_MDRV_CMD_PCM_SetPowerState,
    E_MDRV_CMD_PCM_GetConfig,
    E_MDRV_CMD_PCM_MAX,
} ePcmIoctlOpt;

// E_MDRV_CMD_PCM_Detect
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bDetected;
} PCM_DETECT;

// E_MDRV_CMD_PCM_Enable_Interrupt
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bEnable;
} PCM_ENABLE_INT;

// E_MDRV_CMD_PCM_InstallIsrCallback
typedef struct {
    PCMCIA_MODULE   eModule;
    IsrCallback     pfcb;
} PCM_INSTCB;

// E_MDRV_CMD_PCM_Get_CD_Interval
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U32          u32Interval;
} PCM_GET_CD_INTERVAL;

// E_MDRV_CMD_PCM_Get_InterruptStatus
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bIntStatus;
} PCM_GET_INT_STATUS;

// E_MDRV_CMD_PCM_IsDataAvailable
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bDA; // data available
} PCM_DATA_AVAILABLE;

// E_MDRV_CMD_PCM_IsModuleStillPlugged
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bPlugIn;
} PCM_MODULE_PLUGIN;

// E_MDRV_CMD_PCM_NegotiateBufferSize
typedef struct {
    PCMCIA_MODULE   eModule;
    PCMCIA_INFO*    pInfo;
    MS_U16          u16BufSize;
} PCM_NETOTIAGE_BUFSIZE;

// E_MDRV_CMD_PCM_ParseAttribMem
typedef struct {
    MS_U8*          pu8AttribMem;
    MS_U16          u16DwLen;
    PCMCIA_INFO*    pInfo;
} PCM_PARSE_ATTRIBUTE;

// E_MDRV_CMD_PCM_Polling
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bChanged; // card insertion status change
} PCM_POLL;

// E_MDRV_CMD_PCM_ReadAttribMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Data;
} PCM_READ_ATTRIB_MEM;

// E_MDRV_CMD_PCM_ReadIOMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Data;
} PCM_READ_IO_MEM;

// E_MDRV_CMD_PCM_ResetInterface
typedef struct {
    PCMCIA_MODULE   eModule;
} PCM_RESET_INTERFACE;

// E_MDRV_CMD_PCM_ReadData
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8*          pu8ReadBuffer;
    MS_U16          u16ReadBufferSize;
    MS_U16          u16SizeRead;
} PCM_READ_DATA;

// E_MDRV_CMD_PCM_Set_InterruptStatus
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bStatus;
} PCM_SET_INTR_STATUS;

// E_MDRV_CMD_PCM_SetCommandBit
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8           u8CmdBit;
    MS_BOOL         bValue;
} PCM_SETCOMMANDBIT;

// E_MDRV_CMD_PCM_SwitchToIOmode
typedef struct {
    PCMCIA_MODULE   eModule;
    PCMCIA_INFO*    pInfo;
} PCM_SWITCH_TO_IOMODE;

// E_MDRV_CMD_PCM_WaitForStatusBit
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8           u8StatusBit;
} PCM_WAIT_STATUS;

// E_MDRV_CMD_PCM_WriteAttribMem
// E_MDRV_CMD_PCM_WriteIOMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Value;
} PCM_WRITE_ATTRIB_MEM, PCM_WRITE_IO_MEM;

// E_MDRV_CMD_PCM_WriteBufferSize
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16BufferSize;
} PCM_WRITE_BUF_SIZE;

// E_MDRV_CMD_PCM_WriteData
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8*          pu8WriteBuffer;
    MS_U16          u16DataLen;
} PCM_WRITE_DATA;

// E_MDRV_CMD_PCM_WriteIOMemLong
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Value;
    MS_U16          u16DataLen;
    MS_U8*          pu8WriteBuffer;
} PCM_WRITE_IO_MEM_LONG;

void CIRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PCMCIAOpen(void** pInstantTmp, const void* const pAttribute);
MS_U32 PCMCIAClose(void* pInstantTmp);
MS_U32 PCMCIAIoctl(void* pInstance, MS_U32 u32Cmd, void* const pArgs);

#endif //__PCMCIA_H__
