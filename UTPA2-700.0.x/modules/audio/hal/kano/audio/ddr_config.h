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
        #define ADEC__R2_DDR_SIZE       0x5A0000             // 5.625  MB
        #define ASND__R2_DDR_SIZE       0x2E0000             // 2.875  MB
        #define ASND_DSP_DDR_SIZE       0x2E0000             // 2.875 MB
        #define COMMON_DDR_SIZE         0x4A0000             // 4.625 MB
                                                             // Total = 16 MB
        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE             0x0000000
            #define SPDIF_DLY_DRAM_SIZE                    0x18000

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR            0x0036000
            #define SE_MAIN_IN_DRAM_SIZE                   0x34800      //210KB, (106ms) multiple of (SE_BUFF2_DMA_24BIT_LINE_SIZE*BYTES_IN_MIU_LINE)

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR           0x006C000
            #define SE_MAIN_OUT_DRAM_SIZE                  0x39000

            #define OFFSET_DDENC_METADATA_DRAM_ADDR        0x00A5000
            #define DDENC_METADATA_DRAM_SIZE               0x660       //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE

            #define OFFSET_MSTAR_SURROUND_DRAM_ADDR        0x00A8000
            #define MSTAR_SURROUND_DRAM_SIZE               0x0008000                                                   //32KB (use 12KB)

            #define OFFSET_KTV_SURROUND_DRAM_ADDR          OFFSET_MSTAR_SURROUND_DRAM_ADDR
            #define KTV_SURROUND_DRAM_SIZE                 MSTAR_SURROUND_DRAM_SIZE                                    //KB

            #define OFFSET_DM_PREFETCH_DRAM_ADDR           0x00B0000
            #define DM_PREFETCH_DRAM_SIZE                  0x10000                                                     //64KB

            /* HDMI delay*/
            #define SE_HDMI_DLY_DRAM_BASE                  0x00C0000
            #define SE_HDMI_DLY_DRAM_SIZE                  0x18000                                                      //96KB

            #define OFFSET_ES1_DRAM_ADDR                   0x00E2000
            #define ES1_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES2_DRAM_ADDR                   0x0102000
            #define ES2_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES3_DRAM_ADDR                   0x0122000
            #define ES3_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_ES4_DRAM_ADDR                   0x0142000
            #define ES4_DRAM_SIZE                          0x20000                                                     //128KB

            #define OFFSET_SER2_DDENC_MCHOUT_DRAM_ADDR     0x0162000
            #define SER2_DDENC_MCHOUT_DRAM_SIZE            0xD800           //256 sample * 2byte * 6ch * 18

            #define OFFSET_SER2_OUTPCM_DMX_DRAM_ADDR       0x016F800
            #define SER2_OUTPCM_DMX_DRAM_SIZE              0x4800           //256 sample * 2byte * 2ch * 18

            #define OFFSET_DEC1_METADATA1_DRAM_ADDR        0x0174000
            #define OFFSET_DEC1_METADATA2_DRAM_ADDR        0x0174100
            #define OFFSET_DEC1_METADATA3_DRAM_ADDR        0x0176740

            #define OFFSET_DEC2_METADATA1_DRAM_ADDR        0x018FF00
            #define OFFSET_DEC2_METADATA2_DRAM_ADDR        0x0190000
            #define OFFSET_DEC2_METADATA3_DRAM_ADDR        0x0192640

            #define DEC_METADATA1_DRAM_SIZE                0x100
            #define DEC_METADATA2_DRAM_SIZE                0x2640   //288 * 34=0x2640, 288 * 34 * 6 = 0xE580
            #define DEC_METADATA3_DRAM_SIZE                0x19778

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE          0x01AC000                                                   //216KB
            #define SPDIF_NONPCM_DRAM_SIZE                 0x36000

            #define OFFSET_HDMI_NONPCM_DRAM_BASE           0x01E2000                                                   //864KB
            #define HDMI_NONPCM_DRAM_SIZE                  0xD8000

            /* HEAD PHONE delay */
            #define OFFSET_HEAD_PHONE_DLY_DRAM_BASE        0x0000000
            #define HEAD_PHONE_DLY_DRAM_SIZE               0x00000                                                     //0KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* multi-channel input delay */
            #define OFFSET_MULTI_CH_INPUT_DLY_DRAM_BASE    0x0000000
            #define MULTI_CH_INPUT_DLY_DRAM_SIZE           0x00000      // 6ch (256 ms), can be adjusted by request    //216KB, multiple of (MULTI_CH_INPUT_DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE         0x0000000
            #define CH5_INPUT_DLY_DRAM_SIZE                0x00000                                                      //72KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* CH6 input delay */
            #define OFFSET_CH6_INPUT_DLY_DRAM_BASE         0x0000000
            #define CH6_INPUT_DLY_DRAM_SIZE                0x00000                                                     //0KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE            0x0000000
            #define DMA_READER_DRAM_SIZE                   0x20000                                                     //128KB

            #define OFFSET_SW_DMA_READER_DRAM_BASE         0x0020000
            #define SW_DMA_READER_DRAM_SIZE                0x10000                                                     // 64KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE   0x0030000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE           0x18000                                                     // 96KB

            #define OFFSET_PCM1_DRAM_ADDR                  0x0048000
            #define PCM1_DRAM_SIZE                         0xFF000                                                    //216KB

            #define OFFSET_PCM2_DRAM_ADDR                  0x0147000
            #define PCM2_DRAM_SIZE                         0xFF000                                                     //324KB

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE           0x0246000                                                   //64KB
            #define OUTPUT_TEMP_DRAM_SIZE                  0x10000

            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE     0x0256000
            #define PCM_MIXER_ELEMENT_INFO_SIZE            0x1000

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE   0x0257000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE          0x78000

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE    0x02CF000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE    0x02D3000
            #define PCM_MIXER_MIXED_BUFFER_SIZE            0x4000

            #define OFFSET_MM_PCM_PRE_BUFFER_DRAM_ADDR     0x02D7000
            #define MM_PCM_PRE_BUFFER_DRAM_SIZE            0x10000                                                     //64KB

            #define OFFSET_HW_DMA_READER2_DRAM_BASE        0x02E7000
            #define HW_DMA_READER2_DRAM_SIZE               0x10000                                                     //64KB

            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE   0x02F7000
            #define PCM_CAPTURE2_BUFFER_DRAM_SIZE          0xC000                                                      //48KB

            #define OFFSET_PCM_CAPTURE3_BUFFER_DRAM_BASE   0x0303000
            #define PCM_CAPTURE3_BUFFER_DRAM_SIZE          0xC000                                                      //48KB

            #define OFFSET_MP3_ENC_DRAM_BASE               0x030F000                                                   //28KB
            #define MP3_ENC_DRAM_SIZE                      0x7000

            #define OFFSET_MP3_ENC2_DRAM_BASE              0x0316000                                                   //28KB
            #define MP3_ENC2_DRAM_SIZE                     0x7000

            #define OFFSET_MP3_ENC_APTS_DRAM_BASE          0x031D000
            #define OFFSET_MP3_ENC_APTS_DRAM_SIZE          0x100                                                       //0.25KB

#endif  //_DDR_CONFIG_H_
