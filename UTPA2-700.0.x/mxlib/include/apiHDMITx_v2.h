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

#ifndef _API_HDMITX_V2_H_
#define _API_HDMITX_V2_H_

#include "MsTypes.h"
//#include "halHDMITx.h"
//#include "drvHDMITx.h"
//#include "regHDMITx.h"



#ifdef __cplusplus
extern "C"
{
#endif



typedef enum
{
    E_HDMITX_CMD_INIT,
    E_HDMITX_CMD_EXIT,
    E_HDMITX_CMD_TURNONOFF,
    E_HDMITX_CMD_ENABLEPACKETGEN,
    E_HDMITX_CMD_SETHDMITXMODE,
    E_HDMITX_CMD_SETHDMITXMODE_CD,
    E_HDMITX_CMD_SETTMDSONOFF,
    E_HDMITX_CMD_DISABLETMDSCTRL,
    E_HDMITX_CMD_SETRBCHANNELSWAP,
    E_HDMITX_CMD_EXHIBIT,

    E_HDMITX_CMD_GETRXSTATUS,
    E_HDMITX_CMD_GETRXDCINFOFROMEDID,
    E_HDMITX_CMD_GETRXVIDEODORMATFROMEDID,
    E_HDMITX_CMD_GETVICLISTFROMEDID,
    E_HDMITX_CMD_GETDATABLOCKLENGTHFROMEDID,
    E_HDMITX_CMD_GETRXAUDIOFORMATFROMEDID,
    E_HDMITX_CMD_EDID_HDMISUPPORT,
    E_HDMITX_CMD_GETRXIDMANUFACTURERNAME,
    E_HDMITX_CMD_GETEDIDDATA,
    E_HDMITX_CMD_GETRX3DSTRUCTUREFROMEDID,
    E_HDMITX_CMD_GETCOLORFORMATFROMEDID,
    E_HDMITX_CMD_PKT_USER_DEFINE_CLEAR,

    E_HDMITX_CMD_PKT_USER_DEFINE,
    E_HDMITX_CMD_PKT_CONTENT_DEFINE,
    E_HDMITX_CMD_SETVIDEOONOFF,
    E_HDMITX_CMD_SETCOLORFORMAT,
    E_HDMITX_CMD_SET_VS_INFOFRAME,
    E_HDMITX_CMD_SETVIDEOOUTPUTTIMING,
    E_HDMITX_CMD_SETVIDEOUTPUTTIMINGBYCUSTOMER,
    E_HDMITX_CMD_SETVIDEOOUTPUTASEPCTRATIO, //wrong
    E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD,
    E_HDMITX_CMD_SETAUDIOONOFF,
    E_HDMITX_CMD_SETAUDIOFREQUENCY,

    E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD_II,
	E_HDMITX_CMD_SETAUDIOCONFIGURATION,
    E_HDMITX_CMD_SETAUDIOSOURCEFORMAT,
    E_HDMITX_CMD_SETAVIINFOEXTCOLORIMETRY,
    E_HDMITX_CMD_GETAUDIOCTS,
    E_HDMITX_CMD_MUTEAUDIOFIFO,
    E_HDMITX_CMD_GETHDCPKEY,
    E_HDMITX_CMD_GETBKSV,
    E_HDMITX_CMD_GETAKSV,
    E_HDMITX_CMD_SETHDCPONOFF,
    E_HDMITX_CMD_SETAVMUTE,

    E_HDMITX_CMD_GETAVMUTESTATUS,
    E_HDMITX_CMD_HDCP_REVOCATIONKEY_LIST,
    E_HDMITX_CMD_HDCP_REVOCATIONKEY_CHECK,
    E_HDMITX_CMD_HDCP_ISSRMSIGNATUREVALID,
    E_HDMITX_CMD_GETHDCPSTATUS,
    E_HDMITX_CMD_HDCP_STARTAUTH,
    E_HDMITX_CMD_GETINTHDCPSTATUS,
    E_HDMITX_CMD_GETHDCP_PRESTATUS,
    E_HDMITX_CMD_UNHDCPRXCONTROL,
    E_HDMITX_CMD_HDCPRXFAILCONTROL,

    E_HDMITX_CMD_GETLIBVER,
	E_HDMITX_CMD_GETINFO,
    E_HDMITX_CMD_GETSTATUS,
    E_HDMITX_CMD_SETDBGLEVEL,
    E_HDMITX_CMD_SETHPDGPIOPIN,
    E_HDMITX_CMD_ANALOGTUNING,
    E_HDMITX_CMD_FORCEHDMIOUTPUTMODE,
    E_HDMITX_CMD_FORCEHDMIOUTPUTCOLORFORMAT,
    E_HDMITX_CMD_DISABLEREGWRITE,
    E_HDMITX_CMD_GETEDIDPHYADR,

    E_HDMITX_CMD_SETCECONOFF,
	E_HDMITX_CMD_GETCECSTATUS,
    E_HDMITX_CMD_EDIDCHECKING,
    E_HDMITX_CMD_RXBYPASS_MODE,
    E_HDMITX_CMD_DISABLE_RXBYPASS,
    E_HDMITX_CMD_SETAKSV2R0INTERVAL,
    E_HDMITX_CMD_ISRXVALID,
    E_HDMITX_CMD_GETCHIPCAPS,
    E_HDMITX_CMD_SETPOWERSTATE,
    E_HDMITX_CMD_GETEDIDDATABLOCK,
    E_HDMITX_CMD_GETKSVLIST,
    E_HDMITX_CMD_HDCP2ACCESSX74,
    E_HDMITX_CMD_HDCP2TxInit,
    E_HDMITX_CMD_HDCP2TxEnableEncrypt,
    E_HDMITX_CMD_HDCP2TxFillCipherKey,
    E_HDMITX_CMD_GENERALCTRL,
    E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM,
    E_HDMITX_CMD_SSC_ENABLE,
    E_HDMITX_CMD_SET_COLORIMETRY,
    E_HDMITX_CMD_GET_FULL_RX_STATUS,

    E_HDMITX_CMD_TIMING_CAPABILITY_CHECK,
    E_HDMITX_CMD_GET_PANELSIZE_FROM_EDID,
    E_HDMITX_CMD_GET_TMDS_STATUS,
    #if (defined(VANCLEEF_MCP) || defined(RAPTORS_MCP))
    E_HDMITX_CMD_VANCLEEF_LOADHDCPKEY,
    #endif
} E_HDMITX_IOCTL_CMDS;

typedef struct _stHDMITx_Init
{
    MS_BOOL bReturn;
} stHDMITx_Init,*pstHDMITx_Init;

typedef struct _stHDMITx_Exit
{
    MS_BOOL bReturn;
} stHDMITx_Exit,*pstHDMITx_Exit;


typedef struct _stHDMITx_TurnOnOff
{
    MS_BOOL state;
} stHDMITx_TurnOnOff,*pstHDMITx_TurnOnOff;

typedef struct _stHDMITx_EnablePacketGen
{
    MS_BOOL bflag;
} stHDMITx_EnablePacketGen,*pstHDMITx_EnablePacketGen;


typedef struct _stHDMITx_SetHDMITxMode
{
    HDMITX_OUTPUT_MODE mode;
} stHDMITx_SetHDMITxMode,*pstHDMITx_SetHDMITxMode;


typedef struct _stHDMITx_SetHDMITxMode_CD
{
    HDMITX_OUTPUT_MODE mode;
    HDMITX_VIDEO_COLORDEPTH_VAL val;
} stHDMITx_SetHDMITxMode_CD,*pstHDMITx_SetHDMITxMode_CD;

typedef struct _stHDMITx_SetTMDSOnOff
{
    MS_BOOL state;
} stHDMITx_SetTMDSOnOff,*pstHDMITx_SetTMDSOnOff;


typedef struct _stHDMITx_DisableTMDSCtrl
{
    MS_BOOL bFlag;
} stHDMITx_DisableTMDSCtrl,*pstHDMITx_DisableTMDSCtrl;

typedef struct _stHDMITx_SetRBChannelSwap
{
    MS_BOOL state;
} stHDMITx_SetRBChannelSwap,*pstHDMITx_SetRBChannelSwap;

typedef struct _stHDMITx_GetRxStatus
{
    MS_BOOL bReturn;
} stHDMITx_GetRxStatus,*pstHDMITx_GetRxStatus;

typedef struct _stHDMITx_GetRxDCInfoFromEDID
{
    HDMITX_VIDEO_COLORDEPTH_VAL *val;
    MS_BOOL bReturn;
} stHDMITx_GetRxDCInfoFromEDID,*pstHDMITx_GetRxDCInfoFromEDID;

typedef struct _stHDMITx_GetRxVideoFormatFromEDID
{
    MS_U8 *pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetRxVideoFormatFromEDID,*pstHDMITx_GetRxVideoFormatFromEDID;

typedef struct _stHDMITx_GetVICListFromEDID
{
    MS_U8 *pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetVICListFromEDID,*pstHDMITx_GetVICListFromEDID;

typedef struct _stHDMITx_GetColorFormatFromEDID
{
    HDMITX_VIDEO_TIMING timing;
    HDMITX_EDID_COLOR_FORMAT *pColorFmt;
    MS_BOOL bReturn;
} stHDMITx_GetColorFormatFromEDID,*pstHDMITx_GetColorFormatFromEDID;

typedef struct _stHDMITx_GetDataBlockLengthFromEDID
{
    MS_U8 *pu8Length;
    MS_U8 u8TagCode;
    MS_BOOL bReturn;
} stHDMITx_GetDataBlockLengthFromEDID,*pstHDMITx_GetDataBlockLengthFromEDID;

typedef struct _stHDMITx_GetRxAudioFormatFromEDID
{
    MS_U8 *pu8Buffer;
    MS_U8 u8BufSize;
    MS_BOOL bReturn;
} stHDMITx_GetRxAudioFormatFromEDID,*pstHDMITx_GetRxAudioFormatFromEDID;

typedef struct _stHDMITx_EDID_HDMISupport
{
    MS_BOOL *HDMI_Support;
    MS_BOOL bReturn;
} stHDMITx_EDID_HDMISupport,*pstHDMITx_EDID_HDMISupport;

typedef struct _stHDMITx_GetRxIDManufacturerName
{
    MS_U8 *pu8Buffer;
    MS_BOOL bReturn;
} stHDMITx_GetRxIDManufacturerName,*pstHDMITx_GetRxIDManufacturerName;

typedef struct _stHDMITx_GetEDIDData
{
    MS_U8 *pu8Buffer;
    MS_BOOL BlockIdx;
    MS_BOOL bReturn;
} stHDMITx_GetEDIDData,*pstHDMITx_GetEDIDData;

typedef struct _stHDMITx_GetRx3DStructureFromEDID
{
    HDMITX_VIDEO_TIMING timing;
    HDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure;
    MS_BOOL bReturn;
} stHDMITx_GetRx3DStructureFromEDID,*pstHDMITx_GetRx3DStructureFromEDID;

typedef struct _stHDMITx_PKT_User_Define
{
    HDMITX_PACKET_TYPE packet_type;
    MS_BOOL def_flag;
	HDMITX_PACKET_PROCESS def_process;
    MS_U8 def_fcnt;
} stHDMITx_PKT_User_Define,*pstHDMITx_PKT_User_Define;

typedef struct _stHDMITx_PKT_Content_Define
{
    HDMITX_PACKET_TYPE packet_type;
    MS_U8 *data;
    MS_U8 length;
    MS_BOOL bReturn;
} stHDMITx_PKT_Content_Define,*pstHDMITx_PKT_Content_Define;

typedef struct _stHDMITx_SetVideoOnOff
{
    MS_BOOL state;
} stHDMITx_SetVideoOnOff,*pstHDMITx_SetVideoOnOff;


typedef struct _stHDMITx_SetColorFormat
{
    HDMITX_VIDEO_COLOR_FORMAT in_color;
    HDMITX_VIDEO_COLOR_FORMAT out_color;
} stHDMITx_SetColorFormat,*pstHDMITx_SetColorFormat;

typedef struct _stHDMITx_Set_VS_InfoFrame
{
    HDMITX_VIDEO_VS_FORMAT vs_format;
    HDMITX_VIDEO_3D_STRUCTURE vs_3d;
    HDMITX_VIDEO_4k2k_VIC vs_vic;
} stHDMITx_Set_VS_InfoFrame,*pstHDMITx_Set_VS_InfoFrame;

typedef struct _stHDMITx_SetVideoOutputTiming
{
    HDMITX_VIDEO_TIMING mode;
} stHDMITx_SetVideoOutputTiming,*pstHDMITx_SetVideoOutputTiming;

typedef struct DLL_PACKED
{
    HDMITX_VIDEO_TIMING u8Mode;
    HDMITX_TIMING_INFO_BY_CUSTOMER stTiminginfo;
} stHDMITx_SetVideoOutputTimingByCustomer,*pstHDMITx_stHDMITx_SetVideoOutputTimingByCustomer;

typedef struct _stHDMITx_SetVideoOutputAsepctRatio
{
    HDMITX_VIDEO_ASPECT_RATIO out_ar;
} stHDMITx_SetVideoOutputAsepctRatio,*pstHDMITx_SetVideoOutputAsepctRatio;

typedef struct _stHDMITx_SetVideoOutputOverscan_AFD
{
    MS_BOOL bflag;
    HDMITX_VIDEO_SCAN_INFO out_scaninfo;
    MS_U8 out_afd;
} stHDMITx_SetVideoOutputOverscan_AFD,*pstHDMITx_SetVideoOutputOverscan_AFD;

typedef struct _stHDMITx_SetAudioOnOff
{
    MS_BOOL state;
} stHDMITx_SetAudioOnOff,*pstHDMITx_SetAudioOnOff;

typedef struct _stHDMITx_SetAudioFrequency
{
    HDMITX_AUDIO_FREQUENCY freq;
} stHDMITx_SetAudioFrequency,*pstHDMITx_SetAudioFrequency;

typedef struct _stHDMITx_SetVideoOutputOverscan_AFD_II
{
    MS_BOOL bflag;
    HDMITX_VIDEO_SCAN_INFO out_scaninfo;
    MS_U8 out_afd;
    MS_U8 A0;
} stHDMITx_SetVideoOutputOverscan_AFD_II,*pstHDMITx_SetVideoOutputOverscan_AFD_II;

typedef struct _stHDMITx_SetAudioConfiguration
{
    HDMITX_AUDIO_FREQUENCY freq;
    HDMITX_AUDIO_CHANNEL_COUNT ch;
    HDMITX_AUDIO_CODING_TYPE type;
} stHDMITx_SetAudioConfiguration,*pstHDMITx_SetAudioConfiguration;

typedef struct _stHDMITx_SetAudioSourceFormat
{
    HDMITX_AUDIO_SOURCE_FORMAT fmt;
} stHDMITx_SetAudioSourceFormat,*pstHDMITx_SetAudioSourceFormat;

typedef struct _stHDMITx_SetAVIInfoExtColorimetry
{
    HDMITX_AVI_EXTENDED_COLORIMETRY enExtColorimetry;
    HDMITX_AVI_YCC_QUANT_RANGE enYccQuantRange;
    MS_BOOL bReturn;
} stHDMITx_SetAVIInfoExtColorimetry, *pstHDMITx_SetAVIInfoExtColorimetry;

typedef struct _stHDMITx_GetAudioCTS
{
    MS_U32 u32Return;
} stHDMITx_GetAudioCTS,*pstHDMITx_GetAudioCTS;

typedef struct _stHDMITx_MuteAudioFIFO
{
    MS_BOOL bflag;
} stHDMITx_MuteAudioFIFO,*pstHDMITx_MuteAudioFIFO;

typedef struct _stHDMITx_GetHdcpKey
{
    MS_BOOL useinternalkey;
    MS_U8 *data;
} stHDMITx_GetHdcpKey,*pstHDMITx_GetHdcpKey;

typedef struct _stHDMITx_GetBksv
{
    MS_U8 *pdata;
    MS_BOOL bReturn;
} stHDMITx_GetBksv,*pstHDMITx_GetBksv;

typedef struct _stHDMITx_GetAksv
{
    MS_U8 *pdata;
    MS_BOOL bReturn;
} stHDMITx_GetAksv,*pstHDMITx_GetAksv;

typedef struct _stHDMITx_SetHDCPOnOff
{
    MS_BOOL state;
} stHDMITx_SetHDCPOnOff,*pstHDMITx_SetHDCPOnOff;

typedef struct _stHDMITx_SetAVMUTE
{
    MS_BOOL bflag;
} stHDMITx_SetAVMUTE,*pstHDMITx_SetAVMUTE;

typedef struct _stHDMITx_GetAVMUTEStatus
{
    MS_BOOL bReturn;
} stHDMITx_GetAVMUTEStatus,*pstHDMITx_GetAVMUTEStatus;

typedef struct _stHDMITx_HDCP_RevocationKey_List
{
    MS_U8 *data;
    MS_U16 size;
} stHDMITx_HDCP_RevocationKey_List,*pstHDMITx_HDCP_RevocationKey_List;

typedef struct _stHDMITx_HDCP_RevocationKey_Check
{
    HDMITX_REVOCATION_STATE stReturn;
} stHDMITx_HDCP_RevocationKey_Check,*pstHDMITx_HDCP_RevocationKey_Check;

typedef struct _stHDMITx_HDCP_IsSRMSignatureValid
{
    MS_U8 *data;
    MS_U32 size;
    MS_BOOL bReturn;
} stHDMITx_HDCP_IsSRMSignatureValid,*pstHDMITx_HDCP_IsSRMSignatureValid;

typedef struct _stHDMITx_GetHDCPStatus
{
    HDMITX_HDCP_STATUS stReturn;
} stHDMITx_GetHDCPStatus,*pstHDMITx_GetHDCPStatus;

typedef struct _stHDMITx_HDCP_StartAuth
{
    MS_BOOL bFlag;
} stHDMITx_HDCP_StartAuth,*pstHDMITx_HDCP_StartAuth;

typedef struct _stHDMITx_GetINTHDCPStatus
{
    HDMITX_INT_HDCP_STATUS stReturn;
} stHDMITx_GetINTHDCPStatus,*pstHDMITx_GetINTHDCPStatus;

typedef struct _stHDMITx_GetHDCP_PreStatus
{
    HDMITX_INT_HDCP_STATUS stReturn;
} stHDMITx_GetHDCP_PreStatus,*pstHDMITx_GetHDCP_PreStatus;

typedef struct _stHDMITx_UnHDCPRxControl
{
    HDMITX_UNHDCPRX_CONTROL state;
} stHDMITx_UnHDCPRxControl,*pstHDMITx_UnHDCPRxControl;

typedef struct _stHDMITx_HDCPRxFailControl
{
    HDMITX_HDCPRXFail_CONTROL state;
} stHDMITx_HDCPRxFailControl,*pstHDMITx_HDCPRxFailControl;

typedef struct _stHDMITx_GetLibVer
{
    const MSIF_Version **ppVersion;
    MS_BOOL bReturn;
} stHDMITx_GetLibVer,*pstHDMITx_GetLibVer;

typedef struct _stHDMITx_GetInfo
{
    HDMI_TX_INFO *pInfo;
    MS_BOOL bReturn;
} stHDMITx_GetInfo,*pstHDMITx_GetInfo;

typedef struct _stHDMITx_GetStatus
{
    HDMI_TX_Status *pStatus;
    MS_BOOL bReturn;
} stHDMITx_GetStatus,*pstHDMITx_GetStatus;

typedef struct _stHDMITx_SetDbgLevel
{
    MS_U16 u16DbgSwitch;
    MS_BOOL bReturn;
} stHDMITx_SetDbgLevels,*pstHDMITx_SetDbgLevel;

typedef struct _stHDMITx_SetHPDGpioPin
{
    MS_U8 u8pin;
} stHDMITx_SetHPDGpioPin,*pstHDMITx_SetHPDGpioPin;

typedef struct _stHDMITx_AnalogTuning
{
    HDMITX_ANALOG_TUNING *pInfo;
} stHDMITx_AnalogTuning,*pstHDMITx_AnalogTuning;

typedef struct _stHDMITx_ForceHDMIOutputMode
{
    MS_BOOL bflag;
    HDMITX_OUTPUT_MODE output_mode;
} stHDMITx_ForceHDMIOutputMode,*pstHDMITx_ForceHDMIOutputMode;

typedef struct _stHDMITx_ForceHDMIOutputColorFormat
{
    MS_BOOL bflag;
    HDMITX_VIDEO_COLOR_FORMAT output_color;
    MS_BOOL bReturn;
} stHDMITx_ForceHDMIOutputColorFormat,*pstHDMITx_ForceHDMIOutputColorFormat;

typedef struct _stHDMITx_DisableRegWrite
{
    MS_BOOL bFlag;
} stHDMITx_DisableRegWrite,*pstHDMITx_DisableRegWrite;

typedef struct _stHDMITx_GetEDIDPhyAdr
{
    MS_U8 *pdata;
} stHDMITx_GetEDIDPhyAdr,*pstHDMITx_GetEDIDPhyAdr;

typedef struct _stHDMITx_SetCECOnOff
{
    MS_BOOL bflag;
} stHDMITx_SetCECOnOff,*pstHDMITx_SetCECOnOff;

typedef struct _stHDMITx_GetCECStatus
{
    MS_BOOL bReturn;
} stHDMITx_GetCECStatus,*pstHDMITx_GetCECStatus;

typedef struct _stHDMITx_EdidChecking
{
    MS_BOOL bReturn;
} stHDMITx_EdidChecking,*pstHDMITx_EdidChecking;

typedef struct _stHDMITx_RxBypass_Mode
{
    HDMITX_INPUT_FREQ freq;
    MS_BOOL bflag;
    MS_BOOL bReturn;
} stHDMITx_RxBypass_Mode,*pstHDMITx_RxBypass_Mode;

typedef struct _stHDMITx_Disable_RxBypass
{
    MS_BOOL bReturn;
} stHDMITx_Disable_RxBypass,*pstHDMITx_Disable_RxBypass;

typedef struct _stHDMITx_SetAksv2R0Interval
{
    MS_U32 u32Interval;
    MS_BOOL bReturn;
} stHDMITx_SetAksv2R0Interval,*pstHDMITx_SetAksv2R0Interval;

typedef struct _stHDMITx_IsHDCPRxValid
{
    MS_BOOL bReturn;
} stHDMITx_IsHDCPRxValid,*pstHDMITx_IsHDCPRxValid;

typedef struct _stHDMITx_GetChipCaps
{
    EN_HDMITX_CAPS eCapType;
    MS_U32* pRet;
    MS_U32 ret_size;
    MS_BOOL bReturn;
} stHDMITx_GetChipCaps,*pstHDMITx_GetChipCaps;

typedef struct _stHDMITx_SetPowerState
{
    EN_POWER_MODE u16PowerState;
    MS_U32 u32Return;
} stHDMITx_SetPowerState,*pstHDMITx_SetPowerState;

typedef struct _stHDMITx_GetEdidDataBlocks
{
    HDMITX_CEA_DB_TAG_CODE enTagCode;
    HDMITX_CEA_EXT_TAG_CODE enExtTagCode;
    MS_U8* pu8Data;
    MS_U32 u32DataLen;
    MS_U32* pu32RealLen;
    MS_BOOL bReturn;
} stHDMITx_GetEdidDataBlocks, *pstHDMITx_GetEdidDataBlocks;

typedef struct _stHDMITx_GetKSVList
{
    MS_U8* pu8Bstatus;
    MS_U8* pu8KSVList;
    MS_U16 u16BufLen;
    MS_U16* pu16KSVLength;
    MS_BOOL bReturn;
} stHDMITx_GetKSVList, *pstHDMITx_GetKSVList;

typedef struct _stHDMITx_HDCP2AccessX74
{
    MS_U8 u8PortIdx;
    MS_U8 u8OffsetAddr;
    MS_U8 u8OpCode;
    MS_U8 *pu8RdBuf;
    MS_U16 u16RdLen;
    MS_U8 *pu8WRBuff;
    MS_U16 u16WrLen;
    MS_BOOL bReturn;
} stHDMITx_HDCP2AccessX74, *pstHDMITx_HDCP2AccessX74;

typedef struct _stHDMITx_HDCP2TxInit
{
    MS_U8 u8PortIdx;
    MS_BOOL bEnable;
} stHDMITx_HDCP2TxInit, *pstHDMITx_HDCP2TxInit;

typedef struct _stHDMITx_HDCP2TxEnableEncrypt
{
    MS_U8 u8PortIdx;
    MS_BOOL bEnable;
} stHDMITx_HDCP2TxEnableEncrypt, *pstHDMITx_HDCP2TxEnableEncrypt;

typedef struct _stHDMITx_HDCP2TxFillCipherKey
{
    MS_U8 u8PortIdx;
    MS_U8 *pu8Riv;
    MS_U8 *pu8KsXORLC128;
} stHDMITx_HDCP2TxFillCipherKey, *pstHDMITx_HDCP2TxFillCipherKey;

typedef struct _stHDMITx_GeneralCtrl
{
    MS_U32 u32Cmd;
    void *pu8Buf;
    MS_U32 u32BufSize;
    MS_BOOL bReturn;
} stHDMITx_GeneralCtrl, *pstHDMITx_GeneralCtrl;

typedef struct
{
    HDMITX_VIDEO_COLOR_FORMAT       input_color;
    HDMITX_VIDEO_COLOR_FORMAT       output_color;
    HDMITX_QUANT_RANGE               input_range;
    HDMITX_QUANT_RANGE               output_range;
    MS_BOOL                                         result;
} stHDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS, *pstHDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS;

typedef struct
{
    MS_U8                                             ubSSCEn;
    MS_BOOL                                         result;
} stHDMITX_SSCENABLE, *pstHDMITX_SSCENABLE;

typedef struct DLL_PACKED
{
    MS_U32                      u32StructVersion;//StructVersion Control
    HDMITX_AVI_COLORIMETRY      colorimetry;
    MS_U8                       u8Return;
} stHDMITX_SET_COLORIMETRY, *pstHDMITX_SET_COLORIMETRY;

typedef struct DLL_PACKED
{
    MS_U32  u32StructVersion;//StructVersion Control
    MS_U32  u32RxStatus;
} stHDMITX_GET_FULL_RX_STATUS, *pstHDMITX_GET_FULL_RX_STATUS;

typedef struct DLL_PACKED
{
    MS_U32  u32StructVersion;//StructVersion Control
    HDMITX_OUTPUT_MODE eOutputMode;
    HDMITX_VIDEO_TIMING eTiming;
    HDMITX_VIDEO_COLOR_FORMAT eInColor;
    HDMITX_VIDEO_COLOR_FORMAT eOutColor;
    HDMITX_VIDEO_COLORDEPTH_VAL eColorDepth;
    HDMITX_TIMING_ERROR  ubRet;
} stHDMITX_CHECK_LEGAL_TIMING, *pstHDMITX_CHECK_LEGAL_TIMING;

typedef struct DLL_PACKED
{
    MS_U32  u32StructVersion;//StructVersion Control
    MS_U32  u32TMDSStatus;
    MS_U32  u32Ret;
} stHDMITX_GET_TMDS_STATUS, *pstHDMITX_GET_TMDS_STATUS;

#if defined(VANCLEEF_MCP) || defined(RAPTORS_MCP)
typedef struct _stHDMITx_Vancleef_LoadHDCPKey
{
    MS_U8 *pHdcpKey;
    MS_U32 u32Size;
    MS_BOOL bReturn;
} stHDMITx_Vancleef_LoadHDCPKey,*pstHDMITx_Vancleef_LoadHDCPKey;
#endif

#ifdef __cplusplus
}
#endif


#endif // _API_HDMITX_H_

