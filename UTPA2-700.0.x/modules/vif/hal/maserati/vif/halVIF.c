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
#ifndef _HALVIF_C_
#define _HALVIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvMMIO.h"

// Internal Definition
#include "regVIF.h"
#include "VIF.h"
#include "halVIF.h"
#include "halVIF_Customer.h"
#include "asmCPU.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define HALVIFDBG(x)          //x
#define HALVIFDBG_BIT       (DBB1_REG_BASE+0x06)  // Bit 4~7
#define HALVIFDBG1_BIT       (DBB1_REG_BASE+0x04)  // Bit 1
#define HALVIFDBG2_BIT     (DBB1_REG_BASE+0xF6)  // Bit 0~1
//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
#define HAL_VIF_Delay1ms(x)                MAsm_CPU_DelayMs(x)
#define HAL_VIF_Delay1us(x)                  MAsm_CPU_DelayUs(x)
#define HAL_VIF_GetSystemTime()                     MsOS_GetSystemTime()
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define __CHIP_VERSION 0x1ECF

#ifndef _END_OF_TBL_
#define _END_OF_TBL_        0xFFFF
#endif

#define msRead2Bytes(x)                 RIU_Read2Byte(x)
#define msReadByte(x)                   RIU_ReadByte(x)

// Base address should be initial.
#if defined (__aeon__)            // Non-OS
#define BASEADDR_RIU 0xA0000000UL
#else                                       // ecos
    #define BASEADDR_RIU 0xBF800000UL
#endif

#define RIU_MACRO_START     do {
#define RIU_MACRO_END       } while (0)

// Address bus of RIU is 16 bits.
#define RIU_READ_BYTE(addr)         ( READ_BYTE( _hal_VIF.virtVIFBaseAddr + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( _hal_VIF.virtVIFBaseAddr + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _hal_VIF.virtVIFBaseAddr + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _hal_VIF.virtVIFBaseAddr + (addr), val) }

// Standard Form

#define RIU_ReadByte( u32Reg )   RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define RIU_Read2Byte( u32Reg )    (RIU_READ_2BYTE((u32Reg)<<1))

#define RIU_ReadRegBit( u32Reg, u8Mask )   (RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define RIU_WriteByte( u32Reg, u8Val )                                                  \
    RIU_MACRO_START                                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);            \
    RIU_MACRO_END

#define RIU_Write2Byte( u32Reg, u16Val )                                               \
    RIU_MACRO_START                                                                                     \
    if ( ((u32Reg) & 0x01) )                                                                            \
    {                                                                                                                \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                     \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    RIU_MACRO_END

#define RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

// Address bus of RIU is 16 bits for PM bank.
#define PM_RIU_READ_BYTE(addr)         ( READ_BYTE(virtPMBank + (addr) ) )
#define PM_RIU_READ_2BYTE(addr)        ( READ_WORD(virtPMBank+ (addr) ) )
#define PM_RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE(virtPMBank + (addr), val) }
#define PM_RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD(virtPMBank + (addr), val) }

// Standard Form for PM bank
#define PM_RIU_ReadByte( u32Reg )   PM_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define PM_RIU_Read2Byte( u32Reg )    (PM_RIU_READ_2BYTE((u32Reg)<<1))

#define PM_RIU_ReadRegBit( u32Reg, u8Mask )   (PM_RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define PM_RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    RIU_MACRO_START                                                                     \
    PM_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (PM_RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (PM_RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define PM_RIU_WriteByte( u32Reg, u8Val )                                                  \
    RIU_MACRO_START                                                                                     \
    PM_RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);            \
    RIU_MACRO_END

#define PM_RIU_Write2Byte( u32Reg, u16Val )                                               \
    RIU_MACRO_START                                                                                     \
    if ( ((u32Reg) & 0x01) )                                                                            \
    {                                                                                                                \
        PM_RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                     \
        PM_RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        PM_RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                                       \
    }                                                                               \
    RIU_MACRO_END

#define PM_RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                      \
    RIU_MACRO_START                                                                     \
    PM_RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (PM_RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_VIRT virtVIFBaseAddr;
    BOOL bBaseAddrInitialized;
} hal_VIF_t;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_VIF_t _hal_VIF ={BASEADDR_RIU, 0};
extern VIFInitialIn VIFInitialIn_inst;
//extern VIFSOS33 sVIFSOS33;
extern BOOL bEnableUsrSteadyAgcK;
extern U8 u8UsrSteadyAgcK;
extern BOOL bEnableUsrNonSteadyAgcK;
extern U8 u8UsrNonSteadyAgcK;
/////////////////////////////////////////////////////
BOOL AGC_Change_Index; //for Serious ACI Parameter
U16 SeriousACI_Index = 0;
////////////////////////////////////////////////////
BYTE g_ucVifStatusStep;
BOOL g_bCheckIFFreq;          // 0: 38.9 MHz (PAL/SECAM L); 1: 33.9 MHz (SECAM L')
BOOL g_VifHWKpKiFlag;
BYTE g_VifCrKp;
BYTE g_VifCrKi;
BYTE g_VifCrKpKiAdjLoopCnt;
BOOL g_bCheckModulationType;  // 0: negative; 1: positive
BYTE g_ucVifSoundSystemType;
////////////////////////////////////////////////////

#define VIF_IS_ADC_48MHz   1    // 0:144MHz ; 1:48MHz
#define VIF_IS_EQ_IIR           0   // 0:FIR ; 1:IIR

//-------------------------------------------------------------------------------------------------
//  Local code data
//-------------------------------------------------------------------------------------------------
MS_VIF_REG_TYPE VIF_GDE_PEAKING_1dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x08},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x70},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_2dB[]=
{
    {SOS21_C0_L,0xa4},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x8e},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x12},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x5c},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x60},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_3dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x1c},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x5d},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_4dB[]=
{
    {SOS21_C0_L,0xc1},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x87},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x28},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x3f},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0x51},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_3dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x07},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xd5},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_4dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x0a},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xd2},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_5dB_VSB[]=
{
    {SOS21_C0_L,0xf5},  // SOS21 peaking
    {SOS21_C0_H,0x02},
    {SOS21_C1_L,0x23},
    {SOS21_C1_H,0x06},
    {SOS21_C2_L,0x0e},
    {SOS21_C2_H,0x02},
    {SOS21_C3_L,0x0b},
    {SOS21_C3_H,0x05},
    {SOS21_C4_L,0xcf},
    {SOS21_C4_H,0x01},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_PEAKING_NULL[]=
{
    {SOS21_C0_L,0x00},  // SOS21
    {SOS21_C0_H,0x00},
    {SOS21_C1_L,0x00},
    {SOS21_C1_H,0x00},
    {SOS21_C2_L,0x00},
    {SOS21_C2_H,0x00},
    {SOS21_C3_L,0x00},
    {SOS21_C3_H,0x00},
    {SOS21_C4_L,0x00},
    {SOS21_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_YCDelay_VSB[]=
{
    {SOS22_C0_L,0x15},  // SOS22 Y/C delay
    {SOS22_C0_H,0x02},
    {SOS22_C1_L,0x84},
    {SOS22_C1_H,0x06},
    {SOS22_C2_L,0x7c},
    {SOS22_C2_H,0x01},
    {SOS22_C3_L,0xeb},
    {SOS22_C3_H,0x05},
    {SOS22_C4_L,0x00},
    {SOS22_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_YCDelay_NULL[]=
{
    {SOS22_C0_L,0x00},  // SOS22
    {SOS22_C0_H,0x00},
    {SOS22_C1_L,0x00},
    {SOS22_C1_H,0x00},
    {SOS22_C2_L,0x00},
    {SOS22_C2_H,0x00},
    {SOS22_C3_L,0x00},
    {SOS22_C3_H,0x00},
    {SOS22_C4_L,0x00},
    {SOS22_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_Low_R[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xb4},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf8},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x08},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x4c},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_Low_L[]=
{
    {SOS31_C0_L,0x3c},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0xb8},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x48},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0xc4},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xd9},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf7},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x0a},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x28},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_High_R[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xc7},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xd8},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x28},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x39},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_High_L[]=
{
    {SOS31_C0_L,0xcc},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x6c},
    {SOS31_C1_H,0x07},
    {SOS31_C2_L,0x94},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x34},
    {SOS31_C3_H,0x07},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xb0},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0x13},
    {SOS32_C1_H,0x07},
    {SOS32_C2_L,0xed},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x50},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_VSB_LG[]=
{
    {SOS31_C0_L,0xab},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0x9b},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x65},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0x55},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xe1},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0xf7},
    {SOS32_C1_H,0x06},
    {SOS32_C2_L,0x0a},
    {SOS32_C2_H,0x01},
    {SOS32_C3_L,0x1f},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_VSB_Philips[]=
{
    {SOS31_C0_L,0x9f},  // SOS31
    {SOS31_C0_H,0x02},
    {SOS31_C1_L,0xa8},
    {SOS31_C1_H,0x06},
    {SOS31_C2_L,0x58},
    {SOS31_C2_H,0x01},
    {SOS31_C3_L,0x62},
    {SOS31_C3_H,0x05},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0xcd},  // SOS32
    {SOS32_C0_H,0x02},
    {SOS32_C1_L,0x05},
    {SOS32_C1_H,0x07},
    {SOS32_C2_L,0xfb},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x33},
    {SOS32_C3_H,0x05},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_GDE_GroupDelay_NULL[]=
{
    {SOS31_C0_L,0x00},  // SOS31
    {SOS31_C0_H,0x00},
    {SOS31_C1_L,0x00},
    {SOS31_C1_H,0x00},
    {SOS31_C2_L,0x00},
    {SOS31_C2_H,0x00},
    {SOS31_C3_L,0x00},
    {SOS31_C3_H,0x00},
    {SOS31_C4_L,0x00},
    {SOS31_C4_H,0x02},
    {SOS32_C0_L,0x00},  // SOS32
    {SOS32_C0_H,0x00},
    {SOS32_C1_L,0x00},
    {SOS32_C1_H,0x00},
    {SOS32_C2_L,0x00},
    {SOS32_C2_H,0x00},
    {SOS32_C3_L,0x00},
    {SOS32_C3_H,0x00},
    {SOS32_C4_L,0x00},
    {SOS32_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_B_A2[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};
MS_VIF_REG_TYPE VIF_PAL_B_NICAM[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};
MS_VIF_REG_TYPE VIF_PAL_GH_A2[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};
MS_VIF_REG_TYPE VIF_PAL_GH_NICAM[]=
{
    {N_A1_C0_L,0x9f}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x37},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x9f}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x37},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};
MS_VIF_REG_TYPE VIF_SECAM_L_NICAM[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // SECAM L NICAM
};

MS_VIF_REG_TYPE VIF_PAL_I_NICAM[]=
{
    {N_A1_C0_L,0x6b}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0x6e},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x2e}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xaf},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x6b}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x6e},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL I NICAM
};

MS_VIF_REG_TYPE VIF_SECAM_DK1_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x4f}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x8c},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK1 A2
};

MS_VIF_REG_TYPE VIF_PAL_DK2_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x18}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xc6},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.96
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK2 A2
};

MS_VIF_REG_TYPE VIF_PAL_DK2_NICAM[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x7b}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x5d},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK NICAM
};

MS_VIF_REG_TYPE VIF_SECAM_DK3_A2[]=
{
    {N_A1_C0_L,0x34}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xa9},
    {N_A1_C2_H,0x05},
    {N_A2_C0_L,0x86}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0x51},
    {N_A2_C2_H,0x05},
    {SOS12_C0_L,0x34}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xa9},
    {SOS12_C3_H,0x05},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // PAL DK3 A2
};

MS_VIF_REG_TYPE VIF_GDE_BG_A2_NOTCH[]=
{
    {N_A3_C0_L,0x9f}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x37},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x86}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x51},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL B/G A2
};

MS_VIF_REG_TYPE VIF_GDE_BG_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x9f}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x37},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL B/G NICAM
};

MS_VIF_REG_TYPE VIF_GDE_I_NOTCH[]=
{
    {N_A3_C0_L,0x6b}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0x6e},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x2e}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xaf},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0xbb}, // Notch_A5 at 9.5MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x00},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0x40},
    {N_A5_C2_H,0x07},
    {_END_OF_TBL_,0x00}, // PAL I NICAM
};

MS_VIF_REG_TYPE VIF_GDE_DK1_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x4f}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x8c},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL DK1 A2
};

MS_VIF_REG_TYPE VIF_GDE_DK2_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x18}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xc6},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL DK2 A2
};

MS_VIF_REG_TYPE VIF_GDE_DK3_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x86}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x51},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL DK3 A2
};

MS_VIF_REG_TYPE VIF_GDE_DK_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // PAL DK NICAM
};

MS_VIF_REG_TYPE VIF_GDE_L_NICAM_NOTCH[]=
{
    {N_A3_C0_L,0x34}, // Notch_A3, R = 0.94
    {N_A3_C0_H,0x02},
    {N_A3_C1_L,0x3c},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xa9},
    {N_A3_C2_H,0x05},
    {N_A4_C0_L,0x7b}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0x5d},
    {N_A4_C2_H,0x05},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // SECAM L NICAM
};

MS_VIF_REG_TYPE VIF_GDE_MN_NOTCH[]=
{
    {N_A3_C0_L,0x1c}, // Notch_A3 (0.98)
    {N_A3_C0_H,0x03},
    {N_A3_C1_L,0x14},
    {N_A3_C1_H,0x06},
    {N_A3_C2_L,0xd4},
    {N_A3_C2_H,0x04},
    {N_A4_C0_L,0xe8}, // Notch_A4, R = 0.94
    {N_A4_C0_H,0x02},
    {N_A4_C1_L,0x3c},
    {N_A4_C1_H,0x06},
    {N_A4_C2_L,0xe8},
    {N_A4_C2_H,0x04},
    {N_A5_C0_L,0x02}, // Notch_A5 at 9MHz, image of audio 1 (bw 3.5, Apass 0.05)
    {N_A5_C0_H,0x01},
    {N_A5_C1_L,0x1c},
    {N_A5_C1_H,0x06},
    {N_A5_C2_L,0xf7},
    {N_A5_C2_H,0x06},
    {_END_OF_TBL_,0x00}, // NTSC M/N
};
MS_VIF_REG_TYPE VIF_NTSC_MN_A2[]=
{
    {N_A1_C0_L,0xfc}, // Notch_A1, R = 0.94
    {N_A1_C0_H,0x02},
    {N_A1_C1_L,0x3c},
    {N_A1_C1_H,0x06},
    {N_A1_C2_L,0xd4},
    {N_A1_C2_H,0x04},
    {N_A2_C0_L,0xe8}, // Notch_A2, R = 0.94
    {N_A2_C0_H,0x02},
    {N_A2_C1_L,0x3c},
    {N_A2_C1_H,0x06},
    {N_A2_C2_L,0xe8},
    {N_A2_C2_H,0x04},
    {SOS12_C0_L,0xfc}, // SOS12, R = 0.94
    {SOS12_C0_H,0x02},
    {SOS12_C1_L,0x3c},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xd4},
    {SOS12_C3_H,0x04},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00}, // NTSC M/N
};
MS_VIF_REG_TYPE VIF_PAL_DK_LOWER_ACI[]=
{
    {SOS11_C0_L,0x3a},  // SOS11 notch at 16.5MHz (0.94)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xf3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_I_LOWER_ACI[]=
{
    {SOS11_C0_L,0x0d},  // SOS11 notch at 17MHz (0.94)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0x23},
    {SOS11_C3_H,0x03},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_GH_LOWER_ACI[]=
{
    {SOS11_C0_L,0xe4},  // SOS11 notch at 17.5MHz (0.94)
    {SOS11_C0_H,0x04},
    {SOS11_C1_L,0x3c},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0x4f},
    {SOS11_C3_H,0x03},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_B_LOWER_ACI[]=
{
    {SOS11_C0_L,0x1C},  // SOS11 notch at 16.5MHz (0.98)
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x14},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xF3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};

MS_VIF_REG_TYPE VIF_NTSC_MN_LOWER_ACI[]=
{
    {SOS11_C0_L,0x15},  // SOS11 notch at 16.5MHz
    {SOS11_C0_H,0x05},
    {SOS11_C1_L,0x0A},
    {SOS11_C1_H,0x06},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x02},
    {SOS11_C3_L,0xF3},
    {SOS11_C3_H,0x02},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_DK_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0x0D},  // SOS12 notch at 8.5MHz
    {SOS12_C0_H,0x01},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xB0},
    {SOS12_C3_H,0x06},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_GH_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0x9A},  // SOS12 notch at 9.5MHz
    {SOS12_C0_H,0x00},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0x40},
    {SOS12_C3_H,0x07},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_PAL_I_Audio_SingleSAW[]=
{
    {SOS12_C0_L,0xD4},  // SOS12 notch at 9MHz
    {SOS12_C0_H,0x00},
    {SOS12_C1_L,0xB8},
    {SOS12_C1_H,0x06},
    {SOS12_C2_L,0x00},
    {SOS12_C2_H,0x02},
    {SOS12_C3_L,0xF7},
    {SOS12_C3_H,0x06},
    {SOS12_C4_L,0x00},
    {SOS12_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};

MS_VIF_REG_TYPE VIF_NULL_LOWER_ACI[]=
{
    {SOS11_C0_L,0x00},  // SOS11
    {SOS11_C0_H,0x00},
    {SOS11_C1_L,0x00},
    {SOS11_C1_H,0x00},
    {SOS11_C2_L,0x00},
    {SOS11_C2_H,0x00},
    {SOS11_C3_L,0x00},
    {SOS11_C3_H,0x00},
    {SOS11_C4_L,0x00},
    {SOS11_C4_H,0x02},
    {_END_OF_TBL_,0x00},
};

MS_VIF_REG_TYPE VIF_CR_IIR_LPF1[]=
{
    {CR_IIR_COEF_G,0x02},
    {CR_IIR_COEF_G+1,0x00},
    {CR_IIR_COEF_A1,0xfc},
    {CR_IIR_COEF_A1+1,0x01},
    {CR_IIR_COEF_A2,0x00},
    {CR_IIR_COEF_A2+1,0x00},
    {CR_IIR_COEF_B1,0x00},
    {CR_IIR_COEF_B1+1,0x02},
    {CR_IIR_COEF_B2,0x00},
    {CR_IIR_COEF_B2+1,0x00},
    {_END_OF_TBL_,0x00},
};
MS_VIF_REG_TYPE VIF_CR_IIR_LPF2[]=
{
    {CR_IIR_COEF_G,0x02},
    {CR_IIR_COEF_G+1,0x00},
    {CR_IIR_COEF_A1,0xd6},
    {CR_IIR_COEF_A1+1,0x03},
    {CR_IIR_COEF_A2,0x27},
    {CR_IIR_COEF_A2+1,0x06},
    {CR_IIR_COEF_B1,0x5b},
    {CR_IIR_COEF_B1+1,0x06},
    {CR_IIR_COEF_B2,0x00},
    {CR_IIR_COEF_B2+1,0x02},
    {_END_OF_TBL_,0x00},
};


U16 VIF_PAL_EQ_CO_A_REJ[]=
{
            0x0009,
            0x0003,
            0x000B,
            0x1FE1,
            0x002F,
            0x1FD0,
            0x001F,
            0x1FF2,
            0x001A,
            0x1FB4,
            0x008A,
            0x1F5E,
            0x0077,
            0x1FDC,
            0x1FFC,
            0x1FBB,
            0x00F1,
            0x1E79,
            0x0165,
            0x1FC9,
            0x1E4C,
            0x037C,
            0x0BCA,
            0x037C,
            0x1E4C,
            0x1FC9,
            0x0165,
            0x1E79,
            0x00F1,
            0x1FBB,
            0x1FFC,
            0x1FDC,
            0x0077,
            0x1F5E,
            0x008A,
            0x1FB4,
            0x001A,
            0x1FF2,
            0x001F,
            0x1FD0,
            0x002F,
            0x1FE1,
            0x000B,
            0x0003,
            0x0009,
            0x0000,

            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000
};

U16 VIF_NTSC_EQ_CO_A_REJ[]=
{
            0x001B,
            0x1FEB,
            0x0001,
            0x0000,
            0x0024,
            0x1FBF,
            0x0027,
            0x1FFE,
            0x0025,
            0x1F87,
            0x0083,
            0x1FD6,
            0x0009,
            0x1F78,
            0x0106,
            0x1F5A,
            0x1FD9,
            0x1FEF,
            0x017E,
            0x1DF8,
            0x1FB4,
            0x044E,
            0x09AF,
            0x044E,
            0x1FB4,
            0x1DF8,
            0x017E,
            0x1FEF,
            0x1FD9,
            0x1F5A,
            0x0106,
            0x1F78,
            0x0009,
            0x1FD6,
            0x0083,
            0x1F87,
            0x0025,
            0x1FFE,
            0x0027,
            0x1FBF,
            0x0024,
            0x0000,
            0x0001,
            0x1FEB,
            0x001B,
            0x0000,

            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000,
            0x0000
};

U16 VIF_BG_EQ_IIR_BANDSTOP[]=
{
            0x070B,
            0x0C15,
            0x0BBB,
            0x064E,
            0x070B,
            0x0B1A,
            0x070B,
            0x09F9,
            0x05F9,

            0x07AF,
            0x0BB0,
            0x0A66,
            0x0765,
            0x07AF,
            0x0C6B,
            0x07AF,
            0x0D1B,
            0x078D
};

U16 VIF_BG_NICAM_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0BCC,
            0x0AE6,
            0x06C8,
            0x0764,
            0x0C4A,
            0x0764,
            0x0C4A,
            0x06C8,

            0x0764,
            0x0BCC,
            0x0AE6,
            0x06C8,
            0x0764,
            0x0C4A,
            0x0764,
            0x0C4A,
            0x06C8
};

U16 VIF_I_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0DDB,
            0x0CCD,
            0x06C8,
            0x0764,
            0x0E30,
            0x0764,
            0x0E30,
            0x06C8,

            0x0764,
            0x0DDB,
            0x0CCD,
            0x06C8,
            0x0764,
            0x0E30,
            0x0764,
            0x0E30,
            0x06C8
};

U16 VIF_DK1_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0D87,
            0x0764,
            0x0D87,
            0x06C8,

            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0D87,
            0x0764,
            0x0D87,
            0x06C8
};

U16 VIF_DK2_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0E7F,
            0x0764,
            0x0E7F,
            0x06C8,

            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0E7F,
            0x0764,
            0x0E7F,
            0x06C8
};

U16 VIF_DK3_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0BEB,
            0x0764,
            0x0BEB,
            0x06C8,

            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0BEB,
            0x0764,
            0x0BEB,
            0x06C8
};

U16 VIF_DK_NICAM_EQ_IIR_NOTCH[]=
{
            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0C4A,
            0x0764,
            0x0C4A,
            0x06C8,

            0x0764,
            0x0F41,
            0x0E18,
            0x06C8,
            0x0764,
            0x0C4A,
            0x0764,
            0x0C4A,
            0x06C8
};

U16 VIF_NTSC_MN_EQ_IIR_NOTCH[]=
{
            0x079E,
            0x0620,
            0x05D5,
            0x073C,
            0x079E,
            0x072F,
            0x079E,
            0x072F,
            0x073C,

            0x079E,
            0x0620,
            0x05D5,
            0x073C,
            0x079E,
            0x072F,
            0x079E,
            0x072F,
            0x073C
};


void msWriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    RIU_WriteByteMask(u32Reg, u8Val, u8Mask);
    msVifLoad();
}

void msWriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    RIU_WriteRegBit(u32Reg, bEnable, u8Mask);
    msVifLoad();
}

void msWriteByte(U32 u32Reg, U8 u8Val )
{
    RIU_WriteByte(u32Reg,u8Val);
    msVifLoad();
}

void HAL_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    msWriteByteMask(u32Reg, u8Val, u8Mask);
}

void HAL_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    msWriteBit(u32Reg, bEnable, u8Mask);
}

void HAL_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    msWriteByte(u32Reg, u8Val);
}

U8 HAL_VIF_ReadByte(U32 u32Reg )
{
    return msReadByte(u32Reg);
}

void msWriteRegsTbl(MS_VIF_REG_TYPE *pRegTable)
{
    U16 u16Dummy;
    U32 u32Address;
    U8  u8Value;

    u16Dummy = 2000;

    do
    {
        u32Address = pRegTable->u32Address;
        u8Value = pRegTable->u8Value;
        if (u32Address == 0xFFFF)       // check end of table
            break;
        RIU_WriteByte(u32Address, u8Value);
        pRegTable++;
    } while (--u16Dummy > 0);
    msVifLoad();
}

void HAL_VIF_RegInit (void)
{
    MS_PHY phyNonPMBankSize, phyPMBankSize;
    MS_VIRT  virtNonPMBank, virtPMBank;

    HALVIFDBG(printf("\r\nHAL_VIF_RegInit()"));
    if (!MDrv_MMIO_GetBASE( &virtNonPMBank, &phyNonPMBankSize, MS_MODULE_VIF))
    {
        printf("\r\nIOMap failure to get MAP_NONPM_BANK");
        virtNonPMBank = BASEADDR_RIU; // TODO what to do if failed??
    }
    else
    {
        HALVIFDBG(printf("\r\nMS_MODULE_VIF base = 0x%lX, length = %lu", virtNonPMBank, phyNonPMBankSize));
    }

    _hal_VIF.virtVIFBaseAddr = virtNonPMBank;
    _hal_VIF.bBaseAddrInitialized = 1;

    if (!MDrv_MMIO_GetBASE( &virtPMBank, &phyPMBankSize, MS_MODULE_PM))
    {
        printf("\r\nIOMap failure to get MAP_PM_BANK");
        virtPMBank = (VIRT)BASEADDR_RIU; // TODO what to do if failed??
    }
    else
    {
        HALVIFDBG(printf("\r\nMS_MODULE_PM base = 0x%lX, length = %lu", virtPMBank, phyPMBankSize));
    }
}


void HAL_VIF_SetClock(BOOL bEnable)
{
    HALVIFDBG(printf("\r\nHAL_VIF_SetClock=%d",bEnable));

    //bEnable = (bEnable) ? 0:1; // 0 means enable
}

void msVifAdcInitial(void)
{
    HALVIFDBG(printf("\r\nmsVifAdcInitial()"));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    RIU_WriteByteMask(0x1E39L, 0x00, 0x03);  // DMDTOP/DMDANA_controlled by HK_MCU (0) or DMD_MCU (1)

    // enable vif DAC clock
    // enable atsc_adcd_sync clock
    RIU_WriteByte(0x331AL, 0x04);
    RIU_WriteByte(0x331BL, 0x00);

    // enable vif clock
    RIU_WriteByte(0x11F1CL, 0x00);
    RIU_WriteByte(0x11F1DL, 0x00);

    // SRAM share clock
    RIU_WriteByte(0x11FE2L, 0x01);  // ckg_dvbtm_sram_t14x_t24x_srd1x
    RIU_WriteByte(0x11FE3L, 0x10);
    RIU_WriteByte(0x11F78L, 0x00);  // ckg_dvbtm_sram_t14x_t24x_srd1x_vifssc_isdbt_inner4x
    RIU_WriteByte(0x11F79L, 0x00);

    // ACIFIR clock
    RIU_WriteByte(0x11F24L, 0x01);
    RIU_WriteByte(0x11F25L, 0x08);

    RIU_WriteByte(0x11F2AL, 0x00);   // DAGC1/2 SRAM MUX
    RIU_WriteByte(0x11F2BL, 0x00);

    RIU_WriteByte(0x52990L, 0x00);  // ckg_dvbtm_sram_t14x_t24x_t24x_srd1x_vifssc_isdbt_inner4x_dtmb_inner12x
    RIU_WriteByte(0x52991L, 0x01);  // ckg_sram_t14x_t24x_s2inn_isdbt_inn2x_dtmb_eq2x

    RIU_WriteByte(0x127C0L, 0x00);  // mulan_dvbt2_sram_share_en
    RIU_WriteByte(0x127C1L, 0x00);
    RIU_WriteByte(0x127C2L, 0x00);
    RIU_WriteByte(0x127C3L, 0x00);
    RIU_WriteByte(0x127C4L, 0x00);
    RIU_WriteByte(0x127C5L, 0x00);
    RIU_WriteByte(0x127C6L, 0x00);
    RIU_WriteByte(0x127C7L, 0x00);
    RIU_WriteByte(0x127C8L, 0x00);
    RIU_WriteByte(0x127C9L, 0x00);
    
    // SRAM Power Control
    RIU_WriteByte(0x12090L, 0xB8);
    RIU_WriteByte(0x12091L, 0xBB);
    RIU_WriteByte(0x127E0L, 0x70);   // SRAM_PWR_CTRL_SEL
    RIU_WriteByte(0x127E1L, 0x00);
    RIU_WriteByte(0x127E2L, 0x00);
    RIU_WriteByte(0x127E3L, 0x00);
    RIU_WriteByte(0x127E4L, 0x00);
    RIU_WriteByte(0x127E5L, 0x00);
    RIU_WriteByte(0x127E6L, 0x00);
    RIU_WriteByte(0x127E7L, 0x00);
    RIU_WriteByte(0x127E8L, 0x00);
    RIU_WriteByte(0x127E9L, 0x00);

    // Enable VIF, DVBT, ATSC and VIF reset
    RIU_WriteByte(0x12002L, 0x74);
    RIU_WriteByte(0x12003L, 0x00);
    RIU_WriteByte(0x12002L, 0x14);  // Enable VIF
    RIU_WriteByte(0x12003L, 0x00);
    RIU_WriteByte(0x12004L, 0x22);
    RIU_WriteByte(0x12005L, 0x00);

    // DMD_ANA_ADC_SYNC CLK_W 
    // [0]  : disable clock = 1'b1 
    // [1]  : invert clock
    RIU_WriteByte(0x128D0L, 0x01);
    
    // Disable ADC sign bit
    RIU_WriteByte(0x12060L, 0x04);
    RIU_WriteByte(0x12061L, 0x00);

    // ADC I channel offset
    // Change unsin into sin
    // [11:0] reg_adc_offset_i
    RIU_WriteByte(0x12064L, 0x00);
    RIU_WriteByte(0x12065L, 0x00);

    // ADC Q channel offset
    // Change unsin into sin
    // [11:0] reg_adc_offset_q
    RIU_WriteByte(0x12066L, 0x00);
    RIU_WriteByte(0x12067L, 0x00);

    // VIF use DVB SRAM and FIR
    RIU_WriteByte(0x120A0L, 0x01);

    // Enable LDOS
    RIU_WriteByte(0x128AEL, 0x00);
    RIU_WriteByte(0x128AFL, 0x00);
    RIU_WriteByte(0x128B0L, 0x00);
    RIU_WriteByte(0x128B1L, 0x00);
    RIU_WriteByte(0x128B2L, 0x11);   // ana_setting_sel
    RIU_WriteByte(0x128B3L, 0x00);

    // PLL power up
    RIU_WriteByte(0x1286AL, 0x03);
    RIU_WriteByte(0x1286BL, 0x00);

    // VIF : 24*36/6  = 144 MHz
    // VIF : 24*36/18 =  48 MHz
    // Set MPLL_LOOP_DIV_FIRST and SECOND
    RIU_WriteByte(0x12866L, 0x01);  // loop divider
    RIU_WriteByte(0x12867L, 0x12);

    // Set MPLL_ADC_DIV_SE
    if(VIF_IS_ADC_48MHz == 0)
    {
        // 144MHz case
        RIU_WriteByte(0x12860L, 0x00);     // DMPLL post divider
        RIU_WriteByte(0x12861L, 0x06);
    }
    else
    {
        // 48MHz case
        RIU_WriteByte(0x12860L, 0x00);    // DMPLL post divider
        RIU_WriteByte(0x12861L, 0x12);
    }

    // MPLL_output_div_second
    RIU_WriteByte(0x12864L, 0x00);
    RIU_WriteByte(0x12865L, 0x00);

    // SIF CLK select
    RIU_WriteByte(0x1287EL, 0x00);
    RIU_WriteByte(0x1287FL, 0x00);

    RIU_WriteRegBit(0x12834L, 1, _BIT4);          // EN_VCO_DIG

    // Set IMUXS QMUXS
    RIU_WriteByte(0x12802L, 0x40);                  // VIF path, Bypass PGA
    RIU_WriteByte(0x12803L, 0x04);                  // Mux selection

    // Set enable ADC clock
    RIU_WriteByte(0x12818L, 0x02);                  // ADC_Q power down 
    RIU_WriteByte(0x12819L, 0x00);

    RIU_WriteByte(0x12840L, 0x00);                 // Ref Enable
    RIU_WriteByte(0x12841L, 0x00);
    
    // Set ADC gain is 1
    RIU_WriteByte(0x12816L, 0x05);
    RIU_WriteByte(0x12817L, 0x05);

    // Calibration buffer
    RIU_WriteByte(0x1281EL, 0x80);
    RIU_WriteByte(0x1281FL, 0x00);

    // AGC control
    RIU_WriteByte(0x12830L, 0x01);                 // AGC enable
    RIU_WriteByte(0x12831L, 0x00); 

    if(VIFInitialIn_inst.VifSawArch == NO_SAW_DIF)
        VIFInitialIn_inst.VifSawArch = NO_SAW;
    
    HAL_VIF_Delay1ms(1);

    // RFAGC and IFAGC control (ADC)
    RIU_WriteByteMask(RFAGC_DATA_SEL, 0, 0x0C);		// RFAGC
    RIU_WriteByteMask(IFAGC_DATA_SEL, 0, 0xC0);		// IFAGC
    RIU_WriteRegBit(RFAGC_ODMODE, 0, _BIT1);
    RIU_WriteRegBit(IFAGC_ODMODE, 0, _BIT5);
    
    if ((VIFInitialIn_inst.VifSawArch == SILICON_TUNER) || (VIFInitialIn_inst.VifSawArch == NO_SAW) ||(VIFInitialIn_inst.VifSawArch == SAVE_PIN_VIF))
        RIU_WriteRegBit(RFAGC_ENABLE, 0, _BIT0);         // RFAGC disable
    else
        RIU_WriteRegBit(RFAGC_ENABLE, 1, _BIT0);         // RFAGC enable
    RIU_WriteRegBit(IFAGC_ENABLE, 1, _BIT4);		 // IFAGC enable

    // RFAGC and IFAGC control (RF)
    msWriteBit(RFAGC_SEL_SECONDER, 1, _BIT6);	     // 0: 1st order; 1: 2nd order
    msWriteBit(RFAGC_DITHER_EN, 1, _BIT7);	            // dither disable
    msWriteBit(RFAGC_POLARITY, 1, _BIT4);                // RFAGC polarity 0: negative logic
    msWriteBit(OREN_RFAGC, 0, _BIT5);		            // RFAGC 0: BB control; 1: I2C control
    
    msWriteBit(IFAGC_SEL_SECONDER, 1, _BIT6);	     // 0: 1st order; 1: 2nd order
    msWriteBit(IFAGC_DITHER_EN, 1, _BIT7);	            // dither disable
    msWriteBit(IFAGC_POLARITY, 1, _BIT4);                 // RFAGC polarity 0: negative logic
    msWriteBit(OREN_IFAGC, 0, _BIT6);		            // RFAGC 0: BB control; 1: I2C control

    msWriteBit(OREN_PGA1_V, 0, _BIT3);                  // Video PGA1 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA2_V, 0, _BIT2);                  // Video PGA2 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA1_S, 0, _BIT1);                  // Audio PGA1 0: BB control; 1: I2C control
    msWriteBit(OREN_PGA2_S, 0, _BIT0);                  // Audio PGA2 0: BB control; 1: I2C control

    // ADC_SYNC CLK_R
    RIU_WriteByte(0x11F0AL, 0x00);
    RIU_WriteByte(0x11F0BL, 0x00);

    // ADC_SYNC CLK_W / DMD_ANA_ADC_SYNC CLK_R
    // enable dvbc adc clock
    RIU_WriteByte(0x3314L, 0x04);
    RIU_WriteByte(0x3315L, 0x00);

    // DMD_ANA_ADC_SYNC CLK_W
    RIU_WriteByte(0x128D0L, 0x00);
    
    // EQ BYPASS
    msWriteBit(BYPASS_EQFIR, 1, _BIT0);
}

// For API
void msVifSetIfFreq(IfFrequencyType ucIfFreq)
{
    HALVIFDBG(printf("\r\nmsVifSetIfFreq() ucIfFreq=%d",ucIfFreq));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

     //g_FreqType = ucIfFreq; // 0x1121_D2
     msWriteByte(VIF_RF_RESERVED_1, ucIfFreq); 

     // VifShiftClk : 0x1121_D3
     BYTE VifShiftClk = msReadByte(VIF_RF_RESERVED_1+1);

    // cvbs output
    msWriteBit(VIFDAC_ENABLE, 1, _BIT3);                // enable vifdac

    // for China descrambler box
    if (VIFInitialIn_inst.ChinaDescramblerBox == 0)
    {
        msWriteBit(N_A1_IN_SEL, 0, _BIT4);                         // 0:from dvga, 1:from image_rej_iir
        msWriteByteMask(VIFDAC_OUT_SEL, 0x00, 0x07);    // 0: cvbs output; 4: debug bus
    }
    else
    {
        msWriteByteMask(VIFDAC_OUT_SEL, 0x04, 0x07);    // 0: cvbs output; 4: debug bus
        msWriteBit(DEBUG2_EN, 1, _BIT7);                // select debug2 data
        msWriteByteMask(DEBUG_MODULE, 0x00, 0x0F);      // select filter debug bus

        if(VIFInitialIn_inst.ChinaDescramblerBox == 1)
        {
           msWriteBit(N_A1_IN_SEL, 1, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
           msWriteByte(DEBUG_PORT, 0x84);                                     // selsect CVBS output after Notch_A2 filter   
        }
        else if(VIFInitialIn_inst.ChinaDescramblerBox == 2)
        {
           msWriteBit(N_A1_IN_SEL, 0, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
           msWriteByte(DEBUG_PORT, 0x98);                                     // select CVBS output after IMAGE_IIR
        }
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 3)
        {
            msWriteBit(N_A1_IN_SEL, 0, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
            msWriteByte(DEBUG_PORT, 0x8A);                                     // select CVBS output after IMAGE_REJ1
        }
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 4)
        {
            msWriteBit(N_A1_IN_SEL, 0, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
            msWriteByte(DEBUG_PORT, 0x88);                                     // select CVBS output after ACI_REJ
        }
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 5)
        {
            msWriteBit(N_A1_IN_SEL, 0, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
            msWriteByte(DEBUG_PORT, 0x86);                                     // select CVBS output after MIXER_OUT_I
        }
        else if (VIFInitialIn_inst.ChinaDescramblerBox == 6)
        {
            msWriteBit(N_A1_IN_SEL, 0, _BIT4);                                   // 0:from dvga, 1:from image_rej_iir
            msWriteByte(DEBUG_PORT, 0x8B);                                     // select CVBS output after IMAGE_REJ2
        }
        else
        {
            msWriteByteMask(VIFDAC_OUT_SEL, 0x00, 0x07);            // 0: cvbs output; 4: debug bus
            msWriteBit(DEBUG2_EN, 0, _BIT7);                                   // select debug2 data
            msWriteByteMask(DEBUG_MODULE, 0x00, 0x0F);             // select filter debug bus
            msWriteBit(N_A1_IN_SEL, 1, _BIT4);                                // 0:from dvga, 1:from image_rej_iir
        }
     }

    if (VIFInitialIn_inst.VifTunerType == 1)
    {
        // silicon tuner
        msWriteByte(IF_RATE, 0x00);                         // IF rate for 0 MHz
        msWriteByte(IF_RATE+1, 0x00);
        msWriteByteMask(IF_RATE+2, 0x00, 0x3F);
    }
    else if (VIFInitialIn_inst.VifTunerType == 2)
    {
        // FM tuner
        if(VIFInitialIn_inst.VifSawArch == SILICON_TUNER)
        {
           // silicon tuner
           msWriteByte(IF_RATE, 0x00);                         // IF rate for 0 MHz
           msWriteByte(IF_RATE+1, 0x00);
           msWriteByteMask(IF_RATE+2, 0x00, 0x3F);
        }
    }
    else
    {
        switch(ucIfFreq)
        {
            case IF_FREQ_3395:
                msWriteByte(IF_RATE, 0x44);                 // IF rate for -48.9 MHz  // HEX [ (IF/144) * 2^22]
                msWriteByte(IF_RATE+1, 0x44);
                msWriteByteMask(IF_RATE+2, 0x2A, 0x3F);
                break;
            case IF_FREQ_3800:
            	if (VifShiftClk/*g_VifShiftClk*/ == 1)
            	{
                    msWriteByte(0x12866L, 0x00);//loop divider
                    msWriteByte(0x12867L, 0x23);            	  	
                    
                    msWriteByte(IF_RATE, 0xA8);                 // IF rate for 23 MHz
                    msWriteByte(IF_RATE+1, 0x83);
                    msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);            	  	
            	}
                else if(VifShiftClk/*g_VifShiftClk*/ == 2)
                {
                    msWriteByte(0x12866L, 0x00);//loop divider
                    msWriteByte(0x12867L, 0x25);            	  	
                    
                    msWriteByte(IF_RATE, 0x29);                 // IF rate for 23 MHz
                    msWriteByte(IF_RATE+1, 0xF2);
                    msWriteByteMask(IF_RATE+2, 0x09, 0x3F); 
                }
            	else
            	{
                    msWriteByte(0x12866L, 0x00);//loop divider
                    msWriteByte(0x12867L, 0x24);     
                                	  	
                    msWriteByte(IF_RATE, 0xE3);                 // IF rate for 23 MHz
                    msWriteByte(IF_RATE+1, 0x38);
                    msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                }
                break;
            case IF_FREQ_3890:
                msWriteByte(IF_RATE, 0x49);                 // IF rate for 23.9 MHz
                msWriteByte(IF_RATE+1, 0x9F);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                break;
            case IF_FREQ_3950:
                msWriteByte(IF_RATE, 0x8E);                 // IF rate for 24.5 MHz
                msWriteByte(IF_RATE+1, 0xE3);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
                break;
            case IF_FREQ_4575:
                msWriteByte(IF_RATE, 0xAA);                 // IF rate for 30.75 MHz
                msWriteByte(IF_RATE+1, 0xAA);
                msWriteByteMask(IF_RATE+2, 0x0D, 0x3F);
                break;
            case IF_FREQ_5875:
                msWriteByte(IF_RATE, 0xC7);                 // IF rate for 43.75 MHz
                msWriteByte(IF_RATE+1, 0x71);
                msWriteByteMask(IF_RATE+2, 0x13, 0x3F);
                break;
            default:
                break;
        }
    }
}

void msVifGroupDelayFilter(VIFSoundSystem ucSoundSystem, FrequencyBand frequencyRange)
{
    BYTE VifPeakingFilter=0, VifYcDelayFilter=0, VifGroupDelayFilter=0;

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    switch (ucSoundSystem)
    {
        case VIF_SOUND_B:
        case VIF_SOUND_B_NICAM:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterB_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterB_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterB_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter B frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_GH:
        case VIF_SOUND_GH_NICAM:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterGH_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterGH_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterGH_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter GH frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_I:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
{
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterI_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterI_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterI_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter I frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_DK1:
        case VIF_SOUND_DK2:
        case VIF_SOUND_DK3:
        case VIF_SOUND_DK_NICAM:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterDK_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterDK_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterDK_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter DK frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_L:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterL_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterL_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterL_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter L frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_LL:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterLL_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterLL_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterLL_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter LL frequencyRange=%d", frequencyRange);
            break;

        case VIF_SOUND_MN:
            if (frequencyRange==FREQ_VHF_L)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_VHF_L;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L;
            }
            else if (frequencyRange==FREQ_VHF_H)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_VHF_H;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H;
            }
            else if (frequencyRange==FREQ_UHF)
            {
                VifPeakingFilter = VIFInitialIn_inst.VifPeakingFilterMN_UHF;
                VifYcDelayFilter = VIFInitialIn_inst.VifYcDelayFilterMN_UHF;
                VifGroupDelayFilter =VIFInitialIn_inst.VifGroupDelayFilterMN_UHF;
            }
            else
                printf("\r\n ERROR msVifGroupDelayFilter MN frequencyRange=%d", frequencyRange);
            break;

        default:
            printf("\r\n Error msVifGroupDelayFilter ucSoundSystem=%d",ucSoundSystem);
            break;
    }

    HALVIFDBG(printf("\r\nmsVifGroupDelayFilter() VifPeakingFilter=%d",VifPeakingFilter));
    HALVIFDBG(printf(" VifYcDelayFilter=%d VifGroupDelayFilter=%d",VifYcDelayFilter, VifGroupDelayFilter));

    if (VifPeakingFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_NULL);
    }
    else if (VifPeakingFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_1dB);
    }
    else if (VifPeakingFilter == 0x02)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_2dB);
    }
    else if (VifPeakingFilter == 0x03)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_3dB);
    }
    else if (VifPeakingFilter == 0x04)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_4dB);
    }
    else if (VifPeakingFilter == 0x05)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_3dB_VSB);
    }
    else if (VifPeakingFilter == 0x06)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_4dB_VSB);
    }
    else if (VifPeakingFilter == 0x07)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_PEAKING_5dB_VSB);
    }
    else if (VifPeakingFilter == 0x80)
    {
        msWriteByte(SOS21_C0_L, VIFInitialIn_inst.VifSos21FilterC0);    // SOS21 (user define)
        msWriteByteMask(SOS21_C0_H, VIFInitialIn_inst.VifSos21FilterC0>>8, 0x07);
        msWriteByte(SOS21_C1_L, VIFInitialIn_inst.VifSos21FilterC1);
        msWriteByteMask(SOS21_C1_H, VIFInitialIn_inst.VifSos21FilterC1>>8, 0x07);
        msWriteByte(SOS21_C2_L, VIFInitialIn_inst.VifSos21FilterC2);
        msWriteByteMask(SOS21_C2_H, VIFInitialIn_inst.VifSos21FilterC2>>8, 0x07);
        msWriteByte(SOS21_C3_L, VIFInitialIn_inst.VifSos21FilterC3);
        msWriteByteMask(SOS21_C3_H, VIFInitialIn_inst.VifSos21FilterC3>>8, 0x07);
        msWriteByte(SOS21_C4_L, VIFInitialIn_inst.VifSos21FilterC4);
        msWriteByteMask(SOS21_C4_H, VIFInitialIn_inst.VifSos21FilterC4>>8, 0x07);
    }

    if (VifYcDelayFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_YCDelay_NULL);
    }
    else if (VifYcDelayFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_YCDelay_VSB);
    }
    else if (VifYcDelayFilter == 0x80)
    {
        msWriteByte(SOS22_C0_L, VIFInitialIn_inst.VifSos22FilterC0);    // SOS22 (user define)
        msWriteByteMask(SOS22_C0_H, VIFInitialIn_inst.VifSos22FilterC0>>8, 0x07);
        msWriteByte(SOS22_C1_L, VIFInitialIn_inst.VifSos22FilterC1);
        msWriteByteMask(SOS22_C1_H, VIFInitialIn_inst.VifSos22FilterC1>>8, 0x07);
        msWriteByte(SOS22_C2_L, VIFInitialIn_inst.VifSos22FilterC2);
        msWriteByteMask(SOS22_C2_H, VIFInitialIn_inst.VifSos22FilterC2>>8, 0x07);
        msWriteByte(SOS22_C3_L, VIFInitialIn_inst.VifSos22FilterC3);
        msWriteByteMask(SOS22_C3_H, VIFInitialIn_inst.VifSos22FilterC3>>8, 0x07);
        msWriteByte(SOS22_C4_L, VIFInitialIn_inst.VifSos22FilterC4);
        msWriteByteMask(SOS22_C4_H, VIFInitialIn_inst.VifSos22FilterC4>>8, 0x07);
    }

    if (VifGroupDelayFilter == 0x00)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_NULL);
    }
    else if (VifGroupDelayFilter == 0x01)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_VSB_LG);
    }
    else if (VifGroupDelayFilter == 0x02)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_VSB_Philips);
    }
    else if (VifGroupDelayFilter == 0x03)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_Low_R);
    }
    else if (VifGroupDelayFilter == 0x04)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_Low_L);
    }
    else if (VifGroupDelayFilter == 0x05)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_High_R);
    }
    else if (VifGroupDelayFilter == 0x06)
    {
        msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_GroupDelay_High_L);
    }
    else if (VifGroupDelayFilter == 0x80)
    {
        msWriteByte(SOS31_C0_L, VIFInitialIn_inst.VifSos31FilterC0);    // SOS31 (user define)
        msWriteByteMask(SOS31_C0_H, VIFInitialIn_inst.VifSos31FilterC0>>8, 0x07);
        msWriteByte(SOS31_C1_L, VIFInitialIn_inst.VifSos31FilterC1);
        msWriteByteMask(SOS31_C1_H, VIFInitialIn_inst.VifSos31FilterC1>>8, 0x07);
        msWriteByte(SOS31_C2_L, VIFInitialIn_inst.VifSos31FilterC2);
        msWriteByteMask(SOS31_C2_H, VIFInitialIn_inst.VifSos31FilterC2>>8, 0x07);
        msWriteByte(SOS31_C3_L, VIFInitialIn_inst.VifSos31FilterC3);
        msWriteByteMask(SOS31_C3_H, VIFInitialIn_inst.VifSos31FilterC3>>8, 0x07);
        msWriteByte(SOS31_C4_L, VIFInitialIn_inst.VifSos31FilterC4);
        msWriteByteMask(SOS31_C4_H, VIFInitialIn_inst.VifSos31FilterC4>>8, 0x07);
        msWriteByte(SOS32_C0_L, VIFInitialIn_inst.VifSos32FilterC0);    // SOS32 (user define)
        msWriteByteMask(SOS32_C0_H, VIFInitialIn_inst.VifSos32FilterC0>>8, 0x07);
        msWriteByte(SOS32_C1_L, VIFInitialIn_inst.VifSos32FilterC1);
        msWriteByteMask(SOS32_C1_H, VIFInitialIn_inst.VifSos32FilterC1>>8, 0x07);
        msWriteByte(SOS32_C2_L, VIFInitialIn_inst.VifSos32FilterC2);
        msWriteByteMask(SOS32_C2_H, VIFInitialIn_inst.VifSos32FilterC2>>8, 0x07);
        msWriteByte(SOS32_C3_L, VIFInitialIn_inst.VifSos32FilterC3);
        msWriteByteMask(SOS32_C3_H, VIFInitialIn_inst.VifSos32FilterC3>>8, 0x07);
        msWriteByte(SOS32_C4_L, VIFInitialIn_inst.VifSos32FilterC4);
        msWriteByteMask(SOS32_C4_H, VIFInitialIn_inst.VifSos32FilterC4>>8, 0x07);
    /*    
        msWriteByte(SOS33_C0_L, sVIFSOS33.Vif_SOS_33_C0);    // SOS33 (user define)
        msWriteByteMask(SOS33_C0_H, sVIFSOS33.Vif_SOS_33_C0>>8, 0x07);
        msWriteByte(SOS33_C1_L, sVIFSOS33.Vif_SOS_33_C1);
        msWriteByteMask(SOS33_C1_H, sVIFSOS33.Vif_SOS_33_C1>>8, 0x07);
        msWriteByte(SOS33_C2_L, sVIFSOS33.Vif_SOS_33_C2);
        msWriteByteMask(SOS33_C2_H, sVIFSOS33.Vif_SOS_33_C2>>8, 0x07);
        msWriteByte(SOS33_C3_L, sVIFSOS33.Vif_SOS_33_C3);
        msWriteByteMask(SOS33_C3_H, sVIFSOS33.Vif_SOS_33_C3>>8, 0x07);
        msWriteByte(SOS33_C4_L, sVIFSOS33.Vif_SOS_33_C4);
        msWriteByteMask(SOS33_C4_H, sVIFSOS33.Vif_SOS_33_C4>>8, 0x07);
    */
    }
    msWriteBit(BYPASS_SOS33, 1, _BIT6);
}

// For API
void msVifSetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    HALVIFDBG(printf("\r\nmsVifSetSoundSystem() ucSoundSystem=%d",ucSoundSystem));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    DWORD VifCrRateTemp;
    
     // VifShiftClk : 0x1121_D3
     BYTE VifShiftClk = msReadByte(VIF_RF_RESERVED_1+1);
    
    g_ucVifSoundSystemType = ucSoundSystem;

    if((ucSoundSystem == VIF_SOUND_L)||(ucSoundSystem == VIF_SOUND_LL))
        msVifInitial();

    switch(ucSoundSystem)
    {
        case VIF_SOUND_B:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_B & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_B, _BIT0);                                                      // cr_rate not invert

            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_A2);
            msVifGroupDelayFilter(VIF_SOUND_B, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_A2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass 
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_B_NICAM:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_B;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_B & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_B>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_B, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_B_NICAM, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_B_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
             //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_GH:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
                // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_GH & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_GH, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_A2);
            msVifGroupDelayFilter(VIF_SOUND_GH, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_A2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_GH_NICAM:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_GH;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_GH & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_GH>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_GH, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_GH_NICAM, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_BG_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_GH_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 0, _BIT1);                  // A_LPF_BG not bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_I:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_I;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_I;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_I & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_I>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_I>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_I, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_I_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_I, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_I_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_I_LOWER_ACI);  //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_DK1:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_DK1_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK1, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK1_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_DK2:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK2_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK2, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK2_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);  //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
	        //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_DK_NICAM:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK2_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_DK_NICAM, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);  //Notch N-1 Audio Carrier
            msWriteBit(BYPASS_ACI_REJ_NTSC, 1, _BIT6);       // bypass ACI_REJ_NTSC_filter
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_DK3:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_DK;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_DK & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_DK>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_DK, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_DK3_A2);
            msVifGroupDelayFilter(VIF_SOUND_DK3, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_DK3_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_L:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_L;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_L;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_L & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_L>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_L>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_L, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_L_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_L, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_L_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_LL:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_LL;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_LL;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_LL & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_LL>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_LL>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_LL, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_SECAM_L_NICAM);
            msVifGroupDelayFilter(VIF_SOUND_LL, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_L_NICAM_NOTCH);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_PAL_DK_LOWER_ACI);//Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 0, _BIT6);                     // A_LPF_BG_SEL = 0 (PAL)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);             // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                         // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for Non-NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 1 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 1 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        case VIF_SOUND_MN:
            if (VIFInitialIn_inst.VifTunerType == 1)
            {
               // silicon tuner
                if((VifShiftClk/*g_VifShiftClk*/ == 1)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_MN;
                    VifCrRateTemp = (VifCrRateTemp *2107)>>11; // 43.2/42 = 1.02857 ~= 1.02881
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else if((VifShiftClk/*g_VifShiftClk*/ == 2)&&(VIF_IS_ADC_48MHz == 0))
                {
                    VifCrRateTemp = VIFInitialIn_inst.VifCrRate_MN;
                    VifCrRateTemp = (VifCrRateTemp *1993)>>11; // 43.2/44.4 = 0.97297 ~= 0.97314
                    msWriteByte(CR_RATE, (BYTE) (VifCrRateTemp & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VifCrRateTemp>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VifCrRateTemp>>16) & 0x000000FF), 0x0F);
                }
                else
                {
                    msWriteByte(CR_RATE, (BYTE) (VIFInitialIn_inst.VifCrRate_MN & 0x000000FF));                                     // cr_rate for 6.4 MHz
                    msWriteByte(CR_RATE+1, (BYTE) ((VIFInitialIn_inst.VifCrRate_MN>>8) & 0x000000FF));
                    msWriteByteMask(CR_RATE+2, (BYTE) ((VIFInitialIn_inst.VifCrRate_MN>>16) & 0x000000FF), 0x0F);
                }
                msWriteBit(CR_RATE_INV, VIFInitialIn_inst.VifCrInvert_MN, _BIT0);                                                      // cr_rate not invert
            }

            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_NTSC_MN_A2);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_GDE_MN_NOTCH);
            msVifGroupDelayFilter(VIF_SOUND_MN, (FrequencyBand)VIFInitialIn_inst.VifFreqBand);
            msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_NTSC_MN_LOWER_ACI); //Notch N-1 Audio Carrier
            msWriteBit(A_LPF_BG_SEL, 1, _BIT6);                     // A_LPF_BG_SEL = 1 (NTSC)
            msWriteBit(BYPASS_CO_A_REJ_NTSC, 1, _BIT5);        // CO_A_REJ_NTSC bypass
            msWriteBit(BYPASS_A_LPF_BG, 1, _BIT1);                  // A_LPF_BG bypass
            msWriteBit(BYPASS_N_A3, 0, _BIT5);                      // A3 notch not bypass
            msWriteBit(BYPASS_N_A4, 0, _BIT6);                        // A4 notch not bypass
            msWriteBit(BYPASS_N_A5, 0, _BIT3);                        //Notch_S filter not bypass
            //for NTSC Setting
            msWriteBit(V_ACI_BPF_SEL, 0 , _BIT2); // Video_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M
            msWriteBit(A_ACI_BPF_SEL, 0 , _BIT3); // Audio_ACI_BPF_Selective   0:ACI_BPF_6M , 1:ACI_BPF_7M

            break;

        default:
            break;
    }
    msVifLoadEQCoeff(ucSoundSystem);
}

// For API
void msVifTopAdjust(void)
{
    HALVIFDBG(printf("\r\nmsVifTopAdjust() "));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        msWriteByteMask(AGC_PGA2_MIN, VIFInitialIn_inst.VifTop, 0x1F); 	// pga2 min
        msWriteByteMask(AGC_PGA2_OV, VIFInitialIn_inst.VifTop, 0x1F);
        msWriteBit(AGC_PGA2_OREN, 1, _BIT1);
        msWriteBit(AGC_PGA2_OREN, 0, _BIT1);
    }
}

void msVifDynamicTopAdjust(void)
{
    HALVIFDBG(printf("\r\nmsVifDynamicTopAdjust() "));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    BYTE mean256=0, agc_pga2=0, ref=0, diff=0;
    WORD vga=0;

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        vga = msRead2Bytes(AGC_VGA);
        agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
        mean256 = (BYTE)(msRead2Bytes(AGC_MEAN256)>>1);              // AGC mean256
        ref = msReadByte(AGC_REF);                      // AGC ref

        if (g_bCheckModulationType == 0)
        {
            diff = 0x15;                                // negative modulation
        }
        else
        {
            diff = 0x0A;                                // positive modulation
        }

        if ((vga == VIFInitialIn_inst.VifVgaMinimum) && (mean256 >= (ref+diff)) && (agc_pga2 == VIFInitialIn_inst.VifTop))
        {
            msWriteByteMask(AGC_PGA2_MIN, VIFInitialIn_inst.VifDynamicTopMin, 0x1F);  // pga2 min
        }
        else if (((agc_pga2) < VIFInitialIn_inst.VifTop) && ((vga >= 0xF000) || (vga <= VIFInitialIn_inst.VifVgaMaximum)))
        {
            msVifTopAdjust();
        }
    }
}

void msVifLoad(void)
{
    if (!_hal_VIF.bBaseAddrInitialized) return ;

    RIU_WriteRegBit(RF_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB1_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB2_LOAD , 1 , _BIT0);
    RIU_WriteRegBit(DBB2_LOAD , 0, _BIT0);
}

// For API
void msVifInitial(void)
{
    HALVIFDBG(printf("\r\nmsVifInitial()"));

    if (!_hal_VIF.bBaseAddrInitialized) return ;

     // VifShiftClk : 0x1121_D3
     BYTE VifShiftClk = msReadByte(VIF_RF_RESERVED_1+1);

    msWriteByteMask(VIF_SOFT_RSTZ, 0x00, 0x7F);                     // VIF software reset
    msWriteBit(CLAMPGAIN_RSTZ, 0, _BIT0);                           // clampgain software reset
    msWriteBit(VSYNC_RSTZ, 0, _BIT0);                               // vsync software reset

    g_ucVifStatusStep = VIF_START;

    //Serious_ACI_Det parameter
    AGC_Change_Index = 0;
    RIU_WriteByte(0x1286EL, 0x04); // ADC Setting Overflow value

    if ((g_ucVifSoundSystemType == VIF_SOUND_L) || (g_ucVifSoundSystemType == VIF_SOUND_LL))
    {
        g_bCheckModulationType = 1;     // positive modulation
        g_bCheckIFFreq = (g_ucVifSoundSystemType == VIF_SOUND_L) ? 0 : 1;   // 0: 38.9 MHz; 1: 33.9 MHz
    }
    else
    {
        g_bCheckModulationType = 0;     // negative modulation
        g_bCheckIFFreq = 0;             // 38.9 MHz
    }

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(MODULATION_TYPE, 0x0F, 0x0F);               // positive modulation
    }
    else
    {
        msWriteByteMask(MODULATION_TYPE, 0x00, 0x0F);               // negative modulation
    }

    // AGC
    if (g_bCheckModulationType == 1)
    {
	    msWriteByte(AGC_PEAK_CNT_L, 0x00);                          // AGC peak cnt
	    msWriteByteMask(AGC_PEAK_CNT_H, 0x0B, 0x0F);
	    msWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefPositive);  // AGC ref
    }
    else
    {
	    msWriteByte(AGC_PEAK_CNT_L, 0x00);                          // AGC peak cnt
	    msWriteByteMask(AGC_PEAK_CNT_H, 0x0C, 0x0F);
	    msWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefNegative);  // AGC ref
    }
    msWriteByteMask(AGC_MEAN_SEL, _BIT2, _BIT2|_BIT3);              // mean16
    msWriteByte(AGC_LINE_CNT_L, 0x01);                              // AGC line cnt = 1
    msWriteByte(AGC_LINE_CNT_H, 0x00);

    if (bEnableUsrNonSteadyAgcK)
        msWriteByteMask(AGC_K, u8UsrNonSteadyAgcK, _BIT0|_BIT1|_BIT2);                // k
    else
    {
        if (VIFInitialIn_inst.VifTunerType == 1)
            msWriteByteMask(AGC_K, 0x03, _BIT0|_BIT1|_BIT2);                // k
        else
            msWriteByteMask(AGC_K, 0x02, _BIT0|_BIT1|_BIT2);                // k
    }

    msWriteByteMask(AGC_PGA2_OREN, 0x00, 0x03);
    msWriteByte(AGC_VGA_MAX_L, VIFInitialIn_inst.VifVgaMaximum);    // vga max
    msWriteByte(AGC_VGA_MAX_H, VIFInitialIn_inst.VifVgaMaximum>>8);
    msWriteByte(AGC_VGA_MIN_L, VIFInitialIn_inst.VifVgaMinimum);    // vga min
    msWriteByte(AGC_VGA_MIN_H, VIFInitialIn_inst.VifVgaMinimum>>8);
    msWriteByteMask(AGC_PGA1_MAX, 0x00, 0x0F); 		                // pga1 max
    if (VIFInitialIn_inst.VifTunerType == 1)
        msWriteByteMask(AGC_PGA2_MAX, 0x00, 0x1F); 		                // pga2 max
    else
        msWriteByteMask(AGC_PGA2_MAX, 0x1F, 0x1F); 		                // pga2 max

    msWriteByte(VAGC_VGA2_OV_L, 0x00);		    			    // VGA2(IFAGC) output minimun
    msWriteByte(VAGC_VGA2_OV_H, 0x80);	
    msWriteBit(VAGC_VGA2_OREN, 1, _BIT2);	

    if ((VIFInitialIn_inst.VifSawArch == SILICON_TUNER) || (VIFInitialIn_inst.VifSawArch == NO_SAW) ||(VIFInitialIn_inst.VifSawArch == SAVE_PIN_VIF))
        msWriteBit(VAGC_VGA_OUT_SEL, 1, _BIT0);				// VGA1 -> IFAGC

    if(VIFInitialIn_inst.VifSawArch == NO_SAW)
    {
        msWriteBit(LEVEL_SENSE_BYPASS, 0, _BIT0);               // Level_Sense not bypass
        msWriteBit(LEVEL_SENSE_OUT_SEL, 0, _BIT4);           // DVGA input: 0: from LEVEL_SENSE out(can be bypassed); 1: ACI_BPF out(cannot be bypassed)
        msWriteBit(BYPASS_V_ACI_BPF4AGC, 0, _BIT0);           // bypass ACI_BPF before AGC input: 0:not bypass; 1: bypass
    }
    else
    {
        msWriteBit(LEVEL_SENSE_BYPASS, 1, _BIT0);               // Level_Sense bypass
        msWriteBit(LEVEL_SENSE_OUT_SEL, 0, _BIT4);           // DVGA input: 0: from LEVEL_SENSE out(can be bypassed); 1: ACI_BPF out(cannot be bypassed)
        msWriteBit(BYPASS_V_ACI_BPF4AGC, 1, _BIT0);           // bypass ACI_BPF before AGC input: 0:not bypass; 1: bypass
    }

    msWriteBit(AGC_IN_SEL, 1, _BIT1);                    // AGC input 0: from SOS_out ; 1:from ACI_BPF out(can be bypassed)

    // AGC gain distribution
    msWriteBit(AGC_DBB_VVGA_SEL, 0, _BIT3);                         // Vga gain force x1
    msWriteBit(AGC_DBB_AVGA_SEL, 0, _BIT4);                         // Avga gain force x1

    msWriteByte(AGC_VGA_THR, VIFInitialIn_inst.VifVgaMaximum);      // vga threshold
    msWriteByte(AGC_VGA_THR+1, (VIFInitialIn_inst.VifVgaMaximum - 0x1000)>>8);
    msWriteByte(AGC_VGA_BASE, (VIFInitialIn_inst.VifAgcVgaBase - 0x14));     // vga base
    msWriteByte(AGC_VGA_OFFS, VIFInitialIn_inst.VifAgcVgaOffs);     // vga offset

    msWriteBit(AGC_ENABLE, 1, _BIT0);	                            // AGC enable

    // CR
    msWriteByte(CR_DL_A, 0x16);	            	    	            // CR audio delay line
    msWriteByte(CR_PD_ERR_MAX_L, 0xFF);	                            // CR pd error max
    msWriteByteMask(CR_PD_ERR_MAX_H, 0x3F, 0x3F);
    msWriteByte(CR_NOTCH_A1_L, 0x41);	            	            // CR notch filter coefficient
    msWriteByte(CR_NOTCH_A1_H, 0x0C);
    msWriteByte(CR_NOTCH_A2_L, 0xE9);	            	            // CR notch filter coefficient
    msWriteByte(CR_NOTCH_A2_H, 0x0B);
    msWriteByte(CR_NOTCH_B1_L, 0x58);	            	            // CR notch filter coefficient
    msWriteByte(CR_NOTCH_B1_H, 0x00);
    msWriteBit(CR_ANCO_SEL, 1, _BIT0);	            	            // audio nco select
    if (VIFInitialIn_inst.VifTunerType == 2)
    {
        msWriteByteMask(CR_KF1_HW, 0x00, 0x0F);   // kf1 hardware mode
        msWriteByteMask(CR_KP1_HW, 0x00, 0x0F);   // kp1 hardware mode
        msWriteByteMask(CR_KI1_HW, 0x00, 0xF0);// ki1 hardware mode
        msWriteByteMask(CR_KP2_HW, 0x00, 0x0F);   // kp2 hardware mode
        msWriteByteMask(CR_KI2_HW, 0x00, 0xF0);// ki2 hardware mode
        msWriteBit(CR_K_SEL, 0, _BIT6);	          			            // hw mode
    }
    else
    {
       msWriteByteMask(CR_KF1_HW, VIFInitialIn_inst.VifCrKf1, 0x0F);   // kf1 hardware mode
       msWriteByteMask(CR_KP1_HW, VIFInitialIn_inst.VifCrKp1, 0x0F);   // kp1 hardware mode
       msWriteByteMask(CR_KI1_HW, VIFInitialIn_inst.VifCrKi1<<4, 0xF0);// ki1 hardware mode
       msWriteByteMask(CR_KP2_HW, VIFInitialIn_inst.VifCrKp2, 0x0F);   // kp2 hardware mode
       msWriteByteMask(CR_KI2_HW, VIFInitialIn_inst.VifCrKi2<<4, 0xF0);// ki2 hardware mode
       msWriteBit(CR_K_SEL, 1, _BIT6);			// kp,ki,kf
       msWriteBit(CR_PD_IMAG_INV, 1, _BIT1);                             // for > 150% overmodulation		
    }    
    msWriteByteMask(CR_KF_SW, 0x00, 0x0F);                          // kf software mode
    msWriteByteMask(CR_KP_SW, 0x00, 0x0F);                          // kp software mode
    msWriteByteMask(CR_KI_SW, 0x00, 0xF0);                          // ki software mode
    msWriteBit(CR_JTRDET_IN_SEL, 1, _BIT4);                         // carrier jitter detector input select CR_LF1
    msWriteBit(VNCO_INV_OREN, 0, _BIT1);

    //locking range setting
    msWriteBit(CR_FD_IN_SEL, 0 , _BIT0);                               //0:IIR LPF2; 1:FIR
    msWriteBit(CR_IIR_SEL, 1 , _BIT1);                                   //0:IIR LPF1; 1:IIR LPF2
    
    if(VIFInitialIn_inst.VifCrPdModeSel == 0)                      // 0: imaginary part; 1: cordic
      msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF1);    // IIR LPF1 coefficients
    else
      msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF2);    // IIR LPF2 coefficients
       
    msWriteBit(CR_LPF_SEL, VIFInitialIn_inst.VifCrLpfSel, _BIT4);   // CR LPF 0: FIR LPF; 1: IIR LPF
    msWriteBit(CR_PD_MODE, VIFInitialIn_inst.VifCrPdModeSel, _BIT1);    // 0: imaginary part; 1: cordic
    msWriteBit(LOCK_LEAKY_SEL, VIFInitialIn_inst.VifCrLockLeakySel, _BIT0);
    msWriteBit(CR_PD_X2, VIFInitialIn_inst.VifCrPdX2, _BIT2);       // CR X2 0: lock 0 degree; 1: lock 0 or 180 degree
    msWriteByte(CR_LOCK_TH_L, VIFInitialIn_inst.VifCrLockThr);      // CR lock threshold
    msWriteByteMask(CR_LOCK_TH_H, VIFInitialIn_inst.VifCrLockThr>>8, 0x03);
    msWriteByte(CR_UNLOCK_NUM, 0x00);                               // CR unlock num
    msWriteByte(CR_UNLOCK_NUM+1, 0x40);
    msWriteByteMask(CR_UNLOCK_NUM+2, 0x00, 0x0F);
    msWriteByte(CR_LOCK_NUM, VIFInitialIn_inst.VifCrLockNum);       // CR lock num
    msWriteByte(CR_LOCK_NUM+1, VIFInitialIn_inst.VifCrLockNum>>8);
    msWriteByteMask(CR_LOCK_NUM+2, VIFInitialIn_inst.VifCrLockNum>>16, 0x0F);
    msWriteByte(CR_CODIC_TH, VIFInitialIn_inst.VifCrThr);           // CR cordic threshold
    msWriteByteMask(CR_CODIC_TH+1, VIFInitialIn_inst.VifCrThr>>8, 0x3F);

    if (VIFInitialIn_inst.VifTunerType == 0)
    {
        if (VifShiftClk/*g_VifShiftClk*/ == 1)
        {
            msWriteByte(CR_RATE, 0x6D);                                     // cr_rate for 15 MHz
            msWriteByte(CR_RATE+1, 0xDB);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert
        }    	  
        else if(VifShiftClk/*g_VifShiftClk*/ == 2)
        {
            msWriteByte(CR_RATE, 0x22);                                     // cr_rate for 15 MHz
            msWriteByte(CR_RATE+1, 0x9F);
            msWriteByteMask(CR_RATE+2, 0x15, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert
        }
      	 else
        {
            msWriteByte(CR_RATE, 0xE3);                                     // cr_rate for 15 MHz
            msWriteByte(CR_RATE+1, 0x38);
            msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
            msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert
        }                         
    }

    // tuner step size
    //VIFInitialIn_inst.VifTunerStepSize = FREQ_STEP_62_5KHz;
    if (VIFInitialIn_inst.VifTunerStepSize == FREQ_STEP_62_5KHz)    // 62.5KHz
    {
        if (g_bCheckIFFreq == 0)
        {
            msWriteByte(CR_FOE_SCAL_FACTOR_L, 0xB3);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x02, 0x0F);
        }
        else
        {                                                    // SECAM L'
            msWriteByte(CR_FOE_SCAL_FACTOR_L, 0x4D);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x0D, 0x0F);
        }
    }
    else if (VIFInitialIn_inst.VifTunerStepSize == FREQ_STEP_50KHz) // 50KHz
    {
        if (g_bCheckIFFreq == 0)
        {
            msWriteByte(CR_FOE_SCAL_FACTOR_L, 0x60);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x03, 0x0F);
        }
        else
        {                                                    // SECAM L'
            msWriteByte(CR_FOE_SCAL_FACTOR_L, 0xA0);                // foe scaling factor
            msWriteByteMask(CR_FOE_SCAL_FACTOR_H, 0x0C, 0x0F);
        }
    }

    // Filter
    msWriteBit(DEBUG_V_A, 1, _BIT5);                            // single ADC
    if (VIFInitialIn_inst.ChinaDescramblerBox !=0)
    {
        msWriteByteMask(IMAGE_REJ_IIR_SEL, _BIT3, _BIT2|_BIT3);      // 0: aci_rej_out; 1: nyq_slp_out1; 2: nyq_slp_out2; 3: mixer_out_i
    }
    msWriteByteMask(IMAGE_REJ1_SEL, _BIT0, _BIT0|_BIT1);      // 0: aci_rej_out; 1: nyq_slp_out1; 2: nyq_slp_out2; 3: mixer_out_i
    msWriteBit(BYPASS_CO_A_REJ, 0, _BIT4);                      // CO_A_REJ not bypass

    msWriteBit(IMAGE_REJ_OUT_SEL, 0, _BIT7);                                // 0: IMAGE_REJ1; 1: IMAGE_REJ_IIR
    msWriteBit(A_BP_OUT_X2, 1, _BIT7);                              // A_BP output x2
    msWriteBit(A_DAGC_SEL, 1, _BIT7);                               // 0: input from a_sos; 1: input from a_lpf_up
    msWriteBit(BYPASS_A_NOTCH, 1, _BIT6);                           // A_NOTCH bypass
    msWriteBit(BYPASS_A_SOS, 1, _BIT7);                             // A_SOS bypass
    msWriteBit(BYPASS_SOS21, 0, _BIT2);                             // SOS21 not bypass
    msWriteBit(BYPASS_SOS22, 0, _BIT3);                             // SOS22 not bypass
    msWriteBit(BYPASS_SOS31, 0, _BIT4);             	            // SOS31 not bypass
    msWriteBit(BYPASS_SOS32, 0, _BIT5);             	            // SOS32 not bypass

    // silicon tuner
    if (VIFInitialIn_inst.VifTunerType == 1)
    {
        msWriteBit(BYPASS_V_ACI_BPF4LS, 1, _BIT5);             	        // VACI_BPF bypass
        msWriteBit(BYPASS_A_ACI_BPF, 0, _BIT1);             	        // AACI_BPF not bypass
        msWriteBit(BYPASS_VSPUR_REJ, 1, _BIT2);             	        // VSPUR_REJ bypass
        msWriteBit(BYPASS_ASPUR_REJ, 1, _BIT3);             	        // ASPUR_REJ bypass

        if ((msReadByte(CR_RATE_INV) & 0x01) != 0)
            msWriteBit(BYPASS_SOS11, 1, _BIT0);                            // SOS11 bypass
    }
    else
    {
        msWriteBit(BYPASS_A_ACI_BPF, 0, _BIT1);             	        // AACI_BPF  not bypass
    }

    // DAGC1
    if (_bit1_(VIFInitialIn_inst.VifDelayReduce))
    {
        msWriteBit(DAGC1_DL_BYPASS, 1, _BIT3);                      // DAGC1 delay line bypass
    }
    else
    {
        msWriteBit(DAGC1_DL_BYPASS, 0, _BIT3);                      // DAGC1 delay line not bypass
    }
    msWriteBit(DAGC1_BYPASS, 0, _BIT1);                             // DAGC1 not bypass

    msWriteBit(DAGC1_OREN, 1, _BIT6);	                            // DAGC1 gain_overwrite = 1
    msWriteBit(DAGC1_OREN, 0, _BIT6);	                            // DAGC1 gain_overwrite = 0

    msWriteBit(DAGC1_GAIN0_FB_EN, 0, _BIT2);	                    // DAGC1 gain_update = 1

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(DAGC1_REF, 0x0B, 0x3F);		                // DAGC1 ref
        msWriteByteMask(DAGC1_RATIO, 0x03, 0x07);		            // DAGC1 ratio
        msWriteByte(DAGC1_PEAK_CNT_L, 0x00);	    	            // DAGC1 peak cnt
        msWriteByteMask(DAGC1_PEAK_CNT_H, 0x0B, 0x0F);
        msWriteByte(DAGC1_GAIN_OVERWRITE_L, VIFInitialIn_inst.VifDagc1GainOv);
        msWriteByteMask(DAGC1_GAIN_OVERWRITE_H, VIFInitialIn_inst.VifDagc1GainOv>>8, 0x3F);
    }
    else
    {
        msWriteByteMask(DAGC1_REF, VIFInitialIn_inst.VifDagc1Ref, 0x3F);    // DAGC1 ref
        msWriteByteMask(DAGC1_RATIO, 0x00, 0x07);		            // DAGC1 ratio
        msWriteByte(DAGC1_PEAK_CNT_L, 0x00);	                    // DAGC1 peak cnt
        msWriteByteMask(DAGC1_PEAK_CNT_H, 0x0C, 0x0F);
    }
    msWriteBit(DAGC1_ENABLE, 1, _BIT0);                             // DAGC1 enable

    // DAGC2
    if (_bit2_(VIFInitialIn_inst.VifDelayReduce))
    {
        msWriteBit(DAGC2_DL_BYPASS, 1, _BIT3);                      // DAGC2 delay line bypass
    }
    else
    {
        msWriteBit(DAGC2_DL_BYPASS, 0, _BIT3);                      // DAGC2 delay line not bypass
    }
    msWriteBit(DAGC2_BYPASS, 0, _BIT1);                             // DAGC2 not bypass

    msWriteBit(DAGC2_OREN, 1, _BIT6);	                            // DAGC2 gain_overwrite = 1
    msWriteBit(DAGC2_OREN, 0, _BIT6);	                            // DAGC2 gain_overwrite = 0

    msWriteBit(DAGC2_GAIN0_FB_EN, 0, _BIT2);	                    // DAGC2 gain_update = 1

    if (g_bCheckModulationType == 1)
    {
        msWriteByteMask(DAGC2_REF, 0x0B, 0x3F);		                // DAGC2 ref
        msWriteByteMask(DAGC2_RATIO, 0x03, 0x07);		            // DAGC2 ratio
        msWriteByte(DAGC2_PEAK_CNT_L, 0x00);	                    // DAGC2 peak cnt
        msWriteByteMask(DAGC2_PEAK_CNT_H, 0x0B, 0x0F);
        msWriteByte(DAGC2_GAIN_OVERWRITE_L, VIFInitialIn_inst.VifDagc2GainOv);
        msWriteByteMask(DAGC2_GAIN_OVERWRITE_H, VIFInitialIn_inst.VifDagc2GainOv>>8, 0x3F);
    }
    else
    {
        msWriteByteMask(DAGC2_REF, VIFInitialIn_inst.VifDagc2Ref, 0x3F);    // DAGC2 ref
        msWriteByteMask(DAGC2_RATIO, 0x00, 0x07);		            // DAGC2 ratio
        msWriteByte(DAGC2_PEAK_CNT_L, 0x00);	    	            // DAGC2 peak cnt
        msWriteByteMask(DAGC2_PEAK_CNT_H, 0x0C, 0x0F);
    }
    msWriteBit(DAGC2_ENABLE, 1, _BIT0);                             // DAGC2 enable

    // clampgain
    if (g_bCheckModulationType == 1)
    {
        msWriteByte(CLAMPGAIN_CLAMP_OVERWRITE, VIFInitialIn_inst.VifClampgainClampOvPositive);  // clampgain clamp overwrite value
        msWriteByteMask(CLAMPGAIN_CLAMP_OVERWRITE+1, VIFInitialIn_inst.VifClampgainClampOvPositive>>8, 0x07);
        msWriteByte(CLAMPGAIN_GAIN_OVERWRITE, VIFInitialIn_inst.VifClampgainGainOvPositive);    // clampgain gain overwrite value
        msWriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, VIFInitialIn_inst.VifClampgainGainOvPositive>>8, 0x07);
    }
    else
    {
        msWriteByte(CLAMPGAIN_CLAMP_OVERWRITE, VIFInitialIn_inst.VifClampgainClampOvNegative);  // clampgain clamp overwrite value
        msWriteByteMask(CLAMPGAIN_CLAMP_OVERWRITE+1, VIFInitialIn_inst.VifClampgainClampOvNegative>>8, 0x07);
        msWriteByte(CLAMPGAIN_GAIN_OVERWRITE, VIFInitialIn_inst.VifClampgainGainOvNegative);    // clampgain gain overwrite value
        msWriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, VIFInitialIn_inst.VifClampgainGainOvNegative>>8, 0x07);
    }
    msWriteBit(CLAMPGAIN_BYPASS, 0, _BIT1);				                                        // clampgain not bypass
    msWriteBit(CLAMPGAIN_SEL, VIFInitialIn_inst.VifClampgainClampSel, _BIT3);                   // 0: clamp select sync bottom; 1: clamp select porch
    msWriteByte(CLAMPGAIN_SYNCBOTT_REF, VIFInitialIn_inst.VifClampgainSyncbottRef);	            // porch or syncbottom ref
    msWriteByte(CLAMPGAIN_SYNCHEIGHT_REF, VIFInitialIn_inst.VifClampgainSyncheightRef);         // syncheight ref
    msWriteByteMask(CLAMPGAIN_KC, VIFInitialIn_inst.VifClampgainKc, 0x07);			            // kc
    msWriteByteMask(CLAMPGAIN_KG, VIFInitialIn_inst.VifClampgainKg<<4, 0x70);			        // kg
    msWriteByte(CLAMPGAIN_PORCH_CNT, VIFInitialIn_inst.VifClampgainPorchCnt);                   // clampgain porch cnt for NTSC
    msWriteByteMask(CLAMPGAIN_PORCH_CNT+1, VIFInitialIn_inst.VifClampgainPorchCnt>>8, 0x01);
    msWriteByte(CLAMPGAIN_CLAMP_MIN, VIFInitialIn_inst.VifClampgainClampMin);                   // clampgain clamp min
    msWriteByte(CLAMPGAIN_CLAMP_MAX, VIFInitialIn_inst.VifClampgainClampMax);                   // clampgain clamp max
    msWriteByte(CLAMPGAIN_GAIN_MIN, VIFInitialIn_inst.VifClampgainGainMin);                     // clampgain gain min
    msWriteByte(CLAMPGAIN_GAIN_MAX, VIFInitialIn_inst.VifClampgainGainMax);                     // clampgain gain max
    msWriteBit(CLAMPGAIN_CLAMP_OREN, VIFInitialIn_inst.VifClampgainClampOren, _BIT0);           // clampgain clamp overwrite enable
    msWriteBit(CLAMPGAIN_CLAMP_OREN, VIFInitialIn_inst.VifClampgainGainOren, _BIT1);            // clampgain gain overwrite enable
    msWriteBit(CLAMPGAIN_EN, 1, _BIT2);					                                        // clampgain enable

    // vsync
    msWriteBit(VSYNC_ENABLE, 1, _BIT1);                             // vsync enable

    // ADAGC
    if (g_bCheckModulationType == 1)
    {
        msWriteBit(ADAGC_BYPASS, 1, _BIT1);                         // ADAGC bypass
    	msWriteByteMask(ADAGC_K, 0x00, 0x07);			            // ADAGC k
        msWriteBit(ADAGC_ENABLE, 0, _BIT0);                         // ADAGC disable
    }
    else
    {
        msWriteBit(ADAGC_BYPASS, 0, _BIT1);                         // ADAGC not bypass
        msWriteByteMask(ADAGC_K, 0x04, 0x07);			            // ADAGC k
        msWriteBit(ADAGC_ENABLE, 1, _BIT0);                         // ADAGC enable
    }

    if(VIFInitialIn_inst.VifSeriousACIDetect == 1)                 //ACI_Functions_Selective
    {
         VIFInitialIn_inst.VifACIDetect = 0;
    }

    // zero detector
    msWriteBit(ZERO_IN_SEL, 1 , _BIT1);                         // 0: from dagc_in; 1: from dagc_out
    msWriteByteMask(ZERO_TH, 0x20, 0x7F);
    msWriteByte(ZERO_CNT_NUM, 0x0A);
    msWriteByteMask(ZERO_CNT_NUM+1, 0x00, 0x0F);
    msWriteByte(ZERO_ZERO_NUM, 0x20);
    msWriteByte(ZERO_ZERO_NUM+1, 0x00);
    msWriteBit(ZERO_ENABLE, 0 , _BIT0);                            // zero detector disable

    // Level Sense setting
    msWriteByte(LEVEL_SENSE_LOCK_CNT, 0x00);
    msWriteByte(LEVEL_SENSE_LOCK_CNT+1, 0x01);

    msWriteByte(LEVEL_SENSE_DIFF_AVG_TH, 0x28);
    msWriteByteMask(LEVEL_SENSE_DIFF_AVG_TH+1, 0x00, 0x0F);

    msWriteBit(LEVEL_SENSE_EN, 1, _BIT0);
    msWriteBit(LEVLE_SENSE_MOD_TYPE, 0, _BIT4);                 // 0: negedge; 1: posedge
    msWriteBit(LEVEL_SENSE_MODE, 0, _BIT0);                     // 0: porch; 1: sync height
    msWriteBit(LEVEL_SENSE_VGA_OREN, 0, _BIT4);

    msWriteByteMask(LEVEL_SENSE_MEAN_SEL, 0x01, 0x03);          // 0: 1 line; 1: 16 lines; 2, 3: 256 lines
    msWriteBit(LEVEL_SENSE_DVGA_OREN_SEL, 1 , _BIT4);           // 0: SW; 1: HW

    msWriteByte(LEVEL_SENSE_REF, 0x59);
    msWriteByte(LEVEL_SENSE_REF+1, 0x00);

    msWriteByte(LEVEL_SENSE_LINE_CNT, 0x04);
    msWriteByte(LEVEL_SENSE_LINE_CNT+1, 0x00);

    msWriteByte(LEVEL_SENSE_PORCH_CNT, 0xE0);
    msWriteByteMask(LEVEL_SENSE_PORCH_CNT+1, 0x00, 0x01);

    msWriteByte(LEVEL_SENSE_PEAK_CNT , 0x00);
    msWriteByteMask(LEVEL_SENSE_PEAK_CNT +1, 0x0C, 0x0F);

    msWriteByteMask(LEVEL_SENSE_K, 0x04, 0x07);                 // 0~7: 0, 2^-2 ~ 2^-8
    msWriteByteMask(LEVEL_SENSE_K+1, 0x00, 0x00);

    msWriteByte(LEVEL_SENSE_VGA_OV, 0x80);
    msWriteByte(LEVEL_SENSE_VGA_OV+1, 0x00);

    msWriteByte(LEVEL_SENSE_DIFF_AVG_INI, 0xFF);                // level_sense diff_avg initial value
    msWriteByteMask(LEVEL_SENSE_DIFF_AVG_INI+1, 0x0F, 0x0F);

    //AM Hum detection setting
    msWriteByteMask(AGC_HUM_CNT_MAX , _BIT5 , _BIT4|_BIT5|_BIT6);   // 0->128 ,1->256, 2->512 samples
    msWriteByte(AGC_HUM_ERR_THR , 0x20);                                             // format <8,8> => 0.125 = 0x20
    msWriteByte(AGC_HUM_DET_LIM , 0x20);                                              // format <8,-2> => 128 samples

    //CR_Ki/Kp speed up setting
    msWriteBit(CR_KPKI_SPEEDUP_EN , 0 , _BIT0);                                     //0:disable , 1:enable
    msWriteBit(CR_INV2_EN , 0 , _BIT4);                                                    //0:disable , 1:enable
    msWriteByteMask(CR_KP_SPEED, _BIT2 , _BIT0|_BIT1|_BIT2|_BIT3);
    msWriteByteMask(CR_KI_SPEED, _BIT6 , _BIT4|_BIT5|_BIT6|_BIT7);

    if(VIF_IS_ADC_48MHz == 0)
    {
        // VIF ADC clock setting
        msWriteBit(VIF_ADC_48M, 0, _BIT4);              // 0:144MHz , 1:48MHz

        // VIF DECI_filter coefficient selection
        msWriteBit(VIF_DECI_COEF_SEL, 0, _BIT4);    // 0:old, 1:new

        msWriteBit(HALVIFDBG2_BIT, 0, _BIT4);         // 0:144MHz, 1:48MHz
    }
    else
    {
        // VIF ADC clock setting
        msWriteBit(VIF_ADC_48M, 1, _BIT4);               // 0:144MHz , 1:48MHz

        // VIF DECI_filter coefficient selection
        msWriteBit(VIF_DECI_COEF_SEL, 1, _BIT4);     // 0:old, 1:new

        msWriteBit(HALVIFDBG2_BIT, 1, _BIT4);          // 0:144MHz, 1:48MHz
    }

    // VIF ADC LSB mask 
    msWriteByteMask(VIF_ADC_LSB_MASK, 0x00, _BIT0|_BIT1);    // Un-mask ADC_LSB bits

    // locking range +/- 500KHz  ->  +/- 1MHz  setting
    //msWriteByteMask(CR_KF1_HW, 0x02, 0x0F);   // kf1 hardware mode
    //msWriteByteMask(CR_KP1_HW, 0x43, 0x0F);   // kp1 hardware mode
    //msWriteByteMask(CR_KI1_HW, 0x43, 0xF0);   // ki1 hardware mode
    //msWriteByteMask(CR_FD_DELAY_SEL, _BIT5, _BIT4|_BIT5);
    //msWriteByteMask(CR_FD_MU, _BIT5, _BIT4|_BIT5);

    //msWriteBit(BYPASS_SOS33, 1, _BIT6);

    // real HW_KPKI_THR1_L
    msWriteByte(KPKI_ADJ_TH1_L, 0x50);
    msWriteByte(KPKI_ADJ_TH1_L+1, 0x00);

     // real HW_KPKI_THR1_H
    msWriteByte(KPKI_ADJ_TH1_H, 0x50);
    msWriteByte(KPKI_ADJ_TH1_H+1, 0x00);

    // real HW_KPKI_THR2_L
    msWriteByte(KPKI_ADJ_TH2_L, 0x00);
    msWriteByte(KPKI_ADJ_TH2_L+1, 0x01);

    // real HW_KPKI_THR2_H
    msWriteByte(KPKI_ADJ_TH2_H, 0x00);
    msWriteByte(KPKI_ADJ_TH2_H+1, 0x01);

    // real HW_KPKI_THR3_L
    msWriteByte(KPKI_ADJ_TH3_L, 0xFF);
    msWriteByte(KPKI_ADJ_TH3_L+1, 0xFF);

    // real HW_KPKI_THR3_H
    msWriteByte(KPKI_ADJ_TH3_H, 0xFF);
    msWriteByte(KPKI_ADJ_TH3_H+1, 0xFF);

    // real HW_KPKI setting
    msWriteByteMask(CR_KP_ADJ1, 0x05, 0x0F);
    msWriteByteMask(CR_KI_ADJ1, 0x80, 0xF0);
    msWriteByteMask(CR_KP_ADJ2, 0x04, 0x0F);
    msWriteByteMask(CR_KI_ADJ2, 0x70, 0xF0);
    msWriteByteMask(CR_KP_ADJ3, 0x03, 0x0F);
    msWriteByteMask(CR_KI_ADJ3, 0x60, 0xF0);

    // real HW_KPKI_disable
    msWriteBit(KPKI_ADJ_EN, 0, _BIT0);
    g_VifHWKpKiFlag = 1; // 0:SW_Kp/Ki ; 1:Real HW_Kp/Ki
    msWriteBit(HALVIFDBG2_BIT, g_VifHWKpKiFlag, _BIT0);

    // for China stream setting
    msWriteByte(CR_JTR_MAX_CNT, 0x00);
    msWriteByte(CR_JTR_MAX_CNT+1, 0x70);
    msWriteByteMask(JTR_DELTA_AVE_NUM, 0x20, 0x30);
    
    msWriteBit(BYPASS_N_A1, 1, _BIT2);
    msWriteBit(BYPASS_N_A2, 1, _BIT3);
    msWriteBit(BYPASS_SOS11, 1, _BIT0);
    msWriteBit(BYPASS_SOS12, 1, _BIT1);
    
    //msWriteBit(BYPASS_A_DC, 1, _BIT0);                               // Bypass A_DC filter
    //msWriteBit(BYPASS_A_BPF, 1, _BIT1);                              // Bypass A_BPF
    //msWriteBit(ADAGC_GAIN_OREN, 1, _BIT0);                       // fixed ADAGC gain
    //msWriteByte(ADAGC_GAIN_OV, 0x24);
    //msWriteByte(ADAGC_GAIN_OV+1, 0x00);
    
    msWriteByteMask(VIF_SOFT_RSTZ, 0x7F, 0x7D);                     // VIF software reset
    msWriteBit(CLAMPGAIN_RSTZ, 1, _BIT0);                           // clampgain software reset
    msWriteBit(VSYNC_RSTZ, 1, _BIT0);                               // vsync software reset

    // TOP
    msVifTopAdjust();

    // version control
    msWriteByte(FIRMWARE_VERSION_L, 0x13);                          // 19(dd)
    msWriteByte(FIRMWARE_VERSION_H, 0x90);                         // 09/16 (mm/yy)  firmware version control
    
    HAL_VIF_Delay1ms(1);
    msWriteByteMask(VIF_SOFT_RSTZ, 0x7F, 0x7F);
}

// For API
void msVifExit(void)
{
    if (!_hal_VIF.bBaseAddrInitialized) return ;

    // RFAGC/IFAGC disable
    RIU_WriteRegBit(RFAGC_ENABLE, 0, _BIT0);
    RIU_WriteRegBit(IFAGC_ENABLE, 0, _BIT4);

    // AGC Disable
    RIU_WriteByte(0x12830L, 0x00);
    RIU_WriteByte(0x12831L, 0x00);

    RIU_WriteByte(0x11F0AL, 0x11);
    RIU_WriteByte(0x11F0BL, 0x11);

    RIU_WriteByte(0x11F1CL, 0x11);
    RIU_WriteByte(0x11F1DL, 0x11);

    RIU_WriteByte(0x11FE3L, 0x11);

    RIU_WriteByte(0x11F78L, 0x11);

    RIU_WriteByte(0x11F25L, 0x01);

    RIU_WriteByte(0x52991L, 0x11);

    // SRAM Power Control
    RIU_WriteByte(0x12090L, 0xFC);
    RIU_WriteByte(0x12091L, 0xFF);
    
    RIU_WriteByte(0x127E0L, 0x00);
    RIU_WriteByte(0x127E1L, 0x00);
    RIU_WriteByte(0x127E2L, 0x00);
    RIU_WriteByte(0x127E3L, 0x00);
    RIU_WriteByte(0x127E4L, 0x00);
    RIU_WriteByte(0x127E5L, 0x00);
    RIU_WriteByte(0x127E6L, 0x00);
    RIU_WriteByte(0x127E7L, 0x00);
    RIU_WriteByte(0x127E8L, 0x00);
    RIU_WriteByte(0x127E9L, 0x00);

    // MPLL Power Down
    RIU_WriteByte(0x12860L, 0xFA);
    RIU_WriteByte(0x12861L, 0x24);
}

// For API
void msVifHandler(BOOL bVifDbbAcq)
{
    BYTE afc_foe;
    BYTE mean16;
    BYTE agc_pga2;
    WORD agc_vga;
    BYTE dagc1_var;
    BYTE kpki_gear;
    static BYTE crjtr_det_cnt = 0;
    static WORD kpki_cnt_idx = 0;

    if (!_hal_VIF.bBaseAddrInitialized) return ;

    MsOS_DelayTask(3);

    switch(g_ucVifStatusStep)
    {
        case VIF_START:
        case VIF_AGC_STATUS:
            g_VifCrKpKiAdjLoopCnt = 0;
            kpki_cnt_idx = 0;
            crjtr_det_cnt = 0;
	          mean16 = (BYTE)(msRead2Bytes(AGC_MEAN16)>>1);                        // AGC mean16
            agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
            agc_vga = msRead2Bytes(AGC_VGA);
            if (g_bCheckModulationType == 0)
            {
    	        if (((mean16 < AGC_MEAN16_UPBOUND) && (mean16 > AGC_MEAN16_LOWBOUND)) || (agc_pga2 == 0x1F) || (agc_vga == VIFInitialIn_inst.VifVgaMinimum))
                {
                    msWriteByte(AGC_LINE_CNT_L, 0x04);              // AGC line cnt = 4
                    msWriteByte(AGC_LINE_CNT_H, 0x00);

                    msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1; kp2,ki2,kf2
                    msWriteBit(CR_K_SEL2, 0, _BIT0);
                    
                    g_ucVifStatusStep = VIF_AFC_STATUS;
            	}
            }
            else
            {
                if (((mean16 < AGC_MEAN16_UPBOUND_SECAM) && (mean16 > AGC_MEAN16_LOWBOUND_SECAM)) || (agc_pga2 == 0x1F) || (agc_vga == VIFInitialIn_inst.VifVgaMinimum))
                {
                    msWriteByte(AGC_LINE_CNT_L, 0x04);              // AGC line cnt = 4
                    msWriteByte(AGC_LINE_CNT_H, 0x00);

                    msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1,kp2,ki2,kf2

                    g_ucVifStatusStep = VIF_AFC_STATUS;
                }
            }

	     // for No-SAW use
           if((VIFInitialIn_inst.VifSawArch == NO_SAW)&&(g_bCheckModulationType == 0))
           {
               if(bVifDbbAcq == 0)
                  msWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefNegative);
               else
                  msWriteByte(AGC_REF, VIFInitialIn_inst.VifChanelScanAGCREF);
            }

            break;

        case VIF_AFC_STATUS:
            if (_bit0_(msReadByte(CR_LOCK_STATUS)))
            {
                // DAGC
                if (g_bCheckModulationType == 1)
                {
                    msWriteBit(DAGC1_OREN, 1, _BIT6);	            // DAGC1 gain_overwrite = 1
                    msWriteBit(DAGC2_OREN, 1, _BIT6);	            // DAGC2 gain_overwrite = 1
                }
                g_ucVifStatusStep = VIF_AFC_STATUS2;
            }
            else
            {
                msWriteBit(CR_K_SEL, 0, _BIT6);	                // kp1,ki1,kf1,kp2,ki2,kf2
                HAL_VIF_Delay1us(1);
                msWriteBit(CR_NCO_FF_RSTZ, 0, _BIT2);       // reset NCO_FF
                msWriteBit(CR_LF_FF_RSTZ, 0, _BIT5);            // reset AFC integral part
                HAL_VIF_Delay1us(5);  
                msWriteBit(CR_NCO_FF_RSTZ, 1, _BIT2);
                HAL_VIF_Delay1us(1);
                msWriteBit(CR_LF_FF_RSTZ, 1, _BIT5);
            }
            break;

        case VIF_AFC_STATUS2:
            afc_foe = msReadByte(CR_FOE);                           // AFC_FOE
            if ((afc_foe <= 0x04) || (afc_foe >= 0xFC))             // |AFC_FOE|<=4
            {
                // AGC
                msWriteByte(AGC_VGA_THR, VIFInitialIn_inst.GainDistributionThr);    // vga threshold
                msWriteByte(AGC_VGA_THR+1, VIFInitialIn_inst.GainDistributionThr>>8);
                msWriteByte(AGC_VGA_BASE, VIFInitialIn_inst.VifAgcVgaBase);         // vga base
                if (bVifDbbAcq == 0)        // 0: not channel scan; 1: channel scan
                {
                    // AGC
                    if (bEnableUsrSteadyAgcK)
                        msWriteByteMask(AGC_K, u8UsrSteadyAgcK, _BIT0|_BIT1|_BIT2);// k
                    else
                    msWriteByteMask(AGC_K, 0x04, _BIT0|_BIT1|_BIT2);// k

                    msWriteByte(AGC_LINE_CNT_L, 0x10);              // AGC line cnt = 16
                    msWriteByte(AGC_LINE_CNT_H, 0x00);

                    // CR
                    msWriteRegsTbl((MS_VIF_REG_TYPE *)VIF_CR_IIR_LPF2);    // IIR LPF2 coefficients

                    g_VifCrKp = VIFInitialIn_inst.VifCrKp;
                    g_VifCrKi = VIFInitialIn_inst.VifCrKi;
                    msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                    msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);

                    msWriteByteMask(CR_KF_SW, 0x00, 0x0F);                              // kf software mode
                    msWriteBit(CR_K_SEL, 1, _BIT6);	                                    // kp,ki,kf

                    if (VIFInitialIn_inst.VifCrKpKiAdjust)
                    {
                         //if(g_VifHWKpKiFlag == 1)
                         if((msReadByte(HALVIFDBG2_BIT) & 0x01) != 0)
                         {
                             msWriteBit(KPKI_ADJ_EN, 1, _BIT0);                 // real HW_KPKI_enable
                             kpki_gear = msReadByte(CR_KPKI_GEAR) & 0x30;
                         }
                    }

                    msWriteByte(CR_PD_ERR_MAX_L, VIFInitialIn_inst.VifCrPdErrMax);      // CR pd error max
                    msWriteByteMask(CR_PD_ERR_MAX_H, VIFInitialIn_inst.VifCrPdErrMax>>8, 0x3F);
                    msWriteByte(CR_UNLOCK_NUM, VIFInitialIn_inst.VifCrUnlockNum);       // CR unlock num
                    msWriteByte(CR_UNLOCK_NUM+1, VIFInitialIn_inst.VifCrUnlockNum>>8);
                    msWriteByteMask(CR_UNLOCK_NUM+2, VIFInitialIn_inst.VifCrUnlockNum>>16, 0x0F);

                    // over modulation
                    if ((VIFInitialIn_inst.VifOverModulation == 1) && (g_bCheckModulationType == 0))
                    {
                        msWriteBit(VNCO_INV_OREN, 1, _BIT1);
                        msWriteBit(VNCO_INV_OV, 0, _BIT2);
                    }

                    g_ucVifStatusStep = VIF_STEADY_STATUS;
                }
            }

            if (!(_bit0_(msReadByte(CR_LOCK_STATUS))))
                msVifInitial();
            break;

        case VIF_STEADY_STATUS:

	        // for SAWless, ADC back-off for +20dB ACI
	     if(VIFInitialIn_inst.VifSawArch == NO_SAW)
	     {
		   if(VIFInitialIn_inst.VifSeriousACIDetect)
                       msVifSeriousACIDetection();
            }

            // Dynamic TOP adjust for strong signal
            if (VIFInitialIn_inst.VifDynamicTopAdjust)
            {
                msVifDynamicTopAdjust();
            }

            // AM hum detector
            agc_vga = msRead2Bytes(AGC_VGA);
            dagc1_var = msReadByte(DAGC1_VAR+1);
            if ((VIFInitialIn_inst.VifAmHumDetection == 1) && ((agc_vga > VIFInitialIn_inst.VifVgaMinimum) || (agc_vga < (VIFInitialIn_inst.GainDistributionThr - 0x1000))))
            {
                if ((dagc1_var >= 0x18) && (g_bCheckModulationType == 0))
                {
                    // 20% AM modulation
                    msWriteByte(AGC_REF, 0x43);                                     // AGC ref
                }
                else if ((dagc1_var <= 0x05) && (g_bCheckModulationType == 0))
                {
                    // 10% AM modulation
                    msWriteByte(AGC_REF, VIFInitialIn_inst.VifAgcRefNegative);      // AGC ref
                }
            }

            // AGC
            mean16 = (BYTE)(msRead2Bytes(AGC_MEAN16)>>1);                // AGC mean16
            if (g_bCheckModulationType == 0)
            {
    	        if ((mean16 < AGC_MEAN16_UPBOUND) && (mean16 > AGC_MEAN16_LOWBOUND))
                {
                    if (bEnableUsrSteadyAgcK)
                        msWriteByteMask(AGC_K, u8UsrSteadyAgcK, _BIT0|_BIT1|_BIT2);// k
                    else
                    msWriteByteMask(AGC_K, 0x04, _BIT0|_BIT1|_BIT2);    // k
            	}
                else
                {
                    if (bEnableUsrNonSteadyAgcK)
                        msWriteByteMask(AGC_K, u8UsrNonSteadyAgcK, _BIT0|_BIT1|_BIT2);                // k
                    else
                    {
                        if (VIFInitialIn_inst.VifTunerType == 1)
                            msWriteByteMask(AGC_K, 0x03, _BIT0|_BIT1|_BIT2);                // k
                        else
                    msWriteByteMask(AGC_K, 0x02, _BIT0|_BIT1|_BIT2);    // k
            	}
            }
            }

            // CR monitor
            agc_pga2 = msReadByte(AGC_PGA2C) & 0x1F;
            if ((agc_pga2 >= 0x0F) && (VIFInitialIn_inst.VifCrPdModeSel == 1))
            {
                msWriteByteMask(CR_KP_SW,  (VIFInitialIn_inst.VifCrKp)+0x01, 0x0F);     // kp software mode
                msWriteByteMask(CR_KI_SW, (VIFInitialIn_inst.VifCrKi<<4)+0x10, 0xF0);  // ki software mode
            }
            else
            {
                if (VIFInitialIn_inst.VifCrKpKiAdjust)
                {
                  //if(g_VifHWKpKiFlag == 1)
                  if((msReadByte(HALVIFDBG2_BIT) & 0x01) != 0)
                  {
                      g_VifCrKp = VIFInitialIn_inst.VifCrKp;
                      g_VifCrKi = VIFInitialIn_inst.VifCrKi;
                      msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                      msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);
                    
                      msWriteBit(KPKI_ADJ_EN, 1, _BIT0);                 // real HW_KPKI_enable
                      kpki_gear = msReadByte(CR_KPKI_GEAR) & 0x30;
                      
                      if(kpki_gear == 0)
                          kpki_cnt_idx = 0;
                      else
                      {
                          if(kpki_cnt_idx == 7000)
                          {
                              msWriteBit(CR_PD_IMAG_INV, 0, _BIT1);             // for > 150% overmodulation
                              kpki_cnt_idx = 0;
                          }
                          kpki_cnt_idx++;
                      }
                   }
                   else
                   {
                       msWriteBit(KPKI_ADJ_EN, 0, _BIT0);                 // real HW_KPKI_disable
                      
                       if(crjtr_det_cnt < 6)
                       {                      
                           msVifCrKpKiAutoAdjust(VIFInitialIn_inst.VifCrKpKiAdjustThr1, VIFInitialIn_inst.VifCrKpKiAdjustThr2);

                           if(g_VifCrKpKiAdjLoopCnt == 0)
                           {
                               crjtr_det_cnt++;
                               msWriteByteMask(CR_KP_SW, g_VifCrKp, 0x0F);            // Ki Kp software mode
                               msWriteByteMask(CR_KI_SW, g_VifCrKi << 4, 0xF0);

                               if(g_VifCrKp != VIFInitialIn_inst.VifCrKp)              // If carrier drift
			             msWriteBit(CR_PD_IMAG_INV, 0, _BIT1);
                               else
			             msWriteBit(CR_PD_IMAG_INV, 1, _BIT1);
                            }
                         }
                     }
                }
                else
                {
                    msWriteByteMask(CR_KP_SW, VIFInitialIn_inst.VifCrKp, 0x0F);         // kp software mode
                    msWriteByteMask(CR_KI_SW, VIFInitialIn_inst.VifCrKi<<4, 0xF0);      // ki software mode
                }
            }

            if(!(_bit0_(msReadByte(CR_LOCK_STATUS))))
            {
                    HAL_VIF_Delay1ms(50);                           // for Fluke 54200 50dBuV <-> 51dBuV switch
                if(!(_bit0_(msReadByte(CR_LOCK_STATUS))))
                {
                    // for debug
                    if (msReadByte(HALVIFDBG_BIT) & 0x08)
                    {
                          printf("VIF msVifInitial!!!");
                    }
                    msVifInitial();
                }
            }

            // for debug
            if (msReadByte(HALVIFDBG_BIT) & 0x40)
            {
                if (VIFInitialIn_inst.VifCrKpKiAdjust==1 )
                {
                    VIFInitialIn_inst.VifCrKpKiAdjust=0;
                }
                printf("\r\n Disable VIF KpKi auto adjust");
            }

            // for debug
            if ((msReadByte(HALVIFDBG_BIT) & 0x80) || (VIFInitialIn_inst.VifReserve & _BIT3))
            {
                U8 ir_rate;

                // IR Rate
                ir_rate = msReadByte(IF_RATE);
                if (ir_rate==0x49)
                    printf("\r\n IF_FREQ_3395 IF_FREQ_3890");
                else if (ir_rate==0xE3)
                    printf("\r\n IF_FREQ_3800");
                else if (ir_rate==0x8E)
                    printf("\r\n IF_FREQ_3950");
                else if (ir_rate==0xAA)
                    printf("\r\n IF_FREQ_4575");
                else if (ir_rate==0xC7)
                    printf("\r\n IF_FREQ_5875");
                else
                    printf("\r\n unknown");

                printf(" IR_RATE=0x%x ", ir_rate);

                // sound system
                if (g_ucVifSoundSystemType==0)
                    printf("\r\n VIF_SOUND_B");
                else if (g_ucVifSoundSystemType==1)
                    printf("\r\n VIF_SOUND_B_NICAM");
                else if (g_ucVifSoundSystemType==2)
                    printf("\r\n VIF_SOUND_GH");
                else if (g_ucVifSoundSystemType==3)
                    printf("\r\n VIF_SOUND_GH_NICAM");
                else if (g_ucVifSoundSystemType==4)
                    printf("\r\n VIF_SOUND_I");
                else if (g_ucVifSoundSystemType==5)
                    printf("\r\n VIF_SOUND_DK1");
                else if (g_ucVifSoundSystemType==6)
                    printf("\r\n VIF_SOUND_DK2");
                else if (g_ucVifSoundSystemType==7)
                    printf("\r\n VIF_SOUND_DK3");
                else if (g_ucVifSoundSystemType==8)
                    printf("\r\n VIF_SOUND_DK_NICAM");
                else if (g_ucVifSoundSystemType==9)
                    printf("\r\n VIF_SOUND_L");
                else if (g_ucVifSoundSystemType==10)
                    printf("\r\n VIF_SOUND_LL");
                else if (g_ucVifSoundSystemType==11)
                    printf("\r\n VIF_SOUND_MN");                    
                else
                    printf("\r\n unknown");

                printf(" sound system=%d", (BYTE)g_ucVifSoundSystemType);

                // freq band select
                printf("\r\n band=%d", (BYTE)VIFInitialIn_inst.VifFreqBand);
            }
            break;

        default:
            g_ucVifStatusStep++;
            break;
    }

    if ((g_ucVifSoundSystemType == VIF_SOUND_L) || (g_ucVifSoundSystemType == VIF_SOUND_LL))
    {
        if (g_bCheckModulationType == 0)
            msVifInitial();
        if ((g_ucVifSoundSystemType == VIF_SOUND_L) && (g_bCheckIFFreq == 1))
            msVifInitial();
        if ((g_ucVifSoundSystemType == VIF_SOUND_LL) && (g_bCheckIFFreq == 0))
            msVifInitial();
    }
    else
    {
        if (g_bCheckModulationType == 1)
            msVifInitial();
    }
}

void msVifSeriousACIDetection(void)
{
     BYTE AGC_Ref, AGC_Mean256, temp;
     BYTE PGA = 0, ADC_Index = 0, ADC_Underflow_Index = 0, ADC_Overflow_Index = 0;
     WORD VGA = 0;
          
     temp = msReadByte(AGC_REF);
     AGC_Ref =(temp << 1);
     AGC_Mean256 = msReadByte(AGC_MEAN256);

     if(SeriousACI_Index == 1)
     {
         VGA = msRead2Bytes(AGC_VGA);
         PGA = msReadByte(AGC_PGA2C);
         ADC_Index = RIU_ReadByte(0x12870L);
         ADC_Underflow_Index = ADC_Index & 0x02;
         ADC_Overflow_Index = ADC_Index & 0x04;

         if((ADC_Underflow_Index == 0x02 ||ADC_Overflow_Index == 0x04)&&(VGA == 0x7000)&&(PGA == 0x1F)&&(AGC_Change_Index == 0)
	   	&&(AGC_Ref - AGC_Mean256 > 5))
         {
             msWriteByte(AGC_REF, VIFInitialIn_inst.VifADCOverflowAGCREF);
	      msWriteBit(BYPASS_SOS21, 1 , _BIT2);
	      msWriteBit(BYPASS_SOS22, 1 , _BIT3);
	      msWriteByte(CLAMPGAIN_GAIN_OVERWRITE, 0x00);
	      msWriteByte(CLAMPGAIN_GAIN_OVERWRITE+1, 0x04);
             AGC_Change_Index = 1;
         }
	  SeriousACI_Index = 0;
     }
     SeriousACI_Index = SeriousACI_Index + 1;
 }

void msVifCrKpKiAutoAdjust(BYTE VifCrKpKiAdjustThr1, BYTE VifCrKpKiAdjustThr2)
{
    MS_S16 CrJtrMax, CrJtrMin;
    static DWORD CrJtrDelta;

    HALVIFDBG(printf("\r\msVifCrKpKiAutoAdjust()"));

    msWriteBit(CR_STATUS_LATCH_EN, 1, _BIT4);                  // latch CR loop-filter

    msWriteByteMask(CR_JTR_SEL, 0, _BIT3|_BIT2|_BIT1|_BIT0);         // 0: max
    CrJtrMax = msRead2Bytes(CR_JTR_OUT);

    msWriteByteMask(CR_JTR_SEL, _BIT0, _BIT3|_BIT2|_BIT1|_BIT0);     // 1: min
    CrJtrMin = msRead2Bytes(CR_JTR_OUT);

    msWriteBit(CR_STATUS_LATCH_EN, 0, _BIT4);                  // un-latch CR loop-filter status

    if(g_VifCrKpKiAdjLoopCnt == 0)                           // reset delta value
        CrJtrDelta = 0;

    CrJtrDelta += (DWORD)(CrJtrMax - CrJtrMin);

    if (++g_VifCrKpKiAdjLoopCnt == 32)                       // 32 samples
   {
        CrJtrDelta = CrJtrDelta >> 5;                                // divided by 32
        CrJtrDelta = CrJtrDelta >> 7;
        if (g_VifCrKp >= VIFInitialIn_inst.VifCrKp)
        {
            if (CrJtrDelta >= VifCrKpKiAdjustThr2)
            {
                g_VifCrKp -= 0x02;
                g_VifCrKi -= 0x02;
            }
    	     else if ((CrJtrDelta < VifCrKpKiAdjustThr2) && (CrJtrDelta >= VifCrKpKiAdjustThr1))
            {
                g_VifCrKp -= 0x01;
                g_VifCrKi -= 0x01;
            }
        }
        else if (g_VifCrKp == VIFInitialIn_inst.VifCrKp - 1)
        {
            if (CrJtrDelta >= VifCrKpKiAdjustThr2)               
            {
                g_VifCrKp -= 0x01;
                g_VifCrKi -= 0x01;
            }
            else if (CrJtrDelta < VifCrKpKiAdjustThr1 - 1)
            {
                g_VifCrKp += 0x01 ;
                g_VifCrKi += 0x01;
            }
        }
	 else if (g_VifCrKp == VIFInitialIn_inst.VifCrKp - 2)
	{
            if (CrJtrDelta < VifCrKpKiAdjustThr1 - 1)
            {
                g_VifCrKp += 0x02;
                g_VifCrKi += 0x02;
            }
            else if (CrJtrDelta < VifCrKpKiAdjustThr2 - 3)
            {
                g_VifCrKp += 0x01;
                g_VifCrKi += 0x01;
            }
	}

        g_VifCrKpKiAdjLoopCnt = 0;
        if (msReadByte(HALVIFDBG_BIT) & 0x20)
        {
            printf("\r\ng_ucVifStatusStep = %d", g_ucVifStatusStep);
            printf("\nKi/Kp = %x%x", g_VifCrKi, g_VifCrKp);
            printf("\nCrJtrMax = %x", CrJtrMax >> 7);
            printf("\nCrJtrMin = %x", CrJtrMin >> 7);
            printf("\r\nCrJtrDelta = %x", (WORD)((CrJtrDelta & 0xFFFF0000) >> 16));
            printf("%x\r\n",(WORD)(CrJtrDelta & 0x0000FFFF));
	 }
    }
}

U8 msVifReadCRFOE(void)
{
    HALVIFDBG(printf("\r\nmsVifReadCRFOE()"));

    if (!_hal_VIF.bBaseAddrInitialized) return 0;

    return msReadByte(CR_FOE);
}

U8 msVifReadLockStatus(void)
{
    HALVIFDBG(printf("\r\nmsVifReadLockStatus()"));

    if (!_hal_VIF.bBaseAddrInitialized) return 0;

    return msReadByte(CR_LOCK_STATUS);
}

void msVifLoadEQCoeff(BYTE VifSoundStandard)
{
   U8 u8index;

   HALVIFDBG(printf("\r\n msVifLoadEQCoeff()"));

   // set coef
   RIU_WriteByte(0x120A0L, 0x01);                      // VIF use DVB SRAM and FIR
   RIU_WriteByteMask(0x120A2L, 0x01, 0x0F);    // reg_vif_fir_coef_ctrl
   RIU_WriteByteMask(0x120A2L, 0x03, 0x0F);    // reg_vif_fir_coef_ctrl
   msWriteBit(BYPASS_EQFIR, 1, _BIT0);         // EQ BYPASS

   if(VIF_IS_EQ_IIR == 0)
   {
       // EQ FIR
       RIU_WriteRegBit(0x120A0L, 0, _BIT4);             // 0:FIR, 1:IIR

       if(VifSoundStandard == VIF_SOUND_MN)
       {
           for(u8index = 0; u8index < 56; ++u8index)
           {
               RIU_Write2Byte(0x120A4, VIF_NTSC_EQ_CO_A_REJ[u8index]+0x8000);
               RIU_Write2Byte(0x120A4, VIF_NTSC_EQ_CO_A_REJ[u8index]);
           }
       }
       else
       {
           for(u8index = 0; u8index < 56; ++u8index)
           {
               RIU_Write2Byte(0x120A4, VIF_PAL_EQ_CO_A_REJ[u8index]+0x8000);
               RIU_Write2Byte(0x120A4, VIF_PAL_EQ_CO_A_REJ[u8index]);
           }
       }
   }
   else
   {   
       // EQ IIR
       RIU_WriteRegBit(0x120A0L, 1, _BIT4);             // 0:FIR, 1:IIR
          
       switch(VifSoundStandard)
       {
           case VIF_SOUND_B:
	    case VIF_SOUND_GH:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_BG_EQ_IIR_BANDSTOP[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_BG_EQ_IIR_BANDSTOP[u8index]);
               }
               break;

           case VIF_SOUND_B_NICAM:
           case VIF_SOUND_GH_NICAM:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_BG_NICAM_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_BG_NICAM_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_I:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_I_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_I_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_DK1:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_DK1_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_DK1_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_DK2:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_DK2_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_DK2_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_DK3:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_DK3_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_DK3_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_DK_NICAM:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_DK_NICAM_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_DK_NICAM_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_MN:
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_NTSC_MN_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_NTSC_MN_EQ_IIR_NOTCH[u8index]);
               }
               break;

           case VIF_SOUND_L:
           case VIF_SOUND_LL:
		 //audio carrier 1 and 2 are at the same position as DK NICAM
               for(u8index = 0; u8index < 18; ++u8index)
               {
                   RIU_Write2Byte(0x120A4, VIF_DK_NICAM_EQ_IIR_NOTCH[u8index]+0x8000);
                   RIU_Write2Byte(0x120A4, VIF_DK_NICAM_EQ_IIR_NOTCH[u8index]);
               }
               break;
			
           case VIF_SOUND_NUMS:
           default:
               break;
       }
   }
   msWriteBit(BYPASS_EQFIR , 0 , _BIT0);     // EQ not BYPASS
}

void msVifShiftClk(BYTE VifShiftClk)
{   
    if(VIF_IS_ADC_48MHz == 0)
    {
        if (VifShiftClk == 1)
        {
            //g_VifShiftClk = 1; // 0x1121_D3
            msWriteByte(VIF_RF_RESERVED_1+1, 0x01);

            msWriteByte(0x12866L, 0x00);//loop divider
            msWriteByte(0x12867L, 0x23);
            if (VIFInitialIn_inst.VifTunerType == 0)
            {
                // move to clk 42 Mhz
                msWriteByte(CR_RATE, 0x6D);                                     // cr_rate for 15 MHz
                msWriteByte(CR_RATE+1, 0xDB);
                msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
                msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

    	         // move to clk 140 Mhz
                msWriteByte(IF_RATE, 0xA8);                 			   // IF rate for 23 MHz
                msWriteByte(IF_RATE+1, 0x83);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
            }
        }
        else if(VifShiftClk == 2)
        {
            //g_VifShiftClk = 2; // 0x1121_D3
            msWriteByte(VIF_RF_RESERVED_1+1, 0x02);

            msWriteByte(0x12866L, 0x00);//loop divider
            msWriteByte(0x12867L, 0x25);
            if (VIFInitialIn_inst.VifTunerType == 0)
            {
    	         // move to clk 44.4 Mhz
                msWriteByte(CR_RATE, 0x22);                                     // cr_rate for 15 MHz
                msWriteByte(CR_RATE+1, 0x9F);
                msWriteByteMask(CR_RATE+2, 0x15, 0x1F);
                msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

    	         // move to clk 148 Mhz
                msWriteByte(IF_RATE, 0x29);                 			    // IF rate for 23 MHz
                msWriteByte(IF_RATE+1, 0xF2);
                msWriteByteMask(IF_RATE+2, 0x09, 0x3F);
            }
        }
        else
        {
            //g_VifShiftClk = 0; // 0x1121_D3
            msWriteByte(VIF_RF_RESERVED_1+1, 0x00);

            msWriteByte(0x12866L, 0x00);//loop divider
            msWriteByte(0x12867L, 0x24);
            if (VIFInitialIn_inst.VifTunerType == 0)
            {
    	         // move to clk 43.2 Mhz
                msWriteByte(CR_RATE, 0xE3);                                     // cr_rate for 15 MHz
                msWriteByte(CR_RATE+1, 0x38);
                msWriteByteMask(CR_RATE+2, 0x16, 0x1F);
                msWriteBit(CR_RATE_INV, 0, _BIT0);                           // cr_rate not invert

    	         // move to clk 142 Mhz
                msWriteByte(IF_RATE, 0xE3);                 			   // IF rate for 23 MHz
                msWriteByte(IF_RATE+1, 0x38);
                msWriteByteMask(IF_RATE+2, 0x0A, 0x3F);
            }
        }
    }
}

void HAL_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    HALVIFDBG(printf("HAL_VIF_BypassDBBAudioFilter() bEnableq=%d\n",bEnable));
    msWriteBit(A_DAGC_SEL, (!bEnable), _BIT7);  // 0: input from a_sos; 1: input from a_lpf_up
}

BOOL HAL_VIF_GetInputLevelIndicator(void)
{
    BYTE ref, mean256, diff;

    HALVIFDBG(printf("\r\nHAL_VIF_GetInputLevelIndicator()"));

    ref = msReadByte(AGC_REF);                         // AGC ref
    mean256 = (BYTE)(msRead2Bytes(AGC_MEAN256)>>1);     // AGC mean256

    if (g_bCheckModulationType == 0)
        diff = 0x15;                                // negative modulation
    else
        diff = 0x0A;                                // positive modulation

    if (mean256 >= (ref-diff))
        return 1;
    else
        return 0;
}

U8 HAL_VIF_GetCrPDInverse(void)
{
    HALVIFDBG(printf("HAL_VIF_GetCrPDInverse() %d \n", 0));
    if ((HAL_VIF_ReadByte(CR_PD_IMAG_INV) & _BIT1)!=0)
        return 1;
    else
        return 0;
}

void HAL_VIF_SetCrPDInverse(BOOL bEnable)
{
    HALVIFDBG(printf("HAL_VIF_SetCrPDInverse() bEnableq=%d\n",bEnable));
    msWriteBit(CR_PD_IMAG_INV, (bEnable), _BIT1);  // 0: disable; 1: enable
}

#endif //_HALVIF_C_

