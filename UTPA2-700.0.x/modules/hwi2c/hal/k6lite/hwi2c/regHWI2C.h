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
////////////////////////////////////////////////////////////////////////////////

#ifndef _REGHWI2C_H_
#define _REGHWI2C_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

//############################
//
//IP bank address : for pad mux in chiptop
//
//############################
#define CHIP_REG_BASE                   (0x1E00)
#define PIU_NONPM_REG_BASE              (0x3C00) //0x103C_02_Bit[1:0]
#define PMSLEEP_REG_BASE                (0x0E00)
#define CLKGEN1_REG_BASE                (0x3300)
//for port 0

#define CHIP_REG_HWI2C_MIIC0            (CHIP_REG_BASE+ (0x09*2))
    #define CHIP_MIIC0_PAD_0            0
    #define CHIP_MIIC0_PAD_1            (__BIT0)
    #define CHIP_MIIC0_PAD_2            (__BIT1)
    #define CHIP_MIIC0_PAD_3            (__BIT0|__BIT1)
    #define CHIP_MIIC0_PAD_MSK          (__BIT0|__BIT1)

//for port 1
#define CHIP_REG_HWI2C_MIIC1            (CHIP_REG_BASE+ (0x09*2))
    #define CHIP_MIIC1_PAD_0            (0)
    #define CHIP_MIIC1_PAD_1            (__BIT2)
    #define CHIP_MIIC1_PAD_2            (__BIT3)
    #define CHIP_MIIC1_PAD_3            (__BIT2|__BIT3)
    #define CHIP_MIIC1_PAD_MSK          (__BIT2|__BIT3)


//pad mux configuration
#define CHIP_REG_ALLPADIN               (CHIP_REG_BASE+0xA1)
    #define CHIP_ALLPAD_IN              (__BIT7)

#define REG_HWI2C_MIIC_VER_SEL          (PIU_NONPM_REG_BASE+ (0x02*2)) //select MI2C mode
    #define REG_HWI2C_MIIC_VER_V2       (0)
    #define REG_HWI2C_MIIC_VER_V3       (__BIT1|__BIT0)
    #define REG_HWI2C_MIIC_VER_MSK      (__BIT1|__BIT0)

#define CHIP_REG_HWI2C_MIIC0_CLK              (CLKGEN1_REG_BASE + (0x30) * 2)
    #define CHIP_REG_HWI2C_MIIC0_CLK_72M      0
    #define CHIP_REG_HWI2C_MIIC0_CLK_XTAL     (__BIT2)
    #define CHIP_REG_HWI2C_MIIC0_CLL_36M      (__BIT3)
    #define CHIP_REG_HWI2C_MIIC0_CLK_54M      (__BIT2 | __BIT3)
    #define CHIP_REG_HWI2C_MIIC0_CLK_MSK      (__BIT0 | __BIT1 | __BIT2 | __BIT3)

#define CHIP_REG_HWI2C_MIIC1_CLK              (CLKGEN1_REG_BASE + (0x30) * 2)
    #define CHIP_REG_HWI2C_MIIC1_CLK_72M      0
    #define CHIP_REG_HWI2C_MIIC1_CLK_XTAL     (__BIT6)
    #define CHIP_REG_HWI2C_MIIC1_CLL_36M      (__BIT7)
    #define CHIP_REG_HWI2C_MIIC1_CLK_54M      (__BIT6 | __BIT7)
    #define CHIP_REG_HWI2C_MIIC1_CLK_MSK      (__BIT4 | __BIT5 | __BIT6 | __BIT7)
//############################
//
//IP bank address : for independent port
//
//############################
//Standard mode
#define HWI2C_REG_BASE                  (0x13400) //0x1(13400) + offset ==> default set to port 0
#define HWI2C_REG_BASE_PM               (0x01700) //port3 in PM bank

//#################
//#
//#  For Non-PM HWI2C
//#
//STD mode
#define REG_HWI2C_MIIC_CFG              (HWI2C_REG_BASE+0x00*2)
    #define _MIIC_CFG_RESET             (__BIT0)
    #define _MIIC_CFG_EN_DMA            (__BIT1)
    #define _MIIC_CFG_EN_INT            (__BIT2)
    #define _MIIC_CFG_EN_CLKSTR         (__BIT3)
    #define _MIIC_CFG_EN_TMTINT         (__BIT4)
    #define _MIIC_CFG_EN_FILTER         (__BIT5)
    #define _MIIC_CFG_EN_PUSH1T         (__BIT6)
    #define _MIIC_CFG_RESERVED          (__BIT7)
#define REG_HWI2C_CMD_START             (HWI2C_REG_BASE+0x01*2)
    #define _CMD_START                  (__BIT0)
#define REG_HWI2C_CMD_STOP              (HWI2C_REG_BASE+0x01*2+1)
    #define _CMD_STOP                   (__BIT0)
#define REG_HWI2C_WDATA                 (HWI2C_REG_BASE+0x02*2)
#define REG_HWI2C_WDATA_GET             (HWI2C_REG_BASE+0x02*2+1)
    #define _WDATA_GET_ACKBIT           (__BIT0)
#define REG_HWI2C_RDATA                 (HWI2C_REG_BASE+0x03*2)
#define REG_HWI2C_RDATA_CFG             (HWI2C_REG_BASE+0x03*2+1)
    #define _RDATA_CFG_TRIG             (__BIT0)
    #define _RDATA_CFG_ACKBIT           (__BIT1)
#define REG_HWI2C_INT_CTL               (HWI2C_REG_BASE+0x04*2)
    #define _INT_CTL                    (__BIT0) //write this register to clear int
#define REG_HWI2C_CUR_STATE             (HWI2C_REG_BASE+0x05*2) //For Debug
    #define _CUR_STATE_MSK              (__BIT4|__BIT3|__BIT2|__BIT1|__BIT0)
#define REG_HWI2C_INT_STATUS            (HWI2C_REG_BASE+0x05*2+1) //For Debug
    #define _INT_STARTDET               (__BIT0)
    #define _INT_STOPDET                (__BIT1)
    #define _INT_RXDONE                 (__BIT2)
    #define _INT_TXDONE                 (__BIT3)
    #define _INT_CLKSTR                 (__BIT4)
    #define _INT_SCLERR                 (__BIT5)
#define REG_HWI2C_STP_CNT               (HWI2C_REG_BASE+0x08*2)
#define REG_HWI2C_CKH_CNT               (HWI2C_REG_BASE+0x09*2)
#define REG_HWI2C_CKL_CNT               (HWI2C_REG_BASE+0x0A*2)
#define REG_HWI2C_SDA_CNT               (HWI2C_REG_BASE+0x0B*2)
#define REG_HWI2C_STT_CNT               (HWI2C_REG_BASE+0x0C*2)
#define REG_HWI2C_LTH_CNT               (HWI2C_REG_BASE+0x0D*2)
#define REG_HWI2C_TMT_CNT               (HWI2C_REG_BASE+0x0E*2)
#define REG_HWI2C_SCLI_DELAY            (HWI2C_REG_BASE+0x0F*2)
    #define _SCLI_DELAY                 (__BIT2|__BIT1|__BIT0)

//DMA mode
#define REG_HWI2C_DMA_CFG               (HWI2C_REG_BASE+0x20*2)
    #define _DMA_CFG_RESET              (__BIT1)
    #define _DMA_CFG_INTEN              (__BIT2)
    #define _DMA_CFG_MIURST             (__BIT3)
    #define _DMA_CFG_MIUPRI             (__BIT4)
#define REG_HWI2C_DMA_MIU_ADR           (HWI2C_REG_BASE+0x21*2) // 4 bytes
#define REG_HWI2C_DMA_CTL               (HWI2C_REG_BASE+0x23*2)
//    #define _DMA_CTL_TRIG               (__BIT0)
//    #define _DMA_CTL_RETRIG             (__BIT1)
    #define _DMA_CTL_TXNOSTOP           (__BIT5) //miic transfer format, 1: S+data..., 0: S+data...+P
    #define _DMA_CTL_RDWTCMD            (__BIT6) //miic transfer format, 1:read, 0:write
    #define _DMA_CTL_MIUCHSEL           (__BIT7) //0: miu0, 1:miu1
#define REG_HWI2C_DMA_TXR               (HWI2C_REG_BASE+0x24*2)
    #define _DMA_TXR_DONE               (__BIT0)
#define REG_HWI2C_DMA_CMDDAT0           (HWI2C_REG_BASE+0x25*2) // 8 bytes
#define REG_HWI2C_DMA_CMDDAT1           (HWI2C_REG_BASE+0x25*2+1)
#define REG_HWI2C_DMA_CMDDAT2           (HWI2C_REG_BASE+0x26*2)
#define REG_HWI2C_DMA_CMDDAT3           (HWI2C_REG_BASE+0x26*2+1)
#define REG_HWI2C_DMA_CMDDAT4           (HWI2C_REG_BASE+0x27*2)
#define REG_HWI2C_DMA_CMDDAT5           (HWI2C_REG_BASE+0x27*2+1)
#define REG_HWI2C_DMA_CMDDAT6           (HWI2C_REG_BASE+0x28*2)
#define REG_HWI2C_DMA_CMDDAT7           (HWI2C_REG_BASE+0x28*2+1)
#define REG_HWI2C_DMA_CMDLEN            (HWI2C_REG_BASE+0x29*2)
    #define _DMA_CMDLEN_MSK             (__BIT2|__BIT1|__BIT0)
#define REG_HWI2C_DMA_DATLEN            (HWI2C_REG_BASE+0x2A*2) // 4 bytes
#define REG_HWI2C_DMA_TXFRCNT           (HWI2C_REG_BASE+0x2C*2) // 4 bytes
#define REG_HWI2C_DMA_SLVADR            (HWI2C_REG_BASE+0x2E*2)
    #define _DMA_SLVADR_10BIT_MSK       0x3FF //10 bits
    #define _DMA_SLVADR_NORML_MSK       0x7F //7 bits
#define REG_HWI2C_DMA_SLVCFG            (HWI2C_REG_BASE+0x2E*2+1)
    #define _DMA_10BIT_MODE             (__BIT2)
#define REG_HWI2C_DMA_CTL_TRIG          (HWI2C_REG_BASE+0x2F*2)
    #define _DMA_CTL_TRIG               (__BIT0)
#define REG_HWI2C_DMA_CTL_RETRIG        (HWI2C_REG_BASE+0x2F*2+1)
    #define _DMA_CTL_RETRIG             (__BIT0)

//#################
//#
//#  For PM HWI2C
//#
//STD mode
#define REG_HWI2C_MIIC_CFG_PM           (HWI2C_REG_BASE_PM+0x00*2)
    #define _MIIC_CFG_RESET             (__BIT0)
    #define _MIIC_CFG_EN_DMA            (__BIT1)
    #define _MIIC_CFG_EN_INT            (__BIT2)
    #define _MIIC_CFG_EN_CLKSTR         (__BIT3)
    #define _MIIC_CFG_EN_TMTINT         (__BIT4)
    #define _MIIC_CFG_EN_FILTER         (__BIT5)
    #define _MIIC_CFG_EN_PUSH1T         (__BIT6)
    #define _MIIC_CFG_RESERVED          (__BIT7)
#define REG_HWI2C_CMD_START_PM          (HWI2C_REG_BASE_PM+0x01*2)
    #define _CMD_START                  (__BIT0)
#define REG_HWI2C_CMD_STOP_PM           (HWI2C_REG_BASE_PM+0x01*2+1)
    #define _CMD_STOP                   (__BIT0)
#define REG_HWI2C_WDATA_PM              (HWI2C_REG_BASE_PM+0x02*2)
#define REG_HWI2C_WDATA_GET_PM          (HWI2C_REG_BASE_PM+0x02*2+1)
    #define _WDATA_GET_ACKBIT           (__BIT0)
#define REG_HWI2C_RDATA_PM              (HWI2C_REG_BASE_PM+0x03*2)
#define REG_HWI2C_RDATA_CFG_PM          (HWI2C_REG_BASE_PM+0x03*2+1)
    #define _RDATA_CFG_TRIG             (__BIT0)
    #define _RDATA_CFG_ACKBIT           (__BIT1)
#define REG_HWI2C_INT_CTL_PM            (HWI2C_REG_BASE_PM+0x04*2)
    #define _INT_CTL                    (__BIT0) //write this register to clear int
#define REG_HWI2C_CUR_STATE_PM          (HWI2C_REG_BASE_PM+0x05*2) //For Debug
    #define _CUR_STATE_MSK              (__BIT4|__BIT3|__BIT2|__BIT1|__BIT0)
#define REG_HWI2C_INT_STATUS_PM         (HWI2C_REG_BASE_PM+0x05*2+1) //For Debug
    #define _INT_STARTDET               (__BIT0)
    #define _INT_STOPDET                (__BIT1)
    #define _INT_RXDONE                 (__BIT2)
    #define _INT_TXDONE                 (__BIT3)
    #define _INT_CLKSTR                 (__BIT4)
    #define _INT_SCLERR                 (__BIT5)
#define REG_HWI2C_STP_CNT_PM            (HWI2C_REG_BASE_PM+0x08*2)
#define REG_HWI2C_CKH_CNT_PM            (HWI2C_REG_BASE_PM+0x09*2)
#define REG_HWI2C_CKL_CNT_PM            (HWI2C_REG_BASE_PM+0x0A*2)
#define REG_HWI2C_SDA_CNT_PM            (HWI2C_REG_BASE_PM+0x0B*2)
#define REG_HWI2C_STT_CNT_PM            (HWI2C_REG_BASE_PM+0x0C*2)
#define REG_HWI2C_LTH_CNT_PM            (HWI2C_REG_BASE_PM+0x0D*2)
#define REG_HWI2C_TMT_CNT_PM            (HWI2C_REG_BASE_PM+0x0E*2)
#define REG_HWI2C_SCLI_DELAY_PM         (HWI2C_REG_BASE_PM+0x0F*2)
    #define _SCLI_DELAY                 (__BIT2|__BIT1|__BIT0)

//DMA mode
#define REG_HWI2C_DMA_CFG_PM            (HWI2C_REG_BASE_PM+0x20*2)
    #define _DMA_CFG_RESET              (__BIT1)
    #define _DMA_CFG_INTEN              (__BIT2)
    #define _DMA_CFG_MIURST             (__BIT3)
    #define _DMA_CFG_MIUPRI             (__BIT4)
#define REG_HWI2C_DMA_MIU_ADR_PM        (HWI2C_REG_BASE_PM+0x21*2) // 4 bytes
#define REG_HWI2C_DMA_CTL_PM            (HWI2C_REG_BASE_PM+0x23*2)
    //    #define _DMA_CTL_TRIG               (__BIT0)
    //    #define _DMA_CTL_RETRIG             (__BIT1)
    #define _DMA_CTL_TXNOSTOP           (__BIT5) //miic transfer format, 1: S+data..., 0: S+data...+P
    #define _DMA_CTL_RDWTCMD            (__BIT6) //miic transfer format, 1:read, 0:write
    #define _DMA_CTL_MIUCHSEL           (__BIT7) //0: miu0, 1:miu1
#define REG_HWI2C_DMA_TXR_PM            (HWI2C_REG_BASE_PM+0x24*2)
    #define _DMA_TXR_DONE               (__BIT0)
#define REG_HWI2C_DMA_CMDDAT0_PM        (HWI2C_REG_BASE_PM+0x25*2) // 8 bytes
#define REG_HWI2C_DMA_CMDDAT1_PM        (HWI2C_REG_BASE_PM+0x25*2+1)
#define REG_HWI2C_DMA_CMDDAT2_PM        (HWI2C_REG_BASE_PM+0x26*2)
#define REG_HWI2C_DMA_CMDDAT3_PM        (HWI2C_REG_BASE_PM+0x26*2+1)
#define REG_HWI2C_DMA_CMDDAT4_PM        (HWI2C_REG_BASE_PM+0x27*2)
#define REG_HWI2C_DMA_CMDDAT5_PM        (HWI2C_REG_BASE_PM+0x27*2+1)
#define REG_HWI2C_DMA_CMDDAT6_PM        (HWI2C_REG_BASE_PM+0x28*2)
#define REG_HWI2C_DMA_CMDDAT7_PM        (HWI2C_REG_BASE_PM+0x28*2+1)
#define REG_HWI2C_DMA_CMDLEN_PM         (HWI2C_REG_BASE_PM+0x29*2)
    #define _DMA_CMDLEN_MSK             (__BIT2|__BIT1|__BIT0)
#define REG_HWI2C_DMA_DATLEN_PM         (HWI2C_REG_BASE_PM+0x2A*2) // 4 bytes
#define REG_HWI2C_DMA_TXFRCNT_PM        (HWI2C_REG_BASE_PM+0x2C*2) // 4 bytes
#define REG_HWI2C_DMA_SLVADR_PM         (HWI2C_REG_BASE_PM+0x2E*2)
    #define _DMA_SLVADR_10BIT_MSK       0x3FF //10 bits
    #define _DMA_SLVADR_NORML_MSK       0x7F //7 bits
#define REG_HWI2C_DMA_SLVCFG_PM         (HWI2C_REG_BASE_PM+0x2E*2+1)
    #define _DMA_10BIT_MODE             (__BIT2)
#define REG_HWI2C_DMA_CTL_TRIG_PM       (HWI2C_REG_BASE_PM+0x2F*2)
    #define _DMA_CTL_TRIG               (__BIT0)
#define REG_HWI2C_DMA_CTL_RETRIG_PM     (HWI2C_REG_BASE_PM+0x2F*2+1)
    #define _DMA_CTL_RETRIG             (__BIT0)


#endif
