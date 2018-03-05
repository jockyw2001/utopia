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
/// file    drvNDS.c
/// @brief  NDS Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"               // MStar common .h
#include "drvMMIO.h"

#include "regNDS.h"                 // MStar NDS  HAL
#include "drvNDS.h"
#include "halNDS.h"

#include "nds.h"                    // internal .h

#include "ddiNDS_HDI.h"             // NDS CAHDI driver .h


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


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
//  Global Functions
//-------------------------------------------------------------------------------------------------
void NDS_DBG_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen)
{
    int                 i;
    //printf("(%d)>>", u16DataLen);
    for (i = 0; i < u16DataLen; i++)
    {
        if ((i % 16) == 0)
        {
            NDS_DBG("\n    ");
        }
        NDS_DBG(" %02X ", pu8Data[i]);
    }
    NDS_DBG("\n");
}


NDS_Result MDrv_NDS_Init(NDS_Param *param)
{
    MS_U32              u32BaseAddr, u32BaseSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_NDS))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_RegMap(u32BaseAddr);
    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_NSK))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_NSK_RegMap(u32BaseAddr);
    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_HW_RegMap(u32BaseAddr);

    NDS_CAM_Init();
    NDS_FLT_Init(&(param->flt));    // ECM / EMM
    NDS_SC_Init(&(param->sc));      // SC

    #if 0
    NDS_RASP_Init(&(param->rasp));
    #endif


    NDS_SC_Open();

    //[NOTE] add for J2 to open NSK clk
    HAL_NDS_PowCtrl(0, TRUE);

    // Set Chip Magic Value //
    HAL_NDS_InitMagic();


    return E_NDS_OK;
}


NDS_Result MDrv_NDS_Exit(void)
{
    NDS_SC_Close();
    NDS_SC_Exit();
    NDS_FLT_Exit();
    NDS_CAM_Exit();

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_PowerOff(void)
{
    MS_U32              u32BaseAddr, u32BaseSize;
    int                 i;

    NDS_ERR("%s : Force Power OFF [BEGIN]\n", __FUNCTION__);
    NDS_ERR("%s : All MDrv_NDS_ is not accessible now\n", __FUNCTION__);

    if (FALSE == MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        NDS_ERR("MDrv_MMIO_GetBASE fail\n");
        return E_NDS_FAIL;
    }
    NDS_HW_RegMap(u32BaseAddr);

    for (i = 0; i < NSK_DEV_NUM; i++)
    {
        HAL_NDS_PowCtrl(i, TRUE);
    }

    NDS_ERR("%s : Force Power OFF [COMPLETED]\n", __FUNCTION__);

    return E_NDS_OK;
}


MS_U32 MDrv_NDS_GetCaps(NDS_Caps cap)
{
//    return HAL_NDS_GetCaps(cap);
    switch (cap)
    {
    case E_NDS_CAP_ECM_NUM :
        return NDS_CAM_ECM_FLT_NUM;

    case E_NDS_CAP_EMM_NUM :
        return NDS_CAM_EMM_BUF_NUM;

    case E_NDS_CAP_BUF_PITCH :
        return NDS_CAM_FLT_BUF_SIZE;

    case E_NDS_CAP_BUF_ALIGN :
        return NDS_CAM_FLT_BUF_ALIGN;

    case E_NDS_CAP_BUF_TOTALSIZE :
        return NDS_CAM_FLT_BUFS_SIZE;

    case E_NDS_CAP_RASP_BUF_TOTALSIZE :
        return NDS_RASP_ALL_BUF_SIZE;

    default:
        return 0;
    }
}


NDS_Result MDrv_NDS_SetMagicValue(MS_U8 idx, MS_U16 word_0, MS_U16 word_1, MS_U16 word_2, MS_U16 word_3)
{
    if (idx > 4)
    {
        return E_NDS_FAIL;
    }

    NDS_REG(REG_MAGIC_V(idx, 0)) = word_0;
    NDS_REG(REG_MAGIC_V(idx, 1)) = word_1;
    NDS_REG(REG_MAGIC_V(idx, 2)) = word_2;
    NDS_REG(REG_MAGIC_V(idx, 3)) = word_3;

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_NSK_Open(MS_U32 nds_id)
{
    HAL_NDS_PowCtrl(nds_id, TRUE);

    NDS_NSK_Init(nds_id);

    return E_NDS_OK;
}


NDS_Result MDrv_NDS_NSK_Close(MS_U32 nds_id)
{
    NDS_NSK_Exit(nds_id);

    HAL_NDS_PowCtrl(nds_id, FALSE);

    return E_NDS_OK;
}


