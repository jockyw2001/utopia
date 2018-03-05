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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
#ifndef MHAL_XC_CONFIG_H
#define MHAL_XC_CONFIG_H

//-------------------------------------------------------------------------------------------------
//  Emerald
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Chip Configuration
//-------------------------------------------------------------------------------------------------
#define MAX_XC_DEVICE_NUM       (1)
#define MAX_XC_DEVICE0_OFFSET   (0UL)

#define MAX_WINDOW_NUM          (2)
#define MAX_FRAME_NUM_IN_MEM    (4) // Progressive
#define MAX_FIELD_NUM_IN_MEM    (16) // Interlace
#define NUM_OF_DIGITAL_DDCRAM   (1)

#define SCALER_LINE_BUFFER_MAX  (1920UL)
#define MST_LINE_BFF_MAX        MAX(1680, SCALER_LINE_BUFFER_MAX)

#define SUB_MAIN_LINEOFFSET_GUARD_BAND  0
#define SUB_SCALER_LINE_BUFFER_MAX      960UL - SUB_MAIN_LINEOFFSET_GUARD_BAND
#define SUB_MST_LINE_BFF_MAX            SUB_SCALER_LINE_BUFFER_MAX

#define MS_3D_LINE_BFF_MAX      (960UL)//(1440UL)

// MIU Word (Bytes)
#define BYTE_PER_WORD           (16)
#define OFFSET_PIXEL_ALIGNMENT  (32)
#define LPLL_LOOPGAIN           (16)
#define LVDS_MPLL_CLOCK_MHZ     (432)

#define MCDI_BYTE_PER_WORD          16

#define DEFAULT_STEP_P          4 //conservative step value
#define DEFAULT_STEP_I          ((DEFAULT_STEP_P*DEFAULT_STEP_P)/2)
#define STEP_P                  2 //recommended step value -> more faster fpll(T3)
#define STEP_I                  ((STEP_P*STEP_P)/2)
#define IPGAIN_REFACTOR         5

#define F2_WRITE_LIMIT_EN   BIT(31) //BK12_1b[15]
#define F2_WRITE_LIMIT_MIN  BIT(30) //BK12_1b[14]

#define F1_WRITE_LIMIT_EN   BIT(31) //BK12_5b[15]
#define F1_WRITE_LIMIT_MIN  BIT(30) //BK12_5b[14]

#define F2_FRCM_WRITE_LIMIT_EN   BIT(31) //BK32_1b[15]
#define F1_FRCM_WRITE_LIMIT_EN   BIT(31) //BK32_5b[15]

#define F2_V_WRITE_LIMIT_EN    BIT(15) //BK12_18[12]
#define F1_V_WRITE_LIMIT_EN    BIT(15) //BK12_58[12]

#define F2_OPW_WRITE_LIMIT_EN   BIT(31) //for UC
#define F2_OPW_WRITE_LIMIT_MIN  BIT(30) //for UC

#define F2_V_WRITE_LIMIT_EN    BIT(15)
#define F1_V_WRITE_LIMIT_EN    BIT(15)

#define ADC_MAX_CLK                     (3500)

#define SUPPORTED_XC_INT        ((1UL << SC_INT_VSINT) |            \
                                 (1UL << SC_INT_F2_VTT_CHG) |       \
                                 (1UL << SC_INT_F1_VTT_CHG) |       \
                                 (1UL << SC_INT_F2_VS_LOSE) |       \
                                 (1UL << SC_INT_F1_VS_LOSE) |       \
                                 (1UL << SC_INT_F2_JITTER) |        \
                                 (1UL << SC_INT_F1_JITTER) |        \
                                 (1UL << SC_INT_F2_IPVS_SB) |       \
                                 (1UL << SC_INT_F1_IPVS_SB) |       \
                                 (1UL << SC_INT_F2_IPHCS_DET) |     \
                                 (1UL << SC_INT_F1_IPHCS_DET) |     \
                                 (1UL << SC_INT_F2_HTT_CHG) |       \
                                 (1UL << SC_INT_F1_HTT_CHG) |       \
                                 (1UL << SC_INT_F2_HS_LOSE) |       \
                                 (1UL << SC_INT_F1_HS_LOSE) |       \
                                 (1UL << SC_INT_F2_CSOG) |          \
                                 (1UL << SC_INT_F1_CSOG) |          \
                                 (1UL << SC_INT_F2_ATP_READY) |     \
                                 (1UL << SC_INT_F1_ATP_READY) |		\
                                 (1UL << IRQ_INT_PWM_FP_L_INT))


//These table definition is from SC_BK0 spec.
//Because some chip development is different, it need to check and remap when INT function is used
#define IRQ_INT_DIPW          1
#define IRQ_INT_START         4
#define IRQ_INT_RESERVED1     IRQ_INT_START

#define IRQ_INT_VSINT         5
#define IRQ_INT_F2_VTT_CHG    6
#define IRQ_INT_F1_VTT_CHG    7
#define IRQ_INT_F2_VS_LOSE    8
#define IRQ_INT_F1_VS_LOSE    9
#define IRQ_INT_F2_JITTER     10
#define IRQ_INT_F1_JITTER     11
#define IRQ_INT_F2_IPVS_SB    12
#define IRQ_INT_F1_IPVS_SB    13
#define IRQ_INT_F2_IPHCS_DET  14
#define IRQ_INT_F1_IPHCS_DET  15

#define IRQ_INT_PWM_RP_L_INT  16
#define IRQ_INT_PWM_FP_L_INT  17
#define IRQ_INT_F2_HTT_CHG    18
#define IRQ_INT_F1_HTT_CHG    19
#define IRQ_INT_F2_HS_LOSE    20
#define IRQ_INT_F1_HS_LOSE    21
#define IRQ_INT_PWM_RP_R_INT  22
#define IRQ_INT_PWM_FP_R_INT  23
#define IRQ_INT_F2_CSOG       24
#define IRQ_INT_F1_CSOG       25
#define IRQ_INT_F2_RESERVED2  26
#define IRQ_INT_F1_RESERVED2  27
#define IRQ_INT_F2_ATP_READY  28
#define IRQ_INT_F1_ATP_READY  29
#define IRQ_INT_F2_RESERVED3  30
#define IRQ_INT_F1_RESERVED3  31

//-------------------------------------------------------------------------------------------------
//  Chip Feature
//-------------------------------------------------------------------------------------------------

/* 12 frame mode for progessive */
#define _12FRAME_BUFFER_PMODE_SUPPORTED     0
/* 8 frame mode for progessive */
#define _8FRAME_BUFFER_PMODE_SUPPORTED      1
/* 6 frame mode for progessive */
#define _6FRAME_BUFFER_PMODE_SUPPORTED      0
/* 4 frame mode for progessive */
#define _4FRAME_BUFFER_PMODE_SUPPORTED      1
/* 3 frame mode for progessive */
#define _3FRAME_BUFFER_PMODE_SUPPORTED      1

/*
   Field-packing ( Customized name )
   This is a feature in M10. M10 only needs one IPM buffer address. (Other chips need two or three
   IPM buffer address). We show one of memory format for example at below.

   Block :       Y0      C0      L       M        Y1       C1
   Each block contain 4 fields (F0 ~ F3) and each fields in one block is 64 bits
   Y0 has 64 * 4 bits ( 8 pixel for each field ).
   Y1 has 64 * 4 bits ( 8 pixel for each field ).
   So, in this memory format, pixel alignment is 16 pixels (OFFSET_PIXEL_ALIGNMENT = 16).
   For cropping, OPM address offset have to multiple 4.
*/
#define _FIELD_PACKING_MODE_SUPPORTED       1

#if (_FIELD_PACKING_MODE_SUPPORTED)

/* Linear mode */
#define _LINEAR_ADDRESS_MODE_SUPPORTED      0

#else
/* Linear mode */
#define _LINEAR_ADDRESS_MODE_SUPPORTED      1

#endif

#define SUPPORT_2_FRAME_MIRROR              0

/* Because fix loop_div, lpll initial set is different between singal port and dual port */
#define _FIX_LOOP_DIV_SUPPORTED             0

// You can only enable ENABLE_8_FIELD_SUPPORTED or ENABLE_16_FIELD_SUPPORTED. (one of them)
// 16 field mode include 8 field configurion in it. ENABLE_8_FIELD_SUPPORTED is specital case in T7
#define ENABLE_8_FIELD_SUPPORTED            1
#define ENABLE_16_FIELD_SUPPORTED           1
#define ENABLE_OPM_WRITE_SUPPORTED          0
#define ENABLE_YPBPR_PRESCALING_TO_ORIGINAL 0
#define ENABLE_VD_PRESCALING_TO_DOT75       0
#define ENABLE_NONSTD_INPUT_MCNR            0
#define ENABLE_REGISTER_SPREAD              1

#define ENABLE_REQUEST_FBL                  1
#define DELAY_LINE_SC_UP                    7
#define DELAY_LINE_SC_DOWN                  8

#define CHANGE_VTT_STEPS                    15
#define CHANGE_VTT_DELAY                    5

#define SUPPORT_IMMESWITCH                  1
#define SUPPORT_DVI_AUTO_EQ                 1
#define SUPPORT_HDMI_RX_NEW_FEATURE         1
#define SUPPORT_DEVICE1                     0
#define SUPPORT_SEAMLESS_ZAPPING            0

 #define SUPPORT_OP2_TEST_PATTERN           1
#define SUPPORT_FRCM_MODE                   0
#define SUPPORT_4K2K_PIP                    0

// if H/W support 4k2k_60p output timing
#define HW_SUPPORT_4K2K_60P_OUTPUT          FALSE

// Special frame lock means that the frame rates of input and output are the same in HW design spec.
#define SUPPORT_SPECIAL_FRAMELOCK           FALSE

#define LD_ENABLE                           1

#define FRC_INSIDE                          FALSE

/// need to refine and test
#define Eagle_Bringup                       1

// 480p and 576p have FPLL problem in HV mode.
// So only allow HV mode for 720P
#define ONLY_ALLOW_HV_MODE_FOR_720P         0

#define DS_BUFFER_NUM_EX                     6
#define DS_MAX_INDEX                         6
#define DS_INDEX_DEPTH                       16

#define ENABLE_64BITS_COMMAND               0
#define ENABLE_64BITS_SPREAD_MODE           0
#define ENABLE_DS_4_BASEADDR_MODE           0 // need enable both ENABLE_64BITS_COMMAND and ENABLE_64BITS_SPREAD_MODE first
#define DS_CMD_LEN_64BITS                   8

// T12, T13 cannot use IP_HDMI for HV mode
// We must use IP_HDMI for HV mode, otherwise 480i 576i will have color space proble,m
//Note: if use IP_HDMI, MApi_XC_GetDEWindow() cannot get value correctly
// and IP_HDMI is set in MApi_XC_SetInputSource(), so cannot change dynamically
// Thus, chip could use this flag to determine whether could do HV mode or not.
#define SUPPORT_IP_HDMI_FOR_HV_MODE           0

// version1: edison: 4k2k@mm :mvop->dip->gop->ursa; 4k2k@hdmi:hdmi->ursa
// version2: nike:
// version3: napoli: frc: double frc and width
// version4: monaco: frcm and 2p
// version5: clippers: 4k2k@60 MVOP directly output to HVSP
#define HW_DESIGN_4K2K_VER                  (1)

#define HW_DESIGN_3D_VER                    (3)
#define HW_2DTO3D_SUPPORT                    TRUE
#define HW_2DTO3D_VER                       (3)
#define HW_2DTO3D_BYTE_PER_WORD             (16)
#define HW_2DTO3D_PATCH                     TRUE //a1 u01:2d to 3d hw bug
#define HW_2DTO3D_BLOCK_DR_BUF_SIZE         (0x1000)

//hw support fbl 3d or not. if support,can do SBS to LBL and SBS to SBS
#define HW_3D_SUPPORT_FBL                   (PM_R1BYTE(REG_CHIP_REVISION, 7:0) > 1)

//M10, A2, J2 ,A5,A6,A3,Agate HW will automatic use IPM fetch's reg setting to alignment IPM fetch, so skip sw alignment
//and for mirror cbcr swap, need check IPM fetch to decide if need swap
#define HW_IPM_FETCH_ALIGNMENT              TRUE
//hw support 2 line mode deinterlace for interlace or not
#define HW_2LINEMODE_DEINTERLACE_SUPPORT    FALSE

#define OSD_LAYER_NUM           (3)
#define VIDEO_OSD_SWITCH_VER    (1)


#define SUPPORT_IMMESWITCH                  1
#define SUPPORT_DVI_AUTO_EQ                 1
#define SUPPORT_MHL                         0
#define SUPPORT_SECURITY_MODE               0
#define SUPPORT_HDMI_RX_NEW_FEATURE         1

#define XC_SUPPORT_4K2K       0

// Special frame lock means that the frame rates of input and output are the same in HW design spec.
#define SUPPORT_SPECIAL_FRAMELOCK           FALSE

//device 1 is interlace out
#define XC_DEVICE1_IS_INTERLACE_OUT 0

#define FRC_INSIDE                          FALSE

#define HW_SUPPORT_FORCE_VSP_IN_DS_MODE     FALSE

#define SUPPORT_OSD_HSLVDS_PATH               0

#define ENABLE_LOCK_IVS_OVS_FOR_ATV         1
#define ENABLE_SRC_SPREADER_FOR_ATV         0

// if H/W support interlace output timing
#define HW_SUPPORT_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE    TRUE
#define HW_SUPPORT_INTERLACE_OUTPUT FALSE
#define SUPPORT_HDMI20  0

#define HW_4K2K_VIP_PEAKING_LIMITATION                      0

//-------------------------------------------------------------------------------------------------
//  Register base
//-------------------------------------------------------------------------------------------------
#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)


// PM
#define REG_DDC_BASE                0x000400
#define REG_PM_SLP_BASE             0x000E00
#define REG_PM_SLEEP_BASE           REG_PM_SLP_BASE//0x0E00//alex_tung

// NONPM
#define REG_MIU0_BASE               0x101200
#define REG_MIU1_BASE               0x100600
#define REG_CLKGEN2_BASE            0x100A00
#define REG_CHIPTOP_BASE            0x100B00
#define REG_CLKGEN0_BASE            0x100B00  // 0x1E00 - 0x1EFF
#define REG_UHC0_BASE               0x102400
#define REG_UHC1_BASE               0x100D00
#define REG_ADC_ATOP_BASE           0x102500  // 0x2500 - 0x25FF
#define REG_ADC_DTOP_BASE           0x102600  // 0x2600 - 0x26EF
#define REG_ADC_CHIPTOP_BASE        0x101E00  // 0x1E00 - 0x1EFF
#define REG_HDMI_BASE               0x102700  // 0x2700 - 0x27FF
#define REG_ADC_ATOPB_BASE          0x103D00  // 0x3D00 - 0x3DFF
#define REG_MHL_TMDS_BASE           0x122700

#define REG_HDMI2_BASE              0x101A00
#define REG_IPMUX_BASE              0x102E00
#define REG_MVOP_BASE               0x101400
#if ENABLE_REGISTER_SPREAD
#define REG_SCALER_BASE             0x130000
#else
#define REG_SCALER_BASE             0x102F00
#endif
#define REG_LPLL_BASE               0x103100
#define REG_MOD_BASE                0x103200
#define REG_AFEC_BASE               0x103500
#define REG_COMB_BASE               0x103600

#define REG_HDCPKEY_BASE            0x123A00
#define REG_DVI_ATOP_BASE           0x110900
#define REG_DVI_DTOP_BASE           0x110A00
#define REG_DVI_EQ_BASE             0x110A80     // EQ started from 0x80
#define REG_HDCP_BASE               0x110AC0     // HDCP started from 0xC0
#define REG_ADC_DTOPB_BASE          0x111200     // ADC DTOPB
#define REG_DVI_ATOP1_BASE          0x113200
#define REG_DVI_DTOP1_BASE          0x113300
#define REG_DVI_EQ1_BASE            0x113380     // EQ started from 0x80
#define REG_HDCP1_BASE              0x1133C0     // HDCP started from 0xC0
#define REG_DVI_ATOP2_BASE          0x113400
#define REG_DVI_ATOP3_BASE          0x162F00
#define REG_DVI_DTOP2_BASE          0x113500
#define REG_DVI_EQ2_BASE            0x113580     // EQ started from 0x80
#define REG_HDCP2_BASE              0x1135C0     // HDCP started from 0xC0
#define REG_DVI_PS_BASE             0x113600     // DVI power saving
#define REG_DVI_PS1_BASE            0x113640     // DVI power saving1
#define REG_DVI_PS2_BASE            0x113680     // DVI power saving2
#define REG_DVI_PS3_BASE            0x1136C0     // DVI power saving3
#define REG_DVI_DTOP3_BASE          0x113700
#define REG_DVI_EQ3_BASE            0x113780     // EQ started from 0x80
#define REG_HDCP3_BASE              0x1137C0     // HDCP started from 0xC0

#define REG_HDCP22_P0_BASE          0x162A00
#define REG_HDCP22_P1_BASE          0x162B00
#define REG_HDCP22_P2_BASE          0x162C00
#define REG_HDCP22_P3_BASE          0x162D00

#define REG_CHIP_ID_MAJOR           0x1ECC
#define REG_CHIP_ID_MINOR           0x1ECD
#define REG_CHIP_VERSION            0x1ECE
#define REG_CHIP_REVISION           0x1ECF

#define REG_CLKGEN0_BASE            0x100B00
#define REG_CLKGEN1_BASE            0x103300

#define L_CLKGEN0(x)                BK_REG_L(REG_CLKGEN0_BASE, x)
#define H_CLKGEN0(x)                BK_REG_H(REG_CLKGEN0_BASE, x)
#define L_CLKGEN1(x)                BK_REG_L(REG_CLKGEN1_BASE, x)
#define H_CLKGEN1(x)                BK_REG_H(REG_CLKGEN1_BASE, x)



// store bank
#define LPLL_BK_STORE

// restore bank
#define LPLL_BK_RESTORE

// switch bank
#define LPLL_BK_SWITCH(_x_) MDrv_WriteByte(REG_LPLL_BASE, _x_)

//------------------------------------------------------------------------------
// Register configure
//------------------------------------------------------------------------------
#define REG_CKG_DACA2           (REG_CLKGEN0_BASE + 0x4C ) //DAC out
    #define CKG_DACA2_GATED         BIT(0)
    #define CKG_DACA2_INVERT        BIT(1)
    #define CKG_DACA2_MASK          BMASK(3:2)
    #define CKG_DACA2_VIF_CLK       (0 << 2)
    #define CKG_DACA2_VD_CLK        (1 << 2)
    #define CKG_DACA2_EXT_TEST_CLK  (2 << 2)
    #define CKG_DACA2_XTAL          (3 << 2)

#define REG_CKG_DACB2           (REG_CLKGEN0_BASE + 0x4D ) //DAC out
    #define CKG_DACB2_GATED         BIT(0)
    #define CKG_DACB2_INVERT        BIT(1)
    #define CKG_DACB2_MASK          BMASK(3:2)
    #define CKG_DACB2_VIF_CLK       (0 << 2)
    #define CKG_DACB2_VD_CLK        (1 << 2)
    #define CKG_DACB2_EXT_TEST_CLK  (2 << 2)
    #define CKG_DACB2_XTAL          (3 << 2)

#define REG_CKG_FICLK_F1        (REG_CLKGEN0_BASE + 0xA2 ) // scaling line buffer, set to fclk if post scaling, set to idclk is pre-scaling
    #define CKG_FICLK_F1_GATED      BIT(0)
    #define CKG_FICLK_F1_INVERT     BIT(1)
    #define CKG_FICLK_F1_MASK       BMASK(3:2)
    #define CKG_FICLK_F1_IDCLK1     (0 << 2)
    #define CKG_FICLK_F1_FLK        (1 << 2)
    //#define CKG_FICLK_F1_XTAL       (3 << 2)

#define REG_CKG_FICLK_F2        (REG_CLKGEN0_BASE + 0xA3 ) // scaling line buffer, set to fclk if post scaling, set to idclk is pre-scaling
    #define CKG_FICLK_F2_GATED      BIT(0)
    #define CKG_FICLK_F2_INVERT     BIT(1)
    #define CKG_FICLK_F2_MASK      BMASK(3:2)
    #define CKG_FICLK_F2_IDCLK2     (0 << 2)
    #define CKG_FICLK_F2_FLK        (1 << 2)
    //#define CKG_FICLK_F2_XTAL       (3 << 2)

#define REG_CKG_FICLK2_F2        (REG_CLKGEN0_BASE + 0xA3 ) // scaling line buffer, set to fclk if post scaling, set to idclk is pre-scaling
    #define CKG_FICLK2_F2_GATED      BIT(4)
    #define CKG_FICLK2_F2_INVERT     BIT(5)
    #define CKG_FICLK2_F2_MASK       BMASK(7:6)
    #define CKG_FICLK2_F2_IDCLK2     (0 << 6)
    #define CKG_FICLK2_F2_FCLK       (1 << 6)

#define REG_CKG_FCLK            (REG_CLKGEN0_BASE + 0xA5 ) // after memory, before fodclk
    #define CKG_FCLK_GATED          BIT(0)
    #define CKG_FCLK_INVERT         BIT(1)
    #define CKG_FCLK_MASK           BMASK(5:2)
    #define CKG_FCLK_170MHZ         (0 << 2)
    #define CKG_FCLK_CLK_MIU        (1 << 2)
    #define CKG_FCLK_CLK_ODCLK      (2 << 2)
    #define CKG_FCLK_216MHZ         (3 << 2)
    #define CKG_FCLK_192MHZ         (4 << 2)
    #define CKG_FCLK_SCPLL          (5 << 2)
    #define CKG_FCLK_0              (6 << 2)
    #define CKG_FCLK_XTAL           (7 << 2)
    #define CKG_FCLK_XTAL_          CKG_FCLK_XTAL//(8 << 2) for A5 no XTAL
    #define CKG_FCLK_DEFAULT        CKG_FCLK_216MHZ

#define REG_CKG_ODCLK           (REG_CLKGEN0_BASE + 0xA6 ) // output dot clock, usually select LPLL, select XTAL when debug
    #define CKG_ODCLK_GATED         BIT(0)
    #define CKG_ODCLK_INVERT        BIT(1)
    #define CKG_ODCLK_MASK          BMASK(5:2)
    #define CKG_ODCLK_SC_PLL        (0 << 2)
    #define CKG_ODCLK_LPLL_DIV2     (5 << 2)
    #define CKG_ODCLK_27M           (6 << 2)
    #define CKG_ODCLK_CLK_LPLL      (7 << 2)
    #define CKG_ODCLK_XTAL          (8 << 2)

#define REG_CKG_IDCLK0          (REG_CLKGEN0_BASE + 0xA8 ) // off-line detect idclk
    #define CKG_IDCLK0_GATED        BIT(0)
    #define CKG_IDCLK0_INVERT       BIT(1)
    #define CKG_IDCLK0_MASK         BMASK(5:2)
    #define CKG_IDCLK0_CLK_ADC      (0 << 2)
    #define CKG_IDCLK0_CLK_DVI      (1 << 2)
    #define CKG_IDCLK0_CLK_VD       (2 << 2)
    #define CKG_IDCLK0_CLK_DC0      (3 << 2)
    #define CKG_IDCLK0_ODCLK        (4 << 2)
    #define CKG_IDCLK0_0            (5 << 2)
    #define CKG_IDCLK0_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK0_00           (7 << 2)               // same as 5 --> also is 0
    #define CKG_IDCLK0_XTAL         (8 << 2)

#define REG_CKG_IDCLK1          (REG_CLKGEN0_BASE + 0xA9 ) // sub main window idclk
    #define CKG_IDCLK1_GATED        BIT(0)
    #define CKG_IDCLK1_INVERT       BIT(1)
    #define CKG_IDCLK1_MASK         BMASK(5:2)
    #define CKG_IDCLK1_CLK_ADC      (0 << 2)
    #define CKG_IDCLK1_CLK_DVI      (1 << 2)
    #define CKG_IDCLK1_CLK_VD       (2 << 2)
    #define CKG_IDCLK1_CLK_DC0      (3 << 2)
    #define CKG_IDCLK1_ODCLK        (4 << 2)
    #define CKG_IDCLK1_0            (5 << 2)
    #define CKG_IDCLK1_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK1_00           (7 << 2)               // same as 5 --> also is 0
    #define CKG_IDCLK1_XTAL         (8 << 2)

#define REG_CKG_PRE_IDCLK1       (REG_CLKGEN0_BASE + 0xBC ) // pre-main window idclk
    #define CKG_PRE_IDCLK1_MASK         BMASK(5:3)
    #define CKG_PRE_IDCLK1_CLK_ADC      (0 << 3)
    #define CKG_PRE_IDCLK1_CLK_DVI      (1 << 3)
    #define CKG_PRE_IDCLK1_CLK_MHL      (2 << 3)

#define REG_CKG_IDCLK2          (REG_CLKGEN0_BASE + 0xAA ) // main window idclk
    #define CKG_IDCLK2_GATED        BIT(0)
    #define CKG_IDCLK2_INVERT       BIT(1)
    #define CKG_IDCLK2_SHIFT        2
    #define CKG_IDCLK2_MASK         BMASK(5:2)
    #define CKG_IDCLK2_CLK_ADC      (0 << 2)
    #define CKG_IDCLK2_CLK_DVI      (1 << 2)
    #define CKG_IDCLK2_CLK_VD       (2 << 2)
    #define CKG_IDCLK2_CLK_DC0      (3 << 2)
    #define CKG_IDCLK2_ODCLK        (4 << 2)
    #define CKG_IDCLK2_0            (5 << 2)
    #define CKG_IDCLK2_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK2_00           (7 << 2)               // same as 5 --> also is 0
    #define CKG_IDCLK2_XTAL         (8 << 2)

#define REG_CKG_PRE_IDCLK2       (REG_CLKGEN0_BASE + 0xBC ) // pre-main window idclk
    #define CKG_PRE_IDCLK2_MASK         BMASK(8:6)
    #define CKG_PRE_IDCLK2_CLK_ADC      (0 << 6)
    #define CKG_PRE_IDCLK2_CLK_DVI      (1 << 6)
    #define CKG_PRE_IDCLK2_CLK_MHL      (2 << 6)

#define REG_CKG_IDCLK3          (REG_CLKGEN0_BASE + (0x59<<1) )
    #define CKG_IDCLK3_GATED        BIT(4)
    #define CKG_IDCLK3_INVERT       BIT(5)
    #define CKG_IDCLK3_MASK         BMASK(8:6)
    #define CKG_IDCLK3_SHIFT        6
    #define CKG_IDCLK3_CLK_ADC      (0 << 6)
    #define CKG_IDCLK3_CLK_DVI      (1 << 6)
    #define CKG_IDCLK3_CLK_VD       (2 << 6)
    #define CKG_IDCLK3_CLK_DC0      (3 << 6)
    #define CKG_IDCLK3_ODCLK        (4 << 6)
    #define CKG_IDCLK3_0            (5 << 6)
    #define CKG_IDCLK3_CLK_VD_ADC   (6 << 6)
    #define CKG_IDCLK3_00           (7 << 6)               // same as 5 --> also is 0
    #define CKG_IDCLK3_XTAL         (8 << 6)

#define REG_CKG_PDW0            (REG_CLKGEN0_BASE + (0x5F<<1)  )
    #define CKG_PDW0_GATED          BIT(0)
    #define CKG_PDW0_INVERT         BIT(1)
    #define CKG_PDW0_SHIFT          2
    #define CKG_PDW0_MASK           BMASK(5:2)
    #define CKG_PDW0_CLK_ADC      (0 << 2)
    #define CKG_PDW0_CLK_DVI      (1 << 2)
    #define CKG_PDW0_CLK_VD       (2 << 2)
    #define CKG_PDW0_CLK_DC0      (3 << 2)
    #define CKG_PDW0_ODCLK        (4 << 2)
    #define CKG_PDW0_0            (5 << 2)
    #define CKG_PDW0_CLK_VD_ADC   (6 << 2)
    #define CKG_PDW0_00           (7 << 2)               // same as 5 --> also is 0
    #define CKG_PDW0_XTAL         (8 << 2)

#define REG_CKG_OSDC            (REG_CLKGEN0_BASE + 0xAB )
    #define CKG_OSDC_GATED          BIT(0)
    #define CKG_OSDC_INVERT         BIT(1)
    #define CKG_OSDC_MASK           BMASK(3:2)
    #define CKG_OSDC_CLK_LPLL_OSD   (0 << 2)

#define REG_DE_ONLY_F3          (REG_CLKGEN0_BASE + 0xA0 )
    #define DE_ONLY_F3_MASK         BIT(3)

#define REG_DE_ONLY_F2          (REG_CLKGEN0_BASE + 0xA0 )
    #define DE_ONLY_F2_MASK         BIT(2)

#define REG_DE_ONLY_F1          (REG_CLKGEN0_BASE + 0xA0 )
    #define DE_ONLY_F1_MASK         BIT(1)

#define REG_DE_ONLY_F0          (REG_CLKGEN0_BASE + 0xA0 )
    #define DE_ONLY_F0_MASK         BIT(0)


#define REG_PM_DVI_SRC_CLK      (REG_PM_SLP_BASE +  0x96)
#define REG_PM_DDC_CLK          (REG_PM_SLP_BASE +  0x42)

#define REG_CLKGEN0_50_L        (REG_CLKGEN0_BASE + 0xA0)
#define REG_CLKGEN0_51_L        (REG_CLKGEN0_BASE + 0xA2)
#define REG_CLKGEN0_57_L        (REG_CLKGEN0_BASE + 0xAE)

#define REG_MVOP_MIRROR         (REG_MVOP_BASE    + 0x76)

#define CLK_SRC_IDCLK2  0
#define CLK_SRC_FCLK    1
#define CLK_SRC_XTAL    3

#define MIU0_G0_REQUEST_MASK    (REG_MIU0_BASE + 0x46)
#define MIU0_G1_REQUEST_MASK    (REG_MIU0_BASE + 0x66)
#define MIU0_G2_REQUEST_MASK    (REG_MIU0_BASE + 0x86)
#define MIU0_G3_REQUEST_MASK    (REG_MIU0_BASE + 0xA6)

#define MIU1_G0_REQUEST_MASK    (REG_MIU1_BASE + 0x46)
#define MIU1_G1_REQUEST_MASK    (REG_MIU1_BASE + 0x66)
#define MIU1_G2_REQUEST_MASK    (REG_MIU1_BASE + 0x86)
#define MIU1_G3_REQUEST_MASK    (REG_MIU1_BASE + 0xA6)

#define MIU_SC_G0REQUEST_MASK   (0x0000)
#define MIU_SC_G1REQUEST_MASK   (0x0E00)
#define MIU_SC_G2REQUEST_MASK   (0x0030)
#define MIU_SC_G3REQUEST_MASK   (0x0000)

#define IP_DE_HSTART_MASK       (0x1FFF) //BK_01_13 BK_03_13
#define IP_DE_HEND_MASK         (0x1FFF) //BK_01_15 BK_03_15
#define IP_DE_VSTART_MASK       (0x1FFF) //BK_01_12 BK_03_12
#define IP_DE_VEND_MASK         (0x1FFF) //BK_01_14 BK_03_14

#define VOP_DE_HSTART_MASK      (0x1FFF) //BK_10_04
#define VOP_DE_HEND_MASK        (0x1FFF) //BK_10_05
#define VOP_DE_VSTART_MASK      (0x0FFF) //BK_10_06
#define VOP_DE_VEND_MASK        (0x0FFF) //BK_10_07

#define VOP_VTT_MASK            (0x0FFF) //BK_10_0D
#define VOP_HTT_MASK            (0x1FFF) //BK_10_0C

#define VOP_VSYNC_END_MASK      (0x0FFF) //BK_10_03
#define VOP_DISPLAY_HSTART_MASK (0x1FFF) //BK_10_08
#define VOP_DISPLAY_HEND_MASK   (0x1FFF) //BK_10_09
#define VOP_DISPLAY_VSTART_MASK (0x0FFF) //BK_10_0A
#define VOP_DISPLAY_VEND_MASK   (0x0FFF) //BK_10_0B



#define HW_DESIGN_LD_VER                    (1)

#define FPLL_THRESH_MODE_SUPPORT    0

#define ADC_CENTER_GAIN             0x1000
#define ADC_CENTER_OFFSET           0x0800
#define ADC_GAIN_BIT_CNT            14
#define ADC_OFFSET_BIT_CNT          13

#define ADC_VGA_DEFAULT_GAIN_R      0x1000
#define ADC_VGA_DEFAULT_GAIN_G      0x1000
#define ADC_VGA_DEFAULT_GAIN_B      0x1000
#define ADC_VGA_DEFAULT_OFFSET_R    0x0000
#define ADC_VGA_DEFAULT_OFFSET_G    0x0000
#define ADC_VGA_DEFAULT_OFFSET_B    0x0000
#define ADC_YPBPR_DEFAULT_GAIN_R    0x1212
#define ADC_YPBPR_DEFAULT_GAIN_G    0x11AA
#define ADC_YPBPR_DEFAULT_GAIN_B    0x1212
#define ADC_YPBPR_DEFAULT_OFFSET_R  0x0800
#define ADC_YPBPR_DEFAULT_OFFSET_G  0x0100
#define ADC_YPBPR_DEFAULT_OFFSET_B  0x0800
#define ADC_SCART_DEFAULT_GAIN_R    0x1000
#define ADC_SCART_DEFAULT_GAIN_G    0x1000
#define ADC_SCART_DEFAULT_GAIN_B    0x1000
#define ADC_SCART_DEFAULT_OFFSET_R  0x0100
#define ADC_SCART_DEFAULT_OFFSET_G  0x0100
#define ADC_SCART_DEFAULT_OFFSET_B  0x0100

///////////////////////////////////////////////
// Enable Hardware auto gain/offset
#define ADC_HARDWARE_AUTOOFFSET_RGB   		ENABLE
#define ADC_HARDWARE_AUTOOFFSET_YPBPR 		ENABLE
#define ADC_HARDWARE_AUTOOFFSET_SCARTRGB 	ENABLE
#define ADC_HARDWARE_AUTOGAIN_SUPPORTED     ENABLE
#define ADC_VGA_FIXED_GAIN_R        0x1796
#define ADC_VGA_FIXED_GAIN_G        0x1796
#define ADC_VGA_FIXED_GAIN_B        0x1796
#define ADC_VGA_FIXED_OFFSET_R      0x0000
#define ADC_VGA_FIXED_OFFSET_G      0x0000
#define ADC_VGA_FIXED_OFFSET_B      0x0000
#define ADC_YPBPR_FIXED_GAIN_R      0x14B7
#define ADC_YPBPR_FIXED_GAIN_G      0x1441
#define ADC_YPBPR_FIXED_GAIN_B      0x14B7
#define ADC_YPBPR_FIXED_OFFSET_R    0x0800
#define ADC_YPBPR_FIXED_OFFSET_G    0x0100
#define ADC_YPBPR_FIXED_OFFSET_B    0x0800
#define ADC_SCART_FIXED_GAIN_R      0x1796
#define ADC_SCART_FIXED_GAIN_G      0x1796
#define ADC_SCART_FIXED_GAIN_B      0x1796
#define ADC_SCART_FIXED_OFFSET_R    0x0000
#define ADC_SCART_FIXED_OFFSET_G    0x0000
#define ADC_SCART_FIXED_OFFSET_B    0x0000

#endif /* MHAL_XC_CONFIG_H */

