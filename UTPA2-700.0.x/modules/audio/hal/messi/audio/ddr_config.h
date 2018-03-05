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
        #define ADEC__R2_DDR_SIZE       0x000000             //No ADEC R2
        #define ASND__R2_DDR_SIZE       0x000000             //No ASND R2
        #define DSP_DDR_SIZE            0x180000             //2.5MB
        #define COMMON_DDR_SIZE         0x100000             //2MB

        /* ASND-DSP DDR */
            /* SPDIF delay (GP C Bffer) */
            #define OFFSET_SPDIF_DLY_DRAM_BASE              0x0000000
            #define SPDIF_DLY_DRAM_SIZE                     0x18000         //64KB

            #define OFFSET_MSTAR_SURROUND_DRAM_ADDR         0x005C000
            #define MSTAR_SURROUND_DRAM_SIZE                0x03000         //32KB (use 12KB)

            #define OFFSET_KTV_SURROUND_DRAM_ADDR           OFFSET_MSTAR_SURROUND_DRAM_ADDR //Dummy
            #define KTV_SURROUND_DRAM_SIZE                  MSTAR_SURROUND_DRAM_SIZE        //Dummy

            #define OFFSET_SE_MAIN_OUT_DRAM_ADDR            0x0064000
            #define SE_MAIN_OUT_DRAM_SIZE                   0x39000         //228KB

            #define OFFSET_SE_MAIN_IN_DRAM_ADDR             0x00FD000
            #define SE_MAIN_IN_DRAM_SIZE                    0x39000         //228KB

            /* CH5 input delay */
            #define OFFSET_CH5_INPUT_DLY_DRAM_BASE          0x0136000
            #define CH5_INPUT_DLY_DRAM_SIZE                 0x12000         //72KB

            #define OFFSET_ES1_DRAM_ADDR                    0x009D000
            #define ES1_DRAM_SIZE_BYTE                      0x20000         //128KB

            #define OFFSET_SIF1_DRAM_ADDR                   OFFSET_ES1_DRAM_ADDR
            #define SIF1_DRAM_SIZE                          0x8000                                      // 32KB
            #define OFFSET_SIF2_DRAM_ADDR                   (OFFSET_SIF1_DRAM_ADDR+SIF1_DRAM_SIZE)
            #define SIF2_DRAM_SIZE                          0x8000                                      // 32KB

            #define OFFSET_DM_PREFETCH_DRAM_ADDR            0x00BD000
            #define DM_PREFETCH_DRAM_SIZE_BYTE              0x040000        // 256KB

            #define OFFSET_DM_PREFETCH2_DRAM_ADDR           0               // Dummy
            #define DM_PREFETCH2_DRAM_SIZE_BYTE             0               // Dummy

        /* Common DDR */
            #define OFFSET_DMA_READER_DRAM_BASE             0x0000000
            #define DMA_READER_DRAM_SIZE                    0x20000         //128KB

            #define OFFSET_SW_DMA_READER_DRAM_BASE          0x0020000
            #define SW_DMA_READER_DRAM_SIZE                 0x10000         //64KB

            #define OFFSET_PCM_CAPTURE1_BUFFER_DRAM_BASE    0x0030000
            #define OFFSET_PCM_CAPTURE2_BUFFER_DRAM_BASE    0x003C000
            #define PCM_CAPTURE_BUFFER_DRAM_SIZE            0xC000          //48KB

            #define OFFSET_SPDIF_NONPCM_DRAM_BASE           0x0048000
            #define SPDIF_NONPCM_DRAM_SIZE                  0x66000         //408KB

            #define OFFSET_PCM1_DRAM_ADDR                   0x00AE000
            #define PCM1_DRAM_SIZE_BYTE                     0x66000         //408KB

#endif  //_DDR_CONFIG_H_
