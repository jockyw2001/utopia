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
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regMultiPVR.h
//  Description: TSP Multi-PVR Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_MULTI_PVR_H_
#define _REG_MULTI_PVR_H_

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

#define TSP_MULTI_PVR_ENG_NUM       1
#define TSP_MULTI_PVR_CH_NUM        8

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct _REG32_MULTI_PVR
{
    volatile MS_U16                 low;
    volatile MS_U16                 _null_l;
    volatile MS_U16                 high;
    volatile MS_U16                 _null_h;
} REG32_MULTI_PVR;

typedef struct _REG16_MULTI_PVR
{
    volatile MS_U16                 data;
    volatile MS_U16                 _null;
} REG16_MULTI_PVR;

typedef struct _REG_MULTI_PVR_ENG_Ctrl // Multi-PVR (Bank:0x160A)
{
    REG16_MULTI_PVR     CFG_MULTI_PVR_00;
        #define CFG_MULTI_PVR_00_REG_PVR_STR2MI_EN                              0x0001
        #define CFG_MULTI_PVR_00_REG_PVR_STR2MI_RST_WADR                        0x0002
        #define CFG_MULTI_PVR_00_REG_PVR_STR2MI_PAUSE                           0x0004
        #define CFG_MULTI_PVR_00_REG_PVR_BURST_LEN_MASK                         0x0018
        #define CFG_MULTI_PVR_00_REG_PVR_BURST_LEN_SHIFT                        3
        #define CFG_MULTI_PVR_00_REG_PVR_SRAM_SD_EN                             0x0020
        #define CFG_MULTI_PVR_00_REG_PVR_STR2MI_WP_LD                           0x0040
        #define CFG_MULTI_PVR_00_REG_PVR_CLR                                    0x0080  // clear PVR overflow flag
        #define CFG_MULTI_PVR_00_REG_PVR_DMA_FLUSH_EN                           0x0100
        #define CFG_MULTI_PVR_00_REG_PVR_MIU_HIGHPRI                            0x0200
        #define CFG_MULTI_PVR_00_REG_PVR_WRITE_POINTER_TO_NEXT_ADDR_EN          0x0400
        #define CFG_MULTI_PVR_00_REG_PVR_DMAW_PROTECT_EN                        0x0800
        #define CFG_MULTI_PVR_00_REG_PVR_CLR_NO_HIT_INT                         0x1000

    REG32_MULTI_PVR     CFG_MULTI_PVR_01_02;                                    // reg_pvr_dmaw_waddr_err

    REG16_MULTI_PVR     CFG_MULTI_PVR_03;                                       // reserved

    REG32_MULTI_PVR     CFG_MULTI_PVR_04_05;                                    // reg_pvr_str2mi_wadr_r

    REG16_MULTI_PVR     CFG_MULTI_PVR_06;
        #define CFG_MULTI_PVR_06_REG_PVR_FIFO_STATUS_MASK                       0x001F
        #define CFG_MULTI_PVR_06_REG_PVR_FIFO_STATUS_SHIFT                      0

    REG32_MULTI_PVR     CFG_MULTI_PVR_07_08;                                    // reg_pvr_dmaw_lbnd

    REG32_MULTI_PVR     CFG_MULTI_PVR_09_0A;                                    // reg_pvr_dmaw_ubnd

    REG16_MULTI_PVR     CFG_MULTI_PVR_0B_3F[0x40 - 0x0B];                       // reserved

    REG16_MULTI_PVR     CFG_MULTI_PVR_40;
        #define CFG_MULTI_PVR_40_REG_ACPU_ACTIVE                                0x0001

    REG16_MULTI_PVR     CFG_MULTI_PVR_41;                                       // reg_acpu_cmd

    REG16_MULTI_PVR     CFG_MULTI_PVR_42;                                       // reg_acpu_flag

    REG32_MULTI_PVR     CFG_MULTI_PVR_43_44;                                    // reg_acpu_addr_head

    REG32_MULTI_PVR     CFG_MULTI_PVR_45_46;                                    // reg_acpu_addr_tail

    REG32_MULTI_PVR     CFG_MULTI_PVR_47_48;                                    // reg_acpu_rdata

    REG16_MULTI_PVR     CFG_MULTI_PVR_49;
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_INT_CLR                          0x0001
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_PAUSE                            0x0002
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_DBG_SEL_MASK                     0x00FC
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_DBG_SEL_SHIFT                    4
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_INT_MASK                         0x0100
        #define CFG_MULTI_PVR_49_REG_SGDMA_OUT_VC_INT_TRIGGER                   0x0400

    REG16_MULTI_PVR     CFG_MULTI_PVR_4A;                                       // reg_sgdma_out_dbg

    REG16_MULTI_PVR     CFG_MULTI_PVR_4B;                                       // reg_sgdma_out_vc_int (only bit[7:0])
        #define CFG_MULTI_PVR_4B_REG_SGDMA_OUT_VC_INT_MASK                      0x00FF
        #define CFG_MULTI_PVR_4B_REG_SGDMA_OUT_VC_INT_SHIFT                     0

    REG16_MULTI_PVR     CFG_MULTI_PVR_4C_4E[0x4F - 0x4C];                       // reg_sgdma_out_vc_int (dummy)

    REG16_MULTI_PVR     CFG_MULTI_PVR_4F;
        #define CFG_MULTI_PVR_4F_REG_SGDMA_OUT_VC_INT_VC_ID_MASK                0x003F
        #define CFG_MULTI_PVR_4F_REG_SGDMA_OUT_VC_INT_VC_ID_SHIFT               0
        #define CFG_MULTI_PVR_4F_REG_SGDMA_OUT_VC_INT_CLR                       0x0040  // clear interrupt
        #define CFG_MULTI_PVR_4F_REG_SGDMA_OUT_VC_INT_MASK                      0x0080  // mask interrupt

    REG16_MULTI_PVR     CFG_MULTI_PVR_50;
        #define CFG_MULTI_PVR_50_REG_SGDMA_OUT_VC_STATUS_SEL_MASK               0x00FC
        #define CFG_MULTI_PVR_50_REG_SGDMA_OUT_VC_STATUS_SEL_SHIFT              2

    REG16_MULTI_PVR     CFG_MULTI_PVR_51;                                       // reg_sgdma_out_vc_status
        #define CFG_MULTI_PVR_51_REG_SGDMA_OUT_VC_STATUS_ACTIVE                 0x0001
        #define CFG_MULTI_PVR_51_REG_SGDMA_OUT_VC_STATUS_PINGPONG_PTR           0x0020

    REG16_MULTI_PVR     CFG_MULTI_PVR_52_6F[0x70 - 0x52];                       // reserved

    REG16_MULTI_PVR     CFG_MULTI_PVR_70;
        #define CFG_MULTI_PVR_70_REG_START_READ_BYPASS_EN                       0x0001
        #define CFG_MULTI_PVR_70_REG_CLR_PIDFLT_BYTE_CNT                        0x0002
        #define CFG_MULTI_PVR_70_REG_PVR_ERR_RM_EN                              0x0004
        #define CFG_MULTI_PVR_70_REG_MASK_SCR_PVR_EN                            0x0008
        #define CFG_MULTI_PVR_70_REG_DIS_NULL_PKT                               0x0010
        #define CFG_MULTI_PVR_70_REG_TEI_SKIP_PKT                               0x0020
        #define CFG_MULTI_PVR_70_REG_RECORD_TS                                  0x0040
        #define CFG_MULTI_PVR_70_REG_RECORD_ALL                                 0x0080
        #define CFG_MULTI_PVR_70_REG_SKIP_PVR_RUSH_DATA                         0x0100
        #define CFG_MULTI_PVR_70_REG_ALT_TS_SIZE                                0x0200
        #define CFG_MULTI_PVR_70_REG_PVR_BLOCK_DISABLE                          0x0400
        #define CFG_MULTI_PVR_70_REG_PVR_PES_DIRECTV_130_MODE                   0x0800
        #define CFG_MULTI_PVR_70_REG_RESET_FILTER                               0x4000
        #define CFG_MULTI_PVR_70_REG_ONEWAY_PVR                                 0x8000

    REG16_MULTI_PVR     CFG_MULTI_PVR_71;
        #define CFG_MULTI_PVR_71_REG_PKT_SIZE_MASK                              0x00FF
        #define CFG_MULTI_PVR_71_REG_PKT_SIZE_SHIFT                             0
        #define CFG_MULTI_PVR_71_REG_INPUT_SRC_MASK                             0x0F00
        #define CFG_MULTI_PVR_71_REG_INPUT_SRC_SHIFT                            8

    REG16_MULTI_PVR     CFG_MULTI_PVR_72;
        #define CFG_MULTI_PVR_72_REG_CHK_PRIVILEGE_FLAG                         0x0001
        #define CFG_MULTI_PVR_72_REG_CHK_TEE_FILEIN                             0x0002

    REG16_MULTI_PVR     CFG_MULTI_PVR_73;
        #define CFG_MULTI_PVR_73_REG_ONEWAY_REC_CA_UPPER_PATH                   0x0001
        #define CFG_MULTI_PVR_73_REG_REC_CA_UPPER_PATH                          0x0002

    REG16_MULTI_PVR     CFG_MULTI_PVR_74_7E[0x7F - 0x74];                       // reserved

    REG16_MULTI_PVR     CFG_MULTI_PVR_7F;
        #define CFG_MULTI_PVR_7F_REG_CLK_GATING_TSP_PVR                         0x0001
        #define CFG_MULTI_PVR_7F_REG_CLK_GATING_MIU_PVR                         0x0002

} REG_MULTI_PVR_ENG_Ctrl;

#endif // #ifndef _REG_MULTI_PVR_H_
