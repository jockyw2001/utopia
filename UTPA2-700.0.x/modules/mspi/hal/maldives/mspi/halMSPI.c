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

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsTypes.h"
#include "halMSPI.h"
#include "regMSPI.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MSP_READ(addr)                      READ_WORD(_hal_msp.u32MspBaseAddr + ((addr)<<2))
#define MSP_WRITE(addr, val)                WRITE_WORD(_hal_msp.u32MspBaseAddr + ((addr)<<2), (val))
#define CLK_READ(addr)                      READ_WORD(_hal_msp.u32ClkBaseAddr + ((addr)<<2))
#define CLK_WRITE(addr, val)                WRITE_WORD(_hal_msp.u32ClkBaseAddr + ((addr)<<2), (val))
#define _HAL_MSPI_Trigger()                 MSP_WRITE(MSPI_TRIGGER_OFFSET,MSPI_TRIGGER)
#define _HAL_MSPI_ClearDone()               MSP_WRITE(MSPI_DONE_CLEAR_OFFSET,MSPI_CLEAR_DONE)
#define MAX_CHECK_CNT                       2000
#define DEBUG_HAL_MSPI(debug_level, x)     do { if (_u8HalMSPIDbgLevel >= (debug_level)) (x); } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 u32MspBaseAddr;
    MS_U32 u32ClkBaseAddr;
} hal_msp_t;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_msp_t _hal_msp =
{
    .u32MspBaseAddr = BASEADDR_RIU + BK_MSP,
    .u32ClkBaseAddr = BASEADDR_RIU + BK_CLK0
};

static MS_U8 guChipSelect = 0;
MS_U8 _u8HalMSPIDbgLevel;
static MS_BOOL _gbMspiIntEnable;

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
/// Description : MSPI  interrupt enable
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
/// @prarm u32PMBankBaseAddr \b IN :base address of MMIO (PM domain)
/// @param u32NONPMRegBaseAddr \b IN :base address of MMIO
/// @param u8DeviceIndex \b IN: index of HW IP
//------------------------------------------------------------------------------
void HAL_MSPI_MMIOConfig(MS_U32 u32PMBankBaseAddr, MS_U32 u32NONPMRegBaseAddr, MS_U8 u8DeviceIndex)
{
    DEBUG_MSPI(E_MSPI_DBGLV_INFO,printf("%s(0x%08X)\n", __FUNCTION__, (int)u32NONPMRegBaseAddr));
    if(u8DeviceIndex)
        _hal_msp.u32MspBaseAddr = u32NONPMRegBaseAddr + BK_MSP1;
    else
        _hal_msp.u32MspBaseAddr = u32NONPMRegBaseAddr + BK_MSP;
    _hal_msp.u32ClkBaseAddr = u32NONPMRegBaseAddr + BK_CLK0;

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
	u32MaxClock = u32MaxClock* 1000000;
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

