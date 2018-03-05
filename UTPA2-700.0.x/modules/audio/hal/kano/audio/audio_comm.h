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
#ifndef _AUDIO_COMM_H_
#define _AUDIO_COMM_H_
#include "audio_mbox.h"

/************************************************
*  請保持 Utopia 和 DSP 裡的這個檔案一致
*
*  1. 註解不要用 !
*  2. 宣告不要用 .const xxxx = ????;
************************************************/

/*********************************************************
*   system define
*********************************************************/
    #define DEC_AVSYNC_FIX_SYNTHRATE            0               //always fix synthesizer
    #define SYS_DEC1_MUTIL_CH_OUTPUT            1               //enable multi-channel output
    #define SYS_DEC_OUTPUT_24BIT                0
    #define SYS_DEC_UNI_PLAYER                  1
    #define DDP_HDMI_BYPASS_EN                  1
    #define SYS_DEC_AD_VOLUME_FOR_MAIN          0
    #define SYS_DEC_2ND_DECODER                 1
    #define SYS_DEC_RUN_WITH_SIF                0
    #define HW_ES2_BUG                          0
    #define DDT_DDENC_PATH                      2               // For MS10_DDT_Dual, DDEnc is in : DSP1(1), DSP2(2), R2M(3), Invalid (0xFF)
    #define SYS_DEC_SEC_PVR_MODE                0                // For SEC PVR mode
    #define SYS_DEC_33BIT_IN_MMTS_FILE          0                // For SEC PVR mode, also need to enable this function
    #define SYS_DEC2_USE_DEC1_SYNTH             0

/************************************************
*   DSP ddr address mapping
*************************************************/
    /* ES1 */
        #define DSP1_ES1_DRAM_BASE                     0x000000
        #define DSP1_ES1_DRAM_SIZE                     0x1FFF           // 64KB

    /* ES2 */
        #define DSP1_ES2_DRAM_BASE                     0x002000
        #define DSP1_ES2_DRAM_SIZE                     0xEFF           // 30KB

    /* DM Prefetch */
        #define DSP1_DM_PREFETCH_DRAM_BASE             0x0A600
        #define DSP1_DM_PREFETCH_DRAM_SIZE             0x7FFF          // 256KB

    /* DM Prefetch2 */
        #define DSP1_DM_PREFETCH2_DRAM_BASE            0x12600
        #define DSP1_DM_PREFETCH2_DRAM_SIZE            0x7FFF          // 256KB

    /* SPDIF Non-PCM */
        #define DSP1_SPDIF_DRAM_BASE                   0x3E000
        #define DSP1_SPDIF_DRAM_SIZE                   0x38FF          // 312KB

    /* PCM1 */
        #define DSP1_PCM1_DRAM_BASE                    0x01B000
        #define DSP1_PCM1_DRAM_SIZE                    0x6FFF          // 312KB

    /* PCM2 */
        #define DSP1_PCM2_DRAM_BASE                    0x022000
        #define DSP1_PCM2_DRAM_SIZE                    0x6FFF          // 112KB

    /* PCM1 Multi Channel */
        #define DSP1_MULTI_CHANNEL_PCM_BASE            0x29000
        #define DSP1_MULTI_CHANNEL_PCM_SIZE            0x14FFF         // 672KB

    /* AAC Metadata */
        #define DSP1_HEAAC_METADATA_DRAM_BASE          0x01A800
        #define DSP1_HEAAC_METADATA_DRAM_SIZE          0x3FF           // 8KB

    /* Others */
        #define DSP1_HDMI_DDR_BASE                     0x41900
        #define DSP1_HDMI_DDR_SIZE                     0xE3FF

        #define DSP1_UNI_DEC1_PCM3_DRAM_BASE           0               // Uniplayer/Mstreamer map
        #define DSP1_UNI_DEC1_PCM3_DRAM_SIZE           0               // 192KB (Uniplayer/Mstreamer)
        #define DSP1_UNI_DEC1_PCM3_DRAM_BASE_EXT_MASK  0

/********************************************************************
* DSP sram address mapping
********************************************************************/
    /* DSP SRAM Segment */
        /* CM */
        #define DSP1_CM_MAIN_ADDR           0x0
        #define DSP1_CM_MAIN_SIZE           0x0600
        #define DSP1_CM_CODE_ADDR           0x0600    /* Arch.sys, for dual decoder (DDC, DDT)*/
        #define DSP1_CM_CODE_SIZE           0x09E0
          #define DSP1_CM_CODE1_ADDR        0x0600    /* Arch_AD.sys, for two decoder (MPEG, MPEG_AD) */
          #define DSP1_CM_CODE1_SIZE        0x05F0
          #define DSP1_CM_CODE2_ADDR        0x0BF0
          #define DSP1_CM_CODE2_SIZE        0x03F0

        /* PM */
        #define DSP1_PM_MAIN_ADDR           0x0FF0
        #define DSP1_PM_MAIN_SIZE           0x0008
        #define DSP1_PM_SEG_ADDR            0x0FFF
        #define DSP1_PM_SEG_SIZE            0x2801
          #define DSP1_PM_SEG1_ADDR         0x0FFF
          #define DSP1_PM_SEG1_SIZE         0x1400
          #define DSP1_PM_SEG2_ADDR         0x23FF
          #define DSP1_PM_SEG2_SIZE         0x1401

        /* Prefetch */
        #define DSP1_PM_PREFETCH1_DSPADDR   0x12600
        #define DSP1_PM_PREFETCH1_DDRADDR   DSP1_PM_PREFETCH1_DSPADDR*3/BYTES_IN_MIU_LINE
        #define DSP1_PM_PREFETCH1_DSPSIZE   0x9000
        #define DSP1_PM_PREFETCH1_DDRSIZE   DSP1_PM_PREFETCH1_DSPSIZE*3/BYTES_IN_MIU_LINE

        #define DSP1_PM_PREFETCH2_DSPADDR   0x16E00
        #define DSP1_PM_PREFETCH2_DDRADDR   DSP1_PM_PREFETCH2_DSPADDR*3/BYTES_IN_MIU_LINE

        /* DM */
        #define DSP1_DM_MAIN_ADDR           0x4200
        #define DSP1_DM_MAIN_SIZE           0x0600
        #define DSP1_DM_SEG_ADDR            0x0000
        #define DSP1_DM_SEG_SIZE            0x4200
          #define DSP1_DM_SEG1_ADDR         0x0000
          #define DSP1_DM_SEG1_SIZE         0x2000
          #define DSP1_DM_SEG2_ADDR         0x2000
          #define DSP1_DM_SEG2_SIZE         0x2200

    /* DEC DSP PM vars */
        /* common */
            #define DSP1PmAddr_mainVer              0x0FE8
            #define DSP1PmAddr_alg1Ver              0x0FFF
            #define DSP1PmAddr_alg2Ver              0x23FF

            #define DSP1PmAddr_video_Td_L           0x0FF1
            #define DSP1PmAddr_ipSecurity           0x0FF2
            #define DSP1PmAddr_audio_PTS_H          0x0FF3
            #define DSP1PmAddr_audio_PTS_L          0x0FF4
            #define DSP1PmAddr_audio_Td_H           0x0FF5
            #define DSP1PmAddr_audio_Td_L           0x0FF6
            #define DSP1PmAddr_smpRate              0x0FF7
            #define DSP1PmAddr_soundMode            0x0FF8
            #define DSP1PmAddr_reqMHEG5_size        0x0FF9
            #define DSP1PmAddr_decErrorCnt          0x0FFA
            #define DSP1PmAddr_STest_Report         0x0FFB
            #define DSP1PmAddr_mpg_avDelay          0x0FFC
            #define DSP1PmAddr_ac3_avDelay          0x0FFD
            #define DSP1PmAddr_aac_avDelay          0x0FFE
            #define DSP1PmAddr_es1_blockLevel       0x0FE9      !unit: bytes
            #define DSP1PmAddr_pcm_blockLevel       0x0FEA      !unit: bytes

            #define DSP1PmAddr_audio2_sysInfo       0x478
            #define DSP1PmAddr_audio2_PTS_H         DSP1PmAddr_audio2_sysInfo+0      //audio_dec2_sysInfo[0]
            #define DSP1PmAddr_audio2_PTS_L         DSP1PmAddr_audio2_sysInfo+1      //audio_dec2_sysInfo[1]
            #define DSP1PmAddr_audio2_Td_H          DSP1PmAddr_audio2_sysInfo+2      //audio_dec2_sysInfo[2]
            #define DSP1PmAddr_audio2_Td_L          DSP1PmAddr_audio2_sysInfo+3      //audio_dec2_sysInfo[3]

        /* mpeg & mp3 */

        /* aac */

        /* ac3 */

        /* dts */
            #define DSP1PmAddr_kernelVer            0x1000

    /* DEC DSP DM vars */

        /* system info memory base */
            #define DSP1DmAddr_sys_AvSync           DSP1_DM_MAIN_ADDR
            #define DSP1DmSize_sys_AvSync           23
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


            #define DSP1DmAddr_sys_DecStatus        (DSP1DmAddr_sys_AvSync+DSP1DmSize_sys_AvSync)
            #define DSP1DmSize_sys_DecStatus        24
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

            #define DSP1DmAddr_sys_IoInfo           (DSP1DmAddr_sys_DecStatus+DSP1DmSize_sys_DecStatus)
            #define DSP1DmSize_sys_IoInfo           4
                #define SYS_IOINFO_DEC1_OUT_L       0
                #define SYS_IOINFO_DEC1_OUT_R       1
                #define SYS_IOINFO_DEC2_OUT_L       2
                #define SYS_IOINFO_DEC2_OUT_R       3

            #define DSP1DmAddr_sys_Working          (DSP1DmAddr_sys_IoInfo+DSP1DmSize_sys_IoInfo)
                #define SYS_FILE_PTS1_H             0
                #define SYS_FILE_PTS1_L             1

        /* common */
            #define DSP1DmAddr_dec1_param           0x47A0
            #define DSP1DmAddr_dec1_info            0x47C0
            #define DSP1DmAddr_dec1_inMbox          0x4780
            #define DSP1DmAddr_dec1_outMbox         0x47D0
            #define DSP1DmAddr_dec1_omx_param       0x42B4

        /* mpg */
            #define DSP1DmAddr_mpegInfo             DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_mpegFrmNum_internal  DSP1DmAddr_dec1_info+1
            #define DSP1DmAddr_mpegVer              DSP1DmAddr_dec1_info+2
            #define DSP1DmAddr_mpg_frmNum           DSP1DmAddr_dec1_info+3

        /* ac3+ */
            #define DSP1DmAddr_ac3p_bsMod           DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_ac3p_frmSz           DSP1DmAddr_dec1_info+1
            #define DSP1DmAddr_ac3p_bsid            DSP1DmAddr_dec1_info+2
            #define DSP1DmAddr_ac3p_fscod           DSP1DmAddr_dec1_info+3
            #define DSP1DmAddr_ac3p_find_syncword   DSP1DmAddr_dec1_info+4

        /* dts */
            #define DSP1DmAddr_dts_cdMode           DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_dts_amode            DSP1DmAddr_dec1_info+1
            #define DSP1DmAddr_dts_BitRate          DSP1DmAddr_dec1_info+2
            #define DSP1DmAddr_dts_smpRate          DSP1DmAddr_dec1_info+3

        /* wma pro */
            #define DSP1DmAddr_wma_smpRate          DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_wma_BitRate          DSP1DmAddr_dec1_info+1

        /* flac */
            #define DSP1DmAddr_flac_dmaPcmTotalSz   DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_flac_dmaPcmRemainSz  DSP1DmAddr_dec1_info+1
            #define DSP1DmAddr_flac_dmaPcmSramAddr  DSP1DmAddr_dec1_info+2

        /* dtslbr */
            #define DSP1DmAddr_dtslbr_inChannels    DSP1DmAddr_dec1_info+0
            #define DSP1DmAddr_dtslbr_BitRate       DSP1DmAddr_dec1_info+2
            #define DSP1DmAddr_dtslbr_smpRate       DSP1DmAddr_dec1_info+3

/********************************************************************
*  Decoder default setting
********************************************************************/
    /* default AVsync delay */
        #define MPG_AV_DEFAULT_DELAY                0x3E1E
        #define AC3_AV_DEFAULT_DELAY                0x4300
        #define AAC_AV_DEFAULT_DELAY                0x327E
        #define DELAY_UPPER_LIMIT                   0x6978
        #define DELAY_LOWER_LIMIT                   0xA8C
            #define AAC_AV_DELAY_LC_OFFSET          (90*30)
            #define AAC_AV_DELAY_SBR_OFFSET         -(90*20)

/************************************************
*   Below is MailBox config
*************************************************/

    /************************************************
    *   MCU to DSP mailbox
    ************************************************/
        /* MM */
        #define M2D_MBOX_AAC_SINGLE_FRAME_MODE  MB_2D92
        #define M2D_MBOX_MM_FILEIN_TAG          MB_2D8C

        #define M2D_MBOX_MISC_SETTING           MB_2D8E
            #define MBOX_MMTS_SYNCSTC_BIT           MBOX_BIT4
            #define MBOX_DTV_SYNC_VIDEO             MBOX_BIT10

        #define M2D_MBOX_DEC_CTRL               MB_2D86
            #define M2D_MBOX_MINUS_11_DB_BIT        MBOX_BIT1
            #define M2D_MBOX_MM_FF2X_BIT            MBOX_BIT5
            #define M2D_MBOX_DEC_MUTE_BIT           MBOX_BIT6
            #define M2D_MBOX_PLAY_WO_OUT            MBOX_BIT7
            #define M2D_MBOC_DDC_MINUS_6DB          MBOX_BIT8
            #define M2D_MBOX_PARSER1_ISO11172_1     MBOX_BIT10
            #define M2D_MBOX_PARSER1_BYPASS         MBOX_BIT11
            #define M2D_MBOX_PARSER2_BYPASS         MBOX_BIT12
            #define M2D_MBOX_ES1_LIMITER_EN         MBOX_BIT14
            #define M2D_MBOX_PCM_LIMITER_EN         MBOX_BIT15

        /* SPDIF & Dolby Parameter */
        #define M2D_MBOX_DRC_SETTING            MB_2D8E
            #define MBOX_DRC_BIT                    MBOX_BIT7
            #define MBOX_STDMX_BIT                  MBOX_BIT6
            #define MBOX_AUTO_STDMX_BIT             MBOX_BIT9
            #define MBOX_DOLBY11_3DB_ATTENUATOR		MBOX_BIT12
            #define MBOX_HDMI_NPCM_MODE             MBOX_BIT13
            #define MBOX_HDMI_DDP_BYPASS_BIT        MBOX_BIT14

        #define M2D_MBOX_SPDIF_SETTING              MB_2D8E
            #define MBOX_SPDIF_SETTING_BIT_MUTE             MBOX_BIT0
            #define MBOX_SPDIF_NPCM_MODE                    MBOX_BIT1
            #define MBOX_SPDIF_SETTING_R2_NPCM_SELBIT       MBOX_BIT2
            #define MBOX_SPDIF_SETTING_DVB2_NPCM_SELBIT     MBOX_BIT3

        #define M2D_MBOX_LOW_HIGH_CUT           MB_2D98
            #define MBOX_HIGHCUT_BSTART             MBOX_BITS_SHIFT-0
            #define MBOX_HIGHCUT_BMASK              0xF
            #define MBOX_LOWCUT_BSTART              MBOX_BITS_SHIFT-4
            #define MBOX_LOWCUT_BMASK               0xF

        /* MAIN Volume for AD */
        #define M2D_MBOX_MAIN_CONTROL           MB_2D48
            #define MBOX_MAIN_VOL_BSTART            MBOX_BITS_SHIFT-0
            #define MBOX_MAIN_VOL_BMASK             0x7FF

        /* AD */
        #define M2D_MBOX_AD_CONTROL             MB_2DD8
            #define MBOX_AD_ON_BIT                  MBOX_BIT15
            #define MBOX_AD_VOL_BSTART              MBOX_BITS_SHIFT-0
            #define MBOX_AD_VOL_BMASK               0x7FF

        /* decoder control */
        #define M2D_MBOX_DUALMOD_SEL            MB_2D86
            #define MBOX_DUALMOD_SEL_BSTART         MBOX_BITS_SHIFT-2
            #define MBOX_DUALMOD_SEL_BMASK          0x3

        /* MS10 DDC Command */
        #define M2D_MBOX_DDC_MODSEL             MB_2D80
            #define MBOX_DDC_MODSEL_BSTART          MBOX_BITS_SHIFT-0
            #define MBOX_DDC_MODSEL_BMASK           0x3
            #define MBOX_DDC_MODSEL_MULTIOUT_BSTART MBOX_BIT3
            #define MBOX_DDC_MODSEL_MULTIOUT_BMASK  0x7
            #define MBOX_DDC_OUTSEL_BSTART          MBOX_BITS_SHIFT-4
            #define MBOX_DDC_OUTSEL_BMASK           0x3
            #define MBOX_DDC_OUTSEL2_BSTART         MBOX_BITS_SHIFT-8
            #define MBOX_DDC_OUTSEL2_BMASK          0x3

        #define M2D_MBOX_DDC_ASOC_ID_SEL        MB_2D82
            #define MBOX_ASOC_IDSEL_BSTART          MBOX_BITS_SHIFT-0
            #define MBOX_ASOC_IDSEL_BMASK           0x3

        #define M2D_MBOX_PIO_ID                 MB_2D8A
        #define M2D_MBOX_UNI_PCMOUT_CNT         MB_2D84
        #define M2D_MBOX_UNI_INPUT_ES_WPTR      MB_2D88
        #define M2D_MBOX_UNI_NEED_DECODE_FRMCNT MB_2D90
        #define M2D_MBOX_UNI_PCM3_WRPTR         MB_2D94
        #define M2D_MBOX_UNI_SERVICE            MB_2D96
        #define M2D_MBOX_DTS_CH_CTRL            MB_2D96

        #define M2D_MBOX_POWERDOWN_CMD          MB_2D9A
            #define MBOX_K2_CHIP_U01                MBOX_BIT0
            #define MBOX_POWERDOWN_BIT              MBOX_BIT4

        #define M2D_MBOX_DBG_CMD1               MB_2D9C
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

            #define MBOX_DBGCMD_MIP_INT                 0xE000
            #define MBOX_DBGCMD_ENC_INT                 0xE100
            #define MBOX_DBGCMD_FILE_PTS_INT            0xEA00

            #define MBOX_DBGCMD_RELOAD_DTV1_BEG         0xF000
            #define MBOX_DBGCMD_RELOAD_DTV1_END         0xF100
            #define MBOX_DBGCMD_WAIT_MCU_START          0xF300
            #define MBOX_DBGCMD_RELOAD_DTV2_BEG         0xF600
            #define MBOX_DBGCMD_RELOAD_DTV2_END         0xF700

        #define M2D_MBOX_DBG_CMD2               MB_2D9E

        #define M2D_MBOX_AD_PLAYCMD             MB_2D2C

        #define M2D_MBOX_COMMON_0               DSP1DmAddr_dec1_inMbox+0x0
        #define M2D_MBOX_COMMON_1               DSP1DmAddr_dec1_inMbox+0x1
        #define M2D_MBOX_COMMON_2               DSP1DmAddr_dec1_inMbox+0x2
        #define M2D_MBOX_COMMON_3               DSP1DmAddr_dec1_inMbox+0x3
        #define M2D_MBOX_COMMON_4               DSP1DmAddr_dec1_inMbox+0x4
        #define M2D_MBOX_COMMON_5               DSP1DmAddr_dec1_inMbox+0x5
        #define M2D_MBOX_COMMON_6               DSP1DmAddr_dec1_inMbox+0x6
        #define M2D_MBOX_COMMON_7               DSP1DmAddr_dec1_inMbox+0x7
        #define M2D_MBOX_COMMON_8               DSP1DmAddr_dec1_inMbox+0x8
        #define M2D_MBOX_COMMON_9               DSP1DmAddr_dec1_inMbox+0x9
        #define M2D_MBOX_COMMON_A               DSP1DmAddr_dec1_inMbox+0xA
        #define M2D_MBOX_COMMON_B               DSP1DmAddr_dec1_inMbox+0xB
        #define M2D_MBOX_COMMON_C               DSP1DmAddr_dec1_inMbox+0xC
        #define M2D_MBOX_COMMON_D               DSP1DmAddr_dec1_inMbox+0xD
        #define M2D_MBOX_COMMON_E               DSP1DmAddr_dec1_inMbox+0xE
        #define M2D_MBOX_COMMON_F               DSP1DmAddr_dec1_inMbox+0xF

        #define M2D_MBOX_MM_DEC2_FILEIN_TAG2    M2D_MBOX_COMMON_0
        #define M2D_MBOX_ENC_CONTROL            MB_2D88                 //MPEG Encoder

        #define M2S_MBOX_TIMER_SETTING              MB_2D56

    /************************************************
    *   DSP to MCU mailbox
    ************************************************/
        #define D2M_MBOX_WHILE1_CNTR            MB_2DB8     //[15:8] Always in High Byte
        #define D2M_MBOX_TIMER_CNTR             MB_2DB8     //[ 7:0] Always in Low Byte
        #define D2M_MBOX_DEC_DECSTATUS          MB_2DBA     //[15:8] Always in High Byte
            #define DEC_DECSTATUS_DECVALID_BIT         MBOX_BIT8

        #define D2M_MBOX_HDMI_NPCM_LOCK         MB_2DB4     //[ 3:0] Always in LSB Nibble

        #define D2M_MBOX_ES_MEMCNT              MB_2D60
        #define D2M_MBOX_PCM_MEMCNT             MB_2D62
        #define D2M_MBOX_MM_BROWSE_TIME         MB_2D64
        #define D2M_MBOX_MM_PTS_IN_SEC          MB_2D66
        #define D2M_MBOX_MM_PTS_IN_MSEC         MB_2D68
        #define D2M_MBOX_MM_PTS_HI              MB_2D6A
        #define D2M_MBOX_MM_PTS_ME              MB_2D6C
        #define D2M_MBOX_MM_PTS_LO              MB_2D6E

        #define D2M_MBOX_ES2_MEMCNT             D2M_MBOX_COMMON_0
        #define D2M_MBOX_PCM2_MEMCNT            D2M_MBOX_COMMON_1
        #define D2M_MBOX_MM_DEC2_BROWSE_TIME    D2M_MBOX_COMMON_2
        #define D2M_MBOX_MM_DEC2_PTS_IN_SEC     D2M_MBOX_COMMON_3
        #define D2M_MBOX_MM_DEC2_PTS_IN_MSEC    D2M_MBOX_COMMON_4
        #define D2M_MBOX_MM_DEC2_PTS_HI         D2M_MBOX_COMMON_5
        #define D2M_MBOX_MM_DEC2_PTS_ME         D2M_MBOX_COMMON_6
        #define D2M_MBOX_MM_DEC2_PTS_LO         D2M_MBOX_COMMON_7

        #define D2M_MBOX_DEC2_STATUS            MB_2DB4

        #define D2M_MBOX_MM_FILE_REQ_ADDR       MB_2D6A
        #define D2M_MBOX_MM_FILE_REQ_SIZE       MB_2D6C
        #define D2M_MBOX_MM_FILE_PLAY_END       MB_2D6E
        #define D2M_MBOX_INTR_CMDTYPE           MB_2DB2
            #define DSP_INTR_CMD_MMES_NEED_DATA         0x0003
            #define DSP_INTR_CMD_REPORT_PTS             0x0005
            #define DSP_INTR_CMD_MMUNI_NEED_DATA        0x0006
            #define DSP_INTR_CMD_MMES2_NEED_DATA        0x0007
            #define DSP_INTR_CMD_VOIP                   0x0009
            #define DSP_INTR_CMD_PERFORMANCE_TEST   0x00FF

        #define D2M_MBOX_VOIP_IRQ_CNT           MB_2DA4

        #define D2M_MBOX_SAMPLERATE             MB_2DA6                 //[1:0] sampleRate index
            #define DDP_HDMI_TX_4X_BIT                  MBOX_BIT4

        #define D2M_MBOX_DEC_INTENEL_USE1       MB_2DA8
        #define D2M_MBOX_DEC_INTENEL_USE2       MB_2DAA
        #define D2M_MBOX_DEC_INTENEL_USE3       MB_2DAC
        #define D2M_MBOX_DEC_INTENEL_USE4       MB_2DAE

        #define D2M_MBOX_DBG_RESULT1            MB_2DBC                 //
            #define MBOX_DSP_INIT_ACK                   0xE300

        #define D2M_MBOX_DBG_RESULT2            MB_2DBE                 //
            #define MBOX_DSP_RELOAD_ACK1                0x0033
            #define MBOX_DSP_RELOAD_ACK2                0x0077

        #define D2M_MBOX_ENC_LINEADDR           MB_2DAC                 //MPEG Encoder
        #define D2M_MBOX_ENC_LINESIZE           MB_2DAE                 //MPEG Encoder

        #define D2M_MBOX_UNI_PCM_WPTR           MB_2D66                 //Uni Player
        #define D2M_MBOX_UNI_PCM_SIZE           MB_2D68                 //Uni Player
        #define D2M_MBOX_UNI_PCM_BUFFEBT        MB_2D6A                 //Uni Player
        #define D2M_MBOX_UNI_FRAME_CNT          MB_2D6C                 //Uni Player
        #define D2M_MBOX_UNI_MUTECNT            MB_2D6E                 //Uni Player
        #define D2M_MBOX_UNI_REPORT_ES_MEMCNT   MB_2DA2                 //Uni Player

        #define D2M_MBOX_COMMON_0               DSP1DmAddr_dec1_outMbox+0x0
        #define D2M_MBOX_COMMON_1               DSP1DmAddr_dec1_outMbox+0x1
        #define D2M_MBOX_COMMON_2               DSP1DmAddr_dec1_outMbox+0x2
        #define D2M_MBOX_COMMON_3               DSP1DmAddr_dec1_outMbox+0x3
        #define D2M_MBOX_COMMON_4               DSP1DmAddr_dec1_outMbox+0x4
        #define D2M_MBOX_COMMON_5               DSP1DmAddr_dec1_outMbox+0x5
        #define D2M_MBOX_COMMON_6               DSP1DmAddr_dec1_outMbox+0x6
        #define D2M_MBOX_COMMON_7               DSP1DmAddr_dec1_outMbox+0x7
        //#define D2M_MBOX_COMMON_8               DSP1DmAddr_dec1_outMbox+0x8
        //#define D2M_MBOX_COMMON_9               DSP1DmAddr_dec1_outMbox+0x9
        //#define D2M_MBOX_COMMON_A               DSP1DmAddr_dec1_outMbox+0xA
        //#define D2M_MBOX_COMMON_B               DSP1DmAddr_dec1_outMbox+0xB
        //#define D2M_MBOX_COMMON_C               DSP1DmAddr_dec1_outMbox+0xC
        //#define D2M_MBOX_COMMON_D               DSP1DmAddr_dec1_outMbox+0xD
        //#define D2M_MBOX_COMMON_E               DSP1DmAddr_dec1_outMbox+0xE
        //#define D2M_MBOX_COMMON_F               DSP1DmAddr_dec1_outMbox+0xF

        #define D2M_MBOX_MM_DEC2_FILE_REQ_ADDR      D2M_MBOX_COMMON_0
        #define D2M_MBOX_MM_DEC2_FILE_REQ_SIZE      D2M_MBOX_COMMON_1

#ifdef _COMPILE_DSP_
/************************************************
*  DSP TSCALE & TCOUNT setting
************************************************/
#define TSCALE_CONSTANT             0xF9
#define TIME_CONSTANT_348M          0xAD
#define DSP_TIMER_SETTING           TIME_CONSTANT_348M

/************************************************
*  DSP Buffer Setting
************************************************/
    #define PTS1TAG_MEM                 pm
    #define PTS2TAG_MEM                 pm

/************************************************
*   Below is memory config
*************************************************/
    /* DRAM MAP config */
        #define DDR_DM_PREFETCH_BUF_ADDR    0x0A200
        #define DDR_DM_PREFETCH_BUF_SIZE    0x7FFF                                      // 256KB

        #define DDR_DM_PREFETCH2_BUF_ADDR   0x12200
        #define DDR_DM_PREFETCH2_BUF_SIZE   0x7FFF                                     // 256KB

        #define SPDIF_DRAM_BASE             0x3E000
        #define SPDIF_DRAM_SIZE             0x38FF                                      // 114KB

        #define HDMI_DDR_BASE               0x41900
        #define HDMI_DDR_SIZE               0xE3FF

        !#define ENC_OUT_BUF_ADDR            0x00AC80
        !#define ENC_OUT_BUF_SIZE            0x0D80                                      //  54KB

        !#define UNI_DEC1_PCM1_BUF_ADDR      0x025000                                    // Uniplayer/Mstreamer map
        !#define UNI_DEC1_PCM1_BUF_SIZE      0xA7FF                                      // 144KB (Uniplayer/Mstreamer)

        !#define UNI_DEC1_PCM3_BUF_ADDR      0x030000                                    // Uniplayer/Mstreamer map
        !#define UNI_DEC1_PCM3_BUF_SIZE      0x5FFF                                      // 192KB (Uniplayer/Mstreamer)

        #define DDR_METADATA_ADDR           0x01A800
        #define DDR_METADATA_SIZE           0x3FF                                       // 8KB

        #define DDR_MULTI_CHANNEL_PCM_BASE  0x29000
        #define DDR_MULTI_CHANNEL_PCM_SIZE  0x14FFF                                     // 672KB

/************************************************
*   Below is DMA config
*************************************************/
    #define DMAITF_DSPCMD_ALIGNMENT_BIT         7           // 1/0 : msb / lsb alignment
    #define DMAITF_DSPCMD_BYTESWAP_BIT          6           // set 1 to byte swap
    #define DMAITF_DSPCMD_READY_BIT             5           // set 1 to trigger, will be 0 when dma is finished
    #define DMAITF_DSPCMD_CLRCNTR_BIT           4           // set 1 to clear
    #define DMAITF_DSPCMD_PRIORITY_BIT          3           // 1/0 : high / low
    #define DMAITF_DSPCMD_24BITS_BIT            2           // 1/0 : 24bits / 16bits

    #define DMAITF_DSPCMD_ALIGNMENT_MASK        0x80
    #define DMAITF_DSPCMD_BYTESWAP_MASK         0x40
    #define DMAITF_DSPCMD_READY_MASK            0x20
    #define DMAITF_DSPCMD_CLRCNTR_MASK          0x10
    #define DMAITF_DSPCMD_PRIORITY_MASK         0x08
    #define DMAITF_DSPCMD_24BITS_MASK           0x04
    #define DMAITF_DSPCMD_BURST_6               0x03
    #define DMAITF_DSPCMD_BURST_3               0x02
    #define DMAITF_DSPCMD_BURST_2               0x01
    #define DMAITF_DSPCMD_BURST_1               0x00

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

        #define DMAITF_16BITS_B1_DMA_CMD            0xA8                //16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_DMA_CMD            0xA8                //no Burst 2 cmd, use B1 instead
        #define DMAITF_16BITS_B1_SWAP_DMA_CMD       0xE8                //16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_SWAP_DMA_CMD       0xE8                //no Burst 2 cmd, use B1 instead

        #define DMAITF_24BITS_B3_DMA_CMD            0x2E                //24 Bits Burst 3
        #define DMAITF_24BITS_B6_DMA_CMD            0x2E                //no Burst 6 cmd, use B3 instead

        #define DMAITF_16BITS_LowAlign_B1_DMA_CMD   0x28                //16 Bits Burst 1, low alignment
        #define DMAITF_16BITS_LowAlign_B1_SWAP_DMA_CMD   0x68                //16 Bits Burst 1, low alignment
    #else
        /* 1 MIU Line = 64bit (8 bytes) */
        #define DMAITF_DSPWORDS_IN_1_LINE           4
        #define DMAITF_DSPWORDS_IN_1LINE_LOG2       2
        #define DMAITF_DSPWORDS_IN_3LINE_LOG2       3
        #define DMAITF_WR_BIT                       16
        #define DMAITF_DM_BIT                       15
        #define DMAITF_BYTES_IN_1MIU_LINE           BYTES_IN_MIU_LINE

        #define DMAITF_RD_PM_MASK                   0x000000
        #define DMAITF_WR_PM_MASK                   0x010000
        #define DMAITF_RD_DM_MASK                   0x008000
        #define DMAITF_WR_DM_MASK                   0x018000

        #define DMAITF_16BITS_B1_DMA_CMD            0xA8                //16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_DMA_CMD            0xA9
        #define DMAITF_16BITS_B1_SWAP_DMA_CMD       0xE8                //16 Bits Burst 1, high alignment
        #define DMAITF_16BITS_B2_SWAP_DMA_CMD       0xE9

        #define DMAITF_24BITS_B3_DMA_CMD            0x2E                //24 Bits Burst 3
        #define DMAITF_24BITS_B6_DMA_CMD            0x2F                //24 Bits Burst 6

        #define DMAITF_16BITS_LowAlign_B1_DMA_CMD   0x28                //16 Bits Burst 1, low alignment
        #define DMAITF_16BITS_LowAlign_B1_SWAP_DMA_CMD   0x68                //16 Bits Burst 1, low alignment
    #endif

    /* DMA Mapping */
        #define ADEC_DMA1_CTRL           DSPDMA1_DMA_CTRL
        #define ADEC_DMA2_CTRL           DSPDMA2_DMA_CTRL
        #define ADEC_DMA3_CTRL           DSPDMA3_DMA_CTRL
        #define ADEC_DMA4_CTRL           DSPDMA4_DMA_CTRL
        #define ADEC_DMA5_CTRL           DSPDMA5_DMA_CTRL
        #define ADEC_DMA6_CTRL           DSPDMA6_DMA_CTRL

    #define UNI_PCMBUF_READ_WORD_SIZE           64
    #define UNI_PCMBUF_READ_LINE_SIZE           UNI_PCMBUF_READ_WORD_SIZE*2/BYTES_IN_MIU_LINE

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
    #define ISR_PMASK_PTS1              6
    #define ISR_PMASK_PTS2              5
    #define ISR_PMASK_PCM               -1
    #define ISR_PMASK_PCM2              -1
    #define ISR_PMASK_ES                -1
    #define ISR_PMASK_ES2               -1
    #define ISR_PMASK_MHEG5             -1          //no need to do this, leave system handle

    #define DEC_MAIN_FUNC_PTR           _DecFunPtr
    #define DEC2_FUNC_PTR               _DecFunPtr2
    #define TIMER_FUNC_PTR              TimerIsr_funcPtr
    #define PTS1_FUNC_PTR               ParserA_isr_funcPtr
    #define PTS2_FUNC_PTR               ParserB_isr_funcPtr
    #define PCMOUT_FUNC_PTR             IRQ1_isr_funcPtr
    #define PCMOUT2_FUNC_PTR            IRQ0_isr_funcPtr
    #define MHEG5_FUNC_PTR              PIO_isr_MHEG5_funcPtr

/********************************************************************
* DSP io mapping
********************************************************************/
    #define NULL_IO                 0

    /* DSP common IO */
        #define DSPIO_DECODER_SEL          DECODER_FUNCTION
        #define DSPIO_SPDIF_IN_FREQ        STATUS_SPDIF_IN_FREQ
        #define DSPIO_HDMI_IN_FREQ         STATUS_HDMI_FREQ

    /* DSP Bounding IO */
        #define DSP_BOUND_OPTION                1
        #define DSPIO_BOUND_OPTION              0xA0FF
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

    /* IP AUTH & OTP BONDING */
        #define D2S_MBOX_IP_AUTH                DEC2SE_MAILBOX7

    /* SPDIF NonPCM */
        #define D2S_MBOX_SPDIF_CTRL             DEC2SE_MAILBOX0
        #define D2S_MBOX_SPDIF_WRPTR            DEC2SE_MAILBOX1
        #define D2S_MBOX_SPDIF_HEAAC_METADATA   DEC2SE_MAILBOX3
        #define S2D_MBOX_SPDIF_RDPTR            SE2DEC_MAILBOX0

        #define D2M_MBOX_UNI_SPDIF_WRPTR        MB_2DA0
        #define M2D_MBOX_UNI_SPDIF_RDPTR        MB_2DD0
        #define D2M_MBOX_UNI_SPDIF_MEMCNT       MB_2DA2
        #define M2D_MBOX_UNI_TOTAL_PCM_SIZE     MB_2DD2

        #define D2M_MBOX_UNI_MULTI_PCM_WRPTR    MB_2DA4
        #define M2D_MBOX_UNI_MULTI_PCM_RDPTR    MB_2DDA

        .const MBOX_SPDIF_NPCM_CLOCK_4X         = 17;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_PLAY    = 21;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_FREEZE  = 22;
        .const MBOX_SPDIF_NPCM_CTRL_BIT_START   = 23;

    /* HDMI Tx NPCM config, only avalible for STB chip */
      #if 1
        #define D2S_MBOX_HDMI_WR_ADDR            DEC2SE_MAILBOX2
        #define S2D_MBOX_HDMI_RD_ADDR            SE2DEC_MAILBOX1
        #define S2D_MBOX_HDMI_NPCM_DELAY         SE2DEC_MAILBOX7
      #endif

    /* DVB2 Fade Gain from SE-DSP (MPEG/AAC) */
        #define S2D_MBOX_AD_FADE_GAIN            SE2DEC_MAILBOX6

    /* PCM output port */
        #define DSP_MAIN_DMX_L_OUT               DEC1_PCM11_OUT
        #define DSP_MAIN_DMX_R_OUT               DEC1_PCM12_OUT
        #define DSP_ASOC_DMX_L_OUT               DVB_OUT_LPCM
        #define DSP_ASOC_DMX_R_OUT               DVB_OUT_RPCM
        #define DSP_ASOC_DMX_L_IN                NULL_IO                          //not in used now.
        #define DSP_ASOC_DMX_R_IN                NULL_IO                          //not in used now.
        #define DSP_KTV_DMX_L_OUT                DEC1_PCM1_OUT
        #define DSP_KTV_DMX_R_OUT                DEC1_PCM2_OUT


        #define DSP_MULT_PCM_L_OUT              DEC1_PCM5_OUT
        #define DSP_MULT_PCM_R_OUT              DEC1_PCM6_OUT
        #define DSP_MULT_PCM_C_OUT              DEC1_PCM7_OUT
        #define DSP_MULT_PCM_LS_OUT             DEC1_PCM8_OUT
        #define DSP_MULT_PCM_RS_OUT             DEC1_PCM9_OUT
        #define DSP_MULT_PCM_LFE_OUT            DEC1_PCM10_OUT

        #define DSP_PCM_SYNTH_L                 Synthesizer_N_fL
        #define DSP_PCM_SYNTH_H                 Synthesizer_N_fH
        #define DSP_PCM2_SYNTH_L                DVB2_Synthesizer_N_fL
        #define DSP_PCM2_SYNTH_H                DVB2_Synthesizer_N_fH
        #define DSP_SPDIF_SYNTH_L               NULL_IO
        #define DSP_SPDIF_SYNTH_H               NULL_IO
        #define DSP_HDMI_SYNTH_L                NULL_IO
        #define DSP_HDMI_SYNTH_H                NULL_IO

    #ifdef SIMULATE_PATTERN
        #define DSP_DMA_CHECK                   call.l adec1_sim_dma;
    #else
        #define DSP_DMA_CHECK
    #endif

/************************************************
*   Below is macro for DSP code only
*************************************************/
        #define CLR_DECODER1_DM                   i0 = DSP1_DM_SEG1_ADDR; \
                                                cntr = DSP1_DM_SEG1_SIZE; \
                                                  si = 0; \
                                                  do @1f until ce; \
                                                  @1: dm(i0,m1) = si;

        #define CLR_DECODER2_DM                   i0 = DSP1_DM_SEG2_ADDR; \
                                                cntr = DSP1_DM_SEG2_SIZE; \
                                                  si = 0; \
                                                  do @1f until ce; \
                                                  @1: dm(i0,m1) = si;

        #define INC_WHILE_ONE_CNTR              ar = dm(D2M_MBOX_WHILE1_CNTR); \
                                                ar = ar + 0x100; \
                                                dm(D2M_MBOX_WHILE1_CNTR) = ar

        #define CONFIG_PCM_OUTPUT_PORT          ar = 0;   \
                                                dm (DVB_OUT_SEL_NF_CFG) = ar;

        #define TRIGGER_INT_TO_MCU              ar = 0x0000; IO(PDATA) = ar;   \
                                                nop; nop; nop; nop;   \
                                                nop; nop; nop; nop;   \
                                                ar = 0x8000; IO(PDATA) = ar;   \
                                                nop; nop; nop; nop;   \
                                                nop; nop; nop; nop;   \
                                                ar = 0x0000; IO(PDATA) = ar

        #define INC_DEBUGCNT(x)                 ar = dm(kh_debugCnt+x);  \
                                                ar = ar + 1;    \
                                                dm(kh_debugCnt+x) = ar

		//#define DBG_NEW_MM_MODE

#endif //_COMPILE_DSP_

#endif  //_AUDIO_COMM_H_
