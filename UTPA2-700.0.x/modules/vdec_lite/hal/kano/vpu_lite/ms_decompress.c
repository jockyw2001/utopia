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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#else
#include <linux/string.h>
#endif

#include "MsCommon.h"
//#include "datatype.h"
#include "ms_decompress.h"
#include "ms_decompress_priv.h"

#define _ReadChar(A)\
{\
    A=(*pInStream << (8 - u32BitPos))|(*(pInStream+1)>>u32BitPos);\
    pInStream++;\
}

#define _ReadBit(A)\
{\
    u32BitPos--;\
    A = (*pInStream >> u32BitPos) & 0x01;\
    if(u32BitPos == 0)\
    {\
        u32BitPos = 8;\
        pInStream++;\
    }\
}
static MS_U8 *pSlidingWindow;
static MS_U32 _u32BitPos = 0;
static MS_U32 u32NextChar = 0;
static MS_U8 *pOutStream;
static MS_U32 _u32Offset = 0;

MS_BOOL ms_VDECDecompressInit(MS_U8 *pSliding, MS_U8 *pOut)
{
//    pSlidingWindow = malloc(WINDOW_SIZE);
    pSlidingWindow = pSliding;
    memset(pSlidingWindow, 0, WINDOW_SIZE);
    _u32BitPos = 8;
    u32NextChar = 1;
    pOutStream = pOut;
    _u32Offset = 0;
    return TRUE;
}

void ms_VDECDecompressDeInit(void)
{
//    free(pSlidingWindow);
}

int ms_VDECDecompress(const MS_U8 *pInStream, MS_U32 u32DataLen)
{
    register MS_U32 i ;
    register MS_U32 offset,length;
    register MS_U32 mask=0xFFF;
    register MS_U32 u32BitPos = 0;
    register const MS_U8 *pStreamEnd = NULL;
    register MS_U32 tmp0,tmp1;
    const MS_U8 *pOutHead = pOutStream;

    //pSlidingWindow = malloc(WINDOW_SIZE);
    if (pSlidingWindow)
    {

        u32BitPos = _u32BitPos;
        pStreamEnd = pInStream + u32DataLen;

        pInStream += _u32Offset;
        //pInStream = pInStream;
        //printf("I-pOutStream = %d \n",pOutStream);

        while (pInStream <= pStreamEnd)
        {
            _ReadBit(tmp0);
            if (tmp0 != UNCODED)
            {
                _ReadChar(tmp0);
                _ReadChar(tmp1);
                offset = (tmp0 << 8) | tmp1;
                length = (offset & 0x0F) + 2;
                offset >>= 4;
                if ((u32NextChar+length) < WINDOW_SIZE)
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i)] = *pOutStream++;
                    }
                }
                else
                {
                    for (i = 0; i < length; i++)
                    {
                        *pOutStream = pSlidingWindow[(offset+i) & mask];
                        pSlidingWindow[(u32NextChar+i) & mask] = *pOutStream++;
                    }
                }
                u32NextChar+=length;
            }
            else
            {
                _ReadChar(tmp0);
                *pOutStream = tmp0;
                pSlidingWindow[u32NextChar] = *pOutStream++;
                u32NextChar++;
            }
            u32NextChar = u32NextChar & mask;
        }

    }
    _u32BitPos = u32BitPos;
    _u32Offset = pInStream - pStreamEnd;
    return (pOutStream - pOutHead);
}

//*************************************************************************************************

