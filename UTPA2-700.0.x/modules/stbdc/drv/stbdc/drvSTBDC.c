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
/// drvMVOP.c
/// @brief MPEG Video Output Processor (MVOP)
/// @author MStarSemi Inc.
///
/// Display controller(DC) unit (also called Video Output Processor(VOP)).
/// DC read DRAM MVD decode data and send it to back-end processor.
/// The frame rate could be programmed by registers.
/// The sync. generator(SG) generates the SYNC signal like HSYNC, VSYNC, FIELD.
/// It also generate the standard CCIR656 data.
///
/// Features
///  - Output mode : interlace or progressive
///  - Maximum Display resolution 1920x1080
///  - Support MVD interlace or progressive YUV420 image
///  - one read channel to MIU ,  each request always work with 16 words( 8Y8UV)
///  - high MIU read priority when UV buffer data depth under 32 or 16.
///  - CCIR656 support
///  - Horizontal, Vertical duplicate support.
///  - MVD Image size resize mode
///
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#if 0
#include "drvMVOP.h"

// Internal Definition
#include "regMVOP.h"
#include "halMVOP.h"
#else
#include "drvSTBDC.h"

// Internal Definition
#include "regSTBDC.h"
#include "halSTBDC.h"

#endif

#include "drvMMIO.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
//#define _ENABLE_DC1_FRAMESYNC //Notice: Scaler should disable auto-detection when using this.


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define ENABLE_EXTRA_BLANKING     0
// define MAX blanking area reserved for video shift down right for frame bufferless usage
#if ENABLE_EXTRA_BLANKING
    #define EXTRA_H_BLANKING          0
    #define EXTRA_V_BLANKING          64
#else
    #define EXTRA_H_BLANKING          0
    #define EXTRA_V_BLANKING          0
#endif

#define MVOP_VBlank_Min 45
#define MVOP_HBlank_Min 32

#define MVOP_VBlank               45
#define MVOP_HBlank_SD            150
#define MVOP_HBlank_HD            300
#define DC_BaseClock              86400 //86.4*1000

#define MIN_MVOP_FREQ             16000000ul
#define MAX_MVOP_FREQ             86400000ul
#define MVOP_FREQ_144M           144000000ul
#define MVOP_FREQ_160M           160000000ul

#define MVOP_MIN_FRAMERATE       1000   //1 frame/second

#define MVOP_VBlank_Min_P 20
#define MVOP_VBlank_Min_I 35

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/// Version string
static MSIF_Version _drv_stbdc_version = {
    .DDI = { STBDC_DRV_VERSION },
};

static STBDC_DrvStatus _stSTBDCDrvStat;


static MS_U8 _u8DebugMsgLevel = 0;


#define STBDC_IsInit() (_stSTBDCDrvStat.bIsInit == TRUE)
#define STBDC_CheckIsInit()                                                     \
        do {                                                                   \
            if (!STBDC_IsInit())                                                \
            {                                                                  \
                STBDC_DBG(printf("%s: drvSTBDC is not init!\n", __FUNCTION__);)  \
                return;                                                        \
            }                                                                  \
        } while(0)


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define STBDC_DBG(x)                if (_u8DebugMsgLevel>0)  { x };
#define STBDC_TIMING_DBG(x)         if (_u8DebugMsgLevel>1)  { x };
#define STBDC_INPUT_DBG(x)          if (_u8DebugMsgLevel>2)  { x };
#define STBDC_DBG_VERBAL(x)         if (_u8DebugMsgLevel>3)  { x };
#define STBDC_ERR(x)                { x };

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void STBDC_InitVar(void)
{
    _stSTBDCDrvStat.bIsInit    = FALSE;
    _stSTBDCDrvStat.bIsEnable  = FALSE;
    _stSTBDCDrvStat.bIsUVShift = FALSE;
    _stSTBDCDrvStat.bIsBlackBG = FALSE;
    _stSTBDCDrvStat.bIsMonoMode = FALSE;
    _stSTBDCDrvStat.bIsSetTiming = FALSE;
}


//-------------------------------------------------------------------------------------------------
// Calculate MaxFactor.
// @return MaxFactor
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// This function shows the version of MVOP driver.
///
/// @param *ppVersion : pointer to the driver version
////////////////////////////////////////////////////////////////////////////////
STBDC_Result MDrv_STBDC_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_STBDC_FAIL;

    *ppVersion = &_drv_stbdc_version;
    return E_STBDC_OK;
}


/********************************************************************************/
/// Set detailed level of MVOP driver debug message (level)
/// This function is used for MVOP driver debug message setting
/// @param level  \b IN  level from 0 to 1
/********************************************************************************/
void MDrv_STBDC_SetDbgLevel(MS_U8 level)
{
    _u8DebugMsgLevel = level;
    return;
}

//-----------------------------------------------------------------------------
/// Get status of STBDC driver
/// @param -pstatus: driver status
/// @return - TRUE / FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_STBDC_GetStatus(STBDC_DrvStatus *pSTBDCStat)
{
    if( pSTBDCStat ==  NULL )
    {
        return FALSE;
    }

    pSTBDCStat->bIsInit = _stSTBDCDrvStat.bIsInit;
    pSTBDCStat->bIsEnable = _stSTBDCDrvStat.bIsEnable;
    pSTBDCStat->bIsUVShift = _stSTBDCDrvStat.bIsUVShift;
    pSTBDCStat->bIsBlackBG = _stSTBDCDrvStat.bIsBlackBG;
    pSTBDCStat->bIsMonoMode = _stSTBDCDrvStat.bIsMonoMode;
    return TRUE;
}




//-----------------------------------------------------------------------------
/// Set MVOP test pattern.
//-----------------------------------------------------------------------------
void MDrv_STBDC_SetPattern(MS_BOOL bEnable,STBDCPATGENCTRL ePatternGen)
{
    HAL_STBDC_SetPatGenCtrl(bEnable,ePatternGen);
}



//-----------------------------------------------------------------------------
/// Set MMIO Base for MVOP.
/// @return TRUE or FALSE
//-----------------------------------------------------------------------------
MS_BOOL MDrv_STBDC_SetMMIOMapBase(void)
{
    MS_U32 u32NonPMBankSize, u32NonPMBankAdd;
    if( !MDrv_MMIO_GetBASE( &u32NonPMBankAdd, &u32NonPMBankSize, MS_MODULE_MVOP))
    {
        STBDC_DBG(printf("MDrv_STBDC_SetMMIOMapBase IOMap failure\n"););
        return FALSE;
    }
    else
    {
        STBDC_DBG(printf("MDrv_STBDC_SetMMIOMapBase u32RiuBaseAdd = %lx\n", u32NonPMBankAdd););
    }
    HAL_STBDC_RegSetBase(u32NonPMBankAdd);
    return TRUE;
}

/******************************************************************************/
/// Initialize MVOP hardware and set it to hardwire mode
/******************************************************************************/
void MDrv_STBDC_Init(void)
{
    STBDC_InitVar();

    if(MDrv_STBDC_SetMMIOMapBase() == FALSE)
    {
        return;
    }


}

/******************************************************************************/
/// Exit MVOP, turn off clock
/******************************************************************************/
void MDrv_STBDC_Exit(void)
{
    STBDC_CheckIsInit();
}


/******************************************************************************/
/// Enable and Disable STBDC
/// @param bEnable \b IN
///   - # TRUE  Enable
///   - # FALSE Disable and reset
/******************************************************************************/
void MDrv_STBDC_Enable ( MS_BOOL bEnable )
{
    STBDC_CheckIsInit();


}


/******************************************************************************/
/// Get if STBDC is enable or not.
/// @param pbEnable \b OUT
///   - # TRUE  Enable
///   - # FALSE Disable
/******************************************************************************/
STBDC_Result MDrv_STBDC_GetIsEnable (MS_BOOL* pbEnable)
{
    if (!pbEnable || !STBDC_IsInit())
    {
        return E_STBDC_FAIL;
    }

    *pbEnable = 0;
    return E_STBDC_OK;
}

//-----------------------------------------------------------------------------
/// Configure MVOP input.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
STBDC_Result MDrv_STBDC_SetInputCfg (STBDC_InputSel in, STBDC_InputCfg* pCfg)
{
    if (!STBDC_IsInit())
    {
        STBDC_DBG(printf("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_STBDC_FAIL;
    }
    
    return HAL_STBDC_SetInputCfg(in, pCfg);
}




//-----------------------------------------------------------------------------
/// Configure MVOP output.
/// @return MVOP_Result
//-----------------------------------------------------------------------------
STBDC_Result MDrv_STBDC_SetOutputCfg(STBDC_VidStat *pstVideoStatus, MS_BOOL bEnHDup)
{
    if (!STBDC_IsInit())
    {
        STBDC_DBG(printf("%s: driver is not initialized!\n", __FUNCTION__);)
        return E_STBDC_FAIL;
    }

   return E_STBDC_OK;
}









