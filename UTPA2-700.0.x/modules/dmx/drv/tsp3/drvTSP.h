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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP.h
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_TSP_H_
#define _DRV_TSP_H_



#ifdef __cplusplus
extern "C"
{
#endif

#define TSP_VER_1_0                     0x0100UL
#define TSP_VER_2_0                     0x0200UL
#define TSP_VER_3_0                     0x0300UL
#define TSP_VER_4_0                     0x0400UL

#define TSP_VERSION                     TSP_VER_3_0

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#define DRVTSP_FILTER_DEPTH         16UL                                                              // TSP_FILTER_DEPTH

/// TSP byte address alignment unit
#define DRVTSP_ALIGN_UNIT           8UL

/// TSP byte address alignment macro
#define DRVTSP_ALIGN(_x)            ALIGN_8((MS_U32)_x)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Transport stream null PID
#define DRVTSP_PID_NULL             0x1FFFUL
#define MSIF_TSP_LIB_CODE           {'T','S','P','3'}                                               // Lib code
#define MSIF_TSP_LIBVER             {'1','3'}                                                       // LIB version
#define MSIF_TSP_BUILDNUM           {'0','3'}                                                       // Build Number
#define MSIF_TSP_CHANGELIST         {'0','0','6','0','7','2','6','6'}                               // P4 ChangeList Number

#define TSP_DRV_VERSION             /* Character String for DRV/API version             */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* 0x0000                                           */  \
    MSIF_MOD,                       /* 0x0000                                           */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_TSP_LIB_CODE,              /* IP__                                             */  \
    MSIF_TSP_LIBVER,                /* 0.0 ~ Z.Z                                        */  \
    MSIF_TSP_BUILDNUM,              /* 00 ~ 99                                          */  \
    MSIF_TSP_CHANGELIST,            /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// TSP DDI return value

/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{
typedef enum
{
    DRVTSP_FAIL                     = 0,
    DRVTSP_OK,
    DRVTSP_INVALID_PARAM,
    DRVTSP_FUNC_ERROR,
    DRVTSP_INVALID_SECFLT,
    DRVTSP_NOT_SUPPORTED,
} TSP_Result;

/// @}

#define DOUBLE_BUFFER_ENABLE        TRUE
#define DOUBLE_BUFFER_SWITCH        FALSE

#if 0
/// TSP channel Type
typedef enum //_DrvTSP_FltType
{
    // get TS from Live stream
    E_DRVTSP_FLT_TYPE_VIDEO         = 0x00000000,                                                   ///<\n
    E_DRVTSP_FLT_TYPE_AUDIO         ,                                                               ///<\n
    E_DRVTSP_FLT_TYPE_AUDIO2        ,                                                               ///<\n
    E_DRVTSP_FLT_TYPE_PVR           ,                                                               ///<\n

    // Section Filter Group
    E_DRVTSP_FLT_TYPE_SECTION_MASK  = 0x20000000,                                                   ///<\n
    E_DRVTSP_FLT_TYPE_SECTION       ,                                                               ///<\n
    E_DRVTSP_FLT_TYPE_PCR           ,                                                               ///<\n
    E_DRVTSP_FLT_TYPE_PES           ,                                                               ///<\n
    E_DRVTSP_FLT_TYPE_PACKET        ,                                                               //[Reserved]
    E_DRVTSP_FLT_TYPE_TELETEXT      ,
    E_DRVTSP_FLT_TYPE_EMM           ,                                                               //[Reserved]
    E_DRVTSP_FLT_TYPE_ECM           ,                                                               //[Reserved]


    E_DRVTSP_FLT_SOURCE_TYPE_MASK   =  0xC0000000,
    E_DRVTSP_FLT_SOURCE_TYPE_LIVE   =  0x80000000,
    E_DRVTSP_FLT_SOURCE_TYPE_FILE   =  0x40000000,

    E_DRVTSP_FLT_TYPE_LAST_ENUM

} DrvTSP_FltType;
#endif

#define DrvTSP_FltType                  MS_U32
// get TS from Live stream
#define E_DRVTSP_FLT_TYPE_VIDEO                     0x00000000UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_AUDIO                     0x00000001UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_AUDIO2                    0x00000002UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_PVR                       0x00000003UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_PVR1                      0x00000004UL
#define E_DRVTSP_FLT_TYPE_VIDEO3D                   0x00000008UL
#define E_DRVTSP_FLT_TYPE_CB                        0x00000009UL
#define E_DRVTSP_FLT_TYPE_AUDIO3                    0x0000000AUL
#define E_DRVTSP_FLT_TYPE_AUDIO4                    0x0000000BUL

// Section Filter Group
#define E_DRVTSP_FLT_TYPE_SECTION_MASK              0x01000000UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_SECTION                   0x01000001UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_PCR                       0x01000002UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_PES                       0x01000003UL                                                  ///<\n
#define E_DRVTSP_FLT_TYPE_PACKET                    0x01000004UL                                                  //[Reserved]
#define E_DRVTSP_FLT_TYPE_TELETEXT                  0x01000005UL
#define E_DRVTSP_FLT_TYPE_EMM                       0x01000006UL                                                  //[Reserved]
#define E_DRVTSP_FLT_TYPE_ECM                       0x01000007UL                                                  //[Reserved]
#define E_DRVTSP_FLT_TYPE_SECTION_VER               0x01000008UL
#define E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI           0x01000009UL

#define E_DRVTSP_FLT_TYPE_LAST_ENUM                 0x0100000AUL

#define E_DRVTSP_FLT_MASK                           0xF2FC0F00UL
#define E_DRVTSP_FLT_SOURCE_TYPE_MASK               0xC0FC0000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_LIVE               0x80000000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_FILE               0x40000000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_TS1                0x00800000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_TS2                0x00400000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_TSCB               0x00200000UL
#define E_DRVTSP_FLT_SOURCE_TYPE_FILE1              0x00100000UL                                                // file1 fource from TS1
#define E_DRVTSP_FLT_SOURCE_TYPE_FILE2              0x00080000UL                                                // file2 fource from TS2
#define E_DRVTSP_FLT_SOURCE_TYPE_LIVEB              E_DRVTSP_FLT_SOURCE_TYPE_FILE

// Source ID define.  For merge stream
#define E_DRVTSP_FLT_SOURCEID_SHIFT                 8UL
#define E_DRVTSP_FLT_SOURCEID_MASK                  0x00000F00UL

#define E_DRVTSP_FLT_TYPE_DUPSEC                    0x02000000UL                   // Only for section filter duplicate patch

#define E_DRVTSP_FLT_SCMB_MASK                      0x30000000UL
#define E_DRVTSP_FLT_TYPE_SCMB                      0x20000000UL
#define E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY             0x10000000UL

#define DrvTSP_FltMode MS_U32
#define E_DRVTSP_FLT_MODE_CONTI                     0x00000000UL
#define E_DRVTSP_FLT_MODE_ONESHOT                   0x00000001UL
#define E_DRVTSP_FLT_MODE_PESSCMCHK                 0x00000003UL                 //Only for PES type checking SCMB status

// optional
#define E_DRVTSP_FLT_MODE_CRCCHK                    0x00000002UL
#define E_DRVTSP_FLT_MODE_AUTO_ADDR                 0x00000004UL // auto move read point mode

#define DrvTSP_PKTDMXSrcType                        MS_U32
#define E_DRVTSP_PKTSRC_DEMUX0                      0x00000001UL
#define E_DRVTSP_PKTSRC_DEMUXFILE                   0x00000002UL
#define E_DRVTSP_PKTSRC_DEMUX1                      0x00000003UL
#define E_DRVTSP_PKTSRC_DEMUX2                      0x00000004UL
#define E_DRVTSP_PKTSRC_DEMUXMMFI0                  0x00000006UL
#define E_DRVTSP_PKTSRC_DEMUXMMFI1                  0x00000007UL

//--------------------------------------------------
// Debug table
typedef enum
{
    E_DRVTSP_DEBUG_CMD_NONE,
    E_DRVTSP_DEBUG_CMD_CLEAR,
    E_DRVTSP_DEBUG_CMD_ENABLE,
    E_DRVTSP_DEBUG_CMD_DISABLE,
} DrvTSP_Debug_Cmd;

typedef enum
{
    E_DRVTSP_DEBUG_SRC_TS0,
    E_DRVTSP_DEBUG_SRC_TS1,
    E_DRVTSP_DEBUG_SRC_TS2,
    E_DRVTSP_DEBUG_SRC_FILE,
} DrvTSP_Debug_Src;

typedef enum
{
    E_DRVTSP_DEBUG_TSIF_TS0,
    E_DRVTSP_DEBUG_TSIF_TS1,
    E_DRVTSP_DEBUG_TSIF_TSCB,
} DrvTSP_Debug_Tsif;

typedef enum
{
    E_DRVTSP_DEBUG_FIFO_VIDEO,
    E_DRVTSP_DEBUG_FIFO_AUDIO,
    E_DRVTSP_DEBUG_FIFO_VIDEO3D,
    E_DRVTSP_DEBUG_FIFO_AUDIOB,
    E_DRVTSP_DEBUG_FIFO_AUDIOC,
    E_DRVTSP_DEBUG_FIFO_AUDIOD,
} DrvTSP_Debug_Fifo;

typedef enum
{
    E_DRVTSP_DEBUG_PKT_DEMUX_0,
    E_DRVTSP_DEBUG_PKT_DEMUX_0_FILE,
    E_DRVTSP_DEBUG_PKT_DEMUX_1,
    E_DRVTSP_DEBUG_PKT_DEMUX_2,
    E_DRVTSP_DEBUG_MMFI0,
    E_DRVTSP_DEBUG_MMFI1,
} DrvTSP_Debug_Fifo_Src;

/// TSP channel state bit flags
typedef enum //_DrvTSP_FltState
{
    E_DRVTSP_FLT_STATE_FREE           = 0x00000000UL,                                                 ///<\n
    E_DRVTSP_FLT_STATE_ALLOC          = 0x00000001UL,                                                 ///<\n
    E_DRVTSP_FLT_STATE_ENABLE         = 0x00000002UL,                                                 ///<\n
    E_DRVTSP_FLT_STATE_SCRAMBLED      = 0x00000004UL,                                                 //[TODO]
    E_DRVTSP_FLT_STATE_STREAM_AVAIL   = 0x00000008UL,                                                 //[TODO]
    E_DRVTSP_FLT_STATE_SCRAMBLED_ORG  = 0x00000010UL,
    E_DRVTSP_FLT_STATE_OVERFLOW       = 0x00010000UL,                                                 //[Reserved]
    E_DRVTSP_FLT_STATE_SUSPEND        = 0x20000000UL,
    E_DRVTSP_FLT_STATE_CBRUN          = 0x40000000UL,                                                 // Callback processing
    E_DRVTSP_FLT_STATE_FREEING        = 0x80000000UL,                                                 // Free filter is  running
    E_DRVTSP_FLT_STATE_NA             = 0xFFFFFFFFUL,
} DrvTSP_FltState;


/// TSP record mode
typedef enum //_DrvTSP_RecMode
{
    //[TODO] rename REC_MODE to PVR_MODE

    /// Record ENG0 by @ref E_DRVTSP_FLT_TYPE_PVR
    E_DRVTSP_REC_MODE_ENG0_FLTTYPE    = 0x00000000UL,                                                 // TSP_PVR_CTRL_ENG(0)
    /// Record ENG1 by @ref E_DRVTSP_FLT_TYPE_PVR
    E_DRVTSP_REC_MODE_ENG1_FLTTYPE    = 0x00010000UL,                                                 // TSP_PVR_CTRL_ENG(1)
    /// Record ENG0 bypass PID fliter
    E_DRVTSP_REC_MODE_ENG0_BYPASS     = 0x00000002UL,                                                 // TSP_PVR_CTRL_ENG(0) + TSP_PVR_CTRL_BYPASS
    /// Record ENG1 bypass PID fliter
    E_DRVTSP_REC_MODE_ENG1_BYPASS     = 0x00010002UL,                                                 // TSP_PVR_CTRL_ENG(1) + TSP_PVR_CTRL_BYPASS

    E_DRVTSP_REC_MODE_ENG0_FLT1CA     = 0x00000004UL,
} DrvTSP_RecMode;

/// TSP Control Mode
typedef enum //_DrvTSP_CtrlMode
{
    /// Input From Memory
    E_DRVTSP_CTRL_MODE_MEM,
    /// Input From Stream Source 0
    E_DRVTSP_CTRL_MODE_TS0,
    /// Input From Stream Source 1
    E_DRVTSP_CTRL_MODE_TS1,
    /// Input From Stream Source 2
    E_DRVTSP_CTRL_MODE_TS2,
    /// Input From Stream Source 3
    E_DRVTSP_CTRL_MODE_TS3,
    /// Enable bounding option for PVR descrambled stream
    E_DRVTSP_CTRL_MODE_PVR_CA,
    /// Enable bounding option for PVR1 descrambled stream
    E_DRVTSP_CTRL_MODE_PVR1_CA,
    /// Input TSFI and source is Live
    E_DRVTSP_CTRL_MODE_TSFI_LIVE,
} DrvTSP_CtrlMode;

/// TSP input pad selection
typedef enum
{
    E_DRVTSP_PAD_EXT_INPUT0           = 0x0,
    E_DRVTSP_PAD_EXT_INPUT1           = 0x1,
    E_DRVTSP_PAD_EXT_INPUT2           = 0x2,
    E_DRVTSP_PAD_EXT_INPUT3           = 0x3,
    E_DRVTSP_PAD_EXT_INPUT4           = 0x4,
    E_DRVTSP_PAD_EXT_INPUT5           = 0x5,
    E_DRVTSP_PAD_TSO                  = 0x6,
    E_DRVTSP_PAD_DEMOD                = 0x7,
    E_DRVTSP_PAD_EXT_INPUT0_3WIRE     = 0x80,
    E_DRVTSP_PAD_EXT_INPUT1_3WIRE     = 0x81,
    E_DRVTSP_PAD_EXT_INPUT2_3WIRE     = 0x82,
    E_DRVTSP_PAD_EXT_INPUT3_3WIRE     = 0x83,
    E_DRVTSP_PAD_EXT_INPUT4_3WIRE     = 0x84,
    E_DRVTSP_PAD_EXT_INPUT5_3WIRE     = 0x85,
} DrvTSP_PadIn;

#define DrvTSP_PadOut                   MS_U32
#define E_DRVTSP_OUTPAD_NONE            0x00000000UL
#define E_DRVTSP_OUTPAD_PAD1            0x00000001UL
#define E_DRVTSP_OUTPAD_PAD3            0x00000003UL

#define DrvTSP_PadOut_Mode              MS_U32
#define E_DRVTSP_OUTPAD_MODE_MASK       0x70000000UL
#define E_DRVTSP_OUTPAD_FROM_DMD        0x00000000UL
#define E_DRVTSP_OUTPAD_FROM_S2P        0x10000000UL
#define E_DRVTSP_OUTPAD_FROM_TSO        0x20000000UL
#define E_DRVTSP_OUTPAD_FROM_S2P1       0x40000000UL


/// TSP interface
typedef enum
{
    E_DRVTSP_IF_TS0                   = 0x0,        //TSIF0
    E_DRVTSP_IF_TS1                   = 0x1,        //TSIF1
    E_DRVTSP_IF_TS2                   = 0x2,        //TSIF2
    E_DRVTSP_IF_TS3                   = 0x3,        //TSIF3
    E_DRVTSP_IF_NUM                   = 4,
    E_DRVTSP_IF_FI                    = 0x80,       //TSFI (version 3.0 New)

} DrvTSP_If;

typedef struct
{
    DrvTSP_PadIn        ePad;
    MS_BOOL             bClkInv;
    MS_BOOL             bExtSync;
    MS_BOOL             bParallel;
} DrvTSP_If_Set;

/// FQ interface
typedef enum
{
    E_DRVFQ_SRC_TSIF0                       = 0x00,
    E_DRVFQ_SRC_TSIF1                       = 0x01,
    E_DRVFQ_SRC_TSIF2                       = 0x02,
    E_DRVFQ_SRC_TSIF3                       = 0x03,

    E_DRVFQ_SRC_TSIFFI                      = 0x07,
    E_DRVFQ_SRC_DEFAULT                     = 0xFF,
} DrvFQ_SrcIf;

//----------------------------------
/// DMX debug table information structure
//----------------------------------
typedef struct
{
    DrvTSP_Debug_Cmd            TspCmd;
    DrvTSP_Debug_Src            TspSrc;
    DrvTSP_Debug_Fifo           TspFifo;
} DrvTSP_DisContiCnt_info, DrvTSP_DropPktCnt_info;

typedef struct
{
    DrvTSP_Debug_Cmd            TspCmd;
    DrvTSP_Debug_Tsif           TspTsif;
} DrvTSP_LockPktCnt_info;

typedef struct
{
    DrvTSP_Debug_Cmd            TspCmd;
    DrvTSP_Debug_Fifo           TspFifo;
    DrvTSP_Debug_Fifo_Src       TspFifoSrc;
} DrvTSP_AVPktCnt_info;

// must alignment with TSP_TSDMA_CTRL_XXX in regTSP.h
typedef enum
{
    /// TSP Engine 0 Transport Stream
    E_DRVTSP_FILEIN_MODE_ENG0_TS      = 0x00000000UL,                                                 //TSP_TSDMA_CTRL_ENG0
    /// TSP Engine 0 Video PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_VPES    = 0x00000004UL,                                                 //TSP_TSDMA_CTRL_VPES0
    /// TSP Engine 0 Audio PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_APES    = 0x00000008UL,                                                 //TSP_TSDMA_CTRL_APES0
    /// TSP Engine 0 Audio2 PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_A2PES   = 0x00000010UL,                                                 //TSP_TSDMA_CTRL_A2PES0
    /// TSP Engine 0 Video3D PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_V3DPES  = 0x00000020UL,                                                 //TSP_TSDMA_CTRL_V3DPES0
    /// TSP Engine 0 Video3D PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_A3PES   = 0x00000040UL,                                                 //TSP_TSDMA_CTRL_A3PES0
    /// TSP Engine 0 Video3D PES Only
    E_DRVTSP_FILEIN_MODE_ENG0_A4PES   = 0x00000080UL,                                                 //TSP_TSDMA_CTRL_A4PES0

} DrvTSP_FileinMode;


typedef enum
{
    /// Command Queue is Idle
    E_DRVTSP_FILEIN_STATE_IDLE        = 0000000000UL,
    /// Command Queue is Busy
    E_DRVTSP_FILEIN_STATE_BUSY        = 0x00000001UL,
    /// Command Queue is Paused.
    E_DRVTSP_FILEIN_STATE_PAUSE       = 0x00000002UL,
} DrvTSP_FileinState;

/// TSP TEI  Remove Error Packet Infomation
typedef enum
{
    E_TSP_DRV_TEI_REMOVE_AUDIO_PKT,          ///< DMX TEI Remoce Audio Packet
    E_TSP_DRV_TEI_REMOVE_VIDEO_PKT,          ///< DMX TEI Remoce Video Packet

    E_TSP_DRV_TEI_REMOVE_PKT0_LIVE,
    E_TSP_DRV_TEI_REMOVE_PKT0_FILE,
    E_TSP_DRV_TEI_REMOVE_PKT1,
    E_TSP_DRV_TEI_REMOVE_PKT2,

}TSP_DRV_TEI_RmPktType;

#if 0
/// TSP notification event
typedef enum //_DrvTSP_Event
{
    E_DRVTSP_EVENT_DATA_INIT          = 0x00000000,
    /// Section Data Ready
    E_DRVTSP_EVENT_DATA_READY         = 0x00000001,
    /// Section Buffer Overflow
    E_DRVTSP_EVENT_BUF_OVERFLOW       = 0x00000002,
    /// PVR Buffer is Full
    E_DRVTSP_EVENT_PVRBUF_FULL        = 0x00000010,
    /// PVR Double Buffer Overflow
    E_DRVTSP_EVENT_PVRBUF_OVERFLOW    = 0x00000020,


    /// TSP self task callback // optional --> default is CB by poll
    E_DRVTSP_EVENT_CB_MASK            = 0x80000000,
    E_DRVTSP_EVENT_CB_POLL            = 0x00000000,
    E_DRVTSP_EVENT_CB_SELF            = 0x80000000,
} DrvTSP_Event;
#endif

#define DrvTSP_Event                    MS_U32
#define E_DRVTSP_EVENT_DATA_INIT        0x00000000UL
/// Section Data Ready
#define E_DRVTSP_EVENT_DATA_READY       0x00000001UL
/// Section Buffer Overflow
#define E_DRVTSP_EVENT_BUF_OVERFLOW     0x00000002UL
/// Section Buffer CRC error
#define E_DRVTSP_EVENT_SEC_CRCERR       0x00000004UL
/// PVR Buffer is Full
#define E_DRVTSP_EVENT_PVRBUF_FULL      0x00000010UL
/// PVR Double Buffer Overflow
#define E_DRVTSP_EVENT_PVRBUF_OVERFLOW  0x00000020UL
/// Channel Browser Buffer is Full
#define E_DRVTSP_EVENT_CBBUF_FULL       0x00000040UL
#define E_DRVTSP_EVENT_CBBUF_OVERFLOW   0x00000080UL
///PVR2 Buffer is Full
#define E_DRVTSP_EVENT_PVR2BUF_FULL     0x00000100UL
#define E_DRVTSP_EVENT_PVR2BUF_OVERFLOW 0x00000200UL


/// TSP self task callback // optional --> default is CB by poll
#define E_DRVTSP_EVENT_CB_MASK          0x80000000UL
#define E_DRVTSP_EVENT_CB_POLL          0x00000000UL
#define E_DRVTSP_EVENT_CB_SELF          0x80000000UL

/// TSP file in Packet mode
typedef enum //_DrvTSP_PacketMode
{
    E_DRVTSP_PKTMODE_188              = 0x00000000UL,
    E_DRVTSP_PKTMODE_192              = 0x00000001UL,
    E_DRVTSP_PKTMODE_204              = 0x00000002UL,
    E_DRVTSP_PKTMODE_130              = 0x00000003UL,    // RVU
    E_DRVTSP_PKTMODE_134              = 0x00000004UL,    // RVU with timestamp
} DrvTSP_PacketMode;

/// TSP notification event message
typedef struct //_DrvTSP_Msg
{
    /// Union data type of message
    union
    {
        /// FltInfo message
        ///   - Byte[0] : Section filter id
        ///   - Byte[1] : TSP id
        MS_U32                          FltInfo;
        /// PvrBufId
        ///   - Byte[0] : PVR buffer id
        ///   - Byte[1] : PVR engine id
        MS_U32                          PvrBufId;
    };

} DrvTSP_Msg;

typedef enum //_DrvTSP_DbgLevel
{
    E_DRVTSP_DBG_Release = 0,
    E_DRVTSP_DBG_L1, // display error msg
    E_DRVTSP_DBG_L2, // display error msg and enter while(1)
} DrvTSP_DbgLevel;

/// @name DrvTSP_Msg
/// Macro definitions for manipulating DrvTSP_Msg
/// @{
#define MSG_FLTINFO_SEC_ID_MASK         0x000000FFUL
#define MSG_FLTINFO_SEC_ID_SHFT         0UL
#define MSG_FLTINFO_ENG_ID_MASK         0x0000FF00UL
#define MSG_FLTINFO_ENG_ID_SHFT         8UL
#define MSG_PVRBUF_ID_MASK              0x000000FFUL
#define MSG_PVRBUF_ID_SHFT              0UL
#define MSG_PVRBUF_ID_NULL              0xFFUL
#define MSG_PVR_ID_MASK                 0x0000FF00UL
#define MSG_PVR_ID_SHFT                 8UL

/// @}

#define TSP_MAX_SIGFLAG_NUM             10UL
#define TSP_MAX_PVRSIGFLAG_NUM          10UL
typedef struct DLL_PACKED
{
    MS_U32                 u32Eng;
    MS_U32                 u32EvtWaitOpt;
    MS_U32                 u32DmxEvt;
    MS_U32                 u32TspEvt;

    MS_U32                 u32PvrBufFullFlag[TSP_MAX_PVRSIGFLAG_NUM];

    MS_U32                 u32SecRdyFlag[TSP_MAX_SIGFLAG_NUM];
    MS_U32                 u32SecOvfFlag[TSP_MAX_SIGFLAG_NUM];
    MS_U32                 u32SecCrcErrFlag[TSP_MAX_SIGFLAG_NUM];

    MS_S32                 s32KerModeTspEvtId; //For kernel mode, every user callback process must has itself eventid in multiple process support
    MS_U32                 u32TblIndex;
}DrvTSP_IoSignal;


typedef enum
{
    E_DRVTSP_CAP_PID_FILTER_NUM             = 0,    // Get filter number
    E_DRVTSP_CAP_SEC_FILTER_NUM             = 1,
    E_DRVTSP_CAP_SEC_BUF_NUM                = 2,

    E_DRVTSP_CAP_PVR_ENG_NUM                = 3,
    E_DRVTSP_CAP_PVR_FILTER_NUM             = 4,   // Get pvr filter number
    E_DRVTSP_CAP_PVR1_FILTER_NUM            = 5,   // Get pvr1 filter number

    E_DRVTSP_CAP_MMFI0_FILTER_NUM           = 6,   // Get MMFI0 filter number
    E_DRVTSP_CAP_MMFI1_FILTER_NUM           = 7,   // Get MMFI1 filter number

    E_DRVTSP_CAP_TSIF_NUM                   = 8,
    E_DRVTSP_CAP_DEMOD_NUM                  = 9,
    E_DRVTSP_CAP_TS_PAD_NUM                 = 10,
    E_DRVTSP_CAP_VQ_NUM                     = 11,

    E_DRVTSP_CAP_CA_FLT_NUM                 = 12,  // Get dscmb filter number
    E_DRVTSP_CAP_CA_KEY_NUM                 = 13,

    E_DRVTSP_CAP_FW_ALIGN                   = 14,
    E_DRVTSP_CAP_VQ_ALIGN                   = 15,
    E_DRVTSP_CAP_VQ_PITCH                   = 16,
    E_DRVTSP_CAP_SEC_BUF_ALIGN              = 17,

    E_DRVTSP_CAP_PVR_ALIGN                  = 18,
    E_DRVTSP_CAP_PVRCA_PATH_NUM             = 19,
    E_DRVTSP_CAP_SHAREKEY_FLT_RANGE         = 20,
    E_DRVTSP_CAP_PVRCA0_FLT_RANGE           = 21,
    E_DRVTSP_CAP_PVRCA1_FLT_RANGE           = 22,
    E_DRVTSP_CAP_PVRCA2_FLT_RANGE           = 23,
    E_DRVTSP_CAP_SHAREKEY_FLT1_RANGE        = 24,
    E_DRVTSP_CAP_SHAREKEY_FLT2_RANGE        = 25,

    E_DRVTSP_CAP_HW_TYPE                    = 26,

    E_DRVTSP_CAP_RESOURCE_SIZE              = 27,       // Get the data structure size of private resource (share resource)

    E_DRVTSP_CAP_VFIFO_NUM                  = 28,       // Get VFIFO support number
    E_DRVTSP_CAP_AFIFO_NUM                  = 29,       // Get AFIFO support number

    E_DRVTSP_CAP_HWPCR_SUPPORT              = 30,       // Get HWPCR support status
    E_DRVTSP_CAP_PCRFLT_START_IDX           = 31,       // Get PCR start index

    E_DRVTSP_CAP_HWWP_SET_NUM               = 32,       // Get TSP write protect set numbers

    E_DRVTSP_CAP_DSCMB_ENG_NUM              = 33,       // Get DSCMB engine numbers

    E_DRVTSP_CAP_MAX_MERGESTR_NUM           = 34,       // Get Maxumum merge stream number
    E_DRVTSP_CAP_MAX_SEC_FLT_DEPTH          = 35,
    E_DRVTSP_CAP_FW_BUF_SIZE                = 36,
    E_DRVTSP_CAP_FW_BUF_RANGE               = 37,
    E_DRVTSP_CAP_VQ_BUF_RANGE               = 38,
    E_DRVTSP_CAP_SEC_BUF_RANGE              = 39,
    E_DRVTSP_CAP_FIQ_NUM                    = 40,
    E_DRVTSP_CAP_NULL,
} DrvTSP_Cap;

typedef enum
{
    E_DRVTSP_HW_STATUS = 0,
    E_DRVTSP_HW_INIT,
    E_DRVTSP_HW_ALIVE,
    E_DRVTSP_HW_DEALIVE,
} DrvTSP_HW_Status;

typedef enum
{
    E_DRVTSP_SW_INIT = 0,
    E_DRVTSP_SW_INIT_FAIL,
    E_DRVTSP_SW_SUSPEND,
    E_DRVTSP_SW_RESUME,
    E_DRVTSP_SW_POWEROFF,
    E_DRVTSP_SW_CLOSE,
    E_DRVTSP_SW_EXIT,
} DrvTSP_SW_Status;

// legacy
typedef enum{
    E_DRVTSP_SCMB_NONE                = 0x00000000UL,
    E_DRVTSP_SCMB_TS                  = 0x00000001UL,
    E_DRVTSP_SCMB_PES                 = 0x00000002UL,
    E_DRVTSP_SCMB_TS_PES              = (E_DRVTSP_SCMB_TS| E_DRVTSP_SCMB_PES),
} DrvTSP_Scmb_Level;

//----------------
//For Tee
//----------------
#ifdef SECURE_PVR_ENABLE
typedef enum
{
    E_DRVTSP_REE_TO_TEE_CMD_NULL,
    E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode,                //None parameters
    E_DRVTSP_REE_TO_TEE_CMD_SET_VqBuf,                  //None parameters
    E_DRVTSP_REE_TO_TEE_CMD_SET_PvrBuf,                 //Param1: Engine id, Param2: Option 1 is reset buffer to 0
    E_DRVTSP_REE_TO_TEE_CMD_SET_FileinBuf,              //Param1: Engine id, Param2: Buf address, Param3: Buf size
    E_DRVTSP_REE_TO_TEE_CMD_GET_PvrWPtr,                //Param1: Engine id, Param2: Return Buf address
    E_DRVTSP_REE_TO_TEE_CMD_SET_MMFIBuf                 //Param1: Engine id, Param2: Return Buf address
} TSP_REE_TO_TEE_CMD_TYPE;

typedef struct DLL_PACKED
{
    MS_U32 u32BufId;
    MS_U32 u32BufOpt;
    MS_U32 u32BufSize;
    MS_PHY phyBufAddr;
}DrvTSP_SecureBuf;

#endif  //SECURE_PVR_ENABLE

// TSP event define
#define TSP_TASK_EVENT_SECTION_SELF         0x00000001UL
#define TSP_TASK_EVENT_SECTION_POLL         0x00000002UL
#define TSP_TASK_EVENT_PVR0_RDY_SELF        0x00000004UL //PVR1 buffer0
#define TSP_TASK_EVENT_PVR1_RDY_SELF        0x00000008UL //PVR1 buffer0
#define TSP_TASK_EVENT_PVR0_RDY_POLL        0x00000010UL //PVR1 buffer0
#define TSP_TASK_EVENT_PVR1_RDY_POLL        0x00000020UL //PVR1 buffer0
#define TSP_TASK_EVENT_FLT_FREE             0x00000040UL
#define TSP_TASK_EVENT_CBPVR0_RDY_SELF      0x00000080UL
#define TSP_TASK_EVENT_CBPVR0_RDY_POLL      0x00000100UL
#define TSP_TASK_EVENT_FWMSG                0x00000200UL
#define TSP_TASK_EVENT_PVR2PVR0_RDY_SELF    0x00000400UL //PVR2 buffer0
#define TSP_TASK_EVENT_PVR2PVR1_RDY_SELF    0x00000800UL //PVR2 buffer1
#define TSP_TASK_EVENT_PVR2PVR0_RDY_POLL    0x00001000UL //PVR2 buffer0
#define TSP_TASK_EVENT_PVR2PVR1_RDY_POLL    0x00002000UL //PVR2 buffer1

#define TSP_TASK_EVENT_PVR_SELF       (     TSP_TASK_EVENT_PVR0_RDY_SELF    |   \
                                            TSP_TASK_EVENT_PVR1_RDY_SELF    |   \
                                            TSP_TASK_EVENT_CBPVR0_RDY_SELF  |   \
                                            TSP_TASK_EVENT_PVR2PVR0_RDY_SELF|   \
                                            TSP_TASK_EVENT_PVR2PVR1_RDY_SELF)
#define TSP_TASK_EVENT_PVR_POLL       (     TSP_TASK_EVENT_PVR0_RDY_POLL    |   \
                                            TSP_TASK_EVENT_PVR1_RDY_POLL    |   \
                                            TSP_TASK_EVENT_CBPVR0_RDY_POLL  |   \
                                            TSP_TASK_EVENT_PVR2PVR0_RDY_POLL|   \
                                            TSP_TASK_EVENT_PVR2PVR1_RDY_POLL)


#define TSP_TASK_EVENT_GROUP_SELF       (   TSP_TASK_EVENT_SECTION_SELF     |   \
                                            TSP_TASK_EVENT_PVR0_RDY_SELF    |   \
                                            TSP_TASK_EVENT_PVR1_RDY_SELF    |   \
                                            TSP_TASK_EVENT_CBPVR0_RDY_SELF  |   \
                                            TSP_TASK_EVENT_PVR2PVR0_RDY_SELF|   \
                                            TSP_TASK_EVENT_PVR2PVR1_RDY_SELF)
#define TSP_TASK_EVENT_GROUP_POLL       (   TSP_TASK_EVENT_SECTION_POLL     |   \
                                            TSP_TASK_EVENT_PVR0_RDY_POLL    |   \
                                            TSP_TASK_EVENT_PVR1_RDY_POLL    |   \
                                            TSP_TASK_EVENT_CBPVR0_RDY_POLL  |   \
                                            TSP_TASK_EVENT_PVR2PVR0_RDY_POLL|   \
                                            TSP_TASK_EVENT_PVR2PVR1_RDY_POLL)

/// TSP notification function
typedef void (*P_DrvTSP_EvtCallback)(DrvTSP_Event eEvent, DrvTSP_Msg *pMsg);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

// General API
TSP_Result MDrv_TSP_InitLibResource(void *pResMemAddr);
TSP_Result MDrv_TSP_Init(MS_PHY u32FWAddr, MS_U32 u32FWSize);                                       // Set FW address and size
TSP_Result MDrv_TSP_Reinit(MS_BOOL bHK);
TSP_Result MDrv_TSP_PowerOff(void);
TSP_Result MDrv_TSP_Exit(void);
TSP_Result MDrv_TSP_Suspend(void);
TSP_Result MDrv_TSP_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize);
TSP_Result MDrv_TSP_Resume_Filter(void);
TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId);
TSP_Result MDrv_TSP_Reset(void);
TSP_Result MDrv_TSP_HW_Lock_Release(void);
TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable);
TSP_Result MDrv_TSP_RemoveDupAVFifoPkt(DrvTSP_FltType flttype, MS_BOOL bEnable);
TSP_Result MDrv_TSP_TEI_RemoveErrorPkt(TSP_DRV_TEI_RmPktType eDrvPktType, MS_BOOL bEnable);
TSP_Result MDrv_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable);
TSP_Result MDrv_TSP_DropScmbPkt(DrvTSP_FltType eFilterType, MS_BOOL bEnable);

// Misc API
TSP_Result MDrv_TSP_SetFwDBGParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord);           // for TSP f/w debug
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, DrvTSP_CtrlMode eCtrlMode, MS_U8 u8DscmbEngSel); // Set TSP internal operate
TSP_Result MDrv_TSP_SelPad(MS_U32 u32EngId, DrvTSP_If eIf, DrvTSP_If_Set* pIfSet);                  // Set TSP input pad and ts0/ts1
TSP_Result MDrv_TSP_OutputPadCfg(DrvTSP_PadOut eOutputPad, DrvTSP_PadIn eInputPad, MS_BOOL bInParallel, MS_BOOL bEnable);
TSP_Result MDrv_TSP_OutputClkPhase(MS_U16 u16OutPad, MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt);
TSP_Result MDrv_TSP_AVFifo_Reset(DrvTSP_FltType eFilterType, MS_BOOL bFlush);
TSP_Result MDrv_TSP_AVFifo_Status(DrvTSP_FltType eFilterType, MS_U32 *u32FifoLevel);
TSP_Result MDrv_TSP_AVFifo_BlockEnable(DrvTSP_FltType eFilterType, MS_BOOL bEnable);
TSP_Result MDrv_TSP_AVFifo_SourceSelect(DrvTSP_PKTDMXSrcType ePKTSrc, MS_U32 u32FifoType);
TSP_Result MDrv_TSP_Parl_BitOrderSwap(MS_U32 u32EngId, DrvTSP_If eIf, MS_BOOL bInvert);
TSP_Result MDrv_TSP_Get_FW_VER(MS_U32* u32FWVer);
TSP_Result MDrv_TSP_Get_Intr_Count(MS_U32* pu32Count);
TSP_Result MDrv_TSP_Scmb_Status(MS_U32 u32EngId, DrvTSP_Scmb_Level* pScmbLevel);
TSP_Result MDrv_TSP_GetLastErr(void);
TSP_Result MDrv_TSP_Proc(MS_U32 u32EngId, MS_U32 u32FltId, TSP_Result* pRet, DrvTSP_Event* pEvt);   // for non-OS TSP scheduling
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC);
TSP_Result MDrv_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC);
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr);
TSP_Result MDrv_TSP_Flt_GetState(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState);
TSP_Result MDrv_TSP_GetDscmbFltId_Range(MS_U32 u32EngId, MS_U32 *pu32StartId, MS_U32 *pu32EndId);
TSP_Result MDrv_TSP_GetDscmbEngIdx_BySource(DrvTSP_PKTDMXSrcType eInputSrc, MS_U32* pu32EngId);
TSP_Result MDrv_TSP_Dscmb_Source(MS_U32 u32EngId, DrvTSP_PKTDMXSrcType* peInputSrc, MS_BOOL bSet);
TSP_Result MDrv_TSP_STC64_Mode_Enable(MS_BOOL bEnable);
TSP_Result MDrv_TSP_STCClk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage);


// NDS --> ECM
TSP_Result MDrv_TSP_SecFlt_SetEcmIdx(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32EcmIdx);

// PIDFlt API
TSP_Result MDrv_TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId);
TSP_Result MDrv_TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId);
TSP_Result MDrv_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U16 u16PID);
TSP_Result MDrv_TSP_PidFlt_GetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32* pu32PID);
TSP_Result MDrv_TSP_PidFlt_SelSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SecFltId);
TSP_Result MDrv_TSP_PidFlt_SetInputSrc(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltType eFltSrc);
TSP_Result MDrv_TSP_PidFlt_Enable(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bEnable);
TSP_Result MDrv_TSP_PidFlt_GetState(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState);
TSP_Result MDrv_TSP_PidFlt_ChangeSource(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltType eFilterType);
TSP_Result MDrv_TSP_PidFlt_GetScmbSts(DrvTSP_FltType FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts);
TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(DrvTSP_FltType eFltSrcType);
TSP_Result MDrv_TSP_PidFlt_SetFltRushPass(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U8 u8Enable);

// SecFlt API
TSP_Result MDrv_TSP_SecFlt_Alloc(MS_U32 u32EngId, MS_U32 *pu32SecFltId);
TSP_Result MDrv_TSP_SecFlt_Free(MS_U32 u32EngId, MS_U32 u32SecFltId);
TSP_Result MDrv_TSP_SecFlt_SetMode(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltMode eSecFltMode);
TSP_Result MDrv_TSP_SecFlt_SetPattern(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NotMask);
TSP_Result MDrv_TSP_SecFlt_ResetBuffer(MS_U32 u32EngId, MS_U32 u32SecFltId);
TSP_Result MDrv_TSP_SecFlt_SetBuffer(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY phyStartAddr, MS_U32 u32BufSize);
TSP_Result MDrv_TSP_SecFlt_SetReqCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32ReqCount);
TSP_Result MDrv_TSP_SecFlt_GetBufStart(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyBufStart);
TSP_Result MDrv_TSP_SecFlt_GetBufSize(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 *pu32BufSize);
TSP_Result MDrv_TSP_SecFlt_GetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyReadAddr);
TSP_Result MDrv_TSP_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyWriteAddr);

TSP_Result MDrv_TSP_SecFlt_SetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY phyReadAddr);
TSP_Result MDrv_TSP_SecFlt_SetDataAddr(MS_U32 u32DataAddr);
TSP_Result MDrv_TSP_SecFlt_Notify(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback);
TSP_Result MDrv_TSP_SecFlt_GetState(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltState *peState);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
TSP_Result MDrv_TSP_SecFlt_GetPattern(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NotMask);
#endif //CONFIG_UTOPIA_PROC_DBG_SUPPORT

// PVR API
TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
TSP_Result MDrv_TSP_PVR_Eng_Start(MS_U8 u8PVRId, DrvTSP_RecMode eRecMode, MS_BOOL bStart);
TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U8 u8PVRId, MS_PHY *pphy2WriteAddr);
TSP_Result MDrv_TSP_PVR_Eng_Notify(MS_U8 u8PVRId, DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback);
TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U8 u8PVRId, MS_BOOL bSet);
TSP_Result MDrv_TSP_PVR_Eng_SelSrc(MS_U8 u8PVRId, DrvTSP_PKTDMXSrcType ePktSrc);
TSP_Result MDrv_TSP_PVR_BlockEnable(MS_U8 u8PVRId, MS_BOOL bEnable);
TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U8 u8PVRId, MS_U32 u32Stamp);
TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U8 u8PVRId,MS_U32* u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp);
TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable);

TSP_Result MDrv_TSP_PVR_Eng_SetPlaybackStampClk(MS_U32 u32EngId, MS_U32 u32Clk);
TSP_Result MDrv_TSP_PVR_Eng_SetRecordStampClk(MS_U32 u32PvrId, MS_U32 u32Clk);
TSP_Result MDrv_TSP_PVR_Eng_IsStart(MS_U32 u32PvrId, MS_BOOL *pbIsStart);

TSP_Result MDrv_TSP_PVRCA_Close(MS_U8 u8PVRId);


#define MDrv_TSP_PVR_Notify(eEvents, pfCallback)    MDrv_TSP_PVR_Eng_Notify(0, eEvents, pfCallback)

// File API
TSP_Result MDrv_TSP_File_SetAddr(MS_PHY phyStreamAddr);
TSP_Result MDrv_TSP_File_SetSize(MS_U32 u32StreamSize);
TSP_Result MDrv_TSP_File_Start(DrvTSP_FileinMode eFileinMode);
TSP_Result MDrv_TSP_File_Stop(void);
TSP_Result MDrv_TSP_File_Pause(void);
TSP_Result MDrv_TSP_File_Resume(void);
TSP_Result MDrv_TSP_File_GetState(DrvTSP_FileinState *peFileinState);
TSP_Result MDrv_TSP_File_SetRate(MS_U32 u32Div2);
TSP_Result MDrv_TSP_File_SetPacketMode(DrvTSP_PacketMode PKT_Mode);
TSP_Result MDrv_TSP_File_CMDQ_GetSlot(MS_U32 *pu32EmptySlot);
TSP_Result MDrv_TSP_File_CMDQ_Reset(void);
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel);
TSP_Result MDrv_TSP_ResetFileinTimestamp(void);
TSP_Result MDrv_TSP_File_GetReadAddr(MS_PHY *pphyReadAddr);
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable);
TSP_Result MDrv_TSP_File_PS_Path_Enable(DrvTSP_FileinMode eFileinMode);
TSP_Result MDrv_TSP_File_TSIFSrcSel(DrvTSP_FltType eFltSrc, MS_BOOL bFileMode);

// Capacity query
TSP_Result MDrv_TSP_GetCap(DrvTSP_Cap eCap, void* pOutput);
TSP_Result MDrv_TSP_GetStatus(DrvTSP_HW_Status *HW_Status, DrvTSP_SW_Status *SW_Status);
TSP_Result MDrv_TSP_GetLibVer(const MSIF_Version **ppVersion);
TSP_Result MDrv_TSP_SetDbgLevel(DrvTSP_DbgLevel DbgLevel);
TSP_Result MDrv_TSP_GetTSIFStatus(DrvTSP_If eIf, DrvTSP_If_Set* pIfSet, MS_U16* pu16Clk);

//-------------------------------------------------------------------------------------------------
//  OBSOLETE
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_SetOwner(MS_U32 u32EngId, MS_U32 u32FltIdStart, MS_U32 u32FltIdEnd, MS_BOOL bOwner); // Set/Get the ranges of filters used, which is for AEON/MHEG5 share TSP filters resource.
TSP_Result MDrv_TSP_PidFlt_ChkOwner(MS_U32 u32EngId, MS_U32 u32PidFltId);
TSP_Result MDrv_TSP_SetHK(MS_BOOL bIsHK);
TSP_Result MDrv_TSP_IsAccess(MS_U32 u32Try);
TSP_Result MDrv_TSP_UnlockAccess(void);

TSP_Result MDrv_TSP_TTX_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyWriteAddr); // special case for TTX
TSP_Result MDrv_TSP_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo);
TSP_Result MDrv_TSP_BurstLen(MS_U32 u32Len);
TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen);
#if 0
TSP_Result MDrv_TSP_ResetFileinTimestamp(void);
#endif

TSP_Result MDrv_TSP_SetVQueBuf(MS_PHY phyVQBaseAddr, MS_U32 u32VQBufLen);
TSP_Result MDrv_TSP_VQueEnable(MS_BOOL bEnable);
TSP_Result MDrv_TSP_VQueReset(MS_U8 u8VQID);
TSP_Result MDrv_TSP_VQueue_OverflowInt_En(MS_BOOL bEnable);
TSP_Result MDrv_TSP_Set_VQ_Threshold(MS_U8 u8req_len);
TSP_Result MDrv_TSP_Get_VQStatus(MS_U32* pu32Status);

TSP_Result MDrv_TSP_Set_MOBF_Set(MS_U8 u8Model, MS_U8 u8MobfIndex0, MS_U8 u8MobfIndex1);

void       MDrv_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr);
void       MDrv_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);
TSP_Result MDrv_TSP_TSIF_Enable(DrvTSP_If tsif, MS_BOOL bEnable);
TSP_Result MDrv_TSP_ReadDropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);

#define MDrv_TSP_PVR_SetBuffer(p1, p2...)                   MDrv_TSP_PVR_Eng_SetBuffer(0, p1, p2)
#define MDrv_TSP_PVR_Start(p1, p2...)                       MDrv_TSP_PVR_Eng_Start(0, p1, p2)
#define MDrv_TSP_PVR_GetWriteAddr(p1)                       MDrv_TSP_PVR_Eng_GetWriteAddr(0, p1)
#define MDrv_TSP_PVR_SetPacketMode(p1 )                     MDrv_TSP_PVR_Eng_SetPacketMode(0, p1)
#define MDrv_TSP_PVR_TimeStampSetRecordStamp(p1)            MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(0, p1)
#define MDrv_TSP_PVR_TimeStampGetRecordStamp(p1 )           MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(0, p1)

//---------- Common Interface for TSP config and Api commands ------------//
TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData);
#define DRVTSP_CMD_SET_LIB_MODE           0x80000003UL  //u32Config0: 0 ->  user mode, 1 ->  Kernel mode with user cb
    #define DRVTSP_CMD_LIB_MODE_USER      0x00000000UL
    #define DRVTSP_CMD_LIB_MODE_KRN       0x00000001UL
#define DRVTSP_CMD_STC_ADJUST_UNIT        0x00000020UL //u32Config0: data
#define TSP_CMD_PVR_PES_MODE              0x00000030UL //u32Config0: PVR EngineID, u32Config1=1, *pData = 1 is enable, pData = 0 is disable

TSP_Result MDrv_TSP_GetPesScmbSts(MS_U32 u32FltId, MS_U8 *pu8status);
TSP_Result MDrv_TSP_GetTsScmbSts(MS_U32 u32FltId, MS_U8 *pu8status);

TSP_Result MDrv_TSP_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32Size);

TSP_Result MDrv_TSP_FQ_FltNullPkt(MS_U32 u32FQEng, MS_BOOL bFltNull);
TSP_Result MDrv_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf eTsSrc);
TSP_Result MDrv_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf* peTsSrc);

TSP_Result MDrv_TSP_STC_Alloc(MS_U32 *pu32EngId);
TSP_Result MDrv_TSP_STC_Free(MS_U32 u32EngId);
TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Mode);
TSP_Result MDrv_TSP_STC_Select(DrvTSP_FltType eFltSrc, MS_U32 u32StcEng);
TSP_Result MDrv_TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId);
TSP_Result MDrv_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd);

//----------- Debug table --------------
TSP_Result MDrv_TSP_Get_DisContiCnt(DrvTSP_DisContiCnt_info* tspInfo, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_DropPktCnt(DrvTSP_DropPktCnt_info* tspInfo, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_LockPktCnt(DrvTSP_LockPktCnt_info* tspInfo, MS_U32* pu32Cnt);
TSP_Result MDrv_TSP_Get_AVPktCnt(DrvTSP_AVPktCnt_info* tspInfo, MS_U32* pu32Cnt);

TSP_Result MDrv_TSP_Get_SecTEI_PktCount(DrvTSP_FltType FltSrc, MS_U32* pu32PktCnt);
TSP_Result MDrv_TSP_Reset_SecTEI_PktCount(DrvTSP_FltType FltSrc);
TSP_Result MDrv_TSP_Get_SecDisCont_PktCount(MS_U32 u32FltID, MS_U32* pu32PktCnt);
TSP_Result MDrv_TSP_Reset_SecDisCont_PktCount(MS_U32 u32FltID);

//-------- merge stream --------------------------
TSP_Result MDrv_TSP_MStr_SyncByte(DrvTSP_If eIf, MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet);

//------- For 2K kernel mode, user mode callback thread using events
TSP_Result MDrv_TSP_Create_IoSignal(DrvTSP_IoSignal *pstIoSignal);
TSP_Result MDrv_TSP_Close_IoSignal(DrvTSP_IoSignal *pstIoSignal);
TSP_Result MDrv_TSP_Wait_IoSignal(DrvTSP_IoSignal *pstIoSignal);
TSP_Result MDrv_TSP_Proc_IoSignal(DrvTSP_IoSignal *pstIoSignal);
TSP_Result MDrv_TSP_Get_SecEvents(MS_U32 u32Group, DrvTSP_IoSignal *pstIoSignal);

#ifdef SECURE_PVR_ENABLE
TSP_Result MDrv_TSP_Ree_TeeCmdSystem_Init(void);
TSP_Result MDrv_TSP_Ree_TeeCmdSystem_Exit(void);
TSP_Result MDrv_TSP_Ree_SendTeeCmd(TSP_REE_TO_TEE_CMD_TYPE cmd_type, void* param, size_t datasize);
#endif   //SECURE_PVR_ENABLE

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRV_TSP_H_
