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
#ifndef _HAL_PNL_C_
#define _HAL_PNL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "halPNL.h"
#include "PNL_private.h"
#include "pnl_hwreg_utility2.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include "string.h"
#define do_div(x,y) ((x)/=(y))
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define UNUSED(x)       (x=x)
#if 1
#define HAL_PNL_DBG(_dbgSwitch_, _fmt, _args...)      { if((_dbgSwitch_ & _u16PnlDbgSwitch) != 0) printf("PNL:"_fmt, ##_args); }
#define HAL_MOD_CAL_DBG(x)    //x
#else
#define HAL_PNL_DBG(_dbgSwitch_, _fmt, _args...)      { }
#endif

#define DAC_LPLL_ICTRL     0x0002
#define LVDS_LPLL_ICTRL    0x0001

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Function
//-------------------------------------------------------------------------------------------------
/**
*   @brief: Power On MOD. but not mutex protected
*
*/
MS_U8 MHal_MOD_PowerOn(void *pInstance, MS_BOOL bEn, MS_U8 u8LPLL_Type,MS_U8 DualModeType, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{

    UNUSED(u8LPLL_Type);


    if( bEn )
    {
        //analog MOD power down. 1: power down, 0: power up
        // For Mod2 no output signel
        ///////////////////////////////////////////////////
        // Power On
        MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x0 , BIT(0) );

        //enable ib , enable ck
        MOD_W2BYTEMSK(REG_MOD_BK00_77_L, (BIT(1) | BIT(0)), (BIT(1) | BIT(0)));
    }
    else
    {   //1: power down, 0: power up
        MOD_W2BYTEMSK(REG_MOD_BK00_78_L, BIT(0), BIT(0));
        //disable ib, disable ck
        MOD_W2BYTEMSK(REG_MOD_BK00_77_L, 0x00, (BIT(1) | BIT(0)));
    }


    return 1;
}

/**
*   @brief: Setup the PVDD power 1:2.5V, 0:3.3V
*
*/
void MHal_MOD_PVDD_Power_Setting(void *pInstance, MS_BOOL bIs2p5)
{
//    MOD_W2BYTEMSK(REG_MOD_BK00_37_L, ((bIs2p5)? BIT(6):0), BIT(6));	//MOD PVDD=1: 2.5,PVDD=0: 3.3
}

void MHal_PNL_TCON_Init(void *pInstance)
{

}

void MHal_Shift_LVDS_Pair(void *pInstance, MS_U8 Type)
{
    UNUSED(Type);
    MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x0000, (BIT(7) | BIT(6))); // shift_lvds_pair, set LVDS Mode3
}



void MHal_Output_LVDS_Pair_Setting(void *pInstance, MS_U8 Type, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{
    UNUSED(Type);
    UNUSED(u16OutputCFG0_7);
    UNUSED(u16OutputCFG8_15);
    UNUSED(u16OutputCFG16_21);
    MOD_W2BYTEMSK(REG_MOD_BK00_6D_L, 0x5000, 0xF000);
    MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x5555);
    MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0x0005, 0x000F);
}

void MHal_PQ_Clock_Gen_For_Gamma(void *pInstance)
{

}

void MHal_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping)
{

    if(u8Mapping & GAMMA_MAPPING)
    {
        SC_W2BYTEMSK(0, REG_SC_BK10_74_L, (u8Mapping & GAMMA_10BIT_MAPPING)? BIT(4):0, BIT(4));
    }
    else
    {
        PNL_ASSERT(0, "Invalid eSupportGammaMapMode [%d] Passed to [%s], please make sure the u8Mapping[%d] is valid\n.",
                       u8Mapping, __FUNCTION__, u8Mapping);
    }
}

MS_BOOL Hal_VOP_Is_GammaMappingMode_enable(void *pInstance)
{
    return SC_R2BYTEMSK(0, REG_SC_BK10_74_L, BIT(4));
}

// After A5, 8 bit mode only support burst write!!!
MS_BOOL Hal_VOP_Is_GammaSupportSignalWrite(void *pInstance, DRVPNL_GAMMA_MAPPEING_MODE u8Mapping)
{
    return TRUE;
}

void hal_PNL_WriteGamma12Bit(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    MS_U16 u16Delay = 0xFFFF;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Write [ch %d][addr 0x%x]: 0x%x \n", u8Channel, u16Addr, u16GammaValue);

    if (!bBurstWrite )
    {
        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, 0xE0) && (u16Delay > 0))          // Check whether the Write chanel is ready
        {
            u16Delay--;
        }
        PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6C_L, u16Addr, 0x3FF);                          // set address port
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_6E_L + u8Channel *2), u16GammaValue, 0xFFF);      // Set channel data

        // kick off write
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(5), BIT(5));
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(6), BIT(6));
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(7), BIT(7));
                break;
        }

        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, 0xE0) && (u16Delay > 0))          // Check whether the Write chanel is ready
        {
            u16Delay--;
        }
    }
    else
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6E_L, u16GammaValue, 0xFFF);
    }


    PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

}


void hal_PNL_SetMaxGammaValue( void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Max gamma of %d is 0x%x\n", u8Channel, u16MaxGammaValue);

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_7A_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 0
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_7B_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 1
}


/////////////////////////////////////////////////////////////////////////////
// Gamma format (12 bit LUT)
//      0, 1, 2, 3, ..., NumOfLevel, totally N Sets of tNormalGammaR/G/B[],
//      1 set uses 2 bytes of memory.
//
// [T2 and before ] N = 256
// [T3]             N = 256 or 1024
// ______________________________________________________________________________
//  Byte | 0         1           2               n-1        n
//    [G1|G0]       [G0]       [G1] . ...... .  [Gmax]    [Gmax]
//    3:0  3:0      11:4       11:4              3:0       11:4
//

void Hal_PNL_Set12BitGammaPerChannel( void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode )
{
    MS_U16 u16Addr            = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance, GammaMapMode);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // Go to burst write if not support
    if ( bUsingBurstWrite )
    {
        // 1.   initial burst write address, LUT_ADDR[7:0]
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6C_L, 0x00 , 0x3FF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0x00 , BIT(3) | BIT(2) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(2) , BIT(3) | BIT(2) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(3) , BIT(3) | BIT(2) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(0) , BIT(0)); // Burst write enable

    }

    //printf("\33[0;31m Gamma Mapping mode %d \n \33[m",GammaMapMode );
    // write gamma table per one channel
    for(; u16Addr < u16NumOfLevel; u16CodeTableIndex += 3)
    {
        // gamma x
        u16GammaValue = u8Tab[u16CodeTableIndex] & 0x0F;
        u16GammaValue |= u8Tab[u16CodeTableIndex+1] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA,"Gamma x: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x, GammaLvl=%d\n",
                                    u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+1, u8Tab[u16CodeTableIndex+1], u16GammaValue, u16NumOfLevel);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        hal_PNL_WriteGamma12Bit(pInstance, u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;

        // gamma x+1
        u16GammaValue = (u8Tab[u16CodeTableIndex] & 0xF0) >> 4;
        u16GammaValue |= u8Tab[u16CodeTableIndex+2] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA, "Gamma x+1: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x\n", u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+2, u8Tab[u16CodeTableIndex+2], u16GammaValue);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        hal_PNL_WriteGamma12Bit(pInstance, u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;
    }

    if ( bUsingBurstWrite )
    {
        // 5.   after finish burst write data of one channel, disable burst write mode
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0x00 , BIT(0));
    }

    hal_PNL_SetMaxGammaValue(pInstance, u8Channel, u16MaxGammaValue);
}


void MHal_PNL_Init_LPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    #if 0
    W2BYTE(L_BK_LPLL(0x42),0x1000);    // {H_BK_MOD(0x42), 0x10}, //PDP must set to 1

    W2BYTE(L_BK_LPLL(0x00), ((PANEL_LPLL_INPUT_DIV_2nd<<8)  | PANEL_LPLL_INPUT_DIV_1st));
    W2BYTE(L_BK_LPLL(0x01), ((PANEL_LPLL_LOOP_DIV_2nd<<8)   | PANEL_LPLL_LOOP_DIV_1st));
    W2BYTE(L_BK_LPLL(0x02), ((PANEL_LPLL_OUTPUT_DIV_2nd<<8) | PANEL_LPLL_OUTPUT_DIV_1st));

    if (eLPLL_Type == E_PNL_TYPE_DAC_I ||
        eLPLL_Type == E_PNL_TYPE_DAC_P)
    {
        W2BYTE(L_BK_LPLL(0x03), DAC_LPLL_ICTRL);//DAC output: better value(ICTRL) for stable LPLL
    }
    else
    {
        W2BYTE(L_BK_LPLL(0x03), LVDS_LPLL_ICTRL);
    }

    W2BYTEMSK(L_BK_LPLL(0x03), 0, BIT(6));

    W2BYTEMSK(L_BK_LPLL(0x03), (eLPLL_Mode << 7) , BIT(7));
    #endif

}

MS_U8 MHal_PNL_Get_Loop_DIV( void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    UNUSED(u8LPLL_Mode);
    UNUSED(eLPLL_Type);

    return (u8LPLL_Mode == 0 ? 14 : 7);
}

MS_U16 MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16   u16loopgain = 0;

    switch(eLPLL_Type)
    {
        case E_PNL_TYPE_MINILVDS:
        case E_PNL_TYPE_ANALOG_MINILVDS:
        case E_PNL_TYPE_DIGITAL_MINILVDS:
            u16loopgain = 2;
        break;

        case E_PNL_TYPE_TTL:
        case E_PNL_TYPE_LVDS:
        case E_PNL_TYPE_RSDS:
        case E_PNL_TYPE_PDPLVDS:
        default:
            u16loopgain = LPLL_LOOPGAIN;
        break;
    }

    return u16loopgain;
}

#define SKIP_TIMING_CHANGE_CAP  TRUE
MS_BOOL Hal_PNL_SkipTimingChange_GetCaps(void *pInstance)
{
    #if (SKIP_TIMING_CHANGE_CAP)
        return TRUE;
    #else
        return FALSE;
    #endif
}

void MHal_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode)
{
    UNUSED(bSetMode);
}

void MHal_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo)
{
    UNUSED(lvdsresinfo);
}

////////////////////////////////////////////////////////////////////////
// Turn OD function
////////////////////////////////////////////////////////////////////////
void MHal_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_MSB_limit, MS_PHY u32OD_LSB_Addr, MS_PHY u32OD_LSB_limit, MS_U8 u8MIUSel)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_15_L, (MS_U16)(u32OD_MSB_Addr & 0xFFFF)); // OD MSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_16_L, (MS_U16)((u32OD_MSB_Addr >> 16) & 0x00FF), 0x00FF); // OD MSB request base address
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_17_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD MSB request address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_18_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x00FF), 0x00FF); // OD MSB request address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_39_L, (MS_U16)(u32OD_LSB_limit & 0xFFFF)); // OD frame buffer write address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, (MS_U16)((u32OD_LSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer write address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3B_L, (MS_U16)(u32OD_LSB_limit & 0xFFFF)); // OD frame buffer read address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3C_L, (MS_U16)((u32OD_LSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer read address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_4F_L, (MS_U16)(u32OD_LSB_Addr & 0xFFFF)); // OD LSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_Addr >> 16) & 0x00FF), 0x00FF); // OD LSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_limit & 0x00FF) << 8), 0xFF00); // OD LSB request limit address
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_51_L, (MS_U16)((u32OD_LSB_limit >> 8) & 0xFFFF)); // OD LSB request limit address

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_1A_L, 0x4020); // OD request rFIFO limit threshold, priority threshold
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_1C_L, 0x4020); // OD request wFIFO limit threshold, priority threshold
}

void MHal_PNL_OverDriver_Enable(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // OD mode
    // OD used user weight to output blending directly
    // OD Enable
    if (bEnable)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x2D, 0x2F);
    }
    else
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x2C, 0x2F);
    }
}

void MHal_PNL_OverDriver_TBL(void *pInstance, MS_U8 u8ODTbl[1056])
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_U16 i;
    MS_U8 u8target;
    MS_BOOL bEnable;

    bEnable = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, BIT(0));
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x00, BIT(0)); // OD enable
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_01_L, 0x0E, 0x0E); // OD table SRAM enable, RGB channel

    u8target= u8ODTbl[9];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_03_L, (i == 9)?u8target:(u8target ^ u8ODTbl[i]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_02_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_02_L, BIT(15)));
    }

    u8target= u8ODTbl[(272+19)];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_06_L, (i == 19)?u8target:(u8target ^ u8ODTbl[(272+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_05_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_05_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+29)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_09_L, (i == 29)?u8target:(u8target ^ u8ODTbl[(272*2+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_08_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_08_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+256+39)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0C_L, (i == 39)?u8target:(u8target ^ u8ODTbl[(272*2+256+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0B_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0D_L, BIT(15)));
    }

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_01_L, 0x00, 0x0E); // OD table SRAM enable, RGB channel
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, bEnable, BIT(0)); // OD enable
}

MS_BOOL MHal_PNL_MOD_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level)
{
    // 2X mode
    MOD_W2BYTE(REG_MOD_BK00_2B_L, u16Swing_Level);       //Bank R output swing level
    MOD_W2BYTE(REG_MOD_BK00_2C_L, u16Swing_Level);       //Bank L output swing level

    return TRUE;
}

void MHal_PNL_PreInit(void *pInstance, PNL_OUTPUT_MODE eParam)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = eParam;
}

PNL_OUTPUT_MODE MHal_PNL_Get_Output_MODE(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_OUTPUT_MODE eParam = pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions;

    return eParam;
}

void MHal_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if( eLPLL_Type == E_PNL_TYPE_TTL)
    {
        // select pair output to be TTL
        MOD_W2BYTE(REG_MOD_BK00_6D_L, 0x0000);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x0000);
        MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0x0000, 0xEFFF);//0x6F

        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x0000, (BIT(7) | BIT(6))); // shift_lvds_pair

        // other TTL setting
        MOD_W2BYTE(REG_MOD_BK00_45_L, 0x0000);     // TTL output enable

        MOD_W2BYTE(REG_MOD_BK00_46_L, 0x0000);
        MOD_W2BYTE(REG_MOD_BK00_47_L, 0x0000);
        MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0xE000);

        MOD_W2BYTEMSK(REG_MOD_BK00_44_L, 0x3FF, 0x3FF);       // TTL skew

        // GPO gating
        MOD_W2BYTEMSK(REG_MOD_BK00_4A_L, BIT(8), BIT(8));         // GPO gating
    }
    else
    {
        switch(eOutputMode)
        {
            case E_PNL_OUTPUT_NO_OUTPUT:
                // if MOD_45[5:0] = 0x3F && XC_MOD_EXT_DATA_EN_L = 0x0,
                // then if XC_MOD_OUTPUT_CONF_L = 0x0 ---> output TTL as tri-state
                MOD_W2BYTE(REG_MOD_BK00_6D_L, 0x0000);
                MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x0000);
                MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0x0000, 0x000F);

                MOD_W2BYTEMSK(REG_MOD_BK00_46_L, 0xF000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_47_L, 0xFFFF);
                MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x000F);


                MOD_W2BYTEMSK(REG_MOD_BK00_4D_L, 0xF000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_4E_L, 0xFFFF);
                MOD_W2BYTEMSK(REG_MOD_BK00_4F_L, 0xF000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_50_L, 0x0000);
                MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_52_L, 0x0000);

                break;

            case E_PNL_OUTPUT_CLK_ONLY:

                break;

            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:

                MOD_W2BYTEMSK(REG_MOD_BK00_4D_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_4E_L, 0x0000);

                MHal_Output_LVDS_Pair_Setting(pInstance, pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                                              , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                                              , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                                              , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
                MHal_Shift_LVDS_Pair(pInstance, pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift);

                // other TTL setting
                MOD_W2BYTE(REG_MOD_BK00_45_L, 0x003F);     // LVDS output enable, [5:4] Output enable: PANEL_LVDS/ PANEL_miniLVDS/ PANEL_RSDS

                MOD_W2BYTEMSK(REG_MOD_BK00_46_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_47_L, 0x0000);
                MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x000F);

                MOD_W2BYTEMSK(REG_MOD_BK00_44_L, 0x000, 0x3FF);       // TTL skew

                // GPO gating
                MOD_W2BYTEMSK(REG_MOD_BK00_4A_L, 0x0, BIT(8));         // GPO gating
                break;
        }
    }
}

void Mhal_PNL_Flock_LPLLSet(void *pInstance, MS_U64 ldHz)
{
	UNUSED(ldHz);
}

void MHal_PNL_MISC_Control(void *pInstance, MS_U32 u32PNL_MISC)
{
    if(u32PNL_MISC & E_DRVPNL_MISC_MFC_ENABLE)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, BIT(7), BIT(7));       // shift LVDS pair
    }
}

void MHal_PNL_Init_XC_Clk(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    UNUSED(pstPanelInitData);

    if(pPNLInstancePrivate->u32DeviceID == 0) // SC0
    {
        // setup output dot clock
        //W2BYTEMSK(REG_CKG_ODCLK, CKG_ODCLK_CLK_LPLL, CKG_ODCLK_MASK);      // will set by DAC table
        W2BYTEMSK(REG_CKG_ODCLK, DISABLE, CKG_ODCLK_INVERT);               // clock not invert
        W2BYTEMSK(REG_CKG_ODCLK, DISABLE, CKG_ODCLK_GATED);                // enable clock
    }
    else if(pPNLInstancePrivate->u32DeviceID == 1) // SC1
    {
        // setup output dot clock
        W2BYTEMSK(REG_CKG_SC1_ODCLK, CKG_SC1_ODCLK_13M, CKG_SC1_ODCLK_MASK);      // select 13.5M
        W2BYTEMSK(REG_CKG_SC1_ODCLK, DISABLE, CKG_SC1_ODCLK_INVERT);               // clock not invert
        W2BYTEMSK(REG_CKG_SC1_ODCLK, DISABLE, CKG_SC1_ODCLK_GATED);                // enable clock
    }
    else
    {
        PNL_DBG(PNL_DBGLEVEL_INIT, "Invalid device ID %tu !\n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID);
    }
}

void MHal_PNL_Init_MOD(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    //-------------------------------------------------------------------------
    // Set MOD registers
    //-------------------------------------------------------------------------

    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, pstPanelInitData->u16MOD_CTRL0, LBMASK);

    //    GPIO is controlled in drvPadConf.c
    //    MDrv_Write2Byte(L_BK_MOD(0x46), 0x0000);    //EXT GPO disable
    //    MDrv_Write2Byte(L_BK_MOD(0x47), 0x0000);    //EXT GPO disable
    MOD_W2BYTE(REG_MOD_BK00_49_L, pstPanelInitData->u16MOD_CTRL9); //{L_BK_MOD(0x49), 0x00}, // [7,6] : output formate selction 10: 8bit, 01: 6bit :other 10bit, bit shift
    MOD_W2BYTE(REG_MOD_BK00_4A_L, pstPanelInitData->u16MOD_CTRLA);
    MOD_W2BYTE(REG_MOD_BK00_4B_L,  pstPanelInitData->u8MOD_CTRLB);  //[1:0]ti_bitmode 10:8bit  11:6bit  0x:10bit

    if ( SUPPORT_SYNC_FOR_DUAL_MODE )
    {
        // Set 1 only when PNL is dual mode
        MOD_W2BYTEMSK(REG_MOD_BK00_44_L, (pstPanelInitData->eLPLL_Mode << 12) , BIT(12));
    }

    //dual port lvds _start_//
    // output configure for 26 pair output 00: TTL, 01: LVDS/RSDS/mini-LVDS data differential pair, 10: mini-LVDS clock output, 11: RSDS clock output
    MOD_W2BYTE(REG_MOD_BK00_6F_L, 0x0000);    // output configure for 26 pair output 00: TTL, 01: LVDS/RSDS/mini-LVDS data differential pair, 10: mini-LVDS clock output, 11: RSDS clock output
    MOD_W2BYTEMSK(REG_MOD_BK00_77_L, 0x0C, 0xFC);       // original is MDrv_WriteByteMask(L_BK_MOD(0x77), 0x0F, BITMASK(7:2));
    //dual port lvds _end_//

    //MOD_W2BYTEMSK(REG_MOD_BK00_78_L, (_u8PnlDiffSwingLevel << 1), 0xFE);       //differential output swing level
    if(!MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u8PnlDiffSwingLevel))
	    printf(">>Swing Level setting error!!\n");
    if(pstPanelInitData->eLPLL_Type != E_PNL_TYPE_MINILVDS)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, 0x1F, LBMASK);       //[6]disable power down bit and [5:0]enable all channel
    }
    MOD_W2BYTE(REG_MOD_BK00_73_L, pstPanelInitData->u16LVDSTxSwapValue);


    // TODO: move from MDrv_Scaler_Init(), need to double check!
    MOD_W2BYTEMSK(REG_MOD_BK00_53_L, BIT(0), BIT(0));


    //--------------------------------------------------------------
    //Depend On Bitmode to set Dither
    //--------------------------------------------------------------


    // always enable noise dither and disable TAILCUT
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, ((pstPanelInitData->u8PanelNoiseDith ? XC_PAFRC_DITH_NOISEDITH_EN : (1 - XC_PAFRC_DITH_NOISEDITH_EN)) <<3) , BIT(3));
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, XC_PAFRC_DITH_TAILCUT_DISABLE, BIT(4));

    switch(pstPanelInitData->u8MOD_CTRLB & 0x03)//[1:0]ti_bitmode b'10:8bit  11:6bit  0x:10bit
    {
        case HAL_TI_6BIT_MODE:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(2), BIT(2));
            break;

        case HAL_TI_8BIT_MODE:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;

        case HAL_TI_10BIT_MODE:
        default:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;
    }


    //-----depend on bitmode to set Dither------------------------------
    MHal_PNL_SetOutputType(pInstance, pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions, pstPanelInitData->eLPLL_Type);     // TTL to Ursa

    MHal_PNL_MISC_Control(pInstance, pstPanelInitData->u32PNL_MISC);

}

void MHal_PNL_DumpMODReg(void *pInstance, MS_U32 u32Addr, MS_U16 u16Value, MS_BOOL bHiByte, MS_U16 u16Mask)
{
    if (bHiByte)
    {
        MOD_W2BYTEMSK(u32Addr, (u16Value << 8), (u16Mask << 8));
    }
    else
    {
        MOD_W2BYTEMSK(u32Addr, u16Value, u16Mask);
    }
}

void MHal_MOD_Calibration_Init(void *pInstance, PNL_ModCali_InitData *pstModCaliInitData)
{
        PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
        PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
        UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
	// Target setting
	// =========
	// GCR_CAL_LEVEL[1:0] : REG_MOD_BK00_7D_L =>
	// =========
	//in msModCurrentCalibration, it will transfer to the real data
	switch(pstModCaliInitData->u8ModCaliTarget)
	{
		default:
		case 0:
			pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 0;
		break;
		case 1:
			pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 1;
		break;
		case 2:
			pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 2;
		break;
		case 3:
			pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 3;
		break;
	}
    // Offset setting, for fine tune
    //_usMOD_CALI_OFFSET = pstModCaliInitData->s8ModCaliOffset;
}

void MHal_BD_LVDS_Output_Type(void *pInstance, MS_U16 Type)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = LVDS_DUAL_OUTPUT_SPECIAL;
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 1;
    }
    else
    {
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type  = Type;
    }
}


MS_BOOL msModCalDDAOUT(void *pInstance)
{
   // W2BYTEMSK(BK_MOD(0x7D), ENABLE, 8:8);
   // MsOS_DelayTask(10);  //10ms
    return (MS_BOOL)((MOD_R2BYTEMSK(REG_MOD_BK00_7D_L, BIT(8))) >> 8);
}

MS_U8 msModCurrentCalibration(void *pInstance)
{
    return 0x60;
}

PNL_Result MHal_PNL_MOD_Calibration(void *pInstance)
{

    MS_U8 u8BackUSBPwrStatus;

	u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

	W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

    msModCurrentCalibration(pInstance);

    W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));


    return E_PNL_OK;

}

static void MHal_PNL_PowerDownLPLL(MS_BOOL bEnable)
{
    if(bEnable)
    {
        W2BYTEMSK(L_BK_LPLL(0x03), BIT(5), BIT(5));
    }
    else
    {
        W2BYTEMSK(L_BK_LPLL(0x03), FALSE, BIT(5));
    }
}

PNL_Result MHal_PNL_En(void *pInstance, MS_BOOL bPanelOn, MS_BOOL bCalEn)
{
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(bPanelOn)
    {
        // The order is PanelVCC -> delay pnlGetOnTiming1() -> VOP -> MOD
        // VOP
        SC_W2BYTEMSK(0, REG_SC_BK10_46_L, 0x4000, HBMASK);

        // mod power on
        MHal_MOD_PowerOn(pInstance, ENABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

        // turn on LPLL
        MHal_PNL_PowerDownLPLL(FALSE);

#if ENABLE_MODE_PATCH
		MOD_W2BYTE(REG_MOD_BK00_33_L, 0x0039);
		MsOS_DelayTask(5);
		MOD_W2BYTEMSK(REG_MOD_BK00_33_L, BIT(8), BIT(8));
		MOD_W2BYTEMSK(REG_MOD_BK00_33_L, BIT(9), BIT(9));
#endif

        if(bCalEn)
        {

            u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

		    W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

            msModCurrentCalibration(pInstance);

            W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

        }
        if(!MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel))
            printf(">>Swing Level setting error!!\n");
    }
    else
    {
        // The order is LPLL -> MOD -> VOP -> delay for MOD power off -> turn off VCC

        // LPLL
        // MHal_PNL_PowerDownLPLL(TRUE); //Remove to keep op vsync if panel off

        MHal_MOD_PowerOn(pInstance, DISABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
        // VOP
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_I ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_P)//(bIsLVDS)
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0xFF, LBMASK);
        else
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0x00, 0xFF);
    }

    return E_PNL_OK;
}

void MHal_PNL_SetOutputPattern(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{
    if (bEnable)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_02_L, u16Red , 0x03FF);
        MOD_W2BYTEMSK(REG_MOD_BK00_03_L, u16Green , 0x03FF);
        MOD_W2BYTEMSK(REG_MOD_BK00_04_L, u16Blue , 0x03FF);
        MsOS_DelayTask(10);
        MOD_W2BYTEMSK(REG_MOD_BK00_01_L, BIT(15) , BIT(15));
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_01_L, DISABLE , BIT(15));
    }

}

void MHal_PNL_Switch_LPLL_SubBank(void *pInstance, MS_U16 u16Bank)
{
	UNUSED(u16Bank);
}


void MHal_PNL_Switch_TCON_SubBank(void *pInstance, MS_U16 u16Bank)
{
    UNUSED(u16Bank);
}

MS_U16 MHal_PNL_Read_TCON_SubBank(void *pInstance)
{
    printf("TCON Unsupported");
    return 0x00;
}

MS_BOOL MHal_PNL_IsYUVOutput(void *pInstance)
{
   return TRUE;
}

MS_BOOL MHal_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    UNUSED(u16Fmodulation);
    UNUSED(u16Rdeviation);
    UNUSED(bEnable);
    return FALSE;
}

void MHal_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable)
{
    UNUSED(bEnable);
}

MS_BOOL MHal_PNL_GetOutputInterlaceTiming(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if(pPNLInstancePrivate->u32DeviceID == 0) // SC0
    {
        return FALSE;
    }
    else if(pPNLInstancePrivate->u32DeviceID == 1) // SC1
    {
        return TRUE;
    }
    else
    {
        PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "Invalid device ID %tu !\n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID);
        return FALSE;
    }
}


void MHal_PNL_Set_Device_Bank_Offset(void *pInstance)
{
    UNUSED(pInstance);
    memset(u32PNL_XCDeviceBankOffset, 0, sizeof(MS_U32)*E_PNL_DEVICE_ID_NUM);
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_0] = E_HALPNL_DEVICE0_XC_BANK_OFFSET; // Set SC0 reg bank offset
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_1] = E_HALPNL_DEVICE1_XC_BANK_OFFSET; // Set SC1 reg bank offset
}

void MHal_PNL_Init(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // Set XC no-signal color
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_24_L, 0x0082, 0x00FF);
}

MS_U16 MHal_PNL_GetPanelVStart(void)
{
    return 0;
}

#endif
