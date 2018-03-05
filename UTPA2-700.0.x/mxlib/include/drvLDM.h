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
/// @file   drvLDM.h
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_LDM Local dimming interface
    \ingroup  G_PERIPHERAL

    \brief

    Local dimming is introduced to achieve power saving and higher image contrast for LED backlight.


    <b>Features</b>

    - Less backlight, less energy
    - Compensation restores image luminance
    - Average 10~30% saving
    - Dark region becomes even darker
    - Light leakage problem

    <b> LDM Block Diagram: </b> \n
    \image html apiLD_pic.png

    <b> Operation Code Flow: </b> \n
    -# AP init, local dimming ready
    -# kernel run algorithm
    -# pm51 read led data from sharememory
    -# pm51 send luminance data to led via spi
    \image html apiLD_pic2.png

    \defgroup LDM_Init LDM Init API
    \ingroup LDM
    \defgroup LDM_Debug LDM Debug API
    \ingroup LDM
    \defgroup G_LDM_COMMON Common Task relative
    \ingroup  G_LDM
    \defgroup G_LDM_CONTROL Control relative
    \ingroup  G_LDM

*/


#ifndef _DRV_LDM_H_
#define _DRV_LDM_H_


#ifdef __cplusplus
extern "C"
{
#endif


#ifdef MDRV_LDM_C
#define MDRV_INTERFACE
#else
#define MDRV_INTERFACE extern
#endif

// use 'L' as magic number
#define MDRV_LDM_IOC_MAGIC                  ('L')
#define MDRV_LDM_IOC_INIT                   _IO(MDRV_LDM_IOC_MAGIC, 0)
#define MDRV_LDM_IOC_DEINIT                 _IO(MDRV_LDM_IOC_MAGIC, 1)
#define MDRV_LDM_IOC_ENABLE                 _IOW(MDRV_LDM_IOC_MAGIC, 2, MS_U32)
#define MDRV_LDM_IOC_DISABLE                _IOW(MDRV_LDM_IOC_MAGIC, 3, MS_U32)
#define MDRV_LDM_IOC_SUSPEND                _IOW(MDRV_LDM_IOC_MAGIC, 4, MS_U32)
#define MDRV_LDM_IOC_RESUME                 _IOW(MDRV_LDM_IOC_MAGIC, 5, MS_U32)
#define MDRV_LDM_IOC_SET_STRENGTH           _IOW(MDRV_LDM_IOC_MAGIC, 6, MS_U32)
#define MDRV_LDM_IOC_GET_DATA               _IOR(MDRV_LDM_IOC_MAGIC, 7, MS_U32)
#define MDRV_LDM_IOC_DEBUG                  _IOW(MDRV_LDM_IOC_MAGIC, 8, MS_U32)
#define MDRV_LDM_IOC_CUS_PATH               _IOW(MDRV_LDM_IOC_MAGIC, 9, MS_U32)
#define MDRV_LDM_IOC_DEMO_PATTERN           _IOW(MDRV_LDM_IOC_MAGIC, 10, MS_U32)
#define MDRV_LDM_IOC_DUMP_DRAM              _IOW(MDRV_LDM_IOC_MAGIC, 11, MS_U32)
#define MDRV_LDM_IOC_MAX_NR                 (12)



typedef enum
{
    E_LDM_STATUS_INIT = 1,
    E_LDM_STATUS_ENABLE = 2,
    E_LDM_STATUS_DISNABLE,
    E_LDM_STATUS_SUSPEND,
    E_LDM_STATUS_RESUME,
    E_LDM_STATUS_DEINIT,
} EN_LDM_STATUS;

typedef enum
{
    E_LDM_RET_SUCCESS = 0,
    E_LDM_RET_FAIL = 1,
    E_LDM_RET_NOT_SUPPORTED,
    E_LDM_RET_PARAMETER_ERROR,
    E_LDM_RET_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
} EN_LDM_RETURN;

typedef enum
{
    E_LDM_DEBUG_LEVEL_ERROR=0x01,
    E_LDM_DEBUG_LEVEL_WARNING=0x02,
    E_LDM_DEBUG_LEVEL_INFO=0x04,
    E_LDM_DEBUG_LEVEL_ALL=0x07,
    E_LDM_DEBUG_LEVEL_MAX
}EN_LDM_DEBUG_LEVEL;

typedef enum
{
    E_LDM_DATA_TYPE_LDF=0x01,
    E_LDM_DATA_TYPE_LDB=0x02,
    E_LDM_DATA_TYPE_SPI=0x03,
    E_LDM_DATA_TYPE_MAX
}EN_LDM_GET_DATA_TYPE;

typedef enum
{
    E_LDM_DEMO_PATTERN_SWITCH_SINGLE_LED=0x00,
    E_LDM_DEMO_PATTERN_LEFT_RIGHT_HALF=0x01,
    E_LDM_DEMO_PATTERN_MARQUEE=0x02,
    E_LDM_DEMO_PATTERN_LEFT_RIGHT_COLOR_SHELTER=0x03,
    E_LDM_DEMO_PATTERN_MAX
}EN_LDM_DEMO_PATTERN;


typedef struct __attribute__((packed))
{
    MS_U8 aCusPath[64];
    MS_U8 aCusPathU[64];
}ST_LDM_CONF_PATH;

typedef struct __attribute__((packed))
{
    EN_LDM_GET_DATA_TYPE enDataType;
    MS_PHY phyAddr;
}ST_LDM_GET_DATA;

typedef struct __attribute__((packed))
{
    MS_BOOL bOn;
    EN_LDM_DEMO_PATTERN enDemoPattern;
    MS_U16 u16LEDNum;
}ST_LDM_DEMO_PATTERN;

typedef struct __attribute__((packed))
{
    MS_U32 u32CommandIndex;
    void *pContent;
}ST_LDM_CMD_INFO;

typedef struct
{
    //for LDF addr
    MS_U32 u32LDFAddr0_l;   //double buffer0
    MS_U32 u32LDFAddr0_r;
    MS_U32 u32LDFAddr1_l;   //double buffer1
    MS_U32 u32LDFAddr1_r;
    //for LDB addr
    MS_U32 u32LDBAddr0_l;   //double buffer0
    MS_U32 u32LDBAddr0_r;
    MS_U32 u32LDBAddr1_l;   //double buffer1
    MS_U32 u32LDBAddr1_r;
    //for edge2D addr
    MS_U32 u32Edge2DAddr;
    //for spi data mem base
    MS_U32 u32DataOffset;
}__attribute__((packed))ST_LDM_CFG_MMAP_ADDR;

//----------------------------------------------------------------
// MDrv_LDM_SetDbgLevel - Set debug level
/// @ingroup G_LDM_CONTROL
// @param: eLevel: debug level
// @return: E_LDM_STATUS_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetDbgLevel(EN_LDM_DEBUG_LEVEL eLevel);

#if defined(MSOS_TYPE_LINUX)
//----------------------------------------------------------------
// MDrv_LDM_GetData - get local dimming status
/// @ingroup G_LDM_CONTROL
// @param: void
// @return: EN_LDM_STATUS
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_GetStatus(void);
//----------------------------------------------------------------
// MDrv_LDM_SetConfPath - Set config files path
/// @ingroup G_LDM_CONTROL
// @param: stPath: config files path in board or u-disk
// @return: E_LDM_STATUS_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetConfPath(ST_LDM_CONF_PATH stPath);

//----------------------------------------------------------------
// MDrv_LDM_Init - Set  mmap address to register base
/// @ingroup G_LDM_CONTROL
// @param: phyAddr: local dimming mmap address in mmap.ini
// @return: E_LDM_RET_SUCCESS is inital
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Init(MS_PHY phyAddr);

//----------------------------------------------------------------
// MDrv_LDM_Enable - start local dimming algorithm
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is enable
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Enable(void);

//----------------------------------------------------------------
// MDrv_LDM_Disable - stop local dimming algorithm, send constant luminance  to led
/// @ingroup G_LDM_CONTROL
// @param: u8Lumma: constant luminance range from 00 to 255
// @return: E_LDM_RET_SUCCESS is disable
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Disable(MS_U8 u8Lumma);

//----------------------------------------------------------------
// MDrv_LDM_GetData - get LDF/LDB/SPI data pre frame in buffer
/// @ingroup G_LDM_CONTROL
// @param: stData:  data type and mmap address filled with the requied type
// @return: E_LDM_RET_SUCCESS is getted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_GetData(ST_LDM_GET_DATA *stData);

//----------------------------------------------------------------
// MDrv_LDM_SetStrength - Set back light percent
/// @ingroup G_LDM_CONTROL
// @param: u8Percent: the percent ranged from 0 to 100
// @return: E_LDM_RET_SUCCESS is setted
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SetStrength(MS_U8 u8Percent);

//----------------------------------------------------------------
// MDrv_LDM_Suspend -go to sleep
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is suspend
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Suspend(void);

//----------------------------------------------------------------
// MDrv_LDM_Resume - wake up from sleeping
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is resumed
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Resume(void);

//----------------------------------------------------------------
// MDrv_LDM_DemoPattern - demo pattern from customer
/// @ingroup G_LDM_CONTROL
// @param: stPattern: demo type: turn on led, left-right half show
// @return: E_LDM_RET_SUCCESS is demonstrative
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_DemoPattern(ST_LDM_DEMO_PATTERN stPattern);

//----------------------------------------------------------------
// MDrv_LDM_DumpDRAM - Print LDF/LDB/SPI data
/// @ingroup G_LDM_CONTROL
// @param: eType: LDF/LDB/SPI
// @return: E_LDM_RET_SUCCESS is dumped
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_DumpDRAM(EN_LDM_GET_DATA_TYPE eType);

//----------------------------------------------------------------
// MDrv_LDM_Deinit - stop local dimmg, free resource owned
/// @ingroup G_LDM_CONTROL
// @param: na
// @return: E_LDM_RET_SUCCESS is deinit
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_Deinit(void);

//----------------------------------------------------------------
// MDrv_LDM_SendCommads - the api to reserve
/// @ingroup G_LDM_CONTROL
// @param: pstCMD: pointer to command
// @param: u8Number: number of command
// @return: E_LDM_RET_SUCCESS is sent
//----------------------------------------------------------------
MDRV_INTERFACE MS_U8 MDrv_LDM_SendCommads(ST_LDM_CMD_INFO* pstCMD,MS_U8 u8Number);
#endif
//-------------------------------------------------------------------------------------------------
/// Description : set LDF base address
/// @param u8ClkHz \b IN:  back light period 60HZ or 120HZ
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetIOMapBase(MS_U8 u8ClkHz);
//-------------------------------------------------------------------------------------------------
/// Description : set LDF base address
/// @param stLdmCfgAddr \b IN:  doublebuffer for  LDF index
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_Config_MmapAddr(ST_LDM_CFG_MMAP_ADDR *stLdmCfgAddr);
//-------------------------------------------------------------------------------------------------
/// Description : Set MIU PackOffset & Pack length [DMA load mem offset data ,ps:always offset=0]
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8PackOffset \b IN: u8PackOffset
/// @param u8PackLength \b IN: DMA load mem data pack format size
/// @return TRUE : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetMIUPackFormat(MS_U8 u8HWNum,MS_U8 u8PackOffset,MS_U8 u8PackLength);
//-------------------------------------------------------------------------------------------------
/// Description : Set YoffEnd
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param u8YoffEnd \b IN: u8YoffEnd always =led height
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetYoffEnd(MS_U8 u8HWNum,MS_U8 u8YoffEnd);
//-------------------------------------------------------------------------------------------------
/// Description : Set Back light height number
/// @param u8Height \b IN: LDMA Height = led Height number
/// @param u8Height \b IN: LDMA Width = led Width number
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetBlLdmaSize(MS_U8 u8Height,MS_U8 u8Width);
//-------------------------------------------------------------------------------------------------
/// Description : Enable ldma for LD
/// @param u8HWNum \b IN: LDMA Hw number index
/// @param bEnable \b IN: set DMA for local dimming enable or disable
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_LDM_SetDmaEnable(MS_U8 u8HWNum, MS_BOOL bEnable);
#ifdef __cplusplus
}
#endif

#endif // _DRV_LDM_H_