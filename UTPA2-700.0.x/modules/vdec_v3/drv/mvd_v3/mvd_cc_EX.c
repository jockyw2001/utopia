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
/// @file  drvmvd_cc.h
/// @brief Driver interface for accessing the MPEG Closed Caption decoder.
/// @author MStar Semiconductor Inc.
///
///- Providing MPEG Closed Caption decoder command functions for controlling firmware.
///- MPEG Closed Caption decoder provides Digital 608 & 708 CC Data via accessing Ring Buffer
///
////////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
//  Include Files
//--------------------------------------------------------------------------------------------------
// Common Definition
#ifdef REDLION_LINUX_KERNEL_ENVI
#include "drvMVD_Common.h"
#else
#include "MsCommon.h"
#endif
#include "drvMVD_EX.h"

// Internal Definition
#include "halMVD_EX.h"
#include "drvmvd_cc.h"
#include "mvd4_interface.h" //firmware header
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "halCHIP.h"
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define MVDCC_EXHANDLE(y)     y

//----------------------------------------------------------------------------
/* MVD Command register - Command Definitions */
//----------------------------------------------------------------------------
#define CMD_SET_CCBUF_STARTHWADDR       CMD_USER_BUF_START
#define CMD_SET_CCBUF_TOTALSIZE         CMD_USER_BUF_SIZE
#define CMD_SET_CCTYPE_PARSING          CMD_USER_BUF_SIZE
#define CMD_GET_CCBUF_HWADDR            CMD_RD_USER_WP
#define CMD_GET_CCBUF_WRAPCOUNT         CMD_RD_USER_WP

#define CMD_SET_CCBUF_SWADDR            CMD_WD_USER_RP
#define CMD_GET_CCBUF_PACKETCOUNT       CMD_RD_CC_PKTCNT
#define CMD_GET_OVERFLOW_STATUS         CMD_RD_CC_OV

void MDrv_CC_CM_SetMVDRB_SWAddr(MS_U32 u32Id, MS_U32 u32ReadAddress, MS_U8 u8CC608);
MS_U32 MDrv_CC_CM_GetMVDRB_HWAddr(MS_U32 u32Id, MS_U8 u8CC608);
extern MS_U8 MVD_GetHalIdx(MS_U32 u32Id);

//******************************************************************************/
//* Local definition
//******************************************************************************/
#define MVD_READY_TEST_COUNT        10000
#define MVD_CCRB_PACKET_LENGTH      8

//******************************************************************************/
//* Declaring Variable
//******************************************************************************/
MS_U32 CCMVD_RINGBUFFER_START_ADR[2];
MS_U32 CCMVD_RINGBUFFER_LEN[2];

static MS_U32 volatile u32SW_Address[2], u32HW_Address[2];
static MS_U8 u8ParsingStatus = 0xff;

static MS_BOOL bCC708Enable; //fixme

// SH@20110111, Fixed the issues that move CC buffer from MIU0 to MIU1
// MIU1: buffer to MVD IP, don't need to add the MIU1's offset(512 Mb), but to CC flow's buffer, need to add up

#if defined(CHIP_MONACO)
static MS_U8 u8BufSelMiu = 0xff;
#define MVD_ON_MIU     HAL_MVD_GetFWSelMiu()
#else
static MS_BOOL bBufMiu0 = 0xff;
#define MIU1_BASEADDR   HAL_MVD_GetMiu1BaseAdd()
#define MVD_ON_MIU1     HAL_MVD_GetFWSelMiu1()
#endif

#define _IS_MSTAR_CC(x) ((0==(x)) || (1==(x)))
///////////////////////////////////////////////////////////////////////////////
/// Closed Caption Driver Initialization
///////////////////////////////////////////////////////////////////////////////
void MDrv_CC_Init(MS_U32 u32Id)
{
    if (u8ParsingStatus == 0xff)
        u8ParsingStatus = 0x00;
}

///////////////////////////////////////////////////////////////////////////////
/// Set MVD Ring Buffer's Address
/// @param u32StartAddress \b IN Ring Buffer's Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
#if defined(CHIP_MONACO)
void MDrv_CC_CM_SetMVDRB_HWAddr(MS_U32 u32Id, MS_U32 u32StartAddress, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    MS_U32 u32StartOffset;
    MS_U8  u8MiuSel;
    
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);

    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));

    // SH@20110111, get the first HW buffer to determine the MIU0 or MIU1 for CC buffer. Only once
    if(u8BufSelMiu == 0xff)
    {
        _phy_to_miu_offset(u8MiuSel, u32StartOffset, u32StartAddress);

        u8BufSelMiu = u8MiuSel;
    }

    if(MVD_ON_MIU == E_CHIP_MIU_0)    
    {
        if(u8BufSelMiu == E_CHIP_MIU_0) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_1)    // SH@20110111, mvd code binary in MIU1
    {
        if(u8BufSelMiu == E_CHIP_MIU_0) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu1_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu1_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu1_base;
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_2)    // SH@20110111, mvd code binary in MIU1
    {
        if(u8BufSelMiu == E_CHIP_MIU_0) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu2_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu2_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress ;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu2_base;
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_3)    // SH@20110111, mvd code binary in MIU1
    {
        if(u8BufSelMiu == E_CHIP_MIU_0) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu3_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu3_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | u32Miu3_base;
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3) 
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress ;
        }
    }

    if(u8BufSelMiu == E_CHIP_MIU_0)
    {
        u32MVDCC_Temp1 = (u32StartAddress >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_1)
    {
        u32MVDCC_Temp1 = u32StartAddress - u32Miu1_base ;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_2)
    {
        u32MVDCC_Temp1 = u32StartAddress - u32Miu2_base ;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_3)
    {
        u32MVDCC_Temp1 = u32StartAddress - u32Miu3_base ;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }


    mvdcmd.Arg4 = H_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    mvdcmd.Arg2 = L_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg1 = H_WORD(u32MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u32MVDCC_Temp1);
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCBUF_STARTHWADDR, &mvdcmd)== TRUE)
    {
        u32SW_Address[u8CC608] = u32StartAddress;
        u32HW_Address[u8CC608] = u32StartAddress;
        MDrv_CC_CM_SetMVDRB_SWAddr(u32Id, u32StartAddress, u8CC608);
    }
    else
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCBUF_STARTHWADDR));
        MVDCC_EXHANDLE(printf("\nF:S-HA"));
    }
}
#else
void MDrv_CC_CM_SetMVDRB_HWAddr(MS_U32 u32Id, MS_U32 u32StartAddress, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));

    // SH@20110111, get the first HW buffer to determine the MIU0 or MIU1 for CC buffer. Only once
    if(bBufMiu0 == 0xff)
    {
        if((u32StartAddress&MIU1_BASEADDR) == MIU1_BASEADDR)
        {
            bBufMiu0 = false;
        }
        else
        {
            bBufMiu0 = true;
        }
    }

    if(MVD_ON_MIU1)    // SH@20110111, mvd code binary in MIU1
    {
        if(bBufMiu0) // SH@20110111, if CC buffer in MIU0, enable here
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress | MIU1_BASEADDR;
        }
        else // CC buffer in MIU1
        {
            CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
        }
    }
    else
    {
        CCMVD_RINGBUFFER_START_ADR[u8CC608] = u32StartAddress;
    }

    if(bBufMiu0)
    {
        u32MVDCC_Temp1 = (u32StartAddress >> 3);
    }
    else // CC buffer in MIU1
    {
        u32MVDCC_Temp1 = u32StartAddress - MIU1_BASEADDR ;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }

    mvdcmd.Arg4 = H_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    mvdcmd.Arg2 = L_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg1 = H_WORD(u32MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u32MVDCC_Temp1);
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCBUF_STARTHWADDR, &mvdcmd)== TRUE)
    {
        u32SW_Address[u8CC608] = u32StartAddress;
        u32HW_Address[u8CC608] = u32StartAddress;
        MDrv_CC_CM_SetMVDRB_SWAddr(u32Id, u32StartAddress, u8CC608);
    }
    else
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCBUF_STARTHWADDR));
        MVDCC_EXHANDLE(printf("\nF:S-HA"));
    }
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// Set MVD Ring Buffer's SW Address as HW Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
void MDrv_CC_CM_SyncMVDRB_SWAddr2HWAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    MDrv_CC_CM_SetMVDRB_SWAddr(u32Id, u32HW_Address[u8CC608], u8CC608);
}

///////////////////////////////////////////////////////////////////////////////
/// Set CC Type to MVD CC FW Driver
/// @param u8Operation \b IN Digital Closed Caption Type
/// @param u16BufferSize \b IN buffer size of the given hardware parser
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
void MDrv_CC_CM_SetParsingType(MS_U32 u32Id, MS_U8 u8Operation, MS_U16 u16BufferSize, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U16 u16MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));
    CCMVD_RINGBUFFER_LEN[u8CC608] = u16BufferSize;
    u16MVDCC_Temp1 = (u16BufferSize >> 3);

    u8ParsingStatus |= u8Operation;
    u8ParsingStatus &= 0x07;
    mvdcmd.Arg1 = H_WORD(u16MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u16MVDCC_Temp1);
    mvdcmd.Arg2 = (MS_U8)(u8ParsingStatus);
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCTYPE_PARSING, &mvdcmd) != TRUE)
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCTYPE_PARSING));
        MVDCC_EXHANDLE(printf("\nF:SPT"));
    }
    else
    {
        if (u8CC608==0x00)
        {
            bCC708Enable = TRUE;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Stop MVD CC FW Driver
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
void MDrv_CC_CM_DisableParsing(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U16 u16MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));
    u16MVDCC_Temp1 = CCMVD_RINGBUFFER_LEN[u8CC608];
    u16MVDCC_Temp1 = (u16MVDCC_Temp1 >> 3);

    mvdcmd.Arg1 = H_WORD(u16MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u16MVDCC_Temp1);

    if (u8CC608==0)
    {
        u8ParsingStatus &= ~0x04;
        bCC708Enable = FALSE;
    }
    else
        u8ParsingStatus &= ~0x03;

    u8ParsingStatus &= 0x07;
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    mvdcmd.Arg2 = (MS_U8)(u8ParsingStatus);
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCTYPE_PARSING, &mvdcmd) != TRUE)
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCTYPE_PARSING));
        MVDCC_EXHANDLE(printf("\nF:DisP"));
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Get Hardware Pointer of MVD CC Ring Buffer
/// Return value:: The HW Pointer Address of MVD CC Ring Buffer
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
#if defined(CHIP_MONACO)
MS_U32 MDrv_CC_CM_GetMVDRB_HWAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U32 u32MVDCC_Temp1 = 0;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CmdArg mvdcmd;
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);
    
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_GET_CCBUF_HWADDR, &mvdcmd)== TRUE)
    {
        if (_IS_MSTAR_CC(u8CC608))
        {
            u32MVDCC_Temp1 = COMBU32(mvdcmd.Arg4, mvdcmd.Arg2, mvdcmd.Arg1, mvdcmd.Arg0);
        }
        else //u8CC608 is 2 or 3
        {
            u32MVDCC_Temp1 = COMBU32(mvdcmd.Arg3, mvdcmd.Arg2, mvdcmd.Arg1, mvdcmd.Arg0);
        }
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 << 3);
        
        if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            return (u32MVDCC_Temp1);
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            return (u32MVDCC_Temp1|u32Miu1_base);
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            return (u32MVDCC_Temp1|u32Miu2_base);
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            return (u32MVDCC_Temp1|u32Miu3_base);
        }
    }
    else
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_GET_CCBUF_HWADDR));
        MVDCC_EXHANDLE(printf("\nF:GHAV"));
    }

    return 0xffffffff;
}
#else
MS_U32 MDrv_CC_CM_GetMVDRB_HWAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U32 u32MVDCC_Temp1 = 0;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_GET_CCBUF_HWADDR, &mvdcmd)== TRUE)
    {
        if (_IS_MSTAR_CC(u8CC608))
        {
            u32MVDCC_Temp1 = COMBU32(mvdcmd.Arg4, mvdcmd.Arg2, mvdcmd.Arg1, mvdcmd.Arg0);
        }
        else //u8CC608 is 2 or 3
        {
            u32MVDCC_Temp1 = COMBU32(mvdcmd.Arg3, mvdcmd.Arg2, mvdcmd.Arg1, mvdcmd.Arg0);
        }
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 << 3);
        if(bBufMiu0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            return (u32MVDCC_Temp1);
        }
        else // CC buffer in MIU1
        {
            return (u32MVDCC_Temp1|MIU1_BASEADDR);
        }
    }
    else
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_GET_CCBUF_HWADDR));
        MVDCC_EXHANDLE(printf("\nF:GHAV"));
    }

    return 0xffffffff;
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// Get Overflow Status of MVD CC FW Driver
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: Overflow Status Flag
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_CC_CM_GetOverflowStatus(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));
    mvdcmd.Arg3 = (MS_U8)u8CC608;
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_GET_OVERFLOW_STATUS, &mvdcmd) == TRUE)
    {
        if ((mvdcmd.Arg0& 0x01) == 0)
            return (FALSE);
        else
            return (TRUE);
    }
    else
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_GET_OVERFLOW_STATUS));
        MVDCC_EXHANDLE(printf("\nF:GOV"));
    }
    return (FALSE);
}

///////////////////////////////////////////////////////////////////////////////
/// Set Software Pointer of MVD CC Ring Buffer
/// @param u32ReadAddress \b IN SW Pointer Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
#if defined(CHIP_MONACO)
void MDrv_CC_CM_SetMVDRB_SWAddr(MS_U32 u32Id, MS_U32 u32ReadAddress, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);
    
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));

    u32SW_Address[u8CC608] = u32ReadAddress;

    if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        u32MVDCC_Temp1 = (u32ReadAddress >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        u32MVDCC_Temp1 = u32ReadAddress - u32Miu1_base;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        u32MVDCC_Temp1 = u32ReadAddress - u32Miu2_base;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }
    else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        u32MVDCC_Temp1 = u32ReadAddress - u32Miu3_base;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }

    mvdcmd.Arg3 = (MS_U8)u8CC608;
    mvdcmd.Arg2 = L_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg1 = H_WORD(u32MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u32MVDCC_Temp1);
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCBUF_SWADDR, &mvdcmd) != TRUE)
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCBUF_SWADDR));
        MVDCC_EXHANDLE(printf("\nF:SSA"));
    }
}
#else
void MDrv_CC_CM_SetMVDRB_SWAddr(MS_U32 u32Id, MS_U32 u32ReadAddress, MS_U8 u8CC608)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32MVDCC_Temp1 = 0;
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));

    u32SW_Address[u8CC608] = u32ReadAddress;

    if(bBufMiu0)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        u32MVDCC_Temp1 = (u32ReadAddress >> 3);
    }
    else // CC buffer in MIU1
    {
        u32MVDCC_Temp1 = u32ReadAddress - MIU1_BASEADDR;
        u32MVDCC_Temp1 = (u32MVDCC_Temp1 >> 3);
    }

    mvdcmd.Arg3 = (MS_U8)u8CC608;
    mvdcmd.Arg2 = L_DWORD(u32MVDCC_Temp1);
    mvdcmd.Arg1 = H_WORD(u32MVDCC_Temp1);
    mvdcmd.Arg0 = L_WORD(u32MVDCC_Temp1);
    SET_DECNUM(mvdcmd, u8HalIdx);
    if (HAL_MVD_MVDCommand(CMD_SET_CCBUF_SWADDR, &mvdcmd) != TRUE)
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_SET_CCBUF_SWADDR));
        MVDCC_EXHANDLE(printf("\nF:SSA"));
    }
}
#endif

#if defined(CHIP_MONACO)
#else
#define __Offset2PA(x)      ((MVD_ON_MIU1)?((x)|MIU1_BASEADDR):(x))
#endif
///////////////////////////////////////////////////////////////////////////////
/// Get HW Pointer Address of MVD CC FW Driver
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: HW Pointer Address
///////////////////////////////////////////////////////////////////////////////

#if defined(CHIP_MONACO)
MS_U32 MDrv_CC_PM_GetMVDRB_WriteAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);
    
    u32HW_Address[u8CC608] = MDrv_CC_CM_GetMVDRB_HWAddr(u32Id, u8CC608);

    if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu3_base);
        }  
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu3_base);
        }  
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu3_base);
        } 
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32HW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32HW_Address[u8CC608]);
        } 
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
#else
MS_U32 MDrv_CC_PM_GetMVDRB_WriteAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    u32HW_Address[u8CC608] = MDrv_CC_CM_GetMVDRB_HWAddr(u32Id, u8CC608);

    if(bBufMiu0)
    {
        return (MS_U32)__Offset2PA(u32HW_Address[u8CC608]);
    }
    else
    {
        return (MS_U32)(u32HW_Address[u8CC608]);
    }
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// Get SW Pointer Address of MVD CC FW Driver
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
/// Return value:: SW Pointer Address
///////////////////////////////////////////////////////////////////////////////

#if defined(CHIP_MONACO)
MS_U32 MDrv_CC_PM_GetMVDRB_ReadAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);
    
    if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu3_base);
        } 
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu3_base);
        }
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu3_base);
        }  
        else
        {
            return 0;
        }
    }
    else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
    {
        if(MVD_ON_MIU == E_CHIP_MIU_0)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_1)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu1_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_2)
        {
            return (MS_U32)(u32SW_Address[u8CC608]|u32Miu2_base);
        }
        else if(MVD_ON_MIU == E_CHIP_MIU_3)
        {
            return (MS_U32)(u32SW_Address[u8CC608]);
        }
        else
        {
            return 0;
        }
        
    }
    else
    {
        return 0;
    }
}
#else
MS_U32 MDrv_CC_PM_GetMVDRB_ReadAddr(MS_U32 u32Id, MS_U8 u8CC608)
{
    if(bBufMiu0)
    {
        return (MS_U32)__Offset2PA(u32SW_Address[u8CC608]);
    }
    else
    {
        return (MS_U32)(u32SW_Address[u8CC608]);
    }
}
#endif
///////////////////////////////////////////////////////////////////////////////
/// Set Software Pointer of MVD CC Ring Buffer to MVD CC FW Driver
/// @param u32EachPacketSize \b IN SW Pointer Address
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
#if defined(CHIP_MONACO)
void MDrv_CC_PM_SetMVDRB_ReadAddr(MS_U32 u32Id, MS_U32 u32EachPacketSize, MS_U8 u8CC608)
{
    MS_U32 u32MVDCC_Temp1 = 0;
    MS_U32 u32ParsedDataSize;
    MS_U32 u32Miu1_base = 0;
    MS_U32 u32Miu2_base = 0;
    MS_U32 u32Miu3_base = 0;
    
    _miu_offset_to_phy(E_CHIP_MIU_1,0,u32Miu1_base);
    _miu_offset_to_phy(E_CHIP_MIU_2,0,u32Miu2_base);
    _miu_offset_to_phy(E_CHIP_MIU_3,0,u32Miu3_base);

    // Get the Parsed Data Size
    if ((u32EachPacketSize % MVD_CCRB_PACKET_LENGTH) != 0)
    {
        u32ParsedDataSize = MVD_CCRB_PACKET_LENGTH * ((u32EachPacketSize / MVD_CCRB_PACKET_LENGTH)+1);
    }
    else
    {
        u32ParsedDataSize = u32EachPacketSize;
    }

    // Add the SW Address
    if(MVD_ON_MIU == E_CHIP_MIU_0)
    {
        if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_1)
    {
        if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu1_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu1_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu1_base - 1); // to Phy address
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_2)
    {
        if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu2_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu2_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu2_base - 1); // to Phy address
        }
    }
    else if(MVD_ON_MIU == E_CHIP_MIU_3)
    {
        if(u8BufSelMiu == E_CHIP_MIU_0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu3_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_1)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu3_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_2)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (u32Miu3_base - 1); // to Phy address
        }
        else if(u8BufSelMiu == E_CHIP_MIU_3)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
    }

    if ((u32SW_Address[u8CC608] + u32ParsedDataSize) < (u32MVDCC_Temp1 + CCMVD_RINGBUFFER_LEN[u8CC608]))
    {
        u32SW_Address[u8CC608] += u32ParsedDataSize;
    }
    else
    {
        u32SW_Address[u8CC608] = (u32ParsedDataSize + u32SW_Address[u8CC608] - CCMVD_RINGBUFFER_LEN[u8CC608]);
    }

#if 0
    if(u32HW_Address - u32SW_Address >= 512)
    {
        printf("Sync:0x%08Lx", u32SW_Address);
        u32SW_Address = u32HW_Address;
        printf("-->0x%08Lx\n", u32SW_Address);
    }
#endif

    MDrv_CC_CM_SetMVDRB_SWAddr(u32Id, u32SW_Address[u8CC608], u8CC608);
}
#else
void MDrv_CC_PM_SetMVDRB_ReadAddr(MS_U32 u32Id, MS_U32 u32EachPacketSize, MS_U8 u8CC608)
{
    MS_U32 u32MVDCC_Temp1 = 0;
    MS_U32 u32ParsedDataSize;

    // Get the Parsed Data Size
    if ((u32EachPacketSize % MVD_CCRB_PACKET_LENGTH) != 0)
    {
        u32ParsedDataSize = MVD_CCRB_PACKET_LENGTH * ((u32EachPacketSize / MVD_CCRB_PACKET_LENGTH)+1);
    }
    else
    {
        u32ParsedDataSize = u32EachPacketSize;
    }

    // Add the SW Address
    if(MVD_ON_MIU1)
    {
        if(bBufMiu0)  // SH@20110111, if CC buffer in MIU0, enable here
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] & (MIU1_BASEADDR - 1); // to Phy address
        }
        else // CC buffer in MIU1
        {
            u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608] ;//& (MIU1_BASEADDR - 1); // to Phy address
        }
    }
    else
    {
        u32MVDCC_Temp1 = CCMVD_RINGBUFFER_START_ADR[u8CC608];
    }

    if ((u32SW_Address[u8CC608] + u32ParsedDataSize) < (u32MVDCC_Temp1 + CCMVD_RINGBUFFER_LEN[u8CC608]))
    {
        u32SW_Address[u8CC608] += u32ParsedDataSize;
    }
    else
    {
        u32SW_Address[u8CC608] = (u32ParsedDataSize + u32SW_Address[u8CC608] - CCMVD_RINGBUFFER_LEN[u8CC608]);
    }

#if 0
    if(u32HW_Address - u32SW_Address >= 512)
    {
        printf("Sync:0x%08Lx", u32SW_Address);
        u32SW_Address = u32HW_Address;
        printf("-->0x%08Lx\n", u32SW_Address);
    }
#endif

    MDrv_CC_CM_SetMVDRB_SWAddr(u32Id, u32SW_Address[u8CC608], u8CC608);
}
#endif
void MDrv_CC_CM_GetInfo(MS_U32 u32Id, MS_U32 selector, MS_U8 type, MS_U32 *p1, MS_U32 *p2)
{
    // Functionality select
    switch (selector)
    {
        case CC_SELECTOR_708_SW:
            *p1 = (MS_U32)bCC708Enable;
            break;

#if defined( MSOS_TYPE_LINUX )
        case CC_SELECTOR_CCMVD_RINGBUFFER:
            *p1 = (MS_U32)CCMVD_RINGBUFFER_START_ADR[type];
            *p2 = (MS_U32)CCMVD_RINGBUFFER_LEN[type];
            break;
#endif

        default:
            *p1 = *p2 = 0;
            break;
    }
}

MS_BOOL MDrv_CC_CM_Get708Sw(MS_U32 u32Id)
{
    return bCC708Enable;
}

MS_BOOL MDrv_CC_CM_IsMvdRstDone(MS_U32 u32Id, MS_U8 type)
{
    MS_BOOL bRet = FALSE;
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);

    if(type == E_CC_MVD_TYPE_608)   // CC608
    {
        if(!HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_RST_CC608))
        {
            bRet = TRUE;
        }
    }
    else            // CC708
    {
        if(!HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_RST_CC708))
        {
            bRet = TRUE;
        }
    }

    return bRet;
}


///////////////////////////////////////////////////////////////////////////////
/// Enhance CC info mode, only support cc608
/// @param bEnable \b IN
///   - # TRUE for enhance to dump the pts/tmp_ref info
///   - # FALSE for original
///////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_CC_InfoEnhanceMode(MS_U32 u32Id,MS_BOOL bEnable)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U8 u8Type = E_CC_MVD_TYPE_608;
    MVD_CmdArg mvdcmd;
    memset((void *)&mvdcmd, 0, sizeof(MVD_CmdArg));

    mvdcmd.Arg0 = bEnable;
    mvdcmd.Arg3 = u8Type;
    SET_DECNUM(mvdcmd, u8HalIdx);
   
    if (HAL_MVD_MVDCommand(CMD_EN_CC_INFO_ENHANCE, &mvdcmd) != TRUE)
    {
        MVDCC_EXHANDLE(printf("%s: cmd 0x%x fail!!\n", __FUNCTION__, CMD_EN_CC_INFO_ENHANCE));
        return FALSE;
    }
 
    return TRUE;
}
#endif
