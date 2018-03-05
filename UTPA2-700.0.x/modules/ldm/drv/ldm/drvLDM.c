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
/// file    drvLDM.c
/// @brief  LDM Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "MsOS.h"
#include "drvLDM.h"
#include "ULog.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#include "regLDM.h"
#include "halLDM.h"
#include "drvMMIO.h"
#include "utopia.h"

#if defined(MSOS_TYPE_LINUX)
#include <sys/ioctl.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define LDM_MODULE_NAME       "/dev/localdimming"
#define TAG_LDM "LDM"

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
static int LDM_fd = 0;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static EN_LDM_DEBUG_LEVEL gLDMDbgLevel = E_LDM_DEBUG_LEVEL_ERROR;
static MS_BOOL gbLDMInitialized = FALSE;
static MS_U8 gu8LDMStatus;
static MS_U8 gLDMTemp;

static MS_BOOL bLdm_Inited = FALSE;
static MS_S32 _gs32LDM_Mutex;
//LDM mutex wait time
#define LDM_MUTEX_WAIT_TIME    3000

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define LDM_DBG_FUNC()           if (gLDMDbgLevel >= E_LDM_DEBUG_LEVEL_ALL) \
                                     printf("[UTOPIA]:[LDM]: %s: %d: \n",__FUNCTION__,__LINE__);
#define LDM_DBG_INFO(msg...)     if (gLDMDbgLevel >= E_LDM_DEBUG_LEVEL_INFO ) \
                                     printf("[UTOPIA]:[LDM INFO]: %s: %d: ",__FUNCTION__,__LINE__);\
                                     printf(msg);
#define LDM_DBG_ERR(msg...)      if (gLDMDbgLevel >= E_LDM_DEBUG_LEVEL_ERROR) \
                                    printf("[UTOPIA]:[LDM ERROR]: %s: %d: ",__FUNCTION__,__LINE__);\
                                    printf(msg);
#define LDM_DBG_WARN(msg...)     if (gLDMDbgLevel >= E_LDM_DEBUG_LEVEL_WARNING) \
                                    printf("[UTOPIA]:[LDM WARNING]: %s: %d: ",__FUNCTION__,__LINE__);\
                                    printf(msg);



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//----------------------------------------------------------------
// MDrv_LDM_SetDbgLevel - Set debug level
/// @ingroup G_LDM_CONTROL
// @param: eLevel: debug level
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SetDbgLevel(EN_LDM_DEBUG_LEVEL eLevel)
{
    gLDMDbgLevel = eLevel;

#if defined(MSOS_TYPE_LINUX)
    int iResult = 0;

    //LDM_DBG_INFO("Debug level: %d \n", eLevel);
    if ((E_LDM_DEBUG_LEVEL_MAX <= eLevel)||(1 > eLevel))
    {
        printf("elevel is out or range: %d \n", eLevel);
        //LDM_DBG_ERR("elevel is out or range: %d \n", eLevel);
        return E_LDM_RET_PARAMETER_ERROR;
    }
    if (0 == LDM_fd)   //First time open
    {
        LDM_fd = open(LDM_MODULE_NAME, O_RDWR);
        if (LDM_fd < 0)
        {
            LDM_fd = 0;
            printf("Fail to open LDM Kernal Module\n");
            //LDM_DBG_ERR("Fail to open LDM Kernal Module\n");
            return E_LDM_RET_FAIL;
        }
    }

    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_DEBUG,&gLDMDbgLevel);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }
#endif
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}

//----------------------------------------------------------------
// MDrv_LDM_Enable - start local dimming algorithm
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is enable
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Enable(void)
{
#if defined(MSOS_TYPE_LINUX)
    int iResult = 0;

    LDM_DBG_FUNC();

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }
    if(E_LDM_STATUS_ENABLE == gu8LDMStatus)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_ENABLE);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM enable Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    gu8LDMStatus = E_LDM_STATUS_ENABLE;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
#else
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return E_LDM_RET_FAIL;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return E_LDM_RET_FAIL;
    }
    HAL_LDM_Enable(1);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return E_LDM_RET_SUCCESS;
#endif
}
//----------------------------------------------------------------
// MDrv_LDM_Disable - stop local dimming algorithm, send constant luminance  to led
/// @ingroup G_LDM_CONTROL
// @param: u8Lumma: constant luminance range from 00 to 255
// @return: E_LDM_RET_SUCCESS is disable
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Disable(MS_U8 u8Lumma)
{
#if defined(MSOS_TYPE_LINUX)
    int iResult = 0;

    LDM_DBG_INFO(" u8Lumma: %d \n", u8Lumma);

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }
    if(E_LDM_STATUS_DISNABLE == gu8LDMStatus)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    gLDMTemp = u8Lumma;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_DISABLE,&gLDMTemp);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    gu8LDMStatus = E_LDM_STATUS_DISNABLE;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
#else
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return E_LDM_RET_FAIL;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return E_LDM_RET_FAIL;
    }
    HAL_LDM_Enable(0);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return E_LDM_RET_SUCCESS;
#endif
}

#if defined(MSOS_TYPE_LINUX)
//----------------------------------------------------------------
// MDrv_LDM_GetData - get local dimming status
/// @ingroup G_LDM_CONTROL
// @param: void
// @return: EN_LDM_STATUS
//----------------------------------------------------------------
MS_U8 MDrv_LDM_GetStatus(void)
{
    LDM_DBG_FUNC();
    LDM_DBG_INFO("status: %d \n", gu8LDMStatus);

    return gu8LDMStatus;
}

//----------------------------------------------------------------
// MDrv_LDM_SetConfPath - Set config files path
/// @ingroup G_LDM_CONTROL
// @param: stPath: config files path in board or u-disk
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SetConfPath(ST_LDM_CONF_PATH stPath)
{
    int iResult = 0;
    ST_LDM_CONF_PATH stCusPath;

    if (!(stPath.aCusPath) || !(stPath.aCusPathU))
    {
        LDM_DBG_ERR("path id null \n");
        return E_LDM_RET_PARAMETER_ERROR;
    }
    LDM_DBG_INFO("set config path: %s, u-disk path:%s \n", stPath.aCusPath, stPath.aCusPathU);

    if (0 == LDM_fd)   //First time open
    {
        LDM_fd = open(LDM_MODULE_NAME, O_RDWR);
        if (LDM_fd < 0)
        {
            LDM_fd = 0;
            LDM_DBG_ERR("Fail to open LDM Kernal Module\n");
            return E_LDM_RET_FAIL;
        }
    }

    memset(&stCusPath,0,sizeof(ST_LDM_CONF_PATH));
    strcpy((char *)(stCusPath.aCusPath), (const char *)(stPath.aCusPath));
    strcpy((char *)(stCusPath.aCusPathU), (const char *)(stPath.aCusPathU));
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_CUS_PATH,&stCusPath);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM path Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}

//----------------------------------------------------------------
// MDrv_LDM_Init - Set  mmap address to register base
/// @ingroup G_LDM_CONTROL
// @param: phyAddr: local dimming mmap address in mmap.ini
// @return: E_LDM_RET_SUCCESS is inital
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Init(MS_PHY phyAddr)
{
    int iResult = 0;
    MS_PHY phyMMAPAddr = 0;

    LDM_DBG_INFO("init mmap address: 0x%llx \n", phyAddr);

    if(TRUE == gbLDMInitialized)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    if (0 == LDM_fd)   //First time open
    {
        LDM_fd = open(LDM_MODULE_NAME, O_RDWR);
        if (LDM_fd < 0)
        {
            LDM_fd = 0;
            LDM_DBG_ERR("Fail to open LDM Kernal Module\n");
            return E_LDM_RET_FAIL;
        }
    }

    phyMMAPAddr = phyAddr;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_INIT,&phyMMAPAddr);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    gbLDMInitialized = TRUE;
    gu8LDMStatus = E_LDM_STATUS_INIT;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}

//----------------------------------------------------------------
// MDrv_LDM_GetData - get LDF/LDB/SPI data pre frame in buffer
/// @ingroup G_LDM_CONTROL
// @param: stData:  data type and mmap address filled with the requied type
// @return: E_LDM_RET_SUCCESS is getted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_GetData(ST_LDM_GET_DATA *stData)
{
    int iResult = 0;
    ST_LDM_GET_DATA stGetData;

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }
    if (!(stData))
    {
        LDM_DBG_ERR("poiter is null! \n");
        return E_LDM_RET_PARAMETER_ERROR;
    }
    LDM_DBG_INFO("get enDataType :%d \n", stData->enDataType);

    memset(&stGetData,0,sizeof(ST_LDM_GET_DATA));
    stGetData.enDataType = stData->enDataType;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_GET_DATA,&stGetData);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    stData->phyAddr = stGetData.phyAddr;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_SetStrength - Set back light percent
/// @ingroup G_LDM_CONTROL
// @param: u8Percent: the percent ranged from 0 to 100
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SetStrength(MS_U8 u8Percent)
{
    int iResult = 0;

    LDM_DBG_INFO("strength u8Percent: %d \n", u8Percent);

    if ((100 <= u8Percent)||(0 > u8Percent))
    {
        LDM_DBG_ERR("Percent is out or range: %d \n", u8Percent);
        return E_LDM_RET_PARAMETER_ERROR;
    }
    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }

    gLDMTemp = (u8Percent*255)/100;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_SET_STRENGTH,&gLDMTemp);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_Suspend -go to sleep
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is suspend
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Suspend(void)
{
    int iResult = 0;

    LDM_DBG_FUNC();

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }
    if(E_LDM_STATUS_SUSPEND == gu8LDMStatus)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_SUSPEND);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    gu8LDMStatus = E_LDM_STATUS_SUSPEND;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_Resume - wake up from sleeping
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is resumed
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Resume(void)
{
    //int iResult = 0;

    LDM_DBG_FUNC();

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }

    if(E_LDM_STATUS_RESUME == gu8LDMStatus)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    /*  resume in kernel directly, since the booting time
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_RESUME);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }
    */

   gu8LDMStatus = E_LDM_STATUS_RESUME;
   LDM_DBG_INFO("OK\n");
   return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_DemoPattern - demo pattern from customer
/// @ingroup G_LDM_CONTROL
// @param: stPattern: demo type: turn on led, left-right half show
// @return: E_LDM_RET_SUCCESS is demonstrative
//----------------------------------------------------------------
MS_U8 MDrv_LDM_DemoPattern(ST_LDM_DEMO_PATTERN stPattern)
{
    int iResult = 0;
    ST_LDM_DEMO_PATTERN stDemoPattern;

    LDM_DBG_INFO("bOn: %d, type: %d, led number: %d \n", \
        stPattern.bOn, stPattern.enDemoPattern, stPattern.u16LEDNum);

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }

    memset(&stDemoPattern,0,sizeof(ST_LDM_DEMO_PATTERN));
    stDemoPattern.bOn = stPattern.bOn;
    stDemoPattern.enDemoPattern = stPattern.enDemoPattern;
    stDemoPattern.u16LEDNum = stPattern.u16LEDNum;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_DEMO_PATTERN,&stDemoPattern);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM Initial Configuration Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_DumpDRAM - Print LDF/LDB/SPI data
/// @ingroup G_LDM_CONTROL
// @param: eType: LDF/LDB/SPI
// @return: E_LDM_RET_SUCCESS is dumped
//----------------------------------------------------------------
MS_U8 MDrv_LDM_DumpDRAM(EN_LDM_GET_DATA_TYPE eType)
{
    int iResult = 0;
    EN_LDM_GET_DATA_TYPE eGetType;

    LDM_DBG_INFO("dump eType: %d \n", eType);

    if ((E_LDM_DATA_TYPE_MAX <= eType)||(E_LDM_DATA_TYPE_LDF > eType))
    {
        LDM_DBG_ERR("type is out or range: %d \n", eType);
        return E_LDM_RET_PARAMETER_ERROR;
    }
    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }

    eGetType = eType;
    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_DUMP_DRAM,&eGetType);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM dump dram Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_Deinit - stop local dimmg, free resource owned
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is deinit
//----------------------------------------------------------------
MS_U8 MDrv_LDM_Deinit(void)
{
    int iResult = 0;
    LDM_DBG_FUNC();

    if (0 == LDM_fd)
    {
        LDM_DBG_ERR("LDM file descriptor is not been opened\n");
        return E_LDM_RET_FAIL;
    }
    if(E_LDM_STATUS_DEINIT == gu8LDMStatus)
    {
        LDM_DBG_WARN(" reenter \n");
        return E_LDM_RET_SUCCESS;
    }

    iResult = ioctl(LDM_fd, MDRV_LDM_IOC_DEINIT);
    if(iResult != 0)
    {
        LDM_DBG_ERR("LDM deinit Fails iResult: %d \n", iResult);
        return E_LDM_RET_FAIL;
    }

    close(LDM_fd);
    LDM_fd = 0;
    gLDMDbgLevel = E_LDM_DEBUG_LEVEL_ERROR;
    gbLDMInitialized = FALSE;
    gu8LDMStatus = E_LDM_STATUS_DEINIT;
    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}


//----------------------------------------------------------------
// MDrv_LDM_SendCommads - the api to reserve
/// @ingroup G_LDM_CONTROL
// @param: pstCMD: pointer to command
// @param: u8Number: number of command
// @return: E_LDM_RET_SUCCESS is sent
//----------------------------------------------------------------
MS_U8 MDrv_LDM_SendCommads(ST_LDM_CMD_INFO* pstCMD,MS_U8 u8Number)
{
    int iResult = 0;
    MS_U8 u8Counter = 0;

    LDM_DBG_INFO("number: %d \n", u8Number);

    if (0 == LDM_fd)   //First time open
    {
        LDM_fd = open(LDM_MODULE_NAME, O_RDWR);
        if (LDM_fd < 0)
        {
            LDM_fd = 0;
            LDM_DBG_ERR("Fail to open LDM Kernal Module\n");
            return E_LDM_RET_FAIL;
        }
    }

    if (!pstCMD)
    {
        LDM_DBG_ERR("poiter is null! \n");
        return E_LDM_RET_PARAMETER_ERROR;
    }

    while(u8Number > u8Counter)
    {
        if(!((pstCMD[u8Counter]).pContent))
        {
            LDM_DBG_ERR("pstCMD[%d] poniter is null \n", u8Counter);
            u8Counter++;
            continue;
            //return E_LDM_RET_NOT_SUPPORTED;
        }

        //MS_S8 temp = -1;   //commands without any parameter
        //sCommandArray[i].u32CommandIndex = MDRV_LDM_IOC_ENABLE;
        //sCommandArray[i].pContent = &temp;
        iResult = *(MS_S8 *)((pstCMD[u8Counter]).pContent);
        LDM_DBG_INFO("send command u8Counter: %d, content:%d\n", u8Counter, iResult);
        if(-1 != *(MS_S8 *)((pstCMD[u8Counter]).pContent))
        {
            iResult = ioctl(LDM_fd, (pstCMD[u8Counter]).u32CommandIndex, (pstCMD[u8Counter]).pContent);
        }
        else
        {
            iResult = ioctl(LDM_fd, (pstCMD[u8Counter]).u32CommandIndex);
        }

        if(iResult != 0)
        {
            LDM_DBG_ERR("LDM send command Fails iResult: %d, u8Counter:%d \n", iResult, u8Counter);
            return E_LDM_RET_FAIL;
        }
        u8Counter++;
        usleep(900);
    }

    LDM_DBG_INFO("OK\n");
    return E_LDM_RET_SUCCESS;
}

#endif
//-------------------------------------------------------------------------------------------------
/// Description : set LDM bank base address
/// @param u8ClkHz \b IN:  back light period 60HZ or 120HZ
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetIOMapBase(MS_U8 u8ClkHz)
{
    MS_VIRT VirtNONPMBank = 0;
    MS_PHY  u32NONPMBankSize = 0;

    if(bLdm_Inited)
    {
         LDM_DBG_ERR("Fun:%s @@error: LDM Channel %d has been initiated \n",__FUNCTION__,u8ClkHz);
        return FALSE;
    }
    //create mutex for multi thread
    _gs32LDM_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex LDM", MSOS_PROCESS_SHARED);
    MS_ASSERT(_gs32LDM_Mutex >= 0);

    //get nonPM base bank addr
    if (!MDrv_MMIO_GetBASE( &VirtNONPMBank, &u32NONPMBankSize, MS_MODULE_HW))
    {
         LDM_DBG_ERR("Fun:%s @@error:IOMap failure to get DRV_MMIO_NONPM_BANK\n",__FUNCTION__);
        return FALSE;
    }

    HAL_LDM_MMIOConfig(VirtNONPMBank,u8ClkHz);

    bLdm_Inited = TRUE;
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Description : set LDF/ LDB/ SPI/ Edge2D base address
/// @param stLdmCfgAddr \b IN:  doublebuffer for  LDF index
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_Config_MmapAddr(ST_LDM_CFG_MMAP_ADDR *stLdmCfgAddr)
{
    if((stLdmCfgAddr == NULL) || (bLdm_Inited ==0))
    {
        LDM_DBG_ERR("Fun:%s @@error:param_in stLdmCfgAddr is NULL or LDM not inited!!!\n",__FUNCTION__);
        return FALSE;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error: ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }
    #if 0
    LDM_DBG_INFO("u32LDFAddr0_l = %lx \n",stLdmCfgAddr->u32LDFAddr0_l);
    LDM_DBG_INFO("u32LDFAddr0_r = %lx \n",stLdmCfgAddr->u32LDFAddr0_r);
    LDM_DBG_INFO("u32LDFAddr1_l = %lx \n",stLdmCfgAddr->u32LDFAddr1_l);
    LDM_DBG_INFO("u32LDFAddr1_r = %lx \n",stLdmCfgAddr->u32LDFAddr1_r);
    LDM_DBG_INFO("u32LDBAddr0_l = %lx \n",stLdmCfgAddr->u32LDBAddr0_l);
    LDM_DBG_INFO("u32LDBAddr0_r = %lx \n",stLdmCfgAddr->u32LDBAddr0_r);
    LDM_DBG_INFO("u32LDBAddr1_l = %lx \n",stLdmCfgAddr->u32LDBAddr1_l);
    LDM_DBG_INFO("u32LDBAddr1_r = %lx \n",stLdmCfgAddr->u32LDBAddr1_r);
    LDM_DBG_INFO("u32Edge2DAddr = %lx \n",stLdmCfgAddr->u32Edge2DAddr);
    LDM_DBG_INFO("u32DataOffset = %lx \n",stLdmCfgAddr->u32DataOffset);
    #endif
    //step 1: set LDF base mem address
    HAL_LDM_SetLDFAddr(0,stLdmCfgAddr->u32LDFAddr0_l,stLdmCfgAddr->u32LDFAddr0_r);
    HAL_LDM_SetLDFAddr(1,stLdmCfgAddr->u32LDFAddr1_l,stLdmCfgAddr->u32LDFAddr1_r);

    //step2: set LDB base mem address
    HAL_LDM_SetLDBAddr(0,stLdmCfgAddr->u32LDBAddr0_l,stLdmCfgAddr->u32LDBAddr0_r);
    HAL_LDM_SetLDBAddr(1,stLdmCfgAddr->u32LDBAddr1_l,stLdmCfgAddr->u32LDBAddr1_r);

    //step3: set Edge2D base mem address
    HAL_LDM_SetEdge2DAddr(stLdmCfgAddr->u32Edge2DAddr);

    //step4:set LEDBuf BaseOffset [spi data addr = LDB addr +LEDBuf BaseOffset]
    HAL_LDM_SetLEDBufBaseOffset(stLdmCfgAddr->u32DataOffset);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Set MIU PackOffset & Pack length [DMA load mem offset data ,ps:always offset=0]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8PackOffset \b IN: u8PackOffset
/// @param u8PackLength \b IN: DMA load mem data pack format size
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetMIUPackFormat(MS_U8 u8HWNum,MS_U8 u8PackOffset,MS_U8 u8PackLength)
{
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return FALSE;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_LDM_SetMIUPackOffset(u8HWNum,u8PackOffset);
    HAL_LDM_SetMIUPackLength(u8HWNum,u8PackLength);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Description : Set YoffEnd
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8YoffEnd \b IN: u8YoffEnd always =led height
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetYoffEnd(MS_U8 u8HWNum,MS_U8 u8YoffEnd)
{
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return FALSE;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }
    HAL_LDM_SetYoffEnd(u8HWNum,u8YoffEnd);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
/// Description : Set Back light height & width number
/// @param u8Height \b IN: LDMA Height = led Height number
/// @param u8Height \b IN: LDMA Width = led Width number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetBlLdmaSize(MS_U8 u8Height,MS_U8 u8Width)
{
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return FALSE;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }
    HAL_LDM_SetBlHeightDMA(u8Height);
    HAL_LDM_SetBlWidthDMA(u8Width);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma for LD
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetDmaEnable(MS_U8 u8HWNum, MS_BOOL bEnable)
{
    if(bLdm_Inited ==0)
    {
        LDM_DBG_ERR("Fun:%s @@error:LDM Should init !!!\n",__FUNCTION__);
        return FALSE;
    }
    if (FALSE == MsOS_ObtainMutex(_gs32LDM_Mutex, LDM_MUTEX_WAIT_TIME))
    {
        LDM_DBG_ERR("Fun:%s @@error:ENTRY fails!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_LDM_SetDmaEnable(u8HWNum,bEnable);

    MsOS_ReleaseMutex(_gs32LDM_Mutex);
    return TRUE;

}
