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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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
/// @file   UFO.h
/// @brief  MStar Common Interface Header File
/// @author MStar Semiconductor Inc.
/// @note   utopia feature definition file
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _UFO_H_
#define _UFO_H_


/////////////////////////////////////////////////////////
/// UTOPIA CODELINE, DO NOT MODIFY
/////////////////////////////////////////////////////////
#define UFO_PUBLIC_HEADER

/////////////////////////////////////////////////////////
/// DMX lib feature define
/////////////////////////////////////////////////////////
#define UFO_DMX_FQ
//#define UFO_DMX_TSO
#define UFO_DMX_TSO2
#define UFO_DMX_TSO2_PVR
#define UFO_DMX_RES_ALLOC_EN

/////////////////////////////////////////////////////////
/// CEC lib feature define
/////////////////////////////////////////////////////////
//#define UFO_CEC_PORT_SELECT

/////////////////////////////////////////////////////////
/// XC lib feature define
/////////////////////////////////////////////////////////
//#define UFO_XC_FORCEWIRTE
//#define UFO_XC_FORCEWRITE
//#define UFO_XC_HDMI_5V_DETECT
#define UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
#define UFO_XC_PQ_PATH

/*
** XC Pixelshift new mechanism define
*/
#define UFO_XC_SETPIXELSHIFT

/*
** XC HDR function
*/
#define UFO_XC_HDR

#ifdef UFO_XC_HDR
/*
** XC HDR version
*/
#define UFO_XC_HDR_VERSION 2   // for HW supported Dolby/Open HDR
#endif

/*
** XC test pattern function
*/
#define UFO_XC_TEST_PATTERN

/*
** XC 3D DS Feature
*/
//#define UFO_XC_SUPPORT_3D_DS

/*
** XC auto download function
*/
#define UFO_XC_AUTO_DOWNLOAD

/*
** xc zorder
*/
#define UFO_XC_ZORDER

/*
** XC SUPPORT FRC
*/
#define UFO_XC_FRC_INSIDE

/*
** XC SUPPORT DS PQ relat to SUPPORT_PQ_DS
*/
#define UFO_XC_DS_PQ

#define UFO_XC_SET_DSINFO_V0

#define UFO_XC_HDMI_4K2K_DMA

/*
** XC SUPPORT FRAME LOCK FOR RAPTOR CASE
*/
//#define UFO_XC_RAPTOR_FRAME_LOCK

#define UFO_XC_SETWINDOW_LITE







/////////////////////////////////////////////////////////
/// GOP lib feature define
/////////////////////////////////////////////////////////
#define UFO_GOP_DIP_PINPON
#define UFO_GOP_SUPPORT_DUAL_RATE_OP

/////////////////////////////////////////////////////////
/// DIP lib feature define
/////////////////////////////////////////////////////////
#define UFO_DIP_HVSP
#define UFO_DIP_OSD_BLENDING
#define UFO_DIP_SELECT_TILE_BLOCK
//#define UFO_DIP_CMDQ

/////////////////////////////////////////////////////////
/// Presetcontrol for TEE
/////////////////////////////////////////////////////////
#define UFO_VDEC_TEE_VPU

/////////////////////////////////////////////////////////
/// VDEC DISABLE PB FRAME MODE
/////////////////////////////////////////////////////////
#define UFO_VDEC_DISABLE_PBFRAME_MODE

/////////////////////////////////////////////////////////
/// VDEC PREPARSE U64 PTS
/////////////////////////////////////////////////////////
#define UFO_VDEC_PREPARU64PTS

/////////////////////////////////////////////////////////
/// GFX function
/////////////////////////////////////////////////////////
#define UFO_GFX_TRIANGLE
#define UFO_GFX_SPAN

//#define SUPPORT_BWD


//#define UFO_MVOP_GET_HV_RATIO

/////////////////////////////////////////////////////////
/// MVOP GET SUPPORTED MAX FPS
/////////////////////////////////////////////////////////
#define UFO_MVOP_GET_MAX_FPS

/////////////////////////////////////////////////////////
/// MVOP RESET SETTING
/////////////////////////////////////////////////////////
#define UFO_MVOP_RESET_SETTING

//MI support HDCP2.2
#define UFO_HDCP2_2

/////////////////////////////////////////////////////////
/// MVOP Dolby HDR
/////////////////////////////////////////////////////////
#define UFO_MVOP_DOLBY_HDR

#define IMPORT_PORTMAPPING

/////////////////////////////////////////////////////////
/// MDebug feature define
/////////////////////////////////////////////////////////
#define UFO_MDEBUG_SUPPORT

/////////////////////////////////////////////////////////
/// Audio feature define
/////////////////////////////////////////////////////////
#define UFO_AUDIO_AD_SUPPORT
#define UFO_AUDIO_MULTI_CHANNEL
#define UFO_AUDIO_AVMONITOR_SYNC

/////////////////////////////////////////////////////////
/// SUPPORT LOCAL DIMMING
/////////////////////////////////////////////////////////
#define UFO_LOCAL_DIMMING

/////////////////////////////////////////////////////////
/// XC 3D format, FRC in scaler
/////////////////////////////////////////////////////////
#define UFO_XC_GET_3D_FORMAT


/////////////////////////////////////////////////////////
/// XC SUPPORT Dual miu
/////////////////////////////////////////////////////////
#define UFO_XC_SUPPORT_DUAL_MIU

/////////////////////////////////////////////////////////
/// set xc cma information to utopia
/////////////////////////////////////////////////////////
#define UFO_SET_XC_CMA_INFORMATION

/////////////////////////////////////////////////////////
/// MVOP Support Stream Info Setting
/////////////////////////////////////////////////////////
#define UFO_MVOP_STREAM_INFO

#endif // _UFO_H_
