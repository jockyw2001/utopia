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
////////////////////////////////////////////////////////////////////////////////#include <stdio.h>
#define DRV_GOP_SC_C

#include "MsCommon.h"
#include "MsOS.h"

#include "drv_gop_sc.h"
#include "mhal_gop_sc.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/div64.h>
#else
#include <string.h>
//#define do_div(x,y) ((x)/=(y))
#endif


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DBG_GOP_SC(x) //x

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static DRV_GOP_SC_INFO gGopSC_Info = {0};
static GOP_SC_REF         gGopSC_Ref = {0};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U32 _GCD(MS_U32 u32A,MS_U32 u32B)
{
    MS_U32 u32R;

#ifdef  MSOS_TYPE_LINUX_KERNEL
    while((u32R=do_div(u32A,u32B))!=0)
#else
    while((u32R=u32A%u32B)!=0)
#endif
    {
        u32A=u32B;
        u32B=u32R;
    }

    return u32B;
}

static MS_BOOL _ReductionFrac (MS_U32 *pu32A,MS_U32 *pu32B)
{
    MS_U32 u32GCD;
    while(1)
    {
        if( (*pu32A<3000) && (*pu32B<3000))  return TRUE;
        if( (u32GCD = _GCD(*pu32A,*pu32B))==1) return TRUE;
        *pu32A /= u32GCD;
        *pu32B /= u32GCD;
    }
    return TRUE;
}

static MS_BOOL _MultiplFrac(MS_U32 *pu32A,MS_U32 *pu32B)
{
    while(1)
    {
        if( ( (*pu32A<<1)<3000) && ( (*pu32B<<1) <3000))
        {
             *pu32A<<=1;
             *pu32B<<=1;
        }
        else
        {
            return TRUE;
        }
    }
    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_Set_ScalingFac(VOID)
{
    MS_U32 u32VscFac,u32HscFac;

    MS_U32 u32Fac ;

    DBG_GOP_SC(printf("[GOP_SC] _MDrv_GOP_SC_Set_ScalingFac\n");)
    DBG_GOP_SC(printf("[GOP_SC] VdeIn = %x \t H=%x\n",gGopSC_Info.InCfg.u16VdeIn,gGopSC_Ref.u16ValidH);)
    DBG_GOP_SC(printf("[GOP_SC] ValidV = %x\t HdeIn=%x\n",gGopSC_Ref.u16ValidV,gGopSC_Info.InCfg.u16HdeIn);)

    u32Fac = gGopSC_Info.InCfg.u16VdeIn;
    u32VscFac =(MS_U32) V_ScalingDownRatio(u32Fac, gGopSC_Ref.u16ValidV);

    //If H in = H out , must disable H Scaling
    if(gGopSC_Ref.u16ValidH == gGopSC_Info.InCfg.u16HdeIn)
        u32HscFac = 0;
    else
    {
        u32Fac = gGopSC_Ref.u16ValidH;
        u32HscFac = (MS_U32) H_ScalingDownRatio(u32Fac, gGopSC_Info.InCfg.u16HdeIn);
    }

    DBG_GOP_SC(printf("[GOP_SC] Scalilng FacV = %lx \t Scaling FacH = %lx\n",u32VscFac,u32HscFac);)

    //_XC_ENTRY();
    HAL_GOP_SC_Set_ScalingFac(u32VscFac,u32HscFac);
    //_XC_RETURN();

    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_AheadMode(VOID)
{
    MS_U16 u16VRatio,u16Mod;
    MS_BOOL bEnMode = TRUE;

    #ifdef MSOS_TYPE_LINUX_KERNEL
    u16VRatio = gGopSC_Info.InCfg.u16VdeIn;
    u16Mod = do_div(u16VRatio,  gGopSC_Ref.u16ValidV);
    #else
    u16VRatio = gGopSC_Info.InCfg.u16VdeIn / gGopSC_Ref.u16ValidV;
    u16Mod = gGopSC_Info.InCfg.u16VdeIn - (u16VRatio* gGopSC_Ref.u16ValidV);
    #endif

    bEnMode = ( ((u16Mod>0) && (u16VRatio>=2))) ? FALSE : TRUE;

    DBG_GOP_SC(printf("[GOP_SC] Vratio =%d \t Mod = %d\n",u16VRatio,u16Mod);)
    DBG_GOP_SC(printf("[GOP_SC] AheadMode =%d\n",bEnMode);)

    //_XC_ENTRY();
    HAL_GOP_SC_AheadMode(bEnMode);
    //_XC_RETURN();

    if( u16VRatio>=2)
    {
        gGopSC_Ref.u16VFacInt = 3;
        gGopSC_Ref.u16VFacFrac = 5;
    }
    else
    {
        gGopSC_Ref.u16VFacInt = 2;
        gGopSC_Ref.u16VFacFrac = 0;
    }

    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_FreqSync(VOID)
{
    //idclk_div & odclk_div
    MS_U32 u32FreqIn,u32FreqOut;

    u32FreqIn   = gGopSC_Info.InCfg.u16VttIn  *  gGopSC_Info.InCfg.u16HttIn;
    u32FreqOut = ((MS_U32)(gGopSC_Info.OutCfg.u16VttOut)* (MS_U32)(gGopSC_Info.OutCfg.u16HttOut))*4;
    if(!gGopSC_Info.OutCfg.bInterlace)
        u32FreqOut *= 2;

    DBG_GOP_SC(printf("[GOP_SC] _MDrv_GOP_SC_FreqSync\n"););
    DBG_GOP_SC(printf("[GOP_SC] 32FreqIn=%lx \t u32FreqOut=%lx \n",u32FreqIn,u32FreqOut););

    _ReductionFrac(&u32FreqIn, &u32FreqOut);
    _MultiplFrac(&u32FreqIn,&u32FreqOut);

    gGopSC_Ref.u32IDclk  = u32FreqIn;
    gGopSC_Ref.u32ODclk =  u32FreqOut;

    if(gGopSC_Ref.u32IDclk>0xFFFFFF || gGopSC_Ref.u32ODclk>0xFFFFFF )
            DBG_GOP_SC(printf("[GOP_SC] ASSERT !!! idclk/odclk overflow\n");)

    DBG_GOP_SC(printf("[GOP_SC] idclk=%lx \t odclk=%lx\n",gGopSC_Ref.u32IDclk,gGopSC_Ref.u32ODclk););

    //_XC_ENTRY();
    HAL_GOP_SC_FreqSync((gGopSC_Ref.u32IDclk&0xFFFFFF),(gGopSC_Ref.u32ODclk&0xFFFFFF));
    //_XC_RETURN();

    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_PhaseSync(VOID)
{
    DBG_GOP_SC(printf("[GOP_SC]_MDrv_GOP_SC_PhaseSync\n"););

    //ivs_ref
    MS_U32 u32TotalOffset;
    MS_U32 u32IntY,u32ModF;
    MS_U32 u32OffsetY,u32OffsetX;

    u32TotalOffset =  gGopSC_Ref.u16VFacInt *10 + gGopSC_Ref.u16VFacFrac ;
    u32TotalOffset *= (gGopSC_Ref.u16ValidV/(1+gGopSC_Info.OutCfg.bInterlace));

    #ifdef MSOS_TYPE_LINUX_KERNEL
    u32IntY = u32TotalOffset;
    u32ModF = do_div(u32IntY,  (gGopSC_Info.InCfg.u16VdeIn*10));
    #else
    u32IntY = (u32TotalOffset / (gGopSC_Info.InCfg.u16VdeIn*10)) ;
    u32ModF = u32TotalOffset - (u32IntY* gGopSC_Info.InCfg.u16VdeIn*10);
    #endif

    u32OffsetY = u32IntY;
    u32OffsetX = ((u32ModF<<7) *2 *gGopSC_Info.OutCfg.u16HttOut)/(gGopSC_Info.InCfg.u16VdeIn*10);
    u32OffsetX >>=7;

    DBG_GOP_SC(printf("[GOP_SC] Offset_line=%ld\t Offset_pixel=%ld\n",u32OffsetY,u32OffsetX);)
    DBG_GOP_SC(printf("[GOP_SC] u8SkipV=%d\t u8SkipH=%d\n",gGopSC_Ref.u8SkipV,gGopSC_Ref.u8SkipH);)

    if(u32OffsetX>255)
    {
        gGopSC_Ref.u16VE_RefY= 23-(u32OffsetY+1)-(gGopSC_Ref.u8SkipV/(2*(1+gGopSC_Info.OutCfg.bInterlace)));
        gGopSC_Ref.u16VE_RefX = 255 + 2* gGopSC_Info.OutCfg.u16HttOut-u32OffsetX;
    }
    else
    {
        gGopSC_Ref.u16VE_RefY = 23-u32OffsetY-(gGopSC_Ref.u8SkipV/(2*(1+gGopSC_Info.OutCfg.bInterlace)));
        gGopSC_Ref.u16VE_RefX = 255 -u32OffsetX;
    }

    DBG_GOP_SC(printf("[GOP_SC] u16VE_RefY=%d\t u16VE_RefX=%d\n",gGopSC_Ref.u16VE_RefY,gGopSC_Ref.u16VE_RefX);)

    if(gGopSC_Info.u8TVSystem == 2) //EN_GOP_SC_PAL_M
    {
        gGopSC_Ref.u16VE_RefY-=3;
    }

    //_XC_ENTRY();
    HAL_GOP_SC_PhaseSync(gGopSC_Ref.u16VE_RefY ,gGopSC_Ref.u16VE_RefX,
                  gGopSC_Info.InCfg.u16Hde_St, gGopSC_Info.InCfg.u16Vde_St,gGopSC_Info.InCfg.u16HttIn);
    //_XC_RETURN();

    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_CheckSyncDone(VOID)
{
    //Check lock_done
    MS_U32 u32Time,u32TimeOut;
    MS_U8 u8Status;
    u32Time = MsOS_GetSystemTime();
    u32TimeOut = 5000;

    while(1)
    {
        u8Status= HAL_VEPLL_Check_LockDone();
        if( u8Status == 0x01)
        {
            //printf("[%s]-[%d] : Lock done. \n", __FUNCTION__, __LINE__);
            return TRUE;
        }

        if ( ( MsOS_GetSystemTime() - u32Time) >= u32TimeOut )
        {
            printf("[%s]-[%d] : Timeout . \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_SkipFun(VOID)
{
    MS_U32 u32VttIn=0 ,u32VttOut=0 ;
    MS_U32 u32ValidV= 0,u32ValidH= 0;

    u32VttIn   = gGopSC_Info.InCfg.u16VttIn;
    u32VttOut = gGopSC_Info.OutCfg.u16VttOut;

    u32ValidV = (gGopSC_Info.InCfg.u16VdeIn*u32VttOut) / u32VttIn;
    u32ValidH = (u32ValidV*gGopSC_Info.OutCfg.u16HdeOut) / gGopSC_Info.OutCfg.u16VdeOut;

    //ASSERT((u32ValidV<g_GOP_SC_Info.u16VdeOut)||(u32ValidH<g_GOP_SC_Info.u16HdeOut));

    gGopSC_Ref.u16ValidH = (MS_U16)(u32ValidH);
    gGopSC_Ref.u16ValidV = (MS_U16)(u32ValidV);

    gGopSC_Ref.u8SkipH = (MS_U8)(u32ValidH-gGopSC_Info.OutCfg.u16HdeOut);
    gGopSC_Ref.u8SkipV = (MS_U8)(u32ValidV-gGopSC_Info.OutCfg.u16VdeOut);

    //_XC_ENTRY();
    HAL_GOP_SC_SkipFun(gGopSC_Ref.u8SkipV,gGopSC_Ref.u8SkipH,
                                    gGopSC_Ref.u16ValidV,gGopSC_Ref.u16ValidH);
    //_XC_RETURN();
    DBG_GOP_SC(printf("[GOP_SC] ValidV = %ld \t ValidH = %ld\n",u32ValidV,u32ValidH);)
    DBG_GOP_SC(printf("[GOP_SC] u8SkipV=%d\t u8SkipH=%d\n",gGopSC_Ref.u8SkipV,gGopSC_Ref.u8SkipH);)

    return TRUE;
}

static MS_BOOL _MDrv_GOP_SC_SetHVSP(VOID)
{
    //_XC_ENTRY();
    HAL_GOP_SC_SetHVSP(gGopSC_Info.InCfg.u16HdeIn, gGopSC_Info.InCfg.u16VdeIn,
                                    gGopSC_Info.OutCfg.u16HdeOut,gGopSC_Ref.u16ValidV);
    //_XC_RETURN();
    DBG_GOP_SC(printf("[GOP_SC] src_wd=%d\t src_ht=%d\n",gGopSC_Info.InCfg.u16HdeIn,gGopSC_Info.InCfg.u16VdeIn);)
    DBG_GOP_SC(printf("[GOP_SC] dst_wd=%d\t dst_ht=%d\n",gGopSC_Info.OutCfg.u16HdeOut,gGopSC_Ref.u16ValidV);)
    return TRUE;

}

static MS_BOOL _MDrv_GOP_SC_P2I(MS_U8 u8EnMode)
{
    HAL_GOP_SC_P2I(u8EnMode);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GOP_SC_Init(void)
{
    //Initial Local Variable
    memset(&gGopSC_Ref, 0 , sizeof(GOP_SC_REF) );
    memset(&gGopSC_Info, 0 , sizeof(DRV_GOP_SC_INFO) );

    HAL_GOP_SC_SetClock(EN_DST_FBL);        //Default is FBL case , VEPLL mode
    HAL_VEPLL_Reset(TRUE);

    //default setting
    HAL_VEPLL_PLL_Ctrl(0x08,0x08,0x02);     //0x288
    HAL_VEPLL_PllSet(0x19999999);             //initial PLL set

    //[FSYNC]
    HAL_GOP_SC_Set_PllCfg();

    HAL_VEPLL_Reset(FALSE);
    HAL_VEPLL_PowerDown(FALSE);             //Disable PowerDown LPLL

    return TRUE;
}

VOID MDrv_GOP_SC_Init_riu_base( MS_VIRT vriu_base )
{
    HAL_GOP_SC_Init_riu_base(vriu_base);
    return;
}


MS_BOOL MDrv_GOP_SC_MuxSel(MS_U8 u8Source)
{
    //_XC_ENTRY();
    gGopSC_Info.u8MuxSel = u8Source;
    HAL_GOP_SC_MuxSel(u8Source);
    //_XC_RETURN();
    return TRUE;
}

VOID MDrv_GOP_SC_SetParams(DRV_GOP_SC_InCfg *pInCfg,DRV_GOP_SC_OutCfg *pOutCfg,MS_U8 u8TvSys)
{
    gGopSC_Info.u8TVSystem = u8TvSys;
    memcpy(&gGopSC_Info.InCfg,pInCfg,sizeof(DRV_GOP_SC_InCfg));
    memcpy(&gGopSC_Info.OutCfg,pOutCfg,sizeof(DRV_GOP_SC_OutCfg));
}

MS_BOOL  MDrv_GOP_SC_SetCfg(VOID)
{
    //Scaling & Skip Fun
    _MDrv_GOP_SC_SkipFun();
    _MDrv_GOP_SC_SetHVSP();
    _MDrv_GOP_SC_Set_ScalingFac();
    _MDrv_GOP_SC_AheadMode();
    _MDrv_GOP_SC_P2I(TRUE);

    return TRUE;
}

MS_BOOL MDrv_GOP_SC_SetDst(DRV_GOP_SC_Dst stDst)
{
    gGopSC_Info.stDst = stDst;

    switch(stDst)
    {
        case EN_DST_FB : //case 2
            HAL_VEPLL_PowerDown(TRUE);  //PowerDown LPLL
            HAL_GOP_SC_SetClock(EN_DST_FB);
            HAL_GOP_SC_Vsync(EN_VSYNC_VE);
            break;

        case EN_DST_FBL: //case 3, 4
            HAL_VEPLL_Limit_d5d6d7(0x00000100);     //limit d5d6d7
            HAL_VEPLL_FPLL_Enable(FALSE);

            //initial PLL set
#if (VEPLL_FIXED_PLL)
            if(gGopSC_Info.InCfg.u16HdeIn > 720 ) //HD timing
            {
                HAL_VEPLL_PLL_Ctrl(0x08,0x08,0x02);
                HAL_VEPLL_PllSet(0x19999999);
            }
            else
            {
                MS_U32 u32HdmiPll = HAL_VEPLL_Get_HdmiPllSet();
                HAL_VEPLL_PLL_Ctrl(0x00,0x01,0x03);
                HAL_VEPLL_PllSet(u32HdmiPll);
            }
#else
            HAL_VEPLL_PLL_Ctrl(0x08,0x08,0x02);
            HAL_VEPLL_PllSet(0x19999999);
#endif
            HAL_GOP_SC_Vsync(EN_VSYNC_SC);
            break;

        default :
            DBG_GOP_SC(printf("[GOP_SC] Fail GOP_SC Path!\n"));
            break;
    }
    return TRUE;
}

MS_BOOL MDrv_GOP_SC_SetLock(VOID)
{
    _MDrv_GOP_SC_FreqSync();
    _MDrv_GOP_SC_PhaseSync();

    return TRUE;
}

MS_BOOL MDrv_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable)
{
    MS_BOOL bStatus = FALSE;

#if (VEPLL_FIXED_PLL)
    if(gGopSC_Info.InCfg.u16HdeIn > 720 ) //HD timing
    {
        HAL_VEPLL_PllSet(0x19999999);
    }
    else
    {
        MS_U32 u32HdmiPll = HAL_VEPLL_Get_HdmiPllSet();
        HAL_VEPLL_PllSet(u32HdmiPll);
    }
#else
    HAL_VEPLL_PllSet(0x19999999);
#endif

    HAL_VEPLL_LockFreq_Enable(bEnable);
    HAL_VEPLL_FPLL_Enable(bEnable);

    bStatus = _MDrv_GOP_SC_CheckSyncDone();

#if (VEPLL_FIXED_PLL)
    if( (gGopSC_Info.InCfg.u16HdeIn <= 720) && bStatus ) //SD timing
    {
        HAL_VEPLL_Limit_d5d6d7(0x00000000);     //limit d5d6d7
    }
#endif

    return TRUE;
}

