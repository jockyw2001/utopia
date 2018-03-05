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
#include <string.h>
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "halPNL.h"
#include "PNL_private.h"
#include "drvPNL.h"
#include "halPNL.h"
#include "pnl_hwreg_utility2.h"
#include "Mustang_pnl_lpll_tbl.h"

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

//Get MOD calibration time
#define MOD_CAL_TIMER   FALSE

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

// Output swing = 150mV + Code * 10mV.
// Designer recommand value. 0x15 = 21 = 250mv
static MS_U16  _u16PnlDefault_SwingLevel = 250;
static PNL_OUTPUT_MODE   _eDrvPnlInitOptions = E_PNL_OUTPUT_CLK_DATA;
MS_U8 _u8MOD_CALI_TARGET = 0;     ///< MOD_REG(0x7D),[10:9]00: 250mV ,01: 350mV ,10:300mV ,11:200mV
MS_S8 _usMOD_CALI_OFFSET = 0;        ///< MOD_REG(0x7D),[5:0]+ _usMOD_CALI_OFFSET
MS_U8 _u8MOD_CALI_VALUE  = 0x15;     /// Final value

MS_U8 _u8MOD_LVDS_Pair_Shift = 0;       ///< 0:default setting, LVDS pair Shift
MS_U8 _u8MOD_LVDS_Pair_Type = 0;        ///< 0:default setting, LVDS data differential pair
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
MS_U8 MHal_MOD_PowerOn(void *pInstance,MS_BOOL bEn, MS_U8 u8LPLL_Type,MS_U8 DualModeType, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{
    if( bEn )
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_37_L, 0x00, BIT(8));

        //analog MOD power down. 1: power down, 0: power up
        // For Mod2 no output signel
        ///////////////////////////////////////////////////

        //2. Power on MOD (current and regulator)
        MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x00 , BIT(0) | BIT(7) );

        //enable ib, enable ck
        MOD_W2BYTEMSK(REG_MOD_BK00_77_L, (BIT(1) | BIT(0)), (BIT(1) | BIT(0)));

        // clock gen of dot-mini
        if(u8LPLL_Type == E_PNL_TYPE_MINILVDS)
        {
            MOD_W2BYTE(REG_MOD_BK00_20_L, 0x4400);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_20_L, 0x0000);
        }

        // 3. 4. 5.
        MHal_Output_LVDS_Pair_Setting(pInstance,DualModeType, u16OutputCFG0_7, u16OutputCFG8_15, u16OutputCFG16_21);

    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_37_L, BIT(8), BIT(8));
        if(u8LPLL_Type !=E_PNL_TYPE_MINILVDS)
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x00, BIT(0));                              //analog MOD power down. 1: power down, 0: power up
        }

        MOD_W2BYTEMSK(REG_MOD_BK00_77_L, 0, (BIT(1) | BIT(0) ));                           //enable ib, enable ck

        // clock gen of dot-mini
        MOD_W2BYTE(REG_MOD_BK00_20_L, 0x1100);
    }

    return 1;
}
/**
*   @brief: Setup the PVDD power 1:2.5V, 0:3.3V
*
*/
void MHal_MOD_PVDD_Power_Setting(void *pInstance,MS_BOOL bIs2p5)
{
    MOD_W2BYTEMSK(REG_MOD_BK00_37_L, ((bIs2p5)? BIT(6):0), BIT(6));    //MOD PVDD=1: 2.5,PVDD=0: 3.3
}

void MHal_PNL_TCON_Init(void *pInstance)
{

}

void MHal_Shift_LVDS_Pair(void *pInstance,MS_U8 Type)
{
    if(Type == 1)
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, (BIT(7) | BIT(6)), (BIT(7) | BIT(6))); // shift_lvds_pair, set LVDS Mode3
    else
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x0000, (BIT(7) | BIT(6)));

}

void MHal_Output_LVDS_Pair_Setting(void *pInstance,MS_U8 Type, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{
    // Disable OP
    MOD_W2BYTEMSK(REG_MOD_BK00_78_L, BIT(1) , BIT(1) );

    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        // for 100/128 pin special case
        MOD_W2BYTEMSK(REG_MOD_BK00_34_L, 0x1000, 0xFF00);
        MOD_W2BYTE(REG_MOD_BK00_6D_L, 0x5555);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x0055);
        MOD_W2BYTE(REG_MOD_BK00_6F_L, 0x0000);
    }
    else if(Type == LVDS_SINGLE_OUTPUT_A)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_6D_L, 0x0000, 0xF000);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x5500);
        MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0x0005, 0x000F);
    }
    else if( Type == LVDS_SINGLE_OUTPUT_B)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_6D_L, 0x5000, 0xF000);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x0055);
        MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0x0000, 0x000F);
    }
    else if( Type == LVDS_OUTPUT_User)
    {
        MOD_W2BYTE(REG_MOD_BK00_6D_L, u16OutputCFG0_7);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, u16OutputCFG8_15);
        MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, u16OutputCFG16_21, 0x000F);
    }
    else
    {
        MOD_W2BYTE(REG_MOD_BK00_6D_L, 0x5550);
        MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x0555);
        MOD_W2BYTE(REG_MOD_BK00_6F_L, 0x0000);
    }

    MsOS_DelayTask(2);

    // Enable OP
    MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x00 , BIT(1) );

}

void MHal_Output_Channel_Order(void *pInstance,
                               MS_U8 Type,
                               MS_U16 u16OutputOrder0_3,
                               MS_U16 u16OutputOrder4_7,
                               MS_U16 u16OutputOrder8_11,
                               MS_U16 u16OutputOrder12_13)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
#if 0
    if(Type == APIPNL_OUTPUT_CHANNEL_ORDER_USER )
    {
        MOD_W2BYTE(REG_MOD_BK00_08_L, u16OutputOrder0_3);
        MOD_W2BYTE(REG_MOD_BK00_09_L, u16OutputOrder4_7);
        MOD_W2BYTE(REG_MOD_BK00_0A_L, u16OutputOrder8_11);
        MOD_W2BYTE(REG_MOD_BK00_0B_L, u16OutputOrder12_13);
    }
    else
    {
        if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                MOD_W2BYTE(REG_MOD_BK00_08_L, 0x6420);
                MOD_W2BYTE(REG_MOD_BK00_09_L, 0x7531);
                MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xB9A8);
                MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
            }
            else
            {
                MOD_W2BYTE(REG_MOD_BK00_08_L, 0x5410);
                MOD_W2BYTE(REG_MOD_BK00_09_L, 0x7632);
                MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xB9A8);
                MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
            }
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_08_L, 0x5410);
            MOD_W2BYTE(REG_MOD_BK00_09_L, 0x5410);
            MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xB9A8);
            MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
        }
    }
#endif

}

void MHal_PQ_Clock_Gen_For_Gamma(void *pInstance)
{
    W2BYTEMSK(REG_CLKGEN0_52_L, 0x00, 0x07);
}

void MHal_VOP_SetGammaMappingMode(void *pInstance,MS_U8 u8Mapping)
{

    if(u8Mapping & GAMMA_MAPPING)
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_74_L, (u8Mapping & GAMMA_10BIT_MAPPING)? BIT(15):0, BIT(15));
    }
    else
    {
        PNL_ASSERT(0, "Invalid eSupportGammaMapMode [%d] Passed to [%s], please make sure the u8Mapping[%d] is valid\n.",
                       u8Mapping, __FUNCTION__, u8Mapping);
    }
}

MS_BOOL Hal_VOP_Is_GammaMappingMode_enable(void *pInstance)
{
    return SC_R2BYTEMSK(0,REG_SC_BK10_74_L, BIT(15));
}

// After A5, 8 bit mode only support burst write!!!
MS_BOOL Hal_VOP_Is_GammaSupportSignalWrite(void *pInstance,DRVPNL_GAMMA_MAPPEING_MODE u8Mapping)
{
    return TRUE;
}


void hal_PNL_WriteGamma12Bit(void *pInstance,MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    MS_U16 u16Delay = 0xFFFF;

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Write [ch %d][addr 0x%x]: 0x%x \n", u8Channel, u16Addr, u16GammaValue);

    if (!bBurstWrite )
    {
        while (SC_R2BYTEMSK(0,REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
        PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

        SC_W2BYTEMSK(0,REG_SC_BK10_6C_L, u16Addr, 0x3FF);                          // set address port
        SC_W2BYTEMSK(0,(REG_SC_BK10_6E_L + u8Channel *2), u16GammaValue, 0xFFF);      // Set channel data

        // kick off write
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(5), BIT(5));
                break;

            case 1:  // Green
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(6), BIT(6));
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(7), BIT(7));
                break;
        }

        while (SC_R2BYTEMSK(0,REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_6E_L, u16GammaValue, 0xFFF);
    }


    PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

}


void hal_PNL_SetMaxGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Max gamma of %d is 0x%x\n", u8Channel, u16MaxGammaValue);

    SC_W2BYTEMSK(0,(REG_SC_BK10_7A_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 0
    SC_W2BYTEMSK(0,(REG_SC_BK10_7B_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 1
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

void Hal_PNL_Set12BitGammaPerChannel(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode )
{
    MS_U16 u16Addr            = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance,GammaMapMode);

    // Go to burst write if not support
    if ( bUsingBurstWrite )
    {
        // 1.   initial burst write address, LUT_ADDR[7:0]
        SC_W2BYTEMSK(0,REG_SC_BK10_6C_L, 0x00 , 0x3FF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, 0x00 , BIT(3) | BIT(2) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(2) , BIT(3) | BIT(2) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(3) , BIT(3) | BIT(2) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, BIT(0) , BIT(0)); // Burst write enable

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
        hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
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
        hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;
    }

    if ( bUsingBurstWrite )
    {
        // 5.   after finish burst write data of one channel, disable burst write mode
        SC_W2BYTEMSK(0,REG_SC_BK10_6D_L, 0x00 , BIT(0));
    }

    hal_PNL_SetMaxGammaValue(pInstance,u8Channel, u16MaxGammaValue);
}
static E_PNL_SUPPORTED_LPLL_TYPE _MHal_PNL_GetSupportedLPLLIndex(void *pInstance,
                                                                 PNL_TYPE eLPLL_Type,
                                                                 PNL_MODE eLPLL_Mode,
                                                                 MS_U64 ldHz)
{
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;

    switch (eLPLL_Type)
    {
        case E_PNL_TYPE_LVDS:
            switch (eLPLL_Mode)
            {
                case E_PNL_MODE_SINGLE:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to90MHz;
                break;

                default:
                case E_PNL_MODE_DUAL:
                    if ((ldHz >= 500000000UL) && (ldHz < 1100000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to110MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_110to150MHz;
                    }
                break;
            }
        break;

        case E_PNL_TYPE_HS_LVDS:

            switch (eLPLL_Mode)
            {
                case E_PNL_MODE_SINGLE:
                    if((ldHz >= 500000000UL) && (ldHz < 1100000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to110MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_110to150MHz;
                    }
                break;

                default:
                case E_PNL_MODE_DUAL:
                    if(ldHz >= 2300000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_230to300MHz;
                    }
                    else if(ldHz >= 1145000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_114_5to230MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to114_5MHz;
                    }
                break;
            }
        break;

        case E_PNL_TYPE_TTL:
              if ((ldHz >= 250000000UL) && (ldHz < 500000000UL))
              {
                  u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_25to50MHz;
              }
              else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
              {
                  u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_50to100MHz;
              }
              else
              {
                  u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_100to150MHz;
              }
          break;

        case E_PNL_LPLL_MINILVDS_1CH_3P_8BIT:
            //if (ldHz >= 500000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz;
            }
        break;

        case E_PNL_LPLL_MINILVDS_2CH_3P_8BIT:
            if (ldHz >= 1000000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz;
            }
            else //if (ldHz >= 500000000UL)
            {
                u8SupportedLPLLIndex =     E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz;
            }
        break;

        case E_PNL_LPLL_MINILVDS_1CH_3P_6BIT:
            if (ldHz >= 666700000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz;
            }
            else //if (ldHz >= 500000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz;
            }
        break;

        case E_PNL_LPLL_MINILVDS_2CH_3P_6BIT:
            if (ldHz >= 1333300000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz;
            }
            else if (ldHz >= 666700000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz;
            }
            else
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz;
            }
        break;

        case E_PNL_LPLL_MINILVDS_2CH_6P_8BIT:
            if (ldHz >= 1000000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz;
            }
            else //if (ldHz >= 500000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz;
            }
        break;

        case E_PNL_LPLL_MINILVDS_2CH_6P_6BIT:
            if (ldHz >= 1333300000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133_33to150MHz;
            }
            else if (ldHz >= 666700000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_66_67to133_33MHz;
            }
            else
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to66_67MHz;
            }
        break;

        case E_PNL_LPLL_EPI34_4P:
            if (ldHz >= 950000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_34_10bit_4PAIR_95to150MHz;
            }
            else //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_34_10bit_4PAIR_80to95MHz;
            }
        break;

        case E_PNL_LPLL_EPI34_6P:
            //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_34_10bit_6PAIR_80to150MHz;
            }
        break;

        case E_PNL_LPLL_EPI34_8P:
            if (ldHz >= 950000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_34_10bit_8PAIR_95to150MHz;
            }
            else //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_34_10bit_8PAIR_80to95MHz;
            }
        break;

        case E_PNL_LPLL_EPI28_4P:
            if (ldHz >= 1150000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8bit_4PAIR_115to150MHz;
            }
            else //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8bit_4PAIR_80to115MHz;
            }
        break;

        case E_PNL_LPLL_EPI28_6P:
            //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8bit_6PAIR_80to150MHz;
            }
        break;

        case E_PNL_LPLL_EPI28_8P:
            if (ldHz >= 1150000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8bit_8PAIR_115to150MHz;
            }
            else //if (ldHz >= 800000000UL)
            {
                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8bit_8PAIR_80to115MHz;
            }
        break;

        default:
            u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
        break;
    }
    return u8SupportedLPLLIndex;
}
static void _MHal_PNL_DumpLPLLTable(void *pInstance, E_PNL_SUPPORTED_LPLL_TYPE LPLLTblIndex)
{
    if (LPLLTblIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
        return;
    }

    int indexCounter = 0;

    for(indexCounter = 0 ; indexCounter<LPLL_REG_NUM; indexCounter++)
    {
        if (LPLLSettingTBL[LPLLTblIndex][indexCounter].address == 0xFF) //delay in micro second
        {
            MsOS_DelayTaskUs(LPLLSettingTBL[LPLLTblIndex][indexCounter].value);
            continue; // step forward to next register setting.
        }

        W2BYTEMSK(L_BK_LPLL(LPLLSettingTBL[LPLLTblIndex][indexCounter].address),
                  LPLLSettingTBL[LPLLTblIndex][indexCounter].value,
                  LPLLSettingTBL[LPLLTblIndex][indexCounter].mask);
    }

}

void MHal_PNL_Init_LPLL(void *pInstance,PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz);
    W2BYTEMSK(L_BK_LPLL(0x6D), 0x84, 0xFF);
    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("Not Supported LPLL Type, skip LPLL Init\n");
        return;
    }

    _MHal_PNL_DumpLPLLTable(pInstance, u8SupportedLPLLLIndex);


    MHal_MOD_PVDD_Power_Setting(pInstance, pPNLResourcePrivate->sthalPNL._bPVDD_2V5); // Einstein is always use 3.3V PVDD Power.
    //switch(eLPLL_Type)
    //{
    //    case E_PNL_TYPE_LVDS:

    //        if(eLPLL_Mode == E_PNL_MODE_SINGLE)
    //        {
    //            W2BYTEMSK(L_BK_LPLL(0x02), 0x2000, 0x3000);// [13:12]reg_lpll1_scalar_div_first
    //        }
    //        else
    //        {
    //            W2BYTEMSK(L_BK_LPLL(0x02), 0x1000, 0x3000);// [13:12] reg_lpll1_scalar_div_first
    //        }

    //        W2BYTEMSK(L_BK_LPLL(0x01), 0x203, 0xFF03); // [15:8]|[1:0]  reg_lpll1_loop_div_second | reg_lpll1_loop_div_first
    //        W2BYTEMSK(L_BK_LPLL(0x02), 0x0700, 0x0F00);// [11:8] reg_lpll1_scalar_div_second
    //        W2BYTEMSK(L_BK_LPLL(0x03), 0x12, 0x23F);// [9]|[5]|[4:2]|[1:0]  reg_lpll1_en_skew_div | reg_lpll1_pd | reg_lpll1_ibias_ictrl | reg_lpll1_icp_ictrl
    //        W2BYTEMSK(L_BK_LPLL(0x2E), 0x20, 0x6F);// [6]|[5]|[3:0]   reg_lpll_en_fifo | reg_lpll_vco_sel | reg_lpll_fifo_div
    //        W2BYTEMSK(L_BK_LPLL(0x30), 0x0, 0xFF00);// [15:8]  reg_lpll2_input_div_second
    //        W2BYTEMSK(L_BK_LPLL(0x31), 0x0, 0xFF03);// [15:8]|[1:0]  reg_lpll2_loop_div_second | reg_lpll2_loop_div_first
    //        W2BYTEMSK(L_BK_LPLL(0x32), 0x0, 0x3);// [1:0]  reg_lpll2_output_div_first
    //        W2BYTEMSK(L_BK_LPLL(0x33), 0x20, 0x3F);// [5]|[4:2]|[1:0]  reg_lpll2_pd | reg_lpll2_ibias_ictrl | reg_lpll2_icp_ictrl
    //        W2BYTEMSK(L_BK_LPLL(0x35), 0x1300, 0x7320);// [14:12]|[9:8]|[5]  reg_lpll1_skew_div |reg_lpll_pd_phdac |reg_lpll_2ndpll_clk_sel
    //        W2BYTEMSK(L_BK_LPLL(0x37), 0x0, 0x4006);// [14]|[2]|[1]  reg_lpll1_test[30] | reg_lpll1_test[18]| reg_lpll1_test[17]

    //        break;

    //    case E_PNL_TYPE_HS_LVDS:


    //        break;

    //    case E_PNL_TYPE_HF_LVDS:


    //        break;
    //    default:
    //    //Others
    //        break;
    //}

    //if( eLPLL_Type == E_PNL_TYPE_TTL)
    //    MHal_MOD_PVDD_Power_Setting(pInstance,FALSE);
    //else
    //    MHal_MOD_PVDD_Power_Setting(pInstance,TRUE);
}

MS_U8 MHal_PNL_Get_Loop_DIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U32 ldHz)
{
    MS_U16 u16loop_div = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, u8LPLL_Mode=%u, ldHz=%lu\n", __FUNCTION__, __LINE__, eLPLL_Type, u8LPLL_Mode, ldHz);
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,u8LPLL_Mode,ldHz);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Error LPLL type\n",__FUNCTION__,__LINE__);
        u16loop_div = 0 ;
    }
    else
    {
        u16loop_div = u16LoopDiv[u8SupportedLPLLLIndex];
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "E_PNL_LPLL_VIDEO : u16loop_div=%u\n", u16loop_div);

    u16loop_div *= 2;
    return u16loop_div;
    //MS_U16 u16loop_div=0;
    //switch(eLPLL_Type)
    //{
    //    case E_PNL_TYPE_MINILVDS:
    //    case E_PNL_TYPE_ANALOG_MINILVDS:
    //    case E_PNL_LPLL_MINILVDS_6P_1L:
    //        u16loop_div = 8;
    //    break;

    //    case E_PNL_TYPE_DIGITAL_MINILVDS:  //E_PNL_TYPE_MINILVDS_6P_2L
    //        u16loop_div = 4;
    //    break;

    //    case E_PNL_LPLL_MINILVDS_4P_2L:
    //        u16loop_div = 12;
    //    break;

    //    case E_PNL_LPLL_MINILVDS_4P_1L:
    //        u16loop_div = 6;
    //    break;

    //    case E_PNL_LPLL_MINILVDS_3P_2L:
    //        u16loop_div = 16;
    //    break;
    //    case E_PNL_LPLL_MINILVDS_3P_1L:
    //        u16loop_div = 8;
    //    break;

    //    case E_PNL_TYPE_LVDS:
    //        if(u8LPLL_Mode == E_PNL_MODE_SINGLE)
    //            u16loop_div = 28;
    //        else if(u8LPLL_Mode == E_PNL_MODE_DUAL)
    //            u16loop_div = 14;
    //    break;

    //    case E_PNL_TYPE_TTL:
    //    case E_PNL_TYPE_RSDS:
    //    case E_PNL_TYPE_MFC:
    //    case E_PNL_TYPE_DAC_I:
    //    case E_PNL_TYPE_DAC_P:
    //    case E_PNL_TYPE_PDPLVDS:
    //    default:
    //        u16loop_div = 7;
    //    break;
    //}
    //u16loop_div *= 2;

    //return u16loop_div;
}

MS_U16 MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U32 ldHz)
{
    MS_U8 u8loop_gain = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Error LPLL type\n",__FUNCTION__,__LINE__);
        u8loop_gain = 0 ;
    }
    else
    {
        u8loop_gain = u16LoopGain[u8SupportedLPLLLIndex];
    }

    return u8loop_gain;
    //MS_U16     u16loop_gain = 0;

    //switch(eLPLL_Type)
    //{
    //    case E_PNL_TYPE_MINILVDS:
    //    case E_PNL_TYPE_ANALOG_MINILVDS:
    //    case E_PNL_LPLL_MINILVDS_6P_1L:
    //        u16loop_gain = LPLL_LOOPGAIN;
    //    break;

    //    case E_PNL_TYPE_DIGITAL_MINILVDS:  //E_PNL_TYPE_MINILVDS_6P_2L
    //        u16loop_gain = 24;
    //    break;

    //    case E_PNL_LPLL_MINILVDS_4P_2L:
    //    case E_PNL_LPLL_MINILVDS_4P_1L:
    //        u16loop_gain = LPLL_LOOPGAIN;
    //    break;

    //    case E_PNL_LPLL_MINILVDS_3P_2L:
    //    case E_PNL_LPLL_MINILVDS_3P_1L:
    //        u16loop_gain = LPLL_LOOPGAIN;
    //    break;

    //    case E_PNL_TYPE_TTL:
    //    case E_PNL_TYPE_LVDS:
    //    case E_PNL_TYPE_RSDS:
    //    case E_PNL_TYPE_MFC:
    //    case E_PNL_TYPE_DAC_I:
    //    case E_PNL_TYPE_DAC_P:
    //    case E_PNL_TYPE_PDPLVDS:
    //    default:
    //        u16loop_gain = LPLL_LOOPGAIN;
    //    break;
    //}

    //return u16loop_gain;
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

void MHal_PNL_HWLVDSReservedtoLRFlag(void *pInstance,PNL_DrvHW_LVDSResInfo lvdsresinfo)
{
    if (lvdsresinfo.bEnable)
    {
        if (lvdsresinfo.u16channel & BIT(0))  // Channel A
        {
            if (lvdsresinfo.u32pair & BIT(3))  // pair 3
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, BIT(15), BIT(15));
            }
            if (lvdsresinfo.u32pair & BIT(4))  // pair 4
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, BIT(14), BIT(14));
            }
        }
        if (lvdsresinfo.u16channel & BIT(1))  // Channel B
        {
            if (lvdsresinfo.u32pair & BIT(3))  // pair 3
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, BIT(13), BIT(13));
            }
            if (lvdsresinfo.u32pair & BIT(4))  // pair 4
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, BIT(12), BIT(12));
            }
        }
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, 0x0000, (BIT(15) | BIT(14) | BIT(13) | BIT(12)));
    }
}


////////////////////////////////////////////////////////////////////////
// Turn OD function
////////////////////////////////////////////////////////////////////////
void MHal_PNL_OverDriver_Init(void *pInstance,MS_U32 u32OD_MSB_Addr, MS_U32 u32OD_MSB_limit, MS_U32 u32OD_LSB_Addr, MS_U32 u32OD_LSB_limit)
{
    SC_W2BYTE(0,REG_SC_BK16_15_L, (MS_U16)(u32OD_MSB_Addr & 0xFFFF)); // OD MSB request base address
    SC_W2BYTEMSK(0,REG_SC_BK16_16_L, (MS_U16)((u32OD_MSB_Addr >> 16) & 0x03FF), 0x03FF); // OD MSB request base address
    SC_W2BYTE(0,REG_SC_BK16_17_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD MSB request address limit
    SC_W2BYTEMSK(0,REG_SC_BK16_18_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x03FF), 0x03FF); // OD MSB request address limit

    SC_W2BYTE(0,REG_SC_BK16_39_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD frame buffer write address limit
    SC_W2BYTEMSK(0,REG_SC_BK16_3A_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer write address limit

    SC_W2BYTE(0,REG_SC_BK16_3B_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD frame buffer read address limit
    SC_W2BYTEMSK(0,REG_SC_BK16_3C_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer read address limit

    ///SC_W2BYTE(0,REG_SC_BK16_4F_L, (MS_U16)(u32OD_LSB_Addr & 0xFFFF)); // OD LSB request base address
    ///SC_W2BYTEMSK(0,REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_Addr >> 16) & 0x00FF), 0x00FF); // OD LSB request base address
    ///SC_W2BYTEMSK(0,REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_limit & 0x00FF) << 8), 0xFF00); // OD LSB request limit address
    ///SC_W2BYTE(0,REG_SC_BK16_51_L, (MS_U16)((u32OD_LSB_limit >> 8) & 0xFFFF)); // OD LSB request limit address

    SC_W2BYTE(0,REG_SC_BK16_1A_L, 0x4020); // OD request rFIFO limit threshold, priority threshold
    SC_W2BYTE(0,REG_SC_BK16_1C_L, 0x4020); // OD request wFIFO limit threshold, priority threshold
    SC_W2BYTEMSK(0,REG_SC_BK16_3A_L, 0x00, BIT(14)); // OD strength gradually bypass
    SC_W2BYTEMSK(0,REG_SC_BK16_3A_L, 0x2F00, 0x3F00);    // OD strength gradually slop
    SC_W2BYTEMSK(0,REG_SC_BK16_12_L, 0x0C, 0xFF);    // OD active threshold
}

void MHal_PNL_OverDriver_Enable(void *pInstance,MS_BOOL bEnable)
{
    // OD mode
    // OD used user weight to output blending directly
    // OD Enable
    if (bEnable)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_10_L, 0x2D, 0x2F);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_10_L, 0x2C, 0x2F);
    }
}

void MHal_PNL_OverDriver_TBL(void *pInstance,MS_U8 u8ODTbl[1056])
{
    // SRAM 1: 17x17 = 289
    // SRAM 2: 17x16 = 272
    // SRAM 3: 17x16 = 272
    // SRAM 4: 16x16 = 256
    MS_U16 i;
    MS_U8 u8target;
    MS_BOOL bEnable;

    bEnable = SC_R2BYTEMSK(0,REG_SC_BK16_10_L, BIT(0));
    SC_W2BYTEMSK(0,REG_SC_BK16_10_L, 0x00, BIT(0)); // OD enable
    SC_W2BYTEMSK(0,REG_SC_BK16_01_L, 0x0E, 0x0E); // OD table SRAM enable, RGB channel

    u8target= u8ODTbl[9];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_03_L, (i == 9)?u8target:(u8target ^ u8ODTbl[i]), 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_02_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_02_L, BIT(15)));
    }

    // temp patch for 33x33 for SRAM1
    for (i=272; i<289; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_03_L, 0x00, 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_02_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_02_L, BIT(15)));
    }

    u8target= u8ODTbl[(272+19)];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_06_L, (i == 19)?u8target:(u8target ^ u8ODTbl[(272+i)]), 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_05_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_05_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+29)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_09_L, (i == 29)?u8target:(u8target ^ u8ODTbl[(272*2+i)]), 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_08_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_08_L, BIT(15)));
    }

    // temp patch for 33x33 for SRAM3
    for (i=256; i<272; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_09_L, 0x00, 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_08_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_08_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+256+39)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(0,REG_SC_BK16_0C_L, (i == 39)?u8target:(u8target ^ u8ODTbl[(272*2+256+i)]), 0x00FF);
        SC_W2BYTEMSK(0,REG_SC_BK16_0B_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(0,REG_SC_BK16_0D_L, BIT(15)));
    }

    SC_W2BYTEMSK(0,REG_SC_BK16_01_L, 0x00, 0x0E); // OD table SRAM enable, RGB channel
    SC_W2BYTEMSK(0,REG_SC_BK16_10_L, bEnable, BIT(0)); // OD enable
}

MS_U16 _MHal_PNL_MOD_Swing_Refactor_AfterCAL(void *pInstance, MS_U16 u16Swing_Level)
{
    MS_U8 u8ibcal = 0x00;
    MS_U16 u16AfterCal_value = 0;
    MS_U16 u16Cus_value = 0;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_BK00_7D_L =>
    // 2'b00 250mV ' GCR_ICON_CHx[5:0]=2'h15 (decimal 21)
    // 2'b01 350mV ' GCR_ICON_CHx[5:0]=2'h1F (decimal 31)
    // 2'b10 300mV ' GCR_ICON_CHx[5:0]=2'h1A (decimal 26)
    // 2'b11 200mV ' GCR_ICON_CHx[5:0]=2'h10 (decimal 16)
    // =========
    switch(pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET)
    {
        default:
        case 0:
            u8ibcal = 0x15;
        break;
        case 1:
            u8ibcal = 0x1F;
        break;
        case 2:
            u8ibcal = 0x1A;
        break;
        case 3:
            u8ibcal = 0x10;
        break;
    }
    u16Cus_value = (u16Swing_Level) * (pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE + 4)/(u8ibcal + 4);
    u16AfterCal_value = (u16Cus_value-40)/10+2;

    HAL_MOD_CAL_DBG(printf("\r\n--Swing value after refactor = %d\n", u16AfterCal_value));

    return u16AfterCal_value;
}

MS_BOOL MHal_PNL_MOD_Control_Out_Swing(void *pInstance,MS_U16 u16Swing_Level)
{
    MS_BOOL bStatus = FALSE;
    MS_U16 u16ValidSwing = 0;
    MS_U16 u16ValidSwing_ext = 0;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)||
      (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_HS_LVDS))
    {
        if(u16Swing_Level>600)
            u16Swing_Level=600;
        if(u16Swing_Level<40)
            u16Swing_Level=40;

        u16ValidSwing = _MHal_PNL_MOD_Swing_Refactor_AfterCAL(pInstance, u16Swing_Level);
    }
    else
    {
        u16ValidSwing = u16Swing_Level;
    }

    u16ValidSwing     = u16ValidSwing & 0x3F;
    u16ValidSwing_ext = (u16ValidSwing & 0x40)>>6;

    MOD_W2BYTEMSK(REG_MOD_BK00_29_L,
        ( (u16ValidSwing << 4 )|(u16ValidSwing << 10 ) ) , 0xFFF0);

    MOD_W2BYTE(REG_MOD_BK00_2A_L,
        ((u16ValidSwing  )|(u16ValidSwing << 6 ) |(u16ValidSwing << 12 ) ));

    MOD_W2BYTE(REG_MOD_BK00_2B_L,
        ( (u16ValidSwing >> 4  )|(u16ValidSwing << 2 ) |(u16ValidSwing << 8 ) | (u16ValidSwing << 14 )));

    MOD_W2BYTE(REG_MOD_BK00_2C_L,
        ( (u16ValidSwing >> 2  )|(u16ValidSwing << 4 ) |(u16ValidSwing << 10 ) ) );

    MOD_W2BYTE(REG_MOD_BK00_2D_L,
        ( (u16ValidSwing  )|(u16ValidSwing << 6 ) |(u16ValidSwing << 12 ) ));

    MOD_W2BYTE(REG_MOD_BK00_2E_L,
        ( (u16ValidSwing >> 4 )|(u16ValidSwing << 2 )) );

    /// for extended bit, extended bit for reg_gcr_icon_ch0~ch13
    MOD_W2BYTE(REG_MOD_BK00_2F_L,
            (u16ValidSwing_ext     )|(u16ValidSwing_ext << 1 )|(u16ValidSwing_ext << 2 )|(u16ValidSwing_ext << 3 )|
            (u16ValidSwing_ext << 4)|(u16ValidSwing_ext << 5 )|(u16ValidSwing_ext << 6 )|(u16ValidSwing_ext << 7 )|
            (u16ValidSwing_ext << 8)|(u16ValidSwing_ext << 9 )|(u16ValidSwing_ext <<10 )|(u16ValidSwing_ext <<11 )|
            (u16ValidSwing_ext <<12)|(u16ValidSwing_ext <<13 )|(u16ValidSwing_ext <<14 )|(u16ValidSwing_ext <<15 )
            );

    bStatus = TRUE;

    return bStatus;
}


////////////////////////////////////////////////////////////////////////
// Turn Pre-Emphasis Current function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_PE_Current (void *pInstance,MS_U16 u16Current_Level)
{
    MS_BOOL bStatus = FALSE;
    MS_U16 u16ValidCurrent = u16Current_Level & 0x07;

    MOD_W2BYTEMSK(REG_MOD_BK00_23_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 3 )|(u16ValidCurrent << 6 )
        |(u16ValidCurrent << 9 ) |(u16ValidCurrent << 12 )) , 0x7FFF);

    MOD_W2BYTEMSK(REG_MOD_BK00_24_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 3 )|(u16ValidCurrent << 6 )
        |(u16ValidCurrent << 9 ) |(u16ValidCurrent << 12 )) , 0x7FFF);

    MOD_W2BYTEMSK(REG_MOD_BK00_25_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 3 )|(u16ValidCurrent << 6 )
        |(u16ValidCurrent << 9 ) |(u16ValidCurrent << 12 )) , 0x7FFF);

    MOD_W2BYTEMSK(REG_MOD_BK00_26_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 3 )|(u16ValidCurrent << 6 )
        |(u16ValidCurrent << 9 ) |(u16ValidCurrent << 12 )) , 0x7FFF);

    MOD_W2BYTEMSK(REG_MOD_BK00_29_L, u16ValidCurrent  ,0x0007);

    bStatus = TRUE;

    return bStatus;
}


////////////////////////////////////////////////////////////////////////
// 1.Turn TTL low-power mode function
// 2.Turn internal termination function
// 3.Turn DRIVER BIAS OP function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (void *pInstance,MS_BOOL bEnble)
{
    MS_BOOL bStatus = FALSE;
    if(bEnble)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_36_L, 0xFFFF, 0xFFFF); //Enable TTL low-power mode
        MOD_W2BYTEMSK(REG_MOD_BK00_37_L, 0x001E, 0x001E);

        MOD_W2BYTEMSK(REG_MOD_BK00_75_L, 0xFFFF, 0xFFFF); //GCR_EN_RINT (internal termination open)
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x003F, 0x003F);

        MOD_W2BYTEMSK(REG_MOD_BK00_79_L, 0xFFFF, 0xFFFF); //Disable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_7A_L, 0x003F, 0x003F);
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_36_L, 0x0000, 0xFFFF); //Disable TTL low-power mode
        MOD_W2BYTEMSK(REG_MOD_BK00_37_L, 0x0000, 0x001E);

        MOD_W2BYTEMSK(REG_MOD_BK00_75_L, 0x0000, 0xFFFF); //GCR_EN_RINT (internal termination close)
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x0000, 0x003F);

        MOD_W2BYTEMSK(REG_MOD_BK00_79_L, 0x0000, 0xFFFF); //Enable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_7A_L, 0x0000, 0x003F);
    }

    bStatus = TRUE;
    return bStatus;
}

void MHal_PNL_PreInit(void *pInstance,PNL_OUTPUT_MODE eParam)
{
    _eDrvPnlInitOptions = eParam;
}

PNL_OUTPUT_MODE MHal_PNL_Get_Output_MODE(void *pInstance)
{
    PNL_OUTPUT_MODE eParam = _eDrvPnlInitOptions;

    return eParam;
}

void MHal_PNL_SetOutputType(void *pInstance,PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type)
{
    MS_U16 u16ValidSwing2 = 0;
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

                //----------------------------------
                // Purpose: Set the output to be the GPO, and let it's level to Low
                // 1. External Enable, Pair 0~5
                // 2. GPIO Enable, pair 0~5
                // 3. GPIO Output data : All low, pair 0~5
                // 4. GPIO OEZ: output piar 0~5
                //----------------------------------

                //1.External Enable, Pair 0~5
                MOD_W2BYTEMSK(REG_MOD_BK00_46_L, 0x0FFF, 0x0FFF);
                //2.GPIO Enable, pair 0~5
                MOD_W2BYTEMSK(REG_MOD_BK00_4D_L, 0x0FFF, 0x0FFF);
                //3.GPIO Output data : All low, pair 0~5
                MOD_W2BYTEMSK(REG_MOD_BK00_4F_L, 0x0000, 0x0FFF);
                //4.GPIO OEZ: output piar 0~5
                MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, 0x0FFF);

                //1.External Enable, Pair 6~15
                MOD_W2BYTEMSK(REG_MOD_BK00_46_L, 0xF000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_47_L, 0xFFFF);
                //2.GPIO Enable, pair 6~15
                MOD_W2BYTEMSK(REG_MOD_BK00_4D_L, 0xF000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_4E_L, 0xFFFF);
                //3.GPIO Output data : All low, pair 6~15
                MOD_W2BYTEMSK(REG_MOD_BK00_4F_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_50_L, 0x0000);
                //4.GPIO OEZ: output piar 6~15
                MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_52_L, 0x0000);

                //1234.External Enable, Pair 16~17
                MOD_W2BYTE(REG_MOD_BK00_7E_L, 0xFF00);

                //1.External Enable, Pair 18~20, 2.GPIO Enable, pair 18~20
                MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, 0x3F3F, 0x3F3F);
                //3.GPIO Output data : All low, pair 18~20
                MOD_W2BYTEMSK(REG_MOD_BK00_7A_L, 0x0000, 0x3F00);
                //4.GPIO OEZ: output piar 18~20
                MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, 0x0000, 0xFC00);
                break;

            case E_PNL_OUTPUT_CLK_ONLY:
                MOD_W2BYTEMSK(REG_MOD_BK00_6D_L, 0, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_6E_L, 0x4004);
                MOD_W2BYTEMSK(REG_MOD_BK00_6F_L, 0, 0x000F);
                break;

            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:

                MOD_W2BYTEMSK(REG_MOD_BK00_4D_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_4E_L, 0x0000);
                //1. set GCR_PVDD_2P5=1・b0;           MOD PVDD power:    0: 3.3V
                MOD_W2BYTEMSK(REG_MOD_BK00_37_L, 0x00, BIT(6));
                //2. set PD_IB_MOD=1・b0;
                MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x00, BIT(0));
                //  save ch6 init value
                u16ValidSwing2 = (MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0x3F00)>>8);
                //3. set Desired Pairs: GCR_ICON[5:0]=6・h3f (current all open);
                MHal_PNL_MOD_Control_Out_Swing(pInstance,0x3F);
                //4. set Desired Pairs: GCR_PE_ADJ[2:0]=3・h7 (pre-emphasis current all open )
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance,0x07);
                //5. Enable low-power modeinternal termination Open, Enable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance,1);

                MsOS_DelayTask(1);

                //6. Enable low-power modeinternal termination Open, Enable OP
                MHal_Output_LVDS_Pair_Setting(pInstance,
                                              pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
                //pPNLResourcePrivate->stdrvPNL._stPnlInitData
                MHal_Shift_LVDS_Pair(pInstance,_u8MOD_LVDS_Pair_Shift);

                //7. set Desired Pairs: GCR_PE_ADJ[2:0]=3・h0 (pre-emphasis current all Close)
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance,0x00);
                //8. set Desired Pairs: GCR_ICON[5:0]    (current all init);
                MHal_PNL_MOD_Control_Out_Swing(pInstance,u16ValidSwing2);
                //9. Disable low-power modeinternal termination Close, Disable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance,0);

                // other TTL setting
                MOD_W2BYTE(REG_MOD_BK00_45_L, 0x003F);     // LVDS output enable, [5:4] Output enable: PANEL_LVDS/ PANEL_miniLVDS/ PANEL_RSDS

                MOD_W2BYTEMSK(REG_MOD_BK00_46_L, 0x0000, 0xF000);
                MOD_W2BYTE(REG_MOD_BK00_47_L, 0x0000);
                MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x000F);

                MOD_W2BYTEMSK(REG_MOD_BK00_44_L, 0x000, 0x3FF);    // TTL skew

                // GPO gating
                MOD_W2BYTEMSK(REG_MOD_BK00_4A_L, 0x0, BIT(8));     // GPO gating

            break;
        }
    }
}

void Mhal_PNL_Flock_LPLLSet(void *pInstance,MS_U32 ldHz)
{
    UNUSED(ldHz);
}


void MHal_PNL_MISC_Control(void *pInstance,MS_U32 u32PNL_MISC)
{
    if(u32PNL_MISC & E_DRVPNL_MISC_MFC_ENABLE)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, BIT(7), BIT(7));       // shift LVDS pair
    }
}

void MHal_PNL_Init_XC_Clk(void *pInstance,PNL_InitData *pstPanelInitData)
{
    // setup output dot clock
    W2BYTEMSK(REG_CKG_ODCLK, CKG_ODCLK_CLK_LPLL, CKG_ODCLK_MASK);      // select source tobe LPLL clock
    W2BYTEMSK(REG_CKG_ODCLK, DISABLE, CKG_ODCLK_INVERT);               // clock not invert
    W2BYTEMSK(REG_CKG_ODCLK, DISABLE, CKG_ODCLK_GATED);                // enable clock

    W2BYTEMSK(REG_CKG_BT656, CKG_BT656_CLK_LPLL, CKG_BT656_MASK);      // select source tobe LPLL clock
    W2BYTEMSK(REG_CKG_BT656, DISABLE, CKG_ODCLK_INVERT);               // clock not invert
    W2BYTEMSK(REG_CKG_BT656, DISABLE, CKG_ODCLK_GATED);                // enable clock
    W2BYTE(REG_CLKGEN0_4F_L,0x0000); //[3:0]CLK_VBY1_FIFO clock setting
    W2BYTE(L_CLKGEN0(0x57),0x0000); //[15:12]ckg_bt656 [3:0]ckg_fifo
    W2BYTE(L_CLKGEN0(0x58),0x0000); //[3:0]ckg_tx_mod

    W2BYTE(L_CLKGEN0(0x63),0x0000); //[11:8]ckg_tx_mod [3:0]ckg_osd2mod

    //W2BYTE(L_CLKGEN1(0x31), 0x0000); //[11:8]ckg_odclk_frc
}

void MHal_PNL_Init_MOD(void *pInstance,PNL_InitData *pstPanelInitData)
{

    //-------------------------------------------------------------------------
    // Set MOD registers
    //-------------------------------------------------------------------------

    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, pstPanelInitData->u16MOD_CTRL0, LBMASK);
    if(pstPanelInitData->eLPLL_Type == E_PNL_TYPE_TTL)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_40_L, (pstPanelInitData->u16OSTRL)<<4, 0xF00);
    }
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
    MOD_W2BYTEMSK(REG_MOD_BK00_77_L, 0x1C, 0xFC);       // original is MDrv_WriteByteMask(L_BK_MOD(0x77), 0x0F, BITMASK(7:2));
    //dual port lvds _end_//

    //MOD_W2BYTEMSK(REG_MOD_BK00_78_L, (_u8PnlDiffSwingLevel << 1), 0xFE);       //differential output swing level
    //if(!MHal_PNL_MOD_Control_Out_Swing(_u16PnlDefault_SwingLevel))
    //    printf(">>Swing Level setting error!!\n");
    MOD_W2BYTE(REG_MOD_BK00_73_L, pstPanelInitData->u16LVDSTxSwapValue);


    MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x0100,0x0100);
    // TODO: move from MDrv_Scaler_Init(), need to double check!
    MOD_W2BYTEMSK(REG_MOD_BK00_53_L, BIT(0), BIT(0));

    if (R1BYTEMSK(REG_CHIP_REVISION, 0xFF) > 0)
    {
        // it needs internal LDO for chip after U02
        MOD_W2BYTEMSK(REG_MOD_BK00_7B_L, 0x03, 0xFF); // for internal LDO
    }

    //--------------------------------------------------------------
    //Depend On Bitmode to set Dither
    //--------------------------------------------------------------


    // always enable noise dither and disable TAILCUT
    SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, ((pstPanelInitData->u8PanelNoiseDith ? XC_PAFRC_DITH_NOISEDITH_EN : (1 - XC_PAFRC_DITH_NOISEDITH_EN)) <<3) , BIT(3));
    SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, XC_PAFRC_DITH_TAILCUT_DISABLE, BIT(4));

    switch(pstPanelInitData->u8MOD_CTRLB & 0x03)//[1:0]ti_bitmode b'10:8bit  11:6bit  0x:10bit
    {
        case HAL_TI_6BIT_MODE:
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, BIT(2), BIT(2));
            break;

        case HAL_TI_8BIT_MODE:
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;

        case HAL_TI_10BIT_MODE:
        default:
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, 0x00, BIT(0));
            SC_W2BYTEMSK(0,REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;
            }


    //-----depend on bitmode to set Dither------------------------------
    MHal_PNL_SetOutputType(pInstance,_eDrvPnlInitOptions, pstPanelInitData->eLPLL_Type);     // TTL to Ursa

    MHal_PNL_MISC_Control(pInstance,pstPanelInitData->u32PNL_MISC);

}

void MHal_PNL_DumpMODReg(void *pInstance,MS_U32 u32Addr, MS_U16 u16Value, MS_BOOL bHiByte, MS_U16 u16Mask)
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
    pPNLResourcePrivate->sthalPNL._usMOD_CALI_OFFSET= pstModCaliInitData->u8ModCaliTarget;
    pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE = pstModCaliInitData->s8ModCaliOffset;
}

void MHal_BD_LVDS_Output_Type(void *pInstance,MS_U16 Type)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        _u8MOD_LVDS_Pair_Shift = LVDS_DUAL_OUTPUT_SPECIAL;
        _u8MOD_LVDS_Pair_Type = 1;
    }
    else
    {
        _u8MOD_LVDS_Pair_Type  = Type;
    }

    pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = _u8MOD_LVDS_Pair_Type;
}

MS_BOOL msModCalDDAOUT(void *pInstance)
{
   // W2BYTEMSK(BK_MOD(0x7D), ENABLE, 8:8);
   // MsOS_DelayTask(10);  //10ms
    return (MS_BOOL)((MOD_R2BYTEMSK(REG_MOD_BK00_7D_L, BIT(8))) >> 8);
}

MS_U8 msModCurrentCalibration(void *pInstance)
{
#if MOD_CAL_TIMER
    MS_U32 delay_start_time;
    delay_start_time=MsOS_GetSystemTime();
#endif

#if (!ENABLE_Auto_ModCurrentCalibration)
    return 0x60;
#else
    MS_U8 u8cur_ibcal=0;
    MS_U16 u16cur_ibcal[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    MS_U32 x=0;
    MS_U16 u16reg_3280,u16reg_3282,u16reg_328a;
    MS_U16 u16reg_32da,u16reg_32dc,u16reg_32de;


    u16reg_3280 = MOD_R2BYTEMSK(REG_MOD_BK00_40_L, LBMASK);
    u16reg_3282 = MOD_R2BYTEMSK(REG_MOD_BK00_41_L, LBMASK);
    u16reg_328a = MOD_R2BYTEMSK(REG_MOD_BK00_45_L, LBMASK);

    u16reg_32da = MOD_R2BYTE(REG_MOD_BK00_6D_L);
    u16reg_32dc = MOD_R2BYTE(REG_MOD_BK00_6E_L);
    u16reg_32de = MOD_R2BYTE(REG_MOD_BK00_6F_L);

// =========
// GCR_CAL_LEVEL[1:0] : REG_MOD_BK00_7D_L =>
// 2'b00 250mV ' GCR_ICON_CHx[5:0]=2'h15 (decimal 21)
// 2'b01 350mV ' GCR_ICON_CHx[5:0]=2'h1F (decimal 31)
// 2'b10 300mV ' GCR_ICON_CHx[5:0]=2'h1A (decimal 26)
// 2'b11 200mV ' GCR_ICON_CHx[5:0]=2'h10 (decimal 16)
// =========

    switch(_u8MOD_CALI_TARGET)
    {
        default:
        case 0:
            u8cur_ibcal = 0x19;
        break;
        case 1:
            u8cur_ibcal = 0x26;
        break;
        case 2:
            u8cur_ibcal = 0x14;
        break;
        case 3:
            u8cur_ibcal = 0x12;
        break;
    }

    //MOD_W2BYTEMSK(REG_MOD_BK00_2B_L, ((MS_U16)u8cur_ibcal)<<8, 0x3F00); // calibration initialized value

    //Set output config to be test clock output mode
    MOD_W2BYTE(REG_MOD_BK00_6D_L, 0xFFFF);
    MOD_W2BYTE(REG_MOD_BK00_6E_L, 0xFFFF);
    MOD_W2BYTE(REG_MOD_BK00_6F_L, 0xFFFF);

    // White Pattern for Calibration
    MOD_W2BYTEMSK(REG_MOD_BK00_32_L, BIT(15), BIT(15)); // Enable test enable of digi seri
    MOD_W2BYTEMSK(REG_MOD_BK00_41_L, 0x00, 0xFF);       // Set analog testpix output to low
    // (1) Set keep mode to auto write calibration result into register.
    MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, BIT(15), BIT(15));

    // (2) Set calibration step waiting time
    MOD_W2BYTE(REG_MOD_BK00_3B_L, 0x0080); // (about 5us)
    MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, 0x0000, 0x00FF);

    // (3) Set calibration toggle time
    MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, 0x0500, 0x0F00);
    // Set Calibration target
    MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, BIT(2), BIT(3)|BIT(2));  // Select calibration source pair, 01: channel 6
    MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, _u8MOD_CALI_TARGET, BIT(1)|BIT(0));    // Select calibration target voltage, 00: 200mv, 01:350mv, 10: 250mv, 11: 150mv
    MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, BIT(7), BIT(7));         // Enable calibration function

    HAL_MOD_CAL_DBG(printf("\r\n [%s](1)Init value:u8cur_ibcal = %d\n", __FUNCTION__, u8cur_ibcal));
    MS_U8 u8CheckTimes = 0;
    while(1)
    {
        // (7) Enable Hardware calibration
        MOD_W2BYTEMSK(REG_MOD_BK00_3D_L, BIT(15), BIT(15));

        // (8) Wait 2ms
        MsOS_DelayTask(2);

        // (10) Disable Hardware calibration
        MOD_W2BYTEMSK(REG_MOD_BK00_3D_L, 0x00, BIT(15));

        // (9)Check Finish and Fail flag bit
        //BK1032, 0x3D[14], Finish flag=1
        //BK1032, 0x3D[13], Fail flag=0
        if (MOD_R2BYTEMSK(REG_MOD_BK00_3D_L, 0x6000) == 0x4000)
        {
            printf("\033[0;31m [%s][%d] cal ok, break  \033[0m\n", __FUNCTION__, __LINE__);
            //u16cur_ibcal
            u16cur_ibcal[0]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0001)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_29_L, 0x3F0)>>4);   // ch0
            u16cur_ibcal[1]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0002)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_29_L, 0xFC00)>>10); // ch1

            u16cur_ibcal[2]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0004)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2A_L, 0x3F));       // ch2
            u16cur_ibcal[3]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0008)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2A_L, 0xFC0)>>6);   // ch3
            u16cur_ibcal[4]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0010)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2A_L, 0xF000)>>12) | (MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0x03))<<4;
            u16cur_ibcal[5]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0020)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0xFC)>>2);    // ch5
            u16cur_ibcal[6]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0040)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0x3F00)>>8);  // ch6
            u16cur_ibcal[7]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0080)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0xC000)>>14) | (MOD_R2BYTEMSK(REG_MOD_BK00_2C_L, 0x0F)<<2);
            u16cur_ibcal[8]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0100)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2C_L, 0x3F0)>>4);    // ch8
            u16cur_ibcal[9]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0200)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2C_L, 0xFC00)>>10);    // ch9

            u16cur_ibcal[10]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0400)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2D_L, 0x3F));       // ch10
            u16cur_ibcal[11]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x0800)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2D_L, 0xFC0)>>6);   // ch11
            u16cur_ibcal[12]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x1000)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2D_L, 0xF000)>>12) | (MOD_R2BYTEMSK(REG_MOD_BK00_2E_L, 0x03)<<4);
            u16cur_ibcal[13]=(MOD_R2BYTEMSK(REG_MOD_BK00_2F_L,0x2000)<<6)|  (MOD_R2BYTEMSK(REG_MOD_BK00_2E_L, 0xFC)>>2);    // ch13
            break;
        }
        else
        {
            u8CheckTimes ++;
            //printf("\033[0;31m [%s][%d] cal ng, u8CheckTimes: %d  \033[0m\n", __FUNCTION__, __LINE__, u8CheckTimes);
        }

        if (u8CheckTimes > 3)
        {
            // (13) If 3 times all fail, set all pair to nominal value by disable keep mode
            //MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, 0x00, BIT(15));
            printf("\033[0;31m [%s][%d] If 3 times all fail, set all pair to nominal value by disable keep mode  \033[0m\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, 0x00, BIT(15));

    MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, 0x00, BIT(7));  // Disable calibration function
    MOD_W2BYTEMSK(REG_MOD_BK00_32_L, 0x00, BIT(15)); // Disable test enable of digi seri

    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, u16reg_3280, LBMASK);
    MOD_W2BYTEMSK(REG_MOD_BK00_41_L, u16reg_3282, LBMASK);
    MOD_W2BYTEMSK(REG_MOD_BK00_45_L, u16reg_328a, LBMASK);

    MOD_W2BYTE(REG_MOD_BK00_6D_L, u16reg_32da);
    MOD_W2BYTE(REG_MOD_BK00_6E_L, u16reg_32dc);
    MOD_W2BYTE(REG_MOD_BK00_6F_L, u16reg_32de);

    if (u8CheckTimes <=3)
    {
        HAL_MOD_CAL_DBG(printf("\r\n----- Calibration ok \n"));
        for(x=0;x<14;x++)
        {
            //printf("\033[1;33m[%s:%d]BEF %d = %d\033[m\n",__FUNCTION__,__LINE__,x,u16cur_ibcal[x]);
            if((u16cur_ibcal[x]>=(u8cur_ibcal*1.6))||(u16cur_ibcal[x]<=(u8cur_ibcal*0.4)))
            {
                u16cur_ibcal[x]=u8cur_ibcal;
            }
            //printf("\033[1;33m[%s:%d]AFT %d = %d\033[m\n",__FUNCTION__,__LINE__,x,u16cur_ibcal[x]);
        }
    }
    else
    {
        HAL_MOD_CAL_DBG(printf("\r\n----- Calibration fail: 0x%lx \n", MOD_R2BYTEMSK(REG_MOD_BK00_3D_L, 0x6000)));
        //printf("\033[1;33m[%s:%d]BEF %d = %d\033[m\n",__FUNCTION__,__LINE__,x,u16cur_ibcal[x]);
        for(x=0;x<14;x++)
        {
            u16cur_ibcal[x]=u8cur_ibcal;
        }
        //printf("\033[1;33m[%s:%d]AFT %d = %d\033[m\n",__FUNCTION__,__LINE__,x,u16cur_ibcal[x]);
    }

    // Store the final value
    HAL_MOD_CAL_DBG(printf("\r\n (3)Store value = %d\n", u8cur_ibcal));

    _u8MOD_CALI_VALUE = (u8cur_ibcal & 0x3F);

    // copy the valur of ch6 to all channel( 0~20)
    MOD_W2BYTEMSK(REG_MOD_BK00_29_L,((MS_U16)(u16cur_ibcal[0]&0x3F))<<4, 0x3F0);   // ch0
    MOD_W2BYTEMSK(REG_MOD_BK00_29_L,((MS_U16)(u16cur_ibcal[1]&0x3F))<<10, 0xFC00); // ch1

    MOD_W2BYTEMSK(REG_MOD_BK00_2A_L,((MS_U16)(u16cur_ibcal[2]&0x3F)), 0x3F);       // ch2
    MOD_W2BYTEMSK(REG_MOD_BK00_2A_L,((MS_U16)(u16cur_ibcal[3]&0x3F))<<6, 0xFC0);   // ch3
    MOD_W2BYTEMSK(REG_MOD_BK00_2A_L,((MS_U16)(u16cur_ibcal[4]&0x3F))<<12, 0xF000); // ch4
    MOD_W2BYTEMSK(REG_MOD_BK00_2B_L,((MS_U16)(u16cur_ibcal[4]&0x3F))>>4, 0x03);
    MOD_W2BYTEMSK(REG_MOD_BK00_2B_L,((MS_U16)(u16cur_ibcal[5]&0x3F))<<2, 0xFC);    // ch5
    MOD_W2BYTEMSK(REG_MOD_BK00_2B_L,((MS_U16)(u16cur_ibcal[6]&0x3F))<<8, 0x3F00);  // ch6
    MOD_W2BYTEMSK(REG_MOD_BK00_2B_L,((MS_U16)(u16cur_ibcal[7]&0x3F))<<14, 0xC000);   // ch7
    MOD_W2BYTEMSK(REG_MOD_BK00_2C_L,((MS_U16)(u16cur_ibcal[7]&0x3F))>>2, 0x0F);
    MOD_W2BYTEMSK(REG_MOD_BK00_2C_L,((MS_U16)(u16cur_ibcal[8]&0x3F))<<4, 0x3F0);    // ch8
    MOD_W2BYTEMSK(REG_MOD_BK00_2C_L,((MS_U16)(u16cur_ibcal[9]&0x3F))<<10, 0xFC00);    // ch9

    MOD_W2BYTEMSK(REG_MOD_BK00_2D_L,((MS_U16)(u16cur_ibcal[10]&0x3F)), 0x3F);       // ch10
    MOD_W2BYTEMSK(REG_MOD_BK00_2D_L,((MS_U16)(u16cur_ibcal[11]&0x3F))<<6, 0xFC0);   // ch11
    MOD_W2BYTEMSK(REG_MOD_BK00_2D_L,((MS_U16)(u16cur_ibcal[12]&0x3F))<<12, 0xF000); // ch12
    MOD_W2BYTEMSK(REG_MOD_BK00_2E_L,((MS_U16)(u16cur_ibcal[12]&0x3F))>>4, 0x03);
    MOD_W2BYTEMSK(REG_MOD_BK00_2E_L,((MS_U16)(u16cur_ibcal[13]&0x3F))<<2, 0xFC);    // ch13
    MOD_W2BYTEMSK(REG_MOD_BK00_2F_L,0x0000, 0x1FFF);    // MSB

#if MOD_CAL_TIMER
    printf("[%s] takes %ld ms\n", __FUNCTION__, (MsOS_GetSystemTime()-delay_start_time));
#endif
    return (u8cur_ibcal&0x3F);//MOD_R2BYTEMSK(REG_MOD_BK00_2B_L, 0x3F00);
#endif
}

PNL_Result MHal_PNL_MOD_Calibration(void *pInstance)
{
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;


    u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

    W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

    u8Cab = msModCurrentCalibration(pInstance);

    W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));


    return E_PNL_OK;

}

static void MHal_PNL_PowerDownLPLL(void *pInstance,MS_BOOL bEnable)
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

PNL_Result MHal_PNL_En(void *pInstance,MS_BOOL bPanelOn, MS_BOOL bCalEn)
{
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "[%s][%d]\n", __FUNCTION__, __LINE__);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u32PNL_MISC = %x\n", (unsigned int)pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "bPanelOn = %x\n", bPanelOn);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "eLPLL_Type            = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "_u8MOD_LVDS_Pair_Type = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

    MS_U16 u16PortA = MOD_R2BYTE(REG_MOD_BK00_6D_L);
    MS_U16 u16PortB = MOD_R2BYTE(REG_MOD_BK00_6E_L);

    if(u16PortA!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7   = MOD_R2BYTE(REG_MOD_BK00_6D_L);
    if(u16PortB!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15  = MOD_R2BYTE(REG_MOD_BK00_6E_L);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "==========================\n\n");
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

    if(bPanelOn)
    {
        // The order is PanelVCC -> delay pnlGetOnTiming1() -> VOP -> MOD
        // VOP
        SC_W2BYTEMSK(0,REG_SC_BK10_46_L, 0x4000, HBMASK);

        // mod power on
        MHal_MOD_PowerOn(pInstance
                        , ENABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

        // turn on LPLL
        MHal_PNL_PowerDownLPLL(pInstance,FALSE);

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

            u8Cab = msModCurrentCalibration(pInstance);

            W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

        }
        if(!MHal_PNL_MOD_Control_Out_Swing(pInstance,_u16PnlDefault_SwingLevel))
            printf(">>Swing Level setting error!!\n");
    }
    else
{
        // The order is LPLL -> MOD -> VOP -> delay for MOD power off -> turn off VCC

        // LPLL
        // MHal_PNL_PowerDownLPLL(TRUE); //Remove to keep op vsync if panel off

        MHal_MOD_PowerOn(pInstance
                        , DISABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
        // VOP
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_I ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_P)//(bIsLVDS)
            SC_W2BYTEMSK(0,REG_SC_BK10_46_L, 0xFF, LBMASK);
        else
            SC_W2BYTEMSK(0,REG_SC_BK10_46_L, 0x00, 0xFF);
    }

    return E_PNL_OK;
}

void MHal_PNL_SetOutputPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
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

void MHal_PNL_Switch_LPLL_SubBank(void *pInstance,MS_U16 u16Bank)
{
    UNUSED(u16Bank);
}

void MHal_PNL_Switch_TCON_SubBank(void *pInstance,MS_U16 u16Bank)
{
    W2BYTEMSK(L_BK_TCON(0x00), u16Bank&0xff, 0xFF);
}

MS_U16 MHal_PNL_Read_TCON_SubBank(void *pInstance)
{
    return (MS_U16)R2BYTEMSK(L_BK_TCON(0x00),0xFF);
}

MS_BOOL MHal_PNL_IsYUVOutput(void *pInstance)
{
   return FALSE;
}

void MHal_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode)
{
}
void MHal_PNL_Set_Device_Bank_Offset(void *pInstance)
{
    UNUSED(pInstance);
    memset(u32PNL_XCDeviceBankOffset, 0, sizeof(MS_U32)*E_PNL_DEVICE_ID_NUM);
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_0] = E_HALPNL_DEVICE0_XC_BANK_OFFSET; // Set SC0 reg bank offset
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_1] = E_HALPNL_DEVICE1_XC_BANK_OFFSET; // Set SC2 reg bank offset
}
MS_BOOL MHal_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    return TRUE;
}
void MHal_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable)
{
}
void MHal_PNL_Init(void *pInstance)
{
}

MS_BOOL MHal_PNL_SetOutputInterlaceTiming(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if (bEnable == TRUE)
    {
        //interlace output vtotal modify
        SC_W2BYTEMSK(0, REG_SC_BK0F_2A_L, BIT(5)|BIT(8), BIT(5)|BIT(8));
        // two different interlace information through channel A reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_40_L, BIT(4) | BIT(7), BIT(4) | BIT(7));
        // two different interlace information through channel B reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, BIT(10)|BIT(11), BIT(10)|BIT(11));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_2A_L, 0x00, BIT(5)|BIT(8));
        MOD_W2BYTEMSK(REG_MOD_BK00_40_L, 0x00, BIT(4) | BIT(7));
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x00, BIT(10)|BIT(11));
    }

    return TRUE;
}

MS_BOOL MHal_PNL_GetOutputInterlaceTiming(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_BOOL bIsInterlaceOutput = FALSE;
    //interlace output vtotal modify
    if (SC_R2BYTEMSK(0, REG_SC_BK0F_2A_L, BIT(5)|BIT(8)) == (BIT(5)|BIT(8)))
    {
        if ((MOD_R2BYTEMSK(REG_MOD_BK00_40_L, BIT(4) | BIT(7)) == (BIT(4) | BIT(7)))
            || (MOD_R2BYTEMSK(REG_MOD_BK00_42_L, BIT(10) | BIT(11)) == (BIT(10)|BIT(11))))
        {
            bIsInterlaceOutput = TRUE;
        }
    }
    else
    {
        bIsInterlaceOutput = FALSE;
    }
    return bIsInterlaceOutput;
}

MS_U16 MHal_PNL_GetPanelVStart(void)
{
    return 0;
}
MS_U16 MHal_Pnl_Get_SupportMaxDclk(void *pInstance)
{
    return 0;
}
#endif

