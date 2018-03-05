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
///////////////////////////////////////////////////////////////////////////////
///
/// @file   apiXC_DWIN.h
/// @brief  MStar XC_DWIN Driver Interface
/// @author MStar Semiconductor Inc.
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup Video Video modules

	* \defgroup XC_DWIN XC_DWIN interface
	* \ingroup Video

	DIP is a video capture function ,the whole function block separate four part,source select, clip window, HV scaling down , output window.\n

	<b> Fucntion block </b> \n
	\image html apiXC_DWIN_pic1.png

	<b> Source select </b> \n
	\image html apiXC_DWIN_pic3.png

	<b> Clip window , HV scaling down , output window</b> \n
	\image html apiXC_DWIN_pic4.png

	<b> API call flow  </b> \n
	\image html apiXC_DWIN_pic2.png

	* \defgroup DIP_INIT DIP init related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_RESOURCE DIP resource related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_SET_WINDOW DIP set window related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_SET_PROPERTY DIP set properity related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_SET_COLOR_TRANSFORM  DIP set color transform properity functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_SET_WINPROPERTY DIP set window properity related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_INTERRUPT DIP interrupt related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_TRIGGER DIP trigger related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_GET_BUFINFO DIP get info related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_POWER_STATE DIP suspend and resume related functions
	* \ingroup   XC_DWIN

	* \defgroup DIP_ToBeRemove DIP api to be removed
	* \ingroup   XC_DWIN

  */
//-------------------------------------------------




#ifndef APIXC_DWIN_H
#define APIXC_DWIN_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "MsCommon.h"
#include "mhal_cmdq.h"
//#include "mvideo_context.h"
//==============================================================================

#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define XC_INFO(x, args...) ULOGI("DIP API", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define XC_WARN(x, args...) ULOGW("DIP API", x, ##args)
//  Need debug, illegal paramter.
#define XC_DBUG(x, args...) ULOGD("DIP API", x, ##args)
// Error, function will be terminated but system not crash
#define XC_ERR(x, args...) ULOGE("DIP API", x, ##args)
// Critical, system crash. (ex. assert)
#define XC_FATAL(x, args...) ULOGF("DIP API", x, ##args)

//==============================================================================
#define MAX_BUFF_CNT 8UL
#define AUTO_BUFF_CNT 0UL
/// Define window type for DIP
typedef enum
{
    DIP_WINDOW = 0,         ///< DIP window
    DWIN0_WINDOW = 1,
    DWIN1_WINDOW = 2,
    MAX_DIP_WINDOW          /// The max support window
}SCALER_DIP_WIN;

typedef enum
{
    E_XC_DIP_RGB_SWAPTO_BGR = 0,         ///< RGB Swap to BGR
    E_XC_DIP_RGB_SWAP_MAX                ///< The max support format
}EN_XC_DIP_RGB_SWAP_TYPE;

/// DIP source data format
typedef enum
{
    /// YCrYCb.
    DIP_DATA_FMT_YUV422 = 0,
    /// RGB domain
    DIP_DATA_FMT_RGB565,
    /// RGB domain
    DIP_DATA_FMT_ARGB8888,
    /// YUV420 HVD tile
    DIP_DATA_FMT_YUV420,
    /// YC separate 422
    DIP_DATA_FMT_YC422,
    /// YUV420 H265 tile
    DIP_DATA_FMT_YUV420_H265,
    /// YUV420 H265_10bits tile
    DIP_DATA_FMT_YUV420_H265_10BITS,
    /// YUV420 planer
    DIP_DATA_FMT_YUV420_PLANER,
    /// YUV420 semi planer
    DIP_DATA_FMT_YUV420_SEMI_PLANER,
    /// YUV422 2.5D DI Top field
    DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_TOP,
    /// YUV422 2.5D DI Bottom field
    DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_BOTTOM,
    /// YUV420 2.5D DI Top field
    DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_TOP,
    /// YUV420 2.5D DI Bottom field
    DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_BOTTOM,
    DIP_DATA_FMT_MAX
} EN_XC_DIP_DATA_FMT;

/// DIP tile block
typedef enum
{
    DIP_TILE_BLOCK_R_NONE  = 0x0,
    DIP_TILE_BLOCK_W_NONE  = 0x1,
    DIP_TILE_BLOCK_R_16_32 = 0x2,
    DIP_TILE_BLOCK_W_16_32 = 0x3,
    DIP_TILE_BLOCK_R_32_16 = 0x4,
    DIP_TILE_BLOCK_W_32_16 = 0x5,
    DIP_TILE_BLOCK_R_32_32 = 0x6,
    DIP_TILE_BLOCK_W_32_32 = 0x7,
}EN_XC_DIP_TILE_BLOCK;
/// DIP scan mode
typedef enum
{
    /// progessive.
    DIP_SCAN_MODE_PROGRESSIVE,
    /// interlace.
    DIP_SCAN_MODE_extern,
} EN_XC_DIP_SCAN_MODE;

typedef enum
{
    DIP_CAP_NOT_EXIST = 0x0,
    DIP_CAP_EXIST =     0x1,
    DIP_CAP_420TILE =   0x2,
    DIP_CAP_SCALING_DOWN =   0x4,
    DIP_CAP_MIRROR =    0x8,
    DIP_CAP_DIPR    =   0x10,
    DIP_CAP_R2Y     =   0x20,
    DIP_CAP_OP1_CAPTURE    = 0x40,
    //not support OP1 capture when traveling main source with PIP mode
    DIP_CAP_OP1_CAPTURE_V1 = 0x40,
    //support OP1 capture in all case
    DIP_CAP_OP1_CAPTURE_V2 = 0x80,
    DIP_CAP_SCALING_UP = 0x100,
    DIP_CAP_OSD_BLENDING = 0x200,
    DIP_CAP_OC_CAPTURE = 0x400,
    DIP_CAP_SC2_NON_SCALING = 0x800,
}EN_XC_DIP_CAP;

typedef enum
{
    E_XC_DIP_VOP2   =0x00,
    E_XC_DIP_OP2    =0x01,
    E_XC_DIP_VIP    =0x02,
    E_XC_DIP_BRI    =0x03,
    E_XC_DIP_NONE   ,
}EN_XC_DIP_OP_CAPTURE;

typedef enum
{
    E_XC_DIP_ROTATION_270   =0x00,
    E_XC_DIP_ROTATION_90    =0x01,
}EN_XC_DIP_ROTATION;

// Define DEBUG level.
typedef enum
{
    // DIP DEBUG LEVEL LOW. Just printf error message.
    E_XC_DIP_Debug_Level_LOW   = 0,
    // GOP DEBUG LEVEL Medium. printf warning message and error message.
    E_XC_DIP_Debug_Level_MED   = 1,
    // GOP DEBUG LEVEL hIGH. printf all message with function.
    E_XC_DIP_Debug_Level_HIGH   = 2,
} EN_XC_DIP_DEBUG_LEVEL;

typedef enum
{
    E_XC_DIP_CMDQ_BEGIN = 0,
    E_XC_DIP_CMDQ_END = 1,
}EN_XC_DIP_CMDQ_ACTION;

typedef struct DLL_PACKED
{
    MS_U8               u8BufCnt;
    MS_PHY              u32YBuf[MAX_BUFF_CNT];
    MS_PHY              u32CBuf[MAX_BUFF_CNT];
} BUFFER_INFO;

typedef struct DLL_PACKED
{
    SCALER_DIP_WIN      eWindow;
    MS_U32              u32DipChipCaps;
} ST_XC_DIP_CHIPCAPS;

typedef struct DLL_PACKED
{
    MS_U8  u8BufCnt;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16Pitch;
    MS_PHY u32BufStart;
    MS_PHY u32BufEnd;
    SCALER_DIP_SOURCE_TYPE enSource;
}ST_XC_DIP_WINPROPERTY;

typedef struct DLL_PACKED
{
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16Pitch;
    MS_PHY u32YBufAddr;
    MS_PHY u32CBufAddr;
    EN_XC_DIP_DATA_FMT enDataFmt;
    MS_PHY u32YBufAddr10Bits;
    MS_PHY u32CBufAddr10Bits;
}ST_XC_DIPR_PROPERTY;

typedef struct DLL_PACKED
{
    MS_U32 ID[4];
} DIPRESOURCE;

// Define DIP version control
typedef struct DLL_PACKED
{
    MS_U32 u32version;
    MS_U32 u32size;
} ST_DIPR_VER_CRL;

typedef enum
{
    E_DIP_MFEDC_TILE_16_32     =0x00,
    E_DIP_MFEDC_TILE_32_16     =0x01,
    E_DIP_MFEDC_TILE_32_32     =0x02,
}EN_MFDEC_TILE_MODE;

typedef enum
{
    E_XC_DIP_H26X_MODE     =0x00,
    E_XC_DIP_VP9_MODE      =0x01,
}EN_MFDEC_VP9_MODE;

// Define DIP version control for MFDec
typedef struct DLL_PACKED
{
    MS_U32 u32version;
    MS_U32 u32size;
} ST_MFDEC_HTLB_VER_CRL;

typedef struct DLL_PACKED
{
    ST_MFDEC_HTLB_VER_CRL stMFDec_HTLB_VerCtl;
    MS_PHY u32HTLBEntriesAddr;
    MS_U8  u8HTLBEntriesSize;
    MS_U8  u8HTLBTableId;
    void* pHTLBInfo;
} ST_DIP_MFDEC_HTLB_INFO;

// Define DIP version control
typedef struct DLL_PACKED
{
    MS_U32 u32version;
    MS_U32 u32size;
} ST_MFDEC_VER_CRL;

typedef struct DLL_PACKED
{
    ST_MFDEC_VER_CRL stMFDec_VerCtl;
    MS_BOOL bMFDec_Enable;
    MS_U8 u8MFDec_Select;
    MS_BOOL bHMirror;
    MS_BOOL bVMirror;
    EN_MFDEC_TILE_MODE enMFDec_tile_mode;
    MS_BOOL bUncompress_mode;
    MS_BOOL bBypass_codec_mode;
    EN_MFDEC_VP9_MODE en_MFDecVP9_mode;
    MS_PHY phyLuma_FB_Base;
    MS_PHY phyChroma_FB_Base;
    MS_U16 u16FB_Pitch;
    MS_U16 u16StartX;
    MS_U16 u16StartY;
    MS_U16 u16HSize;
    MS_U16 u16VSize;
    MS_PHY phyBitlen_Base;
    MS_U16 u16Bitlen_Pitch;
    ST_DIP_MFDEC_HTLB_INFO stMFDec_HTLB_Info;
    void* pMFDecInfo;
} ST_DIP_MFDEC_INFO;

typedef enum
{
    //one top field in one address
    E_DIP_3DDI_TOP_SINGLE_FIELD        =0x00,
    //one bottom field in one address
    E_DIP_3DDI_BOTTOM_SINGLE_FIELD     =0x01,
    //stagger Both field in one address, keep top field
    E_DIP_3DDI_TOP_BOTH_FIELD_STAGGER        =0x02,
    //stagger Both field in one address, keep bottom field
    E_DIP_3DDI_BOTTOM_BOTH_FIELD_STAGGER     =0x03,
    //stagger Both field in one address, DIP 2.5D DI top field
    E_DIP_MED_DI_TOP_BOTH_FIELD_STAGGER       =0x04,
    //stagger Both field in one address, DIP 2.5D DI bottom field
    E_DIP_MED_DI_BOTTOM_BOTH_FIELD_STAGGER    =0x05,
    //two field in two address, DI top field
    E_DIP_MED_DI_TOP_BOTH_FIELD_SEPARATE       =0x06,
    //two field in two address, DI bottom field
    E_DIP_MED_DI_BOTTOM_BOTH_FIELD_SEPARATE    =0x07,
}EN_DIP_DI_FIELD;

typedef struct DLL_PACKED
{
    MS_PHY phyDIInitAddr;
    MS_U32 u32DIInitSize;
}ST_DIPR_DI_INIT_INFO;

typedef struct DLL_PACKED
{
    MS_PHY phyDI_BufAddr; //DI buf address
    MS_U32 u32DI_BufSize; //DI buf size
    MS_U8 u8DIBufCnt;
}ST_DIP_DI_NWAY_BUF_INFO;

typedef struct DLL_PACKED
{
    MS_BOOL bEnableDI;
    MS_BOOL bInitDI;
    ST_DIPR_DI_INIT_INFO stInitInfo;
    MS_U8 u8NwayIndex;
    EN_DIP_DI_FIELD enDI_Field;
    MS_PHY phyBotYBufAddr;
    MS_PHY phyBotCBufAddr;
    MS_PHY phyBotYBufAddr10Bits;
    MS_PHY phyBotCBufAddr10Bits;
    MS_BOOL bEnableBob;
    MS_BOOL bEnaNwayInfoRefresh;
    ST_DIP_DI_NWAY_BUF_INFO *stDINwayBufInfo;
    MS_U8 u8NwayBufCnt;
} ST_DIP_DI_SETTING;

// Define DIP version control
typedef struct DLL_PACKED
{
    MS_U32 u32version;
    MS_U32 u32size;
} ST_DIPR_3DDI_VER_CRL;

typedef struct DLL_PACKED
{
    ST_DIPR_3DDI_VER_CRL stDipr3DDI_VerCtl;
    void* pDIPR3DDISetting;
} ST_DIP_DIPR_3DDI_SETTING;

typedef struct DLL_PACKED
{
    ST_DIPR_VER_CRL stDIPR_VerCtl;
    ST_XC_DIPR_PROPERTY stDIPRProperty;
    ST_DIP_MFDEC_INFO stDIPR_MFDecInfo;
    ST_DIP_DIPR_3DDI_SETTING stDIPR3DDISetting;
    void* pDIPRSetting;
}ST_XC_DIPR_PROPERTY_EX;

//-----------------
// DIP_RESOURCE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Query DIP resource avaliable or not
/// @ingroup DIP_RESOURCE
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_QueryResource(SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Get DIP resource
/// @ingroup DIP_RESOURCE
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_GetResource(SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Release DIP resource
/// @ingroup DIP_RESOURCE
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
typedef MHAL_CMDQ_CmdqInterface_t cmd_mload_utopia_interface;
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_ReleaseResource(SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_CMDQ_SetAction( EN_XC_DIP_CMDQ_ACTION eAction, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_CMDQ_SetOperations(cmd_mload_utopia_interface *pOps, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_InitByDIP(SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetInputSource(INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_GetInputSource(INPUT_SOURCE_TYPE_t *enInputSourceType, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_DisableInputSource(MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
//-----------------
// DIP_SET_WINDOW
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DIP Clip and Scaling window
/// @ingroup DIP_SET_WINDOW
/// @param  pstXC_SetWin_Info   \b IN: Clip and Scaling window
/// @param  u32InitDataLen      \b IN: size of XC_SETWIN_INFO
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_GetDEWindow(MS_WINDOW_TYPE *psWin, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVstart(MS_U16 u16Vstart, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHstart(MS_U16 u16Hstart, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVsize(MS_U16 u16Vsize, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHsize(MS_U16 u16Hsize, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_GetCaptureWindow(MS_WINDOW_TYPE* capture_win, SCALER_DIP_WIN eWindow);
//-----------------
// DIP_SET_COLOR_TRANSFORM
//-----------------
//-------------------------------------------------------------------------------------------------
/// Emable DIP RGB->YUV
/// @ingroup DIP_SET_COLOR_TRANSFORM
/// @param  benable             \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_EnableR2Y(MS_BOOL benable, SCALER_DIP_WIN eWindow);

//==============Set===================
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SWReset(SCALER_DIP_WIN eWindow);
//-----------------
// DIP_SET_PROPERTY
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DIP frame rate control
/// @ingroup DIP_SET_PROPERTY
/// @param  benable             \b IN: Enable/Disable
/// @param  u16In               \b IN: input frame
/// @param  u16Out              \b IN: output frame
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_FrameRateCtrl(MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrame(SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrameFast(SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_ClearInt(MS_U16 u8mask, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_Ena(MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_EnaInt(MS_U16 u8mask, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Tell DIP source is interlace or progressive
/// @ingroup DIP_SET_PROPERTY
/// @param  enScanMode          \b IN: interlace or progressive
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetSourceScanType(EN_XC_DIP_SCAN_MODE enScanMode, SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Enable DIP write from interlace to progressive
/// @ingroup DIP_SET_PROPERTY
/// @param  bEnable             \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_EnaInterlaceWrite(MS_BOOL bEnable, SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Set DIP output format
/// @ingroup DIP_SET_PROPERTY
/// @param  fmt                 \b IN: output format
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetOutputDataFmt(EN_XC_DIP_DATA_FMT fmt, SCALER_DIP_WIN eWindow);
//-----------------
// DIP_SET_WINPROPERTY
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DIP input and output infomation
/// @ingroup DIP_SET_WINPROPERTY
/// @param  pstDIPWinProperty   \b IN: input and output infomation
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetDIPWinProperty(ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Emable DIP YUV->RGB
/// @ingroup DIP_SET_COLOR_TRANSFORM
/// @param  benable             \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_EnableY2R(MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP alpha value
/// @ingroup DIP_SET_PROPERTY_COLOR_TRANSFORM
/// @param  u8AlphaVal          \b IN: alpha value
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetAlpha(MS_U8 u8AlphaVal, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Emable DIP UV swap
/// @ingroup DIP_SET_COLOR_TRANSFORM
/// @param  benable             \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SwapUV(MS_BOOL bEnable, SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Emable DIP YC swap
/// @ingroup DIP_SET_COLOR_TRANSFORM
/// @param  benable             \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SwapYC(MS_BOOL bEnable, SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Emable DIP RGB swap
/// @ingroup DIP_SET_PROPERTY_COLOR_TRANSFORM
/// @param  benable             \b IN: Enable/Disable
/// @param  enSwapType          \b IN: RGB swap type
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SwapRGB(MS_BOOL bEnable, EN_XC_DIP_RGB_SWAP_TYPE enSwapType, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// MApi_XC_DIP_SetWinProperty_Ex
/// @ingroup DIP_ToBeRemove
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetWinProperty_Ex(MS_U8 u8BufCnt ,MS_U16 u16Width, MS_U16 u16Height, MS_PHY u32BufStart ,MS_PHY u32BufEnd, SCALER_DIP_SOURCE_TYPE eSource,SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP capture mux when capture from OP
/// @ingroup DIP_SET_PROPERTY
/// @param  bEnable             \b IN: Enable/Disable
/// @param  eOpCapture          \b IN: capture mux when capture from OP
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetOutputCapture(MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow);


//-------------------------------------------------------------------------------------------------
/// Set DIP HV mirror
/// @ingroup DIP_SET_PROPERTY
/// @param  bHMirror            \b IN: Enable/Disable
/// @param  bVMirror            \b IN: Enable/Disable
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetMirror(MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP input from DRAM information
/// @ingroup DIP_SET_WINPROPERTY
/// @param  pstDIPRProperty     \b IN: input from DRAM information
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetDIPRProperty(ST_XC_DIPR_PROPERTY *pstDIPRProperty,SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP input from DRAM or VDEC information
/// @ingroup DIP_SET_WINPROPERTY
/// @param  pstDIPRProperty_ex     \b IN: input from DRAM or VDEC information
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC SYMBOL_WEAK E_APIXC_ReturnValue MApi_XC_DIP_SetDIPRProperty_EX(ST_XC_DIPR_PROPERTY_EX *pstDIPRProperty_ex, SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Attch interrupt call back function
/// @ingroup DIP_INTERRUPT
/// @param  pIntCb              \b IN: interrupt callback function
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_InterruptAttach(InterruptCb pIntCb,SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// De-attch interrupt call back function
/// @ingroup DIP_INTERRUPT
/// @param  eWindow             \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_InterruptDetach(SCALER_DIP_WIN eWindow);
//-----------------
// DIP_POWER_STATE
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set DIP suspend or resume status
/// @ingroup DIP_POWER_STATE
/// @param  u16PowerState       \b IN: Suspend/Resume
/// @return 0                   - Success
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_DIP_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// Set DIP rotate
/// @ingroup DIP_SET_PROPERTY
/// @param  bRotation           \b IN: Enable/Disable
/// @param  eRoDirection        \b IN: rotate type
/// @param  eTmpWindow          \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_Rotation(MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP pinpon with GOP
/// @ingroup DIP_SET_PROPERTY
/// @param  bPinpon             \b IN: Enable/Disable
/// @param  u32PinponAddr       \b IN: pinpon address
/// @param  eTmpWindow          \b IN: DIP number
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetPinpon(MS_BOOL bPinpon,MS_PHY u32PinponAddr,SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_SetHVSP(MS_BOOL bSelect,SCALER_DIP_WIN eWindow);
DLL_PUBLIC E_APIXC_ReturnValue MApi_XC_DIP_Set420TileBlock(EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow);
//-------------------------------------------------------------------------------------------------
/// Set DIP debug log level
/// @ingroup DIP_SET_PROPERTY
/// @param  u8DbgLevel          \b IN: EN_XC_DIP_DEBUG_LEVEL
/// @return E_APIXC_RET_OK      - Success
/// @return E_APIXC_RET_FAIL    - Failure
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MApi_XC_DIP_SetDbgLevel(EN_XC_DIP_DEBUG_LEVEL u8DbgLevel);
//==============Get===================
//-------------------------------------------------------------------------------------------------
/// Get interrpt frame bit-mask status
/// @ingroup DIP_INTERRUPT
/// @param  eWindow             \b IN: DIP number
/// @return MS_U16              - interrpt frame bit-mask status
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U16 MApi_XC_DIP_GetIntStatus(SCALER_DIP_WIN eWindow);
//-----------------
// DIP_GET_BUFINFO
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get DIP buffer address and size info
/// @ingroup DIP_GET_BUFINFO
/// @param  eWindow             \b IN: DIP number
/// @return BUFFER_INFO         - DIP buffer address and size info
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC BUFFER_INFO MApi_XC_DIP_GetBufInfo(SCALER_DIP_WIN eWindow);

//-----------------
// DIP_OP TEE
//-----------------
DLL_PUBLIC MS_U32 MDrv_XC_DIP_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32*  U32RetResNum, DIPRESOURCE* pResource);
DLL_PUBLIC MS_U32 MDrv_XC_DIP_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
DLL_PUBLIC MS_U32 MDrv_XC_DIP_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);

#ifdef __cplusplus
}
#endif
#endif /* APIXC_DWIN_H */

