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
//  Monaco
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Chip Configuration
//-------------------------------------------------------------------------------------------------
#define MAX_XC_DEVICE_NUM       (2)
#define MAX_XC_DEVICE0_OFFSET   (0)
#define MAX_XC_DEVICE1_OFFSET   (128)

#define MAX_WINDOW_NUM          (2)
#define MAX_FRAME_NUM_IN_MEM    (4) // Progressive
#define MAX_FIELD_NUM_IN_MEM    (16) // Interlace
#define NUM_OF_DIGITAL_DDCRAM   (1)

#define SCALER_LINE_BUFFER_MAX  (4096UL)
#define MST_LINE_BFF_MAX        MAX(4096, SCALER_LINE_BUFFER_MAX)

#define SUB_MAIN_LINEOFFSET_GUARD_BAND  0
#define SUB_SCALER_LINE_BUFFER_MAX      2048UL - SUB_MAIN_LINEOFFSET_GUARD_BAND
#define SUB_MST_LINE_BFF_MAX            SUB_SCALER_LINE_BUFFER_MAX

#define MS_3D_LINE_BFF_MAX      (2048UL)
#define PHASE_OFFSET_LIMIT      (0x900UL)
#define PHASE_OFFSET_LIMIT_FREQ_ONLY  (0x1200UL) //0x03DFUL // 0x8000UL
#define LPLL_LIMIT_OFFSET       (0x1600UL)

// MIU Word (Bytes)
#define BYTE_PER_WORD           (32)  // MIU 128: 16Byte/W, MIU 256: 32Byte/W
#define OFFSET_PIXEL_ALIGNMENT  (64)
#define LPLL_LOOPGAIN           (32) //due to bound cale.
#define LVDS_MPLL_CLOCK_MHZ     (432)
#define FRC_OFFSET_PIXEL_ALIGNMENT  (128)

#define FRC_BYTE_PER_WORD           32
#define MCDI_BYTE_PER_WORD          32

//value for pipe vcnt and hcnt delay
#define FRC_PIPE_DELAY_VCNT_FRC     0x10  // need to wait for FRC set to adjust this
#define FRC_PIPE_DELAY_HCNT_FRC     0x140 // need to wait for FRC set to adjust this
#define FRC_PIPE_DELAY_VCNT_FRC_FHD_OUT     0x06  // need to wait for FHD set to adjust this
#define FRC_PIPE_DELAY_HCNT_FRC_FHD_OUT     0x140 // need to wait for FHD set to adjust this
#define FRC_PIPE_DELAY_VCNT_FSC_FHD     0x0E
#define FRC_PIPE_DELAY_HCNT_FSC_FHD     0x140
#define FRC_PIPE_DELAY_VCNT_FSC_4K      0x06
#define FRC_PIPE_DELAY_HCNT_FSC_4K      0x140
#define FRC_PIPE_DELAY_VCNT_FSC_4K_120Hz    0x0A  // Maxim no 120Hz case, don't case this
#define FRC_PIPE_DELAY_HCNT_FSC_4K_120Hz    0x140 // Maxim no 120Hz case, don't case this

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

#define ADC_MAX_CLK                     (3500)

#define SUPPORTED_XC_INT        ((1UL << SC_INT_DIPW) |             \
                                 (1UL << SC_INT_VSINT) |            \
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
                                 (1UL << SC_INT_F1_ATP_READY))

#define SUPPORTED_KERNEL_INT  ((1UL << SC_INT_VSINT))

//These table definition is from SC_BK0 spec.
//Because some chip development is different, it need to check and remap when INT function is used
#define IRQ_CLEAN_INKERNEL    1

#define IRQ_INT_DIPW          0
#define IRQ_INT_START         3
#define IRQ_INT_RESERVED1     IRQ_INT_START

#define IRQ_INT_VSINT         4
#define IRQ_INT_F2_VTT_CHG    5
#define IRQ_INT_F1_VTT_CHG    6
#define IRQ_INT_F2_VS_LOSE    7
#define IRQ_INT_F1_VS_LOSE    8
#define IRQ_INT_F2_JITTER     9
#define IRQ_INT_F1_JITTER     10
#define IRQ_INT_F2_IPVS_SB    11
#define IRQ_INT_F1_IPVS_SB    12
#define IRQ_INT_F2_IPHCS_DET  13
#define IRQ_INT_F1_IPHCS_DET  14

#define IRQ_INT_PWM_RP_L_INT  15
#define IRQ_INT_PWM_FP_L_INT  16
#define IRQ_INT_F2_HTT_CHG    17
#define IRQ_INT_F1_HTT_CHG    18
#define IRQ_INT_F2_HS_LOSE    19
#define IRQ_INT_F1_HS_LOSE    20
#define IRQ_INT_PWM_RP_R_INT  21
#define IRQ_INT_PWM_FP_R_INT  22
#define IRQ_INT_F2_CSOG       23
#define IRQ_INT_F1_CSOG       24
#define IRQ_INT_F2_RESERVED2  25
#define IRQ_INT_F1_RESERVED2  26
#define IRQ_INT_F2_ATP_READY  27
#define IRQ_INT_F1_ATP_READY  28
#define IRQ_INT_F2_RESERVED3  29
#define IRQ_INT_F1_RESERVED3  30

//-------------------------------------------------------------------------------------------------
//  Chip Feature
//-------------------------------------------------------------------------------------------------
/* 12 frame mode for progessive */
#define _12FRAME_BUFFER_PMODE_SUPPORTED     1
/* 8 frame mode for progessive */
#define _8FRAME_BUFFER_PMODE_SUPPORTED      1
/* 6 frame mode for progessive */
#define _6FRAME_BUFFER_PMODE_SUPPORTED      1
/* 4 frame mode for progessive */
#define _4FRAME_BUFFER_PMODE_SUPPORTED      1
/* 3 frame mode for progessive */
#define _3FRAME_BUFFER_PMODE_SUPPORTED      1

// Maxim fix this HW bug, so remove SW patch
/* change Vtt BK68 replace BK10 */
//#define PATCH_HW_VTT_LIMITATION             1
/* Vtt BK10 not be replaced, CHIP number after U3 */
//#define HW_VTT_LIMITATION_CHIPREV           2

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
#define _FIX_LOOP_DIV_SUPPORTED             1

// You can only enable ENABLE_8_FIELD_SUPPORTED or ENABLE_16_FIELD_SUPPORTED. (one of them)
// 16 field mode include 8 field configurion in it. ENABLE_8_FIELD_SUPPORTED is specital case in T7
#define ENABLE_8_FIELD_SUPPORTED            0
#define ENABLE_16_FIELD_SUPPORTED           1
#define ENABLE_OPM_WRITE_SUPPORTED          1
#define ENABLE_YPBPR_PRESCALING_TO_ORIGINAL 0
#define ENABLE_VD_PRESCALING_TO_DOT75       0
#define ENABLE_NONSTD_INPUT_MCNR            0
#define ENABLE_REGISTER_SPREAD              1

#define ENABLE_REQUEST_FBL                  1
#define DELAY_LINE_SC_UP                    7
#define DELAY_LINE_SC_DOWN                  8

#define CHANGE_VTT_STEPS                    1
#define CHANGE_VTT_DELAY                    1

#define SUPPORT_IMMESWITCH                  1
#define SUPPORT_DVI_AUTO_EQ                 1
#define SUPPORT_MHL                         0
#define SUPPORT_HDMI_RX_NEW_FEATURE         1
#define SUPPORT_DEVICE1                     0
#define SUPPORT_SEAMLESS_ZAPPING            0
#define SUPPORT_OP2_TEST_PATTERN            1
#define SUPPORT_FRCM_MODE                   0
#define SUPPORT_4K2K_PIP                    1
#define SUPPORT_KERNEL_MLOAD                1
#define SUPPORT_KERNEL_DS                   1

// Special frame lock means that the frame rates of input and output are the same in HW design spec.
#define SUPPORT_SPECIAL_FRAMELOCK           FALSE

#define LD_ENABLE                           0  // 1
#define FRC_INSIDE                         TRUE //FALSE
#define FRC_IP_NUM_Passive                  17 //FRC__NUM_FRC_Mapping_mode
//#define ENABLE_SPREADMODE
#define SUPPORT_FHD_MEMC
//#define HW_SUPPORT_FRC_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE

// 480p and 576p have FPLL problem in HV mode.
// So only allow HV mode for 720P
#define ONLY_ALLOW_HV_MODE_FOR_720P         0

//For Manhattan U01 GOP HW bug  //U02 Fix
#define MANHATTAN_GOP_HW_BUG_PATCH          0

#define MAXIM_ATV_LOCK_PHASE_PATCH

#define _ENABLE_SW_DS                      0
#define DS_BUFFER_NUM_EX                   6
#define DS_MAX_INDEX                       6

#define ENABLE_64BITS_COMMAND               1
#define ENABLE_64BITS_SPREAD_MODE           1 //need enable ENABLE_64BITS_COMMAND first
//-------------------------------------------------------------------------------------------------
/// enable ENABLE_MLOAD_SAME_REG_COMBINE you can do:
/// MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_01_L, BIT(N), BIT(N));
/// MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_01_L, BIT(M), BIT(M));
/// MApi_XC_MLoad_Fire();
//-------------------------------------------------------------------------------------------------
#define ENABLE_MLOAD_SAME_REG_COMBINE       1
//#define SRAMPD_XC_CLOSE_TEMP     1

#define IS_SUPPORT_64BITS_COMMAND(bEnable64bitsCmd, u32DeviceID)           ((bEnable64bitsCmd == 1) && (u32DeviceID == 0))
#define ENABLE_DS_4_BASEADDR_MODE           1 // need enable both ENABLE_64BITS_COMMAND and ENABLE_64BITS_SPREAD_MODE first
#define DS_CMD_LEN_64BITS                   8

#define IS_SUPPORT_DS_SR(fps)               0//((fps >= 230) && (fps <= 600))
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
// version6: Monet/Maya/Manhattan:
//       for Manhattan pip:1.SC1(1P) Htt = SC0(2P) Htt/2
//               2.if xc mute SC1 main, please mute SC0 sub. The same to mute color.
//               3.SC1 DE(bank 0x90) HStart = SC0 DE HStart/2, SC1 DE(bank 0x90) Width = SC0 DE Width/2
//               4.temp solution:SC0 DE VStart/Vend should add 6, so is SC1 DE Vstart/Vend
#define HW_DESIGN_4K2K_VER                  (6)

// version0: Not support TV chip as HDMITx
// version1: Maserati + Raptor
// version2: Maxim + inside HDMITx
#define HW_DESIGN_HDMITX_VER                (2)

#define HDMITX_COLORDEPTH_DEFAULT_FACTOR    (100)
#define HDMITX_COLORDEPTH_8BIT_FACTOR       (100)
#define HDMITX_COLORDEPTH_10BIT_FACTOR      (125)
#define HDMITX_COLORDEPTH_12BIT_FACTOR      (150)
#define HDMITX_COLORDEPTH_16BIT_FACTOR      (200)

#define HW_DESIGN_3D_VER                    (3)
#define HW_2DTO3D_SUPPORT                   TRUE
#define HW_2DTO3D_VER                       (4)
#define HW_2DTO3D_BYTE_PER_WORD            (32)
#define HW_2DTO3D_PATCH                     FALSE //a1 u01:2d to 3d hw bug
#define HW_2DTO3D_BLOCK_DR_BUF_SIZE         (0x2200)
#define HW_2DTO3D_DD_BUF_SIZE               (0xFF00)
//HW support check board and pixel alternative
#define HW_SUPPORT_3D_CB_AND_PA             TRUE
#define ENABLE_GOP_T3DPATCH
#define VALUE_AUTO_TUNE_AREA_TRIG_4K540P    (0x0A) //4k0.5k 3D
#define VALUE_DISP_AREA_TRIG_4K540P         (0x0D) //4k0.5k 3D
// maserati use 4 tap scaling in 3D mode. PQ is 6 tap mode. Line buffer is not enough because 12 line mode is disabled in 3D mode.
#define HW_6TAP_MODE_SUPPORT        FALSE

//hw support fbl 3d or not. if support,can do SBS to LBL and SBS to SBS
#define HW_3D_SUPPORT_FBL                   TRUE
//M10, A2, J2 ,A5,A6,A3,Agate HW will automatic use IPM fetch's reg setting to alignment IPM fetch, so skip sw alignment
//and for mirror cbcr swap, need check IPM fetch to decide if need swap
#define HW_IPM_FETCH_ALIGNMENT              TRUE
//hw support 2 line mode deinterlace for interlace or not
#define HW_2LINEMODE_DEINTERLACE_SUPPORT    FALSE
#define HW_CLK_CTRL                         TRUE
#define SUPPORT_OSD_HSLVDS_PATH               1
#define MLG_1024

#define OSD_LAYER_NUM           (5)
#define VIDEO_OSD_SWITCH_VER    (2)

//#define FA_1920X540_OUTPUT
#define XC_SUPPORT_4K2K                     1

// if H/W support 2p mode to achieve 600M HZ
#define XC_SUPPORT_2P_MODE                  TRUE

//device 1 is interlace out
#define XC_DEVICE1_IS_INTERLACE_OUT 0

//if H/W support force post-Vscalin-down in DS mode
#define HW_SUPPORT_FORCE_VSP_IN_DS_MODE     TRUE

//if H/W support LPLL lock freqence not lock phase mode
#define HW_SUPPORT_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE    TRUE

// if H/W support interlace output timing
#define HW_SUPPORT_INTERLACE_OUTPUT TRUE

// if H/W support 4k2k_60p output timing
#define HW_SUPPORT_4K2K_60P_OUTPUT TRUE

#define SUPPORT_MOD_ADBANK_SEPARATE

#define SUPPORT_FPLL_REFER_24MXTAL // Maxim is 2P mode

//#define SUPPORT_FPLL_REFER_24MXTAL_4P   /// 24MXTAL_4P>>24MXTAL

// for 4K 0.5K 240Hz case, if input only 25fps, ivs:ovs = 5:48 case
#define SUPPORT_FPLL_DOUBLE_OVS

#define SUPPORT_HDMI20  1

#define LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE   1

/// for Chip bringup
#define ENABLE_CHIP_BRINGUP
#define PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB             1           // support pip&pop by multi sc ,such as sc1 support pip&pop

#define HW_4K2K_VIP_PEAKING_LIMITATION                      0
#define HW_SCALING_LIMITATION                               0  //NO LIMITATION

// support 3D DS
#define SUPPORT_3D_DS           0

#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
//#define ENABLE_TV_SC2_PQ
#endif
//#define MONACO_SC2_PATCH

//#define PATCH_TCON_BRING_UP

#define XC_SUPPORT_CMA TRUE

#define XC_CMA_8MB 0x0800000
#define XC_CMA_10MB 0x0A00000
#define XC_CMA_12MB 0x0C00000
#define XC_CMA_15MB 0x0F00000
#define XC_CMA_16MB 0x1000000
#define XC_CMA_18MB 0x1200000
#define XC_CMA_19MB 0x1300000
#define XC_CMA_20MB 0x1400000
#define XC_CMA_22MB 0x1600000
#define XC_CMA_24MB 0x1800000
#define XC_CMA_32MB 0x2000000
#define XC_CMA_36MB 0x2400000
#define XC_CMA_30MB 0x1E00000
#define XC_CMA_40MB 0x2800000
#define XC_CMA_48MB 0x3000000
#define XC_CMA_64MB 0x4000000
#define XC_CMA_72MB 0x4800000
#define XC_CMA_96MB 0x6000000

#define XC_4K2K_WIDTH_MAX 4500
#define XC_4K2K_WIDTH_MIN 3000
#define XC_4K2K_HIGH_MAX 2500
#define XC_4K2K_HIGH_MIN 1900

#define XC_4K1K_WIDTH_MAX 4500
#define XC_4K1K_WIDTH_MIN 3000
#define XC_4K1K_HIGH_MAX 1300
#define XC_4K1K_HIGH_MIN 900

#define XC_4K_HALFK_WIDTH_MAX 4500  // 4K 0.5K
#define XC_4K_HALFK_WIDTH_MIN 3000  // 4K 0.5K
#define XC_4K_HALFK_HIGH_MAX 600    // 4K 0.5K
#define XC_4K_HALFK_HIGH_MIN 500    // 4K 0.5K

#define XC_2K2K_WIDTH_MAX 2300
#define XC_2K2K_WIDTH_MIN 1500
#define XC_2K2K_HIGH_MAX 2500
#define XC_2K2K_HIGH_MIN 1900

#define XC_FHD_WIDTH_MAX 2300
#define XC_FHD_WIDTH_MIN 1500
#define XC_FHD_HIGH_MAX 1300
#define XC_FHD_HIGH_MIN 900

#define XC_FP1080P_H_SIZE 1920
#define XC_FP1080P_V_SIZE 2205

#define FRC_MEMORY_PROTECT

#define XC_FRC_IPM_L        0x3F48000   // 4096x2160x1.5x10/2
#define XC_FRC_IPM_R        0x3F48000   // 4096x2160x1.5x10/2
#define XC_FRC_MEDS_L       0x0BDD800   // 2048x1080x1.125x10/2
#define XC_FRC_MEDS_R       0x0BDD800   // 2048x1080x1.125x10/2
#define XC_FRC_ME1_X1_READ  0x0002800   // ME1_X1 pre-fetch size
#define XC_FRC_ME1_X1       0x0061C00   // 8x136x32x11.5
#define XC_FRC_ME1_S1       0x0030E00   // 2x136x32x23
#define XC_FRC_ME2_X2       0x02DD200   // 30x272x32x11.5
#define XC_FRC_ME2_Y2       0x00C3800   // 8x272x32x11.5
#define XC_FRC_ME2_F2       0x01B7E00   // 18x272x32x11.5
#define XC_FRC_ME2_LOGO     0x00C3800   // 8x272x32x11.5
#define XC_FRC_ME2_S2       0x00C3800   // 8x272x32x11.5
#define XC_FRC_HR           0x03FC000   // 40x272x32x12
#define XC_FRC_HR_BUF23     0x02FD000   // 30x272x32x12



//-------------------------------------------------------------------------------------------------
//  Register base
//-------------------------------------------------------------------------------------------------

// PM
#define REG_DDC_BASE                0x000400UL
#define REG_PM_SLP_BASE             0x000E00UL
#define REG_PM_GPIO_BASE            0x000F00UL
#define REG_PM_SLEEP_BASE           REG_PM_SLP_BASE//0x0E00//alex_tung
#define REG_PAD_SAR_BASE            0x001400UL
#define REG_SCDC0_BASE              0x010200UL
#define REG_SCDC1_BASE              0x010300UL
#define REG_SCDC2_BASE              0x010400UL
#define REG_SCDC3_BASE              0x010500UL
#define REG_PM_TOP_BASE             0x001E00UL
#define REG_MHL_CBUS_BANK           0x001F00UL
#define REG_EFUSE_BASE              0x002000UL
#define REG_PM_MHL_CBUS_BANK        0x002F00UL

// NONPM
#define REG_MIU0_BASE               0x101200UL
#define REG_MIU0_EX_BASE            0x161500UL
#define REG_MIU0_ARBB_BASE          0x152000UL
#define REG_MIU1_BASE               0x100600UL
#define REG_MIU1_EX_BASE            0x162200UL
#define REG_MIU1_ARBB_BASE          0x152100UL
#define REG_MIU2_BASE               0x162000UL
#define REG_MIU2_EX_BASE            0x162300UL
#define REG_MIU2_ARBB_BASE          0x152200UL

#define REG_CLKGEN2_BASE            0x100A00UL
#define REG_CLKGEN0_BASE            0x100B00UL  // 0x1E00 - 0x1EFF
#define REG_CHIP_BASE               0x101E00UL
#define REG_UHC0_BASE               0x102400UL
#define REG_UHC1_BASE               0x100D00UL
#define REG_ADC_ATOP_BASE           0x102500UL  // 0x2500 - 0x25FF
#define REG_ADC_DTOP_BASE           0x102600UL  // 0x2600 - 0x26EF
#define REG_ADC_CHIPTOP_BASE        0x101E00UL  // 0x1E00 - 0x1EFF
#define REG_HDMI_BASE               0x102700UL  // 0x2700 - 0x27FF
#define REG_CHIP_GPIO_BASE          0x102B00UL
#define REG_ADC_ATOPB_BASE          0x103D00UL  // 0x3D00 - 0x3DFF

#define REG_HDMI2_BASE              0x101A00UL
#define REG_IPMUX_BASE              0x102E00UL
#define REG_MVOP_BASE               0x101400UL
#define REG_SUBMVOP_BASE            0x101300UL

#if ENABLE_REGISTER_SPREAD
#define REG_SCALER_BASE             0x130000UL
#else
#define REG_SCALER_BASE             0x102F00UL
#endif
#define REG_LPLL_BASE               0x103100UL
#define REG_MOD_BASE                0x103200UL
#define REG_PWM_BASE                0x13F400UL
#define REG_MOD_A_BASE              0x111E00UL
#define REG_AFEC_BASE               0x103500UL
#define REG_COMB_BASE               0x103600UL

#define REG_HDCPKEY_BASE            0x173800UL
#define REG_DVI_ATOP_BASE           0x110900UL
#define REG_DVI_DTOP_BASE           0x110A00UL
#define REG_DVI_EQ_BASE             0x110A80UL     // EQ started from 0x80
#define REG_HDCP_BASE               0x110AC0UL     // HDCP started from 0xC0
#define REG_ADC_DTOPB_BASE          0x111200UL     // ADC DTOPB
#define REG_DVI_ATOP1_BASE          0x113200UL
#define REG_DVI_DTOP1_BASE          0x113300UL
#define REG_DVI_EQ1_BASE            0x113380UL     // EQ started from 0x80
#define REG_HDCP1_BASE              0x1133C0UL     // HDCP started from 0xC0
#define REG_DVI_ATOP2_BASE          0x113400UL
#define REG_DVI_ATOP3_BASE          0x162F00UL
#define REG_DVI_DTOP2_BASE          0x113500UL
#define REG_DVI_EQ2_BASE            0x113580UL     // EQ started from 0x80
#define REG_HDCP2_BASE              0x1135C0UL     // HDCP started from 0xC0
#define REG_DVI_PS_BASE             0x113600UL     // DVI power saving
#define REG_DVI_PS1_BASE            0x113640UL     // DVI power saving1
#define REG_DVI_PS2_BASE            0x113680UL     // DVI power saving2
#define REG_DVI_PS3_BASE            0x1136C0UL     // DVI power saving3
#define REG_DVI_DTOP3_BASE          0x113700UL
#define REG_DVI_EQ3_BASE            0x113780UL     // EQ started from 0x80
#define REG_HDCP3_BASE              0x1137C0UL     // HDCP started from 0xC0

#define REG_CHIP_ID_MAJOR           0x1ECC
#define REG_CHIP_ID_MINOR           0x1ECD
#define REG_CHIP_VERSION            0x1ECE
#define REG_CHIP_REVISION           0x1ECFUL
#define REG_CHIP_GPIO1_BASE            0x110300UL

#define REG_COMBO_PHY0_P0_BASE         0x170200UL
#define REG_COMBO_PHY1_P0_BASE         0x170300UL
#define REG_COMBO_PHY0_P1_BASE         0x170400UL
#define REG_COMBO_PHY1_P1_BASE         0x170500UL
#define REG_COMBO_PHY0_P2_BASE         0x170600UL
#define REG_COMBO_PHY1_P2_BASE         0x170700UL
#define REG_COMBO_PHY0_P3_BASE         0x170800UL
#define REG_COMBO_PHY1_P3_BASE         0x170900UL

#define REG_DVI_DTOP_DUAL_P0_BASE      0x171000UL
#define REG_DVI_RSV_DUAL_P0_BASE       0x171100UL
#define REG_HDCP_DUAL_P0_BASE          0x171200UL
#define REG_DVI_DTOP_DUAL_P1_BASE      0x171300UL
#define REG_DVI_RSV_DUAL_P1_BASE       0x171400UL
#define REG_HDCP_DUAL_P1_BASE          0x171500UL
#define REG_DVI_DTOP_DUAL_P2_BASE      0x171600UL
#define REG_DVI_RSV_DUAL_P2_BASE       0x171700UL
#define REG_HDCP_DUAL_P2_BASE          0x171800UL
#define REG_DVI_DTOP_DUAL_P3_BASE      0x171900UL
#define REG_DVI_RSV_DUAL_P3_BASE       0x171A00UL
#define REG_HDCP_DUAL_P3_BASE          0x171B00UL

#define REG_HDMI_DUAL_0_BASE           0x173000UL
#define REG_HDMI2_DUAL_0_BASE          0x173100UL
#define REG_HDMI3_DUAL_0_BASE          0x173400UL
#define REG_HDMI_DUAL_1_BASE           0x173200UL
#define REG_HDMI2_DUAL_1_BASE          0x173300UL
#define REG_HDMI3_DUAL_1_BASE          0x173600UL

#define REG_COMBO_GP_TOP_BASE          0x173900UL
#define REG_SECURE_TZPC_BASE           0x173A00UL

////////////////////////// FRC using ////////////////////////////////
#define REG_CLKGEN0_BASE            0x100B00UL
#define REG_CLKGEN1_BASE            0x103300UL

///FRC Area
#define REG_FSC_BANK_BASE           0x140000UL  // FSC 0x102C bank, direct bank is 0x1400
#define REG_FRC_BANK_BASE           0x400000UL

#define L_CLKGEN0(x)                BK_REG_L(REG_CLKGEN0_BASE, x)
#define H_CLKGEN0(x)                BK_REG_H(REG_CLKGEN0_BASE, x)
#define L_CLKGEN1(x)                BK_REG_L(REG_CLKGEN1_BASE, x)
#define H_CLKGEN1(x)                BK_REG_H(REG_CLKGEN1_BASE, x)
#define L_CLKGEN2(x)                BK_REG_L(REG_CLKGEN2_BASE, x)
#define H_CLKGEN2(x)                BK_REG_H(REG_CLKGEN2_BASE, x)

///////////////////////////////////////////////////////////////////

// store bank
#define LPLL_BK_STORE     \
        MS_U8 u8Bank;      \
        u8Bank = MDrv_ReadByte(REG_LPLL_BASE)

// restore bank
#define LPLL_BK_RESTORE     MDrv_WriteByte(REG_LPLL_BASE, u8Bank)

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

#define REG_CKG_EDCLK_F1            (REG_CLKGEN0_BASE + 0x9C ) // EDCLK_F1, CLK_GEN0_4E_L
    #define CKG_EDCLK_F1_GATED          BIT(0)
    #define CKG_EDCLK_F1_INVERT         BIT(1)
    #define CKG_EDCLK_F1_MASK           BMASK(4:2)
    #define CKG_EDCLK_F1_ADC            (0 << 2)
    #define CKG_EDCLK_F1_DVI            (1 << 2)
    #define CKG_EDCLK_F1_345MHZ         (2 << 2)
    #define CKG_EDCLK_F1_216MHZ         (3 << 2)
    #define CKG_EDCLK_F1_192MHZ         (4 << 2)
    #define CKG_EDCLK_F1_240MHZ         (5 << 2)
    #define CKG_EDCLK_F1_320MHZ         (6 << 2)
    #define CKG_EDCLK_F1_XTAL           (7 << 2)

#define REG_CKG_EDCLK_F2            (REG_CLKGEN0_BASE + 0x9D ) // EDCLK_F2, CLK_GEN0_4E_L
    #define CKG_EDCLK_F2_GATED          BIT(0)
    #define CKG_EDCLK_F2_INVERT         BIT(1)
    #define CKG_EDCLK_F2_MASK           BMASK(4:2)
    #define CKG_EDCLK_F2_ADC            (0 << 2)
    #define CKG_EDCLK_F2_DVI            (1 << 2)
    #define CKG_EDCLK_F2_345MHZ         (2 << 2)
    #define CKG_EDCLK_F2_216MHZ         (3 << 2)
    #define CKG_EDCLK_F2_192MHZ         (4 << 2)
    #define CKG_EDCLK_F2_240MHZ         (5 << 2)
    #define CKG_EDCLK_F2_320MHZ         (6 << 2)
    #define CKG_EDCLK_F2_XTAL           (7 << 2)

#define REG_CKG_FMCLK          (REG_CLKGEN0_BASE + 0xBB )
    #define CKG_FMCLK_GATED             BIT(0)
    #define CKG_FMCLK_INVERT            BIT(1)
    #define CKG_FMCLK_MASK              BMASK(3:2)
    #define CKG_FMCLK_FCLK              (0 << 2)
    #define CKG_FMCLK_MIU_256           (1 << 2)
    #define CKG_FMCLK_MIU_128           (2 << 2)

#define REG_CKG_SC_ROT          (REG_CLKGEN0_BASE + 0xFF )
    #define CKG_SC_ROT_GATED            BIT(0)
    #define CKG_SC_ROT_INVERT           BIT(1)
    #define CKG_SC_ROT_MASK             BMASK(3:2)
    #define CKG_SC_ROT_MIU_256          (0 << 2)
    #define CKG_SC_ROT_MIU_128          (1 << 2)

#define REG_CKG_FICLK_F1        (REG_CLKGEN0_BASE + 0xA2 ) // scaling line buffer, set to fclk if post scaling, set to idclk is pre-scaling
    #define CKG_FICLK_F1_GATED      BIT(0)
    #define CKG_FICLK_F1_INVERT     BIT(1)
    #define CKG_FICLK_F1_MASK       BMASK(3:2)
    #define CKG_FICLK_F1_IDCLK1     (0 << 2)
    #define CKG_FICLK_F1_FLK        (1 << 2)
    //#define CKG_FICLK_F1_XTAL       (3 << 2)

#define REG_CKG_FICLK_F2        (REG_CLKGEN0_BASE + 0xA3 ) // for Monaco, this CLK is for VE using, not for FLCIK, and should set 0x00 for VE
    #define CKG_FICLK_F2_GATED      BIT(0)
    #define CKG_FICLK_F2_INVERT     BIT(1)
    #define CKG_FICLK_F2_MASK      BMASK(3:2)
    #define CKG_FICLK_F2_IDCLK2     (0 << 2)
    #define CKG_FICLK_F2_FLK        (0 << 2)
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
    #define CKG_FCLK_MASK           BMASK(4:2)
    #define CKG_FCLK_170MHZ         (0 << 2)
    #define CKG_FCLK_CLK_MIU        (1 << 2)
    #define CKG_FCLK_345MHZ         (2 << 2)
    #define CKG_FCLK_216MHZ         (3 << 2)
    #define CKG_FCLK_192MHZ         (4 << 2)
    #define CKG_FCLK_240MHZ         (5 << 2)
    #define CKG_FCLK_320MHZ         (6 << 2)
    #define CKG_FCLK_XTAL           (7 << 2)
    #define CKG_FCLK_XTAL_          CKG_FCLK_XTAL//(8 << 2) for A5 no XTAL
    #define CKG_FCLK_DEFAULT        CKG_FCLK_345MHZ

#define REG_CKG_ODCLK           (REG_CLKGEN0_BASE + 0xA6 ) // output dot clock, usually select LPLL, select XTAL when debug
    #define CKG_ODCLK_GATED         BIT(0)
    #define CKG_ODCLK_INVERT        BIT(1)
    #define CKG_ODCLK_MASK          BMASK(5:2)
    #define CKG_ODCLK_SC_PLL        (0 << 2)
    #define CKG_ODCLK_LPLL_DIV2     (1 << 2)
    #define CKG_ODCLK_27M           (2 << 2)
    #define CKG_ODCLK_CLK_LPLL      (3 << 2)
    //#define CKG_ODCLK_XTAL          (8 << 2)

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
    #define CKG_IDCLK0_XTAL         CKG_IDCLK0_ODCLK//(8 << 2) for A5 no XTAL, select as OD

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
    #define CKG_IDCLK1_XTAL         CKG_IDCLK1_ODCLK//(8 << 2) for A5 no XTAL,select as OD

#define REG_CKG_PRE_IDCLK1       (REG_CLKGEN0_BASE + 0xBC ) // pre-main window idclk
    #define CKG_PRE_IDCLK1_MASK         BMASK(5:3)
    #define CKG_PRE_IDCLK1_CLK_ADC      (0 << 3)
    #define CKG_PRE_IDCLK1_CLK_DVI      (1 << 3)
    #define CKG_PRE_IDCLK1_CLK_MHL      (2 << 3)

#define REG_CKG_IDCLK2          (REG_CLKGEN0_BASE + 0xAA ) // main window idclk
    #define CKG_IDCLK2_GATED        BIT(0)
    #define CKG_IDCLK2_INVERT       BIT(1)
    #define CKG_IDCLK2_MASK         BMASK(5:2)
    #define CKG_IDCLK2_CLK_ADC      (0 << 2)
    #define CKG_IDCLK2_CLK_DVI      (1 << 2)
    #define CKG_IDCLK2_CLK_VD       (2 << 2)
    #define CKG_IDCLK2_CLK_DC0      (3 << 2)
    #define CKG_IDCLK2_CLK_ADC2     (4 << 2)
    #define CKG_IDCLK2_0            (5 << 2)
    #define CKG_IDCLK2_00           (6 << 2)
    #define CKG_IDCLK2_ODCLK        (7 << 2)               // same as 5 --> also is 0
    #define CKG_IDCLK2_CLK_SUB_DC0  (8 << 2)
    #define CKG_IDCLK2_CLK_ADC3     (9 << 2)
    #define CKG_IDCLK2_ODCLK2       (10 << 2)
    #define CKG_IDCLK2_CLKMHL       (11 << 2)
    #define CKG_IDCLK2_XTAL         CKG_IDCLK2_ODCLK//(8 << 2)no XTAL select as OD

#define REG_CKG_PRE_IDCLK2       (REG_CLKGEN0_BASE + 0xBC ) // pre-main window idclk
    #define CKG_PRE_IDCLK2_MASK         BMASK(8:6)
    #define CKG_PRE_IDCLK2_CLK_ADC      (0 << 6)
    #define CKG_PRE_IDCLK2_CLK_DVI      (1 << 6)
    #define CKG_PRE_IDCLK2_CLK_MHL      (2 << 6)

#define REG_CKG_IDCLK3          (REG_CLKGEN0_BASE + 0xB2 )
    #define CKG_IDCLK3_GATED        BIT(0)
    #define CKG_IDCLK3_INVERT       BIT(1)
    #define CKG_IDCLK3_MASK         BMASK(5:2)
    #define CKG_IDCLK3_CLK_ADC      (0 << 2)
    #define CKG_IDCLK3_CLK_DVI      (1 << 2)
    #define CKG_IDCLK3_CLK_VD       (2 << 2)
    #define CKG_IDCLK3_CLK_DC0      (3 << 2)
    #define CKG_IDCLK3_ODCLK        (4 << 2)
    #define CKG_IDCLK3_0            (5 << 2)
    #define CKG_IDCLK3_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK3_00           (7 << 2)               // same as 5 --> also is 0
    #define CKG_IDCLK3_XTAL         (8 << 2)

#define REG_CKG_IDCLK_USR_ENA       (REG_CLKGEN0_BASE + 0xB4 ) // idclk user enable
    #define CKG_IDCLK3_USR_ENA        BIT(3)

#define REG_CKG_PRE_IDCLK3       (REG_CLKGEN0_BASE + 0xBC )
    #define CKG_PRE_IDCLK3_MASK         BMASK(11:9)
    #define CKG_PRE_IDCLK3_CLK_ADC      (0 << 9)
    #define CKG_PRE_IDCLK3_CLK_DVI      (1 << 9)
    #define CKG_PRE_IDCLK3_CLK_MHL      (2 << 9)

#define REG_CKG_PDW0            (REG_CLKGEN0_BASE + 0xBE )
    #define CKG_PDW0_GATED          BIT(0)
    #define CKG_PDW0_INVERT         BIT(1)
    #define CKG_PDW0_MASK           BMASK(4:2)
    #define CKG_PDW0_CLK_ADC        (0 << 2)
    #define CKG_PDW0_CLK_DVI        (1 << 2)
    #define CKG_PDW0_CLK_VD         (2 << 2)
    #define CKG_PDW0_CLK_DC0        (3 << 2)
    #define CKG_PDW0_ODCLK          (4 << 2)
    #define CKG_PDW0_0              (5 << 2)
    #define CKG_PDW0_CLK_VD_ADC     (6 << 2)
    #define CKG_PDW0_00             (7 << 2)               // same as 5 --> also is 0
    #define CKG_PDW0_XTAL           (8 << 2)

#define REG_CKG_PDW1            (REG_CLKGEN0_BASE + 0xBF )
    #define CKG_PDW1_GATED          BIT(0)
    #define CKG_PDW1_INVERT         BIT(1)
    #define CKG_PDW1_MASK           BMASK(4:2)
    #define CKG_PDW1_CLK_ADC        (0 << 2)
    #define CKG_PDW1_CLK_DVI        (1 << 2)
    #define CKG_PDW1_CLK_VD         (2 << 2)
    #define CKG_PDW1_CLK_DC0        (3 << 2)
    #define CKG_PDW1_ODCLK          (4 << 2)
    #define CKG_PDW1_0              (5 << 2)
    #define CKG_PDW1_CLK_VD_ADC     (6 << 2)
    #define CKG_PDW1_00             (7 << 2)               // same as 5 --> also is 0
    #define CKG_PDW1_XTAL           (8 << 2)

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

//MVOP 8bit address
#define REG_MVOP_MIRROR         (REG_MVOP_BASE    + 0x76)
#define REG_MVOP_CROP_H_START   (REG_MVOP_BASE    + 0x80)
#define REG_MVOP_CROP_V_START   (REG_MVOP_BASE    + 0x82)
#define REG_MVOP_CROP_H_SIZE    (REG_MVOP_BASE    + 0x84)
#define REG_MVOP_CROP_V_SIZE    (REG_MVOP_BASE    + 0x86)
#define REG_SUBMVOP_MIRROR         (REG_SUBMVOP_BASE    + 0x76)
#define REG_SUBMVOP_CROP_H_START   (REG_SUBMVOP_BASE    + 0x80)
#define REG_SUBMVOP_CROP_V_START   (REG_SUBMVOP_BASE    + 0x82)
#define REG_SUBMVOP_CROP_H_SIZE    (REG_SUBMVOP_BASE    + 0x84)
#define REG_SUBMVOP_CROP_V_SIZE    (REG_SUBMVOP_BASE    + 0x86)

#define REG_CKG_S2_MECLK      (REG_CLKGEN2_BASE + 0x80 )
    #define CKG_S2_MECLK_GATED      BIT(0)
    #define CKG_S2_MECLK_INVERT     BIT(1)
    #define CKG_S2_MECLK_MASK      BMASK(5:2)

#define REG_CKG_S2_MGCLK      (REG_CLKGEN2_BASE + 0x82 )
    #define CKG_S2_MGCLK_GATED      BIT(0)
    #define CKG_S2_MGCLK_INVERT     BIT(1)
    #define CKG_S2_MGCLK_MASK      BMASK(5:2)

#define REG_CKG_S2_GOP_HDR      (REG_CLKGEN2_BASE + 0x84 )
    #define CKG_S2_GOP_HDR_GATED      BIT(0)
    #define CKG_S2_GOP_HDR_INVERT     BIT(1)
    #define CKG_S2_GOP_HDR_MASK      BMASK(5:2)
    #define CKG_S2_GOP_HDR_ODCLK     (0 << 2)
    #define CKG_S2_GOP_HDR_EDCLK     (1 << 2)

//// for SC2, at REG_CLKGEN2_BASE
#define REG_CKG_S2_FICLK_F1        (REG_CLKGEN2_BASE + 0xC2 )
    #define CKG_S2_FICLK_F1_GATED      BIT(0)
    #define CKG_S2_FICLK_F1_INVERT     BIT(1)
    #define CKG_S2_FICLK_F1_MASK       BMASK(3:2)
    #define CKG_S2_FICLK_F1_IDCLK1     (0 << 2)
    #define CKG_S2_FICLK_F1_FLK        (1 << 2)

#define REG_CKG_S2_FICLK_F2        (REG_CLKGEN2_BASE + 0xC3 )
    #define CKG_S2_FICLK_F2_GATED      BIT(0)
    #define CKG_S2_FICLK_F2_INVERT     BIT(1)
    #define CKG_S2_FICLK_F2_MASK      BMASK(3:2)
    #define CKG_S2_FICLK_F2_IDCLK2     (0 << 2)
    #define CKG_S2_FICLK_F2_FLK        (1 << 2)

#define REG_CKG_S2_FICLK2_F2        (REG_SCALER_BASE + REG_SC_BKBF_20_H)
    #define CKG_S2_FICLK2_F2_GATED      BIT(0)
    #define CKG_S2_FICLK2_F2_INVERT     BIT(1)
    #define CKG_S2_FICLK2_F2_MASK       BMASK(3:2)
    #define CKG_S2_FICLK2_F2_IDCLK2     (0 << 2)   // v prescaling
    #define CKG_S2_FICLK2_F2_FCLK       (1 << 2)   // normal
    #define CKG_S2_FICLK2_F2_MIUCLK     (2 << 2)  // DIP case

#define REG_CKG_S2_FCLK            (REG_CLKGEN0_BASE+ 0xAF ) // after memory, before fodclk
    #define CKG_S2_FCLK_GATED          BIT(0)
    #define CKG_S2_FCLK_INVERT         BIT(1)
    #define CKG_S2_FCLK_MASK           BMASK(4:2)
    #define CKG_S2_FCLK_172MHZ         (0 << 2)
    #define CKG_S2_FCLK_CLK_MIU        (1 << 2)
    #define CKG_S2_FCLK_345MHZ         (2 << 2)
    #define CKG_S2_FCLK_216MHZ         (3 << 2)
    #define CKG_S2_FCLK_192MHZ         (4 << 2)
    #define CKG_S2_FCLK_240MHZ         (5 << 2)
    #define CKG_S2_FCLK_320MHZ         (6 << 2)
    #define CKG_S2_FCLK_XTAL           (7 << 2)
    #define CKG_S2_FCLK_DEFAULT        CKG_S2_FCLK_320MHZ

#define REG_CKG_S2_FODCLK           (REG_CLKGEN2_BASE + 0xC4 )
    #define CKG_S2_FODCLK_GATED         BIT(0)
    #define CKG_S2_FODCLK_INVERT        BIT(1)
    #define CKG_S2_FODCLK_CLK_ODCLK     (0 << 2)
    #define CKG_S2_FODCLK_CLK_MIU       (1 << 2)

#define REG_CKG_S2_ODCLK           (REG_CLKGEN2_BASE + 0xC6 )
    #define CKG_S2_ODCLK_GATED         BIT(0)
    #define CKG_S2_ODCLK_INVERT        BIT(1)
    #define CKG_S2_ODCLK_MASK          BMASK(3:2)
    #define CKG_S2_ODCLK_SYN_CLK       (0 << 2)
    #define CKG_S2_ODCLK_LPLL_DIV2     (1 << 2)
    #define CKG_S2_ODCLK_27M           (2 << 2)
    #define CKG_S2_ODCLK_CLK_LPLL      (3 << 2)

#define REG_CKG_S2_IDCLK0          (REG_CLKGEN2_BASE + 0xC8 ) // off-line detect idclk
    #define CKG_S2_IDCLK0_GATED        BIT(0)
    #define CKG_S2_IDCLK0_INVERT       BIT(1)
    #define CKG_S2_IDCLK0_MASK         BMASK(5:2)
    #define CKG_S2_IDCLK0_CLK_ADC      (0 << 2)
    #define CKG_S2_IDCLK0_CLK_DVI      (1 << 2)
    #define CKG_S2_IDCLK0_CLK_VD       (2 << 2)
    #define CKG_S2_IDCLK0_CLK_DC0      (3 << 2)
    #define CKG_S2_IDCLK0_CLK_ADC2     (4 << 2)
    #define CKG_S2_IDCLK0_0            (5 << 2)
    #define CKG_S2_IDCLK0_00           (6 << 2)
    #define CKG_S2_IDCLK0_ODCLK        (7 << 2)
    #define CKG_S2_IDCLK0_CLK_SUB_DC0  (8 << 2)
    #define CKG_S2_IDCLK0_CLK_ADC3     (9 << 2)
    #define CKG_S2_IDCLK0_ODCLK2       (10<< 2)
    #define CKG_S2_IDCLK0_MHL          (13<< 2)
    #define CKG_S2_IDCLK0_XTAL         CKG_S2_IDCLK0_ODCLK

#define REG_CKG_S2_IDCLK1          (REG_CLKGEN2_BASE + 0xC9 ) // off-line detect idclk
    #define CKG_S2_IDCLK1_GATED        BIT(0)
    #define CKG_S2_IDCLK1_INVERT       BIT(1)
    #define CKG_S2_IDCLK1_MASK         BMASK(5:2)
    #define CKG_S2_IDCLK1_CLK_ADC      (0 << 2)
    #define CKG_S2_IDCLK1_CLK_DVI      (1 << 2)
    #define CKG_S2_IDCLK1_CLK_VD       (2 << 2)
    #define CKG_S2_IDCLK1_CLK_DC0      (3 << 2)
    #define CKG_S2_IDCLK1_CLK_ADC2     (4 << 2)
    #define CKG_S2_IDCLK1_0            (5 << 2)
    #define CKG_S2_IDCLK1_00           (6 << 2)
    #define CKG_S2_IDCLK1_ODCLK        (7 << 2)
    #define CKG_S2_IDCLK1_CLK_SUB_DC0  (8 << 2)
    #define CKG_S2_IDCLK1_CLK_ADC3     (9 << 2)
    #define CKG_S2_IDCLK1_ODCLK2       (10<< 2)
    #define CKG_S2_IDCLK1_MHL          (13<< 2)
    #define CKG_S2_IDCLK1_XTAL         CKG_S2_IDCLK1_ODCLK

#define REG_CKG_S2_IDCLK2          (REG_CLKGEN2_BASE + 0xCA ) // off-line detect idclk
    #define CKG_S2_IDCLK2_GATED        BIT(0)
    #define CKG_S2_IDCLK2_INVERT       BIT(1)
    #define CKG_S2_IDCLK2_MASK         BMASK(5:2)
    #define CKG_S2_IDCLK2_CLK_ADC      (0 << 2)
    #define CKG_S2_IDCLK2_CLK_DVI      (1 << 2)
    #define CKG_S2_IDCLK2_CLK_VD       (2 << 2)
    #define CKG_S2_IDCLK2_CLK_DC0      (3 << 2)
    #define CKG_S2_IDCLK2_CLK_ADC2     (4 << 2)
    #define CKG_S2_IDCLK2_0            (5 << 2)
    #define CKG_S2_IDCLK2_00           (6 << 2)
    #define CKG_S2_IDCLK2_ODCLK        (7 << 2)
    #define CKG_S2_IDCLK2_CLK_SUB_DC0  (8 << 2)
    #define CKG_S2_IDCLK2_CLK_ADC3     (9 << 2)
    #define CKG_S2_IDCLK2_ODCLK2       (10<< 2)
    #define CKG_S2_IDCLK2_MHL          (13<< 2)
    #define CKG_S2_IDCLK2_XTAL         CKG_S2_IDCLK2_ODCLK

#define REG_CKG_S2_IDCLK3         (REG_CLKGEN2_BASE + 0xD2 ) // off-line detect idclk
    #define CKG_S2_IDCLK3_ATED         BIT(0)
    #define CKG_S2_IDCLK3_INVERT       BIT(1)
    #define CKG_S2_IDCLK3_MASK         BMASK(5:2)
    #define CKG_S2_IDCLK3_CLK_ADC      (0 << 2)
    #define CKG_S2_IDCLK3_CLK_DVI      (1 << 2)
    #define CKG_S2_IDCLK3_CLK_VD       (2 << 2)
    #define CKG_S2_IDCLK3_CLK_DC0      (3 << 2)
    #define CKG_S2_IDCLK3_CLK_ADC2     (4 << 2)
    #define CKG_S2_IDCLK3_0            (5 << 2)
    #define CKG_S2_IDCLK3_00           (6 << 2)
    #define CKG_S2_IDCLK3_ODCLK        (7 << 2)
    #define CKG_S2_IDCLK3_CLK_SUB_DC0  (8 << 2)
    #define CKG_S2_IDCLK3_CLK_ADC3     (9 << 2)
    #define CKG_S2_IDCLK3_ODCLK2       (10<< 2)
    #define CKG_S2_IDCLK3_MHL          (13<< 2)
    #define CKG_S2_IDCLK3_XTAL         CKG_S2_IDCLK3_ODCLK


#define REG_S2_DE_ONLY_F3          (REG_CLKGEN2_BASE + 0xC0 )
    #define S2_DE_ONLY_F3_MASK         BIT(3)

#define REG_S2_DE_ONLY_F2          (REG_CLKGEN2_BASE + 0xC0 )
    #define S2_DE_ONLY_F2_MASK         BIT(2)

#define REG_S2_DE_ONLY_F1          (REG_CLKGEN2_BASE + 0xC0 )
    #define S2_DE_ONLY_F1_MASK         BIT(1)

#define REG_S2_DE_ONLY_F0          (REG_CLKGEN2_BASE + 0xC0 )
    #define S2_DE_ONLY_F0_MASK         BIT(0)

////
#define CLK_SRC_IDCLK2  0
#define CLK_SRC_FCLK    1
#define CLK_SRC_XTAL    3

#define MIU0_G0_REQUEST_MASK    (REG_MIU0_BASE + 0x46)
#define MIU0_G1_REQUEST_MASK    (REG_MIU0_BASE + 0x66)
#define MIU0_G2_REQUEST_MASK    (REG_MIU0_BASE + 0x86)
#define MIU0_G3_REQUEST_MASK    (REG_MIU0_BASE + 0xA6)
#define MIU0_G4_REQUEST_MASK    (REG_MIU0_EX_BASE + 0x06)
#define MIU0_G5_REQUEST_MASK    (REG_MIU0_EX_BASE + 0x26)
#define MIU0_G6_REQUEST_MASK    (REG_MIU0_ARBB_BASE + 0x06)

#define MIU1_G0_REQUEST_MASK    (REG_MIU1_BASE + 0x46)
#define MIU1_G1_REQUEST_MASK    (REG_MIU1_BASE + 0x66)
#define MIU1_G2_REQUEST_MASK    (REG_MIU1_BASE + 0x86)
#define MIU1_G3_REQUEST_MASK    (REG_MIU1_BASE + 0xA6)
#define MIU1_G4_REQUEST_MASK    (REG_MIU1_EX_BASE + 0x06)
#define MIU1_G5_REQUEST_MASK    (REG_MIU1_EX_BASE + 0x26)
#define MIU1_G6_REQUEST_MASK    (REG_MIU1_ARBB_BASE + 0x06)

#define MIU_SC_G0REQUEST_MASK   (0x0000)
#define MIU_SC_G1REQUEST_MASK   (0x0000)
#define MIU_SC_G2REQUEST_MASK   (0x0000)
#define MIU_SC_G3REQUEST_MASK   (0x0000)
#define MIU_SC_G4REQUEST_MASK   (0x0000)
#define MIU_SC_G5REQUEST_MASK   (0x0000)
#define MIU_SC_G6REQUEST_MASK   (0xF603)

////////////////////////// FRC using ////////////////////////////////

#define MIU_FRC_G0REQUEST_MASK   (0x0000)
#define MIU_FRC_G1REQUEST_MASK   (0x0000)
#define MIU_FRC_G2REQUEST_MASK   (0x0000)
#define MIU_FRC_G3REQUEST_MASK   (0x0000)
#define MIU_FRC_G4REQUEST_MASK   (0x0000)
#define MIU_FRC_G5REQUEST_MASK   (0x7FFF)
#define MIU_FRC_G6REQUEST_MASK   (0x0000)



///////////////////////////////////////////////////////////////////

#define IP_DE_HSTART_MASK       (0x1FFF) //BK_01_13 BK_03_13
#define IP_DE_HEND_MASK         (0x1FFF) //BK_01_15 BK_03_15
#define IP_DE_VSTART_MASK       (0x1FFF) //BK_01_12 BK_03_12
#define IP_DE_VEND_MASK         (0x1FFF) //BK_01_14 BK_03_14

#define VOP_DE_HSTART_MASK      (0x3FFF) //BK_10_04
#define VOP_DE_HEND_MASK        (0x3FFF) //BK_10_05
#define VOP_DE_VSTART_MASK      (0x1FFF) //BK_10_06
#define VOP_DE_VEND_MASK        (0x1FFF) //BK_10_07

#define VOP_VTT_MASK            (0x1FFF) //BK_10_0D
#define VOP_HTT_MASK            (0x3FFF) //BK_10_0C

#define VOP_VSYNC_END_MASK      (0x1FFF) //BK_10_03
#define VOP_DISPLAY_HSTART_MASK (0x3FFF) //BK_10_08
#define VOP_DISPLAY_HEND_MASK   (0x3FFF) //BK_10_09
#define VOP_DISPLAY_VSTART_MASK (0x1FFF) //BK_10_0A
#define VOP_DISPLAY_VEND_MASK   (0x1FFF) //BK_10_0B


#define HW_DESIGN_LD_VER                    (2)

#define FPLL_THRESH_MODE_SUPPORT    0

#define ADC_EFUSE_IN_MBOOT

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
#define ADC_HARDWARE_AUTOOFFSET_RGB         ENABLE
#define ADC_HARDWARE_AUTOOFFSET_YPBPR       ENABLE
#define ADC_HARDWARE_AUTOOFFSET_SCARTRGB    ENABLE
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

// patch for china player
// there are some undefined signal between sync and DE in YPbPr fullHD
// so we ignore this signal with coast window
//#define ADC_BYPASS_YPBPR_MACRO_VISION_PATCH

#define SUPPORT_SC0_SUB_WIN         FALSE
#define SUPPORT_DUAL_MIU_MIRROR_SWAP_IPM TRUE


// GOP need this define, MI system would call MDrv_SC_SetOSDBlendingFormula before XC init
// determinated dual op by panel type would be failed.
#define GOP_SUPPORT_DUALRATE


// IP Authorization number for dolby
#define IPAUTH_DOLBY_HDR_PIN 118

#endif /* MHAL_XC_CONFIG_H */

