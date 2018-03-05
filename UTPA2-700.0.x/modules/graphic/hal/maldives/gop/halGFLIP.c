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
///
/// file    halGFLIP.c
/// @brief  MStar GFLIP DDI HAL LEVEL
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _HAL_GFLIP_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "regGOP.h"
#include "drvGFLIP.h"
#include "halGOP.h"
#include "halGFLIP.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Debug Macros
//=============================================================================
#define GFLIPHAL_DEBUG
#ifdef GFLIPHAL_DEBUG
    #define GFLIPHAL_PRINT(fmt, args...)      printf("[GFLIP (HAL Driver)][%05d] " fmt, __LINE__, ## args)
    #define GFLIPHAL_ASSERT(_cnd, _fmt, _args...)                   \
                                    if (!(_cnd)) {              \
                                        GFLIPHAL_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define GFLIPHAL_PRINT(_fmt, _args...)
    #define GFLIPHAL_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================
#define GFLIP_GOP_BANKOFFSET      0x30000

//Mask bits of #define REG_GOP_INT                   0x0008
#define GOP_INTMASK_VS0             BIT(0)
#define GOP_INTMASK_VS1             BIT(1)
#define GOP_INT_VS0                      BIT(8)
#define GOP_INT_VS1                      BIT(9)

//Mask bits of Gop1GX Int Flag of #define REG_GOP_MUX              0x007e
#define GOP1GX_INT_FLAG                    BIT(14)

//Mask bits of Int Flag of #define REG_GOP_WR_ACK              0x007f
#define GOP4G_INT_FLAG                    BIT(4)
#define GOP2G_INT_FLAG                    BIT(5)
#define GOPD_INT_FLAG                      BIT(6)
#define GOP1G_INT_FLAG                    BIT(7)

//=============================================================================
// Local Variables
//=============================================================================

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
MS_S32 _MHal_GFLIP_DirectSerialDiff(MS_U16 u16TagID1,  MS_U16 u16TagID2);
MS_U32 _MHal_GFLIP_GetValidGWinIDPerGOPIdx(MS_U32 u32GopIdx, MS_U32 u32GwinIdx);

//-------------------------------------------------------------------------------------------------
/// Differ Two TagIDs.
/// @param  u16TagID1                  \b IN: The First TagID
/// @param  u16TagID2                  \b IN: The Second TagID
/// @return MS_S32: The different of two IDs.
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_S32 _MHal_GFLIP_DirectSerialDiff(MS_U16 u16TagID1,  MS_U16 u16TagID2)
{
    if(u16TagID1 < u16TagID2)
    {
        if((u16TagID2-u16TagID1)>0x7FFF)
         {
             return (MS_S32)(0xFFFFUL-u16TagID2+u16TagID1+1);
         }
        else
            return -(MS_S32)(u16TagID2-u16TagID1);
    }
    else
    {
        if((u16TagID1-u16TagID2)>0x7FFF)
         {
             return -(MS_S32)(0xFFFFUL-u16TagID1+u16TagID2+1);
         }
        else
            return (MS_S32)(u16TagID1-u16TagID2);
    }
}

//-------------------------------------------------------------------------------------------------
/// Get Valid GWinID per GopIdx.
/// @param  u32GopIdx                  \b IN: The gop idx value
/// @param  u32GwinIdx                  \b IN: The gop gwin idx value,
/// @return MS_U32 The valid GWinID for related Gop
/// @attention
/// <b>[GFLIP] <em>GOP0: 0/1/2/3; GOP1: 0/1; GOP2: 1; GOP3: 1</em></b>
//-------------------------------------------------------------------------------------------------
MS_U32 _MHal_GFLIP_GetValidGWinIDPerGOPIdx(MS_U32 u32GopIdx, MS_U32 u32GwinIdx)
{
    switch(u32GopIdx)
    {
        case GFLIP_GOP_IDX_4G:
        case GFLIP_GOP_IDX_2G:
            u32GwinIdx = u32GwinIdx % 2;
            break;
        case GFLIP_GOP_IDX_1G:
        case GFLIP_GOP_IDX_1GX:
            u32GwinIdx = 0; //only GWin 1 is valid;
            break;
        default:
            break;
    }

    return u32GwinIdx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// HAL Driver Function
//=============================================================================
//-------------------------------------------------------------------------------------------------
/// Enable/Disable Vsync. Interrupt of gop.
/// @param  u32GopIdx                  \b IN: The gop idx which need enable/disable vsync. interrupt
/// @param bEnable                          \b IN: enable or disable
/// @return TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IntEnable(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_BOOL bEnable)
{
    MS_U16 u16RegVal;

    if(bEnable)
    {
        u16RegVal = GOP_INTMASK_VS1;
    }
    else
    {
        u16RegVal = GOP_INTMASK_VS0 | GOP_INTMASK_VS1;
    }

    HAL_GOP_Write16Reg(pGOPHalLocal, (GFLIP_GOP_BANKOFFSET*u32GopIdx) + GOP_4G_INT, u16RegVal, (GOP_INTMASK_VS0 | GOP_INTMASK_VS1));

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// check if the vsync. interrupt
/// @param  void
/// @return MS_BOOL: TRUE/FALSE
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IsVSyncInt(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx)
{
    MS_U32 u32RegAddr;
    MS_U16 u16RegVal=0;

    switch(u32GopIdx)
    {
        default:
        case GFLIP_GOP_IDX_4G:
            u32RegAddr = GOP_4G_INT;
            break;
        case GFLIP_GOP_IDX_2G:
            u32RegAddr = GOP_2G_INT;
            break;
        case GFLIP_GOP_IDX_1G:
            u32RegAddr = GOP_1G_INT;
            break;
        case GFLIP_GOP_IDX_1GX:
            u32RegAddr = GOP_1GX_INT;
            break;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, u32RegAddr, &u16RegVal);

    if(u16RegVal & GOP_INT_VS0)
    {
        return TRUE;
    }

    return FALSE;
}

//-------------------------------------------------------------------------------------------------
/// Kick off Flip to Gop HW.
/// @param  u32GopIdx                  \b IN: The gop idx who is the flip owner
/// @param  u32GwinIdx                  \b IN: The gwin idx who is the flip owner
/// @param  u32Addr                  \b IN: The flip address
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_SetFlipToGop(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_U32 u32Addr)
{
    MS_U16 u16FWR=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_BAK_SEL, &u16FWR);

    u32GwinIdx = _MHal_GFLIP_GetValidGWinIDPerGOPIdx(u32GopIdx, u32GwinIdx);

    if(u32GopIdx)
    {
         HAL_GOP_Write32Reg(pGOPHalLocal, GOP_2G_DRAM_RBLK_L(u32GwinIdx), u32Addr);
    }
    else
    {
         HAL_GOP_Write32Reg(pGOPHalLocal, GOP_4G_DRAM_RBLK_L(u32GwinIdx), u32Addr);
    }

    if((u16FWR&GOP_VAL_FWR) == 0)
    {
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, GOP_VAL_FWR, GOP_REG_HW_MASK);
        HAL_GOP_Write16Reg(pGOPHalLocal, GOP_BAK_SEL, 0x0000, GOP_REG_HW_MASK);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Clear DWinIRQ if It happened.
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_ClearDWINIRQ(GOP_CTX_HAL_LOCAL *pGOPHalLocal, GFLIP_DWININT_INFO *pGFlipDWinIntInfo)
{
    MS_U16 u16ValEn=0, u16ValDe;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_BAK_SEL, &u16ValEn);

    if(!(u16ValEn & GOPD_INT_FLAG))
    {
        return FALSE;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_DW_INT_MASK, &u16ValEn);
    u16ValDe = u16ValEn;

    //Check and Clear DWin IRQs:
    if(u16ValEn & GOP_DWIN_INT_WADR)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_WADR;
        u16ValDe |= GOP_DWIN_INTMASK_WADR;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntWADR = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_PROG)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_PROG;
        u16ValDe |= GOP_DWIN_INTMASK_PROG;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntPROG = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_TF)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_TF;
        u16ValDe |= GOP_DWIN_INTMASK_TF;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntTF = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_BF)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_BF;
        u16ValDe |= GOP_DWIN_INTMASK_BF;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntBF = 0x1;
    }

    if(u16ValEn & GOP_DWIN_INT_VS)
    {
        u16ValEn &= ~GOP_DWIN_INTMASK_VS;
        u16ValDe |= GOP_DWIN_INTMASK_VS;
        pGFlipDWinIntInfo->sDwinIntInfo.bDWinIntVS = 0x1;
    }

    //Disable Interrupt:
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_DW_INT_MASK, u16ValDe, GOP_REG_LW_MASK);
    //Enable Interrupt:
    HAL_GOP_Write16Reg(pGOPHalLocal, GOP_DW_INT_MASK, u16ValEn, GOP_REG_LW_MASK);

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Handle HW Limitation -- Vsync Limitation.
/// @param  u32GopIdx                  \b IN: The gop idx who is the owner
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em>The Limitation is found on U3/T2/T3. When GOP Src Clock is no signal and GOP
///     Vsync is on, Then there will be no chance to Reset GOP Vsync Interrupt. this will leads to
///     system looks like hang(busy handling interrupt always). T2/T3/U3 will Switch GOP Dst
///     to OP to fix the issue and later chips could Reset GOP to clear interrupt Bit.</em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_HandleVsyncLimitation(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx)
{
    //Save Current Dst Clock Src:
    HAL_GOP_GetGOPDst(pGOPHalLocal, u32GopIdx, &pGOPHalLocal->drvGFlipGOPDst[u32GopIdx]);
    //Set Dst Clock Src to OP:
    HAL_GOP_SetGOPClk(pGOPHalLocal, u32GopIdx, E_DRV_GOP_DST_OP0);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Restore HW Limitation -- Vsync Limitation.
/// @param  u32GopIdx                  \b IN: The gop idx who is the owner
/// @return TRUE: success
/// @return FALSE: fail
/// @attention
/// <b>[GFLIP] <em>The Limitation is found on U3/T2/T3. When GOP Src Clock is no signal and GOP
///     Vsync is on, Then there will be no chance to Reset GOP Vsync Interrupt. this will leads to
///     system looks like hang(busy handling interrupt always). T2/T3/U3 will Switch GOP Dst
///     to OP to fix the issue and later chips could Reset GOP to clear interrupt Bit.</em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_RestoreFromVsyncLimitation(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx)
{
    //Restore Dst Clock Src:
    HAL_GOP_SetGOPClk(pGOPHalLocal, u32GopIdx, pGOPHalLocal->drvGFlipGOPDst[u32GopIdx]);

    //Restore Interrupt:
    MHal_GFLIP_IntEnable(pGOPHalLocal, u32GopIdx, TRUE);

    //Restore Current Dst Clock Src to OP:
    pGOPHalLocal->drvGFlipGOPDst[u32GopIdx] = E_DRV_GOP_DST_OP0;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Check If the gived TagId has back or not.
/// @param  u32TagId                  \b IN: The TagId which need to be check
/// @return TRUE: back
/// @return FALSE: not back
/// @attention
/// <b>[GFLIP] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_BOOL MHal_GFLIP_IsTagIDBack(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U16 u16TagId)
{
    MS_U16 u16TagIDHW=0;
    MS_U16 u16RegVal=0;
    MS_U32 u32RegVal=0;

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_TAG, &u16TagIDHW);

    if(_MHal_GFLIP_DirectSerialDiff(u16TagIDHW, u16TagId) >= 0)
    {    //TagID is back!
          // printk("tagIDHW = %04x\n", tagID_HW);
        return TRUE;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_STATUS, &u16RegVal);

    if((u16RegVal & GOP_GE_CMDQ1_STATUS) < (16UL<<3)) //still has commands in queue.
    {
        return FALSE;
    }

    if((u16RegVal & GOP_GE_CMDQ2_STATUS) < (16UL<<11)) //still has commands in queue.
    {
        return FALSE;
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_FMT_BLT, &u16RegVal);

    if(u16RegVal & GOP_GE_EN_VCMDQ) //still has commands in VCMDQ.
    {
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_VQ_FIFO_STATUS_L, &u16RegVal);
        u32RegVal = u16RegVal;
        HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_VQ_FIFO_STATUS_H, &u16RegVal);
        u32RegVal |= (u16RegVal&1);
        if(u32RegVal)
        {
            return FALSE;
        }
    }

    HAL_GOP_Read16Reg(pGOPHalLocal, GOP_GE_STATUS, &u16RegVal);
    if(u16RegVal & GOP_GE_BUSY) //GE still busy
    {
        return FALSE;
    }

    return TRUE;
}
