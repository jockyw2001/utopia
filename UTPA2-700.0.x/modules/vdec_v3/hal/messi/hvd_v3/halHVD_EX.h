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

#ifndef _HAL_HVD_H_
#define _HAL_HVD_H_
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Feature
#define HAL_HVD_ENABLE_MUTEX_PROTECT    HVD_ENABLE_MUTEX_PROTECT
#define HAL_HVD_ENABLE_MIU_PROTECT      HVD_ENABLE_MIU_RST_PROTECT
#define HAL_HVD_ENABLE_VPU_CMD    1
#define HAL_CHIP_SUPPORT_EVD

// MBox type of FW cmd
#if (HAL_HVD_ENABLE_VPU_CMD)
    #define HAL_HVD_CMD_MBOX            E_HVD_VPU_HI_0
    #define HAL_HVD_CMD_ARG_MBOX        E_HVD_VPU_HI_1
#else
    #define HAL_HVD_CMD_MBOX            E_HVD_HI_0
    #define HAL_HVD_CMD_ARG_MBOX        E_HVD_HI_1
#endif
// MBox other usages
#define HAL_HVD_REG_ISR_VPU     E_HVD_VPU_RISC_1
#define HAL_HVD_REG_FW_STATE    E_HVD_RISC_0
#define HAL_HVD_REG_ISR_HVD     E_HVD_RISC_1
#define HAL_HVD_REG_DISP_INFO_COPYED    E_HVD_RISC_1
#define HAL_HVD_REG_DISP_INFO_CHANGE    E_HVD_RISC_1
#define HAL_HVD_REG_DISP_CTL    E_HVD_HI_0

#define PRESET_ONE_PENDING_BUFFER       (1 << 0)  /// For AVC, one pending buffer mode, reduce from two to one
#define PRESET_FRAMERATE_HANDLING       (1 << 1)  /// For AVC, Handle frame rate by input frame rate when sequence did not have frame rate info.

#if defined(CHIP_T3)
// patch for HW bug
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL//0x10000000UL
#else // CHIP_U3
#define HVD_MIU1_BASE_ADDRESS   0x08000000UL
#endif

#define HVD_RV_BROKENBYUS_BIT    29

#ifdef VDEC3
#define HAL_HVD_INVALID_BBU_ID 0xFFFFFFFF
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_HVD_HI_0,
    E_HVD_HI_1,
    E_HVD_RISC_0,
    E_HVD_RISC_1,
    E_HVD_VPU_HI_0,
    E_HVD_VPU_HI_1,
    E_HVD_VPU_RISC_0,
    E_HVD_VPU_RISC_1,
} HVD_MBOX_TYPE;

typedef enum
{
    //Support TSP/TS/File mode
    E_HAL_HVD_MAIN_STREAM,

    //Only support file mode
    E_HAL_HVD_SUB_STREAM,

    //Only support MVC mode
    E_HAL_HVD_MVC_STREAM,

#ifdef VDEC3
    E_HAL_HVD_N_STREAM,
#endif
} HAL_HVD_StreamType;

#ifdef VDEC3
typedef enum
{
    E_HAL_HVD_AVC,
    E_HAL_HVD_AVS,
    E_HAL_HVD_RM,
    E_HAL_HVD_MVC,
    E_HAL_HVD_VP8,
    E_HAL_HVD_MJPEG,
    E_HAL_HVD_VP6,
    E_HAL_HVD_HEVC,
    E_HAL_HVD_VP9,
    E_HAL_HVD_NONE,
} HAL_HVD_CodecType;

typedef enum
{
    E_HAL_HVD_STATE_STOP,
    E_HAL_HVD_STATE_RUNNING,
    E_HAL_HVD_STATE_PAUSING,
    E_HAL_HVD_STATE_PAUSE_DONE
} HAL_HVD_Task_State;

#endif

typedef struct
{
    HAL_HVD_StreamId eStreamId;
    MS_BOOL bUsed;
    MS_BOOL bDispOutSide;
    MS_PHY u32PTSPreWptr;
    MS_U32 u32PTSByteCnt;
    MS_U32 u32BBUWptr;
    MS_U32 u32BBUEntryNum;
    MS_U32 u32BBUEntryNumTH;
    MS_U32 u32DispQIndex;
    MS_U32 u32PrivateData;
    MS_U32 u32FreeData;
    MS_U32 u32RegBase;
#ifdef VDEC3
    HAL_HVD_CodecType u32CodecType;
    HAL_HVD_Task_State ePpTaskState;
    MS_S32 s32HvdPpTaskId;
#endif
} HVD_EX_Stream;

typedef struct
{
    MS_BOOL bColocateBBUMode;

}HVD_EX_PreSet;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL     HAL_HVD_EX_InitHW(MS_U32 u32Id,VPU_EX_DecoderType DecoderType);
MS_BOOL     HAL_HVD_EX_DeinitHW(void);
void        HAL_HVD_EX_FlushMemory(void);
void        HAL_HVD_EX_ReadMemory(void);
void        HAL_HVD_EX_SetDrvCtrlsBase(HVD_EX_Drv_Ctrl *pHVDCtrlsBase);
void        HAL_HVD_EX_CheckMIUSel(MS_BOOL bChange);
MS_U32      HAL_HVD_EX_GetHWVersionID(void);
HAL_HVD_StreamId HAL_HVD_EX_GetFreeStream(HAL_HVD_StreamType eStreamType);
void        HAL_HVD_EX_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_EX_InitRegBase(MS_VIRT u32RegBase);
HVD_Return  HAL_HVD_EX_InitVariables(MS_U32 u32Id);
#ifdef VDEC3
MS_U32      HAL_HVD_EX_GetBBUId(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_FreeBBUId(MS_U32 u32Id, MS_U32 u32BBUId);
HVD_Return  HAL_HVD_EX_InitShareMem(MS_U32 u32Id, MS_BOOL bFWdecideFB, MS_BOOL bCMAUsed);
HVD_Return  HAL_HVD_EX_InitRegCPU(MS_U32 u32Id, MS_BOOL bFWdecideFB);
#else
HVD_Return  HAL_HVD_EX_InitShareMem(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_InitRegCPU(MS_U32 u32Id);
#endif
HVD_Return  HAL_HVD_EX_SetData(MS_U32 u32Id, HVD_SetData u32type, MS_VIRT u32Data);
MS_VIRT     HAL_HVD_EX_GetData(MS_U32 u32Id, HVD_GetData eType);
MS_S64      HAL_HVD_EX_GetData_EX(MS_U32 u32Id, HVD_GetData eType);
MS_VIRT     HAL_HVD_EX_GetShmAddr(MS_U32 u32Id);
MS_VIRT     HAL_HVD_EX_GetDispQExtShmAddr(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id, MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);
HVD_Return  HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
HVD_Return  HAL_HVD_EX_DeInit(MS_U32 u32Id);
HVD_Return  HAL_HVD_EX_PushPacket(MS_U32 u32Id, HVD_BBU_Info *pInfo);
void        HAL_HVD_EX_EnableISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetClearISR(HWDEC_ISR_TYPE eISRType);
MS_BOOL     HAL_HVD_EX_IsISROccured(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsEnableISR(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_IsAlive(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_RstPTSCtrlVariable(MS_U32 u32Id);
MS_BOOL     HAL_HVD_EX_FlushRstShareMem(MS_U32 u32Id);
void        HAL_HVD_EX_UartSwitch2FW(MS_BOOL bEnable);
MS_U32      HAL_HVD_EX_GetData_Dbg(MS_U32 u32Addr);
void        HAL_HVD_EX_SetData_Dbg(MS_U32 u32Addr, MS_U32 u32Data);
MS_U16      HAL_HVD_EX_GetCorretClock(MS_U16 u16Clock);
void        HAL_HVD_EX_UpdateESWptr_Fire(MS_U32 u32Id);
void        HAL_HVD_EX_MVD_PowerCtrl(MS_BOOL bEnable);
void        HAL_HVD_EX_Dump_FW_Status(MS_U32 u32Id);
void        HAL_HVD_EX_Dump_HW_Status(MS_U32 u32Num);
void        HAL_HVD_EX_SetMiuBurstLevel(HVD_EX_Drv_Ctrl *pDrvCtrl, HVD_MIU_Burst_Cnt_Ctrl eMiuBurstCntCtrl);
void	    HVD_EX_SetRstFlag(MS_BOOL bRst);
MS_BOOL     HAL_HVD_EX_DispFrameAllViewed(MS_U32 u32Id);
MS_U8        _HVD_EX_GetStreamIdx(MS_U32 u32Id);

#if HVD_ENABLE_MVC
MS_BOOL     HAL_HVD_EX_CheckMVCID(MS_U32 u32Id);
VDEC_EX_View  HAL_HVD_EX_GetView(MS_U32 u32Id);
#endif ///HVD_ENABLE_MVC
MS_BOOL HAL_HVD_EX_Init_Share_Mem(void);
void        HAL_HVD_EX_SpareBandwidth(MS_U32 u32Id);
void        HAL_HVD_EX_PowerSaving(MS_U32 u32Id);
MS_U32      HAL_HVD_EX_GetDispFrmNum(MS_U32 u32Id);
HVD_Return HAL_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_BOOL bEnable);
void        HAL_HVD_EX_SetHwRegBase(MS_U32 u32Id, MS_U32 u32ModeFlag);
#if SUPPORT_EVD
void        HAL_EVD_EX_ClearTSPInput(MS_U32 u32Id);
#endif
MS_BOOL HAL_HVD_EX_GetSupport2ndMVOPInterface(void);
void        HAL_HVD_EX_SetBufferAddr(MS_U32 u32Id);
void        HAL_HVD_EX_SetPreCtrlVariables(MS_U32 u32Id,MS_VIRT drvprectrl);
MS_BOOL     HAL_HVD_EX_Is_RM_Supported(MS_U32 u32Id);
void HAL_HVD_MVDMiuClientSel(MS_U8 u8MiuSel);
typedef enum
{
	E_BBU_FSM_START  = 0,
    E_BBU_FSM_0,
    E_BBU_FSM_00,
    E_BBU_FSM_001,
} VDEC_EX_BBU_FSM_STATE;


void HAL_HVD_EX_BBU_Proc(MS_U32 u32Id);
void HAL_HVD_EX_BBU_StopProc(MS_U32 u32Id);

#endif
#endif // _HAL_HVD_H_

