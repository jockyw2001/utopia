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

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#else
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvSYS.h"
#include "halSYS.h"

// Internal Definition
#include "drvAUDIO.h"
#include "regCHIP.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halSOUND.h"
#include "halAUR2.h"
#include "audio_comm2.h"

#include "halAUDSP.h"

#if defined(MSOS_TYPE_NUTTX)
extern int lib_lowprintf(const char *format, ...);
#define DBG_PREFIX lib_lowprintf
#else
#define DBG_PREFIX printf
#endif

#if (defined ANDROID)
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#ifndef LOGI // android 4.1 rename LOGx to ALOGx
#define LOGI ALOGI
#endif

#ifndef LOGE // android 4.1 rename LOGx to ALOGx
#define LOGE ALOGE
#endif

#define HALAUDIO_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
#define HALAUDIO_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)
#else
    #ifdef MBOOT_PLAY_MELODY
        #define HALAUDIO_PRINT(fmt, args...)
        #define HALAUDIO_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #else
        #define HALAUDIO_PRINT(fmt, args...)  DBG_PREFIX("[[utopia]]      " fmt, ## args)
        #define HALAUDIO_ERROR(fmt, args...)  DBG_PREFIX("[[utopia]]      " fmt, ## args)
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_AUDIO(args...)       //DBG_PREFIX(args)
#define DBG_AUDIO_LOAD(args...)  //DBG_PREFIX(args)
#define DBG_AUDIO_ERROR(args...) //DBG_PREFIX(args)

#define AUD_ERRMSG(args...)  DBG_PREFIX(args)
#define AUD_DBGMSG(args...)  DBG_PREFIX(args)

#define HALAUDIO_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALAUDIO_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALAUDIO_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

#ifndef MSOS_TYPE_NOS
void* MDrv_MPool_PA2KSEG1(void* pAddrPhys);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_HAL_ERR(x, args...)        //{printf(x, ##args);}
#define HINIBBLE(u8Data)    ((u8Data) >> 4)
#define LONIBBLE(u8Data)    ((u8Data) << 4)
#if (AUDIO_R2_DMA_READER1_SUPPORT || AUDIO_R2_DMA_READER2_SUPPORT)
#define AUDIO_R2_DMA_RDER_CFG_REG(base, offset)    ((MS_U32)(base + (offset * 2)))
#endif

#define HW_DMA_RDR1_BUF_UNIT    4096

#if AUDIO_HW_DMA_READER1_SUPPORT
#define HW_DMA_RDR1_BUF_ADDR    (DSP2_DMA_READER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_RDR1_BUF_SIZE    ((DSP2_DMA_READER_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
#define HW_DMA_RDR2_BUF_ADDR    (DSP2_HW_DMA_READER2_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_RDR2_BUF_SIZE    ((DSP2_HW_DMA_READER2_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
#define SW_DMA_RDR1_BUF_ADDR    (DSP2_SW_DMA_READER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define SW_DMA_RDR1_BUF_SIZE    ((DSP2_SW_DMA_READER_DRAM_SIZE + 1) * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
#define R2_DMA_RDR1_BUF_ADDR    (ASND_DSP_DDR_SIZE + OFFSET_MCU_R2_DMA1_READER_DRAM_BASE)
#define R2_DMA_RDR1_BUF_SIZE    MCU_R2_DMA1_READER_DRAM_SIZE
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
#define R2_DMA_RDR2_BUF_ADDR    (ASND_DSP_DDR_SIZE + OFFSET_MCU_R2_DMA2_READER_DRAM_BASE)
#define R2_DMA_RDR2_BUF_SIZE    MCU_R2_DMA2_READER_DRAM_SIZE
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
#define PCM_CAPTURE1_BUF_UNIT    128
#define PCM_CAPTURE1_BUF_ADDR    (DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define PCM_CAPTURE1_BUF_SIZE    (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
#define PCM_CAPTURE2_BUF_UNIT    128
#define PCM_CAPTURE2_BUF_ADDR    (DSP2_PCM_CAPTURE2_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define PCM_CAPTURE2_BUF_SIZE    (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
#define HW_DMA_WTR1_BUF_ADDR    (DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE * BYTES_IN_MIU_LINE)
#define HW_DMA_WTR1_BUF_SIZE    ((DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE) * BYTES_IN_MIU_LINE)
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define StereoAACOutputPCM 0
#define HW_AutoDepop 1

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
static MS_U8 SIF_MailBoxArray[12];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
AUDIO_SOURCE_INFO_TYPE AudioInputSrcInfo;
AUDIO_INPUT_TYPE       g_audioSrcType = AUDIO_DSP1_DVB_INPUT;
AUDIO_INPUT_TYPE       g_audioSubSrcType = AUDIO_DSP1_DVB_INPUT;
MS_U16                  g_BalanceMask = 0x0000;
MS_BOOL                g_bAudio_loadcode_from_dram = 0, g_bIsDTV;
MS_VIRT                 _gMIO_MapBase;
MS_S32 _s32AUDIOMutex = -1;
MS_S32  _s32AUDIOMutexReboot = -1;
MS_S32  _s32MutexLoadCode = -1;
MS_S32  _s32AUDIOMutexIDMA = -1;
MS_U8 CheckPlayDoneCnt = 0;
extern MS_BOOL g_bDSPLoadCode;
MS_U32              g_u32bDTSCD = 0x00;
MS_BOOL bReBoot_All_Aud_Proc = FALSE;

extern AUDIO_SHARED_VARS2    * g_AudioVars2;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#else
AUDIO_SHARED_VARS2      g_audioShared;
#endif

#ifndef MSOS_TYPE_NUTTX
static MS_BOOL g_bAudioTeeInfoShmInitFlag = FALSE;
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
#else
AUDIO_TEE_INFO_SHARE_MEM   gAudioTeeInfoShm;
#endif
AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm = NULL;

MS_BOOL g_bAudioTeeEnabled = FALSE, g_bAudioTeeMbxInitialized = FALSE;
MS_BOOL g_bDecDspSecureTeeAccessed = FALSE, g_bSndDspSecureTeeAccessed = FALSE;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 curr_OS_Time=0;
static MS_S64   curr_PTS = 0;
static MS_BOOL  g_bInitShmFlag = FALSE;
static MS_BOOL  g_bEncodeDoneFlag = 0;
static MS_U8  * dma_writePtr;
static MS_U8  * dma_bufStartAddress;
static MS_U8  * dma_bufEndAddress;
static MS_U8    Dvb2DecCmd_tmp;
static AUDIO_DEC_ID DTVDecPriority[AU_DEC_MAX] = {AU_DEC_ID1, AU_DEC_ID3 , AU_DEC_INVALID};
static AUDIO_DEC_ID HDMIDecPriority[AU_DEC_MAX] = {AU_DEC_ID1, AU_DEC_INVALID , AU_DEC_INVALID};
static AUDIO_DEC_ID MMDecPriority[AU_DEC_MAX] = {AU_DEC_ID1, AU_DEC_INVALID , AU_DEC_INVALID};

#ifdef MSOS_TYPE_NUTTX
static MS_S32  _s32MadEventId = -1;
static MS_S32  _s32MadTaskId = -1;
static MBX_Msg TeeToReeMbxAckMsg;
static MBX_Msg msg;
static void _MAD_Proc(void);
static void HAL_AUDIO_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue);
#endif

MS_U32 g_pcm_capture_base_addr = 0;
MS_U32 g_pcm_capture2_base_addr = 0;

MS_U8 g_DSPMadMIUBank[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPMadBaseBufferAdr[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
MS_U32 g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2] = {0};
extern MS_U32 (* gGetDDRInfoFuncPtr)(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

extern MS_U32  g_AudioDumpInfoCtrl;
extern MS_BOOL bIsNonPCMInDec2;
extern MS_U32 EncFrameIdx;
extern MS_U32 EncBuf_W_idx;
extern MS_U8   EncBuf_Count;
extern AU_DVB_ENC_FRAME_INFO MPEG_EN_BUF[6];

extern AUDIO_DEC_ID (* gOpenDecodeSystemFuncPtr)(AudioDecStatus_t * p_AudioDecStatus);
extern MS_BOOL (*pFuncPtr_Setsystem)(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus);
extern MS_BOOL (* gDigitalOutChannelStatusFuncPtr)(DIGITAL_OUTPUT_TYPE , Digital_Out_Channel_Status_t *);
extern MS_U32 (* gGetDDRInfoFuncPtr)(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);


const MS_U32 u32PathArray[8]={0x2C64, 0x2C66, 0x2C68, 0x2C6A, 0x2C65, 0x2C67, 0x2C69, 0x2C6B};

//Refine power on sequence for earphone & DAC pop noise issue
const AUDIO_REG_TYPE  AudioPreInitTbl_Clock_0[] =
{

    //----------------pre-AUPLL-------------------------//Item 23
    {0x112c9e, 0xff, 0x20}, //[15:0]  2nd order synthesizer n.f[23:8]
    {0x112c9f, 0xff, 0x1c},
    {0x112c9c, 0xff, 0x00}, //[7:0]   2nd order synthesizer n.f[7:0]
    {0x112c9d, 0xff, 0xc0}, //[15]    2nd order synthesizer enable, [14] load
    {0x112ca0, 0xff, 0x20}, //[15:0]  codec synthesizer n.f << 112ca2[4] do not select codec synthesizer 1024FS clk
    {0x112ca1, 0xff, 0x1c},
    {0x112c9c, 0xff, 0x00},
    {0x112c9d, 0xff, 0x80}, //[14]    2nd order synthesizer load
    {0x112ca2, 0x08, 0x08}, //[3]     Enable reference clock
    {0x112ca2, 0x10, 0x10}, //[4]     Select 1st-order or 2nd-order synthesizer
    //----------------AUPLL control-------------------------//Item 23
    {0x112c76, 0xff, 0x00},
    {0x112c77, 0xff, 0x03},

    //----------------AUR2PLL------------------------//Item 22
    {0x160342, 0xff, 0x01}, //
    {0x160343, 0xff, 0x00}, //[8]     Set AUR2PLL power down = 0

    //---------------- Enable CLK----------------------------//Item 32
    {0x112ca4, 0xff, 0xff}, //[10:0]  CLKGEN's clock enable
    {0x112ca5, 0xff, 0x1f},
    {0x112cb2, 0x90, 0x90}, //[7]     CLK R2 EN[3], [4] CLK PARSER EN
    {0x112cb3, 0x60, 0x60}, //[14:13] CLK R2 EN[2:1]
    {0x112ccb, 0x3c, 0x3c}, //[13:10] CLK DSP EN[4:1]

    //----------------- DFS ---------------------------//Item 36
    {0x163c61, 0x03, 0x00}, //[9:8]   CLK DSP DFS SEL[1:0] = AUPLL
    {0x163c61, 0x10, 0x00}, //[12]    CLK DSP DFS SEL[2] = 0
    {0x163c61, 0x08, 0x08}, //[11]    CLK DSP DFS EN
    {0x163c60, 0x1f, 0x1f}, //[4:0]   DSP DFS DIV
    {0x163c60, 0x40, 0x40}, //[6]     DSP DFS EN
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x80}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c63, 0x03, 0x00}, //[9:8]   CLK R2 DFS SEL[1:0] = AUR2PLL
    {0x163c63, 0x10, 0x00}, //[12]    CLK R2 DFS SEL[2] = 0
    {0x163c63, 0x08, 0x08}, //[11]    CLK R2 DFS EN
    {0x163c62, 0x1f, 0x1f}, //[4:0]   R2 DFS DIV
    {0x163c62, 0x40, 0x40}, //[6]     R2 DFS EN
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x80}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE


    {0xffffff, 0x00, 0x00}, //        end of table
};


const AUDIO_REG_TYPE  AudioPreInitTbl_Clock_1[] =
{
    //----------------Audio Reset---------------------------//Item 24
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x0f}, //[1]       CLKGEN_RESET
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x0d},
    {0x112c01, 0xff, 0x7f},
    {0x112c00, 0xff, 0x00},
    {0x112c01, 0xff, 0x00},

    //-----------------Synthesizer--------------------------//Item 29
    {0x112bcf, 0x01, 0x01}, // DVB1
    {0x112bce, 0x01, 0x01},
    {0x112ba8, 0xff, 0x94},
    {0x112ba9, 0xff, 0x11},
    {0x112baa, 0xff, 0x00},
    {0x112bab, 0xff, 0x00},
    {0x112c60, 0x80, 0x80},
    {0x112bce, 0x01, 0x00},

    {0x112bcf, 0x02, 0x02}, // DVB2
    {0x112bce, 0x02, 0x02},
    {0x112bac, 0xff, 0x94},
    {0x112bad, 0xff, 0x11},
    {0x112bae, 0xff, 0x00},
    {0x112baf, 0xff, 0x00},
    {0x112c62, 0x80, 0x80},
    {0x112bce, 0x02, 0x00},

    {0x112bcf, 0x04, 0x04}, // DVB3
    {0x112bce, 0x04, 0x04},
    {0x112bb0, 0xff, 0x94},
    {0x112bb1, 0xff, 0x11},
    {0x112bb2, 0xff, 0x00},
    {0x112bb3, 0xff, 0x00},
    {0x112c61, 0x80, 0x80},
    {0x112bce, 0x04, 0x00},

    {0x112bcf, 0x08, 0x08}, // DVB4
    {0x112bce, 0x08, 0x08},
    {0x112bb4, 0xff, 0x94},
    {0x112bb5, 0xff, 0x11},
    {0x112bb6, 0xff, 0x00},
    {0x112bb7, 0xff, 0x00},
    {0x112c63, 0x80, 0x80},
    {0x112bce, 0x08, 0x00},

    {0x112bcf, 0x10, 0x10}, // NonPCM
    {0x112bce, 0x20, 0x20},
    {0x112bb8, 0xff, 0x94},
    {0x112bb9, 0xff, 0x11},
    {0x112bba, 0xff, 0x00},
    {0x112bbb, 0xff, 0x00},
    {0x112c8b, 0x40, 0x40},
    {0x112bce, 0x20, 0x00},

    {0x112bcf, 0x20, 0x20}, // NonPCM2
    {0x112bce, 0x10, 0x10},
    {0x112bd0, 0xff, 0x94},
    {0x112bd1, 0xff, 0x11},
    {0x112bd2, 0xff, 0x00},
    {0x112bd3, 0xff, 0x00},
    {0x112c5f, 0x40, 0x40},
    {0x112bce, 0x10, 0x00},

    {0x112bcf, 0x40, 0x40}, // NonPCM3
    {0x112bcf, 0x80, 0x80},
    {0x112bd6, 0xff, 0x94},
    {0x112bd7, 0xff, 0x11},
    {0x112bd8, 0xff, 0x00},
    {0x112bd9, 0xff, 0x00},
    {0x112c48, 0x80, 0x80},
    {0x112bcf, 0x80, 0x00},

    {0x112c25, 0x20, 0x20}, // DVB5
    {0x112c25, 0x10, 0x10},
    {0x112c26, 0xff, 0x94},
    {0x112c27, 0xff, 0x11},
    {0x112c28, 0xff, 0x00},
    {0x112c29, 0xff, 0x00},
    {0x112c25, 0x80, 0x80},
    {0x112c25, 0x10, 0x00},

    {0x112c25, 0x02, 0x02}, // DVB6
    {0x112c25, 0x01, 0x01},
    {0x112c2a, 0xff, 0x94},
    {0x112c2b, 0xff, 0x11},
    {0x112c2c, 0xff, 0x00},
    {0x112c2d, 0xff, 0x00},
    {0x112c25, 0x08, 0x08},
    {0x112c25, 0x01, 0x00},

    {0x112bcf, 0x01, 0x00}, // DVB1    is controlled by DSP/R2
    {0x112bcf, 0x02, 0x00}, // DVB2    is controlled by DSP/R2
    {0x112bcf, 0x04, 0x00}, // DVB3    is controlled by DSP/R2
    {0x112bcf, 0x08, 0x00}, // DVB4    is controlled by DSP/R2
    {0x112c25, 0x20, 0x00}, // DVB5    is controlled by DSP/R2
    {0x112c25, 0x02, 0x00}, // DVB6    is controlled by DSP/R2

    {0x112bcf, 0x10, 0x00}, // NonPCM1 is controlled by DSP/R2
    {0x112bcf, 0x20, 0x00}, // NonPCM2 is controlled by DSP/R2
    {0x112bcf, 0x40, 0x00}, // NonPCM3 is controlled by DSP/R2

    //---------------- AudioBand ROM----------------------------//Item 35
    //additional 8 SRC needed

    //-----------------AudioBand----------------------------//Item 21
    {0x112b58, 0xff, 0xff}, //[15:0]  DWA fix value
    {0x112b59, 0xff, 0x01}, //[15:0]  DWA fix value
    {0x112b55, 0xf0, 0xf0}, //[15:12] DWA fix enable
    {0x112b46, 0xff, 0x00}, //[15:0]  Enable Group C
    {0x112b47, 0xff, 0xff},
    {0x112b56, 0xff, 0x30}, //[15:0]  Enable ADC1/2
    {0x112b57, 0xff, 0x00},
    {0x112c6e, 0xff, 0x88}, //[7:0]   Enable Grp A/B
    {0x112b50, 0xff, 0x80}, //[15:0]  DC offset value
    {0x112b51, 0xff, 0x00},
    {0x112b10, 0xff, 0x80}, //[15:0]  DC offset value for earphone
    {0x112b60, 0xff, 0xc0},
    {0x112b61, 0xff, 0x38},
    {0x112b62, 0xff, 0x38},
    {0x112b63, 0xff, 0x38},
    {0x112b40, 0xff, 0x4e}, //[15:0]  Init CODEC SRC
    {0x112b41, 0xff, 0x00},
    {0x112b54, 0xff, 0x00}, //[15:0]  DITHER select 0.5LSB
    {0x112b55, 0xff, 0x01},
    {0x112b52, 0xff, 0x0f}, //[15:0]  DC offset ENABLE=0, Dither ENABLE=0
    {0x112b53, 0xff, 0xf0},
    {0x112b40, 0xff, 0x0c}, //[15:0]  Init CODEC SRC
    {0x112b41, 0xff, 0x00},
    {0x112b54, 0x01, 0x01}, //[0]     SDM EN

   //-----------------De-POP_1-----------------------------//Item 25
#if (HW_AutoDepop == 1) // auto depop
    {0x103346, 0x0f, 0x00}, // [0] Auto-depop clock gate
    {0x112e70, 0xff, 0x15}, //wriu -w 0x112e70 0xDB15 //Value 0
    {0x112e71, 0xff, 0xdb},
    {0x112e72, 0xff, 0x01}, //wriu -w 0x112e72 0x0001 //
    {0x112e73, 0xff, 0x00},
    {0x112ec0, 0xff, 0x15}, //wriu -w 0x112ec0 0xC015 //1
    {0x112ec1, 0xff, 0xc0},
    {0x112ec2, 0xff, 0x00}, //wriu -w 0x112ec2 0x0000 //
    {0x112ec3, 0xff, 0x00},
    {0x112ec4, 0xff, 0x51}, //wriu -w 0x112ec4 0xA051 //2
    {0x112ec5, 0xff, 0xa0},
    {0x112ec6, 0xff, 0x00}, //wriu -w 0x112ec6 0x0000 //
    {0x112ec7, 0xff, 0x00},
    {0x112ec8, 0xff, 0x59}, //wriu -w 0x112ec8 0xA059 //3
    {0x112ec9, 0xff, 0xa0},
    {0x112eca, 0xff, 0x00}, //wriu -w 0x112eca 0x0000 //
    {0x112ecb, 0xff, 0x00},
    {0x112ecc, 0xff, 0x5b}, //wriu -w 0x112ecc 0xA05B //4
    {0x112ecd, 0xff, 0xa0},
    {0x112ece, 0xff, 0x00}, //wriu -w 0x112ece 0x0000 //
    {0x112ecf, 0xff, 0x00},
    {0x112ed0, 0xff, 0xcb}, //wriu -w 0x112ed0 0xA0CB //5
    {0x112ed1, 0xff, 0xa0},
    {0x112ed2, 0xff, 0x00}, //wriu -w 0x112ed2 0x0000 //
    {0x112ed3, 0xff, 0x00},
    {0x112ed4, 0xff, 0xcb}, //wriu -w 0x112ed4 0xA0CB //6
    {0x112ed5, 0xff, 0xa0},
    {0x112ed6, 0xff, 0x00}, //wriu -w 0x112ed6 0x0000 //
    {0x112ed7, 0xff, 0x00},

    {0x112e68, 0xff, 0x00}, //wriu -w 0x112e68 0x0300 //Time 0
    {0x112e69, 0xff, 0x03},
#ifdef CONFIG_MBOOT // define for power on music unstable
    {0x112e6a, 0xff, 0x2e}, //wriu -w 0x112e6a 0x122e //1
#else
    {0x112e6a, 0xff, 0x69}, //wriu -w 0x112e6a 0x1269 //1
#endif
    {0x112e6b, 0xff, 0x12},
    {0x112e6c, 0xff, 0x2b}, //wriu -w 0x112e6c 0x452B //3
    {0x112e6d, 0xff, 0x45},
    {0x112e6e, 0xff, 0x00}, //wriu -w 0x112e6e 0x0000 //5
    {0x112e6f, 0xff, 0x00},

    {0x112ed8, 0xff, 0x09}, //wriu -w 0x112ed8 0x9 //AutoBootStart
    {0x112ed9, 0xff, 0x00},

    {0x112cda, 0xff, 0x2b}, //wriu -w 0x112cda 0x32B //Final Step
    {0x112cdb, 0xff, 0x03},
    {0x112cdc, 0xff, 0x00}, //wriu -w 0x112cdc 0x0 //
    {0x112cdd, 0xff, 0x00},
    {0x112cde, 0xff, 0x00}, //wriu -w 0x112cde 0x7800 //
    {0x112cdf, 0xff, 0x78},
    {0x112ce0, 0xff, 0x00}, //wriu -w 0x112ce0 0x0 //
    {0x112ce1, 0xff, 0x00},
    {0x112ce2, 0xff, 0x00}, //wriu -w 0x112ce2 0x1000 //
    {0x112ce3, 0xff, 0x10},
    {0x112ce6, 0xff, 0x00}, //wriu -w 0x112ce6 0x0 //
    {0x112ce7, 0xff, 0x00},
    {0x112ce8, 0xff, 0x00}, //wriu -w 0x112ce8 0x0 //
    {0x112ce9, 0xff, 0x00},
    {0x112cea, 0xff, 0x00}, //wriu -w 0x112cea 0x0 //
    {0x112ceb, 0xff, 0x00},
    {0x112cec, 0xff, 0x00}, //wriu -w 0x112cec 0x1800 //
    {0x112ced, 0xff, 0x18},
    {0x112cee, 0xff, 0x03}, //wriu -w 0x112cee 0x303 //
    {0x112cef, 0xff, 0x03},
    {0x112ee0, 0xff, 0x0c}, //wriu -w 0x112ee0 0xC //
    {0x112ee1, 0xff, 0x00},
#else
    {0x112ee0, 0x0c, 0x0c}, //EN_CHOP_DAC
    {0x112cdc, 0xff, 0xf0},
    {0x112cdd, 0xff, 0x3f},
    {0x112cde, 0xff, 0x00},
    {0x112cdf, 0xff, 0x00},
    {0x112ce6, 0xff, 0x01},
    {0x112ce7, 0xff, 0x00},
    {0x112ce8, 0xff, 0x00},
    {0x112ce9, 0xff, 0x80},
    {0x112ce2, 0xff, 0x01}, //[7:4]SEL_CH_INMUX0,[3:0]SEL_CH_INMUX1
    {0x112ce3, 0xff, 0x00},
    {0x112ce0, 0xff, 0x30},
    {0x112ce1, 0xff, 0x10},
    {0x112cea, 0xff, 0x08},
    {0x112ceb, 0xff, 0x00},
    {0x112cee, 0xff, 0x00},
    {0x112cef, 0xff, 0x00},
    {0x112cec, 0xff, 0x00}, //[7]PD_LDO_ADC,[5]QS_LDO_ADC,[4]PD_LDO_DAC,[3]QS_LDO_DAC
    {0x112ced, 0xff, 0x00},

    {0x112cde, 0xff, 0x00},
    {0x112cdf, 0xff, 0x78},
    {0x112cea, 0xff, 0x08}, //[2]EN_VMID2GND_EAR
    {0x112ceb, 0xff, 0x00},
    {0x112cee, 0xff, 0x81}, //[7]EN_MUTE_EAR
    {0x112cef, 0xff, 0x10},
    {0x112cec, 0xff, 0x28},
    {0x112ced, 0xff, 0x00},

    {0xffffff, 0x01, 0x01}, // wait 1 //delay 1ms
    {0x112cdc, 0xff, 0x00},
    {0x112cdd, 0xff, 0x00},
    {0x112ce6, 0xff, 0x00},
    {0x112ce7, 0xff, 0x00},
    {0x112ce8, 0xff, 0x00},
    {0x112ce9, 0xff, 0x00},
    {0x112cea, 0xff, 0x00}, //[3]PD_OPLP_EAR,[2]EN_VMID2GND_EAR
    {0x112ceb, 0xff, 0x00},
#endif  //End of #if (HW_AutoDepop == 1) // auto depop

    {0xffffff, 0x00, 0x00}, //end of table
};

const AUDIO_REG_TYPE  AudioInitTbl[]=
{
#if (HW_AutoDepop == 1)
    // Mantis ID:1199683
    // This delay is for ac on video green noise,add delay will ok. But not known why cause timer issue.
    {0xffffff, 0x01, 0xff}, // delay 256ms
#else // remove for auto depop
    //wait 1536 //delay 1536ms
    //{0xffffff, 0x01, 0xff},
    //{0xffffff, 0x01, 0xff},
    //{0xffffff, 0x01, 0xff},
    //{0xffffff, 0x01, 0xff},
    {0xffffff, 0x01, 0xff},
    {0xffffff, 0x01, 0xff},
    //-----------------De-POP_2-----------------------------//Item 26
    {0x112ce0, 0xff, 0x00},
    {0x112ce1, 0xff, 0x00},
    {0x112ce2, 0xff, 0x01}, //[7:4]SEL_CH_INMUX0,[3:0]SEL_CH_INMUX1
    {0x112ce3, 0xff, 0x10},
    {0x112ce8, 0xff, 0x00},
    {0x112ce9, 0xff, 0x00},
    {0x112cec, 0xff, 0x28},
    {0x112ced, 0xff, 0x08},
    {0x112cee, 0xff, 0x01}, //[7]EN_MUTE_EAR
    {0x112cef, 0xff, 0x10},

    {0xffffff, 0x01, 0x01}, //wait 1 // delay 50ms -> 1ms
    {0x112cee, 0xff, 0x01},
    {0x112cef, 0xff, 0x13}, //[9]EN_LTX_EAR,[8]EN_RTX_EAR

    {0xffffff, 0x01, 0x32}, //wait 50 // delay 300ms -> 50ms
    {0x112cee, 0xff, 0x03},
    {0x112cef, 0xff, 0x13},
#endif  //End of #if (HW_AutoDepop == 1)

    //-----------------Input: HDMI (only Vivaldi side)------//Item 1
    {0x112c44, 0xff, 0x00}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x00}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x01}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x01}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x02}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x02}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x03}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x03}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x04}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x04}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x05}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x05}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x06}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x06}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x07}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x07}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x08}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x08}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x09}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x09}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0a}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0a}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0b}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0b}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0c}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0c}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0d}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0d}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0e}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0e}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x0f}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x0f}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x10}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x10}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x11}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x11}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x12}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x12}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x13}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x13}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x14}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x14}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x15}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x15}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x16}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x16}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x17}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x17}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x18}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x18}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x19}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x19}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1a}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1a}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1b}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1b}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1c}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1c}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1d}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1d}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1e}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1e}, // [12:8]HDMI Matrix WD
    {0x112c44, 0xff, 0x1f}, // [4:0] HDMI Matrix CFG
    {0x112c45, 0xff, 0x1f}, // [12:8]HDMI Matrix WD
    {0x11176a, 0xff, 0x7f}, // [6:0] HDMI RX LAUNCH CFG
    {0x111718, 0x33, 0x20}, // [2:0]HDMI status channel SEL, [5:4]HDMI status SEL
    {0x111719, 0xff, 0x0d}, // [8]HDMI left justified, [9]MSB first, [15:10]HDMI status block start SEL
    {0x112c47, 0x83, 0x01}, // [8]DSD MODE SW SEL, [9]DSD MODE SW, [15]MCH EN
    {0x112c48, 0x10, 0x10}, // [4] npcm byte mode
    {0x112c48, 0x08, 0x08}, // [3] npcm decoder PA/PB order
    {0x112c48, 0x04, 0x00}, // [2] npcm decoder fast lock
    {0x112c48, 0x03, 0x00}, // [1:0] npcm decoder time out SEL

    //-----------------Input: I2S (default slave)---------------------------//Item 2
    {0x112c02, 0xff, 0x1f}, // [3:0] I2S/SIF/SPDIF clk EN, [4] I2S left-justified
    {0x112c03, 0xff, 0xc0}, // [14]AUTO_CLEAR_PC_PD_BAR, [15]SPDIF_IN_BLOCK_NO_CHECK
    {0x112cae, 0xff, 0x0e}, // [3:1] non-pcm synthesizer source SEL 432MHz
    {0x112caf, 0xff, 0x40}, // [15:13]i2s fs synthesizer SEL source clock 432MHz

    //-----------------Input: SPDIF-------------------------//Item 3
    {0x112c0a, 0xff, 0x70},
    {0x112c0b, 0xff, 0x13},

    //-----------------Input: ADC---------------------------//Item 4
    {0x112cda, 0xff, 0x2b},
    {0x112cdb, 0xff, 0x03},
    {0x112cda, 0xff, 0x28}, // [1:0]PD_ADC
    {0x112cdb, 0xff, 0x03},
    {0x112cf2, 0xff, 0x21}, // ADC set invert
    {0x112cf3, 0xff, 0x0f},

    //-----------------Decode Path--------------------------//Item 28
    {0x112c60, 0xff, 0x80}, // Decoder 1 source from DVB1
    {0x112c61, 0xff, 0x80}, // Decoder 3 source from DVB3
    {0x112c62, 0xff, 0x80}, // Decoder 2 source from DVB2
    {0x112c63, 0xff, 0x87}, // Decoder 4 source from SIF

    //-----------------SIF init table-----------------------//Item 27/31
    //call Jeff-CY.Lee(#7019) for VIF_SIF_ini_table.xls

    //-----------------Input: Channel Source----------------//Item 6
    {0x112c64, 0xff, 0x80}, // CH1: Source0 , CH5: ADC0
    {0x112c65, 0xff, 0x80}, // Set CH5 default source from decoder
    {0x112c66, 0xff, 0x80}, // CH2: Source0 , CH6: Source1
    {0x112c67, 0xff, 0x81},
    {0x112c68, 0xff, 0x80}, // CH3: Source0 , CH7: Source3
    {0x112c69, 0xff, 0x83},
    {0x112c6a, 0xff, 0x80}, // CH4: Source0 , CH8: Source7
    {0x112c6b, 0xff, 0x87},

    //-----------------Output: I2S (BCK 64fs, msater)--------------------------//Item 10
    {0x112c90, 0x40, 0x00},
    {0x112c90, 0x1e, 0x00},
    {0x112c8d, 0x02, 0x00},
    {0x112c8d, 0x01, 0x00},
    {0x112c8c, 0x70, 0x20},
    {0x112c8c, 0x08, 0x00},
    {0x112c8c, 0x07, 0x02},
    {0x112c8d, 0x80, 0x80},
    {0x112c8d, 0x40, 0x40},
    {0x112c8d, 0x08, 0x08},
    {0xffffff, 0x01, 0x01}, // delay 1ms
    {0x112c8d, 0x40, 0x00},
    {0x112c8d, 0x08, 0x00},

    //-----------------Output: SPDIF TX1--------------------//Item 11
    {0x112c90, 0x01, 0x00},
    {0x112c8b, 0x80, 0x80},
    {0x112c8b, 0x20, 0x00},
    {0x112c8b, 0x10, 0x00},
    {0x112c8b, 0x08, 0x00},
    {0x112c8b, 0x04, 0x00},
    {0x112c8b, 0x02, 0x00},
    {0x112c8b, 0x01, 0x00},
    {0x112c8a, 0x10, 0x10}, //[4]spdif status from RIU
    {0x112c8a, 0x08, 0x00},
    {0x112c8a, 0x04, 0x00},
    {0x112c8a, 0x03, 0x00},
    {0x112c8a, 0x80, 0x80},
    {0x112c8a, 0x40, 0x40},
    {0x112c8a, 0x20, 0x20},
    {0xffffff, 0x01, 0x01}, // delay 1ms
    {0x112c8a, 0x40, 0x00},
    {0x112c8a, 0x20, 0x00},

    //-----------------Output: DAC--------------------------//Item 12
#if (HW_AutoDepop == 0) // remove for auto depop
    {0xffffff, 0x01, 0xfa},
    {0xffffff, 0x01, 0x64}, //wait 350 //delay 650ms -> 350ms
    {0x112cee, 0xff, 0x03},
    {0x112cef, 0xff, 0x03}, //[12]EN_MSP
    {0x112cec, 0xff, 0x00}, //[5]QS_LDO_ADC,[3]QS_LDO_DAC
    {0x112ced, 0xff, 0x18},
#endif  //End of #if (HW_AutoDepop == 0) // remove for auto depop

    //-----------------Output: SPDIF TX2--------------------//Item 13
    {0x112c5f, 0x80, 0x80},
    {0x112c5f, 0x40, 0x40},
    {0x112c5f, 0x20, 0x00},
    {0x112c5f, 0x10, 0x00},
    {0x112c5f, 0x08, 0x00},
    {0x112c5f, 0x04, 0x00},
    {0x112c5f, 0x02, 0x00},
    {0x112c5f, 0x01, 0x00},
    {0x112c5e, 0x10, 0x10}, //[4]spdif status from DSP/RIU
    {0x112c5e, 0x08, 0x00},
    {0x112c5e, 0x04, 0x00},
    {0x112c5e, 0x03, 0x00},
    {0x112c5e, 0x80, 0x80},
    {0x112c5e, 0x40, 0x40},
    {0x112c5e, 0x20, 0x20},
    {0xffffff, 0x01, 0x01}, // delay 1ms
    {0x112c5e, 0x40, 0x00},
    {0x112c5e, 0x20, 0x00},
    {0x112c5e, 0x90, 0x90},

    //---------------------------------------------------//Item 30
    {0x112c94, 0x01, 0x01}, // enable CH1 HW force mute, [0] ch1 mute all
    {0x112c95, 0x01, 0x01}, // enable CH5 HW force mute, [8] ch5 mute all
    {0x112c96, 0x01, 0x01}, // enable CH2 HW force mute, [0] ch2 mute all
    {0x112c97, 0x01, 0x01}, // enable CH6 HW force mute, [8] ch6 mute all
    {0x112c98, 0x01, 0x01}, // enable CH3 HW force mute, [0] ch3 mute all
    {0x112c99, 0x01, 0x01}, // enable CH7 HW force mute, [8] ch7 mute all
    {0x112c9a, 0x01, 0x01}, // enable CH4 HW force mute, [0] ch4 mute all
    {0x112c9b, 0x01, 0x01}, // enable CH8 HW force mute, [8] ch8 mute all

    //-----------------Interrupt--------------------------//Item 35
    {0x112aa2, 0x80, 0x00}, //[7]=0       IRQ1E select HDMI TX
    {0x112ad8, 0x10, 0x20}, //[4]=0       IRQ1E select HDMI TX
    {0x112aa2, 0x40, 0x40}, //[6:5]=2'b10 IRQ2n select SPDIF TX
    {0x112aa2, 0x10, 0x00}, //[4]=0       IRQ1L select SPDIF TX2
    {0x112ad8, 0x20, 0x20}, //[5]=1       IRQ1L select SPDIF TX2

    //-----------------MIU & DMA Setting---------------------------//Item 34
    {0x112a44, 0xff, 0x00}, //[15]    maddma fifo addr demeta
    {0x112a45, 0xff, 0x80},
    {0x112ac4, 0xff, 0x00}, //[15]    Sedma fifo addr demeta
    {0x112ac5, 0xff, 0x80},
    {0x112a21, 0x04, 0x00}, //[10]    maddma fifo new mode
    {0x112aa1, 0x04, 0x00}, //[10]    sedma fifo new mode(no function)

    {0x163d03, 0x60, 0x20}, //[14:13] =01 for auR2 use MIU0, =10 for auR2 use MIU1
    {0x163d07, 0x60, 0x20}, //[14:13] =01 for auR2_1 use MIU0, =10 for auR2_1 use MIU1

    {0x112940, 0x20, 0x00}, //[5]     enable R2 Write buffer burst mode
    {0x163040, 0x20, 0x00}, //[5]     enable R2_1 Write buffer burst mode

    {0x112f68, 0xff, 0xff}, //        dma arb
    {0x112f69, 0xff, 0xff},
    {0x112f66, 0xff, 0x00}, //        dma arb
    {0x112f67, 0xff, 0x00},
    {0x112f64, 0xff, 0x00}, //        dma arb
    {0x112f65, 0xff, 0x04},
    {0x112f62, 0xff, 0x08}, //        dma arb
    {0x112f63, 0xff, 0x20},
    {0x112f60, 0xff, 0x15}, //        dma arb
    {0x112f61, 0xff, 0x80},

    {0x112f88, 0xff, 0xff}, //        cache arb
    {0x112f89, 0xff, 0xff},
    {0x112f86, 0xff, 0x00}, //        cache arb
    {0x112f87, 0xff, 0x00},
    {0x112f84, 0xff, 0x00}, //        cache arb
    {0x112f85, 0xff, 0x04},
    {0x112f82, 0xff, 0x08}, //        cache arb
    {0x112f83, 0xff, 0x20},
    {0x112f80, 0xff, 0x15}, //        cache arb
    {0x112f81, 0xff, 0x80},

    {0x163d88, 0xff, 0xff}, //        R2 arb
    {0x163d89, 0xff, 0xff},
    {0x163d86, 0xff, 0x00}, //        R2 arb
    {0x163d87, 0xff, 0x00},
    {0x163d84, 0xff, 0x00}, //        R2 arb
    {0x163d85, 0xff, 0x04},
    {0x163d82, 0xff, 0x08}, //        R2 arb
    {0x163d83, 0xff, 0x20},
    {0x163d80, 0xff, 0x15}, //        R2 arb
    {0x163d81, 0xff, 0x80},

    {0x163da8, 0xff, 0xff}, //        R2_1 arb
    {0x163da9, 0xff, 0xff},
    {0x163da6, 0xff, 0x00}, //        R2_1 arb
    {0x163da7, 0xff, 0x00},
    {0x163da4, 0xff, 0x00}, //        R2_1 arb
    {0x163da5, 0xff, 0x04},
    {0x163da2, 0xff, 0x08}, //        R2_1 arb
    {0x163da3, 0xff, 0x20},
    {0x163da0, 0xff, 0x15}, //        R2_1 arb
    {0x163da1, 0xff, 0x80},

    {0x163e28, 0xff, 0xff}, //        dma_l2 arb
    {0x163e29, 0xff, 0xff},
    {0x163e26, 0xff, 0x00}, //        dma_l2 arb
    {0x163e27, 0xff, 0x00},
    {0x163e24, 0xff, 0x00}, //        dma_l2 arb
    {0x163e25, 0xff, 0x04},
    {0x163e22, 0xff, 0x08}, //        dma_l2 arb
    {0x163e23, 0xff, 0x20},
    {0x163e20, 0xff, 0x15}, //        dma_l2 arb
    {0x163e21, 0xff, 0x80},

    //----------------- DFS ---------------------------//Item 36
    {0x163c61, 0x03, 0x00}, //[9:8]   CLK DSP DFS SEL[1:0] = AUPLL
    {0x163c61, 0x10, 0x00}, //[12]    CLK DSP DFS SEL[2] = 0
    {0x163c61, 0x08, 0x08}, //[11]    CLK DSP DFS EN
    {0x163c60, 0x1f, 0x1f}, //[4:0]   DSP DFS DIV
    {0x163c60, 0x40, 0x40}, //[6]     DSP DFS EN
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x80}, //[7]     DSP DFS UPDATE
    {0x163c60, 0x80, 0x00}, //[7]     DSP DFS UPDATE
    {0x163c63, 0x03, 0x00}, //[9:8]   CLK R2 DFS SEL[1:0] = AUR2PLL
    {0x163c63, 0x10, 0x00}, //[12]    CLK R2 DFS SEL[2] = 0
    {0x163c63, 0x08, 0x08}, //[11]    CLK R2 DFS EN
    {0x163c62, 0x1f, 0x1f}, //[4:0]   R2 DFS DIV
    {0x163c62, 0x40, 0x40}, //[6]     R2 DFS EN
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x80}, //[7]     R2 DFS UPDATE
    {0x163c62, 0x80, 0x00}, //[7]     R2 DFS UPDATE

    // ----------------------------------------------------
    //  sound effect init settings
    //-----------------------------------------------------
    {0x112D01, 0xFF, 0x8C}, // AUOUT0 volume :  0dB ; Mute  @@VVV
    {0x112D03, 0xFF, 0x8C}, // AUOUT1 volume :  0dB ; Mute  @@VVV
    {0x112D05, 0xFF, 0x8C}, // AUOUT2 volume :  0dB ; Mute  @@VVV
    {0x112D07, 0xFF, 0x8C}, // AUOUT3 volume :  0dB ; Mute  @@VVV
    {0x112D09, 0xFF, 0x8C}, // I2S_OUT volume :  0dB ; Mute @@VVV
    {0x112D0B, 0xFF, 0x0C}, // SPDIF_OUT volume :  0dB ; UnMute   @@VVV
                                              // if spdif change to mute, need set g_SPDIF_MuteStatus = 1;
    {0x112D0F, 0xFF, 0x0C}, // HDMI_OUT volume :  0dB ; UnMute   @@VVV

    {0x112D20, 0x80, 0x00}, // Disable EQ @@VVV
    {0x112D21, 0xFF, 0xC8}, // Enable Sound effect & tone @@VVV
    {0x112D22, 0x3F, 0x3F}, // Enable all output Volume control @@VVV
    {0x112D23, 0x01, 0x01}, // Enable SPDIF_OUT volume control   @@VVV
    {0x112D31, 0x02, 0x00}, // disable SE-DSP power-down command    @@VVV

    {0x112D50, 0xFF, 0xFF}, // DAC0/1 source default in NULL path  @@VVV
    {0x112D51, 0xFF, 0xFF}, // DAC2/3 source default in NULL path  @@VVV
    {0x112D52, 0xFF, 0xFF}, // I2S source default in NULL path  @@VVV
    {0x112D53, 0xFF, 0xFF}, // I2S source default in NULL path  @@VVV
    {0x112D54, 0xF0, 0x40}, // SRC source from PCM  @@VVV

    {0x112D5D, 0xFF, 0x0C}, // set mix volume of CH5, CH6, CH8 to 0dB
    {0x112D5C, 0xFF, 0x00},
    {0x112D5F, 0xFF, 0x0C},
    {0x112D5E, 0xFF, 0x00},
    {0x112D5B, 0xFF, 0x0C},
    {0x112D5A, 0xFF, 0x00},

    {0xffffff, 0x00, 0x00}, // end of table

};

const AUDIO_REG_TYPE2 ResetDMIOTbl[]=
{
    //DMIO a0xx reset
    {0x112a80, 0xff, 0x40},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x5e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x62},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x66},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x6e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x72},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xb5},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa0},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x7e},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x82},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x86},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x8a},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0x76},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe0},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a84, 0xff, 0xe8},
    {0x112a85, 0xff, 0xa1},
    {0x112a82, 0xff, 0x00},
    {0x112a83, 0xff, 0x00},
    {0x112a82, 0xff, 0x80},
    {0x112a83, 0xff, 0x00},
    {0x112a80, 0xff, 0x53},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x43},
    {0x112a81, 0xff, 0x00},
    {0x112a80, 0xff, 0x00},
    {0x112a81, 0xff, 0x00},
    {0x112c00, 0xff, 0x01},
    {0x112c01, 0xff, 0x01},
    {0x112c00, 0xff, 0x00},
    {0x112c01, 0xff, 0x00},
    //end DMIO a0xx reset
    {0xffffff, 0x00, 0x00}, // end of table
};

#if AUDIO_HW_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Reader1 = {
    .u8Name = AUDIO_HW_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Reader1_Open,
        .close = HAL_AUDIO_PCM_HwDma_Reader1_Close,
        .start = HAL_AUDIO_PCM_HwDma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Reader1_Set,
        .get = HAL_AUDIO_PCM_HwDma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_HwDma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_HwDma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Reader2 = {
    .u8Name = AUDIO_HW_DMA_READER2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Reader2_Open,
        .close = HAL_AUDIO_PCM_HwDma_Reader2_Close,
        .start = HAL_AUDIO_PCM_HwDma_Reader2_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Reader2_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Reader2_Set,
        .get = HAL_AUDIO_PCM_HwDma_Reader2_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_HwDma_Reader2_Write,
        .flush = HAL_AUDIO_PCM_HwDma_Reader2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_SwDma_Reader1 = {
    .u8Name = AUDIO_SW_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_SwDma_Reader1_Open,
        .close = HAL_AUDIO_PCM_SwDma_Reader1_Close,
        .start = HAL_AUDIO_PCM_SwDma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_SwDma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_SwDma_Reader1_Set,
        .get = HAL_AUDIO_PCM_SwDma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_SwDma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_SwDma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_R2Dma_Reader1 = {
    .u8Name = AUDIO_R2_DMA_READER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_R2Dma_Reader1_Open,
        .close = HAL_AUDIO_PCM_R2Dma_Reader1_Close,
        .start = HAL_AUDIO_PCM_R2Dma_Reader1_Start,
        .stop = HAL_AUDIO_PCM_R2Dma_Reader1_Stop,
        .set = HAL_AUDIO_PCM_R2Dma_Reader1_Set,
        .get = HAL_AUDIO_PCM_R2Dma_Reader1_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_R2Dma_Reader1_Write,
        .flush = HAL_AUDIO_PCM_R2Dma_Reader1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = TRUE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2, 10},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
AUDIO_PCM_t Audio_Pcm_R2Dma_Reader2 = {
    .u8Name = AUDIO_R2_DMA_READER2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_R2Dma_Reader2_Open,
        .close = HAL_AUDIO_PCM_R2Dma_Reader2_Close,
        .start = HAL_AUDIO_PCM_R2Dma_Reader2_Start,
        .stop = HAL_AUDIO_PCM_R2Dma_Reader2_Stop,
        .set = HAL_AUDIO_PCM_R2Dma_Reader2_Set,
        .get = HAL_AUDIO_PCM_R2Dma_Reader2_Get,
        .read = NULL,
        .write = HAL_AUDIO_PCM_R2Dma_Reader2_Write,
        .flush = HAL_AUDIO_PCM_R2Dma_Reader2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = TRUE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = FALSE,
        .u32Channel = {1, 2, 10},
        .u32SampleRate = {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000, 64000, 88200, 96000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
AUDIO_PCM_t Audio_Pcm_Capture1 = {
    .u8Name = AUDIO_PCM_CAPTURE1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_Capture1_Open,
        .close = HAL_AUDIO_PCM_Capture1_Close,
        .start = HAL_AUDIO_PCM_Capture1_Start,
        .stop = HAL_AUDIO_PCM_Capture1_Stop,
        .set = HAL_AUDIO_PCM_Capture1_Set,
        .get = HAL_AUDIO_PCM_Capture1_Get,
        .read = HAL_AUDIO_PCM_Capture1_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_Capture1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
AUDIO_PCM_t Audio_Pcm_Capture2 = {
    .u8Name = AUDIO_PCM_CAPTURE2,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_Capture2_Open,
        .close = HAL_AUDIO_PCM_Capture2_Close,
        .start = HAL_AUDIO_PCM_Capture2_Start,
        .stop = HAL_AUDIO_PCM_Capture2_Stop,
        .set = HAL_AUDIO_PCM_Capture2_Set,
        .get = HAL_AUDIO_PCM_Capture2_Get,
        .read = HAL_AUDIO_PCM_Capture2_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_Capture2_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
AUDIO_PCM_t Audio_Pcm_HwDma_Writer1 = {
    .u8Name = AUDIO_HW_DMA_WRITER1,
    .tPcmOps = {
        .open = HAL_AUDIO_PCM_HwDma_Writer1_Open,
        .close = HAL_AUDIO_PCM_HwDma_Writer1_Close,
        .start = HAL_AUDIO_PCM_HwDma_Writer1_Start,
        .stop = HAL_AUDIO_PCM_HwDma_Writer1_Stop,
        .set = HAL_AUDIO_PCM_HwDma_Writer1_Set,
        .get = HAL_AUDIO_PCM_HwDma_Writer1_Get,
        .read = HAL_AUDIO_PCM_HwDma_Writer1_Read,
        .write = NULL,
        .flush = HAL_AUDIO_PCM_HwDma_Writer1_Flush,
    },
    .tPcmCaps = {
        .u8MultiChFlag = FALSE,
        .u8MixingFlag = FALSE,
        .u8CaptureFlag = TRUE,
        .u32Channel = {2},
        .u32SampleRate = {48000},
        .u32BitWidth = {16},
    },
};
#endif

MS_U8 g_Common_PCM_IO_Init = FALSE;

AUDIO_PCM_t *g_PCM[AUDIO_PCM_MAXNUM] = {
#if AUDIO_HW_DMA_READER1_SUPPORT
    &Audio_Pcm_HwDma_Reader1,
#endif
#if AUDIO_HW_DMA_READER2_SUPPORT
    &Audio_Pcm_HwDma_Reader2,
#endif
#if AUDIO_SW_DMA_READER1_SUPPORT
    &Audio_Pcm_SwDma_Reader1,
#endif
#if AUDIO_R2_DMA_READER1_SUPPORT
    &Audio_Pcm_R2Dma_Reader1,
#endif
#if AUDIO_R2_DMA_READER2_SUPPORT
    &Audio_Pcm_R2Dma_Reader2,
#endif
#if AUDIO_PCM_CAPTURE1_SUPPORT
    &Audio_Pcm_Capture1,
#endif
#if AUDIO_PCM_CAPTURE2_SUPPORT
    &Audio_Pcm_Capture2,
#endif
#if AUDIO_HW_DMA_WRITER1_SUPPORT
    &Audio_Pcm_HwDma_Writer1,
#endif
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    MS_BOOL ret = TRUE;

    if (HAL_AUDIO_HDMI_NonpcmMonitor())
    {
        switch(HAL_AUDIO_HDMI_DolbyMonitor())
        {
            case 0x01:  p_hdmiAudioSts->audio_type = HDMI_RX_DD;        break;
            case 0x0b:  p_hdmiAudioSts->audio_type = HDMI_RX_DTS;       break;
            case 0x15:  p_hdmiAudioSts->audio_type = HDMI_RX_DDP;       break;
            default:    p_hdmiAudioSts->audio_type = HDMI_RX_Other;     break;
        }
    }
    else
        p_hdmiAudioSts->audio_type = HDMI_RX_PCM;

    p_hdmiAudioSts->sampleRate = AUDIO_FS_48KHZ;

    return ret;

}

MS_BOOL HAL_AUDIO_ResetDefaultVars(AUDIO_SHARED_VARS2 * pAUDIOShared)
{
    int i = 0;
    int array_mount = 0;

    if (pAUDIOShared == NULL)
    {
        return FALSE;
    }

    memset(pAUDIOShared, 0x00, sizeof(AUDIO_SHARED_VARS2));

    /* Fill in default value */
    pAUDIOShared->g_DspCodeType = AU_DVB_STANDARD_INVALID;
    pAUDIOShared->g_Dsp2CodeType = AU_DVB_STANDARD_INVALID;
    pAUDIOShared->g_u8MADSkipResetFlag = FALSE;
    pAUDIOShared->g_u8MAD2SkipResetFlag = FALSE;
    pAUDIOShared->g_AdvSndEffDspCodeType = AU_DVB_STANDARD_INVALID;
    pAUDIOShared->g_u8DspAliveFlag = TRUE;
    pAUDIOShared->g_eSpdifOutputType = SPDIF_OUT_NONE;
    pAUDIOShared->g_SpdifNpcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->g_u8SifDspType = DSP_SE;
    pAUDIOShared->g_dolby_truehd_enable = FALSE;
    pAUDIOShared->g_PreMainDspCodeType = AU_DVB_STANDARD_INVALID;

    array_mount = sizeof(pAUDIOShared->sif_gain_0) / sizeof(pAUDIOShared->sif_gain_0[0]);
    for (i = 0; i < array_mount; i++)
    {
        /* Note! sif_gain_0[] is expected same length as sif_shift_0[] */
        pAUDIOShared->sif_gain_0[i] = 0x7FFF;
        pAUDIOShared->sif_shift_0[i] = 0x0000;
    }
    pAUDIOShared->g_u8IntTag = 1;
    pAUDIOShared->int_mb_cnt = 0;

    pAUDIOShared->CompressInfo = 0;
    for ( i = 0; i < MAX_AUDIO_BASE_ADDRESS_NUM2; i++ )
    {
        pAUDIOShared->g_DSPMadMIUBank[i] = g_DSPMadMIUBank[i];
        pAUDIOShared->g_DSPBinBaseAddress[i] = g_DSPBinBaseAddress[i];
        pAUDIOShared->g_DSPMadBaseBufferAdr[i] = g_DSPMadBaseBufferAdr[i];
        pAUDIOShared->g_DSPDDRBinBaseAddress[i] = g_DSPDDRBinBaseAddress[i];
    }

    pAUDIOShared->eMainSourceType = E_AUDIO_INFO_DTV_IN;
    pAUDIOShared->ePrevSourceType = E_AUDIO_INFO_DTV_IN;

    pAUDIOShared->eAudioSource = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->eMainAudioSource = AUDIO_NULL_INPUT;
    pAUDIOShared->eSubAudioSource = AUDIO_NULL_INPUT;
    pAUDIOShared->eScartAudioSource = AUDIO_NULL_INPUT;

    pAUDIOShared->g_SPDIF_MuteStatus = 0;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_SPDIF_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_ARC_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].eDigitalOutfMode  = DIGITAL_OUT_NONE;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].u8R2NonPcmSetting  = 0x00;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].eSourceType  = E_AUDIO_INFO_ADC_IN;
    pAUDIOShared->stDigitalOutSetting[DIGITAL_HDMI_OUTPUT].u8NonPcmPath = AUDIO_DSP_ID_ALL;

    pAUDIOShared->g_dolbyLoudness_DTVsystem = DVB_MODE;
    pAUDIOShared->g_dolbyLoudness_Enable = 1;
    /* 1st decoder (depends on chip) */
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u32AudioCapsVersion = AUD_CAP_VERSION;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].u8MaxDmaReaderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsADSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsPusiDetectSupported = FALSE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsMpegSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAc3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAc3pSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsMp3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsWmaSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsRa8LbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsXpcmSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDtsSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsWmaProSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsFlacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsVorbisSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDtsLbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAmrNbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsAmrWbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID1].bIsDraSupported = TRUE;

    /* 2nd decoder (depends on chip) */
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u32AudioCapsVersion = AUD_CAP_VERSION;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u8MaxDecoderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].u8MaxDmaReaderNum = 2;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsADSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsPusiDetectSupported = FALSE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsMpegSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAc3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAc3pSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsMp3Supported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsWmaSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsRa8LbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsXpcmSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDtsSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsWmaProSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsFlacSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsVorbisSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDtsLbrSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAmrNbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsAmrWbSupported = TRUE;
    pAUDIOShared->AudioCaps[AU_DEC_ID3].bIsDraSupported = TRUE;

    return TRUE;
}

AUDIO_SHARED_VARS2 * HAL_AUDIO_AllocateVars(void)
{
    AUDIO_SHARED_VARS2 * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    AUDIO_SHARED_VARS2 * virtAddr = 0;
    MS_U32 u32BufSize = 0;

    HALAUDIO_ERROR("Start %s InitShmFlag=%d\r\n", __FUNCTION__, g_bInitShmFlag);

    if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_QUERY) == TRUE)
    {
        pAUDIOShared = (AUDIO_SHARED_VARS2 *) virtAddr;

        HALAUDIO_ERROR("%s QUERY ClientCounter=%d\r\n", __FUNCTION__, pAUDIOShared->g_u32ClientCounter);

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            HALAUDIO_ERROR("Resume from Suspend mode, reset Audio SHM data ...\n");
        }
    }
    else
    {
        HALAUDIO_ERROR("create Audio SHM data ...\n");

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_CREATE) == TRUE)
        {
            HALAUDIO_ERROR("Audio SHM data is created\n");
            pAUDIOShared = (AUDIO_SHARED_VARS2 *) virtAddr;

            HALAUDIO_ERROR("reset Audio SHM data ...\n");
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);
        }
        else
        {
            HALAUDIO_ERROR("AUDIO SHM allocation failed!\n");
            return NULL;
        }
    }

    if (g_bInitShmFlag == FALSE)
    {
        g_bInitShmFlag = TRUE;
        pAUDIOShared->g_u32ClientCounter++;
    }

    HALAUDIO_ERROR("End %s InitShmFlag=%d, ClientCounter=%d \r\n", __FUNCTION__, g_bInitShmFlag, pAUDIOShared->g_u32ClientCounter);

#else
    pAUDIOShared = &g_audioShared;

    if (g_bInitShmFlag == FALSE)
    {
        HALAUDIO_PRINT("create Audio SHM data ...\n");
        g_bInitShmFlag = TRUE;

        HAL_AUDIO_ResetDefaultVars(pAUDIOShared);

        pAUDIOShared->g_u32ClientCounter++;
    }
#endif

    gOpenDecodeSystemFuncPtr = HAL_AUDIO_OpenDecodeSystem;
    gDigitalOutChannelStatusFuncPtr = HAL_AUDIO_DigitalOut_SetChannelStatus;
    pFuncPtr_Setsystem = HAL_AUDIO_SetDecodeSystem;
    gGetDDRInfoFuncPtr = HAL_AUDIO_GetDDRInfo;

    return pAUDIOShared;
}

MS_BOOL HAL_AUDIO_DeAllocateVars(void)
{
    AUDIO_SHARED_VARS2 * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    AUDIO_SHARED_VARS2 * virtAddr = 0;
    MS_U32 u32BufSize = 0;

    HALAUDIO_ERROR("Start HAL_AUDIO_DeAllocateVars g_bInitShmFlag=%d\r\n", g_bInitShmFlag);

    if (g_bInitShmFlag == TRUE)
    {
        g_bInitShmFlag = FALSE;

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_SHM_NAME, sizeof(AUDIO_SHARED_VARS2), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_QUERY) == FALSE)
        {
            HALAUDIO_ERROR("%s() : No AUDIO SHM is created!\n", __FUNCTION__);
            return FALSE;
        }

        pAUDIOShared = (AUDIO_SHARED_VARS2 *)virtAddr;
        pAUDIOShared->g_u32ClientCounter--;

        HALAUDIO_ERROR("HAL_AUDIO_DeAllocateVars ClientCounter=%d\r\n", pAUDIOShared->g_u32ClientCounter);

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);

            HALAUDIO_ERROR("free Audio SHM data ...\n");
#if defined(MSOS_TYPE_LINUX)
            if (MsOS_SHM_FreeId((MS_U8 *)AUDIO_SHM_NAME, u32ShmId) == TRUE)
            {
                HALAUDIO_ERROR("Audio SHM data is freed\n");
            }
            else
            {
                HALAUDIO_ERROR("%s() : AUDIO SHM de-allocation failed!\n", __FUNCTION__);
                return FALSE;
            }
#endif

            HAL_AUDIO_Mutex_DeInit();
        }
        /* patch for DC off/on no sound issue start */
        else
        {
            HAL_AUDIO_ResetDefaultVars(pAUDIOShared);
        }
        /* patch for DC off/on no sound issue end */
    }

    HALAUDIO_ERROR("End HAL_AUDIO_DeAllocateVars g_bInitShmFlag=%d\r\n", g_bInitShmFlag);
#else
    pAUDIOShared = &g_audioShared;

    if (g_bInitShmFlag == TRUE)
    {
        HALAUDIO_PRINT("free Audio SHM data ...\n");
        g_bInitShmFlag = FALSE;

        memset(pAUDIOShared, 0x00, sizeof(AUDIO_SHARED_VARS2));
    }
#endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_InitialVars()
/// @brief \b Function \b Description: This routine set audio share memory of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_InitialVars(void)
{
    if (HAL_AUDIO_GET_INIT_FLAG() == FALSE)
    {
        g_AudioVars2 = HAL_AUDIO_AllocateVars();
        if (g_AudioVars2 == NULL)
        {
            return FALSE;
        }

#ifndef MSOS_TYPE_NUTTX
        pAudioTeeInfoShm = HAL_AUDIO_AllocateTeeInfoShm();
        if (pAudioTeeInfoShm == NULL)
        {
            HALAUDIO_ERROR("Create Audio TEE INFO SHM data fail...\n");
            return FALSE;
        }
    }
#else
        {
            MS_U8 audio_mbx_class=0;

            MApi_MBX_GenerateDynamicClass(E_MBX_CPU_MIPS, "MAD", (MS_U8 *)&audio_mbx_class);
            if ( E_MBX_SUCCESS != MApi_MBX_RegisterMSGWithCallBack(audio_mbx_class, 10, HAL_AUDIO_HandleDynamicMBX))
            {
                HALAUDIO_ERROR("MAD Register MBX MSG error\n");
               return FALSE;
            }
            else
                DBG_AUDIO("MAD MBX register msg ok %d\n", audio_mbx_class);

            _s32MadEventId = MsOS_CreateEventGroup("MAD_Event");
            if (_s32MadEventId < 0)
            {
                HALAUDIO_ERROR("MAD CreateEventGroup error....\n");
                return FALSE;
            }

            _s32MadTaskId = MsOS_CreateTask((TaskEntry)_MAD_Proc,
                    NULL,
                    E_TASK_PRI_MEDIUM,
                    TRUE,
                    NULL,
                    32, // stack size..
                    "MAD_ISR_Task");
            if (_s32MadTaskId < 0)
            {
                MsOS_DeleteEventGroup(_s32MadEventId);
                HALAUDIO_ERROR("MAD CreateTask error....\n");
                return FALSE;
            }
        }
    }
#endif

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DeInitialVars()
/// @brief \b Function \b Description: This routine delete audio share memory of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DeInitialVars(void)
{
    MS_BOOL ret = TRUE;

    if (HAL_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        HAL_AUDIO_SET_INIT_FLAG(FALSE);

        ret = HAL_AUDIO_DeAllocateVars();
        if (ret == FALSE)
        {
            HALAUDIO_ERROR ("%s() : AUDIO SHM de-allocation failed!\n", __FUNCTION__);
        }

#if 0  /* patch for STR resume segmentation fault issue */
        g_AudioVars2 = NULL;
#endif

#ifndef MSOS_TYPE_NUTTX
        {
            MS_BOOL ret1;

            ret1 = HAL_AUDIO_DeAllocateTeeInfoShm();
            if (ret1 == FALSE)
            {
                HALAUDIO_ERROR("%s() : AUDIO TEE Info SHM de-allocation failed!\n", __FUNCTION__);
                ret = FALSE;
            }
        }
#endif
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SET_INIT_FLAG()
/// @brief \b Function \b Description: This routine set the initial status of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  MS_BOOL    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SET_INIT_FLAG(MS_BOOL bSet)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_Audio_InitFlag = bSet;

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GET_INIT_FLAG()
/// @brief \b Function \b Description: This routine get the initial status of Aduio module.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  MS_BOOL    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GET_INIT_FLAG(void)
{
    if (g_AudioVars2 == NULL)
    {
        return FALSE;
    }

    return g_AudioVars2->g_Audio_InitFlag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Mutex_Init()
/// @brief \b Function \b Description: This routine is the initialization of Mutex for Audio module.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b  MS_BOOL : TRUE - Success, FALSE - Fail
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Mutex_Init(void)
{
    MS_BOOL ret = TRUE;

    if ((_s32AUDIOMutex != -1) &&
        (_s32AUDIOMutexReboot != -1) &&
        (_s32MutexLoadCode != -1) &&
        (_s32AUDIOMutexIDMA != -1))
    {
        return ret;
    }

    if (_s32AUDIOMutex == -1)
    {
        _s32AUDIOMutex = OS_CREATE_MUTEX(AUDIO);
        if (_s32AUDIOMutex == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex failed!\n");
            ret = FALSE;
        }
    }

    if (_s32AUDIOMutexReboot == -1)
    {
        _s32AUDIOMutexReboot = OS_CREATE_MUTEX(AUDIOreboot);
        if (_s32AUDIOMutexReboot == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for Reboot failed!\n");
            ret = FALSE;
        }
    }

    if (_s32MutexLoadCode == -1)
    {
        _s32MutexLoadCode = OS_CREATE_MUTEX(LOAD_DECDSP);
        if (_s32MutexLoadCode == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for Load DSP failed!\n");
            ret = FALSE;
        }
    }

    if (_s32AUDIOMutexIDMA == -1)
    {
        _s32AUDIOMutexIDMA = OS_CREATE_MUTEX(AUDIO_IDMA);
        if (_s32AUDIOMutexIDMA == -1)
        {
            HALAUDIO_ERROR("Error! Create Mutex for IMDA failed!\n");
            ret = FALSE;
        }
    }

    if (ret == FALSE)
    {
        if (_s32AUDIOMutex != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutex);
            _s32AUDIOMutex = -1;
        }

        if (_s32AUDIOMutexReboot != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutexReboot);
            _s32AUDIOMutexReboot = -1;
        }

        if (_s32MutexLoadCode != -1)
        {
            OS_DELETE_MUTEX(_s32MutexLoadCode);
            _s32MutexLoadCode = -1;
        }

        if (_s32AUDIOMutexIDMA != -1)
        {
            OS_DELETE_MUTEX(_s32AUDIOMutexIDMA);
            _s32AUDIOMutexIDMA = -1;
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Mutex_DeInit()
/// @brief \b Function \b Description: This routine is the deletion of Mutex for Audio module.
/// @param <IN>        \b NONE :
/// @param <OUT>       \b NONE :
/// @param <RET>       \b  MS_BOOL : TRUE - Success, FALSE - Fail
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Mutex_DeInit(void)
{
    MS_BOOL ret = TRUE;

    if ((_s32AUDIOMutex == -1) &&
        (_s32AUDIOMutexReboot == -1) &&
        (_s32MutexLoadCode == -1) &&
        (_s32AUDIOMutexIDMA == -1))
    {
        return ret;
    }

    if (_s32AUDIOMutex != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutex) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutex = -1;
    }

    if (_s32AUDIOMutexReboot != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutexReboot) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for Reboot failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutexReboot = -1;
    }

    if (_s32MutexLoadCode != -1)
    {
        if (OS_DELETE_MUTEX(_s32MutexLoadCode) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for Load DSP failed!\n");
            ret = FALSE;
        }
        _s32MutexLoadCode = -1;
    }

    if (_s32AUDIOMutexIDMA != -1)
    {
        if (OS_DELETE_MUTEX(_s32AUDIOMutexIDMA) != TRUE)
        {
            HALAUDIO_ERROR("Error! Delete Mutex for IMDA failed!\n");
            ret = FALSE;
        }
        _s32AUDIOMutexIDMA = -1;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetChipType()  @@Cathy
/// @brief \b Function \b Description: This routine is used to get the IC version for API.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  AU_CHIP_TYPE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_CHIP_TYPE HAL_AUDIO_GetChipType(void)
{
     return AU_CHIP_MUNICH;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_MMIO_BASE()  @@Cathy
/// @brief \b Function \b Description: This routine is used to set MMIO base
/// @param <IN>        \b u32_MMIO_MapBase    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b      :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_Set_MMIO_BASE(MS_U32 u32_MMIO_MapBase)
{
    _gMIO_MapBase = u32_MMIO_MapBase; // Get the absolute MMIO address
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsReadByte  @@Cathy
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 : 8-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_AbsReadByte(MS_U32 u32RegAddr)
{
    return (_AU_AbsReadByte((u32RegAddr-0x100000)));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_ReadByte  @@Cathy
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 : 8-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_ReadByte(MS_U32 u32RegAddr)
{
    return (_AU_AbsReadByte(u32RegAddr+0x010000));  // Add  audio bank offset
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsReadReg  @@Cathy
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16 : 16-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_AbsReadReg(MS_U32 u32RegAddr)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    return _AU_AbsRead2Byte((u32RegAddr-0x100000));  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_ReadReg  @@Cathy
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16 : 16-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_ReadReg(MS_U32 u32RegAddr)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    return (_AU_AbsRead2Byte(u32RegAddr+0x010000));  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteReg
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    _AU_AbsWrite2Byte((u32RegAddr-0x100000),u16Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteReg  @@Cathy
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    _AU_AbsWrite2Byte((u32RegAddr+0x010000),u16Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
   _AU_AbsWriteByte((u32RegAddr-0x100000), u8Val); // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteByte  @@Cathy
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
   _AU_AbsWriteByte((u32RegAddr+0x010000), u8Val); // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteMaskByte  @@Morris
/// @brief \b Function  \b Description: Mask write 8-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    _AU_AbsWriteMaskByte((u32RegAddr-0x100000), u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteMaskByte  @@Cathy
/// @brief \b Function  \b Description: Mask write 8-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    _AU_AbsWriteMaskByte((u32RegAddr+0x010000), u8Mask, u8Val);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_AbsWriteMaskReg
/// @brief \b Function  \b Description: Mask write 16-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_AbsWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val)
{
    MS_U16 u16RegVal;

    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    u16RegVal = _AU_AbsRead2Byte((u32RegAddr-0x100000));
    u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
    _AU_AbsWrite2Byte((u32RegAddr-0x100000), u16RegVal);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUDIO_WriteMaskReg  @@Cathy
/// @brief \b Function  \b Description: Mask write 16-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val)
{
    MS_U16 u16RegVal;

    u32RegAddr = (u32RegAddr & 0xFFFFFFFE);  // Reg Address must be even
    u16RegVal = HAL_AUDIO_ReadReg(u32RegAddr);
    u16RegVal = ((u16RegVal & (~(u16Mask))) | (u16Val & u16Mask));
    HAL_AUDIO_WriteReg(u32RegAddr, u16RegVal);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _HAL_AUDIO_Write4Byte  @@Cathy
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+0x010000] =(MS_U16) (u32Val&0xFFFF);
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+2+0x010000] =(MS_U16) (u32Val>>16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ReadMailBox()    @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to read the Dec or SE DSP mail box value
/// @param <IN>        \b bDspType    : 0 --DEC     1 -- SE
/// @param <IN>        \b u8ParamNum    : Mail box address
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  U16    : Mail Box value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{
    MS_U16 u16Tmp1, u16Tmp2;
    MS_U32 i, u32MailReg;


    for (i=0; i<1000; i++)
    {
        if(bDspType==DSP_SE)
        {
            if(u8ParamNum<8)
            {
                u32MailReg = 0x2D70+(MS_U32)u8ParamNum * 2;
            }
            else
            {
                u32MailReg = REG_SE_D2M_MAIL_BOX_BASE+(MS_U32)(u8ParamNum-8) * 2;
            }
            u16Tmp1 = HAL_AUDIO_ReadReg(u32MailReg);
            u16Tmp2 = HAL_AUDIO_ReadReg(u32MailReg);
        }
        else
        {
            if(u8ParamNum<8)
            {
                u32MailReg = 0x2D60+(MS_U32)u8ParamNum * 2;
            }
            else
            {
                u32MailReg = REG_DEC_D2M_MAIL_BOX_BASE+(MS_U32)(u8ParamNum-8) * 2;
            }
            u16Tmp1 = HAL_AUDIO_ReadReg(u32MailReg);
            u16Tmp2 = HAL_AUDIO_ReadReg(u32MailReg);
        }
        if(u16Tmp1==u16Tmp2)
        {
          return u16Tmp1;
        }
    }

    printf("Read Mailbox fail! \r\n");
    return 0;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WriteDecMailBox()   @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to write Dec-DSP mail box
/// @param <IN>        \b bDspType    : 0 --DEC     1 -- SE
/// @param <IN>        \b u8ParamNum    : Mail box address
/// @param <IN>        \b u16Data    :  value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{
    MS_U32 u32MailReg;

    if(bDspType==DSP_SE)
    {
        u32MailReg = REG_SE_M2D_MAIL_BOX_BASE + (MS_U32)u8ParamNum* 2;
        HAL_AUDIO_WriteReg(u32MailReg, u16Data);
    }
    else
    {
        u32MailReg = REG_DEC_M2D_MAIL_BOX_BASE + (MS_U32)u8ParamNum* 2;
        HAL_AUDIO_WriteReg(u32MailReg, u16Data);
    }
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO Initialize Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WriteInitTable()  @@Cathy
/// @brief \b Function \b Description:  This function is used to write initial register table(8-bit mode)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void  HAL_AUDIO_WriteInitTable(void)
{
    MS_U16 i = 0;
    MS_U16 j = 0;

    while( !((ResetDMIOTbl[j].u32Addr == 0xFFFFFF) && (ResetDMIOTbl[j].u16Mask == 0)) )
    {
        if((ResetDMIOTbl[j].u32Addr == 0xFFFFFF))
        {
            if(ResetDMIOTbl[j].u16Mask == 1)
            {
                if(ResetDMIOTbl[j].u16Value!=0)
                    MsOS_DelayTask((MS_U32)(ResetDMIOTbl[j].u16Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskReg(ResetDMIOTbl[j].u32Addr, ResetDMIOTbl[j].u16Mask, ResetDMIOTbl[j].u16Value);
        }

        j++;
    }

    while( !((AudioInitTbl[i].u32Addr == 0xFFFFFF) && (AudioInitTbl[i].u8Mask == 0)) )
    {
        if((AudioInitTbl[i].u32Addr == 0xFFFFFF))
        {
            if(AudioInitTbl[i].u8Mask == 1)
            {
                if(AudioInitTbl[i].u8Value!=0)
                    MsOS_DelayTask((MS_U32)(AudioInitTbl[i].u8Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskByte(AudioInitTbl[i].u32Addr, AudioInitTbl[i].u8Mask, AudioInitTbl[i].u8Value);
        }

        i++;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description:  This function is used to set power on DAC sequence before setting init table.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_WritePreInitTable(void)
{
    MS_U16 i = 0;

    if ((HAL_AUDIO_ReadByte(REG_AUDIO_CODEC_CFG7+1) & 0x01) == 0x01) {
        printf("Audio has initialized, don't need to do Pre init again\n");
        return;
    }

    while( !((AudioPreInitTbl_Clock_0[i].u32Addr == 0xFFFFFF) && (AudioPreInitTbl_Clock_0[i].u8Mask == 0)) )
    {
        if((AudioPreInitTbl_Clock_0[i].u32Addr == 0xFFFFFF))
        {
            if(AudioPreInitTbl_Clock_0[i].u8Mask == 1)
            {
                if(AudioPreInitTbl_Clock_0[i].u8Value!=0)
                    MsOS_DelayTask((MS_U32)(AudioPreInitTbl_Clock_0[i].u8Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskByte(AudioPreInitTbl_Clock_0[i].u32Addr, AudioPreInitTbl_Clock_0[i].u8Mask, AudioPreInitTbl_Clock_0[i].u8Value);
        }
        i++;
    }

    i = 0;
    while( !((ResetDMIOTbl[i].u32Addr == 0xFFFFFF) && (ResetDMIOTbl[i].u16Mask == 0)) )
    {
        if((ResetDMIOTbl[i].u32Addr == 0xFFFFFF))
        {
            if(ResetDMIOTbl[i].u16Mask == 1)
            {
                if(ResetDMIOTbl[i].u16Value!=0)
                    MsOS_DelayTask((MS_U32)(ResetDMIOTbl[i].u16Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskReg(ResetDMIOTbl[i].u32Addr, ResetDMIOTbl[i].u16Mask, ResetDMIOTbl[i].u16Value);
        }
        i++;
    }

    i = 0;
    while( !((AudioPreInitTbl_Clock_1[i].u32Addr == 0xFFFFFF) && (AudioPreInitTbl_Clock_1[i].u8Mask == 0)) )
    {
        if((AudioPreInitTbl_Clock_1[i].u32Addr == 0xFFFFFF))
        {
            if(AudioPreInitTbl_Clock_1[i].u8Mask == 1)
            {
                if(AudioPreInitTbl_Clock_1[i].u8Value!=0)
                    MsOS_DelayTask((MS_U32)(AudioPreInitTbl_Clock_1[i].u8Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskByte(AudioPreInitTbl_Clock_1[i].u32Addr, AudioPreInitTbl_Clock_1[i].u8Mask, AudioPreInitTbl_Clock_1[i].u8Value);
        }
        i++;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description:  This function is used to enable earphone low power stage.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_EnaEarphone_LowPower_Stage(void)
{
    HAL_AUDIO_WriteMaskByte(0x2CEE, 0xFF, 0x03);      // Disable BB by-pass
#ifndef CONFIG_MBOOT
    HAL_AUDIO_WriteMaskByte(0x2CEF, 0xFF, 0x03);      // Disable BB by-pass
#endif
    HAL_AUDIO_WriteMaskByte(0x2CEC, 0xFF, 0x00);      // Disable BB by-pass
    HAL_AUDIO_WriteMaskByte(0x2CED, 0xFF, 0x18);      // Disable BB by-pass
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_WritePostInitTable()
/// @brief \b Function \b Description:  This function is used to enable earphone high driving stage.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void HAL_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetPowerOn()    @@Need_Modify
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param <IN>        \b bPower_on    : TRUE --power on
///                                      FALSE--power off
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPowerOn(MS_BOOL bPower_on)
{
    DBG_AUDIO("HAL_AUDIO_SetPowerOn(%x)\r\n", bPower_on);
    if(bPower_on)
    {
       HAL_AUDIO_WriteMaskByte(REG_D2M_MAILBOX_SE_POWERCTRL, 0x02, 0x00);       // DSP power up command, DO NOT touch bit3
       HAL_AUDIO_WriteMaskByte(0x2B42, 0xFF, 0x00);      // Disable BB by-pass
    }
    else
    {
        HAL_AUDIO_DeInitialVars();

        HAL_AUDIO_WriteMaskByte(REG_D2M_MAILBOX_SE_POWERCTRL, 0x02, 0x02);       // DSP power down command
        AUDIO_DELAY1MS(100);

        // Disable MIU Request for DEC-DSP
        HAL_MAD_Dis_MIUREQ();

        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x80, 0x00);      // SPDIF power down
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ADCInit()    @@Cathy
/// @brief \b Function \b Description:  This routine is ADC relational register Init.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_ADCInit(void)
{
    HAL_AUDIO_WriteMaskByte(0x2CDA, 0x03, 0x00);    // power on ADC0 & ADC1
    HAL_AUDIO_WriteMaskByte(0x2CEE, 0x03, 0x03);    //enable ADC dither
    HAL_AUDIO_WriteMaskByte(0x2CE3, 0x03, 0x00);    // power on ADC PGA
    HAL_AUDIO_WriteMaskByte(0x2CE5, 0xFC, 0x00);    //ADC0 & ADC1 PGAain=0dB
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SwResetMAD()  @@Cathy
/// @brief \b Function \b Description:  This function is used to software reset MAD
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SwResetMAD(void)
{
    MS_U16 j = 0;

    while( !((ResetDMIOTbl[j].u32Addr == 0xFFFFFF) && (ResetDMIOTbl[j].u16Mask == 0)) )
    {
        if((ResetDMIOTbl[j].u32Addr == 0xFFFFFF))
        {
            if(ResetDMIOTbl[j].u16Mask == 1)
            {
                if(ResetDMIOTbl[j].u16Value!=0)
                    AUDIO_DELAY1MS((MS_U32)(ResetDMIOTbl[j].u16Value));
            }
        }
        else
        {
            HAL_AUDIO_AbsWriteMaskReg(ResetDMIOTbl[j].u32Addr, ResetDMIOTbl[j].u16Mask, ResetDMIOTbl[j].u16Value);
        }

        j++;
    }

    if ((HAL_AUDIO_ReadByte(REG_AUDIO_CODEC_CFG7+1) & 0x01) == 0x01) {
        printf("Audio has initialized, don't need to do SwReset again\n");
        return;
    }

    /* Audio software engine reset */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0D);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0F);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x0D);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SOFT_RESET, 0x0F, 0x00);
    AUDIO_DELAY1MS(1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ResetDSP()  @@Cathy
/// @brief \b Function \b Description:  This function is used to reset DSP.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_ResetDSP(void)
{
    HAL_AUDIO_DecWriteByte(REG_DEC_IDMA_CTRL0, 0x02);     // Reset DSP
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_DecWriteByte(REG_DEC_IDMA_CTRL0, 0x03);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecDspISR()   @@Need_Modify
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DecDspISR(void) //Cathy need modify
{
    //no event use this function now
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SeDspISR()  @@Need_Modify
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SeDspISR(void)
{
    MS_U8 se_ISR_cmdType;

    se_ISR_cmdType = HAL_AUDIO_ReadByte(REG_D2M_MAILBOX_SE_ISRCMD);

    /* add for PIP ASND Decode */
    if ( se_ISR_cmdType == 0x03 )
    {
        if (g_AudioVars2 != NULL)
        {
            g_AudioVars2->g_bSePlayFileFlag = TRUE;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetPlayFileFlag()  @Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPlayFileFlag(MS_BOOL bDspType, MS_BOOL bSet)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bDspType == DSP_DEC)
    {
        g_AudioVars2->g_bDecPlayFileFlag = bSet;
    }
    else
    {
        g_AudioVars2->g_bSePlayFileFlag = bSet;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetPlayFileFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetPlayFileFlag(MS_BOOL bDspType)
{
        MS_U32      es_rdPtr, es_wrPtr;
        MS_S32      es_level, es_bufSz, es_freeSpace;
        MS_U32      es_reqSize;
        MS_U8        r2_dec_id;

        if(bDspType ==DSP_DEC)
            r2_dec_id = ADEC1;
        else if(bDspType ==DSP_SE)
            r2_dec_id = ADEC2;
        else
            r2_dec_id = ADEC1; //default case

        es_rdPtr = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_ES_RD_PTR, r2_dec_id);
        es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM( R2_SHM_PARAM_ES_WR_PTR, r2_dec_id, 0 );
        es_reqSize = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, r2_dec_id);

        es_bufSz = ES1_DRAM_SIZE;
        es_level = es_wrPtr - es_rdPtr;

        if (es_level < 0)
            es_level += es_bufSz;

        es_freeSpace = es_bufSz - es_level;

        if ( es_freeSpace > es_reqSize )
            return TRUE;
        else
            return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetEncodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetEncodeDoneFlag(MS_BOOL bSet)
{
    if(bSet)
       g_bEncodeDoneFlag= 1;
    else
       g_bEncodeDoneFlag = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetEncodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the Encoder flag status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetEncodeDoneFlag(void)
{
    return g_bEncodeDoneFlag;
}

/////////////////////////////////////////////////////////////////////////////////
///                                                                                                                                   ///
///        AUDIO I/O Config Relational Hal Function                                                               ///
///                                                                                                                                  ///
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetPathGroup()
/// @brief \b Function \b Description: This routine is used to get Audio path group type .
/// @param output      \b : Audio output path-group type
////////////////////////////////////////////////////////////////////////////////
AUDIO_PATH_GROUP_TYPE HAL_AUDIO_GetPathGroup(void)
{
    // AUDIO_PATH_GROUP_1 : U3, Janus
    // AUDIO_PATH_GROUP_2 : T3, U4, T8, T9, T12, T13, J2
    return(AUDIO_PATH_GROUP_2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetNormalPath()  @@ No used in T3 @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input and Output.
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <IN>        \b input    : Audio input type
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetNormalPath(AUDIO_PATH_TYPE u8Path, AUDIO_INPUT_TYPE input, AUDIO_OUTPUT_TYPE output)
{
    u8Path=u8Path;
    input=input;
    output=output;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetInputPath()   @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Input .
/// @param <IN>        \b input    : Audio input type
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetInputPath(AUDIO_INPUT_TYPE input , AUDIO_PATH_TYPE u8Path)
{
    MS_U32 u32path_reg;
    MS_U8  u8input_src, u8input_idx, u8temp, path;
    AUDIO_INPUT_TYPE	 input1; // define this in order to do more check for ATV input source.
    input1 = input;

    if((input == AUDIO_NULL_INPUT) || (u8Path ==AUDIO_PATH_NULL))
    {
        return;
    }

    u8input_src = LONIBBLE(input);
    u8input_idx = HINIBBLE(input);

    if (u8input_idx == 7)	// if input source is ATV, change input depends on the definition of SIF_DSP_TYPE instead of the ATV input mux at boarddef
    {
        #if (SIF_DSP_TYPE == 0)
        input1 = 	AUDIO_DSP1_SIF_INPUT;
        #else
        input1 = 	AUDIO_DSP4_SIF_INPUT;
        #endif
        u8input_src = LONIBBLE(input1);
    }

    if( u8Path == AUDIO_PATH_MAIN )
    {
        if (u8input_src == AUDIO_ADC_INPUT || u8input_src == AUDIO_ADC2_INPUT)
        {
            HAL_SOUND_EnableDcRemove(TRUE);
        }
        else
        {
            HAL_SOUND_EnableDcRemove(FALSE);
        }
    }

    path=(MS_U8)u8Path;

    /* save main speaker audio input */
    if( u8Path == AUDIO_PATH_MAIN )
	{
       g_audioSrcType = input1;
	}
    /* save sub channel audio input */
    else if( u8Path == AUDIO_PATH_6 )
	{
       g_audioSubSrcType = input1;
	}

    if( path >= (sizeof(u32PathArray)/sizeof(u32PathArray[0])))
    {
            DBG_AUDIO(printf("DSP is not support more than CH8\n"));
            return;
    }

    u32path_reg = u32PathArray[path];

    // Set input
    switch(u8input_src)
    {
        case AUDIO_DSP1_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x80);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG, 0x07,u8input_idx);  // Set main parser source
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER2_CFG, 0x07,u8input_idx);  // Set AD parser source
         break;

        case AUDIO_DSP2_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x81);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER2_CFG, 0x07,u8input_idx);
            break;

        case AUDIO_DSP3_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x82);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x07,u8input_idx);
            break;

        case AUDIO_DSP4_INPUT:
            /*
             * A patch here!
             * Just a temporary solution to hardcode channel 7 to 0x90 for HW DMA Reader2,
             */
            if(u8input_idx == 0x7)
            {
                HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x83);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER4_CFG, 0x07,u8input_idx);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x90);
            }
            break;

        case AUDIO_ADC_INPUT:
            if(u8input_idx==0x0A)
                u8temp=0x40;
            else if(u8input_idx==0x0B)
                u8temp=0x50;
            else
                u8temp = (u8input_idx<<4);
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x88);
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0xF0, u8temp );
            break;

        case AUDIO_ADC2_INPUT:
            if(u8input_idx==0x0A)
                u8temp=0x04;
            else if(u8input_idx==0x0B)
                u8temp=0x05;
            else
                u8temp = u8input_idx;
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x89);
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0x0F, u8temp );
            break;

        case AUDIO_SPDIF_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x86);
            break;

        case AUDIO_I2S_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x85);
            break;

        case AUDIO_HDMI_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x84);
            break;

        case AUDIO_DMA_INPUT:
            HAL_AUDIO_WriteMaskByte(u32path_reg, 0x9F, 0x8F);
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetOutputPath()    @@Need_Modify
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio DSP channel
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetOutputPath(AUDIO_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output)
{
    u8Path=u8Path;
    u8Output=u8Output;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetInternalPath()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the topalogy for Audio Output.
/// @param <IN>        \b u8Path    : Audio internal path
/// @param <IN>        \b output    : Audio output type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output)
{
    MS_U8   path;
                                            // OUT0, OUT1, OUT2 , OUT3 , S/PDIF, I2S,   HP , Dummy, Dummy, HDMI
    MS_U8   BalanceBitMap[10]={   0,       1,      2,         3,         8,        4,     1,      7,         7,         10        };

    if(u8Path >= INTERNAL_MAX_NUM)
        return;

    path = (MS_U8)u8Path;

    if(u8Path==INTERNAL_PCM_SE)  // Only speaker use this path
    {
      g_BalanceMask=g_BalanceMask|(0x01<<BalanceBitMap[(int)u8Output]);  // Enable balance mask
    }

    if((HAL_AUDIO_ReadByte(M2S_MBOX_BALANCE_EN+1)&0x80)!=0)
       HAL_AUDIO_WriteMaskReg(M2S_MBOX_BALANCE_EN, g_BalanceMask, 0xFFFF); // Balance enable
    else
       HAL_AUDIO_WriteMaskReg(M2S_MBOX_BALANCE_EN, g_BalanceMask, 0x0000);// Balance disable

    // Set output
      switch(u8Output)
    {
        case AUDIO_AUOUT0_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1, 0x0F, path);
            break;

        case AUDIO_AUOUT1_OUTPUT:
        case AUDIO_HP_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1, 0xF0, (path<<4));
            break;

        case AUDIO_AUOUT2_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1+1, 0x0F, path);
            break;

        case AUDIO_AUOUT3_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL1+1, 0xF0, (path<<4));
            break;

        case AUDIO_I2S_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL2, 0x0F, path);
            break;

        case AUDIO_SPDIF_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL3+1, 0x0F, path); // Fix PCM in
            break;

        case AUDIO_HDMI_OUTPUT:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_INPUT_MUX_SEL3, 0x0F, path);
            break;

        default:
            break;
    }

    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_SPK_OutChannel, g_AudioVars2->AudioPathInfo.SpeakerOut, DSP_MEM_TYPE_PM); // INFO DSP SPK Output channel
    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_HP_OutChannel, g_AudioVars2->AudioPathInfo.HpOut, DSP_MEM_TYPE_PM);       // INFO DSP HP Output channel

}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_FwTriggerDSP()    @@Need_Modify
/// @brief \b Function \b Description:  This routine send a PIO11 interrupt to DSP with a command on 0x2D34.
/// @param <IN>        \b cmd    :    0xF0-- for MHEG5 file protocol
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)  //Cathy need modify
{
    u16Cmd = u16Cmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspBootOnDDR()
/// @brief \b Function \b Description:  Enable DSP load / reload code from DDR
/// @param <IN>        \b bEnable    : 0 -- load code from FLASH
///                                      1 -- load code from DDR
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DspBootOnDDR(MS_BOOL bEnable)
{
    g_bAudio_loadcode_from_dram = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetIsDtvFlag()  @@Cathy
/// @brief \b Function \b Description:  Report the decoder type is ATV or DTV
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b g_bIsDTV: 0 -> ATV , 1 -> DTV
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetIsDtvFlag(void)
{
    return g_bIsDTV;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetIsDtvFlag()  @@Cathy
/// @brief \b Function \b Description:  Report the decoder type is ATV or DTV
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b g_bIsDTV: 0 -> ATV , 1 -> DTV
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetIsDtvFlag(MS_BOOL bIsDTV)
{
    g_bIsDTV=bIsDTV;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Dec_Status()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to read the Decoder status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Decoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Dec_Status(void)
{
    return(HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Enc_Status()  @@Kochien.Kuo
/// @brief \b Function \b Description:  This routine is used to read the Encoder status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Encoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Enc_Status(void)
{
    return(HAL_AUDIO_ReadByte(REG_DEC_ENCODE_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SE_Status()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to read the SE status.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : Decoder Status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_Se_Status(void)
{
    return(HAL_AUR2_ReadByte(REG_R2_DECODE2_CMD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_Fading()  @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to set the Fading response time
/// @param <IN>        \b u32VolFading    : Fading response time parameter
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_Set_Fading(MS_U32 u32VolFading)
{
     HAL_MAD2_Write_DSP_sram(0x114C, u32VolFading, DSP_MEM_TYPE_PM);        //need touch
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BT_SetUploadRate()   @@Need_Modify
/// @brief \b Function \b Description:  According Blue tooth upload path, for different sampling rate setting the Synthesizer.
/// @param <IN>        \b bEnable    : 0 -- Disable Blue Tooth upload
///                                      1 -- Enable Blue Tooth upload
/// @param <IN>        \b u8Samprate: Sampling Rate
///                                    0--no change
///                                    1--48KHz
///                                    2--44KHz
///                                    3--32KHz
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BT_SetUploadRate(MS_BOOL bEnable,MS_U8 u8Samprate)
{
    u8Samprate &= 0x0003;

    if(bEnable)
    {
       HAL_AUDIO_DecWriteByte(0x2C5A, 0x55);
       HAL_AUDIO_DecWriteMaskByte(0x2D6C,0x10,0x10);
       HAL_AUDIO_DecWriteMaskByte(0x2D34,0x03,u8Samprate);
    }
    else
    {
       HAL_AUDIO_DecWriteByte(0x2C5A, 0);
       HAL_AUDIO_DecWriteMaskByte(0x2D6C,0x10,0);
       HAL_AUDIO_DecWriteMaskByte(0x2D34,0x03,0);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BT_SetBufferCounter()  @@Need_Modify
/// @brief \b Function \b Description:  Set the DDR buffer according the sampling rate and the frame time
/// @param <IN>        \b u32Counter    : if the sampling rate is 48KHz, the frame time is 40ms
///                            ==> the frame buffer size is 48000*0x04*2 (L/R) *2(Bytes/sample) = 0x1E00
///                                 the counter is 0x1E00/8 = 960 (For 1*Burst DMA)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BT_SetBufferCounter(MS_U32 u32Counter)
{
    u32Counter &= 0x00FFFFFF;
    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_btFrameSize, u32Counter, DSP_MEM_TYPE_PM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    //this funcion is removed from DSP
    UNUSED(bEnable);
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_USBPCM_GetFlag()
{
    //this funcion is removed from DSP
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void    HAL_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    //this funcion is removed from DSP
    UNUSED(bEnable);
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{
    //this funcion is removed from DSP
    HALAUDIO_ERROR("%s() : Error! unsupported funcion !!!\n", __FUNCTION__);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ I2S_SetMode()    @@Need_Modify
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param <IN>        \b Mode    : MCLK , WORD_WIDTH , SOURCE_CH , FORMAT
/// @param <IN>        \b Value    : Please reference the register table 0x2C8C & 0x2C8D .
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Val)
{
   switch(u8Mode)
   {
     case AUDIO_I2S_MCLK:         //0x2C8C[6:4]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x70,u8Val<<4);
       break;

     case AUDIO_I2S_WORD_WIDTH:   //0x2C8C[2:0]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x07,u8Val);
       break;

     case AUDIO_I2S_FORMAT:      //0x2C8C[3]
       HAL_AUDIO_WriteMaskByte(REG_AUDIO_I2S_OUT1_CFG,0x08,u8Val<<3);
       break;

     case AUDIO_I2S_SOURCE_CH:
      // No need ; Only select Group A in T3 .
       break;

     case AUDIO_I2S_RXMODE:
         if(u8Val==I2S_SLAVE_MODE)      //slave mode
         {   //0x2C8C[2], 2CAE[13], 2CCE[15:13][1:0]
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x04, 0x00);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG5+1, 0xA0, 0x00);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST+1, 0xE0, 0x20);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST, 0x03, 0x00);
         }
         else      //master mode
         {
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x04, 0x04);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG5+1, 0xA0, 0xA0);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST+1, 0xE0, 0xC0);
             HAL_AUDIO_WriteMaskByte(REG_AUDIO_ASIF_TST, 0x03, 0x03);
         }
         break;

        case AUDIO_I2S_TXMODE:
            // Tx always Master Mode;
            break;

     default:
       break;
   }
}


static void HAL_AUDIO_DigitalOut_Set_Encode(AUDIO_ENCODE_TYPE eType, MS_BOOL bEnable)
{
    switch(eType)
    {
        case DD_DDCO:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x01, 0x1 );
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x01, 0x0 );
            }
            break;

        case DD_DDENCODE:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x02, 0x2 );
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(0x2E95, 0x02, 0x0 );
            }
            break;

        case DTS_ENCODE_2CH:
        case DTS_ENCODE_MULTI:
        case MP3_ENCODE:
        default:
            HALAUDIO_ERROR("%s() - Choosen Encoder not exist\n",__FUNCTION__);
            break;
    }
}

static void HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(DIGITAL_OUTPUT_TYPE ePath, AUDIO_DSP_ID eDspID)
{
    switch (ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        case DIGITAL_HDMI_ARC_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        case DIGITAL_HDMI_OUTPUT:
            {
                switch(eDspID)
                {
                    case AUDIO_DSP_ID_R2:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x4);
                        break;
                    case AUDIO_DSP_ID_SND:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x8);
                        break;
                    case AUDIO_DSP_ID_DEC:
                        HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0xC, 0x0);
                        break;
                    default:
                        HALAUDIO_ERROR("%s() - DEC ID %d not exist\n",__FUNCTION__,eDspID);
                        break;
                }
            }
            break;

        default:
            HALAUDIO_ERROR("%s() - Digital Path %d not exist\n",__FUNCTION__,ePath);
            break;
    }

}

static void HAL_AUDIO_DigitalOut_NPCMEnable(DIGITAL_OUTPUT_TYPE ePath, MS_BOOL bEnable)
{

    switch(ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x0); /* PCM Mode */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x00); /* Tx1 Grp A in */
            }
            break;

        case DIGITAL_HDMI_ARC_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            }
            break;

        case DIGITAL_HDMI_OUTPUT:
            if(bEnable == TRUE)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x2);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x0C, 0x00); /* Select DSP1 */
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x2, 0x0);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            }
            break;

        default:
            HALAUDIO_ERROR("%s() - Digital Path %d not exist\n",__FUNCTION__,ePath);
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b u8Spdif_mode    :
///                                    bit[0] = 0: spdif enable,   1: spdif disable (Se-DSP)
///                                    bit[1] = 0: PCM mode,     1: non-PCM mode
///                                    bit[2] = 1: non-PCM NULL Payload
/// @param <IN>        \b u8Input_src  0 : DTV
///                                    1 : ATV
///                                    2 : HDMI
///                                    3 : ADC
///                                    4 : CardReader
///                                    5 : SPDIF
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
static void HAL_AUDIO_DigitalOut_SetDataPath(DIGITAL_OUTPUT_TYPE ePath, DigitalOutSetting_t *stDigitalOutSetting)
{
    Digital_Out_Channel_Status_t stDigitalChannelStatus;
    memset(&stDigitalChannelStatus, 0x00, sizeof(Digital_Out_Channel_Status_t));

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        _HAL_AUDIO_SPDIF_SetMute(TRUE);
    }
    else if (ePath == DIGITAL_HDMI_ARC_OUTPUT)
    {
        //TODO: Mute HDMI , ARC
    }
    else if (ePath == DIGITAL_HDMI_OUTPUT)
    {

    }

    HAL_AUDIO_DigitalOut_GetChannelStatus(ePath, &stDigitalChannelStatus);

    switch (stDigitalOutSetting->eDigitalOutfMode)
    {
        case DIGITAL_OUT_PCM:
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);

            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;
            break;

        case DIGITAL_OUT_DOLBYDIGITAL:
            if (stDigitalOutSetting->u8R2NonPcmSetting == 0x01)
            {
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, TRUE);
            }
            else if (stDigitalOutSetting->u8R2NonPcmSetting == 0x02)
            {
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
                HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, TRUE);
            }

            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_DTS:
            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_AAC_LC:
            stDigitalChannelStatus.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDENCODE, FALSE);
            HAL_AUDIO_DigitalOut_Set_Encode(DD_DDCO, FALSE);
            HAL_AUDIO_DigitalOut_NPCM_SrcSwitch(ePath, stDigitalOutSetting->u8NonPcmPath);
            HAL_AUDIO_DigitalOut_NPCMEnable(ePath, TRUE);
            break;

        case DIGITAL_OUT_NONE:
        default:
            HALAUDIO_ERROR("%s() - Digital Mode %d not exist\n",__FUNCTION__,stDigitalOutSetting->eDigitalOutfMode);
            break;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(ePath, &stDigitalChannelStatus);
    // Restore Digital out mode
    memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), &stDigitalOutSetting, sizeof(stDigitalOutSetting));

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        if (g_AudioVars2->g_SPDIF_MuteStatus == FALSE)
        {
            _HAL_AUDIO_SPDIF_SetMute(FALSE);
        }
    }
    else
    {
        //TODO: um-Mute HDMI , ARC
    }

}



//static MS_BOOL HAL_AUDIO_DigitalOut_GetDataSource(AUDIO_OUTPORT_SOURCE_TYPE ePath, AUDIO_DIGITAL_OUTPUT_TYPE *retOutputType, AUDIO_DSP_ID *retNcmPath)

static MS_BOOL HAL_AUDIO_DigitalOut_DetermineDataPath(DIGITAL_OUTPUT_TYPE ePath, DigitalOutSetting_t *stDigitalOutSegtting)
{
    AUDIO_DSP_CODE_TYPE SourceDspCodeType = AU_DVB_STANDARD_INVALID;
    AUDIO_DSP_CODE_TYPE Dsp1CodeType = AU_DVB_STANDARD_INVALID;
    AUDIO_DSP_CODE_TYPE Dsp2CodeType = AU_DVB_STANDARD_INVALID;

    stDigitalOutSegtting->eSourceType = g_AudioVars2->eAudioSource;
    Dsp1CodeType = g_AudioVars2->g_DspCodeType;
    Dsp2CodeType = g_AudioVars2->g_Dsp2CodeType;

    printf("%s() - eDigitalOutfMode  = %x, eNonPcmPath = %x , Source = %x, R2NonPcmSetting = %x\n\r", \
    __FUNCTION__, \
    stDigitalOutSegtting->eDigitalOutfMode, \
    stDigitalOutSegtting->u8NonPcmPath, \
    stDigitalOutSegtting->eSourceType, \
    stDigitalOutSegtting->u8R2NonPcmSetting);

    switch (stDigitalOutSegtting->eDigitalOutfMode)
    {
        case DIGITAL_OUT_PCM:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC;
            break;
        }

        case DIGITAL_OUT_DOLBYDIGITAL:
        {
            switch (stDigitalOutSegtting->eSourceType)
            {
                case E_AUDIO_INFO_HDMI_IN:
                {
                    if (bIsNonPCMInDec2)
                    {
                        if ((HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE)) &&
                            (Dsp2CodeType == AU_DVB2_STANDARD_AC3P_AD))
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_SND; /* DSP2 */
                        }
                    }
                    else if ((Dsp1CodeType == AU_DVB_STANDARD_AC3) ||
                        (Dsp1CodeType == AU_DVB_STANDARD_AC3P))
                    {
                        stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    }
                    break;
                }

                case E_AUDIO_INFO_DTV_IN:
                case E_AUDIO_INFO_MM_IN:
                case E_AUDIO_INFO_GAME_IN:
                {
                    SourceDspCodeType = Dsp1CodeType;

                    switch(SourceDspCodeType)
                    {
                        case AU_DVB_STANDARD_AAC:
                        case AU_DVB_STANDARD_MS10_DDT:
                        {
                            if ((!MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_MS10_DDT)) &&
                                 (!MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DDE)))
                            {
                                stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                            }
                            else
                            {
                                if ((HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1)&0xFFFF) < 32000)
                                {
                                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                                }
                                else
                                {
                                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_R2; /* R2 */
                                    stDigitalOutSegtting->u8R2NonPcmSetting = (SourceDspCodeType == AU_DVB_STANDARD_AAC) ? 0x01 : 0x02; /* DDCO : 0x2E95[0], DDEnc : 0x2E95[1] */
                                }
                            }

                            break;
                        }

                        case AU_DVB_STANDARD_MS10_DDC:
                        case AU_DVB_STANDARD_AC3:
                        case AU_DVB_STANDARD_AC3P:
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                            break;
                        }

                        default:
                        {
                            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                            break;
                        }

                    }

                    break;
                }

                case E_AUDIO_INFO_ATV_IN:
                case E_AUDIO_INFO_ADC_IN:
                case E_AUDIO_INFO_KTV_IN:
                default:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }
            }
            break;
        }

        case DIGITAL_OUT_DTS:
        {
            switch (stDigitalOutSegtting->eSourceType)
            {
                case E_AUDIO_INFO_HDMI_IN:
                {
                    if (bIsNonPCMInDec2)
                    {
                        if (Dsp2CodeType == AU_DVB2_STANDARD_DTS)  // Need check
                        {
                            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_SND; /* DSP2 */
                        }
                    }
                    else if (Dsp1CodeType == AU_DVB_STANDARD_DTS)
                    {
                        stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    }
                    break;
                }

                case E_AUDIO_INFO_MM_IN:
                case E_AUDIO_INFO_GAME_IN:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_DTS;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }

                default:
                {
                    stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
                    stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
                    break;
                }
            }

            break;
        }

        case DIGITAL_OUT_AAC_LC:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
            break;
        }

        case DIGITAL_OUT_NONE:
        default:
        {
            stDigitalOutSegtting->eDigitalOutfMode = DIGITAL_OUT_PCM;
            stDigitalOutSegtting->u8NonPcmPath = AUDIO_DSP_ID_DEC; /* DSP1 */
            break;
        }

    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b
/// @param <IN>        \b
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDigitalOut(DIGITAL_OUTPUT_TYPE ePath, AUDIO_DIGITAL_OUTPUT_TYPE eDigitalMode)
{

    HALAUDIO_CHECK_SHM_INIT;

    //TODO: add HDMI/ARC digital out status

    // Set Digital mode to Digital out Status structure
    DigitalOutSetting_t stTempDigitalOutStatus;
    stTempDigitalOutStatus.eDigitalOutfMode = eDigitalMode;
    stTempDigitalOutStatus.eSourceType = E_AUDIO_INFO_DTV_IN; // Temp initial value
    stTempDigitalOutStatus.u8NonPcmPath  = AUDIO_DSP_ID_ALL;
    stTempDigitalOutStatus.u8R2NonPcmSetting = 0x00;
    stTempDigitalOutStatus.eGroup = E_CONNECT_NULL;

    if (ePath == DIGITAL_SPDIF_OUTPUT)
    {
        stTempDigitalOutStatus.eGroup = g_AudioVars2->AudioOutputSourceInfo.SpdifOut;
    }
    else if (ePath == DIGITAL_HDMI_ARC_OUTPUT)
    {
        stTempDigitalOutStatus.eGroup = g_AudioVars2->AudioOutputSourceInfo.ArcOut;
    }

    //Determin Data Path
    HAL_AUDIO_DigitalOut_DetermineDataPath(ePath, &stTempDigitalOutStatus);

    // if setting is difference to previous, set digital out mode
    if ((stTempDigitalOutStatus.eDigitalOutfMode != g_AudioVars2->stDigitalOutSetting[ePath].eDigitalOutfMode) ||
        (stTempDigitalOutStatus.eSourceType != g_AudioVars2->stDigitalOutSetting[ePath].eSourceType) ||
        (stTempDigitalOutStatus.u8NonPcmPath != g_AudioVars2->stDigitalOutSetting[ePath].u8NonPcmPath) ||
        (stTempDigitalOutStatus.u8R2NonPcmSetting != g_AudioVars2->stDigitalOutSetting[ePath].u8R2NonPcmSetting) ||
        (stTempDigitalOutStatus.eGroup != g_AudioVars2->stDigitalOutSetting[ePath].eGroup))

    {
        printf("%s() - eDigitalOutfMode  = %x, eNonPcmPath = %x\n\r", \
                           __FUNCTION__, \
                           stTempDigitalOutStatus.eDigitalOutfMode, \
                           stTempDigitalOutStatus.u8NonPcmPath);

        HAL_AUDIO_DigitalOut_SetDataPath(ePath, &stTempDigitalOutStatus);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    MS_BOOL ret = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    switch (ePath)
    {
        case DIGITAL_SPDIF_OUTPUT:
        {

            // Copy protection
            //C bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS0 ,0x20, stChannelStatus->stCopyRight.CBit << 5);
            // L bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS1 ,0x01, stChannelStatus->stCopyRight.LBit);

            // PCM format
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS0 ,0xDF, stChannelStatus->u8PcmFormatControl);
            if(g_u32bDTSCD == 1)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS0 ,0x40, 0x00);
            }

            // Category
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS1 , 0xFE, stChannelStatus->u8Category);

            // Source number
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS2, 0xF0, stChannelStatus->u8SourceNumber);

            // Channel number
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS2, 0x0F, stChannelStatus->u8ChannelNumber);

            // Sampling rate
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0xF0, stChannelStatus->u8SamplingRate);

            // Clock precision
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0x0F, stChannelStatus->u8ClockPrecision);

            // Word Length
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS4 , 0xF0, stChannelStatus->u8WordLength);

            // Original sampling rate
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS4 , 0x0F, stChannelStatus->u8OriginalSamplingRate);

            if ((stChannelStatus->u8PcmFormatControl & SPDIF_CS_FORMAT_NONPCM) == SPDIF_CS_FORMAT_NONPCM)
            {
                if(g_u32bDTSCD == 1)
                {
                    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x80, 0x00);
                }
                else
                {
                    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x80, 0x80);
                }
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x80, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CS3 , 0xF0, SPDIF_CS_FS_48K);
            }

            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x00); //Tx1 Toggle bit[14]
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x40);
            HAL_AUDIO_WriteMaskByte(REG_SPDIFTX_CHANNEL_STATUS_TOGGLE, 0x40, 0x00);
            memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), stChannelStatus, sizeof(Digital_Out_Channel_Status_t));
            break;
        }

        case DIGITAL_HDMI_ARC_OUTPUT:
        {
            // Copy protection
            //C bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS0 ,0x20, stChannelStatus->stCopyRight.CBit << 5);
            // L bit
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS1 ,0x01, stChannelStatus->stCopyRight.LBit);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS0 ,0xDF, stChannelStatus->u8PcmFormatControl);
            if(g_u32bDTSCD == 1)
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS0 ,0x40, 0x00);
            }

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS1 , 0xFE, stChannelStatus->u8Category);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS2, 0xF0, stChannelStatus->u8SourceNumber);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS2, 0x0F, stChannelStatus->u8ChannelNumber);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0xF0, stChannelStatus->u8SamplingRate);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0x0F, stChannelStatus->u8ClockPrecision);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS4 , 0xF0, stChannelStatus->u8WordLength);

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS4 , 0x0F, stChannelStatus->u8OriginalSamplingRate);

            if ((stChannelStatus->u8PcmFormatControl & SPDIF_CS_FORMAT_NONPCM) == SPDIF_CS_FORMAT_NONPCM)
            {
                if(g_u32bDTSCD == 1)
                {
                    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x80, 0x00);
                }
                else
                {
                    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x80, 0x80);
                }
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x80, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CS3 , 0xF0, SPDIF_CS_FS_48K);
            }
            memcpy(&(g_AudioVars2->stDigitalOutChannelStatus[ePath]), stChannelStatus, sizeof(Digital_Out_Channel_Status_t));
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s() - Invalid SPDIF Path\n",__FUNCTION__);
            break;
        }
    }

   return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    MS_BOOL ret = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    if (stChannelStatus == NULL)
    {
        HALAUDIO_ERROR("%s() : stChannelStatus shouldn't be NULL !!!\n", __FUNCTION__);
    }
    else
    {
        memcpy(stChannelStatus, &(g_AudioVars2->stDigitalOutChannelStatus[ePath]), sizeof(Digital_Out_Channel_Status_t));
        ret = TRUE;
    }

    return ret;
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO SPDIF Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_HWEN()  @@Cathy
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param <IN>        \b bEnable:    0--Disable S/PDIF out
///                                    1--Enable S/PDIF out
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_HWEN(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x08, 0x00);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x08, 0x08);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_SetMute()  @@KH
/// @brief \b Function \b Description:  This routine is used to mute S/PDIF output(by DSP)
/// @param <IN>        \b bEnMute    :    0--Disable mute
///                                        1--Enable mute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_SetMute(MS_BOOL bEnMute)
{
    MS_BOOL spdif_mute_status = FALSE;
    MS_U8 spdif_timecnt = 0;

    HALAUDIO_CHECK_SHM_INIT;

    /* check current status according to SPDIF Software mute bit */
    spdif_mute_status = (((HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL) & 0x01) > 0) ? TRUE : FALSE);

    /* apply change only when it's a different status from current status */
    if (spdif_mute_status != bEnMute)
    {
        if (bEnMute == TRUE)
        {
            HAL_AUDIO_WriteMaskByte(REG_SOUND_SPDIF_VOL_FRAC+1, 0x80, 0x80);     //Fading-out mute
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x01, 0x01);        // Software mute
        }
        else
        {
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x01, 0x00);        // Software unmute
            for (spdif_timecnt = 0; spdif_timecnt < 30; spdif_timecnt++)
            {
                AUDIO_DELAY1MS(1);
            }
            HAL_AUDIO_WriteMaskByte(REG_SOUND_SPDIF_VOL_FRAC+1, 0x80, 0x00);     //Fading-in unmute
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_ByPassChannel()  @@Need_Modify
/// @brief \b Function \b Description:  ByPass the SPDIF channel (CH4)
/// @param <IN>        \b \b enable    :     TRUE --BYPASS CH4
///                                        FALSE--OPEN CH4
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_SPDIF_ByPassChannel(MS_BOOL bEnable)
{
    bEnable = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
/// @param <IN>        \b u8Spdif_mode    :
///                                    bit[0] = 0: spdif enable,   1: spdif disable (Se-DSP)
///                                    bit[1] = 0: PCM mode,     1: non-PCM mode
///                                    bit[2] = 1: non-PCM NULL Payload
/// @param <IN>        \b u8Input_src  0 : DTV
///                                    1 : ATV
///                                    2 : HDMI
///                                    3 : ADC
///                                    4 : CardReader
///                                    5 : SPDIF
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetMode(MS_U8 u8Spdif_mode, MS_U8 u8Input_src)
{
    // Multiple npcm source, still need total rework
    AUDIO_SOURCE_INFO_TYPE eAudioSource = -1;
    MS_U8 u8SpdifMode = ((u8Spdif_mode == SPDIF_OUT_NONE) ? SPDIF_OUT_PCM : u8Spdif_mode);
    MS_U8 u8R2SpdifMode = ((u8SpdifMode == SPDIF_OUT_PCM) ? 1 : 2);
    MS_U8 u8MainDecodeId = 0;
    MS_U8 MainDspCodeType = AU_DVB_STANDARD_INVALID;
    MS_U8 Dsp1CodeType = AU_DVB_STANDARD_INVALID;
    MS_U8 Dsp2CodeType = AU_DVB_STANDARD_INVALID;
    MS_U32 u32bDTSCD = 0;
    MS_U32 u32DTSTranscodeFlag = 0;
    MS_U8 Digital_Out_DD_Support = 0;
    MS_U8 Digital_Out_DDP_Support = 0;
    MS_U8 Digital_Out_DTS_Support = 0;
    MS_U8 Digital_Out_AAC_Support = 0;
    MS_U32 u32HDMITxSamplingRate = 0;
    MS_U32 u32SPDIFTxSamplingRate = 0;
    MS_U64 u64DecSamplingRate = 0;
    static MS_U32 u32bPreDTSCD = 0xFF;
    static MS_U32 u32PreHDMITxSamplingRate = 0;
    static MS_U32 u32PreSPDIFTxSamplingRate = 0;

    HALAUDIO_CHECK_SHM_INIT;

    UNUSED(u8Input_src);
    eAudioSource = g_AudioVars2->eAudioSource;
    Dsp1CodeType = g_AudioVars2->g_DspCodeType;
    Dsp2CodeType = g_AudioVars2->g_Dsp2CodeType;
    Digital_Out_DD_Support = g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_DD].support_codec;
    Digital_Out_DDP_Support = g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_DDP].support_codec;
    Digital_Out_DTS_Support = g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_DTS].support_codec;
    Digital_Out_AAC_Support = g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_AAC].support_codec;

    MainDspCodeType = Dsp1CodeType;

    //HALAUDIO_ERROR("%s: u8Spdif_mode = %d, eAudioSource = %d, Dsp1CodeType = %d, Dsp2CodeType = %d, u8Input_src = %d\r\n", __FUNCTION__, u8Spdif_mode, eAudioSource, Dsp1CodeType, Dsp2CodeType, u8Input_src);

    /* Configure SPDIF's Output Setting */
    switch(eAudioSource)
    {
        case E_AUDIO_INFO_DTV_IN:
        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
        {
            if (((eAudioSource == E_AUDIO_INFO_MM_IN) || (eAudioSource == E_AUDIO_INFO_GAME_IN)) &&
                (HAL_MAD_GetDecCmd(DVB_Audio_Decoder2) == AU_DVB_DECCMD_PLAYFRAME_GS))
            {
                MainDspCodeType = Dsp2CodeType;

                u8MainDecodeId = 1;   //DEC2
                u64DecSamplingRate = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC2_sampleRate);
            }
            else
            {
                u8MainDecodeId = 0;   //DEC1
                u64DecSamplingRate = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_sampleRate);
                if (HAL_MAD_GetDecCmd(DVB_Audio_Decoder1) == AU_DVB_DECCMD_STOP)
                {
                    u8SpdifMode  = SPDIF_OUT_PCM;
                    break;
                }
            }


            switch(MainDspCodeType)
            {
                case AU_DVB_STANDARD_AAC:
                case AU_DVB_STANDARD_MS10_DDT:
                {
                    if (g_AudioVars2->DolbyEncFlag != 1)
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                    }
#if (StereoAACOutputPCM)
                    else if (HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_acmod) == AUD_CH_MODE_STEREO)
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                    }
#endif
                    else
                    {
                        if ((u8SpdifMode == SPDIF_OUT_NONPCM) ||
                            (u8SpdifMode == SPDIF_OUT_TRANSCODE))
                        {
                            /* 0: default, 1: support, 2: not support */
                            if (Digital_Out_DD_Support == 2)
                            {
                                u8SpdifMode  = SPDIF_OUT_PCM;
                            }
                            else if ( u64DecSamplingRate < 4) //AAC SampleRate<16k
                            {
                                u8SpdifMode  = SPDIF_OUT_PCM;
                            }
                            else
                            {
                                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, u8MainDecodeId, 0, 0);
                            }
                        }
                        else if (u8SpdifMode == SPDIF_OUT_BYPASS) // AAC Bypass
                        {
                            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_AAC_BYPASS, u8MainDecodeId, 1, 0);
                        }
                    }
                    break;
                }

                case AU_DVB_STANDARD_AC3:
                case AU_DVB_STANDARD_AC3P:
                case AU_DVB_STANDARD_MS10_DDC:
                {
                    /* 0: default, 1: support, 2: not support */
                    if (Digital_Out_DD_Support == 2)
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                    }
                    /* 0: default, 1: support, 2: not support */
                    else if ((Digital_Out_DDP_Support == 1) && (u8SpdifMode != SPDIF_OUT_TRANSCODE))
                    {
                        HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 1, u8MainDecodeId);
                    }
                    else
                    {
                        HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, u8MainDecodeId);
                    }
                    break;
                }

                case AU_DVB_STANDARD_DTS:
                case AU_DVB_STANDARD_DTSLBR:
                {
                    /* 0: disable, 1: bypass/convert, 2: transcoder, 3: pcm */
                    u32DTSTranscodeFlag = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TRANSCODE_FLAG, u8MainDecodeId);
                    if (u32DTSTranscodeFlag == 3)
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                    }

                    if ((MainDspCodeType == AU_DVB_STANDARD_DTS) &&
                        (u32DTSTranscodeFlag == 1))
                    {
                        u32bDTSCD = HAL_MAD_GetDTSInfo(Audio_DTS_infoType_CD_MODE);
                    }

                    /* 0: default, 1: support, 2: not support */
                    if (Digital_Out_DTS_Support == 2)
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                    }

                    break;
                }

                default:
                {
                    u8SpdifMode  = SPDIF_OUT_PCM;
                    break;
                }
            }

            break;
        }

        case E_AUDIO_INFO_ATV_IN:
        case E_AUDIO_INFO_ADC_IN:
        case E_AUDIO_INFO_KTV_IN:
        {
            u8SpdifMode = SPDIF_OUT_PCM;
            break;
        }

        case E_AUDIO_INFO_HDMI_IN:
        {
            if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE))
            {
                MainDspCodeType = Dsp2CodeType;
                u8MainDecodeId = 1;   //DEC2
            }
            else
            {
                u8MainDecodeId = 0;   //DEC1
            }

            if (HAL_AUDIO_HDMI_NonpcmMonitor() == FALSE)
            {
                u8SpdifMode = SPDIF_OUT_PCM;
            }
            else
            {
                switch(MainDspCodeType)
                {
                    case AU_DVB_STANDARD_AC3:
                    case AU_DVB_STANDARD_AC3P:
                    case AU_DVB_STANDARD_MS10_DDC:
                    {
                        /* 0: default, 1: support, 2: not support */
                        if (Digital_Out_DD_Support == 2)
                        {
                            u8SpdifMode  = SPDIF_OUT_PCM;
                        }
                        /* 0: default, 1: support, 2: not support */
                        else if ((Digital_Out_DDP_Support == 1) && (u8SpdifMode != SPDIF_OUT_TRANSCODE))
                        {
                            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 1, u8MainDecodeId);
                        }
                        else
                        {
                            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_hdmiTxBypass_enable, 0, u8MainDecodeId);
                        }
                        break;
                    }

                    case AU_DVB_STANDARD_DTS:
                    {
                        /* 0: disable, 1: bypass/convert, 2: transcoder, 3: pcm */
                        u32DTSTranscodeFlag = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TRANSCODE_FLAG, u8MainDecodeId);
                        if (u32DTSTranscodeFlag == 3)
                        {
                            u8SpdifMode = SPDIF_OUT_PCM;
                        }

                        if ((MainDspCodeType == AU_DVB_STANDARD_DTS) &&
                            (u32DTSTranscodeFlag == 1))
                        {
                            u32bDTSCD = HAL_MAD_GetDTSInfo(Audio_DTS_infoType_CD_MODE);
                        }

                        /* 0: default, 1: support, 2: not support */
                        if (Digital_Out_DTS_Support == 2)
                        {
                            u8SpdifMode  = SPDIF_OUT_PCM;
                        }
                        break;
                    }

                    default:
                    {
                        u8SpdifMode  = SPDIF_OUT_PCM;
                        break;
                    }
                }
            }

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! invalid eAudioSource(%d)!\r\n", __FUNCTION__, eAudioSource);
            break;
        }
    }

    //HALAUDIO_ERROR("%s: MainDspCodeType = %d, u8SpdifMode = %d\r\n", __FUNCTION__, MainDspCodeType, u8SpdifMode);

    if(u8SpdifMode == SPDIF_OUT_PCM)
    {
        u32HDMITxSamplingRate = 48000;
        u32SPDIFTxSamplingRate = 48000;
    }
    else
    {
        u32HDMITxSamplingRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, u8MainDecodeId);
        u32SPDIFTxSamplingRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SPDIFTx_SMP_RATE, u8MainDecodeId);
    }

    if ((_HAL_AUDIO_SPDIF_GetMode() == u8SpdifMode) &&
        (g_AudioVars2->g_PreMainDecodeId == u8MainDecodeId) &&
        (g_AudioVars2->g_PreMainDspCodeType == MainDspCodeType) &&
        (u32PreHDMITxSamplingRate == u32HDMITxSamplingRate) &&
        (u32PreSPDIFTxSamplingRate == u32SPDIFTxSamplingRate))
    {
        if ((MainDspCodeType == AU_DVB_STANDARD_DTS) ||
            (MainDspCodeType == AU_DVB_STANDARD_DTSLBR))
        {
            if (u32bPreDTSCD == u32bDTSCD)
            {
                /* In DTS/DTS-LBR case, if DTS-CD information is same, no need to to change SPDIF setting */
                return;
            }
        }
        else
        {
            /* No need to change SPDIF setting */
            return;
        }
    }

    /* Change multi channel input source for DTS encode */
    if ((eAudioSource == E_AUDIO_INFO_HDMI_IN) && (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE)))
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH1_CFG, 0x07, 0x02);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH2_CFG, 0x07, 0x02);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH3_CFG, 0x07, 0x02);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH4_CFG, 0x07, 0x02);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH1_CFG, 0x07, 0x00);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH2_CFG, 0x07, 0x00);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH3_CFG, 0x07, 0x00);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH4_CFG, 0x07, 0x00);
    }

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    /* Mute SPDIF before changing setting */
    _HAL_AUDIO_SPDIF_SetMute(TRUE);

    /* Apply UI's SPDIF setting to Audio R2 */
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC1, u8R2SpdifMode, 0);
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SPDIF_TYPE, ADEC2, u8R2SpdifMode, 0);

    HAL_AUR2_WriteByte(REG_R2_HDMI_NONPCM_PATH, u8MainDecodeId);

    /* Apply SPDIF's Output Setting */
    switch (u8SpdifMode)
    {
        case SPDIF_OUT_NONPCM:
        case SPDIF_OUT_BYPASS:
        case SPDIF_OUT_TRANSCODE:
        {
            //HALAUDIO_PRINT("HAL SPDIF set as Non-PCM\n");

            if (u8SpdifMode == SPDIF_OUT_TRANSCODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, TRUE, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, TRUE, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, FALSE, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, FALSE, 0);
            }

            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x02); /* Grp C in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG+1, 0x01, 0x00);
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x02); /* Grp C in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG+1, 0x01, 0x00);
            stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;
            AUDIO_DELAY1MS(5);
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x02, 0x02); /* Non-PCM */

            break;
        }

        case SPDIF_OUT_PCM:
        default:
        {
            //HALAUDIO_PRINT("HAL SPDIF set as PCM\n");

            AUDIO_DELAY1MS(5);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC1, FALSE, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TTS_EN, ADEC2, FALSE, 0);
            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x2, 0x0); /* PCM Mode */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF_OUT_CFG, 0x07, 0x00); /* Tx1 Grp A in */
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_SPDIF2_OUT_CFG, 0x07, 0x00); /* Tx2 Grp A in */
            stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;
            stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;

            break;
        }
    }

    u32bPreDTSCD = u32bDTSCD;
    g_u32bDTSCD = u32bDTSCD;
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    g_AudioVars2->g_eSpdifOutputType = u8SpdifMode;
    g_AudioVars2->g_PreMainDecodeId = u8MainDecodeId;
    g_AudioVars2->g_PreMainDspCodeType = MainDspCodeType;

    u32PreHDMITxSamplingRate = u32HDMITxSamplingRate;
    u32PreSPDIFTxSamplingRate = u32SPDIFTxSamplingRate;
    HAL_AUDIO_Set_DecimationMode(AUDIO_HDMI_ARC_OUTPUT, u8MainDecodeId);
    HAL_AUDIO_Set_DecimationMode(AUDIO_SPDIF_OUTPUT, u8MainDecodeId);

    _HAL_AUDIO_SPDIF_SetMute(g_AudioVars2->g_SPDIF_MuteStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_SPDIF_GetMode()  @@KH
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  BYTE    :  Se-DSP REG_M2D_MAILBOX_SPDIF_CTRL value
///                                u8Spdif_mode = 0x0 : SPDIF enable, PCM mode
///                                u8Spdif_mode = 0x1 : SPDIF OFF
///                                u8Spdif_mode = 0x2 : SPDIF enable, non-PCM mode
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _HAL_AUDIO_SPDIF_GetMode(void)
{
    return g_AudioVars2->g_eSpdifOutputType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit, register in 0x2C80[5]
/// @param L_bit_en  \b : generation bit, register in 0x2C82[7]
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    MS_BOOL retTx1, retTx2;

    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    if (retTx1 == TRUE && retTx2 == TRUE)
    {
        stDigitalChannelStatusTx1.stCopyRight.CBit = C_bit_en;
        stDigitalChannelStatusTx1.stCopyRight.LBit = L_bit_en;
        stDigitalChannelStatusTx2.stCopyRight.CBit = C_bit_en;
        stDigitalChannelStatusTx2.stCopyRight.LBit = L_bit_en;
    }
    else
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_SPDIF_GetSCMS(void)
{

    MS_U8   SCMS_status, SCMS_C_bit_tmp, SCMS_L_bit_tmp;

    HALAUDIO_CHECK_SHM_INIT;

    if(g_AudioVars2->stDigitalOutChannelStatus[DIGITAL_SPDIF_OUTPUT].stCopyRight.CBit)
    {
        SCMS_C_bit_tmp = 0x01;
    }
    else
    {
        SCMS_C_bit_tmp = 0x00;
    }

    if(g_AudioVars2->stDigitalOutChannelStatus[DIGITAL_SPDIF_OUTPUT].stCopyRight.LBit)
    {
        SCMS_L_bit_tmp = 0x02;
    }
    else
    {
        SCMS_L_bit_tmp = 0x00;
    }

    SCMS_status = SCMS_C_bit_tmp | SCMS_L_bit_tmp;

    return(SCMS_status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPdif channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)
{
    MS_U8 u8Type;

    u8Type=(MS_U8)eType;

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);


    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return;
    }

    switch(u8Type)
    {
        case SPDIF_CHANNEL_STATUS_FS:
            stDigitalChannelStatusTx1.u8SamplingRate = (MS_U8)eValue; // S/PDIF Tx1
            stDigitalChannelStatusTx2.u8SamplingRate = (MS_U8)eValue; // S/PDIF Tx2
            break;
        case SPDIF_CHANNEL_STATUS_CATEGORY:
            stDigitalChannelStatusTx1.u8Category = (MS_U8)eValue; //  Tx1 By-pass L- bit
            stDigitalChannelStatusTx2.u8Category = (MS_U8)eValue; //  Tx2 By-pass L- bit
            break;
        case SPDIF_CHANNEL_STATUS_PCM_FORMAT:
            stDigitalChannelStatusTx1.u8PcmFormatControl = (MS_U8)eValue; // Tx1 By-pass Copy right bit
            stDigitalChannelStatusTx2.u8PcmFormatControl = (MS_U8)eValue; // Tx2 By-pass Copy right bit
            break;

        default :
            break;
    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:  This routine is used to control SPDIF CS in detail.
/// @return MS_U8      \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{
    MS_BOOL status_rtn = FALSE;
    MS_BOOL retTx1, retTx2;

    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));
    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALAUDIO_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
        return status_rtn;
    }

    switch (cs_mode)
    {
        case SPDIF_CS_CategoryCode:
            switch(status)
            {
                case SPDIF_CS_Category_BroadCast:
                    stDigitalChannelStatusTx1.u8Category = 0x20;
                    stDigitalChannelStatusTx2.u8Category = 0x20;
                    break;

                case SPDIF_CS_Category_General:
                    stDigitalChannelStatusTx1.u8Category = 0x00;
                    stDigitalChannelStatusTx2.u8Category = 0x00;
                    break;

                case SPDIF_CS_Category_BroadCast_JP:
                    stDigitalChannelStatusTx1.u8Category = 0x20;
                    stDigitalChannelStatusTx2.u8Category = 0x20;
                    break;

                case SPDIF_CS_Category_BroadCast_EU:
                    stDigitalChannelStatusTx1.u8Category = 0x30;
                    stDigitalChannelStatusTx2.u8Category = 0x30;
                    break;

                case SPDIF_CS_Category_BroadCast_USA:
                    stDigitalChannelStatusTx1.u8Category = 0x26;
                    stDigitalChannelStatusTx2.u8Category = 0x26;
                    break;

                default :
                    stDigitalChannelStatusTx1.u8Category = 0x00;
                    stDigitalChannelStatusTx2.u8Category = 0x00;
                    break;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_CopyRight:
            if(status == SPDIF_CS_CopyRight_CP)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
            }
            if(status == SPDIF_CS_CopyRight_NonCP)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_SourceNumber:
            if(status == SPDIF_CS_SourceNumber_2)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0x40;
                stDigitalChannelStatusTx2.u8SourceNumber = 0x40;
            }
            if(status == SPDIF_CS_SourceNumber_5)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0xA0;
                stDigitalChannelStatusTx2.u8SourceNumber = 0xA0;
            }
            if(status == SPDIF_CS_SourceNumber_6)
            {
                stDigitalChannelStatusTx1.u8SourceNumber = 0xA0;
                stDigitalChannelStatusTx2.u8SourceNumber = 0xA0;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_ChannelNumber:
            if(status == SPDIF_CS_ChannelNumber_A_Left)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x08;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x08;
            }
            if(status == SPDIF_CS_ChannelNumber_B_Right)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x04;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x04;
            }
            if(status == SPDIF_CS_ChannelNumber_C)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x0C;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x0C;
            }
            if(status == SPDIF_CS_ChannelNumber_D)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x02;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x02;
            }
            if(status == SPDIF_CS_ChannelNumber_E)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x0A;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x0A;
            }
            if(status == SPDIF_CS_ChannelNumber_F)
            {
                stDigitalChannelStatusTx1.u8ChannelNumber = 0x06;
                stDigitalChannelStatusTx2.u8ChannelNumber = 0x06;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_SmpFreq:
            switch (status)
            {
                case SPDIF_CS_SmpFreq_192:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x70;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x70;
                    break;

                case SPDIF_CS_SmpFreq_96:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x50;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x50;
                    break;

                case SPDIF_CS_SmpFreq_48:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x40;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x40;
                    break;

                case SPDIF_CS_SmpFreq_176:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x30;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x30;
                    break;

                case SPDIF_CS_SmpFreq_88:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x10;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x10;
                    break;

                case SPDIF_CS_SmpFreq_44:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                    break;

                case SPDIF_CS_SmpFreq_64:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                    break;

                case SPDIF_CS_SmpFreq_32:
                    stDigitalChannelStatusTx1.u8SamplingRate = 0xC0;
                    stDigitalChannelStatusTx2.u8SamplingRate = 0xC0;
                    break;

                default:
                    status_rtn = FALSE;
                    break;
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_ClockAcc:
            status_rtn = TRUE;
            break;

        case SPDIF_ChannelAB:
            if (status == SPDIF_ChannelAB_Invalid_ON)
            {
                stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;   // Tx1 Invalid bit=high in non-PCM mode
                stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_NONPCM;   // Tx2 Invalid bit=high in non-PCM mode
            }
            if (status == SPDIF_ChannelAB_Invalid_OFF)
            {
                stDigitalChannelStatusTx1.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;      // Tx1 Invalid bit=low in PCM mode
                stDigitalChannelStatusTx2.u8PcmFormatControl = SPDIF_CS_FORMAT_PCM;      // Tx2 Invalid bit=low in PCM mode
            }
            status_rtn = TRUE;
            break;

        case SPDIF_CS_CopyProtect:
            switch (status)
            {
                case SPDIF_CS_CopyProtect_CopyFree:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
                    status_rtn = TRUE;
                    break;

                 case SPDIF_CS_CopyProtect_CopyOnce:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
                    status_rtn = TRUE;
                    break;

                 case SPDIF_CS_CopyProtect_CopyNever:
                    stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx1.stCopyRight.LBit = 1;
                    stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
                    stDigitalChannelStatusTx2.stCopyRight.LBit = 1;
                    status_rtn = TRUE;
                    break;

                default:
                    status_rtn = FALSE;
                    break;
            }
            break;

        default:
            status_rtn = FALSE;
            break;

    }

    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

    return(status_rtn);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_Monitor_SamplingRate()
/// @brief \b Function \b Description: This routine is to set ADC clock for SPDIF raw out
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_Monitor_SamplingRate(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SPDIF_SetOutputType()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF output mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SPDIF_SetOutputType(AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType)
{
    eSource=eSource;
    eType=eType;
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO HDMI Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to mute HDMI Tx output(by DSP)
/// @param <IN>        \b bEnMute    :    0--Disable mute
///                                       1--Enable mute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_HDMI_Tx_SetMute(MS_BOOL bEnMute)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_NonpcmMonitor()
/// @brief \b Function \b Description:  Report PCM/HDMI non-PCM status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : return HDMI PCM/non-PCM status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_HDMI_NonpcmMonitor(void)
{
    HAL_AUDIO_AbsWriteMaskByte(0x1027C8, 0x02, 0x02);        // Add  audio bank offset
    if(((HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_INPUT) & 0xC0)==0x40)||(HAL_AUDIO_HDMI_NonpcmMonitor2() == 0x1))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_NonpcmMonitor2()
/// @brief \b Function \b Description:  Report PCM/HDMI non-PCM status from DSP
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8    : return HDMI PCM/non-PCM status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_HDMI_NonpcmMonitor2(void)
{
    MS_U32 HDMI_lock_status_tmp;
    HDMI_lock_status_tmp = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_npcm_lock, DSP_MEM_TYPE_DM);
    if (HDMI_lock_status_tmp == 0x30)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_DolbyMonitor()  @@Need_Modify
/// @brief \b Function \b Description:  Report HDMI non-PCM Dolby mod status
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  u8DolbyModeType    :
///                                0-- Other mode
///                                1-- Dolby mode
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_HDMI_DolbyMonitor(void)
{
    return HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_HDMI_PC);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_SetNonpcm()  @@Need_Modify
/// @brief \b Function \b Description:  Set HDMI PCM/non-PCM Mode
/// @param <IN>        \b nonPCM_en    :
///                                -- 0: PCM mode
///                                -- 1: non-PCM mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
{
//=======================================================
    Hdmi_Input_Path HDMI_INPUT_PATH ;
    MS_BOOL bIsNonPCM= FALSE;
    MS_U8 u8DecoderType = 0;

    bIsNonPCM = (MS_BOOL)(nonPCM_en & HDMI_NonPCM_Mask);  // Use bit 1 mask check PCM/nonPCM
    HDMI_INPUT_PATH = (Hdmi_Input_Path)(nonPCM_en & HDMI_Input_Path_Mask); // Use bit2 mask check main or sub in PIP

    if (bIsNonPCM)  //====== HDMI nonpcm mode =======
    {
        u8DecoderType = HAL_AUDIO_HDMI_DolbyMonitor();

        switch(u8DecoderType)
        {
            case AU_HDMI_DTS_TYPE1:
            case AU_HDMI_DTS_TYPE2:
            case AU_HDMI_DTS_TYPE3:
                if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB2_DTS);
                else
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB_DTS);

                break;
            case AU_HDMI_AC3:
            case AU_HDMI_AC3P:
                if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB2_AC3);
                else
                    HAL_AUDIO_SetSystem( MSAPI_AUD_DVB_AC3);

                break;
        };

     // Audio Input path config
        if (HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE) == 1)
        {
           // HDMI in 2nd Decoder
            if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP3_HDMI_INPUT, AUDIO_PATH_6);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_6,0x11);
            }
            else
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP3_HDMI_INPUT, AUDIO_PATH_MAIN);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x11);
            }
            HAL_AUR2_WriteByte(REG_R2_DECODE2_CMD, AU_DEC_R2_CMD_STOP);       // Stop
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG,0x07,0x04);
            MsOS_DelayTask(10);
            HAL_AUR2_WriteByte(REG_R2_DECODE2_CMD, AU_DEC_R2_CMD_PLAY);       // PlaY
        }
        else // HDMI in 1st Decoder
        {
            if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP1_HDMI_INPUT, AUDIO_PATH_6);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_6,0x11);
            }
            else
            {
                HAL_AUDIO_SetInputPath(AUDIO_DSP1_HDMI_INPUT, AUDIO_PATH_MAIN);
                HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x11);
            }

            HAL_AUR2_WriteByte(REG_R2_DECODE1_CMD, AU_DEC_R2_CMD_STOP);       // Stop
            HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x04);
            MsOS_DelayTask(10);
            HAL_AUR2_WriteByte(REG_R2_DECODE1_CMD, AU_DEC_R2_CMD_PLAY);       // Play
        }

   //========= Set for S/PDIF out ===========
        HAL_AUDIO_SPDIF_SetMode(_HAL_AUDIO_SPDIF_GetMode(), E_AUDIO_INFO_HDMI_IN);

        //Byte swap setting
        if (HAL_AUDIO_HDMI_DolbyMonitor() == AU_HDMI_AC3P)
        {
            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_BYTESWAP, 1, 0);
        }
        else
        {
            HAL_MAD_SetAC3PInfo(Audio_AC3P_infoType_BYTESWAP, 0, 0);
        }
    }
    else  //====== HDMI pcm mode =======
    {
        if(HDMI_INPUT_PATH == Hdmi_Input_Sub)    //sub
        {
            HAL_AUDIO_SetInputPath(AUDIO_HDMI_INPUT, AUDIO_PATH_6);
            HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_6,0x19);
        }
        else
        {
             HAL_AUDIO_SetInputPath(AUDIO_HDMI_INPUT, AUDIO_PATH_MAIN);
             HAL_AUDIO_HDMIAutoMute(AUDIO_PATH_MAIN,0x19);
        }

        HAL_AUDIO_SPDIF_SetMode(SPDIF_OUT_PCM, E_AUDIO_INFO_HDMI_IN);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_AC3_PathCFG()  @@Need_Modify
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path Control
/// @param <IN>        \b u8Ctrl    :
///                                -- 0: DVB2_AD path
///                                -- 1: HDMI path
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_AC3_PathCFG(MS_U8 u8Ctrl)
{
    if (u8Ctrl == 1)
    {   // HDMI
        /* use CH1 decode HDMI AC3 */
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x04);        // HDMI_AC3 REG CFG
    }
    else
    {   // DTV
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER1_CFG,0x07,0x00);        // DVB1 REG CFG
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Get_SYNTH_FREQ()  @@Need_Modify
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16  : return freq. If no signal, return 0
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////

MS_U16 HAL_AUDIO_HDMI_Get_SYNTH_FREQ(void)
{
    MS_U16 freq;

    freq = HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_SYNTH) + 0x100 * HAL_AUDIO_ReadByte(REG_AUDIO_STATUS_SYNTH+1);

    if(freq & 0x8000)
      return 0;   //no signal

    return freq;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Set_DwonSample()  @@Cathy
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param <IN>        \b MS_U8    :    donwsample ratio ; no need in T3
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////

void HAL_AUDIO_HDMI_Set_DwonSample(MS_U8 ratio)
{
    ratio=ratio; // No need in T3
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMIAutoMute()  @@Need_Modify
/// @brief \b Function \b This routine sets the automute function of HDMI.
/// @param <IN>        \b MS_U8    :     ch : 0~5
///                                                     value: register value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMIAutoMute(MS_U8 ch, MS_U8 value)
{
    if(ch < 4)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL1+(ch<<1),value);
    }
    else if(ch == 4)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL1+1,value);
    }
    else if(ch == 5)
    {
      HAL_AUDIO_WriteByte(REG_AUDIO_MUTE_CTRL2+1,value);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
        *onOff = FALSE;
        *hdmi_SmpFreq = AUDIO_FS_UNSUPPORT;
        *outType = HDMI_OUT_PCM;
}



///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO Internel Use Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _HAL_AUDIO_InitStep1()  @@Need_Modify
/// @brief \b Function \b Description: This routine is the initialization for Audio module(Step1).
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _HAL_AUDIO_InitStep1(void)
{
    HAL_AUDIO_WriteInitTable();
    HAL_AUDIO_SetPowerOn(1);
    HAL_AUDIO_SwResetMAD();

 // Cathy need remove
    HAL_AUDIO_WriteByte(REG_DEC_IDMA_CTRL0, 0x0002);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteByte(REG_DEC_IDMA_CTRL0, 0x0003);

    HAL_AUDIO_WriteByte(REG_SE_IDMA_CTRL0, 0x0002);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteByte(REG_SE_IDMA_CTRL0, 0x0003);
  // Reset Audio MIU Request for DEC-DSP
    HAL_MAD_DisEn_MIUREQ();

}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDspBaseAddr()
/// @brief \b Function \b Description:  This function is used to set bin file base address in flash for Audio module.
/// @param <IN>        \b u8Index    :
/// @param <IN>        \b Bin_Base_Address    :
/// @param <IN>        \b Mad_Base_Buffer_Adr    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_DSPMadMIUBank[u8Index] = (MS_U8) (u32Mad_Base_Buffer_Adr & 0x0000000F);
    u32Mad_Base_Buffer_Adr &= 0xFFFFFFF0;
    printf ("In %s, line(%d), u8Index(0x%x), u32Bin_Base_Address(0x%x), u32Mad_Base_Buffer_Adr(0x%x)\r\n", __FUNCTION__, __LINE__, u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);

    switch(u8Index)
    {
        case(DSP_ADV):   //DSP_ADV=R2=2
            g_AudioVars2->g_DSPBinBaseAddress[u8Index] = u32Bin_Base_Address;                                  //R2 ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index] = u32Mad_Base_Buffer_Adr;
            g_DSPMadMIUBank[u8Index] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index] = g_AudioVars2->g_DSPBinBaseAddress[u8Index];
            g_DSPMadBaseBufferAdr[u8Index] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index];

            g_AudioVars2->g_DSPBinBaseAddress[u8Index-1] = u32Bin_Base_Address;                                 //SE ADEC ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-1] = u32Mad_Base_Buffer_Adr + (ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE);
            g_DSPMadMIUBank[u8Index-1] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index-1] = g_AudioVars2->g_DSPBinBaseAddress[u8Index-1];
            g_DSPMadBaseBufferAdr[u8Index-1] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-1];

            g_AudioVars2->g_DSPBinBaseAddress[u8Index-2] = u32Bin_Base_Address;                                    //Comm ADDR setting
            g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-2] = g_DSPMadBaseBufferAdr[u8Index-1] + ASND_DSP_DDR_SIZE;
            g_DSPMadMIUBank[u8Index-2] = g_AudioVars2->g_DSPMadMIUBank[u8Index];
            g_DSPBinBaseAddress[u8Index-2] = g_AudioVars2->g_DSPBinBaseAddress[u8Index-2];
            g_DSPMadBaseBufferAdr[u8Index-2] = g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index-2];
            break;
        default:
            u8Index = u8Index;
            break;
    }

#ifndef MSOS_TYPE_NUTTX
    AUDIO_TEE_INFO_SHM_CHECK_NULL;
    REE_TO_TEE_MBX_MSG_SET_PARAM(0, u8Index);
    REE_TO_TEE_MBX_MSG_SET_PARAM(1, (MS_U8)(u32Bin_Base_Address&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(2, (MS_U8)((u32Bin_Base_Address>>8)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(3, (MS_U8)((u32Bin_Base_Address>>16)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(4, (MS_U8)((u32Bin_Base_Address>>24)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(5, (MS_U8)(u32Mad_Base_Buffer_Adr&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(6, (MS_U8)((u32Mad_Base_Buffer_Adr>>8)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(7, (MS_U8)((u32Mad_Base_Buffer_Adr>>16)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM(8, (MS_U8)((u32Mad_Base_Buffer_Adr>>24)&0xFF));
    REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(9);
    HAL_AUDIO_SendMBXMsg(REE_TO_TEE_MBX_MSG_HAL_AUDIO_SetDspBaseAddr | REE_TO_TEE_MBX_MSG_TYPE_GENERAL);
#endif
    HALAUDIO_PRINT ("%d DSPBinBaseAddress = 0x%x\n",   u8Index, g_AudioVars2->g_DSPBinBaseAddress[u8Index]);
    HALAUDIO_PRINT ("%d DSPMadBaseBufferAdr = 0x%x\n", u8Index, g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index]);
    HALAUDIO_PRINT ("%d DSPMadMIUBank = %d\n",          u8Index, g_AudioVars2->g_DSPMadMIUBank[u8Index]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDspMadBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b u8Index    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32: return the MAD base address
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{
    HALAUDIO_CHECK_SHM_INIT;

    if (u8Index == DSP_DEC)  //for MM case, they will ask DEC-DSP base as ES1 base, here we use SE-DSP instead
    {
        return g_AudioVars2->g_DSPMadBaseBufferAdr[DSP_SE];
    }
    return g_AudioVars2->g_DSPMadBaseBufferAdr[u8Index];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDspBinBaseAddr()
/// @brief \b Function \b Description:  This function is used to get the Bin file base address.
/// @param <IN>        \b u8Index    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32: return the Bin file base address
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_DSPBinBaseAddress[u8Index];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDspCodeType()
/// @brief \b Function \b Description:  This function is used to set the DSP code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b AUDIO_DSP_CODE_TYPE: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf("  [Audio] DSP1 Load code fail !!! SetDspCodeType Fail !!! \r\n"));
        return;
    }

    g_AudioVars2->g_DspCodeType = dspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDspCodeType()
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetDspCodeType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_DspCodeType;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDsp2CodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the DSP2 code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b AUDIO_DSP_CODE_TYPE: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDsp2CodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if (IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType))
    {
        return;
    }

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf("  [Audio] DSP2 Load code fail !!! SetDsp2CodeType Fail !!! \r\n"));
        return;
    }

    g_AudioVars2->g_Dsp2CodeType = dspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDsp2CodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the MAD base address.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b AUDIO_DSP_CODE_TYPE: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetDsp2CodeType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_Dsp2CodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDSPalive()
/// @brief \b Function \b Description:  This function is used to check if DSP alive.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8: DSP alive status.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_GetDSPalive(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_u8DspAliveFlag;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDSPalive()
/// @brief \b Function \b Description:  This function is used to set flag after DSP alive.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetDSPalive(MS_U8 alive)
{
    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_u8DspAliveFlag = alive;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetAdvSndEffDspCodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the DSP AdvSndEff code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b AUDIO_DSP_CODE_TYPE: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetAdvSndEffDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag)
{
    HALAUDIO_CHECK_SHM_INIT;

    if(bFlag == FALSE)
    {
        DBG_AUDIO_ERROR(printf("  [Audio] ADV_DSP Load code fail !!! SetAdvSndEffDspCodeType Fail !!! \r\n"));
        return;
    }

    g_AudioVars2->g_AdvSndEffDspCodeType = dspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetAdvSndEffDspCodeType()  @@Cathy
/// @brief \b Function \b Description:  This function is used to get the DSP AdvSndEff code type.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b AUDIO_DSP_CODE_TYPE: DSP code type.
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetAdvSndEffDspCodeType(void)
{
    HALAUDIO_CHECK_SHM_INIT;

    return g_AudioVars2->g_AdvSndEffDspCodeType;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_BackupMailbox()    @@Need_Modify
/// @brief \b Function \b Description:  This function is used to backup SIF mailbox.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_BackupMailbox(void)
{
    int i;

    for(i=0;i<12;i++)
    {
        SIF_MailBoxArray[i]=HAL_AUDIO_ReadByte(REG_SE_M2D_MAIL_BOX_BASE+i);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_RestoreMailbox()   @@Need_Modify
/// @brief \b Function \b Description:  This function is used to restore SIF mailbox.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_RestoreMailbox(void)
{
    int i;

    for(i=0;i<12;i++)
    {
        HAL_AUDIO_WriteByte((REG_SE_M2D_MAIL_BOX_BASE+i), SIF_MailBoxArray[i]);
    }
    HAL_AUDIO_WriteMaskByte(0x2BCE, 0xFF, 0xFF); // Toggle 2BCE to update N.F value
    HAL_AUDIO_WriteMaskByte(0x2BCE, 0xFF, 0x00);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description: This routine is used to initilize DMA Reader.
/// @param <IN>        \b   eType   : sampleRate of PCM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate)
{
    MS_U32 audio_pcm_dmaRdr_bufSz = HW_DMA_RDR1_BUF_SIZE;
    MS_U32 audio_pcm_dmaRdr_base = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_RDR1_BUF_ADDR; // DMA Reader Input buffer (DM_Prefetch)
    MS_U16 synthrate, divisor;
    unsigned char tmp1, tmp2, tmp3, tmp4;
    unsigned int i;

    /* init DMA writer address */
    dma_writePtr = (unsigned char *) MS_PA2KSEG1(audio_pcm_dmaRdr_base);
    dma_bufStartAddress =  (unsigned char *) MS_PA2KSEG1(audio_pcm_dmaRdr_base);
    dma_bufEndAddress =  (unsigned char *) MS_PA2KSEG1(audio_pcm_dmaRdr_base + audio_pcm_dmaRdr_bufSz);

    /* New DMA Reader setting
     * Formula is :
     * Synthesizer Rate = 216MHz / Divisor(1, 2, 4 or 8) * 1024 / 256 / Sampling Rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(sampleRate)
    {
        case SAMPLE_RATE_96000:
            divisor = 0;
            synthrate = 0x2328;
            break;
        case SAMPLE_RATE_44100:
            divisor = 0;
            synthrate = 0x4C87;
            break;
        case SAMPLE_RATE_32000:
            divisor = 0;
            synthrate = 0x6978;
            break;
        case SAMPLE_RATE_48000:
            divisor = 0;
            synthrate = 0x4650;
            break;
        case SAMPLE_RATE_22050:
            divisor = 1;
            synthrate = 0x4C87;
            break;
        case SAMPLE_RATE_16000:
            divisor = 1;
            synthrate = 0x6978;
            break;
        case SAMPLE_RATE_24000:
            divisor = 1;
            synthrate = 0x4650;
            break;
        case SAMPLE_RATE_11025:
            divisor = 2;
            synthrate = 0x4C87;
            break;
        case SAMPLE_RATE_8000:
            divisor = 2;
            synthrate = 0x6978;
            break;
        case SAMPLE_RATE_12000:
            divisor = 2;
            synthrate = 0x4650;
            break;
        default:
            divisor = 0;
            synthrate = 0x4650;
            break;
    }

    /* Initial DMA Reader path & clk select */          //DMA reader -> CH8
    HAL_AUDIO_WriteMaskByte(0x2CB0, 0x07, 0x04);        //SEL_CLK_DMA_READER
    HAL_AUDIO_WriteMaskByte(0x2C6B, 0xFF, 0x8F );       //CH8 sel to DMA Rdr

    HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, 0x0000);     //reset dma reader

    HAL_AUDIO_WriteMaskByte(0x2B8E, 0xFF, ((audio_pcm_dmaRdr_base/BYTES_IN_MIU_LINE) & 0xFF));          //DMA Reader Base Addr[7:0]
    HAL_AUDIO_WriteMaskReg(0x2B84, 0xFFFF,(((audio_pcm_dmaRdr_base/BYTES_IN_MIU_LINE)>>8) & 0xFFFF));   //DMA Reader Base Addr[23:8]
    HAL_AUDIO_WriteMaskByte(0x2B8F, 0x0F, (((audio_pcm_dmaRdr_base/BYTES_IN_MIU_LINE)>>24) & 0x0F));    //DMA Reader Base Addr[27:24]

    HAL_AUDIO_WriteMaskReg(0x2B86, 0xFFFF, audio_pcm_dmaRdr_bufSz/BYTES_IN_MIU_LINE);  //setting : DMA Reader Size
    HAL_AUDIO_WriteMaskReg(0x2B88, 0xFFFF, audio_pcm_dmaRdr_bufSz - 12);     //setting : DMA Reader Overrun Thr
    HAL_AUDIO_WriteMaskReg(0x2B8A, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr

    /* synthersizer setting update */                   //DMA reader
    HAL_AUDIO_WriteMaskByte(0x2CA4, 0x20, 0x20);        //enable DMA synthesizer
    HAL_AUDIO_WriteMaskReg(0x2CB0, 0x6000, (divisor << 13));  //Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH
    HAL_AUDIO_WriteMaskReg(0x2C20, 0xFFFF, synthrate);  //DMA synthesizer N.F.

    /* Reset and Start DMA Reader */
    HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFF, 0x6402);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFF, 0x2402);

    /* Clear DMA Reader buffer */
    for ( i = 0; i < (HW_DMA_RDR1_BUF_SIZE/4); i++ )
    {
        tmp1 = 0;
        tmp2 = 0;
        tmp3 = 0;
        tmp4 = 0;

        *dma_writePtr++ = tmp1;
        *dma_writePtr++ = tmp2;
        *dma_writePtr++ = tmp3;
        *dma_writePtr++ = tmp4;
    }

    /* Reset Write Pointer */
    dma_writePtr = dma_bufStartAddress;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description: This routine is used to initilize DMA Reader.
/// @param <IN>        \b   eType   : sampleRate of PCM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DmaReader_AllInput_Init(void)
{
    HAL_AUDIO_DmaReader_Init(SAMPLE_RATE_48000);
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer bytes
/// @param <OUT>       \b NONE    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
{
    MS_U8 *bufptr = (MS_U8 *)MS_PA2KSEG1((MS_VIRT)buffer);
    MS_U8 tmp1, tmp2, tmp3, tmp4, replay_flag=0;
    MS_U32 i, level_cnt;

    // Mask LEVEL_CNT_MASK before read
    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
    level_cnt = HAL_AUDIO_ReadReg(0x2B8C);
    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);

    if ( level_cnt <= 6 )
    {
        if ((HAL_AUDIO_ReadReg(0x2B80) & 0x0001) == 0x0001)
                replay_flag = 1;

        /* reset dma reader */
        HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, 0x0000);    //clear wr cntrs

        /* Reset and Start DMA Reader */
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFE, 0x6402);
        AUDIO_DELAY1MS(1);
        HAL_AUDIO_WriteMaskReg(0x2B80, 0xFFFE, 0x2402);

        /* Reset Write Pointer */
        dma_writePtr = dma_bufStartAddress;

        printf("***** Audio DMAReader Buffer empty !! ***** \r\n");
    }

    // Mask LEVEL_CNT_MASK before read
    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
    level_cnt = HAL_AUDIO_ReadReg(0x2B8C);
    HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);
    if(level_cnt < (HW_DMA_RDR1_BUF_SIZE - bytes*2)/BYTES_IN_MIU_LINE)
    {
        for ( i = 0; i < (bytes/2); i++ )
        {
            tmp1 = 0;
            tmp2 = *bufptr++;
            tmp3 = *bufptr++;
            tmp4 = 0;

            *dma_writePtr++ = tmp1;
            *dma_writePtr++ = tmp2;
            *dma_writePtr++ = tmp3;
            *dma_writePtr++ = tmp4;

            if ( dma_writePtr >= dma_bufEndAddress )
                dma_writePtr = dma_bufStartAddress;
        }
        HAL_AUDIO_WriteMaskReg(0x2B82, 0xFFFF, bytes*2/BYTES_IN_MIU_LINE);
        AUDIO_DELAY1US(5); // To prevent slow update of level_cnt

        if (replay_flag == 1)
        {
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0001);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0100);
        }
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetEntertainmentMode()
/// @brief \b Function \b Description: This routine is used to switch the DTV/KTV/GAME mode
/// @param <IN>        \b   eMode   :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE eMode)
{
    switch((int)eMode)
    {
        case AUDIO_ETMODE_DTV :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP2_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x00);   // ADC0 gain   0dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);  // Fix PCM in
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000); // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo
            break;

        case AUDIO_ETMODE_KTV :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x80);   // enable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x80);   // ADC0 gain    6dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_MIXER); // MIC + PCM
            HAL_AUDIO_WriteMaskReg(REG_SNDR2_ADVSND_SEL, 0xFF00, 0x1E00);  // Switch R2 to KTV mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_MIXED_LR, 0);  // Switch SE-DSP sound mode to L+R for MIC0=L_CH, MIC1=R_CH
            break;

        case AUDIO_ETMODE_KTV2 :  // Use I2S input ; not MicroPhone in
            //HAL_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0xF0, 0xA0);   // enable KTV mode; Skyworth (stone) model
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
            HAL_AUDIO_WriteMaskByte(0x2D5D , 0x80, 0x80);   // Mute MIC volume (only mix MP3 & PCM DMA data)
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_MIXER); // MIC + PCM
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000);  // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_MIXED_LR, 0);  // Switch SE-DSP sound mode to L+R for MIC0=L_CH, MIC1=R_CH
            break;

        case AUDIO_ETMODE_GAME :
            //HAL_AUDIO_SetInputPath(AUDIO_DSP2_DVB_INPUT , AUDIO_PATH_5);
            HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x10);
            HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
            HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);   // Fix PCM in
            //HAL_AUDIO_WriteMaskReg(REG_AUR2_ADVSND_SEL, 0xFF00, 0x0000);   // Switch R2 to normal mode
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo
            break;

        case AUDIO_ETMODE_VOIP_ON:
            //Voip will setup snd system to GAME MODE,
            //In GAME MODE, Voip used 0x2D46[7:0] to
            //upload Raw_Delay_SE to DRAM.
            HAL_AUDIO_WriteMaskByte(0x2D46, 0xFF, 0xA1);

            HAL_AUDIO_WriteMaskByte(0x2C9C, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2C9E, 0xFF, 0xE0);
            HAL_AUDIO_WriteMaskByte(0x2C9F, 0xFF, 0xC4);

            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x00);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo

#if 0
            //config SRC to 8KHz
            HAL_AUDIO_WriteMaskByte(0x2BBC, 0xFF, 0xF0);//0xE0},  // Group A SRC rate
            HAL_AUDIO_WriteMaskByte(0x2BBD, 0xFF, 0x03);//0x07},
            HAL_AUDIO_WriteMaskByte(0x2BBE, 0xFF, 0xF0);//0x50},
            HAL_AUDIO_WriteMaskByte(0x2BBF, 0xFF, 0xD2);//0x46},
            HAL_AUDIO_WriteMaskByte(0x2BC0, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BC1, 0xFF, 0x04);//0x08},
            HAL_AUDIO_WriteMaskByte(0x2BC2, 0xFF, 0xF0);//0x50},
            HAL_AUDIO_WriteMaskByte(0x2BC3, 0xFF, 0xD2);//0x46},
            //update SYNTH
            //HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            //reset GroupA SRC
            //HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x80);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
#endif

            printf("HAL_AUDIO_SetEntertainmentMode: AUDIO_ETMODE_VOIP_ON\r\n");
            break;

        case AUDIO_ETMODE_VOIP_OFF:
            //disable upload Raw_Delay_SE to DRAM.
            HAL_AUDIO_WriteMaskByte(0x2D46, 0xFF, 0x00);

            HAL_AUDIO_WriteMaskByte(0x2C9C, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2C9E, 0xFF, 0xD0);
            HAL_AUDIO_WriteMaskByte(0x2C9F, 0xFF, 0x20);

            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2C9D, 0x40, 0x00);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_SoundMode, AU_DVB_MODE_STEREO, 0);  // Switch SE-DSP sound mode to stereo

#if 0
            //set SRC back to 48KHz
            HAL_AUDIO_WriteMaskByte(0x2BBC, 0xFF, 0xE0);  // Group A SRC rate
            HAL_AUDIO_WriteMaskByte(0x2BBD, 0xFF, 0x07);
            HAL_AUDIO_WriteMaskByte(0x2BBE, 0xFF, 0x50);
            HAL_AUDIO_WriteMaskByte(0x2BBF, 0xFF, 0x46);
            HAL_AUDIO_WriteMaskByte(0x2BC0, 0xFF, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BC1, 0xFF, 0x08);
            HAL_AUDIO_WriteMaskByte(0x2BC2, 0xFF, 0x50);
            HAL_AUDIO_WriteMaskByte(0x2BC3, 0xFF, 0x46);
            //update SYNTH
            //HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x40);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2BCE, 0x40, 0x00);
            //reset GroupA SRC
            //HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x80);
            AUDIO_DELAY1MS(1);
            HAL_AUDIO_WriteMaskByte(0x2B44, 0x80, 0x00);
#endif
            printf("HAL_AUDIO_SetEntertainmentMode: AUDIO_ETMODE_VOIP_OFF\r\n");
            break;

        default :
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetSystem()
/// @brief \b Function \b Description:  This function will load
///                                     different audio decoder to audio system.
///
///                                     In Mstar audio system, we can support 1 or 2 decoders.
///                                     One is for main DTV audio or Multimedia audio player.
///                                     Another is for second audio decode (ex: used for Audio Description purpose)
///
/// @param enDecSystem  \b : (En_DVB_decSystemType) decoder type
///                        - parameter type "MSAPI_AUD_DVB_XXXX" is decoder type for main audio decoder
///                        - parameter type "MSAPI_AUD_DVB2_XXXX" is decoder type for second audio decoder
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    //MS_BOOL pas2_sel = TRUE;
    MS_BOOL bRet = FALSE;

    HAL_AUDIO_SetIsDtvFlag(TRUE);
    HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, FALSE, 0);

    // set decoder system
    switch (enDecSystem)
    {
            ////////////////////////////////////////////////
            //      Audio Decoder 1
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB_MPEG:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_mpeg);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MPEG, TRUE);
                break;

            case MSAPI_AUD_DVB_AC3:
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DDE);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AC3, TRUE);
                break;

            case MSAPI_AUD_DVB_AC3P:
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DDE);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AC3P, TRUE);
                break;

            case MSAPI_AUD_DVB_MP3:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_mpeg);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MP3, TRUE);
                break;

            case MSAPI_AUD_DVB_AAC:
            case MSAPI_AUD_DVB_MS10_DDT:
                if (g_AudioVars2->DolbyAACFlag == 1)
                {
                    HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_aac);
                    g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                    HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MS10_DDT, TRUE);
                }
                else
                {
                    HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_gaac);
                    g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                    HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AAC, TRUE);
                }
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DDE);
                break;

            case MSAPI_AUD_DVB_XPCM:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_xpcm);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_XPCM, TRUE);
                break;

            case MSAPI_AUD_DVB_RA8LBR:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ra8);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_RA8LBR, TRUE);
                break;

            case MSAPI_AUD_DVB_WMA:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_wma);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_WMA, TRUE);
                break;

            case MSAPI_AUD_DVB_DTS:
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dts);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DTS, TRUE);
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DTSE);
                break;

            case MSAPI_AUD_DVB_DTSLBR:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dts);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DTSLBR, TRUE);
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DTSE);
                break;

            case MSAPI_AUD_DVB_MS10_DDC:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_ac3);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MS10_DDC, TRUE);
                break;

            case MSAPI_AUD_DVB_WMA_PRO:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_wma);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_WMA_PRO, TRUE);
                break;

            case MSAPI_AUD_DVB_FLAC:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_flac);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_FLAC, TRUE);
                break;

            case MSAPI_AUD_DVB_VORBIS:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_vorbis);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_VORBIS, TRUE);
                break;

            case MSAPI_AUD_DVB_AMR_NB:
                break;

            case MSAPI_AUD_DVB_AMR_WB:
                break;

            case MSAPI_AUD_DVB_DRA:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dra);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DRA, TRUE);
                break;

            case MSAPI_AUD_DVB_DolbyTrueHDBypass:
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dolby_truhd_bypass);  /*  select r2 decoder to olby TrueHD bypss */
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DolbyTrueHD_Bypass,TRUE); //set decoder type for HDMI Tx handler
                break;

            case MSAPI_AUD_DVB_DTSHDADO:
                HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
                HAL_AUR2_WriteByte(REG_R2_DECODE1_TYPE, adec_type_dts);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID1].eAudFormat = enDecSystem;
                HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DTS, TRUE);
                break;

            ////////////////////////////////////////////////
            //      Audio Decoder 2
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB2_MPEG:
            case MSAPI_AUD_DVB2_MP3:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_mpeg); // MP3 & MPEG use the same decoder
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;

            case MSAPI_AUD_DVB2_AC3:
            case MSAPI_AUD_DVB2_AC3P:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_ac3);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;

            case MSAPI_AUD_DVB2_AAC:
            case MSAPI_AUD_DVB2_GAAC:
                if (g_AudioVars2->DolbyAACFlag == 1)
                {
                    HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_aac);
                    g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                }
                else
                {
                    HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_gaac);
                    g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                }
                break;

            case MSAPI_AUD_DVB2_DTS:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_dts);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;

            case MSAPI_AUD_DVB2_XPCM:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_xpcm);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;

             case MSAPI_AUD_DVB2_DolbyTrueHDBypass:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_dolby_truhd_bypass);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;

             case MSAPI_AUD_DVB2_DTSHDADO:
                HAL_AUR2_WriteByte(REG_R2_DECODE2_TYPE, adec_type_dolby_dtshd_ado);
                g_AudioVars2->AudioDecStatus[AU_DEC_ID3].eAudFormat = enDecSystem;
                break;


            default:
                HAL_AUDIO_SetIsDtvFlag(FALSE);
                bRet = TRUE;
                break;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
        MS_BOOL SetDecSys_RtnStatus = FALSE;
        switch (enDecSystem)
        {
            ////////////////////////////////////////////////
            //      Audio Decoder 1
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB_MPEG:
            case MSAPI_AUD_DVB_AC3:
            case MSAPI_AUD_DVB_AC3P:
            case MSAPI_AUD_DVB_MP3:
            case MSAPI_AUD_DVB_AAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_MS10_DDT:
            case MSAPI_AUD_DVB_MS10_DDC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_XPCM:
            case MSAPI_AUD_DVB_RA8LBR:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_WMA:
            case MSAPI_AUD_DVB_WMA_PRO:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_DTS:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_FLAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_VORBIS:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_AMR_NB:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB_AMR_WB:
                SetDecSys_RtnStatus = FALSE;
                break;

            ////////////////////////////////////////////////
            //      Audio Decoder 2
            ////////////////////////////////////////////////
            case MSAPI_AUD_DVB2_MPEG:
            case MSAPI_AUD_DVB2_MP3:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_AC3:
            case MSAPI_AUD_DVB2_AC3P:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_AAC:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_DDE:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB2_DTSE:
                SetDecSys_RtnStatus = FALSE;
                break;

            case MSAPI_AUD_DVB2_XPCM:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB2_KTV:
                SetDecSys_RtnStatus = TRUE;
                break;

            case MSAPI_AUD_DVB_TONE:
            case MSAPI_AUD_DVB_NONE:
            case MSAPI_AUD_DVB2_NONE:
            default:
                SetDecSys_RtnStatus = FALSE;
                break;
        }
        return(SetDecSys_RtnStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_AlgReloadCode()
/// @brief \b Function \b Description: This routine is used to reload DSP code
/// @param dspCodeType      \b :
/// @return MS_BOOL    \b : TRUE --DEC-DSP reload code okay
///                         FALSE--DEC-DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_AlgReloadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    // No DEC-DSP in Monaco
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_ALSA_Check(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    if (bEnable == TRUE)
    {
        /* Set as GAME mode */
        HAL_AUDIO_SetInputPath(AUDIO_DSP3_DVB_INPUT , AUDIO_PATH_5);
        HAL_AUDIO_WriteMaskByte(0x2DD9 , 0x38, 0x10);
        HAL_AUDIO_WriteMaskByte(0x2D47 , 0x80, 0x00);   // disable KTV function
        HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, 0x40);   // ADC0 gain   -6dB
        HAL_AUDIO_WriteMaskByte(0x2D55, 0x0F, (MS_U8)INTERNAL_PCM);  // Fix PCM in

        return TRUE;
    }
    else
    {
        /* Do nothing for the moment */
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Alg2ReloadCode()
/// @brief \b Function \b Description: This routine is used to reload the algorithm in se-dsp .
/// @param dspCodeType      \b : information of algorithm code
/// @return MS_BOOL    \b : TRUE --DEC-DSP reload code okay
///                         FALSE--DEC-DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Alg2ReloadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    int time_out;
    MS_BOOL ret_status = FALSE;

    HAL_MAD2_SetDspIDMA();
    // Enter MCU/DSP hand-shake
    if (IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ADVSND_BEG/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_BEG/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_BEG/0x100);
    }
    else
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_DTV_BEG/0x100);
    }

    // PIO[8] interrupt
    HAL_MAD2_TriggerPIO8();

    //Wait Dsp Start reload Ack
    time_out = 0;
    while(time_out++<2000)
    {
        if(HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK1)
            break;
        AUDIO_DELAY1MS(1);
    }
    if(time_out>=2000)
    {
        DBG_AUDIO_ERROR("  DSP Reload timeOut1: %d\r\n",time_out);
        return FALSE;
    }

    // Change to IDMA Port
    HAL_MAD2_SetDspIDMA();

    // Start to Reload DSP code
    ret_status = HAL_AUDSP_DspLoadCode(dspCodeType);
    HAL_AUDIO_SetDsp2CodeType(dspCodeType, ret_status);

    // Enter MCU/DSP hand-shake
    if (IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ADVSND_END/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_END/0x100);
    }
    else if (IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType))
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_END/0x100);
    }
    else
    {
        HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_DTV_END/0x100);
    }

    HAL_MAD2_TriggerPIO8();

    // Wait Dsp End Reload Ack
    time_out = 0;
    while(time_out++<3000)
    {
        if(HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK2)
            break;
        AUDIO_DELAY1MS(1);
    }

    if(time_out>=3000)
    {
        DBG_AUDIO_ERROR("  DSP Reload timeOut2: %d\r\n",time_out);
        return FALSE;
    }

    DBG_AUDIO("HAL_AUDIO_SeReloadCode finish\r\n");

    HAL_MAD2_SetMcuCmd(0x00);   // In T3, clear 0x2DDC after reload finish

    return TRUE;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetUniDecodeDoneFlag()  @@Cathy
/// @brief \b Function \b Description:  This function is used to set the Decoder DSP ISR
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_GetUniDecodeDoneTag(void)
{
    MS_U16 temp1, temp2, i;

    for (i = 0; i < 100; i++)
    {
        temp1 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, ADEC1 );
        temp2 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, ADEC1 );

        if (temp1 == temp2)
        {
            return (temp1);
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name:         HAL_AUDIO_GetUniDecodeFrameCnt()
/// @brief \b Function \b Description:  This function is used to get the Decoder decoded frame count
/// @param <IN>        \b NONE:
/// @param <OUT>       \b NONE:
/// @param <RET>       \b NONE:
/// @param <GLOBAL>    \b NONE:
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUDIO_GetUniDecodeFrameCnt(AUDIO_DEC_ID eDSPId)
{
    MS_U16 temp1, temp2, i;
    Audio_id  r2AudioID = ADEC1;


     if (eDSPId == AU_DEC_ID3)
     {
        r2AudioID = ADEC2;
     }

    for ( i = 0; i < 100; i++ )
    {
        temp1 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, r2AudioID);
        temp2 = HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_UNI_DECODE_DONE_CNT, r2AudioID);

        if ( temp1 == temp2 )
        {
            return (temp1);
        }
    }

    return 0;
}

//******************************************************************************
//  [Function Name]:
//      HAL_AUDIO_PTS_info
//  [Description]:
//      get PTS and OS time or set to reset these reference
//  [Arguments]:
//
//*******************************************************************************
void HAL_AUDIO_PTS_info(MS_VIRT access_PTS_info )
{
    AUDIO_PTS_INFO PTS_info;

     memcpy((void *)&PTS_info,(void *)access_PTS_info,sizeof(AUDIO_PTS_INFO));

    //    printf("HAL_AUDIO_PTS_info((0x%x, %d, %lld))\n",(unsigned int)access_PTS_info,(int)PTS_info.os_Time,PTS_info.curPTS);
    if(PTS_info.set2Clr) //setting value
    {
         curr_OS_Time = PTS_info.os_Time;
         curr_PTS = PTS_info.curPTS;
    }
    else // get PTS information
    {

        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);

        PTS_info.set2Clr = 0 ;
        PTS_info.os_Time = curr_OS_Time;
        PTS_info.curPTS = curr_PTS;

        memcpy((void *)access_PTS_info,(void *)&PTS_info,sizeof(AUDIO_PTS_INFO));

        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
    }
}

void HAL_AUDIO_DmaWriter_Init(void)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_RebootDecDSP()
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_RebootDecDSP(void)
{
    AU_DVB_DECCMD deccmd_status;
    AUD_ERRMSG("\n*MAD Auto-Recovery DEC-R2* \n");

    HALAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->g_u8AudioCrashFlag = TRUE;

    deccmd_status = HAL_MAD_GetDecCmd(DVB_Audio_Decoder1);

    HAL_AUDIO_BackupMailbox();

    // Reset MAD module
    HAL_MAD_RSTMAD_DisEn_MIUREQ();
    AUDIO_DELAY1MS(2);

    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, FALSE, 0);    // Audio SIF channel enable setting -> disable
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_SIFFIFO_MODE, 0, 0);

    AUDIO_DELAY1MS(2);

    AUD_DBGMSG("*** Load code and reset DEC-R2 \n");

    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
    HAL_MAD2_SetMemInfo();

    HAL_AUDSP_DECR2LoadCode(); // Reload R2 code

    HAL_MAD_SetDecCmd(deccmd_status);

    AUDIO_DELAY1MS(50);
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_SIFFIFO_MODE, 1, 0);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, 0);    // Audio SIF channel enable setting -> enable
    HAL_AUDIO_RestoreMailbox();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DspReboot()
/// @brief \b Function \b Description: This routine is used to do dsp reboot and some specific setting
/// @param <IN>        \b   eType   : param
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DspReboot(MS_U8 alive)
{
    alive = alive;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_ReadDspCounter()
/// @brief \b Function \b Description:  Read the DSP running counter
/// @param CounterType \b  :
///                 - DSP_DEC_MAIN_COUNTER ==> Decoder DSP Main loop counter
///                 - DSP_DEC_TIMER_COUNTER ==> Decoder Timer counter
///                 - DSP_SE_MAIN_COUNTER ==> SE DSP Main loop counter
///                 - DSP_SE_TIMER_COUNTER ==> SE Timer counter
///                 - DSP_SE_ISR_COUNTER ==> SE Interrupt ISR counter
/// @return MS_U8      \b  : Running counter value
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUDIO_ReadDspCounter(AUDIO_DSP_COUNTER_TYPE  CounterType)
{
    MS_U8  u8CounterValue=0;

    switch(CounterType)
    {
        case DSP_DEC_MAIN_COUNTER:
        case DEC_R2_MAIN_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_DECR2_MAIN_COUNTER);
            break;

        case DSP_DEC_TIMER_COUNTER:
        case DEC_R2_TIMER_COUNTER:
            u8CounterValue = HAL_AUR2_ReadByte(REG_DECR2_TIMER_COUNTER);
            break;

        case DSP_SE_MAIN_COUNTER:
        case SND_R2_MAIN_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_MAIN_COUNTER);
            break;

        case DSP_SE_TIMER_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_TIMER_COUNTER);
            break;

        case DSP_SE_ISR_COUNTER:
        case SND_R2_TIMER_COUNTER:
            u8CounterValue = HAL_AUDIO_ReadByte(REG_SOUND_ISR_COUNTER);
            break;

        default:
            break;
    }
    return u8CounterValue;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AuProcessPowerDown_Wait()
/// @brief \b Function \b Description: This routine is used to DSP power-down wait function.
/// @param bEnable     \b : TRUE --Not wait,
///                         FALSE--wait
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SetPowerDownWait(MS_BOOL bEnable)
{
    bEnable = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SendIntrupt()
/// @brief \b Function \b Description:  send a PIO8 interrupt to DSP
/// @param bDspType    \b :
/// @param u8Cmd       \b :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd)
{
    if (bDspType == DSP_DEC)
    {
        HAL_MAD_SetPIOCmd(u8Cmd);
        HAL_MAD_TriggerPIO8();
    }
    else
    {
            HAL_MAD2_SetPIOCmd(u8Cmd);
            HAL_MAD2_TriggerPIO8();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SeSystemLoadCode()
/// @brief \b Function \b Description: This routine is used to load DSP code for SE-DSP .
/// @return MS_BOOL    \b : TRUE --DEC-DSP load code okay,
///                         FALSE--DEC-DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SeSystemLoadCode(void)
{
        MS_U16 time_out = 0;

        HAL_MAD2_SetDspIDMA();

        AUD_DBGMSG("MDrv_AUDIO_SeSystemLoadCode() \r\n");

        HAL_AUDSP_DspLoadCode(AU_SE_SYSTEM);

        //Wait Dsp init finished Ack
    #ifndef MBOOT_PLAY_MELODY
        while(time_out++<100) {
            if(HAL_MAD2_GetLoadCodeAck() == 0xE3)
                break;
            AUDIO_DELAY1MS(2);
        }
    #endif

        if(time_out>=100) {
            AUD_ERRMSG("DSP2 Re-Active\n");
        }
        else {
            AUD_DBGMSG("audio DSP_SE LoadCode success..\n");
        }

        //inform DSP to start to run
        HAL_MAD2_SetMcuCmd(0xF3);

        return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecoderLoadCode()
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DecoderLoadCode()
////////////////////////////////////////////////////////////////////////////////
MS_BOOL  HAL_AUDIO_DecoderLoadCode(void)
{

    HAL_MAD_SetDspIDMA();

    AUD_DBGMSG("MDrv_AUDIO_DecoderLoadCode() \r\n");

#ifdef MBOOT_PLAY_MELODY
    HAL_AUDSP_DspLoadCode(AU_DVB_STANDARD_MP3);   //MBoot
#endif

    HAL_AUDSP_DspLoadCode(AU_DEC_SYSTEM);

    //enable SIF Channel
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, 0);

    //inform DSP to start to run
    HAL_MAD_SetMcuCmd(0xF3);

    AUD_DBGMSG("DEC System code download finished!\r\n");
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_init()
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_Init(MS_BOOL Initype)
{
    Initype = Initype;
    return Initype;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_HDMI_TX_SetMode()
/// @brief \b Function \b Description:  This routine is used to set HDMI output mode
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType, AUDIO_SOURCE_INFO_TYPE eSource)
{
    outType = outType;
    eSource = eSource;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCaptureInit()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    MS_BOOL ret = TRUE;

    switch(eID)
    {
        case E_DEVICE0:
           g_pcm_capture_base_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE1_BUF_ADDR;
           ret = HAL_AUDIO_SetDataCaptureSource(eID, eSource);
           ret = HAL_AUDIO_PCMCapture_Stop(eID);
           break;

        case E_DEVICE1:
           g_pcm_capture2_base_addr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE2_BUF_ADDR;
           ret = HAL_AUDIO_SetDataCaptureSource(eID, eSource);
           ret = HAL_AUDIO_PCMCapture_Stop(eID);
           break;

        default:
           printf("\r\n =====PCMCapture_Init : unsupported DEVICE TYPE!!!=======");
           ret = FALSE;
           break;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
    MS_BOOL ret = TRUE;

    switch(eID)
    {
        case E_DEVICE0:
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE); // start
           break;

        case E_DEVICE1:
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE); //start
           break;

        default:
           printf("\r\n =====PCMCapture_Start : unsupported DEVICE TYPE!!!=======");
           ret = FALSE;
           break;
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
    MS_BOOL ret = TRUE;
    MS_U8 flagCount = 0;

    switch(eID)
    {
        case E_DEVICE0:
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, 0);  // stop
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, 0);
           do
           {
               MsOS_DelayTask(2);
               flagCount ++;
               if (flagCount > 100)
               {
                    printf("\r\n =====PCMCapture_Stop : Device%d timeout =======", eID);
                    break;
               }
           } while (HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) > 0);
           break;

        case E_DEVICE1:
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, 0); //stop
           HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, 0);
           do
           {
               MsOS_DelayTask(2);
               flagCount ++;
               if (flagCount > 100)
               {
                    printf("\r\n =====PCMCapture_Stop : Device%d timeout =======", eID);
                    break;
               }
           } while (HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) > 0);
           break;

        default:
           printf("\r\n =====PCMCapture_Stop : unsupported DEVICE TYPE!!!=======");
           ret = FALSE;
           break;
    }

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{
    MS_BOOL ret = TRUE;
    MS_VIRT buffer_tmp = (MS_VIRT)buffer;
    MS_U32 request_size = bytes;
    MS_U32 read_ptr_mailbox = 0;
    MS_U32 write_ptr_mailbox = 0;
    MS_PHY pcm_capture_base_addr_tmp = 0;
    MS_PHY pcm_capture_read_addr_tmp = 0;
    MS_PHY pcm_capture_write_addr_tmp = 0;
    MS_S32 avail_size = 0;

    switch(eID)
    {
        case E_DEVICE0:
            read_ptr_mailbox = M2S_MBOX_PCM_CAPTURE_DDR_RdPtr;
            write_ptr_mailbox = S2M_MBOX_PCM_CAPTURE_DDR_WrPtr;
            pcm_capture_base_addr_tmp = g_pcm_capture_base_addr;
            break;

        case E_DEVICE1:
            read_ptr_mailbox = M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr;
            write_ptr_mailbox = S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr;
            pcm_capture_base_addr_tmp = g_pcm_capture2_base_addr;
            break;

        default:
           printf("\r\n =====PCMCapture_Read : unsupported DEVICE TYPE!!!=======\n");
           ret = FALSE;
           break;
    }

    if (ret == FALSE)
        return ret;

    //get read & write pointer
    pcm_capture_read_addr_tmp = pcm_capture_base_addr_tmp + (HAL_AUDIO_ReadReg(read_ptr_mailbox) * BYTES_IN_MIU_LINE);
    pcm_capture_write_addr_tmp = pcm_capture_base_addr_tmp + (HAL_AUDIO_ReadReg(write_ptr_mailbox) * BYTES_IN_MIU_LINE);
    avail_size = (MS_S32)(pcm_capture_write_addr_tmp - pcm_capture_read_addr_tmp);
    if (avail_size < 0)
    {
        avail_size += (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE);
    }

    // if overflow , return false
    if (avail_size >= ((DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE - DSP2_PCM_CAPTURE_COPY_LINESIZE) * BYTES_IN_MIU_LINE))
    {
        printf("\r\n ===== PCM DATA OVERFLOW !!!=======\n");

        HAL_AUDIO_PCMCapture_Stop(eID);
        HAL_AUDIO_PCMCapture_Start(eID);
        ret = FALSE;
        return ret;
    }

    // if no enough data, do nothing.
    if (avail_size < request_size)
    {
        ret = FALSE;
        return ret;
    }

    //copy data to destination.
    do
    {
        MS_U32 size_tmp = (pcm_capture_base_addr_tmp + (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)) - pcm_capture_read_addr_tmp;

        size_tmp = (avail_size >= size_tmp) ? size_tmp : avail_size;
        size_tmp = (request_size >= size_tmp) ? size_tmp : request_size;

        memcpy((void *)buffer_tmp, (void *)MS_PA2KSEG1(pcm_capture_read_addr_tmp), size_tmp);
        MsOS_FlushMemory();

        pcm_capture_read_addr_tmp += size_tmp;
        if (pcm_capture_read_addr_tmp == (pcm_capture_base_addr_tmp + (DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE * BYTES_IN_MIU_LINE)))
            pcm_capture_read_addr_tmp = pcm_capture_base_addr_tmp;

        buffer_tmp += size_tmp;
        avail_size -= size_tmp;
        request_size -= size_tmp;

    } while (request_size > 0);

    //update read pointer
    HAL_AUDIO_WriteReg(read_ptr_mailbox, ((pcm_capture_read_addr_tmp - pcm_capture_base_addr_tmp) / BYTES_IN_MIU_LINE));

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    MS_U32 u32ControlAddress;
    MS_BOOL ret = TRUE;

    switch(eID)
    {
        case E_DEVICE0:
           u32ControlAddress=M2S_MBOX_CAPTURE_CTRL;
           break;

        case E_DEVICE1:
           u32ControlAddress=M2S_MBOX_CAPTURE_CTRL+1;
           break;

        default:
           printf("\r\n =====SetDataCaptureSource : unsupport DEVICE TYPE!!!=======");
           return FALSE;
    }

    switch(eSource)
    {
        case E_CAPTURE_CH5:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH5);
           break;

        case E_CAPTURE_CH6:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH6);
           break;

        case E_CAPTURE_CH7:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH7);
           break;

        case E_CAPTURE_CH8:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_CH8);
           break;

        case E_CAPTURE_ADC:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC1);
           break;

        case E_CAPTURE_ADC2:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC2);
           break;

        case E_CAPTURE_PCM_SE:
           HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_Raw_Delay_SE);
           break;

        case E_CAPTURE_ADC1_AUIN0_INPUT:
        case E_CAPTURE_ADC1_AUIN1_INPUT:
        case E_CAPTURE_ADC1_AUIN2_INPUT:
        case E_CAPTURE_ADC1_AUIN3_INPUT:
        case E_CAPTURE_ADC1_AUIN4_INPUT:
        case E_CAPTURE_ADC1_AUIN5_INPUT:
        case E_CAPTURE_ADC1_AUMIC_INPUT:
        {
            MS_U8 u8temp = 0;

            //check if ADC1 is occupied by main or sub channel
            if((LONIBBLE(g_audioSrcType) == 2 || LONIBBLE(g_audioSubSrcType) == 2))
            {
                printf("===The audio capture setting is failed by the following reason:=== \n");
                printf("The input setting of ADC1 is occupied by main or sub channel\n");
                ret = FALSE;
                break;
            }

            //switch source of ADC1
            if (eSource == E_CAPTURE_ADC1_AUIN0_INPUT)
            {
                u8temp = 0x00;
            }
            else if (eSource == E_CAPTURE_ADC1_AUIN1_INPUT)
            {
                u8temp = 0x01;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN2_INPUT)
            {
                u8temp = 0x02;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN3_INPUT)
            {
                u8temp = 0x03;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN4_INPUT)
            {
                u8temp = 0x04;
             }
            else if (eSource == E_CAPTURE_ADC1_AUIN5_INPUT)
            {
                u8temp = 0x05;
             }
            else if (eSource == E_CAPTURE_ADC1_AUMIC_INPUT)
            {
                u8temp = 0x07;
             }

            HAL_AUDIO_WriteMaskByte(0x2CE2, 0xF0, (u8temp<<4));
            HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC1);
            break;
        }

        case E_CAPTURE_ADC2_AUIN0_INPUT:
        case E_CAPTURE_ADC2_AUIN1_INPUT:
        case E_CAPTURE_ADC2_AUIN2_INPUT:
        case E_CAPTURE_ADC2_AUIN3_INPUT:
        case E_CAPTURE_ADC2_AUIN4_INPUT:
        case E_CAPTURE_ADC2_AUIN5_INPUT:
        case E_CAPTURE_ADC2_AUMIC_INPUT:
        {
            MS_U8 u8temp = 0;

            //check if ADC2 is occupied by main or sub channel
            if((LONIBBLE(g_audioSrcType) == 9 || LONIBBLE(g_audioSubSrcType) == 9))
            {
                printf("===The audio capture setting is faiedl by the following reason:=== \n");
                printf("The input setting of ADC2 is occupied by main or sub channel \n");
                ret = FALSE;
                break;
            }

            //switch source of ADC2
            if (eSource == E_CAPTURE_ADC2_AUIN0_INPUT)
            {
                u8temp = 0x00;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN1_INPUT)
            {
                u8temp = 0x01;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN2_INPUT)
            {
                u8temp = 0x02;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN3_INPUT)
            {
                u8temp = 0x03;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN4_INPUT)
            {
                u8temp = 0x04;
            }
            else if (eSource == E_CAPTURE_ADC2_AUIN5_INPUT)
            {
                u8temp = 0x05;
            }
            else if (eSource == E_CAPTURE_ADC2_AUMIC_INPUT)
            {
                u8temp = 0x07;
            }
            HAL_AUDIO_WriteMaskByte(0x2CE2, 0x0F, u8temp);
            HAL_AUDIO_WriteByte(u32ControlAddress, M2S_MBOX_GET_ADC2);
            break;
         }

         default:
             printf("\r\n =====SetDataCaptureSource : unsupport DEVICE TYPE!!!=======");
             ret = FALSE;
             break;
    }

    return ret;
}

MS_BOOL _AudioReloadCodePreProcess(AUDIO_DSP_ID eDspId, AUDIO_DSP_CODE_TYPE code_type)
{
    int time_out;

    code_type = code_type;
    if ( eDspId >= AUDIO_DSP_ID_R2 )
    {
        HALAUDIO_ERROR ("%s: invalid DSP ID (%d) !!\r\n", __FUNCTION__, eDspId);
        return FALSE;
    }

    if ( eDspId == AUDIO_DSP_ID_DEC )
    {

        // PIO[8] interrupt
        HAL_MAD_SetPIOCmd(0xEE);        // just not enter DSP MHEG5_isr
        HAL_MAD_TriggerPIO8();

        //Wait Dsp Start reload Ack
        time_out = 0;
        while ( time_out++ < 1000 )
        {
            if ( HAL_MAD_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK1 )
                break;

            AUDIO_DELAY1MS(2);
        }
        if ( time_out >= 1000 )
        {
            HALAUDIO_ERROR ("  DSP Re-active1\r\n");
            return FALSE;
        }

        // Change to IDMA Port
        HAL_MAD_SetDspIDMA();
    }
    else if ( eDspId == AUDIO_DSP_ID_SND )
    {
        Dvb2DecCmd_tmp = HAL_AUDIO_Se_Status();
        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);                      // Stop

        MsOS_DisableInterrupt(E_INT_FIQ_DEC_DSP2UP);

        // Reset MAD module
        HAL_MAD2_DisEn_MIUREQ();

        HAL_MAD2_SetDspIDMA();
        // Enter MCU/DSP hand-shake
        if (((code_type & 0xF0) == 0xB0))
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_BEG >> 8);
        }
        else if (((code_type & 0xF0) == 0xD0))
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_BEG >> 8);
        }
        else
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_DTV_BEG >> 8);
        }

        // PIO[8] interrupt
        HAL_MAD2_TriggerPIO8();

        //Wait Dsp Start reload Ack
        time_out = 0;
        while ( time_out++ < 2000 )
        {
            if ( HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK1 )
            {
                break;
            }

            AUDIO_DELAY1MS(1);
        }

        if ( time_out >= 2000 )
        {
            HALAUDIO_ERROR ("  SND DSP Reload timeOut1: %d\r\n", time_out);
            return FALSE;
        }

        // Change to IDMA Port
        HAL_MAD2_SetDspIDMA();
    }

    return TRUE;
}

MS_BOOL _AudioReloadCodePostProcess(AUDIO_DSP_ID eDspId,  AUDIO_DSP_CODE_TYPE code_type)
{
    int time_out;

    code_type = code_type;

    if ( eDspId >= AUDIO_DSP_ID_R2 )
    {
        HALAUDIO_ERROR ("%s: invalid DSP ID (%d) !!\r\n", __FUNCTION__, eDspId);
        return FALSE;
    }

    if ( eDspId == AUDIO_DSP_ID_DEC )
    {

        // PIO[8] interrupt
        HAL_MAD_SetPIOCmd(0xEE);        // just not enter DSP MHEG5_isr
        HAL_MAD_TriggerPIO8();

        // Wait Dsp End Reload Ack
        time_out = 0;
        while(time_out++<1500)
        {
            if(HAL_MAD_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK2)
            {
                break;
            }
            AUDIO_DELAY1MS(2);
        }

        if(time_out>=1500)
        {
            HALAUDIO_ERROR ("  DSP Re-active2\r\n");
            return FALSE;
        }

        HAL_MAD_SetMcuCmd(0x00);   // In T3 T8, clear 0x2D9C after reload finish
    }
    else if ( eDspId == AUDIO_DSP_ID_SND )
    {
        // Enter MCU/DSP hand-shake
        if (((code_type & 0xF0) == 0xB0))
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_SIF_END >> 8);
        }
        else if (((code_type & 0xF0) == 0xD0))
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_ENC_END >> 8);
        }
        else
        {
            HAL_MAD2_SetMcuCmd(MBOX_DBGCMD_RELOAD_DTV_END >> 8);
        }

        HAL_MAD2_TriggerPIO8();

        // Wait Dsp End Reload Ack
        time_out = 0;
        while ( time_out++ < 3000 )
        {
            if ( HAL_MAD2_GetReloadCodeAck() == MBOX_DSP_RELOAD_ACK2 )
            {
                break;
            }

            AUDIO_DELAY1MS(1);
        }

        if ( time_out >= 3000 )
        {
            HALAUDIO_ERROR ("  SND DSP Reload timeOut2\r\n");
            return FALSE;
        }

        HAL_MAD2_SetMcuCmd(0x00);   // In T3, clear 0x2DDC after reload finish

        MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);
        HAL_MAD_SetDSP2DecCmd((AU_DVB_DECCMD) Dvb2DecCmd_tmp);
    }

    return TRUE;
}

MS_BOOL HAL_AUDIO_SetSystem2(AUDIO_DEC_ID DecId, En_DVB_decSystemType Param)
{
    MS_U32 regDecoderType = REG_R2_DECODE1_TYPE;
    MS_BOOL bRet = TRUE;

    if ( DecId == AU_DEC_ID3 )
        regDecoderType = REG_R2_DECODE2_TYPE;
    else if( DecId == AU_DEC_ID1)
        regDecoderType = REG_R2_DECODE1_TYPE;
    else if( DecId == AU_DEC_ID2)
    {
        // AU_DEC_ID2 only for ATV SIF
        // For other case should not use AU_DEC_ID2
        return bRet;
    }

    g_AudioVars2->AudioDecStatus[DecId].eAudFormat = Param;

    switch ( Param )
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_mpeg);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_MPEG, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MPEG, TRUE);
            break;

        case MSAPI_AUD_DVB_MS10_DDC:
        case MSAPI_AUD_DVB_AC3P:
        case MSAPI_AUD_DVB_AC3:
            HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
            HAL_AUR2_WriteByte(regDecoderType, adec_type_ac3);
            HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DDE);

            HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x08, 0x00);  // select ADEC DSP NPCM
            AUDIO_DELAY1MS(1);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_AC3P, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AC3P, TRUE);
            break;

        case MSAPI_AUD_DVB_AAC:
        case MSAPI_AUD_DVB_MS10_DDT:
            if (g_AudioVars2->DolbyAACFlag == 1)
            {
                HAL_AUR2_WriteByte(regDecoderType, adec_type_aac);
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_MS10_DDT, TRUE);
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_MS10_DDE);
            }
            else
            {
                HAL_AUR2_WriteByte(regDecoderType, adec_type_gaac);
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AAC, TRUE);
                HAL_MAD2_ReLoadCode(AU_DVB2_ENCODE_DDE);
            }

            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_AAC, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_AAC, TRUE);
            break;

        case MSAPI_AUD_DVB_XPCM:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_xpcm);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_XPCM, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_XPCM, TRUE);
            break;

        case MSAPI_AUD_DVB_RA8LBR:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_ra8);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_RA8LBR, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_RA8LBR, TRUE);
            break;

        case MSAPI_AUD_DVB_WMA:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_wma);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_WMA, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_WMA, TRUE);
            break;

        case MSAPI_AUD_DVB_DTS:
        case MSAPI_AUD_DVB_DTSLBR:
            HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dts);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_DTS, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DTS, TRUE);
            break;

        case MSAPI_AUD_DVB_WMA_PRO:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_wmap);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_WMA_PRO, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_WMA_PRO, TRUE);
            break;

        case MSAPI_AUD_DVB_DRA:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dra);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_DRA, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DRA, TRUE);
            break;

        case MSAPI_AUD_DVB_FLAC:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_flac);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_FLAC, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_FLAC, TRUE);
            break;

        case MSAPI_AUD_DVB_VORBIS:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_vorbis);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_VORBIS, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_VORBIS, TRUE);
            break;

        case MSAPI_AUD_DVB_AMR_NB:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dummy);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_NONE, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_NONE, TRUE);
            break;

        case MSAPI_AUD_DVB_AMR_WB:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dummy);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_NONE, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_NONE, TRUE);
            break;


        case MSAPI_AUD_DVB_DolbyTrueHDBypass:
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dolby_truhd_bypass);  /*  select r2 decoder to olby TrueHD bypss */
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_DolbyTrueHD_Bypass,TRUE); //set decoder type for HDMI Tx handler
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DolbyTrueHD_Bypass,TRUE); //set decoder type for HDMI Tx handler
            break;

        case MSAPI_AUD_DVB_DTSHDADO:
            HAL_AUDIO_WriteMaskByte(0x2E8C, 0xFF, 0x00);                      // disable R2 encode (R2 DDCO)
            HAL_AUR2_WriteByte(regDecoderType, adec_type_dts);
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_setBypassSPDIF_PAPB_chk, TRUE, 0);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_STANDARD_DTS, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_STANDARD_DTS, TRUE);
            break;

        default:
            HALAUDIO_ERROR ("%s: UN-KNOW DECODER TYPE (0x%x) !!\r\n", __FUNCTION__, Param);
            if (DecId == AU_DEC_ID3)
                HAL_AUDIO_SetDsp2CodeType(AU_DVB_NONE, TRUE);
            else
                HAL_AUDIO_SetDspCodeType(AU_DVB_NONE, TRUE);
            break;
    }

    return bRet;
}

AUDIO_DEC_ID AU_GetDecID(AUDIO_DEC_ID *DecPriority, AudioDecStatus_t * p_AudioDecStatus)
{
    int Index = 0;
    AUDIO_DEC_ID DecID = AU_DEC_INVALID;

    for(Index=0; Index<AU_DEC_MAX; Index++)
    {
        // Get Dec ID by priority
        DecID = DecPriority[Index];
        printf("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
        if ((HAL_MAD_GetAudioCapability(DecID, p_AudioDecStatus) == TRUE) &&
             (g_AudioVars2->AudioDecStatus[DecID].bConnect == FALSE))
        {
            printf("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
            break;
        }
    }

    printf("DecID = %d %s() %d \n",DecID,__FUNCTION__,__LINE__);
    return DecID;
}

AUDIO_DEC_ID HAL_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_DEC_ID DecRet = AU_DEC_INVALID;

    switch (p_AudioDecStatus->eSourceType)
    {
        case E_AUDIO_INFO_ATV_IN:
            DecRet = AU_DEC_ID2;
            break;

        case E_AUDIO_INFO_HDMI_IN:
            DecRet = AU_GetDecID(HDMIDecPriority, p_AudioDecStatus);
            break;

        case E_AUDIO_INFO_DTV_IN:
            DecRet = AU_GetDecID(DTVDecPriority, p_AudioDecStatus);
            break;

        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
            DecRet = AU_GetDecID(MMDecPriority, p_AudioDecStatus);
            break;

        default:
            DecRet = AU_GetDecID(DTVDecPriority, p_AudioDecStatus);
            break;
    }

    printf("DecID = %d %s() %d \n",DecRet,__FUNCTION__,__LINE__);
    return DecRet;
}

MS_BOOL HAL_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus)
{
    AUDIO_DSP_ID DSP_id;
    MS_BOOL bRet = FALSE;

    HALAUDIO_CHECK_SHM_INIT;

    if((g_AudioVars2->AudioDecStatus[DecId].eAudFormat == p_AudioDecStatus->eAudFormat) &&
      (g_AudioVars2->AudioDecStatus[DecId].eSourceType == p_AudioDecStatus->eSourceType) &&
      (g_AudioVars2->AudioDecStatus[DecId].eGroup == p_AudioDecStatus->eGroup) &&
      (g_AudioVars2->AudioDecStatus[DecId].eStcSource == p_AudioDecStatus->eStcSource) &&
      (g_AudioVars2->AudioDecStatus[DecId].bIsAD == p_AudioDecStatus->bIsAD))
    {
        HALAUDIO_PRINT("%s() - DSP code is the same\n",__FUNCTION__);
        return bRet;
    }

    // update occupied decoder
    if (DecId == AU_DEC_ID3)
    {
        DSP_id = AUDIO_DSP_ID_SND;
    }
    else
    {
        DSP_id = AUDIO_DSP_ID_DEC;
    }

    if ((p_AudioDecStatus->eAudFormat & MSAPI_AUD_ATV_NONE) == MSAPI_AUD_ATV_NONE)
    {
        AUDIO_PATH_TYPE u8SifPath;
        if (p_AudioDecStatus->eGroup == E_CONNECT_SUB)
        {
            u8SifPath = AUDIO_PATH_6;
        }
        else
        {
            u8SifPath = AUDIO_PATH_MAIN;
        }
        HAL_AUDIO_SetInputPath(AUDIO_DSP4_SIF_INPUT , u8SifPath);
    }
    else
    {
        HAL_MAD_DvbFLockSynthesizer_En();
    }
    //g_AudioVars2->AudioDecStatus[DecId].eDSPId = DSP_id;
    //p_AudioDecStatus->eDSPId = DSP_id;

    switch ( p_AudioDecStatus->eAudFormat)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_AC3:
        case MSAPI_AUD_DVB_AC3P:
        case MSAPI_AUD_DVB_MP3:
        case MSAPI_AUD_DVB_AAC:
        case MSAPI_AUD_DVB_XPCM:
        case MSAPI_AUD_DVB_RA8LBR:
        case MSAPI_AUD_DVB_WMA:
        case MSAPI_AUD_DVB_DTS:
        case MSAPI_AUD_DVB_MS10_DDT:
        case MSAPI_AUD_DVB_MS10_DDC:
        case MSAPI_AUD_DVB_WMA_PRO:
        case MSAPI_AUD_DVB_FLAC:
        case MSAPI_AUD_DVB_VORBIS:
        case MSAPI_AUD_DVB_AMR_NB:
        case MSAPI_AUD_DVB_AMR_WB:
        case MSAPI_AUD_DVB_DRA:
        case MSAPI_AUD_DVB_DTSLBR:
        case MSAPI_AUD_DVB_DTSHDADO:
            bRet = HAL_AUDIO_SetSystem2(DecId, p_AudioDecStatus->eAudFormat);
            if ( bRet == TRUE )
            {
                g_AudioVars2->AudioDecStatus[DecId].eAudFormat = (En_DVB_decSystemType)  p_AudioDecStatus->eAudFormat;
            }
            break;

        case MSAPI_AUD_ATV_BTSC:
            bRet = HAL_SIF_SetSystem(AU_SIF_BTSC);
            break;

        case MSAPI_AUD_ATV_PAL:
            bRet = HAL_SIF_SetSystem(AU_SIF_PALSUM);
            break;

        case MSAPI_AUD_ATV_EIAJ:
        case MSAPI_AUD_ATV_FM_RADIO:
            break;

        default:
            HALAUDIO_ERROR ("%s: UN-KNOW DECODER TYPE (0x%x) !!\r\n", __FUNCTION__, p_AudioDecStatus->eAudFormat);
            break;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    MS_U32 DDR_ADDR = 0;
    MS_U32 DDR_ADDR_LINE_BASE = 0;
    MS_U16 DDR_ADDR_TMP;
    MS_U32 DDR_Value = 0;
    if (DecId == AU_DEC_ID1)
    {
        switch(DDRInfo)
        {
            case E_AUD_MEMORY_BASE:             //use DSP2 base instead
                DDR_ADDR_TMP = HAL_AUDIO_ReadByte((REG_SE_MAD_OFFSET_BASE_L + 1)); // [7:0]
                DDR_ADDR_LINE_BASE = DDR_ADDR_TMP & 0xFF;
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_H); // [23:8]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xFFFF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<8);
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_EXT); // [27:24]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<24);
                DDR_ADDR = DDR_ADDR + (DDR_ADDR_LINE_BASE<<BYTES_IN_MIU_LINE_LOG2);
                DDR_Value = DDR_ADDR;
                break;
            case  E_AUD_MEMORY_SIZE:
                //TODO
                break;
            default:
                HALAUDIO_ERROR ("%s: UN-KNOW DDRInfo (0x%x) !!\r\n", __FUNCTION__, DDRInfo);
                break;
        }
    }
    else if(DecId == AU_DEC_ID3)
    {
        switch(DDRInfo)
        {
            case E_AUD_MEMORY_BASE:
                DDR_ADDR_TMP = HAL_AUDIO_ReadByte((REG_SE_MAD_OFFSET_BASE_L + 1)); // [7:0]
                DDR_ADDR_LINE_BASE = DDR_ADDR_TMP & 0xFF;
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_H); // [23:8]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xFFFF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<8);
                DDR_ADDR_TMP = HAL_AUDIO_ReadReg(REG_SE_MAD_OFFSET_BASE_EXT); // [27:24]
                DDR_ADDR_TMP = DDR_ADDR_TMP & 0xF;
                DDR_ADDR_LINE_BASE = DDR_ADDR_LINE_BASE + (DDR_ADDR_TMP <<24);
                DDR_ADDR = DDR_ADDR + (DDR_ADDR_LINE_BASE<<BYTES_IN_MIU_LINE_LOG2);
                DDR_Value = DDR_ADDR;
                break;
            case  E_AUD_MEMORY_SIZE:
                //TODO
                break;
            default:
                HALAUDIO_ERROR ("%s: UN-KNOW DDRInfo (0x%x) !!\r\n", __FUNCTION__, DDRInfo);
                break;
        }
    }
    else
    {
        HALAUDIO_ERROR ("%s: UN-KNOW DecId (0x%x) !!\r\n", __FUNCTION__, DecId);
    }
    return DDR_Value;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_DumpDspInfo()
/// @brief \b Function \b Description:  Dump DSP infomation
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_DumpDspInfo(void)
{
    int tmp_H, tmp_L, tmp_M;
    long long result = 0;
    unsigned int ES_Lvl, PCM_Lvl;
    static MS_U32 preSysTime, prv_mmFile_APTS;
    MS_U32 sysTime, mmFile_APTS;

    if(g_AudioDumpInfoCtrl == Audio_DumpInfo_Off)
    {
        return;
    }

    ES_Lvl = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
    PCM_Lvl = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_DMX_LEVEL, ADEC1);

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_Avsync) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08d]", sysTime);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%03X(%04X),", ES_Lvl, tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFF;
        printf("PCM=%04X(%04X,%04X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X",   tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNTH_RATE, ADEC1)&0xFFFF;
        printf("<%04X>,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_STATE, ADEC1)&0x0F;
        tmp_M = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 0)&0x0F;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_FREERUN_TYPE, ADEC1)&0x0F;
        printf("State=%X,%X(%X)|", tmp_L,tmp_M, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",    tmp_L);
        printf("%02X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_1STPTS_CNT, ADEC1)&0x00FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_WAIT_STC_CNT, ADEC1)&0x00FF;
        tmp_M = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_FLUSH_CNT, ADEC1)&0x00FF;
        printf("%02X,%02X,%02X|",  tmp_L, tmp_H, tmp_M);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_WR_IDX, ADEC1)&0xF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_TAG_RD_IDX, ADEC1)&0xF;
        printf("tag:%01X,%01X|",  tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC1)&0x00FFFFFF;
        printf("AvDly=%05X,", tmp_L);

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_STC, ADEC1);
        printf("STC=%08lld,", result/45);

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS, ADEC1);
        printf("PTS=%08lld,", result/45);

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
        printf("cPTS=%08lld,", result/45);

        result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC1);
        printf("|Td=%lld\n", result/90);
    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_Dec1) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08d]", sysTime);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%03X(%04X),", ES_Lvl, tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFF;
        printf("PCM=%04X(%04X,%02X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_TYPE);
        printf("ID=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_TYPE, ADEC1)&0x00FF;
        printf("Type=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFFFF;
        printf("pState=%06X,", tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD);
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_RECEIVE_STOP_CNT, ADEC1)&0x00FF;
        printf("|Cmd=%02X,Stop=%02X|",  tmp_L, tmp_H);

        tmp_L = HAL_AUDIO_ReadReg(REG_AUDIO_DECODER1_CFG);
        tmp_H = HAL_AUDIO_ReadReg(REG_AUDIO_DECODER2_CFG);
        printf("Input_Mux:%02X,", tmp_L&0x7);
        printf("%02X,", tmp_H&0x7);
        printf("%02X|", (tmp_L>>16)&0x7);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",  tmp_L);
        printf("%02X|",  tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CALLING_CNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNC_MISS_CNT, ADEC1)&0x0000FFFF;
        printf("Call:%04X,",    tmp_L);
        printf("sMiss:%04X|",  tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC1)&0x00FFFFFF;
        printf("pcm=%06X,",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_R, ADEC1)&0x00FFFFFF;
        printf("%06X|",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X\n",   tmp_L);
    }


    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_IoInfo) != 0)
    {
        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH5_CFG)) & 0x000F;
        printf("CH5 mux=0x%02X,",  tmp_L);

        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH6_CFG)) & 0x000F;
        printf("Ch6 mux=0x%02X,",  tmp_L);

        tmp_L = (HAL_AUDIO_ReadByte(REG_AUDIO_CH7_CFG)) & 0x000F;
        printf("Ch7 mux=0x%02X",  tmp_L);

        // Not define
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC1)&0x00FFFFFF;
        printf("|DEC1=0x%06X,",  tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_OUT_L, ADEC2)&0x00FFFFFF;
        printf("DEC2=0x%06X",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_RAW+0, DSP_MEM_TYPE_DM);
        printf("|CH5=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_MUL_CH6+0, DSP_MEM_TYPE_DM);
        printf("CH6=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info2+IO_INFO2_SCART+0, DSP_MEM_TYPE_DM);
        printf("CH7=0x%06X",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_DAC1_OUT+0, DSP_MEM_TYPE_DM);
        printf("|DAC1=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_IIS1_OUT+0, DSP_MEM_TYPE_DM);
        printf("I2S=0x%06X,",  tmp_L);

        tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_IO_Info1+IO_INFO1_SPDIF_OUT+0, DSP_MEM_TYPE_DM);
        printf("SPDIF=0x%06X\n",  tmp_L);
    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_AutoTest) != 0)
    {
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x00FFFFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x00FFFFFF;
        printf("[AutoTest][AUDIO][FrameCount][%d]\n",     tmp_L);
        printf("[AutoTest][AUDIO][ErrorCount][%d]\n",     tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
        printf("[AutoTest][AUDIO][SampleRate][%d]\n",     tmp_L);
        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
        printf("[AutoTest][AUDIO][ACMod][%d]\n",     tmp_L);
    }

    if ((g_AudioDumpInfoCtrl & Audio_DumpInfo_mmFileAVsync) != 0)
    {
        sysTime = MsOS_GetSystemTime();
        printf("[%08d](%03d):", sysTime, sysTime - preSysTime);
        preSysTime = sysTime;

        mmFile_APTS = HAL_MAD_GetCommInfo(Audio_Comm_infoType_1ms_PTS);
        printf("APTS:%07d(%03d)|", mmFile_APTS, mmFile_APTS - prv_mmFile_APTS);
        prv_mmFile_APTS = mmFile_APTS;

        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1)&0xFFFF;
        printf("MM:%04X|", tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, ADEC1)&0xFFFF;
        printf("ES=%04X(%04X),", ES_Lvl, tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCMISR_EMPTY_CNT, ADEC1)&0xFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ELAPSED_PCM_CNT, ADEC1)&0xFFFFFF;
        printf("PCM=%04X(%06X,%02X)|", PCM_Lvl, tmp_H, tmp_L);

        tmp_L = (MS_U32)HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD);
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_RECEIVE_STOP_CNT, ADEC1)&0x00FF;
        printf("Cmd=%02X,Stop=%02X,",  tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAYSMPFLAG, ADEC1)&0x1;
        printf("play=%X", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNTH_RATE, ADEC1)&0xFFFF;
        printf("<%04X>,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_STATE, ADEC1)&0x0F;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_FREERUN_TYPE, ADEC1)&0x0F;
        printf("State=%X(%X)|", tmp_L, tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1)&0x0000FFFF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1)&0x0000FFFF;
        printf("frmCnt:%04X,",     tmp_L);
        printf("%04X,",    tmp_H);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SKIP_FRMCNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_REPT_FRMCNT, ADEC1)&0x0000FF;
        printf("%02X,",    tmp_L);
        printf("%02X|",    tmp_H);

        //tmp_L = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_DecStatus+SYS_DECSTATUS_ID, DSP_MEM_TYPE_DM)&0x00FF;
        //printf("DEC_ID=%02X,", tmp_L);

        //tmp_L = HAL_MAD_Read_DSP_sram(DSP1DmAddr_sys_DecStatus+SYS_DECSTATUS_DEC_TYPE, DSP_MEM_TYPE_DM)&0x00FF;
        //printf("Type=%02X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFF;
        printf("pState=%04X,", tmp_L);

        tmp_L = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CALLING_CNT, ADEC1)&0x0000FF;
        tmp_H = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SYNC_MISS_CNT, ADEC1)&0x00000FFF;
        printf("Call:%02X,",    tmp_L);
        printf("Miss:%03X\n",  tmp_H);
    }

}


#ifndef MSOS_TYPE_NUTTX
MS_BOOL HAL_AUDIO_ResetTeeInfoShmToDefault(void)
{
    if (pAudioTeeInfoShm == NULL)
    {
        return FALSE;
    }
    memset(pAudioTeeInfoShm, 0x00, sizeof(AUDIO_TEE_INFO_SHARE_MEM));
    audio_tee_enabled = g_bAudioTeeEnabled;
    audio_tee_mbx_initialized = g_bAudioTeeMbxInitialized;
    dec_dsp_secure_tee_accessed = g_bDecDspSecureTeeAccessed;
    snd_dsp_secure_tee_accessed = g_bSndDspSecureTeeAccessed;
    return TRUE;
}

AUDIO_TEE_INFO_SHARE_MEM *HAL_AUDIO_AllocateTeeInfoShm(void)
{
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    AUDIO_TEE_INFO_SHARE_MEM * virtAddr = 0;
    MS_U32 u32BufSize = 0;

    if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_QUERY) == TRUE)
    {
        pAudioTeeInfoShm = (AUDIO_TEE_INFO_SHARE_MEM *) virtAddr;

        if (pAudioTeeInfoShm->g_u32ClientCounter == 0)
        {
            HALAUDIO_PRINT("reset Audio TEE Info SHM data ...\n");
            HAL_AUDIO_ResetTeeInfoShmToDefault();
        }
    }
    else
    {
        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_CREATE) == TRUE)
        {
            HALAUDIO_PRINT("Audio TEE Info SHM is created...\n");
            pAudioTeeInfoShm = (AUDIO_TEE_INFO_SHARE_MEM *) virtAddr;

            HALAUDIO_PRINT("reset Audio TEE Info SHM data ...\n");
            HAL_AUDIO_ResetTeeInfoShmToDefault();
        }
        else
        {
            HALAUDIO_ERROR("SHM allocation for Audio TEE Info failed!\n");
            return NULL;
        }
    }

    if (g_bAudioTeeInfoShmInitFlag == FALSE)
    {
        g_bAudioTeeInfoShmInitFlag = TRUE;
        pAudioTeeInfoShm->g_u32ClientCounter++;
    }
#else
    pAudioTeeInfoShm = &gAudioTeeInfoShm;

    if (g_bAudioTeeInfoShmInitFlag == FALSE)
    {
        HALAUDIO_PRINT("Audio TEE Info SHM is created...\n");
        g_bAudioTeeInfoShmInitFlag = TRUE;

        HAL_AUDIO_ResetTeeInfoShmToDefault();
        pAudioTeeInfoShm->g_u32ClientCounter++;
    }
#endif
    return pAudioTeeInfoShm;
}

MS_BOOL HAL_AUDIO_DeAllocateTeeInfoShm(void)
{
    AUDIO_TEE_INFO_SHARE_MEM * pAUDIOShared = NULL;

#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS))
    MS_U32 u32ShmId = 0;
    MS_VIRT virtAddr = 0;
    MS_U32 u32BufSize = 0;

    if (g_bAudioTeeInfoShmInitFlag == TRUE)
    {
        g_bAudioTeeInfoShmInitFlag = FALSE;

        if (MsOS_SHM_GetId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, sizeof(AUDIO_TEE_INFO_SHARE_MEM), &u32ShmId, (MS_VIRT *)&virtAddr, &u32BufSize, MSOS_SHM_QUERY) == FALSE)
        {
            HALAUDIO_ERROR("%s() : No AUDIO TEE Info SHM is created!\n", __FUNCTION__);
            return FALSE;
        }

        pAUDIOShared = (AUDIO_TEE_INFO_SHARE_MEM *)virtAddr;
        pAUDIOShared->g_u32ClientCounter--;

        if (pAUDIOShared->g_u32ClientCounter == 0)
        {
            printf("free Audio TEE Info SHM data ...\n");
#if defined(MSOS_TYPE_LINUX)
            if (MsOS_SHM_FreeId((MS_U8 *)AUDIO_TEE_INFO_SHM_NAME, u32ShmId) == TRUE)
            {
                HALAUDIO_PRINT("Audio TEE Info SHM data is freed\n");
            }
            else
            {
                HALAUDIO_ERROR("%s() : AUDIO TEE Info SHM de-allocation failed!\n", __FUNCTION__);
                return FALSE;
            }
#endif
        }
    }
#else
    pAUDIOShared = &gAudioTeeInfoShm;

    if (g_bAudioTeeInfoShmInitFlag == TRUE)
    {
        HALAUDIO_PRINT("free Audio TEE Info SHM data ...\n");
        g_bAudioTeeInfoShmInitFlag = FALSE;

        memset(pAUDIOShared, 0x00, sizeof(AUDIO_TEE_INFO_SHARE_MEM));
    }
#endif

    return TRUE;
}

MS_BOOL HAL_AUDIO_RegisterMBX(void)
{
    MS_U8 ClassNum=0;
    MBX_Result result;
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 10000;

    if (pAudioTeeInfoShm == NULL)
        return FALSE;
    if (audio_tee_mbx_initialized == TRUE)
        return TRUE;

#if 1
    eHKCPU = E_MBX_CPU_MIPS;
    if ( E_MBX_SUCCESS != MApi_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        DBG_AUDIO_ERROR("Error> MBX init failed !!\n");
        return FALSE;
    }
    else
    {
        MApi_MBX_Enable(TRUE);
    }
#endif

    if (E_MBX_SUCCESS != MApi_MBX_QueryDynamicClass (E_MBX_CPU_MIPS_VPE1, "MAD", (MS_U8 *)&ClassNum))
    {
        DBG_AUDIO_ERROR("MAD MApi_MBX_QueryDynamicClass fail\n");

        return FALSE;
    }
    result = MApi_MBX_RegisterMSG(ClassNum, 10);
    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        DBG_AUDIO_ERROR("HAL_AUDIO_RegisterMBX fail");
        return FALSE;
    }
    else
    {
        TEE_MBX_MSG_CLASS_SET(ClassNum);
        DBG_AUDIO("HAL_AUDIO_RegisterMBX ok");
        return TRUE;
    }
}

TEE_TO_REE_MBX_ACK_MSG_TYPE HAL_AUDIO_SendMBXMsg(REE_TO_TEE_MBX_MSG_TYPE msg_type)
{
    MS_U8 u8Index;
    MBX_Result result;
    REE_TO_TEE_MBX_MSG_TYPE msg_type1 = REE_TO_TEE_MBX_MSG_NULL;

    AUDIO_TEE_INFO_SHM_CHECK_NULL;

    if (audio_tee_enabled == FALSE)
    {
        return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
    }
    if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_DEC) == (REE_TO_TEE_MBX_MSG_TYPE_DEC))
    {
        if (dec_dsp_secure_tee_accessed == FALSE)
        {
            return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
        }
        else
        {
            msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_DEC);
        }
    }
    else if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_SE) == (REE_TO_TEE_MBX_MSG_TYPE_SE))
    {
        if (snd_dsp_secure_tee_accessed == FALSE)
        {
            return TEE_TO_REE_MBX_ACK_MSG_NO_TEE;
        }
        else
        {
            msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_SE);
        }
    }
    else if ((msg_type & REE_TO_TEE_MBX_MSG_TYPE_GENERAL) == (REE_TO_TEE_MBX_MSG_TYPE_GENERAL))
    {

        msg_type1 = msg_type & (~REE_TO_TEE_MBX_MSG_TYPE_GENERAL);
    }

    if (audio_tee_mbx_initialized == FALSE)
    {
        return TEE_TO_REE_MBX_ACK_MSG_INVALID;
    }
    REE_TO_TEE_MBX_MSG_INIT;
    MBX_MSG_SET_CLASSID(ree_to_tee_mbx_msg, tee_mbx_msg_class);
    MBX_MSG_SET_CMD(ree_to_tee_mbx_msg, msg_type1);

    result = MApi_MBX_SendMsg(&(ree_to_tee_mbx_msg));
    if (E_MBX_SUCCESS!= result)
    {
        return TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
    }
    // Receive Reply ACK from TEE side.
    memset(&(tee_to_ree_mbx_msg), 0, sizeof(MBX_Msg));
    MBX_MSG_SET_CLASSID(tee_to_ree_mbx_msg, tee_mbx_msg_class);
    do
    {
        result = MApi_MBX_RecvMsg(tee_mbx_msg_class, &(tee_to_ree_mbx_msg), 2000, MBX_CHECK_INSTANT_MSG);
    } while(E_MBX_SUCCESS != result);

    u8Index = tee_to_ree_mbx_msg.u8Index;
    DBG_AUDIO("Recv TEE Ack Msg OK cmd:%x\n", u8Index);

    if (TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL == u8Index)
    {
        DBG_AUDIO_ERROR("RecvMsg TEE_TO_REE_MBX_MSG_ACK_FAIL..\n");
        return TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
    }
    if (TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS == u8Index)
    {
        DBG_AUDIO("RecvMsg TEE_TO_REE_MBX_MSG_ACK_SUCCESS..\n");
    }

    return TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;
}
#else
static void _MAD_Proc(void)
{
    MS_U32              u32Events;
    MS_U8 u8Index = 0;
    MBX_Result result;

    DBG_AUDIO("_MAD_Proc...........\n");

    while (1)
    {
        MsOS_WaitEvent(_s32MadEventId, 1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        u8Index = msg.u8Index;

        memset(&TeeToReeMbxAckMsg, 0, sizeof(MBX_Msg));
        TEE_TO_REE_MBX_MSG_INIT(TeeToReeMbxAckMsg);
        MBX_MSG_SET_CLASSID(TeeToReeMbxAckMsg, msg.u8MsgClass);
        MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS);

        switch ( u8Index )
        {
            case REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode:
            {
                AUDIO_DSP_CODE_TYPE dspCodeType;
                dspCodeType = (msg.u8Parameters[0] || (msg.u8Parameters[1] << 8));
                if (HAL_AUDSP_DspLoadCode((AUDIO_DSP_CODE_TYPE)dspCodeType) == FALSE)
                {
                    MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL);
                }
                break;
            }

            case REE_TO_TEE_MBX_MSG_HAL_AUDIO_SetDspBaseAddr:
            {
                MS_U32 u32Bin_Base_Address, u32Mad_Base_Buffer_Adr;
                u32Bin_Base_Address = (((MS_U32)(msg.u8Parameters[1])) | ((MS_U32)(msg.u8Parameters[2]) << 8) | ((MS_U32)(msg.u8Parameters[3]) << 16) | ((MS_U32)(msg.u8Parameters[4]) << 24));
                u32Mad_Base_Buffer_Adr = (((MS_U32)(msg.u8Parameters[5])) | ((MS_U32)(msg.u8Parameters[6]) << 8) | ((MS_U32)(msg.u8Parameters[7]) << 16) | ((MS_U32)(msg.u8Parameters[8]) << 24));
                HAL_AUDIO_SetDspBaseAddr(msg.u8Parameters[0], u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);
                break;
            }

            default:
            {
                MBX_MSG_SET_CMD(TeeToReeMbxAckMsg, TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL);
                break;
            }
        }

        result = MApi_MBX_SendMsg(&TeeToReeMbxAckMsg);
        if (E_MBX_SUCCESS != result)
            DBG_AUDIO("MAD Reply Ack fail classid: %x cmd:%x \n", msg.u8MsgClass, TeeToReeMbxAckMsg.u8Index);
        else
            DBG_AUDIO("MAD Reply Ack ok classid:%x cmd:%x\n", msg.u8MsgClass, TeeToReeMbxAckMsg.u8Index);
    }
}

static void HAL_AUDIO_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    if(pMsg==NULL)
    {
        DBG_AUDIO("pMsg is a null pointer\n");
        return;
    }
    if (_s32MadEventId < 0)
    {
        DBG_AUDIO("MAD EventGroup Id is not valid...\n");
        return;
    }
    memcpy(&msg, pMsg, sizeof(MBX_Msg));
    MsOS_SetEvent(_s32MadEventId, 1);
    return;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_update_DspUsage()
/// @brief \b Function \b Description:  update DSP resource status when select input source
/// @param <IN>        \b AUDIO_SOURCE_INFO_TYPE : select input source type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_update_DspUsage(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    HALAUDIO_CHECK_SHM_INIT;

    /* same input source, not update */
    if ( g_AudioVars2->eMainSourceType == eSourceType )
    {
        return;
    }

    OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

    HALAUDIO_PRINT ("%s: PrevSource %d NowSource %d NewSource %d !!\r\n", __FUNCTION__, g_AudioVars2->ePrevSourceType, g_AudioVars2->eMainSourceType, eSourceType);

    /* release prev DSP resource */
    if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_DTV_IN && eSourceType != E_AUDIO_INFO_DTV_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID1].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release DEC DSP from DTV !!\r\n", __FUNCTION__);
    }
    else if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_ATV_IN && eSourceType != E_AUDIO_INFO_ATV_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release SND DSP from ATV !!\r\n", __FUNCTION__);
    }
    else if ( (g_AudioVars2->eMainSourceType == E_AUDIO_INFO_HDMI_IN && eSourceType != E_AUDIO_INFO_HDMI_IN) )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = FALSE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_NONE;
        HALAUDIO_PRINT ("%s: Release SND DSP from HDMI !!\r\n", __FUNCTION__);
    }

    g_AudioVars2->ePrevSourceType = g_AudioVars2->eMainSourceType;
    g_AudioVars2->eMainSourceType = eSourceType;

    /* lock main input source DSP resource */
    if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_DTV_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID1].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID1].eUsageType = AUDIO_USAGE_TYPE_DTV;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for DTV !!\r\n", __FUNCTION__);
    }
    else if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_ATV_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_ATV;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for ATV !!\r\n", __FUNCTION__);
    }
    else if ( g_AudioVars2->eMainSourceType == E_AUDIO_INFO_HDMI_IN )
    {
        g_AudioVars2->DspUsage[AU_DEC_ID3].bDecInUsed = TRUE;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eAudioType = Audio_DEC_NULL;
        g_AudioVars2->DspUsage[AU_DEC_ID3].eUsageType = AUDIO_USAGE_TYPE_HDMI;
        HALAUDIO_PRINT ("%s: Alloc DEC DSP for HDMI !!\r\n", __FUNCTION__);
    }

    HALAUDIO_PRINT ("%s: update PrevSource %d NewSource %d !!\r\n", __FUNCTION__, g_AudioVars2->ePrevSourceType, g_AudioVars2->eMainSourceType);

    OS_RELEASE_MUTEX(_s32MutexLoadCode);
}

Audio_id HAL_AUDIO_Convert_DecId_to_ADECId(AUDIO_DEC_ID dec_id)  //temp
{
    Audio_id ADEC_id;
    switch(dec_id)
    {
        case  AU_DEC_INVALID:
        case AU_DEC_MAX:
            printf("[Error] Can't convert...\n");
            ADEC_id = ADEC3; //no usage just take one for it...
            break;

        case  AU_DEC_ID1:
            ADEC_id = ADEC1;
            break;

        case AU_DEC_ID2:
        case AU_DEC_ID3:
            ADEC_id = ADEC2;
            break;

        default:
            ADEC_id = ADEC3;
            break;
        }
    return ADEC_id;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetCaps()
/// @brief \b Function \b Description: Get Audio Capabilities
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b MS_U32 * : Audio Capabilites pointer
/// @param <OUT>        \b MS_BOOL : return TRUE if success, else return FALSE
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps)
{
    AUDIO_CAPABILITIES *pCapsTmp = NULL;
    MS_U32 u32AudioCapsStructSize = sizeof(AUDIO_CAPABILITIES);

    HALAUDIO_CHECK_SHM_INIT;

    if (pCaps == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pCaps should not be NULL!\n", __FUNCTION__);
        return FALSE;
    }

    if ((DecId <= AU_DEC_INVALID) || (DecId >= AU_DEC_MAX) || (DecId == AU_DEC_ID2))
    {
        HALAUDIO_ERROR("%s: Error! Invalid DecId (%d)!\n", __FUNCTION__, DecId);
        return FALSE;
    }
    else if ((g_AudioVars2->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum == 1) && (DecId > AU_DEC_ID1))
    {
        HALAUDIO_ERROR("%s: Error! DecId (%d) is greater than max decoder number (%d)!\n", __FUNCTION__, DecId, g_AudioVars2->AudioCaps[AU_DEC_ID1].u8MaxDecoderNum);
        return FALSE;
    }

    pCapsTmp = (AUDIO_CAPABILITIES *)pCaps;

    if (pCapsTmp->u32AudioCapsVersion == 0)
    {
        HALAUDIO_ERROR("%s: Error! Invalid version number (%u) from function caller!\n", __FUNCTION__, pCapsTmp->u32AudioCapsVersion);
        return FALSE;
    }

    if (pCapsTmp->u32AudioCapsStructSize == 0)
    {
        HALAUDIO_ERROR("%s: Error! Invalid structure size (%u) from function caller!\n", __FUNCTION__, pCapsTmp->u32AudioCapsStructSize);
        return FALSE;
    }

    if (pCapsTmp->u32AudioCapsVersion != g_AudioVars2->AudioCaps[DecId].u32AudioCapsVersion)
    {
        HALAUDIO_ERROR("%s: Warning! function caller's version (%u) is not matched to driver's version (%u) at DecId (%d)\n", __FUNCTION__, pCapsTmp->u32AudioCapsVersion, g_AudioVars2->AudioCaps[DecId].u32AudioCapsVersion, DecId);

        u32AudioCapsStructSize = (u32AudioCapsStructSize <= pCapsTmp->u32AudioCapsStructSize) ? u32AudioCapsStructSize : pCapsTmp->u32AudioCapsStructSize;
    }

    memcpy((void *)pCapsTmp, (void *)&g_AudioVars2->AudioCaps[DecId], u32AudioCapsStructSize);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Set_DecimationMode()
/// @brief \b Function \b Description: HAL_AUDIO_Set_Decimation register
/// @param <IN>        \b AUDIO_OUTPUT_TYPE Output_Type: Audio Output type
/// @param <IN>        \b MS_BOOL Enable : Audio ID: ADEC ID
////////////////////////////////////////////////////////////////////////////////
void  HAL_AUDIO_Set_DecimationMode(AUDIO_OUTPUT_TYPE Output_Type, Audio_id ID)
{
    switch(Output_Type)
    {
        case AUDIO_HDMI_OUTPUT:  //high rate nonPCM application need decimation
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL+1)&0x20))
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x04, 0x04);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x10, 0x10);
                }
            else
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x04, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x10, 0x00);
                }
            break;

        case AUDIO_HDMI_ARC_OUTPUT:
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_HDMITx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL)&0x02))
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x02, 0x02);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x08, 0x08);
                }
            else
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x02, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x08, 0x00);
                }
            break;

        case AUDIO_SPDIF_OUTPUT:
            if( (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SPDIFTx_SMP_RATE, ID) > 48000) && (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL)&0x02))
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x01, 0x01);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x04, 0x04);
                }
            else
                {
                HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECIMATION_CFG, 0x01, 0x00);
                HAL_AUDIO_WriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x04, 0x00);
                }
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_BOOL : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_CheckPlayDone(void)
{
    if (((HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_okFrmCnt) !=0) ||
          (HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_errFrmCnt) !=0)) &&
        (HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_pcmBuf_currLevel) < AUD_R2_DMA_BURST_BYTES))
    {
        CheckPlayDoneCnt = CheckPlayDoneCnt + 1;

        if (CheckPlayDoneCnt == 3)
        {
            CheckPlayDoneCnt = 0;
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        CheckPlayDoneCnt = 0;
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_CheckVersionInfo()
/// @brief \b Function \b Description: Check Version info
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_BOOL : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_CheckVersionInfo(void)
{
    HALAUDIO_PRINT("\r\n \033[1;32m==========Audio DSP Version Check==================\033[0m");
    HALAUDIO_PRINT("\r\n \033[1;32m         DSP System:0x%06X    \033[0m \r\n", system_version_num);

    return TRUE;
}

MS_S32 HAL_AUDIO_PCM_Dump_Setting(MS_U32 u32alue, MS_U32 *pData)
{
    return 0;
}

MS_S32 HAL_AUDIO_PCM_Check_U32_Param_Valid(MS_U32 u32alue, MS_U32 *pData, MS_U32 u32Count)
{
    MS_U32 u32Loop = 0;
    MS_S32 s32Ret = -A_EINVAL;

    if (u32Count == 0)
    {
        return -A_EINVAL;
    }

    if (pData != NULL)
    {
        for (u32Loop = 0; u32Loop < u32Count ; u32Loop++)
        {
            if (pData[u32Loop] == 0)
            {
                break;
            }

            if (u32alue == pData[u32Loop])
            {
                s32Ret = 0;
                break;
            }
        }
    }
    else
    {
        s32Ret = -A_EFAULT;
    }

    return s32Ret;
}

#if AUDIO_HW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Divisor = 0;
    MS_U16 u16Synthrate = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    /*
     * synthesizer Rate = 216MHz / divisor(1, 2, 4 or 8) * 1024 / 256 / sampling rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x6978;
            break;
        }

        case 11025:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4C87;
            break;
        }

        case 12000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4650;
            break;
        }

        case 16000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x6978;
            break;
        }

        case 22050:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4C87;
            break;
        }

        case 24000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4650;
            break;
        }

        case 32000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x6978;
            break;
        }

        case 44100:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4C87;
            break;
        }

        case 48000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            break;
        }

        default:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* enable DMA synthesizer */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG0, 0x20, 0x20);

    /* Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH &  synthesizer N.F. */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_CLK_CFG6, 0x6000, (u16Divisor << 13));
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_SYNTH_NF, 0xFFFF, u16Synthrate);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > (HW_DMA_RDR1_BUF_SIZE / 2))
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR1_BUF_SIZE / 2;
        pPcmInfo->u32BufferDuration = (u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    u32TargetBufferSize = u32TargetBufferSize * 2; /* for HW DMA Reader specification */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_3, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);

    /* set overrun & underrun threshold */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_4, 0xFFFF, u32TargetBufferSize - 12);
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_5, 0xFFFF, 0x0012);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_1, 0xFFFF, 0x0000);

    /* reset & start engine */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0xFFFE, 0x6402);
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0xFFFE, 0x2402);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > (HW_DMA_RDR1_BUF_SIZE / 2))
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR1_BUF_SIZE / 2;
        pUserPcmInfo->u32BufferDuration = (u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DMA_RD_CTRL_7, 0xFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFF)); /* RD_BASE_ADDR[7:0] */
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_2, 0xFFFF, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 8) & 0xFFFF)); /* RD_BASE_ADDR[23:8] */
    HAL_AUDIO_WriteMaskByte((REG_AUDIO_DMA_RD_CTRL_7 + 1), 0x0F, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 24) & 0x0F)); /* RD_BASE_ADDR[27:24] */

    /* set SEL_CLK_DMA_READER */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CLK_CFG6, 0x07, 0x04);

    /* apply setting */
    HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_HwDma_Reader1_Restart();

    /* TODO : force channel8 to HW DMA Reader1, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(0x2CB0, 0x07, 0x04);
    HAL_AUDIO_WriteMaskByte(0x2C6B, 0xFF, 0x8F);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_HwDma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        }

        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0001, 0x0001);
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0100, 0x0100);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0001, 0x0000);
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0100, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            pPcmInfo->u32Channel = u32Channel;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0010, 0x0010);
            pPcmInfo->u32BufferLevel = (HAL_AUDIO_ReadReg(REG_AUDIO_DMA_RD_CTRL_6) * BYTES_IN_MIU_LINE) / 2;
            HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_0, 0x0010, 0x0000);
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;
            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUDIO_ReadReg(REG_AUDIO_DMA_RD_CTRL_3) * BYTES_IN_MIU_LINE) / 2; /* return valid size */
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSampleCount = 0;
    MS_U32 u32Loop = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_HwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    u32PcmLevel = u32PcmLevel * 2;
    if ((u32PcmLevel / BYTES_IN_MIU_LINE) <= 6)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_HwDma_Reader1_Stop();
        HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        HAL_AUDIO_PCM_HwDma_Reader1_Start();
    }

    u32RequestSize = u32Size * 2;
    u32RequestSampleCount = u32Size / 2;

    HAL_AUDIO_PCM_HwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);
    u32BufferSize = u32BufferSize * 2;

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        for (u32Loop = 0; u32Loop < u32RequestSampleCount; u32Loop++)
        {
            *pPCM->tPcmBufferInfo.pWritePtr++ = 0;
            *pPCM->tPcmBufferInfo.pWritePtr++ = *pBufTmp++; /* L low, R low, L low, R low, ... */
            *pPCM->tPcmBufferInfo.pWritePtr++ = *pBufTmp++; /* L high, R high, L high, R high, ... */
            *pPCM->tPcmBufferInfo.pWritePtr++ = 0;

            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }
        }

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUDIO_WriteMaskReg(REG_AUDIO_DMA_RD_CTRL_1, 0xFFFF, (u32RequestSize/BYTES_IN_MIU_LINE));
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;
#ifdef MSOS_TYPE_LINUX_KERNEL
        udelay(50);
#else
        AUDIO_DELAY1US(50);
#endif

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader1_Restart();
        HAL_AUDIO_PCM_HwDma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Divisor = 0;
    MS_U16 u16Synthrate = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    /*
     * synthesizer Rate = 216MHz / divisor(1, 2, 4 or 8) * 1024 / 256 / sampling rate(32Khz, 44.1KHz or 48KHz)
     */
    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x6978;
            break;
        }

        case 11025:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4C87;
            break;
        }

        case 12000:
        {
            u16Divisor = 2;
            u16Synthrate = 0x4650;
            break;
        }

        case 16000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x6978;
            break;
        }

        case 22050:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4C87;
            break;
        }

        case 24000:
        {
            u16Divisor = 1;
            u16Synthrate = 0x4650;
            break;
        }

        case 32000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x6978;
            break;
        }

        case 44100:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4C87;
            break;
        }

        case 48000:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            break;
        }

        default:
        {
            u16Divisor = 0;
            u16Synthrate = 0x4650;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* Configure REG_SEL_CLK_214MHZ_DMA_RD_FIX_SYNTH &  synthesizer N.F. */
    HAL_AUDIO_AbsWriteReg(NewDMAreader_ctrl, 0x0000);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Enable_ctrl, 0x0030, (u16Divisor << 4));
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Syth, 0xFFFF, u16Synthrate);

    /* enable DMA synthesizer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_Enable_ctrl, 0x0145, 0x0145);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR2_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_DRAM_size, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);

    /* set overrun & underrun threshold */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_DRAM_underrun_threshold, 0xFFFF, 0x0012);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_RDR2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_CPU_triggersize, 0xFFFF, 0x0000);

    /* reset & start engine */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
#ifdef MSOS_TYPE_LINUX_KERNEL
    udelay(50);
#else
    AUDIO_DELAY1US(50);
#endif
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pUserPcmInfo->u32BufferDuration, (unsigned int)pUserPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_RDR2_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);

    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Reader2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_RDR2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_BaseAddress_Lo, 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* RD_BASE_ADDR[15:0] */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_BaseAddress_Hi, 0xFFFF, (((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16) & 0xFFFF)); /* RD_BASE_ADDR[31:15] */

    /* apply setting */
    HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_HwDma_Reader2_Restart();

    /* TODO : force channel6 to HW DMA Reader2, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF, 0x90);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_HwDma_Reader2_Apply_Setting();
            HAL_AUDIO_PCM_HwDma_Reader2_Restart();
        }

        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0001, 0x0001);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0002, 0x0002);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0001, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0002, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, (unsigned int)u32Channel);
                return -A_EINVAL;
            }

            pPcmInfo->u32Channel = u32Channel;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, (unsigned int)u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0020, 0x0020);
                u32BufferLevel1 = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_levelcnt) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_levelcnt) * BYTES_IN_MIU_LINE;
                HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0020, 0x0000);

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, u32BufferLevel1, u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = HAL_AUDIO_AbsReadReg(NewDMAreader_DRAM_size) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Reader2_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_HwDma_Reader2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_HwDma_Reader2_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_CPU_triggersize, 0xFFFF, u32RequestSize/BYTES_IN_MIU_LINE);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAreader_ctrl, 0x0010, 0x0010);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Reader2_Restart();
        HAL_AUDIO_PCM_HwDma_Reader2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* configure sample rate */
#ifdef M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H //TODO : need to remove this, DSP side first then Driver side.
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_SynthFreq_H, 0xFFFF, u16Synthrate_H);
#endif
    HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_H, u16Synthrate_H, DSP_MEM_TYPE_DM);
    HAL_MAD2_Write_DSP_sram(DSP2DmAddr_swDmaRdr_synthFreq_L, u16Synthrate_L, DSP_MEM_TYPE_DM);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > SW_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate);
        u32TargetBufferSize = SW_DMA_RDR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, SW_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* clear engine's write pointer */
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, 0x0000);

    /* reset engine, no start engine here! */
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0xFFFF, 0x0001);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > SW_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate);
        u32TargetBufferSize = SW_DMA_RDR1_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_SwDma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + SW_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* apply setting */
    HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_SwDma_Reader1_Restart();

    /* TODO : force channel6 to SW DMA Reader, it's a hardcode */
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_DECODER3_CFG, 0x0F,0x00);
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_CH6_CFG, 0xFF, 0x82);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_SwDma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_SwDma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_SwDma_Reader1_Restart();
        }

        /* start engine */
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0xFFFF, 0x0002);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* stop engine */
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0xFFFF, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0008);
            pPcmInfo->u32BufferLevel = HAL_AUDIO_ReadReg(S2M_MBOX_SW_DMA_READER_DDR_Level) * BYTES_IN_MIU_LINE;
            HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_Ctrl, 0x0008, 0x0000);
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;
            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            MS_U32 u32TargetBufferSize = 0;
            MS_U32 u32TargetChannel = 0;
            MS_U32 u32TargetBitWidth = 0;

            u32TargetChannel = 2;
            u32TargetBitWidth = 16;
            u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;

            *((MS_U32 *)pData) = u32TargetBufferSize;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_SwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32WptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_SwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_SwDma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + SW_DMA_RDR1_BUF_SIZE))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update write pointers to engine */
        u32WptrOffset = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteMaskReg(M2S_MBOX_SW_DMA_READER_DDR_WtPtr, 0xFFFF, (u32WptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_SwDma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_SwDma_Reader1_Restart();
        HAL_AUDIO_PCM_SwDma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Val = 0;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* disable output */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x7FFF, 0x0000);

    /* set synthesizer */
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_L, 0xFFFF, u16Synthrate_L);
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_REG_SYNTH_H, 0xFFFF, u16Synthrate_H);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0101); /* SYTH1 bit8RIU control, bit0 update */
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0101, 0x0100);

    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 3), 0xFFFF, (u32TargetBufferSize / BYTES_IN_MIU_LINE)); /* SIZE [15:0] */

    /* set overrun & underrun threshold */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 5), 0xFFFF, ((u32TargetBufferSize - (u32TargetBufferSize >> 3)) / BYTES_IN_MIU_LINE));
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 6), 0xFFFF, ((u32TargetBufferSize >> 3) / BYTES_IN_MIU_LINE));

    /* set channel mode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* Multi-Channel mode : 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x80, 0x00);
        u16Val = 19; /* MIU burst length = 19 */
        u16Val += (1 << 7); /* MIU Step En = 1 */
        u16Val += (1 << 15); /* Sync Valid En = 1 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 7), 0xFFFF, u16Val);
    }
    else
    {
        /* Stereo mode : stereo = 2 channels */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x80, 0x80);
        u16Val = 3; /* MIU burst length = 3 */
        u16Val += (0 << 7); /* MIU Step En = 0 */
        u16Val += (0 << 15); /* Sync Valid En = 0 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 7), 0xFFFF, u16Val);
    }

    /* set 16 Bit mode */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0860, 0x0020);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* stop engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0002, 0x0000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, R2_DMA_RDR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* initial dlocal buffer */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x2000, 0x2000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x2000, 0x0000);

    /* reset engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x1000, 0x1000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x1000, 0x0000);

    /* start engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0002, 0x0002);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR1_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_R2Dma_Reader1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init DPGA */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_EN, 0x03, 0x03);
    HAL_AUR2_WriteMaskByte((R2_DMARDR1_DPGA_REG_L_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte((R2_DMARDR1_DPGA_REG_R_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_DPGA_REG_L_CTRL, 0x12, 0x12); /* auto mode and fading */
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_DPGA_REG_R_CTRL, 0x12, 0x12);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + R2_DMA_RDR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* switch R2_DMA & DPGA control to MCU */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x8000, 0x8000);
    HAL_AUR2_WriteMaskReg(R2_DMARDR1_EN, 0x0001, 0x0001);

    /* set PCM buffer address */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 1), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* BASE [15:0] */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 2), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16)); /* BASE [26:16] */

    /* apply setting */
    HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_R2Dma_Reader1_Restart();

    /* TODO : force channel 1~4 to decoder1 when multi-channel output is configured, it's a hardcode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x80);
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    /* Switch R2_DMA & DPGA control to R2 */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x8000, 0x0000);
    HAL_AUR2_WriteMaskByte(R2_DMARDR1_EN, 0x01, 0x00);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_R2Dma_Reader1_Apply_Setting();
            HAL_AUDIO_PCM_R2Dma_Reader1_Restart();
        }

        /* enable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0001, 0x0001);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* disable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0001, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = u8MultiChFlag;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 10))) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 10))) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, u32BufferLevel1, u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 3))) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_R2Dma_Reader1_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    /*
     * A patch here!
     * to ensure channel 1~4 are configured as decoder1
     */
    if ((HAL_AUDIO_ReadByte(REG_AUDIO_CH1_CFG) != 0x80) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH2_CFG) != 0x80) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH3_CFG) != 0x80) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH4_CFG) != 0x80))
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x80);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x80);
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_R2Dma_Reader1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_R2Dma_Reader1_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 4), 0xFFFF, (u32RequestSize / BYTES_IN_MIU_LINE));
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0400, 0x0400);
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR1_REG_BASE, 0), 0x0400, 0x0000);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader1_Restart();
        HAL_AUDIO_PCM_R2Dma_Reader1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U16 u16Val = 0;
    MS_U16 u16Synthrate_H = 0;
    MS_U16 u16Synthrate_L = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;

    switch(pPcmInfo->u32SampleRate) {
        case 8000:
        {
            u16Synthrate_H = 0x6978;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 11025:
        {
            u16Synthrate_H = 0x4C87;
            u16Synthrate_L = 0xD634;
            break;
        }

        case 12000:
        {
            u16Synthrate_H = 0x4650;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 16000:
        {
            u16Synthrate_H = 0x34BC;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 22050:
        {
            u16Synthrate_H = 0x2643;
            u16Synthrate_L = 0xEB1A;
            break;
        }

        case 24000:
        {
            u16Synthrate_H = 0x2328;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 32000:
        {
            u16Synthrate_H = 0x1A5E;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 44100:
        {
            u16Synthrate_H = 0x1321;
            u16Synthrate_L = 0xF58D;
            break;
        }

        case 48000:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            break;
        }

        case 96000:
        {
            u16Synthrate_H = 0x08CA;
            u16Synthrate_L = 0x0000;
            break;
        }

        default:
        {
            u16Synthrate_H = 0x1194;
            u16Synthrate_L = 0x0000;
            pPcmInfo->u32SampleRate = 48000;
            break;
        }
    }

    /* disable output */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x7FFF, 0x0000);

    /* set synthesizer */
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_REG_SYNTH_L, 0xFFFF, u16Synthrate_L);
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_REG_SYNTH_H, 0xFFFF, u16Synthrate_H);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0400);
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0404); /* SYTH3 bit10 RIU control, bit3 update */
    HAL_AUR2_WriteMaskReg(R2_DMARDR_REG_SYNTH_UPDATE, 0x0404, 0x0400);

    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, u32TargetBufferSize, pPcmInfo->u32BufferDuration, pPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR2_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 3), 0xFFFF, (u32TargetBufferSize / BYTES_IN_MIU_LINE)); /* SIZE [15:0] */

    /* set overrun & underrun threshold */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 5), 0xFFFF, ((u32TargetBufferSize - (u32TargetBufferSize>> 3)) / BYTES_IN_MIU_LINE));
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 6), 0xFFFF, ((u32TargetBufferSize >> 3) / BYTES_IN_MIU_LINE));

    /* set channel mode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        /* Multi-Channel mode : 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x80, 0x00);
        u16Val = 19; /* MIU burst length = 19 */
        u16Val += (1 << 7); /* MIU Step En = 1 */
        u16Val += (1 << 15); /* Sync Valid En = 1 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 7), 0xFFFF, u16Val);
    }
    else
    {
        /* Stereo mode : stereo = 2 channels */
        HAL_AUR2_WriteMaskByte(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x80, 0x80);
        u16Val = 3; /* MIU burst length = 3 */
        u16Val += (0 << 7); /* MIU Step En = 0 */
        u16Val += (0 << 15); /* Sync Valid En = 0 */
        u16Val += (1 << 8); /* Sync Step Count = 1 */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 7), 0xFFFF, u16Val);
    }

    /* set 16 Bit mode */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0860, 0x0020);

    pPCM->u8SettingChangeFlag = FALSE;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* stop engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0002, 0x0000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, R2_DMA_RDR2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* initial dlocal buffer */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x2000, 0x2000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x2000, 0x0000);

    /* reset engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x1000, 0x1000);
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x1000, 0x0000);

    /* start engine */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0002, 0x0002);

    /* reset write pointer */
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* reset remain size */
    pPCM->tPcmBufferInfo.u32RemainSize = 0;

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if ((pUserPcmInfo->u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support capture mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        /* 2 + 7.1 = 10 channels (DmxL, DmxR, L, C, R, Ls, Rs, LFE, Lrs, Rrs) */
        u32TargetChannel = 10;
    }
    else
    {
        /* stereo = 2 channels */
        u32TargetChannel = 2;
    }

    /* calculate buffer size */
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pUserPcmInfo->u32BufferDuration * pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > R2_DMA_RDR2_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u, channel %u\n", __FUNCTION__, u32TargetBufferSize, pUserPcmInfo->u32BufferDuration, pUserPcmInfo->u32SampleRate, u32TargetChannel);
        u32TargetBufferSize = R2_DMA_RDR2_BUF_SIZE;
        pUserPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pUserPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, pUserPcmInfo->u32BufferDuration);
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BUFFERDURATION, &pUserPcmInfo->u32BufferDuration);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_CHANNEL, &pUserPcmInfo->u32Channel);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_SAMPLERATE, &pUserPcmInfo->u32SampleRate);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_R2Dma_Reader2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init DPGA */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_EN, 0x03, 0x03);
    HAL_AUR2_WriteMaskByte((R2_DMARDR2_DPGA_REG_L_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte((R2_DMARDR2_DPGA_REG_R_CTRL + 1), 0x07,0x07); /* DPGA fading duartion 18ms */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_DPGA_REG_L_CTRL, 0x12, 0x12); /* auto mode and fading */
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_DPGA_REG_R_CTRL, 0x12, 0x12);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + R2_DMA_RDR2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* switch R2_DMA & DPGA control to MCU */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x8000, 0x8000);
    HAL_AUR2_WriteMaskReg(R2_DMARDR2_EN, 0x0001, 0x0001);

    /* set PCM buffer address */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 1), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) & 0xFFFF)); /* BASE [15:0] */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 2), 0xFFFF, ((u32TargetBufferAddrPa / BYTES_IN_MIU_LINE) >> 16)); /* BASE [26:16] */

    /* apply setting */
    HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting();

    /* restart */
    HAL_AUDIO_PCM_R2Dma_Reader2_Restart();

    /* TODO : force channel 1~4 to decoder3 when multi-channel output is configured, it's a hardcode */
    if (pPcmInfo->u8MultiChFlag == TRUE)
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x82);
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    /* switch R2_DMA & DPGA control to R2 */
    HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x8000, 0x0000);
    HAL_AUR2_WriteMaskByte(R2_DMARDR2_EN, 0x01, 0x00);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        if (pPCM->u8SettingChangeFlag == TRUE)
        {
            HAL_AUDIO_PCM_R2Dma_Reader2_Apply_Setting();
            HAL_AUDIO_PCM_R2Dma_Reader2_Restart();
        }

        /* enable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0001, 0x0001);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* disable engine's MIU access */
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0001, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = u8MultiChFlag;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            MS_U32 u32Channel = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Channel(%u)!\n", __FUNCTION__, u32Channel);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32Channel != u32Channel)
            {
                pPcmInfo->u32Channel = u32Channel;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            MS_U32 u32SampleRate = *((MS_U32 *)pData);

            if (HAL_AUDIO_PCM_Check_U32_Param_Valid(u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32SampleRate(%u)!\n", __FUNCTION__, u32SampleRate);
                return -A_EINVAL;
            }

            if (pPcmInfo->u32SampleRate != u32SampleRate)
            {
                pPcmInfo->u32SampleRate = u32SampleRate;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 10))) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 10))) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, u32BufferLevel1, u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = (HAL_AUR2_ReadReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 3))) * BYTES_IN_MIU_LINE;
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_R2Dma_Reader2_Write(const void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    /*
     * A patch here!
     * to ensure channel 1~4 are configured as decoder3
     */
    if ((HAL_AUDIO_ReadByte(REG_AUDIO_CH1_CFG) != 0x82) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH2_CFG) != 0x82) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH3_CFG) != 0x82) ||
        (HAL_AUDIO_ReadByte(REG_AUDIO_CH4_CFG) != 0x82))
    {
        HAL_AUDIO_WriteByte(REG_AUDIO_CH1_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH2_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH3_CFG, 0x82);
        HAL_AUDIO_WriteByte(REG_AUDIO_CH4_CFG, 0x82);
    }

    pBufTmp = (MS_S8 *)pBuf;

    HAL_AUDIO_PCM_R2Dma_Reader2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel == 0)
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    HAL_AUDIO_PCM_R2Dma_Reader2_Get(AUDIO_PCM_CMD_BUFFERSIZE, &u32BufferSize);

    /* copy data to PCM buffer */
    if ((u32PcmLevel + u32RequestSize) <= u32BufferSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pWritePtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pPCM->tPcmBufferInfo.pWritePtr, (void *)pBufTmp, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pWritePtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pWritePtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pWritePtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update copied size to engine */
        u32RequestSize += pPCM->tPcmBufferInfo.u32RemainSize;
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 4), 0xFFFF, (u32RequestSize / BYTES_IN_MIU_LINE));
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0400, 0x0400);
        HAL_AUR2_WriteMaskReg(AUDIO_R2_DMA_RDER_CFG_REG(R2_DMARDR2_REG_BASE, 0), 0x0400, 0x0000);
        pPCM->tPcmBufferInfo.u32RemainSize = u32RequestSize % BYTES_IN_MIU_LINE;

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_R2Dma_Reader2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_R2Dma_Reader2_Restart();
        HAL_AUDIO_PCM_R2Dma_Reader2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32WritePtrOffset = 0;
    MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, 0x0000);

    do {
        u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) * BYTES_IN_MIU_LINE;

        if (u32WritePtrOffset != 0)
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
            udelay(50);
#else
            AUDIO_DELAY1US(50);
#endif
            u32TimeoutCounter++;
        }
        else
        {
            break;
        }
    } while (u32TimeoutCounter < 10000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, PCM_CAPTURE1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* reset PCM capture1 read & write pointer */
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, 0x0000);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((PCM_CAPTURE1_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_Capture1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* restart */
    HAL_AUDIO_PCM_Capture1_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_Capture1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_Size, (u32BufferSize / BYTES_IN_MIU_LINE));

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure buffer duration!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32WritePtrOffset = 0;
            MS_U32 u32BufferSize = 0;
            MS_S32 s32PcmLevel = 0;

            u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE_DDR_WrPtr) * BYTES_IN_MIU_LINE;
            pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer + u32WritePtrOffset;

            s32PcmLevel = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pReadPtr;
            if (s32PcmLevel < 0)
            {
                u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
                s32PcmLevel += u32BufferSize;
            }

            pPcmInfo->u32BufferLevel = (MS_U32)s32PcmLevel;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = PCM_CAPTURE1_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_Capture1_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32RptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_Capture1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel >= (u32BufferSize - PCM_CAPTURE1_BUF_UNIT))
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_Capture1_Stop();
        HAL_AUDIO_PCM_Capture1_Restart();
        HAL_AUDIO_PCM_Capture1_Start();
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update read pointer to engine */
        u32RptrOffset = pPCM->tPcmBufferInfo.pReadPtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE_DDR_RdPtr, (u32RptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_Capture1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_Capture1_Restart();
        HAL_AUDIO_PCM_Capture1_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture2_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32WritePtrOffset = 0;
    MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, 0x0000);

    do {
        u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) * BYTES_IN_MIU_LINE;

        if (u32WritePtrOffset != 0)
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
            udelay(50);
#else
            AUDIO_DELAY1US(50);
#endif
            u32TimeoutCounter++;
        }
        else
        {
            break;
        }
    } while (u32TimeoutCounter < 10000);

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, PCM_CAPTURE2_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();

    /* reset PCM capture1 read & write pointer */
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;
    HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, 0x0000);

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((PCM_CAPTURE2_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_Capture2_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + PCM_CAPTURE2_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* restart */
    HAL_AUDIO_PCM_Capture2_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_Capture2_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_Size, (u32BufferSize / BYTES_IN_MIU_LINE));

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure buffer duration!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32WritePtrOffset = 0;
            MS_U32 u32BufferSize = 0;
            MS_S32 s32PcmLevel = 0;

            u32WritePtrOffset = HAL_AUDIO_ReadReg(S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr) * BYTES_IN_MIU_LINE;
            pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer + u32WritePtrOffset;

            s32PcmLevel = pPCM->tPcmBufferInfo.pWritePtr - pPCM->tPcmBufferInfo.pReadPtr;
            if (s32PcmLevel < 0)
            {
                u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
                s32PcmLevel += u32BufferSize;
            }

            pPcmInfo->u32BufferLevel = (MS_U32)s32PcmLevel;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = PCM_CAPTURE2_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_Capture2_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    MS_U32 u32RptrOffset = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_Capture2_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);
    if (u32PcmLevel >= (u32BufferSize - PCM_CAPTURE2_BUF_UNIT))
    {
        HALAUDIO_ERROR("%s: Warning! PCM buffer full!\n", __FUNCTION__);

        u32PcmLevel = 0;
        HAL_AUDIO_PCM_Capture2_Stop();
        HAL_AUDIO_PCM_Capture2_Restart();
        HAL_AUDIO_PCM_Capture2_Start();
    }

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        /* flush MIU */
        MsOS_FlushMemory();

        /* update read pointer to engine */
        u32RptrOffset = pPCM->tPcmBufferInfo.pReadPtr - pPCM->tPcmBufferInfo.pBuffer;
        HAL_AUDIO_WriteReg(M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr, (u32RptrOffset / BYTES_IN_MIU_LINE));

        return u32Size;
    }

    //HALAUDIO_ERROR("%s: Warning! PCM buffer empty!\n", __FUNCTION__);

    return 0;
}

MS_S32 HAL_AUDIO_PCM_Capture2_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_Capture2;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_Capture2_Restart();
        HAL_AUDIO_PCM_Capture2_Stop();
    }

    return s32Ret;
}
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Restart(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    //MS_U32 u32WritePtrOffset = 0;
    //MS_U32 u32TimeoutCounter = 0;
    MS_S32 s32Ret = 0;

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    /* clear PCM buffer */
    memset((void *)pPCM->tPcmBufferInfo.pBuffer, 0x00, HW_DMA_WTR1_BUF_SIZE);

    /* flush MIU */
    MsOS_FlushMemory();
    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Open(void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    AUDIO_PCM_INFO_t *pUserPcmInfo = NULL;
    MS_U32 u32TargetBufferAddrPa = 0;
    MS_U32 u32TargetBufferSize = 0;
    MS_U32 u32TargetChannel = 0;
    MS_U32 u32TargetBitWidth = 0;
    MS_U32 u32TargetAlignmentSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Open %s\n", __FUNCTION__, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;

    if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
    {
        HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u32StructSize != sizeof(AUDIO_PCM_INFO_t))
    {
        HALAUDIO_ERROR("%s: Error! The size of PCM Info mismatch(%u, %u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32StructSize, (int)sizeof(AUDIO_PCM_INFO_t));
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MultiChFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8MixingFlag == TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (pUserPcmInfo->u8CaptureFlag != TRUE)
    {
        HALAUDIO_ERROR("%s: Error! Not support playback mode!\n", __FUNCTION__);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32Channel, (MS_U32 *)&pPCM->tPcmCaps.u32Channel, (sizeof(pPCM->tPcmCaps.u32Channel) / sizeof(pPCM->tPcmCaps.u32Channel[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support channel(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32Channel);
        return -A_EINVAL;
    }

    if (HAL_AUDIO_PCM_Check_U32_Param_Valid(pUserPcmInfo->u32SampleRate, (MS_U32 *)&pPCM->tPcmCaps.u32SampleRate, (sizeof(pPCM->tPcmCaps.u32SampleRate) / sizeof(pPCM->tPcmCaps.u32SampleRate[0]))) < 0)
    {
        HALAUDIO_ERROR("%s: Error! Not support sample rate(%u)!\n", __FUNCTION__, (unsigned int)pUserPcmInfo->u32SampleRate);
        return -A_EINVAL;
    }

    /* fill in settings */
    memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    pPcmInfo->u32StructVersion = AUDIO_PCM_INFO_VERSION;
    pPcmInfo->u32StructSize = sizeof(AUDIO_PCM_INFO_t);
    pPcmInfo->u8ConnectFlag = TRUE;
    strncpy((char *)pPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);
    strncpy((char *)pUserPcmInfo->u8Name, (char *)pPCM->u8Name, AUDIO_COMMON_PCM_IO_NAME_LENGTH);

    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_NONBLOCKING, &pUserPcmInfo->u8NonBlockingFlag);
    pPcmInfo->u8MultiChFlag = pUserPcmInfo->u8MultiChFlag;
    pPcmInfo->u8MixingFlag = pUserPcmInfo->u8MixingFlag;
    pPcmInfo->u32BufferDuration = ((HW_DMA_WTR1_BUF_SIZE * 1000) / (pUserPcmInfo->u32Channel * 2)) / pUserPcmInfo->u32SampleRate;
    pPcmInfo->u32Channel = pUserPcmInfo->u32Channel;
    pPcmInfo->u32SampleRate = pUserPcmInfo->u32SampleRate;;
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_BITWIDTH, &pUserPcmInfo->u32BitWidth);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_BIGENDIAN, &pUserPcmInfo->u32BigEndian);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_WEIGHTING, &pUserPcmInfo->u32Weighting);
    HAL_AUDIO_PCM_HwDma_Writer1_Set(AUDIO_PCM_CMD_VOLUME, &pUserPcmInfo->u32Volume);

    /* init PCM buffer address */
    u32TargetBufferAddrPa = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + HW_DMA_WTR1_BUF_ADDR;
    pPCM->tPcmBufferInfo.pBuffer = (MS_S8 *)MS_PA2KSEG1(u32TargetBufferAddrPa);
    pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
    pPCM->tPcmBufferInfo.pWritePtr = pPCM->tPcmBufferInfo.pBuffer;

    /* set PCM buffer address */
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_ctrl, 0x0);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Lo, (u32TargetBufferAddrPa/BYTES_IN_MIU_LINE) & 0xFFFF);
    HAL_AUDIO_AbsWriteReg(NewDMAwriter_BaseAddress_Hi, (u32TargetBufferAddrPa/BYTES_IN_MIU_LINE>>16) & 0xFFFF);

    HAL_AUDIO_WriteMaskReg(M2S_MBOX_HW_DMA_WRITER1_CTRL, 0x80, 0x80);
    HAL_AUDIO_WriteMaskReg(M2S_MBOX_HW_DMA_WRITER1_CTRL, 0x0F, M2S_MBOX_GET_Raw);

    /* calculate buffer size */
    u32TargetChannel = 2;
    u32TargetBitWidth = 16;
    u32TargetAlignmentSize = u32TargetChannel * (u32TargetBitWidth / 8) * BYTES_IN_MIU_LINE;
    u32TargetBufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)) / 1000;
    AUDIO_DO_ALIGNMENT(u32TargetBufferSize, u32TargetAlignmentSize);
    if (u32TargetBufferSize > HW_DMA_WTR1_BUF_SIZE)
    {
        HALAUDIO_ERROR("%s: Error! Too large buffer size(%u) is required! Please check the settings: duration %u, sample rate %u\n", __FUNCTION__, (unsigned int)u32TargetBufferSize, (unsigned int)pPcmInfo->u32BufferDuration, (unsigned int)pPcmInfo->u32SampleRate);
        u32TargetBufferSize = HW_DMA_WTR1_BUF_SIZE;
        pPcmInfo->u32BufferDuration = ((u32TargetBufferSize * 1000) / (pPcmInfo->u32SampleRate * u32TargetChannel * (u32TargetBitWidth / 8)));
        HALAUDIO_ERROR("%s: Error! Force duration to %u, which is the maximun value\n", __FUNCTION__, (unsigned int)pPcmInfo->u32BufferDuration);
    }

    /* set buffer size */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_DRAM_size, 0xFFFF, u32TargetBufferSize / BYTES_IN_MIU_LINE);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_DRAM_underrun_threshold, 0xFFFF, 0x00012);                         //setting : DMA Reader Underrun Thr

    /* Reset and Start DMA Writer */
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x800B); //SW reset & enable DMA reader, MIU request
    AUDIO_DELAY1US(1);
    HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0xFFFF, 0x000B); //SW reset & enable DMA reader, MIU request

    /* restart */
    HAL_AUDIO_PCM_HwDma_Writer1_Restart();

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Close(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Close %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8ConnectFlag != FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Writer1_Flush();
        memset((void *)pPcmInfo, 0x00, sizeof(AUDIO_PCM_INFO_t));
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Start(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    //MS_U32 u32BufferSize = 0;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Start %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0001, 0x0001);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0002, 0x0002);

        pPcmInfo->u8StartFlag = TRUE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Stop(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Stop %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag != FALSE)
    {
        /* TODO, there is no such control in DSP */

        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0001, 0x0000);
        HAL_AUDIO_AbsWriteMaskReg(NewDMAwriter_ctrl, 0x0002, 0x0000);

        pPcmInfo->u8StartFlag = FALSE;
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Set(MS_U32 u32Cmd, const void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Set command %u to %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            MS_U8 u8NonBlockingFlag = *((MS_U8 *)pData);

            if ((u8NonBlockingFlag != TRUE) && (u8NonBlockingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8NonBlockingFlag(%d)!\n", __FUNCTION__, u8NonBlockingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8NonBlockingFlag = u8NonBlockingFlag;

            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            MS_U8 u8MultiChFlag = *((MS_U8 *)pData);

            if ((u8MultiChFlag != TRUE) && (u8MultiChFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MultiChFlag(%d)!\n", __FUNCTION__, u8MultiChFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MultiChFlag == TRUE) && (pPCM->tPcmCaps.u8MultiChFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support Multi-Channel!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MultiChFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            MS_U8 u8MixingFlag = *((MS_U8 *)pData);

            if ((u8MixingFlag != TRUE) && (u8MixingFlag != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u8MixingFlag(%d)!\n", __FUNCTION__, u8MixingFlag);
                s32Ret = -A_EINVAL;
                break;
            }

            if ((u8MixingFlag == TRUE) && (pPCM->tPcmCaps.u8MixingFlag != TRUE))
            {
                HALAUDIO_ERROR("%s: Error! Not support mixing mode!\n", __FUNCTION__);
                s32Ret = -A_EINVAL;
                break;
            }

            pPcmInfo->u8MixingFlag = FALSE;

            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            MS_U32 u32MixingGroup = *((MS_U32 *)pData);

            pPcmInfo->u32MixingGroup = u32MixingGroup;

            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            MS_U32 u32BufferDuration = *((MS_U32 *)pData);

            if (u32BufferDuration == 0)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BufferDuration(%u)!\n", __FUNCTION__, (unsigned int)u32BufferDuration);
                s32Ret = -A_EINVAL;
                break;
            }

            if (pPcmInfo->u32BufferDuration != u32BufferDuration)
            {
                pPcmInfo->u32BufferDuration = u32BufferDuration;
                pPCM->u8SettingChangeFlag = TRUE;
            }

            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure channel!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            HALAUDIO_ERROR("%s: Warning! Not support to configure sample rate!\n", __FUNCTION__);
            s32Ret = -A_EPERM;

            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            MS_U32 u32BitWidth = *((MS_U32 *)pData);

            pPcmInfo->u32BitWidth = u32BitWidth;

            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            MS_U32 u32BigEndian = *((MS_U32 *)pData);

            if ((u32BigEndian != TRUE) && (u32BigEndian != FALSE))
            {
                HALAUDIO_ERROR("%s: Error! invalid u32BigEndian(%u)!\n", __FUNCTION__, (unsigned int)u32BigEndian);
                return -A_EINVAL;
            }

            pPcmInfo->u32BigEndian = u32BigEndian;

            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            MS_U32 u32Timestamp = *((MS_U32 *)pData);

            pPcmInfo->u32Timestamp = u32Timestamp;

            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            MS_U32 u32Weighting = *((MS_U32 *)pData);

            if (u32Weighting > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Weighting(%u)!\n", __FUNCTION__, u32Weighting);
                return -A_EINVAL;
            }

            pPcmInfo->u32Weighting = u32Weighting;

            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            MS_U32 u32Volume = *((MS_U32 *)pData);

            if (u32Volume > 100)
            {
                HALAUDIO_ERROR("%s: Error! invalid u32Volume(%u)!\n", __FUNCTION__, u32Volume);
                return -A_EINVAL;
            }

            pPcmInfo->u32Volume = u32Volume | (pPcmInfo->u32Volume & 0x80000000);

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            MS_U32 u32Mute = *((MS_U32*)pData);

            if (u32Mute == TRUE)
            {
                pPcmInfo->u32Volume |= 0x80000000;
            }
            else if (u32Mute == FALSE)
            {
                pPcmInfo->u32Volume &= 0x7FFFFFFF;
            }
            else
            {
                HALAUDIO_ERROR("%s: Error! invalid Invalid mute command(%u)!\n", __FUNCTION__, (unsigned int)u32Mute);
                s32Ret = -A_EINVAL;
            }
            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Get(MS_U32 u32Cmd, void *pData)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    //HALAUDIO_ERROR("%s: Get command %u from %s\n", __FUNCTION__, u32Cmd, pPCM->u8Name);

    if (pData == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pData should not be NULL!\n", __FUNCTION__);
        return -A_EFAULT;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return -A_EPERM;
    }

    switch(u32Cmd)
    {
        case AUDIO_PCM_CMD_ALL:
        {
            AUDIO_PCM_INFO_t *pUserPcmInfo = (AUDIO_PCM_INFO_t *)pData;
            MS_U32 u32MinSize = 0;

            if ((pUserPcmInfo->u32StructVersion & AUDIO_MAJOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MAJOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Error! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
                s32Ret = -A_EINVAL;
                break;
            }
            else if ((pUserPcmInfo->u32StructVersion & AUDIO_MINOR_VERSION_MASK) != (AUDIO_PCM_INFO_VERSION & AUDIO_MINOR_VERSION_MASK))
            {
                HALAUDIO_ERROR("%s: Warning! The version of PCM Info mismatch(0x%08X, 0x%08X)!\n", __FUNCTION__, pUserPcmInfo->u32StructVersion, AUDIO_PCM_INFO_VERSION);
            }

            u32MinSize = sizeof(AUDIO_PCM_INFO_t);
            u32MinSize = (pUserPcmInfo->u32StructSize >= u32MinSize) ? u32MinSize : pUserPcmInfo->u32StructSize;
            memcpy((void *)pUserPcmInfo, (void *)pPcmInfo, u32MinSize);

            break;
        }

        case AUDIO_PCM_CMD_NONBLOCKING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8NonBlockingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MULTICH:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MultiChFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXING:
        {
            *((MS_U8 *)pData) = pPcmInfo->u8MixingFlag;
            break;
        }

        case AUDIO_PCM_CMD_MIXINGGROUP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32MixingGroup;
            break;
        }

        case AUDIO_PCM_CMD_BUFFER:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pBuffer;
             */
            break;
        }

        case AUDIO_PCM_CMD_BUFFERDURATION:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BufferDuration;
            break;
        }

        case AUDIO_PCM_CMD_READPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pReadPtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_WRITEPTR:
        {
            /*
             * TODO, need better coding
             *
             * *((MS_S8 *)pData) = pPCM->tPcmBufferInfo.pWritePtr;
             */
            break;
        }

        case AUDIO_PCM_CMD_CHANNEL:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Channel;
            break;
        }

        case AUDIO_PCM_CMD_SAMPLERATE:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32SampleRate;
            break;
        }

        case AUDIO_PCM_CMD_BITWIDTH:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BitWidth;
            break;
        }

        case AUDIO_PCM_CMD_BIGENDIAN:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32BigEndian;
            break;
        }

        case AUDIO_PCM_CMD_TIMESTAMP:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Timestamp;
            break;
        }

        case AUDIO_PCM_CMD_WEIGHTING:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Weighting;
            break;
        }

        case AUDIO_PCM_CMD_VOLUME:
        {
            *((MS_U32 *)pData) = pPcmInfo->u32Volume & (~0x80000000);
            break;
        }

        case AUDIO_PCM_CMD_BUFFERLEVEL:
        {
            MS_U32 u32BufferLevel1 = 0;
            MS_U32 u32BufferLevel2 = 0;
            MS_U32 u32Timeout = 20;

            while (u32Timeout > 0)
            {
                u32BufferLevel1 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt) * BYTES_IN_MIU_LINE;
                u32BufferLevel2 = HAL_AUDIO_AbsReadReg(NewDMAwriter_DRAM_levelcnt) * BYTES_IN_MIU_LINE;

                if (u32BufferLevel1 == u32BufferLevel2)
                {
                    break;
                }

                u32Timeout--;

#ifdef MSOS_TYPE_LINUX_KERNEL
                udelay(1);
#else
                AUDIO_DELAY1US(1);
#endif
            }

            if (u32BufferLevel1 != u32BufferLevel2)
            {
                HALAUDIO_ERROR("%s: Error! fail to get correct buffer level (%u, %u)\n", __FUNCTION__, u32BufferLevel1, u32BufferLevel2);
            }

            pPcmInfo->u32BufferLevel = u32BufferLevel2;
            *((MS_U32 *)pData) = pPcmInfo->u32BufferLevel;

            break;
        }

        case AUDIO_PCM_CMD_MUTE:
        {
            *((MS_U32 *)pData) = ((pPcmInfo->u32Volume & 0x80000000) > 0) ? TRUE : FALSE;
            break;
        }

        case AUDIO_PCM_CMD_BUFFERSIZE:
        {
            *((MS_U32 *)pData) = HW_DMA_WTR1_BUF_SIZE;

            break;
        }

        default:
        {
            HALAUDIO_ERROR("%s: Error! unsupported command type(%u)!\n", __FUNCTION__, (unsigned int)u32Cmd);
            break;
        }
    }

    return s32Ret;
}

MS_U32 HAL_AUDIO_PCM_HwDma_Writer1_Read(void *pBuf, MS_U32 u32Size)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S8 *pBufTmp = NULL;
    MS_U32 u32BufferSize = 0;
    MS_U32 u32PcmLevel = 0;
    MS_U32 u32RequestSize = 0;
    MS_U32 u32RequestSizeTmp = 0;
    MS_U32 u32SizeToCopy = 0;
    //HALAUDIO_ERROR("%s: Write %u bytes to %s\n", __FUNCTION__, u32Size, pPCM->u8Name);

    if (pBuf == NULL)
    {
        HALAUDIO_ERROR("%s: Error! pBuf should not be NULL!\n", __FUNCTION__);
        return 0;
    }

    if (u32Size == 0)
    {
        HALAUDIO_ERROR("%s: Error! u32Size(%u) is invalid!\n", __FUNCTION__, (unsigned int)u32Size);
        return 0;
    }

    if (pPcmInfo->u8ConnectFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not opened yet!\n", __FUNCTION__);
        return 0;
    }

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HALAUDIO_ERROR("%s: Error! Not started yet!\n", __FUNCTION__);
        return 0;
    }

    pBufTmp = (MS_S8 *)pBuf;

    u32BufferSize = (pPcmInfo->u32BufferDuration * pPcmInfo->u32SampleRate * pPcmInfo->u32Channel * 2) / 1000;
    HAL_AUDIO_PCM_HwDma_Writer1_Get(AUDIO_PCM_CMD_BUFFERLEVEL, &u32PcmLevel);

    u32RequestSize = u32Size;
    u32RequestSizeTmp = u32RequestSize;

    /* copy data from PCM buffer */
    if (u32PcmLevel >= u32RequestSize)
    {
        do {
            u32SizeToCopy = (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize) - pPCM->tPcmBufferInfo.pReadPtr;
            u32SizeToCopy = (u32SizeToCopy > u32RequestSizeTmp) ? u32RequestSizeTmp : u32SizeToCopy;

            memcpy((void *)pBufTmp, (void *)pPCM->tPcmBufferInfo.pReadPtr, u32SizeToCopy);

            pBufTmp += u32SizeToCopy;
            pPCM->tPcmBufferInfo.pReadPtr += u32SizeToCopy;
            if (pPCM->tPcmBufferInfo.pReadPtr >= (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
            {
                if (pPCM->tPcmBufferInfo.pReadPtr > (pPCM->tPcmBufferInfo.pBuffer + u32BufferSize))
                {
                    HALAUDIO_ERROR("%s: Warning! it's an abnormal copy!\n", __FUNCTION__);
                }
                pPCM->tPcmBufferInfo.pReadPtr = pPCM->tPcmBufferInfo.pBuffer;
            }

            u32RequestSizeTmp -= u32SizeToCopy;
        } while (u32RequestSizeTmp > 0);

        HAL_AUR2_WriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0010);
        HAL_AUR2_WriteMaskReg(NewDMAwriter_CPU_triggersize, 0xFFFF, u32RequestSize/BYTES_IN_MIU_LINE);
        HAL_AUR2_WriteMaskReg(NewDMAwriter_ctrl, 0x0010, 0x0000);

        return u32Size;
    }
    return 0;
}

MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Flush(void)
{
    AUDIO_PCM_t *pPCM = &Audio_Pcm_HwDma_Writer1;
    AUDIO_PCM_INFO_t *pPcmInfo = pPCM->pPcmInfo;
    MS_S32 s32Ret = 0;
    HALAUDIO_ERROR("%s: Flush %s\n", __FUNCTION__, pPCM->u8Name);

    if (pPcmInfo->u8StartFlag == FALSE)
    {
        HAL_AUDIO_PCM_HwDma_Writer1_Restart();
        HAL_AUDIO_PCM_HwDma_Writer1_Stop();
    }

    return s32Ret;
}
#endif

AUDIO_DSP_CODE_TYPE HAL_AUDIO_Convert_EnDVBdecSystemType_to_AudioDspCodeType(En_DVB_decSystemType enDecSystem)
{
    AUDIO_DSP_CODE_TYPE dspCodeType = AU_DVB_STANDARD_INVALID;

    switch (enDecSystem)
    {
        case MSAPI_AUD_ATV_BTSC:
            dspCodeType = AU_SIF_BTSC;
            break;
        case MSAPI_AUD_ATV_PAL:
            dspCodeType = AU_SIF_PALSUM;
            break;
        case MSAPI_AUD_DVB_MPEG:
            dspCodeType = AU_DVB_STANDARD_MPEG;
            break;
        case MSAPI_AUD_DVB_AC3:
            dspCodeType = AU_DVB_STANDARD_AC3;
            break;
        case MSAPI_AUD_DVB_AC3P:
            dspCodeType = AU_DVB_STANDARD_AC3P;
            break;
        case MSAPI_AUD_DVB_AAC:
            dspCodeType = AU_DVB_STANDARD_AAC;
            break;
        case MSAPI_AUD_DVB_MP3:
            dspCodeType = AU_DVB_STANDARD_MP3;
            break;
        case MSAPI_AUD_DVB_WMA:
            dspCodeType = AU_DVB_STANDARD_WMA;
            break;
        case MSAPI_AUD_DVB_RA8LBR:
            dspCodeType = AU_DVB_STANDARD_RA8LBR;
            break;
        case MSAPI_AUD_DVB_XPCM:
            dspCodeType = AU_DVB_STANDARD_XPCM;
            break;
        case MSAPI_AUD_DVB_DTS:
            dspCodeType = AU_DVB_STANDARD_DTS;
            break;
        case MSAPI_AUD_DVB_MS10_DDT:
            dspCodeType = AU_DVB_STANDARD_MS10_DDT;
            break;
        case MSAPI_AUD_DVB_MS10_DDC:
            dspCodeType = AU_DVB_STANDARD_MS10_DDC;
            break;
        case MSAPI_AUD_DVB_WMA_PRO:
            dspCodeType = AU_DVB_STANDARD_WMA_PRO;
            break;
        case MSAPI_AUD_DVB_FLAC:
            dspCodeType = AU_DVB_STANDARD_FLAC;
            break;
        case MSAPI_AUD_DVB_VORBIS:
            dspCodeType = AU_DVB_STANDARD_VORBIS;
            break;
        case MSAPI_AUD_DVB_DTSLBR:
            dspCodeType = AU_DVB_STANDARD_DTSLBR;
            break;
        case MSAPI_AUD_DVB_DRA:
            dspCodeType = AU_DVB_STANDARD_DRA;
            break;
        default:
            break;
    }

    return dspCodeType;
}