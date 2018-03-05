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

#ifndef _REG_GOP_H_
#define _REG_GOP_H_

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------
// HW IP Reg Base Adr
//----------------------------------------------------------------------------
#define GOP_REG_BASE                           0x1F00UL
#define GE_REG_BASE                            0x2800UL
#define SC1_REG_BASE                           0x2F00UL
#define CKG_REG_BASE                           0x0B00UL
#define MIU0_REG_BASE                          0x0600UL
#define MIU_REG_BASE                           0x1200UL
#define MIU2_REG_BASE                          0x162000
#define MVOP_REG_BASE                          0x1400UL
#define SC1_DIRREG_BASE                        0x130000UL

//----------------------------------------------------------------------------
// Scaler Reg
//----------------------------------------------------------------------------
#define XC_REG(bk, reg)                        (SC1_REG_BASE+((MS_U32)(bk)<<16) + (reg) * 2)

#define REG_SC_BK00_00_L                        XC_REG(0x00, 0x00)
#define REG_SC_BK00_05_L                        XC_REG(0x00, 0x05)
#define REG_SC_BK00_06_L                        XC_REG(0x00, 0x06)
#define REG_SC_BK01_02_L                        XC_REG(0x01, 0x02)
#define REG_SC_BK02_5F_L                        XC_REG(0x02, 0x5F)
#define REG_SC_BK0F_2B_L                        XC_REG(0x0F, 0x2B)
#define REG_SC_BK10_23_L                        XC_REG(0x10, 0x23)
#define REG_SC_BK10_50_L                        XC_REG(0x10, 0x50)
#define REG_SC_BK10_5B_L                        XC_REG(0x10, 0x5B)
#define REG_SC_BK12_03_L                        XC_REG(0x12, 0x03)
#define REG_SC_BK37_22_L                        XC_REG(0x37, 0x22)
#define REG_SC_BK37_24_L                        XC_REG(0x37, 0x24)
#define REG_SC_BK37_28_L                        XC_REG(0x37, 0x28)
#define REG_SC_BK3D_0D_L                        XC_REG(0x3D, 0x0D)
#define REG_SC_BK3E_28_L                        XC_REG(0x3E, 0x28)
#define REG_SC_BK40_22_L                        XC_REG(0x40, 0x22)
#define REG_SC_BK40_23_L                        XC_REG(0x40, 0x23)
#define REG_SC_BK40_24_L                        XC_REG(0x40, 0x24)
#define REG_SC_BK40_25_L                        XC_REG(0x40, 0x25)
#define REG_SC_BK7F_10_L                        XC_REG(0x7F, 0x10)

#define GOP_SC_BANKSEL                          REG_SC_BK00_00_L
#define GOP_SC_CHANNELSYNC                      REG_SC_BK00_05_L
#define GOP_SC_GOPEN                            REG_SC_BK00_06_L
#define GOP_SC_IP_SYNC                          REG_SC_BK01_02_L
#define GOP_SC_IP2GOP_SRCSEL                    REG_SC_BK02_5F_L
#define GOP_SC_OSD_CHECK_ALPHA                  REG_SC_BK0F_2B_L
#define GOP_SC_VOPNBL                           REG_SC_BK10_23_L
#define GOP_SC_GOPEN_DWIN                       REG_SC_BK10_50_L
#define GOP_SC_GOPENMODE1                       REG_SC_BK10_5B_L
#define GOP_SC_MIRRORCFG                        REG_SC_BK12_03_L
#define GOP_SC_OCMIXER                          REG_SC_BK37_22_L
#define GOP_SC_OCMISC                           REG_SC_BK37_24_L
#define GOP_SC_OCALPHA                          REG_SC_BK37_28_L
#define GOP_SC_GOPSC_SRAM_CTRL                  REG_SC_BK3D_0D_L
#define GOP_SC_GOPEN_BYPASS                     REG_SC_BK3E_28_L
#define GOP_SC_FRC_LAYER1_L_EN                  REG_SC_BK40_22_L
#define GOP_SC_FRC_LAYER1_R_EN                  REG_SC_BK40_23_L
#define GOP_SC_FRC_LAYER2_L_EN                  REG_SC_BK40_24_L
#define GOP_SC_FRC_LAYER2_R_EN                  REG_SC_BK40_25_L
#define GOP_SC_MIU_SEL                          REG_SC_BK7F_10_L

//----------------------------------------------------------------------------
// MVOP Reg
//----------------------------------------------------------------------------
#define GOP_MVOP_MIRRORCFG                      (MVOP_REG_BASE+(0x3B*2))


//----------------------------------------------------------------------------
// GE Reg
//----------------------------------------------------------------------------
#define GOP_GE_FMT_BLT                          (GE_REG_BASE+(0x01*2))
#define GOP_GE_EN_CMDQ                          BIT(0)
#define GOP_GE_EN_VCMDQ                         BIT(1)

#define GOP_GE_VQ_FIFO_STATUS_L                 (GE_REG_BASE+(0x04*2))
#define GOP_GE_VQ_FIFO_STATUS_H                 (GE_REG_BASE+(0x05*2))

#define GOP_GE_STATUS                           (GE_REG_BASE+(0x07*2))
#define GOP_GE_BUSY                             BIT(0)
#define GOP_GE_CMDQ1_STATUS                     BMASK(7:3)
#define GOP_GE_CMDQ2_STATUS                     BMASK(15:11)

#define GOP_GE_TAG                              (GE_REG_BASE+(0x2C*2))

#define GOP_GE_DBBASE0                          (GE_REG_BASE+(0x26*2))
#define GOP_GE_DBBASE1                          (GE_REG_BASE+(0x27*2))
#define GOP_GE_DBPIT                            (GE_REG_BASE+(0x33*2))
#define GOP_GE_FBFMT                            (GE_REG_BASE+(0x34*2))
#define GOP_GE_SRCW                             (GE_REG_BASE+(0x6e*2))
#define GOP_GE_SRCH                             (GE_REG_BASE+(0x6f*2))


//----------------------------------------------------------------------------
// ChipTop Reg
//----------------------------------------------------------------------------
/* GOP0 and GOP1 CLK */
#define GOP_GOPCLK              (CKG_REG_BASE+(0x40<<1))
#define CKG_GOPG0_DISABLE_CLK   ~(GOP_BIT0)
#define CKG_GOPG0_ODCLK         (0 << 2)
#define CKG_GOPG0_IDCLK2        (1 << 2)
#define CKG_GOPG0_IDCLK1        (2 << 2)
#define CKG_GOPG0_IDCLK3        (3 << 2)
#define CKG_GOPG0_DISABLE_CLK_MASK    (GOP_BIT0)
#define CKG_GOPG0_MASK          (GOP_BIT4 | GOP_BIT3 | GOP_BIT2)

#define CKG_GOPG1_DISABLE_CLK   ~(GOP_BIT8)
#define CKG_GOPG1_ODCLK         (0 << 10)
#define CKG_GOPG1_IDCLK2        (1 << 10)
#define CKG_GOPG1_IDCLK1        (2 << 10)
#define CKG_GOPG1_IDCLK3        (3 << 10)
#define CKG_GOPG1_DISABLE_CLK_MASK    (GOP_BIT8)
#define CKG_GOPG1_MASK          (GOP_BIT12 | GOP_BIT11 | GOP_BIT10)

#define CKG_GOPG0_SCALING       (CKG_REG_BASE+0x88)
#define CKG_GOPG0_MG            (CKG_REG_BASE+0xFE)
#define CKG_GOPG0_MG_MASK       (GOP_BIT3 | GOP_BIT2)
#define CKG_GOPG2_MG_MASK       (GOP_BIT7 | GOP_BIT6)

/* GOP2 and GOPDWIN CLK */
#define GOP_GOP2CLK             (CKG_REG_BASE+(0x41<<1))
#define CKG_GOPG2_DISABLE_CLK   ~(GOP_BIT0)
#define CKG_GOPG2_ODCLK         (0<<2)
#define CKG_GOPG2_IDCLK2        (1 << 2)
#define CKG_GOPG2_IDCLK1        (2 << 2)
#define CKG_GOPG2_IDCLK3        (3 << 2)
#define CKG_GOPG2_DISABLE_CLK_MASK    (GOP_BIT0)
#define CKG_GOPG2_MASK          (GOP_BIT4 | GOP_BIT3 | GOP_BIT2)

#define CKG_GOPD_CLK_IDCLK2      (0 << 10)
#define CKG_GOPD_CLK_ODCLK       (1 << 10)
#define CKG_GOPD_CLK_DC0CLK      (2 << 10)
#define CKG_GOPD_CLK_SUBDC0CLK   (3 << 10)
#define CKG_GOPD_DISABLE_CLK_MASK (GOP_BIT8)
#define CKG_GOPD_MASK            (GOP_BIT12 | GOP_BIT11 | GOP_BIT10)


/* GOP3 CLK*/
#define GOP_GOP3CLK             (CKG_REG_BASE+(0x42<<1))
#define CKG_GOPG3_ODCLK         (0<<2)
#define CKG_GOPG3_IDCLK2        (1 << 2)
#define CKG_GOPG3_IDCLK1        (2 << 2)
#define CKG_GOPG3_OCC_FRCCLK    (3 << 2)
#define CKG_GOPG3_FCLK          (8 << 2)
#define CKG_GOPG3_DISABLE_CLK_MASK    (GOP_BIT0)
#define CKG_GOPG3_MASK          (GOP_BIT5 | GOP_BIT4 | GOP_BIT3 | GOP_BIT2)
#define CKG_GOPD_DISABLE_CLK   ~(GOP_BIT8)


/* GOP4 CLK*/
#define GOP_GOP4CLK             (CKG_REG_BASE+(0x7E<<1))
#define CKG_GOPG4_ODCLK         (0 << 10)
#define CKG_GOPG4_IDCLK2        (1 << 10)
#define CKG_GOPG4_IDCLK1        (2 << 10)
#define CKG_GOPG4_OCC_FRCCLK    (3 << 10)
#define CKG_GOPG4_FCLK          (8 << 10)
#define CKG_GOPG4_DISABLE_CLK_MASK    (GOP_BIT8)
#define CKG_GOPG4_MASK          (GOP_BIT13 |GOP_BIT12 | GOP_BIT11 | GOP_BIT10)


/* SRAM CLK */
#define GOP_SRAMCLK             (CKG_REG_BASE+(0x43<<1))
#define CKG_SRAM0_DISABLE_CLK   (GOP_BIT0)
#define CKG_SRAM1_DISABLE_CLK   (GOP_BIT8)
#define CKG_SRAM0_MASK          (GOP_BIT0|GOP_BIT1)
#define CKG_SRAM1_MASK          (GOP_BIT8|GOP_BIT9)

/* LINE BUFFER SRAM CLK */                   
#define GOP_LB_SRAMCLK            (CKG_REG_BASE+(0x45<<1))
#define CKG_LB_SRAM1_DISABLE_CLK   (GOP_BIT0)                   /*GOP1*/
#define CKG_LB_SRAM2_DISABLE_CLK   (GOP_BIT4)                   /*GOP2*/
#define CKG_LB_SRAM1_MASK          (GOP_BIT2|GOP_BIT3)
#define CKG_LB_SRAM2_MASK          (GOP_BIT6|GOP_BIT7)
//----------------------------------------------------------------------------
// MIU Reg
//----------------------------------------------------------------------------
#define GOP_CLIENT_REG          0x7B
#define GOP_MIU_GROUP           (MIU0_REG_BASE+(GOP_CLIENT_REG*2))
#define GOP_MIU_GROUP1          (MIU_REG_BASE+(GOP_CLIENT_REG*2))

/*Define each gop miu clint bit*/
#define GOP_MIU_CLIENT_DWIN     0x9
#define GOP_MIU_CLIENT_GOP0     0x5
#define GOP_MIU_CLIENT_GOP1     0x6
#define GOP_MIU_CLIENT_GOP2     0xf
#define GOP_MIU_CLIENT_GOP3     0xFF
#define GOP_MIU_CLIENT_GOP4     0xFF
#define GOP_MIU_CLIENT_GOP5     0xFF

//----------------------------------------------------------------------------
// VE Reg
//----------------------------------------------------------------------------
#define GOP_VE_TVS_OSD_EN           0x60
#define GOP_VE_TVS_OSD1_EN          0x61

//----------------------------------------------------------------------------
// GOP Reg
//----------------------------------------------------------------------------
#define GOP_REG(bk, reg)                     (GOP_REG_BASE+((MS_U32)(bk)<<16) + (reg) * 2)
#define __GOP_REG(reg)                       (GOP_REG_BASE+(reg) * 2)
#define GOP_REG_DIRECT_BASE                  (0x120200)
#define GOP_REG_GOP4_BK_OFFSET               0x1900
#define GOP_REG_GOP4_GW_OFFSET               0x1C00
#define GOP_REG_GOP4_ST_OFFSET               0x1D00

#define GOP_REG_VAL(x)                       (1<<x)

//MUX Setting
#define GOP_MUX_SHIFT                       0x2
#define GOP_REGMUX_MASK                     BMASK((GOP_MUX_SHIFT-1):0)
#define GOP_MUX0_MASK                       (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*0))
#define GOP_MUX1_MASK                       (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*1))
#define GOP_MUX2_MASK                       (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*2))
#define GOP_MUX3_MASK                       (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*3))
#define GOP_MUX4_MASK                       (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*4))

//IP and VOP MUX Setting
#define GOP_IP_MAIN_MUX_SHIFT                 0
#define GOP_IP_MAIN_MUX_MASK                 (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_IP_MAIN_MUX_SHIFT
#define GOP_IP_SUB_MUX_SHIFT                  3
#define GOP_IP_SUB_MUX_MASK                  (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_IP_SUB_MUX_SHIFT
#define GOP_IP_VOP0_MUX_SHIFT                 6
#define GOP_IP_VOP0_MUX_MASK                 (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_IP_VOP0_MUX_SHIFT
#define GOP_IP_VOP1_MUX_SHIFT                 9
#define GOP_IP_VOP1_MUX_MASK                 (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_IP_VOP1_MUX_SHIFT


//IP and VOP MUX Setting
#define GOP_Mix_MUX0_SHIFT                    0
#define GOP_Mix_MUX0_MASK                    (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_Mix_MUX0_SHIFT
#define GOP_Mix_MUX1_SHIFT                    3
#define GOP_Mix_MUX1_MASK                    (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_Mix_MUX1_SHIFT
#define GOP_VE0_MUX_SHIFT                     6
#define GOP_VE0_MUX_MASK                     (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_VE0_MUX_SHIFT
#define GOP_VE1_MUX_SHIFT                     9
#define GOP_VE1_MUX_MASK                     (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_VE1_MUX_SHIFT


//4k2k FRC MUX Setting
#define GOP_FRC_MUX_SHIFT                     0x3
#define GOP_FRC_REGMUX_MASK                   BMASK((GOP_MUX_SHIFT-1):0)
#define GOP_FRC_MUX0_MASK                     (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*0))
#define GOP_FRC_MUX1_MASK                     (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*1))
#define GOP_FRC_MUX2_MASK                     (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*2))
#define GOP_FRC_MUX3_MASK                     (GOP_REGMUX_MASK<<(GOP_MUX_SHIFT*3))

//DIP Setting
#define GOP_DIP_MUX_SHIFT                     12
#define GOP_DIP_MUX_MASK                     (BMASK((GOP_MUX_SHIFT-1):0))<<GOP_DIP_MUX_SHIFT

#define GOP_BANK_OFFSET                       0x3
#define GOP_4G_OFST                           0x0
#define GOP_2G_OFST                           (0x1*GOP_BANK_OFFSET)
#define GOP_1G_OFST                           (0x2*GOP_BANK_OFFSET)
#define GOP_1GX_OFST                          (0x3*GOP_BANK_OFFSET)
#define GOP_DW_OFST                           (0x4*GOP_BANK_OFFSET)
#define GOP_1GS0_OFST                         0xE
#define GOP_1GS1_OFST                         0x11

#define GOP_OFFSET_WR                       8
#define GOP_VAL_WR                          GOP_REG_VAL(GOP_OFFSET_WR)
#define GOP_OFFSET_FWR                      9
#define GOP_VAL_FWR                         GOP_REG_VAL(GOP_OFFSET_FWR)
#define GOP_OFFSET_FCLR                     11
#define GOP_VAL_FCL                         GOP_REG_VAL(GOP_OFFSET_FCLR)
#define GOP4G_OFFSET_WR_ACK                 12
#define GOP4G_VAL_WR_ACK                    GOP_REG_VAL(GOP4G_OFFSET_WR_ACK)
#define GOP2G_OFFSET_WR_ACK                 13
#define GOP2G_VAL_WR_ACK                    GOP_REG_VAL(GOP2G_OFFSET_WR_ACK)
#define GOPD_OFFSET_WR_ACK                  14
#define GOPD_VAL_WR_ACK                     GOP_REG_VAL(GOPD_OFFSET_WR_ACK)
#define GOP1G_OFFSET_WR_ACK                 15
#define GOP1G_VAL_WR_ACK                    GOP_REG_VAL(GOPD_OFFSET_WR_ACK)
#define GOP_VAL_ACK(x)                      GOP_REG_VAL(GOP4G_OFFSET_WR_ACK+x)

#define GOP_4G_CTRL0                        GOP_REG(GOP_4G_OFST, 0x00)
#define GOP_4G_CTRL1                        GOP_REG(GOP_4G_OFST, 0x01)
#define GOP_4G_RATE                         GOP_REG(GOP_4G_OFST, 0x02)
#define GOP_4G_PALDATA_L                    GOP_REG(GOP_4G_OFST, 0x03)
#define GOP_4G_PALDATA_H                    GOP_REG(GOP_4G_OFST, 0x04)
#define GOP_4G_PALCTRL                      GOP_REG(GOP_4G_OFST, 0x05)
#define GOP_4G_REGDMA_END                   GOP_REG(GOP_4G_OFST, 0x06)
#define GOP_4G_REGDMA_STR                   GOP_REG(GOP_4G_OFST, 0x07)
#define GOP_4G_INT                          GOP_REG(GOP_4G_OFST, 0x08)
#define GOP_4G_HWSTATE                      GOP_REG(GOP_4G_OFST, 0x09)
#define GOP_4G_SVM_HSTR                     GOP_REG(GOP_4G_OFST, 0x0a)
#define GOP_4G_SVM_HEND                     GOP_REG(GOP_4G_OFST, 0x0b)
#define GOP_4G_SVM_VSTR                     GOP_REG(GOP_4G_OFST, 0x0c)
#define GOP_4G_SVM_VEND                     GOP_REG(GOP_4G_OFST, 0x0d)
#define GOP_4G_RDMA_HT                      GOP_REG(GOP_4G_OFST, 0x0e)
#define GOP_4G_HS_PIPE                      GOP_REG(GOP_4G_OFST, 0x0f)
#define GOP_4G_SLOW                         GOP_REG(GOP_4G_OFST, 0x10)
#define GOP_4G_BRI                          GOP_REG(GOP_4G_OFST, 0x11)
#define GOP_4G_CON                          GOP_REG(GOP_4G_OFST, 0x12)
#define GOP_4G_BW                           GOP_REG(GOP_4G_OFST, 0x19)
#define GOP_4G_NEW_BW                       GOP_REG(GOP_4G_OFST, 0x1C)
#define GOP_4G_SRAM_BORROW                  GOP_REG(GOP_4G_OFST, 0x1D)
#define GOP_4G_3D_MIDDLE                    GOP_REG(GOP_4G_OFST, 0x1E)
#define GOP_4G_MIU_SEL                      GOP_REG(GOP_4G_OFST, 0x1F)
#define GOP_4G_PRI0                         GOP_REG(GOP_4G_OFST, 0x20)
#define GOP_4G_GWIN_DELTA                   GOP_REG(GOP_4G_OFST, 0x21)
#define GOP_4G_BOT_HS                       GOP_REG(GOP_4G_OFST, 0x23)
#define GOP_4G_TRSCLR_L                     GOP_REG(GOP_4G_OFST, 0x24)
#define GOP_4G_TRSCLR_H                     GOP_REG(GOP_4G_OFST, 0x25)
#define GOP_4G_YUV_SWAP                     GOP_REG(GOP_4G_OFST, 0x28)
#define GOP_4G_STRCH_HSZ                    GOP_REG(GOP_4G_OFST, 0x30)
#define GOP_4G_STRCH_VSZ                    GOP_REG(GOP_4G_OFST, 0x31)
#define GOP_4G_STRCH_HSTR                   GOP_REG(GOP_4G_OFST, 0x32)
#define GOP_4G_STRCH_VSTR                   GOP_REG(GOP_4G_OFST, 0x34)
#define GOP_4G_HSTRCH                       GOP_REG(GOP_4G_OFST, 0x35)
#define GOP_4G_VSTRCH                       GOP_REG(GOP_4G_OFST, 0x36)
#define GOP_4G_HSTRCH_INI                   GOP_REG(GOP_4G_OFST, 0x38)
#define GOP_4G_VSTRCH_INI                   GOP_REG(GOP_4G_OFST, 0x39)
#define GOP_4G_HVSTRCHMD                    GOP_REG(GOP_4G_OFST, 0x3a)
#define GOP_4G_OLDADDR                      GOP_REG(GOP_4G_OFST, 0x3b)
#define GOP_4G_MULTI_ALPHA                  GOP_REG(GOP_4G_OFST, 0x3c)
#define GOP_4G_HW_USAGE                     GOP_REG(GOP_4G_OFST, 0x40)
#define GOP_4G_BANK_FWR                     GOP_REG(GOP_4G_OFST, 0x50)
#define GOP_4G_BANK_HVAILDSIZE              GOP_REG(GOP_4G_OFST, 0x52)
#define GOP_4G_BANK_VVAILDSIZE              GOP_REG(GOP_4G_OFST, 0x53)
#define GOP_4G_SCALING_H_OUTPUTSIZE         GOP_REG(GOP_4G_OFST, 0x56)
#define GOP_4G_SCALING_HRATIO_L             GOP_REG(GOP_4G_OFST, 0x59)  //GOP scaling down ratio  dst / out * 2^20
#define GOP_4G_SCALING_HRATIO_H             GOP_REG(GOP_4G_OFST, 0x5A)
#define GOP_4G_SCALING_CFG                  GOP_REG(GOP_4G_OFST, 0x5B)
#define GOP_4G_SCALING_VRATIO_L             GOP_REG(GOP_4G_OFST, 0x5C)  //GOP scaling down ratio  dst / out * 2^20
#define GOP_4G_SCALING_VRATIO_H             GOP_REG(GOP_4G_OFST, 0x5D)


#define GOP_4G_RBLK0_VOFFL                  GOP_REG(GOP_4G_OFST, 0x60)
#define GOP_4G_RBLK0_VOFFH                  GOP_REG(GOP_4G_OFST, 0x61)
#define GOP_4G_RBLK1_VOFFL                  GOP_REG(GOP_4G_OFST, 0x62)
#define GOP_4G_RBLK1_VOFFH                  GOP_REG(GOP_4G_OFST, 0x63)
#define GOP_4G_RBLK2_VOFFL                  GOP_REG(GOP_4G_OFST, 0x64)
#define GOP_4G_RBLK2_VOFFH                  GOP_REG(GOP_4G_OFST, 0x65)
#define GOP_4G_RBLK3_VOFFL                  GOP_REG(GOP_4G_OFST, 0x66)
#define GOP_4G_RBLK3_VOFFH                  GOP_REG(GOP_4G_OFST, 0x67)
#define GOP_4G_RBLK0_HOFF                   GOP_REG(GOP_4G_OFST, 0x70)
#define GOP_4G_RBLK1_HOFF                   GOP_REG(GOP_4G_OFST, 0x71)
#define GOP_4G_RBLK2_HOFF                   GOP_REG(GOP_4G_OFST, 0x72)
#define GOP_4G_RBLK3_HOFF                   GOP_REG(GOP_4G_OFST, 0x73)
#define GOP_4G_REGDMA_EN                    GOP_REG(GOP_4G_OFST, 0x78)
#define GOP_MUX_IPVOP                       __GOP_REG(0x77)
#define GOP_MUX4_MIX_VE                     __GOP_REG(0x7B)
#define GOP_BAK_SEL_EX                      __GOP_REG(0x7C)
#define GOP_MUX_4K2K                        __GOP_REG(0x7D)
#define GOP_MUX                             __GOP_REG(0x7e)
#define GOP_BAK_SEL                         __GOP_REG(0x7f)

#define GOP_4G_GWIN0_CTRL(id)               GOP_REG(GOP_4G_OFST+1, 0x00 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_L(id)              GOP_REG(GOP_4G_OFST+1, 0x01 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_H(id)              GOP_REG(GOP_4G_OFST+1, 0x02 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DEL_PIXEL(id)                GOP_REG(GOP_4G_OFST+1, 0x03 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_4G_HSTR(id)                     GOP_REG(GOP_4G_OFST+1, 0x04 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_HEND(id)                     GOP_REG(GOP_4G_OFST+1, 0x05 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_VSTR(id)                     GOP_REG(GOP_4G_OFST+1, 0x06 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_GWIN_MIDDLE(id)              GOP_REG(GOP_4G_OFST+1, 0x07 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_VEND(id)                     GOP_REG(GOP_4G_OFST+1, 0x08 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_HSIZE(id)          GOP_REG(GOP_4G_OFST+1, 0x09 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_GWIN_ALPHA01(id)             GOP_REG(GOP_4G_OFST+1, 0x0A + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_VSTR_L(id)              GOP_REG(GOP_4G_OFST+1, 0x0C + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_VSTR_H(id)              GOP_REG(GOP_4G_OFST+1, 0x0D + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HSTR(id)                GOP_REG(GOP_4G_OFST+1, 0x0E + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_SIZE_L(id)         GOP_REG(GOP_4G_OFST+1, 0x10 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RBLK_SIZE_H(id)         GOP_REG(GOP_4G_OFST+1, 0x11 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RLEN_L(id)              GOP_REG(GOP_4G_OFST+1, 0x12 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_RLEN_H(id)              GOP_REG(GOP_4G_OFST+1, 0x13 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HVSTOP_L(id)            GOP_REG(GOP_4G_OFST+1, 0x14 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_HVSTOP_H(id)            GOP_REG(GOP_4G_OFST+1, 0x15 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_DRAM_FADE(id)                GOP_REG(GOP_4G_OFST+1, 0x16 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_BG_CLR(id)                   GOP_REG(GOP_4G_OFST+1, 0x18 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_BG_HSTR(id)                  GOP_REG(GOP_4G_OFST+1, 0x19 + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_BG_HEND(id)                  GOP_REG(GOP_4G_OFST+1, 0x1a + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_BG_VSTR(id)                  GOP_REG(GOP_4G_OFST+1, 0x1C + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_BG_VEND(id)                  GOP_REG(GOP_4G_OFST+1, 0x1D + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_3DOSD_SUB_RBLK_L(id)         GOP_REG(GOP_4G_OFST+1, 0x1E + (0x20*((id)%MAX_GOP0_GWIN)))
#define GOP_4G_3DOSD_SUB_RBLK_H(id)         GOP_REG(GOP_4G_OFST+1, 0x1F + (0x20*((id)%MAX_GOP0_GWIN)))


#define GOP_2G_CTRL0                        GOP_REG(GOP_2G_OFST, 0x00)
#define GOP_2G_CTRL1                        GOP_REG(GOP_2G_OFST, 0x01)
#define GOP_2G_RATE                         GOP_REG(GOP_2G_OFST, 0x02)
#define GOP_2G_PALDATA_L                    GOP_REG(GOP_2G_OFST, 0x03)
#define GOP_2G_PALDATA_H                    GOP_REG(GOP_2G_OFST, 0x04)
#define GOP_2G_PALCTRL                      GOP_REG(GOP_2G_OFST, 0x05)
#define GOP_2G_REGDMA_END                   GOP_REG(GOP_2G_OFST, 0x06)
#define GOP_2G_REGDMA_STR                   GOP_REG(GOP_2G_OFST, 0x07)
#define GOP_2G_INT                          GOP_REG(GOP_2G_OFST, 0x08)
#define GOP_2G_HWSTATE                      GOP_REG(GOP_2G_OFST, 0x09)
#define GOP_2G_RDMA_HT                      GOP_REG(GOP_2G_OFST, 0x0e)
#define GOP_2G_HS_PIPE                      GOP_REG(GOP_2G_OFST, 0x0f)
#define GOP_2G_SLOW                         GOP_REG(GOP_2G_OFST, 0x10)
#define GOP_2G_BRI                          GOP_REG(GOP_2G_OFST, 0x11)
#define GOP_2G_CON                          GOP_REG(GOP_2G_OFST, 0x12)
#define GOP_2G_BW                           GOP_REG(GOP_2G_OFST, 0x19)
#define GOP_2G_3D_MIDDLE                    GOP_REG(GOP_2G_OFST, 0x1E)
#define GOP_2G_PRI0                         GOP_REG(GOP_2G_OFST, 0x20)
#define GOP_2G_GWIN_DELTA                   GOP_REG(GOP_2G_OFST, 0x21)
#define GOP_2G_TRSCLR_L                     GOP_REG(GOP_2G_OFST, 0x24)
#define GOP_2G_TRSCLR_H                     GOP_REG(GOP_2G_OFST, 0x25)
#define GOP_2G_STRCH_HSZ                    GOP_REG(GOP_2G_OFST, 0x30)
#define GOP_2G_STRCH_VSZ                    GOP_REG(GOP_2G_OFST, 0x31)
#define GOP_2G_STRCH_HSTR                   GOP_REG(GOP_2G_OFST, 0x32)
#define GOP_2G_STRCH_VSTR                   GOP_REG(GOP_2G_OFST, 0x34)
#define GOP_2G_HSTRCH                       GOP_REG(GOP_2G_OFST, 0x35)
#define GOP_2G_VSTRCH                       GOP_REG(GOP_2G_OFST, 0x36)
#define GOP_2G_HSTRCH_INI                   GOP_REG(GOP_2G_OFST, 0x38)
#define GOP_2G_VSTRCH_INI                   GOP_REG(GOP_2G_OFST, 0x39)
#define GOP_2G_HVStrch_MD                   GOP_REG(GOP_2G_OFST, 0x3a)
#define GOP_2G_OLDADDR                      GOP_REG(GOP_2G_OFST, 0x3b)
#define GOP_2G_MULTI_ALPHA                  GOP_REG(GOP_2G_OFST, 0x3c)
#define GOP_2G_REGDMA_EN                    GOP_REG(GOP_2G_OFST, 0x78)


#define GOP_2G_GWIN0_CTRL(id)               GOP_REG(GOP_2G_OFST+1, 0x00 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_GWIN_CTRL(id)                GOP_REG(GOP_2G_OFST+1, 0x00 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_RBLK_L(id)              GOP_REG(GOP_2G_OFST+1, 0x01 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_RBLK_H(id)              GOP_REG(GOP_2G_OFST+1, 0x02 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DEL_PIXEL(id)                GOP_REG(GOP_2G_OFST+1, 0x03 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_HSTR(id)                     GOP_REG(GOP_2G_OFST+1, 0x04 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_HEND(id)                     GOP_REG(GOP_2G_OFST+1, 0x05 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_VSTR(id)                     GOP_REG(GOP_2G_OFST+1, 0x06 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_VEND(id)                     GOP_REG(GOP_2G_OFST+1, 0x08 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_RBLK_HSIZE(id)          GOP_REG(GOP_2G_OFST+1, 0x09 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_GWIN_ALPHA01(id)             GOP_REG(GOP_2G_OFST+1, 0x0A + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_VSTR_L(id)              GOP_REG(GOP_2G_OFST+1, 0x0C + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_VSTR_H(id)              GOP_REG(GOP_2G_OFST+1, 0x0D + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_DRAM_FADE(id)                GOP_REG(GOP_2G_OFST+1, 0x16 + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_3DOSD_SUB_RBLK_L(id)         GOP_REG(GOP_2G_OFST+1, 0x1E + (0x20*((id)%MAX_GOP1_GWIN)))
#define GOP_2G_3DOSD_SUB_RBLK_H(id)         GOP_REG(GOP_2G_OFST+1, 0x1F + (0x20*((id)%MAX_GOP1_GWIN)))

// DWIN reg
#define GOP_DW_CTL0_EN                          GOP_REG(GOP_DW_OFST, 0x00)
#define GOP_DWIN_EN                             (0x00)
#define GOP_DWIN_EN_VAL                         GOP_REG_VAL(GOP_DWIN_EN)
#define GOP_DWIN_SHOT                           (0x07)
#define GOP_DWIN_SHOT_VAL                       GOP_REG_VAL(GOP_DWIN_SHOT)

#define GOP_DW_LSTR_WBE                         GOP_REG(GOP_DW_OFST, 0x01)
#define GOP_DW_INT_MASK                         GOP_REG(GOP_DW_OFST, 0x02)
#define GOP_DW_DEBUG                            GOP_REG(GOP_DW_OFST, 0x03)
#define GOP_DW_ALPHA                            GOP_REG(GOP_DW_OFST, 0x04)
#define GOP_DW_BW                               GOP_REG(GOP_DW_OFST, 0x05)
#define GOP_DW_VSTR                             GOP_REG(GOP_DW_OFST, 0x10)
#define GOP_DW_HSTR                             GOP_REG(GOP_DW_OFST, 0x11)
#define GOP_DW_VEND                             GOP_REG(GOP_DW_OFST, 0x12)
#define GOP_DW_HEND                             GOP_REG(GOP_DW_OFST, 0x13)
#define GOP_DW_HSIZE                            GOP_REG(GOP_DW_OFST, 0x14)
#define GOP_DW_JMPLEN                           GOP_REG(GOP_DW_OFST, 0x15)
#define GOP_DW_DSTR_L                           GOP_REG(GOP_DW_OFST, 0x16)
#define GOP_DW_DSTR_H                           GOP_REG(GOP_DW_OFST, 0x17)
#define GOP_DW_UB_L                             GOP_REG(GOP_DW_OFST, 0x18)
#define GOP_DW_UB_H                             GOP_REG(GOP_DW_OFST, 0x19)

#define GOP_DW_PON_DSTR_L                       GOP_REG(GOP_DW_OFST, 0x1a)
#define GOP_DW_PON_DSTR_H                       GOP_REG(GOP_DW_OFST, 0x1b)
#define GOP_DW_PON_UB_L                         GOP_REG(GOP_DW_OFST, 0x1c)
#define GOP_DW_PON_UB_H                         GOP_REG(GOP_DW_OFST, 0x1d)
#define GOP_DW_FRAME_OFT_L                      GOP_REG(GOP_DW_OFST, 0x28)
#define GOP_DW_FRAME_CTRL                       GOP_REG(GOP_DW_OFST, 0x30)

#define GOP_1G_CTRL0                        GOP_REG(GOP_1G_OFST, 0x00)
#define GOP_1G_CTRL1                        GOP_REG(GOP_1G_OFST, 0x01)
#define GOP_1G_RATE                         GOP_REG(GOP_1G_OFST, 0x02)
#define GOP_1G_PALDATA_L                    GOP_REG(GOP_1G_OFST, 0x03)
#define GOP_1G_PALDATA_H                    GOP_REG(GOP_1G_OFST, 0x04)
#define GOP_1G_PALCTRL                      GOP_REG(GOP_1G_OFST, 0x05)
#define GOP_1G_REGDMA_END                   GOP_REG(GOP_1G_OFST, 0x06)
#define GOP_1G_REGDMA_STR                   GOP_REG(GOP_1G_OFST, 0x07)
#define GOP_1G_INT                          GOP_REG(GOP_1G_OFST, 0x08)
#define GOP_1G_HWSTATE                      GOP_REG(GOP_1G_OFST, 0x09)
#define GOP_1G_RDMA_HT                      GOP_REG(GOP_1G_OFST, 0x0e)
#define GOP_1G_HS_PIPE                      GOP_REG(GOP_1G_OFST, 0x0f)
#define GOP_1G_BRI                          GOP_REG(GOP_1G_OFST, 0x11)
#define GOP_1G_CON                          GOP_REG(GOP_1G_OFST, 0x12)
#define GOP_1G_BW                           GOP_REG(GOP_1G_OFST, 0x19)
#define GOP_1G_3D_MIDDLE                    GOP_REG(GOP_1G_OFST, 0x1E)
#define GOP_1G_TRSCLR_L                     GOP_REG(GOP_1G_OFST, 0x24)
#define GOP_1G_TRSCLR_H                     GOP_REG(GOP_1G_OFST, 0x25)
#define GOP_1G_STRCH_HSZ                    GOP_REG(GOP_1G_OFST, 0x30)
#define GOP_1G_STRCH_VSZ                    GOP_REG(GOP_1G_OFST, 0x31)
#define GOP_1G_STRCH_HSTR                   GOP_REG(GOP_1G_OFST, 0x32)
#define GOP_1G_STRCH_VSTR                   GOP_REG(GOP_1G_OFST, 0x34)
#define GOP_1G_HSTRCH                       GOP_REG(GOP_1G_OFST, 0x35)
#define GOP_1G_HSTRCH_INI                   GOP_REG(GOP_1G_OFST, 0x38)
#define GOP_1G_VSTRCH_INI                   GOP_REG(GOP_1G_OFST, 0x39)
#define GOP_1G_HStrch_MD                    GOP_REG(GOP_1G_OFST, 0x3a)
#define GOP_1G_OLDADDR                      GOP_REG(GOP_1G_OFST, 0x3b)
#define GOP_1G_MULTI_ALPHA                  GOP_REG(GOP_1G_OFST, 0x3c)

#define GOP_1G_GWIN0_CTRL                   GOP_REG(GOP_1G_OFST+1, 0x0)
#define GOP_1G_DRAM_RBLK_L                  GOP_REG(GOP_1G_OFST+1, 0x1)
#define GOP_1G_DRAM_RBLK_H                  GOP_REG(GOP_1G_OFST+1, 0x2)
#define GOP_1G_DEL_PIXEL                    GOP_REG(GOP_1G_OFST+1, 0x3)
#define GOP_1G_HSTR                         GOP_REG(GOP_1G_OFST+1, 0x4)
#define GOP_1G_HEND                         GOP_REG(GOP_1G_OFST+1, 0x5)
#define GOP_1G_VSTR                         GOP_REG(GOP_1G_OFST+1, 0x6)
#define GOP_1G_VEND                         GOP_REG(GOP_1G_OFST+1, 0x8)
#define GOP_1G_DRAM_RBLK_HSIZE              GOP_REG(GOP_1G_OFST+1, 0x9)
#define GOP_1G_GWIN_ALPHA01                 GOP_REG(GOP_1G_OFST+1, 0xA)
#define GOP_1G_DRAM_VSTR_L                  GOP_REG(GOP_1G_OFST+1, 0x0C)
#define GOP_1G_DRAM_VSTR_H                  GOP_REG(GOP_1G_OFST+1, 0x0D)
#define GOP_1G_DRAM_FADE                    GOP_REG(GOP_1G_OFST+1, 0x16)
#define GOP_1G_3DOSD_SUB_RBLK_L             GOP_REG(GOP_1G_OFST+1, 0x1E)
#define GOP_1G_3DOSD_SUB_RBLK_H             GOP_REG(GOP_1G_OFST+1, 0x1F)

#define GOP_1GX_CTRL0                        GOP_REG(GOP_1GX_OFST, 0x00)
#define GOP_1GX_CTRL1                        GOP_REG(GOP_1GX_OFST, 0x01)
#define GOP_1GX_RATE                         GOP_REG(GOP_1GX_OFST, 0x02)
#define GOP_1GX_PALDATA_L                    GOP_REG(GOP_1GX_OFST, 0x03)
#define GOP_1GX_PALDATA_H                    GOP_REG(GOP_1GX_OFST, 0x04)
#define GOP_1GX_PALCTRL                      GOP_REG(GOP_1GX_OFST, 0x05)
#define GOP_1GX_REGDMA_END                   GOP_REG(GOP_1GX_OFST, 0x06)
#define GOP_1GX_REGDMA_STR                   GOP_REG(GOP_1GX_OFST, 0x07)
#define GOP_1GX_INT                          GOP_REG(GOP_1GX_OFST, 0x08)
#define GOP_1GX_HWSTATE                      GOP_REG(GOP_1GX_OFST, 0x09)
#define GOP_1GX_RDMA_HT                      GOP_REG(GOP_1GX_OFST, 0x0e)
#define GOP_1GX_HS_PIPE                      GOP_REG(GOP_1GX_OFST, 0x0f)
#define GOP_1GX_BRI                          GOP_REG(GOP_1GX_OFST, 0x11)
#define GOP_1GX_CON                          GOP_REG(GOP_1GX_OFST, 0x12)
#define GOP_1GX_BW                           GOP_REG(GOP_1GX_OFST, 0x19)
#define GOP_1GX_3D_MIDDLE                    GOP_REG(GOP_1GX_OFST, 0x1E)
#define GOP_1GX_TRSCLR_L                     GOP_REG(GOP_1GX_OFST, 0x24)
#define GOP_1GX_TRSCLR_H                     GOP_REG(GOP_1GX_OFST, 0x25)
#define GOP_1GX_STRCH_HSZ                    GOP_REG(GOP_1GX_OFST, 0x30)
#define GOP_1GX_STRCH_VSZ                    GOP_REG(GOP_1GX_OFST, 0x31)
#define GOP_1GX_STRCH_HSTR                   GOP_REG(GOP_1GX_OFST, 0x32)
#define GOP_1GX_STRCH_VSTR                   GOP_REG(GOP_1GX_OFST, 0x34)
#define GOP_1GX_HSTRCH                       GOP_REG(GOP_1GX_OFST, 0x35)
#define GOP_1GX_HSTRCH_INI                   GOP_REG(GOP_1GX_OFST, 0x38)
#define GOP_1GX_VSTRCH_INI                   GOP_REG(GOP_1GX_OFST, 0x39)
#define GOP_1GX_HStrch_MD                    GOP_REG(GOP_1GX_OFST, 0x3a)
#define GOP_1GX_OLDADDR                      GOP_REG(GOP_1GX_OFST, 0x3b)
#define GOP_1GX_MULTI_ALPHA                  GOP_REG(GOP_1GX_OFST, 0x3c)

#define GOP_1GX_GWIN0_CTRL                   GOP_REG(GOP_1GX_OFST+1, 0x00)
#define GOP_1GX_DRAM_RBLK_L                  GOP_REG(GOP_1GX_OFST+1, 0x01)
#define GOP_1GX_DRAM_RBLK_H                  GOP_REG(GOP_1GX_OFST+1, 0x02)
#define GOP_1GX_DEL_PIXEL                    GOP_REG(GOP_1GX_OFST+1, 0x03)
#define GOP_1GX_HSTR                         GOP_REG(GOP_1GX_OFST+1, 0x04)
#define GOP_1GX_HEND                         GOP_REG(GOP_1GX_OFST+1, 0x05)
#define GOP_1GX_VSTR                         GOP_REG(GOP_1GX_OFST+1, 0x06)
#define GOP_1GX_VEND                         GOP_REG(GOP_1GX_OFST+1, 0x08)
#define GOP_1GX_DRAM_RBLK_HSIZE              GOP_REG(GOP_1GX_OFST+1, 0x09)
#define GOP_1GX_GWIN_ALPHA01                 GOP_REG(GOP_1GX_OFST+1, 0x0A)
#define GOP_1GX_DRAM_VSTR_L                  GOP_REG(GOP_1GX_OFST+1, 0x0C)
#define GOP_1GX_DRAM_VSTR_H                  GOP_REG(GOP_1GX_OFST+1, 0x0D)
#define GOP_1GX_DRAM_FADE                    GOP_REG(GOP_1GX_OFST+1, 0x16)
#define GOP_1GX_3DOSD_SUB_RBLK_L             GOP_REG(GOP_1GX_OFST+1, 0x1E)
#define GOP_1GX_3DOSD_SUB_RBLK_H             GOP_REG(GOP_1GX_OFST+1, 0x1F)

#define GOP_1GS0_CTRL0                        GOP_REG(GOP_1GS0_OFST, 0x00)
#define GOP_1GS0_CTRL1                        GOP_REG(GOP_1GS0_OFST, 0x01)
#define GOP_1GS0_RATE                         GOP_REG(GOP_1GS0_OFST, 0x02)
#define GOP_1GS0_PALDATA_L                    GOP_REG(GOP_1GS0_OFST, 0x03)
#define GOP_1GS0_PALDATA_H                    GOP_REG(GOP_1GS0_OFST, 0x04)
#define GOP_1GS0_PALCTRL                      GOP_REG(GOP_1GS0_OFST, 0x05)
#define GOP_1GS0_REGDMA_END                   GOP_REG(GOP_1GS0_OFST, 0x06)
#define GOP_1GS0_REGDMA_STR                   GOP_REG(GOP_1GS0_OFST, 0x07)
#define GOP_1GS0_INT                          GOP_REG(GOP_1GS0_OFST, 0x08)
#define GOP_1GS0_HWSTATE                      GOP_REG(GOP_1GS0_OFST, 0x09)
#define GOP_1GS0_RDMA_HT                      GOP_REG(GOP_1GS0_OFST, 0x0e)
#define GOP_1GS0_HS_PIPE                      GOP_REG(GOP_1GS0_OFST, 0x0f)
#define GOP_1GS0_BRI                          GOP_REG(GOP_1GS0_OFST, 0x11)
#define GOP_1GS0_CON                          GOP_REG(GOP_1GS0_OFST, 0x12)
#define GOP_1GS0_BW                           GOP_REG(GOP_1GS0_OFST, 0x19)
#define GOP_1GS0_TRSCLR_L                     GOP_REG(GOP_1GS0_OFST, 0x24)
#define GOP_1GS0_TRSCLR_H                     GOP_REG(GOP_1GS0_OFST, 0x25)
#define GOP_1GS0_STRCH_HSZ                    GOP_REG(GOP_1GS0_OFST, 0x30)
#define GOP_1GS0_STRCH_VSZ                    GOP_REG(GOP_1GS0_OFST, 0x31)
#define GOP_1GS0_STRCH_HSTR                   GOP_REG(GOP_1GS0_OFST, 0x32)
#define GOP_1GS0_STRCH_VSTR                   GOP_REG(GOP_1GS0_OFST, 0x34)
#define GOP_1GS0_HSTRCH                       GOP_REG(GOP_1GS0_OFST, 0x35)
#define GOP_1GS0_HSTRCH_INI                   GOP_REG(GOP_1GS0_OFST, 0x38)
#define GOP_1GS0_VSTRCH_INI                   GOP_REG(GOP_1GS0_OFST, 0x39)
#define GOP_1GS0_HVStrch_MD                    GOP_REG(GOP_1GS0_OFST, 0x3a)
#define GOP_1GS0_OLDADDR                      GOP_REG(GOP_1GS0_OFST, 0x3b)
#define GOP_1GS0_MULTI_ALPHA                  GOP_REG(GOP_1GS0_OFST, 0x3c)

#define GOP_1GS0_GWIN0_CTRL                   GOP_REG(GOP_1GS0_OFST+1, 0x00)
#define GOP_1GS0_DRAM_RBLK_L                  GOP_REG(GOP_1GS0_OFST+1, 0x01)
#define GOP_1GS0_DRAM_RBLK_H                  GOP_REG(GOP_1GS0_OFST+1, 0x02)
#define GOP_1GS0_DEL_PIXEL                    GOP_REG(GOP_1GS0_OFST+1, 0x03)
#define GOP_1GS0_HSTR                         GOP_REG(GOP_1GS0_OFST+1, 0x04)
#define GOP_1GS0_HEND                         GOP_REG(GOP_1GS0_OFST+1, 0x05)
#define GOP_1GS0_VSTR                         GOP_REG(GOP_1GS0_OFST+1, 0x06)
#define GOP_1GS0_VEND                         GOP_REG(GOP_1GS0_OFST+1, 0x08)
#define GOP_1GS0_DRAM_RBLK_HSIZE              GOP_REG(GOP_1GS0_OFST+1, 0x09)
#define GOP_1GS0_GWIN_ALPHA01                 GOP_REG(GOP_1GS0_OFST+1, 0x0A)
#define GOP_1GS0_DRAM_VSTR_L                  GOP_REG(GOP_1GS0_OFST+1, 0x0C)
#define GOP_1GS0_DRAM_VSTR_H                  GOP_REG(GOP_1GS0_OFST+1, 0x0D)
#define GOP_1GS0_DRAM_FADE                    GOP_REG(GOP_1GS0_OFST+1, 0x16)
#define GOP_1GS0_3DOSD_SUB_RBLK_L             GOP_REG(GOP_1GS0_OFST+1, 0x1E)
#define GOP_1GS0_3DOSD_SUB_RBLK_H             GOP_REG(GOP_1GS0_OFST+1, 0x1F)

#define GOP_1GS1_CTRL0                        GOP_REG(GOP_1GS1_OFST, 0x00)
#define GOP_1GS1_CTRL1                        GOP_REG(GOP_1GS1_OFST, 0x01)
#define GOP_1GS1_RATE                         GOP_REG(GOP_1GS1_OFST, 0x02)
#define GOP_1GS1_PALDATA_L                    GOP_REG(GOP_1GS1_OFST, 0x03)
#define GOP_1GS1_PALDATA_H                    GOP_REG(GOP_1GS1_OFST, 0x04)
#define GOP_1GS1_PALCTRL                      GOP_REG(GOP_1GS1_OFST, 0x05)
#define GOP_1GS1_REGDMA_END                   GOP_REG(GOP_1GS1_OFST, 0x06)
#define GOP_1GS1_REGDMA_STR                   GOP_REG(GOP_1GS1_OFST, 0x07)
#define GOP_1GS1_INT                          GOP_REG(GOP_1GS1_OFST, 0x08)
#define GOP_1GS1_HWSTATE                      GOP_REG(GOP_1GS1_OFST, 0x09)
#define GOP_1GS1_RDMA_HT                      GOP_REG(GOP_1GS1_OFST, 0x0e)
#define GOP_1GS1_HS_PIPE                      GOP_REG(GOP_1GS1_OFST, 0x0f)
#define GOP_1GS1_BRI                          GOP_REG(GOP_1GS1_OFST, 0x11)
#define GOP_1GS1_CON                          GOP_REG(GOP_1GS1_OFST, 0x12)
#define GOP_1GS1_BW                           GOP_REG(GOP_1GS1_OFST, 0x19)
#define GOP_1GS1_TRSCLR_L                     GOP_REG(GOP_1GS1_OFST, 0x24)
#define GOP_1GS1_TRSCLR_H                     GOP_REG(GOP_1GS1_OFST, 0x25)
#define GOP_1GS1_STRCH_HSZ                    GOP_REG(GOP_1GS1_OFST, 0x30)
#define GOP_1GS1_STRCH_VSZ                    GOP_REG(GOP_1GS1_OFST, 0x31)
#define GOP_1GS1_STRCH_HSTR                   GOP_REG(GOP_1GS1_OFST, 0x32)
#define GOP_1GS1_STRCH_VSTR                   GOP_REG(GOP_1GS1_OFST, 0x34)
#define GOP_1GS1_HSTRCH                       GOP_REG(GOP_1GS1_OFST, 0x35)
#define GOP_1GS1_HSTRCH_INI                   GOP_REG(GOP_1GS1_OFST, 0x38)
#define GOP_1GS1_VSTRCH_INI                   GOP_REG(GOP_1GS1_OFST, 0x39)
#define GOP_1GS1_HVStrch_MD                    GOP_REG(GOP_1GS1_OFST, 0x3a)
#define GOP_1GS1_OLDADDR                      GOP_REG(GOP_1GS1_OFST, 0x3b)
#define GOP_1GS1_MULTI_ALPHA                  GOP_REG(GOP_1GS1_OFST, 0x3c)

#define GOP_1GS1_GWIN0_CTRL                   GOP_REG(GOP_1GS1_OFST+1, 0x00)
#define GOP_1GS1_DRAM_RBLK_L                  GOP_REG(GOP_1GS1_OFST+1, 0x01)
#define GOP_1GS1_DRAM_RBLK_H                  GOP_REG(GOP_1GS1_OFST+1, 0x02)
#define GOP_1GS1_DEL_PIXEL                    GOP_REG(GOP_1GS1_OFST+1, 0x03)
#define GOP_1GS1_HSTR                         GOP_REG(GOP_1GS1_OFST+1, 0x04)
#define GOP_1GS1_HEND                         GOP_REG(GOP_1GS1_OFST+1, 0x05)
#define GOP_1GS1_VSTR                         GOP_REG(GOP_1GS1_OFST+1, 0x06)
#define GOP_1GS1_VEND                         GOP_REG(GOP_1GS1_OFST+1, 0x08)
#define GOP_1GS1_DRAM_RBLK_HSIZE              GOP_REG(GOP_1GS1_OFST+1, 0x09)
#define GOP_1GS1_GWIN_ALPHA01                 GOP_REG(GOP_1GS1_OFST+1, 0x0A)
#define GOP_1GS1_DRAM_VSTR_L                  GOP_REG(GOP_1GS1_OFST+1, 0x0C)
#define GOP_1GS1_DRAM_VSTR_H                  GOP_REG(GOP_1GS1_OFST+1, 0x0D)
#define GOP_1GS1_DRAM_FADE                    GOP_REG(GOP_1GS1_OFST+1, 0x16)
#define GOP_1GS1_3DOSD_SUB_RBLK_L             GOP_REG(GOP_1GS1_OFST+1, 0x1E)
#define GOP_1GS1_3DOSD_SUB_RBLK_H             GOP_REG(GOP_1GS1_OFST+1, 0x1F)
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------
// GOP Test Pattern Reg
//----------------------------------------------------------------------------
#define REG_TSTCLR_EN                       GOP_REG(GOP_4G_OFST, 0x00)
#define REG_TSTCLR_ALPHA_EN                 GOP_REG(GOP_4G_OFST+2, 0x00)
#define REG_TLB_TAG_ADDR_L                  GOP_REG(GOP_4G_OFST+2, 0x2C)
#define REG_TLB_TAG_ADDR_H                  GOP_REG(GOP_4G_OFST+2, 0x2D)
#define REG_TLB_TAG_ADDR_RVIEW_L            GOP_REG(GOP_4G_OFST+2, 0x2E)
#define REG_TLB_TAG_ADDR_RVIEW_H            GOP_REG(GOP_4G_OFST+2, 0x2F)
#define REG_TSTCLR_ALPHA                    GOP_REG(GOP_4G_OFST+2, 0x40)
#define REG_R_STC                           GOP_REG(GOP_4G_OFST+2, 0x41)
#define REG_G_STC                           GOP_REG(GOP_4G_OFST+2, 0x48)
#define REG_B_STC                           GOP_REG(GOP_4G_OFST+2, 0x49)
#define REG_TSTCLR_HDUP                     GOP_REG(GOP_4G_OFST+2, 0x01)
#define REG_TSTCLR_VDUP                     GOP_REG(GOP_4G_OFST+2, 0x01)
#define REG_HR_INC                          GOP_REG(GOP_4G_OFST+2, 0x42)
#define REG_HR_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x42)
#define REG_HG_INC                          GOP_REG(GOP_4G_OFST+2, 0x43)
#define REG_HG_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x43)
#define REG_HB_INC                          GOP_REG(GOP_4G_OFST+2, 0x44)
#define REG_HB_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x44)
#define REG_HR_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4A)
#define REG_HG_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4B)
#define REG_HB_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4C)
#define REG_VR_INC                          GOP_REG(GOP_4G_OFST+2, 0x45)
#define REG_VR_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x45)
#define REG_VG_INC                          GOP_REG(GOP_4G_OFST+2, 0x46)
#define REG_VG_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x46)
#define REG_VB_INC                          GOP_REG(GOP_4G_OFST+2, 0x47)
#define REG_VB_INC_SIGNZ                    GOP_REG(GOP_4G_OFST+2, 0x47)
#define REG_VR_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4D)
#define REG_VG_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4E)
#define REG_VB_STEP                         GOP_REG(GOP_4G_OFST+2, 0x4F)
#define REG_TLB_BASE_ADDR_L                 GOP_REG(GOP_4G_OFST+2, 0x58)
#define REG_TLB_BASE_ADDR_H                 GOP_REG(GOP_4G_OFST+2, 0x59)
#define REG_TLB_BASE_ADDR_RVIEW_L           GOP_REG(GOP_4G_OFST+2, 0x5A)
#define REG_TLB_BASE_ADDR_RVIEW_H           GOP_REG(GOP_4G_OFST+2, 0x5B)

#define MASK_TSTCLR_EN                      GOP_BIT6
#define MASK_TSTCLR_ALPHA_EN                GOP_BIT1
#define MASK_TSTCLR_ALPHA                   BMASK(11:8)|BMASK(3:0)
#define MASK_RGB_STC_VALID                  BMASK(7:0)
#define MASK_R_STC                          BMASK(11:8)|BMASK(3:0)
#define MASK_G_STC                          BMASK(11:8)|BMASK(3:0)
#define MASK_B_STC                          BMASK(11:8)|BMASK(3:0)
#define MASK_INI_TSTCLR_EN                  GOP_BIT0
#define MASK_TSTCLR_HDUP                    BMASK(3:2)
#define MASK_TSTCLR_VDUP                    BMASK(1:0)
#define MASK_HR_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_HR_INC_SIGNZ                   GOP_BIT11
#define MASK_HG_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_HG_INC_SIGNZ                   GOP_BIT11
#define MASK_HB_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_HB_INC_SIGNZ                   GOP_BIT11
#define MASK_HR_STEP                        BMASK(11:8)|BMASK(3:0)
#define MASK_HG_STEP                        BMASK(11:8)|BMASK(3:0)
#define MASK_HB_STEP                        BMASK(11:8)|BMASK(3:0)
#define MASK_VR_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_VR_INC_SIGNZ                   GOP_BIT11
#define MASK_VG_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_VG_INC_SIGNZ                   GOP_BIT11
#define MASK_VB_INC                         BMASK(10:8)|BMASK(3:0)
#define MASK_VB_INC_SIGNZ                   GOP_BIT11
#define MASK_VR_STEP                        BMASK(11:8)|BMASK(3:0)
#define MASK_VG_STEP                        BMASK(11:8)|BMASK(3:0)
#define MASK_VB_STEP                        BMASK(11:8)|BMASK(3:0)

#define SHIFT_TSTCLR_EN                     6
#define SHIFT_TSTCLR_ALPHA_EN               1
#define SHIFT_TSTCLR_ALPHA                  8
#define SHIFT_R_STC                         0
#define SHIFT_G_STC                         0
#define SHIFT_B_STC                         0
#define SHIFT_INI_TSTCLR_EN                 0
#define SHIFT_TSTCLR_HDUP                   2
#define SHIFT_TSTCLR_VDUP                   0
#define SHIFT_HR_INC                        0
#define SHIFT_HR_INC_SIGNZ                  11
#define SHIFT_HG_INC                        0
#define SHIFT_HG_INC_SIGNZ                  11
#define SHIFT_HB_INC                        0
#define SHIFT_HB_INC_SIGNZ                  11
#define SHIFT_HR_STEP                       0
#define SHIFT_HG_STEP                       0
#define SHIFT_HB_STEP                       0
#define SHIFT_VR_INC                        0
#define SHIFT_VR_INC_SIGNZ                  11
#define SHIFT_VG_INC                        0
#define SHIFT_VG_INC_SIGNZ                  11
#define SHIFT_VB_INC                        0
#define SHIFT_VB_INC_SIGNZ                  11
#define SHIFT_VR_STEP                       0
#define SHIFT_VG_STEP                       0
#define SHIFT_VB_STEP                       0
#endif // _REG_GOP_H_
