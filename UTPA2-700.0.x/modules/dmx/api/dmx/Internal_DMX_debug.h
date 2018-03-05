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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX.c
/// @brief  Demux API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_DMX_DBG_H__
#define __API_DMX_DBG_H__

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"

#define DMX_DEBUG 0

#if DMX_DEBUG
MS_BOOL _DMX_DBG_Record_Stop(void);
MS_BOOL _DMX_DBG_Record_Start(const char[], MS_U32*, MS_U32, MS_PHYADDR, MS_U32, MS_BOOL, MS_BOOL);
void _DMX_DBG_LogSel(MS_U32 u32Flag);
void _DMX_DBG_PidMonitor(MS_U32 *u32Pids, MS_U32 u32Num);
void _DMX_Debug(char path[]);
MS_BOOL MApi_DMX_DBG_IsFltMonitor(MS_U32 u32DmxId);


typedef struct
{
    int         id;
    const char *str;
} DMX_DMX_INPUT_STR, DMX_AV_FIFO_STR;

typedef struct
{
    MS_U32      u32DmxId;
    MS_U32      u32TimeCreate;
//    MS_U32      u32TimeLastEnable;
//    MS_U32      u32TimeLastDisable;
//    MS_U32      u32TimeActive; // enable time
    MS_U32      u32Int_rdy;
    MS_U32      u32Int_ovf;
    MS_U32      u32Int_crc;
    MS_U32      u32CB_rdy;
    MS_U32      u32CB_ovf;
} DMX_FLT_DBG_INFO;
//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[0;32;31m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[0;32;34m"
#define COLOR_BROWN "\033[0;33m"

#define COLOR_DBG   COLOR_RED
#define dmx_dbg_print(fmt, ...) ULOGD("DMX", COLOR_DBG fmt COLOR_NONE, ##__VA_ARGS__)

//#define DMX_DBG_ENTRY() MsOS_ObtainMutex(s32DmxDbgMutex, MSOS_WAIT_FOREVER)
//#define DMX_DBG_EXIT() MsOS_ReleaseMutex(s32DmxDbgMutex)

#define DMX_DBG_ENTRY()
#define DMX_DBG_EXIT()

extern MS_U32 u32PidMonitor[];
extern MS_U32 u32NumFltMonitor;
extern MS_S32 s32DmxDbgMutex;

#ifndef dmx_dbg_IsFltMonitor
#define dmx_dbg_IsFltMonitor(u32DmxId, bRet) {\
        int tempii = 0; \
        bRet = FALSE; \
        if((u32NumFltMonitor == 0) && (u32DmxDbgFltId == 0xFFFFFFFF))bRet = TRUE; \
        if((u32DmxId == u32DmxDbgFltId) && (u32DmxDbgFltId != 0xFFFFFFFF))bRet = TRUE;\
        for(tempii = 0; tempii <u32NumFltMonitor; tempii++) {                             \
            DMX_DBG_ENTRY();                                                             \
            if(_pdmx_res->_FltList[(u32DmxId)].u16Pid == (MS_U16)u32PidMonitor[tempii]) \
                {DMX_DBG_EXIT();bRet = TRUE;}                                          \
            DMX_DBG_EXIT();}}

#endif

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------

#define MAX_NUM_CP_DATA_DUMP 128

    #define DMX_DBG_LOG_FLT_OPEN    0x00000001
    #define DMX_DBG_LOG_FLT_PID     0x00000002
    #define DMX_DBG_LOG_FLT_CLOSE   0x00000004
    #define DMX_DBG_LOG_FLT_ENABLE  0x00000008
    #define DMX_DBG_LOG_FLT_INFO    0x00000010
    #define DMX_DBG_LOG_FLT_CB      0x00000020

    #define DMX_DBG_LOG_FILEIN      0x00000080 // file in operation

    #define DMX_DBG_LOG_COPYDATA    0x00000100 // section buffer address
    #define DMX_DBG_LOG_COPYDATA1   0x00000200 // dump section data (console / file)

    #define DMX_DBG_LOG_FLOWSET     0x00000400
    #define DMX_DBG_LOG_FLT_SEC_OVF 0x00000800
    #define DMX_DBG_LOG_FLT_SEC_RDY 0x00001000
    #define DMX_DBG_LOG_STC_CHANGE  0x00002000
    #define DMX_DBG_LOG_PCR         0x00004000

    #define DMX_DBG_LOG_INTR        0x00080000
    #define DMX_DBG_LOG_FI_READ     0x00100000 // file in read address
    #define DMX_DBG_LOG_PVR_WRITE   0x00200000 // pvr write address
    #define DMX_DBG_LOG_AVFIFO      0x00400000 // a/v fifo reset and status

    #define DMX_DBG_LOG_AV_DISCONTI 0x02000000
    #define DMX_DBG_LOG_AV_DROP     0x04000000
    #define DMX_DBG_LOG_LOCK        0x08000000
    #define DMX_DBG_LOG_STC         0x10000000
    #define DMX_DBG_LOG_AV_PKT      0x20000000
    #define DMX_DBG_LOG_AV_OVF      0x40000000
    #define DMX_DBG_LOG_AEON_ALIVE  0x80000000

//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
#endif // DMX_DEBUG
#endif // __API_DMX_DBG_H__
