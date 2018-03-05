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
// Copyright (c) 2015-2020 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDMX_tee.h
/// @brief  MStar Demux Driver Interface for Tee
/// @author MStar Semiconductor Inc.
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX TEE modules
  */
//-------------------------------------------------

#ifndef __API_DMX_TEE_H__
#define __API_DMX_TEE_H__

#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Pocily Mamager Pipie id definitation
#define DMX_PIPE_NULL                                       0xFFFFFFFF
#define DMX_PIPE_TSPFW                                      0x00000001
#define DMX_PIPE_TSPVQ                                      0x00000002
#define DMX_PIPE_PVR_0                                      0x80000000
#define DMX_PIPE_PVR_1                                      0x80000001
#define DMX_PIPE_PVR_2                                      0x80000002
#define DMX_PIPE_PVR_3                                      0x80000003
#define DMX_PIPE_FILE_0                                     0x40000000
#define DMX_PIPE_FILE_1                                     0x40000001
#define DMX_PIPE_FILE_2                                     0x40000002
#define DMX_PIPE_FILE_3                                     0x40000003
#define DMX_PIPE_MMFI_0                                     0x41000000
#define DMX_PIPE_MMFI_1                                     0x41000001

#ifdef MSOS_TYPE_OPTEE
typedef struct
{
    MS_U32 ID[4]; //id[3]: 0, id[2]=address_h, od[1]=address_l, id[0]=length
}DMX_TEE_RESOURCE;
#endif //MSOS_TYPE_OPTEE

typedef struct _DMX_TEE_BUF_Param
{
    MS_U32  u32EngId;             // Engine ID
    MS_U32  u32Opt;               // Option value or MIU select number
    MS_U32  u32BufSize;          // Phsical Buffer Size
    MS_U32  u32BufAddr;          // Phsical Buffer Address
    MS_U32  u32BufAddr_H;        // Phsical Buffer Address_H
} DMX_TEE_BUF_Param;

typedef enum
{
    E_DMX_TEECMD_SET_TSPFW_BUF,                ///< For TSP FW buffer control
    E_DMX_TEECMD_SET_TSPVQ_BUF,                ///< For TSP VQ buffer control
    E_DMX_TEECMD_SET_PVR_BUF,                  ///< For TSP PVR buffer control
    E_DMX_TEECMD_SET_FILEIN_BUF,               ///< For TSP Finein buffer control
    E_DMX_TEECMD_SET_MMFI_BUF,                 ///< For TSP MMFI buffer control
} DMX_TEE_USER_CMD;

typedef enum
{
    E_DMX_REE_TO_TEE_CMD_NULL,
    E_DMX_REE_TO_TEE_CMD_FW_LoadCode,               //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_VqBuf,                 //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_PvrBuf,                //Param1: Engine id, Param2: Option 1 is reset buffer to 0
    E_DMX_REE_TO_TEE_CMD_SET_FileinBuf,             //Param1: Engine id, Param2: Buf address, Param3: Buf size
    E_DMX_REE_TO_TEE_CMD_GET_PvrWPtr,               //Param1: Engine id, Param2: Return Buf address
    E_DMX_REE_TO_TEE_CMD_SET_MMFIBuf                //Param1: Engine id, Param2: Return Buf address
} DMX_REE_TO_TEE_CMD_TYPE;

//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------


//Used for TEE
//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @ingroup DMX_Tee
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_NUTTX
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void);
#endif

//------------------------------------------------------------------------------
/// SetControl for TEE
/// @ingroup DMX_Tee
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note Tee set DMX setting inform to DMX
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param);

//------------------------------------------------------------------------------
/// Process REE commend
/// @param cmd \b IN : Ree Commend
/// @param param \b IN : TEE control parameters
/// @return MS_U32 Error code
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_TEE_ProcReeCmd(DMX_REE_TO_TEE_CMD_TYPE cmd, void* param);

#ifdef MSOS_TYPE_OPTEE
//------------------------------------------------------------------------------
/// Get secure resource by Pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param pu32MaxNRetRes \b IN : Pointer to store max request resource number
/// @param pResource \b OUT : Pointer to store resource data
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32* pu32MaxNRetRes, DMX_TEE_RESOURCE* pResource);

//------------------------------------------------------------------------------
/// Configure secure resource by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);

//------------------------------------------------------------------------------
/// Check secure resource status by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Status: 0 -> Configure failed; 1 -> Configure OK
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
#endif //MSOS_TYPE_OPTEE

#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_TEE_H__
