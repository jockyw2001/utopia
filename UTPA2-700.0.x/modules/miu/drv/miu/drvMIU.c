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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMIU.c
/// @brief  MIU Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsTypes.h"
#include "drvMIU.h"
#include "drvMIU_private.h"
#include "halMIU.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "utopia.h"
//#include <assert.h>
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
// Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_MIU "MIU"


//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_miu_version = {
    .DDI = { MIU_DRV_VERSION },
};

MS_U8 MIU0_request_mask_count[MIU_CLIENT_MAX];
MS_U8 MIU1_request_mask_count[MIU_CLIENT_MAX];

void* pInstantMIU = NULL;
void* pAttributeMIU = NULL;
static MS_BOOL bMiuInit = FALSE;

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_MIU_Exit(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _MDrv_MIU_Init
/// @brief \b Function  \b Description: set MIU I/O Map base address and parse occupied resource to software structure
/// @param None         \b IN :
/// @param None         \b OUT :
/// @param MS_BOOL      \b RET
/// @param None         \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_Init(void)
{
    MS_VIRT virtMMIOBaseAdr = 0;
    MS_PHY u32NonPMBankSize = 0;

    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_MIU))
    {
        MS_DEBUG_MSG(ULOGE(TAG_MIU, "Get IOMap failure\n"));
        MS_ASSERT(0);
        return FALSE;
    }

    HAL_MIU_SetIOMapBase(virtMMIOBaseAdr);

    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_PM))
    {
        MS_DEBUG_MSG(ULOGE(TAG_MIU, "Get IOMap failure\n"));
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_MIU_SetPMIOMapBase(virtMMIOBaseAdr);
    HAL_MIU_ParseOccupiedResource();
    bMiuInit = TRUE;    
    
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_SetIOMapBase
/// @brief \b Function  \b Description: set MIU I/O Map base address
/// @param None         \b IN :
/// @param None         \b OUT :
/// @param MS_BOOL      \b RET
/// @param None         \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_SetIOMapBase(void)
{
    MS_VIRT virtMMIOBaseAdr = 0;
    MS_PHY u32NonPMBankSize = 0;

    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_MIU))
    {
        MS_DEBUG_MSG(ULOGE(TAG_MIU, "Get IOMap failure\n"));
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_MIU_SetIOMapBase(virtMMIOBaseAdr);


    if( !MDrv_MMIO_GetBASE(&virtMMIOBaseAdr, &u32NonPMBankSize, MS_MODULE_PM))
    {
        MS_DEBUG_MSG(ULOGE(TAG_MIU, "Get IOMap failure\n"));
        MS_ASSERT(0);
        return FALSE;
    }
    HAL_MIU_SetPMIOMapBase(virtMMIOBaseAdr);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetDefaultClientID_KernelProtect()
/// @brief \b Function \b Description:  Get default client id array pointer for protect kernel
/// @param <RET>           \b     : The pointer of Array of client IDs
////////////////////////////////////////////////////////////////////////////////
MS_U8* MDrv_MIU_GetDefaultClientID_KernelProtect(void)
{
    return HAL_MIU_GetDefaultClientID_KernelProtect();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function    \b Name: MDrv_MIU_ProtectAlign()
/// @brief \b Function    \b Description:  Get the page shift for MIU protect
/// @param <*u32PageShift>\b IN: Page shift
/// @param <RET>          \b OUT: None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift)
{
    *u32PageShift = HAL_MIU_ProtectAlign();

    if(*u32PageShift > 0)
        return TRUE;
    else
        return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Dram_Size()
/// @brief \b Function \b Description:  Write the DRAM size reg for MIU protect
/// @param MiuID        \b IN     : MIU ID
/// @param DramSize     \b IN     : Specified Dram size for MIU protect
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_Dram_Size(MS_U8 MiuID, MS_U8 DramSize)
{
    return HAL_MIU_Dram_Size(MiuID, DramSize);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_Dram_ReadSize()
/// @brief \b Function \b Description:  Read the DRAM size
/// @param MiuID        \b IN     : MIU ID
/// @param DramSize     \b IN     :  Dram size for MIU protect
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Dram_ReadSize(MS_U8 MiuID, MIU_DDR_SIZE *pDramSize)
{
    return HAL_MIU_Dram_ReadSize(MiuID, pDramSize);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetClinetNumber()
/// @brief \b Function \b Description:  Get the number of clients for specific MIU block 
/// @param DramSize     \b IN     : MIU Block to protect (0 ~ 3)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_MIU_ClinetNumber(MS_U8 u8Blockx)
{
    return HAL_MIU_ClinetNumber(u8Blockx);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Protect()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 3)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param phy64Start      \b IN     : Starting address
/// @param phy64End        \b IN     : End address
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_Protect(
                          MS_U8    u8Blockx,
                          MS_U8    *pu8ProtectId,
                          MS_PHY phy64Start,
                          MS_PHY phy64End,
                          MS_BOOL  bSetFlag
                         )
{
    MS_BOOL Result;

    Result = HAL_MIU_Protect(u8Blockx, pu8ProtectId, phy64Start, phy64End, bSetFlag);

    if(Result == TRUE)
    {
        return TRUE;
    }
    else
    {
       MS_ASSERT(0);
       return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_ProtectEx()
/// @brief \b Function \b Description:  Enable/Disable MIU Protection mode
/// @param u8Blockx        \b IN     : MIU Block to protect (0 ~ 3)
/// @param *pu8ProtectId   \b IN     : Allow specified client IDs to write
/// @param phy64Start      \b IN     : Starting address
/// @param phy64End        \b IN     : End address
/// @param bSetFlag        \b IN     : Disable or Enable MIU protection
///                                      - -Disable(0)
///                                      - -Enable(1)
/// @param <OUT>           \b None    :
/// @param <RET>           \b None    :
/// @param <GLOBAL>        \b None    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_ProtectEx(
                          MS_U8    u8Blockx,
                          MS_U32    *pu32ProtectId,
                          MS_PHY phy64Start,
                          MS_PHY phy64End,
                          MS_BOOL  bSetFlag
                         )
{
    MS_BOOL Result;

    Result = HAL_MIU_ProtectEx(u8Blockx, pu32ProtectId, phy64Start, phy64End, bSetFlag);

    if(Result == TRUE)
    {
        return TRUE;
    }
    else
    {
       MS_ASSERT(0);
       return FALSE;
    }
}



////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_SetSsc()
/// @brief \b Function \b Description: _MDrv_MIU_SetSsc, @Step & Span
/// @param u16Fmodulation   \b IN : 20KHz ~ 40KHz (Input Value = 20 ~ 40)
/// @param u16FDeviation    \b IN  : under 0.1% ~ 3% (Input Value = 1 ~ 30)
/// @param bEnable          \b IN    :
/// @param None             \b OUT  :
/// @param None             \b RET  :
/// @param None             \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_SetSsc(MS_U16 u16Fmodulation,
                        MS_U16 u16FDeviation,
                        MS_BOOL bEnable)
{
    return HAL_MIU_SetSsc(0, u16Fmodulation, u16FDeviation, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_SetSscValue()
/// @brief \b Function \b Description: _MDrv_MIU_SetSscValue, @Step & Span
/// @param u8Miu            \b IN   : 0: MIU0 1:MIU1
/// @param u16Fmodulation   \b IN   : 20KHz ~ 40KHz (Input Value = 20 ~ 40)
/// @param u16FDeviation    \b IN   : under 0.1% ~ 3% (Input Value = 1 ~ 30)
/// @param bEnable          \b IN   :
/// @param None             \b OUT  :
/// @param None             \b RET  :
/// @param None             \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_SetSscValue(MS_U8 u8MiuDev,
                             MS_U16 u16Fmodulation,
                             MS_U16 u16FDeviation,
                             MS_BOOL bEnable)
{
    return HAL_MIU_SetSsc(u8MiuDev, u16Fmodulation, u16FDeviation, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_MaskReq()
/// @brief \b Function \b Description: Mask MIU request
/// @param u8Miu        \b IN   : 0: MIU0 1:MIU1
/// @param eClientID    \b IN   : Client ID
/// @param None         \b OUT  :
/// @param None         \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    HAL_MIU_MaskReq(u8Miu, eClientID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_UnMaskReq()
/// @brief \b Function \b Description: Unmak MIU request
/// @param u8Miu        \b IN   : 0: MIU0 1:MIU1
/// @param eClientID    \b IN   : Client ID
/// @param None         \b OUT  :
/// @param None         \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    HAL_MIU_UnMaskReq(u8Miu, eClientID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_OPM_R()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_OPM_R(u8Mask, u8Miu);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_DNRB_R()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_DNRB_R( u8Mask,  u8Miu);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_DNRB_W()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_DNRB_W( u8Mask,  u8Miu);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_DNRB_RW()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
   HAL_MIU_Mask_Req_DNRB_RW( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_SC_RW()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_SC_RW( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_MVOP_R()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_MVOP_R( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_MVD_R()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_MVD_R( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_MVD_W()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_MVD_W( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_MVD_RW()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_MVD_RW( u8Mask,  u8Miu);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_Mask_Req_AUDIO_RW()
/// @brief \b Function \b Description:
/// @param None  \b IN   :
/// @param None  \b OUT  :
/// @param None  \b RET  :
///////////////////////////////////////////////////////////////////////////////
void _MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    HAL_MIU_Mask_Req_AUDIO_RW( u8Mask,  u8Miu);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_InitCounter()
/// @brief \b Function \b Description:  This function clear all request mask counter of
///                                     MIU driver
/// @param None \b IN:
/// @param None \b RET:
////////////////////////////////////////////////////////////////////////////////
void MDrv_MIU_InitCounter(void)
{
    MS_U8 i;

    for(i=0;i<MIU_CLIENT_MAX;i++)
    {
        MIU0_request_mask_count[i]=0;
        MIU1_request_mask_count[i]=0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_SwitchMIU()
/// @brief \b Function \b Description:  This function for switch miu
///
/// @param u8MiuID        \b IN     : select MIU0 or MIU1
/// @param None \b RET:
////////////////////////////////////////////////////////////////////////////////
void MDrv_MIU_SwitchMIU(MS_U8 u8MiuID)
{
      HAL_MIU_VOP_SwitchMIU(u8MiuID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: _MDrv_MIU_SwitchMIU()
/// @brief \b Function \b Description:  This function for switch miu
///
/// @param u8MiuID        \b IN     : select MIU0 or MIU1
/// @param None \b RET:
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType)
{
    MS_BOOL ret = TRUE;

    _MDrv_MIU_MaskReq(0, eClientID);
    _MDrv_MIU_MaskReq(1, eClientID);
    ret &= HAL_MIU_SelMIU(eType, eClientID);
    if (MIU_SELTYPE_MIU_ALL == eType)
    {
        _MDrv_MIU_UnMaskReq(0, eClientID);
        _MDrv_MIU_UnMaskReq(1, eClientID);
    }
    else
        _MDrv_MIU_UnMaskReq((MS_U8)eType, eClientID);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_IsSupportMIU1()
/// @brief \b Function \b Description:  This function for checking if support miu1
///
/// @param None \b RET:   TRUE: Support miu1 FALSE: no miu1
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_IsSupportMIU1(void)
{
      return HAL_MIU_IsInitMiu1();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MIU_GetBusWidth()
/// @brief \b Function \b Description:  This function for querying data bus width
///
/// @param None \b RET:   0: not support or 64 or 128 bits
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_MIU_GetBusWidth(void)
{
    return HAL_MIU_IsI64Mode();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_GetClientInfo()
/// @brief \b Function  \b Description:  This function for querying client ID info
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : Client ID
/// @param pInfo        \b OUT  : Client Info
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_GetClientInfo(MS_U8 u8MiuDev, eMIUClientID eClientID, MIU_ClientInfo *pInfo)
{
    MS_S16 sVal;
    if (!pInfo)
        return FALSE;

    sVal = HAL_MIU_GetClientInfo(u8MiuDev, eClientID);
    if (sVal < 0)
        return FALSE;
    pInfo->u8BitPos = (MS_U8)MIU_GET_CLIENT_POS(sVal);
    pInfo->u8Gp = (MS_U8)MIU_GET_CLIENT_GROUP(sVal);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _MDrv_MIU_GetProtectInfo()
/// @brief \b Function  \b Description:  This function for querying client ID info
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : Client ID
/// @param pInfo        \b OUT  : Client Info
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo)
{
    MS_U32 ret;
    ret = HAL_MIU_GetProtectInfo(u8MiuDev, pInfo);
   
    //if(pInfo->bHit)
    //{
    //    if(MsOS_GetKattribute("miuprot")==1)
    //        MS_ASSERT(0);
    //}

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _MDrv_MIU_SetGroupPriority()
/// @brief \b Function  \b Description:  This function for querying client ID info
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : Client ID
/// @param pInfo        \b OUT  : Client Info
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority)
{
    return HAL_MIU_SetGroupPriority(u8MiuDev, sPriority);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _MDrv_MIU_SetHPriorityMask()
/// @brief \b Function  \b Description:  Set miu high priority mask
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : Client ID
/// @param pInfo        \b OUT  : Client Info
/// @param None \b RET:   0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask)
{
    return HAL_MIU_SetHPriorityMask(u8MiuDev, eClientID, bMask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_GetLibVer
/// @brief \b Function  \b Description: Show the MIU driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b 0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_miu_version;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_EnableScramble
/// @brief \b Function  \b Description: Enable MIU Scramble
/// @param bEnable      \b In: Enable MIU Scramble
/// @return             \b 0: Fail 1: Ok
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_EnableScramble(MS_BOOL bEnable)
{
    return HAL_MIU_EnableScramble(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_IsScrambleEnabled
/// @brief \b Function  \b Description: Check if the MIU Scramble enabled
/// @return             \b 0: Disabled 1: Enabled
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_IsScrambleEnabled(void)
{
    return HAL_MIU_IsScrambleEnabled();
}

#ifdef _MIU_INTERNEL_USE 
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_GetLoading
/// @brief \b Function  \b Description: Get the percentage of MIU loading
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param *Loading     \b IN   : percentage of MIU loading
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_GetLoading(MS_U8 u8MiuDev, MS_U32 *u32Loading)
{
    HAL_MIU_SetLoadingRequest(u8MiuDev);

    MsOS_DelayTask(1000);

    HAL_MIU_GetLoadingRequest(u8MiuDev, u32Loading);

    return TRUE;
}
#endif


void _MDrv_MIU_PrintProtectInfo(void)
{
    HAL_MIU_PrintMIUProtectInfo();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_Exit
/// @brief \b Function  \b Description: Exit MIU driver
/// @return             \b 0: Fail 1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_Exit(void)
{    
    if (bMiuInit == FALSE)
    {
        ULOGE(TAG_MIU, "MIU is not be initialized\n");
        return TRUE;
    }
    
    if(UtopiaClose(pInstantMIU) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_MIU, "close MIU fail\n");
        pInstantMIU = NULL;
        return FALSE;
    }
    
    pInstantMIU = NULL;
    bMiuInit = FALSE;
    
    return TRUE;
}

void _MDrv_MIU_MaskReqUtopia20IOCtrl(eMIUIoctlCmd eMIUIOCmd, MS_U8 u8Mask, MS_U8 u8Miu)
{
    PMIU_MIU_MASK pMIUMask;
    if (NULL != pInstantMIU)
    {
        pMIUMask = (PMIU_MIU_MASK)malloc(sizeof(MIU_MIU_MASK));
        if (pMIUMask == NULL)
            return;
        pMIUMask->u8Mask = u8Mask;
        pMIUMask->u8Miu = u8Miu;
        if(UtopiaIoctl(pInstantMIU,eMIUIOCmd,(void*)pMIUMask) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_Mask_Req fail in IP %d\n",eMIUIOCmd);
            free(pMIUMask);
    		return;
        }
        free(pMIUMask);
    }
}

void MDrv_MIU_Mask_Req_OPM_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    // utopia20 flow code
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_OPM_R, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_DNRB_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_DNRB_R, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_DNRB_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_DNRB_W, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_DNRB_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_DNRB_RW, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_SC_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_SC_RW, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_MVOP_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_MVOP_R, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_MVD_R(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_MVD_R, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_MVD_W(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_MVD_W, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_MVD_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_MVD_RW, u8Mask, u8Miu);
}

void MDrv_MIU_Mask_Req_AUDIO_RW(MS_U8 u8Mask, MS_U8 u8Miu)
{
    _MDrv_MIU_MaskReqUtopia20IOCtrl(MDrv_CMD_MIU_Mask_Req_AUDIO_RW, u8Mask, u8Miu);
}

void  MDrv_MIU_MaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    PMIU_MIUREQ_MASK pMIUMaskReq;

    if (NULL != pInstantMIU)
    {
        pMIUMaskReq = (PMIU_MIUREQ_MASK)malloc(sizeof(MIU_MIUREQ_MASK));
        if (pMIUMaskReq == NULL)
            return;
        pMIUMaskReq->u8Miu = u8Miu;
        pMIUMaskReq->eClientID = eClientID;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_Mask_Req,(void*)pMIUMaskReq) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_MIU_MaskReq fail\n");
            free(pMIUMaskReq);
    		return;
        }
        free(pMIUMaskReq);
    }
}

void MDrv_MIU_UnMaskReq(MS_U8 u8Miu, eMIUClientID eClientID)
{
    PMIU_MIUREQ_MASK pMIUMaskReq;

    if (NULL != pInstantMIU)
    {
        pMIUMaskReq = (PMIU_MIUREQ_MASK)malloc(sizeof(MIU_MIUREQ_MASK));
        if (pMIUMaskReq == NULL)
            return;
        pMIUMaskReq->u8Miu = u8Miu;
        pMIUMaskReq->eClientID = eClientID;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_UnMask_Req,(void*)pMIUMaskReq) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_UnMask_Req fail\n");
            free(pMIUMaskReq);
    		return;
        }
        free(pMIUMaskReq);
    }
}

MS_BOOL MDrv_MIU_SetSsc(MS_U16 u16Fmodulation, MS_U16 u16FDeviation,MS_BOOL bEnable)
{
    PMIU_SET_SSC pMIUSetSsc;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUSetSsc = (PMIU_SET_SSC)malloc(sizeof(MIU_SET_SSC));
        if (pMIUSetSsc == NULL)
            return FALSE;
        pMIUSetSsc->u16Fmodulation = u16Fmodulation;
        pMIUSetSsc->u16FDeviation = u16FDeviation;
        pMIUSetSsc->bEnable = bEnable;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_SetSsc,(void*)pMIUSetSsc) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_SetSsc fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
        free(pMIUSetSsc);
    }
    else
    {
        bRet = FALSE;
    }
    
    return bRet;
}

MS_BOOL MDrv_MIU_SetSscValue(MS_U8 u8MiuDev, MS_U16 u16Fmodulation, MS_U16 u16FDeviation, MS_BOOL bEnable)
{
    PMIU_SET_SSCVALUE pMIUSetSscValue;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUSetSscValue = (PMIU_SET_SSCVALUE)malloc(sizeof(MIU_SET_SSCVALUE));
        if (pMIUSetSscValue == NULL)
            return FALSE;
        pMIUSetSscValue->u8MiuDev = u8MiuDev;
        pMIUSetSscValue->u16Fmodulation = u16Fmodulation;
        pMIUSetSscValue->u16FDeviation = u16FDeviation;
        pMIUSetSscValue->bEnable = bEnable;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_SetSscValue,(void*)pMIUSetSscValue) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_SetSscValue fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
        free(pMIUSetSscValue);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MDrv_MIU_Protect( MS_U8 u8Blockx, MS_U8 *pu8ProtectId, MS_PHY phy64Start, MS_PHY phy64End, MS_BOOL bSetFlag )
{
    PMIU_PROTECT pMIUProtect;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUProtect = (PMIU_PROTECT)malloc(sizeof(MIU_PROTECT));
        if (pMIUProtect == NULL)
            return FALSE;
        pMIUProtect->u8Blockx = u8Blockx;
        pMIUProtect->pu8ProtectId = pu8ProtectId;
        pMIUProtect->phy64Start = phy64Start;
        pMIUProtect->phy64End = phy64End;
        pMIUProtect->bSetFlag = bSetFlag;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_Protect,(void*)pMIUProtect) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_Protect fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
        free(pMIUProtect);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MDrv_MIU_ProtectEx( MS_U8 u8Blockx, MS_U32 *pu32ProtectId, MS_PHY phy64Start, MS_PHY phy64End, MS_BOOL bSetFlag )
{
    PMIU_PROTECTEx pMIUProtect;

    if (NULL != pInstantMIU)
    {
        pMIUProtect = (PMIU_PROTECTEx)malloc(sizeof(MIU_PROTECTEx));
        if (pMIUProtect == NULL)
            return FALSE;
        pMIUProtect->u8Blockx = u8Blockx;
        pMIUProtect->pu32ProtectId = pu32ProtectId;
        pMIUProtect->phy64Start = phy64Start;
        pMIUProtect->phy64End = phy64End;
        pMIUProtect->bSetFlag = bSetFlag;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_ProtectEx,(void*)pMIUProtect) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Ioctl MDrv_CMD_MIU_Protect fail\n");
            free(pMIUProtect);
            return FALSE;
        }
        free(pMIUProtect);
    }
    return TRUE;
}

MS_BOOL MDrv_MIU_SelMIU(eMIUClientID eClientID, eMIU_SelType eType)
{
    PMIU_SELMIU pMIUSelMIU;
    MS_BOOL bRet = TRUE;
    if (NULL != pInstantMIU)
    {
        pMIUSelMIU = (PMIU_SELMIU)malloc(sizeof(MIU_SELMIU));
        if (pMIUSelMIU == NULL)
            return FALSE;
        pMIUSelMIU->eClientID = eClientID;
        pMIUSelMIU->eType = eType;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_SelMIU,(void*)pMIUSelMIU) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_SelMIU fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }

        free(pMIUSelMIU);
    }
    else
    {
        bRet = FALSE;
    }
    
    return bRet;
}

MS_BOOL MDrv_MIU_GetProtectInfo(MS_U8 u8MiuDev, MIU_PortectInfo *pInfo)
{
    PMIU_GETPROTECTINFO pMIUGetProtectInfo;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUGetProtectInfo = (PMIU_GETPROTECTINFO)malloc(sizeof(MIU_GETPROTECTINFO));
        if (pMIUGetProtectInfo == NULL)
            return FALSE;
        pMIUGetProtectInfo->u8MiuDev = u8MiuDev;
        pMIUGetProtectInfo->pInfo = pInfo;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_GetProtectInfo,(void*)pMIUGetProtectInfo) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_GetProtectInfo fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
        free(pMIUGetProtectInfo);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MDrv_MIU_SetGroupPriority(MS_U8 u8MiuDev, MIU_GroupPriority sPriority)
{
    PMIU_SETGROUPPRIORITY pMIUSetGroupPri;

    if (NULL != pInstantMIU)
    {
        pMIUSetGroupPri = (PMIU_SETGROUPPRIORITY)malloc(sizeof(MIU_SETGROUPPRIORITY));
        if (pMIUSetGroupPri == NULL)
            return FALSE;
        pMIUSetGroupPri->u8MiuDev = u8MiuDev;
        pMIUSetGroupPri->sPriority = sPriority;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_SetGroupPriority,(void*)pMIUSetGroupPri) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_SetGroupPriority fail\n");
            free(pMIUSetGroupPri);
    		return FALSE;
        }
        free(pMIUSetGroupPri);
    }
    return TRUE;
}


MS_BOOL MDrv_MIU_SetHPriorityMask(MS_U8 u8MiuDev, eMIUClientID eClientID, MS_BOOL bMask)
{
    PMIU_SETHIGHPRIORITY pMIUSetHighPri;

    if (NULL != pInstantMIU)
    {
        pMIUSetHighPri = (PMIU_SETHIGHPRIORITY)malloc(sizeof(MIU_SETHIGHPRIORITY));
        if (pMIUSetHighPri == NULL)
            return FALSE;
        pMIUSetHighPri->u8MiuDev = u8MiuDev;
        pMIUSetHighPri->eClientID = eClientID;
        pMIUSetHighPri->bMask = bMask;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_SetHighPriority,(void*)pMIUSetHighPri) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_MIU_SetHPriorityMask fail\n");
            free(pMIUSetHighPri);
    		return FALSE;
        }
        free(pMIUSetHighPri);
    }
    return TRUE;
}


    
MS_BOOL MDrv_MIU_Init(void)
{    
    PMIU_RET pMIURet;
    MS_BOOL bRet = TRUE;
    if ( bMiuInit == FALSE) 
    {
        //if(UtopiaOpen(MODULE_MIU | KERNEL_MODE, &pInstantMIU, 0, pAttributeMIU) != UTOPIA_STATUS_SUCCESS)
        if(UtopiaOpen(MODULE_MIU, &pInstantMIU, 0, pAttributeMIU) != UTOPIA_STATUS_SUCCESS)
        {
	        ULOGE(TAG_MIU, "MIU had been initialized\n");
	        return FALSE;
        }
    }
    if (NULL != pInstantMIU)
    {
        pMIURet = (PMIU_RET)malloc(sizeof(MIU_RET));
        
        if(pMIURet == NULL)
            return FALSE;

        if(UtopiaIoctl(pInstantMIU, MDrv_CMD_MIU_Init,(void*)pMIURet) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_MIU_SetHPriorityMask fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
        
        free(pMIURet);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
	
    //return _MDrv_MIU_Init();
}

void  MDrv_MIU_PrintMIUProtectInfo(void)
{
    return;
    if (NULL != pInstantMIU)
    {
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_PrintProtectInfo,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
        {
			ULOGE(TAG_MIU, "Ioctl MDrv_MIU_SetHPriorityMask fail\n");
    	    return;
        }
    }
    return;
}
    


MS_BOOL MDrv_MIU_Dram_Size(MS_U8 u8MiuDev, MS_U8 DramSize)
{
    PMIU_DRAM_SIZE pMIUDramSize = NULL;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUDramSize = (PMIU_DRAM_SIZE)malloc(sizeof(MIU_DRAM_SIZE));
        if (pMIUDramSize == NULL)
            return FALSE;
        pMIUDramSize->u8MiuDev = u8MiuDev;
        pMIUDramSize->DramSize = DramSize;
        pMIUDramSize->bRet = FALSE;
        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_Dram_Size,(void*)pMIUDramSize) != UTOPIA_STATUS_SUCCESS)
        {
			ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_Dram_Size fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = pMIUDramSize->bRet;
        }

        free(pMIUDramSize);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MDrv_MIU_ProtectAlign(MS_U32 *u32PageShift)
{
    PMIU_PROTECT_ALIGN pMIUProtectAlign = NULL;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        pMIUProtectAlign = (PMIU_PROTECT_ALIGN)malloc(sizeof(MIU_PROTECT_ALIGN));
        if(pMIUProtectAlign == NULL)
            return FALSE;
        pMIUProtectAlign->u32PageShift = u32PageShift;
        pMIUProtectAlign->bRet = FALSE;
        if(UtopiaIoctl(pInstantMIU, MDrv_CMD_MIU_ProtectAlign,(void*)pMIUProtectAlign) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Ioctl MDrv_CMD_MIU_ProtectAlign fail\n");
            bRet = FALSE;
        }
        else
        {
            bRet = pMIUProtectAlign->bRet;
        }

        free(pMIUProtectAlign);
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_U32 MDrv_MIU_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE u16PreMIUPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        MDrv_MIU_Exit();
        u16PreMIUPowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreMIUPowerState == E_POWER_SUSPEND)
        {
            MDrv_MIU_Init();

            u16PreMIUPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_MIU, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_MIU, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

void _MDrv_MIU_PrintMIUProtectInfo(void)
{
    HAL_MIU_PrintMIUProtectInfo();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MIU_GetClientWidth
/// @brief \b Function  \b Description: Get MIU client bus width
/// @param u8MiuDev     \b IN   : select MIU0 or MIU1
/// @param eClientID    \b IN   : client ID
/// @param pClientWidth \b IN   : client bus width
/// @return             \b 0 : Fail  1: OK
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MIU_GetClientWidth(MS_U8 u8MiuDevi, eMIUClientID eClientID, eMIU_ClientWidth *pClientWidth)
{
    return HAL_MIU_GetClientWidth(u8MiuDevi, eClientID, pClientWidth);
}


MS_BOOL MDrv_MIU_GetDramType(MIU_ID eMiu, MIU_DDR_TYPE* pType)
{
    MIU_GETDRAMTYPE args;
    MS_BOOL bRet = TRUE;

    if (NULL != pInstantMIU)
    {
        args.eMiu = eMiu;
        args.pType = pType;

        if(UtopiaIoctl(pInstantMIU,MDrv_CMD_MIU_GetDramType,(void*)&args) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_MIU, "Ioctl MDrv_CMD_MIU_GetDramType fail\n");
            bRet = FALSE;
        }
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

MS_BOOL _MDrv_MIU_GetDramType(MIU_ID eMiu, MIU_DDR_TYPE* pType)
{
    return HAL_MIU_GetDramType(eMiu, pType);
}
