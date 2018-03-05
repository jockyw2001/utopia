#ifndef __PLATFORM_PROJ_INCLUDE_FILE__
#define __PLATFORM_PROJ_INCLUDE_FILE__

#include "ddr_config.h"
#include "mbox_decR2.h"

#define DEC_R2_CLOCK             432000000

/***********************************************************
*   Decoder threshold settings
***********************************************************/
    #define MPG_AV_DEFAULT_DELAY                0x3E1E
    #define AC3_AV_DEFAULT_DELAY                0x4300
    #define AAC_AV_DEFAULT_DELAY                0x327E
        #define DELAY_UPPER_LIMIT                   0x6978
        #define DELAY_LOWER_LIMIT                   0xA8C

    #define DEFAULT_ES_BLOCK_LEVEL                      0x1800
    #define DEFAULT_PCM_BLOCK_LEVEL                     0x20000
    #define DEFAULT_AVSYNC_OFFSET                       0x3138                  //AVSYNC delay in DTV: 140ms, Unit: 33bits
    #define DEFAULT_SPDIF_OFFSET                        0xC00                    //
    #define DEFAULT_STOP_PARSER_ES_LEVEL                0x10000
    #define DEFAULT_FILE_HAND_SHAKE_SIZE                0x800

    #define DEC_OUTPUT_BYTES_PER_SAMPLE                 2
    #define DEC_OUTPUT_DMX_CHANNELS                     2
    #define DEC_OUTPUT_BYTES_PER_DMX_SAMPLE             4
    #define DEC_OUTPUT_MCH_CHANNELS                     6                   //order : L / C / R / Ls / Rs / LFE
    #define DEC_OUTPUT_OMX_MCH_CHANNELS                 10

    #define AVSYNC_TD_TIMEOUT_THRESHOLD                 0x36EE8/2                                                // 2500ms         !?? Threshold?? STC ? APTS ????, ??? FreeRun
    #define AVSYNC_WAIT_STC_TD                          90*100/2                                                  // 100 ms          !?? Threshold?? STC ?? APTS ???, ??????STC
    #define AVSYNC_SKIP_FRM_TD                          90*45/2                                                  // 45 ms          !?? Threshold?? STC ?? APTS ???, ??skip frame??STC
    #define SYS_DEC_HDMIPLAY_PCM_LEVEL                  45*48  //(1536-64)*48  //0                              // 45 ms
    #define SYS_DEC_HDMISKIP_PCM_LEVEL                  60*48                                                   // 60 ms
    #define SYS_DEC_MMAVI_PLAY_PCM_LEVEL                45*48
    #define AVSYNC_11172_OVERFLOW_THRESHOLD             0x6D3A06             // 0x7FFFFFFF/300
                                                                             //
    #define SYS_DEC1_WAIT_1ST_PTS_TIMEOUT_PCM_LEVEL     700*48                              // 700ms  (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_WAIT_STC_TIMEOUT_PCM_LEVEL         700*48                              // 700ms  (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_FREEPLAY_PCM_LEVEL                 45*48                               // 45ms (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_STOP_DECODE_PCM_LEVEL              (1024-64)*48                        //(1536-64)*DEC_OUTPUT_1MS_48K_SIZE      //(1024-64)*DEC_OUTPUT_1MS_48K_SIZE      // 1024ms (should small than "SYS_DEC_STOP_DECODE_PCM_LEVEL")
    #define SYS_DEC1_STOP_DECODE_PCM_LEVEL_OMX          1728*48
    #define SYS_DEC1_STOP_DECODE_PCM_LEVEL_MM_TS        120*48
    #define SYS_DEC1_STOP_DECODE_PCM_LEVEL_DTV          ((PCM1_DRAM_SIZE/48000/2/10) * 1000 -64)*48
    #define SYS_DEC1_WAIT_STC_TIMEOUT_ES_LEVEL          0x8000
    #define SYS_DEC1_WAIT_1ST_PTS_TIMEOUT_ES_LEVEL      0x2000

    #define SYS_DEC_SLEEP_PCM_LEVEL                     60*48                                     // 60ms
    #define SYS_DEC_WAIT_1st_PTS_FRMCNT                 32
    #define SYS_AD_NOSIGNAL_THRESHOLD                   128
    #define SYS_DEC_DTV_FREEPLAY_PCM_LEVEL              64*48

    #define SYS_DEC_MM_TD_TIMEOUT_THRESHOLD_UP_LIMIT            (200*90000/2)
    #define SYS_DEC_MM_TD_TIMEOUT_THRESHOLD_DOWN_LIMIT        (-30*90000/2)

#ifdef  __BUILD_FOR_R2__
/***********************************************************
*   Software Setting
***********************************************************/
    #define R2_SIMULATION                   0
    #define R2_BUILD_IN_TEST_PATTERN        0

    #define R2_DECODER_EN                           (1)
        #define R2_DECODER1_FORCE_MCH_OUTPUT        (1)
        #define R2_DECODER1_FORCE_2CH_OUTPUT        (0)
        #define R2_DECODER2_FORCE_MCH_OUTPUT        (1)
        #define R2_DECODER2_FORCE_2CH_OUTPUT        (0)
#ifdef __aeon__
        #define R2_DECODER1_OUTPUT_BY_DSP           (0)
        #define R2_DECODER2_OUTPUT_BY_DSP           (0)
#else
        #define R2_DECODER1_OUTPUT_BY_DSP           (0)
        #define R2_DECODER2_OUTPUT_BY_DSP           (0)
#endif

    #define PRE__SOUND_PROCESS_EN           0
    #define POST_SOUND_PROCESS_EN           0
    #define FD230_EN                        1

    #define R2_AD_ENABLE                    1
    #define R2_DISABLE_PARSERAB             0
    #define PARSER_PUSI_SUPPORT             1
    #define PASDMA_ENABLE                   1

    #define R2_INTERRUPT_PRINTF

#if (R2_SIMULATION == 1)
    #define R2_SIM_DEC1_AUDIO_TYPE       1           // 1: AC3, 5: AAC, 3: mp3 (use ES1(md) and ES2(ad))
    #define R2_SIM_DEC2_AUDIO_TYPE       1           // 1: AC3, 5: AAC, 3: mp3 (use ES3(md) and ES4(ad))
    #define R2_SIM_DEC1_COMMAND_ENABLE      0x11        //0: disable, 0x01 : MD only, 0x11: MD+AD
    #define R2_SIM_DEC2_COMMAND_ENABLE      0           //0: disable, 0x01 : MD only, 0x11: MD+AD
    #undef R2_INTERRUPT_PRINTF
#endif

/***********************************************************
*   wrapper define for each IP
***********************************************************/
#define DEC_R2_DQMEM_SIZE                   64      //unit is kbytes
#define DEC_R2_DQMEM_ADDR                   0xFFFF0000

#define R2_OUTPUT_ENC_RAW_DATA          0
#define R2_DEC_HDMI_OUTPUT              1       // 1:HDMI_Nonpcm_Tx or HDMI_ARC
#define UNI_DECODE_DONE_NON_6_BLOCK     1
#define REMOVE_DTS_TRANSCODER           0
#define REMOVE_MS11_DDENCODE            0
#define HEAAC_SPDIF_BYPASS              1
#define SND_R2_SUPPORT_DDENC            1
#define SND_DSP_SUPPORT_DDENC           1
#define SND_R2_SUPPORT_DDPENC           1
#define SND_DSP_SUPPORT_DPDENC          1
#define SND_R2_SUPPORT_DTSENC           1
#define SND_DSP_SUPPORT_DTSENC          1
#define USE_SPDIF_REPLACE_HDMI          0
#define HDMI_RX_LOW_LATENCY_ENABLE      0

#define L2_CRC_ERR_MUTE_NUM             2
#define L2_SYNCWORD_ERR_MUTE_NUM        2
#define DDP_HDMI_FILL256_SAMPLE_BEFORE_FIRST_FRAME  0
#define SUPPORT_DRCLINE_MPEG_LEVEL_MODIFY           1
/***********************************************************
*   Hardware Setting
***********************************************************/
    #define LOG2_BYTES_IN_LINE              4
    #define BYTE_IN_LINE                    (1<<LOG2_BYTES_IN_LINE)
    #define BYTE_IN_LINE_MASK               (BYTE_IN_LINE - 1)
    #define ALIGN_TO_MIU_LINE(x)            (x&BYTE_IN_LINE_MASK)?((x+BYTE_IN_LINE)&(~BYTE_IN_LINE_MASK)):x

    #define HAL_ICACHE_SIZE             8192
    #define HAL_ICACHE_LINE_SIZE        64
    #define HAL_ICACHE_WAY              4

    #define HAL_DCACHE_SIZE             8192
    #define HAL_DCACHE_LINE_SIZE        64
    #define HAL_DCACHE_WAY              4

    /* D_PREFETCH */
    #define DC_WR_ALLOCATE_EN           1       // DC_WRITE_ALLOCATE_EN
    #define D_PREFETCH_NUM              0       // k7 no this feature, 0: disable D_PREFETCH, 1 ~ 4: on
    #define R2_SPRA_DCU_PREFETCH        (118)   //118 for DCU prefetch, 119 for ICU prefetch

    /* interrupt */
    #define NR_IRQS                     20
    #define IRQ_UART                    0x13    //19

/***********************************************************
*
***********************************************************/
    #define MAX_PRINTF_BUF                  0x8000
    #define decSysFunc_start        //dbg_func_entries++
    #define decSysFunc_end          //dbg_func_entries--
    #define decSysPrint(x)           //{int xxx=0;  for( xxx=0; xxx<dbg_func_entries; xxx++) printf("\t");}   x
#ifdef WIN32
    #define decSysPrint_R2(x)
#else
    #define decSysPrint_R2(x)       x
#endif

    #define CHK_BUFF_SUM(addr, size)    {   \
            int xxx=0;  \
            unsigned char *p = (unsigned char *) addr;  \
            unsigned char sum = 0;  \
            for (xxx=0; xxx<size; xxx++)    \
                sum += p[xxx];  \
            printf("sum = %x ", sum);    \
        }

    #define CHK_BUFF_XOR(addr, size)    {   \
            int xxx=0;  \
            unsigned char *p = (unsigned char *) addr;  \
            unsigned char sum = 0;  \
            for (xxx=0; xxx<size; xxx++)    \
                sum ^= p[xxx];  \
            printf("xor = %x ", sum);    \
        }

/***********************************************************
*   DDR Setting
***********************************************************/
    #ifdef __aeon__
    #define OFFSET_R2                0
    #else
    #define OFFSET_R2                ((unsigned int)&chip_audio_memory_buf)
    #endif

    #define OFFSET_R2_TO_ASNDR2     (OFFSET_R2 + ADEC__R2_DDR_SIZE)
    #define OFFSET_R2_TO_ASNDDSP    (OFFSET_R2 + ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE)
    #define OFFSET_R2_TO_COMM_DDR   (OFFSET_R2 + ADEC__R2_DDR_SIZE +ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE)

    /* DECODER DDR */
        #define DEC1_METADATA1_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA1_DRAM_ADDR)
        #define DEC1_METADATA2_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA2_DRAM_ADDR)
        #define DEC1_METADATA3_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC1_METADATA3_DRAM_ADDR)

        #define DEC2_METADATA1_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA1_DRAM_ADDR)
        #define DEC2_METADATA2_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA2_DRAM_ADDR)
        #define DEC2_METADATA3_BUF               (OFFSET_R2_TO_ASNDDSP + OFFSET_DEC2_METADATA3_DRAM_ADDR)
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
        #define BUF_HDMI_OFFSET		            OFFSET_HDMI_NONPCM_DRAM_BASE
        #define BUF_HDMI_ADDR			        (OFFSET_R2_TO_ASNDDSP+ BUF_HDMI_OFFSET)
        #define BUF_HDMI_SIZE			        HDMI_NONPCM_DRAM_SIZE
        #endif

        #if 0
        #define DSP1_ENC_RAW_OFFSET	            0x334000
        #define DSP1_ENC_RAW_BUFADDR		    (OFFSET_R2_TO_COMM_DDR + DSP1_ENC_RAW_OFFSET)
        #define DSP1_ENC_RAW_BUFSIZE 		    0x38E00
        #endif

    /* sound effect related DDR */

        /* pre sound DDR */
        //#define BUF_PCMIN_ADDR          (OFFSET_R2_TO_ASNDDSP + OFFSET_SE_MAIN_IN_DRAM_ADDR)
        //#define BUF_PCMIN_SIZE           SE_MAIN_IN_DRAM_SIZE

        /* post sound DDR */
        //#define BUF_PCMIN2_ADDR          (OFFSET_R2_TO_ASNDDSP + OFFSET_SE_MAIN_OUT_DRAM_ADDR)
        //#define BUF_PCMIN2_SIZE          SE_MAIN_OUT_DRAM_SIZE

        #define BUF_SPDIF_OFFSET        OFFSET_SPDIF_NONPCM_DRAM_BASE
        #define BUF_SPDIF_ADDR          (OFFSET_R2_TO_ASNDDSP+ BUF_SPDIF_OFFSET)
        #define BUF_SPDIF_SIZE          SPDIF_NONPCM_DRAM_SIZE

    /* ENCODER DDR */
        #define DDENC_METADATA_OFFSET   OFFSET_DDENC_METADATA_DRAM_ADDR
        #define DDENC_METADATA_ADDR     (OFFSET_R2_TO_ASNDDSP+ DDENC_METADATA_OFFSET)
        #define DDENC_METADATA_SIZE     DDENC_METADATA_DRAM_SIZE

    /*MPEG ENCODER*/
        #define MPEG_ENC_APTS_BASE  (OFFSET_R2_TO_COMM_DDR+ OFFSET_MP3_ENC_APTS_DRAM_BASE)

    /* MISC */

/***********************************************************
*   OMX nonPCM threshold settings
***********************************************************/
    #define OMX_PCM_COMPARE_LEVEL               0x4800    //SmpPerCH = 0x1200*10/2/2
    #define OMX_DEFAULT_ADECID                  R2_ADEC_ID2
    #define OMX_THRESHOLD                       4
    #define OMX_NONPCM_START_EN_LEVEL           0x4000
    #define OMX_NONPCM_STOPSKIP_LEVEL           0x1600
    #define OMX_PCM_CONSTANT_DELAY              0x0

/***********************************************************
*   Sound effect Setting
***********************************************************/
    #define ASND_PROCESS_MODE               2           // 2(adec_acmod_stereo) or 7(adec_acmod_mod32)
    #define ASND_PROCESS_ITF_UNIT           32
    #define BYTES_IN_PCM                    3

    /* pre sound process */
    #define ASND_PRE_PROCESS_BUF_UNIT           256
    #define ASND_PRE_INPUT_PCM_CH               8           //(RAW DMX + 5.1)

    #define DOWNMIX_2_CH_ORDER              0
    #define MULTI_5P1_CH_ORDER              2
    #define ENC_2_CH_ORDER                  8
    #define ECHO_CH_ORDER                   10

    /* post sound process */
    #define ASND_POST_INPUT_PCM_CH      8
    #define ASND_POST_PROCESS_BUF_UNIT  256

#endif      //__BUILD_FOR_R2__

/***********************************************************
*   Mbox Setting
***********************************************************/
//---------------------------------------
/******************* MCU <--> DEC-R2 *******************/
    /* MCU -> DEC-R2 */
        //// MCU -> DEC-R2 : Common ////
        #define MBOX_WORD_MCUCMD_PARAMTYPE      MBOX_M2D_0380   //generate interrut
        #define MBOX_BYTE_MCUCMD_ID_SEL         MBOX_M2D_0382
        #define MBOX_BYTE_MCUCMD_Get_OR_Set     MBOX_M2D_0382+1
        #define MBOX_WORD_MCUCMD_PARAM1         MBOX_M2D_0384
        #define MBOX_WORD_MCUCMD_PARAM2         MBOX_M2D_0386
        #define MBOX_WORD_R2_CTRL               MBOX_M2D_0388
            #define STREAMING_LOW_LATENCY_LIMITER_EN    _BIT_15_
        #define MBOX_WORD_R2_SYSTEM_DBG         MBOX_M2D_038A
            /*
            [ 0] decoder decode frame statue
            [ 1] decoder output pcm status
            [ 3] reset measured max AD no signal counter
            [ 4] omx decode done print
            [ 8] decoder print info
            [ 9] measure decoder mips
            [10] measure miu latency
            [11] enable wfi mode
            [12] disable skip frm when doing AV sync
            [13] disable wait STC when doing AV sync
            [14] disable calculate increase cPTS in timer
            */

        #define MBOX_WORD_DBG_DUMP_DDR_ADDR_HI     MBOX_M2D_038C
        #define MBOX_WORD_DBG_MBOX_VALUE_HI        MBOX_M2D_038C
        #define MBOX_WORD_DBG_MBOX_PRINT_SIZE      MBOX_M2D_038C
        #define MBOX_WORD_DBG_DUMP_DDR_ADDR_LO     MBOX_M2D_038E
        #define MBOX_WORD_DBG_MBOX_VALUE_LO        MBOX_M2D_038E
        #define MBOX_WORD_DBG_DUMP_DDR_SIZE     MBOX_M2D_039C
        #define MBOX_WORD_DBG_MBOX_ADDR         MBOX_M2D_039C
        #define MBOX_BYTE_OMX_NPCM_SYNC_DBG     MBOX_M2D_039C+1
        #define MBOX_BYTE_SYSTEM_START         MBOX_M2D_039C
        #define MBOX_BYTE_STATUS_SELECT         MBOX_M2D_039E
        #define MBOX_BYTE_STATUS_SPEED          MBOX_M2D_039E+1

        //// MCU -> DEC-R2 : R2 decoder ////
        #define MBOX_BYTE_DEC_SPDIF_SEL         MBOX_M2D_0396
        #define MBOX_BYTE_DEC1_SEL              MBOX_M2D_0398
        #define MBOX_BYTE_DEC2_SEL              MBOX_M2D_039A
        #define MBOX_BYTE_DEC1_CMD              MBOX_M2D_0398+1
        #define MBOX_BYTE_DEC2_CMD              MBOX_M2D_039A+1

        #define MBOX_BYTE_ASNDDSP_SEL           MBOX_M2D_0394

    /* DEC-R2 -> MCU */
        //// DEC-R2 -> MCU : Common ////
        #define MBOX_WORD_MCU_INTERRUPT         MBOX_D2M_03A0
        #define MBOX_BYTE_PARAM_TOKEN           MBOX_D2M_03A2+1
        #define MBOX_WORD_INFO_VALUE1           MBOX_D2M_03A4
        #define MBOX_WORD_INFO_VALUE2           MBOX_D2M_03A6
        #define MBOX_WORD_R2_STATUS             MBOX_D2M_03A8
        #define MBOX_WORD_R2_VERSION            MBOX_D2M_03AA
        #define MBOX_BYTE_R2_DBG_ACK_HI         MBOX_D2M_03AC
        #define MBOX_BYTE_R2_DBG_ACK            MBOX_D2M_03AE
        #define MBOX_BYTE_WHILE_1_CNTR          MBOX_D2M_03B2
        #define MBOX_BYTE_TIMER_CNT             MBOX_D2M_03B2+1
        #define MBOX_BYTE_R2_SPDIF              MBOX_D2M_03B4
        #define MBOX_BYTE_LOADCODE_ACK          MBOX_D2M_03B6
        #define MBOX_BYTE_R2_WFI_CNTR           MBOX_D2M_03B8

    /* dec R2 -> MCU */
        //// dec R2 -> MCU : Common ////
        #define MBOX_R2_START_ADDRESS_LO        MBOX_R2Reg_82
        #define MBOX_R2_START_ADDRESS_HI        MBOX_R2Reg_84

        #if 0
        #define MBOX_WORD_R2_HDMI_STATUS        MBOX_D2M_03B0
        #endif

#ifdef __BUILD_FOR_R2__
/* dec R2 <--> snd R2 */
    #define MBPX_D2A_DDENC                      (MBOX_D2A_IO_1)
    #define MBPX_D2A_DTSENC                     (MBOX_D2A_IO_1+1)
    #define MBPX_D2A_MS12                       (MBOX_D2A_IO_2)
        #define DEC1_MS12_PCMR_ENABLE_BIT       0
        #define DEC2_MS12_PCMR_ENABLE_BIT       1
        #define DEC1_MS12_DAP_MCH_BIT           2
        #define DEC2_MS12_DAP_MCH_BIT           3
    #define MBPX_D2A_PCM_BUF_RPTR               (MBOX_D2A_IO_3)

    /* SND-DSP -> snd R2 */
        //// R2 decoder ////
        #define MBOX_S2D_DSP_TO_R2_COMMAND          MBOX_S2D_IO_0
            #define S2D_CMD_RESET_PCM1_AVSYNC  0x0001
            #define S2D_CMD_RESET_PCM2_AVSYNC  0x0002

        #define MBOX_S2D_DSP_TO_R2_PARAM            MBOX_S2D_IO_1
        #define MBOX_S2D_R2CMD_RECEIVE_CNT          MBOX_S2D_IO_2

        #define MBOX_S2D_PCM1_PLAYCNT               MBOX_S2D_IO_3
        #define MBOX_S2D_PCM1_FIFOCNT               MBOX_S2D_IO_4
        #define MBOX_S2D_PCM1_DRAM_RDPTR            MBOX_S2D_IO_5

        #define MBOX_SNDDSP_2_DECR2_ENC_ON_CHIP           MBOX_S2D_IO_9

        #define MBOX_SNDR2_2_DECR2_ENC_ON_CHIP            MBOX_A2D_IO_0

        #define MBOX_S2D_PCM2_PLAYCNT               MBOX_S2D_IO_6
        #define MBOX_S2D_PCM2_FIFOCNT               MBOX_S2D_IO_7
        #define MBOX_S2D_PCM2_DRAM_RDPTR            MBOX_S2D_IO_8

      //#define MBOX_S2D_REG_DECODER_SEL            MBOX_S2D_IO_A
      //#define MBOX_S2D_REG_HDMI_IN_FREQ           MBOX_S2D_IO_B

        #define MBOX_S2D_DSPWORD_SPDIF_RDPTR        MBOX_S2D_IO_A
        #define MBOX_S2D_DSPWORD_HDMI_RDPTR         MBOX_S2D_IO_B

        #define MBOX_S2D_DSPWORD_IP_AUTH            MBOX_S2D_IO_E
        #define MBOX_S2D_DSPWORD_BOUND_OPTION       MBOX_S2D_IO_F

    /* R2 -> SND DSP */
        #define MBOX_D2S_PCMISR_CTRL            MBOX_D2S_IO_1
            #define MBOX_R2_PCM1ISR_PLAY_START_BIT          _BIT_0_         //--> 這各由 playSmpFlag / stop / pause 決定
            #define MBOX_R2_PCM1ISR_PLAY_MUTE_BIT           _BIT_1_           //--> Mute
            #define MBOX_R2_PCM1ISR_USING_ASINK_ISR_BIT     _BIT_2_
            #define MBOX_R2_PCM2ISR_PLAY_START_BIT          _BIT_8_
            #define MBOX_R2_PCM2ISR_PLAY_MUTE_BIT           _BIT_9_
            #define MBOX_R2_PCM2ISR_USING_ASINK_ISR_BIT     _BIT_10_

        #define MBOX_D2S_PCM1_DRAM_WRPTR        MBOX_D2S_IO_2
        #define MBOX_D2S_PCM1_SYNTH_H           MBOX_D2S_IO_3
        #define MBOX_D2S_PCM1_SYNTH_L           MBOX_D2S_IO_4

        #define MBOX_D2S_PCM2_DRAM_WRPTR        MBOX_D2S_IO_5
        #define MBOX_D2S_PCM2_SYNTH_H           MBOX_D2S_IO_6
        #define MBOX_D2S_PCM2_SYNTH_L           MBOX_D2S_IO_7

        #define MBOX_D2S_R2_TO_DSP_COMMAND      MBOX_D2S_IO_8
            #define D2S_CMD_UPD_PCM1_MUTECNT        _BIT_0_
            #define D2S_CMD_CLR_PCM1_PLAYCNT        _BIT_1_
            #define D2S_CMD_UPD_PCM1_PLAYCNT        _BIT_2_
            #define D2S_CMD_FLUSH_PCM1_SMPS         _BIT_3_
            #define D2S_CMD_RESET_PCM1              _BIT_4_
            #define D2S_CMD_UPD_PCM2_MUTECNT        _BIT_5_
            #define D2S_CMD_CLR_PCM2_PLAYCNT        _BIT_6_
            #define D2S_CMD_UPD_PCM2_PLAYCNT        _BIT_7_
            #define D2S_CMD_FLUSH_PCM2_SMPS         _BIT_8_
            #define D2S_CMD_RESET_PCM2              _BIT_9_

        #define MBOX_D2S_R2_TO_DSP_PARAM        MBOX_D2S_IO_9
        #define MBOX_D2S_DSPWORD_SPDIF_WRPTR    MBOX_D2S_IO_A
        #define MBOX_D2S_DSPWORD_HDMI_WRPTR     MBOX_D2S_IO_B

        #define MBOX_D2S_DSPWORD_SPDIF_CTRL     MBOX_D2S_IO_C
        #define MBOX_D2S_DSPWORD_HDMI_CTRL      MBOX_D2S_IO_C
            #define MBOX_HDMI_CTRL_START_BIT      _BIT_23_
            #define MBOX_HDMI_HBR_MODE_BIT        _BIT_20_
            #define MBOX_HDMI_CTRL_EN_BIT         _BIT_21_
            #define MBOX_SPDIF_CTRL_EN_BIT           _BIT_17_
            #define MBOX_SPDIF_CTRL_START_BIT        _BIT_19_
            #define MBOX_HDMI_PCM_NONPCM            _BIT_7_
            #define MBOX_SPDIF_PCM_NONPCM           _BIT_6_
            #define MBOX_HDMI_NONPCM_OWNER         _BIT_4_
            #define MBOX_SPDIF_NONPCM_OWNER         _BIT_2_
/*
                [23]    HDMI nonPcm Start
                [22]
                [21]    HDMI nonPcm PlayEnable
                [20]    HDMI HBR mode
                [19]    SPDIF nonPcm Start
                [18]    inform DDEncode to attenuate 4.75dB
                [17]    SPDIF nonPcm PlayEnable
                [16]
                [15:12] SPDIF nonPcm sampleRate index
                    // 0: 96K, 1: 88K, 2: 64K
                    // 3: 48K, 4: 44K, 5: 32K
                    // 6: 24K, 7: 22K, 8: 16K
                    // 9: 12K, a: 11K, b:  8K

                [11:8]  HDMI  nonPcm sampleRate index
                    // 0: 96K, 1: 88K, 2: 64K
                    // 3: 48K, 4: 44K, 5: 32K
                    // 6: 24K, 7: 22K, 8: 16K
                    // 9: 12K, a: 11K, b:  8K
                    // c:192K, d: 176K e: 128K

                [7]     HDMI is Pcm or nonPcm
                [6]     SPDIF is Pcm or nonPcm
                [5:4]   hdmi  nonPcm owner
                [3:2]   spdif nonPcm owner
                [0:1]   spdif/hdmi PCM attenuator index
*/


        #define MBOX_D2S_R2_DOLBY_META_DATA     MBOX_D2S_IO_D

        #define MBOX_D2S_R2_TO_DSP_ENCODE        MBOX_D2S_IO_E
            #define D2S_CMD_DDENC_ENABLE            _BIT_6_
            #define D2S_CMD_DTSENC_ENABLE           _BIT_5_
            #define D2S_LFE_MODE                    _BIT_4_
            #define D2S_AC_MODE                     _BIT_0_

        //#define MBOX_D2S_R2_TO_DSP_MISC        MBOX_D2S_IO_F
        //    #define D2S_CMD_SOUND_MIXER_DISABLE   _BIT_0_

/***********************************************************
*   SE R2 mailbox
***********************************************************/
        #define SNDR2_MBOX_BYTE_R2_DBG_ACK            MBOX_A2M_2EAE
        #define SNDR2_MBOX_BYTE_STATUS_SELECT         MBOX_M2A_2E9E
        #define MBOX_S2A_DSPWORD_PCMIN_WRPTR    MBOX_S2A_IO_1
        #define MBOX_A2S_DSPWORD_PCMOUT_WRPTR   MBOX_A2S_IO_1

        #define MBOX_A2S_DSPWORD_HDMI_WRPTR   MBOX_A2S_IO_4
        #define MBOX_A2S_DSPWORD_SPDIF_WRPTR    MBOX_A2S_IO_3

        #define MBOX_S2A_DSPWORD_SPDIF_RDPTR    MBOX_S2A_IO_3
        #define MBOX_S2A_DSPWORD_HDMI_RDPTR     MBOX_S2A_IO_4
/***********************************************************
*   DEBUG
***********************************************************/
    #define dbg_print(x)                                x
//  #define MMTS_REPORT_SCR_INSTEAD

/***********************************************************
*   Hardware patch
***********************************************************/
    #define PATCH_0001      0           //Pacth for R2 MAU problem (T8 U01)

#endif  //__BUILD_FOR_R2__
#endif  //__PLATFORM_PROJ_INCLUDE_FILE__