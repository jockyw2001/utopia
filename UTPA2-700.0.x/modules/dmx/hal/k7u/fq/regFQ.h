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
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regFQ.h
//  Description: FQ Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _FQ_REG_H_
#define _FQ_REG_H_

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// Software

// Bank addr: 0x3004 ~ 0x3007 (FIQ 0~7)
// Bank addr: 0x3008 ~ 0x3009 (FIQ_Mux 0~2)
#define FQ_REG_CTRL_BASE           (0x200400UL * 2)

typedef struct _REG32_FQ
{
    volatile MS_U16                 L;
    volatile MS_U16                 empty_L;
    volatile MS_U16                 H;
    volatile MS_U16                 empty_H;
} REG32_FQ;

typedef struct _REG16_FQ
{
    volatile MS_U16                 data;
    volatile MS_U16                 _resv;
} REG16_FQ;

typedef struct _REG_FIQ
{
    REG16_FQ        Reg_fiq_config0;                                //0x00
        #define FIQ_CFG0_SW_RSTZ                                0x0001              //sw_rstz
        #define FIQ_CFG0_PVR_ENABLE                             0x0002              //stream2miu_en
        #define FIQ_CFG0_RESET_WR_PTR                           0x0004              //str2miu_rst_wadr
        #define FIQ_CFG0_PVR_PAUSE                              0x0020
        #define FIQ_CFG0_LOAD_WR_PTR                            0x0040              //strm2mi2_wp_ld
        #define FIQ_CFG0_MIU_HIGH_PRI                           0x0080
        #define FIQ_CFG0_FORCE_SYNC_EN                          0x0100
        #define FIQ_CFG0_REC_AT_SYNC_DIS                        0x0200
        #define FIQ_CFG0_CLR_PVR_OVERFLOW                       0x0400
        #define FIQ_CFG0_FIQ2MI_R_PRT_HIGHT                     0x0800
        #define FIQ_CFG0_BURST_LEN_MASK                         0x3000
        #define FIQ_CFG0_BURST_LEN_SHIFT                        12
            #define FIQ_CFG0_BURST_LEN_8BYTE                    0
            #define FIQ_CFG0_BURST_LEN_4BYTE                    1
            #define FIQ_CFG0_BURST_LEN_1BYTE                    3
        #define FIQ_CFG0_RUSH_ENABLE                            0x4000              //rush_en
        #define FIQ_CFG0_ADDR_MODE                              0x8000              //addr_mode

    REG32_FQ        str2mi_head;                                    //0x01
    REG32_FQ        str2mi_tail;                                    //0x03
    REG32_FQ        str2mi_mid;                                     //0x05
    REG32_FQ        rush_addr;                                      //0x07
    REG32_FQ        cur_pkt_start_wadr_offset;                      //0x09
        #define FIQ_STR2MI2_ADDR_MASK                           0x0FFFFFFFUL

    REG16_FQ        Reg_fiq_config1;                                //0x0b
//------------------------- no use , just for compatibility -------------------------//
        #define FIQ_CFG11_SKIP_RUSH_DATA_PATH_NON               0x0000
        #define FIQ_CFG11_SKIP_PCR_RUSH_DATA                    0x0000
        #define FIQ_CFG11_SKIP_PVR1_RUSH_DATA                   0x0000
        #define FIQ_CFG11_SKIP_PVR2_RUSH_DATA                   0x0000
        #define FIQ_CFG11_SKIP_PVR3_RUSH_DATA                   0x0000
        #define FIQ_CFG11_SKIP_PVR4_RUSH_DATA                   0x0000
//-----------------------------------------------------------------------------------//
        #define FIQ_CFGB_FIQ_BYPASS                             0x0001              //FIQ_bypass
        #define FIQ_CFGB_BURST_LEVEL_MASK                       0x0006
        #define FIQ_CFGB_BURST_LEVEL_SHIFT                      1
        #define FIG_CFGB_REG_FIQ_INPUT_SAME_T                   0x0008
        #define FIG_CFGB_REG_ECO_TS_SYNC_OUT_REVERSE_BLOCK      0x0010
        #define FIG_CFGB_REG_BYPASS_FILEIN_TO_FIQ               0x0020
        #define FIG_CFGB_READ_BURST_LEN_MASK                    0x00C0
        #define FIG_CFGB_READ_BURST_LEN_SHIFT                   6
            #define FIQ_CFGB_READ_BURST_LEN_12BYTE              0
            #define FIQ_CFGB_READ_BURST_LEN_8BYTE               1   // not support
            #define FIQ_CFGB_READ_BURST_LEN_4BYTE               2
            #define FIQ_CFGB_READ_BURST_LEN_1BYTE               3   // not support
        #define FIG_CFGB_REG_FIQ_MUX_SRC_MASK                   0x0F00              //FIQ_Mux only
        #define FIG_CFGB_REG_FIQ_MUX_SRC_SHIFT                  8                   //
        #define FIQ_CFGB_LPCR1_WLD                              0x2000
        #define FIQ_CFGB_LPCR1_LOAD                             0x4000
        #define FIQ_CFGB_FIQ_SEC_SEL                            0x8000

    REG32_FQ        pkt_addr_offset;                                //0x0c
    REG16_FQ        Reg_fig_config2;                                //0x0e
        #define FIQ_CFGE_C90K_SEL_90K                           0x0000
        #define FIQ_CFGE_C90K_SEL_27M                           0x0001
        #define FIQ_CFGE_TIMESTAMP_SRC_SEL_MASK                 0x0002
        #define FIQ_CFGE_TIMESTAMP_SRC_SEL_SHIFT                1
        #define FIQ_CFGE_TIMESTAMP_SRC_SEL_LPCR                 0
        #define FIQ_CFGE_TIMESTAMP_SRC_SEL_PKT_CONVERTER        1
        #define FIQ_CFGE_REG_FIQ_BYPASS_FIFO                    0x0004
        #define FIQ_CFGE_REG_FIQ_AFIFO_FULL_CONFIG              0x0008
        #define FIQ_CFGE_REG_RUSH_SKIP_PKT_BY_TIMESTAMP_START   0x0100

    REG16_FQ        Reg_fig_config3;                                //0x0f
        #define FIQ_CFGF_STREAM2MI_RD                           0x0001              // FIQ read enable
        #define FIQ_CFGF_RUSH_MODE_EN                           0x0002              // FIQ rush mode (enable output)
        #define FIQ_CFGF_SRC_FILTER_EN                          0x0004              // FIQ src filter enable

    REG16_FQ        Reg_fiq_int;                                    //0x10
        #define FIQ_CFG10_INT_ENABLE_MASK                       0x00FF
        #define FIQ_CFG10_INT_ENABLE_RUSH_DONE                  0x0001
        #define FIQ_CFG10_INT_ENABLE_PVR_MEET_BUF_TAIL          0x0002
        #define FIQ_CFG10_INT_ENABLE_PVR_MEET_BUF_MID           0x0004
        #define FIQ_CFG10_INT_STATUS_MASK                       0xFF00
        #define FIQ_CFG10_INT_STATUS_RUSH_DONE                  0x0100

    REG32_FQ        str2mi2_wadr_r;                                 //0x11
    REG32_FQ        Fiq2mi2_radr_r;                                 //0x13
    REG16_FQ        Fiq_status;                                     //0x15
    REG32_FQ        lpcr1;                                          //0x16

    REG32_FQ        Fiq_Rush_Timestamp_Start;                       //0x18

    REG16_FQ        REG_FIQ_1A;                                     //0x1A
        #define FIQ_CFG1A_REG_ONEWAY_FIQ                        0x0001
        #define FIQ_CFG1A_REG_REST_FIQ_SRC_FLT                  0x0002
        #define FIQ_CFG1A_REG_REST_TOP                          0x0004
        #define FIQ_CFG1A_REG_MIU_HIGHPRI_THOLD_MASK            0x0018
        #define FIQ_CFG1A_REG_MIU_HIGHPRI_THOLD_SHIFT           3

    REG16_FQ        REG1B_1D_RESERVED[0x1E - 0x1B];                 //0x1B~0x1D

    REG16_FQ        REG_FIQ_1E;
        #define FIQ_CFG1E_REG_CHECK_TIMEOUT_CNT_MASK            0x00FF
        #define FIQ_CFG1E_REG_CHECK_TIMEOUT_CNT_SHIFT           0

    REG16_FQ        REG_FIQ_1F;                                     //0x1F
        #define FIQ_CFG1F_REG_CLK_GATING_TSP_FIQ                0x0001
        #define FIQ_CFG1F_REG_CLK_GATING_MIU_FIQ                0x0002
        #define FIQ_CFG1F_REG_CHECK_TIMEOUT_ENABLE_W            0x0004
        #define FIQ_CFG1F_REG_CHECK_TIMEOUT_ENABLE_R            0x0008

    REG16_FQ        Fiq_Src_Filter[8];                              //0x20~0x27
        #define FIQ_SRC_FILTER_SYNC_BYTE_EVEN_MASK              0x00FF
        #define FIQ_SRC_FILTER_SYNC_BYTE_EVEN_SHIFT             0
        #define FIQ_SRC_FILTER_SYNC_BYTE_ODD_MASK               0xFF00
        #define FIQ_SRC_FILTER_SYNC_BYTE_ODD_SHIFT              8

    REG16_FQ        REG_FIQ_28;                                     //0x28
        #define FIQ_CFG28_REG_FIQ_SRC_FILTER_EN                 0x0001  // 0 ~ 15

    REG16_FQ        Fiq_Filter[4];                                  //0x29~0x2C
        #define FIQ_FILTER_PID_MASK                             0x1FFF
        #define FIQ_FILTER_PID_SHIFT                            0
        #define FIQ_FILTER_EN                                   0x8000

    REG16_FQ        Fiq_Filter_SyncByte[2];                         //0x2D~0x2E
        #define FIQ_FILTER_SYNC_BYTE_EVEN_MASK                  0x00FF
        #define FIQ_FILTER_SYNC_BYTE_EVEN_SHIFT                 0
        #define FIQ_FILTER_SYNC_BYTE_ODD_MASK                   0xFF00
        #define FIQ_FILTER_SYNC_BYTE_ODD_SHIFT                  8

    REG16_FQ        REG2F_3F_RESERVED[0x40 - 0x2F];                 //0x2F~0x3F

} REG_FIQ;

#endif // _FQ_REG_H_