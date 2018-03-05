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
/// @file   drvHVD.h
/// @brief  HVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_HVD_H_
#define _DRV_HVD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_HVD_LIB_CODE                     {'H','V','D','_'}    //Lib code
#define MSIF_HVD_LIBVER                       {'0','C'}            //LIB version     0.0 ~ Z.Z
#define MSIF_HVD_BUILDNUM                     {'0','2'}            //Build Number    00 ~ 99
#define MSIF_HVD_CHANGELIST                   {'0','0','6','4','4','6','7','9'} //P4 ChangeList Number

//------------------------------------------------------------------------------
/// @brief \b HVD_DRV_VERSION : HVD Version
//------------------------------------------------------------------------------
#define HVD_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_HVD_LIB_CODE,                  /* IP__                                             */  \
    MSIF_HVD_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_HVD_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_HVD_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define HVD_INIT_HW_MASK                            BMASK(3:0)         ///< HW Type
    #define HVD_INIT_HW_AVC                             BITS(3:0, 0)   ///< HW deflaut: AVC 0X00
    #define HVD_INIT_HW_AVS                             BITS(3:0, 1)   ///< HW : AVS 0X01
    #define HVD_INIT_HW_RM                              BITS(3:0, 2)   ///< HW: RM 0X10
    #define HVD_INIT_HW_MVC                             BITS(3:0, 3)   ///< HW: RM 0X11
    #define HVD_INIT_HW_VP8                             BITS(3:0, 4)   ///< HW: VP8 0X100
    #define HVD_INIT_HW_MJPEG                           BITS(3:0, 5)   ///< HW: MJPEG 0x101
    #define HVD_INIT_HW_VP6                             BITS(3:0, 6)   ///< HW: VP6 0x110
    #define HVD_INIT_HW_HEVC                            BITS(3:0, 7)   ///< HW: HEVC 0x111
    #define HVD_INIT_HW_VP9                             BITS(3:0, 8)   ///< HW: VP9 0x1000
#define HVD_INIT_MAIN_MASK                          BMASK(5:4)         ///< main type
    #define HVD_INIT_MAIN_FILE_RAW                      BITS(5:4, 0)   ///< main type: default: 0X00
    #define HVD_INIT_MAIN_FILE_TS                       BITS(5:4, 1)   ///< main type: 0X01
    #define HVD_INIT_MAIN_LIVE_STREAM                   BITS(5:4, 2)   ///< main type: 0X10
#define HVD_INIT_INPUT_MASK                         BMASK(6:6)         ///< process path for filling BBU table:  file mode. use drive; TSP: use tsp mode
    #define HVD_INIT_INPUT_TSP                          BITS(6:6, 0)   ///< tsp input( default)
    #define HVD_INIT_INPUT_DRV                          BITS(6:6, 1)   ///< driver input
#define HVD_INIT_START_CODE_MASK                    BMASK(7:7)         ///< AVC FILE MODE ONLY: mkv, mp4 container use.
    #define HVD_INIT_START_CODE_REMAINED                BITS(7:7, 0)   ///< start code remained.(Defualt)
    #define HVD_INIT_START_CODE_REMOVED                 BITS(7:7, 1)   ///< start code removed.
#define HVD_INIT_UTOPIA_ENVI                            BIT(8)         ///< check MIU sel and set it
#define HVD_INIT_DBG_FW                                 BIT(9)         ///< check FW is debug version or not
#define HVD_INIT_DUAL_ES_MASK                     BMASK(10:10)         ///< Dual ES buffer iput.
    #define HVD_INIT_DUAL_ES_DISABLE                  BITS(10:10, 0)   ///< Disable Dual ES buffer input.
    #define HVD_INIT_DUAL_ES_ENABLE                   BITS(10:10, 1)   ///< Enable Dual ES buffer input.

//#define HVD_INIT_ENABLE_ISR_DISP            BIT( 8)        ///< enable display ISR. ISR occurs at every Vsync.

#define HVD_RV_BROKEN_BY_US_MASK        0x80000000

#define HVD_BBU_TAG_LIMITATION          0x200000
#define QUANTITY_AFTER_BROKEN_BY_US     4
#define QUANTITY_LENGTH                  0x1FFF00
#define MAX_QUANTITY                     (QUANTITY_LENGTH * QUANTITY_AFTER_BROKEN_BY_US)
#define MIN_BBU_VACANCY_PER_PUSH        2
#define MIN_BBU_VACANCY_FOR_4K2K         (MIN_BBU_VACANCY_PER_PUSH * QUANTITY_AFTER_BROKEN_BY_US)          // about 6MB

#define HVD_MAX_DEC_NUM 2



#define HVD_CCRB_PACKET_LENGTH      8

#define T35_DVB_COUNTRY_CODE  0xB5
#define T35_DVB_PROVIDER_CODE 0x0031

#define DTV_AFD_CODE   0x44544731
#define DTV_DTB1_CODE  0x47413934

#define DTV_BAR_CODE 0x06
#define DTV_CC_CODE 0x03
#define DTV_DIRECTTV_CODE 0x2F

#define MAX_NTSC_CC_ELE  4
#define MAX_NTSC_CC_BYTE (MAX_NTSC_CC_ELE << 2)
#define MAX_DTV_CC_BYTE  128

#define CC_NTSC1    1
#define CC_NTSC2    2
#define CC_ATSC        4
#define CC_157      (CC_NTSC1|CC_NTSC2)

#define USER_DATA_MODE_DVB_NORMAL                 0x00
#define USER_DATA_MODE_DIRECTTV_CC                0x01
#define USER_DATA_MODE_FRM_PACKING_ARRANGEMENT    0x02
#define USER_DATA_MODE_ATSC_CC_RAW                0x04
#define USER_DATA_MODE_EXTERNAL_CC_BUFFER         0x10

#define MAX_608_CC_LEN 512
#define MAX_708_CC_LEN 512

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef void (*HVD_InterruptCb)(void);


//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: EN_CC_HVD_INFO
/// @brief \b Enum \b Description:  The type of Close Caption Selector
typedef enum
{
    HVD_EX_CC_SELECTOR_708_SW              = 0x0000,
    HVD_EX_CC_SELECTOR_RINGBUFFER    = 0x0001

} EN_CC_HVD_EX_INFO;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_Result
/// @brief \b Enum \b Description:  General result of HVD functions
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FAIL,                  ///< General fail cases.
    E_HVD_EX_OK,                    ///< Action success.
    E_HVD_EX_RET_INVALID_PARAMETER, ///< Function has invalid input.
    E_HVD_EX_RET_ILLEGAL_ACCESS,    ///< Illegal access. like driver not initialized.
    E_HVD_EX_RET_HARDWARE_BREAKDOWN,///< HW has no responses or impossible responses.
    E_HVD_EX_RET_OUTOF_MEMORY,      ///< The input memory config is not enough.
    E_HVD_EX_RET_UNSUPPORTED,       ///< Function is not supported by HVD driver.
    E_HVD_EX_RET_TIMEOUT,           ///< Action timeout.
    E_HVD_EX_RET_NOTREADY,          ///< Action not ready. User needs to call it again later.
    E_HVD_EX_RET_MEMORY_OVERWIRTE,  ///< The input memory config may be overwrite.
    E_HVD_EX_RET_QUEUE_FULL,        ///< HVD BBU queue or ES buffer is full.
    E_HVD_EX_RET_RE_INIT,
    E_HVD_EX_RET_NOT_RUNNING,
} HVD_EX_Result;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_GetPlayState
/// @brief \b Enum \b Description:  The current HVD play state.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_GSTATE_INIT,   ///< Before or during initialization.
    E_HVD_EX_GSTATE_PLAY,   ///< playback.
    E_HVD_EX_GSTATE_PAUSE,  ///< decode and display are all paused.
    E_HVD_EX_GSTATE_STOP,   ///< after stop.
} HVD_EX_GetPlayState;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_SyncType
/// @brief \b Enum \b Description:  The current mode type of HVD synchronization .
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_SYNC_ATS,  ///< Live stream, or TS file mode.
    E_HVD_EX_SYNC_PTS,  ///< only for file mode, input time stamp is PTS.
    E_HVD_EX_SYNC_DTS,  ///< only for file mode, input time stamp is DTS.
    E_HVD_EX_SYNC_STS,  ///< only for file mode, input time stamp is random, output should be sorted.
} HVD_EX_SyncType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_DispSpeed
/// @brief \b Enum \b Description:  The current mode type of HVD display speed
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_DISP_SPEED_FF_32X     = 32,    ///< Speed fast forward 32x.
    E_HVD_EX_DISP_SPEED_FF_16X     = 16,    ///< Speed fast forward 16x.
    E_HVD_EX_DISP_SPEED_FF_8X      = 8,     ///< Speed fast forward 8x.
    E_HVD_EX_DISP_SPEED_FF_4X      = 4,     ///< Speed fast forward 4x.
    E_HVD_EX_DISP_SPEED_FF_2X      = 2,     ///< Speed fast forward 2x.
    E_HVD_EX_DISP_SPEED_NORMAL_1X  = 1,     ///< Normal display speed.
    E_HVD_EX_DISP_SPEED_SF_2X      = -2,    ///< Slow forward 2X.
    E_HVD_EX_DISP_SPEED_SF_4X      = -4,    ///< Slow forward 4X.
    E_HVD_EX_DISP_SPEED_SF_8X      = -8,    ///< Slow forward 8X.
    E_HVD_EX_DISP_SPEED_SF_16X     = -16,   ///< Slow forward 16X.
    E_HVD_EX_DISP_SPEED_SF_32X     = -32,   ///< Slow forward 32X.
} HVD_EX_DispSpeed;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_DropDisp
/// @brief \b Enum \b Description:  The current mode type of HVD dropping decoded frames.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_DROP_DISPLAY_AUTO = (1 << 0),       ///< automatic drop mode, drop frame if display queue is more than threshold
    E_HVD_EX_DROP_DISPLAY_ONCE = (1 << 1),       ///< drop once, drop the number of non-ref frames
} HVD_EX_DropDisp;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_FrmRateConvMode
/// @brief \b Enum \b Description:  The current mode type of HVD frame rate convertion.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FRC_MODE_NORMAL,       ///< Disable FRC mode.
    E_HVD_EX_FRC_MODE_32PULLDOWN,   ///< 3:2 pulldown mode (ex. 24p a 60i or 60p)
    E_HVD_EX_FRC_MODE_PAL2NTSC ,    ///< PALaNTSC conversion (50i a 60i)
    E_HVD_EX_FRC_MODE_NTSC2PAL,     ///< NTSCaPAL conversion (60i a 50i)
    E_HVD_EX_FRC_MODE_DISP_2X,      ///< output rate is twice of input rate (ex. 30p a 60p)
    E_HVD_EX_FRC_MODE_24_TO_50,     ///< output rate 24P->50P 48I->50I
    E_HVD_EX_FRC_MODE_50P_60P,      ///< output rate 50P ->60P
    E_HVD_EX_FRC_MODE_60P_50P,      ///< output rate 60P ->50P
} HVD_EX_FrmRateConvMode;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_Codec
/// @brief \b Enum \b Description:  The supported codec type.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_AVC,
    E_HVD_EX_AVS,
    E_HVD_EX_RM,
    E_HVD_EX_MVC,
    E_HVD_EX_VP8,
    E_HVD_EX_MJPEG,
    E_HVD_EX_VP6,
    E_HVD_EX_HEVC,
    E_HVD_EX_VP9,
    E_HVD_EX_NONE,
} HVD_EX_Codec;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_GetModeStatus
/// @brief \b Enum \b Description:  The available mode information supported by HVD.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_GMODE_IS_SHOW_ERR_FRM,
    E_HVD_EX_GMODE_IS_REPEAT_LAST_FIELD,
    E_HVD_EX_GMODE_IS_ERR_CONCEAL,
    E_HVD_EX_GMODE_IS_SYNC_ON,
    E_HVD_EX_GMODE_IS_PLAYBACK_FINISH,
    E_HVD_EX_GMODE_SYNC_MODE,
    E_HVD_EX_GMODE_SKIP_MODE,
    E_HVD_EX_GMODE_DROP_MODE,
    E_HVD_EX_GMODE_DISPLAY_SPEED,
    E_HVD_EX_GMODE_FRC_MODE,
    E_HVD_EX_GMODE_ISR_TYPE,
    E_HVD_EX_GMODE_IS_STEP_DISPLAY = 0x0100,
    E_HVD_EX_GMODE_STREAM_TYPE,
} HVD_EX_GetModeStatus;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_SkipDecode
/// @brief \b Enum \b Description:  The HVD decoding frame types.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_SKIP_DECODE_ALL,  ///< decode all frames
    E_HVD_EX_SKIP_DECODE_I,    ///< decode I frames only
    E_HVD_EX_SKIP_DECODE_IP,   ///< decode I and referenced frames only( skip non-ref frames)
} HVD_EX_SkipDecode;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_FrmType
/// @brief \b Enum \b Description:  The frame type.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FRM_TYPE_I,        ///< I frame.
    E_HVD_EX_FRM_TYPE_P,        ///< P frame.
    E_HVD_EX_FRM_TYPE_B,        ///< B frame.
    E_HVD_EX_FRM_TYPE_OTHER,    ///reservase
} HVD_EX_FrmType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_FieldType
/// @brief \b Enum \b Description:  The Field type.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FIELDTYPE_NONE,    ///< no field.
    E_HVD_EX_FIELDTYPE_TOP,     ///< Top field only.
    E_HVD_EX_FIELDTYPE_BOTTOM,  ///< Bottom field only.
    E_HVD_EX_FIELDTYPE_BOTH,    ///< Both fields.
} HVD_EX_FieldType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_UartLevel
/// @brief \b Enum \b Description:  The debug level of HVD.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_UART_LEVEL_NONE = 0,   ///< Disable all uart message.
    E_HVD_EX_UART_LEVEL_ERR,        ///< Only output error message
    E_HVD_EX_UART_LEVEL_INFO,       ///< output general message, and above.
    E_HVD_EX_UART_LEVEL_DBG,        ///< output debug message, and above.
    E_HVD_EX_UART_LEVEL_TRACE,      ///< output function trace message, and above.
    E_HVD_EX_UART_LEVEL_FW,         ///< output FW message, and above.
} HVD_EX_UartLevel;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_PatternInfo
/// @brief \b Enum \b Description:  The information type of specific pattern.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FLUSH_PATTERN_SIZE,    ///< flush pattern size.
    E_HVD_EX_DUMMY_HW_FIFO,         ///< HW fifo size.
} HVD_EX_PatternInfo;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_DynamicScalingInfo
/// @brief \b Enum \b Description:  The information type of specific dynamic information.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_DS_BUF_MIUSEL,       ///< the HW MIU selection of the buffer of dynamic scaling. TRUE: MIU 1. FALSE: MIU 0.
    E_HVD_EX_DS_BUF_ADDR,         ///< the start physical address of the buffer of dynamic scaling.
    E_HVD_EX_DS_BUF_SIZE,         ///< the size of the buffer of dynamic scaling.
    E_HVD_EX_DS_VECTOR_DEPTH,     ///< the required vector depth of the dynamic scaling.
    E_HVD_EX_DS_INFO_ADDR,        ///< the scaler info buffer address of dynamic scaling.
    E_HVD_EX_DS_IS_ENABLED,       ///< if dynamic scaling is enabled.
} HVD_EX_DynamicScalingInfo;

typedef enum
{
    E_HVD_EX_FLUSH_NONE = 0,
    E_HVD_EX_FLUSH_RUNNING,     //HK -> FW
    E_HVD_EX_FLUSH_DONE         //FW -> HK

}HVD_EX_FlushStatus;


//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_IsrEvent
/// @brief \b Enum \b Description:  The information type of ISR event.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_ISR_NONE = 0,                        ///< disable ISR
    E_HVD_EX_ISR_DISP_ONE = BIT(0),               ///< HVD display one frame on screen.
    E_HVD_EX_ISR_DISP_REPEAT = BIT(1),            ///< The current displayed frame is repeated frame.
    E_HVD_EX_ISR_DISP_WITH_CC = BIT(2),           ///< Current displayed frame should be displayed with user data.
    E_HVD_EX_ISR_DISP_FIRST_FRM = BIT(3),         ///< HVD display first frame on screen.

    E_HVD_EX_ISR_DEC_ONE = BIT(8),                ///< HVD decoded one frame done.
    E_HVD_EX_ISR_DEC_I = BIT(9),                  ///< HVD decoded one I frame done.
    E_HVD_EX_ISR_DEC_HW_ERR = BIT(10),            ///< HVD HW found decode error.
    E_HVD_EX_ISR_DEC_CC_FOUND = BIT(11),          ///< HVD found one user data with decoded frame.
    E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE = BIT(12),  ///< HVD found display information change.
    E_HVD_EX_ISR_DEC_DATA_ERR = BIT(13),          ///< HVD HW found decode error.
    E_HVD_EX_ISR_DEC_FIRST_FRM = BIT(14),         ///< HVD decode first frame.
    E_HVD_EX_ISR_DEC_SEQ_HDR_FOUND = BIT(15),         ///< HVD decode first frame.
} HVD_EX_IsrEvent;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_GetFrmInfoType
/// @brief \b Enum \b Description:  The information type of get frame information.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_GFRMINFO_DISPLAY = 0,  ///< Displayed frame.
    E_HVD_EX_GFRMINFO_DECODE,       ///< Decoded frame.
    E_HVD_EX_GFRMINFO_NEXT_DISPLAY, ///< Next frame to be displayed.
    E_HVD_EX_GFRMINFO_DISPLAY_SUB,  ///< Displayed sub frame.
    E_HVD_EX_GFRMINFO_DECODE_SUB,   ///< Decoded sub frame.
    E_HVD_EX_GFRMINFO_LAST_DISPLAY
} HVD_EX_GetFrmInfoType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_GDataType
/// @brief \b Enum \b Description:  The information type of get data
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_GDATA_TYPE_DISP_CNT,
    E_HVD_EX_GDATA_TYPE_SKIP_CNT,
    E_HVD_EX_GDATA_TYPE_DROP_CNT,
    E_HVD_EX_GDATA_TYPE_IDLE_CNT,
    E_HVD_EX_GDATA_TYPE_VSYNC_CNT,
    E_HVD_EX_GDATA_TYPE_MAIN_LOOP_CNT,
    E_HVD_EX_GDATA_TYPE_AVC_LEVEL_IDC,
    E_HVD_EX_GDATA_TYPE_DISP_Q_SIZE,
    E_HVD_EX_GDATA_TYPE_ES_LEVEL,
    E_HVD_EX_GDATA_TYPE_AVC_VUI_DISP_INFO,
    E_HVD_EX_GDATA_TYPE_DISP_STC,
    E_HVD_EX_GDATA_TYPE_USERDATA_IDX_TBL_SIZE,
    E_HVD_EX_GDATA_TYPE_USERDATA_PACKET_SIZE,
    E_HVD_EX_GDATA_TYPE_REAL_FRAMERATE,
    E_HVD_EX_GDATA_TYPE_IS_ORI_INTERLACE_MODE,
    E_HVD_EX_GDATA_TYPE_FRAME_MBS_ONLY_FLAG,
    E_HVD_EX_GDATA_TYPE_FW_CODEC_TYPE,
    E_HVD_EX_GDATA_TYPE_FRC_MODE,
    E_HVD_EX_GDATA_TYPE_IS_LEAST_DISPQ_SIZE,
    E_HVD_EX_GDATA_TYPE_FIELD_PIC_FLAG,
    E_HVD_EX_GDATA_TYPE_VSYNC_BRIDGE_ADDR,
    E_HVD_EX_GDATA_TYPE_FW_STATUS_FLAG,
    E_HVD_EX_GDATA_TYPE_HVD_HW_MAX_PIXEL,
} HVD_EX_GDataType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_GDataType
/// @brief \b Enum \b Description:  The type of set settings
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_SSET_TIME_UNIT,            ///< set the HVD time unit of all interface. HVD_TimeUnit_Type
    E_HVD_EX_SSET_PITCH,                ///< set the pitch of vsync.
    E_HVD_EX_SSET_SYNC_EACH_FRM,        ///< HVD does sync action at every frame. TREU: turn on; FALSE: turn off.
    E_HVD_EX_SSET_MAX_DEC_TICK,         ///< HVD limits the max decode ticks for one field.
    E_HVD_EX_SSET_AUTO_FREE_ES,         ///< HVD frees the ES buffer data when ES is being fulled.
    E_HVD_EX_SSET_MIN_FRAME_GAP,        ///< set HVD not to report error which is caused by the frame gap larger than min frame gap.
    E_HVD_EX_SSET_DISABLE_DEBLOCKING,   ///< HVD will not do deblocking process.
    E_HVD_EX_SSET_DISABLE_QUARTER_PIXEL,///< HVD will not do quarter pixel process.
    E_HVD_EX_SSET_MIU_BURST_CNT_LEVEL,  ///< HVD MIU Burst Cnt, Arg 0~7: burst cnt level, 0xFFFFFFFF = Disable
} HVD_EX_SSettingsType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_PatternType
/// @brief \b Enum \b Description:  The type of special pattern for specific purpose.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_PATTERN_FLUSH = 0,           ///< Used after MDrv_HVD_Flush().
    E_HVD_EX_PATTERN_FILEEND,             ///< Used after MDrv_HVD_EX_SetDataEnd().
} HVD_EX_PatternType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_ESLevel
/// @brief \b Enum \b Description:  The level of ES buffer.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_ES_LEVEL_NORMAL = 0,
    E_HVD_EX_ES_LEVEL_UNDER = BIT(0),
    E_HVD_EX_ES_LEVEL_OVER = BIT(1),
} HVD_EX_ESLevel;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_ErrorCode
/// @brief \b Enum \b Description:  The type of HVD error
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_ERRCODE_GENERAL_BASE = 0x0000,
    E_HVD_EX_ERRCODE_OUT_OF_SPEC,
    E_HVD_EX_ERRCODE_UNKNOW_ERR,
    E_HVD_EX_ERRCODE_HW_BREAK_DOWN,
    E_HVD_EX_ERRCODE_HW_DEC_TIMEOUT,
    E_HVD_EX_ERRCODE_OUT_OF_MEMORY,
    E_HVD_EX_ERRCODE_UNKNOWN_CODEC,

    // AVC
    E_HVD_EX_ERRCODE_AVC_BASE = 0x1000,
    E_HVD_EX_ERRCODE_AVC_SPS_BROKEN,           // SPS is not valid
    E_HVD_EX_ERRCODE_AVC_SPS_NOT_IN_SPEC,
    E_HVD_EX_ERRCODE_AVC_SPS_NOT_ENOUGH_FRM,   // DPB size at specified level is smaller than the specified number of reference frames. This is not allowed
    E_HVD_EX_ERRCODE_AVC_PPS_BROKEN,           // PPS is not valid
    E_HVD_EX_ERRCODE_AVC_REF_LIST,
    E_HVD_EX_ERRCODE_AVC_NO_REF,
    E_HVD_EX_ERRCODE_AVC_RES,

    // AVS
    E_HVD_EX_ERRCODE_AVS_BASE = (0x2000),
    E_HVD_EX_ERRCODE_AVS_RES,

    // RM
    E_HVD_EX_ERRCODE_RM_BASE = (0x3000),
    E_HVD_EX_ERRCODE_RM_PACKET_HEADER,
    E_HVD_EX_ERRCODE_RM_FRAME_HEADER,
    E_HVD_EX_ERRCODE_RM_SLICE_HEADER,
    E_HVD_EX_ERRCODE_RM_BYTE_CNT,
    E_HVD_EX_ERRCODE_RM_DISP_TIMEOUT,
    E_HVD_EX_ERRCODE_RM_NO_REF,
    E_HVD_EX_ERRCODE_RM_RES,                    // out of supported resolution
    E_HVD_EX_ERRCODE_RM_VLC,
    E_HVD_EX_ERRCODE_RM_SIZE_OUT_FB_LAYOUT,
} HVD_EX_ErrorCode;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_TurboInitLevel
/// @brief \b Enum \b Description:  The level of turbo init mode.
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_TURBOINIT_NONE        = 0,     ///< Not omit any process.
    E_HVD_EX_TURBOINIT_CHECK       = BIT(0),     ///< Omit checking.
    E_HVD_EX_TURBOINIT_MEMORY      = BIT(1),     ///< Omit memory reset process.
    E_HVD_EX_TURBOINIT_DISPLAY     = BIT(2),     ///< Omit FW display setup process.
    E_HVD_EX_TURBOINIT_FW_RELOAD   = BIT(3), ///< Omit FW reload process.
} HVD_EX_TurboInitType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_FWSourceType
/// @brief \b Enum \b Description:  The type of fw binary input source
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_FW_SOURCE_NONE,       ///< No input FW; FW will be loaded by VDEC library.
    E_HVD_EX_FW_SOURCE_DRAM,       ///< input source from DRAM.
    E_HVD_EX_FW_SOURCE_FLASH,      ///< input source from FLASH.
} HVD_EX_FWSourceType;

//-----------------------------------------------------------------------------
/// @brief \b Enum \b Name: HVD_EX_DumpStatus
/// @brief \b Enum \b Description:  The type of fw binary input source
//-----------------------------------------------------------------------------
typedef enum
{
    E_HVD_EX_DUMP_STATUS_DRV   = BIT(0),       ///< Dump Driver status
    E_HVD_EX_DUMP_STATUS_FW    = BIT(1),       ///< Dump firmware status
    E_HVD_EX_DUMP_STATUS_HW    = BIT(2),       ///< Dump hardware status
} HVD_EX_DumpStatus;

typedef enum
{
    E_HVD_EX_DRV_STREAM_NONE = 0,
    E_HVD_EX_DRV_MAIN_STREAM,
    E_HVD_EX_DRV_SUB_STREAM,
    E_HVD_EX_DRV_MVC_STREAM,
} HVD_EX_DRV_StreamType;

typedef enum
{
    E_HVD_EX_FB_REDUCTION_NONE = 0,        ///< FB reduction disable
    E_HVD_EX_FB_REDUCTION_1_2 = 1,         ///< FB reduction 1/2
    E_HVD_EX_FB_REDUCTION_1_4 = 2,         ///< FB reduction 1/4
} HVD_EX_FBReductionType;

typedef enum
{
    E_HVD_EX_CLOCK_SPEED_NONE = 0,
    E_HVD_EX_CLOCK_SPEED_HIGHEST,
    E_HVD_EX_CLOCK_SPEED_HIGH,
    E_HVD_EX_CLOCK_SPEED_MEDIUM,
    E_HVD_EX_CLOCK_SPEED_LOW,
    E_HVD_EX_CLOCK_SPEED_LOWEST,
    E_HVD_EX_CLOCK_SPEED_DEFAULT,
} HVD_EX_ClockSpeed;


typedef enum
{
    E_HVD_EX_ES_BUF_STATUS_UNKNOWN   = 0,
    E_HVD_EX_ES_BUF_STATUS_UNDERFLOW = 1,
    E_HVD_EX_ES_BUF_STATUS_OVERFLOW  = 2,
    E_HVD_EX_ES_BUF_STATUS_NORMAL    = 3,

}HVD_EX_ES_Buf_Status;


//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_DispInfo
/// @brief \b Struct \b Description:  Store the HVD driver information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U16 u16HorSize;          ///< pixel width.
    MS_U16 u16VerSize;          ///< pixel height.
    MS_U32 u32FrameRate;    ///< 1000 times frames per second.
    MS_U8 u8AspectRate;     ///< aspect ration ID.
    MS_U8 u8Interlace;          ///< interlace content
    MS_U8 u8AFD;            ///< AFD ID number
    MS_U8 bChroma_idc_Mono;             ///< - TRUE: mono mode FALSE: colorful, not mono
    MS_U16 u16SarWidth;            ///< Sample aspect width ratio.
    MS_U16 u16SarHeight;           ///< Sample aspect height ratio.
    MS_U16 u16CropRight;            ///< crop right.
    MS_U16 u16CropLeft;             ///< crop left.
    MS_U16 u16CropBottom;           ///< crop bottom.
    MS_U16 u16CropTop;          ///< crop top.
    MS_U16 u16Pitch;            ///< pitch
    MS_U8  u8ColourPrimaries;   ///< Color Primaries in VUI
    //****************************
    MS_U8 reserved8_0;          ///< reserved.
    //******************************
} HVD_EX_DispInfo;     //  bytes

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_DrvInfo
/// @brief \b Struct \b Description:  Store the HVD driver information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_BOOL bAVC;       ///< - TRUE: HW does support AVC.  - FALSE: HW does not support AVC.
    MS_BOOL bAVS;       ///< - TRUE: HW does support AVS.  - FALSE: HW does not support AVS.
    MS_BOOL bRM;        ///< - TRUE: HW does support RM.  - FALSE: HW does not support RM.
    MS_U32  FWversion;       ///<  FW version number.
} HVD_EX_DrvInfo;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_DrvStatus
/// @brief \b Struct \b Description:  Store the HVD driver status
//-----------------------------------------------------------------------------
typedef struct
{
    MS_BOOL bInit;       ///< - TRUE: Initialization success.  - FALSE: Initialization failed or not initialized yet.
    MS_BOOL bBusy;       ///< - TRUE: Driver is processing  - FALSE: Driver is Idle.
} HVD_EX_DrvStatus;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_RVInfo
/// @brief \b Struct \b Description:  RV file information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U16 RV_Version;      ///< Real Video Bitstream version
    MS_U16 ulNumSizes;      ///< Real Video Number sizes
    MS_U16 ulPicSizes_w[8]; ///< Real Video file width
    MS_U16 ulPicSizes_h[8]; ///< Real Video file height
} HVD_EX_RVInfo;

typedef struct
{
    HVD_EX_FBReductionType LumaFBReductionMode;     ///< Luma frame buffer reduction mode.
    HVD_EX_FBReductionType ChromaFBReductionMode;   ///< Chroma frame buffer reduction mode.
    MS_U8                 u8EnableAutoMode;        /// 0: Disable, 1: Enable
} HVD_EX_FBReduction;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_MemCfg
/// @brief \b Struct \b Description:  Store the HVD driver config
//-----------------------------------------------------------------------------
typedef struct
{
    HVD_EX_FWSourceType eFWSourceType;             //!< the input FW source type.
    MS_U32  u32FWBinaryVAddr;                //!<  virtual address of input FW binary in DRAM
    MS_PHYADDR  u32FWBinaryAddr;                //!< the physical memory start address in Flash/DRAM memory of FW code
    MS_U32  u32FWBinarySize;                //!< the FW code size
    MS_U32 u32VLCBinaryVAddr;///< VLC table binary data buffer start address
    MS_PHYADDR u32VLCBinaryAddr;///< VLC table binary data buffer start address
    MS_U32 u32VLCBinarySize;///<VLC table binary data buffer size
    MS_PHYADDR  u32MIU1BaseAddr;       //!< the physical memory start address of MIU 1 base address. 0: default value.
    MS_U32  u32CodeBufVAddr;        //!< the virtual memory start address of code buffer
    MS_PHYADDR  u32CodeBufAddr;         //!< the physical memory start address of code buffer
    MS_U32  u32CodeBufSize;             //!< the code buffer size
    MS_U32  u32FrameBufVAddr;           //!< the virtual memory start address of frame buffer
    MS_PHYADDR  u32FrameBufAddr;            //!< the physical memory start address of frame buffer
    MS_U32  u32FrameBufSize;                //!< the frame buffer size
    MS_U32  u32BitstreamBufVAddr;           //!< the virtual memory start address of bit stream buffer
    MS_PHYADDR  u32BitstreamBufAddr;                //!< the physical memory start address of bit stream buffer
    MS_U32  u32BitstreamBufSize;            //!< the bit stream buffer size
    MS_U32  u32DrvProcessBufVAddr;       //!< the virtual memory start address of driver process buffer
    MS_PHYADDR  u32DrvProcessBufAddr;       //!< the physical memory start address of driver process buffer
    MS_U32  u32DrvProcessBufSize;        //!< the driver process buffer size
} HVD_EX_MemCfg;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_Init_Params
/// @brief \b Struct \b Description:  Store the initialization settings
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32ModeFlag;     ///< init mode flag, use HVD_INIT_* to setup HVD.
    MS_U32 u32FrameRate;     ///< frame rate.
    MS_U32 u32FrameRateBase;     ///< frame rate base. The value of u32FrameRate /u32FrameRateBase must be frames per sec.
    MS_U8   u8MinFrmGap;    ///< set the min frame gap.
    MS_U8   u8SyncType;         ///< HVD_EX_SyncType. sync type of current playback.
    MS_U16 u16Pitch;   ///< not zero: specify the pitch. 0: use default value.
    MS_U32 u32MaxDecTick;   ///< not zero: specify the max decode tick. 0: use default value.
    MS_BOOL bSyncEachFrm;   ///< TRUE: sync STC at each frame. FALSE: not sync each frame.
    MS_BOOL bAutoFreeES;   ///< TRUE: auto free ES buffer when ES buffer is full. FALSE: not do the auto free.
    MS_BOOL bAutoPowerSaving;   ///< TRUE: auto power saving. FALSE: not do the auto power saving.
    MS_BOOL bDynamicScaling;   ///< TRUE: enable Dynamic Scaling. FALSE: disable Dynamic Scaling.
    MS_BOOL bFastDisplay;   ///< TRUE: enable Fast Display. FALSE: disable Fast Display.
    MS_BOOL bUserData;   ///< TRUE: enable processing User data. FALSE: disable processing User data.
    MS_U8 u8TurboInit;       ///< HVD_TurboInitLevel. set the turbo init mode.
    MS_U8 u8TimeUnit;   ///< HVD_Time_Unit_Type.set the type of input/output time unit.
    MS_U16 u16DecoderClock;      ///< HVD decoder clock speed. 0: default value. non-zero: any nearist clock.
    MS_U16 u16ChipECONum;    ///< Chip revision, ECO number.    
    HVD_EX_RVInfo* pRVFileInfo;           ///< pointer to RV file info
    HVD_EX_FBReduction stFBReduction; ///< HVD Frame buffer reduction type
} HVD_EX_InitSettings;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_PacketInfo
/// @brief \b Struct \b Description:  Store the packet information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_VIRT u32Staddr;     ///< Packet offset from bitstream buffer base address. unit: byte.
    MS_VIRT u32Length;    ///< Packet size. unit: byte.
    MS_VIRT u32Staddr2;     ///< Packet offset from bitstream buffer base address. unit: byte.
    MS_VIRT u32Length2;    ///< Packet size. unit: byte.
    MS_U32 u32TimeStamp;    ///< Packet time stamp. unit: ms.
    MS_U32 u32ID_L;    ///< Packet ID low part.
    MS_U32 u32ID_H;    ///< Packet ID high part.
    MS_U32 u32AllocLength;    ///< Allocated Packet size. unit: byte.
} HVD_EX_PacketInfo;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_EX_FrameInfo
/// @brief \b Struct \b Description:  Store the frame information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_PHYADDR u32LumaAddr;                 ///< The start physical of luma data. Unit: byte.
    MS_PHYADDR u32ChromaAddr;               ///< The start physcal of chroma data. Unit: byte.
    MS_U32 u32TimeStamp;                ///< Time stamp(DTS, PTS) of current displayed frame. Unit: 90khz.
    MS_U32 u32ID_L;                     ///< low part of ID number decided by MDrv_HVD_EX_PushQueue().
    MS_U32 u32ID_H;                     ///< high part of ID number decided by MDrv_HVD_EX_PushQueue().
    MS_U16 u16Pitch;                        ///< The pitch of current frame.
    MS_U16 u16Width;                        ///< pixel width of current frame.
    MS_U16 u16Height;                       ///< pixel height of current frame.
    HVD_EX_FrmType eFrmType;     ///< picture type: I, P, B frame
    HVD_EX_FieldType  eFieldType;                 ///< none, top , bottom, both field
    MS_U32 u32PrivateData;              //[STB]only for AVC
    MS_PHYADDR u32LumaAddr_2bit;                 ///< The start offset of luma data. Unit: byte.
    MS_PHYADDR u32ChromaAddr_2bit;               ///< The start offset of chroma data. Unit: byte.
    MS_U16 u16Pitch_2bit;
    MS_U8  u8LumaBitdepth;
    MS_U8  u8ChromaBitdepth;
} HVD_EX_FrameInfo;

//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_DISP_INFO_THRESHOLD
/// @brief \b Struct \b Description:  Store the disp information threshold
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32FrmrateUpBound;       //Framerate filter upper bound
    MS_U32 u32FrmrateLowBound;      //Framerate filter lower bound
    MS_U32 u32MvopUpBound;          //mvop filter upper bound
    MS_U32 u32MvopLowBound;         //mvop filter lower bound
} HVD_EX_DispInfoThreshold;

typedef struct
{
    MS_U8  u8DecMod;
    MS_U8  u8CodecCnt;
    MS_U8  u8CodecType[HVD_MAX_DEC_NUM];
    MS_U8  u8ArgSize;
    MS_U32 u32Arg;
} HVD_EX_DecModCfg;
//-----------------------------------------------------------------------------
/// @brief \b Struct \b Name: HVD_UsrData_Info
/// @brief \b Struct \b Description:  Store the User Data information
//-----------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32Pts;
    MS_U8  u8PicStruct;           // picture_structure
    MS_U8  u8PicType;             // picture type: 1->I picture, 2->P,3->B
    MS_U8  u8TopFieldFirst;       // Top field first: 1 if top field first
    MS_U8  u8RptFirstField;       // Repeat first field: 1 if repeat field first

    MS_U32 u32DataBuf;            // User_Data data buffer address
    MS_U16 u16TmpRef;             // Temporal reference of the picture
    MS_U8  u8ByteCnt;             // User Data length
    MS_U8  u8Reserve;             // Reserved
} HVD_EX_UserData_Info;

typedef struct
{
    MS_BOOL bvaild;
    MS_BOOL bUsed;
    MS_U8   u8Frm_packing_arr_cnl_flag;
    MS_U8   u8Frm_packing_arr_type;
    MS_U8   u8content_interpretation_type;
    MS_U8   u1Quincunx_sampling_flag;
    MS_U8   u1Spatial_flipping_flag;
    MS_U8   u1Frame0_flipping_flag;
    MS_U8   u1Field_views_flag;
    MS_U8   u1Current_frame_is_frame0_flag;
    MS_U8   u1Frame0_self_contained_flag;
    MS_U8   u1Frame1_self_contained_flag;
    MS_U8   u4Frame0_grid_position_x;
    MS_U8   u4Frame0_grid_position_y;
    MS_U8   u4Frame1_grid_position_x;
    MS_U8   u4Frame1_grid_position_y;
    MS_U16  u16CropRight;
    MS_U16  u16CropLeft;
    MS_U16  u16CropBottom;
    MS_U16  u16CropTop;
    MS_U8   u8payload_len;
    MS_U8   u8WaitSPS;
    MS_U8   u8Reserved01;
    MS_U8   u8Reserved02;
    MS_U32  u32payload;
}HVD_EX_FrmPackingSEI;

typedef struct
{
    MS_VIRT  u32DSBufAddr;       // Buffer Address
    MS_U32  u32DSBufSize;       // Buffer Size
}HVD_EX_ExternalDSBuf;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void        MDrv_HVD_EX_SetCtrlsBase(MS_U32 u32Id);
void        MDrv_HVD_EX_MJPEG_InitSharemem(MS_U32 u32Id, MS_U32 u32CodeBufVAddr);
void        MDrv_HVD_EX_MJPEG_Exit(MS_U32 u32Id);

void        MDrv_HVD_EX_SetOSRegBase(MS_U32 u32RegBaseAddr);
HVD_EX_Result  MDrv_HVD_EX_GetFreeStream(MS_U32 *pu32Id, HVD_EX_DRV_StreamType eStreamType);
HVD_EX_Result  MDrv_HVD_EX_Init(MS_U32 u32Id, HVD_EX_MemCfg *pStMemCfg, HVD_EX_InitSettings *pStInitSettings);
HVD_EX_Result  MDrv_HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);
HVD_EX_Result  MDrv_HVD_EX_Play(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_Exit(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_Pause(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_Flush(MS_U32 u32Id, MS_BOOL bShowLast);
HVD_EX_Result  MDrv_HVD_EX_StepDisp(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_StepDecode(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_PushQueue(MS_U32 u32Id, HVD_EX_PacketInfo *pInfo);
HVD_EX_Result  MDrv_HVD_EX_PushQueue_Fire(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_DecodeIFrame(MS_U32 u32Id, MS_PHYADDR u32SrcSt, MS_VIRT u32SrcSize);

HVD_EX_Result  MDrv_HVD_EX_SetDataEnd(MS_U32 u32Id, MS_BOOL bEnd);
HVD_EX_Result  MDrv_HVD_EX_SetDispErrFrm(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetDispRepeatField(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetSkipDecMode(MS_U32 u32Id, HVD_EX_SkipDecode eDecType);
HVD_EX_Result  MDrv_HVD_EX_SetDispSpeed(MS_U32 u32Id, HVD_EX_DispSpeed eSpeed);
HVD_EX_Result  MDrv_HVD_EX_SetSyncActive(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetDropMode(MS_U32 u32Id, HVD_EX_DropDisp eMode, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_RstPTS(MS_U32 u32Id, MS_U32 u32PTS);
HVD_EX_Result  MDrv_HVD_EX_SetFrcMode(MS_U32 u32Id, HVD_EX_FrmRateConvMode eMode);
HVD_EX_Result  MDrv_HVD_EX_SetSyncTolerance(MS_U32 u32Id, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetSyncVideoDelay(MS_U32 u32Id, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetSyncFreeRunTH(MS_U32 u32Id, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetSyncRepeatTH(MS_U32 u32Id, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetErrConceal(MS_U32 u32Id, MS_BOOL u32Arg);
void        MDrv_HVD_EX_SetDbgLevel(HVD_EX_UartLevel elevel);
HVD_EX_Result  MDrv_HVD_EX_SeekToPTS(MS_U32 u32Id, MS_U32 u32PTS);
HVD_EX_Result  MDrv_HVD_EX_SkipToPTS(MS_U32 u32Id, MS_U32 u32PTS);
HVD_EX_Result  MDrv_HVD_EX_SetFreezeImg(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetDispOneField(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetISREvent(MS_U32 u32Id, MS_U32 u32Event, HVD_InterruptCb fnISRHandler);
MS_BOOL     MDrv_HVD_EX_SetEnableISR(MS_U32 u32Id, MS_BOOL bEnable);
MS_BOOL     MDrv_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable);
MS_BOOL     MDrv_HVD_EX_SetMVOPDone(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height);
HVD_EX_Result  MDrv_HVD_EX_SetDynScalingParam(MS_U32 u32Id, void *pStAddr, MS_U32 u32Size);
HVD_EX_Result  MDrv_HVD_EX_SetAutoRmLstZeroByte(MS_U32 u32Id, MS_BOOL bOn);
HVD_EX_Result  MDrv_HVD_EX_SetDispInfoTH(MS_U32 u32Id, HVD_EX_DispInfoThreshold *DispInfoTH);
HVD_EX_Result  MDrv_HVD_EX_SetFastDisplay(MS_U32 u32Id, MS_BOOL bFastDisplay);
HVD_EX_Result  MDrv_HVD_EX_SetIgnoreErrRef(MS_U32 u32Id, MS_BOOL bIgnore);
HVD_EX_Result  MDrv_HVD_EX_ForceFollowDTVSpec(MS_U32 u32Id, MS_BOOL bEnable);

MS_BOOL     MDrv_HVD_EX_IsISROccured(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsDispFinish(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsFrameShowed(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsStepDecodeDone(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_CheckDispInfoRdy(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsDispInfoChg(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsIdle(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsSyncStart(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsSyncReach(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsLowDelay(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsIFrmFound(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_Is1stFrmRdy(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_IsAllBufferEmpty(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_IsAlive(MS_U32 u32Id);

MS_U32      MDrv_HVD_EX_GetBBUVacancy(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_GetDispInfo(MS_U32 u32Id, HVD_EX_DispInfo *pInfo);
MS_U32      MDrv_HVD_EX_GetPTS(MS_U32 u32Id);
MS_U64      MDrv_HVD_EX_GetU64PTS(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetNextPTS(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetNextDispQPtr(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetDataErrCnt(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetDecErrCnt(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetESWritePtr(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetESReadPtr(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetESQuantity(MS_U32 u32Id);
MS_BOOL     MDrv_HVD_EX_GetCaps(HVD_EX_Codec u32Type);
MS_U32      MDrv_HVD_EX_GetErrCode(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetPlayMode(MS_U32 u32Id, HVD_EX_GetModeStatus eMode);
HVD_EX_GetPlayState MDrv_HVD_EX_GetPlayState(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetDecodeCnt(MS_U32 u32Id);
MS_U8       MDrv_HVD_EX_GetActiveFormat(MS_U32 u32Id);
const HVD_EX_DrvInfo *MDrv_HVD_EX_GetInfo(void);
HVD_EX_Result  MDrv_HVD_EX_GetLibVer(const MSIF_Version **ppVersion);
MS_BOOL     MDrv_HVD_EX_GetStatus(MS_U32 u32Id, HVD_EX_DrvStatus *pstatus);
HVD_EX_Result  MDrv_HVD_EX_GetFrmInfo(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType, HVD_EX_FrameInfo *pInfo);
MS_BOOL     MDrv_HVD_EX_GetISRInfo(MS_U32 u32Id, MS_U32 *eType);
MS_U32      MDrv_HVD_EX_CalLumaSum(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType);
MS_U32      MDrv_HVD_EX_GetUserData_Wptr(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetUserData_Packet(MS_U32 u32Id, MS_U32 u32Idx, MS_U32 *u32Size);
HVD_EX_Result  MDrv_HVD_EX_GenPattern(MS_U32 u32Id, HVD_EX_PatternType eType, MS_U32 u32VAddr, MS_U32 *u32Size);
MS_U32      MDrv_HVD_EX_GetPatternInfo(MS_U32 u32Id, HVD_EX_PatternInfo eType);
MS_U32      MDrv_HVD_EX_GetDynamicScalingInfo(MS_U32 u32Id, HVD_EX_DynamicScalingInfo eType);
MS_BOOL     MDrv_HVD_EX_GetFrmRateIsSupported(MS_U32 u32Id);

MS_U32      MDrv_HVD_EX_GetData(MS_U32 u32Id, HVD_EX_GDataType eType);
MS_U32      MDrv_HVD_EX_GetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr);
void        MDrv_HVD_EX_DbgDumpStatus(MS_U32 u32Id, HVD_EX_DumpStatus eFlag);
void        MDrv_HVD_EX_SetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetCmd_Dbg(MS_U32 u32Id, MS_U32 u32Cmd, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetSettings_Pro(MS_U32 u32Id, HVD_EX_SSettingsType eType, MS_U32 u32Arg);
HVD_EX_Result  MDrv_HVD_EX_SetBalanceBW(MS_U32 u32Id, MS_U8 u8QPCnt, MS_U8 u8DBCnt, MS_U8 u8Upper);
MS_S64      MDrv_HVD_EX_GetPtsStcDiff(MS_U32 u32Id);
MS_U32      MDrv_HVD_EX_GetDrvFwVer(void);
HVD_EX_Result  MDrv_HVD_EX_SetFdMaskDelayCnt(MS_U32 u32Id, MS_U8 u8DelayCnt);
HVD_EX_Result  MDrv_HVD_EX_SetOutputFRCMode(MS_U32 u32Id, MS_U8 u8FrameRate, MS_U8 u8Interlace);
HVD_EX_Result  MDrv_HVD_EX_DispFrame(MS_U32 u32Id, MS_U32 u32FrmIdx);
HVD_EX_Result  MDrv_HVD_EX_FreeFrame(MS_U32 u32Id, MS_U32 u32FrmIdx);
HVD_EX_Result  MDrv_HVD_EX_EnableDispQue(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_EnableVSizeAlign(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_ShowDecodeOrder(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_Disp_Ignore_Crop(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_RmEnablePtsTbl(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetFRCDropType(MS_U32 u32Id, MS_U8 u8DropType);
MS_U32      MDrv_HVD_EX_GetDrvFwVer(void);
MS_U32      MDrv_HVD_EX_GetFwVer(MS_U32 u32Id);
HVD_EX_Result MDrv_HVD_EX_DispOutsideMode(MS_U32 u32Id, MS_BOOL bEnable);
MS_BOOL MDrv_HVD_SetSingleDecodeMode(MS_BOOL bEnable);
MS_BOOL MDrv_HVD_SetDecodeMode(HVD_EX_DecModCfg *pstCfg);
void MDrv_HVD_EX_SetBurstMode(MS_U32 u32Id, MS_BOOL bBurst);
HVD_EX_Result  MDrv_HVD_EX_ForceInterlaceMode(MS_U32 u32Id, MS_U8 u8Mode);
HVD_EX_Result  MDrv_HVD_EX_Support_AVC2MVC(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_3DLR_View_Exchange(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_Enable_New_Slow_Motion(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_SetDTVUserDataMode(MS_U32 u32Id,MS_U8 u8UserDataMode);
MS_BOOL MDrv_HVD_EX_GetUsrDataIsAvailable(MS_U32 u32Id);
HVD_EX_Result  MDrv_HVD_EX_GetUserDataInfo(MS_U32 u32Id,HVD_EX_UserData_Info* pUsrInfo);
HVD_EX_Result  MDrv_HVD_EX_GetFrmPackingArrSEI(MS_U32 u32Id,HVD_EX_FrmPackingSEI *pFrmPacking);
HVD_EX_Result  MDrv_HVD_EX_SuspendDynamicScale(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_PushDispQWithRefNum(MS_U32 u32Id, MS_U8 u8Mode);
HVD_EX_Result  MDrv_HVD_EX_IgnorePicOverrun(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_DynamicScalingResvNBuffer(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_CtlSpeedInDispOnly(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result  MDrv_HVD_EX_AVCSupportRefNumOverMaxDBPSize(MS_U32 u32Id, MS_BOOL bEnable);
MS_U32 MDrv_HVD_EX_GetBBUQNum(MS_U32 u32Id);
MS_U32 MDrv_HVD_EX_GetDispFrmNum(MS_U32 u32Id);
HVD_EX_Result MDrv_HVD_Init_Share_Mem(void);
HVD_EX_Result MDrv_HVD_EX_SetHVDClockSpeed(HVD_EX_ClockSpeed eClockSpeed);
HVD_EX_Result MDrv_HVD_EX_SetVPUClockSpeed(HVD_EX_ClockSpeed eClockSpeed);
HVD_EX_Result MDrv_HVD_EX_DSReportDispInfoChange(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_SetSecureMode(MS_U32 u32Id, MS_U32 u32SecureMode);
HVD_EX_Result MDrv_HVD_EX_SupportRefNumOverMaxDpbSize(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_RVU_Setting_Mode(MS_U32 u32Id, MS_U32 u32Param);
HVD_EX_Result MDrv_HVD_EX_FramerateHandling(MS_U32 u32Id, MS_U32 u32FrameRate);
HVD_EX_Result MDrv_HVD_EX_DualNonBlockMode(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_IgnorePicStructDisplay(MS_U32 u32Id, MS_U32 param);
HVD_EX_Result MDrv_HVD_EX_InputPtsFreerunMode(MS_U32 u32Id, MS_U32 param);
HVD_EX_Result MDrv_HVD_EX_ErrConcealStartSlice1stMB(MS_U32 u32Id, MS_U32 param);
HVD_EX_Result MDrv_HVD_EX_SetExternalDSBuffer(MS_U32 u32Id, HVD_EX_ExternalDSBuf *pExternalBuf);
HVD_EX_Result MDrv_HVD_EX_SetHVDColBBUMode(MS_U32 u32Id, MS_U8 bEnable);

HVD_EX_Result MDrv_HVD_EX_CC_Init(MS_U32 u32Id);
HVD_EX_Result MDrv_HVD_EX_CC_SetCfg(MS_U32 u32Id, MS_U8 u8Operation, MS_U16 u16BufferSize, MS_U8 u8CC608);
HVD_EX_Result MDrv_HVD_EX_CC_Set_RB_StartAddr(MS_U32 u32Id, MS_U32 u32StartPAddress, MS_U8 u8CC608);
HVD_EX_Result MDrv_HVD_EX_CC_SyncRB_RdAddr2WrAddr(MS_U32 u32Id, MS_U8 u8CC608);
HVD_EX_Result MDrv_HVD_EX_CC_Adv_RB_ReadAddr(MS_U32 u32Id, MS_U32 u32EachPacketSize, MS_U8 u8CC608);
HVD_EX_Result MDrv_HVD_EX_CC_DisableParsing(MS_U32 u32Id, MS_U8 u8CC608);
HVD_EX_Result MDrv_HVD_EX_CC_GetInfo(MS_U32 u32Id, MS_U32 selector, MS_U8 type, MS_U32 *p1, MS_U32 *p2);
MS_BOOL MDrv_HVD_EX_CC_IsHvdRstDone(MS_U32 u32Id, MS_U8 type);
MS_U8 MDrv_HVD_EX_CC_GetOverflowStatus(MS_U32 u32Id, MS_U8 u8CC608);
MS_U32 MDrv_HVD_EX_CC_Get_RB_WriteAddr(MS_U32 u32Id, MS_U8 u8CC608);
MS_U32 MDrv_HVD_EX_CC_Get_RB_ReadAddr(MS_U32 u32Id, MS_U8 u8CC608);
MS_BOOL MDrv_HVD_EX_CC_InfoEnhanceMode(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_AutoExhaustESMode(MS_U32 u32Id, MS_U32 u32ESbound);
MS_U32 MDrv_HVD_EX_GetESBufferStatus(MS_U32 u32Id);
HVD_EX_Result MDrv_HVD_EX_ReturnInvalidAFD(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_AVCForceBrokenByUs(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_ShowFirstFrameDirect(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_AVCResizeDosDispPendBuf(MS_U32 u32Id, MS_U32 u32Size);
HVD_EX_Result MDrv_HVD_EX_SetMinTspSize(MS_U32 u32Id, MS_U32 u32Size);
HVD_EX_Result MDrv_HVD_EX_SetDmxFrameRate(MS_U32 u32Id, MS_U32 u32Value);
HVD_EX_Result MDrv_HVD_EX_SetDmxFrameRateBase(MS_U32 u32Id, MS_U32 u32Value);
MS_BOOL MDrv_HVD_EX_GetSupport2ndMVOPInterface(void);
MS_BOOL MDrv_HVD_EX_SetVPUSecureMode(MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_SetExternal_CC608_Buffer(MS_U32 u32Id, MS_U32 u32Addr, MS_U32 u32Len);
HVD_EX_Result MDrv_HVD_EX_SetExternal_CC708_Buffer(MS_U32 u32Id, MS_U32 u32Addr, MS_U32 u32Len);
HVD_EX_Result MDrv_HVD_EX_OnePendingBufferMode(MS_U32 u32Id,MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_TsInBbuMode(MS_U32 u32Id,MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_IapGnBufShareBWMode(MS_U32 u32Id,MS_BOOL bEnable, MS_U32 u32IapGnBufAddr, MS_U32 u32IapGnBufSize);
HVD_EX_Result MDrv_HVD_EX_SetPTSUsecMode(MS_U32 u32Id, MS_BOOL bEnable);
HVD_EX_Result MDrv_HVD_EX_Set_Err_Tolerance(MS_U32 u32Id, MS_U32 u32Arg);
HVD_EX_Result MDrv_HVD_EX_PVRTimeShiftSeamlessMode(MS_U32 u32Id, MS_U8 u8Arg);
MS_U8 MDrv_HVD_EX_GetDSBufMiuSelect(MS_U32 u32Id);
void MDrv_HVD_EX_BBU_Proc(MS_U32 u32Id);
void MDrv_HVD_EX_BBU_StopProc(MS_U32 u32Id);
MS_BOOL MDrv_HVD_EX_CHIP_Capability(void* pHWCap);

#else
MS_BOOL MDrv_HVD_EX_LoadCodeInSecure(MS_U32 addr);
MS_BOOL MDrv_HVD_EX_SetLockDownRegister(void* param);
#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_HVD_H_

