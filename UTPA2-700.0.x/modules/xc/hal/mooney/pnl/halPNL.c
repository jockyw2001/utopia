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
#include "Mooney_pnl_lpll_tbl.h"
#include "Mooney_pnl_lpll_ext_tbl.h"

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

//Get MOD calibration time
#define MOD_CAL_TIMER   FALSE

//if mboot read eFuse and fill the register, then add this define to mark utopia efuse code flow
#define MOD_EFUSE_IN_MBOOT
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define LANE_NUM_EACH_PINMAPPING_GROUP1 4
#define LANE_NUM_EACH_PINMAPPING_GROUP2 4
#define LANE_NUM_EACH_PINMAPPING_GROUP3 4
#define LANE_NUM_EACH_PINMAPPING_GROUP4 2

#define PINMAPPING_EXP 16
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U8 LANE_AND_CLK_TBL[VBY1_CLK_TBL_ROW][3]=
{ //lane(from)  lane(to) bit(mask)
 { 0, 3, 0x02, },
 { 4, 6, 0x04, },
 { 7, 9, 0x08, },
 { 10, 13, 0x10, }
};
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MHal_PNL_Auto_Set_Config(void *pInstance,
                                      MS_U16 u16OutputOrder0_3,
                                      MS_U16 u16OutputOrder4_7,
                                      MS_U16 u16OutputOrder8_11,
                                      MS_U16 u16OutputOrder12_13);
#if 0
static void _MHal_PNL_Set_Clk(void *pInstance,
                              MS_U8 u8LaneNum,
                              MS_U16 u16OutputOrder0_3,
                              MS_U16 u16OutputOrder4_7,
                              MS_U16 u16OutputOrder8_11,
                              MS_U16 u16OutputOrder12_13);
#endif

static MS_U8 _MHal_PNL_Get_LaneNum(void *pInstance);

static MS_BOOL _Hal_MOD_External_eFuse(void);

//-------------------------------------------------------------------------------------------------
//  Global Function
//-------------------------------------------------------------------------------------------------
/**
*   @brief: Power On MOD. but not mutex protected
*
*/
MS_U8 MHal_MOD_PowerOn(void *pInstance, MS_BOOL bEn, MS_U8 u8LPLL_Type,MS_U8 DualModeType, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if( bEn )
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0x00, BIT(8));

        //analog MOD power down. 1: power down, 0: power up
        // For Mod2 no output signel
        ///////////////////////////////////////////////////

        //2. Power on MOD (current and regulator)
        if(u8LPLL_Type == E_PNL_TYPE_TTL)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, BIT(0) , BIT(0));
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0x00 , BIT(0));
        }
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, BIT(8) , BIT(8));


        // 3. 4. 5.
        MHal_Output_LVDS_Pair_Setting(pInstance, DualModeType, u16OutputCFG0_7, u16OutputCFG8_15, u16OutputCFG16_21);


        //enable ib, enable ck
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, (BIT(1) | BIT(0)), (BIT(1) | BIT(0)));

        // clock gen of dot-mini
        if(u8LPLL_Type == E_PNL_TYPE_MINILVDS)
        {
            MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x4400);
            MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x4400);
        }
        else if((u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE) ||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_8BIT_2LANE))

        {
            MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x0400);  // [11:8]reg_ckg_dot_mini_pre2_osd
            MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x0044);  // [3:0]reg_ckg_dot_mini_osd
                                                    // [7:4]reg_ckg_dot_mini_pre_osd
        }
        //// for osd dedicated output port, 1 port for video and 1 port for osd
        else if((u8LPLL_Type == E_PNL_TYPE_HS_LVDS)&&
                (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode == E_PNL_MODE_SINGLE))
        {
            MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x0400);  // [11:8]reg_ckg_dot_mini_pre2_osd
            MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x0044);  // [3:0]reg_ckg_dot_mini_osd
                                                    // [7:4]reg_ckg_dot_mini_pre_osd
        }
        #if 0 // this only for 8V4O case,  no need this for Maxim 23x23 8V case
        else if(u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
        {
            MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x0440);  // [11:8]reg_ckg_dot_mini_pre2_osd
            MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x0044);  // [3:0]reg_ckg_dot_mini_osd
        }
        #endif
        else
        {
            MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x0000);
            MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x0000);
        }

        MOD_W2BYTEMSK(REG_MOD_BK00_5B_L, 0x0008, 0x0008); // reg_fix_cnt_clr
        MOD_W2BYTEMSK(REG_MOD_BK00_5B_L, 0x0000, 0x0008);

        MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, 0x0080, 0x0080); // reg_proc_st_clr
        MOD_W2BYTEMSK(REG_MOD_BK00_7D_L, 0x0000, 0x0080);

        MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0080, 0x0080); // reg_unlock_cnt_clr
        MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x0080);

        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x0000, 0x8000);
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x8000, 0x8000);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, BIT(8), BIT(8));
        if(u8LPLL_Type !=E_PNL_TYPE_MINILVDS)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, BIT(0), BIT(0));                              //analog MOD power down. 1: power down, 0: power up
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0x00, BIT(8));
        }

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0, (BIT(1) | BIT(0) ));                           //enable ib, enable ck

        // clock gen of dot-mini
        MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x1100);
        MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x1100);

        if(  IsVBY1(u8LPLL_Type) )
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0020,0x0020);
            MsOS_DelayTask(1);
            MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000);
            MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0000);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0000,0x0020);
        }
    }
    return 1;
}

/**
*   @brief: Setup the PVDD power 1:2.5V, 0:3.3V
*
*/
void MHal_MOD_PVDD_Power_Setting(void *pInstance, MS_BOOL bIs2p5)
{
    //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, ((bIs2p5)? BIT(6):0), BIT(6));    //MOD PVDD=1: 0.9
}

void MHal_PNL_TCON_Init(void *pInstance)
{

}

void MHal_Shift_LVDS_Pair(void *pInstance, MS_U8 Type)
{
    if(Type == 1)
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, (BIT(7) | BIT(6)), (BIT(7) | BIT(6))); // shift_lvds_pair, set LVDS Mode3
    else
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x0000, (BIT(7) | BIT(6)));

}

void MHal_Output_LVDS_Pair_Setting(void *pInstance, MS_U8 Type, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{

    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0555);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x1554);
    }
    else if(Type == LVDS_SINGLE_OUTPUT_A)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0555);
    }
    else if( Type == LVDS_SINGLE_OUTPUT_B)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0555);
    }
    else if( Type == LVDS_OUTPUT_User)
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, u16OutputCFG0_7);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, u16OutputCFG8_15);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0555);
    }

    MsOS_DelayTask(2);


}

static MS_U8 _MHal_PNL_Get_LaneNum(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    MS_U8 u8LaneNum = 0;
    //check lane num
    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
     ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
    {
        u8LaneNum = 8;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
    {
        u8LaneNum = 4;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
    {
        u8LaneNum = 2;
    }
    else
    {
        u8LaneNum = 0;
    }
    return u8LaneNum;
}


static void _MHal_PNL_Auto_Set_Config(void *pInstance,
                                      MS_U16 u16OutputOrder0_3,
                                      MS_U16 u16OutputOrder4_7,
                                      MS_U16 u16OutputOrder8_11,
                                      MS_U16 u16OutputOrder12_13)
{
    //attention : This function just support vby1 now.
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));


    MS_U8   u8OutputConfigCount = 0;
    MS_U16  u16Config =0;
    MS_U8   u8Count = 0;
    MS_U8   u8LaneNum = 0;
    MS_BOOL bSkip = TRUE;

    //check lane num
    u8LaneNum = _MHal_PNL_Get_LaneNum(pInstance);
    if(u8LaneNum!=0)
    {
        bSkip = FALSE;
    }
    else
    {
        bSkip = TRUE;

        //use default config
        MHal_Output_LVDS_Pair_Setting(pInstance,
                                      pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
    }

    if(!bSkip)
    {
        //set output config
        u16Config = 0;
        u8OutputConfigCount = 0;
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP1 ; u8Count++ )
        {
            if( ( u16OutputOrder0_3 % PINMAPPING_EXP ) < u8LaneNum)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder0_3 = u16OutputOrder0_3 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP2 ; u8Count++ )
        {
            if( (u16OutputOrder4_7 % PINMAPPING_EXP ) < u8LaneNum)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder4_7 = u16OutputOrder4_7 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, u16Config);

        u16Config =0;
        u8OutputConfigCount = 0;
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP3 ; u8Count++ )
        {
            if( (u16OutputOrder8_11 % PINMAPPING_EXP ) < u8LaneNum)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder8_11 = u16OutputOrder8_11 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP4 ; u8Count++ )
        {
            if( (u16OutputOrder12_13 % PINMAPPING_EXP ) < u8LaneNum)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder12_13 = u16OutputOrder12_13 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, u16Config);

        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7 = MOD_A_R2BYTE(REG_MOD_A_BK00_00_L);
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15 = MOD_A_R2BYTE(REG_MOD_A_BK00_01_L);
    }
}

#if 0
static void _MHal_PNL_Set_Clk(void *pInstance,
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

    if(Type == APIPNL_OUTPUT_CHANNEL_ORDER_USER )
    {
        MS_U8 u8Clk = 0;
        MS_U8   u8LaneNum = 0;
        MS_BOOL bSkip = TRUE;
        MS_U8   u8Count = 0;
        MS_U8   u8Count1 = 0;
        MS_U8   u8StartLane = 0;

        //check lane num
        u8LaneNum = _MHal_PNL_Get_LaneNum(pInstance);
        if(u8LaneNum!=0)
        {
            bSkip = FALSE;
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x1F, 0x1F); //open all clk
            bSkip = TRUE;
        }

        //count clk
        if(!bSkip)
        {
            u8Clk = 0;
            u8StartLane = 0;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP1) ; u8Count++ )
            {//lane 0 - lane 3
                if( ( u16OutputOrder0_3 % PINMAPPING_EXP ) < u8LaneNum)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            u8Clk |= 0x01;//mod_a_reg38 [1]:clk enable
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder0_3 /= PINMAPPING_EXP;
            }

            u8StartLane = 4;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP2) ; u8Count++ )
            {//lane 4 - lane 7
                if( ( u16OutputOrder4_7 % PINMAPPING_EXP ) < u8LaneNum)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            u8Clk |= 0x01;//mod_a_reg38 [1]:clk enable
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder4_7 /= PINMAPPING_EXP;
            }

            u8StartLane = 8;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP3) ; u8Count++ )
            {//lane 8 - lane 11
                if( ( u16OutputOrder8_11 % PINMAPPING_EXP ) < u8LaneNum)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            u8Clk |= 0x01;//mod_a_reg38 [1]:clk enable
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder8_11 /= PINMAPPING_EXP;

            }

            u8StartLane = 12;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP4) ; u8Count++ )
            {//lane 12 - lane 13
                if( ( u16OutputOrder12_13 % PINMAPPING_EXP ) < u8LaneNum)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            u8Clk |= 0x01;//mod_a_reg38 [1]:clk enable
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder12_13 /= PINMAPPING_EXP;
            }

            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, u8Clk, 0x1F);
        }
    }
    else
    {
        if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x0F, 0x1F);
            }
            else
            {
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x0F, 0x1F);
            }
        }
        else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
        {//LVDS
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x1F, 0x1F);
        }
        else if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
                 ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x03, 0x1F);
        }
        else if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)
                 ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x03, 0x1F);
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x1F, 0x1F);
        }
    }
}
#endif

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
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1)) // 2 Divisoin
            {
                MOD_W2BYTE(REG_MOD_BK00_08_L, 0x0000);
                MOD_W2BYTE(REG_MOD_BK00_09_L, 0x6420);
                MOD_W2BYTE(REG_MOD_BK00_0A_L, 0x7531);
                MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
            }
            else
            {
                MOD_W2BYTE(REG_MOD_BK00_08_L, 0x0000);
                MOD_W2BYTE(REG_MOD_BK00_09_L, 0x3210);
                MOD_W2BYTE(REG_MOD_BK00_0A_L, 0x7654);
                MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
            }
        }
        else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
        {//LVDS
            MOD_W2BYTE(REG_MOD_BK00_08_L, 0x10DC);
            MOD_W2BYTE(REG_MOD_BK00_09_L, 0x5432);
            MOD_W2BYTE(REG_MOD_BK00_0A_L, 0x9876);
            MOD_W2BYTE(REG_MOD_BK00_0B_L, 0xFEBA);
        }
        else
        {// default is VB1 4lane/2lane
            MOD_W2BYTE(REG_MOD_BK00_08_L, 0x0000);
            MOD_W2BYTE(REG_MOD_BK00_09_L, 0x3210);
            MOD_W2BYTE(REG_MOD_BK00_0A_L, 0x7654);
            MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
        }
    }

}

void MHal_PQ_Clock_Gen_For_Gamma(void *pInstance)
{
    W2BYTEMSK(REG_CLKGEN0_52_L, 0x00, 0x07);
}

void MHal_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if(u8Mapping & GAMMA_MAPPING)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_74_L, (u8Mapping & GAMMA_10BIT_MAPPING)? BIT(15):0, BIT(15));
    }
    else
    {
        PNL_ASSERT(0, "Invalid eSupportGammaMapMode [%d] Passed to [%s], please make sure the u8Mapping[%d] is valid\n.",
                       u8Mapping, __FUNCTION__, u8Mapping);
    }
}

MS_BOOL Hal_VOP_Is_GammaMappingMode_enable(void *pInstance)
{
    // Only support 1024 entry
    return TRUE;
}

// After A5, 8 bit mode only support burst write!!!
MS_BOOL Hal_VOP_Is_GammaSupportSignalWrite(void *pInstance, DRVPNL_GAMMA_MAPPEING_MODE u8Mapping)
{
    if( u8Mapping == E_DRVPNL_GAMMA_10BIT_MAPPING )
        return TRUE;
    else
        return FALSE;
}


void hal_PNL_WriteGamma12Bit(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    MS_U16 u16Delay = 0xFFFF;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Write SC%tu [ch %d][addr 0x%x]: 0x%x \n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID, u8Channel, u16Addr, u16GammaValue);

    if (!bBurstWrite )
    {
        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
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

        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
    }
    else
    {

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_72_L, u16GammaValue, 0xFFF);
        SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK00_7F_L, 0x00); // make little time delay
    }


    PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

}


void hal_PNL_SetMaxGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Max gamma of SC%tu %d is 0x%x\n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID, u8Channel, u16MaxGammaValue);
#ifdef MONACO_SC2
    if(pPNLInstancePrivate->u32DeviceID == 0)
    {
#endif
           switch(u8Channel)
        {
            case 0:  // max. Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7D_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                break;

            case 1:  // max. Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7E_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;

            case 2:  //max.  Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7F_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;
           }
#ifdef MONACO_SC2
    }else    //Nike
    {
    switch(u8Channel)
    {
        case 0:  // max. Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7A_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7B_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;

        case 1:  // max. Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7C_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7D_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;

        case 2:  //max.  Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7E_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7F_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;
     }

    }
#endif
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

void Hal_PNL_Set12BitGammaPerChannel(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

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
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_71_L, 0x00 , 0x3FF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, 0x00 , BIT(6) | BIT(5) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(5) , BIT(6) | BIT(5) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(6) , BIT(6) | BIT(5) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(7) , BIT(7)); // Burst write enable

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
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, 0x00 , BIT(7));
    }

    hal_PNL_SetMaxGammaValue(pInstance,u8Channel, u16MaxGammaValue);
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
#ifdef MONACO_SC2
void Hal_PNL_Set12BitGammaPerChannel_SC2(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode )
{
    MS_U16 u16Addr             = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance,GammaMapMode);
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
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0x00 , BIT(0));
    }

    hal_PNL_SetMaxGammaValue(pInstance,u8Channel, u16MaxGammaValue);
}
#endif
// src : 1 (scaler lpll)
// src : 0 (frc lpll)
MS_U8 MHal_PNL_FRC_lpll_src_sel(void *pInstance, MS_U8 u8src)
{
    if (u8src > 1)
    {
        return FALSE;
    }
    else
    {
//Not support FRCINSIDE(frc lpll) for Monet
#if 0
        W2BYTEMSK(L_BK_LPLL(0x00), 0x00, 0x0F);
        W2BYTEMSK(L_BK_LPLL(0x7F), u8src?BIT(8):0, BIT(8));

        if(u8src==0)
        {
            W2BYTEMSK(L_BK_LPLL(0x00), 0x01, 0x0F);
            W2BYTEMSK(L_BK_LPLL(0x7F), BIT(8), BIT(8));
            W2BYTEMSK(L_BK_LPLL(0x00), 0x00, 0x0F); // restore to sub bnak 0
        }
#endif
        return TRUE;
    }

}

static MS_U8 _MHal_PNL_GetSupportedLPLLIndex(void *pInstance,
                                                                 PNL_TYPE eLPLL_Type,
                                                                 PNL_MODE eLPLL_Mode,
                                                                 MS_U64 ldHz, PNL_LPLL_TYPE_SEL lpll_type_sel)
{
    MS_U8 u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%lu, lpll_type_sel=%u\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz, lpll_type_sel);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%llu, lpll_type_sel=%u\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz, lpll_type_sel);
#endif

    /// Mini LVDS, EPI34/28, LVDS_1CH, Vx1_1P are 1P structure
    if(!((eLPLL_Type == E_PNL_TYPE_TTL)||
        ((eLPLL_Type == E_PNL_TYPE_LVDS)&&(eLPLL_Mode==E_PNL_MODE_SINGLE))||
        ((eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&(eLPLL_Mode==E_PNL_MODE_SINGLE))||
        (eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_1LANE)||(eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_1LANE)||
        ((eLPLL_Type >= E_PNL_LPLL_MINILVDS_2CH_3P_8BIT)&&(eLPLL_Type <= E_PNL_LPLL_MINILVDS_1CH_6P_6BIT))||
        ((eLPLL_Type >= E_PNL_LPLL_EPI34_2P)&&(eLPLL_Type <= E_PNL_LPLL_EPI28_4P))))
    {
        ldHz/=2;
    }

    switch(lpll_type_sel)
    {
        default:
        case E_PNL_LPLL_VIDEO:
        {
            switch (eLPLL_Type)
            {
                case E_PNL_TYPE_TTL:
                    if ((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_75to100MHz;
                    }
                    else if((ldHz >= 1000000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_100to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_75to75MHz;
                    }
                break;

                case E_PNL_TYPE_LVDS:
                    switch (eLPLL_Mode)
                    {
                        case E_PNL_MODE_SINGLE:
                            if ((ldHz >= 550000000UL) && (ldHz < 800000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_55to80MHz;
                            }
                            else if ((ldHz >= 400000000UL) && (ldHz < 550000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to55MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to40MHz;
                            }
                        break;

                        default:
                        case E_PNL_MODE_DUAL:
                            if ((ldHz >= 500000000UL) && (ldHz < 750000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz;
                            }
                            else if ((ldHz >= 375000000UL) && (ldHz < 500000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to50MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to37_5MHz;
                            }
                        break;
                    }
                break;

                case E_PNL_TYPE_HS_LVDS:

                    switch (eLPLL_Mode)
                    {
                        case E_PNL_MODE_SINGLE:
                            if((ldHz >= 1000000000UL) && (ldHz < 1500000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz;
                            }
                            else if((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to100MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to75MHz;
                            }
                        break;

                        default:
                        case E_PNL_MODE_DUAL:
                            if((ldHz >= 1000000000UL) && (ldHz < 1500000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz;
                            }
                            else if((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to100MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to75MHz;
                            }
                        break;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_1CH_3P_8BIT:
                    if((ldHz >= 400000000UL) && (ldHz < 800000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to80MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to40MHz;
                    }

                break;

                case E_PNL_LPLL_MINILVDS_2CH_3P_8BIT:
                    if((ldHz >= 750000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_6P_8BIT:
                    if((ldHz >= 750000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_1CH_3P_6BIT:
                    if((ldHz >= 666700000UL) && (ldHz < 800000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_66_67to80MHz;
                    }
                    else if((ldHz >= 400000000UL) && (ldHz < 666700000UL))
                    {
                       u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to66_67MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to40MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_3P_6BIT:
                    if ((ldHz <= 1333300000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_133_33to150MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1333300000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to133_33MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_6P_6BIT:
                    if ((ldHz <= 1333300000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_133_33to150MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1333300000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to133_33MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to75MHz;
                    }
                break;


#if 0
//not support in mooney
                case E_PNL_LPLL_EPI34_2P:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_2PAIR_150to150MHz;
                break;

                case E_PNL_LPLL_EPI34_4P:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_80to150MHz;
                break;

                case E_PNL_LPLL_EPI34_6P:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz;
                break;

                case E_PNL_LPLL_EPI34_8P:
                    if((ldHz >= 800000000UL) && (ldHz < 1880000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to188MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz;
                    }
                break;
#endif

                case E_PNL_LPLL_EPI28_2P:
                    if((ldHz >= 1150000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_115to150MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1150000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to115MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_4P:
                    if((ldHz >= 1150000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_115to150MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1150000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to115MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_6P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_8P:
                    if((ldHz >= 2300000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_230to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2300000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to230MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to150MHz;
                    }
                break;


                case E_PNL_LPLL_EPI28_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_16P:
                    if((ldHz >= 2000000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_200to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI24_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI24_16P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_6P_EPI3G:
                    if((ldHz >= 1800000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_180to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 1800000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to180MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_8P_EPI3G:
                    if((ldHz >= 2400000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_240to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2400000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to240MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to150MHz;
                    }
                break;


                case E_PNL_LPLL_CMPI24_10BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_CMPI27_8BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_USI_T_8BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_USI_T_10BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_8BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_8BIT_6P_D:
                    if((ldHz >= 2000000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_200to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_8BIT_8P:
                    if((ldHz >= 2490000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_249to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2490000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to249MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_10BIT_12P:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_10BIT_6P_D:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_ISP_10BIT_8P:
                    if((ldHz >= 1992000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_199_2to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 1992000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to199_2MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to150MHz;
                    }
                break;

/*
                case E_PNL_LPLL_EPI34_12P:
                break;
*/

                case E_PNL_LPLL_VBY1_10BIT_8LANE:
                    if((ldHz >= 1500000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to300MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_4LANE:
                    if((ldHz >= 750000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_2LANE:
                    if((ldHz >= 375000000UL) && (ldHz < 750000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_8LANE:
                    if((ldHz >= 2000000000UL) && (ldHz < 3000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_200to300MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_4LANE:
                    if((ldHz >= 1000000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_2LANE:
                    if((ldHz >= 500000000UL) && (ldHz < 750000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz;
                    }
                    else if((ldHz >= 375000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz;
                    }
                break;

                default:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
                break;
            }
        }
        break;
        case E_PNL_LPLL_OSD:
        {
            switch (eLPLL_Type)
            {
                case E_PNL_TYPE_HS_LVDS:
                {
                    if((ldHz >= 1000000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz;
                    }
                    else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz;
                    }
                    else if((ldHz >= 250000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz;
                    }
                }
                break;

                case E_PNL_LPLL_VBY1_10BIT_4LANE:
                    if((ldHz >= 1500000000UL) && (ldHz < 3400000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_2LANE:
                    if((ldHz >= 750000000UL) && (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_1LANE:
                    if((ldHz >= 375000000UL) && (ldHz < 750000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_4LANE:
                    if((ldHz >= 2000000000UL) && (ldHz < 3400000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_2LANE:
                    if((ldHz >= 1000000000UL)&& (ldHz < 1500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz;
                    }
                    else if((ldHz >= 750000000UL)&& (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_1LANE:
                    if((ldHz >= 500000000UL) && (ldHz < 750000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz;
                    }
                    else if((ldHz >= 375000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz;
                    }
                break;

                default:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
                break;
            }
        }
        break;
    }
    return u8SupportedLPLLIndex;
}

static void _MHal_PNL_DumpLPLLTable(void *pInstance, MS_U8 LPLLTblIndex, PNL_LPLL_TYPE_SEL lpll_type_sel)
{
    if(lpll_type_sel == E_PNL_LPLL_VIDEO)
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
        PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, LPLLTblIndex);
    }
    else
    {
        if (LPLLTblIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
        {
            printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
            return;
        }

        int indexCounter = 0;

        for(indexCounter = 0 ; indexCounter<LPLL_EXT_REG_NUM; indexCounter++)
        {
            if (LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].address == 0xFF) //delay in micro second
            {
                MsOS_DelayTaskUs(LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].value);
                continue; // step forward to next register setting.
            }

            W2BYTEMSK(L_BK_LPLL(LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].address),
                      LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].value,
                      LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].mask);
        }
        PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, LPLLTblIndex);
    }
}

void MHal_PNL_Init_LPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz, E_PNL_LPLL_VIDEO);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("Not Supported LPLL Type, skip LPLL Init\n");
        return;
    }

    _MHal_PNL_DumpLPLLTable(pInstance, u8SupportedLPLLLIndex, E_PNL_LPLL_VIDEO);


    MHal_MOD_PVDD_Power_Setting(pInstance, pPNLResourcePrivate->sthalPNL._bPVDD_2V5); // Einstein is always use 3.3V PVDD Power.
}

MS_U8 MHal_PNL_Get_Loop_DIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_div = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, u8LPLL_Mode=%u, ldHz=%lu\n", __FUNCTION__, __LINE__, eLPLL_Type, u8LPLL_Mode, ldHz);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, u8LPLL_Mode=%u, ldHz=%llu\n", __FUNCTION__, __LINE__, eLPLL_Type, u8LPLL_Mode, ldHz);
#endif
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,u8LPLL_Mode,ldHz,E_PNL_LPLL_VIDEO);

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
}

MS_U16 MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_gain = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%lu\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%llu\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz);
#endif
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz,E_PNL_LPLL_VIDEO);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Error LPLL type\n",__FUNCTION__,__LINE__);
        u16loop_gain = 0 ;
    }
    else
    {
        u16loop_gain = u16LoopGain[u8SupportedLPLLLIndex];
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "E_PNL_LPLL_VIDEO : u16loop_gain=%u\n", u16loop_gain);
    return u16loop_gain;
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
    if (bSetMode == TRUE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, 0, BIT(15));
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, BIT(15), BIT(15));
    }
}

void MHal_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

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

        if(  IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_30_L, BIT(14), BIT(14)); //reg_sel_ext_bit: sel extend bit, 0: osd_de 1: three_d_flag
        }
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, 0x0000, (BIT(15) | BIT(14) | BIT(13) | BIT(12)));

        if(  IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_30_L, 0x00, BIT(14)); //reg_sel_ext_bit: sel extend bit, 0: osd_de 1: three_d_flag
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Turn OD function
////////////////////////////////////////////////////////////////////////
void MHal_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_MSB_limit, MS_U32 u32OD_LSB_Addr, MS_U32 u32OD_LSB_limit, MS_U8 u8MIUSel)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK16_61_L,u8MIUSel<<8,BIT(8)|BIT(9)); // OD MIU select

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_15_L, (MS_U16)(u32OD_MSB_Addr & 0xFFFF)); // OD MSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_16_L, (MS_U16)((u32OD_MSB_Addr >> 16) & 0x00FF), 0x00FF); // OD MSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_60_L, (MS_U16)((u32OD_MSB_Addr >> 24) & 0x0003), 0x0003); // OD MSB request base address
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_17_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD MSB request address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_18_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x00FF), 0x00FF); // OD MSB request address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_60_L, (MS_U16)((u32OD_MSB_limit >> 24) & 0x0003)<<2, 0x000C); // OD MSB request address limit
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
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, 0x00, BIT(14)); // OD strength gradually bypass
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, 0x2F00, 0x3F00);    // OD strength gradually slop
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_12_L, 0x0C, 0xFF);    // OD active threshold

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
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
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

MS_U16 MHal_PNL_MODSwingRegToRealLevelValue(void *pInstance, MS_U16 u16SwingRegValue)
{
    MS_U8 u8ibcal = 0x00;
    MS_U16 u16SwingRealLevelValue = 0;
    MS_U16 u16CusValue = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
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

    u16CusValue =  ((u16SwingRegValue-2)*10)+40;
    u16SwingRealLevelValue=(u16CusValue*(u8ibcal + 4))/(pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE + 4);

    HAL_MOD_CAL_DBG(printf("\r\n--Swing Real Level Value = %d\n", u16SwingRealLevelValue));

    return u16SwingRealLevelValue;
}

MS_BOOL MHal_PNL_MOD_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level)
{
    MS_BOOL bStatus = FALSE;

    MS_U16 u16ValidSwing = 0;
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

    // Disable HW calibration keep mode first, to make SW icon value can write into register.
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, 0, BIT(15));

    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_LPLL_VBY1_10BIT_4LANE)&&
       (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type <= E_PNL_LPLL_VBY1_8BIT_8LANE))
    {
        u16ValidSwing &=0x0F;
        // vby1 vreg
        // ch0+ch1+ch2+ch3
        MOD_A_W2BYTE(REG_MOD_A_BK00_20_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
        // ch4+ch5+ch6+ch7
        MOD_A_W2BYTE(REG_MOD_A_BK00_21_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
        // ch8+ch9+ch10+ch11
        MOD_A_W2BYTE(REG_MOD_A_BK00_22_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
        // ch12+ch13+ch14+ch15
        MOD_A_W2BYTE(REG_MOD_A_BK00_23_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
    }
    else
    {
        u16ValidSwing &=0xFF;
        // LVDS fill ICON
        // ch0+ch1
        MOD_A_W2BYTE(REG_MOD_A_BK00_08_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch2+ch3
        MOD_A_W2BYTE(REG_MOD_A_BK00_09_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch4+ch5
        MOD_A_W2BYTE(REG_MOD_A_BK00_0A_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch6+ch7
        MOD_A_W2BYTE(REG_MOD_A_BK00_0B_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch8+ch9
        MOD_A_W2BYTE(REG_MOD_A_BK00_0C_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch10+ch11
        MOD_A_W2BYTE(REG_MOD_A_BK00_0D_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch12+ch13
        MOD_A_W2BYTE(REG_MOD_A_BK00_0E_L, (u16ValidSwing << 8 | u16ValidSwing));
        // ch14+ch15
        MOD_A_W2BYTE(REG_MOD_A_BK00_0F_L, (u16ValidSwing << 8 | u16ValidSwing));
    }
    bStatus = TRUE;

    return bStatus;
}

////////////////////////////////////////////////////////////////////////
// Turn Pre-Emphasis Current function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_PE_Current (void *pInstance, MS_U16 u16Current_Level)
{
    MS_BOOL bStatus = FALSE;
    MS_U16 u16ValidCurrent = u16Current_Level & 0x0F;

    MOD_A_W2BYTE(REG_MOD_A_BK00_18_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_19_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_1A_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_1B_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    bStatus = TRUE;

    return bStatus;
}

void MHal_PNL_MOD_PECurrent_Setting(void *pInstance, MS_U16 u16Current_Level, MS_U16 u16Channel_Select)
{
    MS_U16 u16ValidCurrent = u16Current_Level & 0x0F;
    MS_U16 u16Ch00_03_mask,u16Ch04_07_mask,u16Ch08_11_mask,u16Ch12_15_mask  = 0;

    u16Ch00_03_mask = (((u16Channel_Select & BIT(0))? 0x000F:0x00)|((u16Channel_Select & BIT(1))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(2))? 0x0F00:0x00)|((u16Channel_Select & BIT(3))? 0xF000:0x00));
    u16Ch04_07_mask = (((u16Channel_Select & BIT(4))? 0x000F:0x00)|((u16Channel_Select & BIT(5))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(6))? 0x0F00:0x00)|((u16Channel_Select & BIT(7))? 0xF000:0x00));
    u16Ch08_11_mask = (((u16Channel_Select & BIT(8))? 0x000F:0x00)|((u16Channel_Select & BIT(9))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(10))? 0x0F00:0x00)|((u16Channel_Select & BIT(11))? 0xF000:0x00));
    u16Ch12_15_mask = (((u16Channel_Select & BIT(12))? 0x000F:0x00)|((u16Channel_Select & BIT(13))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(14))? 0x0F00:0x00)|((u16Channel_Select & BIT(15))? 0xF000:0x00));

    if(u16Ch00_03_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_18_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch00_03_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L,0x00);
    }

    if(u16Ch04_07_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_19_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch04_07_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L,0x00);
    }

    if(u16Ch08_11_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_1A_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch08_11_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_1A_L,0x00);
    }

    if(u16Ch12_15_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_1B_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch12_15_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_1B_L,0x00);
    }
}

////////////////////////////////////////////////////////////////////////
// 1.Turn TTL low-power mode function
// 2.Turn internal termination function
// 3.Turn DRIVER BIAS OP function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (void *pInstance, MS_BOOL bEnble)
{
    MS_BOOL bStatus = FALSE;
    if(bEnble)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_34_L, 0xFFFF, 0xFFFF); //Enable TTL low-power mode
//        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0x001E, 0x001E);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_32_L, 0xFFFF, 0xFFFF); //GCR_EN_RINT (internal termination open)
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x003F, 0x003F);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3C_L, 0xFFFF, 0xFFFF); //Disable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_7A_L, 0x003F, 0x003F);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_34_L, 0x0000, 0xFFFF); //Disable TTL low-power mode
//        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0x0000, 0x001E);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_32_L, 0x0000, 0xFFFF); //GCR_EN_RINT (internal termination close)
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x0000, 0x003F);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3C_L, 0x0000, 0xFFFF); //Enable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_7A_L, 0x0000, 0x003F);
    }

    bStatus = TRUE;
    return bStatus;
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

MS_U32 msReadEfuse(void *pInstance, MS_U8 u8Bank, MS_U32 u32Mask)
{
    MS_U32 u32Result = 0;
    MS_U8 u8Count = 0;

    W2BYTEMSK(0x2050, u8Bank<<2, BMASK(8:2));  /// reg28[8:2]Addr 6~0
    W2BYTEMSK(0x2050, BIT(13), BIT(13));       /// Reg28[13] Margin Read
    while(R2BYTEMSK(0x2050, BIT(13)) == BIT(13))
    {
        MsOS_DelayTask(1);
        u8Count ++;

        if (u8Count >10)
            break;
    }

    u32Result = (R4BYTE(0x2058)& u32Mask);    /// reg2C,2D read value
    printf("[%s][%d]u32Result=%x, after mask u32Result=%x\n", __FUNCTION__, __LINE__, R4BYTE(0x2058), u32Result);
    return u32Result;

}

void msSetVBY1RconValue(void *pInstance)
{
    MS_U16 u16DefaultICON_Max = 40, u16DefaultICON_Min = 7;
    MS_U16 u16DefaultICON = 18;
    MS_U32 u32Mask = 0x3F;
    MS_BOOL bEfuseMode = FALSE;
    MS_U16 u16SwingOffset = 0;  // by HW RD request
    MS_U16 u16temp = 0;

    if(!(_Hal_MOD_External_eFuse()))
    {
        if (msReadEfuse(pInstance, 0x4E, BIT(6)) == BIT(6))
            bEfuseMode = TRUE;

        // Disable HW calibration keep mode first, to make SW icon value can write into register.
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, 0, BIT(15));

        if (bEfuseMode)
        {
            if(((MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset) > u16DefaultICON_Max)
                u16temp = u16DefaultICON;
            else if(((MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset) < u16DefaultICON_Min)
                u16temp = u16DefaultICON;
            else
                u16temp = (MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset;
        }
        else
        {
            u16temp = u16DefaultICON;
        }

        //ch0~ch13 rcon setting
        u16temp &= (u16temp&(MS_U16)u32Mask);
        printf("[%s][%d]u16temp= %x\n", __FUNCTION__, __LINE__, u16temp);

        MOD_A_W2BYTE(REG_MOD_A_BK00_10_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_11_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_12_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_13_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_14_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_15_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_16_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_17_L, (u16temp<<8|u16temp));
    }
}

void MHal_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type)
{
    MS_U16 u16ValidSwing2 = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if( eLPLL_Type == E_PNL_TYPE_TTL)
    {
        // select pair output to be TTL
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0020,0x0020);
        MsOS_DelayTask(1);
        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0000);
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0000,0x0020);

        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0x0000, (BIT(7) | BIT(6))); // shift_lvds_pair

        // other TTL setting
        MOD_A_W2BYTE(REG_MOD_A_BK00_68_L, 0x0000);     // TTL output enable

        MOD_A_W2BYTE(REG_MOD_A_BK00_40_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_41_L, 0x0000);
        MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0xE000);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, 0x3FF, 0x3FF);       // TTL skew

        MOD_W2BYTEMSK(REG_MOD_BK00_20_L, 0x0001, 0x0007);//[2:0]reg_mft_mode

        MOD_A_W2BYTE(REG_MOD_A_BK00_38_L, 0x0000); // enable clk_dot_mini_pre_osd & clk_dot_mini_osd

        // GPO gating
        MOD_W2BYTEMSK(REG_MOD_BK00_4A_L, BIT(8), BIT(8));         // GPO gating
    }
    else if(( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE))
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_34_L, 0xF000, 0xF000);  //[15:14]datax[13:12]data_format3,2

        // rcon
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & (MS_U32)E_APIPNL_MISC_SKIP_ICONVALUE)
        {
            HAL_MOD_CAL_DBG(printf("User define Swing Value=%u\n", __FUNCTION__, __LINE__, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel));
            MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel);
        }
        else
        {
            HAL_MOD_CAL_DBG(printf("Use RconValue\n", __FUNCTION__, __LINE__));
            msSetVBY1RconValue(pInstance);
        }

        // rint
        MOD_A_W2BYTE(REG_MOD_A_BK00_32_L, 0x0000);

        MOD_A_W2BYTE(REG_MOD_A_BK00_68_L, 0x003f);

        //-------------------------------------
        //## pe
//        MOD_A_W2BYTE(REG_MOD_A_BK00_30_L, 0x3fff);
//        MOD_W2BYTE(REG_MOD_BK00_23_L, 0x7000);
//        MOD_W2BYTE(REG_MOD_BK00_24_L, 0x7fff);
//        MOD_W2BYTE(REG_MOD_BK00_25_L, 0x003f);

        // VBY1 setting
        MOD_A_W2BYTE(REG_MOD_A_BK00_71_L, 0xFFFF);

        MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8F3F); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt
//        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0xA040, 0xFDCF); //[15]proc_st[13:12]byte_mode 4 byte mode[6]4ch_vby1[9]swap

        MOD_A_W2BYTE(REG_MOD_A_BK00_40_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_41_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_48_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_49_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_42_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_43_L, 0xFFFF);

        switch(eOutputMode)
        {
            case E_PNL_OUTPUT_NO_OUTPUT:
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0020,0x0020);
                MsOS_DelayTask(1);
                MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0000);
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0000,0x0020);
                if(1)//( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
                {
                    //-------------------------------------
                    //## icon (Swing)
                    MOD_A_W2BYTE(REG_MOD_A_BK00_08_L, 0x0000);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_09_L, 0x0000);

                    //-------------------------------------
                    //vby1
                    MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8f3f); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt
                    //MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0xa040, 0xFDCF); //[15]proc_st[13:12]byte_mode 4 byte mode[6]4ch_vby1[9]swap
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)
                {
                    //-------------------------------------
                    //## icon (Swing)
                    MOD_A_W2BYTE(REG_MOD_A_BK00_08_L, 0x0000);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_09_L, 0x0000);

                    //vby1
                    MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8f3f);
                    //MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0xa040, 0xFDCF);
                }
                break;

            case E_PNL_OUTPUT_CLK_ONLY:
            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:
                if(eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
                {
                    //MOD_A_W2BYTE(REG_MOD_A_BK00_3A_L, 0xC100);
                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x5500); //[15:0]reg_output_conf[15:0]
                        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0055); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
                {
                    //MOD_A_W2BYTE(REG_MOD_A_BK00_3A_L, 0xC000);
                    MOD_W2BYTE(REG_MOD_BK00_4A_L, 0x0002); //[1]reg_dualmode[0]abswitch
                    MOD_A_W2BYTE(REG_MOD_A_BK00_0A_L, 0x7f7f);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_0B_L, 0x7f7f);

                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000); //[15:0]reg_output_conf[15:0]
                        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0055); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)
                {
                    //MOD_A_W2BYTE(REG_MOD_A_BK00_3A_L, 0xC000);
                    MOD_W2BYTE(REG_MOD_BK00_4A_L, 0x0000);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_0A_L, 0x7f7f);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_0B_L, 0x0000);

                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000);
                        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0005); //[15:0]reg_output_conf[15:0]
                    }

                }
                break;
        }
    }
    //// for osd dedicated output port, 1 port for video and 1 port for osd
    else if((eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode == E_PNL_MODE_SINGLE))
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0xC01F , 0xFFFF); // enable clk_dot_mini_pre_osd & clk_dot_mini_osd
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, BIT(3), 0xFFFF ); // enable osd lvds path
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, BIT(15), BIT(15)); //[15]sw_rst
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0xC000 , 0xF000); // enable clk_dot_mini_pre_osd & clk_dot_mini_osd
    }
    else
    {
        switch(eOutputMode)
        {
            case E_PNL_OUTPUT_NO_OUTPUT:
                // if MOD_45[5:0] = 0x3F && XC_MOD_EXT_DATA_EN_L = 0x0,
                // then if XC_MOD_OUTPUT_CONF_L = 0x0 ---> output TTL as tri-state
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0020,0x0020);
                MsOS_DelayTask(1);
                MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0000);
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3B_L, 0x0000,0x0020);

                //----------------------------------
                // Purpose: Set the output to be the GPO, and let it's level to Low
                // 1. External Enable, Pair 0~5
                // 2. GPIO Enable, pair 0~5
                // 3. GPIO Output data : All low, pair 0~5
                // 4. GPIO OEZ: output piar 0~5
                //----------------------------------

                //1.External Enable, Pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_40_L, 0x0FFF, 0x0FFF);
                //2.GPIO Enable, pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_48_L, 0x0FFF, 0x0FFF);
                //3.GPIO Output data : All low, pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_44_L, 0x0000, 0x0FFF);
                //4.GPIO OEZ: output piar 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_42_L, 0x0000, 0x0FFF);

                //1.External Enable, Pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_40_L, 0xF000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_41_L, 0xFFFF);
                //2.GPIO Enable, pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_48_L, 0xF000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_49_L, 0xFFFF);
                //3.GPIO Output data : All low, pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_44_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_45_L, 0x0000);
                //4.GPIO OEZ: output piar 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_42_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_43_L, 0x0000);

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
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x4004);
                break;

            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:
                // LVDS default setting
                MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x5550);
                MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0x0555);

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_48_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_49_L, 0x0000);
                //1. set GCR_PVDD_2P5=1¡¦b1;           MOD PVDD power:    1: 2.5V
                //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0, BIT(6));
                //2. set PD_IB_MOD=1¡¦b0;
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0x00, BIT(0));
                //  save ch6 init value
                u16ValidSwing2 = (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_0B_L, 0x3F00)>>8);
                //3. set Desired Pairs: GCR_ICON[5:0]=6h3f (current all open);
                MHal_PNL_MOD_Control_Out_Swing(pInstance, MHal_PNL_MODSwingRegToRealLevelValue(pInstance, 0x3F));
                //4. set Desired Pairs: GCR_PE_ADJ[2:0]=3h7 (pre-emphasis current all open )
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance, 0x07);
                //5. Enable low-power modeinternal termination Open, Enable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance, 1);

                MsOS_DelayTask(1);

                //6. Enable low-power modeinternal termination Open, Enable OP
                MHal_Output_LVDS_Pair_Setting(pInstance,
                                              pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
                MHal_Shift_LVDS_Pair(pInstance, pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift);

                //7. set Desired Pairs: GCR_PE_ADJ[2:0]=3¡¦h0 (pre-emphasis current all Close)
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance, 0x00);
                //8. set Desired Pairs: GCR_ICON[5:0]    (current all init);
                MHal_PNL_MOD_Control_Out_Swing(pInstance, MHal_PNL_MODSwingRegToRealLevelValue(pInstance, u16ValidSwing2));
                //9. Disable low-power modeinternal termination Close, Disable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance, 0);

                // other TTL setting
                MOD_A_W2BYTE(REG_MOD_A_BK00_68_L, 0x003F);     // LVDS output enable, [5:4] Output enable: PANEL_LVDS/ PANEL_miniLVDS/ PANEL_RSDS

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_40_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_41_L, 0x0000);
                MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x000F);

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, 0x000, 0x3FF);    // TTL skew

                // GPO gating
                MOD_W2BYTEMSK(REG_MOD_BK00_4A_L, 0x0, BIT(8));     // GPO gating

                break;
        }
    }

//    MHal_PNL_Bringup(pInstance);
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
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    // setup output dot clock
    if (pstPanelInitData->eLPLL_Type == E_PNL_TYPE_TTL)
    {
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock, 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_CLK_DIV_2, CKG_ODCLK_MASK);               // [4..3] LPLL clock div, TTL:DIV2

        W2BYTEMSK(REG_RVD_44_L, 0x0004, 0x001F); //[4:0]reg_ckg_clk_misc
        W2BYTEMSK(L_CLKGEN0(0x58), 0x0001, 0x000F); //[3:0]ckg_tx_mod
    }
    else if ((pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS) && ((pstPanelInitData->eLPLL_Mode == E_PNL_MODE_SINGLE)))
    {
        // 1366x768, 1CH LVDS case
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock, 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_CLK_DIV_2, CKG_ODCLK_MASK);               // [4..3] LPLL clock div, TTL:DIV2

        W2BYTEMSK(REG_RVD_44_L, 0x0004, 0x001F); //[4:0]reg_ckg_clk_misc
        W2BYTEMSK(L_CLKGEN0(0x58), 0x0000, 0x000F); //[3:0]ckg_tx_mod
    }
    else
    {
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock, 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_XTAL, CKG_ODCLK_MASK);               // [4..3] LPLL clock div

        W2BYTEMSK(REG_RVD_44_L, 0x000C, 0x001F); //[4:0]reg_ckg_clk_misc
        W2BYTEMSK(L_CLKGEN0(0x58), 0x0000, 0x000F); //[3:0]ckg_tx_mod
    }

    // reg_ckg_odclk_mft
    W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_MFT_GATED);                          // [8] disable clock
    W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_MFT_INVERT);                         // [9] invert clock
    W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_MFT_SEL_LPLL, CKG_ODCLK_MFT_SEL_SOURCE);      // [10] select source tobe LPLL clock, 0:synthetic clock out, 1:LPLL clock out                                                            // 0:synthetic clock out, 1:LPLL clock out
    W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_MFT_XTAL, CKG_ODCLK_MFT_MASK);                // [12:11] LPLL clock div

    W2BYTE(L_CLKGEN1(0x31), 0x0000); //[11:8]ckg_odclk_frc

    W2BYTEMSK(REG_RVD_43_L, 0x0400,0x0F00); //for scaler tgen

    if(  IsVBY1(pstPanelInitData->eLPLL_Type) )
    {
        W2BYTEMSK(REG_CLKGEN0_57_L, 0x000C, 0x000F); //[3:0]ckg_fifo
        W2BYTE(REG_RVD_09_L, 0x0800); //[13:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo
        //W2BYTEMSK(L_CLKGEN0(0x55), 0x00, 0xF00); //[11:8] reg_ckg_osdc
        W2BYTEMSK(L_CLKGEN0(0x56),0x0010, 0x00F0); //[7:4]reg_ckg_odclk_a
        W2BYTE(L_CLKGEN0(0x63),0x1001); //[15:12]reg_ckg_nossc_odclk[11:8]ckg_tx_mod [5:0]ckg_osd2mod

        if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            W2BYTE(L_CLKGEN0(0x5A),0x0800); //[15:12]reg_ckg_vby1_omode [11:8]reg_ckg_vby1_vmode
        }
        else if ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
                ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
        {
            W2BYTE(L_CLKGEN0(0x5A),0x0900); //[15:12]reg_ckg_vby1_omode [11:8]reg_ckg_vby1_vmode
        }
        else
        {
            W2BYTE(L_CLKGEN0(0x5A),0x0A00); //[15:12]reg_ckg_vby1_omode [11:8]reg_ckg_vby1_vmode
        }
    }
    else
    {

        if (pstPanelInitData->eLPLL_Type == E_PNL_TYPE_TTL)
        {
            W2BYTEMSK(REG_CLKGEN0_57_L, 0x0001, 0x000F); //[3:0]ckg_fifo
            W2BYTEMSK(L_CLKGEN0(0x56),0x0040, 0x00F0); //[7:4]reg_ckg_odclk_a
            W2BYTE(L_CLKGEN0(0x63),0x1101); //[15:12]reg_ckg_nossc_odclk[11:8]ckg_tx_mod [5:0]ckg_osd2mod
        }
        else if(pstPanelInitData->eLPLL_Type == E_PNL_LPLL_ISP_8BIT_12P)
        {
            W2BYTE(L_CLKGEN0(0x63),0x1000); //[7:4]reg_ckg_odclk_a
            W2BYTEMSK(REG_CLKGEN0_57_L, 0x000C, 0x000F); //[3:0]ckg_fifo
            W2BYTEMSK(L_CLKGEN0(0x56),0x0000, 0x000F); //[7:4]reg_ckg_odclk_a
        }
        else
        {
            W2BYTEMSK(L_CLKGEN0(0x56),0x0010, 0x00F0); //[7:4]reg_ckg_odclk_a
            W2BYTEMSK(REG_CLKGEN0_57_L, 0x0000, 0x000F); //[3:0]ckg_fifo
            W2BYTEMSK(L_CLKGEN0(0x63),0x0001, 0x00FF); //[7:4]reg_ckg_odclk_a
        }

        if((pstPanelInitData->eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&(pstPanelInitData->eLPLL_Mode == E_PNL_MODE_SINGLE))
        {
            W2BYTE(REG_RVD_09_L, 0x1000); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo+
        }
        else if(pstPanelInitData->eLPLL_Type == E_PNL_LPLL_ISP_8BIT_12P)
        {
            W2BYTE(REG_RVD_09_L, 0x0800); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo
        }
        else
        {
            W2BYTE(REG_RVD_09_L, 0x0000); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo
        }
    }

    if( (E_PNL_TYPE_TTL == pstPanelInitData->eLPLL_Type)||
       ((E_PNL_TYPE_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode))||
       ((E_PNL_TYPE_HS_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode)))
    {
        //set LPLL mux
        W2BYTEMSK(L_CLKGEN0(0x7E), 0x0010, 0x00FC);           //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              // 011 / 111: LPLL output clock
    }
    else
    {
        //set LPLL mux
        W2BYTEMSK(L_CLKGEN0(0x7E), 0x0030, 0x00FC);           //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              // 011 / 111: LPLL output clock
    }

    if(pstPanelInitData->eLPLL_Type == E_PNL_LPLL_ISP_8BIT_12P)
    {
        W2BYTEMSK(REG_CHIP_50_L, 0x000F, 0x000F);
    }
}

#define OUTPUT_CHANNEL_PART_A   0x0055 // CH0~3
#define OUTPUT_CHANNEL_PART_B   0x5500 // CH4~7
#define OUTPUT_CHANNEL_PART_C   0x0015 // CH8~10
#define OUTPUT_CHANNEL_PART_D   0x0540 // CH11~13
#define OUTPUT_CHANNEL_ALL      0x5555

void MHal_PNL_Init_MOD(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    //------------------------------------------------------------------------

    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRL0 = %x\n", pstPanelInitData->u16MOD_CTRL0);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRL9 = %x\n", pstPanelInitData->u16MOD_CTRL9);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRLA = %x\n", pstPanelInitData->u16MOD_CTRLA);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u8MOD_CTRLB  = %x\n", pstPanelInitData->u8MOD_CTRLB);

    //-------------------------------------------------------------------------
    // Set MOD registers
    //-------------------------------------------------------------------------

    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, pstPanelInitData->u16MOD_CTRL0, LBMASK);

    //    GPIO is controlled in drvPadConf.c
    //    MDrv_Write2Byte(L_BK_MOD(0x46), 0x0000);    //EXT GPO disable
    //    MDrv_Write2Byte(L_BK_MOD(0x47), 0x0000);    //EXT GPO disable
    if(IsVBY1(pstPanelInitData->eLPLL_Type_Ext))
    {
        //0x1032, h27, [14] = 0x0 //disable Vsync/Hsync/VFDE signal align by Time Slot vld
        //0x1032, h27, [13:8] = 0xF //valid wrap counter for fix htt to times of lanes
        //Register setting is recommand by HW RD Roy.Yeh (For Mooney boot logo shake with SEC panel issue)
        MOD_W2BYTEMSK(REG_MOD_BK00_27_L, 0x0F00, 0x3F00);

        ///u16MOD_CTRL9 [6] : 62[9]reg_vby1_pair_swap
        ///u16MOD_CTRL9 [12:11] : 62[5:4]reg_vby1_pair_mirror
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0xA040, 0xFDCF); //[15]proc_st[13:12]byte_mode 4 byte mode[6]4ch_vby1[9]swap

        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, (pstPanelInitData->u16MOD_CTRL9 & BIT(6))<<3, BIT(9));
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, (pstPanelInitData->u16MOD_CTRL9 & (BIT(12)|BIT(11)))>>7, BIT(5)|BIT(4));

        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))  // use Dual port to decide the Vx1 1 or 2 devision config
        {
            if(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            {
                // 2 divison just be supported in monet vby1 8 lane
                printf("\n[%s][%d]Vx1 2 division\n", __FUNCTION__, __LINE__);
                MOD_W2BYTEMSK(REG_MOD_BK00_20_L, 0x0002, 0x0007);//[2:0]reg_mft_mode

                MOD_W2BYTEMSK(REG_MOD_BK00_21_L, 0x1002, 0xFFFF); //[11:0]reg_dly_value
                MOD_W2BYTEMSK(REG_MOD_BK00_22_L, 0x0F00, 0xFFFF); //[12:0]reg_hsize
                MOD_W2BYTEMSK(REG_MOD_BK00_26_L, 0x0780, 0xFFFF); //[12:0]reg_div_len
                MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, 0x0002, 0xFFFF); //[2:0]reg_sram_usage
                MOD_W2BYTEMSK(REG_MOD_BK00_53_L, 0x4000, 0xFFFF); //[14]reg_vfde_mask
            }
            else
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_20_L, 0x0000, 0x0007);//[2:0]reg_mft_mode
            }
        }
        else
        {
            printf("\n[%s][%d]Vx1 1 division\n", __FUNCTION__, __LINE__);
            MOD_W2BYTEMSK(REG_MOD_BK00_21_L, 0x0000, 0xFFFF); //[11:0]reg_dly_value
            MOD_W2BYTEMSK(REG_MOD_BK00_20_L, 0x0000, 0x0007); //[2:0]reg_mft_mode
            MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, 0x0000, 0xFFFF); //[2:0]reg_sram_usage
            MOD_W2BYTEMSK(REG_MOD_BK00_53_L, 0x0000, 0xFFFF); //[14]reg_vfde_mask
        }

        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(14), BIT(14)); //[14]:reg_lockn_to_acq
        MOD_W2BYTEMSK(REG_MOD_BK00_45_L, 0x0003, 0x0003); //[1]:reg_vby1_vs_inv, [0]:reg_vby1_hs_inv

        if((pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x00, BIT(6)); //[6]4ch_vby1
        else
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(6), BIT(6)); //[6]4ch_vby1


        ///u16MOD_CTRL9 [7] : 63[13]reg_vby1_pair_swap_osd
        ///u16MOD_CTRL9 [14:13] : 63[11:10]reg_vby1_pair_mirror2
        MOD_W2BYTEMSK(REG_MOD_BK00_63_L, (pstPanelInitData->u16MOD_CTRL9 & BIT(7))<<6, BIT(13));
        MOD_W2BYTEMSK(REG_MOD_BK00_63_L, (pstPanelInitData->u16MOD_CTRL9 & (BIT(14)|BIT(13)))>>3, BIT(11)|BIT(10));

        if((pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_63_L, BIT(12), BIT(12)); // [12] enable 8ch vx1 mode : 1

            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                // 2 Divisoin
                MOD_W2BYTEMSK(REG_MOD_BK00_63_L, BIT(11), BIT(11)); // [11:10]reg_vby1_pair_mirror2
            }
            else
            {
                // 1 Division
                MOD_W2BYTEMSK(REG_MOD_BK00_63_L, 0x00, BIT(11)); // [11:10]reg_vby1_pair_mirror2
            }
        }
        else //if   ///E_PNL_LPLL_VBY1_10BIT_4LANE, E_PNL_LPLL_VBY1_10BIT_2LANE
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_63_L,    0x00, BIT(12)); // [12] enable 8ch vx1 mode : 0
        }

        MHal_Output_Channel_Order(pInstance,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);


        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))  // use Dual port to decide the Vx1 1 or 2 devision config
        {
            printf("\n[%s][%d]Vx1 2 division\n", __FUNCTION__, __LINE__);
        }
        else
        {
            printf("\n[%s][%d]Vx1 1 division\n", __FUNCTION__, __LINE__);
        }

        ////per RD's suggestion ---Start
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_30_L, 0xFFFF, 0xFFFF);   //reg_gcr_pe_en_ch
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_32_L, 0x0000, 0xFFFF);   //reg_gcr_en_rint_ch
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3C_L, 0xFFFF, 0xFFFF);   //reg_gcr_test_ch

        ////per RD's suggestion, for VBY1 channel skew
        MOD_A_W2BYTE(REG_MOD_A_BK00_02_L, 0x4444);   //Fine tune skew for channel 0~7
        MOD_A_W2BYTE(REG_MOD_A_BK00_03_L, 0x4444);   //Fine tune skew for channel 8~15

        /// reg_gcr_pe_adj ch0~ch15
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1A_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1B_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1C_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1D_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1E_L,0x0202);
        MOD_A_W2BYTE(REG_MOD_A_BK00_1F_L,0x0202);

        /// reg_gcr_vby1_vreg
        MOD_A_W2BYTE(REG_MOD_A_BK00_20_L,0x4444);
        MOD_A_W2BYTE(REG_MOD_A_BK00_21_L,0x4444);
        MOD_A_W2BYTE(REG_MOD_A_BK00_22_L,0x4444);
        MOD_A_W2BYTE(REG_MOD_A_BK00_23_L,0x4444);

        MOD_W2BYTEMSK(REG_MOD_BK00_67_L, 0x80E8, 0xFFFF); //[0]:reg_vby1_8v4o_mode

        //The threshold value be set too strict ( ori: MOD_5C =0x0 )
        //And this reg should be set before enable serializer function
        //[15]reg_sw_rptr_fix_en: pointer fix by sw mode enable
        //[14:12]reg_sw_wptr_check: sw mode to decision write point check point
        //[10:8]reg_sw_rptr_fix_ini: sw mode to decision read point initial value
        //[6:4]reg_sw_rptr_fix_hi_th: sw mode to decision read pointer hi boundary
        //[2:0]reg_sw_rptr_fix_lo_th: sw mode to decision read pointer low boundary
        MOD_W2BYTEMSK(REG_MOD_BK00_5C_L,0x8142,0xFFFF);
        MOD_W2BYTEMSK(REG_MOD_BK00_5E_L,0x8142,0xFFFF);

        MOD_W2BYTE(REG_MOD_BK00_5B_L, 0x0087); //[0]enable serializer function ,
                                               //[1]enable serializer auto fix read/write point mis-balance
                                               //[2]enable osd serializer auto fix read/write point mis-balance
                                               //[7]for OSD, switch chanel 8~13 as OSD path
    }
    else
    {
        // non-VBY1 case
        MOD_W2BYTE(REG_MOD_BK00_49_L, pstPanelInitData->u16MOD_CTRL9); //{L_BK_MOD(0x49), 0x00}, // [7,6] : output formate selction 10: 8bit, 01: 6bit :other 10bit, bit shift
        MHal_Output_Channel_Order(pInstance,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
    }

    MOD_W2BYTE(REG_MOD_BK00_4A_L, pstPanelInitData->u16MOD_CTRLA);
    MOD_W2BYTE(REG_MOD_BK00_4B_L,  pstPanelInitData->u8MOD_CTRLB);  //[1:0]ti_bitmode 10:8bit  11:6bit  0x:10bit

    #if 0 // no need this for Maxim 23x23 8V case
    //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, (_u8PnlDiffSwingLevel << 1), 0xFE);       //differential output swing level
    if(((pstPanelInitData->eLPLL_Type_Ext>= E_PNL_LPLL_VBY1_10BIT_4LANE)&&
        (pstPanelInitData->eLPLL_Type_Ext<= E_PNL_LPLL_VBY1_8BIT_8LANE)))
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0xC000, 0xF000);       //bank selection for skew clock
    #endif

    //if(!MHal_PNL_MOD_Control_Out_Swing(_u8PnlDiffSwingLevel))
    //    printf(">>Swing Level setting error!!\n");
    if(pstPanelInitData->eLPLL_Type != E_PNL_TYPE_MINILVDS)
    {
        MOD_W2BYTEMSK(REG_MOD_A_BK00_70_L, 0x7, 0x07);
    }

    //// Patch for Vx1 and it should be control by panel ini
    if(  IsVBY1(pstPanelInitData->eLPLL_Type_Ext) )
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_31_L, 0x0FFF, 0x3FFF);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_31_L, pstPanelInitData->u16LVDSTxSwapValue);
    }


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

    //dual port lvds _start_//
    // output configure for 26 pair output 00: TTL, 01: LVDS/RSDS/mini-LVDS data differential pair, 10: mini-LVDS clock output, 11: RSDS clock output
    if (IsVBY1(pstPanelInitData->eLPLL_Type_Ext))
    {
        MS_U16 u16ChannelClk_En = BIT(0); // bit0 is en_clk
        MS_U16 u16OutputCFG0_7 = OUTPUT_CHANNEL_ALL, u16OutputCFG8_15 = OUTPUT_CHANNEL_ALL;
        if(pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type == LVDS_OUTPUT_User)
        {
            u16OutputCFG0_7 = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7;
            u16OutputCFG8_15 = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15;
        }

        if(u16OutputCFG0_7  & OUTPUT_CHANNEL_PART_A)
            u16ChannelClk_En |= BIT(1); //bit1: partA - ch0~3
        if(u16OutputCFG0_7  & OUTPUT_CHANNEL_PART_B)
            u16ChannelClk_En |= BIT(2); //bit2: partB - ch4~7
        if(u16OutputCFG8_15 & OUTPUT_CHANNEL_PART_C)
            u16ChannelClk_En |= BIT(3); //bit3: partC - ch8~10
        if(u16OutputCFG8_15 & OUTPUT_CHANNEL_PART_D)
            u16ChannelClk_En |= BIT(4); //bit4: partD - ch11~13

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, u16ChannelClk_En, 0x1F); //[4]ck_pd[3]ck_pc[2]ck_pb[1]ck_pa[0]en_ck   // original is MDrv_WriteByteMask(L_BK_MOD(0x77), 0x0F, BITMASK(7:2));
    }
    else
    {
        // LVDS
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x1F, 0x1F);    //[4]ck_pd[3]ck_pc[2]ck_pb[1]ck_pa[0]en_ck   // original is MDrv_WriteByteMask(L_BK_MOD(0x77), 0x0F, BITMASK(7:2));
    }
    //dual port lvds _end_//
    //MHal_PNL_Bringup(pInstance);

    MHal_PNL_MISC_Control(pInstance, pstPanelInitData->u32PNL_MISC);

    PNL_DBG(PNL_DBGLEVEL_INIT, "OutputType  = %x, eLPLL_Type = %x\n", pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions, pstPanelInitData->eLPLL_Type);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u32PNL_MISC  = %tx\n", (ptrdiff_t)pstPanelInitData->u32PNL_MISC);

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
    // Setup the default swing level
    pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = pstModCaliInitData->u16ExpectSwingLevel;   //mv
#if 0
    // Pair setting
    // =========
    // Select calibration source pair, 00: ch2, 01: ch6, 10:ch8, 11:ch12
    //MOD_7D_L[3:2]
    // =========
    //in msModCurrentCalibration, it will transfer to the real data

    switch(pstModCaliInitData->u8ModCaliPairSel)
    {
        default:
        case 0:
        //ch 2
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x00; // ch2
        break;
        case 1:
        //ch 6
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01; // ch6, calibration initialized value
        break;
        case 2:
        //ch 8
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x02;
        break;
        case 3:
        //ch 12
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x03;
        break;
    }
#endif
    // Target setting
    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
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
    // _u8MOD_CALI_VALUE is a real value; the _u8MOD_CALI_VALUE is an idea value
    // Target value should be the same with _u8MOD_CALI_VALUE to be a default value
    pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE= pstModCaliInitData->u8ModCaliTarget;
    // PVDD setting
    pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = pstModCaliInitData->bPVDD_2V5;

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16ExpectSwingLevel = %u\n", pstModCaliInitData->u16ExpectSwingLevel);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u8ModCaliTarget     = %x\n", pstModCaliInitData->u8ModCaliTarget);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_CALI_TARGET  = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_CALI_VALUE   = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE);
    PNL_DBG(PNL_DBGLEVEL_INIT, "bPVDD_2V5           = %x\n", pstModCaliInitData->bPVDD_2V5);

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
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_LVDS_Pair_Type = %u\n", pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type);

}

MS_BOOL msModCalDDAOUT(void)
{
   // W2BYTEMSK(BK_MOD(0x7D), ENABLE, 8:8);
   // MsOS_DelayTask(10);  //10ms
    return (MS_BOOL)((MOD_R2BYTEMSK(REG_MOD_A_BK00_74_L, BIT(8))) >> 8);
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
    MS_U16 u16reg_32da = 0, u16reg_32dc = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16reg_32da = MOD_A_R2BYTE(REG_MOD_A_BK00_00_L);
    u16reg_32dc = MOD_A_R2BYTE(REG_MOD_A_BK00_01_L);

    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "[%s][%d]\n", __FUNCTION__, __LINE__);

    // (1) Set keep mode to auto write calibration result into register.
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, BIT(15), BIT(15));

    // (2) Set calibration step waiting time
    MOD_A_W2BYTE(REG_MOD_A_BK00_71_L, 0x0080); // (about 5us)
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, 0x0000, 0x00FF);

    // (3) Set calibration toggle time
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, 0x0500, 0x0F00);

    // (4) Select calibration level (LVDS is 250mV)
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_70_L, pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET, BIT(2)|BIT(1)|BIT(0));    // Select calibration target voltage, 00: 250mV, 01:350mV, 10: 300mV, 11: 200mV

    // (5) Enable Calibration mode
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_70_L, BIT(7), BIT(7));         // Enable calibration function

    // (6) Store output configuration value and Enable each pair test mode
    MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, 0xFFFF);
    MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0FFF);

    MS_U8 u8CheckTimes = 0;
    while(1)
    {
        // (7) Enable Hardware calibration
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_73_L, BIT(15), BIT(15));

        // (8) Wait 2ms
        MsOS_DelayTask(2);

        // (10) Disable Hardware calibration
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_73_L, 0x00, BIT(15));

        // (9)Check Finish and Fail flag bit
        //BK111E, 0x73[14], Finish flag=1
        //BK111E, 0x73[13], Fail flag=0
        if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_73_L, 0x6000) == 0x4000)
        {
            //printf("\033[0;31m [%s][%d] cal ok, break  \033[0m\n", __FUNCTION__, __LINE__);
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
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_72_L, 0x00, BIT(15));
            //printf("\033[0;31m [%s][%d] If 3 times all fail, set all pair to nominal value by disable keep mode  \033[0m\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    if (u8CheckTimes <=3)
    {
         PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n----- Calibration ok \n");
    }
    else
    {
        PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n----- Calibration fail: 0x%x \n", MOD_R2BYTEMSK(REG_MOD_BK00_3D_L, 0x6000));
    }

    // Wait 2ms to make sure HW auto write calibration result into register
    MsOS_DelayTask(2);

    // (14) Restore each pair output configuration
    MOD_A_W2BYTE(REG_MOD_A_BK00_00_L, u16reg_32da);
    MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, u16reg_32dc);

    // (15) Disable calibration mode
    MOD_W2BYTEMSK(REG_MOD_A_BK00_70_L, 0x00, BIT(7));         // Disable calibration function

    // With HW calibration mode, HW would cal for each channel, and each channel would get different value
    // Return channel 2 vaule
    u8cur_ibcal = MOD_A_R2BYTEMSK(REG_MOD_A_BK00_09_L, 0x007F); // return ch2 calibration result

#if MOD_CAL_TIMER
    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "[%s] takes %ld ms\n", __FUNCTION__, (MsOS_GetSystemTime()-delay_start_time));
#endif
    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n Calibration result= %x\n", u8cur_ibcal);

    return (u8cur_ibcal&0x7F);//MOD_A_R2BYTEMSK(REG_MOD_A_BK00_0D_L, 0x003F);
#endif
}

PNL_Result MHal_PNL_MOD_Calibration(void *pInstance)
{
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

    W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

    u8Cab = msModCurrentCalibration(pInstance);

    W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

    if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type !=E_PNL_TYPE_MINILVDS)
        MOD_W2BYTEMSK(REG_MOD_A_BK00_70_L, u8Cab, 0x07);

    return E_PNL_OK;

}

static void MHal_PNL_PowerDownLPLL(void *pInstance, MS_BOOL bEnable)
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
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "[%s][%d]\n", __FUNCTION__, __LINE__);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u32PNL_MISC = %tx\n", (ptrdiff_t)pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "bPanelOn = %x\n", bPanelOn);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "eLPLL_Type            = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "_u8MOD_LVDS_Pair_Type = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

    MS_U16 u16PortA = MOD_A_R2BYTE(REG_MOD_A_BK00_00_L);
    MS_U16 u16PortB = MOD_A_R2BYTE(REG_MOD_A_BK00_01_L);

    if((u16PortA!=0)||(u16PortB!=0))
    {
        MHal_BD_LVDS_Output_Type(pInstance, LVDS_OUTPUT_User);
    }

    if(u16PortA!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7   = MOD_A_R2BYTE(REG_MOD_A_BK00_00_L);
    if(u16PortB!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15  = MOD_A_R2BYTE(REG_MOD_A_BK00_01_L);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "==========================\n\n");
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);


    if(bPanelOn)
    {
        // The order is PanelVCC -> delay pnlGetOnTiming1() -> VOP -> MOD
        // VOP
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0x4000, HBMASK);

        // For Napoli compatible
        // need to wait 1ms to wait LDO stable before MOD power on
        MsOS_DelayTask(1);

        // turn on LPLL
        MHal_PNL_PowerDownLPLL(pInstance, FALSE);

        if(pPNLResourcePrivate->stapiPNL.ePrevPowerState != E_POWER_RESUME)
        {
            // for LG panel black video
            MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE, DISABLE);
        }

        // mod power on
        MHal_MOD_PowerOn(pInstance
                        , ENABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

        if(bCalEn)
        {

            u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

            W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

            u8Cab = msModCurrentCalibration(pInstance);

            W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

        }
        else
        {
            if( ( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) ) &&
                ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & (MS_U32)E_APIPNL_MISC_SKIP_ICONVALUE) == FALSE))
            {
                HAL_MOD_CAL_DBG(printf("Use RCON value \n", __FUNCTION__, __LINE__));
                msSetVBY1RconValue(pInstance);
            }
            else
            {
                HAL_MOD_CAL_DBG(printf("User define Swing Value=%u\n", __FUNCTION__, __LINE__, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel));

                if(!MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel))
                    printf(">>Swing Level setting error!!\n");
            }
        }

        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En)
            MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE, ENABLE);
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
    W2BYTEMSK(L_BK_TCON(0x00), u16Bank&0xff, 0xFF);
}

MS_U16 MHal_PNL_Read_TCON_SubBank(void *pInstance)
{
    return (MS_U16)R2BYTEMSK(L_BK_TCON(0x00),0xFF);
}

MS_BOOL MHal_PNL_Is_VBY1_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0300) == 0x00)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MHal_PNL_Is_VBY1_LockN_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0100) == 0x00)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MHal_PNL_VBY1_Handshake(void *pInstance)
{
    MS_BOOL bIsLock = FALSE;

    if (MHal_PNL_Is_VBY1_Locked(pInstance) == FALSE)
    {
        MS_U16 u16CheckTimes = 0;
        //MS_U16 u16DeboundTimes = 0;

        // need to toggle vby1 packer  process start first
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x00, BIT(11));
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(11), BIT(11));

        MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0F56); // set reg. initial value
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xD6, 0x00FF); // after power on go to stand-by
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0x96, 0x00FF); // connection is established, go to Acquisition
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xB6, 0x00FF); // when internal clock is stable, got to CDR training
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xBE, 0x00FF); // enable encoder for DC blance

        while(u16CheckTimes < 10)
        {
#if 0
            u16DeboundTimes = 2;
            while ((!MHal_PNL_Is_VBY1_LockN_Locked()) && (u16DeboundTimes --))
            {
                MsOS_DelayTask(1); // can't remove
            }
#endif
            if(MHal_PNL_Is_VBY1_LockN_Locked(pInstance))
            {
                //-------------------------------------------------------------------
                // step1. Toggle clock when training
                MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0FAE);
                //--------------------------------------------------------------------
                bIsLock = TRUE;
                // pass 2 times debound to make sure VBY1 is locked
                break;
            }

            u16CheckTimes++;
            MsOS_DelayTaskUs(40);
        }

        if(bIsLock)
            {
            // step3. Disable HW check when lock done, Enable when loss lock
            //MOD_W2BYTEMSK(REG_MOD_BK00_33_L, 0x00, BIT15);

            /// Add the delay to increase time to send
            //MDrv_TIMER_Delayms(10);
        }
    }
    else
    {
        if(MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF) != 0x0FAE)
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0FAE);
        }
        bIsLock = TRUE;
    }

    return bIsLock;
}

MS_BOOL MHal_PNL_Is_VBY1_OC_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0C00) == 0x00)  // MOD_BK00_56_L[11:10]        for OSD
            {
        return TRUE;
    }
    else
    {
        return FALSE;
            }
}

MS_BOOL MHal_PNL_Is_VBY1_OC_LockN_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0400) == 0x00)  // MOD_BK00_56_L[11:10]        for OSD
            {
        return TRUE;
    }
    else
    {
        return FALSE;
            }
}

MS_BOOL MHal_PNL_VBY1_OC_Handshake(void *pInstance)
{
    MS_BOOL bIsLock = FALSE;

    if (MHal_PNL_Is_VBY1_OC_Locked(pInstance) == FALSE)
    {
        MS_U16 u16CheckTimes = 0;
//        MS_U16 u16DeboundTimes = 0;

        // need to toggle vby1 packer  process start first
        MOD_W2BYTEMSK(REG_MOD_BK00_66_L, 0x00, BIT(11));
        MOD_W2BYTEMSK(REG_MOD_BK00_66_L, BIT(11), BIT(11));


        MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0F56); // set reg. initial value
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xD6, 0x00FF); // after power on go to stand-by
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0x96, 0x00FF); // connection is established, go to Acquisition
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xB6, 0x00FF); // when internal clock is stable, got to CDR training
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xBE, 0x00FF); // enable encoder for DC blance

        while(u16CheckTimes < 10)
        {
        #if 0
            u16DeboundTimes = 2;
            while ((!MHal_PNL_Is_VBY1_OC_LockN_Locked()) && (u16DeboundTimes --))
            {
            MsOS_DelayTask(1);
            }
        #endif
            if(MHal_PNL_Is_VBY1_OC_LockN_Locked(pInstance))
            {
                //-------------------------------------------------------------------
                // step1. Toggle clock when training

                MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0FAE);
                bIsLock = TRUE;
                // pass 2 times debound to make sure VBY1 is locked
                break;
            }

            u16CheckTimes++;
            MsOS_DelayTaskUs(40);
        }

        if(bIsLock)
        {
            // step3. Disable HW check when lock done, Enable when loss lock
//            MOD_W2BYTEMSK(REG_MOD_BK00_33_L, 0x00, BIT15);
        }
    }
    else
    {
        if(MOD_R2BYTEMSK(REG_MOD_BK00_64_L, 0x0FFF) != 0x0FAE)
        {
            MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0FAE);
        }
        bIsLock = TRUE;
    }

    return bIsLock;
}

MS_BOOL MHal_PNL_IsYUVOutput(void *pInstance)
{
   return FALSE;
}

MS_BOOL MHal_PNL_SetOutputInterlaceTiming(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if (bEnable == TRUE)
    {
        //interlace output vtotal modify
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L, BIT(9), BIT(9));

        // two different interlace information through channel A reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_40_L, BIT(4) | BIT(7), BIT(4) | BIT(7));
        // two different interlace information through channel B reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, BIT(10)|BIT(11), BIT(10)|BIT(11));
    }
    else
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L  , 0, BIT(9));
        MOD_W2BYTEMSK(REG_MOD_BK00_40_L, 0, BIT(4) | BIT(7));
        MOD_W2BYTEMSK(REG_MOD_BK00_42_L, 0, BIT(10)|BIT(11));
    }

    return TRUE;
}

MS_BOOL MHal_PNL_GetOutputInterlaceTiming(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_BOOL bIsInterlaceOutput = FALSE;
    //interlace output vtotal modify
    if (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L, BIT(9)) == BIT(9))
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

////Ext LPLL setting
static void _MHal_PNL_Init_ExtLPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;

    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz, E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        printf("Not Supported LPLL Type, skip LPLL Init\n");
        return;
    }

    _MHal_PNL_DumpLPLLTable(pInstance, u8SupportedLPLLLIndex, E_PNL_LPLL_OSD);
}

static MS_U8 _MHal_PNL_Get_ExtLPLL_LoopDIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_div = 0;
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,u8LPLL_Mode,ldHz,E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        u16loop_div = 0 ;
    }
    else
    {
        u16loop_div = u16EXT_LoopDiv[u8SupportedLPLLLIndex];
    }

    u16loop_div *= 2;
    return u16loop_div;
}

static MS_U8 _MHal_PNL_Get_ExtLPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_gain = 0;
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz,E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        u16loop_gain = 0 ;
    }
    else
    {
        u16loop_gain = u16EXT_LoopGain[u8SupportedLPLLLIndex];
    }
    return u16loop_gain;
}


// Output Dclk
void MHal_PNL_CalExtLPLLSETbyDClk(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U64 ldHz)
{

    MS_U64 u64LdPllSet = 0;
    MS_U64 u64DclkFactor = 0;
    MS_U32 u32Div = 0;
    // loop div and loop gain use default parameters to avoid dclk floating out of range and getting wrong value
    MS_U32 u32Factor = 10;

    _MHal_PNL_Init_ExtLPLL(pInstance, u8LPLL_Type, u8LPLL_Mode, ldHz);

    //the first " *2 " is from  the dual mode
    u32Div=(MS_U32)(_MHal_PNL_Get_ExtLPLL_LoopDIV(pInstance, u8LPLL_Mode, u8LPLL_Type, ldHz));
    u64DclkFactor=((MS_U64)LVDS_MPLL_CLOCK_MHZ * (MS_U64)524288 * (MS_U64)_MHal_PNL_Get_ExtLPLL_LoopGain(pInstance, u8LPLL_Mode, u8LPLL_Type, ldHz));
    u64LdPllSet = (u64DclkFactor * 1000000 * u32Factor *2) + ((ldHz * u32Div) >> 1);
    do_div(u64LdPllSet, ldHz);
    do_div(u64LdPllSet, u32Div);

    W4BYTE(L_BK_LPLL(0x48), (MS_U32)u64LdPllSet);
    //printf("MHal_PNL_CalExtLPLLSETbyDClk u32KHz = %u, u32LpllSet = %x\n", ldHz, (MS_U32)u64LdPllSet);

}

void MHal_PNL_SetOSDCOutputType(void *pInstance, PNL_TYPE eLPLL_Type, E_PNL_OSDC_OUTPUT_FORMAT eOC_OutputFormat)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    // VBy1 co-registers
    if(  IsVBY1(eLPLL_Type) )
    {
        MOD_W2BYTE(REG_MOD_BK00_42_L, 0x1008); //[3]enable osd lvds channel
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_69_L, BIT(15), BIT(15)); //[15]sw_rst

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, 0x4000, 0xF000);       //bank selection for skew clock

        //-------------------------------------
        //## pe
//        MOD_A_W2BYTE(REG_MOD_A_BK00_30_L, 0x3fff);
//        MOD_W2BYTE(REG_MOD_BK00_23_L, 0x7000);
//        MOD_W2BYTE(REG_MOD_BK00_24_L, 0x7fff);
//        MOD_W2BYTE(REG_MOD_BK00_25_L, 0x003f);

        MOD_W2BYTE(REG_MOD_BK00_65_L, 0x8f3f); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt

        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x00, BIT(14)); //[14]:reg_lockn_to_acq
        MOD_W2BYTEMSK(REG_MOD_BK00_45_L, 0x0000, 0x0003); //[1]:reg_vby1_vs_inv, [0]:reg_vby1_hs_inv

        W2BYTE(REG_CLKGEN0_63_L,0x1410); //[11:8] clk_tx_mod_osd, [4:0] osd2mod
        W2BYTE(REG_RVD_09_L, 0x1800); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo

        MOD_W2BYTEMSK(REG_MOD_BK00_67_L, 0x0001, 0x0001); //[0]:reg_vby1_8v4o_mode
    }

    if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, BIT(14),BIT(14)|BIT(15)); //[15]enskew_path2[14]enclk_path2[4]ck_pd[3]ck_pc[2]ck_pb[1]ck_pa[0]en_ck
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0055); //[7:0]reg_output_conf[27:16]
        W2BYTE(L_CLKGEN0(0x5A),0x8800); //[15:12]reg_ckg_vby1_omode [11:8]reg_ckg_vby1_vmode

        MOD_W2BYTE(REG_MOD_BK00_42_L, 0x1000); //[12]sw_rst, [3]enable osd lvds channel
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x001F, 0x001F); //[4]ck_pd[3]ck_pc[2]ck_pb[1]ck_pa[0]en_ck
        MOD_A_W2BYTE(REG_MOD_A_BK00_58_L, 0x0440); //[3:0] reg_ckg_tx_mod
        MOD_A_W2BYTE(REG_MOD_A_BK00_59_L, 0x0044); //reg_ckg_dot

        MOD_W2BYTE(REG_MOD_BK00_71_L, 0xffff);
        MOD_W2BYTE(REG_MOD_BK00_5B_L, 0x0087); //[0]enable serializer function ,
                                               //[1]enable serializer auto fix read/write point mis-balance
                                               //[2]enable osd serializer auto fix read/write point mis-balance
                                               //[7]for OSD, switch chanel 8~13 as OSD path
        MOD_W2BYTE(REG_MOD_BK00_64_L, 0xd000);
        MOD_W2BYTE(REG_MOD_BK00_64_L, 0xd330);
        MOD_W2BYTE(REG_MOD_BK00_64_L, 0xd320);
        MOD_W2BYTE(REG_MOD_BK00_65_L, 0x8f3f);
        //-------------------------------------
        //## icon (Swing)
        MOD_A_W2BYTE(REG_MOD_A_BK00_0C_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_0D_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_0E_L, 0x0000);

        // vby1 osd 4 lane
        MOD_W2BYTE(REG_MOD_BK00_66_L, 0xa260); //[15]proc_st[13:12]byte_mode 4 byte mode[6]2ch_vby1_osd[9]swap

        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1)) // 2 Divisoin
        {
            MOD_W2BYTE(REG_MOD_BK00_08_L, 0x6420);
            MOD_W2BYTE(REG_MOD_BK00_09_L, 0x7531);
            MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xBA98);
            MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_08_L, 0x3210);
            MOD_W2BYTE(REG_MOD_BK00_09_L, 0x7654);
            MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xBA98);
            MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
        }
    }
    else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, BIT(14) ,BIT(14)|BIT(15));
        MOD_A_W2BYTE(REG_MOD_A_BK00_01_L, 0x0005);
        W2BYTE(L_CLKGEN0(0x5A),0x9900); //[15:12]reg_ckg_vby1_omode [11:8]reg_ckg_vby1_vmode
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_38_L, 0x001B, 0x001F); //[4]ck_pd[3]ck_pc[2]ck_pb[1]ck_pa[0]en_ck
        //-------------------------------------
        //## icon (Swing)
        MOD_A_W2BYTE(REG_MOD_A_BK00_0C_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_0D_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_0E_L, 0x0000);

        //vby1 osd 2 lane
        MOD_W2BYTE(REG_MOD_BK00_66_L, 0xa240); //[15]proc_st[13:12]byte_mode 4 byte mode[6]2ch_vby1_osd[9]swap[5]vby1_osd_4ch

        MOD_W2BYTE(REG_MOD_BK00_08_L, 0x3210);
        MOD_W2BYTE(REG_MOD_BK00_09_L, 0x7654);
        MOD_W2BYTE(REG_MOD_BK00_0A_L, 0xBA98);
        MOD_W2BYTE(REG_MOD_BK00_0B_L, 0x0000);
    }

    // Control VBY1 output format and bit orders
    switch(eOC_OutputFormat)
    {
        case E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB1:
            MOD_W2BYTEMSK(REG_MOD_BK00_6A_L, 0, BIT(1));
            break;

        case E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB2:
            MOD_W2BYTEMSK(REG_MOD_BK00_6A_L, BIT(1), BIT(1));
            break;

        default:
            printf("OSDC output format uses default value\n");
            MOD_W2BYTEMSK(REG_MOD_BK00_6A_L, 0, BIT(1));
            break;
    }


}

MS_BOOL MHal_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    MS_U16 u16Span;
    MS_U16 u16Step;
    MS_U32 u32PLL_SET;/// = MDrv_Read3Byte(L_BK_LPLL(0x0F));

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x48));
    // Set SPAN
    if(u16Fmodulation < 200 || u16Fmodulation > 400)
        u16Fmodulation = 300;
    u16Span =( ( (((MS_U32)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR ) / (u16Fmodulation) ) * 10000) / ((MS_U32)u32PLL_SET) ) ;

    // Set STEP
    if(u16Rdeviation > 300)
        u16Rdeviation = 300;
    u16Step = ((MS_U32)u32PLL_SET*u16Rdeviation) / ((MS_U32)u16Span*10000);

    W2BYTE(L_BK_LPLL(0x4E), u16Step & 0x0FFF);// LPLL_STEP
    W2BYTE(L_BK_LPLL(0x4F), u16Span & 0x3FFF);// LPLL_SPAN
    W2BYTEMSK((L_BK_LPLL(0x4E)), (bEnable << 15), BIT(15)); // Enable ssc


    return TRUE;
}

void MHal_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable)
{
    //printf("bEnable = %d\n", bEnable);
    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    W2BYTEMSK((L_BK_LPLL(0x4E)), (bEnable << 15), BIT(15)); // Enable ssc
}

void MHal_PNL_Set_T3D_Setting(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_55_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x1FFF);//pixel width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_66_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_51_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x1FFF);//reg_ln_width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_52_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_61_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x3FFF);//reg_ln_width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_62_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height

    //per designer, should always enable t3d, since it will affect osd/video's pipeline
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_61_L, BIT(0), BIT(0));//Enable Depth Render, for osd pipe line adjustment
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_08_L, BIT(4), BIT(4));//mtv bypass mode
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_57_L,(BIT(0)|BIT(1)),(BIT(0)|BIT(1)));//T3D fix subde enable, fix for T3D/PIP conflict issue (bit 0)     Bug Fix miu eco (bit 1)

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_63_L, 0x00, BIT(0)); // default disable T3D SRAM
}

void MHal_PNL_Set_Device_Bank_Offset(void *pInstance)
{
    UNUSED(pInstance);
    memset(u32PNL_XCDeviceBankOffset, 0, sizeof(MS_U32)*E_PNL_DEVICE_ID_NUM);
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_0] = E_HALPNL_DEVICE0_XC_BANK_OFFSET; // Set SC0 reg bank offset
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_1] = E_HALPNL_DEVICE1_XC_BANK_OFFSET; // Set SC2 reg bank offset
}

void MHal_PNL_Init(void *pInstance)
{
    // Do nothing
    //UNUSED(pInstance);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // STGEN reset
    //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK68_50_L, (BIT(0)|BIT(3)), (BIT(0)|BIT(3)) );
}

void MHal_PNL_Bringup(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    ///patch for bring up
    if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
    {
    }
    else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
    {
    }

}

MS_U16 MHal_PNL_GetPanelVStart(void)
{
    return 6;
}

MS_BOOL MHal_PNL_Check_VBY1_Handshake_Status(void *pInstance)
{
    if(MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF) != 0xFAE)
    {
        //printf("VBY1 handshake return because the reg value is 0x%u, not 0xFAE.\n", MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF));
        return FALSE;
    }
    else
    {
        //printf("VBY handshake check success.\n");
        return TRUE;
    }
}

void MHal_PNL_ChannelFIFOPointerADjust(void *pInstance)
{
    // 0 to 1 then will do write and read point capture to
    // Read  : REG_MOD_BK00_5F_L[14:12]
    // write : REG_MOD_BK00_5F_L[10:8]
    // it takes 3 ticks to capture and riu takes 5 ticks to write
    // so we don't have to do any delay between rising capture and
    // read/write pointer recognition
    MsOS_DelayTaskUs(500);
    MOD_A_W2BYTE(REG_MOD_A_BK00_5C_L, 0x3300);

    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L,      0, BIT(0));
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(0), BIT(0));

    //split Video & OSD process start bit
    //if (MDrv_ReadByte(REG_CHIP_REVISION) >= 1)
    if((R2BYTEMSK(REG_CHIP_REVISION, 0xFF00)>>8) >=1)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L,       0, BIT(14));
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L, BIT(14), BIT(14));
    }

    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, 0 , BIT(1));
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(1), BIT(1));

    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, 0, BIT(2)|BIT(3));
    MS_U16 u16name = MOD_A_R2BYTE(REG_MOD_A_BK00_5D_L);
    MS_S8 u8WritePointer = (u16name & 0x0700) >> 8; // REG_MOD_BK00_5F_L[10:8]
    MS_S8 u8ReadPointer = (u16name & 0x7000) >> 12; // REG_MOD_BK00_5F_L[14:12]

    //OSD part
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(2)|BIT(3), BIT(2)|BIT(3));
    MS_U16 OSDu16name = MOD_A_R2BYTE(REG_MOD_A_BK00_5D_L);
    MS_S8 OSDu8WritePointer = (OSDu16name & 0x0700) >> 8; // REG_MOD_BK00_5F_L[10:8]
    MS_S8 OSDu8ReadPointer = (OSDu16name & 0x7000) >> 12; // REG_MOD_BK00_5F_L[14:12]

    MS_BOOL bOSDC = ((MOD_A_R2BYTE(REG_MOD_A_BK00_58_L)&0x00F0) == 0x0040)?TRUE:FALSE;
    while (((abs(u8WritePointer-u8ReadPointer) >4) && (abs(u8WritePointer-u8ReadPointer)<2))
        ||(((abs(OSDu8WritePointer-OSDu8ReadPointer) >4) && (abs(OSDu8WritePointer-OSDu8ReadPointer)<2))&&bOSDC))
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L,      0, BIT(0));
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(0), BIT(0));

        //split Video & OSD process start bit
        //if (MDrv_ReadByte(REG_CHIP_REVISION) >= 1)
        if((R2BYTEMSK(REG_CHIP_REVISION, 0xFF00)>>8) >=1)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L,       0, BIT(14));
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L, BIT(14), BIT(14));
        }

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, 0 , BIT(1));
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(1), BIT(1));

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, 0, BIT(2)|BIT(3));
                u16name = MOD_A_R2BYTE(REG_MOD_A_BK00_5D_L);
        u8WritePointer = (u16name & 0x0700) >> 8; // REG_MOD_BK00_5F_L[10:8]
        u8ReadPointer = (u16name & 0x7000) >> 12; // REG_MOD_BK00_5F_L[14:12]

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_5D_L, BIT(2)|BIT(3), BIT(2)|BIT(3));
        OSDu16name = MOD_A_R2BYTE(REG_MOD_A_BK00_5D_L);
        OSDu8WritePointer = (OSDu16name & 0x0700) >> 8; // REG_MOD_BK00_5F_L[10:8]
        OSDu8ReadPointer = (OSDu16name & 0x7000) >> 12; // REG_MOD_BK00_5F_L[14:12]
        printf("bOSDC [%d]\n",bOSDC);

    }

}

void MHal_PNL_VBY1_Hardware_TrainingMode_En(void *pInstance, MS_BOOL bIsVideoMode ,MS_BOOL bEnable)
{
    if(bIsVideoMode)
    {
        if(bEnable)
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0AAE);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0AA6);
        }
    }
    else
    {
        if(bEnable)
        {
            MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0AAE);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0AA6);
        }
    }
}

MS_BOOL MHal_PNL_VBY1_IsSupport_Hardware_TrainingMode(void *pInstance)
{
    #ifdef SUPPORT_VBY1_HWTRAINING_MODE
        return TRUE;
    #else
        return FALSE;
    #endif
}

void MHal_PNL_TCON_Patch(void* pInstance)
{
//tcon apply new tcon.bin in maxim
//so change patch
//step 1: enable param.
//step 2: delay one vysnc
//step 3: disable param.
    MS_U16 u16CurrentBank = 0;
    u16CurrentBank = MHal_PNL_Read_TCON_SubBank(pInstance);

    MHal_PNL_Switch_TCON_SubBank(pInstance, 0x04);
    W2BYTEMSK(L_BK_TCON(0x50), BIT(1), BIT(1));
    MsOS_DelayTask(20);
    W2BYTEMSK(L_BK_TCON(0x50), 0, BIT(1));
    MHal_PNL_Switch_TCON_SubBank(pInstance, u16CurrentBank);
}

static MS_BOOL _Hal_MOD_External_eFuse(void)
{
#ifdef MOD_EFUSE_IN_MBOOT
    return TRUE;
#else
    return FALSE;
#endif
}

#endif

