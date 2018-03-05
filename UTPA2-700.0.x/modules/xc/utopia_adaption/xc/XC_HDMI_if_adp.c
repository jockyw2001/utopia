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

////////////////////////////////////////////////////////////////////////////////
/// file    XC_HDMI_if_adp.c
/// @brief  Scaler API layer Interface.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_HDMI_IF_ADP_C_
#define _XC_HDMI_IF_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/compat.h>

#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvXC_HDMI_if.h"
#include "drvXC_HDMI_if_v2.h"
#include "apiHDMIRx_private.h"
#include "apiXC_v2.h"
#include "XC_HDMI_if_adp.h"
#include "XC_adp.h"
#include "mvideo_context.h"

//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MADP_HDMIRX_MSG_INFO(format, args...)       ULOGI("HDMIRX", format, ##args)
#define MADP_HDMIRX_MSG_WARNING(format, args...)    ULOGW("HDMIRX", format, ##args)
#define MADP_HDMIRX_MSG_DEBUG(format, args...)      ULOGD("HDMIRX", format, ##args)
#define MADP_HDMIRX_MSG_ERROR(format, args...)      ULOGE("HDMIRX", format, ##args)
#define MADP_HDMIRX_MSG_FATAL(format, args...)      ULOGF("HDMIRX", format, ##args)

#else
#define MADP_HDMIRX_MSG_INFO(format, args...)       printf(format, ##args)
#define MADP_HDMIRX_MSG_WARNING(format, args...)    printf(format, ##args)
#define MADP_HDMIRX_MSG_DEBUG(format, args...)      printf(format, ##args)
#define MADP_HDMIRX_MSG_ERROR(format, args...)      printf(format, ##args)
#define MADP_HDMIRX_MSG_FATAL(format, args...)      printf(format, ##args)

#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER(a,b,c)                    if(copy_from_user(a,b,c) != 0) { break; }
#define CPY_TO_USER(a,b,c)                      if(copy_to_user(a,b,c) != 0) { break; }
#else
#define CPY_FROM_USER                           memcpy
#define CPY_TO_USER                             memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define COMPAT_PTR(a)                           compat_ptr(a)
#define COMPAT_NEXT_PTR(a)                      (*((MS_U32*)compat_ptr((unsigned long)a)))
#define IS_CMP_TASK()                           is_compat_task()
#define CMP_CPY_FROM_USER(a,b,c)                if(copy_from_user(a, compat_ptr((unsigned long)b), c) != 0)  {  break; }
#define CMP_CPY_TO_USER(a,b,c)                  if(copy_to_user(compat_ptr((unsigned long)a), b, c) != 0) { break;  }
#define MS_COMPAT_HDMIRX                        compat_uptr_t

#else
#define COMPAT_PTR(a)                           (a)
#define COMPAT_NEXT_PTR(a)                      (*((MS_U32*)a))
#define IS_CMP_TASK()                           (FALSE)
#define CMP_CPY_FROM_USER                       CPY_FROM_USER
#define CMP_CPY_TO_USER                         CPY_TO_USER
#define MS_COMPAT_HDMIRX                        MS_U8*

#endif //CONFIG_COMPAT

//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------
typedef struct DLL_PACKED
{
    MS_U8 ucPortType;
    MS_U8 ucPortIdx;
    MS_COMPAT_HDMIRX pucData;
    MS_U32 dwDataLen;
    MS_COMPAT_HDMIRX pDummy;
} stHDCP22_SEND_MESSAGE_COMPAT, *pstHDCP22_SEND_MESSAGE_COMPAT;

typedef struct DLL_PACKED
{
    MS_U32 u32Cmd;
    MS_COMPAT_HDMIRX pBuf;
    MS_U32 u32BufSize;
    MS_BOOL bRet;
    MS_U8 ucHDMIInfoSource;
} stHDMI_CONTROL_COMPAT, *pstHDMI_CONTROL_COMPAT;

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_MS_U8, 0);
// E_XC_HDMI_IF_CMD_FUNC_CAPS -- stHDMI_FUNC_CAPS
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_FUNC_CAPS, 0);
// E_XC_HDMI_IF_CMD_INIT -- stHDMI_INITIAL_SETTING
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_INITIAL_SETTING, 0);
// E_XC_HDMI_IF_CMD_PKT_RESET -- stHDMI_PKT_RESET
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_PKT_RESET, 0);
// E_XC_HDMI_IF_CMD_PULLHPD -- stHDMI_PULLHPD
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_PULLHPD, 0);
// E_XC_HDMI_IF_CMD_GC_INFO -- stHDMI_GC_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GC_INFO, 0);
// E_XC_HDMI_IF_CMD_PACKET_RECEIVED -- stHDMI_PACKET_RECEIVED
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_PACKET_RECEIVED, 0);
// E_XC_HDMI_IF_CMD_GET_COLORFORMAT -- stHDMI_GET_COLORFORMAT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_COLORFORMAT, 0);
// E_XC_HDMI_IF_CMD_GET_COLORRANGE -- stHDMI_GET_COLORRANGE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_COLORRANGE, 0);
// E_XC_HDMI_IF_CMD_SET_EQ -- stHDMI_SET_EQ
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SET_EQ, 0);
// E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT -- stHDMI_SET_EQ_TO_PORT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SET_EQ_TO_PORT, 0);
// E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE -- stHDMI_AUDIO_MUTE_ENABLE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_AUDIO_MUTE_ENABLE, 0);
// E_XC_HDMI_IF_CMD_GET_ASPECTRATIO -- stHDMI_GET_ASPECTRATIO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_ASPECTRATIO, 0);
// E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO -- stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO, 0);
// E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER -- stHDMI_GET_AVIINFOFRAMEVER
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_AVIINFOFRAMEVER, 0);
// E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE -- stHDMI_ERR_STATUS_UPDATE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_ERR_STATUS_UPDATE, 0);
// E_XC_HDMI_IF_CMD_GET_TMDS_FREQ -- stHDMI_GET_TMDS_FREQ
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_TMDS_FREQ, 0);
// E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS -- stHDMI_GET_POLLINGSTATUS
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_POLLINGSTATUS, 0);
// E_XC_HDMI_IF_CMD_ISHDMI_MODE -- stHDMI_ISHDMI_MODE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_ISHDMI_MODE, 0);
// E_XC_HDMI_IF_CMD_GET_MID_INFO -- stHDMI_GET_MID_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_MID_INFO_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_MID_INFO, 1);
// E_XC_HDMI_IF_CMD_GET_PACKET_VALUE -- stHDMI_GET_PACKET_VALUE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_PACKET_VALUE, 1);
// E_XC_DVI_IF_CMD_CHANNELPHASESTATUS -- stDVI_CHANNELPHASESTATUS
UADP_SPT_NAMEnNXT_DEF(XC_DVI_CHANNELPHASESTATUS, 0);
// E_XC_DVI_IF_CMD_SOFTWARERESET -- stDVI_SOFTWARERESET
UADP_SPT_NAMEnNXT_DEF(XC_DVI_SOFTWARERESET, 0);
// E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT -- stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, 0);
// E_XC_DVI_IF_CMD_CLKPULLLOW -- stDVI_CLKPULLLOW
UADP_SPT_NAMEnNXT_DEF(XC_DVI_CLKPULLLOW, 0);
// E_XC_DVI_IF_CMD_SWITCHSRC -- stDVI_SWITCHSRC
UADP_SPT_NAMEnNXT_DEF(XC_DVI_SWITCHSRC, 0);
// E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ -- stHDMI_SETFORCLOCKLESSTHAN70MHZ
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SETFORCLOCKLESSTHAN70MHZ, 0);
// E_XC_HDMI_IF_CMD_DVI_ADJUST -- stHDMI_DVI_ADJUST
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_DVI_ADJUST, 0);
// E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT -- stHDMI_SETUPDATEPHASELINECOUNT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SETUPDATEPHASELINECOUNT, 0);
// E_XC_HDCP_IF_CMD_ENABLE -- stHDCP_ENABLE
UADP_SPT_NAMEnNXT_DEF(XC_HDCP_ENABLE, 0);
// E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE -- stHDMI_SET_HDCPENABLE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SET_HDCPENABLE, 0);
// E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY -- stHDCP_INITPRODUCTIONKEY
UADP_SPT_NAMEnNXT_DEF(XC_HDCP_KEY_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDCP_INITPRODUCTIONKEY, 1);
// E_XC_HDCP_IF_CMD_GETSTATUS -- stHDCP_GETSTATUS -- MS_HDCP_STATUS_INFO_t
UADP_SPT_NAMEnNXT_DEF(XC_MS_HDCP_STATUS_INFO, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDCP_GETSTATUS, 0);
// E_XC_HDCP_IF_CMD_VSYNC_END_EN -- stHDCP_VSYNC_END_EN
UADP_SPT_NAMEnNXT_DEF(XC_HDCP_VSYNC_END_EN, 0);
// E_XC_HDMI_IF_CMD_AUDIO_OUTPUT -- stHDMI_AUDIO_OUTPUT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_AUDIO_OUTPUT, 0);
// E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO -- stHDMI_AUDIO_CP_HDR_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_AUDIO_CP_HDR_INFO, 0);
// E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS -- stHDMI_AUDIO_CHANNEL_STATUS
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_AUDIO_CHANNEL_STATUS, 0);
// E_XC_HDMI_IF_CMD_GETLIBVER -- stHDMI_GETLIBVER
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GETLIBVER, 0);
// E_XC_HDMI_IF_CMD_GETINFO -- stHDMI_GETINFO -- MS_HDMI_INFO
UADP_SPT_NAMEnNXT_DEF(XC_MS_HDMI_INFO, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GETINFO, 1);
// E_XC_HDMI_IF_CMD_GETSTATUS -- stHDMI_GETSTATUS -- MS_HDMI_Status
UADP_SPT_NAMEnNXT_DEF(XC_MS_HDMI_STATUS, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GETSTATUS, 1);
// E_XC_HDMI_IF_CMD_READ_DDCRAM -- stHDMI_READ_DDCRAM -- XC_DDCRAM_PROG_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_EDID_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_DDCRAM_PROG_INFO, 1);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_READ_DDCRAM, 1);
// E_XC_HDMI_IF_CMD_PROG_DDCRAM -- stHDMI_PROG_DDCRAM -- XC_DDCRAM_PROG_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_PROG_DDCRAM, 1);
// E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE -- stHDMI_GET_CONTENT_TYPE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_CONTENT_TYPE, 0);
// E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY -- stHDMI_GET_EXT_COLORIMETRY
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_EXT_COLORIMETRY, 0);
// E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION -- stHDMI_GET_PIXEL_REPETITION
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_PIXEL_REPETITION, 0);
// E_XC_HDMI_IF_CMD_CHECK_4K2K -- stHDMI_CHECK_4K2K
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_CHECK_4K2K, 0);
// E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS -- stHDMI_3D_4K2K_PROCESS
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_3D_4K2K_PROCESS, 0);
// E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN -- stHDMI_AVG_SCALERINGDOWN
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_AVG_SCALERINGDOWN, 0);
// E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT -- stHDMI_CHECK_ADDITIONAL_FORMAT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_CHECK_ADDITIONAL_FORMAT, 0);
// E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE -- stHDMI_GET_3D_STRUCTURE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_3D_STRUCTURE, 0);
// E_XC_HDMI_CMD_GET_3D_EXT_DATA -- stHDMI_GET_3D_EXT_DATA
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_3D_EXT_DATA, 0);
// E_XC_HDMI_IF_CMD_GET_3D_META_FIELD -- stHDMI_GET_3D_META_FIELD -- sHDMI_3D_META_FIELD
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_3D_META_FIELD, 0);
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_3D_META_FIELD, 1);
// E_XC_HDMI_IF_CMD_GET_VIC_CODE -- stHDMI_GET_VIC_CODE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_VIC_CODE, 0);
// E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE -- stHDMI_GET_4KX2K_VIC_CODE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_4KX2K_VIC_CODE, 0);
// E_XC_HDMI_IF_CMD_ARC_PINCONTROL -- stHDMI_ARC_PINCONTROL
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_ARC_PINCONTROL, 0);
// E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET -- stDVI_SOFTWARE_RESET
UADP_SPT_NAMEnNXT_DEF(XC_DVI_SOFTWARE_RESET, 0);
// E_XC_HDMI_IF_CMD_SET_POWERSTATE -- stHDMI_SET_POWERSTATE
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SET_POWERSTATE, 0);
// E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING -- stHDMI_CHECK_HDMI20_SETTING
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_CHECK_HDMI20_SETTING, 0);
// E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT -- stHDMI_SET_5V_DETECT_GPIO_SELECT
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_SET_5V_DETECT_GPIO_SELECT, 0);
// E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS -- stHDMI_GET_DE_STABLE_STATUS
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_DE_STABLE_STATUS, 0);
// E_XC_HDMI_IF_CMD_GET_DATA_INFO -- stHDMI_GET_DATA_INFO
UADP_SPT_NAMEnNXT_DEF(XC_HDMI_GET_DATA_INFO, 0);
// MAPI_CMD_HDMIRX_INITIAL -- stHDMI_INITIAL_SETTING
UADP_SPT_NAMEnNXT_DEF(HDMIRX_INITIAL_SETTING, 0);
// MAPI_CMD_HDMIRX_GET_SOURCE_VERSION -- stHDMI_GET_SOURCE_VERSION
UADP_SPT_NAMEnNXT_DEF(HDMIRX_GET_SOURCE_VERSION, 0);
// MAPI_CMD_HDMIRX_CHECK_HDCP_STATE -- stHDMI_CHECK_HDCP_STATE
UADP_SPT_NAMEnNXT_DEF(HDMIRX_CHECK_HDCP_STATE, 0);
// MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE -- stHDMI_CHECK_HDCP_ENC_STATE
UADP_SPT_NAMEnNXT_DEF(HDMIRX_CHECK_HDCP_ENC_STATE, 0);
// MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY -- stHDCP_INITPRODUCTIONKEY
UADP_SPT_NAMEnNXT_DEF(HDMIRX_CHECK_HDCP14_KEY_VAILD, 1);
// MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD -- stHDMI_GET_HDCP14_KEY_VAILD
UADP_SPT_NAMEnNXT_DEF(HDMIRX_GET_HDCP14_KEY_VAILD, 0);
// MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG -- stHDMI_SET_HPD_INVERT_FLAG
UADP_SPT_NAMEnNXT_DEF(HDMIRX_SET_HPD_INVERT_FLAG, 0);
// MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE -- stHDMI_PACKET_RECEIVED
UADP_SPT_NAMEnNXT_DEF(HDMIRX_GET_PACKET_RECEIVE, 0);
/*************************** HDCP Repeater ***************************/
// MAPI_CMD_HDMIRX_WRITE_X74-- stHDCP_WRITE_X74
UADP_SPT_NAMEnNXT_DEF(HDMIRX_WRITE_X74, 0);
// MAPI_CMD_HDMIRX_READ_X74-- stHDCP_READ_X74
UADP_SPT_NAMEnNXT_DEF(HDMIRX_READ_X74, 0);
// MAPI_CMD_HDMIRX_SET_REPEATER -- stHDCP_SET_REPEATER
UADP_SPT_NAMEnNXT_DEF(HDMIRX_SET_REPEATER, 0);
// MAPI_CMD_HDMIRX_SET_BSTATUS -- stHDCP_SET_BSTATUS
UADP_SPT_NAMEnNXT_DEF(HDMIRX_SET_BSTATUS,  0);
// MAPI_CMD_HDMIRX_SET_HDMI_MODE -- stHDCP_SET_HDMI_MODE
UADP_SPT_NAMEnNXT_DEF(HDMIRX_SET_HDMI_MODE, 0);
// MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS -- stHDCP_GET_INTERRUPT_STATUS
UADP_SPT_NAMEnNXT_DEF(HDMIRX_GET_INTERRUPT_STATUS, 0);
// MAPI_CMD_HDMIRX_WRITE_KSV_LIST -- stHDCP_WRITE_KSV_LIST
UADP_SPT_NAMEnNXT_DEF(HDMIRX_KSV_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(HDMIRX_WRITE_KSV_LIST, 1);
// MAPI_CMD_HDMIRX_SET_VPRIME -- stHDCP_SET_VPRIME
UADP_SPT_NAMEnNXT_DEF(HDMIRX_VPRIME_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(HDMIRX_SET_VPRIME, 1);
// MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY -- stHDCP22_FILL_CIPHER_KEY
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_RIV_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_SESSION_KEY_TABLE, 0);
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_FILL_CIPHER_KEY, 2);
//MAPI_CMD_HDMIRX_HDCP22_PORT_INIT -- stHDCP22_PORT_INIT
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_PORT_INITIAL, 0);
// MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE -- stHDCP22_POLLING_READ_DONE
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_POLLING_READ_DONE, 0);
// MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER -- stHDCP22_ENABLE_CIPHER
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_ENABLE_CIPHER, 0);
// MAPI_CMD_HDMIRX_HDCP22_HANDLER  -- stHDCP22_HANDLER
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_HANDLER, 0);
// MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT -- stHDCP22_WAIT_EVENT
UADP_SPT_NAMEnNXT_DEF(HDMIRX_HDCP22_WAIT_EVENT, 0);

/*************************** HDCP Repeater ***************************/

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  XC_HDMI_if_adp_Init()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 XC_HDMI_if_adp_Init(void)
{
    MS_U32 ulDataSize = 0;

    UADP_SPT_NAME0NXT(XC_HDMI_MS_U8, MS_U8);
    // E_XC_HDMI_IF_CMD_FUNC_CAPS
    UADP_SPT_NAME0NXT(XC_HDMI_FUNC_CAPS, stHDMI_FUNC_CAPS);
    // E_XC_HDMI_IF_CMD_INIT
    UADP_SPT_NAME0NXT(XC_HDMI_INITIAL_SETTING, stHDMI_INITIAL_SETTING);
    // E_XC_HDMI_IF_CMD_PKT_RESET
    UADP_SPT_NAME0NXT(XC_HDMI_PKT_RESET, stHDMI_PKT_RESET);
    // E_XC_HDMI_IF_CMD_PULLHPD
    UADP_SPT_NAME0NXT(XC_HDMI_PULLHPD, stHDMI_PULLHPD);
    // E_XC_HDMI_IF_CMD_GC_INFO
    UADP_SPT_NAME0NXT(XC_HDMI_GC_INFO, stHDMI_GC_INFO);
    // E_XC_HDMI_IF_CMD_PACKET_RECEIVED
    UADP_SPT_NAME0NXT(XC_HDMI_PACKET_RECEIVED, stHDMI_PACKET_RECEIVED);
    // E_XC_HDMI_IF_CMD_GET_COLORFORMAT
    UADP_SPT_NAME0NXT(XC_HDMI_GET_COLORFORMAT, stHDMI_GET_COLORFORMAT);
    // E_XC_HDMI_IF_CMD_GET_COLORRANGE
    UADP_SPT_NAME0NXT(XC_HDMI_GET_COLORRANGE, stHDMI_GET_COLORRANGE);
    // E_XC_HDMI_IF_CMD_SET_EQ
    UADP_SPT_NAME0NXT(XC_HDMI_SET_EQ, stHDMI_SET_EQ);
    // E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT
    UADP_SPT_NAME0NXT(XC_HDMI_SET_EQ_TO_PORT, stHDMI_SET_EQ_TO_PORT);
    // E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE
    UADP_SPT_NAME0NXT(XC_HDMI_AUDIO_MUTE_ENABLE, stHDMI_SET_EQ_TO_PORT);
    // E_XC_HDMI_IF_CMD_GET_ASPECTRATIO
    UADP_SPT_NAME0NXT(XC_HDMI_GET_ASPECTRATIO, stHDMI_GET_ASPECTRATIO);
    // E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO
    UADP_SPT_NAME0NXT(XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO, stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO);
    // E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER
    UADP_SPT_NAME0NXT(XC_HDMI_GET_AVIINFOFRAMEVER, stHDMI_GET_AVIINFOFRAMEVER);
    // E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE
    UADP_SPT_NAME0NXT(XC_HDMI_ERR_STATUS_UPDATE, stHDMI_ERR_STATUS_UPDATE);
    // E_XC_HDMI_IF_CMD_GET_TMDS_FREQ
    UADP_SPT_NAME0NXT(XC_HDMI_GET_TMDS_FREQ, stHDMI_GET_TMDS_FREQ);
    // E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS
    UADP_SPT_NAME0NXT(XC_HDMI_GET_POLLINGSTATUS, stHDMI_GET_POLLINGSTATUS);
    // E_XC_HDMI_IF_CMD_ISHDMI_MODE
    UADP_SPT_NAME0NXT(XC_HDMI_ISHDMI_MODE, stHDMI_ISHDMI_MODE);
    // E_XC_HDMI_IF_CMD_GET_MID_INFO
    ulDataSize = sizeof(MS_U8) *26;
    UADP_SPT_BGN(&spt_XC_HDMI_MID_INFO_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_XC_HDMI_MID_INFO_TABLE[1]);
    UADP_SPT_NAME1NXT(XC_HDMI_GET_MID_INFO, stHDMI_GET_MID_INFO, pu8ManufactorData, XC_HDMI_MID_INFO_TABLE);
    // E_XC_HDMI_IF_CMD_GET_PACKET_VALUE
    UADP_SPT_NAME1NXT(XC_HDMI_GET_PACKET_VALUE, stHDMI_GET_PACKET_VALUE, pValue, XC_HDMI_MS_U8);
    // E_XC_DVI_IF_CMD_CHANNELPHASESTATUS
    UADP_SPT_NAME0NXT(XC_DVI_CHANNELPHASESTATUS, stDVI_CHANNELPHASESTATUS);
    // E_XC_DVI_IF_CMD_SOFTWARERESET
    UADP_SPT_NAME0NXT(XC_DVI_SOFTWARERESET, stDVI_SOFTWARERESET);
    // E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT
    UADP_SPT_NAME0NXT(XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT);
    // E_XC_DVI_IF_CMD_CLKPULLLOW
    UADP_SPT_NAME0NXT(XC_DVI_CLKPULLLOW, stDVI_CLKPULLLOW);
    // E_XC_DVI_IF_CMD_SWITCHSRC
    UADP_SPT_NAME0NXT(XC_DVI_SWITCHSRC, stDVI_SWITCHSRC);
    // E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ
    UADP_SPT_NAME0NXT(XC_HDMI_SETFORCLOCKLESSTHAN70MHZ, stHDMI_SETFORCLOCKLESSTHAN70MHZ);
    // E_XC_HDMI_IF_CMD_DVI_ADJUST
    UADP_SPT_NAME0NXT(XC_HDMI_DVI_ADJUST, stHDMI_DVI_ADJUST);
    // E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT
    UADP_SPT_NAME0NXT(XC_HDMI_SETUPDATEPHASELINECOUNT, stHDMI_SETUPDATEPHASELINECOUNT);
    // E_XC_HDCP_IF_CMD_ENABLE
    UADP_SPT_NAME0NXT(XC_HDCP_ENABLE, stHDCP_ENABLE);
    // E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE
    UADP_SPT_NAME0NXT(XC_HDMI_SET_HDCPENABLE, stHDMI_SET_HDCPENABLE);
    // E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP_KEY_LENGTH;
    UADP_SPT_BGN(&spt_XC_HDCP_KEY_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_XC_HDCP_KEY_TABLE[1]);
    UADP_SPT_NAME1NXT(XC_HDCP_INITPRODUCTIONKEY, stHDCP_INITPRODUCTIONKEY, pu8HdcpKeyData, XC_HDCP_KEY_TABLE);
    // E_XC_HDCP_IF_CMD_GETSTATUS
    UADP_SPT_NAME0NXT(XC_MS_HDCP_STATUS_INFO, MS_HDCP_STATUS_INFO_t);
    UADP_SPT_NAME0NXT(XC_HDCP_GETSTATUS, stHDCP_GETSTATUS);
    // E_XC_HDCP_IF_CMD_VSYNC_END_EN
    UADP_SPT_NAME0NXT(XC_HDCP_VSYNC_END_EN, stHDCP_VSYNC_END_EN);
    // E_XC_HDMI_IF_CMD_AUDIO_OUTPUT
    UADP_SPT_NAME0NXT(XC_HDMI_AUDIO_OUTPUT, stHDMI_AUDIO_OUTPUT);
    // E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO
    UADP_SPT_NAME0NXT(XC_HDMI_AUDIO_CP_HDR_INFO, stHDMI_AUDIO_CP_HDR_INFO);
    // E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS
    UADP_SPT_NAME0NXT(XC_HDMI_AUDIO_CHANNEL_STATUS, stHDMI_AUDIO_CHANNEL_STATUS);
    // E_XC_HDMI_IF_CMD_GETLIBVER
    UADP_SPT_NAME0NXT(XC_HDMI_GETLIBVER, stHDMI_GETLIBVER);
    // E_XC_HDMI_IF_CMD_GETINFO
    UADP_SPT_NAME0NXT(XC_MS_HDMI_INFO, MS_HDMI_INFO);
    UADP_SPT_NAME1NXT(XC_HDMI_GETINFO, stHDMI_GETINFO, pInfo, XC_MS_HDMI_INFO);
    // E_XC_HDMI_IF_CMD_GETSTATUS
    UADP_SPT_NAME0NXT(XC_MS_HDMI_STATUS, MS_HDMI_Status);
    UADP_SPT_NAME1NXT(XC_HDMI_GETSTATUS, stHDMI_GETSTATUS, pStatus, XC_MS_HDMI_STATUS);
    // E_XC_HDMI_IF_CMD_READ_DDCRAM
    ulDataSize = sizeof(MS_U8) *256;
    UADP_SPT_BGN(&spt_XC_HDMI_EDID_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_XC_HDMI_EDID_TABLE[1]);
    UADP_SPT_NAME1NXT(XC_HDMI_DDCRAM_PROG_INFO, XC_DDCRAM_PROG_INFO, EDID, XC_HDMI_EDID_TABLE);
    UADP_SPT_NAME1NXT(XC_HDMI_READ_DDCRAM, stHDMI_READ_DDCRAM, pstDDCRam_Info, XC_HDMI_DDCRAM_PROG_INFO);
    // E_XC_HDMI_IF_CMD_PROG_DDCRAM
    UADP_SPT_NAME1NXT(XC_HDMI_PROG_DDCRAM, stHDMI_PROG_DDCRAM, pstDDCRam_Info, XC_HDMI_DDCRAM_PROG_INFO);
    // E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE
    UADP_SPT_NAME0NXT(XC_HDMI_GET_CONTENT_TYPE, stHDMI_GET_CONTENT_TYPE);
    // E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY
    UADP_SPT_NAME0NXT(XC_HDMI_GET_EXT_COLORIMETRY, stHDMI_GET_EXT_COLORIMETRY);
    // E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION
    UADP_SPT_NAME0NXT(XC_HDMI_GET_PIXEL_REPETITION, stHDMI_GET_PIXEL_REPETITION);
    // E_XC_HDMI_IF_CMD_CHECK_4K2K
    UADP_SPT_NAME0NXT(XC_HDMI_CHECK_4K2K, stHDMI_CHECK_4K2K);
    // E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS
    UADP_SPT_NAME0NXT(XC_HDMI_3D_4K2K_PROCESS, stHDMI_3D_4K2K_PROCESS);
    // E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN
    UADP_SPT_NAME0NXT(XC_HDMI_AVG_SCALERINGDOWN, stHDMI_AVG_SCALERINGDOWN);
    // E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT
    UADP_SPT_NAME0NXT(XC_HDMI_CHECK_ADDITIONAL_FORMAT, stHDMI_CHECK_ADDITIONAL_FORMAT);
    // E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE
    UADP_SPT_NAME0NXT(XC_HDMI_GET_3D_STRUCTURE, stHDMI_GET_3D_STRUCTURE);
    // E_XC_HDMI_CMD_GET_3D_EXT_DATA
    UADP_SPT_NAME0NXT(XC_HDMI_GET_3D_EXT_DATA, stHDMI_GET_3D_EXT_DATA);
    // E_XC_HDMI_IF_CMD_GET_3D_META_FIELD
    UADP_SPT_NAME0NXT(XC_HDMI_3D_META_FIELD, sHDMI_3D_META_FIELD);
    UADP_SPT_NAME1NXT(XC_HDMI_GET_3D_EXT_DATA, stHDMI_GET_3D_META_FIELD, pdata, XC_HDMI_3D_META_FIELD);
    // E_XC_HDMI_IF_CMD_GET_VIC_CODE
    UADP_SPT_NAME0NXT(XC_HDMI_GET_VIC_CODE, stHDMI_GET_VIC_CODE);
    // E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE
    UADP_SPT_NAME0NXT(XC_HDMI_GET_4KX2K_VIC_CODE, stHDMI_GET_4KX2K_VIC_CODE);
    // E_XC_HDMI_IF_CMD_ARC_PINCONTROL
    UADP_SPT_NAME0NXT(XC_HDMI_ARC_PINCONTROL, stHDMI_ARC_PINCONTROL);
    // E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET
    UADP_SPT_NAME0NXT(XC_DVI_SOFTWARE_RESET, stDVI_SOFTWARE_RESET);
    // E_XC_HDMI_IF_CMD_SET_POWERSTATE
    UADP_SPT_NAME0NXT(XC_HDMI_SET_POWERSTATE, stHDMI_SET_POWERSTATE);
    // E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING
    UADP_SPT_NAME0NXT(XC_HDMI_CHECK_HDMI20_SETTING, stHDMI_CHECK_HDMI20_SETTING);
    // E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT
    UADP_SPT_NAME0NXT(XC_HDMI_SET_5V_DETECT_GPIO_SELECT, stHDMI_SET_5V_DETECT_GPIO_SELECT);
    // E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS
    UADP_SPT_NAME0NXT(XC_HDMI_GET_DE_STABLE_STATUS, stHDMI_GET_DE_STABLE_STATUS);
    // E_XC_HDMI_IF_CMD_GET_DATA_INFO
    UADP_SPT_NAME0NXT(XC_HDMI_GET_DATA_INFO, stHDMI_GET_DATA_INFO);

    return 0;
}

//**************************************************************************
//  [Function Name]:
//                  XC_HDMI_if_adp_Ioctl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 XC_HDMI_if_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    char buffer_arg[2];
    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;
    MS_U32 ulDataSize = 0;

    switch(u32Cmd)
    {
        case E_XC_HDMI_IF_CMD_FUNC_CAPS :
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_FUNC_CAPS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_INIT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_INITIAL_SETTING, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_EXIT:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_XC_HDMI_IF_CMD_PKT_RESET:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_PKT_RESET, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_PULLHPD:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_PULLHPD, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GC_INFO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GC_INFO, spt_XC_HDMI_GC_INFO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_PACKET_RECEIVED:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_PACKET_RECEIVED, spt_XC_HDMI_PACKET_RECEIVED, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_COLORFORMAT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_COLORFORMAT, spt_XC_HDMI_GET_COLORFORMAT, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_COLORRANGE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_COLORRANGE, spt_XC_HDMI_GET_COLORRANGE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SET_EQ:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SET_EQ, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SET_EQ_TO_PORT, spt_XC_HDMI_SET_EQ_TO_PORT, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_AUDIO_MUTE_ENABLE, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_XC_HDMI_IF_CMD_GET_ASPECTRATIO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_ASPECTRATIO, spt_XC_HDMI_GET_ASPECTRATIO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO, spt_XC_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd,pArgs, spt_XC_HDMI_GET_AVIINFOFRAMEVER, spt_XC_HDMI_GET_AVIINFOFRAMEVER, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_ERR_STATUS_UPDATE, spt_XC_HDMI_ERR_STATUS_UPDATE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_TMDS_FREQ:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_GET_TMDS_FREQ, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS: // Need check
            //ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_GET_POLLINGSTATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_ISHDMI_MODE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_ISHDMI_MODE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_MID_INFO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_MID_INFO, spt_XC_HDMI_GET_MID_INFO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_PACKET_VALUE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_PACKET_VALUE, spt_XC_HDMI_GET_PACKET_VALUE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_DVI_IF_CMD_CHANNELPHASESTATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_DVI_CHANNELPHASESTATUS, spt_XC_DVI_CHANNELPHASESTATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_DVI_IF_CMD_SOFTWARERESET:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_DVI_SOFTWARERESET, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_DVI_IF_CMD_RESET:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, spt_XC_HDMI_GET_AVIIINFO_ACTIVEINFOPRESENT, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_DVI_IF_CMD_CLKPULLLOW:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_DVI_CLKPULLLOW, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_DVI_IF_CMD_SWITCHSRC:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_DVI_SWITCHSRC, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ, spt_XC_HDMI_SETFORCLOCKLESSTHAN70MHZ, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_DVI_ADJUST:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_DVI_ADJUST, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SETUPDATEPHASELINECOUNT, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDCP_IF_CMD_ENABLE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDCP_ENABLE, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SET_HDCPENABLE, spt_XC_HDMI_SET_HDCPENABLE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDCP_IF_CMD_CLEARSTATUS:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDCP_INITPRODUCTIONKEY, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDCP_IF_CMD_GETSTATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDCP_GETSTATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDCP_IF_CMD_VSYNC_END_EN:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDCP_VSYNC_END_EN, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AUDIO_OUTPUT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_AUDIO_OUTPUT, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_AUDIO_CP_HDR_INFO, spt_XC_HDMI_AUDIO_CP_HDR_INFO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_AUDIO_CHANNEL_STATUS, spt_XC_HDMI_AUDIO_CHANNEL_STATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GETLIBVER: // Need check
            //ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDCP_VSYNC_END_EN, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GETINFO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_GETINFO, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GETSTATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_GETSTATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_READ_DDCRAM:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_XC_HDMI_READ_DDCRAM, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_PROG_DDCRAM:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_PROG_DDCRAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_CONTENT_TYPE, spt_XC_HDMI_GET_CONTENT_TYPE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_EXT_COLORIMETRY, spt_XC_HDMI_GET_EXT_COLORIMETRY, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_PIXEL_REPETITION, spt_XC_HDMI_GET_PIXEL_REPETITION, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_CHECK_4K2K:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_CHECK_4K2K, spt_XC_HDMI_CHECK_4K2K, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_3D_4K2K_PROCESS, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_AVG_SCALERINGDOWN, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT, spt_XC_HDMI_CHECK_ADDITIONAL_FORMAT, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_3D_STRUCTURE, spt_XC_HDMI_GET_3D_STRUCTURE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_CMD_GET_3D_EXT_DATA:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_3D_EXT_DATA, spt_XC_HDMI_GET_3D_EXT_DATA, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_3D_META_FIELD:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_3D_META_FIELD, spt_XC_HDMI_GET_3D_META_FIELD, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_VIC_CODE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_VIC_CODE, spt_XC_HDMI_GET_VIC_CODE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_4KX2K_VIC_CODE, spt_XC_HDMI_GET_4KX2K_VIC_CODE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_ARC_PINCONTROL:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_ARC_PINCONTROL, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_DVI_SOFTWARE_RESET, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SET_POWERSTATE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SET_POWERSTATE, spt_XC_HDMI_SET_POWERSTATE, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_DVI_HF_ADJUST:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_CHECK_HDMI20_SETTING, spt_XC_HDMI_CHECK_HDMI20_SETTING, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_SET_5V_DETECT_GPIO_SELECT, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_DE_STABLE_STATUS, spt_XC_HDMI_GET_DE_STABLE_STATUS, buffer_arg, sizeof(buffer_arg));
            break;

        case E_XC_HDMI_IF_CMD_CTRL:
            {
                stHDMI_CONTROL stHDMIRxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDMI_CONTROL_COMPAT stCompatHDMIRxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMIRxArgs, pArgs, sizeof(stHDMI_CONTROL_COMPAT));

                    ulDataSize = sizeof(MS_U8) *stCompatHDMIRxArgs.u32BufSize;

                    stHDMIRxArgs.pBuf = malloc(ulDataSize);
                    stHDMIRxArgs.ucHDMIInfoSource = stCompatHDMIRxArgs.ucHDMIInfoSource;
                    stHDMIRxArgs.u32Cmd = stCompatHDMIRxArgs.u32Cmd;
                    stHDMIRxArgs.u32BufSize = stCompatHDMIRxArgs.u32BufSize;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMIRxArgs);

                    stCompatHDMIRxArgs.bRet = stHDMIRxArgs.bRet;
                    CMP_CPY_TO_USER(stCompatHDMIRxArgs.pBuf, stHDMIRxArgs.pBuf, ulDataSize);
                    CMP_CPY_TO_USER(pArgs, &stCompatHDMIRxArgs, sizeof(stHDMI_CONTROL_COMPAT));

                    free(stHDMIRxArgs.pBuf);
                }
                else
                {
                    CPY_FROM_USER(&stHDMIRxArgs, pArgs, sizeof(stHDMI_CONTROL));

                    ulDataSize = sizeof(MS_U8) *stHDMIRxArgs.u32BufSize;

                    stHDMIRxArgs.pBuf = malloc(ulDataSize);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMIRxArgs);

                    CPY_TO_USER(pArgs, &stHDMIRxArgs, sizeof(stHDMI_CONTROL));

                    free(stHDMIRxArgs.pBuf);
                }
            }

            break;

        case E_XC_HDMI_IF_CMD_GET_DATA_INFO:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_XC_HDMI_GET_DATA_INFO, spt_XC_HDMI_GET_DATA_INFO, buffer_arg, sizeof(buffer_arg));
            break;

        default:
            MADP_HDMIRX_MSG_WARNING("Command %ld is not existed\n",u32Cmd);
            ulReturnValue = UTOPIA_STATUS_ERR_INVALID_HANDLE;

            break;
    };

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRX_adp_Ioctl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    char usbuffer_arg[2];
    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;
    MS_U32 ulDataSize = 0;

    switch(u32Cmd)
    {
        case MAPI_CMD_HDMIRX_INITIAL:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_INITIAL_SETTING, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_STABLE_POLLING:
            ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case MAPI_CMD_HDMIRX_GET_SOURCE_VERSION:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_GET_SOURCE_VERSION, spt_HDMIRX_GET_SOURCE_VERSION, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP_STATE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_CHECK_HDCP_STATE, spt_HDMIRX_CHECK_HDCP_STATE, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_CHECK_HDCP_ENC_STATE, spt_HDMIRX_CHECK_HDCP_ENC_STATE, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_CHECK_HDCP14_KEY_VAILD, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_HDMIRX_GET_HDCP14_KEY_VAILD, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_SET_HPD_INVERT_FLAG, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_GET_PACKET_RECEIVE, spt_HDMIRX_GET_PACKET_RECEIVE, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        /*************************** HDCP Repeater ***************************/
        case MAPI_CMD_HDMIRX_WRITE_X74:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_WRITE_X74, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_READ_X74:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_READ_X74, spt_HDMIRX_READ_X74, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_SET_REPEATER:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_SET_REPEATER, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_SET_BSTATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_SET_BSTATUS, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_SET_HDMI_MODE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_SET_HDMI_MODE, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_GET_INTERRUPT_STATUS, spt_HDMIRX_GET_INTERRUPT_STATUS, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_WRITE_KSV_LIST:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_WRITE_KSV_LIST, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_SET_VPRIME:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_SET_VPRIME, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;
        /*************************** HDCP Repeater ***************************/

        case MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_HDCP22_FILL_CIPHER_KEY, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_HDCP22_PORT_INIT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_HDCP22_PORT_INITIAL, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_HDCP22_POLLING_READ_DONE, spt_HDMIRX_HDCP22_POLLING_READ_DONE, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_HDCP22_ENABLE_CIPHER, NULL, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_HDCP22_SEND_MESSAGE:
            {
                stHDCP22_SEND_MESSAGE stHDMIRxArgs = {0};

                if(IS_CMP_TASK())
                {
                    stHDCP22_SEND_MESSAGE_COMPAT stCompatHDMIRxArgs = {0};

                    CMP_CPY_FROM_USER(&stCompatHDMIRxArgs, pArgs, sizeof(stHDCP22_SEND_MESSAGE_COMPAT));

                    ulDataSize = sizeof(MS_U8) *stCompatHDMIRxArgs.dwDataLen;

                    stHDMIRxArgs.pucData = malloc(ulDataSize);
                    CMP_CPY_FROM_USER(stHDMIRxArgs.pucData, stCompatHDMIRxArgs.pucData, ulDataSize);
                    stHDMIRxArgs.ucPortType = stCompatHDMIRxArgs.ucPortType;
                    stHDMIRxArgs.ucPortIdx = stCompatHDMIRxArgs.ucPortIdx;
                    stHDMIRxArgs.dwDataLen = stCompatHDMIRxArgs.dwDataLen;

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMIRxArgs);

                    free(stHDMIRxArgs.pucData);
                }
                else
                {
                    void *pData = NULL;

                    CPY_FROM_USER(&stHDMIRxArgs, pArgs, sizeof(stHDCP22_SEND_MESSAGE));

                    ulDataSize = sizeof(MS_U8) *stHDMIRxArgs.dwDataLen;

                    pData = stHDMIRxArgs.pucData;
                    stHDMIRxArgs.pucData = malloc(ulDataSize);
                    CPY_FROM_USER(stHDMIRxArgs.pucData, pData, ulDataSize);

                    ulReturnValue = UtopiaIoctl(pInstanceTmp, u32Cmd, &stHDMIRxArgs);

                    free(stHDMIRxArgs.pucData);
                }
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_HANDLER:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_HDMIRX_HDCP22_HANDLER, spt_HDMIRX_HDCP22_HANDLER, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        case MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT:
            ulReturnValue = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_HDMIRX_HDCP22_WAIT_EVENT, usbuffer_arg, sizeof(usbuffer_arg));
            break;

        default:

            break;
    };

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRX_adp_Init()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRX_adp_Init(FUtopiaIOctl* pIoctl)
{
    MS_U32 ulDataSize = 0;

    // MAPI_CMD_HDMIRX_INITIAL
    UADP_SPT_NAME0NXT(HDMIRX_INITIAL_SETTING, stHDMI_INITIAL_SETTING);
    // MAPI_CMD_HDMIRX_GET_SOURCE_VERSION
    UADP_SPT_NAME0NXT(HDMIRX_GET_SOURCE_VERSION, stHDMI_GET_SOURCE_VERSION);
    // MAPI_CMD_HDMIRX_CHECK_HDCP_STATE
    UADP_SPT_NAME0NXT(HDMIRX_CHECK_HDCP_STATE, stHDMI_CHECK_HDCP_STATE);
    // MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE
    UADP_SPT_NAME0NXT(HDMIRX_CHECK_HDCP_ENC_STATE, stHDMI_CHECK_HDCP_ENC_STATE);
    // MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP_KEY_LENGTH;
    UADP_SPT_BGN(&spt_XC_HDCP_KEY_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_XC_HDCP_KEY_TABLE[1]);
    UADP_SPT_NAME1NXT(HDMIRX_CHECK_HDCP14_KEY_VAILD, stHDCP_INITPRODUCTIONKEY, pu8HdcpKeyData, XC_HDCP_KEY_TABLE);
    // MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD
    UADP_SPT_NAME0NXT(HDMIRX_GET_HDCP14_KEY_VAILD, stHDMI_GET_HDCP14_KEY_VAILD);
    // MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG
    UADP_SPT_NAME0NXT(HDMIRX_SET_HPD_INVERT_FLAG, stHDMI_SET_HPD_INVERT_FLAG);
    // MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE
    UADP_SPT_NAME0NXT(HDMIRX_GET_PACKET_RECEIVE, stHDMI_PACKET_RECEIVED);

    /*************************** HDCP Repeater ***************************/
    // MAPI_CMD_HDMIRX_WRITE_X74
    UADP_SPT_NAME0NXT(HDMIRX_WRITE_X74, stHDCP_WRITE_X74);
    // MAPI_CMD_HDMIRX_READ_X74
    UADP_SPT_NAME0NXT(HDMIRX_READ_X74, stHDCP_READ_X74);
    // MAPI_CMD_HDMIRX_SET_REPEATER
    UADP_SPT_NAME0NXT(HDMIRX_SET_REPEATER, stHDCP_SET_REPEATER);
    // MAPI_CMD_HDMIRX_SET_BSTATUS
    UADP_SPT_NAME0NXT(HDMIRX_SET_BSTATUS, stHDCP_SET_BSTATUS);
    // MAPI_CMD_HDMIRX_SET_HDMI_MODE
    UADP_SPT_NAME0NXT(HDMIRX_SET_HDMI_MODE, stHDCP_SET_HDMI_MODE);
    // MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS
    UADP_SPT_NAME0NXT(HDMIRX_GET_INTERRUPT_STATUS, stHDCP_GET_INTERRUPT_STATUS);
    // MAPI_CMD_HDMIRX_WRITE_KSV_LIST
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP_KSV_LIST_LENGTH;
    UADP_SPT_BGN(&spt_HDMIRX_KSV_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_HDMIRX_KSV_TABLE[1]);
    UADP_SPT_NAME1NXT(HDMIRX_WRITE_KSV_LIST, stHDCP_WRITE_KSV_LIST, pucKSV, HDMIRX_KSV_TABLE);
    // MAPI_CMD_HDMIRX_SET_VPRIME
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP_VPRIME_LENGTH;
    UADP_SPT_BGN(&spt_HDMIRX_VPRIME_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_HDMIRX_VPRIME_TABLE[1]);
    UADP_SPT_NAME1NXT(HDMIRX_SET_VPRIME, stHDCP_SET_VPRIME, pucVPrime, HDMIRX_VPRIME_TABLE);
    /*************************** HDCP Repeater ***************************/
    // MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP22_RIV_LENGTH;
    UADP_SPT_BGN(&spt_HDMIRX_HDCP22_RIV_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_HDMIRX_HDCP22_RIV_TABLE[1]);
    ulDataSize = sizeof(MS_U8) *HDMI_HDCP22_SESSION_KEY_LENGTH;
    UADP_SPT_BGN(&spt_HDMIRX_HDCP22_SESSION_KEY_TABLE[0], ulDataSize);
    UADP_SPT_FIN(&spt_HDMIRX_HDCP22_SESSION_KEY_TABLE[1]);
    UADP_SPT_NAME2NXT(HDMIRX_HDCP22_FILL_CIPHER_KEY, stHDCP22_FILL_CIPHER_KEY, pucRiv, HDMIRX_HDCP22_RIV_TABLE, pucSessionKey, HDMIRX_HDCP22_SESSION_KEY_TABLE);
    // MAPI_CMD_HDMIRX_HDCP22_PORT_INIT
    UADP_SPT_NAME0NXT(HDMIRX_HDCP22_PORT_INITIAL, stHDCP22_PORT_INIT);
    // MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE
    UADP_SPT_NAME0NXT(HDMIRX_HDCP22_POLLING_READ_DONE, stHDCP22_POLLING_READ_DONE);
    // MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER
    UADP_SPT_NAME0NXT(HDMIRX_HDCP22_ENABLE_CIPHER, stHDCP22_ENABLE_CIPHER);
    // MAPI_CMD_HDMIRX_HDCP22_HANDLER
    UADP_SPT_NAME0NXT(HDMIRX_HDCP22_HANDLER, stHDCP22_HANDLER);
    // MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT
    UADP_SPT_NAME0NXT(HDMIRX_HDCP22_WAIT_EVENT, stHDCP22_WAIT_EVENT);

    *pIoctl = (FUtopiaIOctl)HDMIRX_adp_Ioctl;

    return 0;
}

#endif

