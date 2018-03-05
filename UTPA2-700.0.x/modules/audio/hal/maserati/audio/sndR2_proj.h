#ifndef __PLATFORM_SNDR2_PROJ_INCLUDE_FILE__
#define __PLATFORM_SNDR2_PROJ_INCLUDE_FILE__

#include "ddr_config.h"
#include "mbox_sndR2.h"

#define SND_R2_CLOCK            432000000

#ifdef  __BUILD_FOR_R2__
/***********************************************************
*   Software Setting
***********************************************************/
    #define R2_SIMULATION                   0
    #define R2_BUILD_IN_TEST_PATTERN        0

    #define R2_DECODER_EN                   0
    #define PRE__SOUND_PROCESS_EN           1
    #define POST_SOUND_PROCESS_EN           1

    #define R2_DEC_HDMI_OUTPUT          0

#if (R2_SIMULATION == 1)
    #define R2_SIM_AUDIO_TYPE       5           // 1: AC3, 5: AAC
#endif
/***********************************************************
*   IP wrapper Setting
***********************************************************/
    #define SUPPORT_DDENC       1
    #define SUPPORT_DTSENC      1
    #define SUPPORT_DDPENC      1

/***********************************************************
*   Hardware Setting
***********************************************************/
    #define LOG2_BYTES_IN_LINE              4
    #define BYTE_IN_LINE                    (1<<LOG2_BYTES_IN_LINE)
    #define BYTE_IN_LINE_MASK               (BYTE_IN_LINE - 1)
    #define ALIGN_TO_MIU_LINE(x)            (x&BYTE_IN_LINE_MASK)?((x+BYTE_IN_LINE)&(~BYTE_IN_LINE_MASK)):x

    #define DC_WR_ALLOCATE_EN               1

	#define R2_High_Priority                0

/***********************************************************
*   Software Setting
***********************************************************/
    #define MAX_PRINTF_BUF                  0x8000
    #define R2_INTERRUPT_PRINTF

/***********************************************************
*   wrapper define for each IP
***********************************************************/
#define SND_R2_DQMEM_SIZE                   32      //unit is kbytes
#define SND_R2_DQMEM_ADDR                   0xFFFF0000

/***********************************************************
*   DDR Setting
***********************************************************/
    #ifdef __aeon__
    #define OFFSET_R2                0
    #else
    #define OFFSET_R2                (((unsigned int)&chip_audio_memory_buf)+ADEC__R2_DDR_SIZE)
    #endif

    #define OFFSET_R2_TO_ASNDDSP    (OFFSET_R2 + ASND__R2_DDR_SIZE)
    #define OFFSET_R2_TO_COMM_DDR   (OFFSET_R2 + ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE)

    /* DECODER DDR */
        //// ES & PCM ////
        #if 1
        #define ES1_DRAM_ADDR                   (OFFSET_R2_TO_ASNDDSP + OFFSET_ES1_DRAM_ADDR)
        #define ES2_DRAM_ADDR                   (OFFSET_R2_TO_ASNDDSP + OFFSET_ES2_DRAM_ADDR)
        #define ES3_DRAM_ADDR                   (OFFSET_R2_TO_ASNDDSP + OFFSET_ES3_DRAM_ADDR)
        #define ES4_DRAM_ADDR                   (OFFSET_R2_TO_ASNDDSP + OFFSET_ES4_DRAM_ADDR)
        #define PCM1_DRAM_ADDR                  (OFFSET_R2_TO_COMM_DDR + OFFSET_PCM1_DRAM_ADDR)
        #define PCM2_DRAM_ADDR                  (OFFSET_R2_TO_COMM_DDR + OFFSET_PCM2_DRAM_ADDR)
        #endif

        //// HDMI non-PCM ////
        #if (R2_DEC_HDMI_OUTPUT == 1)
        #define BUF_HDMI_OFFSET		            0x0250000
        #define BUF_HDMI_ADDR			        (OFFSET_R2_TO_COMM_DDR+ BUF_HDMI_OFFSET)
        #define BUF_HDMI_SIZE			        0xE4000
        #endif

        #if 0
        #define DSP1_ENC_RAW_OFFSET	            0x334000
        #define DSP1_ENC_RAW_BUFADDR		    (OFFSET_R2_TO_COMM_DDR + DSP1_ENC_RAW_OFFSET)
        #define DSP1_ENC_RAW_BUFSIZE 		    0x38E00
        #endif

    /* sound effect related DDR */
        #define DEC1_METADATA1_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA1_DRAM_ADDR)
        #define DEC1_METADATA2_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA2_DRAM_ADDR)
        #define DEC1_METADATA3_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA3_DRAM_ADDR)

        #define DEC2_METADATA1_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA1_DRAM_ADDR)
        #define DEC2_METADATA2_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA2_DRAM_ADDR)
        #define DEC2_METADATA3_BUF       (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA3_DRAM_ADDR)

        /* pre sound DDR */
        #define BUF_PCMIN_ADDR          (OFFSET_R2_TO_ASNDDSP + OFFSET_SE_MAIN_IN_DRAM_ADDR)
        #define BUF_PCMIN_SIZE           SE_MAIN_IN_DRAM_SIZE

        /* post sound DDR */
        #define BUF_PCMIN2_ADDR          (OFFSET_R2_TO_ASNDDSP + OFFSET_SE_MAIN_OUT_DRAM_ADDR)
        #define BUF_PCMIN2_SIZE          SE_MAIN_OUT_DRAM_SIZE

        #define BUF_SPDIF_OFFSET        OFFSET_SPDIF_NONPCM_DRAM_BASE
        #define BUF_SPDIF_ADDR          (OFFSET_R2_TO_COMM_DDR+ BUF_SPDIF_OFFSET)
        #define BUF_SPDIF_SIZE          SPDIF_NONPCM_DRAM_SIZE

        #define BUF_HDMI_OFFSET        OFFSET_HDMI_NONPCM_DRAM_BASE
        #define BUF_HDMI_ADDR          (OFFSET_R2_TO_COMM_DDR+ BUF_HDMI_OFFSET)
        #define BUF_HDMI_SIZE          HDMI_NONPCM_DRAM_SIZE

    /* ENCODER DDR */
        #define DDENC_METADATA_OFFSET    OFFSET_DDENC_METADATA_DRAM_ADDR
        #define DDENC_METADATA_ADDR      (OFFSET_R2_TO_ASNDDSP+ DDENC_METADATA_OFFSET)
        #define DDENC_METADATA_SIZE      DDENC_METADATA_DRAM_SIZE

    /* MISC */
        #define BUF_DDENC_MCH_ADDR      (OFFSET_R2_TO_ASNDDSP + OFFSET_SER2_DDENC_MCHOUT_DRAM_ADDR)
        #define BUF_DDENC_MCH_SIZE      SER2_DDENC_MCHOUT_DRAM_SIZE

        #define BUF_OUTPCM_DMX_ADDR      (OFFSET_R2_TO_ASNDDSP + OFFSET_SER2_OUTPCM_DMX_DRAM_ADDR)
        #define BUF_OUTPCM_DMX_SIZE      SER2_OUTPCM_DMX_DRAM_SIZE

        /* mpeg encoder */

        #define MPEG_ENC_BASE           (OFFSET_R2_TO_COMM_DDR+ OFFSET_MP3_ENC_DRAM_BASE)
        #define MPEG_ENC_SIZE           MP3_ENC_DRAM_SIZE


/***********************************************************
*   Decoder threshold settings
***********************************************************/
    #define R2_OUTPUT_ENC_RAW_DATA      0

    #define DEFAULT_ES_BLOCK_LEVEL                      0x1800
    #define DEFAULT_PCM_BLOCK_LEVEL                     0x20000
    #define DEFAULT_AVSYNC_OFFSET                       0x3138                  //AVSYNC delay in DTV: 140ms, Unit: 33bits
    #define DEFAULT_SPDIF_OFFSET                        0xC00                    //
    #define DEFAULT_STOP_PARSER_ES_LEVEL                0x8000
    #define DEFAULT_FILE_HAND_SHAKE_SIZE                0x1000

    #define DEC_OUTPUT_BYTES_PER_SAMPLE                 2
    #define DEC_OUTPUT_DMX_CHANNELS                     2
    #define DEC_OUTPUT_MCH_CHANNELS                     6                   //order : L / C / R / Ls / Rs / LFE

    #define AVSYNC_TD_TIMEOUT_THRESHOLD                 0x36EE8/2                                                // 2500ms         !?? Threshold?? STC ? APTS ????, ??? FreeRun
    #define AVSYNC_WAIT_STC_TD                          90*70/2                                                  // 70 ms          !?? Threshold?? STC ?? APTS ???, ??????STC
    #define AVSYNC_SKIP_FRM_TD                          90*45/2                                                  // 45 ms          !?? Threshold?? STC ?? APTS ???, ??skip frame??STC
    #define SYS_DEC_HDMIPLAY_PCM_LEVEL                  0 //(1536-64)*48  //0                              // 45 ms
                                                                             //
    #define SYS_DEC1_WAIT_1ST_PTS_TIMEOUT_PCM_LEVEL     700*48                              // 700ms  (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_WAIT_STC_TIMEOUT_PCM_LEVEL         700*48                              // 700ms  (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_FREEPLAY_PCM_LEVEL                 1000*48                                     // 1000ms (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_STOP_DECODE_PCM_LEVEL              (1024-64)*48                        //(1536-64)*DEC_OUTPUT_1MS_48K_SIZE      //(1024-64)*DEC_OUTPUT_1MS_48K_SIZE      // 1024ms (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_WAIT_STC_TIMEOUT_ES_LEVEL          0x8000
    #define SYS_DEC1_WAIT_1ST_PTS_TIMEOUT_ES_LEVEL      0x2000

    #define SYS_DEC_WAIT_1st_PTS_FRMCNT                 32
    #define SYS_DEC_MAX_PTS_TAG                         16

    #define SYS_DEC_DTV_FREEPLAY_PCM_LEVEL              64*48

/***********************************************************
*   Sound effect Setting
***********************************************************/
    #define ASND_PROCESS_MODE               2           // 2(adec_acmod_stereo) or 7(adec_acmod_mod32)
    #define ASND_PROCESS_ITF_UNIT           32
    #define BYTES_IN_PCM                    3

    /* pre sound process */
    #define ASND_PRE_PROCESS_BUF_UNIT       256
    #define ASND_PRE_INPUT_PCM_CH           12           //(RAW DMX + 5.1)
    #define ASND_PRE_SE_DMX_L_CH            8
    #define ASND_PRE_SE_DMX_R_CH            9

    #define DOWNMIX_2_CH_ORDER              0
    #define MULTI_5P1_CH_ORDER              2
    #define ENC_2_CH_ORDER                  8
    #define ECHO_CH_ORDER                   10

    /* post sound process */
    #define ASND_POST_PROCESS_BUF_UNIT      256
    #define ASND_POST_INPUT_PCM_CH          6
    #define ASND_POST_SE_DMX_L_CH           2
    #define ASND_POST_SE_DMX_R_CH           3

#endif  //__BUILD_FOR_R2__

/***********************************************************
*   Mbox Setting
***********************************************************/
//---------------------------------------
/******************* MCU <--> SND-R2 *******************/
    /* MCU -> SND-R2 */
        //// MCU -> SND-R2 : Common ////
        #define SNDR2_MBOX_WORD_MCUCMD_PARAMTYPE      MBOX_M2A_2E80   //generate interrut
        #define SNDR2_MBOX_BYTE_MCUCMD_ID_SEL         MBOX_M2A_2E82
        #define SNDR2_MBOX_BYTE_MCUCMD_Get_OR_Set     MBOX_M2A_2E82+1
        #define SNDR2_MBOX_WORD_MCUCMD_PARAM1         MBOX_M2A_2E84
        #define SNDR2_MBOX_WORD_MCUCMD_PARAM2         MBOX_M2A_2E86
        #define SNDR2_MBOX_WORD_R2_CTRL               MBOX_M2A_2E88
        #define SNDR2_MBOX_WORD_R2_DEBUG              MBOX_M2A_2E8A

        #define MBOX_WORD_DBG_DUMP_SE_DDR_ADDR_HI     MBOX_M2A_2E8C
        #define MBOX_WORD_DBG_DUMP_SE_DDR_ADDR_LO     MBOX_M2A_2E8E
        #define MBOX_WORD_DBG_DUMP_SE_DDR_SIZE        MBOX_M2A_2E9C

        #define SNDR2_MBOX_BYTE_STATUS_SELECT         MBOX_M2A_2E9E
        #define SNDR2_MBOX_BYTE_STATUS_SPEED          MBOX_M2A_2E9E+1

        //// MCU -> SND-R2 : R2 sound effect ////
        #define SNDR2_MBOX_BYTE_ECHO_SEL              MBOX_M2A_2E92
        #define SNDR2_MBOX_BYTE_ABS3D_SEL             MBOX_M2A_2E92
        #define SNDR2_MBOX_BYTE_LGSE_SEL              MBOX_M2A_2E92
        #define SNDR2_MBOX_BYTE_SRS_SEL               MBOX_M2A_2E94


        #define SNDR2_MBOX_BYTE_DAP_SEL               (MBOX_M2A_2E92+1)
            #define MS12_DAP_ENABLE_BIT            0
            #define PCMR_MD_INDEX_BIT              1
            //#define DAP_MCU_CTRL_BIT                _BIT_1_
            //#define MS12_DAP_ENABLE_BIT              4
            #define MS12_DDPE_ENABLE_BIT             5

        //// MCU -> SND-R2 : R2 encoder ////
        #define SNDR2_MBOX_BYTE_TRANSCODE_SEL              (MBOX_M2A_2E94+1)
            #define MP3_ENCODE_ENABLE_BIT           0
            #define MP3_ENCODE_MUTE_BIT             1
            #define DTS_ENCODE_MUTE_BIT             _BIT_6_
            #define DD_ENCODE_MUTE_BIT              _BIT_4_

        #define SNDR2_MBOX_BYTE_MCU_CTRL            MBOX_M2A_2E96
            #define PIP_MAIN_ID             0

        #define SNDR2_MBOX_BYTE_VOL_GAIN              (MBOX_M2A_2E98)

    /* SND-R2 -> MCU */
        //// SND-R2 -> MCU : Common ////
        #define SNDR2_MBOX_BYTE_PARAM_TOKEN           MBOX_A2M_2EA2+1
        #define SNDR2_MBOX_WORD_INFO_VALUE1           MBOX_A2M_2EA4
        #define SNDR2_MBOX_WORD_INFO_VALUE2           MBOX_A2M_2EA6
        #define SNDR2_MBOX_WORD_R2_STATUS             MBOX_A2M_2EA8
        #define SNDR2_MBOX_WORD_R2_VERSION            MBOX_A2M_2EAA
        #define SNDR2_MBOX_BYTE_R2_DBG_ACK            MBOX_A2M_2EAE
        #define SNDR2_MBOX_BYTE_WHILE_1_CNTR          MBOX_A2M_2EB2
        #define SNDR2_MBOX_BYTE_TIMER_CNT             MBOX_A2M_2EB2+1

    /* SND R2 -> MCU */

        //// SND-R2 -> MCU : R2 decoder ////

        //// SND-R2 -> MCU : R2 sound effect ////

        //// SND-R2 -> MCU : R2 encoder ////
        #define SNDR2_MBOX_WORD_R2_HDMI_STATUS        MBOX_A2M_2EB0
        #define SNDR2_MBOX_WORD_MP3E_WRPTR            MBOX_A2M_2EB4

#ifdef  __BUILD_FOR_R2__
/* DEC-R2 --> SND-R2 */   //32bits per register
    #define MBPX_DECR2_SNDR2_DDENC_SEL         (MBOX_SER2_D2A_IO_1)
        #define DDENC_ENABLE_BIT                0
        #define DDENC_LFE_BIT                  6
        #define DDENC_ACMOD_BIT                2

    #define MBPX_DECR2_SNDR2_DTSENC_SEL         (MBOX_SER2_D2A_IO_1+1)
        #define DTSENC_ENABLE_BIT                0
        #define DTSENC_LFE_BIT                  6
        #define DTSENC_ACMOD_BIT                2
    #define MBPX_D2A_MS12                      (MBOX_SER2_D2A_IO_2)
        #define DEC1_MS12_PCMR_ENABLE_BIT       0
        #define DEC2_MS12_PCMR_ENABLE_BIT       1
        #define DEC1_MS12_DAP_MCH_BIT           2
        #define DEC2_MS12_DAP_MCH_BIT           3
    #define MBPX_D2A_PCM_BUF_RPTR              (MBOX_SER2_D2A_IO_3)

/* SND-R2 --> DEC-R2  */
    #define MBOX_SNDR2_DECR2_ENC_ON_CHIP      MBOX_SER2_A2D_IO_0
        #define DDENC_SUPPORT_BIT           _BIT0
        #define DTSENC_SUPPORT_BIT          _BIT1
        #define DDPENC_SUPPORT_BIT          _BIT2

    /* SND-DSP -> SND-R2 */
        //// R2 decoder ////
        #define MBOX_S2A_DSPWORD_SPEAKER_CH_VOLUME  MBOX_S2A_IO_A
        #define MBOX_S2A_DSPWORD_IP_AUTH         MBOX_S2A_IO_E
        #define MBOX_S2A_DSPWORD_BOUND_OPTION    MBOX_S2A_IO_F

        //// R2 sound effect ////
        #define MBOX_S2A_DSPWORD_PCMIN_WRPTR    MBOX_S2A_IO_1
        #define MBOX_S2A_DSPWORD_PCMIN2_WRPTR   MBOX_S2A_IO_2
        #define MBOX_S2A_DSPWORD_SPDIF_RDPTR    MBOX_S2A_IO_3
        #define MBOX_S2A_DSPWORD_HDMI_RDPTR     MBOX_S2A_IO_4

    /* SND-R2 -> SND DSP */
        //// R2 decoder ////

        //// R2 sound effect ////
        #define MBOX_A2S_DSPWORD_PCMOUT_WRPTR   MBOX_A2S_IO_1
        #define MBOX_A2S_DSPWORD_PCMOUT2_WRPTR   MBOX_A2S_IO_2
        #define MBOX_A2S_DSPWORD_SPDIF_WRPTR    MBOX_A2S_IO_3

        #define MBOX_A2S_DSPWORD_HDMI_WRPTR   MBOX_A2S_IO_4

        #define MBOX_A2S_DSPWORD_SPDIF_CTRL     MBOX_A2S_IO_5
        #define MBOX_A2S_DSPWORD_HDMI_CTRL 	    MBOX_A2S_IO_5
            #define MBOX_HDMI_CTRL_START_BIT	    _BIT_23_
            #define MBOX_HDMI_CTRL_EN_BIT	        _BIT_21_
            #define MBOX_HDMI_HBR_MODE_BIT          _BIT_20_
            #define MBOX_SPDIF_CTRL_EN_BIT          _BIT_17_
            #define MBOX_SPDIF_CTRL_START_BIT       _BIT_19_
            #define MBOX_SPDIF_SRATE_BIT            12
            #define MBOX_HDMI_SRATE_BIT             8
            #define MBOX_HDMI_PCM_NONPCM            _BIT_7_
            #define MBOX_SPDIF_PCM_NONPCM           _BIT_6_
            #define MBOX_HDMI_NONPCM_OWNER          _BIT_4_
            #define MBOX_HDMI_NONPCM_BIT            4
            #define MBOX_SPDIF_NONPCM_OWNER         _BIT_2_
            #define MBOX_SPDIF_NONPCM_BIT           2
        #define MBOX_A2S_DDENC_OUTMCH_WRPTR     MBOX_A2S_IO_6

        #define MBOX_A2S_DSP_CTRL_BYTE        MBOX_A2S_IO_7
            #define A2S_DDENC_ENABLE               _BIT_0_
            #define A2S_MIXER_DISABLE              _BIT_1_
        #define MBOX_A2S_OUTPCM_DMX_WRPTR     MBOX_A2S_IO_9

        #define MBOX_A2S_DSPWORD_SOUND_PROCESS_CTRL     MBOX_A2S_IO_F
           #define MBOX_PRE_SOUND_PROCESS_DISABLE_BIT	    _BIT0
           #define MBOX_POST_SOUND_PROCESS_DISABLE_BIT	    _BIT1

        //// snd R2 -> MCU : Common ////
        #define MBOX_R2_START_ADDRESS_LO        MBOX_R2Reg_82
        #define MBOX_R2_START_ADDRESS_HI        MBOX_R2Reg_84

/***********************************************************
*   DEBUG
***********************************************************/
    #define dbg_print(x)                                x
//  #define MMTS_REPORT_SCR_INSTEAD

/***********************************************************
*   Hardware patch
***********************************************************/
    #define PATCH_0001      0           //Pacth for R2 MAU problem (T8 U01)

/***********************************************************
*   BDMA Address Setting (R2)    For R2 at 112E
***********************************************************/
#define RIUBASE_AU_BDMA_CH0    0x00000200
#define RIUBASE_AU_BDMA_CH1    0x00000220
#define RIUBASE_AU_BDMA_CH2    0x00000240
#define RIUBASE_AU_BDMA_CH3    0x00000260
#define RIUBASE_AU_BDMA_MISC   0x00000280
#define BANK_BDMA_RIU                 0xa0000000

#endif  //__BUILD_FOR_R2__
#endif  //__PLATFORM_SNDR2_PROJ_INCLUDE_FILE__
