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

#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "madp_ms_dprintf.h"

#include "mdrv_mfe_io.h"
#include "mdrv_mfe_st.h"
#include "madp_mfe.h"

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
    #include <sys/mman.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <sys/prctl.h>   // use for thread naming
#if defined(_KERNEL_MODE_)
    #include <poll.h>       // poll
    #include "madp_system.h"
    #include "mdrv_system.h"
    #include "madp_msos.h"
    #include <sys/ioctl.h>
       #include "../mpool/madp_mpool.h"
#else
    #include "MsCommon.h"
    #include "MsIRQ.h"
    #include "MsOS.h"
       #include "drvMMIO.h"
       #include "drvMIU.h"
#endif
    #define cyg_current_time()    MsOS_GetSystemTime()
#endif

#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
    #include "pthread_map.h"
    #include "shellcfg.h"
#else
    #include <pthread.h>
#endif

#ifdef _ENC_PERFORMANCE_
    #define EMI_MEASURE
    #include "frame_in.h"
    #include "chiptop.h"
#endif

#include <assert.h>
//#ifdef _FILE_IN_
//#include <file.h>
//#endif

#if defined(_FILE_IN_) ||defined(_YUVGEN_)
#include "yuvgen.h"
#endif
#if defined(_YUVGEN_)||defined(_FILE_IN_)
// File IN
FILE_IN_INFO mfe_file_in;
#endif
//#define _EN_DROPFRAME_
#define VIDEO_SHIFT     0//21
int video_shift_frame;

static int mfe_fd = 0;

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
#define MFE_DEVICE "/dev/mfe"
#endif

#ifndef VVOIP_INTERFACE1
 typedef enum {
    MFE_Invalid            = 0x00,
    MFE_Init                 = 0x20,
    MFE_Idle                =0x30,
    MFE_Busy               =0x40,
    MFE_DeInit             =0x50,
    MFE_Error               =0x60
 } MFE_STATE;
#endif
MFE_STATE mfe_state = MFE_Invalid;

#include <semaphore.h>
//#include <linux/time.h>  //added
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    MFE_S32 madp_sem_input; // Input frame
    static pthread_mutex_t s32MFEMutex;
    //MS_S32   s32MFEMutex;
#else
    sem_t madp_sem_input; // Input frame
#endif


#if defined(CUSTOM_INTERFACE1)||defined(VVOIP_INTERFACE1)
static void* g_pCustomCtx;
static StreamCallback g_StreamCallBackFunc;
#endif    // CUSTOM_INTERFACE1

#if defined(VVOIP_INTERFACE1)
static CleanCallback g_CleanCallBackFunc;
MFE_U32 VVOIP_MFE_input_VAaddr;
#endif

//////////////////////////////////////////////////////////////////////////
// MUX related
//////////////////////////////////////////////////////////////////////////

MFE_U8* mux_frame_buf; //video tempral bitstream buffer

#ifdef _MUXTS_
    #include "TsMuxer.h"
#endif
#ifdef _MUX_WITH_AUDIO_
    #include "Framer.h"
#endif

#ifdef AUDIO_MUX_DUMP
FILE* AUDIO_DUMP;
#endif
#ifdef _MUXTS_
#define DEFAULT_VIDEO_ID CODEC_ID_VIDEO_MPEG4
//#define DEFAULT_VIDEO_ID CODEC_ID_VIDEO_MPEG2
//
#define DEFAULT_AUDIO_ID CODEC_ID_AUDIO_MP2
//
#define DEFAULT_VIDEO_BITRATE 4096000
#define DEFAULT_AUDIO_BITRATE 192000//384000//256000 test.MP2//160000
//
#define MPEG_TS_CLOCKRATE        90000
#define DEFAULT_VIDEO_FRAMERATE 30
#define DEFAULT_AUDIO_FREQUENCY 48000//32000 test.MP2//44100
//
#define DOWNLOADSTREAM_BUFFER    384000
#define DOWNLOADSTREAM_MAX_SESSION    96000
#define MAX_INDEX_RING_LENGTH    64
#define AUDIO_ONESHOT_FRAMECOUNT    4
// DEBUG
// #define DEBUG_SEGMENT   1000000

    StreamContext   g_avs[2];
    MediaContext    g_av_ct;
    AUContext       g_vid_auctx, g_aud_auctx;

    boolean g_bHasVideoFrame, g_bHasAudioFrame;
    MFE_U32 g_nAvailAudFrames;
    boolean g_bMuxTS = 0;
#ifdef _MUX_WITH_AUDIO_
    BitstreamInfo g_AudInfo; //ts_VidInfo,
#endif
    Outstream        g_outstream;
//    IndexRingEntry  *ts_index_ring;
    // MPEGTS specific
    MpegTSWrite     ts_muxinfo;
    avmux_st        g_avmux;
    /*
#ifdef DEBUG_READ
    MFE_U8   *ts_read_sbuf;
    #define READ_TIMER 4 // smaller means more frequent reading.
    FILE* ts_read_fp;
    MFE_U8 *ts_read_ptr;
    MFE_U32 ts_readsize;
//    MFE_U32 ts_loopcount;
#endif
*/
    int mux_prev_pos=0;
    int mux_curr_pos=0;
    int mux_frame_size;

    int mux_audframecount;
//    int mux_vidframecount;
    int mux_audioframesize;

    #define MIN_SESSION_SIZE 256000
    int mux_total_len;
#endif    // _MUXTS_

#if defined(_MUX_WITH_AUDIO_) && defined(_FILE_IN_)
    FILE* mux_mp2file = NULL;
#endif


static pthread_t thread_info; //stephen

int Encode_stop=1;
#define ADP_STATE_WAIT_GETBITS        0
#define ADP_STATE_WAIT_FRAMEDONE    1
#define ADP_STATE_WAIT_INPUT        2
int enc_state=0; //0: input_frame available and already frame done, 0: wait getbits  1: wait frame done  2: wait inputframe
int Madp_enc_frame_count=0;
int Madp_out_frame_count=0;
PVR_Info appPVR_Info;

#if defined(NEW_APVR_INTERFACE)
VENC_INFRAME_INFO mfe_pInfo;
GET_YUVFRAME_FUNC MAdp_MFE_GetDiBuffStatus/*(void*ppInfo)*/;
CLEAR_BUF_STATUS_FUNC MAdp_MFE_ClearDiBuffStatus/*(MFE_U16 DIP_status,void* ppInfo)*/;
THUMB_NAIL thumb_nail;
GET_AUDIO_DATA_FUNC MAdp_MFE_ReadAudioData/*(MFE_U32 *AudAddr, MFE_U32 *AudSize, MFE_U32 *AudPTS, MFE_U8 *AudFrameCnt)*/;
#endif
MFE_U32 DIP_Total_Num = 6; //the same to DIP_DI_BUFFER_COUNT in madp_apvr.c


MFE_INPUT_BUF_INFO madp_mfe_buff_info;

extern char* in_file_name;


static MFE_BOOL MAdp_MFE_Mutex_Init(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    pthread_mutex_init(&s32MFEMutex, NULL);
/*
    ms_dprintf(ADP_L1,"Before MsOS_CreateMutex!!\n");
     s32MFEMutex = MsOS_CreateMutex(E_MSOS_FIFO, "MFE_Mutex", MSOS_PROCESS_SHARED);
    ms_dprintf(ADP_L1,"After MsOS_CreateMutex!!\n");
    if(s32MFEMutex < 0)
    {
        ms_dprintf(ADP_L1,"%s MsOS_CreateMutex failed!!\n", __FUNCTION__);
        return FALSE;
    }
*/
    return TRUE;
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    no support now!
#endif

}

static MFE_BOOL MAdp_MFE_Mutex_Delete(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    return pthread_mutex_destroy(&s32MFEMutex);
    //return MsOS_DeleteMutex(s32MFEMutex);

#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    no support now!
#endif

}

static MFE_BOOL MAdp_MFE_Mutex_Lock(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
/*
    if (s32MFEMutex != -1)
    {
        return MsOS_ObtainMutex(s32MFEMutex, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
*/
    return pthread_mutex_lock(&s32MFEMutex);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    no support now!
#endif

}

static MFE_BOOL MAdp_MFE_Mutex_Unlock(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
/*
    if (s32MFEMutex != -1)
    {
        return MsOS_ReleaseMutex(s32MFEMutex);
    }
    else
    {
        return FALSE;
    }
*/
    return pthread_mutex_unlock(&s32MFEMutex);

#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    no support now!
#endif

}

static MFE_BOOL MAdp_MFE_Change_State(MFE_STATE in_state)
{
/*
    if(MAdp_MFE_Mutex_Lock()== FALSE)
        return FALSE;
    mfe_state = in_state;
    if(MAdp_MFE_Mutex_Unlock()== FALSE)
        return FALSE;
*/
MAdp_MFE_Mutex_Lock();
mfe_state = in_state;
MAdp_MFE_Mutex_Unlock();
    return TRUE;
}

static void MAdp_MFE_Sem_Init(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
     madp_sem_input = MsOS_CreateSemaphore(0,E_MSOS_FIFO ,"ADP_MFE_SEM") ;
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
     sem_init(&madp_sem_input, 0, 0);
#endif

}

static void MAdp_MFE_Sem_Wait(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
     MsOS_ObtainSemaphore(madp_sem_input, MSOS_WAIT_FOREVER);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
     sem_wait(&madp_sem_input);
#endif

}


static void MAdp_MFE_Sem_Destroy(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    MsOS_ReleaseSemaphore(madp_sem_input);
    MsOS_DeleteSemaphore(madp_sem_input);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    sem_destroy(&madp_sem_input);
#endif

}

static void MAdp_MFE_Sem_Wake_Up(void)
{
#if defined(_MFE_UTOPIA_)&&defined(_MIPS_PLATFORM_)
    MsOS_ReleaseSemaphore(madp_sem_input);
#elif !(defined(_MFE_T8_)&&defined(_KERNEL_MODE_))
    wake_up(&madp_sem_input);
#endif

}

MFE_U8 *my_malloc(MFE_U32* ppVAStart,unsigned int size,const char* msg)
{
    MFE_U8 *ret = (MFE_U8 *)*ppVAStart;
    ms_dprintf(ADP_L1,"MFE ADP,my_malloc[%s]: VA_START:0x%x,SIZE:0x%x\n", msg, (unsigned int)ret, size);

#ifdef MEM_CHECK
            ms_dprintf(ADP_L1,"my_malloc:Check mem set.\n");
            memset(ret, 0xFF, size);
            sleep(100000);
#endif

    *ppVAStart += size;
    return ret;
}


#if defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
int pthread_join(cyg_handle_t thread_hdl_obj, void **status)
{
    int ret, thread_timeout=0;
    //ret : 1 is success, others fail
    do {
        ret = cyg_thread_check_exit(thread_hdl_obj);
        if (ret == 1)
            break;
        thread_timeout++;
        cyg_thread_delay(10);
    } while (!ret  && (thread_timeout < 200));
    return ret;
}

void pthread_exit(int i)
{
    cyg_thread_exit();
}
#endif

#ifdef _WIN32
MFE_U8* WIN32_MMAP_FPGA_VBASE;
MFE_U8* WIN32_MMAP_MFE_VBASE;
    #define MFE_DRAM_BASE 0x00010000
    #define MAdp_MPool_Init()
    int MAdp_MPool_GetBlockVa(MFE_U32 opt, MFE_U32 *buf_start, MFE_U32 *buf_size)
    {
        *buf_start = (MFE_U32)(MFE_U8 *)malloc((*buf_size)*sizeof(MFE_U8));
            assert(buf_start);
        if (opt==E_SYS_MMAP_MFE)
            WIN32_MMAP_MFE_VBASE = (MFE_U8*)*buf_start;
//         else if (opt==E_SYS_MMAP_FPGA_POOL_BASE)
//             WIN32_MMAP_FPGA_VBASE = *buf_start;
        if ((MFE_U8*)*buf_start!=NULL)
            return 1;
        else
            return 0;
    }
    MFE_U32 MAdp_MPool_VA2PA_Win32(MFE_U32 a)
    {
        return (MFE_U32)(a) - (MFE_U32)(WIN32_MMAP_MFE_VBASE) + (MFE_U32)(MFE_DRAM_BASE);
    }
    MFE_U32 MAdp_MPool_PA2VA_Win32(MFE_U32 a)
    {
        return (MFE_U32)(a) - (MFE_U32)(MFE_DRAM_BASE) + (MFE_U32)(WIN32_MMAP_MFE_VBASE);
    }

#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&(!defined(_KERNEL_MODE_))
    #define MAdp_MPool_Init()
#elif defined(_MFE_BIG2_)
    #define MAdp_MPool_Init()
    int MAdp_MPool_GetBlockVa(int opt, MFE_U32 *buf_start, MFE_U32 *buf_size)
    {
        *buf_start = DRAM_BASE+MEM_LOGMAPPHY;//0x20000;
        return 1;
    }
#endif

#if defined(_MEM_READ_WRITE_CONTROL_)

static __inline MFE_BOOL MFE_Flush_memory( MFE_U32 u32Start_VA, MFE_U32 u32Size )
{

#if defined(_MFE_UTOPIA_)
    return MsOS_Dcache_Flush(u32Start_VA ,u32Size);
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    //LG
    return MAdp_SYS_FlushMemory();
#endif
}

#endif

#ifdef _MUXTS_

void MPEG2TS_Set_DownloadPID(MFE_U32 Video_PID, MFE_U32 Audio_PID)
{
    g_avs[0].pid = Video_PID;    // 0 means TSMUX decides it.
    g_avs[1].pid = Audio_PID;    // 0 means TSMUX decides it.
}

#ifdef _MUX_WITH_AUDIO_
void MAdp_init_audio_info(BitstreamInfo* pBsInfo, MFE_U32 nAudBitrate, MFE_U32 nSampleFreq)
{
    pBsInfo->nAudFrameSize = (nAudBitrate>>3) * SAMPLES_IN_AUDIO_FRAME / nSampleFreq;
    pBsInfo->pCur = pBsInfo->pLast = pBsInfo->pBits;
    pBsInfo->pEnd = pBsInfo->pBits + pBsInfo->nFileSize;
    pBsInfo->nFrameIndex = 0;
    pBsInfo->bPrevVOP = 0;
    ms_dprintf(ADP_L1,"Audio file size: %d %d %d %d %d %d\n", pBsInfo->nFileSize, pBsInfo->nAudFrameSize, pBsInfo->pCur[10]
        , pBsInfo->pCur[11], pBsInfo->pCur[12], pBsInfo->pCur[13]);
}

void MPEG2TS_Init_AudioData_Func(GET_AUDIO_DATA_FUNC getAudioDataCallbackFunc)
{
    MAdp_MFE_ReadAudioData = (GET_AUDIO_DATA_FUNC)getAudioDataCallbackFunc;
}
#endif

void init_tsmux_plugin(avmux_st *prec_mxint, MediaContext *pav_info)
{
    ms_dprintf(ADP_L1,"init_tsmux_plugin\n");
    prec_mxint->mctx = pav_info;
    prec_mxint->write_open = tsmux_open;
    prec_mxint->write_packet = tsmux_write_packet;
    prec_mxint->write_close = tsmux_close;
    prec_mxint->write_pause = 0;
    prec_mxint->write_resume = 0;
    // read
    prec_mxint->read = mux_read;
    prec_mxint->read_done = mux_readdone;
}

void MAdp_TSMUX_Init(MUX_TYPE MuxType, CodecID VideoType, CodecID AudioType,
                     S32 Width, S32 Height, float Framerate,
                     MFE_U8 *g_read_bbuf, IndexRingEntry *g_index_ring,
                     MFE_U32 read_bbuf_size, MFE_U32 index_ring_size)
{
    pavmux_st pavmux = &g_avmux;

    switch (MuxType) {
    case MUX_TS:
        g_av_ct.muxinfo = &ts_muxinfo;
        break;
#ifdef _EN_AVIMUX_
    case MUX_AVI:
        g_av_ct.muxinfo = &avi_muxinfo;
        break;
#endif
    default:
        assert(0);
        break;
    }

#ifdef AUDIO_MUX_DUMP
    AUDIO_DUMP = fopen("audio_dump.mp2","wb");
#endif
    // Muxer configuration
    g_avs[0].codec_type = CODEC_TYPE_VIDEO;
    g_avs[0].codec_id = (VideoType==CODEC_ID_NONE ? DEFAULT_VIDEO_ID : VideoType);
    g_avs[0].bit_rate = DEFAULT_VIDEO_BITRATE;
    g_avs[0].framerate = Framerate;
    g_avs[1].codec_type = CODEC_TYPE_AUDIO;
    g_avs[1].codec_id = (AudioType==CODEC_ID_NONE ? DEFAULT_AUDIO_ID : AudioType);
    g_avs[1].bit_rate = DEFAULT_AUDIO_BITRATE;
//    g_avs[1].framerate = Framerate;
#ifdef _EN_AVIMUX_
    if (MuxType==1) {
        g_avs[0].width = Width;
        g_avs[0].height = Height;
        g_avs[0].framerate = Framerate;

           g_avs[1].samplerate = DEFAULT_AUDIO_FREQUENCY;
    }
#endif
//    MPEG2TS_Set_DownloadPID(gDefaultVideoPID, gDefaultAudioPID);
    //
    g_av_ct.outstream = &g_outstream;
    g_av_ct.streams[0] = &g_avs[0];
    g_vid_auctx.stream_index = 0;
    g_vid_auctx.pts = g_vid_auctx.dts = DEFAULT_START_PTS;
    g_av_ct.streams[1] = &g_avs[1];
    g_aud_auctx.stream_index = 1;
    g_aud_auctx.pts = g_aud_auctx.dts = DEFAULT_START_PTS;
    //
#ifdef _MUX_WITH_AUDIO_
    g_av_ct.stream_count = 2;

#ifdef _FILE_IN_
    // Open audio file
    mux_mp2file = fopen("test.MP2", "rb");
    if (mux_mp2file==0) {
        ms_dprintf(ADP_L1,"no audio file input\n");
        return;
    }
    fseek(mux_mp2file, 0, SEEK_END);
    g_AudInfo.nFileSize = ftell(mux_mp2file);
    fseek(mux_mp2file, 0, SEEK_SET);
    g_AudInfo.pBits = my_malloc(&madp_mfe_buff_info.mfe_buf_va, 400000,"audInfo");
    //assert(ts_AudInfo.nFileSize<400000);
    fread(g_AudInfo.pBits, sizeof(MFE_U8), MUX(g_AudInfo.nFileSize<400000, g_AudInfo.nFileSize, 400000), mux_mp2file);
    MAdp_init_audio_info(&g_AudInfo, DEFAULT_AUDIO_BITRATE, DEFAULT_AUDIO_FREQUENCY);
#else
    g_AudInfo.nFileSize = 0;
#endif

    if (DEFAULT_AUDIO_ID==CODEC_ID_AUDIO_MP2)
        g_AudInfo.GetFrameBits = GetOneFrameBitsMP1A;
    else
        assert(0);

    mux_audframecount = 0;
    g_AudInfo.bIsAudio = 1;

    //ASSERT(0!=OpenMappedFile(gMuxAudioFileName, &ts_AudInfo));
#else
    g_av_ct.stream_count = 1;
    g_aud_auctx.pts = MAXVALUE_PTS;//(S64)(0x7FFFFFFFFFFFFFFFLL);
#endif
//    ts_read_bbuf = my_malloc(DOWNLOADSTREAM_BUFFER);
    assert(g_read_bbuf!=0);
    g_outstream.bbuffer = g_read_bbuf;
//    ts_outstream.bbufend_ptr = ts_outstream.bbuffer + DOWNLOADSTREAM_BUFFER;
    g_outstream.bbufend_ptr = g_outstream.bbuffer + read_bbuf_size;

    //ts_index_ring = (IndexRingEntry*)my_malloc(sizeof(IndexRingEntry)*MAX_INDEX_RING_LENGTH);
    assert(g_index_ring!=0);
    g_outstream.index_ring = g_index_ring;
    //ts_outstream.ring_size = MAX_INDEX_RING_LENGTH;
    g_outstream.ring_size = index_ring_size;
#ifdef _EN_AVIMUX_
    if (MuxType==MUX_AVI) {
        avi_muxinfo.hbuffer = (MFE_U8*)malloc(16*32);
        assert(avi_muxinfo.hbuffer);
        avi_muxinfo.hbuf_size = 16*32;
        avi_muxinfo.xbuffer = (MFE_U8*)malloc(700000);
        assert(avi_muxinfo.xbuffer);
        avi_muxinfo.xbuf_size = 700000;
    }
#endif
    switch(MuxType) {
    case MUX_TS:
        init_tsmux_plugin(pavmux, &g_av_ct);
        break;
#ifdef _EN_AVIMUX_
    case MUX_AVI:
        init_avimux_plugin(pavmux, &g_av_ct);
        pavmux->endinfo = (void*)&avi_endinfo;
        break;
#endif
    default:
        assert(0);
        break;
    }

//    ts_VidInfo.bIsAudio = 0;
//    ts_VidInfo.nFrameIndex = 0;
#ifdef _MUXTS_
    g_nAvailAudFrames = 0;
#endif
    // Initial muxer
//    mpegts_mux_start(&g_av_ct);

    pavmux->write_open(pavmux);


//init appPVR_Info
    appPVR_Info.width  = Width;
    appPVR_Info.height = Height;
    appPVR_Info.BitsBuffStart =0;
    appPVR_Info.BitsBuffSize =0;

    appPVR_Info.nDispWidth  = Width;
       appPVR_Info.nDispHeight = Height;  // Display resolution
    appPVR_Info.nBufWidth    = Width;
       appPVR_Info.nBufHeight   = Height;       // Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
/*
       // GOP
    appPVR_Info.nPbetweenI=9;
    appPVR_Info.nBbetweenP=0;
    // Rate control
    appPVR_Info.FrameRatex100 =(int) (Framerate*100);
    appPVR_Info.nBitrate = 10000000;
       appPVR_Info.nTotalFrames = 5;    // -1 means infinite. 0 is not allowed.
*/
       // GOP
    appPVR_Info.nPbetweenI=89;
    appPVR_Info.nBbetweenP=0;
    // Rate control
    appPVR_Info.FrameRatex100 =(int) (Framerate*100);
    appPVR_Info.nBitrate = 1000000;
       appPVR_Info.nTotalFrames = 5;    // -1 means infinite. 0 is not allowed.

    // MFE new settings
    if(VideoType == CODEC_ID_VIDEO_MPEG4){
        appPVR_Info.nCodecType =0;
           appPVR_Info.bInterlace     = 1;
           MAdp_MFE_Init(appPVR_Info.FrameRatex100 / 100,0,4);

       }
       else if(VideoType == CODEC_ID_VIDEO_H264 ){
           appPVR_Info.bInterlace    =0;  // interlace is no implement now.
           appPVR_Info.nCodecType =2;
           MAdp_MFE_Init(appPVR_Info.FrameRatex100 / 100,2,4);
        }
       else{
            ms_dprintf(ADP_L1,"[MUX_INIT]VideoType error!\n");
            assert(0);
        }


}


/*
    MUX algorithm
    while (video_avail || audio_avail) {
        if (video_avail && audio_avail) {
            use particular mux algorithm based on PTS difference
        } else if (video_avail) {
            mux 1 video frame
        } else if (audio_avail) {
            mux 4 audio frame
        }
    }
*/
int MAdp_TSMUX_AddFrame(MFE_U32 Vaddr, MFE_U32 Aaddr, MFE_U32 Vsize, MFE_U8 vop_type)
{
    pavmux_st        pavmux = &g_avmux;
#ifdef DEBUG_SEGMENT
    int left_size;
#endif
//    int audio_ready=1;
    int pre_audioframe = mux_audframecount;
    int audio_data_avail=0;
    boolean bHasVideoFrame = 1;
    boolean bHasAudioFrame =
#if defined(_MUX_WITH_AUDIO_)
        1;
#else
        0;
#endif
    S64 tmp = AUDIO_ONESHOT_FRAMECOUNT*1152*MPEG_TS_CLOCKRATE/DEFAULT_AUDIO_FREQUENCY;
    S64 tmp2 = g_vid_auctx.pts-g_aud_auctx.pts;

#ifdef AUDIO_MUX_DUMP
    int x=0,t;
#endif

    while (bHasVideoFrame==1 || bHasAudioFrame==1) {
//        ms_dprintf(ADP_L1,"MUX time: %d\n", cyg_current_time());
#ifdef WIN32
        ms_dprintf(ADP_L2,"vid_pts:0x%I64x  aud_pts:0x%I64x  AvailAudFrame:%d Vsize:0x%x 0x%I64x 0x%I64x %dn"
#else
        ms_dprintf(ADP_L2,"vid_pts:0x%llx  aud_pts:0x%llx  AvailAudFrame:%d Vsize:0x%x 0x%llx 0x%llx %d\n"
#endif
            , g_vid_auctx.pts, g_aud_auctx.pts,(unsigned int) g_nAvailAudFrames, (unsigned int)Vsize , tmp2 , tmp, (tmp2 > tmp) );

        if ( (tmp2 > tmp ) || (/*Vsize==0 &&*/ g_nAvailAudFrames==0)){
            g_nAvailAudFrames = AUDIO_ONESHOT_FRAMECOUNT;
            ms_dprintf(ADP_L2,"g_nAvailAudFrames = AUDIO_ONESHOT_FRAMECOUNT;\n");
        }
        if (Vsize==0){
            bHasVideoFrame = 0;
            ms_dprintf(ADP_L2,"bHasVideoFrame = 0;\n");
        }
        if (bHasVideoFrame && ( g_nAvailAudFrames==0 || bHasAudioFrame==0)) {
            mux_frame_size = Vsize;//mux_curr_pos-mux_prev_pos;
            ms_dprintf(ADP_L2,"Vsize:%d %d\n", mux_frame_size, vop_type);
            if (Madp_out_frame_count==0) // The very first frame
                g_vid_auctx.index_type = INDEX_TYPE_SEQ_I_PIC;
            else {
                switch (vop_type)
                {
                case I_VOP:
                    g_vid_auctx.index_type = INDEX_TYPE_I_PIC;    break;
                case P_VOP:
                    g_vid_auctx.index_type = INDEX_TYPE_P_PIC;    break;
                case B_VOP:
                    g_vid_auctx.index_type = INDEX_TYPE_B_PIC;    break;
                }
             }
#ifdef DEBUG_SEGMENT
            left_size = mux_frame_size;
            while (left_size>0) {
                g_vid_auctx.access_unit = (MFE_U8 *)(Vaddr + (mux_frame_size - left_size));
                g_vid_auctx.unit_size = left_size>DEBUG_SEGMENT ? DEBUG_SEGMENT : left_size;
                g_vid_auctx.pts = (S64)((/*g_VidInfo.nFrameIndex-1*/Madp_out_frame_count) * MPEG_TS_CLOCKRATE / g_avs[0].framerate) + DEFAULT_START_PTS;
                g_vid_auctx.dts = g_vid_auctx.pts;
                g_vid_auctx.first_segment = (left_size==(int)Vsize ? 1 : 0);
                g_vid_auctx.last_segment = (left_size-g_vid_auctx.unit_size==0 ? 1 : 0);
                pavmux->write_packet(pavmux, &g_vid_auctx);
                left_size -= g_vid_auctx.unit_size;
                ms_dprintf(ADP_L2,"mux video frame PTS: %lld %d %d\n", g_vid_auctx.dts, Madp_out_frame_count, g_vid_auctx.unit_size);
            }
#else
            g_vid_auctx.access_unit = (MFE_U8 *)Vaddr; //mux_frame_buf;
            g_vid_auctx.unit_size = Vsize; //mux_frame_size;
            g_vid_auctx.pts = (Madp_out_frame_count) * MPEG_TS_CLOCKRATE / g_avs[0].framerate + DEFAULT_START_PTS;
            g_vid_auctx.dts = g_vid_auctx.pts;
            g_vid_auctx.first_segment = 1;
            g_vid_auctx.last_segment = 1;
//            mpegts_mux_writeAU(pavmux, &g_vid_auctx);
            pavmux->write_packet(pavmux, &g_vid_auctx);
            ms_dprintf(ADP_L2,"access: 0x%x %x %x %x %x\n", Vaddr,(unsigned int) g_vid_auctx.access_unit[10]
                ,(unsigned int) g_vid_auctx.access_unit[11],(unsigned int) g_vid_auctx.access_unit[12], (unsigned int)g_vid_auctx.access_unit[13]);
#ifdef _WIN32
            ms_dprintf(ADP_L2,"mux video frame PTS: %I64d %d %d\n", g_vid_auctx.dts, g_vid_auctx.unit_size, Madp_out_frame_count);
#else
            ms_dprintf(ADP_L2,"mux video frame PTS: %lld %d %d\n", g_vid_auctx.dts, g_vid_auctx.unit_size, Madp_out_frame_count);
#endif
#endif
            pre_audioframe = mux_audframecount; //prevent mux too many audio, and no time to mux video
            bHasVideoFrame = 0;
            //Vsize = 0;
            //break;
        }
        else if (g_nAvailAudFrames) {
#ifdef _MUX_WITH_AUDIO_
#ifndef _FILE_IN_
            MFE_U32 AudPTS;
            MFE_U8 AudFrame;
            if (mux_audframecount>(pre_audioframe+3) && Vsize==0) {
                //prevent mux too many audio, and no time to mux video
                ms_dprintf(ADP_L2,"audio has mux 4 frames %d %d\n", mux_audframecount, g_nAvailAudFrames);
                break;
            }
            if (g_AudInfo.nFileSize==0) {
                MAdp_MFE_ReadAudioData((MFE_U32 *)(&(g_AudInfo.pBits)), &g_AudInfo.nFileSize, &AudPTS, &AudFrame);
                ms_dprintf(ADP_L1,"read AudioData from AuEnc 0x%x 0x%x %d %d\n", (MFE_U32)g_AudInfo.pBits, g_AudInfo.nFileSize, AudFrame, AudPTS);
                if(g_AudInfo.pBits) {
                    /*
                    MFE_U8 *tmp_ptr = ((MFE_U32)g_AudInfo.pBits)+20000;
                    for (ii=0; ii<1024; ii++)
                        tmp_char = tmp_ptr[ii];
                    g_AudInfo.nFileSize = 0x2400;
                    AudFrame = 16;
                      */
                    MAdp_init_audio_info(&g_AudInfo, DEFAULT_AUDIO_BITRATE, DEFAULT_AUDIO_FREQUENCY);
                    g_AudInfo.nFrameIndex = mux_audframecount;
                } else {
                    bHasAudioFrame = 0;
                    g_nAvailAudFrames = 0;
                }
                /*else if (audio_ready==1) {
                    audio_ready = 0;
                } else if (audio_ready==0)
                    bHasAudioFrame = 0;
                    */
//                ms_dprintf(ADP_L1,"read AudioData from AuEnc 0x%x 0x%x %d\n", g_AudInfo.pBits, g_AudInfo.nFileSize, AudFrame);
            }
#endif
            ms_dprintf(ADP_L2,"pBits ptr 0x%x %d\n", (MFE_U32)g_AudInfo.pBits, bHasAudioFrame);
            if (g_AudInfo.pBits) {
                audio_data_avail = 1; //for checking is there any frame muxing this time
                bHasAudioFrame = g_AudInfo.GetFrameBits(&g_AudInfo, 1);
                ms_dprintf(ADP_L2,"bHasAudioFrame:%d %d\n", bHasAudioFrame, g_AudInfo.nFileSize);

                if (bHasAudioFrame) {
                    //memcpy(mux_frame_buf, g_AudInfo.pFrameBits, g_AudInfo.nFrameSize);
                    g_aud_auctx.access_unit = g_AudInfo.pFrameBits;//mux_frame_buf;
                    g_aud_auctx.unit_size = g_AudInfo.nFrameSize;
                    g_aud_auctx.pts = (((U64)g_AudInfo.nFrameIndex-1)*1152) * MPEG_TS_CLOCKRATE / DEFAULT_AUDIO_FREQUENCY;
                    g_aud_auctx.dts = g_aud_auctx.pts;
                    g_aud_auctx.index_type = INDEX_TYPE_NONE;
                    g_aud_auctx.first_segment = 1;
                    g_aud_auctx.last_segment = 1;
//                 mpegts_mux_writeAU(pavmux, &g_aud_auctx);
                    pavmux->write_packet(pavmux, &g_aud_auctx);
#ifdef AUDIO_MUX_DUMP
                    //simple checksum
                    for(t=0;t<576;t+=32)
                        x+=(g_AudInfo.pFrameBits)[t];
                    ms_dprintf(ADP_L5,"DUMPx=%d,, 0x%x, %d\n",x, g_AudInfo.pFrameBits, g_AudInfo.nFrameSize);
                    //fwrite(g_AudInfo.pFrameBits,g_AudInfo.nFrameSize,1,AUDIO_DUMP);
#endif
                    ms_dprintf(ADP_L5,"nFileSize = %d, g_nAvailAudFrames=%d\n",g_AudInfo.nFileSize,g_nAvailAudFrames);
                    g_nAvailAudFrames--;
                    mux_audframecount++;
                    g_AudInfo.nFileSize -= g_AudInfo.nFrameSize;
            ms_dprintf(ADP_L1,"access: 0x%x %x %x %x %x\n", (MFE_U32)g_aud_auctx.access_unit, mux_audframecount
                    ,g_aud_auctx.access_unit[11], g_aud_auctx.access_unit[12], g_aud_auctx.access_unit[13]);

                    ms_dprintf(ADP_L5,"nFileSize = %d, g_nAvailAudFrames=%d\n",g_AudInfo.nFileSize,g_nAvailAudFrames);
#ifdef _WIN32
                    ms_dprintf(ADP_L2,"mux audio frame PTS: %I64d %d %d\n", g_aud_auctx.dts, g_AudInfo.nFileSize, g_AudInfo.nFrameIndex-1);
#else
            ms_dprintf(ADP_L1,"mux audio frame PTS: %lld %d %d\n", g_aud_auctx.dts, g_AudInfo.nFileSize, g_AudInfo.nFrameIndex-1);
#endif
                } else {
#ifdef _FILE_IN_
                    fread(g_AudInfo.pBits, sizeof(MFE_U8), /*ts_AudInfo.nFileSize*/400000, mux_mp2file);
#endif
                    g_nAvailAudFrames = 0;
                    ms_dprintf(ADP_L1,"no more Audio frame\n");
                }
            }
#endif
        } else {
            break;
        }
    } // Muxing loop
    if (Vsize ==0 && audio_data_avail==0)
        return -1;
    else
        return 0;
}

void MPEG2TS_GetBuffer(MFE_U8** ppBuffer, MFE_U8 **ppBufReadPtr, MFE_U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr)
{
    pavmux_st pavmux = &g_avmux;
    //mpegts_mux_read(&g_av_ct, ppBuffer, ppBufReadPtr, ppBufWritePtr, ppIndexPtr);
    pavmux->read(pavmux, ppBuffer, ppBufReadPtr, ppBufWritePtr, ppIndexPtr);
}

void MPEG2TS_GetBufferDone(MFE_U8* pNewBufReadPtr, MFE_U32 IndexRead)
{
    pavmux_st pavmux = &g_avmux;
    //mpegts_mux_readdone(&g_av_ct, pNewBufReadPtr, IndexRead);
    pavmux->read_done(pavmux, pNewBufReadPtr, IndexRead);
}
#endif //_MUXTS_

#if defined(NEW_APVR_INTERFACE)
BOOL MPEG2TS_isIFrame_Ready(MFE_U32* pts) //redcloud00@lge.com BOOLEAN -> BOOL
{
    *pts = thumb_nail.I_Frame_pts;
    if (thumb_nail.have_thumb) {
        thumb_nail.I_Frame_ptr = 0;
        ms_dprintf(ADP_L1,"Thumbnail pts: %d\n", (unsigned int)thumb_nail.I_Frame_pts);
    }
    return thumb_nail.have_thumb;
}

void MPEG2TS_Get_I_Frame(/*YUV420_START***/MFE_U32 frameBuf, MFE_U32 bufSize, MFE_U32* pts)
{
    thumb_nail.I_Frame_ptr = frameBuf;
    thumb_nail.have_thumb = 0;
    ms_dprintf(ADP_L1,"DDI call MPEG2TS_Get_I_Frame 0x%x\n", (unsigned int)frameBuf);
}

void MPEG2TS_Init_Buf_Info(MFE_U8 frameBufferCount, MFE_U16 width, MFE_U16 height) //for DIP buffer setting
{

    DIP_Total_Num = (MFE_U32)frameBufferCount;

    appPVR_Info.nDispWidth= width;
    appPVR_Info.nDispHeight= height;
    appPVR_Info.nBufWidth = width/16*16  + (width%16!=0 ? 16:0);
    appPVR_Info.nBufHeight= height/16*16 + (height%16!=0 ? 16:0);
    appPVR_Info.width  = appPVR_Info.nBufWidth;
    appPVR_Info.height = appPVR_Info.nBufHeight;

}


void MPEG2TS_Init_FrameBuf_Status_Func(GET_YUVFRAME_FUNC getBufStatusCallbackFunc,
            CLEAR_BUF_STATUS_FUNC clearBufStatusCallbackFunc)
{
    MAdp_MFE_GetDiBuffStatus = (GET_YUVFRAME_FUNC)getBufStatusCallbackFunc;
    MAdp_MFE_ClearDiBuffStatus = (CLEAR_BUF_STATUS_FUNC)clearBufStatusCallbackFunc;

}

void MPEG2TS_SetVideoBitrate(MFE_U32 bitRate)
{
    int err;
    appPVR_Info.nBitrate = bitRate;

    err = ioctl(mfe_fd, MFE_IOC_SETBITRATE, (MFE_U32)(&appPVR_Info));
    if(err==1)
        ms_dprintf(ADP_L1,"MADP SET BITRATE ERROR!!\n");

}

void MPEG2TS_Video_Encode_Enable(BOOL enable) //redcloud00@lge.com BOOLEAN -> BOOL
{
    Encode_stop = !enable;
#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
    if(Encode_stop)
        pthread_join(thread_hdl, NULL); //stephen
#else
    if(Encode_stop)
        pthread_join(thread_info, NULL); //stephen
#endif
}

void PrepareThumbNail(unsigned char voptype,unsigned long IVOP_address,MFE_U32 nFrameSize)
{
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
//     ms_dprintf(ADP_L1,"thumb_nail %d 0x%x %d %lld\n", thumb_nail.have_thumb, thumb_nail.I_Frame_ptr, voptype, g_vid_auctx.pts);
    if (thumb_nail.have_thumb==0 && thumb_nail.I_Frame_ptr!=0 && voptype==0) {
        //I frame
        //ms_dprintf(ADP_L1,"MADP IVOP_address 0x%x  0x%x  0x%x\n", thumb_nail.I_Frame_ptr, IVOP_address, (MFE_U32)MAdp_MPool_PA2KSEG1((void *)IVOP_address));
        ms_dprintf(ADP_L1,"before ConvertYuvFromTile %d\n",(int) cyg_current_time());
#if(!defined(_KERNEL_MODE_))
        memcpy((unsigned char *)thumb_nail.I_Frame_ptr, (unsigned char *)MAdp_MPool_PA2KSEG1((void *)(IVOP_address+0x10000000))
            , nFrameSize);
#else
        memcpy((unsigned char *)thumb_nail.I_Frame_ptr, (unsigned char *)MAdp_MPool_PA2KSEG1((void *)(IVOP_address))
            , nFrameSize);
#endif
        ms_dprintf(ADP_L1,"after ConvertYuvFromTile %d\n", (int) cyg_current_time());
#ifdef _MUXTS_
        thumb_nail.I_Frame_pts = g_vid_auctx.pts;
        thumb_nail.have_thumb=1;
#endif
    }
#endif
}

#endif //#if defined(NEW_APVR_INTERFACE)

int MAdp_MFE_Finish(void)
{
    int err;
    MFE_U32 temp=0;
#ifdef _MUXTS_
    pavmux_st pavmux = &g_avmux;
#endif
    Encode_stop = 1;
    err = ioctl(mfe_fd, MFE_IOC_FINISH, temp);
    ms_dprintf(ADP_L1,"MAdp_MFE_Finish\n");

#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
    MFE_U16 thread_timeout = 0;
    ms_dprintf(ADP_L1,"destroy_vo_thread\n");
    do
    {
        cyg_thread_delay(1);
        thread_timeout++;
        if (cyg_thread_check_exit(thread_hdl))
            break;
    } while (thread_timeout < 0xFFF);

    if(!cyg_thread_delete(thread_hdl))
        cyg_thread_delete(thread_hdl);
#endif

    ms_dprintf(ADP_L1,"MAdp_MFE_Finish,after pthread_mutex_destroy().\n");
#ifdef _MUXTS_
//    mpegts_mux_end(&g_av_ct);
    pavmux->write_close(pavmux);
#if defined(_FILE_IN_)&&defined(_MUX_WITH_AUDIO_)
    fclose(mux_mp2file);
#endif
#endif

#ifdef AUDIO_MUX_DUMP
    fclose(AUDIO_DUMP);
#endif

    MAdp_MFE_Sem_Destroy();
    pthread_cancel(thread_info);

    if((err=pthread_join(thread_info,NULL))!=0)
    {
        ms_dprintf(ADP_L1,"pthread_join(thread_info,NULL) Fail!!\n");
    }

#if defined(_FILE_IN_)
    fclose(mfe_file_in.infile);
    fclose(mfe_file_in.outfile);
#endif

    MAdp_MFE_Change_State(MFE_Invalid);
    MAdp_MFE_Mutex_Delete();
    return err;

}


// Buffer status
#define BUF_STATE_OK        0
#define BUF_STATE_SKIP        1    // Example: DIP in, no frame to encode.
#define BUF_STATE_STOP        2    // Example: File in, the last frame was encoded.
                                // Example: File in, read error.

#if defined(_YUVGEN_)||defined(_FILE_IN_)

// Needed for File input:
//    Convert input YUV420 frame into Block-Tile
//    Also calculate dest physical address
void ConvertYUVToTile(MEMMAP_t* pmmapDest, MEMMAP_t* pmmapSrc, int width, int height)
{
    yuv2tile(pmmapDest->miuPointer, width, height, pmmapSrc->miuPointer, 1);
    yuv2tile((MFE_U8 *)((MFE_U32)pmmapDest->miuPointer+width*height), width, height,
        (MFE_U8 *)((MFE_U32)pmmapSrc->miuPointer+width*height), 0);
    pmmapDest->miuAddress = (MFE_U32)MAdp_MPool_VA2PA((void*)(pmmapDest->miuPointer));
    MsOS_Dcache_Flush(pmmapDest->miuPointer, width*height*3/2);
}

#if defined(_FILE_IN_)
int GetFrame_FileIn(FILE_IN_INFO* pInfo, MEMMAP_t* pmmapTmp, MEMMAP_t* pmmapDst)
{
    int err;

    if (Madp_enc_frame_count>=pInfo->nTotalFramesInFile) {
        ms_dprintf(ADP_L1,"GetFrame_FileIn() Max frame encoded, no more data in file.encode stop\n");
        return BUF_STATE_STOP;
    }

    if (Madp_enc_frame_count>=appPVR_Info.nTotalFrames) {
        ms_dprintf(ADP_L1,"GetFrame_FileIn() Max frame encoded, encode stop\n");
        return BUF_STATE_STOP;
    }

    err = fread((MFE_U8 *)pmmapTmp->miuPointer, sizeof(MFE_U8), pInfo->nFrameSize, pInfo->infile);

//    if (err != pInfo->nFrameSize) {
//        ms_dprintf(ADP_L1,"GetFrame_FileIn() Cannot read frame, encode stop\n");
//        return BUF_STATE_STOP;
//    }

    ConvertYUVToTile(pmmapDst, pmmapTmp, appPVR_Info.width, appPVR_Info.height);
    return BUF_STATE_OK;
}
#endif
// YUV generation
int GetFrame_YUVGen(MFE_U8 seed, int width, int height, MEMMAP_t* pmmapTmp, MEMMAP_t* pmmapDst)
{
    if (Madp_enc_frame_count>=appPVR_Info.nTotalFrames) {
        ms_dprintf(ADP_L1,"GetFrame_YUVGen() Max frame encoded, encode stop\n");
        return BUF_STATE_STOP;
    }

    YuvGen(seed, width, height, (MFE_U8 *) pmmapTmp->miuPointer);
    ConvertYUVToTile(pmmapDst, pmmapTmp, width, height);
    MsOS_Dcache_Flush(pmmapTmp->miuPointer ,width*height*3/2);
    return BUF_STATE_OK;
}

#endif

#ifdef _SCALER_IN_
static void MFE_CleanDi_Function(MEMMAP_t* input_frame)
{
#if defined(NEW_APVR_INTERFACE) //||defined(VVOIP_INTERFACE1)
    MAdp_MFE_ClearDiBuffStatus(1,&mfe_pInfo);
#elif defined(VVOIP_INTERFACE1)
    g_CleanCallBackFunc(1,(MFE_U32)input_frame->miuAddress );
#else
    MAdp_MFE_ClearDiBuffStatus((MFE_U16)((Madp_out_frame_count-1)%DIP_Total_Num));
#endif

}

static void MFE_GetFrame_Function(MEMMAP_t* input_frame,int* nBufStatus)
{

#if defined(NEW_APVR_INTERFACE)
    input_frame->miuPointer = (unsigned char*)mfe_pInfo.Currentindex;
#elif defined(VVOIP_INTERFACE1)
    input_frame->miuPointer = (unsigned char*)VVOIP_MFE_input_VAaddr;
#else
    MAdp_MFE_GetDiBuff((MFE_U32*)(&input_frame->miuPointer));
#endif

    ms_dprintf(ADP_L2,"Video Frame: 0x%x\n",(unsigned int) input_frame->miuPointer);
    input_frame->miuAddress = (MFE_U32)MAdp_MPool_VA2PA((void*)(input_frame->miuPointer));
    ms_dprintf(ADP_L2,"Video Frame(PA): 0x%x\n",(unsigned int) input_frame->miuAddress);
    nBufStatus[0] = BUF_STATE_OK;


}

#endif //_SCALER_IN_

void MFE_Thread_Naming(void)
{
//thread naming
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
    prctl(PR_SET_NAME, (unsigned long)"mfe_thread",0,0,0);
    {// for debug
        char name[32];
        prctl(16, (unsigned long)name); //PR_GET_NAME = 16
        ms_dprintf(ADP_L2,"in MAdp_MFE_Proc, thread name = %s\n", name);
    }
#endif

}

void MAdp_MFE_Proc(void)
{
    int nBufStatus;
    MFE_U32 nDispOrder=0;    //     Driver is responsible for frame buffer re-ordering

    MEMMAP_t input_frame;
#if defined(_YUVGEN_)||defined(_FILE_IN_)
    MEMMAP_t tmp_frame;
    MFE_U32 nFrameSize;
    int src_inc = 0;
#endif

    MFE_U32 Vaddr, Vsize=0;
#ifdef _MUXTS_
    MFE_U32 Aaddr = 0; //NULL;
    int is_mux;
#endif
    volatile BITSFRAME_INFO bitsinfo = {0};


#if defined(_SCALER_IN_)||defined(_MUXTS_)
    MFE_U16 DIP_buff_status = 1;
#endif

#if defined(TIME_MEASURE)
    MFE_U32 MFE_U32ticktmp = 0;    // For TIME_MEASURE
//    MFE_U32 MFE_U32ticktmp2 = 0;
#endif
/*
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
    struct pollfd       PollFd;
    PollFd.fd=          mfe_fd;
    PollFd.events=      POLLIN;
    PollFd.revents=     0;
#endif
*/
//thread naming
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
    prctl(PR_SET_NAME, (unsigned long)"mfe_thread",0,0,0);
    {// for debug
        char name[32];
            prctl(16, (unsigned long)name); //PR_GET_NAME = 16
            ms_dprintf(ADP_L2,"in MAdp_MFE_Proc, thread name = %s\n", name);
    }
#endif

    video_shift_frame = 0;

    ms_dprintf(ADP_L2,"in MAdp_MFE_Proc");
#if defined(_YUVGEN_)||defined(_FILE_IN_)
    nFrameSize = appPVR_Info.width*appPVR_Info.height*3/2;

    {
        //input frame must 256 Align
        MFE_U32 tmp_pa = MAdp_MPool_VA2PA(madp_mfe_buff_info.mfe_buf_va);
        tmp_pa = (tmp_pa+0xFF)&~0xFF;
        madp_mfe_buff_info.mfe_buf_va = MAdp_MPool_PA2KSEG1(tmp_pa);
        input_frame.miuPointer = my_malloc(&madp_mfe_buff_info.mfe_buf_va, nFrameSize,"input_frame");
        input_frame.miuAddress = MAdp_MPool_VA2PA(input_frame.miuPointer);
        tmp_frame.miuPointer   = my_malloc(&madp_mfe_buff_info.mfe_buf_va, nFrameSize,"tmp_frame");
        tmp_frame.miuAddress = MAdp_MPool_VA2PA(tmp_frame.miuPointer);
    }
#endif

    /* Prepare the very first frame */
    ioctl(mfe_fd, MFE_IOC_GET_DISPORDER, (MFE_U32)&nDispOrder);


#if defined(_SCALER_IN_)
    //nBufStatus==BUF_STATE_STOP;
    ms_dprintf(ADP_L1,"wait DIP buffer..\n");

#if defined(VVOIP_INTERFACE1)
            //sem_wait(&madp_sem_input);
            MAdp_MFE_Change_State(MFE_Idle); //start to wait compressonepicture.
            MAdp_MFE_Sem_Wait();
            input_frame.miuPointer = (unsigned char*)VVOIP_MFE_input_VAaddr;
#else
    do {

#if defined(NEW_APVR_INTERFACE)
            MAdp_MFE_GetDiBuffStatus(&mfe_pInfo);
            DIP_buff_status = (mfe_pInfo.frame_num ==0 ? FALSE : TRUE);
            input_frame.miuPointer = (unsigned char*)mfe_pInfo.Currentindex;
#else
            DIP_buff_status = MAdp_MFE_GetDiBuffStatus();
            if (DIP_buff_status)
                MAdp_MFE_GetDiBuff((MFE_U32*)&input_frame.miuPointer);
#endif
    } while(DIP_buff_status==0);
#endif

    // debug code
    ms_dprintf(ADP_L2,"DIP buffer: 0x%x\n", (unsigned int)input_frame.miuPointer);
    ms_dprintf(ADP_L2,"debug code: 0x%x %02x %02x %02x %02x\n", (unsigned int)input_frame.miuPointer,
        ((MFE_U8 *)input_frame.miuPointer)[0], ((MFE_U8 *)input_frame.miuPointer)[1], ((MFE_U8 *)input_frame.miuPointer)[2], ((MFE_U8 *)input_frame.miuPointer)[3]);
//    ms_dprintf(ADP_L2,"debug code: 0x%x %02x %02x %02x %02x\n", (unsigned int)input_frame.miuPointer,
//        ((MFE_U8 *)input_frame.miuPointer)[nFrameSize-8+0], ((MFE_U8 *)input_frame.miuPointer)[nFrameSize-8+1], ((MFE_U8 *)input_frame.miuPointer)[nFrameSize-8+2], ((MFE_U8 *)input_frame.miuPointer)[nFrameSize-8+3]);
    ////////////

    input_frame.miuAddress = (MFE_U32)MAdp_MPool_VA2PA((void*)(input_frame.miuPointer));
    ms_dprintf(ADP_L2,"DIP VA2PA: 0x%x\n", input_frame.miuAddress);
    nBufStatus = BUF_STATE_OK;

#elif defined(_YUVGEN_)
    nBufStatus = GetFrame_YUVGen((MFE_U8)(100+src_inc*5), appPVR_Info.width, appPVR_Info.height, &tmp_frame, &input_frame);
    src_inc++;
#elif defined(_FILE_IN_)
    nBufStatus = GetFrame_FileIn(&mfe_file_in, &tmp_frame, &input_frame);
#endif //_scaler_in_


#ifdef TIME_MEASURE
        MFE_U32ticktmp = cyg_current_time();
        ms_dprintf(ADP_L4,"[Time] MADP GetFrame: %d\n", (int)MFE_U32ticktmp);
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    input_frame.miuAddress -= (input_frame.miuAddress&madp_mfe_buff_info.mfe_miu_offset);
#if defined(_MEM_READ_WRITE_CONTROL_)
    MAdp_SYS_FlushMemory();
#endif
#endif

    if (video_shift_frame < VIDEO_SHIFT) {
        enc_state = ADP_STATE_WAIT_INPUT;
        ms_dprintf(ADP_L2,"video_shift_frame %d\n", video_shift_frame);
        goto START_MUX;
    }
    ms_dprintf(ADP_L2,"video_shift_frame %d %d\n", video_shift_frame, VIDEO_SHIFT);

#ifdef TIME_MEASURE
    ms_dprintf(ADP_L4,"[Time] MADP EncodeFrame: %d\n", (unsigned int)cyg_current_time());
#endif

    ioctl(mfe_fd, MFE_IOC_ENC_ONEFRAME, input_frame.miuAddress);

    //ms_dprintf(ADP_L2,"after ioctl  MFE_IOC_ENC_ONEFRAME err= %d\n", err);
    enc_state = ADP_STATE_WAIT_GETBITS;
    Madp_enc_frame_count++;
    Vsize = 0;

    for (;;) {
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_UTOPIA_)
        if(1)
#else
        if ((poll(&PollFd, 1, -1) > 0))
#endif
        {
//            ms_dprintf(ADP_L1,"poll enc_state %d %d\n", enc_state, cyg_current_time());
            if (enc_state==ADP_STATE_WAIT_GETBITS) {
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_UTOPIA_)
/*
                {
                    MFE_HW_STATUS status;
                    while(1) {
                        ioctl(mfe_fd, MFE_IOC_CHECK_STATUS, (MFE_U32)&status);
                        if(status == GETBITS_READY)
                            break;
                        usleep(1000);
                    }
                }
*/
                ioctl(mfe_fd, MFE_IOC_GETBITS, (MFE_U32)(&bitsinfo));
#else
                //no wait at APVR mode
                ioctl(mfe_fd, MFE_IOC_GETBITS, (MFE_U32)(&bitsinfo));
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
                bitsinfo.miuAddress += madp_mfe_buff_info.mfe_miu_offset;
                MsOS_ReadMemory();
#endif
#if defined(_MEM_READ_WRITE_CONTROL_)
             //LG GP3 kernel space must call this function before get data from HW.
             MAdp_SYS_ReadMemoryRange((unsigned long) bitsinfo.miuAddress , (unsigned long)  bitsinfo.size);
#endif
                Vaddr  = (MFE_U32) MAdp_MPool_PA2KSEG1((void *)bitsinfo.miuAddress);
#if defined(_MUXTS_)||defined(VVOIP_INTERFACE1)
                if (bitsinfo.size<=0) {
                    ms_dprintf(ADP_L2,"[ADP] ERROR: BitsSize=%d<0. Data = %x %x %x %x\n", (int)bitsinfo.size,
                            ((MFE_U8*)Vaddr)[0], ((MFE_U8*)Vaddr)[1], ((MFE_U8*)Vaddr)[2], ((MFE_U8*)Vaddr)[3]);
                }
#if defined(_FPGA_)
                    // Copy from FPGA memory
                if(FPGA_MIURead(mux_frame_buf+Vsize, bitsinfo.size, bitsinfo.miuAddress, 0)==FALSE) {
                    ms_dprintf(ADP_L2, "[ADP] ERROR: Read bits from FPGA DRAM.\n");
                }
#else
                memcpy(mux_frame_buf+Vsize, (MFE_U8 *)Vaddr, bitsinfo.size);
#endif

#elif defined(_FILE_IN_)
        fwrite(Vaddr, mfe_file_in.nFrameSize , 1 , mfe_file_in.outfile);
#endif
                Vsize += bitsinfo.size;
                //assert(bitsinfo.is_more_bits == 0);
                ioctl(mfe_fd, MFE_IOC_CLEAR_BITSBUF, 1);
                ms_dprintf(ADP_L2, "[ADP] After MFE_IOC_CLEAR_BITSBUF.\n");
            };

START_MUX:
#ifdef TIME_MEASURE
            {
                MFE_U32 MFE_U32tick = cyg_current_time();
                ms_dprintf(ADP_L4,"[Time] MADP GetBitsDone: %d\n", (unsigned int)MFE_U32tick);
                ms_dprintf(ADP_L4,"%d\n", (unsigned int)(MFE_U32tick-MFE_U32ticktmp));
                if ((MFE_U32tick-MFE_U32ticktmp)>=53) {
                    ms_dprintf(ADP_L4,"performance problem1: %d\n",(unsigned int) (MFE_U32tick-MFE_U32ticktmp));
                }
            }
#endif

            if (bitsinfo.status==BITS_INFO_FRAMEDONE || enc_state==ADP_STATE_WAIT_INPUT)
            {
#if defined(VVOIP_INTERFACE1)
//                if(Vsize)
//                    g_StreamCallBackFunc(g_pCustomCtx, (MFE_S32)bitsinfo.voptype,mux_frame_buf, Vsize, 0x3);
#endif
#ifdef _MUXTS_
                is_mux = MAdp_TSMUX_AddFrame((MFE_U32)mux_frame_buf, Aaddr, Vsize, bitsinfo.voptype);
                if (((is_mux==-1)&&video_shift_frame>VIDEO_SHIFT) || (video_shift_frame<=VIDEO_SHIFT && DIP_buff_status==0)) {
                    ms_dprintf(ADP_L2,"no more data to mux\n");
//                    usleep(1000); //no more data to mux, sleep for a while
                }
                ms_dprintf(ADP_L2,"TSMUX Video 0x%x size=0x%x\n", (MFE_U32)mux_frame_buf, Vsize);
#endif
                if (Vsize) {
                    Madp_out_frame_count++;
                }
#ifndef VVOIP_INTERFACE1
                Vsize = 0;
#endif
                if (video_shift_frame>=VIDEO_SHIFT) {
                    ms_dprintf(ADP_L1,"out #%d\n", Madp_out_frame_count-1);
                }

                enc_state = ADP_STATE_WAIT_GETBITS;
#if defined(TARGET_FRAME)&&(!defined(_MUXTS_))
                if (Madp_enc_frame_count==appPVR_Info.nTotalFrames/*TARGET_FRAME*/ && Madp_out_frame_count==appPVR_Info.nTotalFrames/*TARGET_FRAME*/) {
                    Encode_stop=1;
                    ms_dprintf(ADP_L1,"DEBUG_GET, Encode_stop\n");
                    break;
                }
#else
                if (Encode_stop==1) {
#if defined(VVOIP_INTERFACE1)
                    if(Vsize) {
                        g_StreamCallBackFunc(g_pCustomCtx, (MFE_S32)bitsinfo.voptype,mux_frame_buf, Vsize, 0x3);
                        Vsize = 0;
                    }
#endif
                    ms_dprintf(ADP_L1,"Encode_stop\n");
                    break;
                }
#endif
                if (Encode_stop==0) {
                    if (enc_state==ADP_STATE_WAIT_GETBITS) {
//                PrepareThumbNail(bitsinfo.voptype,bitsinfo.IVOP_address,nFrameSize);
#ifdef _SCALER_IN_
                        if (DIP_buff_status /*|| Madp_out_frame_count==1*/) {
                            //clean di or callback-clean
                            MFE_CleanDi_Function(&input_frame);
                        }

#if defined(VVOIP_INTERFACE1)
                        ms_dprintf(ADP_L1,"MADP MFE MAIT INPUT....\n");
                        //sem_wait(&madp_sem_input);
                        MAdp_MFE_Change_State(MFE_Idle);
                        if(Vsize){
                            int tmp_size = Vsize;
                            Vsize = 0;
                            g_StreamCallBackFunc(g_pCustomCtx, (MFE_S32)bitsinfo.voptype,mux_frame_buf, tmp_size, 0x3);
                        }
                        MAdp_MFE_Sem_Wait();
                        ms_dprintf(ADP_L1,"MADP MFE MAIT INPUT....done\n");
                        if (Encode_stop==1) {
                            ms_dprintf(ADP_L1,"Encode_stop\n");
                            break;
                        }
#endif

#if defined(NEW_APVR_INTERFACE)
                        MAdp_MFE_GetDiBuffStatus(&mfe_pInfo);
                        DIP_buff_status = (mfe_pInfo.frame_num ==0 ? 0 : 1);
#elif !defined(VVOIP_INTERFACE1)
                        DIP_buff_status = MAdp_MFE_GetDiBuffStatus();
#endif

                        if (DIP_buff_status==0) {
                            ms_dprintf(ADP_L4,"DIP_buff_status==0,enc_state = ADP_STATE_WAIT_INPUT,goto START_MUX\n");
                            enc_state = ADP_STATE_WAIT_INPUT;
                            goto START_MUX;
                        }
                        else{
                            ms_dprintf(ADP_L2,"DIP_buff_status= 0x%x\n",DIP_buff_status);
                        }

                        ioctl(mfe_fd, MFE_IOC_GET_DISPORDER, (MFE_U32)&nDispOrder);
                        MFE_GetFrame_Function(&input_frame,&nBufStatus);
#elif defined(_YUVGEN_) //yuvgen()
                        ioctl(mfe_fd, MFE_IOC_GET_DISPORDER, (MFE_U32)&nDispOrder);
                nBufStatus = GetFrame_YUVGen((MFE_U8)(100+src_inc*5), appPVR_Info.width, appPVR_Info.height, &tmp_frame, &input_frame);
                src_inc++;
#elif defined(_FILE_IN_)
                        nBufStatus = GetFrame_FileIn(&mfe_file_in, &tmp_frame, &input_frame);
#endif //_SCALER_IN_

                if (nBufStatus==BUF_STATE_STOP) {
                    Encode_stop = 1;
                 break;
                }
#ifdef TIME_MEASURE
                        MFE_U32ticktmp = cyg_current_time(); // test only
                ms_dprintf(ADP_L4,"[Time] MADP EncodeFrame: %d\n",(unsigned int) MFE_U32ticktmp);
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    input_frame.miuAddress -= (input_frame.miuAddress&madp_mfe_buff_info.mfe_miu_offset);
#if defined(_MEM_READ_WRITE_CONTROL_)
    MAdp_SYS_FlushMemory();
#endif
#endif
                        video_shift_frame++;
                        if (video_shift_frame < VIDEO_SHIFT) {
                            enc_state = ADP_STATE_WAIT_INPUT;
                            ms_dprintf(ADP_L1,"video_shift_frame %d\n", video_shift_frame);
                            goto START_MUX;
                        }

                if (nBufStatus==BUF_STATE_OK)
                    ioctl(mfe_fd, MFE_IOC_ENC_ONEFRAME, input_frame.miuAddress);
//                        ms_dprintf(ADP_L1,"ENC time: %d\n", MFE_U32ticktmp);
                        Madp_enc_frame_count++;
                    }
            else {
                        ms_dprintf(ADP_L1,"enc_state==ADP_STATE_WAIT_INPUT\n");
                    }
                    //printf("after encoding on frame %d %d\n", MFE_U32Size, encode_VOP_type);
                }
            }
          else {
                //Kernel MFE driver is encoding, will return bitsbuf later
                ms_dprintf(ADP_L1,"wait frame done encoding\n");
                //printf(,"wait frame done encoding\n");
            }
        }
      else {
            ms_dprintf(ADP_L1,"MFE adp: poll return<=0\n");
        }
    }

    ms_dprintf(ADP_L1,"MFE proc pthread_exit.\n");
    pthread_exit(0); //stephen

}

MFE_BOOL MAdp_MFE_Thread_Create(void)
{

//    pthread_t thread_info;
    pthread_attr_t thrattr;
    struct sched_param thrsched;
    pthread_attr_init(&thrattr);
    pthread_attr_setdetachstate(&thrattr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setinheritsched(&thrattr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&thrattr, SCHED_FIFO);
    pthread_attr_getschedparam(&thrattr, &thrsched);
    thrsched.sched_priority = 99;//86;
    pthread_attr_setschedparam(&thrattr, &thrsched);
    pthread_attr_getschedparam(&thrattr, &thrsched);


    if (0 != pthread_create(&thread_info,
                            &thrattr,
                            (void *(*)(void *))&MAdp_MFE_Proc,
                            NULL))
    {
        ms_dprintf(ADP_L1,"MADP MFE create thread error\n");
#ifdef _MFE_UTOPIA_
        printf("MADP MFE create thread error\n");
#endif
        return FALSE;
    }

    return TRUE;

}



#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) && (!defined(_KERNEL_MODE_))

MFE_BOOL MFE_SetRIU_Base(void)
{

    MFE_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW)){
        ms_dprintf(ADP_L1,"MFE Init Err: MMIO_GetBASE failure\n");
        return FALSE;
    }

    ms_dprintf(ADP_L1,"MFE Init u32RiuBaseAdd = 0x%X, u32NonPMBankSize = 0x%X, \n", (unsigned int)u32RiuBaseAdd,(unsigned int)u32NonPMBankSize);
    //MDrv_MFE_SetOSRegBase(u32RiuBaseAdd);
     ioctl(mfe_fd, MFE_IOC_SET_RIU_BASE, u32RiuBaseAdd);

    return TRUE;

}


MFE_BOOL MApi_MFE_SetBufferInfo(MFE_S32 miu_type, MFE_U32 mfe_pa_without_offset_addr, MFE_U32 miu_offset, MFE_U32 mem_size)
{
    madp_mfe_buff_info.mfe_buf_pa_withoutoffset=0;
    madp_mfe_buff_info.mfe_miu_offset = miu_offset;
    madp_mfe_buff_info.mfe_buf_size = mem_size;
    madp_mfe_buff_info.mfe_buf_pa_withoffset = mfe_pa_without_offset_addr+miu_offset;
    madp_mfe_buff_info.mfe_buf_va = MAdp_MPool_PA2KSEG1(madp_mfe_buff_info.mfe_buf_pa_withoffset);
    madp_mfe_buff_info.mfe_buf_va_end = madp_mfe_buff_info.mfe_buf_va+mem_size;
    madp_mfe_buff_info.mfe_buf_pa_withoutoffset = mfe_pa_without_offset_addr;

//    remove set MIU function because this is depend on diff platform.
/*
    if(miu_type==0)
        eType = MIU_SELTYPE_MIU0;
    else if(miu_type==1)
        eType = MIU_SELTYPE_MIU1;
    else if(miu_type==2)
        eType = MIU_SELTYPE_MIU_ALL;
    else{
        ms_dprintf(ADP_L1,"[ERROR]MAdp_MFE_SetMIU, no supprot miu %d.\n",(int)miu_type);
        error_flag=1;
    }

    ms_dprintf(ADP_L1,"Switch to MIU%d.\n",miu_type);
    if(MDrv_MIU_SelMIU(MIU_CLIENT_MFE0_W,eType) == FALSE){
        ms_dprintf(ADP_L1,"[ERROR]MAdp_MFE_SetMIU, MDrv_MIU_SelMIU MIU_CLIENT_MFE0_W.\n");
        error_flag=1;
    }
    if(MDrv_MIU_SelMIU(MIU_CLIENT_MFE1_R,eType) == FALSE){
        ms_dprintf(ADP_L1,"[ERROR]MAdp_MFE_SetMIU, MDrv_MIU_SelMIU MIU_CLIENT_MFE1_R.\n");
        error_flag=1;
    }
*/

    return TRUE;
//    return error_flag==0 ? TRUE : FALSE;


}


#endif //#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) && (!defined(_KERNEL_MODE_))

MFE_BOOL MFE_SetMemory(void)
{

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&defined(_KERNEL_MODE_)


if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_MPOOL, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
//    if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_MAD_BASE, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
//   if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_SCALER_DNR_BUF, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
//    if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_JPG_OSD, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
//   if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_MFE, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
        ms_dprintf(ADP_L1,"Get MVDFB buffer fail.\n");
        return FALSE;
    }
/////////
     madp_mfe_buff_info.mfe_buf_pa_withoffset = (MFE_U32)MAdp_MPool_VA2PA((void*)(madp_mfe_buff_info.mfe_buf_va));
     madp_mfe_buff_info.mfe_buf_pa_withoffset += 0xE00000;
     madp_mfe_buff_info.mfe_buf_va = (MFE_U32)MAdp_MPool_PA2KSEG1((void*)(madp_mfe_buff_info.mfe_buf_pa_withoffset));
//////////////

    madp_mfe_buff_info.mfe_buf_pa_withoffset = (MFE_U32)MAdp_MPool_VA2PA((void*)(madp_mfe_buff_info.mfe_buf_va));
    ms_dprintf(ADP_L2,"Get MFEFB PA = 0x%x.\n",(unsigned int)madp_mfe_buff_info.mfe_buf_pa_withoffset);

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) //to subtract MIU1 offset. (it could be 0x8000000 or 0x10000000)
    madp_mfe_buff_info.mfe_buf_pa_withoutoffset -= (madp_mfe_buff_info.mfe_buf_pa_withoffset&madp_mfe_buff_info.mfe_miu_offset);
#endif

#elif defined(_WIN32)
    if (!MAdp_MPool_GetBlockVa((int)E_SYS_MMAP_MFE, &madp_mfe_buff_info.mfe_buf_va, &madp_mfe_buff_info.mfe_buf_size)){
        ms_dprintf(ADP_L1,"Get MVDFB buffer fail.\n");
        return FALSE;
    }
     madp_mfe_buff_info.mfe_buf_pa_withoffset = (MFE_U32)MAdp_MPool_VA2PA((void*)(madp_mfe_buff_info.mfe_buf_va));
     madp_mfe_buff_info.mfe_buf_pa_withoutoffset= madp_mfe_buff_info.mfe_buf_pa_withoffset;
#endif

    return TRUE;

}


MFE_U32 MAdp_MFE_Init(float frame_rate,int test_case_format,int test_case_num)
{

    MFE_U32 temp;
    MFE_U32 nFrameSize = appPVR_Info.nBufHeight*appPVR_Info.nBufWidth*3/2;
    MFE_U32 MallocToDrvSize ;
    appPVR_Info.nOutBuffer = 1;
    appPVR_Info.OutBufferSize = 409600;
    ms_dprintf(ADP_L1,"OutBuf=%d,size=%d\n",appPVR_Info.nOutBuffer,appPVR_Info.OutBufferSize );
    MallocToDrvSize = nFrameSize*6+appPVR_Info.OutBufferSize*appPVR_Info.nOutBuffer + 10*1024;

     MAdp_MPool_Init();

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
    //set RIU BASE
    if(MFE_SetRIU_Base() == FALSE)
        return FALSE;
#else
    madp_mfe_buff_info.mfe_miu_offset = MIU_OFFSET;
#endif

    if(MFE_SetMemory()==FALSE)
        return FALSE;

    //madp_APVRBuffPAStart be subtracted miu offset
    ms_dprintf(ADP_L1,"MFEFB Virtual start 0x%x, Physical start 0x%x, madp_mfe_buff_info.mfe_buf_size 0x%x.\n",
        (unsigned int)madp_mfe_buff_info.mfe_buf_va,(unsigned int) madp_mfe_buff_info.mfe_buf_pa_withoutoffset, (unsigned int)madp_mfe_buff_info.mfe_buf_size );

    if (0>(mfe_fd = open_dev(MFE_DEVICE, O_RDWR)))
        return FALSE;

    ioctl(mfe_fd, MFE_IOC_SET_MIU_OFFSET, madp_mfe_buff_info.mfe_miu_offset);

#if defined(_MUXTS_)||defined(VVOIP_INTERFACE1)
    mux_frame_buf = my_malloc(&madp_mfe_buff_info.mfe_buf_va, FRAM_BUF_SIZE,"mux_frame_buf");
#endif

// malloc memory for DRV level
    temp = (MFE_U32)my_malloc(&madp_mfe_buff_info.mfe_buf_va, MallocToDrvSize,"pretend to allocate for MFE driver"); //pretend to allocate for MFE
    appPVR_Info.BitsBuffSize = MallocToDrvSize;

    appPVR_Info.BitsBuffStart = (MFE_U32)MAdp_MPool_VA2PA((void*)(temp));

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
    appPVR_Info.BitsBuffStart -= (appPVR_Info.BitsBuffStart&madp_mfe_buff_info.mfe_miu_offset);
#endif

#if defined(_MFE_T8_)||defined(_FPGA_)||defined(WIN32)
    appPVR_Info.test_case_format = test_case_format;
    appPVR_Info.test_case_num = test_case_num;
#endif
    ms_dprintf(ADP_L1,"Test format = %d,Num = %d\n", appPVR_Info.test_case_format, appPVR_Info.test_case_num);
    ms_dprintf(ADP_L1,"W=%d H=%d addr=0x%x size = %ld\n", appPVR_Info.width, appPVR_Info.height, (unsigned int)appPVR_Info.BitsBuffStart,appPVR_Info.BitsBuffSize);
    ms_dprintf(ADP_L1,"Frameratex100 = %d, bitrate=%d\n", appPVR_Info.FrameRatex100, appPVR_Info.nBitrate);


    if ( ioctl(mfe_fd, MFE_IOC_INIT, (MFE_U32)(&appPVR_Info)) != 0)
        return FALSE;

    Madp_enc_frame_count = Madp_out_frame_count = Encode_stop = 0;

#ifdef NEW_APVR_INTERFACE
    thumb_nail.have_thumb = 0;
    thumb_nail.I_Frame_ptr = 0;
#endif

#if defined(_YUVGEN_)||defined(_FILE_IN_)
    ms_dprintf(ADP_L1,"TARGET_FRAME = %d\n",TARGET_FRAME);
    appPVR_Info.nTotalFrames = TARGET_FRAME ;
#endif

#if defined(_FILE_IN_)
    if(!(mfe_file_in.infile = fopen("test.yuv","rb")))
        ms_dprintf(ADP_L1,"_FILE_IN_ OpenFileIn fail!!\n");

    mfe_file_in.outfile = fopen("test_out.264","wb");
    mfe_file_in.nFrameSize = appPVR_Info.width*appPVR_Info.height*3/2;
    fseek(mfe_file_in.infile,0 , SEEK_END);
    mfe_file_in.nTotalFramesInFile = ftell( mfe_file_in.infile ) / mfe_file_in.nFrameSize;
    fseek(mfe_file_in.infile,0 , SEEK_SET);

    ms_dprintf(ADP_L1,"_FILE_IN_ nTotalFramesInFile = %d\n",mfe_file_in.nTotalFramesInFile);
#endif

    return TRUE;
}

MFE_BOOL MAdp_MFE_Encode(void)
{

    return MAdp_MFE_Thread_Create();
//    return 0;
}


#ifdef VVOIP_INTERFACE1

MFE_BOOL MApi_MFE_ResetBitrate(MFE_S32 bitrate)
{
    if(mfe_state != MFE_Idle)
        return FALSE;
    appPVR_Info.nBitrate = bitrate;

    if (ioctl(mfe_fd, MFE_IOC_RESET_BITRATE,  (MFE_U32)(&appPVR_Info))){
           ms_dprintf(ADP_L1,"MFE_IOC_RESET_BITRATE error\n");
           return FALSE;
    }

    return TRUE;

}

MFE_BOOL MApi_MFE_ResetFramerate(MFE_S32 framerate)
{
    if(mfe_state != MFE_Idle)
        return FALSE;
    appPVR_Info.FrameRatex100 = framerate*100;

    if (ioctl(mfe_fd, MFE_IOC_RESET_FRAMERATE,  (MFE_U32)(&appPVR_Info))){
           ms_dprintf(ADP_L1,"MFE_IOC_RESET_FRAMERATE error\n");
           return FALSE;
    }

    return TRUE;

}

MFE_BOOL MApi_MFE_ResetBitrateAndFramerate(MFE_S32 bitrate,MFE_S32 framerate)
{
    if(mfe_state != MFE_Idle)
        return FALSE;
    appPVR_Info.nBitrate = bitrate;
    appPVR_Info.FrameRatex100 = framerate*100;

    if (ioctl(mfe_fd, MFE_IOC_RESET_BITRATE_AND_FRAMERATE,  (MFE_U32)(&appPVR_Info))){
           ms_dprintf(ADP_L1,"MFE_IOC_RESET_BITRATE_AND_FRAMERATE error\n");
           return FALSE;
    }

    return TRUE;

}

MFE_BOOL MApi_MFE_CompressOnePicture(MFE_U32 YUVPlane, MFE_BOOL bForceIframe)
{

    if(mfe_state != MFE_Idle) {
        ms_dprintf(ADP_L1,"MApi_MFE_CompressOnePicture() return FALSE\n");
        return FALSE;
    }
    else
        MAdp_MFE_Change_State(MFE_Busy);

    VVOIP_MFE_input_VAaddr = (MFE_U32) MAdp_MPool_PA2KSEG1((void *)YUVPlane);
    ms_dprintf(ADP_L0,"MApi_MFE_CompressOnePicture()\n");

    if(bForceIframe) {
        ioctl(mfe_fd, MFE_IOC_SET_FORCEDI, (MFE_U32)NULL);
    }
    //sem_post(&madp_sem_input);
    MAdp_MFE_Sem_Wake_Up();
    return TRUE;

}

MFE_BOOL MApi_MFE_Encode(void)
{

    if(mfe_state == MFE_Init)
        MAdp_MFE_Change_State(MFE_Busy);
    else
        return FALSE;

    return MAdp_MFE_Thread_Create();

}

MFE_BOOL MApi_MFE_SetGOP(MFE_S32 PbetweenI,MFE_BOOL is_infinite)
{
    appPVR_Info.nPbetweenI = PbetweenI;
    appPVR_Info.nP_is_infinite = is_infinite;

    return TRUE;
}


MFE_BOOL MApi_MFE_Initialize(VOIP_CODEC_TYPE codec_type,MFE_U32 gDipVideoWidth,MFE_U32 gDipVideoHeight, PQ_LEVEL picture_quality,
        StreamCallback out_cb,CleanCallback mfe_CleanBufStatus, void *ctx)
{

    MFE_U32 ret;
    int bitrate = 128000;
    int frm_size = gDipVideoWidth*gDipVideoHeight;
    int test_case;

    switch (codec_type) {
    case VOIP_MPEG4:
        appPVR_Info.nCodecType = 0;
        test_case = 99;
        break;
    case VOIP_H263:
        appPVR_Info.nCodecType = 1;
        test_case = 99;
        break;
    case VOIP_H264:
        appPVR_Info.nCodecType = 2;
        test_case = 99;
        break;
    default:
        return FALSE;
    }

    appPVR_Info.nDispWidth= gDipVideoWidth;
    appPVR_Info.nDispHeight= gDipVideoHeight;
    appPVR_Info.nBufWidth = gDipVideoWidth/16*16  + (gDipVideoWidth%16!=0 ? 16:0);
    appPVR_Info.nBufHeight= gDipVideoHeight/16*16 + (gDipVideoHeight%16!=0 ? 16:0);
    appPVR_Info.width  = appPVR_Info.nBufWidth;
    appPVR_Info.height = appPVR_Info.nBufHeight;

    appPVR_Info.FrameRatex100 = 3000;
    appPVR_Info.nBbetweenP = 0;
    //appPVR_Info.nPbetweenI = 100;
    appPVR_Info.bInterlace = 0;

    if (frm_size <= 176*144) {
        switch (picture_quality) {
        case PQ_LOW:
            bitrate = 32000;
            break;
        case PQ_NORMAL:
            bitrate = 64000;
            break;
        case PQ_HIGH:
        default:
            bitrate = 96000;
            break;
        }
    }
    else if (frm_size <= 352*288) {
        switch (picture_quality) {
        case PQ_LOW:
            bitrate = 128000;
            break;
        case PQ_NORMAL:
            bitrate = 192000;
            break;
        case PQ_HIGH:
        default:
            bitrate = 256000;
            break;
        }
    }
    //else if (frm_size <= 704*576) {
    else if (frm_size <= 720*576) {
        switch (picture_quality) {
        case PQ_LOW:
            bitrate = 512000;
            break;
        case PQ_NORMAL:
            bitrate = 1024000;
            break;
        case PQ_HIGH:
        default:
            bitrate = 1536000;
            break;
        }
    }
    else if (frm_size <= 960*576) {
        switch (picture_quality) {
        case PQ_LOW:
            bitrate = 1024000;
            break;
        case PQ_NORMAL:
            bitrate = 1536000;
            break;
        case PQ_HIGH:
        default:
            bitrate = 2048000;
            break;
        }
    }
    else {
        return FALSE;
    }

    appPVR_Info.nBitrate= bitrate;

    DIP_Total_Num = 1;
    g_pCustomCtx = ctx;
    g_StreamCallBackFunc = out_cb;
    g_CleanCallBackFunc = mfe_CleanBufStatus;

    MAdp_MFE_Sem_Init();
    MAdp_MFE_Mutex_Init();
    ret = MAdp_MFE_Init((float)appPVR_Info.FrameRatex100 / 100,appPVR_Info.nCodecType,test_case);

    ms_dprintf(ADP_L1," After msAPI_MFE_Initialize\n");
    ms_dprintf(ADP_L1,"BUF:%dx%d, bitrate=%d,fps=%f \n",appPVR_Info.width,appPVR_Info.height,appPVR_Info.nBitrate,(float)appPVR_Info.FrameRatex100 / 100);
    ms_dprintf(ADP_L1,"DIS:%dx%d\n",appPVR_Info.nDispWidth,appPVR_Info.nDispHeight);

    if((MFE_BOOL)ret == FALSE)
        return FALSE;

    return MAdp_MFE_Change_State(MFE_Init);


}

MFE_BOOL MApi_MFE_DeInitialize(void)
{

    MAdp_MFE_Change_State(MFE_DeInit);
    return MAdp_MFE_Finish() == 0 ? TRUE : FALSE;

}

MFE_STATE MApi_MFE_GetState(void)
{
    return mfe_state;
}

MFE_BOOL MApi_MFE_GetVOL(MFE_S8* Buf)
{

    if(mfe_state != MFE_Idle)
        return FALSE;

    ioctl(mfe_fd, MFE_IOC_GET_VOL, (MFE_U32)Buf);

  //GETVOL test codes
/*
    size = (Buf[1]<<8)|Buf[0];

    ms_dprintf(ADP_L1, "[MFE_IOC_GET_VOL] size = %d\n", size);
    for (i=0; i<size; i++) {
        ms_dprintf(ADP_L1, "%02x ", Buf[i+2]);
        if ((i+1)%16 == 0)
            ms_dprintf(ADP_L1, "\n");
    }
*/

    return TRUE;
}

#endif




