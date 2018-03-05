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
#ifndef __M4VD_MSG_QUE_H__
#define __M4VD_MSG_QUE_H__

#include "controller.h"



#define DISPQ_SIZE 12
#define FRAMEQ_SIZE 16

#ifdef LIGHTWEIGHT //FW31_1.8M
#define OFFSET_BASE              0x000C0000
#else
#if defined(SUPPORT_EVD) && (SUPPORT_EVD==1)
#define OFFSET_BASE              0x00100000
#else
#define OFFSET_BASE              0x000A0000
#endif
#endif

#define FW_VOL_INFO_START           (0x000+OFFSET_BASE)
#define DEBUG_BUF_START             (0x500+OFFSET_BASE)
#define DEBUG_VSYNC_BUF_START       (0xF00+OFFSET_BASE)
#define FW_FRAME_INFO_START         (0x1000+OFFSET_BASE)
#define FW_DIVX_INFO_START          (0x2000+OFFSET_BASE)
#define DEC_FRMAE_INFO_START        (0x14300+OFFSET_BASE)
#define VBBU_TABLE_START            (0x15400+OFFSET_BASE)
#define DISP_QUEUE_START            (0x17400+OFFSET_BASE)
#define VCOMMANDQ_INFO_START        (0x18400+OFFSET_BASE)
#define VDISP_COMMANDQ_START        (0x19400+OFFSET_BASE)  // 4k bytes
#define VNORMAL_COMMANDQ_START      (0x1A400+OFFSET_BASE)  // 4k bytes

#define MVD_DRAM_SIZE 0x40000   // MVD DRAM heap size, 256k

#define MVD_MAX_VSYNC_DBG_CNT       0x100

/*
Share Memory layout
  | FW_VOL_INFO | DEBUG_BUF | FW_FRAME_INFO | FW_DIVX_INFO | USERDATABUF | SLQBUF | PTSTBL | DSADDR | SCALER_INFO | DEC_FRMAE_INFO | VBBU_TABLE | VSYNC_BRIDGE
  0            500        1000            2000           3000          B000     B200     13200    14200         14300            14400         1FA00
*/

// VC1_SEQ_INFO put on 65K
#define FW_VERSION               0x08043655
#define INTERFACE_VERSION        0x00000368  //add information : u8TileMode
#define EN_SECTION_START         0x20000000

typedef struct _FW_VOL_INFO
{
    //VOL infomation
    unsigned short      vol_info;                    //0
//    D[5] short_video_header; //1
//    D[4] vol_interlaced;
//    D[3] vol_qpel;
//    D[2] vol_rsync_marker_disable;
//    D[1] vol_dp_enable;
//    D[0] vol_rvlc_enable;
    unsigned short      sprite_usage;                          //2

    unsigned int        width;                                 //4
    unsigned int        height;                                //8

    unsigned short      pts_incr;                              //12
    unsigned short      reserved0;                             //14

    unsigned char       aspect_ratio;                          //16
    unsigned char       progressive_sequence;                  //17
    unsigned char       mpeg1;                                 //18
    unsigned char       play_mode;                             //19

    unsigned char       mpeg_frc_mode;                         //20
    unsigned char       first_display;                         //21
    unsigned char       low_delay;                             //22
    unsigned char       video_range;                           //23

    unsigned int        bit_rate;                              //24

    unsigned short      vol_time_incr_res;                     //28
    unsigned short      fixed_vop_time_incr;                   //30    // 0: not fixed_vop_rate  others : vop_time_incr

    unsigned char       par_width;                             //32
    unsigned char       par_height;                            //33
    unsigned char       u8AFD;                                 //34
    unsigned char       original_progressive;                  //35

    unsigned int        vc1_frame_rate;                        //36
    unsigned int        frame_rate;                            //40

    unsigned char       key_gen[32];                           //44

    unsigned char       ds_enable;                             //76
    unsigned char       stereo_type;                           //77
    unsigned short      CropBottom;                            //78    // For HDMI 3D Output mode.

    unsigned int        DSbufsize;                             //80

    unsigned char       suspend_ds;                            //84
    unsigned char       reserved4[2];                          //85,86
    unsigned char       CMA_AllocDone;                         //87

    unsigned int        CMA_FB_Address;                        //88
    unsigned int        CMA_FB_Size;                           //92
    unsigned int        slq_end;                               //96
    unsigned int        slq_start;                             //100
    unsigned int        slq_end2;                             //104
    unsigned int        slq_start2;                           //108

    volatile unsigned char          u32VirtualCommandArg0;     //112
    volatile unsigned char          u32VirtualCommandArg1;     //113
    volatile unsigned char          u32VirtualCommandArg2;     //114
    volatile unsigned char          u32VirtualCommandArg3;     //115

    volatile unsigned char          u32VirtualCommandArg4;     //116
    volatile unsigned char          u32VCHandshake;            //117
    unsigned short                  u16Profile;                //118

    unsigned int        DS_Depth;                              //120

    unsigned int        u32RealFrameRate;                      //124

    unsigned int        u32DispWidth;                          //128
    unsigned int        u32DispHeight;                         //132

    unsigned short      u16Level;                              //136
    unsigned char       reserved[2];                           //138
}FW_VOL_INFO,*pFW_VOL_INFO;//16 byte

#define OFFSET_VOL_INFO                0
#define OFFSET_SPRITE_USAGE            2
#define OFFSET_WIDTH                   4
#define OFFSET_HEIGHT                  8
#define OFFSET_PTS_INCR                12
#define OFFSET_RESERVED0               14
#define OFFSET_ASPECT_RATIO            16
#define OFFSET_PROGRESSIVE_SEQUENCE    17
#define OFFSET_MPEG1                   18
#define OFFSET_PLAY_MODE               19
#define OFFSET_MPEG_FRC_MODE           20
#define OFFSET_FIRST_DISPLAY           21
#define OFFSET_LOW_DELAY               22
#define OFFSET_VIDEO_RANGE             23
#define OFFSET_BIT_RATE                24
#define OFFSET_VOL_TIME_INCR_RES       28
#define OFFSET_FIXED_VOP_TIME_INCR     30
#define OFFSET_PAR_WIDTH               32
#define OFFSET_PAR_HEIGHT              33
#define OFFSET_AFD                     34
#define OFFSET_ORIGINAL_PROGRESSIVE    35
#define OFFSET_VC1_FRAME_RATE          36
#define OFFSET_FRAME_RATE              40
#define OFFSET_KEY_GEN                 44
#define OFFSET_DS_ENABLE               76
#define OFFSET_STEREO_TYPE             77
#define OFFSET_CROPBOTTOM              78
#define OFFSET_DSBUFSIZE               80
#define OFFSET_SUSPEND_DS              84
#define OFFSET_CMA_ALLOCDONE           87
#define OFFSET_CMA_FB_ADDRESS          88
#define OFFSET_CMA_FB_SIZE             92
#define OFFSET_SLQ_END                 96
#define OFFSET_SLQ_START               100
#define OFFSET_SLQ_END1                104
#define OFFSET_SLQ_START1              108
#define OFFSET_VIRTUAL_COMMANDARG0     112
#define OFFSET_VIRTUAL_COMMANDARG1     113
#define OFFSET_VIRTUAL_COMMANDARG2     114
#define OFFSET_VIRTUAL_COMMANDARG3     115
#define OFFSET_VIRTUAL_COMMANDARG4     116
#define OFFSET_VCHANDSHAKE             117
#define OFFSET_PROFILE                 118
#define OFFSET_DS_DEPTH                120
#define OFFSET_REALFRAMERATE           124
#define OFFSET_DISPWIDTH               128
#define OFFSET_DISPHEIGHT              132
#define OFFSET_LEVEL                   136

typedef struct
{
    union
    {
        struct
        {
            unsigned int mvdcmd_handshake_pause : 1;        // 1 for handshake ready with CMD_PAUSE,
            unsigned int mvdcmd_handshake_slq_reset : 1;    // 1 for handshake ready with CMD_VC1_HW_SLQ_RESET
            unsigned int mvdcmd_handshake_stop : 1;         // 1 for handshake ready with CMD_STOP
            unsigned int mvdcmd_handshake_skip_data : 1;    // 1 for handshake ready with CMD_SKIP_DATA
            unsigned int mvdcmd_handshake_skip_to_pts : 1;  // 1 for handshake ready with CMD_SKIP_TO_PTS
            unsigned int mvdcmd_handshake_single_step : 1;  // 1 for handshake ready with CMD_SINGLE_STEP
            unsigned int mvdcmd_handshake_scaler_data_ready : 1;
            unsigned int mvdcmd_handshake_get_nextdispfrm_ready : 1; // for Mstreamer mode
            unsigned int mvdcmd_handshake_parser_rst : 1;  // 1 for handshake done with CMD_PTS_TBL_RESET
            unsigned int mvdcmd_handshake_cc608_rst : 1;    // "0" for handshake done with mstar cc608
            unsigned int mvdcmd_handshake_cc708_rst : 1;    // "0" for handshake done with mstar cc708
            unsigned int mvdcmd_handshake_fast_rst : 1;     // 1 for handshake done with CMD_FAST_RST, 1 for fast_rst_done...
            unsigned int mvdcmd_handshake_detatch : 1;      // 1 for handshake ready with exit main loop
            unsigned int mvdcmd_handshake_pvr_seamless_mode : 1;      // 1 for handshake ready
            unsigned int mvdcmd_handshake_virtualCommand : 1;      // 1 for handshake ready with virtual command get status done
            unsigned int mvdcmd_handshake_flush : 1;      // 1 for handshake ready with flush disp queue command
            unsigned int mvdcmd_handshake_vsync_control : 1;      // 1 for handshake ready with flush disp queue command
            unsigned int mvdcmd_handshake_reserved : 15;    // reserved for extend
        };
        unsigned int value;
    };
}MVD_CMD_HANDSHADE_INDEX;

typedef struct
{
    union
    {
        struct
        {
            unsigned int mvd_xc_disable_black_screen : 1;   // 1 for XC disable the black screen, defaule is "0"...
            unsigned int mvd_xc_release_force_rbank : 1;    // 1 for XC release force read bank, defaule is "0"...
            unsigned int mvd_xc_release_bob_mode : 1;       // 1 for XC release BOB mode, defaule is "0"...
            unsigned int mvd_xc_release_UCNR : 1;           // 1 for XC release UCNR, defaule is "0"...
            unsigned int mvd_xc_reserved : 28;              // reserved for extend
        };
        unsigned int value;
    };
}MVD_XC_LOW_DELAY_INT_STATE;

typedef struct _FW_FRAME_INFO
{
    unsigned int            frame_count;                 //0
    unsigned int            slq_tbl_rptr;                //4        // ==>ms
    unsigned int            vol_update;                  //8
    unsigned int            error_code;                  //12

    unsigned int            error_status;                //16
    unsigned int            skip_frame_count;            //20
    unsigned int            picture_type;                //24    // 0:I frame 1:P frame 2:B frame
    unsigned int            slq_sw_index;                //28

    unsigned char           fb_index;                    //32
    unsigned char           top_ff;                      //33
    unsigned char           repeat_ff;                   //34
    unsigned char           invalidstream;               //35
    unsigned int            vld_err_count;               //36
    unsigned short          tmp_ref;                     //40
    unsigned char           first_frame;                 //42
    unsigned char           first_I_found;               //43
    unsigned int            gop_i_fcnt;                  //44

    unsigned int            gop_p_fcnt;                  //48
    unsigned int            gop_b_fcnt;                  //52
    unsigned int            overflow_count;              //56
    unsigned int            time_incr;                   //60

    unsigned int            self_rst_count;              //64
    unsigned int            sw_vd_count;                 //68
    unsigned int            step_disp_done;              //72
    unsigned int            step_to_pts_done;            //76

    MVD_CMD_HANDSHADE_INDEX cmd_handshake_index;         //80
    unsigned int            last_frame_show_done;        //84
    unsigned int            meet_file_end_sc;            //88
    unsigned int            rcv_payload_lenth;           //92

    unsigned int            firmware_version;            //96
    unsigned int            ic_version;                  //100
    unsigned int            interface_version;           //104
    unsigned char           color_primaries;             //108
    unsigned char           transfer_char;               //109
    unsigned char           matrix_coef;                 //110
    unsigned char           video_format;                //111

    unsigned short          disp_h_size;                 //112
    unsigned short          disp_v_size;                 //114
    unsigned char           time_code_hours;             //116
    unsigned char           time_code_minutes;           //117
    unsigned char           time_code_seconds;           //118
    unsigned char           time_code_pictures;          //119
    unsigned char           drop_frame_flag;             //120
    unsigned char           time_code_hours_disp;        //121
    unsigned char           time_code_minutes_disp;      //122
    unsigned char           time_code_seconds_disp;      //123
    unsigned char           time_code_pictures_disp;     //124
    unsigned char           drop_frame_flag_disp;        //125
    unsigned char           PicStruct;                   //126
    unsigned char           chroma_format;               //127

    int                     pts_stc;                     //128
    unsigned int            displayed_cnt;               //132
    unsigned int            next_pts;                    //136
    unsigned short          centre_h_offset;             //140
    unsigned short          centre_v_offset;             //142

    unsigned int            int_cnt;                     //144
    unsigned int            disp_pts;                    //148    // pts of current displayed frame
    unsigned int            high32_pts;                  //152    // msb of 33-bit pts
    unsigned char           dispQnum;                    //156
    unsigned char           CurrentESBufferStatus;       //157    // init:0x00,underflow:0x01,overflow:0x02,normal:0x03
    unsigned char           framebufferresource;         //158    // default: 0, ok: 1, fail : 2
    unsigned char           framebuffer_status;          //159

    unsigned int            divx_ver_5x;                 //160    // report divx version...
    unsigned int            frame_buf_size;              //164    // report real frame buffer size(unit in bytes)...
    MVD_XC_LOW_DELAY_INT_STATE xc_low_delay_int_state;   //168    // for xc low delay interrupt status...
    unsigned int            xc_low_delay_cnt;            //172

    unsigned int            xc_diff_field_no;            //176    // for get XC diff field number...
    unsigned int            xc_low_delay_cnt_latched;    //180    // for dbg xc_low_delay timing only...
    unsigned int            drop_count;                  //184    // for counting that decoded frame who doesn't display

    unsigned int            rdptr_pts_low;               //188    // for TM14 pts flow control, pts based on pts table read pointer
    unsigned int            rdptr_pts_high;              //192    // for TM14 pts flow control, pts based on pts table read pointer
    unsigned int            wrptr_pts_low;               //196    // for TM14 pts flow control, pts based on pts table write pointer
    unsigned int            wrptr_pts_high;              //200    // for TM14 pts flow control, pts based on pts table write pointer

    unsigned int            wait_decode_done_cnt;       //204
    unsigned int            wait_seach_buffer_cnt;      //208
    unsigned int            wait_search_code_cnt;       //212
    unsigned int            wait_pre_buf_cnt;           //216
    unsigned int            wait_vfifo_buf_cnt;         //220
    unsigned int            wait_search_header_cnt;     //224
    unsigned int            wait_flash_pattern_cnt;     //228
    unsigned int            pb_chunk_count;             //232
    unsigned int            pb_buffer_count;            //236
    unsigned int            pb_chunk_flag;              //240
    unsigned int            divx311_flag;               //244
    unsigned int            pvr_seamless_status;        //248
    unsigned int            int_stat;                   //252
    unsigned int            drop_frame_count;           //256
    unsigned int            disp_stc;                   //260
    unsigned int            repeat_frame_count;         //264
    unsigned char           color_descript;             //268
    unsigned char           u8PVRSeamlessTargetPTSHigh; //269
    unsigned char           u8PVRSeamlessTargetFrameType; //270
    unsigned char           seq_found;                  //271
    unsigned int            idle_count;                 //272
    unsigned int            u32PVRSeamlessTargetPTS;     //276
    unsigned int            frame_wrtptr;                //280
    unsigned int            frame_readptr;               //284
    unsigned char           frame_type[FRAMEQ_SIZE];     //288
    unsigned int            frame_timestamp[FRAMEQ_SIZE];//304
    volatile unsigned int   first_pts_h;                 //368
    volatile unsigned int   first_pts_l;                 //372
    unsigned int            u32CurMinTspDataSize;        //376

}FW_FRAME_INFO, *pFW_FRAME_INFO;

#define OFFSET_FRAME_COUNT                 0
#define OFFSET_SLQ_TBL_RPTR                4
#define OFFSET_VOL_UPDATE                  8
#define OFFSET_ERROR_CODE                  12
#define OFFSET_ERROR_STATUS                16
#define OFFSET_SKIP_FRAME_COUNT            20
#define OFFSET_PICTURE_TYPE                24
#define OFFSET_SLQ_SW_INDEX                28
#define OFFSET_FB_INDEX                    32
#define OFFSET_TOP_FF                      33
#define OFFSET_REPEAT_FF                   34
#define OFFSET_INVALIDSTREAM               35
#define OFFSET_VLD_ERR_COUNT               36
#define OFFSET_TMP_REF                     40
#define OFFSET_FIRST_FRAME                 42
#define OFFSET_FIRST_I_FOUND               43
#define OFFSET_GOP_I_FCNT                  44
#define OFFSET_GOP_P_FCNT                  48
#define OFFSET_GOP_B_FCNT                  52
#define OFFSET_OVERFLOW_COUNT              56
#define OFFSET_TIME_INCR                   60
#define OFFSET_SELF_RST_COUNT              64
#define OFFSET_SW_VD_COUNT                 68
#define OFFSET_STEP_DISP_DONE              72
#define OFFSET_STEP_TO_PTS_DONE            76
#define OFFSET_CMD_HANDSHAKE_INDEX         80
#define OFFSET_CMD_LAST_FRAME_SHOW         84
#define OFFSET_MEET_FILE_END_SC            88
#define OFFSET_RCV_PAYLOAD_LENGTH          92
#define OFFSET_FIRMWARE_VERSION            96
#define OFFSET_IC_VERSION                  100
#define OFFSET_INTERFACE_VERSION           104
#define OFFSET_COLOR_PRIMARIES             108
#define OFFSET_TRANSFER_CHAR               109
#define OFFSET_MATRIX_COEF                 110
#define OFFSET_VIDEO_FORMAT                111
#define OFFSET_DISP_H_SIZE                 112
#define OFFSET_DISP_V_SIZE                 114
#define OFFSET_TIME_CODE_HOURS             116 // for decoding frame
#define OFFSET_TIME_CODE_MINUTES           117 // for decoding frame
#define OFFSET_TIME_CODE_SECONDS           118 // for decoding frame
#define OFFSET_TIME_CODE_PICTURES          119 // for decoding frame
#define OFFSET_DROP_FRAME_FLAG             120 // for decoding frame
#define OFFSET_TIME_CODE_HOURS_DISP        121 // for displaying frame
#define OFFSET_TIME_CODE_MINUTES_DISP      122 // for displaying frame
#define OFFSET_TIME_CODE_SECONDS_DISP      123 // for displaying frame
#define OFFSET_TIME_CODE_PICTURES_DISP     124 // for displaying frame
#define OFFSET_DROP_FRAME_FLAG_DISP        125 // for displaying frame
#define OFFSET_PICTURE_STRUCTURE           126
#define OFFSET_CHROMA_FORMAT               127
#define OFFSET_PTS_STC                     128 // integer, pts_stc(n)=pts(n)-stc(n)
#define OFFSET_DISPLAYED_CNT               132
#define OFFSET_NEXT_PTS                    136
#define OFFSET_CENTRE_H_OFFSET             140
#define OFFSET_CENTRE_V_OFFSET             142
#define OFFSET_INT_CNT                     144
#define OFFSET_DISP_PTS                    148
#define OFFSET_DISP_PTS_MSB                152
#define OFFSET_DISPQ_NUM                   156
#define OFFSET_CURRENT_ES_BUFFER_STATUS    157
#define OFFSET_FRAME_BUFFER_RESOURCE       158
#define OFFSET_FRAME_BUFFER_STATUS         159
#define OFFSET_DIVX_VER_5X                 160
#define OFFSET_FRAME_BUF_SIZE              164 // report real frame buffer size(unit in bytes)...
#define OFFSET_XC_LOW_DELAY_INT_STATE      168 // for xc low delay interrupt status...
#define OFFSET_XC_LOW_DELAY_CNT            172 // for xc low delay interrupt status...
#define OFFSET_XC_DIFF_FIELD_NO            176 // for get XC diff field number...
#define OFFSET_XC_LOW_DELAY_CNT_LATCH      180 // for dbg xc_low_delay timing only...
#define OFFSET_DROP_COUNT                  184 // for counting that decoded frame who doesn't display
#define OFFSET_RDPTR_PTS_LOW               188
#define OFFSET_RDPTR_PTS_HIGH              192
#define OFFSET_WRPTR_PTS_LOW               196
#define OFFSET_WRPTR_PTS_HIGH              200
#define OFFSET_DECODEDONE_COUNT            204
#define OFFSET_SEARCHBUF_COUNT             208
#define OFFSET_SEARCHCODE_COUNT            212
#define OFFSET_PREBUF_COUNT                216
#define OFFSET_VFIFOBUF_COUNT              220
#define OFFSET_SEARCHHEADER_COUNT          224
#define OFFSET_FLASHPATTERN_COUNT          228
#define OFFSET_PB_CHUNK_COUNT              232
#define OFFSET_PB_BUFFER_COUNT             236
#define OFFSET_PB_CHUNK_FLAG               240
#define OFFSET_DIVX311_FLAG                244
#define OFFSET_PVR_SEAMLESS_STATUS         248
#define OFFSET_INT_STAT                    252
#define OFFSET_DROP_FRAME_COUNT            256
#define OFFSET_DISP_STC                    260
#define OFFSET_REPEAT_FRAME_COUNT          264
#define OFFSET_COLOR_DESCRIPT              268
#define OFFSET_PVRSEAMLESSTARGETPTSHIGH    269
#define OFFSET_PVRSEAMLESSTARGETFRAMETYPE  270
#define OFFSET_SEQ_FOUND                   271
#define OFFSET_IDLE_COUNT                  272
#define OFFSET_PVRSEAMLESSTARGETPTS        276
#define OFFSET_FRAME_WRTPTR                280
#define OFFSET_FRAME_READPTR               284
#define OFFSET_FRAME_TYPE                  288
#define OFFSET_FRAME_TIMESTAMP             304
#define OFFSET_FIRST_PTS_H                 368
#define OFFSET_FIRST_PTS_L                 372
#define OFFSET_CUR_MIN_TSP_DATA_SIZE       376

typedef struct _FW_DIVX_INFO
{
    unsigned int        vol_handle_done;            //0

    unsigned int        width;                        //4
    unsigned int        height;                        //8
    unsigned int        frame_count;                //12
    unsigned int        frame_time;                    //16

    unsigned short      pts_incr;                    //20
    unsigned short      reserve0;

    unsigned char       aspect_ratio;                //24
    unsigned char       progressive_sequence;        //25
    unsigned char       mpeg1;                        //26
    unsigned char       play_mode;                    //27

    unsigned char       mpeg_frc_mode;                //28
    unsigned char       invalidstream;                //29
    unsigned char       reserve[2];                    //30
    unsigned int        frame_rate;                    //32
}FW_DIVX_INFO, *pFW_DIVX_INFO;

#define OFFSET_DIVX_VOL_HANDLE_DONE      0
#define OFFSET_DIVX_WIDTH                4
#define OFFSET_DIVX_HEIGHT               8
#define OFFSET_DIVX_FRAME_COUNT          12
#define OFFSET_DIVX_FRAME_TIME           16
#define OFFSET_DIVX_PTS_INCR             20
#define OFFSET_DIVX_RESERVE0             22
#define OFFSET_DIVX_ASPECT_RATIO         24
#define OFFSET_DIVX_PROGRESSIVE_SEQUENCE 25
#define OFFSET_DIVX_MPEG1                26
#define OFFSET_DIVX_PLAY_MODE            27
#define OFFSET_DIVX_MPEG_FRC_MODE        28
#define OFFSET_DIVX_INVALIDSTREAM        29
#define OFFSET_DIVX_RESERVED             30
#define OFFSET_DIVX_FRAME_RATE           32

#define STATUS_VIDEO_SYNC    (1<<0)
#define STATUS_VIDEO_FREERUN (1<<1)
#define STATUS_VIDEO_SKIP    (1<<2)
#define STATUS_VIDEO_REPEAT  (1<<3)

typedef struct _FW_USER_DATA_BUF
{
    unsigned char picType;                            /* picture type: 1->I picture, 2->P,3->B */
    unsigned char top_ff;                            /* Top field first: 1 if top field first*/
    unsigned char rpt_ff;                            /* Repeat first field: 1 if repeat field first*/
    unsigned char userdatabytecnt;

    unsigned short     tmpRef;                        /* Temporal reference of the picture*/

    unsigned char userdata[250];
}FW_USER_DATA_BUF,*pFW_USER_DATA_BUF;

#define FW_USER_DATA_BUF_EXT_PACK_LEN  240
typedef struct _FW_USER_DATA_BUF_EXT
{
    unsigned char            picType;                /* picture type: 1->I picture, 2->P,3->B */
    unsigned char            top_ff;                    /* Top field first: 1 if top field first*/
    unsigned char            rpt_ff;                    /* Repeat first field: 1 if repeat field first*/
    unsigned char            userdatabytecnt;

    unsigned short           tmpRef;                    /* Temporal reference of the picture*/
    unsigned char            PicStruct;                /* picture struct with this cc pack*/
    unsigned char            reserved;

    unsigned int             pts;                    /* pts with this cc pack*/
    unsigned int             reserved2;

    unsigned char userdata[FW_USER_DATA_BUF_EXT_PACK_LEN];
}FW_USER_DATA_BUF_EXT,*pFW_USER_DATA_BUF_EXT;

typedef struct _DecFrameInfo
{
    unsigned int            u32DecLumaAddr;                //0
    unsigned int            u32DecChromaAddr;            //4
    unsigned int            u32DecTimeStamp;            //8
    unsigned int            u32DecID_L;                    //12
    unsigned int            u32DecID_H;                    //16
    unsigned short          u16DecPitch;                //20
    unsigned short          u16DecWidth;                //22
    unsigned short          u16DecHeight;                //24
    unsigned short          u16DeceFrameType;            //26
    unsigned int            u32DispLumaAddr;            //28
    unsigned int            u32DispChromaAddr;            //32
    unsigned int            u32DispTimeStamp;            //36
    unsigned int            u32DispID_L;                //40
    unsigned int            u32DispID_H;                //44
    unsigned short          u16DispPitch;                //48
    unsigned short          u16DispWidth;                //50
    unsigned short          u16DispHeight;                //52
    unsigned short          u16DispeFrameType;            //54
    // for Mstreamer mode
    unsigned int            u32NextDispLumaAddr;        //56
    unsigned int            u32NextDispChromaAddr;        //60
    unsigned int            u32NextDispTimeStamp;        //64
    unsigned int            u32NextDispID_L;            //68
    unsigned int            u32NextDispID_H;            //72
    unsigned short          u16NextDispPitch;            //76
    unsigned short          u16NextDispWidth;            //78
    unsigned short          u16NextDispHeight;            //80
    unsigned short          u16NextDispeFrameType;        //82
    unsigned short          u16NextDispFrameIdx;        //84
    // for vc1/rcv range reduction
    unsigned char           u8NextDispRangeRed_Y;      //86 //[7]: on/off [6:0]: scale
    unsigned char           u8NextDispRangeRed_UV;     //87 //[7]: on/off [6:0]: scale
    // for MCU mode, which support interlace
    unsigned short          u16ExtData;                     //88
    unsigned char           u8Progressive;                  //90
    unsigned char           u8TileMode;                     //91
}DecFrameInfo, *pDecFrameInfo;

#define OFFSET_DECFRAMEINFO_DEC_LUMAADDR           0
#define OFFSET_DECFRAMEINFO_DEC_CHROMAADDR         4
#define OFFSET_DECFRAMEINFO_DEC_TIMESTAMP          8
#define OFFSET_DECFRAMEINFO_DEC_ID_L              12
#define OFFSET_DECFRAMEINFO_DEC_ID_H              16
#define OFFSET_DECFRAMEINFO_DEC_PITCH             20
#define OFFSET_DECFRAMEINFO_DEC_WIDTH             22
#define OFFSET_DECFRAMEINFO_DEC_HEIGHT            24
#define OFFSET_DECFRAMEINFO_DEC_FRAMETYPE         26
#define OFFSET_DECFRAMEINFO_DISP_LUMAADDR         28
#define OFFSET_DECFRAMEINFO_DISP_CHROMAADDR       32
#define OFFSET_DECFRAMEINFO_DISP_TIMESTAMP        36
#define OFFSET_DECFRAMEINFO_DISP_ID_L             40
#define OFFSET_DECFRAMEINFO_DISP_ID_H             44
#define OFFSET_DECFRAMEINFO_DISP_PITCH            48
#define OFFSET_DECFRAMEINFO_DISP_WIDTH            50
#define OFFSET_DECFRAMEINFO_DISP_HEIGHT           52
#define OFFSET_DECFRAMEINFO_DISP_FRAMETYPE        54
#define OFFSET_DECFRAMEINFO_NEXTDISP_LUMAADDR     56 // for Mstreamer mode
#define OFFSET_DECFRAMEINFO_NEXTDISP_CHROMAADDR   60
#define OFFSET_DECFRAMEINFO_NEXTDISP_TIMESTAMP    64
#define OFFSET_DECFRAMEINFO_NEXTDISP_ID_L         68
#define OFFSET_DECFRAMEINFO_NEXTDISP_ID_H         72
#define OFFSET_DECFRAMEINFO_NEXTDISP_PITCH        76
#define OFFSET_DECFRAMEINFO_NEXTDISP_WIDTH        78
#define OFFSET_DECFRAMEINFO_NEXTDISP_HEIGHT       80
#define OFFSET_DECFRAMEINFO_NEXTDISP_FRAMETYPE    82
#define OFFSET_DECFRAMEINFO_NEXTDISP_FRAMEIDX     84 // for Mstreamer mode
#define OFFSET_DECFRAMEINFO_NEXTDISP_RANGERED_Y   86 // for vc1/rcv
#define OFFSET_DECFRAMEINFO_NEXTDISP_RANGERED_UV  87 // for vc1/rcv
#define OFFSET_DECFRAMEINFO_NEXTDISP_EXT_DATA     88 // for MCU mode, which support interlace
#define OFFSET_DECFRAMEINFO_NEXTDISP_PROGRESSIVE  90 // is progressive or not


typedef struct __DISPQ_IN_DRAM
{
    volatile unsigned int        dispQ_rd;
    volatile unsigned int        dispQ_wr;
    volatile DecFrameInfo        disp_info[DISPQ_SIZE];
    volatile int        dispQ_len;
    volatile unsigned int        bUsedByOutside[DISPQ_SIZE];
}DISPQ_IN_DRAM;



typedef struct _DEBUG_INFO
{
    //
    volatile unsigned short max_coded_width;
    volatile unsigned short max_coded_height;
    volatile unsigned short sync_status;              // defined by AV_SYNC

    volatile unsigned short REG67;
    volatile unsigned short REG68;
    volatile unsigned short REG69;

    volatile unsigned short REG6a;
    volatile unsigned short REG6b;
    volatile unsigned short REG6c;

    volatile unsigned short REG6d;
    volatile unsigned short REG6e;
    volatile unsigned short REG6f;

    //
    volatile unsigned short overflow_count;
    volatile unsigned short underflow_count;
    volatile unsigned short vlderr_count;
    volatile unsigned short frame_conut;//0

    volatile unsigned int   y_start_addr; //in byte unit
    volatile unsigned int   uv_start_addr;//in byte unit

    volatile unsigned int   width;
    volatile unsigned int   height;

    // where
    volatile unsigned short mb_x;
    volatile unsigned short mb_y;

    volatile unsigned short file_end;
    //16-byte aligned
    volatile unsigned char reserved[24];

}DebugInfo;

typedef struct VC1_SEQ_INFO
{
    volatile    unsigned int        PROFILE;
    volatile    unsigned int        FRMRTQ_POSTPROC;
    volatile    unsigned int        BITRTQ_POSTPROC;
    volatile    unsigned int        LOOPFILTER;
    volatile    unsigned int        MULTIRES;
    volatile    unsigned int        FASTUVMC;
    volatile    unsigned int        EXTENDED_MV;
    volatile    unsigned int        DQUANT;
    volatile    unsigned int        VSTRANSFORM;
    volatile    unsigned int        OVERLAP;
    volatile    unsigned int        SYNCMARKER;
    volatile    unsigned int        RANGERED;
    volatile    unsigned int        MAXBFRAMES;
    volatile    unsigned int        QUANTIZER;
    volatile    unsigned int        FINTERPFLAG;
    volatile    unsigned int        LEVEL;
    volatile    unsigned int        CBR;
    volatile    unsigned int        FRAMERATE;
    volatile    unsigned int        VERT_SIZE;
    volatile    unsigned int        HORIZ_SIZE;
}VC1_SEQUENCE_INFO, *pVC1_SEQUENCE_INFO;

#define OFFSET_RCV_PROFILE                        0
#define OFFSET_RCV_FRMRTQ_POSTPROC                4
#define OFFSET_RCV_BITRTQ_POSTPROC                8
#define OFFSET_RCV_LOOPFILTER                    12
#define OFFSET_RCV_MULTIRES                      16
#define OFFSET_RCV_FASTUVMC                      20
#define OFFSET_RCV_EXTENDED_MV                   24
#define OFFSET_RCV_DQUANT                        28
#define OFFSET_RCV_VSTRANSFORM                   32
#define OFFSET_RCV_OVERLAP                       36
#define OFFSET_RCV_SYNCMARKER                    40
#define OFFSET_RCV_RANGERED                      44
#define OFFSET_RCV_MAXBFRAMES                    48
#define OFFSET_RCV_QUANTIZER                     52
#define OFFSET_RCV_FINTERPFLAG                   56
#define OFFSET_RCV_LEVEL                         60
#define OFFSET_RCV_CBR                           64
#define OFFSET_RCV_FRAMERATE                     68
#define OFFSET_RCV_VERT_SIZE                     72
#define OFFSET_RCV_HORIZ_SIZE                    76

typedef struct _FW_AVSYNC_TABLE
{
    unsigned int            byte_cnt;            //0        //23 valid bits
    unsigned int            dummy_cnt;            //4        //dummy packet counter
    unsigned int            id_low;                //8        //ID specified by player
    unsigned int            id_high;            //12

    unsigned int            time_stamp;            //16    //pts or dts
    unsigned int            reserved_int0;        //20
    unsigned int            reserved_int1;        //24
    unsigned int            reserved_int2;        //28
}FW_AVSYNC_TABLE, *pFW_AVSYNC_TABLE;

#define OFFSET_BYTE_CNT                         0
#define OFFSET_DUMMY_CNT                        4
#define OFFSET_ID_LOW                           8
#define OFFSET_ID_HIGH                         12
#define OFFSET_TIME_STAMP                      16

typedef struct _fw_VBBU
{
    unsigned int u32WrPtr;
    unsigned int u32RdPtr;
    unsigned char u8Reserved[8];
    VDEC_VBBU_Entry stEntry[MAX_VDEC_VBBU_ENTRY_COUNT];
} FW_VBBU,*pFW_VBBU;

#ifdef M4VDPLAYER
extern    pFW_VOL_INFO        gp_vol_info;
extern    pFW_DIVX_INFO       gp_divx_info;
#endif

//interupt flag
#define INT_CC_NEW          (1<<0)
#define INT_USER_DATA       (1<<0)
#define INT_VBUF_OVF        (1<<1)
#define INT_VBUF_UNF        (1<<2)
#define INT_VES_VALID       (1<<3)
#define INT_VES_INVALID     (1<<4)
#define INT_SEQ_FOUND       (1<<5)
#define INT_PIC_FOUND       (1<<6)
#define INT_DEC_ERR         (1<<7)
#define INT_FIRST_FRAME     (1<<8)
#define INT_DISP_RDY        (1<<9)
#define INT_SYN_SKIP        (1<<10)
#define INT_SYN_REP         (1<<11)
#define INT_DISP_VSYNC      (1<<12)
#define INT_USER_DATA_DISP  (1<<13) //user data in display order
#define INT_PTS_DISCONTINUE (1<<14) //detection pts discontinue for t3-gp2, 20101214
#define INT_DEC_DONE        (1<<15) //finishing decoding one frame.
#define INT_DEC_I           (1<<16) //finishing decoding one frame.
#define INT_XC_LOW_DELAY    (1<<17) //trigger this interrupt for XC speed up to show image on channel change...
#define INT_DISP_FINISH    (1<<18)// enable last frame show one

#define INT_SYN_SKIP_P       10
#define INT_SYN_REP_P        11


//MVD TLB
#define MVD_TLB_BSR1          (1<<0)
#define MVD_TLB_BSR2          (1<<1)
#define MVD_TLB_PAS1          (1<<2)
#define MVD_TLB_PAS2          (1<<3)
#define MVD_TLB_PESFI1        (1<<4)
#define MVD_TLB_PESFI2        (1<<5)
#define MVD_TLB_SMDB          (1<<6)
#define MVD_TLB_VLD           (1<<7)
#define MVD_TLB_REF           (1<<8)
#define MVD_TLB_NM            (1<<9)
#define MVD_TLB_IAP           (1<<10)
#define MVD_TLB_JSC           (1<<11)
#define MVD_TLB_MTO           (1<<12)
#define MVD_TLB_BSR3          (1<<13)


// decoding state definition
#define DEC_STAT_IDLE                0x00
#define DEC_STAT_FIND_SC             0x01
#define DEC_STAT_FIND_SPE_SC         0x11
#define DEC_STAT_FIND_FRAMEBUFFER    0x02
#define DEC_STAT_WAIT_DECODE_DONE    0x03
#define DEC_STAT_DECODE_DONE         0x04
#define DEC_STAT_WAIT_VDFIFO         0x05
#define DEC_STAT_INIT_SUCCESS        0x06
#define DEC_STAT_NO_FRAME_BUFFER     0x07
#define DEC_STAT_WAIT_PLAY_CMD       0x08

//error_code
#define VOL_SHAPE                       1  //error_status 0:rectanglular    1:binary 2: binary only 3: grayscale
#define VOL_USED_SPRITE                 2  //error_status 0:sprite not used 1:static 2: GMC 3: reserved
#define VOL_NOT_8_BIT                   3  //error_status : bits per pixel
#define VOL_NERPRED_ENABLE              4
#define VOL_REDUCED_RES_ENABLE          5
#define VOL_SCALABILITY                 6
#define VOL_OTHER                       7
#define VOL_H263_ERROR                  8
#define VOL_RES_NOT_SUPPORT             9 //error_status : none
#define VOL_MPEG4_NOT_SUPPORT          10 //error_status : none
#define VOL_PROFILE_NOT_SUPPORT        11
#define VOL_RCV_ERROR_OCCUR            12
#define VOL_VC1_NOT_SUPPORT            13
#define VOL_UNKNOW_CODEC_NOT_SUPPORT   14
#define VOL_SLQ_TBL_NOT_SUPPORT        15
#define VOL_FRAME_BUF_NOT_ENOUGH       16 //error_status : none
#define CODEC_MPEG4                    0x00 //arg0: 0: mpeg4, 1: mpeg4 with short_video_header, 2: DivX311
#define CODEC_MPEG4_SHORT_VIDEO_HEADER 0x01
#define CODEC_DIVX311                  0x02
#define CODEC_MPEG2                    0x10
typedef enum //arg1: 0: file mode 1:slq  2:live stream mode 3:slqtbl 4: Ts file mode
{
    FILE_MODE = 0,
    SLQ_MODE,
    STREAM_MODE,
    SLQ_TBL_MODE,
    TS_FILE_MODE,
    OTHER
}stream_type;

typedef enum
{
    E_MVD_CHIP_U01 = 0,
    E_MVD_CHIP_U02,
}chip_eco_rev;

#define ENABLE_PARSER                  0x00 //arg2: 0/1 enable/disable parser;
#define DISABLE_PARSER                 0x01
#define ENABLE_PKT_LEN                 0x02
#define PARSER_MPEG2                   0x00 //arg3: 0 13818-1 pes header;
#define PARSER_MPEG1                   0x01 //      1 11172-1 pes header;

#define FrcNormal                         0
#define FrcDisplayTwice                   1 //output rate is twice of input rate (ex. 30p a 60p)
#define Frc32Pulldown                     2 //3:2 pulldown mode (ex. 24p a 60i or 60p)
#define FrcPALtoNTSC                      3 //PALaNTSC conversion (50i a 60i)
#define FrcNTSCtoPAL                      4 //NTSCaPAL conversion (60i a 50i)
#define FrcShowOneFiled                   5
#define FrcDisplayDropHalf                6
#define FrcDisplay120To50                 7
#define FrcDisplay100To60                 8
#define FrcDisplay30To50                  9
#define FrcDisplayRepeat51                10
#define FrcDisplayDrop51                  11
#define FrcDisplayDrop52                  12
#define FrcDisplayDrop53                  13
#define FrcDisplay50To30                  14
#define FrcDisplay60To24                  15
#define FrcDisplay60To25                  16
#define FrcDisplay30To24                  17

#define FrcDisplayMultipleRepeat          20 //output_rate/input_rate=integer
#define FrcDisplayGeneralRepeat           21 //output_rate > input_rate, ex. 15->50...
#define FrcDisplayGeneralSkip             22 //output_rate < input_rate,
#define FrcDisplayThreeTimes              23
#define FrcDisplayFourTimes               24

#define MVD3_FILE_SD_MODE              0x02 //960*544
#define MVD3_HD_MODE                   0x10 //1920*1088
#define MVD3_SD_MODE                   0x00 //720*576
#define MVD3_DHD_MODE                  0x20  // dual HD
#define MVD3_DHD_MODE_MIN              0x40  //ECO ISSUE : THE WIDTH OVER 2560 IN VC1 WILL HIT THE HW_ISSUE

#define MVD_CMA_MODE                   0x1
// File mode avsync related
#define NONE_FILE_MODE                    0
#define FILE_PTS_MODE                     1
#define FILE_DTS_MODE                     2
#define FILE_STS_MODE                     3


// argument for "CMD_DISPLAY_PAUSE"
#define DISPLAY_PAUSE_OFF              0x00
#define DISPLAY_PAUSE_ON               0x01

// argument for "CMD_FRC_DROP_BEHAVIOR"
#define FRC_DROP_FRAME                 0x00 // for default frc drop behavior, drop per frame
#define FRC_DROP_FIELD                 0x01 // for frc drop behavior, drop per field to improve more smoothly in field mode

// ARG0 for "CMD_DRAM_OBF"
#define OBF_PAS1_WR                    0x01 // for Dram obf write index (Parser1 write)
#define OBF_VBUF1_RD                   0x02 // for Dram obf read index (VBUF1 read)
#define OBF_PES_FILE_IN1_WR            0x03 // for Dram obf read index for PESFI1
#define OBF_PAS2_WR                    0x04 // for Dram obf write index (Parser2 write)
#define OBF_VBUF2_RD                   0x05 // for Dram obf read index (VBUF2 read)
#define OBF_PES_FILE_IN2_WR            0x06 // for Dram obf read index for PESFI2

//command interface
#define CMD_PLAY                       0x01
#define CMD_PAUSE                      0x02
#define CMD_STOP                       0x03
#define CMD_FIND_SEQ                   0x04 //find seq header and set command = pause at picture header start code found
#define CMD_SINGLE_STEP                0x05
#define CMD_PLAY_NO_SQE                0x06
#define CMD_FAST_SLOW                  0x07 //arg0: 0: nomarl mode, 1: decode I only,  2: deocde I/P only,  3: slow motion
#define CMD_CODEC_INFO                 0x08 //arg0: 0: mpeg4, 1: mpeg4 with short_video_header, 2: DivX311
#define CMD_SYN_THRESHOLD              0x09
#define CMD_SYNC_ON                    0x0a
#define CMD_SYNC_OFFSET                0x0b
#define CMD_DISPLAY_CTL                0x0c
//arg0: 0/1-display by display/decode order
//arg1:   1-drop display decoding error frame
//arg2:   1-drop display when decode fast than display
//arg3:set frame rate conversion mode
#define CMD_GET_SYNC_STAT               0x0d //return arg0: 0/1 sync off/on ; arg1: 3 sync init done
#define CMD_GET_AFD                     0x0e
#define CMD_SKIP_DATA                   0x0f //set to skip all data till find FW_SPE_SCODE to resume normal play

#define CMD_STREAM_BUF_START           0x10
#define CMD_STREAM_BUF_END             0x11
#define CMD_FB_BASE                    0x12 //Frame buffer base address, from LSB to MSB are arg0, arg1, arg2, arg3
#define CMD_IAP_BUF_START              0x13
#define CMD_DP_BUF_START               0x14
#define CMD_MV_BUF_START               0x15
#define CMD_DMA_OVFTH                  0x16
#define CMD_DMA_UNFTH                  0x17
#define CMD_VC1_MIU_PROTECT_START      0x18
#define CMD_VC1_MIU_PROTECT_END        0x19
#define CMD_DISP_SPEED_CTRL            0x1a
#define CMD_STEP_DISP_DECODE_ONE       0x1b
#define CMD_STEP_DISP_ING              0x1c // repeat disp this frame
#define CMD_STEP_TO_PTS                0x1d
#define CMD_HANDSHAKE_STATUS           0x1e //report handshake status
#define CMD_DISPLAY_PAUSE              0x1f // display pause

#define CMD_USER_BUF_START             0x20
#define CMD_USER_BUF_SIZE              0x21
#define CMD_RD_USER_WP                 0x22
#define CMD_WD_USER_RP                 0x23
#define CMD_RD_CC_PKTCNT               0x24
#define CMD_RD_CC_OV                   0x25
#define CMD_CLOSE_CC                   0x26
#define CMD_EN_CC_INFO_ENHANCE         0X27 // arg0=1, for enhance to dump the pts/tmp_ref info with each cc-608 packet for mstar cc-lib, 20120406
#define CMD_BUF_OFFSET                 0x2d // stream/frame buf offset, programable high adderss [bit-25] that allocate to low/high 256MB MIU:(only for K2)
#define CMD_ENABLE_VLD_TIMEOUT         0x2e // enable mvd vld timeout and threshold
#define CMD_ENABLE_INT_STAT            0x2f // set which int be enabled

#define CMD_GET_INT_STAT               0x30
#define CMD_PARSE_M4V_PACKMD           0x31
#define CMD_RD_PTS                     0x32
#define CMD_FLUSH_LAST_IPFRAME         0x33
#define CMD_DECODE_STATUS              0x34 // arg0 = lastcommand ; arg1 = decode_status
#define CMD_VBUFFER_COUNT              0x35
#define CMD_START_DEC_STRICT           0x36 // start decoding in First I and skip non reference frame B decoding
#define CMD_SW_RESET                   0x37
#define CMD_MVD_FAST_INT               0x38
#define CMD_DIU_WIDTH_ALIGN            0x39
#define CMD_SW_IDX_ADJ                 0x3a // arg0=1 set sw_index as previous queue index infomation
#define CMD_PARSER_READ_POSITION       0x3b
#define CMD_REPEAT_MODE                0x3c // arg0=1 when frame display repeat only show one field
#define CMD_PTS_BASE                   0x3d
#define CMD_SKIP_TO_PTS                0x3e
#define CMD_AVSYNC_FREERUN_THRESHOLD   0x3f

#define CMD_DEBUG_BUF_START            0x40
#define CMD_DEBUG_CTL                  0x42
#define CMD_RD_IO                      0x43
#define CMD_WR_IO                      0x44
#define CMD_FB_RED_SET                 0x45
#define CMD_FB_NUM                     0x46
#define CMD_PTS_DETECTOR_EN            0x47 // enable filter for stream discontinue // to force the pts follow stc when pts=-1
#define CMD_PTS_TBL_RESET              0x48 // to reset pas/vld and pts_tbl // add new arg: 2 for only use mvd parser
#define CMD_DRAM_OBF                   0x49 // for Dram OBF key setting
#define CMD_FP_FILTER                  0x4A // 0:1 for disable/enable field polarity tuning filter, 0 for default...
#define CMD_PUSH_FIRST_FRAME_DISP      0x4B // 0:1 for disable/enable to push the first I-frame to dispQ when decoded done on ts and ts-file mode, 0 for default(disable)...
#define CMD_FAST_RST                   0x4C // 1 for enable mvd self reset...
#define CMD_RVU_EN                     0x4D //open RVU feature

#define CMD_SLQ_START                  0x50 //SLQ start address, from LSB to MSB are arg0, arg1, arg2, arg3
#define CMD_SLQ_END                    0x51 //SLQ end address, from LSB to MSB are arg0, arg1, arg2, arg3
#define CMD_SLQ_AVAIL_LEVEL            0x52 //arg0: 4-0
#define CMD_FPGA_COMP                  0x53 //arg0: 1/0:enable/disable FPGA comp
#define CMD_DIVX_PATCH                 0x54 //arg0: D[0] divx mv p interlace chroma adjust
#define CMD_HEADER_INFO_BUF            0x55 //header info buffer base address, from LSB to MSB are arg0, arg1, arg2, arg3
#define CMD_IDCT_SEL                   0x56 // arg0 D[0]:0/1 llm/divx6   D[1]:0/1 unbias/bias rounding mode
#define CMD_VOL_INFO_BUF               0x57
#define CMD_FRAME_INFO_BUF             0x58
#define CMD_CODE_OFFSET                0x59
#define CMD_RESET_FRAMECOUNT           0x5a
#define CMD_CHIPID                     0x5b
#define CMD_DEC_FRAME_INFO_BUF         0x5c
#define CMD_GET_FW_VERSION             0x5E
#define CMD_GET_EN_CATCH_DATA          0x5F

#define CMD_SLQ_TBL_BUF_START          0x60
#define CMD_SLQ_TBL_BUF_END            0x61
#define CMD_SLQ_UPDATE_TBL_WPTR        0x62
#define CMD_SLQ_GET_TBL_RPTR           0x63

// FW stop updating frames when vsync, but decoding process is still going.
#define CMD_FREEZE_DISP                0x64
#define CMD_DS_VIRTUAL_BOX             0x65
#define CMD_SHOW_ONE_FIELD             0x66
#define CMD_FD_MASK_DELAY_CNT          0x67 // delay n's vsync then active the fd_mask

#define CMD_UPDATE_FRAME               0x68 // updating next frame in slow motion mode
#define CMD_FRC_OUPUT                  0x69
#define CMD_FRC_DROP_BEHAVIOR          0x6A // arg0: FRC_DROP_FRAME/FRC_DROP_FIELD, default is FRC_DROP_FRAME

#define CMD_GET_NEXTDISPFRM            0x6B // for Mstreamer mode and mcu mode
#define CMD_FLIP_RELEASE_FRAME         0x6C // for Mstreamer mode and mcu mode
#define CMD_SEND_UNI_PTS               0x6D // for Mstreamer mode and mcu mode
#define CMD_SET_MST_MODE               0x6E // for Mstreamer mode and mcu mode
#define CMD_SET_MCU_MODE               0x6F // for mcu mode

#define CMD_DUMP_BITSTREAM_BASE        0x70
#define CMD_DUMP_BITSTREAM_LENGTH      0x71

#define CMD_XC_LOW_DELAY_PARA          0x72 // set the parameter for XC_low_delay mechanism

#define CMD_MVD_IDLE                   0x73
#define CMD_INTERFACE_VERSION          0x74
#define CMD_VC1_STREAM_TYPE_JPEG       0x75
#define CMD_VC1_STREAM_TYPE_MJPEG      0x76
#define CMD_VC1_BYPASS_MODE            0x77
#define CMD_VC1_UPDATE_SLQ             0x78
#define CMD_VC1_HW_SLQ_RESET           0x79
#define CMD_FLUSH_DISP_QUEUE           0x7A
#define CMD_VC1_FORCE_INTLACE_DISP     0x7B
#define CMD_VC1_IP_SCALE_THRESHOLD     0x7C
#define CMD_MOTION_COM_REDUCE          0x7D
#define CMD_CLOSE_DEBLOCK              0x7E
#define CMD_FIXED_FRAME_BUFFER         0x7F
#define CMD_ENABLE_AUTO_MUTE           0x80
#define CMD_FORCE_ALIGN_VSIZE          0x81
#define CMD_PROG_SEQ_STREAM            0x82

// File mode avsync related
#define CMD_ENABLE_AVSYNC_QUALIFIER     0x83// arg0=1:for enhance to do avsync when "enable_avsync=1" && "(lastcommand != CMD_PLAY)" for patch avsync on particular clip, 20120314
#define CMD_ENABLE_LAST_FRAME_QUALIFIER 0x84// arg0=1:for strict qualify the last_frame_show_done after the last_frame been displayed by mvop, 20120309
#define CMD_ENABLE_FILE_SYNC            0x85
#define CMD_PTS_TBL_START               0x86
#define CMD_FORCE_BLUE_SCREEN           0x87
#define CMD_ENABLE_LAST_FRAME_SHOW      0x88
#define CMD_DYNAMIC_SCALE_BASE          0x89
#define CMD_ENABLE_DYNAMIC_SCALE        0x8A
#define CMD_SCALER_INFO_BASE            0x8B
#define CMD_SW_BITPLANE_BASE            0x8C
#define CMD_FRONTEND_SEL                0x8D      // front end input selection
#define CMD_ENABLE_FREEZE_PIC           0x8E
#define CMD_FORBID_RESOLUTION_CHANGE    0x8F

// JPEG command
#define CMD_JPEG_CONSTRAIN_SIZE        0x91
#define CMD_JPEG_STATUS                0x92
#define CMD_JPEG_SCALEFACTOR           0x93
#define CMD_JPEG_ROI                   0x94
#define CMD_JPEG_ROI_DIM               0x95
#define CMD_JPEG_IPM                   0x96

#define CMD_ENABLE_SAM_UNI               0xA0 // for Mstreamer mode
#define CMD_FLIP_TO_DISP                 0xA1 // for Mstreamer mode

#define CMD_MIU_OFFSET                   0xA2 // saving miu offset from hk for LDMA usage
#define CMD_IQMEM_CTRL                   0xA3 // for iqmem ctrl from HK
#define CMD_IQMEM_CTRL_ACK               0xA4 // return ack by f/w
#define CMD_IQMEM_BASE_ADDR              0xA5 // unit in byte

// PES file-in command
#define CMD_PES_FILE_LOW_BND           0xA6 // for pes file in mode low bound
#define CMD_PES_FILE_UP_BND            0xA7 // for pes file in mode upper bound
#define CMD_PES_FILE_EN                0xA8 // for enable pes file in mode
#define CMD_PES_FILE_UPDATE_WPTR       0xA9 // for update pes file mode wr_ptr
#define CMD_PES_FILE_GET_RPTR          0xAA // for got pes file in mode rd_ptr

#define CMD_REGISTER_BASE              0xAB

#define CMD_DIVX_DISABLE               0xAC

//new feature
#define CMD_SUSPEND_DS                 0xB0
#define CMD_MPEG_LINER_START           0xB1
#define CMD_PREBUFFER_SIZE             0xB2 //unit:bytes
#define CMD_CC_ENABLE_EXTERNAL_BUFFER  0xB3
#define CMD_TIME_INCR_PREDICT	 	   0xB4 // to predict the "vol_time_incr" when there is no vol_header on mpeg4...
#define CMD_RUNTIME_DEBUG_CMD          0XB5
#define CMD_DUMP_MVD_HARDWARE_REGISTER 0xB6
#define CMD_SMOOTH_REWIND              0xB7 //Smooth_rewind
#define CMD_DECODE_ERROR_TOLERANCE     0xB8 //drop error rate
#define CMD_PVR_SEAMLESS_MODE          0xB9
#define CMD_AUTO_REDUCE_ES_DATA        0xBA
#define CMD_MVD_TLB                    0xBB
#define CMD_FRC_ONLY_SHOW_TOP_FIELD    0xBC
#define CMD_THUMBNAIL_LESS_FB_MODE     0xBD
#define CMD_ES_FULL_STOP               0xBE
#define CMD_SET_DV_XC_SHM_ADDR         0xBF
#define CMD_DISABLE_PATCH_PBFRAME      0xC0
#define CMD_AUTO_DROP_FRAME_IN_DECODE  0xC1
#define CMD_DYNAMIC_MVOP_CONNECT       0xC2
#define CMD_SLOW_SYNC_REPEAT           0xC3
#define CMD_SLOW_SYNC_SKIP             0xC4
#define CMD_VARIABLE_FRAMERATE         0xC5
#define CMD_DYNAMIC_SCALE_SIZE         0xC6
#define CMD_PUSI_CONTROL               0xC7
#define CMD_PVR_HEADER_EVENT           0xC8
#define CMD_PRESET_CONNET_INPUT_TSP    0xC9
#define CMD_ENABLE_NDS                 0xCA

#define MVD_FHD_ES_MIN_TSP_DATA_SIZE 0x69500
#define MVD_HD_ES_MIN_TSP_DATA_SIZE 0x42000
#define MVD_DEFAULT_ES_MIN_TSP_DATA_SIZE 0x16000
#define MVD_SINGLE_ES_MIN_TSP_DATA_SIZE 0x400

// add data in bitstream from skip mode back to normal
// 00_00_01_C5_ab_08_06_27
#define FW_SPE_SCODE                     0xC5
#define FW_RESUME1                       0xab08
#define FW_RESUME2                       0x0627
#define FILE_PAUSE_SC                    0xBE
#define FILE_END_SC                      0xC6
#define FILE_END_EXT1                    0xaabb
#define FILE_END_EXT2                    0xccdd
#define FILE_END_EXT3                    0xeeff
#define FILE_END_EXT4                    0xffff
#define FILE_END_EXT5                    0x0000
// For Git Changes
#define GIT_TIMESTAMP 1516791421
#endif
