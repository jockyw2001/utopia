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
#include "drvMSPI.h"
#include "MsOS.h"

#include "ULog.h"

// Internal Definition
#include "regMSPI.h"
#include "halMSPI.h"
#include "drvMMIO.h"
#include "utopia.h"
#include "drvGPIO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
MS_U8 gu8MSPIConfig;
MS_U8 gu8MSPICurConfig[MSPI_CMD_TYPE];
MS_BOOL gbInitFlag = FALSE;
MS_BOOL gbInitInUtopia = FALSE;

MS_U8 _u8MSPIDbgLevel;
static MS_U32 _gu32CsPad = 0xFF;
static MS_U32 _gu32DevID = 0xFF;
static MS_U32 _gu32MaxClk = 0;
static MS_S32 _gs32MSPI_Mutex;
//MSPI mutex wait time
#define MSPI_MUTEX_WAIT_TIME    3000

#define TAG_MSPI "MSPI"

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define DEBUG_MSPI(debug_level, x)     do { if (_u8MSPIDbgLevel >= (debug_level)) (x); } while(0)


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// Description : Set Chip Select
/// @return E_MSPI_OK :
/// @return >1 : failed to set Chip select
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_ChipSelectSetting(MS_U8 u8Cs)
{
    MS_U8 u8CSMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8CSMax = HAL_MSPI_ChipSelectMax();
    if(u8Cs > u8CSMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetChipSelect(u8Cs);
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Set TrStart timing of DC config
/// @return E_MSPI_OK :
/// @return >1 : failed to set TrStart timing
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_DC_TrStartSetting(MS_U8 TrStart)
{
    MS_U8 u8TrStartMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TrStartMax = HAL_MSPI_DCConfigMax(E_MSPI_TRSTART);
    if(TrStart > u8TrStartMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TRSTART ,TrStart);
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Set TrEnd timing of DC config
/// @return E_MSPI_OK :
/// @return >1 : failed to set TrEnd timing
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_DC_TrEndSetting(MS_U8 TrEnd)
{
    MS_U8 u8TrEndMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TrEndMax = HAL_MSPI_DCConfigMax(E_MSPI_TREND);
    if(TrEnd > u8TrEndMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TREND ,TrEnd);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set TB timing of DC config
/// @return E_MSPI_OK :
/// @return >1 : failed to set TB timing
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_DC_TBSetting(MS_U8 TB)
{
    MS_U8 u8TBMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TBMax = HAL_MSPI_DCConfigMax(E_MSPI_TB);
    if(TB > u8TBMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TB ,TB);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set TRW timing of DC config
/// @return E_MSPI_OK :
/// @return >1 : failed to set TRW timging
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_DC_TRWSetting(MS_U8 TRW)
{
    MS_U8 u8TRWMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8TRWMax = HAL_MSPI_DCConfigMax(E_MSPI_TRW);
    if(TRW > u8TRWMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetDcTiming(E_MSPI_TRW ,TRW);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock polarity of MSPI
/// @return E_MSPI_OK :
/// @return >1 : failed to set clock polarity
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_CLK_PolaritySetting(MS_U8 u8Pol)
{
    MS_U8 u8PolarityMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8PolarityMax = HAL_MSPI_CLKConfigMax(E_MSPI_POL);
    if(u8Pol > u8PolarityMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_POL ,u8Pol);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock phase of MSPI
/// @return E_MSPI_OK :
/// @return >1 : failed to set clock phase
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_CLK_PhaseSetting(MS_U8 u8Pha)
{
    MS_U8 u8PhaseMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8PhaseMax = HAL_MSPI_CLKConfigMax(E_MSPI_PHA);
    if(u8Pha > u8PhaseMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_PHA ,u8Pha);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set clock rate of MSPI
/// @return E_MSPI_OK :
/// @return >1 : failed to set clock rate
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_CLK_ClockSetting(MS_U8 u8Clock)
{
    MS_U8 u8ClockMax;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8ClockMax = HAL_MSPI_CLKConfigMax(E_MSPI_CLK);
    if(u8Clock > u8ClockMax)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetCLKTiming(E_MSPI_CLK ,u8Clock);
    return errnum;

}

//------------------------------------------------------------------------------
/// Description : Set transfer bit pre frame for read/write buffer
/// @return E_MSPI_OK :
/// @return >1 : failed to check paramter
//------------------------------------------------------------------------------
static MSPI_ErrorNo _MDrv_Frame_BitSetting(MS_BOOL bDirect, MS_U8 u8Index, MS_U8 u8BitPerFrame)
{
    MS_U8 u8MAxBitPerFrame;
    MSPI_ErrorNo errnum = E_MSPI_OK;

    u8MAxBitPerFrame = HAL_MSPI_FrameConfigMax();
    if(u8BitPerFrame > u8MAxBitPerFrame)
        errnum = E_MSPI_PARAM_OVERFLOW;
    else
        HAL_MSPI_SetPerFrameSize(bDirect,  u8Index, u8BitPerFrame);
    return errnum;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Description : Set detailed level of MSPI driver debug message
/// @param u8DbgLevel    \b IN  debug level for Serial Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8MSPIDbgLevel = u8DbgLevel;

    return TRUE;
}
//------------------------------------------------------------------------------
/// Description : MSPI initial Ext
/// @return E_MSPI_OK :
/// @return >1 : failed to initial
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Init_Ext(MS_U8 u8HWNum)
{
    MS_VIRT VirtNONPMBank;
    MS_PHY  u32NONPMBankSize;
    MS_VIRT VirtPMBank;
    MS_PHY  u32PMBankSize;

    MSPI_ErrorNo errorno = E_MSPI_OK;
    if(gbInitFlag == TRUE)
    {
        ULOGE(TAG_MSPI, "MSPI Driver Already init\r\n");
        return E_MSPI_INIT_FLOW_ERROR;
    }

    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE;
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }

    if (!MDrv_MMIO_GetBASE( &VirtPMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_PM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_INFO, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
        return E_MSPI_MMIO_ERROR;
    }
    //DEBUG_MSPI(E_MSPI_DBGLV_INFO, ULOGI(TAG_MSPI, "IOMap address PM %x NONPM %x\n",VirtPMBank, VirtNONPMBank));

    HAL_MSPI_MMIOConfig(VirtPMBank, VirtNONPMBank, u8HWNum);

    memset(gu8MSPICurConfig, 0, sizeof(gu8MSPICurConfig));
    // default use CS0 for slave device
    errorno = _MDrv_ChipSelectSetting(0);
    //default use polling mode
    HAL_MSPI_IntEnable(0);
    HAL_MSPI_Init();
    if(_gu32MaxClk)
        HAL_MSPI_CLOCK_Config(_gu32MaxClk);
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, ULOGI(TAG_MSPI, " MSPI Init complete\n"));
    gbInitFlag = TRUE;
    gbInitInUtopia = TRUE;
    //default clock setting
    errorno = _MDrv_CLK_PolaritySetting(0);
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, ULOGI(TAG_MSPI, "PolaritySetting complete\n"));
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    errorno = _MDrv_CLK_PhaseSetting(0);
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    errorno = _MDrv_CLK_ClockSetting(0);
    if(errorno != E_MSPI_OK)
        goto ERROR_HANDLE;
    return E_MSPI_OK;
ERROR_HANDLE:
    errorno |= E_MSPI_INIT_FLOW_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI CLKconfig error errno =%d\n",errorno));
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK :
/// @return >1 : failed to initial
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
    MS_VIRT VirtNONPMBank;
    MS_PHY  u32NONPMBankSize;
    MS_VIRT VirtPMBank;
    MS_PHY  u32PMBankSize;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }
    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE;

    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    if (!MDrv_MMIO_GetBASE( &VirtPMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_PM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
    }
    if(u8HWNum > 2)
        return E_MSPI_PARAM_OVERFLOW;
    HAL_MSPI_MMIOConfig(VirtPMBank, VirtNONPMBank, u8HWNum);

    if(tMSPIConfig->BIntEnable)
    {
        //register IRQ handler
    }
    memcpy(gu8MSPICurConfig, tMSPIConfig->U8BitMapofConfig, sizeof(gu8MSPICurConfig));

    errorno = _MDrv_ChipSelectSetting(tMSPIConfig->U8ChipSel);
    if(errorno != E_MSPI_OK)
        return errorno;
    HAL_MSPI_IntEnable(tMSPIConfig->BIntEnable);
    HAL_MSPI_Init();
    gbInitFlag = TRUE;
    gbInitInUtopia = TRUE;
    return errorno;
}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b IN :pointer to receive data from MSPI read buffer
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Read(MS_U8 *pData, MS_U16 u16Size)
{
    MSPI_ErrorNo errorno = E_MSPI_OK;

    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    // check config error
    if(!(gu8MSPIConfig & MSPI_DC_CONFIG))
        errorno |= E_MSPI_DCCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_CLK_CONFIG))
        errorno |= E_MSPI_CLKCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_FRAME_CONFIG))
        errorno |= E_MSPI_FRAMECONFIG_ERROR;
    if(errorno != E_MSPI_OK)
    {
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "Read Operation MSPI config error %d\n",errorno));
    }

    HAL_MSPI_Read(pData, u16Size);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param pData \b OUT :pointer to write  data to MSPI write buffer
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Write(MS_U8 *pData, MS_U16 u16Size)
{
    MSPI_ErrorNo errorno = E_MSPI_OK;

    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    // check config error
    if(!(gu8MSPIConfig & MSPI_DC_CONFIG))
        errorno |= E_MSPI_DCCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_CLK_CONFIG))
        errorno |= E_MSPI_CLKCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_FRAME_CONFIG))
        errorno |= E_MSPI_FRAMECONFIG_ERROR;
    if(errorno != E_MSPI_OK)
    {
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "Write Operation MSPI config error %d\n",errorno));
    }

    // write operation
    HAL_MSPI_Wirte(pData, u16Size);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_DCConfig(MSPI_DCConfig *ptDCConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptDCConfig == NULL)
    {
        HAL_MSPI_Reset_DCConfig();
        return E_MSPI_OK;
    }
    errnum = _MDrv_DC_TrStartSetting(ptDCConfig->u8TrStart);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TrEndSetting(ptDCConfig->u8TrEnd);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TBSetting(ptDCConfig->u8TB);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_DC_TRWSetting(ptDCConfig->u8TRW);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    gu8MSPIConfig |= MSPI_DC_CONFIG;
    return E_MSPI_OK;

ERROR_HANDLE:
    errnum |= E_MSPI_DCCONFIG_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI DCconfig error errno =%d\n",errnum));
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_CLKConfig(MSPI_CLKConfig *ptCLKConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptCLKConfig == NULL)
    {
        HAL_MSPI_Reset_CLKConfig();
        return E_MSPI_OK;
    }

    errnum = _MDrv_CLK_PolaritySetting(ptCLKConfig->BClkPolarity);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_CLK_PhaseSetting(ptCLKConfig->BClkPhase);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    errnum = _MDrv_CLK_ClockSetting(ptCLKConfig->U8Clock);
    if(errnum != E_MSPI_OK)
        goto ERROR_HANDLE;
    gu8MSPIConfig |= MSPI_CLK_CONFIG;
    return E_MSPI_OK;

ERROR_HANDLE:
    errnum |= E_MSPI_CLKCONFIG_ERROR;
    DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI CLKconfig error errno =%d\n",errnum));
    return errnum;

}


//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_FRAMEConfig(MSPI_FrameConfig *ptFrameConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_U8 u8Index = 0;
    MS_U8 u8BufSize;

    //check init
    if(!gbInitFlag)
        return E_MSPI_INIT_FLOW_ERROR;

    if(ptFrameConfig == NULL)
    {
        HAL_MSPI_Reset_FrameConfig();
        return E_MSPI_OK;
    }
    // read buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8RBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_READ_INDEX, u8Index, ptFrameConfig->u8RBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
             DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI FRAMEconfig error errno =%d\n",errnum));
            return errnum;
        }
    }
    //write buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8WBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_WRITE_INDEX, u8Index, ptFrameConfig->u8WBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI FRAMEconfig error errno =%d\n",errnum));
            return errnum;
        }
    }

    gu8MSPIConfig |= MSPI_FRAME_CONFIG;
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Enable Slave device
//------------------------------------------------------------------------------
void MDrv_MSPI_SlaveEnable(MS_BOOL Enable)
{
    HAL_MSPI_SlaveEnable(Enable);
    if(_gu32CsPad != 0xFF)
    {
        if(Enable)
        {
            mdrv_gpio_set_low(_gu32CsPad);
        }
        else
        {
            mdrv_gpio_set_high(_gu32CsPad);
        }
    }
}

//------------------------------------------------------------------------------
/// Description :
/// @return TRUE : chip support
/// @return FALSE:
//------------------------------------------------------------------------------
MS_BOOL MDrv_MSPI_HW_Support(void)
{
    return HAL_MSPI_HW_Support();
}

//-------------------------------------------------------
// Description : MSPI Power state
//-------------------------------------------------------
MS_U32 MDrv_MSPI_SetPowerState(EN_POWER_MODE enPowerState)
{
    static EN_POWER_MODE enPreBDMAPowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
   if(!gbInitInUtopia)
    {
        return E_MSPI_INIT_FLOW_ERROR;
    }
    if (enPowerState == E_POWER_SUSPEND)
    {
        enPreBDMAPowerState = enPowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        gbInitFlag = FALSE;

    }
    else if (enPowerState == E_POWER_RESUME)
    {
        if (enPreBDMAPowerState == E_POWER_SUSPEND)
        {
            MDrv_MSPI_Init_Ext(0);

            enPreBDMAPowerState = enPowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_MSPI, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        ULOGE(TAG_MSPI, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,enPowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}

//------------------------------------------------------------------------------
/// Description : MSPI initial
/// @return E_MSPI_OK :
/// @return >1 : failed to initial
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Init(MSPI_config *tMSPIConfig, MS_U8 u8HWNum)
{
    MS_VIRT VirtNONPMBank;
    MS_PHY  u32NONPMBankSize;
    MS_VIRT VirtPMBank;
    MS_PHY  u32PMBankSize;
    MSPI_ErrorNo errorno = E_MSPI_OK;
    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }

    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    _u8MSPIDbgLevel = E_MSPI_DBGLV_NONE;

    if (!MDrv_MMIO_GetBASE( &VirtPMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_PM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
    }
    if(u8HWNum > 2)
        return E_MSPI_PARAM_OVERFLOW;
    HAL_MSPI_MMIOConfig(VirtPMBank, VirtNONPMBank, u8HWNum);

    if(tMSPIConfig->BIntEnable)
    {
        //register IRQ handler
    }
    memcpy(gu8MSPICurConfig, tMSPIConfig->U8BitMapofConfig, sizeof(gu8MSPICurConfig));

    errorno = _MDrv_ChipSelectSetting(tMSPIConfig->U8ChipSel);
    if(errorno != E_MSPI_OK)
        return errorno;
    HAL_MSPI_IntEnable(tMSPIConfig->BIntEnable);
    HAL_MSPI_Init();
    gbInitFlag = TRUE;
    return errorno;
}

//-------------------------------------------------------------------------------------------------
/// Description : read data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b IN :pointer to receive data from MSPI read buffer
/// @param u16Size \ b OTU : read data size
/// @return the errorno of operation
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Read(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
    MSPI_ErrorNo errorno = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_Read_return;
    }
    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_Read_return;
    }

    // check config error
    if(!(gu8MSPIConfig & MSPI_DC_CONFIG))
        errorno |= E_MSPI_DCCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_CLK_CONFIG))
        errorno |= E_MSPI_CLKCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_FRAME_CONFIG))
        errorno |= E_MSPI_FRAMECONFIG_ERROR;
    if(errorno != E_MSPI_OK)
    {
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "Read Operation MSPI config error %d\n",errorno));
    }

    HAL_MSPI_Read(pData, u16Size);
MDrv_MasterSPI_Read_return:
	MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : write data from MSPI
/// @param u32DevID IN: device id of slave device
/// @param pData \b OUT :pointer to write  data to MSPI write buffer
/// @param u16Size \ b OTU : write data size
/// @return the errorno of operation
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_Write(MS_U32 u32DevID, MS_U8 *pData, MS_U16 u16Size)
{
    MSPI_ErrorNo errorno = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_Write_return;
    }

    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errorno = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_Write_return;
    }

    // check config error
    if(!(gu8MSPIConfig & MSPI_DC_CONFIG))
        errorno |= E_MSPI_DCCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_CLK_CONFIG))
        errorno |= E_MSPI_CLKCONFIG_ERROR;
    if(!(gu8MSPIConfig & MSPI_FRAME_CONFIG))
        errorno |= E_MSPI_FRAMECONFIG_ERROR;
    if(errorno != E_MSPI_OK)
    {
        // reset config
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "Write Operation MSPI config error %d\n",errorno));
    }

    // write operation
    HAL_MSPI_Wirte(pData, u16Size);
MDrv_MasterSPI_Write_return:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errorno;
}

//------------------------------------------------------------------------------
/// Description : config spi transfer timing DCConfig==Delay Clock Config
/// @param u32DevID      \b IN device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of transfer timing config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_DCCONFIG_ERROR : failed to config transfer timing
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_DCConfig(MS_U32 u32DevID, MSPI_DCConfig *ptDCConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }
    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum =  E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }

    if(ptDCConfig == NULL)
    {
        HAL_MSPI_Reset_DCConfig();
        goto MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TrStartSetting(ptDCConfig->u8TrStart);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TrEndSetting(ptDCConfig->u8TrEnd);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TBSetting(ptDCConfig->u8TB);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }
    errnum = _MDrv_DC_TRWSetting(ptDCConfig->u8TRW);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_DCCONFIG_ERROR;
        goto MDrv_MasterSPI_DCConfig_return;
    }
    gu8MSPIConfig |= MSPI_DC_CONFIG;

MDrv_MasterSPI_DCConfig_return:

    DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI DCconfig error errno =%d\n",errnum));
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : config spi clock setting
/// @param u32DevID       \b IN  device id of slave device
/// @param ptCLKConfig    \b OUT  struct pointer of clock config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_CLKCONFIG_ERROR : failed to config spi clock
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_CLKConfig(MS_U32 u32DevID, MSPI_CLKConfig *ptCLKConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_BOOL bRet = FALSE;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_CLKConfig_return;
    }
    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_CLKConfig_return;
    }

    if(ptCLKConfig == NULL)
    {
        HAL_MSPI_Reset_CLKConfig();
        goto MDrv_MasterSPI_CLKConfig_return;
    }

    errnum = _MDrv_CLK_PolaritySetting(ptCLKConfig->BClkPolarity);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto MDrv_MasterSPI_CLKConfig_return;
    }
    errnum = _MDrv_CLK_PhaseSetting(ptCLKConfig->BClkPhase);
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto MDrv_MasterSPI_CLKConfig_return;
    }
    bRet = HAL_MSPI_CLOCK_Config(ptCLKConfig->u32MAXClk);
    if(bRet)
        errnum = E_MSPI_OK;
    else
        errnum = E_MSPI_OPERATION_ERROR;
    if(errnum != E_MSPI_OK)
    {
        errnum |= E_MSPI_CLKCONFIG_ERROR;
        goto MDrv_MasterSPI_CLKConfig_return;
    }
    gu8MSPIConfig |= MSPI_CLK_CONFIG;

MDrv_MasterSPI_CLKConfig_return:
	DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI CLKconfig error errno =%d\n",errnum));
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum;

}


//------------------------------------------------------------------------------
/// Description : config spi transfer timing
/// @param u32DevID      \b IN   device id of slave device
/// @param ptDCConfig    \b OUT  struct pointer of bits of buffer tranferred to slave config
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config transfered bit per buffer
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_FRAMEConfig(MS_U32 u32DevID, MSPI_FrameConfig *ptFrameConfig)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    MS_U8 u8Index = 0;
    MS_U8 u8BufSize;

    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    //check init
    if(!gbInitFlag)
    {
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_FRAMEConfigreturn;
    }
    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_FRAMEConfigreturn;
    }

    if(ptFrameConfig == NULL)
    {
        HAL_MSPI_Reset_FrameConfig();
        goto MDrv_MasterSPI_FRAMEConfigreturn;
    }
    // read buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8RBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_READ_INDEX, u8Index, ptFrameConfig->u8RBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI FRAMEconfig error errno =%d\n",errnum));
            goto MDrv_MasterSPI_FRAMEConfigreturn;
        }
    }
    //write buffer bit config
    u8BufSize = sizeof(ptFrameConfig->u8WBitConfig);
    for(u8Index = 0;u8Index < u8BufSize; u8Index++)
    {
        errnum = _MDrv_Frame_BitSetting(MSPI_WRITE_INDEX, u8Index, ptFrameConfig->u8WBitConfig[u8Index]);
        if(errnum != E_MSPI_OK)
        {
            errnum |= E_MSPI_FRAMECONFIG_ERROR;
            DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "MSPI FRAMEconfig error errno =%d\n",errnum));
            goto MDrv_MasterSPI_FRAMEConfigreturn;
        }
    }
MDrv_MasterSPI_FRAMEConfigreturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    gu8MSPIConfig |= MSPI_FRAME_CONFIG;
    return errnum;
}

//------------------------------------------------------------------------------
/// Description : Enable Slave device
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MasterSPI_SlaveEnable(MS_U32 u32DevID, MS_BOOL Enable)
{
    MSPI_ErrorNo errnum = E_MSPI_OK;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        errnum = E_MSPI_INIT_FLOW_ERROR;
        goto MDrv_MasterSPI_SlaveEnablereturn;
    }

    HAL_MSPI_SlaveEnable(Enable);
    if(Enable && _gu32CsPad != 0xFF)
    {
        mdrv_gpio_set_low(_gu32CsPad);
    }
    else if(_gu32CsPad != 0xFF)
    {
        mdrv_gpio_set_high(_gu32CsPad);
    }
MDrv_MasterSPI_SlaveEnablereturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return errnum;

}

MS_BOOL MDrv_MasterSPI_CsPadConfig(MS_U32 u32DevID, MS_U32 u32CsPad)
{
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    _gu32CsPad =  u32CsPad;
    _gu32DevID =  u32DevID;
    if(_gu32CsPad != 0xFF)
        mdrv_gpio_set_high(_gu32CsPad);
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return TRUE;
}

MS_BOOL MDrv_MasterSPI_MaxClkConfig(MS_U32 u32DevID, MS_U32 u32MaxClk)
{
    MS_BOOL bRet = FALSE;
    if (FALSE == MsOS_ObtainMutex(_gs32MSPI_Mutex, MSPI_MUTEX_WAIT_TIME))
    {
        ULOGE(TAG_MSPI, "%s ENTRY fails!\n", __FUNCTION__);
        return E_MSPI_OPERATION_ERROR;
    }

    if(_gu32DevID != u32DevID)
    {
        ULOGE(TAG_MSPI, "%s FAIL please call MDrv_MasterSPI_CsPadConfig first !!!\r\n",__FUNCTION__);
        goto MDrv_MasterSPI_MaxClkConfigreturn;
    }
    _gu32MaxClk = u32MaxClk;
    bRet = HAL_MSPI_CLOCK_Config(u32MaxClk);

MDrv_MasterSPI_MaxClkConfigreturn:
    MsOS_ReleaseMutex(_gs32MSPI_Mutex);
    return bRet;
}

//------------------------------------------------------------------------------
/// Description : config mspi info for local dimming
/// @param stMspi_Info      \b IN   mspi info struct
/// @return E_MSPI_OK : succeed
/// @return E_MSPI_FRAMECONFIG_ERROR : failed to config mspi
//------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_Info_Config(ST_DRV_MSPI_INFO *stMspi_Info)
{
    MSPI_ErrorNo errorno = E_MSPI_OK;
    MS_VIRT VirtNONPMBank = 0;
    MS_PHY  u32NONPMBankSize = 0;
    MS_VIRT VirtPMBank = 0;
    MS_PHY  u32PMBankSize = 0;
    MSPI_FrameConfig stFramconfig;
    if(stMspi_Info == NULL)
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI,"[Lwc Debug][%s__%d]arg error!\n",__FUNCTION__,__LINE__));
        return E_MSPI_OPERATION_ERROR;
    }

    if(!MDrv_MSPI_HW_Support())
    {
        gbInitFlag = FALSE;
        return E_MSPI_HW_NOT_SUPPORT;
    }
//step 1: config mspi register base
    _gs32MSPI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex MSPI", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32MSPI_Mutex >= 0);

    //_u8MSPIDbgLevel = E_MSPI_DBGLV_DEBUG;

    if (!MDrv_MMIO_GetBASE( &VirtPMBank, &u32PMBankSize, MS_MODULE_PM))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_PM_BANK\n"));
    }

    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
        DEBUG_MSPI(E_MSPI_DBGLV_ERR, ULOGE(TAG_MSPI, "IOMap failure to get DRV_MMIO_NONPM_BANK\n"));
        errorno = E_MSPI_MMIO_ERROR;
    }
    if(stMspi_Info->u8MspiChanel > 2)
        return E_MSPI_PARAM_OVERFLOW;
    HAL_MSPI_MMIOConfig(VirtPMBank, VirtNONPMBank, stMspi_Info->u8MspiChanel);

//step 2 : init mspi
    // default use CS0 for slave device && default use polling mode
    HAL_MSPI_IntEnable(0);
    HAL_MSPI_Init();
    errorno = _MDrv_ChipSelectSetting(0);
    gbInitFlag = TRUE;

//step 3 :set mspi mode
    switch(stMspi_Info->u8MspiMode)
    {
        case 0 :    // CPOL = 0,CPHA =0
            _MDrv_CLK_PolaritySetting(0);
            _MDrv_CLK_PhaseSetting(0);
            break;
        case 1 :    // CPOL = 0,CPHA =1
            _MDrv_CLK_PolaritySetting(0);
            _MDrv_CLK_PhaseSetting(1);
            break;
        case 2 :    // CPOL = 1,CPHA =0
            _MDrv_CLK_PolaritySetting(1);
            _MDrv_CLK_PhaseSetting(0);
            break;
        case 3 :    // CPOL = 1,CPHA =1
            _MDrv_CLK_PolaritySetting(1);
            _MDrv_CLK_PhaseSetting(1);
            break;
        default:
            _MDrv_CLK_PolaritySetting(0);
            _MDrv_CLK_PhaseSetting(0);
            break;
    }
//step 4 : set mspi clk
    HAL_MSPI_CLK_Config(stMspi_Info->u8MspiChanel,stMspi_Info->u32MspiClk);

//step 5 : config DC timming
    HAL_MSPI_Reset_DCConfig();

    _MDrv_DC_TrStartSetting(stMspi_Info->u8TrStart);

    _MDrv_DC_TrEndSetting(stMspi_Info->u8TrEnd);

    _MDrv_DC_TBSetting(stMspi_Info->u8TB);

    _MDrv_DC_TRWSetting(stMspi_Info->u8TRW);

//step 6: reset per byte bits
    memcpy(&stFramconfig.u8RBitConfig,stMspi_Info->u8RBitConfig,8);
    memcpy(&stFramconfig.u8WBitConfig,stMspi_Info->u8WBitConfig,8);
    MDrv_MSPI_FRAMEConfig(&stFramconfig);
    DEBUG_MSPI(E_MSPI_DBGLV_INFO, ULOGE(TAG_MSPI, "[Lwc Debug] MDrv_MSPI_LD_Config Pass!\n"));
    return errorno;
}
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_MSPI_ToBeRemove
/// @param Direct \b IN: read or write flag 0:read 1:write
/// @param u8Bytes \b IN: set read/write trigger buffer bytes size
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MSPI_ErrorNo MDrv_MSPI_RWBytes(MS_BOOL Direct, MS_U8 u8Bytes)
{
    return HAL_MSPI_RWBytes(Direct,u8Bytes);
}
