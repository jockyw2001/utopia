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
#ifndef _MADP_MFE_H_
#define _MADP_MFE_H_

#include "MFE_chip.h"
#if defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
#include "mdrv_mfe_io.h" //include define: _MFE_T8_ _MIPS_PLATFORM_...etc
#endif
#include "mdrv_mfe_st.h"



#ifdef __cplusplus
extern "C"
{
#endif

//input method: _SCALER_IN_, _FILE_IN_, _YUVGEN_, and _MUXTS_

#define _SCALER_IN_

// #define _FILE_IN_
//#define _AUDIO_FILE_IN_
//#define _YUVGEN_

//#define _MUXTS_
//#define _MUX_WITH_AUDIO_

//#define NEW_APVR_INTERFACE

#define VVOIP_INTERFACE1
//#define AUDIO_MUX_DUMP

//#define _EN_ADP_CRC_ //for testing
//#define CRC_Accumulate	//acc CRC value for overnight testing
//#define DEBUG_GET
//#define TIME_MEASURE

#define MFE_MIU 0 //kernel mode

#if defined(_YUVGEN_)||defined(_FILE_IN_)
	#define TARGET_FRAME 10
#endif

#define FRAM_BUF_SIZE 512*1024 //output bsp buffer

typedef struct _MFE_INPUT_BUF_INFO {
    MFE_U32 mfe_miu_offset;
    MFE_U32 mfe_buf_pa_withoffset;
    MFE_U32 mfe_buf_pa_withoutoffset;
    MFE_U32 mfe_buf_size;
    MFE_U32 mfe_buf_va;
    MFE_U32 mfe_buf_va_end;
} MFE_INPUT_BUF_INFO;


#if defined(_YUVGEN_)||defined(_FILE_IN_)
// File IN
typedef struct _FILE_IN_INFO {
	FILE *infile;
	FILE *outfile;
	int nTotalFramesInFile;	// How many frames in the specified file.
	int nFrameSize;
} FILE_IN_INFO;
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)
	#include "MuxCommon.h"
	#if defined(_KERNEL_MODE_)
//		#define E_SYS_MMAP_MFE E_SYS_MMAP_M4VE
		#include "linux/types.h"
		#define open_dev open
             //#define MIU_OFFSET 0x0000000000
            #if MFE_MIU==0
                #define MIU_OFFSET 0x0000000000
            #else
                #define MIU_OFFSET 0x0010000000
            #endif

	#else
		#include "MsTypes.h"
		#define E_SYS_MMAP_MFE 0x02010000//0
		#define MAdp_MPool_VA2PA(a) MS_VA2PA((MS_U32)(a))
		#define MAdp_MPool_PA2KSEG1(a) MS_PA2KSEG1((MS_U32)(a))
		#define open_dev(a, b) 1
             #define MIU_OFFSET 0x18000000
	#endif

#else
    #define open_dev(a, b) 1
	#include "memmap.h"
	#define E_SYS_MMAP_MFE 0
       #define MIU_OFFSET 0x0000000000
	#ifdef WIN32
		#include <windows.h>
		#define usleep(time) Sleep(time)
		#define MAdp_MPool_VA2PA(a) MAdp_MPool_VA2PA_Win32((U32)(a))
		#define MAdp_MPool_PA2KSEG1(a) MAdp_MPool_PA2VA_Win32((U32)(a))
	#else
		#ifndef BOOLEAN
			#define BOOLEAN bool
		#endif
		#define MAdp_MPool_VA2PA(a) (((U32)(a))-MEM_LOGMAPPHY)
		#define MAdp_MPool_PA2KSEG1(a) (((U32)(a))+MEM_LOGMAPPHY)
	#endif
#endif //defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)

#ifdef _MUXTS_
#include "TsMuxer.h"
#endif

#if defined(_MFE_BIG2_)&&defined(_MIPS_PLATFORM_)
//#define printf          diag_printf
#define usleep(time)    cyg_thread_delay(time)
#endif

#ifndef MUX
#define MUX(a, b, c) ((a) ? (b) : (c))
#endif


#if defined(VVOIP_INTERFACE1)
    #include "apiVOIPMFE.h"
#else
#define I_VOP        0        /* vop coding modes */
#define P_VOP        1
#define B_VOP        2

#if defined(NEW_APVR_INTERFACE)

typedef struct {
	U32 Currentindex;		// range = [0, DIP_DI_BUFFER_COUNT-1], MFE read index.
	int frame_num;	// number of frames in DI buffer.
} VENC_INFRAME_INFO;


typedef VENC_INFRAME_INFO* VENC_INFRAME_INFO_PTR;
typedef void (*GET_YUVFRAME_FUNC) (void* ppInfo);
typedef void (*CLEAR_BUF_STATUS_FUNC) (U16 nFrameCoded,void* ppInfo);
typedef void (*GET_AUDIO_DATA_FUNC) (U32 *AudAddr, U32 *AudSize, U32 *AudPTS, U8 *AudFrameCnt);

#endif


MFE_U32 MAdp_MFE_Init(float frame_rate,int test_case_format,int test_case_num);
MFE_BOOL MAdp_MFE_Encode(void/*U8 frc*/);
int MAdp_MFE_Finish(void);


#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
#define MAdp_MPool_Init()
int MAdp_MPool_GetBlockVa(MFE_U32 opt, MFE_U32 *buf_start, MFE_U32 *buf_size);
#endif

//set memory addr and size, miu0 or miu1.
#if defined(_MFE_T8_)&& (!defined(_KERNEL_MODE_))
int MAdp_MFE_SetBuffInfo(int miu_type,unsigned int mfe_pa_without_offset_addr,unsigned int miu_offset,unsigned int mem_size);
#endif



#ifdef _MUXTS_
#ifdef DEBUG_READ
  void MPEG2TS_GetBuffer(U8** ppBuffer, U32* pSize, U32 readall);
  void MPEG2TS_GetBufferDone(U8* pBuffer);
#else
  void MPEG2TS_GetBuffer(U8** ppBuffer, U8 **ppBufReadPtr, U8 **ppBufWritePtr, IndexRingEntry **ppIndexPtr);
  void MPEG2TS_GetBufferDone(U8* pNewBufReadPtr, U32 IndexRead);
#endif
void MAdp_TSMUX_Init(MUX_TYPE MuxType, CodecID VideoType, CodecID AudioType,
					 S32 Width, S32 Height, float Framerate,
					 U8 *g_read_bbuf, IndexRingEntry *g_index_ring,
					 U32 read_bbuf_size, U32 index_ring_size);
void MPEG2TS_Set_DownloadPID(U32 VideoPID, U32 AudioPID);
// TSMUX will call function provide by APVR adaptation layer instead of calling audio driver directly
void MPEG2TS_Init_AudioData_Func(GET_AUDIO_DATA_FUNC getAudioDataCallbackFunc);

// Related DDI API: DDI_PVR_SetEncodingBitRate
// Bitrates to map into PVR_ENCODING_BITRATE_T: 6000000, 5000000, 400000, 3000000, 2000000
void MPEG2TS_SetVideoBitrate(U32 bitRate);

// start/stop MPEG2TS file encoding
// Related DDI API: DDI_PVR_StartDownload, DDI_PVR_StopDownload
void MPEG2TS_Video_Encode_Enable(BOOL enable);

// get current I frame buffer information, it will be used to generate thumbnail
// Related DDI API: DDI_PVR_GetThumbnail
void MPEG2TS_Get_I_Frame(/*YUV420_START***/U32 frameBuf, U32 bufSize, U32* pts);
BOOL MPEG2TS_isIFrame_Ready(U32* pts);
void MPEG2TS_Init_Buf_Info(U8 frameBufferCount, U16 width, U16 height);

#endif


#if defined(NEW_APVR_INTERFACE)
void MPEG2TS_Init_FrameBuf_Status_Func(GET_YUVFRAME_FUNC getBufStatusCallbackFunc,
            CLEAR_BUF_STATUS_FUNC clearBufStatusCallbackFunc);
#endif


//this function can write data to DRAM. We can use trace32:"data.save" to get data from FPGA to PC.
                                              //data pointer, offset of 0x82C00000(physical DRAM addr,size(bytes) )
void MADP_MFE_WRITE(unsigned char* data,unsigned int offset,unsigned int size);


#endif



#ifdef __cplusplus
}
#endif

#endif

