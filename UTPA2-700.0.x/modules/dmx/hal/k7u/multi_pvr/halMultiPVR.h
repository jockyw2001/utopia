////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halMultiPVR.h
// @brief  Multi-PVR HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _HAL_MULTI_PVR_H_
#define _HAL_MULTI_PVR_H_

void    HAL_MultiPVR_SetBank(MS_VIRT u32BankAddr);
void    HAL_MultiPVR_Init(MS_U32 u32PVREng, MS_U32 pktDmxId);
void    HAL_MultiPVR_Exit(MS_U32 u32PVREng);
void    HAL_MultiPVR_FlushData(MS_U32 u32PVREng);
void    HAL_MultiPVR_Skip_Scrmb(MS_U32 u32PVREng, MS_BOOL bSkip);
void    HAL_MultiPVR_Block_Dis(MS_U32 u32PVREng, MS_BOOL bDisable);
void    HAL_MultiPVR_BurstLen(MS_U32 u32PVREng, MS_U16 u16BurstMode);
void    HAL_MultiPVR_Start(MS_U32 u32PVREng, MS_U32 u32ChId);
void    HAL_MultiPVR_Stop(MS_U32 u32PVREng, MS_U32 u32ChId);
void    HAL_MultiPVR_Pause(MS_U32 u32PVREng , MS_BOOL bPause);
void    HAL_MultiPVR_RecPid(MS_U32 u32PVREng, MS_BOOL bSet);
void    HAL_MultiPVR_RecNull(MS_U32 u32PVREng, MS_BOOL bSet);
void    HAL_MultiPVR_SetStr2Miu_StartAddr(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1);
void    HAL_MultiPVR_SetStr2Miu_EndAddr(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1);
MS_U32  HAL_MultiPVR_GetWritePtr(MS_U32 u32PVREng, MS_U32 u32ChId);
void    HAL_MultiPVR_SetStrPacketMode(MS_U32 u32PVREng, MS_U32 u32ChId, MS_BOOL bSet);
void    HAL_MultiPVR_SetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32ChId, MS_U32 u32Stamp);
MS_U32  HAL_MultiPVR_GetPVRTimeStamp(MS_U32 u32PVREng, MS_U32 u32ChId);

#endif // #ifndef _HAL_MULTI_PVR_H_