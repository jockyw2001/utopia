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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mdrv_ldalgo.c
/// @brief  local dimming algorithm
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_LDALGO_C_

#include "MsCommon.h"
#include "MsOS.h"
#include "utopia_dapi.h"
#include "mhal_xc_chip_config.h"
#include "hwreg_sc.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
// Internal Definition
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#include "mdrv_ld_gamma.h"

#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"



#if  (LD_ENABLE == 1)
//--------------------------------------------------------
//--------Local dimmming macro define---------------------
//--------------------------------------------------------
#define MAX_AVG_BLENDING                1
#define DIRECTTYPE_PATCH_ENABLE         0
#define ADD_OPPOSITE_LUMA               1

#define LUMA_AVG_DIFF_THRESHOLD1        0x38
#define LUMA_AVG_DIFF_THRESHOLD2        0x28
#define BIN_WIDTH                       16

#ifndef MIN
#define MIN(a,b)                 (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)                 (((a) > (b)) ? (a) : (b))
#endif

#define ABS(x)                    (((x) > 0) ? (x) : (-(x)))
#define DIFF(a,b)                (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
#define MINMAX(v,a,b)             (((v) < (a)) ? (a) : ((v) > (b)) ? (b) : (v))

#ifndef UNUSED
#define UNUSED(x)                 ((x) = (x))
#endif

//----------------------------------------------------------
//--------Local dimmming Local variables--------------------
//----------------------------------------------------------
#if 0 //
static Enu_LD_BLOCK_NUM s_a_LDBlockTBL[]={
 LD_BLOCK_10,   //LD_PANEL_LG32inch_LR10
 LD_BLOCK_10,   //LD_PANEL_LG37inch_LR10
 LD_BLOCK_16,   //LD_PANEL_LG42inch_LR16
 LD_BLOCK_16,   //LD_PANEL_LG47inch_LR16
 LD_BLOCK_16,   //LD_PANEL_LG55inch_LR16
 LD_BLOCK_TSB,   //LD_PANEL_TOSHIBA_LR16
 LD_BLOCK_TSB,   //LD_PANEL_TOSHIBA_TB16
 LD_BLOCK_DEFALUT, //===Empty===
 LD_BLOCK_32,   //LD_PANEL_LG50inchCinema_TB32
 LD_BLOCK_32,   //LD_PANEL_LG60inchCinema_TB32
 LD_BLOCK_72,   //LD_PANEL_LG47inch_Direct72
 LD_BLOCK_96,   //LD_PANEL_LG55inch_Direct96
 LD_BLOCK_480,  //LD_PANEL_LG72inch_Direct480
};
#endif


//-------------------------------------------------------------
//--------Local dimmming internal interface--------------------
//-------------------------------------------------------------

void MDrv_appLDModeHandler_R2(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._stLDInfo = MDrv_LD_GetLDInfo(pInstance);
    //MFC_3D_FORMAT e3DMode = MDrv_SysInfo_GetInternal3DMode();
    //Eun_LD_PANEL_TYPE eLDPanelType =  MDrv_SysInfo_GetPanelLDType();
    pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num = LD_BLOCK_16;///s_a_LDBlockTBL[eLDPanelType];

    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumV * pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LedNumH;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize = pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightH * pXCResourcePrivate->stdrv_LDALGO._stLDInfo.u8LD_BackLightV;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D = 0;
    pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D_tmax = 0;
}

MS_U32 MDrv_Temporal_Max(void *pInstance, MS_BOOL bLR_Flag) //NO_USE
{
    MS_U8 u8MaxTemp1 = 0;
    MS_U8 u8MaxTemp2 = 0;
    MS_U8 u8MaxTemp3 = 0;
    MS_U8 i = 0;
    MS_U8 j = 0;
    MS_U32 u32Luma_Sum = 0;
    MS_U32 u32Luma_Avg = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for (i = 0; i < BIN_WIDTH; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] = 0;
    }

    if (bLR_Flag)
    {
        for (j = 0 ; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8MaxTemp1 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
                pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block_Pre[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
            }
        }
    }
    else
    {
        pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero = 0;
        for (j = 0 ; j <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0 ; i <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8MaxTemp1 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
                u8MaxTemp2 = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block_Pre[j][i];
                u8MaxTemp3 = MAX(u8MaxTemp1, u8MaxTemp2);
                pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] = u8MaxTemp3;

                pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/BIN_WIDTH)]++;
                if(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] == 0)
                {
                    pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero++;
                }
            }
        }
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize == 0)
        {
            return 0;
        }
        else
        {
            return u32Luma_Avg = u32Luma_Sum/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize;
        }
    }

    return 0;
}



MS_U32 MDrv_FramLockBffdepack_CSep_8B_Full(void *pInstance)      // depack per 4 pixel    = 128bit = 16 bytes
{
    MS_U32 u32Ioffset = 0;           // Byte index of packed data
    MS_U8 u8Bpixel = 0, u8Rpixel = 0;
    MS_U32 u32Luma_Avg = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LDWidth_Input = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth;
    MS_U8 u8LDHeight_Input = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight;
    MS_U8 u8MaxTemp1 = 0;

#if (MAX_AVG_BLENDING == 0)
    MS_U32 u32Block_Avg = 0;
    MS_U32 u32Block_Max = 0;
    //int block_alpha=0;
    MS_U32 u32Block_Alpha_Tmp = 0;
    //long u32Frame_Avg_Sum,u32Frame_Avg;
    MS_U8 u8Block_Avg_Thrd_High = 50;
    MS_U8 u8Block_Avg_Thrd_Low  = 34;
#endif
    pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum = 0;

    for (u8Rpixel = 0; u8Rpixel < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight; u8Rpixel++)
    {
        for (u8Bpixel = 0; u8Bpixel < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth; u8Bpixel++)
        {
            #if (MAX_AVG_BLENDING)
            if (pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0)
            {
                u8MaxTemp1 = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD*u8Rpixel);
            }
            else
            {
                u8MaxTemp1 = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD*u8Rpixel);
            }
            pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[u8Rpixel][u8Bpixel] = u8MaxTemp1;
            #else
            {
                //u32Block_Avg   = readb(BANK_MIU+Ipack+8*(u8Bpixel+_stLDPanelPara.u8LDWidth*u8Rpixel)+FramLockMbase);
                //u32Block_Max   = readb(BANK_MIU+Ipack+8*(u8Bpixel+_stLDPanelPara.u8LDWidth*u8Rpixel)+4+FramLockMbase);
                //block_alpha = readb(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x1F)) & 0xF;
                if (pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0)
                {
                    u32Block_Avg = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel));
                    u32Block_Max = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_1, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel) + 4);
                }
                else
                {
                    u32Block_Avg = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel));
                    u32Block_Max = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, 8*(u8Bpixel + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth * u8Rpixel) + 4);
                }

                if ( ABS(u32Block_Max-u32Block_Avg) >= u8Block_Avg_Thrd_High )
                {
                    u8MaxTemp1 = (u32Block_Max * 16 + u32Block_Avg * 0)/16;
                }
                else if ( (ABS(u32Block_Max-u32Block_Avg) < u8Block_Avg_Thrd_High) && (ABS(u32Block_Max-u32Block_Avg) >= u8Block_Avg_Thrd_Low) )
                {
                    u32Block_Alpha_Tmp = MAX(ABS(u32Block_Max - u32Block_Avg) - 34, 0);
                    u8MaxTemp1 = (u32Block_Max * u32Block_Alpha_Tmp + u32Block_Avg * (16 - u32Block_Alpha_Tmp))/16;
                }
                else
                {
                    u8MaxTemp1 = (u32Block_Max * 0 + u32Block_Avg * 16)/16;
                }

                pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[u8Rpixel][u8Bpixel] = u8MaxTemp1;
            }
            #endif
            pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum += u8MaxTemp1;
            u32Ioffset++;
        }
    }

    if ( (u8LDHeight_Input == 0) || (u8LDWidth_Input == 0) )
        return pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum;
    else
        return u32Luma_Avg = pXCResourcePrivate->stdrv_LDALGO._u32Lluma_Sum/(u8LDHeight_Input * u8LDWidth_Input);
}

MS_BOOL  MDrv_LD_Get_Value(void *pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize )
{
    MS_U8 u8Bpixel = 0, u8Rpixel = 0;
    MS_U16 i=0;
    MS_U32 u32HScalingDownRatio=0, u32VScalingDownRatio=0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_WINDOW_TYPE DEWin;
    Hal_SC_get_disp_de_window(pInstance, &DEWin);

    // boundary of weight / height
    if((u8WSize > 32 || u8WSize < 1) || (u8HSize > 18 || u8HSize < 1))
    {
        return FALSE;
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, 0 ,0xFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, 0 ,0xFF);

    //Calculate [9:5]Height [4:0]Width
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, ((MS_U16)(u8HSize-1) << 5 |(u8WSize-1))   , 0x3FF);

    if((DEWin.width <= 3000)&&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC))
    {
        //When FRC case input source is FHD. The NAPOLI output is 2k2k to FRC.
        u32HScalingDownRatio = ((MS_U32)u8WSize * 1024 *1024*2 /pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width) +1 ;
    }
    else
    {
        //input horizontal scaling down ratio = (W_ledx1024x1024/W_lcd)+1
        u32HScalingDownRatio = ((MS_U32)u8WSize * 1024 *1024 /pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width) +1 ;
    }

    //input vertical scaling down ratio = (H_ledx1024x1024/H_lcd)+1
    u32VScalingDownRatio = ((MS_U32)u8HSize * 1024 *1024 /pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height) +1 ;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L, (MS_U16)(u32HScalingDownRatio&0xFFFF) ,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1A_L, (MS_U16)((u32HScalingDownRatio>> 16)&0xF) ,0xF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L, (MS_U16)(u32VScalingDownRatio&0xFFFF)  ,0xFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1C_L, (MS_U16)((u32VScalingDownRatio>> 16)&0xF) ,0xF);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x8 ,0xF);

    for (u8Rpixel = 0; u8Rpixel < u8HSize; u8Rpixel++)
    {
        for (u8Bpixel = 0; u8Bpixel < u8WSize; u8Bpixel++)
        {
            pu8LDValue[i] = MDrv_LD_MIUReadByte(LD_BUF_ReadDBuf_0, u8Bpixel + u8WSize*u8Rpixel);
            i++;
        }
    }
    return TRUE;

}

void MDrv_Output_SPIdata(void *pInstance)
{
    MS_U8 i = 0,j = 0;
    MS_U8 u8Tmp = 0;
    #if 0 //for debug printf
    MS_U8 h,v;
    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32WriteBuf = ((pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0) ? LD_BUF_SPIBuf_0 : LD_BUF_SPIBuf_1);
    MS_U8 u8Min_BLight_value = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, 0x00FF);//readb(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0xEC/2)) & 0xFF;

    MS_U32 u32Frame_Avg_Sum = 0,u32Frame_Avg = 0;

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            u8Tmp = pXCResourcePrivate->stdrv_LDALGO._u8LED_resort[i + j * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth];
            pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i] = MAX(u8Tmp, u8Min_BLight_value);
            u32Frame_Avg_Sum += pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i];
            pXCResourcePrivate->stdrv_LDALGO._u8LD_AlgoSPIBuffer[j * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth + i] = pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i];
        }
    }

    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh % 2) // odd patch
        {
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, 0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, 0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//8
        }
        else
        {
            if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 6)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//3
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//9
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//7
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//10
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//8
            }
            else
            {
                if (Mdrv_LD_GetPanelType(pInstance) == E_LD_PANEL_CMO42inch_LR16)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);
                }
                else
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 + pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);
                }
            }
        }
    }
    else
    {
        for (j = 0 ; j <  pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, i + j*pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);
            }
        }
    }

    u32Frame_Avg = (u32Frame_Avg_Sum + (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize/2) )/(pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize);

    if (REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_PWM_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt + 1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20 + pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);//1
            }
        }
    }
    #if  0
    for( v = 0; v < 6; ++ v )
    {
        for( h = 0; h < 2; ++ h )
        {
            printf("0x%02X,", _u8LD_AlgoSPIBuffer[i] );
            i += 1;
        }
        printf("\n" );
    }
    #endif
    MDrv_LD_SetLocalDimmingData(pInstance, pXCResourcePrivate->stdrv_LDALGO._u8LD_AlgoSPIBuffer, (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh * pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth));
}


void MDrv_Output_DigitalBLdata(void *pInstance)
{
    MS_U8 i = 0,j = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32WriteBuf = ((pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final == 0) ? LD_BUF_WriteBuf_0 : LD_BUF_WriteBuf_1);

    if ( pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh % 2) // odd patch
        {
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
            MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//2
            MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
            MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,0);
            MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
            MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//8
        }
        else
        {
            #if 0
            for(i = 0;i<_stLDPanelPara.u8PWMWidth;i++)
            {
                for(j=0 ; j< _stLDPanelPara.u8PWMHigh; j++)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, j+i*_u8pack_length_PWM ,_u8PWM_Block[j][i]);
                }
            }
            #endif
            if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 6)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//3
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);//9
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//7
                MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//2
                MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//4
                MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);
                MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//10
                MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);//8
            }
            else
            {
                if (Mdrv_LD_GetPanelType(pInstance) == E_LD_PANEL_CMO42inch_LR16)
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]); //1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]); //2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]); //2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]); //4

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//10

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);//10

                }
                else
                {
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][0]);//1
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][0]);//2
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][0]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][0]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][0]);//3
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7 ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][0]);

                    MDrv_LD_MIUWriteByte(u32WriteBuf, 0+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[0][1]);//4
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 1+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[1][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 2+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[2][1]);//9
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 3+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[3][1]);//10
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 4+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[4][1]);//7
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 5+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[5][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 6+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[6][1]);
                    MDrv_LD_MIUWriteByte(u32WriteBuf, 7+pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM ,pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[7][1]);
                }
            }
        }
    }
    else
    {
        for (j = 0 ; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth;i++)
            {
                MDrv_LD_MIUWriteByte(u32WriteBuf, i + j * pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM, pXCResourcePrivate->stdrv_LDALGO._u8PWM_Block[j][i]);
            }
        }
    }
}


void MDrv_LD_3Ddownsample(void *pInstance, MS_U8 u8ModeSelection_3D)
{
    MS_U8 i = 0,j = 0;
    MS_U8 u8MaxL = 0,u8MaxR = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch (u8ModeSelection_3D)
    {
        case 0:
        default:
            return;

        case 1:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2); i++)
                {
                    u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                    u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2];
                    pXCResourcePrivate->stdrv_LDALGO._u8SBSDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                }
            }
            break;

        case 2:
            if ( pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num != LD_BLOCK_480)
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                    }
                }
            }
            else
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight+1)/2; j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, u8MaxR);
                    }
                }
            }
            break;

        case 3:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2); i++)
                {
                    u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                    u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/2];
                    pXCResourcePrivate->stdrv_LDALGO._u8SBSDown_Block[j][i] = MAX(u8MaxL, 0);
                }
            }
            break;

        case 4:
            if (pXCResourcePrivate->stdrv_LDALGO._enLDBlock_Num != LD_BLOCK_480)
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, 0);
                    }
                }
            }
            else
            {
                for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight+1)/2; j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i++)
                    {
                        u8MaxL = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i];
                        u8MaxR = pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j + pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/2][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8TBDown_Block[j][i] = MAX(u8MaxL, 0);
                    }
                }
            }
            break;
    }
}

void MDrv_Get_Histogram(void *pInstance)
{
    MS_U8 i = 0,j = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero = 0;

    for (i = 0; i < BIN_WIDTH; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] = 0;
    }


    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight; j++)
    {
        for (i = 0 ; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[(pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i]/BIN_WIDTH)]++;
            if (pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i] < 3)
            {
                pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero++;
            }
        }
    }
}

void MDrv_BLMxNto2xN_White_8B(void *pInstance)
{
    MS_U8 i = 0, j = 0;
    MS_U8 u8Max = 0, u8Itemp = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_Get_Histogram(pInstance);

    MDrv_LD_3Ddownsample(pInstance, pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D);

    switch (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D)
    {
        case 0:
        default:
            for (j = 0 ; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j++)
            {
                for (i = 0 ; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth); i=i+pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio )
                {
                    u8Max = 0;
                    for (u8Itemp = 0 ; u8Itemp < pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio; u8Itemp++)
                    {
                        u8Max = MAX(u8Max,pXCResourcePrivate->stdrv_LDALGO._u8Input_Block[j][i+u8Itemp]);
                    }
                    if (pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio > 0)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i/pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio] = u8Max;
                    }
                }
            }
            break;

#if 0   //A5 no need 3D scaling
        case 1:
        case 3:
            for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
            {
                for (i = 0 ; i < (_stLDPanelPara.u8LDWidth/2); i=i+_u8H_Down_Ratio_3D)
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_Down_Ratio_3D ; u8Itemp++)
                        u8Max = MAX(u8Max,_u8SBSdown_Block[j][i+u8Itemp]);
                    _u8Hdown_Block[j][i/_u8H_Down_Ratio_3D] = u8Max;
                }
            }
            break;

        case 2:
        case 4:
            for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh/2); j++)
            {
                for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio)
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                        u8Max = MAX(u8Max,_u8TBdown_Block[j][i+u8Itemp]);

                    _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
                }
            }
            break;

        default:
            break;
#endif
    }

}


void MDrv_BL2xNto2xNd3_White_8B(void *pInstance)
{
    MS_U8 i =0,j = 0;
    MS_U8 u8Max = 0, u8Itemp = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LD_Mode2D3D)
    {
        case 0:
        case 1:
        case 3:
        default:
            for (j = 0; j < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight); j = j + pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio )
            {
                for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                {
                    u8Max = 0;
                    for (u8Itemp = 0; u8Itemp < pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio ; u8Itemp++)
                    {
                        u8Max = MAX(u8Max, pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j+u8Itemp][i]);
                    }
                    if (pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio > 0)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j/pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio][i] = u8Max;
                    }
                }
            }
            break;

        case 2:
        case 4:
            if (pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D == 0)
            {
                for (j = 0; j <  (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh/2); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                    {
                        u8Max = pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i];
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j*2][i] = u8Max;
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j*2+1][i] = u8Max;
                    }
                }
            }
            else if (pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D == 1)
            {
                for (j = 0; j <  (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh); j++)
                {
                    for (i = 0; i < (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth); i++)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8HDown_Block[j][i];
                    }
                }
            }
            break;
    }

    if (REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_VDown_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt+1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20 + pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i , pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]);//1
            }
        }
    }

}

#if 0
void MDrv_BLMxNto2xN_White_8B_480block(void)
{
    int i, j, u8Itemp;
    int u8Max;

    MDrv_Get_histogram();

    MDrv_LD_3Ddownsample(_stLDPanelPara.u8LD_Mode2D3D);

    if(_stLDPanelPara.u8LD_Mode2D3D==0 )
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio )
                {
                    u8Max = 0 ;
                    for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                    u8Max = MAX(u8Max,_u8Input_Block[j][i+u8Itemp]);
                    _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
                }
        }
    }
    else if(_stLDPanelPara.u8LD_Mode2D3D==1 || _stLDPanelPara.u8LD_Mode2D3D==3)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth/2); i++)
            {
                u8Max = _u8SBSdown_Block[j][i];
                _u8Hdown_Block[j][i*2] = u8Max;
                _u8Hdown_Block[j][i*2+1] = u8Max;
            }
        }
    }
    else if(_stLDPanelPara.u8LD_Mode2D3D==2 || _stLDPanelPara.u8LD_Mode2D3D==4)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh+1)/2 ; j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8LDWidth); i=i+_u8H_down_ratio)
            {
                u8Max = 0 ;
                for (u8Itemp = 0 ; u8Itemp < _u8H_down_ratio ; u8Itemp++)
                u8Max = MAX(u8Max,_u8TBdown_Block[j][i+u8Itemp]);
                _u8Hdown_Block[j][i/_u8H_down_ratio] = u8Max;
            }
        }
    }
}


void MDrv_BL2xNto2xNd3_White_8B_480block(void)
{
    int j, i;
    int u8Max;
    int u8Itemp;
    if( _stLDPanelPara.u8LD_Mode2D3D==0 || _stLDPanelPara.u8LD_Mode2D3D==1  || _stLDPanelPara.u8LD_Mode2D3D==3)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j=j+_u8V_down_ratio )
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth); i++)
            {
                u8Max = 0;
                for (u8Itemp = 0 ; u8Itemp < _u8V_down_ratio ; u8Itemp++)
                {
                    u8Max = MAX(u8Max,_u8Hdown_Block[j+u8Itemp][i]);
                }
                _u8Vdown_Block[j/_u8V_down_ratio][i] = u8Max;

            }
        }
    }
    else if( _stLDPanelPara.u8LD_Mode2D3D==2 ||  _stLDPanelPara.u8LD_Mode2D3D==4)
    {
        for (j = 0 ; j <  (_stLDPanelPara.u8LDHigh); j++)
        {
            for (i = 0 ; i < (_stLDPanelPara.u8PWMWidth); i++)
            {
                u8Max = MAX ( _u8Hdown_Block[j/2][i],_u8Hdown_Block[MAX(j-(j/2),0)][i] );
                _u8Vdown_Block[j][i] = u8Max;
            }
        }
    }
}
#endif

MS_U8 MDrv_Spatial_Filtering1D_Large(void *pInstance, MS_U8 x_index, MS_U8 y_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 u8Tmp_Value_Adj[5];
    MS_S8  x = 0,y = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        for(y = -2; y < 3; y++)
        {
            if( ( (y+y_index >= 0)  && (y+y_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) )	 )
            {
                u8Tmp_Value_Adj[y+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y+y_index][x_index];
            }
            else
            {
                u8Tmp_Value_Adj[y+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x_index];
            }
        }
    }
    else
    {
        for(x = -2; x < 3; x++)
        {
            if( ( (x+x_index >= 0)  && (x+x_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh ) )  )
            {
                u8Tmp_Value_Adj[x+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x+x_index];
            }
            else
            {
                u8Tmp_Value_Adj[x+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[y_index][x_index];
            }
        }
    }
    for(x = -2; x < 3; x++)
    {
        if(u8Tmp_Value_Adj[x+2] >= u8Max_Value)
        {
            u8Max_Value = u8Tmp_Value_Adj[x+2];
        }
    }
    if(u8Tmp_Value_Adj[2] == u8Max_Value )
    {
        return u8Max_Value;
    }
    else if(u8Tmp_Value_Adj[1] == u8Max_Value || u8Tmp_Value_Adj[3] == u8Max_Value)
    {
        if(u8Tmp_Value_Adj[2]>= MAX ( u8Max_Value -u8Spatial_Strength,0) )
        {
            return u8Tmp_Value_Adj[2];
        }
        else
        {
            return MAX( u8Max_Value - u8Spatial_Strength, 0);
        }
    }
    else if(u8Tmp_Value_Adj[0] == u8Max_Value || u8Tmp_Value_Adj[4] == u8Max_Value)
    {
        if(u8Tmp_Value_Adj[2] >= MAX ( u8Max_Value - 2*u8Spatial_Strength, 0) )
        {
            return u8Tmp_Value_Adj[2];
        }
        else
        {
            return MAX ( u8Max_Value - 2* u8Spatial_Strength,0);
        }
    }
	return 0;
}

MS_U8 MDrv_Spatial_Filtering2D(void *pInstance, MS_U8 i_index, MS_U8 j_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 au8Tmp_Value_Adj[5][5];
    MS_U8 u8Max_Value_Array = 0, u8Max_Value_Array1 = 0, u8Max_Value_Array2 = 0;
    MS_U8 u8Loop_Circle = 2;
    MS_S8 i = 0,j = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    for (j = -2; j < 3; j++)
    {
        for (i = -2; i < 3; i++)
        {
            if (( (i+i_index >= 0)  && (i + i_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth ) )
                && ( (j+j_index >= 0)  && (j+j_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) ))
            {
                au8Tmp_Value_Adj[j+2][i+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j+j_index][i+i_index];
            }
            else
            {
                au8Tmp_Value_Adj[j+2][i+2] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }

    for (j = -2;j < 3; j++)
    {
        for ( i =-2; i < 3; i++)
        {
            if ( (au8Tmp_Value_Adj[j+2][i+2] > u8Max_Value_Array2) && (MAX( ABS(i),ABS(j)) == 2) )
            {
                u8Max_Value_Array2 = au8Tmp_Value_Adj[j+2][i+2];
                //u8Loop_Circle = 2;
            }
            if ( (au8Tmp_Value_Adj[j+2][i+2] > u8Max_Value_Array1) && (MAX( ABS(i),ABS(j)) == 1) )
            {
                u8Max_Value_Array1 = au8Tmp_Value_Adj[j+2][i+2];
                // u8Loop_Circle = 2;
            }
        }
    }

    if (u8Max_Value_Array1 >= u8Max_Value_Array2)
    {
        u8Loop_Circle = 1;
        u8Max_Value_Array = u8Max_Value_Array1;
    }
    else
    {
        u8Loop_Circle = 2;
        u8Max_Value_Array = u8Max_Value_Array2;
    }

    if ( au8Tmp_Value_Adj[2][2] >= u8Max_Value_Array)
    {
        u8Max_Value = au8Tmp_Value_Adj[2][2];
    }
    else
    {
        if (u8Loop_Circle == 2)
        {
            if (( au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = au8Tmp_Value_Adj[2][2];
            }
            else if ( ( au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 );
            }
            else if ( ( au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 );
            }
            else if ( ( au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array1- u8Spatial_Strength, 0 )) && (au8Tmp_Value_Adj[2][2] < MAX( u8Max_Value_Array2- u8Spatial_Strength*2, 0 )) )
            {
                u8Max_Value = MAX( u8Max_Value_Array2-u8Spatial_Strength*2, 0 );
            }
        }
        else if (u8Loop_Circle == 1)
        {
            if (au8Tmp_Value_Adj[2][2] >= MAX( u8Max_Value_Array1 - u8Spatial_Strength, 0 ) )
            {
                u8Max_Value = au8Tmp_Value_Adj[2][2];
            }
            else
            {
                u8Max_Value = MAX( u8Max_Value_Array1 -u8Spatial_Strength, 0 );
            }
        }
    }
    return u8Max_Value;
}

MS_U8 MDrv_Spatial_Filtering1D(void *pInstance, MS_U8 i_index, MS_U8 j_index, MS_U8 u8Spatial_Strength)
{
    MS_U8 u8Max_Value = 0;
    MS_U8 au8Tmp_Value_Adj[3];
    MS_S8 i = 0,j = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)
    {
        for (j = -1; j < 2; j++)
        {
            if ( ( (j+j_index>=0)  && (j+j_index<pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh) ))
            {
                au8Tmp_Value_Adj[j+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j+j_index][i_index];
            }
            else
            {
                au8Tmp_Value_Adj[j+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }
    else
    {
        for (i = -1; i < 2; i++)
        {
            if ( (i+i_index >= 0) && (i+i_index < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth ) )
            {
                au8Tmp_Value_Adj[i+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i+i_index];
            }
            else
            {
                au8Tmp_Value_Adj[i+1] = pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][i_index];
            }
        }
    }

    if ( (au8Tmp_Value_Adj[1] >= au8Tmp_Value_Adj[0]) && (au8Tmp_Value_Adj[1] >= au8Tmp_Value_Adj[2]) )
    {
        u8Max_Value = au8Tmp_Value_Adj[1];
    }
    else if ( ( DIFF(au8Tmp_Value_Adj[1],au8Tmp_Value_Adj[0]) <= u8Spatial_Strength) && (DIFF(au8Tmp_Value_Adj[1],au8Tmp_Value_Adj[2])<=u8Spatial_Strength) )
    {
        u8Max_Value = au8Tmp_Value_Adj[1];
    }
    else
    {
        u8Max_Value = MAX (MAX(au8Tmp_Value_Adj[0],au8Tmp_Value_Adj[2]) - u8Spatial_Strength, 0);
    }
#if (ADD_OPPOSITE_LUMA)
    return MIN(u8Max_Value+pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j_index][(i_index == 1 ? 0 : 1)]/2,0xFF);
#else
    return u8Max_Value;
#endif
}


void  MDrv_Maxnxn_FrameData_CSep_8B(void *pInstance, MS_U32 u32Luma_Avg_Diff, MS_U8 u8Idx, MS_U32 u32Luma_Avg_OSD_diff, MS_U8 u8Spatial_Strength)
{
    MS_U8 i = 0,j = 0;//, ChannelIdx;
    MS_U32 u32Frame_Factor =0;
    MS_U32 u32Frame_Factor_dec = 0;
    MS_U16 u16Pindex = 0,u16Pindex2 = 0;
    MS_U16 u16Tmp = 0;
    MS_U16 u16Tmp1 = 0;
    MS_U32 u32P_Tmp_Cur = 0, u32P_Tmp_Pre = 0, u32P_Tmp_IIR = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u32Frame_Factor = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L,0xFF00) >> 8;
    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            u16Pindex = ( j*pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth + i);
            u16Pindex2 = u16Pindex * 2;

            if ( (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2) || (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh == 2) )
            {
                u32P_Tmp_Cur = MDrv_Spatial_Filtering1D_Large(pInstance, i, j, u8Spatial_Strength)*256;
            }
            else
            {
                u32P_Tmp_Cur = MDrv_Spatial_Filtering2D(pInstance, i, j, u8Spatial_Strength)*256;
            }

            u16Tmp = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[u16Pindex2+1];
            u16Tmp1 = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[u16Pindex2];

            u32P_Tmp_Pre = u16Tmp*256 + u16Tmp1;

            if ( (u32Luma_Avg_Diff >= LUMA_AVG_DIFF_THRESHOLD1) /*|| (u32Luma_Avg_OSD_diff > 200*/)//To fix konka flicker bug. Mantis 0265653 )
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-0) + u32P_Tmp_Pre*0 + 0 )>>8;//div 256;
            }
            else if ( (u32Luma_Avg_Diff<LUMA_AVG_DIFF_THRESHOLD1) && (u32Luma_Avg_Diff>=LUMA_AVG_DIFF_THRESHOLD2))
            {
                u32Frame_Factor_dec = u32Frame_Factor - (u32Frame_Factor/16) * (u32Luma_Avg_Diff - LUMA_AVG_DIFF_THRESHOLD2);
                u32P_Tmp_IIR = (u32P_Tmp_Cur * (256-u32Frame_Factor_dec) + u32P_Tmp_Pre * u32Frame_Factor_dec + 0 )>>8;//div 256;
            }
            else  if ( u32Luma_Avg_Diff < LUMA_AVG_DIFF_THRESHOLD2)
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-u32Frame_Factor) + u32P_Tmp_Pre*u32Frame_Factor )>>8;//div 256;
            }

            MS_U8 u8Gamma_Min = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_78_L, 0x00FF);//MFC_ReadByte(REG_2FF0);
            if (u8Gamma_Min == 1)
            {
                u32P_Tmp_IIR = (u32P_Tmp_Cur*(256-u32Frame_Factor) + u32P_Tmp_Pre*u32Frame_Factor )>>8;//div 256;
            }

            if (u8Idx == 1)
            {
                u32P_Tmp_IIR = (255*256/1);
            }

            pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[u16Pindex2] = u32P_Tmp_IIR & 0x0FF;
            pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[u16Pindex2+1] = (u32P_Tmp_IIR & 0x0000FF00)>>8;
        }
    }

    //prepare for next frame
    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize*2; j++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2B[j] = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[j];
    }
}


void MDrv_BLgamma(void *pInstance, MS_BOOL bDark_Info)
{
    MS_U8 i = 0,j = 0;
    MS_U8 u8LUT_Index_L = 0,u8LUT_Index_H = 0;
    MS_U8 u8Blending = 0;
    MS_U8 u8Gamma_Min = 0;//SC_R2BYTEMSK(REG_SC_BK2E_78_L,0xFF);
    const MS_U8* pu8GammaTbl = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (DIRECTTYPE_PATCH_ENABLE)
    if (bDark_Info)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = 1;
    }

#else
        pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L,0xFF00))>>8;
#endif

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i];
        }
    }
    switch (pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control)
    {
        default:
        pu8GammaTbl = NULL; //No gamma adaptation
        break;

        case 1:
        pu8GammaTbl = gamma_0dot4;
        break;

        case 2:
        pu8GammaTbl = gamma_0dot6;
        break;

        case 3:
        pu8GammaTbl = gamma_1dot2;
        break;

        case 4:
        pu8GammaTbl = gamma_1dot4;
        break;

        case 5:
        pu8GammaTbl = gamma_0dot9;
        break;

        case 6:
        pu8GammaTbl = gamma_0dot8;
        break;

        case 7:
        pu8GammaTbl = gamma_0dot7;
        break;

        case 8:
        pu8GammaTbl = gamma_0dot75;
        break;

        case 9:
        pu8GammaTbl = gamma_0dot78;
        break;

        case 10:
        pu8GammaTbl = LGE_sample3;
        break;

        case 11:
        pu8GammaTbl = LGE_sampleTHX;
        break;

        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        pu8GammaTbl = LGE_sample1_adjust;
        break;

        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        pu8GammaTbl = LGE_sample2_adjust;
        break;

        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
        case 38:
        pu8GammaTbl = LGE_sample7_adjust;
        break;

        case 39:
        pu8GammaTbl = LGE_sample1_0dot81;
        break;
    }

    if (pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control)
    {
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                u8LUT_Index_L =  MINMAX(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/4, 0, 63);
                u8LUT_Index_H =  MINMAX(pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i]/4+1, 0, 63);
                u8Blending = pXCResourcePrivate->stdrv_LDALGO._u8VDown_Block[j][i] % 4;
                pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = ( (4-u8Blending)*pu8GammaTbl[u8LUT_Index_L] + (u8Blending)*pu8GammaTbl[u8LUT_Index_H] +2 )/4;
            }
        }
    }

    for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
    {
        for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i] = MAX(u8Gamma_Min, pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i]);
        }
    }

    if(REG_DEBUG_LD_DUMP_SEL == E_LD_DUMP_Gamma_Block)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt = (pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt+1)%8;
        for (j = 0; j < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh; j++)
        {
            for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth; i++)
            {
                MDrv_LD_MIUWriteByte(LD_BUF_WriteBuf_0, (0x20+pXCResourcePrivate->stdrv_LDALGO._u8DebugArrayDumpDummyCnt*2) + j*0x10 + i, pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Block[j][i]);//1
            }
        }
    }
}



void MDrv_EdgeModeImpulse(void *pInstance)
{
    MS_U32 u32Rounding = 0,u32Value = 0;
    MS_U32 i = 0;
    MS_U16 u16BIN_Pulse = 0;
    MS_U8 u8BLStrength = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16BIN_Pulse  = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_22_L);// readw(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x44/2) );
    u8BLStrength = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_23_L, 0xFF);  //readw(HAL_MSTAR_AEON_RIU_BASE+HAL_REGBANK_OFFSET(0x3300 / 2 + 0x46/2) );

    for (i = 0;i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize*2;i = i+2)
    {
        u32Rounding = pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[i];
        u32Value =  pXCResourcePrivate->stdrv_LDALGO._u32IIR_T2C[i+1];
        if (u32Value == 0xFF)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i/2] = 0xFF;
        }
        else
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i/2] = MIN(u32Value + (u32Rounding/128), 0xFF);
        }
    }

    if ( (u16BIN_Pulse > 0) && (u16BIN_Pulse != 0xFFFF) )//For testing control, only one lamp enabled, others off.
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2) //LR edge type
        {
            for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
            {
                if (i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)
                {
                    if (i == u16BIN_Pulse)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[(i*2-1)-1] = u8BLStrength;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[(i*2-1)-1] = 0;
                    }
                }
                else
                {
                    if (i == u16BIN_Pulse)
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[((i - pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)*2)-1] = u8BLStrength;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[((i - pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh)*2)-1] = 0;
                    }
                }
            }
        }
        else
        {
            for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
            {
                if (i == u16BIN_Pulse)
                {
                    pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = u8BLStrength;
                }
                else
                {
                    pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = 0;
                }
            }
        }

    }
    else if (u16BIN_Pulse == 0xFFFF)
    {
        for (i = 1; i <= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
        {
            pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i-1] = u8BLStrength;
        }
    }

    for (i = 0; i < pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16PWMSize; i++)
    {
        pXCResourcePrivate->stdrv_LDALGO._u8LED_resort[i] = pXCResourcePrivate->stdrv_LDALGO._u8IIR_T2[i];
    }

}


MS_U8 MDrv_volumn_OSD_patch(void)
{
    return 0;
}


void MDrv_GetFrameIndex(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_40_L, 0xFF) & 0x10 ;
}

void MDrv_LDimmingFunction(void *pInstance)
{
    MS_U32 u32Luma_Cur = 0;
    MS_U32 u32Luma_Diff = 0;
    MS_U32 u32Luma_OSD_Diff = 0;
    MS_U8 u8Spatial_Strength = 0,u8Spatial_Strength_adj = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (DIRECTTYPE_PATCH_ENABLE)
    MS_U8 u8Histogram_Max1 = 0;
    MS_U8 u8Histogram_Max2 = 0;
    MS_U8 u8Histogram_Max1_bin = 0;
    MS_U8 u8Histogram_Max2_bin = 0xF;
    MS_U8 u8Histogram_Bin_Diff = 0;
    MS_BOOL bDark_Info = TRUE;
    MS_U8 i = 0;
#endif
    MDrv_GetFrameIndex(pInstance);

    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Test = pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx;
    pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx_Final = pXCResourcePrivate->stdrv_LDALGO._u8Frame_Idx;
    //MDrv_Write2Byte(0x2620,0xA5A5);
    pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth;
    pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio = pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDHeight/pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh;
    pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio_3D = pXCResourcePrivate->stdrv_LDALGO._u8H_Down_Ratio/2;
    pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio_3D = pXCResourcePrivate->stdrv_LDALGO._u8V_Down_Ratio/2;
    pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_LD = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8LDWidth>16) ? 32 : 16 ;

    if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth != 2 && pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh != 2 ) //direct type
    {
        pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth > 16) ? 32 : 16 ;
    }
    else   //edge type
    {
        if (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth == 2)   //LR edge
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMHigh > 16) ? 32 : 16 ;
        }
        else                          //TB edge
        {
            pXCResourcePrivate->stdrv_LDALGO._u8Pack_Length_PWM = (pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u8PWMWidth > 16) ? 32 : 16 ;
        }
    }

    u32Luma_Cur =  MDrv_FramLockBffdepack_CSep_8B_Full(pInstance);
    MDrv_BLMxNto2xN_White_8B(pInstance);
    MDrv_BL2xNto2xNd3_White_8B(pInstance);
    //=======================================
    u8Spatial_Strength = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, 0xFF);
    u8Spatial_Strength_adj = u8Spatial_Strength;

#if (DIRECTTYPE_PATCH_ENABLE)
    for (i = 0; i < 16; i++)
    {
        if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max1 ) && ( pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max2) )
        {
            u8Histogram_Max2_bin = u8Histogram_Max1_bin;
            u8Histogram_Max2 = u8Histogram_Max1;
            u8Histogram_Max1_bin = i;
            u8Histogram_Max1 = pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i];
        }
        else if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] < u8Histogram_Max1 ) && ( pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i] > u8Histogram_Max2) )
        {
            u8Histogram_Max2_bin = i;
            u8Histogram_Max2 = pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[i];
        }
    }

    u8Histogram_Bin_Diff = ABS(u8Histogram_Max1_bin - u8Histogram_Max2_bin);

    if ((pXCResourcePrivate->stdrv_LDALGO._u8Histogram_Image[0] >= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize*9/10 )
        && (pXCResourcePrivate->stdrv_LDALGO._u16Histogram_Zero >= pXCResourcePrivate->stdrv_LDALGO._stLDPanelPara.u16BLSize*9/10 ) )
    {
        u8Spatial_Strength_adj = MINMAX( u8Spatial_Strength+0x80, 0, 0xFF );
        //MDrv_Write2Byte(0x3346,0x00FF);
    }
    else if ( u8Histogram_Max1_bin <= 8 &&  u8Histogram_Bin_Diff >= 5 )
    {
        u8Spatial_Strength_adj = MINMAX( u8Spatial_Strength , 0, 0xFF );
        bDark_Info = TRUE;
    }
    else if ( u8Histogram_Max1_bin <= 8 && MDrv_volumn_OSD_patch() == 1)
    {
        u8Spatial_Strength_adj = MINMAX(u8Spatial_Strength, 0, 0xFF);
        bDark_Info = TRUE;
    }

    //===============================================================
    MDrv_BLgamma(pInstance, bDark_Info);
#else
    MDrv_BLgamma(pInstance, FALSE);
#endif
    u32Luma_Diff = DIFF(u32Luma_Cur, pXCResourcePrivate->stdrv_LDALGO._u8Iluma_Pre);
    pXCResourcePrivate->stdrv_LDALGO._u8Iluma_Pre = u32Luma_Cur;

    if(pXCResourcePrivate->stdrv_LDALGO._u8OSDEnable && (u32Luma_Cur < 30))
    {
        u32Luma_OSD_Diff = 255;
    }

    MDrv_Maxnxn_FrameData_CSep_8B(pInstance, u32Luma_Diff, pXCResourcePrivate->stdrv_LDALGO._bLDoff, u32Luma_OSD_Diff, u8Spatial_Strength_adj);          //T2b -> T2
    // MDrv_Maxnxn_FrameData_CSep_8B(u32Luma_Diff,0,0,u8Spatial_Strength_adj);// T2b -> T2
    MDrv_EdgeModeImpulse(pInstance);                  // upperline / lower line             T2
    MDrv_Output_SPIdata(pInstance);             //for SPI data
    MDrv_Output_DigitalBLdata(pInstance);               // 0xAA
}

void MDrv_LDAlgo_Main(SC_INT_SRC enIntNum, void *pParam)
{
    UNUSED(enIntNum);
    UNUSED(pParam);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pu32XCInst, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    #if 1
    if(pXCResourcePrivate->stdrv_LDALGO._bLDoff || !MDrv_LD_Get_LocalDimmingEn(pu32XCInst))
    {
        return;
    }

    MDrv_appLDModeHandler_R2(pu32XCInst);
    MDrv_LDimmingFunction(pu32XCInst);
    #else
    static MS_U8  u8RandomStatus=0x0;
    *(volatile unsigned int *)(0x80000000+(LD_BUF_ADR))=u8RandomStatus;
    if (u8RandomStatus>=0x80)
    {
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+1)=u8RandomStatus-0x10;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+2)=u8RandomStatus-0x15;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+3)=u8RandomStatus-0x19;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+4)=u8RandomStatus-0x35;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+5)=u8RandomStatus-0x70;

        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+10)=u8RandomStatus-0x50;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+11)=u8RandomStatus-0x18;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+12)=u8RandomStatus-0x36;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+13)=u8RandomStatus-0x79;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+14)=u8RandomStatus-0x25;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+15)=u8RandomStatus-0x44;
    }
    else
    {
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+01)=u8RandomStatus+0x10;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+02)=u8RandomStatus+0x15;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+03)=u8RandomStatus+0x19;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+04)=u8RandomStatus+0x35;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+05)=u8RandomStatus+0x70;

        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+10)=u8RandomStatus+0x50;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+11)=u8RandomStatus+0x18;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+12)=u8RandomStatus+0x36;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+13)=u8RandomStatus+0x79;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+14)=u8RandomStatus+0x25;
        *(volatile unsigned char *)(0x80000000+(LD_BUF_ADR)+15)=u8RandomStatus+0x44;
    }
    if (u8RandomStatus == 0xFF)
    {
        u8RandomStatus =0x0;
    }
    else
    {
        u8RandomStatus = u8RandomStatus + 1;
    }
    #endif
}

void MDrv_XC_LD_ISR(void* pInstance)
{
    if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, MDrv_LDAlgo_Main, NULL))
    {
        MDrv_XC_InterruptAttach(pInstance, SC_INT_VSINT, MDrv_LDAlgo_Main, (void *) NULL);
    }
}

void MDrv_LD_EnableLD(void *pInstance, MS_BOOL bLDEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (pXCResourcePrivate->stdrv_LDALGO._bLDoff == bLDEnable)
    {
        pXCResourcePrivate->stdrv_LDALGO._bLDoff = !bLDEnable;
        MDrv_LD_Set_LocalDimmingEn(pInstance, bLDEnable);
    }
}

void MDrv_LD_SetGammaControl(void *pInstance, MS_U8 u8GammaControl)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8Gamma_Control = u8GammaControl;
}

void MDrv_LD_SetOSDEnable(void *pInstance, MS_U8 u8OSDEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LDALGO._u8OSDEnable = u8OSDEnable;
}

#endif

