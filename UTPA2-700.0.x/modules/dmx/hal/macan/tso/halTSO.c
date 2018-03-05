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
// file   halTSO.c
// @brief  TS I/O HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "halTSO.h"
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HAL_REG_SAFE_MODE       1UL             // Register protection access between 1 task and 1+ ISR

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct _HalTSO_OutPad
{
    MS_U16        u16OutPad[TSO_ENGINE_NUM];
    MS_U16        u16TSCfgOld[TSO_ENGINE_NUM];
    MS_U16        u16TSOutModeOld[TSO_ENGINE_NUM];
} HalTSO_OutPad;

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl_TSO* _TSOCtrl = NULL;
static REG_Ctrl_TSO1* _TSOCtrl1 = NULL;


static MS_VIRT        _virtTSORegBase = 0;
static MS_PHY         _phyTSOFiMiuOffset[TSO_FILE_IF_NUM] = {[0 ... (TSO_FILE_IF_NUM-1)] = 0UL};
static MS_PHY         _phyTSOVQiMiuOffset = 0U;

static HalTSO_OutPad  _stOutPadCtrl;

//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFFUL);                          \
                                         (reg)->H = ((value) >> 16UL); } while(0)

#define _HAL_REG16_W(reg, value)    (reg)->data = (value);

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

#define TSO_CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_virtTSORegBase + 0x1600UL + ((addr)<<2UL))))
    #define REG_CLKGEN0_TSO_IN                          0x27UL
        #define REG_CLKGEN0_TSO_TRACE_MASK              0x000FUL
            #define REG_CLKGEN0_TSO_TRACE_DISABLE       0x0001UL
            #define REG_CLKGEN0_TSO_TRACE_INVERT        0x0002UL
            #define REG_CLKGEN0_TSO_TRACE_216M          0x0000UL
        #define REG_CLKGEN0_TSO_IN_MASK                 0x1F00UL
        #define REG_CLKGEN0_TSO_IN_SHIFT                8UL
        #define REG_CLKGEN0_TSO_IN_DISABLE              0x0100UL
        #define REG_CLKGEN0_TSO_IN_INVERT               0x0200UL
        // bit[12:8]  -> 0: disable clock
        //                   1: invert clock
        //                   bit [4:2] -> 000: Sel TS0 Clk
        //                                     001: Sel TS1 Clk
        //                                     010: Sel TS2 Clk
        //                                     011: Sel TS3 Clk
        //                                     100: Sel TS4 Clk
        //                                     101: Sel TS5 Clk
        //                                     110: Sel Dmd Clk
    #define REG_CLKGEN0_TSO_OUT_PHASE                   0x7CUL
        #define REG_CLKGEN0_TSO_OUT_DIVNUM_MASK         0x001FUL
        #define REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_MASK     0x1F00UL
        #define REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_SHIFT    8UL

    #define REG_CLKGEN0_TSO_OUT_CLK                     0x7DUL
        #define REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK        0x0001UL
        // bit[0]    ->  0: CLK_DMPLLDIV2
        //                   1: CLK_DMPLLDIV3
        #define REG_CLKGEN0_TSO_OUT_INV                 0x0002UL
        #define REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE    0x0004UL
        #define REG_CLKGEN1_TSO_OUT_PRE_CLK_MASK        0x0070UL
        #define REG_CLKGEN1_TSO_OUT_PRE_CLK_SHIFT       4UL
        // bit[6:4]  -> 000:CLK_TS0_IN
        //                     001:CLK_TS1_IN
        //                     010:CLK_TS2_IN
        //                     011:CLK_TS3_IN
        //                     100:CLK_TS4_IN
        //                     101:CLK_TS5_IN
        #define REG_CLKGEN0_TSO_OUT_CLK_MASK            0x1F00UL
            #define REG_CLKGEN0_TSO_OUT_CLK_DISABLE     0x0100UL
            #define REG_CLKGEN0_TSO_OUT_CLK_INVERT      0x0200UL
        // bit[12:8]  ->  0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 000: TSO_OUT_DIV2 (clock/2N+1)
        //                                       001: 62MHz
        //                                       010: 54MHz
        //                                       011: clk_p_tso_out (live in)
        //                                       100: clk_p_tso_out_div8 (live in)
        //                                       101: 27MHz
        //                                       111: clk_demod_ts_p
    #define REG_CLKGEN0_RESERVED0                       0x7EUL
        #define REG_CLKGEN0_RES0_CLKTSO_TOPAD_INV       0x8000UL
#define TSO_CLKGEN2_REG(addr)       (*((volatile MS_U16*)(_virtTSORegBase + 0x1400UL + ((addr)<<2UL))))
    #define REG_CLKGEN2_TSO1_IN                         0x10UL
        #define REG_CLKGEN2_TSO1_IN_MASK                0x001FUL
        #define REG_CLKGEN2_TSO1_IN_SHIFT               0UL
        #define REG_CLKGEN2_TSO1_IN_DISABLE             0x0001UL
        #define REG_CLKGEN2_TSO1_IN_INVERT              0x0002UL
        // bit[4:0]  -> 0: disable clock
        //                   1: invert clock
        //                   bit [4:2] -> 000: Sel TS0 Clk
        //                                     001: Sel TS1 Clk
        //                                     010: Sel TS2 Clk
        //                                     011: Sel TS3 Clk
        //                                     100: Sel TS4 Clk
        //                                     101: Sel TS5 Clk
        //                                     111: Sel Dmd Clk
        #define REG_CLKGEN2_TSO2_IN_MASK                0x1F00UL
        #define REG_CLKGEN2_TSO2_IN_SHIFT               8UL
        #define REG_CLKGEN2_TSO2_IN_DISABLE             0x0001UL
        #define REG_CLKGEN2_TSO2_IN_INVERT              0x0002UL
        // bit[12:8]  -> 0: disable clock
        //                    1: invert clock
        //                    bit [12:10] -> 000: Sel TS0 Clk
        //                                     001: Sel TS1 Clk
        //                                     010: Sel TS2 Clk
        //                                     011: Sel TS3 Clk
        //                                     100: Sel TS4 Clk
        //                                     101: Sel TS5 Clk
        //                                     111: Sel Dmd Clk
#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_virtTSORegBase + 0x3c00UL + ((addr)<<2UL))))
    #define REG_TOP_TSO_MUX                             0x10UL
        #define REG_TOP_TSO_EVDMODE_MASK                0x0600UL
            #define REG_TOP_TS3_OUT_MODE_TSO            0x0400UL
        #define REG_TOP_TSO_MUX_MASK                    0x7000UL
        #define REG_TOP_TSO_MUX_SHIFT                   12UL
        // bit[14:12]  -> 000: PAD_TS0
        //                      001: PAD_TS1
        //                      010: PAD_TS2
        //                      011: PAD_TS3
        //                      100: PAD_TS4
        //                      101: PAD_TS5
        //                      111: DEMOD
    #define REG_TOP_TSO1_MUX                            0x14UL
        #define REG_TOP_TSO1_MUX_MASK                   0x0007UL
        #define REG_TOP_TSO1_MUX_SHIFT                  0UL
        // bit[2:0]  ->    000: PAD_TS0
        //                      001: PAD_TS1
        //                      010: PAD_TS2
        //                      011: PAD_TS3
        //                      100: PAD_TS4
        //                      101: PAD_TS5
        //                      111: DEMOD
        #define REG_TOP_TSO2_MUX_MASK                   0x0070UL
        #define REG_TOP_TSO2_MUX_SHIFT                  4UL
        // bit[6:4]  ->    000: PAD_TS0
        //                      001: PAD_TS1
        //                      010: PAD_TS2
        //                      011: PAD_TS3
        //                      100: PAD_TS4
        //                      101: PAD_TS5
        //                      111: DEMOD
    #define REG_TOP_TS4TS5_CFG                          0x40UL
        #define REG_TOP_TS_OUT_MODE_MASK                0x0070UL
            #define REG_TOP_TS1_OUT_MODE_TSO            0x0030UL
            #define REG_TOP_TS1_OUT_MODE_Ser2Par        0x0040UL
            #define REG_TOP_TS1_OUT_MODE_Ser2Par1       0x0050UL
        #define REG_TOP_TS4_CFG_MASK                    0x0C00UL
        #define REG_TOP_TS4_CFG_SHIFT                   10UL
            #define REG_TOP_TS4_CFG_SERIAL_IN           0x0400UL
            #define REG_TOP_TS4_CFG_PARALLEL_IN         0x0800UL
        #define REG_TOP_TS5_CFG_MASK                    0x3000UL
        #define REG_TOP_TS5_CFG_SHIFT                   12UL
            #define REG_TOP_TS5_CFG_SERIAL_IN           0x1000UL

    #define REG_TOP_TS_CONFIG                           0x57UL
        #define REG_TOP_TS0_CONFIG_MASK                 0x0700UL
            #define REG_TOP_TS0_CONFIG_PARALLEL_IN      0x0100UL
            #define REG_TOP_TS0_CONFIG_SERIAL_IN        0x0200UL
            #define REG_TOP_TS0_CONFIG_MSPI_MODE        0x0300UL
            #define REG_TOP_TS0_CONFIG_3WIRE_MODE       0x0400UL
        #define REG_TOP_TS1_CONFIG_MASK                 0x3800UL
            #define REG_TOP_TS1_CONFIG_PARALLEL_IN      0x0800UL
            #define REG_TOP_TS1_CONFIG_PARALLEL_OUT     0x1000UL //out from demod
            #define REG_TOP_TS1_CONFIG_SERIAL_IN        0x1800UL
            #define REG_TOP_TS1_CONFIG_3WIRE_MODE       0x2000UL
            #define REG_TOP_TS1_CONFIG_MSPI_MODE        0x2800UL
        #define REG_TOP_TS2_CONFIG_MASK                 0xC000UL
            #define REG_TOP_TS2_CONFIG_PARALLEL_IN      0x8000UL
            #define REG_TOP_TS2_CONFIG_SERIAL_IN        0x4000UL

    #define REG_TOP_TS3_CONFIG                          0x67UL
        #define REG_TOP_TS3_CONFIG_MASK                 0xF000UL
            #define REG_TOP_TS3_CONFIG_SERIAL_IN        0x1000UL
            #define REG_TOP_TS3_CONFIG_PARALLEL_IN      0x2000UL
            #define REG_TOP_TS3_CONFIG_MSPI             0x3000UL
            #define REG_TOP_TS3_CONFIG_PAROUT_DMD       0x5000UL
            #define REG_TOP_TS3_CONFIG_PAROUT_Ser2Par   0x7000UL
            #define REG_TOP_TS3_CONFIG_PAROUT_Ser2Par1  0x8000UL

#define TSP_TSP5_REG(addr)                (*((volatile MS_U16*)(_virtTSORegBase + 0xc7600 + ((addr)<<2))))
    #define REG_TSP5_TSOIN_MUX                          0x13UL
        #define REG_TSP5_TSOIN_MUX_MASK                 0x000FUL
        #define REG_TSP5_TSOIN0_MUX_SHIFT               0UL
        #define REG_TSP5_TSOIN1_MUX_SHIFT               4UL
        #define REG_TSP5_TSOIN2_MUX_SHIFT               8UL
    #define REG_TSP5_TSOOUT_MUX                         0x15UL
        #define REG_TSP5_TSOOUT_MUX_MASK                0x000FUL
        #define REG_TSP5_TSOOUT_MUX_TSO                 0x0000UL
        #define REG_TSP5_TSOOUT_MUX_S2P0                0x0001UL

#define TSP_TS_SAMPLE_REG(addr)           (*((volatile MS_U16*)(_virtTSORegBase + 0x21600 + ((addr)<<2))))
    #define REG_TSO_OUT_CLK_SEL                         0x30UL
    #define REG_TSO_OUT_CLK_SEL_MASK                    1UL
        #define REG_TSO_OUT_TSO                         0x0000UL
        #define REG_TSO_OUT_S2P                         0x0001UL

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32 _HAL_REG32_R(REG32 *reg)
{
    MS_U32     value = 0UL;
    value  = (reg)->H << 16UL;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_REG16_R(REG16 *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

static MS_PHY _HAL_TSO_MIU_OFFSET(MS_PHY Phyaddr)
{
    #ifdef HAL_MIU2_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU2_BASE)
        return ((MS_PHY)HAL_MIU2_BASE & 0xFFFFFFFFUL);
    else
    #endif  //HAL_MIU2_BASE
    #ifdef HAL_MIU1_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU1_BASE)
        return ((MS_PHY)HAL_MIU1_BASE & 0xFFFFFFFFUL);
    else
    #endif //HAL_MIU1_BASE
        return ((MS_PHY)HAL_MIU0_BASE & 0xFFFFFFFFUL);
}

void HAL_TSO_SetBank(MS_VIRT virtBankAddr)
{
    _virtTSORegBase = virtBankAddr;
    _TSOCtrl = (REG_Ctrl_TSO*)(_virtTSORegBase+ REG_CTRL_BASE_TSO);
    _TSOCtrl1 = (REG_Ctrl_TSO1*)(_virtTSORegBase+ REG_CTRL_BASE_TSO1);
}

static MS_U32 HAL_TSO_REG32_IndR(REG32 *reg)
{
    MS_U32 u32tmp;
    MS_VIRT virtReg = (MS_VIRT)reg;

    u32tmp = ((MS_U32)virtReg)>> 1UL;

    _HAL_REG32_W(&(_TSOCtrl->TSO_INDR_ADDR), u32tmp);  // set address
    _HAL_REG16_W(&(_TSOCtrl->TSO_INDR_CTRL) , SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_INDR_CTRL)), TSO_INDIR_R_ENABLE));  // set command

    u32tmp = ((MS_U32)_HAL_REG16_R(&(_TSOCtrl->TSO_INDR_RDATA))) & 0xFFFFUL;   // get read value

    return u32tmp;
}

static void HAL_TSO_REG32_IndW(REG32 *reg, MS_U32 value)
{
    MS_VIRT virtReg = (MS_VIRT)reg;
     MS_U32 u32tmp = 0;

    u32tmp = ((MS_U32)virtReg)>> 1;

    _HAL_REG32_W(&(_TSOCtrl->TSO_INDR_ADDR), u32tmp);  // set address
    _HAL_REG32_W(&(_TSOCtrl->TSO_INDR_WDATA), value);  // set write value
    _HAL_REG16_W(&(_TSOCtrl->TSO_INDR_CTRL) , SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_INDR_CTRL)), TSO_INDIR_W_ENABLE));  // set command
}

//
// General API
//
void HAL_TSO_Init(void)
{
    MS_U8 u8ii = 0;

    //select MIU0, and 128bit MIU bus
    #if 0
    TSO_MIUDIG0_REG(REG_MIUDIG_MIU_SEL1) &= ~REG_MIUDIG_MIU_SEL1_TSO_SEL_MASK; //select miu0
    TSO_MIUDIG1_REG(REG_MIUDIG_MIU_SEL1) &= ~REG_MIUDIG_MIU_SEL1_TSO_SEL_MASK; //select miu0
    TSP_TOP_REG(REG_TOP_MIU_GP1_i64) =
        (TSP_TOP_REG(REG_TOP_MIU_GP1_i64) & ~REG_TOP_MIU_GP1_i64_TSO_MASK) | REG_TOP_MIU_GP1_i64_TSO_128BIT_CLIENT;
    #endif

    for(u8ii = 0; u8ii < (MS_U8)TSO_ENGINE_NUM; u8ii++)
    {
        _stOutPadCtrl.u16OutPad[u8ii] = 0;
        _stOutPadCtrl.u16TSCfgOld[u8ii] = 0;
        _stOutPadCtrl.u16TSOutModeOld[u8ii] = 0;
    }

    //reset
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ1), TSO_SW_RSTZ1_ALL);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ_ALL);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ1), 0);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ_DISABLE);

    //default local stream id
    _HAL_REG16_W(&(_TSOCtrl1->TSO_PRE_HEADER1_CFG0), 0x47);
    _HAL_REG16_W(&(_TSOCtrl1->TSO_PRE_HEADER5_CFG0), 0x47);
    _HAL_REG16_W(&(_TSOCtrl1->TSO_PRE_HEADER6_CFG0), 0x47);

    _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ_RX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ_RX_CFG)) & ~TSO_SVQ_RX_CFG_MODE_MASK) | TSO_SVQ_RX_CFG_MODE_CIPL);

    // Set SVQ FIFO timeout value
   _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ1_TX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ1_TX_CFG)) & ~TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_MASK) | (0x0C << TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_SHIFT));
   _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ5_TX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ5_TX_CFG)) & ~TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_MASK) | (0x0C << TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_SHIFT));
   _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ6_TX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ6_TX_CFG)) & ~TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_MASK) | (0x0C << TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_SHIFT));

}

void HAL_TSO_Reset_All(MS_U8 u8Eng)
{
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ1), TSO_SW_RSTZ1_ALL);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ_ALL);

    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ1), 0);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ_DISABLE);
}

void HAL_TSO_Reset(MS_U8 u8Eng)
{
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), _HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)) & ~TSO_SW_RSTZ_DISABLE);
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), _HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)) | TSO_SW_RSTZ_DISABLE);
}

void HAL_TSO_Reset_SubItem(MS_U8 u8Eng, MS_U16 u16RstItem)
{
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), (_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)) | u16RstItem));
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), (_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)) & ~u16RstItem));
}

void HAL_TSO_HWInt_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U16 u16init)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_Interrupt));

    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_Interrupt), (u16data | u16init));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_Interrupt), (u16data & ~u16init));
    }
}

void HAL_TSO_HWInt_Clear(MS_U8 u8Eng, MS_U16 u16Int)
{
    _HAL_REG16_W(&(_TSOCtrl->TSO_Interrupt), (_HAL_REG16_R(&(_TSOCtrl->TSO_Interrupt)) & ~u16Int));
}

MS_U16 HAL_TSO_HWInt_Status(MS_U8 u8Eng)
{
    return (_HAL_REG16_R(&(_TSOCtrl->TSO_Interrupt)) & TSO_INT_STATUS_MASK);
}

void HAL_TSO_Recover_TSOutMode(MS_U8 u8Eng)
{
    switch(_stOutPadCtrl.u16OutPad[u8Eng])
    {
        case HAL_TSOOUT_MUX_TS1:
            TSP_TOP_REG(REG_TOP_TS_CONFIG) = (TSP_TOP_REG(REG_TOP_TS_CONFIG) & ~REG_TOP_TS1_CONFIG_MASK) | _stOutPadCtrl.u16TSCfgOld[u8Eng];
            TSP_TOP_REG(REG_TOP_TS4TS5_CFG) = (TSP_TOP_REG(REG_TOP_TS4TS5_CFG) & ~REG_TOP_TS_OUT_MODE_MASK) | _stOutPadCtrl.u16TSOutModeOld[u8Eng];
            break;
        case HAL_TSOOUT_MUX_TS3:
            TSP_TOP_REG(REG_TOP_TS3_CONFIG) = TSP_TOP_REG(REG_TOP_TS3_CONFIG) & ~REG_TOP_TS3_CONFIG_MASK;
            TSP_TOP_REG(REG_TOP_TSO_MUX) = (TSP_TOP_REG(REG_TOP_TSO_MUX) & ~REG_TOP_TSO_EVDMODE_MASK) | REG_TOP_TS3_OUT_MODE_TSO;
            break;
        default:
            return;
    }
}

MS_BOOL HAL_TSO_OutPad(MS_U8 u8Eng, MS_U16* pu16OutPad, MS_BOOL bSet)
{
    if(bSet)
    {
        switch(*pu16OutPad)
        {
            case HAL_TSOOUT_MUX_TS1:
                _stOutPadCtrl.u16OutPad[u8Eng]   = *pu16OutPad;
                _stOutPadCtrl.u16TSCfgOld[u8Eng] = TSP_TOP_REG(REG_TOP_TS_CONFIG) & REG_TOP_TS1_CONFIG_MASK;
                _stOutPadCtrl.u16TSOutModeOld[u8Eng] = TSP_TOP_REG(REG_TOP_TS4TS5_CFG) & REG_TOP_TS_OUT_MODE_MASK;
                TSP_TOP_REG(REG_TOP_TS_CONFIG)   = TSP_TOP_REG(REG_TOP_TS_CONFIG) & ~REG_TOP_TS1_CONFIG_MASK;
                TSP_TOP_REG(REG_TOP_TS4TS5_CFG)  = (TSP_TOP_REG(REG_TOP_TS4TS5_CFG) & ~REG_TOP_TS_OUT_MODE_MASK) | REG_TOP_TS1_OUT_MODE_TSO;
                return TRUE;
            case HAL_TSOOUT_MUX_TS3:
                _stOutPadCtrl.u16OutPad[u8Eng]   = *pu16OutPad;
                _stOutPadCtrl.u16TSCfgOld[u8Eng] = TSP_TOP_REG(REG_TOP_TS3_CONFIG) & REG_TOP_TS3_CONFIG_MASK;
                _stOutPadCtrl.u16TSOutModeOld[u8Eng] = TSP_TOP_REG(REG_TOP_TSO_MUX) & REG_TOP_TSO_EVDMODE_MASK;
                TSP_TOP_REG(REG_TOP_TS3_CONFIG)  = TSP_TOP_REG(REG_TOP_TS3_CONFIG) & ~REG_TOP_TS3_CONFIG_MASK;
                TSP_TOP_REG(REG_TOP_TSO_MUX)     = (TSP_TOP_REG(REG_TOP_TSO_MUX) & ~REG_TOP_TSO_EVDMODE_MASK) | REG_TOP_TS3_OUT_MODE_TSO;
                return TRUE;
            default:
            return FALSE;
        }
    }
    else
    {
        *pu16OutPad = HAL_TSOOUT_MUX_NONE;

        if((TSP_TOP_REG(REG_TOP_TS4TS5_CFG) & REG_TOP_TS_OUT_MODE_MASK) == REG_TOP_TS1_OUT_MODE_TSO)
        {
            if((TSP_TOP_REG(REG_TOP_TS_CONFIG) & REG_TOP_TS1_CONFIG_MASK) == 0)
            {
                *pu16OutPad = HAL_TSOOUT_MUX_TS1;
            }
        }
        else if((TSP_TOP_REG(REG_TOP_TSO_MUX) & REG_TOP_TSO_EVDMODE_MASK) == REG_TOP_TS3_OUT_MODE_TSO)
        {
            if((TSP_TOP_REG(REG_TOP_TS3_CONFIG) & REG_TOP_TS3_CONFIG_MASK) == 0)
            {
                *pu16OutPad = HAL_TSOOUT_MUX_TS3;
            }
        }

        return TRUE;
    }
}

MS_BOOL HAL_TSO_SelPad(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16InPadSel, MS_BOOL bParallel)
{
    MS_U16 u16Reg, u16RegMask, u16RegShift;
    MS_U16 u16data = 0;

    // Set pad mux
    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16RegShift = REG_TSP5_TSOIN0_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16RegShift = REG_TSP5_TSOIN1_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16RegShift = REG_TSP5_TSOIN2_MUX_SHIFT;
            break;
        default:
            return FALSE;
    }

    //set pad configure
    switch(u16InPadSel)
    {
        case HAL_TSOIN_MUX_TS0:
            u16Reg = REG_TOP_TS_CONFIG;
            u16RegMask = REG_TOP_TS0_CONFIG_MASK;
            if(bParallel)
            {
                u16data = REG_TOP_TS0_CONFIG_PARALLEL_IN;
            }
            else
            {
                u16data = REG_TOP_TS0_CONFIG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TS1:
            u16Reg = REG_TOP_TS_CONFIG;
            u16RegMask = REG_TOP_TS1_CONFIG_MASK;
            if(bParallel)
            {
                u16data = REG_TOP_TS1_CONFIG_PARALLEL_IN;
            }
            else
            {
                u16data = REG_TOP_TS1_CONFIG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TS2:
            u16Reg = REG_TOP_TS_CONFIG;
            u16RegMask = REG_TOP_TS2_CONFIG_MASK;
            if(bParallel)
            {
                u16data = REG_TOP_TS2_CONFIG_PARALLEL_IN;
            }
            else
            {
                u16data = REG_TOP_TS2_CONFIG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TS3:
            u16Reg = REG_TOP_TS3_CONFIG;
            u16RegMask = REG_TOP_TS3_CONFIG_MASK;
            if(bParallel)
            {
                u16data = REG_TOP_TS3_CONFIG_PARALLEL_IN;
            }
            else
            {
                u16data = REG_TOP_TS3_CONFIG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TS4:
            u16Reg = REG_TOP_TS4TS5_CFG;
            u16RegMask = REG_TOP_TS4_CFG_MASK;
            if(bParallel)
            {
                u16data = REG_TOP_TS4_CFG_PARALLEL_IN;
            }
            else
            {
                u16data = REG_TOP_TS4_CFG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TS5:
            u16Reg = REG_TOP_TS4TS5_CFG;
            u16RegMask = REG_TOP_TS5_CFG_MASK;
            if(bParallel)
            {
                return FALSE;
            }
            else
            {
                u16data = REG_TOP_TS5_CFG_SERIAL_IN;
            }
            break;
        case HAL_TSOIN_MUX_TSDEMOD0:
            TSP_TSP5_REG(REG_TSP5_TSOIN_MUX) = (TSP_TSP5_REG(REG_TSP5_TSOIN_MUX) & ~(REG_TSP5_TSOIN_MUX_MASK << u16RegShift)) | (u16InPadSel << u16RegShift);
            return TRUE;

        default:
            return FALSE;
    }

    TSP_TOP_REG(u16Reg) = (TSP_TOP_REG(u16Reg) & ~u16RegMask) | u16data;

    TSP_TSP5_REG(REG_TSP5_TSOIN_MUX) = (TSP_TSP5_REG(REG_TSP5_TSOIN_MUX) & ~(REG_TSP5_TSOIN_MUX_MASK)) | (u16InPadSel << u16RegShift);

    return TRUE;
}

MS_BOOL HAL_TSO_Set_InClk(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16ClkSel, MS_BOOL bClkInvert, MS_BOOL bEnable)
{
    MS_U16 u16Reg, u16RegMask, u16RegShift;
    MS_U16 u16value = 0;

     //printf("[%s] u8TsIf %x, u16ClkSel %d\n", __FUNCTION__, (int)u8TsIf, u16ClkSel);

    //set clock
    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16Reg = REG_CLKGEN0_TSO_IN;
            u16RegMask = REG_CLKGEN0_TSO_IN_MASK;
            u16RegShift = REG_CLKGEN0_TSO_IN_SHIFT;
            u16value = TSO_CLKGEN0_REG(u16Reg) & ~u16RegMask;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16Reg = REG_CLKGEN2_TSO1_IN;
            u16RegMask = REG_CLKGEN2_TSO1_IN_MASK;
            u16RegShift = REG_CLKGEN2_TSO1_IN_SHIFT;
            u16value = TSO_CLKGEN2_REG(u16Reg) & ~u16RegMask;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16Reg = REG_CLKGEN2_TSO1_IN;
            u16RegMask = REG_CLKGEN2_TSO2_IN_MASK;
            u16RegShift = REG_CLKGEN2_TSO2_IN_SHIFT;
            u16value = TSO_CLKGEN2_REG(u16Reg) & ~u16RegMask;
            break;
        default:
            return FALSE;
    }

    //printf("[%s] u16RegMask %x, u16RegShift %d\n", __FUNCTION__, u16RegMask, u16RegShift);

    if(!bEnable)
    {
        u16value |= ((REG_CLKGEN2_TSO1_IN_DISABLE << u16RegShift) & 0xFFFFUL);
    }
    else
    {
        if(u16ClkSel > TSO_CLKIN_TS5)
        {
            return FALSE;
        }

        u16value |= (u16ClkSel << u16RegShift);
        if(bClkInvert)
        {
            u16value |= ((REG_CLKGEN2_TSO1_IN_INVERT << u16RegShift) & 0xFFFFUL);
        }
    }

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            TSO_CLKGEN0_REG(u16Reg) = u16value;
            break;
        case HAL_TSO_TSIF_LIVE2:
        case HAL_TSO_TSIF_LIVE3:
            TSO_CLKGEN2_REG(u16Reg) = u16value;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_GetInputTSIF_Status(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16* pu16Pad, MS_BOOL* pbClkInvert, MS_BOOL* pbExtSync, MS_BOOL* pbParl)
{
    MS_U16 u16Reg, u16RegMask, u16RegShift;
    MS_U16 u16data = 0;
    REG16* reg16 = 0;

    // Set pad mux
    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegMask = REG_TOP_TSO_MUX_MASK;
            u16RegShift = REG_TOP_TSO_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegMask = REG_TOP_TSO1_MUX_MASK;
            u16RegShift = REG_TOP_TSO1_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegMask = REG_TOP_TSO2_MUX_MASK;
            u16RegShift = REG_TOP_TSO2_MUX_SHIFT;
            break;
        default:
            return FALSE;
    }

    *pu16Pad = (TSP_TOP_REG(u16Reg) & u16RegMask) >> u16RegShift;

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16data = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & REG_CLKGEN0_TSO_IN_MASK) >> REG_CLKGEN0_TSO_IN_SHIFT;
            reg16 = &(_TSOCtrl->TSO_CH0_IF1_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16data = (TSO_CLKGEN0_REG(REG_CLKGEN2_TSO1_IN) & REG_CLKGEN2_TSO1_IN_MASK) >> REG_CLKGEN2_TSO1_IN_SHIFT;
            reg16 = &(_TSOCtrl->TSO_CH0_IF5_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = (TSO_CLKGEN0_REG(REG_CLKGEN2_TSO1_IN) & REG_CLKGEN2_TSO2_IN_MASK) >> REG_CLKGEN2_TSO2_IN_SHIFT;
            reg16 = &(_TSOCtrl->TSO_CH0_IF6_CFG2);
            break;
        default:
            return FALSE;
    }

    *pbExtSync = ((_HAL_REG16_R(reg16) & TSO_CHCFG_EXT_SYNC_SEL) == TSO_CHCFG_EXT_SYNC_SEL);
    *pbParl = ((_HAL_REG16_R(reg16) & TSO_CHCFG_P_SEL) == TSO_CHCFG_P_SEL);
    *pbClkInvert = ((u16data & REG_CLKGEN2_TSO1_IN_INVERT) == REG_CLKGEN2_TSO1_IN_INVERT);

    return TRUE;

}

MS_BOOL HAL_TSO_OutClk_DefSelect(MS_U8 u8Eng, MS_U16 u16PadSel, MS_BOOL bSet, HalTSOOutClk* pstOutClkSet)
{
    if((u16PadSel == 0xFFFF) || (bSet == TRUE))
    {
        return FALSE; //not support yet
    }

    switch(u16PadSel)
    {
        case HAL_TSOIN_MUX_TS0:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS0IN;
            break;
        case HAL_TSOIN_MUX_TS1:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS1IN;
            break;
        case HAL_TSOIN_MUX_TS2:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS2IN;
            break;
        case HAL_TSOIN_MUX_TS3:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS3IN;
            break;
        case HAL_TSOIN_MUX_TS4:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS4IN;
            break;
        case HAL_TSOIN_MUX_TS5:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT;
            pstOutClkSet->u16PreTsoOutClk = HAL_PRE_TSO_OUT_SEL_TS5IN;
            break;
        case HAL_TSOIN_MUX_TSDEMOD0:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD;
            break;
        case HAL_TSOIN_MUX_MEM:
        case HAL_TSOIN_MUX_MEM1:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_DIV2N;
            pstOutClkSet->u16OutDivSrc = HAL_TSO_OUT_DIV_SEL_172M_2N;
            pstOutClkSet->u16OutDivNum = 0x0F; //default: 172.8/2(15+1) = 5.4M
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

// default: dmplldiv5 / 2 (11+1) = 7.2 MHz
// default: dmplldiv_3 / 2 (17+1) = 8 MHz
void HAL_TSO_OutputClk(MS_U8 u8Eng, HalTSOOutClk* pstOutClkSet, MS_BOOL bSet)
{
    if(bSet == TRUE)
    {
        if(pstOutClkSet->bEnable == FALSE)
        {
            HAL_TSO_OutClk(u8Eng, &(pstOutClkSet->u16OutClk), &(pstOutClkSet->bClkInvert), &(pstOutClkSet->bEnable), TRUE);
            return;
        }

        switch(pstOutClkSet->u16OutClk)
        {
            case HAL_TSO_OUT_SEL_TSO_OUT_DIV2N:
                HAL_TSO_TSOOutDiv(u8Eng, &(pstOutClkSet->u16OutDivSrc), &(pstOutClkSet->u16OutDivNum), TRUE);
                break;
            case HAL_TSO_OUT_SEL_TSO_OUT_62MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_54MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_27MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD:
                break;
            case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT:
            case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8:
                HAL_TSO_PreTsoOutClk(u8Eng, &(pstOutClkSet->u16PreTsoOutClk), TRUE);
                break;
            default:
                return;
        }

        HAL_TSO_Set_TSOOut_Phase_Tune(u8Eng, 0, FALSE); //default -> no phase tuning
        HAL_TSO_OutClk(u8Eng, &(pstOutClkSet->u16OutClk), &(pstOutClkSet->bClkInvert), &(pstOutClkSet->bEnable), TRUE); //alyays need TSO out clock
    }
    else
    {
        HAL_TSO_OutClk(u8Eng, &(pstOutClkSet->u16OutClk), &(pstOutClkSet->bClkInvert), &(pstOutClkSet->bEnable), FALSE);
        if(pstOutClkSet->u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_DIV2N)
        {
            HAL_TSO_TSOOutDiv(u8Eng, &(pstOutClkSet->u16OutDivSrc), &(pstOutClkSet->u16OutDivNum), FALSE);
        }
        else if((pstOutClkSet->u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT) || (pstOutClkSet->u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8))
        {
            HAL_TSO_PreTsoOutClk(u8Eng, &(pstOutClkSet->u16PreTsoOutClk), FALSE);
        }
    }
}

MS_BOOL HAL_TSO_Set_TSOOut_Phase_Tune(MS_U8 u8Eng, MS_U16 u16ClkOutPhase, MS_BOOL bPhaseEnable)
{
    MS_U16 u16value = 0;

    if(!bPhaseEnable)
    {
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) &= ~REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE;
    }
    else
    {
        u16value = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & ~REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_MASK)
                    | (u16ClkOutPhase << REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_SHIFT);

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) = u16value;
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) |= REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_PreTsoOutClk(MS_U8 u8Eng, MS_U16* pu16PreTsoOutSel, MS_BOOL bSet)
{
    if(bSet == TRUE)
    {
        if(*pu16PreTsoOutSel > HAL_PRE_TSO_OUT_SEL_TS5IN)
        {
            return FALSE;
        }

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) =
        (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) & ~REG_CLKGEN1_TSO_OUT_PRE_CLK_MASK) | (*pu16PreTsoOutSel << REG_CLKGEN1_TSO_OUT_PRE_CLK_SHIFT);
    }
    else
    {
        *pu16PreTsoOutSel = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) & REG_CLKGEN1_TSO_OUT_PRE_CLK_MASK) >> REG_CLKGEN1_TSO_OUT_PRE_CLK_SHIFT;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_TSOOutDiv(MS_U8 u8Eng, MS_U16* pu16ClkOutDivSrcSel, MS_U16 *pu16ClkOutDivNum, MS_BOOL bSet)
{
    //clock source for clock divide
    if(bSet == TRUE)
    {
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & ~REG_CLKGEN0_TSO_TRACE_MASK) | REG_CLKGEN0_TSO_TRACE_216M;

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) & ~REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK) | (*pu16ClkOutDivSrcSel);

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & ~REG_CLKGEN0_TSO_OUT_DIVNUM_MASK) | (*pu16ClkOutDivNum);
    }
    else
    {
        *pu16ClkOutDivSrcSel = TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) & REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK;
        *pu16ClkOutDivNum = TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & REG_CLKGEN0_TSO_OUT_DIVNUM_MASK;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_OutClk(MS_U8 u8Eng, MS_U16* pu16ClkOutSel, MS_BOOL* pbClkInvert, MS_BOOL* pbEnable, MS_BOOL bSet)
{
    MS_U16 u16Clk = TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) & ~REG_CLKGEN0_TSO_OUT_CLK_MASK;

    if(bSet == TRUE)
    {
        if(*pbEnable == FALSE)
        {
            u16Clk |= REG_CLKGEN0_TSO_OUT_CLK_DISABLE;
        }
        else
        {
            TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) =
                (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & ~REG_CLKGEN0_TSO_TRACE_MASK) | REG_CLKGEN0_TSO_TRACE_216M;

            u16Clk |= (*pu16ClkOutSel);

            if(*pbClkInvert)
            u16Clk |= REG_CLKGEN0_TSO_OUT_CLK_INVERT;

            TSO_CLKGEN0_REG(REG_CLKGEN0_RESERVED0) |= REG_CLKGEN0_RES0_CLKTSO_TOPAD_INV;
        }
    TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) = u16Clk;
    }
    else
    {
        *pbEnable = ((u16Clk & REG_CLKGEN0_TSO_OUT_CLK_DISABLE) == 0);
        *pbClkInvert = ((u16Clk & REG_CLKGEN0_TSO_OUT_CLK_INVERT) == REG_CLKGEN0_TSO_OUT_CLK_INVERT);
        *pu16ClkOutSel = u16Clk;
    }

    return TRUE;
}

// ------------------------------------------------------
//  APIS
//-------------------------------------------------------
void HAL_TSO_Flt_SetPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16PID)
{
    MS_U32 u32value;
    REG_PidFlt* pidReg = &(_TsoPid[u8Eng].Flt[u16FltId]);

    u32value = (HAL_TSO_REG32_IndR((REG32 *)pidReg) & ~TSO_PIDFLT_PID_MASK) | (((MS_U32)u16PID << TSO_PIDFLT_PID_SHFT) & TSO_PIDFLT_PID_MASK);
    HAL_TSO_REG32_IndW((REG32 *)pidReg, u32value);
}

void HAL_TSO_Flt_SetInputSrc(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16InputSrc)
{
    MS_U32 u32value;
    REG_PidFlt* pidReg = &(_TsoPid[u8Eng].Flt[u16FltId]);

    u32value = (HAL_TSO_REG32_IndR((REG32 *)pidReg) & ~TSO_PIDFLT_IN_MASK) | (u16InputSrc << TSO_PIDFLT_IN_SHIFT);
    HAL_TSO_REG32_IndW((REG32 *)pidReg, u32value);
}

MS_BOOL HAL_TSO_ReplaceFlt_SetPktPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U8 u8TsIf, MS_U16 u16OldPid, MS_U16 u16NewPid)
{
    REG32 *pReg = 0;
    MS_U32 u32data = (((MS_U32)u16OldPid) & REP_PIDFLT_ORG_PID_MASK) | (((MS_U32)u8TsIf) << REP_PIDFLT_SRC_SHIFT) |
                        ((((MS_U32)u16NewPid) << REP_PIDFLT_NEW_PID_SHIFT) & REP_PIDFLT_NEW_PID_MASK);
    pReg = &(_TSOCtrl->REP_PidFlt[u16FltId]);
    _HAL_REG32_W(pReg, u32data);

    return TRUE;
}

MS_BOOL HAL_TSO_ReplaceFlt_Enable(MS_U8 u8Eng, MS_U16 u16FltId, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG32_W(&(_TSOCtrl->REP_PidFlt[u16FltId]), SET_FLAG1(_HAL_REG32_R(&(_TSOCtrl->REP_PidFlt[u16FltId])), REP_PIDFLT_REPLACE_EN));
    }
    else
    {
        _HAL_REG32_W(&(_TSOCtrl->REP_PidFlt[u16FltId]), RESET_FLAG1(_HAL_REG32_R(&(_TSOCtrl->REP_PidFlt[u16FltId])), REP_PIDFLT_REPLACE_EN));
    }

    return TRUE;
}

void HAL_TSO_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng, MS_PHY phyAddr)
{
    _phyTSOFiMiuOffset[u8FileEng] = _HAL_TSO_MIU_OFFSET(phyAddr);

    if(u8FileEng == 0)
    {
        _HAL_REG32_W(&(_TSOCtrl->TSO_Filein_raddr), (MS_U32)(phyAddr-_phyTSOFiMiuOffset[u8FileEng]));
    }
    else if(u8FileEng == 1)
    {
        _HAL_REG32_W(&(_TSOCtrl->TSO_Filein_raddr1), (MS_U32)(phyAddr-_phyTSOFiMiuOffset[u8FileEng]));
    }
}

void HAL_TSO_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32len)
{
    if(u8FileEng == 0)
    {
        _HAL_REG32_W(&(_TSOCtrl->TSO_Filein_rNum), u32len);
    }
    else if(u8FileEng == 1)
    {
        _HAL_REG32_W(&(_TSOCtrl->TSO_Filein_rNum1), u32len);
    }
}

MS_PHY HAL_TSO_Get_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_PHY phyvalue = 0;

    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng])) | TSO_FICFG_RADDR_READ);
    phyvalue = ((MS_PHY)_HAL_REG32_R(&(_TSOCtrl->TSO_TSO2MI_RADDR[u8FileEng])) & 0xFFFFFFFFUL) << TSO_MIU_BUS;
    phyvalue += _phyTSOFiMiuOffset[u8FileEng];
    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng])) & ~TSO_FICFG_RADDR_READ);
    return phyvalue;
}

void HAL_TSO_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16ctrl)
{
    if(u8FileEng == 0)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_Filein_Ctrl), (_HAL_REG16_R(&(_TSOCtrl->TSO_Filein_Ctrl)) & ~TSO_FILEIN_CTRL_MASK) | u16ctrl);
    }
    else if(u8FileEng == 1)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_Filein_Ctrl1), (_HAL_REG16_R(&(_TSOCtrl->TSO_Filein_Ctrl1)) & ~TSO_FILEIN_CTRL_MASK) | u16ctrl);
    }
}

MS_U16 HAL_TSO_Get_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    if(u8FileEng == 0)
    {
        return (_HAL_REG16_R(&(_TSOCtrl->TSO_Filein_Ctrl)) & TSO_FILEIN_CTRL_MASK);
    }
    else if(u8FileEng == 1)
    {
        return (_HAL_REG16_R(&(_TSOCtrl->TSO_Filein_Ctrl1)) & TSO_FILEIN_CTRL_MASK);
    }

    return 0;
}

MS_BOOL HAL_TSO_Set_Filein_MOBFKey(MS_U8 u8FileEng, MS_U32 u32Key, MS_BOOL bSecured)
{
    MS_U16 u16data = (bSecured ? TSO_FILEIN_RIU_TSO_NS : 0);
    REG16* pReg = ((u8FileEng == 0)? (&(_TSOCtrl->TSO_Filein_Ctrl)) : (&(_TSOCtrl->TSO_Filein_Ctrl1)));

    if((_HAL_REG16_R(pReg) & (TSO_FILEIN_RSTART|TSO_FILEIN_ABORT)) != 0)
    {
        return FALSE;
    }

    u16data |= ((MS_U16)(u32Key << TSO_FILEIN_MOBF_IDX_SHIFT)  & TSO_FILEIN_MOBF_IDX_MASK);
    _HAL_REG16_W(pReg, u16data)

    return TRUE;
}

MS_BOOL HAL_TSO_Filein_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    MS_U16 u16ChIf = ((u8Eng == 0)? TSO_CFG1_TSO_TSIF5_EN: TSO_CFG1_TSO_TSIF6_EN);

    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG1)), u16ChIf));
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]),
            SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_TSO_FILEIN|TSO_FICFG_FILE_SEGMENT|TSO_FICFG_TS_DATAPORT_SEL));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG1)), u16ChIf));
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]),
            RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_TSO_FILEIN|TSO_FICFG_FILE_SEGMENT|TSO_FICFG_TS_DATAPORT_SEL));
    }

    return TRUE;
}

void HAL_TSO_FileinTimer_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng])) | TSO_FICFG_TIMER_ENABLE);
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng])) & ~TSO_FICFG_TIMER_ENABLE);
    }
}

void HAL_TSO_Filein_Rate(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16timer)
{
    _HAL_REG16_W(&(_TSOCtrl->TSO_FI_TIMER[u8FileEng]), u16timer);
}

void HAL_TSO_Filein_192Mode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_PKT192_ENABLE));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]),
            RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_PKT192_ENABLE));
    }
}

void HAL_TSO_Filein_192BlockMode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]),
            RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_PKT192_BLK_DISABLE));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]),
            SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_FILE_CFG[u8FileEng]), TSO_FICFG_PKT192_BLK_DISABLE));
    }
}

MS_U16 HAL_TSO_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Shift = ((u8FileEng == 0) ? 0: 8);

    return ((_HAL_REG16_R(&(_TSOCtrl->TSO_CMDQ_STATUS)) >> u16Shift) & TSO_CMDQ_STS_WCNT_MASK);
}

MS_BOOL HAL_TSO_CmdQ_FIFO_IsFull(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Shift = ((u8FileEng == 0) ? 0: 8);

    return (MS_BOOL)((_HAL_REG16_R(&(_TSOCtrl->TSO_CMDQ_STATUS)) >> u16Shift) & TSO_CMDQ_STS_FIFO_FULL);
}

MS_BOOL HAL_TSO_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Shift = ((u8FileEng == 0) ? 0: 8);

    return (MS_BOOL)((_HAL_REG16_R(&(_TSOCtrl->TSO_CMDQ_STATUS)) >> u16Shift) & TSO_CMDQ_STS_FIFO_EMPTY);
}

MS_U8 HAL_TSO_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Shift = ((u8FileEng == 0) ? 0: 8);

    return (MS_U8)((_HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_CMDQ_STATUS)) >> u16Shift) & TSO_CMDQ_STS_WLEVEL_MASK);
}

MS_BOOL HAL_TSO_CmdQ_Reset(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16data = ((u8FileEng == 0)? TSO_SW_RSTZ_CMDQ : TSO_SW_RSTZ_CMDQ1);

    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)), u16data));
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)), u16data));
    return TRUE;
}

void HAL_TSO_RW_ValidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16ValidBlockCnt)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CFG2));

    if(bWrite)
    {
        u16data &= ~TSO_CFG2_VALID_BYTECNT_MASK;
        u16data |= (*pu16ValidBlockCnt << TSO_CFG2_VALID_BYTECNT_SHIFT);
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG2), u16data);

        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
    }
    else
    {
        *pu16ValidBlockCnt = (u16data & TSO_CFG2_VALID_BYTECNT_MASK) >> TSO_CFG2_VALID_BYTECNT_SHIFT;
    }
}

void HAL_TSO_RW_InvalidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16InvalidBlockCnt)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CFG2));

    if(bWrite)
    {
        u16data &= ~TSO_CFG2_INVALID_BYTECNT_MASK;
        u16data |= (*pu16InvalidBlockCnt << TSO_CFG2_INVALID_BYTECNT_SHIFT);
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG2), u16data);

        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
    }
    else
    {
        *pu16InvalidBlockCnt = u16data & TSO_CFG2_INVALID_BYTECNT_MASK;
    }
}

void HAL_TSO_RW_OutputPktSize(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16PktSize)
{
    if(bWrite)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG3), *pu16PktSize);
    }
    else
    {
        *pu16PktSize = _HAL_REG16_R(&(_TSOCtrl->TSO_CFG3));
    }

    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CFG1), TSO_CFG1_PKT_PARAM_LD));
}

void   HAL_TSO_LPcr2_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32lpcr2)
{
    MS_U16 u16temp = _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]));

    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), u16temp | TSO_FICFG_LPCR2_WLD);
    _HAL_REG32_W(&(_TSOCtrl->TSO_LPCR2[u8FileEng]), u32lpcr2);
    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), u16temp);
}

MS_U32 HAL_TSO_LPcr2_Get(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U32 u32temp = 0;
    MS_U16 u16temp = _HAL_REG16_R(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]));

    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), ((u16temp & ~TSO_FICFG_LPCR2_WLD)|TSO_FICFG_LPCR2_LD));
    u32temp = _HAL_REG32_R(&_TSOCtrl->TSO_LPCR2[u8FileEng]);
    _HAL_REG16_W(&(_TSOCtrl->TSO_FILE_CFG[u8FileEng]), u16temp);

    return u32temp;
}

MS_U32 HAL_TSO_TimeStamp_Get(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    return _HAL_REG32_R(&(_TSOCtrl->TSO_TIMESTAMP[u8FileEng]));
}

MS_BOOL HAL_TSO_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8If, MS_U8 u8size)
{
    MS_U16 u16data = 0;

    if(u8If == HAL_TSO_TSIF_LIVE1)
    {
        u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CH0_IF1_CFG0)) & ~TSO_PKT_SIZE_CHK_LIVE_MASK;
        _HAL_REG16_W(&(_TSOCtrl->TSO_CH0_IF1_CFG0), u16data | (MS_U16)u8size);
    }
    else if((u8If == HAL_TSO_TSIF_LIVE2) || (u8If == HAL_TSO_TSIF_FILE1))
    {
        u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CH0_IF5_CFG0)) & ~TSO_PKT_SIZE_CHK_LIVE_MASK;
        _HAL_REG16_W(&(_TSOCtrl->TSO_CH0_IF5_CFG0), u16data | (MS_U16)u8size);
    }
    else if((u8If == HAL_TSO_TSIF_LIVE3) || (u8If == HAL_TSO_TSIF_FILE2))
    {
        u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CH0_IF6_CFG0)) & ~TSO_PKT_SIZE_CHK_LIVE_MASK;
        _HAL_REG16_W(&(_TSOCtrl->TSO_CH0_IF6_CFG0), u16data | (MS_U16)u8size);
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

void HAL_TSO_Filein_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U8 u8size)
{
    MS_U16 u16temp = 0, u16shift = ((u8FileEng == 0) ? 0: 8);

    u16temp = _HAL_REG16_R(&(_TSOCtrl->TSO_PKT_CHKSIZE_FI)) & ~(TSO_PKT_CHKSIZE_FI_MASK << u16shift);
    u16temp |= (((MS_U16)(u8size & 0xFF)) << u16shift);

    _HAL_REG16_W(&(_TSOCtrl->TSO_PKT_CHKSIZE_FI), u16temp);
}

void HAL_TSO_Cfg1_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CFG1));

    if(bEnable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), u16data);
}

void HAL_TSO_Cfg4_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CFG4));

    if(bEnable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG4), u16data);
}

MS_BOOL HAL_TSO_ChIf_Enable(MS_U8 u8Eng, MS_U8 u8ChIf, MS_BOOL bEnable)
{
    MS_U16 u16data = 0;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16data = TSO_CFG1_TSO_TSIF1_EN;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16data = TSO_CFG1_TSO_TSIF5_EN;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = TSO_CFG1_TSO_TSIF6_EN;
            break;
        default:
            return FALSE;
    }

    if(bEnable)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG1)), u16data));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG1), RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG1)), u16data));
    }

    return FALSE;

}

MS_BOOL HAL_TSO_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16 u16Cfg, MS_BOOL bEnable)
{
    REG16* pReg = NULL;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            pReg = &(_TSOCtrl->TSO_CH0_IF1_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            pReg = &(_TSOCtrl->TSO_CH0_IF5_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            pReg = &(_TSOCtrl->TSO_CH0_IF6_CFG2);
            break;
        default:
            return FALSE;
    }

    if(bEnable)
    {
        _HAL_REG16_W(pReg, SET_FLAG1(_HAL_REG16_R(pReg), u16Cfg));
    }
    else
    {
        _HAL_REG16_W(pReg, RESET_FLAG1(_HAL_REG16_R(pReg), u16Cfg));
    }

    return TRUE;
}

MS_BOOL HAL_TSO_Get_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Cfg, MS_BOOL *pbEnable)
{
    REG16* pReg = NULL;
    MS_U16 u16data = 0;

    *pu16Cfg = 0;
    *pbEnable = FALSE;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            pReg = &(_TSOCtrl->TSO_CH0_IF1_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            pReg = &(_TSOCtrl->TSO_CH0_IF5_CFG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            pReg = &(_TSOCtrl->TSO_CH0_IF6_CFG2);
            break;
        default:
            return FALSE;
    }

    *pu16Cfg = _HAL_REG16_R(pReg);

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16data = TSO_CFG1_TSO_TSIF1_EN;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16data = TSO_CFG1_TSO_TSIF5_EN;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = TSO_CFG1_TSO_TSIF6_EN;
            break;
        default:
            return FALSE;
    }

    *pbEnable = ((_HAL_REG16_R(&(_TSOCtrl->TSO_CFG1)) & u16data) == u16data);

    return TRUE;

}


MS_BOOL HAL_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_U8 u8ChIf, MS_PHY phyBufAddr, MS_U32 u32BufSize)
{
    REG32* p32Reg = NULL;
    REG16* p16Reg = NULL;
    REG16* p16RegCfg = NULL;
    MS_U32 u32addr = 0;

    _phyTSOVQiMiuOffset = _HAL_TSO_MIU_OFFSET(phyBufAddr);
    u32addr = (MS_U32)(phyBufAddr - _phyTSOVQiMiuOffset);

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            p32Reg = &(_TSOCtrl1->TSO_SVQ1_BASE);
            p16Reg = &(_TSOCtrl1->TSO_SVQ1_SIZE);
            p16RegCfg = &(_TSOCtrl1->TSO_SVQ1_TX_CFG);
            break;
        case HAL_TSO_TSIF_LIVE2:
            p32Reg = &(_TSOCtrl1->TSO_SVQ5_BASE);
            p16Reg = &(_TSOCtrl1->TSO_SVQ5_SIZE);
            p16RegCfg = &(_TSOCtrl1->TSO_SVQ5_TX_CFG);
            break;
        case HAL_TSO_TSIF_LIVE3:
            p32Reg = &(_TSOCtrl1->TSO_SVQ6_BASE);
            p16Reg = &(_TSOCtrl1->TSO_SVQ6_SIZE);
            p16RegCfg = &(_TSOCtrl1->TSO_SVQ6_TX_CFG);
            break;
        default:
            return FALSE;
    }

    _HAL_REG32_W(p32Reg, u32addr >> TSO_MIU_BUS);
    _HAL_REG16_W(p16Reg , u32BufSize/TSO_SVQ_UNIT_SIZE);

    // Reset SVQ
    _HAL_REG16_W(p16RegCfg , SET_FLAG1(_HAL_REG16_R(p16RegCfg), TSO_SVQ_TX_CFG_TX_RESET));
    _HAL_REG16_W(p16RegCfg , RESET_FLAG1(_HAL_REG16_R(p16RegCfg), TSO_SVQ_TX_CFG_TX_RESET));

    _HAL_REG16_W(p16RegCfg, SET_FLAG1(_HAL_REG16_R(p16RegCfg), TSO_SVQ_TX_CFG_SVQ_EN));

    return TRUE;
}

MS_BOOL HAL_TSO_ChIf_ClrByteCnt(MS_U8 u8Eng, MS_U8 u8ChIf)
{
    MS_U16 u16data = 0;

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16data = TSO_CLR_BYTE_CNT_1;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16data = TSO_CLR_BYTE_CNT_5;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = TSO_CLR_BYTE_CNT_6;
            break;
        default:
            return FALSE;
    }


    _HAL_REG16_W(&(_TSOCtrl->TSO_CLR_BYTE_CNT), SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CLR_BYTE_CNT)), u16data));
    _HAL_REG16_W(&(_TSOCtrl->TSO_CLR_BYTE_CNT), RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CLR_BYTE_CNT)), u16data));

    return TRUE;
}

MS_BOOL HAL_TSO_LocalStreamID(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U8* pu8StrID, MS_BOOL beSet)
{
    REG16* p16Reg = NULL;

    if(beSet == FALSE)
    {
        *pu8StrID = 0xFF;
    }

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            p16Reg = &(_TSOCtrl1->TSO_PRE_HEADER1_CFG0);
            break;
        case HAL_TSO_TSIF_LIVE2:
            p16Reg = &(_TSOCtrl1->TSO_PRE_HEADER5_CFG0);
            break;
        case HAL_TSO_TSIF_LIVE3:
            p16Reg = &(_TSOCtrl1->TSO_PRE_HEADER6_CFG0);
            break;
        default:
            return FALSE;
    }

    if(beSet == TRUE)
    {
        _HAL_REG16_W(p16Reg , (MS_U16)(*pu8StrID) & 0xFF);
    }
    else
    {
        *pu8StrID = (MS_U8)(_HAL_REG16_R(p16Reg) & 0xFF);
    }

    return TRUE;

}

MS_BOOL HAL_TSO_SVQ_TX_Reset(MS_U8 u8Eng, MS_U8 u8ChIf)
{
    REG16* p16Reg = NULL;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            p16Reg = &(_TSOCtrl1->TSO_SVQ1_TX_CFG);
            break;
        case HAL_TSO_TSIF_LIVE2:
            p16Reg = &(_TSOCtrl1->TSO_SVQ5_TX_CFG);
            break;
        case HAL_TSO_TSIF_LIVE3:
            p16Reg = &(_TSOCtrl1->TSO_SVQ6_TX_CFG);
            break;
        default:
            return FALSE;
    }

    _HAL_REG16_W(p16Reg , SET_FLAG1(_HAL_REG16_R(p16Reg), TSO_SVQ_TX_CFG_TX_RESET));
    _HAL_REG16_W(p16Reg , RESET_FLAG1(_HAL_REG16_R(p16Reg), TSO_SVQ_TX_CFG_TX_RESET));

    return TRUE;

}

MS_BOOL HAL_TSO_Set_SVQRX_MOBFKey(MS_U8 u8Eng, MS_U32 u32Key, MS_BOOL bSecured)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ_RX_CFG)) & ~TSO_SVQ_RX_CFG_SVQ_MOBF_IDX_MASK;

    u16data |= ((MS_U16)(u32Key << TSO_SVQ_RX_CFG_SVQ_MOBF_IDX_SHIFT)  & TSO_SVQ_RX_CFG_SVQ_MOBF_IDX_MASK);

    if(bSecured)
    {
        u16data |= TSO_SVQ_RX_CFG_SVQ_MIU_NS;
    }
    else
    {
        u16data &= ~TSO_SVQ_RX_CFG_SVQ_MIU_NS;
    }

    _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ_RX_CFG), u16data);

    return TRUE;
}

MS_BOOL HAL_TSO_Set_SVQRX_PktMode(MS_U8 u8Eng, MS_U16 u16mode)
{
    _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ_RX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ_RX_CFG)) & ~TSO_SVQ_RX_CFG_MODE_MASK) | u16mode);

    return TRUE;
}

MS_BOOL HAL_TSO_Set_SVQRX_ArbitorMode(MS_U8 u8Eng, MS_U16 u16mode, MS_U16 *pu16SvqRxPri)
{
    MS_U8 u8ii = 0, u8jj = 0;
    MS_U16 u16shift = 0;

    _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ_RX_CFG), (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ_RX_CFG)) & ~TSO_SVQ_RX_CFG_ARBMODE_MASK) | u16mode);

    if(u16mode != TSO_SVQ_RX_CFG_ARBMODE_FIXPRI)
    {
        return TRUE;
    }

    for(u8ii = 0; u8ii < TSO_SVQ_RX_NUM; u8ii++)
    {
        u8jj = u8ii >> 1;
        u16shift = ((u8ii % 2) ? TSO_SVQ_RX_PRI_SHIFT: 0);

        _HAL_REG16_W(&(_TSOCtrl1->TSO_SVQ_RX_PRI[u8jj]),
            (_HAL_REG16_R(&(_TSOCtrl1->TSO_SVQ_RX_PRI[u8jj])) & ~(TSO_SVQ_RX_PRI_MASK << u16shift)) | (pu16SvqRxPri[u8ii] << u16shift));
    }

    return TRUE;
}

MS_BOOL HAL_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32systime)
{
    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG4) , RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG4)), TSO_CFG4_SET_SYS_TIMESTAMP));
    _HAL_REG32_W(&(_TSOCtrl->TSO_SYSTIMESTAMP), u32systime);
    _HAL_REG16_W(&(_TSOCtrl->TSO_CFG4) , SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG4)), TSO_CFG4_SET_SYS_TIMESTAMP));

    return FALSE;
}

MS_BOOL HAL_TSO_Get_SVQ_Status(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Status)
{
    MS_U32 u32data = 0;
    MS_U32 u32Shift = 0;

    *pu16Status = 0;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    u32data = _HAL_REG32_R(&(_TSOCtrl1->TSO_SVQ_STATUS));

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u32Shift = 0;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u32Shift = 16;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u32Shift = 20;
            break;
        default:
            return FALSE;
    }

    *pu16Status = ((MS_U16)(u32data >> u32Shift) & TSO_SVQ_STS_MASK);

    return TRUE;

}

MS_BOOL HAL_TSO_GetDelayTime_PreHd2Output(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U32 *pu32time)
{
    *pu32time = 0;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    _HAL_REG16_W(&(_TSOCtrl1->TSO_DELTA_CFG) , SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl1->TSO_DELTA_CFG)), TSO_DELTA_CFG_DELTA_CLR));
    _HAL_REG16_W(&(_TSOCtrl1->TSO_DELTA_CFG) , RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl1->TSO_DELTA_CFG)), TSO_DELTA_CFG_DELTA_CLR));

    _HAL_REG16_W(&(_TSOCtrl1->TSO_DELTA_CFG) ,
        (_HAL_REG16_R(&(_TSOCtrl1->TSO_DELTA_CFG)) & ~TSO_DELTA_CFG_SEL_CH_MASK) | u8ChIf);

    *pu32time = _HAL_REG32_R(&(_TSOCtrl1->TSO_DELTA));

    return TRUE;
}

MS_BOOL HAL_TSO_Get_MaxDelta_ChId(MS_U8 u8Eng, MS_U8 *pu8ChIf)
{
    *pu8ChIf = 0xFF;

    *pu8ChIf = (MS_U8)((_HAL_REG16_R(&(_TSOCtrl1->TSO_DELTA_CFG)) & TSO_DELTA_CFG_MAX_ID_MASK) >> TSO_DELTA_CFG_MAX_ID_SHIFT);

    return TRUE;
}

MS_BOOL HAL_TSO_Sel_LocalSysStampClkBase(MS_U8 u8Eng, MS_U16 u16ClkBase)
{
    if(u16ClkBase == HAL_TSO_TIMESTAMP_27M)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG4) , SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG4)), TSO_CFG4_SET_TIMESTAMP_27M));
    }
    else
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CFG4) , RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->TSO_CFG4)), TSO_CFG4_SET_TIMESTAMP_BASE_MASK));
    }

    return TRUE;
}



