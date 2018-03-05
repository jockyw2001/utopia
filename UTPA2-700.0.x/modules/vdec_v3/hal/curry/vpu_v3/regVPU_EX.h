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
/// file    regVPU.h
/// @brief  VPU Module Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_VPU_H_
#define _REG_VPU_H_

//#include "MsCommon.h"

#ifndef BMASK
#define BIT(_bit_)                  (1 << (_bit_))
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))
#endif

#ifndef READ_BYTE
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
#endif

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//*****************************************************************************
// RIU macro
#define VPU_MACRO_START     do {
#define VPU_MACRO_END       } while (0)
#define VPU_RIU_BASE        u32VPURegOSBase

#define VPU_HIGHBYTE(u16)               ((MS_U8)((u16) >> 8))
#define VPU_LOWBYTE(u16)                ((MS_U8)(u16))
#define VPU_RIU_READ_BYTE(addr)   ( READ_BYTE( VPU_RIU_BASE + (addr) ) )
#define VPU_RIU_READ_WORD(addr)   ( READ_WORD( VPU_RIU_BASE + (addr) ) )
#define VPU_RIU_WRITE_BYTE(addr, val)      { WRITE_BYTE( VPU_RIU_BASE+(addr), val); }
#define VPU_RIU_WRITE_WORD(addr, val)      { WRITE_WORD( VPU_RIU_BASE+(addr), val); }


#define _VPU_ReadByte( u32Reg )   VPU_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define _VPU_Read2Byte( u32Reg )    (VPU_RIU_READ_WORD((u32Reg)<<1))

#define _VPU_Read4Byte( u32Reg )   ( (MS_U32)VPU_RIU_READ_WORD((u32Reg)<<1) | ((MS_U32)VPU_RIU_READ_WORD(((u32Reg)+2)<<1)<<16 )  )

#define _VPU_ReadRegBit( u32Reg, u8Mask )   (VPU_RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define _VPU_ReadWordBit( u32Reg, u16Mask )   (_VPU_Read2Byte( u32Reg ) & (u16Mask))

#define _VPU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (VPU_RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (VPU_RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    VPU_MACRO_END

#define _VPU_WriteByte( u32Reg, u8Val )                                                 \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);   \
    VPU_MACRO_END

#define _VPU_Write2Byte( u32Reg, u16Val )                                               \
    VPU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                               \
        VPU_RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                  \
        VPU_RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        VPU_RIU_WRITE_WORD( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    VPU_MACRO_END

#define _VPU_Write3Byte( u32Reg, u32Val )   \
    if ((u32Reg) & 0x01)                                                                \
    {                                                                                               \
        VPU_RIU_WRITE_BYTE((u32Reg << 1) - 1, u32Val);                                    \
        VPU_RIU_WRITE_WORD( (u32Reg + 1)<<1 , ((u32Val) >> 8));                                      \
    }                                                                                           \
    else                                                                                        \
    {                                                                                               \
        VPU_RIU_WRITE_WORD( (u32Reg) << 1,  u32Val);                                                         \
        VPU_RIU_WRITE_BYTE( (u32Reg + 2) << 1 ,  ((u32Val) >> 16));                             \
    }

#define _VPU_Write4Byte( u32Reg, u32Val )                                               \
    VPU_MACRO_START                                                                     \
    if ((u32Reg) & 0x01)                                                      \
    {                                                                                               \
        VPU_RIU_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  u32Val);                                         \
        VPU_RIU_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8));                                      \
        VPU_RIU_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24));                           \
    }                                                                                               \
    else                                                                                                \
    {                                                                                                   \
        VPU_RIU_WRITE_WORD( (u32Reg) <<1 ,  u32Val);                                                             \
        VPU_RIU_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16));                                             \
    }                                                                     \
    VPU_MACRO_END

#define _VPU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    VPU_MACRO_START                                                                     \
    VPU_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (VPU_RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    VPU_MACRO_END

#define _VPU_WriteWordMask( u32Reg, u16Val , u16Msk)                                               \
    VPU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                        \
    {                                                                                           \
        if ((u16Msk)&0xff00) _VPU_WriteByteMask( ((u32Reg)+1) , (((u16Val) & 0xff00)>>8) , (((u16Msk)&0xff00)>>8) );    \
        _VPU_WriteByteMask( (u32Reg) , ((u16Val) & 0x00ff) , ((u16Msk)&0x00ff) );                                                                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        VPU_RIU_WRITE_WORD( ((u32Reg)<<1) ,  (((u16Val) & (u16Msk))  | (_VPU_Read2Byte( u32Reg  ) & (~( u16Msk ))))  );                                                       \
    }                                                                               \
    VPU_MACRO_END



//------------------------------------------------------------------------------
// VPU Reg
//------------------------------------------------------------------------------
//#define EVDR2
#ifdef EVDR2
#define REG_VPU_BASE           (0x63300)
#define REG_MBX_BASE           (0x63400)
#define REG_MAU1_BASE          (0x63400)
#define REG_MAU1_LV2_0_BASE    (0x63700)
#define REG_MAU1_LV2_1_BASE    (0x63800)
#else
#define REG_VPU_BASE            (0x0300)
#define REG_MBX_BASE            (0x0400)
#define REG_MAU1_BASE           (0x0400)
#define REG_MAU1_LV2_0_BASE    (0x63500)
#define REG_MAU1_LV2_1_BASE    (0x63600)
#endif

//write back stage PC
#define VPU_REG_EXPC_L          (REG_VPU_BASE+(0x000a<<1))
#define VPU_REG_EXPC_H          (REG_VPU_BASE+(0x000b<<1))

#define VPU_REG_CPU_STATUS      (REG_VPU_BASE+( 0x000f<<1))
    #define VPU_REG_CPU_D_REPLAY            BIT(8)
    #define VPU_REG_CPU_I_EMPTY             BIT(6)

#define VPU_REG_MIU_LAST        (REG_VPU_BASE+( 0x0020<<1))
    #define VPU_REG_MIU_LAST_EN             BIT(5)

#define VPU_REG_ICU_STATUS      (REG_VPU_BASE+( 0x001f<<1))
    #define VPU_REG_ICU_IDLE                BIT(1)
    #define VPU_REG_ISB_IDLE                BIT(8)

#define VPU_REG_ICU_DBG_SEL     (REG_VPU_BASE+( 0x0010<<1))
#define VPU_REG_ICU_DBG_DAT0    (REG_VPU_BASE+( 0x0014<<1))
#define VPU_REG_ICPU_REQ                    BIT(14)

#define VPU_REG_DCU_DBG_SEL     (REG_VPU_BASE+( 0x0028<<1))
    #define VPU_REG_DCU_DBG_SEL_0	    BIT(2)
    #define VPU_REG_DCU_DBG_SEL_1	    BIT(3)
#define VPU_REG_DCU_STATUS      (REG_VPU_BASE+( 0x0029<<1))
    #define VPU_REG_BIU_EMPTY               BIT(11)

#define VPU_REG_CPU_SETTING     (REG_VPU_BASE+( 0x0040<<1))
    #define VPU_REG_CPU_R2_EN               BIT(0)
    #define VPU_REG_CPU_SW_RSTZ             BIT(1)
    #define VPU_REG_CPU_MIU_SW_RSTZ         BIT(2)
    #define VPU_REG_CPU_RIU_SW_RSTZ         BIT(3)
    #define VPU_REG_CPU_SPI_BOOT            BIT(4)
    #define VPU_REG_CPU_SDRAM_BOOT          BIT(5)
    #define VPU_REG_CPU_R2_INTO             BIT(6)
    #define VPU_REG_CPU_DBG_ON_DCU          BIT(8)
    #define VPU_REG_CPU_DBG_ON_ICU          BIT(9)
    #define VPU_REG_CPU_DBG_CLK_SEL         BIT(10)
    #define VPU_REG_CPU_DBG_CLK_TOGGLE      BIT(11)

#define VPU_REG_ICU_SDR_BASE_L  (REG_VPU_BASE+(0x0041<<1))  //byte address
#define VPU_REG_ICU_SDR_BASE_H    (REG_VPU_BASE+(0x0042<<1))
#define VPU_REG_DCU_SDR_BASE_L  (REG_VPU_BASE+(0x0043<<1))  //byte address
#define VPU_REG_DCU_SDR_BASE_H  (REG_VPU_BASE+(0x0044<<1))

#define VPU_REG_SPI_BASE     (REG_VPU_BASE+(0x0048<<1))    //REG ACCESS BASE32


#define VPU_REG_IQMEM_BASE_L    (REG_VPU_BASE+(0x0049<<1))
#define VPU_REG_IQMEM_BASE_H    (REG_VPU_BASE+(0x004a<<1))
#define VPU_REG_IQMEM_MASK_L    (REG_VPU_BASE+(0x004b<<1))
#define VPU_REG_IQMEM_MASK_H    (REG_VPU_BASE+(0x004c<<1))

#define VPU_REG_DQMEM_BASE_L    (REG_VPU_BASE+(0x004d<<1))
#define VPU_REG_DQMEM_BASE_H    (REG_VPU_BASE+(0x004e<<1))
#define VPU_REG_DQMEM_MASK_L    (REG_VPU_BASE+(0x004f<<1))
#define VPU_REG_DQMEM_MASK_H    (REG_VPU_BASE+(0x0050<<1))

#define VPU_REG_PQMEM_BASE_L    (REG_VPU_BASE+(0x0051<<1))
#define VPU_REG_PQMEM_BASE_H    (REG_VPU_BASE+(0x0052<<1))
#define VPU_REG_PQMEM_MASK_L    (REG_VPU_BASE+(0x0053<<1))
#define VPU_REG_PQMEM_MASK_H    (REG_VPU_BASE+(0x0054<<1))
#define VPU_REG_IQMEM_SETTING   (REG_VPU_BASE+(0x0064<<1))
#define VPU_REG_VQMEM_BASE_L    (REG_VPU_BASE+(0x0067<<1))
#define VPU_REG_VQMEM_BASE_H    (REG_VPU_BASE+(0x0068<<1))
#define VPU_REG_VQMEM_MASK_L    (REG_VPU_BASE+(0x0069<<1))
#define VPU_REG_VQMEM_MASK_H    (REG_VPU_BASE+(0x006a<<1))
#define VPU_REG_CPU_CONFIG      (REG_VPU_BASE+(0x006b<<1))
    #define VPU_REG_CPU_STALL_EN            BIT(15)

#define VPU_REG_QMEM_OWNER      (REG_VPU_BASE+(0x006c<<1))

#define VPU_REG_IO0_BASE     (REG_VPU_BASE+(0x0045<<1))   //RIU
#define VPU_REG_IO1_BASE     (REG_VPU_BASE+(0x0055<<1))   //R2 internal UART
#define VPU_REG_IO2_BASE     (REG_VPU_BASE+(0x0056<<1))   //R2 read SPI
#define VPU_REG_IO3_BASE     (REG_VPU_BASE+(0x0057<<1))   //IP use
#define VPU_REG_CONTROL_SET  (REG_VPU_BASE+(0x0058<<1))

#define VPU_REG_R2_MI_SEL_BASE  (REG_VPU_BASE+(0x0072<<1))

    #define VPU_REG_IO0_EN           BIT(0)  //default Enable
    #define VPU_REG_IO1_EN           BIT(1)  //default Enable
    #define VPU_REG_IO2_EN           BIT(2)
    #define VPU_REG_IO3_EN           BIT(3)
    #define VPU_REG_QMEM_SPACE_EN    BIT(4)
    #define VPU_REG_MMU_IO_EN        BIT(5)
    #define VPU_REG_WMB_FORCE_OFF    BIT(6)
    #define VPU_REG_WMB_AUTO_OFF     BIT(7)
    #define VPU_REG_PQMEM_SPACE_EN   BIT(13)
    #define VPU_REG_VQMEM_SPACE_EN   BIT(14)


#define VPU_REG_VERSION                        (REG_MBX_BASE+(0x0055<<1))

#define VPU_REG_HI_MBOX0_L                     (REG_MBX_BASE+(0x005b<<1))
#define VPU_REG_HI_MBOX0_H                     (REG_MBX_BASE+(0x005c<<1))
#define VPU_REG_HI_MBOX1_L                     (REG_MBX_BASE+(0x005d<<1))
#define VPU_REG_HI_MBOX1_H                     (REG_MBX_BASE+(0x005e<<1))

#define VPU_REG_HI_MBOX_SET                    (REG_MBX_BASE+(0x005f<<1))
    #define VPU_REG_HI_MBOX0_SET   BIT(0)
    #define VPU_REG_HI_MBOX1_SET   BIT(1)

#define VPU_REG_RISC_MBOX_CLR                  (REG_MBX_BASE+(0x0067<<1))
    #define VPU_REG_RISC_MBOX0_CLR    BIT(0)
    #define VPU_REG_RISC_MBOX1_CLR    BIT(1)
    #define VPU_REG_RISC_ISR_CLR      BIT(2)
    #define VPU_REG_RISC_ISR_MSK      BIT(6)
    #define VPU_REG_RISC_ISR_FORCE    BIT(10)

#define VPU_REG_RISC_MBOX_RDY                  (REG_MBX_BASE+( 0x0068<<1))
    #define VPU_REG_RISC_MBOX0_RDY     BIT(0)
    #define VPU_REG_RISC_MBOX1_RDY     BIT(1)
    #define VPU_REG_RISC_ISR_VALID     BIT(2)

#define VPU_REG_HI_MBOX_RDY                    (REG_MBX_BASE+(0x0069<<1))
    #define VPU_REG_HI_MBOX0_RDY   BIT(0)
    #define VPU_REG_HI_MBOX1_RDY   BIT(1)

#define VPU_REG_RISC_MBOX0_L                   (REG_MBX_BASE+(0x006b<<1))
#define VPU_REG_RISC_MBOX0_H                   (REG_MBX_BASE+(0x006c<<1))
#define VPU_REG_RISC_MBOX1_L                   (REG_MBX_BASE+(0x006d<<1))
#define VPU_REG_RISC_MBOX1_H                   (REG_MBX_BASE+(0x006e<<1))

#define MAU1_CPU_RST                           (REG_MAU1_BASE+(0x0002<<1))
    #define MAU1_REG_SW_RESET           BIT(0)

#define MAU1_ARB0_DBG0                         (REG_MAU1_BASE+(0x0008<<1))
#define MAU1_ARB1_DBG0                         (REG_MAU1_BASE+(0x000a<<1))
    #define MAU1_FSM_CS_MASK            BMASK(13:9)
    #define MAU1_FSM_CS_IDLE            BITS(13:9, 1)

#define MAU1_MIU_SEL                    (REG_MAU1_BASE+(0x0001<<1))
#define MAU1_LV2_0_MIU_SEL             (REG_MAU1_LV2_0_BASE+(0x0001<<1))
#define MAU1_LV2_1_MIU_SEL             (REG_MAU1_LV2_1_BASE+(0x0001<<1))


//------------------------------------------------------------------------------
// ChipTop Reg
//------------------------------------------------------------------------------

#define CHIPTOP_REG_BASE               (0x1E00 )
#define CLKGEN0_REG_BASE               (0x0B00 )
#define CLKGEN1_REG_BASE               (0x3300)

#define REG_TOP_VPU             (CLKGEN0_REG_BASE+(0x0030<<1))
    #define TOP_CKG_VPU_MASK                BMASK(4:0)
    #define TOP_CKG_VPU_DIS                 BIT(0)
    #define TOP_CKG_VPU_INV                 BIT(1)
    #define TOP_CKG_VPU_CLK_MASK            BMASK(4:2)

#define REG_CHIPTOP_DUMMY_CODEC             (CHIPTOP_REG_BASE+(0x0015<<1))
    #define REG_CHIPTOP_DUMMY_CODEC_MASK    BMASK(15:0)
    #define REG_CHIPTOP_DUMMY_CODEC_ENABLE  BITS(15:14,1)

#define REG_CLKGEN1_RESERVERD0         (CLKGEN1_REG_BASE+(0x003e<<1))
    #define SELECT_CLK_HVD_AEC_P            BMASK(3:2)
    #define SELECT_CLK_HVD_AEC_P_216        BITS(3:2,0)
    #define SELECT_CLK_HVD_AEC_P_240        BITS(3:2,1)
    #define SELECT_CLK_HVD_AEC_P_288        BITS(3:2,2)
    #define SELECT_CLK_HVD_AEC_P_172        BITS(3:2,3)
    #define SELECT_CLK_HVD_AEC_P_MASK       BMASK(3:2)

#ifdef CONFIG_MSTAR_SRAMPD
#define REG_PATGEN_BASE                         0x71200
#define REG_PATGEN_VP9_BASE                     0x71800
#define REG_CODEC_SRAM_SD_EN                (REG_PATGEN_BASE+((0x0010)<<1))
    #define CODEC_SRAM_HVD_R2                   BIT(4)
    #define CODEC_SRAM_HVD_R2_MIU0_BWP          BIT(5)
    #define CODEC_SRAM_HVD_R2_MIU1_BWP          BIT(6)
#define REG_HICODEC_LITE_SRAM_SD_EN         (REG_PATGEN_VP9_BASE+((0x0010)<<1))
    #define HICODEC_LITE_SRAM_HICODEC0          BIT(0)
    #define HICODEC_LITE_SRAM_HICODEC1          BIT(1)
#endif
//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// MIU Reg
//------------------------------------------------------------------------------
#define MIU0_REG_HVD_BASE             	(0x1200)
#define MIU0_REG_HVD_BASE2             	(0x61500)

#define MIU1_REG_HVD_BASE             	(0x0600)
#define MIU1_REG_HVD_BASE2             	(0x62200)

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

//------------------------------------------------------------------------------
// oneway Reg
//------------------------------------------------------------------------------
#define ONEWAY_REG_BASE                 (0x60F00)

#define REG_ONEWAY_B16                  (ONEWAY_REG_BASE+(0x0041<<1))
    #define ONEWAY_B16                  BIT(0)

#define REG_LOCK_ONEWAY_B16             (ONEWAY_REG_BASE+(0x0051<<1))
    #define LOCK_ONEWAY_B16             BIT(0)

//------------------------------------------------------------------------------
// blk_codec Reg
//------------------------------------------------------------------------------
#define BLK_CODEC_REG_BASE              (0x71200)

// data side
#define REG_VDR2_D_ACCESS_RANGE0_CFG    (BLK_CODEC_REG_BASE+(0x0020<<1))
    #define VDR2_D_ACCESS_RANGE0_CFG_ALWAYS_PASS_W_ADDR          BIT(5)         // Bypass Address Limiter: means allow non-secure write access
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR_MASK             BMASK(11:8)
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR0_START           BITS(11:8, 1)  // configure 1st secure range start address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR0_END             BITS(11:8, 2)  // configure 1st secure range end   address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR1_START           BITS(11:8, 3)  // configure 2nd secure range start address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR1_END             BITS(11:8, 4)  // configure 2nd secure range end   address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR2_START           BITS(11:8, 5)  // configure 3rd secure range start address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR2_END             BITS(11:8, 6)  // configure 3rd secure range end   address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR3_START           BITS(11:8, 7)  // configure 4th secure range start address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_ADDR3_END             BITS(11:8, 8)  // configure 4th secure range end   address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_WRITE_REPLACE_ADDR          BITS(11:8, 9)  // configure replace                address from REG_VDR2_D_ACCESS_RANGE_ADDR
    #define VDR2_D_ACCESS_RANGE0_CFG_REF_ADDR0_RANGE_PASS_W_ADDR BIT(12)        // Enable limit write access
    #define VDR2_D_ACCESS_RANGE0_CFG_LOCK_RD_LAT_CLR             BIT(13)        // Bypass Address Limiter: means allow non-secure read access
    #define VDR2_D_ACCESS_RANGE0_CFG_ADDR0_LIMIT_EN              BIT(15)        // Enable 1st secure range

#define REG_VDR2_D_ACCESS_RANGE1_CFG    (BLK_CODEC_REG_BASE+(0x0021<<1))
    #if 0  // debug use
    #define VDR2_D_ACCESS_RANGE1_CFG_WRITE_ADDR_MASK             BMASK(11:8)
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR0_START            BITS(11:8, 1)  // get configure 1st secure range start address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR0_END              BITS(11:8, 2)  // get configure 1st secure range end   address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR1_START            BITS(11:8, 3)  // get configure 2nd secure range start address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR1_END              BITS(11:8, 4)  // get configure 2nd secure range end   address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR2_START            BITS(11:8, 5)  // get configure 3rd secure range start address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR2_END              BITS(11:8, 6)  // get configure 3rd secure range end   address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR3_START            BITS(11:8, 7)  // get configure 4th secure range start address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_ADDR3_END              BITS(11:8, 8)  // get configure 4th secure range end   address to REG_VDR2_D_ACCESS_RANGE_STS
    #define VDR2_D_ACCESS_RANGE1_CFG_READ_REPLACE_ADDR           BITS(11:8, 9)  // get configure replace                address to REG_VDR2_D_ACCESS_RANGE_STS
    #endif
    #define VDR2_D_ACCESS_RANGE1_CFG_REF_ADDR1_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_D_ACCESS_RANGE1_CFG_ADDR1_LIMIT_EN              BIT(15)

#define REG_VDR2_D_ACCESS_RANGE2_CFG    (BLK_CODEC_REG_BASE+(0x0022<<1))
    #define VDR2_D_ACCESS_RANGE2_CFG_REF_ADDR2_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_D_ACCESS_RANGE2_CFG_ADDR2_LIMIT_EN              BIT(15)

#define REG_VDR2_D_ACCESS_RANGE3_CFG    (BLK_CODEC_REG_BASE+(0x0023<<1))
    #define VDR2_D_ACCESS_RANGE3_CFG_REF_ADDR3_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_D_ACCESS_RANGE3_CFG_ADDR3_LIMIT_EN              BIT(15)

#define REG_VDR2_D_ACCESS_RANGE_ADDR_L  (BLK_CODEC_REG_BASE+(0x0024<<1))
#define REG_VDR2_D_ACCESS_RANGE_ADDR_H  (BLK_CODEC_REG_BASE+(0x0025<<1))
#define REG_VDR2_D_ACCESS_RANGE_STS_L   (BLK_CODEC_REG_BASE+(0x0026<<1))
#define REG_VDR2_D_ACCESS_RANGE_STS_H   (BLK_CODEC_REG_BASE+(0x0027<<1))

// instruction side
#define REG_VDR2_I_ACCESS_RANGE0_CFG    (BLK_CODEC_REG_BASE+(0x0028<<1))
    #define VDR2_I_ACCESS_RANGE0_CFG_ALWAYS_PASS_W_ADDR          BIT(5)         // Bypass Address Limiter: means allow non-secure write access
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR_MASK             BMASK(11:8)
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR0_START           BITS(11:8, 1)  // configure 1st secure range start address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR0_END             BITS(11:8, 2)  // configure 1st secure range end   address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR1_START           BITS(11:8, 3)  // configure 2nd secure range start address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR1_END             BITS(11:8, 4)  // configure 2nd secure range end   address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR2_START           BITS(11:8, 5)  // configure 3rd secure range start address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR2_END             BITS(11:8, 6)  // configure 3rd secure range end   address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR3_START           BITS(11:8, 7)  // configure 4th secure range start address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_ADDR3_END             BITS(11:8, 8)  // configure 4th secure range end   address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_WRITE_REPLACE_ADDR          BITS(11:8, 9)  // configure replace                address from REG_VDR2_I_ACCESS_RANGE_ADDR
    #define VDR2_I_ACCESS_RANGE0_CFG_REF_ADDR0_RANGE_PASS_W_ADDR BIT(12)        // Enable limit write access
    #define VDR2_I_ACCESS_RANGE0_CFG_LOCK_RD_LAT_CLR             BIT(13)        // Bypass Address Limiter: means allow non-secure read access
    #define VDR2_I_ACCESS_RANGE0_CFG_ADDR0_LIMIT_EN              BIT(15)        // Enable 1st secure range

#define REG_VDR2_I_ACCESS_RANGE1_CFG    (BLK_CODEC_REG_BASE+(0x0029<<1))
    #if 0  // debug use
    #define VDR2_I_ACCESS_RANGE1_CFG_WRITE_ADDR_MASK             BMASK(11:8)
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR0_START            BITS(11:8, 1)  // get configure 1st secure range start address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR0_END              BITS(11:8, 2)  // get configure 1st secure range end   address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR1_START            BITS(11:8, 3)  // get configure 2nd secure range start address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR1_END              BITS(11:8, 4)  // get configure 2nd secure range end   address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR2_START            BITS(11:8, 5)  // get configure 3rd secure range start address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR2_END              BITS(11:8, 6)  // get configure 3rd secure range end   address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR3_START            BITS(11:8, 7)  // get configure 4th secure range start address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_ADDR3_END              BITS(11:8, 8)  // get configure 4th secure range end   address to REG_VDR2_I_ACCESS_RANGE_STS
    #define VDR2_I_ACCESS_RANGE1_CFG_READ_REPLACE_ADDR           BITS(11:8, 9)  // get configure replace                address to REG_VDR2_I_ACCESS_RANGE_STS
    #endif
    #define VDR2_I_ACCESS_RANGE1_CFG_REF_ADDR1_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_I_ACCESS_RANGE1_CFG_ADDR1_LIMIT_EN              BIT(15)

#define REG_VDR2_I_ACCESS_RANGE2_CFG    (BLK_CODEC_REG_BASE+(0x002a<<1))
    #define VDR2_I_ACCESS_RANGE2_CFG_REF_ADDR2_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_I_ACCESS_RANGE2_CFG_ADDR2_LIMIT_EN              BIT(15)

#define REG_VDR2_I_ACCESS_RANGE3_CFG    (BLK_CODEC_REG_BASE+(0x002b<<1))
    #define VDR2_I_ACCESS_RANGE3_CFG_REF_ADDR3_RANGE_PASS_W_ADDR BIT(12)
    #define VDR2_I_ACCESS_RANGE3_CFG_ADDR3_LIMIT_EN              BIT(15)

#define REG_VDR2_I_ACCESS_RANGE_ADDR_L  (BLK_CODEC_REG_BASE+(0x002c<<1))
#define REG_VDR2_I_ACCESS_RANGE_ADDR_H  (BLK_CODEC_REG_BASE+(0x002d<<1))
#define REG_VDR2_I_ACCESS_RANGE_STS_L   (BLK_CODEC_REG_BASE+(0x002e<<1))
#define REG_VDR2_I_ACCESS_RANGE_STS_H   (BLK_CODEC_REG_BASE+(0x002f<<1))



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------




#endif // _REG_VPU_H_

