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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// @file  mailbox.h
/// @brief CPU mailbox header File
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "MsTypes.h"

#define INTERFACE extern

////////////////////////////////////////////////////////////////////////////////
// Define
////////////////////////////////////////////////////////////////////////////////

#define MB_CMDBIT_FIRE                          0x01
#define MB_CMDBIT_INSTANT_READ                  0x02
#define MB_CMDBIT_INSTANT_HANDLE                0x04

////////////////////////////////////////////////////////////////////////////////
// Mail Box Structure
////////////////////////////////////////////////////////////////////////////////

typedef struct _mail_box {
    MS_U8 u8CmdClass;
    MS_U8 u8CmdIdx;
    MS_U8 u8Param[10];
    MS_U8 ParamCnt         : 4;
    MS_U8 CMDType          : 3;
    MS_U8 Busy             : 1;
} MailBox, *PMailBox;

typedef struct _Mail_Box_Cmd_Queue {
    MS_U8 u8CmdIdx;
    MS_U8 u8ParamCnt;
    MS_U8 u8Param[10];
} MailBoxCmdQueue, *PMailBoxCmdQueue;

typedef struct _Cmd_Queue_Info
{
    MS_U8 ReadPtr;
    MS_U8 QLen;
} CmdQueueInfo, *pCmdQueueInfo;
/*
typedef struct _Cmd_Queue_Info
{
    MS_U8 ReadPtr   :4;
    MS_U8 QLen        :4;
} CmdQueueInfo, *pCmdQueueInfo;
*/

typedef enum
{
    MB_CMD_TYPE_DEFAULT,
    MB_CMD_TYPE_INSTANT_READ,
    MB_CMD_TYPE_INSTANT_HANDLE,
} MB_CMD_TYPE;

//-------------------------------
// MB_CLASS
//-------------------------------

//-------------------------------
// MB_CLASS_JPEG
//-------------------------------
typedef enum
{
    MB_JPEG_CMD_NONE                     = 0x00,

    // 51 --> Aeon
    MB_JPEG_CMD_RESET                    = 0x11,
    MB_JPEG_CMD_GET_STATUS               = 0x12,
    MB_JPEG_CMD_FEED_DATA_SUCCESS        = 0x13,
    MB_JPEG_CMD_DECODE_HEADER            = 0x15,
    MB_JPEG_CMD_DECODE_LINE              = 0x16,
    MB_JPEG_CMD_GET_PIC_INFO             = 0x17,
    MB_JPEG_CMD_GET_PIC_ORIGINAL_SIZE    = 0x18,
    MB_JPEG_CMD_SET_RATIO                = 0x19,
    MB_JPEG_CMD_DECODE_INIT              = 0x20,
    MB_JPEG_CMD_STOP_DECODE              = 0x21,

    // Aeon --> 51
    MB_JPEG_CMD_ASK_4_DATA               = 0x80,
    MB_JPEG_CMD_RETURN_STATUS            = 0x81,
    MB_JPEG_CMD_RETURN_PIC_INFO          = 0x82,
    MB_JPEG_CMD_RETURN_PIC_ORIGINAL_SIZE = 0x83,
    MB_JPEG_CMD_RETURN_LOOP_OVER         = 0x84,
    MB_JPEG_CMD_GET_RESOLUTION           = 0x85,
    MB_JPEG_CMD_STOP_DECODE_ACK          = 0x86,
} MB_JPEG_CMD;

typedef enum
{
    e_Idle          = 0x01,
    e_DecodeErr     = 0x02,
    e_DecodeHeader  = 0x03,
    e_DecodeLine    = 0x04,
    e_DecodeDone    = 0x05,
    e_Init          = 0x06,
} EN_DECODE_STATE;

//-------------------------------
// MB_CLASS_JPEG_EN
//-------------------------------
typedef enum
{
    MB_JPEG_EN_CMD_NONE                     = 0x00,

    // 51 --> Aeon
    MB_JPEG_EN_CMD_SET_READ_BUFF_INFO       = 0x11,
    MB_JPEG_EN_CMD_SET_INTERNAL_BUFF_INFO   = 0x12,
    MB_JPEG_EN_CMD_SET_WRITE_BUFF_INFO      = 0x13,
    MB_JPEG_EN_CMD_SET_RESOLUTION_INFO      = 0x14,

    // Aeon --> 51
    MB_JPEG_EN_RESULT                       = 0x80,
} MB_JPEG_EN_CMD;

typedef enum
{
    MB_CLASS_SYSTEM         = 0,
    MB_CLASS_INPUT          = 1,
    MB_CLASS_TSP            = 2,
    MB_CLASS_CHANNEL        = 3,
    MB_CLASS_MEMORY         = 4,
    MB_CLASS_MPEG           = 5,
    MB_CLASS_VIDEO          = 6,
    MB_CLASS_AUDIO          = 7,
    MB_CLASS_MHEG5_STATE    = 8,
    MB_CLASS_MVF            = 9,
    MB_CLASS_GE             = 10,
    MB_CLASS_CI             = 11, // rename unused "MB_CLASS_NUM" as "MB_CLASS_CI" for CIMHEG usage
    MB_CLASS_TEST           = 12,
    MB_CLASS_MAD            = 13,
    MB_CLASS_MPEG2_FS       = 14,
    MB_CLASS_MPEG2          = 15,
    MB_CLASS_BMP            = 16,
    MB_CLASS_PNG            = 17,
    MB_CLASS_JPEG           = 18,
    MB_CLASS_MJPEG          = 19,
    MB_CLASS_JPEG_ENCODER   = 20,
    MB_CLASS_VDPLAYER_FS    = 21,
    MB_CLASS_VDPLAYER       = 22,
    MB_CLASS_RMPLAYER_FS    = 23,
    MB_CLASS_RMPLAYER       = 24,
    MB_CLASS_TSPLAYER_FS    = 25,
    MB_CLASS_TSPLAYER       = 26,
    MB_CLASS_LZSS           = 27,
    MB_CLASS_CAPE           = 28,
    MB_CLASS_CC             = 29,
    MB_CLASS_DLNA           = 30,
    MB_CLASS_DUMMY_LOOP     = 31,
    MB_CLASS_CHAKRA_SUBSYS  = 32,
    MB_CLASS_FCNTL          = 33,
    MB_CLASS_IRKEY          = 34,
    MB_CLASS_BTPD           = 35,
    MB_CLASS_OBAMA_CMD      =36,
    MB_CLASS_OBAMA_APP      =37,
    MB_CLASS_KTV               = 38,
    MB_CLASS_CIPLUS         =39,
    MB_CLASS_PRINT_MESSAGE      =40,
    MB_CLASS_SHWFS         =41,
    // 42: reserved
    MB_CLASS_STILLIMAGE             = 43,
    MB_CLASS_MAILBOX_FILE_IO_FAST   = 44,
    MB_CLASS_MAILBOX_FILE_IO        = 45,

    MB_CLASS_NET_DEBUG = 46, //add by Ian //46
} MB_CLASS;

//-------------------------------
// MB_CLASS_SYSTEM
//-------------------------------
typedef enum
{
    MB_SYS_CMD_QUERY_STATUS,
    MB_SYS_CMD_SYS_TIMER,
    MB_SYS_CMD_PAUSE_AEON,
} MB_SYSTEM_CLASS;

//-----------------------------------------------
// Class :: MPEG
//-----------------------------------------------
typedef enum
{
    MB_MPEG_IFRAME_DECODE            = 0x80,
    MB_MPEG_IFRAME_ENABLE            = 0x81,
    MB_MPEG_IFRAME_SET_POSITION      = 0x82,
    MB_MPEG_IFRAME_SCALE             = 0x83,
    MB_MPEG_IFRAME_SET_POSITION_CROP = 0x84,
    MB_MPEG_IFRAME_SCALE_CROP        = 0x85,
    MB_MPEG_IFRAME_SET_VIDOFFSET     = 0x86,
    MB_MPEG_IFRAME_BOXSIZE           = 0x87
} MB_MPEG;

//-------------------------------
// MB_CLASS_INPUT
//-------------------------------
typedef enum
{
    MB_INPUT_CMD_IR,
    MB_INPUT_CMD_KEYPAD,
    MB_INPUT_CMD_JOYSTICK,
    MB_INPUT_CMD_BIKE_IR

} MB_INPUT_CLASS;

//-------------------------------
// MB_CLASS_TSP
//-------------------------------
typedef enum
{
    MB_TSP_CMD_ADD_PID,
    MB_TSP_CMD_DEL_PID,
} MB_TSP_CLASS;

//-------------------------------
// MB_CLASS_MEMORY
//-------------------------------
typedef enum
{
    MB_MEM_CMD_MAPINFO,
} MB_INPUT_MEMORY;

//-------------------------------
// MB_CLASS_VIDEO
//-------------------------------
typedef enum
{
    MB_VID_CMD_CHANGESTREAM     = 0,
    MB_VID_CMD_SETAFD           = 1,
    MB_VID_CMD_SETARC_PART1     = 2,
    MB_VID_CMD_SETARC_PART2     = 3,
    MB_VID_CMD_SETVTG           = 4,
    MB_VID_CMD_SETVTG2          = 5,
    MB_VID_CMD_SETVTG_SETTING   = 6,
} MB_VIDEO_CMD_INDEX;

//-------------------------------
// MB_CLASS_MHEG5_STATE
//-------------------------------
typedef enum
{
    //HK => CP
    MB_MHEG5_STATE_CMD_AUD_PID_PNL_RATIO    = 0x00,
    MB_MHEG5_STATE_CMD_OS_FB                = 0x01,
    MB_MHEG5_STATE_CMD_SETARC_PART1         = 0x02,
    MB_MHEG5_STATE_CMD_SETARC_PART2         = 0x03,

    MB_MHEG5_STATE_CMD_PNL_INFO             = 0x05,
    MB_MHEG5_STATE_CMD_SEMAPHORE            = 0x06,
    MB_MHEG5_STATE_CMD_OS_BAK_COLOR         = 0x07,
    MB_MHEG5_STATE_CMD_RT_FB                = 0x08,
    MB_MHEG5_STATE_CMD_DLA_FB               = 0x09,
    MB_MHEG5_STATE_CMD_IFRAME_CONTROL       = 0x0A,
    MB_MHEG5_STATE_CMD_SETARC_SETTING       = 0x0B,
    MB_MHEG5_STATE_CMD_MMAP                 = 0x0C,
    MB_MHEG5_STATE_CMD_HKMB_READY           = 0x0D,

    //CP => HK
    MB_MHEG5_STATE_CMD_EXIT                 = 0x80,
    MB_MHEG5_STATE_CMD_AUTOBOOT             = 0x81,
    MB_MHEG5_STATE_CMD_CHANNEL_CHANGE       = 0x82,
    MB_MHEG5_STATE_CMD_MHEG5_TO_MENU        = 0x83,
    MB_MHEG5_STATE_CMD_RESOURCE_CHANGE      = 0x84,
    MB_MHEG5_STATE_CMD_KEY_REGISTER         = 0x85,



    MB_MHEG5_STATE_CMD_MMAP_OK              = 0x8C,
    MB_MHEG5_STATE_CMD_CPMB_READY           = 0x8D,
} MB_MHEG5_STATE_CLASS;

//-------------------------------
// MB_CLASS_CAPE
//-------------------------------
typedef enum
{
    //HK51 => AEON
    MB_CAPE_INIT_RES                 = 0x00,
    MB_CAPE_GWIN_CREATE_RES          = 0x10,
    MB_CAPE_GWIN_ENABLE_RES          = 0x11,
    MB_CAPE_GWIN_MOVE_RES            = 0x12,
    MB_CAPE_GWIN_DESTROY_RES         = 0x13,
    MB_CAPE_VOP_INIT_RES             = 0x14,
    MB_CAPE_VOP_SETBLUESCREEN_RES    = 0x15,
    MB_CAPE_VOP_MOVE_RES             = 0x16,
    MB_CAPE_VOP_DESTROY_RES          = 0x17,
    MB_CAPE_I8_SET_RES               = 0x20,
    MB_CAPE_I8_GET_RES               = 0x21,
    MB_CAPE_FILE_LEN_RES             = 0x30,
    MB_CAPE_FILE_COPY_RES            = 0x31,
    #ifdef ENABLE_BIKEIR
    MB_CAPE_INIT_BIKEIR_RES          = 0x40,
    #endif
    #ifdef ENABLE_DMP
    MB_CAPE_AUDIO_INIT_RES           = 0x50,
    #endif

    //AEON => HK51
    MB_CAPE_INIT_REQ                 = 0x80,
    MB_CAPE_GWIN_CREATE_REQ          = 0x90,
    MB_CAPE_GWIN_ENABLE_REQ          = 0x91,
    MB_CAPE_GWIN_MOVE_REQ            = 0x92,
    MB_CAPE_GWIN_DESTROY_REQ         = 0x93,
    MB_CAPE_VOP_INIT_REQ             = 0x94,
    MB_CAPE_VOP_SETBLUESCREEN_REQ    = 0x95,
    MB_CAPE_VOP_MOVE_REQ             = 0x96,
    MB_CAPE_VOP_DESTROY_REQ          = 0x97,
    MB_CAPE_AUDIO_INIT_REQ           = 0xA0,
    MB_CAPE_I8_SET_REQ               = 0xB0,
    MB_CAPE_I8_GET_REQ               = 0xB1,
    MB_CAPE_APP_FINALIZED_SIG        = 0xC0,
    MB_CAPE_APP_ALIVE_SIG            = 0xC1,
    MB_CAPE_APP_INITIALIZED_SIG      = 0xC2,
    MB_CAPE_FILE_LEN_REQ             = 0xD0,
    MB_CAPE_FILE_COPY_REQ            = 0xD1,
    #ifdef ENABLE_BIKEIR
    MB_CAPE_INIT_BIKEIR_REQ          = 0xD2,
    #endif
    MB_CAPE_NET_IP_INFO              = 0xE0,
    MB_CAPE_NET_MASK_INFO            = 0xE1,
    MB_CAPE_NET_GATEWAY_INFO         = 0xE2,
    MB_CAPE_NET_DNS_INFO             = 0xE3,

} MB_CAPE_CLASS;

//-------------------------------
// MB_CLASS_GE_STATE
//-------------------------------
typedef enum
{
    MB_GE_STATUS                       = 0,
    MB_Relase_GE_Mutex_BY_HK51         = 1,
    MB_Request_GE_Mutex_BY_HK51        = 2,
} MB_GE_STATE_CLASS;

//-------------------------------
// MB_CLASS_MVF
//-------------------------------
typedef enum
{
    //HK51 => AEON
    MB_MVF_CMD_LOAD_FACE           = 0,
    MB_MVF_CMD_GENERATE_BITMAP     = 1,
    MB_MVF_CMD_DONE_FACE           = 2,

    //AEON => HK51
    MB_MVF_CMD_LOAD_DONE           = 0x80,
    MB_MVF_CMD_GENERATE_DONE       = 0x81,

} MB_MVF_CLASS;

/// Mail box return state
typedef enum
{
    MB_RETURN_OK,                                               ///< Mail box return OK
    MB_RETURN_MHEG5_EXIT,                                               ///< Mail box return EXIT
    MB_RETURN_MHEG5_AUTOBOOT,                                               ///< Mail box return AUTOBOOT
    MB_RETURN_MHEG5_CHANNELCHANGE,                                               ///< Mail box return Channelchange
    MB_RETURN_MVF_DONE,                                               ///< Mail box return MVF Done
    MB_RETURN_MVF_ERROR,                                               ///< Mail box return MVF Erroe
    MB_RETURN_MHEG5_SELECTED_QUIT_SEL,                                               ///< Mail box return quit by "select key"
    MB_RETURN_MHEG5_SELECTED_QUIT_0,                                               ///< Mail box return quit by selected state "0 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_1,                                               ///< Mail box return quit by selected state "1 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_2,                                               ///< Mail box return quit by selected state "2 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_3,                                               ///< Mail box return quit by selected state "3 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_4,                                               ///< Mail box return quit by selected state "4 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_5,                                               ///< Mail box return quit by selected state "5 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_6,                                               ///< Mail box return quit by selected state "6 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_7,                                               ///< Mail box return quit by selected state "7 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_8,                                               ///< Mail box return quit by selected state "8 key"
    MB_RETURN_MHEG5_SELECTED_QUIT_9,                                               ///< Mail box return quit by selected state "9 key"
    MB_RETURN_51_STATE_CHANNEL_CHANGE,                                               ///< Mail box return 51 state channel change
    MB_RETURN_51_STATE_MHEG5_TO_MENU,                                               ///< Mail box return 51 state MHEG5 to Menu
    MB_RETURN_LZSS_COMPRESS_DONE,
    MB_RETURN_LZSS_COMPRESS_FAILED,
} MB_RETURN_STATE;

//-------------------------------
// MB_CLASS_MPEG2_FS
//-------------------------------
//Issue Command Index
#define MB_MPEG2_FS_PARSE                       0x01        //[0]~[3]: PS buffer start
                                                            //[4]~[7]: PS buffer end
#define MB_MPEG2_FS_INFO                        0x02        //[0]~[3]: File length
#define MB_MPEG2_FS_SET_POS                     0x03        //[0]~[3]: File position
#define MB_MPEG2_FS_PRE_PARSE                   0x04        //[0]~[3]: PS buffer start
                                                            //[4]~[7]: PS buffer end
//Receive Command Index
#define MB_MPEG2_FS_PARSE_COMPLETED             0x80        //[0]: 0: forward Seek. 1: backward Seek.
                                                            //[1]~[4]: Offset
                                                            //[5]~[8]: Request size
#define MB_MPEG2_FS_PARSE_COMPLETED_DATA        0x83        //[0]: 0: forward Seek. 1: backward Seek.
 															//[1]~[4]: video demux num
                                                            //[5]~[8]: audio demux num
#define MB_MPEG2_FS_PRE_PARSE_COMPLETED         0x81        //[0]: 0: Can't support decode, 1: Can support decode.
                                                            //[1]: MPEG file type.
                                                            //[2]: MPEG file audio count.
#define MB_MPEG2_FS_PRE_PARSE_COMPLETED_DATA    0x84        //[0]: subpic count
                                                            //[1]~[3]: sub pic ID


#define MB_MPEG2_FS_PRE_PARSE_NEXT_BUFF         0x82        //
//-------------------------------
// MB_CLASS_MPEG2
//-------------------------------
//Issue Command Index
#define MB_MPEG2_SET_VID_PARS_TYPE              0x01        //[0]: Parse type.
                                                            //[1]~[4]: Output buffer start
                                                            //[5]~[8]: Output buffer end
#define MB_MPEG2_SET_VID_RD_PTR                 0x03        //[0]~[3]: TSP Read buffer pointer
#define MB_MPEG2_SET_AUD_PARS_TYPE              0x04        //[0]: Parse type.
                                                            //[1]~[4]: Output buffer start
                                                            //[5]~[8]: Output buffer end
#define MB_MPEG2_SET_AUD_RD_PTR                 0x06
#define MB_MPEG2_PARS_ENABLE                    0x08        //[0]: 0x1, enable video parsing. 0x2, enable audio parsing.
                                                            //[1]~[2]: Video PID (type is TS)
                                                            //[3]~[4]: Audio PID (type is TS)
#define MB_MPEG2_PARS_DISABLE                   0x09        //[0]: 0x1, disable video parsing. 0x2, disable audio parsing.
#define MB_MPEG2_PARS_CTL                       0x0a        //[0]: 0x0, Stop parsing process.
                                                            //     0x1, Normal parsing process.
                                                            //     0x2, Forward I frame parsing process.
                                                            //     0x3, Backward I frame parsing process.
#define MB_MPEG2_GET_PTS_POS                    0x0b        //[0]~[3]: PTS

#define MB_MPEG2_SET_AUD_INFO                   0x0c        //[0]: Audio Type
                                                            //[1]: Audio stream_id or sub_Stream_id

#define MB_MPEG2_SET_PCR_INFO                   0x0d        //[0]~[1]: PCR PID

#define MB_MPEG2_RESET_TSP_BUFF_PTR             0x0e        //[0]~[3]: TSP buffer pointer
#define MB_MPEG2_SET_PTS_SHARE_ADR             	0x0f        //[0]~[3]: PTS buffer pointer
#define MB_MPEG2_SET_SUBPIC_INFO                0x10        //[0]: subpic stream_id



//Receive Command Index
#define MB_MPEG2_SET_STC                        0x80        //[0]: STC bit 32, [1]: STC bit 31~24,
                                                            //[2]: STC bit 23~16, [3]: STC bit 15~8, [4]: STC bit 7~0,
#define MB_MPEG2_SET_TSP_PTR                    0x81        //[0]~[3]: TSP buffer write pointer
#define MB_MPEG2_SET_VID_ES_PTR                 0x82
#define MB_MPEG2_SET_AUD_ES_PTR                 0x83        //[0]~[3]: ES buffer write pointer
                                                            //[4]~[7]: PTS

#define MB_MPEG2_SET_TS_PGINFO                  0x84        //[0]: Program Index
                                                            //[1]~[2]: Program number
                                                            //[3]~[4]: PCR Pid
                                                            //[5]~[6]: Video Pid
                                                            //[7]~[8]: Audio Pid
                                                            //[9]: Audio type
#define MB_MPEG2_SET_TS_PGINFO2                 0x85        //[0]: Program Index
                                                            //[1]~[2]: Sub-pic Pid
#define MB_MPEG2_SET_TS_PGCOUNT                 0x86        //[0]~[3]: Program count
#define MB_MPEG2_SET_DMA_REQUEST                0x87        //[0]~[2]: Dst addr (256 bytes alignment)
                                                            //[3]~[5]: Src addr (256 bytes alignment)
                                                            //[6]~[9]: DMA size (byte unit)
#define MB_MPEG2_PARS_SPU                       0x88

//-------------------------------
// MB_CLASS_TSPLAYER
//-------------------------------
//MB_VDPLAYER_SETUP_DISPLAY  Params[0]
//#define MB_TS_SET_DISPLAY                       0x01
//#define MB_TS_SETUP_DISPLAY                     0x02
//#define MB_TS_SET_FREEZEIMG                     0x03
//#define MB_TS_VIDMONITOR                        0x04
//#define MB_TS_VID_SETH264OUTPUT                 0x05
//#define MB_TS_SET_STC                           0x06

//MB_VDPLAYER_SCALER_API CommandIndex
//Params[0]
//#define MB_msAPI_Scaler_SetBlueScreen               0x01//arg 2( ENABLE, FREE_RUN_COLOR_BLACK );
//#define MB_msAPI_Scaler_SetDisplayWindowStatus      0x02//arg 1 ( displayWindowStatue |= DISPLAYWINDOW_HDTV);
//#define MB_msAPI_Scaler_SetCustomerDisplayWindow    0x03//arg 5(TRUE, MOVIE_PREVIEW_X, MOVIE_PREVIEW_Y, MOVIE_PREVIEW_WIDTH, MOVIE_PREVIEW_HEIGHT);
//#define MB_msAPI_Scaler_SetTimingAndWindow          0x04//arg 3 ( SYS_INPUT_SOURCE_TYPE, &stVOPTiming, stSystemInfo.enAspectRatio);
//#define MB_msAPI_Scaler_GetDisplayWindowStatus      0x05//arg 0;
//#define msAPI_Scaler_SetFreezeImg                 0x06//arg 1;
//#define MB_msAPI_Scaler_GenerateBlackVideo          0x07//arg 1 (DISABLE);

//MB_VDPLAYER_AUD_API CommandIndex
//Params[0]
#define MB_msAPI_AUD_AdjustAudioFactor       0x01//msAPI_AUD_AdjustAudioFactor(E_ADJUST_SWITCH_AUDIOSTREAMMODE, tsSrvInfo.stAudInfo[m_u8SoundTrackIdx].wAudType, 0);
//#define MB_msAPI_AUD_SPDIF_SetMode           0x02//msAPI_AUD_SPDIF_SetMode(MSAPI_AUD_SPDIF_NONPCM);
//#define MB_msAPI_AUD_SetStreamID_Mod         0x03//msAPI_AUD_SetStreamID_Mod(TRUE);
//#define MB_msAPI_AUD_DTV_HDMI_CFG            0x04//msAPI_AUD_DTV_HDMI_CFG(FALSE);  // DTV mod set False
//#define MB_msAPI_AUD_SetDecCommand           0x05//msAPI_AUD_SetDecCommand(MSAPI_AUD_RESYNC);
//#define MB_msAPI_AUD_SetPaser                0x06//msAPI_AUD_SetPaser(MSAPI_AUD_PASER_PLAY);
//#define MB_msAPI_AUD_SetFreeRun              0x07//msAPI_AUD_SetFreeRun(MSAPI_AUD_DVB_FreeRunMode_FreeRun);

//MB_VDPLAYER_VID_API CommandIndex
//Params[0]
//#define MB_msAPI_VID_MVDInit                 0x01//msAPI_VID_MVDInit();
//#define MB_msAPI_VID_VOPInit                 0x02//msAPI_VID_VOPInit();
//#define MB_msAPI_VID_VOPEnable               0x03//emsAPI_VID_VOPEnable( TRUE );
//#define MB_msAPI_VID_SetVOPClk               0x04//msAPI_VID_SetVOPClk( ENABLE );
//#define MB_msAPI_VID_Command                 0x05//msAPI_VID_Command(MSAPI_VID_STOP);
//#define MB_msAPI_VID_Init                    0x06//msAPI_VID_Init();
//#define MB_msAPI_VID_AVCH264EnableParserStop 0x07//msAPI_VID_AVCH264EnableParserStop(x) MDrv_AVCH264_EnableParserStop(x)
//#define MB_msAPI_VID_MVDResetBuffAddr        0x08//msAPI_VID_MVDResetBuffAddr();
//#define MB_msAPI_VID_MVDCommand              0x08//msAPI_VID_MVDCommand(MSAPI_VID_PLAY);//only mpeg2, need change as msAPI_VID_Command(MSAPI_VID_PLAY) if h264 add in;
//#define MB_msAPI_VID_GetPicCount             0x09//msAPI_VID_GetPicCount();
//#define MB_msAPI_VID_AVCH264GetDispRdy       0x0A//msAPI_VID_AVCH264GetDispRdy()
//#define MB_msAPI_VID_AVCH264GetSeqInfo       0x0B//(msAPI_VID_AVCH264GetSeqInfo(&stVidInfo))
//#define MB_msAPI_VID_GetDispRdy              0x0C//(msAPI_VID_GetDispRdy())
//#define MB_msAPI_VID_GetVidInfo              0x0D//(msAPI_VID_GetVidInfo(&stVidInfo))
//#define MB_msAPI_VID_SetH264Output           0x0E//( msAPI_VID_SetH264Output(&stVOPTiming, &stVidInfo ) == PASS )
//#define MB_msAPI_VID_VOPSetBlackBG           0x0F//msAPI_VID_VOPSetBlackBG();
//#define MB_msAPI_VID_AVCH264GetDispInfoDone  0x10//msAPI_VID_AVCH264GetDispInfoDone();
//#define MB_msAPI_VID_SetMVDOutput            0x11//( msAPI_VID_SetMVDOutput(&stVOPTiming, &stVidInfo) == PASS )
//#define MB_msAPI_VID_GetGOPCount             0x12//msAPI_VID_GetGOPCount();
//#define MB_msAPI_VID_SetTrickModeInfo        0x13//msAPI_VID_SetTrickModeInfo(1,1);
//#define MB_msAPI_VID_GetFirstFrame           0x14//msAPI_VID_GetFirstFrame()

//-------------------------------
// MB_CLASS_VDPLAYER_FS
//-------------------------------
//Receive Command Index
#define MB_VDPLAYER_FS_INIT                        0x01         //[0]~[3]: VDPlayer info buffer start
                                                                //[4]~[7]: VDPlayer data buffer start

#define MB_VDPLAYER_FS_PAR_STREAM_INFO             0x02         //[0]: no. of stream

#define MB_VDPLAYER_FS_PAR_FRAME_INFO              0x03         //[0]~[3]: Frame info buffer address

#define MB_VDPLAYER_INIT_INDEX                     0x04

#define MB_VDPLAYER_INIT_AUDIO                     0x05
#define MB_VDPLAYER_INIT_AUDIO_2                   0x06
#define MB_VDPLAYER_INIT_AUDIO_3                   0x07
#define MB_VDPLAYER_INIT_AC3                       0x08

//Issue Command Index
#define MB_VDPLAYER_FS_INIT_DONE                   0x80         //[0]: 0: success. 1: fail.


//-------------------------------
// MB_CLASS_VDPLAYER
//-------------------------------
//51-->Aeon Command Index
#define MB_VDPLAYER_INIT                    0x01
#define MB_VDPLAYER_MEM_INIT                0x02
#define MB_VDPLAYER_STATE_INIT              0x03
#define MB_VDPLAYER_RESET                   0x04
#define MB_VDPLAYER_SET_PREVIEW_MODE        0x05

#define MB_VDPLAYER_PLAY                    0x10
#define MB_VDPLAYER_STOP                    0x11
#define MB_VDPLAYER_PAUSE                   0x12
#define MB_VDPLAYER_RESUME                  0x13
#define MB_VDPLAYER_FF                      0x14
#define MB_VDPLAYER_REW                     0x15
#define MB_VDPLAYER_SET_TRICK_MODE          0x16
#define MB_VDPLAYER_GET_TRICK_MODE          0x17
#define MB_VDPLAYER_REPEAT_AB               0x18

#define MB_VDPLAYER_SET_SUBTITLE_TRACKID    0x21
#define MB_VDPLAYER_GET_SUBTITLE_STATE      0x22
#define MB_VDPLAYER_GET_VDPLAYER_INFO       0x23
#define MB_VDPLAYER_BEFORE_CAPTURE_VIDEO    0x24
#define MB_VDPLAYER_AFTER_CAPTURE_VIDEO     0x25
#define MB_VDPLAYER_SET_PLAYER_POSITION     0x26

#define MB_VDPLAYER_ACK_51ToAEON            0x30

//DivX MM Function
#define MB_VDPLAYER_GET_AUDIO_TRACK_COUNT_TO_AEON       0x41
#define MB_VDPLAYER_GET_AUDIO_TRACK_ID_TO_AEON          0x42
#define MB_VDPLAYER_SET_AUDIO_TRACK_ID_TO_AEON          0x43
//Subtitle
#define MB_VDPLAYER_SPUQUEUE_GET_DRAM_ADDR_TO_AEON      0x44
#define MB_VDPLAYER_SPUQUEUE_PUSH                       0x45
#define MB_VDPLAYER_MPEG_SUBTITLE_MAIN                  0x46
#define MB_VDPLAYER_MPEG_SET_SUBTITLE_SHOW              0x47
#define MB_VDPLAYER_MPEG_SUBTITLE_INIT_DEINIT           0x48//Init: 0 deinit:1
#define MB_VDPLAYER_MPEG_SUBTITLE_RESET                 0x49
#define MB_VDPLAYER_MPEG_SUBTITLE_SETHEADERINFO         0x4A
#define MB_VDPLAYER_MPEG_SUBTITLE_GETQUEUEINFO          0x4B
#define MB_VDPLAYER_MPEG_SUBTITLE_FILEPROCESS           0x4C
// DRM
#define MB_VDPLAYER_GET_DRM_REG_CODE                    0x50
#define MB_VDPLAYER_CHECK_DRM_AUTHORIZATION             0x51
#define MB_VDPLAYER_QUERY_DRM_RENTAL_STATUS             0x52
#define MB_VDPLAYER_GET_DRM_REG_CODE_2                  0x53
#define MB_VDPLAYER_GET_DRM_DEACTIVATION_CODE           0x54

#define MB_VDPLAYER_TS_SET_PROGRAM                      0x60

//Aeon->51 Command Index
#define MB_VDPLAYER_FILE_READ               0x80
#define MB_VDPLAYER_FILE_SEEK               0x81
#define MB_VDPLAYER_FILE_OPEN               0x82
#define MB_VDPLAYER_FILE_CLOSE              0x83
#define MB_VDPLAYER_VIDEO_INIT              0x84
#define MB_VDPLAYER_AUDIO_INIT              0x85
#define MB_VDPLAYER_SETUP_DISPLAY           0x86
#define MB_VDPLAYER_CLOSE_DISPLAY           0x87
#define MB_VDPLAYER_AUDIO_START             0x88
#define MB_VDPLAYER_FILE_READ_NONBLOCKING   0X89
#define MB_VDPLAYER_FILE_DUMP_RAW           0x8A
#define MB_VDPLAYER_GET_DISPLAY_INFO        0x8B
#define MB_VDPLAYER_SET_DISPLAY_INFO        0x8C
#define MB_VDPLAYER_FILE_SEEK_NONBLOCKING   0x8D


#define MB_VDPLAYER_ACK_AEONTo51                    0xA0
#define MB_VDPLAYER_STATE_RET                       0xA1
#define MB_VDPLAYER_STATE_RET_WAIT                  0xA2
#define MB_VDPLAYER_STATE_RET_EXIT                  0xA3
#define MB_VDPLAYER_FILE_READ_NONBLOCKING_DONE      0XA4
#define MB_VDPLAYER_FILE_SEEK_NONBLOCKING_DONE      0XA5

//TS MM Function
//#define MB_VDPLAYER_SET_INTERRUPT    0xA5
//#define MB_VDPLAYER_AUDIO_SETTING    0xA6
//#define MB_VDPLAYER_SCALER_API       0xA7
#define MB_VDPLAYER_AUD_API                             0xA8
//#define MB_VDPLAYER_VID_API          0xA9

//DivX MM Function
#define MB_VDPLAYER_GET_AUDIO_TRACK_COUNT_FROM_AEON     0x90
#define MB_VDPLAYER_GET_AUDIO_TRACK_ID_FROM_AEON        0x91

#define MB_51_PARALLEL_QLENGTH              1
#define MB_51_PARALLEL_QSIZE                13

#define MB_51_STATE_QLENGTH                 10
#define MB_51_STATE_QSIZE                   13
#define MB_MHEG5_EXIT_SELECTED              2

#define MB_51_MPEG2_QLENGTH                 24
#define MB_51_MPEG2_QSIZE                   13

#define MB_51_MPEG2_FS_QLENGTH              6
#define MB_51_MPEG2_FS_QSIZE                13

#define MB_51_TSPLAYER_QLENGTH              MB_51_MPEG2_QLENGTH
#define MB_51_TSPLAYER_QSIZE                MB_51_MPEG2_QSIZE

#define MB_51_TSPLAYER_FS_QLENGTH           MB_51_MPEG2_FS_QLENGTH
#define MB_51_TSPLAYER_FS_QSIZE             MB_51_MPEG2_FS_QSIZE
#define MB_51_VDPLAYER_QLENGTH              MB_51_MPEG2_QLENGTH // 24
#define MB_51_VDPLAYER_QSIZE                MB_51_MPEG2_QSIZE // 13

#define MB_51_VDPLAYER_FS_QLENGTH           MB_51_MPEG2_FS_QLENGTH // 4
#define MB_51_VDPLAYER_FS_QSIZE             MB_51_MPEG2_FS_QSIZE // 13

#define MB_51_IR_QLENGTH                    13
#define MB_51_IR_QSIZE                      13

#define MB_OBAMA_QLENGTH                    13
#define MB_OBAMA_QSIZE                      13

#define MB_51_MBFS_QLENGTH                  10
#define MB_51_MBFS_QSIZE                    13

#define SHWFS_CMD_QLENGTH                  10
#define SHWFS_CMD_QSIZE                    13

//Add by Ian
#define NWDEBUG_CMD_QLENGTH                  3
#define NWDEBUG_CMD_QSIZE                    13

#define MB_PRINT_MESSAGE_QLENGTH                    13
#define MB_PRINT_MESSAGE_QSIZE                      13

#define MB_CIPLUS_QLENGTH                    13
#define MB_CIPLUS_QSIZE                      13

#define MB_DLNA_QLENGTH                    24
#define MB_DLNA_QSIZE                      13

#define MB_BTPD_QLENGTH                 10
#define MB_BTPD_QSIZE                   13

#define MB_KTV_QLENGTH                 10
#define MB_KTV_QSIZE                   13

#define MB_51_APENGINE_QLENGTH                 10
#define MB_51_APENGINE_QSIZE                   13

#define MB_51_MM_QLENGTH                 10
#define MB_51_MM_QSIZE                   13

#define MB_51_MHEG5_QLENGTH                 10
#define MB_51_MHEG5_QSIZE                   13

//-----------------------------------------------
// Class :: BMP
//-----------------------------------------------
//8051->Aeon
//set bmp decoding output parameters
//para 0~3: output buffer address (para0:LSB  ;  para3:MSB)
//para 4~5: output imgage max width (para4:LSB  ;  para5:MSB)
//para 6~7: output imgage max height (para6:LSB  ;  para7:MSB)
#define MB_BMP_SET_OUTPUT_INFO      0x01


//8051->Aeon
//set bmp file data buffer parameters
//para 0: buffer index (0 or 1, please use 0 at begining of file decoding)
//para 1~4: data buffer address (para1:LSB  ;  para4:MSB)
//para 5~6: data buffer length (para5:LSB  ;  para6:MSB ; max:2048)
#define MB_BMP_SET_DATA_BUF         0x02


//Aeon->8051
//Notify 8051 that data in buffer is consumed
//para 0: buffer index (0 or 1)
#define MB_BMP_NOTIFY_BUFFER_EMPTY  0x03

//Aeon->8051
//Report this bmp image size
//para 0~1: image width (para0:LSB  ;  para1:MSB)
//para 2~3: image height (para2:LSB  ;  para3:MSB)
#define MB_BMP_NOTIFY_IMG_INFO      0x04
#define MB_BMP_STOP_DECODE          0x05
#define MB_BMP_STOP_DECODE_ACK      0x06

//-----------------------------------------------
// Class :: PNG
//-----------------------------------------------
#define MB_PNG_SET_OUTPUT_INFO      0x01
#define MB_PNG_SET_DATA_BUF         0x02
#define MB_PNG_NOTIFY_BUFFER_EMPTY  0x03
#define MB_PNG_NOTIFY_IMG_INFO      0x04
#define MB_PNG_STOP_DECODE          0x05
#define MB_PNG_NOTIFY_DECODE_END    0x06

//----------------------------------
// Get MailBox Message Return Value
//----------------------------------
typedef enum
{
    MB_GET_MSG_OK,
    MB_QUEUE_EMPTY,
    MB_ERROR_CLASS,
} MB_GETMSG_RET;

//-----------------------------------------------
//INTERFACE
//-----------------------------------------------

INTERFACE MailBox MailBoxPara;
INTERFACE void MDrv_InitMailBoxQueue(void);
INTERFACE void MDrv_SendMailBoxMsg(void);
INTERFACE void MDrv_HandleMailBox(void);
INTERFACE MS_U8 MDrv_GetMailBoxMsg(MS_U8 u8QueueType, MS_U8 *Msg);
INTERFACE MS_BOOL MDrv_IsMailBoxMsgEmpty(MS_U8 u8QueueType);
INTERFACE MS_BOOL MDrv_CheckMailBoxIdle(void);
INTERFACE MS_BOOL MDrv_CheckMailBoxReady(void);
INTERFACE MS_U8 MDrv_GetMailBoxMsgCount(MS_U8 u8QueueType);
INTERFACE void MDrv_ClearMailBoxStatus(void);
INTERFACE MS_BOOL MDrv_WaitMailBoxReady(void);

#undef INTERFACE
#endif //_MAILBOX_H_

