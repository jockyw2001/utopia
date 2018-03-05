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
/// file    halMSPI.c
/// @brief  Master SPI Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "MsTypes.h"
#include "halMSPI.h"
#include "regMSPI.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MSP_READ(addr)                      READ_WORD(_hal_msp.VirtMspBaseAddr + ((addr)<<2))
#define MSP_WRITE(addr, val)                WRITE_WORD(_hal_msp.VirtMspBaseAddr + ((addr)<<2), (val))
#define CLK_READ(addr)                      READ_WORD(_hal_msp.VirtClkBaseAddr + ((addr)<<2))
#define CLK_WRITE(addr, val)                WRITE_WORD(_hal_msp.VirtClkBaseAddr + ((addr)<<2), (val))
#define _HAL_MSPI_Trigger()                 MSP_WRITE(MSPI_TRIGGER_OFFSET,MSPI_TRIGGER)
#define _HAL_MSPI_ClearDone()               MSP_WRITE(MSPI_DONE_CLEAR_OFFSET,MSPI_CLEAR_DONE)
#define MAX_CHECK_CNT                       2000
#define DEBUG_HAL_MSPI(debug_level, x)      do { if (_u8HalMSPIDbgLevel >= (debug_level)) (x); } while(0)

#define MSPI_CLK_INDEX_MAX   256
#define MSPI_CLK_P1_MAX      8
#define MSPI_CLK_P2_MAX      16
#define MSPI_CLK_DIV_INDEX_MAX 8
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_VIRT VirtMspBaseAddr;
    MS_VIRT VirtClkBaseAddr;
} hal_msp_t;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_msp_t _hal_msp =
{
    .VirtMspBaseAddr = BASEADDR_RIU + BK_MSP,
    .VirtClkBaseAddr = BASEADDR_RIU + BK_CLK0
};

static MS_U8 guChipSelect = 0;
MS_U8 _u8HalMSPIDbgLevel;
static MS_BOOL _gbMspiIntEnable;

static ST_DRV_MSPI_CLK clk_buffer[MSPI_CLK_INDEX_MAX];

static MS_U8 clk_spi_m_p1[8] = {E_MSPI_CLK_P1_27M, E_MSPI_CLK_P1_48M, E_MSPI_CLK_P1_62M, E_MSPI_CLK_P1_72M,
                                E_MSPI_CLK_P1_86M, E_MSPI_CLK_P1_108M, E_MSPI_CLK_P1_123M, E_MSPI_CLK_P1_160M};

static MS_U16 clk_spi_div[8] = {E_MSPI_CLK_P2_DIV2, E_MSPI_CLK_P2_DIV4,E_MSPI_CLK_P2_DIV8, E_MSPI_CLK_P2_DIV16,
                                E_MSPI_CLK_P2_DIV32, E_MSPI_CLK_P2_DIV64, E_MSPI_CLK_P2_DIV128, E_MSPI_CLK_P2_DIV256};

//-------------------------------------------------------------------------------------------------
// Local Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description : Set MSPI read /write buf size in current operation
/// @param Direct \b OUT: read /write operation direction
/// @param Size   \b OUT: size of read /write buffer
//------------------------------------------------------------------------------

static void _HAL_MSPI_RWBUFSize(MS_BOOL Direct, MS_U8 Size)
{
    MS_U16 u16Data = 0;
    u16Data = MSP_READ(MSPI_RBF_SIZE_OFFSET);

    if(Direct == MSPI_READ_INDEX)
    {
        u16Data &= MSPI_RWSIZE_MASK;
        u16Data |= Size << MSPI_RSIZE_BIT_OFFSET;
    }
    else
    {
        u16Data &= ~MSPI_RWSIZE_MASK;
        u16Data |= Size;
    }
    MSP_WRITE(MSPI_RBF_SIZE_OFFSET, u16Data);
}

//------------------------------------------------------------------------------
/// Description : SPI chip select enable and disable
/// @param Enable \b OUT: enable or disable chip select
//------------------------------------------------------------------------------
static void _HAL_MSPI_ChipSelect(MS_BOOL Enable)
{
    MS_U16 regdata = 0;
    MS_U8 bitmask = 0;
    regdata = MSP_READ(MSPI_CHIP_SELECT_OFFSET);
    if(Enable)
    {
        bitmask = ~(1 << guChipSelect);
        regdata &= bitmask;
    }
    else
    {
        bitmask = (1 << guChipSelect);
        regdata |= bitmask;
    }
    MSP_WRITE(MSPI_CHIP_SELECT_OFFSET, regdata);
}

//------------------------------------------------------------------------------
/// Description : check MSPI operation complete
/// @return TRUE :  operation complete
/// @return FAIL : failed timeout
//------------------------------------------------------------------------------
static MS_BOOL _HAL_MSPI_CheckDone(void)
{
    MS_U16 uCheckDoneCnt = 0;
    MS_U16 uDoneFlag = 0;
    while(uCheckDoneCnt < MAX_CHECK_CNT)
    {
        uDoneFlag = MSP_READ(MSPI_DONE_OFFSET);
        if(uDoneFlag & MSPI_DONE_FLAG)
            return TRUE;
        uCheckDoneCnt++;
    }
    DEBUG_MSPI(E_MSPI_DBGLV_ERR,printf("ERROR:MSPI Operation Timeout!!!!!\n"));
    return FALSE;
}

static MS_U32 _HAL_MSPI_GetCLK(MS_U16 u16CLK)
{
    MS_U32 u32MspiClk = 0;
    switch (u16CLK)
    {
    case 0:
        u32MspiClk = 27000000/2;
        return u32MspiClk;
    case 1:
        u32MspiClk = 48000000/2;
        return u32MspiClk;
    case 2:
        u32MspiClk = 62000000/2;
        return u32MspiClk;
    case 3:
        u32MspiClk = 72000000/2;
        return u32MspiClk;
    case 4:
        u32MspiClk = 86000000/2;
        return u32MspiClk;
    case 5:
        u32MspiClk = 108000000/2;
        return u32MspiClk;
    case 6:
        u32MspiClk = 123000000/2;
        return u32MspiClk;
    case 7:
        u32MspiClk = 160000000/2;
        return u32MspiClk;
    default:
        printf("ERROR CLOCK SETTING NOT SUPPORT \r\n");
        return u32MspiClk;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return void :
//------------------------------------------------------------------------------
void HAL_MSPI_Init(void)
{
    MS_U16 TempData;
    //init  MSP
    DEBUG_MSPI(E_MSPI_DBGLV_INFO,printf("HAL_MSPI_Init\n"));


    // CLK SETTING
    TempData = CLK_READ(MSPI_CLK_CFG);
    TempData &= !(MSPI_CLK_MASK);
    TempData |= MSPI_CLK_DEFAULT;
    CLK_WRITE(MSPI_CLK_CFG, TempData);

    if(_gbMspiIntEnable)
    {
        MSP_WRITE(MSPI_CTRL_OFFSET,(MSPI_INT_ENABLE|MSPI_RESET|MSPI_ENABLE));
    }
    else
    {
        MSP_WRITE(MSPI_CTRL_OFFSET,(MSPI_RESET|MSPI_ENABLE));
    }
}

//------------------------------------------------------------------------------
/// Description : MSPI    interrupt enable
/// @param bEnable \b OUT: enable or disable mspi interrupt
/// @return void:
//------------------------------------------------------------------------------
void HAL_MSPI_IntEnable(MS_BOOL bEnable)
{
    if(bEnable)
        _gbMspiIntEnable = TRUE;
    else
        _gbMspiIntEnable = FALSE;
}

//------------------------------------------------------------------------------
/// Description : Get Max of chip select
/// @param void:
/// @return Max of Chip select
//------------------------------------------------------------------------------
MS_U8 HAL_MSPI_ChipSelectMax(void)
{
    return MSPI_CHIP_SELECT_MAX;
}

//------------------------------------------------------------------------------
/// Description : Set MSPI chip select
/// @param u8CS \u8 OUT: MSPI chip select
/// @return void:
//------------------------------------------------------------------------------
void HAL_MSPI_SetChipSelect(MS_U8 u8CS)
{
    guChipSelect = u8CS;
}

//------------------------------------------------------------------------------
/// Description : Config MSP MMIO base address
/// @param u32PMBankBaseAddr \b IN:base address of MMIO (PM domain)
/// @param u32NONPMRegBaseAddr \b IN :base address of MMIO
/// @param u8DeviceIndex \b IN: index of HW IP
//------------------------------------------------------------------------------
void HAL_MSPI_MMIOConfig(MS_U32 u32PMBankBaseAddr, MS_U32 u32NONPMRegBaseAddr, MS_U8 u8DeviceIndex)
{
    DEBUG_MSPI(E_MSPI_DBGLV_INFO,printf("%s(0x%08X)\n", __FUNCTION__, (int)u32NONPMRegBaseAddr));
    if(u8DeviceIndex)
    {
        _hal_msp.VirtMspBaseAddr = u32NONPMRegBaseAddr + BK_MSP1;
        _hal_msp.VirtClkBaseAddr = u32NONPMRegBaseAddr + BK_CLK1;
    }
    else
    {
        _hal_msp.VirtMspBaseAddr = u32NONPMRegBaseAddr + BK_MSP;
        _hal_msp.VirtClkBaseAddr = u32NONPMRegBaseAddr + BK_CLK0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b IN :pointer to receive data from MSPI read buffer
/// @param u16Size \ b OTU : read data size
/// @return TRUE  : read data success
/// @return FALSE : read data fail
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Read(MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8  u8Index = 0;
	MS_U16  u16TempBuf = 0;
    MS_U16 i =0, j = 0;

    for(i = 0; i < u16Size; i+= MAX_READ_BUF_SIZE)
    {
        u16TempBuf = u16Size - i;
        if(u16TempBuf > MAX_READ_BUF_SIZE)
        {
            j = MAX_READ_BUF_SIZE;
        }
        else
        {
            j = u16TempBuf;
        }
        _HAL_MSPI_RWBUFSize(MSPI_READ_INDEX, j);

        HAL_MSPI_Trigger();
        DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("Read Size %x\n",j));
        for(u8Index = 0; u8Index < j; u8Index++)
        {

            if(u8Index & 1)
            {
                u16TempBuf = MSP_READ((MSPI_READ_BUF_OFFSET + (u8Index >> 1)));
                DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("read Buf data %x index %d\n",u16TempBuf, u8Index));
                pData[u8Index] = u16TempBuf >> 8;
                pData[u8Index-1] = u16TempBuf & 0xFF;
            }
            else if(u8Index == (j -1))
            {
                u16TempBuf = MSP_READ((MSPI_READ_BUF_OFFSET + (u8Index >> 1)));
                DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("read Buf data %x index %d\n",u16TempBuf, u8Index));
                pData[u8Index] = u16TempBuf & 0xFF;
            }
        }
        pData+= j;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b OUT :pointer to write  data to MSPI write buffer
/// @param u16Size \ b OTU : write data size
/// @return TRUE  : write data success
/// @return FALSE : wirte data fail
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Wirte(MS_U8 *pData, MS_U16 u16Size)
{
    MS_U8  u8Index = 0;
    MS_U16 u16TempBuf = 0;
    MS_U16 i =0, j = 0;
    for(i = 0; i < u16Size; i+= MAX_WRITE_BUF_SIZE)
    {
        u16TempBuf = u16Size - i;
        if(u16TempBuf > MAX_WRITE_BUF_SIZE)
        {
            j = MAX_WRITE_BUF_SIZE;
        }
        else
        {
            j = u16TempBuf;
        }
        DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("Write Size %x\n",j));
        for(u8Index = 0; u8Index < j; u8Index++)
        {
            if(u8Index & 1)
            {
                u16TempBuf = (pData[u8Index] << 8) | pData[u8Index-1];
                DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("write Buf data %x index %d\n",u16TempBuf, u8Index));
                MSP_WRITE((MSPI_WRITE_BUF_OFFSET + (u8Index >> 1)),u16TempBuf);
            }
            else if(u8Index == (j -1))
            {
                DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("write Buf data %x index %d\n",pData[u8Index], u8Index));
                MSP_WRITE((MSPI_WRITE_BUF_OFFSET + (u8Index >> 1)),pData[u8Index]);
            }
        }
        pData += j;
        _HAL_MSPI_RWBUFSize(MSPI_WRITE_INDEX, j);
        HAL_MSPI_Trigger();
    }
        // set write data size

    return TRUE;
}

//------------------------------------------------------------------------------
/// Description : Reset  DC register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_DCConfig(void)
{
	//DC reset
    MSP_WRITE(MSPI_DC_TR_START_OFFSET, 0x00);
    MSP_WRITE(MSPI_DC_TB_OFFSET, 0x00);
	return TRUE;
}

//------------------------------------------------------------------------------
/// Description : Reset  Frame register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_FrameConfig(void)
{
    // Frame reset
    MSP_WRITE(MSPI_FRAME_WBIT_OFFSET, 0xFFF);
	MSP_WRITE(MSPI_FRAME_WBIT_OFFSET+2, 0xFFF);
	MSP_WRITE(MSPI_FRAME_RBIT_OFFSET, 0xFFF);
	MSP_WRITE(MSPI_FRAME_RBIT_OFFSET+2, 0xFFF);
	return TRUE;
}

//------------------------------------------------------------------------------
/// Description : Reset  CLK register setting of MSPI
/// @param NONE
/// @return TRUE  : reset complete
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Reset_CLKConfig(void)
{
    MS_U16 Tempbuf;
	//reset clock
    Tempbuf = MSP_READ(MSPI_CTRL_OFFSET);
    Tempbuf &= 0x3F;
    MSP_WRITE(MSPI_CTRL_OFFSET, Tempbuf);
	return TRUE;
}

//------------------------------------------------------------------------------
/// Description : get read/write buffer size
/// @param bDirection    \b OUT   specify to get read/write buffer size
/// @return buffer sizel
//------------------------------------------------------------------------------
MS_U8 HAL_MSPI_GetBufSize(MS_BOOL bDirection)
{
    if(bDirection == MSPI_READ_INDEX)
        return MAX_READ_BUF_SIZE;
    else
        return MAX_WRITE_BUF_SIZE;
}

//------------------------------------------------------------------------------
/// Description : Trigger MSPI operation
/// @return TRUE  : operation success
/// @return FALSE : operation timeout
//------------------------------------------------------------------------------
MS_BOOL HAL_MSPI_Trigger(void)
{
    // chip select enable
  //  _HAL_MSPI_ChipSelect(TRUE);

    // trigger operation
    _HAL_MSPI_Trigger();
    // check operation complete
    if(!_HAL_MSPI_CheckDone())
        return FALSE;
    // clear done flag
    _HAL_MSPI_ClearDone();
    // chip select disable
   // _HAL_MSPI_ChipSelect(FALSE);
    // reset read/write buffer size
    MSP_WRITE(MSPI_RBF_SIZE_OFFSET,0x0);
	return TRUE;
}

void HAL_MSPI_SlaveEnable(MS_BOOL Enable)
{
     _HAL_MSPI_ChipSelect(Enable);
}

//------------------------------------------------------------------------------
/// Description : get valid range of DC timing
/// @param eDCField    \b OUT enum of DC timing type
/// @return NONE
//------------------------------------------------------------------------------
MS_U8 HAL_MSPI_DCConfigMax(eDC_config eDCField)
{
    switch(eDCField)
    {
    case E_MSPI_TRSTART:
        return MSPI_DC_TRSTART_MAX;
    case E_MSPI_TREND:
        return MSPI_DC_TREND_MAX;
    case E_MSPI_TB:
        return MSPI_DC_TB_MAX;
    case E_MSPI_TRW:
        return MSPI_DC_TRW_MAX;
    default:
        return 0;
    }

}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return NONE
//------------------------------------------------------------------------------
void HAL_MSPI_SetDcTiming (eDC_config eDCField, MS_U8 u8DCtiming)
{
   MS_U16 u16TempBuf = 0;
   switch(eDCField)
   {
   case E_MSPI_TRSTART:
       u16TempBuf = MSP_READ(MSPI_DC_TR_START_OFFSET);
       u16TempBuf &= (~MSPI_DC_MASK);
       u16TempBuf |= u8DCtiming;
       MSP_WRITE(MSPI_DC_TR_START_OFFSET, u16TempBuf);
       break;
   case E_MSPI_TREND:
       u16TempBuf = MSP_READ(MSPI_DC_TR_END_OFFSET);
       u16TempBuf &= MSPI_DC_MASK;
       u16TempBuf |= u8DCtiming << MSPI_DC_BIT_OFFSET;
       MSP_WRITE(MSPI_DC_TR_END_OFFSET, u16TempBuf);
       break;
   case E_MSPI_TB:
       u16TempBuf = MSP_READ(MSPI_DC_TB_OFFSET);
       u16TempBuf &= (~MSPI_DC_MASK);
       u16TempBuf |= u8DCtiming;
       MSP_WRITE(MSPI_DC_TB_OFFSET, u16TempBuf);
       break;
   case E_MSPI_TRW:
       u16TempBuf = MSP_READ(MSPI_DC_TRW_OFFSET);
       u16TempBuf &= MSPI_DC_MASK;
       u16TempBuf |= u8DCtiming << MSPI_DC_BIT_OFFSET;
       MSP_WRITE(MSPI_DC_TRW_OFFSET, u16TempBuf);
       break;
   }
}

MS_U8 HAL_MSPI_CLKConfigMax(eCLK_config eCLKField)
{
    switch(eCLKField)
    {
    case E_MSPI_POL:
        return MSPI_CLK_POLARITY_MAX;
    case E_MSPI_PHA:
        return MSPI_CLK_PHASE_MAX;
    case E_MSPI_CLK:
        return MSPI_CLK_CLOCK_MAX;
    default:
        return 0;
    }
}

void HAL_MSPI_SetCLKTiming(eCLK_config eCLKField, MS_U8 u8CLKVal)
{
   MS_U16 u16TempBuf = 0;
   switch(eCLKField)
   {
   case E_MSPI_POL:
       u16TempBuf = MSP_READ(MSPI_CLK_CLOCK_OFFSET);
       u16TempBuf &= ~(MSPI_CLK_POLARITY_MASK);
       u16TempBuf |= u8CLKVal << MSPI_CLK_POLARITY_BIT_OFFSET;
       break;
   case E_MSPI_PHA:
       u16TempBuf = MSP_READ(MSPI_CLK_CLOCK_OFFSET);
       u16TempBuf &= ~(MSPI_CLK_PHASE_MASK);
       u16TempBuf |= u8CLKVal << MSPI_CLK_PHASE_BIT_OFFSET;
       break;
   case E_MSPI_CLK:
       u16TempBuf = MSP_READ(MSPI_CLK_CLOCK_OFFSET);
       u16TempBuf &= MSPI_CLK_CLOCK_MASK;
       u16TempBuf |= u8CLKVal << MSPI_CLK_CLOCK_BIT_OFFSET;
       break;
   }
   MSP_WRITE(MSPI_CLK_CLOCK_OFFSET, u16TempBuf);
}

MS_U8 HAL_MSPI_FrameConfigMax(void)
{
    return MSPI_FRAME_BIT_MAX;
}

void HAL_MSPI_SetPerFrameSize(MS_BOOL bDirect, MS_U8 u8BufOffset, MS_U8 u8PerFrameSize)
{
    MS_U8 u8Index = 0;
    MS_U16 u16TempBuf = 0;
    MS_U8 u8BitOffset = 0;
    MS_U16 u16regIndex = 0;
    if(bDirect == MSPI_READ_INDEX)
    {
        u16regIndex = MSPI_FRAME_RBIT_OFFSET;
    }
    else
    {
        u16regIndex = MSPI_FRAME_WBIT_OFFSET;
    }
    if(u8BufOffset >=4)
    {
        u8Index++;
        u8BufOffset -= 4;
    }
    u8BitOffset = u8BufOffset * MSPI_FRAME_BIT_FIELD;
    u16TempBuf = MSP_READ(u16regIndex+ u8Index);
    u16TempBuf &= ~(MSPI_FRAME_BIT_MASK << u8BitOffset);
    u16TempBuf |= u8PerFrameSize << u8BitOffset;
    MSP_WRITE((u16regIndex + u8Index), u16TempBuf);
}

MS_BOOL HAL_MSPI_CLOCK_Config(MS_U32 u32MaxClock)
{
    MS_U16 u16ClkDiv = 0;
	MS_U16 u16ClkLevel = 0;
	MS_U16 u16TempDiv = 0;
	MS_U32 u32Clock = 0;
	MS_U32 u32TempClock = 0;
	MS_U32 u32ClockMatch = 0;
	MS_U8  u8ClkIdx = 0;
	MS_U8  u8DivIdx = 0;
	u32MaxClock = u32MaxClock * 1000000;
    // get current clock

    for(u8ClkIdx = 0; u8ClkIdx < MSPI_MAXCLKLEVEL; u8ClkIdx++)
    {
        u32Clock = _HAL_MSPI_GetCLK(u8ClkIdx);
        if(u32Clock < u32MaxClock && u32Clock > u32ClockMatch)
        {
           u32ClockMatch = u32Clock;
           u16ClkLevel = u8ClkIdx;
        }
        else if(u32Clock >= u32MaxClock )
        {
            if(u32Clock == u32MaxClock)
                break;
            else
            {
                for(u8DivIdx = 1; u8DivIdx < MSPI_CLK_CLOCK_MAX; u8DivIdx++)
                {
                    u32TempClock = u32Clock / (1 << (u8DivIdx + 1));
                    if(u32TempClock <= u32MaxClock)
                    {
                        if(u32TempClock > u32ClockMatch)
                        {
                            u32ClockMatch = u32TempClock;
                            u16ClkDiv = u8DivIdx;
                            u16ClkLevel = u8ClkIdx;
                        }
                        break;
                    }
                }
            }
        }
    }
    //set clock div of mspi
    u16TempDiv = MSP_READ(MSPI_CLK_CLOCK_OFFSET);
    u16TempDiv &= MSPI_CLK_CLOCK_MASK;
    u16TempDiv |= u16ClkDiv << MSPI_CLK_CLOCK_BIT_OFFSET;
    MSP_WRITE(MSPI_CLK_CLOCK_OFFSET, u16TempDiv);
    u16ClkLevel = u16ClkLevel << 2;
    CLK_WRITE(MSPI_CLK_CFG, u16ClkLevel);
    return TRUE;
}


MS_BOOL HAL_MSPI_CLK_Config(MS_U8 u8Chanel,MS_U32 u32MspiClk)
{
    MS_U8 i = 0;
    MS_U8 j = 0;
    MS_U8 k= 0;
    MS_U16 TempData = 0;
    MS_U32 clk =0;
    ST_DRV_MSPI_CLK temp;
    if(u8Chanel >=2)
        return FALSE;
    memset(&temp,0,sizeof(ST_DRV_MSPI_CLK));
    memset(&clk_buffer,0,sizeof(ST_DRV_MSPI_CLK)*MSPI_CLK_INDEX_MAX);
    for(i = 0;i < MSPI_CLK_P1_MAX;i++)//clk_spi_m_p1
    {
        for(j =0 ;j < MSPI_CLK_P2_MAX;j++)//clk_spi_m_p2
        {
            for(k = 0;k < MSPI_CLK_DIV_INDEX_MAX;k++)//spi div
            {
                clk = clk_spi_m_p1[i]*1000000;
                clk_buffer[k+MSPI_CLK_P1_MAX*j+MSPI_CLK_P2_MAX*i].u8ClkSpi_P1 = i;
                clk_buffer[k+MSPI_CLK_P1_MAX*j+MSPI_CLK_P2_MAX*i].u8ClkSpi_P2 = j;
                clk_buffer[k+MSPI_CLK_P1_MAX*j+MSPI_CLK_P2_MAX*i].u8ClkSpi_DIV = k ;
                clk_buffer[k+MSPI_CLK_P1_MAX*j+MSPI_CLK_P2_MAX*i].u32ClkSpi = clk/((j+1)*(clk_spi_div[k]));
            }
        }
    }
    for(i = 0;i<MSPI_CLK_INDEX_MAX;i++)
    {
        for(j = i;j<MSPI_CLK_INDEX_MAX;j++)
        {
            if(clk_buffer[i].u32ClkSpi > clk_buffer[j].u32ClkSpi)
            {
                memcpy(&temp,&clk_buffer[i],sizeof(ST_DRV_MSPI_CLK));

                memcpy(&clk_buffer[i],&clk_buffer[j],sizeof(ST_DRV_MSPI_CLK));

                memcpy(&clk_buffer[j],&temp,sizeof(ST_DRV_MSPI_CLK));
            }
        }
    }
    for(i = 0;i<MSPI_CLK_INDEX_MAX;i++)
    {
        if(u32MspiClk <= clk_buffer[i].u32ClkSpi)
        {
            break;
        }
    }
    //match Closer clk
    if((i>0)&&(i<MSPI_CLK_INDEX_MAX))
    {
      if((u32MspiClk - clk_buffer[i-1].u32ClkSpi)<(clk_buffer[i].u32ClkSpi - u32MspiClk))
      {
        i -= 1;
      }
    }
    else
    {
       DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("Wrong Index \n"));
       return FALSE;
    }
    DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("[Lwc Debug] u8ClkSpi_P1 =%d\n",clk_buffer[i].u8ClkSpi_P1));
    DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("[Lwc Debug] u8ClkSpi_P2 =%d\n",clk_buffer[i].u8ClkSpi_P2));
    DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("[Lwc Debug] u8ClkSpi_DIV =%d\n",clk_buffer[i].u8ClkSpi_DIV));
    DEBUG_MSPI(E_MSPI_DBGLV_DEBUG,printf("[Lwc Debug] u32ClkSpi = %x\n",clk_buffer[i].u32ClkSpi));

    if(u8Chanel == 0)//mspi0
    {
        TempData = CLK_READ(MSPI_CLK_CFG);
        TempData &= ~(MSPI_CLK_MASK);
        TempData |= clk_buffer[i].u8ClkSpi_P1 << 2;
        CLK_WRITE(MSPI_CLK_CFG, TempData);

        TempData = CLK_READ(MSPI_CLK_DIV);
        TempData &= ~(MSPI_CLK_DIV_MASK);
        TempData |= clk_buffer[i].u8ClkSpi_P2;
        CLK_WRITE(MSPI_CLK_DIV, TempData);
    }
    else//mspi1
    {
        TempData = CLK_READ(MSPI1_CLK_CFG);
        TempData &= ~(MSPI1_CLK_MASK);
        TempData |= clk_buffer[i].u8ClkSpi_P1 << 10;
        CLK_WRITE(MSPI1_CLK_CFG, TempData);

        TempData = CLK_READ(MSPI1_CLK_DIV);
        TempData &= ~(MSPI1_CLK_DIV_MASK);
        TempData |= clk_buffer[i].u8ClkSpi_P2;
        CLK_WRITE(MSPI1_CLK_DIV, TempData);
    }
    //set DIV for mspi at 153A bank 0x49(16bit)
    TempData = MSP_READ(MSPI_CLK_CLOCK_OFFSET);
    TempData &= MSPI_CLK_CLOCK_MASK;
    TempData |= clk_buffer[i].u8ClkSpi_DIV << MSPI_CLK_CLOCK_BIT_OFFSET;
    MSP_WRITE(MSPI_CLK_CLOCK_OFFSET, TempData);

    return TRUE;
}

MS_BOOL HAL_MSPI_RWBytes(MS_BOOL Direct, MS_U8 u8Bytes)
{
    //set read write trigger buffer size
    _HAL_MSPI_RWBUFSize(Direct, u8Bytes);
    return TRUE;
}
