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
/// file    drvMSPI.c
/// @brief  Master SPI Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvLDMA.h"
#include "MsOS.h"
#include "ULog.h"

// Internal Definition
#include "regLDMA.h"
#include "halLDMA.h"
#include "drvMMIO.h"
#include "utopia.h"

#define TAG_LDMA "LDMA"

static MS_S32 _gs32LDMA_Mutex;
//DMA0 DMA1
static MS_BOOL bInited[2]= {false,false};
MS_U8 _u8LDMADbgLevel;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DEBUG_LDMA(debug_level, x)     do { if (_u8LDMADbgLevel >= (debug_level)) (x); } while(0)


//------------------------------------------------------------------------------
/// Description : Set detailed level of LDMA driver debug message
/// @param u8DbgLevel    \b IN  debug level for local dimming LDMA
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_LDMA_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8LDMADbgLevel = u8DbgLevel;

    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Description : Set Menuload Number [if need transfer 100 data, then u32MenuldNum =100 ]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u32MenuldNum \b IN: set Dma load to mspi data number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetMenuloadNumber(MS_U8 u8HWNum, MS_U32 u32MenuldNum )
{
    if(bInited[u8HWNum] == false)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }
    return HAL_LDMA_SetMenuloadNumber(u8HWNum,  u32MenuldNum);
}
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI Command Format
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8CmdLen \b IN: set mspi spec cmd length
/// @param pu16CmdBuf \b IN: mspi spec cmds
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetSPICommandFormat( MS_U8 u8HWNum,MS_U8 u8CmdLen, MS_U16* pu16CmdBuf )
{
    if(bInited[u8HWNum] == false)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }

    return  HAL_LDMA_SetSPICommandFormat(u8HWNum, u8CmdLen,  pu16CmdBuf  );
}
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI check sum mode
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8SumMode \b IN: set spi data check summode [000: All (start + command + data) 001: Command + data 010: Data only]
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetCheckSumMode(MS_U8 u8HWNum,MS_U8 u8SumMode)
{
    if(bInited[u8HWNum] == false)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }

    return  HAL_LDMA_SetCheckSumMode(u8HWNum, u8SumMode);
}
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI trigger mode
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8TriggerMode \b IN: set DMA trigger mspi mode [0:trigger stop 1: one shot real time trigger 2: HW real time trigger]
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetSpiTriggerMode( MS_U8 u8HWNum, MS_U8 u8TriggerMode )
{
    if(bInited[u8HWNum] == false)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }

    return HAL_LDMA_SetSPITriggerMode(u8HWNum,  u8TriggerMode);
}
//-------------------------------------------------------------------------------------------------
/// Description : Set MSPI Trigger delay time
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param pu16TrigDelay \b IN: set spi clk delay time while trigger done
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_SetTrigDelay(MS_U8 u8HWNum,MS_U16 *pu16TrigDelay)
{
    if(bInited[u8HWNum] == false ||(pu16TrigDelay == NULL))
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }

    return  HAL_LDMA_SetTrigDelayCnt(u8HWNum, pu16TrigDelay);
}
//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma cs
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable cs
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_EnableCS(MS_U8 u8HWNum, MS_BOOL bEnable)
{
    if(bInited[u8HWNum] == false)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has not initiated, please check init first!\n",u8HWNum));
        return -1;
    }

    return  HAL_LDMA_EnableCS(u8HWNum,  bEnable);
}
//-------------------------------------------------------------------------------------------------
/// Description : LDMA Init function to set iobase [should call first]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8ClkHz \b IN: select local dimming period for control LD bank
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_S8 MDrv_LDMA_Init(MS_U8 u8HWNum,MS_U8 u8ClkHz)
{
    MS_S8 ret = 0;
    MS_VIRT VirtNONPMBank = 0;
    MS_PHY  u32NONPMBankSize = 0;

    if(bInited[u8HWNum])
    {
         DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"LDMA Channel %d has been  initiated \n",u8HWNum));
        return -1;
    }
    //config LDM_DMA register base
    _gs32LDMA_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex LDMA", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32LDMA_Mutex >= 0);

    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
         DEBUG_LDMA(E_LDMA_DBGLV_ERR,ULOGE(TAG_LDMA,"IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
        return -1;
    }

    HAL_LDMA_MMIOConfig(VirtNONPMBank,u8HWNum);

    bInited[u8HWNum] = true;

    return ret;
}
