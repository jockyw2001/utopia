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
/// file    regHVD.h
/// @brief  HVD Module Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_HVD_H_
#define _REG_HVD_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//*****************************************************************************
// RIU macro
#define HVD_MACRO_START     do {
#define HVD_MACRO_END       } while (0)
#define HVD_RIU_BASE        (u32HVDRegOSBase)

#define HVD_HIGHBYTE(u16)               ((MS_U8)((u16) >> 8))
#define HVD_LOWBYTE(u16)                ((MS_U8)(u16))
#define HVD_RIU_READ_BYTE(addr)   ( READ_BYTE( HVD_RIU_BASE + (addr) ) )
#define HVD_RIU_READ_WORD(addr)   ( READ_WORD( HVD_RIU_BASE + (addr) ) )
#define HVD_RIU_WRITE_BYTE(addr, val)      { WRITE_BYTE( HVD_RIU_BASE+(addr), val); }
#define HVD_RIU_WRITE_WORD(addr, val)      { WRITE_WORD( HVD_RIU_BASE+(addr), val); }


#define _HVD_ReadByte( u32Reg )   HVD_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define _HVD_Read2Byte( u32Reg )    (HVD_RIU_READ_WORD((u32Reg)<<1))

#define _HVD_Read4Byte( u32Reg )   ( (MS_U32)HVD_RIU_READ_WORD((u32Reg)<<1) | ((MS_U32)HVD_RIU_READ_WORD(((u32Reg)+2)<<1)<<16 )  )

#define _HVD_ReadRegBit( u32Reg, u8Mask )   (HVD_RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define _HVD_ReadWordBit( u32Reg, u16Mask )   (_HVD_Read2Byte( u32Reg ) & (u16Mask))

#define _HVD_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (HVD_RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (HVD_RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    HVD_MACRO_END

#define _HVD_WriteByte( u32Reg, u8Val )                                                 \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    HVD_MACRO_END

#define _HVD_Write2Byte( u32Reg, u16Val )                                               \
    HVD_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        HVD_RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        HVD_RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        HVD_RIU_WRITE_WORD( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    HVD_MACRO_END

#define _HVD_Write3Byte( u32Reg, u32Val )   \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                               \
        HVD_RIU_WRITE_BYTE((u32Reg << 1) - 1, u32Val);                                    \
        HVD_RIU_WRITE_WORD( (u32Reg + 1)<<1 , ((u32Val) >> 8));                                      \
    }                                                                                           \
    else                                                                                        \
    {                                                                                               \
        HVD_RIU_WRITE_WORD( (u32Reg) << 1,  u32Val);                                                         \
        HVD_RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16));                             \
    }

#define _HVD_Write4Byte( u32Reg, u32Val )                                               \
    HVD_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                      \
    {                                                                                               \
        HVD_RIU_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  u32Val);                                         \
        HVD_RIU_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8));                                      \
        HVD_RIU_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24));                           \
    }                                                                                               \
    else                                                                                                \
    {                                                                                                   \
        HVD_RIU_WRITE_WORD( (u32Reg) <<1 ,  u32Val);                                                             \
        HVD_RIU_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16));                                             \
    }                                                                     \
    HVD_MACRO_END

#define _HVD_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    HVD_MACRO_START                                                                     \
    HVD_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (HVD_RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    HVD_MACRO_END

#define _HVD_WriteWordMask( u32Reg, u16Val , u16Msk)                                               \
    HVD_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                                           \
        _HVD_WriteByteMask( ((u32Reg)+1) , (((u16Val) & 0xff00)>>8) , (((u16Msk)&0xff00)>>8) );                                                                          \
        _HVD_WriteByteMask( (u32Reg) , ((u16Val) & 0x00ff) , ((u16Msk)&0x00ff) );                                                                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        HVD_RIU_WRITE_WORD( ((u32Reg)<<1) ,  (((u16Val) & (u16Msk))  | (_HVD_Read2Byte( u32Reg  ) & (~( u16Msk ))))  );                                                       \
    }                                                                               \
    HVD_MACRO_END

//------------------------------------------------------------------------------
// MVD Reg
//------------------------------------------------------------------------------
#define REG_MVD_BASE                    (0x1100)

#define MVD_REG_STAT_CTRL               (REG_MVD_BASE)
    #define MVD_REG_CTRL_RST            BIT(0)
    #define MVD_REG_CTRL_INIT           BIT(2)
    #define MVD_REG_DISCONNECT_MIU      BIT(6)

#if 1//Note: this setting should be set according client table of each chip
#define MIU0_REG_BASE                           0x1200
#define MIU1_REG_BASE                           0x0600

#define MIU_CLIENT_SELECT_GP2          (MIU0_REG_BASE + (0x007A<<1))
    #define MIU_CLIENT_SELECT_GP2_MVD   BIT(4)
#endif



//------------------------------------------------------------------------------
// HVD Reg
//------------------------------------------------------------------------------
#define REG_HVD_BASE                    (0x1B00)
#define REG_EVD_BASE                    (0x1C00)
#define REG_MIU_SOURCE_BASE             (0x0600)
#define REG_G2VP9_BASE                  (0x60E00)
#define REG_HVD_BBU5_BBU6_BASE         (0x1700)
#define REG_EVD_BBU3_BBU4_BASE         (0x2700)


#define HVD_REG_REV_ID                          (REG_HVD_BASE + ((0x0000) << 1))
#define HVD_REG_RESET                           (REG_HVD_BASE + ((0x0001) << 1))
    #define HVD_REG_RESET_SWRST                 BIT(0)
    #define HVD_REG_RESET_IDB_MIU_256           BIT(1)
    #define HVD_REG_RESET_SWRST_FIN             BIT(2)
    #define HVD_REG_RESET_STOP_BBU              BIT(3)
    #define HVD_REG_RESET_MIU_RDY               BIT(4)
    #define HVD_REG_RESET_MIU1_128              BIT(5)
    #define HVD_REG_RESET_MIU1_256              BIT(6)
    #define HVD_REG_MC_MIU_256                  BIT(7)
    #define HVD_REG_RESET_HK_AVS_MODE           BIT(8)
    #define HVD_REG_RESET_HK_RM_MODE            BIT(9)
    #define HVD_REG_RESET_HK_RV9_DEC_MODE       BIT(10)
    #define HVD_REG_RESET_MIU_128               BIT(11)
    #define HVD_REG_RESET_CPUIF_SEL             BIT(12)
    #define HVD_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define HVD_REG_RESET_MIU_256               BIT(14)
    #define HVD_REG_RESET_BOND_HD               BIT(15)

#define HVD_REG_ESB_ST_ADDR_L(reg_base)                   (reg_base + ((0x0002) << 1))
#define HVD_REG_ESB_ST_ADDR_H(reg_base)                   (reg_base + ((0x0003) << 1))

#define HVD_REG_ESB_LENGTH_L(reg_base)                    (reg_base + ((0x0004) << 1))
#define HVD_REG_ESB_LENGTH_H(reg_base)                    (reg_base + ((0x0005) << 1))

#define HVD_REG_ESB_RPTR(reg_base)                        (reg_base + ((0x0006) << 1))
    #define HVD_REG_ESB_RPTR_POLL               BIT(0)

#define HVD_REG_ESB_RPTR_H(reg_base)                      (reg_base + ((0x0007) << 1))

#define HVD_REG_MIF_BBU(reg_base)                         (reg_base + ((0x0008) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS           7
    #define HVD_REG_MIF_OFFSET_H                BIT(12)
    #define HVD_REG_BBU_TSP_INPUT               BIT(8)
    #define HVD_REG_BBU_PASER_MASK              (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE           0
    #define HVD_REG_BBU_PASER_ENABLE_ALL        BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03         (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB            BIT(11)

#define HVD_REG_NAL_TBL_ST_ADDR_L(reg_base)               (reg_base + ((0x0009) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H(reg_base)               (reg_base + ((0x000A) << 1))

#define HVD_REG_HI_MBOX0_L(reg_base)                      (reg_base + ((0x000B) << 1))
#define HVD_REG_HI_MBOX0_H(reg_base)                      (reg_base + ((0x000C) << 1))
#define HVD_REG_HI_MBOX1_L(reg_base)                      (reg_base + ((0x000D) << 1))
#define HVD_REG_HI_MBOX1_H(reg_base)                      (reg_base + ((0x000E) << 1))
#define HVD_REG_HI_MBOX_SET(reg_base)                     (reg_base + ((0x000F) << 1))
    #define HVD_REG_HI_MBOX0_SET                BIT(0)
    #define HVD_REG_HI_MBOX1_SET                BIT(8)

#define HVD_REG_RISC_MBOX_CLR(reg_base)                   (reg_base + ((0x0010) << 1))
    #define HVD_REG_RISC_MBOX0_CLR              BIT(0)
    #define HVD_REG_RISC_MBOX1_CLR              BIT(1)
    #define HVD_REG_RISC_ISR_CLR                BIT(2)
    #define HVD_REG_NAL_WPTR_SYNC               BIT(3)
    #define HVD_REG_RISC_ISR_MSK                BIT(6)
    #define HVD_REG_RISC_ISR_FORCE              BIT(10)

#define HVD_REG_RISC_MBOX_RDY(reg_base)                   (reg_base + ((0x0011) << 1))
    #define HVD_REG_RISC_MBOX0_RDY              BIT(0)
    #define HVD_REG_RISC_MBOX1_RDY              BIT(4)
    #define HVD_REG_RISC_ISR_VALID              BIT(8)

#define HVD_REG_HI_MBOX_RDY(reg_base)                     (reg_base + ((0x0012) << 1))
    #define HVD_REG_HI_MBOX0_RDY                BIT(0)
    #define HVD_REG_HI_MBOX1_RDY                BIT(8)

#define HVD_REG_RISC_MBOX0_L(reg_base)                    (reg_base + ((0x0013) << 1))
#define HVD_REG_RISC_MBOX0_H(reg_base)                    (reg_base + ((0x0014) << 1))
#define HVD_REG_RISC_MBOX1_L(reg_base)                    (reg_base + ((0x0015) << 1))
#define HVD_REG_RISC_MBOX1_H(reg_base)                    (reg_base + ((0x0016) << 1))

#define HVD_REG_POLL_NAL_RPTR(reg_base)                   (reg_base + ((0x0017) << 1))
    #define HVD_REG_POLL_NAL_RPTR_BIT           BIT(0)
#define HVD_REG_NAL_RPTR_HI(reg_base)                     (reg_base + ((0x0018) << 1))
#define HVD_REG_NAL_WPTR_HI(reg_base)                     (reg_base + ((0x0019) << 1))
#define HVD_REG_NAL_TAB_LEN(reg_base)                     (reg_base + ((0x0020) << 1))

#define HVD_REG_DEBUG_DAT_L                     (REG_HVD_BASE + ((0x0023) << 1))
#define HVD_REG_DEBUG_DAT_H                     (REG_HVD_BASE + ((0x0024) << 1))
#define HVD_REG_DEBUG_SEL                       (REG_HVD_BASE + ((0x0025) << 1))

#define HVD_REG_BWA_CLK                          (REG_HVD_BASE + ((0x002D) << 1))
    #define HVD_REG_CLK_HVD_SW_OV_EN              BIT(0)
    #define HVD_REG_CLK_HVD_SW_UPD                BIT(1)
    #define HVD_REG_CLK_HVD_IDB_SW_OV_EN          BIT(2)
    #define HVD_REG_CLK_HVD_IDB_SW_UPD            BIT(3)
    #define HVD_REG_CLK_HVD_SW_DIV_MASK           BMASK(8:4)
    #define HVD_REG_CLK_HVD_SW_DIV_10             BITS(8:4, 10)
    #define HVD_REG_CLK_HVD_SW_DIV_30             BITS(8:4, 30)
    #define HVD_REG_CLK_HVD_IDB_SW_DIV_MASK	      BMASK(13:9)
    #define HVD_REG_CLK_HVD_IDB_SW_DIV_10         BITS(13:9, 10)
    #define HVD_REG_CLK_HVD_IDB_SW_DIV_30         BITS(13:9, 30)

/* Second bitstream registers definition */
#define HVD_REG_MODE_BS2                        (REG_HVD_BASE + ((0x0030) << 1))
    #define HVD_REG_MODE_HK_AVS_MODE_BS2        BIT(8)
    #define HVD_REG_MODE_HK_RM_MODE_BS2         BIT(9)
    #define HVD_REG_MODE_HK_RV9_DEC_MODE_BS2    BIT(10)

#define HVD_REG_ESB_ST_ADDR_L_BS2(reg_base)               (reg_base + ((0x0032) << 1))
#define HVD_REG_ESB_ST_ADDR_H_BS2(reg_base)               (reg_base + ((0x0033) << 1))

#define HVD_REG_ESB_LENGTH_L_BS2(reg_base)                (reg_base + ((0x0034) << 1))
#define HVD_REG_ESB_LENGTH_H_BS2(reg_base)                (reg_base + ((0x0035) << 1))

#define HVD_REG_ESB_RPTR_L_BS2(reg_base)                  (reg_base + ((0x0036) << 1))
#define HVD_REG_ESB_RPTR_H_BS2(reg_base)                  (reg_base + ((0x0037) << 1))

#define HVD_REG_MIF_BBU_BS2(reg_base)                     (reg_base + ((0x0038) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS_BS2       7
    #define HVD_REG_MIF_OFFSET_H_BS2            BIT(12)
    #define HVD_REG_BBU_TSP_INPUT_BS2           BIT(8)
    #define HVD_REG_BBU_PASER_MASK_BS2          (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE_BS2       0
    #define HVD_REG_BBU_PASER_ENABLE_ALL_BS2    BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03_BS2     (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB_BS2        BIT(11)

#define EVD_REG_ESB_ST_ADDR_L_BS3                            (REG_EVD_BBU3_BBU4_BASE + ((0x0002) << 1))
#define EVD_REG_ESB_ST_ADDR_H_BS3                            (REG_EVD_BBU3_BBU4_BASE + ((0x0003) << 1))

#define EVD_REG_ESB_LENGTH_L_BS3                             (REG_EVD_BBU3_BBU4_BASE + ((0x0004) << 1))
#define EVD_REG_ESB_LENGTH_H_BS3                             (REG_EVD_BBU3_BBU4_BASE + ((0x0005) << 1))

#define EVD_REG_ESB_RPTR_L_BS3                               (REG_EVD_BBU3_BBU4_BASE + ((0x0036) << 1))
#define EVD_REG_ESB_RPTR_H_BS3                               (REG_EVD_BBU3_BBU4_BASE + ((0x0037) << 1))

#define EVD_REG_MIF_BBU_BS3                                 (REG_EVD_BBU3_BBU4_BASE + ((0x0038) << 1))
    #define EVD_REG_MIF_OFFSET_L_BITS_BS3       7
    #define EVD_REG_MIF_OFFSET_H_BS3            BIT(12)
    #define EVD_REG_BBU_TSP_INPUT_BS3           BIT(8)
    #define EVD_REG_BBU_PASER_MASK_BS3          (BIT(10) | BIT(9))
    #define EVD_REG_BBU_PASER_DISABLE_BS3       0
    #define EVD_REG_BBU_PASER_ENABLE_ALL_BS3    BIT(9)
    #define EVD_REG_BBU_PASER_ENABLE_03_BS3     (BIT(9) | BIT(10))
    #define EVD_REG_BBU_AUTO_NAL_TAB_BS3        BIT(11)


#define HVD_REG_ESB_ST_ADDR_L_BS5                            (REG_HVD_BBU5_BBU6_BASE + ((0x0002) << 1))
#define HVD_REG_ESB_ST_ADDR_H_BS5                            (REG_HVD_BBU5_BBU6_BASE + ((0x0003) << 1))

#define HVD_REG_ESB_LENGTH_L_BS5                             (REG_HVD_BBU5_BBU6_BASE + ((0x0004) << 1))
#define HVD_REG_ESB_LENGTH_H_BS5                             (REG_HVD_BBU5_BBU6_BASE + ((0x0005) << 1))

#define HVD_REG_ESB_RPTR_L_BS5                               (REG_HVD_BBU5_BBU6_BASE + ((0x0036) << 1))
#define HVD_REG_ESB_RPTR_H_BS5                               (REG_HVD_BBU5_BBU6_BASE + ((0x0037) << 1))

#define HVD_REG_MIF_BBU_BS5                                 (REG_HVD_BBU5_BBU6_BASE + ((0x0038) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS_BS5       7
    #define HVD_REG_MIF_OFFSET_H_BS5            BIT(12)
    #define HVD_REG_BBU_TSP_INPUT_BS5           BIT(8)
    #define HVD_REG_BBU_PASER_MASK_BS5          (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE_BS5       0
    #define HVD_REG_BBU_PASER_ENABLE_ALL_BS5    BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03_BS5     (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB_BS5        BIT(11)





#define EVD_REG_ESB_ST_ADDR_L_BS4                            (REG_EVD_BBU3_BBU4_BASE + ((0x0032) << 1))
#define EVD_REG_ESB_ST_ADDR_H_BS4                            (REG_EVD_BBU3_BBU4_BASE + ((0x0033) << 1))

#define EVD_REG_ESB_LENGTH_L_BS4                             (REG_EVD_BBU3_BBU4_BASE + ((0x0034) << 1))
#define EVD_REG_ESB_LENGTH_H_BS4                             (REG_EVD_BBU3_BBU4_BASE + ((0x0035) << 1))

#define EVD_REG_ESB_RPTR_L_BS4                               (REG_EVD_BBU3_BBU4_BASE + ((0x0076) << 1))
#define EVD_REG_ESB_RPTR_H_BS4                               (REG_EVD_BBU3_BBU4_BASE + ((0x0077) << 1))

#define EVD_REG_MIF_BBU_BS4                                 (REG_EVD_BBU3_BBU4_BASE + ((0x0078) << 1))
    #define EVD_REG_MIF_OFFSET_L_BITS_BS4       7
    #define EVD_REG_MIF_OFFSET_H_BS4            BIT(12)
    #define EVD_REG_BBU_TSP_INPUT_BS4           BIT(8)
    #define EVD_REG_BBU_PASER_MASK_BS4          (BIT(10) | BIT(9))
    #define EVD_REG_BBU_PASER_DISABLE_BS4       0
    #define EVD_REG_BBU_PASER_ENABLE_ALL_BS4    BIT(9)
    #define EVD_REG_BBU_PASER_ENABLE_03_BS4     (BIT(9) | BIT(10))
    #define EVD_REG_BBU_AUTO_NAL_TAB_BS4        BIT(11)


#define HVD_REG_ESB_ST_ADDR_L_BS6                            (REG_HVD_BBU5_BBU6_BASE + ((0x0032) << 1))
#define HVD_REG_ESB_ST_ADDR_H_BS6                            (REG_HVD_BBU5_BBU6_BASE + ((0x0033) << 1))

#define HVD_REG_ESB_LENGTH_L_BS6                             (REG_HVD_BBU5_BBU6_BASE + ((0x0034) << 1))
#define HVD_REG_ESB_LENGTH_H_BS6                             (REG_HVD_BBU5_BBU6_BASE + ((0x0035) << 1))

#define HVD_REG_ESB_RPTR_L_BS6                               (REG_HVD_BBU5_BBU6_BASE + ((0x0076) << 1))
#define HVD_REG_ESB_RPTR_H_BS6                               (REG_HVD_BBU5_BBU6_BASE + ((0x0077) << 1))

#define HVD_REG_MIF_BBU_BS6                                 (REG_HVD_BBU5_BBU6_BASE + ((0x0078) << 1))
    #define HVD_REG_MIF_OFFSET_L_BITS_BS6       7
    #define HVD_REG_MIF_OFFSET_H_BS6            BIT(12)
    #define HVD_REG_BBU_TSP_INPUT_BS6           BIT(8)
    #define HVD_REG_BBU_PASER_MASK_BS6          (BIT(10) | BIT(9))
    #define HVD_REG_BBU_PASER_DISABLE_BS6       0
    #define HVD_REG_BBU_PASER_ENABLE_ALL_BS6    BIT(9)
    #define HVD_REG_BBU_PASER_ENABLE_03_BS6     (BIT(9) | BIT(10))
    #define HVD_REG_BBU_AUTO_NAL_TAB_BS6        BIT(11)



#define HVD_REG_NAL_TBL_ST_ADDR_L_BS2(reg_base)           (reg_base + ((0x0039) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H_BS2(reg_base)           (reg_base + ((0x003A) << 1))

#define HVD_REG_NAL_RPTR_HI_BS2(reg_base)                 (reg_base + ((0x003B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS2(reg_base)                 (reg_base + ((0x003C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS2(reg_base)                 (reg_base + ((0x003D) << 1))

#define EVD_REG_NAL_TBL_ST_ADDR_L_BS3                         (REG_EVD_BBU3_BBU4_BASE + ((0x0039) << 1))
#define EVD_REG_NAL_TBL_ST_ADDR_H_BS3                         (REG_EVD_BBU3_BBU4_BASE + ((0x003A) << 1))

#define EVD_REG_NAL_RPTR_HI_BS3                             (REG_EVD_BBU3_BBU4_BASE + ((0x003B) << 1))
#define EVD_REG_NAL_WPTR_HI_BS3                             (REG_EVD_BBU3_BBU4_BASE + ((0x003C) << 1))
#define EVD_REG_NAL_TAB_LEN_BS3                             (REG_EVD_BBU3_BBU4_BASE + ((0x003D) << 1))

#define EVD_REG_NAL_TBL_ST_ADDR_L_BS4                         (REG_EVD_BBU3_BBU4_BASE + ((0x0079) << 1))
#define EVD_REG_NAL_TBL_ST_ADDR_H_BS4                         (REG_EVD_BBU3_BBU4_BASE + ((0x007A) << 1))

#define EVD_REG_NAL_RPTR_HI_BS4                             (REG_EVD_BBU3_BBU4_BASE + ((0x007B) << 1))
#define EVD_REG_NAL_WPTR_HI_BS4                             (REG_EVD_BBU3_BBU4_BASE + ((0x007C) << 1))
#define EVD_REG_NAL_TAB_LEN_BS4                             (REG_EVD_BBU3_BBU4_BASE + ((0x007D) << 1))


#define HVD_REG_NAL_TBL_ST_ADDR_L_BS5                         (REG_HVD_BBU5_BBU6_BASE + ((0x0039) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H_BS5                         (REG_HVD_BBU5_BBU6_BASE + ((0x003A) << 1))

#define HVD_REG_NAL_RPTR_HI_BS5                             (REG_HVD_BBU5_BBU6_BASE + ((0x003B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS5                             (REG_HVD_BBU5_BBU6_BASE + ((0x003C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS5                             (REG_HVD_BBU5_BBU6_BASE + ((0x003D) << 1))

#define HVD_REG_NAL_TBL_ST_ADDR_L_BS6                         (REG_HVD_BBU5_BBU6_BASE + ((0x0079) << 1))
#define HVD_REG_NAL_TBL_ST_ADDR_H_BS6                         (REG_HVD_BBU5_BBU6_BASE + ((0x007A) << 1))

#define HVD_REG_NAL_RPTR_HI_BS6                             (REG_HVD_BBU5_BBU6_BASE + ((0x007B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS6                             (REG_HVD_BBU5_BBU6_BASE + ((0x007C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS6                             (REG_HVD_BBU5_BBU6_BASE + ((0x007D) << 1))


#define HVD_REG_ESB_WPTR_L_BS2                 (REG_HVD_BASE + ((0x003E) << 1))
#define HVD_REG_ESB_WPTR_H_BS2                 (REG_HVD_BASE + ((0x003F) << 1))

/* VP8 Registers */
#define HVD_REG_HK_VP8                          (REG_HVD_BASE + ((0x0040) << 1))
    #define HVD_REG_HK_VP8_DEC_MODE             BIT(0)
    #define HVD_REG_HK_PLAYER_FM                BIT(1)

#define HVD_REG_ESB_ST_ADR_L_BS34               (REG_HVD_BASE + ((0x0042) << 1))
#define HVD_REG_ESB_ST_ADR_H_BS34               (REG_HVD_BASE + ((0x0043) << 1))
#define HVD_REG_ESB_LENGTH_L_BS34               (REG_HVD_BASE + ((0x0044) << 1))
#define HVD_REG_ESB_LENGTH_H_BS34               (REG_HVD_BASE + ((0x0045) << 1))


#define EVD_REG_MIF_SOURCE_GROUP0               (REG_MIU_SOURCE_BASE + ((0x0078) << 1))
#define EVD_REG_MIF_SOURCE_GROUP1               (REG_MIU_SOURCE_BASE + ((0x0079) << 1))
#define EVD_REG_MIF_SOURCE_GROUP2               (REG_MIU_SOURCE_BASE + ((0x007A) << 1))
#define EVD_REG_MIF_SOURCE_GROUP3               (REG_MIU_SOURCE_BASE + ((0x007B) << 1))
#define EVD_REG_MIF_SOURCE_GROUP4               (REG_MIU_SOURCE_BASE + ((0x007C) << 1))
#define EVD_REG_MIF_SOURCE_GROUP5               (REG_MIU_SOURCE_BASE + ((0x007B) << 1))



#define HVD_REG_MIF_BS34                        (REG_HVD_BASE + ((0x0048) << 1))
    #define HVD_REG_BS34_MIF_OFFSET_L_BITS       7
    #define HVD_REG_BS34_MIF_OFFSET_H            BIT(12)
    #define HVD_REG_BS34_TSP_INPUT               BIT(8)
    #define HVD_REG_BS34_PASER_MASK              (BIT(10) | BIT(9))
    #define HVD_REG_BS34_PASER_DISABLE           0
    #define HVD_REG_BS34_PASER_ENABLE_ALL        BIT(9)
    #define HVD_REG_BS34_PASER_ENABLE_03         (BIT(9) | BIT(10))
    #define HVD_REG_BS34_AUTO_NAL_TAB            BIT(11)
    #define HVD_REG_BS34_NAL_BUF_SKIP            BIT(13)
    #define HVD_REG_BS34_NAL_BUF_SKIP_RDY        BIT(14)

#define HVD_REG_NAL_TAB_ST_L_BS3                 (REG_HVD_BASE + ((0x0049) << 1))
#define HVD_REG_NAL_TAB_ST_H_BS3                 (REG_HVD_BASE + ((0x004A) << 1))
#define HVD_REG_NAL_RPTR_HI_BS3                  (REG_HVD_BASE + ((0x004B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS3                  (REG_HVD_BASE + ((0x004C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS3                  (REG_HVD_BASE + ((0x004D) << 1))
#define HVD_REG_NAL_TAB_ST_L_BS4                 (REG_HVD_BASE + ((0x0059) << 1))
#define HVD_REG_NAL_TAB_ST_H_BS4                 (REG_HVD_BASE + ((0x005A) << 1))
#define HVD_REG_NAL_RPTR_HI_BS4                  (REG_HVD_BASE + ((0x005B) << 1))
#define HVD_REG_NAL_WPTR_HI_BS4                  (REG_HVD_BASE + ((0x005C) << 1))
#define HVD_REG_NAL_TAB_LEN_BS4                  (REG_HVD_BASE + ((0x005D) << 1))

//------------------------------------------------------------------------------
// EVD Reg
//------------------------------------------------------------------------------
#define REG_EVDPLL_BASE                         (0x10900)
#define REG_EVDPLL_PD                           (REG_EVDPLL_BASE + ((0x0041) << 1))
    #define REG_EVDPLL_PD_DIS                   BIT(8)

#define REG_EVDPLL_LOOP_DIV_SECOND                (REG_EVDPLL_BASE+(0x0043<<1))
    #define REG_EVDPLL_LOOP_DIV_SECOND_MASK       BMASK(7:0)
    #define REG_EVDPLL_LOOP_DIV_SECOND_456MHZ     BITS(7:0, 19)

#define EVD_REG_RESET                           (REG_EVD_BASE + ((0x0001) << 1))
    #define EVD_REG_RESET_SWRST                 BIT(0)
    #define EVD_REG_RESET_SWRST_FIN             BIT(2)
    #define EVD_REG_RESET_STOP_BBU              BIT(3)
    #define EVD_REG_RESET_MIU_RDY               BIT(4)
    #define EVD_REG_RESET_MIU1_128              BIT(5)
    #define EVD_REG_RESET_MIU1_256              BIT(6)
    #define EVD_REG_RESET_USE_HVD_MIU_EN        BIT(7)
    #define EVD_REG_RESET_HK_HEVC_MODE          BIT(8)
    #define EVD_REG_RESET_HK_TSP2EVD_EN         BIT(9)
    #define EVD_REG_RESET_MIU0_256              BIT(10)
    #define EVD_REG_RESET_MIU0_128              BIT(11)
    #define EVD_REG_RESET_CPUIF_SEL             BIT(12)
    #define EVD_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define EVD_REG_RESET_BOND_UHD              BIT(14)
    #define EVD_REG_RESET_BOND_HD               BIT(15)

#define EVD_BBU23_SETTING                  (REG_EVD_BASE + ((0x0001B) << 1))
    #define REG_TSP2EVD_EN_BS3              BIT(9)
    #define REG_TSP2EVD_EN_BS4              BIT(10)


#define REG_CLK_EVD                             (REG_EVD_BASE + ((0x002d) << 1))
    #define REG_CLK_EVD_SW_OV_EN                BIT(0)
    #define REG_CLK_EVD_SW_UPD                  BIT(1)
    #define REG_CLK_EVD_PPU_SW_OV_EN            BIT(2)
    #define REG_CLK_EVD_PPU_SW_UPD              BIT(3)
    #define REG_CLK_EVD_SW_DIV_MASK             BMASK(8:4)
    #define REG_CLK_EVD_SW_DIV_10               BITS(8:4, 10)
    #define REG_CLK_EVD_SW_DIV_30               BITS(8:4, 30)
    #define REG_CLK_EVD_PPU_SW_DIV_MASK         BMASK(13:9)
    #define REG_CLK_EVD_PPU_SW_DIV_10           BITS(13:9, 10)
    #define REG_CLK_EVD_PPU_SW_DIV_30           BITS(13:9, 30)



#define EVD_BBU_MIU_SETTING                    (REG_EVD_BASE + ((0x00040) << 1))
    #define REG_BBU_MIU_128                    BIT(0)
    #define REG_BBU_MIU_256                    BIT(1)

//------------------------------------------------------------------------------
// G2 VP9 Reg
//------------------------------------------------------------------------------
#define VP9_REG_RESET                           (REG_G2VP9_BASE + ((0x0001) << 1))
    #define VP9_REG_RESET_SWRST                 BIT(0)
    #define VP9_REG_RESET_SWRST_FIN             BIT(2)
    #define VP9_REG_RESET_MIU_RDY               BIT(4)
    #define VP9_REG_RESET_ALL_SRAM_SD_EN        BIT(13)
    #define VP9_REG_RESET_APB_SEL               BIT(15)

#define EVD_REG_VP9_MODE                        (REG_EVD_BASE + ((0x001b) << 1))
    #define EVD_REG_SET_VP9_MODE                BIT(0)


//------------------------------------------------------------------------------
// ChipTop Reg
//------------------------------------------------------------------------------

#define CHIPTOP_REG_BASE               (0x1E00 )
#define CLKGEN0_REG_BASE               (0x0B00 )
#define CLKGEN2_REG_BASE               (0x0A00)


#define REG_TOP_PSRAM0_1_MIUMUX            (CHIPTOP_REG_BASE+(0x002D<<1))   //TODO
    #define TOP_CKG_PSRAM0_MASK                 BMASK(1:0)
    #define TOP_CKG_PSRAM0_DIS                  BIT(0)
    #define TOP_CKG_PSRAM0_INV                  BIT(1)
    #define TOP_CKG_PSRAM1_MASK                 BMASK(3:2)
    #define TOP_CKG_PSRAM1_DIS                  BIT(0)
    #define TOP_CKG_PSRAM1_INV                  BIT(1)
    #define TOP_MIU_MUX_G07_MASK                BMASK(7:6)
	#define TOP_MIU_MUX_G07_OD_LSB_R            BITS(7:6,0)
	#define TOP_MIU_MUX_G07_GOP2_R              BITS(7:6,1)
    #define TOP_MIU_MUX_G08_MASK                BMASK(9:8)
	#define TOP_MIU_MUX_G08_OD_LSB_W            BITS(9:8,0)
	#define TOP_MIU_MUX_G08_VE_W                BITS(9:8,1)
    #define TOP_MIU_MUX_G15_MASK                BMASK(11:10)
	#define TOP_MIU_MUX_G15_GOP2_R              BITS(11:10,0)
	#define TOP_MIU_MUX_G15_OD_LSB_R            BITS(11:10,1)
    #define TOP_MIU_MUX_G1A_MASK                BMASK(13:12)
	#define TOP_MIU_MUX_G1A_VE_W                BITS(13:12,0)
	#define TOP_MIU_MUX_G1A_OD_LSB_W            BITS(13:12,1)
    #define TOP_MIU_MUX_G26_MASK                BMASK(15:14)
	#define TOP_MIU_MUX_G26_RVD_RW              BITS(15:14,0)
	#define TOP_MIU_MUX_G26_SVD_INTP_R          BITS(15:14,1)
	#define TOP_MIU_MUX_G26_MVD_R               BITS(15:14,2)

#define REG_TOP_VPU             (CLKGEN0_REG_BASE+(0x0030<<1))
    #define TOP_CKG_VPU_MASK                  BMASK(6:0)
    #define TOP_CKG_VPU_DIS                   BIT(0)
    #define TOP_CKG_VPU_INV                   BIT(1)
    #define TOP_CKG_VPU_CLK_MASK              BMASK(6:2)
    #define TOP_CKG_VPU_480MHZ                BITS(6:2, 3)
    #define TOP_CKG_VPU_432MHZ                BITS(6:2, 6)
    #define TOP_CKG_VPU_384MHZ                BITS(6:2, 7)
    #define TOP_CKG_VPU_ICG_EN                BIT(8)
    #define TOP_CKG_VPU_LITE_ICG_EN           BIT(9)

#define REG_TOP_HVD             (CLKGEN0_REG_BASE+(0x0034<<1))
    #define TOP_CKG_HVD_MASK                  BMASK(4:0)
    #define TOP_CKG_HVD_DIS                   BIT(0)
    #define TOP_CKG_HVD_INV                   BIT(1)
    #define TOP_CKG_HVD_CLK_MASK              BMASK(4:2)
    #define TOP_CKG_HVD_384MHZ                BITS(4:2, 0)  // default use this
    #define TOP_CKG_HVD_288MHZ                BITS(4:2, 3)
    #define TOP_CKG_HVD_432MHZ                BITS(4:2, 7)  // for overclocking


#define REG_TOP_VP9             (CLKGEN0_REG_BASE+(0x0032<<1))
    #define TOP_CKG_VP9_MASK                  BMASK(8:4)
    #define TOP_CKG_VP9_DIS                   BIT(4)
    #define TOP_CKG_VP9_INV                   BIT(5)
    #define TOP_CKG_VP9_CLK_MASK              BMASK(8:6)
    #define TOP_CKG_VP9_432MHZ                BITS(8:6,0)
    #define TOP_CKG_VP9_384MHZ                BITS(8:6,1)
    #define TOP_CKG_VP9_345MHZ                BITS(8:6,2)
    #define TOP_CKG_VP9_320MHZ                BITS(8:6,3)
    #define TOP_CKG_VP9_288MHZ                BITS(8:6,4)
    #define TOP_CKG_VP9_240MHZ                BITS(8:6,5)
    #define TOP_CKG_VP9_216MHZ                BITS(8:6,6)
    #define TOP_CKG_VP9_172MHZ                BITS(8:6,7)

#define REG_TOP_MVD             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD_MASK                  BMASK(3:0)
    #define TOP_CKG_MHVD_DIS                  BIT(0)
    #define TOP_CKG_MVD_INV                   BIT(1)
    #define TOP_CKG_MVD_CLK_MASK              BMASK(3:2)
    #define TOP_CKG_MVD_144MHZ                BITS(3:2, 0)
    #define TOP_CKG_MVD_123MHZ                BITS(3:2, 1)
    #define TOP_CKG_MVD_MIU                   BITS(3:2, 2)
    #define TOP_CKG_MVD_XTAL                  BITS(3:2, 3)

#define REG_TOP_MVD2             (CLKGEN0_REG_BASE+(0x0039<<1))
    #define TOP_CKG_MVD2_MASK                  BMASK(11:8)
    #define TOP_CKG_MHVD2_DIS                  BIT(8)
    #define TOP_CKG_MVD2_INV                   BIT(9)
    #define TOP_CKG_MVD2_CLK_MASK              BMASK(11:10)
    #define TOP_CKG_MVD2_170MHZ                BITS(11:10, 0)
    #define TOP_CKG_MVD2_144MHZ                BITS(11:10, 1)
    #define TOP_CKG_MVD2_160MHZ                BITS(11:10, 1)
    #define TOP_CKG_MVD2_CLK_MIU_P             BITS(11:10, 1)

#define REG_TOP_CKG_EVD_PPU         (CLKGEN2_REG_BASE+(0x001c<<1))
    #define TOP_CKG_EVD_PPU_MASK                BMASK(4:2)
    #define TOP_CKG_EVD_PPU_DIS                 BIT(0)
    #define TOP_CKG_EVD_PPU_INV                 BIT(1)
    #define TOP_CKG_EVD_PPU_PLL_BUF             BITS(4:2, 0)
    #define TOP_CKG_EVD_PPU_MIU128PLL           BITS(4:2, 1)
    #define TOP_CKG_EVD_PPU_MIU256PLL           BITS(4:2, 2)
    #define TOP_CKG_EVD_PPU_480MHZ              BITS(4:2, 3)
    #define TOP_CKG_EVD_PPU_384MHZ              BITS(4:2, 4)
    #define TOP_CKG_EVD_PPU_320MHZ              BITS(4:2, 5)
    #define TOP_CKG_EVD_PPU_240MHZ              BITS(4:2, 6)
    #define TOP_CKG_EVD_PPU_192MHZ              BITS(4:2, 7)

#define REG_TOP_CKG_EVD             (CLKGEN0_REG_BASE+(0x003d<<1))
    #define TOP_CKG_EVD_MASK                    BMASK(4:2)
    #define TOP_CKG_EVD_DIS                     BIT(0)
    #define TOP_CKG_EVD_INV                     BIT(1)
    #define TOP_CKG_EVD_PLL_BUF                 BITS(4:2, 0)
    #define TOP_CKG_EVD_MIU128PLL               BITS(4:2, 1)
    #define TOP_CKG_EVD_MIU256PLL               BITS(4:2, 2)
    #define TOP_CKG_EVD_480MHZ                  BITS(4:2, 3)
    #define TOP_CKG_EVD_384MHZ                  BITS(4:2, 4)
    #define TOP_CKG_EVD_320MHZ                  BITS(4:2, 5)
    #define TOP_CKG_EVD_240MHZ                  BITS(4:2, 6)
    #define TOP_CKG_EVD_192MHZ                  BITS(4:2, 7)


#define REG_TOP_UART_SEL0             (CHIPTOP_REG_BASE+(0x0053<<1))
    #define REG_TOP_UART_SEL_0_MASK            BMASK(3:0)
    #define REG_TOP_UART_SEL_MHEG5             BITS(3:0, 1)
    #define REG_TOP_UART_SEL_VD_MHEG5          BITS(3:0, 2)
    #define REG_TOP_UART_SEL_TSP               BITS(3:0, 3)
    #define REG_TOP_UART_SEL_PIU_0             BITS(3:0, 4)
    #define REG_TOP_UART_SEL_PIU_1             BITS(3:0, 5)
    #define REG_TOP_UART_SEL_PIU_FAST          BITS(3:0, 7)
    #define REG_TOP_UART_SEL_VD_R2_LITE        BITS(3:0, 8)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD0    BITS(3:0, 10)
    #define REG_TOP_UART_SEL_VD_MCU_51_TXD1    BITS(3:0, 11)

#define REG_TOP_HVD_AEC_LITE         (CLKGEN2_REG_BASE+(0x0018<<1))
    #define TOP_CKG_HVD_AEC_LITE_MASK           BMASK(4:0)
    #define TOP_CKG_HVD_AEC_LITE_DIS            BIT(0)
    #define TOP_CKG_HVD_AEC_LITE_INV            BIT(1)
    #define TOP_CKG_HVD_AEC_LITE_CLK_MASK       BMASK(3:2)
    #define TOP_CKG_HVD_AEC_LITE_288MHZ         BITS(3:2, 0)  //default use this
    #define TOP_CKG_HVD_AEC_LITE_240MHZ         BITS(3:2, 1)
    #define TOP_CKG_HVD_AEC_LITE_216MHZ         BITS(3:2, 2)
    #define TOP_CKG_HVD_AEC_LITE_320MHZ         BITS(3:2, 3)

#define REG_TOP_HVD_IDB              (CLKGEN2_REG_BASE+(0x001a<<1))
    #define TOP_CKG_HVD_IDB_CLK_MASK            BMASK(2:0)
    #define TOP_CKG_HVD_IDB_432MHZ              BITS(2:0, 0)  // default use this
    #define TOP_CKG_HVD_IDB_384MHZ              BITS(2:0, 1)
    #define TOP_CKG_HVD_IDB_480MHZ              BITS(2:0, 3)  // for overclocking

#define REG_TOP_HVD_AEC              (CLKGEN2_REG_BASE+(0x001b<<1))
    #define TOP_CKG_HVD_AEC_MASK                BMASK(4:0)
    #define TOP_CKG_HVD_AEC_DIS                 BIT(0)
    #define TOP_CKG_HVD_AEC_INV                 BIT(1)
    #define TOP_CKG_HVD_AEC_CLK_MASK            BMASK(3:2)
    #define TOP_CKG_HVD_AEC_288MHZ              BITS(3:2, 0)  //default use this
    #define TOP_CKG_HVD_AEC_240MHZ              BITS(3:2, 1)
    #define TOP_CKG_HVD_AEC_216MHZ              BITS(3:2, 2)
    #define TOP_CKG_HVD_AEC_320MHZ              BITS(3:2, 3)  // for overclocking

#define REG_TOP_VP8                  (CLKGEN2_REG_BASE+(0x001d<<1))
    #define TOP_CKG_VP8_MASK                    BMASK(3:0)
    #define TOP_CKG_VP8_DIS                     BIT(0)
    #define TOP_CKG_VP8_INV                     BIT(1)
    #define TOP_CKG_VP8_CLK_MASK                BMASK(3:2)
    #define TOP_CKG_VP8_216MHZ                  BITS(3:2, 0)  // default use this
    #define TOP_CKG_VP8_172MHZ                  BITS(3:2, 1)
    #define TOP_CKG_VP8_144MHZ                  BITS(3:2, 2)
    #define TOP_CKG_VP8_240MHZ                  BITS(3:2, 3)  // for overclocking



//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
#define MIU0_REG_HVD_BASE             	(0x1200)
#define MIU0_REG_HVD_BASE2             	(0x61500)

#define MIU1_REG_HVD_BASE             	(0x0600)
#define MIU1_REG_HVD_BASE2             	(0x62200)

//#define MIU2_REG_HVD_BASE             	(0x62000)
//#define MIU2_REG_HVD_BASE2             	(0x62300)


#define MIU0_CLIENT_SELECT_GP4          (MIU0_REG_HVD_BASE + (0x007C<<1))
    #define MIU0_CLIENT_SELECT_GP4_HVD_MIF0   BIT(4)
    #define MIU0_CLIENT_SELECT_GP4_HVD_MIF1   BIT(5)
/*
#define MIU2_CLIENT_SELECT_GP4          (MIU2_REG_HVD_BASE + (0x007C<<1))
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF0   BIT(4)
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF1   BIT(5)
*/
/*
#define MIU2_CLIENT_SELECT_GP4          (MIU2_REG_HVD_BASE + (0x007C<<1))
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF0   BIT(2)
    #define MIU2_CLIENT_SELECT_GP4_HVD_MIF1   BIT(3)
    #define MIU2_CLIENT_SELECT_GP4_HVD_MALI   BIT(4)
*/

//#define MIU2_REG_HVD_BASE             	(0x62000)
//#define MIU2_REG_HVD_BASE2             	(0x62300)



#define MIU0_REG_RQ0_MASK                 (MIU0_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU0_REG_RQ1_MASK                 (MIU0_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU0_REG_RQ2_MASK                 (MIU0_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU0_REG_RQ3_MASK                 (MIU0_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU0_REG_RQ4_MASK                 (MIU0_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU0_REG_RQ5_MASK                 (MIU0_REG_HVD_BASE2+(( 0x0013)<<1))

#define MIU1_REG_RQ0_MASK                 (MIU1_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU1_REG_RQ1_MASK                 (MIU1_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU1_REG_RQ2_MASK                 (MIU1_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU1_REG_RQ3_MASK                 (MIU1_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU1_REG_RQ4_MASK                 (MIU1_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU1_REG_RQ5_MASK                 (MIU1_REG_HVD_BASE2+(( 0x0013)<<1))

/*
#define MIU2_REG_RQ0_MASK                 (MIU2_REG_HVD_BASE+(( 0x0023)<<1))
#define MIU2_REG_RQ1_MASK                 (MIU2_REG_HVD_BASE+(( 0x0033)<<1))
#define MIU2_REG_RQ2_MASK                 (MIU2_REG_HVD_BASE+(( 0x0043)<<1))
#define MIU2_REG_RQ3_MASK                 (MIU2_REG_HVD_BASE+(( 0x0053)<<1))
#define MIU2_REG_RQ4_MASK                 (MIU2_REG_HVD_BASE2+(( 0x0003)<<1))
#define MIU2_REG_RQ5_MASK                 (MIU2_REG_HVD_BASE2+(( 0x0013)<<1))
*/



#define MIU0_REG_SEL0                 (MIU0_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU0_REG_SEL1                 (MIU0_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU0_REG_SEL2                 (MIU0_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU0_REG_SEL3                 (MIU0_REG_HVD_BASE+(( 0x007B)<<1))
#define MIU0_REG_SEL4                 (MIU0_REG_HVD_BASE+(( 0x007C)<<1))
#define MIU0_REG_SEL5                 (MIU0_REG_HVD_BASE+(( 0x007D)<<1))
/*
#define MIU2_REG_SEL0                 (MIU2_REG_HVD_BASE+(( 0x0078)<<1))
#define MIU2_REG_SEL1                 (MIU2_REG_HVD_BASE+(( 0x0079)<<1))
#define MIU2_REG_SEL2                 (MIU2_REG_HVD_BASE+(( 0x007A)<<1))
#define MIU2_REG_SEL3                 (MIU2_REG_HVD_BASE+(( 0x007B)<<1))
#define MIU2_REG_SEL4                 (MIU2_REG_HVD_BASE+(( 0x007C)<<1))
#define MIU2_REG_SEL5                 (MIU2_REG_HVD_BASE+(( 0x007D)<<1))
*/

//#define MIU1_REG_SEL0                 (MIU1_REG_HVD_BASE+(( 0x0078)<<1))


#define MIU_HVD_RW      (BIT(10)|BIT(11))
#define MIU_MVD_RW      (BIT(5)|BIT(6))

//------------------------------------------------------------------------------
// SRAM Reg
//------------------------------------------------------------------------------

#ifdef CONFIG_MSTAR_SRAMPD
#define REG_PATGEN_HI_BASE                      0x71300
#define REG_PATGEN_VP9_BASE                     0x71800

#define REG_HICODEC_SRAM_SD_EN              (REG_PATGEN_HI_BASE+(( 0x0010)<<1))
    #define HICODEC_SRAM_HICODEC0               BIT(0)
    #define HICODEC_SRAM_HICODEC1               BIT(1)

#define REG_HICODEC_LITE_SRAM_SD_EN         (REG_PATGEN_VP9_BASE+(( 0x0010)<<1))
    #define HICODEC_LITE_SRAM_HICODEC0          BIT(0)
    #define HICODEC_LITE_SRAM_HICODEC1          BIT(1)
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_HVD_H_
