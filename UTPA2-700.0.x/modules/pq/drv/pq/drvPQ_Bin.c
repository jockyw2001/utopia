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
#define _DRV_PQ_BIN_C


#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif

#include "MsCommon.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "hwreg_utility2.h"
#include "color_reg.h"
#include "drvMMIO.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "drvPQ_Bin.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "UFO.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#ifndef PQ_BIN_SOURCELUT_DATA_LEN //to avoid compile warnings...
#define PQ_BIN_SOURCELUT_DATA_LEN           2 // 2 bytes
#endif

#define PQ_BIN_DBG_HEADER_DATA      0
#define PQ_BIN_DBG(x)               //(x)
#define PQ_BIN_DBG_HEADER(x)        //(x)
#define PQ_BIN_DUMP_DBG(x)          //(x)
#define PQ_BIN_DUMP_FILTER_DBG(x)   //(x)
#define PQ_BIN_XRULE_DBG(x)         //(x)
#define PQ_BIN_GRULE_DBG(x)         //(x)
#define PQ_TEXT_BIN_DBG_HEADER(x)   //(x)
#define PQ_TEXT_BIN_DBG(x)          //(x)
#define PQ_BIN_DBG_SRAMERROR(x)     //(x)
#define PQ_BIN_DUMP_CLONE_DBG(x)    //(x)

//-------------------------------------------------------------------------------------------------
//  Local Variable
//-------------------------------------------------------------------------------------------------
static MS_U16 _u16PQTabIdx[PQ_BIN_MAX_PNL][PQ_BIN_MAX_IP];    // store all TabIdx of all IPs

static MS_U8 _gPQBinDisplayType;
static MS_U8 _gPQBinPnlIdx;
static MS_BOOL _bMLoadEn = FALSE;

#define MLOAD_MAX_CMD   192
static MS_U32 _u32MLoadCmd[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadMsk[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadVal[MLOAD_MAX_CMD];
static MS_U16 _u16MLoadCmdCnt = 0;
#ifdef MSOS_TYPE_LINUX
static pthread_mutex_t _stPQFilterTableMutex = PTHREAD_MUTEX_INITIALIZER;
#endif

// to save loading SRAM table time, SRAM are only downloaded
// when current SRAM table is different to previous SRAM table
static MS_U16 _u16SRAM1Table = 0xFFFF;
static MS_U16 _u16SRAM2Table = 0xFFFF;
static MS_U16 _u16SRAM3Table = 0xFFFF;
static MS_U16 _u16SRAM4Table = 0xFFFF;
static MS_U16 _u16CSRAM1Table = 0xFFFF;
static MS_U16 _u16CSRAM2Table = 0xFFFF;
static MS_U16 _u16CSRAM3Table = 0xFFFF;
static MS_U16 _u16CSRAM4Table = 0xFFFF;
static MS_U16 _u16ICC_CRD_Table = 0xFFFF;
static MS_U16 _u16IHC_CRD_Table = 0xFFFF;
static MS_U16 _u16XVYCC_DE_GAMMA_SRAM_Table = 0xFFFF;
static MS_U16 _u16XVYCC_GAMMA_SRAM_Table = 0xFFFF;

static MS_U16 _u16SRAM1Table_main2 = 0xFFFF;
static MS_U16 _u16SRAM2Table_main2 = 0xFFFF;
static MS_U16 _u16SRAM3Table_main2 = 0xFFFF;
static MS_U16 _u16SRAM4Table_main2 = 0xFFFF;
static MS_U16 _u16CSRAM1Table_main2 = 0xFFFF;
static MS_U16 _u16CSRAM2Table_main2 = 0xFFFF;
static MS_U16 _u16CSRAM3Table_main2 = 0xFFFF;
static MS_U16 _u16CSRAM4Table_main2 = 0xFFFF;
static MS_U16 _u16ICC_CRD_Table_main2 = 0xFFFF;
static MS_U16 _u16IHC_CRD_Table_main2 = 0xFFFF;
static MS_U16 _u16XVYCC_DE_GAMMA_SRAM_Table_main2 = 0xFFFF;
static MS_U16 _u16XVYCC_GAMMA_SRAM_Table_main2 = 0xFFFF;

#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
static MS_U16 _u16UFSCSRAM1Table = 0xFFFF;
static MS_U16 _u16UFSCSRAM2Table = 0xFFFF;
static MS_U16 _u16UFSCSRAM3Table = 0xFFFF;
static MS_U16 _u16UFSCSRAM4Table = 0xFFFF;
static MS_U16 _u16UFSCCSRAM1Table = 0xFFFF;
static MS_U16 _u16UFSCCSRAM2Table = 0xFFFF;
static MS_U16 _u16UFSCCSRAM3Table = 0xFFFF;
static MS_U16 _u16UFSCCSRAM4Table = 0xFFFF;
static MS_U16 _u16UFSCICC_CRD_Table = 0xFFFF;
static MS_U16 _u16UFSCIHC_CRD_Table = 0xFFFF;
//static MS_U16 _u16UFSCXVYCC_DE_GAMMA_SRAM_Table = 0xFFFF;
//static MS_U16 _u16UFSCXVYCC_GAMMA_SRAM_Table = 0xFFFF;
#endif
//-------------------------------------------------------------------------------------------------
//  Function
//-------------------------------------------------------------------------------------------------

MS_U8 MDrv_PQBin_GetByteData(void *pInstance,void *pAddrVirt, MS_U32 *u32Offset)
{
    MS_U8 u8ret = *((MS_U8 *)pAddrVirt);
    *u32Offset = *u32Offset + 1;

    return u8ret;
}

MS_U16 MDrv_PQBin_Get2ByteData(void *pInstance,void *pAddrVirt, MS_U32 *u32Offset)
{
    MS_U16 u16ret = (((MS_U16)*((MS_U8 *)pAddrVirt+0)) << 0) |
                    (((MS_U16)*((MS_U8 *)pAddrVirt+1)) << 8);

    *u32Offset = *u32Offset + 2;


    return u16ret;
}


MS_U32 MDrv_PQBin_Get4ByteData(void *pInstance,void *pAddrVirt, MS_U32 *u32Offset)
{
    MS_U32 u32ret = (((MS_U32)*((MS_U8 *)pAddrVirt+0)) << 0)  |
                    (((MS_U32)*((MS_U8 *)pAddrVirt+1)) << 8)  |
                    (((MS_U32)*((MS_U8 *)pAddrVirt+2)) << 16) |
                    (((MS_U32)*((MS_U8 *)pAddrVirt+3)) << 24);

    *u32Offset = *u32Offset + 4;

    return u32ret;
}




void MDrv_PQBin_DumpGeneralRegTable(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabInfo)
{
    MS_U8  u8Bank;
    MS_U8  u8Addr;
    MS_U8  u8Mask;
    MS_U8  u8Value;
    MS_U16 i;
    MS_PHYADDR u32Addr, u32RegAddr;
    MS_U32 u32Offset;
    PQ_BIN_DUMP_DBG(printf("tab: general\n"));
    if (pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DUMP_DBG(printf("[PQBin]IP_Info error: General Reg Table\n"));
        return;
    }

    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i< pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8Bank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        PQ_BIN_DBG_SRAMERROR(printf("4  [read addr=%lx], GroupIdx=%x,RegNum=%x,reg=%04x, msk=%02x, val=%02x, u8CurBank=%x\n",
					(u32Addr+u32Offset-(MS_U32)pTabInfo->u16GroupIdx),pTabInfo->u16GroupIdx,pTabInfo->u16RegNum,NON_PM_BASE | (((MS_U16)u8Bank) << 8) | (MS_U16)u8Addr, u8Mask, u8Value, u8Bank));

        u32RegAddr = NON_PM_BASE | (((MS_U16)u8Bank) << 8) | (MS_U16)u8Addr;

        PQ_BIN_DUMP_DBG(printf("[addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value));

        PQ_REG_FUNC(u32RegAddr, u8Value, u8Mask);
    }
}

void MDrv_PQBin_DumpCombRegTable(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabInfo)
{
    MS_U16 i;
    MS_U8 u8Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    MS_PHYADDR u32Addr, u32RegAddr;
    MS_U32 u32Offset;
    COMB_BK_STORE;

    PQ_BIN_DUMP_DBG(printf("tab: comb\n"));

    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DUMP_DBG(printf("[PQBin]IP_Info error: Comb Reg Table\n"));
        return;
    }

    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i< pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        PQ_BIN_DBG_SRAMERROR(printf("2  [read addr=%lx], GroupIdx=%x,RegNum=%x,reg=%04lx, msk=%02x, val=%02x, u8CurBank=%x\n",
					(u32Addr+u32Offset-(MS_U32)pTabInfo->u16GroupIdx),pTabInfo->u16GroupIdx,pTabInfo->u16RegNum,COMB_REG_BASE | (MS_U32)u8Addr, u8Mask, u8Value, u8CurBank));

        if (u8CurBank != COMB_BK_CURRENT)
        {
            PQ_BIN_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
            COMB_BK_SWITCH(u8CurBank);
        }

        u32RegAddr = COMB_REG_BASE | (MS_U16)u8Addr;

        PQ_BIN_DUMP_DBG(printf("[addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value));
        PQ_REG_FUNC(u32RegAddr, u8Value, u8Mask);

    }
    COMB_BK_RESTORE;
}

#define BITSHIFT(x) ((((x)&0x01) == 0x01)? 0 : ( \
                     (((x)&0x02) == 0x02)? 1 : ( \
                     (((x)&0x04) == 0x04)? 2 : ( \
                     (((x)&0x08) == 0x08)? 3 : ( \
                     (((x)&0x10) == 0x10)? 4 : ( \
                     (((x)&0x20) == 0x20)? 5 : ( \
                     (((x)&0x40) == 0x40)? 6 : 7 )))))))


void MDrv_PQBin_DumpCloneScalerRegTable(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabMainInfo,
        const MS_PQBin_IP_Table_Info* const pTabSubInfo)
{
    MS_PHYADDR u32RegAddr1, u32RegAddr2;
    MS_U8 u8Mask1, u8Mask2;
    MS_U8 u8Addr1, u8Addr2;
    MS_U8 u8Value;
    MS_U8 u8CurBank1 = 0xff, u8CurBank2 = 0xff;
    MS_PHYADDR u32Addr1, u32Addr2;
    MS_U32 u32Offset1, u32Offset2;
    MS_U8 u8BitShift1 = 0, u8BitShift2 = 0;
    MS_U16 i;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_lock(&_PQ_MLoad_Mutex);
#endif
#endif
    //consider dump reg amount is very large, so we shouldn't use MApi_XC_W2BYTE(),
    //  because mutex cost much time.
    SC_BK_STORE_MUTEX;

    PQ_BIN_DUMP_CLONE_DBG(printf("tab: sc\n"));
    if(  (pTabMainInfo->u16GroupIdx >= pTabMainInfo->u16GroupNum)
      || (pTabSubInfo->u16GroupIdx >= pTabMainInfo->u16GroupNum))
    {
        PQ_BIN_DUMP_CLONE_DBG(printf("[PQBin]IP_Info error: Scaler Reg Table\n"));

        SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
        pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif

        return;
    }

    u32Addr1 = pTabMainInfo->u32TabOffset;
    u32Addr2 = pTabSubInfo->u32TabOffset;
    for(i=0; i<pTabMainInfo->u16RegNum; i++)
    {
        u32Offset1 = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabMainInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank1 = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr1+u32Offset1)), &u32Offset1);
        u8Addr1    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr1+u32Offset1)), &u32Offset1);
        u8Mask1    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr1+u32Offset1)), &u32Offset1);

        u32Offset2 = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabSubInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank2 = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr2+u32Offset2)), &u32Offset2);
        u8Addr2    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr2+u32Offset2)), &u32Offset2);
        u8Mask2    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr2+u32Offset2)), &u32Offset2);

        //read from main reg
#if (SCALER_REGISTER_SPREAD)
        u32RegAddr1 = BK_SCALER_BASE | (((MS_U32)u8CurBank1) << 8) | (MS_U32)u8Addr1;
#else
        if (u8CurBank1 != SC_BK_CURRENT)
        {
            PQ_BIN_DUMP_CLONE_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank1));
            SC_BK_SWITCH(u8CurBank1);
        }

        u32RegAddr1 = BK_SCALER_BASE | (MS_U32)u8Addr1;
#endif

        u8Value = PQ_REG_READ_FUNC(u32RegAddr1) & u8Mask1;

        //when clone main and sub's mask bit is different
        //we need to shift the value and then write to new mask position
        u8BitShift1 = BITSHIFT(u8Mask1);
        u8BitShift2 = BITSHIFT(u8Mask2);

        if(u8BitShift1 > u8BitShift2)
        {
            u8Value = u8Value >> (u8BitShift1 - u8BitShift2);
        }
        else
        {
            u8Value = u8Value << (u8BitShift2 - u8BitShift1);
        }

        //write to sub reg
        if(_bMLoadEn)
        {
            u32RegAddr2 = (((MS_U32)u8CurBank2) << 8) | (MS_U32)u8Addr2;
            PQ_REG_MLOAD_WRITE_CMD(u32RegAddr2, u8Value, u8Mask2);
        }
        else
        {

#if (SCALER_REGISTER_SPREAD)
            u32RegAddr2 = BK_SCALER_BASE | (((MS_U32)u8CurBank2) << 8) | (MS_U32)u8Addr2;
#else
            if (u8CurBank2 != SC_BK_CURRENT)
            {
                PQ_BIN_DUMP_CLONE_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank2));
                SC_BK_SWITCH(u8CurBank2);
            }

            u32RegAddr2 = BK_SCALER_BASE | (MS_U32)u8Addr2;
#endif
            PQ_REG_FUNC(u32RegAddr2, u8Value, u8Mask2);
        }
        PQ_BIN_DUMP_CLONE_DBG(printf("%u  [read from addr=%lx], GroupIdx=%x,RegNum=%x,reg=%04lx, msk=%02x, u8CurBank=%x\n",
            i,(u32Addr1+u32Offset1-(MS_U32)pTabMainInfo->u16GroupIdx),pTabMainInfo->u16GroupIdx,pTabMainInfo->u16RegNum,u32RegAddr1, u8Mask1, u8CurBank1));
        PQ_BIN_DUMP_CLONE_DBG(printf("%u  [write to  addr=%lx], GroupIdx=%x,RegNum=%x,reg=%04lx, msk=%02x, u8CurBank=%x\n",
            i,(u32Addr2+u32Offset2-(MS_U32)pTabSubInfo->u16GroupIdx),pTabSubInfo->u16GroupIdx,pTabSubInfo->u16RegNum,u32RegAddr2, u8Mask2, u8CurBank2));

        PQ_BIN_DUMP_CLONE_DBG(printf("[value=%02x], shift1=%u, shift2=%u\n", u8Value, u8BitShift1, u8BitShift2));
    }

    SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif
}

#if (defined (UFO_XC_DS_PQ))
stDS_PQ_REG DS_REG[DS_PQ_MAX_NUM];
MS_U16 DS_REG_CNT = 0;
stDS_PQ_REG DS_UFSC_REG[DS_PQ_MAX_NUM];
MS_U16 DS_UFSC_REG_CNT = 0;
MS_BOOL bUFSC_DSEn=0;
void MDrv_PQBin_DumpScalerRegTable_v2(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabInfo)
{
    MS_PHYADDR u32RegAddr;
    MS_U8 u8Mask;
    MS_U8 u8Addr;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U32 x =0;

    MS_U16 i;

    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        //XC_PRINTF("[PQBin]IP_Info error: Scaler Reg Table\n");


        return;
    }


    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i<pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  (MS_U32)pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

#if (SCALER_REGISTER_SPREAD)
        u32RegAddr = /*BK_SCALER_BASE |*/ (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
#else
        if (u8CurBank != SC_BK_CURRENT)
        {
            //XC_PRINTF("<<bankswitch=%02x>>\n", u8CurBank);
            SC_BK_SWITCH(u8CurBank);
        }

        u32RegAddr = BK_SCALER_BASE | (MS_U32)u8Addr;
#endif
        //XC_PRINTF("[addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value);
        if(DS_REG_CNT == 0)
        {
            if((u32RegAddr%2)==0)
            {
                DS_REG[DS_REG_CNT].u16Value = u8Value;
                DS_REG[DS_REG_CNT].u16Mask  = u8Mask;
            }
            else
            {
                DS_REG[DS_REG_CNT].u16Value = u8Value<<8;
                DS_REG[DS_REG_CNT].u16Mask  = u8Mask<<8;
            }

            DS_REG[DS_REG_CNT].u16Addr  = u32RegAddr;
            DS_REG[DS_REG_CNT].u16Addr = (DS_REG[DS_REG_CNT].u16Addr/2) *2;
            DS_REG_CNT++;
        }else
        {
            for(x=0;x < DS_REG_CNT;x++)
            {
                if((DS_REG[x].u16Addr/2) == (u32RegAddr/2))
                {
                    if(DS_REG[x].u16Addr < u32RegAddr)
                    {
                        DS_REG[x].u16Value = (DS_REG[x].u16Value)    | (u8Value<<8);
                        DS_REG[x].u16Mask  = (DS_REG[x].u16Mask)    | (u8Mask<<8);
                    }
                    else
                    {
                        DS_REG[x].u16Value = (u8Value)      | DS_REG[x].u16Value;
                        DS_REG[x].u16Mask  = (u8Mask)        | DS_REG[x].u16Mask;
                    }
                    break;
                }
            }
            if( x == DS_REG_CNT )
            {
                if((u32RegAddr%2)==0)
                {
                    DS_REG[DS_REG_CNT].u16Value = u8Value;
                    DS_REG[DS_REG_CNT].u16Mask  = u8Mask;
                }
                else
                {
                    DS_REG[DS_REG_CNT].u16Value = u8Value<<8;
                    DS_REG[DS_REG_CNT].u16Mask  = u8Mask<<8;
                }

                DS_REG[DS_REG_CNT].u16Addr  = u32RegAddr;
                DS_REG[DS_REG_CNT].u16Addr = (DS_REG[DS_REG_CNT].u16Addr/2) *2;
                DS_REG_CNT++;
            }
        }
        if(DS_REG_CNT >= DS_PQ_MAX_NUM)
        {
            //XC_PRINTF("[PQBin]IP_Info error: DS-PQ CNT has reach max num!\n");
            MS_ASSERT(0);
        }

    }

}
void MDrv_PQBin_DumpUFSCScalerRegTable_v2(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabInfo)
{
    MS_PHYADDR u32RegAddr;
    MS_U8 u8Mask;
    MS_U8 u8Addr;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U32 x =0;

    MS_U16 i;

    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        //XC_PRINTF("[PQBin]IP_Info error: Scaler Reg Table\n");


        return;
    }


    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i<pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  (MS_U32)pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        if (0)//(_bMLoadEn)
        {
            u32RegAddr = (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
        }
        else
        {
            u32RegAddr = BK_UFSC_SCALER_BASE | (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
        }

        //XC_PRINTF("[addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value);

        if(DS_UFSC_REG_CNT == 0)
        {
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Bank  = (BK_UFSC_SCALER_BASE>>8) | (MS_U32)u8CurBank;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Value = u8Value;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Mask  = u8Mask;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr = u8Addr;
                if((DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr/2) *2 != DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr)
                {
                    DS_UFSC_REG[x].u16Value = (DS_UFSC_REG[x].u16Value) <<8;
                    DS_UFSC_REG[x].u16Mask  = (DS_UFSC_REG[x].u16Mask) <<8;
                    DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr = (DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr/2) *2;
                }
                DS_UFSC_REG_CNT++;
        }else
        {
            for(x=0;x < DS_UFSC_REG_CNT;x++)
            {
                if(DS_UFSC_REG[x].u16Bank == ((BK_UFSC_SCALER_BASE>>8) | (MS_U32)u8CurBank))
                {
                    if((DS_UFSC_REG[x].u16Addr/2) == ((MS_U16)u8Addr/2))
                    {
                        if(DS_UFSC_REG[x].u16Addr < (MS_U16)u8Addr)
                        {
                            DS_UFSC_REG[x].u16Value = (DS_UFSC_REG[x].u16Value)    | (u8Value<<8);
                            DS_UFSC_REG[x].u16Mask  = (DS_UFSC_REG[x].u16Mask)    | (u8Mask<<8);
                        }
                        else
                        {
                            DS_UFSC_REG[x].u16Value = (u8Value)      | DS_UFSC_REG[x].u16Value;
                            DS_UFSC_REG[x].u16Mask  = (u8Mask)        | DS_UFSC_REG[x].u16Mask;
                        }
                        break;
                    }
                }
            }
            if( x == DS_UFSC_REG_CNT )
            {
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Bank  = (BK_UFSC_SCALER_BASE>>8) | (MS_U32)u8CurBank;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Value = u8Value;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Mask  = u8Mask;
                DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr = u8Addr;
                if((DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr/2) *2 != DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr)
                {
                    DS_UFSC_REG[x].u16Value = (DS_UFSC_REG[x].u16Value) <<8;
                    DS_UFSC_REG[x].u16Mask  = (DS_UFSC_REG[x].u16Mask) <<8;
                    DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr = (DS_UFSC_REG[DS_UFSC_REG_CNT].u16Addr/2) *2;
                }
                DS_UFSC_REG_CNT++;
            }
        }

        if(DS_UFSC_REG_CNT >= DS_PQ_MAX_NUM)
        {
            //XC_PRINTF("[PQBin]IP_Info error: DS-PQ CNT has reach max num!\n");
            MS_ASSERT(0);
        }

    }
    bUFSC_DSEn=TRUE;

}
#endif
void MDrv_PQBin_DumpScalerRegTable(void *pInstance,const MS_PQBin_IP_Table_Info* const pTabInfo)
{
    MS_PHYADDR u32RegAddr;
    MS_U8 u8Mask;
    MS_U8 u8Addr;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;

    MS_U16 i;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_lock(&_PQ_MLoad_Mutex);
#endif
#endif
    //consider dump reg amount is very large, so we shouldn't use MApi_XC_W2BYTE(),
    //  because mutex cost much time.
    SC_BK_STORE_MUTEX;

    PQ_BIN_DUMP_DBG(printf("tab: sc\n"));
    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DBG(printf("[PQBin]IP_Info error: Scaler Reg Table\n"));

        SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
        pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif

        return;
    }

#if (ENABLE_PQ_MLOAD)
    if (_bMLoadEn)
    {
        if (pTabInfo->u16RegNum >= MLOAD_MAX_CMD)
        {
            printf("[PQ ERROR] Function %s ==== ML overflow !!!\n",__FUNCTION__);
            MS_ASSERT(FALSE);
        }
        else if ( (_u16MLoadCmdCnt + pTabInfo->u16RegNum) >= MLOAD_MAX_CMD)
        {
            SC_BK_RESTORE_MUTEX;
            MDrv_PQBin_Set_MLoadEn(pInstance,FALSE);
            MDrv_PQBin_Set_MLoadEn(pInstance,TRUE);
            SC_BK_STORE_MUTEX;
        }
    }
#endif

    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i<pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  (MS_U32)pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        PQ_BIN_DBG_SRAMERROR(printf("1  [read addr=%lx], GroupIdx=%x,RegNum=%x,reg=%04lx, msk=%02x, val=%02x, u8CurBank=%x\n",
            (u32Addr+u32Offset-(MS_U32)pTabInfo->u16GroupIdx),pTabInfo->u16GroupIdx,pTabInfo->u16RegNum,BK_SCALER_BASE | (MS_U32)u8Addr, u8Mask, u8Value, u8CurBank));

        if(_bMLoadEn)
        {
            u32RegAddr = (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
            //PQ_REG_MLOAD_FUNC(u32RegAddr, u8Value, u8Mask);
                      PQ_REG_MLOAD_WRITE_CMD(u32RegAddr, u8Value, u8Mask);
        }
        else
        {
#if (SCALER_REGISTER_SPREAD)
            u32RegAddr = BK_SCALER_BASE | (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
#else
            if (u8CurBank != SC_BK_CURRENT)
            {
                PQ_BIN_DUMP_DBG(printf("<<bankswitch=%02x>>\n", u8CurBank));
                SC_BK_SWITCH(u8CurBank);
            }

            u32RegAddr = BK_SCALER_BASE | (MS_U32)u8Addr;
#endif
            PQ_REG_FUNC(u32RegAddr, u8Value, u8Mask);
        }
        PQ_BIN_DUMP_DBG(printf("[addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value));

    }

    SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if(ENABLE_PQ_MLOAD)
    pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif
}

void MDrv_PQBin_DumpFilterTable(void *pInstance,MS_PQBin_IP_Table_Info *pTabInfo)
{
    MS_PHYADDR u32Addr;
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32PTHChk;
    s32PTHChk = pthread_mutex_lock(&_stPQFilterTableMutex);
    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d lock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif

    PQ_BIN_DBG(printf("tab: sram\n"));

    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]IP_Info error: SRAM Table\n"));
#ifdef MSOS_TYPE_LINUX
        s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
        if(s32PTHChk != 0)
        {
            printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
        }
#endif
        return;
    }

    u32Addr = pTabInfo->u32TabOffset + pTabInfo->u16GroupIdx * pTabInfo->u16RegNum;

    switch(pTabInfo->u16TableType)
    {
    case PQ_TABTYPE_SRAM1:
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM2:
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM3:
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM4:
        Hal_PQ_set_y_sram_table(pInstance,SC_FILTER_Y_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM1:
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM2:
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM3:
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM4:
        Hal_PQ_set_c_sram_table(pInstance,SC_FILTER_C_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_INDEX:
        Hal_PQ_set_sram_color_index_table(pInstance,SC_FILTER_SRAM_COLOR_INDEX, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_GAIN_SNR:
        Hal_PQ_set_sram_color_gain_snr_table(pInstance,SC_FILTER_SRAM_COLOR_GAIN_SNR, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_GAIN_DNR:
        Hal_PQ_set_sram_color_gain_dnr_table(pInstance,SC_FILTER_SRAM_COLOR_GAIN_DNR, (void *)u32Addr);
        break;
    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
        Hal_PQ_set_sram_icc_crd_table(pInstance,SC_FILTER_SRAM_ICC_CRD, (void *)u32Addr);
        break;
    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
        Hal_PQ_set_sram_ihc_crd_table(pInstance,SC_FILTER_SRAM_IHC_CRD, (void *)u32Addr);
        break;
#ifdef MaseratiLinearRGB
    case PQ_TABTYPE_LinearRGB_DE_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_DE_GAMMA, (void *)u32Addr);
        break;
    case PQ_TABTYPE_LinearRGB_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_GAMMA, (void *)u32Addr);
        break;
#else
    case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_DE_GAMMA, (void *)u32Addr);
        break;
    case PQ_TABTYPE_XVYCC_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_GAMMA, (void *)u32Addr);
        break;
#endif
    default:
        printf("[PQBin] Unknown sram type %u\n", pTabInfo->u16TableType);
        MS_ASSERT(0);
        break;
    }

#ifdef MSOS_TYPE_LINUX
    s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif
}

#ifdef UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
void MDrv_PQBin_DumpFilterTable_Main2(void *pInstance,MS_PQBin_IP_Table_Info *pTabInfo)
{
    MS_PHYADDR u32Addr;
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32PTHChk;
    s32PTHChk = pthread_mutex_lock(&_stPQFilterTableMutex);
    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d lock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif

    PQ_BIN_DBG(printf("tab: sram\n"));

    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]IP_Info error: SRAM Table\n"));
#ifdef MSOS_TYPE_LINUX
        s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
        if(s32PTHChk != 0)
        {
            printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
        }
#endif
        return;
    }

    u32Addr = pTabInfo->u32TabOffset + pTabInfo->u16GroupIdx * pTabInfo->u16RegNum;

    switch(pTabInfo->u16TableType)
    {
    case PQ_TABTYPE_SRAM1:
        Hal_PQ_set_y_sram_table_Main2(pInstance,SC_FILTER_Y_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM2:
        Hal_PQ_set_y_sram_table_Main2(pInstance,SC_FILTER_Y_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM3:
        Hal_PQ_set_y_sram_table_Main2(pInstance,SC_FILTER_Y_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM4:
        Hal_PQ_set_y_sram_table_Main2(pInstance,SC_FILTER_Y_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM1:
        Hal_PQ_set_c_sram_table_Main2(pInstance,SC_FILTER_C_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM2:
        Hal_PQ_set_c_sram_table_Main2(pInstance,SC_FILTER_C_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM3:
        Hal_PQ_set_c_sram_table_Main2(pInstance,SC_FILTER_C_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_C_SRAM4:
        Hal_PQ_set_c_sram_table_Main2(pInstance,SC_FILTER_C_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_INDEX:
        Hal_PQ_set_sram_color_index_table(pInstance,SC_FILTER_SRAM_COLOR_INDEX, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_GAIN_SNR:
        Hal_PQ_set_sram_color_gain_snr_table(pInstance,SC_FILTER_SRAM_COLOR_GAIN_SNR, (void *)u32Addr);
        break;
    case PQ_TABTYPE_SRAM_COLOR_GAIN_DNR:
        Hal_PQ_set_sram_color_gain_dnr_table(pInstance,SC_FILTER_SRAM_COLOR_GAIN_DNR, (void *)u32Addr);
        break;
    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
        Hal_PQ_set_sram_icc_crd_table(pInstance,SC_FILTER_SRAM_ICC_CRD, (void *)u32Addr);
        break;
    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
        Hal_PQ_set_sram_ihc_crd_table(pInstance,SC_FILTER_SRAM_IHC_CRD, (void *)u32Addr);
        break;
#ifdef MaseratiLinearRGB
    case PQ_TABTYPE_LinearRGB_DE_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_DE_GAMMA, (void *)u32Addr);
        break;
    case PQ_TABTYPE_LinearRGB_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_GAMMA, (void *)u32Addr);
        break;
#else
    case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_DE_GAMMA, (void *)u32Addr);
        break;
    case PQ_TABTYPE_XVYCC_GAMMA_SRAM:
        Hal_PQ_set_sram_xvycc_gamma_table(pInstance,SC_FILTER_SRAM_XVYCC_GAMMA, (void *)u32Addr);
        break;
#endif
    default:
        printf("[PQBin] Unknown sram type %u\n", pTabInfo->u16TableType);
        MS_ASSERT(0);
        break;
    }

#ifdef MSOS_TYPE_LINUX
    s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif
}
#endif

void MDrv_PQBin_DumpCloneTable(void *pInstance,MS_PQBin_IP_Table_Info* pTabMainInfo, MS_PQBin_IP_Table_Info* pTabSubInfo)
{
    if ((pTabMainInfo->u16RegNum == 0) || (pTabMainInfo->u32TabOffset == PQ_BIN_ADDR_NULL))
    {
        PQ_BIN_DUMP_CLONE_DBG(printf("NULL Table\n"));
        return;
    }

    if ((pTabSubInfo->u16RegNum == 0) || (pTabSubInfo->u32TabOffset == PQ_BIN_ADDR_NULL))
    {
        PQ_BIN_DUMP_CLONE_DBG(printf("NULL Table\n"));
        return;
    }

    switch(pTabMainInfo->u16TableType)
    {
    case PQ_TABTYPE_SCALER:
        MDrv_PQBin_DumpCloneScalerRegTable(pInstance,pTabMainInfo, pTabSubInfo);
        break;

    default:
        PQ_BIN_DUMP_CLONE_DBG(printf("[PQBin]DumpCloneTable:unknown type: %u\n", pTabMainInfo->u16TableType));
        break;
    }
}
#if (defined (UFO_XC_DS_PQ))
void MDrv_PQBin_DS_DumpTable(void *pInstance,MS_PQBin_IP_Table_Info* pTabInfo)
{
    if ((pTabInfo->u16RegNum == 0) || (pTabInfo->u32TabOffset == PQ_BIN_ADDR_NULL))
    {
        PQ_BIN_DUMP_DBG(printf("NULL Table\n"));
        return;
    }
    if(pTabInfo->u16TableType == PQ_TABTYPE_SCALER)
        MDrv_PQBin_DumpScalerRegTable_v2(pInstance,pTabInfo);
     else if(pTabInfo->u16TableType == PQ_TABTYPE_UFSC_SCALER)
        MDrv_PQBin_DumpUFSCScalerRegTable_v2(pInstance,pTabInfo);
    return;
}
#endif
void MDrv_PQBin_DumpTable(void *pInstance,MS_PQBin_IP_Table_Info* pTabInfo)
{
    if ((pTabInfo->u16RegNum == 0) || (pTabInfo->u32TabOffset == PQ_BIN_ADDR_NULL))
    {
        PQ_BIN_DUMP_DBG(printf("NULL Table\n"));
        return;
    }
    switch(pTabInfo->u16TableType)
    {
    case PQ_TABTYPE_SCALER:
        MDrv_PQBin_DumpScalerRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_COMB:
        MDrv_PQBin_DumpCombRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_GENERAL:
        MDrv_PQBin_DumpGeneralRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_SRAM1:
        if (_u16SRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram1: %u, new sram1: %u\n",
                _u16SRAM1Table, pTabInfo->u16GroupIdx));

            _u16SRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM2:
        if (_u16SRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram2: %u, new sram2: %u\n",
                _u16SRAM2Table, pTabInfo->u16GroupIdx));

            _u16SRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM3:
        if (_u16SRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram3: %u, new sram3: %u\n",
                _u16SRAM3Table, pTabInfo->u16GroupIdx));

            _u16SRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM4:
        if (_u16SRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram4: %u, new sram4: %u\n",
                _u16SRAM4Table, pTabInfo->u16GroupIdx));

            _u16SRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_C_SRAM1:
        if (_u16CSRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram1: %u, new Csram1: %u\n",
                _u16CSRAM1Table, pTabInfo->u16GroupIdx));

            _u16CSRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM2:
        if (_u16CSRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old Csram2: %u, new Csram2: %u\n",
                _u16CSRAM2Table, pTabInfo->u16GroupIdx));

            _u16CSRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM3:
        if (_u16CSRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old Csram3: %u, new Csram3: %u\n",
                _u16CSRAM3Table, pTabInfo->u16GroupIdx));

            _u16CSRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM4:
        if (_u16CSRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram4: %u, new Csram4: %u\n",
                _u16CSRAM2Table, pTabInfo->u16GroupIdx));

            _u16CSRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
        if(_u16ICC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old ICC_CRD_SRAM: %u, new ICC_CRD_SRAM: %u\n",
                _u16ICC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16ICC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        break;

    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
        if(_u16IHC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old IHC_CRD_SRAM: %u, new IHC_CRD_SRAM: %u\n",
                _u16IHC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16IHC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        break;
#ifdef MaseratiLinearRGB
   case PQ_TABTYPE_LinearRGB_DE_GAMMA_SRAM:
        if (_u16XVYCC_DE_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin] xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16XVYCC_DE_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16XVYCC_DE_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_LinearRGB_GAMMA_SRAM:
        if (_u16XVYCC_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16XVYCC_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16XVYCC_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

#else
    case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
        if (_u16XVYCC_DE_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin] xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16XVYCC_DE_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16XVYCC_DE_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_XVYCC_GAMMA_SRAM:
        if (_u16XVYCC_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16XVYCC_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16XVYCC_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
#endif
   //---------------------------------------------------------------------------------------------------------------
#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
    case PQ_TABTYPE_UFSC_SCALER:
        MDrv_PQBin_DumpUFSCScalerRegTable(pInstance, pTabInfo);
        break;
    #if 0
    //case PQ_TABTYPE_UFSC_COMB:
    //    MDrv_PQBin_DumpUFSCCombRegTable(pTabInfo);
    //    break;
    case PQ_TABTYPE_UFSC_GENERAL:
        MDrv_PQBin_DumpUFSCGeneralRegTable(pTabInfo);
        break;
    #endif

    case PQ_TABTYPE_UFSC_SRAM1:
        if (_u16UFSCSRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram1: %u, new sram1: %u\n",
                _u16UFSCSRAM1Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM2:
        if (_u16UFSCSRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram2: %u, new sram2: %u\n",
                _u16UFSCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM3:
        if (_u16UFSCSRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin[UFSC] ]old sram3: %u, new sram3: %u\n",
                _u16UFSCSRAM3Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM4:
        if (_u16UFSCSRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram4: %u, new sram4: %u\n",
                _u16UFSCSRAM4Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_UFSC_C_SRAM1:
        if (_u16UFSCCSRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram1: %u, new Csram1: %u\n",
                _u16UFSCCSRAM1Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM2:
        if (_u16UFSCCSRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old Csram2: %u, new Csram2: %u\n",
                _u16UFSCCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM3:
        if (_u16UFSCCSRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old Csram3: %u, new Csram3: %u\n",
                _u16UFSCCSRAM3Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM4:
        if (_u16UFSCCSRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram4: %u, new Csram4: %u\n",
                _u16UFSCCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_UFSC_VIP_ICC_CRD_SRAM:
        if(_u16UFSCICC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old ICC_CRD_SRAM: %u, new ICC_CRD_SRAM: %u\n",
                _u16UFSCICC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16UFSCICC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        break;

    case PQ_TABTYPE_UFSC_VIP_IHC_CRD_SRAM:
        if(_u16UFSCIHC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old IHC_CRD_SRAM: %u, new IHC_CRD_SRAM: %u\n",
                _u16UFSCIHC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16UFSCIHC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        break;

    #if 0
    case PQ_TABTYPE_UFSC_XVYCC_DE_GAMMA_SRAM:
        if (_u16UFSCXVYCC_DE_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC]  xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16UFSCXVYCC_DE_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16UFSCXVYCC_DE_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_XVYCC_GAMMA_SRAM:
        if (_u16UFSCXVYCC_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16UFSCXVYCC_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16UFSCXVYCC_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_100SERIES:
        MDrv_PQBin_DumpUFSC100RegTable(pTabInfo);
        break;
    #endif
#endif
    default:
        PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]DumpTable:unknown type: %u\n", pTabInfo->u16TableType));
        break;
    }
}

#ifdef UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
void MDrv_PQBin_DumpTable_Main2(void *pInstance,MS_PQBin_IP_Table_Info* pTabInfo)
{
    if ((pTabInfo->u16RegNum == 0) || (pTabInfo->u32TabOffset == PQ_BIN_ADDR_NULL))
    {
        PQ_BIN_DUMP_DBG(printf("NULL Table\n"));
        return;
    }

    switch(pTabInfo->u16TableType)
    {
    case PQ_TABTYPE_SCALER:
        MDrv_PQBin_DumpScalerRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_COMB:
        MDrv_PQBin_DumpCombRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_GENERAL:
        MDrv_PQBin_DumpGeneralRegTable(pInstance,pTabInfo);
        break;
    case PQ_TABTYPE_SRAM1:
        if (_u16SRAM1Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram1: %u, new sram1: %u\n",
                _u16SRAM1Table_main2, pTabInfo->u16GroupIdx));

            _u16SRAM1Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM2:
        if (_u16SRAM2Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram2: %u, new sram2: %u\n",
                _u16SRAM2Table_main2, pTabInfo->u16GroupIdx));

            _u16SRAM2Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM3:
        if (_u16SRAM3Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram3: %u, new sram3: %u\n",
                _u16SRAM3Table_main2, pTabInfo->u16GroupIdx));

            _u16SRAM3Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_SRAM4:
        if (_u16SRAM4Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram4: %u, new sram4: %u\n",
                _u16SRAM4Table_main2, pTabInfo->u16GroupIdx));

            _u16SRAM4Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same sram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_C_SRAM1:
        if (_u16CSRAM1Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram1: %u, new Csram1: %u\n",
                _u16CSRAM1Table_main2, pTabInfo->u16GroupIdx));

            _u16CSRAM1Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM2:
        if (_u16CSRAM2Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old Csram2: %u, new Csram2: %u\n",
                _u16CSRAM2Table_main2, pTabInfo->u16GroupIdx));

            _u16CSRAM2Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM3:
        if (_u16CSRAM3Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old Csram3: %u, new Csram3: %u\n",
                _u16CSRAM3Table_main2, pTabInfo->u16GroupIdx));

            _u16CSRAM3Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_C_SRAM4:
        if (_u16CSRAM4Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old sram4: %u, new Csram4: %u\n",
                _u16CSRAM2Table_main2, pTabInfo->u16GroupIdx));

            _u16CSRAM4Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable_Main2(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same Csram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_VIP_ICC_CRD_SRAM:
        if(_u16ICC_CRD_Table_main2  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old ICC_CRD_SRAM: %u, new ICC_CRD_SRAM: %u\n",
                _u16ICC_CRD_Table_main2, pTabInfo->u16GroupIdx));

            _u16ICC_CRD_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        break;

    case PQ_TABTYPE_VIP_IHC_CRD_SRAM:
        if(_u16IHC_CRD_Table_main2  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]old IHC_CRD_SRAM: %u, new IHC_CRD_SRAM: %u\n",
                _u16IHC_CRD_Table_main2, pTabInfo->u16GroupIdx));

            _u16IHC_CRD_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        break;
#ifdef MaseratiLinearRGB
   case PQ_TABTYPE_LinearRGB_DE_GAMMA_SRAM:
        if (_u16XVYCC_DE_GAMMA_SRAM_Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin] xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16XVYCC_DE_GAMMA_SRAM_Table_main2, pTabInfo->u16GroupIdx));

            _u16XVYCC_DE_GAMMA_SRAM_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_LinearRGB_GAMMA_SRAM:
        if (_u16XVYCC_GAMMA_SRAM_Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16XVYCC_GAMMA_SRAM_Table_main2, pTabInfo->u16GroupIdx));

            _u16XVYCC_GAMMA_SRAM_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

#else
    case PQ_TABTYPE_XVYCC_DE_GAMMA_SRAM:
        if (_u16XVYCC_DE_GAMMA_SRAM_Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin] xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16XVYCC_DE_GAMMA_SRAM_Table_main2, pTabInfo->u16GroupIdx));

            _u16XVYCC_DE_GAMMA_SRAM_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_XVYCC_GAMMA_SRAM:
        if (_u16XVYCC_GAMMA_SRAM_Table_main2 != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16XVYCC_GAMMA_SRAM_Table_main2, pTabInfo->u16GroupIdx));

            _u16XVYCC_GAMMA_SRAM_Table_main2 = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
#endif
   //---------------------------------------------------------------------------------------------------------------
#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
    case PQ_TABTYPE_UFSC_SCALER:
        MDrv_PQBin_DumpUFSCScalerRegTable(pInstance, pTabInfo);
        break;
    #if 0
    //case PQ_TABTYPE_UFSC_COMB:
    //    MDrv_PQBin_DumpUFSCCombRegTable(pTabInfo);
    //    break;
    case PQ_TABTYPE_UFSC_GENERAL:
        MDrv_PQBin_DumpUFSCGeneralRegTable(pTabInfo);
        break;
    #endif

    case PQ_TABTYPE_UFSC_SRAM1:
        if (_u16UFSCSRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram1: %u, new sram1: %u\n",
                _u16UFSCSRAM1Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM2:
        if (_u16UFSCSRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram2: %u, new sram2: %u\n",
                _u16UFSCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM3:
        if (_u16UFSCSRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin[UFSC] ]old sram3: %u, new sram3: %u\n",
                _u16UFSCSRAM3Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_SRAM4:
        if (_u16UFSCSRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram4: %u, new sram4: %u\n",
                _u16UFSCSRAM4Table, pTabInfo->u16GroupIdx));

            _u16UFSCSRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same sram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_UFSC_C_SRAM1:
        if (_u16UFSCCSRAM1Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram1: %u, new Csram1: %u\n",
                _u16UFSCCSRAM1Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM1Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram1: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM2:
        if (_u16UFSCCSRAM2Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old Csram2: %u, new Csram2: %u\n",
                _u16UFSCCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM2Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram2: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM3:
        if (_u16UFSCCSRAM3Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old Csram3: %u, new Csram3: %u\n",
                _u16UFSCCSRAM3Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM3Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram3: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_C_SRAM4:
        if (_u16UFSCCSRAM4Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old sram4: %u, new Csram4: %u\n",
                _u16UFSCCSRAM2Table, pTabInfo->u16GroupIdx));

            _u16UFSCCSRAM4Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same Csram4: %u\n", pTabInfo->u16GroupIdx));
        }
        break;

    case PQ_TABTYPE_UFSC_VIP_ICC_CRD_SRAM:
        if(_u16UFSCICC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old ICC_CRD_SRAM: %u, new ICC_CRD_SRAM: %u\n",
                _u16UFSCICC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16UFSCICC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        break;

    case PQ_TABTYPE_UFSC_VIP_IHC_CRD_SRAM:
        if(_u16UFSCIHC_CRD_Table  != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] old IHC_CRD_SRAM: %u, new IHC_CRD_SRAM: %u\n",
                _u16UFSCIHC_CRD_Table, pTabInfo->u16GroupIdx));

            _u16UFSCIHC_CRD_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance, pTabInfo);
        }
        break;

    #if 0
    case PQ_TABTYPE_UFSC_XVYCC_DE_GAMMA_SRAM:
        if (_u16UFSCXVYCC_DE_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC]  xvYcc DE Gamma SRAM: %u, new xvYcc DE Gamma SRAM: %u\n",
                _u16UFSCXVYCC_DE_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16UFSCXVYCC_DE_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same xvYcc DE Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_XVYCC_GAMMA_SRAM:
        if (_u16UFSCXVYCC_GAMMA_SRAM_Table != pTabInfo->u16GroupIdx)
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] xvYcc Gamma SRAM: %u, new xvYcc Gamma SRAM: %u\n",
                _u16UFSCXVYCC_GAMMA_SRAM_Table, pTabInfo->u16GroupIdx));

            _u16UFSCXVYCC_GAMMA_SRAM_Table = pTabInfo->u16GroupIdx;
            MDrv_PQBin_DumpUFSCFilterTable(pInstance,pTabInfo);
        }
        else
        {
            PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin][UFSC] use the same xvYcc Gamma SRAM: %u\n", pTabInfo->u16GroupIdx));
        }
        break;
    case PQ_TABTYPE_UFSC_100SERIES:
        MDrv_PQBin_DumpUFSC100RegTable(pTabInfo);
        break;
    #endif
#endif
    default:
        PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]DumpTable:unknown type: %u\n", pTabInfo->u16TableType));
        break;
    }
}
#endif

// return total IP count
MS_U16 MDrv_PQBin_GetIPNum(void *pInstance,const MS_PQBin_Header_Info* const pPQBinHeader)
{
    PQ_BIN_DBG(printf("[PQBin]:IPNum=%u\n", pPQBinHeader->u16IP_Comm_Num));
    return pPQBinHeader->u16IP_Comm_Num;
}

// return total table count of given IP
MS_U16 MDrv_PQBin_GetTableNum(void *pInstance,MS_U16 u16PQIPIdx, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16TabNum;

    u32Addr = pPQBinHeader->u32BinStartAddress + pPQBinHeader->u32IP_Comm_Offset;
    u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_IP_GROUP_NUM_OFFSET;
    u16TabNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
    PQ_BIN_DBG(printf("[PQBin]TabNum=%u\n", u16TabNum));
    return u16TabNum;
}

// return current used table index of given IP
MS_U16 MDrv_PQBin_GetCurrentTableIndex(void *pInstance,MS_U16 u16PnlIdx, MS_U16 u16PQIPIdx)
{
    printf("[PQBin]CurrTableIdx=%u\n", _u16PQTabIdx[u16PnlIdx][u16PQIPIdx]);

    return _u16PQTabIdx[u16PnlIdx][u16PQIPIdx];
}


MS_U16 MDrv_PQBin_GetTableIndex(void *pInstance,MS_U16 u16PQSrcType, MS_U16 u16PQIPIdx, MS_U16 u16PQPnlIdx, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_PHYADDR u32SourceLUT_Addr;
    MS_U32 u32SourceLUT_Offset;
    MS_U16 u16SourceLUT_SourceNum;
    MS_U16 u16SourceLUT_IPNum;
    MS_U32 u32SourceLUT_Data_Pitch;
    MS_U16 u16TableIdx;

    if(u16PQPnlIdx >= pPQBinHeader->u16SourceLUT_PnlNum)
    {
        (printf("[PQBin]invalid panel type\n"));
        return PQ_BIN_IP_NULL;
    }


    u32Addr = pPQBinHeader->u32BinStartAddress + pPQBinHeader->u32SourceLUT_Offset;
    u32Offset =  pPQBinHeader->u32SourceLUT_Pitch * u16PQPnlIdx + PQ_BIN_SOURCELUT_SOURCE_NUM_OFFSET;
    u16SourceLUT_SourceNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
    if (u16PQSrcType >= u16SourceLUT_SourceNum)
    {
        PQ_BIN_DBG(printf("[PQBin]invalid input type\n"));
        return PQ_BIN_IP_NULL;
    }

    u32Offset =  pPQBinHeader->u32SourceLUT_Pitch * u16PQPnlIdx + PQ_BIN_SOURCELUT_IP_NUM_OFFSET;
    u16SourceLUT_IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
    if (u16PQIPIdx >= u16SourceLUT_IPNum)
    {
        PQ_BIN_DBG(printf("[PQBin]invalid ip type\n"));
        return PQ_BIN_IP_NULL;
    }

    u32Offset =  pPQBinHeader->u32SourceLUT_Pitch * u16PQPnlIdx + PQ_BIN_SOURCELUT_OFFSET_OFFSET;
    u32SourceLUT_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

    u32SourceLUT_Addr += pPQBinHeader->u32BinStartAddress  + PQ_BIN_HEADER_LEN;
    u32SourceLUT_Data_Pitch = (MS_U32)u16SourceLUT_IPNum * PQ_BIN_SOURCELUT_DATA_LEN;
    u32SourceLUT_Offset =  u32SourceLUT_Data_Pitch * (MS_U32)u16PQSrcType +
                          (MS_U32)u16PQIPIdx * PQ_BIN_SOURCELUT_DATA_LEN;
#if PQ_BIN_SOURCELUT_DATA_LEN == 2
    u16TableIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32SourceLUT_Addr+u32SourceLUT_Offset),
                                          &u32SourceLUT_Offset);
#else
    u16TableIdx = (MS_U16)MDrv_PQBin_GetByteData(pInstance,(void *)(u32SourceLUT_Addr+u32SourceLUT_Offset),
                                          &u32SourceLUT_Offset);
#endif
    PQ_BIN_DBG(printf("[PQBin]TableId=%u\n",u16TableIdx));
    return  u16TableIdx;
}

void MDrv_PQBin_GetTable(void *pInstance,MS_U16 u16PnlIdx, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, MS_PQBin_IP_Table_Info *pTableInfo, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;

    if((u16PnlIdx >= PQ_BIN_MAX_PNL) || (u16PQIPIdx >= PQ_BIN_MAX_IP))
    {
        printf("[PQBin]:Out Of Range!! PnlIdx=%u, IPIdx=%u\n", u16PnlIdx, u16PQIPIdx);
        //MS_ASSERT(0);
        return;
    }
    _u16PQTabIdx[u16PnlIdx][u16PQIPIdx] = u16TabIdx;

    if (u16TabIdx != PQ_BIN_IP_NULL)
    {
        u32Addr   = pPQBinHeader->u32IP_Comm_Offset +
                    pPQBinHeader->u32BinStartAddress;

        if(u16TabIdx == PQ_BIN_IP_COMM)
        {
            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_TABLE_TYPE_OFFSET;
            pTableInfo->u16TableType = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_COMM_REG_NUM_OFFSET;
            pTableInfo->u16RegNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_COMMOFFSET_OFFSET;
            pTableInfo->u32TabOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
            pTableInfo->u32TabOffset += (pPQBinHeader->u32BinStartAddress + PQ_BIN_HEADER_LEN);

            pTableInfo->u16GroupNum = 1;
            pTableInfo->u16GroupIdx = 0;
        }
        else
        {
            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_TABLE_TYPE_OFFSET;
            pTableInfo->u16TableType = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_IP_REG_NUM_OFFSET;
            pTableInfo->u16RegNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_IP_GROUP_NUM_OFFSET;
            pTableInfo->u16GroupNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

            u32Offset = pPQBinHeader->u32IP_Comm_Pitch * (MS_U32)u16PQIPIdx + PQ_BIN_IP_COMM_IPOFFSET_OFFSET;
            pTableInfo->u32TabOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
            pTableInfo->u32TabOffset += (pPQBinHeader->u32BinStartAddress + PQ_BIN_HEADER_LEN);

            pTableInfo->u16GroupIdx = u16TabIdx;
        }
    }
    else
    {
        pTableInfo->u16GroupIdx  = 0;
        pTableInfo->u32TabOffset = PQ_BIN_ADDR_NULL;
        pTableInfo->u16RegNum    = 0;
        pTableInfo->u16TableType = 0;
        pTableInfo->u16GroupNum  = 0;
    }

    PQ_BIN_DBG(printf("[PQBin]: GetTable: Offset=%lx, RegNum=%u, GroupIdx=%u, GroupNum=%u, Type=%u \n",
        pTableInfo->u32TabOffset,
        pTableInfo->u16RegNum,
        pTableInfo->u16GroupIdx,
        pTableInfo->u16GroupNum,
        pTableInfo->u16TableType));
}

void MDrv_PQBin_LoadTableData(
	void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16TabIdx,
    MS_U16 u16PQIPIdx,
    MS_PQBin_Header_Info *pPQBinHeader,
    MS_U8 *pTable,
    MS_U16 u16TableSize)
{
    MS_PQBin_IP_Table_Info stTableInfo;
    MS_U8 __attribute__((__unused__))u8Mask = 0;
    MS_U8 __attribute__((__unused__))u8Addr = 0;
    MS_U8 u8Value = 0;
    MS_U8 __attribute__((__unused__))u8CurBank = 0xff;
    MS_PHYADDR u32Addr = 0;
    MS_U32 u32Offset = 0;
    MS_U16 i = 0;


    memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

    MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                        u16TabIdx,
                        u16PQIPIdx,
                        &stTableInfo,
                        pPQBinHeader);



    PQ_BIN_DUMP_DBG(printf("LoadTableData: \n"));
    if(stTableInfo.u16GroupIdx >= stTableInfo.u16GroupNum)
    {
        PQ_BIN_DBG(printf("[PQBin]IP_Info error: LoadTable Data\n"));
        return;
    }

    if(u16TableSize > stTableInfo.u16RegNum)
    {
        PQ_BIN_DBG(printf("[PQBin]IP_Info error: LoadTable Data is too big\n"));
        return;
    }
    u32Addr = stTableInfo.u32TabOffset;
    for(i=0; i<stTableInfo.u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)stTableInfo.u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  (MS_U32)stTableInfo.u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        pTable[i] = u8Value;
    }
}


void MDrv_PQBin_CloneTable(
	void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16TabIdx,
    MS_U16 u16PQMainIPIdx,
    MS_U16 u16PQSubIPIdx,
    MS_PQBin_Header_Info *pPQBinHeader)
{
    MS_PQBin_IP_Table_Info stTableMainInfo, stTableSubInfo;
    PQ_BIN_DBG_SRAMERROR(printf("---------MDrv_PQBin_LoadTable: u16TabIdx=%u, u16PQMainIPIdx=%u, u16PQSubIPIdx=%u\n",
        u16TabIdx, u16PQMainIPIdx, u16PQSubIPIdx));

	memset(&stTableMainInfo, 0, sizeof(MS_PQBin_IP_Table_Info));
    memset(&stTableSubInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

    MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                        u16TabIdx,
                        u16PQMainIPIdx,
                        &stTableMainInfo,
                        pPQBinHeader);

    MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                        u16TabIdx,
                        u16PQSubIPIdx,
                        &stTableSubInfo,
                        pPQBinHeader);

    MDrv_PQBin_DumpCloneTable(pInstance,&stTableMainInfo, &stTableSubInfo);
}

extern MS_PQBin_Header_Info stPQBinHeaderInfo[MAX_PQ_BIN_NUM];
#if (defined (UFO_XC_DS_PQ))
void _MDrv_PQBin_DS_GetTable(
	void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16TabIdx,
    MS_U16 u16PQIPIdx,
    MS_PQBin_Header_Info *pPQBinHeader,PQ_WIN eWindow)
{
    MS_PQBin_IP_Table_Info stTableInfo;
    PQ_BIN_DBG_SRAMERROR(printf("---------MDrv_PQBin_LoadTable: u16TabIdx=%u, u16PQIPIdx=%u\n", u16TabIdx, u16PQIPIdx));

	memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

    MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                        u16TabIdx,
                        u16PQIPIdx,
                        &stTableInfo,
                        pPQBinHeader);

    MDrv_PQBin_DS_DumpTable(pInstance,&stTableInfo);
}
void MDrv_PQBin_DS_GetTable(void* pInstance,PQ_WIN eWindow, MS_U16 u16TabIdx, MS_U16 u16PQIPIdx, PQ_ENGINE_TYPE ePQEngineType)
{
    switch(ePQEngineType)
    {
        case PQ_XC0_STD:
            _MDrv_PQBin_DS_GetTable(pInstance,MDrv_PQBin_GetPanelIdx(pInstance),
                         u16TabIdx,
                         u16PQIPIdx,
                         &stPQBinHeaderInfo[PQ_BIN_STD_MAIN],eWindow);
            break;
        case PQ_XC0_UFSC:
            _MDrv_PQBin_DS_GetTable(pInstance,MDrv_PQBin_GetPanelIdx_UFSC(pInstance),
                         u16TabIdx,
                         u16PQIPIdx,
                         &stPQBinHeaderInfo[PQ_BIN_UFSC],eWindow);
            break;
        default:
            break;

    }
}
#endif
void MDrv_PQBin_LoadTable(
	void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16TabIdx,
    MS_U16 u16PQIPIdx,
    MS_PQBin_Header_Info *pPQBinHeader,PQ_WIN eWindow)
{
    MS_PQBin_IP_Table_Info stTableInfo;
    PQ_BIN_DBG_SRAMERROR(printf("---------MDrv_PQBin_LoadTable: u16TabIdx=%u, u16PQIPIdx=%u\n", u16TabIdx, u16PQIPIdx));

    memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

    MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                        u16TabIdx,
                        u16PQIPIdx,
                        &stTableInfo,
                        pPQBinHeader);

    #ifdef UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(eWindow==PQ_SUB_WINDOW)
        MDrv_PQBin_DumpTable_Main2(pInstance,&stTableInfo);
    else
#endif
    {
        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);
    }
}

void MDrv_PQBin_LoadCommTable(void *pInstance,MS_U16 u16PnlIdx, MS_PQBin_Header_Info *pPQBinHeader)
{
    MS_U16 i;
    MS_PQBin_IP_Table_Info stTableInfo;

    _u16SRAM1Table = 0xFFFF;
    _u16SRAM2Table = 0xFFFF;
    _u16SRAM3Table = 0xFFFF;
    _u16SRAM4Table = 0xFFFF;
    _u16CSRAM1Table = 0xFFFF;
    _u16CSRAM2Table = 0xFFFF;
    _u16CSRAM3Table = 0xFFFF;
    _u16CSRAM4Table = 0xFFFF;
    _u16ICC_CRD_Table = 0xFFFF;
    _u16IHC_CRD_Table = 0xFFFF;
    _u16XVYCC_DE_GAMMA_SRAM_Table = 0xFFFF;
    _u16XVYCC_GAMMA_SRAM_Table = 0xFFFF;

    _u16SRAM1Table_main2 = 0xFFFF;
    _u16SRAM2Table_main2 = 0xFFFF;
    _u16SRAM3Table_main2 = 0xFFFF;
    _u16SRAM4Table_main2 = 0xFFFF;
    _u16CSRAM1Table_main2 = 0xFFFF;
    _u16CSRAM2Table_main2 = 0xFFFF;
    _u16CSRAM3Table_main2 = 0xFFFF;
    _u16CSRAM4Table_main2 = 0xFFFF;
    _u16ICC_CRD_Table_main2 = 0xFFFF;
    _u16IHC_CRD_Table_main2 = 0xFFFF;
    _u16XVYCC_DE_GAMMA_SRAM_Table_main2 = 0xFFFF;
    _u16XVYCC_GAMMA_SRAM_Table_main2 = 0xFFFF;

#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
    _u16UFSCSRAM1Table = 0xFFFF;
    _u16UFSCSRAM2Table = 0xFFFF;
    _u16UFSCSRAM3Table = 0xFFFF;
    _u16UFSCSRAM4Table = 0xFFFF;
    _u16UFSCCSRAM1Table = 0xFFFF;
    _u16UFSCCSRAM2Table = 0xFFFF;
    _u16UFSCCSRAM3Table = 0xFFFF;
    _u16UFSCCSRAM4Table = 0xFFFF;
    _u16UFSCICC_CRD_Table = 0xFFFF;
    _u16UFSCIHC_CRD_Table = 0xFFFF;
    //_u16UFSCXVYCC_DE_GAMMA_SRAM_Table = 0xFFFF;
    //_u16UFSCXVYCC_GAMMA_SRAM_Table = 0xFFFF;
#endif
    memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

    PQ_BIN_DBG(printf("[PQBin]LoadCommTable\n"));
    for (i=0; i< pPQBinHeader->u16IP_Comm_Num; i++)
    {
        PQ_BIN_DBG_SRAMERROR(printf("--------MDrv_PQBin_LoadCommTable: u16TabIdx=%u, u16PQIPIdx=%u\n", PQ_BIN_IP_COMM, i));
        MDrv_PQBin_GetTable(pInstance,u16PnlIdx,
                            PQ_BIN_IP_COMM,
                            i,
                            &stTableInfo,
                            pPQBinHeader);

        if((stTableInfo.u16TableType == PQ_TABTYPE_SRAM1) ||
           (stTableInfo.u16TableType == PQ_TABTYPE_SRAM2))
        {
           continue;
        }

        MDrv_PQBin_DumpTable(pInstance,&stTableInfo);
    }
}



void MDrv_PQBin_Parsing(void *pInstance,MS_PQBin_Header_Info *pPQBinHeader)
{
    MS_PHYADDR u32PQBin_Addr;
    MS_U32 u32Offset;
#if PQ_BIN_DBG_HEADER_DATA
    MS_U32 u32tmpoffset;
#endif
    MS_U8  i;

    u32Offset = 0;
    u32PQBin_Addr = pPQBinHeader->u32BinStartAddress;
    printf("BinStartaDDR ID%u=%tx\n", pPQBinHeader->u8BinID, (ptrdiff_t) u32PQBin_Addr);
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8Header[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8Version[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    pPQBinHeader->u32StartOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    pPQBinHeader->u32EndOffset   = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

    for(i=0; i< PQ_BIN_DUMMY; i++)
    {
        pPQBinHeader->u8Dummy[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    PQ_BIN_DBG_HEADER(printf("Header :%16s\n", pPQBinHeader->u8Header));
    PQ_BIN_DBG_HEADER(printf("Version:%16s\n", pPQBinHeader->u8Version));
    PQ_BIN_DBG_HEADER(printf("startADDR=%lx, EndAddr=%lx\n", pPQBinHeader->u32StartOffset, pPQBinHeader->u32EndOffset));
    PQ_BIN_DBG_HEADER(printf("Dummy :%8s\n", pPQBinHeader->u8Dummy));
    //-------------------------------------------------------------------------------------------
    // GRule
    //
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8GRuleHader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    pPQBinHeader->u16GRule_RuleNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    pPQBinHeader->u16GRule_PnlNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);


    PQ_BIN_DBG_HEADER(printf("GRuleHeader:%16s\n", pPQBinHeader->u8GRuleHader));
    PQ_BIN_DBG_HEADER(printf("GRule: RuleNum=%u, PnlNum=%u \n", pPQBinHeader->u16GRule_RuleNum, pPQBinHeader->u16GRule_PnlNum));

    pPQBinHeader->u32GRule_Offset = u32Offset;
    pPQBinHeader->u32GRule_Pitch = PQ_BIN_GRULE_INFO_SIZE;
    u32Offset += pPQBinHeader->u32GRule_Pitch * (MS_U32)pPQBinHeader->u16GRule_RuleNum;


    pPQBinHeader->u32GRule_Lvl_Offset = u32Offset;
    pPQBinHeader->u32GRule_Lvl_Pitch = PQ_BIN_GRULE_LEVEL_INFO_SIZE;
    u32Offset += pPQBinHeader->u32GRule_Lvl_Pitch * (MS_U32)pPQBinHeader->u16GRule_RuleNum * (MS_U32)pPQBinHeader->u16GRule_PnlNum;

    PQ_BIN_DBG_HEADER(printf("GRule Rule : offset=%lx, pitch=%lu\n",  pPQBinHeader->u32GRule_Offset,  pPQBinHeader->u32GRule_Pitch));
    PQ_BIN_DBG_HEADER(printf("GRule Level: offset=%lx, pitch=%lu \n", pPQBinHeader->u32GRule_Lvl_Offset, pPQBinHeader->u32GRule_Lvl_Pitch));
    PQ_BIN_DBG_HEADER(printf("XRule Start:%lx\n", u32Offset));

    //-------------------------------------------------------------------------------------------
    // XRule
    //
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8XRuleHader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }
    pPQBinHeader->u16XRuleNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

    PQ_BIN_DBG_HEADER(printf("XRuleHeader:%16s\n", pPQBinHeader->u8XRuleHader));
    PQ_BIN_DBG_HEADER(printf("XRule: RuleNum=%u\n", pPQBinHeader->u16XRuleNum));

    pPQBinHeader->u32XRule_Offset = u32Offset;
    pPQBinHeader->u32XRUle_Pitch  = PQ_BIN_XRULE_INFO_SIZE;
    u32Offset += pPQBinHeader->u32XRUle_Pitch * (MS_U32)pPQBinHeader->u16XRuleNum;

    PQ_BIN_DBG_HEADER(printf("XRule: offset=%lx, pitch=%lu\n",pPQBinHeader->u32XRule_Offset, pPQBinHeader->u32XRUle_Pitch));
    PQ_BIN_DBG_HEADER(printf("SkipRule Start:%lx\n", u32Offset));

    //-------------------------------------------------------------------------------------------
    // SkipRule
    //
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8SkipRuleHader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    pPQBinHeader->u16SkipRule_IPNum  = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    pPQBinHeader->u32SkipRule_Offset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

    PQ_BIN_DBG_HEADER(printf("SkipRuleHeader:%16s IpNum=%u, Offset=%lx\n", pPQBinHeader->u8SkipRuleHader, pPQBinHeader->u16SkipRule_IPNum, pPQBinHeader->u32SkipRule_Offset));
    PQ_BIN_DBG_HEADER(printf("IP_Comm Start:%lx\n", u32Offset));

    //-------------------------------------------------------------------------------------------
    // IP & Common
    //
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8IP_Comm_Header[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    pPQBinHeader->u16IP_Comm_Num = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

    PQ_BIN_DBG_HEADER(printf("IP_CommHeader:%16s\n", pPQBinHeader->u8IP_Comm_Header));
    PQ_BIN_DBG_HEADER(printf("IP_Comm, Num=%u\n", pPQBinHeader->u16IP_Comm_Num));

    pPQBinHeader->u32IP_Comm_Offset = u32Offset;
    pPQBinHeader->u32IP_Comm_Pitch = PQ_BIN_IP_COMM_INFO_SIZE;
    u32Offset += pPQBinHeader->u32IP_Comm_Pitch  * (MS_U32)pPQBinHeader->u16IP_Comm_Num;

    PQ_BIN_DBG_HEADER(printf("IP_Comm, Offset=%lx, pitch=%lu\n", pPQBinHeader->u32IP_Comm_Offset, pPQBinHeader->u32IP_Comm_Pitch));
    PQ_BIN_DBG_HEADER(printf("SourceLUT Start:%lx\n", u32Offset));
    //-------------------------------------------------------------------------------------------
    // SourceLUT
    //
    for(i=0; i< PQ_BIN_HEADER_LEN; i++)
    {
        pPQBinHeader->u8SourceLUT_Header[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
    }

    pPQBinHeader->u16SourceLUT_PnlNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

    PQ_BIN_DBG_HEADER(printf("SourceLUTHeader:%16s\n", pPQBinHeader->u8SourceLUT_Header));
    PQ_BIN_DBG_HEADER(printf("SourceLUT, PnlNum=%u\n", pPQBinHeader->u16SourceLUT_PnlNum));

    pPQBinHeader->u32SourceLUT_Offset = u32Offset;
    pPQBinHeader->u32SourceLUT_Pitch = PQ_BIN_SOURCELUT_INFO_SIZE;
    u32Offset += pPQBinHeader->u32SourceLUT_Pitch * (MS_U32)pPQBinHeader->u16SourceLUT_PnlNum;

    PQ_BIN_DBG_HEADER(printf("SourceLUT: offset=%lx, pitch=%lu\n", pPQBinHeader->u32SourceLUT_Offset, pPQBinHeader->u32SourceLUT_Pitch));


#if PQ_BIN_DBG_HEADER_DATA
    // GRule
    u32tmpoffset = pPQBinHeader->u32GRule_Offset;
    for(i=0; i<pPQBinHeader->u16GRule_RuleNum; i++)
    {
        MS_PQBin_GRule_Info stGRuleInfo;

        stGRuleInfo.u16IPNum      = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stGRuleInfo.u16SourceNum  = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stGRuleInfo.u16GroupNum   = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stGRuleInfo.u32IPOffset   = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stGRuleInfo.u32RuleOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);

        printf("GRule %u, IPNum=%u, SourceNum=%u, GroupNum=%u, IPOffset=%lx, RuleOffset=%lx\n",
            i,
            stGRuleInfo.u16IPNum,
            stGRuleInfo.u16SourceNum,
            stGRuleInfo.u16GroupNum,
            stGRuleInfo.u32IPOffset,
            stGRuleInfo.u32RuleOffset);
    }

    // GRule Level
    for(i=0; i<pPQBinHeader->u16GRule_RuleNum*pPQBinHeader->u16GRule_PnlNum; i++)
    {
        MS_PQBin_GRule_Level_Info stGRuleLvlInfo;

        stGRuleLvlInfo.u16LvlNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stGRuleLvlInfo.u32Offset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);

        printf("GRule_Lvl %u, u16LvlNum=%u, u32Offset=%lx \n",
            i, stGRuleLvlInfo.u16LvlNum, stGRuleLvlInfo.u32Offset);
    }

    // XRule
    u32tmpoffset = pPQBinHeader->u32XRule_Offset;
    for(i=0; i<pPQBinHeader->u16XRuleNum; i++)
    {
        MS_PQBin_XRule_Info stXRuleInfo;

        stXRuleInfo.u16IPNum       = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stXRuleInfo.u16GroupNum    = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stXRuleInfo.u32IPOffset    = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stXRuleInfo.u32GroupOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);

        printf("XRule %u, IPNum=%u, GroupNum=%u, IPOffset=%lx, GroupOffset=%lx\n",
            i, stXRuleInfo.u16IPNum, stXRuleInfo.u16GroupNum, stXRuleInfo.u32IPOffset, stXRuleInfo.u32GroupOffset);
    }

    // IP & Common
    u32tmpoffset = pPQBinHeader->u32IP_Comm_Offset;
    for(i=0; i<pPQBinHeader->u16IP_Comm_Num; i++)
    {
        MS_PQBin_IP_Common_Info stIPCommInfo;

        stIPCommInfo.u16CommRegNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stIPCommInfo.u16IPRegNum   = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stIPCommInfo.u16IPGroupNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stIPCommInfo.u32CommOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stIPCommInfo.u32IPOffset   = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);

        printf("IP_Comm %u, CommRegNum=%u, IPRegNum=%u, IPGroupNum=%u, CommOffset=%lx, IPOffset=%lx\n",
            i,
            stIPCommInfo.u16CommRegNum,
            stIPCommInfo.u16IPRegNum,
            stIPCommInfo.u16IPGroupNum,
            stIPCommInfo.u32CommOffset,
            stIPCommInfo.u32IPOffset);
    }

    // SourceLUT
    u32tmpoffset = pPQBinHeader->u32SourceLUT_Offset;
    for(i=0; i<pPQBinHeader->u16SourceLUT_PnlNum; i++)
        {
        MS_PQBin_SourceLUT_Info stSourceLUTInfo;

        stSourceLUTInfo.u16SourceNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stSourceLUTInfo.u16IPNum     = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);
        stSourceLUTInfo.u32Offset    = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32tmpoffset), &u32tmpoffset);

        printf("SourceLUT %u, SourceNum=%u, IPNum=%u, Offset=%lx\n",
            i,
            stSourceLUTInfo.u16SourceNum,
            stSourceLUTInfo.u16IPNum,
            stSourceLUTInfo.u32Offset);

    }
#endif

}


MS_U16 MDrv_PQBin_GetSkipRule(void *pInstance,MS_U16 u16PQIPIdx, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16SkipRet;

    u32Addr = pPQBinHeader->u32SkipRule_Offset +
              pPQBinHeader->u32BinStartAddress +
              PQ_BIN_SKIPRULE_HEADER_LEN;

    u32Offset = u16PQIPIdx * 2;
    u16SkipRet = MDrv_PQBin_Get2ByteData(pInstance,(void *)((void *)(u32Addr+u32Offset)) , &u32Offset);
    return u16SkipRet;
}

MS_BOOL MDrv_PQBin_Enable8LBMode(void *pInstance,MS_BOOL bEnable)
{
    if(_bMLoadEn)
    {
        if(bEnable)
        {
            PQ_REG_MLOAD_WRITE_CMD(REG_SC_BK2A_02_H, BIT(7), BIT(7));
        }
        else
        {
            PQ_REG_MLOAD_WRITE_CMD(REG_SC_BK2A_02_H, 0x00, BIT(7));
        }
    }
    else
    {
        if(bEnable)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, BIT(15), BIT(15));
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0000, BIT(15));
        }
    }

    return TRUE;
}

void MDrv_PQBin_LoadTableBySrcType(
    void *pInstance,
    MS_U16 u16PQSrcType,
    MS_U16 u16PQIPIdx,
    MS_U16 u16PQPnlIdx,
    MS_PQBin_Header_Info *pPQBinHeader,
    PQ_WIN eWindow)
{
    MS_U16 QMIPtype_size,i;
    MS_U16 u16TabIdx;
    MS_PQBin_IP_Table_Info stTableInfo;

    memset(&stTableInfo, 0, sizeof(MS_PQBin_IP_Table_Info));

#if (PQ_ENABLE_PIP)
    MS_U16 u16SFilter = 0;
#endif

    if (u16PQIPIdx==PQ_BIN_IP_ALL)
    {
        QMIPtype_size= MDrv_PQBin_GetIPNum(pInstance,pPQBinHeader);
        u16PQIPIdx=0;
    }
    else
    {
        QMIPtype_size=1;
    }


    for(i=0; i<QMIPtype_size; i++, u16PQIPIdx++)
    {
        if ( MDrv_PQBin_GetSkipRule(pInstance,u16PQIPIdx, pPQBinHeader))
        {
            PQ_BIN_DBG(printf("skip ip idx:%u\n", u16PQIPIdx));
            continue;
        }

        if(pPQBinHeader->u8BinID < PQ_BIN_CUSTOMER_MAIN )
        {
            if(PQ_MAIN_WINDOW == eWindow)
            {
                //if now it's going to dump HSD_Y and HSD_C, just skip it.
                if((u16PQIPIdx == PQ_IP_HSD_C_Main) || (u16PQIPIdx == PQ_IP_HSD_Y_Main))
                {
                    PQ_BIN_DBG(printf("skip ip idx:%u because of customer scaling case\n", u16PQIPIdx));
                    continue;
                }
            }
#if (PQ_ENABLE_PIP)
            else
            {
                //if now it's going to dump HSD_Y and HSD_C, just skip it.
                if((u16PQIPIdx == PQ_IP_HSD_C_Sub) || (u16PQIPIdx == PQ_IP_HSD_Y_Sub))
                {
                    PQ_BIN_DBG(printf("skip ip idx:%u because of customer scaling case\n", u16PQIPIdx));
                    continue;
                }
            }
#endif
        }

        u16TabIdx = MDrv_PQBin_GetTableIndex(pInstance,u16PQSrcType,
                                             u16PQIPIdx,
                                             u16PQPnlIdx,
                                             pPQBinHeader);

        PQ_BIN_DBG(printf("[PQ]SrcType=%u, IPIdx=%u, TabIdx=%u\n",
            u16PQSrcType, u16PQIPIdx, u16TabIdx));
        PQ_BIN_DBG_SRAMERROR(printf("----------MDrv_PQBin_LoadTableBySrcType:u16TabIdx=%u, u16PQIPIdx=%u\n", u16TabIdx, u16PQIPIdx));

        MDrv_PQBin_GetTable(pInstance,u16PQPnlIdx,
                            u16TabIdx,
                            u16PQIPIdx,
                            &stTableInfo,
                            pPQBinHeader);

#if (PQ_ENABLE_PIP)
        //Sub SRAM3,4 dumping will make main window flash, so set it to 0x0303 to skip sram affection.
        if(((u16PQIPIdx == PQ_IP_SRAM3_Sub) || (u16PQIPIdx == PQ_IP_SRAM4_Sub) ) && (PQ_SUB_WINDOW == eWindow)
        &&((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())))
        {
            u16SFilter = MApi_XC_R2BYTE(REG_SC_BK23_0B_L);
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, 0x0303 , 0xFFFF );
        }
#endif
#ifdef UFO_XC_PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(eWindow==PQ_SUB_WINDOW)
            MDrv_PQBin_DumpTable_Main2(pInstance,&stTableInfo);
        else
#endif
        {
            MDrv_PQBin_DumpTable(pInstance,&stTableInfo);
        }
#if (PQ_ENABLE_PIP)
        if(((u16PQIPIdx == PQ_IP_SRAM3_Sub) || (u16PQIPIdx == PQ_IP_SRAM4_Sub) ) && (PQ_SUB_WINDOW == eWindow)
        &&((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MApi_XC_Get_3D_Output_Mode())
        || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MApi_XC_Get_3D_Output_Mode())))
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK23_0B_L, u16SFilter , 0xFFFF );
        }
#endif
    }
}


MS_U16 MDrv_PQBin_GetXRuleTableIndex(void *pInstance,MS_U16 u16XRuleType, MS_U16 u16XRuleIdx, MS_U16 u16XRuleIP, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_PHYADDR u32Rule_Addr;
    MS_U32 u32Rule_Offset;
    MS_U16 u16IPNum;
    MS_U16 u16TableIdx;

    u32Addr = pPQBinHeader->u32XRule_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32XRUle_Pitch * u16XRuleType + PQ_BIN_XRULE_IP_NUM_OFFSET;
    u16IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    u32Offset = pPQBinHeader->u32XRUle_Pitch * u16XRuleType + PQ_BIN_XRULE_GROUPOFFSET_OFFSET;
    u32Rule_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                   pPQBinHeader->u32BinStartAddress;


    u32Rule_Offset = (u16IPNum * u16XRuleIdx + u16XRuleIP) * PQ_BIN_SOURCELUT_DATA_LEN;
#if PQ_BIN_SOURCELUT_DATA_LEN == 2
    u16TableIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Rule_Addr+u32Rule_Offset), &u32Rule_Offset);
#else
    u16TableIdx = (MS_U16)MDrv_PQBin_GetByteData(pInstance,(void *)(u32Rule_Addr+u32Rule_Offset), &u32Rule_Offset);
#endif
    PQ_BIN_XRULE_DBG(printf("[PQBin_XRule]: TableIdx=%u\n", u16TableIdx));
    return u16TableIdx;

}

MS_U16 MDrv_PQBin_GetXRuleIPIndex(void *pInstance,MS_U16 u16XRuleType, MS_U16 u16XRuleIP, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_PHYADDR u32IP_Addr;
    MS_U32 u32IP_Offset;
    MS_U16 u16IPIdx;

    u32Addr = pPQBinHeader->u32XRule_Offset + pPQBinHeader->u32BinStartAddress;
    u32Offset = pPQBinHeader->u32XRUle_Pitch * u16XRuleType + PQ_BIN_XRULE_IPOFFSET_OFFSET;

    u32IP_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQBinHeader->u32BinStartAddress + PQ_BIN_XRULE_HEADER_LEN;

    u32IP_Offset =  u16XRuleIP * 2;

    u16IPIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32IP_Addr+u32IP_Offset), &u32IP_Offset);

    PQ_BIN_XRULE_DBG(printf("[PQBin_XRule]: IPIdx=%u\n", u16IPIdx));
    return u16IPIdx;

}

MS_U16 MDrv_PQBin_GetXRuleIPNum(void *pInstance,MS_U16 u16XRuleType, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16IPNum;

    u32Addr = pPQBinHeader->u32XRule_Offset + pPQBinHeader->u32BinStartAddress;
    u32Offset = pPQBinHeader->u32XRUle_Pitch * u16XRuleType + PQ_BIN_XRULE_IP_NUM_OFFSET;
    u16IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    PQ_BIN_XRULE_DBG(printf("[PQBin_XRule]: IPNum=%u\n", u16IPNum));
    return u16IPNum;
}



MS_U16 MDrv_PQBin_GetGRule_LevelIndex(
	void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16GRuleType,
    MS_U16 u16GRuleLvlIndex,
    const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr, u32Lvl_Addr;
    MS_U32 u32Offset;
    MS_U16 u16LvlNum;
    MS_U16 u16LvlIdx;

    if(u16PnlIdx >= pPQBinHeader->u16GRule_PnlNum)
    {
        MS_ASSERT(0);
        printf("GRue: Pnl idx out of rage =%u\n", u16PnlIdx);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Lvl_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32GRule_Lvl_Pitch * (MS_U32) (u16PnlIdx + pPQBinHeader->u16GRule_PnlNum * u16GRuleType) +
                PQ_BIN_GRULE_LEVEL_LVL_NUM_OFFSET;
    u16LvlNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    if(u16GRuleLvlIndex >= u16LvlNum)
    {
        MS_ASSERT(0);
        printf("GRule: lvl idx out of rage =%u\n", u16GRuleLvlIndex);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Lvl_Offset + pPQBinHeader->u32BinStartAddress;
    u32Offset = pPQBinHeader->u32GRule_Lvl_Pitch * (MS_U32) (u16PnlIdx + pPQBinHeader->u16GRule_PnlNum * u16GRuleType) +
                PQ_BIN_GRULE_LEVEL_OFFSET_OFFSET;

    u32Lvl_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    u32Addr = u32Lvl_Addr + pPQBinHeader->u32BinStartAddress;
    u32Offset = PQ_BIN_GRULE_HEADER_LEN + u16GRuleLvlIndex * 2;

    u16LvlIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    PQ_BIN_GRULE_DBG(printf("[PQBin_GRule: LvlIdx=%u\n", u16LvlIdx));
    return u16LvlIdx;
}


MS_U16 MDrv_PQBin_GetGRule_IPIndex(
    void *pInstance,
    MS_U16 u16GRuleType,
    MS_U16 u16GRuleIPIndex,
    const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr, u32IP_Addr;
    MS_U32 u32IP_Offset, u32Offset;
    MS_U16 u16IPIdx;

    if(u16GRuleType >= pPQBinHeader->u16GRule_RuleNum)
    {
        MS_ASSERT(0);
        printf("GRule: ruleid out of range=%u\n", u16GRuleType);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Offset + pPQBinHeader->u32BinStartAddress;
    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType +
                PQ_BIN_GRULE_IPOFFSET_OFFSET;

    u32IP_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQBinHeader->u32BinStartAddress + PQ_BIN_GRULE_HEADER_LEN;
    u32IP_Offset = u16GRuleIPIndex * 2;

    u16IPIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32IP_Addr+u32IP_Offset), &u32IP_Offset);

    PQ_BIN_GRULE_DBG(printf("[PQBin_GRule: IPIdx=%u\n", u16IPIdx));
    return u16IPIdx;
}

MS_U16 MDrv_PQBin_GetGRule_TableIndex(
    void *pInstance,
    MS_U16 u16GRuleType,
    MS_U16 u16PQSrcType,
    MS_U16 u16PQ_NRIdx,
    MS_U16 u16GRuleIPIndex,
    const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr, u32Rule_Addr;
    MS_U32 u32Offset, u32Rule_Offset;
    MS_U16 u16GroupNum, u16IPNum, u16SrcNum;
    MS_U16 u16TableIdx;

    if(u16GRuleType >= pPQBinHeader->u16GRule_RuleNum)
    {
        MS_ASSERT(0);
        printf("GRule: ruleid out of range=%u\n", u16GRuleType);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_GROUP_NUM_OFFSET;
    u16GroupNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_IP_NUM_OFFSET;
    u16IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_SOURCE_NUM_OFFSET;
    u16SrcNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);



    if(u16PQSrcType >= u16SrcNum)
    {
        MS_ASSERT(0);
        printf("GRule: SrcIdx out of range =%u\n", u16PQSrcType);
        return PQ_BIN_IP_NULL;
    }

    if(u16PQ_NRIdx >= u16GroupNum)
    {
        MS_ASSERT(0);
        printf("GRule: GroupIdx out of range =%u\n", u16PQ_NRIdx);
        return PQ_BIN_IP_NULL;
    }

    if(u16GRuleIPIndex >= u16IPNum)
    {
        MS_ASSERT(0);
        printf("GRule: IPIdx out of range =%u\n", u16GRuleIPIndex);
        return PQ_BIN_IP_NULL;
    }


    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_RULEOFFSET_OFFSET;
    u32Rule_Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                   pPQBinHeader->u32BinStartAddress + PQ_BIN_GRULE_HEADER_LEN;


    u32Rule_Offset = (u16IPNum * u16GroupNum * u16PQSrcType  +
                      u16IPNum * u16PQ_NRIdx +
                      u16GRuleIPIndex) * PQ_BIN_SOURCELUT_DATA_LEN;
#if PQ_BIN_SOURCELUT_DATA_LEN == 2
    u16TableIdx = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Rule_Addr+u32Rule_Offset), &u32Rule_Offset);
#else
    u16TableIdx = (MS_U16)MDrv_PQBin_GetByteData(pInstance,(void *)(u32Rule_Addr+u32Rule_Offset), &u32Rule_Offset);
#endif
    PQ_BIN_GRULE_DBG(printf("[PQBin_GRule: TabIdx=%u\n", u16TableIdx));
    return u16TableIdx;
}

MS_U16 MDrv_PQBin_GetGRule_LvlNum(
    void *pInstance,
    MS_U16 u16PnlIdx,
    MS_U16 u16GRuleType,
    const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16LvlNum;

    if(u16PnlIdx >= pPQBinHeader->u16GRule_PnlNum)
    {
        //MS_ASSERT(0);
        printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16PnlIdx, pPQBinHeader->u16GRule_PnlNum);
        return PQ_BIN_IP_NULL;
    }

    if(u16GRuleType >= pPQBinHeader->u16GRule_RuleNum)
    {
        printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, pPQBinHeader->u16GRule_RuleNum);
       // MS_ASSERT(0);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Lvl_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32GRule_Lvl_Pitch * (MS_U32) (u16PnlIdx + pPQBinHeader->u16GRule_PnlNum * u16GRuleType) +
                PQ_BIN_GRULE_LEVEL_LVL_NUM_OFFSET;
    u16LvlNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    return u16LvlNum;
}
MS_U16 MDrv_PQBin_GetGRule_IPNum(void *pInstance,MS_U16 u16GRuleType, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16IPNum;

    if(u16GRuleType >= pPQBinHeader->u16GRule_RuleNum)
    {
        printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, pPQBinHeader->u16GRule_RuleNum);
       // MS_ASSERT(0);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_IP_NUM_OFFSET;
    u16IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    return u16IPNum;
}

MS_U16 MDrv_PQBin_GetGRule_GroupNum(void *pInstance,MS_U16 u16GRuleType, const MS_PQBin_Header_Info* const pPQBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 u16GroupNum;

    if(u16GRuleType >= pPQBinHeader->u16GRule_RuleNum)
    {
        printf("=ERROR!!!=%s, %d, =%d, out of %d, =\n", __FUNCTION__, __LINE__, u16GRuleType, pPQBinHeader->u16GRule_RuleNum);
       // MS_ASSERT(0);
        return PQ_BIN_IP_NULL;
    }

    u32Addr = pPQBinHeader->u32GRule_Offset + pPQBinHeader->u32BinStartAddress;

    u32Offset = pPQBinHeader->u32GRule_Pitch * u16GRuleType + PQ_BIN_GRULE_GROUP_NUM_OFFSET;
    u16GroupNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    return u16GroupNum;
}

void MDrv_PQBin_CheckCommTable(void *pInstance,MS_U16 u16PnlIdx, MS_PQBin_Header_Info * pPQBinHeader)
{
#if (PQ_ENABLE_CHECK == 1)
    _u8PQBinfunction = PQ_FUNC_CHK_REG;
    MDrv_PQBin_LoadCommTable(pInstance,u16PnlIdx, pPQBinHeader);
    _u8PQBinfunction = PQ_FUNC_DUMP_REG;
#else
    UNUSED(u16PnlIdx);
    UNUSED(pPQBinHeader);
#endif
}

void MDrv_PQBin_CheckTableBySrcType(
    void *pInstance,
    MS_U16 u16PQSrcType,
    MS_U16 u16PQIPIdx,
    MS_U16 u16PQPnlIdx,
    MS_PQBin_Header_Info * pPQBinHeader,
    PQ_WIN eWindow)
{
#if (PQ_ENABLE_CHECK == 1)
    _u8PQBinfunction = PQ_FUNC_CHK_REG;
    MDrv_PQBin_LoadTableBySrcType(pInstance,u16PQSrcType,
                                  u16PQIPIdx,
                                  u16PQPnlIdx,
                                  pPQBinHeader,
                                  eWindow);
    _u8PQBinfunction = PQ_FUNC_DUMP_REG;
#else
    UNUSED(u16PQSrcType);
    UNUSED(u16PQIPIdx);
    UNUSED(u16PQPnlIdx);
    UNUSED(pPQBinHeader);
    UNUSED(eWindow);
#endif
}

void MDrv_PQBin_SetDisplayType(void *pInstance,PQ_BIN_DISPLAY_TYPE enDisplayType)
{
    _gPQBinDisplayType = enDisplayType;
}

PQ_BIN_DISPLAY_TYPE MDrv_PQBin_GetDisplayType(void *pInstance)
{
    return (PQ_BIN_DISPLAY_TYPE)_gPQBinDisplayType;
}

void MDrv_PQBin_SetPanelID(void *pInstance,MS_U8 u8PnlIDx)
{
    _gPQBinPnlIdx = u8PnlIDx;
}

MS_U8 MDrv_PQBin_GetPanelIdx(void *pInstance)
{
#if (PQ_ENABLE_PIP)
    return _gPQBinPnlIdx * PQ_BIN_DISPLAY_NUM + _gPQBinDisplayType;
#else
    return _gPQBinPnlIdx * 1 + _gPQBinDisplayType;
#endif
}

MS_U8 MDrv_PQBin_GetPanelIdx_UFSC(void *pInstance)
{
    return _gPQBinPnlIdx;
}

void MDrv_PQBin_Set_MLoadEn(void *pInstance,MS_BOOL bEn)
{
    if(bEn == DISABLE)
    {
        //if spread reg, no need to use mutex, but it's ok to use mutex
        //   (because it's not MApi_XC_W2BYTE(), which has mutex already)
        //if not spread reg, must use mutex to protect MLoad_trigger func.
        SC_BK_STORE_MUTEX;

        if(_u16MLoadCmdCnt)
        {
        #if(ENABLE_PQ_MLOAD)
            MApi_XC_MLoad_WriteCmds_And_Fire(
                &_u32MLoadCmd[0], &_u16MLoadVal[0], &_u16MLoadMsk[0], _u16MLoadCmdCnt);
        #endif

            _u16MLoadCmdCnt = 0;
        }

        SC_BK_RESTORE_MUTEX;
    }

    _bMLoadEn = bEn;
}

//------------------------------------------------------------------------------
// Text Bin function
//------------------------------------------------------------------------------
void MDrv_PQTextBin_Parsing(void *pInstance,MS_PQTextBin_Header_Info *pPQTextBinHeader)
{
        MS_PHYADDR u32PQBin_Addr;
        MS_U32 u32Offset;
#if PQ_BIN_DBG_HEADER_DATA
        MS_U32 u32tmpoffset;
#endif
        MS_U8  i;

        u32Offset = 0;
        u32PQBin_Addr = pPQTextBinHeader->u32BinStartAddress;
        printf("TextBinStartaDDR ID%u=%tx\n", pPQTextBinHeader->u8BinID, (ptrdiff_t) u32PQBin_Addr);
        for(i=0; i< PQ_TEXT_BIN_HEADER_LEN; i++)
        {
            pPQTextBinHeader->u8Header[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }

        for(i=0; i< PQ_TEXT_BIN_HEADER_LEN; i++)
        {
            pPQTextBinHeader->u8Version[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }


        pPQTextBinHeader->u32StartOffset = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        pPQTextBinHeader->u32EndOffset   = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

        for(i=0; i< PQ_TEXT_BIN_DUMMY; i++)
        {
            pPQTextBinHeader->u8Dummy[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }

        PQ_TEXT_BIN_DBG_HEADER(printf("Header :%16s\n", pPQTextBinHeader->u8Header));
        PQ_TEXT_BIN_DBG_HEADER(printf("Version:%16s\n", pPQTextBinHeader->u8Version));
        PQ_TEXT_BIN_DBG_HEADER(printf("startADDR=%lx, pPQTextBinHeader=%lx\n", pPQTextBinHeader->u32StartOffset, pPQTextBinHeader->u32EndOffset));
        PQ_TEXT_BIN_DBG_HEADER(printf("Dummy :%8s\n", pPQTextBinHeader->u8Dummy));


        // SrcType
        for(i=0; i< PQ_TEXT_BIN_HEADER_LEN; i++)
        {
            pPQTextBinHeader->u8SrcTypeHeader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }
        pPQTextBinHeader->u16SrcNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

        PQ_TEXT_BIN_DBG_HEADER(printf("SrcTypeHeader:%16s\n", pPQTextBinHeader->u8SrcTypeHeader));
        PQ_TEXT_BIN_DBG_HEADER(printf("SrcType: RuleNum=%u\n", pPQTextBinHeader->u16SrcNum));

        pPQTextBinHeader->u32SrcType_Offset = u32Offset;
        pPQTextBinHeader->u32SrcType_Pitch  = PQ_TEXT_BIN_SRCTYPE_INFO_SIZE;
        u32Offset += pPQTextBinHeader->u32SrcType_Pitch * (MS_U32)pPQTextBinHeader->u16SrcNum;

        PQ_TEXT_BIN_DBG_HEADER(printf("SrcType: offset=%lx, pitch=%lu\n",pPQTextBinHeader->u32SrcType_Offset, pPQTextBinHeader->u32SrcType_Pitch));
        PQ_TEXT_BIN_DBG_HEADER(printf("IPName Start:%lx\n", u32Offset));

        // IP Name
        for(i=0; i< PQ_TEXT_BIN_HEADER_LEN; i++)
        {
            pPQTextBinHeader->u8IPNameHeader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }
        pPQTextBinHeader->u16IPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

        PQ_TEXT_BIN_DBG_HEADER(printf("IPNameHeader:%16s\n", pPQTextBinHeader->u8IPNameHeader));
        PQ_TEXT_BIN_DBG_HEADER(printf("IPName: IPNum=%u\n", pPQTextBinHeader->u16IPNum));

        pPQTextBinHeader->u32IPName_Offset = u32Offset;
        pPQTextBinHeader->u32IPName_Pitch  = PQ_TEXT_BIN_IPNAME_INFO_SIZE;
        u32Offset += pPQTextBinHeader->u32IPName_Pitch * (MS_U32)pPQTextBinHeader->u16IPNum;

        PQ_TEXT_BIN_DBG_HEADER(printf("IPName: offset=%lx, pitch=%lu\n",pPQTextBinHeader->u32IPName_Offset, pPQTextBinHeader->u32IPName_Pitch));
        PQ_TEXT_BIN_DBG_HEADER(printf("GroupName Start:%lx\n", u32Offset));

        // Group Name
        for(i=0; i< PQ_TEXT_BIN_HEADER_LEN; i++)
        {
            pPQTextBinHeader->u8GroupNameHeader[i] = MDrv_PQBin_GetByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);
        }
        pPQTextBinHeader->u16GroupIPNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32PQBin_Addr+u32Offset), &u32Offset);

        PQ_TEXT_BIN_DBG_HEADER(printf("GroupName:%16s\n", pPQTextBinHeader->u8GroupNameHeader));
        PQ_TEXT_BIN_DBG_HEADER(printf("GroupName: IPNum=%u\n", pPQTextBinHeader->u16GroupIPNum));

        pPQTextBinHeader->u32IP_Group_Offset = u32Offset;
        pPQTextBinHeader->u32IP_Group_pitch = PQ_TEXT_BIN_GROUPNAME_INFO_SIZE;
        u32Offset += pPQTextBinHeader->u32IP_Group_pitch * (MS_U32)pPQTextBinHeader->u16GroupIPNum;

        PQ_TEXT_BIN_DBG_HEADER(printf("IPName: offset=%lx, pitch=%lu\n",pPQTextBinHeader->u32IP_Group_Offset, pPQTextBinHeader->u32IP_Group_pitch));
        PQ_TEXT_BIN_DBG_HEADER(printf("Group Start:%lx\n", u32Offset));

}

MS_U32 MDrv_PQTextBin_GetSrcType(void *pInstance,MS_U16 u16SrcIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U32 u32DstAddr;

    if(u16SrcIdx >= pPQTextBinHeader->u16SrcNum)
    {
        MS_ASSERT(0);
        printf("[PQBinText] GetSrcType:: SrcIdx Out of range %u\n", u16SrcIdx);
        u16SrcIdx = 0;
    }
    u32Addr   = pPQTextBinHeader->u32SrcType_Offset +  pPQTextBinHeader->u32BinStartAddress;
    u32Offset = pPQTextBinHeader->u32SrcType_Pitch * u16SrcIdx +
                PQ_TEXT_BIN_SRCTYPE_OFFSET_OFFSET;

    u32DstAddr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQTextBinHeader->u32BinStartAddress;

    PQ_TEXT_BIN_DBG(printf("PQTextBin SrcType: Addr%lx, SrcIdx%u\n", u32DstAddr, u16SrcIdx));
    return u32DstAddr;
}

MS_U32 MDrv_PQTextBin_GetIPName(void *pInstance,MS_U16 u16PQIPIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U32 u32DstAddr;

    if(u16PQIPIdx >= pPQTextBinHeader->u16IPNum)
    {
        MS_ASSERT(0);
        printf("[PQBinText] GetIPName:: IPIdx Out of range %u\n", u16PQIPIdx);
        u16PQIPIdx = 0;
    }
    u32Addr   = pPQTextBinHeader->u32IPName_Offset + pPQTextBinHeader->u32BinStartAddress;
    u32Offset = pPQTextBinHeader->u32IPName_Pitch * u16PQIPIdx +
                PQ_TEXT_BIN_IPNAME_OFFSET_OFFSET;

    u32DstAddr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQTextBinHeader->u32BinStartAddress;

    PQ_TEXT_BIN_DBG(printf("PQTextBin IPName: %lx\n", u32DstAddr));
    return u32DstAddr;
}

MS_U32 MDrv_PQTextBin_GetTableName(void *pInstance,MS_U16 u16PQIPIdx, MS_U16 u16TabIdx, const MS_PQTextBin_Header_Info* const pPQTextBinHeader)
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U32 u32DstAddr;
    MS_U16 u16GroupNum;
    MS_U32 u32Total_Len;
    MS_U32 i;

    if(u16PQIPIdx >= pPQTextBinHeader->u16GroupIPNum)
    {
        MS_ASSERT(0);
        printf("[PQBinText] GetGroupName:: IPIdx Out of range %u\n", u16PQIPIdx);
        u16PQIPIdx = 0;
    }

    u32Addr   = pPQTextBinHeader->u32IP_Group_Offset + pPQTextBinHeader->u32BinStartAddress;
    u32Offset = pPQTextBinHeader->u32IP_Group_pitch * u16PQIPIdx +
                PQ_TEXT_BIN_GROUPNAME_IPGROUP_GROUPNUM_OFFSET;
    u16GroupNum = MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);

    if(u16TabIdx >= u16GroupNum)
    {
        MS_ASSERT(0);
        printf("[PQBinText] GetGroupName :: TableIdx Out of range %u\n", u16TabIdx);
        u16TabIdx = 0;
    }
    u32Addr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQTextBinHeader->u32BinStartAddress;

    u32Offset  = PQ_TEXT_BIN_GROUPNAME_GROUPOFFSET_OFFSET;
    u32DstAddr = MDrv_PQBin_Get4ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset) +
                 pPQTextBinHeader->u32BinStartAddress;

    u32Total_Len = 0;
    for(i=0; i< u16TabIdx; i++)
    {
        u32Offset = PQ_TEXT_BIN_GROUPNAME_GROUPOFFSET_LEN +
                    PQ_TEXT_BIN_GROUPNAME_GROUPLENGTH_LEN * i;

        u32Total_Len += MDrv_PQBin_Get2ByteData(pInstance,(void *)(u32Addr+u32Offset), &u32Offset);
    }

    PQ_TEXT_BIN_DBG(printf("PQTextBin GroupName: %lx\n", (u32DstAddr+u32Total_Len)));
    return (u32DstAddr+u32Total_Len);
}

//------------------ UFSC ---------------------------------------
void MDrv_PQBin_DumpUFSCScalerRegTable(void *pInstance, const MS_PQBin_IP_Table_Info* const pTabInfo)
{
#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC

    MS_PHYADDR u32RegAddr;
    MS_U8 u8Mask;
    MS_U8 u8Addr;
    MS_U8 u8Value;
    MS_U8 u8CurBank = 0xff;
    MS_PHYADDR u32Addr;
    MS_U32 u32Offset;
    MS_U16 i;

#ifdef MSOS_TYPE_LINUX
#if 0//(ENABLE_PQ_MLOAD)
    pthread_mutex_lock(&_PQ_MLoad_Mutex);
#endif
#endif
    //consider dump reg amount is very large, so we shouldn't use MApi_XC_W2BYTE(),
    //  because mutex cost much time.
    SC_BK_STORE_MUTEX;

    PQ_BIN_DUMP_DBG(printf("tab: sc\n"));
    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {
        PQ_BIN_DBG(printf("[PQBin]IP_Info error: Scaler Reg Table\n"));

        SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if 0//(ENABLE_PQ_MLOAD)
        pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif

        return;
    }

#if 0//(ENABLE_PQ_MLOAD)
    if (_bMLoadEn)
    {
        if (pTabInfo->u16RegNum >= MLOAD_MAX_CMD)
        {
            printf("[PQ ERROR] Function %s ==== ML overflow !!!\n",__FUNCTION__);
            assert(FALSE);
        }
        else if ( (_u16MLoadCmdCnt + pTabInfo->u16RegNum) >= MLOAD_MAX_CMD)
        {
            SC_BK_RESTORE_MUTEX;
            MDrv_PQBin_Set_MLoadEn(FALSE);
            MDrv_PQBin_Set_MLoadEn(TRUE);
            SC_BK_STORE_MUTEX;
        }
    }
#endif

    u32Addr = pTabInfo->u32TabOffset;
    for(i=0; i<pTabInfo->u16RegNum; i++)
    {
        u32Offset = ( PQ_BIN_BANK_SIZE + PQ_BIN_ADDR_SIZE + PQ_BIN_MASK_SIZE + (MS_U32)pTabInfo->u16GroupNum) * (MS_U32)i;
        u8CurBank = MDrv_PQBin_GetByteData(pInstance, (void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Addr    = MDrv_PQBin_GetByteData(pInstance, (void *)((void *)(u32Addr+u32Offset)), &u32Offset);
        u8Mask    = MDrv_PQBin_GetByteData(pInstance, (void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        u32Offset +=  (MS_U32)pTabInfo->u16GroupIdx;
        u8Value = MDrv_PQBin_GetByteData(pInstance, (void *)((void *)(u32Addr+u32Offset)), &u32Offset);

        PQ_BIN_DBG_SRAMERROR(printf("[UFSC][read addr=%x], GroupIdx=%x,RegNum=%x,reg=%04x, msk=%02x, val=%02x, u8CurBank=%x\n",
            (u32Addr+u32Offset-(MS_U32)pTabInfo->u16GroupIdx),pTabInfo->u16GroupIdx,pTabInfo->u16RegNum, BK_UFSC_SCALER_BASE | (MS_U32)u8Addr, u8Mask, u8Value, u8CurBank));

        if (0)//(_bMLoadEn)
        {
            u32RegAddr = (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;
            //PQ_REG_MLOAD_FUNC(u32RegAddr, u8Value, u8Mask);
                      PQ_REG_MLOAD_WRITE_CMD(u32RegAddr, u8Value, u8Mask);
        }
        else
        {
            u32RegAddr = BK_UFSC_SCALER_BASE | (((MS_U32)u8CurBank) << 8) | (MS_U32)u8Addr;

            PQ_REG_FUNC(u32RegAddr, u8Value, u8Mask);
        }
        PQ_BIN_DUMP_DBG(printf("[UFSC][addr=%04lx, msk=%02x, val=%02x]\n", u32RegAddr, u8Mask, u8Value));

    }

    SC_BK_RESTORE_MUTEX;

#ifdef MSOS_TYPE_LINUX
#if 0//(ENABLE_PQ_MLOAD)
    pthread_mutex_unlock(&_PQ_MLoad_Mutex);
#endif
#endif
#endif
}

void MDrv_PQBin_DumpUFSCFilterTable(void *pInstance, MS_PQBin_IP_Table_Info *pTabInfo)
{

#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
    MS_PHYADDR u32Addr;
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32PTHChk;

    s32PTHChk = pthread_mutex_lock(&_stPQFilterTableMutex);

    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d lock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif

    PQ_BIN_DBG(printf("tab: sram\n"));


    if(pTabInfo->u16GroupIdx >= pTabInfo->u16GroupNum)
    {

        PQ_BIN_DUMP_FILTER_DBG(printf("[PQBin]IP_Info error: SRAM Table\n"));
#ifdef MSOS_TYPE_LINUX
        s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
        if(s32PTHChk != 0)
        {
            printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
        }
#endif
        return;
    }

    u32Addr = pTabInfo->u32TabOffset + pTabInfo->u16GroupIdx * pTabInfo->u16RegNum;

    switch(pTabInfo->u16TableType)
    {
    case PQ_TABTYPE_UFSC_SRAM1:
        Hal_PQ_set_UFSC_y_sram_table(pInstance, SC_FILTER_Y_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_SRAM2:
        Hal_PQ_set_UFSC_y_sram_table(pInstance, SC_FILTER_Y_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_SRAM3:
        Hal_PQ_set_UFSC_y_sram_table(pInstance, SC_FILTER_Y_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_SRAM4:
        Hal_PQ_set_UFSC_y_sram_table(pInstance, SC_FILTER_Y_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_C_SRAM1:
        Hal_PQ_set_UFSC_c_sram_table(pInstance, SC_FILTER_C_SRAM1, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_C_SRAM2:
        Hal_PQ_set_UFSC_c_sram_table(pInstance, SC_FILTER_C_SRAM2, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_C_SRAM3:
        Hal_PQ_set_UFSC_c_sram_table(pInstance, SC_FILTER_C_SRAM3, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_C_SRAM4:
        Hal_PQ_set_UFSC_c_sram_table(pInstance, SC_FILTER_C_SRAM4, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_VIP_ICC_CRD_SRAM:
        Hal_PQ_set_UFSC_sram_icc_crd_table(pInstance, SC_FILTER_SRAM_ICC_CRD, (void *)u32Addr);
        break;
    case PQ_TABTYPE_UFSC_VIP_IHC_CRD_SRAM:
        Hal_PQ_set_UFSC_sram_ihc_crd_table(pInstance, SC_FILTER_SRAM_IHC_CRD, (void *)u32Addr);
        break;
    default:
        printf("[PQBin] Unknown sram type %u\n", pTabInfo->u16TableType);
        MS_ASSERT(0);
        break;
    }

#ifdef MSOS_TYPE_LINUX
    s32PTHChk = pthread_mutex_unlock(&_stPQFilterTableMutex);
    if(s32PTHChk != 0)
    {
        printf("Func: %s():%d unlock mutex error\n", __FUNCTION__, __LINE__);
    }
#endif
#endif
}


//--------------------------------------------------------------
