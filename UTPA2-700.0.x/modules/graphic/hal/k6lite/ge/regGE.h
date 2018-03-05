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
/// file    regGE.h
/// @brief  GE Module Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_GE_H_
#define _REG_GE_H_

//#include "regCHIP.h"

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

#define EUCLID_BRINGUP 1        //[EUCLID] BRINGUP will remove after bringup
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined (__aeon__)
#define RIU_MAP pGEHalLocal->va_mmio_base
#else
#define RIU_MAP pGEHalLocal->va_mmio_base
#endif
/* Chip Reversion*/
#define CHIP_REG_BASE               0x1E00UL  // 0x1E00 - 0x1EFF
#define CHIP_CLK_BASE               0x0B00UL  // 0x1E00 - 0x1EFF
#define GHIP_REVERSION              (0x02UL)
#define CHIP_GE_CLK                 (0x48UL)
#define MIU1_REG_BASE               0x0600UL
#define MIU_SELETE_OFFSET			31UL

#define RIU     ((unsigned short volatile *) RIU_MAP)
#define RIU8    ((unsigned char  volatile *) RIU_MAP)

#define REG8(a) RIU8[((a) * 2) - ((a) & 1)]

#define REG_GE_BASE                 (0xBF800000UL+(0x1400UL<<2)) // 0xbf805000
//#define GE_REG(addr)                (*((volatile MS_U16*)(REG_GE_BASE + ((addr)<<2))))
#define MIU1_GEGROUP                 0x7DUL
#define MIU1_GE_CLIENT             BIT(15)
#define MIU1_REG(addr)             RIU[ (((addr)*2) + 0x600UL)]
#define GE_REG(addr)                 RIU[ (((addr)*2) + 0x2800UL)]
#define CLK_REG(addr)                 RIU[ (((addr)*2) + 0x0B00UL)]

#define REG_GE_EN                   0x0000UL
    #define GE_EN_GE                            BIT(0)
    #define GE_EN_DITHER                        BIT(1)
    #define GE_EN_BLEND                         BIT(2)
#if (EUCLID_BRINGUP==1)
    #define GE_EN_ASCK                          BIT(3)
    #define GE_EN_DSCK                          BIT(4)
#endif
    #define GE_EN_ROP2                          BIT(5)
    #define GE_EN_SCK                           BIT(6)
    #define GE_EN_DCK                           BIT(7)
    #define GE_EN_LINEPAT                       BIT(8)
    #define GE_EN_DITHER_RAND                   BIT(9)                  //[OBERON]
    #define GE_EN_DFB_BLD                       BIT(10)
    #define GE_EN_ACMP                          BIT(11)
    #define GE_EN_ATEST                         BIT(12)                 //[OBERON]
    #define GE_EN_CALC_SRC_WH                   BIT(13)
#if (EUCLID_BRINGUP==1)
    #define GE_EN_TRAP_SUB_CORR                 BIT(14)                 //[Euclid]
#endif
    #define GE_EN_BURST                         BIT(14)
    #define GE_EN_ONE_PIXEL_MODE                BIT(15)                 //[OBERON]
#define REG_GE_CFG                  0x0001UL
    #define GE_CFG_CMDQ_MASK                    BMASK(3:0)
    #define GE_CFG_CMDQ                         BIT(0)
    #define GE_CFG_VCMDQ                        BIT(1)                  //[OBERON]
    #define GE_CFG_RPRIO                        BIT(2)

#if (EUCLID_BRINGUP==1)
    #define GE_CFG_DISABLE_MIU_ACS              BIT(3)                  //[Euclid]
#else
    #define GE_CFG_WPRIO                        BIT(3)
#endif
    #define GE_CFG_BLT_STRETCH                  BIT(4)
    #define GE_CFG_EN_CLIPCHK                   BIT(5)                  //[OBERON] hw patch for both start/end out of clipping window
    #define GE_CFG_BLT_ITALIC                   BIT(6)
    #define GE_CFG_LENGTH_LIMIT                 BIT(7)                  //[OBERON] burst length limit to 1
    #define GE_CFG_SRC_TILE                     BIT(8)                  //[OBERON] source buffer tile mode
    #define GE_CFG_DST_TILE                     BIT(9)                  //[OBERON] destination buffer tile mode
    #define GE_CFG_MIU0_PROT                    BIT(10)                 //[OBERON]
    #define GE_CFG_MIU1_PROT                    BIT(11)                 //[OBERON]
    #define GE_CFG_RW_SPLIT                     BIT(14)                 //[OBERON]
#if (EUCLID_BRINGUP==1)
    #define GE_CFG_CLR_MIU_FLG                  BIT(12)                 //[Euclid]
    #define GE_CFG_EN_DNY_CLK_GATE              BIT(15)                 //[Euclid]
#endif
#define REG_GE_DBG                  0x0002UL
#if (EUCLID_BRINGUP==1)
    #define GE_DBG_MIU_MAX_LEG                  BMASK(8:12)             //[Euclid]
    #define GE_DBG_POL_VAR_MODE                 BIT(14)                 //[Euclid]
    #define GE_DBG_LEG_MODE                     BIT(15)                 //[Euclid]
#endif

#define REG_GE_TH                   0x0003UL
    #define GE_TH_STBB_MASK                     BMASK(3:0)
    #define GE_TH_CMDQ_MASK                     BMASK(7:4)              //[OBERON]
    #define GE_TH_CMDQ2_MASK                    BMASK(11:8)             //[OBERON]
#define REG_GE_VCMDQ_STAT           0x0004UL
#define REG_GE_BIST_STAT            0x0005UL
    #define GE_VCMDQ_STAT_H_MASK                BMASK(0:0)              //[OBERON] virtual queue status [16]
    #define GE_STAT_VCMDQ_MAX                    131071UL
#define REG_GE_STAT                 0x0007UL
    #define GE_STAT_BUSY                        BIT(0)
    #define GE_STAT_CMDQ2_MASK                  BMASK(7:3)
    #define GE_STAT_CMDQ2_SHFT                  3UL
    #define GE_STAT_CMDQ2_MAX                   16UL
    #define GE_STAT_CMDQ_MASK                   BMASK(15:11)
    #define GE_STAT_CMDQ_SHFT                   11UL
    #define GE_STAT_CMDQ_MAX                    8UL
#define REG_GE_MIU_PROT_LTH_L(_miu) (0x0008UL+(_miu<<2))
#define REG_GE_MIU_PROT_LTH_H(_miu) (0x0009UL+(_miu<<2))
    #define GE_MIU_PROT_LTH_H_MASK              BMASK(10:0)
    #define GE_MIU_PROT_MODE_MASK               BIT(15)
    #define GE_MIU_PROT_MODE_SHFT               15UL
    #define GE_MIU_PROT_MODE_EQ                 BITS(15:15, 0)
    #define GE_MIU_PROT_MODE_NE                 BITS(15:15, 1)
#define REG_GE_MIU_PROT_HTH_L(_miu) (0x000A+(_miu<<2))
#define REG_GE_MIU_PROT_HTH_H(_miu) (0x000B+(_miu<<2))
#define REG_GE_ROP2                 0x0010UL
    #define GE_ROP2_MASK                        BMASK(3:0)
#define REG_GE_BLEND                0x0011UL // source coefficient of alpha blending
    #define GE_BLEND_MASK                       BMASK(3:0)
    #define GE_BLEND_ONE                        BITS(3:0, 0)
    #define GE_BLEND_CONST                      BITS(3:0, 1)
    #define GE_BLEND_ASRC                       BITS(3:0, 2)
    #define GE_BLEND_ADST                       BITS(3:0, 3)
    #define GE_BLEND_ROP8_ALPHA                 BITS(3:0, 4)            //[OBERON]
    #define GE_BLEND_ROP8_SRCOVER               BITS(3:0, 5)            //[OBERON]
    #define GE_BLEND_ROP8_DSTOVER               BITS(3:0, 6)            //[OBERON]
#if (EUCLID_BRINGUP==1)
    #define GE_BLEND_ROP8_DSTNEW                BITS(3:0, 7)            //[Euclid] Csrc*Aconst
#endif
    #define GE_BLEND_ZERO                       BITS(3:0, 8)
    #define GE_BLEND_CONST_INV                  BITS(3:0, 9)
    #define GE_BLEND_ASRC_INV                   BITS(3:0, 10)
    #define GE_BLEND_ADST_INV1                  BITS(3:0, 11)
    #define GE_BLEND_ADST_INV2                  BITS(3:0, 12)
    #define GE_BLEND_ADST_INV3                  BITS(3:0, 13)
#if (EUCLID_BRINGUP==1)
    #define GE_BLEND_ADST_INV4                  BITS(3:0, 14)           //[Euclid] Csrc*Aconst
    #define GE_BLEND_ADST_INV5                  BITS(3:0, 15)           //[Euclid] Csrc*Aconst
#endif

    #define GE_ALPHA_ARGB1555                   BMASK(15:8)             //[OBERON]
#define REG_GE_ALPHA                0x0012UL
    #define GE_ALPHA_MASK                       BMASK(12:8)
    #define GE_ALPHA_SHFT                       8UL
#define REG_GE_ALPHA_CONST          0x0013UL
    #define GE_ALPHA_CONST_MASK                 BMASK(7:0)
    #define GE_ALPHA_SRCMASK_MASK               BMASK(15:8)

#define REG_GE_SCK_HTH_L            0x0014UL      //ARGB8888(32), CrYCb, BLINK(16), I8(8)
#define REG_GE_SCK_HTH_H            0x0015UL      //
#define REG_GE_SCK_LTH_L            0x0016UL      //ARGB8888(32), CrYCb, BLINK(16), I8(8)
#define REG_GE_SCK_LTH_H            0x0017UL      //
#define REG_GE_DCK_HTH_L            0x0018UL      //ARGB8888(32), CrYCb, BLINK(16), I8(8)
#define REG_GE_DCK_HTH_H            0x0019UL      //
#define REG_GE_DCK_LTH_L            0x001AUL      //ARGB8888(32), CrYCb, BLINK(16), I8(8)
#define REG_GE_DCK_LTH_H            0x001BUL      //
#define REG_GE_OP_MODE              0x001CUL
    #define GE_OP_SCK_SHFT                      0UL
    #define GE_OP_SCK_NE                        BIT(0) //
    #define GE_OP_DCK_SHFT                      1UL
    #define GE_OP_DCK_NE                        BIT(1) //
    #define GE_OP_ACMP_SHFT                     4UL
    #define GE_OP_ACMP_MIN                      BIT(4) // MIN(Asrc,Adst)
    #define GE_OP_ATEST_SHFT                    5UL
    #define GE_OP_ATEST_NE                      BIT(5)
    #define GE_SRCCOLOR_MASK_B                  BMASK(15:8)
    #define GE_SRCCOLOR_MASK_B_SHIFT            8UL
#define REG_GE_ATEST_TH             0x001DUL
    #define GE_ATEST_HTH_MASK                   BMASK(7:0)
    #define GE_ATEST_LTH_MASK                   BMASK(15:8)
#define REG_GE_SRCMASK_GB           0x001EUL
    #define GE_SRCCOLOR_MASK_G                  BMASK(7:0)
    #define GE_SRCCOLOR_MASK_G_SHIFT            0UL
    #define GE_SRCCOLOR_MASK_R                  BMASK(15:8)
    #define GE_SRCCOLOR_MASK_R_SHIFT            8UL
#define REG_GE_YUV_MODE             0x001FUL      //[URANUS] write only
    #define GE_FMT_YVYU                         0UL
    #define GE_FMT_YUYV                         1UL
    #define GE_FMT_VYUY                         2UL
    #define GE_FMT_UYVY                         3UL
    #define GE_YUV_RGB2YUV_MASK                 BMASK(1:0)
    #define GE_YUV_RGB2YUV_SHFT                 0UL
    #define GE_YUV_RGB2YUV_PC                   BITS(1:0, 0)
    #define GE_YUV_RGB2YUV_255                  BITS(1:0, 1)
    #define GE_YUV_OUT_RANGE_MASK               BMASK(2)
    #define GE_YUV_OUT_RANGE_SHFT               2UL
    #define GE_YUV_OUT_255                      BITS(2:2, 0)
    #define GE_YUV_OUT_PC                       BITS(2:2, 1)
    #define GE_YUV_IN_RANGE_MASK                BMASK(3)
    #define GE_YUV_IN_RANGE_SHFT                3UL
    #define GE_YUV_IN_255                       BITS(3:3, 0)
    #define GE_YUV_IN_127                       BITS(3:3, 1)
    #define GE_YUV_SRC_YUV422_MASK              BMASK(5:4)
    #define GE_YUV_SRC_YUV422_SHFT              4UL
    #define GE_YUV_SRC_YVYU                     BITS(5:4, GE_FMT_YVYU) // CbY1CrY0
    #define GE_YUV_SRC_YUYV                     BITS(5:4, GE_FMT_YUYV) // CrY1CbY0 (ATI)
    #define GE_YUV_SRC_VYUY                     BITS(5:4, GE_FMT_VYUY) // Y1CbY0Cr
    #define GE_YUV_SRC_UYVY                     BITS(5:4, GE_FMT_UYVY) // Y1CrY0Cb
    #define GE_YUV_DST_YUV422_MASK              BMASK(7:6)
    #define GE_YUV_DST_YUV422_SHFT              6UL
    #define GE_YUV_DST_YVYU                     BITS(7:6, GE_FMT_YVYU)
    #define GE_YUV_DST_YUYV                     BITS(7:6, GE_FMT_YUYV)
    #define GE_YUV_DST_VYUY                     BITS(7:6, GE_FMT_VYUY)
    #define GE_YUV_DST_UYVY                     BITS(7:6, GE_FMT_UYVY)
    #define GE_YUV_CSC_MASK                     BMASK(7:0)
    #define GE_SRC_BUFFER_MIU_H_SHFT              13UL
    #define GE_DST_BUFFER_MIU_H_SHFT              14UL
#define REG_GE_SRC_BASE_L           0x0020UL
#define REG_GE_SRC_BASE_H           0x0021UL
#if (EUCLID_BRINGUP==1)
    #define GE_SB_MIU_SEL                       BIT(15) //[Euclid] Source Buffer MIU Selection
#endif
#define REG_GE_DST_BASE_L           0x0026UL
#define REG_GE_DST_BASE_H           0x0027UL
#if (EUCLID_BRINGUP==1)
    #define GE_DB_MIU_SEL                       BIT(15) //[Euclid] Destination Buffer MIU Selection
#endif

#define REG_GE_VCMDQ_BASE_L         0x0028UL
#define REG_GE_VCMDQ_BASE_H         0x0029UL
#if (EUCLID_BRINGUP==1)
    #define GE_VCMQ_MIU_SEL                     BIT(15) //[Euclid] Virtual Command Queue MIU Selection
#endif

#define REG_GE_VCMDQ_SIZE           0x002AUL
    #define GE_VCMDQ_SIZE_MASK                  BMASK(2:0)
    #define GE_VCMDQ_SIZE_MIN                   0x00001000UL // 4KB
    #define GE_VCMDQ_SIZE_MAX                   0x00080000UL // 512KB
    #define GE_VCMDQ_SIZE(_size)                ( (_size>>19) ? 7 :     \
                                                (_size>>18) ? 6 :     \
                                                (_size>>17) ? 5 :     \
                                                (_size>>16) ? 4 :     \
                                                (_size>>15) ? 3 :     \
                                                (_size>>14) ? 2 :     \
                                                (_size>>13) ? 1 : 0 )
#define REG_GE_DFB_BLD_OP            0x002AUL
    #define GE_DFB_SRC_COLORMASK                BIT(7)
    #define GE_DFB_SRC_COLORMASK_SHIFT          7UL
    #define GE_DFB_SRCBLD_OP_MASK               BMASK(11:8)
    #define GE_DFB_SRCBLD_OP_SHFT               8UL
    #define GE_DFB_SRCBLD_OP_ZERO               BITS(11:8, 0)
    #define GE_DFB_SRCBLD_OP_ONE                BITS(11:8, 1)
    #define GE_DFB_SRCBLD_OP_SRCCOLOR           BITS(11:8, 2)
    #define GE_DFB_SRCBLD_OP_INVSRCCOLOR        BITS(11:8, 3)
    #define GE_DFB_SRCBLD_OP_SRCALPHA           BITS(11:8, 4)
    #define GE_DFB_SRCBLD_OP_INVSRCALPHA        BITS(11:8, 5)
    #define GE_DFB_SRCBLD_OP_DESTALPHA          BITS(11:8, 6)
    #define GE_DFB_SRCBLD_OP_INVDESTALPHA       BITS(11:8, 7)
    #define GE_DFB_SRCBLD_OP_DESTCOLOR          BITS(11:8, 8)
    #define GE_DFB_SRCBLD_OP_INVDESTCOLOR       BITS(11:8, 9)
    #define GE_DFB_SRCBLD_OP_SRCALPHASAT        BITS(11:8, 10)
    #define GE_DFB_DSTBLD_OP_MASK               BMASK(15:12)
    #define GE_DFB_DSTBLD_OP_SHFT               12UL
    #define GE_DFB_DSTBLD_OP_ZERO               BITS(15:12, 0)
    #define GE_DFB_DSTBLD_OP_ONE                BITS(15:12, 1)
    #define GE_DFB_DSTBLD_OP_SRCCOLOR           BITS(15:12, 2)
    #define GE_DFB_DSTBLD_OP_INVSRCCOLOR        BITS(15:12, 3)
    #define GE_DFB_DSTBLD_OP_SRCALPHA           BITS(15:12, 4)
    #define GE_DFB_DSTBLD_OP_INVSRCALPHA        BITS(15:12, 5)
    #define GE_DFB_DSTBLD_OP_DESTALPHA          BITS(15:12, 6)
    #define GE_DFB_DSTBLD_OP_INVDESTALPHA       BITS(15:12, 7)
    #define GE_DFB_DSTBLD_OP_DESTCOLOR          BITS(15:12, 8)
    #define GE_DFB_DSTBLD_OP_INVDESTCOLOR       BITS(15:12, 9)
    #define GE_DFB_DSTBLD_OP_SRCALPHASAT        BITS(15:12, 10)
#define REG_GE_DFB_BLD_FLAGS        0x002BUL
    #define GE_DFB_BLD_FLAGS_MASK               BMASK(7:0)
    #define GE_DFB_BLD_FLAG_COLORALPHA          BIT(0)
    #define GE_DFB_BLD_FLAG_ALPHACHANNEL        BIT(1)
    #define GE_DFB_BLD_FLAG_COLORIZE            BIT(2)
    #define GE_DFB_BLD_FLAG_SRCPREMUL           BIT(3)
    #define GE_DFB_BLD_FLAG_SRCPREMULCOL        BIT(4)
    #define GE_DFB_BLD_FLAG_DSTPREMUL           BIT(5)
    #define GE_DFB_BLD_FLAG_XOR                 BIT(6)
    #define GE_DFB_BLD_FLAG_DEMULTIPLY          BIT(7)
#define REG_GE_B_CONST              0x002BUL
    #define GE_B_CONST_MASK                     BMASK(15:8)
    #define GE_B_CONST_SHIFT                    8UL
#define REG_GE_G_CONST              0x002CUL
    #define GE_G_CONST_MASK                     BMASK(7:0)
    #define GE_G_CONST_SHIFT                    0UL
#define REG_GE_R_CONST              0x002CUL
    #define GE_R_CONST_MASK                     BMASK(15:8)
    #define GE_R_CONST_SHIFT                    8UL
#define REG_GE_CLUT_L               0x002DUL
#define REG_GE_CLUT_H               0x002EUL
#define REG_GE_CLUT_CTRL            0x002FUL
    #define GE_CLUT_CTRL_IDX_MASK               BMASK(7:0)
    #define GE_CLUT_CTRL_RD                     BITS(8:8, 0)
    #define GE_CLUT_CTRL_WR                     BITS(8:8, 1)
#define REG_GE_SRC_PITCH            0x0030UL
#define REG_GE_TAG                  0x0032UL
#define REG_GE_DST_PITCH            0x0033UL
#define REG_GE_FMT                  0x0034UL
    #define GE_SRC_FMT_MASK                     BMASK(4:0)
    #define GE_SRC_FMT_SHFT                     0UL
    #define GE_DST_FMT_MASK                     BMASK(12:8)
    #define GE_DST_FMT_SHFT                     8UL
#define REG_GE_C_L(_idx)           (0x0035UL+(_idx<<1)) // [31:0]ARGB8888, [15:0]blink, [7:0]I8
#define REG_GE_C_H(_idx)           (0x0036UL+(_idx<<1))
#define REG_GE_CLIP_L               0x0055UL
#define REG_GE_CLIP_R               0x0056UL
#define REG_GE_CLIP_T               0x0057UL
#define REG_GE_CLIP_B               0x0058UL
#define REG_GE_ROT_MODE             0x0059UL
    #define REG_GE_ROT_MODE_MASK                BMASK(1:0)
    #define REG_GE_ROT_MODE_SHFT                0UL
    #define REG_GE_ROT_0                        BITS(1:0, 0)
    #define REG_GE_ROT_90                       BITS(1:0, 1)
    #define REG_GE_ROT_180                      BITS(1:0, 2)
    #define REG_GE_ROT_270                      BITS(1:0, 3)
#if (EUCLID_BRINGUP==1)
    #define GE_TRAP_DX1                         BMASK(15:0)
#define REG_GE_BLT_SCK_MODE         0x0059UL
    #define GE_BLT_SCK_MODE_MASK                BMASK(7:6)
    #define GE_BLT_SCK_BILINEAR                 BITS(7:6, 0) //[Euclid] Do nothing
    #define GE_BLT_SCK_NEAREST                  BITS(7:6, 1) //[Euclid] NEAREST WHEN THE COLOR KEY HAPPENED
    #define GE_BLT_SCK_CONST                    BITS(7:6, 2) //[Euclid] REPLACE THE KEY TO CUSTOM COLOR
    #define GE_TRAP_DX0_MSB                     BMASK(9:8)
    #define GE_TRAP_DX0_MSB_SHFT                   8UL
    #define GE_TRAP_DX1_MSB                     BMASK(13:12)
    #define GE_TRAP_DX1_MSB_SHFT                   12UL

#define REG_GE_TRAPEZOID_DX0        0x005AUL
    #define GE_TRAP_DX0                         BMASK(15:0)
#define REG_GE_TRAPEZOID_DX1        0x005BUL
    #define GE_TRAP_DX1                         BMASK(15:0)
#else
#define REG_GE_BLT_SCK_MODE         0x005BUL
    #define GE_BLT_SCK_MODE_MASK                BMASK(1:0)
    #define GE_BLT_SCK_BILINEAR                 BITS(1:0, 0)
    #define GE_BLT_SCK_NEAREST                  BITS(1:0, 1)
    #define GE_BLT_SCK_CONST                    BITS(1:0, 2) // replace to const color
#endif
#define REG_GE_BLT_SCK_CONST_L      0x005CUL // GB
#define REG_GE_BLT_SCK_CONST_H      0x005DUL // R, Uranus doesn't have alpha key
#define REG_GE_BLT_DST_X_OFST       0x005EUL // (s.12)
#if (EUCLID_BRINGUP==1)
    #define GE_STBB_DX_MSB                    BIT(15)
    #define GE_STBB_DX_MSB_SHFT                 15UL
#endif
#define REG_GE_BLT_DST_Y_OFST       0x005FUL // (s.12)
#if (EUCLID_BRINGUP==1)
    #define GE_STBB_DY_MSB                    BIT(15)
    #define GE_STBB_DY_MSB_SHFT                 15UL
#endif
#define REG_GE_CMD                  0x0060UL
    #define GE_PRIM_TYPE_MASK                   BMASK(6:4)
    #define GE_PRIM_LINE                        BITS(6:4, 1)
#if (EUCLID_BRINGUP==1)
    #define GE_PRIM_TRAPEZOID                   BITS(6:4, 2)
#endif
    #define GE_PRIM_RECT                        BITS(6:4, 3)
    #define GE_PRIM_BITBLT                      BITS(6:4, 4)
#if (EUCLID_BRINGUP==1)
    #define GE_PRIM_TRAPEZOID_BLT               BITS(6:4, 5)
#endif
    #define GE_SRC_DIR_X_INV                    BIT(7)
    #define GE_SRC_DIR_Y_INV                    BIT(8)
    #define GE_DST_DIR_X_INV                    BIT(9)
    #define GE_DST_DIR_Y_INV                    BIT(10)
    #define GE_LINE_GRADIENT                    BIT(11)
    #define GE_RECT_GRADIENT_H                  BIT(12)
    #define GE_RECT_GRADIENT_V                  BIT(13)
    #define GE_STRETCH_BILINEAR                 BITS(14:14, 0)
    #define GE_STRETCH_NEAREST                  BITS(14:14, 1)
    #define GE_STRETCH_CLAMP                    BIT(15)
#define REG_GE_LINE_DELTA           0x0061UL
    #define GE_LINE_DELTA_MASK                  BMASK(14:1) // (s1.12) minor direction delta value of line
    #define GE_LINE_DELTA_SHFT                  1UL
    #define GE_LINE_MAJOR_X                     BITS(15:15, 0)
    #define GE_LINE_MAJOR_Y                     BITS(15:15, 1)
#define REG_GE_LINE_STYLE           0x0062UL
    #define GE_LINEPAT_MASK                     BMASK(5:0)
    #define GE_LINEPAT_RST                      BIT(8)
    #define GE_LINE_LAST                        BIT(9)
    #define GE_LINEPAT_REP_MASK                 BMASK(7:6)
    #define GE_LINEPAT_REP_SHFT                 6UL
    #define GE_LINEPAT_REP1                     BITS(7:6, 0)
    #define GE_LINEPAT_REP2                     BITS(7:6, 1)
    #define GE_LINEPAT_REP3                     BITS(7:6, 2)
    #define GE_LINEPAT_REP4                     BITS(7:6, 3)
#define REG_GE_LINE_LENGTH          0x0063UL
    #define GE_LINE_LENGTH_MASK                 BMASK(11:0)
#define REG_GE_BLT_SRC_DX           0x0064UL  //[EUCLID] (5.12)
#define REG_GE_BLT_SRC_DY           0x0065UL  //[EUCLID] (5.12)
    #define GE_STBB_DXY_MASK                    BMASK(12:0)
#define REG_GE_ITALIC_OFFSET        0x0066UL
    #define GE_ITALIC_X_MASK                    BMASK(7:0)
    #define GE_ITALIC_X_SHFT                    0UL
    #define GE_ITALIC_Y_MASK                    BMASK(15:8)
    #define GE_ITALIC_Y_SHFT                    8UL
#define REG_GE_ITALIC_DELTA         0x0067UL
    #define GE_ITALIC_D_MASK                    BMASK(7:0) // (s1.3)
    #define GE_ITALIC_D_SHFT                    0UL
#define REG_GE_PRIM_V0_X            0x0068UL      //[EUCLID] COORDINATE X0 OF PRIMITIVE VERTEX 0
#define REG_GE_PRIM_V0_Y            0x0069UL      //[EUCLID] COORDINATE Y0 OF PRIMITIVE VERTEX 0
#define REG_GE_PRIM_V1_X            0x006AUL      //[EUCLID] COORDINATE X1 OF PRIMITIVE VERTEX 1
#define REG_GE_PRIM_V1_Y            0x006BUL      //[EUCLID] COORDINATE Y1 OF PRIMITIVE VERTEX 1
#define REG_GE_PRIM_V2_X            0x006CUL
#define REG_GE_PRIM_V2_Y            0x006DUL
#define REG_GE_BLT_SRC_W            0x006EUL
#define REG_GE_BLT_SRC_H            0x006FUL
#define REG_GE_PRIM_C_L             0x0070UL // [L]:B,Bg(AXFB2355,AAFB2266),I8, [H]:G,Fg(AXFB2355,AAFB2266)
#define REG_GE_PRIM_C_H             0x0071UL // [L]:R,AX(AXFB2355),Ba(AAFB2266), [H]:A,1(AXFB2355),Fa(AAFB2266)
#define REG_GE_PRIM_RDX_L           0x0072UL // (s7.12)
#define REG_GE_PRIM_RDX_H           0x0073UL
#define REG_GE_PRIM_RDY_L           0x0074UL
#define REG_GE_PRIM_RDY_H           0x0075UL
#define REG_GE_PRIM_GDX_L           0x0076UL
#define REG_GE_PRIM_GDX_H           0x0077UL
#define REG_GE_PRIM_GDY_L           0x0078UL
#define REG_GE_PRIM_GDY_H           0x0079UL
#define REG_GE_PRIM_BDX_L           0x007AUL
#define REG_GE_PRIM_BDX_H           0x007BUL
#define REG_GE_PRIM_BDY_L           0x007CUL
#define REG_GE_PRIM_BDY_H           0x007DUL
#define REG_GE_PRIM_ADX             0x007EUL // (s4.11)
#define REG_GE_PRIM_ADY             0x007FUL

#define REG_GE_TLB_TYPE_EN          0x0000UL
#define GE_TLB_MODE_MASK                BMASK(2:1)
    #define GE_TLB_SRC                  BIT(1)
    #define GE_TLB_DST                  BIT(2)
    #define GE_TLB_FLUSH                BIT(3)

#define REG_GE_TLB_EN               0x0001UL
    #define GE_TLB_EN                   BIT(0)

#define REG_GE_TLB_TAG              0x0010UL
    #define GE_TLB_TAG                  BMASK(5:0)

#define REG_GE_TLB_BASE_MIU_H       0x001FUL
    #define GE_SB_TLB_SRC_MIU_SEL_H     BIT(13)
    #define GE_SB_TLB_DST_MIU_SEL_H     BIT(14)

#define REG_GE_SRC_TLB_BASE_L       0x0020UL
#define REG_GE_SRC_TLB_BASE_H       0x0021UL
    #define GE_SB_TLB_MIU_SEL           BIT(15)

#define REG_GE_DST_TLB_BASE_L       0x0022UL
#define REG_GE_DST_TLB_BASE_H       0x0023UL
    #define GE_DB_TLB_MIU_SEL           BIT(15)


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_GE_H_
