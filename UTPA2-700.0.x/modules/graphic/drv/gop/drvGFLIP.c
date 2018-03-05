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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   drvGFLIP.c
/// @brief  MStar gflip Interface
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_GFLIP_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "regGOP.h"
#include "halGOP.h"
#include "drvGFLIP.h"
#include "halGFLIP.h"
#include "halCHIP.h"
#if defined(MSOS_TYPE_LINUX)
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_gflip_io.h"
    #include <string.h>
#elif   defined(MSOS_TYPE_LINUX_KERNEL)
    #include "mdrv_gflip_io.h"
#endif
#include "drvGOP_priv.h"

//=============================================================================
// Compile options
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================
#define GFLIP_DEBUG
#ifdef GFLIP_DEBUG
    #define GFLIP_PRINT(fmt, args...)      printf("[GFlip (Driver)][%05d] " fmt, __LINE__, ## args)
    #define GFLIP_ASSERT(_cnd, _fmt, _args...)                   \
                                    if (!(_cnd)) {              \
                                        GFLIP_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define GFLIP_PRINT(_fmt, _args...)
    #define GFLIP_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Global Variables
//=============================================================================
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
#else
GOP_CTX_DRV_LOCAL*pGFLIP_IntGOPDrvLocalCtx = NULL;
//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MDrv_GFLIP_ResetFlipInfoPtr(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx);

//Irq Relaated(hw process):
static MS_BOOL _MDrv_GFLIP_ClearIRQ(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx);
static MS_BOOL _MDrv_GFLIP_ProcessIRQ(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx);
//Interrupt call back function:
void _MDrv_GFLIP_IntHandler(InterruptNum eIntNum);

//=============================================================================
// Local Function
//=============================================================================
//-------------------------------------------------------------------------------------------------
/// Reset Flip Info Reader/Writer ptr.
/// @param  u32GopIdx    \b IN: the idx of ptr's owner(gop)
/// @return void
/// @attention
/// <b>[MxLib] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_GFLIP_ResetFlipInfoPtr(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx)
{
    MS_U16 u16Idx;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    for(u16Idx=0; u16Idx<MAX_GOP_GWIN; u16Idx++)
    {
        pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u16Idx] = 0;
        pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u16Idx] = 0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear Irq
/// @param  void
/// @return TRUE: success
/// @attention
/// <b>[MxLib] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_ClearIRQ(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx)
{
    MS_U32 u32GopIdx;

    MHal_GFLIP_ClearDWINIRQ(&pGOPDrvLocalCtx->halCtxLocal, &pGOPDrvLocalCtx->gflipDWinIntInfo);

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        if(MHal_GFLIP_IsVSyncInt(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx) == FALSE)
        {
            continue;
        }

        MHal_GFLIP_IntEnable(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx, FALSE);

        if(MHal_GFLIP_IsVSyncInt(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx) == TRUE)
        {   //Wow...The Vsync Issue Happened...
            MHal_GFLIP_HandleVsyncLimitation(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx); //different chip maybe has different handle.
            pGOPDrvLocalCtx->bGFlipInVsyncLimitation[u32GopIdx] = TRUE;
            continue;
        }

        MHal_GFLIP_IntEnable(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx, TRUE);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Process Irq
/// @param  void
/// @return TRUE: success kickoff flip to gop
/// @return FALSE: no flip kickoff to gop
/// @attention
/// <b>[MxLib] <em>
///         step1: check src of vsync. interrupt
///         step2: check if any flip request in queue with this Gop
///         step3: check if TAG back
///         step4: set flip to GOP
///         step5: set ReadPtr to next, this entry consumed!
/// </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_GFLIP_ProcessIRQ(GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx)
{
    MS_U32 u32GopIdx, u32GwinIdx, u32Addr;
    MS_U8 u8MiuSel;

    for(u32GopIdx=GFLIP_GOP_IDX_4G; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        //step 1: check src of vsync. interrupt
        if(MHal_GFLIP_IsVSyncInt(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx) == FALSE)
        {
            continue;
        }

        for(u32GwinIdx=0; u32GwinIdx<MAX_GOP_GWIN; u32GwinIdx++)
        {
            //step 2:check if any flip request in queue with this Gop:
            if(pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] == pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]) //no any flip request in queue
            {
                continue;
            }

            //step 3: if get queue, check if TAG back.
            if(MHal_GFLIP_IsTagIDBack(&pGOPDrvLocalCtx->halCtxLocal, pGOPDrvLocalCtx->GFlipInfo[u32GopIdx][u32GwinIdx][pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u32TagId) == FALSE)
            {
                continue;
            }

            //Has Flip Request, and Tag Returned! we need programming flip address:
            //step 4: if Tag Back: set flip to GOP.
            u32Addr = pGOPDrvLocalCtx->GFlipInfo[u32GopIdx][u32GwinIdx][pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]].u64Addr;
             //GOP HW just read the relative offset of each MIU
            _phy_to_miu_offset(u8MiuSel, u32Addr, u32Addr);            
            MHal_GFLIP_SetFlipToGop(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx, u32GwinIdx, u32Addr);

            //Step 5: set ReadPtr to next, this entry consumed!
            pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx] = (pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]+1)%MAX_FLIP_ADDR_FIFO;
        }
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Handle GFLIP Interrupt notification handler
/// @param  irq                  \b IN: interrupt number
/// @param  devid                  \b IN: device id
/// @return IRQ_HANDLED
/// @attention
/// <b>[MxLib] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_GFLIP_IntHandler(InterruptNum eIntNum)
{
    //process gflip interrupt:
    if(pGFLIP_IntGOPDrvLocalCtx)
    {
        _MDrv_GFLIP_ProcessIRQ(pGFLIP_IntGOPDrvLocalCtx);

        //clear interrupt
        _MDrv_GFLIP_ClearIRQ(pGFLIP_IntGOPDrvLocalCtx);
    }
    MsOS_EnableInterrupt(E_INT_IRQ_GOP); //Need check if needed.
}

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// GFLIP Driver Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// GFlip Init(reset flip info ptr, interrupt enable, etc..)
/// @param  u32GopIdx                  \b IN:the idx of gop which need be initialized.
/// @return TRUE: success
/// @attention
/// <b>[Mxlib] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_Init(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_BOOL bEnableVsyncIntFlip)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(u32GopIdx >= MAX_GOP_SUPPORT) //overflow
    {
        return FALSE;
    }

    pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx] = bEnableVsyncIntFlip;

    if(TRUE == pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx])
    {

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_U32 _MDrv_GFLIPIO_Init(MS_U32 u32GopIdx);
    if(_MDrv_GFLIPIO_Init(u32GopIdx))
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif    
#elif defined(MSOS_TYPE_LINUX)
        if(0> pGOPDrvLocalCtx->s32FdGFlip)
        {
            pGOPDrvLocalCtx->s32FdGFlip = open("/dev/gflip", O_RDWR);
            if(0> pGOPDrvLocalCtx->s32FdGFlip)
            {
                GFLIP_PRINT("[%s][%d] open fail /dev/gflip\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
        }
        if(pGOPDrvLocalCtx->s32FdGFlip >= 0)
        {
            if(ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_INIT, &u32GopIdx))
            {
                GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
                close(pGOPDrvLocalCtx->s32FdGFlip);
                pGOPDrvLocalCtx->s32FdGFlip = -1;
                return FALSE;
            }          
        }
        #else
        {
            pGFLIP_IntGOPDrvLocalCtx = pGOPDrvLocalCtx;

            if((++pGOPDrvLocalCtx->u16GOPRefCnt[u32GopIdx]) > 1)
            {
                return TRUE;
            }

            //Attch IRQ:
            MsOS_AttachInterrupt(E_INT_IRQ_GOP, (InterruptCb)_MDrv_GFLIP_IntHandler);
            MsOS_EnableInterrupt(E_INT_IRQ_GOP); //Need check if needed.

            //reset flip Info:
            _MDrv_GFLIP_ResetFlipInfoPtr(pGOPCtx, u32GopIdx);
            //enable gop hw interrupt:
            MHal_GFLIP_IntEnable(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx,TRUE);
        }
        #endif
    }
    else
    {
        #ifdef MSOS_TYPE_LINUX
            if(0> pGOPDrvLocalCtx->s32FdGFlip)
            {
                pGOPDrvLocalCtx->s32FdGFlip = open("/dev/gflip", O_RDWR);
                if(0> pGOPDrvLocalCtx->s32FdGFlip)
                {//In order to Get DWin Int Infor from Kernel, Open the GFlip Dev but not enable GFilp.
                    GFLIP_PRINT("[%s][%d] open fail /dev/gflip\n", __FUNCTION__, __LINE__);
                    return FALSE;
                }
            }
        #elif defined(MSOS_TYPE_LINUX_KERNEL)
        #else
            pGFLIP_IntGOPDrvLocalCtx = pGOPDrvLocalCtx;

            if((++pGOPDrvLocalCtx->u16GOPRefCnt[u32GopIdx]) > 1)
            {
                return TRUE;
            }

            _MDrv_GFLIP_ResetFlipInfoPtr(pGOPCtx, u32GopIdx);
        #endif
        MHal_GFLIP_IntEnable(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx, FALSE);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// GFlip DeInit(interrupt disable)
/// @param  u32GopIdx                  \b IN:the idx of gop which need be de-initialized.
/// @return TRUE: success
/// @attention
/// <b>[MxLib] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_DeInit(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(u32GopIdx >= MAX_GOP_SUPPORT) //overflow
    {
        return FALSE;
    }

    if(TRUE == pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx])
    {
        #ifdef MSOS_TYPE_LINUX
        {
            close(pGOPDrvLocalCtx->s32FdGFlip);
            pGOPDrvLocalCtx->s32FdGFlip = -1;
            pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx] = FALSE; //Application not shared
        }
        #elif defined(MSOS_TYPE_LINUX_KERNEL)
        {
            pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx] = FALSE; //Application not shared
        }
        #else
        {
            if(0 == (--pGOPDrvLocalCtx->u16GOPRefCnt[u32GopIdx]))
            {
                MHal_GFLIP_IntEnable(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx, FALSE);
                MsOS_DisableInterrupt(E_INT_IRQ_GOP);
                MsOS_DetachInterrupt(E_INT_IRQ_GOP);
                pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx] = FALSE;
            }
        }
        #endif
    }
    else
    {
        #ifdef MSOS_TYPE_LINUX
        {
            close(pGOPDrvLocalCtx->s32FdGFlip);
            pGOPDrvLocalCtx->s32FdGFlip = -1;
        }
        #elif defined(MSOS_TYPE_LINUX_KERNEL)
        #else
            pGOPDrvLocalCtx->u16GOPRefCnt[u32GopIdx]--;
        #endif
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Set Flip Info(flip request) to GFlip
/// @param  u32GopIdx       \b IN: the gop which flip kickoff to
/// @param  u32GwinIdx       \b IN: the gwin which flip kickoff to
/// @param  u32Addr             \b IN: the flip address which will be kickoff
/// @param  u32TagId            \b IN: the TagId need be waited before flip
/// @param  u32QEntry           \b IN: the queued flips, if current queued flips >= u32QEntry, won't set this flip request to gop
/// @param  u32QEntry           \b out: the current queued flips in GFLIP
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set flip info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_SetMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_GFLIP_MULTIINFO* pMultiinfo)
{
        MS_BOOL bRet = TRUE;

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_SetMultiFlipInfo(MS_GFLIP_MULTIINFO * pMultiFlipInfo);
    if(!_MDrv_GFLIP_SetMultiFlipInfo(pMultiinfo))    
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif    
#elif defined(MSOS_TYPE_LINUX)
        GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETMULTIFLIPINFO,pMultiinfo))
        {
            GFLIP_PRINT("[%s][%d] fail!!!!\n",__FUNCTION__,__LINE__);
            bRet = FALSE;
        }
    #endif
    return bRet;
}

MS_BOOL MDrv_GFLIP_SetTLBMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_TLB_GFLIP_MULTIINFO* pTLBMultiinfo)
{
        MS_BOOL bRet = TRUE;

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_SetTLBMultiFlipInfo(MS_TLB_GFLIP_MULTIINFO * pTLBMultiFlipInfo);
    if(!_MDrv_GFLIP_SetTLBMultiFlipInfo(pTLBMultiinfo))
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif    
#elif defined(MSOS_TYPE_LINUX)
        GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETTLBMULTIFLIPINFO,pTLBMultiinfo))
        {
            GFLIP_PRINT("[%s][%d]  fail!!!!\n",__FUNCTION__,__LINE__);
            bRet = FALSE;
        }
    #endif
    return bRet;
}

MS_BOOL MDrv_GFLIP_SetFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY u64MainAddress, MS_PHY u64SubAddress, MS_U32 u32TagId, MS_U32 * u32QEntry)
{
   MS_U16 u16WordUnit=0;
   GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
   MS_U8 u8MiuSel=0xFF;
   UNUSED(u8MiuSel);

    if(pGOPDrvLocalCtx->apiCtxLocal.pGOPCtxShared->bPixelMode[u32GopIdx] == TRUE)
    {
        u16WordUnit = 1;
    }
    else
    {
        u16WordUnit = GOP_WordUnit;
    }

    //GOP HW just read the relative offset of each MIU
    _phy_to_miu_offset(u8MiuSel, u64MainAddress, u64MainAddress);    
    if(TRUE == pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx])
    {
        if((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
        {
            return FALSE;
        }

        #if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
        {
            MS_GFLIP_INFO stFlinInfo;
           MS_BOOL bRet = TRUE;
            #ifdef MSOS_TYPE_LINUX
            if(0 > pGOPDrvLocalCtx->s32FdGFlip)
            {
                GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            #endif

            stFlinInfo.u32GopIdx = u32GopIdx;
            stFlinInfo.u32GwinIdx = u32GwinIdx;
            stFlinInfo.u64Addr= u64MainAddress;
            stFlinInfo.u32TagId= u32TagId;
            stFlinInfo.u32QEntry= *u32QEntry;
            stFlinInfo.u32Result = TRUE;
            if((pGOPCtx->pGOPCtxShared->GOP_StereoMode[u32GopIdx] != E_DRV_GOP_3D_DISABLE) &&
               (u64SubAddress != 0))
            {
                MS_GFLIP_3DINFO st3DFlipInfo;
                //GOP HW just read the relative offset of each MIU
                _phy_to_miu_offset(u8MiuSel, u64SubAddress, u64SubAddress);
                memcpy(&st3DFlipInfo, &stFlinInfo, sizeof(MS_GFLIP_INFO));
                st3DFlipInfo.u64SubAddr = u64SubAddress;

                #ifdef MSOS_TYPE_LINUX_KERNEL
                #if GFLIP_ENABLE
                    extern MS_BOOL _MDrv_GFLIP_SetFlipInfo(MS_U32 u32GopIdx,MS_U32 u32GwinIdx,MS_PHY u32MainAddr,MS_PHY u32SubAddr,MS_U32 u32TagId,MS_U32 * u32QEntry,MS_U32 * u32Result);
                    if(!_MDrv_GFLIP_SetFlipInfo(stFlinInfo.u32GopIdx, stFlinInfo.u32GwinIdx, stFlinInfo.u64Addr, st3DFlipInfo.u64SubAddr, stFlinInfo.u32TagId, &(stFlinInfo.u32QEntry), &(stFlinInfo.u32Result)))
                    {
                        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
                        return FALSE;
                    }
                #endif    
                #elif defined(MSOS_TYPE_LINUX)
                
                if((pGOPCtx->pGOPCtxShared->bTLB[u32GopIdx]))
                {
                    MS_TLB_GFLIP_3DINFO stTLB3DFlipInfo;
                    //GOP HW just read the relative offset of each MIU
                    memcpy(&stTLB3DFlipInfo, &st3DFlipInfo, sizeof(MS_GFLIP_INFO));
                    stTLB3DFlipInfo.bTLBEnable = pGOPCtx->pGOPCtxShared->bTLB[u32GopIdx];
                    stTLB3DFlipInfo.u64TLBAddr = pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx];
                    if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_TLBSET3DFLIPINFO, &stTLB3DFlipInfo))
                    {
                        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
                        bRet = FALSE;
                    }
                }
                else
                {
                    if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SET3DFLIPINFO, &st3DFlipInfo))
                    {
                        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
                        bRet = FALSE;
                    }
                }                    
                #endif
                if(bRet)
                {
                    memcpy(&stFlinInfo, &st3DFlipInfo, sizeof(MS_GFLIP_INFO));
                }
                else
                {
                    return FALSE;
                }
            }
            else
            {
                #ifdef MSOS_TYPE_LINUX_KERNEL
                #if GFLIP_ENABLE
                extern MS_BOOL _MDrv_GFLIP_SetFlipInfo(MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY u32MainAddr, MS_PHY u32SubAddr, MS_U32 u32TagId, MS_U32 * u32QEntry, MS_U32 *u32Result);
                if(!_MDrv_GFLIP_SetFlipInfo(stFlinInfo.u32GopIdx, stFlinInfo.u32GwinIdx, stFlinInfo.u64Addr, 0, stFlinInfo.u32TagId, &stFlinInfo.u32QEntry, &stFlinInfo.u32Result))
                {
                    GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
                    return FALSE;
                }
                #endif    
                #elif defined(MSOS_TYPE_LINUX)
                if((pGOPCtx->pGOPCtxShared->bTLB[u32GopIdx]))
                {
                    MS_TLB_GFLIP_INFO stTLBFlinInfo;
                    //GOP HW just read the relative offset of each MIU
                    memcpy(&stTLBFlinInfo, &stFlinInfo, sizeof(MS_GFLIP_INFO));
                    stTLBFlinInfo.bTLBEnable = pGOPCtx->pGOPCtxShared->bTLB[u32GopIdx];
                    stTLBFlinInfo.u64TLBAddr = pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx];
                    if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_TLBSETFLIPINFO, &stTLBFlinInfo))
                    {
                        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
                        bRet = FALSE;
                    }
                }
                else
                {
                    if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETFLIPINFO, &stFlinInfo))
                    {
                        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
                        bRet = FALSE;
                    }
                }
                #endif   
            }
            if(bRet)
            {
                *u32QEntry = stFlinInfo.u32QEntry;
                return stFlinInfo.u32Result;
            }
            else
            {
                return FALSE;
            }
        }
        #else
        {
            MS_U32 u32NextWritePtr;
            MS_U32 u32QCnt = 0;

            if(TRUE == pGOPDrvLocalCtx->bGFlipInVsyncLimitation[u32GopIdx])
            { //in handling vsync limitation status, just print the error and return true to avoid block app:
                GFLIP_PRINT("\n !!! [%s][%d]Error. No Signal for GOP Clock Source and GFlip Vsync Interrupt disabled!!!\n",__FUNCTION__,__LINE__);
                return TRUE;
            }

            u32NextWritePtr = (pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]+1) % MAX_FLIP_ADDR_FIFO;
            if(u32NextWritePtr !=pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx]) //can write
            {
                if(pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
                {
                    u32QCnt = pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] -pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
                }
                else
                {
                    u32QCnt = MAX_FLIP_ADDR_FIFO + pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
                }

                if(u32QCnt >= *u32QEntry )
                {
                    *u32QEntry = u32QCnt;
                    return FALSE;
                }

                pGOPDrvLocalCtx->GFlipInfo[u32GopIdx][u32GwinIdx][pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u64Addr = (u64MainAddress/u16WordUnit);
                pGOPDrvLocalCtx->GFlipInfo[u32GopIdx][u32GwinIdx][pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx]].u32TagId= u32TagId;
                pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] = u32NextWritePtr;

                //Hold on when New QCnt can't match requested QCnt:
                //Fix issue if we set Flip Info and back which leads to App write to on-show window.
                while(TRUE)
                {
                    if(pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] >= pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx])
                    {
                        u32QCnt = pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] -pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
                    }
                    else
                    {
                        u32QCnt = MAX_FLIP_ADDR_FIFO + pGOPDrvLocalCtx->u32GFlipInfoWritePtr[u32GopIdx][u32GwinIdx] - pGOPDrvLocalCtx->u32GFlipInfoReadPtr[u32GopIdx][u32GwinIdx];
                    }

                    if( u32QCnt < *u32QEntry )
                    {
                        break;
                    }
                }

                //since do not do the sync. with mutex/spinlock, the return of avaiable queue number maybe not accurate.
                *u32QEntry = (u32QCnt + 1);

                return TRUE;
            }
            else
            {
                *u32QEntry = MAX_FLIP_ADDR_FIFO-1;
                return FALSE;
            }
        }
        #endif
    }
    else
    {
        MS_PHY u64tmp;
        MS_U32 u32BankOffSet=0;
        _GetBnkOfstByGop(u32GopIdx, &u32BankOffSet);

        if((pGOPCtx->pGOPCtxShared->bTLB[u32GopIdx]))
        {
            if(pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx])
            {
                u64tmp = pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx] + (u64MainAddress/PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                u64tmp /= ADDRESSING_8BYTE_UNIT;
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+REG_TLB_BASE_ADDR_L, u64tmp&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+REG_TLB_BASE_ADDR_H, u64tmp>>16, GOP_REG_WORD_MASK);
                u64tmp = u64MainAddress % (PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                u64tmp = u64tmp /u16WordUnit;
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_DRAM_RBLK_L(u32GwinIdx), u64tmp);
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_DRAM_RBLK_H(u32GwinIdx), 0);                
            }

            if((pGOPCtx->pGOPCtxShared->GOP_StereoMode[u32GopIdx] != E_DRV_GOP_3D_DISABLE)&&(pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx]!= 0))
            {
                u64tmp = pGOPCtx->pGOPCtxShared->u64TLBAddress[u32GopIdx] + (u64SubAddress/PAGE_SIZE)*TLB_PER_ENTRY_SIZE;
                u64tmp /= ADDRESSING_8BYTE_UNIT;
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_L, u64tmp&GOP_REG_WORD_MASK, GOP_REG_WORD_MASK);
                HAL_GOP_Write16Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet+REG_TLB_BASE_ADDR_RVIEW_H, u64tmp>>16, GOP_REG_WORD_MASK);
                u64tmp = u64SubAddress % (PAGE_SIZE*PER_MIU_TLB_ENTRY_COUNT);
                u64tmp = u64tmp /u16WordUnit;
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_3DOSD_SUB_RBLK_L(u32GwinIdx), u64tmp);
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_3DOSD_SUB_RBLK_H(u32GwinIdx), 0);                
            }
        }
        else
        {
            u64tmp = u64MainAddress /u16WordUnit;
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_DRAM_RBLK_L(u32GwinIdx), u64tmp);
            HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_DRAM_RBLK_H(u32GwinIdx), u64tmp>>16);
            if((pGOPCtx->pGOPCtxShared->GOP_StereoMode[u32GopIdx] != E_DRV_GOP_3D_DISABLE) && (u64SubAddress != 0))
            {
                u64tmp = u64SubAddress /u16WordUnit;
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_3DOSD_SUB_RBLK_L(u32GwinIdx), u64tmp);
                HAL_GOP_Write32Reg(&pGOPDrvLocalCtx->halCtxLocal, u32BankOffSet + GOP_4G_3DOSD_SUB_RBLK_H(u32GwinIdx), u64tmp>>16);                
            }
        }

        MDrv_GOP_GWIN_UpdateReg(pGOPCtx,u32GopIdx);

        return TRUE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Gwin Info(3D request) to GFlip
/// @param  u32GopIdx       \b IN: the gop which flip kickoff to
/// @param  u32GwinIdx       \b IN: the gwin which flip kickoff to
/// @param  u32Addr             \b IN: the flip address which will be kickoff
/// @param  u32TagId            \b IN: the TagId need be waited before flip
/// @param  u32QEntry           \b IN: the queued flips, if current queued flips >= u32QEntry, won't set this flip request to gop
/// @param  u32QEntry           \b out: the current queued flips in GFLIP
/// @return TRUE: DDI call success
/// @attention
/// <b>[MxLib] <em>Flip when vsync interrupt, and the DDI used for set flip info to GFlip. </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_SetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopIdx, MS_U8 u8GwinIdx,DRV_GOP_GWIN_INFO* pinfo)
{
    MS_GWIN_INFO MsGwinInfo;

    MsGwinInfo.u8GopIdx = u8GopIdx;
    MsGwinInfo.u8GwinIdx = u8GwinIdx;
    MsGwinInfo.u64Addr = pinfo->u64DRAMRBlkStart;
    MsGwinInfo.u16X = pinfo->u16DispHPixelStart;
    MsGwinInfo.u16Y = pinfo->u16DispVPixelStart;
    MsGwinInfo.u16W = pinfo->u16RBlkHPixSize;
    MsGwinInfo.u16H = pinfo->u16RBlkVPixSize;
    MsGwinInfo.clrType =(GFLIP_COLOR_TYPE)pinfo->clrType;
#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_SetGwinInfo(MS_GWIN_INFO stGwinInfo);
    if(!_MDrv_GFLIP_SetGwinInfo(MsGwinInfo))    
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif    
#elif defined(MSOS_TYPE_LINUX)
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    if(0 > pGOPDrvLocalCtx->s32FdGFlip)
    {
        GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    if(ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETGWININFO, &MsGwinInfo))
    {
        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

#endif
return TRUE;

}
//-------------------------------------------------------------------------------------------------
/// Restore HW Limitation -- Vsync Limitation.
/// @return TRUE: success
/// @attention
/// <b>[Mxlib] <em></em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_GFLIP_RestoreFromVsyncLimitation(MS_GOP_CTX_LOCAL *pGOPCtx)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    MS_U32 u32GopIdx;
    MS_BOOL bHasSignal = TRUE;
    UNUSED(bHasSignal);

    for(u32GopIdx=0; u32GopIdx<MAX_GOP_SUPPORT; u32GopIdx++)
    {
        //Only GFlip has the issue.
        if(TRUE == pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx])
        {
#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
            extern MS_BOOL _MDrv_GFLIP_RestoreFromVsyncLimitation(void);
            if(!_MDrv_GFLIP_RestoreFromVsyncLimitation())
            {
                GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
#endif            
#elif defined(MSOS_TYPE_LINUX)
            if(0 > pGOPDrvLocalCtx->s32FdGFlip)
            {
                GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
                return FALSE;
            }
            if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETINPUTSIGSTATUS, &bHasSignal))
            {
                GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
                return FALSE;
            }

            break; //Only one call into Kernel could restore All.

            #else //GFlip UMD Handling:
            if(TRUE == pGOPDrvLocalCtx->bGFlipInVsyncLimitation[u32GopIdx])
            {
                MHal_GFLIP_RestoreFromVsyncLimitation(&pGOPDrvLocalCtx->halCtxLocal, u32GopIdx);
                pGOPDrvLocalCtx->bGFlipInVsyncLimitation[u32GopIdx] = FALSE;
            }
            #endif
        }
    }

    return TRUE;
}

/******************************************************************************/
/// Get DWin Interrupt Information
/// @param pGFlipDWinIntInfo \b OUT: dwin interrup flag
/******************************************************************************/
MS_BOOL MDrv_GFLIP_GetDWinIntInfo(MS_GOP_CTX_LOCAL *pGOPCtx, GFLIP_DWININT_INFO *pGFlipDWinIntInfo,MS_U32 u32Timeout)
{
    MS_GFLIP_DWININT_INFO2 stGFlipDWinIntInfo;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

    stGFlipDWinIntInfo.bResetDWinIntInfo = TRUE;
    stGFlipDWinIntInfo.u32Timeout = u32Timeout;

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_GetDWinIntInfo(GFLIP_DWININT_INFO * pGFlipDWinIntInfo,MS_BOOL bResetDWinIntInfo,MS_U32 u32Timeout);
    if(!_MDrv_GFLIP_GetDWinIntInfo(pGFlipDWinIntInfo, stGFlipDWinIntInfo.bResetDWinIntInfo, stGFlipDWinIntInfo.u32Timeout))
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    pGFlipDWinIntInfo->u8DWinIntInfo = stGFlipDWinIntInfo.gflipDWinIntInfo.u8DWinIntInfo;
#endif    
#elif defined(MSOS_TYPE_LINUX)
    if(0 > pGOPDrvLocalCtx->s32FdGFlip)
    {
        GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_GETDWININTINFO2, &stGFlipDWinIntInfo))
    {
        GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    pGFlipDWinIntInfo->u8DWinIntInfo = stGFlipDWinIntInfo.gflipDWinIntInfo.u8DWinIntInfo;
#else //will Add GFlip Handle in UMD GFlip later.
    pGFlipDWinIntInfo->u8DWinIntInfo = pGOPDrvLocalCtx->gflipDWinIntInfo.u8DWinIntInfo;
    pGOPDrvLocalCtx->gflipDWinIntInfo.u8DWinIntInfo = 0x0;
#endif
    return TRUE;
}

/******************************************************************************/
/// Enable VE capture
/// @param pstVECapState \b OUT: VECAPTURESTATE flag
/******************************************************************************/
MS_BOOL MDrv_GFLIP_EnaVECapture(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(pGOPDrvLocalCtx != NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
#ifdef CONFIG_MSTAR_VE_CAPTURE_SUPPORT
        extern MS_BOOL _MDrv_GFLIPIO_EnableVECapture(MS_GFLIP_VECAPTURESTATE * pVECaptureState);
        if(_MDrv_GFLIPIO_EnableVECapture((MS_GFLIP_VECAPTURESTATE*)pstVECapState))
        {
            GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
#endif
#endif
#elif defined(MSOS_TYPE_LINUX)
        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_ENABLEVECAPTURE, (PMS_GFLIP_VECAPTURESTATE)pstVECapState))
        {
            GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        return pstVECapState->u8Result;
#endif
    }
    return TRUE; //Unsupport now
}

/******************************************************************************/
/// Get VE capture state
/// @param pstVECapState \b OUT: VECAPTURESTATE flag
/******************************************************************************/
MS_BOOL MDrv_GFLIP_GetVECaptureState(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(pGOPDrvLocalCtx != NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
#ifdef CONFIG_MSTAR_VE_CAPTURE_SUPPORT
    extern MS_BOOL MDrv_GFLIP_GetVECapCurState(MS_BOOL * pbEna,MS_U8 * pu8FramCount);
    if(MDrv_GFLIP_GetVECapCurState(&pstVECapState->bEnable, &pstVECapState->u8FrameCount))
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#endif
#endif
#elif defined(MSOS_TYPE_LINUX)
        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_GETVECAPTURESTATE, (PMS_GFLIP_VECAPTURESTATE)pstVECapState))
        {
            GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        //printf("GFLP=%u\n", pstVECapState->u8FrameCount);
        return pstVECapState->u8Result;
#endif
    }
    return TRUE; //Unsupport now
}

/******************************************************************************/
/// Get VE capture state
/// @param pstVECapState \b OUT: VECAPTURESTATE flag
/******************************************************************************/
MS_BOOL MDrv_GFLIP_VECaptureWaitOnFrame(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState)
{
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;
    if(pGOPDrvLocalCtx != NULL)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
#ifdef CONFIG_MSTAR_VE_CAPTURE_SUPPORT
        extern MS_BOOL MDrv_GFLIP_VECapWaitOnFrame(MS_BOOL * pbEna,MS_U8 * pu8FramNum);
        pstVECapState->u8Result= MDrv_GFLIP_VECapWaitOnFrame(&pstVECapState->bEnable, &pstVECapState->u8FrameCount);
            return  pstVECapState->u8Result;
#endif
#endif
#elif defined(MSOS_TYPE_LINUX)
        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }

        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_VECAPTUREWAITONFRAME, (PMS_GFLIP_VECAPTURESTATE)pstVECapState))
        {
            GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        //printf("GFLP=%u\n", pstVECapState->u8FrameCount);
        return pstVECapState->u8Result;
#endif
    }
    return TRUE; //Unsupport now
}


/***********************************************************************/
/// GFlip Clear Flip Queue(Clear the Flip queue)
/// @param  u32GopIdx                  \b IN:the idx of gop which Flip kick off to
/// @param  u32GwinIdx                  \b IN:the idx of gwin which Flip kick off to
/// @return TRUE: success
/// <b>[Mxlib] <em></em></b>
/*******************************************************************88*/
MS_BOOL MDrv_GFLIP_ClearFlipQueue(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx)
{
    MS_GFLIP_GOPGWINIDX stFlipQueueIdx;
    GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

        if((u32GopIdx >= MAX_GOP_SUPPORT) || (u32GwinIdx >= MAX_GOP_GWIN)) //overflow
        {
            return FALSE;
        }

    if(TRUE == pGOPDrvLocalCtx->bEnableVsyncIntFlip[u32GopIdx])
    {

        stFlipQueueIdx.u32GopIdx = u32GopIdx;
        stFlipQueueIdx.u32GwinIdx = u32GwinIdx;
        stFlipQueueIdx.u32Result = TRUE;

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_ClearFlipQueue(MS_U32 u32GopIdx,MS_U32 u32GwinIdx);
    if(!_MDrv_GFLIP_ClearFlipQueue(u32GopIdx, u32GwinIdx))    
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    return stFlipQueueIdx.u32Result;
#endif    
#elif defined(MSOS_TYPE_LINUX)
        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_CLEARFLIPQUEUE, &stFlipQueueIdx))
        {
            GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        return stFlipQueueIdx.u32Result;
#else
    return FALSE;
#endif
    }
    else
    {
       return FALSE;
    }
}

//******************************************************************************
/// notify kernel about 3D gpio pin
/// @param pGOPCtx      \b IN: gop context
/// @param u32GPIO3DPin \b IN: GPIO 3D pin
/// @return TURE: Success FALSE: Fail
//******************************************************************************
MS_BOOL MDrv_GFLIP_SetGPIO3DPin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GPIO3DPin)
{
        MS_GFLIP_INFO stFlinInfo;

        stFlinInfo.u32GopIdx = 0;
        stFlinInfo.u32GwinIdx = 0;
        stFlinInfo.u64Addr= u32GPIO3DPin;
        stFlinInfo.u32TagId= 0;
        stFlinInfo.u32QEntry= 0;
        stFlinInfo.u32Result = TRUE;

#ifdef MSOS_TYPE_LINUX_KERNEL
#if GFLIP_ENABLE
    extern MS_BOOL _MDrv_GFLIP_SetGPIO3DPin(MS_PHY u32Addr,MS_U32 * u32Result);
    if(!_MDrv_GFLIP_SetGPIO3DPin(stFlinInfo.u64Addr, &stFlinInfo.u32Result))    
    {
        GFLIP_PRINT("[%s][%d] failed!\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
#else
    return FALSE;
#endif    
#elif defined(MSOS_TYPE_LINUX)
        GOP_CTX_DRV_LOCAL*pGOPDrvLocalCtx = (GOP_CTX_DRV_LOCAL*)pGOPCtx;

        if(0 > pGOPDrvLocalCtx->s32FdGFlip)
        {
            GFLIP_PRINT("[%s][%d]GLIP device not openedl!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        if (ioctl(pGOPDrvLocalCtx->s32FdGFlip, MDRV_GFLIP_IOC_SETGPIO3DPIN, &stFlinInfo))
        {
            GFLIP_PRINT("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
            return FALSE;
        }
        return TRUE;
#else
    return FALSE;
#endif
    return TRUE;
}

