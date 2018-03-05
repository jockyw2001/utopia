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
#ifndef _MFE_REG_H_
#define _MFE_REG_H_

#include "mfe_type.h"


// Reg bank base address
// See mhal_mfe.h
// #define REG_BANK_MFE 0x0a80 //0
// #define REG_BANK_MFE1 0xB00


#define REG_ENC_MODE_MPG4 0
#define REG_ENC_MODE_H263 1
#define REG_ENC_MODE_H264 2
#define REG_ENC_MODE_JPEG 3
#ifdef _MFE_A3_
#define MFE_MIAW 29//25//24
#else
#define MFE_MIAW 26//25//24
#endif
#define ADDR_HI_BITS    ((MFE_MIAW-5)-16)
#define OUTBUF_HI_BITS    (MFE_MIAW-16)
#define ADDR_MASK (0xffffffff>>(32-(MFE_MIAW-5)))

#define LAST_FRAME_AVGQP_HI_BITS (24-16)
#define BITCOUNT_HI_BITS (23-16)

// IRQ's
#define IRQ_NET_TRIGGER      6
#define IRQ_FS_FAIL          5
#define IRQ_TXIP_TIME_OUT    4
#define IRQ_BSPOBUF_FULL     3
#define IRQ_IMG_BUF_FULL     2
#define IRQ_MARB_BSPOBUF_FUL 1
#define IRQ_FRAME_DONE       0
#define CHECK_IRQ_STATUS(u16reg, irqnum) ((u16reg>>irqnum)&1)

typedef struct _fdc_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_s_fdc_ack:1;                    // fdc to cpu ack; 0/1: frame data pool not empty/frame data pool empty; 48x64 bits of space
            MFE_U16 reg_mfe_s_fdc_done_clr:1;        // fdc done clear (write one clear)
            MFE_U16 reg_mfe_s_fdc_done:1;                // fdc done; indicate to CPU that data has been written to internal buffer
            MFE_U16 reg48_dummy:8;
            MFE_U16 reg_mfe_s_fdc_bs_vld:1;            // set for bitstream write out (write one clear)
        };
        MFE_U16 reg48;
    };
} RegFdcDone;


typedef struct _wqt_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_s_tbc_rw:1;                // table mode; 0: read, 1: write
            MFE_U16 reg_mfe_s_tbc_done_clr:1;            // table done clear (write one clear)
            MFE_U16 reg_mfe_s_tbc_done:1;                // table done; indicate to CPU that (1) data has been written to table (2) table output is ready at reg_mfe_s_tbc_rdata
            MFE_U16 reg49_dummy:5;
            MFE_U16 reg_mfe_s_tbc_adr:6;                // table address
        };
        MFE_U16 reg49;
    };
} RegWQTDone;


typedef struct _qt_rb_check_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_s_tbc_rdata:16;                // table mode; 0: read, 1: write
        };
        MFE_U16 reg4b;
    };
} RegQT_RB_CHECK;
#ifdef _MFE_A3_
typedef struct _qt_rb_check_reg03_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_g_er_mode:2;                    // 0/1/2/3: mby/bs/mby+bs/off
            MFE_U16 reg_mfe_g_er_mby:2;                    // 0/1/2/3: every 1/2/4/8 mb row(s) (error resilence)
            MFE_U16 reg_mfe_g_packed_mode:1;                // frame buffer format for 422 packed mode; 0/1: YVYU/YUYV
            MFE_U16 reg_mfe_g_qmode:1;                        // quantization method; 0/1: h263/mp4
            MFE_U16 reg_mfe_g_tbc_mode:1;                    // table mode; 0: SW control, 1: HW control
            MFE_U16 reg_mfe_g_fldpic_en:1;                    //field picture coding
            MFE_U16 reg_mfe_g_dct_only_en:1;                // [NOT SUPPORRTED] reg_mfe_g_dct_only_en
            MFE_U16 reg_mfe_g_mstar_tile:1;                // input buffer format; 0: m4ve tiled mode, 1: mstar tiled mode
            MFE_U16 reg_mfe_g_mstar_tile_field_split:1;    // input field buffer format; 0: top/bottom fields interlaced(new), 1: fields split(old)
            MFE_U16 reg_mfe_g_fldpic_idx:1;                // input field buffer order; 0: top, 1: bottom
        };
        MFE_U16 reg03;
    };
} RegQT_RB_CHECK_REG03;
#else
typedef struct _qt_rb_check_reg03_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_g_er_mode:2;        // 0/1/2/3: mby/bs/mby+bs/off
            MFE_U16 reg_mfe_g_er_mby:2;        // 0/1/2/3: every 1/2/4/8 mb row(s) (error resilence)
            MFE_U16 reg_mfe_g_packed_mode:1;    // frame buffer format for 422 packed mode; 0/1: YVYU/YUYV
            MFE_U16 reg_mfe_g_qmode:1;            // quantization method; 0/1: h263/mp4
            MFE_U16 reg_mfe_g_tbc_mode:1;        // table mode; 0: SW control, 1: HW control
            MFE_U16 reg_mfe_g_fldpic_en:1;        //field picture coding
        };
        MFE_U16 reg03;
    };
} RegQT_RB_CHECK_REG03;
#endif

typedef struct _miu_protection_info_ {
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_0_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 BSP obuf
#if (10-OUTBUF_HI_BITS)>0
            MFE_U16 reg_mfe_59_reserved:(10-OUTBUF_HI_BITS);
#endif
            MFE_U16 reg_mfe_s_marb_miu_bound_err:1; //miu write protection, miu bound error status for write port 0 ~ 3
#ifndef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_off:1; //miu write protection, miu off
            MFE_U16 reg_mfe_s_marb_miu_bound_en:4; //miu write protection, miu bound enable for write port 0 ~ 3
#endif
        };
        MFE_U16 reg59;
    };
} MIU_PRO_REG;

typedef struct _miu_protection_info2_ {
    union {
        struct {
            MFE_U16 reg_mfe_g_mp4_itlc:1;                // 0/1: MPEG4 progressive/interlaced mode
            MFE_U16 reg_mfe_g_mp4_pskip_off:1;            // 0/1: MPEG4 enable/disable p skip mode
            MFE_U16 reg_mfe_g_mp4_acp:2;                // [0]: 0/1: sw/hw acp selection; [1]: sw default value: 0/1: disable/enable acp; current version off
            MFE_U16 reg_mfe_g_mp4_rounding_ctrl:1;        // mp4 rounding control specified as in spec
            MFE_U16 reg_mfe_g_er_hec:1;                // 0/1: header extension code off/on
            MFE_U16 reg_mfe_g_er_hec_t:3;                // HEC counter reset values
            MFE_U16 reg_mfe_g_er_h263_unit:2;            // 0/1/2: unit is 1/2/4, for calculating gob_num.
            MFE_U16 reg_mfe_g_mp4_direct_en:1;            // MPEG4 direct enable
            MFE_U16 reg_mfe_g_mp4_direct_mvstore:1;    // [M]: enable storing of mv & skip_mb information to DRAM in P(or sometimes I) frame
        };
        MFE_U16 reg19;
    };
} MIU_PRO_REG2;
typedef struct _mfe_reg_ {
    // [GLOBAL SETTING]
    union {
        struct {
            MFE_U16 reg_mfe_g_frame_start_sw:1;    // frame start (1T clk_mfe)
            MFE_U16 reg_mfe_g_soft_rstz:1;    // software reset; 0/1: reset/not reset
            MFE_U16 reg_mfe_g_enc_mode:2;        // 0/1/2/3: MPEG4/H263/H264/JPEG
            MFE_U16 reg_mfe_g_frame_type:2;    // 0/1/2: I/P/B
            MFE_U16 reg_mfe_g_ref_no:1;        // 0/1: 1 frame/2 frames
            MFE_U16 reg_mfe_g_mbr_en:1;        // 0/1: disable/enable MB-level Rate control
            MFE_U16 reg_mfe_g_qscale:6;        // frame level qscale: [H264]: 1 ~ 51; [MPEG4]: 1 ~ 31
            MFE_U16 reg_mfe_g_rec_en:1;        // reconstruct enable
        };
        MFE_U16 reg00;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_pic_width:12;    // picture width
        };
        MFE_U16 reg01;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_pic_height:12;    // picture height
        };
        MFE_U16 reg02;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_er_mode:2;        // 0/1/2/3: mby/bs/mby+bs/off
            MFE_U16 reg_mfe_g_er_mby:2;        // 0/1/2/3: every 1/2/4/8 mb row(s) (error resilence)
            MFE_U16 reg_mfe_g_packed_mode:1;    // frame buffer format for 422 packed mode; 0/1: YVYU/YUYV
            MFE_U16 reg_mfe_g_qmode:1;            // quantization method; 0/1: h263/mp4
            MFE_U16 reg_mfe_g_tbc_mode:1;        // table mode; 0: SW control, 1: HW control
            MFE_U16 reg_mfe_g_fldpic_en:1;        //field picture coding
        };
        MFE_U16 reg03;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_er_bs_th:16;    // er_bs mode threshold
        };
        MFE_U16 reg04;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_inter_pref:16;    // inter prediction preference
        };
        MFE_U16 reg05;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_cur_y_adr_low:16;    // current luma base address
        };
        MFE_U16 reg06;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_cur_y_adr_high:ADDR_HI_BITS;    // current luma base address
        };
        MFE_U16 reg07;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_cur_c_adr_low:16;    // current chroma base address
        };
        MFE_U16 reg08;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_cur_c_adr_high:ADDR_HI_BITS;    // current chroma base address
        };
        MFE_U16 reg09;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_y_adr0_low:16;    // reference luma base address0
        };
        MFE_U16 reg0a;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_y_adr0_high:ADDR_HI_BITS;    // reference luma base address0
        };
        MFE_U16 reg0b;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_y_adr1_low:16;    // reference luma base address1
        };
        MFE_U16 reg0c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_y_adr1_high:ADDR_HI_BITS;    // reference luma base address0
        };
        MFE_U16 reg0d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_c_adr0_low:16;    // reference chroma base address0
        };
        MFE_U16 reg0e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_c_adr0_high:ADDR_HI_BITS;    // reference chroma base address0
        };
        MFE_U16 reg0f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_c_adr1_low:16;    // reference chroma base address1
        };
        MFE_U16 reg10;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_ref_c_adr1_high:ADDR_HI_BITS;    // reference chroma base address1
        };
        MFE_U16 reg11;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_rec_y_adr_low:16;    // reconstructed luma base address
        };
        MFE_U16 reg12;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_rec_y_adr_high:ADDR_HI_BITS;    // reconstructed luma base address
        };
        MFE_U16 reg13;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_rec_c_adr_low:16;    // reconstructed chroma base address
        };
        MFE_U16 reg14;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_rec_c_adr_high:ADDR_HI_BITS;    // reconstructed chroma base address
        };
        MFE_U16 reg15;
    };
    union {
        struct {    // clock gating
            MFE_U16 gate_cry_crc_sram:1;
            MFE_U16 gate_qtab_dbfdc_dbqtb_sram:1;
            MFE_U16 gate_mcy_mcc_sram:1;
            MFE_U16 gate_res0_res1_sram:1;
            MFE_U16 gate_ieap:1;
            MFE_U16 gate_dct_idct:1;
            MFE_U16 gate_dbf:1;
        };
        MFE_U16 reg16;
    };
    union {
        struct {
            MFE_U16 reserved_reg17;
        };
        MFE_U16 reg17;
    };
    // [JPEG]
    union {
        struct {
            MFE_U16 reg_mfe_g_jpe_enc_mode:2;            // JPE encode mode; 2'b00/2'b01/2'b10/2'b11: 420/422/444/gray; current version supports 422 only
            MFE_U16 reg_mfe_g_jpe_buffer_mode:1;        // JPE buffer mode; 0/1: double buffer mode/frame buffer mode
            MFE_U16 reg_mfe_g_jpe_multibuf_mode:2;        // JPE multi-buffer mode; 0/1/2: 2/4/8 buffers
            MFE_U16 reg_mfe_g_jpe_qfactor:4;            // JPE q factor; 0 ~ 15: (1 ~ 16)/4
            MFE_U16 reg_mfe_g_jpe_fsvs_mode:2;            // JPE fsvs generation mode; 0/1/2: pure sw/sw+hw/hw
            MFE_U16 reg18_dummy:4;
            MFE_U16 reg_mfe_g_viu_soft_rstz:1;            // viu software reset; 0/1: reset/not reset
        };
        MFE_U16 reg18;
    };
    // [MPEG4/H263]
    union {
        struct {
            MFE_U16 reg_mfe_g_mp4_itlc:1;                // 0/1: MPEG4 progressive/interlaced mode
            MFE_U16 reg_mfe_g_mp4_pskip_off:1;            // 0/1: MPEG4 enable/disable p skip mode
            MFE_U16 reg_mfe_g_mp4_acp:2;                // [0]: 0/1: sw/hw acp selection; [1]: sw default value: 0/1: disable/enable acp; current version off
            MFE_U16 reg_mfe_g_mp4_rounding_ctrl:1;        // mp4 rounding control specified as in spec
            MFE_U16 reg_mfe_g_er_hec:1;                // 0/1: header extension code off/on
            MFE_U16 reg_mfe_g_er_hec_t:3;                // HEC counter reset values
            MFE_U16 reg_mfe_g_er_h263_unit:2;            // 0/1/2: unit is 1/2/4, for calculating gob_num.
            MFE_U16 reg_mfe_g_mp4_direct_en:1;            // MPEG4 direct enable
            MFE_U16 reg_mfe_g_mp4_direct_mvstore:1;    // [M]: enable storing of mv & skip_mb information to DRAM in P(or sometimes I) frame
        };
        MFE_U16 reg19;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_mp4_direct_pref:8;        // used in mp4 only, mp4 direct mode preference value
            MFE_U16 reg_mfe_g_mp4_direct_trb:3;        // used in mp4 only, mp4 direct mode trb (P0-B distance)
            MFE_U16 reg_mfe_g_mp4_direct_trd:3;        // used in mp4 only, mp4 direct mode trd (P0-P1 distance)
        };
        MFE_U16 reg1a;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_mp4_flddct_diff_thr:8;    // used in mp4 only, mp4 field dct difference threshold
            MFE_U16 reg_mfe_g_mp4_flddct_en:1;            // used in mp4 only, mp4 field dct enable
        };
        MFE_U16 reg1b;
    };
    // [IRQ & important IP status checkings]
    union {
        struct {
            MFE_U16 reg_mfe_g_irq_mask:8;                // 0/1: irq not-mask/mask
            MFE_U16 reg_mfe_g_irq_force:8;                // 0/1: set corresponding interrupt as usual/force corresponding interrupt
        };
        MFE_U16 reg1c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_irq_clr0:1;                // 0/1: not clear interrupt/clear interrupt 0
            MFE_U16 reg_mfe_g_irq_clr1:1;                // 0/1: not clear interrupt/clear interrupt 1
            MFE_U16 reg_mfe_g_irq_clr2:1;                // 0/1: not clear interrupt/clear interrupt 2
            MFE_U16 reg_mfe_g_irq_clr3:1;                // 0/1: not clear interrupt/clear interrupt 3
            MFE_U16 reg_mfe_g_irq_clr4:1;                // 0/1: not clear interrupt/clear interrupt 4
            MFE_U16 reg_mfe_g_irq_clr5:1;                // 0/1: not clear interrupt/clear interrupt 5
            MFE_U16 reg_mfe_g_irq_clr6:1;                // 0/1: not clear interrupt/clear interrupt 6
            MFE_U16 reg_mfe_g_irq_clr7:1;                // 0/1: not clear interrupt/clear interrupt 7
            MFE_U16 reg_mfe_g_swrst_safe:1;            // to indicate there're no miu activities that need to pay attention to
        };
        MFE_U16 reg1d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_irq_cpu:8;                // status of interrupt on CPU side
            MFE_U16 reg_mfe_g_irq_ip:8;                // status of interrupt on IP side
        };
        MFE_U16 reg1e;
    };
    union {
        struct {
            MFE_U16 reserved_reg1f;
        };
        MFE_U16 reg1f;
    };
    // [ME setting]
    union {
        struct {
            MFE_U16 reg_mfe_s_me_4x4_disable:1;        // 4x4_disable
            MFE_U16 reg_mfe_s_me_8x4_disable:1;        // 8x4_disable
            MFE_U16 reg_mfe_s_me_4x8_disable:1;        // 4x8_disable
            MFE_U16 reg_mfe_s_me_16x8_disable:1;        // 16x8_disable
            MFE_U16 reg_mfe_s_me_8x16_disable:1;        // 8x16_disable
            MFE_U16 reg_mfe_s_me_8x8_disable:1;        // 8x8_disable
            MFE_U16 reg_mfe_s_me_16x16_disable:1;        // 16x16_disable
            MFE_U16 reg_mfe_s_mesr_adapt:1;            // me search range auto-adaptive; 0/1: off/on
            MFE_U16 reg_mfe_s_me_ref_en_mode:2;        // ref enable mode: 2'b01/2'b10/2'b11: ref0 enable/ref1 enable/ref0&1 enable
        };
        MFE_U16 reg20;
    };
    // [IME PIPELINE]
    union {
        struct {
            MFE_U16 reg_mfe_s_ime_sr16:1;                // search range limited to (h,v) = (+/-16, +/-16); 0/1: search range 32/16
            MFE_U16 reg_mfe_s_ime_umv_disable:1;        // 0/1: UMV enable/disable
            MFE_U16 reg_mfe_s_ime_ime_wait_fme:1;        // 0/1: ime wait fme/fme wait ime
            MFE_U16 reg_mfe_s_ime_boundrect_en:1;        // ime bounding rectangle enable (needed for level 3.0 and below)
            MFE_U16 reg_mfe_s_ime_h264_p8x8_ctrl_en:1;    // ime h264 max p8x8 count control enable
            MFE_U16 reg21_dummy:3;
            MFE_U16 reg_mfe_s_ime_h264_p8x8_max:6;        // ime h264 max p8x8 count; value 0 is prohibited
                                                        // Max P8x8 MB count = 16 * reg_mfe_s_ime_h264_p8x8_max
        };
        MFE_U16 reg21;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_ime_mesr_max_addr:8;        // me search range max depth
            MFE_U16 reg_mfe_s_ime_mesr_min_addr:8;        // me search range min depth
        };
        MFE_U16 reg22;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_ime_mvx_min:6;            // me mvx min; 0/¡K/62 --> -32/¡K/30
            MFE_U16 reg24_dummy:2;
            MFE_U16 reg_mfe_s_ime_mvx_max:6;            // me mvx max; 0/¡K/62 --> -32/¡K/30
        };
        MFE_U16 reg23;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_ime_mvy_min:6;            // me mvy min; 0/¡K/62 --> -32/¡K/30
            MFE_U16 reg25_dummy:2;
            MFE_U16 reg_mfe_s_ime_mvy_max:6;            // me mvy max; 0/¡K62/ --> -32/¡K/30
        };
        MFE_U16 reg24;
    };
    // [FME pipeline]
    union {
        struct {
            MFE_U16 reg_mfe_s_fme_quarter_disable:1;    // 0/1: Quarter fine-tune enable/disable
            MFE_U16 reg_mfe_s_fme_half_disable:1;        // 0/1: Half fine-tune enable/disable
            MFE_U16 /*reg_mfe_s_fme_one_mode*/reg26_dummy:1;
            MFE_U16 reg_mfe_s_fme_pmv_enable:1;        // 0/1: disable/enable Previous Skip MV mode
            MFE_U16 reg_mfe_s_fme_mode_no:1;            // 0: one mode.  1: two mode.
            MFE_U16 reg_mfe_s_fme_mode0_refno:1;        // 0: one ref. for mode0   1: two ref. for mode0
            MFE_U16 reg_mfe_s_fme_mode1_refno:1;        // 0: one ref. for mode1   1: two ref. for mode1
            MFE_U16 reg_mfe_s_fme_mode2_refno:1;        // 0: one ref. for mode2   1: two ref. for mode2
            MFE_U16 reg_mfe_s_fme_skip:1;                // fme skip
            MFE_U16 reg_mfe_s_fme_pipeline_on:1;    //0/1: FME pipeline off/on
        };
        MFE_U16 reg25;
    };
    // MBR
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_pqp_dlimit:2;    // previous qp diff limit
            MFE_U16 reg_mfe_s_mbr_uqp_dlimit:2;    // upper qp diff limit
            MFE_U16 reg_mfe_s_mbr_tmb_bits:12;        // target MB bits
        };
        MFE_U16 reg26;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_frame_qstep:13;    // frame level qp's qstep
        };
        MFE_U16 reg27;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_last_frm_avg_qp_low:16;    // last frame average qp (status register)
        };
        MFE_U16 reg28;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_last_frm_avg_qp_high:LAST_FRAME_AVGQP_HI_BITS;    // last frame average qp (status register)
            MFE_U16 reg_mfe_s_mbr_qp_cidx_offset:5;        // [H264] chroma qp index offset (+12). Spec range is [-12,12]
        };
        MFE_U16 reg29;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_qp_min:6;                // qp min
            MFE_U16 reg_mfe_s_mbr_qp_max:6;                // qp max
            MFE_U16 reg_mfe_s_mvdctl_ref0_offset:2;        // H264 mvy offset adjustment for MCC if ref is frame 0: 0/1/2: 0/+2/-2
            MFE_U16 reg_mfe_s_mvdctl_ref1_offset:2;        // H264 mvy offset adjustment for MCC if ref is frame 1: 0/1/2: 0/+2/-2
        };
        MFE_U16 reg2a;
    };
    // IEAP
    union {
        struct {
            MFE_U16 reg_mfe_s_ieap_last_mode:4;            // software control of the last mode of Intra4x4 mode 0 ~ 8
            MFE_U16 reg_mfe_s_ieap_constraint_intra:1;        // software control constraint intra; 0/1: OFF/ON
            MFE_U16 reg_mfe_s_ieap_ccest_en:1;                // software control cost estimator; 0/1: OFF/ON
            MFE_U16 reg_mfe_s_ieap_ccest_thr:2;            // threshold of cost estimator set 0 ~ 3 for threshold 1 ~ 4
            MFE_U16 reg_mfe_s_ieap_drop_i16:1;                // software control stop-Intra16x16-mode; 1:w/o I16M, 0:w/i I16MB
        };
        MFE_U16 reg2b;
    };
    // QUAN
    union {
        struct {
            MFE_U16 reg_mfe_s_quan_idx_last:6;        // the index of the last non-zero coefficient in the zig-zag order
            MFE_U16 reg_mfe_s_quan_idx_swlast:1;    // software control of the index of the last non-zero coefficient in the zig-zag order; 0/1: disable/enable
        };
        MFE_U16 reg2c;
    };
    // TXIP control & debug
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_mbx:9;              //txip mbx
            MFE_U16 reg_mfe_s_txip_sng_mb:1;           //0/1: disable/enable txip controller stop-and-go mechanism using (txip_mbx == reg_mfe_g_debug_trig_mbx) & (txip_mby == reg_mfe_g_debug_trig_mby)
            MFE_U16 reg_mfe_s_txip_sng_set:1;          //txip controller stop-and-go mechanism using this register bit: 0/1: go/stop
            MFE_U16 reg_mfe_s_txip_dbf_full_halt_en:1; //txip controller stop-and-go mechanism using double buffer fullness as criterion; 0/1: disable/enable
        };
        MFE_U16 reg2d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_mby:9;                // txip mby
        };
        MFE_U16 reg2e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_irfsh_mb_s0:12;    // intra refresh mb start 0
            MFE_U16 reg_mfe_s_txip_irfsh_en:2;        // intra refresh enable bits: bit0: enable condition 0; bit 1: enable condition 1
        };
        MFE_U16 reg2f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_irfsh_mb_e0:12;    // intra refresh mb end 0
        };
        MFE_U16 reg30;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_irfsh_mb_s1:12;    // intra refresh mb start 1
        };
        MFE_U16 reg31;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_irfsh_mb_e1:12;    // intra refresh mb end 1
            MFE_U16 reg_mfe_s_txip_timeout_en:1;    // txip time out enable
            MFE_U16 reg_mfe_s_txip_wait_mode:1;    // txip waiting mode to move to next MB; 0/1: idle count/cycle count
        };
        MFE_U16 reg32;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_idle_cnt:16;    // wait mode is 0: txip idle count (x 64T)/ wait mode is 1: txip total processing count (x 64T)
        };
        MFE_U16 reg33;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_txip_timeout:16;        // txip timeout count (x 64T)
        };
        MFE_U16 reg34;
    };
    // [ECDB PIPELINE]
    // ECDB control & debug
    union {
        struct {
            MFE_U16 reg_mfe_s_ecdb_mbx:9;            // ecdb mbx
        };
        MFE_U16 reg35;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_ecdb_mby:9;            // ecdb mby
        };
        MFE_U16 reg36;
    };
    // MDC
    union {
        struct {
            MFE_U16 reg_mfe_s_mdc_total_mb_bw:4;    // total mb bit width used in video_pkt
            MFE_U16 reg_mfe_s_mdc_m4vpktpzero:1;    // MPEG4 video packet preceding zeros: 0/1: 16/17 zeros
            MFE_U16 reg_mfe_s_mdc_m4time:2;        // MPEG4 modulo time base: 0/1/2/3: 0/10/110/1110
            MFE_U16 reg_mfe_s_mdc_m4iadcvlc_th:3;    // MPEG4 intra dc vlc threshold
            MFE_U16 reg_mfe_s_mdc_m4vop_tinc_bw:4;    // vop_time_increment bit width
        };
        MFE_U16 reg37;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mdc_m4vop_tinc:15;    //  vop_time_increment
        };
        MFE_U16 reg38;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mdc_gob_frame_id:2;            //  H263 gob frame id
            MFE_U16 reg_mfe_s_mdc_h264_nal_ref_idc:2;        // nal_ref_idc
            MFE_U16 reg_mfe_s_mdc_h264_nal_unit_type:1;    //  0/1: 1/5
            MFE_U16 reg_mfe_s_mdc_h264_fnum_bits:2;        // H264 frame num bits
            MFE_U16 reg_mfe_s_mdc_h264_dbf_control:1;        // dbf control present flag
            MFE_U16 reg_mfe_s_mdc_h264_fnum_value:8;        // H264 frame num value
        };
        MFE_U16 reg39;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mdc_h264_idr_pic_id:3;
            MFE_U16 reg_mfe_s_mdc_h264_disable_dbf_idc:2;
            MFE_U16 reg_mfe_s_mdc_h264_alpha:4;                // slice_alpha_c0_offset_div2
            MFE_U16 reg_mfe_s_mdc_h264_beta:4;                    // slice_beta_offset_div2
            MFE_U16 reg_mfe_s_mdc_h264_ridx_aor_flag:1;        // reference index active override flag
        };
        MFE_U16 reg3a;
    };
    // BSPOBUF/MVOBUF
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_set_adr:1;            // set bsp obuf start address(write one clear)
            MFE_U16 reg_mfe_s_mvobuf_set_adr:1;            // set mv obuf start address (write one clear)
            MFE_U16 reg_mfe_s_bspobuf_fifo_th:3;            // bsp obuf threshold
            MFE_U16 reg_mfe_s_mvobuf_fifo_th:3;            // mv obuf threshold
            MFE_U16 reg_mfe_s_bsp_fdc_skip:1;                // fdc skip enable; 0: fdc skip disable, 1: fdc skip enable
        };
        MFE_U16 reg3b;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_sadr_low:16;                    // bsp obuf start address
        };
        MFE_U16 reg3c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_sadr_high:OUTBUF_HI_BITS;    // bsp obuf start address
        };
        MFE_U16 reg3d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_eadr_low:16;                    // bsp obuf end address
        };
        MFE_U16 reg3e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_eadr_high:OUTBUF_HI_BITS;    // bsp obuf end address
        };
        MFE_U16 reg3f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mvobuf_sadr_low:16;                    // mv obuf start address
        };
        MFE_U16 reg40;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mvobuf_sadr_high:OUTBUF_HI_BITS;        // mv obuf start address
        };
        MFE_U16 reg41;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bsp_bit_cnt_low:16;    // encoded bit count (one frame)
        };
        MFE_U16 reg42;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bsp_bit_cnt_high:BITCOUNT_HI_BITS;    // encoded bit count (one frame)
        };
        MFE_U16 reg43;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_wptr_low:16;    // bspobuf write pointer (8 byte unit)
        };
        MFE_U16 reg44;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_bspobuf_wptr_high:OUTBUF_HI_BITS;    // bspobuf write pointer (8 byte unit)
        };
        MFE_U16 reg45;
    };
    // FDC
    union {
        struct {
            MFE_U16 reg_mfe_s_fdc_bs:16;        // cpu to fdc bitstream data
        };
        MFE_U16 reg46;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_fdc_bs_len:5;        // cpu to fdc bitstream len; 0 ~ 16
            MFE_U16 reg_mfe_s_fdc_bs_count:10;        // cpu to fdc round count
        };
        MFE_U16 reg47;
    };
    // [Table Control]
    union {
        struct {
            MFE_U16 reg_mfe_s_fdc_ack:1;                    // fdc to cpu ack; 0/1: frame data pool not empty/frame data pool empty; 48x64 bits of space
            MFE_U16 reg_mfe_s_fdc_done_clr:1;        // fdc done clear (write one clear)
            MFE_U16 reg_mfe_s_fdc_done:1;                // fdc done; indicate to CPU that data has been written to internal buffer
            MFE_U16 reg48_dummy:8;
            MFE_U16 reg_mfe_s_fdc_bs_vld:1;            // set for bitstream write out (write one clear)
            MFE_U16 reg_mfe_s_tbc_en:1;            // set for table read & write ; 1: enable, 0: disable (write one clear)
        };
        MFE_U16 reg48;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_tbc_rw:1;                // table mode; 0: read, 1: write
            MFE_U16 reg_mfe_s_tbc_done_clr:1;            // table done clear (write one clear)
            MFE_U16 reg_mfe_s_tbc_done:1;                // table done; indicate to CPU that (1) data has been written to table (2) table output is ready at reg_mfe_s_tbc_rdata
            MFE_U16 reg49_dummy:5;
            MFE_U16 reg_mfe_s_tbc_adr:6;                // table address
        };
        MFE_U16 reg49;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_tbc_wdata:16;            // table write data
        };
        MFE_U16 reg4a;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_tbc_rdata:16;            // table read data
        };
        MFE_U16 reg4b;
    };
    // [Get Neighbor]
    union {
        struct {
            MFE_U16 reg_mfe_s_gn_sadr_low:16;                // gn base adr low
        };
        MFE_U16 reg4c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_gn_sadr_high:OUTBUF_HI_BITS;    // gn base adr high
        };
        MFE_U16 reg4d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_gn_mvibuf_sadr_low:16;                // mv ibuf start address low
        };
        MFE_U16 reg4e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_gn_mvibuf_sadr_high:OUTBUF_HI_BITS;    // mv ibuf start address high
        };
        MFE_U16 reg4f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_mrpriority_sw:2;        // mfe2mi_rpriority software programmable
            MFE_U16 reg_mfe_s_marb_mr_timeout_ref:1;    //miu read burst timeout count start point
            MFE_U16 reg_mfe_s_marb_mr_nwait_mw:1;        //miu read not wait mi2mfe_wrdy
            MFE_U16 reg_mfe_s_marb_mwpriority_sw:2;        //mfe2mi_wpriority software programmable
            MFE_U16 reg_mfe_s_marb_mw_timeout_ref:1;    //miu write burst timeout count start point
            MFE_U16 reg_mfe_s_marb_mw_nwait_mr:1;        //miu read not wait mi2mfe_wrdy
            MFE_U16 reg_mfe_s_marb_mr_pending:4;        //max. pending read requests to miu
            MFE_U16 reg_mfe_s_marb_32b_ad_nswap:1;        //32bits miu address not swap. only for 32bits mode
            MFE_U16 reg_mfe_s_marb_miu_wmode:1;        //0/1: original miu protocol/new miu protocol(wd_en)

        };
        MFE_U16 reg56;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_0_low:16;    // MIU protect for MPEG4 BSP obuf
        };
        MFE_U16 reg58;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_0_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 BSP obuf
#if (10-OUTBUF_HI_BITS)>0
            MFE_U16 reg_mfe_59_reserved:(10-OUTBUF_HI_BITS);
#endif
            MFE_U16 reg_mfe_s_marb_miu_bound_err:1; //miu write protection, miu bound error status for write port 0 ~ 3
#ifndef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_off:1; //miu write protection, miu off
            MFE_U16 reg_mfe_s_marb_miu_bound_en:4; //miu write protection, miu bound enable for write port 0 ~ 3
#endif
        };
        MFE_U16 reg59;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_0_low:16;    // MIU protect for MPEG4 BSP obuf
        };
        MFE_U16 reg5a;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_0_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 BSP obuf
#ifdef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_bound_en_0:1;    //
#endif
        };
        MFE_U16 reg5b;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_1_low:16;    // MIU Upper bound protect for MPEG4 MC obuf  rec
        };
        MFE_U16 reg5c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_1_high:OUTBUF_HI_BITS;    // MIU Upper bound protect for MPEG4 MC obuf rec
        };
        MFE_U16 reg5d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_1_low:16;    // MIU Lower bound protect for MPEG4 MC obuf
        };
        MFE_U16 reg5e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_1_high:OUTBUF_HI_BITS;    // MIU Lower bound protect for MPEG4 MC obuf
#ifdef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_bound_en_1:1;    //
#endif
        };
        MFE_U16 reg5f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_2_low:16;    // MIU protect for MPEG4 MV obuf
        };
        MFE_U16 reg60;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_2_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 MV obuf
        };
        MFE_U16 reg61;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_2_low:16;    // MIU protect for MPEG4 MV obuf
        };
        MFE_U16 reg62;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_2_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 MV obuf
#ifdef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_bound_en_2:1;    //
#endif
        };
        MFE_U16 reg63;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_3_low:16;    // MIU protect for MPEG4 GN
        };
        MFE_U16 reg64;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_ubound_3_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 GN
        };
        MFE_U16 reg65;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_3_low:16;    // MIU protect for MPEG4 GN
        };
        MFE_U16 reg66;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_marb_lbound_3_high:OUTBUF_HI_BITS;    // MIU protect for MPEG4 GN
#ifdef _MFE_A3_
            MFE_U16 reg_mfe_s_marb_miu_bound_en_3:1;    //
            MFE_U16 reg_mfe_s_marb_miu_off:1;    //
            MFE_U16 reg_mfe_s_marb_miu_bound_err:1;    //
#endif
        };
        MFE_U16 reg67;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_qstep_min:7;    // qstep min (note: max value of qstep_min is 128 because condition is qstep <= qstep_min)
        };
        MFE_U16 reg6e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_s_mbr_qstep_max:13; // qstep max
        };
        MFE_U16 reg6f;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_mode:6;            // debug mode
            MFE_U16 reg_mfe_g_debug_trig_cycle:10;        // wait (8 * reg_mfe_g_debug_trig_cycle) cycles
        };
        MFE_U16 reg70;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_trig_mbx:9;        // debug trigger mbx
        };
        MFE_U16 reg71;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_trig_mby:9;        // debug trigger mby
        };
        MFE_U16 reg72;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_trig:1;            // reg trigger (write one clear)
            MFE_U16 reg_mfe_g_debug_trig_mode:2;        // debug trigger mode; 0/1/2/3: reg_trigger/3rd stage (mbx, mby)/frame start
            MFE_U16 reg_mfe_g_debug_en:1;                // debug enable
            MFE_U16 reg_mfe_g_crc_mode:4;                //'h0: Disable,¡¥hc: bsp obuf, 'hd: mc obuf, 'hd: mc obuf
            MFE_U16 reg_mfe_g_debug_tcycle_chk_en:1; //enable total cycle check
            MFE_U16 reg_mfe_g_debug_tcycle_chk_sel:1; //select total cycle and report it on reg_mfe_g_crc_result[15:0]
        };
        MFE_U16 reg73;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_state0:16;            // "debug state for TXIP/ECDB submodule {txip2q_en, txip2iq_en, txip2mbr_en, txip2zmem_en, txip2dpcm_en,
            //  txip2mve_en, txip2mcobuf_en, txip2mbldr_en, ecdb2mdc_en,
            // ecdb2rlc_en, ecdb2vlc_en, 5'd0}"
        };
        MFE_U16 reg74;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_debug_state1;            // "debug state for ME submodule {3'd0, load_w4_ok, load_w3_ok, load_w2_ok, load_w1_ok, load_w0_ok,  2'd0,
            //    busy_ime, busy_fme, busy_mesr, busy_iacost, end_this_mb, init_this_mb"
        };
        MFE_U16 reg75;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_crc_result0:16;    // CRC64[15..0]
        };
        MFE_U16 reg76;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_crc_result1:16;    // CRC64[31..16]
        };
        MFE_U16 reg77;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_crc_result2:16;    // CRC64[47..32]
        };
        MFE_U16 reg78;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_crc_result3:16;    // CRC64[63..48]
        };
        MFE_U16 reg79;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_bist_fail0;
        };
        MFE_U16 reg7a;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_bist_fail1;
        };
        MFE_U16 reg7b;
    };
    union {
        struct {
            MFE_U16 reg_mfe_g_bist_fail2;
        };
        MFE_U16 reg7c;
    };
    union {
        struct {
            MFE_U16 reg_mfe_rsv0;
        };
        MFE_U16 reg7d;
    };
    union {
        struct {
            MFE_U16 reg_mfe_rsv1;
        };
        MFE_U16 reg7e;
    };
    union {
        struct {
            MFE_U16 reg_mfe_rsv2;
        };
        MFE_U16 reg7f;
    };

} MFE_REG;

//////////////////////////////////////////////////////////////////////////
// PERFORMANCE
//////////////////////////////////////////////////////////////////////////

#define CLOCK_GATING    // Enable clock gating

#define FME_PIPELINE_OPEN    // Enable David's FME speedup version

//#define DONT_PUT_FDC

//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: FDC && QTable
//////////////////////////////////////////////////////////////////////////

//#define CHECK_FDC_DONE    // Verify if hw receives the fdc command

//#define CHECK_WriteQTable_DONE    // Verify if hw receives the WriteQTable command

//#define QTABLE_READBACK_CHECK    // Verify all WriteQTable Value

//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: StopAndGo series
//////////////////////////////////////////////////////////////////////////

#define STOP_FRAME 0
#define STOP_MBX 7
#define STOP_MBY 7
// #define TEST_MB_STOPANDGO
// #define TEST_MB_STOPANDDROP

// #define TEST_STOPANDGO
// #define TEST_STOPANDDROP


//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: test miu protection
//////////////////////////////////////////////////////////////////////////


#define TEST_MIU_PROTECTION_MODE 0





#ifdef REG_JPEG_CMODEL

        //////////////////////////////////////////////////////////////////////////
        // DEBUG Flags: continuous shot test (JPEG only)
        //////////////////////////////////////////////////////////////////////////

        //#define TEST_CONTINUOUS_SHOT
        //#define CONTINUOUS_SHOT_NUMBER    5    //number of test shot

        //////////////////////////////////////////////////////////////////////////
        // DEBUG Flags: input row mode test (JPEG only)
        //////////////////////////////////////////////////////////////////////////
        //JPEG row mode only!
//        #define TEST_INPUT_ROW_MODE_HW

            #ifdef TEST_INPUT_ROW_MODE_HW
                #ifndef DONT_PUT_FDC
                    #define DONT_PUT_FDC
                #endif

                #define    NUM_OF_ROW_DONE_BEFORE_FS    1
            #endif

        //JPEG row mode only! only check fs_fail_irq
        //#define TEST_INPUT_ROW_MODE_SW_HW
            #ifdef TEST_INPUT_ROW_MODE_SW_HW
                #define    NUM_OF_ROW_DONE_BEFORE_FS    1
            #endif
#endif // REG_JPEG_CMODEL
//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: test CRC mode
//////////////////////////////////////////////////////////////////////////
#define TEST_CRC_MODE

#endif

