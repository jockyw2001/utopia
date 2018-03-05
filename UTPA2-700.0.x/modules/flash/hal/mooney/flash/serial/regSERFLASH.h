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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regSERFLASH.h
/// @brief  Serial Flash Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SERFLASH_H_
#define _REG_SERFLASH_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

// !!! Uranus Serial Flash Notes: !!!
//  - The clock of DMA & Read via XIU operations must be < 3*CPU clock
//  - The clock of DMA & Read via XIU operations are determined by only REG_ISP_CLK_SRC; other operations by REG_ISP_CLK_SRC only
//  - DMA program can't run on DRAM, but in flash ONLY
//  - DMA from SPI to DRAM => size/DRAM start/DRAM end must be 8-B aligned


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// BASEADDR & BK

#define BASEADDR_RIU                0x1F000000  // TODO: <-@@@ CHIP SPECIFIC
#define BASEADDR_XIU                0x14000000  // FLASH0 // TODO: <-@@@ CHIP SPECIFIC
// #define BASEADDR_XIU                0xB8000000  // FLASH1 // TODO: use define instead of hard code e.g. CONFIG_SERFLASH1_SEL
#define BK_ISP                      0x1000
#define BK_PIU                      0x7800
#define BK_MHEG5                    0x1E00
#define BK_PMSLP                    0x1C00
#define BK_CLK0                     0x1600
#define BK_QSPI                     0x1400
#define BK_FSP                      0x1200
#define BK_BDMA                     0x1200
//----- Chip flash -------------------------
#define REG_SPI_BASE                0x7F

// ISP_CMD

#define REG_ISP_PASSWORD            0x00 // ISP / XIU read / DMA mutual exclusive
#define REG_ISP_SPI_COMMAND         0x01
    // please refer to the serial flash datasheet
    #define ISP_SPI_CMD_WDSR                BITS(7:0, 0x01)
    #define ISP_SPI_CMD_READ                BITS(7:0, 0x03)
    #define ISP_SPI_CMD_FASTREAD            BITS(7:0, 0x0B)
    #define ISP_SPI_CMD_RDID                BITS(7:0, 0x9F)
    #define ISP_SPI_CMD_WREN                BITS(7:0, 0x06)
    #define ISP_SPI_CMD_WRDI                BITS(7:0, 0x04)
    #define ISP_SPI_CMD_SE                  BITS(7:0, 0x20)
    #define ISP_SPI_CMD_32BE                BITS(7:0, 0x52)
    #define ISP_SPI_CMD_64BE                BITS(7:0, 0xD8)
    #define ISP_SPI_CMD_CE                  BITS(7:0, 0xC7)
    #define ISP_SPI_CMD_PP                  BITS(7:0, 0x02)
    #define ISP_SPI_CMD_RDSR                BITS(7:0, 0x05)
    #define ISP_SPI_CMD_RDSR2               BITS(7:0, 0x35) // support for new WinBond Flash
    #define ISP_SPI_CMD_WRSR                BITS(7:0, 0x01)
    #define ISP_SPI_CMD_DP                  BITS(7:0, 0xB9)
    #define ISP_SPI_CMD_RDP                 BITS(7:0, 0xAB)
    #define ISP_SPI_CMD_RES                 BITS(7:0, 0xAB)
    #define ISP_SPI_CMD_REMS                BITS(7:0, 0x90)
    #define ISP_SPI_CMD_REMS4               BITS(7:0, 0xCF) // support for new MXIC Flash
    #define ISP_SPI_CMD_PARALLEL            BITS(7:0, 0x55)
    #define ISP_SPI_CMD_EN4K                BITS(7:0, 0xA5)
    #define ISP_SPI_CMD_EX4K                BITS(7:0, 0xB5)
	/* MXIC Individual Block Protection Mode */
    #define ISP_SPI_CMD_WPSEL               BITS(7:0, 0x68)
    #define ISP_SPI_CMD_SBLK                BITS(7:0, 0x36)
    #define ISP_SPI_CMD_SBULK               BITS(7:0, 0x39)
    #define ISP_SPI_CMD_RDSCUR              BITS(7:0, 0x2B)
    #define ISP_SPI_CMD_RDBLOCK             BITS(7:0, 0x3C)
    #define ISP_SPI_CMD_GBLK                BITS(7:0, 0x7E)
    #define ISP_SPI_CMD_GBULK               BITS(7:0, 0x98)
#define REG_ISP_SPI_ADDR_L          0x02 // A[15:0]
#define REG_ISP_SPI_ADDR_H          0x03 // A[23:16]
#define REG_ISP_SPI_WDATA           0x04
    #define ISP_SPI_WDATA_DUMMY     BITS(7:0, 0xFF)
#define REG_ISP_SPI_RDATA           0x05
#define REG_ISP_SPI_CLKDIV          0x06 // clock = CPU clock / this div
	#define ISP_SPI_CLKDIV2			BIT(0)
	#define	ISP_SPI_CLKDIV4			BIT(2)
	#define	ISP_SPI_CLKDIV8			BIT(6)
	#define	ISP_SPI_CLKDIV16    	BIT(7)
	#define	ISP_SPI_CLKDIV32		BIT(8)
	#define	ISP_SPI_CLKDIV64		BIT(9)
	#define	ISP_SPI_CLKDIV128		BIT(10)
#define REG_ISP_DEV_SEL             0x07
#define REG_ISP_SPI_CECLR           0x08
    #define ISP_SPI_CECLR                   BITS(0:0, 1)
#define REG_ISP_SPI_RDREQ           0x0C
    #define ISP_SPI_RDREQ                   BITS(0:0, 1)
#define REG_ISP_SPI_ENDIAN          0x0F
#define REG_ISP_SPI_RD_DATARDY      0x15
    #define ISP_SPI_RD_DATARDY_MASK     BMASK(0:0)
    #define ISP_SPI_RD_DATARDY              BITS(0:0, 1)
#define REG_ISP_SPI_WR_DATARDY      0x16
    #define ISP_SPI_WR_DATARDY_MASK     BMASK(0:0)
    #define ISP_SPI_WR_DATARDY              BITS(0:0, 1)
#define REG_ISP_SPI_WR_CMDRDY       0x17
    #define ISP_SPI_WR_CMDRDY_MASK      BMASK(0:0)
    #define ISP_SPI_WR_CMDRDY               BITS(0:0, 1)
#define REG_ISP_TRIGGER_MODE        0x2a
#define REG_ISP_CHIP_SEL            0x36
    #define SFSH_CHIP_SEL_MASK          BMASK(6:0)
    #define SFSH_CHIP_SEL_FLASH1            BIT(0)
    #define SFSH_CHIP_SEL_FLASH2            BIT(1)
    #define SFSH_CHIP_SEL_SPI_DEV1          BIT(2)
    #define SFSH_CHIP_SEL_SPI_DEV2          BIT(3)
    #define SFSH_CHIP_SEL_SPI_DEV3          BIT(4)
    #define SFSH_CHIP_SEL_SPI_DEV4          BIT(5)
    #define SFSH_CHIP_SEL_SPI_DEV5          BIT(6)
//    #define SFSH_CHIP_SEC_MASK          BMASK(7:0)          // 0x00FF // TODO: review this define
    #define SFSH_CHIP_SEL_MODE_SEL_MASK BMASK(7:7)
    #define SFSH_CHIP_SEL_RIU               BITS(7:7, 1)    // 0x0080
    #define SFSH_CHIP_SEL_XIU               BITS(7:7, 0)    // 0x0000
#define REG_ISP_CHIP_RST            0x3F // SPI clock source  [0]:gate  [1]:inv  [4:2]:clk_sel  000:Xtal clock 001:27MHz 010:36MHz 011:43.2MHz 100:54MHz 101:72MHz 110:86MHz 111:108MHz  [5]:0:xtal 1:clk_Sel
    #define SFSH_CHIP_RESET_MASK          BMASK(2:2)
    #define SFSH_CHIP_RESET                 BITS(2:2, 0)
    #define SFSH_CHIP_NOTRESET              BITS(2:2, 1)
#define REG_ISP_SPI_MODE            0x72
    #define SFSH_CHIP_FAST_MASK          BMASK(0:0) // SPI CMD [0x0B]
    #define SFSH_CHIP_FAST_ENABLE           BITS(0:0, 1)
    #define SFSH_CHIP_FAST_DISABLE          BITS(0:0, 0)
    #define SFSH_CHIP_2XREAD_MASK        BMASK(1:1) // SPI CMD [0xBB]
    #define SFSH_CHIP_2XREAD_DADD_ENABLE    BITS(1:1, 1)
    #define SFSH_CHIP_2XREAD_DISABLE        BITS(1:1, 0)
    #define SFSH_CHIP_QUAD_MASK          BMASK(3:0)
    #define SFSH_CHIP_QUAD_ENABLE           BITS(3:0, 0xA)
    #define SFSH_CHIP_QUAD_DISABLE          BITS(3:0, 0x0)

#define REG_ISP_SPI_CHIP_SELE       0x7A
    #define SFSH_CHIP_SELE_MASK          BMASK(1:0) // only for secure booting = 0;
    #define SFSH_CHIP_SELE_EXT1             BITS(1:0, 0)
    #define SFSH_CHIP_SELE_EXT2             BITS(1:0, 1)
    #define SFSH_CHIP_SELE_EXT3             BITS(1:0, 2)
#define REG_ISP_SPI_CHIP_SELE_BUSY  0x7B
    #define SFSH_CHIP_SELE_BUSY_MASK     BMASK(0:0)
    #define SFSH_CHIP_SELE_SWITCH           BITS(0:0, 1)
    #define SFSH_CHIP_SELE_DONE             BITS(0:0, 0)

// PIU_DMA

#define REG_PIU_DMA_STATUS          0x10 // [1]done [2]busy [8:15]state
    #define PIU_DMA_DONE_MASK           BMASK(0:0)
    #define PIU_DMA_DONE                    BITS(0:0, 1)
    #define PIU_DMA_BUSY_MASK           BMASK(1:1)
    #define PIU_DMA_BUSY                    BITS(1:1, 1)
    #define PIU_DMA_STATE_MASK          BMASK(15:8)
#define REG_PIU_SPI_CLK_SRC         0x26 // SPI clock source  [0]:gate  [1]:inv  [4:2]:clk_sel  000:Xtal clock 001:27MHz 010:36MHz 011:43.2MHz 100:54MHz 101:72MHz 110:86MHz 111:108MHz  [5]:0:xtal 1:clk_Sel
    #define PIU_SPI_RESET_MASK          BMASK(8:8)
    #define PIU_SPI_RESET                   BITS(8:8, 1)
    #define PIU_SPI_NOTRESET                BITS(8:8, 0)
    #define PSCS_DISABLE_MASK           BMASK(0:0)
    #define PSCS_INVERT_MASK            BMASK(1:1)
    #define PSCS_CLK_SEL_MASK           BMASK(4:2)
    #define PSCS_CLK_SEL_XTAL               BITS(4:2, 0)
    #define PSCS_CLK_SEL_27MHZ              BITS(4:2, 1)
    #define PSCS_CLK_SEL_36MHZ              BITS(4:2, 2)
    #define PSCS_CLK_SEL_43MHZ              BITS(4:2, 3)
    #define PSCS_CLK_SEL_54MHZ              BITS(4:2, 4)
    #define PSCS_CLK_SEL_72MHZ              BITS(4:2, 5)
    #define PSCS_CLK_SEL_86MHZ              BITS(4:2, 6)
    #define PSCS_CLK_SEL_108MHZ             BITS(4:2, 7)
    #define PSCS_CLK_SRC_SEL_MASK       BMASK(5:5)
    #define PSCS_CLK_SRC_SEL_XTAL           BITS(5:5, 0)
    #define PSCS_CLK_SRC_SEL_CLK            BITS(5:5, 1)
#define REG_PIU_DMA_SPISTART_L      0x70 // [15:0]
#define REG_PIU_DMA_SPISTART_H      0x71 // [23:16]
#define REG_PIU_DMA_DRAMSTART_L     0x72 // [15:0]  in unit of B; must be 8B aligned
#define REG_PIU_DMA_DRAMSTART_H     0x73 // [23:16]
#define REG_PIU_DMA_SIZE_L          0x74 // [15:0]  in unit of B; must be 8B aligned
#define REG_PIU_DMA_SIZE_H          0x75 // [23:16]
#define REG_PIU_DMA_CMD             0x76
    #define PIU_DMA_CMD_FIRE            0x0001
    #define PIU_DMA_CMD_LE              0x0000
    #define PIU_DMA_CMD_BE              0x0020

// Serial Flash Register // please refer to the serial flash datasheet
#define SF_SR_WIP_MASK                  BMASK(0:0)
#define SF_SR_WEL_MASK                  BMASK(1:1)
#define SF_SR_BP_MASK                   BMASK(5:2) // BMASK(4:2) is normal case but SERFLASH_TYPE_MX25L6405 use BMASK(5:2)
#define SF_SR_PROG_ERASE_ERR_MASK       BMASK(6:6)
#define SF_SR_SRWD_MASK                 BMASK(7:7)
    #define SF_SR_SRWD                      BITS(7:7, 1)

// PM_SLEEP CMD.
#define REG_PM_CKG_SPI              0x20 // Ref spec. before using these setting.
    #define PM_SPI_CLK_SEL_MASK         BMASK(13:10)
    #define PM_SPI_CLK_XTALI                BITS(13:10, 0)
    #define PM_SPI_CLK_54MHZ                BITS(13:10, 1)
    #define PM_SPI_CLK_86MHZ                BITS(13:10, 2)
    #define PM_SPI_CLK_108MHZ               BITS(13:10, 3)
    #define PM_SPI_CLK_SWITCH_MASK      BMASK(14:14)
    #define PM_SPI_CLK_SWITCH_OFF           BITS(14:14, 0)
    #define PM_SPI_CLK_SWITCH_ON            BITS(14:14, 1)
#define REG_PM_CHK_51MODE           0x53  
    #define PM_51_ON_SPI                BITS(0:0, 0x1)
    #define PM_51_ONT_ON_SPI            BITS(0:0, 0x0)

// For Power Consumption
#define REG_PM_GPIO_SPICZ_OEN		0x17
#define REG_PM_GPIO_SPICK_OEN		0x18
#define REG_PM_GPIO_SPIDI_OEN		0x19
#define REG_PM_GPIO_SPIDO_OEN		0x1A
#define REG_PM_SPI_IS_GPIO			0x35
	#define PM_SPI_GPIO_MASK			BMASK(3:0)
	#define PM_SPI_IS_GPIO					BITS(3:0, 0xF)
	#define PM_SPI_NOT_GPIO					BITS(3:0, 0x0)

// CLK_GEN0
#define REG_CLK0_CKG_SPI            0x16
    #define CLK0_CKG_SPI_MASK           BMASK(5:2)
    #define CLK0_CKG_SPI_XTALI              BITS(5:2, 0)
    #define CLK0_CKG_SPI_54MHZ              BITS(5:2, 1)
    #define CLK0_CKG_SPI_86MHZ              BITS(5:2, 2)
    #define CLK0_CKG_SPI_108MHZ             BITS(5:2, 3)
    #define CLK0_CLK_SWITCH_MASK        BMASK(6:6)
    #define CLK0_CLK_SWITCH_OFF           BITS(6:6, 0)
    #define CLK0_CLK_SWITCH_ON            BITS(6:6, 1)

//QSPI
#define REG_QSPI_MASK_GRANT         0x60
#define REG_DEBUG_BUS_0             0x76
#define REG_SPI_BURST_WRITE         0x6A
    #define REG_SPI_DISABLE_BURST 0x02
    #define REG_SPI_ENABLE_BURST  0x01
// please refer to the serial flash datasheet
#define SPI_CMD_READ                (0x03)
#define SPI_CMD_FASTREAD            (0x0B)
#define SPI_CMD_RDID                (0x9F)
#define SPI_CMD_WREN                (0x06)
#define SPI_CMD_WRDI                (0x04)
#define SPI_CMD_SE                  (0x20)
#define SPI_CMD_32BE                (0x52)
#define SPI_CMD_64BE                (0xD8)
#define SPI_CMD_CE                  (0xC7)
#define SPI_CMD_PP                  (0x02)
#define SPI_CMD_RDSR                (0x05)
#define SPI_CMD_RDSR2               (0x35) // support for new WinBond Flash#define SPI_CMD_WRSR                (0x01)
#define FLASH_OIP                    0x01
// FSP Register 
	// FSP
#define REG_FSP_WDB0    0x60
#define REG_FSP_WDB0_MASK       BMASK(7:0)
#define REG_FSP_WDB0_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB1    0x60
#define REG_FSP_WDB1_MASK       BMASK(15:8)
#define REG_FSP_WDB1_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB2    0x61
#define REG_FSP_WDB2_MASK       BMASK(7:0)
#define REG_FSP_WDB2_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB3    0x61
#define REG_FSP_WDB3_MASK       BMASK(15:8)
#define REG_FSP_WDB3_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB4    0x62
#define REG_FSP_WDB4_MASK       BMASK(7:0)
#define REG_FSP_WDB4_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB5    0x62
#define REG_FSP_WDB5_MASK       BMASK(15:8)
#define REG_FSP_WDB5_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB6    0x63
#define REG_FSP_WDB6_MASK       BMASK(7:0)
#define REG_FSP_WDB6_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB7    0x63
#define REG_FSP_WDB7_MASK       BMASK(15:8)
#define REG_FSP_WDB7_DATA(d)    BITS(15:8, d)
#define REG_FSP_WDB8    0x64
#define REG_FSP_WDB8_MASK       BMASK(7:0)
#define REG_FSP_WDB8_DATA(d)    BITS(7:0, d)
#define REG_FSP_WDB9    0x64
#define REG_FSP_WDB9_MASK       BMASK(15:8)
#define REG_FSP_WDB9_DATA(d)    BITS(15:8, d)
#define REG_FSP_RDB0    0x65
#define REG_FSP_RDB1    0x65
#define REG_FSP_RDB2    0x66
#define REG_FSP_RDB3    0x66
#define REG_FSP_RDB4    0x67
#define REG_FSP_RDB5    0x67
#define REG_FSP_RDB6    0x68
#define REG_FSP_RDB7    0x68
#define REG_FSP_RDB8    0x69
#define REG_FSP_RDB9    0x69
#define REG_FSP_WBF_SIZE   0x6a
#define REG_FSP_WBF_SIZE0_MASK     BMASK(3:0)
#define REG_FSP_WBF_SIZE0(s)       BITS(3:0,s)
#define REG_FSP_WBF_SIZE1_MASK     BMASK(7:4)
#define REG_FSP_WBF_SIZE1(s)       BITS(7:4,s)
#define REG_FSP_WBF_SIZE2_MASK     BMASK(11:8)
#define REG_FSP_WBF_SIZE2(s)       BITS(11:8,s)
#define REG_FSP_RBF_SIZE   0x6b 
#define REG_FSP_RBF_SIZE0_MASK     BMASK(3:0)
#define REG_FSP_RBF_SIZE0(s)       BITS(3:0,s)
#define REG_FSP_RBF_SIZE1_MASK     BMASK(7:4)
#define REG_FSP_RBF_SIZE1(s)       BITS(7:4,s)
#define REG_FSP_RBF_SIZE2_MASK     BMASK(11:8)
#define REG_FSP_RBF_SIZE2(s)       BITS(11:8,s)
#define REG_FSP_CTRL   0x6c
#define REG_FSP_ENABLE_MASK        BMASK(0:0)
#define REG_FSP_ENABLE             BITS(0:0,1)
#define REG_FSP_DISABLE            BITS(0:0,0)
#define REG_FSP_RESET_MASK         BMASK(1:1)
#define REG_FSP_RESET              BITS(1:1,0)
#define REG_FSP_NRESET             BITS(1:1,1)
#define REG_FSP_INT_MASK           BMASK(2:2)
#define REG_FSP_INT                BITS(2:2,1)
#define REG_FSP_INT_OFF            BITS(2:2,0)
#define REG_FSP_CHK_MASK           BMASK(3:3)
#define REG_FSP_CHK                BITS(3:3,1)
#define REG_FSP_CHK_OFF            BITS(3:3,0)
#define REG_FSP_RDSR_MASK          BMASK(12:11)
#define REG_FSP_1STCMD             BITS(12:11,0)
#define REG_FSP_2NDCMD             BITS(12:11,1)
#define REG_FSP_3THCMD             BITS(12:11,2)
#define REG_FSP_FSCHK_MASK         BMASK(13:13)
#define REG_FSP_FSCHK_ON           BITS(13:13,1)
#define REG_FSP_FSCHK_OFF          BITS(13:13,0)
#define REG_FSP_3THCMD_MASK        BMASK(14:14)
#define REG_FSP_3THCMD_ON          BITS(14:14,1)
#define REG_FSP_3THCMD_OFF         BITS(14:14,0)
#define REG_FSP_2NDCMD_MASK        BMASK(15:15)
#define REG_FSP_2NDCMD_ON          BITS(15:15,1)
#define REG_FSP_2NDCMD_OFF         BITS(15:15,0)
#define REG_FSP_TRIGGER   0x6d
#define REG_FSP_TRIGGER_MASK        BMASK(0:0)
#define REG_FSP_FIRE             BITS(0:0,1)
#define REG_FSP_DONE_FLAG   0x6e
#define REG_FSP_DONE_FLAG_MASK        BMASK(0:0)
#define REG_FSP_DONE             BITS(0:0,1)
#define REG_FSP_DONE_CLR   0x6f
#define REG_FSP_DONE_CLR_MASK        BMASK(0:0)
#define REG_FSP_CLR             BITS(0:0,1)
	// Serial Flash Register // please refer to the serial flash datasheet
#define SF_SR_WIP_MASK                  BMASK(0:0)
#define SF_SR_WEL_MASK                  BMASK(1:1)
#define SF_SR_BP_MASK                   BMASK(5:2)
	// BMASK(4:2) is normal case but SERFLASH_TYPE_MX25L6405 use BMASK(5:2)
#define SF_SR_PROG_ERASE_ERR_MASK       BMASK(6:6)
#define SF_SR_SRWD_MASK                 BMASK(7:7)
#define SF_SR_SRWD                      BITS(7:7, 1)
#define REG_FSP_WRITEDATA_SIZE    0x4
#define REG_FSP_MAX_WRITEDATA_SIZE 0xA 
#define REG_FSP_READDATA_SIZE     0xA
// FSP BUF definition
#define MAX_READ_BUF_CNT          0xA
#define SPI_FLASH_ADDR_LEN        0x3
#define SINGLE_WRITE_SIZE         0x4
#define ENABLE_SECOND_CMD         0x8000
#define ENABLE_THIRD_CMD          0x4000
#define ENABLE_AUTO_CHECK         0x2000
#define CHECK_CMD_OFFSET          0x1000
#define FLASH_PAGE_SIZE           0x100
#endif // _REG_SERFLASH_H_
