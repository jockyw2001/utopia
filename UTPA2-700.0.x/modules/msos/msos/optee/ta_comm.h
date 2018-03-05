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
////////////////////////////////////////////////////////////////////////////////

#ifndef TA_COMM_H
#define TA_COMM_H

#define TEE_Result MS_U32
#define TEE_SUCCESS                       0x00000000
#define TEE_ERROR_CORRUPT_OBJECT          0xF0100001
#define TEE_ERROR_CORRUPT_OBJECT_2        0xF0100002
#define TEE_ERROR_STORAGE_NOT_AVAILABLE   0xF0100003
#define TEE_ERROR_STORAGE_NOT_AVAILABLE_2 0xF0100004
#define TEE_ERROR_GENERIC                 0xFFFF0000
#define TEE_ERROR_ACCESS_DENIED           0xFFFF0001
#define TEE_ERROR_CANCEL                  0xFFFF0002
#define TEE_ERROR_ACCESS_CONFLICT         0xFFFF0003
#define TEE_ERROR_EXCESS_DATA             0xFFFF0004
#define TEE_ERROR_BAD_FORMAT              0xFFFF0005
#define TEE_ERROR_BAD_PARAMETERS          0xFFFF0006
#define TEE_ERROR_BAD_STATE               0xFFFF0007
#define TEE_ERROR_ITEM_NOT_FOUND          0xFFFF0008
#define TEE_ERROR_NOT_IMPLEMENTED         0xFFFF0009
#define TEE_ERROR_NOT_SUPPORTED           0xFFFF000A
#define TEE_ERROR_NO_DATA                 0xFFFF000B
#define TEE_ERROR_OUT_OF_MEMORY           0xFFFF000C
#define TEE_ERROR_BUSY                    0xFFFF000D
#define TEE_ERROR_COMMUNICATION           0xFFFF000E
#define TEE_ERROR_SECURITY                0xFFFF000F
#define TEE_ERROR_SHORT_BUFFER            0xFFFF0010
#define TEE_ERROR_EXTERNAL_CANCEL         0xFFFF0011
#define TEE_ERROR_OVERFLOW                0xFFFF300F
#define TEE_ERROR_TARGET_DEAD             0xFFFF3024
#define TEE_ERROR_STORAGE_NO_SPACE        0xFFFF3041
#define TEE_ERROR_MAC_INVALID             0xFFFF3071
#define TEE_ERROR_SIGNATURE_INVALID       0xFFFF3072
#define TEE_ERROR_TIME_NOT_SET            0xFFFF5000
#define TEE_ERROR_TIME_NEEDS_RESET        0xFFFF5001

/* Parameter Type Constants */
#define TEE_PARAM_TYPE_NONE             0
#define TEE_PARAM_TYPE_VALUE_INPUT      1
#define TEE_PARAM_TYPE_VALUE_OUTPUT     2
#define TEE_PARAM_TYPE_VALUE_INOUT      3
#define TEE_PARAM_TYPE_MEMREF_INPUT     5
#define TEE_PARAM_TYPE_MEMREF_OUTPUT    6
#define TEE_PARAM_TYPE_MEMREF_INOUT     7

typedef unsigned int                    uint32_t;

#define __unused __attribute__((unused))

#define TEE_PARAM_TYPES(t0,t1,t2,t3) \
   ((t0) | ((t1) << 4) | ((t2) << 8) | ((t3) << 12))
typedef union {
    struct {
        void *buffer;
        MS_U32 size;
    } memref;
    struct {
        MS_U32 a;
        MS_U32 b;
    } value;
} TEE_Param;


typedef struct {
    MS_U32 timeLow;
    MS_U16 timeMid;
    MS_U16 timeHiAndVersion;
    MS_U8 clockSeqAndNode[8];
} TEE_UUID;

/* Trusted Application Function header */
typedef struct ta_func_head {
    MS_U32 cmd_id;    /* Trusted Application Function ID */
    MS_U32 start;        /* offset to start func */
} ta_func_head_t;


typedef struct {
    /* Same Prefix as ta_head_t */
    TEE_UUID uuid;
    const char *name;
    MS_U32 flags;

    /* properties */
    MS_U32 prop_datasize;
    MS_U32 prop_stacksize;
    MS_U32 prop_tracelevel;

    const ta_func_head_t *funcs;
    MS_U32 nbr_func;
     TEE_Result(*create_entry_point) (void);
    void (*destroy_entry_point) (void);
     TEE_Result(*open_session_entry_point) (MS_U32 nParamTypes,
                         TEE_Param pParams[4],
                         void **ppSessionContext);
    void (*close_session_entry_point) (void *pSessionContext);
     TEE_Result(*invoke_command_entry_point) (void *pSessionContext,
                           MS_U32 nCommandID,
                           MS_U32 nParamTypes,
                           TEE_Param pParams[4]);
     TEE_Result(*core_entries) (MS_U32 nServiceId, MS_U32 nParamTypes,
                   TEE_Param pParam[4]);
} ta_static_head_t;

#endif /* TA_COMM_H */
