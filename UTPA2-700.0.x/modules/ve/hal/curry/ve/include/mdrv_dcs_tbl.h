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

#ifndef _DRVDCS_TBL_H_
#define _DRVDCS_TBL_H_

#ifdef __cplusplus
extern "C"
{
#endif

//ives DCS
#if 1
#define DRV_DCS_REG(reg)      ((reg>>8)&0xFF), (reg&0xFF)
#endif

#define REG_ADDR_SIZE          2
#define REG_MASK_SIZE          1
#define REG_TABLE_END          0xFFFF

//ives DCS
#if 1
#define DCS_DCSTBL_EXTEND_SIZE          2
#endif

#define _PK_L_(bank, addr)     ((MS_U16)(bank << 8) | (addr))
#define _PK_H_(bank, addr)     ((MS_U16)((bank|0x80) << 8) | (addr))

#define  REG_TC_COMPONENT_BASE     0x3000

//#define  REG_TC_VE_SRC_BASE     0x3B00
//#define  REG_TC_VE_ENC1_BASE     0x3E00
#define  REG_TC_VE_ENC2_BASE     0x3F00

typedef enum
{
    E_DCSST_IS_ACTIVITIVE_KEYBYTE = 0,
    E_DCSST_IS_NOT_ACTIVITIVE_KEYBYTE,
} E_DCS_STATE_TRANSITION_ACTIVITIVE_KEYBYTE_OR_NOT;

typedef enum
{
    E_DCS_STATE_OFF_TO_TRANSITIONON = 0,
    E_DCS_STATE_TRANSITIONON_TO_ON,
    E_DCS_STATE_ON_TO_TRANSITIONOFF,
    E_DCS_STATE_TRANSITIONOFF_TO_OFF,
    E_DCS_STATE_TRANSITION_TYPE_NUM,
} E_DCS_STATE_TRANSITION_TYPE;

typedef enum
{
    E_DCSST_ACTIVITIVE_KEYBYTE_0 = 0,
    E_DCSST_ACTIVITIVE_KEYBYTE_1,
    E_DCSST_ACTIVITIVE_KEYBYTE_2,
    E_DCSST_ACTIVITIVE_KEYBYTE_3,
    E_DCSST_ACTIVITIVE_KEYBYTE_4,
    E_DCSST_ACTIVITIVE_KEYBYTE_5,
    E_DCSST_ACTIVITIVE_KEYBYTE_6,
    E_DCSST_ACTIVITIVE_KEYBYTE_7,
    E_DCSST_ACTIVITIVE_KEYBYTE_NUM,
} E_DCS_STATE_TRANSITION_ACTIVITIVE_KEYBYTE_INDEX;

typedef enum
{
    E_DCS_TABTYPE_MIN = 0x10,
    E_DCS_TABTYPE_CVBS = E_DCS_TABTYPE_MIN,
    E_DCS_TABTYPE_COMPONENT,
    E_DCS_TABTYPE_MAX = E_DCS_TABTYPE_COMPONENT,
    E_DCS_TABTYPE_NUM = (E_DCS_TABTYPE_MAX - E_DCS_TABTYPE_MIN + 1),
} E_DCS_TAB_TYPE;

typedef struct
{
    MS_U8           *pVeDCSTab;
    E_DCS_TAB_TYPE   eDCStype;
} DCS_TAB_INFO;

typedef enum
{
    E_DCS_TABLE_INDEX_MIN = 0,
    E_DCS_TABLE_DCS_CVBS_NTSC = E_DCS_TABLE_INDEX_MIN,
    E_DCS_TABLE_DCS_CVBS_PAL,
    E_DCS_TABLE_DCS_COMPONENT_480I,
    E_DCS_TABLE_DCS_COMPONENT_480P,
    E_DCS_TABLE_DCS_COMPONENT_576I,
    E_DCS_TABLE_DCS_COMPONENT_576P,
    E_DCS_TABLE_DCS_COMPONENT_720P_24,
    E_DCS_TABLE_DCS_COMPONENT_720P_25,
    E_DCS_TABLE_DCS_COMPONENT_720P_30,
    E_DCS_TABLE_DCS_COMPONENT_720P_50,
    E_DCS_TABLE_DCS_COMPONENT_720P_60,
    E_DCS_TABLE_DCS_COMPONENT_1080I_50,
    E_DCS_TABLE_DCS_COMPONENT_1080I_60,
    E_DCS_TABLE_DCS_COMPONENT_1080P_24,
    E_DCS_TABLE_DCS_COMPONENT_1080P_25,
    E_DCS_TABLE_DCS_COMPONENT_1080P_30,
    E_DCS_TABLE_DCS_COMPONENT_1080P_50,
    E_DCS_TABLE_DCS_COMPONENT_1080P_60,
    E_DCS_TABLE_DCS_INDEX_MAX = E_DCS_TABLE_DCS_COMPONENT_1080P_60,
    E_DCS_TABLE_DCS_INDEX_NUM,
} E_DCS_TABLE_INDEX;

extern MS_U8 DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEY_TABLE[E_DCS_STATE_TRANSITION_TYPE_NUM][E_DCSST_ACTIVITIVE_KEYBYTE_NUM];
extern MS_U8 *DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL;
extern DCS_TAB_INFO DCSMAP_Main[E_DCS_TABLE_DCS_INDEX_NUM];

typedef enum
{
    E_DCS_TAB_DCS_CVBS_NTSC_EXTEND_NONE,
    E_DCS_TAB_DCS_CVBS_NTSC_EXTEND_ONE,
    //E_DCS_TAB_DCS_CVBS_NTSC_EXTEND_TWO,
} E_DCS_TAB_DCS_CVBS_NTSC_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_CVBS_PAL_EXTEND_NONE,
    E_DCS_TAB_DCS_CVBS_PAL_EXTEND_ONE,
    //E_DCS_TAB_DCS_CVBS_PAL_EXTEND_TWO,
} E_DCS_TAB_DCS_CVBS_PAL_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_480I_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_480I_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_480I_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_480I_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_480P_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_480P_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_480P_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_480P_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_576I_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_576I_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_576I_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_576I_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_576P_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_576P_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_576P_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_576P_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_720P_24_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_720P_24_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_720P_24_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_720P_24_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_720P_25_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_720P_25_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_720P_25_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_720P_25_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_720P_30_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_720P_30_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_720P_30_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_720P_30_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_720P_50_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_720P_50_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_720P_50_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_720P_50_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_720P_60_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_720P_60_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_720P_60_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_720P_60_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080I_50_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080I_50_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080I_50_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080I_50_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080I_60_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080I_60_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080I_60_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080I_60_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080P_24_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080P_24_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080P_24_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080P_24_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080P_25_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080P_25_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080P_25_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080P_25_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080P_30_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080P_30_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080P_30_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080P_30_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080P_50_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080P_50_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080P_50_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080P_50_EXTEND;

typedef enum
{
    E_DCS_TAB_DCS_COMPONENT_1080P_60_EXTEND_NONE,
    E_DCS_TAB_DCS_COMPONENT_1080P_60_EXTEND_ONE,
    //E_DCS_TAB_DCS_COMPONENT_1080P_60_EXTEND_TWO,
} E_DCS_TAB_DCS_COMPONENT_1080P_60_EXTEND;

//-------------------------------------------------------------------------------------------------
// register define used for TCON table
//-------------------------------------------------------------------------------------------------
#if 0
#define  REG_TC_VE_SRC_00_L       (REG_TC_VE_SRC_BASE + 0x00)
#define  REG_TC_VE_SRC_00_H       (REG_TC_VE_SRC_BASE + 0x01)
#define  REG_TC_VE_SRC_01_L       (REG_TC_VE_SRC_BASE + 0x02)
#define  REG_TC_VE_SRC_01_H       (REG_TC_VE_SRC_BASE + 0x03)
#define  REG_TC_VE_SRC_02_L       (REG_TC_VE_SRC_BASE + 0x04)
#define  REG_TC_VE_SRC_02_H       (REG_TC_VE_SRC_BASE + 0x05)
#define  REG_TC_VE_SRC_03_L       (REG_TC_VE_SRC_BASE + 0x06)
#define  REG_TC_VE_SRC_03_H       (REG_TC_VE_SRC_BASE + 0x07)
#define  REG_TC_VE_SRC_04_L       (REG_TC_VE_SRC_BASE + 0x08)
#define  REG_TC_VE_SRC_04_H       (REG_TC_VE_SRC_BASE + 0x09)
#define  REG_TC_VE_SRC_05_L       (REG_TC_VE_SRC_BASE + 0x0A)
#define  REG_TC_VE_SRC_05_H       (REG_TC_VE_SRC_BASE + 0x0B)
#define  REG_TC_VE_SRC_06_L       (REG_TC_VE_SRC_BASE + 0x0C)
#define  REG_TC_VE_SRC_06_H       (REG_TC_VE_SRC_BASE + 0x0D)
#define  REG_TC_VE_SRC_07_L       (REG_TC_VE_SRC_BASE + 0x0E)
#define  REG_TC_VE_SRC_07_H       (REG_TC_VE_SRC_BASE + 0x0F)
#define  REG_TC_VE_SRC_08_L       (REG_TC_VE_SRC_BASE + 0x10)
#define  REG_TC_VE_SRC_08_H       (REG_TC_VE_SRC_BASE + 0x11)
#define  REG_TC_VE_SRC_09_L       (REG_TC_VE_SRC_BASE + 0x12)
#define  REG_TC_VE_SRC_09_H       (REG_TC_VE_SRC_BASE + 0x13)
#define  REG_TC_VE_SRC_0A_L       (REG_TC_VE_SRC_BASE + 0x14)
#define  REG_TC_VE_SRC_0A_H       (REG_TC_VE_SRC_BASE + 0x15)
#define  REG_TC_VE_SRC_0B_L       (REG_TC_VE_SRC_BASE + 0x16)
#define  REG_TC_VE_SRC_0B_H       (REG_TC_VE_SRC_BASE + 0x17)
#define  REG_TC_VE_SRC_0C_L       (REG_TC_VE_SRC_BASE + 0x18)
#define  REG_TC_VE_SRC_0C_H       (REG_TC_VE_SRC_BASE + 0x19)
#define  REG_TC_VE_SRC_0D_L       (REG_TC_VE_SRC_BASE + 0x1A)
#define  REG_TC_VE_SRC_0D_H       (REG_TC_VE_SRC_BASE + 0x1B)
#define  REG_TC_VE_SRC_0E_L       (REG_TC_VE_SRC_BASE + 0x1C)
#define  REG_TC_VE_SRC_0E_H       (REG_TC_VE_SRC_BASE + 0x1D)
#define  REG_TC_VE_SRC_0F_L       (REG_TC_VE_SRC_BASE + 0x1E)
#define  REG_TC_VE_SRC_0F_H       (REG_TC_VE_SRC_BASE + 0x1F)
#define  REG_TC_VE_SRC_10_L       (REG_TC_VE_SRC_BASE + 0x20)
#define  REG_TC_VE_SRC_10_H       (REG_TC_VE_SRC_BASE + 0x21)
#define  REG_TC_VE_SRC_11_L       (REG_TC_VE_SRC_BASE + 0x22)
#define  REG_TC_VE_SRC_11_H       (REG_TC_VE_SRC_BASE + 0x23)
#define  REG_TC_VE_SRC_12_L       (REG_TC_VE_SRC_BASE + 0x24)
#define  REG_TC_VE_SRC_12_H       (REG_TC_VE_SRC_BASE + 0x25)
#define  REG_TC_VE_SRC_13_L       (REG_TC_VE_SRC_BASE + 0x26)
#define  REG_TC_VE_SRC_13_H       (REG_TC_VE_SRC_BASE + 0x27)
#define  REG_TC_VE_SRC_14_L       (REG_TC_VE_SRC_BASE + 0x28)
#define  REG_TC_VE_SRC_14_H       (REG_TC_VE_SRC_BASE + 0x29)
#define  REG_TC_VE_SRC_15_L       (REG_TC_VE_SRC_BASE + 0x2A)
#define  REG_TC_VE_SRC_15_H       (REG_TC_VE_SRC_BASE + 0x2B)
#define  REG_TC_VE_SRC_16_L       (REG_TC_VE_SRC_BASE + 0x2C)
#define  REG_TC_VE_SRC_16_H       (REG_TC_VE_SRC_BASE + 0x2D)
#define  REG_TC_VE_SRC_17_L       (REG_TC_VE_SRC_BASE + 0x2E)
#define  REG_TC_VE_SRC_17_H       (REG_TC_VE_SRC_BASE + 0x2F)
#define  REG_TC_VE_SRC_18_L       (REG_TC_VE_SRC_BASE + 0x30)
#define  REG_TC_VE_SRC_18_H       (REG_TC_VE_SRC_BASE + 0x31)
#define  REG_TC_VE_SRC_19_L       (REG_TC_VE_SRC_BASE + 0x32)
#define  REG_TC_VE_SRC_19_H       (REG_TC_VE_SRC_BASE + 0x33)
#define  REG_TC_VE_SRC_1A_L       (REG_TC_VE_SRC_BASE + 0x34)
#define  REG_TC_VE_SRC_1A_H       (REG_TC_VE_SRC_BASE + 0x35)
#define  REG_TC_VE_SRC_1B_L       (REG_TC_VE_SRC_BASE + 0x36)
#define  REG_TC_VE_SRC_1B_H       (REG_TC_VE_SRC_BASE + 0x37)
#define  REG_TC_VE_SRC_1C_L       (REG_TC_VE_SRC_BASE + 0x38)
#define  REG_TC_VE_SRC_1C_H       (REG_TC_VE_SRC_BASE + 0x39)
#define  REG_TC_VE_SRC_1D_L       (REG_TC_VE_SRC_BASE + 0x3A)
#define  REG_TC_VE_SRC_1D_H       (REG_TC_VE_SRC_BASE + 0x3B)
#define  REG_TC_VE_SRC_1E_L       (REG_TC_VE_SRC_BASE + 0x3C)
#define  REG_TC_VE_SRC_1E_H       (REG_TC_VE_SRC_BASE + 0x3D)
#define  REG_TC_VE_SRC_1F_L       (REG_TC_VE_SRC_BASE + 0x3E)
#define  REG_TC_VE_SRC_1F_H       (REG_TC_VE_SRC_BASE + 0x3F)
#define  REG_TC_VE_SRC_20_L       (REG_TC_VE_SRC_BASE + 0x40)
#define  REG_TC_VE_SRC_20_H       (REG_TC_VE_SRC_BASE + 0x41)
#define  REG_TC_VE_SRC_21_L       (REG_TC_VE_SRC_BASE + 0x42)
#define  REG_TC_VE_SRC_21_H       (REG_TC_VE_SRC_BASE + 0x43)
#define  REG_TC_VE_SRC_22_L       (REG_TC_VE_SRC_BASE + 0x44)
#define  REG_TC_VE_SRC_22_H       (REG_TC_VE_SRC_BASE + 0x45)
#define  REG_TC_VE_SRC_23_L       (REG_TC_VE_SRC_BASE + 0x46)
#define  REG_TC_VE_SRC_23_H       (REG_TC_VE_SRC_BASE + 0x47)
#define  REG_TC_VE_SRC_24_L       (REG_TC_VE_SRC_BASE + 0x48)
#define  REG_TC_VE_SRC_24_H       (REG_TC_VE_SRC_BASE + 0x49)
#define  REG_TC_VE_SRC_25_L       (REG_TC_VE_SRC_BASE + 0x4A)
#define  REG_TC_VE_SRC_25_H       (REG_TC_VE_SRC_BASE + 0x4B)
#define  REG_TC_VE_SRC_26_L       (REG_TC_VE_SRC_BASE + 0x4C)
#define  REG_TC_VE_SRC_26_H       (REG_TC_VE_SRC_BASE + 0x4D)
#define  REG_TC_VE_SRC_27_L       (REG_TC_VE_SRC_BASE + 0x4E)
#define  REG_TC_VE_SRC_27_H       (REG_TC_VE_SRC_BASE + 0x4F)
#define  REG_TC_VE_SRC_28_L       (REG_TC_VE_SRC_BASE + 0x50)
#define  REG_TC_VE_SRC_28_H       (REG_TC_VE_SRC_BASE + 0x51)
#define  REG_TC_VE_SRC_29_L       (REG_TC_VE_SRC_BASE + 0x52)
#define  REG_TC_VE_SRC_29_H       (REG_TC_VE_SRC_BASE + 0x53)
#define  REG_TC_VE_SRC_2A_L       (REG_TC_VE_SRC_BASE + 0x54)
#define  REG_TC_VE_SRC_2A_H       (REG_TC_VE_SRC_BASE + 0x55)
#define  REG_TC_VE_SRC_2B_L       (REG_TC_VE_SRC_BASE + 0x56)
#define  REG_TC_VE_SRC_2B_H       (REG_TC_VE_SRC_BASE + 0x57)
#define  REG_TC_VE_SRC_2C_L       (REG_TC_VE_SRC_BASE + 0x58)
#define  REG_TC_VE_SRC_2C_H       (REG_TC_VE_SRC_BASE + 0x59)
#define  REG_TC_VE_SRC_2D_L       (REG_TC_VE_SRC_BASE + 0x5A)
#define  REG_TC_VE_SRC_2D_H       (REG_TC_VE_SRC_BASE + 0x5B)
#define  REG_TC_VE_SRC_2E_L       (REG_TC_VE_SRC_BASE + 0x5C)
#define  REG_TC_VE_SRC_2E_H       (REG_TC_VE_SRC_BASE + 0x5D)
#define  REG_TC_VE_SRC_2F_L       (REG_TC_VE_SRC_BASE + 0x5E)
#define  REG_TC_VE_SRC_2F_H       (REG_TC_VE_SRC_BASE + 0x5F)
#define  REG_TC_VE_SRC_30_L       (REG_TC_VE_SRC_BASE + 0x60)
#define  REG_TC_VE_SRC_30_H       (REG_TC_VE_SRC_BASE + 0x61)
#define  REG_TC_VE_SRC_31_L       (REG_TC_VE_SRC_BASE + 0x62)
#define  REG_TC_VE_SRC_31_H       (REG_TC_VE_SRC_BASE + 0x63)
#define  REG_TC_VE_SRC_32_L       (REG_TC_VE_SRC_BASE + 0x64)
#define  REG_TC_VE_SRC_32_H       (REG_TC_VE_SRC_BASE + 0x65)
#define  REG_TC_VE_SRC_33_L       (REG_TC_VE_SRC_BASE + 0x66)
#define  REG_TC_VE_SRC_33_H       (REG_TC_VE_SRC_BASE + 0x67)
#define  REG_TC_VE_SRC_34_L       (REG_TC_VE_SRC_BASE + 0x68)
#define  REG_TC_VE_SRC_34_H       (REG_TC_VE_SRC_BASE + 0x69)
#define  REG_TC_VE_SRC_35_L       (REG_TC_VE_SRC_BASE + 0x6A)
#define  REG_TC_VE_SRC_35_H       (REG_TC_VE_SRC_BASE + 0x6B)
#define  REG_TC_VE_SRC_36_L       (REG_TC_VE_SRC_BASE + 0x6C)
#define  REG_TC_VE_SRC_36_H       (REG_TC_VE_SRC_BASE + 0x6D)
#define  REG_TC_VE_SRC_37_L       (REG_TC_VE_SRC_BASE + 0x6E)
#define  REG_TC_VE_SRC_37_H       (REG_TC_VE_SRC_BASE + 0x6F)
#define  REG_TC_VE_SRC_38_L       (REG_TC_VE_SRC_BASE + 0x70)
#define  REG_TC_VE_SRC_38_H       (REG_TC_VE_SRC_BASE + 0x71)
#define  REG_TC_VE_SRC_39_L       (REG_TC_VE_SRC_BASE + 0x72)
#define  REG_TC_VE_SRC_39_H       (REG_TC_VE_SRC_BASE + 0x73)
#define  REG_TC_VE_SRC_3A_L       (REG_TC_VE_SRC_BASE + 0x74)
#define  REG_TC_VE_SRC_3A_H       (REG_TC_VE_SRC_BASE + 0x75)
#define  REG_TC_VE_SRC_3B_L       (REG_TC_VE_SRC_BASE + 0x76)
#define  REG_TC_VE_SRC_3B_H       (REG_TC_VE_SRC_BASE + 0x77)
#define  REG_TC_VE_SRC_3C_L       (REG_TC_VE_SRC_BASE + 0x78)
#define  REG_TC_VE_SRC_3C_H       (REG_TC_VE_SRC_BASE + 0x79)
#define  REG_TC_VE_SRC_3D_L       (REG_TC_VE_SRC_BASE + 0x7A)
#define  REG_TC_VE_SRC_3D_H       (REG_TC_VE_SRC_BASE + 0x7B)
#define  REG_TC_VE_SRC_3E_L       (REG_TC_VE_SRC_BASE + 0x7C)
#define  REG_TC_VE_SRC_3E_H       (REG_TC_VE_SRC_BASE + 0x7D)
#define  REG_TC_VE_SRC_3F_L       (REG_TC_VE_SRC_BASE + 0x7E)
#define  REG_TC_VE_SRC_3F_H       (REG_TC_VE_SRC_BASE + 0x7F)
#define  REG_TC_VE_SRC_40_L       (REG_TC_VE_SRC_BASE + 0x80)
#define  REG_TC_VE_SRC_40_H       (REG_TC_VE_SRC_BASE + 0x81)
#define  REG_TC_VE_SRC_41_L       (REG_TC_VE_SRC_BASE + 0x82)
#define  REG_TC_VE_SRC_41_H       (REG_TC_VE_SRC_BASE + 0x83)
#define  REG_TC_VE_SRC_42_L       (REG_TC_VE_SRC_BASE + 0x84)
#define  REG_TC_VE_SRC_42_H       (REG_TC_VE_SRC_BASE + 0x85)
#define  REG_TC_VE_SRC_43_L       (REG_TC_VE_SRC_BASE + 0x86)
#define  REG_TC_VE_SRC_43_H       (REG_TC_VE_SRC_BASE + 0x87)
#define  REG_TC_VE_SRC_44_L       (REG_TC_VE_SRC_BASE + 0x88)
#define  REG_TC_VE_SRC_44_H       (REG_TC_VE_SRC_BASE + 0x89)
#define  REG_TC_VE_SRC_45_L       (REG_TC_VE_SRC_BASE + 0x8A)
#define  REG_TC_VE_SRC_45_H       (REG_TC_VE_SRC_BASE + 0x8B)
#define  REG_TC_VE_SRC_46_L       (REG_TC_VE_SRC_BASE + 0x8C)
#define  REG_TC_VE_SRC_46_H       (REG_TC_VE_SRC_BASE + 0x8D)
#define  REG_TC_VE_SRC_47_L       (REG_TC_VE_SRC_BASE + 0x8E)
#define  REG_TC_VE_SRC_47_H       (REG_TC_VE_SRC_BASE + 0x8F)
#define  REG_TC_VE_SRC_48_L       (REG_TC_VE_SRC_BASE + 0x90)
#define  REG_TC_VE_SRC_48_H       (REG_TC_VE_SRC_BASE + 0x91)
#define  REG_TC_VE_SRC_49_L       (REG_TC_VE_SRC_BASE + 0x92)
#define  REG_TC_VE_SRC_49_H       (REG_TC_VE_SRC_BASE + 0x93)
#define  REG_TC_VE_SRC_4A_L       (REG_TC_VE_SRC_BASE + 0x94)
#define  REG_TC_VE_SRC_4A_H       (REG_TC_VE_SRC_BASE + 0x95)
#define  REG_TC_VE_SRC_4B_L       (REG_TC_VE_SRC_BASE + 0x96)
#define  REG_TC_VE_SRC_4B_H       (REG_TC_VE_SRC_BASE + 0x97)
#define  REG_TC_VE_SRC_4C_L       (REG_TC_VE_SRC_BASE + 0x98)
#define  REG_TC_VE_SRC_4C_H       (REG_TC_VE_SRC_BASE + 0x99)
#define  REG_TC_VE_SRC_4D_L       (REG_TC_VE_SRC_BASE + 0x9A)
#define  REG_TC_VE_SRC_4D_H       (REG_TC_VE_SRC_BASE + 0x9B)
#define  REG_TC_VE_SRC_4E_L       (REG_TC_VE_SRC_BASE + 0x9C)
#define  REG_TC_VE_SRC_4E_H       (REG_TC_VE_SRC_BASE + 0x9D)
#define  REG_TC_VE_SRC_4F_L       (REG_TC_VE_SRC_BASE + 0x9E)
#define  REG_TC_VE_SRC_4F_H       (REG_TC_VE_SRC_BASE + 0x9F)
#define  REG_TC_VE_SRC_50_L       (REG_TC_VE_SRC_BASE + 0xA0)
#define  REG_TC_VE_SRC_50_H       (REG_TC_VE_SRC_BASE + 0xA1)
#define  REG_TC_VE_SRC_51_L       (REG_TC_VE_SRC_BASE + 0xA2)
#define  REG_TC_VE_SRC_51_H       (REG_TC_VE_SRC_BASE + 0xA3)
#define  REG_TC_VE_SRC_52_L       (REG_TC_VE_SRC_BASE + 0xA4)
#define  REG_TC_VE_SRC_52_H       (REG_TC_VE_SRC_BASE + 0xA5)
#define  REG_TC_VE_SRC_53_L       (REG_TC_VE_SRC_BASE + 0xA6)
#define  REG_TC_VE_SRC_53_H       (REG_TC_VE_SRC_BASE + 0xA7)
#define  REG_TC_VE_SRC_54_L       (REG_TC_VE_SRC_BASE + 0xA8)
#define  REG_TC_VE_SRC_54_H       (REG_TC_VE_SRC_BASE + 0xA9)
#define  REG_TC_VE_SRC_55_L       (REG_TC_VE_SRC_BASE + 0xAA)
#define  REG_TC_VE_SRC_55_H       (REG_TC_VE_SRC_BASE + 0xAB)
#define  REG_TC_VE_SRC_56_L       (REG_TC_VE_SRC_BASE + 0xAC)
#define  REG_TC_VE_SRC_56_H       (REG_TC_VE_SRC_BASE + 0xAD)
#define  REG_TC_VE_SRC_57_L       (REG_TC_VE_SRC_BASE + 0xAE)
#define  REG_TC_VE_SRC_57_H       (REG_TC_VE_SRC_BASE + 0xAF)
#define  REG_TC_VE_SRC_58_L       (REG_TC_VE_SRC_BASE + 0xB0)
#define  REG_TC_VE_SRC_58_H       (REG_TC_VE_SRC_BASE + 0xB1)
#define  REG_TC_VE_SRC_59_L       (REG_TC_VE_SRC_BASE + 0xB2)
#define  REG_TC_VE_SRC_59_H       (REG_TC_VE_SRC_BASE + 0xB3)
#define  REG_TC_VE_SRC_5A_L       (REG_TC_VE_SRC_BASE + 0xB4)
#define  REG_TC_VE_SRC_5A_H       (REG_TC_VE_SRC_BASE + 0xB5)
#define  REG_TC_VE_SRC_5B_L       (REG_TC_VE_SRC_BASE + 0xB6)
#define  REG_TC_VE_SRC_5B_H       (REG_TC_VE_SRC_BASE + 0xB7)
#define  REG_TC_VE_SRC_5C_L       (REG_TC_VE_SRC_BASE + 0xB8)
#define  REG_TC_VE_SRC_5C_H       (REG_TC_VE_SRC_BASE + 0xB9)
#define  REG_TC_VE_SRC_5D_L       (REG_TC_VE_SRC_BASE + 0xBA)
#define  REG_TC_VE_SRC_5D_H       (REG_TC_VE_SRC_BASE + 0xBB)
#define  REG_TC_VE_SRC_5E_L       (REG_TC_VE_SRC_BASE + 0xBC)
#define  REG_TC_VE_SRC_5E_H       (REG_TC_VE_SRC_BASE + 0xBD)
#define  REG_TC_VE_SRC_5F_L       (REG_TC_VE_SRC_BASE + 0xBE)
#define  REG_TC_VE_SRC_5F_H       (REG_TC_VE_SRC_BASE + 0xBF)
#define  REG_TC_VE_SRC_60_L       (REG_TC_VE_SRC_BASE + 0xC0)
#define  REG_TC_VE_SRC_60_H       (REG_TC_VE_SRC_BASE + 0xC1)
#define  REG_TC_VE_SRC_61_L       (REG_TC_VE_SRC_BASE + 0xC2)
#define  REG_TC_VE_SRC_61_H       (REG_TC_VE_SRC_BASE + 0xC3)
#define  REG_TC_VE_SRC_62_L       (REG_TC_VE_SRC_BASE + 0xC4)
#define  REG_TC_VE_SRC_62_H       (REG_TC_VE_SRC_BASE + 0xC5)
#define  REG_TC_VE_SRC_63_L       (REG_TC_VE_SRC_BASE + 0xC6)
#define  REG_TC_VE_SRC_63_H       (REG_TC_VE_SRC_BASE + 0xC7)
#define  REG_TC_VE_SRC_64_L       (REG_TC_VE_SRC_BASE + 0xC8)
#define  REG_TC_VE_SRC_64_H       (REG_TC_VE_SRC_BASE + 0xC9)
#define  REG_TC_VE_SRC_65_L       (REG_TC_VE_SRC_BASE + 0xCA)
#define  REG_TC_VE_SRC_65_H       (REG_TC_VE_SRC_BASE + 0xCB)
#define  REG_TC_VE_SRC_66_L       (REG_TC_VE_SRC_BASE + 0xCC)
#define  REG_TC_VE_SRC_66_H       (REG_TC_VE_SRC_BASE + 0xCD)
#define  REG_TC_VE_SRC_67_L       (REG_TC_VE_SRC_BASE + 0xCE)
#define  REG_TC_VE_SRC_67_H       (REG_TC_VE_SRC_BASE + 0xCF)
#define  REG_TC_VE_SRC_68_L       (REG_TC_VE_SRC_BASE + 0xD0)
#define  REG_TC_VE_SRC_68_H       (REG_TC_VE_SRC_BASE + 0xD1)
#define  REG_TC_VE_SRC_69_L       (REG_TC_VE_SRC_BASE + 0xD2)
#define  REG_TC_VE_SRC_69_H       (REG_TC_VE_SRC_BASE + 0xD3)
#define  REG_TC_VE_SRC_6A_L       (REG_TC_VE_SRC_BASE + 0xD4)
#define  REG_TC_VE_SRC_6A_H       (REG_TC_VE_SRC_BASE + 0xD5)
#define  REG_TC_VE_SRC_6B_L       (REG_TC_VE_SRC_BASE + 0xD6)
#define  REG_TC_VE_SRC_6B_H       (REG_TC_VE_SRC_BASE + 0xD7)
#define  REG_TC_VE_SRC_6C_L       (REG_TC_VE_SRC_BASE + 0xD8)
#define  REG_TC_VE_SRC_6C_H       (REG_TC_VE_SRC_BASE + 0xD9)
#define  REG_TC_VE_SRC_6D_L       (REG_TC_VE_SRC_BASE + 0xDA)
#define  REG_TC_VE_SRC_6D_H       (REG_TC_VE_SRC_BASE + 0xDB)
#define  REG_TC_VE_SRC_6E_L       (REG_TC_VE_SRC_BASE + 0xDC)
#define  REG_TC_VE_SRC_6E_H       (REG_TC_VE_SRC_BASE + 0xDD)
#define  REG_TC_VE_SRC_6F_L       (REG_TC_VE_SRC_BASE + 0xDE)
#define  REG_TC_VE_SRC_6F_H       (REG_TC_VE_SRC_BASE + 0xDF)
#define  REG_TC_VE_SRC_70_L       (REG_TC_VE_SRC_BASE + 0xE0)
#define  REG_TC_VE_SRC_70_H       (REG_TC_VE_SRC_BASE + 0xE1)
#define  REG_TC_VE_SRC_71_L       (REG_TC_VE_SRC_BASE + 0xE2)
#define  REG_TC_VE_SRC_71_H       (REG_TC_VE_SRC_BASE + 0xE3)
#define  REG_TC_VE_SRC_72_L       (REG_TC_VE_SRC_BASE + 0xE4)
#define  REG_TC_VE_SRC_72_H       (REG_TC_VE_SRC_BASE + 0xE5)
#define  REG_TC_VE_SRC_73_L       (REG_TC_VE_SRC_BASE + 0xE6)
#define  REG_TC_VE_SRC_73_H       (REG_TC_VE_SRC_BASE + 0xE7)
#define  REG_TC_VE_SRC_74_L       (REG_TC_VE_SRC_BASE + 0xE8)
#define  REG_TC_VE_SRC_74_H       (REG_TC_VE_SRC_BASE + 0xE9)
#define  REG_TC_VE_SRC_75_L       (REG_TC_VE_SRC_BASE + 0xEA)
#define  REG_TC_VE_SRC_75_H       (REG_TC_VE_SRC_BASE + 0xEB)
#define  REG_TC_VE_SRC_76_L       (REG_TC_VE_SRC_BASE + 0xEC)
#define  REG_TC_VE_SRC_76_H       (REG_TC_VE_SRC_BASE + 0xED)
#define  REG_TC_VE_SRC_77_L       (REG_TC_VE_SRC_BASE + 0xEE)
#define  REG_TC_VE_SRC_77_H       (REG_TC_VE_SRC_BASE + 0xEF)
#define  REG_TC_VE_SRC_78_L       (REG_TC_VE_SRC_BASE + 0xF0)
#define  REG_TC_VE_SRC_78_H       (REG_TC_VE_SRC_BASE + 0xF1)
#define  REG_TC_VE_SRC_79_L       (REG_TC_VE_SRC_BASE + 0xF2)
#define  REG_TC_VE_SRC_79_H       (REG_TC_VE_SRC_BASE + 0xF3)
#define  REG_TC_VE_SRC_7A_L       (REG_TC_VE_SRC_BASE + 0xF4)
#define  REG_TC_VE_SRC_7A_H       (REG_TC_VE_SRC_BASE + 0xF5)
#define  REG_TC_VE_SRC_7B_L       (REG_TC_VE_SRC_BASE + 0xF6)
#define  REG_TC_VE_SRC_7B_H       (REG_TC_VE_SRC_BASE + 0xF7)
#define  REG_TC_VE_SRC_7C_L       (REG_TC_VE_SRC_BASE + 0xF8)
#define  REG_TC_VE_SRC_7C_H       (REG_TC_VE_SRC_BASE + 0xF9)
#define  REG_TC_VE_SRC_7D_L       (REG_TC_VE_SRC_BASE + 0xFA)
#define  REG_TC_VE_SRC_7D_H       (REG_TC_VE_SRC_BASE + 0xFB)
#define  REG_TC_VE_SRC_7E_L       (REG_TC_VE_SRC_BASE + 0xFC)
#define  REG_TC_VE_SRC_7E_H       (REG_TC_VE_SRC_BASE + 0xFD)
#define  REG_TC_VE_SRC_7F_L       (REG_TC_VE_SRC_BASE + 0xFE)
#define  REG_TC_VE_SRC_7F_H       (REG_TC_VE_SRC_BASE + 0xFF)

#define  REG_TC_VE_ENC1_00_L       (REG_TC_VE_ENC1_BASE + 0x00)
#define  REG_TC_VE_ENC1_00_H       (REG_TC_VE_ENC1_BASE + 0x01)
#define  REG_TC_VE_ENC1_01_L       (REG_TC_VE_ENC1_BASE + 0x02)
#define  REG_TC_VE_ENC1_01_H       (REG_TC_VE_ENC1_BASE + 0x03)
#define  REG_TC_VE_ENC1_02_L       (REG_TC_VE_ENC1_BASE + 0x04)
#define  REG_TC_VE_ENC1_02_H       (REG_TC_VE_ENC1_BASE + 0x05)
#define  REG_TC_VE_ENC1_03_L       (REG_TC_VE_ENC1_BASE + 0x06)
#define  REG_TC_VE_ENC1_03_H       (REG_TC_VE_ENC1_BASE + 0x07)
#define  REG_TC_VE_ENC1_04_L       (REG_TC_VE_ENC1_BASE + 0x08)
#define  REG_TC_VE_ENC1_04_H       (REG_TC_VE_ENC1_BASE + 0x09)
#define  REG_TC_VE_ENC1_05_L       (REG_TC_VE_ENC1_BASE + 0x0A)
#define  REG_TC_VE_ENC1_05_H       (REG_TC_VE_ENC1_BASE + 0x0B)
#define  REG_TC_VE_ENC1_06_L       (REG_TC_VE_ENC1_BASE + 0x0C)
#define  REG_TC_VE_ENC1_06_H       (REG_TC_VE_ENC1_BASE + 0x0D)
#define  REG_TC_VE_ENC1_07_L       (REG_TC_VE_ENC1_BASE + 0x0E)
#define  REG_TC_VE_ENC1_07_H       (REG_TC_VE_ENC1_BASE + 0x0F)
#define  REG_TC_VE_ENC1_08_L       (REG_TC_VE_ENC1_BASE + 0x10)
#define  REG_TC_VE_ENC1_08_H       (REG_TC_VE_ENC1_BASE + 0x11)
#define  REG_TC_VE_ENC1_09_L       (REG_TC_VE_ENC1_BASE + 0x12)
#define  REG_TC_VE_ENC1_09_H       (REG_TC_VE_ENC1_BASE + 0x13)
#define  REG_TC_VE_ENC1_0A_L       (REG_TC_VE_ENC1_BASE + 0x14)
#define  REG_TC_VE_ENC1_0A_H       (REG_TC_VE_ENC1_BASE + 0x15)
#define  REG_TC_VE_ENC1_0B_L       (REG_TC_VE_ENC1_BASE + 0x16)
#define  REG_TC_VE_ENC1_0B_H       (REG_TC_VE_ENC1_BASE + 0x17)
#define  REG_TC_VE_ENC1_0C_L       (REG_TC_VE_ENC1_BASE + 0x18)
#define  REG_TC_VE_ENC1_0C_H       (REG_TC_VE_ENC1_BASE + 0x19)
#define  REG_TC_VE_ENC1_0D_L       (REG_TC_VE_ENC1_BASE + 0x1A)
#define  REG_TC_VE_ENC1_0D_H       (REG_TC_VE_ENC1_BASE + 0x1B)
#define  REG_TC_VE_ENC1_0E_L       (REG_TC_VE_ENC1_BASE + 0x1C)
#define  REG_TC_VE_ENC1_0E_H       (REG_TC_VE_ENC1_BASE + 0x1D)
#define  REG_TC_VE_ENC1_0F_L       (REG_TC_VE_ENC1_BASE + 0x1E)
#define  REG_TC_VE_ENC1_0F_H       (REG_TC_VE_ENC1_BASE + 0x1F)
#define  REG_TC_VE_ENC1_10_L       (REG_TC_VE_ENC1_BASE + 0x20)
#define  REG_TC_VE_ENC1_10_H       (REG_TC_VE_ENC1_BASE + 0x21)
#define  REG_TC_VE_ENC1_11_L       (REG_TC_VE_ENC1_BASE + 0x22)
#define  REG_TC_VE_ENC1_11_H       (REG_TC_VE_ENC1_BASE + 0x23)
#define  REG_TC_VE_ENC1_12_L       (REG_TC_VE_ENC1_BASE + 0x24)
#define  REG_TC_VE_ENC1_12_H       (REG_TC_VE_ENC1_BASE + 0x25)
#define  REG_TC_VE_ENC1_13_L       (REG_TC_VE_ENC1_BASE + 0x26)
#define  REG_TC_VE_ENC1_13_H       (REG_TC_VE_ENC1_BASE + 0x27)
#define  REG_TC_VE_ENC1_14_L       (REG_TC_VE_ENC1_BASE + 0x28)
#define  REG_TC_VE_ENC1_14_H       (REG_TC_VE_ENC1_BASE + 0x29)
#define  REG_TC_VE_ENC1_15_L       (REG_TC_VE_ENC1_BASE + 0x2A)
#define  REG_TC_VE_ENC1_15_H       (REG_TC_VE_ENC1_BASE + 0x2B)
#define  REG_TC_VE_ENC1_16_L       (REG_TC_VE_ENC1_BASE + 0x2C)
#define  REG_TC_VE_ENC1_16_H       (REG_TC_VE_ENC1_BASE + 0x2D)
#define  REG_TC_VE_ENC1_17_L       (REG_TC_VE_ENC1_BASE + 0x2E)
#define  REG_TC_VE_ENC1_17_H       (REG_TC_VE_ENC1_BASE + 0x2F)
#define  REG_TC_VE_ENC1_18_L       (REG_TC_VE_ENC1_BASE + 0x30)
#define  REG_TC_VE_ENC1_18_H       (REG_TC_VE_ENC1_BASE + 0x31)
#define  REG_TC_VE_ENC1_19_L       (REG_TC_VE_ENC1_BASE + 0x32)
#define  REG_TC_VE_ENC1_19_H       (REG_TC_VE_ENC1_BASE + 0x33)
#define  REG_TC_VE_ENC1_1A_L       (REG_TC_VE_ENC1_BASE + 0x34)
#define  REG_TC_VE_ENC1_1A_H       (REG_TC_VE_ENC1_BASE + 0x35)
#define  REG_TC_VE_ENC1_1B_L       (REG_TC_VE_ENC1_BASE + 0x36)
#define  REG_TC_VE_ENC1_1B_H       (REG_TC_VE_ENC1_BASE + 0x37)
#define  REG_TC_VE_ENC1_1C_L       (REG_TC_VE_ENC1_BASE + 0x38)
#define  REG_TC_VE_ENC1_1C_H       (REG_TC_VE_ENC1_BASE + 0x39)
#define  REG_TC_VE_ENC1_1D_L       (REG_TC_VE_ENC1_BASE + 0x3A)
#define  REG_TC_VE_ENC1_1D_H       (REG_TC_VE_ENC1_BASE + 0x3B)
#define  REG_TC_VE_ENC1_1E_L       (REG_TC_VE_ENC1_BASE + 0x3C)
#define  REG_TC_VE_ENC1_1E_H       (REG_TC_VE_ENC1_BASE + 0x3D)
#define  REG_TC_VE_ENC1_1F_L       (REG_TC_VE_ENC1_BASE + 0x3E)
#define  REG_TC_VE_ENC1_1F_H       (REG_TC_VE_ENC1_BASE + 0x3F)
#define  REG_TC_VE_ENC1_20_L       (REG_TC_VE_ENC1_BASE + 0x40)
#define  REG_TC_VE_ENC1_20_H       (REG_TC_VE_ENC1_BASE + 0x41)
#define  REG_TC_VE_ENC1_21_L       (REG_TC_VE_ENC1_BASE + 0x42)
#define  REG_TC_VE_ENC1_21_H       (REG_TC_VE_ENC1_BASE + 0x43)
#define  REG_TC_VE_ENC1_22_L       (REG_TC_VE_ENC1_BASE + 0x44)
#define  REG_TC_VE_ENC1_22_H       (REG_TC_VE_ENC1_BASE + 0x45)
#define  REG_TC_VE_ENC1_23_L       (REG_TC_VE_ENC1_BASE + 0x46)
#define  REG_TC_VE_ENC1_23_H       (REG_TC_VE_ENC1_BASE + 0x47)
#define  REG_TC_VE_ENC1_24_L       (REG_TC_VE_ENC1_BASE + 0x48)
#define  REG_TC_VE_ENC1_24_H       (REG_TC_VE_ENC1_BASE + 0x49)
#define  REG_TC_VE_ENC1_25_L       (REG_TC_VE_ENC1_BASE + 0x4A)
#define  REG_TC_VE_ENC1_25_H       (REG_TC_VE_ENC1_BASE + 0x4B)
#define  REG_TC_VE_ENC1_26_L       (REG_TC_VE_ENC1_BASE + 0x4C)
#define  REG_TC_VE_ENC1_26_H       (REG_TC_VE_ENC1_BASE + 0x4D)
#define  REG_TC_VE_ENC1_27_L       (REG_TC_VE_ENC1_BASE + 0x4E)
#define  REG_TC_VE_ENC1_27_H       (REG_TC_VE_ENC1_BASE + 0x4F)
#define  REG_TC_VE_ENC1_28_L       (REG_TC_VE_ENC1_BASE + 0x50)
#define  REG_TC_VE_ENC1_28_H       (REG_TC_VE_ENC1_BASE + 0x51)
#define  REG_TC_VE_ENC1_29_L       (REG_TC_VE_ENC1_BASE + 0x52)
#define  REG_TC_VE_ENC1_29_H       (REG_TC_VE_ENC1_BASE + 0x53)
#define  REG_TC_VE_ENC1_2A_L       (REG_TC_VE_ENC1_BASE + 0x54)
#define  REG_TC_VE_ENC1_2A_H       (REG_TC_VE_ENC1_BASE + 0x55)
#define  REG_TC_VE_ENC1_2B_L       (REG_TC_VE_ENC1_BASE + 0x56)
#define  REG_TC_VE_ENC1_2B_H       (REG_TC_VE_ENC1_BASE + 0x57)
#define  REG_TC_VE_ENC1_2C_L       (REG_TC_VE_ENC1_BASE + 0x58)
#define  REG_TC_VE_ENC1_2C_H       (REG_TC_VE_ENC1_BASE + 0x59)
#define  REG_TC_VE_ENC1_2D_L       (REG_TC_VE_ENC1_BASE + 0x5A)
#define  REG_TC_VE_ENC1_2D_H       (REG_TC_VE_ENC1_BASE + 0x5B)
#define  REG_TC_VE_ENC1_2E_L       (REG_TC_VE_ENC1_BASE + 0x5C)
#define  REG_TC_VE_ENC1_2E_H       (REG_TC_VE_ENC1_BASE + 0x5D)
#define  REG_TC_VE_ENC1_2F_L       (REG_TC_VE_ENC1_BASE + 0x5E)
#define  REG_TC_VE_ENC1_2F_H       (REG_TC_VE_ENC1_BASE + 0x5F)
#define  REG_TC_VE_ENC1_30_L       (REG_TC_VE_ENC1_BASE + 0x60)
#define  REG_TC_VE_ENC1_30_H       (REG_TC_VE_ENC1_BASE + 0x61)
#define  REG_TC_VE_ENC1_31_L       (REG_TC_VE_ENC1_BASE + 0x62)
#define  REG_TC_VE_ENC1_31_H       (REG_TC_VE_ENC1_BASE + 0x63)
#define  REG_TC_VE_ENC1_32_L       (REG_TC_VE_ENC1_BASE + 0x64)
#define  REG_TC_VE_ENC1_32_H       (REG_TC_VE_ENC1_BASE + 0x65)
#define  REG_TC_VE_ENC1_33_L       (REG_TC_VE_ENC1_BASE + 0x66)
#define  REG_TC_VE_ENC1_33_H       (REG_TC_VE_ENC1_BASE + 0x67)
#define  REG_TC_VE_ENC1_34_L       (REG_TC_VE_ENC1_BASE + 0x68)
#define  REG_TC_VE_ENC1_34_H       (REG_TC_VE_ENC1_BASE + 0x69)
#define  REG_TC_VE_ENC1_35_L       (REG_TC_VE_ENC1_BASE + 0x6A)
#define  REG_TC_VE_ENC1_35_H       (REG_TC_VE_ENC1_BASE + 0x6B)
#define  REG_TC_VE_ENC1_36_L       (REG_TC_VE_ENC1_BASE + 0x6C)
#define  REG_TC_VE_ENC1_36_H       (REG_TC_VE_ENC1_BASE + 0x6D)
#define  REG_TC_VE_ENC1_37_L       (REG_TC_VE_ENC1_BASE + 0x6E)
#define  REG_TC_VE_ENC1_37_H       (REG_TC_VE_ENC1_BASE + 0x6F)
#define  REG_TC_VE_ENC1_38_L       (REG_TC_VE_ENC1_BASE + 0x70)
#define  REG_TC_VE_ENC1_38_H       (REG_TC_VE_ENC1_BASE + 0x71)
#define  REG_TC_VE_ENC1_39_L       (REG_TC_VE_ENC1_BASE + 0x72)
#define  REG_TC_VE_ENC1_39_H       (REG_TC_VE_ENC1_BASE + 0x73)
#define  REG_TC_VE_ENC1_3A_L       (REG_TC_VE_ENC1_BASE + 0x74)
#define  REG_TC_VE_ENC1_3A_H       (REG_TC_VE_ENC1_BASE + 0x75)
#define  REG_TC_VE_ENC1_3B_L       (REG_TC_VE_ENC1_BASE + 0x76)
#define  REG_TC_VE_ENC1_3B_H       (REG_TC_VE_ENC1_BASE + 0x77)
#define  REG_TC_VE_ENC1_3C_L       (REG_TC_VE_ENC1_BASE + 0x78)
#define  REG_TC_VE_ENC1_3C_H       (REG_TC_VE_ENC1_BASE + 0x79)
#define  REG_TC_VE_ENC1_3D_L       (REG_TC_VE_ENC1_BASE + 0x7A)
#define  REG_TC_VE_ENC1_3D_H       (REG_TC_VE_ENC1_BASE + 0x7B)
#define  REG_TC_VE_ENC1_3E_L       (REG_TC_VE_ENC1_BASE + 0x7C)
#define  REG_TC_VE_ENC1_3E_H       (REG_TC_VE_ENC1_BASE + 0x7D)
#define  REG_TC_VE_ENC1_3F_L       (REG_TC_VE_ENC1_BASE + 0x7E)
#define  REG_TC_VE_ENC1_3F_H       (REG_TC_VE_ENC1_BASE + 0x7F)
#define  REG_TC_VE_ENC1_40_L       (REG_TC_VE_ENC1_BASE + 0x80)
#define  REG_TC_VE_ENC1_40_H       (REG_TC_VE_ENC1_BASE + 0x81)
#define  REG_TC_VE_ENC1_41_L       (REG_TC_VE_ENC1_BASE + 0x82)
#define  REG_TC_VE_ENC1_41_H       (REG_TC_VE_ENC1_BASE + 0x83)
#define  REG_TC_VE_ENC1_42_L       (REG_TC_VE_ENC1_BASE + 0x84)
#define  REG_TC_VE_ENC1_42_H       (REG_TC_VE_ENC1_BASE + 0x85)
#define  REG_TC_VE_ENC1_43_L       (REG_TC_VE_ENC1_BASE + 0x86)
#define  REG_TC_VE_ENC1_43_H       (REG_TC_VE_ENC1_BASE + 0x87)
#define  REG_TC_VE_ENC1_44_L       (REG_TC_VE_ENC1_BASE + 0x88)
#define  REG_TC_VE_ENC1_44_H       (REG_TC_VE_ENC1_BASE + 0x89)
#define  REG_TC_VE_ENC1_45_L       (REG_TC_VE_ENC1_BASE + 0x8A)
#define  REG_TC_VE_ENC1_45_H       (REG_TC_VE_ENC1_BASE + 0x8B)
#define  REG_TC_VE_ENC1_46_L       (REG_TC_VE_ENC1_BASE + 0x8C)
#define  REG_TC_VE_ENC1_46_H       (REG_TC_VE_ENC1_BASE + 0x8D)
#define  REG_TC_VE_ENC1_47_L       (REG_TC_VE_ENC1_BASE + 0x8E)
#define  REG_TC_VE_ENC1_47_H       (REG_TC_VE_ENC1_BASE + 0x8F)
#define  REG_TC_VE_ENC1_48_L       (REG_TC_VE_ENC1_BASE + 0x90)
#define  REG_TC_VE_ENC1_48_H       (REG_TC_VE_ENC1_BASE + 0x91)
#define  REG_TC_VE_ENC1_49_L       (REG_TC_VE_ENC1_BASE + 0x92)
#define  REG_TC_VE_ENC1_49_H       (REG_TC_VE_ENC1_BASE + 0x93)
#define  REG_TC_VE_ENC1_4A_L       (REG_TC_VE_ENC1_BASE + 0x94)
#define  REG_TC_VE_ENC1_4A_H       (REG_TC_VE_ENC1_BASE + 0x95)
#define  REG_TC_VE_ENC1_4B_L       (REG_TC_VE_ENC1_BASE + 0x96)
#define  REG_TC_VE_ENC1_4B_H       (REG_TC_VE_ENC1_BASE + 0x97)
#define  REG_TC_VE_ENC1_4C_L       (REG_TC_VE_ENC1_BASE + 0x98)
#define  REG_TC_VE_ENC1_4C_H       (REG_TC_VE_ENC1_BASE + 0x99)
#define  REG_TC_VE_ENC1_4D_L       (REG_TC_VE_ENC1_BASE + 0x9A)
#define  REG_TC_VE_ENC1_4D_H       (REG_TC_VE_ENC1_BASE + 0x9B)
#define  REG_TC_VE_ENC1_4E_L       (REG_TC_VE_ENC1_BASE + 0x9C)
#define  REG_TC_VE_ENC1_4E_H       (REG_TC_VE_ENC1_BASE + 0x9D)
#define  REG_TC_VE_ENC1_4F_L       (REG_TC_VE_ENC1_BASE + 0x9E)
#define  REG_TC_VE_ENC1_4F_H       (REG_TC_VE_ENC1_BASE + 0x9F)
#define  REG_TC_VE_ENC1_50_L       (REG_TC_VE_ENC1_BASE + 0xA0)
#define  REG_TC_VE_ENC1_50_H       (REG_TC_VE_ENC1_BASE + 0xA1)
#define  REG_TC_VE_ENC1_51_L       (REG_TC_VE_ENC1_BASE + 0xA2)
#define  REG_TC_VE_ENC1_51_H       (REG_TC_VE_ENC1_BASE + 0xA3)
#define  REG_TC_VE_ENC1_52_L       (REG_TC_VE_ENC1_BASE + 0xA4)
#define  REG_TC_VE_ENC1_52_H       (REG_TC_VE_ENC1_BASE + 0xA5)
#define  REG_TC_VE_ENC1_53_L       (REG_TC_VE_ENC1_BASE + 0xA6)
#define  REG_TC_VE_ENC1_53_H       (REG_TC_VE_ENC1_BASE + 0xA7)
#define  REG_TC_VE_ENC1_54_L       (REG_TC_VE_ENC1_BASE + 0xA8)
#define  REG_TC_VE_ENC1_54_H       (REG_TC_VE_ENC1_BASE + 0xA9)
#define  REG_TC_VE_ENC1_55_L       (REG_TC_VE_ENC1_BASE + 0xAA)
#define  REG_TC_VE_ENC1_55_H       (REG_TC_VE_ENC1_BASE + 0xAB)
#define  REG_TC_VE_ENC1_56_L       (REG_TC_VE_ENC1_BASE + 0xAC)
#define  REG_TC_VE_ENC1_56_H       (REG_TC_VE_ENC1_BASE + 0xAD)
#define  REG_TC_VE_ENC1_57_L       (REG_TC_VE_ENC1_BASE + 0xAE)
#define  REG_TC_VE_ENC1_57_H       (REG_TC_VE_ENC1_BASE + 0xAF)
#define  REG_TC_VE_ENC1_58_L       (REG_TC_VE_ENC1_BASE + 0xB0)
#define  REG_TC_VE_ENC1_58_H       (REG_TC_VE_ENC1_BASE + 0xB1)
#define  REG_TC_VE_ENC1_59_L       (REG_TC_VE_ENC1_BASE + 0xB2)
#define  REG_TC_VE_ENC1_59_H       (REG_TC_VE_ENC1_BASE + 0xB3)
#define  REG_TC_VE_ENC1_5A_L       (REG_TC_VE_ENC1_BASE + 0xB4)
#define  REG_TC_VE_ENC1_5A_H       (REG_TC_VE_ENC1_BASE + 0xB5)
#define  REG_TC_VE_ENC1_5B_L       (REG_TC_VE_ENC1_BASE + 0xB6)
#define  REG_TC_VE_ENC1_5B_H       (REG_TC_VE_ENC1_BASE + 0xB7)
#define  REG_TC_VE_ENC1_5C_L       (REG_TC_VE_ENC1_BASE + 0xB8)
#define  REG_TC_VE_ENC1_5C_H       (REG_TC_VE_ENC1_BASE + 0xB9)
#define  REG_TC_VE_ENC1_5D_L       (REG_TC_VE_ENC1_BASE + 0xBA)
#define  REG_TC_VE_ENC1_5D_H       (REG_TC_VE_ENC1_BASE + 0xBB)
#define  REG_TC_VE_ENC1_5E_L       (REG_TC_VE_ENC1_BASE + 0xBC)
#define  REG_TC_VE_ENC1_5E_H       (REG_TC_VE_ENC1_BASE + 0xBD)
#define  REG_TC_VE_ENC1_5F_L       (REG_TC_VE_ENC1_BASE + 0xBE)
#define  REG_TC_VE_ENC1_5F_H       (REG_TC_VE_ENC1_BASE + 0xBF)
#define  REG_TC_VE_ENC1_60_L       (REG_TC_VE_ENC1_BASE + 0xC0)
#define  REG_TC_VE_ENC1_60_H       (REG_TC_VE_ENC1_BASE + 0xC1)
#define  REG_TC_VE_ENC1_61_L       (REG_TC_VE_ENC1_BASE + 0xC2)
#define  REG_TC_VE_ENC1_61_H       (REG_TC_VE_ENC1_BASE + 0xC3)
#define  REG_TC_VE_ENC1_62_L       (REG_TC_VE_ENC1_BASE + 0xC4)
#define  REG_TC_VE_ENC1_62_H       (REG_TC_VE_ENC1_BASE + 0xC5)
#define  REG_TC_VE_ENC1_63_L       (REG_TC_VE_ENC1_BASE + 0xC6)
#define  REG_TC_VE_ENC1_63_H       (REG_TC_VE_ENC1_BASE + 0xC7)
#define  REG_TC_VE_ENC1_64_L       (REG_TC_VE_ENC1_BASE + 0xC8)
#define  REG_TC_VE_ENC1_64_H       (REG_TC_VE_ENC1_BASE + 0xC9)
#define  REG_TC_VE_ENC1_65_L       (REG_TC_VE_ENC1_BASE + 0xCA)
#define  REG_TC_VE_ENC1_65_H       (REG_TC_VE_ENC1_BASE + 0xCB)
#define  REG_TC_VE_ENC1_66_L       (REG_TC_VE_ENC1_BASE + 0xCC)
#define  REG_TC_VE_ENC1_66_H       (REG_TC_VE_ENC1_BASE + 0xCD)
#define  REG_TC_VE_ENC1_67_L       (REG_TC_VE_ENC1_BASE + 0xCE)
#define  REG_TC_VE_ENC1_67_H       (REG_TC_VE_ENC1_BASE + 0xCF)
#define  REG_TC_VE_ENC1_68_L       (REG_TC_VE_ENC1_BASE + 0xD0)
#define  REG_TC_VE_ENC1_68_H       (REG_TC_VE_ENC1_BASE + 0xD1)
#define  REG_TC_VE_ENC1_69_L       (REG_TC_VE_ENC1_BASE + 0xD2)
#define  REG_TC_VE_ENC1_69_H       (REG_TC_VE_ENC1_BASE + 0xD3)
#define  REG_TC_VE_ENC1_6A_L       (REG_TC_VE_ENC1_BASE + 0xD4)
#define  REG_TC_VE_ENC1_6A_H       (REG_TC_VE_ENC1_BASE + 0xD5)
#define  REG_TC_VE_ENC1_6B_L       (REG_TC_VE_ENC1_BASE + 0xD6)
#define  REG_TC_VE_ENC1_6B_H       (REG_TC_VE_ENC1_BASE + 0xD7)
#define  REG_TC_VE_ENC1_6C_L       (REG_TC_VE_ENC1_BASE + 0xD8)
#define  REG_TC_VE_ENC1_6C_H       (REG_TC_VE_ENC1_BASE + 0xD9)
#define  REG_TC_VE_ENC1_6D_L       (REG_TC_VE_ENC1_BASE + 0xDA)
#define  REG_TC_VE_ENC1_6D_H       (REG_TC_VE_ENC1_BASE + 0xDB)
#define  REG_TC_VE_ENC1_6E_L       (REG_TC_VE_ENC1_BASE + 0xDC)
#define  REG_TC_VE_ENC1_6E_H       (REG_TC_VE_ENC1_BASE + 0xDD)
#define  REG_TC_VE_ENC1_6F_L       (REG_TC_VE_ENC1_BASE + 0xDE)
#define  REG_TC_VE_ENC1_6F_H       (REG_TC_VE_ENC1_BASE + 0xDF)
#define  REG_TC_VE_ENC1_70_L       (REG_TC_VE_ENC1_BASE + 0xE0)
#define  REG_TC_VE_ENC1_70_H       (REG_TC_VE_ENC1_BASE + 0xE1)
#define  REG_TC_VE_ENC1_71_L       (REG_TC_VE_ENC1_BASE + 0xE2)
#define  REG_TC_VE_ENC1_71_H       (REG_TC_VE_ENC1_BASE + 0xE3)
#define  REG_TC_VE_ENC1_72_L       (REG_TC_VE_ENC1_BASE + 0xE4)
#define  REG_TC_VE_ENC1_72_H       (REG_TC_VE_ENC1_BASE + 0xE5)
#define  REG_TC_VE_ENC1_73_L       (REG_TC_VE_ENC1_BASE + 0xE6)
#define  REG_TC_VE_ENC1_73_H       (REG_TC_VE_ENC1_BASE + 0xE7)
#define  REG_TC_VE_ENC1_74_L       (REG_TC_VE_ENC1_BASE + 0xE8)
#define  REG_TC_VE_ENC1_74_H       (REG_TC_VE_ENC1_BASE + 0xE9)
#define  REG_TC_VE_ENC1_75_L       (REG_TC_VE_ENC1_BASE + 0xEA)
#define  REG_TC_VE_ENC1_75_H       (REG_TC_VE_ENC1_BASE + 0xEB)
#define  REG_TC_VE_ENC1_76_L       (REG_TC_VE_ENC1_BASE + 0xEC)
#define  REG_TC_VE_ENC1_76_H       (REG_TC_VE_ENC1_BASE + 0xED)
#define  REG_TC_VE_ENC1_77_L       (REG_TC_VE_ENC1_BASE + 0xEE)
#define  REG_TC_VE_ENC1_77_H       (REG_TC_VE_ENC1_BASE + 0xEF)
#define  REG_TC_VE_ENC1_78_L       (REG_TC_VE_ENC1_BASE + 0xF0)
#define  REG_TC_VE_ENC1_78_H       (REG_TC_VE_ENC1_BASE + 0xF1)
#define  REG_TC_VE_ENC1_79_L       (REG_TC_VE_ENC1_BASE + 0xF2)
#define  REG_TC_VE_ENC1_79_H       (REG_TC_VE_ENC1_BASE + 0xF3)
#define  REG_TC_VE_ENC1_7A_L       (REG_TC_VE_ENC1_BASE + 0xF4)
#define  REG_TC_VE_ENC1_7A_H       (REG_TC_VE_ENC1_BASE + 0xF5)
#define  REG_TC_VE_ENC1_7B_L       (REG_TC_VE_ENC1_BASE + 0xF6)
#define  REG_TC_VE_ENC1_7B_H       (REG_TC_VE_ENC1_BASE + 0xF7)
#define  REG_TC_VE_ENC1_7C_L       (REG_TC_VE_ENC1_BASE + 0xF8)
#define  REG_TC_VE_ENC1_7C_H       (REG_TC_VE_ENC1_BASE + 0xF9)
#define  REG_TC_VE_ENC1_7D_L       (REG_TC_VE_ENC1_BASE + 0xFA)
#define  REG_TC_VE_ENC1_7D_H       (REG_TC_VE_ENC1_BASE + 0xFB)
#define  REG_TC_VE_ENC1_7E_L       (REG_TC_VE_ENC1_BASE + 0xFC)
#define  REG_TC_VE_ENC1_7E_H       (REG_TC_VE_ENC1_BASE + 0xFD)
#define  REG_TC_VE_ENC1_7F_L       (REG_TC_VE_ENC1_BASE + 0xFE)
#define  REG_TC_VE_ENC1_7F_H       (REG_TC_VE_ENC1_BASE + 0xFF)
#endif

#define  REG_TC_VE_ENC2_00_L       (REG_TC_VE_ENC2_BASE + 0x00)
#define  REG_TC_VE_ENC2_00_H       (REG_TC_VE_ENC2_BASE + 0x01)
#define  REG_TC_VE_ENC2_01_L       (REG_TC_VE_ENC2_BASE + 0x02)
#define  REG_TC_VE_ENC2_01_H       (REG_TC_VE_ENC2_BASE + 0x03)
#define  REG_TC_VE_ENC2_02_L       (REG_TC_VE_ENC2_BASE + 0x04)
#define  REG_TC_VE_ENC2_02_H       (REG_TC_VE_ENC2_BASE + 0x05)
#define  REG_TC_VE_ENC2_03_L       (REG_TC_VE_ENC2_BASE + 0x06)
#define  REG_TC_VE_ENC2_03_H       (REG_TC_VE_ENC2_BASE + 0x07)
#define  REG_TC_VE_ENC2_04_L       (REG_TC_VE_ENC2_BASE + 0x08)
#define  REG_TC_VE_ENC2_04_H       (REG_TC_VE_ENC2_BASE + 0x09)
#define  REG_TC_VE_ENC2_05_L       (REG_TC_VE_ENC2_BASE + 0x0A)
#define  REG_TC_VE_ENC2_05_H       (REG_TC_VE_ENC2_BASE + 0x0B)
#define  REG_TC_VE_ENC2_06_L       (REG_TC_VE_ENC2_BASE + 0x0C)
#define  REG_TC_VE_ENC2_06_H       (REG_TC_VE_ENC2_BASE + 0x0D)
#define  REG_TC_VE_ENC2_07_L       (REG_TC_VE_ENC2_BASE + 0x0E)
#define  REG_TC_VE_ENC2_07_H       (REG_TC_VE_ENC2_BASE + 0x0F)
#define  REG_TC_VE_ENC2_08_L       (REG_TC_VE_ENC2_BASE + 0x10)
#define  REG_TC_VE_ENC2_08_H       (REG_TC_VE_ENC2_BASE + 0x11)
#define  REG_TC_VE_ENC2_09_L       (REG_TC_VE_ENC2_BASE + 0x12)
#define  REG_TC_VE_ENC2_09_H       (REG_TC_VE_ENC2_BASE + 0x13)
#define  REG_TC_VE_ENC2_0A_L       (REG_TC_VE_ENC2_BASE + 0x14)
#define  REG_TC_VE_ENC2_0A_H       (REG_TC_VE_ENC2_BASE + 0x15)
#define  REG_TC_VE_ENC2_0B_L       (REG_TC_VE_ENC2_BASE + 0x16)
#define  REG_TC_VE_ENC2_0B_H       (REG_TC_VE_ENC2_BASE + 0x17)
#define  REG_TC_VE_ENC2_0C_L       (REG_TC_VE_ENC2_BASE + 0x18)
#define  REG_TC_VE_ENC2_0C_H       (REG_TC_VE_ENC2_BASE + 0x19)
#define  REG_TC_VE_ENC2_0D_L       (REG_TC_VE_ENC2_BASE + 0x1A)
#define  REG_TC_VE_ENC2_0D_H       (REG_TC_VE_ENC2_BASE + 0x1B)
#define  REG_TC_VE_ENC2_0E_L       (REG_TC_VE_ENC2_BASE + 0x1C)
#define  REG_TC_VE_ENC2_0E_H       (REG_TC_VE_ENC2_BASE + 0x1D)
#define  REG_TC_VE_ENC2_0F_L       (REG_TC_VE_ENC2_BASE + 0x1E)
#define  REG_TC_VE_ENC2_0F_H       (REG_TC_VE_ENC2_BASE + 0x1F)
#define  REG_TC_VE_ENC2_10_L       (REG_TC_VE_ENC2_BASE + 0x20)
#define  REG_TC_VE_ENC2_10_H       (REG_TC_VE_ENC2_BASE + 0x21)
#define  REG_TC_VE_ENC2_11_L       (REG_TC_VE_ENC2_BASE + 0x22)
#define  REG_TC_VE_ENC2_11_H       (REG_TC_VE_ENC2_BASE + 0x23)
#define  REG_TC_VE_ENC2_12_L       (REG_TC_VE_ENC2_BASE + 0x24)
#define  REG_TC_VE_ENC2_12_H       (REG_TC_VE_ENC2_BASE + 0x25)
#define  REG_TC_VE_ENC2_13_L       (REG_TC_VE_ENC2_BASE + 0x26)
#define  REG_TC_VE_ENC2_13_H       (REG_TC_VE_ENC2_BASE + 0x27)
#define  REG_TC_VE_ENC2_14_L       (REG_TC_VE_ENC2_BASE + 0x28)
#define  REG_TC_VE_ENC2_14_H       (REG_TC_VE_ENC2_BASE + 0x29)
#define  REG_TC_VE_ENC2_15_L       (REG_TC_VE_ENC2_BASE + 0x2A)
#define  REG_TC_VE_ENC2_15_H       (REG_TC_VE_ENC2_BASE + 0x2B)
#define  REG_TC_VE_ENC2_16_L       (REG_TC_VE_ENC2_BASE + 0x2C)
#define  REG_TC_VE_ENC2_16_H       (REG_TC_VE_ENC2_BASE + 0x2D)
#define  REG_TC_VE_ENC2_17_L       (REG_TC_VE_ENC2_BASE + 0x2E)
#define  REG_TC_VE_ENC2_17_H       (REG_TC_VE_ENC2_BASE + 0x2F)
#define  REG_TC_VE_ENC2_18_L       (REG_TC_VE_ENC2_BASE + 0x30)
#define  REG_TC_VE_ENC2_18_H       (REG_TC_VE_ENC2_BASE + 0x31)
#define  REG_TC_VE_ENC2_19_L       (REG_TC_VE_ENC2_BASE + 0x32)
#define  REG_TC_VE_ENC2_19_H       (REG_TC_VE_ENC2_BASE + 0x33)
#define  REG_TC_VE_ENC2_1A_L       (REG_TC_VE_ENC2_BASE + 0x34)
#define  REG_TC_VE_ENC2_1A_H       (REG_TC_VE_ENC2_BASE + 0x35)
#define  REG_TC_VE_ENC2_1B_L       (REG_TC_VE_ENC2_BASE + 0x36)
#define  REG_TC_VE_ENC2_1B_H       (REG_TC_VE_ENC2_BASE + 0x37)
#define  REG_TC_VE_ENC2_1C_L       (REG_TC_VE_ENC2_BASE + 0x38)
#define  REG_TC_VE_ENC2_1C_H       (REG_TC_VE_ENC2_BASE + 0x39)
#define  REG_TC_VE_ENC2_1D_L       (REG_TC_VE_ENC2_BASE + 0x3A)
#define  REG_TC_VE_ENC2_1D_H       (REG_TC_VE_ENC2_BASE + 0x3B)
#define  REG_TC_VE_ENC2_1E_L       (REG_TC_VE_ENC2_BASE + 0x3C)
#define  REG_TC_VE_ENC2_1E_H       (REG_TC_VE_ENC2_BASE + 0x3D)
#define  REG_TC_VE_ENC2_1F_L       (REG_TC_VE_ENC2_BASE + 0x3E)
#define  REG_TC_VE_ENC2_1F_H       (REG_TC_VE_ENC2_BASE + 0x3F)
#define  REG_TC_VE_ENC2_20_L       (REG_TC_VE_ENC2_BASE + 0x40)
#define  REG_TC_VE_ENC2_20_H       (REG_TC_VE_ENC2_BASE + 0x41)
#define  REG_TC_VE_ENC2_21_L       (REG_TC_VE_ENC2_BASE + 0x42)
#define  REG_TC_VE_ENC2_21_H       (REG_TC_VE_ENC2_BASE + 0x43)
#define  REG_TC_VE_ENC2_22_L       (REG_TC_VE_ENC2_BASE + 0x44)
#define  REG_TC_VE_ENC2_22_H       (REG_TC_VE_ENC2_BASE + 0x45)
#define  REG_TC_VE_ENC2_23_L       (REG_TC_VE_ENC2_BASE + 0x46)
#define  REG_TC_VE_ENC2_23_H       (REG_TC_VE_ENC2_BASE + 0x47)
#define  REG_TC_VE_ENC2_24_L       (REG_TC_VE_ENC2_BASE + 0x48)
#define  REG_TC_VE_ENC2_24_H       (REG_TC_VE_ENC2_BASE + 0x49)
#define  REG_TC_VE_ENC2_25_L       (REG_TC_VE_ENC2_BASE + 0x4A)
#define  REG_TC_VE_ENC2_25_H       (REG_TC_VE_ENC2_BASE + 0x4B)
#define  REG_TC_VE_ENC2_26_L       (REG_TC_VE_ENC2_BASE + 0x4C)
#define  REG_TC_VE_ENC2_26_H       (REG_TC_VE_ENC2_BASE + 0x4D)
#define  REG_TC_VE_ENC2_27_L       (REG_TC_VE_ENC2_BASE + 0x4E)
#define  REG_TC_VE_ENC2_27_H       (REG_TC_VE_ENC2_BASE + 0x4F)
#define  REG_TC_VE_ENC2_28_L       (REG_TC_VE_ENC2_BASE + 0x50)
#define  REG_TC_VE_ENC2_28_H       (REG_TC_VE_ENC2_BASE + 0x51)
#define  REG_TC_VE_ENC2_29_L       (REG_TC_VE_ENC2_BASE + 0x52)
#define  REG_TC_VE_ENC2_29_H       (REG_TC_VE_ENC2_BASE + 0x53)
#define  REG_TC_VE_ENC2_2A_L       (REG_TC_VE_ENC2_BASE + 0x54)
#define  REG_TC_VE_ENC2_2A_H       (REG_TC_VE_ENC2_BASE + 0x55)
#define  REG_TC_VE_ENC2_2B_L       (REG_TC_VE_ENC2_BASE + 0x56)
#define  REG_TC_VE_ENC2_2B_H       (REG_TC_VE_ENC2_BASE + 0x57)
#define  REG_TC_VE_ENC2_2C_L       (REG_TC_VE_ENC2_BASE + 0x58)
#define  REG_TC_VE_ENC2_2C_H       (REG_TC_VE_ENC2_BASE + 0x59)
#define  REG_TC_VE_ENC2_2D_L       (REG_TC_VE_ENC2_BASE + 0x5A)
#define  REG_TC_VE_ENC2_2D_H       (REG_TC_VE_ENC2_BASE + 0x5B)
#define  REG_TC_VE_ENC2_2E_L       (REG_TC_VE_ENC2_BASE + 0x5C)
#define  REG_TC_VE_ENC2_2E_H       (REG_TC_VE_ENC2_BASE + 0x5D)
#define  REG_TC_VE_ENC2_2F_L       (REG_TC_VE_ENC2_BASE + 0x5E)
#define  REG_TC_VE_ENC2_2F_H       (REG_TC_VE_ENC2_BASE + 0x5F)
#define  REG_TC_VE_ENC2_30_L       (REG_TC_VE_ENC2_BASE + 0x60)
#define  REG_TC_VE_ENC2_30_H       (REG_TC_VE_ENC2_BASE + 0x61)
#define  REG_TC_VE_ENC2_31_L       (REG_TC_VE_ENC2_BASE + 0x62)
#define  REG_TC_VE_ENC2_31_H       (REG_TC_VE_ENC2_BASE + 0x63)
#define  REG_TC_VE_ENC2_32_L       (REG_TC_VE_ENC2_BASE + 0x64)
#define  REG_TC_VE_ENC2_32_H       (REG_TC_VE_ENC2_BASE + 0x65)
#define  REG_TC_VE_ENC2_33_L       (REG_TC_VE_ENC2_BASE + 0x66)
#define  REG_TC_VE_ENC2_33_H       (REG_TC_VE_ENC2_BASE + 0x67)
#define  REG_TC_VE_ENC2_34_L       (REG_TC_VE_ENC2_BASE + 0x68)
#define  REG_TC_VE_ENC2_34_H       (REG_TC_VE_ENC2_BASE + 0x69)
#define  REG_TC_VE_ENC2_35_L       (REG_TC_VE_ENC2_BASE + 0x6A)
#define  REG_TC_VE_ENC2_35_H       (REG_TC_VE_ENC2_BASE + 0x6B)
#define  REG_TC_VE_ENC2_36_L       (REG_TC_VE_ENC2_BASE + 0x6C)
#define  REG_TC_VE_ENC2_36_H       (REG_TC_VE_ENC2_BASE + 0x6D)
#define  REG_TC_VE_ENC2_37_L       (REG_TC_VE_ENC2_BASE + 0x6E)
#define  REG_TC_VE_ENC2_37_H       (REG_TC_VE_ENC2_BASE + 0x6F)
#define  REG_TC_VE_ENC2_38_L       (REG_TC_VE_ENC2_BASE + 0x70)
#define  REG_TC_VE_ENC2_38_H       (REG_TC_VE_ENC2_BASE + 0x71)
#define  REG_TC_VE_ENC2_39_L       (REG_TC_VE_ENC2_BASE + 0x72)
#define  REG_TC_VE_ENC2_39_H       (REG_TC_VE_ENC2_BASE + 0x73)
#define  REG_TC_VE_ENC2_3A_L       (REG_TC_VE_ENC2_BASE + 0x74)
#define  REG_TC_VE_ENC2_3A_H       (REG_TC_VE_ENC2_BASE + 0x75)
#define  REG_TC_VE_ENC2_3B_L       (REG_TC_VE_ENC2_BASE + 0x76)
#define  REG_TC_VE_ENC2_3B_H       (REG_TC_VE_ENC2_BASE + 0x77)
#define  REG_TC_VE_ENC2_3C_L       (REG_TC_VE_ENC2_BASE + 0x78)
#define  REG_TC_VE_ENC2_3C_H       (REG_TC_VE_ENC2_BASE + 0x79)
#define  REG_TC_VE_ENC2_3D_L       (REG_TC_VE_ENC2_BASE + 0x7A)
#define  REG_TC_VE_ENC2_3D_H       (REG_TC_VE_ENC2_BASE + 0x7B)
#define  REG_TC_VE_ENC2_3E_L       (REG_TC_VE_ENC2_BASE + 0x7C)
#define  REG_TC_VE_ENC2_3E_H       (REG_TC_VE_ENC2_BASE + 0x7D)
#define  REG_TC_VE_ENC2_3F_L       (REG_TC_VE_ENC2_BASE + 0x7E)
#define  REG_TC_VE_ENC2_3F_H       (REG_TC_VE_ENC2_BASE + 0x7F)
#define  REG_TC_VE_ENC2_40_L       (REG_TC_VE_ENC2_BASE + 0x80)
#define  REG_TC_VE_ENC2_40_H       (REG_TC_VE_ENC2_BASE + 0x81)
#define  REG_TC_VE_ENC2_41_L       (REG_TC_VE_ENC2_BASE + 0x82)
#define  REG_TC_VE_ENC2_41_H       (REG_TC_VE_ENC2_BASE + 0x83)
#define  REG_TC_VE_ENC2_42_L       (REG_TC_VE_ENC2_BASE + 0x84)
#define  REG_TC_VE_ENC2_42_H       (REG_TC_VE_ENC2_BASE + 0x85)
#define  REG_TC_VE_ENC2_43_L       (REG_TC_VE_ENC2_BASE + 0x86)
#define  REG_TC_VE_ENC2_43_H       (REG_TC_VE_ENC2_BASE + 0x87)
#define  REG_TC_VE_ENC2_44_L       (REG_TC_VE_ENC2_BASE + 0x88)
#define  REG_TC_VE_ENC2_44_H       (REG_TC_VE_ENC2_BASE + 0x89)
#define  REG_TC_VE_ENC2_45_L       (REG_TC_VE_ENC2_BASE + 0x8A)
#define  REG_TC_VE_ENC2_45_H       (REG_TC_VE_ENC2_BASE + 0x8B)
#define  REG_TC_VE_ENC2_46_L       (REG_TC_VE_ENC2_BASE + 0x8C)
#define  REG_TC_VE_ENC2_46_H       (REG_TC_VE_ENC2_BASE + 0x8D)
#define  REG_TC_VE_ENC2_47_L       (REG_TC_VE_ENC2_BASE + 0x8E)
#define  REG_TC_VE_ENC2_47_H       (REG_TC_VE_ENC2_BASE + 0x8F)
#define  REG_TC_VE_ENC2_48_L       (REG_TC_VE_ENC2_BASE + 0x90)
#define  REG_TC_VE_ENC2_48_H       (REG_TC_VE_ENC2_BASE + 0x91)
#define  REG_TC_VE_ENC2_49_L       (REG_TC_VE_ENC2_BASE + 0x92)
#define  REG_TC_VE_ENC2_49_H       (REG_TC_VE_ENC2_BASE + 0x93)
#define  REG_TC_VE_ENC2_4A_L       (REG_TC_VE_ENC2_BASE + 0x94)
#define  REG_TC_VE_ENC2_4A_H       (REG_TC_VE_ENC2_BASE + 0x95)
#define  REG_TC_VE_ENC2_4B_L       (REG_TC_VE_ENC2_BASE + 0x96)
#define  REG_TC_VE_ENC2_4B_H       (REG_TC_VE_ENC2_BASE + 0x97)
#define  REG_TC_VE_ENC2_4C_L       (REG_TC_VE_ENC2_BASE + 0x98)
#define  REG_TC_VE_ENC2_4C_H       (REG_TC_VE_ENC2_BASE + 0x99)
#define  REG_TC_VE_ENC2_4D_L       (REG_TC_VE_ENC2_BASE + 0x9A)
#define  REG_TC_VE_ENC2_4D_H       (REG_TC_VE_ENC2_BASE + 0x9B)
#define  REG_TC_VE_ENC2_4E_L       (REG_TC_VE_ENC2_BASE + 0x9C)
#define  REG_TC_VE_ENC2_4E_H       (REG_TC_VE_ENC2_BASE + 0x9D)
#define  REG_TC_VE_ENC2_4F_L       (REG_TC_VE_ENC2_BASE + 0x9E)
#define  REG_TC_VE_ENC2_4F_H       (REG_TC_VE_ENC2_BASE + 0x9F)
#define  REG_TC_VE_ENC2_50_L       (REG_TC_VE_ENC2_BASE + 0xA0)
#define  REG_TC_VE_ENC2_50_H       (REG_TC_VE_ENC2_BASE + 0xA1)
#define  REG_TC_VE_ENC2_51_L       (REG_TC_VE_ENC2_BASE + 0xA2)
#define  REG_TC_VE_ENC2_51_H       (REG_TC_VE_ENC2_BASE + 0xA3)
#define  REG_TC_VE_ENC2_52_L       (REG_TC_VE_ENC2_BASE + 0xA4)
#define  REG_TC_VE_ENC2_52_H       (REG_TC_VE_ENC2_BASE + 0xA5)
#define  REG_TC_VE_ENC2_53_L       (REG_TC_VE_ENC2_BASE + 0xA6)
#define  REG_TC_VE_ENC2_53_H       (REG_TC_VE_ENC2_BASE + 0xA7)
#define  REG_TC_VE_ENC2_54_L       (REG_TC_VE_ENC2_BASE + 0xA8)
#define  REG_TC_VE_ENC2_54_H       (REG_TC_VE_ENC2_BASE + 0xA9)
#define  REG_TC_VE_ENC2_55_L       (REG_TC_VE_ENC2_BASE + 0xAA)
#define  REG_TC_VE_ENC2_55_H       (REG_TC_VE_ENC2_BASE + 0xAB)
#define  REG_TC_VE_ENC2_56_L       (REG_TC_VE_ENC2_BASE + 0xAC)
#define  REG_TC_VE_ENC2_56_H       (REG_TC_VE_ENC2_BASE + 0xAD)
#define  REG_TC_VE_ENC2_57_L       (REG_TC_VE_ENC2_BASE + 0xAE)
#define  REG_TC_VE_ENC2_57_H       (REG_TC_VE_ENC2_BASE + 0xAF)
#define  REG_TC_VE_ENC2_58_L       (REG_TC_VE_ENC2_BASE + 0xB0)
#define  REG_TC_VE_ENC2_58_H       (REG_TC_VE_ENC2_BASE + 0xB1)
#define  REG_TC_VE_ENC2_59_L       (REG_TC_VE_ENC2_BASE + 0xB2)
#define  REG_TC_VE_ENC2_59_H       (REG_TC_VE_ENC2_BASE + 0xB3)
#define  REG_TC_VE_ENC2_5A_L       (REG_TC_VE_ENC2_BASE + 0xB4)
#define  REG_TC_VE_ENC2_5A_H       (REG_TC_VE_ENC2_BASE + 0xB5)
#define  REG_TC_VE_ENC2_5B_L       (REG_TC_VE_ENC2_BASE + 0xB6)
#define  REG_TC_VE_ENC2_5B_H       (REG_TC_VE_ENC2_BASE + 0xB7)
#define  REG_TC_VE_ENC2_5C_L       (REG_TC_VE_ENC2_BASE + 0xB8)
#define  REG_TC_VE_ENC2_5C_H       (REG_TC_VE_ENC2_BASE + 0xB9)
#define  REG_TC_VE_ENC2_5D_L       (REG_TC_VE_ENC2_BASE + 0xBA)
#define  REG_TC_VE_ENC2_5D_H       (REG_TC_VE_ENC2_BASE + 0xBB)
#define  REG_TC_VE_ENC2_5E_L       (REG_TC_VE_ENC2_BASE + 0xBC)
#define  REG_TC_VE_ENC2_5E_H       (REG_TC_VE_ENC2_BASE + 0xBD)
#define  REG_TC_VE_ENC2_5F_L       (REG_TC_VE_ENC2_BASE + 0xBE)
#define  REG_TC_VE_ENC2_5F_H       (REG_TC_VE_ENC2_BASE + 0xBF)
#define  REG_TC_VE_ENC2_60_L       (REG_TC_VE_ENC2_BASE + 0xC0)
#define  REG_TC_VE_ENC2_60_H       (REG_TC_VE_ENC2_BASE + 0xC1)
#define  REG_TC_VE_ENC2_61_L       (REG_TC_VE_ENC2_BASE + 0xC2)
#define  REG_TC_VE_ENC2_61_H       (REG_TC_VE_ENC2_BASE + 0xC3)
#define  REG_TC_VE_ENC2_62_L       (REG_TC_VE_ENC2_BASE + 0xC4)
#define  REG_TC_VE_ENC2_62_H       (REG_TC_VE_ENC2_BASE + 0xC5)
#define  REG_TC_VE_ENC2_63_L       (REG_TC_VE_ENC2_BASE + 0xC6)
#define  REG_TC_VE_ENC2_63_H       (REG_TC_VE_ENC2_BASE + 0xC7)
#define  REG_TC_VE_ENC2_64_L       (REG_TC_VE_ENC2_BASE + 0xC8)
#define  REG_TC_VE_ENC2_64_H       (REG_TC_VE_ENC2_BASE + 0xC9)
#define  REG_TC_VE_ENC2_65_L       (REG_TC_VE_ENC2_BASE + 0xCA)
#define  REG_TC_VE_ENC2_65_H       (REG_TC_VE_ENC2_BASE + 0xCB)
#define  REG_TC_VE_ENC2_66_L       (REG_TC_VE_ENC2_BASE + 0xCC)
#define  REG_TC_VE_ENC2_66_H       (REG_TC_VE_ENC2_BASE + 0xCD)
#define  REG_TC_VE_ENC2_67_L       (REG_TC_VE_ENC2_BASE + 0xCE)
#define  REG_TC_VE_ENC2_67_H       (REG_TC_VE_ENC2_BASE + 0xCF)
#define  REG_TC_VE_ENC2_68_L       (REG_TC_VE_ENC2_BASE + 0xD0)
#define  REG_TC_VE_ENC2_68_H       (REG_TC_VE_ENC2_BASE + 0xD1)
#define  REG_TC_VE_ENC2_69_L       (REG_TC_VE_ENC2_BASE + 0xD2)
#define  REG_TC_VE_ENC2_69_H       (REG_TC_VE_ENC2_BASE + 0xD3)
#define  REG_TC_VE_ENC2_6A_L       (REG_TC_VE_ENC2_BASE + 0xD4)
#define  REG_TC_VE_ENC2_6A_H       (REG_TC_VE_ENC2_BASE + 0xD5)
#define  REG_TC_VE_ENC2_6B_L       (REG_TC_VE_ENC2_BASE + 0xD6)
#define  REG_TC_VE_ENC2_6B_H       (REG_TC_VE_ENC2_BASE + 0xD7)
#define  REG_TC_VE_ENC2_6C_L       (REG_TC_VE_ENC2_BASE + 0xD8)
#define  REG_TC_VE_ENC2_6C_H       (REG_TC_VE_ENC2_BASE + 0xD9)
#define  REG_TC_VE_ENC2_6D_L       (REG_TC_VE_ENC2_BASE + 0xDA)
#define  REG_TC_VE_ENC2_6D_H       (REG_TC_VE_ENC2_BASE + 0xDB)
#define  REG_TC_VE_ENC2_6E_L       (REG_TC_VE_ENC2_BASE + 0xDC)
#define  REG_TC_VE_ENC2_6E_H       (REG_TC_VE_ENC2_BASE + 0xDD)
#define  REG_TC_VE_ENC2_6F_L       (REG_TC_VE_ENC2_BASE + 0xDE)
#define  REG_TC_VE_ENC2_6F_H       (REG_TC_VE_ENC2_BASE + 0xDF)
#define  REG_TC_VE_ENC2_70_L       (REG_TC_VE_ENC2_BASE + 0xE0)
#define  REG_TC_VE_ENC2_70_H       (REG_TC_VE_ENC2_BASE + 0xE1)
#define  REG_TC_VE_ENC2_71_L       (REG_TC_VE_ENC2_BASE + 0xE2)
#define  REG_TC_VE_ENC2_71_H       (REG_TC_VE_ENC2_BASE + 0xE3)
#define  REG_TC_VE_ENC2_72_L       (REG_TC_VE_ENC2_BASE + 0xE4)
#define  REG_TC_VE_ENC2_72_H       (REG_TC_VE_ENC2_BASE + 0xE5)
#define  REG_TC_VE_ENC2_73_L       (REG_TC_VE_ENC2_BASE + 0xE6)
#define  REG_TC_VE_ENC2_73_H       (REG_TC_VE_ENC2_BASE + 0xE7)
#define  REG_TC_VE_ENC2_74_L       (REG_TC_VE_ENC2_BASE + 0xE8)
#define  REG_TC_VE_ENC2_74_H       (REG_TC_VE_ENC2_BASE + 0xE9)
#define  REG_TC_VE_ENC2_75_L       (REG_TC_VE_ENC2_BASE + 0xEA)
#define  REG_TC_VE_ENC2_75_H       (REG_TC_VE_ENC2_BASE + 0xEB)
#define  REG_TC_VE_ENC2_76_L       (REG_TC_VE_ENC2_BASE + 0xEC)
#define  REG_TC_VE_ENC2_76_H       (REG_TC_VE_ENC2_BASE + 0xED)
#define  REG_TC_VE_ENC2_77_L       (REG_TC_VE_ENC2_BASE + 0xEE)
#define  REG_TC_VE_ENC2_77_H       (REG_TC_VE_ENC2_BASE + 0xEF)
#define  REG_TC_VE_ENC2_78_L       (REG_TC_VE_ENC2_BASE + 0xF0)
#define  REG_TC_VE_ENC2_78_H       (REG_TC_VE_ENC2_BASE + 0xF1)
#define  REG_TC_VE_ENC2_79_L       (REG_TC_VE_ENC2_BASE + 0xF2)
#define  REG_TC_VE_ENC2_79_H       (REG_TC_VE_ENC2_BASE + 0xF3)
#define  REG_TC_VE_ENC2_7A_L       (REG_TC_VE_ENC2_BASE + 0xF4)
#define  REG_TC_VE_ENC2_7A_H       (REG_TC_VE_ENC2_BASE + 0xF5)
#define  REG_TC_VE_ENC2_7B_L       (REG_TC_VE_ENC2_BASE + 0xF6)
#define  REG_TC_VE_ENC2_7B_H       (REG_TC_VE_ENC2_BASE + 0xF7)
#define  REG_TC_VE_ENC2_7C_L       (REG_TC_VE_ENC2_BASE + 0xF8)
#define  REG_TC_VE_ENC2_7C_H       (REG_TC_VE_ENC2_BASE + 0xF9)
#define  REG_TC_VE_ENC2_7D_L       (REG_TC_VE_ENC2_BASE + 0xFA)
#define  REG_TC_VE_ENC2_7D_H       (REG_TC_VE_ENC2_BASE + 0xFB)
#define  REG_TC_VE_ENC2_7E_L       (REG_TC_VE_ENC2_BASE + 0xFC)
#define  REG_TC_VE_ENC2_7E_H       (REG_TC_VE_ENC2_BASE + 0xFD)
#define  REG_TC_VE_ENC2_7F_L       (REG_TC_VE_ENC2_BASE + 0xFE)
#define  REG_TC_VE_ENC2_7F_H       (REG_TC_VE_ENC2_BASE + 0xFF)

#define  REG_TC_COMPONENT_00_L       (REG_TC_COMPONENT_BASE + 0x00)
#define  REG_TC_COMPONENT_00_H       (REG_TC_COMPONENT_BASE + 0x01)
#define  REG_TC_COMPONENT_01_L       (REG_TC_COMPONENT_BASE + 0x02)
#define  REG_TC_COMPONENT_01_H       (REG_TC_COMPONENT_BASE + 0x03)
#define  REG_TC_COMPONENT_02_L       (REG_TC_COMPONENT_BASE + 0x04)
#define  REG_TC_COMPONENT_02_H       (REG_TC_COMPONENT_BASE + 0x05)
#define  REG_TC_COMPONENT_03_L       (REG_TC_COMPONENT_BASE + 0x06)
#define  REG_TC_COMPONENT_03_H       (REG_TC_COMPONENT_BASE + 0x07)
#define  REG_TC_COMPONENT_04_L       (REG_TC_COMPONENT_BASE + 0x08)
#define  REG_TC_COMPONENT_04_H       (REG_TC_COMPONENT_BASE + 0x09)
#define  REG_TC_COMPONENT_05_L       (REG_TC_COMPONENT_BASE + 0x0A)
#define  REG_TC_COMPONENT_05_H       (REG_TC_COMPONENT_BASE + 0x0B)
#define  REG_TC_COMPONENT_06_L       (REG_TC_COMPONENT_BASE + 0x0C)
#define  REG_TC_COMPONENT_06_H       (REG_TC_COMPONENT_BASE + 0x0D)
#define  REG_TC_COMPONENT_07_L       (REG_TC_COMPONENT_BASE + 0x0E)
#define  REG_TC_COMPONENT_07_H       (REG_TC_COMPONENT_BASE + 0x0F)
#define  REG_TC_COMPONENT_08_L       (REG_TC_COMPONENT_BASE + 0x10)
#define  REG_TC_COMPONENT_08_H       (REG_TC_COMPONENT_BASE + 0x11)
#define  REG_TC_COMPONENT_09_L       (REG_TC_COMPONENT_BASE + 0x12)
#define  REG_TC_COMPONENT_09_H       (REG_TC_COMPONENT_BASE + 0x13)
#define  REG_TC_COMPONENT_0A_L       (REG_TC_COMPONENT_BASE + 0x14)
#define  REG_TC_COMPONENT_0A_H       (REG_TC_COMPONENT_BASE + 0x15)
#define  REG_TC_COMPONENT_0B_L       (REG_TC_COMPONENT_BASE + 0x16)
#define  REG_TC_COMPONENT_0B_H       (REG_TC_COMPONENT_BASE + 0x17)
#define  REG_TC_COMPONENT_0C_L       (REG_TC_COMPONENT_BASE + 0x18)
#define  REG_TC_COMPONENT_0C_H       (REG_TC_COMPONENT_BASE + 0x19)
#define  REG_TC_COMPONENT_0D_L       (REG_TC_COMPONENT_BASE + 0x1A)
#define  REG_TC_COMPONENT_0D_H       (REG_TC_COMPONENT_BASE + 0x1B)
#define  REG_TC_COMPONENT_0E_L       (REG_TC_COMPONENT_BASE + 0x1C)
#define  REG_TC_COMPONENT_0E_H       (REG_TC_COMPONENT_BASE + 0x1D)
#define  REG_TC_COMPONENT_0F_L       (REG_TC_COMPONENT_BASE + 0x1E)
#define  REG_TC_COMPONENT_0F_H       (REG_TC_COMPONENT_BASE + 0x1F)
#define  REG_TC_COMPONENT_10_L       (REG_TC_COMPONENT_BASE + 0x20)
#define  REG_TC_COMPONENT_10_H       (REG_TC_COMPONENT_BASE + 0x21)
#define  REG_TC_COMPONENT_11_L       (REG_TC_COMPONENT_BASE + 0x22)
#define  REG_TC_COMPONENT_11_H       (REG_TC_COMPONENT_BASE + 0x23)
#define  REG_TC_COMPONENT_12_L       (REG_TC_COMPONENT_BASE + 0x24)
#define  REG_TC_COMPONENT_12_H       (REG_TC_COMPONENT_BASE + 0x25)
#define  REG_TC_COMPONENT_13_L       (REG_TC_COMPONENT_BASE + 0x26)
#define  REG_TC_COMPONENT_13_H       (REG_TC_COMPONENT_BASE + 0x27)
#define  REG_TC_COMPONENT_14_L       (REG_TC_COMPONENT_BASE + 0x28)
#define  REG_TC_COMPONENT_14_H       (REG_TC_COMPONENT_BASE + 0x29)
#define  REG_TC_COMPONENT_15_L       (REG_TC_COMPONENT_BASE + 0x2A)
#define  REG_TC_COMPONENT_15_H       (REG_TC_COMPONENT_BASE + 0x2B)
#define  REG_TC_COMPONENT_16_L       (REG_TC_COMPONENT_BASE + 0x2C)
#define  REG_TC_COMPONENT_16_H       (REG_TC_COMPONENT_BASE + 0x2D)
#define  REG_TC_COMPONENT_17_L       (REG_TC_COMPONENT_BASE + 0x2E)
#define  REG_TC_COMPONENT_17_H       (REG_TC_COMPONENT_BASE + 0x2F)
#define  REG_TC_COMPONENT_18_L       (REG_TC_COMPONENT_BASE + 0x30)
#define  REG_TC_COMPONENT_18_H       (REG_TC_COMPONENT_BASE + 0x31)
#define  REG_TC_COMPONENT_19_L       (REG_TC_COMPONENT_BASE + 0x32)
#define  REG_TC_COMPONENT_19_H       (REG_TC_COMPONENT_BASE + 0x33)
#define  REG_TC_COMPONENT_1A_L       (REG_TC_COMPONENT_BASE + 0x34)
#define  REG_TC_COMPONENT_1A_H       (REG_TC_COMPONENT_BASE + 0x35)
#define  REG_TC_COMPONENT_1B_L       (REG_TC_COMPONENT_BASE + 0x36)
#define  REG_TC_COMPONENT_1B_H       (REG_TC_COMPONENT_BASE + 0x37)
#define  REG_TC_COMPONENT_1C_L       (REG_TC_COMPONENT_BASE + 0x38)
#define  REG_TC_COMPONENT_1C_H       (REG_TC_COMPONENT_BASE + 0x39)
#define  REG_TC_COMPONENT_1D_L       (REG_TC_COMPONENT_BASE + 0x3A)
#define  REG_TC_COMPONENT_1D_H       (REG_TC_COMPONENT_BASE + 0x3B)
#define  REG_TC_COMPONENT_1E_L       (REG_TC_COMPONENT_BASE + 0x3C)
#define  REG_TC_COMPONENT_1E_H       (REG_TC_COMPONENT_BASE + 0x3D)
#define  REG_TC_COMPONENT_1F_L       (REG_TC_COMPONENT_BASE + 0x3E)
#define  REG_TC_COMPONENT_1F_H       (REG_TC_COMPONENT_BASE + 0x3F)
#define  REG_TC_COMPONENT_20_L       (REG_TC_COMPONENT_BASE + 0x40)
#define  REG_TC_COMPONENT_20_H       (REG_TC_COMPONENT_BASE + 0x41)
#define  REG_TC_COMPONENT_21_L       (REG_TC_COMPONENT_BASE + 0x42)
#define  REG_TC_COMPONENT_21_H       (REG_TC_COMPONENT_BASE + 0x43)
#define  REG_TC_COMPONENT_22_L       (REG_TC_COMPONENT_BASE + 0x44)
#define  REG_TC_COMPONENT_22_H       (REG_TC_COMPONENT_BASE + 0x45)
#define  REG_TC_COMPONENT_23_L       (REG_TC_COMPONENT_BASE + 0x46)
#define  REG_TC_COMPONENT_23_H       (REG_TC_COMPONENT_BASE + 0x47)
#define  REG_TC_COMPONENT_24_L       (REG_TC_COMPONENT_BASE + 0x48)
#define  REG_TC_COMPONENT_24_H       (REG_TC_COMPONENT_BASE + 0x49)
#define  REG_TC_COMPONENT_25_L       (REG_TC_COMPONENT_BASE + 0x4A)
#define  REG_TC_COMPONENT_25_H       (REG_TC_COMPONENT_BASE + 0x4B)
#define  REG_TC_COMPONENT_26_L       (REG_TC_COMPONENT_BASE + 0x4C)
#define  REG_TC_COMPONENT_26_H       (REG_TC_COMPONENT_BASE + 0x4D)
#define  REG_TC_COMPONENT_27_L       (REG_TC_COMPONENT_BASE + 0x4E)
#define  REG_TC_COMPONENT_27_H       (REG_TC_COMPONENT_BASE + 0x4F)
#define  REG_TC_COMPONENT_28_L       (REG_TC_COMPONENT_BASE + 0x50)
#define  REG_TC_COMPONENT_28_H       (REG_TC_COMPONENT_BASE + 0x51)
#define  REG_TC_COMPONENT_29_L       (REG_TC_COMPONENT_BASE + 0x52)
#define  REG_TC_COMPONENT_29_H       (REG_TC_COMPONENT_BASE + 0x53)
#define  REG_TC_COMPONENT_2A_L       (REG_TC_COMPONENT_BASE + 0x54)
#define  REG_TC_COMPONENT_2A_H       (REG_TC_COMPONENT_BASE + 0x55)
#define  REG_TC_COMPONENT_2B_L       (REG_TC_COMPONENT_BASE + 0x56)
#define  REG_TC_COMPONENT_2B_H       (REG_TC_COMPONENT_BASE + 0x57)
#define  REG_TC_COMPONENT_2C_L       (REG_TC_COMPONENT_BASE + 0x58)
#define  REG_TC_COMPONENT_2C_H       (REG_TC_COMPONENT_BASE + 0x59)
#define  REG_TC_COMPONENT_2D_L       (REG_TC_COMPONENT_BASE + 0x5A)
#define  REG_TC_COMPONENT_2D_H       (REG_TC_COMPONENT_BASE + 0x5B)
#define  REG_TC_COMPONENT_2E_L       (REG_TC_COMPONENT_BASE + 0x5C)
#define  REG_TC_COMPONENT_2E_H       (REG_TC_COMPONENT_BASE + 0x5D)
#define  REG_TC_COMPONENT_2F_L       (REG_TC_COMPONENT_BASE + 0x5E)
#define  REG_TC_COMPONENT_2F_H       (REG_TC_COMPONENT_BASE + 0x5F)
#define  REG_TC_COMPONENT_30_L       (REG_TC_COMPONENT_BASE + 0x60)
#define  REG_TC_COMPONENT_30_H       (REG_TC_COMPONENT_BASE + 0x61)
#define  REG_TC_COMPONENT_31_L       (REG_TC_COMPONENT_BASE + 0x62)
#define  REG_TC_COMPONENT_31_H       (REG_TC_COMPONENT_BASE + 0x63)
#define  REG_TC_COMPONENT_32_L       (REG_TC_COMPONENT_BASE + 0x64)
#define  REG_TC_COMPONENT_32_H       (REG_TC_COMPONENT_BASE + 0x65)
#define  REG_TC_COMPONENT_33_L       (REG_TC_COMPONENT_BASE + 0x66)
#define  REG_TC_COMPONENT_33_H       (REG_TC_COMPONENT_BASE + 0x67)
#define  REG_TC_COMPONENT_34_L       (REG_TC_COMPONENT_BASE + 0x68)
#define  REG_TC_COMPONENT_34_H       (REG_TC_COMPONENT_BASE + 0x69)
#define  REG_TC_COMPONENT_35_L       (REG_TC_COMPONENT_BASE + 0x6A)
#define  REG_TC_COMPONENT_35_H       (REG_TC_COMPONENT_BASE + 0x6B)
#define  REG_TC_COMPONENT_36_L       (REG_TC_COMPONENT_BASE + 0x6C)
#define  REG_TC_COMPONENT_36_H       (REG_TC_COMPONENT_BASE + 0x6D)
#define  REG_TC_COMPONENT_37_L       (REG_TC_COMPONENT_BASE + 0x6E)
#define  REG_TC_COMPONENT_37_H       (REG_TC_COMPONENT_BASE + 0x6F)
#define  REG_TC_COMPONENT_38_L       (REG_TC_COMPONENT_BASE + 0x70)
#define  REG_TC_COMPONENT_38_H       (REG_TC_COMPONENT_BASE + 0x71)
#define  REG_TC_COMPONENT_39_L       (REG_TC_COMPONENT_BASE + 0x72)
#define  REG_TC_COMPONENT_39_H       (REG_TC_COMPONENT_BASE + 0x73)
#define  REG_TC_COMPONENT_3A_L       (REG_TC_COMPONENT_BASE + 0x74)
#define  REG_TC_COMPONENT_3A_H       (REG_TC_COMPONENT_BASE + 0x75)
#define  REG_TC_COMPONENT_3B_L       (REG_TC_COMPONENT_BASE + 0x76)
#define  REG_TC_COMPONENT_3B_H       (REG_TC_COMPONENT_BASE + 0x77)
#define  REG_TC_COMPONENT_3C_L       (REG_TC_COMPONENT_BASE + 0x78)
#define  REG_TC_COMPONENT_3C_H       (REG_TC_COMPONENT_BASE + 0x79)
#define  REG_TC_COMPONENT_3D_L       (REG_TC_COMPONENT_BASE + 0x7A)
#define  REG_TC_COMPONENT_3D_H       (REG_TC_COMPONENT_BASE + 0x7B)
#define  REG_TC_COMPONENT_3E_L       (REG_TC_COMPONENT_BASE + 0x7C)
#define  REG_TC_COMPONENT_3E_H       (REG_TC_COMPONENT_BASE + 0x7D)
#define  REG_TC_COMPONENT_3F_L       (REG_TC_COMPONENT_BASE + 0x7E)
#define  REG_TC_COMPONENT_3F_H       (REG_TC_COMPONENT_BASE + 0x7F)
#define  REG_TC_COMPONENT_40_L       (REG_TC_COMPONENT_BASE + 0x80)
#define  REG_TC_COMPONENT_40_H       (REG_TC_COMPONENT_BASE + 0x81)
#define  REG_TC_COMPONENT_41_L       (REG_TC_COMPONENT_BASE + 0x82)
#define  REG_TC_COMPONENT_41_H       (REG_TC_COMPONENT_BASE + 0x83)
#define  REG_TC_COMPONENT_42_L       (REG_TC_COMPONENT_BASE + 0x84)
#define  REG_TC_COMPONENT_42_H       (REG_TC_COMPONENT_BASE + 0x85)
#define  REG_TC_COMPONENT_43_L       (REG_TC_COMPONENT_BASE + 0x86)
#define  REG_TC_COMPONENT_43_H       (REG_TC_COMPONENT_BASE + 0x87)
#define  REG_TC_COMPONENT_44_L       (REG_TC_COMPONENT_BASE + 0x88)
#define  REG_TC_COMPONENT_44_H       (REG_TC_COMPONENT_BASE + 0x89)
#define  REG_TC_COMPONENT_45_L       (REG_TC_COMPONENT_BASE + 0x8A)
#define  REG_TC_COMPONENT_45_H       (REG_TC_COMPONENT_BASE + 0x8B)
#define  REG_TC_COMPONENT_46_L       (REG_TC_COMPONENT_BASE + 0x8C)
#define  REG_TC_COMPONENT_46_H       (REG_TC_COMPONENT_BASE + 0x8D)
#define  REG_TC_COMPONENT_47_L       (REG_TC_COMPONENT_BASE + 0x8E)
#define  REG_TC_COMPONENT_47_H       (REG_TC_COMPONENT_BASE + 0x8F)
#define  REG_TC_COMPONENT_48_L       (REG_TC_COMPONENT_BASE + 0x90)
#define  REG_TC_COMPONENT_48_H       (REG_TC_COMPONENT_BASE + 0x91)
#define  REG_TC_COMPONENT_49_L       (REG_TC_COMPONENT_BASE + 0x92)
#define  REG_TC_COMPONENT_49_H       (REG_TC_COMPONENT_BASE + 0x93)
#define  REG_TC_COMPONENT_4A_L       (REG_TC_COMPONENT_BASE + 0x94)
#define  REG_TC_COMPONENT_4A_H       (REG_TC_COMPONENT_BASE + 0x95)
#define  REG_TC_COMPONENT_4B_L       (REG_TC_COMPONENT_BASE + 0x96)
#define  REG_TC_COMPONENT_4B_H       (REG_TC_COMPONENT_BASE + 0x97)
#define  REG_TC_COMPONENT_4C_L       (REG_TC_COMPONENT_BASE + 0x98)
#define  REG_TC_COMPONENT_4C_H       (REG_TC_COMPONENT_BASE + 0x99)
#define  REG_TC_COMPONENT_4D_L       (REG_TC_COMPONENT_BASE + 0x9A)
#define  REG_TC_COMPONENT_4D_H       (REG_TC_COMPONENT_BASE + 0x9B)
#define  REG_TC_COMPONENT_4E_L       (REG_TC_COMPONENT_BASE + 0x9C)
#define  REG_TC_COMPONENT_4E_H       (REG_TC_COMPONENT_BASE + 0x9D)
#define  REG_TC_COMPONENT_4F_L       (REG_TC_COMPONENT_BASE + 0x9E)
#define  REG_TC_COMPONENT_4F_H       (REG_TC_COMPONENT_BASE + 0x9F)
#define  REG_TC_COMPONENT_50_L       (REG_TC_COMPONENT_BASE + 0xA0)
#define  REG_TC_COMPONENT_50_H       (REG_TC_COMPONENT_BASE + 0xA1)
#define  REG_TC_COMPONENT_51_L       (REG_TC_COMPONENT_BASE + 0xA2)
#define  REG_TC_COMPONENT_51_H       (REG_TC_COMPONENT_BASE + 0xA3)
#define  REG_TC_COMPONENT_52_L       (REG_TC_COMPONENT_BASE + 0xA4)
#define  REG_TC_COMPONENT_52_H       (REG_TC_COMPONENT_BASE + 0xA5)
#define  REG_TC_COMPONENT_53_L       (REG_TC_COMPONENT_BASE + 0xA6)
#define  REG_TC_COMPONENT_53_H       (REG_TC_COMPONENT_BASE + 0xA7)
#define  REG_TC_COMPONENT_54_L       (REG_TC_COMPONENT_BASE + 0xA8)
#define  REG_TC_COMPONENT_54_H       (REG_TC_COMPONENT_BASE + 0xA9)
#define  REG_TC_COMPONENT_55_L       (REG_TC_COMPONENT_BASE + 0xAA)
#define  REG_TC_COMPONENT_55_H       (REG_TC_COMPONENT_BASE + 0xAB)
#define  REG_TC_COMPONENT_56_L       (REG_TC_COMPONENT_BASE + 0xAC)
#define  REG_TC_COMPONENT_56_H       (REG_TC_COMPONENT_BASE + 0xAD)
#define  REG_TC_COMPONENT_57_L       (REG_TC_COMPONENT_BASE + 0xAE)
#define  REG_TC_COMPONENT_57_H       (REG_TC_COMPONENT_BASE + 0xAF)
#define  REG_TC_COMPONENT_58_L       (REG_TC_COMPONENT_BASE + 0xB0)
#define  REG_TC_COMPONENT_58_H       (REG_TC_COMPONENT_BASE + 0xB1)
#define  REG_TC_COMPONENT_59_L       (REG_TC_COMPONENT_BASE + 0xB2)
#define  REG_TC_COMPONENT_59_H       (REG_TC_COMPONENT_BASE + 0xB3)
#define  REG_TC_COMPONENT_5A_L       (REG_TC_COMPONENT_BASE + 0xB4)
#define  REG_TC_COMPONENT_5A_H       (REG_TC_COMPONENT_BASE + 0xB5)
#define  REG_TC_COMPONENT_5B_L       (REG_TC_COMPONENT_BASE + 0xB6)
#define  REG_TC_COMPONENT_5B_H       (REG_TC_COMPONENT_BASE + 0xB7)
#define  REG_TC_COMPONENT_5C_L       (REG_TC_COMPONENT_BASE + 0xB8)
#define  REG_TC_COMPONENT_5C_H       (REG_TC_COMPONENT_BASE + 0xB9)
#define  REG_TC_COMPONENT_5D_L       (REG_TC_COMPONENT_BASE + 0xBA)
#define  REG_TC_COMPONENT_5D_H       (REG_TC_COMPONENT_BASE + 0xBB)
#define  REG_TC_COMPONENT_5E_L       (REG_TC_COMPONENT_BASE + 0xBC)
#define  REG_TC_COMPONENT_5E_H       (REG_TC_COMPONENT_BASE + 0xBD)
#define  REG_TC_COMPONENT_5F_L       (REG_TC_COMPONENT_BASE + 0xBE)
#define  REG_TC_COMPONENT_5F_H       (REG_TC_COMPONENT_BASE + 0xBF)
#define  REG_TC_COMPONENT_60_L       (REG_TC_COMPONENT_BASE + 0xC0)
#define  REG_TC_COMPONENT_60_H       (REG_TC_COMPONENT_BASE + 0xC1)
#define  REG_TC_COMPONENT_61_L       (REG_TC_COMPONENT_BASE + 0xC2)
#define  REG_TC_COMPONENT_61_H       (REG_TC_COMPONENT_BASE + 0xC3)
#define  REG_TC_COMPONENT_62_L       (REG_TC_COMPONENT_BASE + 0xC4)
#define  REG_TC_COMPONENT_62_H       (REG_TC_COMPONENT_BASE + 0xC5)
#define  REG_TC_COMPONENT_63_L       (REG_TC_COMPONENT_BASE + 0xC6)
#define  REG_TC_COMPONENT_63_H       (REG_TC_COMPONENT_BASE + 0xC7)
#define  REG_TC_COMPONENT_64_L       (REG_TC_COMPONENT_BASE + 0xC8)
#define  REG_TC_COMPONENT_64_H       (REG_TC_COMPONENT_BASE + 0xC9)
#define  REG_TC_COMPONENT_65_L       (REG_TC_COMPONENT_BASE + 0xCA)
#define  REG_TC_COMPONENT_65_H       (REG_TC_COMPONENT_BASE + 0xCB)
#define  REG_TC_COMPONENT_66_L       (REG_TC_COMPONENT_BASE + 0xCC)
#define  REG_TC_COMPONENT_66_H       (REG_TC_COMPONENT_BASE + 0xCD)
#define  REG_TC_COMPONENT_67_L       (REG_TC_COMPONENT_BASE + 0xCE)
#define  REG_TC_COMPONENT_67_H       (REG_TC_COMPONENT_BASE + 0xCF)
#define  REG_TC_COMPONENT_68_L       (REG_TC_COMPONENT_BASE + 0xD0)
#define  REG_TC_COMPONENT_68_H       (REG_TC_COMPONENT_BASE + 0xD1)
#define  REG_TC_COMPONENT_69_L       (REG_TC_COMPONENT_BASE + 0xD2)
#define  REG_TC_COMPONENT_69_H       (REG_TC_COMPONENT_BASE + 0xD3)
#define  REG_TC_COMPONENT_6A_L       (REG_TC_COMPONENT_BASE + 0xD4)
#define  REG_TC_COMPONENT_6A_H       (REG_TC_COMPONENT_BASE + 0xD5)
#define  REG_TC_COMPONENT_6B_L       (REG_TC_COMPONENT_BASE + 0xD6)
#define  REG_TC_COMPONENT_6B_H       (REG_TC_COMPONENT_BASE + 0xD7)
#define  REG_TC_COMPONENT_6C_L       (REG_TC_COMPONENT_BASE + 0xD8)
#define  REG_TC_COMPONENT_6C_H       (REG_TC_COMPONENT_BASE + 0xD9)
#define  REG_TC_COMPONENT_6D_L       (REG_TC_COMPONENT_BASE + 0xDA)
#define  REG_TC_COMPONENT_6D_H       (REG_TC_COMPONENT_BASE + 0xDB)
#define  REG_TC_COMPONENT_6E_L       (REG_TC_COMPONENT_BASE + 0xDC)
#define  REG_TC_COMPONENT_6E_H       (REG_TC_COMPONENT_BASE + 0xDD)
#define  REG_TC_COMPONENT_6F_L       (REG_TC_COMPONENT_BASE + 0xDE)
#define  REG_TC_COMPONENT_6F_H       (REG_TC_COMPONENT_BASE + 0xDF)
#define  REG_TC_COMPONENT_70_L       (REG_TC_COMPONENT_BASE + 0xE0)
#define  REG_TC_COMPONENT_70_H       (REG_TC_COMPONENT_BASE + 0xE1)
#define  REG_TC_COMPONENT_71_L       (REG_TC_COMPONENT_BASE + 0xE2)
#define  REG_TC_COMPONENT_71_H       (REG_TC_COMPONENT_BASE + 0xE3)
#define  REG_TC_COMPONENT_72_L       (REG_TC_COMPONENT_BASE + 0xE4)
#define  REG_TC_COMPONENT_72_H       (REG_TC_COMPONENT_BASE + 0xE5)
#define  REG_TC_COMPONENT_73_L       (REG_TC_COMPONENT_BASE + 0xE6)
#define  REG_TC_COMPONENT_73_H       (REG_TC_COMPONENT_BASE + 0xE7)
#define  REG_TC_COMPONENT_74_L       (REG_TC_COMPONENT_BASE + 0xE8)
#define  REG_TC_COMPONENT_74_H       (REG_TC_COMPONENT_BASE + 0xE9)
#define  REG_TC_COMPONENT_75_L       (REG_TC_COMPONENT_BASE + 0xEA)
#define  REG_TC_COMPONENT_75_H       (REG_TC_COMPONENT_BASE + 0xEB)
#define  REG_TC_COMPONENT_76_L       (REG_TC_COMPONENT_BASE + 0xEC)
#define  REG_TC_COMPONENT_76_H       (REG_TC_COMPONENT_BASE + 0xED)
#define  REG_TC_COMPONENT_77_L       (REG_TC_COMPONENT_BASE + 0xEE)
#define  REG_TC_COMPONENT_77_H       (REG_TC_COMPONENT_BASE + 0xEF)
#define  REG_TC_COMPONENT_78_L       (REG_TC_COMPONENT_BASE + 0xF0)
#define  REG_TC_COMPONENT_78_H       (REG_TC_COMPONENT_BASE + 0xF1)
#define  REG_TC_COMPONENT_79_L       (REG_TC_COMPONENT_BASE + 0xF2)
#define  REG_TC_COMPONENT_79_H       (REG_TC_COMPONENT_BASE + 0xF3)
#define  REG_TC_COMPONENT_7A_L       (REG_TC_COMPONENT_BASE + 0xF4)
#define  REG_TC_COMPONENT_7A_H       (REG_TC_COMPONENT_BASE + 0xF5)
#define  REG_TC_COMPONENT_7B_L       (REG_TC_COMPONENT_BASE + 0xF6)
#define  REG_TC_COMPONENT_7B_H       (REG_TC_COMPONENT_BASE + 0xF7)
#define  REG_TC_COMPONENT_7C_L       (REG_TC_COMPONENT_BASE + 0xF8)
#define  REG_TC_COMPONENT_7C_H       (REG_TC_COMPONENT_BASE + 0xF9)
#define  REG_TC_COMPONENT_7D_L       (REG_TC_COMPONENT_BASE + 0xFA)
#define  REG_TC_COMPONENT_7D_H       (REG_TC_COMPONENT_BASE + 0xFB)
#define  REG_TC_COMPONENT_7E_L       (REG_TC_COMPONENT_BASE + 0xFC)
#define  REG_TC_COMPONENT_7E_H       (REG_TC_COMPONENT_BASE + 0xFD)
#define  REG_TC_COMPONENT_7F_L       (REG_TC_COMPONENT_BASE + 0xFE)
#define  REG_TC_COMPONENT_7F_H       (REG_TC_COMPONENT_BASE + 0xFF)

#ifdef __cplusplus
}
#endif

#undef _DRVADCTBL_H_
#endif
