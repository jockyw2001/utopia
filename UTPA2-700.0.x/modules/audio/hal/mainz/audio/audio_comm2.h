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
#ifndef _AUDIO_COMM2_H_
#define _AUDIO_COMM2_H_
#include "audio_mbox2.h"

#include "ddr_config.h"

/************************************************
*  請保持 Utopia 和 DSP 裡的這個檔案一致
*
*  1. 註解不要用 !
*  2. 宣告不要用 .const xxxx = ????;
************************************************/

/*********************************************************
*   Version Control
*********************************************************/
#define  system_version_num              0x000DE2
#define  palsum_version_num              0x544007
#define  btsc_version_num                0x046C06
#define  dbx_version_num                 0x1A01BC
#define  abs3d_version_num               0x100005

#define  AUDIO_DSP2_VERSION    (system_version_num +  palsum_version_num + btsc_version_num)


/*********************************************************
*   system define
*********************************************************/
    #define DSP2_DEC_AVSYNC_FIX_SYNTHRATE            0               //always fix synthesizer
    #define DSP2_SYS_DEC1_MUTIL_CH_OUTPUT            0               //enable multi-channel output
    #define DSP2_SYS_DEC_OUTPUT_24BIT                0
    #define DSP2_SYS_DEC_UNI_PLAYER                  1
    #define DSP2_DDP_HDMI_BYPASS_EN                  0
    #define DSP2_SYS_DEC_AD_VOLUME_FOR_MAIN          0
    #define DSP2_SYS_DEC_2ND_DECODER                 0               //enable this, also need enable "DSP2_SYS_DEC_DDP_DUAL_DECODE"
    #define DSP2_SYS_DEC_RUN_WITH_SIF                1
    #define DSP2_HW_ES2_BUG                          0
    #define DSP2_SYS_DEC_DDP_DUAL_DECODE             1
    #define DDT_DDENC_PATH                           3                // For MS10_DDT_Dual, DDEnc is in : DSP1(1), DSP2(2), R2M(3), Invalid (0xFF)
    #define DSP2_SYS_DEC_SEC_PVR_MODE                0                // For SEC PVR mode
    #define DSP2_SYS_DEC_33BIT_IN_MMTS_FILE          0                // For SEC PVR mode, also need to enable this function
    #define DSP2_SYS_DEC2_USE_DEC1_SYNTH             0

    /* ASND System Channels */
        #define SE_PROCESS_FETCH_CHANNELS           8               // SE_Buffer input channels
        #define SE_PROCESS_STORE_CHANNELS           8               // SE_Buffer output channels
        #define SPDIF_DELAY_STORE_CHANNELS          2               // SPDIF Buffer channels
        #define DELAY_FUNCTION_STORE_CHANNELS       2               // Delay function Buffer channels

    /* DSP Audio Delay Setting */
        #define AUDIO_DELAY_FS                      48              // fs = 48kHz
        #define SPDIF_DELAY_FS                      48
        #define DMA24BIT_BYTES_IN_WORDS             3
        #define DMA16BIT_BYTES_IN_WORDS             2

        #define AUDIO_DELAY_LOWER_BOUND             0x20            // min main audio delay , 0x20 = 32 ms
        #define SPDIF_DELAY_LOWER_BOUND             0x05            // min spdif audio delay, 0x05 =  5 ms
        #define KTV_DELAY_LOWER_BOUND               0x14            // min KTV audio delay  , 0x14 = 20 ms
        #define AUDIO_DELAY_UPPER_BOUND             ((SE_MAIN_IN_DRAM_SIZE/SE_PROCESS_FETCH_CHANNELS + SE_MAIN_OUT_DRAM_SIZE/SE_PROCESS_STORE_CHANNELS)/DMA24BIT_BYTES_IN_WORDS/AUDIO_DELAY_FS)
        #define SPDIF_DELAY_UPPER_BOUND             ((SPDIF_DLY_DRAM_SIZE/SPDIF_DELAY_STORE_CHANNELS)/DMA16BIT_BYTES_IN_WORDS/SPDIF_DELAY_FS)

    /* Audio Ease */
        #define AUDIO_EASE_TYPE_LINEAR              0
        #define AUDIO_EASE_TYPE_INCUBIC             1
        #define AUDIO_EASE_TYPE_OUTCUBIC            2
        #define AUDIO_EASE_TYPE_INOUTCUBIC          3

        #define AUDIO_EASE_CH_NONE                  0
        #define AUDIO_EASE_CH_A                     1
        #define AUDIO_EASE_CH_B                     2

        #define CH5_INPUT_DLY_UPPER_BOUND           ((CH5_INPUT_DLY_DRAM_SIZE/DELAY_FUNCTION_STORE_CHANNELS)/DMA24BIT_BYTES_IN_WORDS/AUDIO_DELAY_FS)

/************************************************
*   DSP ddr address mapping
*************************************************/
    /* DSP DRAM Config */
        #define DSP_TO_COMMON_DRAM_OFFSET               (DSP_DDR_SIZE / BYTES_IN_MIU_LINE)

    /* SPDIF delay (GP C Bffer) */
        #define DSP2_SPDIF_DLY_DRAM_BASE                (OFFSET_SPDIF_DLY_DRAM_BASE / BYTES_IN_MIU_LINE)
        #define DSP2_SPDIF_DLY_DRAM_SIZE                ((SPDIF_DLY_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)

    /* Surround */
        #define SUR_DRAM_BASEADDR                       (OFFSET_MSTAR_SURROUND_DRAM_ADDR / BYTES_IN_MIU_LINE)                               // Line Address
        #define SUR_DRAM_ENDADDR                        ((OFFSET_MSTAR_SURROUND_DRAM_ADDR + MSTAR_SURROUND_DRAM_SIZE) / BYTES_IN_MIU_LINE)  // Line Address

    /* KTV */
        #define SUR_DRAM_KTV_BASEADDR                   (OFFSET_KTV_SURROUND_DRAM_ADDR / BYTES_IN_MIU_LINE)                             // Line address, Only in KTV mode, MS surround -> echo
        #define SUR_DARM_KTV_ENDADDR                    ((OFFSET_KTV_SURROUND_DRAM_ADDR + KTV_SURROUND_DRAM_SIZE) / BYTES_IN_MIU_LINE)  // Line address, Overlay with DM prefetch

    /* ES1 */
        #define DSP2_ES1_DRAM_BASE                      (OFFSET_ES1_DRAM_ADDR / BYTES_IN_MIU_LINE)
        #define DSP2_ES1_DRAM_SIZE                      ((ES1_DRAM_SIZE_BYTE / BYTES_IN_MIU_LINE) - 1)        // 128KB
        #define DSP2_UNI_ES1_DRAM_BASE_EXT_MASK         DSP2_ES1_DRAM_BASE&0xFF0000

    /* DSP DM Prefetch */
        #define DSP2_DM_PREFETCH_DRAM_BASE              (OFFSET_DM_PREFETCH_DRAM_ADDR / BYTES_IN_MIU_LINE)
        #define DSP2_DM_PREFETCH_DRAM_SIZE              (DM_PREFETCH_DRAM_SIZE_BYTE / BYTES_IN_MIU_LINE)

    /* sound system */
        #define DSP2_DMA_START_DRAM_BASE1               (OFFSET_SE_MAIN_IN_DRAM_ADDR / BYTES_IN_MIU_LINE)
        #define DSP2_DMA_START_DRAM_SIZE1               (SE_MAIN_IN_DRAM_SIZE / BYTES_IN_MIU_LINE)
        #define DSP2_DMA_START_DRAM_BASE2               (OFFSET_SE_MAIN_OUT_DRAM_ADDR / BYTES_IN_MIU_LINE)
        #define DSP2_DMA_START_DRAM_SIZE2               (SE_MAIN_OUT_DRAM_SIZE / BYTES_IN_MIU_LINE)

    /* DDCO within GAAC single / DDT single */
        #define DSP2_GAAC_DDE_6CH_PCM_BASE              0         // Dummy
        #define DSP2_GAAC_DDE_6CH_PCM_SIZE              0          // Dummy

        #define DSP2_GAAC_DDE_PREV_INFO_BASE            0        // Dummy
        #define DSP2_GAAC_DDE_PREV_INFO_SIZE            0         // Dummy

    /* CH5 input delay */
        #define DSP2_CH5_INPUT_DLY_DRAM_BASE            (OFFSET_CH5_INPUT_DLY_DRAM_BASE / BYTES_IN_MIU_LINE)    // Line address
        #define DSP2_CH5_INPUT_DLY_DRAM_SIZE            ((CH5_INPUT_DLY_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)     // 32KB

    /* DM Prefetch2 */
        #define DSP2_DM_PREFETCH2_DRAM_BASE             (OFFSET_DM_PREFETCH2_DRAM_ADDR / BYTES_IN_MIU_LINE)
        #define DSP2_DM_PREFETCH2_DRAM_SIZE             ((DM_PREFETCH2_DRAM_SIZE_BYTE / BYTES_IN_MIU_LINE) - 1)

    /* COMMON DRAM */

        /* DMA Reader Buffer */
            #define DSP2_DMA_READER_DRAM_BASE           ((OFFSET_DMA_READER_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)        // DMA Reader base address
            #define DSP2_DMA_READER_DRAM_SIZE           ((DMA_READER_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)

        /* Software DMA */
            #define DSP2_SW_DMA_READER_DRAM_BASE        ((OFFSET_SW_DMA_READER_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)
            #define DSP2_SW_DMA_READER_DRAM_SIZE        ((SW_DMA_READER_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)

        /* PCM capture buffer */
            #define DSP2_PCM_CAPTURE_BUFFER_DRAM_BASE   ((OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)
            #define DSP2_PCM_CAPTURE2_BUFFER_DRAM_BASE  ((OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)
            #define DSP2_PCM_CAPTURE_BUFFER_DRAM_SIZE   (PCM_CAPTURE_BUFFER_DRAM_SIZE/ BYTES_IN_MIU_LINE)           // 48KB
            #define DSP2_PCM_CAPTURE_COPY_WORDSIZE      32
            #define DSP2_PCM_CAPTURE_COPY_LINESIZE      4

        /* PCM1 Multi Channel */
            #define DSP2_MULTI_CHANNEL_PCM_BASE         0          // Dummy
            #define DSP2_MULTI_CHANNEL_PCM_SIZE         0          // Dummy

        /* SPDIF Non-PCM in Common DDR */
            #define DSP2_SPDIF_DRAM_BASE                ((OFFSET_SPDIF_NONPCM_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)
            #define DSP2_SPDIF_DRAM_SIZE                ((SPDIF_NONPCM_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)         // 312KB

        /* PCM1 */
            #define DSP2_PCM1_DRAM_BASE                 ((OFFSET_PCM1_DRAM_ADDR / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)
            #define DSP2_PCM1_DRAM_SIZE                 ((PCM1_DRAM_SIZE_BYTE / BYTES_IN_MIU_LINE) - 1)          // 324KB
            #define DSP2_DEC1_PCM1_DRAM_BASE_EXT_MASK   (DSP2_PCM1_DRAM_BASE&0xFF0000)

        /* Uni-Player interface */
            #define DSP2_UNI_DEC1_PCM3_DRAM_BASE        DSP2_PCM1_DRAM_BASE
            #define DSP2_UNI_DEC1_PCM3_DRAM_SIZE        DSP2_PCM1_DRAM_SIZE
            #define DSP2_UNI_DEC1_PCM3_DRAM_BASE_EXT_MASK   (DSP2_UNI_DEC1_PCM3_DRAM_BASE&0xFF0000)

          /* DMA Reader2 Buffer */
            #define DSP2_HW_DMA_READER2_DRAM_BASE       ((OFFSET_HW_DMA_READER2_DRAM_BASE / BYTES_IN_MIU_LINE) + DSP_TO_COMMON_DRAM_OFFSET)        // DMA Reader2 base address
            #define DSP2_HW_DMA_READER2_DRAM_SIZE       ((HW_DMA_READER2_DRAM_SIZE / BYTES_IN_MIU_LINE) - 1)

/************************************************
* DSP sram address mapping
************************************************/
    /* DSP SRAM Segment */
        /* CM */
        #define DSP2_CM_MAIN_ADDR           0x0
        #define DSP2_CM_MAIN_SIZE           0x08C0
        #define DSP2_CM_CODE1_ADDR          0x08C0
        #define DSP2_CM_CODE1_SIZE          0x0100
        #define DSP2_CM_CODE2_ADDR          0x09C0
        #define DSP2_CM_CODE2_SIZE          0x0B80

        /* PM */
        #define DSP2_PM_MAIN_ADDR           0x1540
        #define DSP2_PM_MAIN_SIZE           0x02BF
        #define DSP2_PM_SEG1_ADDR           0x17FF
        #define DSP2_PM_SEG1_SIZE           0x3800
        #define DSP2_PM_SEG2_ADDR           0x5000
        #define DSP2_PM_SEG2_SIZE           0x0000

        /* Prefetch */
        #define DSP2_PM_PREFETCH_DSPADDR    0x15800                // check "arch.sys"
        #define DSP2_PM_PREFETCH_DDRADDR    DSP2_PM_PREFETCH_DSPADDR*3/BYTES_IN_MIU_LINE
        #define DSP2_PM_PREFETCH2_DSPADDR   0x1E000                // check "arch.sys"
        #define DSP2_PM_PREFETCH2_DDRADDR   DSP2_PM_PREFETCH2_DSPADDR*3/BYTES_IN_MIU_LINE

        /* DM */
        #define DSP2_DM_MAIN_ADDR           0x3700
        #define DSP2_DM_MAIN_SIZE           0x0D00
        #define DSP2_DM_SEG1A_ADDR          0x0
        #define DSP2_DM_SEG1A_SIZE          0x2F00
        #define DSP2_DM_SEG1B_ADDR          0x2F00
        #define DSP2_DM_SEG1B_SIZE          0x0
        #define DSP2_DM_SEG2_ADDR           0x2F00
        #define DSP2_DM_SEG2_SIZE           0x800

    /* SND DSP PM vars */
        /* common */
            #define DSP2PmAddr_mainVer              (DSP2_PM_MAIN_ADDR)                         // system version
            #define DSP2PmAddr_alg1Ver              (DSP2_PM_SEG1_ADDR)                         // decode version
            #define DSP2PmAddr_alg2Ver              (DSP2_CM_CODE2_ADDR + DSP2_CM_CODE2_SIZE - 1)

            #define DSP2PmAddr_mainAudioDelay       (DSP2PmAddr_mainVer         +1) // DSP2PmAddr_mainVer + 0x01
            #define DSP2PmAddr_spdifDelay           (DSP2PmAddr_mainAudioDelay  +1) //                    + 0x02
            #define DSP2PmAddr_hdmiNonPcmSts        (DSP2PmAddr_spdifDelay      +1) //                    + 0x03
            #define DSP2PmAddr_btFrameSize          (DSP2PmAddr_hdmiNonPcmSts   +1) //                    + 0x04
            #define DSP2PmAddr_audio_PTS_H          (DSP2PmAddr_btFrameSize     +1) //                    + 0x05
            #define DSP2PmAddr_audio_PTS_L          (DSP2PmAddr_audio_PTS_H     +1) //                    + 0x06
            #define DSP2PmAddr_audio_Td_H           (DSP2PmAddr_audio_PTS_L     +1) //                    + 0x07
            #define DSP2PmAddr_audio_Td_L           (DSP2PmAddr_audio_Td_H      +1) //                    + 0x08
            #define DSP2PmAddr_smpRate              (DSP2PmAddr_audio_Td_L      +1) //                    + 0x09
            #define DSP2PmAddr_soundMode            (DSP2PmAddr_smpRate         +1) //                    + 0x0A
            #define DSP2PmAddr_reqMHEG5_size        (DSP2PmAddr_soundMode       +1) //                    + 0x0B
            #define DSP2PmAddr_decErrorCnt          (DSP2PmAddr_reqMHEG5_size   +1) //                    + 0x0C
            #define DSP2PmAddr_STest_Report         (DSP2PmAddr_decErrorCnt     +1) //                    + 0x0D
            #define DSP2PmAddr_video_Td_L           (DSP2PmAddr_STest_Report    +1) //                    + 0x0E
            #define DSP2PmAddr_ipSecurity           (DSP2PmAddr_video_Td_L      +1) //                    + 0x0F
            #define DSP2PmAddr_es1_blockLevel       (DSP2PmAddr_ipSecurity      +1) //                    + 0x10
            #define DSP2PmAddr_pcm_blockLevel       (DSP2PmAddr_es1_blockLevel  +1) //                    + 0x11
            #define DSP2PmAddr_mpg_avDelay          (DSP2PmAddr_pcm_blockLevel  +1) //                    + 0x12
            #define DSP2PmAddr_ac3_avDelay          (DSP2PmAddr_mpg_avDelay     +1) //                    + 0x13
            #define DSP2PmAddr_aac_avDelay          (DSP2PmAddr_ac3_avDelay     +1) //                    + 0x14
            #define DSP2PmAddr_ch5InputDelay        (DSP2PmAddr_aac_avDelay     +1) //                    + 0x15

            #define DSP2PmAddr_ext_Synth_H          0x17EC
            #define DSP2PmAddr_ext_Synth_L          0x17ED

        /* basic sound effect */
            #define PEQ_band_num                    8
            #define DSP2PmAddr_peq48KCoeffAddr      (DSP2PmAddr_mainVer      +0x20) // DSP2PmAddr_mainVer + 0x20 (0x20 is just for reserved, If need, this can be re-assigned.)
            #define DSP2PmAddr_peq32KCoeffAddr      (DSP2PmAddr_peq48KCoeffAddr        )
            #define DSP2PmAddr_peqscale48KAddr      (DSP2PmAddr_peq48KCoeffAddr     +40)
            #define DSP2PmAddr_peqbandEnAddr        (DSP2PmAddr_peqscale48KAddr     +8 )
            #define DSP2PmAddr_peqbandDoubleAddr    (DSP2PmAddr_peqbandEnAddr       +1 )
            #define DSP2PmAddr_hpf48KCoeffAddr      (DSP2PmAddr_peqbandDoubleAddr   +1 )
            #define DSP2PmAddr_toneSelectAddr       (DSP2PmAddr_hpf48KCoeffAddr     +5 )
            #define DSP2PmAddr_bass48KCoeffAddr     (DSP2PmAddr_toneSelectAddr      +1 )
            #define DSP2PmAddr_bassscale48KAddr     (DSP2PmAddr_bass48KCoeffAddr    +5 )
            #define DSP2PmAddr_treble48KCoeffAddr   (DSP2PmAddr_bassscale48KAddr    +1 )
            #define DSP2PmAddr_treblescale48KAddr   (DSP2PmAddr_treble48KCoeffAddr  +5 )
            #define DSP2PmAddr_SPK_OutChannel       (DSP2PmAddr_treblescale48KAddr  +1 )
            #define DSP2PmAddr_HP_OutChannel        (DSP2PmAddr_SPK_OutChannel      +1 )
            #define DSP2PmAddr_VolEaseAddr          (DSP2PmAddr_HP_OutChannel       +1 )
            #define DSP2PmAddr_VolFadingStep1       (DSP2PmAddr_VolEaseAddr         +9 )
            #define DSP2PmAddr_VolFadingStep2       (DSP2PmAddr_VolFadingStep1      +1 )

    /* SND DSP DM vars */
        /* system info memory base */
            #define DSP2DmAddr_sys_AvSync           DSP2_DM_MAIN_ADDR
            #define DSP2DmSize_sys_AvSync           23
                #define SYS_AVSYNC_STATE            0
                #define SYS_AVSYNC_FREERUN_TYPE     1
                #define SYS_AVSYNC_STC_L            2
                #define SYS_AVSYNC_STC_H            3
                #define SYS_AVSYNC_PTS_L            4
                #define SYS_AVSYNC_PTS_H            5
                #define SYS_AVSYNC_CURR_PTS_L       6
                #define SYS_AVSYNC_CURR_PTS_H       7
                #define SYS_AVSYNC_TD_L             8
                #define SYS_AVSYNC_TD_H             9
                #define SYS_AVSYNC_VIDEO_PTS_L      10
                #define SYS_AVSYNC_VIDEO_PTS_H      11
                #define SYS_AVSYNC_CURR_SYNTHRATE   12
                #define SYS_AVSYNC_WAIT_1STPTS_CNT  13
                #define SYS_AVSYNC_WAIT_STC_CNT     14
                #define SYS_AVSYNC_MMTS_SYNC_STC    15
                #define SYS_AVSYNC_SMP_FLUSH_CNT    16
                #define SYS_AVSYNC_PTS_TAG_RD_IDX   17
                #define SYS_AVSYNC_PTS_TAG_WR_IDX   18
                #define SYS_AVSYNC_PTS_TAG_CNT      19
                #define SYS_AVSYNC_ACCUM_ES_WORDCNT 20
                #define SYS_AVSYNC_AVSYNC_OFFSET    21
                #define SYS_AVSYNC_SPDIF_OFFSET     22

            #define DSP2DmAddr_sys_DecStatus        (DSP2DmAddr_sys_AvSync+DSP2DmSize_sys_AvSync)
            #define DSP2DmSize_sys_DecStatus        24
                #define SYS_DECSTATUS_ID            0
                #define SYS_DECSTATUS_PLAY_STATE    1
                #define SYS_DECSTATUS_DEC_TYPE      2
                #define SYS_DECSTATUS_OK_CNT        3
                #define SYS_DECSTATUS_ERR_CNT       4
                #define SYS_DECSTATUS_SKIP_CNT      5
                #define SYS_DECSTATUS_REPT_CNT      6
                #define SYS_DECSTATUS_OK_CNT2       7
                #define SYS_DECSTATUS_ERR_CNT2      8
                #define SYS_DECSTATUS_SKIP_CNT2     9
                #define SYS_DECSTATUS_REPT_CNT2     10
                #define SYS_DECSTATUS_PLAYSMPFLAG   11
                #define SYS_DECSTATUS_PLAYSMPFLAG2  12
                #define SYS_DECSTATUS_SYNC_MISS_CNT     13
                #define SYS_DECSTATUS_DEC_CALLING_CNT   14
                #define SYS_DECSTATUS_PCMISR_EMPTY_CNT  15
                #define SYS_DECSTATUS_PLAYSTS_FLAG      16
                #define SYS_DECSTATUS_NEED_DATA_INT     17
                #define SYS_DECSTATUS_ES_REQ_SIZE       18
                #define SYS_DECSTATUS_ELAPSED_PCM_CNT   19
                #define SYS_DECSTATUS_RECEIVE_STOP_CNT  20
                #define SYS_DECSTATUS_ES_DDR_RD_PTR     21
                #define SYS_DECSTATUS_ES_FIFO_CNT       22
                #define SYS_DECSTATUS_ES_TAG_FROM_MCU   23

            #define DSP2DmAddr_sys_IoInfo           (DSP2DmAddr_sys_DecStatus+DSP2DmSize_sys_DecStatus)
            #define DSP2DmSize_sys_IoInfo           4
                #define SYS_IOINFO_DEC1_OUT_L       0
                #define SYS_IOINFO_DEC1_OUT_R       1
                #define SYS_IOINFO_DEC2_OUT_L       2
                #define SYS_IOINFO_DEC2_OUT_R       3


            #define DSP2DmAddr_sys_Working          (DSP2DmAddr_sys_IoInfo+DSP2DmSize_sys_IoInfo)
                #define SYS_FILE_PTS1_H             0
                #define SYS_FILE_PTS1_L             1
                #define SYS_ACCUM_ES_CNT            5

        /* sys_info */
            #define DSP2DmAddr_IO_Info1             0x3F40
                #define IO_INFO1_DAC1_OUT           0x0000
                #define IO_INFO1_DAC2_OUT           0x0002
                #define IO_INFO1_DAC3_OUT           0x0004
                #define IO_INFO1_DAC4_OUT           0x0006
                #define IO_INFO1_IIS1_OUT           0x0008
                #define IO_INFO1_IIS2_OUT           0x000A
                #define IO_INFO1_IIS3_OUT           0x000C
                #define IO_INFO1_IIS4_OUT           0x000E
                #define IO_INFO1_SRC_IN             0x0010
                #define IO_INFO1_GPB_IN             0x0012
                #define IO_INFO1_SPDIF_OUT          0x0014

            #define DSP2DmAddr_IO_Info2             0x3F60
                #define IO_INFO2_MUL_CH1            0x0000
                #define IO_INFO2_MUL_CH2            0x0002
                #define IO_INFO2_MUL_CH3            0x0004
                #define IO_INFO2_MUL_CH4            0x0006
                #define IO_INFO2_RAW                0x0008
                #define IO_INFO2_RAW_DELAY          0x000A
                #define IO_INFO2_RAW_DELAY_SE       0x000C
                #define IO_INFO2_SCART              0x000E
                #define IO_INFO2_KTV                0x0010
                #define IO_INFO2_MUL_CH6            0x0012
                #define IO_INFO2_SPDIF_DATA         0x0014
                #define IO_INFO2_RESERVED4          0x0016
                #define IO_INFO2_SINTONE            0x0018
                #define IO_INFO2_MUL_CH8            0x001A
                #define IO_INFO2_GPA_FS             0x001C
                #define IO_INFO2_GPB_FS             0x001D
                #define IO_INFO2_GPC_FS             0x001E
                #define IO_INFO2_ALSA_MODE          0x001F
                    #define ALSA_OUTPUT_DUAL_MONO       0x000001

        /* common */
            #define DSP2DmAddr_dec1_param               0x3F80
            #define DSP2DmAddr_dec1_info                0x3FE0

            #define DSP2DmAddr_dec1_stc1_select         0x3F5E
            #define DSP2DmAddr_dec1_signal_energy       0x3FA5
            #define DSP2DmAddr_mips_crisis_flag         0x3FA6
            #define DSP2DmAddr_pcmUpload_cmd            0x3FA7
            #define DSP2DmAddr_dec1_omx_param           0x3FA8

            #define DSP2DmAddr_hdmi_unstable_protect    0x3FAA
            #define DSP2DmAddr_hdmi_unstable_threshold  0x3FAB
            #define DSP2DmAddr_hdmi_npcm_lock           0x3AF7

            #define DSP2DmAddr_spdif_nonpcm_fs          0x3FAC

            #define DSP2DmAddr_pcmCapture_volume        0x3FB0
            #define DSP2DmAddr_pcmCapture2_volume       0x3FB1

            #define DSP2DmAddr_dec1_unsupport           0x3FC0
            #define DSP2DmAddr_swDmaRdr_ctrlBase        0x3FC1
            #define DSP2DmAddr_advse_volume             0x43E0

        /* basic sound effect */
            #define DSP2DmAddr_AvcSOffsetAddr           0x3FB6
            #define DSP2DmAddr_KTV_XAGain               0x3FB7
            #define DSP2DmAddr_KTV_XBGain               0x3FB8
            #define DSP2DmAddr_SND_EFFECT_VOL1          0x3FB9
            #define DSP2DmAddr_SND_EFFECT_VOL2          0x3FBA

        /* MISC */
            #define DSP2DmAddr_SAMPLERATE               0x3FBB
            #define DSP2DmAddr_ptsLatency_L             0x3F59
            #define DSP2DmAddr_ptsLatency_H             0x3F5A

        /* temp */
            #define DSP2DmAddr_demo_mode_flag           0x3FAD
            #define DSP2DmAddr_sysDec1_pcm_dram_wrPtr_report               (DSP2DmAddr_demo_mode_flag+1)

        /* mpg */
            #define DSP2DmAddr_mpegInfo             DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_mpegFrmNum_internal  DSP2DmAddr_dec1_info+1
            #define DSP2DmAddr_mpegVer              DSP2DmAddr_dec1_info+2
            #define DSP2DmAddr_mpg_frmNum           DSP2DmAddr_dec1_info+3
            #define DSP2DmPcmHighLevel_pcmdata      DSP2DmAddr_dec1_info+4
            #define DSP2DmPcmHighLevel_Threshold    DSP2DmAddr_dec1_info+5
            #define DSP2DmPcmHighLevel_NumTimeOut   DSP2DmAddr_dec1_info+6
            #define DSP2DmPcmHighLevel_MuteNum      DSP2DmAddr_dec1_info+7

        /* ac3+ */
            #define DSP2DmAddr_ac3p_bsMod           DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_ac3p_frmSz           DSP2DmAddr_dec1_info+1
            #define DSP2DmAddr_ac3p_bsid            DSP2DmAddr_dec1_info+2
            #define DSP2DmAddr_ac3p_fscod           DSP2DmAddr_dec1_info+3
            #define DSP2DmAddr_ac3p_find_syncword   DSP2DmAddr_dec1_info+4

        /* dts */
            #define DSP2DmAddr_dts_cdMode           DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_dts_amode            DSP2DmAddr_dec1_info+1
            #define DSP2DmAddr_dts_BitRate          DSP2DmAddr_dec1_info+2
            #define DSP2DmAddr_dts_smpRate          DSP2DmAddr_dec1_info+3

        /* dtslbr */
            #define DSP2DmAddr_dtslbr_inChannels    DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_dtslbr_BitRate       DSP2DmAddr_dec1_info+2
            #define DSP2DmAddr_dtslbr_smpRate       DSP2DmAddr_dec1_info+3

        /* wma pro */
            #define DSP2DmAddr_wma_smpRate          DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_wma_BitRate          DSP2DmAddr_dec1_info+1

        /* flac */
            #define DSP2DmAddr_flac_dmaPcmTotalSz   DSP2DmAddr_dec1_info+0
            #define DSP2DmAddr_flac_dmaPcmRemainSz  DSP2DmAddr_dec1_info+1
            #define DSP2DmAddr_flac_dmaPcmSramAddr  DSP2DmAddr_dec1_info+2

        /* dra */
            #define DSP1DmAddr_dra_nSampleRateIndex  DSP2DmAddr_dec1_info+0
            #define DSP1DmAddr_dra_nNumNormalCh      DSP2DmAddr_dec1_info+1
            #define DSP1DmAddr_dra_nNumLfeCh         DSP2DmAddr_dec1_info+2
            #define DSP1DmAddr_dra_bAuxChCfg         DSP2DmAddr_dec1_info+3

        /* srs puresound */
            #define SRS_PURESOUND_AeqFir_NumOfTaps_addr             DSP2_CM_CODE2_ADDR+0x300+1
            #define SRS_PURESOUND_AeqIir_NumOfSections_addr         DSP2_CM_CODE2_ADDR+0x300+2
            #define SRS_PURESOUND_AeqIir1Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+3
            #define SRS_PURESOUND_AeqIir2Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+8
            #define SRS_PURESOUND_AeqIir3Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+13
            #define SRS_PURESOUND_AeqIir4Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+18
            #define SRS_PURESOUND_AeqIir5Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+23
            #define SRS_PURESOUND_AeqIir6Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+28
            #define SRS_PURESOUND_AeqIir7Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+33
            #define SRS_PURESOUND_AeqIir8Coefs_addr                 DSP2_CM_CODE2_ADDR+0x300+38
            #define SRS_PURESOUND_AeqIir1_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+43
            #define SRS_PURESOUND_AeqIir2_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+44
            #define SRS_PURESOUND_AeqIir3_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+45
            #define SRS_PURESOUND_AeqIir4_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+46
            #define SRS_PURESOUND_AeqIir5_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+47
            #define SRS_PURESOUND_AeqIir6_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+48
            #define SRS_PURESOUND_AeqIir7_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+49
            #define SRS_PURESOUND_AeqIir8_iwl_addr                  DSP2_CM_CODE2_ADDR+0x300+50
            #define SRS_PURESOUND_AeqIir_Gain_iwl_addr              DSP2_CM_CODE2_ADDR+0x300+51
            #define SRS_PURESOUND_AeqIir_Gain_addr                  DSP2_CM_CODE2_ADDR+0x300+52
            #define SRS_PURESOUND_AeqFir_iwl_addr                   DSP2_CM_CODE2_ADDR+0x300+53
            #define SRS_PURESOUND_AeqFir_Gain_iwl_addr              DSP2_CM_CODE2_ADDR+0x300+54
            #define SRS_PURESOUND_AeqFir_Gain_addr                  DSP2_CM_CODE2_ADDR+0x300+55
            #define SRS_PURESOUND_AeqFirCoefs_addr                  DSP2_CM_CODE2_ADDR+0x300+64

            #define SRS_PURESOUND_ftbLowPassFilterCoef_Audio_addr           DSP2_CM_CODE2_ADDR+0x300+155
            #define SRS_PURESOUND_ftbLowPassFilterCoef_Analysis_addr        DSP2_CM_CODE2_ADDR+0x300+158
            #define SRS_PURESOUND_ftbLowFilterCoef_Audio_addr               DSP2_CM_CODE2_ADDR+0x300+173
            #define SRS_PURESOUND_ftbLowFilterCoef_Analysis_addr            DSP2_CM_CODE2_ADDR+0x300+177
            #define SRS_PURESOUND_ftbMidFilterCoef_Audio_addr               DSP2_CM_CODE2_ADDR+0x300+129
            #define SRS_PURESOUND_ftbMidFilterCoef_Analysis_addr            DSP2_CM_CODE2_ADDR+0x300+123

            #define SRS_PURESOUND_mDummy_addr                       DSP2_DM_SEG2_ADDR
            #define SRS_PURESOUND_SRS_EN_BITS_addr                  DSP2_DM_SEG2_ADDR+1
            #define SRS_PURESOUND_mInputGain_addr                   DSP2_DM_SEG2_ADDR+2
            #define SRS_PURESOUND_mOutputGain_addr                  DSP2_DM_SEG2_ADDR+3
            #define SRS_PURESOUND_mBypassGain_addr                  DSP2_DM_SEG2_ADDR+4
            #define SRS_PURESOUND_SpeakerSize_addr                  DSP2_DM_SEG2_ADDR+5
            #define SRS_PURESOUND_TruBassControl_addr               DSP2_DM_SEG2_ADDR+6
            #define SRS_PURESOUND_SpeakerSize_Audio_addr            DSP2_DM_SEG2_ADDR+7
            #define SRS_PURESOUND_SpeakerSize_Analysis_addr         DSP2_DM_SEG2_ADDR+8
            #define SRS_PURESOUND_tbc_mCompressorControl_addr       DSP2_DM_SEG2_ADDR+9
            #define SRS_PURESOUND_tbc_mLevelIndependent_addr        DSP2_DM_SEG2_ADDR+10
            #define SRS_PURESOUND_tbc_mMode_addr                    DSP2_DM_SEG2_ADDR+11
            #define SRS_PURESOUND_mHPFfc_addr                       DSP2_DM_SEG2_ADDR+12
            #define SRS_PURESOUND_TBHD_init_flag_addr               DSP2_DM_SEG2_ADDR+13
            #define SRS_PURESOUND_hlInputGain_addr                  DSP2_DM_SEG2_ADDR+25
            #define SRS_PURESOUND_hlOutputGain_addr                 DSP2_DM_SEG2_ADDR+26
            #define SRS_PURESOUND_hlBypassGain_addr                 DSP2_DM_SEG2_ADDR+27
            #define SRS_PURESOUND_hlLimiterboost_addr               DSP2_DM_SEG2_ADDR+28
            #define SRS_PURESOUND_hlHardLimit_addr                  DSP2_DM_SEG2_ADDR+29
            #define SRS_PURESOUND_hlDelaylen_addr                   DSP2_DM_SEG2_ADDR+30
            #define SRS_PURESOUND_AeqInputGain_addr                 DSP2_DM_SEG2_ADDR+32
            #define SRS_PURESOUND_AeqOutputGain_addr                DSP2_DM_SEG2_ADDR+33
            #define SRS_PURESOUND_AeqBypassGain_addr                DSP2_DM_SEG2_ADDR+34
            #define SRS_PURESOUND_srs_hl_tbl_addr                   DSP2_DM_SEG2_ADDR+35

        /* srs tshd_tsxt */
            #define SRS_TSHD_base_addr                        DSP2_DM_SEG2_ADDR
            #define SRS_TSXT_base_addr                        DSP2_DM_SEG2_ADDR

        /* dbx */
            #define DBX_PM_ABS_ADDRESS  0x14C0
            #define DBX_DM_ABS_ADDRESS  0x2F00


        /* abs3d */
            #define ABS3D_parameter_addr  0x14C0
            #define ABS3D_PM_ABS_SIZE     0x100
            #define ABS3D_DM_ABS_ADDRESS  0x2F00

        /* Bongiovi DPS */
            // The magic number of 8 is for the purpose to let the address of StereoWidener_xover1LO_addr is aligned by 8 dsp word address.
            // And leaves 3 dsp word PM SPACE for DPS_PARAM_COUNT_ADDR, DPS_PARAM_UPDATE_COUNT_ADDR and StereoWidener_bypass_addr just before StereoWidener_xover1LO_addr.
            // -- BONGIOVI_DPS_PM_addr                  -> PM SEGMENT ADDRESS FOR Bongiovi DPS PARAMETER
            //                                          -> XXXX (DUMMY)
            //                                          -> ........
            //                                          -> BONGIOVI_DPS_STEREO_WIDENER_MIDGAIN_CONTROL_ADDR
            //                                          -> BONGIOVI_DPS_STEREO_WIDENER_LOGAIN_CONTROL_ADDR
            //                                          -> BONGIOVI_DPS_STEREO_WIDENER_SGAIN_CONTROL_ADDR
            //                                          -> BONGIOVI_DPS_STEREO_WIDENER_MGAIN_CONTROL_ADDR
            //                                          -> BONGIOVI_DPS_GLOBAL_OUTPUT_GAIN_CONTROL_ADDR
            //                                          -> BONGIOVI_DPS_GLOBAL_INPUT_GAIN_CONTROL_ADDR
            //                                          -> DPS_PARAM_COUNT_ADDR
            //                                          -> DPS_PARAM_UPDATE_COUNT_ADDR
            //                                          -> StereoWidener_bypass_addr
            // -- BONGIOVI_DPS_COEF_PM_BASE_ADDR        -> StereoWidener_xover1LO_addr, Bongiovi DPS PARAMETER (StereoWidener_xover1LO) ADDRESS WHICH NEEDS TO ALIGNED by 8
            #define BONGIOVI_DPS_CM_PM_OFFSET           0x420   // The offset (PM SEG - CM SEG) if there is only one code segment for advance sound at arch.sys
            #define BONGIOVI_DPS_PM_addr                (DSP2_CM_CODE2_ADDR+BONGIOVI_DPS_CM_PM_OFFSET)
            #define BONGIOVI_DPS_COEF_PM_BASE_ADDR      ((((BONGIOVI_DPS_PM_addr+8-5)/8)+1)*8)
            #define BONGIOVI_DPS_STEREO_WIDENER_ADDR    (BONGIOVI_DPS_COEF_PM_BASE_ADDR-1)
            #define BONGIOVI_DPS_UPDATE_COUNT_ADDR      (BONGIOVI_DPS_COEF_PM_BASE_ADDR-2)
            #define BONGIOVI_DPS_PARAM_COUNT_ADDR       (BONGIOVI_DPS_COEF_PM_BASE_ADDR-3)
            #define BONGIOVI_DPS_GLOBAL_INPUT_GAIN_CONTROL_ADDR       (BONGIOVI_DPS_COEF_PM_BASE_ADDR-4)
            #define BONGIOVI_DPS_GLOBAL_OUTPUT_GAIN_CONTROL_ADDR      (BONGIOVI_DPS_COEF_PM_BASE_ADDR-5)
            #define BONGIOVI_DPS_STEREO_WIDENER_MGAIN_CONTROL_ADDR    (BONGIOVI_DPS_COEF_PM_BASE_ADDR-6)
            #define BONGIOVI_DPS_STEREO_WIDENER_SGAIN_CONTROL_ADDR    (BONGIOVI_DPS_COEF_PM_BASE_ADDR-7)
            #define BONGIOVI_DPS_STEREO_WIDENER_LOGAIN_CONTROL_ADDR   (BONGIOVI_DPS_COEF_PM_BASE_ADDR-8)
            #define BONGIOVI_DPS_STEREO_WIDENER_MIDGAIN_CONTROL_ADDR  (BONGIOVI_DPS_COEF_PM_BASE_ADDR-9)

/********************************************************************
*  Decoder default setting
********************************************************************/
    /* default AVsync delay */
        #define MPG_AV_DEFAULT_DELAY_2                0x3E1E
        #define AC3_AV_DEFAULT_DELAY_2                0x4300
        #define AAC_AV_DEFAULT_DELAY_2                0x327E
        #define DELAY_UPPER_LIMIT_2                   0x20F58
        #define DELAY_LOWER_LIMIT_2                   0xA8C
            #define AAC_AV_DELAY_LC_OFFSET_2          (90*30)
            #define AAC_AV_DELAY_SBR_OFFSET_2         -(90*20)

    /* SIF DSP PM vars */
        /*  for SIF PAL DSP PM vars */
            #define ADDR_gain_base_2                  0x1921
            #define ADDR_thr_base_2		              0x1A20
            #define ADDR_pfir_base_2                  0x1A90
            //  for SIF BTSC DSP PM vars //
            #define BTSC_COMPILE_OPTION_Addr_2        0x19F1   // len 1
            #define BTSC_OUTPUT_GAIN_Addr_2           0x1A21   // len 2
            #define BTSC_THRESHOLD_Addr_2             0x1A23   // len 10
            #define MTS_OUTPUT_GAIN_Addr_2            0x1A34   //len 6
            #define SIF_AGC_THRESHOLD_Addr_2          0x192D   //len 3

            /// PAL gain setting address
            #define ADDR_fm_stdM_gain_2               ADDR_gain_base_2           // len = 4
            #define ADDR_fm_stdX_gain_2               ADDR_fm_stdM_gain_2+4      // len = 4
            #define ADDR_nicam_gain_2                 ADDR_fm_stdX_gain_2+4      // len = 2
            #define ADDR_am_gain_2                    ADDR_nicam_gain_2+2        // len = 2
            #define ADDR_agc_gain_2                   ADDR_am_gain_2+2           // len = 24

            // PAL threshold setting address
            #define ADDR_a2_stdM_thr_2                ADDR_thr_base_2            // len = 15
            #define ADDR_a2_stdBG_thr_2               ADDR_a2_stdM_thr_2+15    	 // len = 15
            #define ADDR_a2_stdDK_thr_2               ADDR_a2_stdBG_thr_2+15 		 // len = 15
            #define ADDR_a2_stdI_thr_2                ADDR_a2_stdDK_thr_2+15   	 // len = 4
            #define ADDR_am_thr_2                     ADDR_a2_stdI_thr_2+4       // len = 3
            #define ADDR_hidev_stdM_thr_2             ADDR_am_thr_2+3            // len = 4
            #define ADDR_hidev_stdBG_thr_2            ADDR_hidev_stdM_thr_2+4  	 // len = 4
            #define ADDR_hidev_stdDK_thr_2            ADDR_hidev_stdBG_thr_2+4   // len = 4
            #define ADDR_hidev_stdI_thr_2             ADDR_hidev_stdDK_thr_2+4 	 // len = 4
            #define ADDR_nicam_stdBG_pherr_thr_2      ADDR_hidev_stdI_thr_2+4  	 //len = 3
            #define ADDR_nicam_stdI_pherr_thr_2       ADDR_nicam_stdBG_pherr_thr_2+3  // len = 3
            #define ADDR_a2_bg_nicam_fm_nsr_thr_2     0x186F	 // len = 1
            #define ADDR_a2_dk_nicam_fm_nsr_thr_2     0x1870 	 // len = 1

            // pfir setting address
            #define ADDR_hidev_demfir_2               ADDR_pfir_base_2             // len = 15
            #define ADDR_fm_ch1_pfir_2                ADDR_hidev_demfir_2+16       // len = 30
            #define ADDR_fm_ch2_pfir_2                ADDR_fm_ch1_pfir_2+30        // len = 30
            #define ADDR_hidev_lv1_pfir_2             ADDR_fm_ch2_pfir_2+30        // len = 20
            #define ADDR_hidev_lv2_pfir_2             ADDR_hidev_lv1_pfir_2+20     // len = 20
            #define ADDR_hidev_lv3_pfir_2             ADDR_hidev_lv2_pfir_2+20     // len = 20

            // BTSC threshold setting address
            #define HIDEV_NSR_THRESHOLD_Addr_2        BTSC_THRESHOLD_Addr_2+10            // len 2
            #define BTSC_MONO_AMP_THRESHOLD_Addr_2    HIDEV_NSR_THRESHOLD_Addr_2+2        // len 2
            #define HIDEV_AMP_THRESHOLD_Addr_2        BTSC_MONO_AMP_THRESHOLD_Addr_2+2    // len 2

            #define BTSC_SAP_PHASE_DIFF_CLIP_THR_Addr_2    HIDEV_AMP_THRESHOLD_Addr_2+2   // len 1
            #define BTSC_PILOT_DEBOUNCE_THRESHOLD_Addr_2   MTS_OUTPUT_GAIN_Addr_2+6       // len 3
            #define BTSC_SAP_ON_DETECTION_DEBOUNCE_THRESHOLD_Addr_2 BTSC_PILOT_DEBOUNCE_THRESHOLD_Addr_2+3 // len 1

/************************************************
*   Below is MailBox config
*************************************************/

    /************************************************
    *   MCU to DSP mailbox
    ************************************************/
    #define M2S_MBOX_AAC_SINGLE_FRAME_MODE          MB_2D92

    #define M2S_MBOX_DDC_MODSEL                     MB_2D80
    #define M2S_MBOX_DDC_MODSEL_BSTART              MBOX_BITS_SHIFT-0
    #define M2S_MBOX_DDC_MODSEL_BMASK               0x3
    #define M2S_MBOX_DDC_MODSEL_MULTIOUT_BSTART     MBOX_BIT3
    #define M2S_MBOX_DDC_MODSEL_MULTIOUT_BMASK      0x7
    #define M2S_MBOX_DDC_OUTSEL_BSTART              MBOX_BITS_SHIFT-4
    #define M2S_MBOX_DDC_OUTSEL_BMASK               0x3
    #define M2S_MBOX_DDC_OUTSEL2_BSTART             MBOX_BITS_SHIFT-8
    #define M2S_MBOX_DDC_OUTSEL2_BMASK              0x3

    #define M2S_MBOX_DDC_ASOC_ID_SEL                MB_2D82
    #define M2S_MBOX_ASOC_IDSEL_BSTART              MBOX_BITS_SHIFT-0
    #define M2S_MBOX_ASOC_IDSEL_BMASK               0x3

    #define M2S_MBOX_DUALMOD_SEL                    MB_2D86
    #define M2S_MBOX_DUALMOD_SEL_BSTART             MBOX_BITS_SHIFT-2
    #define M2S_MBOX_DUALMOD_SEL_BMASK              0x3

    #define M2S_MBOX_DEC_CTRL                       MB_2D86
    #define M2S_MBOX_DEC1_USE_EXT_SYNTH_BIT         MBOX_BIT0
    #define M2S_MBOX_MM_FF2X_BIT                    MBOX_BIT5
    #define M2S_MBOX_DEC_MUTE_BIT                   MBOX_BIT6
    #define M2S_MBOX_PLAY_WO_OUT                    MBOX_BIT7
    #define M2S_MBOC_DDC_MINUS_6DB                  MBOX_BIT8
    #define M2S_MBOX_MINUS_11_DB_BIT                MBOX_BIT1
    #define M2S_MBOX_ES1_LIMITER_EN                 MBOX_BIT14
    #define M2S_MBOX_PCM_LIMITER_EN                 MBOX_BIT15

    #define M2S_MBOX_MISC_SETTING                   MB_2D8E
    #define M2S_MBOX_MMTS_SYNCSTC_BIT               MBOX_BIT4
    #define M2S_MBOX_DTV_SYNC_VIDEO                 MBOX_BIT10

    #define M2S_MBOX_SPDIF_SETTING                  MB_2D8E
    #define MBOX_SPDIF_SETTING_BIT_MUTE             MBOX_BIT0
    #define MBOX_SPDIF_SETTING_BIT_NPCMSEL          MBOX_BIT1
    #define MBOX_SPDIF_SETTING_R2_NPCM_SELBIT       MBOX_BIT2
    #define MBOX_SPDIF_SETTING_DVB2_NPCM_SELBIT     MBOX_BIT3
    #define MBOX_SPDIF_SETTING_MINUS_11DB_BIT       MBOX_BIT5

    #define M2S_MBOX_DRC_SETTING                    MB_2D8E
    #define M2S_MBOX_SPDIF_NPCM_MODE                MBOX_BIT1
    #define M2S_MBOX_DRC_BIT                        MBOX_BIT7
    #define M2S_MBOX_STDMX_BIT                      MBOX_BIT6
    #define M2S_MBOX_AUTO_STDMX_BIT                 MBOX_BIT9
    #define M2S_MBOX_DOLBY11_3DB_ATTENUATOR		    MBOX_BIT12

    #define M2S_MBOX_LOW_HIGH_CUT               MB_2D98
    #define MBOX_HIGHCUT_BSTART                 MBOX_BITS_SHIFT-0
    #define MBOX_HIGHCUT_BMASK                  0xF
    #define MBOX_LOWCUT_BSTART                  MBOX_BITS_SHIFT-4
    #define MBOX_LOWCUT_BMASK                   0xF

    #define M2S_MBOX_HDMI_SETTING               MB_2D8E
    #define M2S_MBOX_HDMI_NPCM_MODE             MBOX_BIT1
    #define M2S_MBOX_HDMI_DDP_BYPASS_BIT        MBOX_BIT14
    #define M2S_MBOX_HDMI_SETTING_BIT_MUTE      MBOX_BIT15

    #define M2S_MBOX_AUOUT0_VOL                 MB_2D00
    #define M2S_MBOX_AUOUT1_VOL                 MB_2D02
    #define M2S_MBOX_AUOUT2_VOL                 MB_2D04
    #define M2S_MBOX_AUOUT3_VOL                 MB_2D06
    #define M2S_MBOX_I2S_VOL                    MB_2D08
    #define M2S_MBOX_SPDIF_VOL                  MB_2D0A
    #define M2S_MBOX_SRC_VOL                    MB_2D0C
    #define M2S_MBOX_HDMI_VOL                   MB_2D0E    //STB only
    #define M2S_MBOX_I2S2_VOL                   MB_2D0E
    #define M2S_MBOX_PRESCALE                   MB_2D10

    #define M2S_MBOX_EQ1_GAIN                   MB_2D14             //[15:8]
    #define M2S_MBOX_BASS_CTRL                  MB_2D14             //[7:0]
    #define M2S_MBOX_EQ2_GAIN                   MB_2D16             //[15:8]
    #define M2S_MBOX_TREBLE_CTRL                MB_2D16             //[7:0]
    #define M2S_MBOX_EQ3_GAIN                   MB_2D18             //[15:8]
    #define M2S_MBOX_SUPBASS_CTRL               MB_2D18             //[7:0]
    #define M2S_MBOX_EQ4_GAIN                   MB_2D1A             //[15:8]
    #define M2S_MBOX_EQ5_GAIN                   MB_2D1C             //[15:8]
    #define M2S_MBOX_BAL_CTRL                   MB_2D1E             //L[15:8], R[7:0]

    #define M2S_MBOX_SNDEFF_EN                  MB_2D20
        #define M2S_MBOX_STEREO_EN_BIT              MBOX_BIT15
        #define M2S_MBOX_DRC_EN_BIT                 MBOX_BIT13
        #define M2S_MBOX_AVC_EN_BIT                 MBOX_BIT12
        #define M2S_MBOX_TONE_EN_BIT                MBOX_BIT11
        #define M2S_MBOX_SPATIAL_EN_BIT             MBOX_BIT10
        #define M2S_MBOX_VOLBAL_EN_BIT              MBOX_BIT9
        #define M2S_MBOX_GEQ_EN_BIT                 MBOX_BIT7
        #define M2S_MBOX_EASE_EN_BIT                MBOX_BIT6
        #define M2S_MBOX_BASSBOOST_EN_BIT           MBOX_BIT5
        #define M2S_MBOX_ECHO_EN_BIT                MBOX_BIT4
        #define M2S_MBOX_DC_REMOVE_EN_BIT           MBOX_BIT3
        #define M2S_MBOX_HPF_EN_BIT                 MBOX_BIT2
        #define M2S_MBOX_COEFFUPDATE_EN_BIT         MBOX_BIT1
        #define M2S_MBOX_PEQ_EN_BIT                 MBOX_BIT0

    #define M2S_MBOX_VOLUME_EN                  MB_2D22
        #define M2S_MBOX_VOL_DAC0_EN_BIT            MBOX_BIT0
        #define M2S_MBOX_VOL_DAC1_EN_BIT            MBOX_BIT1
        #define M2S_MBOX_VOL_DAC2_EN_BIT            MBOX_BIT2
        #define M2S_MBOX_VOL_DAC3_EN_BIT            MBOX_BIT3
        #define M2S_MBOX_VOL_I2S0_EN_BIT            MBOX_BIT4
        #define M2S_MBOX_VOL_I2S1_EN_BIT            MBOX_BIT5
        #define M2S_MBOX_VOL_I2S2_EN_BIT            MBOX_BIT6
        #define M2S_MBOX_VOL_I2S3_EN_BIT            MBOX_BIT7
        #define M2S_MBOX_VOL_SPDIF_EN_BIT           MBOX_BIT8
        #define M2S_MBOX_VOL_SRC_EN_BIT             MBOX_BIT9
        #define M2S_MBOX_VOL_SE_VOL1_EN_BIT         MBOX_BIT13
        #define M2S_MBOX_VOL_SE_VOL2_EN_BIT         MBOX_BIT14

    #define M2S_MBOX_AVC_CTRL                   MB_2D24         //[15:13] AT, [12:10] RT, [9:8] AVC_Mode, [7:0] AVC Threshold
    #define M2S_MBOX_SURR_CTRL                  MB_2D26

    #define M2S_MBOX_SINE_GEN                   MB_2D28         //[15:8] Tone key CTRL, [7] Enable bit
        #define M2S_MBOX_SINE_GEN_CTRL_MASK         0x7F
    	#define M2S_MBOX_SINE_GEN_EN_BIT            MBOX_BIT7
    	#define M2S_MBOX_MENUSOUND_EN_BIT           MBOX_BIT0   //[15:8] LOOP CNT CTRL, [0] Enable bit

    #define M2S_MBOX_BALANCE_EN                 MB_2D2A
        #define M2S_MBOX_BAL_DAC0_EN_BIT            MBOX_BIT0
        #define M2S_MBOX_BAL_DAC1_EN_BIT            MBOX_BIT1
        #define M2S_MBOX_BAL_DAC2_EN_BIT            MBOX_BIT2
        #define M2S_MBOX_BAL_DAC3_EN_BIT            MBOX_BIT3
        #define M2S_MBOX_BAL_I2S0_EN_BIT            MBOX_BIT4
        #define M2S_MBOX_BAL_I2S1_EN_BIT            MBOX_BIT5
        #define M2S_MBOX_BAL_I2S2_EN_BIT            MBOX_BIT6
        #define M2S_MBOX_BAL_I2S3_EN_BIT            MBOX_BIT7
        #define M2S_MBOX_BAL_SPDIF_EN_BIT           MBOX_BIT8
        #define M2S_MBOX_BAL_SRC_EN_BIT             MBOX_BIT9

    #define M2S_MBOX_PHASE_SHIFT_CTRL           MB_2D2C
        #define M2S_MBOX_PHASE_SHIFT_DAC0_L_BIT     MBOX_BIT0
        #define M2S_MBOX_PHASE_SHIFT_DAC0_R_BIT     MBOX_BIT1
        #define M2S_MBOX_PHASE_SHIFT_DAC1_L_BIT     MBOX_BIT2
        #define M2S_MBOX_PHASE_SHIFT_DAC1_R_BIT     MBOX_BIT3
        #define M2S_MBOX_PHASE_SHIFT_DAC2_L_BIT     MBOX_BIT4
        #define M2S_MBOX_PHASE_SHIFT_DAC2_R_BIT     MBOX_BIT5
        #define M2S_MBOX_PHASE_SHIFT_DAC3_L_BIT     MBOX_BIT6
        #define M2S_MBOX_PHASE_SHIFT_DAC3_R_BIT     MBOX_BIT7
        #define M2S_MBOX_PHASE_SHIFT_I2S0_L_BIT     MBOX_BIT8
        #define M2S_MBOX_PHASE_SHIFT_I2S0_R_BIT     MBOX_BIT9
        #define M2S_MBOX_PHASE_SHIFT_I2S1_L_BIT     MBOX_BIT10
        #define M2S_MBOX_PHASE_SHIFT_I2S1_R_BIT     MBOX_BIT11
        #define M2S_MBOX_PHASE_SHIFT_I2S2_L_BIT     MBOX_BIT12
        #define M2S_MBOX_PHASE_SHIFT_I2S2_R_BIT     MBOX_BIT13
        #define M2S_MBOX_PHASE_SHIFT_I2S3_L_BIT     MBOX_BIT14
        #define M2S_MBOX_PHASE_SHIFT_I2S3_R_BIT     MBOX_BIT15

    #define M2S_MBOX_DRC_CTRL                   MB_2D2E         //[15:9] reserved, [8] DRC_Entry (DRC behind Vol), [7:0] DRC Threshold
        #define M2S_MBOX_DRC_ENTRY_EN_BIT           MBOX_BIT8

    #define M2S_MBOX_SOUND_MODE_SEL             MB_2D30             //[1:0] : LR / LL / RR
    #define M2S_MBOX_POWER_DOWN                 MB_2D30             //[8:9]
    #define M2S_MBOX_NR_CTRL                    MB_2D32

    #define M2S_MBOX_ADVSND_EN                  MB_2D40
    	#define M2S_MBOX_ADVSND_SUMMARY_EN_BIT      MBOX_BIT15

    #define M2S_MBOX_KTV_CTRL                   MB_2D46
    #define M2S_MBOX_KTV_EN_BIT                 MBOX_BIT15

    /* no use any more
    #define M2S_MBOX_VOIP_CTRL                  MB_2D48
        #define M2S_MBOX_VOIP_EN_BIT                    MBOX_BIT15 */

    #define M2S_MBOX_CAPTURE_CTRL               MB_2D4A   //[7:0] PCM_capture1  [15:8] PCM_capture2
        #define M2S_MBOX_GET_CH5                    1
        #define M2S_MBOX_GET_CH6                    2
        #define M2S_MBOX_GET_CH7                    3
        #define M2S_MBOX_GET_CH8                    4
        #define M2S_MBOX_GET_ADC1                   5
        #define M2S_MBOX_GET_ADC2                   6
        #define M2S_MBOX_GET_Raw_Delay_SE           7
        #define M2S_MBOX_GET_MIXER                  8
        #define M2S_MBOX_GET_Raw                    9
        #define M2S_MBOX_GET_DEBUG                  128

    #define M2S_MBOX_DTS_CH_CTRL                MB_2D96

    /* SIF */
    #define M2S_MBOX_SIF_CMD_STANDARD           MB_2DC0
    #define M2S_MBOX_SIF_CMD_PFIRBANDWIDTH      MB_2DC2
    #define M2S_MBOX_SIF_CMD_MODE1              MB_2DC4
    #define M2S_MBOX_SIF_CMD_MODE2              MB_2DC6

    #define M2S_MBOX_BT_CTRL                    MB_2D4C

    #define M2S_MBOX_INPUT_MUX_SEL1             MB_2D50
    #define M2S_MBOX_INPUT_MUX_SEL2             MB_2D52
    #define M2S_MBOX_INPUT_MUX_SEL3             MB_2D54

    #define M2S_MBOX_KTV8_VOL                   MB_2D5A
    #define M2S_MBOX_KTV5_VOL                   MB_2D5C
    #define M2S_MBOX_KTV6_VOL                   MB_2D5E

    #define M2S_MBOX_CH7_VOL                    MB_2D58

    /* MAIN Volume for AD */
    #define M2S_MBOX_MAIN_CONTROL               MB_2D48
    #define M2S_MBOX_MAIN_VOL_BSTART            MBOX_BITS_SHIFT-0
    #define M2S_MBOX_MAIN_VOL_BMASK             0x7FF

    #define M2S_MBOX_AD_CONTROL                 MB_2DD8
        #define M2S_MBOX_AD_MIX_EN_BIT              MBOX_BIT15          ! [15]: AD_MIX_EN
        #define M2S_MBOX_MIX_MODE_BSTART            MBOX_BITS_SHIFT-11  ! [13:11]: MIX_MODE_SELECT
        #define M2S_MBOX_MIX_MODE_BMASK             0x7
        #define M2S_MBOX_AD_VOL_BSTART              MBOX_BITS_SHIFT-0   ! [10:0]: AD VOL
        #define M2S_MBOX_AD_VOL_BMASK               0x7FF

        /* M2S_MBOX_MIX_MODE */
        #define GPA_MIX_MODE_IS_FORWARD         0
        #define GPA_MIX_MODE_IS_BACKWARD        1
        #define GPA_MIX_MODE_IS_NULL            2      ! 2 & 3: NULL_Mixer_Mode

    #define M2S_MBOX_DBG_CMD1                   MB_2DDC
    #define MBOX_DBGCMD_SET_ADDR                0x0200
    #define MBOX_DBGCMD_WRITE_DM                0x0300
    #define MBOX_DBGCMD_WRITE_PM                0x0400
    #define MBOX_DBGCMD_READ_DM                 0x0500
    #define MBOX_DBGCMD_READ_PM                 0x0600
    #define MBOX_DBGCMD_READ_PMASK              0x0700
    #define MBOX_DBGCMD_READ_IMASK              0x0800

    #define MBOX_DBGCMD_READ_MAIN_VER           0x9000
    #define MBOX_DBGCMD_READ_ALG1_VER           0x9100
    #define MBOX_DBGCMD_READ_ALG2_VER           0x9200

    #define MBOX_DBGCMD_SoundEffTest                       0xD000
    #define MBOX_DBGCMD_GPA_ISR_DISABLE_AND_SE_DMA_Init    (MBOX_DBGCMD_SoundEffTest)
    #define MBOX_DBGCMD_GPA_ISR_ENABLE                     (MBOX_DBGCMD_SoundEffTest+1)
    #define MBOX_DBGCMD_TONE_TEST_BEGIN                    (MBOX_DBGCMD_SoundEffTest+2)
    #define MBOX_DBGCMD_PEQ_TEST_BEGIN                     (MBOX_DBGCMD_SoundEffTest+3)
    #define MBOX_DBGCMD_GEQ_TEST_BEGIN                     (MBOX_DBGCMD_SoundEffTest+4)
    #define MBOX_DBGCMD_AVC_TEST_BEGIN                     (MBOX_DBGCMD_SoundEffTest+5)
    #define MBOX_DBGCMD_SRS_THEATER_SOUND_TEST_BEGIN       (MBOX_DBGCMD_SoundEffTest+6)
    #define MBOX_DBGCMD_SONICEMOTION_ABS3D_TEST_BEGIN      (MBOX_DBGCMD_SoundEffTest+7)
    #define MBOX_DBGCMD_BONGIOVI_DPS_TEST_BEGIN            (MBOX_DBGCMD_SoundEffTest+8)
    #define MBOX_DBGCMD_DTS_STUDIOSOUND_3D_TEST_BEGIN      (MBOX_DBGCMD_SoundEffTest+9)
    #define MBOX_DBGCMD_SE_PROCESS_BEGIN                   0xD090
    #define MBOX_DBGCMD_SE_PROCESS_FEED                    0xD091
    #define MBOX_DBGCMD_SE_PROCESS_CONSUME                 0xD092
    #define MBOX_DBGCMD_SE_PROCESS_CONSUME_NEED            0xD092
    #define MBOX_DBGCMD_SE_PROCESS_CONSUME_NO_NEED         0xD192

    //#define MBOX_DBGCMD_MIP_INT                 0xE000
    //#define MBOX_DBGCMD_ENC_INT                 0xE100
    #define MBOX_DBGCMD_FILE_PTS_INT            0xEA00

    #define MBOX_DBGCMD_RELOAD_SIF_BEG          0xF000
    #define MBOX_DBGCMD_RELOAD_SIF_END          0xF100
    #define MBOX_DBGCMD_WAIT_MCU_START          0xF300
    #define MBOX_DBGCMD_RELOAD_ADVSND_BEG       0xF400
    #define MBOX_DBGCMD_RELOAD_ADVSND_END       0xF500

    #define M2S_MBOX_PIO_ID                     MB_2DCC             //[15:8]
    #define M2S_MBOX_MM_FILEIN_TAG              MB_2DCC             //[7:0]
    #define MBOX_DBGCMD_MHEQ5_INT               0xE000
    #define MBOX_DBGCMD_UNI_INT                 0xE300

    #define M2S_MBOX_UNI_PCMOUT_CNT                 MB_2DCA
    #define M2S_MBOX_UNI_INPUT_ES_WPTR              MB_2DD2
    #define M2S_MBOX_UNI_NEED_DECODE_FRMCNT         MB_2DC8
    #define M2S_MBOX_UNI_PCM3_WRPTR                 MB_2DCE

    #define S2M_MBOX_UNI_SPDIF_WRPTR                MB_2D74                 //Uni Player
    #define S2M_MBOX_UNI_PCM_WPTR                   MB_2D76                 //Uni Player
    #define S2M_MBOX_UNI_PCM_SIZE                   MB_2D78                 //Uni Player
    #define S2M_MBOX_UNI_PCM_BUFFEBT                MB_2D7A                 //Uni Player
    #define S2M_MBOX_UNI_FRAME_CNT                  MB_2D7C                 //Uni Player
    #define S2M_MBOX_UNI_MUTECNT                    MB_2D7E                 //Uni Player

    #define M2S_MBOX_DBG_CMD2                       MB_2DDE

    #define M2S_MBOX_SW_DMA_READER_DDR_WtPtr        MB_2D34
    #define M2S_MBOX_SW_DMA_READER_DDR_Ctrl         MB_2D36
    #define M2S_MBOX_SW_DMA_READER_DDR_SynthFreq    MB_2D56

    #define M2S_MBOX_PCM_CAPTURE_DDR_RdPtr          MB_2DD4
    #define M2S_MBOX_PCM_CAPTURE_DDR_Size           MB_2DD6

    #define M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr         MB_2D38
    #define M2S_MBOX_PCM_CAPTURE2_DDR_Size          MB_2D3A

    #define M2S_MBOX_DOLBY_LOUDNESS_INFO            MB_2D98
        #define MBOX_DOLBY_LOUDNESS_ENABLE_BIT          MBOX_BIT15
        #define MBOX_DOLBY_LOUDNESS_ATSC_MODE           MBOX_BIT14
        #define MBOX_DOLBY_LOUDNESS_OTHER_MODE          MBOX_BIT13
    /************************************************
    *   DSP to MCU mailbox
    ************************************************/
    #define S2M_MBOX_ES_MEMCNT                  MB_2D70
    #define S2M_MBOX_PCM_MEMCNT                 MB_2D72
    #define S2M_MBOX_MM_BROWSE_TIME             MB_2D74
    #define S2M_MBOX_MM_PTS_IN_SEC              MB_2D76
    #define S2M_MBOX_MM_PTS_IN_MSEC             MB_2D78
    #define S2M_MBOX_MM_PTS_HI                  MB_2D7A
    #define S2M_MBOX_MM_PTS_ME                  MB_2D7C
    #define S2M_MBOX_MM_PTS_LO                  MB_2D7E

    #define S2M_MBOX_VOIP_FIFO_WRPTR            MB_2D74
    #define S2M_MBOX_VOIP_MAGIC_WORD            MB_2D76

    #define DDP_HDMI_TX_4X_BIT                  MBOX_BIT4

    #define S2M_MBOX_MM_FILE_REQ_ADDR           MB_2D7A
    #define S2M_MBOX_MM_FILE_REQ_SIZE           MB_2D7C
    #define S2M_MBOX_MM_FILE_PLAY_END           MB_2D7E

    #define S2M_MBOX_DEC_STATUS                 MB_2DFA

    #define S2M_MBOX_SIF_DETECTION_RESULT       MB_2DE0
    #define S2M_MBOX_SIF_STATUS_MODE1           MB_2DE4
    #define S2M_MBOX_SIF_STATUS_MODE2           MB_2DE6

    #define S2M_MBOX_MENU_KEY_CNT               MB_2DEA

    #define S2M_MBOX_NR_STATUS                  MB_2DEE
    #define S2M_MBOX_BSND_STATUS                MB_2DEE
        #define MBOX_NR_WORKING_NOW                 MBOX_BIT0           // 1: NR working now            , 0 NR not working
        #define MBOX_TONE_FUNC_SELECT               MBOX_BIT1           // 0: EQ_Bass_Treble            , 1: Bass_Treble_old
        #define MBOX_PEQ_FUNC_SELECT                MBOX_BIT2           // 0: PEQ: single precision     , 1: double precision
        #define MBOX_DRC_ENTRY_SELECT               MBOX_BIT3           // 1: Volume before DRC         , 0: Volume behined DRC (default)

    #define S2M_MBOX_MAIN_OVERFLOW_CNT          MB_2DF2                 //[15:8], full cnt of input SRAM buff2
    #define S2M_MBOX_MAIN_UNDERFLOW_CNT         MB_2DF2                 //[ 7:0], empty cnt of output SRAM buff1

    #define S2M_MBOX_ISR_CNTR                   MB_2DF6                 //[7:0]
    #define S2M_MBOX_INTR_CMDTYPE               MB_2DF6                 //[15:8]
    #define SE_DSP_INTR_CMD_MMES_NEED_DATA      0x0300
    #define SE_DSP_INTR_CMD_REPORT_PTS          0x0500
    #define SE_DSP_INTR_CMD_MMUNI_NEED_DATA     0x0600
    #define SE_DSP_INTR_CMD_VOIP                0x0900
    #define DSP_INTR_CMD_PCM_UPLOAD             0x3300
    #define DSP_INTR_CMD_PCM_DOWNLOAD           0xC000

    #define S2M_MBOX_WHILE1_CNTR                MB_2DF8                 //[15:8] Always in High Byte
    #define S2M_MBOX_TIMER_CNTR                 MB_2DF8                 //[ 7:0] Always in Low Byte

    #define S2M_MBOX_DBG_RESULT1                MB_2DFC                 //
    #define S2M_MBOX_DSP_INIT_ACK               0x00E3

    #define S2M_MBOX_DBG_RESULT2                MB_2DFE                 //
    #define MBOX_DSP_RELOAD_ACK1                0x0033
    #define MBOX_DSP_RELOAD_ACK2                0x0077

    #define S2M_MBOX_SW_DMA_READER_DDR_Level    MB_2DE2

    #define S2M_MBOX_PCM_CAPTURE_DDR_WrPtr      MB_2DF0
    #define S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr     MB_2DF4

    // NEW for MM in SE DSP
    #define S2M_MBOX_DEC_INTENEL_USE1           MB_2DE8
    #define S2M_MBOX_DEC_INTENEL_USE2           MB_2DEA
    #define S2M_MBOX_DEC_INTENEL_USE3           MB_2DEC
    #define S2M_MBOX_DEC_INTENEL_USE4           MB_2DEE
    #define S2M_MBOX_DEC_DECSTATUS              MB_2DFA

    /* sound effect buffer / share buffer setting */
    #define SE_R2_FRAME_SIZE                    256         // 256 samples for R2_SE, refer to #define ASND_PROCESS_ITF_UNIT
    #define SE_PROCESS_FRAME_SMP_UNIT           128         // 128 samples per frame

    /* SE Monitor process DDR1 --> DDR2 */
    #define SE_PROCESS_FETCH_FRAME_LINE_SIZE  (SE_PROCESS_FRAME_SMP_UNIT*SE_PROCESS_FETCH_CHANNELS*3/BYTES_IN_MIU_LINE)
    #define SE_PROCESS_STORE_FRAME_LINE_SIZE  (SE_PROCESS_FRAME_SMP_UNIT*SE_PROCESS_STORE_CHANNELS*3/BYTES_IN_MIU_LINE)


#ifdef _COMPILE_DSP_
/************************************************
*  DSP TSCALE & TCOUNT setting
************************************************/
.const DSP_SYSTEM_FREQUENCY       = 432;
.const TSCALE_CONSTANT            = 0xF9;
.const DSP_TIME_CONSTANT          = DSP_SYSTEM_FREQUENCY/2 -1;
.const DSP_TIMER_SETTING          = DSP_TIME_CONSTANT;

/************************************************
*  DSP Buffer Setting
************************************************/
    #define PTS1TAG_MEM                 dm
    #define PTS2TAG_MEM                 dm

/************************************************
*   Below is DMA config
*************************************************/
    .const DMAITF_DSPCMD_ALIGNMENT_BIT          = 7;           // 1/0 : msb / lsb alignment
    .const DMAITF_DSPCMD_BYTESWAP_BIT           = 6;           // set 1 to byte swap
    .const DMAITF_DSPCMD_READY_BIT              = 5;           // set 1 to trigger, will be 0 when dma is finished
    .const DMAITF_DSPCMD_CLRCNTR_BIT            = 4;           // set 1 to clear
    .const DMAITF_DSPCMD_PRIORITY_BIT           = 3;           // 1/0 : high / low
    .const DMAITF_DSPCMD_24BITS_BIT             = 2;           // 1/0 : 24bits / 16bits

    .const DMAITF_DSPCMD_ALIGNMENT_MASK         = 0x80;
    .const DMAITF_DSPCMD_BYTESWAP_MASK          = 0x40;
    .const DMAITF_DSPCMD_READY_MASK             = 0x20;
    .const DMAITF_DSPCMD_CLRCNTR_MASK           = 0x10;
    .const DMAITF_DSPCMD_PRIORITY_MASK          = 0x08;
    .const DMAITF_DSPCMD_24BITS_MASK            = 0x04;
    .const DMAITF_DSPCMD_BURST_6                = 0x03;
    .const DMAITF_DSPCMD_BURST_3                = 0x02;
    .const DMAITF_DSPCMD_BURST_2                = 0x01;
    .const DMAITF_DSPCMD_BURST_1                = 0x00;

    #if (MIU_128 == 1)
        /* 1 MIU Line = 128bit (16 bytes) */
        #define DMAITF_DSPWORDS_IN_1_LINE           8
        #define DMAITF_DSPWORDS_IN_1LINE_LOG2       3
        #define DMAITF_DSPWORDS_IN_3LINE_LOG2       4
        #define DMAITF_WR_BIT                       16
        #define DMAITF_DM_BIT                       15
        #define DMAITF_BYTES_IN_1MIU_LINE           BYTES_IN_MIU_LINE

        #define DMAITF_RD_PM_MASK                   0x000000
        #define DMAITF_WR_PM_MASK                   0x010000
        #define DMAITF_RD_DM_MASK                   0x008000
        #define DMAITF_WR_DM_MASK                   0x018000

        #define DMAITF_16BITS_B1_DMA_CMD            0xA8                // 16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_DMA_CMD            0xA8                //no Burst 2 cmd, use B1 instead
        #define DMAITF_16BITS_B1_SWAP_DMA_CMD       0xE8                // 16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_SWAP_DMA_CMD       0xE8                //no Burst 2 cmd, use B1 instead

        #define DMAITF_24BITS_B3_DMA_CMD            0x2E                // 24 Bits Burst 3
        #define DMAITF_24BITS_B6_DMA_CMD            0x2E                //no Burst 6 cmd, use B3 instead

        #define DMAITF_16BITS_LowAlign_B1_DMA_CMD   0x28                //16 Bits Burst 1, low alignment
        #define DMAITF_16BITS_LowAlign_B1_SWAP_DMA_CMD   0x68                //16 Bits Burst 1, low alignment
    #else
        /* 1 MIU Line = 64bit (8 bytes) */
        #define DMAITF_DSPWORDS_IN_1_LINE           4
        #define DMAITF_DSPWORDS_IN_1LINE_LOG2       2
        #define DMAITF_DSPWORDS_IN_3LINE_LOG2       3
        #define DMAITF_WR_BIT                       15
        #define DMAITF_DM_BIT                       14
        #define DMAITF_BYTES_IN_1MIU_LINE           BYTES_IN_MIU_LINE

        #define DMAITF_RD_PM_MASK                   0x000000
        #define DMAITF_WR_PM_MASK                   0x008000
        #define DMAITF_RD_DM_MASK                   0x004000
        #define DMAITF_WR_DM_MASK                   0x00C000

        #define DMAITF_16BITS_B1_DMA_CMD            0xA8                // 16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_DMA_CMD            0xA9
        #define DMAITF_16BITS_B1_SWAP_DMA_CMD       0xE8                // 16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_SWAP_DMA_CMD       0xE9

        #define DMAITF_24BITS_B3_DMA_CMD            0x2E                // 24 Bits Burst 3
        #define DMAITF_24BITS_B6_DMA_CMD            0x2F                // 24 Bits Burst 6

        #define DMAITF_16BITS_LowAlign_B1_DMA_CMD   0x28                //16 Bits Burst 1, low alignment
        #define DMAITF_16BITS_LowAlign_B1_SWAP_DMA_CMD   0x68                //16 Bits Burst 1, low alignment
    #endif

    /* DMA Mapping */
        #define SNDISR1_DMA_CTRL        DSPDMA7_DMA_CTRL
        #define SNDISR2_DMA_CTRL        DSPDMA2_DMA_CTRL
        #define SNDBG_DMA_CTRL          DSPDMA3_DMA_CTRL        // Background sound effect
        #define SPDIF_DMA_CTRL          DSPDMA6_DMA_CTRL        // SPDIF npcm & spdif delay
      //#define HDMI_DMA_CTRL           DSPDMA6_DMA_CTRL        // SPDIF npcm
        #define PCM_CAPTURE_DMA_CTRL    DSPDMA1_DMA_CTRL
//        #define VOIO_DMA_CTRL           DSPDMA1_DMA_CTRL
//        #define GETUSB_DMA_CTRL         DSPDMA1_DMA_CTRL
        #define SW_DMARDR_DMA_CTRL      DSPDMA7_DMA_CTRL
        #define CH5_IN_DLY_DMA_CTRL     DSPDMA1_DMA_CTRL

        #define ADEC_DMA1_CTRL          DSPDMA3_DMA_CTRL
        #define ADEC_DMA2_CTRL          DSPDMA4_DMA_CTRL
        #define ADEC_DMA3_CTRL          DSPDMA3_DMA_CTRL
        #define ADEC_DMA4_CTRL          DSPDMA4_DMA_CTRL
        #define ADEC_DMA5_CTRL          DSPDMA5_DMA_CTRL

/************************************************
*   Below is DSP FIFO/DDR unit Setting
*************************************************/
        .const SE_PROCESS_FIFO_SIZE_UNIT            =   64;         //delay fifo size per channel
        .const SE_PROCESS_DMA_WORD_SIZE_UNIT        =   16;         //DMA_WORD_SIZE per channel

        /* input ISR PCM samples -> DDR1 unit setting */
        .const SE_BUFF2_FIFO_SIZE                   =   (SE_PROCESS_FIFO_SIZE_UNIT * SE_PROCESS_FETCH_CHANNELS);
        .const SE_BUFF2_DMA_WORD_SIZE               =   (SE_PROCESS_DMA_WORD_SIZE_UNIT * SE_PROCESS_FETCH_CHANNELS);    // (DMA_WORD_SIZE per channel) * channel number
        .const SE_BUFF2_DMA_24BIT_LINE_SIZE         =   SE_BUFF2_DMA_WORD_SIZE*3/BYTES_IN_MIU_LINE;
        .const SE_BUFF2_DMA_16BIT_LINE_SIZE         =   SE_BUFF2_DMA_WORD_SIZE*2/BYTES_IN_MIU_LINE;

        /* DDR2 --> output ISR PCM samples */
        .const SE_BUFF1_FIFO_SIZE                   =   (SE_PROCESS_FIFO_SIZE_UNIT * SE_PROCESS_STORE_CHANNELS);
        .const SE_BUFF1_DMA_WORD_SIZE               =   (SE_PROCESS_DMA_WORD_SIZE_UNIT * SE_PROCESS_STORE_CHANNELS);    // (DMA_WORD_SIZE per channel) * channel number
        .const SE_BUFF1_DMA_24BIT_LINE_SIZE         =   SE_BUFF1_DMA_WORD_SIZE*3/BYTES_IN_MIU_LINE;
        .const SE_BUFF1_DMA_16BIT_LINE_SIZE         =   SE_BUFF1_DMA_WORD_SIZE*2/BYTES_IN_MIU_LINE;

    /* share buffer in dm mapping */
        .const SE_PROCESS_BUFFER_MAIN               = 0x0;                                                                //share buffer in dm
        .const SE_PROCESS_BUFFER_MAIN_RAW1_LR       = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*0;
        .const SE_PROCESS_BUFFER_MAIN_RAW2_LR       = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*2;
        .const SE_PROCESS_BUFFER_MAIN_SE_LR         = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*4;
        .const SE_PROCESS_BUFFER_MAIN_SE_LmRm       = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*6;
        .const SURR_DLY_BUFFER                      = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*8;
        .const NR_PARAMETER_BUFFER                  = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*10;
        .const SE_PROCESS_BUFFER_MAIN_TMP           = SE_PROCESS_BUFFER_MAIN + SE_PROCESS_FRAME_SMP_UNIT*12;    //0x600

        #define DSP2DmAddr_system_shareBuff_base    0x0                 //overlay with decoder
        #define DSP2DmAddr_system_shareBuff_size    0x600               //overlay with decoder
        #define DSP2DmAddr_advSnd_shareBuff_base    SE_PROCESS_BUFFER_MAIN_TMP              //overlay with decoder
        #define DSP2DmAddr_advSnd_shareBuff_size    0x1000              //overlay with decoder, // min size: (SE_PROCESS_FRAME_SMP_UNIT*SE_PROCESS_FETCH_CHANNELS) for system frame base SE

        .const SndEff_Array_TMP1                    = SE_PROCESS_BUFFER_MAIN_TMP;
        .const SndEff_Array_TMP2                    = SndEff_Array_TMP1 + SE_PROCESS_FRAME_SMP_UNIT*2;
        .const SndEff_Array_TMP3                    = SndEff_Array_TMP2 + SE_PROCESS_FRAME_SMP_UNIT*2;
        .const SndEff_Array_TMP4                    = SndEff_Array_TMP3 + SE_PROCESS_FRAME_SMP_UNIT*2;

        .const Apply_NR_Status_BUFFER               = NR_PARAMETER_BUFFER;
        .const Apply_NR_Gain_BUFFER                 = Apply_NR_Status_BUFFER + SE_PROCESS_FRAME_SMP_UNIT;

    /* General delay buffer template */
        .const HP_DLY_MAX_DELAY             =      512;        //unit : ms
        .const DELAY_MAX_DELAY              =      512;
        .const DELAY_FIFO_SIZE              =      128;
        .const DELAY_DMA_WORDSIZE           =      32;
        .const DELAY_DMA_LINESIZE           =      DELAY_DMA_WORDSIZE*DMA24BIT_BYTES_IN_WORDS/BYTES_IN_MIU_LINE;

        /* Dly_status */
        .const DELAY_INPUT_STOP             =      0;
        .const DELAY_OUTPUT_STOP            =      1;
        .const DELAY_DLYIN_DMA_ASSERT       =      2;
        .const DELAY_DLYOUT_DMA_ASSERT      =      3;

    /* Mstar Surround delay buffer */
        .const SUR_FIFO_UNIT                 =      SE_PROCESS_FRAME_SMP_UNIT*2;
        .const SUR_DRAM_BURSRT               =     (SUR_FIFO_UNIT*3)/BYTES_IN_MIU_LINE;

    /* spdif delay unit setting */
        .const SPDIF_DLYBUF_DMA_WORDSIZE          = 32;
        .const SPDIF_DLYBUF_DMA_LINESIZE          = SPDIF_DLYBUF_DMA_WORDSIZE*2/BYTES_IN_MIU_LINE;
        .const SPDIF_DLYFIFO_LEN                  = 64;

    /* spdif nonPcm unit setting */
        .const SPDIF_NPCM_DMA_WORDSIZE            = 32;
        .const SPDIF_NPCM_DMA_LINESIZE            = SPDIF_NPCM_DMA_WORDSIZE*2/BYTES_IN_MIU_LINE;
        .const SPDIF_NPCMFIFO_LEN                 = 64;

    /* hdmi npcm fifo unit setting */
        .const HDMI_DLYBUF_DMA_WORDSIZE           = 128;
        .const HDMI_DLYBUF_DMA_LINESIZE           = HDMI_DLYBUF_DMA_WORDSIZE*2/BYTES_IN_MIU_LINE;
        .const HDMI_DLYFIFO_LEN                   = 128*4;


    /* Uni-player interface*/
        .const UNI_PCMBUF_READ_WORD_SIZE          = 64;
        .const UNI_PCMBUF_READ_LINE_SIZE          = UNI_PCMBUF_READ_WORD_SIZE*2/BYTES_IN_MIU_LINE;

    /* SW DMA */
        .const SW_DMA_CTRL_RESET_BIT              = 0;
        .const SW_DMA_CTRL_START_BIT              = 1;
        .const SW_DMA_CTRL_CIRCL_BIT              = 2;

        .const SW_DMA_WORDSIZE                    = 32;
        .const SW_DMA_LINESIZE                    = SW_DMA_WORDSIZE*2/BYTES_IN_MIU_LINE;

/********************************************************************
* DSP ISR mapping
********************************************************************/
    /* default 0 (no use) */
    #define ISR_MASK_AVSYNC             IMASK_PIO
    #define ISR_MASK_AVSYNC2            IMASK_PIO
    #define ISR_MASK_PCM                IMASK_SP1T_IRQ1
    #define ISR_MASK_PCM2               IMASK_SP1R_IRQ0
    #define ISR_MASK_ES                 IMASK_TM
    #define ISR_MASK_MHEG5              IMASK_PIO
    #define ISR_MASK_TIMER              IMASK_TM
    #define ISR_MASK_MM                 IMASK_IRQE0

    /* default -1 (no use) */
    #define ISR_PMASK_DEC3              4
    #define ISR_PMASK_PTS1              6
    #define ISR_PMASK_PTS2              -1
    #define ISR_PMASK_PCM               -1
    #define ISR_PMASK_PCM2              -1
    #define ISR_PMASK_ES                -1
    #define ISR_PMASK_ES2               -1
    #define ISR_PMASK_MHEG5             -1          //no need to do this, leave system handle

    #define DEC_MAIN_FUNC_PTR           g_DecFunPtr
    #define TIMER_FUNC_PTR              g_TimerIsr_funcPtr
    #define PTS1_FUNC_PTR               g_ParserC_isr_funcPtr
    #define PTS2_FUNC_PTR               g_ParserD_isr_funcPtr
    #define PCMOUT_FUNC_PTR             g_DEC3_isr_funcPtr
    #define SIF_PCMOUT_FUNC_PTR         g_IRQ0_isr_funcPtr
    #define MHEG5_FUNC_PTR              g_PIO_isr_MHEG5_funcPtr

/********************************************************************
* DSP internal mailbox mapping
********************************************************************/

    /* SPDIF NonPCM */
        #define D2S_MBOX_SPDIF_CTRL             ddco_spdifNpcmCtrl      // [0:2]=Acmod for AC3/AC3+/HE-AAC, [3]=LFE flag,
                                                                        // [12:15]: SPDIF sample rate
                                                                        // 0: 96K, 1: 88K, 2: 64K
                                                                        // 3: 48K, 4: 44K, 5: 32K
                                                                        // 6: 24K, 7: 22K, 8: 16K
                                                                        // 9: 12K, a: 11K, b:  8K
                                                                        // [18] info to DDCO: 1-> -4.75dB; 0-> do nothing
                                                                        // [19] : 1->48KHz, 0->non-48KHz
                                                                        // [20] MultiCH_EN, [21]=ADEC Stop/Play,[22]=Freeze,[23]=Start
        #define D2S_MBOX_SPDIF_WRPTR            ddco_spdifNpcmWrPtr
        #define S2D_MBOX_SPDIF_RDPTR            ddco_spdifNpcmRdPtr

        .const MBOX_SPDIF_NPCM_CLOCK_4X         = 17;
        .const MBOX_MULTI_CHANNEL_ENABLE_BIT    = 20;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_PLAY    = 21;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_FREEZE  = 22;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_START   = 23;

        #define NULL_PAYLOAD_TEST               1

        #if (NULL_PAYLOAD_TEST == 1)
            .const SPDIF_NPCM_MUTE_FRMCNT       =   0x8;
            .const SPDIF_PCM_MUTE_SMPCNT        =   0;
            .const SPDIF_NPCM_NULL_FRMCNT       =   0xC + SPDIF_NPCM_MUTE_FRMCNT;
        #else
            .const SPDIF_NPCM_MUTE_FRMCNT       =   0x10;
            .const SPDIF_PCM_MUTE_SMPCNT        =   0;
        #endif

    /* Mailbox with R2 */
        #define S2A_R2_PCMIN_WRPTR              SE2R2M_MAILBOX0
//        #define S2A_R2_KTV_CTRL                 SE2R2M_MAILBOX2

        #define A2S_R2_PCMOUT_WRPTR             R2M2SE_MAILBOX0
        #define A2S_R2_PCMUPLOAD_WRPTR          R2M2SE_MAILBOX3

        #define S2R_MBOX_DDCO_INFO_1            SE2R2M_MAILBOX3
        #define S2R_MBOX_DDCO_INFO_2            SE2R2M_MAILBOX4
        #define S2R_MBOX_PCM_RPTR               SE2R2M_MAILBOX5
/********************************************************************
* DSP io mapping
********************************************************************/
    #define NULL_IO                 0

    /* Multi-Channel from Decoder DSP */
        #define DSP_MAIN_L_IN                   DIG5L_I
        #define DSP_MAIN_R_IN                   DIG5R_I
        #define DSP_AD_L_IN                     DIG6L_I
        #define DSP_AD_R_IN                     DIG6R_I
        #define DSP_DMARDR_L_IN                 DIG8L_I
        #define DSP_DMARDR_R_IN                 DIG8R_I

        #define DSP_MULT_PCM_L_IN               DIG1L_I
        #define DSP_MULT_PCM_R_IN               DIG2L_I
        #define DSP_MULT_PCM_C_IN               DIG1R_I
        #define DSP_MULT_PCM_LS_IN              DIG2R_I
        #define DSP_MULT_PCM_RS_IN              DIG3L_I
        #define DSP_MULT_PCM_LFE_IN             DIG3R_I

    /* DSP common IO */
        #define DSPIO_DECODER_SEL          DECODER_FUNCTION
        #define DSPIO_SPDIF_IN_FREQ        STATUS_SPDIF_FREQ
        #define DSPIO_HDMI_IN_FREQ         STATUS_HDMI_FREQ
        #define DSPIO_HDMI_IN_PC           STATUS_HDMI_PC

    /* DSP Bounding IO */
        #define DSP_BOUND_OPTION            1
        #define DSPIO_BOUND_OPTION          0xA0FF
            .const BOUNDING_BIT_DD              = 0;                   //DD
            .const BOUNDING_BIT_DDP             = 1;                   //DD+
            .const BOUNDING_BIT_DPULSE          = 2;                   //Dolby Pulse (MS10 DDT) or DDCO
            .const BOUNDING_BIT_DVOL            = 3;                   //Dolby Volume
            .const BOUNDING_BIT_DTRUEHD         = 4;                   //Dolby TrueHD
            .const BOUNDING_BIT_DDDCO           = 5;                   //Dolby DDCO
            .const BOUNDING_BIT_DTSENVELO       = 6;                   //DTS Envelo / Symmetry
            .const BOUNDING_BIT_DTSDMP          = 7;                   //DTS DMP
            .const BOUNDING_BIT_DTSLBR          = 8;                   //DTS LBR
            .const BOUNDING_BIT_DTSTS           = 9;                   //DTS Transcoder
            .const BOUNDING_BIT_DTSCORELESS     = 10;                  //DTS Coreless
            .const BOUNDING_BIT_SRS             = 11;                  //SRS / Adv Sound
            .const BOUNDING_BIT_DOLBY           = 12;                  // when bit[12] = 0, all dolby ip's licenses open

        //#define AUTH_OPTION                   0x0FF2
            .const AUTH_BIT_DD                  = 0;
            .const AUTH_BIT_DDP                 = 1;
            .const AUTH_BIT_DDE                 = 2;
            .const AUTH_BIT_DTSDEC              = 3;
            .const AUTH_BIT_MS10DDT             = 4;
            .const AUTH_BIT_WMA                 = 5;
            .const AUTH_BIT_DRA                 = 6;
            .const AUTH_BIT_DTSLBR              = 7;
            .const AUTH_BIT_GAAC                = 8;
            .const AUTH_BIT_DEMOMODE            = 12;
            .const AUTH_BIT_COOK                = 16;

    /* PCM output port */
        #define DSP_MAIN_DMX_L_OUT              DEC3_PCM1_OUT
        #define DSP_MAIN_DMX_R_OUT              DEC3_PCM2_OUT
        #define DSP_ASOC_DMX_L_OUT              DEC3_PCM11_OUT
        #define DSP_ASOC_DMX_R_OUT              DEC3_PCM12_OUT
        #define DSP_ASOC_DMX_L_IN               NULL_IO                          //not in used now.
        #define DSP_ASOC_DMX_R_IN               NULL_IO                          //not in used now.
        #define SIF_DSP_MAIN_DMX_L_OUT          DEC4_PCM1_OUT
        #define SIF_DSP_MAIN_DMX_R_OUT          DEC4_PCM2_OUT

        #define DSP_MULT_PCM_L_OUT              DEC3_PCM1_OUT
        #define DSP_MULT_PCM_C_OUT              DEC3_PCM2_OUT
        #define DSP_MULT_PCM_R_OUT              DEC3_PCM3_OUT
        #define DSP_MULT_PCM_LS_OUT             DEC3_PCM4_OUT
        #define DSP_MULT_PCM_RS_OUT             DEC3_PCM5_OUT
        #define DSP_MULT_PCM_LFE_OUT            DEC3_PCM6_OUT

        #define DSP_SW_DMA_PCM_L_OUT            DEC4_PCM1_OUT
        #define DSP_SW_DMA_PCM_R_OUT            DEC4_PCM2_OUT

        #define DSP_PCM_SYNTH_L                 DVB3_FIX_SYNTH_NF_L
        #define DSP_PCM_SYNTH_H                 DVB3_FIX_SYNTH_NF_H

/************************************************
*   Below is macro for DSP code only
*************************************************/
        #define DSP_DMA_CHECK

        #define INC_WHILE_ONE_CNTR              ar = dm(S2M_MBOX_WHILE1_CNTR); \
                                                ar = ar + 0x000100; \
                                                dm(S2M_MBOX_WHILE1_CNTR) = ar;

        #define CONFIG_PCM_OUTPUT_PORT          ar = 0;   \
                                                dm (DEC_OUT_SEL) = ar;

        #define TRIGGER_INT_TO_MCU              ar = 0x0000; IO(PDATA) = ar;   \
                                                nop; nop; nop; nop;   \
                                                nop; nop; nop; nop;   \
                                                ar = 0x8000; IO(PDATA) = ar;   \
                                                nop; nop; nop; nop;   \
                                                nop; nop; nop; nop;   \
                                                ar = 0x0000; IO(PDATA) = ar

        /* Saft jump to i0 ~ i7 x:address, y:i0 ~ i7 */
        #define I_REGISTER_JUMP(x,y)            sr = lshift x by -16(lo);   \
                                                y = x;    \
                                                CPR = sr0;  \
                                                jump (y);

        /* Saft call to i0 ~ i7 x:address, y:i0 ~ i7 */
        #define I_REGISTER_CALL(x,y)            sr = lshift x by -16(lo);   \
                                                y = x;    \
                                                CPR = sr0;  \
                                                call (y);

#endif //_COMPILE_DSP_

    /************************************************
    *   For Compile Pass  mailbox (Need to remove later)
    ************************************************/
        #define D2M_MBOX_INTR_CMDTYPE           MB_2DB2
        #define DSP1PmAddr_ipSecurity           0x0FF2
        #define D2M_MBOX_ENC_LINEADDR           MB_2DAC                 //MPEG Encoder
        #define D2M_MBOX_ENC_LINESIZE           MB_2DAE                 //MPEG Encoder
        #define M2D_MBOX_PIO_ID                 MB_2D8A
        #define DSP1PmAddr_smpRate              0x0FF7
        #define MBOX_DBGCMD_RELOAD_DTV1_BEG     0xF000
        #define MBOX_DBGCMD_RELOAD_DTV1_END     0xF100
        #define MBOX_DBGCMD_RELOAD_DTV2_BEG     0xF600
        #define MBOX_DBGCMD_RELOAD_DTV2_END     0xF700
        #define DSP1DmAddr_sys_IoInfo           NULL
        #define M2D_MBOX_MM_FILEIN_TAG          MB_2D8C
        #define MBOX_DSP_INIT_ACK               0xE300
        #define D2M_MBOX_DBG_RESULT2            MB_2DBE
        #define D2M_MBOX_DBG_RESULT1            MB_2DBC
        #define M2D_MBOX_DEC_CTRL               MB_2D86
        #define M2D_MBOX_DBG_CMD1               MB_2D9C

        #define D2M_MBOX_SAMPLERATE             MB_2DA6
        #define DSP1DmAddr_dec1_omx_param       0x42B4
        #define M2D_MBOX_UNI_PCM3_WRPTR         MB_2D94
        #define DSP1PmAddr_video_TD             0x0FF1
        #define D2M_MBOX_UNI_PCM_BUFFEBT        MB_2D6A
        #define MPG_AV_DEFAULT_DELAY            0x3E1E
        #define AC3_AV_DEFAULT_DELAY            0x4300
        #define AAC_AV_DEFAULT_DELAY            0x327E
        #define DELAY_UPPER_LIMIT               0x2BF20
        #define DELAY_LOWER_LIMIT               0xA8C

#endif  //_AUDIO_COMM2_H_
