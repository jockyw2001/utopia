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
#define _DRVNSK2_C

////////////////////////////////////////////////////////////////////////////////
/// file drvNSK2.c
/// @author MStar Semiconductor Inc.
/// @brief
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"

#include "drvNSK2.h"
#include "halNSK2.h"
#include "drvNSK2Type.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static MS_U32 _g32NSK2DbgLv = NSK2_DBGLV_DEBUG;

#define DRVNSK2_DBG(lv, x, args...)   if (lv <= _g32NSK2DbgLv ) \
                                        {printf(x, ##args);}

#define NSK2_TASK_STACK_SIZE        16*1024

#define NDS_NSK2_IRQ                E_INT_IRQ_CA_NSK_INT

#define NSK2_INT_EVENT_0            0x00000001
#define NSK2_INT_GROUP              0x0000FFFF

//#define ACPUTest
////////////////////////////////////////////////////////////////////////////////
// Local Variables
////////////////////////////////////////////////////////////////////////////////
static MS_U32  u32NSK2_Stack[NSK2_TASK_STACK_SIZE/sizeof(MS_U32)];
static MS_S32  s32NSK2_TaskId = -1;
static MS_S32  s32NSK2_EventId = -1;

static MS_BOOL _bNSKInit = FALSE;
static NSK2_IntNotify _gNs2IntNtf = NULL;

#ifdef ACPUTest
static MS_U32  u32NSK2IntStatus = 0;
#endif

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// External funciton
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////

static void _NSK2_Isr(void)
{
	MS_U32 status;
   	//MsOS_DisableInterrupt(NDS_NSK2_IRQ);

	status = HAL_NSK2_GetIntStatus();
#ifdef ACPUTest
    u32NSK2IntStatus = status;
    //printf("_NSK2_Isr, status = %x time = %d\n",u32NSK2IntStatus,MsOS_GetSystemTime());
#else
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"_NSK2_Isr, status = %x\n",status);
	HAL_NSK2_ClearInt(status);
    MsOS_SetEvent(s32NSK2_EventId, status);
#endif
}

static void _NSK2_Isr_Task(void)
{
    MS_U32              u32Reg;
    MS_U32              u32Events = 0;

    while(1)
    {
        MsOS_WaitEvent(s32NSK2_EventId, NSK2_INT_GROUP, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"s32NSK2_EventId = %d\n",s32NSK2_EventId);
		u32Reg = u32Events;
        if (NSK2_INT_ASYNC_EVENT != u32Reg)	//because frequent int's for this event
       	{
	       	if(_gNs2IntNtf != NULL)
	        {
	            _gNs2IntNtf(u32Reg, 0);
	        }
       	}

        MsOS_DelayTask(1);
		MsOS_EnableInterrupt(NDS_NSK2_IRQ);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////


MS_U32 MDrv_NSK2_Init(void)
{
    //init clock, register bank
    MS_VIRT u32Base;
	MS_PHY u32BankSize;

    if(TRUE == _bNSKInit)
        return TRUE;

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"enter %s \n", __FUNCTION__);

    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32BankSize, MS_MODULE_PM))
    {
        DRVNSK2_DBG(NSK2_DBGLV_EMERG,"%s, %d MDrv_MMIO_GetBASE (NonPM base)fail\n", __FUNCTION__,__LINE__);
        MS_ASSERT(0);
    }

    HAL_NSK2_SetBase((MS_U32)u32Base); //set non pm bank


    s32NSK2_EventId = MsOS_CreateEventGroup("NDS_NSK2_Event");
    if (s32NSK2_EventId < 0)
    {
        return FALSE;
    }

    s32NSK2_TaskId = MsOS_CreateTask((TaskEntry)_NSK2_Isr_Task,
                                    (MS_U32)NULL,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    u32NSK2_Stack,
                                    NSK2_TASK_STACK_SIZE,
                                    "NDS_NSK2_Task");
    if (s32NSK2_TaskId < 0)
    {
        MsOS_DeleteEventGroup(s32NSK2_EventId);
        return FALSE;
    }

    MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    MsOS_DetachInterrupt(NDS_NSK2_IRQ);
    MsOS_AttachInterrupt(NDS_NSK2_IRQ, (InterruptCb)_NSK2_Isr);
    MsOS_EnableInterrupt(NDS_NSK2_IRQ);

    _bNSKInit = TRUE;
    return TRUE;
}

MS_U32 MDrv_NSK2_Exit(void)
{
    return HAL_NSK2_Exit();
}

MS_U32 MDrv_NSK2_CtrlInt(MS_BOOL bEnable)
{
    if(TRUE == bEnable)
    {
        MsOS_EnableInterrupt(NDS_NSK2_IRQ);
    }
    else
    {
        MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    }

    return TRUE;
}

MS_U32 MDrv_NSK2_ColdReset(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_ColdReset();
    return TRUE;
}

MS_U32 MDrv_NSK2_EndSubtest(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_EndSubtest();
    return TRUE;
}


MS_U32 MDrv_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                         MS_U32 Mask, MS_U32 ExpectResult)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

#ifdef ACPUTest
    if(0xFC08 == StartAddr)
    {
        MS_U32 status;
        status = HAL_NSK2_Compare(StartAddr, CompareLens, CompareSim, Mask,  ExpectResult);
        HAL_NSK2_ClearInt(ExpectResult);
        MsOS_EnableInterrupt(NDS_NSK2_IRQ);
        return status;
    }
    else
        return HAL_NSK2_Compare(StartAddr, CompareLens, CompareSim, Mask,  ExpectResult);
#else
    return HAL_NSK2_Compare(StartAddr, CompareLens, CompareSim, Mask,  ExpectResult);
#endif


}

MS_U32 MDrv_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                            MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareMem(reserved, StartAddr, CompareLens, CompareSim, ExpectResult, pGolden);
}

MS_U32 MDrv_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens, void *pWriteData)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteMem(reserved,StartAddr,WriteLens,pWriteData);
    return TRUE;
}


MS_U32 MDrv_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteSFR(StartAddr, Data);
    return TRUE;
}

MS_U32 MDrv_NSK2_BasicInitializationComplete(void)
{
    //the chip should manager internal variables NSKactive and ConcurrencyMode
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_NSKBasicInitializationComplete();
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteESA(MS_U8 ESASelect, MS_U8 ESASubSelect, MS_U8 pid_no)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_WriteESA(ESASelect, ESASubSelect, pid_no);
    return TRUE;

}

MS_U32 MDrv_NSK2_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLabel, MS_U8 pid_no )
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"SCB = %x, ForceSCB = %x\n",SCB,ForceSCB);


    HAL_NSK2_WriteTransportKey(SCB,ForceSCB,pLabel,pid_no);
    return TRUE;
}

MS_U32 MDrv_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareKTE(reserved_1, reserved_2, pLabel);
}

MS_U32 MDrv_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    return HAL_NSK2_CompareOut( reserved_1, reserved_2, HighDWord, LowDWord);
}

MS_U32 MDrv_NSK2_SetRNG(MS_U32 reserved_1, MS_U16 RNG_Value)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    HAL_NSK2_SetRNG( reserved_1, RNG_Value);
    return TRUE;
}


MS_U32 MDrv_NSK2_DriveKteAck(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    HAL_NSK2_DriveKteAck();
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteSCPUKey(void)
{
    return HAL_NSK2_WriteSCPUKey();
}

MS_U32 MDrv_NSK2_WriteReservedKey(void)
{
    return HAL_NSK2_WriteReservedKey();
}

MS_U32 MDrv_NSK2_GetReserveKeyNum(void)
{
    return HAL_NSK2_GetReserveKeyNum();
}

MS_U32 MDrv_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, SubAlgo = %x \n", __FUNCTION__,SubAlgo);

    //can't find the algorithm meaning
    HAL_NSK2_WriteM2MKey(pIV,SubAlgo);
    return TRUE;
}

// TODO: need to remove and check by MW
MS_U32 MDrv_NSK2_FillJTagPswd(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_SetJTagPswd();
    return TRUE;
}

// TODO: need to remove and check by MW
MS_U32 MDrv_NSK2_CheckPubOTPConfig(void *pCheck)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CheckPubOTPConfig(pCheck);
}

// TODO:Check OTP control password here
MS_U32 MDrv_NSK2_UnlockOTPCtrl(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_UnlockOTPCtrl();
    return TRUE;
}


MS_U32 MDrv_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData32(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData32(addr_offset, data_size, data);
}


MS_U32 MDrv_NSK2_SetIntNotify(NSK2_IntNotify IntNotify)
{
    _gNs2IntNtf = IntNotify;
    return TRUE;
}


MS_U32 MDrv_NSK2_ExcuteCmd(MS_U32 command, MS_U32 control,
                           MS_U32 register_offset, MS_U32 data_size,
                           const MS_U8    *data)
{
    MS_U32 u32Status;
    HAL_NSK2_WriteControl(control);

    if(data_size != 0)
    {
        DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s data_size = %x\n", __FUNCTION__,data_size);
        u32Status = HAL_NSK2_WriteData8(register_offset, data_size, ( MS_U8 *)data);
        if(TRUE != u32Status)
        {
            DRVNSK2_DBG(NSK2_DBGLV_ERR,"%s, write excute cmd error", __FUNCTION__);
        }
    }

    HAL_NSK2_WriteCommand(command);
    return TRUE;
}




MS_U32 MDrv_NSK2_GetMaxXConn(void)
{
    return HAL_NSK2_GetMaxXConn();
}

MS_U32 MDrv_NSK2_CMChannelNum(void)
{
    return HAL_NSK2_CMChannelNum();
}

void MDrv_NSK2_SetDbgLevel(MS_U32 u32Level)
{
    _g32NSK2DbgLv = u32Level;
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s level: %x\n", __FUNCTION__, u32Level);
    HAL_NSK2_SetDbgLevel(u32Level);
}

void MDrv_NSK2_SetPollingCnt(MS_U32 u32Cnt)
{
    HAL_NSK2_SetPollingCnt(u32Cnt);
}

void MDrv_NSK2_RunFree(MS_BOOL bRunFree)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, bRunFree = %d\n", __FUNCTION__, bRunFree);

	HAL_NSK2_RunFree(bRunFree);
}

void MDrv_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 TenSecond)
{
	HAL_NSK2_PushSlowClock(HaltClk, TenSecond);
}

void MDrv_NSK2_GetRNGThroughPut(void *pRngData, MS_U32 u32DataSize, MS_BOOL bDump)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s enter\n", __FUNCTION__);
    HAL_NSK2_GetRNGThroughPut(pRngData, u32DataSize, bDump);
}

#if 0
void MDrv_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode)
{
	HAL_NSK2_BurstLen(u32PVREng, u32BurstMode);
}
#endif

MS_U32 MDrv_NSK2_GetCMProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetCMProperties(desc_size, desc);
}

MS_U32 MDrv_NSK2_GetM2MProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetM2MProperties(desc_size, desc);
}

MS_U32 MDrv_NSK2_GetDMAProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetDMAProperties(desc_size, desc);
}

MS_U32 MDrv_NSK2_GetOTPProperties(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetOTPProperties(desc_size, desc);
}

MS_U32 MDrv_NSK2_GetFullChipConfig(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetFullChipConfig(desc_size, desc);
}

MS_U32 MDrv_NSK2_GetNVCounterConfig(MS_U32 *desc_size, MS_U8 *desc)
{
    return HAL_NSK2_GetNVCounterConfig(desc_size, desc);
}


void MDrv_NSK2_ReadAllOTP(void)
{
    HAL_NSK2_ReadAllOTP();
}


MS_BOOL MDrv_NSK2_GetPubOTP(MS_U8 *pPubOTP)
{
    //return HAL_NSK2_GetPubOTP(pPubOTP);
    return TRUE;
}

//=====================================
// =========== Debug Usage=================
//=====================================
void DBG_NSK2_AllTSPPidFilter(void)
{
    HAL_NSK2_AllTSPPidFilter();
}

void DBG_NSK2_ChangePidFilter(MS_U32 pid_no, MS_U32 Data)
{
    HAL_NSK2_ChangePidFilter(pid_no, Data);
}


//=====================================
//======   NSK2.1 new functions  ======
//=====================================

MS_U32 MDrv_NSK21_InvalidCmChannel(MS_U16 PidSlot)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s PidSlot = %x\n", __FUNCTION__,PidSlot);
    HAL_NSK21_InvalidCmChannel(PidSlot);
    return TRUE;
}

MS_U32 MDrv_NSK21_CfgCmChannel(MS_U16 PidSlot, MS_U16 LocalDAlgo, MS_U16  ESAAlgo, MS_U16 LocalSAlgo, MS_U16 SysKeyIndex)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s PidSlot = %x\n", __FUNCTION__,PidSlot);

    return HAL_NSK21_CfgCmChannel(PidSlot, LocalDAlgo,  ESAAlgo, LocalSAlgo, SysKeyIndex);
}

MS_U32 MDrv_NSK21_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLocalDIV1, void *pLocalDIV2,
                                    void *pESAIV1, void *pESAIV2, void *pLocalSIV1, void *pLocalSIV2, MS_U16 PidSlot )
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"SCB = %x, ForceSCB = %x\n",SCB,ForceSCB);

    return HAL_NSK21_WriteTransportKey(SCB, ForceSCB, pLocalDIV1, pLocalDIV2, pESAIV1, pESAIV2, pLocalSIV1, pLocalSIV2, PidSlot);
}

MS_U32 MDrv_NSK21_WriteM2MKey(MS_U32 M2MAlgo)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s M2MAlgo = %x\n", __FUNCTION__,M2MAlgo);

    return HAL_NSK21_WriteM2MKey(M2MAlgo);
}

MS_U32 MDrv_NSK21_ModifyGenIn(MS_U32 MaskVal,MS_U32 XorVal)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s MaskVal = %x, XorVal = %x\n", __FUNCTION__,MaskVal,XorVal);

    return HAL_NSK21_ModifyGenIn(MaskVal,XorVal);
}

MS_U32 MDrv_NSK21_WriteJTagKey(MS_U32 OverrideOid, MS_U32 Select)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    return HAL_NSK21_WriteJTagKey(OverrideOid, Select);
}

MS_U32 MDrv_NSK21_IncrementNvCounter(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    return HAL_NSK21_IncrementNvCounter();
}

MS_U32 MDrv_NSK21_WriteReservedKey(void)
{

    return TRUE;
}


MS_U32 MDrv_NSK2_CompareIRQ(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 ExpectedValue)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    MS_U32 status;

    MsOS_DelayTask(100);

#ifdef ACPUTest
    //printf("IntStatus = %x, ExpectedValue = %x, time = %d\n",u32NSK2IntStatus, ExpectedValue, MsOS_GetSystemTime());
    status = u32NSK2IntStatus; //HAL_NSK2_GetIntStatus();
    u32NSK2IntStatus = 0;
#else
    status = HAL_NSK2_GetIntStatus();
#endif
    //printf("Int Status = %x\n",status);

    if(0 == ExpectedValue)
    {
        if(0 != status)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if(0 == status)
            return TRUE;
        else
            return FALSE;
    }

}

MS_U32 MDrv_NSK2_WriteOtpKey(void)
{
    return HAL_NSK2_WriteOtpKey();
}

MS_U32 MDrv_NSK2_ReadPWD_Status(void)
{
    return HAL_NSK2_ReadPWD_Status();
}

MS_U32 MDrv_NSK2_CtrlClk(MS_BOOL Enable)
{
    return HAL_NSK2_CtrlClk(Enable);
}

MS_U32 MDrv_NSK2_ReadClkStatus(void)
{
    return HAL_NSK2_ReadClkStatus();
}

MS_U32 MDrv_NSK2_GetSRAlignBit(void)
{
    return HAL_NSK2_GetSRAlignBit();
}

MS_BOOL MDrv_NSK2_RSA_SetSecureRange(MS_U32 u32SecSet, MS_U32 u32SecStart, MS_U32 u32SecEnd)
{
    return HAL_NSK2_RSA_SetSecureRange(u32SecSet, u32SecStart, u32SecEnd, TRUE);
}

void MDrv_NSK2_RSA_SetSR_Mask(MS_U32 u32DramSize)
{
    if (0 != u32DramSize)
    {
        HAL_NSK2_RSA_SetSR_Mask(u32DramSize);
    }
}
