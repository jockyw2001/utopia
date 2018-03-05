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
///////////////////////////////////////////////////////////////////////////////
#define _DRVBYTEDMA_C

////////////////////////////////////////////////////////////////////////////////
/// file drvBDMA.c
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX_KERNEL)
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvBDMA.h"
#include "halBDMA.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"
#include "utopia.h"
#include "drvSEM.h"

#include "ULog.h"

////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////
#define TAG_BDMA "BDMA"
#define BDMA_WAIT_TIME              (HAL_BDMA_MS(100)*0x400)
#define BDMA_DELAYMS(ms)            MsOS_DelayTask(ms)
#define BDMA_IS_TIMEOUT(x)          ((x) ? FALSE : TRUE)
#define BDMA_DBG_FUNC()             if (_gsInfo.eDbgLv >= E_BDMA_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\t====   %s   ====\n", __FUNCTION__));}
#define BDMA_DBG_INFO(x, args...)   if (_gsInfo.eDbgLv >= E_BDMA_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_BDMA, x, ##args));}
#define BDMA_DBG_ERR(x, args...)    if (_gsInfo.eDbgLv >= E_BDMA_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_BDMA, x, ##args));}

#define BDMA_DFLUSH(base, size)     MsOS_Dcache_Flush(MS_PA2KSEG0(base), size)
//bdma mutex wait time
#define BDMA_MUTEX_WAIT_TIME        (5000)
#define _NOP_                       MAsm_CPU_Nop();
#define BDMA_USE_HW_SEM             (0)
#define BDMA_USE_ISR                (0)
#define BDMA_USE_DMA_FLASH_COPY     (0)


#if defined(__AEONR2__)
#define AEON_R2
#define RES_ID          (0x02)
#define START_CH        (0)
#elif (defined (MCU_AEON))
#define AEON_MCU
#define RES_ID          (0x01)
#define START_CH        (1)
#elif defined (MCU_MIPS_4KE) || defined(MCU_MIPS_34K) || defined(MCU_MIPS_74K) || defined(MCU_MIPS_1004K)
#define MIPS_MCU
#define RES_ID          (0x02)
#define START_CH        (0)
#elif defined (__arm__)
#define ARM_MCU
#define RES_ID          (0x02)
#define START_CH        (0)
#elif defined (__aarch64__)
#define ARM_MCU
#define RES_ID          (0x02)
#define START_CH        (0)
#else
#Error_MCU_for_BDMA
#endif

typedef struct _BDMA_OP_CB  //control block
{
    MS_U8     u8OpCfg;
    MS_U8     u8SrcDevCfg;
    MS_U8     u8DstDevCfg;
    MS_U8     u8DmyWrCnt;
    MS_PHY  phy64SrcAddr;
    MS_PHY  phy64DstAddr;
    MS_U32    u32DataSize;
    MS_U32    u32Cmd0;
    MS_U32    u32Cmd1;
    MS_U32    u32Cmd2;
    BDMA_Act        eAct;
    BDMA_ISR_CBF    pCbf;
}BDMA_OP_CB;

#if defined(T3_REMOVE_LATER)
MS_U8 _gu8ChipVer = 0;
#endif

#define BDMASEMID       (0x03)
////////////////////////////////////////////////////////////////////////////////
// External funciton
////////////////////////////////////////////////////////////////////////////////
//extern MS_BOOL MDrv_FLASH_CheckWriteDone(void);

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static BDMA_Info _gsInfo = {
                            .u8ChNum = 0,
                            };
static BDMA_HwInfo _gsHwInfo = {
                                .bEnMIU1 = 0,
                                };
static MSIF_Version _drv_bdma_version = {
    .DDI = { BDMA_DRV_VERSION },
};

#if (BDMA_USE_ISR)
//ISR callback function for channel 0 & channel 1
static BDMA_ISR_CBF pIsrCbf[E_BDMA_CH_END];
#endif

#if (BDMA_UTOPIA20)
void* pInstantBdma = NULL;
void* pAttributeBdma = NULL;

#endif
////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
static void _BDMA_MemFlush(BDMA_Dev eDev, MS_U32 u32Base, MS_U32 u32Size);
static MS_U8 _BDMA_GetDevCfg(BDMA_Dev eDev);
static MS_BOOL _BDMA_Is_Init(void);
static MS_BOOL _BDMA_Is_Done_INT(MS_U8 u8Ch);
static MS_BOOL _BDMA_Get_FreeCh(MS_U8 *pu8Ch);
static MS_BOOL _BDMA_GetHwInfo(BDMA_HwInfo *psHwInfo);
#if (BDMA_USE_DMA_FLASH_COPY)
static BDMA_Result _DMA_FlashCopy(BDMA_OP_CB sOpCB);
#endif
static BDMA_Result _BDMA_WaitDone(BDMA_Act eAct, MS_U8 u8Ch);
static BDMA_Result _BDMA_CmnHnd(BDMA_OP_CB sOpCB, MS_U8 *pu8Ch);
static BDMA_Result _BDMA_Start(MS_U8 u8SrcDev, MS_U8 u8DstDev, MS_U8 u8Ch);
static BDMA_Result _BDMA_Check_Device(BDMA_SrcDev eSrc, BDMA_DstDev eDst);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Is_Init
/// @brief \b Function  \b Description: check if BDMA is initial
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b TRUE: Initial FALSE: Not initial
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _BDMA_Is_Init(void)
{
    BDMA_DBG_FUNC();
    if (!_gsInfo.bInit)
        BDMA_DBG_ERR("Call MDrv_BDMA_Init first!\n");
    return _gsInfo.bInit;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Is_Int
/// @brief \b Function  \b Description: Check if interrupted for BDMA done
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Interrupted FALSE: No interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _BDMA_Is_Done_INT(MS_U8 u8Ch)
{
    BDMA_DBG_FUNC();
    return HAL_BDMA_Is_Int(u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Get_FreeCh
/// @brief \b Function  \b Description: get free channel for BDMA action
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _BDMA_Get_FreeCh(MS_U8 *pu8Ch)
{
    MS_U32 u32Delay = 0;

    BDMA_DBG_FUNC();
    if (!pu8Ch)
        return FALSE;

#if (BDMA_USE_HW_SEM)
    static MS_U8 preCh = 0;
    *pu8Ch = ((preCh+1) % E_BDMA_CH_END);
#else
    *pu8Ch = START_CH;
#endif
    u32Delay = BDMA_WAIT_TIME;
    do
    {
        if ((!HAL_BDMA_Is_Queued(*pu8Ch)) && (!HAL_BDMA_Is_Busy(*pu8Ch)))
            break;
        if(BDMA_IS_TIMEOUT(u32Delay))
        {
            BDMA_DBG_ERR("BDAM no free channel!\n");
            *pu8Ch = 0xFF;
            return FALSE;
        }
#if (BDMA_USE_HW_SEM)
        if (++*pu8Ch == E_BDMA_CH_END)
            *pu8Ch = E_BDMA_CH_START;
#endif
        u32Delay--;
    }while(1);

    BDMA_DBG_INFO("BDMA Ch%u is free!\n", *pu8Ch);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Start
/// @brief \b Function  \b Description: Start BDMA action for channel 0/1
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static BDMA_Result _BDMA_Start(MS_U8 u8SrcDev, MS_U8 u8DstDev, MS_U8 u8Ch)
{
    BDMA_DBG_FUNC();

    if (HAL_BDMA_Is_Queued(u8Ch) || HAL_BDMA_Is_Busy(u8Ch))
    {
        BDMA_DBG_ERR("BDMA Ch%u can't start!\n", u8Ch);
        return E_BDMA_FAIL;
    }
    HAL_BDMA_TrigOn(u8SrcDev, u8DstDev, u8Ch);
    return E_BDMA_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_MemFlush
/// @brief \b Function  \b Description: flush dcache
/// @param <IN>         \b eDev: selected device
/// @param <IN>         \b u32Base: physical address
/// @param <IN>         \b u32Size: len
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static void _BDMA_MemFlush(BDMA_Dev eDev, MS_U32 u32Base, MS_U32 u32Size)
{
    BDMA_DBG_FUNC();

    switch (eDev)
    {
    case E_BDMA_DEV_MIU0:
    case E_BDMA_DEV_MIU1:
    case E_BDMA_DEV_MIU2:
    case E_BDMA_DEV_MIU3:
        if(MS_PA2KSEG0(u32Base))
        {
            BDMA_DFLUSH(u32Base, u32Size);
        }
        MsOS_FlushMemory();
        break;

    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_WaitDone
/// @brief \b Function  \b Description: wait done for bdma action
/// @param <IN>         \b eAct: action
/// @param <IN>         \b u8Ch: channel
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static BDMA_Result _BDMA_WaitDone(BDMA_Act eAct, MS_U8 u8Ch)
{
    MS_U32 u32Delay = 0;

    BDMA_DBG_FUNC();
    if (E_BDMA_ACT_MAX <= eAct)
    {
        BDMA_DBG_ERR("Error action: %u!\n", eAct);
        return E_BDMA_FAIL;
    }

    u32Delay = BDMA_WAIT_TIME;
    do
    {
        if (HAL_BDMA_Is_Done(eAct, u8Ch))
        {
            if (_BDMA_Is_Done_INT(u8Ch))
            {
                BDMA_DBG_INFO("BDMA ch%u act:%u interruptted!\n ",u8Ch, eAct);
            }
            BDMA_DBG_INFO("BDMA ch%u act:%u done!\n",u8Ch, eAct);
            if (E_BDMA_DBGLV_REG_DUMP == _gsInfo.eDbgLv)
            {
                HAL_BDMA_Dump_AllReg();
            }
            return E_BDMA_OK;
        }
        u32Delay--;
    }while(!BDMA_IS_TIMEOUT(u32Delay));
    BDMA_DBG_ERR("BDMA ch%u wait act:%u done timeout!\n ",u8Ch, eAct);

    return E_BDMA_TIMEOUT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_CmnHnd
/// @brief \b Function  \b Description: Handle for common actions
/// @param <IN>         \b sOpCB : control block
/// @param <OUT>        \b u8Ch : action channel
/// @param <RET>        \b TRUE : Success FALSE: Fail
/// @param <GLOBAL>     \b sOpCB : global control block
////////////////////////////////////////////////////////////////////////////////
static BDMA_Result _BDMA_CmnHnd(BDMA_OP_CB sOpCB, MS_U8 *pu8Ch)
{
    BDMA_Result eRet;

    BDMA_DBG_FUNC();
    MDrv_BDMA_DumpCB(&sOpCB);

    //make atomic operation
    if (FALSE == MsOS_ObtainMutex(_gsInfo.s32Mutex, BDMA_MUTEX_WAIT_TIME))
    {
        BDMA_DBG_ERR("%s ENTRY fails!\n", __FUNCTION__);
        return E_BDMA_FAIL;
    }


#if (BDMA_USE_HW_SEM)
    MS_U32 u32Cnt = 0;

    while(FALSE == MDrv_SEM_Get_Resource(BDMASEMID, RES_ID))
    {
        if (BDMA_WAIT_TIME == ++u32Cnt)
        {
            BDMA_DBG_ERR("%s can't get HW SEM!\n", __FUNCTION__);
            eRet = E_BDMA_TIMEOUT;
            MsOS_ReleaseMutex(_gsInfo.s32Mutex);
            return eRet;
        }
    }
#endif

    if (!pu8Ch || !_BDMA_Get_FreeCh(pu8Ch))
    {
        eRet = E_BDMA_FAIL;
        goto end;
    }
    eRet = E_BDMA_FAIL;
    //clear status first
    HAL_BDMA_Clear_Status(sOpCB.eAct, *pu8Ch);

    //Set start address
    HAL_BDMA_SetSrcAddr(sOpCB.eAct, sOpCB.phy64SrcAddr, *pu8Ch);
    HAL_BDMA_SetDstAddr(sOpCB.eAct, sOpCB.phy64DstAddr, *pu8Ch);
    //Set size
    HAL_BDMA_SetLen(sOpCB.eAct, sOpCB.u32DataSize, *pu8Ch);

    HAL_BDMA_SetSrcDev(sOpCB.u8SrcDevCfg, *pu8Ch);
    HAL_BDMA_SetDstDev(sOpCB.u8DstDevCfg, *pu8Ch);
    HAL_BDMA_SetCmd0(sOpCB.u32Cmd0 ,*pu8Ch);
    HAL_BDMA_SetCmd1(sOpCB.u32Cmd1 ,*pu8Ch);
    HAL_BDMA_SetCmd2(sOpCB.u32Cmd2 ,*pu8Ch);
    // Set INT
    HAL_BDMA_Enable_INT(((sOpCB.pCbf)?TRUE:FALSE), *pu8Ch);
    // Set address direction
    HAL_BDMA_Set_Addr_Dec((sOpCB.u8OpCfg & BDMA_OPCFG_INV_COPY), *pu8Ch);
    // Set crc reflection
    HAL_BDMA_Set_CRC_Reflect((sOpCB.u8OpCfg & BDMA_OPCFG_CRC_REFLECT), *pu8Ch);
#ifdef MOBF_ENABLE
    // Set mobf ps
    HAL_BDMA_MOBF((sOpCB.u8OpCfg & BDMA_OPCFG_MOBF_PS), *pu8Ch);
#endif
    HAL_BDMA_SetDmyWrCnt(sOpCB.u8DmyWrCnt, *pu8Ch);

    if (E_BDMA_DBGLV_REG_DUMP == _gsInfo.eDbgLv)
    {
        HAL_BDMA_Dump_AllReg();
    }
    //while(1);
    if (_BDMA_Start(sOpCB.u8SrcDevCfg, sOpCB.u8DstDevCfg, *pu8Ch))
    {
#if (BDMA_USE_ISR)
        if (sOpCB.pCbf)
        {
            pIsrCbf[*pu8Ch] = sOpCB.pCbf;
            BDMA_DBG_INFO("BDMA Ch%u act:%u wait ISR!\n", *pu8Ch, sOpCB.eAct);
            eRet = E_BDMA_OK;
            goto end;

        }
#endif
        eRet = _BDMA_WaitDone(sOpCB.eAct, *pu8Ch);
    }
    else
    {
        BDMA_DBG_ERR("BDMA Act:%u can't start\n", sOpCB.eAct);
    }
end:
    _NOP_ _NOP_ _NOP_

#if (BDMA_USE_HW_SEM)
    MDrv_SEM_Free_Resource(BDMASEMID, RES_ID);
#endif
    MsOS_ReleaseMutex(_gsInfo.s32Mutex);
    return eRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _DMA_FlashCopy
/// @brief \b Function  \b Description: Handle for DMA flash copy action
/// @param <IN>         \b sOpCB : control block
/// @param <OUT>        \b u8Ch : action channel
/// @param <RET>        \b TRUE : Success FALSE: Fail
/// @param <GLOBAL>     \b sOpCB : global control block
////////////////////////////////////////////////////////////////////////////////
#if (BDMA_USE_DMA_FLASH_COPY)
static BDMA_Result _DMA_FlashCopy(BDMA_OP_CB sOpCB)
{
    MS_U8 u8Ch = E_BDMA_CH0;
    MS_U32 u32Delay = BDMA_WAIT_TIME;

    BDMA_DBG_FUNC();
    MDrv_BDMA_DumpCB(&sOpCB);

    //if bdma support flash, plz use bdma command
    if (_gsHwInfo.bEnFlsCpy)
    {
        BDMA_DBG_ERR("BDMA support flash copy!\n");
        return E_BDMA_FAIL;
    }

    while (!HAL_DMA_Is_FlashReady())
    {
        if (BDMA_IS_TIMEOUT(u32Delay))
        {
            BDMA_DBG_ERR("DMA flash is busy!\n");
            return E_BDMA_TIMEOUT;
        }
        u32Delay--;
    }

    //Set start address
    HAL_BDMA_SetSrcAddr(sOpCB.eAct, sOpCB.phy64SrcAddr, u8Ch);
    HAL_BDMA_SetDstAddr(sOpCB.eAct, sOpCB.phy64DstAddr, u8Ch);
    //Set size
    HAL_BDMA_SetLen(sOpCB.eAct, HAL_BDMA_ALIGN(sOpCB.u32DataSize), u8Ch);
    HAL_BDMA_Clear_Status(sOpCB.eAct, u8Ch);

    if (_BDMA_Start(sOpCB.u8SrcDevCfg, sOpCB.u8DstDevCfg, u8Ch))
    {
        BDMA_Result eRet = _BDMA_WaitDone(sOpCB.eAct, u8Ch);
        if (E_BDMA_OK == eRet)
        {
            //flush miu FIFO data to MIU
            if (!_gsHwInfo.bEnDmyWrCnt && (E_BDMA_DEV_MIU0 == (sOpCB.u8DstDevCfg&0x0F) || E_BDMA_DEV_MIU1 == (sOpCB.u8DstDevCfg&0x0F) || E_BDMA_DEV_MIU2 == (sOpCB.u8DstDevCfg&0x0F) || E_BDMA_DEV_MIU3 == (sOpCB.u8DstDevCfg&0x0F)))
                MDrv_BDMA_CRC32(sOpCB.phy64DstAddr, HAL_BDMA_DMY_WRCNT, BDMA_CRC32_POLY, BDMA_CRC_SEED_0, (BDMA_SrcDev)(sOpCB.u8DstDevCfg&0x0F), FALSE);
        }
        return eRet;
    }
    else
    {
        BDMA_DBG_ERR("DMA can't start flash copy\n");
        return E_BDMA_FAIL;
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Check_Device
/// @brief \b Function  \b Description: Check if support selected device
/// @param <IN>         \b eSrc: source device
/// @param <IN>         \b eDst: destination device
/// @param <OUT>        \b None :
/// @param <RET>        \b BDMA_Result
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static BDMA_Result _BDMA_Check_Device(BDMA_SrcDev eSrc, BDMA_DstDev eDst)
{
    BDMA_DBG_FUNC();

    switch(eSrc)
    {
    case E_BDMA_SRCDEV_MIU0:
    case E_BDMA_SRCDEV_MIU1:
    case E_BDMA_SRCDEV_MIU2:
    case E_BDMA_SRCDEV_MIU3:
    case E_BDMA_SRCDEV_FLASH:
    case E_BDMA_SRCDEV_MEM_FILL:
        break;

    default:
        BDMA_DBG_ERR("Source Device error:%x!\n", eSrc);
        return E_BDMA_FAIL;
    }
    if (E_BDMA_DSTDEV_NOT_SUPPORT <= eDst)
    {
        BDMA_DBG_ERR("Destination Device error:%x!\n", eDst);
        return E_BDMA_FAIL;
    }

    if (((E_BDMA_DEV_MIU1 == (BDMA_Dev)eSrc) || (E_BDMA_DEV_MIU1 == (BDMA_Dev)eDst)) && (!_gsHwInfo.bEnMIU1))
    {
        BDMA_DBG_ERR("Not support MIU1!\n");
        return E_BDMA_FAIL;
    }

    if (((!_gsHwInfo.bEnDMDMCU) && (E_BDMA_DSTDEV_DMDMCU == eDst))
        || ((!_gsHwInfo.bEnDSP) && (E_BDMA_DSTDEV_DSP == eDst))
        || ((!_gsHwInfo.bEnTSP) && (E_BDMA_DSTDEV_TSP == eDst)))
    {
        BDMA_DBG_ERR("Device:%u is not supported!\n", eDst);
        return E_BDMA_NOT_SUPPORT;
    }

    return E_BDMA_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_GetDevCfg
/// @brief \b Function  \b Description: Get device cfg for BDMA copy
/// @param <IN>         \b eDev: device
/// @param <OUT>        \b None :
/// @param <RET>        \b src & dst device configuration for bdma copy
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 _BDMA_GetDevCfg(BDMA_Dev eDev)
{
    BDMA_DBG_FUNC();

    if (!_gsHwInfo.bEnDevDw)
    {
        return eDev;
    }
    return (MS_U8)(HAL_BDMA_GetDevId(eDev) | HAL_BDMA_GetDevDw(eDev));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Isr_Proc
/// @brief \b Function  \b Description: BDMA ISR
/// @param <IN>         \b vpReg: registers status
/// @param <IN>         \b u32Data: passing data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
#if (BDMA_USE_ISR)
static void _BDMA_Isr_Proc(InterruptNum eIntNum)
{
    BDMA_Result eRet = E_BDMA_OK;

    BDMA_DBG_INFO("BDMA Ch%u ISR!\n", eIntNum);

    MDrv_BDMA_Stop(eIntNum);
    pIsrCbf[eIntNum](eRet);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_GetHwInfo
/// @brief \b Function  \b Description: Get hardware support facilities
/// @param <IN>         \b None :
/// @param <OUT>        \b *psHwInfo: Hardware information
/// @param <RET>        \b TRUE : Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _BDMA_GetHwInfo(BDMA_HwInfo *psHwInfo)
{
    if (!psHwInfo)
    {
        return FALSE;
    }
    BDMA_DBG_FUNC();
    psHwInfo->bEnMemFill = BDMA_MEM_FILL;
    psHwInfo->bEnFlsCpy = BDMA_FLASH_COPY;
    psHwInfo->bEnDevDw = BDMA_DEV_DATA_WIDTH;
    psHwInfo->bEnDmyWrCnt = BDMA_DUMMY_WRCNT;
    psHwInfo->bEnDMDMCU = BDMA_DMDMCU;
    psHwInfo->bEnTSP = BDMA_TSP;
    psHwInfo->bEnDSP = BDMA_DSP;
    psHwInfo->bEnHK51_1KSRAM = BDMA_HK51_1KSRAM;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _BDMA_Check_MIUDev
/// @brief \b Function  \b Description: check if address within miu range
/// @param <IN>         \b eDev : device
/// @param <IN>         \b phy64Addr : start address
/// @param <OUT>        \b None:
/// @param <RET>        \b BDMA_Dev: device
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static BDMA_Dev _BDMA_Check_MIUDev(BDMA_Dev eDev, MS_PHY phy64Addr)
{
    MS_U8 u8MiuSel;
    MS_U32 u32Offset;
    BDMA_DBG_FUNC();

    _phy_to_miu_offset(u8MiuSel, u32Offset, phy64Addr)

    if (eDev != E_BDMA_DEV_MIU0 && eDev != E_BDMA_DEV_MIU1 && eDev != E_BDMA_DEV_MIU2 && eDev != E_BDMA_DEV_MIU3)
        return eDev;

    if (u8MiuSel == E_CHIP_MIU_3)
    {
        return E_BDMA_DEV_MIU3;
    }
    else if (u8MiuSel == E_CHIP_MIU_2)
    {
        return E_BDMA_DEV_MIU2;
    }
    else if (u8MiuSel == E_CHIP_MIU_1)
    {
        return E_BDMA_DEV_MIU1;
    }
    else
    {
        return E_BDMA_DEV_MIU0;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_WaitFlashDone
/// @brief \b Function  \b Description: Wait for flash copy done
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_WaitFlashDone(void)
{
    return _BDMA_WaitDone(E_BDMA_ACT_COPY_FLASH, E_BDMA_CH0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_GetLibVer
/// @brief \b Function  \b Description: Show the BDMA driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result MDrv_BDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    BDMA_DBG_FUNC();

    if (!ppVersion)
    {
        return E_BDMA_FAIL;
    }

    *ppVersion = &_drv_bdma_version;
    return E_BDMA_OK;
}

BDMA_Result MDrv_BDMA_Init(MS_PHY phy64Miu1Base)
{
#if BDMA_UTOPIA20
    if(pInstantBdma == NULL)
    {
        if(UtopiaOpen(MODULE_BDMA, &pInstantBdma, 0, pAttributeBdma) !=  UTOPIA_STATUS_SUCCESS)
        {
            printf("Open BDMA fail\n");
            return E_BDMA_FAIL;
        }
    }

    PBDMA_INIT_PARAM pParam;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pParam = (PBDMA_INIT_PARAM)malloc(sizeof(BDMA_INIT_PARAM));
    if (NULL == pParam)
        return E_BDMA_FAIL;
    pParam->u64Miu1Base = phy64Miu1Base;
    if(UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_Init,(void*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_MemCopy fail\n");
        free(pParam);
        return E_BDMA_FAIL;
    }
    free(pParam);
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_Init(phy64Miu1Base);
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Init
/// @brief \b Function  \b Description: Init BDMA driver
/// @param phy64Miu1Base  \b if 0, not support miu1
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_Init(MS_PHY phy64Miu1Base)
{
    MS_PHY u32BaseSize = 0;

    if (!_gsInfo.bInit)
    {
        _gsInfo.s32Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex BDMA", MSOS_PROCESS_SHARED);
        MS_ASSERT(_gsInfo.s32Mutex >= 0);

#if defined(T3_REMOVE_LATER)
        if (MDrv_MMIO_GetBASE(&_gsInfo.u32IOMap, &u32BaseSize, MS_MODULE_PM))
        {
            HAL_BDMA_SetIOMapBase(_gsInfo.u32IOMap);
            _gu8ChipVer = HAL_BDMA_ReadByte(0x001ECF);
            BDMA_DBG_INFO("ChipVer:%u\n", _gu8ChipVer);
        }
#endif

        if(!MDrv_MMIO_GetBASE(&_gsInfo.u32IOMap, &u32BaseSize, MS_MODULE_BDMA))
        {
            BDMA_DBG_ERR("Get IOMAP Base faill!\n");
            return E_BDMA_FAIL;
        }
        BDMA_DBG_INFO("Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_BDMA, _gsInfo.u32IOMap);
        HAL_BDMA_SetIOMapBase(_gsInfo.u32IOMap);

        //test read/write register
        if (HAL_BDMA_SetSrcAddr(E_BDMA_ACT_CRC32, 0x5A6B7C8F, E_BDMA_CH1)
            && (0x5A6B7C8F != HAL_BDMA_GetSrcAddr(E_BDMA_ACT_CRC32, E_BDMA_CH1)))
        {
            BDMA_DBG_ERR("Reg CMD1 r/w test error: %lx!\n", HAL_BDMA_GetCmd1(E_BDMA_CH1));
            return E_BDMA_FAIL;
        }

        if (_BDMA_GetHwInfo(&_gsHwInfo))
        {
            _gsHwInfo.bEnMIU1 = (HAL_MIU1_BASE) ? TRUE : FALSE;
        }
        _gsInfo.phy64MIU1Base = HAL_MIU1_BASE;
        BDMA_DBG_INFO("Miu1 base:%lx\n", _gsInfo.phy64MIU1Base);
        _gsInfo.eDbgLv = E_BDMA_DBGLV_ERR_ONLY;
        _gsInfo.bInit = TRUE;
        return E_BDMA_OK;
    }
    else
    {
        BDMA_DBG_ERR("BDMA had initial!\n");
        return E_BDMA_OK;
    }

    return E_BDMA_FAIL;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Exit
/// @brief \b Function  \b Description: Exit BDMA driver
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result MDrv_BDMA_Exit()
{
    if (!_gsInfo.bInit)
    {
        BDMA_DBG_INFO("BDMA not init!\n");
        return E_BDMA_FAIL;
    }

    MsOS_DeleteMutex(_gsInfo.s32Mutex);
    _gsInfo.bInit = FALSE;
#if (BDMA_UTOPIA20)
    if(UtopiaClose(pInstantBdma) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "close BDMA fail\n");
        pInstantBdma = NULL;
        return E_BDMA_FAIL;
    }
    pInstantBdma = NULL;
#endif
    return E_BDMA_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_SetDbgLevel
/// @brief \b Function  \b Description: Set debug level for debug message
/// @param eLevel       \b IN: E_BDMA_DBGLV_NONE/E_BDMA_DBGLV_ERR_ONLY/E_BDMA_DBGLV_INFO/E_BDMA_DBGLV_ALL
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result MDrv_BDMA_SetDbgLevel(BDMA_DbgLv eLevel)
{
    BDMA_DBG_INFO("%s level: %u\n", __FUNCTION__, eLevel);

    _gsInfo.eDbgLv = eLevel;
    return E_BDMA_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Stop
/// @brief \b Function  \b Description: Stop BDMA actions in specific channel
/// @param u8Ch         \b IN: Channel 0/1
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result MDrv_BDMA_Stop(MS_U8 u8Ch)
{
    BDMA_DBG_FUNC();
    return E_BDMA_NOT_SUPPORT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Stop_All
/// @brief \b Function  \b Description: Stop all BDMA actions
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result MDrv_BDMA_Stop_All(void)
{
    BDMA_DBG_FUNC();
    return E_BDMA_NOT_SUPPORT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_PatternFill
/// @brief \b Function  \b Description: Fill pattern to filled dst device
/// @param u32Addr      \b IN: Physical Start address
/// @param u32Len       \b IN: Fill Pattern length
/// @param u32Pattern   \b IN: Fill Pattern data
/// @param eDev         \b IN: filled dst device
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_PatternFill(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev)
{
    MS_U8 u8Ch;
    BDMA_OP_CB sOpCB;

    BDMA_DBG_FUNC();

    if (!_BDMA_Is_Init())
    {
        return E_BDMA_FAIL;
    }
    if (DISABLE == _gsHwInfo.bEnMemFill)
    {
        BDMA_DBG_ERR("Mem fill is not supported!\n");
        return E_BDMA_NOT_SUPPORT;
    }

    BDMA_DBG_INFO("BDMA Fill pattern start....\n");
    if (E_BDMA_OK != _BDMA_Check_Device(E_BDMA_SRCDEV_MEM_FILL, eDev))
    {
        return E_BDMA_FAIL;
    }

    sOpCB.eAct = E_BDMA_ACT_MEM_FILL;
    sOpCB.phy64SrcAddr = 0;
    sOpCB.phy64DstAddr = phy64Addr;
    sOpCB.u32DataSize = u32Len;

    sOpCB.u8SrcDevCfg = _BDMA_GetDevCfg(E_BDMA_DEV_MEM_FILL);
    sOpCB.u8DstDevCfg = _BDMA_GetDevCfg((BDMA_Dev)eDev);
    sOpCB.u32Cmd0 = u32Pattern;
    sOpCB.u32Cmd1 = 0;
    sOpCB.u32Cmd2 = 0;
    sOpCB.u8OpCfg = BDMA_OPCFG_DEF;
    sOpCB.u8DmyWrCnt = 0;
    sOpCB.pCbf = (void *)0;

    _BDMA_MemFlush((BDMA_Dev)eDev, sOpCB.phy64DstAddr, sOpCB.u32DataSize);
    return _BDMA_CmnHnd(sOpCB, &u8Ch);
}


BDMA_Result MDrv_BDMA_CopyHnd (MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg)
{
#if (BDMA_UTOPIA20)
    PBDMA_COPYHND_PARAM pParam = NULL;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pParam = (PBDMA_COPYHND_PARAM)malloc(sizeof(BDMA_COPYHND_PARAM));
    if (NULL == pParam)
        return E_BDMA_FAIL;
    pParam->phy64SrcAddr = phy64SrcAddr;
    pParam->phy64DstAddr = phy64DstAddr;
    pParam->u32Len = u32Len;
    pParam->eCpyType = eCpyType;
    pParam->u8OpCfg = u8OpCfg;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_CopyHnd,(void*)pParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_CopyHnd fail\n");
        free(pParam);
        return E_BDMA_FAIL;
    }
    free(pParam);
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_CopyHnd (phy64SrcAddr, phy64DstAddr, u32Len, eCpyType, u8OpCfg);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_CopyHnd
/// @brief \b Function  \b Description: Handle for BDMA copy data from src to dst
/// @param phy64SrcAddr   \b IN: Physical source address
/// @param phy64DstAddr   \b IN: Physical dst address
/// @param u32Len       \b IN: data length
/// @param eCpyType     \b IN: BDMA copy type
/// @param u8OpCfg      \b IN: u8OpCfg: default is BDMA_OPCFG_DEF
///                         - Bit0: inverse mode --> BDMA_OPCFG_INV_COPY
///                         - Bit2: Copy & CRC check in wait mode --> BDMA_OPCFG_CRC_COPY
///                         - Bit3: Copy without waiting --> BDMA_OPCFG_NOWAIT_COPY
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_CopyHnd (MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg)
{
    MS_U8 u8MiuSel;
    MS_U8 u8Ch;
    MS_U32 u32CRCVal = 0xFFFFFFFF;
    MS_U32 u32Offset;
    BDMA_OP_CB sOpCB;
    BDMA_Dev eSrcDev, eDstDev;
    BDMA_Result eRet = E_BDMA_FAIL;

    MS_DEBUG_MSG(MS_U32 u32Time = 0);
    BDMA_DBG_FUNC();
    if (!_BDMA_Is_Init())
    {
        return eRet;
    }

    eSrcDev = (BDMA_Dev)(eCpyType & 0x0F);
    eDstDev = (BDMA_Dev)_RShift(eCpyType, 8);
    if (eSrcDev == eDstDev && phy64SrcAddr == phy64DstAddr)
    {
        BDMA_DBG_ERR("No meaning copy: Src is same as Dst!\n");
        return E_BDMA_OK;
    }

    if ((!u32Len) || (E_BDMA_CPYTYPE_MAX <= eCpyType))
    {
        BDMA_DBG_ERR("BDMA copy critical error len:%lu type:%u!!\n", u32Len, eCpyType);
        return eRet;
    }

    sOpCB.phy64SrcAddr = phy64SrcAddr;
    sOpCB.phy64DstAddr = phy64DstAddr;
    sOpCB.u32DataSize = u32Len;

    //avoid address overlapping
    if (eSrcDev == eDstDev)
    {
        if ((phy64SrcAddr < phy64DstAddr) && ((phy64SrcAddr+u32Len-1) >= phy64DstAddr))
        {
            u8OpCfg |= BDMA_OPCFG_INV_COPY;
        }
    }

    if (u8OpCfg & BDMA_OPCFG_INV_COPY)
    {
        sOpCB.phy64SrcAddr += (sOpCB.u32DataSize-1);
        sOpCB.phy64DstAddr += (sOpCB.u32DataSize-1);
        BDMA_DBG_INFO("a4 copy inv src:%lx dst:%lx\n", sOpCB.phy64SrcAddr, sOpCB.phy64DstAddr);
    }

    if (E_BDMA_OK != _BDMA_Check_Device((BDMA_SrcDev)eSrcDev, (BDMA_DstDev)eDstDev))
    {
        return eRet;
    }
    _BDMA_MemFlush(eSrcDev, phy64SrcAddr, sOpCB.u32DataSize);
    _BDMA_MemFlush(eDstDev, phy64DstAddr, sOpCB.u32DataSize);

    sOpCB.u8SrcDevCfg = _BDMA_GetDevCfg(eSrcDev);
    sOpCB.u8DstDevCfg = _BDMA_GetDevCfg(eDstDev);

    if((E_BDMA_DSTDEV_MIU1 == (BDMA_DstDev)eDstDev) || (E_BDMA_DSTDEV_MIU2 == (BDMA_DstDev)eDstDev) || (E_BDMA_DSTDEV_MIU3 == (BDMA_DstDev)eDstDev))
    {
        _phy_to_miu_offset(u8MiuSel, u32Offset, phy64DstAddr);
        phy64DstAddr = u32Offset;

        _phy_to_miu_offset(u8MiuSel, u32Offset, sOpCB.phy64DstAddr);
        sOpCB.phy64DstAddr = u32Offset;
    }

    if((E_BDMA_SRCDEV_MIU1 == (BDMA_SrcDev)eSrcDev) || (E_BDMA_DSTDEV_MIU2 == (BDMA_DstDev)eDstDev) || (E_BDMA_DSTDEV_MIU3 == (BDMA_DstDev)eDstDev))
    {
        _phy_to_miu_offset(u8MiuSel, u32Offset, phy64SrcAddr);
        phy64SrcAddr = u32Offset;

        _phy_to_miu_offset(u8MiuSel, u32Offset, sOpCB.phy64SrcAddr);
        sOpCB.phy64SrcAddr = u32Offset;
    }

    if ((E_BDMA_DEV_MIU0 == eDstDev) || (E_BDMA_DEV_MIU1 == eDstDev) || (E_BDMA_DEV_MIU2 == eDstDev) || (E_BDMA_DEV_MIU3 == eDstDev))
    {
        sOpCB.u8DmyWrCnt = HAL_BDMA_DMY_WRCNT;
    }
    else
    {
        sOpCB.u8DmyWrCnt = 0;
    }
    sOpCB.u32Cmd0 = sOpCB.phy64DstAddr;
    sOpCB.u32Cmd1 = 0;
    sOpCB.u32Cmd2 = 0;

    sOpCB.u8OpCfg = u8OpCfg;
    sOpCB.eAct = (E_BDMA_DEV_FLASH == eSrcDev) ? E_BDMA_ACT_COPY_FLASH : E_BDMA_ACT_COPY_MEM;
    //TBD
    sOpCB.pCbf = (void *)0;

#if (BDMA_USE_DMA_FLASH_COPY)
    //compatible DMA flash copy
    if (E_BDMA_DEV_FLASH == eSrcDev)
    {
        if (!_gsHwInfo.bEnFlsCpy)
        return _DMA_FlashCopy(sOpCB);
    }
#endif
    if (u8OpCfg & BDMA_OPCFG_NOWAIT_COPY)
    {
        BDMA_DBG_INFO("BDMA no wait copy\n");
        return _BDMA_CmnHnd(sOpCB, &u8Ch);
    }

    MS_DEBUG_MSG(u32Time = MsOS_GetSystemTime());
    //Copy and CRC32 check
    if (u8OpCfg & BDMA_OPCFG_CRC_COPY)
    {
        u32CRCVal = MDrv_BDMA_CRC32(phy64SrcAddr, u32Len, BDMA_CRC32_POLY, BDMA_CRC_SEED_0, (BDMA_SrcDev)eSrcDev, FALSE);
    }
    if(E_BDMA_OK == (eRet = _BDMA_CmnHnd(sOpCB, &u8Ch)))
    {
        //flush miu FIFO data to MIU
        if (!_gsHwInfo.bEnDmyWrCnt && (E_BDMA_DEV_MIU0 == eDstDev || E_BDMA_DEV_MIU1 == eDstDev || E_BDMA_DEV_MIU2 == eDstDev || E_BDMA_DEV_MIU3 == eDstDev))
        {
            MDrv_BDMA_CRC32(sOpCB.phy64DstAddr, HAL_BDMA_DMY_WRCNT, BDMA_CRC32_POLY, BDMA_CRC_SEED_0, (BDMA_SrcDev)eDstDev, FALSE);
        }

        if (u8OpCfg & BDMA_OPCFG_CRC_COPY)
        {
            phy64DstAddr = MDrv_BDMA_CRC32(phy64DstAddr, u32Len, BDMA_CRC32_POLY, BDMA_CRC_SEED_0, (BDMA_SrcDev)eDstDev, FALSE);
            if ( (u32CRCVal != phy64DstAddr) )
            {
                BDMA_DBG_ERR("Copy check CRC error: (%lx != %lx)\n", phy64DstAddr, u32CRCVal);
                return E_BDMA_FAIL;
            }
        }

        if (E_BDMA_DBGLV_PERFORMANCE == _gsInfo.eDbgLv)
        {
            MS_DEBUG_MSG(ULOGD(TAG_BDMA, "bdma copy:%lu ms\n", MsOS_GetSystemTime() - u32Time));
        }
        return E_BDMA_OK;
    }

    BDMA_DBG_ERR("Copy error:%u\n", eRet);
    return eRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_MemCopy
/// @brief \b Function  \b Description: Copy dram to dram no matter dram address within miu0 or miu1
/// @param phy64SrcAddr   \b IN: Physical Source address
/// @param phy64DstAddr   \b IN: Physical Dst address
/// @param u32Len       \b IN: data length
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_MemCopy(MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len)
{
    MS_U8 eSrc, eDst;
    BDMA_CpyType eCpyType;

    eSrc = (MS_U8)_BDMA_Check_MIUDev(E_BDMA_DEV_MIU0, phy64SrcAddr);
    eDst = (MS_U8)_BDMA_Check_MIUDev(E_BDMA_DEV_MIU0, phy64DstAddr);
    eCpyType = (BDMA_CpyType)(eSrc | _LShift(eDst, 8));
    return _MDrv_BDMA_CopyHnd(phy64SrcAddr, phy64DstAddr, u32Len, eCpyType, BDMA_OPCFG_DEF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_FlashCopy2Dram
/// @brief \b Function  \b Description: Copy data from flash to dram no matter dram address within miu0 or miu1
/// @param u32FlashAddr \b IN: Physical Source address in flash
/// @param u32DramAddr  \b IN: Physical Dst address in dram
/// @param u32Len       \b IN: data length
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
BDMA_Result _MDrv_BDMA_FlashCopy2Dram(MS_PHY phy64FlashAddr, MS_PHY phy64DramAddr, MS_U32 u32Len)
{
    MS_U8 eDst = (MS_U8)_BDMA_Check_MIUDev(E_BDMA_DEV_MIU0, phy64DramAddr);
    BDMA_CpyType eCpyType;

    eCpyType = (BDMA_CpyType)(E_BDMA_DEV_FLASH | _LShift(eDst, 8));
    return _MDrv_BDMA_CopyHnd(phy64FlashAddr, phy64DramAddr, u32Len, eCpyType, BDMA_OPCFG_DEF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Search
/// @brief \b Function  \b Description: Search pattern from MIU
/// @param phy64Addr    \b IN: Physical Start address
/// @param u32Len       \b IN: Search Pattern length, must > 0
/// @param u32Pattern   \b IN: Search Pattern data
/// @param u32ExcluBit  \b IN: Don't care bit filter
/// @param eDev         \b IN: source device for searching, valid sources are
/// @return             \b MS_U32 : matched address, if error, return 0xFFFFFFFF
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MDrv_BDMA_Search(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev)
{
    MS_U8 u8Ch;
    BDMA_OP_CB sOpCB;

    BDMA_DBG_FUNC();

    if (!_BDMA_Is_Init()){
        MDrv_BDMA_Init(0);
    }
    
    if ((!u32Len) || (E_BDMA_OK != _BDMA_Check_Device(eDev, E_BDMA_DSTDEV_SEARCH)))
    {
        BDMA_DBG_ERR("Critial error! Len:%lx!\n", u32Len);
        return 0xFFFFFFFF;
    }

    sOpCB.eAct = E_BDMA_ACT_SEARCH;
    sOpCB.phy64SrcAddr = phy64Addr;
    sOpCB.phy64DstAddr = 0;
    sOpCB.u32DataSize = u32Len;

    sOpCB.u8SrcDevCfg = _BDMA_GetDevCfg((BDMA_Dev)eDev);
    sOpCB.u8DstDevCfg = _BDMA_GetDevCfg(E_BDMA_DEV_SEARCH);
    sOpCB.u32Cmd0 = u32Pattern;
    sOpCB.u32Cmd1 = u32ExcluBit;
    sOpCB.u32Cmd2 = 0;

    sOpCB.u8OpCfg = BDMA_OPCFG_DEF;
    sOpCB.u8DmyWrCnt = 0;
    sOpCB.pCbf = (void *)0;

    if (E_BDMA_OK == _BDMA_CmnHnd(sOpCB, &u8Ch))
    {
        if (HAL_BDMA_Is_Found(u8Ch))
        {
            return HAL_BDMA_GetMatched(phy64Addr, u8Ch);
        }
        else
        {
            BDMA_DBG_INFO("Search not found!\n");
        }
    }

    return 0xFFFFFFFF;
}

#ifdef MOBF_ENABLE
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_MOBFSearch
/// @brief \b Function  \b Description: Search pattern from MIU by De-MOBF channel
/// @param u32Addr      \b IN: Physical Start address
/// @param u32Len       \b IN: Search Pattern length, must > 0
/// @param u32Pattern   \b IN: Search Pattern RAW data
/// @param u32MBOFKey \b IN: MOBF Key
/// @param eDev         \b IN: source device for searching, valid sources are
/// @return             \b MS_U32 : matched address, if error, return 0xFFFFFFFF
////////////////////////////////////////////////////////////////////////////////
MS_PHY _MDrv_BDMA_MOBFSearch(MS_PHY phy64Addr, MS_U32 u32Len, BDMA_MOBF_PS* pMobfPsCfg, BDMA_SrcDev eDev)
{
    MS_U8 u8Ch;
    MS_U32 phy64RetAddr = 0xFFFFFFFF;
    BDMA_OP_CB sOpCB;

    BDMA_DBG_FUNC();
    if (!_BDMA_Is_Init() || !u32Len || (E_BDMA_OK != _BDMA_Check_Device(eDev, E_BDMA_DSTDEV_SEARCH)))
    {
        BDMA_DBG_ERR("%s:Critial error! Len:%lx!\n", __FUNCTION__, u32Len);
        return phy64RetAddr;
    }

    sOpCB.eAct = E_BDMA_ACT_SEARCH;
    sOpCB.phy64SrcAddr = phy64Addr;
    sOpCB.phy64DstAddr = phy64Addr;
    sOpCB.u32DataSize = u32Len;

    sOpCB.u8SrcDevCfg = _BDMA_GetDevCfg((BDMA_Dev)eDev);
    sOpCB.u8DstDevCfg = _BDMA_GetDevCfg(E_BDMA_DEV_SEARCH);
    sOpCB.u32Cmd0 = pMobfPsCfg->u32Pattern;
    sOpCB.u32Cmd1 = pMobfPsCfg->u32ExcluBit;
    sOpCB.u32Cmd2 = pMobfPsCfg->u32MobfKey;

    sOpCB.u8OpCfg = BDMA_OPCFG_MOBF_PS;
    sOpCB.u8DmyWrCnt = 0;
    sOpCB.pCbf = (void *)0;

    if (E_BDMA_OK == _BDMA_CmnHnd(sOpCB, &u8Ch))
    {
        if (HAL_BDMA_Is_Found(u8Ch))
        {
            phy64RetAddr = (HAL_BDMA_GetMatched(phy64Addr, u8Ch)-phy64Addr);
        }
        else
        {
            BDMA_DBG_INFO("%s : Search not found!\n", __FUNCTION__);
        }
    }
    return phy64RetAddr;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_DumpCB
/// @brief \b Function  \b Description: Dump control block informaiton
/// @param *pvOpCB      \b IN: control block
/// @return             \b BDMA_Result
////////////////////////////////////////////////////////////////////////////////
void MDrv_BDMA_DumpCB(void *pvOpCB)
{
    MS_DEBUG_MSG(BDMA_OP_CB *psOpCB);

    if (E_BDMA_DBGLV_ALL != _gsInfo.eDbgLv)
    {
         return;
    }

    if (pvOpCB)
    {
        MS_DEBUG_MSG(psOpCB = (BDMA_OP_CB *)pvOpCB);
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tDump BDMA CB.......\n"));
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tAct: %u Src dev: 0x%x Dst dev: 0x%x\n",psOpCB->eAct, psOpCB->u8SrcDevCfg, psOpCB->u8DstDevCfg));
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tSrc addr: %lx Dst addr: %lx\n",psOpCB->phy64SrcAddr, psOpCB->phy64DstAddr));
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tData len: %lx\n",psOpCB->u32DataSize));
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tCmd0: %lx Cmd1: %lx Cmd2: %lx\n",psOpCB->u32Cmd0, psOpCB->u32Cmd1, psOpCB->u32Cmd2));
        MS_DEBUG_MSG(ULOGD(TAG_BDMA, "\tDmyWrCnt: %u OpCfg: %u Cbf:%lx\n",psOpCB->u8DmyWrCnt, psOpCB->u8OpCfg, (MS_U32)psOpCB->pCbf));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_CRC
/// @brief \b Function  \b Description: Crc32 for BDMA copy action
/// @param u32Addr      \b IN: Physical Start address
/// @param u32Len       \b IN: data length
/// @param u32Poly      \b IN: CRC Poly Ex: 0xEDB88320 or invert 0x04C11DB7
/// @param u32Seed      \b IN: Start value Ex: 0x00000000 or 0xFFFFFFFF
/// @param eDev         \b IN: Source device
/// @param bReflect     \b IN: TRUE: CRC reflection FALSE: No reflection
/// @return             \b MS_U32: result of CRC
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MDrv_BDMA_CRC32(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect)
{
    MS_U8 u8Ch;
    BDMA_OP_CB sOpCB;

    BDMA_DBG_FUNC();

    if ((!_BDMA_Is_Init()) || (E_BDMA_OK != _BDMA_Check_Device(eDev, E_BDMA_DSTDEV_CRC32)))
    {
        return 0xFFFFFFFF;
    }
    BDMA_DBG_INFO("BDMA CRC32 start....\n");
    sOpCB.eAct = E_BDMA_ACT_CRC32;
    sOpCB.phy64SrcAddr = phy64Addr;
    sOpCB.phy64DstAddr = 0;
    sOpCB.u32DataSize = u32Len;

    sOpCB.u8SrcDevCfg = _BDMA_GetDevCfg((BDMA_Dev)eDev);
    sOpCB.u8DstDevCfg = _BDMA_GetDevCfg(E_BDMA_DEV_CRC32);
    sOpCB.u32Cmd0 = u32Poly;
    sOpCB.u32Cmd1 = u32Seed;
	sOpCB.u32Cmd2 = 0;

    //only care reflection bit
    sOpCB.u8OpCfg = (bReflect) ? BDMA_OPCFG_CRC_REFLECT : BDMA_OPCFG_DEF;
    sOpCB.u8DmyWrCnt = 0;
    sOpCB.pCbf = (void *)0;

    if (E_BDMA_OK == _BDMA_CmnHnd(sOpCB, &u8Ch))
    {
        return HAL_BDMA_GetCRC32(u8Ch);
    }
    return 0xFFFFFFFF;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_GetInfo
/// @brief \b Function  \b Description: Get BDMA driver information
/// @return             \b const BDMA_Info *: bdma driver informaiton
////////////////////////////////////////////////////////////////////////////////
const BDMA_Info* MDrv_BDMA_GetInfo()
{
    if (!_BDMA_Is_Init())
    {
        return (BDMA_Info *)0;
    }
    _gsInfo.u8ChNum = E_BDMA_CH_END;
    _gsInfo.sHwCap = _gsHwInfo;
    return (&_gsInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_GetStatus
/// @brief \b Function  \b Description: Get BDMA driver information
/// @param pStatus      \b Out: bdma driver status
////////////////////////////////////////////////////////////////////////////////
void _MDrv_BDMA_GetStatus(BDMA_Status *pStatus)
{
    MS_U8 u8Ch = E_BDMA_CH0;

    if (pStatus==NULL)
    {
        return;
    }

    if (!_BDMA_Is_Init())
    {
        return;
    }
    for (; u8Ch <= E_BDMA_CH1; u8Ch++)
    {
        pStatus->sChSta[u8Ch].bIsBusy = HAL_BDMA_Is_Busy(u8Ch);
        pStatus->sChSta[u8Ch].bIsInt = HAL_BDMA_Is_Int(u8Ch);
        pStatus->sChSta[u8Ch].bIsFound = HAL_BDMA_Is_Found(u8Ch);
    }
    pStatus->bInit = _gsInfo.bInit;
    pStatus->eDbgLv = _gsInfo.eDbgLv;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_GetMinSize
/// @brief \b Function  \b Description: Get BDMA service min size
/// @param pStatus      \b Out: bdma driver status
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_BDMA_GetMinSize(void)
{
    return BDMA_MIN_SIZE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_SetSPIOffsetForMCU
/// @brief \b Function  \b Description: MCU Reset
/// @param pStatus      \b Out: non
////////////////////////////////////////////////////////////////////////////////
void _MDrv_BDMA_SetSPIOffsetForMCU(void)
{
   MS_PHY u32BaseSize = 0;
    if(!MDrv_MMIO_GetBASE(&_gsInfo.u32IOMap, &u32BaseSize, MS_MODULE_PM))
    {
        BDMA_DBG_ERR("Get IOMAP Base faill!\n");
        //return E_BDMA_FAIL;
    }
	HAL_BDMA_SetIOMapBase(_gsInfo.u32IOMap);

    HAL_BDMA_SetSPIOffsetForMCU();

	if(!MDrv_MMIO_GetBASE(&_gsInfo.u32IOMap, &u32BaseSize, MS_MODULE_BDMA))
    {
        BDMA_DBG_ERR("Get IOMAP Base faill!\n");
        //return E_BDMA_FAIL;
    }
	HAL_BDMA_SetIOMapBase(_gsInfo.u32IOMap);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Internal_Test
/// @brief \b Function  \b Description: Internal_Test
/// @param pStatus      \b Out: non
////////////////////////////////////////////////////////////////////////////////
#if 0
void MDrv_BDMA_Internal_Test(void)
{
    MS_U8 tmp;

    _BDMA_Get_FreeCh(0);
    _BDMA_MemFlush(E_BDMA_DEV_MIU1, 0, 0);
    _BDMA_WaitDone(E_BDMA_ACT_MAX, 0);
    BDMA_OP_CB sOpCB;
    _BDMA_CmnHnd(sOpCB, 0);
    _BDMA_GetHwInfo(0);
    _BDMA_Check_MIUDev(E_BDMA_DEV_FLASH, 0);
    _BDMA_Check_MIUDev(E_BDMA_DEV_MIU0, _gsInfo.phy64MIU1Base);
    _BDMA_Check_MIUDev(E_BDMA_DEV_MIU1, 0);
    _BDMA_Check_Device(E_BDMA_DEV_NOT_SUPPORT, E_BDMA_DEV_NOT_SUPPORT);
    _BDMA_Check_Device(E_BDMA_DEV_MIU1, E_BDMA_DEV_NOT_SUPPORT+1);

    tmp = _gsHwInfo.bEnMIU1;
    _gsHwInfo.bEnMIU1 = 0;
    _BDMA_Check_Device(E_BDMA_DEV_MIU1, E_BDMA_DEV_MIU1);
    _gsHwInfo.bEnMIU1 = tmp;

    tmp = _gsHwInfo.bEnDMDMCU;
    _gsHwInfo.bEnDMDMCU = 0;
    _BDMA_Check_Device(E_BDMA_DEV_MIU0, E_BDMA_DEV_DMDMCU);
    _gsHwInfo.bEnDMDMCU = tmp;

    tmp = _gsHwInfo.bEnDevDw;
    _gsHwInfo.bEnDevDw = 0;
    _BDMA_GetDevCfg(0);
    _gsHwInfo.bEnDevDw = tmp;

    tmp = _gsHwInfo.bEnMemFill;
    _gsHwInfo.bEnMemFill = 0;
    _gsInfo.bInit = TRUE;
    MDrv_BDMA_PatternFill(0, 0, 0, 0);
    _gsInfo.bInit = FALSE;
    _gsHwInfo.bEnMemFill = tmp;
}
#endif

// below are utopia10 interface and will call to utopia20 core
BDMA_Result MDrv_BDMA_MemCopy(MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len)
{
#if (BDMA_UTOPIA20)
    PBDMA_MEMCOPY_PARAM pMemCpyParam;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pMemCpyParam = (PBDMA_MEMCOPY_PARAM)malloc(sizeof(BDMA_MEMCOPY_PARAM));
    if (NULL == pMemCpyParam)
        return E_BDMA_FAIL;
    pMemCpyParam->phy64SrcAddr = phy64SrcAddr;
    pMemCpyParam->phy64DstAddr = phy64DstAddr;
    pMemCpyParam->u32Len = u32Len;
    if(UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_MemCopy,(void*)pMemCpyParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_MemCopy fail\n");
        free(pMemCpyParam);
        return E_BDMA_FAIL;
    }
    free(pMemCpyParam);
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_MemCopy(phy64SrcAddr,phy64DstAddr,u32Len);
#endif
}

MS_U32 MDrv_BDMA_Search(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev)
{
#if (BDMA_UTOPIA20)
    PBDMA_SEARCH_PARAM pSearchParam = NULL;
    MS_U32 u32SearchAddr=0;

    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pSearchParam = (PBDMA_SEARCH_PARAM)malloc(sizeof(BDMA_SEARCH_PARAM));
    if (NULL == pSearchParam)
        return E_BDMA_FAIL;
    pSearchParam->u32Addr = phy64Addr;
    pSearchParam->u32Len = u32Len;
    pSearchParam->u32Pattern = u32Pattern;
    pSearchParam->u32ExcluBit = u32ExcluBit;
    pSearchParam->eDev = eDev;
    pSearchParam->pSearchAddr = 0;
    UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_Search,(void*)pSearchParam);
    u32SearchAddr = pSearchParam->pSearchAddr;
	
    free(pSearchParam);
    return u32SearchAddr;
#else
    return _MDrv_BDMA_Search(phy64Addr, u32Len, u32Pattern, u32ExcluBit, eDev);
#endif
}

MS_U32 MDrv_BDMA_CRC32(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect)
{
#if (BDMA_UTOPIA20)
    PBDMA_CRC32_PARAM pCRCParam = NULL;
    MS_U32 u32CRC32;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pCRCParam = (PBDMA_CRC32_PARAM)malloc(sizeof(BDMA_CRC32_PARAM));
    if (NULL == pCRCParam)
        return E_BDMA_FAIL;
    pCRCParam->phy64Addr = phy64Addr;
    pCRCParam->u32Len = u32Len;
    pCRCParam->u32Poly = u32Poly;
    pCRCParam->u32Seed = u32Seed;
    pCRCParam->eDev = eDev;
    pCRCParam->bReflect = bReflect;
    pCRCParam->u32CRC32 = 0;
    if(UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_CRC32,(void*)pCRCParam) != UTOPIA_STATUS_SUCCESS)
    {
        free(pCRCParam);
        return 0xFFFFFFFF;
    }
    u32CRC32 = pCRCParam->u32CRC32;
    free(pCRCParam);
    return u32CRC32;
#else
    return _MDrv_BDMA_CRC32(phy64Addr, u32Len, u32Poly, u32Seed, eDev, bReflect);
#endif
}

BDMA_Result MDrv_BDMA_PatternFill(MS_PHY phy64Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev)
{
#if (BDMA_UTOPIA20)
    PBDMA_PATTERN_FILL_PARAM pPattFillParam = NULL;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pPattFillParam = (PBDMA_PATTERN_FILL_PARAM)malloc(sizeof(BDMA_PATTERN_FILL_PARAM));
    if (NULL == pPattFillParam)
        return E_BDMA_FAIL;
    pPattFillParam->u32Addr = phy64Addr;
    pPattFillParam->u32Len = u32Len;
    pPattFillParam->u32Pattern = u32Pattern;
    pPattFillParam->eDev = eDev;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_Pattern_Fill,(void*)pPattFillParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_PatternFill fail\n");
        free(pPattFillParam);
        return E_BDMA_FAIL;
    }
    free(pPattFillParam);
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_PatternFill(phy64Addr, u32Len, u32Pattern, eDev);
#endif
}

BDMA_Result MDrv_BDMA_FlashCopy2Dram(MS_PHY phy64FlashAddr, MS_PHY phy64DramAddr, MS_U32 u32Len)
{
#if (BDMA_UTOPIA20)
    PBDMA_FLASHCOPY2DRAM_PARAM pFlash2DramParam = NULL;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    pFlash2DramParam = (PBDMA_FLASHCOPY2DRAM_PARAM)malloc(sizeof(BDMA_FLASHCOPY2DRAM_PARAM));
    if (NULL == pFlash2DramParam)
        return E_BDMA_FAIL;
    pFlash2DramParam->u32FlashAddr = phy64FlashAddr;
    pFlash2DramParam->u32DramAddr = phy64DramAddr;
    pFlash2DramParam->u32Len = u32Len;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_FlashCopy2Dram,(void*)pFlash2DramParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_FlashCopy2Dram fail\n");
        free(pFlash2DramParam);
        return E_BDMA_FAIL;
    }
    free(pFlash2DramParam);
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_FlashCopy2Dram(phy64FlashAddr, phy64DramAddr, u32Len);
#endif
}

#ifdef MOBF_ENABLE
MS_PHY MDrv_BDMA_MOBFSearch(MS_PHY phy64Addr, MS_U32 u32Len, BDMA_MOBF_PS* pMobfPsCfg, BDMA_SrcDev eDev)
{
#if (BDMA_UTOPIA20)
    PBDMA_MOBFSEARCH_PARAM pMOBFSearchParam = NULL;
    MS_PHY phy64Ret;
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    if (NULL == pMobfPsCfg)
        return E_BDMA_FAIL;
    pMOBFSearchParam = (PBDMA_MOBFSEARCH_PARAM)malloc(sizeof(BDMA_MOBFSEARCH_PARAM));
    if (NULL == pMOBFSearchParam)
        return E_BDMA_FAIL;
    pMOBFSearchParam->phy64Addr = phy64Addr;
    pMOBFSearchParam->u32Len = u32Len;
    pMOBFSearchParam->pMobfPsCfg = pMobfPsCfg;
    pMOBFSearchParam->eDev = eDev;
    pMOBFSearchParam->phy64SearchAddr = 0;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_MOBFSearch,(void*)pMOBFSearchParam) != UTOPIA_STATUS_SUCCESS)
    {
        free(pMOBFSearchParam);
        return 0xFFFFFFFF;
    }
    phy64Ret = pMOBFSearchParam->phy64SearchAddr;
    free(pMOBFSearchParam);
    return phy64Ret;
#else
    return _MDrv_BDMA_MOBFSearch(u32Addr, u32Len, pMobfPsCfg, eDev);
#endif
}
#endif

void MDrv_BDMA_GetStatus(BDMA_Status *pStatus)
{
#if (BDMA_UTOPIA20)
    PBDMA_GETSTATUS_PARAM pGetStatusParam = NULL;
    if (NULL == pInstantBdma)
        return;
    pGetStatusParam = (PBDMA_GETSTATUS_PARAM)malloc(sizeof(BDMA_GETSTATUS_PARAM));
    if (NULL == pGetStatusParam)
        return;
    pGetStatusParam->pStatus = pStatus;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_GetStatus,(void*)pGetStatusParam) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_GetStatus fail\n");
    }
    free(pGetStatusParam);
#else
    _MDrv_BDMA_GetStatus(pStatus);
#endif
}

void MDrv_BDMA_SetSPIOffsetForMCU(void)
{
#if (BDMA_UTOPIA20)
    if (NULL == pInstantBdma)
        return;
    UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_SetSPIOffsetForMCU,(void*)NULL);
#else
    _MDrv_BDMA_SetSPIOffsetForMCU();
#endif
}

BDMA_Result MDrv_BDMA_WaitFlashDone(void)
{
#if (BDMA_UTOPIA20)
    if (NULL == pInstantBdma)
        return E_BDMA_FAIL;
    if (UtopiaIoctl(pInstantBdma,MDrv_CMD_BDMA_WaitFlashDone,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_BDMA, "Ioctl MDrv_BDMA_WaitFlashDone fail\n");
        return E_BDMA_FAIL;
    }
    return E_BDMA_OK;
#else
    return _MDrv_BDMA_WaitFlashDone();
#endif
}

MS_U32 MDrv_BDMA_SetPowerState(EN_POWER_MODE u16PowerState)
{
    ULOGD(TAG_BDMA, "[Test] %s\n", __FUNCTION__);
    static EN_POWER_MODE u16PreBDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        u16PreBDMAPowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreBDMAPowerState == E_POWER_SUSPEND)
        {
            MDrv_BDMA_Init(0);

            u16PreBDMAPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_BDMA, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_BDMA, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

