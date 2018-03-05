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
#ifndef MHAL_MENULOAD_H
#define MHAL_MENULOAD_H

//Select the source to trigger menuload
#define TRIG_SRC_FE_VFDE        0
#define TRIG_SRC_RE_VSYNC       1
#define TRIG_SRC_FE_VSYNC       2
#define TRIG_SRC_DELAY_LINE     3

#define OPM_LOCK_INIT_STATE     0
#define OPM_LOCK_FE_VSYNC       1
#define OPM_LOCK_TRAIN_TRIG_P   2
#define OPM_LOCK_DS             3

#define  MS_MLOAD_CMD_ALIGN     4
#define  MS_MLOAD_REG_LEN       0x04 // how many data in one MIU request
#define  MS_MLOAD_CMD_LEN       BYTE_PER_WORD
#define  MS_MLOAD_MEM_BASE_UNIT BYTE_PER_WORD
#define  MS_MLOAD_BUS_WIDTH     (16)
#define  MS_MLOAD_MAX_CMD_CNT   192
#define  MS_MLOAD_NULL_CMD      0xFF010000
#define  MS_MLOAD_DUMMY_CMD_CNT(x)  ((x+0x0003)&~0x0003)
#define  MS_MLOAD_END_CMD(x) (0x1F020000 | (MS_MLOAD_REG_LEN) | (_XC_Device_Offset[psXCInstPri->u32DeviceID] << 24))
#define  MS_MLOAD_MIU_BUS_SEL   0x00

#define  MS_MLOAD_CMD_LEN_64BITS 8 //8 bytes
#define  MS_MLOAD_NULL_CMD_SPREAD_MODE    (((MS_U64)0x13FF<<23)|((MS_U64)0x01<<16)|((MS_U64)0x0000))  //0x00000009FF810000
#define  MS_MLOAD_END_CMD_SPREAD_MODE(x)   ( ((MS_U64)0x0000<<48) | (((MS_U64)0x131F<<23)|((MS_U64)0x02<<16)|((MS_U64)0x0000)) | (MS_MLOAD_REG_LEN) ) //(0x000000098F820000 | (MS_MLOAD_REG_LEN)) = 0x000000098F820004
#define  MS_MLOAD_END_CMD_DEV1_SPREAD_MODE(x)   ( ((MS_U64)0x0000<<48) | (((MS_U64)0x139F<<23)|((MS_U64)0x02<<16)|((MS_U64)0x0000)) | (MS_MLOAD_REG_LEN) ) //(0x000000098F820000 | (MS_MLOAD_REG_LEN)) = 0x000000098F820004

#define  MS_MLG_REG_LEN         0x40 // how many data in one MIU request
#define  MS_MLG_CMD_LEN         BYTE_PER_WORD
#define  MS_MLG_MEM_BASE_UNIT   BYTE_PER_WORD

typedef struct
{
    union
    {
        struct
        {
        	MS_U16 u16Data;
        	MS_U8 u8Addr;
        	MS_U8 u8Bank;
        };
        MS_U32 u32Cmd;
    };
}MS_MLoad_Data;

typedef struct
{
    union
    {
        struct
        {
        	MS_U16 u16Data;
        	MS_U8  u8Addr; //addr 0 ~ 0x7F
        	MS_U8  u8Bank; //subbank 0 ~ 0xFF
        	MS_U32 u32NoUse;
        };
        MS_U64 u64Cmd;
    };
}MS_MLoad_Data_64Bits_SubBank;

#ifdef MLG_1024 // Gamma_1024
typedef struct
{
    MS_U64 BData0 : 12;     //0
    MS_U64 BData1 : 12;
    MS_U64 BData2 : 12;
    MS_U64 GData0 : 12;
    MS_U64 GData1 : 12;
    MS_U64 GData2_L : 4;      //63

    MS_U64 GData2_H : 8;      //64
    MS_U64 RData0 : 12;
    MS_U64 RData1 : 12;
    MS_U64 RData2 : 12;
    MS_U64 Dummy0 :20;      //127

    MS_U64 Dummy1 : 16 ;       //128
    MS_U64 BEnable : 1 ;
    MS_U64 GEnable : 1 ;
    MS_U64 REnable : 1 ;
    MS_U64 Dummy2 :45;      //191

    MS_U64 Dummy3;      //192~255
} MS_SC_MLG_TBL;
#else // Gamma_256 or not support MLG case
typedef struct
{
    MS_U16 u16B;
    MS_U16 u16G;
    MS_U16 u16R;
    MS_U16 u16Enable;
    MS_U16 u16Dummy[4];
} MS_SC_MLG_TBL;
#endif

typedef enum
{
    MLOAD_TRIGGER_BY_OP_SYNC=0,
    MLOAD_TRIGGER_BY_IP_MAIN_SYNC=1,
    MLOAD_TRIGGER_BY_IP_SUB_SYNC=2,
    MLOAD_TRIGGER_BY_SW=3,
    MLOAD_TRIGGER_MAX,
}MLoad_Trigger_Sync;

typedef enum
{
    MLoad_WD_Timer_Reset_DMA = 0,
    MLoad_WD_Timer_Reset_MIU = 1,
    MLoad_WD_Timer_Reset_ALL = 3,
    MLoad_WD_Timer_Reset_MAX,
}MLoad_WD_Timer_Reset_Type;

MS_U16 Hal_XC_MLoad_get_status(void *pInstance);
void Hal_XC_MLoad_set_on_off(void *pInstance, MS_BOOL bEn);
void Hal_XC_MLoad_set_len(void *pInstance, MS_U16 u16Len);
void Hal_XC_MLoad_set_depth(void *pInstance, MS_U16 u16depth);
void Hal_XC_MLoad_set_miusel(void *pInstance, MS_U8 u8MIUSel);
MS_U8 Hal_XC_MLoad_get_miusel(void *pInstance);
void Hal_XC_MLoad_set_base_addr(void *pInstance, MS_PHY u32addr);
void Hal_XC_MLoad_set_trigger_timing(void *pInstance, MS_U16 u16sel);
void Hal_XC_MLoad_set_opm_lock(void *pInstance, MS_U16 u16sel);
void Hal_XC_MLoad_set_trigger_delay(void *pInstance, MS_U16 u16delay);
void Hal_XC_MLoad_set_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp);
MS_BOOL Hal_XC_MLoad_get_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp);
void Hal_XC_MLoad_Set_riu(void *pInstance, MS_BOOL bEn);
MS_BOOL Hal_XC_MLoad_GetCaps(void *pInstance);
void Hal_XC_MLoad_set_riu_cs(void *pInstance, MS_BOOL bEn);
#define Hal_XC_MLoad_set_opm_arbiter_bypass(args...)
#define Hal_XC_MLoad_set_miu_bus_sel(args...)
#define Hal_XC_MLoad_enable_watch_dog(args...)
#define Hal_XC_MLoad_set_watch_dog_time_delay(args...)
#define Hal_XC_MLoad_enable_watch_dog_reset(args...)


MS_BOOL Hal_XC_MLG_GetCaps(void *pInstance);
MS_U16 Hal_XC_MLG_get_status(void *pInstance);
#define Hal_XC_MLG_set_on_off(args...)
#define Hal_XC_MLG_set_len(args...)
#define Hal_XC_MLG_set_depth(args...)
#define Hal_XC_MLG_set_base_addr(args...)
#define Hal_XC_MLG_set_trigger_timing(args...)
#define Hal_XC_MLG_set_trigger_delay(args...)
#define Hal_XC_MLG_set_init_addr(args...)

void Hal_XC_MLoad_Enable_64BITS_COMMAND(void *pInstance,MS_BOOL bEn);
void Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(void *pInstance,MS_BOOL bEn);
void Hal_XC_MLoad_Command_Format_initial(void *pInstance);
void Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(void *pInstance);
MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL Hal_XC_MLoad_parsing_64bits_spreadMode_NonXC(void *pInstance,MS_U64 u64Cmd, MS_U32 *u32Addr, MS_U16 *u16Data);
MS_U64 Hal_XC_MLoad_Gen_64bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_U32 Hal_XC_MLoad_Gen_32bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL Hal_XC_MLoad_parsing_32bits_subBankMode(void *pInstance, MS_U32 u32MloadData, MS_U32 *pu32Addr, MS_U16 *pu16Data);
MS_U16 Hal_XC_MLoad_Get_Depth(void *pInstance, MS_U16 u16CmdCnt);

void Hal_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync);
void Hal_SC_ControlMloadTrig(void *pInstance);
void Hal_XC_MLoad_set_BitMask(void *pInstance,MS_BOOL enable);
#endif

