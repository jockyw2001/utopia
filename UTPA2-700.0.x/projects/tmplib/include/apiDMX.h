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
/// @file   apiDMX.h
/// @brief  Demux API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX DMX interface 

  TSP (Transport Stream Processor) receives transport stream through serial or parallel interface.
  The input stream can be filtered by PID and masked pattern to get individual section data streams that are qualified with CRC and stored into the ring buffers located in external memory.
  Besides section filter, audio PES FIFO and video PES FIFO are used to pass the selected Audio/Video PES packet to audio/video decoder.
  PVR function is also supported.

  In order to parse section data in TS stream, TSP equipment a 32-bit RISC processor, besides, The PID filter, header compare, CRC, DMA and PVR are implemented with dedicated hardware.

  <b> Ownership of Filters </b> \n
  Not only one processor will use Demux driver at the same time, all the filters can be grouped into different processor by API MApi_DMX_SetOwner.
   \image html apiDMX_pic1.png

   <b> Filter Life Cycle </b> \n
   \image html apiDMX_pic2.png

  <b> API call flow for initiation: </b> \n
   \image html apiDMX_pic3.png

  <b> API call flow for exit: </b> \n
   \image html apiDMX_pic4.png

  <b> API call flow for general usage - Live in playback: </b> \n
   \image html apiDMX_pic5.png

  <b> API call flow for general usage - File in playback: </b> \n
   \image html apiDMX_pic6.png

  <b> API call flow for general usage - PVR/PVRCA: </b> \n
   \image html apiDMX_pic7.png

  <b> API call flow for general usage - MMFI Audio/MMFI Video3D: </b> \n
   \image html apiDMX_pic8.png

  *! \defgroup TSP TSP related functions
  *   \ingroup   DMX

  *! \defgroup TSP_General  TSP General functions
  *   \ingroup   TSP

  *! \defgroup TSP_Flow  TSP Flow control functions
  *   \ingroup   TSP

  *! \defgroup TSP_STC TSP STC/PCR functions
  *   \ingroup   TSP

  *! \defgroup TSP_Filter TSP Basic filter control functions
  *   \ingroup   TSP

  *! \defgroup TSP_SecFilter TSP Section filter control functions
  *   \ingroup   TSP

  *! \defgroup TSP_TTX TSP Teltext related functions
  *   \ingroup   TSP

  *! \defgroup TSP_FIFO TSP AVFIFO functions
  *   \ingroup   TSP

  *! \defgroup TSP_PVR TSP PVR (record) related functions
  *   \ingroup   TSP

  *! \defgroup TSP_PVR_General TSP PVR general  functions
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_PVR_CA TSP PVR CA  functions (need decrypt)
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_FILEIN_PVR TSP file-in record related functions
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_PVR_STB TSP PVR related functions for STB
  *   \ingroup   TSP_PVR

  *! \defgroup TSP_Playback TSP playback (file-in) related functions
  *   \ingroup   TSP

  *! \defgroup TSP_SCMB TSP Scramble status check functions
  *   \ingroup   TSP

  *! \defgroup TSP_STB TSP functions which only used on STB chip
  *   \ingroup   TSP

  *! \defgroup TSP_Debug TSP Debug functions
  *   \ingroup   TSP

  *! \defgroup TSP_FIQ TSP FIQ related functions
  *   \ingroup   TSP

  *! \defgroup TSP_Merge TSP merge stream related functions for CI+ 1.4 used
  *   \ingroup   TSP

  *! \defgroup TSP_MM TSP MM (Multi-Media) related functions
  *   \ingroup   TSP

  *! \defgroup TSO TSO related functions
  *   \ingroup   DMX

  *! \defgroup TSO_General TSO general functions
  *   \ingroup   TSO

  *! \defgroup DMX_STR DMX STR functions
  *   \ingroup   DMX

  *! \defgroup DMX_ToBeModified DMX api to be modified
  *   \ingroup   DMX

  *! \defgroup DMX_ToBeModified_TSP TSP api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeModified_MM MM api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeModified_TSO TSO api to be modified
  *   \ingroup   DMX_ToBeModified

  *! \defgroup DMX_ToBeRemove DMX api to be removed
  *   \ingroup   DMX

  */
//-------------------------------------------------

#ifndef __API_DMX_H__
#define __API_DMX_H__

#include "MsCommon.h"

#include "ULog.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// NULL PID definition
#define DMX_PID_NULL               (0x1FFFUL)

// Please don't change this value if you know exactly what you are doing
/// DMX Filter pattern depth
#define DMX_SECT_FLT_DEPTH         (16UL)

/// NULL DMX Id
#define DMX_DMXID_NULL             ((MS_U8)0xFFUL)
#define MSIF_DMX_LIB_CODE                     {'D','M','X','_'}    //Lib code
#define MSIF_DMX_LIBVER                       {'1','5'}            //LIB version
#define MSIF_DMX_BUILDNUM                     {'0','3'}            //Build Number
#define MSIF_DMX_CHANGELIST                   {'0','0','6','1','7','4','7','0'} //P4 ChangeList Number

/// DMX Version
#define DMX_API_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DMX_LIB_CODE,                  /* IP__                                             */  \
    MSIF_DMX_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_DMX_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_DMX_CHANGELIST,  /* CL#                                              */  \
    MSIF_OS

/// DMX status indicator
typedef enum
{
    DMX_FILTER_STATUS_OK,               ///< DMX status OK
    DMX_FILTER_STATUS_ERROR,            ///< DMX status ERROR
} DMX_FILTER_STATUS;

typedef enum
{
    DMX_DBG_NONE,
    DMX_DBG_ERR,
    DMX_DBG_WARN,
    DMX_DBG_INFO,
    DMX_DBG_PVR,
    DMX_DBG_FUNC,
} DMX_DBGMSG_LEVEL;

/// DMX internal startup parameter
typedef struct _DMX_FW_Param
{
    MS_PHY                            phyFWAddr;
    MS_U32                              u32FWSize;

    MS_PHY                            phyVQAddr;
    MS_U32                              u32VQSize;

    MS_U32                              u32IsHK;
    MS_U32                              u32Res;

    MS_U32                              _zero_reserved[2];

} DMX_TSPParam;


#if 0
/// DMX fitler type and source type
typedef enum
{
    DMX_FILTER_TYPE_VIDEO,              ///< DMX video filter type
    DMX_FILTER_TYPE_AUDIO,              ///< DMX audio filter type
    DMX_FILTER_TYPE_AUDIO2,             ///< DMX audio2 filter type
    DMX_FILTER_TYPE_SECTION,            ///< DMX section filter type
    DMX_FILTER_TYPE_PES,                ///< DMX PES filter type
    DMX_FILTER_TYPE_PACKET,             ///< DMX TS packet filter type
    DMX_FILTER_TYPE_PCR,                ///< DMX PCR filter type
    DMX_FILTER_TYPE_TELETEXT,           ///< DMX teletext filter type

    DMX_FILTER_SOURCE_TYPE_MASK  =  0xC0000000,              ///< DMX filter source mask
    DMX_FILTER_SOURCE_TYPE_LIVE  =  0x80000000,              ///< DMX live stream filter source
    DMX_FILTER_SOURCE_TYPE_FILE  =  0x40000000,              ///< DMX file in filter type
} DMX_FILTER_TYPE;
#endif

#define DMX_FILTER_TYPE MS_U32

#define DMX_FILTER_TYPE_VIDEO                   0x00000000UL                  ///< DMX video filter type
#define DMX_FILTER_TYPE_AUDIO                   0x00000001UL                  ///< DMX audio filter type
#define DMX_FILTER_TYPE_AUDIO2                  0x00000002UL                  ///< DMX audio2 filter type
#define DMX_FILTER_TYPE_SECTION                 0x00000003UL                  ///< DMX section filter type
#define DMX_FILTER_TYPE_PES                     0x00000004UL                  ///< DMX PES filter type
#define DMX_FILTER_TYPE_PACKET                  0x00000005UL                  ///< DMX TS packet filter type
#define DMX_FILTER_TYPE_PCR                     0x00000006UL                  ///< DMX PCR filter type
#define DMX_FILTER_TYPE_TELETEXT                0x00000007UL                  ///< DMX teletext filter type
#define DMX_FILTER_TYPE_VIDEO3D                 0x00000008UL                  ///< DMX V3D filter type
#define DMX_FILTER_TYPE_AUDIO3                  0x00000009UL                  ///< DMX audio 3 filter type
#define DMX_FILTER_TYPE_AUDIO4                  0x0000000AUL                  ///< DMX audio 4 filter type
#define DMX_FILTER_TYPE_REC                     0x0000000BUL                  ///< DMX record filter type no destination needs to be set
#define DMX_FILTER_TYPE_SCMBCHK                 0x0000000CUL                  ///< DMX filter type for checking TS scrambling status
#define DMX_FILTER_TYPE_SECTION_VER             0x0000000DUL                  ///< DMX section filter type with version and CRC check
#define DMX_FILTER_TYPE_SECTION_NO_PUSI         0x0000000EUL                  ///< DMX section filter type which receive section without PUSI
#define DMX_FILTER_FLT_MASK                     0xF2FF0F00UL

#define DMX_FILTER_SOURCE_TYPE_MASK             0xF0FC0000UL                   ///< DMX filter source mask
#define DMX_FILTER_SOURCE_TYPE_LIVE             0x80000000UL                   ///< DMX live stream filter source
#define DMX_FILTER_SOURCE_TYPE_FILE             0x40000000UL                   ///< DMX file in filter type
#define DMX_FILTER_SOURCE_TYPE_LIVEB            DMX_FILTER_SOURCE_TYPE_FILE  ///< Input from TS1 to File PID filter
#define DMX_FILTER_SOURCE_TYPE_TS1              0x00800000UL                   ///< DMX live stream filter source 1
#define DMX_FILTER_SOURCE_TYPE_TS2              0x00400000UL                   ///< DMX live stream filter source 2
#define DMX_FILTER_SOURCE_TYPE_TS3              0x00200000UL                   ///< DMX live stream filter source 3
#define DMX_FILTER_SOURCE_TYPE_FILE1            0x00100000UL                   ///< DMX file1 fource from TS1
#define DMX_FILTER_SOURCE_TYPE_FILE2            0x00080000UL                   ///< DMX file2 fource from TS2
#define DMX_FILTER_SOURCE_TYPE_FILE3            0x00040000UL

// Source ID define.  For merge stream 
#define DMX_FILTER_SOURCEID_MASK                0x00000F00UL
#define DMX_FILTER_SOURCEID_0                   0x00000000UL
#define DMX_FILTER_SOURCEID_1                   0x00000100UL
#define DMX_FILTER_SOURCEID_2                   0x00000200UL
#define DMX_FILTER_SOURCEID_3                   0x00000300UL
#define DMX_FILTER_SOURCEID_4                   0x00000400UL
#define DMX_FILTER_SOURCEID_5                   0x00000500UL
#define DMX_FILTER_SOURCEID_6                   0x00000600UL
#define DMX_FILTER_SOURCEID_7                   0x00000700UL
#define DMX_FILTER_SOURCEID_8                   0x00000800UL
#define DMX_FILTER_SOURCEID_9                   0x00000900UL
#define DMX_FILTER_SOURCEID_10                  0x00000a00UL
#define DMX_FILTER_SOURCEID_11                  0x00000b00UL
#define DMX_FILTER_SOURCEID_12                  0x00000c00UL
#define DMX_FILTER_SOURCEID_13                  0x00000d00UL
#define DMX_FILTER_SOURCEID_14                  0x00000e00UL
#define DMX_FILTER_SOURCEID_15                  0x00000f00UL


// Only supported by TSP ver3.0 driver for section filter duplicate sw patch
#define DMX_FILTER_FLT_TYPE_DUPSEC              0x02000000UL


// Only supported by TSP ver2.0 driver
#define DMX_FILTER_SOURCE_TYPE_PLAYBACK         DMX_FILTER_SOURCE_TYPE_LIVE
#define DMX_FILTER_SOURCE_TYPE_PVR0             0x20000000UL   ///< DMX PVR0 stream filter source
#define DMX_FILTER_SOURCE_TYPE_PVR1             0x10000000UL   ///< DMX PVR1 stream filter source

#define DMX_FILTER_FLT_SCMB_MASK                0x30000000UL
#define DMX_FILTER_FLT_TYPE_SCMB                0x20000000UL      ///< DMX descramble filter type
#define DMX_FILTER_FLT_TYPE_SCMB_SHAREKEY       0x10000000UL      ///< DMX descramble filter type with sharekey

//--------------------------------------------------
// Debug table
typedef enum
{
    DMX_DEBUG_CMD_NONE,
    DMX_DEBUG_CMD_CLEAR,
    DMX_DEBUG_CMD_ENABLE,
    DMX_DEBUG_CMD_DISABLE,
} DMX_DEBUG_CMD;

typedef enum
{
    DMX_DEBUG_SRC_TS0,
    DMX_DEBUG_SRC_TS1,
    DMX_DEBUG_SRC_TS2,
    DMX_DEBUG_SRC_FILE,
} DMX_DEBUG_SRC;

typedef enum
{
    DMX_DEBUG_TSIF_TS0,
    DMX_DEBUG_TSIF_TS1,
    DMX_DEBUG_TSIF_TSCB,
} DMX_DEBUG_TSIF;

typedef enum
{
    DMX_DEBUG_FIFO_VIDEO,
    DMX_DEBUG_FIFO_AUDIO,
    DMX_DEBUG_FIFO_VIDEO3D,
    DMX_DEBUG_FIFO_AUDIOB,
    DMX_DEBUG_FIFO_AUDIOC,
    DMX_DEBUG_FIFO_AUDIOD,
} DMX_DEBUG_FIFO;

typedef enum
{
    DMX_DEBUG_PKT_DEMUX_0,
    DMX_DEBUG_PKT_DEMUX_0_FILE,
    DMX_DEBUG_PKT_DEMUX_1,
    DMX_DEBUG_PKT_DEMUX_2,
    DMX_DEBUG_MMFI0,
    DMX_DEBUG_MMFI1,
} DMX_DEBUG_FIFO_SRC;

// only one playback flow or PVR flow exist
// Playback flow and PVR flow can exist simultaneously
/// DMX flow type
typedef enum
{
    DMX_FLOW_PLAYBACK                   = 0,                       ///< DMX playback flow
    DMX_FLOW_PVR                        = 1,                       ///< DMX recording flow
    DMX_FLOW_PVR1                       = 2,                       ///< DMX recording flow, note: Not support for all chip

    DMX_FLOW_PVRCA                      = 3,                       ///< CI+ PVR

    DMX_FLOW_FILEIN_MM                  = 4,                       ///< DMX Filein flow with Main path Vido and MMFI path Audio
    DMX_FLOW_MMFI0                      = DMX_FLOW_FILEIN_MM,      ///< DMX Filein flow with MMFI 0 (MMFI version 2.0)
    DMX_FLOW_FILEIN_MM3D                = 5,                       ///< DMX Filein flow with Main path Video, MMFI path 3DVideo, and MMFI Audio
    DMX_FLOW_MMFI1                      = DMX_FLOW_FILEIN_MM3D,    ///< DMX Filein flow with MMFI 1 (MMFI version 2.0)

    DMX_FLOW_CIPHSS_PLAYBACK            = 6,                       ///< Playback flow for CI+ HSS 2 path input for CI+
    DMX_FLOW_CIPHSS_PVRCA               = 7,                       ///< PVR flow for CI+ HSS 2 path input for CI+

    DMX_FLOW_PVR2                       = 8,                       ///< DMX recording flow, note: Not support for all chip, PVR2
    DMX_FLOW_PVR3                       = 9,                       ///< DMX recording flow, note: Not support for all chip
    DMX_FLOW_PVR4                       = 10,                      ///< DMX recording flow, note: Not support for all chip
    DMX_FLOW_PVR5                       = 11,                      ///< DMX recording flow, note: Not support for all chip

    DMX_FLOW_TSO_PLAYBACK               = 12,                      ///< DMX TSO playback flow (live0 & file0)
    DMX_FLOW_PLAYBACK1                  = 13,                      ///< DMX playback flow from TS1 (version 3.0)
    DMX_FLOW_PVRCA1                     = 14,                      ///< CI+ PVR (version 3.0)

    DMX_FLOW_TSO_PLAYBACK1              = 15,                      ///< DMX TSO1 playback flow (live1 & file1)
    DMX_FLOW_PLAYBACK2                  = 16,                      ///< DMX playback flow from TS2 (version 3.0/4.0)
    DMX_FLOW_PLAYBACK3                  = 17,                      ///< DMX playback flow from TS3 (version 4.0)

    DMX_FLOW_PLAYBACK_NOCA              = 18,                      ///< DMX playback flow without pass through CA
    DMX_FLOW_PLAYBACK_SRC_TS1           = 19,                      ///< DMX playback, CA input from TS1 output to Live0

    DMX_FLOW_PLAYBACK_FI                = 20,                       ///< DMX playback flow from TSFI (version 3.0)

    DMX_FLOW_NUM,
} DMX_FLOW;

/// DMX input type
typedef enum
{
    DMX_FLOW_INPUT_DEMOD,               ///< DMX input from internal demod
    DMX_FLOW_INPUT_MEM,                 ///< DMX input from memory
    DMX_FLOW_INPUT_EXT_INPUT0,          ///< DMX input from external input 0
    DMX_FLOW_INPUT_EXT_INPUT1,          ///< DMX input from external input 1
    DMX_FLOW_INPUT_EXT_INPUT2,          ///< DMX input from external input 2. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT3,          ///< DMX input from external input 3. note: Not support for all chip
    DMX_FLOW_INPUT_MEM_NOPASSCA,        ///< DMX input from memory, but not pass through CA
    DMX_FLOW_INPUT_DEMOD1,
    DMX_FLOW_INPUT_TSO,                 ///< DMX input from TSOutput
    DMX_FLOW_INPUT_TSO1,
    DMX_FLOW_INPUT_MEM_PASSCA1,         ///< DMX input from memory, but pass through CA1, not CA0
    DMX_FLOW_INPUT_MEM1,                ///< DMX input from memory path 2
    DMX_FLOW_INPUT_EXT_INPUT4,          ///< DMX input from external input 4. note: Not support for all chip
    DMX_FLOW_INPUT_EXT_INPUT5,          ///< DMX input from external input 5. note: Not support for all chip
    DMX_FLOW_INPUT_DISABLE,

} DMX_FLOW_INPUT;

/// DMX output pad
typedef enum
{
    DMX_FLOW_OUTPUT_NONE                = 0,
    DMX_FLOW_OUTPUT_EXT_PAD1            = 1,            ///< DMX output to external pad 1
    DMX_FLOW_OUTPUT_EXT_PAD3            = 3,            ///< DMX output to external pad 3
}DMX_FLOW_OUTPUT_PAD;

typedef enum
{
    DMX_TSIF_LIVE0,
    DMX_TSIF_LIVE1,
    DMX_TSIF_LIVE2,
    DMX_TSIF_LIVE3,

    DMX_TSIF_FILE0,
    DMX_TSIF_FILE1,
    DMX_TSIF_FILE2,
    DMX_TSIF_FILE3,

    DMX_TSIF_MAX,
} DMX_TSIF;

typedef enum
{
    DMX_PVR_EGN0,
    DMX_PVR_EGN1,
    DMX_PVR_EGN2,
    DMX_PVR_EGN3,
    DMX_PVR_EGN4,
    DMX_PVR_EGN5,

    DMX_PVR_ENG_NUM,
} DMX_PVR_ENG;

#if 0
// make sure what you are doing before changing the value
/// DMX callback types
typedef enum
{
    DMX_EVENT_DATA_READY           = 0x00000001, ///< DMX event ready
    DMX_EVENT_BUF_OVERFLOW         = 0x00000002, ///< DMX event overflow
    DMX_EVENT_PVRBUF_FULL          = 0x00000010, ///< DMX event PVR buffer overflow

    /// TSP self task callback // optional --> default is CB by poll
    DMX_EVENT_CB_MASK              = 0x80000000, ///< DMX callback mask
    DMX_EVENT_CB_POLL              = 0x00000000, ///< DMX callback by polling // OBSOLETE
    DMX_EVENT_CB_SELF              = 0x80000000, ///< DMX callback by demux module
    DMX_EVENT_CB_SELF_TYPE2        = 0xC0000000, ///< DMX callback by demux module
} DMX_EVENT;
#endif

#define DMX_EVENT MS_U32
#define DMX_EVENT_DATA_READY          0x00000001UL ///< DMX event ready
#define DMX_EVENT_BUF_OVERFLOW        0x00000002UL ///< DMX event overflow
#define DMX_EVENT_SEC_CRCERROR        0x00000004UL ///< DMX event CRC error
#define DMX_EVENT_PVRBUF_FULL         0x00000010UL ///< DMX event PVR buffer overflow
#define DMX_EVENT_PVR1BUF_FULL        0x00000020UL ///< DMX event Channel Browser buffer overflow
#define DMX_EVENT_PVRBUF_SIZE_MET     0x00000040UL ///< DMX event PVR buffer callback size meet. note: Not support for all chips

/// TSP self task callback // optional --> default is CB by poll
#define DMX_EVENT_CB_MASK             0x80000000UL ///< DMX callback mask
#define DMX_EVENT_CB_POLL             0x00000000UL ///< DMX callback by polling // OBSOLETE
#define DMX_EVENT_CB_SELF             0x80000000UL ///< DMX callback by demux module
#define DMX_EVENT_CB_SELF_TYPE2       0xC0000000UL ///< DMX callback by demux module

typedef enum
{
    DMX_BURSTSINGLE                = 0x00,
    DMX_BURST2BYTE                 = 0x01,
    DMX_BURST4BYTE                 = 0x02,
    DMX_BURST8BYTE                 = 0x03,
} DMX_BURSTTYPE;

#if 0
// make sure what you are doing before changing the value
/// DMX section mode setting
typedef enum
{
    DMX_SECT_MODE_CONTI            = 0x00000000, ///< DMX receive section contiguously
    DMX_SECT_MODE_ONESHOT          = 0x00000001, ///< DMX only receive one section

    // optional
    DMX_SECT_MODE_CRCCHK           = 0x00000002, ///< DMX enables CRC check
    DMX_SECT_MODE_AUTO_ADDR        = 0x00000004, ///< DMX align read pointer to write pointer when get write pointer // OBSOLETE
} DMX_SECT_MODE;
#endif

#define DMX_SECT_MODE MS_U32
#define DMX_SECT_MODE_CONTI               0x00000000UL ///< DMX receive section contiguously
#define DMX_SECT_MODE_ONESHOT             0x00000001UL ///< DMX only receive one section
#define DMX_SECT_MODE_PESSCMCHK           0x00000003UL ///< DMX checking PES SCMB status

// optional
#define DMX_SECT_MODE_CRCCHK              0x00000002UL ///< DMX enables CRC check
#define DMX_SECT_MODE_AUTO_ADDR           0x00000004UL ///< DMX align read pointer to write pointer when get write pointer // OBSOLETE
#define DMX_SECT_MODE_AUTO_CRCCHK         0x00000008UL ///< DMX enables Auto CRC check, should not be set with DMX_SECT_MODE_CRCCHK together

/// for MApi_DMX_AVFifo_Status return value u32FifoStatus
#define DMX_FIFO_STATUS            MS_U32
#define DMX_FIFO_STATUS_LV_MASK           0x0000000FUL /// use this mask to get fifo level and use below macro to get current fifo status
#define DMX_FIFO_STATUS_LV_USAGE_MASK     0x0000000CUL /// use this mask to get fifo level usage
#define DMX_FIFO_STATUS_LV_USAGE_SHIFT             2UL
#define DMX_FIFO_STATUS_LV_EMPTY          0x00000001UL /// if fifo level status equal to this macro means fifo is empty
#define DMX_FIFO_STATUS_RST_MASK          0x80000000UL /// use this mask to get fifo reset status
#define DMX_FIFO_STATUS_RST_EN            0x80000000UL /// if fifo reset status equal to this macro means fifo reset is enabled.

/// DMX filein Engine by path
typedef enum
{
    DMX_FILEIN_TSIF0,
    DMX_FILEIN_TSIF1,
    DMX_FILEIN_TSIF2,
    DMX_FILEIN_TSIF3,
    DMX_FILEIN_INVALID,
} DMX_FILEIN_PATH;


/// DMX filein destination types
typedef enum
{
    DMX_PES_AUDIO2_FIFO     = 0,              ///< DMX destination is audio2 fifo
    DMX_PES_AUDIO_FIFO      = 1,              ///< DMX destination is audio fifo
    DMX_PES_VIDEO_FIFO      = 2,              ///< DMX destination is video fifo
    DMX_PES_NO_BYPASS_FIFO  = 3,              ///< DMX destination with no bypass
    DMX_PES_VIDEO3D_FIFO    = 4,              ///< DMX destination is video3D fifo
    DMX_PES_AUDIO3_FIFO     = 5,              ///< DMX destination is audio3 fifo
    DMX_PES_AUDIO4_FIFO     = 6,              ///< DMX destination is audio4 fifo
} DMX_FILEIN_DST; //  DMXPESDATAFIFO;

/// DMX callback prototype
typedef void (*DMX_NotifyCb)(MS_U8 u8DmxId, DMX_EVENT enEvent);
typedef void (*DMX_Type2NotifyCb)(MS_U8 u8DmxId, DMX_EVENT enEvent, MS_U32 NotifyParam1);
/// DMX callback used in copy data
typedef MS_BOOL (*DMX_CheckCb)(MS_U8 u8DmxId, MS_U8* start, MS_U8* end, MS_U8* read, MS_U8* write);

// dummy data structure
typedef struct
{
    MS_U32                      streaminfo;
} DMX_Stream_info;

/// DMX section filter information structure
typedef struct
{
    MS_PHY                    SectBufAddr;        ///< DMX section buffer in physical address
    MS_U32                      SectBufSize;        ///< DMX section buffer size
    DMX_SECT_MODE               SectMode;           ///< DMX section mode
    DMX_EVENT                   Event;              ///< DMX section event
    DMX_NotifyCb                pNotify;            ///< DMX section callback
    DMX_Type2NotifyCb           pType2Notify;       ///< DMX section callback
    MS_U32                      Type2NotifyParam1;  ///< DMX section callback
} DMX_Sect_info;

/// DMX filter information structure
typedef struct
{
    union {
        DMX_Stream_info    StreamInfo; ///< DMX stream information for future use
        DMX_Sect_info      SectInfo;   ///< DMX section information
    } Info;
} DMX_Flt_info;

/// DMX PVR information structure
typedef struct
{
    MS_PHY                    pPvrBuf0;           ///< DMX PVR buffer 0 starting address
    MS_PHY                    pPvrBuf1;           ///< DMX PVR buffer 1 starting address
    MS_U32                      PvrBufSize0;        ///< DMX PVR buffer 0 size
    MS_U32                      PvrBufSize1;        ///< DMX PVR buffer 1 size
    DMX_EVENT                   Event;              ///< DMX PVR event
    DMX_NotifyCb                pNotify;            ///< DMX PVR callback

    MS_U16                      u16CaPvrModel;      ///< DMX PVR CA mode
    MS_BOOL                     bSpsEnable;         ///< DMX PVR CA mode

} DMX_Pvr_info;

typedef enum
{
    DMX_CA_PVR_TSP                          = 0x00000000UL,
    DMX_CA_PVR_PROG0                        = 0x00000001UL,
    DMX_CA_PVR_PROG1                        = 0x00000002UL,
}DMX_CA_PVRMODE;

//----------------------------------
/// DMX debug table information structure
//----------------------------------
typedef struct
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_SRC               TspSrc;
    DMX_DEBUG_FIFO              TspFifo;
} DMX_DisContiCnt_info, DMX_DropPktCnt_info;

typedef struct
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_TSIF              TspTsif;
} DMX_LockPktCnt_info;

typedef struct
{
    DMX_DEBUG_CMD               TspCmd;
    DMX_DEBUG_FIFO              TspFifo;
    DMX_DEBUG_FIFO_SRC          TspFifoSrc;
} DMX_AVPktCnt_info;

#define DMX_MAX_SIGFLAG_NUM             10UL
#define DMX_MAX_PVRSIGFLAG_NUM          6UL
typedef struct 
{   
    MS_U32                 u32Eng;
    MS_U32                 u32EvtWaitOpt;   
    MS_U32                 u32DmxEvt;
    MS_U32                 u32TspEvt;
    
    MS_U32                 u32PvrBufFullFlag[DMX_MAX_PVRSIGFLAG_NUM];       

    MS_U32                 u32SecRdyFlag[DMX_MAX_SIGFLAG_NUM];
    MS_U32                 u32SecOvfFlag[DMX_MAX_SIGFLAG_NUM];
}DMX_TSP_IOSIGNAL;

/// DMX capability query type
typedef enum
{
    DMX_CAP_PID_FILTER_NUM          = 0,          ///< Get PID filter number
    DMX_CAP_SEC_FILTER_NUM          = 1,          ///< Get section filter number
    DMX_CAP_SEC_BUF_NUM             = 2,          ///< Get section buffer number

    DMX_CAP_PVR_ENG_NUM             = 3,          ///< Get pvr engine number
    DMX_CAP_PVR_FILTER_NUM          = 4,          ///< Get pvr filter number
    DMX_CAP_PVR1_FILTER_NUM         = 5,          ///< Get pvr1 filter number

    DMX_CAP_MMFI_AUDIO_FILTER_NUM   = 6,          ///< Get MM file in audio path filter number
    DMX_CAP_MMFI_V3D_FILTER_NUM     = 7,          ///< Get MM file in video 3D path filter number

    DMX_CAP_MMFI0_FILTER_NUM        = 6,          ///< Get MM file in 0 path filter number
    DMX_CAP_MMFI1_FILTER_NUM        = 7,          ///< Get MM file in 1 path filter number

    DMX_CAP_TSIF_NUM                = 8,          ///< Get ts interface number
    DMX_CAP_DEMOD_NUM               = 9,          ///< Get demod support number
    DMX_CAP_TS_PAD_NUM              = 10,         ///< Get TS pad support number
    DMX_CAP_VQ_NUM                  = 11,         ///< Get virtual queue support number

    DMX_CAP_CA_FLT_NUM              = 12,         ///< Get descramble filter number
    DMX_CAP_CA_KEY_NUM              = 13,         ///< Get total descramble key number

    DMX_CAP_FW_ALIGN                = 14,         ///< Get MIU alignment in bytes of DMX FW
    DMX_CAP_VQ_ALIGN                = 15,         ///< Get MIU alignment in bytes of Virtual queue
    DMX_CAP_VQ_PITCH                = 16,         ///< Get length in bytes in bytes of one VQ unit
    DMX_CAP_SEC_BUF_ALIGN           = 17,         ///< Get MIU alignment in bytes of section buffer
    DMX_CAP_PVR_ALIGN               = 18,         ///< Get miu alignment in bytes of record buffer

    DMX_CAP_PVRCA_PATH_NUM          = 19,         ///< Get the numbers of descramble record path

    DMX_CAP_SHAREKEY_FLT_RANGE      = 20,         ///< Get the range of sharekey descramble 0  filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA0_FLT_RANGE        = 21,         ///< Get the range of PVRCA0 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA1_FLT_RANGE        = 22,         ///< Get the range of PVRCA1 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_PVRCA2_FLT_RANGE        = 23,         ///< Get the range of PVRCA2 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_SHAREKEY_FLT1_RANGE     = 24,         ///< Get the range of sharekey descramble 1 filter id. data0 is start ID, data 1 is end ID
    DMX_CAP_SHAREKEY_FLT2_RANGE     = 25,         ///< Get the range of sharekey descramble 2 filter id. data0 is start ID, data 1 is end ID

    DMX_CAP_HW_TYPE                 = 26,         ///< Get the HW architecture type. 0xXXX0: No PVR, 0x100X: TV series, 0x200X: STB series

    DMX_CAP_RESOURCE_SIZE           = 27,         ///< Get DMX Private Resource data struct size (Share resource)

    DMX_CAP_VFIFO_NUM               = 28,         ///< Get VFIFO support number
    DMX_CAP_AFIFO_NUM               = 29,         ///< Get AFIFO support number

    DMX_CAP_HWPCR_SUPPORT           = 30,         ///< Get HWPCR support status
    DMX_CAP_PCRFLT_START_IDX        = 31,         ///< Get PCR start index
    DMX_CAP_TSP_HWWP_SET_NUM        = 32,         ///< Get TSP write protect set numbers

    DMX_CAP_DSCMB_ENG_NUM           = 33,         ///< Get DSCMB engine numbers

    DMX_CAP_MAX_MERGESTR_NUM        = 34,         ///< Get Maxumum merge stream number

    DMX_CAP_MAX_SEC_FLT_DEPTH       = 35,
    DMX_CAP_FW_BUF_SIZE             = 36,
    DMX_CAP_FW_BUF_RANGE            = 37,
    DMX_CAP_VQ_BUF_RANGE            = 38,
    DMX_CAP_SEC_BUF_RANGE           = 39,
    DMX_CAP_FIQ_NUM                 = 40,

    DMX_CAP_NULL,
} DMX_QUERY_TYPE;

typedef enum
{
    DMX_HW_TYPE_NONE             = 0UL,
    DMX_HW_TYPE_TV_SERIES        = 0x00001000UL,       ///< The HW architecture is TV series without PVR cunfction
    DMX_HW_TYPE_TV_TYPE1         = 0x00001001UL,       ///< The HW architecture is TV series with Seperate PVR filters
    DMX_HW_TYPE_TV_TYPE2         = 0x00001002UL,       ///< The HW architecture is TV series with general PVR filters
    DMX_HW_TYPE_TV_TYPE2_CA2     = 0x00001006UL,       ///< The HW architecture is TV series with general PVR filters, with general dscmb filters, and merge stream
    DMX_HW_TYPE_STB_SERIES       = 0x00002000UL,       ///< The HW architecture is BOX series without PVR cunfction
    DMX_HW_TYPE_STB_TYPE1        = 0x00002001UL,       ///< The HW architecture is BOX series 1
    DMX_HW_TYPE_STB_TYPE2        = 0x00002002UL,       ///< The HW architecture is BOX series 2
    DMX_HW_TYPE_STB_TYPE3        = 0x00002003UL,       ///< The HW architecture is BOX series 3
    DMX_HW_TYPE_STB_TYPE3_CASPS  = 0x80002003UL,       ///< The HW architecture is BOX series 3 with PVR CA SPS enable mode
} DMX_HW_TYPE;

typedef enum
{
    DMX_TIMESTAMP_CLK_90K = 0x00000000UL,
    DMX_TIMESTAMP_CLK_27M = 0x00000001UL,
} DMX_TimeStamp_Clk;

/// TSP file in Packet mode
typedef enum //_DMX_Filein_Packmode
{
    DMX_PKTMODE_188         = 0x00000000UL,       ///< DMX file in packet size with 188 (Single stream)
    DMX_PKTMODE_192         = 0x00000001UL,       ///< DMX file in packet size with 192 (Single stream)
    DMX_PKTMODE_204         = 0x00000002UL,       ///< DMX file in packet size with 204 (Single stream)
    DMX_PKTMODE_130         = 0x00000003UL,       ///< DMX file in packet size with 130 (RVU)
    DMX_PKTMODE_134         = 0x00000004UL,       ///< DMX file in packet size with 134 (RVU with timestamp)
    DMX_PKTMODE_200         = 0x00000005UL,       ///< DMX file in packet size with 200 (Open cable Single stream)

    DMX_PKTMODE_MERG188     = 0x00000006UL,       ///< DMX file in packet size with 188 (Merge stream)
    DMX_PKTMODE_MERG192     = 0x00000007UL,       ///< DMX file in packet size with 192 (Merge stream)
    DMX_PKTMODE_MERG200     = 0x00000008UL,       ///< DMX file in packet size with 200 (Open cable Merge stream)
} DMX_Filein_PacketMode;

#define DMX_PacketMode DMX_Filein_PacketMode

/// TSP file in information structure
typedef struct
{
    MS_U32                 Rate;   ///< DMX file in rate
    DMX_Filein_PacketMode  PKT_Mode;    ///< DMX file in packet size mode
} DMX_Filein_info;

typedef struct
{
    DMX_FLOW_INPUT          Input;
    MS_BOOL                 bClkInv;
    MS_BOOL                 bExtSync;
    MS_BOOL                 bParallel;
}DMX_FlowInput_Info;

//MMFI Path
#define DMX_MMFI_PATH       MS_U32
#define DMX_MMFI_AUDIO      0UL                   ///< DMX destination with no bypass to MM Audio path
#define DMX_MMFI_VIDEO      1UL                   ///< DMX destination with no bypass to MM Video path
#define DMX_MMFI_PATH0      DMX_MMFI_AUDIO
#define DMX_MMFI_PATH1      DMX_MMFI_VIDEO

// must alignment with TSP_FIFO_XXX in halTSP.h
typedef enum
{
    DMX_MMFI_FLTTYPE_AUD    = 0x00,
    DMX_MMFI_FLTTYPE_AUDB   = 0x01,
    DMX_MMFI_FLTTYPE_VD3D   = 0x02,
    DMX_MMFI_FLTTYPE_VD     = 0x03,
    DMX_MMFI_FLTTYPE_AUDC   = 0x04,
    DMX_MMFI_FLTTYPE_AUDD   = 0x05,

    DMX_MMFI1_FLTTYPE_MASK  = 0x80,
}DMX_MMFI_FLTTYPE;

typedef enum
{
    DMX_MMFI_PES_NO_BYPASS_AU       = 0x0000,

    DMX_MMFI_PES_PS_AU              = 0x0001,
    DMX_MMFI_PES_PS_AUB             = 0x0002,

    DMX_MMFI_PES_APID_BYPASS        = 0x0004,
    DMX_MMFI_PES_APIDB_BYPASS       = 0x0008,

    DMX_MMFI_PES_NO_BYPASS_V3D      = 0x0010,
    DMX_MMFI_PES_PS_V3D             = 0x0020,
    DMX_MMFI_PES_V3DPID_BYPASS      = 0x0040,

    DMX_MMFI_PES_NO_BYPASS_VD       = 0x0080,
    DMX_MMFI_PES_VPID_BYPASS        = 0x0100,

    DMX_MMFI_PES_NO_BYPASS_TS       = 0x0200,

    DMX_MMFI_PES_PS_VD              = 0x0400,

    DMX_MMFI_PES_PS_AUC             = 0x0800,
    DMX_MMFI_PES_PS_AUD             = 0x1000,

    DMX_MMFI1_PES_TYPE_MASK         = 0x8000,   ///<  Must set, when using MMFI1 path
}DMX_MMFI_DST;

typedef enum
{
    E_DMX_TSO_OUTCLK_DIV2N,
    E_DMX_TSO_OUTCLK_DIVN,
    E_DMX_TSO_OUTCLK_62M,
    E_DMX_TSO_OUTCLK_54M,
    E_DMX_TSO_OUTCLK_PTSOOUT,
    E_DMX_TSO_OUTCLK_PTSOOUT_DIV8,
    E_DMX_TSO_OUTCLK_Dmd,    
    E_DMX_TSO_OUTCLK_27M,
}DMX_TSO_OutClk;

typedef enum
{
    E_DMX_TSO_OUTCLKSRC_172M_2N,    // E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_288M_2N,    // E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_432M_2N,    // E_DMX_TSO_OUTCLK_DIV2N
    E_DMX_TSO_OUTCLKSRC_216M_N,     // E_DMX_TSO_OUTCLK_DIVN
    E_DMX_TSO_OUTCLKSRC_P_TS0IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS1IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS2IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS3IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS4IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
    E_DMX_TSO_OUTCLKSRC_P_TS5IN,    // E_DMX_TSO_OUTCLK_PTSOOUT or E_DMX_TSO_OUTCLK_PTSOOUT_DIV8
}DMX_TSO_OutClkSrc;

typedef struct
{
    MS_U32                  u32Res;
    DMX_FLOW                eFlow;
    DMX_TSIF                 eTSOInIf;
    DMX_FlowInput_Info        stInputInfo;
    MS_U8                    u8LocalStrId;
    MS_BOOL                    bBypassAll;
    MS_BOOL                    bEnable;
    MS_BOOL                 bSet;
}DMX_TSO_InputCfg;

typedef struct
{
    MS_U32                  u32Res;
    DMX_FLOW                eFlow;
    DMX_FLOW_OUTPUT_PAD        eOutPad;
    MS_U16                    u16OutPktSize;
    MS_BOOL                    bEnable;    
    
    MS_BOOL                    bDefOutClk;    
    MS_BOOL                    bOutClkInv;    
    DMX_TSO_OutClk             eTsoOutClk;
    DMX_TSO_OutClkSrc          eTsoOutClkSrc;
    MS_U16                     u16DivNum;            
    MS_BOOL                 bSet;
}DMX_TSO_OutputCfg;

/// DMX FQ init infomation
typedef struct
{
    MS_U8    u8AddrMode;
    MS_PHY u32BufStart;
    MS_U32   u32BufSize;
} DMX_FQ_Info;

typedef enum
{
    E_DMX_FQ_SKIP_RUSH_DATA_PATH_NON        = 0x00000000UL,
    E_DMX_FQ_SKIP_APES_RUSH_DATA            = 0x00000008UL,
    E_DMX_FQ_SKIP_APES_B_RUSH_DATA          = 0x00000010UL,
    E_DMX_FQ_SKIP_VPES_RUSH_DATA            = 0x00000020UL,
    E_DMX_FQ_SKIP_SEC_RUSH_DATA             = 0x00000040UL,
    E_DMX_FQ_SKIP_ADP_RUSH_DATA             = 0x00000080UL,
    E_DMX_FQ_SKIP_PCR_RUSH_DATA             = 0x00000100UL,
    E_DMX_FQ_SKIP_RASP_RUSH_DATA            = 0x00000200UL,
    E_DMX_FQ_SKIP_PVR1_RUSH_DATA            = E_DMX_FQ_SKIP_RASP_RUSH_DATA,    
    E_DMX_FQ_SKIP_PCR1_RUSH_DATA            = 0x00001000UL,
    E_DMX_FQ_SKIP_PVR3_RUSH_DATA            = E_DMX_FQ_SKIP_PCR1_RUSH_DATA,    
    E_DMX_FQ_SKIP_CFG1_MASK                 = 0x80000000UL,
    E_DMX_FQ_SKIP_APES_C_RUSH_DATA          = 0x80000008UL,
    E_DMX_FQ_SKIP_APES_D_RUSH_DATA          = 0x80000010UL,
    E_DMX_FQ_SKIP_V3DPES_RUSH_DATA          = 0x80000020UL,
    E_DMX_FQ_SKIP_PVR2_RUSH_DATA            = 0x80000200UL,
}DMX_FQ_SkipPath;

/// DMX TEI  Remove Error Packet Infomation
typedef enum
{
    E_DMX_TEI_REMOVE_AUDIO_PKT,     ///< DMX TEI Remoce Audio Packet
    E_DMX_TEI_REMOVE_VIDEO_PKT,     ///< DMX TEI Remoce Video Packet

    E_DMX_TEI_REMOVE_PKT0_LIVE,
    E_DMX_TEI_REMOVE_PKT0_FILE,
    E_DMX_TEI_REMOVE_PKT1,
    E_DMX_TEI_REMOVE_PKT2,

}DMX_TEI_RmPktType;

//STC update Control Parameters define
#define eStcUpdateCtrlMode                  MS_U8
#define DMX_STC_UPDATE_FW                   0x00
#define DMX_STC_UPDATE_HK                   0x01
#define DMX_STC_UPDATE_UPDATEONCE           0x02

//S2P Clock Option
#define eS2PClkOpt                          MS_U32
#define S2P_CLK_OPT_NONE                    0x00000000
#define S2P_CLK_OPT_INVERT                  0x00000001
#define S2P_CLK_OPT_NON_INVERT              0x00000002

////////////////////////////////////////////////////////////////////////////////
// include utopia v2  header files here
////////////////////////////////////////////////////////////////////////////////
#ifdef DMX_UTOPIA_20
#include "apiDMX_v2.h"
#endif

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @ingroup TSP_General
/// @param pu32ResMemAddr       \b IN: Pointer to store resource memory addresses
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_InitLibResource(void *pResMemAddr);
//-------------------------------------------------------------------------------------------------
/// Set TSP firmware address and size.
/// @ingroup TSP_General
/// @param pFwAddr        \b IN: TSP firmware address in DRAM physical address
/// @param u32FwSize     \b IN: TSP firmware size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize); // In DRAM physical address
//-------------------------------------------------------------------------------------------------
/// Set the MIU address for TSP aeon (firmware) used.
/// @ingroup TSP_General
/// @param phyDataAddr       \b IN: MIU phsyical address
/// @param u32size               \b IN: MIU buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32size);
/// Set TSP is work on HK or AEON.
/// @ingroup TSP_General
/// @param bIsHK       \b IN: TRUE: HK, FALSE: AEON
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetHK(MS_BOOL bIsHK); // OBSOLETE
//-------------------------------------------------------------------------------------------------
/// Initialize Demux module.
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Init(void);

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for Virtual Quere Enable
/// @ingroup TSP_General
/// @param param          \b IN: Init Virtual Quere Setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use virtual quere, please call this API after calling MApi_DMX_Init. (VQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_TSPInit(DMX_TSPParam *param);
//-------------------------------------------------------------------------------------------------
/// Exit Demux API
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK      - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Exit(void);

//-------------------------------------------------------------------------------------------------
/// Force exit Demux API
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It is special case for MM
///-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_ForceExit(void);
//-------------------------------------------------------------------------------------------------
/// Save DMX driver states to DRAM.
/// @ingroup DMX_STR
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Suspend(void);
//-------------------------------------------------------------------------------------------------
/// Restore DMX driver states from DRAM. And TSP need input the firmware info. to reload TSP FW.
/// @ingroup DMX_STR
/// @param phyFWAddr     \b IN: TSP firmware load address
/// @param u32FWSize      \b IN: TSP firmware size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize);
//-------------------------------------------------------------------------------------------------
/// Check TSP is alive or not. (Firmware)
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_ChkAlive(void);

//-------------------------------------------------------------------------------------------------
/// Reset TSP and MM. It will reset all filter and do HW reset.
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Release TSP driver HW lock semaphone
/// @ingroup TSP_General
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_ReleaseSemaphone(void);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable TSP MIU internal write protect. Include PVR record buffer and section buffer.
/// @ingroup TSP_General
/// @param bEnable             \b IN: Enable/Disable protect
/// @param pphyStartAddr    \b IN: Protect start address. Array[0-3]: [0]: PVR, [1-2]: Section, [3]: PVR1
/// @param pphyEndAddr      \b IN: Protect end address. Array[0-3]: [0]: PVR, [1-2]: Section, [3]: PVR1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  void              MApi_DMX_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Demux MIU internal Firmware write protect.
/// @ingroup TSP_General
/// @param bEnable             \b IN: Enable/Disable protect
/// @param phyStartAddr     \b IN: Protect start address.
/// @param phyEndAddr       \b IN: Protect end address.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  void              MApi_DMX_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);
//-----------------
// DMX_ToBeRemove
//-----------------
//-------------------------------------------------------------------------------------------------
/// Read drop packet count of AV due to duplicate. (Use new debug function to get this information)
/// @ingroup DMX_ToBeRemove
/// @param  eDmxPktType  \b IN: The packet type wihch want to remove
/// @param  bEnable           \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
// // DMX_FILTER_STATUS MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);
//  DMX_FILTER_STATUS MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);
//-------------------------------------------------------------------------------------------------
/// Control DMX STR functions.
/// @ingroup DMX_STR
/// @param  u16PowerState  \b IN: STR power mode. (to control STR suspend & resume case.)
/// @param phyFWAddr        \b IN: TSP firmware load address
/// @param u32FWSize         \b IN: TSP firmware size
/// @return : Utopia status - UTOPIA_STATUS_SUCCESS (Succeed) or UTOPIA_STATUS_FAIL (Fail)
//-------------------------------------------------------------------------------------------------
 MS_U32 MApi_DMX_SetPowerState(EN_POWER_MODE u16PowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize);
DMX_FILTER_STATUS MApi_DMX_TEI_RemoveErrorPkt(DMX_TEI_RmPktType eDmxPktType, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set Demux Flow packet mode
/// @ingroup TSP_General
/// @param DmxFlow   \b IN: TSP flow path
/// @param ePktMode  \b IN: Packet mode of flow path.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_SetPacketMode(DMX_FLOW DmxFlow, DMX_PacketMode ePktMode);
//-------------------------------------------------------------------------------------------------
/// Set Sync Byte value to Stream source ID or TS source ID.
/// @ingroup TSP_Merge
/// @param u32SrcID        \b IN: Index of stream source id or TS source id
/// @param u8SyncByte     \b IN: Sync byte, defalut value is 0x47
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  u32SrcID - Stream source ID is from 0 ~7
///                              TS source ID is mapping TSIF index
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte);
//-------------------------------------------------------------------------------------------------
/// Set enable/disable drop scramble packet of FIFO/PVR. If enable, scramble packet can't pass through FIFO/PVR.
/// @ingroup TSP_General
/// @param  DmxFltType      \b IN: Type select: FIFO or PVR
/// @param  bEnable            \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_DropScmbPkt(DMX_FILTER_TYPE pDmxFltType, MS_BOOL bEnable);

// owner ship set/checking
//--------------------------------------------------------------------------------------------------
/// Set demux filter owner.
/// @ingroup TSP_General
/// @param  u8DmxIdStart     \b IN: Demux filter start index
/// @param  u8DmxIdEnd       \b IN: Demux filter end index
/// @param  bOwner              \b IN: TRUE for owner, FALSE for not owner
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetOwner(MS_U8 u8DmxIdStart, MS_U8 u8DmxIdEnd, MS_BOOL bOwner); //OBSOLETE

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability.
/// @ingroup TSP_General
/// @param  DmxQueryType     \b IN: query type
/// @param  pOutput                \b OUT: answer of query type
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput);
//-----------------
// DMX_ToBeModified
//-----------------
//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability by string type.
/// @ingroup DMX_ToBeModified_TSP
/// @param  pstrQueryType      \b IN: String of capability item
/// @param  s32Strlen              \b IN: String size of capability item
/// @param  pOutput                \b OUT: answer of query type
/// @param  pseOutSize           \b OUT: Output data length
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Keep MApi_DMX_GetCap function and refine interface for parameter
//--------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_GetCap_Ex(char* pstrQueryType, MS_S32 s32Strlen, void* pOutput, MS_S32* ps32OutSize);

//-------------------------------------------------------------------------------------------------
/// Invert Demux parallel input bit order
/// @ingroup TSP_General
/// @param DmxFlow     \b IN: TSP flow path
/// @param bInvert       \b IN: Invert bit order of TS parallel in. (TRUE: Invert, FALSE: Not invert)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Parl_Invert(DMX_FLOW DmxFlow, MS_BOOL bInvert);
//-------------------------------------------------------------------------------------------------
/// Set Demux DMA burst length.
/// @ingroup TSP_General
/// @param BurstType    \b IN: Burst length
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//------------------------------------------------------------------------------------------------
// // DMX_FILTER_STATUS MApi_DMX_SetBurstLen(DMX_BURSTTYPE BurstType);
//  DMX_FILTER_STATUS MApi_DMX_SetBurstLen(DMX_BURSTTYPE BurstType);
//-------------------------------------------------------------------------------------------------
/// Enable/Disable TSP VQ.
/// @ingroup TSP_General
/// @param bEnable        \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_VQ_Enable(MS_BOOL bEnable);

//-----------------
// TSP_Flow
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set Demux Flow config.
/// @ingroup TSP_Flow
/// @param DmxFlow          \b IN: TSP flow path
/// @param DmxFlowInput   \b IN: Input port. (ex: external pad0, internal demod...etc)
/// @param bClkInv            \b IN: TS-in options: clock phase inversion
/// @param bExtSync          \b IN: TS-in options: sync by external signal
/// @param bParallel           \b IN: TS-in is parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel);
//--------------------------------------------------------------------------------------------------
/// Setting PVR flow.
/// @ingroup TSP_Flow
/// @param  Eng                  \b IN: PVR Engine ID
/// @param  ePvrSrcTSIf      \b IN: Select the record source
/// @param  bDscmbRec       \b IN: TRUE for recording dscmbled stream; FALSE for recording orignal stream
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PVR_FlowSet(DMX_PVR_ENG Eng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec);

//-------------------------------------------------------------------------------------------------
/// Get Demux Flow setting.
/// @ingroup TSP_Flow
/// @param DmxFlow           \b IN: TSP flow path
/// @param pDmxFlowInput  \b OUT: pointer to store DMX input flow value.
/// @param pbClkInv           \b OUT: pointer to store clock phase inversion
/// @param pbExtSync         \b OUT: pointer to store sync by external signal
/// @param pbParallel          \b OUT: pointer to store parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel);

//-------------------------------------------------------------------------------------------------
/// Enable Demux flow path.
/// @ingroup TSP_Flow
/// @param DmxFlow           \b IN: TSP flow path
/// @param bEnable             \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_FlowEnable(DMX_FLOW DmxFlow, MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Set/Get DScmbler engine id depend on TSIF path
/// @ingroup TSP_Flow
/// @param  eTsIf                 \b IN: TSP flow path (interface)
/// @param  pu32EngId         \b IN/OUT: pointer to store dscmb engine id
/// @param  bSet                  \b IN: TRUE:Set dscmb engine id ; FALSE: Get dscmb engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Flow_DscmbEng(DMX_TSIF eTsIf, MS_U32* pu32EngId, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Configure TS output pad.
/// @ingroup TSP_Flow
/// @param  eOutPad            \b IN: Select output pad
/// @param  eInSrcPad         \b IN: Select input source pad
/// @param  bInParallel         \b IN: TRUE: input source is paralleled; FALSE: input source is serial in
/// @param  u32ResvNum      \b IN: Reserved value of the number of reserved parameters for future using
/// @param  pu32vRes           \b IN: Reserved pointer to store parameters for future using
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only EXT_PAD1 and EXT_PAD3 has output mode
/// @note If output from S2P mode, input source should be external input pad only
/// @note If output from TSO mode, eInputPad should be E_DRVTSP_PAD_TSO
/// @note If input source is internal demod, output mode can be demod out or TSO out
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TsOutputPadCfg(DMX_FLOW_OUTPUT_PAD eOutPad, DMX_FLOW_INPUT eInSrcPad, MS_BOOL bInParallel, MS_U32 u32ResvNum, MS_U32 *pu32Resv);

//-------------------------------------------------------------------------------------------------/// Configure output pad
/// Set S2P output clock phase and invert.
/// @ingroup TSP_Flow
/// @param  u16Val               \b IN: Value of phase
/// @param  bEnable              \b IN: Enable/Disable phase tuning
/// @param  u32Reserved      \b IN: Reserved for future using
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Configure for S2P mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TsS2POutputClkPhase(MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt);

//-----------------
// TSP_STC
//-----------------
//--------------------------------------------------------------------------------------------------
/// Enable/Disable STC64 bit mode.
/// @ingroup TSP_STC
/// @param  bEnable              \b IN: Enable or Disable STC64 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note This is also set by TSP init function.
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_STC64_Mode_Enable(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Get STC counter.
/// @ingroup DMX_ToBeRemove
/// @param pu32Stc32       \b OUT: STC counter (bit 32)
/// @param pu32Stc          \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Get(MS_U32* pu32Stc32, MS_U32* pu32Stc);
//-------------------------------------------------------------------------------------------------
/// Set STC counter.
/// @ingroup DMX_ToBeRemove
/// @param pu32Stc32     \b OUT: STC counter (bit 32)
/// @param pu32Stc         \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Set(MS_U32 u32Stc32, MS_U32 u32Stc);
//-------------------------------------------------------------------------------------------------
/// Get PCR value.
/// @ingroup DMX_ToBeRemove
/// @param pu32Pcr32    \b OUT: PCR counter (bit 32)
/// @param pu32Pcr       \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pcr_Get(MS_U32* pu32Pcr32, MS_U32* pu32Pcr);
//-------------------------------------------------------------------------------------------------
/// Select STC Engine.
/// @ingroup TSP_STC
/// @param eFltSrc            \b IN: Fliter Source type and Filter source ID
/// @param u32StcEng       \b IN: STC engine ID (0, 1)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only support in driver version 3.0.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng);
//-------------------------------------------------------------------------------------------------
/// Get STC counter.
/// @ingroup TSP_STC
/// @param u8Eng             \b IN: STC Engine ID
/// @param pu32Stc32       \b OUT: STC counter (bit 32)
/// @param pu32Stc          \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Stc32, MS_U32* pu32Stc);

//-------------------------------------------------------------------------------------------------
/// Set STC counter.
/// @ingroup TSP_STC
/// @param u8Eng           \b IN: STC Engine ID
/// @param pu32Stc32     \b OUT: STC counter (bit 32)
/// @param pu32Stc         \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Eng_Set(MS_U8 u8Eng, MS_U32 u32Stc32, MS_U32 u32Stc);

//-------------------------------------------------------------------------------------------------
/// Get PCR.
/// @ingroup TSP_STC
/// @param u32PcrEng     \b INPUT: PCR Eng
/// @param pu32Pcr32     \b OUT: PCR counter (bit 32)
/// @param pu32Pcr         \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pcr_Eng_Get(MS_U8 u8PcrEng, MS_U32* pu32Pcr32, MS_U32* pu32Pcr);
//-------------------------------------------------------------------------------------------------
/// Set STC control.
/// @ingroup TSP_STC
/// @param u8Eng             \b IN: STC Engine ID
/// @param eMode             \b IN: STC update control options
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note eMode: 0x00 -> Update STC by TSP FW
/// @note             0x01 -> Update STC by HK
/// @note             0x02 -> Update STC Once when PCR reset
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_STC_UpdateCtrl(MS_U8 u8Eng, eStcUpdateCtrlMode eMode);
//-------------------------------------------------------------------------------------------------
/// Add or subtract STC Offset.
/// @ingroup TSP_STC
/// @param u8Eng               \b IN: STC Engine ID
/// @param u32Offset          \b IN: STC offset value
/// @param bAdd                \b IN:  If TRUE, add the offset value; otherwise subtract the offset value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Stc_Eng_SetOffset(MS_U32 u32Eng, MS_U32 u32Offset, MS_BOOL bAdd);

//-------------------------------------------------------------------------------------------------
/// Adjust STC Clock.
/// @ingroup TSP_STC
/// @param u32EngId            \b IN: STC Engine ID
/// @param bUpClk                \b IN: If TRUE, Clk will be faster, and if FALSE, Clk will be slower
/// @param u32Percentage     \b IN: The percentage of STC clock ratio
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stc_Clk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage);

//-----------------
// TSP_Filter
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open a demux filter.
/// @ingroup TSP_Filter
/// @param DmxFltType    \b IN: The filter information to allocate
/// @param pu8DmxId      \b OUT: The available demux filer index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Open(DMX_FILTER_TYPE DmxFltType, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Close a demux filter.
/// @ingroup TSP_Filter
/// @param u8DmxId        \b IN: the demux filer index to free
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Activate a demux filter.
/// @ingroup TSP_Filter
/// @param u8DmxId        \b IN: the demux filer index to activate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Start(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter.
/// @ingroup TSP_Filter
/// @param u8DmxId         \b IN: the demux filer index to deactivate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Stop(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Get a demux filter information.
/// @ingroup TSP_Filter
/// @param u8DmxId         \b IN: the demux filer index
/// @param pDmxFltInfo     \b IN/OUT: the demux filer info
/// @param pDmxFltType    \b IN/OUT: the demux filer type
/// @param bSet                \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Info(MS_U8 u8DmxId, DMX_Flt_info* pDmxFltInfo, DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set a demux filter PID.
/// @ingroup TSP_Filter
/// @param u8DmxId      \b IN: the demux filer index
/// @param pu16Pid        \b IN/OUT: PID
/// @param bSet             \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pid(MS_U8 u8DmxId, MS_U16* pu16Pid, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Get a demux filter status.
/// @ingroup TSP_Filter
/// @param u8DmxId       \b IN: the demux filer index
/// @param pbEnable       \b OUT: the demux filer enable/disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_IsStart(MS_U8 u8DmxId, MS_BOOL* pbEnable);

//--------------------------------------------------------------------------------------------------
/// Copy section data to assigned buffer
/// @ingroup TSP_Filter
/// @param  u8DmxId              \b IN: demux ID
/// @param  pu8Buf                 \b OUT: section buffer address
/// @param  u32BufSize            \b IN: section buffer size
/// @param  pu32ActualSize      \b IN: section data size
/// @param  pu32RmnSize        \b OUT: section buffer remainning data size
/// @param  pfCheckCB           \b IN: callback function to check data correct or not
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_CopyData(MS_U8 u8DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB);

//--------------------------------------------------------------------------------------------------
/// Ask demuxer to process section/pvr data for Non-OS driver.
/// @ingroup TSP_Filter
/// @param  u32FltId           \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
/// @param  u32Timeout      \b IN: Max time for TSP to process
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Calling this function in OS environment causes nothing
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Proc(MS_U8 u8DmxId, DMX_EVENT* pEvent);

//--------------------------------------------------------------------------------------------------
/// Change DMX filter source type.
/// @ingroup TSP_Filter
/// @param  u8DmxId             \b IN: DMX filter ID
/// @param  DmxFltSrcType     \b IN: DMX filter Source Type and filter source id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Change_FilterSource(MS_U8 u8DmxId, DMX_FILTER_TYPE DmxFltSrcType);
//--------------------------------------------------------------------------------------------------
///MApi_DMX_FilterFlowSwitch : Switch filter source from one channel to another.
/// @ingroup TSP_Filter
/// @param  TgtFlow        \b IN: chagne to this path setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note TSP 1.0:
/// @note DMX_FILTER_SOURCE_TYPE_LIVE
/// @note DMX_FILTER_SOURCE_TYPE_TS1
/// @note TSP 2.0:
/// @note DMX_FILTER_SOURCE_TYPE_PLAYBACK
/// @note DMX_FILTER_SOURCE_TYPE_PVR0
/// @note DMX_FILTER_SOURCE_TYPE_PVR1
/// @note TSP 4.0:
/// @note DMX_FILTER_SOURCE_TYPE_LIVE
/// @note DMX_FILTER_SOURCE_TYPE_FILE
/// @note DMX_FILTER_SOURCE_TYPE_TS1
/// @note DMX_FILTER_SOURCE_TYPE_TS2
/// @note DMX_FILTER_SOURCE_TYPE_TS3
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_LiveSrcSwitch(DMX_FILTER_TYPE TgtFlow);
/// Get demux filter owner.
/// @ingroup TSP_General
/// @param  u8DmxIdStart       \b IN: Demux filter index
/// @param  bOwner                \b OUT: Pointer to store return value of owner flag
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_GetOwner(MS_U8 u8DmxId, MS_BOOL* pbOwner); //OBSOLETE
//-----------------
// TSP_SCMB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status.
/// @ingroup TSP_SCMB
/// @param  FltSrc                 \b IN    : Filter source
/// @param  u32FltGroupId     \b IN    : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63, 2: 64~95, 3: 96~127
/// @param  PidFltId               \b IN    : Pid filter id for getting. If 0xFFFFFFFF, means getting the status of all filters of the same group
/// @param  pu32ScmbSts      \b OUT : The scrmabling status of pid filter.
/// @return TSP_Result
/// @note pu32ScmbSts: If getting all group status, every bit means one status of every filter.
/// @note                       If only checking one filter,  1 means scrambled status and 0 means non-scrambled status.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_FltScmbSts(DMX_FILTER_TYPE FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts);
//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status.
/// @ingroup TSP_SCMB
/// @param u8DmxId       \b IN: the demux filer Id
/// @param pu8scmb       \b OUT: the PES scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_PesScmbSts(MS_U8 u8DmxId, MS_U8* pu8scmb);
//-------------------------------------------------------------------------------------------------
/// Get the TS scrambling control status.
/// @ingroup TSP_SCMB
/// @param u8DmxId        \b IN: the demux filer Id
/// @param pu8ScmSts     \b OUT: TS scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_TsScmbSts(MS_U8 u8DmxId, MS_U8* pu8ScmSts);

//-----------------
// TSP_STB
//-----------------
// ---------------- [NOTE] API for TSP Ver2.0 HW architecture.-------------
//-------------------------------------------------------------------------------------------------
/// Open a demux filter and attach to a existed filter and buffer
/// @ingroup TSP_STB
/// @param DmxFltType         \b IN: the filter information to allocate
/// @param pu8DmxId           \b OUT: the available demux filer Id
/// @param u8TargetDmxId    \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only use on TSP 2.0 (STB)
/// @note API for U4 TSP N filter to 1 Buffer HW architecture.
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Open_MultiFlt( DMX_FILTER_TYPE DmxFltType , MS_U8* pu8DmxId, MS_U8 u8TargetDmxId);
//-------------------------------------------------------------------------------------------------
/// Enable drop section filter packet.
/// @ingroup TSP_STB
/// @param bSet          \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only use on TSP 2.0 (STB)
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Drop_Enable(MS_BOOL bSet);
//--------------------------------------------------------------------------------

//-----------------
// TSP_SecFilter
//-----------------
//-------------------------------------------------------------------------------------------------
/// Reset demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReset(MS_U8 u8DmxId);
//-------------------------------------------------------------------------------------------------
/// Set the read pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param Read              \b IN: the read pointer to be set
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReadSet(MS_U8 u8DmxId, MS_PHY Read);
//-------------------------------------------------------------------------------------------------
/// Get the read pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pRead            \b OUT: the pointer to store the obtained read pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectReadGet(MS_U8 u8DmxId, MS_PHY* pRead);
//-------------------------------------------------------------------------------------------------
/// Get the write pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pWrite            \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectWriteGet(MS_U8 u8DmxId, MS_PHY* pWrite);
//-------------------------------------------------------------------------------------------------
/// Get the start pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pStart             \b OUT: the pointer to store the obtained start pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectStartGet(MS_U8 u8DmxId, MS_PHY* pStart);
//-------------------------------------------------------------------------------------------------
/// Get the end pointer of demux filer buffer.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pEnd              \b OUT: the pointer to store the obtained end pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectEndGet(MS_U8 u8DmxId, MS_PHY* pEnd);
//-------------------------------------------------------------------------------------------------
/// Set demux filer match patterns.
/// @ingroup TSP_SecFilter
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pPattern         \b IN: the match patterns
/// @param pMask            \b IN: the bit mask for match patterns
/// @param pu8NotMask    \b IN: the bit mask for negative match patterns
/// @param MatchSize       \b IN: the size in bytes for match patterns.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note The match pattern size is 16 bytes
/// @note       match mask      -- must set 1 to be compare (customer request)
/// @note       not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SectPatternSet(MS_U8 u8DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize);

//-----------------
// TSP_TTX
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get Teltext section buffer write address.
/// @ingroup TSP_TTX
/// @param u8DmxId        \b IN: the target demux filer index
/// @param pWrite            \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Special case for TTX due to it can't use HW lock to do indirect read/write. (co-processer)
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_TTX_WriteGet(MS_U8 u8DmxId, MS_PHY* pWrite);

//-------------------------------------------------------------------------------------------------
/// Check can access TSP or not due to different CPU co-process. (HK and AEON)
/// @ingroup TSP_TTX
/// @param u32Try        \b IN: TRUE: check, FALSE: not check
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_GetAccess(MS_U32 u32Try); //OBSOLETE
//-------------------------------------------------------------------------------------------------
/// Unlock HW semaphore.
/// @ingroup TSP_TTX
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_ReleaseAccess(void); //OBSOLETE

//-----------------
// TSP_FIFO
//-----------------
//--------------------------------------------------------------------------------------------------
/// Reset AVFIFO.
/// @ingroup TSP_FIFO
/// @param  DmxFltType    \b IN: demux type
/// @param  bReset           \b IN: section buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE DmxFltType, MS_BOOL bFlush);

//--------------------------------------------------------------------------------------------------
/// Get AVFIFO status (Level).
/// @ingroup TSP_FIFO
/// @param  DmxFltType      \b IN: demux type
/// @param  u32FifoLevel     \b OUT: fifo level
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE DmxFltType, DMX_FIFO_STATUS* u32FifoLevel);
//--------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets.
/// @ingroup DMX_ToBeRemove
/// @param  bEnable          \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note It will apply to all FIFO.
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_RemoveDupAVPkt(MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V Fifo packets.
/// @ingroup TSP_FIFO
/// @param  DmxFltType    \b IN: demux type
/// @param  bEnable          \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_RemoveDupAVFifoPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable/Disable Audio AB S
/// @ingroup TSP_FIFO
///@param bEnable           \b IN: Enable/Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_AU_BD_Mode_Enable(MS_BOOL bEnable);

//--------------------------------------------------------------------------------------------------
/// Enable/Disable AV FIFO block mechanism.
/// @ingroup TSP_FIFO
/// @param  DmxFltType    \b IN: demux type
/// @param  bEnable          \b OUT: TRUR: Enable block mechanism, FALSE: Disable block mechanism
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable);

// PVR Record API (Old)
//-------------------------------------------------------------------------------------------------
/// Set up the recording parameters.
/// @ingroup DMX_ToBeRemove
/// @param pInfo      \b IN: the recording parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Open(DMX_Pvr_info* pPvrInfo);
//-------------------------------------------------------------------------------------------------
/// Clear the recording parameters.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Close(void);
//-------------------------------------------------------------------------------------------------
/// Pause/Resume PVR engine.
/// @ingroup DMX_ToBeRemove
/// @param bPause          \b IN: TRUE: pause, FALSE: resume
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS MApi_DMX_Pvr_Pause(MS_BOOL bPause);
//-------------------------------------------------------------------------------------------------
/// Set the PID to be recorded.
/// @ingroup DMX_ToBeRemove
/// @param Pid                 \b IN: The target PID to be recorded
/// @param pu8DmxId      \b OUT: The demux filter Id for this recorded PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Pid_Open(MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID to be recorded.
/// @ingroup DMX_ToBeRemove
/// @param u8DmxId        \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record.
/// @ingroup DMX_ToBeRemove
/// @param bPvrAll           \b IN: TRUE to record original bit stream, FALSE to recording the PIDs of interests
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Start(MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop recording.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Stop(void);

//-------------------------------------------------------------------------------------------------
/// Get write pointer of recording buffer.
/// @ingroup DMX_ToBeRemove
/// @param pphyWrite       \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_WriteGet(MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set recording packet mode.
/// @ingroup DMX_ToBeRemove
/// @param bSet               \b IN: TRUE: enable 192 mode, FALSE: disable 192 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PVR_SetPacketMode(MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set recording starting time stamp.
/// @ingroup DMX_ToBeRemove
/// @param u32Stamp       \b IN: Record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_SetRecordStamp(MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get current recording time stamp.
/// @ingroup DMX_ToBeRemove
/// @param pu32Stamp     \b OUT: the pointer to store the obtained record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_GetRecordStamp(MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt.
/// @ingroup DMX_ToBeRemove
/// @param bEnable          \b IN: PVR MOBF Enable/Disable
/// @param u32key0         \b IN: MOBF encrypt key0
/// @param u32key1         \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS MApi_DMX_Pvr_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);

//PVR Playback API (Old)
//-------------------------------------------------------------------------------------------------
/// Set playback time stamp.
/// @ingroup TSP_FILEIN_PVR
/// @param u32Stamp        \b IN: time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_SetPlaybackStamp(MS_U32 u32Stamp);
//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @ingroup TSP_FILEIN_PVR
/// @param u32Stamp        \b OUT: time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_GetPlaybackStamp(MS_U32* pu32Stamp);
//-------------------------------------------------------------------------------------------------
/// Enable filein time stamp. (filein use 192 mode)
/// @ingroup DMX_ToBeModified_TSP
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_Pvr_TimeStampDisable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_TimeStampEnable(void);
//-------------------------------------------------------------------------------------------------
/// Disable filein time stamp. (filein not use 192 mode)
/// @ingroup DMX_ToBeModified_TSP
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_Pvr_TimeStampEnable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_TimeStampDisable(void);


//-----------------
// TSP_PVR_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set up the recording parameters.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @param pInfo             \b IN: the recording parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Open(DMX_PVR_ENG Eng ,DMX_Pvr_info* pPvrInfo);

//-------------------------------------------------------------------------------------------------
/// Clear the recording parameters.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Close(DMX_PVR_ENG Eng );

//-------------------------------------------------------------------------------------------------
/// Pause/Resume PVR engine.
/// @ingroup TSP_PVR_General
/// @param Eng               \b IN: PVR engine ID
/// @param bPause          \b IN: TRUE: pause, FALSE: resume
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pause(DMX_PVR_ENG Eng ,MS_BOOL bPause);

//-------------------------------------------------------------------------------------------------
/// Set the PID to be recorded.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param Pid                 \b IN: The target PID to be recorded
/// @param pu8DmxId      \b OUT: The demux filter Id for this recorded PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID to be recorded.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param u8DmxId        \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng ,MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param bPvrAll           \b IN: TRUE to record original bit stream, FALSE to recording the PIDs of interests
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Start(DMX_PVR_ENG Eng ,MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop recording.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Stop(DMX_PVR_ENG Eng );

//-------------------------------------------------------------------------------------------------
/// Get write pointer of recording buffer.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param pphyWrite       \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_ENG Eng ,MS_PHY* pphyWrite);

//-------------------------------------------------------------------------------------------------
/// Set recording packet mode.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param bSet               \b IN: TRUE: enable 192 mode, FALSE: disable 192 mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetPacketMode(DMX_PVR_ENG Eng ,MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set recording starting time stamp.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param u32Stamp       \b IN: Record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetRecordStamp(DMX_PVR_ENG Eng ,MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get current recording time stamp.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param pu32Stamp     \b OUT: the pointer to store the obtained record time-stamp (LPCR)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_ENG Eng ,MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt.
/// @ingroup TSP_PVR_General
/// @param Eng                 \b IN: PVR engine ID
/// @param bEnable          \b IN: PVR MOBF Enable/Disable
/// @param u32key0         \b IN: MOBF encrypt key0
/// @param u32key1         \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1);

//-------------------------------------------------------------------------------------------------
/// Check if PVR engine is started.
/// @ingroup TSP_PVR_General
/// @param Eng                 \b IN: PVR engine ID
/// @param pbIsStart         \b OUT: Pointer to store PVR engine starting status
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_IsStart(DMX_PVR_ENG Eng, MS_BOOL *pbIsStart);
//-----------------
// TSP_PVR_STB
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set/Get PVR PID filter PID.
/// @ingroup TSP_PVR_STB
/// @param Eng                 \b IN: PVR engine ID
/// @param u8DmxId         \b IN: PID filter index
/// @param Pid                  \b IN: PID value
/// @param bSet                \b IN: TRUE: set, FALSE: get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only TSP 2.0/4.0 use this function now.
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_Pid(DMX_PVR_ENG Eng ,MS_U8 u8DmxId , MS_U32 *Pid, MS_BOOL bSet);
//-------------------------------------------------------------------------------------------------
/// Set PVR time-stamp clock.
/// @ingroup TSP_PVR_General
/// @param Eng                \b IN: PVR engine ID
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetRecordStampClk(DMX_PVR_ENG Eng, DMX_TimeStamp_Clk eClk);
//-------------------------------------------------------------------------------------------------
/// Set RASP callback engine size.
/// @ingroup TSP_PVR_STB
/// @param Eng                \b IN: PVR engine ID
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Only TSP 2.0 use this function now.
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_CallbackSize(DMX_PVR_ENG Eng, MS_U32* pu32CallbackSize, MS_BOOL bSet);
//-------------------------------------------------------------------------------------------------
///Set PVR CA SPS enable mode
/// @ingroup TSP_PVR_STB
/// @param Eng                  \b IN: PVR Engine ID
/// @param eCaMode          \b IN: CA PVR Selection, 0: from TSP, 1: from ca program0, 2: from ca program1
/// @param bspsEnable       \b IN: SPS mode enabled flag
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetCaMode(DMX_PVR_ENG Eng, DMX_CA_PVRMODE eCaMode, MS_BOOL bspsEnable);

//-------------------------------------------------------------------------------------------------
/// Set playback time-stamp clock.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                \b IN: PVR engine ID
/// @param eClk               \b IN: select 90K or 27M
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Pvr_Eng_SetPlaybackStampClk(MS_U8 u8Eng, DMX_TimeStamp_Clk eClk);

//PVR CA API (Old)
//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID.
/// @ingroup DMX_ToBeRemove
/// @param Pid                         \b IN: PID to record
/// @param pu8DmxId              \b IN: Pointer to store PVR PID filter index
/// @param u8ShareKeyType     \b IN: PID filter share key type. 0: Not use sharekey filters.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Pid_Open(MS_U32 Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID.
/// @ingroup DMX_ToBeRemove
/// @param u8DmxId         \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id.
/// @ingroup DMX_ToBeRemove
/// @param bPvrAll             \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Start(MS_BOOL bPvrAll);
//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have engine parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Stop(void);

//-----------------
// TSP_PVR_CA
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID.
/// @ingroup TSP_PVR_CA
/// @param Eng                        \b IN: PVR engine ID
/// @param Pid                         \b IN: PID to record
/// @param pu8DmxId              \b IN: Pointer to store PVR PID filter index
/// @param u8ShareKeyType     \b IN: PID filter share key type. 0: Not use sharekey filters.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType);
//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID.
/// @ingroup TSP_PVR_CA
/// @param Eng                 \b IN: PVR engine ID
/// @param u8DmxId         \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId);
//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id.
/// @ingroup TSP_PVR_CA
/// @param Eng                  \b IN: PVR engine ID
/// @param bPvrAll             \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);
//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream.
/// @ingroup TSP_PVR_CA
/// @param Eng                  \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_ENG Eng);
//---------------------------------------------

//-----------------
// TSP_FILEIN_PVR
//-----------------
//-------------------------------------------------------------------------------------------------
/// Open & set pid filter for file-in PVR using.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                             \b IN   : PVR eigine id
/// @param Pid                              \b IN   : PVR PID to record
/// @param pu8DmxId                   \b OUT: Pointer to store PID filter index
/// @param u8ShareKeyType          \b IN   : The flag of sharekey filter using. if 0, not use sharekey filters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType);

//-------------------------------------------------------------------------------------------------
/// Close pid filter of file-in PVR.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                        \b IN   : PVR eigine id
/// @param u8DmxId                \b IN   : PID filter id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Start file-in PVR recording.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                         \b IN   : PVR eigine id
/// @param bPvrAll                    \b IN   : FALSE is record PID, TRUE is record All ts data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll);

//-------------------------------------------------------------------------------------------------
/// Stop file-in PVR recording.
/// @ingroup TSP_FILEIN_PVR
/// @param Eng                         \b IN   : PVR eigine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Pvr_Eng_Stop(DMX_PVR_ENG Eng);
//---------------------------------------------


// File-in API (Old)
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream from memory.
/// @ingroup DMX_ToBeRemove
/// @param Dst               \b IN: file in destination type
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Start(DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);   // msAPI_DMX_PESPlayback/msAPI_DMX_TSPlayback will integrate into it
//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream from memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Stop(void);

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup DMX_ToBeRemove
/// @param pFileinInfo      \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Info(DMX_Filein_info *pFileinInfo);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Pause(void);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Resume(void);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsIdle(void);

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsBusy(void);

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused.
/// @ingroup DMX_ToBeRemove
/// @return TRUE - if idle
/// @return FALSE - otherwise
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
MS_BOOL           MApi_DMX_Filein_IsPause(void);

//-------------------------------------------------------------------------------------------------
/// Reset CMDQ.
/// @ingroup DMX_ToBeRemove
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Get the number of CMDQ empty slot.
/// @ingroup DMX_ToBeRemove
/// @param pu32EmptySlot      \b OUT: empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_GetEmptyNum(MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Get Command queue FIFO level.
/// @ingroup DMX_ToBeRemove
/// @param  pu8CmdQStatus   \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup DMX_ToBeRemove
/// @param bbypass     \b IN: TRUE: bypass file-in timestamp, FALSE: not bypass
/// @return None
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
void              MApi_DMX_BypassFileInTimeStamp(MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Get Command queue FIFO level.
/// @ingroup DMX_ToBeRemove
/// @param  pu8CmdQStatus   \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Get filein time stamp. (Current time stamp from TS packet). It is used on 192 packet mode.
/// @ingroup DMX_ToBeRemove
/// @param pu32FileInTS     \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetFileInTimeStamp(MS_U32 *pu32FileInTS);

//-------------------------------------------------------------------------------------------------
/// Get playback read address.
/// @ingroup DMX_ToBeRemove
/// @param pphyRead        \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_GetReadAddr(MS_PHY* pphyRead);

//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key.
/// @ingroup DMX_ToBeRemove
/// @param bEnable    \b IN: File-in MOBF Enable/Disable
/// @param u32key     \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Keep the function which have path parameter.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key);

//-----------------
// TSP_Playback
//-----------------
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream from memory.
/// @ingroup TSP_Playback
/// @param ePath            \b IN: file in path (TSIF)
/// @param Dst               \b IN: file in destination type
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Start(DMX_FILEIN_PATH ePath, DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize);   // msAPI_DMX_PESPlayback/msAPI_DMX_TSPlayback will integrate into it
//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream from memeory.
/// @ingroup TSP_Playback
/// @param ePath            \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Stop(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @param pFileinInfo      \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Info(DMX_FILEIN_PATH ePath, DMX_Filein_info *pFileinInfo);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Pause(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Eng_Resume(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DMX_Filein_Eng_IsIdle(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
//  MS_BOOL MApi_DMX_Filein_Eng_IsBusy(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
//  MS_BOOL MApi_DMX_Filein_Eng_IsPause(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Reset CMDQ.
/// @ingroup TSP_Playback
/// @param ePath             \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_Reset(DMX_FILEIN_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Get the number of CMDQ empty slot.
/// @ingroup TSP_Playback
/// @param ePath                   \b IN: file in path (TSIF)
/// @param pu32EmptySlot      \b OUT: empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(DMX_FILEIN_PATH ePath, MS_U32 *pu32EmptySlot);
//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @param bbypass     \b IN: TRUE: bypass file-in timestamp, FALSE: not bypass
/// @return None
//-------------------------------------------------------------------------------------------------
void MApi_DMX_Filein_Eng_BypassFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Get Command queue FIFO level.
/// @ingroup TSP_Playback
/// @param ePath                   \b IN: file in path (TSIF)
/// @param  pu8CmdQStatus   \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel(DMX_FILEIN_PATH ePath, MS_U8 *pu8CmdQStatus);
//-------------------------------------------------------------------------------------------------
/// Get filein time stamp. (Current time stamp from TS packet). It is used on 192 packet mode.
/// @ingroup TSP_Playback
/// @param ePath                \b IN: file in path (TSIF)
/// @param pu32FileInTS     \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_U32 *pu32FileInTS);

//-------------------------------------------------------------------------------------------------
/// Get playback read address.
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param pphyRead        \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetReadAddr(DMX_FILEIN_PATH ePath, MS_PHY* pphyRead);
//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key.
/// @ingroup TSP_Playback
/// @param ePath        \b IN: file in path (TSIF)
/// @param bEnable    \b IN: File-in MOBF Enable/Disable
/// @param u32key     \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Filein_Eng_MOBF_Enable(DMX_FILEIN_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);

//-------------------------------------------------------------------------------------------------
/// Enable FileEng Playback time stamp.
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Disable fileEng Playback time stamp.
/// @ingroup TSP_Playback
/// @param ePath         \b IN: file in path (TSIF)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(DMX_FILEIN_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Set playback time stamp. (LPCR)
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param u32Stamp       \b IN: time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_SetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32 u32Stamp);
//-------------------------------------------------------------------------------------------------
/// Get playback time stamp. (LPCR)
/// @ingroup TSP_Playback
/// @param ePath              \b IN: file in path (TSIF)
/// @param pu32Stamp      \b OUT: pointer to store time-stamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Filein_Eng_GetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32* pu32Stamp);


//-------------------------------------------------------------
//MMFI & MINITSP API
//-----------------
// TSP_MM
//-----------------
//-------------------------------------------------------------------------------------------------
/// Check mmfi engine is ilde or not.
/// @ingroup TSP_MM
/// @param  ePath     \b IN: MM File-in Path
/// @return TRUE - Idle
/// @return FALSE - not idle
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DMX_MMFI_Filein_IsIdle(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Check mmfi engine is busy or not.
/// @ingroup TSP_MM
/// @param  ePath      \b IN: MM File-in Path
/// @return TRUE - Busy
/// @return FALSE - not busy
//-------------------------------------------------------------------------------------------------
//  MS_BOOL MApi_DMX_MMFI_Filein_IsBusy(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Reset MMFilein Command queue
/// @ingroup TSP_MM
/// @param  ePath       \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_Reset(DMX_MMFI_PATH ePath);

//-------------------------------------------------------------------------------------------------
/// Get the MMFilein CMDQ empty slot number.
/// @ingroup TSP_MM
/// @param  ePath                  \b IN: MM File-in Path
/// @param  pu32EmptySlot     \b OUT: Empty slot
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(DMX_MMFI_PATH ePath, MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory.
/// @ingroup TSP_MM
/// @param eDst             \b IN: file in destination path
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_Start(DMX_MMFI_DST eDst, MS_PHY pBuf, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory.
/// @ingroup TSP_MM
/// @param  ePath           \b IN: MM File-in Path
/// @param pFileinInfo     \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_Info(DMX_MMFI_PATH ePath, DMX_Filein_info *pFileinInfo);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @ingroup TSP_MM
/// @param  ePath       \b IN: MM File-in Path
/// @param bbypass    \b IN: If true, bypass file-in timestamp.
/// @return None
//-------------------------------------------------------------------------------------------------
//  void MApi_DMX_MMFI_Filein_BypassTimeStamp(DMX_MMFI_PATH ePath, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein.
/// @ingroup TSP_MM
/// @param  ePath                \b IN: MM File-in Path
/// @param pu32FileInTS       \b OUT: pointer for timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_GetFileInTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32FileInTS);

//MMFI  (MMFI Only) API
//-------------------------------------------------------------------------------------------------
/// Set the PID to be MM File-in
/// @ingroup TSP_MM
/// @param flttype         \b IN: MMFI filter type
/// @param u16Pid        \b IN: The target PID for MM Filein
/// @param pu8DmxId   \b OUT: The demux filter Id for this MMFilein PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Pid_Open(DMX_MMFI_FLTTYPE flttype, MS_U16 u16Pid, MS_U8* pu8DmxId);

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein.
/// @ingroup TSP_MM
/// @param u8DmxId      \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Pid_Close(MS_U8 u8DmxId);

//-------------------------------------------------------------------------------------------------
/// Get MMFilein Command queue fifo level.
/// @ingroup TSP_MM
/// @param  ePath                   \b IN: MM File-in Path
/// @param  pu8CmdQStatus    \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(DMX_MMFI_PATH ePath, MS_U8 *pu8CmdQStatus);

//-------------------------------------------------------------------------------------------------
/// Set MMFI playback timestamp.
/// @ingroup TSP_MM
/// @param  ePath          \b IN: MM File-in Path
/// @param u32pcr2       \b IN: LPCR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32pcr2);

//-------------------------------------------------------------------------------------------------
/// Get MMFI Playback timestamp.
/// @ingroup TSP_MM
/// @param ePath               \b IN: MM File-in Path
/// @param pu32pcr2          \b OUT: pointer to store LCPR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_GetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32pcr2);

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets.
/// @ingroup TSP_MM
/// @param  bEnable           \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_RemoveDupAVPkt(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// Enable MMFI timestamp mode.
/// @ingroup DMX_ToBeModified_MM
/// @param ePath               \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_MMFI_TimeStampDisable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampEnable(DMX_MMFI_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Disable MMFI timestamp mode.
/// @ingroup DMX_ToBeModified_MM
/// @param ePath                \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note:  Merge with MApi_DMX_MMFI_TimeStampEnable
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampDisable(DMX_MMFI_PATH ePath);
//-------------------------------------------------------------------------------------------------
/// Set MMFI playback timestamp.
/// @ingroup DMX_ToBeRemove
/// @param  ePath           \b IN: MM File-in Path
/// @param u32pcr2        \b IN: LPCR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Duplicate function.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MMFI MOBF function and set MOBF key index.
/// @ingroup TSP_MM
/// @param ePath               \b IN: MM File-in Path
/// @param  bEnable           \b IN: Enable or Disable
/// @param  u32key            \b IN: MOBF key index
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_Enable(DMX_MMFI_PATH ePath, MS_BOOL bEnable, MS_U32 u32key);

//-------------------------------------------------------------------------------------------------
/// Set MMFI MOBF level.
/// @ingroup TSP_MM
/// @param ePath              \b IN: MM File-in Path
/// @param  u8level           \b IN: the value of MOBF level
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note only used on TSP 1.0
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_SetLevel(DMX_MMFI_PATH epath, MS_U8 u8level);
//-------------------------------------------------------------------------------------------------
/// Set MMFI MOBF level.
/// @ingroup TSP_MM
/// @param ePath              \b IN: MM File-in Path
/// @param  u8level           \b IN: the value of MOBF level
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note only used on TSP 1.0
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_MMFI_TimeStampClk(DMX_MMFI_PATH ePath, DMX_TimeStamp_Clk eClk);

//-------------------------------------------------------------


//-----------------
// TSO_General
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng          \b IN: TSO engine ID
/// @param pFileinInfo    \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Info(MS_U8 u8Eng, DMX_Filein_info *pFileinInfo);

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going of TSO path
/// @ingroup TSO_General
/// @param u8Eng       \b IN: TSO engine ID
/// @return TRUE - if idle
/// @return FALSE - otherwise
//-------------------------------------------------------------------------------------------------
// MS_BOOL SYMBOL_WEAK MApi_DMX_TSO_Filein_IsIdle(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ empty number.
/// @ingroup TSO_General
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32EmptySlot     \b OUT: pointer to store CMDQ empty slot number
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum(MS_U8 u8Eng, MS_U32 *pu32EmptySlot);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ reset.
/// @ingroup TSO_General
/// @param u8Eng           \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_CMDQ_Reset(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng            \b IN: TSO engine ID
/// @param pBuf              \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Start(MS_U8 u8Eng, MS_PHY pBuf, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory of TSO path.
/// @ingroup TSO_General
/// @param u8Eng            \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_Stop(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Set TSO playback time stamp. (LPCR)
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param u32Stamp       \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp. (LPCR)
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param pu32Stamp      \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_GetPlaybackStamp(MS_U8 u8Eng, MS_U32* pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp.
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param pu32Stamp      \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32 *pu32Stamp);

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode block scheme to bypass fill-in timestamp.
/// @ingroup TSO_General
/// @param u8Eng           \b IN: TSO engine id.
/// @param bbypass        \b IN: If true, bypass file-in timestamp.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_BypassFileInTimeStamp(MS_U8 u8Eng, MS_BOOL bbypass);
//-------------------------------------------------------------------------------------------------
/// Enable TSO file in  time stamp.
/// @ingroup DMX_ToBeModified_MM_TSO
/// @param u8Eng           \b IN: TSO engine id.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_TimeStampEnable(MS_U8 u8Eng);

//-------------------------------------------------------------------------------------------------
/// Disable TSO file in time stamp
/// @ingroup DMX_ToBeModified_MM_TSO
/// @param u8Eng           \b IN: TSO engine id.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_TimeStampDisable(MS_U8 u8Eng);
//-------------------------------------------------------------------------------------------------
/// Set TSO Out Clock.
/// @ingroup TSO_General
/// @param  u8Eng                   \b IN: TSO engine ID
/// @param  eTsOutClk             \b IN: TSO out clock select
/// @param  eTsOutClkSrc        \b IN: TSO out clock source select
/// @param  u16DivNum           \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv                \b IN: If Out Clock invert
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_DIV2N, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_172M_2N/E_DMX_TSO_OUTCLKSRC_288M_2N/E_DMX_TSO_OUTCLKSRC_432M_2N,
/// @note and user should set u16DivNum to generate final output clock.
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_DIVN, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_216M_N, and user should set u16DivNum to generate final output clock.
/// @note If eTsOutClk is E_DMX_TSO_OUTCLK_PTSOOUT/E_DMX_TSO_OUTCLK_PTSOOUT_DIV8, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_P_TS0IN/E_DMX_TSO_OUTCLKSRC_P_TS1IN, etc,
/// @note and this colck is from external pad
/// @note E_DMX_TSO_OUTCLK_62M/E_DMX_TSO_OUTCLK_54M/E_DMX_TSO_OUTCLK_27M/E_DMX_TSO_OUTCLK_Dmd are const
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SetOutClk(MS_U8 u8Eng, DMX_TSO_OutClk eTsOutClk, DMX_TSO_OutClkSrc eTsOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv);

//-------------------------------------------------------------------------------------------------
/// Enable or disable TSO output.
/// @ingroup TSO_General
/// @param  u8Eng                  \b IN: TSO engine ID
/// @param  bEnable               \b IN: TRUE is enable, FALSE is disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable);

//TSO2 API
//-------------------------------------------------------------------------------------------------
/// Set or get local stream ID of TSO Out.
/// @ingroup TSO_General
/// @param  u8Eng           \b IN: TSO engine ID
/// @param  eIf                \b IN: TSO input TSIF
/// @param  pu8StrId       \b IN or OUT: pointer to store local stream ID, default value is 0x47
/// @param  bSet             \b IN: If TRUE, set local stream id, otherwise get local stream id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_LocalStreamId(MS_U8 u8Eng, DMX_TSIF eIf, MS_U8* pu8StrId, MS_BOOL bSet);

//-------------------------------------------------------------------------------------------------
/// Set TSO virtual queue buffer.
/// @ingroup TSO_General
/// @param  u8Eng           \b IN: TSO engine ID
/// @param  u32Addr        \b IN: TSO VQ buffer address
/// @param  u32BufSize    \b IN: TSO VQ buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_PHY phyAddr, MS_U32 u32BufSize);

//-------------------------------------------------------------------------------------------------
///TSO (TS output) Demux Flow input sources configure getting and setting.
/// @ingroup TSO_General
/// @param eFlow                         \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
/// @param eTSOInIf                    \b IN: TSO input interface,  DMX_TSIF_LIVE0/DMX_TSIF_LIVE1/DMX_TSIF_LIVE2/DMX_TSIF_FILE0/DMX_TSIF_FILE1
/// @param stInputInfo.Input         \b IN/OUT: TSO input interface source,  enum item of DMX_FLOW_INPUT
/// @param stInputInfo.bClkInv     \b IN/OUT: If TSO input interface source is from demod, please set clock invert type
/// @param stInputInfo.bExtSync   \b IN/OUT: If TSO input interface source is from demod, please set external sync type
/// @param stInputInfo.bParallel    \b IN/OUT: If TSO input interface source is from demod, please set parallel or serial type
/// @param u8LocalStrId               \b IN/OUT: The local stream ID (TS output sync byte) value, default is 0x47
/// @param bBypassAll                  \b IN/OUT: TRUE means bypass all ts data for TSO playback; FALSE means tso will output ts data by PIDs
/// @param bEnable                      \b IN/OUT: TRUE means enable TSO input now; FALSE means disable this TSO input fource
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note This API is for TSO 2.0 HW architecture.
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring one of TSO input.
/// @note If there are 3 TSO input source, you shold call this API 3 times for every TSO input path configure.
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg);

//-------------------------------------------------------------------------------------------------
/// TSO (TS output) Demux Flow output path configure
/// @ingroup TSO_General
/// @param eFlow                        \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
/// @param eOutPad                    \b IN: TSO output pad  select,  DMX_FLOW_OUTPUT_EXT_PAD1/DMX_FLOW_OUTPUT_EXT_PAD3///
/// @param u16OutPktSize            \b IN/OUT: TSO output packet size. default vale is 188 bytes
/// @param bEnable                     \b IN/OUT: TRUE means enable TSO output now; FALSE means disable TSO output
/// @param bDefOutClk                \b IN: TURE means using default clock setting given by driver; FALSE means set output clock by user parameters
/// @param bOutClkInv                \b IN/OUT: Set inver type of TSO output clock
/// @param eTsoOutClk                \b IN/OUT: Select TSO output clock, enum item of DMX_TSO_OutClk
/// @param eTsoOutClkSrc           \b IN/OUT: Select TSO output clock source, enum item of DMX_TSO_OutClkSrc
/// @param u16DivNum                \b IN/OUT: If TSO output clock source is E_DMX_TSO_OUTCLK_DIV2N/E_DMX_TSO_OUTCLK_DIVN, set this value for clock generatng.
/// @param bSet                          \b IN: TURE: set, FALSE: get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note This API is for TSO 2.0 HW architecture.
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring TSO output path.
/// @note TSO output clock source selection, please also refer to MApi_DMX_TSO_SetOutClk API.
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg);
//-------------------------------------------------------------------------------------------------
/// TSO (TS output) input pid filter open.
/// @ingroup TSO_General
/// @param u8Eng              \b IN: TSO engine ID
/// @param eTSOInSrc       \b IN: TSO PID filter source
/// @param u16Pid             \b IN: PID value of TSO PID filter
/// @param pu16DmxId      \b IN: Pointer to store PID filter ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Pid_Open(MS_U8 u8Eng, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId);

//-------------------------------------------------------------------------------------------------
/// TSO (TS output) input pid filter close.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param u16DmxId       \b IN: TSO PID filter source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Pid_Close(MS_U8 u8Eng, MS_U16 u16DmxId);
//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  read address.
/// @ingroup TSO_General
/// @param u8Eng             \b IN: TSO engine ID
/// @param pu32Read       \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_PHY* pphyRead);
//----------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/// Get TSP firmware version.
/// @ingroup TSP_General
/// @param u32FWVer      \b OUT: TSP firmware version infomation
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
// DMX_FILTER_STATUS MApi_TSP_Get_FW_VER(MS_U32* u32FWVer);
//-------------------------------------------------------------------------------------------------
/// Set TSP Debug Level.
/// @ingroup TSP_General
/// @param  u16DbgSwitch    \b IN: debug level.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_SetDbgLevel(DMX_DBGMSG_LEVEL level);

//-----------------
// TSP_Debug
//-----------------
//-------------------------------------------------------------------------------------------------
/// Get interrupt count.
/// @ingroup TSP_Debug
/// @param pu32Count       \b OUT: interrupt count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Get_Intr_Count(MS_U32* pu32Count);
//--------------------------------------------------------------------------------------------------
/// Get TSP driver library information
/// @ingroup TSP_General
/// @param ppVersion      \b OUT: TSP library version infomation
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetLibVer(const MSIF_Version **ppVersion);

//-------------------------------------------------------------------------------------------------
/// Get the register value of HW debug port .
/// @ingroup TSP_Debug
/// @param u32DbgSel       \b IN: Set which HW debug info you want to get. (ex: 0x4F, check TSIF0 data info)
/// @param u32DbgInfo      \b OUT: pointer to store the debug port value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo);
//-------------------------------------------------------------------------------------------------
/// Set memory buffer information for TSP AEON to print debug message.
/// @ingroup TSP_Debug
/// @param phyAddr         \b IN: physical address of buffer
/// @param u32Size          \b IN: size of buffer
/// @param u32DbgWord   \b IN: control word to filter debug message
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_SetFwDbgParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord);
//-------------------------------------------------------------------------------------------------
/// Get dis-continue count by ts source and specific FIFO.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_DisContiCnt(DMX_DisContiCnt_info* pDmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
/// Get drop packet count by ts source and specific FIFO.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: drop packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_DropPktCnt(DMX_DropPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);

//-------------------------------------------------------------------------------------------------
///Get locked packet count by ts source and specific FIFO.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: Lock packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_LockPktCnt(DMX_LockPktCnt_info* pDmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
///Get dis-continue count by ts source and specific FIFO.
/// @ingroup TSP_Debug
/// @param pDmxInfo          \b IN: set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: AV packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_Get_AVPktCnt(DMX_AVPktCnt_info* DmxInfo, MS_U32* pu32Cnt);
//-------------------------------------------------------------------------------------------------
/// Get section TEI packet count by ts source.
/// @ingroup TSP_Debug
/// @param FltSrc               \b IN: TS source
/// @param pu32PktCnt      \b OUT: TEI packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Get_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc, MS_U32* pu32PktCnt);
//-------------------------------------------------------------------------------------------------
/// Reset section TEI packet count by DMX filter index.
/// @ingroup TSP_Debug
/// @param FltSrc               \b IN: TS source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Reset_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc);
//-------------------------------------------------------------------------------------------------
/// Get section dis-continue packet count.
/// @ingroup TSP_Debug
/// @param u32DmxID        \b IN: DMX filter index
/// @param pu32PktCnt       \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Get_SecDisCont_PktCount(MS_U32 u32DmxID, MS_U32* pu32PktCnt);
//-------------------------------------------------------------------------------------------------
/// Reset section dis-continue packet count.
/// @ingroup TSP_Debug
/// @param u32DmxID      \b IN: DMX filter Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS MApi_DMX_Reset_SecDisCont_PktCount(MS_U32 u32DmxID);
//-----------------------------------------------------

//-----------------
// TSP_FIQ
//-----------------
//-------------------------------------------------------------------------------------------------
/// Enable/Disable section filter rush pass.
/// @ingroup TSP_FIQ
/// @param u32DmxID      \b IN: DMX filter Id
/// @param u8Enable        \b IN: TRUE: enable, FALSE: disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note Now only TSP 2.0 & TSP 4.0 support it.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_SetFltRushPass(MS_U8 u8DmxId, MS_U8 u8Enable);
//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for FIQ Enable.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @param pFQInfo          \b IN: Init FQ Setting
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use FQ, please call this API after calling MApi_DMX_Init. (FQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_Init(MS_U32 u32FQEng, DMX_FQ_Info* pFQInfo);
//-------------------------------------------------------------------------------------------------
/// Finalize Demux API for FIQ.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_Exit(MS_U32 u32FQEng);
//-------------------------------------------------------------------------------------------------
/// Enable FIQ rush function.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_RushEnable(MS_U32 u32FQEng);

//-------------------------------------------------------------------------------------------------
/// Select path to skip rush data.
/// @ingroup TSP_FIQ
/// @param u32FQEng       \b IN: FQ engine id
/// @param eSkipPath       \b IN: Select path to skip rush data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_FQ_SkipRushData(MS_U32 u32FQEng, DMX_FQ_SkipPath eSkipPath);
//-----------------------------------------------------

//-----------------
// TSP_Merge
//-----------------
//-------------------------------------------------------------------------------------------------
/// Set or Get merge stream source id and sync byte
/// @ingroup TSP_Merge
/// @param eIf                    \b IN:  Eunm value of DMX TSIF selection
/// @param u8StrId             \b IN:  Stream index
/// @param pu8SyncByte     \b IN:  Pointer to sync bytearray of merege streams
/// @param bSet                 \b IN:  TRUE to setting data or FALSE to getting table
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note: Currently, maxmum number is 8, and don't call this API when stream processing is started
//-------------------------------------------------------------------------------------------------
//  DMX_FILTER_STATUS SYMBOL_WEAK MApi_DMX_MStr_SyncByte(DMX_TSIF eIf, MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet);
//-----------------------------------------------------


//---  Common Interface for TSP config and Api commands ------------//
//-------------------------------------------------------------------------------------------------
/// General function for set cmd or get infomation.
/// @ingroup TSP_Debug
/// @param u32Cmd          \b IN: DMX filter Id
/// @param u32Config       \b IN: Config 0 (define by case)
/// @param u32DataNum   \b IN: Config 0 (define by case)
/// @param pData             \b OUT: Output data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData);

#if 0
DMX_FILTER_STATUS MApi_DMX_ResetFileinTimestamp(void)
#endif


#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_H__
