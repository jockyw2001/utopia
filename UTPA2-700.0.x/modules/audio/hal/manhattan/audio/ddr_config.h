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
        #define ADEC__R2_DDR_SIZE       0x7C0000             //7.75 MB
        #define ASND__R2_DDR_SIZE       0x0C0000             //0.75 MB
        #define ASND_DSP_DDR_SIZE       0x280000             //2.50 MB
        #define COMMON_DDR_SIZE         0x400000             //4.00 MB

        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE             0x0000000
            #define SPDIF_DLY_DRAM_SIZE                    0x18000

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR           0x003E000
            #define SE_MAIN_OUT_DRAM_SIZE                  0x39000

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR            0x0077000
            #define SE_MAIN_IN_DRAM_SIZE                   0x38400
 
            #define OFFSET_DDENC_METADATA_DRAM_ADDR        0x00B8000
            #define DDENC_METADATA_DRAM_SIZE               0x660       //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE


            /* HEAD PHONE delay */
            #define OFFSET_HEAD_PHONE_DLY_DRAM_BASE        0x00F0000
            #define HEAD_PHONE_DLY_DRAM_SIZE               0x24000                                                     //144KB

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE         0x0114000
            #define CH5_INPUT_DLY_DRAM_SIZE                0x24000                                                     //144KB

            /* CH6 input delay */
            #define OFFSET_CH6_INPUT_DLY_DRAM_BASE         0x0138000
            #define CH6_INPUT_DLY_DRAM_SIZE                0x24000                                                     //144KB

            #define OFFSET_ES1_DRAM_ADDR                   0x0160000
            #define ES1_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES2_DRAM_ADDR                   0x0180000
            #define ES2_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES3_DRAM_ADDR                   0x01A0000
            #define ES3_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES4_DRAM_ADDR                   0x01C0000
            #define ES4_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_SIF1_DRAM_ADDR                  0x01E0000
            #define SIF1_DRAM_SIZE                         0x8000                                                      // 32KB
            #define OFFSET_SIF2_DRAM_ADDR                  0x01E8000
            #define SIF2_DRAM_SIZE                         0x8000                                                      // 32KB

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE            0x0000000
            #define DMA_READER_DRAM_SIZE                   0x20000                                                     //128KB

            #define OFFSET_SW_DMA_READER_DRAM_BASE         0x0020000
            #define SW_DMA_READER_DRAM_SIZE                0x10000                                                     //64KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE   0x0030000
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE   0x003C000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE           0xC000                                                      //48KB

            #define OFFSET_PCM1_DRAM_ADDR                  0x0048000
            #define PCM1_DRAM_SIZE                         0xFF000                                                     //1020KB

            #define OFFSET_PCM2_DRAM_ADDR                  0x015A000
            #define PCM2_DRAM_SIZE                         0xFF000                                                     //1020KB

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE          0x0259000                                                   //216KB
            #define SPDIF_NONPCM_DRAM_SIZE                 0x36000

            #define OFFSET_MP3_ENC_DRAM_BASE               0x028F000                                                   //12KB
            #define MP3_ENC_DRAM_SIZE                      0x3000

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE           0x0292000                                                   //64KB
            #define OUTPUT_TEMP_DRAM_SIZE                  0x10000

            #define OFFSET_HDMI_NONPCM_DRAM_BASE           0x02A0000
            #define HDMI_NONPCM_DRAM_SIZE                  0xD8000

            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE     0x0378000
            #define PCM_MIXER_ELEMENT_INFO_SIZE            0x1000

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE   0x0379000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE          0x78000

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE    0x03F1000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE    0x03F5000
            #define PCM_MIXER_MIXED_BUFFER_SIZE            0x4000

#endif  //_DDR_CONFIG_H_
