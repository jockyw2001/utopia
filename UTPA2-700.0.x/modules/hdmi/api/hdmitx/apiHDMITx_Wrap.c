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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiHDMITx.c
/// @author MStar Semiconductor,Inc.
/// @brief  HDMI Tx Api
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef HDMITX_SUPPORT

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#else
#include <linux/time.h>
#endif /* #include MSOS_TYPE_LINUX_KERNEL */

#include "MsCommon.h"
#include "drvHDMITx.h"
#include "apiHDMITx.h"
#ifdef CUSTOMER_NDS
#include "apiHDMITx_NDS.h"
#endif // CUSTOMER_NDS
//#include "drvMAD.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern MDrvHDMITX_PARAMETER_LIST     gHDMITxInfo;
extern MS_BOOL     g_bDisableRegWrite;
extern MS_BOOL     g_bDisableTMDSCtrl;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------






//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//*********************//
//        DVI / HDMI   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine initial HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_Init(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine exits HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_Exit(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module
/// @param[in] state ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_TurnOnOff(MS_BOOL state)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module output mode (DVI/HDMI)
/// @param[in] mode ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetHDMITxMode(HDMITX_OUTPUT_MODE mode)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module output mode (DVI/HDMI) and deep color depth
/// @param[in] mode ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetHDMITxMode_CD(HDMITX_OUTPUT_MODE mode, HDMITX_VIDEO_COLORDEPTH_VAL val)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module TMDS signal On/Off
/// @param[in] state ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetTMDSOnOff(MS_BOOL state)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine disable HDMI Tx TMDS control
/// @param[in] bFlag Disable/Enable
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_DisableTMDSCtrl(MS_BOOL bFlag)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine swap R and B channel color
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetRBChannelSwap(MS_BOOL state)
{
    return;
}

MS_BOOL DLL_PUBLIC MApi_HDMITx_EDID_HDMISupport(MS_BOOL *HDMI_Support)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's data block length from EDID
/// @param[in] pu8Length pointer of buffer
/// @param[in] u8TagCode
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetDataBlockLengthFromEDID(MS_U8 *pu8Length, MS_U8 u8TagCode)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine doing HDMITx exhibition
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_Exhibit(void)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine getting the connected HDMI Rx status
/// @return Ture: Rx is alive
///             FALSE: Rx is dead
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRxStatus(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's deep color definition from EDID
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRxDCInfoFromEDID(HDMITX_VIDEO_COLORDEPTH_VAL *val)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's support video format from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] u8BufSize buffer size
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRxVideoFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's support audio format from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] u8BufSize buffer size
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRxAudioFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's ID Manu facturer Name from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRxIDManufacturerName(MS_U8 *pu8Buffer)
{
    return FALSE;
}


//------------------------------------------------------------------------------
/// @brief This routine getting Rx's EDID Data
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] BlockIdx 0: for 0~127, 1: for 128~255
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetEDIDData(MS_U8 *pu8Buffer, MS_BOOL BlockIdx)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine gets Rx's supported 3D structures of specific timing from EDID
/// @param[in] timing input video timing
/// @param[in] p3DStructure pointer of supported 3D structure
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetRx3DStructureFromEDID(HDMITX_VIDEO_TIMING timing, HDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This function clear settings of user defined packet
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_PKT_User_Define_Clear(void)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This function set user defined hdmi packet
/// @param[in] packet_type: type of user defined packet
/// @param[in] def_flag: enable/disable user defined packet
/// @param[in] def_process: packet process of user defined packet
/// @param[in] def_fcnt: frame counts of user defined packet (0~31)
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_PKT_User_Define(HDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag,
	HDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This function let user define packet's content
/// @param[in] packet_type: type of user defined packet
/// @param[in] data: content of user defined packet
/// @param[in] length: length of packet content
/// @return None
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_PKT_Content_Define(HDMITX_PACKET_TYPE packet_type, MS_U8 *data, MS_U8 length)
{
    return FALSE;
}


//*********************//
//             Video   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module video output
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetVideoOnOff(MS_BOOL state)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module video color format
/// @param[in] in_color input color format
/// @param[in] out_color output color format
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_FORMAT in_color, HDMITX_VIDEO_COLOR_FORMAT out_color)
{
    return;
}


//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx vs packet content
/// @param[in] in_color input color format
/// @param[in] out_color output color format
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_FORMAT vs_format, HDMITX_VIDEO_3D_STRUCTURE vs_3d, HDMITX_VIDEO_4k2k_VIC vs_vic)
{
    return;
}


//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module video output timing
/// @param[in] mode output video timing
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetVideoOutputTiming(HDMITX_VIDEO_TIMING mode)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx video output aspect ratio
/// @param[in] out_ar output video aspect ratio
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetVideoOutputAsepctRatio(HDMITX_VIDEO_ASPECT_RATIO out_ar)
{
    return;
}


// HDMI Tx video output Overscan and AFD ratio
void DLL_PUBLIC MApi_HDMITx_SetVideoOutputOverscan_AFD(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd)
{
    return;
}

//*********************//
//             Audio   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module audio output
/// @param[in] state output video timing
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetAudioOnOff(MS_BOOL state)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module audio output sampling frequency and channel count
/// @param[in] freq audio frequency
/// @return None
//   For Uranus
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_FREQUENCY freq)
{
    return;
}

void DLL_PUBLIC MApi_HDMITx_SetVideoOutputOverscan_AFD_II(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd, MS_U8 A0 )
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module audio output: sampling frequency, channel count and coding type
/// @param[in]  freq audio frequency
/// @param[in]  ch audio channel type
/// @param[in]  type audio coding type
/// @return None
//   For Oberon
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_FREQUENCY freq, HDMITX_AUDIO_CHANNEL_COUNT ch, HDMITX_AUDIO_CODING_TYPE type)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine Get Audio CTS value
/// @return CTS
//------------------------------------------------------------------------------
MS_U32 DLL_PUBLIC MApi_HDMITx_GetAudioCTS(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine Mute Audio FIFO
/// @param[in] bflag: True: mute audio, False: unmute audio
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_MuteAudioFIFO(MS_BOOL bflag)
{
    return;
}

//*********************//
//             HDCP    //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine get HDCP key
/// @param[in] useinternalkey: TRUE -> from internal, FALSE -> from external, like SPI flash
/// @param[in] data: data point
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_GetHdcpKey(MS_BOOL useinternalkey, MS_U8 *data)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module HDCP encryption
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetHDCPOnOff(MS_BOOL state)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine set HDMI Tx AVMUTE
/// @argument:
///              - bflag: True=> SET_AVMUTE, FALSE=>CLEAR_AVMUTE
///
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetAVMUTE(MS_BOOL bflag)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine get HDMI Tx AVMUTE status
/// @argument:
///
/// @return:
///              - True=> SET_AVMUTE, FALSE=>CLEAR_AVMUTE
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetAVMUTEStatus(void)
{
    return FALSE;
}


//------------------------------------------------------------------------------
/// @brief This routine check whether SRM DSA signauter is valid or not
/// @argument:
///              - *data: data point
///              -   size: size of SRM list(bytes)
/// @return:
///              Ture: valid, FALSE: invalid
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_IsSRMSignatureValid(MS_U8 *data, MS_U32 size)
{
    return FALSE;
}


//------------------------------------------------------------------------------
/// @brief This routine get HDCP authentication status
/// @argument:
/// @return:
///                  E_HDCP_DISABLE      = 0, // HDCP disable
///                  E_HDCP_FAIL = 1, // HDCP fail
///                  E_HDCP_PASS = 2, // HDCP pass
//------------------------------------------------------------------------------
HDMITX_HDCP_STATUS DLL_PUBLIC MApi_HDMITx_GetHDCPStatus(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine get HDCP authentication status
/// @argument:
/// @return:
///                  HDMITX_INT_HDCP_DISABLE  = 0, // HDCP disable
///                  HDMITX_INT_HDCP_FAIL     = 1, // HDCP fail
///                  HDMITX_INT_HDCP_PASS     = 2, // HDCP pass
///                  HDMITX_INT_HDCP_PROCESS  = 3, // HDCP processing
//------------------------------------------------------------------------------
HDMITX_INT_HDCP_STATUS DLL_PUBLIC MApi_HDMITx_GetINTHDCPStatus(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine set HDMI video output or blank or encryption while connected with unsupport HDCP Rx
/// @argument:
///                 E_UNHDCPRX_NORMAL_OUTPUT      = 0, // still display normally
///                 E_UNHDCPRX_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
///                 E_UNHDCPRX_BLUE_SCREEN = 2, // blue screen
/// @return:
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_UnHDCPRxControl(HDMITX_UNHDCPRX_CONTROL state)
{
    return;
}


//------------------------------------------------------------------------------
/// @brief This routine set HDMI video output or blank or encryption while HDCP authentication fail
/// @argument:
///                 E_HDCPRXFail_NORMAL_OUTPUT      = 0, // still display normally
///                 E_HDCPRXFail_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
///                 E_HDCPRXFail_BLUE_SCREEN = 2, // blue screen
/// @return:
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_HDCPRxFailControl(HDMITX_HDCPRXFail_CONTROL state)
{
    return;
}


// Debug
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetLibVer(const MSIF_Version **ppVersion)
{
    return FALSE;
}
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetInfo(HDMI_TX_INFO *pInfo)
{
    return FALSE;
}
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetStatus(HDMI_TX_Status *pStatus)
{
    return FALSE;
}
/**
* @brief set debug mask
* @param[in] u16DbgSwitch DEBUG MASK,
*   0x01: Debug HDMITX, 0x02: Debug HDCP
*/
MS_BOOL DLL_PUBLIC MApi_HDMITx_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    return FALSE;
}

void DLL_PUBLIC MApi_HDMITx_SetHPDGpioPin(MS_U8 u8pin)
{
    return;
}

// Adjust HDMITx analog setting for HDMI test or compliant issue
void DLL_PUBLIC MApi_HDMITx_AnalogTuning(HDMITX_ANALOG_TUNING *pInfo)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx output force mode
/// @argument:
///              - bflag: TRUE: force mode, FALSE: auto mode
///              - output_mode: HDMITX_DVI: DVI, HDMITX_HDMI: HDMI
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_ForceHDMIOutputMode(MS_BOOL bflag, HDMITX_OUTPUT_MODE output_mode)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx output force color format
/// @argument:
///              - bflag: TRUE: force output color format, FALSE: auto mode
///              - output_mode: HDMITX_VIDEO_COLOR_RGB444: RGB, HDMITX_VIDEO_COLOR_YUV444: YUV
/// @return Ture: Set force output color format successfully
///             FALSE: Fail to set force output color format
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_ForceHDMIOutputColorFormat(MS_BOOL bflag, HDMITX_VIDEO_COLOR_FORMAT output_color)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine set flag to mask register write for special case \n
///			e.g. MBoot to APP with logo display
/// @argument:
///              - bFlag: TRUE: Mask register write, FALSE: not Mask
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_DisableRegWrite(MS_BOOL bFlag)
{
    return;
}


//*********************//
//             CEC                 //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine get EDID physical address
/// @argument:
///              - pdata: two bytes of physical address,  ie. 1.0.0.0 => 0x10 0x00
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_GetEDIDPhyAdr(MS_U8 *pdata)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx CEC
/// @argument:
///              - bflag: TRUE: Enable, FALSE: Disable
/// @return None
//------------------------------------------------------------------------------
void DLL_PUBLIC MApi_HDMITx_SetCECOnOff(MS_BOOL bflag)
{
    return;
}

//------------------------------------------------------------------------------
/// @brief This routine get HDMI Tx CEC On/Off status
/// @argument: None
/// @return: CEC On/Off status
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_GetCECStatus(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// @brief This routine get EDID from reciver
/// @argument: None
/// @return: read status
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_EdidChecking(void)
{
    return FALSE;
}

//*********************//
//      RxBypassMode         //
//*********************//
//------------------------------------------------------------------------------
/// @brief This routine set Rx Bypass Mode
/// @argument: freq: Input Freq; bflag:DDC/CEC/ARC bypass
/// @return: setting status.
//------------------------------------------------------------------------------
MS_BOOL DLL_PUBLIC MApi_HDMITx_RxBypass_Mode(HDMITX_INPUT_FREQ freq, MS_BOOL bflag)
{
    return FALSE;
}


// *************  For customer NDS **************//
#ifdef CUSTOMER_NDS

//17.7.7.5 Ioctl: Get Events
MS_BOOL DLL_PUBLIC MApi_HDMITx_Get_Events(MS_U32 *events, MS_BOOL bEraseEvent)
{
    return FALSE;
}

//17.7.7.8 Ioctl: Get Connection Status
MS_BOOL DLL_PUBLIC MApi_HDMITx_Get_Rx_CONNECTION_STATUS(HDMIConnectionStatus *status)
{
    return FALSE;
}

//17.7.7.9 Ioctl: Get EEDID Block
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL DLL_PUBLIC MApi_HDMITx_Get_EEDID_Block(HDMIGetEEDIDBlock *eedid)
{
    return FALSE;
}

//17.7.7.10 Ioctl: Start
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_HDMI_Protocol(MS_U8 protocol)
{
    return FALSE;
}

//17.7.7.11 Ioctl: Stop
MS_BOOL DLL_PUBLIC MApi_HDMITx_Stop(void)
{
    return FALSE;
}

//17.7.7.14 Ioctl: Set Video Code
// VIC codes: {6, 21, 2, 17, 19, 4, 20, 5, 32, 33, 34, 31, 16} => 480i60,576i50,480p60,576p50,720p50,720p60,1080i50,1080i60,1080p24,1080p25,1080p30,1080p50,1080p60
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_Video_Code(MS_U8 *videoCode)
{
    return FALSE;
}

//17.7.7.15 Ioctl: Set Pixel Format
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_Pixel_Format(MS_U8 *pixelFormat)
{
    return FALSE;
}

//17.7.7.16 Ioctl: Set Aspect Ratio
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_Aspect_Ratio(HDMIAspectRatio *aspectRatio)
{
    return FALSE;
}

//17.7.7.17 Ioctl: Set Colorimetry
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_Colorimetry(HDMIColorimetry *colorimetryData)
{
    return FALSE;
}


//17.7.7.18 Ioctl: Set AVMUTE
//17.7.7.19 Ioctl: Clear AVMUTE
MS_BOOL DLL_PUBLIC MApi_HDMITx_Set_AVMUTE(MS_BOOL bflag)
{
    return FALSE;
}

//17.7.7.20 Ioctl: HDCP Start Authentication
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Start_Authentication(void)
{
    return FALSE;
}

//17.7.7.21 Ioctl: HDCP Stop Authentication
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Stop_Authentication(void)
{
    return FALSE;
}

//17.7.7.22 Ioctl: HDCP Get Bcaps
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Get_BCaps(HDMIHDCPBCaps *arg)
{
    return FALSE;
}

//17.7.7.23 Ioctl: HDCP Get Bstatus
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Get_BStatus(HDMIHDCPBStatus *arg)
{
    return FALSE;
}

//17.7.7.24 Ioctl: HDCP Send SRM List
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Send_SRM(DigitalCPSrm *srm)
{
    return FALSE;
}

void DLL_PUBLIC MApi_HDMITx_HDCP_Reset_SRM(void)
{
    return;
}

//17.7.7.25 Ioctl: HDCP Encryption Control
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Encryption_Enable(MS_U8 control)
{
    return FALSE;
}

//17.7.7.26 Ioctl: HDCP Get Status
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Get_HDCP_Status(MS_U8 *status)
{
    return FALSE;
}

//17.7.7.27 Ioctl: HDCP Configure
MS_BOOL DLL_PUBLIC MApi_HDMITx_HDCP_Configure(HDMIHDCPConfiguration *arg)
{
    return FALSE;
}

#endif //CUSTOMER_NDS

#endif

