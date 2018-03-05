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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVCLKM_H_
#define _DRVCLKM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvCLKM.h
/// @author MStar Semiconductor Inc.
/// @brief Byte CLK control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define CLKM_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'C','L','K','M'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','6'},                          /* 00 ~ 99                                          */  \
    {'0','0','3','1','0','0','5','0'},  /* CL#                                              */  \
    MSIF_OS



typedef enum _CLKM_DbgLvl
{
    E_CLKM_DBGLVL_NONE = 0,      /// no debug message
    E_CLKM_DBGLVL_WARNING,       /// show warning only
    E_CLKM_DBGLVL_ERROR,         /// show error only
    E_CLKM_DBGLVL_INFO,          /// show error & informaiton
    E_CLKM_DBGLVL_ALL,           /// show error, information & funciton name
}CLKM_DbgLvl;


typedef struct _CLKM_Info
{
    MS_BOOL     bInit;
    MS_S32      s32fd;
    MS_S32      s32Mutex;

}CLKM_Info;



typedef enum {
   
    E_MDRV_CMD_CLKM_Init,
    E_MDRV_CMD_CLKM_GET_HANDLE,
    E_MDRV_CMD_CLKM_CLK_GATE_DIABLE,
    E_MDRV_CMD_CLKM_SET_CLK_SOURCE,
    E_MDRV_CMD_CLKM_GET_SRAM_SD_INFO,


} eClkmIoctlOpt;

typedef enum _CLKM_Result
{
    E_CLKM_NOT_SUPPORT = -1
    ,E_CLKM_FAIL = 0
    ,E_CLKM_OK = 1
    ,E_CLKM_TIMEOUT
    ,E_CLKM_BUSY
}CLKM_Result;


typedef struct _CLKM_GetHandle_PARAM
{
    char *s8_Handle_Name;
    MS_S32 s32_Handle;
	
}CLKM_GetHandle_PARAM,*PCLKM_GetHandle_PARAM;


typedef struct _CLKM_CLK_GATE_DISABLE_PARAM
{

    MS_S32 s32_Handle;
	
}CLKM_CLK_GATE_DISABLE_PARAM,*PCLKM_CLK_GATE_DISABLE_PARAM;


typedef struct _CLKM_SET_CLK_SRC_PARAM
{
    MS_S32 s32_Handle;
    char *clk_src_name;
	
}CLKM_SET_CLK_SRC_PARAM,*PCLKM_SET_CLK_SRC_PARAM;


typedef struct _CLKM_GET_SRAM_SD_INFO_PARAM
{
    char *sram_sd_name;
    MS_U16 sram_sd_value;
	
}CLKM_GET_SRAM_SD_INFO_PARAM,*PCLKM_GET_SRAM_SD_INFO_PARAM;




MS_BOOL Drv_CLKM_Init(void);

void Drv_Clkm_Clk_Gate_Disable(MS_S32 s32Handle);

void Drv_Clkm_Set_Clk_Source(MS_S32 s32Handle,char *clk_src_name);

 MS_S32 Drv_Clkm_Get_Handle(char *name);

 MS_U16 Drv_Get_Sram_Sd_Info(char *name);

#ifdef __cplusplus
}
#endif
#endif // _DRVCLKM_H_
