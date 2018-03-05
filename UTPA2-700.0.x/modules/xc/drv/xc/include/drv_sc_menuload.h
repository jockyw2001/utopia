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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================

#ifndef DRV_SC_MENULOAD_H
#define DRV_SC_MENULOAD_H

#include "mhal_menuload.h"

#define MLOAD_MUTEX
#define MLOAD_MUTEX_DBG	0


#ifdef DRV_SC_MENULOAD_C
#define INTERFACE
INTERFACE MS_S32            _MLOAD_MUTEX = -1;
#else
#define INTERFACE extern
INTERFACE MS_S32            _MLOAD_MUTEX;
#endif

#define _MLOAD_MUTEX_TIME_OUT                 MSOS_WAIT_FOREVER

#ifdef MLOAD_MUTEX
#if(MLOAD_MUTEX_DBG)
#define _MLOAD_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_MLD);                                      \
            printf("0,==========================Prepare to get mutex\n");                             \
            printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
            if(!MsOS_ObtainMutex(_MLOAD_MUTEX, _MLOAD_MUTEX_TIME_OUT))                     \
            {                                                                        \
                printf("==========================\n");                              \
                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
            }\
            printf("[%s][%06d] menuload mutex got\n",__FUNCTION__,__LINE__);

#define _MLOAD_RETURN(pInstance)                                                                \
            printf("1,==========================\n");                              \
            printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
            MsOS_ReleaseMutex(_MLOAD_MUTEX);                                          \
            printf("==========================prepare to release semaphore\n");                    \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_MLD);                           \
            printf("[%s][%06d] menuload mutex released\n",__FUNCTION__,__LINE__);
                                //return _ret;
#else
#define _MLOAD_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_MLD);                  \
            if(!MsOS_ObtainMutex(_MLOAD_MUTEX, _MLOAD_MUTEX_TIME_OUT))                     \
            {                                                                        \
            }
#define _MLOAD_RETURN(pInstance)                                                                    \
            MsOS_ReleaseMutex(_MLOAD_MUTEX);                                               \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_MLD);
                                //return _ret;

#endif
#else // #if not def MLOAD_MUTEX
#define _MLOAD_ENTRY()         while(0)
#define _MLOAD_RETURN()    while(0)
#endif // #ifdef _MLOAD_MUTEX

typedef struct __attribute__((packed))
{
    MS_U16 u16WPoint;
    MS_U16 u16RPoint;
    MS_U16 u16FPoint;
    MS_PHY PhyAddr;
    MS_U16 u16MaxCmdCnt;
    MS_BOOL bEnable;
}MS_MLoad_Info;

MS_BOOL MDrv_XC_MLoad_Check_Done(void *pInstance);
void MDrv_XC_MLoad_Wait_HW_Done(void *pInstance);
void  MDrv_XC_MLoad_Init(void *pInstance, MS_PHY phyAddr);
void MDrv_XC_MLoad_Trigger(void *pInstance, MS_PHY startAddr, MS_U16 u16CmdCnt);
void MDrv_XC_MLoad_AddCmd(void *pInstance, MS_U32 u32Cmd);
MS_U32 MDrv_XC_MLoad_GetCmd(MS_PHY DstAddr); //NO_NEED
MS_U64 MDrv_XC_MLoad_GetCmd_64Bits(MS_PHY DstAddr);
MS_BOOL MDrv_XC_MLoad_BufferEmpty(void *pInstance);
MS_BOOL MDrv_XC_MLoad_KickOff(void *pInstance);
MS_BOOL MDrv_XC_MLoad_GetCaps(void *pInstance);
void MDrv_XC_MLoad_AddNull(void *pInstance);
void MDrv_XC_MLoad_Add_32Bits_Cmd(void *pInstance,MS_U32 u32Cmd, MS_U16 u16Mask);
void MDrv_XC_MLoad_Add_64Bits_Cmd(void *pInstance,MS_U64 u64Cmd, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCommand(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MApi_XC_MLoad_WriteCmd_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCommand_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);


void MApi_XC_MLoad_Init(MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLoad_Cus_Init(EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLoad_Enable(MS_BOOL bEnable);
void MApi_XC_MLoad_Cus_Enable(EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_BOOL bEnable);
MLOAD_TYPE MApi_XC_MLoad_GetStatus(void);
MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmeidate);
MS_BOOL MDrv_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate);
MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

void MApi_XC_MLG_Init(MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLG_Enable(MS_BOOL bEnable);
MS_BOOL MApi_XC_MLG_GetCaps(void);
MLG_TYPE MApi_XC_MLG_GetStatus(void);
void MApi_XC_MLG_Fire(MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue);
MLOAD_TYPE MDrv_XC_MLoad_GetStatus(void *pInstance);
void MDrv_XC_MLoad_Enable(void *pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_XC_MLoad_set_IP_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp);
MS_BOOL MDrv_XC_MLoad_get_IP_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp);
void MDrv_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync);

#undef INTERFACE
#endif

