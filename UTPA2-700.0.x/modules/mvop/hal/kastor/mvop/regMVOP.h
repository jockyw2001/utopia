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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_VOP_H_
#define _REG_VOP_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Base Address
//------------------------------------------------------------------------------
#define MVOP_REG_BASE                           0x1400  // 0x1400 - 0x14FF
#define MVOP_SUB_REG_BASE                       0x3D00  // 0x1300 - 0x13FF
#define CHIP_REG_BASE                           0x0B00  //chiptop CLKGEN0
#define CLKGEN1_REG_BASE                        0x3300  //chiptop CLKGEN01


//------------------------------------------------------------------------------
// MVOP Reg
//------------------------------------------------------------------------------
#define VOP_FRAME_VCOUNT                        (MVOP_REG_BASE + 0x00)
#define VOP_FRAME_HCOUNT                        (MVOP_REG_BASE + 0x02)
#define VOP_VB0_STR                             (MVOP_REG_BASE + 0x04)
#define VOP_VB0_END                             (MVOP_REG_BASE + 0x06)
#define VOP_VB1_STR                             (MVOP_REG_BASE + 0x08)
#define VOP_VB1_END                             (MVOP_REG_BASE + 0x0A)
#define VOP_TF_STR                              (MVOP_REG_BASE + 0x0C)
#define VOP_BF_STR                              (MVOP_REG_BASE + 0x0E)
#define VOP_HACT_STR                            (MVOP_REG_BASE + 0x10)
#define VOP_IMG_HSTR                            (MVOP_REG_BASE + 0x12)
#define VOP_IMG_VSTR0                           (MVOP_REG_BASE + 0x14)
#define VOP_IMG_VSTR1                           (MVOP_REG_BASE + 0x16)
#define VOP_TF_VS                               (MVOP_REG_BASE + 0x18)
#define VOP_BF_VS                               (MVOP_REG_BASE + 0x1A)


///TOP field Vsync start line number to MVD
#define VOP_TF_VS_MVD                           (MVOP_REG_BASE + 0x1C) //u3 new
///Bottom field Vsync start line number to MVD
#define VOP_BF_VS_MVD                           (MVOP_REG_BASE + 0x1E) //u3 new

    #define VOP_FSYNC_EN    BIT4    //frame sync enable
#define VOP_CTRL0                               (MVOP_REG_BASE + 0x22)
    #define VOP_EXTFLD_EN   BIT6

    #define VOP_FLIP_UV     BIT0
    #define VOP_FLIP_YC     BIT1
    #define VOP_FLD_INV     BIT2
    #define VOP_OFLD_INV    BIT4
    #define VOP_CCIR_MD     BIT5
    #define VOP_MVD_VS_MD   BIT6   //u3 new 0: Original mode;1: Use new vsync (tf_vs_mvd, bf_vs_mvd)
    #define VOP_MVD_VS_SEL  BIT7
#define VOP_CTRL1                               (MVOP_REG_BASE + 0x23)

#define VOP_TST_IMG                             (MVOP_REG_BASE + 0x24)
    #define VOP_SC_VS_INV   BIT7

#define VOP_U_PAT                               (MVOP_REG_BASE + 0x26)

    #define VOP_DMA_THD      (BIT0|BIT1|BIT2|BIT3|BIT4)
            //DMA FIFO threshold
            //= reg_dma_thd x 2 (reg_miu128b=1)
            //= reg_dma_thd x 4 (reg_miu128b=0)
    #define VOP_BURST_ST_SEL BIT7
            //Timing to calculate burst length (only valid when reg_burst_ext = all)
            //0: at mi2dc_rdy; 1: at dc2mi_rdy
#define VOP_DMA0                                (MVOP_REG_BASE + 0x28)  //t3 new

    #define VOP_BURST_EXT    (BIT0|BIT1|BIT2)
            //DMA burst length
            //0:  4 (reg_miu128b=1),   8 (reg_miu128b=0)
            //1:  8 (reg_miu128b=1),  16 (reg_miu128b=0)
            //2: 16 (reg_miu128b=1),  32 (reg_miu128b=0)
            //3: 24 (reg_miu128b=1),  48 (reg_miu128b=0)
            //4: 32 (reg_miu128b=1),  64 (reg_miu128b=0)
            //5: 48 (reg_miu128b=1),  96 (reg_miu128b=0)
            //6: 64 (reg_miu128b=1), 128 (reg_miu128b=0)
            //7: all
    #define VOP_HI_TSH       (BIT3|BIT4|BIT5|BIT6) //DMA High priority threshold
            //(assert high priority if data count less then reg_hi_tsh x 8)
    #define VOP_FORCE_HIGH   BIT7 //Force DMA High priority
#define VOP_DMA1                                (MVOP_REG_BASE + 0x29)  //t3 new
#define VOP_BURST_CTRL0                          (MVOP_REG_BASE + 0x2A) //monaco new
#define VOP_BURST_CTRL1                          (MVOP_REG_BASE + 0x2B)  //monaco new

#define VOP_DC_STRIP_H                          (MVOP_REG_BASE + 0x30)
    #define VOP_SRAM_SD_MASK          BIT3
    #define MFDEC_SRAM_SD_MASK        BIT4

#define VOP_INT_MASK                            (MVOP_REG_BASE + 0x3E)
#define VOP_MPG_JPG_SWITCH                      (MVOP_REG_BASE + 0x40)
    #define VOP_DRAM_RD_MODE    BIT5
#define VOP_DC_STRIP                            (MVOP_REG_BASE + 0x41)
#define VOP_JPG_YSTR0_L                         (MVOP_REG_BASE + 0x42)
#define VOP_JPG_YSTR0_H                         (MVOP_REG_BASE + 0x44)
#define VOP_JPG_UVSTR0_L                        (MVOP_REG_BASE + 0x46)
#define VOP_JPG_UVSTR0_H                        (MVOP_REG_BASE + 0x48)
    #define VOP_YUV_STR_HIBITS (BIT4 | BIT3 | BIT2 |BIT1 | BIT0)        //Bits(28:24)

#define VOP_JPG_HSIZE                           (MVOP_REG_BASE + 0x4A)
#define VOP_JPG_VSIZE                           (MVOP_REG_BASE + 0x4C)

    #define VOP_LOAD_REG        BIT0 //load new value into active registers 0x20-0x26
    #define VOP_TILE_FORMAT     BIT1 //0: 8x32, 1: 16x32
    #define VOP_BUF_DUAL        BIT2
    #define VOP_FORCELOAD_REG   BIT4 //force load registers
#define VOP_REG_WR                              (MVOP_REG_BASE + 0x4E)

#define VOP_INPUT_SWITCH0                        (MVOP_REG_BASE + 0x50)
    #define VOP_R2_WISHBONE      BIT6
    #define EVD_ENABLE           BIT7

#define VOP_INPUT_SWITCH1                        (MVOP_REG_BASE + 0x51)
    #define VOP_MVD_EN          BIT0 //t8 new
    #define VOP_H264_PUREY      BIT1
    //#define VOP_RVD_EN          BIT2  //a3: removed
    #define VOP_HVD_EN          BIT3
    #define VOP_FORCE_SC_RDY    BIT4 //u3 new: force sc2mvop_rdy = 1
    #define VOP_DEBUG_SEL       (BIT5 | BIT6 | BIT7) //u3 new: MVOP debug out select

    #define VOP_RAMAP_LUMA_VAL  0x1f
    #define VOP_RAMAP_LUMA_EN   BIT7
#define VOP_RAMAP_LUMA                          (MVOP_REG_BASE + 0x52)
//u3 new: Luma range mapping for VC1 (value = 8~16)

    #define VOP_RAMAP_CHROMA_VAL  0x1f
    #define VOP_RAMAP_CHROMA_EN   BIT7
#define VOP_RAMAP_CHROMA                        (MVOP_REG_BASE + 0x53)
//u3 new: Chroma range mapping for VC1 (value = 8~16)

// [T3 new
#define VOP_DEBUG_2A                            (MVOP_REG_BASE + 0x54) //2-byte
#define VOP_DEBUG_2B                            (MVOP_REG_BASE + 0x56)
#define VOP_DEBUG_2B_H                            (MVOP_REG_BASE + 0x57)
    #define VOP_SEND_DATA_FLAG                 BIT7
#define VOP_DEBUG_2C                            (MVOP_REG_BASE + 0x58)
#define VOP_DEBUG_2D                            (MVOP_REG_BASE + 0x5A)
#define VOP_DEBUG_2D_H                            (MVOP_REG_BASE + 0x5B)
		#define VOP_DMA_STATUS                 (BIT7 | BIT6 | BIT5 | BIT4)
#define VOP_DEBUG_2E                            (MVOP_REG_BASE + 0x5C)

    #define VOP_UF                BIT0 //buf underflow
    #define VOP_OF                BIT1 //buf overflow
#define VOP_DEBUG_2F_L                          (MVOP_REG_BASE + 0x5E)

    #define VOP_BIST_FAIL         BIT0 //YUV fifo bist fail
    #define VOP_RIU_DEBUG_SEL     (BIT6|BIT7) //RIU debug register select
#define VOP_DEBUG_2F_H                          (MVOP_REG_BASE + 0x5F)
// ]

#define VOP_UV_SHIFT                            (MVOP_REG_BASE + 0x60)

    #define VOP_GCLK_MIU_ON     BIT2 //clk_miu use 0: free-run clock; 1: gated clock
    #define VOP_GCLK_VCLK_ON    BIT3 //clk_dc0 use 0: free-run clock; 1: gated clock
#define VOP_GCLK                                (MVOP_REG_BASE + 0x60) //u3 new

#define VOP_MIU_IF                              (MVOP_REG_BASE + 0x60)
    #define VOP_MIU_128BIT      BIT4 //MIU bus use 0: 64bit 1:128bit
    #define VOP_MIU_128B_I64    BIT5
    #define VOP_MIU_REQ_DIS     BIT6

#define VOP_MIU_BUS                             (MVOP_REG_BASE + 0x60) //t3 new
#define VOP_FD_MASK                             (MVOP_REG_BASE + 0x61) //kappa new
    #define VOP_FD_MASK_CLR     BIT2
    #define VOP_FD_MASK_INV     BIT3

#define VOP_MIU_SEL                             (MVOP_REG_BASE + 0x61) //kaiser new
    #define VOP_MSB_MIU_DIFF                 BIT0
    #define VOP_LSB_MIU_DIFF                 BIT1
    #define VOP_MSB_BUF0_MIU_SEL            (BIT4|BIT5)  // Y miu select: miu0~3 = 0x0~0x3
    #define VOP_MSB_BUF1_MIU_SEL            (BIT6|BIT7)  // UV miu select: miu0~3 = 0x0~0x3

#define VOP_JPG_YSTR1_L                         (MVOP_REG_BASE + 0x62)
#define VOP_JPG_YSTR1_H                         (MVOP_REG_BASE + 0x64)
#define VOP_JPG_UVSTR1_L                        (MVOP_REG_BASE + 0x66)
#define VOP_JPG_UVSTR1_H                        (MVOP_REG_BASE + 0x68)

#define VOP_SYNC_FRAME_V                        (MVOP_REG_BASE + 0x6A)
#define VOP_SYNC_FRAME_H                        (MVOP_REG_BASE + 0x6C)

#define VOP_INFO_FROM_CODEC_L                   (MVOP_REG_BASE + 0x70)
    #define VOP_INFO_FROM_CODEC_BASE_ADDR    (BIT0) //base address
    #define VOP_INFO_FROM_CODEC_PITCH        (BIT1) //pitch
    #define VOP_INFO_FROM_CODEC_SIZE         (BIT2) //size
    #define VOP_INFO_FROM_CODEC_PROG_SEQ     (BIT3) //progressive sequence
    #define VOP_INFO_FROM_CODEC_FIELD        (BIT4) //field
    #define VOP_INFO_FROM_CODEC_RANGE_MAP    (BIT5) //range map
    #define VOP_INFO_FROM_CODEC_COMP_MODE    (BIT6) //compression mode
    #define VOP_INFO_FROM_CODEC_422_FMT      (BIT7) //422 format

#define VOP_INFO_FROM_CODEC_H                   (MVOP_REG_BASE + 0x71)
    #define VOP_INFO_FROM_CODEC_DUAL_BUFF    (BIT0) //dual buffer flag
    #define VOP_INFO_FROM_CODEC_BPIC_REDUCT  (BIT1) //bpic reduction
    #define VOP_INFO_FROM_CODEC_MIU_BUF0_SEL      (BIT4) //MSB miu select
    #define VOP_INFO_FROM_CODEC_MIU_BUF1_SEL      (BIT5) //LSB miu select
    #define VOP_INFO_FROM_CODEC_10BIT      (BIT6) //10 bits enable
    #define VOP_INFO_FROM_CODEC_DS_IDX        (BIT7) //dynamic scaling index

#define VOP_EVD_10B_EN                   (MVOP_REG_BASE + 0x73)
#define VOP_INT_TYPE                      (MVOP_REG_BASE + 0x73)
    #define VOP_EVD_INT_SEP    (BIT0)
    #define VOP_EVD_10B_Y_EN   (BIT1) //Enable EVD Y 10 bits mode
    #define VOP_EVD_10B_UV_EN  (BIT2) //Enable EVD UV 10 bits mode

#define VOP_NOT_WAIT_READ_DATA                   (MVOP_REG_BASE + 0x72)
    #define VOP_NOT_WAIT_RDLAT              (BIT0|BIT1|BIT2)

#define VOP_MIU_SEL_LSB                   (MVOP_REG_BASE + 0x75)
    #define VOP_LSB_BUF0_MIU_SEL            (BIT4|BIT5)  // LSB Y miu select: miu0~3 = 0x0~0x3
    #define VOP_LSB_BUF1_MIU_SEL            (BIT6|BIT7)  // LSB UV miu select: miu0~3 = 0x0~0x3

#define VOP_MIRROR_CFG                          (MVOP_REG_BASE + 0x76)
    #define VOP_MIRROR_CFG_VEN    (BIT0) //vertical mirror enable
    #define VOP_MIRROR_CFG_HEN    (BIT1) //horizontal mirror enable
    #define VOP_FIELD_FROM_ADDR   (BIT3) //decide top or bot by diu addr
    #define VOP_HW_FLD_BASE       (BIT5) //Hardware calculate field jump base address
    #define VOP_MASK_BASE_LSB     (BIT7) //mask LSB of base address from Codec (always get top field base address)
    #define VOP_MIRROR_CFG_ENABLE (BIT3 | BIT4 | BIT5 | BIT6 | BIT7)

#define VOP_MIRROR_CFG_HI                       (MVOP_REG_BASE + 0x77)
    #define VOP_REF_SELF_FLD      (BIT1) //source field flag set by internal timing generator
    #define VOP_HK_MASK            (BIT4) // HSK show back ground as vdec not ready

#define VOP_MULTI_WIN_CFG0                      (MVOP_REG_BASE + 0x78)
    #define VOP_LR_BUF_MODE       (BIT0) //3D L/R dual buffer mode
    #define VOP_P2I_MODE          (BIT1) //progressive input, interlace output
                                         //to SC vsync is twice of to MVD vsync
    #define VOP_LR_LA_OUT         (BIT2) //3D L/R dual buffer line alternative output
    #define VOP_LR_LA2SBS_OUT     (BIT3) //3D L/R dual buffer line alternative read, side-by-side output
    #define VOP_LR_DIFF_SIZE      (BIT7) //3D L/R dual buffer with difference size

#define VOP_RGB_FMT                             (MVOP_REG_BASE + 0x79)
    #define VOP_RGB_FMT_565       (BIT0) //RGB 565
    #define VOP_RGB_FMT_888       (BIT1) //RGB 888
    #define VOP_RGB_FMT_SEL       (BIT0 | BIT1) //RGB format selection

#define VOP_REG_DUMMY_3D_0                             (MVOP_REG_BASE + 0x7A)
#define VOP_REG_DUMMY_3D_1                             (MVOP_REG_BASE + 0x7B)
    #define VOP_RGB_FILED_BYPASS       (BIT3) // TOP/BOT bypass to xc
#define VOP_REG_DUMMY                           (MVOP_REG_BASE + 0x7B)
    #define VOP_32x32_WB          (BIT6) //32x32 from vdec: Reg_evd_en =  0 + reg_diu_sel = 1
    #define VOP_420_BW_SAVE       (BIT7) //420 bw saving mode

#define VOP_HANDSHAKE                             (MVOP_REG_BASE + 0x7C)
    #define VOP_HANDSHAKE_MODE       (BIT0) //Handshake interface output
    #define VOP_TRIG_FROM_XC      (BIT1) //Trigger start from Scaler
    #define VOP_VSUNC_FROM_XC      (BIT2) //Vsync from Scaler
    #define VOP_HANDSHAKE_ENABLE      (BIT0 | BIT1 | BIT2)

#define VOP_REG_STRIP_ALIGN                     (MVOP_REG_BASE + 0x7E)
#define VOP_REG_WEIGHT_CTRL                     (MVOP_REG_BASE + 0x7E)
#define VOP_REG_422_OUT_EN                      (MVOP_REG_BASE + 0x7F)  //BIT 8
#define VOP_REG_FD_MASK_SEL                      (MVOP_REG_BASE + 0x7F)  //BIT 8
#define VOP_REG_FRAME_RST                       (MVOP_REG_BASE + 0x7E)  //BIT15
#define VOP_REG_CSC_EN                           (MVOP_REG_BASE + 0x7E)  //BIT15

#define VOP_REG_CROP_HSTART			(MVOP_REG_BASE + 0x80)
#define VOP_REG_CROP_VSTART			(MVOP_REG_BASE + 0x82)
#define VOP_REG_CROP_HSIZE			(MVOP_REG_BASE + 0x84)
#define VOP_REG_CROP_VSIZE			(MVOP_REG_BASE + 0x86)
#define VOP_REG_SINGLE_3D_L			(MVOP_REG_BASE + 0x88)
    #define VOP_FORCE_SKIP             BIT2
    #define VOP_SKIP_SIZE_LVIEW       (BIT4 | BIT5 | BIT6 | BIT7)
#define VOP_REG_SINGLE_3D_H			(MVOP_REG_BASE + 0x89)
    #define VOP_SKIP_SIZE_RVIEW       (BIT0 | BIT1 | BIT2 | BIT3)
#define VOP_REG_ENABLE_SKIP           (MVOP_REG_BASE + 0x8D)
    #define VOP_SKIP_LVIEW             BIT6
    #define VOP_SKIP_RVIEW             BIT7

#define VOP_REG_MASK                       (MVOP_REG_BASE + 0x8E)
    #define VOP_LSB_REQ_MASK       (BIT0 | BIT1) //RGB format selection

#define VOP_LSB_YSTR0_L                         (MVOP_REG_BASE + 0x94)
#define VOP_LSB_YSTR0_H                         (MVOP_REG_BASE + 0x96)
#define VOP_LSB_UVSTR0_L                        (MVOP_REG_BASE + 0x98)
#define VOP_LSB_UVSTR0_H                        (MVOP_REG_BASE + 0x9A)
#define VOP_LSB_YSTR1_L                         (MVOP_REG_BASE + 0x9C)
#define VOP_LSB_YSTR1_H                         (MVOP_REG_BASE + 0x9E)
#define VOP_LSB_UVSTR1_L                        (MVOP_REG_BASE + 0xA0)
#define VOP_LSB_UVSTR1_H                        (MVOP_REG_BASE + 0xA2)

#define VOP_DC_STRIP_LSB                            (MVOP_REG_BASE + 0xA4)

#define VOP_REG_4K2K_2P                          (MVOP_REG_BASE + 0xA6)
    #define VOP_4K2K_2P             BIT1

#define VOP_REG_MFDEC_0_L                          (MVOP_REG_BASE + 0xA8)
    #define VOP_MFDEC_EN            BIT0
    #define VOP_MF0_BURST           (BIT4 | BIT5)
    #define VOP_MF1_BURST           (BIT6 | BIT7)

#define VOP_REG_MFDEC_2_L                          (MVOP_REG_BASE + 0xAC)
    #define VOP_MF_FROM_WB          BIT6

#define VOP_REG_BW_SAVE                          (MVOP_REG_BASE + 0xC0)
    #define VOP_420_BW_SAVE_EX     BIT0

#define VOP_REG_BLK_VCNT_L                         (MVOP_REG_BASE + 0xC4)
#define VOP_REG_BLK_VCNT_H                         (MVOP_REG_BASE + 0xC5)

#define VOP_REG_MRQ                          (MVOP_REG_BASE + 0xC9)
	#define VOP_LST_CTRL_DCTOP     	(BIT3 | BIT4 | BIT5)
	#define VOP_MRQ_EN     			BIT6


//------------------------------------------------------------------------------
// chip top
//------------------------------------------------------------------------------
#define REG_CKG_DC0                             (CHIP_REG_BASE + 0x98)
    #define CKG_DC0_GATED           BIT0
    #define CKG_DC0_INVERT          BIT1
    #define CKG_DC0_MASK            (BIT4 | BIT3 | BIT2) //select clk src
        #define CKG_DC0_SYNCHRONOUS   (0 << 2)
        #define CKG_DC0_FREERUN        (1 << 2)
        #define CKG_DC0_320MHZ         (2 << 2)
        #define CKG_DC0_108MHZ         (3 << 2)
        #define CKG_DC0_123MHZ         (4 << 2) //as default
        #define CKG_DC0_144MHZ         (5 << 2)
        #define CKG_DC0_160MHZ         (6 << 2)
        #define CKG_DC0_192MHZ         (7 << 2)


#define REG_CKG_SUB_DC0                             (CHIP_REG_BASE + 0xAF)
    #define CKG_SUB_DC0_GATED           BIT0
    #define CKG_SUB_DC0_INVERT          BIT1
    #define CKG_SUB_DC0_MASK            (BIT4 | BIT3 | BIT2) //select clk src
        #define CKG_SUB_DC0_SYNCHRONOUS   (0 << 2)
        #define CKG_SUB_DC0_FREERUN        (1 << 2)
        #define CKG_SUB_DC0_320MHZ         (2 << 2)
        #define CKG_SUB_DC0_108MHZ         (3 << 2)
        #define CKG_SUB_DC0_123MHZ         (4 << 2) //as default
        #define CKG_SUB_DC0_144MHZ         (5 << 2)
        #define CKG_SUB_DC0_160MHZ         (6 << 2)
        #define CKG_SUB_DC0_192MHZ         (7 << 2)

#define REG_CKG_DC_SRAM                             (CHIP_REG_BASE + 0x9E)
    #define CKG_DC0_SRAM                BIT0
    #define CKG_DC1_SRAM                BIT4

// For check stc cw
#define REG_STC_CW_SLE_L                 (CHIP_REG_BASE + 0x0A) //reg_stc0_cw_sel
#define REG_STC_CW_SLE_H                 (CHIP_REG_BASE + 0x0B) //reg_stc1_cw_sel
#define REG_STC0_CW_L                    (CHIP_REG_BASE + 0x0C)
#define REG_STC0_CW_H                    (CHIP_REG_BASE + 0x0E)
#define REG_STC1_CW_L                    (CHIP_REG_BASE + 0x10)
#define REG_STC1_CW_H                    (CHIP_REG_BASE + 0x12)
#define REG_TSP_CLK                      (CHIP_REG_BASE + 0x54) //reg_ckg_tsp

//For main mvop
#define REG_UPDATE_DC0_CW                       (CHIP_REG_BASE + 0xE0)
    #define UPDATE_DC0_FREERUN_CW       BIT0
    #define UPDATE_DC0_SYNC_CW          BIT1
#define REG_DC0_FREERUN_CW_L                    (CHIP_REG_BASE + 0xE4)
#define REG_DC0_FREERUN_CW_H                    (CHIP_REG_BASE + 0xE6)
#define REG_DC0_NUM                             (CHIP_REG_BASE + 0xE8)
#define REG_DC0_DEN                             (CHIP_REG_BASE + 0xEA)

//For sub mvop
#define REG_UPDATE_DC1_CW                       (CHIP_REG_BASE + 0xE1)
    #define UPDATE_DC1_FREERUN_CW       BIT0
    #define UPDATE_DC1_SYNC_CW          BIT1
#define REG_DC1_FREERUN_CW_L                    (CHIP_REG_BASE + 0xEC)
#define REG_DC1_FREERUN_CW_H                    (CHIP_REG_BASE + 0xEE)
#define REG_DC1_NUM                             (CHIP_REG_BASE + 0xF0)
#define REG_DC1_DEN                             (CHIP_REG_BASE + 0xF2)

//------------------------------------------------------------------------------
// SC_FE
//------------------------------------------------------------------------------
#if 0
#define REG_MIU_SEL_FROM_IP                     (SC_FE_REG_BASE + 0x22)
    #define MVOP_MIU_IP_SEL             BIT2
    #define MFDEC0_MIU_IP_SEL           BIT3
    #define MFDEC1_MIU_IP_SEL           BIT4
#endif
//-----------------------------------------------------------------------------
// CLK_GEN1
//-----------------------------------------------------------------------------
#define REG_CKG_FBDEC                          (CLKGEN1_REG_BASE + 0x4A)
    #define VOP_SEL_CLK_432                    (BIT0 | BIT1 | BIT2 | BIT3)

#endif // _REG_VOP_H_
