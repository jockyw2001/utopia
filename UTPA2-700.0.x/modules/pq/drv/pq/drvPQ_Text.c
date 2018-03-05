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

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif
#include "MsCommon.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"

#include "QualityMap_Text.c"
#include "drvPQ_Bin.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"


MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
MS_BOOL gbEnablePQTextBin = 0;

char* MDrv_PQ_GetSrcTypeName_U2(void *pInstance,PQ_WIN eWindow)
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        MS_U16 u16SrcIDx = MDrv_PQ_GetSrcType_U2(pInstance, eWindow);

        u32Addr = MDrv_PQTextBin_GetSrcType(pInstance,u16SrcIDx, &stPQTextBinHeaderInfo[eWindow]);
        return ((char *)u32Addr);
    }
    else
    {
        return PQ_INPUTTYPE_TEXT_Main[MDrv_PQ_GetSrcType_U2(pInstance, eWindow)];
    }
}
char* MDrv_PQ_GetSrcTypeName(PQ_WIN eWindow)
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetSrcTypeName PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_SRC_TYPE_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

char* MDrv_PQ_GetIPName_U2(void *pInstance,MS_U8 u8PQIPIdx)
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        u32Addr = MDrv_PQTextBin_GetIPName(pInstance,(MS_U16)u8PQIPIdx,  &stPQTextBinHeaderInfo[PQ_MAIN_WINDOW]);
        return ((char *)u32Addr);
    }
    else
    {
        return PQ_IP_Text_Main[u8PQIPIdx];
    }
}
char* MDrv_PQ_GetIPName(MS_U8 u8PQIPIdx)
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetIPName PQArgs;
    PQArgs.u8PQIPIdx = u8PQIPIdx;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_IP_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

char* MDrv_PQ_GetTableName_U2(void *pInstance,MS_U8 u8PQIPIdx, MS_U8 u8TabIdx )
{
    if(gbEnablePQTextBin)
    {
#if defined (__aarch64__)
        MS_U64 u32Addr;
#else
        MS_U32 u32Addr;
#endif
        u32Addr = MDrv_PQTextBin_GetTableName(pInstance,(MS_U16)u8PQIPIdx,
                                              (MS_U16)u8TabIdx,
                                              &stPQTextBinHeaderInfo[PQ_MAIN_WINDOW]);
        return ((char *)u32Addr);

    }
    else
    {
        char** PQ_Stream;

        PQ_Stream = PQ_IPTAB_TEXT_Main[u8PQIPIdx];
        return PQ_Stream[u8TabIdx];
    }
}
char* MDrv_PQ_GetTableName(MS_U8 u8PQIPIdx, MS_U8 u8TabIdx )
{
#ifdef TRACE_DBG
		printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetTableName PQArgs;
    PQArgs.u8PQIPIdx = u8PQIPIdx;
    PQArgs.u8TabIdx = u8TabIdx;
    PQArgs.u8Ret = NULL;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_TABLE_NAME, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return NULL;
    }
    else
    {
        return PQArgs.u8Ret;
    }
}

#if 0
void MDrv_PQ_Text_Demo(SCALER_WIN eWindow)
{
    MS_U8 u8IPIdx, u8TabIdx, u8IPNum, u8TabNum;
    U16 u16PQSrcType;

    u16PQSrcType = MDrv_PQ_GetSrcType(eWindow);
    printf("u16PQSrcType=%u\n", u16PQSrcType);

    u8IPNum = MDrv_PQ_GetIPNum(eWindow);

    for( u8IPIdx = 0; u8IPIdx < u8IPNum; u8IPIdx++)
    {
        u8TabNum = MDrv_PQ_GetTableNum(eWindow, u8IPIdx);
        printf("IP:%u [%s], tabnum=%u\n", u8IPIdx, MDrv_PQ_GetIPName(u8IPIdx), u8TabNum);

        u8TabIdx = MDrv_PQ_GetCurrentTableIndex(eWindow, u8IPIdx);
        printf("TAB:%u [%s]\n", u8TabIdx,
        (u8TabIdx == PQ_IP_NULL) ? "null" : MDrv_PQ_GetTableName(u8IPIdx, u8TabIdx));
    }
}
#endif
