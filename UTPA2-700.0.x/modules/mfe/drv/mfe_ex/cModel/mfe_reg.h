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
// #define REG_BANK_MFE 0x0a80UL //0
// #define REG_BANK_MFE1 0xB00UL


#define REG_ENC_MODE_MPG4 0UL
#define REG_ENC_MODE_H263 1UL
#define REG_ENC_MODE_H264 2UL
#define REG_ENC_MODE_JPEG 3UL

#ifdef _MFE_M1_
#define MFE_MIAW 27UL //25//24
#elif defined(_MFE_A3_)
#define MFE_MIAW 29UL //25//24
#define MFE_MIDW (128UL) // MFE-T1: 128bit MIU
#else
#define MFE_MIAW 26UL //25//24
#endif
#define ADDR_HI_BITS	((MFE_MIAW-5UL)-16UL)
#define OUTBUF_HI_BITS	(MFE_MIAW-16UL)
#define IMIBUF_HI_BITS  (MFE_MIAW-16UL)     // low-bandwidth imi buffer is using 8-byte unit.
#define ADDR_MASK (0xffffffffUL>>(32UL-(MFE_MIAW-5UL)))

#define LAST_FRAME_AVGQP_HI_BITS (24UL-16UL)
#ifdef _MFE_AGATE_
#define BITCOUNT_HI_BITS (24UL-16UL)
#else
#define BITCOUNT_HI_BITS (23UL-16UL)
#endif
// IRQ's
#define IRQ_LESS_ROW_DONE    7UL
#define IRQ_NET_TRIGGER      6UL
#define IRQ_FS_FAIL          5UL
#define IRQ_TXIP_TIME_OUT    4UL
#define IRQ_BSPOBUF_FULL     3UL
#define IRQ_IMG_BUF_FULL     2UL
#define IRQ_MARB_BSPOBUF_FUL 1UL
#define IRQ_FRAME_DONE       0UL
#define CHECK_IRQ_STATUS(u16reg, irqnum) ((u16reg>>irqnum)&1UL)

#define HW_ECO_STARTCODE_PREVENTION

typedef struct _fdc_info_ {
	union {
		struct {
			MS_U16 reg_mfe_s_fdc_ack:1;					// fdc to cpu ack; 0/1: frame data pool not empty/frame data pool empty; 48x64 bits of space
			MS_U16 reg_mfe_s_fdc_done_clr:1;		// fdc done clear (write one clear)
			MS_U16 reg_mfe_s_fdc_done:1;				// fdc done; indicate to CPU that data has been written to internal buffer
			MS_U16 reg48_dummy:8;
			MS_U16 reg_mfe_s_fdc_bs_vld:1;			// set for bitstream write out (write one clear)
		};
		MS_U16 reg48;
	};
} RegFdcDone;


typedef struct _wqt_info_ {
	union {
		struct {
			MS_U16 reg_mfe_s_tbc_rw:1;				// table mode; 0: read, 1: write
			MS_U16 reg_mfe_s_tbc_done_clr:1;			// table done clear (write one clear)
			MS_U16 reg_mfe_s_tbc_done:1;				// table done; indicate to CPU that (1) data has been written to table (2) table output is ready at reg_mfe_s_tbc_rdata
			MS_U16 reg49_dummy:5;
			MS_U16 reg_mfe_s_tbc_adr:6;				// table address
		};
		MS_U16 reg49;
	};
} RegWQTDone;


typedef struct _qt_rb_check_info_ {
	union {
		struct {
			MS_U16 reg_mfe_s_tbc_rdata:16;				// table mode; 0: read, 1: write
		};
		MS_U16 reg4b;
	};
} RegQT_RB_CHECK;


typedef struct _ratecontrol_info_ {
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_last_frm_avg_qp_low:16;	// last frame average qp (status register)
		};
		MS_U16 reg28;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_last_frm_avg_qp_high:LAST_FRAME_AVGQP_HI_BITS;	// last frame average qp (status register)
			MS_U16 reg_mfe_s_mbr_qp_cidx_offset:5;		// [H264] chroma qp index offset (+12). Spec range is [-12,12]
		};
		MS_U16 reg29;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bsp_bit_cnt_low:16;	// encoded bit count (one frame)
		};
		MS_U16 reg42;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bsp_bit_cnt_high:BITCOUNT_HI_BITS;	// encoded bit count (one frame)
		};
		MS_U16 reg43;
	};
} RateControl_REG;




typedef struct _mfe_reg_ {
	// [GLOBAL SETTING]
	union {
		struct {
			MS_U16 reg_mfe_g_frame_start_sw:1;	// frame start (1T clk_mfe)
			MS_U16 reg_mfe_g_soft_rstz:1;	// software reset; 0/1: reset/not reset
			MS_U16 reg_mfe_g_enc_mode:2;		// 0/1/2/3: MPEG4/H263/H264/JPEG
			MS_U16 reg_mfe_g_frame_type:2;	// 0/1/2: I/P/B
			MS_U16 reg_mfe_g_ref_no:1;		// 0/1: 1 frame/2 frames
			MS_U16 reg_mfe_g_mbr_en:1;		// 0/1: disable/enable MB-level Rate control
			MS_U16 reg_mfe_g_qscale:6;		// frame level qscale: [H264]: 1 ~ 51; [MPEG4]: 1 ~ 31
			MS_U16 reg_mfe_g_rec_en:1;		// reconstruct enable
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_g_jpe_mst422_mode:1;	// YUV422 input buffer format; 0: YUYV, 1: MST422
#endif
		};
		MS_U16 reg00;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pic_width:12;	// picture width
#ifdef _MFE_M1_
			MS_U16 reg_mfe_g_ver_minor_idx:4;
#endif
		};
		MS_U16 reg01;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pic_height:12;	// picture height
#ifdef _MFE_M1_
			MS_U16 reg_mfe_g_ver_major_idx:4;
#endif
		};
		MS_U16 reg02;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_er_mode:2;		// 0/1/2/3: mby/bs/mby+bs/off
			MS_U16 reg_mfe_g_er_mby:2;		// 0/1/2/3: every 1/2/4/8 mb row(s) (error resilence)
			MS_U16 reg_mfe_g_packed_mode:1;	// frame buffer format for 422 packed mode; 0/1: YVYU/YUYV
			MS_U16 reg_mfe_g_qmode:1;			// quantization method; 0/1: h263/mp4
			MS_U16 reg_mfe_g_tbc_mode:1;		// table mode; 0: SW control, 1: HW control
			MS_U16 reg_mfe_g_fldpic_en:1;		//field picture coding
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_g_dct_only_en:1;	// reg_mfe_g_dct_only_en
#endif
#if defined(_MFE_AGATE_)
			MS_U16 reg_mfe_g_mstar_tile:1;	            // input buffer format; 0: m4ve tiled mode, 1: mstar tiled mode
			MS_U16 reg_mfe_g_mstar_tile_field_split:1;	// input field buffer format; 0: top/bottom fields interlaced, 1: fields split
			MS_U16 reg_mfe_g_fldpic_idx:1;
#endif
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
            MS_U16 reg_mfe_g_fldpic_multislice_en:1;      // multi-slice of field picture coding 0/1 : off/on
            MS_U16 reg_mfe_g_cabac_en:1;					// entropy encoding mode 0/1: cavlc/cabac
			MS_U16 reg03_dummy:1;
            MS_U16 reg_mfe_miu_sel:1;
#elif defined(_MFE_EDISON_)
            MS_U16 reg03_dummy:2;
            MS_U16 reg_mfe_ns:1;
            MS_U16 reg_mfe_miu_sel:1;
#endif
		};
		MS_U16 reg03;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_er_bs_th:16;	// er_bs mode threshold
		};
		MS_U16 reg04;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_inter_pref:16;	// inter prediction preference
		};
		MS_U16 reg05;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_cur_y_adr_low:16;	// current luma base address
		};
		MS_U16 reg06;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_cur_y_adr_high:ADDR_HI_BITS;	// current luma base address
		};
		MS_U16 reg07;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_cur_c_adr_low:16;	// current chroma base address
		};
		MS_U16 reg08;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_cur_c_adr_high:ADDR_HI_BITS;	// current chroma base address
		};
		MS_U16 reg09;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_y_adr0_low:16;	// reference luma base address0
		};
		MS_U16 reg0a;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_y_adr0_high:ADDR_HI_BITS;	// reference luma base address0
		};
		MS_U16 reg0b;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_y_adr1_low:16;	// reference luma base address1
		};
		MS_U16 reg0c;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_y_adr1_high:ADDR_HI_BITS;	// reference luma base address0
		};
		MS_U16 reg0d;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_c_adr0_low:16;	// reference chroma base address0
		};
		MS_U16 reg0e;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_c_adr0_high:ADDR_HI_BITS;	// reference chroma base address0
		};
		MS_U16 reg0f;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_c_adr1_low:16;	// reference chroma base address1
		};
		MS_U16 reg10;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_ref_c_adr1_high:ADDR_HI_BITS;	// reference chroma base address1
		};
		MS_U16 reg11;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_rec_y_adr_low:16;	// reconstructed luma base address
		};
		MS_U16 reg12;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_rec_y_adr_high:ADDR_HI_BITS;	// reconstructed luma base address
		};
		MS_U16 reg13;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_rec_c_adr_low:16;	// reconstructed chroma base address
		};
		MS_U16 reg14;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_rec_c_adr_high:ADDR_HI_BITS;	// reconstructed chroma base address
		};
		MS_U16 reg15;
	};
	union {
		struct {	// clock gating
			MS_U16 gate_cry_crc_sram:1;
			MS_U16 gate_qtab_dbfdc_dbqtb_sram:1;
			MS_U16 gate_mcy_mcc_sram:1;
			MS_U16 gate_res0_res1_sram:1;
			MS_U16 gate_ieap:1;
			MS_U16 gate_dct_idct:1;
			MS_U16 gate_dbf:1;
		};
		MS_U16 reg16;
	};
	union {
		struct {
#ifdef _MFE_M1_
			MS_U16 reg_mfe_s_auto_rst_wait_cnt:6;		// the waiting count for regen_soft_rstz and regen_fs_sw generation
            MS_U16 reg_mfe_g_sram1p_wp_type:1;        // "MFE 1p SRAM wrapper Type  'b1: Fix write-through problem 'b0: Original"
            MS_U16 reg_mfe_g_sram2p_wp_type:1;        // "MFE 2p SRAM wrapper Type  'b1: Fix write-through problem 'b0: Original"
            MS_U16 reg_mfe_g_clk_mfe_en:4;            // NOT used now.
            MS_U16 reg_mfe_g_mreq_sel:1;              // "1" D1 MIU clk gating; "0" dynamic MIU clk gating
            MS_U16 reg_mfe_g_mreq_always_active:1;    // "1" mreq always active; "0" make mreq active according to FSM. (let this be default)
            MS_U16 reg_mfe_g_clk_miu_d2_gate:1;       // b1: turn off miu clock of power-domain "dma" and sleep into d2 mode
            MS_U16 reg_mfe_g_clk_mfe_d2_gate:1;       // b1:  turn off mfe clock of power-domain "core" and sleep into d2 mode
#elif defined(_MFE_AGATE_)
            MS_U16 reg_mfe_s_auto_rst_wait_cnt:6;		// the waiting count for regen_soft_rstz and regen_fs_sw generation
            MS_U16 reg_mfe_g_clk_mfe_en_dummy:8;      // no use
            MS_U16 reg_mfe_g_clk_miu_d2_gate:1;       // b1: turn off miu clock of power-domain "dma" and sleep into d2 mode
            MS_U16 reg_mfe_g_clk_mfe_d2_gate:1;       // b1:  turn off mfe clock of power-domain "core" and sleep into d2 mode
#else
			MS_U16 reserved_reg17;
#endif
		};
		MS_U16 reg17;
	};
	// [JPEG]
	union {
		struct {
			MS_U16 reg_mfe_g_jpe_enc_mode:2;			// JPE encode mode; 2'b00/2'b01/2'b10/2'b11: 420/422/444/gray; current version supports 422 only
			MS_U16 reg_mfe_g_jpe_buffer_mode:1;		// JPE buffer mode; 0/1: double buffer mode/frame buffer mode
			MS_U16 reg_mfe_g_jpe_multibuf_mode:2;		// JPE multi-buffer mode; 0/1/2: 2/4/8 buffers
			MS_U16 reg_mfe_g_jpe_qfactor:4;			// JPE q factor; 0 ~ 15: (1 ~ 16)/4
			// (M1)JPE fsvs generation mode;
			// 0: pure sw
			// 1: sw+hw
			// 2: hw w/o auto-restart
			// 3: hw w/i auto-restart
			MS_U16 reg_mfe_g_jpe_fsvs_mode:2;			// (T8)JPE fsvs generation mode; 0/1/2: pure sw/sw+hw/hw
			MS_U16 reg18_dummy:4;
			MS_U16 reg_mfe_g_viu_soft_rstz:1;			// viu software reset; 0/1: reset/not reset
		};
		MS_U16 reg18;
	};
	// [MPEG4/H263]
	union {
		struct {
			MS_U16 reg_mfe_g_mp4_itlc:1;				// 0/1: MPEG4 progressive/interlaced mode
			MS_U16 reg_mfe_g_mp4_pskip_off:1;			// 0/1: MPEG4 enable/disable p skip mode
			MS_U16 reg_mfe_g_mp4_acp:2;				// [0]: 0/1: sw/hw acp selection; [1]: sw default value: 0/1: disable/enable acp; current version off
			MS_U16 reg_mfe_g_mp4_rounding_ctrl:1;		// mp4 rounding control specified as in spec
			MS_U16 reg_mfe_g_er_hec:1;				// 0/1: header extension code off/on
			MS_U16 reg_mfe_g_er_hec_t:3;				// HEC counter reset values
			MS_U16 reg_mfe_g_er_h263_unit:2;			// 0/1/2: unit is 1/2/4, for calculating gob_num.
			MS_U16 reg_mfe_g_mp4_direct_en:1;			// MPEG4 direct enable
			MS_U16 reg_mfe_g_mp4_direct_mvstore:1;	// [M]: enable storing of mv & skip_mb information to DRAM in P(or sometimes I) frame
		};
		MS_U16 reg19;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_mp4_direct_pref:8;		// used in mp4 only, mp4 direct mode preference value
			MS_U16 reg_mfe_g_mp4_direct_trb:3;		// used in mp4 only, mp4 direct mode trb (P0-B distance)
			MS_U16 reg_mfe_g_mp4_direct_trd:3;		// used in mp4 only, mp4 direct mode trd (P0-P1 distance)
#ifdef _MFE_AGATE_
            MS_U16 reg1a_dummy:1;
            MS_U16 reg_mfe_g_mb_pitch_en:1;
#endif
		};
		MS_U16 reg1a;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_mp4_flddct_diff_thr:8;	// used in mp4 only, mp4 field dct difference threshold
			MS_U16 reg_mfe_g_mp4_flddct_en:1;			// used in mp4 only, mp4 field dct enable
		};
		MS_U16 reg1b;
	};
	// [IRQ & important IP status checkings]
	union {
		struct {
			MS_U16 reg_mfe_g_irq_mask:8;				// 0/1: irq not-mask/mask
			MS_U16 reg_mfe_g_irq_force:8;				// 0/1: set corresponding interrupt as usual/force corresponding interrupt
		};
		MS_U16 reg1c;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_irq_clr0:1;				// 0/1: not clear interrupt/clear interrupt 0
			MS_U16 reg_mfe_g_irq_clr1:1;				// 0/1: not clear interrupt/clear interrupt 1
			MS_U16 reg_mfe_g_irq_clr2:1;				// 0/1: not clear interrupt/clear interrupt 2
			MS_U16 reg_mfe_g_irq_clr3:1;				// 0/1: not clear interrupt/clear interrupt 3
			MS_U16 reg_mfe_g_irq_clr4:1;				// 0/1: not clear interrupt/clear interrupt 4
			MS_U16 reg_mfe_g_irq_clr5:1;				// 0/1: not clear interrupt/clear interrupt 5
			MS_U16 reg_mfe_g_irq_clr6:1;				// 0/1: not clear interrupt/clear interrupt 6
			MS_U16 reg_mfe_g_irq_clr7:1;				// 0/1: not clear interrupt/clear interrupt 7
			MS_U16 reg_mfe_g_swrst_safe:1;			// to indicate there're no miu activities that need to pay attention to
		};
		MS_U16 reg1d;
	};
	union {
		struct {
			//status of interrupt on CPU side ({1'b0, net_trigger, fs_fail_irq, txip_time_out, early_bspobuf_full_irq/buf1_full, img_buf_full_irq, marb_bspobuf_ful1/buf0_full, frame_done_irq})
			//[3] SW mode: early obuf full; HW mode: buf1 full
			//[1]  SW mode: buf full; HW mode: buf0 full
			MS_U16 reg_mfe_g_irq_cpu:8;
			//status of interrupt on IP side ({1'b0, net_trigger, fs_fail_irq, txip_time_out, early_bspobuf_full_irq/buf1_full, img_buf_full_irq, marb_bspobuf_ful/buf0_full, frame_done_irq})
			//[3] SW mode: early obuf full; HW mode: buf1 full
			//[1]  SW mode: buf full; HW mode: buf0 full
			MS_U16 reg_mfe_g_irq_ip:8;
		};
		MS_U16 reg1e;
	};
	union {
		struct {
			MS_U16 reserved_reg1f;
		};
		MS_U16 reg1f;
	};
	// [ME setting]
	union {
		struct {
			MS_U16 reg_mfe_s_me_4x4_disable:1;		// 4x4_disable
			MS_U16 reg_mfe_s_me_8x4_disable:1;		// 8x4_disable
			MS_U16 reg_mfe_s_me_4x8_disable:1;		// 4x8_disable
			MS_U16 reg_mfe_s_me_16x8_disable:1;		// 16x8_disable
			MS_U16 reg_mfe_s_me_8x16_disable:1;		// 8x16_disable
			MS_U16 reg_mfe_s_me_8x8_disable:1;		// 8x8_disable
			MS_U16 reg_mfe_s_me_16x16_disable:1;		// 16x16_disable
			MS_U16 reg_mfe_s_mesr_adapt:1;			// me search range auto-adaptive; 0/1: off/on
			MS_U16 reg_mfe_s_me_ref_en_mode:2;		// ref enable mode: 2'b01/2'b10/2'b11: ref0 enable/ref1 enable/ref0&1 enable
		};
		MS_U16 reg20;
	};
	// [IME PIPELINE]
	union {
		struct {
			MS_U16 reg_mfe_s_ime_sr16:1;				// search range limited to (h,v) = (+/-16, +/-16); 0/1: search range 32/16
			MS_U16 reg_mfe_s_ime_umv_disable:1;		// 0/1: UMV enable/disable
			MS_U16 reg_mfe_s_ime_ime_wait_fme:1;		// 0/1: ime wait fme/fme wait ime
			MS_U16 reg_mfe_s_ime_boundrect_en:1;		// ime bounding rectangle enable (needed for level 3.0 and below)
			MS_U16 reg_mfe_s_ime_h264_p8x8_ctrl_en:1;	// ime h264 max p8x8 count control enable
			MS_U16 reg21_dummy:3;
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_ime_h264_p8x8_max:8;		// ime h264 max p8x8 count; value 0 is prohibited
														// Max P8x8 MB count = 16 * reg_mfe_s_ime_h264_p8x8_max

#else
			MS_U16 reg_mfe_s_ime_h264_p8x8_max:6;		// ime h264 max p8x8 count; value 0 is prohibited
														// Max P8x8 MB count = 16 * reg_mfe_s_ime_h264_p8x8_max
#endif
		};
		MS_U16 reg21;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_ime_mesr_max_addr:8;		// me search range max depth
			MS_U16 reg_mfe_s_ime_mesr_min_addr:8;		// me search range min depth
		};
		MS_U16 reg22;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_ime_mvx_min:6;			// me mvx min; 0/¡K/62 --> -32/¡K/30
			MS_U16 reg24_dummy:2;
			MS_U16 reg_mfe_s_ime_mvx_max:6;			// me mvx max; 0/¡K/62 --> -32/¡K/30
		};
		MS_U16 reg23;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_ime_mvy_min:6;			// me mvy min; 0/¡K/62 --> -32/¡K/30
			MS_U16 reg25_dummy:2;
			MS_U16 reg_mfe_s_ime_mvy_max:6;			// me mvy max; 0/¡K62/ --> -32/¡K/30
		};
		MS_U16 reg24;
	};
	// [FME pipeline]
	union {
		struct {
			MS_U16 reg_mfe_s_fme_quarter_disable:1;	// 0/1: Quarter fine-tune enable/disable
			MS_U16 reg_mfe_s_fme_half_disable:1;		// 0/1: Half fine-tune enable/disable
			MS_U16 /*reg_mfe_s_fme_one_mode*/reg26_dummy:1;
			MS_U16 reg_mfe_s_fme_pmv_enable:1;		// 0/1: disable/enable Previous Skip MV mode
			MS_U16 reg_mfe_s_fme_mode_no:1;			// 0: one mode.  1: two mode.
			MS_U16 reg_mfe_s_fme_mode0_refno:1;		// 0: one ref. for mode0   1: two ref. for mode0
			MS_U16 reg_mfe_s_fme_mode1_refno:1;		// 0: one ref. for mode1   1: two ref. for mode1
			MS_U16 reg_mfe_s_fme_mode2_refno:1;		// 0: one ref. for mode2   1: two ref. for mode2
			MS_U16 reg_mfe_s_fme_skip:1;				// fme skip
            MS_U16 reg_mfe_s_fme_pipeline_on:1;    //0/1: FME pipeline off/on
		};
		MS_U16 reg25;
	};
	// MBR
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_pqp_dlimit:2;	// previous qp diff limit
			MS_U16 reg_mfe_s_mbr_uqp_dlimit:2;	// upper qp diff limit
			MS_U16 reg_mfe_s_mbr_tmb_bits:12;		// target MB bits
		};
		MS_U16 reg26;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_frame_qstep:13;	// frame level qp's qstep
		};
		MS_U16 reg27;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_last_frm_avg_qp_low:16;	// last frame average qp (status register)
		};
		MS_U16 reg28;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_last_frm_avg_qp_high:LAST_FRAME_AVGQP_HI_BITS;	// last frame average qp (status register)
			MS_U16 reg_mfe_s_mbr_qp_cidx_offset:5;		// [H264] chroma qp index offset (+12). Spec range is [-12,12]
		};
		MS_U16 reg29;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_qp_min:6;				// qp min
			MS_U16 reg_mfe_s_mbr_qp_max:6;				// qp max
			MS_U16 reg_mfe_s_mvdctl_ref0_offset:2;		// H264 mvy offset adjustment for MCC if ref is frame 0: 0/1/2: 0/+2/-2
			MS_U16 reg_mfe_s_mvdctl_ref1_offset:2;		// H264 mvy offset adjustment for MCC if ref is frame 1: 0/1/2: 0/+2/-2
		};
		MS_U16 reg2a;
	};
	// IEAP
	union {
		struct {
			MS_U16 reg_mfe_s_ieap_last_mode:4;			// software control of the last mode of Intra4x4 mode 0 ~ 8
			MS_U16 reg_mfe_s_ieap_constraint_intra:1;		// software control constraint intra; 0/1: OFF/ON
			MS_U16 reg_mfe_s_ieap_ccest_en:1;				// software control cost estimator; 0/1: OFF/ON
			MS_U16 reg_mfe_s_ieap_ccest_thr:2;			// threshold of cost estimator set 0 ~ 3 for threshold 1 ~ 4
			MS_U16 reg_mfe_s_ieap_drop_i16:1;				// software control stop-Intra16x16-mode; 1:w/o I16M, 0:w/i I16MB
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
            MS_U16 reg_mfe_s_ieap_early_termination:1;    // 1/0: turn on/off ieap early termination
#endif
		};
		MS_U16 reg2b;
	};
	// QUAN
	union {
		struct {
			MS_U16 reg_mfe_s_quan_idx_last:6;		// the index of the last non-zero coefficient in the zig-zag order
			MS_U16 reg_mfe_s_quan_idx_swlast:1;	// software control of the index of the last non-zero coefficient in the zig-zag order; 0/1: disable/enable
#ifdef _MFE_AGATE_
            MS_U16 reg_mfe_g_mb_pitch:7;           // mb pitch (x-direction)
#endif
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
            MS_U16 reg2c_dummy:1;// dummy
            MS_U16 reg_mfe_g_ieap_sram_4x2_swap:1;//0/1: for ieap 4pels sram interface / for ieap 8pels sram interface
#endif
		};
		MS_U16 reg2c;
	};
	// TXIP control & debug
	union {
		struct {
			MS_U16 reg_mfe_s_txip_mbx:9;              //txip mbx
			MS_U16 reg_mfe_s_txip_sng_mb:1;           //0/1: disable/enable txip controller stop-and-go mechanism using (txip_mbx == reg_mfe_g_debug_trig_mbx) & (txip_mby == reg_mfe_g_debug_trig_mby)
			MS_U16 reg_mfe_s_txip_sng_set:1;          //txip controller stop-and-go mechanism using this register bit: 0/1: go/stop
			MS_U16 reg_mfe_s_txip_dbf_full_halt_en:1; //txip controller stop-and-go mechanism using double buffer fullness as criterion; 0/1: disable/enable
		};
		MS_U16 reg2d;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_txip_mby:9;				// txip mby
		};
		MS_U16 reg2e;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_txip_irfsh_mb_s0:13;	// intra refresh mb start 0
			MS_U16 reg2f_dummy:1;
#else
			MS_U16 reg_mfe_s_txip_irfsh_mb_s0:12;	// intra refresh mb start 0
#endif
			MS_U16 reg_mfe_s_txip_irfsh_en:2;		// intra refresh enable bits: bit0: enable condition 0; bit 1: enable condition 1
		};
		MS_U16 reg2f;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_txip_irfsh_mb_e0:13;	// intra refresh mb end 0
#else
			MS_U16 reg_mfe_s_txip_irfsh_mb_e0:12;	// intra refresh mb end 0
#endif
		};
		MS_U16 reg30;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_txip_irfsh_mb_s1:13;	// intra refresh mb start 1
#else
			MS_U16 reg_mfe_s_txip_irfsh_mb_s1:12;	// intra refresh mb start 1
#endif
		};
		MS_U16 reg31;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_txip_irfsh_mb_e1:13;	// intra refresh mb end 1
			MS_U16 reg32_dummy:1;
#else
			MS_U16 reg_mfe_s_txip_irfsh_mb_e1:12;	// intra refresh mb end 1
#endif
			MS_U16 reg_mfe_s_txip_timeout_en:1;	// txip time out enable
			MS_U16 reg_mfe_s_txip_wait_mode:1;	// txip waiting mode to move to next MB; 0/1: idle count/cycle count
		};
		MS_U16 reg32;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_txip_idle_cnt:16;	// wait mode is 0: txip idle count (x 64T)/ wait mode is 1: txip total processing count (x 64T)
		};
		MS_U16 reg33;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_txip_timeout:16;		// txip timeout count (x 64T)
		};
		MS_U16 reg34;
	};
	// [ECDB PIPELINE]
	// ECDB control & debug
	union {
		struct {
			MS_U16 reg_mfe_s_ecdb_mbx:9;			// ecdb mbx
		};
		MS_U16 reg35;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_ecdb_mby:9;			// ecdb mby
		};
		MS_U16 reg36;
	};
	// MDC
	union {
		struct {
			MS_U16 reg_mfe_s_mdc_total_mb_bw:4;	// total mb bit width used in video_pkt
			MS_U16 reg_mfe_s_mdc_m4vpktpzero:1;	// MPEG4 video packet preceding zeros: 0/1: 16/17 zeros
			MS_U16 reg_mfe_s_mdc_m4time:2;		// MPEG4 modulo time base: 0/1/2/3: 0/10/110/1110
			MS_U16 reg_mfe_s_mdc_m4iadcvlc_th:3;	// MPEG4 intra dc vlc threshold
			MS_U16 reg_mfe_s_mdc_m4vop_tinc_bw:4;	// vop_time_increment bit width
		};
		MS_U16 reg37;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mdc_m4vop_tinc:15;	//  vop_time_increment
		};
		MS_U16 reg38;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mdc_gob_frame_id:2;			//  H263 gob frame id
			MS_U16 reg_mfe_s_mdc_h264_nal_ref_idc:2;		// nal_ref_idc
			MS_U16 reg_mfe_s_mdc_h264_nal_unit_type:1;	//  0/1: 1/5
			MS_U16 reg_mfe_s_mdc_h264_fnum_bits:2;		// H264 frame num bits
			MS_U16 reg_mfe_s_mdc_h264_dbf_control:1;		// dbf control present flag
			MS_U16 reg_mfe_s_mdc_h264_fnum_value:8;		// H264 frame num value
		};
		MS_U16 reg39;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mdc_h264_idr_pic_id:3;
			MS_U16 reg_mfe_s_mdc_h264_disable_dbf_idc:2;
			MS_U16 reg_mfe_s_mdc_h264_alpha:4;				// slice_alpha_c0_offset_div2
			MS_U16 reg_mfe_s_mdc_h264_beta:4;					// slice_beta_offset_div2
			MS_U16 reg_mfe_s_mdc_h264_ridx_aor_flag:1;		// reference index active override flag
		};
		MS_U16 reg3a;
	};
	// BSPOBUF/MVOBUF
	union {
		struct {
			MS_U16 reg_mfe_s_bspobuf_set_adr:1;			// set bsp obuf start address(write one clear)
			MS_U16 reg_mfe_s_mvobuf_set_adr:1;			// set mv obuf start address (write one clear)
			MS_U16 reg_mfe_s_bspobuf_fifo_th:3;			// bsp obuf threshold
			MS_U16 reg_mfe_s_mvobuf_fifo_th:3;			// mv obuf threshold
			MS_U16 reg_mfe_s_bsp_fdc_skip:1;				// fdc skip enable; 0: fdc skip disable, 1: fdc skip enable
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 dummy_reg3b:5;
			MS_U16 reg_mfe_s_obuf_toggle_obuf0_status:1;	// toggle buf0 status(write one clear)
			MS_U16 reg_mfe_s_obuf_toggle_obuf1_status:1;	// toggle buf1 status(write one clear)
#elif !defined(_MFE_T8_)
			MS_U16 reg_mfe_s_txip_irfsh_mb_e1:13;	// intra refresh mb end 1
#endif
		};
		MS_U16 reg3b;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_bspobuf_low:16;					    // bsp obuf start address
#else
			MS_U16 reg_mfe_s_bspobuf_sadr_low:16;					// bsp obuf start address
#endif
		};
		MS_U16 reg3c;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_bspobuf_high:OUTBUF_HI_BITS;		// bsp obuf address high
			MS_U16 reg3d_dummy_bits:(16-OUTBUF_HI_BITS-2);
            MS_U16 reg_mfe_s_obuf_id:2;                   // 00: s0, 01: e0, 10: s1, 11: e1
#else
			MS_U16 reg_mfe_s_bspobuf_sadr_high:OUTBUF_HI_BITS;	// bsp obuf start address
#endif

		};
		MS_U16 reg3d;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_obuf_write_id_adr:1;             // write to this address to enable writing of bspobuf address
#else
			MS_U16 reg_mfe_s_bspobuf_eadr_low:16;					// bsp obuf end address
#endif
		};
		MS_U16 reg3e;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_s_bspobuf_hw_en:1;					// enable HW obuf automatic mechanism
			MS_U16 reg_mfe_s_bspobuf_update_adr:1;			// update obuf address(write one clear)
			MS_U16 reg_mfe_s_bspobuf_adr_rchk_sel:2;			// obuf adr read back check selection: 0/1/2/3: s0/e0/s1/e1
			MS_U16 reg_mfe_s_bspobuf_adr_rchk_en:1;			// enable bspobuf adr read back check through reg_mfe_s_bspobuf_wptr
            MS_U16 dummy_reg3f:3;
            MS_U16 reg_mfe_s_bsp_fdc_offset:7;                // bsp's fdc offset
#else
			MS_U16 reg_mfe_s_bspobuf_eadr_high:OUTBUF_HI_BITS;	// bsp obuf end address
#endif
		};
		MS_U16 reg3f;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mvobuf_sadr_low:16;					// mv obuf start address
		};
		MS_U16 reg40;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mvobuf_sadr_high:OUTBUF_HI_BITS;		// mv obuf start address
		};
		MS_U16 reg41;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bsp_bit_cnt_low:16;	// encoded bit count (one frame)
		};
		MS_U16 reg42;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bsp_bit_cnt_high:BITCOUNT_HI_BITS;	// encoded bit count (one frame)
		};
		MS_U16 reg43;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bspobuf_wptr_low:16;	// bspobuf write pointer (8 byte unit)
		};
		MS_U16 reg44;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bspobuf_wptr_high:OUTBUF_HI_BITS;	// bspobuf write pointer (8 byte unit)
		};
		MS_U16 reg45;
	};
	// FDC
	union {
		struct {
			MS_U16 reg_mfe_s_fdc_bs:16;		// cpu to fdc bitstream data
		};
		MS_U16 reg46;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_fdc_bs_len:5;		// cpu to fdc bitstream len; 0 ~ 16
			MS_U16 reg_mfe_s_fdc_bs_count:10;		// cpu to fdc round count
		};
		MS_U16 reg47;
	};
	// [Table Control]
	union {
		struct {
			MS_U16 reg_mfe_s_fdc_ack:1;					// fdc to cpu ack; 0/1: frame data pool not empty/frame data pool empty; 48x64 bits of space
			MS_U16 reg_mfe_s_fdc_done_clr:1;		// fdc done clear (write one clear)
			MS_U16 reg_mfe_s_fdc_done:1;				// fdc done; indicate to CPU that data has been written to internal buffer
			MS_U16 reg48_dummy:8;
			MS_U16 reg_mfe_s_fdc_bs_vld:1;			// set for bitstream write out (write one clear)
			MS_U16 reg_mfe_s_tbc_en:1;			// set for table read & write ; 1: enable, 0: disable (write one clear)
		};
		MS_U16 reg48;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_tbc_rw:1;				// table mode; 0: read, 1: write
			MS_U16 reg_mfe_s_tbc_done_clr:1;			// table done clear (write one clear)
			MS_U16 reg_mfe_s_tbc_done:1;				// table done; indicate to CPU that (1) data has been written to table (2) table output is ready at reg_mfe_s_tbc_rdata
			MS_U16 reg49_dummy:5;
			MS_U16 reg_mfe_s_tbc_adr:6;				// table address
		};
		MS_U16 reg49;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_tbc_wdata:16;			// table write data
		};
		MS_U16 reg4a;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_tbc_rdata:16;			// table read data
		};
		MS_U16 reg4b;
	};
	// [Get Neighbor]
	union {
		struct {
			MS_U16 reg_mfe_s_gn_sadr_low:16;				// gn base adr low
		};
		MS_U16 reg4c;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_gn_sadr_high:OUTBUF_HI_BITS;	// gn base adr high
#ifdef _MFE_M1_
		  #if (16-OUTBUF_HI_BITS-1>0)
            MS_U16 dummy_reg4d:16-OUTBUF_HI_BITS-1;
          #endif
            MS_U16 reg_mfe_s_gn_sadr_mode:1;  // 1: gn save data in one frame   0: gn save data in one row
#endif
		};
		MS_U16 reg4d;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_gn_mvibuf_sadr_low:16;				// mv ibuf start address low
		};
		MS_U16 reg4e;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_gn_mvibuf_sadr_high:OUTBUF_HI_BITS;	// mv ibuf start address high
#if defined(MFE_DBF_PACKED_MODE)
#if (16-OUTBUF_HI_BITS-2 > 0)
            MS_U16 reg4f_dummy:(16-OUTBUF_HI_BITS-2);
#endif
            MS_U16 reg_mfe_s_gn_bwr_mode:2;  // bit[0]: 0, DBF bottom row data write to reconstructed address,
                                             //         1, DBF bottom row data write to IMI
                                             // bit[1]: DBF packed mode. 0: disable, 1: enable
#endif
		};
		MS_U16 reg4f;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_rp0_promote:4;
            MS_U16 reg_mfe_s_marb_rp1_promote:4;
            MS_U16 reg_mfe_s_marb_rp2_promote:4;
            MS_U16 reg_mfe_s_marb_rp3_promote:4;
		};
		MS_U16 reg51;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_mrpriority_thd:4;
            MS_U16 reg_mfe_s_marb_mwpriority_thd:4;
            MS_U16 reg_mfe_s_marb_rp4_occupy:4;
            MS_U16 reg_mfe_s_marb_rp4_promote:4;
		};
		MS_U16 reg55;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_mrpriority_sw:2;		// mfe2mi_rpriority software programmable
			MS_U16 reg_mfe_s_marb_mr_timeout_ref:1;	//miu read burst timeout count start point
			MS_U16 reg_mfe_s_marb_mr_nwait_mw:1;		//miu read not wait mi2mfe_wrdy
			MS_U16 reg_mfe_s_marb_mwpriority_sw:2;		//mfe2mi_wpriority software programmable
			MS_U16 reg_mfe_s_marb_mw_timeout_ref:1;	//miu write burst timeout count start point
			MS_U16 reg_mfe_s_marb_mw_nwait_mr:1;		//miu read not wait mi2mfe_wrdy
			MS_U16 reg_mfe_s_marb_mr_pending:4;		//max. pending read requests to miu
			MS_U16 reg_mfe_s_marb_32b_ad_nswap:1;		//32bits miu address not swap. only for 32bits mode
			MS_U16 reg_mfe_s_marb_miu_wmode:1;		//0/1: original miu protocol/new miu protocol(wd_en)

		};
		MS_U16 reg56;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_0_low:16;	// MIU protect for MPEG4 BSP obuf
		};
		MS_U16 reg58;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_0_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 BSP obuf
			MS_U16 reg59_dummy:(16-OUTBUF_HI_BITS);

		};
		MS_U16 reg59;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_0_low:16;	// MIU protect for MPEG4 BSP obuf
		};
		MS_U16 reg5a;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_0_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 BSP obuf
#ifdef _MFE_A3_
            MS_U16 reg_mfe_s_marb_miu_bound_en_0:1;	//
#endif
		};
		MS_U16 reg5b;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_1_low:16;	// MIU Upper bound protect for MPEG4 MC obuf  rec
		};
		MS_U16 reg5c;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_1_high:OUTBUF_HI_BITS;	// MIU Upper bound protect for MPEG4 MC obuf rec
		};
		MS_U16 reg5d;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_1_low:16;	// MIU Lower bound protect for MPEG4 MC obuf
		};
		MS_U16 reg5e;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_1_high:OUTBUF_HI_BITS;	// MIU Lower bound protect for MPEG4 MC obuf
#ifdef _MFE_A3_
            MS_U16 reg_mfe_s_marb_miu_bound_en_1:1;	//
#endif
		};
		MS_U16 reg5f;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_2_low:16;	// MIU protect for MPEG4 MV obuf
		};
		MS_U16 reg60;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_2_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 MV obuf
		};
		MS_U16 reg61;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_2_low:16;	// MIU protect for MPEG4 MV obuf
		};
		MS_U16 reg62;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_2_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 MV obuf
#ifdef _MFE_A3_
            MS_U16 reg_mfe_s_marb_miu_bound_en_2:1;	//
#endif
		};
		MS_U16 reg63;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_3_low:16;	// MIU protect for MPEG4 GN
		};
		MS_U16 reg64;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_ubound_3_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 GN
		};
		MS_U16 reg65;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_3_low:16;	// MIU protect for MPEG4 GN
		};
		MS_U16 reg66;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_lbound_3_high:OUTBUF_HI_BITS;	// MIU protect for MPEG4 GN
#ifdef _MFE_A3_
            MS_U16 reg_mfe_s_marb_miu_bound_en_3:1;	//
            MS_U16 reg_mfe_s_marb_miu_off:1;	        //
            MS_U16 reg_mfe_s_marb_miu_bound_err:1;	    //
#endif
		};
		MS_U16 reg67;
	};
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
	union {
		struct {
			MS_U16 reg_mfe_s_marb_eimi_block:1;	// miu emi/imi block, 0: disable
			MS_U16 reg_mfe_s_marb_lbwd_mode:1;	// low bandwidth mode, 0: disable
            MS_U16 reg_mfe_s_marb_imi_burst_thd:5;//imi write burst bunch up threshold
            MS_U16 reg_mfe_s_marb_imi_timeout:3;  //imi write burst timeout
            MS_U16 reg_mfe_s_marb_imilast_thd:4;  //auto mfe2imi_last threshold
			MS_U16 reg_mfe_s_prfh_cryc_en:1;      // 0: disable prfh_cryc circuit, 1: enable prfh_cryc circuit
			MS_U16 reg_mfe_s_prfh_refy_en:1;      // 0: disable prfh_refy circuit, 1: enable prfh_refy circuit
		};
		MS_U16 reg68;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pat_gen_init:16;					// pattern generation initial value
		};
		MS_U16 reg69;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_bspobuf_idx:2;					// HW obuf index(0->1->2->3->0->1¡K)
			// SW obuf index(00->01->11->10->00...)
            MS_U16 reg_mfe_s_obuf0_status:1;
            MS_U16 reg_mfe_s_obuf1_status:1;
			// reg_mfe_s_bspobuf_adr_status:6 {fifo_not_full, fifo_not_empty, enable_obufadr_update, obufadr_update_cnt[2:0]}
			MS_U16 obufadr_update_cnt:3;
            MS_U16 enable_obufadr_update:1;
            MS_U16 fifo_not_empty:1;
            MS_U16 fifo_not_full:1;
		};
		MS_U16 reg6a;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_imi_sadr_low:16;	// imi base address for low bandwidth mode (in 8-byte unit)
		};
		MS_U16 reg6b;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_marb_imi_sadr_high:IMIBUF_HI_BITS;
            MS_U16 reg_mfe_s_marb_rimi_force:1;
			MS_U16 reg_mfe_s_marb_imi_cache_size:2;	// imi cache size (0: 32kB, 1:64kB, 2:8kB, 3:16kB)
		};
		MS_U16 reg6c;
	};
	union {
		struct {
            MS_U16 reg_mfe_s_marb_imipriority_thd:4;  //hardware mfe2imi_priority threshold
            MS_U16 reg_mfe_s_marb_imipriority_sw:2;   //mfe2imi_priority software programmable
            MS_U16 dummy:2;
			MS_U16 reg_mfe_s_prfh_cryc_idle_cnt:4;	// prfh idle count (x 16T) for prfh_cryc
			MS_U16 reg_mfe_s_prfh_refy_idle_cnt:4;	// prfh idle count (x 16T) for prfh_refy
		};
		MS_U16 reg6d;
	};
#endif
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_qstep_min:7;	// qstep min (note: max value of qstep_min is 128 because condition is qstep <= qstep_min)
		};
		MS_U16 reg6e;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_mbr_qstep_max:13; // qstep max
		};
		MS_U16 reg6f;
	};
	union {
		struct {
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_g_debug_mode:7;			// debug mode
			MS_U16 reg_mfe_g_debug_trig_cycle:9;		// wait (8 * reg_mfe_g_debug_trig_cycle) cycles
#else
			MS_U16 reg_mfe_g_debug_mode:6;			// debug mode
			MS_U16 reg_mfe_g_debug_trig_cycle:10;		// wait (8 * reg_mfe_g_debug_trig_cycle) cycles
#endif
		};
		MS_U16 reg70;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_debug_trig_mbx:9;		// debug trigger mbx
		};
		MS_U16 reg71;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_debug_trig_mby:9;		// debug trigger mby
		};
		MS_U16 reg72;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_debug_trig:1;			// reg trigger (write one clear)
			MS_U16 reg_mfe_g_debug_trig_mode:2;		// debug trigger mode; 0/1/2/3: reg_trigger/3rd stage (mbx, mby)/frame start
			MS_U16 reg_mfe_g_debug_en:1;				// debug enable
			MS_U16 reg_mfe_g_crc_mode:4;				//'h0: Disable,¡¥hc: bsp obuf, 'hd: mc obuf, 'hd: mc obuf
            MS_U16 reg_mfe_g_debug_tcycle_chk_en:1; //enable total cycle check
            MS_U16 reg_mfe_g_debug_tcycle_chk_sel:1; //select total cycle and report it on reg_mfe_g_crc_result[15:0]
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
			MS_U16 reg_mfe_g_sw_buffer_mode:1;		// 0/1: hw/sw buffer mode
			MS_U16 reg_mfe_g_sw_row_done:1;			// sw row done (1T clk_jpe) (write one clear)
			MS_U16 reg_mfe_g_sw_vs:1;					// sw vsync (1T clk_jpe) (write one clear)
			MS_U16 reg_mfe_g_pat_gen_en:1;			// enable pattern generation
            MS_U16 reg_mfe_g_row_down_irq_en:1;       // row done irq enable; 0: disable, 1: enable
            MS_U16 reg_mfe_g_vs_irq_en:1;             // vs irq enable; 0: disable, 1: enable
#endif
		};
		MS_U16 reg73;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_debug_state0:16;			// "debug state for TXIP/ECDB submodule {txip2q_en, txip2iq_en, txip2mbr_en, txip2zmem_en, txip2dpcm_en,
			//  txip2mve_en, txip2mcobuf_en, txip2mbldr_en, ecdb2mdc_en,
			// ecdb2rlc_en, ecdb2vlc_en, 5'd0}"
		};
		MS_U16 reg74;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_debug_state1;			// "debug state for ME submodule {3'd0, load_w4_ok, load_w3_ok, load_w2_ok, load_w1_ok, load_w0_ok,  2'd0,
			//	busy_ime, busy_fme, busy_mesr, busy_iacost, end_this_mb, init_this_mb"
		};
		MS_U16 reg75;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_crc_result0:16;	// CRC64[15..0]
		};
		MS_U16 reg76;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_crc_result1:16;	// CRC64[31..16]
		};
		MS_U16 reg77;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_crc_result2:16;	// CRC64[47..32]
		};
		MS_U16 reg78;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_crc_result3:16;	// CRC64[63..48]
		};
		MS_U16 reg79;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_bist_fail0;
		};
		MS_U16 reg7a;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_bist_fail1;
		};
		MS_U16 reg7b;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_bist_fail2;
		};
		MS_U16 reg7c;
	};
	union {
		struct {
#ifdef _MFE_M1_
            MS_U16 reg_mfe_s_txip_eco0:1;//0: original, 1: fix mbldr_cry_done, mbldr_crc_done bug
            MS_U16 reg_mfe_s_txip_eco1:1; //reserved registers
            MS_U16 reg_mfe_rsv0:14;       //reserved registers
#elif defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
            MS_U16 reg_mfe_s_txip_eco0:1;     //0: fix mbldr_cry_done, mbldr_crc_done bug, 1: original
            MS_U16 reg_mfe_s_dummy0:1;
			MS_U16 reg_eco_mreq_stallgo:1;
			MS_U16 reg_eco_marb_stallgo:1;
			MS_U16 reg_mfe_s_dummy1:1;
            MS_U16 reg_eco_bsp_stuffing:1;
			MS_U16 reg_eco_bsp_rdy_fix:1;
			MS_U16 reg_eco_bsp_multi_slice_fix:1;
			MS_U16 reg_mfe_s_reserved2:6;
            MS_U16 reg_mfe_g_secure_obufadr:1;
            MS_U16 reg_mfe_g_secure_miu_sel:1;
#else
            MS_U16 reg_mfe_s_txip_eco0:1;            //0: original, 1: fix mbldr_cry_done, mbldr_crc_done bug
            MS_U16 reg_mfe_s_txip_eco1:1;            //reserved registers
            MS_U16 reg_eco_mreq_stallgo:1;           //reg_eco_mreq_stallgo
            MS_U16 reg_eco_marb_stallgo:1;           //reg_eco_marb_stallgo
            MS_U16 reg_mfe_dummy:2;
            MS_U16 reg_eco_bsp_rdy_fix:1;
            MS_U16 reg_eco_bsp_multi_slice_fix:1;
            MS_U16 reg_mfe_reg7d_dummy2:6;
            MS_U16 reg_mfe_g_secure_obufadr:1;
            MS_U16 reg_mfe_g_secure_miu_sel:1;
#endif
		};
		MS_U16 reg7d;
	};
	union {
		struct {
			MS_U16 reg_mfe_rsv1;
		};
		MS_U16 reg7e;
	};
	union {
		struct {
                    MS_U16 reg_mfe_rsv2:12;
                    MS_U16 reg_mfe_s_mcc_prldr_mode:4;
		};
		MS_U16 reg7f;
	};

} MFE_REG;

typedef struct _mfe_reg1_ {
	// [GLOBAL SETTING]
	union {
		struct {
			MS_U16 reg_mfe_s_pp_en:1;	                // Video Stabilization Enable, 1: on, 0: off
			MS_U16 reg_mfe_s_pp_mw_burst_thd:5;	    // MI write burst bunch up threshold
			MS_U16 reg_mfe_s_pp_mw_timeout:3;		    // IMI write burst timeout
			MS_U16 reg_mfe_s_pp_mw_timeout_ref:1;	    // IMI write burst timeout count start point
			MS_U16 reg_mfe_s_pp_burst_split:2;		// 0: disable; N: MIU Request IDLE ¡§N¡¨ cycles with every last signal
			MS_U16 reg_mfe_s_pp_32b_ad_nswap:1;		// 32bits miu address not swap. only for 32bits mode
			MS_U16 reg_mfe_s_00_dummy:3;		            // dummy bits
		};
		MS_U16 reg00;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_pp_mwlast_thd:5;	        // auto mfepp2imi_wlast threshold
            MS_U16 reg_mfe_s_pp_mwpriority_sw:2;      // mfepp2imi_wpriority software programmable
            MS_U16 reg_mfe_s_pp_mwpriority_thd:4;     // hardware mfepp2imi_wpriority threshold
            MS_U16 reg_mfe_s_pp_mrlast_thd:5;         // auto mfepp2mi_rlast threshold
		};
		MS_U16 reg01;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_pp_mrpriority_sw:2;	    // mfepp2mi_rpriority software programmable
            MS_U16 reg_mfe_s_pp_mrpriority_thd:4;     // hardware mfepp2mi_rpriority threshold
            MS_U16 reg_mfe_s_02_dummy:10;
		};
		MS_U16 reg02;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf0_badr_y_low:16;	// Current Y IMI buffer 0 base address( 256 bytes aligned )
		};
		MS_U16 reg03;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf0_badr_y_high:8;	// Current Y IMI buffer 0 base address( 256 bytes aligned )
		};
		MS_U16 reg04;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf0_badr_c_low:16;	// Current CbCr IMI buffer 0 base address( 256 bytes aligned )
		};
		MS_U16 reg05;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf0_badr_c_high:8;	// Current CbCr IMI buffer 0 base address( 256 bytes aligned )
		};
		MS_U16 reg06;
	};
	union {
		struct {
            MS_U16 reg_mfe_g_pp_buf1_badr_y_low:16;   // Current Y IMI buffer 1 base address( 256 bytes aligned )
		};
		MS_U16 reg07;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf1_badr_y_high:8;	// Current Y IMI buffer 1 base address( 256 bytes aligned )
		};
		MS_U16 reg08;
	};
	union {
		struct {
            MS_U16 reg_mfe_g_pp_buf1_badr_c_low:16;   // Current CbCr IMI buffer 1 base address( 256 bytes aligned )
		};
		MS_U16 reg09;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_pp_buf1_badr_c_high:8;	// Current CbCr IMI buffer 1 base address( 256 bytes aligned )
		};
		MS_U16 reg0a;
	};
    union {
        struct {
            MS_U16 reg_mfe_g_mb_pitch:12;	            // mb pitch (x-direction)
            MS_U16 reg_mfe_g_mb_pitch_en:1;	        // 0: mb pitch off, 1: mb pitch enabled
        };
        MS_U16 reg0b;
    };
	union {
		struct {
			MS_U16 reg_mfe_g_capture_width_y:12;      // Capture input frame width (Luma Pels)
		};
		MS_U16 reg10;
	};
	union {
		struct {
#if defined(HW_SUPPORT_ANDROID_YV12)
            MS_U16 reg_mfe_g_capture_width_c:12;	    // Capture input frame width (Chroma Pels)
#else
            MS_U16 reg_mfe_g_capture_hright_y:12;	    // Capture input frame height (Luma Pels)
#endif
		};
		MS_U16 reg11;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_cury_offset:3;       // Encoding frame Y shift
            MS_U16 reg_mfe_g_enc_cury_adr_low:13;     // Encoding frame Y base address (8 bytes aligned)
		};
		MS_U16 reg12;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_cury_adr_high:16;	// Encoding frame Y base address (8 bytes aligned)
		};
		MS_U16 reg13;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_curcb_offset:3;       // Encoding frame Cb shift
            MS_U16 reg_mfe_g_enc_curcb_adr_low:13;     // Encoding frame Cb base address (8 bytes aligned)
		};
		MS_U16 reg14;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_curcb_adr_high:16;	// Encoding frame Cb base address (8 bytes aligned)
		};
		MS_U16 reg15;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_curcr_offset:3;       // Encoding frame Cr shift
            MS_U16 reg_mfe_g_enc_curcr_adr_low:13;     // Encoding frame Cr base address (8 bytes aligned)
		};
		MS_U16 reg16;
	};
	union {
		struct {
			MS_U16 reg_mfe_g_enc_curcr_adr_high:16;	// Encoding frame Cr base address (8 bytes aligned)
		};
		MS_U16 reg17;
	};
	union {
		struct {
			MS_U16 reg_mfe_s_pp_semiplaner:1;	// 0: y, cb, cr palner mode 1: y, cb/cr semiplaner mode
		};
		MS_U16 reg18;
	};
    union {
        struct {
            MS_U16 reg_mfe_s_jpe_rst_mcu_cnt_low:16;	// JPE MCU count for restart marker
        };
        MS_U16 reg20;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_jpe_rst_mcu_cnt_high:10;	// JPE MCU count for restart marker
            MS_U16 reg_mfe_s_jpe_rst_en:1;	        // JPE restart marker enable
        };
        MS_U16 reg21;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_cabac_bin_count_low:16;  // CABAC total bin counts
        };
        MS_U16 reg30;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_cabac_bin_count_high:16; // CABAC total bin counts
        };
        MS_U16 reg31;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_cabac_bit_count_low:16;  // CABAC total bit counts
        };
        MS_U16 reg32;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_cabac_bit_count_high:16; // CABAC total bit counts
        };
        MS_U16 reg33;
    };

    union {
        struct {
            MS_U16 reg_mfe_s_bspobuf_low:16;                        // bsp obuf start address
        };
        MS_U16 reg34;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_bspobuf_high:(MFE_MIAW-16);        // bsp obuf address high
            MS_U16 reg35_dummy_bits:(16-(MFE_MIAW-16)-2);
            MS_U16 reg_mfe_s_obuf_id:2;                   // 00: s0, 01: e0, 10: s1, 11: e1

        };
        MS_U16 reg35;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_obuf_write_id_adr:1;         //write to this address to enable witring of bspobuf address
            MS_U16 reg_mfe_miu_sel:2;                     //select miu bank; 0: bank0, 1:bank1
#if defined(MFE_SUPPORT_TLB)
            MS_U16 reg_mfe_tlb:1;                         // MIU TLB remap enable
            MS_U16 reg36_dummy_bits:12;
#else
            MS_U16 reg36_dummy_bits:13;
#endif
        };
        MS_U16 reg36;
    };
    union {
        struct {
            MS_U16 reg_mfe_g_secure_obufadr:1;                        // Enable security control (Obuf adr)
            MS_U16 reg_mfe_g_secure_miu_sel:1;                         // Enable security control (MIU sel)
            MS_U16 reg37_dummy_bits:14;
        };
        MS_U16 reg37;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_secure_bank_sel:1;                        // select security bank; 0: bank0, 1:bank1
            MS_U16 reg38_dummy_bits:15;
        };
        MS_U16 reg38;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_whist_en:1;                        // Write histogram enable
            MS_U16 reg_mfe_s_whist_count_range:1;         // Write histogram count range or total
            MS_U16 reg_mfe_s_whist_mode:2;             // Write histogram mode 0: Request count 1: Reserved 2: Latch count 3: Latch count without first data
            MS_U16 reg_mfe_s_whist_burst_mode:1;   //Write histogram burst count mode 0: Count request 1: Count burs
            MS_U16 reg_mfe_s_whist_upper_bound_en:1;                        //Write histogram upper bound enable
            MS_U16 reg_mfe_s_whist_lower_bound_en:1;                        //Write histogram lower bound enable
            MS_U16 reg40_dummy_bits:9;
        };
        MS_U16 reg40;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_whist_upper_bound:16;                        // Write histogram upper bound
        };
        MS_U16 reg41;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_whist_lower_bound:16;                        // Write histogram lower bound
        };
        MS_U16 reg42;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_whist_count_bubble:16;                        // Write histogram bubble count
        };
        MS_U16 reg43;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_whist_count_burst:16;                        // Write histogram burst count
        };
        MS_U16 reg44;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_rhist_en:1;                        // Read histogram enable
            MS_U16 reg_mfe_s_rhist_count_range:1;         // Read histogram count range or total
            MS_U16 reg_mfe_s_rhist_mode:2;             // Read histogram mode 0: Request count 1: Reserved 2: Latch count 3: Latch count without first data
            MS_U16 reg_mfe_s_rhist_burst_mode:1;   //Read histogram burst count mode 0: Count request 1: Count burs
            MS_U16 reg_mfe_s_rhist_upper_bound_en:1;                        //Read histogram upper bound enable
            MS_U16 reg_mfe_s_rhist_lower_bound_en:1;                        //Read histogram lower bound enable
            MS_U16 reg45_dummy_bits:9;
        };
        MS_U16 reg45;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_rhist_upper_bound:16;                        // Read histogram upper bound
        };
        MS_U16 reg46;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_rhist_lower_bound:16;                        // Read histogram lower bound
        };
        MS_U16 reg47;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_rhist_count_bubble:16;                        // Read histogram bubble count
        };
        MS_U16 reg48;
    };
    union {
        struct {
            MS_U16 reg_mfe_s_rhist_count_burst:16;                        // Read histogram burst count
        };
        MS_U16 reg49;
    };
    union {
    	struct {
            MS_U16 reg_mfe_g_jpd_hsk_en:1;	            // JPD handshake enable
            MS_U16 reg_mfe_s_packed422_en:1;	            // MFE YUV422 input enable
            MS_U16 reg_mfe_s_packed422_endian:1;	        // Endianness of YUV422
            MS_U16 reg_mfe_s_packed422_yc_swap:1;	        // YUV422 YC swap mode
            MS_U16 reg_mfe_s_packed422_uv_swap:1;	        // YUV422 UV swap mode
            MS_U16 reg_mfe_s_packed422_delta:8;           // 8-bit, Delta between JPD counter and MFE counter
            MS_U16 reg_mfe_g_jpd_hsk_shot:1;              // Shot for JPD handshake enable
            MS_U16 reg_mfe_g_jpd_hsk_mfe:1;               // JPD handshake enable in MFE
            MS_U16 reg_mfe_g_jpd_frame_done_mask:1;       // 0: Monitor JPD framedone when handshake enable 1: Disable JPD framedone monitor
    	};
    	MS_U16 reg52;
    };

#if defined(MFE_YUV_LOADER)
    union {
        struct {
            MS_U16 reg_mfe_g_yuvldr_en:1;            // YUV data loader
            MS_U16 reg_mfe_g_hevd_tile:1;            // EVD tile (32x16)
            MS_U16 reg_mfe_g_hevd_tile32:1;          // 32x32 tile
            MS_U16 reg_mfe_g_yuv420_semi:1;          // YUV420 semi-planar
            MS_U16 reg_mfe_g_yuv420_semi_uv_swap:1;  // YUV420 semi-planar uv-swap
            MS_U16 reg56_dummy_bits:11;
        };
        MS_U16 reg56;
    };
#endif
} MFE_REG1;

//////////////////////////////////////////////////////////////////////////
// PERFORMANCE
//////////////////////////////////////////////////////////////////////////

#define CLOCK_GATING	// Enable clock gating

#define FME_PIPELINE_OPEN	// Enable David's FME speedup version

//#define DONT_PUT_FDC

//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: FDC && QTable
//////////////////////////////////////////////////////////////////////////

//#define CHECK_FDC_DONE	// Verify if hw receives the fdc command

//#define CHECK_WriteQTable_DONE	// Verify if hw receives the WriteQTable command

//#define QTABLE_READBACK_CHECK	// Verify all WriteQTable Value

//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: StopAndGo series
//////////////////////////////////////////////////////////////////////////

#define STOP_FRAME 0UL
#define STOP_MBX   7UL
#define STOP_MBY   7UL
// #define TEST_MB_STOPANDGO
// #define TEST_MB_STOPANDDROP

// #define TEST_STOPANDGO
// #define TEST_STOPANDDROP


//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: test miu protection
//////////////////////////////////////////////////////////////////////////


#define TEST_MIU_PROTECTION_MODE 0UL





#ifdef REG_JPEG_CMODEL

		//////////////////////////////////////////////////////////////////////////
		// DEBUG Flags: continuous shot test (JPEG only)
		//////////////////////////////////////////////////////////////////////////

		//#define TEST_CONTINUOUS_SHOT
		//#define CONTINUOUS_SHOT_NUMBER	5UL	//number of test shot

		//////////////////////////////////////////////////////////////////////////
		// DEBUG Flags: input row mode test (JPEG only)
		//////////////////////////////////////////////////////////////////////////
		//JPEG row mode only!
//		#define TEST_INPUT_ROW_MODE_HW

			#ifdef TEST_INPUT_ROW_MODE_HW
				#ifndef DONT_PUT_FDC
					#define DONT_PUT_FDC
				#endif

				#define	NUM_OF_ROW_DONE_BEFORE_FS	1UL
			#endif

		//JPEG row mode only! only check fs_fail_irq
		//#define TEST_INPUT_ROW_MODE_SW_HW
			#ifdef TEST_INPUT_ROW_MODE_SW_HW
				#define	NUM_OF_ROW_DONE_BEFORE_FS	1UL
			#endif
#endif // REG_JPEG_CMODEL
//////////////////////////////////////////////////////////////////////////
// DEBUG Flags: test CRC mode
//////////////////////////////////////////////////////////////////////////
#define TEST_CRC_MODE

#endif

