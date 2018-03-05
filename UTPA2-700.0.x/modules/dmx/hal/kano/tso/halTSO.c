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
#include "MsCommon.h"
#include "regTSO.h"
#include "halTSO.h"
#include "drvSYS.h"

#ifdef CONFIG_MSTAR_CLKM
    #include "drvCLKM.h"
#endif

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HAL_REG_SAFE_MODE       1             // Register protection access between 1 task and 1+ ISR

#define MIU_BUS                     4

#define KANOU02                     (MDrv_SYS_GetChipRev() >= 0x1) // This feature/behavior is supported after Kano U02


//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl_TSO* _TSOCtrl = NULL;
static REG_Ctrl_TSO1* _TSOCtrl1 = NULL;

static MS_VIRT   _u32TSORegBase = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Message
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_HAL_TSO_DBG_LEVEL_NONE,   // no debug message shown
    E_HAL_TSO_DBG_LEVEL_ERR,    // only shows error message that can't be recover
    E_HAL_TSO_DBG_LEVEL_WARN,   // error case can be recover, like retry
    E_HAL_TSO_DBG_LEVEL_EVENT,  // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_HAL_TSO_DBG_LEVEL_INFO,   // information for internal parameter
    E_HAL_TSO_DBG_LEVEL_FUNC,   // Function trace and input parameter trace
    E_HAL_TSO_DBG_LEVEL_TRACE,  // debug trace
} EN_HAL_TSO_DBGMSG_LEVEL;

typedef enum
{
    E_HAL_TSO_DBG_MODEL_NONE,   // @temporarily , need to refine
    E_HAL_TSO_DBG_MODEL_ALL,
} EN_HAL_TSO_DBGMSG_MODEL;

#define HAL_TSO_DBGMSG(_level,_model,_f) do {if(_u32TSODbgLevel >= (_level)&&((_u32TSODbgModel&_model)!=0)) (_f);} while(0)
static MS_U32  _u32TSODbgLevel = E_HAL_TSO_DBG_LEVEL_ERR;
static MS_U32  _u32TSODbgModel = E_HAL_TSO_DBG_MODEL_ALL;


//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                         (reg)->H = ((value) >> 16); } while(0)

#define _HAL_REG16_W(reg, value)    { (reg)->data = (MS_U16)((value) & 0xFFFF); }

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))


#define _REG16_SET(reg, value);    _HAL_REG16_W(reg, SET_FLAG1(_HAL_REG16_R(reg), value));
#define _REG32_SET(reg, value);    _HAL_REG32_W(reg, SET_FLAG1(_HAL_REG32_R(reg), value));
#define _REG16_CLR(reg, value);    _HAL_REG16_W(reg, RESET_FLAG1(_HAL_REG16_R(reg), value));
#define _REG32_CLR(reg, value);    _HAL_REG32_W(reg, RESET_FLAG1(_HAL_REG32_R(reg), value));




#define TSO_CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_u32TSORegBase + 0x1600UL + ((addr)<<2))))
    #define REG_CLKGEN0_TSO_IN                               0x27
        #define REG_CLKGEN0_TSO_IN_MASK                      0x001F
        #define REG_CLKGEN0_TSO_IN_SHIFT                     2
            #define REG_CLKGEN0_TSO_IN_DISABLE               0x0001
            #define REG_CLKGEN0_TSO_IN_INVERT                0x0002
            // bit[4:0]  ->      0: disable clock
            //                        1: invert clock
            //                        bit [3:2] ->    000: select TS0_CLK
            //                                             001: select TS1_CLK
            //                                             010: from demod 0
        #define REG_CLKGEN0_TSO_TRACE_MASK                   0x0F00
            #define REG_CLKGEN0_TSO_TRACE_DISABLE            0x0100
            #define REG_CLKGEN0_TSO_TRACE_INVERT             0x0200
            #define REG_CLKGEN0_TSO_TRACE_216M               0x0000

    #define REG_CLKGEN0_TSO_OUT_PHASE                        0x2D
        #define REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_MASK          0x001F
        #define REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_SHIFT         0
        #define REG_CLKGEN0_TSO_OUT_PHASE_TUN_EN_MASK        0x0020
            #define REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE     0x0020
        #define REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK             0x0040
            #define HAL_TSO_OUT_DIV_SEL_172M_2N              0x0000
            #define HAL_TSO_OUT_DIV_SEL_288M_2N              0x0040

    #define REG_CLKGEN0_TSO_OUT_DIVNUM                       0x2E
        #define REG_CLKGEN0_TSO_OUT_DIVNUM_MASK              0x07C0
        #define REG_CLKGEN0_TSO_OUT_DIVNUM_SHIFT             6
        #define REG_CLKGEN_TSO_P_TSO_OUT_MASK                0xF800
        #define REG_CLKGEN_TSO_P_TSO_OUT_SHIFT               13

    #define REG_CLKGEN0_TSO_OUT_CLK                          0x2F
        #define REG_CLKGEN0_TSO_OUT_CLK_MASK                 0x001F
            #define REG_CLKGEN0_TSO_OUT_CLK_DISABLE          0x0001
            #define REG_CLKGEN0_TSO_OUT_CLK_INVERT           0x0002
            #define REG_CLKGEN0_TSO_OUT_CLK_SRC_SHIFT        2
            // bit[4:0]  ->      0: disable clock
            //                        1: invert clock
            //                        bit [4:2] ->     000: from demod 0, clk_dvbtc_ts_p
            //                                              001: 62MHz
            //                                              010: 54MHz
            //                                              011: clk_p_tso_out (live in)
            //                                              100: clk_p_tso_out_div8 (live in)
            //                                              101: tso_out_div (clock/(N+1))
            //                                              110: 86MHz

    #define TSO_CLKGEN1_REG(addr)       (*((volatile MS_U16*)(_u32TSORegBase + 0x6600UL + ((addr)<<2))))
    #define REG_CLKGEN1_DEMOD0_OUT_CLK                   0x00
        #define REG_CLKGEN1_DVBTM0_TS_DIVNUM_MASK 0x001F //demod0 div num of output clk
        #define REG_CLKGEN1_DVBTM0_TS_DIVNUM_SHIFT 0
        #define REG_CLKGEN1_ATSC_DVB0_DIV_SEL_MASK 0x0100 //demod0 div src of output clk
        #define REG_CLKGEN1_ATSC_DVB0_DIV_SEL_SHIFT 8
        // bit[8]    ->  0: CLK_DMPLLDIV2
        //               1: CLK_DMPLLDIV3

#define TSO_CLKGEN2_REG(addr)       (*((volatile MS_U16*)(_u32TSORegBase + 0x1400UL + ((addr)<<2))))
    #define REG_CLKGEN2_TSO1_IN                             0x06
    #define REG_CLKGEN2_TSO2_IN                             0x07
    #define REG_CLKGEN2_TSO3_IN                             0x08
    #define REG_CLKGEN2_TSO4_IN                             0x09
    #define REG_CLKGEN2_TSO5_IN                             0x0a

#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_u32TSORegBase + 0x3c00UL + ((addr)<<2))))
    #define REG_TOP_TSO_MUX                         0x3A
        #define REG_TOP_TSO_MUX_MASK                0x0007
        #define REG_TOP_TSO1_MUX_SHIFT              0
        #define REG_TOP_TSO2_MUX_SHIFT              4
        #define REG_TOP_TSO3_MUX_SHIFT              8
        #define REG_TOP_TSO4_MUX_SHIFT              12

    #define REG_TOP_TSO1_MUX                        0x3B
        #define REG_TOP_TSO5_MUX_SHIFT              0
        #define REG_TOP_TSO6_MUX_SHIFT              4
        // bit[2:0]  ->        000: PAD_TS0
        //                     001: PAD_TS1
        //                     0111: DEMOD

#if 0 // Not used
    #define REG_TOP_MIU_GP1_i64                         0x21
        #define REG_TOP_MIU_GP1_i64_TSO_MASK            0x0080
        #define REG_TOP_MIU_GP1_i64_TSO_128BIT_CLIENT   0x0000
        #define REG_TOP_MIU_GP1_i64_TSO_64BIT_CLIENT    0x0080
    #define REG_TOP_TS_CONFIG                           0x57
        #define REG_TOP_TS0_CONFIG_MASK                 0x0700
            #define REG_TOP_TS0_CONFIG_PARALLEL_IN      0x0100
            #define REG_TOP_TS0_CONFIG_SERIAL_IN        0x0200
        #define REG_TOP_TS1_CONFIG_MASK                 0x3800
            #define REG_TOP_TS1_CONFIG_PARALLEL_IN      0x0800
            #define REG_TOP_TS1_CONFIG_PARALLEL_OUT     0x1000
            #define REG_TOP_TS1_CONFIG_SERIAL_IN        0x1800
        #define REG_TOP_TS2_CONFIG_MASK                 0x4000
            #define REG_TOP_TS2_CONFIG_PARALLEL_IN      0x4000
        #define REG_TOP_TSCB_CONFIG_MASK                0x8000
            #define REG_TOP_TSCB_CONFIG_SERIAL_IN       0x8000
    #define REG_TOP_TS_OUT_MODE                         0x6E
        #define REG_TOP_TS_OUT_MODE_MASK                0x0300
            #define REG_TOP_TS_OUT_MODE_TSO             0x0200
#endif

#define TSO_MIUDIG0_REG(addr)       (*((volatile MS_U16*)(_u32TSORegBase + 0x0C00UL + ((addr)<<2))))
#define TSO_MIUDIG1_REG(addr)       (*((volatile MS_U16*)(_u32TSORegBase + 0x2400UL + ((addr)<<2))))
    #define REG_MIUDIG_MIU_SEL1                         0x79
        #define REG_MIUDIG_MIU_SEL1_TSO_SEL_MASK        0x0080

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static MS_U32 _HAL_REG32_R(REG32_TSO *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_REG16_R(REG16_TSO *reg)
{
    MS_U16              value = 0;
    value = (reg)->data;
    return value;
}

void HAL_TSO_SetBank(MS_VIRT u32BankAddr)
{
    _u32TSORegBase = u32BankAddr;
    _TSOCtrl = (REG_Ctrl_TSO*)(_u32TSORegBase+ REG_CTRL_BASE_TSO);      // 0x1706
    _TSOCtrl1 = (REG_Ctrl_TSO1*)(_u32TSORegBase+ REG_CTRL_BASE_TSO1);   // 0x1612
}

//
// General API
//
void HAL_TSO_Init(void)
{
    //select MIU0, and 128bit MIU bus
    /*
    TSO_MIUDIG0_REG(REG_MIUDIG_MIU_SEL1) &= ~REG_MIUDIG_MIU_SEL1_TSO_SEL_MASK; //select miu0
    TSO_MIUDIG1_REG(REG_MIUDIG_MIU_SEL1) &= ~REG_MIUDIG_MIU_SEL1_TSO_SEL_MASK; //select miu0
    TSP_TOP_REG(REG_TOP_MIU_GP1_i64) =
        (TSP_TOP_REG(REG_TOP_MIU_GP1_i64) & ~REG_TOP_MIU_GP1_i64_TSO_MASK) | REG_TOP_MIU_GP1_i64_TSO_128BIT_CLIENT;
    */

    //KANO U02 ECO
    if(KANOU02)
    {
        _REG16_SET(&(_TSOCtrl->TSO_CONFIG5), TSO_CONFIG5_FIXED_MIU_REG_FLUSH);
    }
}

void HAL_TSO_Reset_All(MS_U8 u8Eng)
{
    _REG16_CLR(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ);//low active
    _REG16_SET(&(_TSOCtrl->SW_RSTZ), TSO_SW_RSTZ);

    _REG16_SET(&(_TSOCtrl->SW_RSTZ), TSO_SW_RST_ALL | TSO_SW_RST_ALL1);
    _REG16_CLR(&(_TSOCtrl->SW_RSTZ), TSO_SW_RST_ALL | TSO_SW_RST_ALL1);
}

void HAL_TSO_Reset(MS_U8 u8Eng)
{
    //@TODO not find in register table
    /*
    _HAL_REG16_W(&(_TSOCtrl[u8Eng].SW_RSTZ), _HAL_REG16_R(&(_TSOCtrl[u8Eng].SW_RSTZ)) & ~TSO_SW_RSTZ_DISABLE);
    _HAL_REG16_W(&(_TSOCtrl[u8Eng].SW_RSTZ), _HAL_REG16_R(&(_TSOCtrl[u8Eng].SW_RSTZ)) | TSO_SW_RSTZ_DISABLE);
    */
}

void HAL_TSO_Reset_SubItem(MS_U8 u8Eng, MS_U16 u16RstItem)
{
    _HAL_REG16_W(&(_TSOCtrl[u8Eng].SW_RSTZ), (_HAL_REG16_R(&(_TSOCtrl[u8Eng].SW_RSTZ)) | u16RstItem));
    _HAL_REG16_W(&(_TSOCtrl[u8Eng].SW_RSTZ), (_HAL_REG16_R(&(_TSOCtrl[u8Eng].SW_RSTZ)) & ~u16RstItem));
}

void HAL_TSO_HWInt_Enable(MS_U8 u8Eng, MS_BOOL benable, MS_U16 u16init)
{
    REG16_TSO *reg = (u8Eng == 0)? &(_TSOCtrl->INTERRUPT) : &(_TSOCtrl->INTERRUPT1);
    MS_U16 u16data = _HAL_REG16_R(reg);

    if(benable)
    {
        _HAL_REG16_W(reg, (u16data | u16init));
    }
    else
    {
        _HAL_REG16_W(reg, (u16data & ~u16init));
    }
}

void HAL_TSO_HWInt_Clear(MS_U8 u8Eng, MS_U16 u16Int)
{
    REG16_TSO *reg = (u8Eng == 0)? &(_TSOCtrl->INTERRUPT) : &(_TSOCtrl->INTERRUPT1);

    _HAL_REG16_W(reg, (_HAL_REG16_R(reg) & ~u16Int));
}

MS_U16 HAL_TSO_HWInt_Status(MS_U8 u8Eng)
{
    REG16_TSO *reg = (u8Eng == 0)? &(_TSOCtrl->INTERRUPT) : &(_TSOCtrl->INTERRUPT1);

    return (_HAL_REG16_R(reg) & TSO_INT_STS_MASK);
}

MS_BOOL HAL_TSO_3WirePadMapping(MS_U8 u8Pad3WireId, MS_U16 *u16Pad, MS_U16 *u16Clk)
{
    switch(u8Pad3WireId)
    {
        case 0:
            *u16Pad = HAL_TSOIN_MUX_TS3;
            *u16Clk = TSO_CLKIN_TS3;
            break;
        case 1:
            *u16Pad = HAL_TSOIN_MUX_TS4;
            *u16Clk = TSO_CLKIN_TS4;
            break;
        case 2:
            *u16Pad = HAL_TSOIN_MUX_TS5;
            *u16Clk = TSO_CLKIN_TS5;
            break;
        case 3:
            *u16Pad = HAL_TSOIN_MUX_TS6;
            *u16Clk = TSO_CLKIN_TS6;
            break;
        default:
            printf("[%s][%d]: Not support !!\n", __FUNCTION__, __LINE__);
            return FALSE;
    }

    return TRUE;
}


MS_BOOL HAL_TSO_SelPad(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16InPadSel, MS_BOOL bParallel)
{
    MS_U16 u16Reg, u16RegShift;

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO1_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO2_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO3_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE4:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO4_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE5:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegShift = REG_TOP_TSO5_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE6:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegShift = REG_TOP_TSO6_MUX_SHIFT;
            break;
        default:
            printf("Not support !!\n");
            return FALSE;
    }

    TSP_TOP_REG(u16Reg) = (TSP_TOP_REG(u16Reg) & ~(REG_TOP_TSO_MUX_MASK << u16RegShift)) | (u16InPadSel << u16RegShift);

    //@NOTE: no need to change input pad mode dynamically (Mboot handle it...)

    return TRUE;
}

MS_BOOL HAL_TSO_OutPad(MS_U8 u8Eng, MS_U16* pu16OutPad, MS_BOOL bSet)
{
    //@TODO not implement
    return TRUE;
}


MS_BOOL HAL_TSO_GetInputTSIF_Status(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16* pu16Pad, MS_BOOL* pbClkInvert, MS_BOOL* pbExtSync, MS_BOOL* pbParl)
{
    MS_U16 u16Reg, u16RegShift;
    MS_U16 u16data = 0;
    REG16_TSO* reg16 = 0;

    // Set pad mux
    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO1_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO2_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO3_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE4:
            u16Reg = REG_TOP_TSO_MUX;
            u16RegShift = REG_TOP_TSO4_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE5:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegShift = REG_TOP_TSO5_MUX_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE6:
            u16Reg = REG_TOP_TSO1_MUX;
            u16RegShift = REG_TOP_TSO6_MUX_SHIFT;
            break;
        default:
            printf("Not support !!\n");
            return FALSE;
    }

    *pu16Pad = (TSP_TOP_REG(u16Reg) >> u16RegShift) & REG_TOP_TSO_MUX_MASK;

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u16data = TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF1_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16data = TSO_CLKGEN2_REG(REG_CLKGEN2_TSO1_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF2_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = TSO_CLKGEN2_REG(REG_CLKGEN2_TSO2_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF3_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE4:
            u16data = TSO_CLKGEN2_REG(REG_CLKGEN2_TSO3_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF4_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE5:
            u16data = TSO_CLKGEN2_REG(REG_CLKGEN2_TSO4_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF5_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE6:
            u16data = TSO_CLKGEN2_REG(REG_CLKGEN2_TSO5_IN) & REG_CLKGEN0_TSO_IN_MASK;
            reg16 = &(_TSOCtrl->CHANNEL0_IF6_CONFIG2);
            break;
    }

    *pbExtSync = ((_HAL_REG16_R(reg16) & TSO_CHCFG_EXT_SYNC_SEL) == TSO_CHCFG_EXT_SYNC_SEL);
    *pbParl = ((_HAL_REG16_R(reg16) & TSO_CHCFG_P_SEL) == TSO_CHCFG_P_SEL);
    *pbClkInvert = ((u16data & REG_CLKGEN0_TSO_IN_INVERT) == REG_CLKGEN0_TSO_IN_INVERT);

    return TRUE;
}

MS_BOOL HAL_TSO_Set_InClk(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16ClkSel, MS_BOOL bClkInvert, MS_BOOL bEnable)
{
    MS_BOOL isCLKGEN0 = FALSE;
    MS_U16  u16Reg;
    MS_U16  u16value = 0;

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            isCLKGEN0 = TRUE;
            u16Reg = REG_CLKGEN0_TSO_IN;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u16Reg = REG_CLKGEN2_TSO1_IN;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16Reg = REG_CLKGEN2_TSO2_IN;
            break;
        case HAL_TSO_TSIF_LIVE4:
            u16Reg = REG_CLKGEN2_TSO3_IN;
            break;
        case HAL_TSO_TSIF_LIVE5:
            u16Reg = REG_CLKGEN2_TSO4_IN;
            break;
        case HAL_TSO_TSIF_LIVE6:
            u16Reg = REG_CLKGEN2_TSO5_IN;
            break;
        default:
            printf("Not support !!\n");
            return FALSE;
    }

    if(u16ClkSel == 0xFFFF)
    {
        return FALSE;
    }

    if(isCLKGEN0)
    {
        u16value = TSO_CLKGEN0_REG(u16Reg) & ~REG_CLKGEN0_TSO_IN_MASK;
    }
    else
    {
        u16value = TSO_CLKGEN2_REG(u16Reg) & ~REG_CLKGEN0_TSO_IN_MASK;
    }

    if(!bEnable)
    {
        u16value |= REG_CLKGEN0_TSO_IN_DISABLE;
    }
    else
    {
    #ifndef CONFIG_MSTAR_CLKM
        u16value |= (u16ClkSel << REG_CLKGEN0_TSO_IN_SHIFT);
    #endif

        if(bClkInvert)
        {
            u16value |= REG_CLKGEN0_TSO_IN_INVERT;
        }
    }

    if(isCLKGEN0)
    {
        TSO_CLKGEN0_REG(u16Reg) = u16value;
    }
    else
    {
        TSO_CLKGEN2_REG(u16Reg) = u16value;
    }

#ifdef CONFIG_MSTAR_CLKM

    MS_S32 s32Handle;
    char   u8ClkSrcName[20] = "";
    MS_U8  u8Idx = u8TsIf - 1;

    switch(u16ClkSel)
    {
        case TSO_CLKIN_TS0:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD0", u8Idx);
            break;
        case TSO_CLKIN_TS1:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD1", u8Idx);
            break;
        case TSO_CLKIN_TS2:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD2", u8Idx);
            break;
        case TSO_CLKIN_TS3:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD3", u8Idx);
            break;
        case TSO_CLKIN_TS4:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD4", u8Idx);
            break;
        case TSO_CLKIN_TS5:
            sprintf(u8ClkSrcName, "CLK_TSOIN%u_PAD5", u8Idx);
            break;
        default:
            printf("[%s][%d] Not support !!\n", __FUNCTION__, __LINE__);
            return FALSE;
    }

    switch(u8TsIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        case HAL_TSO_TSIF_LIVE2:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso1_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        case HAL_TSO_TSIF_LIVE3:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso2_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        case HAL_TSO_TSIF_LIVE4:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso3_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        case HAL_TSO_TSIF_LIVE5:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso4_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        case HAL_TSO_TSIF_LIVE6:
            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso5_in");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcName);
            break;
        default:
            printf("Not support !!\n");
            return FALSE;
    }

#endif

    return TRUE;
}

#if 0
#define NPM_REG_ANAMISC                         0x10C00 //0x110c00 //ana misc
#define NPM_REG_MIPS_PLLCLK                     (NPM_REG_ANAMISC+0x0002)
static MS_U32  _HAL_TSO_CPU_QueryClock(void)
{
    MS_U32 u32Count = 0;
    MS_U32 u32Speed = 0;
    //here we assum that _u32TSORegBase is the same as non-PM bank
    u32Speed = (MS_U32)((volatile MS_U16*)(_u32TSORegBase))[NPM_REG_MIPS_PLLCLK];
    u32Count = ((u32Speed & 0xFF00) >> 8) * 12000000;

    return u32Count;
}

static void _HAL_TSO_Delay(MS_U32 u32Us)
{
    MS_U32 u32CPUClk = _HAL_TSO_CPU_QueryClock();
    register MS_U32 u32Loop = (((u32CPUClk/1000000)/3)*(u32Us));// 3 cycles / loop
    while(u32Loop--);
}
#endif

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
        /*
        case HAL_TSOIN_MUX_TSDEMOD0:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD;
            break;
        */
        case HAL_TSOIN_MUX_MEM:
        case HAL_TSOIN_MUX_MEM1:
            pstOutClkSet->u16OutClk = HAL_TSO_OUT_SEL_TSO_OUT_DIV2N;
            pstOutClkSet->u16OutDivSrc = HAL_TSO_OUT_DIV_SEL_172M_2N;
            pstOutClkSet->u16OutDivNum = 0x0F; //default: 172.8/2*(15+1) = 5.4M
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


MS_BOOL HAL_TSO_TSOOutDiv(MS_U8 u8Eng, MS_U16 *pu16ClkOutDivSrcSel, MS_U16 *pu16ClkOutDivNum, MS_BOOL bSet)
{
    //clock source for clock divide
    if(bSet == TRUE)
    {
        /*
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & ~REG_CLKGEN0_TSO_TRACE_MASK) | REG_CLKGEN0_TSO_TRACE_216M;
        */

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & ~REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK) | (*pu16ClkOutDivSrcSel);

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) & ~REG_CLKGEN0_TSO_OUT_DIVNUM_MASK) | (*pu16ClkOutDivNum << REG_CLKGEN0_TSO_OUT_DIVNUM_SHIFT);
    }
    else
    {
        *pu16ClkOutDivSrcSel = TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & REG_CLKGEN0_TSO_OUT_DIV_SEL_MASK;
        *pu16ClkOutDivNum = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) & REG_CLKGEN0_TSO_OUT_DIVNUM_MASK) >> REG_CLKGEN0_TSO_OUT_DIVNUM_SHIFT;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_PreTsoOutClk(MS_U8 u8Eng, MS_U16* pu16PreTsoOutSel, MS_BOOL bSet)
{
    if(bSet == TRUE)
    {
        if(*pu16PreTsoOutSel > HAL_PRE_TSO_OUT_SEL_TS6IN)
        {
            return FALSE;
        }

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) =
            (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) & ~REG_CLKGEN_TSO_P_TSO_OUT_MASK) | ((*pu16PreTsoOutSel) << REG_CLKGEN_TSO_P_TSO_OUT_SHIFT);
    }
    else
    {
        *pu16PreTsoOutSel = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_DIVNUM) & REG_CLKGEN_TSO_P_TSO_OUT_MASK) >> REG_CLKGEN_TSO_P_TSO_OUT_SHIFT;
    }

    return TRUE;
}

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
            case HAL_TSO_OUT_SEL_TSO_OUT_DIV:
                HAL_TSO_TSOOutDiv(u8Eng, &(pstOutClkSet->u16OutDivSrc), &(pstOutClkSet->u16OutDivNum), TRUE);
                break;
            case HAL_TSO_OUT_SEL_TSO_OUT_27MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_62MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_86MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_108MHz:
            case HAL_TSO_OUT_SEL_TSO_OUT_123MHz:
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
        HAL_TSO_OutClk(u8Eng, &(pstOutClkSet->u16OutClk), &(pstOutClkSet->bClkInvert), &(pstOutClkSet->bEnable), TRUE); //always need TSO out clock
    }
    else
    {
        HAL_TSO_OutClk(u8Eng, &(pstOutClkSet->u16OutClk), &(pstOutClkSet->bClkInvert), &(pstOutClkSet->bEnable), FALSE);
        if(pstOutClkSet->u16OutClk == HAL_TSO_OUT_SEL_TSO_OUT_DIV)
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
    if(u8Eng == 0)
    {
        if(!bPhaseEnable)
        {
            TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) &= ~REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE;
        }
        else
        {
            u16value = (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) & ~REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_MASK)
                        | (u16ClkOutPhase << REG_CLKGEN0_TSO_OUT_PH_TUN_NUM_SHIFT);

            TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) = u16value;
            TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_PHASE) |= REG_CLKGEN0_TSO_OUT_PHASE_TUN_ENABLE;
        }
    }
    else
    {
        return FALSE;
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
            /*
            TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) =
                (TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) & ~REG_CLKGEN0_TSO_TRACE_MASK) | REG_CLKGEN0_TSO_TRACE_216M;
            */
        #ifndef CONFIG_MSTAR_CLKM
            u16Clk |= (*pu16ClkOutSel << REG_CLKGEN0_TSO_OUT_CLK_SRC_SHIFT);
        #endif

            if(*pbClkInvert)
                u16Clk |= REG_CLKGEN0_TSO_OUT_CLK_INVERT;
        }

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) = u16Clk;

        #ifdef CONFIG_MSTAR_CLKM

            MS_S32 s32Handle;
            MS_U8  u8NameIdx = 0;
            char* u8ClkSrcNames[] =
            {
                "CLK_TSOOUT_27M",
                "CLK_TSOOUT_62M",
                "CLK_TSOOUT_108M",
                "CLK_TSOOUT_FROMPAD",
                "CLK_TSOOUT_DIV8",
                "CLK_TSOOUT_DIV",
                "CLK_TSOOUT_86M",
                "CLK_TSOOUT_123M"
            };

            switch(*pu16ClkOutSel)
            {
                case HAL_TSO_OUT_SEL_TSO_OUT_27MHz:
                    u8NameIdx = 0;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_62MHz:
                    u8NameIdx = 1;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_108MHz:
                    u8NameIdx = 2;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT:
                    u8NameIdx = 3;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8:
                    u8NameIdx = 4;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_DIV:
                    u8NameIdx = 5;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_86MHz:
                    u8NameIdx = 6;
                    break;
                case HAL_TSO_OUT_SEL_TSO_OUT_123MHz:
                    u8NameIdx = 7;
                    break;
                default:
                    printf("[%s][%d] Not support !!\n", __FUNCTION__, __LINE__);
                    return FALSE;
            }

            s32Handle = Drv_Clkm_Get_Handle("g_clk_tso_out");
            Drv_Clkm_Set_Clk_Source(s32Handle, u8ClkSrcNames[u8NameIdx]);

        #endif
    }
    else
    {
        *pbEnable = ((u16Clk & REG_CLKGEN0_TSO_OUT_CLK_DISABLE) == 0);
        *pbClkInvert = ((u16Clk & REG_CLKGEN0_TSO_OUT_CLK_INVERT) == REG_CLKGEN0_TSO_OUT_CLK_INVERT);
        *pu16ClkOutSel = u16Clk >> REG_CLKGEN0_TSO_OUT_CLK_SRC_SHIFT;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_Set_PidBypass(MS_U8 u8Eng, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u8Eng)
        {
            case HAL_TSO_TSIF_LIVE1:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF1_CONFIG2),TSO_CHCFG_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE2:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF2_CONFIG2),TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE3:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF3_CONFIG2),TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE4:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF4_CONFIG2),TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE5:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF5_CONFIG2),TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE6:
                _REG16_SET(&(_TSOCtrl->CHANNEL0_IF6_CONFIG2),TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_REC_ALL);
                break;
            default:
                printf("Not support !!\n");
                break;
        }
    }
    else
    {
        switch(u8Eng)
        {
            case HAL_TSO_TSIF_LIVE1:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF1_CONFIG2),TSO_CHCFG_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE2:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF2_CONFIG2),TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE3:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF3_CONFIG2),TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE4:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF4_CONFIG2),TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE5:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF5_CONFIG2),TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_REC_ALL);
                break;
            case HAL_TSO_TSIF_LIVE6:
                _REG16_CLR(&(_TSOCtrl->CHANNEL0_IF6_CONFIG2),TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_REC_ALL);
                break;
            default:
                printf("Not support !!\n");
                break;
        }
    }

    return TRUE;
}

// ------------------------------------------------------
//  APIS
//-------------------------------------------------------
#define TSO_PIDFLT_ADDR(FltId)  (REG_PIDFLT_BASE + ((FltId) << 2))

static void _HAL_TSO_PageTableIndW(MS_U32 u32Addr, MS_U16 u16Wdata)
{
    // addr
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_ADDR_L), (u32Addr & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_ADDR_H), (u32Addr >> 16));

    // Wdata
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_WDATA_L), u16Wdata);
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_WDATA_H), 0x0000);

    // Wen
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_EN), TSO_PDTABLE_W_EN);
}

static MS_U16 _HAL_TSO_PageTableIndR(MS_U32 u32Addr)
{
    // addr
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_ADDR_L), (u32Addr & 0x0000FFFF));
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_ADDR_H), (u32Addr >> 16));

    // Ren
    _HAL_REG16_W(&(_TSOCtrl->PDTABLE_EN), TSO_PDTABLE_R_EN);

    // Rdata
    return _HAL_REG16_R(&(_TSOCtrl->PDTABLE_RDATA));
}

void HAL_TSO_Flt_SetPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16PID)
{
    MS_U16 u16Temp = 0;

    u16Temp = _HAL_TSO_PageTableIndR(TSO_PIDFLT_ADDR(u16FltId));
    u16Temp = (u16Temp & (TSO_PIDFLT_IN_MASK << TSO_PIDFLT_IN_SHIFT)) | (u16PID & TSO_PIDFLT_PID_MASK);
    _HAL_TSO_PageTableIndW(TSO_PIDFLT_ADDR(u16FltId), u16Temp);
}

void HAL_TSO_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32Addr)
{
    REG32_TSO *FileInRaddr = (u8FileEng == 0)? &(_TSOCtrl->CFG_TSO_60_63[0]) : &(_TSOCtrl->CFG_TSO_65_68[0]);

    _HAL_REG32_W(FileInRaddr, u32Addr);
}

void HAL_TSO_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32len)
{
    REG32_TSO *FileInRlen = (u8FileEng == 0)? &(_TSOCtrl->CFG_TSO_60_63[1]) : &(_TSOCtrl->CFG_TSO_65_68[1]);

    _HAL_REG32_W(FileInRlen, u32len);
}

MS_U32 HAL_TSO_Get_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U32 u32temp = 0;
    REG32_TSO *TSO2MI_RADDR = (u8FileEng == 0)? &(_TSOCtrl->TSO2MI_RADDR) : &(_TSOCtrl->TSO2MI_RADDR1);
    REG16_TSO *FILE_CONFIG = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);


    _REG16_SET(FILE_CONFIG,TSO_FILE_CONFIG_TSO_RADDR_READ);
    u32temp = (_HAL_REG32_R(TSO2MI_RADDR) << TSO_MIU_BUS);
    _REG16_CLR(FILE_CONFIG,TSO_FILE_CONFIG_TSO_RADDR_READ);

    return u32temp;
}

void HAL_TSO_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16ctrl)
{
    REG16_TSO *FileinCtrl = (u8FileEng == 0)? &(_TSOCtrl->TSO_Filein_Ctrl) : &(_TSOCtrl->TSO_Filein_Ctrl1);

    _HAL_REG16_W(FileinCtrl, (_HAL_REG16_R(FileinCtrl) & ~TSO_FILEIN_CTRL_MASK) | u16ctrl);
}

MS_U16 HAL_TSO_Get_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    REG16_TSO *FileinCtrl = (u8FileEng == 0)? &(_TSOCtrl->TSO_Filein_Ctrl) : &(_TSOCtrl->TSO_Filein_Ctrl1);

    return (_HAL_REG16_R(FileinCtrl) & TSO_FILEIN_CTRL_MASK);
}

MS_BOOL HAL_TSO_Filein_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    MS_U16 u16ChIf = (u8FileEng == 0)? TSO_CFG1_TSO_TSIF5_EN : TSO_CFG1_TSO_TSIF6_EN;

    HAL_TSO_Cfg1_Enable(0, u16ChIf, bEnable);
    HAL_TSO_File_Cfg1_Enable(u8FileEng, (TSO_FILE_CONFIG_TSO_FILE_IN|TSO_FILE_CONFIG_TSP_FILE_SEGMENT|TSO_FILE_CONFIG_TS_DATA_PORT_SEL), bEnable);

    return TRUE;
}

void HAL_TSO_FileinTimer_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    REG16_TSO *pReg = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);

    if(bEnable)
    {
        _HAL_REG16_W(pReg, _HAL_REG16_R(pReg) | TSO_FILE_CONFIG_TIMER_EN);
    }
    else
    {
        _HAL_REG16_W(pReg, _HAL_REG16_R(pReg) & ~TSO_FILE_CONFIG_TIMER_EN);
    }
}

void HAL_TSO_Filein_Rate(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16timer)
{
    _HAL_REG16_W(&(_TSOCtrl->FILE_TIMER[u8FileEng]), u16timer);
}

void HAL_TSO_Filein_192Mode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    REG16_TSO *FILE_CONFIG = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);

    if(bEnable)
    {
        _HAL_REG16_W(FILE_CONFIG, (_HAL_REG16_R(FILE_CONFIG) | TSO_FILE_CONFIG_PKT_192_EN));
    }
    else
    {
        _HAL_REG16_W(FILE_CONFIG, (_HAL_REG16_R(FILE_CONFIG) & ~TSO_FILE_CONFIG_PKT_192_EN));
    }
}

void HAL_TSO_Filein_192BlockMode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable)
{
    REG16_TSO *FILE_CONFIG = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);

    if(bEnable)
    {
        _HAL_REG16_W(FILE_CONFIG, (_HAL_REG16_R(FILE_CONFIG) & ~TSO_FILE_CONFIG_PKT_192_BLK_DISABLE));
    }
    else
    {
        _HAL_REG16_W(FILE_CONFIG, (_HAL_REG16_R(FILE_CONFIG) | TSO_FILE_CONFIG_PKT_192_BLK_DISABLE));
    }
}

MS_U16 HAL_TSO_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Mask = (u8FileEng == 0)? TSO_CMD_QUEUE_STATUS_CMD_WR_COUNT_MASK : TSO_CMD_QUEUE_STATUS1_CMD_WR_COUNT_MASK;
    MS_U16 u16Shift = (u8FileEng == 0)? TSO_CMD_QUEUE_STATUS_CMD_WR_COUNT_SHIFT : TSO_CMD_QUEUE_STATUS1_CMD_WR_COUNT_SHIFT;

    return ((_HAL_REG16_R(&(_TSOCtrl[u8Eng].CMD_QUEUE_STATUS)) & u16Mask) >> u16Shift);
}

MS_BOOL HAL_TSO_CmdQ_FIFO_IsFull(MS_U8 u8Eng)
{
    //@TODO not implement
    //return (MS_BOOL)(_HAL_REG16_R(&(_TSOCtrl[u8Eng].CmdQSts)) & TSO_CMDQ_STS_FIFO_FULL);
    return FALSE;
}

MS_BOOL HAL_TSO_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng)
{
    //@TODO not implement
    //return (MS_BOOL)(_HAL_REG16_R(&(_TSOCtrl[u8Eng].CmdQSts)) & TSO_CMDQ_STS_FIFO_EMPTY);
    return FALSE;
}

MS_U8 HAL_TSO_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16Mask = (u8FileEng == 0)? TSO_CMD_QUEUE_STATUS_CMD_WR_LEVEL_MASK : TSO_CMD_QUEUE_STATUS1_CMD_WR_LEVEL_MASK;
    MS_U16 u16Shift = (u8FileEng == 0)? TSO_CMD_QUEUE_STATUS_CMD_WR_LEVEL_SHIFT : TSO_CMD_QUEUE_STATUS1_CMD_WR_LEVEL_SHIFT;

    return (MS_U8)((_HAL_REG16_R(&(_TSOCtrl[u8Eng].CMD_QUEUE_STATUS)) & u16Mask) >> u16Shift);
}

MS_BOOL HAL_TSO_CmdQ_Reset(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U16 u16data = ((u8FileEng == 0)? TSO_SW_RST_CMDQ : TSO_SW_RST_CMDQ1);

    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), SET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)), u16data));
    _HAL_REG16_W(&(_TSOCtrl->SW_RSTZ), RESET_FLAG1(_HAL_REG16_R(&(_TSOCtrl->SW_RSTZ)), u16data));

    return TRUE;
}

void HAL_TSO_Cfg0_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable)
{
    //@TODO not implement
    /*
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_CFG0));

    if(benable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl[u8Eng].TSO_CFG0), u16data);
    */
}

void HAL_TSO_Cfg1_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl->TSO_CONFIG1));

    if(benable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl->TSO_CONFIG1), (u16data | TSO_CFG1_PKT_PARAM_LD));
    _REG16_CLR(&(_TSOCtrl->TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD);
}

MS_BOOL HAL_TSO_ChIf_ClrByteCnt(MS_U8 u8Eng, MS_U8 u8ChIf)
{
    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_1);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_1);
            break;
        case HAL_TSO_TSIF_LIVE2:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_2);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_3);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_3);
            break;
        case HAL_TSO_TSIF_LIVE4:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_4);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_4);
            break;
        case HAL_TSO_TSIF_LIVE5:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_5);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_5);
            break;
        case HAL_TSO_TSIF_LIVE6:
            _REG16_SET(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_6);
            _REG16_CLR(&(_TSOCtrl[u8Eng].CLR_BYTE_CNT),TSO_CLR_BYTE_CNT_6);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_ChIf_Enable(MS_U8 u8Eng, MS_U8 u8ChIf, MS_BOOL bEnable)
{
    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF1_EN, bEnable);
            break;
        case HAL_TSO_TSIF_LIVE2:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF2_EN, bEnable);
            break;
        case HAL_TSO_TSIF_LIVE3:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF3_EN, bEnable);
            break;
        case HAL_TSO_TSIF_LIVE4:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF4_EN, bEnable);
            break;
        case HAL_TSO_TSIF_LIVE5:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF5_EN, bEnable);
            break;
        case HAL_TSO_TSIF_LIVE6:
            HAL_TSO_Cfg1_Enable(u8Eng, TSO_CFG1_TSO_TSIF6_EN, bEnable);
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSO_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16 u16Cfg, MS_BOOL bEnable)
{
    REG16_TSO *reg = NULL;
    MS_U16 u16data = 0;

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            reg = &(_TSOCtrl->CHANNEL0_IF1_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            reg = &(_TSOCtrl->CHANNEL0_IF2_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            reg = &(_TSOCtrl->CHANNEL0_IF3_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE4:
            reg = &(_TSOCtrl->CHANNEL0_IF4_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE5:
            reg = &(_TSOCtrl->CHANNEL0_IF5_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE6:
            reg = &(_TSOCtrl->CHANNEL0_IF6_CONFIG2);
            break;
        default:
            return FALSE;
    }

    u16data = _HAL_REG16_R(reg);

    if(bEnable)
    {
        u16data |= u16Cfg;
    }
    else
    {
        u16data &= ~u16Cfg;
    }

    _HAL_REG16_W(reg, u16data);
    return TRUE;
}

MS_BOOL HAL_TSO_Get_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Cfg, MS_BOOL *pbEnable)
{
    REG16_TSO* pReg = NULL;
    MS_U16 u16data = 0;

    *pu16Cfg = 0;
    *pbEnable = FALSE;

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            pReg = &(_TSOCtrl->CHANNEL0_IF1_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE2:
            pReg = &(_TSOCtrl->CHANNEL0_IF2_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE3:
            pReg = &(_TSOCtrl->CHANNEL0_IF3_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE4:
            pReg = &(_TSOCtrl->CHANNEL0_IF4_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE5:
            pReg = &(_TSOCtrl->CHANNEL0_IF5_CONFIG2);
            break;
        case HAL_TSO_TSIF_LIVE6:
            pReg = &(_TSOCtrl->CHANNEL0_IF6_CONFIG2);
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
            u16data = TSO_CFG1_TSO_TSIF2_EN;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u16data = TSO_CFG1_TSO_TSIF3_EN;
            break;
        case HAL_TSO_TSIF_LIVE4:
            u16data = TSO_CFG1_TSO_TSIF4_EN;
            break;
        case HAL_TSO_TSIF_LIVE5:
            u16data = TSO_CFG1_TSO_TSIF5_EN;
            break;
        case HAL_TSO_TSIF_LIVE6:
            u16data = TSO_CFG1_TSO_TSIF6_EN;
            break;
    }

    *pbEnable = ((_HAL_REG16_R(&(_TSOCtrl->TSO_CONFIG1)) & u16data) == u16data);

    return TRUE;
}

void HAL_TSO_File_Cfg1_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable)
{
    REG16_TSO *reg = (u8Eng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);
    MS_U16 u16data = _HAL_REG16_R(reg);

    if(benable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(reg, u16data);
}



void HAL_TSO_Cfg4_Enable(MS_U8 u8Eng, MS_U32 u32CfgItem, MS_BOOL benable)
{
    //@TODO not implement
    /*
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_CFG4));

    if(benable)
    {
        u16data |= u32CfgItem;
    }
    else
    {
        u16data &= ~u32CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl[u8Eng].TSO_CFG4), u16data);
    */
}

void HAL_TSO_RW_ValidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16ValidBlockCnt)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_CONFIG2));

    if(bWrite)
    {
        u16data &= ~TSO_CONFIG2_VALID_BYTE_CNT_MASK;
        u16data |= (*pu16ValidBlockCnt << TSO_CONFIG2_VALID_BYTE_CNT_SHIFT);
        _HAL_REG16_W(&(_TSOCtrl[u8Eng].TSO_CONFIG2), u16data);

        _REG16_SET(&(_TSOCtrl[u8Eng].TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD);
        _REG16_CLR(&(_TSOCtrl[u8Eng].TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD);
    }
    else
    {
        *pu16ValidBlockCnt = ((u16data & TSO_CONFIG2_VALID_BYTE_CNT_MASK) >> TSO_CONFIG2_VALID_BYTE_CNT_SHIFT);
    }
}

void HAL_TSO_RW_InvalidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16InvalidBlockCnt)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_CONFIG2));

    if(bWrite)
    {
        u16data &= ~TSO_CONFIG2_INVALID_BYTE_CNT_MASK;
        u16data |= (*pu16InvalidBlockCnt << TSO_CONFIG2_INVALID_BYTE_CNT_SHIFT);
        _HAL_REG16_W(&(_TSOCtrl[u8Eng].TSO_CONFIG2), u16data);

        _REG16_SET(&(_TSOCtrl[u8Eng].TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD);
        _REG16_CLR(&(_TSOCtrl[u8Eng].TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD);
    }
    else
    {
        *pu16InvalidBlockCnt = (u16data & TSO_CONFIG2_INVALID_BYTE_CNT_MASK) >> TSO_CONFIG2_INVALID_BYTE_CNT_SHIFT;
    }
}

MS_U32 HAL_TSO_Get_TsHDInfo_CC(MS_U8 u8Eng)
{
    //@TODO not implement
    //return (_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_CONT_COUNTER_MASK);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_ADP(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_CONT_COUNTER_MASK) >> TSO_HD_ADP_FIELD_SHIFT);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_SCM(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_PID_MASK) >> TSO_HD_PID_SHIFT);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_PID(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_SCRM_FIELD_MASK) >> TSO_HD_SCRM_FIELD_SHIFT);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_TSPri(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_TS_PRIORITY_MASK) >> TSO_HD_TS_PRIORITY_SHIFT);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_PSI(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_PAYLOAD_START_IND_MASK) >> TSO_HD_PAYLOAD_START_IND_SHIFT);
    return 0;
}

MS_U32  HAL_TSO_Get_TsHDInfo_ErrInd(MS_U8 u8Eng)
{
    //@TODO not implement
    //return ((_HAL_REG32_R(&(_TSOCtrl[u8Eng].TSO_Header)) & TSO_HD_PAYLOAD_ERR_IND_MASK) >> TSO_HD_PAYLOAD_ERR_IND_SHIFT);
    return 0;
}

void   HAL_TSO_Get_Pid_Info(MS_U8 u8Eng, MS_U16 *pu16matchpid, MS_BOOL *pbchanged)
{
    //@TODO not implement
    /*
    MS_U16 u16temp;

    u16temp = _HAL_REG16_R(&(_TSOCtrl[u8Eng].TSO_PID_Info));

    *pu16matchpid = u16temp & TSO_PID_INFO_MATCH_PID_MASK;
    *pbchanged = (((u16temp & TSO_PID_INFO_MATCH_PID_CHANGE_MASK) > 0) ? TRUE: FALSE);
    */
}

void   HAL_TSO_LPcr2_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32lpcr2)
{
    REG32_TSO *LPCR2 = (u8FileEng == 0)? &(_TSOCtrl->LPCR2_BUF) : &(_TSOCtrl->LPCR2_BUF1);
    REG16_TSO *FILE_CONFIG = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);


    _REG16_SET(FILE_CONFIG,TSO_FILE_CONFIG_LPCR2_WLD);
    _HAL_REG32_W(LPCR2, u32lpcr2);
    _REG16_CLR(FILE_CONFIG,TSO_FILE_CONFIG_LPCR2_WLD);
}

MS_U32 HAL_TSO_LPcr2_Get(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    MS_U32 u32temp = 0;
    REG32_TSO *LPCR2 = (u8FileEng == 0)? &(_TSOCtrl->LPCR2_BUF) : &(_TSOCtrl->LPCR2_BUF1);
    REG16_TSO *FILE_CONFIG = (u8FileEng == 0)? &(_TSOCtrl->TSO_FILE_CONFIG) : &(_TSOCtrl->TSO_FILE_CONFIG1);


    _REG16_SET(FILE_CONFIG,TSO_FILE_CONFIG_LPCR2_LOAD);
    u32temp = _HAL_REG32_R(LPCR2);
    _REG16_CLR(FILE_CONFIG,TSO_FILE_CONFIG_LPCR2_LOAD);

    return u32temp;
}

MS_U32 HAL_TSO_TimeStamp_Get(MS_U8 u8Eng, MS_U8 u8FileEng)
{
    REG32_TSO *TIMESTAMP = (u8FileEng == 0)? &(_TSOCtrl->TIMESTAMP) : &(_TSOCtrl->TIMESTAMP1);

    return _HAL_REG32_R(TIMESTAMP);
}

void HAL_TSO_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8If, MS_U8 u8size)
{
    switch(u8If)
    {
        case HAL_TSO_TSIF_LIVE1:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF1_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF1_CONFIG0)) & ~TSO_CHANNEL0_IF1_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF1_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF1_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE2:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF2_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF2_CONFIG0)) & ~TSO_CHANNEL0_IF2_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF2_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF2_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE3:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF3_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF3_CONFIG0)) & ~TSO_CHANNEL0_IF3_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF3_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF3_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE4:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF4_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF4_CONFIG0)) & ~TSO_CHANNEL0_IF4_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF4_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF4_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE5:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF5_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF5_CONFIG0)) & ~TSO_CHANNEL0_IF5_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF5_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF5_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE6:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF6_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF6_CONFIG0)) & ~TSO_CHANNEL0_IF6_CONFIG0_PIDFLT_PKT_SIZE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF6_CONFIG0_PIDFLT_PKT_SIZE_SHIFT)) & TSO_CHANNEL0_IF6_CONFIG0_PIDFLT_PKT_SIZE_MASK));
            break;
        default:
            printf("Not support !!\n");
            break;
    }
}

void HAL_TSO_RW_OutputPktSize(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16PktSize)
{
    if(bWrite)
    {
        _HAL_REG16_W(&(_TSOCtrl->TSO_CONFIG3), *pu16PktSize);
    }
    else
    {
        *pu16PktSize = _HAL_REG16_R(&(_TSOCtrl->TSO_CONFIG3));
    }

    _HAL_REG16_W(&(_TSOCtrl->TSO_CONFIG1), SET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD));
    _HAL_REG16_W(&(_TSOCtrl->TSO_CONFIG1), RESET_FLAG1(_HAL_REG16_R(&_TSOCtrl->TSO_CONFIG1), TSO_CFG1_PKT_PARAM_LD));
}

void HAL_TSO_Filein_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U8 u8size)
{
    switch(u8FileEng)
    {
        case 0:
            _HAL_REG16_W(&(_TSOCtrl->PKT_CHK_SIZE_FIN),
                          (_HAL_REG16_R(&(_TSOCtrl->PKT_CHK_SIZE_FIN)) & ~TSO_PKT_CHK_SIZE_FIN_MASK)
                         |(((MS_U16)(u8size << TSO_PKT_CHK_SIZE_FIN_SHIFT)) & TSO_PKT_CHK_SIZE_FIN_MASK));
            break;
        case 1:
            _HAL_REG16_W(&(_TSOCtrl->PKT_CHK_SIZE_FIN),
                          (_HAL_REG16_R(&(_TSOCtrl->PKT_CHK_SIZE_FIN)) & ~TSO_PKT_CHK_SIZE_FIN1_MASK)
                         |(((MS_U16)(u8size << TSO_PKT_CHK_SIZE_FIN1_SHIFT)) & TSO_PKT_CHK_SIZE_FIN1_MASK));
            break;
        default:
            printf("Not support !!\n");
            break;
    }
}

void HAL_TSO_Livein_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8If, MS_U8 u8size)
{
    switch(u8If)
    {
        case HAL_TSO_TSIF_LIVE1:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF1_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF1_CONFIG0)) & ~TSO_CHANNEL0_IF1_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF1_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF1_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE2:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF2_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF2_CONFIG0)) & ~TSO_CHANNEL0_IF2_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF2_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF2_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE3:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF3_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF3_CONFIG0)) & ~TSO_CHANNEL0_IF3_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF3_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF3_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE4:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF4_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF4_CONFIG0)) & ~TSO_CHANNEL0_IF4_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF4_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF4_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE5:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF5_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF5_CONFIG0)) & ~TSO_CHANNEL0_IF5_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF5_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF5_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        case HAL_TSO_TSIF_LIVE6:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF6_CONFIG0),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF6_CONFIG0)) & ~TSO_CHANNEL0_IF6_CONFIG0_PKT_SIZE_CHK_LIVE_MASK)
                         |(((MS_U16)(u8size << TSO_CHANNEL0_IF6_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT)) & TSO_CHANNEL0_IF6_CONFIG0_PKT_SIZE_CHK_LIVE_MASK));
            break;
        default:
            printf("Not support !!\n");
            break;
    }
}


void HAL_TSO_Livein_Input_Config(MS_U8 u8Eng, MS_U8 u8PktInputMode, MS_U8 u8PktSyncByte, MS_U8 u8PktHeaderLength)
{
    switch(u8Eng)
    {
        case HAL_TSO_TSIF_LIVE1:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1)) & ~TSO_CHANNEL0_IF1_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF1_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF1_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1)) & ~TSO_CHANNEL0_IF1_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF1_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF1_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF1_CONFIG1)) & ~TSO_CHANNEL0_IF1_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF1_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF1_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        case HAL_TSO_TSIF_LIVE2:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1)) & ~TSO_CHANNEL0_IF2_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF2_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF2_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1)) & ~TSO_CHANNEL0_IF2_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF2_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF2_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF2_CONFIG1)) & ~TSO_CHANNEL0_IF2_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF2_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF2_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        case HAL_TSO_TSIF_LIVE3:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1)) & ~TSO_CHANNEL0_IF3_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF3_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF3_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1)) & ~TSO_CHANNEL0_IF3_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF3_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF3_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF3_CONFIG1)) & ~TSO_CHANNEL0_IF3_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF3_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF3_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        case HAL_TSO_TSIF_LIVE4:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1)) & ~TSO_CHANNEL0_IF4_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF4_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF4_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1)) & ~TSO_CHANNEL0_IF4_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF4_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF4_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF4_CONFIG1)) & ~TSO_CHANNEL0_IF4_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF4_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF4_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        case HAL_TSO_TSIF_LIVE5:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1)) & ~TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1)) & ~TSO_CHANNEL0_IF5_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF5_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF5_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF5_CONFIG1)) & ~TSO_CHANNEL0_IF5_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF5_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF5_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        case HAL_TSO_TSIF_LIVE6:
            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1)) & ~TSO_CHANNEL0_IF6_CONFIG1_PKT_INPUT_MODE_MASK)
                         |(((MS_U16)(u8PktInputMode << TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_SHIFT)) & TSO_CHANNEL0_IF6_CONFIG1_PKT_INPUT_MODE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1)) & ~TSO_CHANNEL0_IF6_CONFIG1_SYNC_BYTE_MASK)
                         |(((MS_U16)(u8PktSyncByte << TSO_CHANNEL0_IF6_CONFIG1_SYNC_BYTE_SHIFT)) & TSO_CHANNEL0_IF6_CONFIG1_SYNC_BYTE_MASK));

            _HAL_REG16_W(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1),
                          (_HAL_REG16_R(&(_TSOCtrl->CHANNEL0_IF6_CONFIG1)) & ~TSO_CHANNEL0_IF6_CONFIG1_PKT_HEADER_LEN_MASK)
                         |(((MS_U16)(u8PktHeaderLength << TSO_CHANNEL0_IF6_CONFIG1_PKT_HEADER_LEN_SHIFT)) & TSO_CHANNEL0_IF6_CONFIG1_PKT_HEADER_LEN_MASK));
            break;
        default:
            printf("Not support !!\n");
            break;
    }
}


void HAL_TSO_Livein_3Wire(MS_U8 u8Eng, MS_U8 u8ChIf, MS_BOOL bEnable)
{
    if(bEnable)
    {
        switch(u8ChIf)
        {
            case HAL_TSO_TSIF_LIVE1:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_1);
                break;
            case HAL_TSO_TSIF_LIVE2:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_2);
                break;
            case HAL_TSO_TSIF_LIVE3:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_3);
                break;
            case HAL_TSO_TSIF_LIVE4:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_4);
                break;
            case HAL_TSO_TSIF_LIVE5:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_5);
                break;
            case HAL_TSO_TSIF_LIVE6:
                _REG16_SET(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_6);
                break;
            default:
                HAL_TSO_DBGMSG(E_HAL_TSO_DBG_LEVEL_ERR, E_HAL_TSO_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] ChIf not support !!\n",__FUNCTION__,__LINE__));
                break;
        }
    }
    else
    {
        switch(u8ChIf)
        {
            case HAL_TSO_TSIF_LIVE1:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_1);
                break;
            case HAL_TSO_TSIF_LIVE2:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_2);
                break;
            case HAL_TSO_TSIF_LIVE3:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_3);
                break;
            case HAL_TSO_TSIF_LIVE4:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_4);
                break;
            case HAL_TSO_TSIF_LIVE5:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_5);
                break;
            case HAL_TSO_TSIF_LIVE6:
                _REG16_CLR(&(_TSOCtrl->TSO_CONFIG5),TSO_CONFIG5_3_WIRE_EN_6);
                break;
            default:
                HAL_TSO_DBGMSG(E_HAL_TSO_DBG_LEVEL_ERR, E_HAL_TSO_DBG_MODEL_ALL, printf("[PVR ERROR][%s][%d] ChIf not support !!\n",__FUNCTION__,__LINE__));
                break;
        }

    }
}

void HAL_TSO_TsioMode_En(MS_BOOL bEnable)
{
    //dummy function for make
    return;
}

void HAL_TSO_SerialMode_En(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _REG16_SET(&(_TSOCtrl->TSO_CONFIG1),TSO_CONFIG1_SERIAL_OUT_EN);
    }
    else
    {
        _REG16_CLR(&(_TSOCtrl->TSO_CONFIG1),TSO_CONFIG1_SERIAL_OUT_EN);
    }
}

MS_BOOL HAL_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_U8 u8ChIf, MS_PHY phyBufAddr, MS_U32 u32BufSize)
{
    REG32_TSO *Base = NULL;
    REG16_TSO *Size = NULL, *TX_Config = NULL;


    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            Base = &(_TSOCtrl1->SVQ1_BASE);
            Size = &(_TSOCtrl1->SVQ1_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ1_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE2:
            Base = &(_TSOCtrl1->SVQ2_BASE);
            Size = &(_TSOCtrl1->SVQ2_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ2_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE3:
            Base = &(_TSOCtrl1->SVQ3_BASE);
            Size = &(_TSOCtrl1->SVQ3_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ3_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE4:
            Base = &(_TSOCtrl1->SVQ4_BASE);
            Size = &(_TSOCtrl1->SVQ4_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ4_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE5:
            Base = &(_TSOCtrl1->SVQ5_BASE);
            Size = &(_TSOCtrl1->SVQ5_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ5_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE6:
            Base = &(_TSOCtrl1->SVQ6_BASE);
            Size = &(_TSOCtrl1->SVQ6_SIZE_200BYTE);
            TX_Config = &(_TSOCtrl1->SVQ6_TX_CONFIG);
            break;
        default:
            printf("Not support !!\n");
            return FALSE;
    }

    _HAL_REG32_W(Base, ((phyBufAddr >> TSO_MIU_BUS) & TSO1_SVQ1_BASE_MASK));
    _HAL_REG16_W(Size, ((u32BufSize / TSO_SVQ_UNIT_SIZE) & TSO1_SVQ1_SIZE_200BYTE_SVQ_SIZE_MASK));
    _REG16_SET(TX_Config, TSO1_SVQ1_TX_CONFIG_TX_RESET);
    _REG16_CLR(TX_Config, TSO1_SVQ1_TX_CONFIG_TX_RESET);
    _REG16_SET(TX_Config, TSO1_SVQ1_TX_CONFIG_SVQ_TX_ENABLE);

    return TRUE;
}

MS_BOOL HAL_TSO_SVQ_TX_Reset(MS_U8 u8Eng, MS_U8 u8ChIf)
{
    REG16_TSO* p16Reg = NULL;

    if(u8Eng > 0)
    {
        return FALSE;
    }

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            p16Reg = &(_TSOCtrl1->SVQ1_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE2:
            p16Reg = &(_TSOCtrl1->SVQ2_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE3:
            p16Reg = &(_TSOCtrl1->SVQ3_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE4:
            p16Reg = &(_TSOCtrl1->SVQ4_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE5:
            p16Reg = &(_TSOCtrl1->SVQ5_TX_CONFIG);
            break;
        case HAL_TSO_TSIF_LIVE6:
            p16Reg = &(_TSOCtrl1->SVQ6_TX_CONFIG);
            break;
	default:
            printf("Not support !!\n");
            return FALSE;
    }

    _HAL_REG16_W(p16Reg , SET_FLAG1(_HAL_REG16_R(p16Reg), TSO1_SVQ1_TX_CONFIG_TX_RESET));
    _HAL_REG16_W(p16Reg , RESET_FLAG1(_HAL_REG16_R(p16Reg), TSO1_SVQ1_TX_CONFIG_TX_RESET));
    return TRUE;
}

void HAL_TSO1_SVQ_Rx_Enable(MS_U16 u16CfgItem, MS_BOOL benable)
{
    MS_U16 u16data = _HAL_REG16_R(&(_TSOCtrl1->SVQ_RX_CONFIG));

    if(benable)
    {
        u16data |= u16CfgItem;
    }
    else
    {
        u16data &= ~u16CfgItem;
    }

    _HAL_REG16_W(&(_TSOCtrl1->SVQ_RX_CONFIG), u16data);
}

MS_BOOL HAL_TSO_Set_SVQRX_PktMode(MS_U8 u8Eng, MS_U16 u16mode)
{
    _HAL_REG16_W(&(_TSOCtrl1->SVQ_RX_CONFIG), (_HAL_REG16_R(&(_TSOCtrl1->SVQ_RX_CONFIG)) & ~TSO1_SVQ_RX_CONFIG_MODE_MASK) | u16mode);

    return TRUE;
}

MS_BOOL HAL_TSO_Get_SVQ_Status(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Status)
{
    MS_U32 u32data = 0;
    MS_U32 u32Shift = 0;

    *pu16Status = 0;
    u32data = _HAL_REG32_R(&(_TSOCtrl1->SVQ_STATUS));

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            u32Shift = TSO_SVQ1_STS_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE2:
            u32Shift = TSO_SVQ2_STS_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE3:
            u32Shift = TSO_SVQ3_STS_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE4:
            u32Shift = TSO_SVQ4_STS_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE5:
            u32Shift = TSO_SVQ5_STS_SHIFT;
            break;
        case HAL_TSO_TSIF_LIVE6:
            u32Shift = TSO_SVQ6_STS_SHIFT;
            break;
        default:
            return FALSE;
    }

    *pu16Status = ((MS_U16)(u32data >> u32Shift) & TSO_SVQ_STS_MASK);

    return TRUE;
}

MS_BOOL HAL_TSO_Set_SVQRX_ArbitorMode(MS_U8 u8Eng, MS_U16 u16mode, MS_U16 *pu16SvqRxPri)
{
    //@TODO not implement
    return TRUE;
}

MS_BOOL HAL_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32systime)
{
    //@TODO not implement
    return TRUE;
}

MS_BOOL HAL_TSO_LocalStreamID(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U8* pu8StrID, MS_BOOL beSet)
{
    REG16_TSO* p16Reg = NULL;

    switch(u8ChIf)
    {
        case HAL_TSO_TSIF_LIVE1:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_1_CONFIG_0);
            break;
        case HAL_TSO_TSIF_LIVE2:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_2_CONFIG_0);
            break;
        case HAL_TSO_TSIF_LIVE3:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_3_CONFIG_0);
            break;
        case HAL_TSO_TSIF_LIVE4:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_4_CONFIG_0);
            break;
        case HAL_TSO_TSIF_LIVE5:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_5_CONFIG_0);
            break;
        case HAL_TSO_TSIF_LIVE6:
            p16Reg = &(_TSOCtrl1->REG_PRE_HEADER_6_CONFIG_0);
            break;
        default:
            return FALSE;
    }

    if(beSet == TRUE)
    {
        _HAL_REG16_W(p16Reg , (MS_U16)(*pu8StrID) & TSO1_REG_PRE_HEADER_1_CONFIG_0_LOCAL_STREAMID_MASK);
    }
    else
    {
        *pu8StrID = (MS_U8)(_HAL_REG16_R(p16Reg) & TSO1_REG_PRE_HEADER_1_CONFIG_0_LOCAL_STREAMID_MASK);
    }

    return TRUE;

}

void HAL_TSO_Flt_SetInputSrc(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16InputSrc)
{
    MS_U16 u16Temp = 0;

    u16Temp = _HAL_TSO_PageTableIndR(TSO_PIDFLT_ADDR(u16FltId));
    u16Temp = (u16Temp & TSO_PIDFLT_PID_MASK) | ((u16InputSrc & TSO_PIDFLT_IN_MASK) << TSO_PIDFLT_IN_SHIFT);
    _HAL_TSO_PageTableIndW(TSO_PIDFLT_ADDR(u16FltId), u16Temp);
}

void HAL_TSO_Flt_SetSid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U8 u8Sid)
{
    //dummy function for make
    return;
}

MS_BOOL HAL_TSO_ReplaceFlt_SetPktPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U8 u8TsIf, MS_U16 u16OldPid, MS_U16 u16NewPid)
{
    //@TODO not implement
    return TRUE;
}

MS_BOOL HAL_TSO_ReplaceFlt_Enable(MS_U8 u8Eng, MS_U16 u16FltId, MS_BOOL bEnable)
{
    //@TODO not implement
    return TRUE;
}

void HAL_TSO_SaveRegs(void)
{
    //@TODO not implement
}

void HAL_TSO_RestoreRegs(void)
{
    //@TODO not implement
}

#ifdef CONFIG_MSTAR_CLKM
void HAL_TSO_PowerCtrl(MS_BOOL bOn)
{
    MS_S32 s32ClkHandle;

    if (bOn)
    {
        // Enable TSO out Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso_out");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSOOUT_DIV");

        // Enable TSO in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSOIN0_PAD0");

        // Enable TSO1 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso1_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSIN1_PAD0");

        // Enable TSO2 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso2_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSIN2_PAD0");

        // Enable TSO3 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso3_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSIN3_PAD0");

        // Enable TSO4 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso4_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSIN4_PAD0");

        // Enable TSO5 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso5_in");
        Drv_Clkm_Set_Clk_Source(s32ClkHandle , "CLK_TSIN5_PAD0");

        // Disable MCM
        _REG16_SET(&(_TSOCtrl->TSO_CONFIG1), TSO_CONFIG1_TURN_OFF_MCM);
    }
    else
    {
        // Enable MCM
        _REG16_CLR(&(_TSOCtrl->TSO_CONFIG1), TSO_CONFIG1_TURN_OFF_MCM);

        // Disabel TSO out Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso_out");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO1 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso1_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO2 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso2_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO3 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso3_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO4 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso4_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);

        // Disabel TSO5 in Clock
        s32ClkHandle = Drv_Clkm_Get_Handle("g_clk_tso5_in");
        Drv_Clkm_Clk_Gate_Disable(s32ClkHandle);
    }
}
#else
void HAL_TSO_PowerCtrl(MS_BOOL bOn)
{
    if (bOn)
    {
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) &= ~REG_CLKGEN0_TSO_OUT_CLK_MASK;
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO1_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO2_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO3_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO4_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO5_IN) &= ~REG_CLKGEN0_TSO_IN_MASK;

        // Disable MCM
        _REG16_SET(&(_TSOCtrl->TSO_CONFIG1), TSO_CONFIG1_TURN_OFF_MCM);
    }
    else
    {
        // Enable MCM
        _REG16_CLR(&(_TSOCtrl->TSO_CONFIG1), TSO_CONFIG1_TURN_OFF_MCM);

        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_OUT_CLK) |= REG_CLKGEN0_TSO_OUT_CLK_DISABLE;
        TSO_CLKGEN0_REG(REG_CLKGEN0_TSO_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO1_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO2_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO3_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO4_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
        TSO_CLKGEN2_REG(REG_CLKGEN2_TSO5_IN) |= REG_CLKGEN0_TSO_IN_DISABLE;
    }
}
#endif
