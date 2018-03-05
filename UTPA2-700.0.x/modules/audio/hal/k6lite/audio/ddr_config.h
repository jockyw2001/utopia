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
        #define ADEC__R2_DDR_SIZE       0x110000             // 1.0625 MB
        #define ASND__R2_DDR_SIZE       0x000000             // 0.000 MB
        #define ASND_DSP_DDR_SIZE       0x0E0000             // 0.875 MB
        #define COMMON_DDR_SIZE         0x160000             // 1.375 MB
                                                             // Total = 3.3125 MB

        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE              0x0000000
            #define SPDIF_DLY_DRAM_SIZE                     0x18000                     //96KB

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR            0x0036000                   //( 8 ch) * (3 bytes/per sample), 6400 sample, about 133.33ms (6400/48000)
            #define SE_MAIN_OUT_DRAM_SIZE                   0x38400                     //225KB

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR             0x006F000                   //(12 ch) * (3 bytes/per sample), 6144 sample, about 128ms (6144/48000)
            #define SE_MAIN_IN_DRAM_SIZE                    0x36000			            //216KB

            #define OFFSET_ES1_DRAM_ADDR                    0x00A8000
            #define ES1_DRAM_SIZE                           0x0020000                                                     //128KB
            
            /* HDMI delay*/
            #define SE_HDMI_DLY_DRAM_BASE                   0x0C8000
            #define SE_HDMI_DLY_DRAM_SIZE                   0x018000                                                      //96KB

            #define DSP_DUMMY_BUFFER_BASE                       0x0E0000           
            
            #define OFFSET_KTV_SURROUND_DRAM_ADDR           DSP_DUMMY_BUFFER_BASE
            #define KTV_SURROUND_DRAM_SIZE                  0x0020000                   //128KB            
            
            #define OFFSET_MSTAR_SURROUND_DRAM_ADDR         DSP_DUMMY_BUFFER_BASE
            #define MSTAR_SURROUND_DRAM_SIZE                0x0008000                   //32KB (use 12KB)            
            
            #define OFFSET_DDENC_METADATA_DRAM_ADDR         DSP_DUMMY_BUFFER_BASE                   //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE
            #define DDENC_METADATA_DRAM_SIZE                0x660                       // //For MS10-DDE/MS12-DDPE

            #define OFFSET_DM_PREFETCH_DRAM_ADDR            DSP_DUMMY_BUFFER_BASE
            #define DM_PREFETCH_DRAM_SIZE                   0x10000                                                     //64KB

            #define OFFSET_HEAAC_METADATA_DRAM_ADDR         DSP_DUMMY_BUFFER_BASE
            #define HEAAC_METADATA_DRAM_SIZE                0x002000

            #define OFFSET_ES2_DRAM_ADDR                    DSP_DUMMY_BUFFER_BASE
            #define ES2_DRAM_SIZE                           0x10000                                                     //128KB

            #define OFFSET_ES3_DRAM_ADDR                   DSP_DUMMY_BUFFER_BASE
            #define ES3_DRAM_SIZE                          0x10000                                                     //128KB

            #define OFFSET_ES4_DRAM_ADDR                   DSP_DUMMY_BUFFER_BASE
            #define ES4_DRAM_SIZE                          0x10000                                                     //128KB

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE          DSP_DUMMY_BUFFER_BASE                                                   //216KB
            #define SPDIF_NONPCM_DRAM_SIZE                 0x18000

            #define OFFSET_HDMI_NONPCM_DRAM_BASE           DSP_DUMMY_BUFFER_BASE                                                   //864KB
            #define HDMI_NONPCM_DRAM_SIZE                  0x18000

            /* HEAD PHONE delay */
            #define OFFSET_HEAD_PHONE_DLY_DRAM_BASE         0x0000000
            #define HEAD_PHONE_DLY_DRAM_SIZE                0x00000                                                     //0KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* multi-channel input delay */
            #define OFFSET_MULTI_CH_INPUT_DLY_DRAM_BASE     0x0000000
            #define MULTI_CH_INPUT_DLY_DRAM_SIZE            0x00000      // 6ch (256 ms), can be adjusted by request    //0KB, multiple of (MULTI_CH_INPUT_DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE          0x0000000
            #define CH5_INPUT_DLY_DRAM_SIZE                 0x00000                                                      //0KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

            /* CH6 input delay */
            #define OFFSET_CH6_INPUT_DLY_DRAM_BASE          0x0000000
            #define CH6_INPUT_DLY_DRAM_SIZE                 0x00000                                                     //0KB, multiple of (DELAY_DMA_LINESIZE*BYTES_IN_MIU_LINE)

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE             0x0000000
            #define DMA_READER_DRAM_SIZE                    0x20000                     //128KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE    0x0020000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE            0x18000                                                     // 96KB
                        
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE    0x0038000
            #define PCM_CAPTURE2_BUFFER_DRAM_SIZE           0xC000                                                     // 48KB

            #define OFFSET_PCM1_DRAM_ADDR                   0x0044000
            #define PCM1_DRAM_SIZE                          0x36000                     //1020KB, DEC R2 10 channel PCM, 34 frame * 1536 sample per frame * 10 channel * sizeof(short)
            
            #define OFFSET_MM_PCM_PRE_BUFFER_DRAM_ADDR      0x007A000
            #define MM_PCM_PRE_BUFFER_DRAM_SIZE             0x10000                                                     //64KB

            #define OFFSET_HW_DMA_READER2_DRAM_BASE         0x008A000
            #define HW_DMA_READER2_DRAM_SIZE                0x10000                                                     //64KB            
            
            #define OFFSET_DDPENC_METADATA_DRAM_ADDR        0x009A000                   //This size must be (3066+28*2) bytes align, IIDK
            #define DDPENC_METADATA_DRAM_SIZE               0x03100                     //11.25KB, 4 frame * 16 bytes_align(3122) = 4 * 0xC40 = 0x3100

            #define DEC_METADATA1_DRAM_SIZE                 0x100
            #define DEC_METADATA2_DRAM_SIZE                 0x2640                      //288 * 34=0x2640, 288 * 34 * 6 = 0xE580
            #define DEC_METADATA3_DRAM_SIZE                 0x19778

            #define OFFSET_DEC1_METADATA1_DRAM_ADDR         (OFFSET_DDPENC_METADATA_DRAM_ADDR+DDPENC_METADATA_DRAM_SIZE) // 0x032D100
            #define OFFSET_DEC1_METADATA2_DRAM_ADDR         (OFFSET_DEC1_METADATA1_DRAM_ADDR+DEC_METADATA1_DRAM_SIZE)   // PCMR metadata // 0x032D200
            #define OFFSET_DEC1_METADATA3_DRAM_ADDR         (OFFSET_DEC1_METADATA2_DRAM_ADDR+DEC_METADATA2_DRAM_SIZE)   // Evo metadata // 0x032F840

            #define OFFSET_DEC2_METADATA1_DRAM_ADDR         (OFFSET_DEC1_METADATA3_DRAM_ADDR+DEC_METADATA3_DRAM_SIZE) // 0x0348FB8
            #define OFFSET_DEC2_METADATA2_DRAM_ADDR         (OFFSET_DEC2_METADATA1_DRAM_ADDR+DEC_METADATA1_DRAM_SIZE) // 0x03490B8
            #define OFFSET_DEC2_METADATA3_DRAM_ADDR         (OFFSET_DEC2_METADATA2_DRAM_ADDR+DEC_METADATA2_DRAM_SIZE) // 0x034B6F8, end: 0x0364E70            
            
            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE      0x00C7000
            #define PCM_MIXER_ELEMENT_INFO_SIZE             0x1000                      //4KB

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE    0x00C8000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE           0x78000                     //480KB

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE     0x0140000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE     0x0144000
            #define PCM_MIXER_MIXED_BUFFER_SIZE             0x4000                      //16KB            
            
            #define COMM_DUMMY_BUFFER_BASE                  0x0228000
            
            #define OFFSET_SW_DMA_READER_DRAM_BASE          COMM_DUMMY_BUFFER_BASE
            #define SW_DMA_READER_DRAM_SIZE                 0                                                     // 64KB            

            #define OFFSET_PCM2_DRAM_ADDR                   COMM_DUMMY_BUFFER_BASE
            #define PCM2_DRAM_SIZE                          0                     //1020KB

            #define OFFSET_MP3_ENC_DRAM_BASE                COMM_DUMMY_BUFFER_BASE
            #define MP3_ENC_DRAM_SIZE                       0                      //12KB

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE            COMM_DUMMY_BUFFER_BASE
            #define OUTPUT_TEMP_DRAM_SIZE                   0                     //64KB

            #define OFFSET_PCM_CAPTURE3_BUFFER_DRAM_BASE    COMM_DUMMY_BUFFER_BASE
            #define PCM_CAPTURE3_BUFFER_DRAM_SIZE           0    //48KB

            #define OFFSET_SEDSP_DDPENC_MCHOUT_DRAM_ADDR    COMM_DUMMY_BUFFER_BASE
            #define SEDSP_DDPENC_MCHOUT_DRAM_SIZE           0                     //72KB , 4 frmae * 6ch * 1536samples * * sizeof(short)



#endif  //_DDR_CONFIG_H_
