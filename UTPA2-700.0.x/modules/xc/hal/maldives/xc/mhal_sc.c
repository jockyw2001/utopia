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
//==============================================================================
// [mhal_sc.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_SC_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif

// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#ifdef STELLAR
#include "apiXC_Cus.h"
#endif
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mdrv_sc_3d.h"
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#include "drv_sc_scaling.h"
#include "XC_private.h"

#include "mhal_menuload.h"
#include "halCHIP.h"
#include "drv_sc_isr.h"

#include "mhal_dynamicscaling.h"
#include "mdrv_sc_dynamicscaling.h"


#define SC_DBG(x)   //x
#define FPLL_DBG(x) //x

#define new_chakra

// empirical value
#define REPWINVERTICALOFFSET 0x31
#define REPWINHORIZONTALOFFSET 0x1F

//==============================================================================
//==============================================================================
MS_U32 _XC_RIU_BASE;
MS_U32 _PM_RIU_BASE;

MS_U32 _DVI_ATOP_RIU_BASE;
MS_U32 _DVI_DTOP_RIU_BASE;
MS_U32 _HDCP_RIU_BASE;
MS_U32 _DVI_EQ_RIU_BASE;
MS_U32 _ADC_DTOPB_RIU_BASE;

MS_U32 _DDC_RIU_BASE;
MS_U32 _PM_SLEEP_RIU_BASE;

extern PQ_Function_Info    s_PQ_Function_Info;
//extern MS_BOOL             s_bKeepPixelPointerAppear;
static SC_MIUMASK_t Miu0Mask, Miu1Mask;
static SC_MIUMASK_t Miu0MaskOld, Miu1MaskOld;
static MS_BOOL bDynamicScalingEnable = 0;

static MS_U16 u16OP1_fclk_gate_en = 0;
static MS_U16 u16OP1_reg_ip2_clk_gate_en = 0;
static MS_U16 u16OP1_ficlk_status = 0;
static MS_U32 u32OP1_reg_pre_down_f2 = 0;
static MS_U32 u32OP1_reg_pre_down_f1 = 0;
static MS_U16 u16OP1_color_3x3 = 0;
static MS_U16 u16OP1_color_vip = 0;
static MS_U16 u16OP1_reg_di1lb_en = 0;
static MS_U16 u16OP1_bypass_miu = 0;
static MS_U16 u16OP1_vop_dualpixel = 0;

static MS_U16 u16RegForceMryValue = 0;
MS_U32 _HAL_XC_DEVICE_OFFSET[MAX_XC_DEVICE_NUM] = {MAX_XC_DEVICE0_OFFSET};
static MS_BOOL _HAL_bopm_lcnt_inv[MAX_WINDOW] = {false , false};
static void MHal_XC_SetForceCurrentReadBankInterrupt(SC_INT_SRC eIntNum, void *pParam);
// Put this function here because hwreg_utility2 only for hal.
void Hal_SC_init_riu_base(MS_U32 u32riu_base, MS_U32 u32PMriu_base)
{
    _XC_RIU_BASE = u32PMriu_base;
    _PM_RIU_BASE = u32PMriu_base;


    _DVI_ATOP_RIU_BASE = u32PMriu_base;
    _DVI_DTOP_RIU_BASE = u32PMriu_base;
    _HDCP_RIU_BASE = u32PMriu_base;
    _DVI_EQ_RIU_BASE = u32PMriu_base;
    _ADC_DTOPB_RIU_BASE = u32PMriu_base;

    _DDC_RIU_BASE =u32PMriu_base;
    _PM_SLEEP_RIU_BASE =u32PMriu_base;

}

//=========================================================//
// Function : Hal_SC_setfield
// Description:
//=========================================================//
void Hal_SC_setfield(void *pInstance, MS_U16 reg_1D, MS_U16 reg_21, MS_U16 reg_23, SCALER_WIN eWindow )
{
    //printf("reg_1D=%x, reg_21=%x, reg_23=%x\n", reg_1D, reg_21, reg_23);

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_1D_L, (reg_1D<<8), 0xEF00);
        SC_W2BYTEMSK(0,REG_SC_BK01_21_L, reg_21, 0x3FFF);
        SC_W2BYTEMSK(0,REG_SC_BK01_23_L, (reg_23<<8), HBMASK);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK03_1D_L, (reg_1D<<8), 0xEF00);
        SC_W2BYTEMSK(0,REG_SC_BK03_21_L, reg_21, 0x3FFF);
        SC_W2BYTEMSK(0,REG_SC_BK03_23_L, (reg_23<<8), HBMASK);
    }
//    printf("%x ",SC_R2BYTEMSK(0,REG_SC_BK01_1D_L, HBMASK));
//    printf("%x ",SC_R2BYTEMSK(0,REG_SC_BK01_21_L, 0x3FFF));
//    printf("%x ",SC_R2BYTEMSK(0,REG_SC_BK01_23_L, HBMASK));

//    printf("%x ",SC_R4BYTE(0,REG_SC_BK01_21_L));
//    printf("%x ",SC_R4BYTE(0,REG_SC_BK01_23_L));
}

//=========================================================//
// Function : Hal_SC_de_only_en
// Description: DE Only. HSYNC and VSYNC are ignored.
//#0: Disable.
//#1: Enable.
//=========================================================//
void Hal_SC_de_only_en(void *pInstance,MS_BOOL benable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(0,REG_SC_BK01_03_L, (benable ? BIT(6):0), BIT(6));
    else
        SC_W2BYTEMSK(0,REG_SC_BK03_03_L, (benable ? BIT(6):0), BIT(6));
}

//=========================================================//
// Function : Hal_SC_de_direct_en
// Description: Digital Input Horizontal Sample Range
//#0: Use DE as sample range, only V position can be adjusted.
//#1: Use SPRHST and SPRHDC as sample range, both H and V position can be adjusted.
//=========================================================//
void Hal_SC_de_direct_en(void *pInstance,MS_BOOL benable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(0,REG_SC_BK01_03_L, (benable ? BIT(7):0), BIT(7));
    else
        SC_W2BYTEMSK(0,REG_SC_BK03_03_L, (benable ? BIT(7):0), BIT(7));

}

void Hal_SC_set_ficlk(void *pInstance,MS_BOOL bPreDown, SCALER_WIN eWindow)
{
    //refine CL :1177928 move code from drv to hal
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_BOOL bMMwithDS = IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)
                    &&(MHAL_SC_Get_DynamicScaling_Status(pInstance) || (pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[eWindow]));
    //if it's playing mm & enabling DS, using fclk causes purple stripes showing upon video.
    if ( !gSrcInfo[eWindow].bPreV_ScalingDown && !bMMwithDS)
    {
        bPreDown = FALSE;
    }
    else
    {
        bPreDown = TRUE;
    }

    if( eWindow == MAIN_WINDOW )
    {
        if (bPreDown)
        {
            W2BYTEMSK(REG_CLKGEN0_51_L, CLK_SRC_IDCLK2<<10, (BIT(11)|BIT(10))); // clk_idclk2
        }
        else
        {
            W2BYTEMSK(REG_CLKGEN0_51_L, CLK_SRC_FCLK<<10, (BIT(11)|BIT(10))); // clk_fclk
        }
    }
    else
    {
        if (bPreDown)
        {
            W2BYTEMSK(REG_CLKGEN0_51_L, CLK_SRC_IDCLK2<<2, (BIT(3)|BIT(2))); // clk_idclk2
        }
        else
        {
            W2BYTEMSK(REG_CLKGEN0_51_L, CLK_SRC_FCLK<<2, (BIT(3)|BIT(2))); // clk_fclk
        }
    }
}


void Hal_SC_set_shiftline(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow )
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_2F_L, (u8Val & 0x07)<<8, (BIT(10)|BIT(9)|BIT(8)));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_2F_L, (u8Val & 0x03)<<13, (BIT(14)|BIT(13)));
    }
}

void Hal_SC_set_422_cbcr_swap(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK02_0A_L, bEnable ? BIT(6) : 0, BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK04_0A_L, bEnable ? BIT(6) : 0, BIT(6));
    }
}

void Hal_SC_set_pre_align_pixel(void *pInstance,MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow)
{
    if (bEnable)
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Enable pre align pixel for mirror mode.
            SC_W2BYTEMSK(0,REG_SC_BK02_2A_L, BIT(15), BIT(15));

            // Number of pixels need to be inserted. (U3 has 15 pixels)
            SC_W2BYTEMSK(0,REG_SC_BK02_2A_L, pixels << 8 , 0x1F00);
        }
        else
        {
            // Enable pre align pixel for mirror mode.
            SC_W2BYTEMSK(0,REG_SC_BK04_2A_L, BIT(15), BIT(15));

            // Number of pixels need to be inserted. (U3 has 15 pixels)
            SC_W2BYTEMSK(0,REG_SC_BK04_2A_L, pixels << 8 , 0x1F00);
        }

    }
    else
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Disable pre align pixel for mirror mode.
            SC_W2BYTEMSK(0,REG_SC_BK02_2A_L, 0x00 , BIT(15));
        }
        else
        {
            // Disable pre align pixel for mirror mode.
            SC_W2BYTEMSK(0,REG_SC_BK04_2A_L, 0x00 , BIT(15));
        }
    }
}

void Hal_XC_Set_FreeFRCMD(void *pInstance,MS_BOOL bEnable)
{
    //T8 has different freemd reg with T3
    SC_W2BYTEMSK(0,REG_SC_BK12_05_L, bEnable? BIT(5) : 0, BIT(5));
}

// Obsolete in M10/J2/A5
void Hal_SC_set_linearmem_mode(MS_BOOL bEnable, SCALER_WIN eWindow)
{
}

// This function will return 8/10/12/14/16 field mode or 8 frame mode.
// Otherwise it return IMAGE_STORE_2_FRAMES
XC_FRAME_STORE_NUMBER Hal_SC_GetFrameStoreMode(void *pInstance,SCALER_WIN eWindow)
{
    MS_U16 u16FrameCount = 0x00;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (eWindow == MAIN_WINDOW)
    {
        u16FrameCount = 0x1F & SC_R2BYTE(0,REG_SC_BK12_19_L);
    }
    else
    {
        u16FrameCount = 0x1F & SC_R2BYTE(0,REG_SC_BK12_59_L);
    }

    switch( u16FrameCount) // Get field type.
    {
        default:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_2_FRAMES;
            }
        case 3:
            return IMAGE_STORE_3_FRAMES;
        case 4:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_4_FRAMES;
            }
        case 6:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_6_FIELDS;
            }
            else
            {
                return IMAGE_STORE_6_FRAMES;
            }
        case 8:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_8_FIELDS;
            }
            else
            {
                return IMAGE_STORE_8_FRAMES;
            }
        case 10:
            return IMAGE_STORE_10_FIELDS;
        case 12:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_12_FIELDS;
            }
            else
            {
                return IMAGE_STORE_12_FRAMES;
            }
        case 14:
            return IMAGE_STORE_14_FIELDS;
        case 16:
            return IMAGE_STORE_16_FIELDS;

    }

    return IMAGE_STORE_UNDEFINE;
}


void Hal_SC_set_wr_bank_mapping(void *pInstance,MS_U8 u8val, SCALER_WIN eWindow)
{
    SC_DBG(printf("Set WR bank mapping SC_BK12_07/47_L[15:13]= 0x%x\n", ((MS_U16)u8val)<<13));

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_07_L, ((MS_U16)u8val)<<13, BIT(15)|BIT(14)|BIT(13));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_47_L, ((MS_U16)u8val)<<13, BIT(15)|BIT(14)|BIT(13));
    }
}

void Hal_SC_set_wr_bank_mapping_num(void *pInstance,MS_U8 u8Val, SCALER_WIN eWindow)
{
    Hal_SC_set_wr_bank_mapping(pInstance,u8Val, eWindow);
}

MS_U8 Hal_SC_Get_WR_Bank_Mapping(void *pInstance,SCALER_WIN eWindow)
{
    MS_U8 u8val = 0;

    if( eWindow == MAIN_WINDOW )
    {
        u8val = SC_R2BYTEMSK(0,REG_SC_BK12_07_L, BIT(15)|BIT(14)|BIT(13)) >> 13;
    }
    else
    {
        u8val = SC_R2BYTEMSK(0,REG_SC_BK12_47_L, BIT(15)|BIT(14)|BIT(13)) >> 13;
    }
    SC_DBG(printf("Get WR bank mapping SC_BK12_07/47_L[15:13]= 0x%x\n", ((MS_U16)u8val)<<13));
    return u8val;
}

MS_U8 Hal_SC_Get_WR_Bank_Mapping_Num(void *pInstance,SCALER_WIN eWindow)
{
    return Hal_SC_Get_WR_Bank_Mapping(pInstance,eWindow);
}

void Hal_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK02_40_L, (bEnable ? BIT(3):0), BIT(3));
        SC_W2BYTEMSK(0,REG_SC_BK02_01_L, (bEnable ? BIT(2):0), BIT(2));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK04_40_L, (bEnable ? BIT(3):0), BIT(3));
        SC_W2BYTEMSK(0,REG_SC_BK04_01_L, (bEnable ? BIT(2):0), BIT(2));
    }
}

MS_BOOL Hal_SC_get_csc(void *pInstance,SCALER_WIN eWindow)
{
    MS_BOOL bYUVInput;

    if(eWindow == MAIN_WINDOW)
    {
        // If it has CSC, it shows it must be RGB input.
        // Because in scaler, it must be YUV. So if input is RGB, then we need to do CSC
        bYUVInput = ( SC_R2BYTEMSK(0,REG_SC_BK02_40_L, BIT(3) ) ||
                                SC_R2BYTEMSK(0,REG_SC_BK18_6E_L, BIT(0) ) )? FALSE:TRUE;
    }
    else
    {
        bYUVInput = ( SC_R2BYTEMSK(0,REG_SC_BK04_40_L, BIT(3) ) ||
                                SC_R2BYTEMSK(0,REG_SC_BK18_6E_L, BIT(4) ) )? FALSE:TRUE;
    }
    return bYUVInput;
}

void Hal_SC_set_delayline(void *pInstance, MS_U8 u8DelayLines, SCALER_WIN eWindow )
{
    //skip bit7, which is other usage
    MS_U16 u16DelayLines = ((MS_U16)u8DelayLines) & 0xFF;
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_1C_L, (u16DelayLines<<4), (BIT(9)|BIT(8)|BIT(7)|BIT(6)|BIT(5)|BIT(4)));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK03_1C_L, (u16DelayLines<<4), (BIT(9)|BIT(8)|BIT(7)|BIT(6)|BIT(5)|BIT(4)));
    }
}

void Hal_SC_set_write_limit(void *pInstance,MS_U32 u32WritelimitAddrBase, SCALER_WIN eWindow)
{
    MS_U32 u32MinLimitAddress = MHal_XC_GetDNRBufAddress(pInstance, eWindow)/BYTE_PER_WORD ;
    MS_U32 u32MaxLimitAddress = (MHal_XC_GetDNRBufAddress(pInstance, eWindow) + MHal_XC_GetDNRBufSize(pInstance, eWindow))/BYTE_PER_WORD - 1;
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK0D_38_L, ((u32WritelimitAddrBase&BIT(31))?BIT(0):0),BIT(0)); //F2 memory address limit enable
        SC_W4BYTE(0,REG_SC_BK0D_40_L, u32MinLimitAddress); //F2 memory min address
        SC_W4BYTE(0,REG_SC_BK0D_42_L, u32MaxLimitAddress); //F2 memory max address
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0D_38_L, ((u32WritelimitAddrBase&BIT(31))?BIT(1):0),BIT(1)); //F1 memory address limit enable
        SC_W4BYTE(0,REG_SC_BK0D_44_L, u32MinLimitAddress); //F1 memory min address
        SC_W4BYTE(0,REG_SC_BK0D_46_L, u32MaxLimitAddress); //F1 memory max address
    }
}

void Hal_SC_set_opm_write_limit(void *pInstance,MS_BOOL bEnable, MS_BOOL bFlag, MS_U32 u32OPWlimitAddr, SCALER_WIN eWindow)
{
    MS_U16 u16temp = 0x00;

    // Only support address of Main_Window
    if (eWindow != MAIN_WINDOW )
        return;

    // Bit 0 ~ 24
    SC_W2BYTE(0,REG_SC_BK12_5E_L, u32OPWlimitAddr & 0xFFFF);
    SC_W2BYTE(0,REG_SC_BK12_5F_L, (u32OPWlimitAddr >> 16) & 0x1FF);

    // OPW limit enable
    if (bEnable)
        u16temp = BIT(9);

    // OPW limit flag 0: maximum 1:minimum
    if (bFlag)
        u16temp |= BIT(10);

    SC_W2BYTEMSK(0,REG_SC_BK12_5F_L,u16temp ,BIT(9) | BIT(10) );
}

MS_BOOL Hal_SC_is_extra_req_en(void *pInstance,
    MS_U16 *pu16MainHStart, MS_U16 *pu16MainHEnd,
    MS_U16 *pu16SubHStart,  MS_U16 *pu16SubHEnd)
{
    MS_U16 u16MainHStart, u16SubHStart;
    MS_U16 u16MainHEnd, u16SubHEnd;
    MS_BOOL bEn = FALSE;

    u16MainHStart = pu16MainHStart == NULL ? SC_R2BYTE(0,REG_SC_BK10_08_L) : *pu16MainHStart;
    u16MainHEnd   = pu16MainHEnd   == NULL ? SC_R2BYTE(0,REG_SC_BK10_09_L) : *pu16MainHEnd;
    u16SubHStart  = pu16SubHStart  == NULL ? SC_R2BYTE(0,REG_SC_BK0F_07_L) : *pu16SubHStart;
    u16SubHEnd    = pu16SubHEnd    == NULL ? SC_R2BYTE(0,REG_SC_BK0F_08_L) : *pu16SubHEnd;

    if( SC_R2BYTEMSK(0, REG_SC_BK20_10_L, BIT(1)))
    {
        if(u16MainHStart > u16SubHStart || u16MainHEnd < u16SubHEnd)
        {
            bEn = FALSE;
        }
        else
        {
            bEn = TRUE;
        }
    }
    else
    {
        bEn = FALSE;
    }

    SC_DBG(printf("Sub=%d, MainHs=%d, SubHs=%d, MainHe=%d, SubHe=%d :: Extra=%d\n",
     SC_R2BYTEMSK(0, REG_SC_BK20_10_L, BIT(1)),
     u16MainHStart, u16SubHStart, u16MainHEnd, u16SubHEnd, bEn));

    return bEn;

}

static void _Hal_SC_fill_main_sw_db_burst(void *pInstance,P_SC_SWDB_INFO pDBreg)
{

    if(Hal_SC_is_extra_req_en(pInstance,&pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd, NULL, NULL))
    {
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, BIT(15), BIT(15));
    }
    else
    {
         MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, 0, BIT(15));
    }

    //H pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK02_04_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK02_05_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>16), 0xFFFF);
    //V pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK02_08_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK02_09_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>16), 0xFFFF);
    //H post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_07_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_08_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>16), 0xFFFF);
    //V post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_09_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_0A_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>16), 0xFFFF);
    //SCMI: vertical limitation
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_18_L, (MS_U16)(pDBreg->u16VWritelimit), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_1A_L, (MS_U16)(pDBreg->u32WritelimitBase>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_1B_L, (MS_U16)(pDBreg->u32WritelimitBase>>16), 0xFFFF);
    //SCMI: DNR address 0
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_08_L, (MS_U16)(pDBreg->u32DNRBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_09_L, (MS_U16)(pDBreg->u32DNRBase0>>16), 0xFFFF);
    //SCMI: DNR address 1
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0A_L, (MS_U16)(pDBreg->u32DNRBase1>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0B_L, (MS_U16)(pDBreg->u32DNRBase1>>16), 0xFFFF);
    //SCMI: DNR address 2
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0C_L, (MS_U16)(pDBreg->u32DNRBase2>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0D_L, (MS_U16)(pDBreg->u32DNRBase2>>16), 0xFFFF);
    //SCMI: DNR offset
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0E_L, (MS_U16)(pDBreg->u16DNROffset>>00), 0xFFFF);
    //SCMI: DNR fetch
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_0F_L, (MS_U16)(pDBreg->u16DNRFetch>>00), 0xFFFF);
    //SCMI: OPM address 0
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK08_32_L, (MS_U16)(_HAL_bopm_lcnt_inv[MAIN_WINDOW]? BIT(5):0x00), BIT(5));
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_10_L, (MS_U16)(pDBreg->u32OPMBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_11_L, (MS_U16)(pDBreg->u32OPMBase0>>16), 0xFFFF);
    //SCMI: OPM address 1
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_12_L, (MS_U16)(pDBreg->u32OPMBase1>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_13_L, (MS_U16)(pDBreg->u32OPMBase1>>16), 0xFFFF);
    //SCMI: OPM address 2
//    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_14_L, (MS_U16)(pDBreg->u32OPMBase2>>00), 0xFFFF);
//    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_15_L, (MS_U16)(pDBreg->u32OPMBase2>>16), 0xFFFF);
    //SCMI: OPM offset
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_16_L, (MS_U16)(pDBreg->u16OPMOffset>>00), 0xFFFF);
    //SCMI: OPM fetch
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_17_L, (MS_U16)(pDBreg->u16OPMFetch>>00), 0xFFFF);

    //PIP
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_15_L, (MS_U16)(pDBreg->u16VLen>>00), 0xFFFF); // Write V length
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_1D_L, (MS_U16)(pDBreg->u16LBOffset&0xFF), 0x00FF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_1C_L, (MS_U16)(pDBreg->u16DispOffset&0xFF), 0x00FF);

    //capture window
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_04_L, (MS_U16)(pDBreg->u16V_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_05_L, (MS_U16)(pDBreg->u16H_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_06_L, (MS_U16)(pDBreg->u16V_CapSize>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_07_L, (MS_U16)(pDBreg->u16H_CapSize>>00), 0xFFFF);

    //Display window
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_08_L, (MS_U16)(pDBreg->u16H_DisStart>>00), VOP_DISPLAY_HSTART_MASK);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_09_L, (MS_U16)(pDBreg->u16H_DisEnd>>00), VOP_DISPLAY_HEND_MASK);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_0A_L, (MS_U16)(pDBreg->u16V_DisStart>>00), VOP_DISPLAY_VSTART_MASK);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_0B_L, (MS_U16)(pDBreg->u16V_DisEnd>>00), VOP_DISPLAY_VEND_MASK);

    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_1C_L, 0x0, 0x1FFF); //always disable IP H mirror line offset note:ds will enable this register at vdec firmware

}

static void _Hal_SC_fill_sub_sw_db_burst(void *pInstance,P_SC_SWDB_INFO pDBreg)
{

    if(Hal_SC_is_extra_req_en(pInstance,NULL, NULL, &pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd))
    {
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, BIT(15), BIT(15));
    }
    else
    {
         MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, 0, BIT(15));
    }

    //H pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK04_04_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK04_05_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>16), 0xFFFF);
    //V pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK04_08_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK04_09_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>16), 0xFFFF);

    //H post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_27_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_28_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>16), 0xFFFF);
    //V post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_29_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK23_2A_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>16), 0xFFFF);

    //SCMI: vertical limitation
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_58_L, (MS_U16)(pDBreg->u16VWritelimit), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_5A_L, (MS_U16)(pDBreg->u32WritelimitBase>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_5B_L, (MS_U16)(pDBreg->u32WritelimitBase>>16), 0xFFFF);
    //SCMI: DNR address 0
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_48_L, (MS_U16)(pDBreg->u32DNRBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_49_L, (MS_U16)(pDBreg->u32DNRBase0>>16), 0xFFFF);
    //SCMI: DNR address 1
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4A_L, (MS_U16)(pDBreg->u32DNRBase1>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4B_L, (MS_U16)(pDBreg->u32DNRBase1>>16), 0xFFFF);
    //SCMI: DNR address 2
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4C_L, (MS_U16)(pDBreg->u32DNRBase2>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4D_L, (MS_U16)(pDBreg->u32DNRBase2>>16), 0xFFFF);
    //SCMI: DNR offset
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4E_L, (MS_U16)(pDBreg->u16DNROffset>>00), 0xFFFF);
    //SCMI: DNR fetch
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_4F_L, (MS_U16)(pDBreg->u16DNRFetch>>00), 0xFFFF);
    //SCMI: OPM address 0
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK08_32_L, (MS_U16)(_HAL_bopm_lcnt_inv[SUB_WINDOW]? BIT(4):0x00), BIT(4));
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_50_L, (MS_U16)(pDBreg->u32OPMBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_51_L, (MS_U16)(pDBreg->u32OPMBase0>>16), 0xFFFF);
    //SCMI: OPM address 1
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_52_L, (MS_U16)(pDBreg->u32OPMBase1>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_53_L, (MS_U16)(pDBreg->u32OPMBase1>>16), 0xFFFF);
    //SCMI: OPM address 2
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_54_L, (MS_U16)(pDBreg->u32OPMBase2>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_55_L, (MS_U16)(pDBreg->u32OPMBase2>>16), 0xFFFF);
    //SCMI: OPM offset
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_56_L, (MS_U16)(pDBreg->u16OPMOffset>>00), 0xFFFF);
    //SCMI: OPM fetch
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_57_L, (MS_U16)(pDBreg->u16OPMFetch>>00), 0xFFFF);

    //PIP
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_16_L, (MS_U16)(pDBreg->u16VLen>>00), 0xFFFF); // Write V length
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_1D_L, (MS_U16)((pDBreg->u16LBOffset&0xFF)<<8), 0xFF00);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_1C_L, (MS_U16)((pDBreg->u16DispOffset&0xFF)<<8), 0xFF00);

    // capture window
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_04_L, (MS_U16)(pDBreg->u16V_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_05_L, (MS_U16)(pDBreg->u16H_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_06_L, (MS_U16)(pDBreg->u16V_CapSize>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_07_L, (MS_U16)(pDBreg->u16H_CapSize>>00), 0xFFFF);

    //Display window
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_07_L, (MS_U16)(pDBreg->u16H_DisStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_08_L, (MS_U16)(pDBreg->u16H_DisEnd>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_09_L, (MS_U16)(pDBreg->u16V_DisStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK0F_0A_L, (MS_U16)(pDBreg->u16V_DisEnd>>00), 0xFFFF);

    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_5C_L, 0x0, 0x1FFF); //always disable IP H mirror line offset note:ds will enable this register at vdec firmware

}

void Hal_SC_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
#ifdef STELLAR
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#endif
    _MLOAD_ENTRY(pInstance);

    if( eWindow == MAIN_WINDOW )
    {
        _Hal_SC_fill_main_sw_db_burst(pInstance,pDBreg);
#ifdef STELLAR
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.u32CusMISC_A & E_XC_S_DISPLAY_INITMISC_A_FIRE_DISABLE_ML)
        {
            MDrv_XC_MLoad_Fire(pInstance,FALSE);
        }
        else
#endif
        {
            MDrv_XC_MLoad_Fire(pInstance,TRUE);
        }
    }
    else
    {
        _Hal_SC_fill_sub_sw_db_burst(pInstance,pDBreg);
        MDrv_XC_MLoad_Fire(pInstance,TRUE);
    }

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_dual_sw_db_burst(void *pInstance,P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg)
{
    _MLOAD_ENTRY(pInstance);

    _Hal_SC_fill_main_sw_db_burst(pInstance,pMainDBreg);
    _Hal_SC_fill_sub_sw_db_burst(pInstance,pSubDBreg);
    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_sw_db(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
    if( eWindow == MAIN_WINDOW )
    {
        if(Hal_SC_is_extra_req_en(pInstance,&pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd, NULL, NULL))
        {
            SC_W2BYTEMSK(0,REG_SC_BK20_11_L, BIT(15), BIT(15));
        }
        else
        {
             SC_W2BYTEMSK(0,REG_SC_BK20_11_L, 0, BIT(15));
        }

        SC_W4BYTE(0,REG_SC_BK02_04_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
        SC_W4BYTE(0,REG_SC_BK02_08_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling
        //HVSP
        SC_W4BYTE(0,REG_SC_BK23_07_L, pDBreg->u32H_PostScalingRatio);  // H post-scaling
        SC_W4BYTE(0,REG_SC_BK23_09_L, pDBreg->u32V_PostScalingRatio);  // V post-scaling
        //SCMI
        SC_W2BYTE(0,REG_SC_BK12_18_L, pDBreg->u16VWritelimit); // Set vertical limitation
        SC_W4BYTE(0,REG_SC_BK12_1A_L, pDBreg->u32WritelimitBase);
        SC_W4BYTE(0,REG_SC_BK12_08_L, pDBreg->u32DNRBase0);     // input address0
        //SC_W4BYTE(0,REG_SC_BK12_0A_L, pDBreg->u32DNRBase1);     // input address1
        //SC_W4BYTE(0,REG_SC_BK12_0C_L, pDBreg->u32DNRBase2);     // input address2
        SC_W2BYTE(0,REG_SC_BK12_0E_L, pDBreg->u16DNROffset);// input Offset
        SC_W2BYTE(0,REG_SC_BK12_0F_L, pDBreg->u16DNRFetch); // input fetch
        Hal_SC_set_opm_lcnt_inv(0, _HAL_bopm_lcnt_inv[eWindow], eWindow);
        SC_W4BYTE(0,REG_SC_BK12_10_L, pDBreg->u32OPMBase0); // output address0
        SC_W4BYTE(0,REG_SC_BK12_12_L, pDBreg->u32OPMBase1); // output address1
//        SC_W4BYTE(0,REG_SC_BK12_14_L, pDBreg->u32OPMBase2); // output address2
        SC_W2BYTE(0,REG_SC_BK12_16_L, pDBreg->u16OPMOffset); // output offset
        SC_W2BYTE(0,REG_SC_BK12_17_L, pDBreg->u16OPMFetch);  // output fetch

        //PIP
        SC_W2BYTE(0,REG_SC_BK20_15_L, pDBreg->u16VLen); // Write V length
        SC_W2BYTEMSK(0,REG_SC_BK20_1D_L, (pDBreg->u16LBOffset&0xFF), 0xFF);
        SC_W2BYTEMSK(0,REG_SC_BK20_1C_L, (pDBreg->u16DispOffset&0xFF), 0xFF);

        // capture window
        SC_W2BYTE(0,REG_SC_BK01_04_L, pDBreg->u16V_CapStart); // Capture V start
        SC_W2BYTE(0,REG_SC_BK01_05_L, pDBreg->u16H_CapStart); // Capture H start
        SC_W2BYTE(0,REG_SC_BK01_06_L, pDBreg->u16V_CapSize);  // Capture V size
        SC_W2BYTE(0,REG_SC_BK01_07_L, pDBreg->u16H_CapSize);  // Capture H size

        //Display window
        SC_W2BYTE(0,REG_SC_BK10_08_L, pDBreg->u16H_DisStart); // Display H start
        SC_W2BYTE(0,REG_SC_BK10_09_L, pDBreg->u16H_DisEnd);   // Display H end
        SC_W2BYTE(0,REG_SC_BK10_0A_L, pDBreg->u16V_DisStart); // Display V start
        SC_W2BYTE(0,REG_SC_BK10_0B_L, pDBreg->u16V_DisEnd);   // Display V end

        SC_W2BYTEMSK(0,REG_SC_BK12_1C_L, 0x0, 0x1FFF);         //always disable IP H mirror line offset note:ds will enable this register at vdec firmware
    }
    else
    {

        if(Hal_SC_is_extra_req_en(pInstance,&pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd, NULL, NULL))
        {
            SC_W2BYTEMSK(0,REG_SC_BK20_11_L, BIT(15), BIT(15));
        }
        else
        {
             SC_W2BYTEMSK(0,REG_SC_BK20_11_L, 0, BIT(15));
        }

        SC_W4BYTE(0,REG_SC_BK04_04_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
        SC_W4BYTE(0,REG_SC_BK04_08_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling

        //HVSP
        SC_W4BYTE(0,REG_SC_BK23_27_L, pDBreg->u32H_PostScalingRatio);  // H post-scaling
        SC_W4BYTE(0,REG_SC_BK23_29_L, pDBreg->u32V_PostScalingRatio);  // V post-scaling

        //SCMI
        SC_W2BYTE(0,REG_SC_BK12_58_L, pDBreg->u16VWritelimit); // Set vertical limitation
        SC_W4BYTE(0,REG_SC_BK12_5A_L, pDBreg->u32WritelimitBase);
        SC_W4BYTE(0,REG_SC_BK12_48_L, pDBreg->u32DNRBase0);     // input address0
        //SC_W4BYTE(0,REG_SC_BK12_4A_L, pDBreg->u32DNRBase1); // input address1
        //SC_W4BYTE(0,REG_SC_BK12_4C_L, pDBreg->u32DNRBase2); // input address2
        SC_W2BYTE(0,REG_SC_BK12_4E_L, pDBreg->u16DNROffset);// input Offset
        SC_W2BYTE(0,REG_SC_BK12_4F_L, pDBreg->u16DNRFetch); // input fetch
        Hal_SC_set_opm_lcnt_inv(0, _HAL_bopm_lcnt_inv[eWindow], eWindow);
        SC_W4BYTE(0,REG_SC_BK12_50_L, pDBreg->u32OPMBase0); // output address0
        SC_W4BYTE(0,REG_SC_BK12_52_L, pDBreg->u32OPMBase1); // output address1
        //SC_W4BYTE(0,REG_SC_BK12_54_L, pDBreg->u32OPMBase2); // output address2
        SC_W2BYTE(0,REG_SC_BK12_56_L, pDBreg->u16OPMOffset); // output offset
        SC_W2BYTE(0,REG_SC_BK12_57_L, pDBreg->u16OPMFetch);  // output fetch

        //PIP
        SC_W2BYTE(0,REG_SC_BK20_16_L, pDBreg->u16VLen); // Write V length
        SC_W2BYTEMSK(0,REG_SC_BK20_1D_L, (pDBreg->u16LBOffset&0xFF)<<8, 0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK20_1C_L, (pDBreg->u16DispOffset&0xFF)<<8, 0xFF00);

        // capture window
        SC_W2BYTE(0,REG_SC_BK03_04_L, pDBreg->u16V_CapStart); // Capture V start
        SC_W2BYTE(0,REG_SC_BK03_05_L, pDBreg->u16H_CapStart); // Capture H start
        SC_W2BYTE(0,REG_SC_BK03_06_L, pDBreg->u16V_CapSize);  // Capture V size
        SC_W2BYTE(0,REG_SC_BK03_07_L, pDBreg->u16H_CapSize);  // Capture H size

        //Display window
        SC_W2BYTE(0,REG_SC_BK0F_07_L, pDBreg->u16H_DisStart); // Display H start
        SC_W2BYTE(0,REG_SC_BK0F_08_L, pDBreg->u16H_DisEnd);   // Display H end
        SC_W2BYTE(0,REG_SC_BK0F_09_L, pDBreg->u16V_DisStart); // Display V start
        SC_W2BYTE(0,REG_SC_BK0F_0A_L, pDBreg->u16V_DisEnd);   // Display V end

        SC_W2BYTEMSK(0,REG_SC_BK12_5C_L, 0x0, 0x1FFF);         //always disable IP H mirror line offset note:ds will enable this register at vdec firmware
    }
}

void Hal_SC_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] eWindow:%d bDynamicScalingEnable%d  \n",__func__,__LINE__,eWindow,pXCResourcePrivate->sthal_SC.bDynamicScalingEnable);

    XC_DS_CMDCNT PSTXC_DS_CMDCNT;

    PSTXC_DS_CMDCNT.u16CMDCNT_IPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_IPS =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPS =0;

    if(pXCResourcePrivate->sthal_SC.bDynamicScalingEnable)
    {
        pXCResourcePrivate->sthal_SC.u8DynamicScalingNextIndex[eWindow] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow];


        MS_U32 ActiveSWClearEn = 0;
        MS_U32 u32SW_Reset_Enable = 0;
        //ActiveSWClearEn enable in first DS IP command ; patch for IPM tune after DS
        u32SW_Reset_Enable = 0x0400; // bit10: reg_ds_ipm_active_sel
        ActiveSWClearEn = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_17_L, 0xFFFF);
        ActiveSWClearEn |= u32SW_Reset_Enable;

        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);

        if(eWindow == MAIN_WINDOW)
        {
            //H pre-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_04_L,(MS_U16)(pDBreg->u32H_PreScalingRatio>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8104XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_05_L,(MS_U16)(pDBreg->u32H_PreScalingRatio>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8105XXXX
            //V pre-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_08_L,(MS_U16)(pDBreg->u32V_PreScalingRatio>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8108XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_09_L,(MS_U16)(pDBreg->u32V_PreScalingRatio>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8109XXXX
            //H post-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_07_L,(MS_U16)(pDBreg->u32H_PostScalingRatio>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9187XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_08_L,(MS_U16)(pDBreg->u32H_PostScalingRatio>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9188XXXX
            //V post-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_09_L,(MS_U16)(pDBreg->u32V_PostScalingRatio>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9189XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_0A_L,(MS_U16)(pDBreg->u32V_PostScalingRatio>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 918AXXXX

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_18_L,(MS_U16)(pDBreg->u16VWritelimit>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8918XXXX


            if(IsVMirrorMode(MAIN_WINDOW))
            {
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_08_L,(MS_U16)(pDBreg->u32DNRBase0>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_09_L,(MS_U16)(pDBreg->u32DNRBase0>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
            }

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_0F_L,(MS_U16)(pDBreg->u16DNRFetch>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 890FXXXX

            //H_mirror_offset
            if(IsVMirrorMode(MAIN_WINDOW))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] VB_width:%d framNum:%d u8BitPerPixel:%d  \n",__func__,__LINE__,gSrcInfo[MAIN_WINDOW].Status2.u16VirtualBox_Width,MDrv_XC_TransFrameNumToFactor(pInstance,gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber, gSrcInfo[MAIN_WINDOW].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED),gSrcInfo[MAIN_WINDOW].u8BitPerPixel);

                MS_U32 H_mirror_offset;
                H_mirror_offset = (gSrcInfo[eWindow].Status2.u16VirtualBox_Width * gSrcInfo[eWindow].u8BitPerPixel * 2 / 8 * MDrv_XC_TransFrameNumToFactor(pInstance,gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED)) / BYTE_PER_WORD;

                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_1C_L,H_mirror_offset,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 891CXXXX
            }
            else
            {
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_1C_L,0x0000,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 891CXXXX
            }

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_10_L, (MS_U16)(pDBreg->u32OPMBase0>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM base0  //0xFFFF0009 8910XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_11_L,(MS_U16)(pDBreg->u32OPMBase0>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM base0   //0xFFFF0009 8911XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_17_L,(MS_U16)(pDBreg->u16OPMFetch>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM fetch   //0xFFFF0009 8917XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_15_L, (MS_U16)(pDBreg->u16VLen>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // V Len
            MS_U16 u16LBOffset = (MS_U16)(pDBreg->u16LBOffset&0xFF);
            u16LBOffset |= SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, 0xFF00);
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_1D_L,(MS_U16)(pDBreg->u16LBOffset&0xFF),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //LBOffset    //0xFFFF0009 901DXXXX

            //Cap info
            /// before enable ML bit mask feature, the capture size for 2P setting BK01_07[15] need to be considered
            if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15)))
                pDBreg->u16H_CapSize |= 0x8000;
            else
                pDBreg->u16H_CapSize |= 0x0000;

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK01_06_L,(MS_U16)(pDBreg->u16V_CapSize>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8086XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK01_07_L,(MS_U16)(pDBreg->u16H_CapSize>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8087XXXX


            //Disp info
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_08_L,(MS_U16)(pDBreg->u16H_DisStart),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //0xFFFF0009 8808XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_09_L,(MS_U16)(pDBreg->u16H_DisEnd),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);    //0xFFFF0009 8809XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_0A_L,(MS_U16)(pDBreg->u16V_DisStart),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //0xFFFF0009 880AXXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_0B_L,(MS_U16)(pDBreg->u16V_DisEnd),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);    //0xFFFF0009 880BXXXX

#ifdef UFO_XC_SWDS_SW_FILM_ENABLE // ForceI SW film mode De interlace mode setting
            if ((pXCResourcePrivate->sthal_SC.bDynamicScalingForceI[eWindow] == TRUE) && (eWindow== MAIN_WINDOW))
            {
                MS_U16 u16SWFilmMode = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0A_17_L) & ~(0x1C0); // [8:6]
                if (pXCResourcePrivate->sthal_SC.bCurrentPmode[eWindow] == TRUE
                    /*&& (  pXCResourcePrivate->sthal_SC.u16CurrentFramerate[eWindow] == 25000  || pXCResourcePrivate->sthal_SC.u16CurrentFramerate[eWindow] == 30000 )*/) // FIXME: only 25p/30p
                {
                    if (pXCResourcePrivate->sthal_SC.bIsTopField[eWindow])
                    {
                        u16SWFilmMode |= 0x1C0; //[8:7] = 0x3 SW film enable. [6] = 1 for top field
                    }
                    else
                    {
                        u16SWFilmMode |= 0x180; //[8:7] = 0x3 SW film enable. [6] = 0 for bottom field
                    }
                    //printf("[SWDS] ForceI film mode patch ==> bIsTopField[%x], u16SWFilmMode[%x], u16FrameRate[%u] \n", pXCResourcePrivate->sthal_SC.bIsTopField[eWindow], u16SWFilmMode,pXCResourcePrivate->sthal_SC.u16CurrentFramerate[eWindow]);
                    Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK0A_17_L,u16SWFilmMode,DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                }
                else //disable sw film mode
                {
                    //[8:7] = 0x0 disable.
                    //printf("[SWDS] ForceI film mode patch ==> bIsTopField[%x], u16SWFilmMode[%x], u16FrameRate[%u] \n", pXCResourcePrivate->sthal_SC.bIsTopField[eWindow], u16SWFilmMode,pXCResourcePrivate->sthal_SC.u16CurrentFramerate[eWindow]);
                    Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK0A_17_L,u16SWFilmMode,DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                }
            }
#endif

#ifdef UFO_XC_SWDS_DYMAMIC_CONTROL_DNR
            MS_U16 u16DNR_RegStatus = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L));

            if(gSrcInfo[MAIN_WINDOW].Status2.bEnableDNR)
            {
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK06_21_L,u16DNR_RegStatus,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
            }
            else
            {
                u16DNR_RegStatus &= 0xFFFC;
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK06_21_L,u16DNR_RegStatus,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
            }
#endif

            //toggle reg_ds_ipm_active_set in last DS IP command  ; patch for IPM tune after DS
            MS_U32 u32SW_Reset_IPM = 0;
            u32SW_Reset_IPM = 0x0100 ;//bit8:reg_ds_ipm_active_set
            ActiveSWClearEn |= u32SW_Reset_IPM;
            //Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP);
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);

            MS_U8 u8MaxCmdCnt = pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth * (Hal_XC_MLoad_Get_64Bits_MIU_Bus_Sel(pInstance)+1);

            if(PSTXC_DS_CMDCNT.u16CMDCNT_IPM < u8MaxCmdCnt)
            {
                Hal_SC_Add_NullCommand(pInstance,MAIN_WINDOW,DS_IP,&PSTXC_DS_CMDCNT);
            }
            if(!((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL)))
            {
                if(PSTXC_DS_CMDCNT.u16CMDCNT_OPM < u8MaxCmdCnt)
                {
                    Hal_SC_Add_NullCommand(pInstance,MAIN_WINDOW,DS_OP,&PSTXC_DS_CMDCNT);
                }
            }

        }

    }

}

void Hal_SC_SWDS_Fire(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow]++;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] CurIdx:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow]);

    pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow] % DS_BUFFER_NUM_EX;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] CurIdx_Ring:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow])

    if( pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow] >= DS_BUFFER_NUM_EX )
    {
         Hal_SC_ResetSWDSCommand(pInstance,eWindow, pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow]);
    }
}

E_APIXC_ReturnValue Hal_SC_support_source_to_ve(void *pInstance,MS_U16* pOutputCapability)
{
    *pOutputCapability = E_XC_IP | E_XC_OP2 | E_XC_OVERLAP;
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_set_output_capture_enable(void *pInstance,MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    MS_BOOL bCaptureEna;
    MS_U16 u16CaptureSrc;
    // Current only support IP and OP2
    bCaptureEna = bEnable;
    switch(eSourceToVE)
    {
        case E_XC_OVERLAP:
            u16CaptureSrc = 0;
            break;
        case E_XC_OP2:
            u16CaptureSrc = 1;
            break;
        case E_XC_VOP2:
            u16CaptureSrc = 2;
            break;
        case E_XC_BRI:
            u16CaptureSrc = 3;
            break;
        default:
            u16CaptureSrc = 0;
            bCaptureEna = FALSE;
            break;
    }

    if(bCaptureEna)
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, BIT(11), BIT(11));     // Disable
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, u16CaptureSrc<<4, BMASK(6:4));              // Disable
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, 0, BIT(11));    // Disable
    }

    if(eSourceToVE == E_XC_OP2) // 00: only video, 10 or 11 :video+gop
    {
        SC_W2BYTEMSK(0, REG_SC_BK10_50_L, BIT(2), BMASK(2:1));  //set fde 0: main 1: sub 2: fde
    }

    return E_APIXC_RET_OK; // Support Scaler OP -> VE
}

/******************************************************************************/
///Set output DE window size
///@param pstWindow \b IN
///-  pointer to window information
/******************************************************************************/
void Hal_SC_set_de_window(void *pInstance,XC_PANEL_INFO *pPanel_Info)
{
#ifdef new_chakra
    SC_W2BYTE(0,REG_SC_BK10_04_L, pPanel_Info->u16HStart);                                // DE H start
    SC_W2BYTE(0,REG_SC_BK10_05_L, pPanel_Info->u16HStart + pPanel_Info->u16Width - 1);    // DE H end
    SC_W2BYTE(0,REG_SC_BK10_06_L, pPanel_Info->u16VStart);                                // DE V start
    SC_W2BYTE(0,REG_SC_BK10_07_L, pPanel_Info->u16VStart + pPanel_Info->u16Height - 1);   // DE V end
#else
    SC_W2BYTE(0,REG_SC_BK10_04_L, pPanel_Info->u16HStart);                                // DE H start
    SC_W2BYTE(0,REG_SC_BK10_05_L, pPanel_Info->u16HStart + pPanel_Info->u16Width - 1);    // DE H end
    SC_W2BYTE(0,REG_SC_BK10_06_L, pSrcInfo->u8DE_V_Start);                                // DE V start
    SC_W2BYTE(0,REG_SC_BK10_07_L, pSrcInfo->u16DE_V_End);   // DE V end
#endif
}

void Hal_SC_get_disp_de_window(void *pInstance,MS_WINDOW_TYPE *pWin)
{
    pWin->x = SC_R2BYTE(0,REG_SC_BK10_04_L);
    pWin->y = SC_R2BYTE(0,REG_SC_BK10_06_L);
    pWin->width = SC_R2BYTE(0,REG_SC_BK10_05_L) - pWin->x + 1;
    pWin->height= SC_R2BYTE(0,REG_SC_BK10_07_L) - pWin->y + 1;
}

MS_U32 Hal_SC_Get_DNRBase0(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(0,REG_SC_BK12_08_L);     // input address0
    }
    else
    {
        return SC_R4BYTE(0,REG_SC_BK12_48_L);     // input address0
    }
}

MS_U32 Hal_SC_Get_DNRBase1(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(0,REG_SC_BK12_0A_L);     // input address1
    }
    else
    {
        return SC_R4BYTE(0,REG_SC_BK12_4A_L);     // input address1
    }
}

MS_U32 Hal_SC_Get_OPMBase0(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(0,REG_SC_BK12_10_L);     // output address0
    }
    else
    {
        return SC_R4BYTE(0,REG_SC_BK12_50_L);     // output address0
    }
}

MS_U32 Hal_SC_Get_OPMBase1(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(0,REG_SC_BK12_12_L);     // output address1
    }
    else
    {
        return SC_R4BYTE(0,REG_SC_BK12_52_L);     // output address1
    }
}

void Hal_SC_set_DNRBase0(void *pInstance,MS_U32 u32DNRBase0, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(0,REG_SC_BK12_08_L, u32DNRBase0 / BYTE_PER_WORD);     // input address0
    }
    else
    {
        SC_W4BYTE(0,REG_SC_BK12_48_L, u32DNRBase0 / BYTE_PER_WORD);     // input address0
    }
}

void Hal_SC_set_DNRBase1(void *pInstance,MS_U32 u32DNRBase1, SCALER_WIN eWindow)
{
    UNUSED(u32DNRBase1);
    UNUSED(eWindow);
}

void Hal_SC_set_DNRBase2(void *pInstance,MS_U32 u32DNRBase2, SCALER_WIN eWindow)
{
    UNUSED(u32DNRBase2);
    UNUSED(eWindow);
}

void Hal_SC_set_OPMBase0(void *pInstance,MS_U32 u32OPMBase0, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(0,REG_SC_BK12_10_L, u32OPMBase0 / BYTE_PER_WORD);     // input address0
    }
    else
    {
        SC_W4BYTE(0,REG_SC_BK12_50_L, u32OPMBase0 / BYTE_PER_WORD);     // input address0
    }
}

void Hal_SC_set_OPMBase1(void *pInstance,MS_U32 u32OPMBase1, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(0,REG_SC_BK12_12_L, u32OPMBase1 / BYTE_PER_WORD);     // input address1
    }
    else
    {
        SC_W4BYTE(0,REG_SC_BK12_52_L, u32OPMBase1 / BYTE_PER_WORD);     // input address1
    }
}

void Hal_SC_set_OPMBase2(void *pInstance,MS_U32 u32OPMBase2, SCALER_WIN eWindow)
{
    UNUSED(u32OPMBase2);
    UNUSED(eWindow);
}

void Hal_SC_set_WriteVLength(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(0,REG_SC_BK20_15_L, 0x0);     // Write V length
    }
    else
    {
        SC_W2BYTE(0,REG_SC_BK20_16_L, 0x0);     // Write V length
    }
}

void Hal_SC_set_memoryaddress(void *pInstance,MS_U32 u32DNRBase0, MS_U32 u32DNRBase1, MS_U32 u32DNRBase2, MS_U32 u32OPMBase0, MS_U32 u32OPMBase1, MS_U32 u32OPMBase2, SCALER_WIN eWindow)
{
    Hal_SC_set_DNRBase0(pInstance,u32DNRBase0, eWindow);
    Hal_SC_set_DNRBase1(pInstance,u32DNRBase1, eWindow);
    Hal_SC_set_DNRBase2(pInstance,u32DNRBase2, eWindow);
    Hal_SC_set_OPMBase0(pInstance,u32OPMBase0, eWindow);
    Hal_SC_set_OPMBase1(pInstance,u32OPMBase1, eWindow);
    Hal_SC_set_OPMBase2(pInstance,u32OPMBase2, eWindow);
}

static SC_MIUMASK_t Hal_SC_get_miu0mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU0_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU0_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU0_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU0_G3_REQUEST_MASK);

    return mask;
}

static SC_MIUMASK_t Hal_SC_get_miu1mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU1_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU1_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU1_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU1_G3_REQUEST_MASK);

    return mask;
}

static void Hal_SC_set_miu0mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU0_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU0_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU0_G2_REQUEST_MASK, mask.u16MiuG2Mask);
    MDrv_Write2Byte(MIU0_G3_REQUEST_MASK, mask.u16MiuG3Mask);
}

static void Hal_SC_set_miu1mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU1_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU1_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU1_G2_REQUEST_MASK, mask.u16MiuG2Mask);
    MDrv_Write2Byte(MIU1_G3_REQUEST_MASK, mask.u16MiuG3Mask);
}

void Hal_SC_Enable_MiuMask(void *pInstance)
{
    Miu0MaskOld = Hal_SC_get_miu0mask();
    Miu1MaskOld = Hal_SC_get_miu1mask();

    Miu0Mask = Miu0MaskOld;
    Miu1Mask = Miu1MaskOld;

    Miu0Mask.u16MiuG0Mask |= MIU_SC_G0REQUEST_MASK;
    Miu0Mask.u16MiuG1Mask |= MIU_SC_G1REQUEST_MASK;
    Miu0Mask.u16MiuG2Mask |= MIU_SC_G2REQUEST_MASK;
    Miu0Mask.u16MiuG3Mask |= MIU_SC_G3REQUEST_MASK;

    Miu1Mask.u16MiuG0Mask |= MIU_SC_G0REQUEST_MASK;
    Miu1Mask.u16MiuG1Mask |= MIU_SC_G1REQUEST_MASK;
    Miu1Mask.u16MiuG2Mask |= MIU_SC_G2REQUEST_MASK;
    Miu1Mask.u16MiuG3Mask |= MIU_SC_G3REQUEST_MASK;

    Hal_SC_set_miu0mask(Miu0Mask);
    Hal_SC_set_miu1mask(Miu1Mask);
}

void Hal_SC_Disable_MiuMask(void *pInstance)
{
    Hal_SC_set_miu0mask(Miu0MaskOld);
    Hal_SC_set_miu1mask(Miu1MaskOld);
}

void Hal_SC_set_miusel(void *pInstance,MS_U8 u8MIUSel)
{
    // Scaler control MIU by itself
    // So MIU1 IP-select is set to 1. At this moment, MIU0 Miu select is not working.

    if (u8MIUSel==0)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_05_L, 0x0000, 0x0010);     // reg_miu_select_f2 to MIU0
        SC_W2BYTEMSK(0,REG_SC_BK12_44_L, 0x0000, 0x4000);     // reg_miu_select_f1 to MIU0

        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF3, 0x00, 0x0E);
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x30);

        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF3, 0x0E, 0x0E);
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x30, 0x30);
    /* Munich Special Items There are two MIU in this chip, but scaler related IP only can use one MIU.
    ** 0x130008 BITS[7:5] miu to scaler read data / scaler to miu write byte / scaler to miu write data
    ** Set Both to 0:miu0,1:miu1
    */
        SC_W2BYTEMSK(0,REG_SC_BK00_40_L, 0x0000, 0x00E0);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_05_L, 0x0010, 0x0010);     // reg_miu_select_f2 to MIU1
        SC_W2BYTEMSK(0,REG_SC_BK12_44_L, 0x4000, 0x4000);     // reg_miu_select_f1 to MIU1

        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF3, 0x00, 0x0E);
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x30);

        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF3, 0x0E, 0x0E);
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x30, 0x30);
    /* Munich Special Items There are two MIU in this chip, but scaler related IP only can use one MIU.
    ** 0x130008 BITS[7:5] miu to scaler read data / scaler to miu write byte / scaler to miu write data
    ** Set Both to 0:miu0,1:miu1
    */
        SC_W2BYTEMSK(0,REG_SC_BK00_40_L, 0x00E0, 0x00E0);
    }
}

void Hal_SC_set_blsk(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK10_21_L, bEn ? BIT(12) : 0, BIT(12));
}

void Hal_SC_set_blsk_burst(void *pInstance,MS_BOOL bEn)
{
    _MLOAD_ENTRY(pInstance);
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_21_L, bEn ? BIT(12) : 0, BIT(12));
    MDrv_XC_MLoad_Fire(pInstance,TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_black_screen_burst(void *pInstance,MS_BOOL bEn)
{
    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, bEn ? BIT(1) : 0, BIT(1));
    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_sub_black_screen_burst(void *pInstance,MS_BOOL bEn)
{
    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, bEn ? (BIT(5)|BIT(1)) : 0, (BIT(6)|BIT(5)|BIT(1)));
    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_black_screen(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK10_19_L, bEn ? BIT(1) : 0, BIT(1));
}

void Hal_SC_set_main_sub_black_screen(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK10_19_L, bEn ? (BIT(5)|BIT(1)) : 0, (BIT(6)|BIT(5)|BIT(1)));
}

void Hal_SC_set_sub_blue_screen_burst(void *pInstance,MS_BOOL bEn, MS_BOOL bBlue)
{
    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, bBlue ? BIT(6) : 0, BIT(6));
    MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK10_19_L, bEn ? BIT(5) : 0, BIT(5));
    MDrv_XC_MLoad_Fire(pInstance,TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_sub_blue_screen(void *pInstance,MS_BOOL bEn, MS_BOOL bBlue)
{
    SC_W2BYTEMSK(0,REG_SC_BK10_19_L, bBlue ? BIT(6) : 0, BIT(6));
    SC_W2BYTEMSK(0,REG_SC_BK10_19_L, bEn ? BIT(5) : 0, BIT(5));
}

E_APIXC_ReturnValue Hal_SC_IP_Memory_Write_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(MAIN_WINDOW == eWindow)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_07_L, bEnable? 0 : BIT(0) , BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_47_L, bEnable? 0 : BIT(0) , BIT(0));
    }
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_IP_Memory_Read_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(MAIN_WINDOW == eWindow)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_07_L, bEnable? 0 : BIT(1) , BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_47_L, bEnable? 0 : BIT(1) , BIT(1));
    }
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_OP_Memory_Write_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    SC_W2BYTEMSK(0,REG_SC_BK12_67_L, bEnable? 0 : BIT(1) , BIT(1));//This will stop OPW and OP memory write request
    return E_APIXC_RET_OK;
}

void Hal_SC_disable_inputsource_burst(void *pInstance,MS_BOOL bDisable, SCALER_WIN eWindow)
{
    _MLOAD_ENTRY(pInstance);

    // HW PIP architeucture
    // Becasue BK3_02[8] and BK20_11[15] can not be enabled toghter,
    // otherwise garbage will be showed,we need to use BK12_47[0] to instead.
    if( eWindow == MAIN_WINDOW )
    {
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK01_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
    else
    {
        //MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_02_L, (bDisable ? BIT(7):0), BIT(7));
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK12_47_L, (bDisable ? BIT(0) : 0), BIT(0));
    }
    MDrv_XC_MLoad_Fire(pInstance,TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_disable_inputsource(void *pInstance,MS_BOOL bDisable, SCALER_WIN eWindow)
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK03_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
}


MS_U16 Hal_SC_Is_InputSource_Disable(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(0,REG_SC_BK01_02_L)&0x80);
    else
    {
        if((SC_R2BYTE(0,REG_SC_BK12_47_L)&0x01) == 0)
        {
            return (SC_R2BYTE(0,REG_SC_BK03_02_L)&0x80);
        }
        else
        {
            return TRUE;
        }
    }
}


void Hal_SC_set_nosignal_color(void *pInstance,MS_U8 u8Color,SCALER_WIN eWindow)
{
    if ( eWindow == MAIN_WINDOW )
    {
#ifdef new_chakra
        SC_W2BYTEMSK(0,REG_SC_BK10_24_L, u8Color, LBMASK);
#else
        SC_W2BYTEMSK(0,REG_SC_BK10_24_L, u8Color, LBMASK);
#endif
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_17_L, ((MS_U16)u8Color)<<8, HBMASK);
    }
}

void Hal_SC_set_fbl(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK12_01_L, (bEn ? BIT(7): 0), BIT(7));  //Enable/Disable FBL
    SC_W2BYTEMSK(0,REG_SC_BK12_02_L, (bEn ? BIT(14):0), BIT(14)); //F2 force IP read request disable

    SC_W2BYTEMSK(0,REG_SC_BK12_07_L, (bEn ? 0x3:0), 0x3); //F2 IP read/write request disable
}

MS_BOOL Hal_SC_get_fbl(void *pInstance)
{
    return SC_R2BYTEMSK(0,REG_SC_BK12_01_L, BIT(7));  //Enable/Disable FBL
}

void Hal_SC_set_freezeimg_burst(void *pInstance,MS_BOOL bEn, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _MLOAD_ENTRY(pInstance);
    if( eWindow == MAIN_WINDOW )
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_01_L, (bEn<<11), BIT(11));
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_41_L, (bEn<<11), BIT(11));
    }
    MDrv_XC_MLoad_Fire(pInstance, TRUE);
    if(pXCResourcePrivate->stdrvXC_Scaling._au8StoreFrameNumFactor[eWindow] <= 2)
    {
      MDrv_XC_wait_input_vsync(pInstance, 1, 100, eWindow);
    }
    _MLOAD_RETURN(pInstance);
}


void Hal_SC_set_freezeimg(void *pInstance,MS_BOOL bEn, SCALER_WIN eWindow)
{
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_01_L, (bEn<<11), BIT(11));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_41_L, (bEn<<11), BIT(11));
    }
}

MS_BOOL Hal_SC_get_freezeimg(void *pInstance,SCALER_WIN eWindow)
{
    if( eWindow == MAIN_WINDOW )
    {
        return (SC_R2BYTEMSK(0,REG_SC_BK12_01_L, BIT(11)) ? TRUE: FALSE);
    }
    else
    {
        return (SC_R2BYTEMSK(0,REG_SC_BK12_41_L, BIT(11)) ? TRUE: FALSE);
    }
}

void Hal_SC_exwith_miuprotect_swreset(void *pInstance,MS_U8 x)
{
    //reset IP1F2
    //select pattern generator source
    SC_W2BYTEMSK(0,REG_SC_BK00_02_L, x, x);
    //MsOS_DelayTask(2);

    //select pattern generator source
    SC_W2BYTEMSK(0,REG_SC_BK00_02_L, 0, x);
    //MsOS_DelayTask(2);
}

void Hal_SC_ip_Init_for_internal_timing(void *pInstance,XC_Internal_TimingType timingtype, SCALER_WIN eWindow)
{

    SC_W2BYTE(0,REG_SC_BK01_02_L, 0x104) ;  //Video source
    Hal_SC_exwith_miuprotect_swreset(pInstance,REST_IP_F2);  //when source is changed, we must reset it to make it work
    SC_W2BYTE(0,REG_SC_BK01_03_L, 0x890);

    SC_W2BYTEMSK(0,REG_SC_BK01_21_L, 0x01, 0x03);   //force progressive mode
    SC_W2BYTEMSK(0,REG_SC_BK01_0E_L, BIT(15), BIT(15));   //auto gain 10bit

    SC_W2BYTE(0,REG_SC_BK01_04_L, 0x020);  //capture V start
    SC_W2BYTE(0,REG_SC_BK01_05_L, 0x50) ;  //capture H start

    switch (timingtype)
    {
        case E_XC_480P:
        default:

            SC_W2BYTE(0,REG_SC_BK01_06_L, 0x1c0) ; // V size
            SC_W2BYTE(0,REG_SC_BK01_07_L, 0x280) ; // h size

            break;
        case E_XC_720P:
            SC_W2BYTE(0,REG_SC_BK01_06_L, 0x290) ; // V size (720 - 64) = 656 = 0x290
            SC_W2BYTE(0,REG_SC_BK01_07_L, 0x460) ; // h size (1280 - 160) = 1120 = 0x460

            SC_W2BYTE(0,REG_SC_BK12_0E_L, 0x0800); //DNR Offset
            SC_W2BYTE(0,REG_SC_BK12_0F_L, 0x0460); //DNR Fetch
            SC_W2BYTE(0,REG_SC_BK12_16_L, 0x0800); //OPM Offsest
            SC_W2BYTE(0,REG_SC_BK12_17_L, 0x0460); //OPM Fetch

            break;
        case E_XC_1080P:
            SC_W2BYTE(0,REG_SC_BK01_06_L, 0x3F8) ; // V size (1080 - 64) = 1016 = 0x3F8
            SC_W2BYTE(0,REG_SC_BK01_07_L, 0x6E0) ; // h size (1920 - 160) = 1760 = 0x6E0

            SC_W2BYTE(0,REG_SC_BK12_0E_L, 0x0800); //DNR Offset
            SC_W2BYTE(0,REG_SC_BK12_0F_L, 0x06E0); //DNR Fetch
            SC_W2BYTE(0,REG_SC_BK12_16_L, 0x0800); //OPM Offsest
            SC_W2BYTE(0,REG_SC_BK12_17_L, 0x06E0); //OPM Fetch
            break;
    }
}

MS_U16 Hal_SC_rgb_average_info(void *pInstance,MS_U16 u16mask, SCALER_WIN eWindow)
{
    MS_U16 u16value;

    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_0F_L, u16mask, 0x0F);
        u16value=SC_R2BYTE(0,REG_SC_BK01_11_L);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK03_0F_L, u16mask, 0x0F);
        u16value=SC_R2BYTE(0,REG_SC_BK03_11_L);
    }

    return u16value;
}


void Hal_SC_autogain_enable(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    //auto gain enable
    if(eWindow == MAIN_WINDOW)
    {
        if(bEnable)
            W2BYTEMSK(REG_SC_BK01_0E_L, 0x11, 0x11);
        else
            W2BYTEMSK(REG_SC_BK01_0E_L, 0x00, 0x11);
    }
    else
    {
        if(bEnable)
            W2BYTEMSK(REG_SC_BK03_0E_L, 0x11, 0x11);
        else
            W2BYTEMSK(REG_SC_BK03_0E_L, 0x00, 0x11);
    }
}

MS_BOOL Hal_SC_autogain_status(void *pInstance,SCALER_WIN eWindow)
{
    MS_BOOL bvalue;
    if(eWindow == MAIN_WINDOW)
        bvalue = SC_R2BYTEMSK(0,REG_SC_BK01_0E_L, BIT(1));
    else
        bvalue = SC_R2BYTEMSK(0,REG_SC_BK03_0E_L, BIT(1));

    return bvalue;

}

void Hal_SC_set_mirror(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    MS_U32 u32NoSigReg, u32MirrorReg;

    if(eWindow == MAIN_WINDOW)
    {
        u32NoSigReg = REG_SC_BK01_02_L;
        u32MirrorReg = REG_SC_BK12_03_L;
    }
    else
    {
        u32NoSigReg = REG_SC_BK03_02_L;
        u32MirrorReg = REG_SC_BK12_43_L;

    }

    SC_W2BYTEMSK(0,u32NoSigReg, BIT(7), BIT(7));

    if( bEnable )
    {
        SC_W2BYTEMSK(0,u32MirrorReg, (BIT(12)|BIT(13)), (BIT(12)|BIT(13)));
    }
    else
    {
        SC_W2BYTEMSK(0,u32MirrorReg, 0x0, (BIT(12)|BIT(13)));
    }
}

MS_U16 Hal_SC_GetAVDStatus(void)
{
    return MDrv_Read2Byte(L_BK_AFEC(0x66));
}

void Hal_SC_set_rep_window(void *pInstance,MS_BOOL bEnable,MS_U16 x,MS_U16 y,MS_U16 w,MS_U16 h,MS_U8 u8Color)
{
    SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, bEnable?BIT(9):0x0, BIT(9));
    SC_W2BYTE(0,REG_SC_BK0F_42_L,x);
    SC_W2BYTE(0,REG_SC_BK0F_43_L,x + w);
    SC_W2BYTE(0,REG_SC_BK0F_44_L,y);
    SC_W2BYTE(0,REG_SC_BK0F_45_L,y + h);
    SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, (~u8Color)<<8, 0xFF00);
}

//--------------------------------------------------
//update display window registers with input window
//IN:
//   pdspwin: input window info
//--------------------------------------------------
void Hal_SC_set_disp_window(void *pInstance,SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    if(eWindow == MAIN_WINDOW)
    {
        //Display window
        SC_W2BYTE(0,REG_SC_BK10_08_L, pdspwin->x);                  // Display H start
        SC_W2BYTE(0,REG_SC_BK10_09_L, pdspwin->x+pdspwin->width-1);   // Display H end
        SC_W2BYTE(0,REG_SC_BK10_0A_L, pdspwin->y);                  // Display V start
        SC_W2BYTE(0,REG_SC_BK10_0B_L, pdspwin->y+pdspwin->height-1);  // Display V end
    }
    else
    {
        //Display window
        SC_W2BYTE(0,REG_SC_BK0F_07_L, pdspwin->x);                  // Display H start
        SC_W2BYTE(0,REG_SC_BK0F_08_L, pdspwin->x+pdspwin->width-1);   // Display H end
        SC_W2BYTE(0,REG_SC_BK0F_09_L, pdspwin->y);                  // Display V start
        SC_W2BYTE(0,REG_SC_BK0F_0A_L, pdspwin->y+pdspwin->height-1);  // Display V end
    }
}

//--------------------------------------------------
//get display window registers setting
//OUT:
//   pdspwin: Pointer for ouput disp window register
//--------------------------------------------------
void Hal_SC_get_disp_window(void *pInstance,SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    if(eWindow == MAIN_WINDOW)
    {
        //Display window
        pdspwin->x = SC_R2BYTE(0,REG_SC_BK10_08_L);                   // Display H start
        pdspwin->width  = SC_R2BYTE(0,REG_SC_BK10_09_L) - pdspwin->x + 1; // Display H end
        pdspwin->y = SC_R2BYTE(0,REG_SC_BK10_0A_L);                   // Display V start
        pdspwin->height = SC_R2BYTE(0,REG_SC_BK10_0B_L) - pdspwin->y + 1; // Display V end
    }
    else
    {
        //Display window
        pdspwin->x = SC_R2BYTE(0,REG_SC_BK0F_07_L);                   // Display H start
        pdspwin->width  = SC_R2BYTE(0,REG_SC_BK0F_08_L) - pdspwin->x + 1; // Display H end
        pdspwin->y = SC_R2BYTE(0,REG_SC_BK0F_09_L);                   // Display V start
        pdspwin->height = SC_R2BYTE(0,REG_SC_BK0F_0A_L) - pdspwin->y + 1; // Display V end
    }
}

void Hal_SC_set_Fclk(void *pInstance,EN_SET_FCLK_CASE enCase)
{
        MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_216MHZ, CKG_FCLK_MASK);
}

void Hal_SC_get_framebuf_Info(void *pInstance,SC_FRAMEBUF_INFO_t *pFrameBufInfo,SCALER_WIN eWindow)
{
    MS_U8 u8Reg;

    if (!pFrameBufInfo) return;

    pFrameBufInfo->u32IPMBase0 = ((SC_R2BYTEMSK(0,REG_SC_BK12_09_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(0,REG_SC_BK12_08_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u32IPMBase1 = ((SC_R2BYTEMSK(0,REG_SC_BK12_0B_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(0,REG_SC_BK12_0A_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u32IPMBase2 = ((SC_R2BYTEMSK(0,REG_SC_BK12_0D_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(0,REG_SC_BK12_0C_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u16IPMOffset = SC_R2BYTE(0,REG_SC_BK12_0E_L);
    pFrameBufInfo->u16IPMFetch = SC_R2BYTE(0,REG_SC_BK12_0F_L);
    pFrameBufInfo->u16VLength = SC_R2BYTEMSK(0,REG_SC_BK12_18_L, 0x1FFF);
    pFrameBufInfo->bLinearAddrMode = SC_R2BYTEMSK(0,REG_SC_BK12_03_L, BIT(4));
    pFrameBufInfo->bYCSeparate = ( SC_R2BYTEMSK(0,REG_SC_BK12_02_L, BIT(10)) )?TRUE:FALSE;
    pFrameBufInfo->u32WriteLimitBase = SC_R4BYTE(0,REG_SC_BK12_1A_L);
    pFrameBufInfo->u8BitPerPixel = 24;//default

    u8Reg = SC_R2BYTEMSK(0,REG_SC_BK12_01_L, 0x00FF);
    if (u8Reg & 0x30)
    {//444
        pFrameBufInfo->bMemFormat422 = FALSE;
        if (u8Reg & 0x10)
        { // 444 8BIT
            pFrameBufInfo->u8BitPerPixel = 24;
        }
        else
        { // 444 10BIT
            pFrameBufInfo->u8BitPerPixel = 32;
        }
        pFrameBufInfo->bInterlace = FALSE;
    }
    else
    {//422
        pFrameBufInfo->bMemFormat422 = TRUE;

        if((BIT(8)|BIT(9)) == SC_R2BYTEMSK(0,REG_SC_BK12_02_L, BIT(8)|BIT(9))) //user mode
        {
            MS_U8 u8MemFmt = SC_R2BYTE(0,REG_SC_BK12_02_L) & 0x0F;
            if((4 == u8MemFmt) || (5 == u8MemFmt) || (6 == u8MemFmt) || (7 == u8MemFmt))
            {
                pFrameBufInfo->u8BitPerPixel = 20;
            }
            else if((8 == u8MemFmt) || (9 == u8MemFmt) || (10 == u8MemFmt) || (11 == u8MemFmt))
            {
                pFrameBufInfo->u8BitPerPixel = 24;
            }
        }
        else
        {
            u8Reg = (SC_R2BYTEMSK(0,REG_SC_BK12_01_L, 0xFF00)) >> 8;

            switch(u8Reg & 0x77)
            {
                case 0x25:
                    pFrameBufInfo->u8BitPerPixel = 24;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x24:
                    pFrameBufInfo->u8BitPerPixel = 20;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x27:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x21:
                    if(BIT(1) == SC_R2BYTEMSK(0,REG_SC_BK12_04_L, BIT(0)|BIT(1)))
                    {
                        pFrameBufInfo->u8BitPerPixel = 24;
                    }
                    else
                    {
                        pFrameBufInfo->u8BitPerPixel = 20;
                    }
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
                case 0x20:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
                case 0x00:
                default:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
            }
        }
    }

    //In M10/J2/A5/A6/A3, New SCMI
    pFrameBufInfo->u8FBNum = (MS_U8)(SC_R2BYTEMSK(0,REG_SC_BK12_19_L, 0x1F));
#if 0
    printf("base0=0x%x, base1=0x%x, base2=0x%x, offset=%u, 422=%u, i=%u, bpp=%u, fbnum=%u\n",
        (unsigned int)pFrameBufInfo->u32IPMBase0,
        (unsigned int)pFrameBufInfo->u32IPMBase1,
        (unsigned int)pFrameBufInfo->u32IPMBase2,
        pFrameBufInfo->u16IPMOffset,
        pFrameBufInfo->bMemFormat422,
        pFrameBufInfo->bInterlace,
        pFrameBufInfo->u8BitPerPixel,
        pFrameBufInfo->u8FBNum);
#endif
}
void Hal_SC_set_framebuf_Info(void *pInstance,SC_FRAMEBUF_INFO_t pFrameBufInfo)
{
    MS_U32 u32IPMBase0, u32IPMBase1, u32IPMBase2;

    u32IPMBase0=pFrameBufInfo.u32IPMBase0/BYTE_PER_WORD;
    u32IPMBase1=pFrameBufInfo.u32IPMBase1/BYTE_PER_WORD;
    u32IPMBase2=pFrameBufInfo.u32IPMBase2/BYTE_PER_WORD;

    //Step 5.4, Memory Limit V : BK12_18[15] = 1, BK12_18[12:0] to the real V line number
    SC_W2BYTEMSK(0,REG_SC_BK12_18_L, (pFrameBufInfo.u16VLength|0x8000),0x9FFF);

//    u32WritelimitBase = ((u32IPMBase0 + u32MemSize) / BYTE_PER_WORD - 1) | 0x2000000;
    SC_W4BYTE(0,REG_SC_BK12_1A_L, pFrameBufInfo.u32WriteLimitBase);

    SC_W2BYTEMSK(0,REG_SC_BK12_09_L, (MS_U16)(u32IPMBase0 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(0,REG_SC_BK12_08_L, (MS_U16)(u32IPMBase0 & 0xFFFF));

    SC_W2BYTEMSK(0,REG_SC_BK12_0B_L, (MS_U16)(u32IPMBase1 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(0,REG_SC_BK12_0A_L, (MS_U16)(u32IPMBase1 & 0xFFFF));

    SC_W2BYTEMSK(0,REG_SC_BK12_0D_L, (MS_U16)(u32IPMBase2 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(0,REG_SC_BK12_0C_L, (MS_U16)(u32IPMBase2 & 0xFFFF));


    SC_W2BYTE(0,REG_SC_BK12_0E_L, pFrameBufInfo.u16IPMOffset);
    SC_W2BYTE(0,REG_SC_BK12_0F_L, pFrameBufInfo.u16IPMFetch);
//    SC_W2BYTEMSK(0,REG_SC_BK12_18_L, (pFrameBufInfo.u16VLength|0x1000),0x1FFF);
    SC_W2BYTEMSK(0,REG_SC_BK12_03_L, ((pFrameBufInfo.bLinearAddrMode)?BIT(4):0),BIT(4));

}

MS_U8 Hal_SC_get_cs_det_cnt(void *pInstance,SCALER_WIN eWindow)
{
    MS_U8 u8val;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(0,REG_SC_BK01_0C_L) & 0xFF00) >> 8);
    }
    else
    {
        u8val = ((SC_R2BYTE(0,REG_SC_BK03_0C_L) & 0xFF00) >> 8);
    }
    return u8val;
}

void Hal_SC_set_cs_det_cnt(void *pInstance,MS_U8 u8val, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_0C_L, (((MS_U16)u8val)<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK03_0C_L, (((MS_U16)u8val)<<8), 0xFF00);
    }
}

MS_U8 Hal_SC_get_plus_width(void *pInstance,SCALER_WIN eWindow)
{
    MS_U8 u8val;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = (SC_R2BYTE(0,REG_SC_BK01_22_L) & 0x00FF);
    }
    else
    {
        u8val = (SC_R2BYTE(0,REG_SC_BK03_22_L) & 0x00FF);
    }
    return u8val;
}

void Hal_SC_set_opm_fetch(void *pInstance,SCALER_WIN eWindow, MS_U16 u16OPMFetch)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_17_L, u16OPMFetch, 0x0FFF);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_57_L, u16OPMFetch, 0x0FFF);
    }
}

MS_U16 Hal_SC_get_opm_fetch(void *pInstance,SCALER_WIN eWindow)
{
    MS_U16 u16Val = 0;
    if(eWindow == MAIN_WINDOW)
    {
        u16Val = SC_R2BYTE(0,REG_SC_BK12_17_L) & 0x0FFF;
    }
    else
    {
        u16Val = SC_R2BYTE(0,REG_SC_BK12_57_L) & 0x0FFF;
    }
    return u16Val;
}

MS_U8 HAL_SC_get_dnr_setting(void *pInstance,SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
        return (MS_U8)SC_R2BYTEMSK(0,REG_SC_BK06_21_L, BIT(1)|BIT(0));
    else
        return (MS_U8)SC_R2BYTEMSK(0,REG_SC_BK06_01_L, BIT(1)|BIT(0));
}

void HAL_SC_enable_field_avg_y(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK05_21_L, bEnable ? BIT(6) : 0, BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK05_01_L, bEnable ? BIT(6) : 0, BIT(6));
    }

}

void HAL_SC_enable_field_avg_c(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK05_21_L, bEnable ? BIT(7) : 0, BIT(7));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK05_01_L, bEnable ? BIT(7) : 0, BIT(7));
    }
}

void HAL_SC_enable_dnr(void *pInstance,MS_U8 u8Val, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(0,REG_SC_BK06_21_L, u8Val, BIT(1)|BIT(0));
    else
        SC_W2BYTEMSK(0,REG_SC_BK06_01_L, u8Val, BIT(1)|BIT(0));
}

void HAL_SC_VOP_Set_Contrast_En(void *pInstance,MS_BOOL bEenable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(0):0), BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(8):0), BIT(8));
    }
}
void HAL_SC_VOP_Set_Contrast_Value(void *pInstance,MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(0,REG_SC_BK0F_47_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(0,REG_SC_BK0F_48_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(0,REG_SC_BK0F_49_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(0,REG_SC_BK0F_47_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_48_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_49_L, u16Val, 0x0FFF);
                break;
        }
    }
    else
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4A_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4B_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4C_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4A_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_4B_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_4C_L, u16Val, 0x0FFF);
                break;
        }
    }
}

void HAL_SC_VOP_Set_Brightness_En(void *pInstance,MS_BOOL bEenable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(1):0), BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(9):0), BIT(9));
    }
}
void HAL_SC_VOP_Set_Brightness_Value(void *pInstance,MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4D_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4E_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4F_L, u16Val, 0x07FF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(0,REG_SC_BK0F_4D_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_4E_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_4F_L, u16Val, 0x07FF);
                break;
        }
    }
    else
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(0,REG_SC_BK0F_50_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(0,REG_SC_BK0F_51_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(0,REG_SC_BK0F_52_L, u16Val, 0x07FF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(0,REG_SC_BK0F_50_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_51_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(0,REG_SC_BK0F_52_L, u16Val, 0x07FF);
                break;
        }
    }
}

MS_BOOL HAL_SC_Set_DynamicScaling(void *pInstance,MS_U32 u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On)
{
    MS_U32 u32Base = u32MemBaseAddr;// / BYTE_PER_WORD;
    MS_U16 u16OnOff = 0;
    static MS_U16 su16OldValue = 0;
    static MS_BOOL sbOldValueReaded = FALSE;
    static MS_U16 u16OldTrain = 0;
    static MS_U16 u16OldDisp = 0;
    static MS_BOOL bTrig_pStored = FALSE;

    if( u32Base > HAL_MIU1_BASE)
    {
        u32Base = u32Base - HAL_MIU1_BASE;
    }
    u32Base = u32Base/BYTE_PER_WORD;

    // disable DS before modifying DS configuration
    if (MApi_XC_MLoad_GetStatus_U2(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK1F_10_L, 0, 0x0E00);
        MDrv_XC_MLoad_Fire(pInstance,TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK1F_10_L, 0, 0x0E00);
    }

    // enable/disable
    u16OnOff = SC_R2BYTE(0,REG_SC_BK1F_10_L) & 0x3000;
    u16OnOff |= ((bOP_On << 9) | (bIPS_On << 10) | (bIPM_On << 11));

    if(u16OnOff & 0x0E00)
    {
        // ENABLE DS
        u16OnOff |= BIT(12);     // enable write register through RIU
        bDynamicScalingEnable = TRUE;

        // currently, these 3 will use same MIU
        u16OnOff |= ((u8MIU_Select & 0x01) << 13);                       // MIU select of OP
        u16OnOff |= ((u8MIU_Select & 0x01) << 14);                       // MIU select of IPS
        u16OnOff |= ((u8MIU_Select & 0x01) << 15);                       // MIU select of IPM

        // patch IP1F2_21[15:14] r/w bank
        su16OldValue = SC_R2BYTE(0,REG_SC_BK01_21_L);
        sbOldValueReaded = TRUE;

        SC_W2BYTEMSK(0,REG_SC_BK01_21_L, 0x4000, 0xC000);
        Hal_XC_MLoad_set_opm_lock(pInstance,OPM_LOCK_DS);
    }
    else
    {
        bDynamicScalingEnable = FALSE;
        // DISABLE DS
        if(sbOldValueReaded)
        {
            SC_W2BYTE(0,REG_SC_BK01_21_L, su16OldValue);
            sbOldValueReaded = FALSE;
        }
        Hal_XC_MLoad_set_opm_lock(pInstance,OPM_LOCK_INIT_STATE);
    }

    // if disabled, need to set dynamic scaling ratio to 1:1
    if(!bOP_On)
    {
        SC_W4BYTE(0,REG_SC_BK23_1C_L, 0x00100000);
        SC_W4BYTE(0,REG_SC_BK23_1E_L, 0x00100000);
    }

    if(u16OnOff & 0x0E00)
    {
        // set base address
        SC_W2BYTE(0,REG_SC_BK1F_11_L, u32Base & 0xFFFF);
        SC_W2BYTEMSK(0,REG_SC_BK1F_12_L, (u32Base >> 16) & 0x1FF, 0x01FF);

        // set index depth
        SC_W2BYTEMSK(0,REG_SC_BK1F_13_L, u8IdxDepth , 0x00FF);

        // MIU sel
        SC_W2BYTEMSK(0,REG_SC_BK1F_10_L, u16OnOff & 0xE000, 0xE000);

        // set DMA threthold, length, fixed value, no need to change
        SC_W2BYTEMSK(0,REG_SC_BK1F_10_L, 0x88, 0x00FF);

        // store
        Hal_XC_MLoad_get_trig_p(pInstance,&u16OldTrain, &u16OldDisp);
        bTrig_pStored = TRUE;

        // set the trigger point from delayed line,  DS: 0x08, Train:0x14, Disp:0x18
        SC_W2BYTEMSK(0,REG_SC_BK20_21_L, 0x08, 0xFF);  // ds_trig_dly
        Hal_XC_MLoad_set_trig_p(pInstance,0x14, 0x18);

        // set trigger source to trigger
        SC_W2BYTEMSK(0,REG_SC_BK20_19_L, 0x08F9, 0x08FF);
    }
    else
    {
        // restore
        if (bTrig_pStored)
        {
            bTrig_pStored = FALSE;
            Hal_XC_MLoad_set_trig_p(pInstance,u16OldTrain, u16OldDisp);
        }
    }

    // set DS in IPS[10],ipm[11] On/Off
    if (MApi_XC_MLoad_GetStatus_U2(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK1F_10_L, u16OnOff & 0x1D00, 0x1D00);
        MDrv_XC_MLoad_Fire(pInstance,TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK1F_10_L, u16OnOff & 0x1D00, 0x1D00);
    }

    // set DS in OP[9] On/Off
    if (MApi_XC_MLoad_GetStatus_U2(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK1F_10_L, u16OnOff & 0x0200, 0x0200);
        MDrv_XC_MLoad_Fire(pInstance,TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK1F_10_L, u16OnOff & 0x0200, 0x0200);
    }

    return TRUE;
}

MS_BOOL HAL_SC_Get_DynamicScaling_Status(void)
{

    return bDynamicScalingEnable;
}

void HAL_SC_Set_FB_Num(void *pInstance,SCALER_WIN eWindow, XC_FRAME_STORE_NUMBER enFBType, MS_BOOL bInterlace)
{
    MS_U8 u8FrameCount = 2;

    switch( enFBType )
    {
        case IMAGE_STORE_2_FRAMES:
            u8FrameCount = 2;
            break;
        case IMAGE_STORE_3_FRAMES:
            u8FrameCount = 3;
            break;
        case IMAGE_STORE_4_FRAMES:
            u8FrameCount = 4;
            break;
        case IMAGE_STORE_6_FRAMES:
            u8FrameCount = 6;
            break;
        case IMAGE_STORE_8_FRAMES:
            u8FrameCount = 8;
            break;
        case IMAGE_STORE_12_FRAMES:
            u8FrameCount = 12;
            break;
        default:
            break;
    }

    if (!bInterlace)
    {
        if (eWindow == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_19_L, u8FrameCount , BITMASK(4:0) );
        }
        else
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_59_L, u8FrameCount , BITMASK(4:0) );
        }
    }
}


void HAL_SC_Enable_VInitFactor(void *pInstance,MS_BOOL bEnable,SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(0,REG_SC_BK23_13_L, (MS_U16)(bEnable?BIT(0):0), BIT(0));
    else
        SC_W2BYTEMSK(0,REG_SC_BK23_33_L, (MS_U16)(bEnable?BIT(0):0), BIT(0));
}

void HAL_SC_Set_VInitFactorOne(void *pInstance,MS_U32 u32Value,SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
        SC_W4BYTE(0,REG_SC_BK23_03_L, u32Value);
    else
        SC_W4BYTE(0,REG_SC_BK23_23_L, u32Value);
}

void HAL_SC_Set_VInitFactorTwo(void *pInstance,MS_U32 u32Value,SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
        SC_W4BYTE(0,REG_SC_BK23_05_L, u32Value);
    else
        SC_W4BYTE(0,REG_SC_BK23_25_L, u32Value);
}

void HAL_SC_Set_vsd_output_line_count(void *pInstance,MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_WIN eWindow)
{
    MS_U16 u16OutputLineCount = 0x00;

    if (bEnable)
    {
        u16OutputLineCount = BIT(12);
        u16OutputLineCount |= (MS_U16)(u32LineCount & 0xFFF);
    }

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(0,REG_SC_BK02_37_L, u16OutputLineCount);
    }
    else
    {
        SC_W2BYTE(0,REG_SC_BK04_37_L, u16OutputLineCount);
    }

}

/*
 bEnable : Enable input line count.
 bUserMode : 1 -> the input line count will ref u32UserLineCount
                   0 -> the input line count will ref V capture win
*/
void HAL_SC_Set_vsd_input_line_count(void *pInstance,MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_WIN eWindow)
{
    MS_U16 u16InputLineCount = 0x00;

    if (bEnable)
    {
        u16InputLineCount = BIT(15);

        if (bUserMode)
        {
            u16InputLineCount |= BIT(14);
            u16InputLineCount |= (MS_U16)(u32UserLineCount & 0x1FFF);
        }
    }

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(0,REG_SC_BK02_36_L, u16InputLineCount);
    }
    else
    {
        SC_W2BYTE(0,REG_SC_BK04_36_L, u16InputLineCount);
    }

}

MS_U32 Hal_SC_CheckSubWinPreScaling(MS_U16 u16ScaleDst,MS_BOOL bInterlace)
{
    UNUSED(u16ScaleDst);
    UNUSED(bInterlace);
    //New chip sub window unnecessary to the same with the main window's pre scaling setting.
    return 0;
}

MS_U16  g_u16TmpValue = 0;
MS_BOOL g_bCntFlg = 1;
void HAL_SC_FilmMode_Patch1(void *pInstance)
{
#if 1
    MS_U16 u16Value;
    MS_U16 u16RegValue;

    static MS_U8 u8Time_count = 0;

    u16RegValue = SC_R2BYTEMSK(0,REG_SC_BK22_7D_L, 0xFF);
    if(g_bCntFlg)
        g_u16TmpValue = SC_R2BYTEMSK(0,REG_SC_BK0A_02_L, 0xFF);

    if(SC_R2BYTEMSK(0,REG_SC_BK0A_21_L, BIT(6)))
    {
        //Counter
        if(u8Time_count != 0xFF)
            u8Time_count ++;

        if(u8Time_count < u16RegValue)  // fake out duration
           u16Value = 0x06;      // madi fake out enable
        else
           u16Value = 0x00;      // madi fake out disable

        SC_W2BYTEMSK(0,REG_SC_BK0A_25_L, (0x10<<8), 0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK0A_02_L, 0x22, 0xFF);
        g_bCntFlg = 0;
    }
    else
    {
        u8Time_count = 0;
        u16Value = 0x00;      // madi fake out disable
        SC_W2BYTEMSK(0,REG_SC_BK0A_25_L, (0x08<<8), 0xFF00);
        SC_W2BYTEMSK(0,REG_SC_BK0A_02_L, g_u16TmpValue, 0xFF);
        g_bCntFlg = 1;
    }
    SC_W2BYTEMSK(0,REG_SC_BK0A_21_L, u16Value, BIT(2)|BIT(1));

#else
    MS_U8 u8Bank, u8Value;
    MS_U8 u8RegValue;

    static MS_U8 u8Time_count = 0;

    MDrv_WriteByte(BK_SELECT_00, REG_BANK_MADI); //bk22
    u8RegValue = MDrv_ReadByte(L_BK_MADI(0x7D));

    MDrv_WriteByte(BK_SELECT_00, REG_BANK_FILM);


    if(MDrv_ReadRegBit(L_BK_FILM(0x21), BIT(6)))
    {
        //Counter
        if(u8Time_count != 0xFF)
            u8Time_count ++;

        if(u8Time_count < u8RegValue)  // fake out duration
           u8Value = 0x06;      // madi fake out enable
        else
           u8Value = 0x00;      // madi fake out disable

        MDrv_WriteByte(H_BK_FILM(0x25), 0x10);

    }
    else
    {
        u8Time_count = 0;
        u8Value = 0x00;      // madi fake out disable
        MDrv_WriteByte(H_BK_FILM(0x25), 0x08);
    }

    MDrv_WriteByteMask(L_BK_FILM(0x21), u8Value, BIT(2)|BIT(1));
    MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
}

void HAL_SC_EnableFPLL(void)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    MDrv_WriteByteMask(L_BK_LPLL(0x0C), BIT(3), BIT(3));
    MDrv_WriteByteMask(L_BK_LPLL(0x0C),      0, BIT(6));
    LPLL_BK_RESTORE;
}

static void _HAL_SC_GetFPLLPhaseDiffISR(SC_INT_SRC eIntNum, void *pParam)
{
    volatile MS_XC_GET_FPLL_PHASEDIFF * pFpllPhaseDiff = (volatile MS_XC_GET_FPLL_PHASEDIFF *) pParam;

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    pFpllPhaseDiff->u16PhaseDiff = MDrv_Read2Byte(L_BK_LPLL(0x11));
    pFpllPhaseDiff->eFpllDir = (MDrv_Read2Byte(L_BK_LPLL(0x12)) & BIT(0)) == BIT(0) ? E_XC_FPLL_DIR_UP : E_XC_FPLL_DIR_DOWN;
    pFpllPhaseDiff->u8Debounce++;
    LPLL_BK_RESTORE;
    if(pFpllPhaseDiff->u8Debounce > 3)
    {
        if(pFpllPhaseDiff->u16PhaseDiff < 0x200)
        {
            pFpllPhaseDiff->eFpllResult = E_XC_FPLL_RES_FINISHED;
        }
        else
        {
            if(pFpllPhaseDiff->u8Debounce > 90)
            {
                pFpllPhaseDiff->eFpllResult = E_XC_FPLL_RES_TIMEOUT;
            }
        }
    }
}

MS_BOOL HAL_SC_WaitFPLLDone(void *pInstance)
{
    MS_XC_GET_FPLL_PHASEDIFF stGetFpllPhaseDiff = {0, 0, E_XC_FPLL_DIR_UNKNOWN, E_XC_FPLL_RES_WAITING};

    MDrv_XC_InterruptAttach(pInstance,SC_INT_VSINT, _HAL_SC_GetFPLLPhaseDiffISR, (void *) &stGetFpllPhaseDiff);
    while(*(volatile MS_BOOL*)(&(stGetFpllPhaseDiff.eFpllResult)) == E_XC_FPLL_RES_WAITING);
    MDrv_XC_InterruptDeAttach(pInstance,SC_INT_VSINT, _HAL_SC_GetFPLLPhaseDiffISR, (void *) &stGetFpllPhaseDiff);

    if(stGetFpllPhaseDiff.eFpllResult == E_XC_FPLL_RES_FINISHED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U16 HAL_SC_GetOutputVFreqX100(MS_U32 u32XTAL_Clock)
{

    MS_U16 u16Value=0;
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u16Value =(MS_U16)((u32XTAL_Clock * 100) / MDrv_Read4Byte(L_BK_LPLL(0x23)));
    LPLL_BK_RESTORE;
    return u16Value;
}

//Line buffer offset between Main and Sub, MAX(Main+Sub) = 1920+960
//Main Win Line buffer offset , start from max size of main's
void HAL_SC_SetMainLineBufferOffset(void *pInstance,MS_U16 u16Linebuffer)
{
    SC_W2BYTE(0,REG_SC_BK20_12_L, u16Linebuffer);
}

//Line buffer offset between Main and Sub, MAX(Main+Sub) = 1920+960
//Sub Win Line buffer offset , start from max size of main's
void HAL_SC_SetSubLineBufferOffset(void *pInstance,MS_U16 u16Linebuffer)
{
    SC_W2BYTE(0,REG_SC_BK20_13_L, u16Linebuffer);
}

//Select the trig mode
//0: Line base(Line Buffer Mode)
//1: Fill line buffer(Ring Buffer Mode)
void HAL_SC_SetDisplay_LineBuffer_Mode(void *pInstance,MS_BOOL bEnable)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_1F_L, (bEnable ? BIT(4) : 0), BIT(4));
}

//Select the start mode
//0: start at advance 1 display line
//1: start at faling edge of Vsync_init
void HAL_SC_SetDisplay_Start_Mode(void *pInstance,MS_BOOL bEnable)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_1F_L, (bEnable ? BIT(5) : 0), BIT(5));
}

//bk20_10[10]: Fill the main window's line buffer in vertical blanking for pip left and up corner
//bk20_10[11]: Fill the sub window's line buffer in vertical blanking for pip left and down corner
void HAL_SC_FillLineBuffer(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(bEnable)
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(0,REG_SC_BK20_10_L, BIT(10), BIT(10));
        else
            SC_W2BYTEMSK(0,REG_SC_BK20_10_L, BIT(11), BIT(11));
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(0,REG_SC_BK20_10_L, 0, BIT(10));
        else
            SC_W2BYTEMSK(0,REG_SC_BK20_10_L, 0, BIT(11));
    }
}

void Hal_SC_enable_window(void *pInstance,MS_BOOL bEn,SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK20_10_L, bEn ? BIT(0) : 0, BIT(0));
    }
    else
    {
        // Sub prefill line must be enable, when PIP on (enable input source of sub win)
        SC_W2BYTEMSK(0,REG_SC_BK20_10_L, BIT(11) , BIT(11));
        SC_W2BYTEMSK(0,REG_SC_BK20_10_L, bEn ? BIT(1) : 0, BIT(1));
    }
}

void Hal_SC_enable_window_burst(void *pInstance,MS_BOOL bEn,SCALER_WIN eWindow)
{
    _MLOAD_ENTRY(pInstance);
    if (eWindow == MAIN_WINDOW)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_10_L, bEn ? BIT(0) : 0, BIT(0));
        MDrv_XC_MLoad_Fire(pInstance,TRUE);
    }
    else
    {
        if(bEn == TRUE)
        {
            MS_U16 u16extra_req;
            // When Main is foreground, sub is background, extra must enable.
            u16extra_req = 0x8000 | 0x01;
            // HW PIP architeucture
            // Becasue BK3_02[8] and BK20_11[15] should enable together, otherwise garbage will be showed,
            // we need to use BK12_47[0] to instead. And the default value of BK3_02[8] is 1. So, we need
            // to set it to 0 in this function.
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_11_L, u16extra_req , 0x800F);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_02_L, 0, 0x0080);

            // Sub prefill line must be enable, when PIP on (enable input source of sub win)
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_10_L, BIT(11), BIT(11) );
            MDrv_XC_MLoad_Fire(pInstance,TRUE);
            //if we let REG_SC_BK20_10_L[1] to enable with other register settings in the same menuload fire
            //it will produce a rectangular horizontal noise. so we move outside to let those register settings
            //to be set first then let REG_SC_BK20_10_L[1] on.
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_10_L, BIT(1), BIT(1));
            MDrv_XC_MLoad_Fire(pInstance,TRUE);
        }
        else
        {
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK20_10_L, 0, BIT(1));
            MDrv_XC_MLoad_Fire(pInstance,TRUE);
            MDrv_XC_MLoad_WriteCmd(pInstance,REG_SC_BK03_02_L, 0x0080, 0x0080);
            MDrv_XC_MLoad_Fire(pInstance,TRUE);
        }
    }
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_trigger_signal(void)
{
    // only used in U4
}

void Hal_SC_Set_OSD2VE(void *pInstance,EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    UNUSED(eVOPSelOSD_MUX);
}

MS_BOOL Hal_SC_IsOPMFetchPatch_Enable(void)
{
    //Patch for Janus u01 and T4 U01/02: OPMFetch need +2
    return FALSE;
}
MS_BOOL Hal_SC_Check_HNonLinearScaling(void *pInstance)
{
    MS_U16 u16Offset = (SC_R2BYTE(0,REG_SC_BK23_15_L)>>8);
    MS_U16 u16MaxOffset = ((SC_R4BYTE(0,REG_SC_BK23_07_L) & 0xFFFFFF)>>12);
    //Non-Linear scaling Postive offset, the maximum offset is HPstRatio[23:12]
    if(((u16Offset & BIT(7)) == 0) && (u16Offset > u16MaxOffset))
    {
        return FALSE;
    }
    return TRUE;
}

//IP2 Pre-Filter enable
void Hal_SC_IP2_PreFilter_Enable(void *pInstance,MS_BOOL bEnable)
{
    SC_W2BYTEMSK(0,REG_SC_BK02_48_L, (bEnable?BIT(15):0), BIT(15));
}

void Hal_SC_Set_extra_fetch_line(void *pInstance,MS_U8 u8val)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_17_L, ((MS_U16)(u8val & 0x0F))<<8, 0x0F00);
}

void Hal_SC_Set_extra_adv_line(void *pInstance,MS_U8 u8val)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_17_L, ((MS_U16)(u8val & 0x0F))<<12, 0xF000);
}

void HAL_SC_Set_FPLL_Limit(void *pInstance,MS_U32 *u32PllSet, MS_U32 u32LowBound, MS_U32 u32UpBound, MS_BOOL _bInFPLLDbgMode, MS_U32 _U32LimitD5D6D7)
{
    MS_U32 u32LpllLimitLow, u32LpllLimitHigh;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //printf("Fpll_Limit: Set %lx, bound (%lx -> %lx)\n", *u32PllSet, u32UpBound, u32LowBound);
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    if(_bInFPLLDbgMode)
    {
        MDrv_WriteByteMask(L_BK_LPLL(0x0D), 0x00, BIT(4));          // turn off 2 limit

        //if(!pXCResourcePrivate->stdrvXC_MVideo.g_pSContext->bEnableFPLLManualDebug)
        if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
        {
            MDrv_Write3Byte(L_BK_LPLL(0x06), _U32LimitD5D6D7);
            MDrv_Write3Byte(L_BK_LPLL(0x08), _U32LimitD5D6D7);
        }
    }
    else
    {
        MDrv_WriteByteMask(L_BK_LPLL(0x0D), BIT(4), BIT(4));        // turn on 2 limit

        // check if u32PllSet out of range and also the FPLL limit
        if (*u32PllSet <= u32LowBound)
        {
            *u32PllSet = u32LowBound;
            gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
        }
        else if (*u32PllSet >= u32UpBound)
        {
            *u32PllSet = u32UpBound;
            gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
        }

        u32LpllLimitHigh = u32UpBound - *u32PllSet;
        u32LpllLimitLow = *u32PllSet - u32LowBound - 1;

        //printf("Fpll set %lx, limit (high %lx, low %lx)\n", *u32PllSet, u32LpllLimitHigh, u32LpllLimitLow);

        //if(!pXCResourcePrivate->stdrvXC_MVideo.g_pSContext->bEnableFPLLManualDebug)
        if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
        {
            MDrv_Write3Byte(L_BK_LPLL(0x06), u32LpllLimitHigh);
            MDrv_Write3Byte(L_BK_LPLL(0x08), u32LpllLimitLow);
        }

        //printf("Fpll limit reg (high %lx, low %lx)\n", MDrv_Read4Byte(L_BK_LPLL(0x06)), MDrv_Read4Byte(L_BK_LPLL(0x08)));
    }
    LPLL_BK_RESTORE;
}

void Hal_XC_H3D_Enable(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_70_L, (bEn<<15), (BIT(15)));
}

void Hal_XC_H3D_Input3DType(void *pInstance,E_XC_3D_INPUT_MODE e3DType)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_7B_L, (e3DType), (BIT(1) | BIT(0)));
}

void Hal_XC_H3D_Breakline_Enable(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_7B_L, (bEn<<7), (BIT(7)));
    if(bEn)
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_7B_L, (8<<8), HBMASK);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK01_7B_L, (0<<8), HBMASK);
    }
}

void Hal_XC_H3D_HDE(void *pInstance,MS_U16 u16Hde)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_71_L, (u16Hde), 0x0FFF);
}

void Hal_XC_H3D_VDE_F0(void *pInstance,MS_U16 u16Vde)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_73_L, (u16Vde), 0x0FFF);
}

void Hal_XC_H3D_VDE_F2(void *pInstance,MS_U16 u16Vde)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_77_L, (u16Vde), 0x0FFF);
}

void Hal_XC_H3D_HBLANK(void *pInstance,MS_U16 u16Hblank)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_70_L, (u16Hblank), 0x03FF);
}

void Hal_XC_H3D_INIT_VBLANK(void *pInstance,MS_U8 u8Vblank)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_72_L, (u8Vblank), LBMASK);
}

void Hal_XC_H3D_VBLANK0(void *pInstance,MS_U8 u8Vblank)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_74_L, (u8Vblank), LBMASK);
}

void Hal_XC_H3D_VBLANK1(void *pInstance,MS_U8 u8Vblank)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_76_L, (u8Vblank), LBMASK);
}

void Hal_XC_H3D_VBLANK2(void *pInstance,MS_U8 u8Vblank)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_78_L, (u8Vblank), LBMASK);
}

void Hal_XC_H3D_VSYNC_WIDTH(void *pInstance,MS_U8 u8Width)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_7A_L, (u8Width), 0x001F);
}

void Hal_XC_H3D_VSYNC_POSITION(void *pInstance,MS_U16 u16Position)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_7A_L, (u16Position), HBMASK);
}

void Hal_XC_H3D_SELECT_REGEN_TIMING(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK01_7B_L, (bEn<<4), (BIT(4)));
}

void Hal_XC_H3D_LR_Toggle_Enable(void *pInstance,MS_BOOL bEn, MS_BOOL b2DTo3D, MS_BOOL bSkipDefaultLRFlag)
{
    if(!bSkipDefaultLRFlag)
    {
        if(bEn)
        {
            //Enanble I2S_IN_SD to send out lr flag
            MDrv_WriteByteMask(0x101E24, 0x00, BIT(4)|BIT(5)|BIT(6));
            MDrv_WriteByteMask(0x101EC9, 0x00, BIT(0)|BIT(1));
            MDrv_WriteByteMask(0x101EDC, 0x00, BIT(2));
            MDrv_WriteByteMask(0x101EB3, 0xC0, 0xC0);
        }
        else
        {
            MDrv_WriteByteMask(0x101EB3, 0x00, 0xC0);
        }
    }

    if(b2DTo3D)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_33_L, (bEn<<15), (BIT(15))); //enable op active 2d to 3d
        SC_W2BYTEMSK(0,REG_SC_BK63_57_L, (bEn<<1), (BIT(1))); // Nugget U02 ECO item: 2D->3D
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_33_L, (bEn<<1), (BIT(1)));
    }
}

void HAL_XC_H3D_OPM_SBYS_PIP_Enable(void *pInstance,MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK12_34_L, (bEn<<7), (BIT(7)));
}

void Hal_SC_enable_cursor_report(void *pInstance,MS_BOOL bEn)
{
    if(bEn)
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, BIT(8) | BIT(10), BIT(8) | BIT(10));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, 0x00 , BIT(8) | BIT(10));
    }
}

MS_BOOL Hal_SC_get_pixel_rgb(void *pInstance,XC_Get_Pixel_RGB *pData)
{
    MS_BOOL bret = TRUE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));


    switch(pData->enStage)
    {
    case E_XC_GET_PIXEL_STAGE_AFTER_DLC:
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, 0x00 , BMASK(3:0));
        break;
    case E_XC_GET_PIXEL_STAGE_PRE_GAMMA:
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, 0x08, BMASK(3:0));
        break;
    default:
        bret = FALSE;
        break;
    }

    if(bret)
    {
        // Enable
        if(!pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear)
        {
            Hal_SC_enable_cursor_report(pInstance,TRUE);
        }

        // Initial position (x,y)  Set point you want to read.
        SC_W2BYTE(0,REG_SC_BK0F_19_L, pData->u16x);
        SC_W2BYTE(0,REG_SC_BK0F_1A_L, pData->u16y);

        MsOS_DelayTask(32);  // Must wait .

        pData->u32r = SC_R2BYTE(0,REG_SC_BK0F_2D_L);
        pData->u32g = SC_R2BYTE(0,REG_SC_BK0F_2E_L);
        pData->u32b = SC_R2BYTE(0,REG_SC_BK0F_2F_L);

        // Disable
        if(!pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear)
        {
            Hal_SC_enable_cursor_report(pInstance,FALSE);
        }

    }


    return bret;
}

void Hal_SC_SetOPWriteOff(void *pInstance,MS_BOOL bEna)
{
    MS_BOOL u16OldReg = (MS_BOOL)(SC_R2BYTEMSK(0,REG_SC_BK12_27_L, BIT(0)));
    if(u16OldReg != bEna)
    {
        SC_W2BYTEMSK(0,REG_SC_BK12_27_L, bEna, BIT(0));//This will stop OPW only, OP memory write request still on
    }
}


MS_BOOL Hal_SC_GetOPWriteOff(void *pInstance)
{
    return (MS_BOOL)SC_R2BYTEMSK(0,REG_SC_BK12_27_L, BIT(0));
}

MS_BOOL Hal_SC_Detect_RequestFBL_Mode(void *pInstance)
{
    return ENABLE_REQUEST_FBL;
}

void Hal_XC_SetFrameColor(void *pInstance,MS_U32 u32aRGB)
{
    SC_W2BYTEMSK(0,REG_SC_BK10_19_L,  (u32aRGB >> 8),0xFF00);            ///< R
    SC_W2BYTEMSK(0,REG_SC_BK10_1A_L,  (u32aRGB >> 8),0x00FF);            ///< G
    SC_W2BYTEMSK(0,REG_SC_BK10_1A_L,  (u32aRGB << 8),0xFF00);            ///< B
}
MS_U8 Hal_SC_getVSyncWidth(void *pInstance,SCALER_WIN eWindow)
{
    MS_U8 u8val = 0;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(0,REG_SC_BK01_22_L) & 0xFF00) >> 8);
    }
    else
    {
    }
    return u8val;
}

E_APIXC_ReturnValue Hal_SC_Set_OSDLayer(void *pInstance,E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow)
{
    //do nothing
    return E_APIXC_RET_FAIL;
}

E_VOP_OSD_LAYER_SEL Hal_SC_Get_OSDLayer(void *pInstance,SCALER_WIN eWindow)
{
    //do nothing
    return E_VOP_LAYER_RESERVED;
}

E_APIXC_ReturnValue Hal_SC_Set_VideoAlpha(void *pInstance,MS_U8 u8Val, SCALER_WIN eWindow)
{
    //do nothing
    return E_APIXC_RET_FAIL;
}

E_APIXC_ReturnValue Hal_SC_Get_VideoAlpha(void *pInstance,MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    *pu8Val += 0;

    //do nothing
    return E_APIXC_RET_FAIL;
}

void Hal_XC_ClearScalingFactorForInternalCalib(void *pInstance)
{
    //MDrv_WriteRegBit(H_BK_SCMI(0x09), FALSE, BIT(7));
    SC_W2BYTE(0,REG_SC_BK02_04_L, 0x0000);   // H pre-scaling
    SC_W2BYTE(0,REG_SC_BK02_05_L, 0xC010);   // H pre-scaling
    SC_W2BYTE(0,REG_SC_BK02_08_L, 0x0000);   // V pre-scaling
    SC_W2BYTE(0,REG_SC_BK02_09_L, 0x0000);   // V pre-scaling

    //HVSP
    SC_W2BYTE(0,REG_SC_BK23_07_L, 0x0000);  // H post-scaling
    SC_W2BYTE(0,REG_SC_BK23_08_L, 0x0100);  // H post-scaling
    SC_W2BYTE(0,REG_SC_BK23_09_L, 0x0000);  // V post-scaling
    SC_W2BYTE(0,REG_SC_BK23_0A_L, 0x0100);  // V post-scaling
}


MS_U8 MHal_SC_cal_usedgain_to_reggain(MS_U32 u16UsedGain_x32)
{
    MS_U8 u8Value = 0;

    if      (u16UsedGain_x32 >=65536) u8Value = 16;
    else if (u16UsedGain_x32 >=32768) u8Value = 15;
    else if (u16UsedGain_x32 >=16384) u8Value = 14;
    else if (u16UsedGain_x32 >= 8192) u8Value = 13;
    else if (u16UsedGain_x32 >= 4096) u8Value = 12;
    else if (u16UsedGain_x32 >= 2048) u8Value = 11;
    else if (u16UsedGain_x32 >= 1024) u8Value = 10;
    else if (u16UsedGain_x32 >=  512) u8Value = 9;
    else if (u16UsedGain_x32 >=  256) u8Value = 8;
    else if (u16UsedGain_x32 >=  128) u8Value = 7;
    else if (u16UsedGain_x32 >=   64) u8Value = 6;
    else if (u16UsedGain_x32 >=   32) u8Value = 5;
    else if (u16UsedGain_x32 >=   16) u8Value = 4;
    else if (u16UsedGain_x32 >=    8) u8Value = 3;
    else if (u16UsedGain_x32 >=    4) u8Value = 2;
    else if (u16UsedGain_x32 >=    2) u8Value = 1;
    else if (u16UsedGain_x32 >=    1) u8Value = 0;
    else {
        u8Value = 0;
    }

    u8Value += IPGAIN_REFACTOR;

    return u8Value;
}

E_APIXC_ReturnValue Hal_SC_OP2VOPDESel(void *pInstance,E_OP2VOP_DE_SEL eVopDESel)
{
    //Select op2vop de, for DWIN capture etc
    if(eVopDESel == E_OP2VOPDE_WHOLEFRAME_WITHOSD)
        SC_W2BYTEMSK(0,REG_SC_BK10_23_L, BIT(15), BIT(15));
    else
        SC_W2BYTEMSK(0,REG_SC_BK10_23_L, 0, BIT(15));
    return E_APIXC_RET_OK;
}

MS_BOOL Hal_XC_SVOutput_GetCaps(void *pInstance)
{
    return FALSE;
}

E_APIXC_ReturnValue Hal_XC_IsForcePrescaling(void *pInstance,const XC_InternalStatus *pSrcInfo, MS_BOOL *pbForceV, MS_BOOL *pbForceH, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    *pbForceV = FALSE;
    *pbForceH = FALSE;

    //-----------------------------------------
    // Vertical
    //-----------------------------------------

    //FHD case. FHD has tight bandwidth in FCLK (post scaling), so force pre-scaling if disp.v size > 80% of crop.v size.
    if ( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000) &&
         (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1600) ) //consider 1680x1050 into this case
    {
        // 80%
        if ( ( pSrcInfo->stDispWin.height * 10 ) <  ( pSrcInfo->stCropWin.height * 8 ) )
        {
            *pbForceV = TRUE;
        }
    }
    else // SD, HD panel.
    {
        // 60%
        if ( ( pSrcInfo->stDispWin.height * 10 ) <  ( pSrcInfo->stCropWin.height * 6 ) )
        {
            *pbForceV = TRUE;
        }
    }

    //-----------------------------------------
    // Horizontal
    //-----------------------------------------

    if ( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000)
        && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1600) ) //consider 1680x1050 into this case
    {
        // 80% ( SEC using 80% for HD panel ). Increase % if meet FCLK bandwidth issue in the future.
        if (pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
        {
             if ( ( pSrcInfo->stDispWin.width * 20 ) < ( pSrcInfo->stCropWin.width * 8 ) )
             {
                 *pbForceH = TRUE;
             }
        }
        else if ( ( pSrcInfo->stDispWin.width * 10 ) < ( pSrcInfo->stCropWin.width * 8 ) )
        {
            *pbForceH = TRUE;
        }
    }
    else // SD panel.
    {
        // 60%
        if ( ( pSrcInfo->stDispWin.width * 10 ) < ( pSrcInfo->stCropWin.width * 6 ) )
        {
            *pbForceH = TRUE;
        }
    }

    return E_APIXC_RET_OK;
}

MS_BOOL Hal_SC_IsHW2Dto3DPatch_Enable(void)
{
    MS_BOOL bRet = FALSE;
    if(PM_R1BYTE(REG_CHIP_REVISION, 7:0) < 1)//a1 u01 2d to 3d hw bug
    {
        bRet = TRUE;
    }
    return bRet;
}

MS_U32 MHal_SC_Get_LpllSet_Factor(void *pInstance,MS_U8 u8LPLL_Mode,MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk)
{
    MS_U32 u32Factor = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u32Factor = 3;

    if((u8LPLL_Mode==E_XC_MOD_OUTPUT_DUAL)&&(u32DefaultDClk > 1500000000)&&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1100))
    {
        /// for the WUXGA case
    }

    return u32Factor;
}

MS_U32 MHal_SC_Get_LpllSet_Factor_denominator(void *pInstance,MS_U8 u8LPLL_Mode,MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk)
{
    UNUSED(u8LPLL_Mode);
    UNUSED(u8LPLL_Type);
    UNUSED(u32DefaultDClk);
    MS_U32 u32Denom = 0;

    u32Denom = 2;
    return u32Denom;
}


E_APIXC_ReturnValue Hal_SC_Enable_AVMute(void *pInstance,SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_01_L, BIT(13) | BIT(15), BIT(13) | BIT(15));
        SC_W2BYTEMSK(0,REG_SC_BK10_23_L, BIT(9) | BIT(11), BIT(9) | BIT(11));
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK10_01_L, BIT(12) | BIT(14), BIT(12) | BIT(14));
        SC_W2BYTEMSK(0,REG_SC_BK10_23_L, BIT(8) | BIT(10), BIT(8) | BIT(10));
    }

    return E_APIXC_RET_OK;
}


MS_BOOL Hal_XC_ReportPixelInfo(void *pInstance,MS_XC_REPORT_PIXELINFO *pstRepPixInfo)
{
    MS_BOOL bret = TRUE;

    switch(pstRepPixInfo->enStage)
    {
    case E_XC_GET_PIXEL_STAGE_AFTER_DLC:
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, 0x00 , BMASK(3:0));
        break;
    case E_XC_GET_PIXEL_STAGE_PRE_GAMMA:
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, 0x08, BMASK(3:0));
        break;
    case E_XC_GET_PIXEL_STAGE_AFTER_OSD:
        SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, 0x0C, BMASK(3:0));
        break;
    default:
        bret = FALSE;
        break;
    }

    if(bret)
    {
        //---------------------------------------------------------------
        //read BK0F min/max
        //---------------------------------------------------------------
        // Enable
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, BIT(8) | BIT(9), BIT(8) | BIT(9));

        if(pstRepPixInfo->bShowRepWin)
        {
            SC_W2BYTEMSK(0,REG_SC_BK0F_46_L, pstRepPixInfo->u16RepWinColor<<8, 0xFF00);
            SC_W2BYTEMSK(0,REG_SC_BK10_1A_L, BIT(0), BIT(0));
        }

        // Set Report Window, MLoad replace the RIU setting.
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_42_L, pstRepPixInfo->u16XStart, 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_43_L, pstRepPixInfo->u16XEnd, 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_44_L, pstRepPixInfo->u16YStart, 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_45_L, pstRepPixInfo->u16YEnd, 0xFFFF);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);

        MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW); // Must wait .

        pstRepPixInfo->u16RCrMin = SC_R2BYTE(0,REG_SC_BK0F_64_L);
        pstRepPixInfo->u16RCrMax = SC_R2BYTE(0,REG_SC_BK0F_65_L);
        pstRepPixInfo->u16GYMin  = SC_R2BYTE(0,REG_SC_BK0F_66_L);
        pstRepPixInfo->u16GYMax  = SC_R2BYTE(0,REG_SC_BK0F_67_L);
        pstRepPixInfo->u16BCbMin = SC_R2BYTE(0,REG_SC_BK0F_68_L);
        pstRepPixInfo->u16BCbMax = SC_R2BYTE(0,REG_SC_BK0F_69_L);

        if(pstRepPixInfo->bShowRepWin)
        {
            SC_W2BYTEMSK(0,REG_SC_BK10_1A_L, 0x00, BIT(0));
        }
        // Disable
        SC_W2BYTEMSK(0,REG_SC_BK0F_57_L, 0x00, BIT(8) | BIT(9));

        //---------------------------------------------------------------
        //read BK64 SUM
        //---------------------------------------------------------------
        //BK64 REP WIN window0 report enable
        SC_W2BYTEMSK(0,REG_SC_BK64_01_L, BIT(15), BIT(15));
        if(pstRepPixInfo->bShowRepWin)
        {
            SC_W2BYTEMSK(0,REG_SC_BK64_0F_L, BIT(7), BIT(7));
        }

        if(pstRepPixInfo->u16XStart < REPWINHORIZONTALOFFSET)
        {
            pstRepPixInfo->u16XStart = REPWINHORIZONTALOFFSET;
        }
        if(pstRepPixInfo->u16XEnd < REPWINHORIZONTALOFFSET)
        {
            pstRepPixInfo->u16XEnd = REPWINHORIZONTALOFFSET;
        }
        pstRepPixInfo->u16XStart -=REPWINHORIZONTALOFFSET ;
        pstRepPixInfo->u16XEnd -=REPWINHORIZONTALOFFSET ;
        pstRepPixInfo->u16YStart +=REPWINVERTICALOFFSET ;
        pstRepPixInfo->u16YEnd +=REPWINVERTICALOFFSET ;
        // Set Report Window, MLoad replace the RIU setting.
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK64_01_L, pstRepPixInfo->u16XStart, 0x1FFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK64_02_L, pstRepPixInfo->u16XEnd, 0x1FFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK64_03_L, pstRepPixInfo->u16YStart, 0x1FFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK64_04_L, pstRepPixInfo->u16YEnd, 0x1FFF);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);

        MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW); // Must wait .

        pstRepPixInfo->u32RCrSum = SC_R2BYTE(0,REG_SC_BK64_05_L)|(SC_R2BYTE(0,REG_SC_BK64_06_L)<<16);
        pstRepPixInfo->u32GYSum  = SC_R2BYTE(0,REG_SC_BK64_08_L)|(SC_R2BYTE(0,REG_SC_BK64_09_L)<<16);
        pstRepPixInfo->u32BCbSum = SC_R2BYTE(0,REG_SC_BK64_0B_L)|(SC_R2BYTE(0,REG_SC_BK64_0C_L)<<16);

        if(pstRepPixInfo->bShowRepWin)
        {
            SC_W2BYTEMSK(0,REG_SC_BK64_0F_L, 0x00, BIT(7));
        }
        //BK64 REP WIN window0 report disable
        SC_W2BYTEMSK(0,REG_SC_BK64_01_L, 0x0, BIT(15));
    }

    return bret;
}

/// bandwidth saving Mode
void Hal_SC_set_bws_mode(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //ipm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_01_L, (bEnable? 0x00:BIT(0)), BIT(0));
    }
    else
    {
        //ipm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_01_L, (bEnable? 0x00:BIT(1)), BIT(1));
    }
}

void Hal_SC_sw_lcnt_en(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //ipm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_11_L, (bEnable? BIT(0):0x00), BIT(0));
    }
    else
    {
        //ipm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_21_L, (bEnable? BIT(0):0x00), BIT(0));
    }
}

void Hal_SC_set_sw_lcnt(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //ipm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_11_L, (bEnable? BIT(1):0x00), BIT(1));
    }
    else
    {
        //ipm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_21_L, (bEnable? BIT(1):0x00), BIT(1));
    }
}

void Hal_SC_set_ipmw_lcnt_inv(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //ipm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_12_L, (bEnable? BIT(4):0x00), BIT(4));
    }
    else
    {
        //ipm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_22_L, (bEnable? BIT(4):0x00), BIT(4));
    }
}

void Hal_SC_set_ipmr_lcnt_inv(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //ipm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_12_L, (bEnable? BIT(12):0x00), BIT(12));
    }
    else
    {
        //ipm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_22_L, (bEnable? BIT(12):0x00), BIT(12));
    }
}

void Hal_SC_set_opm_lcnt_inv_pre(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    _HAL_bopm_lcnt_inv[eWindow] = bEnable;
}

void Hal_SC_set_opm_lcnt_inv(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        //opm Main
        SC_W2BYTEMSK(0,REG_SC_BK08_32_L, (bEnable? BIT(5):0x00), BIT(5));
    }
    else
    {
        //opm Sub
        SC_W2BYTEMSK(0,REG_SC_BK08_32_L, (bEnable? BIT(4):0x00), BIT(4));
    }
}

void MHal_XC_Calc_IGainPGain(void *pInstance,MS_U8 *u8GainI, MS_U8 *u8GainP, MS_U32 u32XTAL_Clock, MS_U8 u8LGain, MS_U8 u8Vco, MS_U16 u16HTotal, MS_U16 u16VTotal, MS_U8 u8FRC_Out)
{
    MS_U32 u32Factor, u32Temp;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u32Temp = (MS_U32)(u16VTotal*u16HTotal*u8FRC_Out);
    u32Factor = ((LVDS_MPLL_CLOCK_MHZ*1000000UL/u32XTAL_Clock)*524288*u8LGain)/((MS_U32)(u32Temp*u8Vco/2*8));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MM=%ld,u8LGain1=%d,Htt=%d,Vtt=%d,Ovs=%d,Vco=%d,i_gain=%ld,REGIg=%d\n",
    (LVDS_MPLL_CLOCK_MHZ*1000000UL/pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock),u8LGain,u16HTotal,
    u16VTotal,u8FRC_Out,u8Vco/2,u32Factor,MHal_SC_cal_usedgain_to_reggain(u32Factor));

    *u8GainI = MHal_SC_cal_usedgain_to_reggain(u32Factor);
    *u8GainP = *u8GainI+1;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "==>LG1=%d,iGain=%ld,I=%d,P=%d\n",u8LGain,u32Factor,*u8GainI,*u8GainP);
}

//MS_BOOL MHal_XC_Get_DSForceIndexSupported(SCALER_WIN eWindow)
//{
//    if (eWindow >= MAX_WINDOW)
//    {
//        printf("[%s,%5d] maximum window exceeded",__FUNCTION__,__LINE__);
//        return FALSE;
//    }
//
//    return FALSE;
//}

void MHal_XC_SetForceCurrentReadBank(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow)
{
    static SC_SET_FORCE_CURRENT_READ_BANK_t stSetForceReadBank;
    stSetForceReadBank.pInstance = pInstance;
    stSetForceReadBank.bEnable = bEnable;
    stSetForceReadBank.eWindow = eWindow;

    //color team suggest setting
    //film mode setting
    if(bEnable)
    {
        u16RegForceMryValue = SC_R2BYTEMSK(0, REG_SC_BK22_78_L, 0xffff);
        SC_W2BYTEMSK(0,REG_SC_BK22_78_L, 0x8080, 0xffff);
    }
    else
    {
        SC_W2BYTEMSK(0,REG_SC_BK22_78_L, u16RegForceMryValue, 0xffff);
        u16RegForceMryValue = 0;
    }
    MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_F2_IPVS_SB, MHal_XC_SetForceCurrentReadBankInterrupt, (void*)&stSetForceReadBank);
}



static void MHal_XC_SetForceCurrentReadBankInterrupt(SC_INT_SRC eIntNum, void *pParam)
{
    volatile SC_SET_FORCE_CURRENT_READ_BANK_t *pstSetForceCurrentReadBank = (volatile SC_SET_FORCE_CURRENT_READ_BANK_t *)pParam;

    volatile MS_U16 u16Bank = 0;
    volatile MS_U8 u8Bank = 0;

    if(pstSetForceCurrentReadBank->bEnable)
    {
        if (pstSetForceCurrentReadBank->eWindow == MAIN_WINDOW)
        {
            u16Bank = SC_R2BYTEMSK(0, REG_SC_BK12_3A_L, BIT(7)|BIT(6)|BIT(5)|BIT(4));
        }
        else
        {
            u16Bank = SC_R2BYTEMSK(0, REG_SC_BK12_7A_L, BIT(7)|BIT(6)|BIT(5)|BIT(4));
        }
    }

    u8Bank = (MS_U8)u16Bank >> 4;

    MHal_XC_SetForceReadBank(pstSetForceCurrentReadBank->pInstance, pstSetForceCurrentReadBank->bEnable, u8Bank, pstSetForceCurrentReadBank->eWindow);

    MDrv_XC_InterruptDeAttachWithoutMutex(pstSetForceCurrentReadBank->pInstance, SC_INT_F2_IPVS_SB, MHal_XC_SetForceCurrentReadBankInterrupt, pParam);
}


void MHal_XC_SetForceReadBank(void *pInstance,MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow)
{
    if (eWindow == MAIN_WINDOW)
    {
        if (bEnable)
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_07_L, BIT(11), BIT(11));
            SC_W2BYTEMSK(0,REG_SC_BK12_07_L, (u8Bank & 0x7) << 8, BIT(10)|BIT(9)|BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK12_06_L, ((u8Bank & 0x8) >> 3) << 14, BIT(14));
        }
        else
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_07_L, 0, BIT(11));
            SC_W2BYTEMSK(0,REG_SC_BK12_07_L, 0, BIT(10)|BIT(9)|BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK12_06_L, 0, BIT(14));
        }
    }
    else
    {
        if (bEnable)
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_47_L, BIT(11), BIT(11));
            SC_W2BYTEMSK(0,REG_SC_BK12_47_L, (u8Bank & 0x7) << 8, BIT(10)|BIT(9)|BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK12_46_L, ((u8Bank & 0x8) >> 3) << 14, BIT(14));
        }
        else
        {
            SC_W2BYTEMSK(0,REG_SC_BK12_47_L, 0, BIT(11));
            SC_W2BYTEMSK(0,REG_SC_BK12_47_L, 0, BIT(10)|BIT(9)|BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK12_46_L, 0, BIT(14));
        }
    }
}

MS_BOOL MHal_XC_SetDNRBufAddress(void *pInstance,MS_U32 u32DNRBaseAddr, SCALER_WIN eWindow)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC._u32DNRBaseAddress[eWindow] = u32DNRBaseAddr;
    return TRUE;
}

MS_U32 MHal_XC_GetDNRBufAddress(void *pInstance,SCALER_WIN eWindow)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_SC._u32DNRBaseAddress[eWindow] ;
}

MS_BOOL MHal_XC_SetDNRBufSize(void *pInstance,MS_U32 u32DNRBufSize, SCALER_WIN eWindow)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC._u32DNRBufferSize[eWindow] = u32DNRBufSize;
    return TRUE;
}

MS_U32 MHal_XC_GetDNRBufSize(void *pInstance,SCALER_WIN eWindow)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_SC._u32DNRBufferSize[eWindow] ;
}

MS_BOOL MHal_XC_IsPNLYUVOutput(void *pInstance)
{
    MS_BOOL bIsPNLYUVOutput = FALSE, bIsY2REnable = FALSE, bIsR2YEnable = FALSE;
    SCALER_WIN eWindow = MAIN_WINDOW;
    MS_U16 u16Temp = 0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    //Get CSC state before OP2
    if(gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled || gSrcInfo[MAIN_WINDOW].bBluescreenEnabled)
    {
        //Main is mute, so check sub
        if(Hal_SC_Is_subwindow_enable(pInstance))
        {
            if(!(gSrcInfo[SUB_WINDOW].bBlackscreenEnabled || gSrcInfo[SUB_WINDOW].bBluescreenEnabled))
            {
                eWindow = SUB_WINDOW;
            }
        }
    }

    if(eWindow == MAIN_WINDOW)
    {
        u16Temp = SC_R2BYTE(0, REG_SC_BK10_2F_L);
        if((u16Temp & ( BIT(0) | BIT(2) | BIT(4))) == ( BIT(0) | BIT(2) | BIT(4)))
        {
            bIsY2REnable =  TRUE;
        }
    }
    else
    {
        u16Temp = SC_R2BYTE(0, REG_SC_BK0F_26_L);
        if((u16Temp & ( BIT(0) | BIT(2) | BIT(4))) == ( BIT(0) | BIT(2) | BIT(4)))
        {
            bIsY2REnable =  TRUE;
        }
    }

    if(bIsY2REnable)
    {
        bIsPNLYUVOutput = FALSE;
    }
    else
    {
        //OP Y2R disable, so check op input
        if(eWindow == MAIN_WINDOW)
        {
            bIsR2YEnable = (MS_BOOL)(SC_R2BYTEMSK(0, REG_SC_BK02_40_L, BIT(3)) || SC_R2BYTEMSK(0, REG_SC_BK18_6E_L, BIT(0)));

        }
        else
        {
            bIsR2YEnable = (MS_BOOL)(SC_R2BYTEMSK(0, REG_SC_BK04_40_L, BIT(3)) || SC_R2BYTEMSK(0, REG_SC_BK18_6E_L, BIT(4)));
        }

        if(bIsR2YEnable)
        {
            //Y2R enable
            bIsPNLYUVOutput = TRUE;
        }
        else
        {
            //All Csc is off, so check input
            //VGA/DVI/rgb HDMI is rgb input
            if(  IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType) //VGA
               ||IsSrcTypeDVI(gSrcInfo[eWindow].enInputSourceType) //DVI
               ||(  IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
                  &&(g_HdmiPollingStatus.bIsHDMIMode == TRUE)
                  &&(g_HdmiPollingStatus.u8ColorFormat == MS_HDMI_COLOR_RGB))//HDMI RGB
               ||(  IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
                  &&(g_HdmiPollingStatus.bIsHDMIMode == FALSE)) //HDMI_DVI
              )
            {
                bIsPNLYUVOutput = FALSE;
            }
            else
            {
                bIsPNLYUVOutput = TRUE;
            }
        }
    }
    return bIsPNLYUVOutput;
}

void HAL_SC_EnableLegacyMode(void *pInstance, MS_BOOL bEnable)
{
}
void Hal_SC_3D_SetLRChgMode(void *pInstance, HAL_SC_3D_LRCHGMODE eLRChgMode)
{
    SC_W2BYTEMSK(0, REG_SC_BK02_63_L, eLRChgMode, BIT(1)|BIT(0));
}
void Hal_SC_set_ficlk2(void *pInstance, MS_U8 u8Clk_Mux, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow==MAIN_WINDOW)
    {
        MDrv_WriteByteMask(REG_CKG_FICLK2_F2, (bEnable? 0x00 : CKG_FICLK2_F2_GATED), CKG_FICLK2_F2_GATED);
        if(bEnable)
            MDrv_WriteByteMask(REG_CKG_FICLK2_F2, u8Clk_Mux, CKG_FICLK2_F2_MASK);
    }
    else if(eWindow==SUB_WINDOW)
    {
        printf("[%s,%5d] ficlk2_f2 not support sub!",__FUNCTION__,__LINE__);// no ficlk2_f1 for sub window
    }
}
MS_BOOL Hal_SC_Init(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(psXCInstPri->u32DeviceID > (MAX_XC_DEVICE_NUM - 1))
    {
        printf("u32DeviceID > device number \n");
        return FALSE;
    }

    //enable F2 IPM wbank freeze alginment mode
    SC_W2BYTEMSK(0, REG_SC_BK12_06_L, BIT(12) , BIT(12));
    // freeze IPM enable at av-mute case, disable this function for short garbage when hdmi av-mute case
    SC_W2BYTEMSK(0, REG_SC_BK12_19_L, BIT(5), BIT(5));

    // Enable XC interrupts
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteRegBit(L_BK_VOP(0x25), ENABLE, BIT(7));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_25_L, 0x80, BIT(7));

    // Daten FixMe, move from panel init to here, need to check why
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
    //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(1));  //Using new ovs_ref
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x02, BIT(1));

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    //MDrv_WriteByteMask(L_BK_IP1F2(0x09), 0x02, BIT(3) | BIT(2) | BIT(1) | BIT(0));  //YLock
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x02, BIT(3) | BIT(2) | BIT(1) | BIT(0));
    //Extend the pixel of bottom A session at the right side [7:0]
    //Extend the pixel of bottom B session at the left side[15:8] for PIP issue
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_14_L, 0x2020);

    //HSYNC Tolerance for Mode Change
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, 0x02, 0x1F);

    //if mweEnable not be called, sub win will be open default, we need to black it at first.
    //so that to avoid garbage when opening sub win.
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(5), BIT(5));
    gSrcInfo[SUB_WINDOW].bBlackscreenEnabled = TRUE;

    //enable F2 IPM wbank freeze alginment mode //mantis:0747407
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, BIT(12) , BIT(12));

    return TRUE;
}
void HAL_SC_Set_vsd_3D_autofactor_reset(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    bEnable = bEnable << 6;

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK02_68_L, bEnable, BIT(6));
    }
    else
    {
        //printf("Not support for sub window \n");
    }
}
void Hal_SC_set_fmclk(void *pInstance, MS_BOOL bEnable)
{
}
void MHal_SC_set_manual_rbank_switch_cnt(void *pInstance, MS_BOOL bEnable, MS_U16 u16SwitchCnt, SCALER_WIN eWindow)
{
}
void Hal_SC_3D_SetSplitHalf(void *pInstance, MS_BOOL bEnable)
{
    SC_W2BYTEMSK(0, REG_SC_BK02_63_L, (bEnable==TRUE)?BIT(8):0, BIT(8));
}
MS_U32 Hal_SC_Get_Device_Offset(MS_U8 deviceIdx)
{
    return _HAL_XC_DEVICE_OFFSET[deviceIdx];
}

E_APIXC_ReturnValue Hal_SC_SetOSDDetect(void *pInstance, MS_BOOL bEnable, MS_U32 Threhold)
{
    MS_U16 u16Reg = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u16Reg = ((MS_U16)Threhold)|(bEnable << 15);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_6A_L, u16Reg, (BIT(15)+BMASK(11:0)));
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_GetOSDDetect(void *pInstance, MS_BOOL *pbOSD)
{
    MS_U16 u16Reg = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u16Reg = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_6A_L);
    *pbOSD = ((u16Reg&BIT(14))>>14);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow)
{
    UNUSED(eWindow);
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
}
MS_BOOL Hal_SC_get_ip2_csc(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bYUVInput;
    if(eWindow == MAIN_WINDOW)
    {
        // If it has CSC, it shows it must be RGB input.
        // Because in scaler, it must be YUV. So if input is RGB, then we need to do CSC
        bYUVInput = ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_40_L, BIT(3) ))? FALSE:TRUE;
    }
    else
    {
        bYUVInput = ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_40_L, BIT(3) ))? FALSE:TRUE;
    }
    return bYUVInput;
}
void Hal_SC_get_miusel(void *pInstance, SC_MIUSEL_t* stMIUSel)
{
    if(SC_R2BYTEMSK(0, REG_SC_BK12_05_L, BIT(4)))
    {
        stMIUSel-> u8MainFBSel = 1;
    }
    else
    {
        if(SC_R2BYTEMSK(0, REG_SC_BK11_05_L, BIT(4)))
        {
            stMIUSel-> u8MainFBSel = 2;
        }
        else
        {
            stMIUSel-> u8MainFBSel = 0;
        }
    }
}
MS_BOOL Hal_SC_3D_Is_LR_Sbs2Line(void *pInstance)
{
    if(SC_R2BYTEMSK(0, REG_SC_BK23_53_L, BIT(15)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void Hal_SC_3D_SetInitialLRIndex(void *pInstance, MS_BOOL bRFirst)
{
    SC_W2BYTEMSK(0, REG_SC_BK02_63_L, (bRFirst==TRUE)?BIT(4):0, BIT(4));
}

void MHal_SC_OP1_Pattern_backup_setting(void)
{
    u16OP1_fclk_gate_en = SC_R2BYTEMSK(0,REG_SC_BK00_03_L,0xFF);
    u16OP1_reg_ip2_clk_gate_en = SC_R2BYTEMSK(0,REG_SC_BK01_2E_L,0xFF);
    u32OP1_reg_pre_down_f2 = SC_R4BYTE(0,REG_SC_BK02_08_L);
    u32OP1_reg_pre_down_f1 = SC_R4BYTE(0,REG_SC_BK04_08_L);
    u16OP1_ficlk_status = R2BYTEMSK(REG_CLKGEN0_51_L, 0x0C00);

    // all color function need to disable
    u16OP1_color_3x3 = SC_R2BYTEMSK(0,REG_SC_BK10_2F_L,0xFFFF);
    u16OP1_color_vip = SC_R2BYTEMSK(0,REG_SC_BK27_70_L,0xFF);
    u16OP1_reg_di1lb_en = SC_R2BYTEMSK(0,REG_SC_BK20_18_L,0xFFFF);
    u16OP1_bypass_miu = SC_R2BYTEMSK(0,REG_SC_BK20_08_L,BIT(12));
    u16OP1_vop_dualpixel = SC_R2BYTEMSK(0,REG_SC_BK10_21_L,BIT(8));
}

void MHal_SC_OP1_Pattern_enable(MS_BOOL bEnable)
{
    //enable the op1 test pattern
    SC_W2BYTEMSK(0,REG_SC_BK20_71_L, (bEnable << 1), BIT(1));
}

void MHal_SC_OP1_Pattern_pure_color(MS_U8 u8Color)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_71_L, u8Color, 0xF0);
}

void MHal_SC_OP1_Pattern_set_sram(MS_U16 u16Tbl0, MS_U16 u16Tbl1, MS_U16 u16Tbl2, MS_U16 u16Tbl3)
{
    //Entry_0 needs to set the hTile size and vTile size
    SC_W2BYTEMSK(0,REG_SC_BK20_77_L, u16Tbl0,0xFFFF);
    SC_W2BYTEMSK(0,REG_SC_BK20_78_L, u16Tbl1,0xFFFF);
    SC_W2BYTEMSK(0,REG_SC_BK20_79_L, u16Tbl2,0xFFFF);
    SC_W2BYTEMSK(0,REG_SC_BK20_7A_L, u16Tbl3,0xFFFF);
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, BIT(2),BIT(2)); //fast_mode = 1, write pulse, write_en = 1
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, BIT(6),BIT(6));//write_flag_clr pulse, fast_mode = 1, write_en = 1
}

void MHal_SC_OP1_Pattern_define_tile(MS_U16 hTile,MS_U16 vTile)
{
    //Entry_0 needs to set the hTile size and vTile size
    MHal_SC_OP1_Pattern_set_sram(hTile, vTile, 0, 0);
}

void MHal_SC_OP1_Pattern_enable_border(MS_BOOL bEnable,MS_U16 RVal, MS_U16 GVal, MS_U16 BVal)
{
    if(bEnable == TRUE)
    {
        SC_W2BYTEMSK(0,REG_SC_BK20_6A_L, BIT(15) ,BIT(15));
        SC_W2BYTEMSK(0,REG_SC_BK20_6B_L, RVal,0x3FF);
        SC_W2BYTEMSK(0,REG_SC_BK20_6C_L, GVal,0x3FF);
        SC_W2BYTEMSK(0,REG_SC_BK20_6D_L, BVal,0x3FF);

    }else
    {
        SC_W2BYTEMSK(0,REG_SC_BK20_6A_L,0x00,BIT(15));
    }
}

void MHal_SC_OP1_Pattern_customize_blcok(MS_U8 block_Size,MS_U8 repeat_Num,MS_BOOL isLineStart,MS_BOOL isLineEnd,MS_U16 RVal, MS_U16 GVal, MS_U16 BVal, E_XC_OP1_PATTERN_DTAT_TYPE eType)
{
    MS_U16 u16SramTlb_0,u16SramTlb_1,u16SramTlb_2,u16SramTlb_3;
    u16SramTlb_0 = u16SramTlb_1 = u16SramTlb_2 = u16SramTlb_3 = 0 ;

    u16SramTlb_0 |= (BVal & 0x03FF);
    u16SramTlb_0 |= ((GVal & 0x003F) << 10);
    u16SramTlb_1 |= (((GVal & 0x03FF)>>6) & 0x000F);
    u16SramTlb_1 |= ((RVal & 0x03FF) << 4);
    u16SramTlb_2 |= ((block_Size <<2)&(0x3F <<2)); // block size ;
    u16SramTlb_3 |= ((repeat_Num << 4)&(0x3F<<4));

    if(isLineStart)
    {
        u16SramTlb_3 |= (BIT(11));
    }
    if(isLineEnd)
    {
        u16SramTlb_3 |= (BIT(10));
    }

    switch(eType)
    {
        case E_XC_OP1_PATTERN_TYPE_FADE:
            {
                u16SramTlb_2 |= BIT(8);
            }
            break;
        case E_XC_OP1_PATTERN_TYPE_LINEDOT:
            {
                u16SramTlb_2 |= BIT(9);
            }
            break;
        case E_XC_OP1_PATTERN_TYPE_CALIBRATION:
            {
                u16SramTlb_2 |= (BIT(8)|BIT(9));
            }
            break;
        case E_XC_OP1_PATTERN_TYPE_DOTFADE:
            {
                u16SramTlb_1 |= BIT(14);
            }
            break;
        default:
            break;
    }
    MHal_SC_OP1_Pattern_set_sram(u16SramTlb_0, u16SramTlb_1, u16SramTlb_2, u16SramTlb_3);
}

void MHal_SC_OP1_Pattern_set_border_sram(XC_OP1_PatGen_Border tile1, XC_OP1_PatGen_Border tile2, XC_OP1_PatGen_Border tile3, XC_OP1_PatGen_Border tile4, XC_OP1_PatGen_Border tile5)

{
    MS_U16 u16SramTlb_0,u16SramTlb_1,u16SramTlb_2,u16SramTlb_3;
    u16SramTlb_0 = u16SramTlb_1 = u16SramTlb_2 = u16SramTlb_3 = 0 ;

    //tile1 's border
    if(tile1.bRight==TRUE){
        u16SramTlb_0 |= (BIT(2)); //enable border
        u16SramTlb_0 |= ((tile1.u8RightBold) & 0x03); //set the border's bold
    }

    if(tile1.bLeft==TRUE){
        u16SramTlb_0 |= (BIT(5));
        u16SramTlb_0 |= (((tile1.u8LeftBold) & 0x03)<<3);
    }

    if(tile1.bBottom==TRUE){
        u16SramTlb_0 |= (BIT(8));
        u16SramTlb_0 |= (((tile1.u8BottomBold) & 0x03)<<6);
    }

    if(tile1.bTop==TRUE){
        u16SramTlb_0 |= (BIT(11));
        u16SramTlb_0 |= (((tile1.u8TopBold) & 0x03)<<9);
    }

    //tile2 's border
    if(tile2.bRight==TRUE){
        u16SramTlb_0 |= (BIT(14));
        u16SramTlb_0 |= (((tile2.u8RightBold) & 0x03)<<12);
    }

    if(tile2.bLeft==TRUE){
        u16SramTlb_0 |= (((tile2.u8LeftBold) & BIT(0))<<15);
        u16SramTlb_1 |= (((tile2.u8LeftBold) & BIT(1)) >> 1);
        u16SramTlb_1 |= (BIT(1));
    }

    if(tile2.bBottom==TRUE){
        u16SramTlb_1 |= (BIT(4));
        u16SramTlb_1 |= (((tile2.u8BottomBold) & 0x03)<<2);
    }

    if(tile2.bTop==TRUE){
        u16SramTlb_1 |= (BIT(7));
        u16SramTlb_1 |= (((tile2.u8TopBold) & 0x03)<<5);
    }

    //tile3 's border
    if(tile3.bRight==TRUE){
        u16SramTlb_1 |= (BIT(10));
        u16SramTlb_1 |= (((tile3.u8RightBold) & 0x03)<<8);
    }

    if(tile3.bLeft==TRUE){
        u16SramTlb_1 |= (BIT(13));
        u16SramTlb_1 |= (((tile3.u8LeftBold) & 0x03)<<11);
    }

    if(tile3.bBottom==TRUE){
        u16SramTlb_2 |= (BIT(0));
        u16SramTlb_1 |= (((tile3.u8BottomBold) & 0x03)<<14);
    }

    if(tile3.bTop==TRUE){
        u16SramTlb_2 |= (BIT(3));
        u16SramTlb_2 |= (((tile3.u8TopBold) & 0x03)<<1);
    }

    //tile4 's border
    if(tile4.bRight==TRUE){
        u16SramTlb_2 |= (BIT(6));
        u16SramTlb_2 |= (((tile4.u8RightBold) & 0x03)<<4);
    }

    if(tile4.bLeft==TRUE){
        u16SramTlb_2 |= (BIT(9));
        u16SramTlb_2 |= (((tile4.u8LeftBold) & 0x03)<<7);
    }

    if(tile4.bBottom==TRUE){
        u16SramTlb_2 |= (BIT(12));
        u16SramTlb_2 |= (((tile4.u8BottomBold) & 0x03)<<10);
    }

    if(tile4.bTop==TRUE){
        u16SramTlb_2 |= (BIT(15));
        u16SramTlb_2 |= (((tile4.u8TopBold) & 0x03)<<13);
    }

    //tile5 's border
    if(tile5.bRight==TRUE){
        u16SramTlb_3 |= (BIT(2)); //enable border
        u16SramTlb_3 |= ((tile5.u8RightBold) & 0x03); //set the border's bold
    }

    if(tile5.bLeft==TRUE){
        u16SramTlb_3 |= (BIT(5));
        u16SramTlb_3 |= (((tile5.u8LeftBold) & 0x03)<<3);
    }

    if(tile5.bBottom==TRUE){
        u16SramTlb_3 |= (BIT(8));
        u16SramTlb_3 |= (((tile5.u8BottomBold) & 0x03)<<6);
    }

    if(tile5.bTop==TRUE){
        u16SramTlb_3 |= (BIT(11));
        u16SramTlb_3 |= (((tile5.u8TopBold) & 0x03)<<9);
    }

    //Entry_0 needs to set the hTile size and vTile size
    MHal_SC_OP1_Pattern_set_sram(u16SramTlb_0, u16SramTlb_1, u16SramTlb_2, u16SramTlb_3);
}

void MHal_SC_OP1_Pattern_init_setting(void)
{
    //--------------------------------------------
    // BK_00 SC_reg
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK00_03_L,0x0000,BIT(12)); //[4]fclk_gate_en :disable

    //--------------------------------------------
    // BK_20 reg_force_bypass_miu_en
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK20_08_L,0x0000,BIT(12)); //[12]reg_force_bypass_miu_en

    //--------------------------------------------
    // reg_ckg_ficlk_f2 : fixing the SsKeyInput Debug issue.
    //--------------------------------------------
    W2BYTEMSK(REG_CLKGEN0_51_L, CLK_SRC_FCLK<<10, (BIT(11)|BIT(10))); // clk_idclk2

    //--------------------------------------------
    // BK_01 IP1_reg
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK01_2E_L,0x00,0xFF); //[0]reg_ip2_clk_gate_en

    // use the SdVenderVideo_ColorEnhanceBypass to control the PQ by SEC.
#if 1
    //--------------------------------------------
    // BK_10 sc vop reg
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK10_2F_L,0x0000,0xFFFF); //disable 3x3
#endif

    //--------------------------------------------
    // BK_02 ip2_reg_f2
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK02_09_L,0x80,0xFF);//[15] reg_pre_down

    //--------------------------------------------
    // BK_02 ip2_reg_f1
    //--------------------------------------------
    SC_W2BYTEMSK(0,REG_SC_BK04_09_L,0x80,0xFF);//[15] reg_pre_down

// use the VenderVideo_ColorEnhanceBypass to control the PQ by SEC.
#if 1
    //BK27 70 [0] disable color
     SC_W2BYTEMSK(0,REG_SC_BK27_70_L,0x01,0xFF);
#endif

    //bk20 [4]reg_di1lb_en
    SC_W2BYTEMSK(0,REG_SC_BK20_18_L, 0x10,0xFFFF);

    //[10:0]sram jump thr
    SC_W2BYTEMSK(0,REG_SC_BK20_7D_L, 0xFFF,0xFFFF);

    //For no timing trigger setting : need to set the vop to dual pixel output.
    SC_W2BYTEMSK(0,REG_SC_BK10_21_L, 0x100,0xFFFF);

}

void MHal_SC_OP1_Pattern_restore_setting(void)
{
    SC_W2BYTEMSK(0,REG_SC_BK00_03_L,u16OP1_fclk_gate_en,0xFF); //[4]fclk_gate_en :disable
    SC_W2BYTEMSK(0,REG_SC_BK01_2E_L,u16OP1_reg_ip2_clk_gate_en,0xFF); //[0]reg_ip2_clk_gate_en
    W2BYTEMSK(REG_CLKGEN0_51_L, u16OP1_ficlk_status, (BIT(11)|BIT(10))); // clk_idclk2s

    #if 0
        if (u16OP1_reg_pre_downH_f2 == 0xC010 && u16OP1_reg_pre_downL_f2 == 0x0000)
        {
           SC_W2BYTEMSK(REG_SC_BK02_09_L,0,0xFFFF);//[15] reg_pre_down
           SC_W2BYTEMSK(REG_SC_BK02_08_L,0,0xFFFF);//[15] reg_pre_down
        }
        else
        {
            SC_W2BYTEMSK(REG_SC_BK02_09_L, u16OP1_reg_pre_downH_f2,0xFFFF);//[15] reg_pre_down
            SC_W2BYTEMSK(REG_SC_BK02_08_L, u16OP1_reg_pre_downL_f2,0xFFFF);//[15] reg_pre_down
        }
    #endif
    SC_W4BYTE(0,REG_SC_BK02_08_L, u32OP1_reg_pre_down_f2);//[15] reg_pre_down
    SC_W4BYTE(0,REG_SC_BK04_08_L, u32OP1_reg_pre_down_f1);//[15] reg_pre_down

    // use the VenderVideo_ColorEnhanceBypass to control the PQ by SEC.
    //if(!bSkipColorMatrix)
    SC_W2BYTEMSK(0,REG_SC_BK18_6E_L, 0, BIT(0));  //restore post 3X3
#if 1
    SC_W2BYTEMSK(0,REG_SC_BK10_2F_L, u16OP1_color_3x3, 0xFFFF); //disable 3x3
    SC_W2BYTEMSK(0,REG_SC_BK27_70_L, u16OP1_color_vip, 0xFF);
#endif
    SC_W2BYTEMSK(0,REG_SC_BK20_18_L, u16OP1_reg_di1lb_en, BIT(4));

    SC_W2BYTEMSK(0,REG_SC_BK20_08_L, u16OP1_bypass_miu, BIT(12));
    SC_W2BYTEMSK(0,REG_SC_BK10_21_L, u16OP1_vop_dualpixel, BIT(8));

}

void MHal_SC_OP1_Pattern_set_jump_threshold(MS_U16 u16Threshold)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_7D_L, u16Threshold, 0xFFFF);
}

void MHal_SC_OP1_Pattern_set_wraddr(MS_U16 u16Addr)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_7B_L, u16Addr, 0x0FFF);
}

void MHal_SC_OP1_Pattern_sram_write_enable(MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, (bEn) << 1,BIT(1));//write start
}

void MHal_SC_OP1_Pattern_sram_write_fastmode(MS_BOOL bEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, (bEn) << 4,BIT(4));//write start
}

void MHal_SC_OP1_Pattern_sram_writestart(void)
{
    //[10:0]sram base addr
    SC_W2BYTEMSK(0,REG_SC_BK20_7C_L, 0x00,0xFFFF);//[11:0]sram base addr for pattern

    //no timing trigger setting: need to set the SRAM write done bit
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, 0x12,BIT(1)|BIT(3)|BIT(4));//write start
}

void MHal_SC_OP1_Pattern_sram_writedone(void)
{
    //[10:0]sram base addr
    SC_W2BYTEMSK(0,REG_SC_BK20_7C_L, 0x00,0xFFFF);//[11:0]sram base addr for pattern

    //no timing trigger setting: need to set the SRAM write done bit
    SC_W2BYTEMSK(0,REG_SC_BK20_76_L, 0x08,BIT(1)|BIT(3)|BIT(4));//write done
}

MS_BOOL MHal_SC_OP1_Pattern_set_patgen_mode(EN_XC_OP1_PATGEN_MODE mode)
{
    MS_BOOL bRet = TRUE;
    //Set the Pattern gen mode
    //opm mode or disp_lb_mode
    switch (mode)
    {
        case E_XC_OP1_PATGEN_OPM_MODE:
            SC_W2BYTEMSK(0,REG_SC_BK20_71_L,BIT(8),BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK20_71_L,0x00,BIT(9));
            break;
        case E_XC_OP1_PATGEN_DISP_LB_MODE:
            SC_W2BYTEMSK(0,REG_SC_BK20_71_L,0x00,BIT(8));
            SC_W2BYTEMSK(0,REG_SC_BK20_71_L,BIT(9),BIT(9));
            break;

        default:
            printf("input wrong mode !!");
            bRet = FALSE;
            break;
    }

    return bRet;
}

void MHal_SC_OP1_PatGen_adjust_frame_size(MS_U16 u16HSize, MS_U16 u16VSize)
{
    MS_U16 u16PanelWidth, u16PanelHeight;
    MS_U16 u16H_DE_Start, u16H_DE_End, u16V_DE_Start, u16V_DE_End;
    MS_U16 u16H_DisStart, u16H_DisEnd, u16V_DisStart, u16V_DisEnd;
    MS_U16 u16Temp;

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    u16H_DE_Start  = SC_R2BYTEMSK(0,REG_SC_BK10_04_L, 0x0FFF);
    u16H_DE_End    = SC_R2BYTEMSK(0,REG_SC_BK10_05_L, 0x0FFF);
    u16V_DE_Start  = SC_R2BYTEMSK(0,REG_SC_BK10_06_L, 0x0FFF);
    u16V_DE_End    = SC_R2BYTEMSK(0,REG_SC_BK10_07_L, 0x0FFF);

    u16PanelWidth =  pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    u16PanelHeight =  pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

    if ( u16HSize == u16PanelWidth )
    {
        u16H_DisStart = u16H_DE_Start;
        u16H_DisEnd   = u16H_DE_End;
    }
    else if ( u16HSize < u16PanelWidth )
    {
        u16Temp       = (u16PanelWidth - u16HSize) / 2;
        u16H_DisStart = u16H_DE_Start + u16Temp;
        u16H_DisEnd   = u16H_DE_End   - u16Temp;
    }
    else
    {
        u16Temp       = (u16HSize - u16PanelWidth) / 2;
        if ( u16Temp > u16H_DE_Start / 2 )
            u16Temp = u16H_DE_Start / 2;
        u16H_DisStart = u16H_DE_Start - u16Temp;
        u16H_DisEnd   = u16H_DE_End   + u16Temp;
    }

    if ( u16VSize == u16PanelHeight )
    {
        u16V_DisStart = u16V_DE_Start;
        u16V_DisEnd   = u16V_DE_End;
    }
    else if ( u16VSize < u16PanelHeight )
    {
        u16Temp       = (u16PanelHeight - u16VSize) / 2;
        u16V_DisStart = u16V_DE_Start + u16Temp;
        u16V_DisEnd   = u16V_DE_End   - u16Temp;
    }
    else
    {
        u16Temp       = (u16VSize - u16PanelHeight) / 2;
        if ( u16Temp > u16V_DE_Start / 2 )
            u16Temp = u16V_DE_Start / 2;
        u16V_DisStart = u16V_DE_Start - u16Temp;
        u16V_DisEnd   = u16V_DE_End   + u16Temp;
    }

    SC_W2BYTEMSK(0,REG_SC_BK10_08_L, u16H_DisStart, 0x0FFF);
    SC_W2BYTEMSK(0,REG_SC_BK10_09_L, u16H_DisEnd, 0x0FFF);
    SC_W2BYTEMSK(0,REG_SC_BK10_0A_L, u16V_DisStart, 0x0FFF);
    SC_W2BYTEMSK(0,REG_SC_BK10_0B_L, u16V_DisEnd, 0x0FFF);

    MHal_SC_OP1_Pattern_set_frame_size(u16HSize, u16VSize);

}

void MHal_SC_OP1_Pattern_set_frame_size(MS_U16 u16HSize, MS_U16 u16VSize)
{
    //hSize
    SC_W2BYTEMSK(0,REG_SC_BK20_72_L, u16HSize, 0x7FF);
    //vSize
    SC_W2BYTEMSK(0,REG_SC_BK20_73_L, u16VSize, 0x7FF);
}

void MHal_SC_OP1_Pattern_set_dotfade_value(MS_U8 RVal, MS_U8 GVal, MS_U8 BVal)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_68_L, RVal,0xFF);
    SC_W2BYTEMSK(0,REG_SC_BK20_68_L, (GVal << 8),0xFF00);
    SC_W2BYTEMSK(0,REG_SC_BK20_69_L, BVal,0xFF);
}
void MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(MS_BOOL REn, MS_BOOL GEn, MS_BOOL BEn)
{
    SC_W2BYTEMSK(0,REG_SC_BK20_69_L, (REn << 8), BIT(8));
    SC_W2BYTEMSK(0,REG_SC_BK20_69_L, (GEn << 9), BIT(9));
    SC_W2BYTEMSK(0,REG_SC_BK20_69_L, (BEn << 10), BIT(10));
}
void MHAL_SC_set_r2y_en(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_6E_L, bEnable?BIT(0):0x00, BIT(0));
    }
    else if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK18_6E_L, bEnable?BIT(4):0x00, BIT(4));
    }
}

void MHAL_SC_set_VOP(void *pInstance, MS_XC_VOP_Data *pstVOPData)
{
    //Note: H/V Total, H/V DE End, H/V Syncwidth all need minus 1 when set to register
    MS_U16 u16DE_HEND = pstVOPData->u16HStart + pstVOPData->u16Width - 1;
    MS_U16 u16DE_HStart = pstVOPData->u16HStart;
    MS_U16 u16DE_VEND = pstVOPData->u16VStart + pstVOPData->u16Height - 1;
    MS_U16 u16DE_VStart = pstVOPData->u16VStart;
    MS_U16 u16VsyncStart = pstVOPData->u16VTotal - pstVOPData->u16VSyncStart;// vsync start = Vtt - (VSyncWidth + BackPorch)

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(psXCInstPri->u32DeviceID > (MAX_XC_DEVICE_NUM - 1))
    {
        printf("u32DeviceID > device number \n");
        return;
    }
    // Htotal/Vtotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, (pstVOPData->u16HTotal - 1), VOP_HTT_MASK);         // output htotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pstVOPData->u16VTotal - 1), VOP_VTT_MASK);         // output vtotal

    // DE H/V start/size
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16DE_HStart, VOP_DE_HSTART_MASK);          // DE H start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, u16DE_HEND  , VOP_DE_HEND_MASK);            // DE H end
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L, u16DE_VStart, VOP_DE_VSTART_MASK);          // DE V start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, u16DE_VEND  , VOP_DE_VEND_MASK);            // DE V end

    // Display H/V start/size
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, u16DE_HStart , VOP_DISPLAY_HSTART_MASK);                                               // Display H start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, u16DE_HEND   , VOP_DISPLAY_HEND_MASK);  // Display H end
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16DE_VStart , VOP_DISPLAY_VSTART_MASK);                                               // Display V start
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16DE_VEND   , VOP_DISPLAY_VEND_MASK);  // Display V end

    // H/V sync start/width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, (pstVOPData->u8HSyncWidth-1), LBMASK);                    // hsync width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, ((u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1)), 0x17FF);     // vsync start = Vtt - (VSyncWidth + BackPorch)
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, (u16VsyncStart + pstVOPData->u8VSyncWidth - 1), VOP_VSYNC_END_MASK);    // vsync end = Vsync start + Vsync Width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, (pstVOPData->bManuelVSyncCtrl << 15), BIT(15));                                  // manual vsync control
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(14), BIT(14));       //default set to mode1                                                                       // disable auto_htotal
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_23_L, BIT(7), BIT(7));

    // output control
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_21_L, pstVOPData->u16OCTRL);

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_46_L, pstVOPData->u16OSTRL);

    // output driving current
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_47_L, pstVOPData->u16ODRV, LBMASK);
}

void Hal_SC_3D_Adjust_PreHVscaling_SaveBW(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling,MS_BOOL *pb3DPreVScaling,MS_BOOL bForcePreHScalingDown,MS_BOOL bForcePreVScalingDown,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_FRAME_PACKING))
    {
        if(!(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
        {
            *pb3DPreHScaling = TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            if(MDrv_ReadRegBit(REG_CKG_FCLK, CKG_FCLK_MASK) < 24)//fclk < 320MHz may have fclk issue for fi
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = MAX(384, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) * 2 / 3 - 64);
            }
            else
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = MAX(384, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) * 2 / 3);
            }
        }
    }
    else if((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_FRAME_ALTERNATIVE) && (!(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)) &&(pSrcInfo->stCapWin.width > 1280)
            && (MDrv_ReadRegBit(REG_CKG_FCLK, CKG_FCLK_MASK) < 24))//fclk < 320MHz may have fclk issue for fi
    {
        *pb3DPreHScaling = TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = MIN(1280, pSrcInfo->stDispWin.width);
    }
    else
    {
        if(bForcePreHScalingDown)
        {
            *pb3DPreHScaling = FALSE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;
        }

        if(bForcePreVScalingDown)
        {
            *pb3DPreVScaling = FALSE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->u16V_SizeAfterPreScaling;
        }
    }
}

MS_BOOL Hal_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bRet = FALSE;

    if(SUB_WINDOW == eWindow)
    {
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(5)) ? TRUE: FALSE);
    }
    else if(MAIN_WINDOW == eWindow)
    {
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, BIT(1)) ? TRUE: FALSE);
    }
    return bRet;
}

#undef  MHAL_SC_C
