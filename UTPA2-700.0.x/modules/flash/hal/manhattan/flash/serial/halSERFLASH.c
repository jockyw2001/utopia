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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>

// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvMMIO.h"

// Internal Definition
#include "drvSERFLASH.h"
#include "regSERFLASH.h"
#include "halSERFLASH.h"

// !!! Uranus Serial Flash Notes: !!!
//  - The clock of DMA & Read via XIU operations must be < 3*CPU clock
//  - The clock of DMA & Read via XIU operations are determined by only REG_ISP_CLK_SRC; other operations by REG_ISP_CLK_SRC only
//  - DMA program can't run on DRAM, but in flash ONLY
//  - DMA from SPI to DRAM => size/DRAM start/DRAM end must be 8-B aligned


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define WRITE_WORD_MASK(_reg, _val, _mask)  { (*((volatile MS_U16*)(_reg))) = ((*((volatile MS_U16*)(_reg))) & ~(_mask)) | ((MS_U16)(_val) & (_mask)); }


// XIU_ADDR
// #define SFSH_XIU_REG32(addr)                (*((volatile MS_U32 *)(_hal_isp.u32XiuBaseAddr + ((addr)<<2))))

#define SFSH_XIU_READ32(addr)               (*((volatile MS_U32 *)(_hal_isp.VirtXiuBaseAddr + ((addr)<<2)))) // TODO: check AEON 32 byte access order issue


// ISP_CMD

#define ISP_READ(addr)                      READ_WORD(_hal_isp.VirtIspBaseAddr + ((addr)<<2))
#define ISP_WRITE(addr, val)                WRITE_WORD(_hal_isp.VirtIspBaseAddr + ((addr)<<2), (val))
#define ISP_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(_hal_isp.VirtIspBaseAddr + ((addr)<<2), (val), (mask))

#define FSP_READ(addr)                      READ_WORD(_hal_isp.VirtFspBaseAddr + ((addr)<<2))
#define FSP_WRITE(addr, val)                WRITE_WORD(_hal_isp.VirtFspBaseAddr + ((addr)<<2), (val))
#define FSP_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(_hal_isp.VirtFspBaseAddr + ((addr)<<2), (val), (mask))

#define QSPI_READ(addr)                     READ_WORD(_hal_isp.VirtQspiBaseAddr + ((addr)<<2))
#define QSPI_WRITE(addr, val)               WRITE_WORD(_hal_isp.VirtQspiBaseAddr + ((addr)<<2), (val))
#define QSPI_WRITE_MASK(addr, val, mask)    WRITE_WORD_MASK(_hal_isp.VirtQspiBaseAddr + ((addr)<<2), (val), (mask))

#define BDMA_READ(addr)                      READ_WORD(_hal_isp.VirtBdmaBaseAddr + ((addr)<<2))
#define BDMA_WRITE(addr, val)                WRITE_WORD(_hal_isp.VirtBdmaBaseAddr + ((addr)<<2), (val))

#define SPI_FLASH_CMD(u8FLASHCmd)           ISP_WRITE(REG_ISP_SPI_COMMAND, (MS_U8)u8FLASHCmd)
#define SPI_WRITE_DATA(u8Data)              ISP_WRITE(REG_ISP_SPI_WDATA, (MS_U8)u8Data)
#define SPI_READ_DATA()                     READ_BYTE(_hal_isp.VirtIspBaseAddr + ((REG_ISP_SPI_RDATA)<<2))

#define MHEG5_READ(addr)                    READ_WORD(_hal_isp.VirtMheg5BaseAddr + ((addr)<<2))
#define MHEG5_WRITE(addr, val)              WRITE_WORD((_hal_isp.VirtMheg5BaseAddr + (addr << 2)), (val))
#define MHEG5_WRITE_MASK(addr, val, mask)   WRITE_WORD_MASK(_hal_isp.VirtMheg5BaseAddr + ((addr)<<2), (val), (mask))

// PIU_DMA
#define PIU_READ(addr)                      READ_WORD(_hal_isp.VirtPiuBaseAddr + ((addr)<<2))
#define PIU_WRITE(addr, val)                WRITE_WORD(_hal_isp.VirtPiuBaseAddr + ((addr)<<2), (val))
#define PIU_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(_hal_isp.VirtPiuBaseAddr + ((addr)<<2), (val), (mask))

// PM_SLEEP CMD.
#define PM_READ(addr)                      READ_WORD(_hal_isp.VirtPMBaseAddr+ ((addr)<<2))
#define PM_WRITE(addr, val)                WRITE_WORD(_hal_isp.VirtPMBaseAddr+ ((addr)<<2), (val))
#define PM_WRITE_MASK(addr, val, mask)     WRITE_WORD_MASK(_hal_isp.VirtPMBaseAddr+ ((addr)<<2), (val), (mask))

// CLK_GEN
#define CLK_READ(addr)                     READ_WORD(_hal_isp.VirtCLK0BaseAddr + ((addr)<<2))
#define CLK_WRITE(addr, val)               WRITE_WORD(_hal_isp.VirtCLK0BaseAddr + ((addr)<<2), (val))
#define CLK_WRITE_MASK(addr, val, mask)    WRITE_WORD_MASK(_hal_isp.VirtCLK0BaseAddr + ((addr)<<2), (val), (mask))

//MS_U32<->MS_U16
#define LOU16(u32Val)   ((MS_U16)(u32Val))
#define HIU16(u32Val)   ((MS_U16)((u32Val) >> 16))

//serial flash mutex wait time
#define SERFLASH_MUTEX_WAIT_TIME    3000

// Time-out system
#if !defined (MSOS_TYPE_NOS)
    #define SERFLASH_SAFETY_FACTOR      10

    #define SER_FLASH_TIME(_stamp, _msec)    { (_stamp) = MsOS_GetSystemTime() + (_msec); }
    #define SER_FLASH_EXPIRE(_stamp)         ( (MsOS_GetSystemTime() > (_stamp)) ? 1 : 0 )

#else // defined (MSOS_TYPE_NOS)
    #define SERFLASH_SAFETY_FACTOR      1000000

    #define SER_FLASH_TIME(_stamp, _msec)    (_stamp =_msec)
    #define SER_FLASH_EXPIRE(_stamp)         ( !(--_stamp))
#endif

#define CHK_NUM_WAITDONE     2000
#define SINGLE_WRITE_LENGTH  4
#define SINGLE_READ_LENGTH   8

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_VIRT  VirtXiuBaseAddr;     // REG_SFSH_XIU_BASE
    MS_VIRT  VirtMheg5BaseAddr;
    MS_VIRT  VirtIspBaseAddr;     // REG_ISP_BASE
    MS_VIRT  VirtFspBaseAddr;     // REG_FSP_BASE
    MS_VIRT  VirtQspiBaseAddr;    // REG_QSPI_BASE
    MS_VIRT  VirtPiuBaseAddr;     // REG_PIU_BASE
    MS_VIRT  VirtPMBaseAddr;      // REG_PM_BASE
    MS_VIRT  VirtCLK0BaseAddr;    // REG_PM_BASE
    MS_VIRT  VirtRiuBaseAddr;     // REG_PM_BASE
    MS_VIRT  VirtBdmaBaseAddr;
} hal_isp_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
hal_SERFLASH_t _hal_SERFLASH;
MS_U8 _u8SERFLASHDbgLevel;
MS_BOOL _bXIUMode = 0;      // default XIU mode, set 0 to RIU mode
MS_BOOL _bFSPMode = 0;
MS_BOOL bDetect = FALSE;    // initial flasg : true and false
MS_BOOL _bIBPM = FALSE;     // Individual Block Protect mode : true and false
MS_BOOL _bWPPullHigh = 0;   // WP pin pull high or can control info
MS_U8 u8Mode;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_S32 _s32SERFLASH_Mutex;

//
//  Sprcial Block Table (List)
//
static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25F10 =
{
    .u16Start     = 0,  // Start block# of special block size
    .u16End       = 3,  // End   block# of special block size
    .au32SizeList =     // List of special size;Total size must be equal to block size
    {
        SIZE_32KB,
        SIZE_32KB,
        SIZE_32KB,
        SIZE_32KB,
    }
};

static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B32B =
{
    .u16Start     = 0,  // Start block# of special block size
    .u16End       = 4,  // End   block# of special block size
    .au32SizeList =     // List of special size;Total size must be equal to block size
    {
        SIZE_4KB,
        SIZE_4KB,
        SIZE_8KB,
        SIZE_16KB,
        SIZE_32KB,
    }
};

static ST_SPECIAL_BLOCKS _stSpecialBlocks_EN25B64B =
{
    .u16Start     = 0,  // Start block# of special block size
    .u16End       = 4,  // End   block# of special block size
    .au32SizeList =     // List of special size;Total size must be equal to block size
    {
        SIZE_4KB,
        SIZE_4KB,
        SIZE_8KB,
        SIZE_16KB,
        SIZE_32KB,
    }
};

//-------------------------------------------------------------------------------------------------
//  Write Protect Table (List)
//-------------------------------------------------------------------------------------------------

static ST_WRITE_PROTECT _pstWriteProtectTable_W25X32[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(5:2, 1),   0x003F0000,     0x003FFFFF  },
    {   BITS(5:2, 2),   0x003E0000,     0x003FFFFF  },
    {   BITS(5:2, 3),   0x003C0000,     0x003FFFFF  },
    {   BITS(5:2, 4),   0x00380000,     0x003FFFFF  },
    {   BITS(5:2, 5),   0x00300000,     0x003FFFFF  },
    {   BITS(5:2, 6),   0x00200000,     0x003FFFFF  },
    {   BITS(5:2, 9),   0x00000000,     0x0000FFFF  },
    {   BITS(5:2, 10),  0x00000000,     0x0001FFFF  },
    {   BITS(5:2, 11),  0x00000000,     0x0003FFFF  },
    {   BITS(5:2, 12),  0x00000000,     0x0007FFFF  },
    {   BITS(5:2, 13),  0x00000000,     0x000FFFFF  },
    {   BITS(5:2, 14),  0x00000000,     0x001FFFFF  },
    {   BITS(5:2, 15),  0x00000000,     0x003FFFFF  },
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_W25X64[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(5:2, 1),   0x007E0000,     0x007FFFFF  },
    {   BITS(5:2, 2),   0x007C0000,     0x007FFFFF  },
    {   BITS(5:2, 3),   0x00780000,     0x007FFFFF  },
    {   BITS(5:2, 4),   0x00700000,     0x007FFFFF  },
    {   BITS(5:2, 5),   0x00600000,     0x007FFFFF  },
    {   BITS(5:2, 6),   0x00400000,     0x007FFFFF  },
    {   BITS(5:2, 9),   0x00000000,     0x0001FFFF  },
    {   BITS(5:2, 10),  0x00000000,     0x0003FFFF  },
    {   BITS(5:2, 11),  0x00000000,     0x0007FFFF  },
    {   BITS(5:2, 12),  0x00000000,     0x000FFFFF  },
    {   BITS(5:2, 13),  0x00000000,     0x001FFFFF  },
    {   BITS(5:2, 14),  0x00000000,     0x003FFFFF  },
    {   BITS(5:2, 15),  0x00000000,     0x007FFFFF  },
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },
};


static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL032K_CMP0[]=
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x01),   0x003F0000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x02),   0x003E0000,     0x003FFFFF  }, // 62-63
    {   BITS(6:2, 0x03),   0x003C0000,     0x003FFFFF  }, // 60-63
    {   BITS(6:2, 0x04),   0x00380000,     0x003FFFFF  }, // 56-63
    {   BITS(6:2, 0x05),   0x00300000,     0x003FFFFF  }, // 48-63
    {   BITS(6:2, 0x06),   0x00200000,     0x003FFFFF  }, // 32-63
    {   BITS(6:2, 0x09),   0x00000000,     0x0000FFFF  }, // 0
    {   BITS(6:2, 0x0A),   0x00000000,     0x0001FFFF  }, // 00-01
    {   BITS(6:2, 0x0B),   0x00000000,     0x0003FFFF  }, // 00-03
    {   BITS(6:2, 0x0C),   0x00000000,     0x0007FFFF  }, // 00-07
    {   BITS(6:2, 0x0D),   0x00000000,     0x000FFFFF  }, // 00-15
    {   BITS(6:2, 0x0E),   0x00000000,     0x001FFFFF  }, // 00-31
    {   BITS(6:2, 0x1F),   0x00000000,     0x003FFFFF  }, // 0-63
    {   BITS(6:2, 0x11),   0x003FF000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x12),   0x003FE000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x13),   0x003FC000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x14),   0x003F8000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x19),   0x00000000,     0x00000FFF  }, // 00
    {   BITS(6:2, 0x1A),   0x00000000,     0x00001FFF  }, // 00
    {   BITS(6:2, 0x1B),   0x00000000,     0x00003FFF  }, // 00
    {   BITS(6:2, 0x1C),   0x00000000,     0x00007FFF  }, // 00
};


static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL032K_CMP1[]=
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x01),   0x00000000,     0x003EFFFF  }, // 00-62
    {   BITS(6:2, 0x02),   0x00000000,     0x003DFFFF  }, // 00-61
    {   BITS(6:2, 0x03),   0x00000000,     0x003BFFFF  }, // 00-59
    {   BITS(6:2, 0x04),   0x00000000,     0x0037FFFF  }, // 00-55
    {   BITS(6:2, 0x05),   0x00000000,     0x002FFFFF  }, // 00-47
    {   BITS(6:2, 0x06),   0x00000000,     0x001FFFFF  }, // 00-31
    {   BITS(6:2, 0x09),   0x00010000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x0A),   0x00020000,     0x003FFFFF  }, // 02-63
    {   BITS(6:2, 0x0B),   0x00040000,     0x003FFFFF  }, // 04-63
    {   BITS(6:2, 0x0C),   0x00080000,     0x003FFFFF  }, // 08-63
    {   BITS(6:2, 0x0D),   0x00100000,     0x003FFFFF  }, // 16-63
    {   BITS(6:2, 0x0E),   0x00200000,     0x003FFFFF  }, // 32-63
    {   BITS(6:2, 0x1F),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x11),   0x00000000,     0x003FEFFF  }, // 00-63
    {   BITS(6:2, 0x12),   0x00000000,     0x003FDFFF  }, // 00-63
    {   BITS(6:2, 0x13),   0x00000000,     0x003FBFFF  }, // 00-62
    {   BITS(6:2, 0x14),   0x00000000,     0x003F7FFF  }, // 00-62
    {   BITS(6:2, 0x19),   0x00001000,     0x003FFFFF  }, // 00-63
    {   BITS(6:2, 0x1A),   0x00002000,     0x003FFFFF  }, // 00-63
    {   BITS(6:2, 0x1B),   0x00004000,     0x00003FFF  }, // 00-63
    {   BITS(6:2, 0x1C),   0x00008000,     0x003FFFFF  }, // 00-63
};

static ST_WRITE_PROTECT _pstWriteProtectTable_GD25Q16[] =
{
    //   BPX,             Lower Bound     Upper Bound
    {   BITS(6:2, 0x00),   0xFFFFFFFF,     0xFFFFFFFF  },
    {   BITS(6:2, 0x01),   0x001F0000,     0x001FFFFF  },
    {   BITS(6:2, 0x02),   0x001E0000,     0x001FFFFF  },
    {   BITS(6:2, 0x03),   0x001C0000,     0x001FFFFF  },
    {   BITS(6:2, 0x04),   0x00180000,     0x001FFFFF  },
    {   BITS(6:2, 0x05),   0x00100000,     0x001FFFFF  },

    {   BITS(6:2, 0x09),   0x00000000,     0x0000FFFF  },
    {   BITS(6:2, 0x0A),   0x00000000,     0x0001FFFF  },
    {   BITS(6:2, 0x0B),   0x00000000,     0x0003FFFF  },
    {   BITS(6:2, 0x0C),   0x00000000,     0x0007FFFF  },
    {   BITS(6:2, 0x0D),   0x00000000,     0x000FFFFF  },
    {   BITS(6:2, 0x0E),   0x00000000,     0x001FFFFF  },

    {   BITS(6:2, 0x11),   0x001FF000,     0x001FFFFF  },
    {   BITS(6:2, 0x12),   0x001EE000,     0x001FFFFF  },
    {   BITS(6:2, 0x13),   0x001FC000,     0x001FFFFF  },
    {   BITS(6:2, 0x14),   0x001F8000,     0x001FFFFF  },

    {   BITS(6:2, 0x19),   0x00000000,     0x00000FFF  },
    {   BITS(6:2, 0x1A),   0x00000000,     0x00001FFF  },
    {   BITS(6:2, 0x1B),   0x00000000,     0x00003FFF  },
    {   BITS(6:2, 0x1C),   0x00000000,     0x00007FFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_GD25Q32_CMP0[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x01),   0x003F0000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x02),   0x003E0000,     0x003FFFFF  }, // 62-63
    {   BITS(6:2, 0x03),   0x003C0000,     0x003FFFFF  }, // 60-63
    {   BITS(6:2, 0x04),   0x00380000,     0x003FFFFF  }, // 56-63
    {   BITS(6:2, 0x05),   0x00300000,     0x003FFFFF  }, // 48-63
    {   BITS(6:2, 0x06),   0x00200000,     0x003FFFFF  }, // 32-63

    {   BITS(6:2, 0x09),   0x00000000,     0x0000FFFF  }, // 00
    {   BITS(6:2, 0x0A),   0x00000000,     0x0001FFFF  }, // 00-01
    {   BITS(6:2, 0x0B),   0x00000000,     0x0003FFFF  }, // 00-03
    {   BITS(6:2, 0x0C),   0x00000000,     0x0007FFFF  }, // 00-07
    {   BITS(6:2, 0x0D),   0x00000000,     0x000FFFFF  }, // 00-15
    {   BITS(6:2, 0x0E),   0x00000000,     0x001FFFFF  }, // 00-31

    {   BITS(6:2, 0x11),   0x003FF000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x12),   0x003EE000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x13),   0x003FC000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x14),   0x003F8000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x15),   0x003F8000,     0x003FFFFF  }, // 63
    {   BITS(6:2, 0x16),   0x003F8000,     0x003FFFFF  }, // 63

    {   BITS(6:2, 0x19),   0x00000000,     0x00000FFF  }, // 00
    {   BITS(6:2, 0x1A),   0x00000000,     0x00001FFF  }, // 00
    {   BITS(6:2, 0x1B),   0x00000000,     0x00003FFF  }, // 00
    {   BITS(6:2, 0x1C),   0x00000000,     0x00007FFF  }, // 00
    {   BITS(6:2, 0x1D),   0x00000000,     0x00007FFF  }, // 00
    {   BITS(6:2, 0x1E),   0x00000000,     0x00007FFF  }, // 00

    {   BITS(6:2, 0x07),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x0F),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x17),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x1F),   0x00000000,     0x003FFFFF  }, // ALL

    {   BITS(6:2, 0x18),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x10),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x08),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
};

static ST_WRITE_PROTECT _pstWriteProtectTable_GD25Q32_CMP1[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x01),   0x00000000,     0x003EFFFF  }, // 00-62
    {   BITS(6:2, 0x02),   0x00000000,     0x003DFFFF  }, // 00-61
    {   BITS(6:2, 0x03),   0x00000000,     0x003BFFFF  }, // 00-59
    {   BITS(6:2, 0x04),   0x00000000,     0x0037FFFF  }, // 00-55
    {   BITS(6:2, 0x05),   0x00000000,     0x002FFFFF  }, // 00-47
    {   BITS(6:2, 0x06),   0x00000000,     0x001FFFFF  }, // 00-31

    {   BITS(6:2, 0x09),   0x00010000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x0A),   0x00020000,     0x003FFFFF  }, // 02-63
    {   BITS(6:2, 0x0B),   0x00040000,     0x003FFFFF  }, // 04-63
    {   BITS(6:2, 0x0C),   0x00080000,     0x003FFFFF  }, // 08-63
    {   BITS(6:2, 0x0D),   0x00100000,     0x003FFFFF  }, // 16-63
    {   BITS(6:2, 0x0E),   0x00200000,     0x003FFFFF  }, // 32-63

    {   BITS(6:2, 0x11),   0x00000000,     0x003FEFFF  }, // 00-62
    {   BITS(6:2, 0x12),   0x00000000,     0x003FDFFF  }, // 00-62
    {   BITS(6:2, 0x13),   0x00000000,     0x003FBFFF  }, // 00-62
    {   BITS(6:2, 0x14),   0x00000000,     0x003F7FFF  }, // 00-62
    {   BITS(6:2, 0x15),   0x00000000,     0x003F7FFF  }, // 00-62
    {   BITS(6:2, 0x16),   0x00000000,     0x003F7FFF  }, // 00-62

    {   BITS(6:2, 0x19),   0x00001000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x1A),   0x00002000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x1B),   0x00004000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x1C),   0x00008000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x1D),   0x00008000,     0x003FFFFF  }, // 01-63
    {   BITS(6:2, 0x1E),   0x00008000,     0x003FFFFF  }, // 01-63

    {   BITS(6:2, 0x07),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x0F),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x17),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE
    {   BITS(6:2, 0x1F),   0xFFFFFFFF,     0xFFFFFFFF  }, // NONE

    {   BITS(6:2, 0x18),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x10),   0x00000000,     0x003FFFFF  }, // ALL
    {   BITS(6:2, 0x08),   0x00000000,     0x003FFFFF  }, // ALL
};

static ST_WRITE_PROTECT _pstWriteProtectTable_W25Q64CV_CMP1[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0x00000000,     0x007FFFFF  },
    {   BITS(6:2, 0x01),   0x00000000,     0x007DFFFF  },
    {   BITS(6:2, 0x02),   0x00000000,     0x007BFFFF  },
    {   BITS(6:2, 0x03),   0x00000000,     0x0077FFFF  },
    {   BITS(6:2, 0x04),   0x00000000,     0x006FFFFF  },
    {   BITS(6:2, 0x05),   0x00000000,     0x005FFFFF  },
    {   BITS(6:2, 0x06),   0x00000000,     0x003FFFFF  },

    {   BITS(6:2, 0x09),   0x00020000,     0x007FFFFF  },
    {   BITS(6:2, 0x0A),   0x00040000,     0x007FFFFF  },
    {   BITS(6:2, 0x0B),   0x00080000,     0x007FFFFF  },
    {   BITS(6:2, 0x0C),   0x00100000,     0x007FFFFF  },
    {   BITS(6:2, 0x0D),   0x00200000,     0x007FFFFF  },
    {   BITS(6:2, 0x0E),   0x00400000,     0x007FFFFF  },
    {   BITS(6:2, 0x1F),   0xFFFFFFFF,     0xFFFFFFFF  },

    {   BITS(6:2, 0x11),   0x00000000,     0x007FEFFF  },
    {   BITS(6:2, 0x12),   0x00000000,     0x007FDFFF  },
    {   BITS(6:2, 0x13),   0x00000000,     0x007FBFFF  },
    {   BITS(6:2, 0x14),   0x00000000,     0x007F7FFF  },

    {   BITS(6:2, 0x19),   0x00001000,     0x007FFFFF  },
    {   BITS(6:2, 0x1A),   0x00002000,     0x007FFFFF  },
    {   BITS(6:2, 0x1B),   0x00004000,     0x007FFFFF  },
    {   BITS(6:2, 0x1C),   0x00008000,     0x007FFFFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_W25Q32BV_CMP1[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 0x00),   0x00000000,     0x003FFFFF  },
    {   BITS(6:2, 0x01),   0x00000000,     0x003EFFFF  },
    {   BITS(6:2, 0x02),   0x00000000,     0x003DFFFF  },
    {   BITS(6:2, 0x03),   0x00000000,     0x003BFFFF  },
    {   BITS(6:2, 0x04),   0x00000000,     0x0037FFFF  },
    {   BITS(6:2, 0x05),   0x00000000,     0x002FFFFF  },
    {   BITS(6:2, 0x06),   0x00000000,     0x001FFFFF  },

    {   BITS(6:2, 0x09),   0x00010000,     0x003FFFFF  },
    {   BITS(6:2, 0x0A),   0x00020000,     0x003FFFFF  },
    {   BITS(6:2, 0x0B),   0x00040000,     0x003FFFFF  },
    {   BITS(6:2, 0x0C),   0x00080000,     0x003FFFFF  },
    {   BITS(6:2, 0x0D),   0x00100000,     0x003FFFFF  },
    {   BITS(6:2, 0x0E),   0x00200000,     0x003FFFFF  },
    {   BITS(6:2, 0x1F),   0xFFFFFFFF,     0xFFFFFFFF  },

    {   BITS(6:2, 0x11),   0x00000000,     0x003FEFFF  },
    {   BITS(6:2, 0x12),   0x00000000,     0x003FDFFF  },
    {   BITS(6:2, 0x13),   0x00000000,     0x003FBFFF  },
    {   BITS(6:2, 0x14),   0x00000000,     0x003F7FFF  },

    {   BITS(6:2, 0x19),   0x00001000,     0x003FFFFF  },
    {   BITS(6:2, 0x1A),   0x00002000,     0x003FFFFF  },
    {   BITS(6:2, 0x1B),   0x00004000,     0x003FFFFF  },
    {   BITS(6:2, 0x1C),   0x00008000,     0x003FFFFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_W25Q128[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(5:2, 1),   0x007E0000,     0x007FFFFF  },
    {   BITS(5:2, 2),   0x007C0000,     0x007FFFFF  },
    {   BITS(5:2, 3),   0x00780000,     0x007FFFFF  },
    {   BITS(5:2, 4),   0x00700000,     0x007FFFFF  },
    {   BITS(5:2, 5),   0x00600000,     0x007FFFFF  },
    {   BITS(5:2, 6),   0x00400000,     0x007FFFFF  },
    {   BITS(5:2, 9),   0x00000000,     0x0001FFFF  },
    {   BITS(5:2, 10),  0x00000000,     0x0003FFFF  },
    {   BITS(5:2, 11),  0x00000000,     0x0007FFFF  },
    {   BITS(5:2, 12),  0x00000000,     0x000FFFFF  },
    {   BITS(5:2, 13),  0x00000000,     0x001FFFFF  },
    {   BITS(5:2, 14),  0x00000000,     0x003FFFFF  },
    {   BITS(5:2, 15),  0x00000000,     0x007FFFFF  },
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },
};

#define FROM_BLK(x, blk_size)   ((x) * (blk_size))
#define TO_BLK(x, blk_size)     (((x) + 1) * (blk_size) - 1)

static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L8005[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 7),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 7,  00~15
    {   BITS(4:2, 6),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 6,  00~15
    {   BITS(4:2, 5),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 5,  00~15
    {   BITS(4:2, 4),   FROM_BLK(  8, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 4,  08~15
    {   BITS(4:2, 3),   FROM_BLK( 12, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 3,  12~15
    {   BITS(4:2, 2),   FROM_BLK( 14, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 2,  14~15
    {   BITS(4:2, 1),   FROM_BLK( 15, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 1,  15~15
    {   BITS(4:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L3205D[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 14),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(62, SIZE_64KB) }, // 0~62
    {   BITS(5:2, 13),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(61, SIZE_64KB) }, // 0~61
    {   BITS(5:2, 12),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(59, SIZE_64KB) }, // 0~59
    {   BITS(5:2, 11),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(55, SIZE_64KB) }, // 0~55
    {   BITS(5:2, 10),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(47, SIZE_64KB) }, // 0~47
    {   BITS(5:2, 9),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(5:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(5:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(5:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(5:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(5:2, 15),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};
static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L3206E[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 15),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 14),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(62, SIZE_64KB) }, // 0~62
    {   BITS(5:2, 13),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(61, SIZE_64KB) }, // 0~61
    {   BITS(5:2, 12),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(59, SIZE_64KB) }, // 0~59
    {   BITS(5:2, 11),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(55, SIZE_64KB) }, // 0~55
    {   BITS(5:2, 10),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(47, SIZE_64KB) }, // 0~47
    {   BITS(5:2, 9),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 8),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 7),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(5:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(5:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(5:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(5:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_SST25VF032B[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(2:0, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(2:0, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(2:0, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(2:0, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(2:0, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(2:0, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(2:0, 7),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 00~63
    {   BITS(2:0, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L6405D[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 14),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(125, SIZE_64KB) }, // 0~125
    {   BITS(5:2, 13),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(123, SIZE_64KB) }, // 0~123
    {   BITS(5:2, 12),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(119, SIZE_64KB) }, // 0~119
    {   BITS(5:2, 11),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(111, SIZE_64KB) }, // 0~111
    {   BITS(5:2, 10),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(95,  SIZE_64KB) }, // 0~95
    {   BITS(5:2, 9),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(63,  SIZE_64KB) }, // 0~63
    {   BITS(5:2, 6),   FROM_BLK(64,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 64~127
    {   BITS(5:2, 5),   FROM_BLK(96,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 96~127
    {   BITS(5:2, 4),   FROM_BLK(112, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 112~127
    {   BITS(5:2, 3),   FROM_BLK(120, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 120~127
    {   BITS(5:2, 2),   FROM_BLK(124, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 124~127
    {   BITS(5:2, 1),   FROM_BLK(126, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 126~127
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 0~127
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L1606E[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 1),  FROM_BLK(31,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 31~31
    {   BITS(5:2, 2),  FROM_BLK(30,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 30~31
    {   BITS(5:2, 3),  FROM_BLK(28,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 28~31
    {   BITS(5:2, 4),  FROM_BLK(24,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 24~31
    {   BITS(5:2, 5),  FROM_BLK(16,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 16~31
    {   BITS(5:2, 6),  FROM_BLK(0,    SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 0~31
    {   BITS(5:2, 7),  FROM_BLK(0,    SIZE_64KB),   TO_BLK(31,  SIZE_64KB) }, // 0~31
    {   BITS(5:2, 8),  FROM_BLK(0,    SIZE_64KB),   TO_BLK(31,  SIZE_64KB) }, // 0~31
    {   BITS(5:2, 9),  FROM_BLK(0,    SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 10), FROM_BLK(0,    SIZE_64KB),   TO_BLK(15, SIZE_64KB) }, // 0~15
    {   BITS(5:2, 11), FROM_BLK(0,    SIZE_64KB),   TO_BLK(23, SIZE_64KB) }, // 0~23
    {   BITS(5:2, 12), FROM_BLK(0,    SIZE_64KB),   TO_BLK(27, SIZE_64KB) }, // 0~27
    {   BITS(5:2, 13), FROM_BLK(0,    SIZE_64KB),   TO_BLK(29, SIZE_64KB) }, // 0~29
    {   BITS(5:2, 14), FROM_BLK(0,    SIZE_64KB),   TO_BLK(30, SIZE_64KB) }, // 0~30
    {   BITS(5:2, 15), FROM_BLK(0,    SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // none
};
static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L6406E[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 14),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(125, SIZE_64KB) }, // 0~125
    {   BITS(5:2, 13),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(123, SIZE_64KB) }, // 0~123
    {   BITS(5:2, 12),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(119, SIZE_64KB) }, // 0~119
    {   BITS(5:2, 11),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(111, SIZE_64KB) }, // 0~111
    {   BITS(5:2, 10),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(95,  SIZE_64KB) }, // 0~95
    {   BITS(5:2, 9),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(63,  SIZE_64KB) }, // 0~63
    {   BITS(5:2, 6),   FROM_BLK(64,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 64~127
    {   BITS(5:2, 5),   FROM_BLK(96,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 96~127
    {   BITS(5:2, 4),   FROM_BLK(112, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 112~127
    {   BITS(5:2, 3),   FROM_BLK(120, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 120~127
    {   BITS(5:2, 2),   FROM_BLK(124, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 124~127
    {   BITS(5:2, 1),   FROM_BLK(126, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 126~127
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 0~127
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // none
};

// New MXIC Flash with the same RDID as MX25L6405D
static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L6445E[] =
{
    //   BPX,                    Lower Bound                            Upper Bound
    {   BITS(5:2, 6),   FROM_BLK(64,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 064~127
    {   BITS(5:2, 5),   FROM_BLK(96,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 096~127
    {   BITS(5:2, 4),   FROM_BLK(112, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 112~127
    {   BITS(5:2, 3),   FROM_BLK(120, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 120~127
    {   BITS(5:2, 2),   FROM_BLK(124, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 124~127
    {   BITS(5:2, 1),   FROM_BLK(126, SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 126~127
    {   BITS(5:2, 7),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 8),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 9),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 10),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 11),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 12),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 13),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 14),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 000~127
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25Q32A[] =
{
    //   BPX,                    Lower Bound                            Upper Bound
    {   BITS(5:2, 1),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(62, SIZE_64KB) }, // 00~62
    {   BITS(5:2, 2),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(61, SIZE_64KB) }, // 00~61
    {   BITS(5:2, 3),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(59, SIZE_64KB) }, // 00~59
    {   BITS(5:2, 4),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(55, SIZE_64KB) }, // 00~55
    {   BITS(5:2, 5),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(47, SIZE_64KB) }, // 00~47
    {   BITS(5:2, 6),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(31, SIZE_64KB) }, // 00~31
    {   BITS(5:2, 7),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 00~63
    {   BITS(5:2, 9),   FROM_BLK(1,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 01~63
    {   BITS(5:2, 10),  FROM_BLK(2,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 02~63
    {   BITS(5:2, 11),  FROM_BLK(4,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 04~63
    {   BITS(5:2, 12),  FROM_BLK(8,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 08~63
    {   BITS(5:2, 13),  FROM_BLK(16,  SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 16~63
    {   BITS(5:2, 14),  FROM_BLK(32,  SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(63, SIZE_64KB) }, // 00~63
    {   BITS(5:2, 8),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};
static ST_WRITE_PROTECT _pstWriteProtectTable_EN25Q64[] =
{
    //   BPX,                    Lower Bound                            Upper Bound
    {   BITS(5:2, 1),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(126, SIZE_64KB) }, // 00~126
    {   BITS(5:2, 2),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(125, SIZE_64KB) }, // 00~125
    {   BITS(5:2, 3),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(123, SIZE_64KB) }, // 00~123
    {   BITS(5:2, 4),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(119, SIZE_64KB) }, // 00~119
    {   BITS(5:2, 5),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(111, SIZE_64KB) }, // 00~111
    {   BITS(5:2, 6),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(95, SIZE_64KB) }, // 00~95
    {   BITS(5:2, 7),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 9),   FROM_BLK(1,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 10),  FROM_BLK(2,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 11),  FROM_BLK(4,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 12),  FROM_BLK(8,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 13),  FROM_BLK(16,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 14),  FROM_BLK(32,  SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(127, SIZE_64KB) }, // 00~127
    {   BITS(5:2, 8),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25Q128[] =
{
    //   BPX,                    Lower Bound                            Upper Bound
    {   BITS(5:2, 1),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(254, SIZE_64KB) }, // 00~254
    {   BITS(5:2, 2),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(253, SIZE_64KB) }, // 00~253
    {   BITS(5:2, 3),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(251, SIZE_64KB) }, // 00~251
    {   BITS(5:2, 4),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(247, SIZE_64KB) }, // 00~247
    {   BITS(5:2, 5),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(239, SIZE_64KB) }, // 00~239
    {   BITS(5:2, 6),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(223, SIZE_64KB) }, // 00~223
    {   BITS(5:2, 7),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 00~255
    {   BITS(5:2, 9),   FROM_BLK(1,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 01~255
    {   BITS(5:2, 10),  FROM_BLK(2,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 02~255
    {   BITS(5:2, 11),  FROM_BLK(4,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 04~255
    {   BITS(5:2, 12),  FROM_BLK(8,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 08~255
    {   BITS(5:2, 13),  FROM_BLK(16,  SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 16~255
    {   BITS(5:2, 14),  FROM_BLK(32,  SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 32~255
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 00~255
    {   BITS(5:2, 8),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};
/*
static ST_WRITE_PROTECT _pstWriteProtectTable_EN25F32[] =
{
    {   BITS(4:2, 7),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(4:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};
*/
static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L12805D[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 8),   FROM_BLK(128, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 128~255
    {   BITS(5:2, 7),   FROM_BLK(192, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 192~255
    {   BITS(5:2, 6),   FROM_BLK(224, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 224~255
    {   BITS(5:2, 5),   FROM_BLK(240, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 240~255
    {   BITS(5:2, 4),   FROM_BLK(248, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 248~255
    {   BITS(5:2, 3),   FROM_BLK(252, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 252~255
    {   BITS(5:2, 2),   FROM_BLK(254, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 254~255
    {   BITS(5:2, 1),   FROM_BLK(255, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 255
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 0~255
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // none
};

// New MXIC Flash with the same RDID as MX25L12805D
static ST_WRITE_PROTECT _pstWriteProtectTable_MX25L12845E[] =
{
    //  BPX,                            Lower Bound                 Upper Bound
    {   BITS(5:2, 14),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 14, 0~255
    {   BITS(5:2, 13),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 13, 0~255
    {   BITS(5:2, 12),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 12, 0~255
    {   BITS(5:2, 11),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 11, 0~255
    {   BITS(5:2, 10),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 10, 0~255
    {   BITS(5:2, 9),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 9,  0~255
    {   BITS(5:2, 8),   FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 8,  0~255
    {   BITS(5:2, 7),   FROM_BLK(128, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 7,  128~255
    {   BITS(5:2, 6),   FROM_BLK(192,  SIZE_64KB),  TO_BLK(255, SIZE_64KB) }, // 6,  192~255
    {   BITS(5:2, 5),   FROM_BLK(224,  SIZE_64KB),  TO_BLK(255, SIZE_64KB) }, // 5,  224~255
    {   BITS(5:2, 4),   FROM_BLK(240, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 4,  240~255
    {   BITS(5:2, 3),   FROM_BLK(248, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 3,  248~255
    {   BITS(5:2, 2),   FROM_BLK(252, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 2,  252~255
    {   BITS(5:2, 1),   FROM_BLK(254, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 1,  254~255
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 15, 0~255
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};

// NOTE: AT26DF321 could protect each sector independently and BITS(5:2, 1)~BITS(5:2, 14) is no change for protection.
// This table is just used to figure out the lower bound and upper bound (no such param in function argument).
static ST_WRITE_PROTECT _pstWriteProtectTable_AT26DF321[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 14),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(62, SIZE_64KB) }, // 0~62
    {   BITS(5:2, 13),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(61, SIZE_64KB) }, // 0~61
    {   BITS(5:2, 12),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(59, SIZE_64KB) }, // 0~59
    {   BITS(5:2, 11),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(55, SIZE_64KB) }, // 0~55
    {   BITS(5:2, 10),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(47, SIZE_64KB) }, // 0~47
    {   BITS(5:2, 9),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(5:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(5:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(5:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(5:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(5:2, 15),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_AT25DF321[] =
{
    {   BITS(5:2, 0),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL128P[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 7),   FROM_BLK(128, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 7,  128~255
    {   BITS(5:2, 6),   FROM_BLK(192, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 6,  192~255
    {   BITS(5:2, 5),   FROM_BLK(224, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 5,  224~255
    {   BITS(5:2, 4),   FROM_BLK(240, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 4,  240~255
    {   BITS(5:2, 3),   FROM_BLK(248, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 3,  248~255
    {   BITS(5:2, 2),   FROM_BLK(252, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 2,  252~255
    {   BITS(5:2, 1),   FROM_BLK(254, SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 1,  254~255
    {   BITS(5:2, 15),  FROM_BLK(0,   SIZE_64KB),   TO_BLK(255, SIZE_64KB) }, // 15, 0~255
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL008A[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 7),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 7,  00~15
    {   BITS(5:2, 6),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 6,  00~15
    {   BITS(5:2, 5),   FROM_BLK(  0, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 5,  00~15
    {   BITS(5:2, 4),   FROM_BLK(  8, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 4,  08~15
    {   BITS(5:2, 3),   FROM_BLK( 12, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 3,  12~15
    {   BITS(5:2, 2),   FROM_BLK( 14, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 2,  14~15
    {   BITS(5:2, 1),   FROM_BLK( 15, SIZE_64KB),   TO_BLK( 15, SIZE_64KB) }, // 1,  15~15
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25P16[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 1),   0x001F0000,     0x001FFFFF  },//1, 31
    {   BITS(4:2, 2),   0x001E0000,     0x001FFFFF  },//2, 30~31
    {   BITS(4:2, 3),   0x001C0000,     0x001FFFFF  },//3, 28~31
    {   BITS(4:2, 4),   0x00180000,     0x001FFFFF  },//4, 24~31
    {   BITS(4:2, 5),   0x00100000,     0x001FFFFF  },//5, 16~31
    {   BITS(4:2, 6),   0x00000000,     0x001FFFFF  },//6, all
    {   BITS(4:2, 7),   0x00000000,     0x001FFFFF  },//7, all
    {   BITS(4:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//0, none
};
static ST_WRITE_PROTECT _pstWriteProtectTable_EN25Q32[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 1),   0x00000000,     0x003EFFFF  },//1, 31
    {   BITS(5:2, 2),   0x00000000,     0x003DFFFF  },//2, 30~31
    {   BITS(5:2, 3),   0x00000000,     0x003BFFFF  },//3, 28~31
    {   BITS(5:2, 4),   0x00000000,     0x0037FFFF  },//4, 24~31
    {   BITS(5:2, 5),   0x00000000,     0x002FFFFF  },//5, 16~31
    {   BITS(5:2, 6),   0x00000000,     0x001FFFFF  },//6, all
    {   BITS(5:2, 7),   0x00000000,     0x003FFFFF  },//7, all
    {   BITS(5:2, 8),   0xFFFFFFFF,     0xFFFFFFFF  },//8, 31
    {   BITS(5:2, 9),   0x003FFFFF,     0x00010000  },//9, 30~31
    {   BITS(5:2, 10),  0x003FFFFF,     0x00020000  },//10, 28~31
    {   BITS(5:2, 11),  0x003FFFFF,     0x00040000  },//11, 24~31
    {   BITS(5:2, 12),  0x003FFFFF,     0x00080000  },//12, 16~31
    {   BITS(5:2, 13),  0x003FFFFF,     0x00100000  },//13, all
    {   BITS(5:2, 14),  0x003FFFFF,     0x00200000  },//14, all
    {   BITS(5:2, 15),  0x00000000,     0x003FFFFF  },//15, all
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//0, none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25Q16[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 1),   0x00000000,     0x001EFFFF  },//1, 31
    {   BITS(5:2, 2),   0x00000000,     0x001DFFFF  },//2, 30~31
    {   BITS(5:2, 3),   0x00000000,     0x001BFFFF  },//3, 28~31
    {   BITS(5:2, 4),   0x00000000,     0x0017FFFF  },//4, 24~31
    {   BITS(5:2, 5),   0x00000000,     0x000FFFFF  },//5, 16~31
    {   BITS(5:2, 6),   0x00000000,     0x001FFFFF  },//6, all
    {   BITS(5:2, 7),   0x00000000,     0x001FFFFF  },//7, all
    {   BITS(5:2, 8),   0xFFFFFFFF,     0xFFFFFFFF  },//8, 31
    {   BITS(5:2, 9),   0x001FFFFF,     0x00010000  },//9, 30~31
    {   BITS(5:2, 10),  0x001FFFFF,     0x00020000  },//10, 28~31
    {   BITS(5:2, 11),  0x001FFFFF,     0x00040000  },//11, 24~31
    {   BITS(5:2, 12),  0x001FFFFF,     0x00080000  },//12, 16~31
    {   BITS(5:2, 13),  0x001FFFFF,     0x00100000  },//13, all
    {   BITS(5:2, 14),  0x001FFFFF,     0x00000000  },//14, all
    {   BITS(5:2, 15),  0x001FFFFF,     0x00000000  },//15, all
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//0, none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25F16[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 1),   0x001F0000,     0x001FFFFF  },//1, 31
    {   BITS(4:2, 2),   0x001E0000,     0x001FFFFF  },//2, 30~31
    {   BITS(4:2, 3),   0x001C0000,     0x001FFFFF  },//3, 28~31
    {   BITS(4:2, 4),   0x00180000,     0x001FFFFF  },//4, 24~31
    {   BITS(4:2, 5),   0x00100000,     0x001FFFFF  },//5, 16~31
    {   BITS(4:2, 6),   0x00000000,     0x001FFFFF  },//6, all
    {   BITS(4:2, 7),   0x00000000,     0x001FFFFF  },//7, all
    {   BITS(4:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//0, none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25F32[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 1),   0x00000000,     0x003EFFFF  },//0~62
    {   BITS(5:2, 2),   0x00000000,     0x003DFFFF  },//0~61
    {   BITS(5:2, 3),   0x00000000,     0x003BFFFF  },//0~59
    {   BITS(5:2, 4),   0x00000000,     0x0037FFFF  },//0~55
    {   BITS(5:2, 5),   0x00000000,     0x002FFFFF  },//0~47
    {   BITS(5:2, 6),   0x00000000,     0x001FFFFF  },//0~31
    {   BITS(5:2, 7),   0x00000000,     0x003FFFFF  },//all
    {   BITS(5:2, 8),   0xFFFFFFFF,     0xFFFFFFFF  },//none
    {   BITS(5:2, 9),   0x003FFFFF,     0x00010000  },//63~1
    {   BITS(5:2, 10),  0x003FFFFF,     0x00020000  },//63~2
    {   BITS(5:2, 11),  0x003FFFFF,     0x00040000  },//63~4
    {   BITS(5:2, 12),  0x003FFFFF,     0x00080000  },//63~8
    {   BITS(5:2, 13),  0x003FFFFF,     0x00100000  },//63~16
    {   BITS(5:2, 14),  0x003FFFFF,     0x00200000  },//63~32
    {   BITS(5:2, 15),  0x00000000,     0x003FFFFF  },//all
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//none

};


static ST_WRITE_PROTECT _pstWriteProtectTable_W25Q80[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(6:2, 1),   0x000F0000,     0x000FFFFF  },
    {   BITS(6:2, 2),   0x000E0000,     0x000FFFFF  },
    {   BITS(6:2, 3),   0x000C0000,     0x000FFFFF  },
    {   BITS(6:2, 4),   0x00080000,     0x000FFFFF  },
    {   BITS(6:2, 9),   0x00000000,     0x0000FFFF  },
    {   BITS(6:2, 10),  0x00000000,     0x0001FFFF  },
    {   BITS(6:2, 11),  0x00000000,     0x0003FFFF  },
    {   BITS(6:2, 12),  0x00000000,     0x0007FFFF  },
    {   BITS(6:2, 15),  0x00000000,     0x000FFFFF  },
    {   BITS(6:2, 17),  0x000FF000,     0x000FFFFF  },
    {   BITS(6:2, 18),  0x000FE000,     0x000FFFFF  },
    {   BITS(6:2, 19),  0x000FC000,     0x000FFFFF  },
    {   BITS(6:2, 20),  0x000F8000,     0x000FFFFF  },
    {   BITS(6:2, 25),  0x00000000,     0x00000FFF  },
    {   BITS(6:2, 26),  0x00000000,     0x00001FFF  },
    {   BITS(6:2, 27),  0x00000000,     0x00003FFF  },
    {   BITS(6:2, 28),  0x00000000,     0x00007FFF  },
    {   BITS(6:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_W25X80[] =
{
    //   BPX,                    Lower Bound         Upper Bound
    {   BITS(5:2, 1),   0x000F0000,     0x000FFFFF  },
    {   BITS(5:2, 2),   0x000E0000,     0x000FFFFF  },
    {   BITS(5:2, 3),   0x000C0000,     0x000FFFFF  },
    {   BITS(5:2, 4),   0x00080000,     0x000FFFFF  },
    {   BITS(5:2, 9),   0x00000000,     0x0000FFFF  },
    {   BITS(5:2, 10),  0x00000000,     0x0001FFFF  },
    {   BITS(5:2, 11),  0x00000000,     0x0003FFFF  },
    {   BITS(5:2, 12),  0x00000000,     0x0007FFFF  },
    {   BITS(5:2, 13),  0x00000000,     0x000FFFFF  },
    {   BITS(5:2, 15),  0x00000000,     0x000FFFFF  },
    {   BITS(5:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },
};

static ST_WRITE_PROTECT _pstWriteProtectTable_EN25F80[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 1),   0x000F0000,     0x000FFFFF  },//1, 15
    {   BITS(4:2, 2),   0x000E0000,     0x000FFFFF  },//2, 14~15
    {   BITS(4:2, 3),   0x000C0000,     0x000FFFFF  },//3, 12~15
    {   BITS(4:2, 4),   0x00080000,     0x000FFFFF  },//4, 8~15
    {   BITS(4:2, 5),   0x00000000,     0x000FFFFF  },//5, all
    {   BITS(4:2, 6),   0x00000000,     0x000FFFFF  },//6, all
    {   BITS(4:2, 7),   0x00000000,     0x000FFFFF  },//7, all
    {   BITS(4:2, 0),   0xFFFFFFFF,     0xFFFFFFFF  },//0, NONE

};
static ST_WRITE_PROTECT _pstWriteProtectTable_PM25LQ032C[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 15),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 14),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(31, SIZE_64KB) }, // 0~31
    {   BITS(5:2, 13),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(15, SIZE_64KB) }, // 0~15
    {   BITS(5:2, 12),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(7, SIZE_64KB) }, // 0~7
    {   BITS(5:2, 11),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(3, SIZE_64KB) }, // 0~3
    {   BITS(5:2, 10),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(1, SIZE_64KB) }, // 0~1
    {   BITS(5:2, 9),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(0, SIZE_64KB) }, // 0~0
    {   BITS(5:2, 8),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
    {   BITS(5:2, 7),   FROM_BLK(0, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(5:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(5:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(5:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(5:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none

};

static ST_WRITE_PROTECT _pstWriteProtectTable_M25P16[] =
{
    //  BPX,           Lower Bound                Upper Bound
    {   BITS(4:2, 0),  0xFFFFFFFF,                0xFFFFFFFF           }, // 0,  none
    {   BITS(4:2, 1),  FROM_BLK(31,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 1, 7
    {   BITS(4:2, 2),  FROM_BLK(30,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 2, 6-7
    {   BITS(4:2, 3),  FROM_BLK(28,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 3, 4-7
    {   BITS(4:2, 4),  FROM_BLK(24,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 4, all
    {   BITS(4:2, 5),  FROM_BLK(16,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 7, all
    {   BITS(4:2, 6),  FROM_BLK(0,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 7, all
    {   BITS(4:2, 7),  FROM_BLK(0,  SIZE_64KB),   TO_BLK(31, SIZE_64KB)  }, // 7, all

};

static ST_WRITE_PROTECT _pstWriteProtectTable_N25Q32[] =
{
    //  BPX,           Lower Bound                Upper Bound
    {   BITS(5:2, 0),  0xFFFFFFFF,                0xFFFFFFFF           }, // 0,  none
    {   BITS(5:2, 1),  FROM_BLK(63,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 1, 7
    {   BITS(5:2, 2),  FROM_BLK(62,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 2, 6-7
    {   BITS(5:2, 3),  FROM_BLK(60,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 3, 4-7
    {   BITS(5:2, 4),  FROM_BLK(56,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 4, all
    {   BITS(5:2, 5),  FROM_BLK(48,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 6),  FROM_BLK(32,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 7),  FROM_BLK(0,  SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 8),  0xFFFFFFFF,                0xFFFFFFFF           }, // 0,  none
    {   BITS(5:2, 9),  FROM_BLK(0,  SIZE_64KB),  TO_BLK(0, SIZE_64KB)  }, // 1, 7
    {   BITS(5:2, 10), FROM_BLK(0,  SIZE_64KB),  TO_BLK(1, SIZE_64KB)  }, // 2, 6-7
    {   BITS(5:2, 11), FROM_BLK(0,  SIZE_64KB),  TO_BLK(3, SIZE_64KB)  }, // 3, 4-7
    {   BITS(5:2, 12), FROM_BLK(0,  SIZE_64KB),  TO_BLK(7, SIZE_64KB)  }, // 4, all
    {   BITS(5:2, 13), FROM_BLK(0,  SIZE_64KB),  TO_BLK(15, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 14), FROM_BLK(0,  SIZE_64KB),  TO_BLK(31, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 15), FROM_BLK(0,   SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 7, all
};

static ST_WRITE_PROTECT _pstWriteProtectTable_N25Q64[] =
{
    //  BPX,           Lower Bound                Upper Bound
    {   BITS(5:2, 0),  0xFFFFFFFF,                0xFFFFFFFF           }, // 0,  none
    {   BITS(5:2, 1),  FROM_BLK(127,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 1, 7
    {   BITS(5:2, 2),  FROM_BLK(126,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 2, 6-7
    {   BITS(5:2, 3),  FROM_BLK(124,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 3, 4-7
    {   BITS(5:2, 4),  FROM_BLK(120,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 4, all
    {   BITS(5:2, 5),  FROM_BLK(112,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 6),  FROM_BLK(96,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 7),  FROM_BLK(64,  SIZE_64KB),   TO_BLK(127, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 8),  0xFFFFFFFF,                0xFFFFFFFF           }, // 0,  none
    {   BITS(5:2, 9),  FROM_BLK(0,  SIZE_64KB),  TO_BLK(0, SIZE_64KB)  }, // 1, 7
    {   BITS(5:2, 10), FROM_BLK(0,  SIZE_64KB),  TO_BLK(1, SIZE_64KB)  }, // 2, 6-7
    {   BITS(5:2, 11), FROM_BLK(0,  SIZE_64KB),  TO_BLK(3, SIZE_64KB)  }, // 3, 4-7
    {   BITS(5:2, 12), FROM_BLK(0,  SIZE_64KB),  TO_BLK(7, SIZE_64KB)  }, // 4, all
    {   BITS(5:2, 13), FROM_BLK(0,  SIZE_64KB),  TO_BLK(15, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 14), FROM_BLK(0,  SIZE_64KB),  TO_BLK(31, SIZE_64KB)  }, // 7, all
    {   BITS(5:2, 15), FROM_BLK(0,   SIZE_64KB),   TO_BLK(63, SIZE_64KB)  }, // 7, all
};

static ST_WRITE_PROTECT _pstWriteProtectTable_N25Q128[] =
{
    //  BPX,           Lower Bound                Upper Bound
    {   BITS(5:2, 0),  0xFFFFFFFF,                0xFFFFFFFF           },
    {   BITS(5:2, 1),  FROM_BLK(255,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 2),  FROM_BLK(254,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 3),  FROM_BLK(252,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 4),  FROM_BLK(248,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 5),  FROM_BLK(240,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 6),  FROM_BLK(224,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 7),  FROM_BLK(192,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 8),  FROM_BLK(128,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 9),  FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 10), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 11), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 12), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 13), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 14), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
    {   BITS(5:2, 15), FROM_BLK(0  ,  SIZE_64KB),   TO_BLK(255, SIZE_64KB)  },
};

/*
static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL032P[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 7),   FROM_BLK( 0, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 7,  00~63
    {   BITS(4:2, 6),   FROM_BLK( 32, SIZE_64KB), TO_BLK( 63, SIZE_64KB) }, // 6,  32~63
    {   BITS(4:2, 5),   FROM_BLK( 48, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 5,  48~63
    {   BITS(4:2, 4),   FROM_BLK( 56, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 4,  56~63
    {   BITS(4:2, 3),   FROM_BLK( 60, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 3,  60~63
    {   BITS(4:2, 2),   FROM_BLK( 62, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 2,  62~63
    {   BITS(4:2, 1),   FROM_BLK( 63, SIZE_64KB),   TO_BLK( 63, SIZE_64KB) }, // 1,  63~63
    {   BITS(4:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};
static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL064P[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 7),   FROM_BLK( 0, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 7,  00~127
    {   BITS(4:2, 6),   FROM_BLK( 64, SIZE_64KB), TO_BLK( 127, SIZE_64KB) }, // 6,  64~127
    {   BITS(4:2, 5),   FROM_BLK( 96, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 5,  96~127
    {   BITS(4:2, 4),   FROM_BLK( 112, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 4,  112~127
    {   BITS(4:2, 3),   FROM_BLK( 120, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 3,  120~127
    {   BITS(4:2, 2),   FROM_BLK( 124, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 2,  124~127
    {   BITS(4:2, 1),   FROM_BLK( 126, SIZE_64KB),   TO_BLK( 127, SIZE_64KB) }, // 1,  126~127
    {   BITS(4:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF             }, // 0,  none
};
*/

static ST_WRITE_PROTECT _pstWriteProtectTable_F25L032QA[] =
{    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(5:2, 15),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 14),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(62, SIZE_64KB) }, // 0~62
    {   BITS(5:2, 13),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(61, SIZE_64KB) }, // 0~61
    {   BITS(5:2, 12),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(59, SIZE_64KB) }, // 0~59
    {   BITS(5:2, 11),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(55, SIZE_64KB) }, // 0~55
    {   BITS(5:2, 10),  FROM_BLK(0,  SIZE_64KB),    TO_BLK(47, SIZE_64KB) }, // 0~47
    {   BITS(5:2, 9),   FROM_BLK(0,  SIZE_64KB),    TO_BLK(31, SIZE_64KB) }, // 0~31

    {   BITS(5:2, 8),   FROM_BLK(0, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63
    {   BITS(5:2, 7),   FROM_BLK(0, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 0~63

    {   BITS(5:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(5:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(5:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(5:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(5:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(5:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(5:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_F25L032PA[] =
{    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(4:2, 7),   FROM_BLK(0, SIZE_64KB),     TO_BLK(63, SIZE_64KB) }, // 0~63

    {   BITS(4:2, 6),   FROM_BLK(32, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 32~63
    {   BITS(4:2, 5),   FROM_BLK(48, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 48~63
    {   BITS(4:2, 4),   FROM_BLK(56, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 56~63
    {   BITS(4:2, 3),   FROM_BLK(60, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 60~63
    {   BITS(4:2, 2),   FROM_BLK(62, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 62~63
    {   BITS(4:2, 1),   FROM_BLK(63, SIZE_64KB),    TO_BLK(63, SIZE_64KB) }, // 63
    {   BITS(4:2, 0),   0xFFFFFFFF,                 0xFFFFFFFF            }, // none
};

static ST_WRITE_PROTECT _pstWriteProtectTable_S25FL164K[] =
{
    //  BPX,            Lower Bound                 Upper Bound
    {   BITS(6:2,31),   FROM_BLK(   0, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,30),   0xFFFFFFFF               ,  0xFFFFFFFF              },
    {   BITS(6:2,29),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   0, SIZE_64KB) },
    {   BITS(6:2,28),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   0, SIZE_64KB) },
    {   BITS(6:2,27),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   0, SIZE_64KB) },
    {   BITS(6:2,26),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   0, SIZE_64KB) },
    {   BITS(6:2,25),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   0, SIZE_64KB) },
    {   BITS(6:2,24),   0xFFFFFFFF,                 0xFFFFFFFF              },
    {   BITS(6:2,23),   FROM_BLK(   0, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,22),   0xFFFFFFFF               ,  0xFFFFFFFF              },
    {   BITS(6:2,21),   FROM_BLK( 127, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,20),   FROM_BLK( 127, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,19),   FROM_BLK( 127, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,18),   FROM_BLK( 127, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,17),   FROM_BLK( 127, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,16),   0xFFFFFFFF              ,   0xFFFFFFFF              },
    {   BITS(6:2,15),   FROM_BLK(   0, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2,14),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(  63, SIZE_64KB) },
    {   BITS(6:2,13),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(  31, SIZE_64KB) },
    {   BITS(6:2,12),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(  15, SIZE_64KB) },
    {   BITS(6:2,11),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   7, SIZE_64KB) },
    {   BITS(6:2,10),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   3, SIZE_64KB) },
    {   BITS(6:2, 9),   FROM_BLK(   0, SIZE_64KB),  TO_BLK(   1, SIZE_64KB) },
    {   BITS(6:2, 8),   0xFFFFFFFF              ,   0xFFFFFFFF              },
    {   BITS(6:2, 7),   FROM_BLK(   0, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 6),   FROM_BLK(  64, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 5),   FROM_BLK(  96, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 4),   FROM_BLK( 112, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 3),   FROM_BLK( 120, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 2),   FROM_BLK( 124, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 1),   FROM_BLK( 126, SIZE_64KB),  TO_BLK( 127, SIZE_64KB) },
    {   BITS(6:2, 0),   0xFFFFFFFF               ,  0xFFFFFFFF              },
};

//
//  Flash Info Table (List)
//
static hal_SERFLASH_t _hal_SERFLASH_table[] =   // Need to Add more sample for robust
{
    /**********************************************************************/
    /* 01.  u16FlashType                                                                                                 */
    /* 02.  u8MID                                                                                                           */
    /* 03.  u8DID0                                                                                                          */
    /* 04.  u8DID1                                                                                                          */
    /* 05.  pWriteProtectTable                                                                                          */
    /* 06.  pSpecialBlocks                                                                                                */
    /* 07.  u32FlashSize                                                                                                   */
    /* 08.  u32NumBLK                                                                                                    */
    /* 09.  u32BlockSize                                                                                                   */
    /* 10.  u16PageSize                                                                                                   */
    /* 11.  u16MaxChipWrDoneTimeout                                                                               */
    /* 12.  u8WrsrBlkProtect                                                                                             */
    /* 13.  u16DevSel                                                                                                      */
    /* 14.  u16SpiEndianSel                                                                                              */
    /* 15.  Support 2XREAD(SPI CMD is 0xBB)                                                                      */
    /**********************************************************************/

    { FLASH_IC_SST25VF032B, MID_SST,    0x25,   0x4A,   _pstWriteProtectTable_SST25VF032B,  NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(2:0, 0x07),    ISP_DEV_SST,    ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_MX25L3205D,  MID_MXIC,   0x20,   0x16,   _pstWriteProtectTable_MX25L3205D,   NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_MX25L6405D,  MID_MXIC,   0x20,   0x17,   _pstWriteProtectTable_MX25L6405D,   NULL,                       0x800000,   128,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_MX25L1606E,  MID_MXIC,   0x20,   0x15,   _pstWriteProtectTable_MX25L1606E,   NULL,                       0x200000,   32,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_MX25L12805D, MID_MXIC,   0x20,   0x18,   _pstWriteProtectTable_MX25L12805D,  NULL,                       0x1000000,  256,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MX25L3206E,  MID_MXIC,   0x20,   0x15,   _pstWriteProtectTable_MX25L3206E,   NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_MX25L8005,   MID_MXIC,   0x20,   0x14,   _pstWriteProtectTable_MX25L8005,    NULL,                       0x100000,   16,     SIZE_64KB,  256,    50,     BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MX25L6406E,  MID_MXIC,   0x20,   0x17,   _pstWriteProtectTable_MX25L6406E,   NULL,                       0x800000,   128,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25Q16,	    MID_WB, 	0x40,	0x15,	NULL,                             	NULL,						0x200000,	32, 	SIZE_64KB,	256,	50, 	BITS(4:2, 0x07),	ISP_DEV_ST,	    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_W25X32,      MID_WB,     0x30,   0x16,   _pstWriteProtectTable_W25X32,       NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25Q32,      MID_WB,     0x40,   0x16,   _pstWriteProtectTable_W25X32,       NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25X64,      MID_WB,     0x30,   0x17,   _pstWriteProtectTable_W25X64,       NULL,                       0x800000,   128,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25Q64,      MID_WB,     0x40,   0x17,   _pstWriteProtectTable_W25X64,       NULL,                       0x800000,   128,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25X80,		MID_WB, 	0x30,	0x14,	_pstWriteProtectTable_W25X80,		NULL,						0x100000,	16, 	SIZE_64KB,	256,	50, 	BITS(5:2, 0x0F),	ISP_DEV_ST,	    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_W25Q80,      MID_WB,     0x40,   0x14,   _pstWriteProtectTable_W25Q80,       NULL,                       0x100000,   16,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_W25Q128,     MID_WB,     0x40,   0x18,   _pstWriteProtectTable_W25Q128,      NULL,                       0x1000000,  256,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_AT25DF321A,  MID_ATMEL,  0x47,   0x01,   _pstWriteProtectTable_AT25DF321,    NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ATMEL,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_AT26DF321,   MID_ATMEL,  0x47,   0x00,   _pstWriteProtectTable_AT26DF321,    NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ATMEL,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_STM25P32,    MID_ST,     0x20,   0x16,   NULL,                               NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25B32B,    MID_EON,    0x20,   0x16,   NULL,                               &_stSpecialBlocks_EN25B32B, 0x400000,   68,     SIZE_64KB,  256,    384,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25B64B,    MID_EON,    0x20,   0x17,   NULL,                               &_stSpecialBlocks_EN25B64B, 0x800000,   132,    SIZE_64KB,  256,    384,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25Q32A,    MID_EON,    0x30,   0x16,   _pstWriteProtectTable_EN25Q32A,     NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25Q64,      MID_EON,    0x30,   0x17,   _pstWriteProtectTable_EN25Q64,     NULL,                       0x800000,   128,     SIZE_64KB,  256,    50,    BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25Q128,    MID_EON,    0x30,   0x18,   _pstWriteProtectTable_EN25Q128,     NULL,                       0x1000000,   256,   SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25F10,     MID_EON,    0x31,   0x11,   NULL,                               &_stSpecialBlocks_EN25F10,  0x20000,    4,      SIZE_32KB,  256,    384,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25F16,     MID_EON,    0x31,   0x15,   _pstWriteProtectTable_EN25F16,      NULL,  						0x200000,   32,     SIZE_64KB,  256,    384,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25F32,     MID_EON,    0x31,   0x16,   _pstWriteProtectTable_EN25F32,      NULL,                       0x400000,   64,     SIZE_64KB,  256,    384,    BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25F80,     MID_EON,    0x31,   0x14,   _pstWriteProtectTable_EN25F80,      NULL,                       0x100000,   16,     SIZE_64KB,  256,    384,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25P16,     MID_EON,    0x20,   0x15,    _pstWriteProtectTable_EN25P16,      NULL,                      0x200000,   32,     SIZE_64KB,  256,    50,     BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25QH16,     MID_EON,    0x70,   0x15,    _pstWriteProtectTable_EN25Q16,      NULL,                      0x200000,   32,     SIZE_64KB,  256,    50,    BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL032P,   MID_SPAN,   0x02,   0x15,   NULL,                               NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(4:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL064P,   MID_SPAN,   0x02,   0x16,   NULL,                               NULL,                       0x800000,   128,    SIZE_64KB,  256,    50,     BITS(4:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL032K,   MID_SPAN,   0x40,   0x16,   _pstWriteProtectTable_S25FL032K_CMP0, NULL,                     0x400000,   64,     SIZE_64KB,  256,    50,     BITS(6:2, 0x1F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL128P,   MID_SPAN,   0x20,   0x18,   _pstWriteProtectTable_S25FL128P,    NULL,                       0x1000000,  256,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL008A,   MID_SPAN,   0x20,   0x13,   _pstWriteProtectTable_S25FL008A,    NULL,                       0x100000,   16,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_S25FL164K,   MID_SPAN,   0x40,   0x17,   _pstWriteProtectTable_S25FL164K,    NULL,                       0x800000,    128,    SIZE_64KB,  256,    50,     BITS(6:2, 0x1F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_PM25LQ032C,  MID_PMC,    0x46,   0x15,   _pstWriteProtectTable_PM25LQ032C,  NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(5:2, 0x1F),    ISP_DEV_ST,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_GD25Q16,     MID_GD,     0x40,   0x15,   _pstWriteProtectTable_GD25Q16,      NULL,                       0x200000,   32,     SIZE_64KB,  256,    50,     BITS(6:2, 0x1F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
    { FLASH_IC_GD25Q32,     MID_GD,     0x40,   0x16,   _pstWriteProtectTable_GD25Q32_CMP0, NULL,                       0x400000,   64,     SIZE_64KB,  256,    50,     BITS(6:2, 0x1F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, TRUE },
	{ FLASH_IC_EN25Q32,     MID_EON,    0x30,   0x16,    _pstWriteProtectTable_EN25Q32,      NULL,                      0x400000,   32,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_EN25Q16,     MID_EON,    0x30,   0x15,    _pstWriteProtectTable_EN25Q16,      NULL,                      0x200000,   32,     SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MICRON_M25P16,   MID_MICRON, 0x20,   0x15,   _pstWriteProtectTable_M25P16,   NULL,                       0x200000,    32,    SIZE_64KB,  256,    50,     BITS(4:2, 0x07),    ISP_DEV_PMC,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MICRON_N25Q32,   MID_MICRON, 0xBA,   0x16,   _pstWriteProtectTable_N25Q32,   NULL,                       0x400000,    64,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MICRON_N25Q64,   MID_MICRON, 0xBA,   0x17,   _pstWriteProtectTable_N25Q64,   NULL,                       0x800000,    128,   SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_MICRON_N25Q128,  MID_MICRON, 0xBA,   0x18,   _pstWriteProtectTable_N25Q128,   NULL,                       0x1000000,   256,   SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,    ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_ESMT_F25L032QA,  MID_ESMT,   0x41,   0x16,   _pstWriteProtectTable_F25L032QA, NULL,                      SIZE_4MB,    64,    SIZE_64KB,  256,    50,     BITS(5:2, 0x0F),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},
    { FLASH_IC_ESMT_F25L032PA,  MID_ESMT,   0x20,   0x16,   _pstWriteProtectTable_F25L032PA, NULL,                      SIZE_4MB,    64,    SIZE_64KB,  256,    50,     BITS(4:2, 0x07),    ISP_DEV_ST,     ISP_SPI_ENDIAN_LITTLE, FALSE},

};

//
//  Spi  Clk Table (List)
//
static MS_U16 _hal_ckg_spi_pm[] = {
     PM_SPI_CLK_XTALI
    ,PM_SPI_CLK_54MHZ
    ,PM_SPI_CLK_86MHZ
    ,PM_SPI_CLK_108MHZ
};

static MS_U16 _hal_ckg_spi_nonpm[] = {
     CLK0_CKG_SPI_XTALI
    ,CLK0_CKG_SPI_54MHZ
    ,CLK0_CKG_SPI_86MHZ
    ,CLK0_CKG_SPI_108MHZ
};

static hal_isp_t _hal_isp =
{
    .VirtXiuBaseAddr = BASEADDR_XIU,
    .VirtMheg5BaseAddr = BASEADDR_RIU + BK_MHEG5,
    .VirtIspBaseAddr = BASEADDR_RIU + BK_ISP,
    .VirtFspBaseAddr = BASEADDR_RIU + BK_FSP,
    .VirtQspiBaseAddr = BASEADDR_RIU + BK_QSPI,
    .VirtBdmaBaseAddr = BASEADDR_RIU + BK_BDMA,
    .VirtPiuBaseAddr = BASEADDR_RIU + BK_PIU,
    .VirtPMBaseAddr = BASEADDR_RIU + BK_PMSLP,
    .VirtCLK0BaseAddr = BASEADDR_RIU + BK_CLK0,
    .VirtRiuBaseAddr= BASEADDR_RIU,
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_Check51RunMode(void);
static void _HAL_SPI_Rest(void);
static void _HAL_ISP_Enable(void);
static void _HAL_ISP_Disable(void);
static void _HAL_ISP_2XMode(MS_BOOL bEnable);
static MS_BOOL _HAL_SERFLASH_WaitWriteCmdRdy(void);
static MS_BOOL _HAL_SERFLASH_WaitWriteDataRdy(void);
static MS_BOOL _HAL_SERFLASH_WaitReadDataRdy(void);
static MS_BOOL _HAL_SERFLASH_WaitWriteDone(void);
static MS_BOOL _HAL_SERFLASH_CheckWriteDone(void);
static MS_BOOL _HAL_SERFLASH_XIURead(MS_U32 u32Addr,MS_U32 u32Size,MS_U8 * pu8Data);
static MS_BOOL _HAL_SERFLASH_RIURead(MS_U32 u32Addr,MS_U32 u32Size,MS_U8 * pu8Data);
static void _HAL_SERFLASH_ActiveFlash_Set_HW_WP(MS_BOOL bEnable);
static void HAL_SERFLASH_ISP_Client_MASK(void);
static void HAL_SERFLASH_ISP_Client_unMASK(void);
MS_BOOL HAL_FSP_Write_Normal(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data);

static void HAL_SERFLASH_ISP_Client_MASK(void)
{
    QSPI_WRITE(REG_QSPI_MASK_GRANT, 0xFF);
}

static void HAL_SERFLASH_ISP_Client_unMASK(void)
{
    QSPI_WRITE(REG_QSPI_MASK_GRANT, 0x00);
}

//-------------------------------------------------------------------------------------------------
// check if pm51 on SPI
// @return TRUE : succeed
// @return FALSE : fail
// @note :
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_Check51RunMode(void)
{
     MS_U8 u8PM51RunMode;
     u8PM51RunMode = PM_READ(REG_PM_CHK_51MODE);
     if((u8PM51RunMode & PM_51_ON_SPI))
         return FALSE;
     else
         return TRUE;
}

//-------------------------------------------------------------------------------------------------
// Software reset spi_burst
// @return TRUE : succeed
// @return FALSE : fail
// @note : If no spi reset, it may cause BDMA fail.
//-------------------------------------------------------------------------------------------------
static void _HAL_SPI_Rest(void)
{
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_RESET, SFSH_CHIP_RESET_MASK);
    ISP_WRITE_MASK(REG_ISP_CHIP_RST, SFSH_CHIP_NOTRESET, SFSH_CHIP_RESET_MASK);

    // Call the callback function to switch back the chip selection.
    if(McuChipSelectCB != NULL )
    {
        (*McuChipSelectCB)();
    }
}

//-------------------------------------------------------------------------------------------------
// Enable RIU ISP engine
// @return TRUE : succeed
// @return FALSE : fail
// @note : If Enable ISP engine, the XIU mode does not work
//-------------------------------------------------------------------------------------------------
static void _HAL_ISP_Enable(void)
{
    ISP_WRITE(REG_ISP_PASSWORD, 0xAAAA);
}

//-------------------------------------------------------------------------------------------------
// Disable RIU ISP engine
// @return TRUE : succeed
// @return FALSE : fail
// @note : If Disable ISP engine, the XIU mode works
//-------------------------------------------------------------------------------------------------
static void _HAL_ISP_Disable(void)
{
    ISP_WRITE(REG_ISP_PASSWORD, 0x5555);
    _HAL_SPI_Rest();
}

//-------------------------------------------------------------------------------------------------
// Enable/Disable address and data dual mode (SPI command is 0xBB)
// @return TRUE : succeed
// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
static void _HAL_ISP_2XMode(MS_BOOL bEnable)
{
    if(bEnable) // on 2Xmode
    {
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE,SFSH_CHIP_2XREAD_DADD_ENABLE,SFSH_CHIP_2XREAD_MASK);
    }
    else        // off 2Xmode
    {
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE,SFSH_CHIP_2XREAD_DISABLE,SFSH_CHIP_2XREAD_MASK);
    }
}

//-------------------------------------------------------------------------------------------------
// Wait for SPI Write Cmd Ready
// @return TRUE : succeed
// @return FALSE : fail before timeout
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_WaitWriteCmdRdy(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;

#if defined (MSOS_TYPE_LINUX)
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR * 30);
#else
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);
#endif
    do
    {
        if ( (ISP_READ(REG_ISP_SPI_WR_CMDRDY) & ISP_SPI_WR_CMDRDY_MASK) == ISP_SPI_WR_CMDRDY )
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    if (bRet == FALSE)
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("Wait for SPI Write Cmd Ready fails!\n"));
    }

    return bRet;
}


//-------------------------------------------------------------------------------------------------
// Wait for SPI Write Data Ready
// @return TRUE : succeed
// @return FALSE : fail before timeout
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_WaitWriteDataRdy(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;

#if defined (MSOS_TYPE_LINUX)
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*30);
#else
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);
#endif
    do
    {
        if ( (ISP_READ(REG_ISP_SPI_WR_DATARDY) & ISP_SPI_WR_DATARDY_MASK) == ISP_SPI_WR_DATARDY )
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    if (bRet == FALSE)
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("Wait for SPI Write Data Ready fails!\n"));
    }

    return bRet;
}


//-------------------------------------------------------------------------------------------------
// Wait for SPI Read Data Ready
// @return TRUE : succeed
// @return FALSE : fail before timeout
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_WaitReadDataRdy(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;

#if defined (MSOS_TYPE_LINUX)
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*30);
#else
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR);
#endif
    do
    {
        if ( (ISP_READ(REG_ISP_SPI_RD_DATARDY) & ISP_SPI_RD_DATARDY_MASK) == ISP_SPI_RD_DATARDY )
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    if (bRet == FALSE)
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("Wait for SPI Read Data Ready fails!\n"));
    }

    return bRet;
}

//-------------------------------------------------------------------------------------------------
// Wait for Write/Erase to be done
// @return TRUE : succeed
// @return FALSE : fail before timeout
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_WaitWriteDone(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;

#if defined (MSOS_TYPE_LINUX)
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*SERFLASH_MAX_CHIP_WR_DONE_TIMEOUT*1000);
#else
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR*SERFLASH_MAX_CHIP_WR_DONE_TIMEOUT);
#endif

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        return FALSE;
    }

    do
    {

        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            break;
        }

        if ( (ISP_READ(REG_ISP_SPI_RDATA) & SF_SR_WIP_MASK) == 0 ) // WIP = 0 write done
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    if (bRet == FALSE)
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("Wait for Write to be done fails!\n"));
    }

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    return bRet;
}

//-------------------------------------------------------------------------------------------------
// Check Write/Erase to be done
// @return TRUE : succeed
// @return FALSE : fail before timeout
//-------------------------------------------------------------------------------------------------
static MS_BOOL _HAL_SERFLASH_CheckWriteDone(void)
{
    MS_BOOL bRet = FALSE;

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto _HAL_SERFLASH_CheckWriteDone_return;
    }

    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_RDSR); // RDSR

    ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto _HAL_SERFLASH_CheckWriteDone_return;
    }

    if ( (ISP_READ(REG_ISP_SPI_RDATA) & SF_SR_WIP_MASK) == 0 ) // WIP = 0 write done
    {
        bRet = TRUE;
    }

_HAL_SERFLASH_CheckWriteDone_return:

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable flash HW WP
/// @param  bEnable \b IN: enable or disable HW protection
//-------------------------------------------------------------------------------------------------
static void _HAL_SERFLASH_ActiveFlash_Set_HW_WP(MS_BOOL bEnable)
{
    extern void msFlash_ActiveFlash_Set_HW_WP(MS_BOOL bEnable) __attribute__ ((weak));

    if (msFlash_ActiveFlash_Set_HW_WP != NULL)
    {
        msFlash_ActiveFlash_Set_HW_WP(bEnable);
    }
    else
    {
        if(FlashSetHWWPCB != NULL )
        {
            (*FlashSetHWWPCB)(bEnable);
        }

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_NOTICE, printf("msFlash_ActiveFlash_Set_HW_WP() is not defined in this system\n"));
        // ASSERT(msFlash_ActiveFlash_Set_HW_WP != NULL);
    }

    return;
}

#if defined (MCU_AEON)
//Aeon SPI Address is 64K bytes windows
static MS_BOOL _HAL_SetAeon_SPIMappingAddr(MS_U32 u32addr)
{
    MS_U16 u16MHEGAddr = (MS_U16)((_hal_isp.u32XiuBaseAddr + u32addr) >> 16);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, 0x%08X)\n", __FUNCTION__, (int)u32addr, (int)u16MHEGAddr));
    MHEG5_WRITE(REG_SPI_BASE, u16MHEGAddr);

    return TRUE;
}
#endif

static MS_BOOL _HAL_SERFLASH_RIURead(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32I;
    MS_U8 *pu8ReadBuf = pu8Data;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, %d, %p)\n", __FUNCTION__, (int)u32Addr, (int)u32Size, pu8Data));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    do{
        if(_HAL_SERFLASH_WaitWriteDone()) break;
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis
    }while(1);

    ISP_WRITE(REG_ISP_SPI_ADDR_L, LOU16(u32Addr));
    ISP_WRITE(REG_ISP_SPI_ADDR_H, HIU16(u32Addr));

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_Read_return;
    }

    SPI_FLASH_CMD(ISP_SPI_CMD_READ);// READ // 0x0B fast Read : HW doesn't support now

    for ( u32I = 0; u32I < u32Size; u32I++ )
    {
        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_Read_return;
        }

            pu8ReadBuf[ u32I ] = SPI_READ_DATA();
    }
    //--- Flush OCP memory --------
    MsOS_FlushMemory();

    bRet = TRUE;

HAL_SERFLASH_Read_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

#if defined (MCU_AEON)
    //restore default value
    _HAL_SetAeon_SPIMappingAddr(0);
#endif
     MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

static MS_BOOL _HAL_SERFLASH_XIURead(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32I;
    MS_U8 *pu8ReadBuf = pu8Data;
    MS_U32 u32Value, u32AliSize;
    MS_U32 u32AliAddr, u32RemSize = u32Size;
    MS_U32 u32pos;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, %d, %p)\n", __FUNCTION__, (int)u32Addr, (int)u32Size, pu8Data));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    do{
        if(_HAL_SERFLASH_WaitWriteDone()) break;
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis
    }while(1);

    _HAL_ISP_Disable();

    // 4-BYTE Aligment for 32 bit CPU Aligment
    u32AliAddr = (u32Addr & 0xFFFFFFFC);
    u32pos = u32AliAddr >> 2;

#if defined (MCU_AEON)
    //write SPI mapping address
    _HAL_SetAeon_SPIMappingAddr(u32AliAddr);
#endif

    //---- Read first data for not aligment address ------
    if(u32AliAddr < u32Addr)
    {
        u32Value = SFSH_XIU_READ32(u32pos);
        u32pos++;
        for(u32I = 0; (u32I < 4) && (u32RemSize > 0); u32I++)
        {
            if(u32AliAddr >= u32Addr)
            {
                *pu8ReadBuf++ = (MS_U8)(u32Value & 0xFF);
                u32RemSize--;
            }
            u32Value >>= 8;
            u32AliAddr++;
        }
    }
    //----Read datum for aligment address------
    u32AliSize = (u32RemSize & 0xFFFFFFFC);
    for( u32I = 0; u32I < u32AliSize; u32I += 4)
    {
#if defined (MCU_AEON)
            if((u32AliAddr & 0xFFFF) == 0)
                _HAL_SetAeon_SPIMappingAddr(u32AliAddr);
#endif

            // only indirect mode
            u32Value = SFSH_XIU_READ32(u32pos);

            *pu8ReadBuf++ = ( u32Value >> 0) & 0xFF;
            *pu8ReadBuf++ = ( u32Value >> 8) & 0xFF;
            *pu8ReadBuf++ = ( u32Value >> 16)& 0xFF;
            *pu8ReadBuf++ = ( u32Value >> 24)& 0xFF;

            u32pos++;
            u32AliAddr += 4;
        }

    //--- Read remain datum --------
    if(u32RemSize > u32AliSize)
    {
#if defined (MCU_AEON)
            if((u32AliAddr & 0xFFFF) == 0)
                _HAL_SetAeon_SPIMappingAddr(u32AliAddr);
#endif
            u32Value = SFSH_XIU_READ32(u32pos);
        }
        while(u32RemSize > u32AliSize)
        {
            *pu8ReadBuf++ = (u32Value & 0xFF);
            u32Value >>= 8;
            u32AliSize++;
        }
    //--- Flush OCP memory --------
    MsOS_FlushMemory();


        bRet = TRUE;

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

#if defined (MCU_AEON)
    //restore default value
    _HAL_SetAeon_SPIMappingAddr(0);
#endif
     MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SERFLASH_SetCKG()
/// @brief \b Function \b Description: This function is used to set ckg_spi dynamically
/// @param <IN>        \b eCkgSpi    : enumerate the ckg_spi
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
/// @param <NOTE>    \b : Please use this function carefully , and is restricted to Flash ability
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SERFLASH_SetCKG(SPI_DrvCKG eCkgSpi)
{
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    // NON-PM Doman
    CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CLK_SWITCH_OFF,CLK0_CLK_SWITCH_MASK);      // run @ 12M

    switch (eCkgSpi)
    {
        case E_SPI_XTALI:
            CLK_WRITE_MASK(REG_CLK0_CKG_SPI,_hal_ckg_spi_nonpm[0],CLK0_CKG_SPI_MASK); // set ckg_spi
            break;

        case E_SPI_54M:
            CLK_WRITE_MASK(REG_CLK0_CKG_SPI,_hal_ckg_spi_nonpm[1],CLK0_CKG_SPI_MASK); // set ckg_spi
            break;

        case E_SPI_86M:
            CLK_WRITE_MASK(REG_CLK0_CKG_SPI,_hal_ckg_spi_nonpm[2],CLK0_CKG_SPI_MASK); // set ckg_spi
            break;

        case E_SPI_108M:
            CLK_WRITE_MASK(REG_CLK0_CKG_SPI,_hal_ckg_spi_nonpm[3],CLK0_CKG_SPI_MASK); // set ckg_spi
            break;

        default:
            CLK_WRITE_MASK(REG_CLK0_CKG_SPI,_hal_ckg_spi_nonpm[1],CLK0_CKG_SPI_MASK); // set ckg_spi
            break;
    }

    CLK_WRITE_MASK(REG_CLK0_CKG_SPI,CLK0_CLK_SWITCH_ON,CLK0_CLK_SWITCH_MASK);       // run @ ckg_spi
    // PM Doman
    PM_WRITE_MASK(REG_PM_CKG_SPI,PM_SPI_CLK_SWITCH_OFF,PM_SPI_CLK_SWITCH_MASK); // run @ 12M
    switch (eCkgSpi)
    {
        case E_SPI_XTALI:
            PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[0],PM_SPI_CLK_SEL_MASK); // set ckg_spi
            break;

        case E_SPI_54M:
            PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[1],PM_SPI_CLK_SEL_MASK); // set ckg_spi
            break;

        case E_SPI_86M:
            PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[2],PM_SPI_CLK_SEL_MASK); // set ckg_spi
            break;

        case E_SPI_108M:
            PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[3],PM_SPI_CLK_SEL_MASK); // set ckg_spi
            break;

        default:
            PM_WRITE_MASK(REG_PM_CKG_SPI,_hal_ckg_spi_pm[1],PM_SPI_CLK_SEL_MASK); // set ckg_spi
            break;
    }
    PM_WRITE_MASK(REG_PM_CKG_SPI,PM_SPI_CLK_SWITCH_ON,PM_SPI_CLK_SWITCH_MASK);  // run @ ckg_spi
    Ret = TRUE;
    return Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SERFLASH_ClkDiv()
/// @brief \b Function \b Description: This function is used to set clock div dynamically
/// @param <IN>        \b eCkgSpi    : enumerate the clk_div
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
/// @param <NOTE>    \b : Please use this function carefully , and is restricted to Flash ability
////////////////////////////////////////////////////////////////////////////////
void HAL_SERFLASH_ClkDiv(SPI_DrvClkDiv eClkDivSpi)
{
    switch (eClkDivSpi)
    {
        case E_SPI_DIV2:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV2);
            break;
        case E_SPI_DIV4:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV4);
            break;
        case E_SPI_DIV8:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV8);
            break;
        case E_SPI_DIV16:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV16);
            break;
        case E_SPI_DIV32:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV32);
            break;
        case E_SPI_DIV64:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV64);
            break;
        case E_SPI_DIV128:
            ISP_WRITE(REG_ISP_SPI_CLKDIV,ISP_SPI_CLKDIV128);
            break;
        case E_SPI_ClkDiv_NOT_SUPPORT:
       default:
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SERFLASH_SetMode()
/// @brief \b Function \b Description: This function is used to set RIU/XIU dynamically
/// @param <IN>        \b bXiuRiu    : Enable for XIU (Default) Disable for RIU(Optional)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
/// @param <NOTE>    \b : XIU is faster than RIU, but is sensitive to ckg.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SERFLASH_SetMode(MS_BOOL bXiuRiu)
{
    MS_BOOL Ret = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));
    _bXIUMode = bXiuRiu;
    Ret = TRUE;
    return Ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SERFLASH_Set2XREAD()
/// @brief \b Function \b Description: This function is used to set 2XREAD dynamically
/// @param <IN>        \b b2XMode    : ENABLE for 2XREAD DISABLE for NORMAL
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
/// @param <NOTE>    \b : Please use this function carefully, and needs Flash support
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SERFLASH_Set2XREAD(MS_BOOL b2XMode)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    if(!bDetect)
    {
        HAL_SERFLASH_DetectType();
    }
    MS_ASSERT(_hal_SERFLASH.b2XREAD); // check hw support or not
    if(_hal_SERFLASH.b2XREAD)
    {
        _HAL_ISP_2XMode(b2XMode);
    }
    else
    {
        UNUSED(b2XMode);
        printf("%s This flash does not support 2XREAD!!!\n", __FUNCTION__);
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SERFLASH_ChipSelect()
/// @brief \b Function \b Description: set active flash among multi-spi flashes
/// @param <IN>        \b u8FlashIndex : flash index (0 or 1)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SERFLASH_ChipSelect(MS_U8 u8FlashIndex)
{
    MS_BOOL Ret = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X)\n", __FUNCTION__, (int)u8FlashIndex));
    switch (u8FlashIndex)
    {
        case FLASH_ID0:
            ISP_WRITE_MASK(REG_ISP_SPI_CHIP_SELE,SFSH_CHIP_SELE_EXT1,SFSH_CHIP_SELE_MASK);
            break;
        case FLASH_ID1:
            ISP_WRITE_MASK(REG_ISP_SPI_CHIP_SELE,SFSH_CHIP_SELE_EXT2,SFSH_CHIP_SELE_MASK);
            break;
        case FLASH_ID2:
            ISP_WRITE_MASK(REG_ISP_SPI_CHIP_SELE,SFSH_CHIP_SELE_EXT3,SFSH_CHIP_SELE_MASK);
            break;
        case FLASH_ID3:
            UNUSED(u8FlashIndex); //Reserved
            break;
        default:
            UNUSED(u8FlashIndex); //Invalid flash ID
            return FALSE;
    }
    WAIT_SFSH_CS_STAT(); // wait for chip select done
    return Ret = TRUE;
}

void HAL_SERFLASH_Config(MS_VIRT u32PMRegBaseAddr, MS_VIRT u32NonPMRegBaseAddr, MS_VIRT u32XiuBaseAddr)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, 0x%08X, 0x%08X)\n", __FUNCTION__, (int)u32PMRegBaseAddr, (int)u32NonPMRegBaseAddr, (int)u32XiuBaseAddr));
    _hal_isp.VirtXiuBaseAddr   = u32XiuBaseAddr;
    _hal_isp.VirtMheg5BaseAddr = u32NonPMRegBaseAddr + BK_MHEG5;
    _hal_isp.VirtIspBaseAddr   = u32PMRegBaseAddr + BK_ISP;
    _hal_isp.VirtFspBaseAddr   = u32PMRegBaseAddr + BK_FSP;
    _hal_isp.VirtQspiBaseAddr  = u32PMRegBaseAddr + BK_QSPI;
    _hal_isp.VirtBdmaBaseAddr  = u32NonPMRegBaseAddr + BK_BDMA,
    _hal_isp.VirtPiuBaseAddr   = u32PMRegBaseAddr + BK_PIU;
    _hal_isp.VirtPMBaseAddr    = u32PMRegBaseAddr + BK_PMSLP;
    _hal_isp.VirtCLK0BaseAddr  = u32NonPMRegBaseAddr + BK_CLK0;
    _hal_isp.VirtRiuBaseAddr   = u32PMRegBaseAddr;
}


void HAL_SERFLASH_Init(void)
{
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    _s32SERFLASH_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SERFLASH", MSOS_PROCESS_SHARED);
    MS_ASSERT(_s32SERFLASH_Mutex >= 0);

#ifdef MCU_AEON
    ISP_WRITE(REG_ISP_SPI_CLKDIV, 1<<2); // cpu clock / div4
#else// MCU_MIPS @ 720 Mhz for T8
    HAL_SERFLASH_ClkDiv(E_SPI_DIV8);
#endif

    ISP_WRITE(REG_ISP_SPI_ENDIAN, ISP_SPI_ENDIAN_SEL);
}

void HAL_SERFLASH_SetGPIO(MS_BOOL bSwitch)
{
    MS_VIRT u32PmGPIObase = _hal_isp.VirtPMBaseAddr + 0x200;

    if(bSwitch)// The PAD of the SPI set as GPIO IN.
    {
        PM_WRITE_MASK(REG_PM_SPI_IS_GPIO, PM_SPI_IS_GPIO, PM_SPI_GPIO_MASK);
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPICZ_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPICZ_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPICK_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPICK_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPIDI_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPIDI_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPIDO_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPIDO_OEN)<<2))|(BIT(0))));
    }
    else
    {
        PM_WRITE_MASK(REG_PM_SPI_IS_GPIO, PM_SPI_NOT_GPIO, PM_SPI_GPIO_MASK);
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPICZ_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPICZ_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPICK_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPICK_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPIDI_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPIDI_OEN)<<2))|(BIT(0))));
        WRITE_BYTE( u32PmGPIObase + ((REG_PM_GPIO_SPIDO_OEN)<<2), (READ_BYTE(u32PmGPIObase + ((REG_PM_GPIO_SPIDO_OEN)<<2))|(BIT(0))));
    }
}

MS_BOOL HAL_SERFLASH_DetectType(void)
{
    #define READ_ID_SIZE    3
    #define READ_REMS4_SIZE 2

    MS_U8   u8FlashId[READ_ID_SIZE];
    MS_U8   u8FlashREMS4[READ_REMS4_SIZE];
    MS_U32  u32Index;
    MS_U8   u8Status0, u8Status1;

    memset(&_hal_SERFLASH, 0, sizeof(_hal_SERFLASH));

    // If use MXIC MX25L6445E
    HAL_SERFLASH_ReadREMS4(u8FlashREMS4,READ_REMS4_SIZE);

    if (HAL_SERFLASH_ReadID(u8FlashId, sizeof(u8FlashId))== TRUE)
    {
        /* find current serial flash */
        for (u32Index = 0; _hal_SERFLASH_table[u32Index].u8MID != 0; u32Index++)
        {

            if (   (_hal_SERFLASH_table[u32Index].u8MID  == u8FlashId[0])
                && (_hal_SERFLASH_table[u32Index].u8DID0 == u8FlashId[1])
                && (_hal_SERFLASH_table[u32Index].u8DID1 == u8FlashId[2])
                )
            {
                memcpy(&_hal_SERFLASH, &(_hal_SERFLASH_table[u32Index]), sizeof(_hal_SERFLASH));

                // patch : MXIC 6405D vs 6445E(MXIC 12805D vs 12845E)
                if( u8FlashREMS4[0] == 0xC2)
                {
                    if( u8FlashREMS4[1] == 0x16)
                    {
                        _hal_SERFLASH.u16FlashType = FLASH_IC_MX25L6445E;
                        _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_MX25L6445E;
                    }
                    if( u8FlashREMS4[1] == 0x17)
                    {
                        _hal_SERFLASH.u16FlashType = FLASH_IC_MX25L12845E;
                        _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_MX25L12845E;
                    }
                }

                DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO,
                                printf("Flash is detected (0x%04X, 0x%02X, 0x%02X, 0x%02X)\n",
                                       _hal_SERFLASH.u16FlashType,
                                       _hal_SERFLASH.u8MID,
                                       _hal_SERFLASH.u8DID0,
                                       _hal_SERFLASH.u8DID1
                                       )
                                );
                bDetect = TRUE;
                break;
            }
            else
            {
                continue;
            }
        }

       // printf("[%x]\n",_hal_SERFLASH.u16FlashType);
        // customization for GigaDevice
        if( _hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32 )
        {
            HAL_SERFLASH_ReadStatusReg(&u8Status0);
            HAL_SERFLASH_ReadStatusReg2(&u8Status1);
            HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1
            //_hal_SERFLASH.u8WrsrBlkProtect = BITS(6:2, 0x00);
            _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_GD25Q32_CMP1;
        }

        if( _hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K )
        {
            HAL_SERFLASH_ReadStatusReg(&u8Status0);
            HAL_SERFLASH_ReadStatusReg2(&u8Status1);
            HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1
           // _hal_SERFLASH.u8WrsrBlkProtect = BITS(6:2, 0x00);
            _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_S25FL032K_CMP1;
        }

        if(_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV)
        {
            HAL_SERFLASH_ReadStatusReg(&u8Status0);
            HAL_SERFLASH_ReadStatusReg2(&u8Status1);
            HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1

            //_hal_SERFLASH.u8WrsrBlkProtect = BITS(6:2, 0x00);
            _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_W25Q64CV_CMP1;
        }

        if( _hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV)
        {
            HAL_SERFLASH_ReadStatusReg(&u8Status0);
            HAL_SERFLASH_ReadStatusReg2(&u8Status1);
            HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1

            //_hal_SERFLASH.u8WrsrBlkProtect = BITS(6:2, 0x00);
            _hal_SERFLASH.pWriteProtectTable = _pstWriteProtectTable_W25Q32BV_CMP1;
        }
        MsOS_DelayTask(40);
        // If the Board uses a unknown flash type, force setting a secure flash type for booting. //FLASH_IC_MX25L6405D
        if( bDetect != TRUE )
        {
            #if 0
            memcpy(&_hal_SERFLASH, &(_hal_SERFLASH_table[2]), sizeof(_hal_SERFLASH));

            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO,
                            printf("Unknown flash type (0x%02X, 0x%02X, 0x%02X) and use default flash type 0x%04X\n",
                                   _hal_SERFLASH.u8MID,
                                   _hal_SERFLASH.u8DID0,
                                   _hal_SERFLASH.u8DID1,
                                   _hal_SERFLASH.u16FlashType
                                   )
                            );
            #endif
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_INFO,
                            printf("Unsupport flash type (0x%02X, 0x%02X, 0x%02X), please add flash info to serial flash driver\n",
                                   u8FlashId[0],
                                   u8FlashId[1],
                                   u8FlashId[2]
                                   )
                            );
            MS_ASSERT(0);
            bDetect = TRUE;
        }

    }
    ISP_WRITE(REG_ISP_DEV_SEL, ISP_DEV_SEL);
    return bDetect;

}

MS_BOOL HAL_SERFLASH_DetectSize(MS_U32  *u32FlashSize)
{
    MS_BOOL Ret = FALSE;

    do{

        *u32FlashSize = _hal_SERFLASH.u32FlashSize;
        Ret = TRUE;

    }while(0);

    return Ret;
}

MS_BOOL HAL_SERFLASH_EraseChip(void)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_EraseChip_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_EraseChip_return;
    }
    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN


    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_EraseChip_return;
    }
    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_CE); // CHIP_ERASE

    bRet = _HAL_SERFLASH_WaitWriteDone();

HAL_SERFLASH_EraseChip_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}


MS_BOOL HAL_SERFLASH_AddressToBlock(MS_U32 u32FlashAddr, MS_U32 *pu32BlockIndex)
{
    MS_U32  u32NextAddr;
    MS_BOOL bRet = FALSE;

    if (_hal_SERFLASH.pSpecialBlocks == NULL)
    {
        *pu32BlockIndex = u32FlashAddr / SERFLASH_SECTOR_SIZE;

        bRet = TRUE;
    }
    else
    {
        // TODO: review, optimize this flow
        for (u32NextAddr = 0, *pu32BlockIndex = 0; *pu32BlockIndex < NUMBER_OF_SERFLASH_SECTORS; (*pu32BlockIndex)++)
        {
            // outside the special block
            if (   *pu32BlockIndex < _hal_SERFLASH.pSpecialBlocks->u16Start
                || *pu32BlockIndex > _hal_SERFLASH.pSpecialBlocks->u16End
                )
            {
                u32NextAddr += SERFLASH_SECTOR_SIZE; // i.e. normal block size
            }
            // inside the special block
            else
            {
                u32NextAddr += _hal_SERFLASH.pSpecialBlocks->au32SizeList[*pu32BlockIndex - _hal_SERFLASH.pSpecialBlocks->u16Start];
            }

            if (u32NextAddr > u32FlashAddr)
            {
                bRet = TRUE;
                break;
            }
        }
    }

    return bRet;
}


MS_BOOL HAL_SERFLASH_BlockToAddress(MS_U32 u32BlockIndex, MS_U32 *pu32FlashAddr)
{
    if (   _hal_SERFLASH.pSpecialBlocks == NULL
        || u32BlockIndex <= _hal_SERFLASH.pSpecialBlocks->u16Start
        )
    {
        *pu32FlashAddr = u32BlockIndex * SERFLASH_SECTOR_SIZE;
    }
    else
    {
        MS_U32 u32Index;

        *pu32FlashAddr = _hal_SERFLASH.pSpecialBlocks->u16Start * SERFLASH_SECTOR_SIZE;

        for (u32Index = _hal_SERFLASH.pSpecialBlocks->u16Start;
             u32Index < u32BlockIndex && u32Index <= _hal_SERFLASH.pSpecialBlocks->u16End;
             u32Index++
             )
        {
            *pu32FlashAddr += _hal_SERFLASH.pSpecialBlocks->au32SizeList[u32Index - _hal_SERFLASH.pSpecialBlocks->u16Start];
        }

        if (u32BlockIndex > _hal_SERFLASH.pSpecialBlocks->u16End + 1)
        {
            *pu32FlashAddr += (u32BlockIndex - _hal_SERFLASH.pSpecialBlocks->u16End - 1) * SERFLASH_SECTOR_SIZE;
        }
    }

    return TRUE;
}

MS_BOOL HAL_SERFLASH_BlockErase(MS_U32 u32StartBlock, MS_U32 u32EndBlock, MS_BOOL bWait)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32I;
    MS_U32 u32FlashAddr = 0;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, 0x%08X, %d)\n", __FUNCTION__, (int)u32StartBlock, (int)u32EndBlock, bWait));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());
    HAL_SERFLASH_ISP_Client_MASK();

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    if( u32StartBlock > u32EndBlock || u32EndBlock >= _hal_SERFLASH.u32NumSec )
    {
        printf("%s (0x%08X, 0x%08X, %d)\n", __FUNCTION__, (int)u32StartBlock, (int)u32EndBlock, bWait);
        goto HAL_SERFLASH_BlockErase_return;
    }

    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_BlockErase_return;
    }

    for( u32I = u32StartBlock; u32I <= u32EndBlock; u32I++)
    {
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }
        SPI_FLASH_CMD(ISP_SPI_CMD_WREN);    // WREN

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode

        ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_64BE); // BLOCK_ERASE

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        if (HAL_SERFLASH_BlockToAddress(u32I, &u32FlashAddr) == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32FlashAddr) & 0xFF);

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) >> 8);

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) & 0xFF);

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_BlockErase_return;
        }

        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x5555); // disable trigger mode

        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        if(bWait == TRUE )
        {
            if(!_HAL_SERFLASH_WaitWriteDone())
            {
                printf("%s : Wait Write Done Fail!!!\n", __FUNCTION__ );
                bRet = FALSE;
            }
            else
            {
                bRet = TRUE;
            }
        }
        else
        {
            bRet = TRUE;
        }
    }

HAL_SERFLASH_BlockErase_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
    HAL_SERFLASH_ISP_Client_unMASK();

    return bRet;
}

MS_BOOL HAL_SERFLASH_SectorErase(MS_U32 u32SectorAddress)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X)\n", __FUNCTION__, (int)u32SectorAddress));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());
    HAL_SERFLASH_ISP_Client_MASK();

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s ENTRY fails!\n", __FUNCTION__));
        return bRet;
    }

    if( u32SectorAddress > _hal_SERFLASH.u32FlashSize )
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s (0x%08X)\n", __FUNCTION__, (int)u32SectorAddress));
        goto HAL_SERFLASH_BlockErase_return;
    }

    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_BlockErase_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    SPI_FLASH_CMD(ISP_SPI_CMD_WREN);    // WREN

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_SE);

    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32SectorAddress) & 0xFF);

    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32SectorAddress) >> 8);

    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32SectorAddress) & 0xFF);

    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
            goto HAL_SERFLASH_BlockErase_return;
    }

    bRet = TRUE;

HAL_SERFLASH_BlockErase_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x5555);     // disable trigger mode

    _HAL_ISP_Disable();
    HAL_SERFLASH_ISP_Client_unMASK();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}


MS_BOOL HAL_SERFLASH_CheckWriteDone(void)
{
    MS_BOOL bRet = FALSE;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR);    // SPI CEB dis

    bRet = _HAL_SERFLASH_CheckWriteDone();

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR);    // SPI CEB dis

    _HAL_ISP_Disable();

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s() = %d\n", __FUNCTION__, bRet));

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

MS_BOOL HAL_SERFLASH_Write(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = FALSE;
    MS_U16 u16I, u16Rem, u16WriteBytes;
    MS_U8 *u8Buf = pu8Data;
    MS_BOOL b2XREAD = FALSE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, %d, %p)\n", __FUNCTION__, (int)u32Addr, (int)u32Size, pu8Data));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());
    HAL_SERFLASH_ISP_Client_MASK();
    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    b2XREAD = (BIT(2) & ISP_READ(REG_ISP_SPI_MODE))? 1 : 0;
    _HAL_ISP_2XMode(DISABLE);
    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_Write_return;
    }

    u16Rem = u32Addr % SERFLASH_PAGE_SIZE;

    if (u16Rem)
    {
        u16WriteBytes = SERFLASH_PAGE_SIZE - u16Rem;
        if (u32Size < u16WriteBytes)
        {
            u16WriteBytes = u32Size;
        }

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_Write_return;
        }

        SPI_FLASH_CMD(ISP_SPI_CMD_WREN);

        ISP_WRITE(REG_ISP_SPI_ADDR_L, LOU16(u32Addr));
        ISP_WRITE(REG_ISP_SPI_ADDR_H, (MS_U8)HIU16(u32Addr));

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_Write_return;
        }

        SPI_FLASH_CMD(ISP_SPI_CMD_PP);  // PAGE_PROG

        for ( u16I = 0; u16I < u16WriteBytes; u16I++ )
        {
            SPI_WRITE_DATA( *(u8Buf + u16I) );

            if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
            {
                goto HAL_SERFLASH_Write_return;
            }
        }

        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        bRet = _HAL_SERFLASH_WaitWriteDone();

        if ( bRet == TRUE )
        {
            u32Addr += u16WriteBytes;
            u8Buf   += u16WriteBytes;
            u32Size -= u16WriteBytes;
        }
        else
        {
            goto HAL_SERFLASH_Write_return;
        }
    }

    while(u32Size)
    {
        if( u32Size > SERFLASH_PAGE_SIZE)
        {
            u16WriteBytes = SERFLASH_PAGE_SIZE;  //write SERFLASH_PAGE_SIZE bytes one time
        }
        else
        {
            u16WriteBytes = u32Size;
        }

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_Write_return;
        }

        SPI_FLASH_CMD(ISP_SPI_CMD_WREN);    // WREN

        ISP_WRITE(REG_ISP_SPI_ADDR_L, LOU16(u32Addr));
        ISP_WRITE(REG_ISP_SPI_ADDR_H, (MS_U8)HIU16(u32Addr));

        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SERFLASH_Write_return;
        }
        SPI_FLASH_CMD(ISP_SPI_CMD_PP);  // PAGE_PROG

        // Improve flash write speed
        if(u16WriteBytes == 256)
        {
            // Write 256 bytes to flash
            MS_U8 u8Index = 0;

            do{

                SPI_WRITE_DATA( *(u8Buf + u8Index) );

                u8Index++;

                if( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
                {
                    goto HAL_SERFLASH_Write_return;
                }

            }while(u8Index != 0);
        }
        else
        {

            for ( u16I = 0; u16I < u16WriteBytes; u16I++ )
            {
                SPI_WRITE_DATA( *(u8Buf + u16I) );

                if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
                {
                    goto HAL_SERFLASH_Write_return;
                }
            }
        }

        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        bRet = _HAL_SERFLASH_WaitWriteDone();

        if ( bRet == TRUE )
        {
            u32Addr += u16WriteBytes;
            u8Buf   += u16WriteBytes;
            u32Size -= u16WriteBytes;
        }
        else
        {
            goto HAL_SERFLASH_Write_return;
        }
    }


HAL_SERFLASH_Write_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

    //  restore the 2x READ setting.
    _HAL_ISP_2XMode(b2XREAD);

    HAL_SERFLASH_ISP_Client_unMASK();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

MS_BOOL HAL_SERFLASH_Read(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL Ret = FALSE;

    if( _bXIUMode )
    {
        Ret = _HAL_SERFLASH_XIURead( u32Addr, u32Size, pu8Data);
    }
    else// RIU mode
    {
        Ret = _HAL_SERFLASH_RIURead( u32Addr, u32Size, pu8Data);
    }

    return Ret;
}

EN_WP_AREA_EXISTED_RTN HAL_SERFLASH_WP_Area_Existed(MS_U32 u32UpperBound, MS_U32 u32LowerBound, MS_U8 *pu8BlockProtectBits)
{
    ST_WRITE_PROTECT   *pWriteProtectTable;
    MS_U8               u8Index;
    MS_BOOL             bPartialBoundFitted;
    MS_BOOL             bEndOfTable;
    MS_U32              u32PartialFittedLowerBound = u32UpperBound;
    MS_U32              u32PartialFittedUpperBound = u32LowerBound;


    if (NULL == _hal_SERFLASH.pWriteProtectTable)
    {
        return WP_TABLE_NOT_SUPPORT;
    }


    for (u8Index = 0, bEndOfTable = FALSE, bPartialBoundFitted = FALSE; FALSE == bEndOfTable; u8Index++)
    {
        pWriteProtectTable = &(_hal_SERFLASH.pWriteProtectTable[u8Index]);

        if (   0xFFFFFFFF == pWriteProtectTable->u32LowerBound
            && 0xFFFFFFFF == pWriteProtectTable->u32UpperBound
            )
        {
            bEndOfTable = TRUE;
        }

        if (   pWriteProtectTable->u32LowerBound == u32LowerBound
            && pWriteProtectTable->u32UpperBound == u32UpperBound
            )
        {
            *pu8BlockProtectBits = pWriteProtectTable->u8BlockProtectBits;

            return WP_AREA_EXACTLY_AVAILABLE;
        }
        else if (u32LowerBound <= pWriteProtectTable->u32LowerBound && pWriteProtectTable->u32UpperBound <= u32UpperBound)
        {
            //
            // u32PartialFittedUpperBound & u32PartialFittedLowerBound would be initialized first time when bPartialBoundFitted == FALSE (init value)
            // 1. first match:  FALSE == bPartialBoundFitted
            // 2. better match: (pWriteProtectTable->u32UpperBound - pWriteProtectTable->u32LowerBound) > (u32PartialFittedUpperBound - u32PartialFittedLowerBound)
            //

            if (   FALSE == bPartialBoundFitted
                || (pWriteProtectTable->u32UpperBound - pWriteProtectTable->u32LowerBound) > (u32PartialFittedUpperBound - u32PartialFittedLowerBound)
                )
            {
                u32PartialFittedUpperBound = pWriteProtectTable->u32UpperBound;
                u32PartialFittedLowerBound = pWriteProtectTable->u32LowerBound;
                *pu8BlockProtectBits = pWriteProtectTable->u8BlockProtectBits;
            }

            bPartialBoundFitted = TRUE;
        }
    }

    if (TRUE == bPartialBoundFitted)
    {
        return WP_AREA_PARTIALLY_AVAILABLE;
    }
    else
    {
        return WP_AREA_NOT_AVAILABLE;
    }
}

MS_BOOL HAL_SERFLASH_WriteProtect_Area(MS_BOOL bEnableAllArea, MS_U8 u8BlockProtectBits)
{
    MS_BOOL bRet = FALSE;
    MS_U8   u8Status0, u8Status1;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(%d, 0x%02X)\n", __FUNCTION__, bEnableAllArea, u8BlockProtectBits));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());
    HAL_SERFLASH_ISP_Client_MASK();

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_SERFLASH_ActiveFlash_Set_HW_WP(DISABLE);
    MsOS_DelayTask(bEnableAllArea ? 5 : 20); // when disable WP, delay more time

    _HAL_ISP_Enable();

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_Flash_WriteProtect_Area_return;
    }

    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_Flash_WriteProtect_Area_return;
    }

    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR

    if (TRUE == bEnableAllArea)
    {
        if (_hal_SERFLASH.u8MID == MID_ATMEL)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, SERFLASH_WRSR_BLK_PROTECT); // SPRL 1 -> 0

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_Flash_WriteProtect_Area_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_Flash_WriteProtect_Area_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, SF_SR_SRWD | SERFLASH_WRSR_BLK_PROTECT); // SF_SR_SRWD: SRWD Status Register Write Protect
    }
    else
    {
        if (_hal_SERFLASH.u8MID == MID_ATMEL)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, u8BlockProtectBits); // [4:2] or [5:2] protect blocks // SPRL 1 -> 0

            // programming sector protection
            {
                int i;
                MS_U32 u32FlashAddr;

                // search write protect table
                for (i = 0;
                     0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32LowerBound && 0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32UpperBound; // the end of write protect table
                     i++
                     )
                {
                    // if found, write
                    if (u8BlockProtectBits == _hal_SERFLASH.pWriteProtectTable[i].u8BlockProtectBits)
                    {
                        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("u8BlockProtectBits = 0x%X, u32LowerBound = 0x%X, u32UpperBound = 0x%X\n",
                                                                       (unsigned int)u8BlockProtectBits,
                                                                       (unsigned int)_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound,
                                                                       (unsigned int)_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound
                                                                       )
                                        );
                        for (u32FlashAddr = 0; u32FlashAddr < _hal_SERFLASH.u32FlashSize; u32FlashAddr += _hal_SERFLASH.u32SecSize)
                        {
                            if (_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound <= (u32FlashAddr + _hal_SERFLASH.u32SecSize - 1) &&
                                u32FlashAddr <= _hal_SERFLASH.pWriteProtectTable[i].u32UpperBound)
                            {
                                continue;
                            }

                            ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

                            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
                            {
                                goto HAL_Flash_WriteProtect_Area_return;
                            }

                            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

                            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
                            {
                                goto HAL_Flash_WriteProtect_Area_return;
                            }

                            ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode

                            ISP_WRITE(REG_ISP_SPI_WDATA, 0x39); // unprotect sector

                            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
                            {
                                goto HAL_Flash_WriteProtect_Area_return;
                            }

                            ISP_WRITE(REG_ISP_SPI_WDATA, (u32FlashAddr >> 16) & 0xFF);

                            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
                            {
                                goto HAL_Flash_WriteProtect_Area_return;
                            }

                            ISP_WRITE(REG_ISP_SPI_WDATA, ((MS_U16)u32FlashAddr) >> 8);

                            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
                                goto HAL_Flash_WriteProtect_Area_return;
                            }

                            ISP_WRITE(REG_ISP_SPI_WDATA, u32FlashAddr & 0xFF);

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_Flash_WriteProtect_Area_return;
            }

                            ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222); // disable trigger mode

            ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

                            bRet = _HAL_SERFLASH_WaitWriteDone();
                        }
                        break;
                    }
                }
            }

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_Flash_WriteProtect_Area_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_Flash_WriteProtect_Area_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, SF_SR_SRWD | u8BlockProtectBits); // [4:2] or [5:2] protect blocks
    }
    HAL_SERFLASH_ISP_Client_unMASK();

    bRet = _HAL_SERFLASH_WaitWriteDone();

HAL_Flash_WriteProtect_Area_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

   if ((_hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32) || (_hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K)
          || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV) || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV))//MSB S8-S15
    {
        HAL_SERFLASH_ReadStatusReg(&u8Status0);
        HAL_SERFLASH_ReadStatusReg2(&u8Status1);
        HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1
        MsOS_DelayTask(40);
    }

    if (bEnableAllArea)// _REVIEW_
    {
        _HAL_SERFLASH_ActiveFlash_Set_HW_WP(bEnableAllArea);
    }

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/*spasion flash DYB unlock*/

MS_BOOL HAL_SERFLASH_SPSNDYB_UNLOCK(MS_U32 u32StartBlock, MS_U32 u32EndBlock)
{
//u32StartBlock, u32EndBlock --> 0~285
  MS_BOOL bRet = FALSE;
  MS_U32 u32I;
  MS_U32 u32FlashAddr = 0;
  MS_U8 dybStatusReg=0;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("%s ENTRY \n", __FUNCTION__));
    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_SERFLASH_ActiveFlash_Set_HW_WP(DISABLE);
    MsOS_DelayTask(20); // when disable WP, delay more time
    _HAL_ISP_Enable();
    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SPSNFLASH_DYB_UNLOCK_return;
    }

    for( u32I = u32StartBlock; u32I <= u32EndBlock; u32I++)
    {
        bRet = FALSE;
        dybStatusReg=0;
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis
        ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN
        if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode
        ISP_WRITE(REG_ISP_SPI_WDATA, 0xE1); // DYB write
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        if(u32I>=0 && u32I<=31)
            u32FlashAddr = u32I * 4096;
        else 
            u32FlashAddr = (u32I -30) * 64 * 1024;
        ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32FlashAddr)>>8); //MSB , 4th byte
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32FlashAddr) & 0xFF);//3th byte

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) >> 8);//2th byte

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) & 0xFF);//1th byte

        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, 0xFF);//0xFF --> unlock;  0x00-->lock
        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x5555); // disable trigger mode
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis
        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); 
        //read DYB status register to check the result
        ISP_WRITE(REG_ISP_SPI_WDATA, 0xE0); // DYB read
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32FlashAddr)>>8); //MSB , 4th byte
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, HIU16(u32FlashAddr) & 0xFF);//3th byte
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) >> 8);//2th byte
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, LOU16(u32FlashAddr) & 0xFF);//1th byte
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }

        dybStatusReg = (MS_U8)SPI_READ_DATA();
        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x5555); // disable trigger mode
        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis	
        if(dybStatusReg != 0xFF )
        {
            bRet = FALSE;
            goto HAL_SPSNFLASH_DYB_UNLOCK_return;
        }
        else
            bRet = TRUE;
    }

HAL_SPSNFLASH_DYB_UNLOCK_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis
    _HAL_ISP_Disable();
    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
    return bRet;
}

MS_BOOL HAL_SERFLASH_WriteProtect(MS_BOOL bEnable)
{
// Note: Temporarily don't call this function until MSTV_Tool ready
#if 1
    MS_BOOL bRet = FALSE;
    MS_U8   u8Status0, u8Status1;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, bEnable));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_SERFLASH_ActiveFlash_Set_HW_WP(DISABLE);
    MsOS_DelayTask(bEnable ? 5 : 20); // when disable WP, delay more time

    _HAL_ISP_Enable();

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_WriteProtect_return;
    }
    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_WriteProtect_return;
    }
    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR

    if (bEnable)
    {
        if (_hal_SERFLASH.u8MID == MID_ATMEL)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, SERFLASH_WRSR_BLK_PROTECT); // SPRL 1 -> 0

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR
        }

        ISP_WRITE(REG_ISP_SPI_WDATA, SF_SR_SRWD | SERFLASH_WRSR_BLK_PROTECT); // SF_SR_SRWD: SRWD Status Register Write Protect

        if ((_hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32) || (_hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K)
          || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV) || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV))//MSB S8-S15
        {
            if ( _HAL_SERFLASH_WaitWriteDone() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }
            ISP_WRITE(REG_ISP_SPI_WDATA, 0x40);
        }

    }
    else
    {
        if (_hal_SERFLASH.u8MID == MID_ATMEL)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, 0 << 2); // [4:2] or [5:2] protect blocks // SPRL 1 -> 0

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }

            ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

            if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }

            ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WRSR); // WRSR
        }

        if ((_hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32) || (_hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K)
          || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV) || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV))//MSB S8-S15
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, BITS(6:2, 0x1F)); // [6:2] protect blocks

            if ( _HAL_SERFLASH_WaitWriteDone() == FALSE )
            {
                goto HAL_SERFLASH_WriteProtect_return;
            }
            ISP_WRITE(REG_ISP_SPI_WDATA, 0x40);
        }
        else
        {
        ISP_WRITE(REG_ISP_SPI_WDATA, SF_SR_SRWD | 0 << 2); // [4:2] or [5:2] protect blocks
    }

    }

    bRet = _HAL_SERFLASH_WaitWriteDone();

HAL_SERFLASH_WriteProtect_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();
    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

   if ((_hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32) || (_hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K)
          || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV) || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV))//MSB S8-S15
    {
        HAL_SERFLASH_ReadStatusReg(&u8Status0);
        HAL_SERFLASH_ReadStatusReg2(&u8Status1);
        HAL_SERFLASH_WriteStatusReg(((u8Status1 << 8)|u8Status0|0x4000));//CMP = 1
        MsOS_DelayTask(40);
    }

    if (bEnable) // _REVIEW_
    {
        _HAL_SERFLASH_ActiveFlash_Set_HW_WP(bEnable);
    }

    //MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
#else
    return TRUE;
#endif
}


MS_BOOL HAL_SERFLASH_ReadID(MS_U8 *pu8Data, MS_U32 u32Size)
{
    // HW doesn't support ReadID on MX/ST flash; use trigger mode instead.
    MS_BOOL bRet = FALSE;
    MS_U32 u32I;
    MS_U8 *u8ptr = pu8Data;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_ReadID_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadID_return;
    }
    // SFSH_RIU_REG16(REG_SFSH_SPI_COMMAND) = ISP_SPI_CMD_RDID; // RDID
    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDID); // RDID
    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadID_return;
    }

    for ( u32I = 0; u32I < u32Size; u32I++ )
    {
        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_ReadID_return;
        }

        u8ptr[u32I] = ISP_READ(REG_ISP_SPI_RDATA);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf(" 0x%02X", u8ptr[u32I]));
    }
    bRet = TRUE;

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222); // disable trigger mode


HAL_SERFLASH_ReadID_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("\n"));

    return bRet;
}

MS_U64 HAL_SERFLASH_ReadUID(void)
{
    #define READ_UID_SIZE 8
    MS_U32 u32I = 0;
    MS_U8  u8ptr[READ_UID_SIZE];
    MS_U32 u32Size = READ_UID_SIZE;

    MS_U64   u64FlashUId = 0;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if(!_HAL_SERFLASH_WaitWriteDone())
    {
        goto HAL_SERFLASH_READUID_RETURN;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_READUID_RETURN;
    }
    // SFSH_RIU_REG16(REG_SFSH_SPI_COMMAND) = ISP_SPI_CMD_RDID; // RDID
    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333); // enable trigger mode

    if(_hal_SERFLASH.u16FlashType == FLASH_IC_EN25QH16)
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, 0x5A); // RDUID
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_READUID_RETURN;
        }
        for(u32I = 0; u32I < 2; u32I++)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, 0x00);
            if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
            {
                goto HAL_SERFLASH_READUID_RETURN;
            }
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, 0x80); //start address
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_READUID_RETURN;
        }
        ISP_WRITE(REG_ISP_SPI_WDATA, 0xFF);
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_READUID_RETURN;
        }
    }
    else if( _hal_SERFLASH.u16FlashType == FLASH_IC_W25Q16)
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, 0x4B); // RDUID
        if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_READUID_RETURN;
        }
        for(u32I = 0;u32I < 4;u32I++)
        {
            ISP_WRITE(REG_ISP_SPI_WDATA, 0xFF); // RDUID
            if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
            {
                goto HAL_SERFLASH_READUID_RETURN;
            }
        }
    }
    else
    {
         goto HAL_SERFLASH_READUID_RETURN;
    }
    SPI_FLASH_CMD(ISP_SPI_CMD_READ);

    for ( u32I = 0; u32I < u32Size; u32I++ )
    {
        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ); // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_READUID_RETURN;
        }

        u8ptr[u32I] = ISP_READ(REG_ISP_SPI_RDATA);
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s, %d 0x%02X\n",__FUNCTION__, u32I, u8ptr[u32I]));
    }

    for(u32I = 0;u32I < 8;u32I++)
    {
        u64FlashUId <<= 8;
        u64FlashUId += u8ptr[u32I];
    }

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222); // disable trigger mode


HAL_SERFLASH_READUID_RETURN:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("\n"));

    return u64FlashUId;
}

MS_BOOL HAL_SERFLASH_ReadREMS4(MS_U8 * pu8Data, MS_U32 u32Size)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Index;
    MS_U8 *u8ptr = pu8Data;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if ( !_HAL_SERFLASH_WaitWriteDone() )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    if ( !_HAL_SERFLASH_WaitWriteCmdRdy() )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);           // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_REMS4);   // READ_REMS4 for new MXIC Flash

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_WDATA_DUMMY);
    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_WDATA_DUMMY);
    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, 0x00); // if ADD is 0x00, MID first. if ADD is 0x01, DID first
    if ( _HAL_SERFLASH_WaitWriteDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadREMS4_return;
    }

    for ( u32Index = 0; u32Index < u32Size; u32Index++ )
    {
        ISP_WRITE(REG_ISP_SPI_RDREQ, ISP_SPI_RDREQ);   // SPI read request

        if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
        {
            goto HAL_SERFLASH_ReadREMS4_return;
        }

        u8ptr[u32Index] = ISP_READ(REG_ISP_SPI_RDATA);

        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf(" 0x%02X",  u8ptr[u32Index]));
    }

    bRet = TRUE;

        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

HAL_SERFLASH_ReadREMS4_return:

        ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

        _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

return bRet;

}

MS_BOOL HAL_SERFLASH_DMA(MS_U32 u32FlashStart, MS_U32 u32DRAMStart, MS_U32 u32Size)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;

#if defined (MSOS_TYPE_LINUX)
    MS_U32 u32Timeout = SERFLASH_SAFETY_FACTOR*u32Size/(108*1000/4/8);
#else
    MS_U32 u32Timeout = SERFLASH_SAFETY_FACTOR;
#endif

    u32Timeout=u32Timeout; //to make compiler happy
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, 0x%08X, %d)\n", __FUNCTION__, (int)u32FlashStart, (int)u32DRAMStart, (int)u32Size));

    // [URANUS_REV_A][OBSOLETE] // TODO: <-@@@ CHIP SPECIFIC
    #if 0   // TODO: review
    if (MDrv_SYS_GetChipRev() == 0x00)
    {
        // DMA program can't run on DRAM, but in flash ONLY
        return FALSE;
    }
    #endif  // TODO: review
    // [URANUS_REV_A][OBSOLETE] // TODO: <-@@@ CHIP SPECIFIC

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    ISP_WRITE_MASK(REG_ISP_CHIP_SEL, SFSH_CHIP_SEL_RIU, SFSH_CHIP_SEL_MODE_SEL_MASK);   // For DMA only

    _HAL_ISP_Disable();

    // SFSH_RIU_REG16(REG_SFSH_SPI_CLK_DIV) = 0x02; // 108MHz div3 (max. 50MHz for this ST flash) for FAST_READ

    PIU_WRITE(REG_PIU_DMA_SIZE_L, LOU16(u32Size));
    PIU_WRITE(REG_PIU_DMA_SIZE_H, HIU16(u32Size));
    PIU_WRITE(REG_PIU_DMA_DRAMSTART_L, LOU16(u32DRAMStart));
    PIU_WRITE(REG_PIU_DMA_DRAMSTART_H, HIU16(u32DRAMStart));
    PIU_WRITE(REG_PIU_DMA_SPISTART_L, LOU16(u32FlashStart));
    PIU_WRITE(REG_PIU_DMA_SPISTART_H, HIU16(u32FlashStart));
    // SFSH_PIU_REG16(REG_SFSH_DMA_CMD) = 0 << 5; // 0: little-endian 1: big-endian
    // SFSH_PIU_REG16(REG_SFSH_DMA_CMD) |= 1; // trigger
    PIU_WRITE(REG_PIU_DMA_CMD, PIU_DMA_CMD_LE | PIU_DMA_CMD_FIRE); // trigger

    // Wait for DMA to be done
    SER_FLASH_TIME(u32Timer, u32Timeout);
    do
    {
        if ( (PIU_READ(REG_PIU_DMA_STATUS) & PIU_DMA_DONE_MASK) == PIU_DMA_DONE ) // finished
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));

    if (bRet == FALSE)
    {
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("DMA timeout!\n"));
    }

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}


MS_BOOL HAL_SERFLASH_ReadStatusReg(MS_U8 *pu8StatusReg)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    *pu8StatusReg = 0xFF;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }
    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);           // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDSR);   // RDSR

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    ISP_WRITE(REG_ISP_SPI_RDREQ, 0x01); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    *pu8StatusReg = ISP_READ(REG_ISP_SPI_RDATA);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf(" 0x%02X", *pu8StatusReg));

    bRet = TRUE;

HAL_SERFLASH_ReadStatusReg_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("\n"));

    return bRet;
}

MS_BOOL HAL_SERFLASH_ReadStatusReg2(MS_U8 *pu8StatusReg)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    *pu8StatusReg = 0x00;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if ( !_HAL_SERFLASH_WaitWriteDone() )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);           // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDSR2);   // RDSR2

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    ISP_WRITE(REG_ISP_SPI_RDREQ, 0x01); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto HAL_SERFLASH_ReadStatusReg_return;
    }

    *pu8StatusReg = ISP_READ(REG_ISP_SPI_RDATA);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf(" 0x%02X", *pu8StatusReg));

    bRet = TRUE;

HAL_SERFLASH_ReadStatusReg_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("\n"));

    return bRet;
}

MS_BOOL HAL_SERFLASH_WriteStatusReg(MS_U16 u16StatusReg)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if ( !_HAL_SERFLASH_WaitWriteDone() )
    {
        goto HAL_SERFLASH_WriteStatusReg_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);          // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_WREN);   // WREN

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_WriteStatusReg_return;
    }

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR);      // SPI CEB dis

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_WRSR);   // WRSR

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_WriteStatusReg_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, (MS_U8)(u16StatusReg & 0xFF ));   // LSB

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_WriteStatusReg_return;
    }

    if((_hal_SERFLASH.u16FlashType == FLASH_IC_GD25Q32) || (_hal_SERFLASH.u16FlashType == FLASH_IC_S25FL032K)
     ||(_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q64CV) || (_hal_SERFLASH.u16FlashType == FLASH_IC_W25Q32BV))
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, (MS_U8)(u16StatusReg >> 8 ));   // MSB
       // printf("write_StatusReg=[%x]\n",u16StatusReg);

        if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
        {
            goto HAL_SERFLASH_WriteStatusReg_return;
        }
    }

    bRet = TRUE;

HAL_SERFLASH_WriteStatusReg_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("\n"));

    return bRet;
}

MS_BOOL HAL_SPI_EnterIBPM(void)
{
    MS_BOOL bRet = FALSE;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    _HAL_ISP_Enable();

    if ( !_HAL_SERFLASH_WaitWriteDone() )
    {
        goto HAL_SPI_EnableIBPM_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SPI_EnableIBPM_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);           // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_WPSEL);   // WPSEL

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SPI_EnableIBPM_return;
    }

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDSCUR);	// Read Security Register

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SPI_EnableIBPM_return;
    }

    ISP_WRITE(REG_ISP_SPI_RDREQ, 0x01); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto HAL_SPI_EnableIBPM_return;
    }

    if((ISP_READ(REG_ISP_SPI_RDATA) & BIT(7)) == BIT(7))
    {
        bRet = TRUE;
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,
                        printf("MXIC Security Register 0x%02X\n", ISP_READ(REG_ISP_SPI_RDATA)));
    }

HAL_SPI_EnableIBPM_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

MS_BOOL HAL_SPI_SingleBlockLock(MS_PHYADDR u32FlashAddr, MS_BOOL bLock)
{
    MS_BOOL bRet = FALSE;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return bRet;
    }

    if ( _bIBPM != TRUE )
    {
        printf("%s not in Individual Block Protect Mode\n", __FUNCTION__);
        MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
        return bRet;
    }

    _HAL_ISP_Enable();

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);   // Enable trigger mode

    if( bLock )
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_SBLK);   // Single Block Lock Protection
    }
    else
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_SBULK);   // Single Block unLock Protection
    }

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x10)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x08)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x00)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

#if defined (MS_DEBUG)
    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDBLOCK);	// Read Block Lock Status

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
       	goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x10)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x08)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x00)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    ISP_WRITE(REG_ISP_SPI_RDREQ, 0x01); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto HAL_SPI_SingleBlockLock_return;
    }

    if( bLock )
    {
        if( ISP_READ(REG_ISP_SPI_RDATA) == 0xFF )
            bRet = TRUE;
    }
    else
    {
        if( ISP_READ(REG_ISP_SPI_RDATA) == 0x00 )
            bRet = TRUE;
    }
#else//No Ceck
    bRet = TRUE;
#endif

HAL_SPI_SingleBlockLock_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

MS_BOOL HAL_SPI_GangBlockLock(MS_BOOL bLock)
{
    MS_BOOL bRet = FALSE;

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(%d)\n", __FUNCTION__, bLock));

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return bRet;
    }

    if ( _bIBPM != TRUE )
    {
        printf("%s not in Individual Block Protect Mode\n", __FUNCTION__);
        MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
        return bRet;
    }

    _HAL_SERFLASH_ActiveFlash_Set_HW_WP(bLock);
    MsOS_DelayTask(bLock ? 5 : 20); // when disable WP, delay more time

    _HAL_ISP_Enable();

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_WriteProtect_return;
    }

    ISP_WRITE(REG_ISP_SPI_COMMAND, ISP_SPI_CMD_WREN); // WREN

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SERFLASH_WriteProtect_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);	// Enable trigger mode

    if( bLock )
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_GBLK);   // Gang Block Lock Protection
    }
    else
    {
        ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_GBULK);   // Gang Block unLock Protection
    }

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
        goto HAL_SERFLASH_WriteProtect_return;
    }

    bRet = TRUE;

HAL_SERFLASH_WriteProtect_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    if (bLock) // _REVIEW_
    {
        _HAL_SERFLASH_ActiveFlash_Set_HW_WP(bLock);
    }

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return bRet;
}

MS_U8 HAL_SPI_ReadBlockStatus(MS_PHYADDR u32FlashAddr)
{
    MS_U8 u8Val = 0xA5;

    MS_ASSERT( MsOS_In_Interrupt() == FALSE );
    MS_ASSERT(_HAL_SERFLASH_Check51RunMode());

    if (FALSE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME))
    {
        printf("%s ENTRY fails!\n", __FUNCTION__);
        return u8Val;
    }

    if ( _bIBPM != TRUE )
    {
        printf("%s not in Individual Block Protect Mode\n", __FUNCTION__);
        MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
        return u8Val;
    }

    _HAL_ISP_Enable();

    if ( !_HAL_SERFLASH_WaitWriteDone() )
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    if ( _HAL_SERFLASH_WaitWriteCmdRdy() == FALSE )
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x3333);          // Enable trigger mode

    ISP_WRITE(REG_ISP_SPI_WDATA, ISP_SPI_CMD_RDBLOCK);	// Read Block Lock Status

    if ( !_HAL_SERFLASH_WaitWriteDataRdy() )
    {
       	goto HAL_SPI_ReadBlockStatus_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x10)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x08)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    ISP_WRITE(REG_ISP_SPI_WDATA, BITS(7:0, ((u32FlashAddr >> 0x00)&0xFF)));
    if(!_HAL_SERFLASH_WaitWriteDataRdy())
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    ISP_WRITE(REG_ISP_SPI_RDREQ, 0x01); // SPI read request

    if ( _HAL_SERFLASH_WaitReadDataRdy() == FALSE )
    {
        goto HAL_SPI_ReadBlockStatus_return;
    }

    u8Val = ISP_READ(REG_ISP_SPI_RDATA);

HAL_SPI_ReadBlockStatus_return:

    ISP_WRITE(REG_ISP_SPI_CECLR, ISP_SPI_CECLR); // SPI CEB dis

    ISP_WRITE(REG_ISP_TRIGGER_MODE, 0x2222);     // disable trigger mode

    _HAL_ISP_Disable();

    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);

    return u8Val;
}
MS_BOOL HAL_SERFLASH_WriteProtect_Area_Lookup(MS_U32 u32ProtectSpace, MS_U32 u32NonProtectSpace, MS_U8 *pu8BlockProtectBits)
{
    MS_U32 i;
    MS_U32 j;

    j=0xFFFFFFFF;
    for (i = 0;
        (0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32LowerBound) && (0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32UpperBound); // the end of write protect table
        i++
        )
       {
        if(_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound == 0x0)
        {

        #if 0
            if((((_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound)>= u32ProtectSpace) && (_hal_SERFLASH.u32FlashSize-(_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound)))>= u32NonProtectSpace)
            {
                  if(((_hal_SERFLASH.pWriteProtectTable[j].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[j].u32LowerBound) >= (_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound)) || (j==-1))
                   {
                      j = i;
                      *pu8BlockProtectBits = _hal_SERFLASH.pWriteProtectTable[j].u8BlockProtectBits;
                      DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("pu8BlockProtectBits = %x\n",*pu8BlockProtectBits));
                   }
              }
        #else

                if(((_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound+1)>= u32ProtectSpace) && ((_hal_SERFLASH.u32FlashSize-(_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound+1))>= u32NonProtectSpace))
                {

                      if(j==0xFFFFFFFF)
                        {
                            j = i;
                            *pu8BlockProtectBits = _hal_SERFLASH.pWriteProtectTable[j].u8BlockProtectBits;
                            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("pu8BlockProtectBits = %x\n",*pu8BlockProtectBits));
                        }
                      else
                        {
                            if((_hal_SERFLASH.pWriteProtectTable[j].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[j].u32LowerBound) >= (_hal_SERFLASH.pWriteProtectTable[i].u32UpperBound-_hal_SERFLASH.pWriteProtectTable[i].u32LowerBound))
                                {
                                    j = i;
                                    *pu8BlockProtectBits = _hal_SERFLASH.pWriteProtectTable[j].u8BlockProtectBits;
                                    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("pu8BlockProtectBits = %x\n",*pu8BlockProtectBits));
                                }
                        }
                  }


        #endif

        }
    }

    if(j==0xFFFFFFFF)  //check Protect Bits
        return FALSE;
    else
        return TRUE;

}

MS_U32 HAL_SERFLASH_WriteProtect_Area_Boundary(void)
{

    MS_U32  i;
    MS_U32  k;

    MS_U8 u8stats = 0x00;
    k = 0xFFFFFFFF;

    HAL_SERFLASH_ReadStatusReg(&u8stats);

    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("u8stats = %x\n",u8stats&_hal_SERFLASH.u8WrsrBlkProtect));

    for (i = 0;
        (0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32LowerBound) && (0xFFFFFFFF != _hal_SERFLASH.pWriteProtectTable[i].u32UpperBound); // the end of write protect table
        i++
        )
    {
        if(_hal_SERFLASH.pWriteProtectTable[i].u8BlockProtectBits == (u8stats&_hal_SERFLASH.u8WrsrBlkProtect))
        {
            k = i;
            DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG,printf("u32UpperBound = %x\n",_hal_SERFLASH.pWriteProtectTable[k].u32UpperBound));
            return _hal_SERFLASH.pWriteProtectTable[k].u32UpperBound;
        }
    }
    return FALSE;
}

void HAL_SERFLASH_SelectReadMode(SPI_READ_MODE eReadMode)
{
    switch(eReadMode)
    {
    case E_SINGLE_MODE:
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_FAST_DISABLE, SFSH_CHIP_FAST_MASK);
    break;
    case E_FAST_MODE:
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_FAST_ENABLE, SFSH_CHIP_FAST_MASK);
    break;
    case E_DUAL_D_MODE:
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_2XREAD_DADD_ENABLE, SFSH_CHIP_2XREAD_MASK);
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_FAST_DISABLE, SFSH_CHIP_FAST_MASK);
    break;
    case E_DUAL_AD_MODE:
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_2XREAD_DADD_ENABLE, SFSH_CHIP_2XREAD_MASK);
    break;
    case E_QUAD_MODE:
        QSPI_WRITE_MASK(REG_ISP_SPI_MODE, SFSH_CHIP_QUAD_ENABLE, SFSH_CHIP_QUAD_MASK);
    break;
    }

}

MS_BOOL HAL_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        printf("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_hal_isp.VirtRiuBaseAddr))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

MS_BOOL HAL_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        printf("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

   ((volatile MS_U16*)(_hal_isp.VirtRiuBaseAddr))[u32RegAddr] = u16Val;
    return TRUE;
}

MS_U8 HAL_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_hal_isp.VirtRiuBaseAddr))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

MS_U16 HAL_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_hal_isp.VirtRiuBaseAddr))[u32RegAddr];
}
#if 0
MS_U16 HAL_FSP_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        printf("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_hal_isp.u32FspBaseAddr))[u32RegAddr] = u16Val;
    return TRUE;

}
#endif
#if 0
static void _HAL_FSP_SetWData(MS_U8 *pu8Data, MS_U8 u8DataSize)
{
    MS_U8 u8Index = 0;
    MS_U32 u32Addr = 0;
    for (u8Index = 0; u8Index < u8DataSize; u8Index++)
    {
        u32Addr = REG_FSP_WRITE_BUFF + REG8_FSP_WDATA + u8Index;
        HAL_WriteByte(u32Addr, pu8Data[u8Index]);
    }
}
#endif

static MS_BOOL _HAL_FSP_WaitDone(void)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32Timer;
    SER_FLASH_TIME(u32Timer, SERFLASH_SAFETY_FACTOR * 30);
    do{
        if ( (FSP_READ(REG_FSP_DONE_FLAG) & REG_FSP_DONE_FLAG_MASK) == REG_FSP_DONE )
        {
            bRet = TRUE;
            break;
        }
    } while (!SER_FLASH_EXPIRE(u32Timer));
    FSP_WRITE_MASK(REG_FSP_DONE_CLR,REG_FSP_CLR,REG_FSP_DONE_CLR_MASK);
    return bRet;
}

static MS_U8 _HAL_FSP_ReadBuf0(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB0) & 0x00FF) >> 0);
}

static MS_U8 _HAL_FSP_ReadBuf1(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB1) & 0xFF00) >> 8);
}

static MS_U8 _HAL_FSP_ReadBuf2(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB2) & 0x00FF) >> 0);
}

static MS_U8 _HAL_FSP_ReadBuf3(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB3) & 0xFF00) >> 8);
}

static MS_U8 _HAL_FSP_ReadBuf4(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB4) & 0x00FF) >> 0);
}

static MS_U8 _HAL_FSP_ReadBuf5(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB5) & 0xFF00) >> 8);
}

static MS_U8 _HAL_FSP_ReadBuf6(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB6) & 0x00FF) >> 0);
}

static MS_U8 _HAL_FSP_ReadBuf7(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB7) & 0xFF00) >> 8);
}

static MS_U8 _HAL_FSP_ReadBuf8(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB8) & 0x00FF) >> 0);
}

static MS_U8 _HAL_FSP_ReadBuf9(void)
{
    return (MS_U8)((FSP_READ(REG_FSP_RDB9) & 0xFF00) >> 8);
}

MS_U8 HAL_FSP_ReadBufs(MS_U8 u8Idx)
{
    MS_U8 u8Data;
    switch ( u8Idx )
    {
    case 0:    u8Data = _HAL_FSP_ReadBuf0();
        break;
    case 1:    u8Data = _HAL_FSP_ReadBuf1();
        break;
    case 2:    u8Data = _HAL_FSP_ReadBuf2();
        break;
    case 3:    u8Data = _HAL_FSP_ReadBuf3();
        break;
    case 4:    u8Data = _HAL_FSP_ReadBuf4();
        break;
    case 5:    u8Data = _HAL_FSP_ReadBuf5();
        break;
    case 6:    u8Data = _HAL_FSP_ReadBuf6();
        break;
    case 7:    u8Data = _HAL_FSP_ReadBuf7();
        break;
    case 8:    u8Data = _HAL_FSP_ReadBuf8();
        break;
    case 9:    u8Data = _HAL_FSP_ReadBuf9();
        break;
    default:    u8Data = 0xFF;
                return -1;    
    }
    return u8Data;
}

static void _HAL_FSP_WriteBuf0(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB0,REG_FSP_WDB0_DATA(u8Data),REG_FSP_WDB0_MASK);
}

static void _HAL_FSP_WriteBuf1(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB1,REG_FSP_WDB1_DATA(u8Data),REG_FSP_WDB1_MASK);
}

static void _HAL_FSP_WriteBuf2(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB2,REG_FSP_WDB2_DATA(u8Data),REG_FSP_WDB2_MASK);
}

static void _HAL_FSP_WriteBuf3(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB3,REG_FSP_WDB3_DATA(u8Data),REG_FSP_WDB3_MASK);
}

static void _HAL_FSP_WriteBuf4(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB4,REG_FSP_WDB4_DATA(u8Data),REG_FSP_WDB4_MASK);
}

static void _HAL_FSP_WriteBuf5(MS_U8 u8Data){
    FSP_WRITE_MASK(REG_FSP_WDB5,REG_FSP_WDB5_DATA(u8Data),REG_FSP_WDB5_MASK);
}

static void _HAL_FSP_WriteBuf6(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB6,REG_FSP_WDB6_DATA(u8Data),REG_FSP_WDB6_MASK);
}

static void _HAL_FSP_WriteBuf7(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB7,REG_FSP_WDB7_DATA(u8Data),REG_FSP_WDB7_MASK);
}

static void _HAL_FSP_WriteBuf8(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB8,REG_FSP_WDB8_DATA(u8Data),REG_FSP_WDB8_MASK);
}

static void _HAL_FSP_WriteBuf9(MS_U8 u8Data)
{
    FSP_WRITE_MASK(REG_FSP_WDB9,REG_FSP_WDB9_DATA(u8Data),REG_FSP_WDB9_MASK);
}

void HAL_FSP_WriteBufs(MS_U8 u8Idx, MS_U8 u8Data)
{
    switch ( u8Idx )
    {
    case 0:   
        _HAL_FSP_WriteBuf0(u8Data);
        break;
    case 1:
        _HAL_FSP_WriteBuf1(u8Data);
        break;
    case 2:
        _HAL_FSP_WriteBuf2(u8Data);
        break;
    case 3:
        _HAL_FSP_WriteBuf3(u8Data);
        break;
    case 4:
        _HAL_FSP_WriteBuf4(u8Data);
        break;
    case 5:
        _HAL_FSP_WriteBuf5(u8Data);
        break;
    case 6:
        _HAL_FSP_WriteBuf6(u8Data);
        break;
    case 7:
        _HAL_FSP_WriteBuf7(u8Data);
        break;
    case 8:
        _HAL_FSP_WriteBuf8(u8Data);
        break;
    case 9:
        _HAL_FSP_WriteBuf9(u8Data);
        break;
    default:
        DEBUG_SER_FLASH(E_SERFLASH_DBGLV_ERR, printf("HAL_FSP_WriteBufs fails\n"));
        break;
    }
}

void HAL_FSP_Entry(void)
{
    MS_ASSERT(TRUE == MsOS_ObtainMutex(_s32SERFLASH_Mutex, SERFLASH_MUTEX_WAIT_TIME));
    u8Mode = (FSP_READ(REG_ISP_SPI_MODE) & 0x000F);
    FSP_WRITE(REG_ISP_SPI_MODE, 0x0000);
}

void HAL_FSP_Exit(void)
{
    FSP_WRITE(REG_ISP_SPI_MODE, u8Mode);
    MsOS_ReleaseMutex(_s32SERFLASH_Mutex);
}

MS_BOOL HAL_FSP_EraseChip(void)
{
    MS_BOOL bRet = TRUE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()\n", __FUNCTION__));
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    HAL_FSP_WriteBufs(1,SPI_CMD_CE);
    HAL_FSP_WriteBufs(2,SPI_CMD_RDSR);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(1),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_WaitDone();
    return bRet;
}

MS_BOOL HAL_FSP_BlockErase(MS_U32 u32FlashAddr, EN_FLASH_ERASE eSize)
{
    MS_BOOL bRet = TRUE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s(0x%08X, 0x%08X)\n", __FUNCTION__, (int)u32FlashAddr, (int)eSize));
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    HAL_FSP_WriteBufs(1,eSize);
    HAL_FSP_WriteBufs(2,(MS_U8)((u32FlashAddr>>0x10)&0xFF));
    HAL_FSP_WriteBufs(3,(MS_U8)((u32FlashAddr>>0x08)&0xFF));
    HAL_FSP_WriteBufs(4,(MS_U8)((u32FlashAddr>>0x00)&0xFF));
    HAL_FSP_WriteBufs(5,SPI_CMD_RDSR);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(4),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_WaitDone();
    return bRet;
}

MS_BOOL HAL_FSP_CheckWriteDone(void)
{
    MS_BOOL bRet = TRUE;
    DEBUG_SER_FLASH(E_SERFLASH_DBGLV_DEBUG, printf("%s()", __FUNCTION__));
    HAL_FSP_WriteBufs(0,SPI_CMD_RDSR);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(0),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(0),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(1),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(0),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_OFF,REG_FSP_2NDCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_OFF,REG_FSP_3THCMD_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_1STCMD,REG_FSP_RDSR_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
    bRet &= _HAL_FSP_WaitDone();
    return bRet;
}

MS_BOOL HAL_FSP_Write_Burst(MS_U32 u32Addr, MS_U32 u32Size, MS_U8* pu8Data)
{
    MS_BOOL bRet = TRUE;
    MS_U8  u8Status = 0;
    MS_U16 u16Data = 0;
    MS_U32 u32I, u32J, u32K;
    MS_U32 u32quotient;
    MS_U32 u32remainder;
    MS_U32 u32PageIdx;
    MS_U32 u32PageRemainder;
    //write enable
    HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(0),REG_FSP_WBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(0),REG_FSP_WBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
    FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(0),REG_FSP_RBF_SIZE2_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
    FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
    u16Data = FSP_READ(REG_FSP_CTRL);
    u16Data &= 0xFF;
    FSP_WRITE(REG_FSP_CTRL, u16Data);
    FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);

    bRet &= _HAL_FSP_WaitDone();
    if(!bRet)
    {
        printf("Write enable command Fail!!!!\r\n");
        return bRet;
    }

    u32PageIdx = u32Size / FLASH_PAGE_SIZE;
    u32PageRemainder = u32Size % FLASH_PAGE_SIZE;
    if(u32PageRemainder)
    {
        u32PageIdx++;
        u32Size = u32PageRemainder; 
    }
    else
    {
        u32Size = FLASH_PAGE_SIZE;
    }
    for(u32K = 0; u32K < u32PageIdx; u32K++)
    {
        //write command
        HAL_FSP_WriteBufs(0,SPI_CMD_PP);
        HAL_FSP_WriteBufs(1,(MS_U8)((u32Addr>>0x10)&0xFF));
        HAL_FSP_WriteBufs(2,(MS_U8)((u32Addr>>0x08)&0xFF));
        HAL_FSP_WriteBufs(3,(MS_U8)((u32Addr>>0x00)&0xFF));
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(4),REG_FSP_WBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
        bRet &= _HAL_FSP_WaitDone();
 
        if(!bRet)
        {
            printf("Write command Fail!!!!\r\n");
            return bRet;
        }
        QSPI_WRITE(REG_SPI_BURST_WRITE,REG_SPI_ENABLE_BURST);

        u32quotient = (u32Size / REG_FSP_MAX_WRITEDATA_SIZE);
        u32remainder = (u32Size % REG_FSP_MAX_WRITEDATA_SIZE);
        if(u32remainder)
        {
            u32quotient++;
            u32Size = u32remainder;
        }
        else
            u32Size = REG_FSP_MAX_WRITEDATA_SIZE;

        for( u32I = 0; u32I < u32quotient; u32I++ )
        {
            for( u32J = 0; u32J < u32Size; u32J++ )
            {
                HAL_FSP_WriteBufs(u32J,*(pu8Data+u32J));
            }
            pu8Data += u32I;
            u32Size = REG_FSP_MAX_WRITEDATA_SIZE;
            FSP_WRITE_MASK(REG_FSP_WBF_SIZE, REG_FSP_WBF_SIZE0(u32J), REG_FSP_WBF_SIZE0_MASK);
            FSP_WRITE_MASK(REG_FSP_TRIGGER, REG_FSP_FIRE, REG_FSP_TRIGGER_MASK);
            bRet &= _HAL_FSP_WaitDone();
        }

        QSPI_WRITE(REG_SPI_BURST_WRITE,REG_SPI_DISABLE_BURST);
        do
        {
            HAL_FSP_WriteBufs(0,SPI_CMD_RDSR);
            FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
            FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(1),REG_FSP_RBF_SIZE0_MASK);
            FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
            bRet &= _HAL_FSP_WaitDone();
            u8Status = HAL_FSP_ReadBufs(0);
        }while(u8Status & FLASH_OIP);
        u32Size = FLASH_PAGE_SIZE;
    }
    return bRet;
}

MS_BOOL HAL_FSP_Write(MS_U32 u32Addr, MS_U32 u32DataSize, MS_U8 *pu8Data)
{
    if(_bFSPMode)
    {
        printf("Burst Mode Write Data!!!!\r\n");
        return HAL_FSP_Write_Burst(u32Addr, u32DataSize, pu8Data);
    }
    else
    {
        printf("Normal Mode Write Data!!!!\r\n");
        return HAL_FSP_Write_Normal(u32Addr, u32DataSize, pu8Data);
    }
}

MS_BOOL HAL_FSP_Write_Normal(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    MS_U32 u32I, u32J;
    MS_U32 u32quotient;
    MS_U32 u32remainder;
    MS_U8  u8BufSize = REG_FSP_WRITEDATA_SIZE;
    u32quotient = (u32Size / u8BufSize);
    u32remainder = (u32Size % u8BufSize);
    printf("HAL_FSP_Write:Write Data Size %d \r\n",u32Size);
    printf("HAL_FSP_Write:u32quotient %d \r\n",u32quotient);
    printf("HAL_FSP_Write:u32remainder %d \r\n",u32remainder);
    if(u32remainder)
    {
        u32quotient++;
        u32Size = u32remainder;
    }
    else
        u32Size = u8BufSize;
    for(u32J = 0; u32J < u32quotient; u32J++)
    {
        HAL_FSP_WriteBufs(0,SPI_CMD_WREN);
        HAL_FSP_WriteBufs(1,SPI_CMD_PP);
        HAL_FSP_WriteBufs(2,(MS_U8)((u32Addr>>0x10)&0xFF));
        HAL_FSP_WriteBufs(3,(MS_U8)((u32Addr>>0x08)&0xFF));
        HAL_FSP_WriteBufs(4,(MS_U8)((u32Addr>>0x00)&0xFF));
        for( u32I = 0; u32I < u32Size; u32I++ )
        {
            HAL_FSP_WriteBufs((5+u32I),*(pu8Data+u32I));
        }
        u32Addr += u32I;
        pu8Data += u32I;
        HAL_FSP_WriteBufs((5 + u32Size),SPI_CMD_RDSR);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(1),REG_FSP_WBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1((4+u32Size)),REG_FSP_WBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(1),REG_FSP_WBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(0),REG_FSP_RBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(1),REG_FSP_RBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_ON,REG_FSP_2NDCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_ON,REG_FSP_3THCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD,REG_FSP_RDSR_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_ON,REG_FSP_FSCHK_MASK);
        FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
        bRet &= _HAL_FSP_WaitDone();
        u32Size = u8BufSize;
        
    }
    return bRet;
}

MS_BOOL HAL_FSP_Read(MS_U32 u32Addr, MS_U32 u32Size, MS_U8 *pu8Data)
{
    MS_BOOL bRet = TRUE;
    MS_U32 u32Idx, u32J;
    MS_U32 u32quotient;
    MS_U32 u32remainder;
    MS_U8  u8BufSize = REG_FSP_READDATA_SIZE;
    u32quotient = (u32Size / u8BufSize);
    u32remainder = (u32Size % u8BufSize);
    printf("HAL_FSP_Read:Write Data Size %d \r\n",u32Size);
    printf("HAL_FSP_Read:FLASH ADDRESS %x\r\n",u32Addr);
    printf("HAL_FSP_Read:u32quotient %d \r\n",u32quotient);
    printf("HAL_FSP_Read:u32remainder %d \r\n",u32remainder);
    if(u32remainder)
    {
        u32quotient++;
        u32Size = u32remainder;
    }
    else
        u32Size = u8BufSize;
    for(u32J = 0; u32J < u32quotient; u32J++)
    {
        HAL_FSP_WriteBufs(0, SPI_CMD_FASTREAD);
        HAL_FSP_WriteBufs(1,(MS_U8)((u32Addr>>0x10)&0xFF));
        HAL_FSP_WriteBufs(2,(MS_U8)((u32Addr>>0x08)&0xFF));
        HAL_FSP_WriteBufs(3,(MS_U8)((u32Addr>>0x00)&0xFF));
        HAL_FSP_WriteBufs(4, 0x00);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE0(5),REG_FSP_WBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE1(0),REG_FSP_WBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_WBF_SIZE,REG_FSP_WBF_SIZE2(0),REG_FSP_WBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE0(u32Size),REG_FSP_RBF_SIZE0_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE1(0),REG_FSP_RBF_SIZE1_MASK);
        FSP_WRITE_MASK(REG_FSP_RBF_SIZE,REG_FSP_RBF_SIZE2(0),REG_FSP_RBF_SIZE2_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_ENABLE,REG_FSP_ENABLE_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_NRESET,REG_FSP_RESET_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_INT,REG_FSP_INT_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_2NDCMD_OFF,REG_FSP_2NDCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_3THCMD_OFF,REG_FSP_3THCMD_MASK);
        FSP_WRITE_MASK(REG_FSP_CTRL,REG_FSP_FSCHK_OFF,REG_FSP_FSCHK_MASK);
        FSP_WRITE_MASK(REG_FSP_TRIGGER,REG_FSP_FIRE,REG_FSP_TRIGGER_MASK);
        bRet &= _HAL_FSP_WaitDone();
        for( u32Idx = 0; u32Idx < u32Size; u32Idx++ )
            *(pu8Data + u32Idx) = HAL_FSP_ReadBufs(u32Idx);
        pu8Data += u32Idx;
        u32Addr += u32Idx;
    }
    return bRet;
}
