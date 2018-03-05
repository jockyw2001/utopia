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
/// file    drvMJPEG.c
/// @brief  MJPEG Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "halCHIP.h"

#include "drvMJPEG.h"
//#include "regJPD.h"
#include "halMJPEG.h"
#include "halVPU.h"

#define _JPD_DECODER_RENDER_ARCH 1

#define JPD_DEBUG_DRV_MSG(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_MSG) printf(format, ##args);}while(0)
#define JPD_DEBUG_DRV_ERR(format, args...)  do{if(u8JPD_DrvDbgLevel & E_JPD_DEBUG_DRV_ERR) printf(format, ##args);}while(0)



//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL gbEnableTurboFWMode = FALSE;

static MS_U32 __u32FwAddr = 0xFFFFFFFF;

static MS_U8 u8JPD_DrvDbgLevel = E_JPD_DEBUG_DRV_NONE;

/* =============================== MJPEG =====================================*/
void MDrv_MJPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
    u8JPD_DrvDbgLevel = u8DbgLevel;
    HAL_MJPEG_SetDbgLevel(u8DbgLevel);
}

/******************************************************************************/
/// Stop VPU
/******************************************************************************/
void MDrv_JPD_StopVPU(void)
{
    HAL_JPD_StopVPU();
}

/******************************************************************************/
/// Reset VPU
///@return success or fail
/******************************************************************************/
MS_BOOL MDrv_JPD_RstVPU(void)
{
    return HAL_JPD_RstVPU();
}

/******************************************************************************/
/// Release VPU
/******************************************************************************/
void MDrv_JPD_ReleaseVPU(void)
{
    HAL_JPD_ReleaseVPU();
}

/******************************************************************************/
///Load JPD Firmware for MJPEG
///@param u32Addr \b IN address to load firmware
///@return success or fail
/******************************************************************************/
MS_BOOL MDrv_JPD_LoadCode(MS_PHYADDR u32Addr, MS_U32 u32Size, MS_U32 u32BinAddr, MS_U8 u8FwSrcType)
{
    __u32FwAddr = u32Addr;
    return HAL_JPD_LoadCode(u32Addr, u32Size, u32BinAddr, u8FwSrcType);
}

/******************************************************************************/
///Config SVD
///@param u32StAddr \b IN: CPU binary code base address in DRAM.
///@param u8dlend_en \b IN: endian
///     - 1, little endian
///     - 0, big endian
/******************************************************************************/
void MDrv_JPD_ConfigVPU(MS_PHYADDR u32StAddr, MS_U8 u8dlend_en)
{
    HAL_JPD_ConfigVPU(u32StAddr, u8dlend_en);
}

/******************************************************************************/
///Check if MJPEG firmware ready or not
///@return TRUE or FALSE
/******************************************************************************/
MS_BOOL MDrv_JPD_IsFirmwareReady(void)
{
    MS_U32 u32FirmVer = 0, u32Timeout = 50000;

    // read VPU_RISC_MBOX0 to get firmware version
    while(u32Timeout)
    {
        if(HAL_JPD_IsVPUMBoxReady(VPU_RISC_MBOX0))
        {
            HAL_JPD_ReadVPUMBox(VPU_RISC_MBOX0, (MS_U32 *)&u32FirmVer);
            HAL_JPD_ClearVPUMBox(VPU_RISC_MBOX0, TRUE);
            break;
        }
        u32Timeout--;
    }
    if(u32Timeout > 0)
    {
        JPD_DEBUG_DRV_MSG("MJPEG firmware version 0x%lx\n", u32FirmVer);
    }
    else
    {
        JPD_DEBUG_DRV_ERR("Cannot get MJPEG firmware version !! \n");
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
///Set VPU Command
///@return TRUE or FALSE
///@param u8Cmd \b IN: VPU Command
///     - JPD_CMD_SET_FRAME_BUFF_ADDR, Set frame buffer address
///     - JPD_CMD_SET_FRAME_BUFF_SIZE, Set frame buffer size
/******************************************************************************/
static MS_BOOL MDrv_JPD_SetVPUCmd(MS_U32 u32Cmd)
{
    //currently we only use VPU_HI_MBOX0 to send command.
    //MS_U32 u32TimeOut = MsOS_GetSystemTime() + _u32FrameTime;
    MS_U32 u32TimeOut = 20000;
    MS_BOOL bResult = FALSE;

    JPD_DEBUG_DRV_MSG("Send CMD 0x%lx to JPD firmware\n", u32Cmd);
    //while(MsOS_GetSystemTime() < u32TimeOut)
    while(u32TimeOut)
    {
        if(HAL_JPD_IsVPUMBoxReady(VPU_HI_MBOX0))
        {
            bResult = HAL_JPD_SendVPUMBox(VPU_HI_MBOX0, u32Cmd);
            break;
        }
        u32TimeOut--;
    }
    if(u32TimeOut <= 0)
    {
        JPD_DEBUG_DRV_ERR("MJPEG send command timeout !! \n");
        bResult = FALSE;
    }

    return bResult;
}

/******************************************************************************/
///Set VPU command argument
///@return TRUE or FALSE
///@param u32Arg \b IN: VPU command argument
/******************************************************************************/
static MS_BOOL MDrv_JPD_SetVPUCmdArg(MS_U32 u32Arg)
{
    // we use VPU_HI_MBOX1 to send command argument
    //U16 u16TimeOut = 0xFFFF;
    MS_U16 u16TimeOut = 20000;
    MS_BOOL bResult = FALSE;

    JPD_DEBUG_DRV_MSG("Send argument 0x%lx to VPU \n", u32Arg);
    while(--u16TimeOut)
    {
        if( HAL_JPD_IsVPUMBoxReady(VPU_HI_MBOX0)&& HAL_JPD_IsVPUMBoxReady(VPU_HI_MBOX1))
        {
            bResult = HAL_JPD_SendVPUMBox(VPU_HI_MBOX1, u32Arg);
            break;
        }
    }
    if(u16TimeOut <= 0)
    {
        JPD_DEBUG_DRV_ERR("MJPEG send command argument timeout !! \n");
        bResult = FALSE;
    }

    return bResult;
}

/******************************************************************************/
///Check VPU status
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
static MS_U32 MDrv_JPD_CheckVPUStatus(MS_U32 u32Cmd)
{
//    MS_BOOL bResult = FALSE;
    MS_U32 u32Status = 0, u32Timeout = 20000;

    JPD_DEBUG_DRV_MSG("Firmware status check:  CMD 0x%lx\n", u32Cmd);
    while (u32Timeout) // Need timeout protection
    {
        if(HAL_JPD_IsVPUMBoxReady(VPU_HI_MBOX0))
        {
//            bResult = HAL_JPD_SendVPUMBox(VPU_HI_MBOX0, u32Cmd);
            HAL_JPD_SendVPUMBox(VPU_HI_MBOX0, u32Cmd);
            break;
        }
        u32Timeout--;
    }

    // Get firmware return value
    u32Timeout = 20000;
    while(u32Timeout)
    {
        if(HAL_JPD_IsVPUMBoxReady(VPU_RISC_MBOX0))
        {
            HAL_JPD_ReadVPUMBox(VPU_RISC_MBOX0, (MS_U32 *)&u32Status);
            HAL_JPD_ClearVPUMBox(VPU_RISC_MBOX0, TRUE);
            break;
        }
        u32Timeout--;
    }

    JPD_DEBUG_DRV_MSG("Firmware status : 0x%lX\n", u32Status);
    return u32Status;
}

/******************************************************************************/
///Set VPU command argument
///@return TRUE or FALSE
///@param u32Arg \b IN: VPU command argument
/******************************************************************************/
MS_U32 MDrv_JPD_SendVPUCommand(MJPEG_User_Cmd eCmd, MS_U32 u32Arg)
{
    MS_U32 u32Ret = 0;


    switch (eCmd)
    {
    case E_MJPEG_CMD_GO:
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_GO);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_START_ADDR);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_UNIT_SIZE);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_IDX);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_IDX_READY);
        break;

    case E_MJPEG_CMD_SET_WIDTH:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_WIDTH);
        break;

    case E_MJPEG_CMD_SET_HEIGHT:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_HEIGHT);
        break;

    case E_MJPEG_CMD_SET_PITCH:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_PITCH);
        break;

    case E_MJPEG_CMD_SET_FRAME_ID_L:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_ID_L);
        break;

    case E_MJPEG_CMD_SET_FRAME_ID_H:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_ID_H);
        break;

    case E_MJPEG_CMD_SET_TIMESTAMP:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_TIMESTAMP);
        break;

    case E_MJPEG_CMD_SET_FRAMERATE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAMERATE);
        break;

    case E_MJPEG_CMD_SET_FRAMERATE_BASE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAMERATE_BASE);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_FRAME_BUFF_IDX_VALID);
        break;

    case E_MJPEG_CMD_SET_CHIP_ID:
         MDrv_JPD_SetVPUCmdArg(u32Arg);
         MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_CHIP_ID);
         break;

    case E_MJPEG_CMD_PLAY:
        u32Ret = (MS_U32)MDrv_JPD_SetVPUCmd(E_JPD_CMD_PLAY);
        break;

    case E_MJPEG_CMD_PAUSE:
        u32Ret = (MS_U32)MDrv_JPD_SetVPUCmd(E_JPD_CMD_PAUSE);
        break;

    case E_MJPEG_CMD_RESUME:
        u32Ret = (MS_U32)MDrv_JPD_SetVPUCmd(E_JPD_CMD_RESUME);
        break;

    case E_MJPEG_CMD_STEP_PLAY:
        u32Ret = (MS_U32)MDrv_JPD_SetVPUCmd(E_JPD_CMD_STEP_PLAY);
        break;

    case E_MJPEG_CMD_SET_SPEED_TYPE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_SPEED_TYPE);
        break;

    case E_MJPEG_CMD_SET_SPEED:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_SPEED);
        break;

    case E_MJPEG_CMD_FLUSH_DISP_QUEUE:
        u32Ret = (MS_U32)MDrv_JPD_SetVPUCmd(E_JPD_CMD_FLUSH_DISP_QUEUE);
        break;

    case E_MJPEG_CMD_FREEZE_DISP:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_FREEZE_DISP);
        break;

    case E_MJPEG_CMD_ENABLE_AVSYNC:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_ENABLE_AVSYNC);
        break;

    case E_MJPEG_CMD_SET_AVSYNC_DELAY:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_AVSYNC_DELAY);
        break;

    case E_MJPEG_CMD_SET_AVSYNC_TOLERENCE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_AVSYNC_TOLERENCE);
        break;

    case E_MJPEG_CMD_SET_PTS_BASE:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_PTS_BASE);
        break;

    case E_MJPEG_CMD_SET_STC_BASE:
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_STC_BASE);
        break;

    case E_MJPEG_CMD_SET_BLUE_SCREEN:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_SET_BLUE_SCREEN);
        break;

    case E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX);
        break;

    case E_MJPEG_CMD_COMPENSATE_PTS:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_COMPENSATE_PTS);
        break;

    // Display Command Queue -- BEGIN
    case E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE:
#if _JPD_DECODER_RENDER_ARCH
        MDrv_JPD_SetVPUCmdArg(u32Arg);
#endif
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_ENABLE_DISP_CMD_QUEUE);
        break;

    case E_MJPEG_CMD_PUSH_DISP_CMD:
        MDrv_JPD_SetVPUCmdArg(u32Arg);
        MDrv_JPD_SetVPUCmd(E_JPD_CMD_PUSH_DISP_CMD);
        break;

    case E_MJPEG_CMD_GET_DISP_CMD_Q_VACANCY:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_GET_DISP_CMD_Q_VACANCY);
        break;
    // Display Command Queue -- END

    case E_MJPEG_CMD_IS_STEP_PLAY_DONE:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_STEP_PLAY_DONE);
        break;

    case E_MJPEG_CMD_IS_DISP_FINISH:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_DISP_FINISH);
        break;

    case E_MJPEG_CMD_IS_PLAYING:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_PLAYING);
        break;

    case E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_DISPLAY_QUEUE_FULL);
        break;

    case E_MJPEG_CMD_IS_AVSYNC_ON:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_AVSYNC_ON);
        break;

    case E_MJPEG_CMD_IS_REACH_AVSYNC:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_REACH_AVSYNC);
        break;

    case E_MJPEG_CMD_IS_FLUSH_DONE:
        u32Ret = MDrv_JPD_CheckVPUStatus(E_JPD_CMD_IS_FLUSH_DONE);
        break;

    default:
        printf("    !!! Unknown MJPEG Command !!!\n");
        u32Ret = 0xFFFFFFFF;
        break;
    }


    return u32Ret;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_BOOL MDrv_JPD_IsVPUOnMiu1(void)
{
    return HAL_JPD_IsVPUOnMiu1();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 MDrv_JPD_GetMiuBase(void)
{
    return HAL_JPD_GetMiuBase();
}

/******************************************************************************/
///Get ShareMemory address
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_U32 MDrv_JPD_GetShareMemAddr(void)
{
    MS_U32 u32ShareMemAddr = 0xFFFFFFFF;

    if (__u32FwAddr != 0xFFFFFFFF)
        u32ShareMemAddr = __u32FwAddr + JPD_SHARE_MEM_ST_OFFSET;
    else
        return 0xFFFFFFFF;

    u32ShareMemAddr = MS_PA2KSEG1((MS_U32)u32ShareMemAddr); //PhysicalAddr to VirtualAddr

    return u32ShareMemAddr;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_JPD_ClearShareMemory(void)
{
    MS_U32 u32Idx = 0, u32Size;
    volatile MS_U8 *u8Ptr = 0;

    u8Ptr = (volatile MS_U8 *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)u8Ptr==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    u32Size = sizeof(JPD_ShareMem);

    for (u32Idx = 0; u32Idx < u32Size; u32Idx++)
    {
        *u8Ptr = 0;
        u8Ptr++;
    }
    return;
}

/******************************************************************************/
///Get PTS table address
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_U32 MDrv_JPD_GetPtsTableAddr(void)
{
    MS_U32 u32PtsAddr = 0xFFFFFFFF;

    if (__u32FwAddr != 0xFFFFFFFF)
        u32PtsAddr = __u32FwAddr + JPD_PTS_TABLE_ST_OFFSET;
    else
        return 0xFFFFFFFF;

    u32PtsAddr = MS_PA2KSEG1((MS_U32)u32PtsAddr); //PhysicalAddr to VirtualAddr

    return u32PtsAddr;
}

/******************************************************************************/
///Reset PTS value
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_BOOL MDrv_JPD_ResetPTS(MS_U32 u32PtsBase)
{

    MDrv_JPD_SendVPUCommand(E_MJPEG_CMD_SET_PTS_BASE, u32PtsBase);

    return TRUE;
}

/******************************************************************************/
///Return PTS of current displayed frame
///@return PTS
/******************************************************************************/
MS_U32 MDrv_JPD_GetPTS(void)
{
    volatile JPD_ShareMem *pShareMem = NULL;

    pShareMem = (JPD_ShareMem *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }

    return pShareMem->infoDispFrame.u32TimeStamp;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_JPD_DispFrame(MS_U32 u32DispIdx)
{
#if _JPD_DECODER_RENDER_ARCH
    volatile JPD_ShareMem *pShareMem = NULL;
    pShareMem = (JPD_ShareMem *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    if (pShareMem->DispQueue[u32DispIdx].u32Status == E_JPD_DISPQ_STATUS_VIEW)
    {
        pShareMem->DispQueue[u32DispIdx].u32Status = E_JPD_DISPQ_STATUS_DISP;
    }
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_JPD_FreeFrame(MS_U32 u32DispIdx)
{
#if _JPD_DECODER_RENDER_ARCH
    volatile JPD_ShareMem *pShareMem = NULL;
    pShareMem = (JPD_ShareMem *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    if (pShareMem->DispQueue[u32DispIdx].u32Status == E_JPD_DISPQ_STATUS_VIEW)
    {
        pShareMem->DispQueue[u32DispIdx].u32Status = E_JPD_DISPQ_STATUS_FREE;
    }
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
#define JPD_DISPQ_PREFETCH_COUNT        2
static MS_U16 _u16DispQPtr = 0;
void* MDrv_JPD_GetNextDispFrame()
{
#if _JPD_DECODER_RENDER_ARCH
    volatile JPD_ShareMem *pShareMem = NULL;
    pShareMem = (JPD_ShareMem *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }
    MS_U16 u16QNum = pShareMem->u16DispQNumb;
    MS_U16 u16QPtr = pShareMem->u16DispQPtr;
    MJPG_FrameInfo *pFrm;

#if 0
    if (u16QNum > JPD_DISPQ_PREFETCH_COUNT) u16QNum = JPD_DISPQ_PREFETCH_COUNT;
#endif

    while (u16QNum != 0)
    {
        pFrm = (MJPG_FrameInfo*)&pShareMem->DispQueue[u16QPtr];

        if (pFrm->u32Status == E_JPD_DISPQ_STATUS_INIT)
        {
            _u16DispQPtr = u16QPtr;
            pFrm->u32Status = E_JPD_DISPQ_STATUS_VIEW; /////Change its state!!
            JPD_DEBUG_DRV_MSG("FrameDone: %d, pFrm=0x%lx, timestamp=%ld\n", u16QPtr, (MS_U32)pFrm, pFrm->u32TimeStamp);
            return pFrm;
        }
        u16QNum--;
        u16QPtr++;
        if (u16QPtr == pShareMem->u16DispQSize) u16QPtr = 0; //wrap to the begin
    }
#endif
    return NULL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MDrv_JPD_GetNextDispQPtr()
{
    return _u16DispQPtr;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_JPD_GetDispFrameInfo(void *pInfo)
{
    MJPG_FrameInfo *pFrmInfo = (MJPG_FrameInfo *)pInfo;

    volatile JPD_ShareMem *pShareMem = NULL;
    pShareMem = (JPD_ShareMem *)MDrv_JPD_GetShareMemAddr();
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        JPD_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    pFrmInfo->u32LumaAddr = pShareMem->infoDispFrame.u32LumaAddr;
    pFrmInfo->u32ChromaAddr = pShareMem->infoDispFrame.u32ChromaAddr;
    pFrmInfo->u32TimeStamp = pShareMem->infoDispFrame.u32TimeStamp;
    pFrmInfo->u32ID_L = pShareMem->infoDispFrame.u32ID_L;
    pFrmInfo->u32ID_H = pShareMem->infoDispFrame.u32ID_H;
}

void MDrv_JPD_EnableTurboFWMode(MS_BOOL bEnable)
{
    gbEnableTurboFWMode = bEnable;
}

JPD_Return MDrv_JPD_InitFw(MS_PHYADDR u32Addr, MS_U32 u32Size, MS_PHYADDR u32BinAddr, MS_U8 u8FwSrcType)
{
    MS_BOOL fw_on_miu1 = FALSE;

    JPD_DEBUG_DRV_MSG("MDrv_JPD_InitFw u32Addr = 0x%lx, u32Size = 0x%lx, u32BinAddr = 0x%lx, u8FwSrcType = %d\n", u32Addr, u32Size, u32BinAddr, u8FwSrcType);

    if (u32Addr > JPD_MIU_ADDR_MASK)
    {
        fw_on_miu1 = TRUE;
    }

#if !(defined(CHIP_T2) || defined(CHIP_T7))
    if(FALSE == HAL_JPD_InitVPU(fw_on_miu1))
#else
    if(FALSE == HAL_JPD_InitVPU())
#endif
    {
        JPD_DEBUG_DRV_ERR("HAL_JPD_InitVPU failed!!!\n");
        return E_JPD_INVALID_PARAM;
    }

    // Reset VPU
    if(!HAL_JPD_RstVPU())
    {
        JPD_DEBUG_DRV_ERR("HAL_JPD_RstVPU failed!!!\n");
        return E_JPD_FAILED;
    }

    // Load JPD firmware
    __u32FwAddr = u32Addr;
    if(FALSE == HAL_JPD_LoadCode(u32Addr, u32Size, u32BinAddr, u8FwSrcType))
    {
        JPD_DEBUG_DRV_ERR("HAL_JPD_LoadCode failed!!!\n");
        return E_JPD_FAILED;
    }

    // Set VPU base and endian
    HAL_JPD_ConfigVPU(u32Addr, 1); // 1 for little endian

    // Clear ShareMemory before releasing VPU
    MDrv_JPD_ClearShareMemory();

    // Release VPU
    HAL_JPD_ReleaseVPU();
    return E_JPD_OK;
}

MJPEG_MSTAR_CHIP_ID MDrv_JPD_Get_Chip_ID(void)
{
    return HAL_JPD_Get_Chip_ID();
}


