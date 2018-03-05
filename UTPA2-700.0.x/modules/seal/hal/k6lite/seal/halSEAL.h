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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_SEAL_H_
#define _HAL_SEAL_H_

#include <drvSEAL.h>

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define _FUNC_NOT_USED()        do {} while ( 0 )

#define SEAL_SECURE_RANGE_ALIGN     (0x1000UL) //4k bytes for secure range alignement
#define SEAL_IMI_RANGE_ALIGN        (0x400UL) //1k bytes for IMI range alignment
#define SEAL_NONPM_GROUP_NUM        (24UL)
#define SEAL_NONPM_GROUP_IP_NUM     (16UL)
#define SEAL_NONPM_TBL_IP_NUM       (SEAL_NONPM_GROUP_NUM*SEAL_NONPM_GROUP_IP_NUM)
#define SEAL_PM_GROUP_NUM           (4UL)
#define SEAL_PM_GROUP_IP_NUM        (16UL)
#define SEAL_PM_TBL_IP_NUM          (SEAL_PM_GROUP_NUM*SEAL_PM_GROUP_IP_NUM)
#define SEAL_PROCESSOR_GROUP_NUM    (1UL)
#define SEAL_PROCESSOR_NUM_PERGROUP (9UL)
#define SEAL_TBL_PROCESSOR_NUM      (SEAL_PROCESSOR_GROUP_NUM*SEAL_PROCESSOR_NUM_PERGROUP)
#define SEAL_LOCK_GROUP_NUM         (26UL)
#define SEAL_LOCK_NUM_PERGROUP      (16UL)
#define SEAL_TBL_LOCK_NUM           (SEAL_LOCK_GROUP_NUM*SEAL_LOCK_NUM_PERGROUP)
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_HAL_MIU_DEV0,
    E_HAL_MIU_DEV1,
    E_HAL_MIU_NUM,
}eHalMiu_Dev;

typedef enum
{
    E_HAL_SEAL_SECURE_RANGE_ID0,
    E_HAL_SEAL_SECURE_RANGE_ID1,
    E_HAL_SEAL_SECURE_RANGE_ID2,
    E_HAL_SEAL_SECURE_RANGE_ID3,
    E_HAL_SEAL_SECURE_RANGE_ID4,
    E_HAL_SEAL_SECURE_RANGE_ID5,
    E_HAL_SEAL_SECURE_RANGE_ID6,
    E_HAL_SEAL_SECURE_RANGE_ID7,
    E_HAL_SEAL_SECURE_RANGE_ID8,
    E_HAL_SEAL_SECURE_RANGE_ID9,
    E_HAL_SEAL_SECURE_RANGE_ID10,
    E_HAL_SEAL_SECURE_RANGE_ID11,
    E_HAL_SEAL_SECURE_RANGE_ID12,
    E_HAL_SEAL_SECURE_RANGE_ID13,
    E_HAL_SEAL_SECURE_RANGE_ID14,
    E_HAL_SEAL_SECURE_RANGE_ID15,
    E_HAL_SEAL_SECURE_RANGE_NUM,
}eHalSeal_SecureRangeId;

typedef enum
{
    E_HAL_SEAL_DBBUS,
    E_HAL_SEAL_MCU51,
    E_HAL_SEAL_CPU2,
    E_HAL_SEAL_VD_R2,
    E_HAL_SEAL_SECURE_R2,
    E_HAL_SEAL_SC,
    E_HAL_SEAL_CMDQ,
    E_HAL_SEAL_PROCESSOR_NUM,
}eHalSeal_ProcessorId;

/*
    Total 16 bytes

    B0: dummy[7:1], miu_ns[0]
    B1: dummy[7:2], ns_w/r[1:0]
    B2: r4_w/r[7:6], r3_w/r[5:4], r2_w/r[3:2], r1_w/r[1:0]
    B3: r8_w/r[7:6], r7_w/r[5:4], r6_w/r[3:2], r5_w/r[1:0]
    B4: r12_w/r[7:6], r11_w/r[5:4], r10_w/r[3:2], r9_w/r[1:0]
    B5: r16_w/r[7:6], r15_w/r[5:4], r14_w/r[3:2], r13_w/r[1:0]
    B6: addr_lmt_st_bit7-bit0 (total 20-bit)
    B7: addr_lmt_st_bit15-bit8
    B8: dummy[7:4], addr_lmt_st_bit19-bit16[3:0]
    B9: addr_lmt_end_bit7-bit0 (total 20-bit)
    B10: addr_lmt_end_bit15-bit8
    B11: dummy[7:4], addr_lmt_end_bit19-bit16[3:0]
    B12: dummy[7:6], addr_lmt_w/r[5:4], dummy[3:1], addr_lmt_en[0]
    B13: key_index
    B14: dummy
    B15: dummy

    Simplified version:
        non-secure client = 0x1, 0x3, 0x0, 0x0, 0x0...
        secure client = 0x0, 0x3, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0...
*/
typedef struct _SRAM_TZPC_NSClient
{
    volatile MS_U8 data[16];
    volatile MS_U8 dummy[16];
} SRAM_TZPC_NSClient;

typedef struct _SRAM_TZPC_NSGroup
{
    SRAM_TZPC_NSClient client[SEAL_NONPM_TBL_IP_NUM];
} SRAM_TZPC_NSGroup;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_SEAL_SetIOMapBase(MS_U32 u32NonPmBaseAddr, MS_U32 u32PmBaseAddr);
void HAL_SEAL_ResetSram(void);
MS_BOOL HAL_Seal_SecureRangeSet(MS_U8 u8SecureRangeId, MS_U64 u64StartAddr, MS_U64 u64EndAddr, MS_U32 u32Attribute);
MS_BOOL HAL_Seal_SecureRangeQuery(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId, MS_U32 *pAttribute, MS_U64 *pStartAddr, MS_U64 *pEndAddr);
MS_BOOL HAL_Seal_IMI_RangeSet(MS_U32 u32StartAddr, MS_U32 u32EndAddr, MS_BOOL bEnable);
MS_BOOL HAL_Seal_GetMiuHitFlag(MS_U8 u8MiuDev);
MS_U8 HAL_Seal_GetMiuHitRangeID(MS_U8 u8MiuDev);
MS_U8 HAL_Seal_GetMiuHitClientID(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_GetMiuSecureFlag(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_GetMiuWriteFlag(MS_U8 u8MiuDev);
MS_U64 HAL_Seal_GetMiuHitAddr(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_ClearMiuHitLog(MS_U8 u8MiuDev);
MS_BOOL HAL_Seal_SecureRangeLock(MS_U8 u8MiuDev, MS_U8 u8SecureRangeId);
MS_BOOL HAL_Seal_NonSecureProcessorSet(MS_U8 u8ProcessorId, MS_BOOL bNonSecure);
MS_BOOL HAL_Seal_NonSecureProcessorQuery(MS_U8 u8ProcessorId, MS_BOOL *pNonSecure);
MS_BOOL HAL_Seal_SecureSlaveSet(MS_U32 u32SlaveId, MS_BOOL bSecure);
MS_BOOL HAL_Seal_SecureSlaveQuery(MS_U32 u32SlaveId, MS_BOOL *pSecure);
MS_BOOL HAL_Seal_SecureMasterSet(MS_U32 u32MasterId, MS_BOOL bSecure);
MS_BOOL HAL_Seal_SecureMasterQuery(MS_U32 u32MasterId, MS_BOOL *pSecure);
MS_BOOL HAL_Seal_BufferLock(MS_U8 u8SecureBufferLockId, MS_BOOL bLock);
MS_BOOL HAL_Seal_QueryBufferLocked(eSeal_Lock eLockId, MS_BOOL* bLocked);
MS_BOOL HAL_Seal_MaskInterruptFlag(MS_U8 u8MiuDev, MS_BOOL bMask);
MS_BOOL HAL_SEAL_POWER_SUSPEND(void);
MS_BOOL HAL_SEAL_POWER_RESUME(void);
MS_BOOL HAL_Seal_CheckSecureRange(MS_PHY phyStartAddr, MS_U32 u32Length, eSeal_CheckSecureRangeType eCheckSecureRangeType);

#ifdef MSOS_TYPE_OPTEE
MS_BOOL HAL_Seal_ChangeIPSecureDMAAbillity(MS_U32 u32ModuleID, MS_U32 u32ModuleParameter, MS_U32 u32IsSecure);
MS_BOOL HAL_Seal_SetSecureRange(MS_PHY u64phy, MS_U32 u32Length, MS_U32 u32IsSecure);
#endif
#endif // _HAL_SEAL_H_
