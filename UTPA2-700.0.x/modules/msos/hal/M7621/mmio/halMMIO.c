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
///////////////////////////////////////////////////////////////////////////////
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
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halMMIO.c
/// @brief  memory map io (MMIO) HAL
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "halMMIO.h"
#include "drvMMIO.h"

// for getting mapped IO base from DRV
extern MS_VIRT   _virtPM_Bank         ;
extern MS_U32   _u32PM_Bank_SIZE    ;
extern MS_VIRT   _virtNonPM_Bank      ;
extern MS_U32   _u32NonPM_Bank_SIZE ;
extern MS_VIRT   _virtFRC_Bank      ; //frcr2_integration###
extern MS_U32   _u32FRC_Bank_SIZE ; //frcr2_integration###

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global Variables
//--------------------------------------------------------------------------------------------------
MS_VIRT virt_ge0_mmio_base;


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX
    // assume linux always running on mips
    #define HAL_MMIO_PM_BASE            0x1f000000UL //[MMIO][HAL][001] Base address of PM domain registers
    #define HAL_MMIO_PM_SIZE            0x00A00000UL //[MMIO][HAL][002] Size of PM domain registers
    #define HAL_MMIO_NONPM_BASE         0x1f200000UL //[MMIO][HAL][003] Base address of Non-PM domain registers
    #define HAL_MMIO_NONPM_SIZE         0x01000000UL //[MMIO][HAL][004] Size of Non-PM domain registers
    #define HAL_MMIO_FLASH_BASE0        0x14000000UL //[MMIO][HAL][005] Base address of SPI flash
    #define HAL_MMIO_FLASH_SIZE0        0x01000000UL //[MMIO][HAL][006] Size of SPI flash
    #define HAL_MMIO_FRC_BASE           0x1f800000UL //frcr2_integration### //[MMIO][HAL][026] Base address of FRC R2
    #define HAL_MMIO_FRC_SIZE           0x00013600UL //frcr2_integration### //[MMIO][HAL][027] Size of FRC R2
#elif defined(MSOS_TYPE_LINUX_KERNEL)
    #ifdef CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT
        extern ptrdiff_t mstar_pm_base;
        #define RIU_BASE    mstar_pm_base
    #else
        #define RIU_BASE     0xfd000000UL
    #endif
    #define HAL_MMIO_PM_BASE            RIU_BASE //[MMIO][HAL][007] Base address of PM domain registers (Kernel Mode)
    #define HAL_MMIO_PM_SIZE            0x00A00000UL //[MMIO][HAL][008] Size of PM domain registers (Kernel Mode)
    #define HAL_MMIO_NONPM_BASE         RIU_BASE+0x200000UL //[MMIO][HAL][009] Base address of Non-PM domain registers (Kernel Mode)
    #define HAL_MMIO_NONPM_SIZE         0x01000000UL //[MMIO][HAL][010] Size of Non-PM domain registers (Kernel Mode)
    #define HAL_MMIO_FLASH_BASE0        0x14000000UL //[MMIO][HAL][011] Base address of SPI flash (Kernel Mode)
    #define HAL_MMIO_FLASH_SIZE0        0x01000000UL //[MMIO][HAL][012] Size of SPI flash (Kernel Mode)
    #define HAL_MMIO_FRC_BASE           RIU_BASE+0x800000UL //0x1f800000UL //frcr2_integration### //[MMIO][HAL][028] Base address of FRC R2 (Kernel Mode)
    #define HAL_MMIO_FRC_SIZE           0x00013600UL //frcr2_integration### //[MMIO][HAL][029] Size of FRC R2 (Kernel Mode)
#else
    #if defined (MCU_AEON)
        #define HAL_MMIO_PM_BASE        0xFA000000UL
        #define HAL_MMIO_PM_SIZE        0x00007B80UL
        #define HAL_MMIO_NONPM_BASE     0xFA200000UL
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL
        #define HAL_MMIO_FLASH_BASE0    0xA1000000UL // non-cache // 0xA1000000 for cache
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL
        #define HAL_MMIO_FRC_BASE       0xFA800000UL //frcr2_integration###
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration###
    #elif defined (MCU_ARM_CA7)
        #define HAL_MMIO_PM_BASE        0x1f000000UL
        #define HAL_MMIO_PM_SIZE        0x00007B80UL
        #define HAL_MMIO_NONPM_BASE     0x1f200000UL
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL
        #define HAL_MMIO_FLASH_BASE0    0x14000000UL
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL
        #define HAL_MMIO_FRC_BASE       0x1f800000UL //frcr2_integration###
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration###
    #elif defined (MCU_ARM_CA53)
        #define HAL_MMIO_PM_BASE        0x1f000000UL //[MMIO][HAL][013] Base address of PM domain registers (Non-OS)
        #define HAL_MMIO_PM_SIZE        0x00007B80UL //[MMIO][HAL][014] Size of PM domain registers (Non-OS)
        #define HAL_MMIO_NONPM_BASE     0x1f200000UL //[MMIO][HAL][015] Base address of Non-PM domain registers (Non-OS)
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL //[MMIO][HAL][016] Size of Non-PM domain registers (Non-OS)
        #define HAL_MMIO_FLASH_BASE0    0x14000000UL //[MMIO][HAL][017] Base address of SPI flash (Non-OS)
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL  //[MMIO][HAL][018] Size of SPI flash (Non-OS)
        #define HAL_MMIO_FRC_BASE       0x1f800000UL //frcr2_integration### //[MMIO][HAL][030] Base address of FRC R2 (Non-OS)
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration### //[MMIO][HAL][031] Size of FRC R2 (Non-OS)
    #else
        #error "Please choose MCU";
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U16  HAL_MMIO_GetType(MS_U32 u32Module)
{
    switch (u32Module)
    {
    //HAL_MMIO_PM_BANK
    case MS_MODULE_PM       :
    case MS_MODULE_IR       :
    case MS_MODULE_ISP      :
    case MS_MODULE_PWS      :
        return DRV_MMIO_PM_BANK;

    //HAL_MMIO_NONPM_BANK
    case MS_MODULE_HW       :
    case MS_MODULE_CHIPTOP  :
    case MS_MODULE_MIU      :
    case MS_MODULE_ACE      :
    case MS_MODULE_AUDIO    :
    case MS_MODULE_AVD      :
    case MS_MODULE_BDMA     :
    case MS_MODULE_DLC      :
    case MS_MODULE_DMD      :
    case MS_MODULE_GE       :
    case MS_MODULE_GOP      :
    case MS_MODULE_GPIO     :
    case MS_MODULE_HVD      :
    case MS_MODULE_HWI2C    :
    case MS_MODULE_IRQ      :
    case MS_MODULE_JPD      :
    case MS_MODULE_MBX      :
    case MS_MODULE_MFE      :
    case MS_MODULE_MHEG5    :
    case MS_MODULE_MVD      :
    case MS_MODULE_MVOP     :
    case MS_MODULE_RVD      :
    case MS_MODULE_TSP      :
    case MS_MODULE_UART     :
    case MS_MODULE_VPU      :
    case MS_MODULE_XC       :
    case MS_MODULE_PCMCIA   :
    case MS_MODULE_PFSH     :
    case MS_MODULE_PNL      :
    case MS_MODULE_PWM      :
    case MS_MODULE_SEM      :
    case MS_MODULE_VBI      :
    case MS_MODULE_VIF      :
    case MS_MODULE_DIP      :
    case MS_MODULE_MPIF     :
    case MS_MODULE_MMFILEIN :
    case MS_MODULE_GPD      :
    case MS_MODULE_TSO      :
    case MS_MODULE_CMDQ     :
        return DRV_MMIO_NONPM_BANK;

    case MS_MODULE_SC       :
        return DRV_MMIO_SC_BANK;

     //HAL_MMIO_FLASH_BANK0
    case MS_MODULE_FLASH    :
        return DRV_MMIO_FLASH_BANK0;
    case MS_MODULE_FRC      : //frcr2_integration###
        return DRV_MMIO_FRC_BANK; //frcr2_integration###

    default:
        return 0xFFFF; //undefine type
    }

    return 0xFFFF; //undefine type
}


MS_BOOL HAL_MMIO_GetBase(MS_PHY* virtBaseAddr, MS_PHY* pu32BaseSize, MS_U32 u32BankType)
{
    MS_BOOL bRet = TRUE;

    *virtBaseAddr = 0;
    *pu32BaseSize = 0;
    switch (u32BankType)
    {
    case DRV_MMIO_PM_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_PM_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_PM_SIZE;
        break;
    case DRV_MMIO_NONPM_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_NONPM_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_NONPM_SIZE;
        break;
    case DRV_MMIO_FLASH_BANK0:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_FLASH_BASE0;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_FLASH_SIZE0;
        break;
    case DRV_MMIO_FRC_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_FRC_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_FRC_SIZE;
        break;
    default:
//        MS_ASSERT(0);
        bRet = FALSE;
        break;
    }

    return bRet;
}


// @NOTE: Only run after MMIO_Init
MS_BOOL HAL_MMIO_GetIPBase(MS_VIRT *virtBaseAddr, MS_U16 u16BankType)
{
    *virtBaseAddr = 0;

    // if MMIO_Init is not yet initialized.
    if ( (_u32PM_Bank_SIZE == 0x0) || (_u32NonPM_Bank_SIZE == 0x0) || (_u32FRC_Bank_SIZE == 0x0) ) //frcr2_integration###
    {
        return FALSE;
    }

    switch (u16BankType)
    {
    case DRV_MMIO_SC_BANK:
        *virtBaseAddr =_virtNonPM_Bank  + 0x00005200; // 0xBF220C00, 0xBF220C80
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
static void _chip_flush_miu_pipe(void)
{
    unsigned int    dwReadData = 0;

//[MMIO][HAL][019] Flush MIU pipe [START]
     //toggle the flush miu pipe fire bit
    *(volatile unsigned int *)(HAL_MMIO_PM_BASE + (0x10188A << 1)) &= ~(0x0001);
    *(volatile unsigned int *)(HAL_MMIO_PM_BASE + (0x10188A << 1)) |= 0x0001;

    do
    {
        dwReadData = *(volatile unsigned int *)(HAL_MMIO_PM_BASE+ (0x1018A0 << 1));
        dwReadData &= BIT(12);  //Check Status of Flush Pipe Finish

    } while(dwReadData == 0);
//[MMIO][HAL][019] Flush MIU pipe [END]
}
#endif
#endif

void HAL_MMIO_FlushMemory(void)
{
#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
    _chip_flush_miu_pipe();
#endif
#endif
}

void HAL_MMIO_ReadMemory(void)
{
#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
    _chip_flush_miu_pipe();
#endif
#endif
}


