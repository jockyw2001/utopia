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
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


#include <assert.h>
#include <string.h>

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "madp_ms_dprintf.h"

#include "MuxCommon.h"
#include "TsMuxer.h"
#include <pthread.h>
#ifdef FILE_TEST
    #include <stdio.h>
#endif

#define MAX_VALUE           (1<<30)
//#define INVALID_PTS         ((INT64_C)(0x8000000000000000LL))

#define TS_SYNCBYTE  0x47
#define TS_PACKET_SIZE 188

#ifdef TS_192_BYTE
	#define TIMECODE_AHEAD			1000
    #define TIMECODE_CLOCK			90000//27000000
#endif

#define DEFAULT_SID             0x0001
#define DEFAULT_PMT_START_PID   0x0100
#define DEFAULT_START_PID       0x0064    // Valid range [0x0010, 0x1FFE]

// PAT
#define TID_PAT                 0x00        // table_id
#define TSID_DEFAULT            0x0001        // transport_stream_id
#define PID_PAT                 0x0000

// PMT stream_type
#define PMT_STREAM_TYPE_13818_2     0x02
#define PMT_STREAM_TYPE_11172_3     0x03
#define PMT_STREAM_TYPE_14496_2     0x10
#define PMT_STREAM_TYPE_14496_10    0x1b
#define TID_PMT                        0x02    // table_id

// PES related
#define DEFAULT_PES_HEADER_FREQ 16
#define DEFAULT_PES_PAYLOAD_SIZE ((DEFAULT_PES_HEADER_FREQ - 1) * 184 + 170)
#define PES_NO_VIDEO_PAYLOADSIZE    // If defined, PES payload size is filled as 0
#define PES_MAX_PAYLOADSIZE  0xFFFF

#define PAT_RETRANS_TIME 100

#define PCR_TRANSMISSION
#define PCR_RETRANS_TIME 20

//////////////////////////////////////////////////////////////////////////
// MPEG-TS SI info
//////////////////////////////////////////////////////////////////////////

static const MFE_U32 gCRC32Table[256] = {
    0x00000000, 0xB71DC104, 0x6E3B8209, 0xD926430D, 0xDC760413, 0x6B6BC517, 0xB24D861A, 0x0550471E,
    0xB8ED0826, 0x0FF0C922, 0xD6D68A2F, 0x61CB4B2B, 0x649B0C35, 0xD386CD31, 0x0AA08E3C, 0xBDBD4F38,
    0x70DB114C, 0xC7C6D048, 0x1EE09345, 0xA9FD5241, 0xACAD155F, 0x1BB0D45B, 0xC2969756, 0x758B5652,
    0xC836196A, 0x7F2BD86E, 0xA60D9B63, 0x11105A67, 0x14401D79, 0xA35DDC7D, 0x7A7B9F70, 0xCD665E74,
    0xE0B62398, 0x57ABE29C, 0x8E8DA191, 0x39906095, 0x3CC0278B, 0x8BDDE68F, 0x52FBA582, 0xE5E66486,
    0x585B2BBE, 0xEF46EABA, 0x3660A9B7, 0x817D68B3, 0x842D2FAD, 0x3330EEA9, 0xEA16ADA4, 0x5D0B6CA0,
    0x906D32D4, 0x2770F3D0, 0xFE56B0DD, 0x494B71D9, 0x4C1B36C7, 0xFB06F7C3, 0x2220B4CE, 0x953D75CA,
    0x28803AF2, 0x9F9DFBF6, 0x46BBB8FB, 0xF1A679FF, 0xF4F63EE1, 0x43EBFFE5, 0x9ACDBCE8, 0x2DD07DEC,
    0x77708634, 0xC06D4730, 0x194B043D, 0xAE56C539, 0xAB068227, 0x1C1B4323, 0xC53D002E, 0x7220C12A,
    0xCF9D8E12, 0x78804F16, 0xA1A60C1B, 0x16BBCD1F, 0x13EB8A01, 0xA4F64B05, 0x7DD00808, 0xCACDC90C,
    0x07AB9778, 0xB0B6567C, 0x69901571, 0xDE8DD475, 0xDBDD936B, 0x6CC0526F, 0xB5E61162, 0x02FBD066,
    0xBF469F5E, 0x085B5E5A, 0xD17D1D57, 0x6660DC53, 0x63309B4D, 0xD42D5A49, 0x0D0B1944, 0xBA16D840,
    0x97C6A5AC, 0x20DB64A8, 0xF9FD27A5, 0x4EE0E6A1, 0x4BB0A1BF, 0xFCAD60BB, 0x258B23B6, 0x9296E2B2,
    0x2F2BAD8A, 0x98366C8E, 0x41102F83, 0xF60DEE87, 0xF35DA999, 0x4440689D, 0x9D662B90, 0x2A7BEA94,
    0xE71DB4E0, 0x500075E4, 0x892636E9, 0x3E3BF7ED, 0x3B6BB0F3, 0x8C7671F7, 0x555032FA, 0xE24DF3FE,
    0x5FF0BCC6, 0xE8ED7DC2, 0x31CB3ECF, 0x86D6FFCB, 0x8386B8D5, 0x349B79D1, 0xEDBD3ADC, 0x5AA0FBD8,
    0xEEE00C69, 0x59FDCD6D, 0x80DB8E60, 0x37C64F64, 0x3296087A, 0x858BC97E, 0x5CAD8A73, 0xEBB04B77,
    0x560D044F, 0xE110C54B, 0x38368646, 0x8F2B4742, 0x8A7B005C, 0x3D66C158, 0xE4408255, 0x535D4351,
    0x9E3B1D25, 0x2926DC21, 0xF0009F2C, 0x471D5E28, 0x424D1936, 0xF550D832, 0x2C769B3F, 0x9B6B5A3B,
    0x26D61503, 0x91CBD407, 0x48ED970A, 0xFFF0560E, 0xFAA01110, 0x4DBDD014, 0x949B9319, 0x2386521D,
    0x0E562FF1, 0xB94BEEF5, 0x606DADF8, 0xD7706CFC, 0xD2202BE2, 0x653DEAE6, 0xBC1BA9EB, 0x0B0668EF,
    0xB6BB27D7, 0x01A6E6D3, 0xD880A5DE, 0x6F9D64DA, 0x6ACD23C4, 0xDDD0E2C0, 0x04F6A1CD, 0xB3EB60C9,
    0x7E8D3EBD, 0xC990FFB9, 0x10B6BCB4, 0xA7AB7DB0, 0xA2FB3AAE, 0x15E6FBAA, 0xCCC0B8A7, 0x7BDD79A3,
    0xC660369B, 0x717DF79F, 0xA85BB492, 0x1F467596, 0x1A163288, 0xAD0BF38C, 0x742DB081, 0xC3307185,
    0x99908A5D, 0x2E8D4B59, 0xF7AB0854, 0x40B6C950, 0x45E68E4E, 0xF2FB4F4A, 0x2BDD0C47, 0x9CC0CD43,
    0x217D827B, 0x9660437F, 0x4F460072, 0xF85BC176, 0xFD0B8668, 0x4A16476C, 0x93300461, 0x242DC565,
    0xE94B9B11, 0x5E565A15, 0x87701918, 0x306DD81C, 0x353D9F02, 0x82205E06, 0x5B061D0B, 0xEC1BDC0F,
    0x51A69337, 0xE6BB5233, 0x3F9D113E, 0x8880D03A, 0x8DD09724, 0x3ACD5620, 0xE3EB152D, 0x54F6D429,
    0x7926A9C5, 0xCE3B68C1, 0x171D2BCC, 0xA000EAC8, 0xA550ADD6, 0x124D6CD2, 0xCB6B2FDF, 0x7C76EEDB,
    0xC1CBA1E3, 0x76D660E7, 0xAFF023EA, 0x18EDE2EE, 0x1DBDA5F0, 0xAAA064F4, 0x738627F9, 0xC49BE6FD,
    0x09FDB889, 0xBEE0798D, 0x67C63A80, 0xD0DBFB84, 0xD58BBC9A, 0x62967D9E, 0xBBB03E93, 0x0CADFF97,
    0xB110B0AF, 0x060D71AB, 0xDF2B32A6, 0x6836F3A2, 0x6D66B4BC, 0xDA7B75B8, 0x035D36B5, 0xB440F7B1,
};

MFE_U32 getCRC(const MFE_U8 *buffer, size_t length)
{
    const MFE_U8 *end= buffer+length;
    MFE_U32 crc = 0xFFFFFFFF;
    while(buffer<end)
        crc = (crc >> 8) ^ gCRC32Table[(crc & 0xFF) ^ *buffer++];
    return crc;
}

#ifdef TS_192_BYTE
MFE_U32 GetTimecode(MediaContext* ctx, int data_len)
{
	int mux_rate, increment;
    MpegTSWrite *ts = (MpegTSWrite*)ctx->muxinfo;

	if (ts->waited_datasize<=0 || ts->min_time<=ctx->timestamp+TIMECODE_AHEAD)
		mux_rate = ts->mux_rate;
	else
		mux_rate = ts->waited_datasize * TIMECODE_CLOCK / (int)(ts->min_time - (ctx->timestamp+TIMECODE_AHEAD));
	ts->waited_datasize -= data_len;

	increment = data_len * TIMECODE_CLOCK / mux_rate - 1;
	if (increment<=0) increment = 1;
	ctx->timestamp += increment;
#if 0
	assert(ctx->streams[0]->payload_pts==INVALID_PTS || ctx->timestamp<ctx->streams[0]->payload_pts);
	assert(ctx->streams[1]->payload_pts==INVALID_PTS || ctx->timestamp<ctx->streams[1]->payload_pts);
#endif
	return (MFE_U32)(ctx->timestamp & 0x3FFFFFFF); // % 2^30
}
#endif

static void mpegts_write_section(MediaContext* ctx, MpegTSSection *s, int table_id, int id, MFE_U8 *section, MFE_U32 len)
{
    MFE_U8 *q, *buf_ptr;
    MFE_U32 tot_len, first, b, len1, left, crc;
    MpegTSWrite *ts = (MpegTSWrite*)ctx->muxinfo;
    MFE_U8 packet[TS_PACKET_SIZE];
    OutDataInfo data_info;

    tot_len = 8 + len + 4;    // 4: CRC bytes
    assert(tot_len<=1024);

    // First common 8 bytes
    q = section;
    *q++ = table_id;
    put16_msbf(&q, 0xb000 | (len + 5 + 4)); // header:5 , CRC:4
    put16_msbf(&q, id);    // transport_stream_id (PAT) or program_number (PMT)
    *q++ = 0xc1;    // version_number=0, current_next_indicator=1
    *q++ = 0x0;        // section_number
    *q++ = 0x0;        // last_section_number

    // CRC
    crc = getCRC(section, tot_len - 4);
    buf_ptr = &section[tot_len-4];
    put32_lsbf(&buf_ptr, crc);

    // ts packets
    buf_ptr = section;
    while (tot_len > 0) {
        first = (section == buf_ptr);
        q = packet;
        *q++ = TS_SYNCBYTE;
        b = (s->pid >> 8);
        if (first)
            b |= 0x40;    // payload_unit_start_indicator=1
        *q++ = b;
        *q++ = s->pid;
        *q++ = 0x10 | s->cc;    // no scrambling, payload only
        s->cc = (s->cc + 1) & 0xF;
        if (first)
            *q++ = 0x00;    // pointer_field
        len1 = TS_PACKET_SIZE - (q - packet);
        if (len1 > tot_len)
            len1 = tot_len;
        memcpy(q, buf_ptr, len1);
        q += len1;
        // padding bytes
        left = TS_PACKET_SIZE - (q - packet);
        if (left > 0)
            memset(q, 0xFF, left);

        data_info.header = packet;
        data_info.size1 = TS_PACKET_SIZE;
        data_info.data = 0;
        data_info.size2 = 0;
        data_info.write_type = 1;
        data_info.type = INDEX_TYPE_NONE;
#ifdef TS_192_BYTE
        data_info.write_timecode = 1;
        data_info.timecode = GetTimecode(ctx, 0);
#else
        data_info.write_timecode = 0;
#endif
        data_info.stuffing = 0;
        data_info.lock_write = 0;
        data_info.update_count = 0;
        outbuf_put(ctx->outstream, &data_info);

        buf_ptr += len1;
        tot_len -= len1;
#ifdef PCR_TRANSMISSION
        ts->cur_pcr += TS_PACKET_SIZE*8*90000L/ts->mux_rate;
#endif
    }
}

// Only 1 program
static void mpegts_write_pat(MediaContext *s)
{
    MpegTSWrite *ts = (MpegTSWrite*)s->muxinfo;
    MFE_U8 data[1024], *q, *pat_start;

    q = pat_start = data+8;
    put16_msbf(&q, ts->program.prog_num);    // program_number
    put16_msbf(&q, 0xe000 | ts->program.pmt.pid);    // program_map_PID
    mpegts_write_section(s, &ts->pat, TID_PAT, ts->tsid, data, q - pat_start);
}

static void mpegts_write_pmt(MediaContext *s, MpegTSProgram *program)
{
    MFE_U8 data[1024], *q, *pmt_start, *p_es_desc_len, *p_prog_desc_len;
    int val;
	MFE_U32 stream_type=0, i;

    q = pmt_start = data+8;
    put16_msbf(&q, 0xE000 | program->pcr_pid);

    p_prog_desc_len = q;
    q += 2;

    // Optional program descriptor goes here

    val = 0xF000 | (q - p_prog_desc_len - 2);
    p_prog_desc_len[0] = val >> 8;
    p_prog_desc_len[1] = val;

    for (i=0; i<s->stream_count; i++) {
        StreamContext *st = s->streams[i];
        switch(st->codec_id) {
        case CODEC_ID_VIDEO_MPEG4:
            stream_type = PMT_STREAM_TYPE_14496_2;
            break;
        case CODEC_ID_AUDIO_MP2:
        case CODEC_ID_AUDIO_MP3:
            stream_type = PMT_STREAM_TYPE_11172_3;
            break;
        case CODEC_ID_VIDEO_MPEG2:
            stream_type = PMT_STREAM_TYPE_13818_2;
            break;
        case CODEC_ID_VIDEO_H264:
            stream_type = PMT_STREAM_TYPE_14496_10;
            break;
        case CODEC_ID_NONE:
            assert(0);
            //printf("wrong codec in TSMUXER\n");
            break;
        }
        *q++ = stream_type;
        put16_msbf(&q, 0xE000 | st->pid);
        p_es_desc_len = q;
        q += 2;

        // Optional stream descriptor goes here
        switch(st->codec_type) {
        case CODEC_TYPE_AUDIO:
            break;
        case CODEC_TYPE_VIDEO:
            break;
        }

        val = 0xF000 | (q - p_es_desc_len - 2);
        p_es_desc_len[0] = val >> 8;
        p_es_desc_len[1] = val;
    }
    mpegts_write_section(s, &program->pmt, TID_PMT, program->prog_num, data, q - pmt_start);
}

static void resend_PAT_PMT(MediaContext *s)
{
    MpegTSWrite *ts = (MpegTSWrite*)s->muxinfo;

    if (++ts->pat_packet_count == ts->pat_packet_freq) {
        ts->pat_packet_count = 0;
        mpegts_write_pat(s);
        mpegts_write_pmt(s, &ts->program);
    }
}

//////////////////////////////////////////////////////////////////////////
// MPEG-TS PES packet
//////////////////////////////////////////////////////////////////////////

// Write 33-bit PTS or DTS
static void write_ptsdts(MFE_U8 *q, int fourbits, MFE_S64 pts)
{
    int val;

    pts = pts & MFE_INT64_C(0x1FFFFFFFF);	// % 2^33
//       pts &= (MFE_S64)(0x1FFFFFFFF);	// % 2^33
       
    val = fourbits << 4 | ((((MFE_U32)(pts>>30)) & 0x07) << 1) | 1;
    *q++ = val;
    val = ((((MFE_U32)(pts>>15)) & 0x7fff) << 1) | 1;
    *q++ = val >> 8;
    *q++ = val;
    val = (((MFE_U32)(pts&0x7fff)) << 1) | 1;
    *q++ = val >> 8;
    *q++ = val;
}

// The payload must be either:
//    one video frame (with headers after the last bytes of the last frame), or
//    one audio frame.
// If audio stream, index_type must be INDEX_TYPE_NONE.
static void mpegts_write_pes(MediaContext *s, StreamContext *st, MFE_U32 payload_size, INDEX_TYPE_T index_type, MFE_U8 first, MFE_U8 last)
{
    MpegTSWrite *ts = (MpegTSWrite*)s->muxinfo;
    MpegTSProgram* program = (MpegTSProgram*)st->program;
    MFE_U8 *payload = st->payload;
    MFE_S64 pts = st->payload_pts;
    MFE_S64 dts = st->payload_dts;
    MFE_U8 buf[TS_PACKET_SIZE];
    MFE_U8 *q;
    int val, is_start, write_pcr, flags;
    MFE_U32 len, header_len, data_len, stuffing_len;
    INDEX_TYPE_T type;
    OutDataInfo data_info;
#ifdef PCR_TRANSMISSION
    MFE_S64 pcr;
#endif

    //assert( !(index_type!=INDEX_TYPE_NONE&&!last) );

    is_start = first;//1;
    stuffing_len = 0;
    if (ts->temp_size>0) {
        assert(ts->temp_size<=TS_PACKET_SIZE);
        payload -= ts->temp_size;
        payload_size += ts->temp_size;
        memcpy(payload, ts->temp188, ts->temp_size);
        ts->temp_size = 0;
    }
    type = index_type;
    while (payload_size > 0) {
        resend_PAT_PMT(s);

        write_pcr = 0;
#ifdef PCR_TRANSMISSION
        if (st->pid == program->pcr_pid) {
            program->pcr_packet_count++;
            if (program->pcr_packet_count>=program->pcr_packet_freq) {
                program->pcr_packet_count = 0;
                write_pcr = 1;
            }
        }
#endif
        // ts packet header: 4 bytes
        q = buf;
        *q++ = TS_SYNCBYTE;
        val = (st->pid >> 8);
        if (is_start)
            val |= 0x40;    // payload_unit_start_indicator=1
        *q++ = (MFE_U8)val;
        *q++ = (MFE_U8)st->pid;
        *q++ = (MFE_U8)((write_pcr ? 0x30 : 0x10) | st->cc);    // no scrambling
        //st->cc = (st->cc + 1) & 0xF;

        // adaptation_field and/or PES_packet() starts here.
#ifdef PCR_TRANSMISSION
        if (write_pcr) {
            // add 11, pcr references the last byte of program clock reference base
            pcr = ts->cur_pcr + (4+7)*8*90000L / ts->mux_rate;
            if (dts != INVALID_PTS && dts < pcr)
                assert(0);
            *q++ = 7;        // adaptation_field_length
            *q++ = 0x10;    // Has PCR
            *q++ = (MFE_U8)(pcr >> 25);
            *q++ = (MFE_U8)(pcr >> 17);
            *q++ = (MFE_U8)(pcr >> 9);
            *q++ = (MFE_U8)(pcr >> 1);
            *q++ = (MFE_U8)((pcr&1) << 7);
            *q++ = 0;
        }
#endif
        // Stuffing bytes by adaptation_field, if necessary.
        header_len = q-buf;
        if (is_start) {
            header_len += 9;
            if (pts != INVALID_PTS) {
                header_len += 5;
                if (dts != pts)
                    header_len += 5;
            }
        }
        data_len = TS_PACKET_SIZE - header_len;
        if (data_len > payload_size) {
            stuffing_len = data_len - payload_size;
            data_len = payload_size;
            if (buf[3] & 0x20) {
                // already has adaptation_field
                len = buf[4] + 1;
                buf[4] += stuffing_len;    // Update length
                memset(q, 0xFF, stuffing_len);
                q += stuffing_len;
            }
            else {
                buf[3] |= 0x20;    // notify existence of adaptation_field
                *q++ = stuffing_len-1;    // adaptation_field_length is used for 1-byte stuffing.
                if (stuffing_len >= 2) {
                    *q++ = 0x00;    // All flags are zero.
                    memset(q, 0xFF, stuffing_len-2);
                    q += stuffing_len-2;
                }
            }
        }

        if (is_start) {
            // PES header
            *q++ = 0x00;
            *q++ = 0x00;
            *q++ = 0x01;
            if (st->codec_type == CODEC_TYPE_VIDEO) {
                *q++ = 0xe0;    // MPEG-1 or MPEG-2 or MPEG-4 or H.264 video
            } else if (st->codec_type == CODEC_TYPE_AUDIO &&
                (st->codec_id==CODEC_ID_AUDIO_MP2 || st->codec_id==CODEC_ID_AUDIO_MP3)) {
                *q++ = 0xc0;
            }
            header_len = 0;
            flags = 0;
            if (pts != INVALID_PTS) {
                header_len += 5;
                flags |= 0x80;
                if (dts != pts) {
                    header_len += 5;
                    flags |= 0x40;
                }
            }
            len = payload_size + header_len + 3;
#ifdef PES_NO_VIDEO_PAYLOADSIZE
            if (st->codec_type == CODEC_TYPE_VIDEO) {
                *q++ = 0;
                *q++ = 0;
            }
            else
#endif
            {
                assert(len<PES_MAX_PAYLOADSIZE);
                *q++ = len >> 8;
                *q++ = len;
            }
            val = 0x80;
            *q++ = val;
            *q++ = flags;
            *q++ = header_len;
            if (pts != INVALID_PTS) {
                write_ptsdts(q, flags >> 6, pts);
                q += 5;
                if (dts != pts) {
                    write_ptsdts(q, 1, dts);
                    q += 5;
                }
            }
            //is_start = 0;
        }

        // pes_packet_data_byte
        //memcpy(q, payload, data_len);
        assert(q+data_len-buf==TS_PACKET_SIZE);

        if (!last && stuffing_len>0) {
            assert(ts->temp_size==0);
            memcpy(ts->temp188, payload, payload_size);
            ts->temp_size = payload_size;
            break;
        }

        data_info.header = buf;
        data_info.size1 = TS_PACKET_SIZE-data_len;
        data_info.data = payload;
        data_info.size2 = data_len;
        data_info.write_type = 1;
        data_info.type = (last&&is_start) ? type : INDEX_TYPE_NONE;
#ifdef TS_192_BYTE
        data_info.write_timecode = 1;
        data_info.timecode = GetTimecode(s, data_len);
#else
        data_info.write_timecode = 0;
#endif
        data_info.stuffing = 0;
        data_info.lock_write = last ? 0 : 1;
        data_info.update_count = 0;
        outbuf_put(s->outstream, &data_info);

        // Prepare for next round
        st->cc = (st->cc + 1) & 0xF;
        payload += data_len;
        payload_size -= data_len;
        if (is_start) {
            is_start = 0;
            type = INDEX_TYPE_NONE;
        }
#ifdef PCR_TRANSMISSION
        ts->cur_pcr += TS_PACKET_SIZE*8*90000L/ts->mux_rate;
#endif
    }
}

//////////////////////////////////////////////////////////////////////////
// TS-mux external API's
//////////////////////////////////////////////////////////////////////////

int tsmux_open(struct avmux_s *inst)
{
    MediaContext *s = inst->mctx;
    MpegTSWrite *ctx = (MpegTSWrite*)s->muxinfo;
    MpegTSProgram *program = &ctx->program;
    StreamContext *st;
    MFE_U32 i, total_bit_rate;
    MFE_U32 pat_pmt_size;
    MFE_U64 pos;

#ifdef TS_192_BYTE
    s->timestamp = 0;
#endif

    outbuf_init(s->outstream);

    ctx->tsid = TSID_DEFAULT;
    ctx->pat.pid = PID_PAT;
    ctx->pat.cc = 0;
    ctx->pat_packet_count = 0;
    ctx->cur_pcr = 0;
    ctx->temp_size = 0;
	ctx->min_time = 0;
	ctx->waited_datasize = 0;

    /* Initial the single program */
    program->pmt.pid = DEFAULT_PMT_START_PID;
    program->pmt.cc =0;
    program->prog_num = DEFAULT_SID;
    program->pcr_pid = 0x1FFF;
    program->pcr_packet_count = 0;

    // Initial streams
    total_bit_rate = 0;
    for(i=0; i<s->stream_count; i++) {
        st = s->streams[i];
        st->cc = 0;
        st->program = program;
        if (st->pid==0)    // If not forced.
            st->pid = DEFAULT_START_PID + i;
        st->payload_pts = INVALID_PTS;
        st->payload_dts = INVALID_PTS;
        // PCR pid is the first video stream
        if (st->codec_type == CODEC_TYPE_VIDEO && program->pcr_pid == 0x1FFF)
            program->pcr_pid = st->pid;
        total_bit_rate += st->bit_rate;
    }

    // if no video stream, use the first stream as PCR
    if (program->pcr_pid == 0x1FFF && s->stream_count > 0) {
        program->pcr_pid = s->streams[0]->pid;
    }

    if (total_bit_rate <= 8 * 1024)
        total_bit_rate = 8 * 1024;
    program->pcr_packet_freq = MAX_VALUE;//(total_bit_rate * PCR_RETRANS_TIME) / (TS_PACKET_SIZE * 8 * 1000);
    ctx->pat_packet_freq = MAX_VALUE;//(total_bit_rate * PAT_RETRANS_TIME) / (TS_PACKET_SIZE * 8 * 1000);

    ctx->mux_rate = 1+total_bit_rate;
    pos = s->outstream->total_size;
    mpegts_write_pat(s);
    mpegts_write_pmt(s, &ctx->program);
    pat_pmt_size = (MFE_U32)(s->outstream->total_size - pos);

    total_bit_rate +=
        total_bit_rate * 25 / (8 * DEFAULT_PES_PAYLOAD_SIZE) // PES header
        + total_bit_rate * 4 / (8 * TS_PACKET_SIZE)          // TS header
        + PAT_RETRANS_TIME * pat_pmt_size                     // PAT+PMT size
#ifdef PCR_TRANSMISSION
        + PCR_RETRANS_TIME * 8                              // PCR size
#endif
        ;
    ctx->mux_rate = total_bit_rate;

#ifdef PCR_TRANSMISSION
    ctx->cur_pcr /= ctx->mux_rate;
#endif

    return 0;
}

int tsmux_write_packet(struct avmux_s *inst, AUContext *pkt)
{
    MediaContext *s = inst->mctx;
#ifdef TS_192_BYTE
    MpegTSWrite *ts = (MpegTSWrite*)s->muxinfo;
    int i;
    MFE_S64 min_ts;
#endif
    StreamContext *st = s->streams[pkt->stream_index];
    MFE_U8 *buf= pkt->access_unit;
    //const MFE_U8 *access_unit_index = NULL;
    MFE_S64 dts = INVALID_PTS, pts = INVALID_PTS;

    //assert(pkt->first_segment==1 && pkt->last_segment==1);
    if (pkt->pts != INVALID_PTS)
        pts = pkt->pts;
    if (pkt->dts != INVALID_PTS)
        dts = pkt->dts;

    st->payload = buf;
    st->payload_dts = dts;
    st->payload_pts = pts;

#ifdef TS_192_BYTE
    if (pkt->first_segment) {
        assert(pts!=INVALID_PTS);
		min_ts = s->streams[0]->payload_pts==INVALID_PTS ? 0 :  s->streams[0]->payload_pts;
		for (i=1; i<(int)s->stream_count; i++) {
            if (s->streams[i]->payload_pts==INVALID_PTS)
                min_ts = 0;
            else if (s->streams[i]->payload_pts<min_ts)
                min_ts = s->streams[i]->payload_pts;
        }
		ts->min_time = min_ts;
    }
	ts->waited_datasize += pkt->unit_size;
#endif

	mpegts_write_pes(s, st, pkt->unit_size, pkt->index_type, pkt->first_segment, pkt->last_segment);
    return 0;
}

int tsmux_close(struct avmux_s *inst)
{
    MediaContext *s = inst->mctx;
    outbuf_flush(s->outstream);
    outbuf_close(s->outstream);
    return 0;
}
