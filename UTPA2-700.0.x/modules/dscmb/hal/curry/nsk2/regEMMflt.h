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

#ifndef _REGEMMFLT_H_
#define _REGEMMFLT_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
#define __BIT(x)    ((MS_U16)(1 << (x)))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)
#define __BIT8       __BIT(8)
#define __BIT9       __BIT(9)
#define __BITA       __BIT(0xA)
#define __BITB       __BIT(0xB)
#define __BITC       __BIT(0xC)
#define __BITD       __BIT(0xD)
#define __BITE       __BIT(0xE)
#define __BITF       __BIT(0xF)


#define BYTE0        0xFF
#define BYTE1        (0xFF<<8)
#define BYTE0_MASK   0x00FF
#define BYTE1_MASK   0xFF00

/* KERES */
#define REG_EMMFLT_BASE1        (0x170500<<1)
#define REG_EMMFLT_BASE2        (0x161400<<1)
#define REG_OTP_BASE            (0x190000<<1)
#define REG_OTP_CTRL_BASE       (0x1A1300<<1)
#define REG_PVR0_BASE           (0x1A1100<<1)
#define REG_CHIPTOP_BASE        (0x101E00<<1)
#define REG_CLKGEN0_BASE        (0x100B00<<1)
#define REG_CLKGEN2_BASE        (0x100A00<<1)




#define REG_EMM_TID_MODE_L      0x0000
#define REG_EMM_TID_MODE_H      0x0001
#define REG_EMM_DATA_ID1_L      0x0002
#define REG_EMM_DATA_ID1_H      0x0003
#define REG_EMM_DATA_ID2_L      0x0004
#define REG_EMM_DATA_ID2_H      0x0005
#define REG_EMM_DATA_ID3_L      0x0006
#define REG_EMM_DATA_ID3_H      0x0007

#define REG_EMM_MASK_ID1_L      0x000A
#define REG_EMM_MASK_ID1_H      0x000B
#define REG_EMM_MASK_ID2_L      0x000C
#define REG_EMM_MASK_ID2_H      0x000D
#define REG_EMM_MASK_ID3_L      0x000E
#define REG_EMM_MASK_ID3_H      0x000F

#define REG_EMM_PID             0x0010
    #define REG_EMM_ENABLE_TID      __BITE
    #define EMM_ENABLE_TID_SHIFT    0xE
    #define REG_EMM_ENABLE_PID      __BITF
    #define EMM_ENABLE_PID_SHIFT    0xF

#define REG_EMM_TID             0x0011

#define REG_EMM_CA_INT          0x0018
    #define EMM_ONEPAKCET_INT           __BIT1
    #define EMM_OVERFLOW_INT            __BIT2
    #define EMM_RESET_INT               __BIT7
    #define EMM_INT_MASK                BMASK(7:0)

#define REG_EMM_DATA_ID4_L      0x0038
#define REG_EMM_DATA_ID4_H      0x0039
#define REG_EMM_DATA_ID5_L      0x003A
#define REG_EMM_DATA_ID5_H      0x003B
#define REG_EMM_DATA_ID6_L      0x003C
#define REG_EMM_DATA_ID6_H      0x003D
#define REG_EMM_DATA_ID7_L      0x003E
#define REG_EMM_DATA_ID7_H      0x003F
#define REG_EMM_DATA_ID8_L      0x0040
#define REG_EMM_DATA_ID8_H      0x0041

#define REG_EMM_MASK_ID4_L      0x0042
#define REG_EMM_MASK_ID4_H      0x0043
#define REG_EMM_MASK_ID5_L      0x0044
#define REG_EMM_MASK_ID5_H      0x0045
#define REG_EMM_MASK_ID6_L      0x0046
#define REG_EMM_MASK_ID6_H      0x0047
#define REG_EMM_MASK_ID7_L      0x0048
#define REG_EMM_MASK_ID7_H      0x0049
#define REG_EMM_MASK_ID8_L      0x004A
#define REG_EMM_MASK_ID8_H      0x004B

#define REG_EMM_CTRL_ID         0x004C
    #define REG_EMM_CTRL_MAX        0x3
    #define REG_EMM_IRD_MAX         0x8

#define REG_EMM_INT_STAT        0x004D
    #define EMM_RECIEVE_ACT         0x10

#define REG_EMM_RESET           0x004E
    #define EMM_RESET_UNLOCK         0x1


#define REG_EMM_CTRL0_L         0x0051
    #define EMM_PACKET256_EN           __BIT0
    #define EMM_PVR_EN                 __BIT1
    #define EMM_CLR_PVR_OVERFLOW       __BIT2
    #define EMM_DMA_FLUSH_EN           __BIT3
    #define EMM_FORCE_SYNC_EN          __BIT4
    #define EMM_FW_FILEIN              __BITC
    #define EMM_FLT_BYPASS             __BITF

#define REG_EMM_CTRL0_H         0x0052

#define REG_EMM_GENERAL_CTRL_L  0x0059
    #define EMM_TS_INPUT_SELECT        BMASK(1:0)
    #define EMM_FIRST_4_BYTES          __BIT2
    #define EMM_VERSION_NUMBER         BMASK(11:4)


#define REG_EMM_GENERAL_CTRL_H  0x005A
    #define EMM_IGNORE_TS_INPUT        __BITF

#define REG_EMM_STR2MIU_EN      0x005B
    #define EMM_PINGPONG_EN             __BIT0
    #define EMM_STR2MIU_EN              __BIT1
    #define EMM_STR2MIU_RST_WADR        __BIT2
    #define EMM_STR2MIU_DATA_SWAP       __BIT3
    #define EMM_STR2MIU_BIT_ORDER       __BIT4
    #define EMM_STR2MIU_PAUSE           __BIT5
    #define EMM_SKIP_ADDR_EN            __BIT6

#define REG_EMM_STR2MIU_HEAD1_L 0x005C
#define REG_EMM_STR2MIU_HEAD1_H 0x005D
#define REG_EMM_STR2MIU_TAIL1_L 0x005E
#define REG_EMM_STR2MIU_TAIL1_H 0x005F
#define REG_EMM_STR2MIU_MID1_L  0x0060
#define REG_EMM_STR2MIU_MID1_H  0x0061

#define REG_EMM_STR2MIU_HEAD2_L 0x0062
#define REG_EMM_STR2MIU_HEAD2_H 0x0063
#define REG_EMM_STR2MIU_TAIL2_L 0x0064
#define REG_EMM_STR2MIU_TAIL2_H 0x0065
#define REG_EMM_STR2MIU_MID2_L  0x0066
#define REG_EMM_STR2MIU_MID2_H  0x0067


#define REG_EMM_STR2MIU_CTRL    0x0068
    #define REG_STR_OVER_CTRL           __BIT0
    #define REG_MIU_HIGH_PRI            __BIT1
    #define REG_WADR_PROTECT_EN         __BIT2
    #define REG_MI2STR_WD_EN            __BIT3
    #define REG_LAST_STR_WD_EN          __BIT4
    #define REG_STR2MI_WP_LD            __BIT5
    #define REG_PKT192_EN               __BIT6
    #define REG_LPCR1_WLD               __BIT7
    #define REG_BURST_LEN               (__BIT8 | __BIT9)
    #define REG_RECORD_AT_SYNC_DIS      __BITA


#define REG_EMM_LPCR1_BUF_L     0x0069
#define REG_EMM_LPCR1_BUF_H     0x006A

#define REG_EMM_TS_IF2_CTRL     0x0070
    #define REG_TS_IF2_EN               __BIT0
    #define REG_TS_DATA2_SWAP           __BIT1
    #define REG_SIM_C0_CONFIG           __BIT2
    #define REG_SIM_C1_CONFIG           __BIT3
    #define REG_FORCE_SYNCBYTE          __BIT4
    #define REG_P_SEL2                  __BIT5
    #define REG_EXT_SYNC_SEL2           __BIT6
    #define REG_DATA_CHK_2T             __BIT7
    #define REG_EMM_DGB_SEL             (__BIT8 | __BIT9 | __BITA | __BITB)
    #define REG_SERIAL_EXT_SYNC_1T      __BITC
    #define REG_SW_RSTS                 __BITF

#define REG_EMM_SYNC_BYTES      0x0071
    #define REG_SYNC_BYTES              BYTE0
    #define REG_PKT_CHK_SIZE2           BYTE1

#define REG_EMM_TS_IF2_DEBUG_L  0x0072
#define REG_EMM_TS_IF2_DEBUG_H  0x0073

#define REG_EMM_TSIF_LOCKED_CNT_STATUS  0x0074
    #define REG_TSIF_LOCKED_CNT_INC         __BIT0
    #define REG_TSIF_LOSE_LOCKED_CNT_INC    __BIT1

#define REG_EMM_PACKET_CNT      0x0076

#define REG_EMM_HW_CONFIG0      0x007C

#endif //_REGEMMFLT_H_

