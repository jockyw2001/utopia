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
#define _DRVEMMFLT_C

////////////////////////////////////////////////////////////////////////////////
/// file drvEMMFlt.c
/// @author MStar Semiconductor Inc.
/// @brief to filter EMM packet
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsOS.h"
#include "MsVersion.h"
#include "drvEMMFlt.h"
#include "halEMMflt.h"
#include "drvMMIO.h"
#include "asmCPU.h"
#include "halCHIP.h"
#include "drvNSK2Type.h"
#include "MsIRQ.h"

////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static MS_U32 _g32EMMDbgLv = EMM_DBGLV_DEBUG;
static InterruptNum _gEMMFltIntNum = 0;

#define _EMM_DBG(x, args...) {printf(x, ##args);}

#define DRVEMM_DBG(lv, x, args...)  do { if(lv <= _g32EMMDbgLv)     \
                      {                       \
                            _EMM_DBG("%s(%d): ", __FUNCTION__,__LINE__); _EMM_DBG(x, ##args); \
                      } } while (0)


#define EMM_ENABLE                      1
#define EMM_DISABLE                     0

#define EMMFLT_TASK_STACK_SIZE          16*1024

//#define EMM_IRQ_INT     E_INT_FIQ_TSP_SPARE_CA2PM_8

//interrupt control should be modified after checking the correct one.
#define EMMFLTIntEnable()                       MsOS_EnableInterrupt(_gEMMFltIntNum);
#define EMMFLTIntDisable()                      MsOS_DisableInterrupt(_gEMMFltIntNum);
#define EMMFLTIntAttach()                       MsOS_AttachInterrupt(_gEMMFltIntNum, (InterruptCb )_MDrv_EMMDFLT_Isr)
#define EMMFLTIntDetach()                       MsOS_DetachInterrupt (_gEMMFltIntNum)


#define NDS_EMM_EVENT_OVERFLOW_SW       0x00000040

#define StatusCheck(x) { if(x!=DRV_EMMFLT_OK)  \
                            return (x); }
//#define CompileWithNSKLib
////////////////////////////////////////////////////////////////////////////////
// External funciton
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local Variables
////////////////////////////////////////////////////////////////////////////////
static MS_S32 _s32EMMFLTMutexId = -1;
static MS_S32 _s32EMMFLTEventId = -1;
static MS_S32 _s32EMMDFLTTaskId = -1;

static MS_U8  _u8EmmIsInit = FALSE;

static void*  _pEMMDFLTTaskStack;
static MS_U8  _u8EMMDFLT_StackBuffer[EMMFLT_TASK_STACK_SIZE];

MS_U8  _u8EmmBuf[EMMENG_NUMBER][MAX_EMM_LIST_NUM][NDS_EMMFLT_BUF_SIZE];
static MS_U32 _gHWEMMBufAddr[EMMENG_NUMBER][MAX_EMM_LIST_NUM];
static MS_BOOL bEmmOverflow[EMMENG_NUMBER];
//static MS_BOOL bEmmOverflowReturn[EMMENG_NUMBER];


static MS_U8 _u8EmmRead[EMMENG_NUMBER];
static MS_U8 _u8EmmWrite[EMMENG_NUMBER];
static MS_U8 _u8EmmHold[EMMENG_NUMBER];

/*	backup address, later used to find connection	*/
static MS_U8 *_pu8BufAddrBackup[EMMENG_NUMBER][MAX_EMM_LIST_NUM];

static EMMFLT_Notify _gEMMFltNtf = NULL;

////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
static void _MDrv_EMMDFLT_Isr(void);

#define EMMFLT_ENTRY()                 MsOS_ObtainMutex(_s32EMMFLTMutexId, MSOS_WAIT_FOREVER)
#define EMMFLT_RETURN(_ret)            { MsOS_ReleaseMutex(_s32EMMFLTMutexId); return _ret; }
#define EMMFLT_LOCK()                  MsOS_ObtainMutex(_s32EMMFLTMutexId, MSOS_WAIT_FOREVER)
#define EMMFLT_UNLOCK()                MsOS_ReleaseMutex(_s32EMMFLTMutexId)


#define emmQueueInc(connection)                         \
{                                                       \
    _u8EmmWrite[connection]++;                          \
    if (MAX_EMM_LIST_NUM <= _u8EmmWrite[connection])    \
    {                                                   \
        _u8EmmWrite[connection] = 0;                    \
    }                                                   \
}

#define emmQueueDec(connection)                         \
{                                                       \
    _u8EmmRead[connection]++;                           \
    if (MAX_EMM_LIST_NUM <= _u8EmmRead[connection])     \
    {                                                   \
        _u8EmmRead[connection] = 0;                     \
    }                                                   \
}


#define emmQueueIsEmpty(connection)       (_u8EmmRead[connection] == _u8EmmWrite[connection])
#define emmQueueFreeSize(connection)      (MAX_EMM_LIST_NUM - emmQueueUsedSize(connection) - 1)
#define emmQueueUsedSize(connection)      ((_u8EmmWrite[connection] >= _u8EmmRead[connection]) ? \
												(_u8EmmWrite[connection] - _u8EmmRead[connection]) : \
												(MAX_EMM_LIST_NUM - _u8EmmRead[connection] + _u8EmmWrite[connection]))
#define emmQueueIsFull(connection)        (0 == emmQueueFreeSize(connection))
#define emmQueueReset(connection)         { _u8EmmRead[connection] = _u8EmmWrite[connection] = _u8EmmHold[connection] = 0 }

#define CONNECTFLAG_BASE        0x10000



#ifdef MSOS_TYPE_LINUX_KERNEL
extern irqreturn_t _NDS_RASP_Isr(int irq, void *dev_id);
#else
extern void _NDS_RASP_Isr(InterruptNum eIntNum);
#endif

static MS_BOOL EMMRASP_En[MAX_ENGINE] = {FALSE, FALSE};

static void _EMMRASP_Isr_(void)
{
    if( TRUE == EMMRASP_En[EMM_ENGINE] )
    {
        _MDrv_EMMDFLT_Isr();
    }

    if( TRUE == EMMRASP_En[RASP_ENGINE] )
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        _NDS_RASP_Isr(E_INT_IRQ_RASP, NULL);
#else
        _NDS_RASP_Isr(E_INT_IRQ_RASP);
#endif
    }

    MsOS_EnableInterrupt(E_INT_IRQ_RASP);
}

void EMMRASP_IntAttach(EMMRASP_e eEng, MS_BOOL Enable)
{
    if(HAL_EMMFLT_GetIntMode())
    {
        EMMRASP_En[eEng] = Enable;
        if(TRUE == Enable)
        {
            if( (TRUE == EMMRASP_En[EMM_ENGINE]) && (TRUE == EMMRASP_En[RASP_ENGINE]) )
            {
            }
            else
            {
                MsOS_AttachInterrupt(E_INT_IRQ_RASP, (InterruptCb )_EMMRASP_Isr_);
                MsOS_EnableInterrupt(E_INT_IRQ_RASP);
            }
        }
        else
        {
            if( (FALSE == EMMRASP_En[EMM_ENGINE]) && (FALSE == EMMRASP_En[RASP_ENGINE]) )
            {
                MsOS_DetachInterrupt(E_INT_IRQ_RASP);
                MsOS_DisableInterrupt(E_INT_IRQ_RASP);
            }
        }
    }
    else
    {
        if(EMM_ENGINE == eEng)
        {
            if(TRUE == Enable)
            {
                EMMFLTIntAttach();
                EMMFLTIntEnable();
            }
            else
            {
                EMMFLTIntDetach();
                EMMFLTIntDisable();
            }
        }
        else
        {
            if(TRUE == Enable)
            {
                MsOS_AttachInterrupt(E_INT_IRQ_RASP, (InterruptCb )_NDS_RASP_Isr);
                MsOS_EnableInterrupt(E_INT_IRQ_RASP);
            }
            else
            {
                MsOS_DetachInterrupt(E_INT_IRQ_RASP);
                MsOS_DisableInterrupt(E_INT_IRQ_RASP);
            }
        }
    }

}

static void _MDrv_EMMDFLT_Isr(void)
{
    MS_U16 u16EMMInt[EMMENG_NUMBER];	//useless
	MS_U16 u16EMMBufIndex[EMMENG_NUMBER];
    MS_U8  connectflag[EMMENG_NUMBER],i;
    MS_U8  u8EMMBufCnt = 0;
    MS_U32 u32EventFlag = 0;
	MS_U16 u16EMMIntStat[EMMENG_NUMBER];

    DRVEMM_DBG(EMM_DBGLV_DEBUG,"\n_MDrv_EMMDFLT_Isr\n");

    //EMMFLTIntDisable();

    HAL_EMMFLT_GetIntStatus(u16EMMInt,u16EMMIntStat,connectflag);

    for( i=0; i<EMMENG_NUMBER; i++)
    {
    	//HAL_EMMFLT_ResetInt(i);//egi_test_20141211

        if(connectflag[i] == FALSE){ continue; }
		u32EventFlag = (CONNECTFLAG_BASE<<i);

        //u16EMMBufIndex[i] = HAL_EMMFLT_GetCurrentBufIndex(i);	//need to move position
        HAL_EMMFLT_PacketCount(i,&u8EMMBufCnt);
        DRVEMM_DBG(EMM_DBGLV_DEBUG, "conn[%x]: u8EMMBufCnt = %x\n",i,u8EMMBufCnt);

        if (u16EMMIntStat[i] & EMMFLT_EMM_OVERFLOW_INT)
        {
            DRVEMM_DBG(EMM_DBGLV_ERR, "EMMFLT_EMM_HW_OVERFLOW_INT\n");
            if (bEmmOverflow[i] == FALSE)
            {
                bEmmOverflow[i] = TRUE;
                u32EventFlag |= u16EMMIntStat[i];

				HAL_EMMFLT_PacketAct(i);	//act to clear interrupt....
                MsOS_SetEvent(_s32EMMFLTEventId, u32EventFlag);
            }
        }

        if (u16EMMIntStat[i] & EMMFLT_EMM_INT)
        {
        	DRVEMM_DBG(EMM_DBGLV_INFO, "EMMFLT_EMM_INT\n");
            if (bEmmOverflow[i] == FALSE)
            {
                if(emmQueueIsFull(i))
                {
                	DRVEMM_DBG(EMM_DBGLV_ERR, "EMM Queue Is Full\n");
                    bEmmOverflow[i] = TRUE;
					u32EventFlag |= NDS_EMM_EVENT_OVERFLOW_SW;
                    HAL_EMMFLT_PacketAct(i);	//act to clear interrupt....

                }
                else
                {
                    MsOS_ReadMemory();
                    while(u8EMMBufCnt>0)
                    {
						u16EMMBufIndex[i] = HAL_EMMFLT_GetCurrentBufIndex(i);
						DRVEMM_DBG(EMM_DBGLV_DEBUG, "EMMBufIndex=%x, EMMBufCnt = %x\n",u16EMMBufIndex[i],u8EMMBufCnt);
                        memcpy(_u8EmmBuf[i][_u8EmmWrite[i]], (void*)MS_PA2KSEG1(_gHWEMMBufAddr[i][u16EMMBufIndex[i]]),  EMM_COPY_PACKET_SIZE);
                        emmQueueInc(i);
						u8EMMBufCnt--;

						DRVEMM_DBG(EMM_DBGLV_INFO, "EmmWrite[%x],EmmHold[%x],EmmRead[%x]\n",_u8EmmWrite[i],_u8EmmHold[i],_u8EmmRead[i]);
                        HAL_EMMFLT_PacketAct(i);	/*act to clear interrupt...*/
                    }
                }
				u32EventFlag |= u16EMMIntStat[i];
				MsOS_SetEvent(_s32EMMFLTEventId, u32EventFlag);

            }
            else
            {
            	#if 0 //test
                //needs to consider...
                u32EventFlag |= (u16EMMIntStatus[i] | NDS_EMM_EVENT_OVERFLOW_SW);
	            #endif

                HAL_EMMFLT_PacketAct(i); 	//act to clear interrupt....
                //MsOS_SetEvent(_s32EMMFLTEventId, u32EventFlag);
            }
        }

    }

    if(FALSE == HAL_EMMFLT_GetIntMode())
    {
        EMMFLTIntEnable();
    }

}

MS_U8 bufcnt = 0;

static void _MDrv_EMMDFLT_Isr_Proc(void)
{
    MS_U32 u32Events;
    MS_U8  i,connection;
	MS_U32 u32UsedSize;
	MS_U8 tmp;

    bufcnt = 0;

    DRVEMM_DBG(EMM_DBGLV_DEBUG,"EMMFlt wait event\n");
    HAL_EMMFLT_SetDebugMode(0xF);

    while (1)
    {
        MsOS_WaitEvent(_s32EMMFLTEventId, 0xFFFFFFFF /*EMMFLT_STATES_GROUP*/, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER /*1000 */ );
        DRVEMM_DBG(EMM_DBGLV_DEBUG,"u32Event = %x\n",u32Events);

        for( i=0; i<EMMENG_NUMBER; i++)
        {
            if( ( u32Events & (CONNECTFLAG_BASE<<i) ) == 0 ) {continue;}

            connection = i;
            DRVEMM_DBG(EMM_DBGLV_DEBUG,"connection = %x\n",connection);

            if (u32Events & EMMFLT_EMM_INT)
            {
                u32UsedSize = ((_u8EmmWrite[i] >= _u8EmmHold[i]) ? (_u8EmmWrite[i] - _u8EmmHold[i]) : (MAX_EMM_LIST_NUM - _u8EmmHold[i] + _u8EmmWrite[i]));
                for (; u32UsedSize > 0; u32UsedSize--)
                {
					if (_u8EmmBuf[i][_u8EmmHold[i]][0] != 0x47)
                    {
                    	DRVEMM_DBG(EMM_DBGLV_ERR,"packet %d is DULL _u8EmmBuf[i][_u8EmmHold[i]][0] = %x \n", _u8EmmHold[i], _u8EmmBuf[i][_u8EmmHold[i]][0]);	//for dbg [Aug8,14]
						/*for prevent sw overflow due to dull packet*/
						emmQueueDec(connection);
                        goto jump_next_emm;
                    }
                    DRVEMM_DBG(EMM_DBGLV_DEBUG,"\nEMM_RECV %d\n\n", MsOS_GetSystemTime());

					/*	backup address for release emm later	*/
                    _pu8BufAddrBackup[i][_u8EmmHold[i]] = &_u8EmmBuf[i][_u8EmmHold[i]][0];
					DRVEMM_DBG(EMM_DBGLV_INFO,"NSK2HDX_EmmArrived = %p\n",_pu8BufAddrBackup[i][_u8EmmHold[i]]);

            		jump_next_emm:
            		tmp = _u8EmmHold[i]++;
                    if ( MAX_EMM_LIST_NUM  <= _u8EmmHold[i])
                    {
                        _u8EmmHold[i] = 0;
                    }
					DRVEMM_DBG(EMM_DBGLV_DEBUG, "EmmWrite[%x],EmmHold[%x],EmmRead[%x]\n",_u8EmmWrite[i],_u8EmmHold[i],_u8EmmRead[i]);
					if(_gEMMFltNtf) {  _gEMMFltNtf(i,EMM_EVENT_ARRIVE, &_u8EmmBuf[i][tmp][0]);  }

                }
            }

            if (u32Events & NDS_EMM_EVENT_OVERFLOW_SW)
            {
                DRVEMM_DBG(EMM_DBGLV_ERR,"EMM Buffer SW Overflow\n");
				if(_gEMMFltNtf)  { _gEMMFltNtf(i,EMM_EVENT_OVERFLOW, NULL); }
            }

            if (u32Events & EMMFLT_EMM_OVERFLOW_INT)
            {
                DRVEMM_DBG(EMM_DBGLV_ERR,"EMM Buffer HW Overflow\n");
				if(_gEMMFltNtf)  { _gEMMFltNtf(i,EMM_EVENT_OVERFLOW, NULL); }
            }
        }//connection loop

		EMMFLTIntEnable();	//egi_test_20141211

    } // Task while loop
}

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
EMMFLT_RESULT MDrv_EMMFLT_Init(EMMFLT_Require_t *pRequire)
{

    MS_VIRT u32Bank;
	MS_PHY u32BankSize;
    MS_U32 i;

    DRVEMM_DBG(EMM_DBGLV_DEBUG,"enter MDrv_EMMFLT_Init\n");
    if(_u8EmmIsInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_PM))
        {
            DRVEMM_DBG(EMM_DBGLV_ERR,"MDrv_MMIO_GetBASE (NonPM base)fail\n");
            MS_ASSERT(0);
        }

        printf("u32Bank = %X\n",u32Bank);
        DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s u32Bank: %x\n", __FUNCTION__, u32Bank);
        HAL_EMMFLT_SetBank(u32Bank); //set non pm bank

        for( i=0 ; i<EMMENG_NUMBER ; i++ )
        {
            HAL_EMMFLT_SWReset(i);
        }
        HAL_EMMFLT_Init();

        _gEMMFltIntNum = (InterruptNum)HAL_EMMFLT_GetIntNumber();

        EMMRASP_IntAttach(EMM_ENGINE, TRUE);
        HAL_EMMFLT_Enable_Int();

        _s32EMMFLTEventId = MsOS_CreateEventGroup("EMMFLT_Event");
        if (_s32EMMFLTEventId < 0)
        {
            DRVEMM_DBG(EMM_DBGLV_ERR,"CreateEvent fail\n");
            return DRV_EMMFLT_FAIL;
        }

        _s32EMMFLTMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "EMMFLT_Mutex", MSOS_PROCESS_SHARED);
        if (_s32EMMFLTMutexId < 0)
        {
            DRVEMM_DBG(EMM_DBGLV_ERR,"CreateMutex fail\n");
            MsOS_DeleteEventGroup(_s32EMMFLTEventId);
            return DRV_EMMFLT_FAIL;
        }

        _pEMMDFLTTaskStack = _u8EMMDFLT_StackBuffer;
        _s32EMMDFLTTaskId = MsOS_CreateTask((TaskEntry)_MDrv_EMMDFLT_Isr_Proc,
                                            (MS_U32)NULL,
                                            E_TASK_PRI_HIGH,
                                            TRUE,
                                            _pEMMDFLTTaskStack,
                                            EMMFLT_TASK_STACK_SIZE,
                                            "EMMFLT_ISR_Task");
        if (_s32EMMDFLTTaskId < 0)
        {
            DRVEMM_DBG(EMM_DBGLV_ERR,"CreateTask fail\n");
            MsOS_DeleteEventGroup(_s32EMMFLTEventId);
            MsOS_DeleteMutex(_s32EMMFLTMutexId);
            return DRV_EMMFLT_FAIL;
        }

        _u8EmmIsInit = TRUE;
    }
    else
    {
        DRVEMM_DBG(EMM_DBGLV_INFO,"EMMFlt had been initialed\n");
    }

    for( i=0 ; i<EMMENG_NUMBER ; i++ )
    {
        bEmmOverflow[i] = FALSE;
    }

    HAL_EMMFLT_ReqDstBufSize(&pRequire->u32DstBufSize,&pRequire->u32AlignBytes,&pRequire->u8EMMNumber);

    DRVEMM_DBG(EMM_DBGLV_INFO,"u32DstBufSize = %x , u32AlignBytes = %x\n",pRequire->u32DstBufSize,pRequire->u32AlignBytes);

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_Deinit(void)
{

    DRVEMM_DBG(EMM_DBGLV_INFO,"MDrv_EMMFLT_Deinit\n");


    //mask interrupt ...
    HAL_EMMFLT_IntCtrl(1);

    EMMFLTIntDetach();
    EMMFLTIntDisable();

    if (_s32EMMFLTEventId > 0)
    {
        MsOS_DeleteEventGroup(_s32EMMFLTEventId);
    }

    if (_s32EMMFLTMutexId > 0)
    {
        MsOS_DeleteMutex(_s32EMMFLTMutexId);
    }

    if (_s32EMMDFLTTaskId > 0)
    {
        MsOS_DeleteTask(_s32EMMDFLTTaskId);
    }

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_SetNotify(EMMFLT_Notify Notify)
{
    _gEMMFltNtf = Notify;
    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_GetHwBufCnt(MS_U32 *pHwBufCnt)
{
    return HAL_EMMFLT_GetHwBufCnt(pHwBufCnt);
}

EMMFLT_RESULT MDrv_EMMFLT_SetDbgLevel(MS_U32 u32Level)
{
    _g32EMMDbgLv = u32Level;
    DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s level: %x\n", __FUNCTION__, u32Level);
    HAL_EMMFLT_SetDbgLevel(u32Level);
    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_SetDstBufInfo(MS_U32 x_connection, MS_U32 u32BufAddr, MS_U32 u32BufSize)
{
    HAL_EMMFLT_SetDstBufInfo(x_connection,u32BufAddr,u32BufSize,_gHWEMMBufAddr[x_connection]);
    return DRV_EMMFLT_OK;
}


EMMFLT_RESULT MDrv_EMMFLT_SetEmmPID(MS_U32 connection,MS_U16 u16EmmPID)
{
    DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s conn: %x, EmmPID = %x\n", __FUNCTION__, connection,u16EmmPID);
    bEmmOverflow[connection] = FALSE;
    return HAL_EMMFLT_SetEmmPID(connection,u16EmmPID);
}

EMMFLT_RESULT MDrv_EMMFLT_SetTidMode(MS_U32 connection, MS_U8 u8TidValue, EMMDRV_TIDMODE_e eTIDMODE)
{
    DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s conn: %x, TIDMODE = %d\n", __FUNCTION__, connection,eTIDMODE);
    switch(eTIDMODE)
    {
        case EMM_TIDMODE_INGORE:
            HAL_EMMFLT_SetTidMode(connection,(u8TidValue&0xf),E_TIDMODE_INGORE);
            break;

        case EMM_TIDMODE_ACCEPT_ALL:
            HAL_EMMFLT_SetTidMode(connection,(u8TidValue&0xf),E_TIDMODE_ACCEPT_ALL);
            break;

        case EMM_TIDMODE_ACCEPT_ADDRESS:
            HAL_EMMFLT_SetTidMode(connection,(u8TidValue&0xf),E_TIDMODE_ACCEPT_ADDRESS);
            break;

        default :
            HAL_EMMFLT_SetTidMode(connection,(u8TidValue&0xf),E_TIDMODE_RESERVED);
            break;
    }

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_EnableEmmTID(MS_U32 connection, MS_BOOL bEnable)
{
    DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s conn: %x, bEnable = %x\n", __FUNCTION__, connection,bEnable);
    HAL_EMMFLT_SetEmmTID(connection,0x82);

    HAL_EMMFLT_EnableEmmTID(connection,bEnable);

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_SetFilterCfg(MS_U32 connection, MS_U8 FilerIndex, EMM_FILTER_DEF_t *pFilter)
{
    MS_U32 u32Status;
    DRVEMM_DBG(EMM_DBGLV_DEBUG, "%s conn: %x, FilerIndex = %x\n", __FUNCTION__, connection,FilerIndex);

    u32Status = HAL_EMMFLT_SetIRDMode(connection,FilerIndex,pFilter->filter_type);
    StatusCheck(u32Status);

    u32Status = HAL_EMMFLT_SetEmmDataIDx(connection,FilerIndex,pFilter->filter_address);
    StatusCheck(u32Status);

    u32Status = HAL_EMMFLT_SetEmmMaskIDx(connection,FilerIndex,pFilter->filter_address_mask);
    StatusCheck(u32Status);

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_Fire(MS_U32 connection)
{
    //fire EMM filter to wait stream...
    DRVEMM_DBG(EMM_DBGLV_INFO, "%s conn: %x\n", __FUNCTION__, connection);
    return HAL_EMMFLT_GeneralCtrl(connection);
}


EMMFLT_RESULT MDrv_EMMFLT_StopEmm(MS_U32 x_connection)
{

    DRVEMM_DBG(EMM_DBGLV_INFO,"MDrv_EMMFLT_StopEmm, x_connection = %x\n",x_connection);
    HAL_EMMFLT_DisableEMM(x_connection);
    //MDrv_EMMFLT_Deinit();
    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_EmmProcessingFinished(MS_U8 *emm_buf/* RPC SIZECONST(256) */)
{
    DRVEMM_DBG(EMM_DBGLV_INFO,"free EMM %x\n", (MS_U32)emm_buf);

    MS_U8  i,j, connection = 0xf;
    EMMFLT_ENTRY(); // for disable interrupt protection

    if(emm_buf != NULL)
    {
        for(j=0; j<EMMENG_NUMBER ; j++)
        {
            for(i=0; i<MAX_EMM_LIST_NUM ; i++)
            {
                if(_pu8BufAddrBackup[j][i] == emm_buf)
                {
                    connection = j;
                    break;
                }
            }
        }

		if (connection==0xf)
		{
			EMMFLT_RETURN( DRV_EMMFLT_INVALID_REQUEST );
		}

        EMMFLTIntDisable();
        //*(emm_buf) = 0;
        //*(emm_buf-8) = 0;

		if (_u8EmmRead[connection] != _u8EmmWrite[connection])	//patch,test,2014dec11. MDrv_EMMFLT_EmmProcessingFinished being called twice by verif_lib when theres single packet arrived
        {
        	emmQueueDec(connection);
		}

	    #if 1
        bEmmOverflow[connection] = FALSE;
	    #else
        if ( (bEmmOverflow[connection] == TRUE) && (emmQueueIsEmpty(connection)) )
        {
            bEmmOverflow[connection] = FALSE;
        }
	    #endif
        EMMFLTIntEnable();

        DRVEMM_DBG(EMM_DBGLV_DEBUG,"%d (read, hold, write) = (%02d %02d %02d)\n", connection,_u8EmmRead[connection], _u8EmmHold[connection], _u8EmmWrite[connection]);
        if (bEmmOverflow[connection] == TRUE)
        {
            DRVEMM_DBG(EMM_DBGLV_ERR,"%d o.flow (read, hold, write) = (%02d %02d %02d)\n", connection,_u8EmmRead[connection], _u8EmmHold[connection], _u8EmmWrite[connection]);
            //DRVEMM_DBG(0,"free Emm %d %02x %02x\n", MsOS_GetSystemTime() - emm_overflow_time, *(emm_buf), *(emm_buf-8));
        }

    }
    else
    {
        EMMFLT_RETURN( DRV_EMMFLT_INVALID_REQUEST );
    }
    EMMFLT_RETURN(DRV_EMMFLT_OK);
}

EMMFLT_RESULT MDrv_EMMFLT_SetOutputType(MS_U32 x_connection, EMM_OUTTYPE_e eOutType)
{
    DRVEMM_DBG(EMM_DBGLV_INFO,"[%s]-[%d], conn = %x, OutType = %d\n", __FUNCTION__, __LINE__, x_connection, eOutType);
    HAL_EMMFLT_SetOutputType(x_connection, eOutType);
    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_SrcSelect(MS_U32 x_connection, MS_U32 u32SrcFrom, EMM_SRCTYPE_e eSrcType)
{
    DRVEMM_DBG(EMM_DBGLV_INFO,"[%s]-[%d], conn = %x, u32SrcFrom = %x, SrcType = %d\n", __FUNCTION__, __LINE__, x_connection, u32SrcFrom,eSrcType);
    HAL_EMMFLT_SrcSelect(x_connection, u32SrcFrom, eSrcType);
    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_ConnectCheck(MS_U32 connection)
{
    return HAL_EMMFLT_ConnectCheck(connection);
}


EMMFLT_RESULT MDrv_EMMFLT_SetExtendConfig(MS_U32 x_connection, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte)
{

    printf("MDrv_EMMFLT_SetExtendConfig, x_connection = %x\n",x_connection);
    HAL_EMMFLT_SetExtendConfig(x_connection, type, extendbytes, syncbyte);

    return DRV_EMMFLT_OK;
}

EMMFLT_RESULT MDrv_EMMFLT_InputMode(MS_U32 x_connection, MS_BOOL bSerial)
{
    DRVEMM_DBG(EMM_DBGLV_INFO,"[%s]-[%d], conn = %x, bSerial = %d\n", __FUNCTION__, __LINE__, x_connection, bSerial);
    return HAL_EMMFLT_InputMode(x_connection, bSerial);
}


