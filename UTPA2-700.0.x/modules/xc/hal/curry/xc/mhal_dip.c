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
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "mvideo_context.h"
#include "mhal_sc.h"
#include "mhal_menuload.h"
#include "halCHIP.h"
#include "drv_sc_menuload.h"
#include "drv_sc_isr.h"
#include "drv_sc_DIP_scaling.h"
#include "mhal_dip.h"
#include "mhal_mux.h"
#include "hwreg_ipmux.h"
#include "drvMIU.h"
#include "mdrv_sc_dynamicscaling.h"
#ifdef UTOPIA_V2
#include "XC_DWIN_private.h"
#include "utopia_dapi.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#define SC_DBG(x)   //x
#define FPLL_DBG(x) //x
#define FHD_WIDTH 1920UL
#ifdef UTOPIA_V2
#define g_DIPSrcInfo            pDipResPri->g_DIPSrcInfo
#endif
#define H_DIP_PostScalingRatio(Input, Output)        (Output!=0)?((MS_U32)( ((MS_U64)(Input)) * 1048576ul / (Output))) : ((MS_U64)(Input)) * 1048576ul
#define V_DIP_PostScalingRatio(Input, Output)        (Output!=0)?((MS_U32)( ((MS_U64)(Input)) * 1048576ul / (Output))) : ((MS_U64)(Input)) * 1048576ul

//TrustZone
#define REG_TZPC_NONPM_BASE      0x123900UL
#define REG_TZPC_NONPM_DIP       (REG_TZPC_NONPM_BASE + (0x74<<1) )
#define REG_TZPC_NONPM_DWIN0     (REG_TZPC_NONPM_BASE + (0x79<<1) )

#define new_chakra
static MS_U8  DipWBufCnt=0,Dwin0WBufCnt=0,Dwin1WBufCnt=0;
static SCALER_DIP_SOURCE_TYPE DipSource = SCALER_DIP_SOURCE_TYPE_OP_SUB;
static SCALER_DIP_SOURCE_TYPE Dwin0Source = SCALER_DIP_SOURCE_TYPE_OP_SUB;
static SCALER_DIP_SOURCE_TYPE Dwin1Source = SCALER_DIP_SOURCE_TYPE_OP_SUB;
static MS_BOOL bPIPmode = FALSE;
static InterruptCb pDipIntCb;
static InterruptCb pDwin0IntCb;
static InterruptCb pDwin1IntCb;
static MS_BOOL bDipIsr = FALSE,bDwin0Isr = FALSE,bDwin1Isr = FALSE,bDIPRotation = FALSE,bAttached = FALSE;
static EN_DRV_XC_DWIN_DATA_FMT DipRFmt;
static MS_U16 DipSrcWidth = 0,Dwin0SrcWidth = 0,Dwin1SrcWidth = 0;
static MS_BOOL bDipHMirror = FALSE,bDWIN0HMirror = FALSE,bDWIN1HMirror = FALSE;
static MS_BOOL bDipUVSwap = FALSE,bDWIN0UVSwap = FALSE,bDWIN1UVSwap = FALSE;
static MS_BOOL bDipYCSwap = FALSE,bDWIN0YCSwap = FALSE,bDWIN1YCSwap = FALSE;
static MS_BOOL bDipR2Y = FALSE,bDwin0R2Y = FALSE,bDwin1R2Y = FALSE;
static EN_DRV_XC_DWIN_DATA_FMT DipFmt,Dwin0Fmt,Dwin1Fmt;
static SCALER_DIP_WIN DIPRotation = MAX_DIP_WINDOW;

DIP_CHIP_PROPERTY g_DIPChipPro =
{
    .bSourceSupport =                {TRUE, TRUE, FALSE},
    .bDIPBuildIn    =                {TRUE, TRUE, TRUE},
    .XCPathCount    =                XC_PATH_COUNT_SUPPT,
    .BusWordUnit    =                {DWIN_BYTE_PER_WORD, DWIN_BYTE_PER_WORD, 0},
    .bSourceSel =
    {
        .bSOURCE_TYPE_MVOP[0]    =      {TRUE, TRUE, FALSE},
        .bSOURCE_TYPE_MVOP[1]    =      {TRUE, TRUE, FALSE},

        .bSOURCE_TYPE_IP_MAIN[0] =      {TRUE, TRUE, FALSE},
        .bSOURCE_TYPE_IP_SUB[0]  =      {TRUE, TRUE, FALSE},
        .bSOURCE_TYPE_OP_MAIN[0] =      {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OP_SUB[0]  =      {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OP_CAPTURE[0]=    {TRUE, TRUE,  FALSE},

        .bSOURCE_TYPE_IP_MAIN[1] =      {FALSE, FALSE, FALSE}, //VE
        .bSOURCE_TYPE_IP_SUB[1]  =      {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OP_MAIN[1] =      {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OP_SUB[1]  =      {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OP_CAPTURE[1]=    {TRUE, TRUE,  FALSE},

        .bSOURCE_TYPE_DRAM =            {FALSE, FALSE, FALSE},
        .bSOURCE_TYPE_OSD =             {FALSE, FALSE, FALSE},
    }
};

#define DIP_R2BYTEMSK(u32Id,u32Reg,u16Mask)\
({\
    SC_R2BYTEMSK(u32Id, u32Reg, u16Mask);\
})

static MS_U16 HAL_XC_DIP_GetWidth(void *pInstance, SCALER_DIP_SOURCE_TYPE eSource, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    MS_U16 u16MaxWith = 0;

	if(eSource == SCALER_DIP_SOURCE_TYPE_MAIN)
    {
        if (pu32XCInst == NULL)
        {
            printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
            u16MaxWith = 0;
        }
        else
        {
            if (MDrv_XC_GetDynamicScalingStatus(pu32XCInst) || MDrv_XC_Is_DSForceIndexEnabled(pu32XCInst, eWindow) )
            {
                u16MaxWith = g_DIPSrcInfo[eWindow].stCapWin.width;
            }
            else
            {
                u16MaxWith = DIP_R2BYTEMSK(0, REG_SC_BK01_07_L,BMASK(13:0));
            }
        }
    }
    else if(eSource == SCALER_DIP_SOURCE_TYPE_SUB)
    {
        u16MaxWith = DIP_R2BYTEMSK(0, REG_SC_BK03_07_L,BMASK(13:0));
    }
    else if(eSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN)
    {
        u16MaxWith = DIP_R2BYTEMSK(0, REG_SC_BK12_17_L,BMASK(13:0));
    }
    else if(eSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)
    {
        u16MaxWith = DIP_R2BYTEMSK(0, REG_SC_BK12_57_L,BMASK(13:0));
    }
    else if(eSource == SCALER_DIP_SOURCE_TYPE_OP_CAPTURE)
    {
        u16MaxWith = (DIP_R2BYTEMSK(0, REG_SC_BK10_05_L,BMASK(13:0)) - DIP_R2BYTEMSK(0, REG_SC_BK10_04_L,BMASK(13:0))+1);
    }
    else if(eSource == SCALER_DIP_SOURCE_TYPE_DRAM)
    {
        u16MaxWith = DIP_R2BYTEMSK(0, REG_SC_BK36_27_L,BMASK(13:0));
    }
    return u16MaxWith;
}

static MS_BOOL IsNeedAverageMode(SCALER_DIP_SOURCE_TYPE eSource, MS_U16 u16MaxWith)
{
    MS_BOOL bNeedAverageMode = FALSE;
    //k2k@60 averge to 2k2k@60
    if((eSource == SCALER_DIP_SOURCE_TYPE_OP_CAPTURE) && (u16MaxWith>FHD_WIDTH))
    {
        bNeedAverageMode = TRUE;
    }
    else if(( (eSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN ) || (eSource == SCALER_DIP_SOURCE_TYPE_OP_SUB) ) && (u16MaxWith>FHD_WIDTH) )
    {
        bNeedAverageMode = TRUE;
    }
    else if( (eSource == SCALER_DIP_SOURCE_TYPE_MAIN) && (DIP_R2BYTEMSK(0,REG_SC_BK02_0A_L,BIT(15)) != 0 ) )
    {
        bNeedAverageMode = TRUE;
    }
    else if( (eSource == SCALER_DIP_SOURCE_TYPE_SUB) && (DIP_R2BYTEMSK(0,REG_SC_BK04_0A_L,BIT(15)) != 0 ) )
    {
        bNeedAverageMode = TRUE;
    }
    else
    {
        bNeedAverageMode = FALSE;
    }
    return bNeedAverageMode;
}

//==============================================================================
MS_BOOL HAL_XC_DIP_CMDQBegin(void *pInstance,SCALER_DIP_WIN eWindow)
{
    printf("[Warnning][%s], Line:%d, SCALER_DIP CMDQ not Support\n", __FUNCTION__, __LINE__);
    return FALSE;
}

MS_BOOL HAL_XC_DIP_CMDQEnd(void *pInstance,SCALER_DIP_WIN eWindow)
{
    printf("[Warnning][%s], Line:%d, SCALER_DIP CMDQ not Support\n", __FUNCTION__, __LINE__);
    return FALSE;
}
//==============================================================================

//extern PQ_Function_Info    s_PQ_Function_Info;
//extern MS_BOOL             s_bKeepPixelPointerAppear;

void Hal_SC_DWIN_set_422_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
    if(eWindow == DIP_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK34_30_L, bEnable ? BIT(3) : 0, BIT(3));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_44_L, bEnable ? BIT(3) : 0, BIT(3));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_44_L, bEnable ? BIT(3) : 0, BIT(3));
    }
    else
    {
        return;
    }
}

void Hal_SC_DWIN_set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        if (bEnable)
            SC_W2BYTEMSK(0, REG_SC_BK34_60_L, BIT(1), BIT(1));
        else
            SC_W2BYTEMSK(0, REG_SC_BK34_60_L, 0x00 , BIT(1));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        if (bEnable)
            SC_W2BYTEMSK(0, REG_SC_BK3B_62_L, BIT(1), BIT(1));
        else
            SC_W2BYTEMSK(0, REG_SC_BK3B_62_L, 0x00 , BIT(1));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        if (bEnable)
            SC_W2BYTEMSK(0, REG_SC_BK3C_62_L, BIT(1), BIT(1));
        else
            SC_W2BYTEMSK(0, REG_SC_BK3C_62_L, 0x00 , BIT(1));
    }
    else
    {
        return;
    }
}

// This function will return 8/10/12/14/16 field mode or 8 frame mode.
// Otherwise it return IMAGE_STORE_2_FRAMES
XC_FRAME_STORE_NUMBER Hal_SC_DWIN_GetFrameStoreMode(void *pInstance, SCALER_DIP_WIN eWindow,MS_BOOL bInterlace)
{
    if (bInterlace)
    {
        return IMAGE_STORE_4_FIELDS;
    }
    else
    {
        return IMAGE_STORE_2_FRAMES;
    }
}


void Hal_SC_DWIN_EnableR2YCSC(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        bDipR2Y = bEnable;
        if( ((DipSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (DipSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDipR2Y == FALSE) )
        {
            //enable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK34_6C_L,BIT(9),BIT(9));
            //enable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,BIT(14),BIT(14));
        }
        else
        {
            //disable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK34_6C_L,0,BIT(9));
        	//disable  source yc swap
        	SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,0,BIT(14));
        }
        SC_W2BYTEMSK(0, REG_SC_BK34_31_L, (bEnable ? BIT(0):0), BIT(0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bDwin0R2Y = bEnable;
        if( ((Dwin0Source == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (Dwin0Source == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDwin0R2Y == FALSE) )
        {
            //enable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK3B_78_L,BIT(9),BIT(9));
            //enable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,BIT(8),BIT(8));
        }
        else
        {
            //disable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK3B_78_L,0,BIT(9));
            //disable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,0,BIT(8));
        }
        SC_W2BYTEMSK(0, REG_SC_BK3B_45_L, (bEnable ? BIT(0):0), BIT(0));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        bDwin1R2Y = bEnable;
        if( ((Dwin1Source == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (Dwin1Source == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDwin1R2Y == FALSE) )
        {
            //enable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK3C_78_L,BIT(9),BIT(9));
            //enable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,BIT(8),BIT(8));
        }
        else
        {
            //disable  422 to 444
            SC_W2BYTEMSK(0, REG_SC_BK3C_78_L,0,BIT(9));
            //disable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,0,BIT(8));
        }
        SC_W2BYTEMSK(0, REG_SC_BK3C_45_L, (bEnable ? BIT(0):0), BIT(0));
    }
    else
    {
        return;
    }
}

void Hal_SC_DWIN_Set_vsd_output_line_count(void *pInstance, MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16OutputLineCount = 0x00;

    if (bEnable)
    {
        u16OutputLineCount = BIT(15);
        u16OutputLineCount |= (MS_U16)(u32LineCount & 0x1FFF);
    }

    if (eWindow == DIP_WINDOW)
    {
        SC_W2BYTEMSK(0, REG_SC_BK34_37_L, u16OutputLineCount,0x81FF);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_47_L, u16OutputLineCount,0x81FF);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_47_L, u16OutputLineCount,0x81FF);
    }
    else
    {
        return;
    }
}

/*
 bEnable : Enable input line count.
 bUserMode : 1 -> the input line count will ref u32UserLineCount
             0 -> the input line count will ref V capture win
*/
void Hal_SC_DWIN_Set_vsd_input_line_count(void *pInstance, MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_DIP_WIN eWindow)
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

    if (eWindow == DIP_WINDOW)
    {
        SC_W2BYTE(0, REG_SC_BK34_36_L, u16InputLineCount);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTE(0, REG_SC_BK3B_46_L, u16InputLineCount);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTE(0, REG_SC_BK3C_46_L, u16InputLineCount);
    }
    else
    {
        return;
    }
}


void Hal_SC_DWIN_sw_db(void *pInstance, P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
      SC_W4BYTE(0, REG_SC_BK34_04_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
      SC_W4BYTE(0, REG_SC_BK34_08_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling

      SC_W2BYTEMSK(0, REG_SC_BK34_60_L, BIT(1)|BIT(0),BIT(1)|BIT(0));
      SC_W2BYTE(0, REG_SC_BK34_61_L, pDBreg->u16H_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK34_62_L, pDBreg->u16H_CapStart+pDBreg->u16H_CapSize);
      SC_W2BYTE(0, REG_SC_BK34_63_L, pDBreg->u16V_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK34_64_L, pDBreg->u16V_CapStart+pDBreg->u16V_CapSize);
      DipSrcWidth = pDBreg->u16H_CapStart + pDBreg->u16H_CapSize;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
      SC_W4BYTE(0, REG_SC_BK3B_15_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
      SC_W4BYTE(0, REG_SC_BK3B_1B_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling

      SC_W2BYTEMSK(0, REG_SC_BK3B_62_L, BIT(1)|BIT(0),BIT(1)|BIT(0));
      SC_W2BYTE(0, REG_SC_BK3B_63_L, pDBreg->u16H_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK3B_64_L, pDBreg->u16H_CapStart+pDBreg->u16H_CapSize);
      SC_W2BYTE(0, REG_SC_BK3B_65_L, pDBreg->u16V_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK3B_66_L, pDBreg->u16V_CapStart+pDBreg->u16V_CapSize);
      Dwin0SrcWidth = pDBreg->u16H_CapStart + pDBreg->u16H_CapSize;
  }
  else if( eWindow == DWIN1_WINDOW )
  {
      SC_W4BYTE(0, REG_SC_BK3C_15_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
      SC_W4BYTE(0, REG_SC_BK3C_1B_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling

      SC_W2BYTEMSK(0, REG_SC_BK3C_62_L, BIT(1)|BIT(0),BIT(1)|BIT(0));
      SC_W2BYTE(0, REG_SC_BK3C_63_L, pDBreg->u16H_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK3C_64_L, pDBreg->u16H_CapStart+pDBreg->u16H_CapSize);
      SC_W2BYTE(0, REG_SC_BK3C_65_L, pDBreg->u16V_CapStart+1);
      SC_W2BYTE(0, REG_SC_BK3C_66_L, pDBreg->u16V_CapStart+pDBreg->u16V_CapSize);
      Dwin1SrcWidth = pDBreg->u16H_CapStart + pDBreg->u16H_CapSize;
  }
  else
  {
      return;
  }
}
void Hal_SC_DWIN_get_sw_db(void *pInstance, P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
      pDBreg->u32H_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK34_04_L);
      pDBreg->u32V_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK34_08_L);

      pDBreg->u16H_CapStart = (SC_R2BYTE(0, REG_SC_BK34_61_L) - 1);
      pDBreg->u16H_CapSize = (SC_R2BYTE(0, REG_SC_BK34_62_L) - pDBreg->u16H_CapStart);
      pDBreg->u16V_CapStart = (SC_R2BYTE(0, REG_SC_BK34_63_L) - 1);
      pDBreg->u16V_CapSize = (SC_R2BYTE(0, REG_SC_BK34_64_L) - pDBreg->u16V_CapStart);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
      pDBreg->u32H_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK3B_15_L);
      pDBreg->u32V_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK3B_1B_L);

      pDBreg->u16H_CapStart = (SC_R2BYTE(0, REG_SC_BK3B_63_L) - 1);
      pDBreg->u16H_CapSize = (SC_R2BYTE(0, REG_SC_BK3B_64_L) - pDBreg->u16H_CapStart);
      pDBreg->u16V_CapStart = (SC_R2BYTE(0, REG_SC_BK3B_65_L) - 1);
      pDBreg->u16V_CapSize = (SC_R2BYTE(0, REG_SC_BK3B_66_L) - pDBreg->u16V_CapStart);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
      pDBreg->u32H_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK3C_15_L);
      pDBreg->u32V_PreScalingRatio =  SC_R4BYTE(0, REG_SC_BK3C_1B_L);

      pDBreg->u16H_CapStart = (SC_R2BYTE(0, REG_SC_BK3C_63_L) - 1);
      pDBreg->u16H_CapSize = (SC_R2BYTE(0, REG_SC_BK3C_64_L) - pDBreg->u16H_CapStart);
      pDBreg->u16V_CapStart = (SC_R2BYTE(0, REG_SC_BK3C_65_L) - 1);
      pDBreg->u16V_CapSize = (SC_R2BYTE(0, REG_SC_BK3C_66_L) - pDBreg->u16V_CapStart);
    }
    else
    {
      return;
    }
}
void HAL_XC_DIP_Set444to422(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,MS_BOOL bSrcYUVFmt,MS_BOOL bSrcFmt422,SCALER_DIP_WIN eWindow)
{

}
void HAL_XC_DIP_SetFRC(void *pInstance, MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16Ratio=0;

    if( eWindow == DIP_WINDOW )
    {
        if(bEnable)
        {
            if(u16In > u16Out)
            {
                u16Ratio = (u16In-u16Out)*64/u16In;
                SC_W2BYTEMSK(0, REG_SC_BK34_40_L,u16Ratio,BMASK(5:0));
            }
            else
            {
                SC_W2BYTEMSK(0, REG_SC_BK34_40_L,0,BMASK(5:0));
            }
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_40_L,0,BMASK(5:0));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        if(bEnable)
        {
            if(u16In > u16Out)
            {
                u16Ratio = (u16In-u16Out)*64/u16In;
                SC_W2BYTEMSK(0, REG_SC_BK3B_4D_L,u16Ratio,BMASK(5:0));
            }
            else
            {
                SC_W2BYTEMSK(0, REG_SC_BK3B_4D_L,0,BMASK(5:0));
            }
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_4D_L,0,BMASK(5:0));
        }
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        if(bEnable)
        {
            if(u16In > u16Out)
            {
                u16Ratio = (u16In-u16Out)*64/u16In;
                SC_W2BYTEMSK(0, REG_SC_BK3C_4D_L,u16Ratio,BMASK(5:0));
            }
            else
            {
                SC_W2BYTEMSK(0, REG_SC_BK3C_4D_L,0,BMASK(5:0));
            }
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_4D_L,0,BMASK(5:0));
        }
    }
    else
    {
        return;
    }

}
void Hal_SC_DWIN_set_input_vsync_inverse(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0,  REG_SC_BK34_01_L, (bEnable<<2), BIT(2) );
    }
    else if( eWindow == DWIN0_WINDOW )
    {
    	SC_W2BYTEMSK(0,  REG_SC_BK3B_12_L, (bEnable<<2), BIT(2) );
    }
    else if( eWindow == DWIN1_WINDOW )
    {
    	SC_W2BYTEMSK(0,  REG_SC_BK3C_12_L, (bEnable<<2), BIT(2) );
    }
    else
    {
    return;
    }
}

//=============== DIP =====================//
#define DWIN_CAPTURE_TIMEOUT_CNT 10 //consider 576i video, maximum time is 20+20x2 = 60 ms

MS_U16 HAL_XC_DIP_GetBusSize(void *pInstance, SCALER_DIP_WIN eWindow)
{
    return g_DIPChipPro.BusWordUnit[eWindow];
}

void HAL_XC_DIP_SWReset(void *pInstance, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,BIT(0), BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,0, BIT(0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_73_L,BIT(0), BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK3B_73_L,0, BIT(0));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_73_L,BIT(0), BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK3C_73_L,0, BIT(0));
    }
    else
    {
        return;
    }
}

void HAL_XC_DIP_Init(void *pInstance, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
        SC_W2BYTE(0, REG_SC_BK36_01_L,0x0840);
        // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
        SC_W2BYTE(0, REG_SC_BK36_02_L,0x0400);
        //[8] write once; [9] write once trig
        if(HAL_XC_DIP_GetBusSize(pInstance,eWindow) == DIP_BYTE_PER_WORD)
            SC_W2BYTE(0, REG_SC_BK36_03_L,0x0000);
        else
            SC_W2BYTE(0, REG_SC_BK36_03_L,0x0800);
        //When dip overstep memory next to dip,dip won't stop telling this wrong state with continuous interrupt.
        //The continuous interrupt will cause the high cpu possesion.
        //So we mask the related bit to ignore the wrong state.
        //Function HAL_XC_MIU_Protect will protect the memory next to dip being overstepped by dip in case "MIU_PROTECT == 1".
        SC_W2BYTEMSK(0, REG_SC_BK36_04_L, BIT(4),BIT(4));
        //clear mirror state
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L, 0 ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L, 0 ,BIT(10));
        //[15:8]wreq max ; [7:0]wreq threshold
        SC_W2BYTE(0, REG_SC_BK36_6F_L,0x200A);
        //tile request number
        SC_W2BYTE(0, REG_SC_BK36_7E_L,0x0010);
        //Double buffer enable
        SC_W2BYTEMSK(0, REG_SC_BK34_7E_L,BIT(0),BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK36_7F_L,BIT(7),BIT(7));
        //enable dip clk
        MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_GATED);                    // Enable clock
        SC_W2BYTEMSK(0, REG_SC_BK6C_40_L, 0, BIT(7));    //Enable clock

        //enable OSD blending
        SC_W2BYTEMSK(0, REG_SC_BK34_10_L, BIT(0) ,BIT(0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
        SC_W2BYTE(0, REG_SC_BK3B_01_L,0x0840);
        // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
        SC_W2BYTE(0, REG_SC_BK3B_02_L,0x0400);
        //[8] write once; [9] write once trig[11]256mode
        if(HAL_XC_DIP_GetBusSize(pInstance,eWindow) == DIP_BYTE_PER_WORD)
            SC_W2BYTE(0, REG_SC_BK3B_03_L,0x0000);
        else
            SC_W2BYTE(0, REG_SC_BK3B_03_L,0x0800);
        //When dip overstep memory next to dip,dip won't stop telling this wrong state with continuous interrupt.
        //The continuous interrupt will cause the high cpu possesion.
        //So we mask the related bit to ignore the wrong state.
        //Function HAL_XC_MIU_Protect will protect the memory next to dip being overstepped by dip in case "MIU_PROTECT == 1".
        SC_W2BYTEMSK(0, REG_SC_BK3B_04_L, BIT(4),BIT(4));
        //clear mirror state
        SC_W2BYTEMSK(0, REG_SC_BK3B_0B_L, 0 ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK3B_0B_L, 0 ,BIT(10));
        //[15:8]wreq max ; [7:0]wreq threshold
        SC_W2BYTE(0, REG_SC_BK3B_6F_L,0x200A);
        //tile request number
        SC_W2BYTE(0, REG_SC_BK3B_7E_L,0x0010);
        //Double buffer enable
        SC_W2BYTEMSK(0, REG_SC_BK3B_72_L,BIT(0),BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,BIT(7),BIT(7));
        //enable dwin0 clk
        MDrv_WriteRegBit(REG_CKG_PDW0, DISABLE, CKG_PDW0_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_PDW0, DISABLE, CKG_PDW0_GATED);                    // Enable clock
        SC_W2BYTEMSK(0, REG_SC_BK6C_60_L, 0, BIT(7));    //Enable clock
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //[15]intlac. src;[12:8]422to420 ratio;[6]dwin_en;[5:4]format(rgb888);[3]intlac_w;[2:0]frm_buf_num
        SC_W2BYTE(0, REG_SC_BK3C_01_L,0x0840);
        // [10]pdw_off;[9:8] reg_dip_pdw_src_sel;[7:0]alpha
        SC_W2BYTE(0, REG_SC_BK3C_02_L,0x0400);
        //[8] write once; [9] write once trig[11]256mode
        if(HAL_XC_DIP_GetBusSize(pInstance,eWindow) == DIP_BYTE_PER_WORD)
            SC_W2BYTE(0, REG_SC_BK3C_03_L,0x0000);
        else
            SC_W2BYTE(0, REG_SC_BK3C_03_L,0x0800);
        //When dip overstep memory next to dip,dip won't stop telling this wrong state with continuous interrupt.
        //The continuous interrupt will cause the high cpu possesion.
        //So we mask the related bit to ignore the wrong state.
        //Function HAL_XC_MIU_Protect will protect the memory next to dip being overstepped by dip in case "MIU_PROTECT == 1".
        SC_W2BYTEMSK(0, REG_SC_BK3C_04_L, BIT(4),BIT(4));
        //clear mirror state
        SC_W2BYTEMSK(0, REG_SC_BK3C_0B_L, 0 ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK3C_0B_L, 0 ,BIT(10));
        //[15:8]wreq max ; [7:0]wreq threshold
        SC_W2BYTE(0, REG_SC_BK3C_6F_L,0x200A);
        //tile request number
        SC_W2BYTE(0, REG_SC_BK3C_7E_L,0x0010);
        //Double buffer enable
        SC_W2BYTEMSK(0, REG_SC_BK3C_72_L,BIT(0),BIT(0));
        SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,BIT(7),BIT(7));
        //enable dwin1 clk
        MDrv_WriteRegBit(REG_CKG_PDW1, DISABLE, CKG_PDW1_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_PDW1, DISABLE, CKG_PDW1_GATED);                    // Enable clock
    }
    SC_W2BYTEMSK(0, REG_SC_BK36_1C_L, 0 ,BIT(14));

    //force OP1 ACK 1
    //SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,BIT(1),BIT(1));
    //for DIP R
    //read request maximum length and 2������, tile 420 used
    SC_W2BYTE(0, REG_SC_BK36_7F_L,0x400F);
    //read request threshold
    SC_W2BYTEMSK(0, REG_SC_BK36_7D_L,0xA,BMASK(5:0));
    //reg_dipr_tile_req_num_evd
    SC_W2BYTEMSK(0, REG_SC_BK36_1C_L,0x7,BMASK(4:0));

    //output capture setting
    SC_W2BYTEMSK(0, REG_SC_BK10_23_L, BIT(4), BIT(4));  // Set to no osd for DIP select
    SC_W2BYTEMSK(0, REG_SC_BK10_23_L, E_XC_DIP_VOP2<<12, BMASK(13:12));
    SC_W2BYTEMSK(0, REG_SC_BK10_50_L, BIT(14), BMASK(14:13));

    SC_W2BYTEMSK(0, REG_SC_BK10_50_L, BIT(12), BIT(12));  // Set to with osd for DIP select
    SC_W2BYTEMSK(0, REG_SC_BK10_50_L, E_XC_DIP_OP2<<8, BMASK(9:8));
    SC_W2BYTEMSK(0, REG_SC_BK10_6B_L, BIT(13), BMASK(13:12));

    //recv2dip_cap_en_main
    SC_W2BYTEMSK(0, REG_SC_BK4A_05_L, BIT(1), BIT(1));
    //recv2dip_cap_en_sub
    SC_W2BYTEMSK(0, REG_SC_BK4A_04_L, BIT(1), BIT(1));

    //Sc2 op capture enable:
    SC_W2BYTEMSK(0, REG_SC_BK90_50_L, BIT(12), BIT(12));
    //Sc2 op capture stage 0: before osdb 1: after osdb
    SC_W2BYTEMSK(0, REG_SC_BK90_50_L, E_XC_DIP_OP2<<8, BMASK(9:8));// Set to with osd for DIP select
}

void HAL_XC_DIP_EnableCaptureStream(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16IntrStus = 0;
    MS_XC_DIP_SOURCE_TYPE eHalSource;
    SCALER_DIP_SOURCE_TYPE eWindowSource = SCALER_DIP_SOURCE_TYPE_MAIN;

    if( eWindow == DIP_WINDOW )
        eWindowSource = DipSource;
    else if( eWindow == DWIN0_WINDOW )
        eWindowSource = Dwin0Source;
    else if( eWindow == DWIN1_WINDOW )
        eWindowSource = Dwin1Source;

    switch(eWindowSource)
    {
        case SCALER_DIP_SOURCE_TYPE_DRAM:
        case SCALER_DIP_SOURCE_TYPE_SUB2 :
        case SCALER_DIP_SOURCE_TYPE_OP_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_OSD:
        case SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_MVOP0:
        case SCALER_DIP_SOURCE_TYPE_MVOP1:
            eHalSource = E_XC_DIP_SOURCE_TYPE_SUB2;
            break;
        case SCALER_DIP_SOURCE_TYPE_MAIN :
            eHalSource = E_XC_DIP_SOURCE_TYPE_MAIN;
            break;
        case SCALER_DIP_SOURCE_TYPE_SUB :
            eHalSource = E_XC_DIP_SOURCE_TYPE_SUB;
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_MAIN:
        case SCALER_DIP_SOURCE_TYPE_OP_SUB:
            eHalSource = E_XC_DIP_SOURCE_TYPE_OP;
            break;
        default:
            printf("SCALER_DIP_SOURCE_TYPE does not support\n");
            return;
    }


    if(bEnable == TRUE)
    {
        //Control DIPR
        if(eWindowSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            // Because Kano DIP has DIP multi view(4-way), In order to achieve performace, so adjust fclk to 320M
            MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_320MHZ, CKG_FCLK_MASK);
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L,BIT(14), BIT(14));
        }
        else if( (eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB) )
        {
            if( eWindow == DIP_WINDOW )
            {
                SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(15):0),BIT(15));
            }
            else if( eWindow == DWIN0_WINDOW )
            {
                SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L, ((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(6):0),BIT(6));
            }
            else if( eWindow == DWIN1_WINDOW )
            {
                SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L, ((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(6):0),BIT(6));
            }
        }
        //new rotation enabel
        SC_W2BYTEMSK(0, REG_SC_BK34_6E_L,bDIPRotation?BIT(11):0,BIT(11));
        //Enable last
        if( eWindow == DIP_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        }
    }
    else
    {
		//new rotation disable
        SC_W2BYTEMSK(0, REG_SC_BK34_6E_L,0,BIT(11));
        //Disable first
        if( eWindow == DIP_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BIT(10), BMASK(10:8));
            //disable write once
            SC_W2BYTEMSK(0, REG_SC_BK36_03_L, 0, BIT(8));
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BIT(10), BMASK(10:8));
            //disable write once
            SC_W2BYTEMSK(0, REG_SC_BK3B_03_L, 0, BIT(8));
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, BIT(10), BMASK(10:8));
            //disable write once
            SC_W2BYTEMSK(0, REG_SC_BK3C_03_L, 0, BIT(8));
        }

        if(eWindowSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            // Because Kano DIP has DIP multi view(4-way), In order to achieve performace, so adjust fclk to 320M
            // disable DIPR so adjust fclk to 216M
            MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_216MHZ, CKG_FCLK_MASK);
            //Dipr disable
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L,0, BIT(14));
        }
    }


    if (!bEnable)
    {
        //Auto clear status to zero
        u16IntrStus = HAL_XC_DIP_GetIntrStatus(pInstance, eWindow);
        HAL_XC_DIP_ClearIntr(pInstance, u16IntrStus,eWindow);
        HAL_XC_DIP_SWReset(pInstance, eWindow);
    }
}
void HAL_XC_DIP_EnableIntr(void *pInstance, MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    MS_U16 regval =0;

    if( eWindow == DIP_WINDOW )
    {
        regval = SC_R2BYTE(0, REG_SC_BK36_08_L);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        regval = SC_R2BYTE(0, REG_SC_BK3B_08_L);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        regval = SC_R2BYTE(0, REG_SC_BK3C_08_L);
    }

    if(bEnable)
        regval &= ~u8mask;
    else
        regval |= u8mask;

    SC_W2BYTEMSK(0, REG_SC_BK36_08_L, regval, BMASK(7:0));
}
void HAL_XC_DIP_ClearIntr(void *pInstance, MS_U16 u16mask,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_09_L,u16mask&BMASK(7:0), BMASK(7:0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_09_L,u16mask&BMASK(7:0), BMASK(7:0));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_09_L,u16mask&BMASK(7:0), BMASK(7:0));
    }
    else
    {
        return;
    }

}
MS_U16 HAL_XC_DIP_GetIntrStatus(void *pInstance, SCALER_DIP_WIN eWindow)
{
    MS_U16 u16IntrStus = 0;
    if( eWindow == DIP_WINDOW )
    {
        u16IntrStus = SC_R2BYTEMSK(0, REG_SC_BK36_0A_L,BMASK(7:0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        u16IntrStus = SC_R2BYTEMSK(0, REG_SC_BK3B_0A_L,BMASK(7:0));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        u16IntrStus = SC_R2BYTEMSK(0, REG_SC_BK3C_0A_L,BMASK(7:0));
    }

    return u16IntrStus;
}
void HAL_XC_DIP_CpatureOneFrame(void *pInstance, SCALER_DIP_WIN eWindow)//MS_BOOL benable, SCALER_DIP_WIN eWindow)
{
    MS_U16 regval = 0;
    MS_U16 u16Count = 0;

    HAL_XC_DIP_CpatureOneFrame2(pInstance, eWindow);

    do{
        regval = HAL_XC_DIP_GetIntrStatus(pInstance, eWindow);
        if(regval > 0x0)
        break;
        u16Count++;
        MsOS_DelayTask(1);
    }while(u16Count < DWIN_CAPTURE_TIMEOUT_CNT);
    //if(u16Count >= DWIN_CAPTURE_TIMEOUT_CNT)
    //    printf("!!!Alert !!! DWIN Capture, wait ack time out!\n");

}
void HAL_XC_DIP_CpatureOneFrame2(void *pInstance, SCALER_DIP_WIN eWindow)//MS_BOOL benable, SCALER_DIP_WIN eWindow)
{
    MS_XC_DIP_SOURCE_TYPE eHalSource;
    SCALER_DIP_SOURCE_TYPE eWindowSource = SCALER_DIP_SOURCE_TYPE_MAIN;

    if( eWindow == DIP_WINDOW )
        eWindowSource = DipSource;
    else if( eWindow == DWIN0_WINDOW )
        eWindowSource = Dwin0Source;
    else if( eWindow == DWIN1_WINDOW )
        eWindowSource = Dwin1Source;

    switch(eWindowSource)
    {
        case SCALER_DIP_SOURCE_TYPE_DRAM:
        case SCALER_DIP_SOURCE_TYPE_SUB2 :
        case SCALER_DIP_SOURCE_TYPE_OP_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_OSD:
        case SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE:
        case SCALER_DIP_SOURCE_TYPE_MVOP0:
        case SCALER_DIP_SOURCE_TYPE_MVOP1:
            eHalSource = E_XC_DIP_SOURCE_TYPE_SUB2;
            break;
        case SCALER_DIP_SOURCE_TYPE_MAIN :
            eHalSource = E_XC_DIP_SOURCE_TYPE_MAIN;
            break;
        case SCALER_DIP_SOURCE_TYPE_SUB :
            eHalSource = E_XC_DIP_SOURCE_TYPE_SUB;
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_MAIN:
        case SCALER_DIP_SOURCE_TYPE_OP_SUB:
            eHalSource = E_XC_DIP_SOURCE_TYPE_OP;
            break;
        default:
            printf("SCALER_DIP_SOURCE_TYPE does not support\n");
            return;
    }


    //Control OP1 Bank and DIPR
    if(eWindowSource == SCALER_DIP_SOURCE_TYPE_DRAM)
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7D_L,BIT(14), BIT(14));
    }
    else if( (eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB) )
    {
        if( eWindow == DIP_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(15):0),BIT(15));
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L, ((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(6):0),BIT(6));
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L, ((eWindowSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)?BIT(6):0),BIT(6));
        }
    }

	//new rotation enabel
    SC_W2BYTEMSK(0, REG_SC_BK34_6E_L,bDIPRotation?BIT(11):0,BIT(11));
    //Enable last
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        if(DipSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //Dipr trigger
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(15),BIT(15));
        }
        else
        {
            // Set the dwin write once   (0x03 bit:8)
            SC_W2BYTEMSK(0, REG_SC_BK36_03_L, BIT(8),BIT(8));
            //trigger
            SC_W2BYTEMSK(0, REG_SC_BK36_03_L, BIT(9),BIT(9));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        if(Dwin0Source == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //Dipr trigger
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L,BIT(15), BIT(15));
        }
        else
        {
            // Set the dwin write once   (0x03 bit:8)
            SC_W2BYTEMSK(0, REG_SC_BK3B_03_L, BIT(8), BIT(8));
            //trigger
            SC_W2BYTEMSK(0, REG_SC_BK3B_03_L, BIT(9), BIT(9));
        }
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, BITS(9:8,eHalSource),BMASK(10:8));
        if(Dwin1Source == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //Dipr trigger
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L,BIT(15), BIT(15));
        }
        else
        {
            // Set the dwin write once   (0x03 bit:8)
            SC_W2BYTEMSK(0, REG_SC_BK3C_03_L, BIT(8), BIT(8));
            //trigger
            SC_W2BYTEMSK(0, REG_SC_BK3C_03_L, BIT(9), BIT(9));
        }
    }

}
void HAL_XC_DIP_SelectSourceScanType(void *pInstance, EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_01_L,enScan?BIT(15):0, BIT(15));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_01_L,enScan?BIT(15):0, BIT(15));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_01_L,enScan?BIT(15):0, BIT(15));
    }
    else
    {
        return;
    }
}
EN_XC_DWIN_SCAN_TYPE HAL_XC_DIP_GetSourceScanType(void *pInstance, SCALER_DIP_WIN eWindow)
{
    EN_XC_DWIN_SCAN_TYPE eSacnType = GOPDWIN_SCAN_MODE_MAX;
    if( eWindow == DIP_WINDOW )
    {
        eSacnType = (EN_XC_DWIN_SCAN_TYPE)(SC_R2BYTEMSK(0, REG_SC_BK36_01_L,BIT(15))>>15);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        eSacnType = (EN_XC_DWIN_SCAN_TYPE)(SC_R2BYTEMSK(0, REG_SC_BK3B_01_L,BIT(15))>>15);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        eSacnType = (EN_XC_DWIN_SCAN_TYPE)(SC_R2BYTEMSK(0, REG_SC_BK3C_01_L,BIT(15))>>15);
    }

    return eSacnType;
}
void HAL_XC_DIP_SetInterlaceWrite(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_01_L, (bEnable?BIT(3):0) ,BIT(3));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_01_L, (bEnable?BIT(3):0) ,BIT(3));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_01_L, (bEnable?BIT(3):0) ,BIT(3));
    }
    else
    {
        return;
    }
}
MS_BOOL HAL_XC_DIP_GetInterlaceWrite(void *pInstance, SCALER_DIP_WIN eWindow)
{
    MS_BOOL bInterW = FALSE;
    if( eWindow == DIP_WINDOW )
    {
        bInterW = (MS_BOOL)(SC_R2BYTEMSK(0, REG_SC_BK36_01_L,BIT(3))>>3);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bInterW = (MS_BOOL)(SC_R2BYTEMSK(0, REG_SC_BK3B_01_L,BIT(3))>>3);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        bInterW = (MS_BOOL)(SC_R2BYTEMSK(0, REG_SC_BK3C_01_L,BIT(3))>>3);
    }

    return bInterW;
}
void HAL_XC_DIP_SetDataFmt(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16Fmt = 0x0;

    switch (fmt)
    {
        case XC_DWIN_DATA_FMT_YUV422 :
        case XC_DWIN_DATA_FMT_YC422 :
            u16Fmt = 0;
            break;
        case XC_DWIN_DATA_FMT_RGB565 :
            u16Fmt = 1;
            break;
        case XC_DWIN_DATA_FMT_ARGB8888 :
            u16Fmt = 2;
            break;
        case XC_DWIN_DATA_FMT_YUV420 :
        case XC_DWIN_DATA_FMT_YUV420_H265 :
        case XC_DWIN_DATA_FMT_YUV420_H265_10BITS:
        case XC_DWIN_DATA_FMT_YUV420_PLANER:
        case XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER:
            u16Fmt = 3;
            break;
        default :
            u16Fmt = 0x0;
            break;
    }
    if( eWindow == DIP_WINDOW )
    {
        DipFmt = fmt;
        //set fmt
        SC_W2BYTEMSK(0, REG_SC_BK36_01_L, BITS(5:4,u16Fmt),BMASK(5:4));
        //select yc separate
        SC_W2BYTEMSK(0, REG_SC_BK36_03_L,((fmt == XC_DWIN_DATA_FMT_YC422)?BIT(6):0),BIT(6));
        //select de-tile for 420 semi planer
        SC_W2BYTEMSK(0, REG_SC_BK36_03_L,((fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER)?BIT(14):0),BIT(14));
        //select flash mode for 420 planer
        SC_W2BYTEMSK(0, REG_SC_BK36_7E_L,((fmt == XC_DWIN_DATA_FMT_YUV420_PLANER)?BIT(14):0),BIT(14));

        if( (fmt == XC_DWIN_DATA_FMT_RGB565) || (fmt == XC_DWIN_DATA_FMT_ARGB8888) )
        {
            //disable yc swap setting
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L,0,BMASK(15:14));
            //disable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK34_30_L,0,BIT(5)|BIT(0));
        }
        else
        {
            //disable rgb swap setting
            SC_W2BYTEMSK(0, REG_SC_BK36_7E_L,0,BIT(13));
            //enable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK34_30_L, (BIT(5)|BIT(0)),(BIT(5)|BIT(0)));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        Dwin0Fmt = fmt;
        //set fmt
        SC_W2BYTEMSK(0, REG_SC_BK3B_01_L, BITS(5:4,u16Fmt),BMASK(5:4));
        //select yc separate
        SC_W2BYTEMSK(0, REG_SC_BK3B_03_L,((fmt == XC_DWIN_DATA_FMT_YC422)?BIT(6):0),BIT(6));
        //select de-tile for 420 semi planer
        SC_W2BYTEMSK(0, REG_SC_BK3B_03_L,((fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER)?BIT(14):0),BIT(14));
        //select flash mode for 420 planer
        SC_W2BYTEMSK(0, REG_SC_BK3B_7E_L,((fmt == XC_DWIN_DATA_FMT_YUV420_PLANER)?BIT(14):0),BIT(14));

        if( (fmt == XC_DWIN_DATA_FMT_RGB565) || (fmt == XC_DWIN_DATA_FMT_ARGB8888) )
        {
            //disable yc swap setting
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L,0,BMASK(15:14));
            //disable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK3B_44_L,0,BIT(5)|BIT(0));
        }
        else
        {
            //disable rgb swap setting
            SC_W2BYTEMSK(0, REG_SC_BK3B_7E_L,0,BIT(13));
            //enable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK3B_44_L, (BIT(5)|BIT(0)),(BIT(5)|BIT(0)));
        }
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        Dwin1Fmt = fmt;
        //set fmt
        SC_W2BYTEMSK(0, REG_SC_BK3C_01_L, BITS(5:4,u16Fmt),BMASK(5:4));
        //select yc separate
        SC_W2BYTEMSK(0, REG_SC_BK3C_03_L,((fmt == XC_DWIN_DATA_FMT_YC422)?BIT(6):0),BIT(6));
        //select de-tile for 420 semi planer
        SC_W2BYTEMSK(0, REG_SC_BK3C_03_L,((fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER)?BIT(14):0),BIT(14));
        //select flash mode for 420 planer
        SC_W2BYTEMSK(0, REG_SC_BK3C_7E_L,((fmt == XC_DWIN_DATA_FMT_YUV420_PLANER)?BIT(14):0),BIT(14));

        if( (fmt == XC_DWIN_DATA_FMT_RGB565) || (fmt == XC_DWIN_DATA_FMT_ARGB8888) )
        {
            //disable yc swap setting
            SC_W2BYTEMSK(0, REG_SC_BK3C_02_L,0,BMASK(15:14));
            //disable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK3C_44_L,0,BIT(5)|BIT(0));
        }
        else
        {
            //disable rgb swap setting
            SC_W2BYTEMSK(0, REG_SC_BK3C_7E_L,0,BIT(13));
            //enable 444 to 422
            SC_W2BYTEMSK(0, REG_SC_BK3C_44_L, (BIT(5)|BIT(0)),(BIT(5)|BIT(0)));
        }
    }
    else
    {
        return;
    }
}
EN_DRV_XC_DWIN_DATA_FMT HAL_XC_DIP_GetDataFmt(void *pInstance, SCALER_DIP_WIN eWindow)
{
    EN_DRV_XC_DWIN_DATA_FMT eFmt = XC_DWIN_DATA_FMT_MAX;
    if( eWindow == DIP_WINDOW )
    {
        return DipFmt;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        return Dwin0Fmt;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        return Dwin1Fmt;
    }

    return eFmt;
}
MS_U16 HAL_XC_DIP_GetBPP(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fbFmt,SCALER_DIP_WIN eWindow)
{
    MS_U16 bpp=0;

    switch ( fbFmt )
    {
        case XC_DWIN_DATA_FMT_YUV422 :
        case XC_DWIN_DATA_FMT_YC422  :
        case XC_DWIN_DATA_FMT_RGB565 :
            bpp = 2;
            break;
        case XC_DWIN_DATA_FMT_ARGB8888 :
            bpp = 4;
            break;
        case XC_DWIN_DATA_FMT_YUV420 :
        case XC_DWIN_DATA_FMT_YUV420_H265 :
        case XC_DWIN_DATA_FMT_YUV420_H265_10BITS :
        case XC_DWIN_DATA_FMT_YUV420_PLANER :
        case XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER :
            bpp = 1;
            break;
        default :
            bpp = 0xFF;
            break;
    }
    return bpp;
}
MS_U16 HAL_XC_DIP_WidthAlignCheck(void *pInstance, MS_U16 u16Width,MS_U16 u16Bpp,SCALER_DIP_WIN eWindow)
{
    MS_U16 AlignFactor;
    MS_U16 u16BusSize = 0;

    u16BusSize = HAL_XC_DIP_GetBusSize(pInstance, eWindow);
    AlignFactor = u16BusSize;
    return (u16Width + AlignFactor-1) & (~(AlignFactor-1));
}

void HAL_XC_DIP_SetMux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        MDrv_WriteByteMask(REG_IPMUX_02_L, u8Data_Mux << 4, 0xF0);
        MDrv_WriteByteMask(REG_CKG_IDCLK3, u8Clk_Mux, CKG_IDCLK3_MASK);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK34_7E_L,u8Data_Mux << 8,BMASK(11:8));
        MDrv_WriteByteMask(REG_CKG_PDW0, u8Clk_Mux, CKG_PDW0_MASK);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK34_7E_L,u8Data_Mux << 12,BMASK(15:12));
        MDrv_WriteByteMask(REG_CKG_PDW1, u8Clk_Mux, CKG_PDW1_MASK);
    }
    else
    {
        return;
    }
}

MS_BOOL HAL_XC_DIP_GetCaptureSourceStatus(void *pInstance, SCALER_DIP_SOURCE_TYPE eSource,SCALER_DIP_WIN eWindow, MS_BOOL* bstatus)
{
    switch(eSource)
    {
        case SCALER_DIP_SOURCE_TYPE_MVOP0:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_MVOP[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_MVOP1:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_MVOP[1][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_MAIN:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_IP_MAIN[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_SUB:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_IP_SUB[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_MAIN:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_MAIN[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_SUB:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_SUB[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_CAPTURE:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_CAPTURE[0][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE:
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_CAPTURE[1][eWindow];
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE :
        {
            if(XC_PATH_COUNT_SUPPT<=2)
            {
                *bstatus= FALSE;
            }
            else
            {
                *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_CAPTURE[XC_PATH_COUNT_SUPPT-1][eWindow];
            }
        }
            break;
        case SCALER_DIP_SOURCE_TYPE_DRAM  :
        {
            *bstatus = g_DIPChipPro.bSourceSel.bSOURCE_TYPE_DRAM[eWindow];
        }
            break;
        default:
            printf("[%s][%d] eWindow =%d ---->eSource = %d is not Support\n",__FUNCTION__,__LINE__,eWindow, eSource);
            *bstatus = FALSE;
            return FALSE;
            break;

    }

    return TRUE;
}

void HAL_XC_DIP_MuxDispatch(void *pInstance, SCALER_DIP_SOURCE_TYPE eSource,SCALER_DIP_WIN eWindow)
{
    MS_BOOL eRet=FALSE;
    //MS_U8 u8Clk = 0;

    switch(eSource)
    {
        case SCALER_DIP_SOURCE_TYPE_MVOP0:
           {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_MVOP[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                    {
                        if( eWindow == DIP_WINDOW )
                        {
                            HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_MVOP,CKG_IDCLK3_CLK_DC0,eWindow);
                        }
                        else if( eWindow == DWIN0_WINDOW )
                        {
                            HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_MVOP,CKG_PDW0_CLK_DC0,eWindow);
                        }
                        else if( eWindow == DWIN1_WINDOW )
                        {
                        }
                        eRet= TRUE;
                    }
            }

            break;
        case SCALER_DIP_SOURCE_TYPE_MVOP1:
           {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_MVOP[1][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                        if( eWindow == DIP_WINDOW )
                        {
                            HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_MVOP2,CKG_IDCLK3_CLK_DC1,eWindow);
                        }
                        else if( eWindow == DWIN0_WINDOW )
                        {
                            HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_MVOP2,CKG_PDW0_CLK_SUB_DC,eWindow);
                        }
                        else if( eWindow == DWIN1_WINDOW )
                        {
                        }
                        eRet= TRUE;
                }
            }
            break;

        case SCALER_DIP_SOURCE_TYPE_MAIN:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_IP_MAIN[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_IP_MAIN,CKG_IDCLK3_ODCLK ,eWindow);
						SC_W2BYTEMSK(0, REG_SC_BK34_50_L, 0,BIT(15));
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_IP_MAIN,CKG_PDW0_IDCLK1,eWindow);
						SC_W2BYTEMSK(0, REG_SC_BK3B_7A_L, 0,BIT(15));
						SC_W2BYTEMSK(0, REG_SC_BK3B_70_L, 0,BMASK(4:1));
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }
            }
            break;

        case SCALER_DIP_SOURCE_TYPE_SUB:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_IP_SUB[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_IP_SUB,CKG_IDCLK3_ODCLK,eWindow);
						SC_W2BYTEMSK(0, REG_SC_BK34_50_L, BIT(15),BIT(15));
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_IP_SUB,CKG_PDW0_IDCLK1,eWindow);
						SC_W2BYTEMSK(0, REG_SC_BK3B_7A_L, BIT(15),BIT(15));
						SC_W2BYTEMSK(0, REG_SC_BK3B_70_L, BIT(4)|BIT(1),BMASK(4:1));
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }
            }
            break;

            case SCALER_DIP_SOURCE_TYPE_OP_MAIN:
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_MAIN[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                eRet= TRUE;
                break;
            case SCALER_DIP_SOURCE_TYPE_OP_SUB:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_SUB[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }

                if( eWindow == DIP_WINDOW )
                {
                }
                else if( eWindow == DWIN0_WINDOW )
                {
                }
                else if( eWindow == DWIN1_WINDOW )
                {
                }
                eRet= TRUE;
            }
            break;

            case SCALER_DIP_SOURCE_TYPE_DRAM:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_DRAM[eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                }
                eRet= TRUE;
            }
            break;

            case SCALER_DIP_SOURCE_TYPE_OP_CAPTURE:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_CAPTURE[0][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_OP1,CKG_IDCLK3_00,eWindow);
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_OP1,CKG_PDW0_ODCLK,eWindow);
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }

            }
            break;

            case SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OP_CAPTURE[1][eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_SC1_CAPTURE,CKG_IDCLK3_XTAL,eWindow);
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                        HAL_XC_DIP_SetMux(pInstance, SC_DWIN_IPMUX_SC1_CAPTURE,CKG_PDW0_IDCLK3_XTAL,eWindow);
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }
            }
            break;

            case SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE:
            {
                if(XC_PATH_COUNT_SUPPT<=2)
                {
                    eRet= FALSE;
                }
                else if( XC_PATH_COUNT_SUPPT <=3)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }
            }
            break;

            case SCALER_DIP_SOURCE_TYPE_OSD:
            {
                if(g_DIPChipPro.bSourceSel.bSOURCE_TYPE_OSD[eWindow]!=TRUE)
                {
                    eRet= FALSE;
                }
                else
                {
                    if( eWindow == DIP_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN0_WINDOW )
                    {
                    }
                    else if( eWindow == DWIN1_WINDOW )
                    {
                    }
                    eRet= TRUE;
                }

            }
            break;
        default:
            eRet= FALSE;
            break;
    }

    if(eRet==FALSE)
    {
        printf("[%s][%d] eWindow =%d ---->eSource = %d is not Support\n",__FUNCTION__,__LINE__,eWindow, eSource);
    }
}

void HAL_XC_DIP_Averge2PTo1P(void *pInstance,MS_BOOL bAVG,SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif

    SC_DIP_SWDB_INFO DBreg;

    if(bAVG == TRUE)
    {
        Hal_SC_DWIN_get_sw_db(pInstance,&DBreg,eWindow);
        DBreg.u16H_CapStart/=2;
        if (DBreg.u16H_CapSize % 2 )
            DBreg.u16H_CapSize += 1;
        DBreg.u16H_CapSize/=2;
        if( (g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc/2) > g_DIPSrcInfo[eWindow].u16PreHCusScalingDst )
        {
            DBreg.u32H_PreScalingRatio = H_PreScalingDownRatioDIP(g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc/2,g_DIPSrcInfo[eWindow].u16PreHCusScalingDst);
            DBreg.u32H_PreScalingRatio &= 0x7FFFFFL;
            DBreg.u32H_PreScalingRatio |= (BIT(31));
        }
        else
            DBreg.u32H_PreScalingRatio = 0x100000L;
        Hal_SC_DWIN_sw_db(pInstance,&DBreg,eWindow);
    }

    if( eWindow == DIP_WINDOW )
    {
        if(bAVG == TRUE)
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_50_L, BIT(15),BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK34_6D_L, BIT(14),BIT(14));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_50_L, 0,BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK34_6D_L, 0,BIT(14));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        if(bAVG == TRUE)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_7A_L, BIT(15),BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK3B_79_L, BIT(14),BIT(14));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_7A_L, 0,BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK3B_79_L, 0,BIT(14));
        }

    }
    else if( eWindow == DWIN1_WINDOW )
    {
        if(bAVG == TRUE)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_7A_L, BIT(15),BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK3C_79_L, BIT(14),BIT(14));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_7A_L, 0,BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK3C_79_L, 0,BIT(14));
        }
    }
    else
    {
        return;
    }

}

void HAL_XC_DIP_SetWinProperty(void *pInstance, MS_U8 u8BufCnt,MS_U16 u16Width,MS_U16 u16LineOft,MS_U16 u16Height,MS_PHY u64OffSet, SCALER_DIP_SOURCE_TYPE eSource,MS_BOOL bPIP, MS_BOOL b2P_Enable, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif

    bPIPmode = bPIP;
    MS_U16 u16SrcWith = 0;
    MS_U16 u16MaxWith = 0;
    MS_BOOL bNeedAverageMode = FALSE;

    u16MaxWith = HAL_XC_DIP_GetWidth(pInstance, eSource, eWindow);

    bNeedAverageMode = IsNeedAverageMode(eSource, u16MaxWith);
    HAL_XC_DIP_Averge2PTo1P(pInstance, bNeedAverageMode, eWindow);

    if( eWindow == DIP_WINDOW )
    {
        DipWBufCnt = u8BufCnt;
        DipSource = eSource;
        u16SrcWith = DipSrcWidth;
        SC_W2BYTEMSK(0, REG_SC_BK36_01_L, u8BufCnt-1,BMASK(2:0));

        if(eSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //1: from dipr
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,BIT(6),BIT(6));
            //    dipr to dip
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,BIT(13),BMASK(13:12));
            //disable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,0,BIT(14));
        }
        else
        {
            //0: from IPMUX/OP1
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,0,BIT(6));
            //Monaco Mode DI input is 422
            if( ((eSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (eSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDipR2Y == FALSE) )
            {
                //enable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK34_6C_L,BIT(9),BIT(9));
                //enable  source yc swap
                SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,BIT(14),BIT(14));
            }
            else
            {
                //disable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK34_6C_L,0,BIT(9));
				//disable  source yc swap
            	SC_W2BYTEMSK(0, REG_SC_BK34_7C_L,0,BIT(14));
            }
        }

        if( (eSource == SCALER_DIP_SOURCE_TYPE_MAIN)||(eSource == SCALER_DIP_SOURCE_TYPE_SUB) )
		{
			SC_W2BYTEMSK(0,  REG_SC_BK34_7F_L, BIT(15), BMASK(15:14));
		}
        if( (bDIPRotation == TRUE) && (DIPRotation == eWindow) )
        {
            //width
            SC_W2BYTE(0, REG_SC_BK36_2F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK36_1F_L, u16Height);
        }
        else
        {
            //width
            SC_W2BYTE(0, REG_SC_BK36_1F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK36_2F_L, u16Height);
        }
        //pitch
        SC_W2BYTE(0, REG_SC_BK36_3F_L, u16LineOft);
        //buffer offset
        SC_W4BYTE(0, REG_SC_BK36_50_L,u64OffSet);

        if(eSource != SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE && eSource != SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE)
        {
            //reg_4k_h_size
            SC_W2BYTEMSK(0, REG_SC_BK34_6D_L, ((u16MaxWith+1)/2),BMASK(11:0));
            SC_W2BYTEMSK(0, REG_SC_BK34_6D_L, BIT(15),BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK34_67_L, 0x3 ,BMASK(1:0));
        }
        else
        {
            //reg_4k_h_size
            SC_W2BYTEMSK(0, REG_SC_BK34_6D_L, 0,BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK34_67_L, 0x2 ,BMASK(1:0));
        }
        //DIP Clk gating seletion and source select
        if(eSource == SCALER_DIP_SOURCE_TYPE_MAIN)
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L, BIT(8),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK34_67_L, 0,BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else if(eSource == SCALER_DIP_SOURCE_TYPE_SUB)
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L, BIT(7),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK34_67_L, BIT(2),BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L, 0x0,BMASK(8:7));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        Dwin0WBufCnt = u8BufCnt;
        Dwin0Source = eSource;
        u16SrcWith = Dwin0SrcWidth;
        SC_W2BYTEMSK(0, REG_SC_BK3B_01_L, u8BufCnt-1,BMASK(2:0));

        if(eSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //1: from dipr
            SC_W2BYTEMSK(0, REG_SC_BK3B_73_L,BIT(6),BIT(6));
            //    dipr to dwin0
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,0,BMASK(13:12));
            //disable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,0,BIT(8));
        }
        else
        {
            //0: from IPMUX/OP1
            SC_W2BYTEMSK(0, REG_SC_BK3B_73_L,0,BIT(6));
            //Monaco Mode DI input is 422
            if( ((eSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (eSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDwin0R2Y == FALSE) )
            {
                //enable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK3B_78_L,BIT(9),BIT(9));
                //enable  source yc swap
                SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,BIT(8),BIT(8));
            }
            else
            {
                //disable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK3B_78_L,0,BIT(9));
                //disable  source yc swap
                SC_W2BYTEMSK(0, REG_SC_BK3B_7F_L,0,BIT(8));
            }
        }

        if( (eSource == SCALER_DIP_SOURCE_TYPE_MAIN)||(eSource == SCALER_DIP_SOURCE_TYPE_SUB) )
        {
			SC_W2BYTEMSK(0,  REG_SC_BK3B_7F_L, BIT(5), BMASK(5:4));
        }

        if( (bDIPRotation == TRUE) && (DIPRotation == eWindow) )
        {
            //width
            SC_W2BYTE(0, REG_SC_BK3B_2F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK3B_1F_L, u16Height);
        }
        else
        {
            //width
            SC_W2BYTE(0, REG_SC_BK3B_1F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK3B_2F_L, u16Height);
        }
        //pitch
        SC_W2BYTE(0, REG_SC_BK3B_3F_L, u16LineOft);
        //buffer offset
        SC_W4BYTE(0, REG_SC_BK3B_50_L,u64OffSet);

        if(eSource != SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE && eSource != SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE)
        {
            //reg_4k_h_size
		    SC_W2BYTEMSK(0, REG_SC_BK3B_79_L, ((u16MaxWith+1)/2),BMASK(11:0));
            SC_W2BYTEMSK(0, REG_SC_BK3B_79_L, BIT(15),BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK3B_67_L, 0x3 ,BMASK(1:0));
        }
        else
        {
            //reg_4k_h_size
            SC_W2BYTEMSK(0, REG_SC_BK3B_79_L, 0x0,BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK3B_67_L, 0x2 ,BMASK(1:0));
        }
        //DWIN0 Clk gating seletion and source select
        if(eSource == SCALER_DIP_SOURCE_TYPE_MAIN)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_73_L, BIT(8),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK3B_67_L, 0,BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else if(eSource == SCALER_DIP_SOURCE_TYPE_SUB)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_73_L, BIT(7),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK3B_67_L, BIT(2),BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_73_L, 0x0,BMASK(8:7));
        }
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        Dwin1WBufCnt = u8BufCnt;
        Dwin1Source = eSource;
        u16SrcWith = Dwin1SrcWidth;
        SC_W2BYTEMSK(0, REG_SC_BK3C_01_L, u8BufCnt-1,BMASK(2:0));

        if(eSource == SCALER_DIP_SOURCE_TYPE_DRAM)
        {
            //1: from dipr
            SC_W2BYTEMSK(0, REG_SC_BK3C_73_L,BIT(6),BIT(6));
            //2. dipr to dwin1
            SC_W2BYTEMSK(0, REG_SC_BK34_7F_L,BIT(12),BMASK(13:12));
            //disable  source yc swap
            SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,0,BIT(8));
        }
        else
        {
            //0: from IPMUX/OP1
            SC_W2BYTEMSK(0, REG_SC_BK3C_73_L,0,BIT(6));
            //Monaco Mode DI input is 422
            if( ((eSource == SCALER_DIP_SOURCE_TYPE_OP_MAIN) || (eSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)) && (bDwin1R2Y == FALSE) )
            {
                //enable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK3C_78_L,BIT(9),BIT(9));
                //enable  source yc swap
                SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,BIT(8),BIT(8));
            }
            else
            {
                //disable  422 to 444
                SC_W2BYTEMSK(0, REG_SC_BK3C_78_L,0,BIT(9));
                //disable  source yc swap
                SC_W2BYTEMSK(0, REG_SC_BK3C_7F_L,0,BIT(8));
            }
        }

        if( (eSource == SCALER_DIP_SOURCE_TYPE_MAIN)||(eSource == SCALER_DIP_SOURCE_TYPE_SUB) )
        {
			SC_W2BYTEMSK(0,  REG_SC_BK3C_7F_L, BIT(5), BMASK(5:4));
        }

        if( (bDIPRotation == TRUE) && (DIPRotation == eWindow) )
        {
            //width
            SC_W2BYTE(0, REG_SC_BK3C_2F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK3C_1F_L, u16Height);
        }
        else
        {
            //width
            SC_W2BYTE(0, REG_SC_BK3C_1F_L, u16Width);
            //height
            SC_W2BYTE(0, REG_SC_BK3C_2F_L, u16Height);
        }
        //pitch
        SC_W2BYTE(0, REG_SC_BK3C_3F_L, u16LineOft);
        //buffer offset
        SC_W4BYTE(0, REG_SC_BK3C_50_L,u64OffSet);

        if(eSource != SCALER_DIP_SOURCE_TYPE_SC2_OP_CAPTURE && eSource != SCALER_DIP_SOURCE_TYPE_OP_SC1_CAPTURE)
        {
            //reg_4k_h_size
		    SC_W2BYTEMSK(0, REG_SC_BK3C_79_L, ((u16MaxWith+1)/2),BMASK(11:0));
	            SC_W2BYTEMSK(0, REG_SC_BK3C_79_L, BIT(15),BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK3C_67_L, 0x3 ,BMASK(1:0));
        }
        else
        {
            //reg_4k_h_size
            SC_W2BYTEMSK(0, REG_SC_BK3C_79_L, 0x0,BIT(15));
            //dip_op_pre_sel (2: sc2 3: sc)
            SC_W2BYTEMSK(0, REG_SC_BK3C_67_L, 0x2 ,BMASK(1:0));
        }
        //DWIN1 Clk gating seletion and source select
        if(eSource == SCALER_DIP_SOURCE_TYPE_MAIN)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_73_L, BIT(8),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK3C_67_L, 0,BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else if(eSource == SCALER_DIP_SOURCE_TYPE_SUB)
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_73_L, BIT(7),BMASK(8:7));
            SC_W2BYTEMSK(0, REG_SC_BK3C_67_L, BIT(2),BMASK(4:2));//0: ip_main    1: ip_sub    2: sc2_ip_main
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3C_73_L, 0x0,BMASK(8:7));
        }
    }

    //Rotation
    if( (bDIPRotation == TRUE) && (DIPRotation == eWindow) )
        SC_W2BYTEMSK(0, REG_SC_BK34_6E_L, (u16Height/0x10),BMASK(7:0));
}
void HAL_XC_DIP_SetWinProperty1(void *pInstance, MS_PHY u64OffSet,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK36_60_L,u64OffSet);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK3B_60_L,u64OffSet);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK3C_60_L,u64OffSet);
    }
    else
    {
        return;
    }
}
MS_U8 HAL_XC_DIP_GetBufCnt(void *pInstance, SCALER_DIP_WIN eWindow)
{
    MS_U8 u8BufCnt = 0;
    if( eWindow == DIP_WINDOW )
    {
        u8BufCnt = DipWBufCnt;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        u8BufCnt = Dwin0WBufCnt;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        u8BufCnt = Dwin1WBufCnt;
    }
    return (MS_U8) u8BufCnt;
}
void HAL_XC_DIP_SetBase0(void *pInstance,MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16BusSize = 0;


    u16BusSize = HAL_XC_DIP_GetBusSize(pInstance, eWindow);
    if( eWindow == DIP_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK36_10_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK36_30_L, u64BufEnd);     // input address0
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK3B_10_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK3B_30_L, u64BufEnd);     // input address0
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK3C_10_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK3C_30_L, u64BufEnd);     // input address0
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetBase1(void *pInstance, MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16BusSize = 0;


    u16BusSize = HAL_XC_DIP_GetBusSize(pInstance, eWindow);
    if( eWindow == DIP_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK36_20_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK36_40_L, u64BufEnd);     // input address0
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK3B_20_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK3B_40_L, u64BufEnd);     // input address0
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //low bound
        SC_W4BYTE(0, REG_SC_BK3C_20_L, u64BufStart / u16BusSize);     // input address0
        //high bound
        u64BufEnd /= u16BusSize;
        u64BufEnd|=(1<<DWIN_W_LIMITE_OFT);
        SC_W4BYTE(0, REG_SC_BK3C_40_L, u64BufEnd);     // input address0
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetMiuSel(void *pInstance, MS_U8 u8MIUSel,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        // Scaler control MIU by itself
        // So MIU1 IP-select is set to 1. At this moment, MIU0 Miu select is not working.
        MDrv_Write2ByteMask((REG_MIU0_BASE +(0x7D*2)), 0x00, BIT(9)); // MIU select (Group3 BIT1)
        MDrv_Write2ByteMask((REG_MIU1_BASE +(0x7D*2)), BIT(9), BIT(9)); // IP select
        if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_0 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, 0, BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK36_07_L, 0, BIT(15));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_1 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BIT(13), BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK36_07_L, 0, BIT(15));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_2 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, 0, BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK36_07_L, BIT(15), BIT(15));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BIT(13), BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK36_07_L, BIT(15), BIT(15));
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        // Scaler control MIU by itself
        // So MIU1 IP-select is set to 1. At this moment, MIU0 Miu select is not working.
        //MDrv_Write2ByteMask((REG_MIU0_BASE +(0x7D*2)), 0x00, BIT(13)); // MIU select (Group3 BIT1)
        //MDrv_Write2ByteMask((REG_MIU1_BASE +(0x7D*2)), BIT(13), BIT(13)); // IP select
        if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_0 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, 0, BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK3B_07_L, 0, BIT(15));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_1 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BIT(13), BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK3B_07_L, 0, BIT(15));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_2 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, 0, BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK3B_07_L, BIT(15), BIT(15));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BIT(13), BIT(13));
            SC_W2BYTEMSK(0, REG_SC_BK3B_07_L, BIT(15), BIT(15));
        }
    }
    else
    {
        //DIPR
        if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_0 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, 0, BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, 0, BIT(14));
            SC_W2BYTEMSK(0, REG_SC_BK36_76_L, 0, BMASK(14:13));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_1 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15), BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, 0, BIT(14));
            SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BIT(13), BMASK(14:13));
        }
        else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_2 )
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, 0, BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14), BIT(14));
            SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BIT(14), BMASK(14:13));
        }
        else
        {
            SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15), BIT(15));
            SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14), BIT(14));
            SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BMASK(14:13), BMASK(14:13));
        }
    }
}

void HAL_XC_DIP_SetDIPRMiuSel(void *pInstance, MS_U8 u8MIUSel,SCALER_DIP_WIN eWindow)
{
    //DIPR
    if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_0 )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, 0, BIT(15));
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, 0, BIT(14));
        SC_W2BYTEMSK(0, REG_SC_BK36_76_L, 0, BMASK(14:13));
    }
    else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_1 )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15), BIT(15));
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, 0, BIT(14));
        SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BIT(13), BMASK(14:13));
    }
    else if( ((CHIP_MIU_ID)u8MIUSel) == E_CHIP_MIU_2 )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, 0, BIT(15));
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14), BIT(14));
        SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BIT(14), BMASK(14:13));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15), BIT(15));
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14), BIT(14));
        SC_W2BYTEMSK(0, REG_SC_BK36_76_L, BMASK(14:13), BMASK(14:13));
    }

}

void HAL_XC_DIP_SetY2R(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        //enable y2r
        SC_W2BYTEMSK(0, REG_SC_BK36_7E_L,(bEnable?BIT(11):0),BIT(11));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //enable y2r
        SC_W2BYTEMSK(0, REG_SC_BK3B_7E_L,(bEnable?BIT(11):0),BIT(11));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //enable y2r
        SC_W2BYTEMSK(0, REG_SC_BK3C_7E_L,(bEnable?BIT(11):0),BIT(11));
    }
    else
    {
        return;
    }
}

void HAL_XC_DIP_SetAlphaValue(void *pInstance, MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_02_L, u8AlphaVal,BMASK(7:0));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, u8AlphaVal,BMASK(7:0));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, u8AlphaVal,BMASK(7:0));
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetUVSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    // do  if do Hmirror
    if( eWindow == DIP_WINDOW )
    {
        bDipUVSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK36_02_L, (bDipHMirror?(!bDipUVSwap?BIT(14):0):(bDipUVSwap?BIT(14):0)), BIT(14));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bDWIN0UVSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, (bDWIN0HMirror?(!bDWIN0UVSwap?BIT(14):0):(bDWIN0UVSwap?BIT(14):0)) ,BIT(14));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        bDWIN1UVSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, (bDWIN1HMirror?(!bDWIN1UVSwap?BIT(14):0):(bDWIN1UVSwap?BIT(14):0)) ,BIT(14));
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetYCSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        bDipYCSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK36_02_L, (bDipHMirror?(!bDipYCSwap?BIT(15):0):(bDipYCSwap?BIT(15):0)), BIT(15));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bDWIN0YCSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, (bDWIN0HMirror?(!bDWIN0YCSwap?BIT(15):0):(bDWIN0YCSwap?BIT(15):0)) ,BIT(15));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        bDWIN1YCSwap = bEnable;
    	SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, (bDWIN1HMirror?(!bDWIN1YCSwap?BIT(15):0):(bDWIN1YCSwap?BIT(15):0)) ,BIT(15));
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetRGBSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_7E_L, (bEnable?BIT(13):0) ,BIT(13));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_7E_L, (bEnable?BIT(13):0) ,BIT(13));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_7E_L, (bEnable?BIT(13):0) ,BIT(13));
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetOutputCapture(void *pInstance, MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow)
{
    MS_BOOL bCaptureEna;
    MS_U16 u16CaptureSrc;

    bCaptureEna = bEnable;
    switch(eOpCapture)
    {
        case E_XC_DIP_VOP2:  // compatible to previous IC, overlap is before osd blending
            u16CaptureSrc = 0;
            break;
        case E_XC_DIP_OP2:
            u16CaptureSrc = 1;
            break;
        case E_XC_DIP_VIP:
            u16CaptureSrc = 2;
            break;
        case E_XC_DIP_BRI:
            u16CaptureSrc = 3;
            break;
        default:
            u16CaptureSrc = 0;
            bCaptureEna = FALSE;
        break;
    }

    if(bCaptureEna)
    {
        SC_W2BYTEMSK(0, REG_SC_BK0F_57_L, BIT(11), BIT(11));  // Enable
        if( eWindow == DIP_WINDOW )
        {
            if(eOpCapture == E_XC_DIP_VOP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 0<<4, BMASK(6:4));
            else if(eOpCapture == E_XC_DIP_OP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 5<<4, BMASK(6:4));
            else
                printf("[DIP] OP capture source not support\n");
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            if(eOpCapture == E_XC_DIP_VOP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 0<<4, BMASK(6:4));
            else if(eOpCapture == E_XC_DIP_OP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 5<<4, BMASK(6:4));
            else
                printf("[DIP] OP capture source not support\n");
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            if(eOpCapture == E_XC_DIP_VOP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 0<<4, BMASK(6:4));
            else if(eOpCapture == E_XC_DIP_OP2)
                SC_W2BYTEMSK(0, REG_SC_BK2F_35_L, 5<<4, BMASK(6:4));
            else
                printf("[DIP] OP capture source not support\n");
        }
        else
        {
        return;
        }
    }
}

void HAL_XC_DIP_SetMirror(void *pInstance, MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow)
{
    EN_DRV_XC_DWIN_DATA_FMT fmt = XC_DWIN_DATA_FMT_MAX;
    // pixel format will swap if set Hmirror,so here do swap pixel format
    fmt = HAL_XC_DIP_GetDataFmt(pInstance, eWindow);
    if (  (XC_DWIN_DATA_FMT_YUV422 == fmt)
             ||(XC_DWIN_DATA_FMT_YUV420 == fmt))
    {
        if( eWindow == DIP_WINDOW )
        {
            bDipHMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, (bDipHMirror?(!bDipUVSwap?BIT(14):0):(bDipUVSwap?BIT(14):0)), BIT(14));
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            bDWIN0HMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, (bDWIN0HMirror?(!bDWIN0UVSwap?BIT(14):0):(bDWIN0UVSwap?BIT(14):0)), BIT(14));
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            bDWIN1HMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, (bDWIN1HMirror?(!bDWIN1UVSwap?BIT(14):0):(bDWIN1UVSwap?BIT(14):0)), BIT(14));
        }
    }
    else if (XC_DWIN_DATA_FMT_YC422 == fmt)
    {
        if( eWindow == DIP_WINDOW )
        {
            bDipHMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK36_02_L, (bDipHMirror?(!bDipYCSwap?BIT(15):0):(bDipYCSwap?BIT(15):0)), BIT(15));
        }
        else if( eWindow == DWIN0_WINDOW )
        {
            bDWIN0HMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, (bDWIN0HMirror?(!bDWIN0YCSwap?BIT(15):0):(bDWIN0YCSwap?BIT(15):0)), BIT(15));
        }
        else if( eWindow == DWIN1_WINDOW )
        {
            bDWIN1HMirror = bHMirror;
            SC_W2BYTEMSK(0, REG_SC_BK3C_02_L, (bDWIN1HMirror?(!bDWIN1YCSwap?BIT(15):0):(bDWIN1YCSwap?BIT(15):0)), BIT(15));
        }
    }

    if( eWindow == DIP_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L, (bHMirror?BIT(9):0) ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L, (bVMirror?BIT(10):0) ,BIT(10));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3B_0B_L, (bHMirror?BIT(9):0) ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK3B_0B_L, (bVMirror?BIT(10):0) ,BIT(10));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W2BYTEMSK(0, REG_SC_BK3C_0B_L, (bHMirror?BIT(9):0) ,BIT(9));
        SC_W2BYTEMSK(0, REG_SC_BK3C_0B_L, (bVMirror?BIT(10):0) ,BIT(10));
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_SetDIPRProperty(void *pInstance, ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow)
{
    MS_BOOL bYUV = TRUE;
    MS_U16 u16Fmt;
    DipRFmt = (EN_DRV_XC_DWIN_DATA_FMT)pstDIPRProperty->enDataFmt;
    switch ((EN_DRV_XC_DWIN_DATA_FMT)pstDIPRProperty->enDataFmt)
    {
        case XC_DWIN_DATA_FMT_YUV422 :
        case XC_DWIN_DATA_FMT_YC422 :
            u16Fmt = 0;
            break;
        case XC_DWIN_DATA_FMT_RGB565 :
            u16Fmt = 1;
            break;
        case XC_DWIN_DATA_FMT_ARGB8888 :
            u16Fmt = 2;
            break;
        case XC_DWIN_DATA_FMT_YUV420 :
        case XC_DWIN_DATA_FMT_YUV420_H265 :
        case XC_DWIN_DATA_FMT_YUV420_H265_10BITS :
            u16Fmt = 3;
            break;
        default :
            u16Fmt = 0x0;
            break;
    }

    //select 420 to 422
    if( (pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420) || (pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420_H265)|| (pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420_H265_10BITS))
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L,BIT(3),BIT(3));
    else
        SC_W2BYTEMSK(0, REG_SC_BK36_7C_L,0,BIT(3));
    if( ((pstDIPRProperty->enDataFmt) == DIP_DATA_FMT_RGB565) || ((pstDIPRProperty->enDataFmt) == DIP_DATA_FMT_ARGB8888))
        bYUV = FALSE;

    if( eWindow == DIP_WINDOW )
    {
        //select  422 to 444
        SC_W2BYTEMSK(0, REG_SC_BK34_6C_L,(bYUV?BIT(9):0),BIT(9));
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //select  422 to 444
        SC_W2BYTEMSK(0, REG_SC_BK3B_78_L,(bYUV?BIT(9):0),BIT(9));
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //select  422 to 444
        SC_W2BYTEMSK(0, REG_SC_BK3C_78_L,(bYUV?BIT(9):0),BIT(9));
    }

    if(bDIPRotation == FALSE)
    SC_W2BYTEMSK(0, REG_SC_BK34_6F_L,0x3,BIT(1)|BIT(0));

    //Y Buffer
    SC_W4BYTE(0, REG_SC_BK36_78_L, ((pstDIPRProperty->u32YBufAddr)/HAL_XC_DIP_GetBusSize(pInstance,eWindow)));
    //C Buffer
    SC_W4BYTE(0, REG_SC_BK36_7A_L, ((pstDIPRProperty->u32CBufAddr)/HAL_XC_DIP_GetBusSize(pInstance,eWindow)));
    //width
    SC_W2BYTE(0, REG_SC_BK36_27_L, (pstDIPRProperty->u16Width));
    //height
    SC_W2BYTE(0, REG_SC_BK36_37_L, (pstDIPRProperty->u16Height));
    //Line offset
    SC_W2BYTE(0, REG_SC_BK36_74_L, (pstDIPRProperty->u16Pitch));
    //Data Fmt
    SC_W2BYTEMSK(0, REG_SC_BK36_7D_L,(u16Fmt)<<6,BMASK(7:6));

    if(pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420_H265_10BITS)
    {
        //Y Buffer for EVD 10 bits
        SC_W4BYTE(0, REG_SC_BK36_12_L, ((pstDIPRProperty->u32YBufAddr10Bits)/HAL_XC_DIP_GetBusSize(pInstance,eWindow)));
        //C Buffer for EVD 10 bits
        SC_W4BYTE(0, REG_SC_BK36_14_L, ((pstDIPRProperty->u32CBufAddr10Bits)/HAL_XC_DIP_GetBusSize(pInstance,eWindow)));
        //Line offset for EVD 10 bits
        SC_W2BYTE(0, REG_SC_BK36_1D_L, (pstDIPRProperty->u16Pitch));
        //Enable DIPR H265 10 bits
        SC_W2BYTEMSK(0, REG_SC_BK36_17_L,BIT(15),BIT(15));
    }
    else
    {
        //Disable DIPR H265 10 bits
        SC_W2BYTEMSK(0, REG_SC_BK36_17_L,0x0,BIT(15));
    }
    if( (pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420_H265) || (pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420_H265_10BITS))
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,BIT(13),BIT(13));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0,BIT(13));
    }
    if(pstDIPRProperty->enDataFmt == DIP_DATA_FMT_YUV420)
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0C_L,BIT(6),BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0C_L,0,BIT(6));
    }
}

void HAL_XC_DIP_SetDIPRProperty_MFDEC(void *pInstance, ST_DIP_MFDEC_INFO stDIPR_MFDecInfo, SCALER_DIP_WIN eWindow)
{
}

void HAL_XC_DIP_Rotation(void *pInstance, MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,MS_PHY u64StartAddr,SCALER_DIP_WIN eTmpWindow)
{


}
void HAL_XC_DIP_SetPinpon(void *pInstance, MS_BOOL bPinpon,MS_PHY u64PinponAddr,MS_PHY u64OffSet,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16BusSize = 0;

    u16BusSize = HAL_XC_DIP_GetBusSize(pInstance, eWindow);
    if( eWindow == DIP_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK36_20_L, u64PinponAddr / u16BusSize);
        SC_W4BYTE(0, REG_SC_BK36_50_L,u64OffSet);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK3B_20_L, u64PinponAddr / u16BusSize);
        SC_W4BYTE(0, REG_SC_BK3B_50_L,u64OffSet);
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        SC_W4BYTE(0, REG_SC_BK3C_20_L, u64PinponAddr / u16BusSize);
        SC_W4BYTE(0, REG_SC_BK3C_50_L,u64OffSet);
    }
}
SCALER_DIP_WIN HAL_XC_DIP_GetHVSP(void* pInstance)
{
        return MAX_DIP_WINDOW;
}
MS_BOOL HAL_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow)
{
        return FALSE;
    }

MS_BOOL HAL_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16SAV_BW=0;
    if(eTileBlock == DIP_TILE_BLOCK_R_16_32)
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(13));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(15));
        u16SAV_BW = BIT(6);
    }
    else if(eTileBlock == DIP_TILE_BLOCK_R_32_16)
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,BIT(13),BIT(13));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(15));
    }
    else if(eTileBlock == DIP_TILE_BLOCK_R_32_32)
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(13));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,BIT(15),BIT(15));
    }
    else if( (eTileBlock == DIP_TILE_BLOCK_W_16_32) && (eWindow == DIP_WINDOW) )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(12));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(14));
    }
    else if( (eTileBlock == DIP_TILE_BLOCK_W_32_16) && (eWindow == DIP_WINDOW) )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,BIT(12),BIT(12));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(14));
    }
    else if( (eTileBlock == DIP_TILE_BLOCK_W_32_32) && (eWindow == DIP_WINDOW) )
    {
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,0x0,BIT(12));
        SC_W2BYTEMSK(0, REG_SC_BK36_0B_L,BIT(14),BIT(14));
    }
    else
        return FALSE;

    SC_W2BYTEMSK(0, REG_SC_BK36_0C_L,u16SAV_BW,BIT(6));

    return TRUE;
}

static void HAL_XC_DIP_CallBack(InterruptNum eIntNum)
{
    if( (SC_R2BYTEMSK(0, REG_SC_BK36_0A_L,BMASK(7:0)) > 0) && (pDipIntCb != NULL) )
        (void)(pDipIntCb)(eIntNum);
    if( (SC_R2BYTEMSK(0, REG_SC_BK3B_0A_L,BMASK(7:0)) > 0) && (pDwin0IntCb != NULL) )
        (void)(pDwin0IntCb)(eIntNum);
    if( (SC_R2BYTEMSK(0, REG_SC_BK3C_0A_L,BMASK(7:0)) > 0) && (pDwin1IntCb != NULL) )
        (void)(pDwin1IntCb)(eIntNum);

    MsOS_EnableInterrupt(E_INT_IRQ_DIPW);
}
void HAL_XC_DIP_InterruptAttach(void *pInstance, InterruptCb pIntCb,SCALER_DIP_WIN eWindow)
{
    if(bAttached == FALSE)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DIPW,HAL_XC_DIP_CallBack);
    }
    bAttached = TRUE;

    if( (bDipIsr == FALSE) && (bDwin0Isr == FALSE) && (bDwin1Isr == FALSE))
    {
        MsOS_EnableInterrupt(E_INT_IRQ_DIPW);
    }

    if( eWindow == DIP_WINDOW )
    {
        pDipIntCb = pIntCb;
        bDipIsr = TRUE;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        pDwin0IntCb = pIntCb;
        bDwin0Isr = TRUE;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        pDwin1IntCb = pIntCb;
        bDwin1Isr = TRUE;
    }
    else
    {
        return;
    }
}
void HAL_XC_DIP_InterruptDetach(void *pInstance, SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        bDipIsr = FALSE;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bDwin0Isr = FALSE;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
    	bDwin1Isr = FALSE;
    }
    else
    {
    	return;
    }

    if( (bDipIsr == FALSE) && (bDwin0Isr == FALSE) && (bDwin1Isr == FALSE))
    {
    MsOS_DisableInterrupt(E_INT_IRQ_DIPW);
    //MsOS_DetachInterrupt(E_INT_IRQ_DIPW); // do not deattach, because deattach will take 100 ms to wait pthread join
    }
}

void HAL_XC_DIP_2P_Width_Check(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    MS_U8  u8bpp = 2;
    MS_U16 u16alignment = 16;
    MS_U16 u16tmp = 0;
    MS_U16 u16MaxWith = 0;
    MS_BOOL bNeedAverageMode = FALSE;

    u16MaxWith = HAL_XC_DIP_GetWidth(pInstance, g_DIPSrcInfo[eWindow].enDIPSourceType, eWindow);
    bNeedAverageMode = IsNeedAverageMode(g_DIPSrcInfo[eWindow].enDIPSourceType, u16MaxWith);

    if(bNeedAverageMode)
    {
        if((pstXC_SetWin_Info->u16PreHCusScalingDst) > (pstXC_SetWin_Info->u16PreHCusScalingSrc/2))
        {
            pstXC_SetWin_Info->u16PreHCusScalingDst = pstXC_SetWin_Info->u16PreHCusScalingSrc/2;
        }
    }

    if(HAL_XC_DIP_GetBusSize(pInstance,eWindow) == DIP_BYTE_PER_WORD)
    {
        u16alignment=DIP_BYTE_PER_WORD/u8bpp;
    }
    else
    {
        u16alignment=DWIN_BYTE_PER_WORD/u8bpp;
    }
    u16tmp=(pstXC_SetWin_Info->u16PreHCusScalingDst)/u16alignment;
    pstXC_SetWin_Info->u16PreHCusScalingDst= u16tmp*u16alignment;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void HAL_XC_DIP_Check_Clock(MS_U64* u64ReqHdl,SCALER_DIP_WIN eWindow)
{
    char string_DIP_Clock[12];
    char DIP_345M[] = "345M";
    char DIP_320M[] = "320M";
    char DIP_216M[] = "216M";
    char DIP_192M[] = "192M";
    char DIP_170M[] = "170M";
    char DIP_160M[] = "160M";
    char DIP_NotDefine[] = "Not Define";
    MS_U16 u16tmp=0;

    //DIP clock
    u16tmp = MDrv_ReadByte(REG_CKG_FCLK)&CKG_FCLK_MASK;
    if(u16tmp == CKG_FCLK_345MHZ)
    {
        strcpy(string_DIP_Clock, DIP_345M);
    }
    else if(u16tmp == CKG_FCLK_320MHZ)
    {
        strcpy(string_DIP_Clock, DIP_320M);
    }
    else if(u16tmp == CKG_FCLK_216MHZ)
    {
        strcpy(string_DIP_Clock, DIP_216M);
    }
    else if(u16tmp == CKG_FCLK_192MHZ)
    {
        strcpy(string_DIP_Clock, DIP_192M);
    }
    else if(u16tmp == CKG_FCLK_170MHZ)
    {
        strcpy(string_DIP_Clock, DIP_170M);
    }
    else if(u16tmp == CKG_FCLK_160MHZ)
    {
        strcpy(string_DIP_Clock, DIP_160M);
    }
    else
    {
        strcpy(string_DIP_Clock, DIP_NotDefine);
    }
    MdbPrint(u64ReqHdl,"DIP Clock:%s\n",string_DIP_Clock);

}

void HAL_XC_DIP_Check_Scale(MS_U64* u64ReqHdl,MS_U16 *u16H_Scaling_Enable, MS_U32 *u32H_Scaling_Ratio, MS_U16 *u16V_Scaling_Enable, MS_U32 *u32V_Scaling_Ratio,SCALER_DIP_WIN eWindow)
{
    if( eWindow == DIP_WINDOW )
    {
        //HV scaling enable/ratio
        *u16H_Scaling_Enable = (SC_R4BYTE(0, REG_SC_BK34_04_L) & BIT(31)) >> 31;
        *u32H_Scaling_Ratio =  SC_R4BYTE(0, REG_SC_BK34_04_L) & BMASK(22:0);
        *u16V_Scaling_Enable = (SC_R4BYTE(0, REG_SC_BK34_08_L) & BIT(31)) >> 31;
        *u32V_Scaling_Ratio =  SC_R4BYTE(0, REG_SC_BK34_08_L) & BMASK(22:0);
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //HV scaling enable/ratio
        *u16V_Scaling_Enable = (SC_R4BYTE(0, REG_SC_BK3B_15_L) & BIT(31)) >> 31;
        *u32H_Scaling_Ratio =  SC_R4BYTE(0, REG_SC_BK3B_15_L) & BMASK(22:0);
        *u16V_Scaling_Enable = (SC_R4BYTE(0, REG_SC_BK3B_1B_L) & BIT(31)) >> 31;
        *u32V_Scaling_Ratio =  SC_R4BYTE(0, REG_SC_BK3B_1B_L) & BMASK(22:0);
    }
}
#endif

void HAL_XC_DIP_Enable(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
#ifdef MSOS_TYPE_OPTEE
        MDrv_WriteRegBit(REG_TZPC_NONPM_DIP, 0, BMASK(1:0));
        MDrv_WriteRegBit(REG_TZPC_NONPM_DWIN0, 0, BMASK(9:8));
#endif
    }
    else
    {
#ifdef MSOS_TYPE_OPTEE
        MDrv_WriteRegBit(REG_TZPC_NONPM_DIP, BMASK(1:0), BMASK(1:0));
        MDrv_WriteRegBit(REG_TZPC_NONPM_DWIN0, BMASK(9:8), BMASK(9:8));
#endif
        SC_W2BYTEMSK(0, REG_SC_BK36_02_L, BIT(10) , BIT(10));
        SC_W2BYTEMSK(0, REG_SC_BK3B_02_L, BIT(10) , BIT(10));
    }
}

//=============== DIP =====================//

#undef  MHAL_SC_C

