#ifndef _DDR_CONFIG_H_
#define _DDR_CONFIG_H_

/************************************************
*  請保持 Utopia 和 DSP 裡的這個檔案一致
*
*  1. 註解不要用 !
*  2. 宣告不要用 .const xxxx = ????;
************************************************/

/************************************************
*   DSP DDR memory layout
*************************************************/
    /* DSP DDR size */
        #define ADEC__R2_DDR_SIZE       0x5C0000             //5.75 MB
        #define ASND__R2_DDR_SIZE       0x000000             //0.00 MB
        #define ASND_DSP_DDR_SIZE       0x2C0000             //2.75 MB  (optional: 2144KB with HDMI buffer)
        #define COMMON_DDR_SIZE         0x180000             //1.50 MB  (optional: 48 KB pcm capture)
                                                             // Total = 10.0 MB
        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE             0x0000000
            #define SPDIF_DLY_DRAM_SIZE                    0x18000      // 96KB

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR           0x004B000
            #define SE_MAIN_OUT_DRAM_SIZE                  0x39000      // 228KB

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR            0x0084000
            #define SE_MAIN_IN_DRAM_SIZE                   0x38400      // 228KB (buffer size 需要被 "SE_BUFF2_DMA_24BIT_LINE_SIZE*BYTES_IN_MIU_LINE" 整除)

            #define OFFSET_MSTAR_SURROUND_DRAM_ADDR        0x00BD000
            #define MSTAR_SURROUND_DRAM_SIZE               0x0003000                                                   //32KB (use 12KB)

            #define OFFSET_KTV_SURROUND_DRAM_ADDR          0x00C5000
            #define KTV_SURROUND_DRAM_SIZE                 0x1FE00                                                     //128KB

            #define OFFSET_DDENC_METADATA_DRAM_ADDR        0x00F5000
            #define DDENC_METADATA_DRAM_SIZE               0x660       //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE


            /* HEAD PHONE delay */
            #define OFFSET_HEAD_PHONE_DLY_DRAM_BASE        0x0000000
            #define HEAD_PHONE_DLY_DRAM_SIZE               0x00000      // 0KB                                                        //144KB

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE         0x0000000
            #define CH5_INPUT_DLY_DRAM_SIZE                0x00000      // 0KB                                                       //144KB

            /* CH6 input delay */
            #define OFFSET_CH6_INPUT_DLY_DRAM_BASE         0x0000000
            #define CH6_INPUT_DLY_DRAM_SIZE                0x00000      // 0KB                                                     //144KB

            #define OFFSET_ES1_DRAM_ADDR                   0x00F7000
            #define ES1_DRAM_SIZE                          0x20000      // 128KB

            #define OFFSET_ES2_DRAM_ADDR                   0x0117000
            #define ES2_DRAM_SIZE                          0x20000      // 128KB

            #define OFFSET_ES3_DRAM_ADDR                   0x0137000
            #define ES3_DRAM_SIZE                          0x10000      // 64KB

            #define OFFSET_ES4_DRAM_ADDR                   0x0147000
            #define ES4_DRAM_SIZE                          0x10000      // 64KB

            #define OFFSET_SIF1_DRAM_ADDR                  0x0157000
            #define SIF1_DRAM_SIZE                         0x8000       // 32KB
            #define OFFSET_SIF2_DRAM_ADDR                  0x015F000
            #define SIF2_DRAM_SIZE                         0x8000       // 32KB

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE          0x0167000
            #define SPDIF_NONPCM_DRAM_SIZE                 0x36000      // 216KB

            #define OFFSET_HDMI_NONPCM_DRAM_BASE           0x019D000
            #define HDMI_NONPCM_DRAM_SIZE                  0xD8000      // 864KB

            #define OFFSET_SIF_OUTPCM_ADDR                 0x0275000
            #define SIF_OUTPCM_DRAM_SIZE                   0x2000       //8KB

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE            0x0000000
            #define DMA_READER_DRAM_SIZE                   0x20000      // 128KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE   0x0020000
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE   0x002C000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE           0x0C000      // 48KB

            #define OFFSET_PCM1_DRAM_ADDR                  0x0038000
            #define PCM1_DRAM_SIZE                         0xFF000      // 1020KB

            #define OFFSET_PCM2_DRAM_ADDR                  0x0137000
            #define PCM2_DRAM_SIZE                         0x32000      // 200KB

            #define OFFSET_SW_DMA_READER_DRAM_BASE         0x016D000
            #define SW_DMA_READER_DRAM_SIZE                0x10000      // 64KB

            #define OFFSET_MP3_ENC_DRAM_BASE               0x0000000
            #define MP3_ENC_DRAM_SIZE                      0x00000      // 0KB

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE           0x0000000
            #define OUTPUT_TEMP_DRAM_SIZE                  0x00000      // 0KB

            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE     0x0000000
            #define PCM_MIXER_ELEMENT_INFO_SIZE            0x00000      // 0KB

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE   0x0000000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE          0x00000      // 0KB

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE    0x0000000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE    0x0000000
            #define PCM_MIXER_MIXED_BUFFER_SIZE            0x00000      // 0KB

            #define OFFSET_MM_PCM_PRE_BUFFER_DRAM_ADDR     0x0000000
            #define MM_PCM_PRE_BUFFER_DRAM_SIZE            0x00000      // 0KB

#endif  //_DDR_CONFIG_H_
