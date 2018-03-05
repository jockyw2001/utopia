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
/// file    drvSERFLASH.c
/// @brief  Serial Flash Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvSERFLASH.h"
#include "MsOS.h"

#include "ULog.h"

// Internal Definition
#include "regSERFLASH.h"
#include "halSERFLASH.h"
#include "drvBDMA.h"
#include "drvMMIO.h"
#include "utopia.h"

#define TAG_SERFLASH "SERFLASH"

#if defined(MSOS_TYPE_LINUX)
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// !!! Uranus Serial Flash Notes: !!!
//  - The clock of DMA & Read via XIU operations must be < 3*CPU clock
//  - The clock of DMA & Read via XIU operations are determined by only REG_ISP_CLK_SRC; other operations by REG_ISP_CLK_SRC only
//  - DMA program can't run on DRAM, but in flash ONLY
//  - DMA from SPI to DRAM => size/DRAM start/DRAM end must be 8-B aligned


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define VER_CHECK_HEADER        'M','S','V','C','0','0'
#define SERFLASH_LIB_ID         'F','1'                             // F1: libFLASH.a
#define SERFLASH_INTERFACE_VER  '0','1'
#define SERFLASH_BUILD_VER      '0','0','0','1'
#define CHANGE_LIST_NUM         '0','0','0','9','1','9','9','6'
#define PRODUCT_NAME            'A','E'                             // AE: T2
#define CUSTOMER_NAME           '0'                                 //  0: Mstar
#define DEVELOP_STAGE           'B','L','E','G'
#define OS_VERSION              '0','0'
#define CHECK_SUM               'T'

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MSIF_Version _drv_spif_version = {
    .DDI = { SPIF_DRV_VERSION },
};
static SPI_Attribute _SPI_Attri = E_SPI_ATTRI_USER_DRV;
static SERFLASH_Info _SERFLASHInfo;
static SERFLASH_DrvStatus _SERFLASHDrvStatus;

/// Ask 51 to select flash
ms_Mcu_ChipSelect_CB McuChipSelectCB = NULL;
ms_Flash_SetHWWP_CB FlashSetHWWPCB = NULL;

#if (SERFLASH_UTOPIA20)
void* pInstantSerFlash = NULL;
void* pAttributeSerFlash = NULL;
#endif

#if defined(MSOS_TYPE_LINUX)
#define SYS_IOCTL_MAGIC             'S'
#define IOCTL_SYS_SPI_READ             _IOWR (SYS_IOCTL_MAGIC, 0x71, int)
#define IOCTL_SYS_SPI_WRITE            _IOWR (SYS_IOCTL_MAGIC, 0x72, int)
#define IOCTL_SYS_SPI_ERASE            _IOWR (SYS_IOCTL_MAGIC, 0x73, int)
#define IOCTL_SYS_SPI_SIZE_DETECT      _IOWR (SYS_IOCTL_MAGIC, 0x74, int)
#define IOCTL_SYS_SPI_READ_STATUS      _IOWR (SYS_IOCTL_MAGIC, 0x76, int)
#define IOCTL_SYS_SPI_WRITE_STATUS     _IOWR (SYS_IOCTL_MAGIC, 0x77, int)
#define IOCTL_SYS_SPI_INIT             _IO (SYS_IOCTL_MAGIC,0x78)
#define IOCTL_SYS_SPI_WRITE_PROTECT    _IOWR (SYS_IOCTL_MAGIC, 0x79, int)
#define IOCTL_SYS_SPI_ERASE_ALL        _IO (SYS_IOCTL_MAGIC,0x7A)
#define IOCTL_SYS_SPI_FLASH_INFO       _IOWR (SYS_IOCTL_MAGIC, 0x7B, int)
#define IOCTL_SYS_SPI_SECTOR_ERASE     _IOWR (SYS_IOCTL_MAGIC, 0x7C, int)

#define SPI_FW_NAME      "/dev/system"
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#define DRV_FLASH_MS(x)     (5955 * x)
#define FLASH_WAIT_TIME     (DRV_FLASH_MS(100)*0x200)
#define FLASH_IS_TIMEOUT(x) ((x) ? FALSE : TRUE)

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// Get the information of Serial Flash
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
const SERFLASH_Info *MDrv_SERFLASH_GetInfo(void)
{
	if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
	{
		DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO,
                    ULOGD(TAG_SERFLASH,"MDrv_SERFLASH_GetInfo()\n"
                           "\tu32AccessWidth = %d\n"
                           "\tu32TotalSize   = %d\n"
                           "\tu32SecNum      = %d\n"
                           "\tu32SecSize     = %d\n",
                           (int)(_SERFLASHInfo.u32AccessWidth),
                           (int)(_SERFLASHInfo.u32TotalSize),
                           (int)(_SERFLASHInfo.u32SecNum),
                           (int)(_SERFLASHInfo.u32SecSize)
                           )
                    );
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            perror("");

        }
        else
        {
            if(-1 == ioctl(fd, IOCTL_SYS_SPI_FLASH_INFO,&_SERFLASHInfo))
            {
                perror("");
                close(fd);
            }
        }
        close(fd);
#endif
  }

    return &_SERFLASHInfo;
}

//------------------------------------------------------------------------------
/// Description : Show the SERFLASH driver version
/// @param  ppVersion \b OUT: output SERFLASH driver version
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return FALSE;

    *ppVersion = &_drv_spif_version;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Description : Get Serial Flash driver status
/// @param  pDrvStatus \b OUT: poniter to store the returning driver status
/// @return TRUE : succeed
/// @return FALSE : failed to get the driver status
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_GetStatus(SERFLASH_DrvStatus* pDrvStatus)
{
    memcpy(pDrvStatus, &_SERFLASHDrvStatus, sizeof(_SERFLASHDrvStatus));

    return TRUE;
}

#if (SERFLASH_UTOPIA20)
//------------------------------------------------------------------------------
/// Description : Set detailed level of Parallel Flash driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8SERFLASHDbgLevel = u8DbgLevel;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Description : Set WP pin info to driver
/// @param bWPInfo
/// @return TRUE : succeed
/// @return FALSE : failed to set the WP pin info
//------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_SetWPInfo(MS_BOOL bWPInfo)
{
    _bWPPullHigh = bWPInfo;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Description : HK ask 8051 to select flash chip by call back function
/// @param ms_Mcu_ChipSelect_CB    \b IN  call back function
/// @return TRUE : succeed
/// @return NULL :
//------------------------------------------------------------------------------
void _MDrv_SERFLASH_SetMcuCSCallBack(ms_Mcu_ChipSelect_CB ChipSel_cb)
{
    McuChipSelectCB = ChipSel_cb;
}

void _MDrv_SERFLASH_SetFlashWPCallBack(ms_Flash_SetHWWP_CB FlashWP_cb)
{
    FlashSetHWWPCB = FlashWP_cb;
}

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash type by reading the MID and DID
/// @return TRUE : succeed
/// @return FALSE : unknown flash type
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_DetectType(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return HAL_SERFLASH_DetectType();
}

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash Size
/// @param  u32FlashSize    \b OUT: u32 ptr to store flash size
/// @return TRUE : succeed
/// @return FALSE : unknown flash size
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_DetectSize(MS_U32 *u32FlashSize)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return HAL_SERFLASH_DetectSize(u32FlashSize);
}

//-------------------------------------------------------------------------------------------------
/// Description : Enable Flash 2XREAD mode, if support
/// @param  b2XMode    \b IN: ENABLE/DISABLE
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_Set2XRead(MS_BOOL b2XMode)
{
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

	Ret = HAL_SERFLASH_Set2XREAD(b2XMode);
    return Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description : Set ckg_spi which flash supports (please ref. the spec. before using this function)
/// @param  SPI_DrvCKG    \b IN: enumerate the ckg_spi
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not. It is safty to run at 43M (Default).
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_SetCKG(SPI_DrvCKG eCKGspi)
{
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_SetCKG(eCKGspi);

    return Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description : Set clock div such that spi clock = mcu clock /clock_div.
/// @param  SPI_DrvClkDiv    \b IN: enumerate the clock_div
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note
//-------------------------------------------------------------------------------------------------
void _MDrv_SERFLASH_ClkDiv(SPI_DrvClkDiv eClkDivspi)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    HAL_SERFLASH_ClkDiv(eClkDivspi);

}

//-------------------------------------------------------------------------------------------------
/// Description : Set XIU/RIU mode (Default : XIU)
/// @param  bXiuRiu    \b IN: 1 for XIU, 0 for RIU
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   XIU mode is faster than RIU mode. It is stable to run by XIU (Default)
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_SetMode(MS_BOOL bXiuRiu)
{
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_SetMode(bXiuRiu);

    return Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description :  Set active flash among multi-spi flashes
/// @param  u8FlashIndex    \b IN: The Flash index, 0 for external #1 spi flash, 1 for external #2 spi flash
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   For Secure booting = 0, please check hw_strapping or e-fuse (the board needs to jump)
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_ChipSelect(MS_U8 u8FlashIndex)
{
    MS_BOOL Ret = FALSE;
    MS_ASSERT((u8FlashIndex < 4));
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_ChipSelect(u8FlashIndex);

    return Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase all sectors in Serial Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_EraseChip(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_EraseChip();

}


//-------------------------------------------------------------------------------------------------
/// Description : Get flash start block index of a flash address
/// @param  u32FlashAddr    \b IN: flash address
/// @param  pu32BlockIndex    \b IN: poniter to store the returning block index
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex)
{
    MS_ASSERT(u32FlashAddr < _SERFLASHInfo.u32TotalSize);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %p)", __FUNCTION__, (int)u32FlashAddr, pu32BlockIndex));

    return HAL_SERFLASH_AddressToBlock(u32FlashAddr, pu32BlockIndex);
}


//-------------------------------------------------------------------------------------------------
/// Description : Get flash start address of a block index
/// @param  u32BlockIndex    \b IN: block index
/// @param  pu32FlashAddr    \b IN: pointer to store the returning flash address
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr)
{
    MS_ASSERT(u32BlockIndex < _SERFLASHInfo.u32SecNum);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %p)", __FUNCTION__, (int)u32BlockIndex, pu32FlashAddr));

    return HAL_SERFLASH_BlockToAddress(u32BlockIndex, pu32FlashAddr);

}

//-------------------------------------------------------------------------------------------------
/// Description : Get Unique ID
/// @param  None
/// @return the SPI Flash's Unique ID
//-------------------------------------------------------------------------------------------------
MS_U64 _MDrv_SERFLASH_ReadUID(void)
{
    MS_U64 u64Ret = 0;

    u64Ret = HAL_SERFLASH_ReadUID();

    if(u64Ret == 0)
    {
        ULOGE(TAG_SERFLASH,"warning: Don't surport Read UID \n");
    }
    return u64Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors given starting address and size in Serial Flash
/// @param  u32StartAddr    \b IN: start address at block boundry
/// @param  u32EraseSize    \b IN: size to erase
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait)
{
    MS_U32  u32StartBlock = 0;
    MS_U32  u32EndBlock = 0;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)", __FUNCTION__,
            (unsigned int)u32StartAddr, (unsigned int)u32EraseSize, (int)bWait));

    if (   FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr, &u32StartBlock)
        || FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr + u32EraseSize - 1, &u32EndBlock)
        )
    {
        return FALSE;
    }

    return MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
}


//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start block
/// @param  u32EndBlock    \b IN: end block
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait)
{
    MS_ASSERT( u32StartBlock<=u32EndBlock && u32EndBlock<NUMBER_OF_SERFLASH_SECTORS );

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)\n", __FUNCTION__, (unsigned int)u32StartBlock, (unsigned int)u32EndBlock, (int)bWait));

    return HAL_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain 4K sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start address
/// @param  u32EndBlock    \b IN: end address
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_SectorErase(MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{

    MS_BOOL bRet = FALSE;
    MS_U32 u32I = 0;
    #define DRV_SERFLASH_SECTOR_SIZE	0x1000

	for( u32I = u32StartAddr; u32I < u32EndAddr; )
	{
		HAL_SERFLASH_SectorErase(u32I);

		if((u32EndAddr-u32I) <= DRV_SERFLASH_SECTOR_SIZE)
		{
			bRet = HAL_SERFLASH_SectorErase(u32EndAddr);
		}

		u32I+=DRV_SERFLASH_SECTOR_SIZE;
	}
	return bRet;

}

//-------------------------------------------------------------------------------------------------
/// Description : Check write done in Serial Flash
/// @return TRUE : done
/// @return FALSE : not done
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_CheckWriteDone(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_CheckWriteDone();
}


//-------------------------------------------------------------------------------------------------
/// Description : Write data to Serial Flash
/// @param  u32FlashAddr    \b IN: start address (4-B aligned)
/// @param  u32FlashSize    \b IN: size in Bytes (4-B aligned)
/// @param  user_buffer    \b IN: Virtual Buffer Address ptr to flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //MS_U8 *pu8BufAddr = (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Data); // Physical Address to Virtual Address, non-cache.
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %d, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (int)u32FlashSize, user_buffer));

    MS_ASSERT( u32FlashAddr + u32FlashSize <= _SERFLASHInfo.u32TotalSize );

    if( user_buffer == NULL )
    {
		return 0;
	}
    return HAL_SERFLASH_Write(u32FlashAddr, u32FlashSize, user_buffer);

}

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //MS_U8 *pu8BufAddr = (MS_U8*)MS_PA2KSEG0((MS_U32)pu8Data); // Physical Address to Virtual Address, cache.

    MS_ASSERT( u32FlashSize > 0 );
    MS_ASSERT( u32FlashAddr + u32FlashSize <= _SERFLASHInfo.u32TotalSize );

    if( user_buffer == NULL )
    {
    	DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"USER BUFFER is NULL\n"));
		return 0;
	}

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %d, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (int)u32FlashSize, user_buffer));
    return HAL_SERFLASH_Read(u32FlashAddr, u32FlashSize, user_buffer);
}

//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @param  bEnable    \b IN: TRUE/FALSE: enable/disable protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteProtect(MS_BOOL bEnable)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d)\n", __FUNCTION__, (int)bEnable));


    if ( _bIBPM == TRUE )
    {
		return HAL_SPI_GangBlockLock(bEnable);
    }
    else
    {
 		return HAL_SERFLASH_WriteProtect(bEnable);
    }
}


//-------------------------------------------------------------------------------------------------
/// Description : Enables all range of flash write protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteProtect_Enable_All_Range(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    if ( _bIBPM == TRUE )
    {
		return HAL_SPI_GangBlockLock(TRUE);
    }
    else
    {
   		return HAL_SERFLASH_WriteProtect_Area(TRUE, 0 << 2);
    }
}


//-------------------------------------------------------------------------------------------------
/// Description : Disables all range of flash write protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteProtect_Disable_All_Range(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    if ( _bIBPM == TRUE )
    {
	    return HAL_SPI_GangBlockLock(FALSE);
    }
    else
    {
    	return MDrv_SERFLASH_WriteProtect_Disable_Range_Set(0, _SERFLASHInfo.u32TotalSize);
    }
}


//-------------------------------------------------------------------------------------------------
/// Description : Set flash disable lower bound and size
/// @param  u32DisableLowerBound    \b IN: the lower bound to disable write protect
/// @param  u32DisableSize    \b IN: size to disable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteProtect_Disable_Range_Set(MS_U32 u32DisableLowerBound, MS_U32 u32DisableSize)
{
    MS_U32  u32EnableLowerBound;
    MS_U32  u32EnableUpperBound;
    MS_U8   u8BlockProtectBit;

    MS_U32  u32DisableUpperBound;
    MS_U32  u32FlashIndexMax;

    EN_WP_AREA_EXISTED_RTN enWpAreaExistedRtn;


    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()", __FUNCTION__));

    _SERFLASHDrvStatus.bIsBusy = TRUE;

    u32DisableUpperBound = u32DisableLowerBound + u32DisableSize - 1;
    u32FlashIndexMax = _SERFLASHInfo.u32TotalSize - 1;


    if (   u32DisableLowerBound > u32FlashIndexMax
        || u32DisableUpperBound > u32FlashIndexMax
        || u32DisableLowerBound > u32DisableUpperBound
        )
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO, ULOGD(TAG_SERFLASH," = FALSE, u32DisableLowerBound(0x%08X), u32DisableUpperBound(0x%08X), u32FlashIndexMax(0x%08X)\n", (int)u32DisableLowerBound, (int)u32DisableUpperBound, (int)u32FlashIndexMax));

        return FALSE;
    }


    // Step 1. decide u32DisableUpperBound // TODO: review, prefer to unprotect the end of the flash
    if (   u32DisableUpperBound != u32FlashIndexMax
        && u32DisableLowerBound != 0
        )
    {
        u32DisableUpperBound = u32FlashIndexMax;
    }


    // Step 2. decide u32EnableLowerBound & u32EnableUpperBound
    if (   (u32DisableUpperBound > (u32FlashIndexMax - _SERFLASHInfo.u32SecSize))
        && (u32DisableLowerBound == 0)
        )
    {
        // i.e. no protect
        u32EnableLowerBound = 0xFFFFFFFF;
        u32EnableUpperBound = 0xFFFFFFFF;
    }
    else if (u32DisableLowerBound == 0)
    {
        u32EnableUpperBound = u32FlashIndexMax;
        u32EnableLowerBound = u32DisableUpperBound + 1;
    }
    else // i.e. (u32DisableUpperBound == u32FlashIndexMax) because of Step 1
    {
        u32EnableUpperBound = u32DisableLowerBound - 1;
        u32EnableLowerBound = 0;
    }


    // Step 3. get u8BlockProtectBit
    enWpAreaExistedRtn = HAL_SERFLASH_WP_Area_Existed(u32EnableUpperBound, u32EnableLowerBound, &u8BlockProtectBit);

    switch (enWpAreaExistedRtn)
    {
    case WP_AREA_NOT_AVAILABLE:
    case WP_TABLE_NOT_SUPPORT:
        u8BlockProtectBit = 0;
        break;

    default:
        /* DO NOTHING */
        break;
    }

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"\n"));

    return HAL_SERFLASH_WriteProtect_Area(FALSE, u8BlockProtectBit);
}


//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @param  bEnableAllArea    \b IN: enable or disable protection
/// @param  u8BlockProtectBits    \b IN: block protection bits which stand for the area to enable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 u8BlockProtectBits)
{

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d, 0x%02X)\n", __FUNCTION__, (int)bEnableAllArea, u8BlockProtectBits));
    return HAL_SERFLASH_WriteProtect_Area(bEnableAllArea, u8BlockProtectBits);
}


//-------------------------------------------------------------------------------------------------
/// Description : Read ID from Serial Flash
/// @param  pu8FlashID    \b OUT: Virtual data ptr to store the read ID
/// @param  u32IDSize    \b IN: size in Bytes
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_ReadID(MS_U8 *pu8FlashID, MS_U32 u32IDSize)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%p, %d)\n", __FUNCTION__, pu8FlashID, (int)u32IDSize));
    return HAL_SERFLASH_ReadID(pu8FlashID, u32IDSize);
}


//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash to DRAM in DMA mode
/// @param  u32FlashStart    \b IN: src start address in flash (0 ~ flash size-1)
/// @param  u32DRAMStart    \b IN: dst start address in DRAM (16B-aligned) (0 ~ DRAM size-1)
/// @param  u32Size    \b IN: size in Bytes (8B-aligned) (>=8)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_DMA(MS_U32 u32FlashStart, MS_U32 u32DRAMStart, MS_U32 u32Size)
{
    MS_ASSERT( u32FlashStart+u32Size <= _SERFLASHInfo.u32TotalSize);
    MS_ASSERT( u32DRAMStart%8 ==0 );
    MS_ASSERT( u32Size%8 ==0 );
    MS_ASSERT( u32Size>=8 );

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return HAL_SERFLASH_DMA(u32FlashStart, u32DRAMStart, u32Size);
}


//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register in Serial Flash
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_ReadStatusRegister(MS_U8 *pu8StatusReg)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_ReadStatusReg(pu8StatusReg);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register2 in Serial Flash
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register (high-byte)
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_ReadStatusRegister2(MS_U8 *pu8StatusReg)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_ReadStatusReg2(pu8StatusReg);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Write Status Register in Serial Flash
/// @param  u16StatusReg    \b IN: Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_WriteStatusRegister(MS_U16 u16StatusReg)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_WriteStatusReg(u16StatusReg);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Handle for BDMA copy data from ONLY Flash src to other dst
/// @param u32FlashAddr \b IN: Physical Source address in spi flash
/// @param u32DramAddr \b IN: Physical Dst address
/// @param u32Len \b IN: data length
/// @param eDstDev \b IN: The Dst Device of Flash BDMA
/// @param u8OpCfg \b IN: u8OpCfg: default is SPIDMA_OPCFG_DEF
/// - Bit0: inverse mode --> SPIDMA_OPCFG_INV_COPY
/// - Bit2: Copy & CRC check in wait mode --> SPIDMA_OPCFG_CRC_COPY
/// - Bit3: Copy without waiting --> SPIDMA_OPCFG_NOWAIT_COPY
/// @return \b MS_BOOL
/// [NONOS_SUPPORT]
/// [fw : drvBDMA ]
//-------------------------------------------------------------------------------------------------
MS_BOOL _MDrv_SERFLASH_CopyHnd(MS_PHYADDR u32FlashAddr,
                                          MS_PHYADDR u32DstAddr,
                                          MS_U32 u32Len,
                                          SPIDMA_Dev eDstDev,
                                          MS_U8 u8OpCfg)
{

    #define BDMA_DEV_FLASH  5
    MS_U16 CpyType = ((BDMA_DEV_FLASH & 0x0F) | _LShift((eDstDev &0x0F), 8));
    MS_U32 u32Delay = FLASH_WAIT_TIME;

    while (!HAL_SERFLASH_CheckWriteDone())
    {
        if (FLASH_IS_TIMEOUT(u32Delay))
        {
            ULOGE(TAG_SERFLASH,"%s() : DMA flash is busy!\n",__FUNCTION__);
            return FALSE;
        }
        u32Delay--;
    }

    return MDrv_BDMA_CopyHnd(u32FlashAddr, u32DstAddr, u32Len, (BDMA_CpyType) CpyType, u8OpCfg);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Switch SPI as GPIO Input
/// @param  bSwitch    \b IN: 1 for GPIO, 0 for NORMAL
/// @note   Not allowed in interrupt context
/// @note   For project's power consumption
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
void _MDrv_SERFLASH_SetGPIO(MS_BOOL bSwitch)
{
	HAL_SERFLASH_SetGPIO(bSwitch);
}
#endif

//------------------------------------------------------------------------------
/// Description : Set detailed level of Parallel Flash driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetDbgLevel(MS_U8 u8DbgLevel)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETDBGLEVEL pSetDbgLevelParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pSetDbgLevelParam = (PSERFLASH_SETDBGLEVEL)malloc(sizeof(SERFLASH_SETDBGLEVEL));
        if (NULL == pSetDbgLevelParam)
            return FALSE;
        pSetDbgLevelParam->u8DbgLevel = u8DbgLevel;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetDbgLevel,(void*)pSetDbgLevelParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetDbgLevel fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    _u8SERFLASHDbgLevel = u8DbgLevel;

    return TRUE;
#endif
}

//------------------------------------------------------------------------------
/// Description : Set WP pin info to driver
/// @param bWPInfo
/// @return TRUE : succeed
/// @return FALSE : failed to set the WP pin info
//------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetWPInfo(MS_BOOL bWPInfo)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETWPINFO pSetWPInfoParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pSetWPInfoParam = (PSERFLASH_SETWPINFO)malloc(sizeof(SERFLASH_SETWPINFO));
        if (NULL == pSetWPInfoParam)
            return FALSE;
        pSetWPInfoParam->bWPInfo = bWPInfo;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetWPInfo,(void*)pSetWPInfoParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetWPInfo fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    _bWPPullHigh = bWPInfo;

    return TRUE;
#endif
}

//------------------------------------------------------------------------------
/// Description : HK ask 8051 to select flash chip by call back function
/// @param ms_Mcu_ChipSelect_CB    \b IN  call back function
/// @return TRUE : succeed
/// @return NULL :
//------------------------------------------------------------------------------
void MDrv_SERFLASH_SetMcuCSCallBack(ms_Mcu_ChipSelect_CB ChipSel_cb)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETMCUCSCB pSetMcuCSCBParam = NULL;
        if (NULL == pInstantSerFlash)
            return;
        pSetMcuCSCBParam = (PSERFLASH_SETMCUCSCB)malloc(sizeof(SERFLASH_SETMCUCSCB));
        if (NULL == pSetMcuCSCBParam)
            return;
        pSetMcuCSCBParam->ChipSel_cb = ChipSel_cb;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetMcuCSCallBack,(void*)pSetMcuCSCBParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetMcuCSCallBack fail\n");
            return;
        }
        return;
    }
#else

    McuChipSelectCB = ChipSel_cb;
#endif
}

void MDrv_SERFLASH_SetFlashWPCallBack(ms_Flash_SetHWWP_CB FlashWP_cb)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETFLASHWPCB pSetFlashWPCBParam = NULL;
        if (NULL == pInstantSerFlash)
            return;
        pSetFlashWPCBParam = (PSERFLASH_SETFLASHWPCB)malloc(sizeof(SERFLASH_SETFLASHWPCB));
        if (NULL == pSetFlashWPCBParam)
            return;
        pSetFlashWPCBParam->FlashWP_cb = FlashWP_cb;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetFlashWPCallBack,(void*)pSetFlashWPCBParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetFlashWPCallBack fail\n");
            return;
        }
        return;
    }
#else

    FlashSetHWWPCB = FlashWP_cb;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash type by reading the MID and DID
/// @return TRUE : succeed
/// @return FALSE : unknown flash type
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DetectType(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_DetectType,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_DetectType fail\n");
        return FALSE;
    }
    return TRUE;
#else
    return HAL_SERFLASH_DetectType();
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Detect flash Size
/// @param  u32FlashSize    \b OUT: u32 ptr to store flash size
/// @return TRUE : succeed
/// @return FALSE : unknown flash size
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DetectSize(MS_U32 *u32FlashSize)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    {
   	    PSERFLASH_DETECTSIZE pDetectSizeParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pDetectSizeParam = (PSERFLASH_DETECTSIZE)malloc(sizeof(SERFLASH_DETECTSIZE));
        if (NULL == pDetectSizeParam)
            return FALSE;
        pDetectSizeParam->u32FlashSize = u32FlashSize;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_DetectSize,(void*)pDetectSizeParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_DetectSize fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    return HAL_SERFLASH_DetectSize(u32FlashSize);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Enable Flash 2XREAD mode, if support
/// @param  b2XMode    \b IN: ENABLE/DISABLE
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Set2XRead(MS_BOOL b2XMode)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SET2XREAD p2xModeParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        p2xModeParam = (PSERFLASH_SET2XREAD)malloc(sizeof(SERFLASH_SET2XREAD));
        if (NULL == p2xModeParam)
            return FALSE;
        p2xModeParam->b2XMode = b2XMode;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_Set2XRead,(void*)p2xModeParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_Set2XRead fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

	Ret = HAL_SERFLASH_Set2XREAD(b2XMode);
    return Ret;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Set ckg_spi which flash supports (please ref. the spec. before using this function)
/// @param  SPI_DrvCKG    \b IN: enumerate the ckg_spi
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   Please ref. sprc. to confirm Flash support or not. It is safty to run at 43M (Default).
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetCKG(SPI_DrvCKG eCKGspi)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETCKG pSETCKGParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pSETCKGParam = (PSERFLASH_SETCKG)malloc(sizeof(SERFLASH_SETCKG));
        if (NULL == pSETCKGParam)
            return FALSE;
        pSETCKGParam->eCKGspi = eCKGspi;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetCKG,(void*)pSETCKGParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetCKG fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_SetCKG(eCKGspi);
    return Ret;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Set clock div such that spi clock = mcu clock /clock_div.
/// @param  SPI_DrvClkDiv    \b IN: enumerate the clock_div
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_ClkDiv(SPI_DrvClkDiv eClkDivspi)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_CLKDIV pCLKDIVParam = NULL;
        if (NULL == pInstantSerFlash)
            return;
        pCLKDIVParam = (PSERFLASH_CLKDIV)malloc(sizeof(SERFLASH_CLKDIV));
        if (NULL == pCLKDIVParam)
            return;
        pCLKDIVParam->eClkDivspi = eClkDivspi;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ClkDiv,(void*)pCLKDIVParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_ClkDiv fail\n");
            return;
        }
        return;
    }
#else
    HAL_SERFLASH_ClkDiv(eClkDivspi);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Set XIU/RIU mode (Default : XIU)
/// @param  bXiuRiu    \b IN: 1 for XIU, 0 for RIU
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   XIU mode is faster than RIU mode. It is stable to run by XIU (Default)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SetMode(MS_BOOL bXiuRiu)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETMODE pSETMODEParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pSETMODEParam = (PSERFLASH_SETMODE)malloc(sizeof(SERFLASH_SETMODE));
        if (NULL == pSETMODEParam)
            return FALSE;
        pSETMODEParam->bXiuRiu = bXiuRiu;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetMode,(void*)pSETMODEParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGD(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SetMode fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_SetMode(bXiuRiu);
    return Ret;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description :  Set active flash among multi-spi flashes
/// @param  u8FlashIndex    \b IN: The Flash index, 0 for external #1 spi flash, 1 for external #2 spi flash
/// @return TRUE : succeed
/// @return FALSE : not succeed
/// @note   For Secure booting = 0, please check hw_strapping or e-fuse (the board needs to jump)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ChipSelect(MS_U8 u8FlashIndex)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_CHIPSELECT pCHIPSELECTParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pCHIPSELECTParam = (PSERFLASH_CHIPSELECT)malloc(sizeof(SERFLASH_CHIPSELECT));
        if (NULL == pCHIPSELECTParam)
            return FALSE;
        pCHIPSELECTParam->u8FlashIndex = u8FlashIndex;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ChipSelect,(void*)pCHIPSELECTParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_ChipSelect fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_BOOL Ret = FALSE;
    MS_ASSERT((u8FlashIndex < 4));
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    Ret = HAL_SERFLASH_ChipSelect(u8FlashIndex);
    return Ret;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Initialize Serial Flash
/// @return None
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_Init(void)
{
    _u8SERFLASHDbgLevel = E_SERFLASH_DBGLV_INFO; // init debug level first         //SERFLASH_DBGLV_DEBUG

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    //
    //  1. HAL init
    //
   if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
    MS_VIRT VirtPMBank;
    MS_PHY  u32PMBankSize;
    MS_VIRT VirtNonPMBank;
    MS_PHY  u32NonPMBankSize;
    MS_VIRT VirtFlashBank0;
    MS_PHY  u32FlashBank0Size;

    if (!MDrv_MMIO_GetBASE( &VirtPMBank, &u32PMBankSize, MS_MODULE_ISP))
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, ULOGE(TAG_SERFLASH,"IOMap failure to get DRV_MMIO_PM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtNonPMBank, &u32NonPMBankSize, MS_MODULE_MHEG5))
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, ULOGE(TAG_SERFLASH,"IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtFlashBank0, &u32FlashBank0Size, MS_MODULE_FLASH))
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, ULOGE(TAG_SERFLASH,"IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
    }
#if (SERFLASH_UTOPIA20)
    if(UtopiaOpen(MODULE_FLASH, &pInstantSerFlash, 0, pAttributeSerFlash) !=  UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Open SERFLASH fail\n");
        return;
    }
#endif

    HAL_SERFLASH_Config(VirtPMBank, VirtNonPMBank, VirtFlashBank0);

    HAL_SERFLASH_Init();

    HAL_SERFLASH_DetectType();

    //
    //  2. init SERFLASH_Info
    //
     _SERFLASHInfo.u32AccessWidth = 1;
     _SERFLASHInfo.u32SecNum      = NUMBER_OF_SERFLASH_SECTORS;
     _SERFLASHInfo.u32SecSize     = SERFLASH_SECTOR_SIZE;

    if (_hal_SERFLASH.pSpecialBlocks == NULL)
    {
     _SERFLASHInfo.u32TotalSize   = (NUMBER_OF_SERFLASH_SECTORS * SERFLASH_SECTOR_SIZE);
    }
    else
    {
      HAL_SERFLASH_DetectSize(&_SERFLASHInfo.u32TotalSize);
    }

    //
    //  3. init other data structure of Serial Flash driver
    //
    _SERFLASHDrvStatus.bIsBusy = FALSE;
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            perror("");
            return;
        }
        else
        {
            if(-1 == ioctl(fd, IOCTL_SYS_SPI_INIT))
            {
                printf("%s ioctl fail\n",__FUNCTION__);

            }
        }
        close(fd);
#endif
  }
}


//-------------------------------------------------------------------------------------------------
/// Description : Erase all sectors in Serial Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_EraseChip(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
  if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
#if (SERFLASH_UTOPIA20)
    if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_EraseChip,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_ChipSelect fail\n");
        return FALSE;
    }
    return TRUE;
#else
    return HAL_SERFLASH_EraseChip();
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));
        if(fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return FALSE;
        }
        else
        {
            if(-1 == ioctl(fd, IOCTL_SYS_SPI_ERASE_ALL))
            {
                close(fd);
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                return FALSE;
            }
        }
        close(fd);
#endif
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Description : Get flash start block index of a flash address
/// @param  u32FlashAddr    \b IN: flash address
/// @param  pu32BlockIndex    \b IN: poniter to store the returning block index
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex)
{
    MS_ASSERT(u32FlashAddr < _SERFLASHInfo.u32TotalSize);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %p)", __FUNCTION__, (int)u32FlashAddr, pu32BlockIndex));
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_ADDRTOBLK pADDRTOBLKParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pADDRTOBLKParam = (PSERFLASH_ADDRTOBLK)malloc(sizeof(SERFLASH_ADDRTOBLK));
        if (NULL == pADDRTOBLKParam)
            return FALSE;
        pADDRTOBLKParam->u32FlashAddr = u32FlashAddr;
        pADDRTOBLKParam->pu32BlockIndex = pu32BlockIndex;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_AddressToBlock,(void*)pADDRTOBLKParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_AddressToBlock fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    return HAL_SERFLASH_AddressToBlock(u32FlashAddr, pu32BlockIndex);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Get flash start address of a block index
/// @param  u32BlockIndex    \b IN: block index
/// @param  pu32FlashAddr    \b IN: pointer to store the returning flash address
/// @return TRUE : succeed
/// @return FALSE : illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr)
{
    MS_ASSERT(u32BlockIndex < _SERFLASHInfo.u32SecNum);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %p)", __FUNCTION__, (int)u32BlockIndex, pu32FlashAddr));
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_BLKTOADDR pBLKTOADDRParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pBLKTOADDRParam = (PSERFLASH_BLKTOADDR)malloc(sizeof(SERFLASH_BLKTOADDR));
        if (NULL == pBLKTOADDRParam)
            return FALSE;
        pBLKTOADDRParam->u32BlockIndex = u32BlockIndex;
        pBLKTOADDRParam->pu32FlashAddr = pu32FlashAddr;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_BlockToAddress,(void*)pBLKTOADDRParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_BlockToAddress fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    return HAL_SERFLASH_BlockToAddress(u32BlockIndex, pu32FlashAddr);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Get Unique ID
/// @param  None
/// @return the SPI Flash's Unique ID
//-------------------------------------------------------------------------------------------------
MS_U64 MDrv_SERFLASH_ReadUID(void)
{
    MS_U64 u64Ret = 0;
#if (SERFLASH_UTOPIA20)
    if (NULL == pInstantSerFlash)
        return FALSE;
    u64Ret = UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ReadUID,(void*)NULL);
#else
    u64Ret = HAL_SERFLASH_ReadUID();
#endif
    if(u64Ret == 0)
    {
        ULOGE(TAG_SERFLASH,"warning: Don't surport Read UID \n");
    }
    return u64Ret;
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors given starting address and size in Serial Flash
/// @param  u32StartAddr    \b IN: start address at block boundry
/// @param  u32EraseSize    \b IN: size to erase
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait)
{
  if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_ADDRERASE pADDRERASEParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pADDRERASEParam = (PSERFLASH_ADDRERASE)malloc(sizeof(SERFLASH_ADDRERASE));
        if (NULL == pADDRERASEParam)
            return FALSE;

        pADDRERASEParam->u32StartAddr = u32StartAddr;
        pADDRERASEParam->u32EraseSize = u32EraseSize;
		pADDRERASEParam->bWait = bWait;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_AddressErase,(void*)pADDRERASEParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_AddressErase fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_U32  u32StartBlock = 0;
    MS_U32  u32EndBlock = 0;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)", __FUNCTION__,
            (unsigned int)u32StartAddr, (unsigned int)u32EraseSize, (int)bWait));

    if (   FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr, &u32StartBlock)
        || FALSE == MDrv_SERFLASH_AddressToBlock(u32StartAddr + u32EraseSize - 1, &u32EndBlock)
        )
    {
        return FALSE;
    }

    return MDrv_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int ret;
        int fd = -1;
        SERFLASH_EraseInfo spi;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08x, 0x%08x, %d)", __FUNCTION__,
                        (unsigned int)u32StartAddr, (unsigned int)u32EraseSize, (int)bWait));
        if(fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return FALSE;
        }
        else
        {
            spi.u32StartAddr = u32StartAddr;
            spi.u32EraseSize = u32EraseSize;
            spi.bWait = bWait;
            ret = ioctl(fd, IOCTL_SYS_SPI_ERASE, &spi);
            if(ret == -1)
            {
                close(fd);
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                return FALSE;
            }
        }
        close(fd);
#endif
  }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Description : Erase certain sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start block
/// @param  u32EndBlock    \b IN: end block
/// @param  bWait    \b IN: wait write done or not
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait)
{
    MS_ASSERT( u32StartBlock<=u32EndBlock && u32EndBlock<NUMBER_OF_SERFLASH_SECTORS );

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)\n", __FUNCTION__, (unsigned int)u32StartBlock, (unsigned int)u32EndBlock, (int)bWait));
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_BLKERASE pBLKERASEParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pBLKERASEParam = (PSERFLASH_BLKERASE)malloc(sizeof(SERFLASH_BLKERASE));
        if (NULL == pBLKERASEParam)
            return FALSE;
	
        pBLKERASEParam->u32StartBlock = u32StartBlock;
        pBLKERASEParam->u32EndBlock = u32EndBlock;
        pBLKERASEParam->bWait = bWait;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_BlockErase,(void*)pBLKERASEParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_BlockErase fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    return HAL_SERFLASH_BlockErase(u32StartBlock, u32EndBlock, bWait);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Erase certain 4K sectors in Serial Flash
/// @param  u32StartBlock    \b IN: start address
/// @param  u32EndBlock    \b IN: end address
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SectorErase(MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{
   if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
   {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SECERASE pSECERASEParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pSECERASEParam = (PSERFLASH_SECERASE)malloc(sizeof(SERFLASH_SECERASE));
        if (NULL == pSECERASEParam)
            return FALSE;

        pSECERASEParam->u32StartAddr = u32StartAddr;
        pSECERASEParam->u32EndAddr = u32EndAddr;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SectorErase,(void*)pSECERASEParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_SectorErase fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else
    MS_BOOL bRet = FALSE;
    MS_U32 u32I = 0;
    #define DRV_SERFLASH_SECTOR_SIZE	0x1000

	for( u32I = u32StartAddr; u32I < u32EndAddr; )
	{
		HAL_SERFLASH_SectorErase(u32I);

		if((u32EndAddr-u32I) <= DRV_SERFLASH_SECTOR_SIZE)
		{
			bRet = HAL_SERFLASH_SectorErase(u32EndAddr);
		}

		u32I+=DRV_SERFLASH_SECTOR_SIZE;
	}
	return bRet;
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)

        int fd = -1;
        int ret = FALSE;
        SERFLASH_SectorEraseInfo spi;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);
        if(fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return ret;
        }
        else
        {
            spi.u32StartAddr = u32StartAddr;
            spi.u32Endaddr = u32EndAddr;
            ret = ioctl(fd, IOCTL_SYS_SPI_SECTOR_ERASE, &spi);
            if(ret == -1)
            {
                close(fd);
                return ret;
            }
        }
        close(fd);
#endif
 }
    return TRUE;

}

//-------------------------------------------------------------------------------------------------
/// Description : Check write done in Serial Flash
/// @return TRUE : done
/// @return FALSE : not done
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_CheckWriteDone(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    if (NULL == pInstantSerFlash)
        return FALSE;

    if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_CheckWriteDone,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_CheckWriteDone fail\n");
        return FALSE;
    }
    return TRUE;
#else
    return HAL_SERFLASH_CheckWriteDone();
#endif
}

//-------------------------------------------------------------------------------------------------
/// Description : Write data to Serial Flash
/// @param  u32FlashAddr    \b IN: start address (4-B aligned)
/// @param  u32FlashSize    \b IN: size in Bytes (4-B aligned)
/// @param  user_buffer    \b IN: Virtual Buffer Address ptr to flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //MS_U8 *pu8BufAddr = (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Data); // Physical Address to Virtual Address, non-cache.
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %d, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (int)u32FlashSize, user_buffer));

    MS_ASSERT( u32FlashAddr + u32FlashSize <= _SERFLASHInfo.u32TotalSize );
   if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
   {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_WRITE pWRITEParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pWRITEParam = (PSERFLASH_WRITE)malloc(sizeof(SERFLASH_WRITE));
        if (NULL == pWRITEParam)
            return FALSE;

        pWRITEParam->u32FlashAddr = u32FlashAddr;
        pWRITEParam->u32FlashSize = u32FlashSize;
        pWRITEParam->user_buffer  = user_buffer;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_Write,(void*)pWRITEParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_Write fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    if( user_buffer == NULL )
    {
        return 0;
    }
    return HAL_SERFLASH_Write(u32FlashAddr, u32FlashSize, user_buffer);
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        SERFLASH_AccessInfo spi;
        int ret;

        fd = open(SPI_FW_NAME, O_RDWR);

        //MS_U8 *pu8BufAddr = (MS_U8*)MS_PA2KSEG1((MS_U32)pu8Data); // Physical Address to Virtual Address, non-cache.
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, %d, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (int)u32FlashSize, user_buffer));
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("MDrv_SERFLASH_Write Cannot open fw descriptor\n"));
            return FALSE;
        }
        else
        {
            spi.u32Start = u32FlashAddr;
            spi.u32Len = u32FlashSize;
            spi.u8data = user_buffer;
            ret = ioctl(fd, IOCTL_SYS_SPI_WRITE, &spi);
            if(ret == -1)
            {
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                close(fd);
                return FALSE;
            }
        }
        close(fd);
#endif
  }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //MS_U8 *pu8BufAddr = (MS_U8*)MS_PA2KSEG0((MS_U32)pu8Data); // Physical Address to Virtual Address, cache.
  if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
  	MS_ASSERT( u32FlashSize > 0 );
    MS_ASSERT( u32FlashAddr + u32FlashSize <= _SERFLASHInfo.u32TotalSize );

    if( user_buffer == NULL )
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"USER BUFFER is NULL\n"));
        return 0;
    }
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_READ pREADParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pREADParam = (PSERFLASH_READ)malloc(sizeof(SERFLASH_READ));
        if (NULL == pREADParam)
            return FALSE;

        pREADParam->u32FlashAddr = u32FlashAddr;
        pREADParam->u32FlashSize = u32FlashSize;
        pREADParam->user_buffer  = user_buffer;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_Read,(void*)pREADParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_Write fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08X, %d, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (int)u32FlashSize, user_buffer));
    return HAL_SERFLASH_Read(u32FlashAddr, u32FlashSize, user_buffer);
#endif
  }
 else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
 {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        int ret;
        SERFLASH_AccessInfo spi;
        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);
        if(fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("MDrv_SERFLASH_Read Cannot open fw descriptor\n"));
            return FALSE;
        }
        else
        {
            spi.u32Start = u32FlashAddr;
            spi.u32Len = u32FlashSize;
            spi.u8data = user_buffer;
            ret = ioctl(fd, IOCTL_SYS_SPI_READ, &spi);
            if(ret == -1)
            {
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                close(fd);
                return FALSE;
            }
        }
        close(fd);
#endif
 }
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @param  bEnable    \b IN: TRUE/FALSE: enable/disable protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect(MS_BOOL bEnable)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d)\n", __FUNCTION__, (int)bEnable));
  if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_WP pWPParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pWPParam = (PSERFLASH_WP)malloc(sizeof(SERFLASH_WP));
        if (NULL == pWPParam)
            return FALSE;

        pWPParam->bEnable = bEnable;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteProtect,(void*)pWPParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    if ( _bIBPM == TRUE )
    {
		return HAL_SPI_GangBlockLock(bEnable);
    }
    else
    {
 		return HAL_SERFLASH_WriteProtect(bEnable);
    }
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)

        int fd = -1;
        int ret;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, (int)bEnable));
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return FALSE;
        }
        else
        {
            ret = ioctl(fd, IOCTL_SYS_SPI_WRITE_PROTECT, &bEnable);
            if(ret == -1)
            {
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                close(fd);
                return FALSE;
            }
        }
        close(fd);
#endif
  }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Description : Enables all range of flash write protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Enable_All_Range(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    if (NULL == pInstantSerFlash)
        return FALSE;

    if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteProtect_Enable_All_Range,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect_Enable_All_Range fail\n");
        return FALSE;
    }
        return TRUE;
#else

    if ( _bIBPM == TRUE )
    {
		return HAL_SPI_GangBlockLock(TRUE);
    }
    else
    {
   		return HAL_SERFLASH_WriteProtect_Area(TRUE, 0 << 2);
    }
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Disables all range of flash write protection
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Disable_All_Range(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
#if (SERFLASH_UTOPIA20)
    if (NULL == pInstantSerFlash)
        return FALSE;
	
    if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteProtect_Disable_All_Range,(void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect_Disable_All_Range fail\n");
        return FALSE;
    }
    return TRUE;
#else

    if ( _bIBPM == TRUE )
    {
	    return HAL_SPI_GangBlockLock(FALSE);
    }
    else
    {
    	return MDrv_SERFLASH_WriteProtect_Disable_Range_Set(0, _SERFLASHInfo.u32TotalSize);
    }
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Set flash disable lower bound and size
/// @param  u32DisableLowerBound    \b IN: the lower bound to disable write protect
/// @param  u32DisableSize    \b IN: size to disable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Disable_Range_Set(MS_U32 u32DisableLowerBound, MS_U32 u32DisableSize)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_WPDISABLERANGESET pWPDisableRangeSetParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pWPDisableRangeSetParam = (PSERFLASH_WPDISABLERANGESET)malloc(sizeof(SERFLASH_WPDISABLERANGESET));
        if (NULL == pWPDisableRangeSetParam)
            return FALSE;

        pWPDisableRangeSetParam->u32DisableLowerBound = u32DisableLowerBound;
        pWPDisableRangeSetParam->u32DisableSize = u32DisableSize;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteProtect_Disable_Range_Set,(void*)pWPDisableRangeSetParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect_Disable_Range_Set fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    MS_U32  u32EnableLowerBound;
    MS_U32  u32EnableUpperBound;
    MS_U8   u8BlockProtectBit;

    MS_U32  u32DisableUpperBound;
    MS_U32  u32FlashIndexMax;

    EN_WP_AREA_EXISTED_RTN enWpAreaExistedRtn;


    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()", __FUNCTION__));

    _SERFLASHDrvStatus.bIsBusy = TRUE;

    u32DisableUpperBound = u32DisableLowerBound + u32DisableSize - 1;
    u32FlashIndexMax = _SERFLASHInfo.u32TotalSize - 1;


    if (   u32DisableLowerBound > u32FlashIndexMax
        || u32DisableUpperBound > u32FlashIndexMax
        || u32DisableLowerBound > u32DisableUpperBound
        )
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO, ULOGI(TAG_SERFLASH," = FALSE, u32DisableLowerBound(0x%08X), u32DisableUpperBound(0x%08X), u32FlashIndexMax(0x%08X)\n", (int)u32DisableLowerBound, (int)u32DisableUpperBound, (int)u32FlashIndexMax));

        return FALSE;
    }


    // Step 1. decide u32DisableUpperBound // TODO: review, prefer to unprotect the end of the flash
    if (   u32DisableUpperBound != u32FlashIndexMax
        && u32DisableLowerBound != 0
        )
    {
        u32DisableUpperBound = u32FlashIndexMax;
    }


    // Step 2. decide u32EnableLowerBound & u32EnableUpperBound
    if (   (u32DisableUpperBound > (u32FlashIndexMax - _SERFLASHInfo.u32SecSize))
        && (u32DisableLowerBound == 0)
        )
    {
        // i.e. no protect
        u32EnableLowerBound = 0xFFFFFFFF;
        u32EnableUpperBound = 0xFFFFFFFF;
    }
    else if (u32DisableLowerBound == 0)
    {
        u32EnableUpperBound = u32FlashIndexMax;
        u32EnableLowerBound = u32DisableUpperBound + 1;
    }
    else // i.e. (u32DisableUpperBound == u32FlashIndexMax) because of Step 1
    {
        u32EnableUpperBound = u32DisableLowerBound - 1;
        u32EnableLowerBound = 0;
    }


    // Step 3. get u8BlockProtectBit
    enWpAreaExistedRtn = HAL_SERFLASH_WP_Area_Existed(u32EnableUpperBound, u32EnableLowerBound, &u8BlockProtectBit);

    switch (enWpAreaExistedRtn)
    {
    case WP_AREA_NOT_AVAILABLE:
    case WP_TABLE_NOT_SUPPORT:
        u8BlockProtectBit = 0;
        break;

    default:
        /* DO NOTHING */
        break;
    }

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"\n"));

    return HAL_SERFLASH_WriteProtect_Area(FALSE, u8BlockProtectBit);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Protect blocks in Serial Flash
/// @param  bEnableAllArea    \b IN: enable or disable protection
/// @param  u8BlockProtectBits    \b IN: block protection bits which stand for the area to enable write protect
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 u8BlockProtectBits)
{
#if (SERFLASH_UTOPIA20)
		{
			PSERFLASH_WPAREA pWPAreaParam = NULL;
			if (NULL == pInstantSerFlash)
				return FALSE;
			pWPAreaParam = (PSERFLASH_WPAREA)malloc(sizeof(SERFLASH_WPAREA));
			if (NULL == pWPAreaParam)
				return FALSE;
	
			pWPAreaParam->bEnableAllArea = bEnableAllArea;
			pWPAreaParam->u8BlockProtectBits = u8BlockProtectBits;
			if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteProtect_Area,(void*)pWPAreaParam) != UTOPIA_STATUS_SUCCESS)
			{
				ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect_Area fail\n");
				return FALSE;
			}
			return TRUE;
		}
#else
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d, 0x%02X)\n", __FUNCTION__, (int)bEnableAllArea, u8BlockProtectBits));
    return HAL_SERFLASH_WriteProtect_Area(bEnableAllArea, u8BlockProtectBits);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Read ID from Serial Flash
/// @param  pu8FlashID    \b OUT: Virtual data ptr to store the read ID
/// @param  u32IDSize    \b IN: size in Bytes
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadID(MS_U8 *pu8FlashID, MS_U32 u32IDSize)
{
#if (SERFLASH_UTOPIA20)
    {

        PSERFLASH_READID pReadIDParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pReadIDParam = (PSERFLASH_READID)malloc(sizeof(SERFLASH_READID));
        if (NULL == pReadIDParam)
            return FALSE;

        pReadIDParam->pu8FlashID = pu8FlashID;
        pReadIDParam->u32IDSize = u32IDSize;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ReadID,(void*)pReadIDParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%p, %d)\n", __FUNCTION__, pu8FlashID, (int)u32IDSize));
    return HAL_SERFLASH_ReadID(pu8FlashID, u32IDSize);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Description : Read data from Serial Flash to DRAM in DMA mode
/// @param  u32FlashStart    \b IN: src start address in flash (0 ~ flash size-1)
/// @param  u32DRAMStart    \b IN: dst start address in DRAM (16B-aligned) (0 ~ DRAM size-1)
/// @param  u32Size    \b IN: size in Bytes (8B-aligned) (>=8)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_DMA(MS_U32 u32FlashStart, MS_U32 u32DRAMStart, MS_U32 u32Size)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_DMA pDMAParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
					pDMAParam = (PSERFLASH_DMA)malloc(sizeof(SERFLASH_DMA));
					if (NULL == pDMAParam)
						return FALSE;
			
					pDMAParam->u32FlashStart = u32FlashStart;
					pDMAParam->u32DRAMStart = u32DRAMStart;
                    pDMAParam->u32Size = u32Size;
					if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_DMA,(void*)pDMAParam) != UTOPIA_STATUS_SUCCESS)
					{
						ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_DMA fail\n");
						return FALSE;
					}
					return TRUE;
				}
#else

    MS_ASSERT( u32FlashStart+u32Size <= _SERFLASHInfo.u32TotalSize);
    MS_ASSERT( u32DRAMStart%8 ==0 );
    MS_ASSERT( u32Size%8 ==0 );
    MS_ASSERT( u32Size>=8 );

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return HAL_SERFLASH_DMA(u32FlashStart, u32DRAMStart, u32Size);
#endif
}


//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register in Serial Flash
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadStatusRegister(MS_U8 *pu8StatusReg)
{
  if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
  {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_READSTATUSREG pReadStatusRegParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pReadStatusRegParam = (PSERFLASH_READSTATUSREG)malloc(sizeof(SERFLASH_READSTATUSREG));
        if (NULL == pReadStatusRegParam)
            return FALSE;
				
        pReadStatusRegParam->pu8StatusReg = pu8StatusReg;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ReadStatusRegister,(void*)pReadStatusRegParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_ReadStatusRegister fail\n");
	        return FALSE;
        }
		return TRUE;
    }
#else

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_ReadStatusReg(pu8StatusReg);
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        int ret;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s\n", __FUNCTION__));
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return FALSE;
        }
        else
        {
            ret = ioctl(fd, IOCTL_SYS_SPI_READ_STATUS, pu8StatusReg);
            if(ret == -1)
            {
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                close(fd);
                return FALSE;
            }
        }
        close(fd);
#endif
  }
    return TRUE;
}

//------- ------------------------------------------------------------------------------------------
/// Description : Read Status Register2 in Serial Flash
/// @param  pu8StatusReg    \b OUT: ptr to Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register (high-byte)
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_ReadStatusRegister2(MS_U8 *pu8StatusReg)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_READSTATUSREG2 pReadStatusReg2Param = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pReadStatusReg2Param = (PSERFLASH_READSTATUSREG2)malloc(sizeof(SERFLASH_READSTATUSREG2));
        if (NULL == pReadStatusReg2Param)
            return FALSE;

        pReadStatusReg2Param->pu8StatusReg = pu8StatusReg;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_ReadStatusRegister2,(void*)pReadStatusReg2Param) != UTOPIA_STATUS_SUCCESS)
		{
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_ReadStatusRegister2 fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_ReadStatusReg2(pu8StatusReg);
#endif
}

//------- ------------------------------------------------------------------------------------------
/// Description : Write Status Register in Serial Flash
/// @param  u16StatusReg    \b IN: Status Register value
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
/// @note   For Specific Flash IC with 16-bit status register
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_WriteStatusRegister(MS_U16 u16StatusReg)
{
   if(_SPI_Attri == E_SPI_ATTRI_USER_DRV)
   {
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_WRITESTATUSREG pWriteStatusRegParam = NULL;
        if (NULL == pInstantSerFlash)
            return FALSE;
        pWriteStatusRegParam = (PSERFLASH_WRITESTATUSREG)malloc(sizeof(SERFLASH_WRITESTATUSREG));
        if (NULL == pWriteStatusRegParam)
            return FALSE;

        pWriteStatusRegParam->u16StatusReg = u16StatusReg;

        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_WriteStatusRegister,(void*)pWriteStatusRegParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteStatusRegister fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));

    return HAL_SERFLASH_WriteStatusReg(u16StatusReg);
#endif
  }
  else if(_SPI_Attri == E_SPI_ATTRI_KERNEL_DRV)
  {
#if defined(MSOS_TYPE_LINUX)
        int fd = -1;
        int ret;

        fd = open(SPI_FW_NAME, O_RDWR|O_EXCL);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, (int)u16StatusReg));
        if (fd == -1)
        {
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("Cannot open fw descriptor"));
            return FALSE;
        }
        else
        {
            ret = ioctl(fd, IOCTL_SYS_SPI_WRITE_STATUS, &u16StatusReg);
            if(ret == -1)
            {
                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_WARNING,printf("%s ioctl fail\n",__FUNCTION__));
                close(fd);
                return FALSE;
            }
        }
        close(fd);
#endif
 }
    return TRUE;
}

//------- ------------------------------------------------------------------------------------------
/// Description : Handle for BDMA copy data from ONLY Flash src to other dst
/// @param u32FlashAddr \b IN: Physical Source address in spi flash
/// @param u32DramAddr \b IN: Physical Dst address
/// @param u32Len \b IN: data length
/// @param eDstDev \b IN: The Dst Device of Flash BDMA
/// @param u8OpCfg \b IN: u8OpCfg: default is SPIDMA_OPCFG_DEF
/// - Bit0: inverse mode --> SPIDMA_OPCFG_INV_COPY
/// - Bit2: Copy & CRC check in wait mode --> SPIDMA_OPCFG_CRC_COPY
/// - Bit3: Copy without waiting --> SPIDMA_OPCFG_NOWAIT_COPY
/// @return \b MS_BOOL
/// [NONOS_SUPPORT]
/// [fw : drvBDMA ]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_CopyHnd(MS_PHYADDR u32FlashAddr,
                                          MS_PHYADDR u32DstAddr,
                                          MS_U32 u32Len,
                                          SPIDMA_Dev eDstDev,
                                          MS_U8 u8OpCfg)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_COPYHND pCopyHndParam = NULL;
        if (NULL == pInstantSerFlash)
		    return FALSE;
        pCopyHndParam = (PSERFLASH_COPYHND)malloc(sizeof(SERFLASH_COPYHND));
        if (NULL == pCopyHndParam)
            return FALSE;
		
		pCopyHndParam->u32FlashAddr = u32FlashAddr;
		pCopyHndParam->u32DstAddr = u32DstAddr;
		pCopyHndParam->u32Len = u32Len;
		pCopyHndParam->eDstDev = eDstDev;
		pCopyHndParam->u8OpCfg = u8OpCfg;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_CopyHnd,(void*)pCopyHndParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_CopyHnd fail\n");
            return FALSE;
        }
        return TRUE;
    }
#else

    #define BDMA_DEV_FLASH  5
    MS_U16 CpyType = ((BDMA_DEV_FLASH & 0x0F) | _LShift((eDstDev &0x0F), 8));
    MS_U32 u32Delay = FLASH_WAIT_TIME;

    while (!HAL_SERFLASH_CheckWriteDone())
    {
        if (FLASH_IS_TIMEOUT(u32Delay))
        {
            ULOGE(TAG_SERFLASH,"%s() : DMA flash is busy!\n",__FUNCTION__);
            return FALSE;
        }
        u32Delay--;
    }

    return MDrv_BDMA_CopyHnd(u32FlashAddr, u32DstAddr, u32Len, (BDMA_CpyType) CpyType, u8OpCfg);
#endif
}

//------- ------------------------------------------------------------------------------------------
/// Description : Switch SPI as GPIO Input
/// @param  bSwitch    \b IN: 1 for GPIO, 0 for NORMAL
/// @note   Not allowed in interrupt context
/// @note   For project's power consumption
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_SetGPIO(MS_BOOL bSwitch)
{
#if (SERFLASH_UTOPIA20)
    {
        PSERFLASH_SETGPIO pSetGPIOParam = NULL;
        if (NULL == pInstantSerFlash)
            return;
        pSetGPIOParam = (PSERFLASH_SETGPIO)malloc(sizeof(SERFLASH_SETGPIO));
        if (NULL == pSetGPIOParam)
            return;
		
        pSetGPIOParam->bSwitch = bSwitch;
        if(UtopiaIoctl(pInstantSerFlash,MDrv_CMD_SERFLASH_SetGPIO,(void*)pSetGPIOParam) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SERFLASH,"Ioctl MDrv_SERFLASH_WriteProtect fail\n");
            return;
        }
        return;
    }
#else

	HAL_SERFLASH_SetGPIO(bSwitch);
#endif
}

#ifdef MXIC_IBPM
//------- ------------------------------------------------------------------------------------------
/// Description : Enable Individual block protection mode, only MXIC support
/// @note   This is OTP mode, and then blocks are default individual protected after power up
/// @note   Customization
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
void MDrv_SERFLASH_EnterIBPM(void)
{
	_bIBPM = HAL_SPI_EnterIBPM();
}

//------- ------------------------------------------------------------------------------------------
/// Description : Read the individual block Protected status, only MXIC support
/// @param  u32FlashAddr    \b IN: the flash's address of the block
/// @return 0xFF : Protected Lock
/// @return 0x00 : Protected UnLock
/// @note   it will activate after calling MDrv_SERFLASH_EnterIBPM.
/// @note   Customization
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_SERFLASH_ReadBlockStatus(MS_PHYADDR u32FlashAddr)
{
    return HAL_SPI_ReadBlockStatus(u32FlashAddr);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Protect (or not) the special address of the block, only MXIC support
/// @param  u32FlashAddr    \b IN: the flash's address of the block
/// @param  bLock    \b IN: True: Lock ; False: Unlock
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   it will activate after calling MDrv_SERFLASH_EnterIBPM.
/// @note   Customization
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SERFLASH_SingleBlockProtect(MS_PHYADDR u32FlashAddr,MS_BOOL bLock)
{
	return HAL_SPI_SingleBlockLock(u32FlashAddr, bLock);
}

//------- ------------------------------------------------------------------------------------------
/// Description : Protect (or not) the continuous block, only MXIC support
/// @param  u32StartBlock    \b IN: the flash's block
/// @param  u32EndBlock    \b IN: the flash's block
/// @param  bLock    \b IN: True: Lock ; False: Unlock
/// @return u32PassCnt : the successful blocks number
/// @note   it will activate after calling MDrv_SERFLASH_EnterIBPM.
/// @note   Customization
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_SERFLASH_MultiBlockProtect(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bLock)
{
	MS_U32 u32InBlock = 0;
	MS_U32 u32PassCnt = 0;

	for ( u32InBlock = u32StartBlock; u32InBlock < u32StartBlock; u32InBlock++ )
	{
		if(HAL_SPI_SingleBlockLock((u32InBlock*SIZE_64KB), bLock))
		{
			u32PassCnt++;
		}
	}
	return u32PassCnt;
}
#endif

//-------------------------------------------------------------------------------------------------
//  WRAPPER FOR CHAKRA
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_FLASH_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s( 0x%x, 0x%x, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (unsigned int)u32FlashSize, user_buffer));
    return MDrv_SERFLASH_Write(u32FlashAddr, u32FlashSize, user_buffer);

}

MS_BOOL MDrv_FLASH_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s( 0x%x, 0x%x, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, (unsigned int)u32FlashSize, user_buffer));
    return MDrv_SERFLASH_Read(u32FlashAddr, u32FlashSize, user_buffer);
}

MS_BOOL MDrv_FLASH_WriteProtect(MS_BOOL bEnable)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d)\n", __FUNCTION__, (int)bEnable));
    return MDrv_SERFLASH_WriteProtect(bEnable);
}

MS_BOOL MDrv_FLASH_WriteProtect_Enable_All_Range(void)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return MDrv_SERFLASH_WriteProtect_Enable_All_Range();
}

MS_BOOL MDrv_FLASH_WriteProtect_Disable_All_Range(void)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return MDrv_SERFLASH_WriteProtect_Disable_All_Range();
}

MS_BOOL MDrv_FLASH_WriteProtect_Disable_Range_Set(MS_U32 DisableLowerBound, MS_U32 DisableSize)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%x, 0x%x)\n", __FUNCTION__, (unsigned int)DisableLowerBound, (unsigned int)DisableSize));
    return MDrv_SERFLASH_WriteProtect_Disable_Range_Set(DisableLowerBound, DisableSize);
}

MS_BOOL MDrv_FLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 BlockProtectBits)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%d, 0x%02x)\n", __FUNCTION__, (int)bEnableAllArea, BlockProtectBits));
    return MDrv_SERFLASH_WriteProtect_Area(bEnableAllArea, BlockProtectBits);
}

MS_BOOL MDrv_FLASH_ReadStatusRegister(MS_U8 *pu8StatusReg)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%p)\n", __FUNCTION__, pu8StatusReg));
    return MDrv_SERFLASH_ReadStatusRegister(pu8StatusReg);
}

MS_BOOL MDrv_FLASH_ReadStatusRegister2(MS_U8 *pu8StatusReg)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(%p)\n", __FUNCTION__, pu8StatusReg));
    return MDrv_SERFLASH_ReadStatusRegister2(pu8StatusReg);
}

MS_BOOL MDrv_FLASH_WriteStatusRegister(MS_U16 u16StatusReg)
{
    return MDrv_SERFLASH_WriteStatusRegister(u16StatusReg);
}
MS_BOOL MDrv_FLASH_DetectType(void)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return MDrv_SERFLASH_DetectType();
}

MS_BOOL MDrv_FLASH_DetectSize(MS_U32 *u32FlashSize)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return MDrv_SERFLASH_DetectSize(u32FlashSize);
}
MS_BOOL MDrv_FLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, %p)\n", __FUNCTION__, (unsigned int)u32FlashAddr, pu16BlockIndex));
    return MDrv_SERFLASH_AddressToBlock(u32FlashAddr, pu32BlockIndex);
}

MS_BOOL MDrv_FLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%04x, %p)\n", __FUNCTION__, u16BlockIndex, pu32FlashAddr));
    return MDrv_SERFLASH_BlockToAddress(u32BlockIndex, pu32FlashAddr);
}

MS_BOOL MDrv_FLASH_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)\n", __FUNCTION__, (unsigned int)u32StartAddr, (unsigned int)u32Size, (int)bWait));
    return MDrv_SERFLASH_AddressErase(u32StartAddr, u32EraseSize, bWait);
}

MS_BOOL MDrv_FLASH_BlockErase(MS_U16 u16StartBlock, MS_U16 u16EndBlock, MS_BOOL bWait)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%04x, 0x%04x, %d)\n", __FUNCTION__, u16StartBlock, u16EndBlock, (int)bWait));
    return MDrv_SERFLASH_BlockErase(u16StartBlock, u16EndBlock, bWait);
}

MS_BOOL MDrv_FLASH_CheckWriteDone(void)
{
    //DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s()\n", __FUNCTION__));
    return MDrv_SERFLASH_CheckWriteDone();
}

#ifdef MUNINN_ENABLE
//-------------------------------------------------------------------------------------------------
/// Get the Flash Status(register) of the Muninn
/// @param  u8ByteAddr                \b IN: Register Address
/// @return Flash Status
/// @note
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_OTP_ReadStatusReg(MS_U8 u8ByteAddr)
{
    return HAL_OTP_ReadStatusReg(u8ByteAddr);
}

//-------------------------------------------------------------------------------------------------
/// Set the Flash Status(register) of the Muninn
/// @param  u8ByteAddr                \b IN: Register Address
/// @param  u8ByteReg                \b IN: Register Value
/// @note
//-------------------------------------------------------------------------------------------------
void MDrv_OTP_WriteStatusReg(MS_U8 u8ByteAddr, MS_U8 u8ByteReg)
{
	HAL_OTP_WriteStatusReg(u8ByteAddr,u8ByteReg);
}

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Muninn
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return MS_PHYADDR : the Address of the done address
/// @note   Not allowed in interrupt context; Normal Read@only 12M Hz
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_Read(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    return HAL_OTP_Read(u32FlashAddr, u32FlashSize, user_buffer);
}

//-------------------------------------------------------------------------------------------------
/// Description : Read data from Muninn
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return MS_PHYADDR : the Address of the done address
/// @note   Not allowed in interrupt context; Fast Read@over 12M Hz
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_FastRead(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
    return HAL_OTP_FastRead(u32FlashAddr, u32FlashSize, user_buffer);
}

//-------------------------------------------------------------------------------------------------
/// Description : Write data into Muninn's array (OTP)
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  u32FlashSize    \b IN: Flash Size Data in Bytes
/// @param  user_buffer    \b OUT: Virtual Buffer Address ptr to store flash read data
/// @return MS_PHYADDR : the Address of the done address
/// @note   Not allowed in interrupt context; Write@only 12M Hz
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_PHYADDR MDrv_OTP_Write(MS_U32 u32FlashAddr, MS_U32 u32FlashSize, MS_U8 *user_buffer)
{
	MS_PHYADDR u32RetAddr = (MS_PHYADDR)u32FlashAddr;
	MS_U32 i;

	for( i = 0; i < u32FlashSize; i++ )
	{
    	HAL_OTP_Write(u32FlashAddr+i,*(user_buffer+i));
		u32RetAddr++;
	}
	return u32RetAddr;
}

//-------------------------------------------------------------------------------------------------
/// Description : Write data into Muninn's test-mode array (OTP)
/// @param  u32FlashAddr    \b IN: Flash Address
/// @param  user_buffer    \b IN: flash write data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context; Write@only 12M Hz
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_OTP_WriteTestMode(MS_U32 u32FlashAddr, MS_U8 ByteData)
{
	return HAL_OTP_WriteTest(u32FlashAddr, ByteData);
}
#endif


MS_BOOL MDrv_SERFLASH_WriteProtect_Area_Lookup(MS_U32 u32ProtectSpace, MS_U32 u32NonProtectSpace, MS_U8 *pu8BlockProtectBits)
{
    return HAL_SERFLASH_WriteProtect_Area_Lookup(u32ProtectSpace, u32NonProtectSpace, pu8BlockProtectBits);
}

MS_U32 MDrv_SERFLASH_WriteProtect_Area_Boundary(void)
{
    return HAL_SERFLASH_WriteProtect_Area_Boundary();
}


//FSP Driver =====

MS_BOOL MDrv_FSP_WriteData(MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data)
{
    return HAL_FSP_Write(u32Addr, u32DataSize, pu8Data);
}

MS_BOOL MDrv_FSP_ReadData(MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data)
{
    //ULOGE(TAG_SERFLASH,"%s( 0x%x, 0x%x, %p)\n", __FUNCTION__, (unsigned int)u32Addr, (unsigned int)u8DataSize, pu8Data);
    return HAL_FSP_Read(u32Addr, u32DataSize, pu8Data);
}

MS_BOOL MDrv_FSP_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait)
{
    MS_BOOL bRet = TRUE;
    MS_U32  u32Idx;
    MS_U32  u32FlashAddr = 0;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, ULOGD(TAG_SERFLASH,"%s(0x%08x, 0x%08x, %d)\n", __FUNCTION__, (unsigned int)u32StartBlock, (unsigned int)u32EndBlock, (int)bWait));
    MS_ASSERT( u32StartBlock<=u32EndBlock && u32EndBlock<NUMBER_OF_SERFLASH_SECTORS );
    HAL_FSP_Entry();
    for( u32Idx = u32StartBlock; u32Idx <= u32EndBlock; u32Idx++ )
    {
        bRet &= HAL_SERFLASH_BlockToAddress(u32Idx, &u32FlashAddr);
        bRet &= HAL_FSP_BlockErase(u32FlashAddr,FLASH_ERASE_64K);
    }
    HAL_FSP_Exit();
    return bRet;

}

MS_BOOL MDrv_FSP_AddressErase(MS_U32 u32StartAddr, MS_U32 u32EraseSize, MS_BOOL bWait)
{
    MS_U32  u32StartBlock;
    MS_U32  u32EndBlock;

    u32StartBlock = u32StartAddr / SERFLASH_BLOCK_SIZE;
    u32EndBlock = u32StartBlock + u32EraseSize - 1;
    return MDrv_FSP_BlockErase(u32StartBlock, u32EndBlock, bWait);
}

MS_U32 MDrv_SERFLASH_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL; 

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_SERFLASH_Init();

			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
			ULOGD(TAG_SERFLASH,"[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE(TAG_SERFLASH,"[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}

MS_BOOL MDrv_FLASH_Attribute(SPI_Attribute SpiAttri)
{
      switch (SpiAttri)
      {
      case E_SPI_ATTRI_USER_DRV:
          _SPI_Attri = E_SPI_ATTRI_USER_DRV;
          break;
      case E_SPI_ATTRI_KERNEL_DRV:
          _SPI_Attri = E_SPI_ATTRI_KERNEL_DRV;
          break;
      case E_SPI_ATTRI_UNKNOWN:

      default:
          DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));
          return FALSE;

          break;
      }
      return TRUE;
}



