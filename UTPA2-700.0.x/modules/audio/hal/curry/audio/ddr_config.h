#ifndef _DDR_CONFIG_H_
#define _DDR_CONFIG_H_

/************************************************
*  ½Ð«O«ù Utopia ©M DSP ¸Ìªº³o­ÓÀÉ®×¤@­P
*
*  1. µù¸Ñ¤£­n¥Î !
*  2. «Å§i¤£­n¥Î .const xxxx = ????;
************************************************/

/************************************************
*   DSP DDR memory layout
*************************************************/
    /* DSP DDR size */
        #define ADEC__R2_DDR_SIZE       0x7C0000             // 7.75 MB
        #define ASND__R2_DDR_SIZE       0x000000             // 0.00 MB
        #define ASND_DSP_DDR_SIZE       0x2C0000             // 2.75 MB
        #define COMMON_DDR_SIZE         0x580000             // 5.50 MB
                                                             // Total = 16 MB

        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE              0x0000000
            #define SPDIF_DLY_DRAM_SIZE                     0x18000                     //96KB


            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR            0x0036000                   //( 8 ch) * (3 bytes/per sample), 6400 sample, about 133.33ms (6400/48000)
            #define SE_MAIN_OUT_DRAM_SIZE                   0x38400                     //225KB

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR             0x006F000                   //(12 ch) * (3 bytes/per sample), 6144 sample, about 128ms (6144/48000)
            #define SE_MAIN_IN_DRAM_SIZE                    0x36000                     //216KB

            #define OFFSET_DDENC_METADATA_DRAM_ADDR         0x00A7400                   //48 * (PCM1_DRAM_SIZE/2/10/1536) if PCM1_DRAM_SIZE == PCM2_DRAM_SIZE
            #define DDENC_METADATA_DRAM_SIZE                0x660                       //

            //0x00A7A60
            //DM_Prefetch2   0x00A8000                                                  //For MS10-DDE/MS12-DDPE
            //HE-AAC Meta    0x00D8000
            //Surround       0x00DA000
            //KTV_Surround   0x00E2000


            #define OFFSET_SPDIF_NONPCM_DRAM_BASE           0x0102000                   //(36 frame) * ( 6144 = 0x1800)
            #define SPDIF_NONPCM_DRAM_SIZE                  0x36000                     //216KB

            #define OFFSET_HDMI_NONPCM_DRAM_BASE            0x0138000                   //(36 frame) * (24576 = 0x6000)
            #define HDMI_NONPCM_DRAM_SIZE                   0xD8000                     //864KB

            #define OFFSET_ES1_DRAM_ADDR                    0x0210000
            #define ES1_DRAM_SIZE                           0x20000                     //128KB

            #define OFFSET_ES2_DRAM_ADDR                    0x0230000
            #define ES2_DRAM_SIZE                           0x20000                     //128KB

            #define OFFSET_ES3_DRAM_ADDR                    0x0250000
            #define ES3_DRAM_SIZE                           0x20000                     //128KB

            #define OFFSET_ES4_DRAM_ADDR                    0x0270000
            #define ES4_DRAM_SIZE                           0x20000                     //128KB

            /* HP Delay */
            //HP delay              0x0290000
            //HP delay size         0x17FFF                                             //96k

            /* HDMI delay */
            #define SE_HDMI_DLY_DRAM_BASE                   0x02A8000
            #define SE_HDMI_DLY_DRAM_SIZE                   0x18000                     //96KB

            /* End */
            // 0x2C0000

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE             0x0000000
            #define DMA_READER_DRAM_SIZE                    0x20000                     //128KB

            #define OFFSET_SW_DMA_READER_DRAM_BASE          0x0020000
            #define SW_DMA_READER_DRAM_SIZE                 0x10000                     //64KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE    0x0030000
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE    0x003C000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE            0xC000                      //48KB

            #define OFFSET_PCM1_DRAM_ADDR                   0x0048000
            #define PCM1_DRAM_SIZE                          0xFF000                     //1020KB, DEC R2 10 channel PCM, 34 frame * 1536 sample per frame * 10 channel * sizeof(short)

            #define OFFSET_PCM2_DRAM_ADDR                   0x0147000
            #define PCM2_DRAM_SIZE                          0xFF000                     //1020KB

            #define OFFSET_MP3_ENC_DRAM_BASE                0x0246000
            #define MP3_ENC_DRAM_SIZE                       0x3000                      //12KB

            #define OFFSET_OUTPUT_TEMP_DRAM_BASE            0x0249000
            #define OUTPUT_TEMP_DRAM_SIZE                   0x10000                     //64KB

            #define OFFSET_PCM_MIXER_ELEMENT_INFO_BASE      0x0259000
            #define PCM_MIXER_ELEMENT_INFO_SIZE             0x1000                      //4KB

            #define OFFSET_PCM_MIXER_ELEMENT_BUFFER_BASE    0x025A000
            #define PCM_MIXER_ELEMENT_BUFFER_SIZE           0x78000                     //480KB

            #define OFFSET_PCM_MIXER1_MIXED_BUFFER_BASE     0x02D2000
            #define OFFSET_PCM_MIXER2_MIXED_BUFFER_BASE     0x02D6000
            #define PCM_MIXER_MIXED_BUFFER_SIZE             0x4000                      //16KB

            #define OFFSET_MM_PCM_PRE_BUFFER_DRAM_ADDR      0x02DA000
            #define MM_PCM_PRE_BUFFER_DRAM_SIZE             0x10000                     //64KB

            #define OFFSET_HW_DMA_READER2_DRAM_BASE         0x02EA000
            #define HW_DMA_READER2_DRAM_SIZE                0x10000                     //64KB

            #define OFFSET_SEDSP_DDPENC_MCHOUT_DRAM_ADDR    0x02FA000
            #define SEDSP_DDPENC_MCHOUT_DRAM_SIZE           0x12000                     //72KB , 4 frmae * 6ch * 1536samples * * sizeof(short)

            #define OFFSET_DDPENC_METADATA_DRAM_ADDR        0x030C000                   //This size must be (3066+28*2) bytes align, IIDK
            #define DDPENC_METADATA_DRAM_SIZE               0x03100                     //11.25KB, 4 frame * 16 bytes_align(3122) = 4 * 0xC40 = 0x3100

            #define DEC_METADATA1_DRAM_SIZE                 0x100
            #define DEC_METADATA2_DRAM_SIZE                 0x2640                      //288 * 34=0x2640, 288 * 34 * 6 = 0xE580
            #define DEC_METADATA3_DRAM_SIZE                 0x19778

            #define OFFSET_DEC1_METADATA1_DRAM_ADDR         (OFFSET_DDPENC_METADATA_DRAM_ADDR+DDPENC_METADATA_DRAM_SIZE) // 0x30F100
            #define OFFSET_DEC1_METADATA2_DRAM_ADDR         (OFFSET_DEC1_METADATA1_DRAM_ADDR+DEC_METADATA1_DRAM_SIZE)   // PCMR metadata // 0x30F200
            #define OFFSET_DEC1_METADATA3_DRAM_ADDR         (OFFSET_DEC1_METADATA2_DRAM_ADDR+DEC_METADATA2_DRAM_SIZE)   // Evo metadata // 0x311840

            #define OFFSET_DEC2_METADATA1_DRAM_ADDR         (OFFSET_DEC1_METADATA3_DRAM_ADDR+DEC_METADATA3_DRAM_SIZE) // 0x32AFB8
            #define OFFSET_DEC2_METADATA2_DRAM_ADDR         (OFFSET_DEC2_METADATA1_DRAM_ADDR+DEC_METADATA1_DRAM_SIZE) // 0x32B0B8
            #define OFFSET_DEC2_METADATA3_DRAM_ADDR         (OFFSET_DEC2_METADATA2_DRAM_ADDR+DEC_METADATA2_DRAM_SIZE) // 0x32D6F8, end: 0x346E70

            #define OFFSET_PCM_CAPTURE3_BUFFER_DRAM_BASE    0x0353000
            #define PCM_CAPTURE3_BUFFER_DRAM_SIZE           PCM_CAPTURE_BUFFER_DRAM_SIZE    //48KB

#endif  //_DDR_CONFIG_H_
