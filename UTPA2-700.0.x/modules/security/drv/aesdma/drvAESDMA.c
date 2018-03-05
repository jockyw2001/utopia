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
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// file    drvAESDMA.c
// @brief  AESDMA Driver
// @author MStar Semiconductor,Inc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsVersion.h"

#include "drvMMIO.h"
#include "halAESDMA.h"
#include "drvAESDMA.h"
#include "utopia.h"
#include "drvAESDMA_private.h"
#include "ULog.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_NOS)
#define AESDMAIntEnable()                       MsOS_EnableInterrupt(E_INT_IRQ_WADR_ERR);
#define AESDMAIntDisable()                      MsOS_DisableInterrupt(E_INT_IRQ_WADR_ERR);
#define AESDMAIntAttach()                       MsOS_AttachInterrupt(E_INT_IRQ_WADR_ERR, (InterruptCb )_AESDMA_Isr)
#define AESDMAIntDetach()                       MsOS_DetachInterrupt (E_INT_IRQ_WADR_ERR)
#endif //#if !defined(MSOS_TYPE_NOS)
#define AESDMAPA2KSEG1(addr)                    ((void *)(((MS_U32)addr) | 0xa0000000)) //physical -> unchched


//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_NOS)
#define AESDMA_TASK_STACK_SIZE 4096UL
#endif //#if !defined(MSOS_TYPE_NOS)

#define TAG_AESDMA "AESDMA"

static MS_PHY phyTmpFileInAddr = 0;
static MS_PHY phyTmpFileOutAddr = 0;
static MS_U32 u32TmpFileInNum = 0;

//--------------------------------------------------------------------------------------------------
//  Global Variable
//--------------------------------------------------------------------------------------------------
//#ifndef MSOS_TYPE_LINUX
static DrvAESDMA_Event                  _AESDMAEvtNotify;
static P_DrvAESDMA_EvtCallback          _AESDMACallback;
//#endif // #ifndef MSOS_TYPE_LINUX

#if !defined(MSOS_TYPE_NOS)
static void*                            _pAESDMATaskStack;
static MS_U8                            _u8AESDMA_StackBuffer[AESDMA_TASK_STACK_SIZE];
static MS_S32                           _s32AESDMATaskId;
static MS_U32                           _u32LastErr;
#endif //#if !defined(MSOS_TYPE_NOS)

static MS_PHY                         _u32MIU0_Addr, _u32MIU1_Addr , _u32MIU2_Addr, _u32MIU_num;
static DrvAESDMA_DbgLevel               _u32AESDMADBGLevel;
static MS_BOOL                          _bExitIsrLoop;

static MSIF_Version _drv_aesdma_version = {
    .DDI = { AESDMA_DRV_VERSION, },
};

#if (AESDMA_UTOPIA20)
void* pInstantAesdma = NULL;
void* pAttributeAesdma = NULL;
#endif

//--------------------------------------------------------------------------------------------------
//  Internal Variable
//--------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_NOS)
static MS_S32                           _s32AESDMAMutexId;
static MS_S32                           _s32AESDMAEventId;
#endif //#if !defined(MSOS_TYPE_NOS)
static MS_U8                            _u8IsInit = FALSE;
#ifndef MSOS_TYPE_NUTTX
static MS_BOOL                          bSecureModeEnable = FALSE;
#endif

#if !defined(MSOS_TYPE_NOS)
#define _AESDMA_SetEvent(flag)          MsOS_SetEvent(_s32AESDMAEventId, (flag))
#define _AESDMA_GetEvent(events)        MsOS_WaitEvent(_s32AESDMAEventId, AESDMA_STATES_GROUP, &events, E_OR_CLEAR, MSOS_WAIT_FOREVER)
#endif //#if !defined(MSOS_TYPE_NOS)

//--------------------------------------------------------------------------------------------------
//  Debug Function
//--------------------------------------------------------------------------------------------------
#define DEBUG_AES_DMA(debug_level, x)     do { if (_u32AESDMADBGLevel >= (debug_level)) (x); } while(0)

//--------------------------------------------------------------------------------------------------
//  Internal Function
//--------------------------------------------------------------------------------------------------
static MS_BOOL _AESDMA_Chk_MIU(MS_PHY *phy64InAddr, MS_U32 u32InLen, MS_PHY *phy64OutSAddr, MS_PHY *phyOutEAddr)
{

    if (_u32MIU_num == 3)
    {
        if ((_u32MIU1_Addr > *phy64InAddr) & (_u32MIU1_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr > *phy64OutSAddr) & (_u32MIU1_Addr > *phyOutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu0->miu0
            return TRUE;
        }

        if ((_u32MIU1_Addr > *phy64InAddr) & (_u32MIU1_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr < *phy64OutSAddr) & (_u32MIU1_Addr < *phyOutEAddr) & (_u32MIU2_Addr > *phy64OutSAddr) & (_u32MIU2_Addr > *phyOutEAddr))
        {
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU1_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(FALSE,TRUE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu0->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr > *phy64InAddr) & (_u32MIU1_Addr > *phy64InAddr+u32InLen) & (_u32MIU2_Addr <= *phy64OutSAddr) & (_u32MIU2_Addr <= *phyOutEAddr))
        {
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU2_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU2_Addr;
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,TRUE); // miu0->miu2
            return TRUE;
        }

        if ((_u32MIU1_Addr <= *phy64InAddr) & (_u32MIU1_Addr <= *phy64InAddr+u32InLen) & (_u32MIU2_Addr > *phy64InAddr) & (_u32MIU2_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr > *phy64OutSAddr) & (_u32MIU1_Addr > *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(TRUE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu1->miu0
            return TRUE;
        }
        
        if ((_u32MIU1_Addr <= *phy64InAddr) & (_u32MIU1_Addr <= *phy64InAddr+u32InLen) & (_u32MIU2_Addr > *phy64InAddr) & (_u32MIU2_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr <= *phy64OutSAddr) & (_u32MIU1_Addr <= *phyOutEAddr) & (_u32MIU2_Addr > *phy64OutSAddr) & (_u32MIU2_Addr > *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU1_Addr;
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU1_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(TRUE,TRUE);
            AESDMA_Set_MIU2_Path(FALSE,FALSE); // miu1->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr <= *phy64InAddr) & (_u32MIU1_Addr <= *phy64InAddr+u32InLen) & (_u32MIU2_Addr > *phy64InAddr) & (_u32MIU2_Addr > *phy64InAddr+u32InLen) & (_u32MIU2_Addr <= *phy64OutSAddr) & (_u32MIU2_Addr <= *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU1_Addr;
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU2_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU2_Addr;
            AESDMA_Set_MIU_Path(TRUE,FALSE);
            AESDMA_Set_MIU2_Path(FALSE,TRUE); // miu1->miu2
            return TRUE;
        }

        if ((_u32MIU2_Addr <= *phy64InAddr) & (_u32MIU2_Addr <= *phy64InAddr+u32InLen) & (_u32MIU1_Addr > *phy64OutSAddr) & (_u32MIU1_Addr > *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU2_Addr;
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(TRUE,FALSE); // miu2->miu0
            return TRUE;
        }

        if ((_u32MIU2_Addr <= *phy64InAddr) & (_u32MIU2_Addr <= *phy64InAddr+u32InLen) & (_u32MIU1_Addr <= *phy64OutSAddr) & (_u32MIU1_Addr <= *phyOutEAddr) & (_u32MIU2_Addr > *phy64OutSAddr) & (_u32MIU2_Addr > *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU2_Addr;
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU1_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(FALSE,TRUE);
            AESDMA_Set_MIU2_Path(TRUE,FALSE); // miu2->miu1
            return TRUE;
        }

        if ((_u32MIU2_Addr <= *phy64InAddr) & (_u32MIU2_Addr <= *phy64InAddr+u32InLen) & (_u32MIU2_Addr <= *phy64OutSAddr) & (_u32MIU2_Addr <= *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU2_Addr;
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU2_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU2_Addr;
            AESDMA_Set_MIU_Path(FALSE,FALSE);
            AESDMA_Set_MIU2_Path(TRUE,TRUE); // miu2->miu2
            return TRUE;
        }
        return FALSE;
    }
    else if (_u32MIU_num == 2)
    {
        if ((_u32MIU1_Addr > *phy64InAddr) & (_u32MIU1_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr > *phy64OutSAddr) & (_u32MIU1_Addr > *phyOutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE); // miu0->miu0
            return TRUE;
        }

        if ((_u32MIU1_Addr < *phy64InAddr) & (_u32MIU1_Addr < *phy64InAddr+u32InLen) & (_u32MIU1_Addr < *phy64OutSAddr) & (_u32MIU1_Addr < *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU1_Addr;
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU1_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(TRUE,TRUE); // miu1->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr > *phy64InAddr) & (_u32MIU1_Addr > *phy64InAddr+u32InLen) & (_u32MIU1_Addr <= *phy64OutSAddr) & (_u32MIU1_Addr <= *phyOutEAddr))
        {
            *phy64OutSAddr = *phy64OutSAddr - _u32MIU1_Addr;
            *phyOutEAddr = *phyOutEAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(FALSE,TRUE); // miu0->miu1
            return TRUE;
        }

        if ((_u32MIU1_Addr <= *phy64InAddr) & (_u32MIU1_Addr <= *phy64InAddr+u32InLen) & (_u32MIU1_Addr > *phy64OutSAddr) & (_u32MIU1_Addr > *phyOutEAddr))
        {
            *phy64InAddr = *phy64InAddr - _u32MIU1_Addr;
            AESDMA_Set_MIU_Path(TRUE,FALSE); // miu1->miu0
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        if ((_u32MIU0_Addr < *phy64InAddr) & (_u32MIU0_Addr < *phy64InAddr+u32InLen) & (_u32MIU0_Addr < *phy64OutSAddr) & (_u32MIU0_Addr < *phyOutEAddr))
        {
            AESDMA_Set_MIU_Path(FALSE,FALSE); // miu0->miu0

        }
        return TRUE;
    }
}

#if !defined(MSOS_TYPE_NOS)
static void _AESDMA_Isr(void)
{
    MS_U32                 u32AESDMAStatus;

    // Direct register access
    u32AESDMAStatus = AESDMA_Get_AESDMA_Status();

    if (HAS_FLAG(u32AESDMAStatus, AESDMA_STATES_GROUP))
    {
        _AESDMA_SetEvent(HAS_FLAG(u32AESDMAStatus, AESDMA_STATES_GROUP)); // Trigger task
    }
    AESDMA_Clear_Int();
    AESDMA_Enable_Int();
    AESDMAIntEnable();
}

static void _AESDMA_Isr_Proc(void)
{
    MS_U32              u32Events;
    
    while ( _bExitIsrLoop == FALSE )
    {
        _AESDMA_GetEvent(u32Events);
        if (u32Events & AESDMA_STATES_GROUP)
        {
            if (HAS_FLAG(u32Events, AESDMA_STATES_GROUP) && _AESDMAEvtNotify && _AESDMACallback)
            {
                switch (u32Events & AESDMA_STATES_GROUP)
                {
                    case AESDMA_DMA_DONE:
                        if (HAS_FLAG(_AESDMAEvtNotify, E_DRVAESDMA_EVENT_DMA_DONE))
                        {
                            MsOS_ReadMemory();
                            _AESDMACallback(E_DRVAESDMA_EVENT_DMA_DONE);
                        }
                        break;
                    case AESDMA_DMA_PAUSE:
                        if (HAS_FLAG(_AESDMAEvtNotify, E_DRVAESDMA_EVENT_DMA_PAUSE))
                        {
                            _AESDMACallback(E_DRVAESDMA_EVENT_DMA_PAUSE);
                        }
                        break;
                   default:
                        break;
                }
            }
        }
    } // Task while loop
}
#endif //#if !defined(MSOS_TYPE_NOS)

MS_U32 _AESDMA_MSBF2Native(const MS_U8 u8Bytes[4])
{
    return (u8Bytes[0] << 24) | (u8Bytes[1] << 16) | (u8Bytes[2] << 8) | u8Bytes[3];
}

//--------------------------------------------------------------------------------------------------
//  Global Function
//--------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_NUTTX
DRVAESDMA_RESULT MDrv_AESDMA_SetSecurityInfo(DrvAESDMA_SecureInfo *pSecureInfo)
{
    bSecureModeEnable = pSecureInfo->bSecureModeEnable;        
    return DRVAESDMA_OK;
}
#endif

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_PHY phy64miu0addr , MS_PHY phy64miu1addr , MS_U32 u32miunum)
{
    MS_VIRT u32Bank, u32IRBank;
    MS_PHY u32BankSize;  // Non-PM bank
    MS_PHY u32IrBankSize;  // PM bank

    _bExitIsrLoop  = FALSE;
    
    if(_u8IsInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_BDMA))
        {
            MS_DEBUG_MSG(ULOGE(TAG_AESDMA, "MDrv_MMIO_GetBASE (NonPM base)fail\n"));
            MS_ASSERT(0);
        }

        if (FALSE == MDrv_MMIO_GetBASE(&u32IRBank, &u32IrBankSize, MS_MODULE_IR))
        {
            MS_DEBUG_MSG(ULOGE(TAG_AESDMA, "MDrv_MMIO_GetBASE (PM base)fail\n"));
            MS_ASSERT(0);
        }
        _u8IsInit = TRUE;
        HAL_AESDMA_SetBank(u32Bank, u32IRBank);
        AESDMA_Enable_Clk();
        _u32MIU_num = u32miunum;
        _u32MIU0_Addr = 0;
        if(_u32MIU_num == 2)
        {
            _u32MIU1_Addr = AESDMA_GetMIU1Base();
        }
        
        if(_u32MIU_num == 3)
        {
            _u32MIU1_Addr = AESDMA_GetMIU1Base();
            _u32MIU2_Addr = AESDMA_GetMIU2Base();
        }

        // ++ Release reset that asserted in the DC off
        // *(volatile MS_U32*)(0xbf800000+(0x2380<<2)) |= BIT15;
        // -- Release reset that asserted in the DC off

        #ifndef MSOS_TYPE_NUTTX    
        if(bSecureModeEnable == FALSE)
        {
            AESDMA_Reset();
        }
        #else
        AESDMA_Reset();
        #endif

#if !defined(MSOS_TYPE_NOS)
        _AESDMAEvtNotify = E_DRVAESDMA_EVENT_DATA_INIT;
        _AESDMACallback = NULL;

        AESDMA_HW_Patch();

        AESDMAIntDetach();
        AESDMAIntAttach();
        AESDMAIntEnable();

        AESDMA_Enable_Int();

        _s32AESDMAEventId = MsOS_CreateEventGroup("AESDMA_Event");
        if (_s32AESDMAEventId < 0)
        {
            MsOS_DeleteMutex(_s32AESDMAMutexId);
            _u32LastErr = DRVAESDMA_FAIL;
            return _u32LastErr;
        }

        _pAESDMATaskStack = _u8AESDMA_StackBuffer;
        _s32AESDMATaskId = MsOS_CreateTask((TaskEntry)_AESDMA_Isr_Proc,
                                        NULL,
                                        E_TASK_PRI_MEDIUM,
                                        TRUE,
                                        _pAESDMATaskStack,
                                        AESDMA_TASK_STACK_SIZE,
                                        "AESDMA_ISR_Task");
        if (_s32AESDMATaskId < 0)
        {
            MsOS_DeleteEventGroup(_s32AESDMAEventId);
            MsOS_DeleteMutex(_s32AESDMAMutexId);
            _u32LastErr = DRVAESDMA_FAIL;
            return _u32LastErr;
        }
#endif //#if !defined(MSOS_TYPE_NOS)
    }
    else
    {
        AESDMA_Enable_Clk();
        #ifndef MSOS_TYPE_NUTTX    
        if(bSecureModeEnable == FALSE)
        {
            AESDMA_Reset();
        }
        #else
        AESDMA_Reset();
        #endif
        
        _u8IsInit = TRUE;
    }

#if AESDMS_SECURED_BANK_EN
    AESDMA_Set_SecureBankEnable (TRUE);
#else
    AESDMA_Set_SecureBankEnable (FALSE);
#endif

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Init(MS_PHY u32miu0addr , MS_PHY u32miu1addr , MS_U32 u32miunum)
{
#if (AESDMA_UTOPIA20)
    AESDMA_INIT sAesdmaInit;

    sAesdmaInit.u32miu0addr = u32miu0addr;
    sAesdmaInit.u32miu1addr = u32miu1addr;
    sAesdmaInit.u32miunum = u32miunum;

#ifndef CONFIG_MBOOT
    if (pInstantAesdma != NULL) // prevent memory leak, and avoid strange issue in mboot after using same instance for a long time
    {
        UtopiaClose(pInstantAesdma);
    }
#endif

    if(UtopiaOpen(MODULE_AESDMA, &pInstantAesdma, 0, pAttributeAesdma) !=  UTOPIA_STATUS_SUCCESS)
    {
        pInstantAesdma = NULL;
        return DRVAESDMA_FAIL;
    }

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Init,(void*)&sAesdmaInit) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Init(u32miu0addr, u32miu1addr, u32miunum);
#endif
}
//--------------------------------------------------------------------------------------------------
/// Set key to the AESDMA
/// @param pu8Key \b IN: The key is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8Key. 16 for AES/TDES and 8 for DES
/// @return DRVAESDMA_OK - Success
/// @return DRVAESDMA_INVALID_PARAM - Invalid key length
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len)
{
    MS_U32 u32Tmp[4] = {0};

    if(pu8Key == NULL)
    {
        AESDMA_Sel_Key(TRUE); // key from KL
        return DRVAESDMA_OK;
    }

    // key from cipherkey
    if(u32Len == 16)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8Key);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8Key + 4);
        u32Tmp[1] = _AESDMA_MSBF2Native(pu8Key + 8);
        u32Tmp[0] = _AESDMA_MSBF2Native(pu8Key + 12);
    }
    else if(u32Len == 8)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8Key);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8Key + 4);
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Set_CipherKey(u32Tmp);
    AESDMA_Sel_Key(FALSE);

    return DRVAESDMA_OK;
}

//--------------------------------------------------------------------------------------------------
/// Set IV to the AESDMA
/// @param pu8IV \b IN: The IV is represented as vectors of bytes with the MSB first
/// @param u32Len \b IN: The byte length of the pu8IV. 16 for AES and 8 for TDES/DES
/// @return DRVAESDMA_OK - Success
/// @return DRVAESDMA_INVALID_PARAM - Invalid IV length
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len)
{
    MS_U32 u32Tmp[4] = {0};

    if(pu8IV == NULL)
    {
        return DRVAESDMA_OK;
    }

    if(u32Len == 16)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8IV);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8IV + 4);
        u32Tmp[1] = _AESDMA_MSBF2Native(pu8IV + 8);
        u32Tmp[0] = _AESDMA_MSBF2Native(pu8IV + 12);
    }
    else if(u32Len == 8)
    {
        u32Tmp[3] = _AESDMA_MSBF2Native(pu8IV);
        u32Tmp[2] = _AESDMA_MSBF2Native(pu8IV + 4);
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Set_InitVector(u32Tmp);

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
    if (pCipherKey != NULL)
    {
        AESDMA_Set_CipherKey(pCipherKey);
        AESDMA_Sel_Key(FALSE); // key from cipherkey
    }
    else
    {
        AESDMA_Sel_Key(TRUE); // key from KL
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetKey(MS_U32 *pCipherKey)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetKey,(void*)pCipherKey) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetKey(pCipherKey);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType)
{
    AESDMA_Parser_Enable_Two_Keys(TRUE);
    switch (stKeyType)
    {
    case E_DRVAESDMA_ODD_KEY:
        AESDMA_Set_CipherOddKey(pCipherKey);
        break;

    case E_DRVAESDMA_EVEN_KEY:
        AESDMA_Set_CipherEvenKey(pCipherKey);
        break;

    default:
        break;
    }

    return DRVAESDMA_OK;
}

//--------------------------------------------------------------------------------------------------
/// Set Odd/Even Key to the AESDMA
/// @param pCipherKey \b IN: The key is represented as vectors of 4-bytes
/// @param stKeyType \b IN: The key type of input key.
/// @return DRVAESDMA_OK - Success
//--------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_AESDMA_SetKeySel(MS_U32 *pCipherKey, DrvAESDMA_KEY_TYPE stKeyType)
{
#ifndef MSOS_TYPE_NUTTX
    if(bSecureModeEnable)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        AESDMA_Parser_Enable_Two_Keys(TRUE);
        return _MDrv_AESDMA_SetKeySel(pCipherKey, stKeyType);
    }
#else
    AESDMA_Parser_Enable_Two_Keys(TRUE);
    return _MDrv_AESDMA_SetKeySel(pCipherKey, stKeyType);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector)
{
    if (pInitVector != NULL)
    {
        AESDMA_Set_OddInitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddIV(MS_U32 *pInitVector)
{
#ifndef MSOS_TYPE_NUTTX
    if(bSecureModeEnable)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        if (NULL == pInstantAesdma)
            return DRVAESDMA_FAIL;

        if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_SetOddIV, (void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
        {
            return DRVAESDMA_FAIL;
        }
        return DRVAESDMA_OK;
    }
#else
    return _MDrv_AESDMA_SetOddIV(pInitVector);
#endif
}


DRVAESDMA_RESULT MDrv_AESDMA_SetSecureKey(void)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetSecureKey,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_Sel_SecureKey();
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void)
{
    AESDMA_Sel_SecureKey();
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
    if (pInitVector != NULL)
    {
        AESDMA_Set_InitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetIV(MS_U32 *pInitVector)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetIV,(void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
    {
        MS_DEBUG_MSG(printf("Ioctl MDrv_AESDMA_SetIV fail\n"));
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetIV(pInitVector);
#endif
}

DRVAESDMA_RESULT MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode)
{
    return AESDMA_QueryCipherMode(eMode);
}

DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
    switch (eMode)
    {
        case E_DRVAESDMA_CIPHER_CTR:
        case E_DRVAESDMA_CIPHER_DES_CTR:
        case E_DRVAESDMA_CIPHER_TDES_CTR:
	        AESDMA_Set_CIPHER_ENG((AESDMA_CipherMode)eMode, FALSE);
            break;
        case E_DRVAESDMA_CIPHER_ECB:
        case E_DRVAESDMA_CIPHER_CBC:
        case E_DRVAESDMA_CIPHER_DES_ECB:
        case E_DRVAESDMA_CIPHER_DES_CBC:
        case E_DRVAESDMA_CIPHER_TDES_ECB:
        case E_DRVAESDMA_CIPHER_TDES_CBC:
        case E_DRVAESDMA_CIPHER_CTS_CBC:
        case E_DRVAESDMA_CIPHER_CTS_ECB:
        case E_DRVAESDMA_CIPHER_DES_CTS_CBC:
        case E_DRVAESDMA_CIPHER_DES_CTS_ECB:
        case E_DRVAESDMA_CIPHER_TDES_CTS_CBC:
        case E_DRVAESDMA_CIPHER_TDES_CTS_ECB:
            AESDMA_Set_CIPHER_ENG((AESDMA_CipherMode)eMode, bDescrypt);
            break;
        default:
            break;
    }
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt)
{
#if (AESDMA_UTOPIA20)
    AESDMA_SEL_ENG SelEngParam;
    SelEngParam.eMode= eMode;
    SelEngParam.bDescrypt= bDescrypt;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SelEng,(void*)&SelEngParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SelEng(eMode,bDescrypt);
#endif
    }

DRVAESDMA_RESULT MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
#if (AESDMA_UTOPIA20)
    AESDMA_SET_PS SetPsParam;
    SetPsParam.u32PTN = u32PTN;
    SetPsParam.u32Mask = u32Mask;
    SetPsParam.bPSin_Enable = bPSin_Enable;
    SetPsParam.bPSout_Enable = bPSout_Enable;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetPS,(void*)&SetPsParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_Set_PS_PTN(u32PTN);
    AESDMA_Set_PS_Mask(u32Mask);
    AESDMA_Set_PS_ENG(bPSin_Enable, bPSout_Enable);
    AESDMA_Enable_Int();
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable)
{
    AESDMA_Set_PS_PTN(u32PTN);
    AESDMA_Set_PS_Mask(u32Mask);
    AESDMA_Set_PS_ENG(bPSin_Enable, bPSout_Enable);
    AESDMA_Enable_Int();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_PSRelease(void)
{    
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PSRelease,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_Set_PS_Release();
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void)
{
    AESDMA_Set_PS_Release();
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_PHY phy64FileinAddr, MS_U32 u32FileInNum, MS_PHY phy64FileOutSAddr, MS_PHY phy64FileOutEAddr)
{
    // check miu addr
    if (!_AESDMA_Chk_MIU(&phy64FileinAddr, u32FileInNum, &phy64FileOutSAddr, &phy64FileOutEAddr))
    {
        return DRVAESDMA_MIU_ADDR_ERROR;
    }

    if ((phy64FileOutSAddr == 0) | (phy64FileOutEAddr == 0))
    {
        AESDMA_Set_FileinDesc(phy64FileinAddr , u32FileInNum);
    }
    else
    {
        AESDMA_Set_FileinDesc(phy64FileinAddr, u32FileInNum);
        AESDMA_Set_FileoutDesc(phy64FileOutSAddr, phy64FileOutEAddr);
    }

    phyTmpFileInAddr = phy64FileinAddr;
    u32TmpFileInNum = u32FileInNum;
    phyTmpFileOutAddr = phy64FileOutSAddr;

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetFileInOut(MS_PHY u32FileinAddr, MS_U32 u32FileInNum, MS_PHY u32FileOutSAddr, MS_PHY u32FileOutEAddr)
{
#if (AESDMA_UTOPIA20)
    AESDMA_FILE_INOUT FileInOutParam;
    FileInOutParam.u32FileinAddr= u32FileinAddr;
    FileInOutParam.u32FileInNum= u32FileInNum;
    FileInOutParam.u32FileOutSAddr= u32FileOutSAddr;
    FileInOutParam.u32FileOutEAddr= u32FileOutEAddr;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetFileInOut,(void*)&FileInOutParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_SetFileInOut(u32FileinAddr,u32FileInNum,u32FileOutSAddr,u32FileOutEAddr);
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart)
{
    if (!MsOS_Dcache_Flush(MsOS_PA2KSEG0(phyTmpFileOutAddr), u32TmpFileInNum))
        MsOS_Dcache_Flush(MsOS_PA2KSEG0(phyTmpFileOutAddr) & ~((MS_VIRT)0x0F), u32TmpFileInNum+32);

    MsOS_FlushMemory();
    AESDMA_Start(bStart);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Start(MS_BOOL bStart)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Start,(void*)&bStart) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_Start(bStart);
#endif
}

DRVAESDMA_RESULT MDrv_AESDMA_Reset(void)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Reset,NULL) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }

    return DRVAESDMA_OK;
#else
    AESDMA_Reset();
    AESDMA_HW_Patch();
    AESDMA_Enable_Int();
    AESDMA_Parser_Enable_HWParser(FALSE);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_NormalReset(void)
{
#ifndef MSOS_TYPE_NUTTX
    if(bSecureModeEnable)
    {
        return DRVAESDMA_OK;
    }
    else
    {
        AESDMA_NormalReset();
        AESDMA_HW_Patch();
        AESDMA_Enable_Int();
        AESDMA_Parser_Enable_HWParser(FALSE);
        return DRVAESDMA_OK;
    }
#else
    AESDMA_NormalReset();
    AESDMA_HW_Patch();
    AESDMA_Enable_Int();
    AESDMA_Parser_Enable_HWParser(FALSE);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void)
{
    AESDMA_Reset();
    AESDMA_HW_Patch();
    AESDMA_Enable_Int();
    AESDMA_Parser_Enable_HWParser(FALSE);
    return DRVAESDMA_OK;
}
MS_U32 MDrv_AESDMA_GetStatus(void)
{
#if (AESDMA_UTOPIA20)
    MS_U32 u32RetVal = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_GetStatus,(void*)&u32RetVal) != UTOPIA_STATUS_SUCCESS)
    {
        return 0;
    }
    return u32RetVal;
#else
    return AESDMA_Get_AESDMA_Status();
#endif
}
DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(MS_U32 *u32RetVal)
{
    *u32RetVal = AESDMA_Get_AESDMA_Status();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(MS_U32 *u32RetVal)
{
    if(AESDMA_Get_AESDMA_IsFinished())
    {
        *u32RetVal = DRVAESDMA_OK;
        MsOS_ReadMemory();
        return DRVAESDMA_OK;
    }
    else
    {
        *u32RetVal = DRVAESDMA_FAIL;	
        return DRVAESDMA_FAIL;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_IsFinished(void)
{
#if (AESDMA_UTOPIA20)
    MS_U32 u32TimeOut = AES_MAX_TIMEOUT_VALUE;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_IsFinished, (void*)&u32TimeOut) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_IsFinished();
#endif
}

MS_U32 MDrv_AESDMA_GetPSMatchedByteCNT(void)
{
#if (AESDMA_UTOPIA20)
    MS_U32 u32Count = 0x000;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_GetPSMatchedByteCNT, (void*)&u32Count) != UTOPIA_STATUS_SUCCESS)
    {
        return 0;
    }

    return u32Count;
#else
    return AESDMA_Get_PS_MatchedBytecnt();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(MS_U32 *u32RetVal)
{
	MS_U32 u32Val = AESDMA_Get_PS_MatchedBytecnt();
	*u32RetVal = u32Val;
    return u32Val;
}

MS_U32 MDrv_AESDMA_GetPSMatchedPTN(void)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    return UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_GetPSMatchedPTN,NULL);
#else
    return AESDMA_Get_PS_MatchedPTN();
#endif
}

MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void)
{
    return AESDMA_Get_PS_MatchedPTN();
}

DRVAESDMA_RESULT MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
#if (AESDMA_UTOPIA20)
    AESDMA_NOTIFY stNotifyParam;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    stNotifyParam.eEvents = eEvents;
    stNotifyParam.pfCallback = pfCallback;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Notify, (void*)&stNotifyParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }

    _AESDMAEvtNotify = eEvents;
    _AESDMACallback = pfCallback;

    return DRVAESDMA_OK;
#else
    if (pfCallback)
    {
        AESDMA_Enable_Int();
    }
    else
    {
        AESDMA_Disable_Int();
    }

    _AESDMAEvtNotify = eEvents;
    _AESDMACallback = pfCallback;

    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback)
{
    if (pfCallback)
    {
        AESDMA_Enable_Int();
    }
    else
    {
        AESDMA_Disable_Int();
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel)
{
    _u32AESDMADBGLevel = DbgLevel;
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVAESDMA_FAIL;
    }

    *ppVersion = &_drv_aesdma_version;

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
#if (AESDMA_UTOPIA20)

    AESDMA_RAND stAesdmaRand;
    stAesdmaRand.u32PABuf = u32PABuf;
    stAesdmaRand.u32Size = u32Size;
	
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_Rand, (void*)&stAesdmaRand) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    MS_U32 Index;
    MS_U8 *u8VABuf = (MS_U8 *)MsOS_PA2KSEG1((MS_U32)u32PABuf);

    for( Index = 0; Index < u32Size; Index++ )
    {
        *(u8VABuf + Index) = AESDMA_Rand();
    }
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size)
{
    MS_U32 Index;
    ptrdiff_t n = (ptrdiff_t)u32PABuf;
    MS_U8 *u8VABuf = (MS_U8 *)MsOS_PA2KSEG1((MS_PHY)n);

    for( Index = 0; Index < u32Size; Index++ )
    {
        *(u8VABuf + Index) = AESDMA_Rand();
    }
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_Get_Rand_Num(MS_U8 *pu8Num)
{

    *pu8Num = AESDMA_Rand();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_SetClk,(void*)&benable) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    if(benable)
    {
        AESDMA_Enable_Clk();
    }
    else
    {
        AESDMA_Disable_Clk();
    }
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_Set_Clk(MS_BOOL benable)
{
    if(benable)
    {
        AESDMA_Enable_Clk();
    }
    else
    {
        AESDMA_Disable_Clk();
    }

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY phy64PAInBuf, MS_U32 u32Size, MS_PHY phy64PAOutBuf)
{
    MS_U8 *u8scattergather_format = 0; //[B7, B6, B5, B4] = MIU byte address;[B3, B2, B1, B0] = the length of this message block
    MS_PHY u32scattergather_Address = 0;
    MS_U32 u32OutOffset = 0;
    MS_U32 u32timeout_count = 0;
    MS_PHY phy64PAOutBufE;


    switch (eMode)
    {
    case E_AESDMA_SHA1:
        u32OutOffset = 20; //Out 160-bit
        break;

    case E_AESDMA_SHA256:
        u32OutOffset = 32; //Out 256-bit
        break;

    default:
        return DRVAESDMA_INVALID_PARAM;
    }
#if 1
    // check miu addr
    phy64PAOutBufE = (phy64PAOutBuf + u32OutOffset);
    if (!_AESDMA_Chk_MIU(&phy64PAInBuf, u32Size, &phy64PAOutBuf, &phy64PAOutBufE))
    {
        DEBUG_AES_DMA(E_DRVAESDMA_DBG_L1, ULOGE(TAG_AESDMA, "@@@DRVAESDMA_MIU_ADDR_ERROR\n"));
        return DRVAESDMA_MIU_ADDR_ERROR;
    }
#endif
    // Clear all SHA setting(clear the last outputs)
    SHA_Reset();

    // Error handler for 16-byte alignment limitation
    if(phy64PAInBuf & WORD_MASK)
    {
        DEBUG_AES_DMA(E_DRVAESDMA_DBG_L1, ULOGE(TAG_AESDMA, "Input Address should be 16-byte alignment\n"));
        return DRVAESDMA_FAIL;
    }

    u32scattergather_Address = ALIGN_8(phy64PAInBuf + u32Size);

    u32Size /= SHA_UNIT_SIZE;
    phy64PAInBuf /= SHA_UNIT_SIZE;

    if(phy64PAInBuf > _u32MIU1_Addr)
    {
        SHA_SetAddress((u32scattergather_Address | SHA_MIU1_ENABLE) / SHA_UNIT_SIZE);
        phy64PAInBuf = phy64PAInBuf | SHA_MIU1_ENABLE;
    }
    else
    {
        SHA_SetAddress((u32scattergather_Address) / SHA_UNIT_SIZE);
    }

    SHA_SetLength(u32Size);

    u8scattergather_format = (MS_U8 *)MsOS_PA2KSEG1(u32scattergather_Address);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x00)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x08)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x10)&0xFF);
    *(u8scattergather_format++) = (MS_U8)((u32Size>>0x18)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((phy64PAInBuf)>>0x00)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((phy64PAInBuf)>>0x08)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((phy64PAInBuf)>>0x10)&0xFF);
    *(u8scattergather_format++) = (MS_U8)(((phy64PAInBuf)>>0x18)&0xFF);

    SHA_SelMode((AESDMA_SHAMode)eMode);
    MsOS_DelayTaskUs(40);

    MsOS_FlushMemory();

    MsOS_DelayTaskUs(200);

    SHA_Start(1);

    while ((SHA_Get_Status() & SHARNG_CTRL_SHA_READY) == 0)
    {
        MsOS_DelayTaskUs(10);
        u32timeout_count++;
#if (SHA_WAIT==1)
        //500ms timeout
        if(u32timeout_count>SHA_TIMEOUT_VALUE)
        {
            SHA_Reset();
            return DRVAESDMA_FAIL;
        }
#endif
    }

    SHA_Out(MsOS_PA2KSEG1(phy64PAOutBuf));
    while ((SHA_Get_Status() & SHARNG_CTRL_SHA_BUSY) != 0)
        ;
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY u32PAInBuf, MS_U32 u32Size, MS_PHY u32PAOutBuf)
{
#if (AESDMA_UTOPIA20)
    SHA_CALCULATE ShaCalParam;
    ShaCalParam.eMode= eMode;
    ShaCalParam.u32PAInBuf= u32PAInBuf;
    ShaCalParam.u32Size= u32Size;
    ShaCalParam.u32PAOutBuf= u32PAOutBuf;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_SHA_Calculate,(void*)&ShaCalParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_SHA_Calculate(eMode,u32PAInBuf,u32Size,u32PAOutBuf, NULL);
#endif    
}

DRVAESDMA_RESULT MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV)
{

    //not implement yet
    return DRVAESDMA_OK;
}



#ifdef MOBF_ENABLE
//-------------------------------------------------------------------------------------------------
/// MOBF BDMA without any Encrypt (sw patch)
///

/// @return AESDMA_Result
/// @note Must be called after MDrv_AESDMA_SelEng(E_DRVAESDMA_CIPHER_DES_CTR,1)
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_DmaOnly(void)
{
    MOBF_DisableTDES();
    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF encrypt (from AESDMA to MIU)
/// @param  u32Key                \b IN: Encrypt Key
/// @param  bEnable                \b IN: Encrypt Enable/disable

/// @return AESDMA_Result
/// @note
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    MOBF_WriteKey(u32Key);
    MOBF_WriteEnable(bEnable);

    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF decrypt (from MIU to AESDMA)
/// @param  u32Key                \b IN: Decrypt Key
/// @param  bEnable                \b IN: Decrypt Enable/disable

/// @return AESDMA_Result
/// @note
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable)
{
    MOBF_ReadKey(u32Key);
    MOBF_ReadEnable(bEnable);

    return DRVAESDMA_OK;
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF One Way
/// @return AESDMA_Result
/// @note disable read mobf key
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT MDrv_MOBF_OneWay(void)
{
    MOBF_OneWay();
    return DRVAESDMA_OK;
}

#endif

//-------------------------------------------------------------------------------------------------
/// Mask Scrmb bit(for decrypt using)
/// @param  bEnable                \b IN: Mask Scrmb Enable/disable
/// @return AESDMA_Result
/// @note mask Scrmb bit
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{    
    AESDMA_Parser_Mask_Scrmb(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable)
{
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_MASKSCRMB,(void*)&bEnable) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Mask_Scrmb(bEnable);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Scrmb Pattern
/// @param  ePattern                \b IN: input Scrmb pattern(10/11)
/// @return AESDMA_Result
/// @note set Scrmb Pattern
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    AESDMA_Parser_Set_ScrmbPattern(ePattern);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_SETSCRMBPATTERN,(void*)&ePattern) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_ScrmbPattern(ePattern);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Scrmb Pattern
/// @param  ePattern                \b IN: input Scrmb pattern(10/11)
/// @return AESDMA_Result
/// @note set Scrmb Pattern
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
    AESDMA_Parser_Set_AddedScrmbPattern(ePattern);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_SETADDEDSCRMBPATTERN,(void*)&ePattern) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_AddedScrmbPattern(ePattern);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Query Pid Count
/// @return Pid Count
/// @note query pid count
//-------------------------------------------------------------------------------------------------
MS_U8 _MDrv_AESDMA_Parser_QueryPidCount(MS_U8* retCount)
{
    *retCount = AESDMA_Parser_Query_PidCount();
    return *retCount;
}

MS_U8 MDrv_AESDMA_Parser_QueryPidCount(void)
{
#if (AESDMA_UTOPIA20)

    MS_U8 mCount = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_QUERYPIDCOUNT,(void*)&mCount) != UTOPIA_STATUS_SUCCESS)
    {
        return mCount;
    }
    return mCount;

#else
    return AESDMA_Parser_Query_PidCount();
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Pid
/// @param  u8Index                 \b IN: Pid Index(0/1)
/// @param  u16Pid                   \b IN: Pid Value(max value is 0x1FFF)
/// @return AESDMA_Result
/// @note set Pid
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
    MS_U8 u8PidCount;
    // check Pid value
    if (u16Pid > AESDMA_PARSER_PID_MAX)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    // check Pid count
    u8PidCount = AESDMA_Parser_Query_PidCount();
    if (u8Index >= u8PidCount)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Parser_Set_Pid(u8Index,u16Pid);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid)
{
#if (AESDMA_UTOPIA20)
    AESDMA_PARSER_PID_CONFIGURATIONS mConfig;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    mConfig.u8PidIndex = u8Index;
    mConfig.u16Pid = u16Pid;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_SETPID,(void*)&mConfig) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    MS_U8 u8PidCount;
    // check Pid value
    if (u16Pid > AESDMA_PARSER_PID_MAX)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    // check Pid count
    u8PidCount = AESDMA_Parser_Query_PidCount();
    if (u8Index >= u8PidCount)
    {
        return DRVAESDMA_INVALID_PARAM;
    }

    AESDMA_Parser_Set_Pid(u8Index,u16Pid);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Bypass Pid
/// @param  bEnable                \b IN: Bypass Pid Enable/disable
/// @return AESDMA_Result
/// @note bypass Pid
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
    AESDMA_Parser_Bypass_Pid(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable)
{
#if (AESDMA_UTOPIA20)

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_BYPASSPID,(void*)&bEnable) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Bypass_Pid(bEnable);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Init Parser Encrypt
/// @param  eMode                \b IN: input parser mode
/// @return AESDMA_Result
/// @note init parser encrypt
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    switch (eMode)
    {
    case E_DRVAESDMA_PARSER_TS_PKT192:
    case E_DRVAESDMA_PARSER_TS_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_TS_PKT188:
    case E_DRVAESDMA_PARSER_TS_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(TRUE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;

    case E_DRVAESDMA_PARSER_HDCP20_PKT192:
    case E_DRVAESDMA_PARSER_HDCP20_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;
    }

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode)
{
#if (AESDMA_UTOPIA20)
    AESDMA_PARSER_CONFIGURATIONS stArgs;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    stArgs.eParserMode = eMode;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_ENCRYPT,(void*)&stArgs) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);
    switch (eMode)
    {
    case E_DRVAESDMA_PARSER_TS_PKT192:
    case E_DRVAESDMA_PARSER_TS_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_TS_PKT188:
    case E_DRVAESDMA_PARSER_TS_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(TRUE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;

    case E_DRVAESDMA_PARSER_HDCP20_PKT192:
    case E_DRVAESDMA_PARSER_HDCP20_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;
    }

    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Init Parser Decrypt
/// @param  eMode                \b IN: input parser mode
/// @return AESDMA_Result
/// @note init parser decrypt
//-------------------------------------------------------------------------------------------------
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);

    switch (eMode)
    {
    case E_DRVAESDMA_PARSER_TS_PKT192:
    case E_DRVAESDMA_PARSER_TS_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_TS_PKT188:
    case E_DRVAESDMA_PARSER_TS_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(TRUE);
        break;

    case E_DRVAESDMA_PARSER_HDCP20_PKT192:
    case E_DRVAESDMA_PARSER_HDCP20_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;
    }


    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode)
{
#if (AESDMA_UTOPIA20)
    AESDMA_PARSER_CONFIGURATIONS stArgs;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    stArgs.eParserMode = eMode;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_PARSER_DECRYPT,(void*)&stArgs) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;

#else
    AESDMA_Parser_Set_Mode((AESDMA_ParserMode)eMode);

    switch (eMode)
    {
    case E_DRVAESDMA_PARSER_TS_PKT192:
    case E_DRVAESDMA_PARSER_TS_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_TS_PKT188:
    case E_DRVAESDMA_PARSER_TS_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(TRUE);
        break;

    case E_DRVAESDMA_PARSER_HDCP20_PKT192:
    case E_DRVAESDMA_PARSER_HDCP20_PKT192_CLEAR:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188:
    case E_DRVAESDMA_PARSER_HDCP20_PKT188_CLEAR:
        AESDMA_Parser_Insert_Scrmb(FALSE);
        AESDMA_Parser_Remove_Scrmb(FALSE);
        break;
    }


    AESDMA_Parser_Enable_HWParser(TRUE);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
    MS_U32 u32KeyLen = 0;
    MS_U8  u8Public = 0;
    MS_U32 *pu32Sig = (MS_U32 *)((void*)pstSign);
    MS_U32 *pu32Key = (MS_U32 *)((void*)pstKey);


	RSA_Reset();
	RSA_Ind32Ctrl(1);//ind32_ctrl=0xE0

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
        {
            u32KeyLen = 128;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA1024_PRIVATE:
        {
            u32KeyLen = 128;
            u8Public = 0;
        }
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
        {
            u32KeyLen = 256;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA2048_PRIVATE:
        {
            u32KeyLen = 256;
            u8Public = 0;
        }
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
        {
            u32KeyLen = 32;
            u8Public = 1;
        }
        break;
    case E_DRVAESDMA_RSA256_PRIVATE:
        {
            u32KeyLen = 32;
            u8Public = 0;
        }
        break;
    default:
        return DRVAESDMA_INVALID_PARAM;
    }

	RSA_LoadSram((pu32Sig+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)-1),E_RSA_ADDRESS_A);
    RSA_LoadSram((pu32Key+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)-1),E_RSA_ADDRESS_N);
    RSA_LoadSram((pu32Key+(RSA_INPUT_SIZE/RSA_UNIT_SIZE)),E_RSA_ADDRESS_E);

    RSA_SetKeyLength((u32KeyLen/RSA_UNIT_SIZE)-1);
    RSA_SetKeyType(0,u8Public); //sw key, public key

	RSA_ExponetialStart();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode)
{
    return DRVAESDMA_FAIL;
}

DRVAESDMA_RESULT MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode)
{
#if (AESDMA_UTOPIA20)
    RSA_CALCULATE stRsaCalParam;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    stRsaCalParam.pstSign = pstSign;
    stRsaCalParam.pstKey = pstKey;
    stRsaCalParam.eMode= eMode;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_Calculate,(void*)&stRsaCalParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Calculate(pstSign,pstKey,eMode);
#endif    
}

DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void)
{
    if(!RSA_Get_RSA_IsFinished())
    {
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}

DRVAESDMA_RESULT MDrv_RSA_IsFinished(void)
{
#if (AESDMA_UTOPIA20)
    MS_U32 u32Timeout = 0x100000;

    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;
        
    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_IsFinished,(void*)&u32Timeout) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_IsFinished();
#endif    
}
DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
    MS_U32 i = 0;
    MS_U32 u32KeyLen = 0;

    MS_U32 *pu32RSAOut = (MS_U32 *)((void*)pstRSAOut);

    RSA_Ind32Ctrl(0);//ind32_ctrl=0xC0

    switch (eMode)
    {
    case E_DRVAESDMA_RSA1024_PUBLIC:
    case E_DRVAESDMA_RSA1024_PRIVATE:
        u32KeyLen = 128;
        break;
    case E_DRVAESDMA_RSA2048_PUBLIC:
    case E_DRVAESDMA_RSA2048_PRIVATE:
        u32KeyLen = 256;
        break;
    case E_DRVAESDMA_RSA256_PUBLIC:
    case E_DRVAESDMA_RSA256_PRIVATE:
        u32KeyLen = 32;
        break;
    default:
        return DRVAESDMA_INVALID_PARAM;
    }

	for( i = 0; i<(u32KeyLen/RSA_UNIT_SIZE); i++)
	{
		RSA_SetFileOutAddr(i);
		RSA_FileOutStart();
		*(pu32RSAOut+i) = RSA_FileOut();
	}

    //clear the used key whenever calculating is finished
    RSA_Reset();

    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut)
{
#if (AESDMA_UTOPIA20)
    RSA_OUTPUT RsaOutParam;
    RsaOutParam.eMode= eMode;
    RsaOutParam.pstRSAOut= pstRSAOut;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_RSA_Output,(void*)&RsaOutParam) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_RSA_Output(eMode,pstRSAOut);
#endif
}

MS_U32 MDrv_AESDMA_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE u16PreAESDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        u16PreAESDMAPowerState = u16PowerState;
	_u8IsInit = FALSE;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;

        AESDMA_POWER_SUSPEND();

#if !defined(MSOS_TYPE_NOS)
        MsOS_DeleteEventGroup(_s32AESDMAEventId);
        _AESDMA_SetEvent(AESDMA_DMA_PAUSE);
#endif //#if !defined(MSOS_TYPE_NOS)
        _bExitIsrLoop = TRUE;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (u16PreAESDMAPowerState == E_POWER_SUSPEND)
        {
            MDrv_AESDMA_Init(0, 0x20000000, 2);

            AESDMA_POWER_RESUME();

            u16PreAESDMAPowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_AESDMA, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_AESDMA, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

//reserved for secure protect function
/*
DRVAESDMA_RESULT MDrv_AESDMA_SecureSetKey(MS_U32 *pCipherKey)
{
    if (pCipherKey != NULL)
    {
        AESDMA_Secure_SetCipherKey(pCipherKey);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureProtect(MS_BOOL bEnable)
{
    AESDMA_SecureProtect(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureSetIV(MS_U32 *pInitVector)
{
    if (pInitVector != NULL)
    {
        AESDMA_Secure_SetInitVector(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetFileOutEnable(MS_BOOL bEnable)
{
    AESDMA_Set_FileOutEnable(bEnable);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureStart(MS_BOOL bStart)
{
    MsOS_FlushMemory();
    AESDMA_Secure_Start(bStart);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SecureIsFinished(void)
{
    if(AESDMA_Get_AESDMA_SecureIsFinished())
    {
        MsOS_ReadMemory();
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_FAIL;
    }
}
*/

DRVAESDMA_RESULT _MDrv_AESDMA_IsSecretKeyInNormalBank(MS_U8 *retVal)
{
    *retVal = AESDMA_IsSecretKeyInNormalBank ();
    if (*retVal == 0)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        return DRVAESDMA_OK;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_IsSecretKeyInNormalBank(void)
{

#if (AESDMA_UTOPIA20)

    MS_U8 retVal = 0;
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_IS_SECRETKEY_IN_NORMAL_BLANK,(void*)&retVal) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }

    if (retVal == 0)
        return DRVAESDMA_FAIL;
    else
        return DRVAESDMA_OK;


#else
    if (AESDMA_IsSecretKeyInNormalBank() == 0)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        return DRVAESDMA_OK;
    }
#endif
}

DRVAESDMA_RESULT MDrv_HDCP_ProcessCipher(MS_U8 u8Idx, MS_U8* pu8Riv, MS_U8 *pu8ContentKey)
{
    HDCP_ProcessCipher(u8Idx, pu8ContentKey, pu8Riv);

    return DRVAESDMA_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function           \b Name: MDrv_HDCP_GetHdcpCipherState 
/// @brief \b Function           \b Description: Check the capability of HDCP 2.2
/// @param None                  \b IN : u8Idx
/// @param None                  \b OUT : pu8State
/// @param DRVAESDMA_RESULT      \b RET : DRVAESDMA_OK
/// @param None                  \b GLOBAL :
////////////////////////////////////////////////////////////////////////////////
DRVAESDMA_RESULT MDrv_HDCP_GetHdcpCipherState(MS_U8 u8Idx, MS_U8 *pu8State)
{
    HDCP_GetHdcpCipherState(u8Idx, pu8State);
 
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey)
{
#if (AESDMA_UTOPIA20)
    AESDMA_KEY_INDEX KeyIndex;
    KeyIndex.bOddKeyEn = bIsOddKey;
    KeyIndex.u32KeyIndex = u32Index;
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_Set_Key_Index, (void*)&KeyIndex) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_SetKeyIndex (u32Index, bIsOddKey);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey)
{
    AESDMA_SetKeyIndex (u32Index, bIsOddKey);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT _MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable)
{
	AESDMA_Parser_Enable_Two_Keys(bEnable);
	return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_EnableTwoKeys(MS_BOOL bEnable)
{
#if (AESDMA_UTOPIA20)
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_ENABLE_Two_Key, (void*)&bEnable) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
	AESDMA_Parser_Enable_Two_Keys(bEnable);
	return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector)
{
    if (pInitVector != NULL)
    {
    	AESDMA_Set_CipherOddKey(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
}

DRVAESDMA_RESULT MDrv_AESDMA_SetOddKey(MS_U32 *pInitVector)
{
#if (AESDMA_UTOPIA20)
    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_SetOddKey,(void*)pInitVector) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    if (pInitVector != NULL)
    {
    	AESDMA_Set_CipherOddKey(pInitVector);
        return DRVAESDMA_OK;
    }
    else
    {
        return DRVAESDMA_INVALID_PARAM;
    }
#endif
}

DRVAESDMA_RESULT MDrv_AESDMA_ParserStart(MS_BOOL bStart)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma,MDrv_CMD_AESDMA_PARSER_Start,(void*)&bStart) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    return _MDrv_AESDMA_ParserStart(bStart);
#endif
}

DRVAESDMA_RESULT MDrv_HDMI_GetM0(MS_U32 u32InputPortType, MS_U8 *pu8Data)
{
    if (pu8Data == NULL)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        HAL_AESDMA_HDMI_GetM0(u32InputPortType, pu8Data);
        return DRVAESDMA_OK;
    }
}

DRVAESDMA_RESULT MDrv_Set_Kslot(MS_U32 u32InputPortType, MS_U8 *pu8Data)
{
    if (pu8Data == NULL)
    {
        return DRVAESDMA_FAIL;
    }
    else
    {
        HAL_AESDMA_HDMI_GetM0(u32InputPortType, pu8Data);
        return DRVAESDMA_OK;
    }
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetAesCtr64 (MS_BOOL bCtr64En)
{
    AESDMA_SET_CTR64(bCtr64En);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetAesCtr64 (MS_BOOL bCtr64En)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_Set_AES_CTR64_En, (void*)&bCtr64En) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_SET_CTR64(bCtr64En);
    return DRVAESDMA_OK;
#endif
}

DRVAESDMA_RESULT _MDrv_AESDMA_SetSecuredKeyIndex (MS_U8 u8KeyIdx)
{
    AESDMA_SetSecureKeyIdx (u8KeyIdx);
    return DRVAESDMA_OK;
}

DRVAESDMA_RESULT MDrv_AESDMA_SetSecuredKeyIndex (MS_U8 u8KeyIdx)
{
#if (AESDMA_UTOPIA20)
    if (NULL == pInstantAesdma)
        return DRVAESDMA_FAIL;

    if(UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_Set_Efuse_Key_Index, (void*)&u8KeyIdx) != UTOPIA_STATUS_SUCCESS)
    {
        return DRVAESDMA_FAIL;
    }
    return DRVAESDMA_OK;
#else
    AESDMA_SetSecureKeyIdx (u8KeyIdx);
    return DRVAESDMA_OK;
#endif
}

MS_BOOL MDrv_AESDMA_GetConfig(PAESDMA_INIT sAesdmaInit)
{
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
#if (AESDMA_UTOPIA20)
    if(NULL == pInstantAesdma)
    {
        if(UTOPIA_STATUS_FAIL == UtopiaOpen(MODULE_AESDMA, &pInstantAesdma, 0, NULL))
        {
            ULOGE("AESDMA", "%s, open MODULE_AESDMA failed\n", __FUNCTION__);
            return FALSE;
        }
    }
    u32Ret = UtopiaIoctl(pInstantAesdma, MDrv_CMD_AESDMA_GetConfig, (PAESDMA_INIT)sAesdmaInit);
    if(UTOPIA_STATUS_SUCCESS != u32Ret)
    {
        ULOGE("AESDMA", "[%s]ERROR : UtopiaIoctl return value 0x%x\n", __FUNCTION__, (unsigned int)u32Ret);
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#else
    return TRUE;
#endif
}
