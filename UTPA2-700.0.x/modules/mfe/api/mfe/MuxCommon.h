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
#ifndef _MUX_COMMON_H_
#define _MUX_COMMON_H_


//#define FILE_TEST    // If defined, will write out muxed file into D: drive.
// Allow reading all unread data
//#define CHECK_READ_WRITE_PTR

#define MAX_STREAM_COUNT 2
#define SAMPLES_IN_AUDIO_FRAME 1152

typedef enum{
    MUX_TS = 0,
    MUX_AVI,
    MUX_MP4
} MUX_TYPE;
//////////////////////////////////////////////////////////////////////////
// OutStream

typedef enum {
    INDEX_TYPE_SEQ = 0,
	INDEX_TYPE_I_PIC,
    INDEX_TYPE_SEQ_I_PIC,
    INDEX_TYPE_P_PIC,
    INDEX_TYPE_B_PIC,
    INDEX_TYPE_NONE
} INDEX_TYPE_T;

typedef struct {
    INDEX_TYPE_T indexType;
    MFE_U32 byteOffset;
} IndexType;

typedef struct {
    IndexType index_data;
    MFE_U32 used;
    MFE_U32 next;
} IndexRingEntry;

typedef struct {
#ifdef FILE_TEST
    void* handle;    // open file handle
#endif
    MFE_U8 *bbuffer;    // big buffer start
    MFE_U8 *bbufend_ptr;    // (bbufend_ptr - bbuffer) is the size of big buffer
    MFE_U8 *write_ptr, *read_ptr;
    MFE_U64 total_size;    // latest ts stream length

    // Index buffer
    IndexRingEntry* index_ring;
    MFE_U32 ring_size;
    MFE_U32 index_read, index_write;

    // Bookmark: for delayed read/write
    MFE_U32 locked;
    MFE_U8 *bookmark;
} Outstream;

#define MAX_UPDATE_COUNT 1
typedef struct OutDataInfo {
    MFE_U8 *header;
    int size1;
    MFE_U8 *data;
    int size2;
    int write_type;    // 0 or 1
    INDEX_TYPE_T type;
    int write_timecode; // 0 or 1
    MFE_U32 timecode;
    // AVI stuffing
    MFE_U32 stuffing;
    // Support multiple segments in one video/audio frame
    MFE_U8 lock_write;
    MFE_U8 update_count;
    MFE_U32 update_addr[MAX_UPDATE_COUNT];    // Offset from the bookmark position.
    MFE_U8* update_data[MAX_UPDATE_COUNT];
    MFE_U32 update_size[MAX_UPDATE_COUNT];
} OutDataInfo;

void outbuf_init(Outstream *s);
void outbuf_put(Outstream *s, OutDataInfo* pinfo);
void outbuf_flush(Outstream *s);
void outbuf_close(Outstream *s);
int outbuf_get(Outstream *s, MFE_U8 **ppbuf, MFE_U8 **ppread, MFE_U8 **ppwrite, IndexRingEntry **ppindex);
void outbuf_getdone(Outstream *s, MFE_U8 *pread, MFE_U32 index_read);

static __inline int put16_msbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val>>8;
    *q++ = val&0xFF;
    *ptr = q;
    return 2;
}

static __inline int put16_lsbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val&0xFF;
    *q++ = val>>8;
    *ptr = q;
    return 2;
}

static __inline int put32_msbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val>>24;
    *q++ = (val>>16)&0xFF;
    *q++ = (val>>8)&0xFF;
    *q++ = val&0xFF;
    *ptr = q;
    return 4;
}

static __inline int put32_lsbf(MFE_U8 **ptr, int val)
{
    MFE_U8 *q;
    q = *ptr;
    *q++ = val&0xFF;
    *q++ = (val>>8)&0xFF;
    *q++ = (val>>16)&0xFF;
    *q++ = val>>24;
    *ptr = q;
    return 4;
}

//////////////////////////////////////////////////////////////////////////
// Media and Access Unit
#define MAX_VALUE            (1<<30)
#ifdef _WIN32
#if (_MSC_VER < 1400) //vc 6 or older version
#define INVALID_PTS         (0x8000000000000000##i64)
#define MAXVALUE_PTS		(0x7FFFFFFFFFFFFFFF##i64)
#else
#define INVALID_PTS         ((MFE_S64)(0x8000000000000000))
#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFF))
#endif
//#define INVALID_PTS         ((MFE_S64)(0x8000000000000000))
//#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFF))
#else

#define INVALID_PTS         (0x8000000000000000##ULL)
#define MAXVALUE_PTS		((0x7FFFFFFFFFFFFFFF##ULL)

//#define INVALID_PTS         ((MFE_S64)(0x8000000000000000LL))
//#define MAXVALUE_PTS		((MFE_S64)(0x7FFFFFFFFFFFFFFFLL))
#endif
typedef enum _CodecType {
    CODEC_TYPE_VIDEO,
    CODEC_TYPE_AUDIO,
} CodecType;

typedef enum _CodecID {
    CODEC_ID_NONE = 999,
    // Video
    CODEC_ID_VIDEO_MPEG2 = 10,
    CODEC_ID_VIDEO_MPEG4 = 0,
    CODEC_ID_VIDEO_H264  = 2,
    // Audio
    CODEC_ID_AUDIO_MP2   = 101,
    CODEC_ID_AUDIO_MP3   = 102,
} CodecID;

typedef struct StreamContext {
    CodecType codec_type;
    CodecID codec_id;
    int bit_rate;    // Average bitrate
    MFE_S64 payload_pts;
    MFE_S64 payload_dts;
    MFE_U8 *payload;

    /* MPEG TS related */

    /*MpegTSProgram*/void *program;
    int pid; // stream pid
    int cc;        // continuity_counter

    /* AVI related */
    // Internal usage
    MFE_U32 frame_count;
    // Input: Video
    MFE_U32 width;
    MFE_U32 height;
    float framerate;
    // Input: Audio
    MFE_U32 samplerate;
} StreamContext;

typedef struct MediaContext {    // Media Context
    void* muxinfo;            // Hold mux-format specific internal data
    Outstream* outstream;    // Output context
    MFE_U32 stream_count;
    StreamContext* streams[MAX_STREAM_COUNT];
    MFE_S64 timestamp;
} MediaContext;

typedef struct AUContext {    // Access Unit Context
    MFE_U8 *access_unit;
    MFE_U32 unit_size;
    MFE_S64 pts;
    MFE_S64 dts;
    int   stream_index;    // index into MediaContext.streams
    INDEX_TYPE_T index_type;
    // (For multiple segments for one video/audio frame)
    MFE_U8 first_segment;
    MFE_U8 last_segment;
} AUContext;

//////////////////////////////////////////////////////////////////////////
// muxer structures

typedef struct avmux_s
{
    MFE_U32 state;

    // internal function
    int (*write_open)(struct avmux_s *inst);
    int (*write_pause)(struct avmux_s *inst);
    int (*write_packet)(struct avmux_s *inst, AUContext *packet);
    int (*write_resume)(struct avmux_s *inst);
    int (*write_close)(struct avmux_s *inst);
    // read functions
    int (*read)(struct avmux_s *inst, MFE_U8 **ppStreamBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
    void (*read_done)(struct avmux_s *inst, MFE_U8 *pBufReadPtr, MFE_U32 IndexRead);

    MediaContext* mctx;
    void*          endinfo;
}avmux_st, *pavmux_st;

// Return 0 if success (buffer not empty).
int mux_read(struct avmux_s *inst, MFE_U8 **ppStreamBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
// pBufReadPtr: Updated read pointer of bitstream buffer
// IndexRead: Number of index read.
void mux_readdone(struct avmux_s *inst, MFE_U8 *pBufReadPtr, MFE_U32 IndexRead);

#endif // _MUX_COMMON_H_
