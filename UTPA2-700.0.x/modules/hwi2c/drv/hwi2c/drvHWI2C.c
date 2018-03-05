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
///////////////////////////////////////////////////////////////////////////////
#define _DRVHWI2C_C

////////////////////////////////////////////////////////////////////////////////
/// file drvHWI2C.c
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvHWI2C.h"
#include "halHWI2C.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "ULog.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////
//define hwi2c ports
#ifdef CONFIG_MSTAR_UTPA2K_WDT_ENABLE
#define HWI2C_UTOPIA20 (1)
#else
#define HWI2C_UTOPIA20 (0)
#endif

#ifdef CONFIG_MSTAR_WDT_KERNEL_MODE
#define HWI2C_KERNEL_MODE (1)
#else
#define HWI2C_KERNEL_MODE (0)
#endif

#if (HWI2C_UTOPIA20)
#include "utopia.h"
#include "drvHWI2C_v2.h"
#include "drvHWI2C_private.h"
#endif

#define HWI2C_PORTM                   8 //maximum support ports
#define HWI2C_PORTS                   HAL_HWI2C_PORTS
#define HWI2C_PORT0                   HAL_HWI2C_PORT0
#define HWI2C_PORT1                   HAL_HWI2C_PORT1
#define HWI2C_PORT2                   HAL_HWI2C_PORT2
#define HWI2C_PORT3                   HAL_HWI2C_PORT3
#define HWI2C_STARTDLY                5  //   20 //us
#define HWI2C_STOPDLY                 60 //us
//define mutex
#define HWI2C_MUTEX_CREATE(_P_)          g_s32HWI2CMutex[_P_] = MsOS_CreateMutex(E_MSOS_FIFO, (char*)gu8HWI2CMutexName[_P_] , MSOS_PROCESS_SHARED)
#define HWI2C_MUTEX_LOCK(_P_)            OS_OBTAIN_MUTEX(g_s32HWI2CMutex[_P_],MSOS_WAIT_FOREVER)
#define HWI2C_MUTEX_UNLOCK(_P_)          OS_RELEASE_MUTEX(g_s32HWI2CMutex[_P_])
#define HWI2C_MUTEX_DELETE(_P_)          OS_DELETE_MUTEX(g_s32HWI2CMutex[_P_])

#define HWI2C_CONTROL_MUTEX_CREATE()          g_s32HWI2C_ControlMutex = MsOS_CreateMutex(E_MSOS_FIFO, "HWI2CCONTROLMUTEX" , MSOS_PROCESS_SHARED)
#define HWI2C_CONTROL_MUTEX_LOCK()            OS_OBTAIN_MUTEX(g_s32HWI2C_ControlMutex,MSOS_WAIT_FOREVER)
#define HWI2C_CONTROL_MUTEX_UNLOCK()          OS_RELEASE_MUTEX(g_s32HWI2C_ControlMutex)
#define HWI2C_CONTROL_MUTEX_DELETE()          OS_DELETE_MUTEX(g_s32HWI2C_ControlMutex)

static MS_U32 g_s32HWI2C_ControlMutex  = -1;
static MS_S32 g_s32HWI2CMutex[HWI2C_PORTM] = {-1,-1,-1,-1,-1,-1,-1,-1};
static char gu8HWI2CMutexName[HWI2C_PORTM][13] = {"HWI2CMUTEXP0","HWI2CMUTEXP1","HWI2CMUTEXP2","HWI2CMUTEXP3","HWI2CMUTEXP4","HWI2CMUTEXP5","HWI2CMUTEXP6","HWI2CMUTEXP7"};

#define HWI2C_DBG_FUNC()               do {if (_geDbgLv >= E_HWI2C_DBGLV_ALL) \
                                            {MS_DEBUG_MSG(ULOGI("HWIIC", "\t====   %s   ====\n", __FUNCTION__);)} } while(0)
#define HWI2C_DBG_INFO(x, args...)     do { if (_geDbgLv >= E_HWI2C_DBGLV_INFO ) \
                                            {MS_DEBUG_MSG(ULOGI("HWIIC", x, ##args);)} } while(0)
#define HWI2C_DBG_ERR(x, args...)      do { if (_geDbgLv >= E_HWI2C_DBGLV_ERR_ONLY) \
                                            {MS_DEBUG_MSG(ULOGE("HWIIC", x, ##args);)} } while(0)

#define I2C_ACCESS_DUMMY_TIME   50
////////////////////////////////////////////////////////////////////////////////
// Local & Global Variables
////////////////////////////////////////////////////////////////////////////////
static MSIF_Version _drv_hwi2c_version = {
    .DDI = { HWI2C_DRV_VERSION },
};

static MS_BOOL _gbInit = FALSE;
static HWI2C_DbgLv _geDbgLv = E_HWI2C_DBGLV_ERR_ONLY;
static HWI2C_State _geState = E_HWI2C_IDLE;
static HWI2C_State _geStatePortGroup[HWI2C_PORTS];
static HWI2C_Info _gsI2CInfo;
static MS_U8  g_u8SelectPort = E_HWI2C_PORT_NOSUP;
static MS_U32 g_u32StartDelay[HWI2C_PORTS];
static MS_U32 g_u32StopDelay[HWI2C_PORTS];
static HWI2C_ReadMode g_HWI2CReadMode[HWI2C_PORTS];
static HWI2C_PORT g_HWI2CPort[HWI2C_PORTS];
static MS_U16 g_u16DelayFactor[HWI2C_PORTS];
static MS_BOOL g_bDMAEnable[HWI2C_PORTS];
static MS_U8 g_HWI2CPortIdx = HWI2C_PORT0;
static HWI2C_UnitCfg g_HWI2CinitCfg;
static HWI2C_UnitCfg *gp_HWI2CinitCfg = &g_HWI2CinitCfg;

#if (HWI2C_UTOPIA20)
MS_BOOL bU20HWI2COpened = FALSE;
void* pInstantHwi2c;
void* pAttributeHwi2c;
#endif

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
#define _MDrv_HWI2C_Send_Byte HAL_HWI2C_Send_Byte
#define _MDrv_HWI2C_Recv_Byte HAL_HWI2C_Recv_Byte

#if 0
static void _MDrv_HWI2C_DelayUs(MS_U32 u32Us)
{
    // volatile is necessary to avoid optimization
    MS_U32 volatile u32Dummy = 0;
    //MS_U32 u32Loop;
    MS_U32 volatile u32Loop;

    u32Loop = (MS_U32)(50 * u32Us);
    while (u32Loop--)
    {
        u32Dummy++;
    }
}
#endif

static MS_BOOL _MDrv_HWI2C_GetPortRegOffset(MS_U8 u8Port, MS_U32 *pu32Offset)
{
    HWI2C_DBG_FUNC();

    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    HWI2C_DBG_INFO("g_HWI2CPort[%d] = 0x%02X\n", u8Port, (MS_U8)g_HWI2CPort[u8Port]);
    return HAL_HWI2C_SetPortRegOffset((HAL_HWI2C_PORT)g_HWI2CPort[u8Port],pu32Offset);
}

static MS_BOOL _MDrv_HWI2C_ReadBytes(MS_U8 u8Port, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_U8 u8SlaveID = LOW_BYTE(u16SlaveCfg);
    MS_U32 u32Offset = 0x00;
    MS_U16 u16Dummy = I2C_ACCESS_DUMMY_TIME; // loop dummy
    MS_BOOL bComplete = FALSE;
    MS_U32 uAddrCntBkp,uSizeBkp;
    MS_U8 *pRegAddrBkp,*pDataBkp;

    HWI2C_DBG_FUNC();

    _geState = E_HWI2C_READ_DATA;
    _geStatePortGroup[u8Port] = E_HWI2C_READ_DATA;
    if (!pRegAddr)
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;

    //check support port index
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    //no meaning operation
    if (!uSize)
    {
        HWI2C_DBG_ERR("Read bytes error!\n");
        return FALSE;
    }

    //configure port register offset ==> important
    if(!_MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset))
    {
        HWI2C_DBG_ERR("Port index error!\n");
        return FALSE;
    }

    if(g_bDMAEnable[u8Port])
    {
        _geState = E_HWI2C_DMA_READ_DATA;
	_geStatePortGroup[u8Port] = E_HWI2C_DMA_READ_DATA;
        return HAL_HWI2C_DMA_ReadBytes(u32Offset, u16SlaveCfg, uAddrCnt, pRegAddr, uSize, pData);
    }

    //start access routines
    uAddrCntBkp = uAddrCnt;
    pRegAddrBkp = pRegAddr;
    uSizeBkp = uSize;
    pDataBkp = pData;
    while (u16Dummy--)
    {
        if((g_HWI2CReadMode[u8Port]!=E_HWI2C_READ_MODE_DIRECT) && (uAddrCnt>0)&& (pRegAddr))
        {
            HAL_HWI2C_Start(u32Offset);
            //add extral delay by user configuration
            MsOS_DelayTaskUs_Poll(g_u32StartDelay[u8Port]);

            if (!_MDrv_HWI2C_Send_Byte(u32Offset,HWI2C_SET_RW_BIT(FALSE, u8SlaveID)))
                goto end;

            while(uAddrCnt--)
            {
                if (!_MDrv_HWI2C_Send_Byte(u32Offset,*pRegAddr))
                    goto end;
                pRegAddr++;
            }

            if(g_HWI2CReadMode[u8Port]==E_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START)
            {
                HAL_HWI2C_Stop(u32Offset);
                //add extral delay by user configuration
                MsOS_DelayTaskUs_Poll(g_u32StopDelay[u8Port]);
            }
        }

        //Very important to add delay to support all clock speeds
        //Strongly recommend that do not remove this delay routine
        HAL_HWI2C_ExtraDelay(g_u16DelayFactor[u8Port]);

        HAL_HWI2C_Start(u32Offset);
        //add extral delay by user configuration
        MsOS_DelayTaskUs_Poll(g_u32StartDelay[u8Port]);

        if (!_MDrv_HWI2C_Send_Byte(u32Offset,HWI2C_SET_RW_BIT(TRUE, u8SlaveID)))
            goto end;

        while(uSize)
        {
            ///////////////////////////////////
            //
            //  must set ACK/NAK before read ready
            //
            uSize--;
            if (uSize==0)
                HAL_HWI2C_NoAck(u32Offset);
            if (_MDrv_HWI2C_Recv_Byte(u32Offset,pData)==FALSE)
                goto end;
            pData++;
        }
        bComplete = TRUE;

    end:
        HAL_HWI2C_Stop(u32Offset);
        //add extral delay by user configuration
        MsOS_DelayTaskUs_Poll(g_u32StopDelay[u8Port]);
        if(u16Dummy&&(bComplete==FALSE))
        {
            uAddrCnt = uAddrCntBkp;
            pRegAddr = pRegAddrBkp;
            uSize = uSizeBkp;
            pData = pDataBkp;
            continue;
        }
        break;
    }
    _geState = E_HWI2C_IDLE;
    _geStatePortGroup[u8Port] = E_HWI2C_IDLE;

    return (uSize) ? FALSE : TRUE;
}

static MS_BOOL _MDrv_HWI2C_WriteBytes(MS_U8 u8Port, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_U8 u8SlaveID = LOW_BYTE(u16SlaveCfg);
    MS_U32 u32Offset = 0x00;
    
    MS_U16 u16Dummy = I2C_ACCESS_DUMMY_TIME; // loop dummy
    MS_BOOL bComplete = FALSE;
    MS_U32 uAddrCntBkp,uSizeBkp;
    MS_U8 *pRegAddrBkp,*pDataBkp;

    HWI2C_DBG_FUNC();

    _geState = E_HWI2C_WRITE_DATA;
    _geStatePortGroup[u8Port] = E_HWI2C_WRITE_DATA;
    if (!pRegAddr)
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;

    //check support port index
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    //no meaning operation
    if (!uSize)
    {
        HWI2C_DBG_ERR("Write bytes error!\n");
        return FALSE;
    }

    //configure port register offset ==> important
    if(!_MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset))
    {
        HWI2C_DBG_ERR("Port index error!\n");
        return FALSE;
    }

    if(g_bDMAEnable[u8Port])
    {
        _geState = E_HWI2C_DMA_WRITE_DATA;
	_geStatePortGroup[u8Port] = E_HWI2C_DMA_WRITE_DATA;
        return HAL_HWI2C_DMA_WriteBytes(u32Offset, u16SlaveCfg, uAddrCnt, pRegAddr, uSize, pData);
    }

    //start access routines
    uAddrCntBkp = uAddrCnt;
    pRegAddrBkp = pRegAddr;
    uSizeBkp = uSize;
    pDataBkp = pData;
    while(u16Dummy--)
    {
        HAL_HWI2C_Start(u32Offset);
        MsOS_DelayTaskUs_Poll(g_u32StartDelay[u8Port]);

        if (!_MDrv_HWI2C_Send_Byte(u32Offset,HWI2C_SET_RW_BIT(FALSE, u8SlaveID)))
            goto end;

        while(uAddrCnt)
        {
            if (!_MDrv_HWI2C_Send_Byte(u32Offset, *pRegAddr))
                goto end;
            uAddrCnt--;
            pRegAddr++;
        }

        while(uSize)
        {
            if (!_MDrv_HWI2C_Send_Byte(u32Offset, *pData))
                goto end;
            uSize--;
            pData++;
        }
        bComplete = TRUE;

    end:
        HAL_HWI2C_Stop(u32Offset);
        //add extral delay by user configuration
        MsOS_DelayTaskUs_Poll(g_u32StopDelay[u8Port]);
        if(u16Dummy&&(bComplete==FALSE))
        {
            uAddrCnt = uAddrCntBkp;
            pRegAddr = pRegAddrBkp;
            uSize = uSizeBkp;
            pData = pDataBkp;
            continue;
        }
        break;
    }
    _geState = E_HWI2C_IDLE;
    _geStatePortGroup[u8Port] = E_HWI2C_IDLE;

    return (uSize) ? FALSE : TRUE;
}

static MS_BOOL _MDrv_HWI2C_SelectPort(HWI2C_PORT ePort)
{
    MS_U32 u32Offset = 0x00;
    MS_U8 u8Port = 0x00;
    MS_BOOL bRet=TRUE;
    
    HWI2C_DBG_FUNC();

    //(1) Get port index by port number 
    if(HAL_HWI2C_GetPortIdxByPort((HAL_HWI2C_PORT)ePort,&u8Port))
    {
        g_HWI2CPortIdx = u8Port;
        g_HWI2CPort[u8Port] = ePort;
        //set default port parameters
        g_HWI2CReadMode[u8Port] = E_HWI2C_READ_MODE_DIRECTION_CHANGE;
        g_u16DelayFactor[u8Port] = 0;
        #ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
        g_bDMAEnable[u8Port] = TRUE;
        #else
        g_bDMAEnable[u8Port] = FALSE;
        #endif
    }
    else
    {
        g_HWI2CPortIdx = u8Port;
        //do nothing
        return FALSE;
    }
    HWI2C_DBG_INFO("ePort:0x%02X, u8Port:0x%02X\n",(MS_U8)ePort, u8Port);

    //(2) Set pad mux for port number 
    HAL_HWI2C_SelectPort((HAL_HWI2C_PORT)ePort);

    //(3) configure port register offset ==> important
    bRet &= _MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset);

    //(4) master init
    bRet &= HAL_HWI2C_Master_Enable(u32Offset);

    return bRet;
}

static MS_BOOL _MDrv_HWI2C_SetClk(MS_U8 u8Port, HWI2C_CLKSEL eClk)
{
    MS_U32 u32Offset = 0x00;

    HWI2C_DBG_FUNC();
    HWI2C_DBG_INFO("Port%d clk: %u\n", u8Port, eClk);
    //check support port index
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    //check support clock speed
    if (eClk >= E_HWI2C_NOSUP)
    {
        HWI2C_DBG_ERR("Clock [%u] is not supported!\n",eClk);
        return FALSE;
    }

    //configure port register offset ==> important
    if(!_MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset))
    {
        HWI2C_DBG_ERR("Port index error!\n");
        return FALSE;
    }

    g_u16DelayFactor[u8Port] = (MS_U16)(1<<(eClk));
    return HAL_HWI2C_SetClk(u32Offset,(HAL_HWI2C_CLKSEL)eClk);
}

static MS_BOOL _MDrv_HWI2C_SetReadMode(MS_U8 u8Port, HWI2C_ReadMode eReadMode)
{
    HWI2C_DBG_FUNC();
    HWI2C_DBG_INFO("Port%d Readmode: %u\n", u8Port, eReadMode);
    //check support port index
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    if(eReadMode>=E_HWI2C_READ_MODE_MAX)
        return FALSE;
    g_HWI2CReadMode[u8Port] = eReadMode;
    return TRUE;
}

static MS_BOOL _MDrv_HWI2C_InitPort(HWI2C_UnitCfg *psCfg)
{
    MS_U8 u8PortIdx = 0, u8Port = 0;
    MS_U32 u32Offset = 0x00;
    MS_BOOL bRet = TRUE;
    HWI2C_PortCfg stPortCfg;
        
    HWI2C_DBG_FUNC();

    //(1) set default value for port variables
    for(u8PortIdx=0; u8PortIdx < HWI2C_PORTS; u8PortIdx++)
    {
        stPortCfg = psCfg->sCfgPort[u8PortIdx];
        if(stPortCfg.bEnable)
        {
            if(_MDrv_HWI2C_SelectPort(stPortCfg.ePort))
            {
                //set clock speed
                bRet &= _MDrv_HWI2C_SetClk(g_HWI2CPortIdx, stPortCfg.eSpeed);
                //set read mode
                bRet &= _MDrv_HWI2C_SetReadMode(g_HWI2CPortIdx, stPortCfg.eReadMode);
                //get port index
                bRet &= HAL_HWI2C_GetPortIdxByPort((HAL_HWI2C_PORT)stPortCfg.ePort,&u8Port);
                //configure port register offset ==> important
                bRet &= _MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset);
                //master init
                bRet &= HAL_HWI2C_Master_Enable(u32Offset);
                //dma init
                bRet &= HAL_HWI2C_DMA_Init(u32Offset,(HAL_HWI2C_PortCfg*)&stPortCfg);
                g_bDMAEnable[u8Port] = stPortCfg.bDmaEnable;
                //dump port information
                HWI2C_DBG_INFO("Port:%u Index=%u\n",stPortCfg.ePort,u8Port);
                HWI2C_DBG_INFO("Enable=%u\n",stPortCfg.bEnable);
                HWI2C_DBG_INFO("DmaReadMode:%u\n",stPortCfg.eReadMode);
                HWI2C_DBG_INFO("Speed:%u\n",stPortCfg.eSpeed);
                HWI2C_DBG_INFO("DmaEnable:%u\n",stPortCfg.bDmaEnable);
                HWI2C_DBG_INFO("DmaAddrMode:%u\n",stPortCfg.eDmaAddrMode);
                HWI2C_DBG_INFO("DmaMiuCh:%u\n",stPortCfg.eDmaMiuCh);
                HWI2C_DBG_INFO("DmaMiuPri:%u\n",stPortCfg.eDmaMiuPri);
                HWI2C_DBG_INFO("DmaPhyAddr:%lx\n",stPortCfg.u32DmaPhyAddr);
            }
        }
    }

    //(2) check initialized port : override above port configuration
    if(_MDrv_HWI2C_SelectPort(psCfg->ePort))
    {
        //set clock speed
        bRet &=_MDrv_HWI2C_SetClk(g_HWI2CPortIdx,psCfg->eSpeed);
        //set read mode
        bRet &=_MDrv_HWI2C_SetReadMode(g_HWI2CPortIdx,psCfg->eReadMode);
        //get port index
        bRet &= HAL_HWI2C_GetPortIdxByPort((HAL_HWI2C_PORT)psCfg->ePort,&u8Port);
        //configure port register offset ==> important
        bRet &= _MDrv_HWI2C_GetPortRegOffset(u8Port,&u32Offset);
        //master init
        bRet &= HAL_HWI2C_Master_Enable(u32Offset);
    }

    //(3) dump allocated port information 
    for(u8PortIdx=0; u8PortIdx < HWI2C_PORTS; u8PortIdx++)
    {
        HWI2C_DBG_INFO("HWI2C Allocated Port[%d] = 0x%02X\n",u8PortIdx,g_HWI2CPort[u8PortIdx]);
    }
  
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
// Global Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_Init
/// @brief \b Function  \b Description: Init HWI2C driver
/// @param psCfg        \b IN: hw I2C config
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_Init_U2K(HWI2C_UnitCfg *psCfg)
{
    MS_BOOL bRet = TRUE;
    MS_VIRT u32BaseAddr = 0xFFFFFFFFUL;
	MS_PHY  u32BaseSize;
    MS_U8 u8Port=0;

    HWI2C_DBG_FUNC();

    if (psCfg == NULL)
        return FALSE;

	//store psCfg for FastBoot resume
	gp_HWI2CinitCfg = &g_HWI2CinitCfg;
	gp_HWI2CinitCfg->eReadMode = psCfg->eReadMode;
	gp_HWI2CinitCfg->ePort = psCfg->ePort;
	gp_HWI2CinitCfg->eSpeed = psCfg->eSpeed;
	gp_HWI2CinitCfg->sI2CPin = psCfg->sI2CPin;
	gp_HWI2CinitCfg->sCfgPort[0] = psCfg->sCfgPort[0];
	gp_HWI2CinitCfg->sCfgPort[1] = psCfg->sCfgPort[1];
	gp_HWI2CinitCfg->sCfgPort[2] = psCfg->sCfgPort[2];
	gp_HWI2CinitCfg->sCfgPort[3] = psCfg->sCfgPort[3];
    if (_gbInit)
    {
        HWI2C_DBG_INFO("HW I2C has been initialed!\n");
        return TRUE;
    }
    //(1) get & set io base
    if ((!psCfg) || (!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_HWI2C)))
        return FALSE;
    HAL_HWI2C_SetIOMapBase(u32BaseAddr);

    //(2) Initialize pad mux and basic settings
    HWI2C_DBG_INFO("Pinreg:%lx bit:%u enable:%u speed:%u\n",psCfg->sI2CPin.u32Reg, psCfg->sI2CPin.u8BitPos, psCfg->sI2CPin.bEnable,psCfg->eSpeed);
    bRet &= HAL_HWI2C_Init_Chip();
    //(3) Initialize all port
    bRet &= _MDrv_HWI2C_InitPort(psCfg);
    //(4) Check final result
    if (!bRet)
    {
        HWI2C_DBG_ERR("I2C init fail!\n");
    }
    
    //(5) Extra procedure to do after initialization
    HAL_HWI2C_Init_ExtraProc();

    _gsI2CInfo.u32IOMap = u32BaseAddr;
    for(u8Port=0;u8Port<(MS_U8)HWI2C_PORTS;u8Port++)
    {
        _geStatePortGroup[u8Port] = E_HWI2C_IDLE;
    	g_u32StartDelay[u8Port] = HWI2C_STARTDLY; 
    	g_u32StopDelay[u8Port] = HWI2C_STOPDLY;
	HWI2C_DBG_INFO("Port:%d, START default delay %d(us)\n",u8Port, (int)g_u32StartDelay[u8Port]);
        HWI2C_DBG_INFO("Port:%d, STOP default delay %d(us)\n",u8Port, (int)g_u32StopDelay[u8Port]);
    }
    memcpy(&_gsI2CInfo.sUnitCfg, psCfg, sizeof(HWI2C_UnitCfg));
    _gbInit = TRUE;

    HWI2C_DBG_INFO("HWI2C_MUTEX_CREATE!\n");
    for(u8Port=0;u8Port<(MS_U8)HWI2C_PORTS;u8Port++)
    {
        HWI2C_MUTEX_CREATE(u8Port);
    }
    HWI2C_CONTROL_MUTEX_CREATE();
    
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_Start
/// @brief \b Function  \b Description: send start bit
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_Start_U2K(void)
{
    HWI2C_DBG_FUNC();
    return HAL_HWI2C_Start(0x0000);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_Stop
/// @brief \b Function  \b Description: send stop bit
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_Stop_U2K(void)
{
    HWI2C_DBG_FUNC();
    return HAL_HWI2C_Stop(0x0000);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_GetPortIndex
/// @brief \b Function  \b Description: Get port index from port number
/// @param ePort        \b IN: port number
/// @param ePort        \b OUT: pointer to port index
/// @return             \b MS_U8: Port Index
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_GetPortIndex(HWI2C_PORT ePort, MS_U8* pu8Port)
{
    MS_BOOL bRet=TRUE;
    
    HWI2C_DBG_FUNC();

    //(1) Get port index by port number 
    bRet &= HAL_HWI2C_GetPortIdxByPort((HAL_HWI2C_PORT)ePort, pu8Port);
    HWI2C_DBG_INFO("ePort:0x%02X, u8Port:0x%02X\n",(MS_U8)ePort,(MS_U8)*pu8Port);

    return bRet;
}

//###################
//
//  Multi-Port Support: Port 0
//
//###################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SelectPort
/// @brief \b Function  \b Description: Decide port index and pad mux for port number
/// @param ePort        \b IN: port number
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SelectPort_U2K(HWI2C_PORT ePort)
{
    MS_BOOL bRet;
    HWI2C_DBG_FUNC();
    if(ePort >= E_HWI2C_PORT_NOSUP)
        return FALSE;
    HWI2C_CONTROL_MUTEX_LOCK();
    bRet =  _MDrv_HWI2C_SelectPort(ePort);
    if (bRet == TRUE)
        g_u8SelectPort = ePort;
    HWI2C_CONTROL_MUTEX_UNLOCK();
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SetClk
/// @brief \b Function  \b Description: Set HW I2C clock
/// @param eClk         \b IN: clock rate
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SetClk_U2K(HWI2C_CLKSEL eClk)
{
    HWI2C_DBG_FUNC();
    return _MDrv_HWI2C_SetClk(g_HWI2CPortIdx, eClk);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SetReadMode
/// @brief \b Function  \b Description: Set HW I2C Read Mode
/// @param eClk         \b IN: ReadMode
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SetReadMode(HWI2C_ReadMode eReadMode)
{
    return _MDrv_HWI2C_SetReadMode(g_HWI2CPortIdx, eReadMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadByte
/// @brief \b Function  \b Description: read 1 byte data
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param u8RegAddr    \b IN: target register address
/// @param pData        \b Out: read 1 byte data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_ReadByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData)
{
    HWI2C_DBG_FUNC();
    return MDrv_HWI2C_ReadBytes(u16SlaveCfg, 1, &u8RegAddr, 1, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadByte
/// @brief \b Function  \b Description: write 1 byte data
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param u8RegAddr    \b IN: target register address
/// @param u8Data       \b IN: 1 byte data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_WriteByte(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data)
{
    HWI2C_DBG_FUNC();
    return MDrv_HWI2C_WriteBytes_U2K(u16SlaveCfg, 1, &u8RegAddr, 1, &u8Data);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_WriteBytes
/// @brief \b Function  \b Description: Init HWI2C driver
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param uAddrCnt     \b IN: register address count
/// @param pRegAddr     \b IN: pointer to targert register address
/// @param uSize        \b IN: data length
/// @param pData        \b IN: data array
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_WriteBytes_U2K(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bRet;
    MS_U8 u8Port;

    u8Port = HIGH_BYTE(u16SlaveCfg);
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    HWI2C_MUTEX_LOCK(u8Port);
    bRet = _MDrv_HWI2C_WriteBytes(u8Port,u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
    HWI2C_MUTEX_UNLOCK(u8Port);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadBytes
/// @brief \b Function  \b Description: Init HWI2C driver
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param uAddrCnt     \b IN: register address count
/// @param pRegAddr     \b IN: pointer to targert register address
/// @param uSize        \b IN: data length
/// @param pData        \b Out: read data aray
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_ReadBytes_U2K(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bRet;
    MS_U8 u8Port;

    u8Port = HIGH_BYTE(u16SlaveCfg);
    if(u8Port>=HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port index is >= max supported ports %d !\n",HWI2C_PORTS);
        return FALSE;
    }
    HWI2C_MUTEX_LOCK(u8Port);
    bRet = _MDrv_HWI2C_ReadBytes(u8Port,u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
    HWI2C_MUTEX_UNLOCK(u8Port);
    return bRet;
}

//###################
//
//  Multi-Port Support: Port 1
//
//###################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SelectPort1
/// @brief \b Function  \b Description: Decide port index and pad mux for port number
/// @param ePort        \b IN: port number
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SelectPort1(HWI2C_PORT ePort)
{
    MS_BOOL bRet;
    HWI2C_DBG_FUNC();
    if(ePort >= E_HWI2C_PORT_NOSUP)
        return FALSE;
    HWI2C_CONTROL_MUTEX_LOCK();
    bRet = _MDrv_HWI2C_SelectPort(ePort);
    if (bRet == TRUE)
	g_u8SelectPort = ePort;
    HWI2C_CONTROL_MUTEX_UNLOCK();
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SetClkP1
/// @brief \b Function  \b Description: Set HW I2C clock
/// @param eClk         \b IN: clock rate
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SetClkP1(HWI2C_CLKSEL eClk)
{
    HWI2C_DBG_FUNC();
    return _MDrv_HWI2C_SetClk(HWI2C_PORT1, eClk);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SetReadModeP1
/// @brief \b Function  \b Description: Set HW I2C Read Mode
/// @param eClk         \b IN: ReadMode
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SetReadModeP1(HWI2C_ReadMode eReadMode)
{
    return _MDrv_HWI2C_SetReadMode(HWI2C_PORT1, eReadMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadByteP1
/// @brief \b Function  \b Description: read 1 byte data
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param u8RegAddr    \b IN: target register address
/// @param pData        \b Out: read 1 byte data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_ReadByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 *pData)
{
    HWI2C_DBG_FUNC();
    return MDrv_HWI2C_ReadBytesP1(u16SlaveCfg, 1, &u8RegAddr, 1, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadByteP1
/// @brief \b Function  \b Description: write 1 byte data
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param u8RegAddr    \b IN: target register address
/// @param u8Data       \b IN: 1 byte data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_WriteByteP1(MS_U16 u16SlaveCfg, MS_U8 u8RegAddr, MS_U8 u8Data)
{
    HWI2C_DBG_FUNC();
    return MDrv_HWI2C_WriteBytesP1(u16SlaveCfg, 1, &u8RegAddr, 1, &u8Data);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_WriteBytesP1
/// @brief \b Function  \b Description: Init HWI2C driver
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param uAddrCnt     \b IN: register address count
/// @param pRegAddr     \b IN: pointer to targert register address
/// @param uSize        \b IN: data length
/// @param pData        \b IN: data array
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_WriteBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bRet;

    HWI2C_MUTEX_LOCK(HWI2C_PORT1);
    bRet = _MDrv_HWI2C_WriteBytes(HWI2C_PORT1,u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
    HWI2C_MUTEX_UNLOCK(HWI2C_PORT1);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ReadBytesP1
/// @brief \b Function  \b Description: Init HWI2C driver
/// @param u16SlaveCfg  \b IN: [15:8]: Channel number [7:0]:Slave ID
/// @param uAddrCnt     \b IN: register address count
/// @param pRegAddr     \b IN: pointer to targert register address
/// @param uSize        \b IN: data length
/// @param pData        \b Out: read data aray
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_ReadBytesP1(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
    MS_BOOL bRet;

    HWI2C_MUTEX_LOCK(HWI2C_PORT1);
    bRet = _MDrv_HWI2C_ReadBytes(HWI2C_PORT1,u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
    HWI2C_MUTEX_UNLOCK(HWI2C_PORT1);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_SetDbgLevel
/// @brief \b Function  \b Description: Set debug level for debug message
/// @param eLevel       \b IN: E_HWI2C_DBGLV_NONE/E_HWI2C_DBGLV_ERR_ONLY/E_HWI2C_DBGLV_INFO/E_HWI2C_DBGLV_ALL
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_SetDbgLevel(HWI2C_DbgLv eLevel)
{
    HWI2C_DBG_FUNC();
    HWI2C_DBG_INFO("level: %u\n", eLevel);
    _geDbgLv = eLevel;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_GetLibVer
/// @brief \b Function  \b Description: Show the HW I2C driver version
/// @param ppVersion    \b Out: output HW I2C driver version
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_HWI2C_GetLibVer(const MSIF_Version **ppVersion)
{
    HWI2C_DBG_FUNC();
    if (!ppVersion)
        return FALSE;
    *ppVersion = &_drv_hwi2c_version;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_GetStatus
/// @brief \b Function  \b Description: Get HW I2C driver status
/// @param pStatus      \b Out : Hw I2C driver status
////////////////////////////////////////////////////////////////////////////////
void MDrv_HWI2C_GetStatus(HWI2C_Status *pStatus)
{
    MS_U8 u8Port;
    MS_BOOL bFlag = TRUE;
    HWI2C_DBG_FUNC();
    if (!pStatus)
        return;

    HWI2C_CONTROL_MUTEX_LOCK();
    u8Port =(MS_U8) g_u8SelectPort / 8;
 
    if (u8Port >= HWI2C_PORTS)
        bFlag = FALSE;
    pStatus->bIsInit = _gbInit;
    pStatus->u8DbgLevel = _geDbgLv;
    pStatus->bIsMaster = HAL_HWI2C_IsMaster();
    pStatus->eState =(bFlag == TRUE? _geStatePortGroup[u8Port] : _geState);
    HWI2C_CONTROL_MUTEX_UNLOCK();
}

void MDrv_HWI2C_GetStatus_EX(void *pParam_Status)
{
    HWI2C_PortStatus *pStatus = NULL;
	
    HWI2C_DBG_FUNC();
    if (!pParam_Status)
        return;

    pStatus = (HWI2C_PortStatus *) pParam_Status;
	
    if (pStatus->u8Port >= HWI2C_PORTS)
        return ;
	
    HWI2C_MUTEX_LOCK(pStatus->u8Port);
    pStatus->bIsInit = _gbInit;
    pStatus->u8DbgLevel = _geDbgLv;
    pStatus->bIsMaster = HAL_HWI2C_IsMaster();
    pStatus->eState = _geStatePortGroup[pStatus->u8Port];
    HWI2C_MUTEX_UNLOCK(pStatus->u8Port);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_GetInfo
/// @brief \b Function  \b Description: Get HW I2C driver information
/// @return             \b HWI2C_Info*: Hw I2C drvier infroamtion
////////////////////////////////////////////////////////////////////////////////
const HWI2C_Info* MDrv_HWI2C_GetInfo()
{
    HWI2C_DBG_FUNC();
    return (&_gsI2CInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ConfigDelay
/// @brief \b Function  \b Description: config delay parameter for START & STOP
/// @param u32StartDelay       \b IN: delay time (us) for after START
/// @param u32StopDelay       \b IN: delay time (us) for after STOP
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL  MDrv_HWI2C_ConfigDelay(MS_U32 u32StartDelay, MS_U32 u32StopDelay)
{
    HWI2C_DBG_FUNC();
    MS_U8 u8Port;
    if(u32StartDelay < HWI2C_STARTDLY)
    {
        if(u32StartDelay==0)
        {
            HWI2C_DBG_ERR("START Delay 0(us) will cause accessing error !\n");
        }
        else
        {
            HWI2C_DBG_ERR("START Delay %d(us) will risk accessing error !\n",(int)u32StartDelay);
        }
    }

    if(u32StopDelay < HWI2C_STOPDLY)
    {
        if(u32StopDelay==0) 
        {
            HWI2C_DBG_ERR("STOP Delay 0(us) will cause accessing error !\n");
        }
        else
        {
            HWI2C_DBG_ERR("STOP Delay %d(us) will risk accessing error !\n",(int)u32StopDelay);
        }
    }

    HWI2C_CONTROL_MUTEX_LOCK();
    u8Port =(MS_U8) g_u8SelectPort / 8;
    
    if (u8Port >= HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Please first call API:MDrv_HWI2C_SelectPort() or  use  API:MDrv_HWI2C_ConfigDelay_EX()\n");
        HWI2C_CONTROL_MUTEX_UNLOCK();
        return FALSE;
    }
    g_u32StartDelay[u8Port] = u32StartDelay;
    g_u32StopDelay[u8Port] = u32StopDelay;
    HWI2C_DBG_INFO("Port:%d, u32StartDelay: %d(us)\n",u8Port, (int)g_u32StartDelay[u8Port]);
    HWI2C_DBG_INFO("Port:%d, u32StopDelay: %d(us)\n",u8Port,(int)g_u32StopDelay[u8Port]);
    HWI2C_CONTROL_MUTEX_UNLOCK();
	
    return (((u32StartDelay==0)||(u32StopDelay==0))? FALSE : TRUE);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_HWI2C_ConfigDelay_EX
/// @brief \b Function  \b Description: config delay parameter for START & STOP by every port
/// @param HWI2C_DelayConfig       \b IN: 
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////

MS_BOOL  MDrv_HWI2C_ConfigDelay_EX(void *pParam_Config)
{
    HWI2C_DelayConfig *pConfig = NULL;
    MS_U8 u8Port;
    MS_U32 u32StartDelay;
    MS_U32 u32StopDelay;
    
    if (!pParam_Config)
	return FALSE;

    pConfig = (HWI2C_DelayConfig *) pParam_Config;
	
    u8Port = pConfig->u8Port;
    u32StartDelay = pConfig->u32StartDelay;
    u32StopDelay = pConfig->u32StopDelay;

    HWI2C_DBG_FUNC();
    HWI2C_DBG_INFO("%s-->Port:%d\n", __FUNCTION__, u8Port);
	
    if(u32StartDelay < HWI2C_STARTDLY)
    {
        if(u32StartDelay==0)
            HWI2C_DBG_ERR("START Delay 0(us) will cause accessing error !\n");
        else
            HWI2C_DBG_ERR("START Delay %d(us) will risk accessing error !\n",(int)u32StartDelay);
    }

    if(u32StopDelay < HWI2C_STOPDLY)
    {
        if(u32StopDelay==0)
            HWI2C_DBG_ERR("STOP Delay 0(us) will cause accessing error !\n");
        else
            HWI2C_DBG_ERR("STOP Delay %d(us) will risk accessing error !\n",(int)u32StopDelay);
    }

    if (u8Port >= HWI2C_PORTS)
    {
        HWI2C_DBG_ERR("Port:%d  is error Port setting will risk accessing error !\n",(int)u8Port);
        return FALSE;
    }

    HWI2C_MUTEX_LOCK(u8Port);
    g_u32StartDelay[u8Port] = u32StartDelay;
    g_u32StopDelay[u8Port] = u32StopDelay;
    HWI2C_DBG_INFO("u32StartDelay: %d(us)\n", (int)g_u32StartDelay[u8Port]);
    HWI2C_DBG_INFO("u32StopDelay: %d(us)\n", (int)g_u32StopDelay[u8Port]);
    HWI2C_MUTEX_UNLOCK(u8Port);

    return (((u32StartDelay==0)||(u32StopDelay==0))? FALSE : TRUE);
}

MS_U16 MDrv_HWI2C_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;
    MS_U8 u8Port = 0;

    if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		_gbInit = false;
        HWI2C_DBG_INFO("HWI2C_MUTEX_DELETE!\n");
        for (u8Port = 0; u8Port < (MS_U8)HWI2C_PORTS; u8Port++)
        {
            if (-1 != g_s32HWI2CMutex[u8Port])
            {
                HWI2C_MUTEX_DELETE(u8Port);
            }
            g_s32HWI2CMutex[u8Port] = -1;
        }
        HWI2C_CONTROL_MUTEX_DELETE();
        g_s32HWI2C_ControlMutex = -1;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            /* Support HW IIC interrupt mode */
            #ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
            HAL_HWI2C_Resume_Flag(TRUE);
            #endif
            MDrv_HWI2C_Init(gp_HWI2CinitCfg);
			_prev_u16PowerState = u16PowerState;
			u16Return = 1;//RESUME_OK;
		}
		else
		{
			HWI2C_DBG_ERR("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = 3;//SUSPEND_FAILED;
		}
	}
	else
	{
		HWI2C_DBG_ERR("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = FALSE;
	}
		
	return u16Return;// for success
}
		
#if (HWI2C_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_HWI2C_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20HWI2COpened)
    {
#if (HWI2C_KERNEL_MODE)
        if(UtopiaOpen(MODULE_HWI2C|KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
#else
        if(UtopiaOpen(MODULE_HWI2C, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
#endif
        {
            //HWI2C_DBG_ERR("Open HWI2C fail\n");
            printf("HWI2C Open fail \n");
    		return FALSE;
        }
        bU20HWI2COpened = TRUE;
    }
    return TRUE;
}
#endif
 
MS_BOOL MDrv_HWI2C_Start(void)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_Start,(MS_U32*)NULL);
    return (MS_BOOL)u32Ret;    
#else
     return MDrv_HWI2C_Start_U2K();
#endif
}

MS_BOOL MDrv_HWI2C_Stop(void)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_Stop,(MS_U32*)NULL);
    return (MS_BOOL)u32Ret;    
#else
     return MDrv_HWI2C_Stop_U2K();
#endif
}

MS_BOOL MDrv_HWI2C_SelectPort(HWI2C_PORT ePort)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    PHWI2C_PRIVATE_PARAM_SelectPort pHwi2cParam_SelectPort = NULL;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    pHwi2cParam_SelectPort = (PHWI2C_PRIVATE_PARAM_SelectPort)malloc(sizeof(HWI2C_PRIVATE_PARAM_SelectPort));
    pHwi2cParam_SelectPort->ePort=ePort;
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_SelectPort,(MS_U32*)pHwi2cParam_SelectPort);
    free(pHwi2cParam_SelectPort);
    return (MS_BOOL)u32Ret;    
#else
     return MDrv_HWI2C_SelectPort_U2K(ePort);
#endif
}

MS_BOOL MDrv_HWI2C_SetClk(HWI2C_CLKSEL eClk)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    PHWI2C_PRIVATE_PARAM_SetClk pHwi2cParam_SetClk = NULL;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    pHwi2cParam_SetClk = (PHWI2C_PRIVATE_PARAM_SetClk)malloc(sizeof(HWI2C_PRIVATE_PARAM_SetClk));
    pHwi2cParam_SetClk->eClk=eClk;
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_SetClk,(MS_U32*)pHwi2cParam_SetClk);
    free(pHwi2cParam_SetClk);
    return (MS_BOOL)u32Ret;    
#else
     return MDrv_HWI2C_SetClk_U2K(eClk);
#endif
}

MS_BOOL MDrv_HWI2C_WriteBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    MS_BOOL bResult;
    PHWI2C_PRIVATE_PARAM_WriteBytes pHwi2cParam_WriteBytes = NULL;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    pHwi2cParam_WriteBytes = (PHWI2C_PRIVATE_PARAM_WriteBytes)malloc(sizeof(HWI2C_PRIVATE_PARAM_WriteBytes));
    pHwi2cParam_WriteBytes->u16SlaveCfg=u16SlaveCfg;
    pHwi2cParam_WriteBytes->uAddrCnt=uAddrCnt;
    pHwi2cParam_WriteBytes->pRegAddr=pRegAddr;
    pHwi2cParam_WriteBytes->uSize=uSize;
    pHwi2cParam_WriteBytes->pData=pData;
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_WriteBytes,(void*)pHwi2cParam_WriteBytes);
    bResult = pHwi2cParam_WriteBytes->bReturn;
    free(pHwi2cParam_WriteBytes);
    return (MS_BOOL)bResult;    
#else
    return MDrv_HWI2C_WriteBytes_U2K(u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
#endif	
}

MS_BOOL MDrv_HWI2C_ReadBytes(MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
#if (HWI2C_UTOPIA20)
    MS_U32 u32Ret;
    MS_BOOL bResult;
    PHWI2C_PRIVATE_PARAM_ReadBytes pHwi2cParam_ReadBytes = NULL;
    if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
        return 0;	
    pHwi2cParam_ReadBytes = (PHWI2C_PRIVATE_PARAM_ReadBytes)malloc(sizeof(HWI2C_PRIVATE_PARAM_ReadBytes));
    pHwi2cParam_ReadBytes->u16SlaveCfg=u16SlaveCfg;
    pHwi2cParam_ReadBytes->uAddrCnt=uAddrCnt;
    pHwi2cParam_ReadBytes->pRegAddr=pRegAddr;
    pHwi2cParam_ReadBytes->uSize=uSize;
    pHwi2cParam_ReadBytes->pData=pData;
    u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_ReadBytes,(void*)pHwi2cParam_ReadBytes);
    bResult = pHwi2cParam_ReadBytes->bReturn;
    free(pHwi2cParam_ReadBytes);
    return (MS_BOOL)bResult;    
#else
    return MDrv_HWI2C_ReadBytes_U2K(u16SlaveCfg,uAddrCnt,pRegAddr,uSize,pData);
#endif	
}


MS_BOOL MDrv_HWI2C_Init(HWI2C_UnitCfg *psCfg)
{
#if (HWI2C_UTOPIA20)
		MS_U32 u32Ret;
		PHWI2C_PRIVATE_PARAM_Init pHwi2cParam_Init = NULL;
		if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
			return 0;
		pHwi2cParam_Init = (PHWI2C_PRIVATE_PARAM_Init)malloc(sizeof(HWI2C_PRIVATE_PARAM_Init));
		pHwi2cParam_Init->psCfg = psCfg;
		u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_Init,(MS_U32*)pHwi2cParam_Init);
		free(pHwi2cParam_Init);
		return (MS_BOOL)u32Ret;
#else
		return MDrv_HWI2C_Init_U2K(psCfg);
#endif
}

MS_BOOL MDrv_HWI2C_GetConfig(HWI2C_UnitCfg *psCfg)
{
#if (HWI2C_UTOPIA20)
		MS_U32 u32Ret;
		PHWI2C_PRIVATE_PARAM_Init pHwi2cParam_Init = NULL;
		if (FALSE == _MDrv_HWI2C_CheckUtopia20Open(&pInstantHwi2c, 0, pAttributeHwi2c))
			return 0;
		pHwi2cParam_Init = (PHWI2C_PRIVATE_PARAM_Init)malloc(sizeof(HWI2C_PRIVATE_PARAM_Init));
		u32Ret = UtopiaIoctl(pInstantHwi2c,MDrv_CMD_HWI2C_GetConfig,pHwi2cParam_Init);
		psCfg=pHwi2cParam_Init->psCfg;
		free(pHwi2cParam_Init);
		return (MS_BOOL)u32Ret;
#else
#endif
    return true;
}

