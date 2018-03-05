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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMMIO.c
/// @brief  MMIO SYS Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "MsCommon.h"
#include "drvMMIO.h"
#include "halMMIO.h"
#include "regCHIP.h"

/* Use 'I' as magic number */
#define MIOMAP_IOC_MAGIC                    'I'
#define MMIO_IOC_SET_MAP                    _IOW (MIOMAP_IOC_MAGIC, 0x01, MIOMap_Info_t)

/* MIOMAP_IOC_INFO */
typedef struct
{
    MS_U64                                  virtAddr;
    MS_U64                                  u32Size;
} MIOMap_Info_t;

#else // #ifdef MSOS_TYPE_LINUX

#include "MsCommon.h"
#include "drvMMIO.h"
#include "halMMIO.h"
#include "regCHIP.h"

#endif
#include "MsVersion.h"
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define MMIO_DEBUG
#endif


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MMIO_DEBUG
    #define MMIO_ERROR(fmt, args...)         MS_DEBUG_MSG(ULOGE("MMIO","[%06d]     " fmt, __LINE__, ##args))
    #define MMIO_WARN(fmt, args...)         MS_DEBUG_MSG(ULOGW("MMIO","[%06d]     " fmt, __LINE__, ##args))
    #define MMIO_PRINT(fmt, args...)         MS_DEBUG_MSG(ULOGI("MMIO", "[%06d]     " fmt, __LINE__, ##args))
    #define MMIO_ASSERT(_bool, _f)          if (!(_bool)) { MS_DEBUG_MSG(ULOGF("MMIO",_f)); MS_ASSERT(0);) }
#else
    #define MMIO_ERROR(fmt, args...)        MS_DEBUG_MSG(while (0))
    #define MMIO_WARN(fmt, args...)         MS_DEBUG_MSG(while (0))
    #define MMIO_PRINT(fmt, args...)        MS_DEBUG_MSG(while (0))
    #define MMIO_ASSERT(_bool, _f)          if (!(_bool)) { MS_DEBUG_MSG((_f)); }
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX
MS_VIRT                               _virtPM_Bank                 = 0x0 ;
MS_PHY                              _u32PM_Bank_SIZE            = 0x0 ;
MS_VIRT                               _virtNonPM_Bank              = 0x0 ;
MS_PHY                              _u32NonPM_Bank_SIZE         = 0x0 ;
MS_VIRT                               _virtFlash_Bank0             = 0x0 ;
MS_PHY                              _u32Flash_Bank0_SIZE        = 0x0 ;
MS_VIRT                               _virtOTP_Bank                = 0x0 ;
MS_PHY                              _u32OTP_BankSize            = 0x0 ;
MS_VIRT                               _virtOTP_Bank2               = 0x0 ;  //OTP raw data area
MS_PHY                              _u32OTP_Bank2Size           = 0x0 ;

MS_VIRT                               _virtFRC_Bank                = 0x0 ;
MS_PHY                              _u32FRC_Bank_SIZE           = 0x0 ;

MS_VIRT                               _virtNSK_Bank                = 0x0 ; // U3 Only
MS_PHY                               _u32NSK_BankSize            = 0x0 ;

MS_VIRT                               _virtSPRAM_Bank              = 0x0 ; // Scratchpad memory K2 Only
MS_PHY                              _u32SPRAM_BankSize          = 0x0 ;
#else
MS_PHY                              _virtPM_Bank                 = 0x0 ;
MS_PHY                              _u32PM_Bank_SIZE            = 0x0 ;
MS_PHY                              _virtNonPM_Bank              = 0x0 ;
MS_PHY                              _u32NonPM_Bank_SIZE         = 0x0 ;
MS_PHY                              _virtFlash_Bank0             = 0x0 ;
MS_PHY                              _u32Flash_Bank0_SIZE        = 0x0 ;
MS_PHY                              _virtOTP_Bank                = 0x0 ;
MS_PHY                              _u32OTP_BankSize            = 0x0 ;
MS_PHY                              _virtOTP_Bank2               = 0x0 ;  //OTP raw data area
MS_PHY                              _u32OTP_Bank2Size           = 0x0 ;

MS_PHY                              _virtFRC_Bank                = 0x0 ;
MS_PHY                              _u32FRC_Bank_SIZE           = 0x0 ;

MS_PHY                               _virtNSK_Bank                = 0x0 ; // U3 Only
MS_PHY                               _u32NSK_BankSize            = 0x0 ;

MS_PHY                              _virtSPRAM_Bank              = 0x0 ; // Scratchpad memory K2 Only
MS_PHY                              _u32SPRAM_BankSize          = 0x0 ;

#endif
static MSIF_Version _drv_mmio_version = {
    .DDI = { MMIO_DRV_VERSION },
};
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_MSOS_GetLibVer
/// @brief \b Function  \b Description: Show the MSOS driver version
/// @param ppVersion    \b Out: Library version string
/// @return             \b Result
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MMIO_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_mmio_version;
    return TRUE;
}

#ifdef MSOS_TYPE_LINUX

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32                       _s32MIOMapFd                = -1;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_MMIO_Init(void)
{
    MIOMap_Info_t       stMIOMapInfo;

    MS_PHY              PM_Bank ;
    MS_PHY              PM_Bank_SIZE ;
    MS_PHY              NonPM_Bank ;
    MS_PHY              NonPM_Bank_SIZE ;
    MS_PHY              Flash_Bank0 ;
    MS_PHY              Flash_Bank0_SIZE ;
    MS_PHY              FRC_Bank ;
    MS_PHY              FRC_Bank_SIZE ;
    MS_PHY              bank_addr = 0;
    MS_PHY              bank_size = 0;

    if (0 <= _s32MIOMapFd)
    {
        MMIO_WARN("%s is initiated more than once\n", __FUNCTION__);
        return FALSE;
    }

    if (0 > (_s32MIOMapFd = open("/dev/miomap", O_RDWR)))
    {
        MMIO_ERROR("Open /dev/miomap fail\n");
        MS_ASSERT(0);
        return FALSE;
    }


    if (FALSE == HAL_MMIO_GetBase(&PM_Bank, &PM_Bank_SIZE, DRV_MMIO_PM_BANK))
    {
        MS_ASSERT(0);
    }
    if (FALSE == HAL_MMIO_GetBase(&NonPM_Bank, &NonPM_Bank_SIZE, DRV_MMIO_NONPM_BANK))
    {
        MS_ASSERT(0);
    }
    if (FALSE == HAL_MMIO_GetBase(&Flash_Bank0, &Flash_Bank0_SIZE, DRV_MMIO_FLASH_BANK0))
    {
        MS_ASSERT(0);
    }

    if (FALSE == HAL_MMIO_GetBase(&FRC_Bank, &FRC_Bank_SIZE, DRV_MMIO_FRC_BANK))
    {
        MS_ASSERT(0);
    }

    if (PM_Bank_SIZE)
    {
        stMIOMapInfo.virtAddr = PM_Bank ;
        stMIOMapInfo.u32Size = PM_Bank_SIZE ;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP , &stMIOMapInfo))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (_virtPM_Bank= (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ| PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }
        _u32PM_Bank_SIZE = PM_Bank_SIZE ;
        MMIO_PRINT("PM Bank mapping success. Base=[%lx], Size=[%x]\n", (MS_VIRT)_virtPM_Bank, (MS_PHY)_u32PM_Bank_SIZE);
    }

    if (NonPM_Bank_SIZE)
    {
        stMIOMapInfo.virtAddr = NonPM_Bank ;
        stMIOMapInfo.u32Size = NonPM_Bank_SIZE ;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP , &stMIOMapInfo))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (_virtNonPM_Bank= (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ| PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }
        _u32NonPM_Bank_SIZE  = NonPM_Bank_SIZE ;
        MMIO_PRINT("NonPM Bank mapping success. Base=[%lx], Size=[%x]\n",(MS_VIRT) _virtNonPM_Bank, (MS_PHY)_u32NonPM_Bank_SIZE);
    }

    if (Flash_Bank0_SIZE)
    {
        stMIOMapInfo.virtAddr = Flash_Bank0 ;
        stMIOMapInfo.u32Size = Flash_Bank0_SIZE ;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP , &stMIOMapInfo))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (_virtFlash_Bank0= (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ| PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }
        _u32Flash_Bank0_SIZE = Flash_Bank0_SIZE ;
        MMIO_PRINT("Flash Bank0 mapping success. Base=[%lx], Size=[%x]\n", (MS_PHY)_virtFlash_Bank0, (MS_PHY)_u32Flash_Bank0_SIZE);
    }

    if (FRC_Bank_SIZE)
    {
        stMIOMapInfo.virtAddr = FRC_Bank ;
        stMIOMapInfo.u32Size = FRC_Bank_SIZE ;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP , &stMIOMapInfo))
        {
            return FALSE;
        }
        if ((MS_VIRT)MAP_FAILED == (_virtFRC_Bank= (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ| PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0)))
        {
            MS_ASSERT(0);
            return FALSE;
        }
        _u32FRC_Bank_SIZE  = FRC_Bank_SIZE ;
        MMIO_PRINT("FRC Bank mapping success. Base=[%lx], Size=[%x]\n",(MS_VIRT) _virtFRC_Bank, (MS_PHY)_u32FRC_Bank_SIZE);
    }
/*
    Not every chip support OTP / NSK, the return value of MMIO_GetBase could be FALSE
*/

    if (HAL_MMIO_GetBase(&bank_addr, &bank_size, DRV_MMIO_OTP_BANK))
    {
        stMIOMapInfo.virtAddr = bank_addr;
        stMIOMapInfo.u32Size = bank_size;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP, &stMIOMapInfo))
        {
            return FALSE;
        }
        _virtOTP_Bank = (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0);
        if ((MS_VIRT)MAP_FAILED == _virtOTP_Bank)
        {
            _virtOTP_Bank = 0x0;
            MS_ASSERT(0);
            return FALSE;
        }
        _u32OTP_BankSize = bank_size;
        MMIO_PRINT("OTP Bank mapping success. Base=[%lx], Size=[%x]\n", (MS_VIRT)_virtOTP_Bank, (MS_PHY)_u32OTP_BankSize);
    }

/*    map OTP base for OTP readable area (OTP raw data)   */
    if (HAL_MMIO_GetBase(&bank_addr, &bank_size, DRV_MMIO_OTP_BANK2))
    {
        stMIOMapInfo.virtAddr = bank_addr;
        stMIOMapInfo.u32Size = bank_size;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP, &stMIOMapInfo))
        {
            return FALSE;
        }
        _virtOTP_Bank2 = (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0);
        if ((MS_VIRT)MAP_FAILED == _virtOTP_Bank2)
        {
            _virtOTP_Bank2 = 0x0;
            MS_ASSERT(0);
            return FALSE;
        }
        _u32OTP_Bank2Size = bank_size;
//        MMIO_PRINT("OTP Bank RAW mapping success. Base=[%lx], Size=[%x]\n", (MS_VIRT)_u32OTP_Bank2, (MS_PHY)_u32OTP_Bank2Size);
    }

    // K2 Only Scratchpad memory/SPRAM
    if (HAL_MMIO_GetBase(&bank_addr, &bank_size, DRV_MMIO_SPRAM_BANK))
    {
        stMIOMapInfo.virtAddr = bank_addr;
        stMIOMapInfo.u32Size = bank_size;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP, &stMIOMapInfo))
        {
            return FALSE;
        }
        _virtSPRAM_Bank = (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0);
        if ((MS_VIRT)MAP_FAILED == _virtSPRAM_Bank)
        {
            _virtSPRAM_Bank = 0x0;
            MS_ASSERT(0);
            return FALSE;
        }
        _u32SPRAM_BankSize = bank_size;
        MMIO_PRINT("SPRAM Bank mapping success. Base=[%lx], Size=[%x]\n", (MS_VIRT)_virtSPRAM_Bank, (MS_PHY)_u32SPRAM_BankSize);
    }

    // U3 Only
    if (HAL_MMIO_GetBase(&bank_addr, &bank_size, DRV_MMIO_NSK_BANK))
    {
        stMIOMapInfo.virtAddr = bank_addr;
        stMIOMapInfo.u32Size = bank_size;
        if (ioctl(_s32MIOMapFd, MMIO_IOC_SET_MAP, &stMIOMapInfo))
        {
            return FALSE;
        }
        _virtNSK_Bank = (MS_VIRT)mmap(0, stMIOMapInfo.u32Size, PROT_READ | PROT_WRITE, MAP_SHARED, _s32MIOMapFd, 0);
        if ((MS_VIRT)MAP_FAILED == _virtNSK_Bank)
        {
            _virtNSK_Bank = 0x0;
            MS_ASSERT(0);
            return FALSE;
        }
        _u32NSK_BankSize = bank_size;
//        MMIO_PRINT("NSK Bank mapping success. Base=[%lx], Size=[%x]\n", (MS_VIRT)_u32NSK_Bank, (MS_PHY)_u32NSK_BankSize);
    }

    return TRUE;

}


MS_BOOL MDrv_MMIO_Close(void)
{
    if (0 > _s32MIOMapFd)
    {
        MMIO_WARN("%s is closed before initiated\n", __FUNCTION__);
        return FALSE;
    }
    munmap((void *)_virtPM_Bank, _u32PM_Bank_SIZE);
    munmap((void *)_virtNonPM_Bank, _u32NonPM_Bank_SIZE );
    munmap((void *)_virtFlash_Bank0, _u32Flash_Bank0_SIZE);
    munmap((void *)_virtFRC_Bank, _u32FRC_Bank_SIZE);
    close(_s32MIOMapFd);
    _s32MIOMapFd = -1;
    return TRUE;
}


MS_BOOL MDrv_MMIO_GetBASE(MS_VIRT *virtBaseaddr, MS_PHY *u32Basesize, MS_U32 u32Module)
{

    MS_U16              u16Type;

    u16Type = HAL_MMIO_GetType(u32Module);

    if (u16Type == DRV_MMIO_PM_BANK)
    {
        *virtBaseaddr = _virtPM_Bank;
        *u32Basesize = _u32PM_Bank_SIZE;
        return TRUE ;
    }
    if (u16Type == DRV_MMIO_NONPM_BANK)
    {
        *virtBaseaddr = _virtNonPM_Bank;
        *u32Basesize = _u32NonPM_Bank_SIZE;
        return TRUE ;
    }
    if (u16Type == DRV_MMIO_FLASH_BANK0 )
    {
        *virtBaseaddr = _virtFlash_Bank0;
        *u32Basesize = _u32Flash_Bank0_SIZE;
        return TRUE ;
    }
    if ((u16Type == DRV_MMIO_OTP_BANK) && (_u32OTP_BankSize)) // not support if BankSize = 0
    {
        *virtBaseaddr = _virtOTP_Bank;
        *u32Basesize = _u32OTP_BankSize;
        return TRUE ;
    }
    if ((u16Type == DRV_MMIO_OTP_BANK2) && (_u32OTP_Bank2Size)) // not support if BankSize = 0
    {
        *virtBaseaddr = _virtOTP_Bank2;
        *u32Basesize = _u32OTP_Bank2Size;
        return TRUE ;
    }
    if (((u16Type == DRV_MMIO_SPRAM_BANK) && (_u32SPRAM_BankSize))) // not support if BankSize = 0
    {
        *virtBaseaddr = _virtSPRAM_Bank;
        *u32Basesize = _u32SPRAM_BankSize;
        return TRUE ;
    }
    if (u16Type == DRV_MMIO_FRC_BANK)
    {
        *virtBaseaddr = _virtFRC_Bank;
        *u32Basesize = _u32FRC_Bank_SIZE;
        return TRUE ;
    }

    // Particular HW base address
    if (HAL_MMIO_GetIPBase(virtBaseaddr, u16Type))
    {
        *u32Basesize = 0;
        return TRUE;
    }

    return FALSE ;

}


#else // #ifdef MSOS_TYPE_LINUX


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

MS_BOOL MDrv_MMIO_Init(void)
{
    // @TODO: remove the getbase code in MMIO_GetBASE?
    HAL_MMIO_GetBase(&_virtPM_Bank,      &_u32PM_Bank_SIZE,      DRV_MMIO_PM_BANK);
    HAL_MMIO_GetBase(&_virtNonPM_Bank,   &_u32NonPM_Bank_SIZE,   DRV_MMIO_NONPM_BANK);
    HAL_MMIO_GetBase(&_virtFlash_Bank0,  &_u32Flash_Bank0_SIZE,  DRV_MMIO_FLASH_BANK0);
    HAL_MMIO_GetBase(&_virtOTP_Bank,     &_u32OTP_BankSize,      DRV_MMIO_OTP_BANK); // query DRV_MMIO capability to HAL
    HAL_MMIO_GetBase(&_virtOTP_Bank2,    &_u32OTP_Bank2Size,     DRV_MMIO_OTP_BANK2); // query DRV_MMIO capability to HAL
    HAL_MMIO_GetBase(&_virtSPRAM_Bank,     &_u32SPRAM_BankSize,      DRV_MMIO_SPRAM_BANK);
    HAL_MMIO_GetBase(&_virtFRC_Bank,   &_u32FRC_Bank_SIZE,   DRV_MMIO_FRC_BANK);

    // U3 Only
    HAL_MMIO_GetBase(&_virtNSK_Bank,     &_u32NSK_BankSize,      DRV_MMIO_NSK_BANK); // U3 Only

    return TRUE;

}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MDrv_MMIO_Init);
#endif



MS_BOOL MDrv_MMIO_Close(void)
{
    return TRUE;
}


MS_BOOL MDrv_MMIO_GetBASE(MS_VIRT *virtBaseaddr, MS_PHY *u32Basesize, MS_U32 u32Module)
{

    MS_BOOL             bRet;
    MS_U16              u16Type;
	MS_PHY virtBasetemp = 0;
	MS_PHY pu32Basetemp = 0;

    u16Type = HAL_MMIO_GetType(u32Module);

    // @TODO: Leave only in MMIO_Init?
    switch (u16Type)
    {
    case DRV_MMIO_PM_BANK:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_PM_BANK);
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_NONPM_BANK:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_NONPM_BANK);
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_FLASH_BANK0:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_FLASH_BANK0);
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_OTP_BANK:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_OTP_BANK); // query DRV_MMIO capability to HAL
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_OTP_BANK2:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_OTP_BANK2); // query DRV_MMIO capability to HAL
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_SPRAM_BANK:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_SPRAM_BANK); // query DRV_MMIO capability to HAL
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    case DRV_MMIO_FRC_BANK:
		bRet = HAL_MMIO_GetBase(&virtBasetemp, &pu32Basetemp, DRV_MMIO_FRC_BANK);
		*virtBaseaddr = virtBasetemp;
		*u32Basesize = pu32Basetemp;
        break;
    default:
        bRet = FALSE;
        break;
    }

    // specific HW base address
    if (bRet == FALSE)
    {
        bRet = HAL_MMIO_GetIPBase((MS_VIRT*)virtBaseaddr, u16Type);
    }

    return bRet;

}

#endif
