////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
// file   halTSP.h
// @brief  TSP HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TSP_H__
#define __HAL_TSP_H__

//--------------------------------------------------------------------------------------------------
//  Macro and Define
//--------------------------------------------------------------------------------------------------
#define HAL_TSP_RET_NULL                0xFFFFFFFF

// PVR define
#define PVR_NUM                         TSP_PVRENG_NUM
#define PVR_PIDFLT_DEF                  0x1fff

//VQ define
#define VQ_NUM                          TSP_TSIF_NUM
#define VQ_PACKET_UNIT_LEN              208

#define TSP_TSIF0                       0x00
#define TSP_TSIF1                       0x01
#define TSP_TSIF2                       0x02
#define TSP_TSIF3                       0x03
#define TSP_TSIF4                       0x04
#define TSP_TSIF5                       0x05
#define TSP_TSIF6                       0x06

//FQ define
#define TSP_FQ_NUM                      11  // exclude FQ #7
#define TSP_FQ_MUX_START_ID             8


//u32Cmd of MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData);
#define HAL_DMX_CMD_RUN_DISABLE_SEC_CC_CHECK 0x00000001 //[u32Config] 1:disable cc check on fw, 0: enable cc check on fw; [u32DataNum,*pData] do not use
//#########################################################################
//#### Software Capability Macro Start
//#########################################################################

#define TSP_CA_RESERVED_FLT_NUM         1
#define TSP_RECFLT_NUM                  1
#define TSP_PIDFLT_REC_NUM              (TSP_PIDFLT_NUM - TSP_PCRFLT_NUM)                           // 0~767 (0 for CA)
                                                                                                    // 777 for Err
                                                                                                    // 776 for REC
                                                                                                    // 768 ~ 775 for PCR0 ~ PCR7

#if HW_PCRFLT_ENABLE
    #define TSP_PIDFLT_NUM_ALL          (TSP_PIDFLT_NUM + STC_ENG_NUM + TSP_RECFLT_NUM)
#else
    #define TSP_PIDFLT_NUM_ALL          (TSP_PIDFLT_NUM + TSP_RECFLT_NUM)
#endif

//#########################################################################
//#### Software Capability Macro End
//#########################################################################

// CA FLT ID (CA HW limitation, the PID Filter "0" must be reserved for CA to connect PID SLOT TABLE.)
#define TSP_CAFLT_START_ID              0
#define TSP_CAFLT_END_ID                (TSP_CAFLT_START_ID + TSP_CA_RESERVED_FLT_NUM)                                     // 1

// section FLT ID
#define TSP_SECFLT_START_ID             TSP_CAFLT_END_ID                                                                   // 1
#define TSP_SECBUF_START_ID             TSP_CAFLT_END_ID                                                                   // 1
#define TSP_SECFLT_END_ID               (TSP_SECFLT_START_ID + TSP_SECFLT_NUM - TSP_CA_RESERVED_FLT_NUM)                   // 192
#define TSP_SECBUF_END_ID               (TSP_SECBUF_START_ID + TSP_SECBUF_NUM - TSP_CA_RESERVED_FLT_NUM)                   // 192

// PID
#define TSP_PIDFLT_START_ID             TSP_CAFLT_END_ID                                                                   // 1
#define TSP_PIDFLT_END_ID               (TSP_PIDFLT_START_ID + TSP_PIDFLT_NUM - TSP_CA_RESERVED_FLT_NUM)                   // 192

// PCR
#define TSP_PCRFLT_START_ID             TSP_PIDFLT_END_ID                                                                  // 192
#define HAL_TSP_PCRFLT_GET_ID(NUM)      (TSP_PCRFLT_START_ID + (NUM))
#define TSP_PCRFLT_END_ID               (TSP_PCRFLT_START_ID + TSP_PCRFLT_NUM)                                             // 196

// REC
#define TSP_RECFLT_IDX                  TSP_PCRFLT_END_ID                                                                  // 196

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  PVR Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// HW characteristic

typedef enum _PVRENG_SEQ
{
    E_TSP_PVR_PVRENG_START          = 0,
    E_TSP_PVR_PVRENG_0              = E_TSP_PVR_PVRENG_START,
    E_TSP_PVR_PVRENG_1,
    E_TSP_PVR_PVRENG_2,
    E_TSP_PVR_PVRENG_3,
    E_TSP_PVR_PVRENG_4,
    E_TSP_PVR_PVRENG_5,
    E_TSP_PVR_PVRENG_6,
    E_TSP_PVR_PVRENG_7,
    E_TSP_PVR_PVRENG_8,
    E_TSP_PVR_PVRENG_9,
    E_TSP_PVR_PVRENG_END,

    E_TSP_PVR_ENG_INVALID,
} PVRENG_SEQ;

typedef enum _FILEENG_SEQ
{
    E_FILEENG_TSIF0                 = TSP_TSIF0,
    E_FILEENG_TSIF1                 = TSP_TSIF1,
    E_FILEENG_TSIF2                 = TSP_TSIF2,
    E_FILEENG_TSIF3                 = TSP_TSIF3,
    E_FILEENG_TSIF4                 = TSP_TSIF4,
    E_FILEENG_TSIF5                 = TSP_TSIF5,
    E_FILEENG_TSIF6                 = TSP_TSIF6,
    E_FILEENG_INVALID,

} FILEENG_SEQ;

// Destination type
typedef enum _TSP_DST_SEQ
{
    E_TSP_DST_FIFO_VIDEO,
    E_TSP_DST_FIFO_VIDEO3D,
    E_TSP_DST_FIFO_VIDEO3,
    E_TSP_DST_FIFO_VIDEO4,
    E_TSP_DST_FIFO_VIDEO5,
    E_TSP_DST_FIFO_VIDEO6,
    E_TSP_DST_FIFO_VIDEO7,
    E_TSP_DST_FIFO_VIDEO8,

    E_TSP_DST_FIFO_AUDIO,
    E_TSP_DST_FIFO_AUDIO2,
    E_TSP_DST_FIFO_AUDIO3,
    E_TSP_DST_FIFO_AUDIO4,
    E_TSP_DST_FIFO_AUDIO5,
    E_TSP_DST_FIFO_AUDIO6,

    E_TSP_DST_INVALID,
} TSP_DST_SEQ;

typedef enum _TSP_SRC_SEQ
{
    E_TSP_SRC_PKTDMX0 = 1,
    E_TSP_SRC_PKTDMX1,
    E_TSP_SRC_PKTDMX2,
    E_TSP_SRC_PKTDMX3,
    E_TSP_SRC_PKTDMX4,
    E_TSP_SRC_PKTDMX5,
    E_TSP_SRC_PKTDMX6,
    E_TSP_SRC_MMFI0,
    E_TSP_SRC_MMFI1,

    E_TSP_SRC_INVALID,
} TSP_SRC_SEQ;

typedef enum _TSIF_CFG
{
    // @NOTE should be Exclusive usage
    E_TSP_TSIF_CFG_DIS      =           0x0000,      // 1: enable ts interface 0 and vice versa oppsite with en
    E_TSP_TSIF_CFG_EN       =           0x0001,
    E_TSP_TSIF_CFG_PARA     =           0x0002,
    E_TSP_TSIF_CFG_SERL     =           0x0000,      // oppsite with Parallel
    E_TSP_TSIF_CFG_EXTSYNC  =           0x0004,
    E_TSP_TSIF_CFG_BITSWAP  =           0x0008,
    E_TSP_TSIF_CFG_3WIRE    =           0x0010
} TSP_TSIF_CFG;

// for stream input source
typedef enum _HAL_TS_PAD
{
    E_TSP_TS_PAD_EXT0,
    E_TSP_TS_PAD_EXT1,
    E_TSP_TS_PAD_EXT2,
    E_TSP_TS_PAD_EXT3,
    E_TSP_TS_PAD_EXT4,
    E_TSP_TS_PAD_EXT5,
    E_TSP_TS_PAD_EXT6,
    E_TSP_TS_PAD_EXT7,
    E_TSP_TS_PAD_INTER0,
    E_TSP_TS_PAD_INTER1,
    E_TSP_TS_PAD_TSOUT0,
    E_TSP_TS_PAD_TSOUT1,
    E_TSP_TS_PAD_TSIOOUT0,
    E_TSP_TS_PAD_INVALID,
} TSP_TS_PAD;

// for ts pad mode
typedef enum _HAL_TS_PAD_MUX_MODE
{
    E_TSP_TS_PAD_MUX_PARALLEL,      // in
    E_TSP_TS_PAD_MUX_3WIRED_SERIAL, // in
    E_TSP_TS_PAD_MUX_4WIRED_SERIAL, // in
    E_TSP_TS_PAD_MUX_TSO,           // out
    E_TSP_TS_PAD_MUX_S2P,           // out
    E_TSP_TS_PAD_MUX_S2P1,          // out
    E_TSP_TS_PAD_MUX_DEMOD,         // out

    E_TSP_TS_PAD_MUX_INVALID
} TSP_TS_PAD_MUX_MODE;

// for pkt converter mode
typedef enum _HAL_TS_PKT_CONVERTER_MODE
{
    E_TSP_PKT_CONVERTER_188Mode         = 0,
    E_TSP_PKT_CONVERTER_CIMode          = 1,
    E_TSP_PKT_CONVERTER_OpenCableMode   = 2,
    E_TSP_PKT_CONVERTER_ATSMode         = 3,
    E_TSP_PKT_CONVERTER_MxLMode         = 4,
    E_TSP_PKT_CONVERTER_NagraDongleMode = 5,
    E_TSP_PKT_CONVERTER_Invalid,
} TSP_TS_PKT_CONVERTER_MODE;

typedef enum _HAL_TS_MXL_PKT_MODE
{
    E_TSP_TS_MXL_PKT_192         = 4,
    E_TSP_TS_MXL_PKT_196         = 8,
    E_TSP_TS_MXL_PKT_200         = 12,
    E_TSP_TS_MXL_PKT_INVALID,
} TSP_TS_MXL_PKT_MODE;

typedef enum _HAL_TSP_CLK_TYPE
{
    E_TSP_HAL_TSP_CLK,
    E_TSP_HAL_STC_CLK,
    E_TSP_HAL_INVALID
} EN_TSP_HAL_CLK_TYPE;

typedef struct _HAL_TSP_CLK_STATUS
{
    MS_BOOL bEnable;
    MS_BOOL bInvert;
    MS_U8   u8ClkSrc;
} ST_TSP_HAL_CLK_STATUS;

typedef enum _PCR_SRC
{
/*    register setting for kaiser pcr
    0: tsif0
    1: tsif1
    2: tsif2
    3: tsif3
    4: tsif4
    5: tsif5
    6: tsif6
    a: MM file in 1
    b: MM file in 2
    c: FIQ in 1
    d: FIQ in 2
*/
    E_TSP_PCR_SRC_TSIF0 = 0,
    E_TSP_PCR_SRC_TSIF1,
    E_TSP_PCR_SRC_TSIF2,
    E_TSP_PCR_SRC_TSIF3,
    E_TSP_PCR_SRC_TSIF4,
    E_TSP_PCR_SRC_TSIF5,
    E_TSP_PCR_SRC_TSIF6,

    E_TSP_PCR_SRC_MMFI0 = 0xA,
    E_TSP_PCR_SRC_MMFI1,
    E_TSP_PCR_SRC_FIQ0,
    E_TSP_PCR_SRC_FIQ1,
    E_TSP_PCR_SRC_INVALID,
} TSP_PCR_SRC;

typedef enum _HAL_TSP_TSIF // for HW TSIF
{
    E_TSP_HAL_TSIF_0            ,
    E_TSP_HAL_TSIF_1            ,
    E_TSP_HAL_TSIF_2            ,
    E_TSP_HAL_TSIF_3            ,
    E_TSP_HAL_TSIF_4            ,
    E_TSP_HAL_TSIF_5            ,
    E_TSP_HAL_TSIF_6            ,

    // @NOTE There are no real TSIFs for TSIF_PVRx , just use those for PVR backward competiable.
    E_TSP_HAL_TSIF_PVR0         ,
    E_TSP_HAL_TSIF_PVR1         ,
    E_TSP_HAL_TSIF_PVR2         ,
    E_TSP_HAL_TSIF_PVR3         ,
    E_TSP_HAL_TSIF_INVALID      ,
} TSP_HAL_TSIF;


typedef enum _TSP_HAL_FileState
{
    /// Command Queue is Idle
    E_TSP_HAL_FILE_STATE_IDLE           =   0000000000,
    /// Command Queue is Busy
    E_TSP_HAL_FILE_STATE_BUSY           =   0x00000001,
    /// Command Queue is Paused.
    E_TSP_HAL_FILE_STATE_PAUSE          =   0x00000002,

    E_TSP_HAL_FILE_STATE_INVALID,
}TSP_HAL_FileState;

typedef enum
{
    E_TSP_HAL_CAP_TYPE_PIDFLT_NUM                    = 0,
    E_TSP_HAL_CAP_TYPE_SECFLT_NUM                    = 1,
    E_TSP_HAL_CAP_TYPE_SECBUF_NUM                    = 2,

    E_TSP_HAL_CAP_TYPE_RECENG_NUM                    = 3,
    E_TSP_HAL_CAP_TYPE_RECFLT_NUM                    = 4,
    E_TSP_HAL_CAP_TYPE_RECFLT1_NUM                   = 5,

    E_TSP_HAL_CAP_TYPE_MMFI_AUDIO_FILTER_NUM         = 6,
    E_TSP_HAL_CAP_TYPE_MMFI_V3D_FILTER_NUM           = 7,

    E_TSP_HAL_CAP_TYPE_TSIF_NUM                      = 8,
    E_TSP_HAL_CAP_TYPE_DEMOD_NUM                     = 9,
    E_TSP_HAL_CAP_TYPE_TSPAD_NUM                     = 10,
    E_TSP_HAL_CAP_TYPE_VQ_NUM                        = 11,

    E_TSP_HAL_CAP_TYPE_CAFLT_NUM                     = 12,
    E_TSP_HAL_CAP_TYPE_CAKEY_NUM                     = 13,

    E_TSP_HAL_CAP_TYPE_FW_ALIGN                      = 14,
    E_TSP_HAL_CAP_TYPE_VQ_ALIGN                      = 15,
    E_TSP_HAL_CAP_TYPE_VQ_PITCH                      = 16,
    E_TSP_HAL_CAP_TYPE_SECBUF_ALIGN                  = 17,
    E_TSP_HAL_CAP_TYPE_PVR_ALIGN                     = 18,

    E_TSP_HAL_CAP_TYPE_PVRCA_PATH_NUM                = 19,
    E_TSP_HAL_CAP_TYPE_SHAREKEY_FLT_RANGE            = 20,
    E_TSP_HAL_CAP_TYPE_PVRCA0_FLT_RANGE              = 21,
    E_TSP_HAL_CAP_TYPE_PVRCA1_FLT_RANGE              = 22,
    E_TSP_HAL_CAP_TYPE_PVRCA2_FLT_RANGE              = 23,
    E_TSP_HAL_CAP_TYPE_SHAREKEY_FLT1_RANGE           = 24,
    E_TSP_HAL_CAP_TYPE_SHAREKEY_FLT2_RANGE           = 25,

    E_TSP_HAL_CAP_TYPE_HW_TYPE                       = 26,

    //27 is reserved, and can not be used

    E_TSP_HAL_CAP_TYPE_VFIFO_NUM                     = 28,
    E_TSP_HAL_CAP_TYPE_AFIFO_NUM                     = 29,
    E_TSP_HAL_CAP_TYPE_HWPCR_SUPPORT                 = 30,
    E_TSP_HAL_CAP_TYPE_PCRFLT_START_IDX              = 31,
    E_TSP_HAL_CAP_TYPE_RECFLT_IDX                    = 32,

    E_TSP_HAL_CAP_TYPE_DSCMB_ENG_NUM                 = 33,
    E_TSP_HAL_CAP_TYPE_MAX_MERGESTR_NUM              = 34,
    E_TSP_HAL_CAP_MAX_SEC_FLT_DEPTH                  = 35,
    E_TSP_HAL_CAP_FW_BUF_SIZE                        = 36,
    E_TSP_HAL_CAP_FW_BUF_RANGE                       = 37,
    E_TSP_HAL_CAP_VQ_BUF_RANGE                       = 38,
    E_TSP_HAL_CAP_SEC_BUF_RANGE                      = 39,
    E_TSP_HAL_CAP_FIQ_NUM                            = 40,
    E_TSP_HAL_CAP_TYPE_NULL,
} TSP_HAL_CAP_TYPE;

// @F_TODO remove unused enum member
typedef enum
{
    E_TSP_HAL_CAP_VAL_PIDFLT_NUM                    = (TSP_PCRFLT_END_ID - TSP_PIDFLT_START_ID),
    E_TSP_HAL_CAP_VAL_SECFLT_NUM                    = (TSP_SECFLT_END_ID - TSP_SECFLT_START_ID),
    E_TSP_HAL_CAP_VAL_SECBUF_NUM                    = (TSP_SECBUF_END_ID - TSP_SECBUF_START_ID),

    E_TSP_HAL_CAP_VAL_RECENG_NUM                    = PVR_NUM,
    E_TSP_HAL_CAP_VAL_RECFLT_NUM                    = TSP_PIDFLT_REC_NUM,
    E_TSP_HAL_CAP_VAL_RECFLT_IDX                    = TSP_RECFLT_IDX,
    E_TSP_HAL_CAP_VAL_PCRFLT_START_IDX              = TSP_PCRFLT_START_ID,
    E_TSP_HAL_CAP_VAL_RECFLT1_NUM                   = 0xDEADBEEF, // 0xDEADBEEF for not support

    E_TSP_HAL_CAP_VAL_MMFI_AUDIO_FILTER_NUM         = 6,  //MMFI0 filters
    E_TSP_HAL_CAP_VAL_MMFI_V3D_FILTER_NUM           = 6,  //MMFI1 filters

    E_TSP_HAL_CAP_VAL_TSIF_NUM                      = TSP_TSIF_NUM,
    E_TSP_HAL_CAP_VAL_DEMOD_NUM                     = STC_ENG_NUM, //internal demod  // [ToDo] STC number... by MM problem Jason-YH.Sun
    E_TSP_HAL_CAP_VAL_TSPAD_NUM                     = 3,
    E_TSP_HAL_CAP_VAL_VQ_NUM                        = VQ_NUM,

    E_TSP_HAL_CAP_VAL_CAFLT_NUM                     = (TSP_PIDFLT_END_ID - TSP_PIDFLT_START_ID), //@NOTE: flt number for descrypt purpose
    E_TSP_HAL_CAP_VAL_CAKEY_NUM                     = 0xDEADBEEF,

    E_TSP_HAL_CAP_VAL_FW_ALIGN                      = 0x100,
    E_TSP_HAL_CAP_VAL_VQ_ALIGN                      = 16,         // 16 byte align??
    E_TSP_HAL_CAP_VAL_VQ_PITCH                      = 208,        // 208 byte per VQ unit
    E_TSP_HAL_CAP_VAL_SECBUF_ALIGN                  = 16,         // 16 byte align
    E_TSP_HAL_CAP_VAL_PVR_ALIGN                     = 16,

    E_TSP_HAL_CAP_VAL_PVRCA_PATH_NUM                = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_SHAREKEY_FLT_RANGE            = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_PVRCA0_FLT_RANGE              = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_PVRCA1_FLT_RANGE              = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_PVRCA2_FLT_RANGE              = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_SHAREKEY_FLT1_RANGE           = 0xDEADBEEF,
    E_TSP_HAL_CAP_VAL_SHAREKEY_FLT2_RANGE           = 0xDEADBEEF,

    E_TSP_HAL_CAP_VAL_HW_TYPE                       = 0x80002003,

    E_TSP_HAL_CAP_VAL_VFIFO_NUM                     = 8,
    E_TSP_HAL_CAP_VAL_AFIFO_NUM                     = 6,
    E_TSP_HAL_CAP_VAL_HWPCR_SUPPORT                 = 1,
    E_TSP_HAL_CAP_VAL_FIQ_NUM                       = TSP_TSIF_NUM,

    E_TSP_HAL_CAP_VAL_FW_BUF_SIZE                   = 0x8000,   // 32KB

    E_TSP_HAL_CAP_VAL_NULL                          = 0xDEADBEEF,
} TSP_HAL_CAP_VAL;

/// TSP TEI  Remove Error Packet Infomation
typedef enum
{
    E_TSP_HAL_TEI_REMOVE_AUDIO_PKT,         ///< TEI Remoce Audio Packet
    E_TSP_HAL_TEI_REMOVE_AUDIO2_PKT,        ///< TEI Remoce Audio2 Packet
    E_TSP_HAL_TEI_REMOVE_AUDIO3_PKT,        ///< TEI Remoce Audio3 Packet
    E_TSP_HAL_TEI_REMOVE_AUDIO4_PKT,        ///< TEI Remoce Audio4 Packet
    E_TSP_HAL_TEI_REMOVE_AUDIO5_PKT,        ///< TEI Remoce Audio5 Packet
    E_TSP_HAL_TEI_REMOVE_AUDIO6_PKT,        ///< TEI Remoce Audio6 Packet

    E_TSP_HAL_TEI_REMOVE_VIDEO_PKT,         ///< TEI Remoce Video Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO2_PKT,        ///< TEI Remoce Video2 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO3_PKT,        ///< TEI Remoce Video3 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO4_PKT,        ///< TEI Remoce Video4 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO5_PKT,        ///< TEI Remoce Video5 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO6_PKT,        ///< TEI Remoce Video6 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO7_PKT,        ///< TEI Remoce Video7 Packet
    E_TSP_HAL_TEI_REMOVE_VIDEO8_PKT         ///< TEI Remoce Video8 Packet

}TSP_HAL_TEI_RmPktType;

// TSP TimeStamp Clk Select
typedef enum
{
    E_TSP_HAL_TIMESTAMP_CLK_90K     = 0,
    E_TSP_HAL_TIMESTAMP_CLK_27M     = 1,
    E_TSP_HAL_TIMESTAMP_CLK_INVALID = 2

} TSP_HAL_TimeStamp_Clk;

/// TSP Packet Converter Input Mode
typedef enum
{
    E_TSP_HAL_PKT_MODE_NORMAL,               ///< Normal Mode (bypass)
    E_TSP_HAL_PKT_MODE_CI,                   ///< CI+ 1.4 (188 bytes)
    E_TSP_HAL_PKT_MODE_OPEN_CABLE,           ///< Open Cable (200 bytes)
    E_TSP_HAL_PKT_MODE_ATS,                  ///< ATS mode (192 bytes) (188+TimeStamp)
    E_TSP_HAL_PKT_MODE_MXL_192,              ///< MXL mode (192 bytes)
    E_TSP_HAL_PKT_MODE_MXL_196,              ///< MXL mode (196 bytes)
    E_TSP_HAL_PKT_MODE_MXL_200,              ///< MXL mode (200 bytes)
    E_TSP_HAL_PKT_MODE_ND,                   ///< Nagra Dongle mode (192 bytes)

    E_TSP_HAL_PKT_MODE_INVALID
}TSP_HAL_PKT_MODE;

//----------------------------------
/// DMX debug table information structure
//----------------------------------

typedef enum
{
    E_TSP_HAL_FLOW_LIVE0,
    E_TSP_HAL_FLOW_LIVE1,
    E_TSP_HAL_FLOW_LIVE2,
    E_TSP_HAL_FLOW_LIVE3,
    E_TSP_HAL_FLOW_LIVE4,
    E_TSP_HAL_FLOW_LIVE5,
    E_TSP_HAL_FLOW_LIVE6,

    E_TSP_HAL_FLOW_FILE0,
    E_TSP_HAL_FLOW_FILE1,
    E_TSP_HAL_FLOW_FILE2,
    E_TSP_HAL_FLOW_FILE3,
    E_TSP_HAL_FLOW_FILE4,
    E_TSP_HAL_FLOW_FILE5,
    E_TSP_HAL_FLOW_FILE6,

    E_TSP_HAL_FLOW_MMFI0,
    E_TSP_HAL_FLOW_MMFI1,

    E_TSP_HAL_FLOW_INVALID,

} TSP_HAL_FLOW;

typedef enum
{
    E_TSP_HAL_GATING_PATH0 = 0,
    E_TSP_HAL_GATING_PATH1,
    E_TSP_HAL_GATING_PATH2,
    E_TSP_HAL_GATING_PATH3,
    E_TSP_HAL_GATING_PATH4,
    E_TSP_HAL_GATING_PATH5,
    E_TSP_HAL_GATING_PATH6,
    E_TSP_HAL_GATING_TSP_ENG,
    E_TSP_HAL_GATING_FIQ,   //global
    E_TSP_HAL_GATING_PVR1,
    E_TSP_HAL_GATING_PVR2,
    E_TSP_HAL_GATING_PVR3,
    E_TSP_HAL_GATING_PVR4,
    E_TSP_HAL_GATING_PVR5,
    E_TSP_HAL_GATING_PVR6,
    E_TSP_HAL_GATING_PVR7,
    E_TSP_HAL_GATING_PVR8,

    E_TSP_HAL_MIU_CLK_GATING_PATH0,
    E_TSP_HAL_MIU_CLK_GATING_PATH1,
    E_TSP_HAL_MIU_CLK_GATING_PATH2,
    E_TSP_HAL_MIU_CLK_GATING_PATH3,
    E_TSP_HAL_MIU_CLK_GATING_PATH4,
    E_TSP_HAL_MIU_CLK_GATING_PATH5,
    E_TSP_HAL_MIU_CLK_GATING_PATH6,
    E_TSP_HAL_MIU_CLK_GATING_TSP_ENG,
    E_TSP_HAL_MIU_CLK_GATING_FIQ,
    E_TSP_HAL_MIU_CLK_GATING_PVR1,
    E_TSP_HAL_MIU_CLK_GATING_PVR2,
    E_TSP_HAL_MIU_CLK_GATING_PVR3,
    E_TSP_HAL_MIU_CLK_GATING_PVR4,
    E_TSP_HAL_MIU_CLK_GATING_PVR5,
    E_TSP_HAL_MIU_CLK_GATING_PVR6,
    E_TSP_HAL_MIU_CLK_GATING_PVR7,
    E_TSP_HAL_MIU_CLK_GATING_PVR8,
    E_TSP_HAL_MIU_CLK_GATING_MMFI0,
    E_TSP_HAL_MIU_CLK_GATING_MMFI1,

    E_TSP_HAL_GATING_FIQ0,
    E_TSP_HAL_GATING_FIQ1,
    E_TSP_HAL_GATING_FIQ2,
    E_TSP_HAL_GATING_FIQ3,
    E_TSP_HAL_GATING_FIQ4,
    E_TSP_HAL_GATING_FIQ5,
    E_TSP_HAL_GATING_FIQ6,

    E_TSP_HAL_MIU_CLK_GATING_FIQ0,
    E_TSP_HAL_MIU_CLK_GATING_FIQ1,
    E_TSP_HAL_MIU_CLK_GATING_FIQ2,
    E_TSP_HAL_MIU_CLK_GATING_FIQ3,
    E_TSP_HAL_MIU_CLK_GATING_FIQ4,
    E_TSP_HAL_MIU_CLK_GATING_FIQ5,
    E_TSP_HAL_MIU_CLK_GATING_FIQ6,

    E_TSP_HAL_GATING_INVALID,

} TSP_HAL_GATING;

typedef enum
{
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER0,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER1,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER2,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER3,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER4,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER5,
    E_TSP_HAL_RESET_CTRL_PKT_CONVERTER6,

    E_TSP_HAL_RESET_CTRL_FIQ0,
    E_TSP_HAL_RESET_CTRL_FIQ1,
    E_TSP_HAL_RESET_CTRL_FIQ2,
    E_TSP_HAL_RESET_CTRL_FIQ3,
    E_TSP_HAL_RESET_CTRL_FIQ4,
    E_TSP_HAL_RESET_CTRL_FIQ5,
    E_TSP_HAL_RESET_CTRL_FIQ6,

    E_TSP_HAL_RESET_CTRL_VQ_TX0,
    E_TSP_HAL_RESET_CTRL_VQ_TX1,
    E_TSP_HAL_RESET_CTRL_VQ_TX2,
    E_TSP_HAL_RESET_CTRL_VQ_TX3,
    E_TSP_HAL_RESET_CTRL_VQ_TX4,
    E_TSP_HAL_RESET_CTRL_VQ_TX5,
    E_TSP_HAL_RESET_CTRL_VQ_TX6,
    E_TSP_HAL_RESET_CTRL_VQ_RX,
    E_TSP_HAL_RESET_CTRL_VQ_TOP,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX0,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX1,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX2,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX3,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX4,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX5,
    E_TSP_HAL_RESET_CTRL_PKT_DEMUX6,

    E_TSP_HAL_RESET_CTRL_PVR1,
    E_TSP_HAL_RESET_CTRL_PVR2,
    E_TSP_HAL_RESET_CTRL_PVR3,
    E_TSP_HAL_RESET_CTRL_PVR4,
    E_TSP_HAL_RESET_CTRL_PVR5,
    E_TSP_HAL_RESET_CTRL_PVR6,
    E_TSP_HAL_RESET_CTRL_PVR7,
    E_TSP_HAL_RESET_CTRL_PVR8,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR1,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR2,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR3,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR4,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR5,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR6,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR7,
    E_TSP_HAL_RESET_CTRL_TIMESTAMP_SEL_PVR8,
    E_TSP_HAL_RESET_CTRL_SP_D0,
    E_TSP_HAL_RESET_CTRL_SP_D1,
    E_TSP_HAL_RESET_CTRL_SP_D2,
    E_TSP_HAL_RESET_CTRL_SP_D3,
    E_TSP_HAL_RESET_CTRL_SP_D4,
    E_TSP_HAL_RESET_CTRL_SP_D5,
    E_TSP_HAL_RESET_CTRL_SP_D6,

    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT0,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT1,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT2,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT3,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT4,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT5,
    E_TSP_HAL_RESET_CTRL_FILTER_NULL_PKT6,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_0,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_1,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_2,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_3,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_4,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_5,
    E_TSP_HAL_RESET_CTRL_DIRECTV_130_188_6,

    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER0,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER1,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER2,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER3,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER4,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER5,
    E_TSP_HAL_RESET_CTRL_SRC_ID_PARSER6,
    E_TSP_HAL_RESET_CTRL_PCRFLT_0,
    E_TSP_HAL_RESET_CTRL_PCRFLT_1,
    E_TSP_HAL_RESET_CTRL_PCRFLT_2,
    E_TSP_HAL_RESET_CTRL_PCRFLT_3,
    E_TSP_HAL_RESET_CTRL_PCRFLT_4,
    E_TSP_HAL_RESET_CTRL_PCRFLT_5,
    E_TSP_HAL_RESET_CTRL_PCRFLT_6,

    E_TSP_HAL_RESET_PATH0,
    E_TSP_HAL_RESET_PATH1,
    E_TSP_HAL_RESET_PATH2,
    E_TSP_HAL_RESET_PATH3,
    E_TSP_HAL_RESET_PATH4,
    E_TSP_HAL_RESET_PATH5,
    E_TSP_HAL_RESET_PATH6,
    E_TSP_HAL_RESET_OTV,
    E_TSP_HAL_RESET_DEBUG_TABLE,
    E_TSP_HAL_RESET_DMA_ENG,
    E_TSP_HAL_RESET_SEC_CMP,
    E_TSP_HAL_RESET_SECFLT_REG,
    E_TSP_HAL_RESET_SEC,
    E_TSP_HAL_RESET_PID_TABLE,

    E_TSP_HAL_RESET_CTRL_INVALID,

} TSP_HAL_RESET_CTRL;

typedef enum
{
    E_TSP_FQ_MUX_OUT_PATH = 0,
    E_TSP_FQ_MUX_OUT_MUX_0,
    E_TSP_FQ_MUX_OUT_MUX_1,
    E_TSP_FQ_MUX_OUT_MUX_2,

    E_TSP_FQ_MUX_OUT_INVALID,

} TSP_FQ_MUX_OUT_SRC;

typedef enum
{
    E_TSP_HAL_MIU_SEL_MMFI = 0,
    E_TSP_HAL_MIU_SEL_FQ   = 1,

    E_TSP_HAL_MIU_SEL_INVALID,

} TSP_HAL_MIU_SEL_TYPE;

//--------------------------------------------------------------------------------------------------
// PVR HAL API
//--------------------------------------------------------------------------------------------------
// Static Register Mapping for external access
#define REG_PIDFLT_BASE0                (0x00240000UL)
#define REG_PIDFLT_BASE1                (0x00241000UL)
#define REG_PIDFLT_BASE2                (0x00242000UL)
#define REG_SECFLT_BASE                 (0x00221000UL)
#define REG_SECBUF_BASE                 (0x00221024UL)
#define REG_SECFLT_BUFID_H_BASE         (0x00230FD0UL)  // section flt buf_id[8]

#define _REGPid0                        ((REG_Pid*) (REG_PIDFLT_BASE0))
#define _REGPid1                        ((REG_Pid*) (REG_PIDFLT_BASE1))
#define _REGPid2                        ((REG_Pid*) (REG_PIDFLT_BASE2))
#define _REGSec                         ((REG_Sec*)  (REG_SECFLT_BASE))
#define _REGBuf                         ((REG_Buf*)  (REG_SECBUF_BASE))

#define PPIDFLT0(_fltid)                (&(_REGPid0->Flt[_fltid]))
#define PPIDFLT1(_fltid)                (&(_REGPid1->Flt[_fltid]))
#define PPIDFLT2(_fltid)                (&(_REGPid2->Flt[_fltid]))
#define PSECFLT(_fltid)                 (&(((REG_Sec*)(REG_SECFLT_BASE+(_fltid>>5)*0x1000))->Flt[_fltid&(0x1F)]))
#define PSECBUF(_bufid)                 (&(((REG_Buf*)(REG_SECBUF_BASE+(_bufid>>5)*0x1000))->Buf[_bufid&(0x1F)]))


//******************** PIDFLT DEFINE START ********************//
//===== [PPIDFLT #0]: 0x240000 =====//
// PID
#define TSP_PIDFLT_PID_MASK             0x00001FFF
#define TSP_PIDFLT_PID_SHFT             0

// PIDFLT SRC
typedef enum _TSP_PIDFLT_SRC
{
    E_TSP_PIDFLT_LIVE0,
    E_TSP_PIDFLT_LIVE1,
    E_TSP_PIDFLT_LIVE2,
    E_TSP_PIDFLT_LIVE3,
    E_TSP_PIDFLT_LIVE4,
    E_TSP_PIDFLT_LIVE5,
    E_TSP_PIDFLT_LIVE6,
    E_TSP_PIDFLT_FILE0,
    E_TSP_PIDFLT_FILE1,
    E_TSP_PIDFLT_FILE2,
    E_TSP_PIDFLT_FILE3,
    E_TSP_PIDFLT_FILE4,
    E_TSP_PIDFLT_FILE5,
    E_TSP_PIDFLT_FILE6,
    E_TSP_PIDFLT_INVALID,
} TSP_PIDFLT_SRC;

// Path_SRC
#define TSP_PIDFLT_IN_MASK              0x0000E000
#define TSP_PIDFLT_TSIF_SHFT            13
#define TSP_PIDFLT_TSIF0                0x01
#define TSP_PIDFLT_TSIF1                0x02
#define TSP_PIDFLT_TSIF2                0x03
#define TSP_PIDFLT_TSIF3                0x04
#define TSP_PIDFLT_TSIF4                0x05
#define TSP_PIDFLT_TSIF5                0x06
#define TSP_PIDFLT_TSIF6                0x07
#define TSP_PIDFLT_TSIF_MAX             0x08

// SRC ID
#define TSP_PIDFLT_SRCID_MASK           0x003F0000
#define TSP_PIDFLT_SRCID_SHIFT          16

// PKT_RUSH_PASS
#define TSP_PID_FLT_PKTPUSH_PASS        0x00400000

// PVR_LUT_EN
#define TSP_PIDFLT_OUT_LUT              0x00800000

//===== [PPIDFLT #1]: 0x241000 =====//
// PIDFLT DST
typedef enum _TSP_PIDFLT_DST
{
    E_TSP_PIDFLT_DST_VIDEO,
    E_TSP_PIDFLT_DST_AUDIO,
    E_TSP_PIDFLT_DST_PVR,

    E_TSP_PIDFLT_DST_INVALID,
} TSP_PIDFLT_DST;

// Audio 0~5
#define TSP_PIDFLT_OUT_MASK             0x03FFFFFF
#define TSP_PIDFLT_OUT_SHFT             0
#define TSP_PIDFLT_OUT_NONE             0x00000000
#define TSP_PIDFLT_OUT_AFIFO            0x00000001
#define TSP_PIDFLT_OUT_AFIFO2           0x00000002
#define TSP_PIDFLT_OUT_AFIFO3           0x00000004
#define TSP_PIDFLT_OUT_AFIFO4           0x00000008
#define TSP_PIDFLT_OUT_AFIFO5           0x00000010
#define TSP_PIDFLT_OUT_AFIFO6           0x00000020
// AF & Sec
#define TSP_PIDFLT_OUT_SECAF            0x00000040
#define TSP_PIDFLT_OUT_SECFLT           0x00000080
// Video 0~7
#define TSP_PIDFLT_OUT_VFIFO            0x00000100
#define TSP_PIDFLT_OUT_VFIFO3D          0x00000200
#define TSP_PIDFLT_OUT_VFIFO3           0x00000400
#define TSP_PIDFLT_OUT_VFIFO4           0x00000800
#define TSP_PIDFLT_OUT_VFIFO5           0x00001000
#define TSP_PIDFLT_OUT_VFIFO6           0x00002000
#define TSP_PIDFLT_OUT_VFIFO7           0x00004000
#define TSP_PIDFLT_OUT_VFIFO8           0x00008000
// PVR 1~10
#define TSP_PIDFLT_OUT_PVR1             0x00010000
#define TSP_PIDFLT_OUT_PVR2             0x00020000
#define TSP_PIDFLT_OUT_PVR3             0x00040000
#define TSP_PIDFLT_OUT_PVR4             0x00080000
#define TSP_PIDFLT_OUT_PVR5             0x00100000
#define TSP_PIDFLT_OUT_PVR6             0x00200000
#define TSP_PIDFLT_OUT_PVR7             0x00400000
#define TSP_PIDFLT_OUT_PVR8             0x00800000
#define TSP_PIDFLT_OUT_PVR9             0x01000000
#define TSP_PIDFLT_OUT_PVR10            0x02000000

#define TSP_PIDFLT_PVRFLT_MASK          0x03FF0000
#define TSP_PIDFLT_PVRFLT_SHIFT         16

//===== [PPIDFLT #2]: 0x242000 =====//
// Section filter Id (0~511)
#define TSP_PIDFLT_SECFLT_MASK          0x000001FF
#define TSP_PIDFLT_SECFLT_SHFT          0
#define TSP_PIDFLT_SECFLT_NULL          0x000000FF  // software usage clean selected section filter
// FIQ_LUT
#define TSP_PIDFLT_FIQ_LUT_MASK         0x0000F000
#define TSP_PIDFLT_FIQ_LUT_SHIFT        12
// MULTI_PVR
#define TSP_PIDFLT_MULTI_PVR_MASK       0x00FF0000
#define TSP_PIDFLT_MULTI_PVR_SHIFT      16

//******************** PIDFLT DEFINE END ********************//
void    TSP32_IdrW(TSP32 *preg, MS_U32 value);
MS_U32  TSP32_IdrR(TSP32 *preg);

//=========================TSIF================================
MS_BOOL HAL_TSP_TSIF_SelPad(MS_U32 tsIf, TSP_TS_PAD eTSPad);
MS_BOOL HAL_TSP_SetTSIF(MS_U16 u16TSIF, TSP_TSIF_CFG u16Cfg, MS_BOOL bFileIn);
MS_BOOL HAL_TSP_TSIF_LiveEn(MS_U32 tsIf, MS_BOOL bEnable);
MS_BOOL HAL_TSP_TSIF_FileEn(FILEENG_SEQ eFileEng, MS_BOOL bEnable);
void    HAL_TSP_TSIF_BitSwap(MS_U32 tsIf, MS_BOOL bEnable);
void    HAL_TSP_TSIF_ExtSync(MS_U32 tsIf, MS_BOOL bEnable);
void    HAL_TSP_TSIF_Parl(MS_U32 tsIf, MS_BOOL bEnable);
void    HAL_TSP_PAD_3Wire(MS_U32 u32Pad, MS_BOOL bEnable);
void    HAL_TSP_TSIF_3Wire(MS_U32 tsIf, MS_BOOL bEnable);
MS_BOOL HAL_TSP_TSIF_SelPad_ClkInv(MS_U32 tsIf , MS_BOOL bClkInv);
MS_BOOL HAL_TSP_TSIF_SelPad_ClkDis(MS_U32 tsIf , MS_BOOL bClkDis);
void    HAL_TSP_TEI_SKIP(MS_U32 tsIf, MS_BOOL bEnable);

//=========================TSP================================
void    HAL_TSP_PktDmx_RmDupAVPkt(MS_BOOL bEnable);
void    HAL_TSP_SetBank(MS_VIRT u32BankAddr);
void    HAL_TSP_Reset(MS_BOOL bEn);
void    HAL_TSP_Power(MS_BOOL bEn);
void    HAL_TSP_CPU(MS_BOOL bEn);
void    HAL_TSP_HwPatch(void);
void    HAL_TSP_RestoreFltState(void);
MS_BOOL HAL_TSP_LoadFW(MS_U32 u32FwPhyAddr, MS_U32 u32FwSize);
void    HAL_TSP_Set_RcvBuf_Src(MS_U32 bufIdx, MS_U32 inputSrc);
void    HAL_TSP_SaveFltState(void);
MS_BOOL HAL_TSP_GetCaps(TSP_HAL_CAP_TYPE eCap, MS_U32 *pu32CapInfo);
MS_BOOL HAL_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, MS_U32* pData);
void    HAL_TSP_TEI_RemoveErrorPkt(TSP_HAL_TEI_RmPktType eHalPktType, MS_BOOL bEnable);
void    HAL_TSP_FQ_MMFI_MIU_Sel(TSP_HAL_MIU_SEL_TYPE eType, MS_U8 u8Eng, MS_PHY phyBufStart);

//=========================TSO================================
void    HAL_TSO_SetTSOOutMUX(MS_BOOL bSet);
MS_BOOL HAL_TSP_TSO_TSIF_SelPad(MS_U32 u32TSOEng, TSP_TS_PAD eTSPad);

//=========================Filein================================
void    HAL_TSP_Filein_PktSize(FILEENG_SEQ eFileEng, MS_U32 u32PktSize);
void    HAL_TSP_Filein_Addr(FILEENG_SEQ eFileEng, MS_U32 addr);
void    HAL_TSP_Filein_Size(FILEENG_SEQ eFileEng, MS_U32 size);
void    HAL_TSP_Filein_Start(FILEENG_SEQ eFileEng);
void    HAL_TSP_Filein_Abort(FILEENG_SEQ eFileEng, MS_BOOL bEn);
void    HAL_TSP_Filein_CmdQRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable);
MS_U32  HAL_TSP_Filein_CmdQSlot(FILEENG_SEQ eFileEng);
MS_U32  HAL_TSP_Filein_CmdQCnt(FILEENG_SEQ eFileEng);
MS_U32  HAL_TSP_Filein_CmdQLv(FILEENG_SEQ eFileEng);
void    HAL_TSP_Filein_ByteDelay(FILEENG_SEQ eFileEng, MS_U32 delay, MS_BOOL bEnable);
MS_U32  HAL_TSP_Filein_Status(FILEENG_SEQ eFileEng);
void    HAL_TSP_Filein_BlockTimeStamp(FILEENG_SEQ eFileEng, MS_BOOL bEn);
void    HAL_TSP_Filein_PacketMode(FILEENG_SEQ eFileEng,MS_BOOL bSet);
void    HAL_TSP_Filein_SetTimeStamp(FILEENG_SEQ eFileEng, MS_U32 u32Stamp);
void    HAL_TSP_Filein_SetTimeStampClk(FILEENG_SEQ eFileEng, TSP_HAL_TimeStamp_Clk eTimeStampClk);
MS_U32  HAL_TSP_Filein_GetTimeStamp(FILEENG_SEQ eFileEng);
MS_U32  HAL_TSP_Filein_PktTimeStamp(FILEENG_SEQ eFileEng);
void    HAL_TSP_Filein_Bypass(FILEENG_SEQ eFileEng, MS_BOOL bBypass);// for PS mode A/V fifo pull back

MS_BOOL HAL_TSP_File_Pause(FILEENG_SEQ eFileEng);
MS_BOOL HAL_TSP_File_Resume(FILEENG_SEQ eFileEng);
TSP_HAL_FileState HAL_TSP_Filein_GetState(FILEENG_SEQ eFileEng);
void    HAL_TSP_Filein_GetCurAddr(FILEENG_SEQ eFileEng, MS_PHY *pu32Addr);
void    HAL_TSP_Filein_WbFsmRst(FILEENG_SEQ eFileEng, MS_BOOL bEnable);
void    HAL_TSP_Filein_Init_Trust_Start(FILEENG_SEQ eFileEng);
MS_BOOL HAL_TSP_Filein_Done_Status(FILEENG_SEQ eFileEng);

//=========================PCR FLT================================
void    HAL_TSP_PcrFlt_SetPid(MS_U32 pcrFltId, MS_U32 u32Pid);
MS_U32  HAL_TSP_PcrFlt_GetPid(MS_U32 pcrFltId);
void    HAL_TSP_PcrFlt_Enable(MS_U32 pcrFltId, MS_BOOL bEnable);
void    HAL_TSP_PcrFlt_SetSrc(MS_U32 pcrFltId, TSP_PCR_SRC src);
void    HAL_TSP_PcrFlt_GetSrc(MS_U32 pcrFltId, TSP_PCR_SRC *pPcrSrc);
void    HAL_TSP_PcrFlt_GetPcr(MS_U32 pcrFltId, MS_U32 *pu32Pcr_H, MS_U32 *pu32Pcr);
void    HAL_TSP_PcrFlt_Reset(MS_U32 pcrFltId);
void    HAL_TSP_PcrFlt_ClearInt(MS_U32 pcrFltId);
MS_U32  HAL_TSP_PcrFlt_GetIntMask(MS_U32 pcrFltId);

//=========================STC================================
void    HAL_TSP_STC_Init(void);
void    HAL_TSP_SetSTCSynth(MS_U32 Eng, MS_U32 u32Sync);
void    HAL_TSP_GetSTCSynth(MS_U32 Eng, MS_U32* u32Sync);
void    HAL_TSP_STC64_Mode_En(MS_BOOL bEnable);                         // @NOTE: need to modify
void    HAL_TSP_STC64_Set(MS_U32 Eng, MS_U32 stcH, MS_U32 stcL);
void    HAL_TSP_STC64_Get(MS_U32 Eng, MS_U32* pStcH, MS_U32* pStcL);
void    HAL_TSP_STC33_CmdQSet(MS_U32 stcH, MS_U32 stcL);                // @NOTE: need to modify
void    HAL_TSP_STC33_CmdQGet(MS_U32* pStcH, MS_U32* pStcL);            // @NOTE: need to modify

//=========================FIFO================================
void    HAL_TSP_FIFO_SetSrc   (TSP_DST_SEQ eFltType, MS_U32 pktDmxId);
void    HAL_TSP_FIFO_GetSrc   (TSP_DST_SEQ eFltType, TSP_SRC_SEQ *pktDmxId);
void    HAL_TSP_FIFO_Bypass   (TSP_DST_SEQ eFltType, MS_BOOL bEn);
void    HAL_TSP_FIFO_Bypass_Src(FILEENG_SEQ eFileEng, TSP_DST_SEQ eFltType);
void    HAL_TSP_FIFO_ClearAll (void);
MS_U32  HAL_TSP_FIFO_PidHit   (TSP_DST_SEQ eFltType);
void    HAL_TSP_FIFO_Reset    (TSP_DST_SEQ eFltType, MS_BOOL bReset);
MS_U32  HAL_TSP_FIFO_Level    (TSP_DST_SEQ eFltType);
MS_BOOL HAL_TSP_FIFO_Overflow (TSP_DST_SEQ eFltType);
MS_BOOL HAL_TSP_FIFO_Empty    (TSP_DST_SEQ eFltType);
void    HAL_TSP_FIFO_BlockDis (TSP_DST_SEQ eFltType, MS_BOOL bDisable);
MS_U32  HAL_TSP_FIFO_GetStatus(TSP_DST_SEQ eFltType);
void    HAL_TSP_FIFO_Skip_Scrmb(TSP_DST_SEQ eFltType,MS_BOOL bSkip);

void    HAL_TSP_Flt_Bypass(TSP_DST_SEQ eFltType, MS_BOOL bEn);
MS_U16  HAL_TSP_FIFO_ReadPkt(TSP_DST_SEQ eFltType);             // read A/V fifo data
void    HAL_TSP_FIFO_ReadEn(TSP_DST_SEQ eFltType, MS_BOOL bEn); //
void    HAL_TSP_FIFO_Connect(TSP_DST_SEQ eFltType,MS_BOOL bEn); //
void    HAL_TSP_FIFO_BD_AUD_En(TSP_DST_SEQ eAudioType, MS_BOOL bMainChEn, MS_BOOL bEn);
void    HAL_TSP_BD_AUD_En(MS_U32 u32BD,MS_BOOL bEn);
void    HAL_TSP_TRACE_MARK_En(TSP_DST_SEQ eFltType,MS_BOOL bEn);

//=========================VQ================================
MS_BOOL HAL_TSP_SetVQ(MS_PHYADDR u32BaseAddr, MS_U32 u32BufLen);
MS_BOOL HAL_TSP_VQ_Buffer(MS_U32 vqId, MS_PHYADDR u32BaseAddr, MS_U32 u32BufLen);
void    HAL_TSP_VQ_Enable(MS_BOOL bEn);
void    HAL_TSP_VQ_Reset(MS_U32 vqId, MS_BOOL bEn);
void    HAL_TSP_VQ_OverflowInt_Clr(MS_U32 vqId);
void    HAL_TSP_VQ_OverflowInt_En(MS_U32 vqId, MS_BOOL bEn);
MS_BOOL HAL_TSP_VQ_Block_Dis(MS_U32 vqId,MS_BOOL bDis);

//=========================Pid Flt================================
void    HAL_TSP_PidFlt_SetPid(MS_U32 fltId, MS_U32 u32PID);
void    HAL_TSP_PidFlt_SetFltIn(MS_U32 fltId, MS_U32 u32FltIn);
void    HAL_TSP_PidFlt_SetFltOut(MS_U32 fltId, MS_U32 u32FltOut);
void    HAL_TSP_PidFlt_SetSecFlt(MS_U32 fltId, MS_U32 u32SecFltId);
void    HAL_TSP_PidFlt_SetPvrFlt(MS_U32 fltId, MS_U32 u32PVREng, MS_BOOL bEn);
void    HAL_TSP_PidFlt_SetFltRushPass(MS_U32 fltId, MS_U8 u8Enable);
MS_U32  HAL_TSP_PidFlt_GetPid(REG_PidFlt* pPidFlt);
MS_U32  HAL_TSP_PidFlt_GetFltOutput(REG_PidFlt *pPidFlt);
void    HAL_TSP_PidFlt_SetSrcID(MS_U32 fltId, MS_U32 u32SrcID);
void    HAL_TSP_PidFlt_SetLutEn(MS_U32 fltId, MS_BOOL bEn);
void    HAL_TSP_PidFlt_SetFqLutEn(MS_U32 fltId, MS_U32 u32FqLutEng, MS_BOOL bEn);
void    HAL_TSP_PidFlt_SetMultiPvrEn(MS_U32 fltId, MS_U32 u32MultiPvrEng, MS_U32 u32MultiPvrChId, MS_BOOL bEn);

//=========================SecFlt================================
void    HAL_TSP_SecFlt_BurstLen(MS_U32 burstMode);
void    HAL_TSP_SecFlt_SetType(REG_SecFlt *pSecFlt, MS_U32 u32FltType);
MS_U16  HAL_TSP_SecFlt_GetSecBuf(REG_SecFlt *pSecFlt);
void    HAL_TSP_SecFlt_ResetState(REG_SecFlt* pSecFlt);
void    HAL_TSP_SecFlt_ResetRmnCnt(REG_SecFlt* pSecFlt);
void    HAL_TSP_SecFlt_ClrCtrl(REG_SecFlt *pSecFlt);
void    HAL_TSP_SecFlt_SetMask(REG_SecFlt *pSecFlt, MS_U8 *pu8Mask);
void    HAL_TSP_SecFlt_SetNMask(REG_SecFlt *pSecFlt, MS_U8 *pu8NMask);
void    HAL_TSP_SecFlt_SetMatch(REG_SecFlt *pSecFlt, MS_U8 *pu8Match);
void    HAL_TSP_SecFlt_SetMode(REG_SecFlt *pSecFlt, MS_U32 u32SecFltMode);
MS_U32  HAL_TSP_SecFlt_GetState(REG_SecFlt *pSecFlt);
void    HAL_TSP_SecFlt_SelSecBuf(REG_SecFlt *pSecFlt, MS_U16 u16BufId);
MS_BOOL HAL_TSP_SecFlt_TryAlloc(REG_SecFlt* pSecFlt, MS_U16 u16TSPId);
void    HAL_TSP_SecFlt_SetAutoCRCChk(REG_SecFlt *pSecFlt, MS_BOOL bSet);
void    HAL_TSP_SecFlt_Free(REG_SecFlt* pSecFlt);
void    HAL_TSP_SecFlt_DropEnable(MS_BOOL bSet); // @TODO not implement yet

//=========================Sec Buf================================
void    HAL_TSP_SecBuf_SetBuf(REG_SecBuf *pSecBuf, MS_U32 u32StartAddr, MS_U32 u32BufSize);
void    HAL_TSP_SecBuf_SetRead(REG_SecBuf *pSecBuf, MS_U32 u32ReadAddr);
MS_U32  HAL_TSP_SecBuf_GetStart(REG_SecBuf *pSecBuf);
MS_U32  HAL_TSP_SecBuf_GetEnd(REG_SecBuf *pSecBuf);
MS_U32  HAL_TSP_SecBuf_GetBufCur(REG_SecBuf *pSecBuf);
void    HAL_TSP_SecBuf_Reset(REG_SecBuf *pSecBuf);
MS_U32  HAL_TSP_SecBuf_GetRead(REG_SecBuf *pSecBuf);
MS_U32  HAL_TSP_SecBuf_GetWrite(REG_SecBuf *pSecBuf);
MS_BOOL HAL_TSP_SecBuf_TryAlloc(REG_SecBuf *pSecBuf, MS_U16 u16TSPId);
void    HAL_TSP_SecBuf_Free(REG_SecBuf *pSecBuf);

//=========================PVR================================
void    HAL_PVR_Init(MS_U32 u32PVREng, MS_U32 pktDmxId);
void    HAL_PVR_Exit(MS_U32 u32PVREng);
void    HAL_PVR_Alignment_Enable(MS_U32 u32PVREng, MS_BOOL bEnable);
/*
void    HAL_PVR_SetTSIF(MS_U32 u32PVREng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP);
void    HAL_PVR_RecAtSync_Dis(MS_U32 u32PVREng, MS_BOOL bDis);
void    HAL_PVR_SetDataSwap(MS_U32 u32PVREng, MS_BOOL bEn);
*/
void    HAL_PVR_FlushData(MS_U32 u32PVREng);
void    HAL_PVR_Skip_Scrmb(MS_U32 u32PVREng,MS_BOOL bSkip);
void    HAL_PVR_Block_Dis(MS_U32 u32PVREng,MS_BOOL bDisable);
void    HAL_PVR_BurstLen(MS_U32 u32PVREng,MS_U16 u16BurstMode);
void    HAL_PVR_Start(MS_U32 u32PVREng);
void    HAL_PVR_Stop(MS_U32 u32PVREng);
void    HAL_PVR_Pause(MS_U32 u32PVREng , MS_BOOL bPause);
void    HAL_PVR_RecPid(MS_U32 u32PVREng, MS_BOOL bSet);
void    HAL_PVR_RecNull(MS_U32 u32PVREng, MS_BOOL bSet);
void    HAL_PVR_SetBuf(MS_U32 u32PVREng , MS_U32 u32StartAddr0, MS_U32 u32BufSize0, MS_U32 u32StartAddr1, MS_U32 u32BufSize1);
void    HAL_PVR_SetStr2Miu_StartAddr(MS_U32 u32PVREng, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1);
void    HAL_PVR_SetStr2Miu_MidAddr(MS_U32 u32PVREng, MS_U32 u32MidAddr0, MS_U32 u32MidAddr1);
void    HAL_PVR_SetStr2Miu_EndAddr(MS_U32 u32PVREng, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1);
MS_U32  HAL_PVR_GetWritePtr(MS_U32 u32PVREng);
void    HAL_PVR_SetStrPacketMode(MS_U32 u32PVREng, MS_BOOL bSet);
void    HAL_PVR_SetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32Stamp);
MS_U32  HAL_PVR_GetPVRTimeStamp(MS_U32 u32PVREng);
void    HAL_PVR_TimeStamp_Stream_En(MS_U32 u32PVREng, MS_BOOL bEnable);
void    HAL_PVR_SetPVRTimeStamp_Stream(MS_U32 u32PVREng, MS_U32 u32Stamp);
void    HAL_PVR_TimeStamp_Sel(MS_U32 u32PVREng, MS_BOOL bLocal_Stream);
void    HAL_PVR_PauseTime_En(MS_U32 u32PVREng, MS_BOOL bEnable);
void    HAL_PVR_SetPauseTime(MS_U32 u32PVREng, MS_U32 u32PauseTime);
void    HAL_PVR_GetEngSrc(MS_U32 u32EngDst, TSP_SRC_SEQ *eSrc);
MS_BOOL HAL_TSP_CAPVR_SPSEnable(MS_U32 u32Eng, MS_U16 u16CaPvrMode, MS_BOOL bEnable);
void    HAL_TSP_SPD_Bypass_En(MS_BOOL bByPassEn);
void    HAL_TSP_PVR_SPSConfig(MS_U32 u32PVREng, MS_BOOL CTR_mode);
void    HAL_TSP_FileIn_SPDConfig(MS_U32 tsIf, MS_BOOL CTR_mode);

//=========================FQ================================
MS_BOOL HAL_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, MS_U32 u32FQSrc);
MS_U32  HAL_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng);
MS_BOOL HAL_TSP_FQ_FLT_NULL_PKT(MS_U32 u32FQEng, MS_BOOL bFltNull);
MS_BOOL HAL_TSP_FQ_MuxOutPathSrc(MS_U32 u32FQEng, TSP_FQ_MUX_OUT_SRC *peSrc, MS_BOOL bSet);

//=========================HCMD================================
MS_U32  HAL_TSP_HCMD_GetInfo(MS_U32 u32Type);
MS_BOOL HAL_TSP_HCMD_BufRst(MS_U32 u32Value);
MS_U32  HAL_TSP_HCMD_Read(MS_U32 u32Addr);
MS_BOOL HAL_TSP_HCMD_Write(MS_U32 u32Addr, MS_U32 u32Value);
MS_BOOL HAL_TSP_HCMD_Alive(void);
void    HAL_TSP_HCMD_SecRdyInt_Disable(MS_U32 FltId ,MS_BOOL bDis);
MS_U32  HAL_TSP_HCMD_Dbg(MS_U32 u32Enable);
void    HAL_TSP_HCMD_SET(MS_U32 mcu_cmd, MS_U32 mcu_data0, MS_U32 mcu_data1);
void    HAL_TSP_HCMD_GET(MS_U32* pmcu_cmd, MS_U32* pmcu_data0, MS_U32* pmcu_data1);

//=========================INT================================
void   HAL_TSP_INT_Enable(MS_U32 u32Mask);
void   HAL_TSP_INT_Disable(MS_U32 u32Mask);
void   HAL_TSP_INT_ClrHW(MS_U32 u32Mask);
MS_U32 HAL_TSP_INT_GetHW(void);
void   HAL_TSP_INT_ClrSW(void);
MS_U32 HAL_TSP_INT_GetSW(void);

//=========================Mapping================================
TSP_PCR_SRC     HAL_TSP_FltSrc2PCRSrc_Mapping(TSP_PIDFLT_SRC ePidFltSrc);
TSP_PIDFLT_SRC  HAL_TSP_PktDmx2FltSrc_Mapping(TSP_SRC_SEQ eSrc);
MS_U32          HAL_TSP_FltSrc2PktDmx_Mapping(TSP_PIDFLT_SRC ePidFltSrc);
FILEENG_SEQ     HAL_TSP_FilePath2Tsif_Mapping(MS_U32 u32FileEng);
MS_U32          HAL_TSP_TsifMapping(TSP_HAL_TSIF u32TSIF, MS_BOOL bFileIn);
TSP_SRC_SEQ     HAL_TSP_Eng2PktDmx_Mapping(MS_U32 u32Eng);
FILEENG_SEQ     HAL_TSP_GetDefaultFileinEng(void);
MS_U32          HAL_TSP_PidFltDstMapping(TSP_PIDFLT_DST eDstType, MS_U32 u32Eng);
MS_U32          HAL_TSP_Tsif2Fq_Mapping(MS_U32 u32Tsif);
TSP_SRC_SEQ     HAL_TSP_Debug_Flow2PktDmx_Mapping(TSP_HAL_FLOW eFlow);
TSP_TS_PAD      HAL_TSP_3WirePadMapping(MS_U8 u8Pad3WireId);

//========================DSCMB Functions===================================
MS_BOOL         HAL_DSCMB_GetStatus(MS_U32 u32PktDmx, MS_U32 u32GroupId, MS_U32 u32PidFltId, MS_U32 *pu32ScmbSts);

//========================MOBF Functions=====================================
void    HAL_TSP_Filein_MOBF_Enable(FILEENG_SEQ eFileEng, MS_BOOL bEnable, MS_U32 u32Key);
void    HAL_PVR_MOBF_Enable(MS_U32 u32PVREng, MS_BOOL bEnable, MS_U32 u32Key);

//========================Protection range===================================
void    HAL_TSP_OR_Address_Protect_En(MS_BOOL bEn);
void    HAL_TSP_OR_Address_Protect(MS_PHY u32AddrH, MS_PHY u32AddrL);
void    HAL_TSP_SEC_Address_Protect_En(MS_BOOL bEn);
void    HAL_TSP_SEC_Address_Protect(MS_U8 u8SecID, MS_PHY u32AddrH, MS_PHY u32AddrL);
void    HAL_TSP_PVR_Address_Protect_En(MS_U32 u32PVREng,MS_BOOL bEnable);
void    HAL_TSP_PVR_Address_Protect(MS_U32 u32PVREng, MS_PHY u32AddrH, MS_PHY u32AddrL);
void    HAL_TSP_FILEIN_Address_Protect_En(FILEENG_SEQ eFileEng,MS_BOOL bEnable);
void    HAL_TSP_FILEIN_Address_Protect(FILEENG_SEQ eFileEng,MS_PHY u32AddrH, MS_PHY u32AddrL);
void    HAL_TSP_MMFI_Address_Protect_En(MS_U32 u32MMFIEng,MS_BOOL bEnable);
void    HAL_TSP_MMFI_Address_Protect(MS_U32 u32MMFIEng,MS_PHY u32AddrH, MS_PHY u32AddrL);

//========================Debug table=============================
// @TODO Renaming Load and Get
void    HAL_TSP_Debug_LockPktCnt_Src(MS_U32 u32TsIf);
void    HAL_TSP_Debug_LockPktCnt_Load(MS_U32 u32TsIf, MS_BOOL bEn);
MS_U16  HAL_TSP_Debug_LockPktCnt_Get(MS_U32 u32TsIf, MS_BOOL bLock);            // @NOTE: prototype is changed (ref: Keres-series line)
void    HAL_TSP_Debug_LockPktCnt_Clear(MS_U32 u32TsIf);
void    HAL_TSP_Debug_ClrSrcSel(TSP_SRC_SEQ eClrSrc);
void    HAL_TSP_Debug_AvPktCnt_Src(TSP_DST_SEQ eAvType, TSP_SRC_SEQ ePktDmxId);
void    HAL_TSP_Debug_AvPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn);
MS_U16  HAL_TSP_Debug_AvPktCnt_Get(TSP_DST_SEQ eAvType);
void    HAL_TSP_Debug_AvPktCnt_Clear(TSP_DST_SEQ eAvType);

// @TODO Implement Drop and Dis Hal
void    HAL_TSP_Debug_DropDisPktCnt_Src(TSP_DST_SEQ eAvType, TSP_SRC_SEQ ePktDmxId);
void    HAL_TSP_Debug_DropPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn);
void    HAL_TSP_Debug_DisPktCnt_Load(TSP_DST_SEQ eAvType, MS_BOOL bEn, MS_BOOL bPayload);
MS_U16  HAL_TSP_Debug_DropDisPktCnt_Get(TSP_DST_SEQ eAvType, MS_BOOL bDrop);    // @NOTE: prototype is changed (ref: Keres-series line)
void    HAL_TSP_Debug_DropPktCnt_Clear(TSP_DST_SEQ eAvType);
void    HAL_TSP_Debug_DisPktCnt_Clear(TSP_DST_SEQ eAvType);

void    HAL_TSP_Debug_ErrPktCnt_Src(MS_U32 u32TsIf);
void    HAL_TSP_Debug_ErrPktCnt_Load(MS_U32 u32TsIf, MS_BOOL bEn);
MS_U16  HAL_TSP_Debug_ErrPktCnt_Get(MS_U32 u32TsIf);
void    HAL_TSP_Debug_ErrPktCnt_Clear(MS_U32 u32TsIf);

void    HAL_TSP_Debug_InputPktCnt_Src(MS_U32 u32TsIf);                          // @NOTE: need to delete
void    HAL_TSP_Debug_InputPktCnt_Load(MS_U32 u32TsIf,MS_BOOL bEn);             // @NOTE: need to delete
MS_U16  HAL_TSP_Debug_InputPktCnt_Get(void);                                    // @NOTE: need to delete
void    HAL_TSP_Debug_InputPktCnt_Clear(MS_U32 u32Tsif);                        // @NOTE: need to delete

//========================MergeStream Functions=============================
void    HAL_TSP_PktConverter_Init(void);
MS_BOOL HAL_TSP_PktConverter_PktMode(MS_U8 u8Path, TSP_HAL_PKT_MODE ePktMode);
MS_BOOL HAL_TSP_PktConverter_SetSrcId(MS_U8 u8Path, MS_U8 u8Idx, MS_U8 *pu8SrcId, MS_BOOL bSet);
MS_BOOL HAL_TSP_PktConverter_SetSyncByte(MS_U8 u8Path, MS_U8 u8Idx, MS_U8 *pu8SyncByte, MS_BOOL bSet);
void    HAL_TSP_PktConverter_ForceSync(MS_U8 u8Path, MS_BOOL bEnable);
void    HAL_TSP_PktConverter_SrcIdFlt(MS_U8 u8Path, MS_BOOL bEnable);
void    HAL_TSP_PcrFlt_SetSrcId(MS_U32 pcrFltId, MS_U32 u32SrcId);
void    HAL_TSP_Reset_TSIF_MergeSetting(MS_U8 u8Path);

//==========================TSIO ============================================
void HAL_TSP_Module_Reset(TSP_HAL_RESET_CTRL ePath, MS_U32 u32Idx, MS_BOOL bEn);
void HAL_TSP_CLK_GATING(TSP_HAL_GATING ePath, MS_U32 u32eng, MS_BOOL bEn);

#endif // #ifndef __HAL_TSP_H__
