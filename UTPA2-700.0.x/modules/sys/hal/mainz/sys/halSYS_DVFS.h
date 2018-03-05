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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_SYS_DVFS_H_
#define _HAL_SYS_DVFS_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DVFS_INFO(x, args...)                   {printf(x, ##args);}
#define DVFS_DEBUG(x, args...)                  //{printf(x, ##args);}

#define CONFIG_DVFS_ENABLE_PATTERN              0x3697
#define CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT   0x2454

#define CONFIG_DVFS_CPU_POWER_I2C_ENABLE        1
#define CONFIG_DVFS_CORE_POWER_I2C_ENABLE       0
#define CONFIG_DVFS_CPU_POWER_GPIO_ENABLE       0
#define CONFIG_DVFS_CORE_POWER_GPIO_ENABLE      0

#define CONFIG_DVFS_DELAY_US                    1
#define CONFIG_DVFS_MUTEX_WAIT_TIME             50

#define CONFIG_DVFS_CPU_POWER                   0
#define CONFIG_DVFS_CORE_POWER                  1
#define CONFIG_DVFS_STR_INIT                    0xFE

#define CONFIG_DVFS_CPU_POWER_SHIFT_PRADO(x)    (x ? 67 : 67)
//#define CONFIG_DVFS_CPU_POWER_SHIFT_PRADO(x)    (x ? 78 : 78)
#define CONFIG_DVFS_CPU_POWER_DEFAULT(x)        (x ? 105 : 105)
#define CONFIG_DVFS_CPU_POWER_STEP(x)           (x ? 3 : 3)

#define CONFIG_DVFS_CORE_POWER_SHIFT            66
#define CONFIG_DVFS_CORE_POWER_DEFAULT          95
#define CONFIG_DVFS_CORE_POWER_L1               90
#define CONFIG_DVFS_CORE_POWER_STEP             3

#define CONFIG_DVFS_POWER_SWI2C_BUS             0x00
#if (CONFIG_DVFS_CPU_POWER_I2C_ENABLE)
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CPU(x)     (x ? 0x82 : 0x80)
#define CONFIG_DVFS_POWER_SWI2C_CLUSTER0        0x00
#define CONFIG_DVFS_POWER_SWI2C_CLUSTER1        0x01
#endif
#if (CONFIG_DVFS_CORE_POWER_I2C_ENABLE)
#define CONFIG_DVFS_POWER_SWI2C_CORE_ID         0x82    //0x82
#define CONFIG_DVFS_POWER_SWI2C_ADDR_CORE       ((CONFIG_DVFS_POWER_SWI2C_BUS << 8) | CONFIG_DVFS_POWER_SWI2C_CORE_ID)
#endif

#define CONFIG_DVFS_CHIP_ID_UNKNOWN             0xFF
#define CONFIG_DVFS_CHIP_ID_PRADA               0x79
#define CONFIG_DVFS_CHIP_ID_PRADO               0x9A
#define CONFIG_DVFS_CORE_POWER_SHIFT_PRADO      (69 - 2)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  bDvfsInitOk;
    MS_U32  dwVidSetting;
    MS_U32  dwPowerChipId;

} MSTAR_DVFS_INFO;

typedef struct
{
    unsigned int    dwCpuClock;
    unsigned int    dwVoltage;
    unsigned int    dwVoltageType;
    unsigned int    dwDataExchangeCount;
    unsigned int    dwCluster;

} MSTAR_DVFS_READ_INFO;

typedef struct
{
    unsigned int    dwDataExchangeCount;
    int             dwDataExchangeResult;
    unsigned int    dwCluster;
} MSTAR_DVFS_WRITE_INFO;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void SysDvfsProc(void);
void SysDvfsInit(void);
void SysDvfsCpuPowerInit(void);
void SysDvfsCpuPowerAdjustment(MS_U32 dwCpuPowerVoltage, MS_U8 byCluster);
void SysDvfsCorePowerInit(void);
void SysDvfsCorePowerAdjustment(MS_U32 dwCorePowerVoltage, MS_U8 byCluster);

#endif // _HAL_SYS_DVFS_H_

