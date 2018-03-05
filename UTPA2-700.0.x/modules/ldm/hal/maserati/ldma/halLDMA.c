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
/// file    halLDMA.c
/// @brief  local dimming DMA Driver HAL Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "halLDMA.h"
#include "regLDMA.h"

extern MS_U8 _u8LDMADbgLevel;

typedef enum
{
    E_LDMA_DBGLV_NONE,    //disable all the debug message
    E_LDMA_DBGLV_INFO,    //information
    E_LDMA_DBGLV_NOTICE,  //normal but significant condition
    E_LDMA_DBGLV_WARNING, //warning conditions
    E_LDMA_DBGLV_ERR,     //error conditions
    E_LDMA_DBGLV_CRIT,    //critical conditions
    E_LDMA_DBGLV_ALERT,   //action must be taken immediately
    E_LDMA_DBGLV_EMERG,   //system is unusable
    E_LDMA_DBGLV_DEBUG,   //debug-level messages
} LDMA_DbgLv;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DEBUG_LDMA(debug_level, x)     do { if (_u8LDMADbgLevel >= (debug_level)) (x); } while(0)

#define LDMA_WriteByte(reg,val) WRITE_BYTE((_hal_ldma.u32VirtLdmaBaseAddr + ((reg)<<2)),val)

#define LDMA_Write2Byte(reg,val) WRITE_WORD((_hal_ldma.u32VirtLdmaBaseAddr + ((reg)<<2)),val)

#define LDMA_ReadByte(reg)    (READ_BYTE(_hal_ldma.u32VirtLdmaBaseAddr + ((reg)<<2)))

#define LDMA_Read2Byte(reg)  (READ_WORD(_hal_ldma.u32VirtLdmaBaseAddr + ((reg)<<2)))


static LDMA_BaseAddr_st _hal_ldma = {
    .u8CurrentCH = 0,
    .u32VirtLdmaBaseAddr = BASEADDR_RIU + REG_LDMA0_BASE,
    .u32NONPMRegBaseAddr = BASEADDR_RIU,
};

static void _HAL_LDMA_CheckAndSetBaseAddr(MS_U8 u8Channel)
{
    DEBUG_LDMA(E_LDMA_DBGLV_INFO,printf("_HAL_LDMA_CheckAndSetBaseAddr channel %d\n",u8Channel));
    if(u8Channel == _hal_ldma.u8CurrentCH) {
        return;
    } else if(u8Channel ==  LDMA0) {
        _hal_ldma.u32VirtLdmaBaseAddr = _hal_ldma.u32NONPMRegBaseAddr + REG_LDMA0_BASE;
    } else if(u8Channel ==  LDMA1) {
        _hal_ldma.u32VirtLdmaBaseAddr = _hal_ldma.u32NONPMRegBaseAddr + REG_LDMA1_BASE;
    } else {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("LDMA Channel is out of range!\n"));
        return ;
    }
}

//------------------------------------------------------------------------------
/// Description : Config LDMA MMIO base address
/// @param u32PMBankBaseAddr \b IN:base address of MMIO (PM domain)
/// @param u32NONPMRegBaseAddr \b IN :base address of MMIO
/// @param u8DeviceIndex \b IN: index of HW IP
//------------------------------------------------------------------------------
MS_BOOL HAL_LDMA_MMIOConfig(MS_U32 u32NONPMRegBaseAddr, MS_U8 u8Channel)
{
    _hal_ldma.u32NONPMRegBaseAddr = u32NONPMRegBaseAddr;
    if(u8Channel ==  LDMA0)
    {
        _hal_ldma.u32VirtLdmaBaseAddr = u32NONPMRegBaseAddr + REG_LDMA0_BASE;

        DEBUG_LDMA(E_LDMA_DBGLV_INFO,printf("[E_LDMA0]_hal_ldma.u32VirtLdmaBaseAddr = %lx\n",_hal_ldma.u32VirtLdmaBaseAddr));
    }
    else if(u8Channel ==  LDMA1)
    {
        _hal_ldma.u32VirtLdmaBaseAddr = u32NONPMRegBaseAddr + REG_LDMA1_BASE;

        DEBUG_LDMA(E_LDMA_DBGLV_INFO,printf("[E_LDMA1]_hal_ldma.u32VirtLdmaBaseAddr = %lx\n",_hal_ldma.u32VirtLdmaBaseAddr));
    }
    else
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("LDMA Channel is out of range!\n"));
        return FALSE;
    }
    return TRUE;
}

MS_BOOL   HAL_LDMA_SetSPITriggerMode(MS_U8 u8Channel, MS_U8 u8TriggerMode)
{
    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    switch(u8TriggerMode)
    {
        case LDMA_SPI_TRIGGER_STOP:
            // Menuload trigger src = one shot

            LDMA_Write2Byte(REG_MENULOAD_CTRL_MODE, LDMA_Read2Byte(REG_MENULOAD_CTRL_MODE)&(~REG_MENULOAD_CTRL_MODE_MSK));
            // menuload abort one shot
            LDMA_Write2Byte(REG_MENULOAD_ABORT, LDMA_Read2Byte(REG_MENULOAD_ABORT)|BIT0);
            // menuload disable one shot
            LDMA_Write2Byte(REG_MENULOAD_ENABLE, LDMA_Read2Byte(REG_MENULOAD_ENABLE)&(~BIT0));
            break;

        case LDMA_SPI_TRIGGER_ONE_SHOT:
            // Menuload trigger src = one shot
            LDMA_Write2Byte(REG_MENULOAD_CTRL_MODE, LDMA_Read2Byte(REG_MENULOAD_CTRL_MODE)&(~REG_MENULOAD_CTRL_MODE_MSK));
            // menuload enable one shot
            LDMA_Write2Byte(REG_MENULOAD_ENABLE, LDMA_Read2Byte(REG_MENULOAD_ENABLE)|(BIT0));
            // menuload realtime trigger one shot
            LDMA_Write2Byte(REG_MENULOAD_REALTIME_TRIG, LDMA_Read2Byte(REG_MENULOAD_REALTIME_TRIG)|(BIT0));

            break;

        case LDMA_SPI_TRIGGER_EVERY_VSYNC:
            // Menuload trigger src = V sync
            LDMA_Write2Byte(REG_MENULOAD_CTRL_MODE, LDMA_Read2Byte(REG_MENULOAD_CTRL_MODE)|BIT0);
            LDMA_Write2Byte(REG_USE_HW_LOOP_MD, LDMA_Read2Byte(REG_USE_HW_LOOP_MD)|BIT7);
            LDMA_Write2Byte(REG_MENULOAD_REALTIME_TRIG, LDMA_Read2Byte(REG_MENULOAD_REALTIME_TRIG)|(BIT0));

            break;
        default :
            break;
    }

    return TRUE;
}

MS_BOOL HAL_LDMA_SetMenuloadNumber(MS_U8 u8Channel, MS_U32 u32MenuldNum)
{
    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    LDMA_Write2Byte(REG_MENULOAD_NUMBER, u32MenuldNum);

    return TRUE;
}

MS_BOOL HAL_LDMA_SetSPICommandFormat(MS_U8 u8Channel,MS_U8 u8CmdLen, MS_U16* pu16CmdBuf )
{
    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    if( u8CmdLen > LDMA_MAX_SPI_CMD_NUM )
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("[LDM-DMA]Spi command len is too long=%u!\n", u8CmdLen));
        u8CmdLen = LDMA_MAX_SPI_CMD_NUM;
    }

    if(pu16CmdBuf == NULL)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("NULL Pointer,please check pu16CmdBuf!\n "));
        return FALSE;
    }

    switch (u8CmdLen)
    {
        case 10:
            LDMA_Write2Byte(REG_COMMAND8, pu16CmdBuf[9]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT9);
        case 9:
            LDMA_Write2Byte(REG_COMMAND7, pu16CmdBuf[8]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT8);
        case 8:
            LDMA_Write2Byte(REG_COMMAND6, pu16CmdBuf[7]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT7);
        case 7:
            LDMA_Write2Byte(REG_COMMAND5, pu16CmdBuf[6]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT6);
        case 6:
            LDMA_Write2Byte(REG_COMMAND4, pu16CmdBuf[5]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT5);
        case 5:
            LDMA_Write2Byte(REG_COMMAND3, pu16CmdBuf[4]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT4);
        case 4:
            LDMA_Write2Byte(REG_COMMAND2, pu16CmdBuf[3]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT3);
        case 3:
            LDMA_Write2Byte(REG_COMMAND1, pu16CmdBuf[2]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT2);
        case 2:
            LDMA_Write2Byte(REG_COMMAND, pu16CmdBuf[1]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT1);
        case 1:
            LDMA_Write2Byte(REG_START_CODE, pu16CmdBuf[0]);
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT0);
            break;
        default:
            DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("Oops, u8CmdLen should  be bigger than 0\n"));
    }

    return TRUE;
}

MS_BOOL HAL_LDMA_SetCheckSumMode(MS_U8 u8Channel,MS_U8 u8SumMode)
{
    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)|BIT15);
    switch (u8SumMode)
    {
        case LDMA_NOCHECKSUM: {
            LDMA_Write2Byte(REG_CMD_LENGTH, LDMA_Read2Byte(REG_CMD_LENGTH)&(~BIT15));
            break;
        }
        case LDMA_CHECKSUM_ALL: {
            LDMA_Write2Byte(REG_CHKSUM_MD, LDMA_Read2Byte(REG_CHKSUM_MD)&(~REG_CHKSUM_MD_MSK));
            break;
        }
        case LDMA_CHECKSUM_CMD_DATA: {
            LDMA_Write2Byte(REG_CHKSUM_MD, BIT0);
            break;
        }
        case LDMA_CHECKSUM_DATA_ONLY: {
            LDMA_Write2Byte(REG_CHKSUM_MD,BIT1);
            break;
        }
        default: {
            DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("Oops, No Such CheckSum Mode. \n"));
        }
    }

    return TRUE;

}

MS_BOOL HAL_LDMA_SetTrigDelayCnt(MS_U8 u8Channel, MS_U16 *pu16DelayCnt)
{
    if(pu16DelayCnt == NULL)
    {
        DEBUG_LDMA(E_LDMA_DBGLV_ERR,printf("[LDMA]Fun:%s  Args Error!!!\n",__FUNCTION__));
        return FALSE;
    }

    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    LDMA_Write2Byte(REG_TRIG_DELAY0_CNT0,pu16DelayCnt[0]);
    LDMA_Write2Byte(REG_TRIG_DELAY1_CNT0,pu16DelayCnt[1]);
    LDMA_Write2Byte(REG_TRIG_DELAY1_CNT0,pu16DelayCnt[2]);
    LDMA_Write2Byte(REG_TRIG_DELAY1_CNT1,pu16DelayCnt[3]);
    return TRUE;
}

MS_BOOL HAL_LDMA_EnableCS(MS_U8 u8Channel, MS_BOOL bEnable)
{
    _HAL_LDMA_CheckAndSetBaseAddr(u8Channel);

    LDMA_Write2Byte(REG_TRIG_DELAY_CTRL, bEnable&BIT0);
    return TRUE;
}
